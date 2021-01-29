/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-Model
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.33  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.32  2006/03/13 06:39:01  yosizaki
  add test of MB_SetUserVolatData..

  Revision 1.31  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.30  2005/11/21 10:51:51  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.29  2005/04/11 08:50:25  yosizaki
  change to use WM_GetNextTgid().

  Revision 1.28  2005/03/08 10:29:04  yosizaki
  change to initialize TGID at random.

  Revision 1.27  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.26  2005/02/04 11:02:55  yasu
  著作年の修正

  Revision 1.25  2005/02/02 09:05:14  seiki_masashi
  sChildInfo の添え字溢れを修正

  Revision 1.24  2004/12/28 09:34:44  pair2
  WH_Initialize() の重複呼び出しに関するフレームワークの修正.

  Revision 1.23  2004/12/03 08:12:58  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX に修正、 MBP_USING_PREVIOUS_DOWNLOADスイッチに対応

  Revision 1.22  2004/11/12 09:01:20  yosizaki
  change mb_parent to /wireless_shared/mbp

  Revision 1.21  2004/11/10 01:12:28  takano_makoto
  子機が14台しか表示されていなかったのを修正

  Revision 1.20  2004/11/02 18:08:07  takano_makoto
  fix comments.

  Revision 1.19  2004/10/28 01:31:09  takano_makoto
  wmhighをwireless_sharedのwhへ移行

  Revision 1.18  2004/10/27 02:06:41  takano_makoto
  再起動後にtgidをインクリメントするように修正

  Revision 1.17  2004/10/25 06:29:56  takano_makoto
  最大接続子機の設定を追加

  Revision 1.16  2004/10/21 00:42:42  yosizaki
  add SHARED_WH switch.

  Revision 1.15  2004/10/18 11:44:06  yosizaki
  change to use MB_StartParentEx/MB_EndEx (switch)

  Revision 1.14  2004/10/08 01:27:10  takano_makoto
  表示子機の番号ズレの不具合修正

  Revision 1.13  2004/10/05 09:45:29  terui
  MACアドレスをu8型の配列に統一。

  Revision 1.12  2004/10/05 07:49:36  takano_makoto
  子機のAcceptをmb_parent.cのコールバック内で処理するように修正

  Revision 1.11  2004/10/05 03:58:49  takano_makoto
  コメントの微修正

  Revision 1.10  2004/10/05 02:15:05  takano_makoto
  エントリー状態で待って、すべての子機で一斉にダウンロードを開始するように変更。

  Revision 1.9  2004/09/30 08:28:26  yosizaki
  add SDK_MAKEGGID_SYSTEM macro.

  Revision 1.8  2004/09/27 08:35:49  takano_makoto
  親機再起動時にチャンネル番号が変化してしまっていたのを修正

  Revision 1.7  2004/09/21 07:26:06  takano_makoto
  コメント追加

  Revision 1.4  2004/09/17 10:36:11  takano_makoto
  BeaconPeriodを標準関数srand経由で調整していたのを変更

  Revision 1.3  2004/09/17 06:11:59  takano_makoto
  ユーザ情報をMBP_GetChildInfo()経由で取得するように変更

  Revision 1.2  2004/09/17 02:20:23  sato_masaki
  TODO: に対する回答コメントを追加。

  Revision 1.1  2004/09/16 14:06:41  takano_makoto
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>

#include "mbp.h"
#include "common.h"
#include "disp.h"
#include "gmain.h"
#include "wh.h"

/*
 * マルチブート後再接続するアプリケーションのサンプル.
 *
 * MBライブラリのサンプルはマルチブート機能を使うため、
 * 同じ通信環境(無線あるいは有線)どうしの開発機材を複数必要とします。
 * $NitroSDK/bin/ARM9-TS/Release/ ディレクトリにある
 * mb_child.bin プログラムが最終実機におけるマルチブート子機と同等の
 * 機能を提供するサンプルになっていますので、
 * このバイナリをサンプルプログラムと同じ方法で他の機材に読み込み、
 * 一緒に実行してください。
 * 
 */

/******************************************************************************/

static void WaitPressButton(void);
static void GetChannelMain(void);
static BOOL ConnectMain(u16 tgid);
static void PrintChildState(void);
static BOOL JudgeConnectableChild(WMStartParentCallback *cb);
static void OnPreSendMBVolat(u32 ggid);


//============================================================================
//  定数定義
//============================================================================

/* このデモで使用する GGID */
#define WH_GGID                 SDK_MAKEGGID_SYSTEM(0x21)


/* このデモがダウンロードさせるプログラム情報 */
const MBGameRegistry mbGameList = {
    "/child.srl",                      // 子機バイナリコード
    (u16 *)L"DataShareDemo",           // ゲーム名
    (u16 *)L"DataSharing demo",        // ゲーム内容説明
    "/data/icon.char",                 // アイコンキャラクタデータ
    "/data/icon.plt",                  // アイコンパレットデータ
    WH_GGID,                           // GGID
    MBP_CHILD_MAX + 1,                 // 最大プレイ人数、親機の数も含めた人数
};



//============================================================================
//   変数定義
//============================================================================

static s32 gFrame;                     // フレームカウンタ

//-----------------------
// 通信経路の保持用
//-----------------------
static u16 sChannel = 0;
static const MBPChildInfo *sChildInfo[MBP_CHILD_MAX];


//============================================================================
//   関数定義
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メインルーチン

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    u16     tgid = 0;

    // 画面、OSの初期化
    CommonInit();
    // 画面初期化
    DispInit();
    // ヒープの初期化
    InitAllocateSystem();

    // WH に情報を設定
    WH_SetGgid(WH_GGID);

    // 割り込み有効
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    DispOn();

    /*
     * DSダウンロードプレイの子機は、親機のTGIDが同じ場合、親機の情報を更新しません。
     * このためDSダウンロードプレイの親機では、起動毎にTGIDを
     * なるべく異なる値にばらつかせるように配慮する必要があります。
     * これは通常、 WM_GetNextTgid() 関数を使用すれば
     * RTC に基づいてある程度一意な値を取得することができます。
     */
    tgid = WM_GetNextTgid();

    while (TRUE)
    {
        // Aボタン押下待ち
        WaitPressButton();

        // トラフィックの少ないチャンネルの検索処理
        GetChannelMain();

        /*
         * tgidは親機が起動の度に基本的には前回と違う値を設定します。
         * ただしマルチブート子機との再接続時には同じtgidで起動しなければ
         * 再スキャンを行なわなければ接続できなくなるため注意が必要です。
         * もう一度スキャンを行なってから再接続をする場合にはtgidを保存しておく
         * 必要はありません。
         */
        // マルチブート配信処理
        if (ConnectMain(++tgid))
        {
            // マルチブート子機の起動に成功
            break;
        }
    }

    //--------------
    // マルチブート後は子機がリセットされ通信が一旦切断されます。
    // また現状親機も一度MB_End()から通信を終了する必要があります。
    // 親子共一度完全に切断された状態で一から通信を確立してください。
    // 
    // またこの時子機のaidがシャッフルされるため、もし必要があれば
    // マルチブート前のaidとMACアドレスの組み合わせを保存しておき、
    // 再接続時に新しいaidとの結びつけを行なってください。
    //--------------


    // データシェアリング通信用にバッファを設定
    GInitDataShare();

#if !defined(MBP_USING_MB_EX)
    (void)WH_Initialize();
#endif

    // 接続子機の判定用関数を設定
    WH_SetJudgeAcceptFunc(JudgeConnectableChild);

    /* メインルーチン */
    for (gFrame = 0; TRUE; gFrame++)
    {
        OS_WaitVBlankIntr();

        ReadKey();

        BgClear();

        switch (WH_GetSystemState())
        {
        case WH_SYSSTATE_IDLE:
            /* ----------------
             * 子機側で再スキャンなしに同じ親機に再接続させたい場合には
             * 子機側とtgid及びchannelを合わせる必要があります。
             * このデモでは、マルチブート時と同じchannelとマルチブート時のtgid+1を
             * 親子ともに使用することで、再スキャンなしでも接続できるようにしています。
             * 
             * MACアドレスを指定して再スキャンさせる場合には同じtgid, channelでなくても
             * 問題ありません。
             * ---------------- */
            (void)WH_ParentConnect(WH_CONNECTMODE_DS_PARENT, (u16)(tgid + 1), sChannel);
            break;

        case WH_SYSSTATE_CONNECTED:
        case WH_SYSSTATE_KEYSHARING:
        case WH_SYSSTATE_DATASHARING:
            {
                BgPutString(8, 1, 0x2, "Parent mode");
                GStepDataShare(gFrame);
                GMain();
            }
            break;
        }
    }
}



/*---------------------------------------------------------------------------*
  Name:         WaitPressButton

  Description:  Aボタンが押されるのをループで待つ関数。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WaitPressButton(void)
{
    while (TRUE)
    {
        OS_WaitVBlankIntr();
        ReadKey();
        BgClear();
        BgSetMessage(PLTT_WHITE, " Push A Button to start   ");
        if (IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            return;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         GetChannelMain

  Description:  使用するチャンネルを電波使用率を調べてまじめに求める。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetChannelMain(void)
{

    /*-----------------------------------------------*
     * チャンネルの電波使用率をきちんと調べた上で、
     * 一番使用率の低いチャンネルを選択します。
     * WM_MeasureChannel()を実行するにはIDLE状態になる必要があり
     * マルチブート状態ではIDLE状態に止まる事がないので実行できません。
     * 一旦WM_Initializeを呼んで電波使用率を調べてからWM_Endで終了し、
     * あらためてMB_Initを実行する。
     *-----------------------------------------------*/
    (void)WH_Initialize();

    while (TRUE)
    {
        ReadKey();
        BgClear();
        BgSetMessage(PLTT_YELLOW, " Search Channel ");

        switch (WH_GetSystemState())
        {
            //-----------------------------------------
            // 初期化完了
        case WH_SYSSTATE_IDLE:
            (void)WH_StartMeasureChannel();
            break;
            //-----------------------------------------
            // チャンネル検索完了
        case WH_SYSSTATE_MEASURECHANNEL:
            {
                sChannel = WH_GetMeasureChannel();
#if !defined(MBP_USING_MB_EX)
                (void)WH_End();
#else
                /* IDLE 状態を維持したままマルチブート処理へ */
                return;
#endif
            }
            break;
            //-----------------------------------------
            // WM終了
        case WH_SYSSTATE_STOP:
            /* WM_Endが完了したらマルチブート処理へ */
            return;
            //-----------------------------------------
            // ビジー中
        case WH_SYSSTATE_BUSY:
            break;
            //-----------------------------------------
            // エラー発生
        case WH_SYSSTATE_ERROR:
            (void)WH_Reset();
            break;
            //-----------------------------------------
        default:
            OS_Panic("Illegal State\n");
        }
        OS_WaitVBlankIntr();           // Vブランク割込終了待ち
    }
}


/*---------------------------------------------------------------------------*
  Name:         ConnectMain

  Description:  マルチブートで接続する。

  Arguments:    tgid        親機として起動する場合のtgidを指定します.

  Returns:      子機への転送に成功した場合には TRUE,
                失敗したりキャンセルされた場合には  FALSE を返します。
 *---------------------------------------------------------------------------*/
static BOOL ConnectMain(u16 tgid)
{
    MBP_Init(mbGameList.ggid, tgid);

    while (TRUE)
    {
        ReadKey();

        BgClear();

        BgSetMessage(PLTT_YELLOW, " MB Parent ");

        switch (MBP_GetState())
        {
            //-----------------------------------------
            // アイドル状態
        case MBP_STATE_IDLE:
            {
                MBP_Start(&mbGameList, sChannel);
                /* ユーザ定義データの送信テスト */
                MB_SetSendVolatCallback(OnPreSendMBVolat, MB_SEND_VOLAT_CALLBACK_TIMMING_BEFORE);
            }
            break;

            //-----------------------------------------
            // 子機からのエントリー受付中
        case MBP_STATE_ENTRY:
            {
                BgSetMessage(PLTT_WHITE, " Now Accepting            ");

                if (IS_PAD_TRIGGER(PAD_BUTTON_B))
                {
                    // Bボタンでマルチブートキャンセル
                    MBP_Cancel();
                    break;
                }

                // エントリー中の子機が一台でも存在すれば開始可能とする
                if (MBP_GetChildBmp(MBP_BMPTYPE_ENTRY) ||
                    MBP_GetChildBmp(MBP_BMPTYPE_DOWNLOADING) ||
                    MBP_GetChildBmp(MBP_BMPTYPE_BOOTABLE))
                {
                    BgSetMessage(PLTT_WHITE, " Push START Button to start   ");

                    if (IS_PAD_TRIGGER(PAD_BUTTON_START))
                    {
                        // ダウンロード開始
                        MBP_StartDownloadAll();
                    }
                }
            }
            break;

            //-----------------------------------------
            // プログラム配信処理
        case MBP_STATE_DATASENDING:
            {

                // 全員がダウンロード完了しているならばスタート可能.
                if (MBP_IsBootableAll())
                {
                    // ブート開始
                    MBP_StartRebootAll();
                }
            }
            break;

            //-----------------------------------------
            // リブート処理
        case MBP_STATE_REBOOTING:
            {
                BgSetMessage(PLTT_WHITE, " Rebooting now                ");
            }
            break;

            //-----------------------------------------
            // 再接続処理
        case MBP_STATE_COMPLETE:
            {
                // 全員無事に接続完了したらマルチブート処理は終了し
                // 通常の親機として無線を再起動する。
                BgSetMessage(PLTT_WHITE, " Reconnecting now             ");

                OS_WaitVBlankIntr();
                return TRUE;
            }
            break;

            //-----------------------------------------
            // エラー発生
        case MBP_STATE_ERROR:
            {
                // 通信をキャンセルする
                MBP_Cancel();
            }
            break;

            //-----------------------------------------
            // 通信キャンセル処理中
        case MBP_STATE_CANCEL:
            // None
            break;

            //-----------------------------------------
            // 通信異常終了
        case MBP_STATE_STOP:
#ifdef MBP_USING_MB_EX
            switch (WH_GetSystemState())
            {
            case WH_SYSSTATE_IDLE:
                (void)WH_End();
                break;
            case WH_SYSSTATE_BUSY:
                break;
            case WH_SYSSTATE_STOP:
                return FALSE;
            default:
                OS_Panic("illegal state\n");
            }
#else
            return FALSE;
#endif
        }

        // 子機状態を表示する
        PrintChildState();

        OS_WaitVBlankIntr();           // Vブランク割込終了待ち
    }
}


/*---------------------------------------------------------------------------*
  Name:         PrintChildState

  Description:  子機情報を画面に表示する

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintChildState(void)
{
    static const char *STATE_NAME[] = {
        "NONE       ",
        "CONNECTING ",
        "REQUEST    ",
        "ENTRY      ",
        "DOWNLOADING",
        "BOOTABLE   ",
        "BOOTING    ",
    };
    enum
    {
        STATE_DISP_X = 2,
        INFO_DISP_X = 15,
        BASE_DISP_Y = 2
    };

    u16     i;

    /* 子機リストの表示 */
    for (i = 1; i <= MBP_CHILD_MAX; i++)
    {
        const MBPChildInfo *childInfo;
        MBPChildState childState = MBP_GetChildState(i);
        const u8 *macAddr;

        SDK_ASSERT(childState < MBP_CHILDSTATE_NUM);

        // 状態表示
        BgPutString(STATE_DISP_X, i + BASE_DISP_Y, PLTT_WHITE, STATE_NAME[childState]);

        // ユーザー情報表示
        childInfo = MBP_GetChildInfo(i);
        macAddr = MBP_GetChildMacAddress(i);

        if (macAddr != NULL)
        {
            BgPrintStr(INFO_DISP_X, i + BASE_DISP_Y, PLTT_WHITE,
                       "%02x%02x%02x%02x%02x%02x",
                       macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         JudgeConnectableChild

  Description:  再接続時に接続可能な子機かどうかを判定する関数

  Arguments:    cb      接続してきた子機の情報.

  Returns:      接続を受け付ける場合は TRUE.
                受け付けない場合は FALSE.
 *---------------------------------------------------------------------------*/
static BOOL JudgeConnectableChild(WMStartParentCallback *cb)
{
    u16     playerNo;

    /*  cb->aid の子機のマルチブート時のaidをMACアドレスから検索します */
    playerNo = MBP_GetPlayerNo(cb->macAddress);

    OS_TPrintf("MB child(%d) -> DS child(%d)\n", playerNo, cb->aid);

    if (playerNo == 0)
    {
        return FALSE;
    }

    sChildInfo[playerNo - 1] = MBP_GetChildInfo(playerNo);
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         OnPreSendMBVolat

  Description:  親機がMBビーコンを送信する前に通知されるコールバック関数

  Arguments:    ggid    送信するゲーム情報のGGID.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void OnPreSendMBVolat(u32 ggid)
{
    /*
     * 複数のゲームを登録し個々のゲームごとに設定値を切り替える場合は
     * 以下のように引数の ggid でゲーム情報を判定します.
     */
    if (ggid == mbGameList.ggid)
    {
        /*
         * 送信可能なユーザ定義データは最大 8 BYTE です.
         * この例ではインクリメンタルな 64bit 値を送信しています.
         * 動的に更新されたデータ内容を、子機側では必ずしもただちに
         * 受信できるわけではないという点に注意してください.
         */
        static u64 count ATTRIBUTE_ALIGN(8);
        SDK_COMPILER_ASSERT(sizeof(count) <= MB_USER_VOLAT_DATA_SIZE);
        SDK_COMPILER_ASSERT(MB_USER_VOLAT_DATA_SIZE == 8);
        ++count;
        MB_SetUserVolatData(ggid, (u8 *)&count, sizeof(count));
    }
}
