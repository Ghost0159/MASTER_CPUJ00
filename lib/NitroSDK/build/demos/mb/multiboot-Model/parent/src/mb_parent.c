/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-Model
  File:     mb_parent.c

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_parent.c,v $
  Revision 1.30  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.29  2004/11/12 09:01:50  yosizaki
  this file was moved to /demos/wireless_shared/mbp.

  Revision 1.28  2004/11/12 05:21:33  takano_makoto
  IsSameMacAddr関数をWM_IsBssidEqual関数に置き換え

  Revision 1.27  2004/11/12 04:38:32  takano_makoto
  不必要なincludeヘッダを削除

  Revision 1.26  2004/11/10 01:12:53  takano_makoto
  WM_CopyBssidを使用するように変更

  Revision 1.25  2004/11/02 18:08:42  takano_makoto
  fix comments.

  Revision 1.24  2004/11/02 09:15:23  takano_makoto
  MACアドレスの保存をMI_CpuCopyを使用しないように変更

  Revision 1.23  2004/10/28 01:31:09  takano_makoto
  wmhighをwireless_sharedのwhへ移行

  Revision 1.22  2004/10/27 02:07:41  takano_makoto
  不正な子機CONNECT時にすぐにはDisconnectせずにリクエストのあった時点でKickするように修正

  Revision 1.21  2004/10/26 09:18:32  takano_makoto
  MB_DisconnectChildを追加

  Revision 1.19  2004/10/21 00:42:42  yosizaki
  add SHARED_WH switch.

  Revision 1.18  2004/10/18 11:44:06  yosizaki
  change to use MB_StartParentEx/MB_EndEx (switch)

  Revision 1.17  2004/10/08 01:26:28  takano_makoto
  Init時のユーザ情報取得で、コピーするsrc, destを逆にしていた不具合を修正

  Revision 1.16  2004/10/05 09:45:29  terui
  MACアドレスをu8型の配列に統一。

  Revision 1.15  2004/10/05 07:49:36  takano_makoto
  子機のAcceptをmb_parent.cのコールバック内で処理するように修正

  Revision 1.14  2004/10/05 02:15:05  takano_makoto
  エントリー状態で待って、すべての子機で一斉にダウンロードを開始するように変更。

  Revision 1.13  2004/10/04 13:40:25  terui
  ゲームグループIDをu32型に統一。

  Revision 1.12  2004/10/04 10:09:08  takano_makoto
  MB_COMM_PSTATE_WAIT_TO_SENDの状態を追加

  Revision 1.11  2004/09/21 10:52:25  takano_makoto
  fix ASSERT in MBP_GetChildBmp().

  Revision 1.10  2004/09/21 09:12:49  yasu
  fix typo in MBP_GetChildBmp()

  Revision 1.9  2004/09/21 06:57:03  takano_makoto
  MB_COMM_PSTATE_ERRORのエラーコードに対する処理を追加

  Revision 1.8  2004/09/17 14:10:39  takano_makoto
  fix bugs in MBP_GetPlayerNo.

  Revision 1.7  2004/09/17 11:35:24  takano_makoto
  関数定義の記述順序を変更


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
