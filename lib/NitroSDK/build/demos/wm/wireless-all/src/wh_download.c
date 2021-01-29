/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     wh_download.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wh_download.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"


/*****************************************************************************/
/* constant */

/* ダウンロードプログラム情報 (クローンブート) */
/* *INDENT-OFF* */
const MBGameRegistry mbGameList =
{
    NULL,
    (u16 *)L"wireless-all",
    (u16 *)L"all of the wireless demo(cloneboot, dataSharing, WFS)",
    "/icon.char", "/icon.plt",
    WH_GGID,
    WH_CHILD_MAX + 1,
};
/* *INDENT-ON* */


/*****************************************************************************/
/* function */

/*****************************************************************************/
/* 親機専用領域 .parent セクションの定義範囲を開始します */
#include <nitro/parent_begin.h>
/*****************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         ConnectMain

  Description:  マルチブートで接続する。

  Arguments:    None.

  Returns:      子機への転送に成功した場合には TRUE,
                失敗したりキャンセルされた場合には  FALSE を返します。
 *---------------------------------------------------------------------------*/
static BOOL ConnectMain(void)
{
    for (;;)
    {
        /* フレーム待ち */
        (void)WaitNextFrame();

        PrintString(4, 22, PLTT_YELLOW, " MB Parent ");

        switch (MBP_GetState())
        {
            //-----------------------------------------
            // 子機からのエントリー受付中
        case MBP_STATE_ENTRY:
            {
                PrintString(4, 22, PLTT_WHITE, " Now Accepting            ");

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
                    PrintString(4, 22, PLTT_WHITE, " Push START Button to start   ");

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
                PrintString(4, 22, PLTT_WHITE, " Rebooting now                ");
            }
            break;

            //-----------------------------------------
            // 再接続処理
        case MBP_STATE_COMPLETE:
            {
                // 全員無事に接続完了したらマルチブート処理は終了し
                // 通常の親機として無線を再起動する。
                PrintString(4, 22, PLTT_WHITE, " Reconnecting now             ");

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
            OS_WaitVBlankIntr();
            return FALSE;
        }

        // 子機状態を表示する
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
            for (i = 1; i < WH_PLAYER_MAX; i++)
            {
                const MBPChildInfo *childInfo;
                MBPChildState childState = MBP_GetChildState(i);
                const u8 *macAddr;
                // 状態表示
                PrintString(STATE_DISP_X, i + BASE_DISP_Y, PLTT_WHITE, STATE_NAME[childState]);
                // ユーザー情報表示
                childInfo = MBP_GetChildInfo(i);
                macAddr = MBP_GetChildMacAddress(i);
                if (macAddr)
                {
                    PrintString(INFO_DISP_X, i + BASE_DISP_Y, PLTT_WHITE,
                                "%02x%02x%02x%02x%02x%02x",
                                macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4],
                                macAddr[5]);
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ExecuteDownloadServer

  Description:  DSダウンロードプレイ親機処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ExecuteDownloadServer(void)
{
    WH_SetGgid(WH_GGID);

    for (;;)
    {
        /* ワイヤレスの初期化 */
        (void)WH_Initialize();
        WaitWHState(WH_SYSSTATE_IDLE);
        while (!IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            (void)WaitNextFrame();
            PrintString(4, 22, PLTT_WHITE, " Press A Button to start   ");
        }
        /* トラフィックの少ないチャンネルの検索処理 */
        SetCurrentChannel(MeasureChannel());

        /* DS ダウンロードプレイ開始 */
#if !defined(MBP_USING_MB_EX)
        (void)WH_End();
        WaitWHState(WH_SYSSTATE_STOP);
#endif
        MBP_Init(WH_GGID, WM_GetNextTgid());
        MBP_Start(&mbGameList, GetCurrentChannel());

        if (ConnectMain())
        {
            /* マルチブート子機の起動に成功 */
            break;
        }
        else
        {
            /* WH モジュールを終了させて繰り返し */
            WH_Finalize();
            (void)WH_End();
        }
    }

}


/*****************************************************************************/
/* 親機専用領域 .parent セクションの定義範囲を終了します */
#include <nitro/parent_end.h>
/*****************************************************************************/
