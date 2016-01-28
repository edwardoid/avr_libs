#include "nrf24l01.h"
#include "spi.h"
#include "bitman.h"
#include "time_utils_delays.h"

nrf24l01_role_t Receiver = 0;
nrf24l01_role_t Transmitter = 1;

byte nrf24l01_send(nrf24l01_conf_t* dev, byte command, byte data)
{
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(command, dev->ss_pin, dev->ss_port); 
    byte res = spi_write_byte_ss(data, dev->ss_pin, dev->ss_port); 
    set_high(*dev->ss_port, dev->ss_pin);
	
    return res;
}

byte nrf24l01_read_register(nrf24l01_conf_t* dev, byte reg)
{
    return nrf24l01_send(dev, NRF24L01_R_REGISTER | (NRF24L01_REGISTER_MASK & reg), NRF24L01_NOP);
}

byte nrf24l01_write_register(nrf24l01_conf_t* dev, byte reg, byte value)
{
    return nrf24l01_send(dev, NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & reg), value );
}

byte nrf24l01_set_register_bit(nrf24l01_conf_t* dev, byte reg, byte bit)
{
    byte current = nrf24l01_read_register(dev, reg);
    current |= bit;
    nrf24l01_write_register(dev, reg, current);
    return current;
}

byte nrf24l01_unset_register_bit(nrf24l01_conf_t* dev, byte reg, byte bit)
{
    byte current = nrf24l01_read_register(dev, reg);
    current &= ~bit;
    nrf24l01_write_register(dev, reg, current);
    return current;
}

byte nrf24l01_init(nrf24l01_conf_t* dev, ddr_ptr_t ce, ddr_ptr_t ss)
{
    set_as_output(*ce, dev->ce_pin);
    set_low(*dev->ce_port, dev->ce_pin);
    
	set_as_output(*ss, dev->ss_pin);
    set_high(*dev->ss_port, dev->ss_pin);
    
    spi_init_as_master(SPI_DIV_CLK_8, SPI_MODE_0);
    spi_set_master_bit_first(1);
    set_low(*dev->ce_port, dev->ce_pin);
    
    tu_delay_ms(140);
    nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_EN_CRC | NRF24L01_CRCO | NRF24L01_PWR_UP); 
    tu_delay_ms(10);
	nrf24l01_set_retries(dev, 5, 15);
	nrf24l01_set_speed(dev, NRF24L01_1MBPS);
	nrf24l01_send(dev, NRF24L01_ACTIVATE, 0x73);
	nrf24l01_write_register(dev, NRF24L01_FEATURE, 0x0);
	nrf24l01_write_register(dev, NRF24L01_DYNPD, 0);
	nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_RX_DR | NRF24L01_TX_DS | NRF24L01_MAX_RT);
	nrf24l01_set_channel(dev, 76);
	nrf24l01_flush_all(dev);
	
	byte rx = nrf24l01_read_register(dev, NRF24L01_EN_RXADDR);
	nrf24l01_write_register(dev, NRF24L01_EN_RXADDR,  rx | NRF24L01_ERX_P0);
	return nrf24l01_read_register(dev, NRF24L01_CONFIG);
}

byte nrf24l01_get_status(nrf24l01_conf_t* dev)
{
	return spi_write_byte_ss(NRF24L01_NOP, dev->ss_pin, dev->ss_port);
}

byte nrf24l01_set_retries(nrf24l01_conf_t* dev, uint8_t delay, uint8_t count)
{
	byte ard = ((delay & 0x0f) << NRF24L01_ARD);
	byte arc = ((count & 0x0f) << NRF24L01_ARC);
	nrf24l01_write_register(dev, NRF24L01_SETUP_RETR,  ard | arc);	
}

byte nrf24l01_set_speed(nrf24l01_conf_t* dev, uint8_t speed)
{
	byte reg = nrf24l01_read_register(dev, NRF24L01_RF_SETUP);
	switch(speed)
	{
		case NRF24L01_250KBPS:
		{
			set_bit(reg, NRF24L01_RF_PWR_LOW);
			clear_bit(reg, NRF24L01_RF_PWR_HIGH);
			dev->transmission_delay = 155;
			break;
		}
		case NRF24L01_1MBPS:
		{
			clear_bit(reg, NRF24L01_RF_PWR_LOW);
			clear_bit(reg, NRF24L01_RF_PWR_HIGH);
			dev->transmission_delay = 85;
			break;
		}
		case NRF24L01_2MBPS:
		{
			clear_bit(reg, NRF24L01_RF_PWR_LOW);
			set_bit(reg, NRF24L01_RF_PWR_HIGH);
			dev->transmission_delay = 65;
			break;
		}
		default:{
		}
	}
	nrf24l01_write_register(dev, NRF24L01_RF_SETUP, reg);
	return nrf24l01_read_register(dev, NRF24L01_RF_SETUP) == reg;
}

byte nrf24l01_get_speed(nrf24l01_conf_t* dev)
{
	byte speed = nrf24l01_read_register(dev, NRF24L01_RF_SETUP) & (_BV(NRF24L01_RF_PWR_LOW) | _BV(NRF24L01_RF_PWR_HIGH));
	switch(speed)
	{
		case _BV(NRF24L01_RF_PWR_LOW):
		{
			return NRF24L01_250KBPS;
		}
		case _BV(NRF24L01_RF_PWR_HIGH):
		{
			return NRF24L01_2MBPS;
		}
		case 0x0:
		{
			return NRF24L01_1MBPS;
		}
		
		default: {}
	}
	
	return NRF24L01_SPEED_UNDEFINED;
}
byte nrf24l01_set_channel(nrf24l01_conf_t* dev, uint8_t channel)
{
	return nrf24l01_write_register(dev, NRF24L01_RF_CH, channel < 125 ? channel : 125);
}

byte nrf24l01_get_channel(nrf24l01_conf_t* dev)
{
	return nrf24l01_read_register(dev, NRF24L01_RF_CH);
}

byte nrf24l01_flush(nrf24l01_conf_t* dev, nrf24l01_role_t role)
{
	return spi_write_byte_ss(role == Receiver ? NRF24L01_FLUSH_RX : NRF24L01_FLUSH_TX, dev->ss_pin, dev->ss_port);
}

void nrf24l01_enable_payload_acknowledge(nrf24l01_conf_t* dev, uint8_t enable)
{
	byte feature = nrf24l01_read_register(dev, NRF24L01_FEATURE);
	if (enable == NRF24L01_ENABLE)
	{
		set_bit(feature, NRF24L01_EN_ACK_PAY);
		set_bit(feature, NRF24L01_EN_DPL);
	}
	else
	{
		clear_bit(feature, NRF24L01_EN_ACK_PAY);
		clear_bit(feature, NRF24L01_EN_DPL);
	}
	
	nrf24l01_write_register(dev, NRF24L01_FEATURE, feature);
	
	if (enable = NRF24L01_ENABLE)
	{
		nrf24l01_set_register_bit(dev, NRF24L01_DYNPD, NRF24L01_DPL_P0);
	}
}

void nrf24l01_enable_dynamic_acknowledge(nrf24l01_conf_t* dev, uint8_t enable)
{
	if (enable == NRF24L01_ENABLE)
	{
		nrf24l01_set_register_bit(dev, NRF24L01_FEATURE, NRF24L01_EN_DYN_ACK);
	}
	else
	{
		nrf24l01_unset_register_bit(dev, NRF24L01_FEATURE, NRF24L01_EN_DYN_ACK);
	}
}