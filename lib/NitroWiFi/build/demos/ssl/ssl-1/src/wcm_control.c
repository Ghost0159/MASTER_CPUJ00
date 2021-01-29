/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     wcm_control.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_control.c,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 に移動

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 デモの追加

  Revision 1.1  2005/07/14 10:15:49  jey-s
  *** empty log message ***

  Revision 1.2  2005/07/12 04:17:50  terui
  固定の WEP 設定の表記を改良
  認証方式を設定に従って切り替えるよう改良

  Revision 1.1  2005/07/11 12:07:09  terui
  新規追加。

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitro/os.h>
#include <nitroWiFi.h>
#include "wcm_control.h"
#include "screen.h"

/*---------------------------------------------------------------------------*
    AP 情報

    NOTICE: ここの定義を、接続する AP に合わせて編集して下さい。
            なお、当サンプルでは BSSID、ESSID、802.11 認証方式、WEP暗号化設定が
            判明している AP へ自動的に接続します。
 *---------------------------------------------------------------------------*/
static const u8*        gWcmBssid = WCM_BSSID_ANY;
static const char       gWcmEssid[] = { "000D0B14ABD2" };
static const u32        gWcmAuthOption = WCM_OPTION_AUTH_OPENSYSTEM;

#define LOCAL_WCM_WEP_MODE  WCM_WEPMODE_NONE

static const WCMWepDesc gWcmWepDesc =
{
    LOCAL_WCM_WEP_MODE ,                        // 暗号化モード
    0,                                          // 送信時に使用する WEP キー ID ( 0 ～ 3 )
#if ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_40 )
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
#elif ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_104 )
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
#elif ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_128 )
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
    }
#else /* ( LOCAL_WCM_WEP_MODE == WCM_WEPMODE_NONE ) */
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    }
#endif
};

/*---------------------------------------------------------------------------*
    定数 定義
 *---------------------------------------------------------------------------*/
#define LOCAL_WCM_THREAD_STACK_SIZE     1024    // ( 変更可 ) 大きめにスタックを確保
#define LOCAL_WCM_THREAD_PRIORITY       24      // ( 変更可 ) 低めの優先度で十分
#define LOCAL_WCM_DMA_NO                3       // ( 変更可 ) 無線機能に使用する DMA 番号
#define LOCAL_WCM_APLIST_COUNT_MAX      1       // WCM ライブラリ内で管理させる AP 情報リストの数
#define LOCAL_WCM_APLIST_BUF_SIZE       ((LOCAL_WCM_APLIST_COUNT_MAX * WCM_APLIST_BLOCK_SIZE) + 12)
#define LOCAL_WCM_THREAD_MSGQ_CAPACITY  2       // メッセージキューの段数
#define LOCAL_WCM_MSG_TERMINATE         ((void*)0)
#define LOCAL_WCM_MSG_FOUND_AP          ((void*)1)
#define LOCAL_WCM_MSG_DISCONNECTED      ((void*)2)

// WCM ライブラリ操作ログ表示用 文字列定数
static const char*      gWcmResultText[8] =
{
    { "SUCCESS" } ,         { "FAILURE" } ,
    { "PROGRESS" } ,        { "ACCEPT" } ,
    { "REJECT" } ,          { "WMDISABLE" } ,
    { "NOT_ENOUGH_MEM" } ,  { "FATAL_ERROR" }
};
static const char*      gWcmNotifyText[10] =
{
    { "COMMON" } ,          { "STARTUP" } ,
    { "CLEANUP" } ,         { "BEGIN_SEARCH" } ,
    { "END_SEARCH" } ,      { "CONNECT" } ,
    { "DISCONNECT" } ,      { "FOUND_AP" } ,
    { "SEARCH_AROUND" } ,   { "TERMINATE" }
};

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

/*---------------------------------------------------------------------------*
    内部関数 プロトタイプ
 *---------------------------------------------------------------------------*/
static void ControlThread(void* arg);
static void FreezeThread(void);
static s32  CallFunction(s16 notifyId);
static void ControlCallback(WCMNotice* arg);

/*---------------------------------------------------------------------------*
  Name:         InitWcmControl

  Description:  内部変数を初期化し、WCM コントロールスレッドを起動する。
                既にスレッドが起動している場合は何も行わない。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitWcmControl(void)
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

    // メッセージキューを初期化
    OS_InitMessageQueue(&gWcmMessageQ, gWcmMsgArray, LOCAL_WCM_THREAD_MSGQ_CAPACITY);

    // スレッドキューを初期化
    OS_InitThreadQueue(&gWcmThreadQ);

    // 変数初期化
    MI_CpuClearFast(&gWcmBssDesc, sizeof(WMBssDesc));

    // スレッド変数を使って新規にスレッドを作成
    OS_CreateThread(&gWcmThread, ControlThread, NULL, (void*)(gWcmStack + LOCAL_WCM_THREAD_STACK_SIZE),
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
    if (gWcmThread.id != 0)
    {
        if (OS_IsThreadTerminated(&gWcmThread) == FALSE)
        {
            if (gWcmTerminateFlag == FALSE)
            {
                gWcmTerminateFlag = TRUE;
                (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_TERMINATE, OS_MESSAGE_NOBLOCK);
            }
        }
    }
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
    PrintSubScreen("WCM_Init() -> %s\n", gWcmResultText[wcmResult]);
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
             * メッセージとしては、WCM の停止要求、及び AP 発見通知の２種類のみ受け取る
             * 可能性がある。AP 発見通知の場合は、このサンプルでは発見した AP がすなわち
             * 接続すべき AP なので、情報を退避する。
             */
            if (tempMessage == LOCAL_WCM_MSG_FOUND_AP)
            {
                (void)WCM_LockApList(WCM_APLIST_LOCK);
                if (WCM_CountApList() == 1)
                {
                    // BSSID 固定なので 発見される可能性のある AP は１つだけのはず
                    MI_CpuCopyFast(WCM_PointApList(0), &gWcmBssDesc, WCM_APLIST_SIZE);
                }
                (void)WCM_LockApList(WCM_APLIST_UNLOCK);
                wcmResult = CallFunction(WCM_NOTIFY_END_SEARCH);
            }
            break;

        case WCM_PHASE_DCF:
            // 切断通知まで待つ
            (void)OS_ReceiveMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_BLOCK);

            /*
             * メッセージとしては、WCM の停止要求、及び AP からの切断の２種類のみ受け取る
             * 可能性がある。このどちらの場合も受け取った時点で行うべき処理は無い。
             */
            break;

        case WCM_PHASE_IRREGULAR:
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
    PrintSubScreen("WCM_Finish() -> %s\n", gWcmResultText[wcmResult]);
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
    s32 wcmResult = WCM_RESULT_FATAL_ERROR;

    while (TRUE)
    {
        // 実行すべき API を振り分け
        switch (notifyId)
        {
        case WCM_NOTIFY_STARTUP:
            wcmResult = WCM_StartupAsync(&gWcmConfig, ControlCallback);
            PrintSubScreen("WCM_StartupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CLEANUP:
            wcmResult = WCM_CleanupAsync();
            PrintSubScreen("WCM_CleanupAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_BEGIN_SEARCH:
            /****************************************************************/

            /*  アクセスポイントを探索する際の重要な設定を行っている箇所    */
            wcmResult = WCM_SearchAsync((void*)gWcmBssid, (void*)gWcmEssid,
                                        WCM_OPTION_CHANNEL_ALL | WCM_OPTION_SCANTYPE_ACTIVE | WCM_OPTION_ROUNDSCAN_IGNORE);

            /****************************************************************/
            PrintSubScreen("WCM_SearchAsync(S) -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_END_SEARCH:
            wcmResult = WCM_SearchAsync(NULL, NULL, 0);
            PrintSubScreen("WCM_SearchAsync(E) -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_CONNECT:
            /****************************************************************/

            /*  アクセスポイントに接続する際の重要な設定を行っている箇所    */
            wcmResult = WCM_ConnectAsync(&gWcmBssDesc, (void*)(&gWcmWepDesc), WCM_OPTION_POWER_ACTIVE | gWcmAuthOption);

            /****************************************************************/
            PrintSubScreen("WCM_ConnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_DISCONNECT:
            wcmResult = WCM_DisconnectAsync();
            PrintSubScreen("WCM_DisconnectAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        case WCM_NOTIFY_TERMINATE:
            wcmResult = WCM_TerminateAsync();
            PrintSubScreen("WCM_TerminateAsync() -> %s\n", gWcmResultText[wcmResult]);
            break;

        default:
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
            FreezeThread();
        }

        break;
    }

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
    PrintSubScreen(" cb %s %s\n", gWcmNotifyText[arg->notify], gWcmResultText[arg->result]);

    switch (arg->notify)
    {
    case WCM_NOTIFY_STARTUP:
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
        if (arg->parameter[1].n == 0)
        {
            // 明示的な切断要求に 1 対 1 に対応する結果応答
            gWcmResult = arg->result;
            OS_WakeupThread(&gWcmThreadQ);
        }
        else
        {
            // AP から切断されたことを示す不定期通知
            (void)OS_JamMessage(&gWcmMessageQ, LOCAL_WCM_MSG_DISCONNECTED, OS_MESSAGE_NOBLOCK);
        }
        break;

    case WCM_NOTIFY_FOUND_AP:
        // 自動探索時に AP を発見したことを示す不定期通知
        {
            OSMessage   tempMessage;

            // 何度も通知されるものなので、メッセージキューが空である場合のみ通知する
            if (OS_ReadMessage(&gWcmMessageQ, &tempMessage, OS_MESSAGE_NOBLOCK) == FALSE)
            {
                (void)OS_SendMessage(&gWcmMessageQ, LOCAL_WCM_MSG_FOUND_AP, OS_MESSAGE_NOBLOCK);
            }
        }
        break;

    case WCM_NOTIFY_SEARCH_AROUND:
        // 自動探索時に指定チャンネルを一巡したことを示す不定期通知

        /*
         * このサンプルプログラムでは発生することはあり得ないので、無視する。
         */
        break;
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
