#include "rc522.h"

#if defined(F_RC522) && defined(F_SPI)

#include "rc522_registers.h"
#include "rc522_commands.h"
#include <spi/spi.h>
#include <my_types.h>
#include <bytes_manip.h>
#include <bitman.h>

#define RC522_PrescalerHi b1101
#define RC522_PrescalerLo b00111110

#define RC522_ReloadHi	b00011110
#define RC522_ReloadLo	b00000000

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

void rc522_init(rc522_cfg_t* cfg)
{
	spi_init_as_master_ex(&cfg->ss.pin, 1, cfg->ss.ddr, SPI_DIV_CLK_16, SPI_MODE_0);
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


#endif // F_RC522 && F_SPI
