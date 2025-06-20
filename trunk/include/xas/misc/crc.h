/*
 * File: crc.h
 * Date: 23-Apr-1993
 * By  : Keivn Esteb
 *
 *	This file conatins the defines and prototypes for crc.c
 */

#ifndef _XAS_MISC_CRC_H_
#define _XAS_MISC_CRC_H_

#define CRC_32_RESIDUAL    0xDEBB20E3L
#define CRC_32_PRECON      0xFFFFFFFFL
#define CRC_CCITT_PRECON   0xFFFF
#define CRC_CCITT_RESIDUAL 0xF0B8

unsigned short crc16(char *, int);
unsigned short crc_ccitt(char *, int);
unsigned long  crc32(char *, int);

#endif
