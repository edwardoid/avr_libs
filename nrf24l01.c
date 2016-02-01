#include "nrf24l01.h"
#include "spi.h"
#include "bitman.h"
#include "time_utils_delays.h"
#include "debug.h"

#ifdef ENABLE_USART_DEBUGGING 

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

void nrf24l01_print_addresses(nrf24l01_conf_t* dev)
{
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P0, dev->pipe_0_addr, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P1, dev->pipe_1_addr, dev->pipe_addr_len);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P2, & dev->pipe_addrs[0], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P3, & dev->pipe_addrs[1], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P4, & dev->pipe_addrs[2], 1);
    nrf24l01_read_address(dev, NRF24L01_RX_ADDR_P5, & dev->pipe_addrs[3], 1);
    

    usart_write_string("PIPE0 ");
    uint8_t i = dev->pipe_addr_len;
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
    uint8_t status;
    spi_write_byte_ss(NRF24L01_R_REGISTER | (NRF24L01_REGISTER_MASK & reg), dev->ss_pin, dev->ss_port);
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
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_ERX_P0), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_0_addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 1)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_ERX_P1), dev->ss_pin, dev->ss_port);
        spi_write_ss(dev->pipe_1_addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 2)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_ERX_P2), dev->ss_port, dev->ss_pin);
        spi_write_byte_ss(dev->pipe_addrs[0], dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 3)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_ERX_P3), dev->ss_port, dev->ss_pin);
        spi_write_byte_ss(dev->pipe_addrs[1], dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 4)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_ERX_P4), dev->ss_port, dev->ss_pin);
        spi_write_byte_ss(dev->pipe_addrs[2], dev->ss_pin, dev->ss_port);
    }
    else if (pipe == 5)
    {
        spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_ERX_P5), dev->ss_port, dev->ss_pin);
        spi_write_byte_ss(dev->pipe_addrs[3], dev->ss_pin, dev->ss_port);
    }
    set_high(*dev->ss_port, dev->ss_pin);
}

void nrf24l01_write_pipe_tx_address(nrf24l01_conf_t* dev, byte* addr, uint8_t length)
{
    set_low(*dev->ss_port, dev->ss_pin);

    spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_RX_ADDR_P0), dev->ss_pin, dev->ss_port);
    spi_write_ss(addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    
    spi_write_byte_ss(NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & NRF24L01_TX_ADDR), dev->ss_pin, dev->ss_port);
    spi_write_ss(addr, dev->pipe_addr_len, dev->ss_pin, dev->ss_port);
    
    set_high(*dev->ss_port, dev->ss_pin);
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

inline void nrf24l01_up(nrf24l01_conf_t* dev)
{
    byte cfg = nrf24l01_read_register(dev, NRF24L01_CONFIG);
    if (cfg & NRF24L01_PWR_UP)
    {
        return;
    }
    nrf24l01_set_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PWR_UP);
    tu_delay_us(5);
}

inline void nrf24l01_down(nrf24l01_conf_t* dev)
{
    set_low(*dev->ce_port, dev->ce_pin);
    nrf24l01_unset_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PWR_UP);
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
    
    tu_delay_us(140);
    nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_EN_CRC | NRF24L01_CRCO); 
    nrf24l01_up(dev);
    tu_delay_us(10);
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
    nrf24l01_set_speed(dev, NRF24L01_250KBPS);
    nrf24l01_write_register(dev, NRF24L01_FEATURE, 0x0);
    nrf24l01_write_register(dev, NRF24L01_DYNPD, 0);
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_RX_DR | NRF24L01_TX_DS | NRF24L01_MAX_RT);
    nrf24l01_set_channel(dev, 76);
    nrf24l01_flush_tx(dev);
    nrf24l01_flush_rx(dev);
    
    byte rx = nrf24l01_read_register(dev, NRF24L01_EN_RXADDR);
    nrf24l01_write_register(dev, NRF24L01_EN_RXADDR,  rx | NRF24L01_ERX_P0);
    nrf24l01_set_role(dev, NRF24L01_ROLE_TRANSMITTER);

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
     
    for (int i = 0; i < 6; ++i)
    {
        if (dev->payload_lengths[i] != 0 && dev->payload_lengths[i] < 32)
        {
            nrf24l01_set_payload_size(dev, i, dev->payload_lengths[i]);
        }
    }
    return nrf24l01_read_register(dev, NRF24L01_CONFIG);
}

void nrf24l01_set_role(nrf24l01_conf_t* dev, uint8_t role)
{
    set_low(*dev->ce_port, dev->ce_pin);
    if (role == NRF24L01_ROLE_RECEIVER)
    {
        nrf24l01_up(dev);
        nrf24l01_set_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PRIM_RX);
        set_high(*dev->ce_port, dev->ce_pin);
        if (nrf24l01_read_register(dev, NRF24L01_FEATURE) & NRF24L01_EN_ACK_PAY)
        {
            nrf24l01_flush_tx(dev);
        }
    }
    else
    {
        tu_delay_us(dev->transmission_delay);
        if (nrf24l01_read_register(dev, NRF24L01_FEATURE) & NRF24L01_EN_ACK_PAY)
        {
            tu_delay_us(dev->transmission_delay);
            nrf24l01_flush_tx(dev);
        }
        
        nrf24l01_unset_register_bit(dev, NRF24L01_CONFIG, NRF24L01_PRIM_RX);
        nrf24l01_up(dev);
    }
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

void nrf24l01_enable_auto_acknowledge_for_all_pipes(nrf24l01_conf_t* dev, uint8_t enable)
{
    byte enaa = nrf24l01_read_register(dev, NRF24L01_EN_AA);
    byte pipes = NRF24L01_ENAA_P0 | NRF24L01_ENAA_P1 | NRF24L01_ENAA_P2 | NRF24L01_ENAA_P3 | NRF24L01_ENAA_P4 | NRF24L01_ENAA_P5;
    if (enable == enable)
    {
        enaa |= pipes;
    }
    else
    {
        enaa &= ~pipes;
    }

    nrf24l01_write_register(dev, NRF24L01_EN_AA, pipes);
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
    byte aa = 0xFF;
    
    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_enable_auto_acknowledge_for_pipe(dev, pipe, enable);
    }

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
        byte feature = nrf24l01_read_register(dev, NRF24L01_FEATURE);
        if (~(feature & NRF24L01_EN_DPL))
        {
            nrf24l01_enable_dynamic_payload_feature(dev, enable);
        }

        nrf24l01_set_register_bit(dev, NRF24L01_DYNPD, pipe);
    }
    else
    {
        nrf24l01_unset_register_bit(dev, NRF24L01_DYNPD, pipe);
    }
}

void nrf24l01_enable_dynamic_payload_on_all_pipes(nrf24l01_conf_t* dev, uint8_t enable)
{
    byte pipes = NRF24L01_DPL_P0 | NRF24L01_DPL_P1 | NRF24L01_DPL_P2 | NRF24L01_DPL_P3 | NRF24L01_DPL_P4 | NRF24L01_DPL_P5;
    byte dynpd = nrf24l01_read_register(dev, NRF24L01_DYNPD);
    
    if (enable == NRF24L01_ENABLE)
    {
        nrf24l01_enable_dynamic_payload_feature(dev, enable);
        nrf24l01_enable_auto_acknowledge_for_all_pipes(dev, enable);
        dynpd |= pipes;
    }
    else
    {
        dynpd &= ~pipes;
    }

    nrf24l01_write_register(dev, NRF24L01_DYNPD, dynpd);
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
    size %= 32;
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
    
    dev->payload_lengths[pipe] = size;
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

    // switch uses RAM (evil!)
    if ( length == NRF24L01_CRC_DISABLED )
    {
    //    nrf24l01_disable_crc(dev);
        dev->crc = NRF24L01_CRC_DISABLED;
    }
    else if ( length == NRF24L01_CRC_8 )
    {
        set_bit(config, NRF24L01_EN_CRC);
        dev->crc = NRF24L01_CRC_8;
    }
    else if(length == NRF24L01_CRC_16)
    {
        set_bit(config, NRF24L01_CRCO);
        set_bit(config, NRF24L01_EN_CRC);
        dev->crc = NRF24L01_CRC_16;
    }

    nrf24l01_write_register(dev, NRF24L01_CONFIG, config);
}

void nrf24l01_disable_crc(nrf24l01_conf_t* dev)
{
    uint8_t config = nrf24l01_read_register(dev, NRF24L01_CONFIG) ;
    clear_bit(config, NRF24L01_CRCO);
    clear_bit(config, NRF24L01_EN_CRC);

    nrf24l01_write_register(dev, NRF24L01_CONFIG, config);
    dev->crc = NRF24L01_CRC_DISABLED;
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
    byte fifo_status = nrf24l01_read_register(dev, NRF24L01_FIFO_STATUS);
    if(test_bit(fifo_status, NRF24L01_RX_EMPTY))
    {
        return 0;
    }

    if (pipe != NULL)
    {
        *pipe = (nrf24l01_get_status(dev) >> NRF24L01_RX_P_NO) & 0x7;
    }

    return 1;
}

void nrf24l01_retransmit_last(nrf24l01_conf_t* dev)
{
    nrf24l01_write_register(dev, NRF24L01_NRF_STATUS, NRF24L01_MAX_RT);
    spi_write_byte_ss(NRF24L01_REUSE_TX_PL, dev->ss_pin, dev->ss_port);    
    set_low(*dev->ce_port, dev->ce_pin);
    set_high(*dev->ce_port, dev->ce_pin);
}

void nrf24l01_wait_for_transmit(nrf24l01_conf_t* dev)
{
    while(nrf24l01_tx_is_full(dev));
}

uint8_t nrf24l01_read_payload(nrf24l01_conf_t* dev, byte* buffer, uint8_t length)
{
    // if RX fifo is empty wait
    while(nrf24l01_rx_is_empty(dev))
    {
        tu_delay_us(dev->transmission_delay);
    }

    length = length < dev->payload_lengths[pipe] ? length : dev->payload_lengths[pipe];
    uint8_t blank = (nrf24l01_dynamic_payload_enabled(dev, pipe) == NRF24L01_ENABLED) ? 0 : (dev->payload_lengths[pipe] - length);

    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(NRF24L01_R_RX_PAYLOAD, dev->ss_pin, dev->ss_port);
    while(length--)
    {
        *buffer++ = spi_write_byte_ss(0xFF, dev->ss_pin, dev->ss_port); 
    }
    while(blank--)
    {
        spi_write_byte_ss(0xFF, dev->ss_pin, dev->ss_port);
    }
    
    set_high(*dev->ss_port, dev->ss_pin);
    
    return nrf24l01_get_status(dev);
}

uint8_t nrf24l01_prepare_for_read(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t payload_length)
{
    nrf24l01_set_role(dev, NRF24L01_ROLE_RECEIVER);
    nrf24l01_write_pipe_rx_address(dev, pipe);
    nrf24l01_set_address_width(dev->pipe_addr_len);
    nrf24l01_enable_pipe(dev, pipe); // enable selected pipe to be ready to receive data :)
    nrf24l01_set_payload_size(dev, pipe, dev->payload_lengths[pipe]);

    set_high(*dev->ce_port, dev->ce_pin); // ready to receive data
}

uint8_t nrf24l01_read(nrf24l01_conf_t* dev, uint8_t pipe, byte* buffer, uint8_t length)
{
    return nrf24l01_read_payload(dev, pipe, &buffer, length);
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
    nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_MAX_RT | NRF24L01_TX_DS | NRF24L01_RX_DR); // default values are 1, see datasheet
    return status;
}

uint8_t nrf24l01_end_reading_keep_irq(nrf24l01_conf_t* dev, uint8_t pipe)
{
    set_low(*dev->ce_port, dev->ce_pin);
    nrf24l01_enable_pipe(dev, pipe, NRF24L01_DISABLED);
    return nrf24l01_get_status(dev);
}

uint8_t nrf24l01_write_succeed(nrf24l01_conf_t* dev, result)
{
    nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_MAX_RT | NRF24L01_TX_DS | NRF24L01_RX_DR); // default values are 1, see datasheet
}

uint8_t nrf24l01_write_payload(nrf24l01_conf_t* dev, byte* data, uint8_t length, byte cmd)
{
    nrf24l01_wait_for_transmit(dev);
    nrf24l01_write_pipe_tx_address(dev, pipe);
    nrf24l01_set_role(dev, NRF24L01_ROLE_TRANSMITTER);
    nrf24l01_set_payload_size(dev, pipe, dev->payload_lengths[pipe]);

    length = length < dev->payload_lengths[pipe] ? length : dev->payload_lengths[pipe];
    
    uint8_t blank = (nrf24l01_dynamic_payload_enabled(dev, pipe) == NRF24L01_ENABLED) ? 0 : (dev->payload_lengths[pipe] - length);
    
    set_low(*dev->ss_port, dev->ss_pin);
    spi_write_byte_ss(cmd, dev->ss_pin, dev->ss_port); // write to correct payload storage

    spi_write_ss(data, length, dev->ss_pin, dev->ss_port);
    while(blank--)
    {
        spi_write_byte_ss(0x00, dev->ss_pin, dev->ss_port);
    }

    set_low(*dev->ss_port, dev->ss_pin);
    return nrf24l01_get_status(dev);
}

uint8_t nrf24l01_prepare_for_write(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t enable_ackowledge)
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
        memcpy(dummy, dev->pipe_1_addr, dev->pipe_addr_len - 2);
        dummy[dev->pipe_addr_len - 1] = dev->pipe_addrs[pipe - 2];
        addr = & dummy[0];
    }

    return nrf24l01_prepare_for_write_to_addr(dev, addr, mode);
}

uint8_t nrf24l01_prepare_for_write_to_addr(nrf24l01_conf_t* dev, byte* address, uint8_t payload_length, uint8_t enable_ackowledge)
{
    nrf24l01_set_role(dev, NRF24L01_ROLE_TRANSMITTER);
    if(dev->flags & NRF24L01_FLAG_MODE_ACK)
    {
        nrf24l01_flush_rx();
    }
    nrf24l01_write_pipe_tx_address(dev, address, mode);
    nrf24l01_write_register(dev, NRF24L01_RX_PW_P0, payload_length);
    dev->tx_payload_size = payload_length;
    nrf24l01_enable_dynamic_acknowledge(enable_ackowledge);
    uint8_t status = nrf24l01_get_status(dev);
    set_high(*dev->ce_port, dev->ce_pin); // start transmission
    return status;
}


uint8_t nrf24l01_write(nrf24l01_conf_t* dev, uint8_t pipe, byte* data, uint8_t length)
{
    if(test_bit(dev->flags, NRF24L01_FLAG_MODE_ACK))
    {
        return nrf24l01_write_payload(dev, pipe, data, length, NRF24L01_W_TX_PAYLOAD);
    }
    else
    {
        return nrf24l01_write_payload(dev, pipe, data, length, NRF24L01_W_TX_PAYLOAD_NO_ACK);
    }
}

uint8_t nrf24l01_end_writing(nrf24l01_conf_t* dev)
{
    
    uint8_t status = nrf24l01_close_writing_keep_irq(dev);
    nrf24l01_write_register(dev, NRF24L01_CONFIG, NRF24L01_MAX_RT | NRF24L01_TX_DS | NRF24L01_RX_DR); // default values are 1, see datasheet

    return status;
}

uint8_t nrf24l01_close_end_keep_irq(nrf24l01_conf_t* dev)
{
    set_low(*dev->ce_port, dev->ce_pin);
    uint8_t status = nrf24l01_get_status(dev);
    nrf24l01_flush_tx(dev);
    return status;
}

uint8_t nrf24l01_succeed(nrf24l01_conf_t* dev, result)
{
    if ((result & NRF24L01_MAX_RT) /* ok */ && (result & NRF24L01_TX_DS) /* ok */)
    {
        if (dev->flags & NRF24L01_FLAG_MODE_ACK)
        {
            if (result & NRF24L01_RX_DR)
            {
                return 1; // OK!
            }        
        }
    }
    
    result 0;
}