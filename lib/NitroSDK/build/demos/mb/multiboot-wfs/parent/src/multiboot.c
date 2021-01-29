/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     multiboot.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: multiboot.c,v $
  Revision 1.2  2005/08/09 13:54:59  yosizaki
  fix gamename.

  Revision 1.1  2005/06/23 09:06:03  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"
#include    "wc.h"
#include    "mbp.h"

#include    "util.h"
#include    "common.h"


/*---------------------------------------------------------------------------*
    内部定数定義
 *---------------------------------------------------------------------------*/

/* このデモがダウンロードさせるプログラム情報 */
const MBGameRegistry mbGameList = {
    "data/main.srl",                   // 子機バイナリコード
    (u16 *)L"MultibootWFS",            // ゲーム名
    (u16 *)L"Multiboot-WFS demo",      // ゲーム内容説明
    "data/icon.char",                  // アイコンキャラクタデータ
    "data/icon.plt",                   // アイコンパレットデータ
    GGID_WBT_FS,                       // GGID
    MBP_CHILD_MAX + 1,                 // 最大プレイ人数、親機の数も含めた人数
};


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/

static void *sWmWork;

/* チャンネル計測用変数の初期化 */
static volatile BOOL sChannelBusy;
static u16 sChannel;
static u16 sChannelBusyRatio;
static u16 sChannelBitmap;


/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/

static void GetChannelProc(void *arg);
static void GetChannelMain(void);
static void ConnectMain(void);
static void PrintChildState(void);


/*---------------------------------------------------------------------------*
  Name:         ModeMultiboot

  Description:  DSダウンロードプレイ親機処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeMultiboot(void)
{
    /* 電波使用率の最も低いチャンネルを検索 */
    GetChannelMain();

    /* マルチブート配信処理 */
    ConnectMain();
}

/*---------------------------------------------------------------------------*
  Name:         GetChannelProc

  Description:  チャンネル計測のための一連の WM 関数コールバック.
                WM_Initialize() のコールバックとして指定した場合は
                使用可能な全チャンネルの電波率を計測し, 最低のものを
                sChannel へ格納して完了する.
                WM_End() のコールバックとして指定した場合はそのまま完了する.
                完了時に sChannelBusy を FALSE にする.

  Arguments:    arg     WM 関数コールバック構造体

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetChannelProc(void *arg)
{
    u16     channel = 1;
    u16     allowedChannel;

    WMCallback *cb = (WMCallback *)arg;

    /* WM_End() コールバックなら完了を通知 */
    if (cb->apiid == WM_APIID_END)
    {
        sChannelBusy = FALSE;
        return;
    }
    /* WM_Initialize() コールバックなら計測処理を開始 */
    else if (cb->apiid == WM_APIID_INITIALIZE)
    {
    }
    /* それ以外は WM_MeasureChannel() コールバック */
    else
    {
        WMMeasureChannelCallback *cb = (WMMeasureChannelCallback *)arg;
        if (cb->errcode != WM_ERRCODE_SUCCESS)
        {
            OS_TPanic("WM_MeasureChannel() failed!");
        }
        channel = cb->channel;
        /* 電波使用率がいままでで最低なら採用 */
        if (sChannelBusyRatio > cb->ccaBusyRatio)
        {
            sChannelBusyRatio = cb->ccaBusyRatio;
            sChannelBitmap = (u16)(1 << (channel - 1));
        }
        /* いままでの最低値と同値の場合もひとまず採用 */
        else if (sChannelBusyRatio == cb->ccaBusyRatio)
        {
            sChannelBitmap |= 1 << (channel - 1);
        }
        ++channel;
    }

    /* 使用可能なチャンネルを取得 */
    allowedChannel = WM_GetAllowedChannel();
    if (allowedChannel == 0x8000)
    {
        OS_TPanic("WM_GetAllowedChannel() failed!");
    }
    else if (allowedChannel == 0)
    {
        OS_TPanic("no available wireless channels!");
    }

    /* 全チャンネルを計測したら完了 */
    if ((1 << (channel - 1)) > allowedChannel)
    {
        int     num = MATH_CountPopulation(sChannelBitmap);
        if (num == 0)
        {
            OS_TPanic("no available wireless channels!");
        }
        else if (num == 1)
        {
            sChannel = (u16)(31 - MATH_CountLeadingZeros(sChannelBitmap) + 1);
        }
        /* 同じ電波使用率のチャンネルが複数存在した場合はランダムに選択 */
        else
        {
            int     select = (int)(((OS_GetVBlankCount() & 0xFF) * num) / 0x100);
            int     i;
            for (i = 0; i < 16; i++)
            {
                if ((sChannelBitmap & (1 << i)) && (--select < 0))
                {
                    break;
                }
            }
            sChannel = (u16)(i + 1);
        }
        sChannelBusy = FALSE;
    }
    /* 未計測のチャンネルがあれば処理を続行 */
    else
    {
        while (((1 << (channel - 1)) & allowedChannel) == 0)
        {
            ++channel;
        }
        if (WM_MeasureChannel(GetChannelProc, 3, 17, channel, 30) != WM_ERRCODE_OPERATING)
        {
            OS_TPanic("WM_MeasureChannel() failed!");
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         GetChannelMain

  Description:  電波使用率の最も低いチャンネルを計測する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetChannelMain(void)
{
    KeyInfo key[1];

    /* チャンネル計測用変数の初期化 */
    sChannelBusy = TRUE;
    sChannel = 0;
    sChannelBusyRatio = 100 + 1;
    sChannelBitmap = 0;

    /* WM ワークメモリの確保とライブラリ起動 */
    sWmWork = OS_Alloc(WM_SYSTEM_BUF_SIZE);
    if (!sWmWork)
    {
        OS_TPanic("failed to allocate memory for WM!");
    }
    else if (WM_Initialize(sWmWork, GetChannelProc, 2) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("WM_Initialize() failed!");
    }

    /* 計測完了まで待機 */
    InitFrameLoop(key);
    while (sChannelBusy)
    {
        WaitForNextFrame(key);
        PrintString(4, 22, COLOR_YELLOW, " Searching Channel %c",
                    ((OS_GetVBlankCount() / 10) & 1) ? '_' : ' ');
    }

#if !defined(MBP_USING_MB_EX)
    /* ライブラリ終了と WM ワークメモリの解放 */
    sChannelBusy = TRUE;
    if (WM_End(GetChannelProc) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("WM_End() failed!");
    }
    while (sChannelBusy)
    {
    }
    OS_Free(sWmWork), sWmWork = NULL;
#endif

}

/*---------------------------------------------------------------------------*
  Name:         ConnectMain

  Description:  マルチブートで接続する。

  Arguments:    tgid        親機として起動する場合のtgidを指定します.

  Returns:      子機への転送に成功した場合には TRUE,
                失敗したりキャンセルされた場合には  FALSE を返します。
 *---------------------------------------------------------------------------*/
static void ConnectMain(void)
{
    KeyInfo key[1];

    MBP_Init(mbGameList.ggid, WM_GetNextTgid());

    InitFrameLoop(key);
    for (;;)
    {
        int     state;

        WaitForNextFrame(key);
        PrintString(4, 22, COLOR_YELLOW, " MB Parent ");
        state = MBP_GetState();
        /* アイドル状態 */
        if (state == MBP_STATE_IDLE)
        {
            MBP_Start(&mbGameList, sChannel);
        }
        /* 子機からのエントリー受付中 */
        else if (state == MBP_STATE_ENTRY)
        {
            PrintString(4, 22, COLOR_WHITE, " Now Accepting            ");
            /* Bボタンでマルチブートキャンセル */
            if (key->trg & PAD_BUTTON_B)
            {
                MBP_Cancel();
            }
            /* エントリー中の子機が一台でも存在すれば開始可能とする */
            else if (MBP_GetChildBmp(MBP_BMPTYPE_ENTRY) ||
                     MBP_GetChildBmp(MBP_BMPTYPE_DOWNLOADING) ||
                     MBP_GetChildBmp(MBP_BMPTYPE_BOOTABLE))
            {
                PrintString(4, 22, COLOR_WHITE, " Push START Button to start   ");
                if (key->trg & PAD_BUTTON_START)
                {
                    /* ダウンロード開始 */
                    MBP_StartDownloadAll();
                }
            }
        }
        /*
         * プログラム配信処理.
         * 全員がダウンロード完了しているならば再起動要求.
         */
        else if (state == MBP_STATE_DATASENDING)
        {
            if (MBP_IsBootableAll())
            {
                MBP_StartRebootAll();
            }
        }
        /* 子機再起動の待機処理 */
        else if (state == MBP_STATE_REBOOTING)
        {
            PrintString(4, 22, COLOR_WHITE, " Rebooting now                ");
        }
        /* 子機再起動の完了時処理 */
        else if (state == MBP_STATE_COMPLETE)
        {
            PrintString(4, 22, COLOR_WHITE, " Reconnecting now             ");
            break;
        }
        /* エラー発生時は通信をキャンセル */
        else if (state == MBP_STATE_ERROR)
        {
            MBP_Cancel();
        }
        /* 通信キャンセル処理中 */
        else if (state == MBP_STATE_CANCEL)
        {
        }
        /* 通信異常終了 */
        else if (state == MBP_STATE_STOP)
        {
            break;
        }

        /* 子機状態を表示 */
        PrintChildState();
    }

#if defined(MBP_USING_MB_EX)
    /* ライブラリ終了と WM ワークメモリの解放 */
    sChannelBusy = TRUE;
    if (WM_End(GetChannelProc) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("WM_End() failed!");
    }
    while (sChannelBusy)
    {
    }
    OS_Free(sWmWork), sWmWork = NULL;
#endif

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
        PrintString(STATE_DISP_X, i + BASE_DISP_Y, COLOR_WHITE, STATE_NAME[childState]);

        // ユーザー情報表示
        childInfo = MBP_GetChildInfo(i);
        macAddr = MBP_GetChildMacAddress(i);

        if (macAddr != NULL)
        {
            PrintString(INFO_DISP_X, i + BASE_DISP_Y, COLOR_WHITE,
                        "%02x%02x%02x%02x%02x%02x",
                        macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
        }
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
