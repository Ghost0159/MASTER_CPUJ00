/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - demos - wbt-fs
  File:     child.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: child.c,v $
  Revision 1.7  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/14 09:35:46  yosizaki
  change about FS_RedFile().

  Revision 1.5  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.4  2005/02/18 09:26:34  yosizaki
  fix around hidden warnings.

  Revision 1.3  2004/11/16 06:36:22  yosizaki
  change to use FS_LoadOverlayImageAsync.

  Revision 1.2  2004/10/28 13:10:16  yosizaki
  fix comments.

  Revision 1.1  2004/10/25 10:19:00  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/wbt.h>
#include <nitro/fs.h>

#include    "wfs.h"

#include    "util.h"
#include    "wc.h"
#include    "func.h"
#include    "common.h"


/*---------------------------------------------------------------------------*
    内部定数定義
 *---------------------------------------------------------------------------*/

/* オーバーレイテスト用配列 */
FS_EXTERN_OVERLAY(main_overlay_1);
FS_EXTERN_OVERLAY(main_overlay_2);
FS_EXTERN_OVERLAY(main_overlay_3);

static FSOverlayID ovl_id[] = {
    FS_OVERLAY_ID(main_overlay_1),
    FS_OVERLAY_ID(main_overlay_2),
    FS_OVERLAY_ID(main_overlay_3),
};

enum
{ OVL_ID_MAX = sizeof(ovl_id) / sizeof(*ovl_id) };
typedef void (*FUNC_TABLE) (char *);

const FUNC_TABLE func_table[] = {
    func_1,
    func_2,
    func_3,
};


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/

/* ファイル操作用 */
static FSFile file[1];
static char tmp_buf[0x40000];

static int file_index;
static int file_max;
static char file_path[8][256];

/* オーバーレイテスト管理変数 */
static BOOL is_overlay_test = FALSE;
static BOOL is_overlay_loaded = FALSE;
static int current_overlay = 0;
static FSOverlayInfo ovl_info[1];


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/

static void ModeChildFile(const KeyInfo * p_key);       /* 子機 ファイルテスト */
static void ModeChildOverlay(const KeyInfo * p_key);    /* 子機 オーバーレイテスト */


/*---------------------------------------------------------------------------*
  Name:         CreateFileList

  Description:  ディレクトリからファイルを列挙します.

  Arguments:    pe         再帰的に共有する FSDirEntry 構造体

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void CreateFileList(FSDirEntry *pe)
{
    FSFile  d;
    FS_InitFile(&d);
    if (FS_SeekDir(&d, &pe->dir_id))
    {
        while (FS_ReadDir(&d, pe))
        {
            if (file_max >= sizeof(file_path) / sizeof(*file_path))
                break;
            if (pe->is_directory)
                CreateFileList(pe);
            else
            {
                FSFile  f;
                FS_InitFile(&f);
                if (FS_OpenFileFast(&f, pe->file_id))
                {
                    if (FS_GetPathLength(&f) <= (int)sizeof(*file_path))
                    {
                        if (FS_GetPathName(&f, file_path[file_max], sizeof(*file_path)))
                        {
                            ++file_max;
                        }
                    }
                    (void)FS_CloseFile(&f);
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrepareOverlay

  Description:  現在選択しているオーバーレイをロードします.
                ただし非同期で読み込む段階まで進め,
                FS_StartOverlay() は呼ばれません.

  Arguments:    new_overlay  新しくロードするオーバーレイのインデックス

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrepareOverlay(int new_overlay)
{
    /*
     * オーバーレイを読み込む際には,
     * 領域を競合する他の全てのオーバーレイを
     * アンロードする必要があります.
     */
    if (is_overlay_loaded)
    {
        is_overlay_loaded = FALSE;
        (void)FS_UnloadOverlayImage(ovl_info);
    }

    current_overlay = new_overlay;

    /*
     * まずオーバーレイ情報を取得します.
     * テーブルは WFS 経由で受信済みですので,
     * この関数はただちに制御を返します.
     */
    (void)FS_LoadOverlayInfo(ovl_info, MI_PROCESSOR_ARM9, ovl_id[current_overlay]);
    {
        /*
         * そしてオーバーレイモジュール領域に実データを読み込みます.
         * これらは単に FS_LoadOverlay() の呼び出しでも可能ですが,
         * 無線経由での同期処理になるため非常に長い時間がかかります.
         *
         * 待ち時間にもアプリケーションフレームワークが駆動するには,
         * 以下のようにFS_LoadOverlayImageAsync()を採用するかまたは
         * FS_LoadOverlay() を呼び出す専用スレッドを用意する必要があります.
         */
        if (!FS_LoadOverlayImageAsync(ovl_info, file))
        {
            OS_Panic("FS_LoadOverlayImageAsync() failed!");
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  子機 通信画面。

  Arguments:    key         キー入力.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ModeChild(const KeyInfo * p_key)
{
    static BOOL init_flag = FALSE;
    if (!init_flag)
    {
        init_flag = TRUE;
        /* ファイルテスト用オブジェクトの初期化 */
        FS_InitFile(file);
    }
    {
        BOOL    bak_state = is_overlay_test;
        if (is_overlay_test)
            ModeChildOverlay(p_key);
        else
            ModeChildFile(p_key);
        if (bak_state != is_overlay_test)
        {
            ClearLine();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChildFile

  Description:  子機 通信画面でのファイル読み込みテスト処理。

  Arguments:    key         キー入力.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChildFile(const KeyInfo * p_key)
{
    int     i;

    PrintString(8, 1, COLOR_WHITE, "Child mode");
    PrintString(2, 3, COLOR_WHITE, "ReadFile test.");
    PrintString(2, 19, COLOR_WHITE, "Press B to move Overlay test.");
    PrintString(2, 20, COLOR_WHITE, "Press START to disconnect.");

    if (WFS_GetStatus() == WFS_STATE_READY)
    {

        /* ファイルリストを生成 */
        if (file_max == 0)
        {
            FSFile  d;
            FSDirEntry e;
            FS_InitFile(&d);
            (void)FS_FindDir(&d, "");
            MI_WriteByte(e.name + 0, (u8)'\0');
            (void)FS_TellDir(&d, &e.dir_id);
            CreateFileList(&e);
        }

        /* 読み込むべきファイル ID を選択 */
        if (p_key->trg & PAD_KEY_UP)
        {
            if (--file_index < 0)
            {
                file_index = file_max - 1;
            }
        }
        if (p_key->trg & PAD_KEY_DOWN)
        {
            if (++file_index > file_max - 1)
            {
                file_index = 0;
            }
        }

        PrintString(3, 5, COLOR_GREEN, "<UP & DOWN key>");
        PrintString(3, 6, COLOR_WHITE, "select testing file");
        PrintString(2, 7 + file_index, COLOR_WHITE, ">");
        for (i = 0; i < file_max; ++i)
        {
            PrintString(4, 7 + i, COLOR_WHITE, file_path[i]);
        }
        PrintString(3, 15, COLOR_GREEN, "<press A button>");

        /* ファイルリードが空きの状態 */
        if (!FS_IsFile(file))
        {

            /* B ボタンでオーバーレイテストへ */
            if (p_key->trg & PAD_BUTTON_B)
            {
                is_overlay_test = TRUE;
                PrepareOverlay(current_overlay);
            }

            /* A ボタンで選択ファイルをリード */
            else if (p_key->trg & PAD_BUTTON_A)
            {

                if (!FS_OpenFile(file, file_path[file_index]))
                {
                    file_index = 0;
                    OS_TPrintf("No file:\"%s\"\n", file_path[file_index]);
                }
                else
                {
                    /*
                     * ファイルを読みにいきます.
                     *
                     * 非同期モードにすると即座に処理を返しますが,
                     * このサンプルでは実際の転送速度を取得するために
                     * この場で同期処理を行います.
                     * 実際のゲームアプリケーションにおいて
                     * メインスレッドからこのように呼び出すと
                     * 非常に長い間ゲームシーケンスがブロッキングされ
                     * フレームワーク全体に悪影響が出ることに注意してください.
                     *
                     * 非同期モードでどのように読み込むべきかは,
                     * オーバーレイのロード処理部分を参照ください.
                     */
                    int     ret;
                    int     bps = 0;
                    int     rest;
                    u64     milli;
                    OSTick  t;
                    t = OS_GetTick();
                    rest = (int)(FS_GetLength(file) - FS_GetPosition(file));

                    PrintString(3, 16, COLOR_WHITE, "reading... %d BYTE", rest);

                    /*
                     * WFS_End 関数が呼ばれない限り同期処理は中止されませんので,
                     * 同期版で切断検知へ対応する場合は以下のようにポーリングします.
                     * スレッド移譲せずポーリングで CPU 時間を占有するという点を除き,
                     * この処理は基本的に FS_ReadFile 関数と等価です.
                     *
                     * 同期処理とは別のスレッドで定期的に切断検知をしている場合なら
                     * このような処理は必要なく, FS_ReadFile 関数を直接使用可能です.
                     * (WFS_End 関数を割り込みハンドラから呼び出すことが
                     *  できないという点に注意してください)
                     */
                    {
                        ret = FS_ReadFileAsync(file, tmp_buf, sizeof(tmp_buf));
                        while (FS_IsBusy(file))
                        {
                            if (WcGetStatus() == WC_STATUS_READY)
                            {
                                WFS_End();
                                ret = -1;
                            }
                        }
                    }
                    /*
                     * サンプルでは中断時は単に Panic としていますが,
                     * アプリケーションで復帰処理に移ることも可能です.
                     * (FS_GetResultCode 関数は FS_RESULT_ERROR を返します)
                     */
                    if (ret != rest)
                    {
                        OS_Panic("FS_ReadFileAsync() returned invalid length!");
                    }
                    milli = OS_TicksToMilliSeconds(OS_GetTick() - t);
                    if (milli)
                    {
                        u64     d = ret * 8;
                        d *= 1000;
                        bps = (int)(d / milli);
                        PrintLine(3, 16, "read done %d BYTE", ret);
                        PrintLine(3, 17, "%d [bps]/%d [Bps]", bps, bps / 8);
                    }
                    (void)FS_CloseFile(file);
                }
            }
        }
    }

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChildOverlay

  Description:  子機 通信画面でのファイルオーバーレイテスト処理。

  Arguments:    key         キー入力.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChildOverlay(const KeyInfo * p_key)
{
    PrintString(8, 1, COLOR_WHITE, "Child mode");
    PrintString(2, 3, COLOR_WHITE, "Overlay test.");
    PrintString(2, 19, COLOR_WHITE, "Press B to move ReadFile test.");
    PrintString(2, 20, COLOR_WHITE, "Press START to disconnect.");

    if (WFS_GetStatus() == WFS_STATE_READY)
    {

        PrintString(3, 5, COLOR_GREEN, "<UP & DOWN key>");
        PrintString(3, 6, COLOR_WHITE, "select overlay");
        PrintString(18, 6 + current_overlay, COLOR_WHITE, ">");
        PrintString(20, 6, COLOR_WHITE, "func_1");
        PrintString(20, 7, COLOR_WHITE, "func_2");
        PrintString(20, 8, COLOR_WHITE, "func_3");

        PrintString(3, 10, COLOR_GREEN, "<press A button>");
        PrintString(3, 11, COLOR_BLUE, "(call overlay func)");

        /* ファイルリードが空きの状態ならオーバーレイ操作可能 */
        if (!FS_IsFile(file))
        {

            /* B ボタンで ReadFile テストへ */
            if (p_key->trg & PAD_BUTTON_B)
            {
                is_overlay_test = FALSE;
            }
            else
            {
                /* 上下でオーバーレイ切り替え */
                int     new_overlay = current_overlay;

                if (p_key->trg & PAD_KEY_DOWN)
                {
                    if (++new_overlay >= OVL_ID_MAX)
                        new_overlay -= OVL_ID_MAX;
                }
                if (p_key->trg & PAD_KEY_UP)
                {
                    if (--new_overlay < 0)
                        new_overlay += OVL_ID_MAX;
                }
                if (current_overlay != new_overlay)
                {
                    PrepareOverlay(new_overlay);
                }
                /* A ボタンで現在の関数をコール */
                else if (p_key->trg & PAD_BUTTON_A)
                {
                    char    tmp[64];
                    (*(func_table[current_overlay])) (tmp);
                    PrintLine(3, 11, tmp);
                }
            }
        }
        else
        {
            /* オーバーレイ受信中なら進捗を表示 */
            if (FS_IsBusy(file))
            {
                int     current, total;
                PrintLine(2, 14, "loading...%8d BYTE", FS_GetLength(file));
                if (WFS_GetCurrentDownloadProgress(&current, &total))
                {
                    PrintLine(2, 15, "progress %8d / %8d", current, total);
                }
            }
            /* リード完了ならオーバーレイ起動 */
            else
            {
                if (!is_overlay_loaded)
                {
                    PrintLine(2, 14, "loading done. %8d BYTE", FS_GetLength(file));
                    PrintLine(2, 15, "");
                    FS_StartOverlay(ovl_info);
                    (void)FS_CloseFile(file);
                    is_overlay_loaded = TRUE;
                }
            }
        }
    }

    if (p_key->trg & PAD_BUTTON_START)
    {
        WcEnd();
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
