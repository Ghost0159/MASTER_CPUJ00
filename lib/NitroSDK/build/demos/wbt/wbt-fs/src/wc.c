/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     wc.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.18  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.17  2004/10/28 13:16:54  yosizaki
  remove all the sources. (moved to wireless_shared!)

  Revision 1.16  2004/10/28 13:11:06  yosizaki
  merge with ../wireless_shared.

  Revision 1.15  2004/10/25 10:19:35  yosizaki
  change include "wfs.h"

  Revision 1.14  2004/10/18 04:17:46  terui
  ビーコン間隔、スキャン間隔をばらつかせる仕様を追加。

  Revision 1.13  2004/10/05 10:22:53  yosizaki
  change packet-size within 5600[us].
  change WMpparam to WMParentParam.

  Revision 1.12  2004/10/05 09:45:29  terui
  MACアドレスをu8型の配列に統一。

  Revision 1.11  2004/10/04 13:44:06  terui
  ゲームグループIDをu32型に統一。

  Revision 1.10  2004/10/04 01:18:55  yosizaki
  fix MP-transfer size warning.

  Revision 1.9  2004/09/30 08:28:51  yosizaki
  add SDK_MAKEGGID_SYSTEM macro.

  Revision 1.8  2004/09/24 12:10:18  yosizaki
  change OS_Printf to OS_TPrintf.

  Revision 1.7  2004/09/11 04:13:46  yosizaki
  add comments.
  unify MP-size definition.

  Revision 1.6  2004/09/10 12:28:10  seiki_masashi
  キーシェアリングの送信バイト数をライブラリ内で隠蔽するように変更

  Revision 1.5  2004/09/04 13:26:46  yosizaki
  move WFS's callbacks to main.c

  Revision 1.4  2004/09/04 01:45:06  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.3  2004/09/03 09:49:18  seiki_masashi
  small fix

  Revision 1.2  2004/09/03 04:40:19  seiki_masashi
  送信失敗の errcode を WM_ERRCODE_SEND_FAILED に変更
  WM_ERRCODE_INVALID_POLLBITMAP への対応

  Revision 1.1  2004/09/03 01:11:58  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
