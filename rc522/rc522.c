/*
	avr_libs
	Copyright (C) 2014  Edward Sargsyan

	avr_libs is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	avr_libs is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with avr_libs.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "rc522.h"

#if defined(F_RC522) && defined(F_SPI)

#include <spi/spi.h>
#include "rc522_registers.h"
#include "rc522_commands.h"
#include <my_types.h>
#include <bytes_manip.h>
#include <bitman.h>

#define RC522_PrescalerHi b1101
#define RC522_PrescalerLo b00111110

#define RC522_ReloadHi	b00011110
#define RC522_ReloadLo	b00000000

#define CARD_FOUND		1
#define CARD_NOT_FOUND	2
#define ERROR			3

void rc522_send(uint8_t reg, byte data, rc522_cfg_t* cfg)
{
	byte data_arr[2];
	data_arr[0] = (reg << 1) & 0x7E;
	data_arr[1] = data;
	spi_write_ss(data_arr, 2, cfg->ss.pin, cfg->ss.port);
}

byte rc522_read(uint8_t reg, rc522_cfg_t* cfg)
{
	byte data_arr[2];
	data_arr[0] = ((reg << 1) & 0x7E) | 0x80;
	data_arr[1] = 0;
	return spi_write_ss(data_arr, 2, cfg->ss.pin, cfg->ss.port);
}

void rc522_apply_bits(uint8_t reg, uint8_t mask, rc522_cfg_t* cfg)
{
	rc522_send(reg, (uint8_t)rc522_read(reg, cfg) | mask, cfg);
}

void rc522_reset_bits(uint8_t reg, uint8_t mask, rc522_cfg_t* cfg)
{
	rc522_send(reg, (uint8_t)rc522_read(reg, cfg) & (~mask), cfg);
}

void rc522_init(rc522_cfg_t* cfg)
{
	spi_init_as_master_ex((uint8_t*)&(cfg->ss.pin), 1, cfg->ss.ddr, SPI_DIV_CLK_16, SPI_MODE_0);
	rc522_send(RC522_CommandReg, RC522_CMD_SoftReset, cfg);
	rc522_send(RC522_TModeReg, RC522_TAuto | RC522_PrescalerHi, cfg);
	rc522_send(RC522_TPrescalerReg, RC522_PrescalerLo, cfg);
	rc522_send(RC522_TReloadRegHi, RC522_ReloadHi, cfg);
	rc522_send(RC522_TReloadRegLo, RC522_ReloadLo, cfg);
	rc522_send(RC522_TxASKReg, RC522_Force100ASK, cfg);
	rc522_send(RC522_ModeReg, RC522_TxWaitRF | RC522_PolMFin | RC522_CRCPreset0, cfg);
	byte val = rc522_read(RC522_TxControlReg, cfg);
	if(!test_bit(val, RC522_InvTx1REFn) || !test_bit(RC522_InvTx1REFn, val))
	{
		rc522_send(RC522_TxControlReg, val | RC522_InvTx1REFn | RC522_InvTx2REFn, cfg);
	}
}

uint8_t	rc522_request(uint8_t req_mode, uint8_t * tag_type, rc522_cfg_t* cfg)
{
	uint8_t  status;  
	uint32_t backBits;//The received data bits

	rc522_send(RC522_BitFramingReg, 0x07, cfg);//TxLastBists = BitFramingReg[2..0]	???
	
	tag_type[0] = req_mode;
	status = rc522_to_card(RC522_CMD_Transceive, tag_type, 1, tag_type, &backBits, cfg);

	if ((status != CARD_FOUND) || (backBits != 0x10))
	{    
		status = ERROR;
	}
   
	return status;
}

uint8_t rc522_to_card(uint8_t cmd, uint8_t *send_data, uint8_t send_data_len, uint8_t *back_data, uint32_t *back_data_len, rc522_cfg_t* cfg)
{
	uint8_t status = ERROR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;

    switch (cmd)
    {
        case RC522_CMD_MFAuthent:		//Certification cards close
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case RC522_CMD_Transceive:	//Transmit FIFO data
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
    }
   
    rc522_reset_bits(RC522_ComIrqReg, RC522_Set1, cfg);
    rc522_apply_bits(RC522_FIFOLevelReg, RC522_FlushBuffer, cfg);
    
	rc522_send(RC522_CommandReg, RC522_CMD_Idle, cfg);	//NO action; Cancel the current cmd???

	//Writing data to the FIFO
    for (i=0; i < send_data_len; i++)
    {   
		rc522_send(RC522_FIFODataReg, send_data[i], cfg);    
	}

	//Execute the cmd
	rc522_send(RC522_CommandReg, cmd, cfg);
    if (cmd == RC522_CMD_Transceive)
    {    
		rc522_apply_bits(RC522_BitFramingReg, RC522_StartSend, cfg);
	}   
    
	//Waiting to receive data to complete
	i = 2000;	//i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
    do 
    {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = rc522_read(RC522_ComIrqReg, cfg);
        i--;
    }
    while ((i != 0) && !( n & 0x01) && !( n & waitIRq));

	rc522_reset_bits(RC522_BitFramingReg, RC522_StartSend, cfg);
	
    if (i != 0)
    {   
    	byte errMask = RC522_BufferOvfl | RC522_CollErr | RC522_ParityErr | RC522_ProtocolErr; 
        if(!(rc522_read(RC522_ErrorReg, cfg) & errMask))
                {
            status = CARD_FOUND;
            if (n & irqEn & 0x01)
            {   
				status = CARD_NOT_FOUND;			//??   
			}

            if (cmd == RC522_CMD_Transceive)
            {
               	n = rc522_read(RC522_FIFOLevelReg, cfg);
              	lastBits = rc522_read(RC522_ControlReg, cfg) & 0x0111;
                if (lastBits)
                {   
					*back_data_len = (n-1) * 8 + lastBits;   
				}
                else
                {   
					*back_data_len = n*8;   
				}

                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAX_LEN)
                {   
					n = MAX_LEN;   
				}
				
				//Reading the received data in FIFO
                for (i = 0; i < n; i++)
                {   
					back_data[i] = rc522_read(RC522_FIFODataReg, cfg);    
				}
            }
        }
        else
        {   
			status = ERROR;  
		}
        
    }
	 

    return status;
}

uint8_t rc522_get_card_serial(uint8_t * serial_out, rc522_cfg_t* cfg)
{
	uint8_t status;
    uint8_t i;
	uint8_t serNumCheck=0;
    uint32_t unLen;
    
	rc522_send(RC522_BitFramingReg, 0x00, cfg);		//TxLastBists = BitFramingReg[2..0]
 
    serial_out[0] = PICC_ANTICOLL;
    serial_out[1] = 0x20;
    status = rc522_to_card(RC522_CMD_Transceive, serial_out, 2, serial_out, &unLen, cfg);

    if (status == CARD_FOUND)
	{
		//Check card serial number
		for (i=0; i<4; i++)
		{   
		 	serNumCheck ^= serial_out[i];
		}
		if (serNumCheck != serial_out[i])
		{   
			status = ERROR;    
		}
    }
    return status;
}

#endif // F_RC522 && F_SPI
