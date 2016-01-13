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

#ifndef SD_CMD_H
#define SD_SD_CMD_H

/* CMD0: response R1 */
#define SD_CMD_GO_IDLE_STATE 0x00
/* CMD1: response R1 */
#define SD_CMD_SEND_OP_COND 0x01
/* CMD8: response R7 */
#define SD_CMD_SEND_IF_COND 0x08
/* CMD9: response R1 */
#define SD_CMD_SEND_CSD 0x09
/* CMD10: response R1 */
#define SD_CMD_SEND_CID 0x0a
/* CMD12: response R1 */
#define SD_CMD_STOP_TRANSMISSION 0x0c
/* CMD13: response R2 */
#define SD_CMD_SEND_STATUS 0x0d
/* CMD16: arg0[31:0]: block length, response R1 */
#define SD_CMD_SET_BLOCKLEN 0x10
/* CMD17: arg0[31:0]: data address, response R1 */
#define SD_CMD_READ_SINGLE_BLOCK 0x11
/* CMD18: arg0[31:0]: data address, response R1 */
#define SD_CMD_READ_MULTIPLE_BLOCK 0x12
/* CMD24: arg0[31:0]: data address, response R1 */
#define SD_CMD_WRITE_SINGLE_BLOCK 0x18
/* CMD25: arg0[31:0]: data address, response R1 */
#define SD_CMD_WRITE_MULTIPLE_BLOCK 0x19
/* CMD27: response R1 */
#define SD_CMD_PROGRAM_CSD 0x1b
/* CMD28: arg0[31:0]: data address, response R1b */
#define SD_CMD_SET_WRITE_PROT 0x1c
/* CMD29: arg0[31:0]: data address, response R1b */
#define SD_CMD_CLR_WRITE_PROT 0x1d
/* CMD30: arg0[31:0]: write protect data address, response R1 */
#define SD_CMD_SEND_WRITE_PROT 0x1e
/* CMD32: arg0[31:0]: data address, response R1 */
#define SD_CMD_TAG_SECTOR_START 0x20
/* CMD33: arg0[31:0]: data address, response R1 */
#define SD_CMD_TAG_SECTOR_END 0x21
/* CMD34: arg0[31:0]: data address, response R1 */
#define SD_CMD_UNTAG_SECTOR 0x22
/* CMD35: arg0[31:0]: data address, response R1 */
#define SD_CMD_TAG_ERASE_GROUP_START 0x23
/* CMD36: arg0[31:0]: data address, response R1 */
#define SD_CMD_TAG_ERASE_GROUP_END 0x24
/* CMD37: arg0[31:0]: data address, response R1 */
#define SD_CMD_UNTAG_ERASE_GROUP 0x25
/* CMD38: arg0[31:0]: stuff bits, response R1b */
#define SD_CMD_ERASE 0x26
/* ACMD41: arg0[31:0]: OCR contents, response R1 */
#define SD_CMD_GET_OCR 0x29
/* CMD42: arg0[31:0]: stuff bits, response R1b */
#define SD_CMD_LOCK_UNLOCK 0x2a
/* CMD55: arg0[31:0]: stuff bits, response R1 */
#define SD_CMD_APP 0x37
/* CMD58: arg0[31:0]: stuff bits, response R3 */
#define SD_CMD_READ_OCR 0x3a
/* CMD59: arg0[31:1]: stuff bits, arg0[0:0]: crc option, response R1 */
#define SD_CMD_CRC_ON_OFF 0x3b

/* command responses */
/* R1: size 1 byte */
#define SD_R1_IDLE_STATE 0
#define SD_R1_ERASE_RESET 1
#define SD_R1_ILLEGAL_COMMAND 2
#define SD_R1_COM_CRC_ERR 3
#define SD_R1_ERASE_SEQ_ERR 4
#define SD_R1_ADDR_ERR 5
#define SD_R1_PARAM_ERR 6
/* R1b: equals R1, additional busy bytes */
/* R2: size 2 bytes */
#define SD_R2_CARD_LOCKED 0
#define SD_R2_WP_ERASE_SKIP 1
#define SD_R2_ERR 2
#define SD_R2_CARD_ERR 3
#define SD_R2_CARD_ECC_FAIL 4
#define SD_R2_WP_VIOLATION 5
#define SD_R2_INVAL_ERASE 6
#define SD_R2_OUT_OF_RANGE 7
#define SD_R2_CSD_OVERWRITE 7
#define SD_R2_IDLE_STATE ( SD_R1_IDLE_STATE + 8)
#define SD_R2_ERASE_RESET ( SD_R1_ERASE_RESET + 8)
#define SD_R2_ILLEGAL_COMMAND ( SD_R1_ILLEGAL_COMMAND + 8)
#define SD_R2_COM_CRC_ERR ( SD_R1_COM_CRC_ERR + 8)
#define SD_R2_ERASE_SEQ_ERR ( SD_R1_ERASE_SEQ_ERR + 8)
#define SD_R2_ADDR_ERR ( SD_R1_ADDR_ERR + 8)
#define SD_R2_PARAM_ERR ( SD_R1_PARAM_ERR + 8)
/* R3: size 5 bytes */
#define SD_R3_OCR_MASK (0xffffffffUL)
#define SD_R3_IDLE_STATE ( SD_R1_IDLE_STATE + 32)
#define SD_R3_ERASE_RESET ( SD_R1_ERASE_RESET + 32)
#define SD_R3_ILLEGAL_COMMAND ( SD_R1_ILLEGAL_COMMAND + 32)
#define SD_R3_COM_CRC_ERR ( SD_R1_COM_CRC_ERR + 32)
#define SD_R3_ERASE_SEQ_ERR ( SD_R1_ERASE_SEQ_ERR + 32)
#define SD_R3_ADDR_ERR ( SD_R1_ADDR_ERR + 32)
#define SD_R3_PARAM_ERR ( SD_R1_PARAM_ERR + 32)
/* Data Response: size 1 byte */
#define SD_DR_STATUS_MASK 0x0e
#define SD_DR_STATUS_ACCEPTED 0x05
#define SD_DR_STATUS_CRC_ERR 0x0a
#define SD_DR_STATUS_WRITE_ERR 0x0c

#endif // SD_CMD_H