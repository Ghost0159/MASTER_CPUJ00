/*
  Project:  NitroSDK - CHT - demos - catch-min
  File:     wmscan.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wmscan.h,v $
  Revision 1.2  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.1  2005/07/14 01:34:40  kitase_hirotake
  Initial Release

  $NoKeywords$
 */
#include <nitro/types.h>
#include <nitro/wm.h>

typedef void (*WSStartScanCallbackFunc) (WMBssDesc *bssDesc);

enum
{
    WS_SYSSTATE_STOP,                  // 無線停止中
    WS_SYSSTATE_IDLE,                  // IDLE状態
    WS_SYSSTATE_SCANNING,              // スキャン中
    WS_SYSSTATE_SCANIDLE,              // スキャン停止中
    WS_SYSSTATE_BUSY,                  // ビジー中
    WS_SYSSTATE_ERROR,                 // エラー発生
    WS_SYSSTATE_FATAL                  // FATALエラー発生
};


/*---------------------------------------------------------------------------*
  Name:         WS_Initialize
  Description:  無線を初期化します。
  Arguments:    buf   WMに渡すバッファサイズです。WM_SYSTEM_BUF_SIZEの領域が必要です。
                dmaNo 無線で使用されるDMA番号を指定します。
  Returns:      処理が正常に開始された場合には TRUE
                開始されなかった場合には場合には FALSE
 *---------------------------------------------------------------------------*/
BOOL    WS_Initialize(void *buf, u16 dmaNo);

/*---------------------------------------------------------------------------*
  Name:         WS_StartScan
  Description:  親機のスキャンを開始します。
  Arguments:    callback 親機発見時のコールバック
                macAddr  親機のMACアドレスを指定します。
                         全親機を検索する場合は FF:FF:FF:FF:FF:FFを指定します
                continuous このフラグがTRUEの場合は、WS_EndScanがコールされるまでの間
                           有効な全チャンネルをスキャンし続けます。
                           FALSEの場合は、有効なチャンネルをローテーションしながら、
                           １スキャン毎にスキャンを停止し、WS_SYSSTATE_SCANIDLE状態に遷移します。
  Returns:      処理が正常に開始された場合には TRUE
                開始されなかった場合には場合には FALSE
 *---------------------------------------------------------------------------*/
BOOL    WS_StartScan(WSStartScanCallbackFunc callback, const u8 *macAddr, BOOL continuous);

/*---------------------------------------------------------------------------*
  Name:         WS_EndScan
  Description:  スキャンを終了する関数
  Arguments:    None.
  Returns:      処理が正常に開始された場合には TRUE
                開始されなかった場合には場合には FALSE
 *---------------------------------------------------------------------------*/
BOOL    WS_EndScan(void);

/*---------------------------------------------------------------------------*
  Name:         WS_End
  Description:  無線通信を終了する。
  Arguments:    None.
  Returns:      成功すれば真。
 *---------------------------------------------------------------------------*/
BOOL    WS_End(void);

/*---------------------------------------------------------------------------*
  Name:         WS_SetGgid
  Description:  ゲームグループIDを設定します。
                親機の接続前に呼び出します。
  Arguments:    ggid    設定するゲームグループID.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WS_SetGgid(u32 ggid);

/*---------------------------------------------------------------------------*
  Name:         WS_GetSystemState
  Description:  WS状態を取得します。
  Arguments:    None.
  Returns:      WS状態.
 *---------------------------------------------------------------------------*/
int     WS_GetSystemState(void);

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOnPictoCatch
  Description:  ピクトキャッチ機能を有効にする。
                WS_StartScanにてスキャン中に、ピクトチャットを発見した場合にも
                コールバック関数が呼ばれるようになる。
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WS_TurnOnPictoCatch(void);

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOffPictoCatch
  Description:  ピクトキャッチ機能を無効にする。
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WS_TurnOffPictoCatch(void);
