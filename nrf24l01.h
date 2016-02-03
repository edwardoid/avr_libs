#ifndef NRF24L01_H
#define NRF24L01_H

#include "my_types.h"

#include "nrf24l01_registers.h"
#include "nrf24l01_commands.h"
#include "nrf24l01_bits.h"
#include "debug.h"

// Flags
#define NRF24L01_FLAG_MODEL 7
#define NRF24L01_FLAG_MODE_ACK 6
// End of flags

#define NRF24L01 0
#define NRF24L01_P 1

#define NRF24L01_PA_MIN 0
#define NRF24L01_PA_LOW 1
#define NRF24L01_PA_HIGH 2
#define NRF24L01_PA_MAX 3

#define NRF24L01_SPEED_UNDEFINED 0
#define NRF24L01_1MBPS 1
#define NRF24L01_2MBPS 2
#define NRF24L01_250KBPS 3

#define NRF24L01_CRC_DISABLED 0
#define NRF24L01_CRC_8 1
#define NRF24L01_CRC_16 2

#define NRF24L01_ADDR_WIDTH_ILLEGAL 0x00
#define NRF24L01_ADDR_WIDTH_3_BYTES 3
#define NRF24L01_ADDR_WIDTH_4_BYTES 4
#define NRF24L01_ADDR_WIDTH_5_BYTES 5

typedef uint8_t nrf24l01_pipe_addr_t;

typedef struct {
  // IRQ
  int8_t interrupt;

  port_ptr_t  ce_port;
  pin_num_t   ce_pin;
  
  port_ptr_t  ss_port;
  pin_num_t   ss_pin;

  uint8_t pipe_addr_len; // common for all pipes!! 5 value is maximum!
  nrf24l01_pipe_addr_t pipe_0_addr [5];
  nrf24l01_pipe_addr_t pipe_1_addr [5]; 
  nrf24l01_pipe_addr_t pipe_addrs[4]; // each byte as addr for pipe 1-5 

  uint8_t flags; // [ 0000 000 ] [ {Plus version?}  ]
  uint8_t tx_payload_size;
  uint16_t transmission_delay; 
} nrf24l01_conf_t;

#define NRF24L01_ENABLE 1
#define NRF24L01_ENABLED NRF24L01_ENABLE
#define NRF24L01_DISABLE 0
#define NRF24L01_DISABLED NRF24L01_DISABLE

#define NRF24L01_ERX_P0 _BV(0)
#define NRF24L01_ERX_P1 _BV(1)
#define NRF24L01_ERX_P2 _BV(2)
#define NRF24L01_ERX_P3 _BV(3)
#define NRF24L01_ERX_P4 _BV(4)

#define NRF24L01_ROLE_RECEIVER 1
#define NRF24L01_ROLE_TRANSMITTER 2

/**
  Initializes nRF24L01(+) chip. Reads initial addresses into config struct, determines chip version(+ version or not)
*/

extern void nrf24l01_init_config(nrf24l01_conf_t* dst);

extern byte nrf24l01_init(nrf24l01_conf_t* dev, ddr_ptr_t ce, ddr_ptr_t ss);

extern uint8_t nrf24l01_is_plus_model(nrf24l01_conf_t* dev);

#define nrf24l01_is_plus_model(dev) (test_bit(dev->flags, NRF24L01_FLAG_MODEL))

extern byte nrf24l01_get_status(nrf24l01_conf_t* dev);

extern void nrf24l01_set_address(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint8_t a5);

extern void nrf24l01_set_role(nrf24l01_conf_t* dev, uint8_t role); 
extern byte nrf24l01_set_retries(nrf24l01_conf_t* dev, uint8_t delay, uint8_t count);

extern byte nrf24l01_set_speed(nrf24l01_conf_t* dev, uint8_t speed);
extern byte nrf24l01_get_speed(nrf24l01_conf_t* dev);

extern byte nrf24l01_set_channel(nrf24l01_conf_t* dev, uint8_t channel);
extern byte nrf24l01_get_channel(nrf24l01_conf_t* dev);

extern byte nrf24l01_flush_rx(nrf24l01_conf_t* dev);
extern byte nrf24l01_flush_tx(nrf24l01_conf_t* dev);

#define nrf24l01_rx_is_full(dev) (!(nrf24l01_read_register(dev, NRF24L01_FIFO_STATUS) & NRF24L01_RX_FULL))
#define nrf24l01_rx_is_empty(dev) (nrf24l01_read_register(dev, NRF24L01_FIFO_STATUS) & NRF24L01_RX_EMPTY)

#define nrf24l01_tx_is_full(dev) (!(nrf24l01_read_register(dev, NRF24L01_NOP) & NRF24L01_TX_FULL))
#define nrf24l01_tx_is_empty(dev) (nrf24l01_read_register(dev, NRF24L01_FIFO_STATUS) & NRF24L01_TX_EMPTY)

/**
  All function nrf24l01_enable* functions takes care about their dependencies
*/
extern void nrf24l01_enable_payload_acknowledge(nrf24l01_conf_t* dev, uint8_t enable);

extern void nrf24l01_enable_auto_acknowledge_for_pipe(nrf24l01_conf_t* dev, uint8_t pipe,uint8_t enable);

extern void nrf24l01_enable_dynamic_acknowledge(nrf24l01_conf_t* dev, uint8_t enable);

extern void nrf24l01_enable_dynamic_payload_feature(nrf24l01_conf_t* dev, uint8_t enable);

extern void nrf24l01_enable_dynamic_payload_on_pipe(nrf24l01_conf_t* dev, uint8_t enable, uint8_t pipe);
extern uint8_t nrf24l01_dynamic_payload_enabled(nrf24l01_conf_t* dev, uint8_t pipe);

extern void nrf24l01_set_payload_size(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t size);
extern uint8_t nrf24l01_get_payload_size(nrf24l01_conf_t* dev, uint8_t pipe);

extern byte nrf24l01_acknowlede_available(nrf24l01_conf_t* dev);

extern void nrf24l01_set_crc_length(nrf24l01_conf_t* dev, uint8_t length);
extern void nrf24l01_disable_crc(nrf24l01_conf_t* dev);

extern void nrf24l01_set_power_amplifier(nrf24l01_conf_t* dev, uint8_t value);
extern uint8_t nrf24l01_get_power_amplifier(nrf24l01_conf_t* dev);

extern void nrf24l01_set_address_width(nrf24l01_conf_t* dev, uint8_t value);
extern uint8_t nrf24l01_get_address_width(nrf24l01_conf_t* dev);

extern void nrf24l01_enable_pipe(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t enable);
extern void nrf24l01_enable_all_pipes(nrf24l01_conf_t* dev, uint8_t enable);

extern uint8_t nrf24l01_data_available(nrf24l01_conf_t* dev, uint8_t* pipe);

extern void nrf24l01_retransmit_last(nrf24l01_conf_t* dev);

extern void nrf24l01_wait_for_transmit(nrf24l01_conf_t* dev);

// RX mode
extern uint8_t nrf24l01_prepare_for_read(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t payload_length);
extern uint8_t nrf24l01_read_byte(nrf24l01_conf_t* dev, uint8_t pipe);
extern uint8_t nrf24l01_read(nrf24l01_conf_t* dev, uint8_t pipe, byte* buffer, uint8_t length);
extern uint8_t nrf24l01_end_reading(nrf24l01_conf_t* dev, uint8_t pipe);
extern uint8_t nrf24l01_end_reading_keep_irq(nrf24l01_conf_t* dev, uint8_t pipe);
extern uint8_t nrf24l01_end_succeed(nrf24l01_conf_t* dev, byte result);
// TX mode
extern byte nrf24l01_prepare_for_write(nrf24l01_conf_t* dev, uint8_t pipe, uint8_t enable_ackowledge);
extern byte nrf24l01_prepare_for_write_to_addr(nrf24l01_conf_t* dev, byte* address, uint8_t payload_length, uint8_t enable_ackowledge);
extern uint8_t nrf24l01_write(nrf24l01_conf_t* dev, byte* data, uint8_t length);
#define nrf24l01_write_byte(dev, pipe, data) nrf24l01_write(dev, pipe, &data, 1);
extern uint8_t nrf24l01_end_writing(nrf24l01_conf_t* dev);
extern uint8_t nrf24l01_end_writing_keep_irq(nrf24l01_conf_t* dev);
extern uint8_t nrf24l01_write_succeed(nrf24l01_conf_t* dev, byte result);
// Common //
extern byte nrf24l01_read_register(nrf24l01_conf_t* dev, byte reg);
extern byte nrf24l01_write_register(nrf24l01_conf_t* dev, byte reg, byte value);

#ifdef ENABLE_USART_DEBUGGING
extern void nrf24l01_print_addresses(nrf24l01_conf_t* dev);
extern void nrf24l01_print_details(nrf24l01_conf_t* dev);
#else
#define nrf24l01_print_addresses(dev) /* nop */
#define nrf24l01_print_details(status)
#endif // ENABLE_USART_DEBUGGING


#endif // NRF24L01_H
    
