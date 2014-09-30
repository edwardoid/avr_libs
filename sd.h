#ifndef SD_H
#define SD_H

#include "my_types.h"
#include "spi.h"
#include "debug.h"
#include "sd_cmd.h"

#define TOKEN_WAIT_MAX_TIMEOUT 1000
#define SD_CMD_RESP_MAX_TIMEOUT 3000
#define SD_IDLE_WAIT_MAX_TIMEOUT 3000

#define SD_R1	1
#define SD_R1B	2
#define SD_R2	3
#define SD_R3	4

#define SD_CRC 0x95

#define SD_UNKNOWN_TYPE 0
#define SD_MMC	(1 << 0)
#define SD_SD	(1 << 2)
#define SD_SDHC (1 << 3)

int8_t	sd_spi_read_byte(port_ptr_t port, uint8_t pin)
{
	spi_write_byte_ss(0xff, pin, port);
	return SPDR;
}

int8_t	sd_responce_lenght(uint8_t resp)
{
	switch(resp)
	{
		case SD_R1:
		case SD_R1B:
		{
			return 1;
		}
		
		case SD_R2:
		{
			return 2;
		}
		
		case SD_R3:
		{
			return 5;
		}
		
		default: {}
	}
	
	return -1;
}

uint8_t	sd_wait_for_token(uint8_t token, port_ptr_t port, uint8_t pin)
{
	uint32_t start = tu_millis();
	char data = 0xFF;
	do 
	{
		data = sd_spi_read_byte(port, pin);
		if(data == token && data != 0xFF)
			break;
	}
	while((tu_millis() - start) < TOKEN_WAIT_MAX_TIMEOUT);
	
	return data;
}

int8_t sd_send_command(uint8_t cmd, uint32_t args,port_ptr_t port, uint8_t pin, char* resp)
{
	if(resp != NULL)
	{
		*resp = 0xFF;
	}
	
	sd_spi_read_byte(port, pin);
	
	spi_write_byte_ss((char)cmd | 0x40, pin, port);
	spi_write_byte_ss((args >> 24) && 0xFF, pin, port);
	spi_write_byte_ss((args >> 16) && 0xFF, pin, port);
	spi_write_byte_ss((args >> 8) && 0xFF, pin, port);
	spi_write_byte_ss((args >> 0) && 0xFF, pin, port);
	
	if(cmd == SD_CMD_SEND_IF_COND)
		spi_write_byte_ss(0x87, pin, port);
	else
		spi_write_byte_ss(SD_CRC, pin, port);
	
	uint32_t rsp_start = tu_millis();
	char r = sd_spi_read_byte(port, pin);
	while((tu_millis() - rsp_start) < SD_CMD_RESP_MAX_TIMEOUT)
	{
		if(r != 0xFF)
		{
			if(resp != NULL)
				*resp = r;
			return 0;
		}
			
		r = sd_spi_read_byte(port, pin);
	}
	
	return 1;
}

int8_t sd_init(ddr_ptr_t ddr, port_ptr_t port, uint8_t pin)
{
	if(!test_bit(SPCR, SPE))
	{
		if(0 != spi_init_as_master(SPI_DIV_CLK_128, SPI_MODE_0))
		{
			DBG_ERR("Can not start SPI.");
			return -1;
		}		
	}
	
	uint8_t i = 0;
	for(i = 0; i < 10; ++i)
	{
		set_bit(*ddr, pin);
		set_bit(*port, pin);
		SPDR = 0xFF;
		while(!test_bit(SPSR, SPIF));
	}
	
	clear_bit(*port, pin);
	char response = 0;
	
	uint32_t op_start = tu_millis();
	
	DBG_MSG("Going to IDLE state");
	while(tu_millis() - op_start < SD_IDLE_WAIT_MAX_TIMEOUT)
	{
		if(0 != sd_send_command(SD_CMD_GO_IDLE_STATE, 0, port, pin, &response))
			return 1;
		if(response == 1)
			break;
	}
	
	if(response == 1)
	{
		DBG_ERR("Can't go to IDLE state");
		return 1;
	}
	
	
	while(response == 0)
	{
		if(0 != sd_send_command(SD_CMD_SEND_IF_COND, 0x000001AA, port, pin, &response))
			return 1;
	}
	
	int8_t type = SD_UNKNOWN_TYPE;
	if(test_bit(response, SD_R1_ILLEGAL_COMMAND))
	{
		uint8_t cmd = SD_CMD_SEND_OP_COND;

		if(0 != sd_send_command(SD_CMD_APP, 0, port, pin, NULL))
			return 1;
		
		if(test_bit(response, SD_R1_ILLEGAL_COMMAND))
		{
			DBG_MSG("MMC Card is attached");
			type = SD_MMC;
			cmd = SD_CMD_APP;
		}
		else
		{
			DBG_MSG("SD card is attached");
			type = SD_SD;
		}
		
		do 
		{
			if(0 != sd_send_command(cmd, 0, port, pin, &response))
				return 1;
				
		} while (test_bit(response, SD_R1_IDLE_STATE));
	}
	else
	{
		sd_spi_read_byte(port, pin);
		sd_spi_read_byte(port, pin);
		if((sd_spi_read_byte(port, pin) & 0x01) == 0)
			return 1; /* card operation voltage range doesn't match */
		if(sd_spi_read_byte(port, pin) != 0xaa)
			return 1; /* wrong test pattern */
		
		type = SD_SDHC;
		DBG_MSG("SDHC card is attached");
	}
	
	DBG_MSG("SD card is initialized...");
	
	return 0;
}


#endif // SD_H