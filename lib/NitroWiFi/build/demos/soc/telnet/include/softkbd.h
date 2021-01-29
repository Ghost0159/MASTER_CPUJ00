/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     softkbd.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: softkbd.h,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#ifndef TELNET_SOFTKBD_H_
#define TELNET_SOFTKBD_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#ifdef WITH_KBD
#include <kbd.h>
#include <kbd_data.h>
#include <kbd_font.h>
#include <kbd_input.h>
#endif

/*---------------------------------------------------------------------------*
  定数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  構造体定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  関数プロトタイプ定義
 *---------------------------------------------------------------------------*/
void    SoftkbdUpdate(void);
void    SoftkbdInit(int x, int y);
u32     SoftkbdRead(void* pBuffer, u32 nLength);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif /* __cplusplus */
#endif /* TELNET_SOFTKBD_H_ */
