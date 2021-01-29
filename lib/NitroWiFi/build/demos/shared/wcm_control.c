/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - shared - demos
  File:     wcm_control.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.c,v $
  Revision 1.13  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.12  2005/11/02 04:13:42  terui
  Fix comment.

  Revision 1.11  2005/09/13 13:14:14  terui
  DisconnectAP関数の追加に伴う改造

  Revision 1.10  2005/09/08 10:42:38  adachi_hiroaki
  ラウンドスキャンの回数を取得できるようにした。またコールバック関数内のデバッグ表示を一回だけ表示されるようにした。

  Revision 1.9  2005/09/06 02:53:44  yasu
  nitroWiFi/env.h の NitroSDK への移動に伴う修正

  Revision 1.8  2005/09/05 04:00:49  terui
  スキャン方式、電力管理モードを外部から変更可能なように修正。

  Revision 1.7  2005/08/29 06:39:02  yosizaki
  add SetWcmManualConnect(), ConnectAP().

  Revision 1.6  2005/08/23 01:56:35  seiki_masashi
  rename ENV_GetBinaryAndLength() to ENV_GetBinaryAndSize()

  Revision 1.5  2005/08/22 12:10:19  yasu
  SiteDefs を参照するように修正

  Revision 1.4  2005/07/27 08:36:22  terui
  Add SetWcmThreadPriority API

  Revision 1.3  2005/07/22 11:18:56  terui
  スタックの底を指定する際の記述の仕方を統一

  Revision 1.2  2005/07/22 10:36:17  terui
  スレッドスタックの底の位置指定が間違っていたバグを修正。

  Revision 1.1  2005/07/21 08:38:16  terui
  サンプルで共有可能なソースファイルとして新規追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro/os.h>
#include <nitroWiFi.h>
#include "wcm_control.h"
#include "ap_info.h"

/*---------------------------------------------------------------------------*
    定数 定義
 *---------------------------------------------------------------------------*/
#define LOCAL_WCM_THREAD_STACK_SIZE     1024    // ( 変更可 ) 大きめにスタックを確保
#define LOCAL_WCM_THREAD_PRIORITY       24      // ( 変更可 ) 低めの優先度で十分
#define LOCAL_WCM_DMA_NO                3       // ( 変更可 ) 無線機能に使用する DMA 番号
#define LOCAL_WCM_APLIST_COUNT_MAX      22      // WCM ライブラリ内で管理させる AP 情報リストの数
#define LOCAL_WCM_APLIST_BUF_SIZE       ((LOCAL_WCM_APLIST_COUNT_MAX * WCM_APLIST_BLOCK_SIZE) + 12)
#define LOCAL_WCM_THREAD_MSGQ_CAPACITY  3       // メッセージキューの段数
#define LOCAL_WCM_MSG_TERMINATE         ((void*)0)
#define LOCAL_WCM_MSG_FOUND_AP          ((void*)1)
#define LOCAL_WCM_MSG_DISCONNECTED      ((void*)2)
#define LOCAL_WCM_MSG_ERROR             ((void*)3)
#define LOCAL_WCM_MSG_CONNECT_AP        ((void*)4)
#define LOCAL_WCM_MSG_DISCONNECT        ((void*)5)

// WCM ライブラリ操作ログ表示用 文字列定数
static const char*  gWcmResultText[8] =
{
    { "SUCCESS" } ,         { "FAILURE" } ,
    { "PROGRESS" } ,        { "ACCEPT" } ,
    { "REJECT" } ,          { "WMDISABLE" } ,
    { "NOT_ENOUGH_MEM" } ,  { "FATAL_ERROR" }
};
static const char*  gWcmNotifyText[10] =
{
    { "COMMON" } ,          { "STARTUP" } ,
    { "CLEANUP" } ,         { "BEGIN_SEARCH" } ,
    { "END_SEARCH" } ,      { "CONNECT" } ,
    { "DISCONNECT" } ,      { "FOUND_AP" } ,
    { "SEARCH_AROUND" } ,   { "TERMINATE" }
};

// デバッグ文字列表示設定
#define LOCAL_DEBUG_PRINT   1
#if LOCAL_DEBUG_PRINT
#define DebugPrintf OS_TPrintf
#else
#define DebugPrintf(...)    ((void)0)
#endif

/*---------------------------------------------------------------------------*
    内部変数 定義
 *---------------------------------------------------------------------------*/

// スレッド制御用
static OSThread         gWcmThread;
static u32          gWcmStack[LOCAL_WCM_THREAD_STACK_SIZE / sizeof( u32 ) ];
static OSMessageQueue   gWcmMessageQ;
static OSMessage        gWcmMsgArray[LOCAL_WCM_THREAD_MSGQ_CAPACITY];
static OSThreadQueue    gWcmThreadQ;

// WCM ライブラリ制御用
static volatile BOOL    gWcmTerminateFlag;
static vs32         gWcmResult;
static void*        gWcmBuf = NULL;
static WCMConfig    gWcmConfig;
static WMBssDesc gWcmBssDesc    ATTRIBUTE_ALIGN(4);
static u32  gWcmScanType = WCM_OPTION_SCANTYPE_ACTIVE;
static u32  gWcmPowerMode = WCM_OPTION_POWER_ACTIVE;

static BOOL gWcmManualConnect = FALSE;
static u32  gWcmSearchAroundCount = 0;

static const WcmControlApInfo*  constApInfoPtr = NULL;

/*---------------------------------------------------------------------------*
    内部関数 プロトタイプ
 *---------------------------------------------------------------------------*/
static void ControlThread(void* arg);
static void FreezeThread(void);
static s32  CallFunction(s16 notifyId);
static void ControlCallback(WCMNotice* arg);

/*---------------------------------------------------------------------------*
  Name:         SetWcmScanType

  Description:  AP を探索する際のスキャン方式を設定する。
                デフォルトではアクティブスキャン方式となっている。
                InitWcmControl() より先に呼び出す必要がある。

  Arguments:    type    -   WCM_OPTION_SCANTYPE_* の定数を指定する。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetWcmScanType(u32 type)
{
    switch (type)
    {
    case WCM_OPTION_SCANTYPE_PASSIVE:
        gWcmScanType = WCM_OPTION_SCANTYPE_PASSIVE;
        break;

    default:
        gWcmScanType = WCM_OPTION_SCANTYPE_ACTIVE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetWcmPowerMode

  Description:  AP と通信する際の電力管理方式を指定する。
                デフォルトでは常時アクティブ方式となっている。
                InitWcmControl() より先に呼び出す必要がある。

  Arguments:    mode    -   WCM_OPTION_POWER_* の定数を指定する。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetWcmPowerMode(u32 mode)
{
    switch (mode)
    {
    case WCM_OPTION_POWER_SAVE:
        gWcmPowerMode = WCM_OPTION_POWER_SAVE;
        break;

    default:
        gWcmPowerMode = WCM_OPTION_POWER_ACTIVE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetWcmManualConnectMode

  Description:  APのリストから手動で選択して接続するよう設定.
                デフォルトでは FALSE (自動接続)となっている.
                InitWcmControl() より先に呼び出す必要がある.

  Arguments:    enable -   手動で接続する場合は TRUE。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void SetWcmManualConnect(BOOL enable)
{
    gWcmManualConnect = enable;
}

/*---------------------------------------------------------------------------*
  Name:         ConnectAP

  Description:  指定されたBssDescのAPへ接続する

  Arguments:    bd     -   接続するAPを示す構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ConnectAP(const WMBssDesc* pbd)
{
    SDK_NULL_ASSERT(pbd);
    MI_CpuCopy8(pbd, &gWcmBssDesc, /*sizeof(WMBssDesc)*/ WCM_APLIST_SIZE);
    (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_CONNECT_AP, OS_MESSAGE_NOBLOCK);
}

/*---------------------------------------------------------------------------*
  Name:         DisconnectAP

  Description:  AP との接続の切断を試みる

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void DisconnectAP(void)
{
    OSIntrMode  e = OS_DisableInterrupts();
    if (WCM_GetPhase() == WCM_PHASE_DCF)
    {
        (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_DISCONNECT, OS_MESSAGE_NOBLOCK);
    }
    (void)OS_RestoreInterrupts(e);
}

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl/InitWcmControlByApInfo

  Description:  内部変数を初期化し、WCM コントロールスレッドを起動する。
                既にスレッドが起動している場合は何も行わない。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitWcmControl(void)
{
    InitWcmControlByApInfo(&constApInfo);
}

void InitWcmControlByApInfo(const WcmControlApInfo* ptr)
{
    // スレッド変数が一回でも初期化されたか？
    if (gWcmThread.id != 0)
    {
        // スレッドは終了状態にあるか？
        if (OS_IsThreadTerminated(&gWcmThread) == FALSE)
        {
            return;
        }
    }

    constApInfoPtr = ptr;

    // メッセージキューを初期化
    OS_InitMessageQueue(&gWcmMessageQ, gWcmMsgArray, LOCAL_WCM_THREAD_MSGQ_CAPACITY);

    // スレッドキューを初期化
    OS_InitThreadQueue(&gWcmThreadQ);

    // 変数初期化
    MI_CpuClearFast(&gWcmBssDesc, sizeof(WMBssDesc));

    // スレッド変数を使って新規にスレッドを作成
    OS_CreateThread(&gWcmThread, ControlThread, NULL, (void*)(gWcmStack + (LOCAL_WCM_THREAD_STACK_SIZE / sizeof(u32))),
                    LOCAL_WCM_THREAD_STACK_SIZE, LOCAL_WCM_THREAD_PRIORITY);

    // 作成したスレッドを起動
    gWcmTerminateFlag = FALSE;
    OS_WakeupThreadDirect(&gWcmThread);
}

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
void TerminateWcmControl(void)
{
    // スレッド変数が一回でも初期化されたか？
    if (gWcmThread.id != 0)
    {
        // スレッドは終了状態にあるか？
        if (OS_IsThreadTerminated(&gWcmThread) == FALSE)
        {
            // 既に終了要求を受け付けている状態か？
            if (gWcmTerminateFlag == FALSE)
            {
                gWcmTerminateFlag = TRUE;
                (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_TERMINATE, OS_MESSAGE_NOBLOCK);
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetWcmThreadPriority

  Description:  WCM ライブラリの状態管理を行うスレッドの優先度を変更する。

  Arguments:    priority    -   0 ～ 31 のスレッド優先度を指定する。

  Returns:      u8  -   変更する前に設定されていた優先度を返す。
                        なんらかの理由で優先度の変更に失敗した場合は 0xff を返す。
 *---------------------------------------------------------------------------*/
u8 SetWcmThreadPriority(u8 priority)
{
    u8  beforePrio = 0xff;

    if (priority < 32)
    {
        if ((gWcmThread.id != 0) && (OS_IsThreadTerminated(&gWcmThread) == FALSE))
        {
            beforePrio = (u8) OS_GetThreadPriority(&gWcmThread);
            if (FALSE == OS_SetThreadPriority(&gWcmThread, (u32) priority))
            {
                beforePrio = 0xff;
            }
        }
    }

    return beforePrio;
}

/*---------------------------------------------------------------------------*
  Name:         GetWcmSearchAroundCount

  Description:  全チャンネルを何週スキャンしたかを返す。
                StartUp直後は0になる。

  Arguments:    None.

  Returns:      何回スキャンしたか。
 *---------------------------------------------------------------------------*/
u32 GetWcmSearchAroundCount(void)
{
    return gWcmSearchAroundCount;
}

/*---------------------------------------------------------------------------*
  Name:         ControlThread

  Description:  WCM ライブラリの状態管理を行うスレッド。
                gWcmTerminateFlag 変数が FALSE の間は指定 AP に接続しようとし、
                また接続完了後はその接続を維持しようとする。
                TerminateWcmControl 関数が呼ばれると gWcmTerminateFlag 変数が
                TRUE に変化し、その後は WCM ライブラリを終了して自身のスレッド
                も終了しようとする。
                なお、WCM ライブラリ内部で FATAL ERROR が発生した場合には、
                当スレッドは内部で無限ループに入る。

  Arguments:    arg     -   使用しない。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ControlThread(void* arg)
{
#pragma unused(arg)

    void*       wcmBuf;
    void*       wcmAplistBuf;
    s32         wcmResult;
    OSMessage   tempMessage;

    // WCM ライブラリが使用するメモリを確保
    wcmBuf = OS_Alloc(WCM_WORK_SIZE + LOCAL_WCM_APLIST_BUF_SIZE);

    if (wcmBuf == NULL)
    {
        /* メモリが確保できなかった場合はその後の操作は不可能なので終了 */
        return;
    }

    wcmAplistBuf = (void*)((u32) wcmBuf + WCM_WORK_SIZE);

    // WCM ライブラリを初期化
    wcmResult = WCM_Init(wcmBuf, WCM_WORK_SIZE);
    DebugPrintf("WCM_Init() -> %s\n", gWcmResultText[wcmResult]);
    if (wcmResult != WCM_RESULT_SUCCESS)
    {
        /*
         * ここでエラーが返される場合は、ライブラリの使い方が間違っている可能性が高い。
         * 他のスレッドから WCM ライブラリが既に初期化されていることが考えられる。
         */
        OS_Free(wcmBuf);
        return;
    }

    // WCM ライブラリの動作設定
    gWcmConfig.dmano = LOCAL_WCM_DMA_NO;
    gWcmConfig.pbdbuffer = wcmAplistBuf;
    gWcmConfig.nbdbuffer = LOCAL_WCM_APLIST_BUF_SIZE;
    gWcmConfig.nbdmode = WCM_APLIST_MODE_IGNORE;

    // 接続開始
    while (gWcmTerminateFlag == FALSE)
    {
        switch (WCM_GetPhase())
        {
        case WCM_PHASE_WAIT:
            // 無線機能の起動
            wcmResult = CallFunction(WCM_NOTIFY_STARTUP);
            break;

        case WCM_PHASE_IDLE:
            if (gWcmBssDesc.length == 0)
            {
                // AP の自動探索を開始
                WCM_ClearApList();
                wcmResult = CallFunction(WCM_NOTIFY_BEGIN_SEARCH);
            }
            else
            {
                // AP に接続
                wcmResult = CallFunction(WCM_NOTIFY_CONNECT);
                if (wcmResult == WCM_RESULT_SUCCESS)
                {
                    // 次に接続しようとする際にサーチからやり直す為に、ここでクリア
                    MI_CpuClearFast(&gWcmBssDesc, sizeof(WMBssDesc));
                }
            }
            break;

        case WCM_PHASE_SEARCH:
            // AP が発見されるまで待つ
            (void)OS_ReceiveMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_BLOCK);

            /*
             * メッセージとしては、WCM の停止要求、AP 発見通知、及びエラー通知の３種類を
             * 受け取る可能性がある。AP 発見通知の場合は、このサンプルでは発見した AP がすなわち
             * 接続すべき AP なので、情報を退避する。
             */
            if (tempMessage == LOCAL_WCM_MSG_FOUND_AP)
            {
                /* 自動接続モードならここで検索終了 */
                if (!gWcmManualConnect)
                {
                    (void)WCM_LockApList(WCM_APLIST_LOCK);
                    if (WCM_CountApList() >= 1)
                    {
                        // 最初に見つかった AP の情報のみを退避する。
                        MI_CpuCopyFast(WCM_PointApList(0), &gWcmBssDesc, WCM_APLIST_SIZE);
                        DebugPrintf("  BSS-ID: %02x-%02x-%02x-%02x-%02x-%02x\n", gWcmBssDesc.bssid[0],
                                    gWcmBssDesc.bssid[1], gWcmBssDesc.bssid[2], gWcmBssDesc.bssid[3],
                                    gWcmBssDesc.bssid[4], gWcmBssDesc.bssid[5]);
                        DebugPrintf("  ESS-ID: %s\n", gWcmBssDesc.ssid);
                    }
                    (void)WCM_LockApList(WCM_APLIST_UNLOCK);
                    wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
                }
            }

            /* 接続要求があればまず検索を終了 */
            else if (tempMessage == LOCAL_WCM_MSG_CONNECT_AP)
            {
                wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
            }
            break;

        case WCM_PHASE_DCF:
            // 切断通知まで待つ
            (void)OS_ReceiveMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_BLOCK);

            /*
             * メッセージとしては、WCM の停止要求、AP からの切断、及びエラー通知の３種類を
             * 受け取る可能性がある。このどれの場合も受け取った時点で行うべき処理は無い。
             */
            if (tempMessage == LOCAL_WCM_MSG_DISCONNECT)
            {
                wcmResult = CallFunction(WCM_NOTIFY_DISCONNECT);
            }
            break;

        case WCM_PHASE_IRREGULAR:
            // FIFO が溢れた可能性が高いので、少し待つ
            OS_Sleep(1);

            // IRREGULAR フェーズ時に有効な処理は強制停止のみ
            wcmResult = CallFunction(WCM_NOTIFY_TERMINATE);
            break;

        case WCM_PHASE_FATAL_ERROR:
        default:
            FreezeThread();
        }
    }

    // 終了処理
    while (gWcmTerminateFlag != FALSE)
    {
        switch (WCM_GetPhase())
        {
        case WCM_PHASE_WAIT:
            // ループから抜ける
            gWcmTerminateFlag = FALSE;
            break;

        case WCM_PHASE_IDLE:
            // 無線機能の停止
            wcmResult = CallFunction(WCM_NOTIFY_CLEANUP);
            break;

        case WCM_PHASE_SEARCH:
            // AP の自動探索を停止
            wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
            break;

        case WCM_PHASE_DCF:
            // AP から切断
            wcmResult = CallFunction(WCM_NOTIFY_DISCONNECT);
            break;

        case WCM_PHASE_IRREGULAR:
            // 無線機能の強制終了
            wcmResult = CallFunction(WCM_NOTIFY_TERMINATE);
            break;

        case WCM_PHASE_FATAL_ERROR:
        default:
            FreezeThread();
        }
    }

    // WCM ライブラリを終了
    wcmResult = WCM_Finish();
    DebugPrintf("WCM_Finish() -> %s\n", gWcmResultText[wcmResult]);
    if (wcmResult != WCM_RESULT_SUCCESS)
    {
        /*
         * ここでエラーが返される場合は、ライブラリの使い方が間違っている可能性が高い。
         * 他のスレッドから WCM ライブラリが終了されてしまっていることが考えられる。
         */
        OS_Free(wcmBuf);
        return;
    }

    // 確保していたメモリを解放
    OS_Free(wcmBuf);
    return;
}

/*---------------------------------------------------------------------------*
  Name:         FreezeThread

  Description:  スレッドから呼び出され、無限ループに入って固まる。

  Arguments:    None.

  Returns:      当関数からは返らない。
 *---------------------------------------------------------------------------*/
static void FreezeThread(void)
{
    /*
     * FATAL ERROR の場合はどうしようもないので、中で固まることにする。
     * 致命的なメモリが破壊された、ARM7の負荷が高すぎて処理しきれなく
     * なった、スレッドのスタックが溢れた、等の原因が考えられる。
     * エンドユーザーに適切なエラー通知をしなければならないが、ソフト的に
     * 復旧する方法は無いので、エンドユーザーに電源を切ってもらうしかない。
     */
    while (TRUE)
    {
        OS_Sleep(10000);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CallFunction

  Description:  WCM 非同期 API をコールし、非同期処理が完了するまで待つ。
                スレッドから呼び出されることを想定しており、非同期処理の完了を
                内部でブロックして待つ点に注意。

  Arguments:    WCM 非同期 API の種別を WCM_NOTIFY_* の形式で指定する。
                指定可能なパラメータは以下の通り。
                    WCM_NOTIFY_STARTUP      : WCM_StartupAsync を実行する。
                    WCM_NOTIFY_CLEANUP      : WCM_CleanupAsync を実行する。
                    WCM_NOTIFY_BEGIN_SEARCH : WCM_SearchAsycn(開始) を実行する。
                    WCM_NOTIFY_END_SEARCH   : WCM_SearchAsync(停止) を実行する。
                    WCM_NOTIFY_CONNECT      : WCM_ConnectAsync を実行する。
                    WCM_NOTIFY_DISCONNECT   : WCM_DisconnectAsync を実行する。
                    WCM_NOTIFY_TERMINATE    : WCM_TerminateAsync を実行する。

  Returns:      同期的・非同期的な処理結果を返す。
                返される値は以下の内のいずれかとなる。
                    WCM_RESULT_SUCCESS      : 処理が正常に完了。
                    WCM_RESULT_FAILURE      : 処理の途中でなんらかの要因で失敗。
                    WCM_RESULT_FATAL_ERROR  : パラメータが想定範囲外
 *---------------------------------------------------------------------------*/
static s32 CallFunction(s16 notifyId)
{
    s32         wcmResult = WCM_RESULT_FATAL_ERROR;
    OSIntrMode  e = OS_DisableInterrupts();

    while (TRUE)
    {
        // 実行すべき API を振り分け
        switch (notifyId)
        {
        case WCM_NOTIFY_STARTUP:
            wcmResult = WCM_StartupAsync(&gWcmConfig, ControlCallback);
            DebugPrintf("WCM_StartupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CLEANUP:
            wcmResult = WCM_CleanupAsync();
            DebugPrintf("WCM_CleanupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_BEGIN_SEARCH:
            /****************************************************************/

            /*  アクセスポイントを探索する際の重要な設定を行っている箇所    */
            wcmResult = WCM_BeginSearchAsync((void*)constApInfoPtr->bssId, (void*)constApInfoPtr->essId, (u32) (
                                                 WCM_OPTION_CHANNEL_ALL | gWcmScanType |
                                                 (gWcmManualConnect ? WCM_OPTION_ROUNDSCAN_NOTIFY : WCM_OPTION_ROUNDSCAN_IGNORE)
                                             ));

            /****************************************************************/
            DebugPrintf("WCM_BeginSearchAsync -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_END_SEARCH:
            wcmResult = WCM_EndSearchAsync();
            DebugPrintf("WCM_EndSearchAsync -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CONNECT:
            /****************************************************************/

            /*  アクセスポイントに接続する際の重要な設定を行っている箇所    */
            wcmResult = WCM_ConnectAsync(&gWcmBssDesc, (void*)(&(constApInfoPtr->wepDesc)),
                                         gWcmPowerMode | constApInfoPtr->auth_option);

            /****************************************************************/
            DebugPrintf("WCM_ConnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_DISCONNECT:
            wcmResult = WCM_DisconnectAsync();
            DebugPrintf("WCM_DisconnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_TERMINATE:
            wcmResult = WCM_TerminateAsync();
            DebugPrintf("WCM_TerminateAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        default:
            (void)OS_RestoreInterrupts(e);
            return wcmResult;
        }

        // 同期的な処理結果を確認
        switch (wcmResult)
        {
        case WCM_RESULT_REJECT:
            // 少し待ってからリトライする
            OS_Sleep(1);
            continue;

        case WCM_RESULT_PROGRESS:
        case WCM_RESULT_ACCEPT:
            // 非同期処理応答がコールバックされるまで待つ
            OS_SleepThread(&gWcmThreadQ);
            break;

        case WCM_RESULT_SUCCESS:
        case WCM_RESULT_FAILURE:
            // 同期的に処理が完了し、非同期処理は行われない場合
            (void)OS_RestoreInterrupts(e);
            return wcmResult;

        case WCM_RESULT_WMDISABLE:
            /*
             * ARM7 側で WM ライブラリが動作していない場合。
             * WCM を使い始める前に WVR ライブラリを使って ARM7 側無線ドライバを
             * 起動させる必要があるが、それはアプリケーションの責任で行うべきこと。
             */
            OS_Panic("ARM7 WM library is not ready.\n");

        case WCM_RESULT_NOT_ENOUGH_MEM: // 非同期関数にこの返り値が返されることは無い
        case WCM_RESULT_FATAL_ERROR:
        default:
            (void)OS_RestoreInterrupts(e);
            FreezeThread();
        }

        // 非同期的な処理結果を確認
        switch (gWcmResult)
        {
        case WCM_RESULT_SUCCESS:
        case WCM_RESULT_FAILURE:
            wcmResult = gWcmResult;
            break;

        case WCM_RESULT_PROGRESS:       // 非同期処理結果にこの処理結果が渡されることは無い
        case WCM_RESULT_ACCEPT:         // 非同期処理結果にこの処理結果が渡されることは無い
        case WCM_RESULT_REJECT:         // 非同期処理結果にこの処理結果が渡されることは無い
        case WCM_RESULT_WMDISABLE:      // 非同期処理結果にこの処理結果が渡されることは無い
        case WCM_RESULT_NOT_ENOUGH_MEM: // 非同期処理結果にこの処理結果が渡されることは無い
        case WCM_RESULT_FATAL_ERROR:
        default:
            (void)OS_RestoreInterrupts(e);
            FreezeThread();
        }

        break;
    }

    (void)OS_RestoreInterrupts(e);
    return wcmResult;
}

/*---------------------------------------------------------------------------*
  Name:         ControlCallback

  Description:  WCM 非同期 API の処理結果応答、及び WCM ライブラリからの 不定期
                通知を受け取るコールバック関数。

  Arguments:    arg     -   通知種別や処理結果などを示す構造体へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ControlCallback(WCMNotice* arg)
{
    static s16  lastNotify = -1;
    static s16  lastResult = -1;

    //前回と結果が違ったら表示
    if (lastNotify != arg->notify || lastResult != arg->result)
    {
        DebugPrintf(" cb %s %s\n", gWcmNotifyText[arg->notify], gWcmResultText[arg->result]);
        lastNotify = arg->notify;
        lastResult = arg->result;
    }

    switch (arg->notify)
    {
    case WCM_NOTIFY_STARTUP:
        gWcmSearchAroundCount = 0;

    case WCM_NOTIFY_CLEANUP:
    case WCM_NOTIFY_BEGIN_SEARCH:
    case WCM_NOTIFY_END_SEARCH:
    case WCM_NOTIFY_CONNECT:
    case WCM_NOTIFY_TERMINATE:
        // 要求と 1 対 1 に対応する非同期処理の結果応答
        gWcmResult = arg->result;
        OS_WakeupThread(&gWcmThreadQ);
        break;

    case WCM_NOTIFY_DISCONNECT:
#ifdef SDK_THREAD_INFINITY
        if (gWcmThreadQ.head != NULL)
#else
            if (gWcmThreadQ != 0)
#endif
            {
                // 要求と 1 対 1 に対応する非同期処理の結果応答
                gWcmResult = arg->result;
                OS_WakeupThread(&gWcmThreadQ);
            }
            else
            {
                // スレッドが稼動中、もしくはメッセージ待ちで停止状態の場合
                if (arg->result == WCM_RESULT_SUCCESS)
                {
                    // DCF 通信中に AP から切断されたことを示す通知
                    (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_DISCONNECTED, OS_MESSAGE_NOBLOCK);
                }
                else
                {
                    // DCF 通信中になんらかのエラーが発生したことを示す通知
                    (void)OS_JamMessage(&gWcmMessageQ, LOCAL_WCM_MSG_ERROR, OS_MESSAGE_NOBLOCK);
                }
            }
        break;

    case WCM_NOTIFY_FOUND_AP:
        // 自動探索時の非同期通知
        {
            if (arg->result == WCM_RESULT_SUCCESS)
            {
                OSMessage   tempMessage;

                // 自動探索時に AP を発見したことを示す通知
                if (OS_ReadMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_NOBLOCK) == FALSE)
                {
                    // 何度も通知されるものなので、メッセージキューが空である場合のみに限定
                    (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_FOUND_AP, OS_MESSAGE_NOBLOCK);
                }
            }
            else
            {
                // 自動探索時になんらかのエラーが発生したことを示す通知
                (void)OS_JamMessage(&gWcmMessageQ, LOCAL_WCM_MSG_ERROR, OS_MESSAGE_NOBLOCK);
            }
        }
        break;

    case WCM_NOTIFY_SEARCH_AROUND:
        // 自動探索時に指定チャンネルを一巡したことを示す不定期通知
        ++gWcmSearchAroundCount;
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         InitWcmApInfo

  Description:  アクセスポイント接続パラメータを環境設定値から取得して
                初期化する

  Arguments:    apinfo   接続パラメータ
                apclass  アクセスポイントのクラス名

  Returns:      TRUE   成功
                FALSE  失敗
 *---------------------------------------------------------------------------*/
static BOOL myEnvCopy(const char* name, void* dest, int dest_size);
static BOOL myEnvCopy(const char* name, void* dest, int dest_size)
{
    void*   src;
    int     src_size;

    if (ENV_GetBinaryAndSize(name, &src, &src_size))
    {
        MI_CpuCopy8(src, dest, (u32) MATH_MIN(src_size, dest_size));
        return TRUE;
    }

    return FALSE;
}

BOOL InitWcmApInfo(WcmControlApInfo* apinfo, const char* apclass)
{
    BOOL    isvalid;

    ENV_SetClass(apclass);

    if (ENV_GetBOOL(".ISVALID", &isvalid) && isvalid)
    {
        MI_CpuClear8(apinfo, sizeof(WcmControlApInfo));

        if (myEnvCopy(".ESSID", apinfo->essId, sizeof(apinfo->essId)) &&
            myEnvCopy(".BSSID", apinfo->bssId, sizeof(apinfo->bssId)) && ENV_GetU32(".AUTHMODE", &apinfo->auth_option) &&
            ENV_GetU8(".WEP.MODE", &apinfo->wepDesc.mode) && ENV_GetU8(".WEP.KEYID", &apinfo->wepDesc.keyId) &&
            myEnvCopy(".WEP.KEY", apinfo->wepDesc.key, sizeof(apinfo->wepDesc.key)))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
