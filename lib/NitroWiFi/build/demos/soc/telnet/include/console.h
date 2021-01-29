/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - demos - soc - telnet
  File:     console.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/
#ifndef TELNET_CONSOLE_H_
#define TELNET_CONSOLE_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include "vt100.h"

/*---------------------------------------------------------------------------*
  定数定義
 *---------------------------------------------------------------------------*/
#define ASCII_NUL   0x00
#define ASCII_BEL   0x07
#define ASCII_BS    0x08
#define ASCII_HT    0x09
#define ASCII_LF    0x0a
#define ASCII_VT    0x0b
#define ASCII_FF    0x0c
#define ASCII_CR    0x0d
#define ASCII_SO    0x0e
#define ASCII_SI    0x0f
#define ASCII_ESC   0x1b

/*---------------------------------------------------------------------------*
  構造体定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  関数プロトタイプ定義
 *---------------------------------------------------------------------------*/
void    ConsoleInit(ConsoleHandle* pHandle);
void    ConsoleSetDefault(ConsoleHandle* pHandle);
void    ConsoleUpdate(const ConsoleHandle* pHandle);
void    ConsoleScrollUp(ConsoleHandle* pHandle, int nLine);
void    ConsoleScrollDown(ConsoleHandle* pHandle, int nLine);
void    ConsoleNextLine(ConsoleHandle* pHandle);
void    ConsoleEraceColumn(ConsoleHandle* pHandle, int nStartColumn, int nEndColumn);
void    ConsoleEraceLine(ConsoleHandle* pHandle, int nStartLine, int nEndLine);

void    ConsolePutChar(char c);
void    ConsolePutString(const char* s);
void    ConsolePrintf(const char* fmt, ...);
void    ConsoleFlush(void);
void    ConsoleClear(void);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif /* __cplusplus */
#endif /* TELNET_CONSOLE_H_ */

/*---------------------------------------------------------------------------*
  $Log: console.h,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

 *---------------------------------------------------------------------------*/
