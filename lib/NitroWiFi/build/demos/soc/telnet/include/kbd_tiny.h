/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     kbd_tiny.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: kbd_tiny.h,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#ifndef TELNET_KBD_TINY_
#define TELNET_KBD_TINY_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  定数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  構造体定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  関数プロトタイプ定義
 *---------------------------------------------------------------------------*/
void    TinykbdInit();
void    TinykbdUpdate();
u8      TinykbdGetInput();

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif /* __cplusplus */
#endif /* TELNET_KBD_TINY_ */
