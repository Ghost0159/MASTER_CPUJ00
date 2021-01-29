/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-1
  File:     bt.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: bt.h,v $
  Revision 1.8  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.7  2004/10/28 11:07:27  yosizaki
  move parameters.

  Revision 1.6  2004/10/04 08:40:40  miya
  add WBT_CancelCurrentCommand

  Revision 1.5  2004/10/04 05:49:17  miya
  change transfer size according to the programing guideline

  Revision 1.4  2004/09/13 08:32:18  miya
  WBT_InitParentの引数の意味変更

  Revision 1.3  2004/08/27 07:08:50  miya
  名前などの変更

  Revision 1.2  2004/08/26 23:54:20  miya
  名前変更など

  Revision 1.1  2004/08/23 09:47:24  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	NITRO_BUILD_DEMOS_WBT_WBT1_INCLUDE_BT_H_
#define	NITRO_BUILD_DEMOS_WBT_WBT1_INCLUDE_BT_H_

#include	<nitro/wbt.h>

/* ブロックデータの数 */
#define NUM_OF_BT_LIST 3

/* １ブロックデータのサイズ */
#define BT_DATA_SIZE 0x10000

//親機のパケットサイズ
#define BT_PARENT_PACKET_SIZE WC_PARENT_DATA_SIZE_MAX

//子機のパケットサイズ
#define BT_CHILD_PACKET_SIZE  WC_CHILD_DATA_SIZE_MAX

#define     SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#define     GGID_WBT_1                  SDK_MAKEGGID_SYSTEM(0x30)
#define     WC_PARENT_DATA_SIZE_MAX     128
#define     WC_CHILD_DATA_SIZE_MAX      16


/* 親機用ブロック登録関数 */
extern void bt_register_blocks(void);

/* ブロック転送汎用コールバック関数 */
extern void bt_callback(void *arg);

/* 子機用ブロック転送開始関数 */
extern void bt_start(void);

/* 子機用ブロック転送終了関数 */
extern void bt_stop(void);

#endif /* NITRO_BUILD_DEMOS_WBT_WBT1_INCLUDE_BT_H_ */
