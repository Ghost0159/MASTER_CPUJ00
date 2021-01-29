/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-1
  File:     wc.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.15  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.14  2004/10/28 13:08:07  yosizaki
  remove all the sources. (moved to wireless_shared!)

  Revision 1.13  2004/10/28 11:04:21  yosizaki
  add callback for block-transfer.
  merge with ../wireless_shared.

  Revision 1.12  2004/10/19 10:27:02  miya
  Port受信コールバックの使用とプリント文をNITRO画面上に表示するように変更

  Revision 1.11  2004/10/18 04:17:46  terui
  ビーコン間隔、スキャン間隔をばらつかせる仕様を追加。

  Revision 1.10  2004/10/05 09:45:29  terui
  MACアドレスをu8型の配列に統一。

  Revision 1.9  2004/10/04 13:44:06  terui
  ゲームグループIDをu32型に統一。

  Revision 1.8  2004/10/04 05:51:26  miya
  change transfer size according to the programing guideline

  Revision 1.7  2004/09/30 08:28:43  yosizaki
  add SDK_MAKEGGID_SYSTEM macro.

  Revision 1.6  2004/09/13 08:32:26  miya
  WBT_InitParentの引数の意味変更

  Revision 1.5  2004/09/10 12:28:10  seiki_masashi
  キーシェアリングの送信バイト数をライブラリ内で隠蔽するように変更

  Revision 1.4  2004/09/09 06:25:17  miya
  small fix.

  Revision 1.3  2004/09/03 04:40:18  seiki_masashi
  送信失敗の errcode を WM_ERRCODE_SEND_FAILED に変更
  WM_ERRCODE_INVALID_POLLBITMAP への対応

  Revision 1.2  2004/08/26 23:54:25  miya
  名前変更など

  Revision 1.1  2004/08/23 09:47:33  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
