#ifndef RC522_REISTERS_H
#define RC522_REISTERS_H

#include "lib_ex_config.h"

#if defined(F_RC522)

#include <bitman.h>


#define RC522_Reserved		0x00
#define RC522_Reserved_1	0x00

/**
	Command
*/
#define RC522_CommandReg	0x01

#define		RC522_RcvOff		_BV(5)
#define		RC522_PowerDown		_BV(4)

#define RC522_ComlEnReg		0x02
#define		RC522_IRqInv		_BV(7)
#define		RC522_TxlEn			_BV(6)
#define		RC522_RxlEn			_BV(5)
#define		RC522_IdlelEn		_BV(4)
#define		RC522_HiAlertEn		_BV(3)
#define		RC522_LoAlertEn		_BV(2)
#define		RC522_EddlEn		_BV(1)
#define		RC522_TimerEn		_BV(0)

#define RC522_DivEnReg		0x03
#define		RC522_IRqPushPull	_BV(7)
#define		RC522_MfinActlEn	_BV(4)
#define		RC522_CRClEn		_BV(2)

#define RC522_ComIrqReg		0x04
#define		RC522_Set1			_BV(7)
#define		RC522_TxIRq			_BV(6)
#define		RC522_RxIRq			_BV(5)
#define		RC522_IdleRq		_BV(4)
#define		RC522_HiAlertRq		_BV(3)
#define		RC522_LoAlertReq	_BV(2)
#define		RC522_ErrIReq		_BV(1)
#define		RC522_TimerIReq		_BV(0)


#define RC522_DivIrqReg		0x05
#define		RC522_Set2			_BV(7)
#define		RC522_MfinActIRq	_BV(4)
#define		RC522_CRCIRq		_BV(2)


#define RC522_ErrorReg		0x06
#define		RC522_WrErr			_BV(7)
#define		RC522_TempErr		_BV(6)
#define		RC522_BufferOvfl	_BV(4)
#define		RC522_CollErr		_BV(3)
#define		RC522_CRCErr		_BV(2)
#define		RC522_ParityErr		_BV(1)
#define		RC522_ProtocolErr	_BV(0)


#define RC522_Status1Reg	0x07
#define		RC522_CRCOk			_BV(6)
#define		RC522_CRCReady		_BV(5)
#define		RC522_IRq			_BV(4)
#define		RC522_TRunning		_BV(3)
#define		RC522_HiAlert		_BV(1)
#define		RC522_LoAlert		_BV(0)


#define RC522_Status2Reg	0x08
#define		RC522_TempSensClear	_BV(7)
#define		RC522_I2CForceHS	_BV(6)
#define		RC522_MFCrypto1On	_BV(3)


#define RC522_FIFODataReg	0x09

#define RC522_FIFOLeveIReg	0x0A
#define		RC522_FlushBuffer	_BV(7)

#define RC522_WaterLeveIReg	0x0B

#define RC522_ControIReg	0x0C
#define		RC522_TStopNow		_BV(7)
#define		RC522_TStartNow		_BV(6)

#define RC522_BitFramingReg	0x0D
#define RC522_ColIReg		0x0E

#define RC522_Reserved_2	0x0F
#define RC522_Reserved_3	0x10

#define RC522_ModeReg		0x11
#define RC522_TxModeReg		0x12
#define RC522_RxModeReg		0x13

#define RC522_TxControIReg	0x14
#define RC522_TxASKReg		0x15

#define RC522_TxSeIReg		0x16
#define RC522_RxSeIReg		0x17

#define RC522_RxThresholdReg 0x18
#define RC522_DemodReg		0x19

#define RC522_Reserved_4	0x1A
#define RC522_Reserver_5	0x1B

#define RC522_MfTxReg		0x1C
#define RC522_MfRxReg		0x1D

#define RC522_Reserver_6	0x1E

#define RC522_SerialSpeedReg 0x1F

/**
	Configuration
*/
#define RC522_Reserved_7	0x20

#define RC522_CRCResultRegMSB 0x21
#define RC522_CRCResultRegLSB 0x22

#define RC522_Reserved_8	0x23

#define RC522_ModWidthReg	0x24

#define RC522_Reserved_9	0x25

#define RC522_RFCfgReg		0x26
#define RC522_GsNReg		0x27
#define RC522_CWGsPReg		0x28
#define RC522_ModGsPReg		0x29

#define RC522_TModeReg		0x2A
#define RC522_TPrescalerReg	0x2B

#define RC522_ReloadReg_H	0x2C
#define RC522_ReloadReg_L	0x2D

#define RC522_TCounterVaIReg_H 0x2E
#define RC522_TCounterVaIReg_L 0x2F

#define RC522_Reserved_10 	0x30

#define RC522_TestSel1Reg	0x31
#define RC522_TestSel2Reg	0x32

#define RC522_TestPinEnReg	0x33
#define RC522_TestPinValueReg	0x34
#define RC522_TestBusReg	0x35
#define RC522_AutoTestReg	0x36
#define RC522_VersionReg	0x37
#define RC522_AnalogTestReg	0x38
#define RC522_TestDAC1Reg	0x39
#define RC522_TestDAC2Reg	0x3A

#define RC522_Reserved_11	0x3B
#define RC522_Reserved_12	0x3C
#define RC522_Reserved_12	0x3D
#define RC522_Reserved_13	0x3E
#define RC522_Reserver_14	0x3F

#endif // F_RC522

#endif // RC522_REISTERS_H 