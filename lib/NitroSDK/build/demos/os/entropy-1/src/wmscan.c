/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CHT - demos - wmscan
  File:     chtmin.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wmscan.c,v $
  Revision 1.4  2007/10/29 00:29:31  seiki_masashi
  Copyright date

  Revision 1.3  2007/10/22 11:12:36  seiki_masashi
  不要な処理の除去

  Revision 1.2  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.1  2005/07/27 07:43:35  seiki_masashi
  新規追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include    <nitro.h>
#include    <nitro/cht.h>
#include    "wmscan.h"


static int sSysState = WS_SYSSTATE_STOP;        // wmscan内部状態
static WSStartScanCallbackFunc sScanCallback = NULL;    // 親機発見時のコールバックポインタ
static WMBssDesc sBssDesc ATTRIBUTE_ALIGN(32);  // 親機データ取得用バッファ
static WMScanParam sScanParam ATTRIBUTE_ALIGN(32);      // スキャンパラメータ構造体
static BOOL sContinuousScanFlag;       // 連続スキャンフラグ
static BOOL sPictoCatchFlag = FALSE;   // ピクトキャッチフラグ
static u32 sGgid = 0;                  // GGID

#define WS_ASSERT(exp) \
    (void) ((exp) || (OSi_Panic(__FILE__, __LINE__, "Failed assertion " #exp), 0))

static void WS_ChangeSysState(int state);
static void WS_StateOutInitialize(void *arg);
static BOOL WS_StateInStartScan(void);
static void WS_StateOutStartScan(void *arg);
static BOOL WS_StateInEndScan(void);
static void WS_StateOutEndScan(void *arg);
static void WS_StateOutEnd(void *arg);


/*---------------------------------------------------------------------------*
  Name:         WS_ChangeSysState
  Description:  WS状態の変更
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void WS_ChangeSysState(int state)
{
    sSysState = state;
}

/*---------------------------------------------------------------------------*
  Name:         WS_GetSystemState

  Description:  WS状態を取得します。

  Arguments:    None.

  Returns:      WS状態.
 *---------------------------------------------------------------------------*/
int WS_GetSystemState(void)
{
    return sSysState;
}

/*---------------------------------------------------------------------------*
  Name:         WS_Initialize

  Description:  無線を初期化します。

  Arguments:    buf   WMに渡すバッファサイズです。WM_SYSTEM_BUF_SIZEの領域が必要です。
                dmaNo 無線で使用されるDMA番号を指定します。

  Returns:      処理が正常に開始された場合には TRUE
                開始されなかった場合には場合には FALSE
 *---------------------------------------------------------------------------*/
BOOL WS_Initialize(void *buf, u16 dmaNo)
{
    WMErrCode result;

    SDK_NULL_ASSERT(buf);

    WS_ChangeSysState(WS_SYSSTATE_BUSY);
    result = WM_Initialize(buf, WS_StateOutInitialize, dmaNo);
    if (result != WM_ERRCODE_OPERATING)
    {
        WS_ChangeSysState(WS_SYSSTATE_FATAL);
        return FALSE;
    }
    sScanParam.channel = 0;
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         WS_StateOutInitialize
  Description:  無線の初期化を実行します。
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WS_StateOutInitialize(void *arg)
{
    // 電源投入後状態です。
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WS_ChangeSysState(WS_SYSSTATE_FATAL);
        return;
    }

    // システム状態をアイドリング（待機中）に変更。
    WS_ChangeSysState(WS_SYSSTATE_IDLE);

    // 次の状態をセットしないので、ここでシーケンスはいったん終了です。
}

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOnPictoCatch

  Description:  ピクトキャッチ機能を有効にする。
                WS_StartScanにてスキャン中に、ピクトチャットを発見した場合にも
                コールバック関数が呼ばれるようになる。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WS_TurnOnPictoCatch(void)
{
    sPictoCatchFlag = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOffPictoCatch

  Description:  ピクトキャッチ機能を無効にする。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WS_TurnOffPictoCatch(void)
{
    sPictoCatchFlag = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WS_SetGgid

  Description:  ゲームグループIDを設定します。
                親機の接続前に呼び出します。

  Arguments:    ggid    設定するゲームグループID.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WS_SetGgid(u32 ggid)
{
    sGgid = ggid;
}


/*---------------------------------------------------------------------------*
  Name:         WS_StartScan

  Description:  親機のスキャンを開始します。

  Arguments:    callback   親機発見時のコールバック
                macAddr    親機のMACアドレスを指定します。
                           全親機を検索する場合は FF:FF:FF:FF:FF:FFを指定します
                continuous このフラグがTRUEの場合は、WS_EndScanがコールされるまでの間
                           有効な全チャンネルをスキャンし続けます。
                           FALSEの場合は、有効なチャンネルをローテーションしながら、
                           １スキャン毎にスキャンを停止し、WS_SYSSTATE_SCANIDLE状態に遷移します。

  Returns:      処理が正常に開始された場合には TRUE
                開始されなかった場合には場合には FALSE
 *---------------------------------------------------------------------------*/
BOOL WS_StartScan(WSStartScanCallbackFunc callback, const u8 *macAddr, BOOL continuous)
{
    OSIntrMode enabled;

    enabled = OS_DisableInterrupts();

    sScanCallback = callback;
    sContinuousScanFlag = continuous;

    // 検索するMACアドレスの条件を設定
    *(u16 *)(&sScanParam.bssid[4]) = *(u16 *)(macAddr + 4);
    *(u16 *)(&sScanParam.bssid[2]) = *(u16 *)(macAddr + 2);
    *(u16 *)(&sScanParam.bssid[0]) = *(u16 *)(macAddr);

    (void)OS_RestoreInterrupts(enabled);

    if (sSysState == WS_SYSSTATE_SCANNING)
    {
        return TRUE;
    }

    WS_ChangeSysState(WS_SYSSTATE_SCANNING);

    if (!WS_StateInStartScan())
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
  state : StartScan
  ---------------------------------------------------------------------- */
static BOOL WS_StateInStartScan(void)
{
    // この状態の時、親機を探索します。
    WMErrCode result;
    u16     chanpat;

    WS_ASSERT(sSysState == WS_SYSSTATE_SCANNING);

    chanpat = WM_GetAllowedChannel();

    // 無線が使用可能かどうかをチェックする
    if (chanpat == 0x8000)
    {
        // 0x8000 が返ってきた場合は、無線が初期化されていないなど
        // 無線ライブラリの状態異常を表しているのでエラーにします。
        return FALSE;
    }
    if (chanpat == 0)
    {
        // 無線が使えない状態。
        return FALSE;
    }

    /* 現在の指定から昇順に、可能なチャンネルを検索します */
    while (TRUE)
    {
        sScanParam.channel++;
        if (sScanParam.channel > 16)
        {
            sScanParam.channel = 1;
        }

        if (chanpat & (0x0001 << (sScanParam.channel - 1)))
        {
            break;
        }
    }

    sScanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    sScanParam.scanBuf = &sBssDesc;
    result = WM_StartScan(WS_StateOutStartScan, &sScanParam);

    if (result != WM_ERRCODE_OPERATING)
    {
        return FALSE;
    }
    return TRUE;
}

static void WS_StateOutStartScan(void *arg)
{
    WMstartScanCallback *cb = (WMstartScanCallback *)arg;

    // スキャンコマンドに失敗した場合
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);
        return;
    }

    if (sSysState != WS_SYSSTATE_SCANNING)
    {
        // 状態が変更されていればスキャン終了
        if (!WS_StateInEndScan())
        {
            WS_ChangeSysState(WS_SYSSTATE_ERROR);
        }
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;

    case WM_STATECODE_PARENT_FOUND:
        // 親機が見つかった場合
        // ggid を比較し、違っていたら失敗とします。
        // まず、WMBssDesc.gameInfoLength を確認し、
        // ggid に有効な値が入っていることから調べる必要があります。

        // BssDescの情報がARM7側から書き込まれているため
        // バッファに設定されたBssDescのキャッシュを破棄
        DC_InvalidateRange(&sBssDesc, sizeof(WMbssDesc));

        // 親機がピクトチャットかどうか判定
        if (sPictoCatchFlag)
        {
            if (CHT_IsPictochatParent(&sBssDesc))
            {
                // 親機がピクトチャットである場合
                if (sScanCallback != NULL)
                {
                    sScanCallback(&sBssDesc);
                }
                break;
            }
        }

        if (cb->gameInfoLength < 8 || cb->gameInfo.ggid != sGgid)
        {
            // GGIDが違っていれば無視する
            break;
        }

        // エントリーフラグが立っていなければ子機を受付中でないので無視する
        if (!(cb->gameInfo.gameNameCount_attribute & WM_ATTR_FLAG_ENTRY))
        {
            break;
        }

        // コールバックが必要ならば呼び出し
        if (sScanCallback != NULL)
        {
            sScanCallback(&sBssDesc);
        }

        break;
    }

    if (!sContinuousScanFlag)
    {
        WS_ChangeSysState(WS_SYSSTATE_SCANIDLE);
        return;
    }

    // チャンネルを変更して再スキャンを開始します。
    if (!WS_StateInStartScan())
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);
    }
}


/*---------------------------------------------------------------------------*
  Name:         WS_EndScan

  Description:  スキャンを終了する関数

  Arguments:    None.

  Returns:      処理が正常に開始された場合には TRUE
                開始されなかった場合には場合には FALSE
 *---------------------------------------------------------------------------*/
BOOL WS_EndScan(void)
{
    if (sSysState == WS_SYSSTATE_SCANIDLE)
    {
        return WS_StateInEndScan();
    }

    if (sSysState != WS_SYSSTATE_SCANNING)
    {
        return FALSE;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();
        sScanCallback = NULL;
        (void)OS_RestoreInterrupts(enabled);
    }

    WS_ChangeSysState(WS_SYSSTATE_BUSY);
    return TRUE;
}


static BOOL WS_StateInEndScan(void)
{
    WMErrCode result;

    // この状態では、スキャンの終了処理を行います。
    result = WM_EndScan(WS_StateOutEndScan);
    if (result != WM_ERRCODE_OPERATING)
    {
        return FALSE;
    }

    return TRUE;
}

static void WS_StateOutEndScan(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        return;
    }

    WS_ChangeSysState(WS_SYSSTATE_IDLE);
}


/*---------------------------------------------------------------------------*
  Name:         WS_End

  Description:  無線通信を終了する。

  Arguments:    None.

  Returns:      成功すれば真。
 *---------------------------------------------------------------------------*/
BOOL WS_End(void)
{
    WS_ASSERT(sSysState == WS_SYSSTATE_IDLE);

    WS_ChangeSysState(WS_SYSSTATE_BUSY);
    if (WM_End(WS_StateOutEnd) != WM_ERRCODE_OPERATING)
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);

        return FALSE;
    }
    return TRUE;
}

/* ----------------------------------------------------------------------
  state : WS_StateOutEnd
  ---------------------------------------------------------------------- */
static void WS_StateOutEnd(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WS_ChangeSysState(WS_SYSSTATE_FATAL);
        return;
    }
    WS_ChangeSysState(WS_SYSSTATE_STOP);
}
