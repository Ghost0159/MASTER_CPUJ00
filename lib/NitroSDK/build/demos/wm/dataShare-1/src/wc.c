/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-1
  File:     wc.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.30  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.29  2004/10/22 11:05:54  terui
  wcモジュールは無線共有領域に切り出されました。

  Revision 1.28  2004/10/18 04:17:46  terui
  ビーコン間隔、スキャン間隔をばらつかせる仕様を追加。

  Revision 1.27  2004/10/05 09:45:29  terui
  MACアドレスをu8型の配列に統一。

  Revision 1.26  2004/10/04 13:40:25  terui
  ゲームグループIDをu32型に統一。

  Revision 1.25  2004/09/30 08:45:09  seiki_masashi
  GGID に割り振られた正式な値を設定

  Revision 1.24  2004/09/28 09:08:04  terui
  WMBssDescのlength比較がワード単位であることを想定していなかったバグを修正。

  Revision 1.23  2004/09/27 13:32:48  terui
  Scan時に直前のスキャンのGameInfoが残っている場合への対処

  Revision 1.22  2004/09/15 12:21:17  terui
  デバッグ出力に関する関数及び定義を変更。

  Revision 1.21  2004/09/10 12:08:10  seiki_masashi
  キーシェアリングの送信バイト数をライブラリ内で隠蔽するように変更

  Revision 1.20  2004/09/10 11:53:07  terui
  初期化、終了時の処理を３段階に分ける仕様への対応。

  Revision 1.19  2004/09/10 04:36:35  seiki_masashi
  送受信バッファを必要最低限のサイズに縮小

  Revision 1.18  2004/09/10 02:48:25  seiki_masashi
  small fix.

  Revision 1.17  2004/09/10 01:19:50  seiki_masashi
  最大接続子機数を WC_MAX_NUM_CHILD で設定するように変更

  Revision 1.16  2004/09/09 12:41:32  seiki_masashi
  子機最大送信データサイズを適正サイズに修正

  Revision 1.15  2004/09/03 04:42:13  seiki_masashi
  送信失敗の errcode を WM_ERRCODE_SEND_FAILED に変更

  Revision 1.14  2004/09/01 05:51:05  seiki_masashi
  MP.ind での WM_ERRCODE_INVALID_POLLBITMAP への対応

  Revision 1.13  2004/09/01 05:17:11  seiki_masashi
  WM_ERRCODE_INVALID_POLLBITMAP への対応

  Revision 1.12  2004/08/30 01:57:30  seiki_masashi
  WM_STATECODE_CHILD_CONNECTED を WM_STATECODE_CONNECTED に変更

  Revision 1.11  2004/08/27 08:03:16  terui
  ビーコンロスト時の処理を変更、接続完了時の処理を変更。

  Revision 1.10  2004/08/23 04:41:01  ooe
  WM_StartConnect()の仕様変更に対応。

  Revision 1.9  2004/08/20 04:31:55  terui
  最大接続子機台数を超えた場合のデバッグ出力を追加。

  Revision 1.8  2004/08/19 15:34:23  miya
  modified WMGameInfo structure.

  Revision 1.7  2004/08/19 02:55:17  terui
  エントリーによる接続制御仕様の導入に伴う改造。

  Revision 1.6  2004/08/18 00:28:35  seiki_masashi
  small fix

  Revision 1.5  2004/08/17 12:26:37  seiki_masashi
  WM_StartMP の引数に送信バッファを指定するように変更
  WM_ERRCODE_FRAME_GAP の廃止に伴う修正

  Revision 1.4  2004/08/12 14:57:11  seiki_masashi
  WM_StepDataSharing の返り値の WM_ERRCODE_FRAME_GAP に対応

  Revision 1.3  2004/08/11 08:01:29  terui
  WcInitに引数を追加し、WM_StartDataSharing時に利用するよう改造。

  Revision 1.2  2004/08/10 06:38:24  terui
  WcGetAid追加。コメント修正、不要関数削除などソースを整理。

  Revision 1.1  2004/08/10 00:41:54  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
