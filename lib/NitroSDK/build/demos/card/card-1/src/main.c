/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - demos - card-1
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.15  2007/08/22 08:31:07  yosizaki
  add CARD_BACKUP_TYPE_FLASH_64MBITS

  Revision 1.14  2007/02/23 01:30:07  yosizaki
  support CARD_BACKUP_TYPE_FLASH_16MBITS.

  Revision 1.13  2006/04/03 01:58:59  yosizaki
  add CARD_BACKUP_TYPE_FLASH_8MBITS.

  Revision 1.12  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.11  2005/11/21 10:18:58  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer の削除

  Revision 1.10  2005/10/20 07:47:05  yosizaki
  add CARD_BACKUP_TYPE_FLASH_4MBITS.

  Revision 1.9  2005/09/02 07:39:23  yosizaki
  add CARDResult codes.

  Revision 1.8  2005/07/05 00:58:03  yosizaki
  add FRAM.

  Revision 1.7  2005/06/15 09:33:16  yosizaki
  add CARD_BACKUP_TYPE_EEPROM_512KBITS.

  Revision 1.6  2005/04/06 11:17:13  yosizaki
  small fix about CARD_GetResultCode().

  Revision 1.5  2005/04/05 07:24:32  yosizaki
  change around CARD_IdentifyBackup().

  Revision 1.4  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.3  2005/02/18 08:02:11  yosizaki
  copyright の年を修正.

  Revision 1.2  2005/02/18 07:30:10  yosizaki
  fix around hidden warnings.

  Revision 1.1  2004/09/09 12:50:30  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/card.h>
#include <nitro/fs.h>


/* 画面描画用のルーチン */
#include "draw.h"


/* CARD ライブラリ用変数 ******************************************************/

/*
 * CARD バックアップのロック用 ID.
 * CARD ハードウェアリソースを FS ライブラリなどと排他使用するため
 * CARD_LockBackup() / CARD_UnlockBackup() で使用します.
 */
static u16 card_lock_id;

/*
 * CARD アクセス関数のエラー時の結果値.
 * CARD_GetResultCode() の返り値は CARD_UnlockBackup() 等で変更されるので,
 * アクセス処理とエラー処理が離れている場合にはこの点に注意してください.
 */
static CARDResult last_result = CARD_RESULT_SUCCESS;

/*
 * 書き込みテストが実行されたかのフラグ.
 */
static BOOL is_test_run;


/* サンプルデモ 関数 **********************************************************/

/*---------------------------------------------------------------------------*
  Name:         GetPadDown

  Description:  押下タイミングのキー入力を取得.

  Arguments:    None.

  Returns:      前回呼び出しから今回呼び出しまでの間に押下されたキーのフラグ.
 *---------------------------------------------------------------------------*/
static u32 GetPadDown(void)
{
    static u32 btn = 0;
    static u32 btn_bak = 0;

    btn_bak = btn;
    btn = PAD_Read();
    return (u32)(~btn_bak & btn);
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V ブランク コールバック.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitApp

  Description:  OS 等, アプリケーション基本部分の初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitApp(void)
{
    void   *tmp;
    OSHeapHandle hh;

    /* OS の初期化 */
    OS_Init();
    OS_InitThread();
    OS_InitTick();
    /* アリーナの初期化 */
    tmp = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tmp);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
        OS_Panic("ARM9: Fail to create heap...\n");
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
    /* 割り込みを有効に */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
}

/*---------------------------------------------------------------------------*
  Name:         GetCardResultString

  Description:  CARD 関数の結果値を文字列で取得.

  Arguments:    result           CARDResult 結果値.

  Returns:      結果値を説明する文字列へのポインタ.
 *---------------------------------------------------------------------------*/
static const char *GetCardResultString(CARDResult result)
{
    switch (result)
    {
    case CARD_RESULT_SUCCESS:
        return "success";
    case CARD_RESULT_FAILURE:
        return "failure";
    case CARD_RESULT_INVALID_PARAM:
        return "invalid param";
    case CARD_RESULT_UNSUPPORTED:
        return "unsupported";
    case CARD_RESULT_TIMEOUT:
        return "timeout";
    case CARD_RESULT_CANCELED:
        return "canceled";
    case CARD_RESULT_NO_RESPONSE:
        return "no response";
    case CARD_RESULT_ERROR:
        return "error";
    default:
        return "unknown error";
    }
}

/*---------------------------------------------------------------------------*
  Name:         SelectDevice

  Description:  デバイス選択画面.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SelectDevice(void)
{
    /* *INDENT-OFF* */
    static const struct
    {
        CARDBackupType type;
        const char *comment;
    }
    types_table[] =
    {
        { CARD_BACKUP_TYPE_EEPROM_4KBITS,   "EEPROM    4 kb"},
        { CARD_BACKUP_TYPE_EEPROM_64KBITS,  "EEPROM   64 kb"},
        { CARD_BACKUP_TYPE_EEPROM_512KBITS, "EEPROM  512 kb"},
        { CARD_BACKUP_TYPE_FLASH_2MBITS,    "FLASH     2 Mb"},
        { CARD_BACKUP_TYPE_FLASH_4MBITS,    "FLASH     4 Mb"},
        { CARD_BACKUP_TYPE_FLASH_8MBITS,    "FLASH     8 Mb"},
        { CARD_BACKUP_TYPE_FLASH_16MBITS,   "FLASH    16 Mb"},
        { CARD_BACKUP_TYPE_FLASH_64MBITS,   "FLASH    64 Mb"},
        { CARD_BACKUP_TYPE_FRAM_256KBITS,   "FRAM    256 kb"},
    };
    /* *INDENT-ON* */
    enum
    { types_table_max = sizeof(types_table) / sizeof(*types_table) };

    int     cur = 0;
    BOOL    error = FALSE;
    BOOL    end = FALSE;
    int     i;

    while (!end)
    {

        u32     btn_down = GetPadDown();

        DrawBegin();
        ClearFrame(RGB555_BLACK);

        /* 上下キーでカーソル移動 */
        if (btn_down & PAD_KEY_DOWN)
        {
            error = FALSE;
            if (++cur >= types_table_max)
                cur -= types_table_max;
        }
        if (btn_down & PAD_KEY_UP)
        {
            error = FALSE;
            if (--cur < 0)
                cur += types_table_max;
        }

        /*
         * A ボタンで現在の選択デバイスに特定.
         *
         * ライブラリは指定のデバイスが実際に搭載されているか
         * 判断できませんので, 正しく指定してください.
         */
        if (btn_down & PAD_BUTTON_A)
        {
            CARD_LockBackup((u16)card_lock_id);
            end = CARD_IdentifyBackup(types_table[cur].type);
            if (!end)
            {
                error = TRUE;
                last_result = CARD_GetResultCode();
            }
            CARD_UnlockBackup(card_lock_id);
        }

        /* 文字列表示 */
        SetTextColor(RGB555_GREEN);
        DrawText(10, 40, "select device!");
        for (i = 0; i < types_table_max; ++i)
        {
            DrawText(20, 60 + 10 * i, types_table[i].comment);
        }
        FillRect(10, 60, 10, i * 10, RGB555_BLACK);
        DrawText(10, 60 + 10 * cur, ">");
        if (error)
        {
            /* エラー表示 */
            SetTextColor(RGB555_RED);
            DrawText(10, 160, "error!");
            DrawText(10, 170, "result:\"%s\"", GetCardResultString(last_result));
        }
        DrawEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         TestWriteAndVerify

  Description:  書き込みテスト画面.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void TestWriteAndVerify(void)
{
    const u32 page_size = CARD_GetBackupPageSize();
    const u32 sector_size = CARD_GetBackupSectorSize();
    const u32 total_size = CARD_GetBackupTotalSize();

    OSTick  erase_time = 0;
    u32     pos = 0;

    BOOL    end = FALSE;

    /* 各種情報を表示 */
    DrawBegin();
    ClearFrame(RGB555_BLACK);
    SetTextColor(RGB555_WHITE);
    if (CARD_IsBackupEeprom())
    {
        DrawText(10, 10, "EEPROM");
    }
    else if (CARD_IsBackupFlash())
    {
        DrawText(10, 10, "FLASH");
    }
    else if (CARD_IsBackupFram())
    {
        DrawText(10, 10, "FRAM");
    }
    DrawText(10, 20, "page:%d  sector:%d", page_size, sector_size);
    DrawText(10, 30, "total:%d", total_size);
    FillRect(10, 50, 256, 100, RGB555_BLACK);
    SetTextColor(RGB555_BLUE);
    MoveTo(0 + 8, 45);
    LineTo(256 - 8, 45);
    LineTo(256 - 8, 182 - 8);
    LineTo(0 + 8, 182 - 8);
    LineTo(0 + 8, 45);
    DrawEnd();

    while (!end)
    {

        u32     btn_down = GetPadDown();

        DrawBegin();

        /* 開始前なら A ボタンで開始 */
        if (!is_test_run)
        {
            if (btn_down & PAD_BUTTON_A)
            {
                is_test_run = TRUE;
            }
            SetTextColor(RGB555_WHITE);
            DrawText(10, 50, "press A-BUTTON to test");
        }

        /* B ボタンでテスト中止 */
        if (btn_down & PAD_BUTTON_B)
        {
            end = TRUE;
        }

        /* テスト中 */
        if (is_test_run)
        {

            static u8 tmp_buf[65536];
            SDK_ASSERT(sizeof(tmp_buf) >= sector_size);

            FillRect(10, 50, 256 - 10 * 2, 100, RGB555_BLACK);

            SetTextColor(RGB555_GREEN);
            DrawText(10, 50, "now testing...");
            DrawText(10, 60, "address:%d-%d", pos, pos + page_size);

            /*
             * 以降のバックアップアクセスのために CARD リソースをロックする.
             *
             * これより CARD_UnlockBackup() を呼ぶまでの間
             * FS など他のモジュールからの CARD アクセスは
             * ブロッキングされることになります.
             * 処理のデッドロックに注意してください.
             */
            CARD_LockBackup((u16)card_lock_id);
            {
                OSTick  tick;

                int     i;

                for (i = 0; i < page_size; ++i)
                {
                    tmp_buf[i] = (u8)(pos * 3 + i);
                }

                tick = OS_GetTick();

                /* 非同期で書き込み + 内容確認判定 */
                if (CARD_IsBackupEeprom())
                {
                    CARD_WriteAndVerifyEepromAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                }
                else if (CARD_IsBackupFram())
                {
                    CARD_WriteAndVerifyFramAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                }
                else if (CARD_IsBackupFlash())
                {
                    // ほとんどのFLASHデバイスはWrite操作を使用可能です
                    if (CARD_GetCurrentBackupType() != CARD_BACKUP_TYPE_FLASH_64MBITS)
                    {
                        CARD_WriteAndVerifyFlashAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                    }
                    // 一部の大容量FLASHはEraseSector操作とProgram操作のみ使用可能です。
                    // 消去操作はセクタサイズの整数倍単位のみという点に注意してください。
                    else
                    {
                        BOOL    programmable = FALSE;
                        if ((pos % sector_size) == 0)
                        {
                            // これから書き込む領域は事前にセクタごと消去する必要があります。
                            // このサンプルデモではセクタ全体の内容を書き換えていますが、
                            // セクタの一部分のみを書き換えたい場合には消去前の内容を退避して
                            // 消去後に再び同じ内容を書き込みなおす必要があります。
                            erase_time = tick;
                            programmable = CARD_EraseFlashSector(pos, sector_size);
                            tick = OS_GetTick();
                            erase_time = tick - erase_time;
                            last_result = CARD_GetResultCode();
                        }
                        else
                        {
                            // すでに消去済みの領域はそのまま書き込み可能です。
                            programmable = TRUE;
                        }
                        if (programmable)
                        {
                            CARD_ProgramAndVerifyFlashAsync(pos, tmp_buf, page_size, NULL, (void *)0);
                        }
                    }
                }

                /*
                 * この場で完了を待つ.
                 *
                 * デバイス種類と指定サイズによっては
                 * 非常に長い時間ブロッキングされますので,
                 * 各ユーザアプリケーションのフレームワークに合う形で
                 * 待ち方を工夫してください.
                 * (1 フレームに 1 回 CARD_TryWaitBackupAsync() で判定する, 等)
                 */
                (void)CARD_WaitBackupAsync();
                last_result = CARD_GetResultCode();

                if (last_result != CARD_RESULT_SUCCESS)
                {
                    /* エラーならひとまず終了 */
                    end = TRUE;
                }
                else
                {
                    /* 成功なら時間を表示 */
                    tick = OS_GetTick() - tick;
                    DrawText(10, 70, "write:%6d[ms]/%d[BYTE]", (int)OS_TicksToMilliSeconds(tick), page_size);
                    if (erase_time != 0)
                    {
                        DrawText(10, 80, "erase:%6d[ms]/%d[BYTE]", (int)OS_TicksToMilliSeconds(erase_time), sector_size);
                    }
                    /* テストアドレスを次へ移動 */
                    pos += page_size;
                    if (pos + page_size > total_size)
                    {
                        end = TRUE;
                    }
                }
            }
            CARD_UnlockBackup(card_lock_id);

        }

        DrawEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ShowResult

  Description:  テスト結果表示画面.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ShowResult(void)
{

    BOOL    end = FALSE;

    while (!end)
    {

        u32     btn_down = GetPadDown();

        DrawBegin();

        /* A / B / START ボタンで戻る */
        if (btn_down & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))
        {
            end = TRUE;
        }

        if (last_result == CARD_RESULT_SUCCESS)
        {
            /* 成功表示 */
            SetTextColor(RGB555_WHITE);
            DrawText(10, 100, "done! (success)");
        }
        else
        {
            /* エラー表示 */
            SetTextColor(RGB555_RED);
            DrawText(10, 100, "error!");
            DrawText(10, 110, "result:\"%s\"", GetCardResultString(last_result));
        }

        DrawEnd();
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン エントリポイント.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{

    {
        /* サンプルアプリケーションの初期化 */
        InitApp();
        InitDraw();
        SetClearColor(RGB555_CLEAR);
    }

    {
        /* CARD ライブラリの初期化 */
        s32     ret;

        CARD_Init();
        ret = OS_GetLockID();
        if (ret == OS_LOCK_ID_ERROR)
        {
            OS_Panic("demo fatal error! OS_GetLockID() failed");
        }
        card_lock_id = (u16)ret;
    }

    /* 画面遷移 */
    for (;;)
    {
        /* デバイス選択画面 */
        SelectDevice();
        /* テスト開始 */
        is_test_run = FALSE;
        TestWriteAndVerify();
        /* 結果表示 */
        if (is_test_run)
            ShowResult();
    }

}
