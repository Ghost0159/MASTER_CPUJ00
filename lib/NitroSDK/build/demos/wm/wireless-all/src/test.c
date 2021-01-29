/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     test.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: test.c,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include "common.h"


/*****************************************************************************/
/* variable */

/* 親機WFSテスト用変数 */
static BOOL is_parent_sync = FALSE;
static int parent_packet_size = WH_PARENT_MAX_SIZE;

/* 子機WFSテスト用変数 */
static u8 tmp_buf[1024 * 128];
static int file_index = -1;
static OSTick read_tick;
static int read_byte;
static int read_time;
static BOOL file_sync_mode;
static FSFile file[1];

static const char *file_path[] = {
    "test/16kB.bin",
    "test/64kB.bin",
};
static const int file_max = sizeof(file_path) / sizeof(*file_path);


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         BeginFileTest

  Description:  ファイルリードテスト開始

  Arguments:    None.

  Returns:      正常に非同期リードが開始したら TRUE.
 *---------------------------------------------------------------------------*/
static BOOL BeginFileTest(void)
{
    BOOL    retval = FALSE;
    if (FS_OpenFile(file, file_path[file_index]))
    {
        read_tick = OS_GetTick();
        read_byte = (int)FS_GetLength(file);
        read_time = 0;
        if (read_byte > (int)sizeof(tmp_buf))
        {
            read_byte = (int)sizeof(tmp_buf);
        }
        OS_TPrintf("reading... %d BYTE\n", read_byte);
        if (FS_ReadFileAsync(file, tmp_buf, sizeof(tmp_buf)))
        {
            retval = TRUE;
        }
    }
    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         EndFileTest

  Description:  ファイルリードテスト完了

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void EndFileTest(void)
{
    if (FS_IsSucceeded(file))
    {
        u64     milli = OS_TicksToMilliSeconds(OS_GetTick() - read_tick);
        if (!milli)
        {
            milli = 1;
        }
        read_time = (int)(((u64)read_byte * 8 * 1000) / milli);
    }
    else
    {
        read_time = -1;
    }
    (void)FS_CloseFile(file);
}

/*---------------------------------------------------------------------------*
  Name:         UpdateFrame

  Description:  フレーム更新処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void UpdateFrame(void)
{
    /* 親機側操作 */
    if (!MB_IsMultiBootChild())
    {
        /* 送信パケットサイズを切り替え */
        int     new_packet_size = parent_packet_size;
        if (IS_PAD_TRIGGER(PAD_KEY_UP))
        {
            new_packet_size += 2;
            if (new_packet_size > WH_PARENT_MAX_SIZE)
                new_packet_size = 16;
        }
        if (IS_PAD_TRIGGER(PAD_KEY_DOWN))
        {
            new_packet_size -= 2;
            if (new_packet_size < 16)
                new_packet_size = WH_PARENT_MAX_SIZE;
        }
        if (parent_packet_size != new_packet_size)
        {
            parent_packet_size = new_packet_size;
            WFS_SetPacketSize(parent_packet_size);
        }
        /*
         * A ボタンで同期モードへ移行.
         *
         * これにより, 同期指定した子機群のうち一台でも要求が届かないうちは
         * 先着の子機がアクセスをブロッキングされます.
         * 同じ組み合わせのファイルを同じ順序で要求されることが
         * 明らかである場合, この設定によって通信が効率化されます.
         *
         * この設定は, 子機群のアクセス要求が食い違った時点で
         * 警告文のデバッグ出力とともに自動でリセットされます.
         */
        if (IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            is_parent_sync = !is_parent_sync;
            WFS_EnableSync(is_parent_sync ? WFS_GetCurrentBitmap() : 0);
        }
    }
    /* 子機側操作 */
    else
    {
        /* 読み込むべきファイルを選択 */
        if (IS_PAD_TRIGGER(PAD_KEY_UP))
        {
            if (--file_index < 0)
            {
                file_index = file_max - 1;
            }
        }
        if (IS_PAD_TRIGGER(PAD_KEY_DOWN))
        {
            if (++file_index > file_max - 1)
            {
                file_index = 0;
            }
        }
        /* テストの同期/非同期モードを切り替え */
        if (IS_PAD_TRIGGER(PAD_KEY_LEFT) || IS_PAD_TRIGGER(PAD_KEY_RIGHT))
        {
            file_sync_mode = !file_sync_mode;
        }
        /* 非同期リードテスト中なら完了を確認 */
        if (FS_IsFile(file))
        {
            /* 完了したなら計測 */
            if (!FS_IsBusy(file))
            {
                EndFileTest();
            }
        }
        /* 可能なら A ボタンでファイルリードテスト */
        else if (IS_PAD_TRIGGER(PAD_BUTTON_A))
        {
            if (!BeginFileTest())
            {
                /* すでにワイヤレスが切断されている場合など */
                EndFileTest();
            }
            /*
             * 同期テストならここで完了を待機.
             * WFS_End 関数が呼ばれない限り同期処理は中止されませんので,
             * 切断検知へ対応する場合は以下のようにポーリングします.
             * スレッド移譲せずポーリングで CPU 時間を占有するという点を除き,
             * この処理は基本的に FS_ReadFile 関数と等価です.
             */
            else if (file_sync_mode)
            {
                while (FS_IsBusy(file))
                {
                    if (WH_GetSystemState() != WH_SYSSTATE_DATASHARING)
                    {
                        WFS_End();
                    }
#if defined(SDK_ENABLE_ARM7_PRINT)
                    // PrintServer for ARM7 (デバッグ目的)
                    OS_PrintServer();
#endif
                }
                EndFileTest();
            }
        }
    }
    /* START ボタンで通信終了 */
    if (IS_PAD_TRIGGER(PAD_BUTTON_START))
    {
        (void)WH_Finalize();
    }
}


/*---------------------------------------------------------------------------*
  Name:         DrawFrame

  Description:  フレーム描画処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DrawFrame(void)
{
    int     i;

    /* データシェアリング受信状況の表示 */
    PrintString(8, 1, 0x2, "data-sharing mode");
    PrintString(4, 5, 0x4, "Receive:");
    for (i = 0; i < WH_PLAYER_MAX; ++i)
    {
        GShareData *input = GetCurrentInput(i);
        if (input)
        {
            PrintString(4, (s16)(6 + i), 0x4,
                        "Player%02d: %04x-%04x", i, input->key, input->count & 0xFFFF);
        }
        else
        {
            PrintString(4, (s16)(6 + i), 0x7, "No child");
        }
    }

    PrintString(0, 11, PLTT_BLUE, "--------------------------------");

    /* WFS テストの表示 */
    if (!MB_IsMultiBootChild())
    {
        /* 親機側 */
        PrintString(3, 13, PLTT_GREEN, "<UP & DOWN key>");
        PrintString(3, 14, PLTT_WHITE, "parent packet ... %d BYTE", parent_packet_size);

        PrintString(3, 15, PLTT_GREEN, "<toggle A button>");
        PrintString(3, 16, PLTT_WHITE, "sync mode     ... %s",
                    is_parent_sync ? "ENABLE" : "DISABLE");

        PrintString(3, 17, PLTT_GREEN, "bitmap status");
        PrintString(9, 18, PLTT_BLUE, "+  idle");
        PrintString(9, 19, PLTT_BLUE, "*  busy");
        PrintString(9, 20, PLTT_BLUE, "!  sync-blocking");
        PrintString(9, 21, PLTT_BLUE, "-  not connected");

        PrintString(3, 19, PLTT_BLUE, "0123");
        {
            const int cur_bitmap = WFS_GetCurrentBitmap();
            const int busy_bitmap = WFS_GetBusyBitmap();
            const int sync_bitmap = WFS_GetSyncBitmap();
            for (i = 0; i < WH_PLAYER_MAX; ++i)
            {
                char    c;
                const int bit = (1 << i);
                if ((bit & busy_bitmap) != 0)
                    c = '*';
                else if ((bit & cur_bitmap) != 0)
                    c = '+';
                else
                    c = '-';
                PrintString(3 + i, 20, PLTT_WHITE, "%c", c);
                if ((bit & sync_bitmap) != 0)
                    PrintString(3 + i, 21, PLTT_WHITE, "!");
            }
        }
    }
    else
    {
        /* 子機側 */
        if (WFS_GetStatus() == WFS_STATE_READY)
        {
            /* 操作概略と現在の設定項目を表示 */
            PrintString(3, 13, PLTT_GREEN, "<LEFT & RIGHT key>");
            if (!file_sync_mode)
            {
                PrintString(3, 14, PLTT_WHITE, " sync ");
                PrintString(10, 14, PLTT_YELLOW, "[async]");
            }
            else
            {
                PrintString(3, 14, PLTT_YELLOW, "[sync]");
                PrintString(10, 14, PLTT_WHITE, " async ");
            }
            PrintString(3, 15, PLTT_GREEN, "<UP & DOWN key>");
            PrintString(3, 16 + file_index, PLTT_YELLOW, ">");
            for (i = 0; i < file_max; ++i)
            {
                PrintString(5, 16 + i, (i == file_index) ? PLTT_YELLOW : PLTT_WHITE, file_path[i]);
            }
            PrintString(3, 16 + file_max, PLTT_GREEN, "<press A button>");
            /* リードテスト失敗時の表示 */
            if (read_time == -1)
            {
                PrintString(3, 20, PLTT_WHITE, "read error!");
            }
            /* リードテスト成功時の表示 */
            else if (read_time != 0)
            {
                PrintString(3, 20, PLTT_WHITE, "read done %d BYTE", read_byte);
                PrintString(3, 21, PLTT_WHITE, "%d [bps]", read_time);
            }
            /* リードテスト中の表示 */
            else
            {
                int     current, total;
                if (WFS_GetCurrentDownloadProgress(&current, &total))
                {
                    PrintString(3, 20, PLTT_WHITE, "reading   %d BYTE", read_byte);
                    PrintString(3, 21, PLTT_WHITE, "%8d / %d", current, total);
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ExecuteDataSharing

  Description:  データシェアリングのメイン処理.
                バックグラウンドで WFS 処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ExecuteDataSharing(void)
{
    /* ワーク変数初期化 */
    file_index = 0;
    FS_InitFile(file);

    /* データシェアリング準備完了まで待機して WFS 起動 */
    WaitWHState(WH_SYSSTATE_DATASHARING);
    WFS_Start();

    /* 通信が続く限りフレーム処理 */
    while (WH_GetSystemState() == WH_SYSSTATE_DATASHARING)
    {
        if (WaitNextFrame())
        {
            /* データシェアリングが成功すれば処理を更新 */
            UpdateFrame();
        }
        DrawFrame();
    }

    /* MP 通信が終了したので WFS 終了 */
    WFS_End();
    WaitWHState(WH_SYSSTATE_IDLE);

}
