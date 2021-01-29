/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-1
  File:     wc.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.h,v $
  Revision 1.13  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.12  2004/10/22 11:05:49  terui
  wcモジュールは無線共有領域に切り出されました。

  Revision 1.11  2004/09/30 08:46:02  seiki_masashi
  GGID に割り振られた正式な値を設定

  Revision 1.10  2004/09/10 04:36:35  seiki_masashi
  送受信バッファを必要最低限のサイズに縮小

  Revision 1.9  2004/09/10 01:19:50  seiki_masashi
  最大接続子機数を WC_MAX_NUM_CHILD で設定するように変更

  Revision 1.8  2004/08/18 06:26:57  terui
  デフォルト接続チャンネルを変更

  Revision 1.7  2004/08/17 12:26:59  seiki_masashi
  WM_ERRCODE_FRAME_GAP の廃止に伴う修正

  Revision 1.6  2004/08/12 14:57:11  seiki_masashi
  WM_StepDataSharing の返り値の WM_ERRCODE_FRAME_GAP に対応

  Revision 1.5  2004/08/11 08:00:58  terui
  WcInitに引数を追加。

  Revision 1.4  2004/08/11 00:27:06  terui
  使用ポートを変更

  Revision 1.3  2004/08/10 06:36:48  terui
  WcGetAid追加。定数定義をwc.cから移動

  Revision 1.2  2004/08/10 02:11:32  terui
  共有データサイズを変更

  Revision 1.1  2004/08/10 00:41:40  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
