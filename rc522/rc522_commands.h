#ifndef RC522_COMMANDS_H

#include <lib_ex_config.h>

#if defined(F_RC522)

#include <bytes_manip.h>

#define RC522_CMD_Idle			b0000		b
#define RC522_CMD_Mem			b0001
#define RC522_CMD_GenRndId		b0010
#define RC522_CMD_CalcCRC		b0011
#define RC522_CMD_Transmit		b0100
#define RC522_CMD_NoCmdChange 	b0111
#define RC522_CMD_Receive		b1000
#define RC522_CMD_Transceive	b1100
#define RC522_CMD_MFAuthent		b1110
#define RC522_CMD_SoftReset		b1111

#endif // F_RC522

#endif // RC522_COMMANDS_H