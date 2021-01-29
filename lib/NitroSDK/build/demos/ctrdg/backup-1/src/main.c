/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CTRDG - demos - backup-1
  File:     main.c

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.2  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.1  2006/04/07 03:30:40  okubata_ryoma
  backup-1サンプルデモの追加

  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

/* 画面描画用のルーチン */
#include "draw.h"

/* ****************************************************************************/
static  async_task_busy = FALSE;       // 別のタスクが実行中ならTRUE

// グローバルなココにデータを入れてコマンドに送る
CTRDGTaskInfo CTRDG_TaskData;

u32     TaskBusyCallback(CTRDGTaskInfo * p);
u32     ReadCallback(CTRDGTaskInfo * p);
//---------------------------------------------------------------------------
static void InitBackupAccess(void);
static void TestBackupAccess(void);
static void SelectDevice(void);

u16     sec_num = 1;                   // 0～16
u8      dst_buf[4096];

// key information
u16     Trg;
u16     Cont;
u16     keyData;

BOOL    sram_flag = FALSE;             // SRAMならTRUE
BOOL    flash512k_flag = FALSE;        // FLASH512KならTRUE
BOOL    flash1m_flag = FALSE;          // FLASH1MならTRUE
BOOL    nocommond_flag = FALSE;        // 指定したコマンドがそのデバイスになければTRUE
BOOL    async_flag = FALSE;            // 非同期モードであればTRUE
/* サンプルデモ 関数 **********************************************************/
u32 TaskBusyCallback(CTRDGTaskInfo * p)
{
    async_task_busy = FALSE;
    if (p->result != 0)
        OS_TPrintf("error\n");
    return 1;
}

u32 ReadCallback(CTRDGTaskInfo * p)
{
    (void)p;
    async_task_busy = FALSE;
    OS_TPrintf("Read : sector%d = %s\n", sec_num, dst_buf);
    ClearFrame(RGB555_BLACK);
    return 1;
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
  Name:         NitroMain

  Description:  メイン エントリポイント.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* サンプルアプリケーションの初期化 */
    InitApp();

    InitBackupAccess();

    OS_TPrintf("start test\n");

    TestBackupAccess();

}

void InitBackupAccess(void)
{
    CTRDG_Init();
    CTRDG_Enable(TRUE);
    InitDraw();

    /* デバイス識別および初期設定 */
    SelectDevice();
}

void TestBackupAccess(void)
{
    u32     result = TRUE;
    u32     offset = 0;
    u8      src_buf[4096];
    u8      src_buf2[4096];
    u32     size = 4096;
    u16     i;

    MI_CpuClear8(dst_buf, size);
    MI_CpuClear8(src_buf, size);
    MI_CpuClear8(src_buf2, size);
    MI_CpuCopy8("Write Sector", src_buf, 13);
    MI_CpuCopy8("Write & Verify", src_buf2, 14);

    if (sram_flag)
        offset = CTRDG_AGB_SRAM_ADR;
    ClearFrame(RGB555_BLACK);

    while (1)
    {
        DrawBegin();
        //---- read pad data
        keyData = PAD_Read();
        keyData = PAD_Read();
        Trg = (u16)(keyData & (keyData ^ Cont));
        Cont = keyData;

        /* 文字列表示 */
        DrawText(10, 40, "A : Write");
        DrawText(10, 50, "B : Erase");
        DrawText(10, 60, "X : Read");
        DrawText(10, 70, "Y : Write & Verify");
        DrawText(10, 80, "UP    : Add SectorNo or offset");
        DrawText(10, 90, "DOWN  : Sub SectorNo or offset");
        DrawText(10, 100, "START : change Async/Sync Mode");

        if (!async_task_busy)
        {
            // AでWrite
            if (Trg == PAD_BUTTON_A)
            {
                if (async_flag)
                {
                    async_task_busy = TRUE;
                    if (!sram_flag)
                    {
                        CTRDG_WriteAgbFlashSectorAsync(sec_num, src_buf, TaskBusyCallback);
                    }
                    else
                    {
                        CTRDG_WriteAgbSramAsync(offset, src_buf, size, TaskBusyCallback);
                    }
                }
                else
                {
                    if (!sram_flag)
                    {
                        result = CTRDG_WriteAgbFlashSector(sec_num, src_buf);
                        if (result != 0)
                            OS_TPrintf("error : Write\n");
                    }
                    else
                    {
                        CTRDG_WriteAgbSram(offset, src_buf, size);
                    }
                }
                OS_TPrintf("Write sector%d\n", sec_num);
            }
            // BでErase
            else if (Trg == PAD_BUTTON_B)
            {
                if (async_flag)
                {
                    async_task_busy = TRUE;
                    if (!sram_flag)
                    {
                        CTRDG_EraseAgbFlashSectorAsync(sec_num, TaskBusyCallback);
                    }
                    else
                    {
                        nocommond_flag = TRUE;
                        ClearFrame(RGB555_BLACK);
                        async_task_busy = FALSE;
                    }
                }
                else
                {
                    if (!sram_flag)
                    {
                        result = CTRDG_EraseAgbFlashSector(sec_num);
                    }
                    else
                    {
                        nocommond_flag = TRUE;
                        ClearFrame(RGB555_BLACK);
                        result = 0;
                    }
                    if (result != 0)
                        OS_TPrintf("error : Erase\n");
                }
                OS_TPrintf("Erase : sector%d\n", sec_num);
            }
            // XでRead
            else if (Trg == PAD_BUTTON_X)
            {
                nocommond_flag = FALSE;
                if (async_flag)
                {
                    async_task_busy = TRUE;
                    if (!sram_flag)
                    {
                        CTRDG_ReadAgbFlashAsync(sec_num, offset, dst_buf, size, ReadCallback);
                    }
                    else
                    {
                        CTRDG_ReadAgbSramAsync(offset, (void *)dst_buf, size, ReadCallback);
                    }
                }
                else
                {
                    if (!sram_flag)
                    {
                        CTRDG_ReadAgbFlash(sec_num, offset, dst_buf, size);
                    }
                    else
                    {
                        CTRDG_ReadAgbSram(offset, (void *)dst_buf, size);
                    }
                    OS_TPrintf("Read : sector%d = %s\n", sec_num, dst_buf);
                    ClearFrame(RGB555_BLACK);
                }
            }
            // YでWriteAndVerify
            else if (Trg == PAD_BUTTON_Y)
            {
                if (async_flag)
                {
                    async_task_busy = TRUE;
                    if (!sram_flag)
                    {
                        CTRDG_WriteAndVerifyAgbFlashAsync(sec_num, src_buf2, size,
                                                          TaskBusyCallback);
                    }
                    else
                    {
                        CTRDG_WriteAndVerifyAgbSramAsync(offset, src_buf2, size, TaskBusyCallback);
                    }
                }
                else
                {
                    if (!sram_flag)
                    {
                        result = CTRDG_WriteAndVerifyAgbFlash(sec_num, src_buf2, size);
                    }
                    else
                    {
                        result = CTRDG_WriteAndVerifyAgbSram(offset, src_buf2, size);
                    }
                    if (result != 0)
                        OS_TPrintf("error : WriteAndVerify\n");
                }
                OS_TPrintf("Write sector%d\n", sec_num);
            }
            // 上キーでアクセスするSectorNoもしくはoffsetを増やす
            else if (Trg == PAD_KEY_UP)
            {
                if (flash512k_flag)
                {
                    if (sec_num < 15)
                        sec_num++;
                }
                else if (flash1m_flag)
                {
                    if (sec_num < 31)
                        sec_num++;
                }
                else if (sram_flag)
                {
                    if (offset <= 167800832)
                        offset += 4096;
                }

                ClearFrame(RGB555_BLACK);
            }
            // 下キーでアクセスするSectorNoもしくはoffsetを減らす
            else if (Trg == PAD_KEY_DOWN)
            {
                if (flash512k_flag || flash1m_flag)
                {
                    if (sec_num > 0)
                        sec_num--;
                }
                else if (sram_flag)
                {
                    if (offset >= 167776256)
                        offset -= 4096;
                }
                ClearFrame(RGB555_BLACK);
            }
            // 右キーでアクセスするSizeを増やす
            else if (Cont == PAD_KEY_RIGHT)
            {
                if (size <= 4088)
                    size += 8;
                OS_TPrintf("Access Size = %d\n", size);
                ClearFrame(RGB555_BLACK);
            }
            // 左キーでアクセスするSizeを減らす
            else if (Cont == PAD_KEY_LEFT)
            {
                if (size >= 8)
                    size -= 8;
                OS_TPrintf("Access Size = %d\n", size);
                ClearFrame(RGB555_BLACK);
            }
            // STARTで非同期/同期モードの変更
            else if (Trg == PAD_BUTTON_START)
            {
                async_flag = !async_flag;
                ClearFrame(RGB555_BLACK);
            }
            // SELECTでVerify
            else if (Trg == PAD_BUTTON_SELECT)
            {
                if (async_flag)
                {
                    async_task_busy = TRUE;
                    if (!sram_flag)
                    {
                        CTRDG_VerifyAgbFlashAsync(sec_num, src_buf2, size, TaskBusyCallback);
                    }
                    else
                    {
                        offset = CTRDG_AGB_SRAM_ADR;
                        CTRDG_VerifyAgbSramAsync(offset, src_buf2, size, TaskBusyCallback);
                    }
                }
                else
                {
                    if (!sram_flag)
                    {
                        result = CTRDG_VerifyAgbFlash(sec_num, src_buf2, size);
                    }
                    else
                    {
                        offset = CTRDG_AGB_SRAM_ADR;
                        result = CTRDG_VerifyAgbSram(offset, src_buf2, size);
                    }
                    if (result != 0)
                        OS_TPrintf("error : Verify\n");
                }
                OS_TPrintf("Verify sector%d\n", sec_num);
            }
            // Lでスレッドの優先度を0に設定する
            else if (Trg == PAD_BUTTON_L)
            {
                u32     priority = 0;
                CTRDG_SetTaskThreadPriority(priority);
                OS_TPrintf("Task Thread Priority = %d\n", priority);
            }
        }
        /* 文字列表示 */
        if (flash512k_flag || flash1m_flag)
        {
            DrawText(10, 140, "SectorNo    : %d", sec_num);
        }
        else if (sram_flag)
        {
            DrawText(10, 140, "offset      : %x", offset);
        }
        DrawText(10, 150, "Access Size : %d", size);
        DrawText(10, 130, "result      : ");
        if (nocommond_flag)
            DrawText(120, 130, "Erase is nothing");
        if (!nocommond_flag)
        {
            for (i = 0; i < size; i++)
            {
                if (dst_buf[i] != 0xFF)
                    DrawText(120 + (i * 8), 130, "%c", dst_buf[i]);
            }
        }
        if (async_flag)
            DrawText(10, 170, "Async Mode");
        if (!async_flag)
            DrawText(10, 170, "Sync Mode");

        DrawEnd();
        OS_WaitVBlankIntr();
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
        CTRDGBackupType type;
        const char *comment;
    }
    types_table[] =
    {
        { CTRDG_BACKUP_TYPE_FLASH_512K,   "FLASH   512 kb"},
        { CTRDG_BACKUP_TYPE_FLASH_1M,     "FLASH     1 Mb"},
        { CTRDG_BACKUP_TYPE_SRAM,         "SRAM    256 kb"},
    };
    /* *INDENT-ON* */
    enum
    { types_table_max = sizeof(types_table) / sizeof(*types_table) };

    int     cur = 0;
    BOOL    error = FALSE;
    BOOL    end = TRUE;
    int     i;

    while (end != 0)
    {

        keyData = PAD_Read();
        keyData = PAD_Read();
        Trg = (u16)(keyData & (keyData ^ Cont));
        Cont = keyData;

        DrawBegin();
        ClearFrame(RGB555_BLACK);

        /* 上下キーでカーソル移動 */
        if (Trg & PAD_KEY_DOWN)
        {
            error = FALSE;
            if (++cur >= types_table_max)
                cur -= types_table_max;
        }
        if (Trg & PAD_KEY_UP)
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
        if (Trg & PAD_BUTTON_A)
        {
            end = CTRDG_IdentifyAgbBackup(types_table[cur].type);
            if (end != 0)
            {
                error = TRUE;
            }
            else
            {
                if (types_table[cur].type == CTRDG_BACKUP_TYPE_SRAM)
                    sram_flag = TRUE;
                if (types_table[cur].type == CTRDG_BACKUP_TYPE_FLASH_512K)
                    flash512k_flag = TRUE;
                if (types_table[cur].type == CTRDG_BACKUP_TYPE_FLASH_1M)
                    flash1m_flag = TRUE;
            }
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
            DrawText(10, 120, "error!");
        }
        DrawEnd();
    }
}
