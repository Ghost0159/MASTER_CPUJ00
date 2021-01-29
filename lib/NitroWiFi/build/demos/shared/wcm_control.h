/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - shared - demos
  File:     wcm_control.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.h,v $
  Revision 1.9  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.8  2005/11/02 04:13:42  terui
  Fix comment.

  Revision 1.7  2005/09/13 13:13:29  terui
  DisconnectAP関数を追加。

  Revision 1.6  2005/09/08 10:42:38  adachi_hiroaki
  ラウンドスキャンの回数を取得できるようにした。またコールバック関数内のデバッグ表示を一回だけ表示されるようにした。

  Revision 1.5  2005/09/05 04:00:49  terui
  スキャン方式、電力管理モードを外部から変更可能なように修正。

  Revision 1.4  2005/08/29 06:39:02  yosizaki
  add SetWcmManualConnect(), ConnectAP().

  Revision 1.3  2005/08/22 12:10:19  yasu
  SiteDefs を参照するように修正

  Revision 1.2  2005/07/27 08:36:22  terui
  Add SetWcmThreadPriority API

  Revision 1.1  2005/07/21 08:38:16  terui
  サンプルで共有可能なソースファイルとして新規追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef SHARED_WCM_CONTROL_H_
#define SHARED_WCM_CONTROL_H_

#include "ap_info.h"

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl

  Description:  内部変数を初期化し、WCM コントロールスレッドを起動する。
                既にスレッドが起動している場合は何も行わない。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitWcmControl(void);
void    InitWcmControlByApInfo(const WcmControlApInfo* ptr);

/*---------------------------------------------------------------------------*
  Name:         InitWcmApInfo

  Description:  アクセスポイント接続パラメータを環境設定値から取得して
                初期化する

  Arguments:    apinfo   接続パラメータ
                apclass  アクセスポイントのクラス名

  Returns:      TRUE   成功
                FALSE  失敗
 *---------------------------------------------------------------------------*/
BOOL    InitWcmApInfo(WcmControlApInfo* apinfo, const char* apclass);

/*---------------------------------------------------------------------------*
  Name:         TerminateWcmControl

  Description:  WCM コントロールスレッドを終了する。
                スレッドが起動されていない場合は何も行わない。
    NOTICE:     当関数はスレッドを終了する為のトリガとなるだけなので、スレッド
                自体が終了するまで待たずに戻る点に注意。
                スレッド自体の終了を待ちたい場合は、OS_SendMessage 呼び出しに
                続いて OS_JoinThread を呼び出す必要がある。
                なお、WCM_GetPhase 関数が WCM_PHASE_NULL を返すようになれば
                スレッドは終了している。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    TerminateWcmControl(void);

/*---------------------------------------------------------------------------*
  Name:         SetWcmThreadPriority

  Description:  WCM ライブラリの状態管理を行うスレッドの優先度を変更する。

  Arguments:    priority    -   0 ～ 31 のスレッド優先度を指定する。

  Returns:      u8  -   変更する前に設定されていた優先度を返す。
                        なんらかの理由で優先度の変更に失敗した場合は 0xff を返す。
 *---------------------------------------------------------------------------*/
u8      SetWcmThreadPriority(u8 priority);

/*---------------------------------------------------------------------------*
  Name:         GetWcmSearchAroundCount

  Description:  全チャンネルを何週スキャンしたかを返す。
                StartUp直後は0になる。

  Arguments:    None.

  Returns:      何回スキャンしたか。
 *---------------------------------------------------------------------------*/
u32     GetWcmSearchAroundCount(void);

/*---------------------------------------------------------------------------*
  Name:         SetWcmManualConnectMode

  Description:  APのリストから手動で選択して接続するよう設定.
                デフォルトでは FALSE (自動接続)となっている.
                InitWcmControl() より先に呼び出す必要がある.

  Arguments:    enable -   手動で接続する場合は TRUE。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SetWcmManualConnect(BOOL enable);

/*---------------------------------------------------------------------------*
  Name:         ConnectAP

  Description:  指定されたBssDescのAPへ接続する

  Arguments:    bd     -   接続するAPを示す構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ConnectAP(const WMBssDesc* pbd);

/*---------------------------------------------------------------------------*
  Name:         DisconnectAP

  Description:  AP との接続の切断を試みる

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    DisconnectAP(void);

/*---------------------------------------------------------------------------*
  Name:         SetWcmScanType

  Description:  AP を探索する際のスキャン方式を設定する。
                デフォルトではアクティブスキャン方式となっている。
                InitWcmControl() より先に呼び出す必要がある。

  Arguments:    type    -   WCM_OPTION_SCANTYPE_* の定数を指定する。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SetWcmScanType(u32 type);

/*---------------------------------------------------------------------------*
  Name:         SetWcmPowerMode

  Description:  AP と通信する際の電力管理方式を指定する。
                デフォルトでは常時アクティブ方式となっている。
                InitWcmControl() より先に呼び出す必要がある。

  Arguments:    mode    -   WCM_OPTION_POWER_* の定数を指定する。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SetWcmPowerMode(u32 mode);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* SHARED_WCM_CONTROL_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
