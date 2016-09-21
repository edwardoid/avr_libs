#include "nrf24l01.h"

#if defined(F_NRF24L01) && defined(F_SPI)

#include <usart/usart.h>
#include <spi/spi.h>
#include <stdio.h>
#include <string.h>
#include "bitman.h"
#include "time_utils_delays.h"
#include "debug.h"

#ifdef ENABLE_USART_DEBUGGING 

void print_addr(char* descriptor, byte* addr, uint8_t len)
{
    usart_write_string(descriptor);
    
    uint8_t i = 0;
    char hex[6];
    while(i < len)
    {
        sprintf(hex, "0x%02X ", addr[i++]);
        usart_write_string(hex);
    }
    usart_write_byte('\n');
}

#define P2_P5(dev, pipe, it, buff)                      \
    it = dev->pipe_addr_len - 1;                        \
    while(it-- > 0)                                     \
    {                                                   \
        sprintf(hex, "0x%02X", dev->pipe_1_addr[it]);   \
        usart_write_byte(' ');                          \
        usart_write_string(hex);                        \
    }                                                   \
    sprintf(hex, "0x%02X", dev->pipe_addrs[pipe]);      \
    usart_write_byte(' ');                              \
    usart_write_string(hex);                            \
    usart_write_byte('\n');                         

#endif

uint8_t nrf24l01_succeed(nrf24l01_conf_t* dev, byte result);
void nrf24l01_read_address(nrf24l01_conf_t* dev, byte reg, byte* buffer, uint8_t len);
void nrf24l01_print_status(nrf24l01_conf_t* dev);

void nrf24l01_print_addresses(nrf24l01_conf_t* dev)
{
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P0, dev->pipe_0_addr, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P1, dev->pipe_1_addr, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P2, & dev->pipe_addrs[0], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P3, & dev->pipe_addrs[1], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P4, & dev->pipe_addrs[2], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P5, & dev->pipe_addrs[3], 1);
   
     
    usart_write_string("PIPE0 ");
    uint8_t i = dev->pipe_addr_len > 5 ? 5 : dev->pipe_addr_len;
    dev->pipe_addr_len = i;
    char hex[6];
    while(i--)
    {
        sprintf(hex, "0x%02X ", dev->pipe_0_addr[i]);
        usart_write_string(hex);
    }
    
    usart_write_string("\nPIPE1 ");
    i = dev->pipe_addr_len;
    while(i--)
    {
        sprintf(hex, "0x%02X ", dev->pipe_1_addr[i]);
        usart_write_string(hex);
    }
    usart_write_byte('\n');
    int pp = 0;
    while(pp < 4 )
    {
        usart_write_string("PIPE");
        usart_write_byte('2' + pp);
        P2_P5(dev, pp, i, hex);
        ++pp;
    }
}

void nrf24l01_print_status(nrf24l01_conf_t* dev)
{
    byte status = nrf24l01_read_register(dev, NRF24L01_NOP);
    byte fifo = nrf24l01_read_register(dev, NRF24L01_FIFO_STATUS);
    byte config = nrf24l01_read_register(dev, NRF24L01_CONFIG);
    DBG_VALUE("STATUS:\t", status);
    DBG_VALUE_NNL("\tRX_DR\t", (status & NRF24L01_RX_DR) ? 1 : 0);
    DBG_VALUE_NNL("\tTX_DS\t", (status & NRF24L01_TX_DS) ? 1 : 0);
    DBG_VALUE_NNL("\tMAX_RT\t", (status & NRF24L01_MAX_RT) ? 1 : 0);
    DBG_VALUE_NNL("\tRX_P_NO\t", (status & NRF24L01_RX_P_NO) ? 1 : 0);
    DBG_VALUE("\tTX_FULL\t", (status & NRF24L01_TX_FULL_ST) ? 1 : 0);

    DBG_VALUE("FIFO_STATUS:", fifo);
    DBG_VALUE_NNL("\tTX_REUSE\t", (fifo & NRF24L01_TX_REUSE) ? 1 : 0);
    DBG_VALUE_NNL("\tTX_FULL\t", (fifo & NRF24L01_TX_FULL_FF) ? 1 : 0);
    DBG_VALUE_NNL("\tTX_EMPTY\t", (fifo & NRF24L01_TX_EMPTY) ? 1 : 0);
    DBG_VALUE_NNL("\tRX_FULL\t", (fifo & NRF24L01_RX_FULL) ? 1 : 0);
    DBG_VALUE("\tRX_EMPTY\t", (fifo & NRF24L01_RX_EMPTY) ? 1 : 0);

    DBG_VALUE("CONFIG:\t", config);
    DBG_VALUE_NNL("\tEN_CRC\t", (config & NRF24L01_EN_CRC) ? 1 : 0);
    DBG_VALUE_NNL("\tEN_CRC\t", (config & NRF24L01_CRCO) ? 1 : 0); 
    DBG_VALUE_NNL("\tPWR_UP\t", (config & NRF24L01_PWR_UP) ? 1 : 0);
    DBG_VALUE("\tPRIM_RX\t", (config & NRF24L01_PRIM_RX) ? 1 : 0);
};

//#endif // ENABLE_USART_DEBUGGING

byte nrf24l01_send(nrf24l01_conf_t* dev, byte command, byte data)
{
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(command, dev->ss_pin, dev->ss_port); 
    byte res = spi_write_byte_ss(data, dev->ss_pin, dev->ss_port); 
    set_high(*dev->ss_port, dev->ss_pin);
    
    return res;
}

void nrf24l01_read_address(nrf24l01_conf_t* dev, byte reg, byte* buffer, uint8_t len)
{
    set_low(*dev->ss_port, dev->ss_pin);
    //set_low(*dev->ss_port, dev->ss_pin);

    //// spi_set_master_bit_first(1);

    spi_write_byte_ss(NRF24L01_R_REGISTER | (reg), dev->ss_pin, dev->ss_port);
        
    while(len--)
    {
        *buffer++ = spi_write_byte_ss(0xFF, dev->ss_pin, dev->ss_port);
    }
    set_high(*dev->ss_port, dev->ss_pin);
}

void nrf24l01_write_pipe_rx_address(nrf24l01_conf_t* dev, uint8_t pipe)
{
    set_low(*dev->ss_port, dev->ss_pin);
    if (pipe == 0)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P0), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_0_addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    }
    else if(pipe == 1)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P1), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_1_addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 2)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P2), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_1_addr, dev->pipe_addr_len - 1, dev->ss_pin, dev->ss_port);
        spi_write_byte_ss(dev->pipe_addrs[0], dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 3)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P3), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_1_addr, dev->pipe_addr_len - 1, dev->ss_pin, dev->ss_port);
        spi_write_byte_ss(dev->pipe_addrs[1], dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 4)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P4), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_1_addr, dev->pipe_addr_len - 1, dev->ss_pin, dev->ss_port);
        spi_write_byte_ss(dev->pipe_addrs[2], dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 5)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P5), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_1_addr, dev->pipe_addr_len - 1, dev->ss_pin, dev->ss_port);
        spi_write_byte_ss(dev->pipe_addrs[3], dev->ss_pin, dev->ss_port);
    }
    set_high(*dev->ss_port, dev->ss_pin);
}

void nrf24l01_write_pipe_tx_address(nrf24l01_conf_t* dev, byte* addr, uint8_t length)
{
    set_low(*dev->ss_port, dev->ss_pin);

    spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P0), dev->ss_pin, dev->ss_port);
    spi_write_ss(addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    set_high(*dev->ss_port, dev->ss_pin);
    
    tu_delay_us(15);
    
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_TX_ADDR), dev->ss_pin, dev->ss_port);
    spi_write_ss(addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    
    set_high(*dev->ss_port, dev->ss_pin);
}

byte nrf24l01_read_register(nrf24l01_conf_t* dev, byte reg)
{
    //return nrf24l01_send(dev, NRF24L01_R_REGISTER | (NRF24L01_REGISTER_MASK & reg), NRF24L01_NOP);
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte(NRF24L01_R_REGISTER | (NRF24L01_REGISTER_MASK & reg));
    byte r = spi_write_byte(NRF24L01_NOP);
    set_high(*dev->ss_port, dev->ss_pin);
    return r;
}

byte nrf24l01_write_register(nrf24l01_conf_t* dev, byte reg, byte value)
{
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & reg));
    byte r = spi_write_byte(value);
    set_high(*dev->ss_port, dev->ss_pin);
    return r;
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

inline void nrf24l01_up(nrf24l01_conf_t* dev)
{
    nrf24l01_set_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PWR_UP);
    tu_delay_us(1600UL); // Standby-I mode
}

inline void nrf24l01_down(nrf24l01_conf_t* dev)
{
    set_low(*dev->ce_port, dev->ce_pin);
    nrf24l01_unset_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PWR_UP);
}

void nrf24l01_init_config(nrf24l01_conf_t* dst)
{
    dst->pipe_addr_len = 0;
    dst->transmission_delay = 85;
    dst->ce_port = NULL;
    dst->ce_pin = 0;

    dst->ss_port = NULL;
    dst->ss_pin = 0;

    dst->flags = 0x00;
}

byte nrf24l01_init(nrf24l01_conf_t* dev, ddr_ptr_t ce, ddr_ptr_t ss)
{
    set_as_output(*ce, dev->ce_pin);
    
    spi_init_as_master(SPI_DIV_CLK_128, SPI_MODE_0);
    spi_set_master_bit_first(0);

    set_as_output(*ss, dev->ss_pin);
    set_low(*dev->ce_port, dev->ce_pin);
    
    tu_delay_ms(100); // Undefined -> Power Down
    nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_EN_CRC | NRF24L01_CRCO);  // enable CRC16
    nrf24l01_print_status(dev);
    
    nrf24l01_set_retries(dev, 5, 15);
    if(nrf24l01_set_speed(dev, NRF24L01_250KBPS))
    {
        if(NRF24L01_P)
        {
            set_bit(dev->flags, NRF24L01_FLAG_MODEL);
        }
        else
        {
            nrf24l01_write_register(dev, NRF24L01_FEATURE, NRF24L01_EN_ACK_PAY | NRF24L01_EN_DYN_ACK);
        }
    }
    


    nrf24l01_set_speed(dev, NRF24L01_1MBPS);
    nrf24l01_write_register(dev, NRF24L01_DYNPD, 0);
    nrf24l01_write_register(dev, NRF24L01_FEATURE, 0);
    
    nrf24l01_set_channel(dev, 76);
    
    if (dev->pipe_addr_len == 0) // read
    {
        dev->pipe_addr_len = 5;
    }
    
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P0, dev->pipe_0_addr, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P1, dev->pipe_1_addr, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P2, & dev->pipe_addrs[0], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P3, & dev->pipe_addrs[1], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P4, & dev->pipe_addrs[2], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P5, & dev->pipe_addrs[3], 1);
    
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(NRF24L01_ACTIVATE, dev->ss_pin, dev->ss_port);
    spi_write_byte_ss(0x73, dev->ss_pin, dev->ss_port);
    set_high(*dev->ss_port, dev->ss_pin);
    
    nrf24l01_flush_rx(dev);
    nrf24l01_flush_tx(dev);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT);
    nrf24l01_print_status(dev);
    byte cfg = nrf24l01_read_register(dev, NRF24L01_CONFIG);
    nrf24l01_up(dev);
    return cfg;
}

void nrf24l01_set_address(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint8_t a5)
{
    if (pipe == 0)
    {
        dev->pipe_0_addr[0] = a1;
        dev->pipe_0_addr[1] = a2;
        dev->pipe_0_addr[2] = a3;
        dev->pipe_0_addr[3] = a4;
        dev->pipe_0_addr[4] = a5;
    }
    else if(pipe == 1)
    {
        dev->pipe_1_addr[0] = a1;
        dev->pipe_1_addr[1] = a2;
        dev->pipe_1_addr[2] = a3;
        dev->pipe_1_addr[3] = a4;
        dev->pipe_1_addr[4] = a5;
    }
    else
    {
        dev->pipe_1_addr[0] = a1;
        dev->pipe_1_addr[1] = a2;
        dev->pipe_1_addr[2] = a3;
        dev->pipe_1_addr[3] = a4;
        dev->pipe_1_addr[4] = a5;
        
        uint8_t last = a1;
        if (dev->pipe_addr_len == 3)
        {
            last = a3;
        }
        else if(dev->pipe_addr_len == 4)
        {
            last = a4;
        }
        else
        {
            last = a5;
        }
        dev->pipe_addrs[pipe - 2] = last;
    }
    nrf24l01_write_pipe_rx_address(dev, pipe);
}

byte nrf24l01_get_status(nrf24l01_conf_t* dev)
{
    return spi_write_byte_ss(NRF24L01_NOP, dev->ss_pin, dev->ss_port);
}

byte nrf24l01_set_retries(nrf24l01_conf_t* dev, uint8_t delay, uint8_t count)
{
    dev->transmission_delay = (uint16_t) delay * 250;
    return nrf24l01_write_register(dev, NRF24L01_SETUP_RETR,  (delay << 4) | count);    
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
            break;
        }
        case NRF24L01_1MBPS:
        {
            clear_bit(reg, NRF24L01_RF_PWR_LOW);
            clear_bit(reg, NRF24L01_RF_PWR_HIGH);
            break;
        }
        case NRF24L01_2MBPS:
        {
            clear_bit(reg, NRF24L01_RF_PWR_LOW);
            set_bit(reg, NRF24L01_RF_PWR_HIGH);
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

byte nrf24l01_flush_rx(nrf24l01_conf_t* dev)
{
    return spi_write_byte_ss(NRF24L01_FLUSH_RX, dev->ss_pin, dev->ss_port);
}

byte nrf24l01_flush_tx(nrf24l01_conf_t* dev)
{
    return spi_write_byte_ss(NRF24L01_FLUSH_TX, dev->ss_pin, dev->ss_port);
}

void nrf24l01_enable_payload_acknowledge(nrf24l01_conf_t* dev, uint8_t enable)
{
    byte feature = nrf24l01_read_register(dev, NRF24L01_FEATURE);
    if (enable == NRF24L01_ENABLE)
    {
        set_bit(feature, NRF24L01_EN_ACK_PAY);
        set_bit(feature, NRF24L01_EN_DPL);
        nrf24l01_enable_dynamic_payload_on_pipe(dev, 0, NRF24L01_ENABLE);
    }
    else
    {
        clear_bit(feature, NRF24L01_EN_ACK_PAY);
        clear_bit(feature, NRF24L01_EN_DPL);
    }
    
    nrf24l01_write_register(dev, NRF24L01_FEATURE, feature);
    
    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_set_register_bit(dev, NRF24L01_DYNPD, NRF24L01_DPL_P0);
    }
}

void nrf24l01_enable_auto_acknowledge_for_pipe(nrf24l01_conf_t* dev, uint8_t pipe,uint8_t enable)
{
    if(pipe == 0)
    {
        pipe = NRF24L01_ENAA_P0;
    }
    else if (pipe == 1)
    {
        pipe = NRF24L01_ENAA_P1;
    }
    else if (pipe == 2)
    {
        pipe = NRF24L01_ENAA_P2;
    }
    else if (pipe == 3)
    {
        pipe = NRF24L01_ENAA_P3;
    }
    else if (pipe == 4)
    {
        pipe = NRF24L01_ENAA_P4;
    }
    else if (pipe == 5)
    {
        pipe = NRF24L01_ENAA_P5;
    }
    else
    {
        return;
    }

    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_set_register_bit(dev, NRF24L01_EN_AA, pipe);
    }
}

void nrf24l01_enable_dynamic_acknowledge(nrf24l01_conf_t* dev, uint8_t enable)
{
    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_set_register_bit(dev, NRF24L01_FEATURE, NRF24L01_EN_DYN_ACK);
        set_bit(dev->flags, NRF24L01_FLAG_MODE_ACK);
    }
    else
    {
        nrf24l01_unset_register_bit(dev, NRF24L01_FEATURE, NRF24L01_EN_DYN_ACK);
        clear_bit(dev->flags, NRF24L01_FLAG_MODE_ACK);
    }
}

void nrf24l01_enable_dynamic_payload_feature(nrf24l01_conf_t* dev, uint8_t enable)
{
    if (enable ==  NRF24L01_ENABLE)
    {
        nrf24l01_set_register_bit(dev, NRF24L01_FEATURE, NRF24L01_EN_DPL);
    }
    else
    {
        byte dynpd = nrf24l01_read_register(dev, NRF24L01_DYNPD);    
        dynpd &= ~( NRF24L01_DPL_P0 | NRF24L01_DPL_P1 | NRF24L01_DPL_P2 | NRF24L01_DPL_P3 | NRF24L01_DPL_P4 | NRF24L01_DPL_P5);
        nrf24l01_write_register(dev, NRF24L01_DYNPD, dynpd);
        nrf24l01_unset_register_bit(dev, NRF24L01_FEATURE, NRF24L01_EN_DPL);
    }
}
void nrf24l01_enable_dynamic_payload_on_pipe(nrf24l01_conf_t* dev, uint8_t enable, uint8_t pipe)
{
    if(pipe == 0)
    {
        pipe = NRF24L01_DPL_P0;
    }
    else if (pipe == 1)
    {
        pipe = NRF24L01_DPL_P1;
    }
    else if (pipe == 2)
    {
        pipe = NRF24L01_DPL_P2;
    }
    else if (pipe == 3)
    {
        pipe = NRF24L01_DPL_P3;
    }
    else if (pipe == 4)
    {
        pipe = NRF24L01_DPL_P4;
    }
    else if (pipe == 5)
    {
        pipe = NRF24L01_DPL_P5;
    }
    else
    {
        return;
    }

    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_enable_auto_acknowledge_for_pipe(dev, pipe, enable);
        nrf24l01_enable_dynamic_payload_feature(dev, enable);
        nrf24l01_set_register_bit(dev, NRF24L01_DYNPD, pipe);
    }
    else
    {
        nrf24l01_unset_register_bit(dev, NRF24L01_DYNPD, pipe);
    }
}

uint8_t nrf24l01_dynamic_payload_enabled(nrf24l01_conf_t* dev, uint8_t pipe)
{
    if(pipe == 0)
    {
        pipe = NRF24L01_DPL_P0;
    }
    else if (pipe == 1)
    {
        pipe = NRF24L01_DPL_P1;
    }
    else if (pipe == 2)
    {
        pipe = NRF24L01_DPL_P2;
    }
    else if (pipe == 3)
    {
        pipe = NRF24L01_DPL_P3;
    }
    else if (pipe == 4)
    {
        pipe = NRF24L01_DPL_P4;
    }
    else if (pipe == 5)
    {
        pipe = NRF24L01_DPL_P5;
    }
    else
    {
        return NRF24L01_DISABLED;
    }
    
    return (nrf24l01_read_register(dev, NRF24L01_DYNPD) & pipe) ? NRF24L01_ENABLED : NRF24L01_DISABLED;
}

void nrf24l01_set_payload_size(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t size)
{
    if (pipe == 0)
    {
        nrf24l01_write_register(dev, NRF24L01_RX_PW_P0, size);
    }
    else if (pipe == 1)
    {
        nrf24l01_write_register(dev, NRF24L01_RX_PW_P1, size);
    }
    else if (pipe == 2)
    {
        nrf24l01_write_register(dev, NRF24L01_RX_PW_P2, size);
    }
    else if (pipe == 3)
    {
        nrf24l01_write_register(dev, NRF24L01_RX_PW_P3, size);
    }
    else if (pipe == 4)
    {
        nrf24l01_write_register(dev, NRF24L01_RX_PW_P4, size);
    }
    else if (pipe == 5)
    {
        nrf24l01_write_register(dev, NRF24L01_RX_PW_P5, size);
    }
    else
    {
        return;
    }
}

uint8_t nrf24l01_get_payload_size(nrf24l01_conf_t* dev, uint8_t pipe)
{
    uint8_t value = 0xFF;
    if (pipe == 0)
    {
        value = nrf24l01_read_register(dev, NRF24L01_RX_PW_P0);
    }
    else if (pipe == 1)
    {
        value = nrf24l01_read_register(dev, NRF24L01_RX_PW_P1);
    }
    else if (pipe == 2)
    {
        value = nrf24l01_read_register(dev, NRF24L01_RX_PW_P2);
    }
    else if (pipe == 3)
    {
        value = nrf24l01_read_register(dev, NRF24L01_RX_PW_P3);
    }
    else if (pipe == 4)
    {
        value = nrf24l01_read_register(dev, NRF24L01_RX_PW_P4);
    }
    else if (pipe == 5)
    {
        value = nrf24l01_read_register(dev, NRF24L01_RX_PW_P5);
    }
    value = (value << 3) >> 3;
    return value;
}

byte nrf24l01_acknowlede_available(nrf24l01_conf_t* dev)
{
    return !(nrf24l01_read_register(dev, NRF24L01_FIFO_STATUS) & NRF24L01_RX_EMPTY);
}

void nrf24l01_enable_crc(nrf24l01_conf_t* dev, uint8_t length)
{
    uint8_t config = nrf24l01_read_register(dev, NRF24L01_CONFIG);
    clear_bit(config, NRF24L01_CRCO);
    clear_bit(config, NRF24L01_EN_CRC);

    if ( length == NRF24L01_CRC_DISABLED )
    {
        //dev->crc = NRF24L01_CRC_DISABLED;
    }
    else if ( length == NRF24L01_CRC_8 )
    {
        set_bit(config, NRF24L01_EN_CRC);
    }
    else if(length == NRF24L01_CRC_16)
    {
        set_bit(config, NRF24L01_CRCO);
        set_bit(config, NRF24L01_EN_CRC);
    }

    nrf24l01_write_register(dev, NRF24L01_CONFIG, config);
}

void nrf24l01_disable_crc(nrf24l01_conf_t* dev)
{
    uint8_t config = nrf24l01_read_register(dev, NRF24L01_CONFIG) ;
    clear_bit(config, NRF24L01_CRCO);
    clear_bit(config, NRF24L01_EN_CRC);

    nrf24l01_write_register(dev, NRF24L01_CONFIG, config);
}

void nrf24l01_set_power_amplifier(nrf24l01_conf_t* dev, uint8_t value)
{
    if(value > NRF24L01_PA_MAX)
    {
        value = NRF24L01_PA_MAX;
    }

    byte setup = nrf24l01_read_register(dev, NRF24L01_RF_SETUP);    

    if(nrf24l01_is_plus_model(dev))
    {
        value = value << 5;
    }
    else
    {
        value = value<< 1;
    }

    setup &= ~value;
    setup |= value;

    nrf24l01_write_register(dev, NRF24L01_RF_SETUP, setup);
}

uint8_t nrf24l01_get_power_amplifier(nrf24l01_conf_t* dev)
{
    byte level = nrf24l01_read_register(dev, NRF24L01_RF_SETUP);    
    if(nrf24l01_is_plus_model(dev))
    {
        level = level >> 5;
    }
    else
    {
        level = level >> 1;
    }

    return level & ~0x3;
}

void nrf24l01_set_address_width(nrf24l01_conf_t* dev, uint8_t value)
{
    if(value > NRF24L01_ADDR_WIDTH_5_BYTES)
    {
        value = NRF24L01_ADDR_WIDTH_5_BYTES;
    }
    nrf24l01_write_register(dev, NRF24L01_SETUP_AW, value - 2);
}

uint8_t nrf24l01_get_address_width(nrf24l01_conf_t* dev)
{
    return nrf24l01_read_register(dev, NRF24L01_SETUP_AW) & 0x3;
}

void nrf24l01_enable_pipe(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t enable)
{
    uint8_t bit;
    if (pipe == 0)
    {
        bit = NRF24L01_ERX_P0;
    }
    else if (pipe == 1)
    {
        bit = NRF24L01_ERX_P1;
    }
    else if (pipe == 2)
    {
        bit = NRF24L01_ERX_P2;
    }
    else if (pipe == 3)
    {
        bit = NRF24L01_ERX_P3;
    }
    else if (pipe == 4)
    {
        bit = NRF24L01_ERX_P4;
    }
    else if (pipe == 5)
    {
        bit = NRF24L01_ERX_P5;
    }
    else
    {
        return;
    }
    
    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_set_register_bit(dev, NRF24L01_EN_RXADDR, bit);
    }
    else
    {
        nrf24l01_unset_register_bit(dev, NRF24L01_EN_RXADDR, bit);
    }
}

void nrf24l01_enable_all_pipes(nrf24l01_conf_t* dev, uint8_t enable)
{
    uint8_t value = NRF24L01_ERX_P0 | NRF24L01_ERX_P1 | NRF24L01_ERX_P2 | NRF24L01_ERX_P3 | NRF24L01_ERX_P4 | NRF24L01_ERX_P5;
    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_write_register(dev, NRF24L01_EN_RXADDR, value);
    }
    else
    {
        nrf24l01_write_register(dev, NRF24L01_EN_RXADDR, ~value);
    }
}
uint8_t nrf24l01_data_available(nrf24l01_conf_t* dev, uint8_t* pipe)
{
    if (nrf24l01_get_status(dev) & NRF24L01_RX_DR)
    {
        if (pipe != NULL)
        {
            *pipe = (nrf24l01_get_status(dev) >> NRF24L01_RX_P_NO) & 0x7;
        }
        
        return 1;
    }
    return 0;
}

void nrf24l01_retransmit_last(nrf24l01_conf_t* dev)
{
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_MAX_RT);
    spi_write_byte_ss(NRF24L01_REUSE_TX_PL, dev->ss_pin, dev->ss_port);    
}

void nrf24l01_wait_for_transmit(nrf24l01_conf_t* dev)
{    
    set_high(*dev->ce_port, dev->ce_pin);
    byte status;
    do 
    {
        status = nrf24l01_get_status(dev);
        tu_delay_ms(150);
    }
    while((status && NRF24L01_MAX_RT) && (status && NRF24L01_TX_DS));
    set_low(*dev->ce_port, dev->ce_pin);
}    

uint8_t nrf24l01_read_payload(nrf24l01_conf_t* dev, byte* buffer, uint8_t length)
{
    set_high(*dev->ce_port, dev->ce_pin);

    while(nrf24l01_rx_is_empty(dev))
    {
        tu_delay_us(10);
    }

    uint8_t pldWidth = nrf24l01_read_register(dev, NRF24L01_R_RX_PL_WID);
    if (pldWidth > 32)
    {
        return 0;
    }

    length = length < pldWidth ? length : pldWidth;

    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(NRF24L01_R_RX_PAYLOAD, dev->ss_pin, dev->ss_port);
    spi_set_master_bit_first(0);
    while(length--)
    {
        *buffer++ = spi_write_byte_ss(0xFF, dev->ss_pin, dev->ss_port); 
    }
    // spi_set_master_bit_first(1);
    nrf24l01_wait_for_transmit(dev);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT);
    return length;
}

void nrf24l01_prepare_for_read(nrf24l01_conf_t* dev, uint8_t pipe)
{
    nrf24l01_up(dev); // go to Standby-I mode
    tu_delay_ms(2UL);
    nrf24l01_flush_tx(dev);
    nrf24l01_set_address_width(dev, dev->pipe_addr_len);
    nrf24l01_write_pipe_rx_address(dev, pipe);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT);
    nrf24l01_set_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PRIM_RX); // Go to PRX mode
    nrf24l01_enable_pipe(dev, pipe, NRF24L01_ENABLE); // enable selected pipe to be ready to receive data :)
    
    set_high(*dev->ce_port, dev->ce_pin);
    tu_delay_us(1500UL);
}

uint8_t nrf24l01_read(nrf24l01_conf_t* dev, uint8_t pipe, byte* buffer, uint8_t length)
{
    return nrf24l01_read_payload(dev, buffer, length);
}

uint8_t nrf24l01_read_byte(nrf24l01_conf_t* dev, uint8_t pipe)
{
    byte buff;
    nrf24l01_read(dev, pipe, &buff, 1);
    return buff;
}

uint8_t nrf24l01_end_reading(nrf24l01_conf_t* dev, uint8_t pipe)
{
    uint8_t status = nrf24l01_end_reading_keep_irq(dev, pipe);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_MAX_RT | NRF24L01_TX_DS | NRF24L01_RX_DR); // default values are 1, see datasheet
    return status;
}

uint8_t nrf24l01_end_reading_keep_irq(nrf24l01_conf_t* dev, uint8_t pipe)
{
    set_low(*dev->ce_port, dev->ce_pin);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT);
    nrf24l01_enable_pipe(dev, pipe, NRF24L01_DISABLED);
    return nrf24l01_get_status(dev);
}

uint8_t nrf24l01_write_succeed(nrf24l01_conf_t* dev, byte result)
{
    return nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT); // default values are 1, see datasheet
}

uint8_t nrf24l01_write_payload(nrf24l01_conf_t* dev, byte* data, uint8_t length, byte cmd)
{
    byte p0[6];
    byte tx[6];
    memset(p0, 0xFF, dev->pipe_addr_len);
    memset(tx, 0xFF, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_TX_ADDR, tx, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P0, p0, dev->pipe_addr_len);
    
    print_addr("RX_P0:\t", p0, dev->pipe_addr_len);
    print_addr("TX ADDR:", tx, dev->pipe_addr_len);
    
    length = length < 32 ? length : 32;
    uint8_t blank = 0;

    if (nrf24l01_dynamic_payload_enabled(dev, 0))
    {
        blank = nrf24l01_get_payload_size(dev, 0) - length;
    }
    
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(cmd, dev->ss_pin, dev->ss_port); // write to correct payload storage

    spi_set_master_bit_first(0);
    spi_write_ss(data, length, dev->ss_pin, dev->ss_port);
    
    while(blank--)
    {
        spi_write_byte_ss(0x00, dev->ss_pin, dev->ss_port);
    }
    // spi_set_master_bit_first(1);
    set_high(*dev->ce_port, dev->ce_pin); // start transmission

    nrf24l01_print_status(dev);

    nrf24l01_wait_for_transmit(dev);

    return length - blank;
}

byte nrf24l01_prepare_for_write(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t enable_ackowledge)
{
    byte* addr = NULL;
    byte dummy[5];
    memset(&dummy[0], 0, 5);
    if (pipe == 0)
    {
        addr = dev->pipe_0_addr;        
    }
    else if (pipe == 1)
    {
        addr = dev->pipe_1_addr;
    }
    else if(pipe < 6)
    {
        memcpy(dummy, dev->pipe_1_addr, dev->pipe_addr_len - 1);
        dummy[dev->pipe_addr_len - 1] = dev->pipe_addrs[pipe - 2];
        addr = & dummy[0];
    }

    return nrf24l01_prepare_for_write_to_addr(dev, addr, dev->tx_payload_size, enable_ackowledge);
}

byte nrf24l01_prepare_for_write_to_addr(nrf24l01_conf_t* dev, byte* address, uint8_t payload_length, uint8_t enable_ackowledge)
{
    set_low(*dev->ce_port, dev->ce_pin);    
    nrf24l01_write_pipe_tx_address(dev, address, dev->pipe_addr_len);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_MAX_RT | NRF24L01_TX_DS | NRF24L01_RX_DR);
    
    nrf24l01_print_status(dev);
    nrf24l01_unset_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PRIM_RX); // go to TX mode
    
    nrf24l01_flush_tx(dev);
    nrf24l01_up(dev);
    return nrf24l01_get_status(dev);
}

uint8_t nrf24l01_write(nrf24l01_conf_t* dev, byte* data, uint8_t length)
{

    if(test_bit(dev->flags, NRF24L01_FLAG_MODE_ACK))
    {
        return nrf24l01_write_payload(dev, data, length, NRF24L01_W_TX_PAYLOAD);
    }
    else
    {
        return nrf24l01_write_payload(dev, data, length, NRF24L01_W_TX_PAYLOAD_NO_ACK);
    }
    
    tu_delay_us(15 + dev->transmission_delay); // write and go to StandBy-II mode
}

uint8_t nrf24l01_end_writing(nrf24l01_conf_t* dev)
{
    //uint8_t status = nrf24l01_end_writing_keep_irq(dev);
    //nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_MAX_RT | NRF24L01_TX_DS | NRF24L01_RX_DR); // default values are 1, see datasheet
    set_low(*dev->ce_port, dev->ce_pin); // start transmission
    nrf24l01_down(dev);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT);
    return 0; //status;
}

uint8_t nrf24l01_end_writing_keep_irq(nrf24l01_conf_t* dev)
{
    nrf24l01_wait_for_transmit(dev);
    uint8_t status = nrf24l01_get_status(dev);
    nrf24l01_down(dev); // Go to power down mode
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_TX_DS | NRF24L01_RX_DR | NRF24L01_MAX_RT);
    return status;
}

uint8_t nrf24l01_succeed(nrf24l01_conf_t* dev, byte result)
{
    if ((result & NRF24L01_MAX_RT) /* ok */ || (result & NRF24L01_TX_DS) /* ok */)
    {
        if (dev->flags & NRF24L01_FLAG_MODE_ACK)
        {
            if (result & NRF24L01_RX_DR)
            {
                return 1; // OK!
            }        
        }
    }
    
    return 0;
}

#endif