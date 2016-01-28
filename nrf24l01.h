#ifndef NRF24L01_H
#define NRF24L01_H

#include "my_types.h"

#include "nrf24l01_registers.h"
#include "nrf24l01_commands.h"
#include "nrf24l01_bits.h"

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
#define NRF24L01_CRC_16 2;

typedef uint8_t nrf24l01_role_t;

extern nrf24l01_role_t Receiver;
extern nrf24l01_role_t Transmitter;

typedef struct {
  // IRQ
  int8_t interrupt;

  nrf24l01_role_t      role;

  port_ptr_t  ce_port;
  pin_num_t   ce_pin;
  
  port_ptr_t  ss_port;
  pin_num_t   ss_pin;

  // internal data. Do not change value manually
  uint8_t payload_size;
  uint8_t transmission_delay;
  
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

extern byte nrf24l01_init(nrf24l01_conf_t* dev, ddr_ptr_t ce, ddr_ptr_t ss);
extern byte nrf24l01_get_status(nrf24l01_conf_t* dev);
extern byte nrf24l01_set_role(nrf24l01_conf_t* dev); 
extern byte nrf24l01_set_retries(nrf24l01_conf_t* dev, uint8_t delay, uint8_t count);
extern byte nrf24l01_set_speed(nrf24l01_conf_t* dev, uint8_t speed);
extern byte nrf24l01_get_speed(nrf24l01_conf_t* dev);
extern byte nrf24l01_set_channel(nrf24l01_conf_t* dev, uint8_t channel);
extern byte nrf24l01_get_channel(nrf24l01_conf_t* dev);
extern byte nrf24l01_flush(nrf24l01_conf_t* dev, nrf24l01_role_t role);
extern void nrf24l01_enable_payload_acknowledge(nrf24l01_conf_t* dev, uint8_t enable);
extern void nrf24l01_enable_dynamic_acknowledge(nrf24l01_conf_t* dev, uint8_t enable);
#define nrf24l01_flush_rx(dev) nrf24l01_flush(dev, Receiver)
#define nrf24l01_flush_tx(dev) nrf24l01_flush(dev, Transmitter)
#define nrf24l01_flush_all(dev) (nrf24l01_flush_rx(dev) & nrf24l01_flush_tx(dev))

// Common //
extern byte nrf24l01_read_register(nrf24l01_conf_t* dev, byte reg);
extern byte nrf24l01_write_register(nrf24l01_conf_t* dev, byte reg, byte value);

#endif // NRF24L01_H
