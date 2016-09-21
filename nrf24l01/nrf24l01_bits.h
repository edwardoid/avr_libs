#ifndef NRF24L01_BITS_H
#define NRF24L01_BITS_H

#ifndef _BV
#define _BV(num) (1 << num)
#endif

#define NRF24L01_MASK_RX_DR  _BV(6)
#define NRF24L01_MASK_TX_DS  _BV(5)
#define NRF24L01_MASK_MAX_RT _BV(4)
#define NRF24L01_EN_CRC      _BV(3)
#define NRF24L01_CRCO        _BV(2)
#define NRF24L01_PWR_UP      _BV(1)
#define NRF24L01_PRIM_RX     _BV(0)

#define NRF24L01_ENAA_P5     _BV(5)
#define NRF24L01_ENAA_P4     _BV(4)
#define NRF24L01_ENAA_P3     _BV(3)
#define NRF24L01_ENAA_P2     _BV(2)
#define NRF24L01_ENAA_P1     _BV(1)
#define NRF24L01_ENAA_P0     _BV(0)

#define NRF24L01_ERX_P5      _BV(5)
#define NRF24L01_ERX_P4      _BV(4)
#define NRF24L01_ERX_P3      _BV(3)
#define NRF24L01_ERX_P2      _BV(2)
#define NRF24L01_ERX_P1      _BV(1)
#define NRF24L01_ERX_P0      _BV(0)

#define NRF24L01_AW          _BV(0)

#define NRF24L01_ARD         _BV(4)
#define NRF24L01_ARC         _BV(0)

#define NRF24L01_PLL_LOCK    _BV(4)
#define NRF24L01_RF_DR       _BV(3)

#define NRF24L01_RF_PWR      _BV(6)

// Status
#define NRF24L01_RX_DR       _BV(6)
#define NRF24L01_TX_DS       _BV(5)
#define NRF24L01_MAX_RT      _BV(4)
#define NRF24L01_RX_P_NO     (_BV(1) | _BV(2) | _BV(3))
#define NRF24L01_TX_FULL_ST  _BV(0)

#define NRF24L01_PLOS_CNT    _BV(4)
#define NRF24L01_ARC_CNT     _BV(0)

// Fifo0
#define NRF24L01_TX_REUSE    _BV(6)
#define NRF24L01_TX_FULL_FF  _BV(5)
#define NRF24L01_TX_EMPTY    _BV(4)
#define NRF24L01_RX_FULL     _BV(1)
#define NRF24L01_RX_EMPTY    _BV(0)

#define NRF24L01_DPL_P5      _BV(5)
#define NRF24L01_DPL_P4      _BV(4)
#define NRF24L01_DPL_P3      _BV(3)
#define NRF24L01_DPL_P2      _BV(2)
#define NRF24L01_DPL_P1      _BV(1)
#define NRF24L01_DPL_P0      _BV(0)

#define NRF24L01_EN_DPL      _BV(2)
#define NRF24L01_EN_ACK_PAY  _BV(1)
#define NRF24L01_EN_DYN_ACK  _BV(0)


/* Non-P omissions */
#define LNA_HCURR   0

/* P model memory Map */
#define NRF24L01_RPD         0x09
#define NRF24L01_W_TX_PAYLOAD_NO_ACK  0xB0

/* P model bit Mnemonics */
#define NRF24L01_RF_DR_LOW   5
#define NRF24L01_RF_DR_HIGH  3
#define NRF24L01_RF_PWR_LOW  1
#define NRF24L01_RF_PWR_HIGH 2

#endif // NRF24L01_BITS_H

