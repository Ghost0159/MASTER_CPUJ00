/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay
  File:     common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/10/20 07:49:17  yosizaki
  fix port number.

  Revision 1.4  2005/10/03 09:54:32  yosizaki
  fix port number.

  Revision 1.3  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.2  2004/10/28 13:05:01  yosizaki
  move some parameters.

  Revision 1.1  2004/10/25 10:18:47  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __NITROSDK_DEMO_WBT_WBT_FS_COMMON_H
#define __NITROSDK_DEMO_WBT_WBT_FS_COMMON_H

#include <nitro.h>


/******************************************************************************/
/* constant */

/* 無線通信パラメータ */

#define		WC_PARENT_DATA_SIZE_MAX		128
#define		WC_CHILD_DATA_SIZE_MAX		256

#if	!defined(SDK_MAKEGGID_SYSTEM)
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#endif
#define GGID_WBT_FS                 SDK_MAKEGGID_SYSTEM(0x31)

/* WFS 用のポート番号 */
#define	port_wbt            5

/* WFS 用の親機送信最大値 */
#define	parent_packet_max   WC_PARENT_DATA_SIZE_MAX


/******************************************************************************/
/* function */

#if	defined(__cplusplus)
extern  "C"
{
#endif

/* サンプルデモの画面遷移ごとの関数 */
    void    ModeSelect(const KeyInfo * p_key);  /* 親機/子機 選択画面 */
    void    ModeError(const KeyInfo * p_key);   /* エラー表示画面 */
    void    ModeWorking(const KeyInfo * p_key); /* ビジー画面 */
    void    ModeParent(const KeyInfo * p_key);  /* 親機 通信画面 */
    void    ModeChild(const KeyInfo * p_key);   /* 子機 通信画面 */

#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


/******************************************************************************/


#endif                                 /* __NITROSDK_DEMO_WBT_WBT_FS_COMMON_H */
