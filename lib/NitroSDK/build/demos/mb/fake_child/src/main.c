/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - fake_child
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.17  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.16  2006/03/13 06:39:22  yosizaki
  add test of MB_SetUserVolatData..

  Revision 1.15  2006/01/20 02:05:13  seiki_masashi
  WM_MPParameter 関数の追加に関するデバッグ表示の変更

  Revision 1.14  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:49:28  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.12  2005/09/02 08:44:03  yosizaki
  fix symbol name.

  Revision 1.11  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.10  2005/02/18 11:06:37  yosizaki
  fix around hidden warnings.

  Revision 1.9  2005/01/11 07:32:58  takano_makoto
  fix copyright header.

  Revision 1.8  2005/01/07 06:00:34  takano_makoto
  コメント修正

  Revision 1.7  2005/01/07 02:54:11  takano_makoto
  WH_SYSSTATE_CONNECT_FAIL時にリセットしてから再接続するように修正。デバッグ出力を追加。

  Revision 1.6  2004/11/26 00:23:04  takano_makoto
  シーケンスの修正、コメント追加

  Revision 1.4  2004/11/25 07:39:13  takano_makoto
  旧バージョンのIPL用のOwnerInfoの操作を修正

  Revision 1.2  2004/11/24 13:09:10  takano_makoto
  エラー処理、キャンセル処理を追加

  Revision 1.1  2004/11/22 13:01:22  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "common.h"
#include "disp.h"
#include "gmain.h"
#include "wh.h"


#define     DEBUG_PRINT

/*
 * このサンプルではdemos/mb/multiboot-Modelに対して子機としてエントリーし、
 * 他のDSダウンロードプレイ子機と混在して通信に参加します。
 * MB_FakeScanParent関数のGGID引数によって、対象とするアプリケーションを
 * 設定します。
 */


enum
{
    MODE_TITLE,
    MODE_START_SCAN,
    MODE_SCANNING,
    MODE_ENTRY,
    MODE_RECONNECT,
    MODE_GMAIN,
    MODE_ERROR,
    MODE_CANCEL
};

typedef struct
{
    MBUserInfo info;
    u16     valid;
    u8      maxPlayerNum;              // 最大プレイヤー数
    u8      nowPlayerNum;              // 現在のプレイヤー数
    u8      macAddr[6];                // MACアドレス
}
ParentInfo;

#define WH_GGID (0x003FFF00 | (0x21))

static void PrintError(MBFakeScanErrorCallback *arg);

static void ModeTitle(void);
static void ModeStartScan(void);
static void ModeScanning(void);
static void ModeEntry(void);
static void ModeError(void);
static void ModeReconnect(void);
static void ModeGMain(void);
static void ModeCancel(void);

static void NotifyScanParent(u16 type, void *arg);
static void MBStateCallback(u32 status, void *arg);


// 親機情報リスト
static ParentInfo parentInfo[MB_GAME_INFO_RECV_LIST_NUM];
static ParentInfo parentInfoBuf[MB_GAME_INFO_RECV_LIST_NUM];

static WMBssDesc sParentBssDesc;       // エントリーした親機のBssDesc

static u8 *mbfBuf = NULL;
static u8 *wmBuf = NULL;


static s32 gFrame;                     // フレームカウンタ

static u16 sMode;

enum
{
    WSTATE_STOP,                       // 停止状態
    WSTATE_INITIALIZE_BUSY,            // ワイヤレス初期化中
    WSTATE_IDLE,                       // アイドル状態
    WSTATE_ERROR,                      // フェイク子機開始前または終了後のエラー
    WSTATE_FAKE_SCAN,                  // スキャン状態
    WSTATE_FAKE_END_SCAN,              // スキャン終了
    WSTATE_FAKE_ENTRY,                 // 親機へエントリー中
    WSTATE_FAKE_BOOT_READY,            // DSダウンロードプレイへのエントリー完了
    WSTATE_FAKE_BOOT_END_BUSY,         // DSダウンロード処理終了中
    WSTATE_FAKE_BOOT_END,
    WSTATE_FAKE_CANCEL_BUSY,           // DSダウンロードキャンセル中
    WSTATE_FAKE_ERROR                  // エラー状態
};
static u16 sWirelessState = WSTATE_STOP;

static u16 sConnectIndex;              // 接続する親機インデックス

/******************************************************************************/
/* function */

#ifdef DEBUG_PRINT
static void ChangeWirelessState(u16 wstate)
{
    static const char *WSTATE_NAME[] = {
        "WSTATE_STOP",
        "WSTATE_INITIALIZE_BUSY",
        "WSTATE_IDLE",
        "WSTATE_ERROR",
        "WSTATE_FAKE_SCAN",
        "WSTATE_FAKE_END_SCAN",
        "WSTATE_FAKE_ENTRY",
        "WSTATE_FAKE_BOOT_READY",
        "WSTATE_FAKE_BOOT_END_BUSY",
        "WSTATE_FAKE_BOOT_END",
        "WSTATE_FAKE_CANCEL_BUSY",
        "WSTATE_FAKE_ERROR"
    };

    if (sWirelessState != wstate)
    {
        OS_TPrintf("change sWirelessState %s -> %s\n", WSTATE_NAME[sWirelessState],
                   WSTATE_NAME[wstate]);
    }
#else
static inline void ChangeWirelessState(u16 wstate)
{
#endif
    sWirelessState = wstate;
}

/* Ｖブランク割り込み処理 */
static void VBlankIntr(void)
{
    //---- 割り込みチェックフラグ
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


/* メイン */
void NitroMain()
{
    // システムの初期化
    CommonInit();
    // 画面初期化
    DispInit();
    // ヒープの初期化
    InitAllocateSystem();

    // 割り込み有効
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // データシェアリング用のバッファをクリア
    GInitDataShare();

    // タイトル画面へ
    sMode = MODE_TITLE;

    // LCD表示開始
    GX_DispOn();
    GXS_DispOn();

    // デバッグ文字列出力
    OS_TPrintf("MB fake child: Simple DataSharing demo started.\n");
    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    ReadKey();

    /* メインループ */
    for (gFrame = 0; TRUE; gFrame++)
    {
        // キー入力情報取得
        ReadKey();

        // スクリーンクリア
        BgClear();

        switch (sMode)
        {
        case MODE_TITLE:
            // タイトル画面の表示
            ModeTitle();
            break;
        case MODE_START_SCAN:
            // スキャン開始
            ModeStartScan();
            break;
        case MODE_SCANNING:
            // MB親機をスキャン中
            ModeScanning();
            break;
        case MODE_ENTRY:
            // MB親機へエントリー
            ModeEntry();
            break;
        case MODE_RECONNECT:
            // MB親機へ再接続
            ModeReconnect();
            break;
        case MODE_GMAIN:
            // ゲームメイン
            ModeGMain();
            break;
        case MODE_ERROR:
            // エラー処理
            ModeError();
            break;
        case MODE_CANCEL:
            // キャンセル中
            ModeCancel();
            break;
        }

        OS_WaitVBlankIntr();
    }
}



/* ----------------------------------------------------------------------
  Name:        ModeTitle
  
  Description: タイトル画面
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeTitle(void)
{
    BgSetMessage(PLTT_WHITE, " Press Button A to start   ");

    ChangeWirelessState(WSTATE_STOP);

    // Aボタンが押されたら無線をONにしてMB親機の検索モードへ移行する。
    if (IS_PAD_TRIGGER(PAD_BUTTON_A))
    {
        sMode = MODE_START_SCAN;
    }
}

/* ----------------------------------------------------------------------
  Name:        ModeStartScan
  
  Description: 親機のスキャン開始処理
  
  Arguments:   None.
               
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeStartScan(void)
{
    BgSetMessage(PLTT_WHITE, "Now working....         ");

    switch (sWirelessState)
    {
        //--------------------------
        // ワイヤレス通信の開始
    case WSTATE_STOP:
        if (!WH_Initialize())
        {
            ChangeWirelessState(WSTATE_ERROR);
            sMode = MODE_ERROR;
            return;
        }
        ChangeWirelessState(WSTATE_INITIALIZE_BUSY);
        break;

        //--------------------------
        // ワイヤレスの初期化中
    case WSTATE_INITIALIZE_BUSY:
        if (WH_GetSystemState() == WH_SYSSTATE_BUSY)
        {
            BgSetMessage(PLTT_WHITE, "Now working....         ");
            return;
        }
        if (WH_GetSystemState() != WH_SYSSTATE_IDLE)
        {
            ChangeWirelessState(WSTATE_ERROR);
            sMode = MODE_ERROR;
            return;
        }
        ChangeWirelessState(WSTATE_IDLE);
        // don't break;

        //--------------------------
        // 親機のスキャンを開始
    case WSTATE_IDLE:
        {
            // ユーザー情報を設定
            MBUserInfo userInfo;

#if ( SDK_NVRAM_FORMAT >= 100 )
            OSOwnerInfo info;

            OS_GetOwnerInfo(&info);
            userInfo.favoriteColor = info.favoriteColor;
            userInfo.nameLength = (u8)info.nickNameLength;
            MI_CpuCopy8(info.nickName, userInfo.name, (u32)(userInfo.nameLength * 2));
#else  // 旧IPL用
            NVRAMConfig *info = (NVRAMConfig *)OS_GetSystemWork()->nvramUserInfo;

#define DEFAULT_FAVORIT_COLOR   1
            userInfo.favoriteColor = DEFAULT_FAVORIT_COLOR;
            userInfo.nameLength = (u8)info->ncd.owner.nickname.length;
            MI_CpuCopy8(info->ncd.owner.nickname.name, userInfo.name,
                        NVRAM_CONFIG_NICKNAME_LENGTH * 2);
#endif
            // 通常のMB子機ではplayerNoは0となるが、値を設定することで親機へパラメータとして渡すことが可能。
            userInfo.playerNo = 7;

            // マルチブートエントリー用のワークバッファを確保
            mbfBuf = (u8 *)OS_Alloc(MB_FakeGetWorkSize());
            MB_FakeInit(mbfBuf, &userInfo);
            // 状態遷移通知コールバック関数を設定
            MB_FakeSetCStateCallback(MBStateCallback);

            // 親機リストのクリア
            MI_CpuClear8(parentInfo, sizeof(parentInfo));

            // ワイヤレス状態をスキャン中へ
            ChangeWirelessState(WSTATE_FAKE_SCAN);

            // スキャンする親機のGGIDと、通知用コールバック関数を設定し、スキャンを開始
            MB_FakeStartScanParent(NotifyScanParent, WH_GGID);

            // スキャンモードへ遷移
            sMode = MODE_SCANNING;
        }
        break;
    default:
        OS_TPanic("illegal state %d", sWirelessState);
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeScanning
  
  Description: 親機のスキャン中
  
  Arguments:   None.
               
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeScanning(void)
{
    enum
    { STR_X = 4, STR_Y = 4, NUM_STR_X = 24 };
    static u16 cursol_idx = 0;

    u16     i;

    // エラーが発生していたら終了
    if (sWirelessState == WSTATE_FAKE_ERROR)
    {
        sMode = MODE_ERROR;
        return;
    }

    // 描画中に割り込みによって変更されないよう親機情報を
    // 一旦テンポラリバッファへコピーしてから使用する。
    {
        OSIntrMode enabled = OS_DisableInterrupts();
        MI_CpuCopy8(parentInfo, parentInfoBuf, sizeof(parentInfo));
        (void)OS_RestoreInterrupts(enabled);
    }

    // カーソルの移動
    if (IS_PAD_TRIGGER(PAD_KEY_UP))
    {
        cursol_idx = (u16)((cursol_idx - 1) & 0xF);
    }
    else if (IS_PAD_TRIGGER(PAD_KEY_DOWN))
    {
        cursol_idx = (u16)((cursol_idx + 1) & 0xF);
    }

    // 親機情報の表示
    BgPutString(10, 1, PLTT_RED, "Select Parent");

    BgPutString(STR_X - 2, STR_Y + cursol_idx, PLTT_WHITE, ">");
    for (i = 0; i < MB_GAME_INFO_RECV_LIST_NUM; i++)
    {
        if (parentInfoBuf[i].valid)
        {
            BgPrintStr(STR_X, STR_Y + i, PLTT_GREEN, "%02x:%02x:%02x:%02x:%02x:%02x",
                       parentInfoBuf[i].macAddr[0], parentInfoBuf[i].macAddr[1],
                       parentInfoBuf[i].macAddr[2], parentInfoBuf[i].macAddr[3],
                       parentInfoBuf[i].macAddr[4], parentInfoBuf[i].macAddr[5]);
            BgPrintStr(NUM_STR_X, STR_Y + i, PLTT_GREEN, "%d/%d",
                       parentInfoBuf[i].nowPlayerNum, parentInfoBuf[i].maxPlayerNum);
        }
        else
        {
            BgPutString(STR_X, STR_Y + i, PLTT_WHITE, "Searching...");
        }
    }

    // 通信をキャンセル
    if (IS_PAD_TRIGGER(PAD_BUTTON_B))
    {
        ChangeWirelessState(WSTATE_FAKE_CANCEL_BUSY);
        sMode = MODE_CANCEL;
        MB_FakeEnd();
        return;
    }

    // 接続する親機を選択
    if (IS_PAD_TRIGGER(PAD_BUTTON_A))
    {
        if (!parentInfoBuf[cursol_idx].valid)
        {
            return;
        }

        // 接続する親機の情報を取得
        sConnectIndex = cursol_idx;
        if (!MB_FakeReadParentBssDesc
            (sConnectIndex, &sParentBssDesc, WH_PARENT_MAX_SIZE, WH_CHILD_MAX_SIZE, FALSE, FALSE))
        {
            // 親機情報が有効でなかったらスキャン続行
            return;
        }
        sMode = MODE_ENTRY;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeEntry
  
  Description: 親機へのエントリー
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeEntry(void)
{
    BgSetMessage(PLTT_WHITE, "Now Entrying...        ");

    switch (sWirelessState)
    {
        //--------------------------
        // スキャン中の場合
    case WSTATE_FAKE_SCAN:
        {
            // スキャンを終了する
            MB_FakeEndScan();
            ChangeWirelessState(WSTATE_FAKE_END_SCAN);
        }
        break;

        //--------------------------
        // スキャン終了中
    case WSTATE_FAKE_END_SCAN:
        break;

        //--------------------------
        // ワイヤレス通信の開始
    case WSTATE_IDLE:
        {
            if (!MB_FakeEntryToParent(sConnectIndex))
            {
                // 指定されたIndexが有効なものでなければエラー
                OS_TPrintf("ERR: Illegal Parent index\n");
                ChangeWirelessState(WSTATE_FAKE_ERROR);
                sMode = MODE_ERROR;
                return;
            }
            ChangeWirelessState(WSTATE_FAKE_ENTRY);
        }
        break;

        //--------------------------
        // エントリー中
    case WSTATE_FAKE_ENTRY:
        // エントリーが完了していなければBボタンでキャンセル
        if (IS_PAD_TRIGGER(PAD_BUTTON_B))
        {
            ChangeWirelessState(WSTATE_FAKE_CANCEL_BUSY);
            sMode = MODE_CANCEL;
            MB_FakeEnd();
            return;
        }
        break;

        //--------------------------
        // エントリー処理完了
    case WSTATE_FAKE_BOOT_READY:
        {
            // エントリー処理の終了処理
            ChangeWirelessState(WSTATE_FAKE_BOOT_END_BUSY);
            MB_FakeEnd();
        }
        break;

        //--------------------------
        // 完了待ち
    case WSTATE_FAKE_BOOT_END_BUSY:
        break;

        //--------------------------
        // フェイク子機の処理完了
    case WSTATE_FAKE_BOOT_END:
        {
            // 親機としめしを合わせてtgidをインクリメント
            sParentBssDesc.gameInfo.tgid++;
            // 再接続処理を開始
            ChangeWirelessState(WSTATE_IDLE);
            sMode = MODE_RECONNECT;
        }
        break;

        //--------------------------
        // エラー終了
    case WSTATE_FAKE_ERROR:
        sMode = MODE_ERROR;
        break;

    default:
        OS_TPanic("illegal state %d\n", sWirelessState);
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeReconnect
  
  Description: タイトル画面
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeReconnect(void)
{
    BgSetMessage(PLTT_WHITE, "Connecting to Parent...        ");

    switch (WH_GetSystemState())
    {
    case WH_SYSSTATE_CONNECT_FAIL:
        {
            // WM_StartConnect()に失敗した場合にはWM内部のステートが不正になっている為
            // 一度WM_ResetでIDLEステートにリセットする必要があります。
            WH_Reset();
        }
        break;
    case WH_SYSSTATE_IDLE:
        {
            (void)WH_ChildConnect(WH_CONNECTMODE_DS_CHILD, &sParentBssDesc);
        }
        break;
    case WH_SYSSTATE_ERROR:
        ChangeWirelessState(WSTATE_ERROR);
        sMode = MODE_ERROR;
        break;
    case WH_SYSSTATE_BUSY:
        break;
    case WH_SYSSTATE_CONNECTED:
    case WH_SYSSTATE_KEYSHARING:
    case WH_SYSSTATE_DATASHARING:
        sMode = MODE_GMAIN;
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeGMain
  
  Description: タイトル画面
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeGMain(void)
{
    if (WH_GetSystemState() == WH_SYSSTATE_ERROR)
    {
        ChangeWirelessState(WSTATE_ERROR);
        sMode = MODE_ERROR;
        return;
    }
    GStepDataShare(gFrame);
    GMain();
}


/* ----------------------------------------------------------------------
  Name:        ModeCancel
  
  Description: キャンセル中画面
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeCancel(void)
{
    switch (sWirelessState)
    {
        //-------------------------
        // アイドル状態へ遷移したらワイヤレス終了
    case WSTATE_IDLE:
        if (WH_GetSystemState() == WH_SYSSTATE_IDLE)
        {
            (void)WH_End();
            return;
        }

        if (WH_GetSystemState() != WH_SYSSTATE_STOP)
        {
            return;
        }
        ChangeWirelessState(WSTATE_STOP);
        sMode = MODE_TITLE;
        break;
        //--------------------------
        // キャンセル中
    case WSTATE_FAKE_CANCEL_BUSY:
        break;

        //--------------------------
        // エラー発生
    case WSTATE_ERROR:
        ChangeWirelessState(WSTATE_IDLE);
        (void)WH_Finalize();
        break;

        //--------------------------
        // 終了処理
    default:
        ChangeWirelessState(WSTATE_IDLE);
        (void)WH_Finalize();
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        ModeError
  
  Description: エラー画面
  
  Arguments:   None.
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void ModeError(void)
{
    BgPutString(4, 10, PLTT_RED, "========== ERROR ==========");

    if (IS_PAD_TRIGGER(PAD_BUTTON_B) || IS_PAD_TRIGGER(PAD_BUTTON_A))
    {
        if (sWirelessState == WSTATE_FAKE_ERROR)
        {
            ChangeWirelessState(WSTATE_FAKE_CANCEL_BUSY);
            MB_FakeEnd();
        }
        sMode = MODE_CANCEL;
    }
}


/* ----------------------------------------------------------------------
  Name:        NotifyScanParent
  
  Description: 親機スキャン状況通知コールバック
  
  Arguments:   state    スキャン状態.
                        MB_FAKESCAN_PARENT_FOUND, MB_FAKESCAN_PARENT_LOST,
                        MB_FAKESCAN_API_ERROR, MB_FAKESCAN_END_SCAN
                        の４種類のメッセージが通知される。
                
               arg      MB_FAKESCAN_PARENT_FOUND, MB_FAKESCAN_PARENT_LOSTの場合には
                        対象となる親機の情報が取得できる。
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void NotifyScanParent(u16 type, void *arg)
{
    static const char *MB_FAKESCAN_CALLBACK_TYPE_NAME[] = {
        "MB_FAKESCAN_PARENT_FOUND",
        "MB_FAKESCAN_PARENT_LOST",
        "MB_FAKESCAN_API_ERROR",
        "MB_FAKESCAN_END_SCAN",
        "MB_FAKESCAN_PARENT_BEACON",
        "MB_FAKESCAN_CONNECTED",
        "MB_FAKESCAN_ENTRY",
        "MB_FAKESCAN_DISCONNECTED",
        "MB_FAKESCAN_SUCCESS"
    };

    OS_TPrintf("> %s\n", MB_FAKESCAN_CALLBACK_TYPE_NAME[type]);

    switch (type)
    {
    case MB_FAKESCAN_PARENT_BEACON:
        {
            MBFakeScanCallback *cb = (MBFakeScanCallback *)arg;
            WMBssDesc *bssdesc = cb->bssDesc;
            const u8 *volat_data = (const u8 *)MB_GetUserVolatData(&bssdesc->gameInfo);
            OS_TPrintf(" find MB-beacon MAC=(%04x%08x)\n",
                       *(u16 *)(&bssdesc->bssid[4]), *(u32 *)(&bssdesc->bssid[0]));
            if (volat_data)
            {
                OS_TPrintf("                VOLAT=(%02x %02x %02x %02x %02x %02x %02x %02x)\n",
                           volat_data[0], volat_data[1], volat_data[2], volat_data[3],
                           volat_data[4], volat_data[5], volat_data[6], volat_data[7]);
            }
        }
        break;
    case MB_FAKESCAN_PARENT_FOUND:
        {
            MBFakeScanCallback *cb = (MBFakeScanCallback *)arg;
            WMBssDesc *bssdesc = cb->bssDesc;
            MBGameInfo *gameInfo = cb->gameInfo;

            OS_TPrintf(" find parent %04x%08x %d\n", *(u16 *)(&bssdesc->bssid[4]),
                       *(u32 *)(&bssdesc->bssid[0]), cb->index);
            OS_TPrintf("      parentMaxSize = %d, childMaxSize = %d\n",
                       bssdesc->gameInfo.parentMaxSize, bssdesc->gameInfo.childMaxSize);

            parentInfo[cb->index].valid = 1;
            parentInfo[cb->index].maxPlayerNum = gameInfo->fixed.maxPlayerNum;
            parentInfo[cb->index].nowPlayerNum = gameInfo->volat.nowPlayerNum;
            MI_CpuCopy8(&gameInfo->fixed.parent, &parentInfo[cb->index].info, sizeof(MBUserInfo));
            WM_CopyBssid(bssdesc->bssid, parentInfo[cb->index].macAddr);
        }
        break;
    case MB_FAKESCAN_PARENT_LOST:
        {
            MBFakeScanCallback *cb = (MBFakeScanCallback *)arg;

            OS_TPrintf(" lost parent %d\n", cb->index);

            parentInfo[cb->index].valid = 0;
        }
        break;
    case MB_FAKESCAN_API_ERROR:
        {
            PrintError((MBFakeScanErrorCallback *)arg);
            OS_TPrintf("ERR : API Error occured\n");
            ChangeWirelessState(WSTATE_FAKE_ERROR);
        }
        break;
    case MB_FAKESCAN_END_SCAN:
        {
            ChangeWirelessState(WSTATE_IDLE);
        }
        break;
    default:
        break;
    }
}


/* ----------------------------------------------------------------------
  Name:        MBStateCallback
  
  Description: フェイク子機の状態遷移通知コールバックです。
               MB_FakeEntryToParent()関数がコールされた後は、
               子機ブート完了メッセージが完了するまでのシーケンスが
               自動的に実行されますので、特別に処理をする必要はありません。
               すべてのマルチブート処理が完了すると、MB_COMM_CSTATE_BOOT_READY
               コールバックが返ってきます。
  
  Arguments:   status   MB子機としての現在の状態
               arg      コールバック引数
                
  Returns:     None.
  ---------------------------------------------------------------------- */
static void MBStateCallback(u32 status, void *arg)
{
#pragma unused( arg )
    switch (status)
    {
    case MB_COMM_CSTATE_INIT_COMPLETE: // 親機への接続完了通知
    case MB_COMM_CSTATE_CONNECT:      // 親機への接続完了通知
    case MB_COMM_CSTATE_REQ_ENABLE:   // 親機とのMP確立通知
    case MB_COMM_CSTATE_DLINFO_ACCEPTED:       // 親機へのエントリー完了通知
    case MB_COMM_CSTATE_RECV_PROCEED: // 親機からのデータ送信開始通知
    case MB_COMM_CSTATE_RECV_COMPLETE: // 親機のデータ送信完了通知
    case MB_COMM_CSTATE_BOOTREQ_ACCEPTED:      // 親機からのブートリクエスト受信通知
        // 特別な処理は必要なし
        break;

    case MB_COMM_CSTATE_BOOT_READY:
        // 親機からの切断が完了しMBの処理が完了した事の通知
        {
            ChangeWirelessState(WSTATE_FAKE_BOOT_READY);
        }
        break;

    case MB_COMM_CSTATE_CANCELED:
        // 途中で親機へのエントリー処理をキャンセルした場合の通知
        break;

    case MB_COMM_CSTATE_CONNECT_FAILED:        // 親機への接続失敗通知
    case MB_COMM_CSTATE_DISCONNECTED_BY_PARENT:        // 親機からの切断通知
    case MB_COMM_CSTATE_REQ_REFUSED:  // 親機にリクエストを拒否された場合の通知
    case MB_COMM_CSTATE_MEMBER_FULL:  // 親機のエントリー子機数をオーバーしていた場合の通知
    case MB_COMM_CSTATE_ERROR:        // 途中で通信状態異常が発生した場合の処理
        {
            OS_TPrintf("ERR : MB Error occured reason 0x%x\n", status);
            ChangeWirelessState(WSTATE_FAKE_ERROR);
        }
        break;

    case MB_COMM_CSTATE_FAKE_END:
        // MBへ渡したワークを解放し
        OS_Free(mbfBuf);
        mbfBuf = NULL;

        if (sWirelessState == WSTATE_FAKE_BOOT_END_BUSY)
        {
            ChangeWirelessState(WSTATE_FAKE_BOOT_END);
        }
        else
        {
            ChangeWirelessState(WSTATE_IDLE);
        }
    }
}



/* ----------------------------------------------------------------------
  Name:        PrintError
  
  Description: WMのAPIエラーの内容をプリント出力します。。
  
  Arguments:   arg MB_FakeStartScanParentのエラーコールバック
  
  Returns:     None.
  ---------------------------------------------------------------------- */
static void PrintError(MBFakeScanErrorCallback *arg)
{
#pragma unused( arg )
    static const char *APIID_NAME[] = {
        "WM_APIID_INITIALIZE",
        "WM_APIID_RESET",
        "WM_APIID_END",
        "WM_APIID_ENABLE",
        "WM_APIID_DISABLE",
        "WM_APIID_POWER_ON",
        "WM_APIID_POWER_OFF",
        "WM_APIID_SET_P_PARAM",
        "WM_APIID_START_PARENT",
        "WM_APIID_END_PARENT",
        "WM_APIID_START_SCAN",
        "WM_APIID_END_SCAN",
        "WM_APIID_START_CONNECT",
        "WM_APIID_DISCONNECT",
        "WM_APIID_START_MP",
        "WM_APIID_SET_MP_DATA",
        "WM_APIID_END_MP",
        "WM_APIID_START_DCF",
        "WM_APIID_SET_DCF_DATA",
        "WM_APIID_END_DCF",
        "WM_APIID_SET_WEPKEY",
        "WM_APIID_START_KS",
        "WM_APIID_END_KS",
        "WM_APIID_GET_KEYSET",
        "WM_APIID_SET_GAMEINFO",
        "WM_APIID_SET_BEACON_IND",
        "WM_APIID_START_TESTMODE",
        "WM_APIID_STOP_TESTMODE",
        "WM_APIID_VALARM_MP",
        "WM_APIID_SET_LIFETIME",
        "WM_APIID_MEASURE_CHANNEL",
        "WM_APIID_INIT_W_COUNTER",
        "WM_APIID_GET_W_COUNTER",
        "WM_APIID_SET_ENTRY",
        "WM_APIID_AUTO_DEAUTH",
        "WM_APIID_SET_MP_PARAMETER",
        "WM_APIID_SET_BEACON_PERIOD",
        "WM_APIID_AUTO_DISCONNECT",
        "WM_APIID_START_SCAN_EX",
        "WM_APIID_KICK_MP_PARENT",
        "WM_APIID_KICK_MP_CHILD",
        "WM_APIID_KICK_MP_RESUME",
        "WM_APIID_ASYNC_KIND_MAX",
        "WM_APIID_INDICATION",
        "WM_APIID_PORT_SEND",
        "WM_APIID_PORT_RECV",
        "WM_APIID_READ_STATUS",
        "WM_APIID_UNKNOWN",
    };

    static const char *ERRCODE_NAME[] = {
        "WM_ERRCODE_SUCCESS",
        "WM_ERRCODE_FAILED",
        "WM_ERRCODE_OPERATING",
        "WM_ERRCODE_ILLEGAL_STATE",
        "WM_ERRCODE_WM_DISABLE",
        "WM_ERRCODE_NO_DATASET",
        "WM_ERRCODE_INVALID_PARAM",
        "WM_ERRCODE_NO_CHILD",
        "WM_ERRCODE_FIFO_ERROR",
        "WM_ERRCODE_TIMEOUT",
        "WM_ERRCODE_SEND_QUEUE_FULL",
        "WM_ERRCODE_NO_ENTRY",
        "WM_ERRCODE_OVER_MAX_ENTRY",
        "WM_ERRCODE_INVALID_POLLBITMAP",
        "WM_ERRCODE_NO_DATA",
        "WM_ERRCODE_SEND_FAILED",
        "WM_ERRCODE_DCF_TEST",
        "WM_ERRCODE_WL_INVALID_PARAM",
        "WM_ERRCODE_WL_LENGTH_ERR",
        "WM_ERRCODE_FLASH_ERROR"
    };

    OS_TPrintf("ERR: %s %s\n", APIID_NAME[arg->apiid], ERRCODE_NAME[arg->errcode]);
}
