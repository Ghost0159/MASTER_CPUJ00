/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless_shared
  File:     wc.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.c,v $
  Revision 1.29  2007/10/03 05:39:13  yosizaki
  change to use WH.

  Revision 1.28  2007/02/20 00:28:09  kitase_hirotake
  indent source

  Revision 1.27  2006/06/14 01:42:12  okubata_ryoma
  Reset時にWM_EndDataSharing関数を呼ぶ処理を追加

  Revision 1.26  2006/04/10 13:29:43  yosizaki
  fix about WcSetBlockTransferCallback.

  Revision 1.25  2006/02/20 08:04:30  seiki_masashi
  WM_IsValidGameBeacon 関数を使用するように変更

  Revision 1.24  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.23  2006/01/10 02:51:04  adachi_hiroaki
  16進表記の出力には全て明示的に0xをつけた。/([^xX])(%\d*[xX][^hH])/ -> \10x\2

  Revision 1.22  2006/01/06 08:20:25  kitase_hirotake
  デバッグ出力を明示的に16進表記に

  Revision 1.21  2005/10/28 11:24:57  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF の追加に対応

  Revision 1.20  2005/06/17 06:34:45  miya
  子機切断時にＷＢＴ用のコールバックを追加

  Revision 1.19  2005/04/26 02:35:02  terui
  Fix comment

  Revision 1.18  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.17  2005/02/18 12:06:07  seiki_masashi
  デバッグ表示を修正

  Revision 1.16  2005/02/18 09:26:50  yosizaki
  fix around hidden warnings.

  Revision 1.15  2005/02/08 07:03:16  terui
  WcFinish関数内でコールバック関数のクリアを先に行うように修正。

  Revision 1.14  2005/02/02 04:21:20  terui
  TurnOff のつづり間違いを修正。

  Revision 1.13  2005/01/11 07:48:40  takano_makoto
  fix copyright header.

  Revision 1.12  2005/01/11 04:59:46  terui
  WcFinish関数を追加。

  Revision 1.11  2005/01/07 12:03:56  terui
  復旧不能なエラー通知を受信した際の処理を追加。

  Revision 1.10  2004/12/27 05:29:46  terui
  StartConnectコールバック内でWc_Resetを呼び出す条件を追加。

  Revision 1.9  2004/12/22 02:47:29  terui
  ピクトチャットサーチに対応

  Revision 1.8  2004/11/19 08:22:37  terui
  GGIDチェック部分のチェック方法を変更。

  Revision 1.7  2004/11/10 06:24:45  terui
  子機が親からDisconnectされた際の分岐処理を省略。

  Revision 1.6  2004/10/29 04:31:43  terui
  リセット時の状態管理における不具合を修正。

  Revision 1.5  2004/10/29 03:37:21  seiki_masashi
  コメントの修正

  Revision 1.4  2004/10/29 01:55:33  terui
  子機のスキャン時のチャンネルをWcInitに渡されたチャンネルを使用するよう修正。

  Revision 1.3  2004/10/28 14:09:10  yosizaki
  change to use wireless_shared.

  Revision 1.2  2004/10/28 11:20:16  yosizaki
  merge with wbt-1.

  Revision 1.1  2004/10/22 11:04:14  terui
  wcモジュールを共有領域に切り出し。

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#if 1
// ワイヤレス制御のサンプルはWHへ統合されました
#include "../wh/wh.c"
#else
#include    <nitro.h>
#include    <nitro/cht.h>
#include    "wc.h"


/*---------------------------------------------------------------------------*
    デバッグ用コンパイルスイッチ
 *---------------------------------------------------------------------------*/
#define     WC_DEBUG_PRINT             // デバッグ出力切り替えスイッチ


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
static void WcCreateParentParameter(void);      // 親機情報編集
static void WcCreateScanParameter(void);        // スキャン設定編集
static void WcSetNextScanCannel(void);

// WMからのコールバック関数
static void WcCb_Enable(void *arg);
static void WcCb_Disable(void *arg);
static void WcCb_PowerOn(void *arg);
static void WcCb_PowerOff(void *arg);
static void WcCb_Indicate(void *arg);
static void WcCb_Reset(void *arg);
static void WcCb_SetParentParameter(void *arg);
static void WcCb_StartParent(void *arg);
static void WcCb_StartMP(void *arg);
static void WcCb_StartScan(void *arg);
static void WcCb_EndScan(void *arg);
static void WcCb_StartConnect(void *arg);
static void WcCb_SetMPData(void *arg);
static void WcCb_ReceiveData(void *arg);

// WM呼び出し単機能関数
static BOOL Wc_Enable(void);
static BOOL Wc_Disable(void);
static BOOL Wc_PowerOn(void);
static BOOL Wc_PowerOff(void);
static BOOL Wc_SetIndCallback(void);
static BOOL Wc_Reset(void);
static BOOL Wc_SetParentParameter(void);
static BOOL Wc_StartParent(void);
static BOOL Wc_StartScan(void);
static BOOL Wc_EndScan(void);
static BOOL Wc_StartConnect(void);
static BOOL Wc_StartMP(void);
static BOOL Wc_StartDataSharing(void);
static BOOL Wc_SetMPData(const void *buf, u16 size);

// プリント出力抑制ダミー関数
#ifdef  WC_DEBUG_PRINT
#define     WcDebugPrintf       OS_TPrintf
#else
#define     WcDebugPrintf(...)  ((void) 0)
#endif


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static WCStatus wcStatus;              // 現在の状態を管理
static WCStatus wcTarget;              // 状態変更の目標を管理
static u16 wcAid;                      // AID(子機時)
static u16 wcFramePeriod;              // ゲームフレーム間隔
static BOOL wcDataShareFlag;           // データシェア / 通常MP 切り替えフラグ
static BOOL wcStartDataShareFlag;      // データシェアリング中フラグ 
static BOOL wcBlockTransFlag;          // ブロック転送 / 通常MP 切り替えフラグ
static BOOL wcSendFlag;                // 送信完了フラグ(通常MPモード時)

static WMParentParam *wcDefaultParameter;
static WCCallbackFunc wcMpRecvCallback;
static WCBlockTransferCallbackFunc wcBtCallback;

static void *wcWmBuffer;               // 実体はキャッシュセーフなバッファ
static WMParentParam *wcParentParameter;        // 実体はキャッシュセーフなバッファ
static WMScanParam *wcScanParameter;   // 実体はキャッシュセーフなバッファ
static WMBssDesc *wcWmBssDesc;         // 実体はキャッシュセーフなバッファ
static s32 wcSendBufferSize;           // 32の倍数
static void *wcSendBuffer;             // 実体はキャッシュセーフなバッファ
static s32 wcRecvBufferSize;           // 32の倍数
static void *wcRecvBuffer;             // 実体はキャッシュセーフなバッファ
static WMDataSharingInfo *wcDataSharingInfo;
static WMDataSet *wcDataSet;

static BOOL wcPictoCatchStatus;
static WCPictoCatchCallbackFunc wcPictoCatchCallback;


/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         WcInit

  Description:  無線通信を初期化する

  Arguments:    pp          -   無線通信における親機としてのパラメータをポインタ
                                で指定します。WCモジュールを駆動中に内容を変更
                                したりしないで下さい。
                dsFlag      -   データシェアフラグです。
                                TRUEを指定するとデータシェアモードになります。
                                FALSEを指定するとMPを直接操作するモードになります。
                btFlag      -   ブロック転送フラグです。
                                TRUEを指定するとブロック転送モードになります。
                framePeriod -   ゲームフレームごとのピクチャーフレーム数。
                                データシェアモードの場合のみ活用されます。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod)
{
    // ステート初期化
    wcStatus = WC_STATUS_READY;
    wcAid = 0;
    wcFramePeriod = framePeriod;
    wcDataShareFlag = dsFlag;
    wcStartDataShareFlag = FALSE;
    wcBlockTransFlag = btFlag;
    wcDefaultParameter = (WMParentParam *)pp;
    // 送信フラグ初期化
    {
        wcSendFlag = FALSE;
    }
    // ユーザーコールバック初期化
    wcMpRecvCallback = NULL;
    wcBtCallback = NULL;

    // WM用メモリ確保
    wcWmBuffer = OS_Alloc(WM_SYSTEM_BUF_SIZE);
    wcParentParameter = (WMParentParam *)OS_Alloc(sizeof(WMParentParam));
    wcScanParameter = (WMScanParam *)OS_Alloc(sizeof(WMScanParam));
    wcWmBssDesc = (WMBssDesc *)OS_Alloc(sizeof(WMBssDesc));
    if (wcDataShareFlag)
    {
        wcDataSharingInfo = (WMDataSharingInfo *)OS_Alloc(sizeof(WMDataSharingInfo));
        wcDataSet = (WMDataSet *)OS_Alloc(sizeof(WMDataSet));
    }
    wcPictoCatchStatus = 0;

    // 送信バッファ用メモリ確保
    {
        s32     parentSize;
        s32     childSize;

        parentSize = WM_SIZE_MP_PARENT_SEND_BUFFER(pp->parentMaxSize, FALSE);
        childSize = WM_SIZE_MP_CHILD_SEND_BUFFER(pp->childMaxSize, FALSE);
        wcSendBufferSize = (parentSize > childSize) ? parentSize : childSize;
    }
    wcSendBuffer = OS_Alloc((u32)wcSendBufferSize);

    // 受信バッファ用メモリ確保
    {
        s32     parentSize;
        s32     childSize;

        parentSize = (s32)WM_SIZE_MP_PARENT_RECEIVE_BUFFER(pp->childMaxSize, pp->maxEntry, FALSE);
        childSize = (s32)WM_SIZE_MP_CHILD_RECEIVE_BUFFER(pp->parentMaxSize, FALSE);
        wcRecvBufferSize = (parentSize > childSize) ? parentSize : childSize;
    }
    wcRecvBuffer = OS_Alloc((u32)wcRecvBufferSize);

    // WMライブラリ初期化
    (void)WM_Init(wcWmBuffer, WC_DMA_NO);
}

/*---------------------------------------------------------------------------*
  Name:         WcFinish

  Description:  無線通信を初期化前の状態に戻す。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcFinish(void)
{
    WMErrCode wmResult;

    // ポート受信コールバッククリア
    {
        WcDebugPrintf("> WM_SetPortCallback start.\n");
        wmResult = WM_SetPortCallback(WC_DEFAULT_PORT, NULL, NULL);
        if (wmResult == WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_SetPortCallback success.\n");
        }
        else
        {
            WcDebugPrintf("< WM_SetPortCallback failed. ERRCODE: 0x%02x\n", wmResult);
        }
    }

    switch (wcStatus)
    {
    case WC_STATUS_READY:
    case WC_STATUS_ERROR:
        // WMライブラリ停止
        WcDebugPrintf("> WM_Finish start.\n");
        wmResult = WM_Finish();
        if (wmResult == WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_Finish success.\n");
            break;
        }
        WcDebugPrintf("< WM_Finish failed. ERRCODE: 0x%02x\n", wmResult);
    default:
        WcDebugPrintf("* Could not finish Wireless library\n");
        return;
    }

    // メモリ解放
    OS_Free(wcRecvBuffer);
    OS_Free(wcSendBuffer);
    if (wcDataShareFlag)
    {
        OS_Free(wcDataSet);
        OS_Free(wcDataSharingInfo);
    }
    OS_Free(wcWmBssDesc);
    OS_Free(wcScanParameter);
    OS_Free(wcParentParameter);
    OS_Free(wcWmBuffer);
}

/*---------------------------------------------------------------------------*
  Name:         WcStartParent

  Description:  親機として無線通信を開始する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcStartParent(void)
{
    switch (wcStatus)
    {
    case WC_STATUS_READY:
        // 親機として初期化開始
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_PARENT;   // 目標状態を"親機接続状態"に
        (void)Wc_Enable();
        break;
    case WC_STATUS_BUSY:
        // 目標状態を更新
        wcTarget = WC_STATUS_PARENT;   // 目標状態を"親機接続状態"に
        break;
    case WC_STATUS_PARENT:
        // 既に親機なので処理なし
        break;
    case WC_STATUS_CHILD:
        // 一旦リセットする
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_PARENT;   // 目標状態を"親機接続状態"に
        (void)Wc_Reset();
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcStartChild

  Description:  子機として無線通信を開始する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcStartChild(void)
{
    switch (wcStatus)
    {
    case WC_STATUS_READY:
        // 子機として初期化開始
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_CHILD;    // 目標状態を"子機接続状態"に
        (void)Wc_Enable();
        break;
    case WC_STATUS_BUSY:
        // 目標状態を更新
        wcTarget = WC_STATUS_CHILD;    // 目標状態を"子機接続状態"に
        break;
    case WC_STATUS_PARENT:
        // 一旦リセットする
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_CHILD;    // 目標状態を"子機接続状態"に
        (void)Wc_Reset();
        break;
    case WC_STATUS_CHILD:
        // 既に子機なので処理なし
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcEnd

  Description:  通信前の状態に戻す

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcEnd(void)
{
    switch (wcStatus)
    {
    case WC_STATUS_READY:
        // 既にREADY状態なので処理なし
        break;
    case WC_STATUS_BUSY:
        // 目標状態を更新
        wcTarget = WC_STATUS_READY;    // 目標状態を"READY"に
        break;
    case WC_STATUS_PARENT:
    case WC_STATUS_CHILD:
        // リセットすることで接続を切断する
        wcStatus = WC_STATUS_BUSY;
        wcTarget = WC_STATUS_READY;    // 目標状態を"READY"に
        (void)Wc_Reset();
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcGetStatus

  Description:  無線通信の現在のステートを取得する。

  Arguments:    None.

  Returns:      現在のステートを返す。
 *---------------------------------------------------------------------------*/
WCStatus WcGetStatus(void)
{
    return wcStatus;
}

/*---------------------------------------------------------------------------*
  Name:         WcGetAid

  Description:  無線通信グループにおいて割り当てられたAIDを取得する。

  Arguments:    None.

  Returns:      割り当てられたAIDを返す。
 *---------------------------------------------------------------------------*/
u16 WcGetAid(void)
{
    return wcAid;
}

/*---------------------------------------------------------------------------*
  Name:         WcStepDataSharing

  Description:  データシェアの同期を１つ進める。

  Arguments:    send   - 共有するデータへのポインタ。データ長はWC_SHARED_DATA_SIZE
                         に固定となる。32-Byte Align されている必要がある。
                bitmap - 通信グループに含まれる端末を示すビットマップを取得する
                         変数へのポインタ。

  Returns:      BOOL   - 処理に成功し、シェアされたデータが取得できた場合に
                         TRUE を返す。
                         取得したデータはWcGetSharedDataAddress関数で各端末からの
                         データへのアドレスを取得する。
                         なんらかの理由で失敗した場合は FALSE を返す。
 *---------------------------------------------------------------------------*/
BOOL WcStepDataSharing(const void *send, u16 *bitmap)
{
    WMErrCode wmResult;

    // データシェアモードでない時は実行不可
    if (!wcDataShareFlag)
    {
        return FALSE;
    }

    // データシェアの同期を１つ進める
    WcDebugPrintf("> WM_StepDataSharing start.\n");
    wmResult = WM_StepDataSharing(wcDataSharingInfo, send, wcDataSet);

    // 処理結果を判定
    if (wmResult != WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_StepDataSharing failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }

    // 接続済み端末のビットmapを格納
    *bitmap = wcDataSet->receivedBitmap;

    WcDebugPrintf("< WM_StepDataSharing success.\n");
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WcGetSharedDataAddress

  Description:  データシェアにて取得したデータから、指定AIDの端末からのデータ
                へのアドレスを取得する。

  Arguments:    aid - データを取得する端末のAIDを指定する。親機は"0"。

  Returns:      u8* - 指定端末からのデータへのアドレスを返す。データ長は
                      WC_SHARED_DATA_SIZEに固定となる。
                      指定端末からのデータが存在しない場合など、処理に失敗した
                      場合にはNULLを返す。
 *---------------------------------------------------------------------------*/
u8     *WcGetSharedDataAddress(u16 aid)
{
    // データシェアモードでない時は実行不可
    if (!wcDataShareFlag)
    {
        return NULL;
    }

    return (u8 *)WM_GetSharedDataAddress(wcDataSharingInfo, wcDataSet, aid);
}

/*---------------------------------------------------------------------------*
  Name:         WcSetParentData

  Description:  親機として送信するデータを設定する。

  Arguments:    buf  - 送信するデータへのポインタ。
                       (32-Byte Align されている必要がある)
                size - 送信するデータのサイズ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetParentData(const void *buf, u16 size)
{
    // 送信データサイズ確認
    if (size > wcParentParameter->parentMaxSize)
    {
        return;
    }
    // 前の送信の完了を確認
    if (wcSendFlag)
    {
        wcSendFlag = FALSE;

        (void)Wc_SetMPData(buf, size);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcSetChildData

  Description:  子機として送信するデータを設定する。

  Arguments:    buf  - 送信するデータへのポインタ。
                       (32-Byte Align されている必要がある)
                size - 送信するデータのサイズ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetChildData(const void *buf, u16 size)
{
    // 送信データサイズ確認
    if (size > wcDefaultParameter->childMaxSize)
    {
        return;
    }
    // 前の送信の完了を確認
    if (wcSendFlag)
    {
        wcSendFlag = FALSE;

        (void)Wc_SetMPData(buf, size);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcSetMpReceiveCallback

  Description:  MP通信にてデータを受信した時のコールバック関数を設定する。

  Arguments:    cb  -   コールバック関数を指定。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetMpReceiveCallback(WCCallbackFunc cb)
{
    wcMpRecvCallback = cb;
}

/*---------------------------------------------------------------------------*
  Name:         WcSetBlockTransferCallback

  Description:  MP通信にてデータを受信した時のコールバック関数を設定する。

  Arguments:    cb  -   コールバック関数を指定。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb)
{
    wcBtCallback = cb;
}

/*---------------------------------------------------------------------------*
  Name:         WcTurnOnPictoCatch

  Description:  ピクトチャットサーチ機能を有効にする。

  Arguments:    func    -   ピクトチャットのビーコン情報を発見した時に
                            呼び出されるコールバック関数を指定する。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func)
{
    wcPictoCatchStatus = 1;
    wcPictoCatchCallback = func;
}

/*---------------------------------------------------------------------------*
  Name:         WcTurnOffPictoCatch

  Description:  ピクトチャットサーチ機能を無効にする。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WcTurnOffPictoCatch(void)
{
    wcPictoCatchStatus = 0;
    wcPictoCatchCallback = NULL;
}

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         WcCreateParentParameter

  Description:  親機情報を編集する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCreateParentParameter(void)
{
    static u16 tempID;

    // 親機情報をキャッシュStoreしても問題ないバッファにコピー
    MI_CpuCopy32(wcDefaultParameter, wcParentParameter, sizeof(WMParentParam));
    // ビーコン間隔を適度な値に設定
    wcParentParameter->beaconPeriod = WM_GetDispersionBeaconPeriod();
    // tempIDをインクリメント
    wcParentParameter->tgid = ++tempID;
}

/*---------------------------------------------------------------------------*
  Name:         WcCreateScanParameter

  Description:  スキャン設定情報を編集する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCreateScanParameter(void)
{
    wcScanParameter->scanBuf = wcWmBssDesc;
    wcScanParameter->channel = wcDefaultParameter->channel;
    // 探索限度時間を適度な値に設定
    wcScanParameter->maxChannelTime = WM_GetDispersionScanPeriod();
    // ブロードキャストアドレスを指定し、全親機を探索するよう設定
    wcScanParameter->bssid[0] = 0xff;
    wcScanParameter->bssid[1] = 0xff;
    wcScanParameter->bssid[2] = 0xff;
    wcScanParameter->bssid[3] = 0xff;
    wcScanParameter->bssid[4] = 0xff;
    wcScanParameter->bssid[5] = 0xff;
}

/*---------------------------------------------------------------------------*
  Name:         WcSetNextScanCannel

  Description:  スキャン設定内のチャンネル指定を次のチャンネルに更新する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcSetNextScanCannel(void)
{
    u16     allowedChannel;
    s32     i;

    allowedChannel = WM_GetAllowedChannel();
    if (allowedChannel == 0)
        return;
    for (i = 0; i < 16; i++)
    {
        if (allowedChannel & (0x0001 << ((wcScanParameter->channel + i) % 16)))
        {
            wcScanParameter->channel = (u16)(((wcScanParameter->channel + i) % 16) + 1);
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Enable

  Description:  WM_Enableに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Enable(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_Enable success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Disable();
            break;
        case WC_STATUS_PARENT:
        case WC_STATUS_CHILD:
            (void)Wc_PowerOn();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_Enable failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Disable

  Description:  WM_Disableに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Disable(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_Disable success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            wcStatus = WC_STATUS_READY; // 正常にREADY状態に遷移完了
            break;
        case WC_STATUS_PARENT:
            wcStatus = WC_STATUS_READY;
            WcStartParent();
            break;
        case WC_STATUS_CHILD:
            wcStatus = WC_STATUS_READY;
            WcStartChild();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_Disable failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_PowerOn

  Description:  WM_PowerOnに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_PowerOn(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_PowerOn success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_PowerOff();
            break;
        case WC_STATUS_PARENT:
            if (Wc_SetIndCallback())
            {
                WcCreateParentParameter();
                if (!Wc_SetParentParameter())
                {
                    wcTarget = WC_STATUS_READY; // 目標状態を"READY"に
                    (void)Wc_Reset();
                }
            }
            break;
        case WC_STATUS_CHILD:
            if (TRUE == Wc_SetIndCallback())
            {
                WcCreateScanParameter();
                if (!Wc_StartScan())
                {
                    wcTarget = WC_STATUS_READY; // 目標状態を"READY"に
                    (void)Wc_Reset();
                }
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_PowerOn failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_PowerOff

  Description:  WM_PowerOffに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_PowerOff(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_PowerOff success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Disable();
            break;
        case WC_STATUS_PARENT:
        case WC_STATUS_CHILD:
            (void)Wc_PowerOn();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_PowerOff failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Indicate

  Description:  Indicate発生時に呼び出されるコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Indicate(void *arg)
{
    WMindCallback *cb = (WMindCallback *)arg;

    WcDebugPrintf("- Indication. STATECODE: 0x%02x\n", cb->state);
    if (cb->errcode == WM_ERRCODE_FIFO_ERROR)
    {
        // 復旧不能なエラー
        wcTarget = WC_STATUS_ERROR;
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_Reset

  Description:  WM_Resetに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_Reset(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_Reset success.\n");
        wcAid = 0;
        if (wcBtCallback != NULL)
            (*wcBtCallback) (arg);
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_PowerOff();
            break;
        case WC_STATUS_PARENT:
            WcCreateParentParameter();
            if (!Wc_SetParentParameter())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            WcCreateScanParameter();
            if (!Wc_StartScan())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_Reset failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcStatus = WC_STATUS_ERROR;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_SetParentParameter

  Description:  WM_SetParentParameterに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_SetParentParameter(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_SetParentParameter success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Reset();
            break;
        case WC_STATUS_PARENT:
            if (!Wc_StartParent())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            wcTarget = WC_STATUS_CHILD;
            // 子機モードに移行する為に一旦リセットする
            (void)Wc_Reset();
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_SetParentParameter failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n",
                      cb->errcode, cb->wlCmdID, cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartParent

  Description:  WM_StartParentに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartParent(void *arg)
{
    WMstartParentCallback *cb = (WMstartParentCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        switch (cb->state)
        {
        case WM_STATECODE_PARENT_START:
            WcDebugPrintf("< WM_StartParent success.\n");
            wcAid = 0;
            switch (wcTarget)
            {
            case WC_STATUS_READY:
                (void)Wc_Reset();
                break;
            case WC_STATUS_PARENT:
                if (!Wc_StartMP())
                {
                    wcTarget = WC_STATUS_READY;
                    (void)Wc_Reset();
                }
                break;
            case WC_STATUS_CHILD:
                wcTarget = WC_STATUS_CHILD;
                // 子機モードに移行する為に一旦リセットする
                (void)Wc_Reset();
                break;
            }
            break;
        case WM_STATECODE_CONNECTED:
            WcDebugPrintf("- Child Connected.\n");
            if (wcBtCallback != NULL)
                (*wcBtCallback) (arg);
            break;
        case WM_STATECODE_BEACON_SENT:
            WcDebugPrintf("- Beacon sent.\n");
            break;
        case WM_STATECODE_DISCONNECTED:
            WcDebugPrintf("- Child disconnected.\n");
            if (wcBtCallback != NULL)
                (*wcBtCallback) (arg);
            break;
        case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
            // 自ら切断した場合は処理を行いません
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_StartParent failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartScan

  Description:  WM_StartScanに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartScan(void *arg)
{
    WMstartScanCallback *cb = (WMstartScanCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_StartScan success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_Reset();
            break;
        case WC_STATUS_PARENT:
            // 親機モードに移行する為に一旦IDLEに戻す
            if (!Wc_EndScan())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            switch (cb->state)
            {
            case WM_STATECODE_PARENT_FOUND:
                // 親機情報格納バッファのキャッシュを破棄
                DC_InvalidateRange(wcWmBssDesc, sizeof(WMbssDesc));
                // ピクトチャットのビーコンであるかを確認
                if (wcPictoCatchStatus)
                {
                    if ((CHT_IsPictochatParent(wcWmBssDesc)) && (wcPictoCatchCallback != NULL))
                    {
                        wcPictoCatchCallback(wcWmBssDesc);
                    }
                }
                else
                {
                    // GameGroupIDが一致するかを確認
                    if (WM_IsValidGameBeacon(wcWmBssDesc) &&
                        (wcWmBssDesc->gameInfo.ggid == wcDefaultParameter->ggid))
                    {
                        // 同じアプリケーションの親機と判定
                        if (!Wc_EndScan())
                        {
                            wcTarget = WC_STATUS_READY;
                            (void)Wc_Reset();
                        }
                        break;
                    }
                }
                // Don't break here.
                // 続けてもう一回スキャンを実行
            case WM_STATECODE_PARENT_NOT_FOUND:
                // もう一回スキャンを実行する
                WcSetNextScanCannel();
                if (!Wc_StartScan())
                {
                    wcTarget = WC_STATUS_READY;
                    (void)Wc_Reset();
                }
                break;
            default:
                WcDebugPrintf("- Invalid state code. STATECODE: 0x%02x\n", cb->state);
                wcStatus = WC_STATUS_BUSY;
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_StartScan failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_EndScan

  Description:  WM_EndScanに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_EndScan(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_EndScan success.\n");
        switch (wcTarget)
        {
        case WC_STATUS_READY:
            (void)Wc_PowerOff();
            break;
        case WC_STATUS_PARENT:
            WcCreateParentParameter();
            if (!Wc_SetParentParameter())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            // 子機としてコネクト開始
            if (!Wc_StartConnect())
            {
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_EndScan failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartConnect

  Description:  WM_StartConnectに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartConnect(void *arg)
{
    WMstartConnectCallback *cb = (WMstartConnectCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        switch (wcTarget)
        {
        case WC_STATUS_READY:
        case WC_STATUS_PARENT:
            WcDebugPrintf("< WM_StartConnect success.\n");
            // 目標状態が"子機"でなくなったので、一旦リセットする
            // コールバックのチェーン中の場合はここではWc_Resetを呼ばない。
            if (wcStatus != WC_STATUS_BUSY)
            {
                (void)Wc_Reset();
            }
            break;
        case WC_STATUS_CHILD:
            switch (cb->state)
            {
            case WM_STATECODE_CONNECT_START:
                // CONNECTEDになるのを待つので、処理なし
                break;
            case WM_STATECODE_CONNECTED:
                // 割り当てられたAIDを退避
                wcAid = cb->aid;
                // 既に子機の場合は何もしない
                if (wcStatus == WC_STATUS_CHILD)
                {
                    WcDebugPrintf("- Connected , but already child mode.\n");
                }
                else
                {
                    // MPを開始
                    WcDebugPrintf("< WM_StartConnect success.\n");
                    if (!Wc_StartMP())
                    {
                        wcTarget = WC_STATUS_READY;
                        (void)Wc_Reset();
                    }
                }
                break;
            case WM_STATECODE_BEACON_LOST:
                WcDebugPrintf("- Beacon from parent lost.\n");
                // ビーコンを失っただけではリセットしない
                break;
            case WM_STATECODE_DISCONNECTED:
                WcDebugPrintf("- DisConnected from parent.\n");
                wcTarget = WC_STATUS_READY;
                // コールバックのチェーン中の場合はここではWc_Resetを呼ばない。
                if (wcStatus != WC_STATUS_BUSY)
                {
                    (void)Wc_Reset();
                }
                break;
            case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
                // 自ら切断した場合は処理を行いません
                break;
            default:
                WcDebugPrintf("< WM_StartConnect failed. Invalid state code. STATECODE: 0x%02x\n",
                              cb->state);
                wcStatus = WC_STATUS_BUSY;
                wcTarget = WC_STATUS_READY;
                (void)Wc_Reset();
            }
            break;
        }
    }
    else
    {
        WcDebugPrintf("< WM_StartConnect failed. ERRCODE: 0x%02x 0x%02x 0x%02x\n", cb->errcode,
                      cb->wlCmdID, cb->wlResult);
        if (cb->errcode == WM_ERRCODE_NO_ENTRY)
        {
            WcDebugPrintf(" ! Parent doesn't accept entry now.\n");
        }
        if (cb->errcode == WM_ERRCODE_OVER_MAX_ENTRY)
        {
            WcDebugPrintf(" ! Over max entry of BSS group.\n");
        }
        wcTarget = WC_STATUS_READY;
        (void)Wc_Reset();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_StartMP

  Description:  WM_StartMPに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_StartMP(void *arg)
{
    WMstartMPCallback *cb = (WMstartMPCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        BOOL    mp_ready = FALSE;
        switch (cb->state)
        {
        case WM_STATECODE_MP_START:
            WcDebugPrintf("< WM_StartMP success.\n");
            switch (wcTarget)
            {
            case WC_STATUS_READY:
                (void)Wc_Reset();
                break;
            case WC_STATUS_PARENT:
            case WC_STATUS_CHILD:
                if (wcDataShareFlag)
                {
                    if (!Wc_StartDataSharing())
                    {
                        wcTarget = WC_STATUS_READY;
                        (void)Wc_Reset();
                    }
                    else
                    {
                        mp_ready = TRUE;
                    }
                }
                else
                {
                    mp_ready = TRUE;
                }
                if (mp_ready)
                {
                    wcSendFlag = TRUE;
                    wcStatus = wcTarget;        // 親(子)機としての接続が正常に完了
                    if (wcBtCallback != NULL)
                        (*wcBtCallback) (arg);
                }
                break;
            }
            break;
        case WM_STATECODE_MPEND_IND:
            // 親機としての送受信完了時のindicate
            break;
        case WM_STATECODE_MP_IND:
            // 子機としての受信完了時のindicate
            break;
        }
    }
    else
    {
        switch (cb->errcode)
        {
        case WM_ERRCODE_SEND_FAILED:
        case WM_ERRCODE_TIMEOUT:
            // 送受信が完了しなかった場合のindicate
            // なにもせずに再送されるのを待つ
            break;
        case WM_ERRCODE_INVALID_POLLBITMAP:
            // 自分宛ではない MP 通信を受け取った場合の indicate
            break;
        default:
            WcDebugPrintf("< WM_StartMP failed. ERRCODE: 0x%02x 0x%02x\n", cb->errcode, cb->state);
            wcStatus = WC_STATUS_BUSY;
            wcTarget = WC_STATUS_READY;
            (void)Wc_Reset();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_SetMPData

  Description:  WM_SetMPDataに対するコールバック関数。

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_SetMPData(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    wcSendFlag = TRUE;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_SetMPData success.\n");
    }
    else
    {
        WMPortSendCallback *cb_PortSend = (WMPortSendCallback *)cb;
        WcDebugPrintf("< WM_SetMPData failed. ERRCODE: 0x%02x 0x%02x 0x%02x restBitmap: 0x%02x\n",
                      cb->errcode, cb->wlCmdID, cb->wlResult, cb_PortSend->restBitmap);
    }
    if (wcBtCallback != NULL)
        (*wcBtCallback) (arg);
}

/*---------------------------------------------------------------------------*
  Name:         WcCb_ReceiveData

  Description:  port に対する受信通知

  Arguments:    arg - コールバック構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WcCb_ReceiveData(void *arg)
{
    WMPortRecvCallback *cb = (WMPortRecvCallback *)arg;

    if (cb->errcode == WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< Receive Data success.\n");
        switch (cb->state)
        {
        case WM_STATECODE_PORT_RECV:
            // データ受信
            switch (wcStatus)
            {
            case WC_STATUS_PARENT:
                // main側に受信を通知
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(cb->aid, cb->data, cb->length);
                }
                break;
            case WC_STATUS_CHILD:
                // main側に受信を通知
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(0, cb->data, cb->length);
                }
                break;
            }
            break;
        case WM_STATECODE_CONNECTED:
            // 接続通知
            break;
        case WM_STATECODE_DISCONNECTED:
            // 切断通知
            switch (wcStatus)
            {
            case WC_STATUS_PARENT:
                // main側に切断を通知
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(cb->aid, NULL, 0);
                }
                break;
            case WC_STATUS_CHILD:
                // main側に切断を通知
                if (wcMpRecvCallback != NULL)
                {
                    wcMpRecvCallback(0, NULL, 0);
                }
                break;
            }
            break;
        case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
            // 自ら切断した場合は処理を行いません
            break;
        }
    }
    else
    {
        WcDebugPrintf("< Receive Data failed. ERRCODE: 0x%02x\n", cb->errcode);
    }
}

/*---------------------------------------------------------------------------*
  Name:         Wc_Enable

  Description:  WM_Enable関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_Enableにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_Enable(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_Enable start.\n");
    wmResult = WM_Enable(WcCb_Enable);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_Enable failed. ERRCODE: 0x%02x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_Disable

  Description:  WM_Disable関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_Disableにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_Disable(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_Disable start.\n");
    wmResult = WM_Disable(WcCb_Disable);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_Disable failed. ERRCODE: 0x%02x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_PowerOn

  Description:  WM_PowerOn関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_PowerOnにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_PowerOn(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_PowerOn start.\n");
    wmResult = WM_PowerOn(WcCb_PowerOn);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_PowerOn failed. ERRCODE: 0x%02x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_PowerOff

  Description:  WM_PowerOff関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_PowerOffにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_PowerOff(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_PowerOff start.\n");
    wmResult = WM_PowerOff(WcCb_PowerOff);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_PowerOff failed. ERRCODE: 0x%02x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_SetIndCallback

  Description:  WM_SetIndCallback関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_SetIndCallback関数が成功した場合にTRUEを返す。
                       失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_SetIndCallback(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_SetIndCallback start.\n");
    wmResult = WM_SetIndCallback(WcCb_Indicate);
    if (wmResult != WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_SetIndCallback failed. ERRCODE: 0x%02x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    WcDebugPrintf("< WM_SetIndCallback success.\n");
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_Reset

  Description:  WM_Reset関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_Resetにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_Reset(void)
{
    WMErrCode wmResult;

    if (wcStartDataShareFlag)
    {
        wmResult = WM_EndDataSharing(wcDataSharingInfo);
        if (wmResult != WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_EndDataSharing failed. ERRCODE: 0x%02x\n", wmResult);
            return FALSE;
        }
        wcStartDataShareFlag = FALSE;
    }

    if ((wcStatus != WC_STATUS_BUSY) && (wcStatus != WC_STATUS_ERROR))
    {
        wcStatus = WC_STATUS_BUSY;
    }
    WcDebugPrintf("> WM_Reset start.\n");
    wmResult = WM_Reset(WcCb_Reset);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_Reset failed. ERRCODE: 0x%02x\n", wmResult);
        wcStatus = WC_STATUS_ERROR;
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_SetParentParameter

  Description:  WM_SetParentParameter関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_SetParentParameterにより非同期処理が正常に開始できた
                       場合にTRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_SetParentParameter(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_SetParentParameter start.\n");
    wmResult = WM_SetParentParameter(WcCb_SetParentParameter, wcParentParameter);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_SetParentParameter failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartParent

  Description:  WM_StartParent関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_StartParentにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartParent(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_StartParent start.\n");
    wmResult = WM_StartParent(WcCb_StartParent);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartParent failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartScan

  Description:  WM_StartScan関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_StartScanにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartScan(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_StartScan start.\n");
    wmResult = WM_StartScan(WcCb_StartScan, wcScanParameter);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartScan failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_EndScan

  Description:  WM_EndScan関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_EndScanにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_EndScan(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_EndScan start.\n");
    wmResult = WM_EndScan(WcCb_EndScan);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_EndScan failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartConnect

  Description:  WM_StartConnect関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_StartConnectにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartConnect(void)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_StartConnect start.\n");
    wmResult = WM_StartConnect(WcCb_StartConnect, wcWmBssDesc, NULL);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartConnect failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartMP

  Description:  WM_StartMP関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_StartMPにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartMP(void)
{
    WMErrCode wmResult;

    if (!wcDataShareFlag)
    {
        WcDebugPrintf("> WM_SetPortCallback start.\n");
        wmResult = WM_SetPortCallback(WC_DEFAULT_PORT, WcCb_ReceiveData, NULL);
        if (wmResult == WM_ERRCODE_SUCCESS)
        {
            WcDebugPrintf("< WM_SetPortCallback success.\n");
        }
        else
        {
            WcDebugPrintf("< WM_SetPortCallback failed. ERRCODE: 0x%02x\n", wmResult);
            return FALSE;
        }
    }

    WcDebugPrintf("> WM_StartMP start.\n");
    wmResult = WM_StartMP(WcCb_StartMP,
                          wcRecvBuffer,
                          (u16)wcRecvBufferSize,
                          wcSendBuffer,
                          (u16)wcSendBufferSize, (u16)(wcParentParameter->CS_Flag ? 0 : 1));
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        WcDebugPrintf("< WM_StartMP failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_StartDataSharing

  Description:  WM_StartDataSharing関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_StartDataSharingが正常に完了した場合にTRUEを返す。
                       失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_StartDataSharing(void)
{
    WMErrCode wmResult;
    u16     aidBitmap;

    aidBitmap = (u16)((1 << (wcDefaultParameter->maxEntry + 1)) - 1);   // 下位 WC_NUM_MAX_CHILD+1 ビットが1の bitmap
    WcDebugPrintf("> WM_StartDataSharing start.\n");
    wmResult = WM_StartDataSharing(wcDataSharingInfo, WC_DS_PORT_NUMBER,        // ポート
                                   aidBitmap,   // 接続する端末のビットマップ
                                   wcDefaultParameter->childMaxSize,    // 端末一台分の共有データのサイズ
                                   (wcFramePeriod > 1 ? FALSE : TRUE));
    if (wmResult != WM_ERRCODE_SUCCESS)
    {
        WcDebugPrintf("< WM_StartDataSharing failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    wcStartDataShareFlag = TRUE;
    WcDebugPrintf("< WM_StartDataSharing success.\n");
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         Wc_SetMPData

  Description:  WM_SetMPData関数を呼び出す。

  Arguments:    None.

  Returns:      BOOL - WM_SetMPDataにより非同期処理が正常に開始できた場合に
                       TRUEを返す。失敗した場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL Wc_SetMPData(const void *buf, u16 size)
{
    WMErrCode wmResult;

    WcDebugPrintf("> WM_SetMPData start. data:0x%08x\n", *(u32 *)buf);
    wmResult = WM_SetMPDataToPort(WcCb_SetMPData,
                                  buf, size, 0xffff, WC_DEFAULT_PORT, WC_DEFAULT_PORT_PRIO);
    if (wmResult != WM_ERRCODE_OPERATING)
    {
        wcSendFlag = TRUE;
        WcDebugPrintf("< WM_SetMPData failed. ERRCODE: 0x%02x\n", wmResult);
        return FALSE;
    }
    return TRUE;
}

#endif
/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
