/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:51:25  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.12  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.11  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.10  2004/10/26 04:27:56  sato_masaki
  TGIDの値をRTCの秒数から、取得するように変更。

  Revision 1.9  2004/10/05 06:47:54  sato_masaki
  設定メニューを追加。その他色々細かな修正。

  Revision 1.8  2004/09/18 04:30:07  sato_masaki
  PXI_InitFifo, OS_InitThread()を削除。

  Revision 1.7  2004/09/10 02:03:17  sato_masaki
  記述修正。bin -> srl

  Revision 1.6  2004/09/08 08:16:01  sato_masaki
  FS 初期化部分変更

  Revision 1.5  2004/09/08 00:56:09  sato_masaki
  読み込んだセグメントイメージのバッファをヒープから確保するように変更。

  Revision 1.4  2004/09/01 00:12:42  yosizaki
  change the timing of OS_EnableIrq.

  Revision 1.3  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.2  2004/08/19 08:10:58  ooe
  beaconPeriodランダム化実験用に、Aボタン待ちを追加

  Revision 1.1  2004/08/10 07:31:53  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>


#include "common.h"
#include "dispfunc.h"

/*
 * BeaconPeriodのランダム化用(rand)
 */
#include <stdlib.h>

/*
 * 単純なマルチブート親機のサンプル.
 *
 * MBライブラリのサンプルはマルチブート機能を使うため、
 * 同じ通信環境(無線あるいは有線)どうしの開発機材を複数必要とします。
 * $NitroSDK/bin/ARM9-TS/Release/ ディレクトリにある
 * mb_child.srl プログラムが最終実機におけるマルチブート子機と同等の
 * 機能を提供するサンプルになっていますので、
 * このバイナリをサンプルプログラムと同じ方法で他の機材に読み込み、
 * 一緒に実行してください。
 */


/*---------------------------------------------------------------------------*/
/* function */

static void Menu(void);

/* Ｖブランク割り込み処理 */
static void VBlankIntr(void)
{
    //---- ＯＡＭの更新
    DC_FlushRange(oamBak, sizeof(oamBak));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    MI_DmaCopy32(3, oamBak, (void *)HW_OAM, sizeof(oamBak));

    //---- BGスクリーン更新
    BgScrSetVblank();

    (void)rand();

    //---- 割り込みチェックフラグ
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/* メイン */
void NitroMain()
{
    /* OS 初期化 */
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    FX_Init();

    /* GX 初期化 */
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    {                                  /* メモリ割り当ての初期化 */
        OSHeapHandle heapHandle;       // Heapハンドル
        enum
        { MAIN_HEAP_SIZE = 0x80000 };
        void   *heapStart, *nstart;
        nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 16);
        OS_SetMainArenaLo(nstart);
        heapStart = OS_AllocFromMainArenaLo((u32)MAIN_HEAP_SIZE, 32);
        heapHandle =
            OS_CreateHeap(OS_ARENA_MAIN, heapStart, (void *)((u32)heapStart + MAIN_HEAP_SIZE));
        if (heapHandle < 0)
        {
            OS_Panic("ARM9: Fail to create heap...\n");
        }
        heapHandle = OS_SetCurrentHeap(OS_ARENA_MAIN, heapHandle);
    }

    /* FS 初期化 */
    FS_Init(FS_DMA_NOT_USE);
    {
        u32     need_size = FS_GetTableSize();
        void   *p_table = OS_Alloc(need_size);
        SDK_ASSERT(p_table != NULL);
        (void)FS_LoadTable(p_table, need_size);
    }

    {                                  /* 描画設定初期化 */
        GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
        MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
        (void)GX_DisableBankForLCDC();
        MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
        MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
        MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
        MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);
        BgInitForPrintStr();
        GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);
        GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);
        GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);
        GXS_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);
        GX_LoadOBJ(sampleCharData, 0, sizeof(sampleCharData));
        GX_LoadOBJPltt(samplePlttData, 0, sizeof(samplePlttData));
        MI_DmaFill32(3, oamBak, 0xc0, sizeof(oamBak));
    }

    /* Vブランク割込設定 */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    // RTC初期化
    RTC_Init();

    /* 表示開始 */
    GX_DispOn();
    GXS_DispOn();
    G2_SetBG0Offset(0, 0);
    G2S_SetBG0Offset(0, 0);

    /* ファイルバッファへのポインタを初期化 */
    InitFileBuffer();

    /* メインループ */
    prog_state = STATE_MENU;
    for (;;)
    {
        OS_WaitVBlankIntr();

        switch (prog_state)
        {
        case STATE_MENU:
            Menu();
            break;
        case STATE_MB_PARENTINIT:
            ParentInit();
            break;
        case STATE_MB_PARENT:
            ParentMain();
            break;
        default:
            break;
        }
    }

}

/* 
 * -------------------------------------------------------------
 * Menu 処理関数
 * -------------------------------------------------------------
 */

enum
{
    MENU_SENDSIZE,
    MENU_MAX_CHILDREN,
    MENU_AUTO_ACCEPT,
    MENU_AUTO_SENDFILE,
    MENU_AUTO_BOOTREQUEST,
    MENU_NUM
};

/* 送信サイズテーブル */
u16     sendsize_table[] = {
    256, 384, 510,
};

/* Menu用ワーク */
typedef struct
{
    u8      item_no;
    u8      size_index;
    u8      channel;
    u8      auto_accept_flag;
    u8      auto_send_flag;
    u8      auto_boot_flag;
}
MenuData;

/* Menu */
static void Menu(void)
{
    const u16 keyData = ReadKeySetTrigger(PAD_Read());
    static MenuData menu;
    enum
    { DISP_OX = 5, DISP_OY = 5 };

    /* カーソル番号変更 */
    if (keyData & PAD_KEY_DOWN)
    {
        (void)RotateU8(&menu.item_no, 0, MENU_NUM - 1, 1);
    }
    else if (keyData & PAD_KEY_UP)
    {
        (void)RotateU8(&menu.item_no, 0, MENU_NUM - 1, -1);
    }
    else if (keyData & (PAD_BUTTON_A | PAD_BUTTON_START))
    {
        prog_state = STATE_MB_PARENTINIT;
    }

    /* 表示開始 */
    BgClear();
    BgPutString((s16)(DISP_OX + 3), (s16)(DISP_OY - 3), WHITE, "Multiboot Sample");

    BgPrintf((s16)(DISP_OX), (s16)(DISP_OY), WHITE,
             "Send Size(Bytes)    [%3d]", GetParentSendSize());
    BgPrintf((s16)(DISP_OX), (s16)(DISP_OY + 1), WHITE, "Children Entry Max  [%2d]", GetMaxEntry());
    BgPrintf((s16)(DISP_OX), (s16)(DISP_OY + 2), WHITE,
             "Auto Accept Entries %s", (menu.auto_accept_flag) ? "[Yes]" : "[No ]");
    BgPrintf((s16)(DISP_OX), (s16)(DISP_OY + 3), WHITE,
             "Auto Send Start     %s", (menu.auto_send_flag) ? "[Yes]" : "[No ]");
    BgPrintf((s16)(DISP_OX), (s16)(DISP_OY + 4), WHITE,
             "Auto Boot           %s", (menu.auto_boot_flag) ? "[Yes]" : "[No ]");
    /* カーソル表示 */
    BgPutChar((s16)(DISP_OX - 2), (s16)(DISP_OY + menu.item_no), RED, '*');
    /* Aボタン待ち */
    BgPutString(DISP_OX, DISP_OY + 14, WHITE, " Push A or START Button");

    switch (menu.item_no)
    {
        /* MP送信サイズの設定 */
    case MENU_SENDSIZE:
        if (keyData & PAD_KEY_RIGHT)
        {
            (void)RotateU8(&menu.size_index, 0, sizeof(sendsize_table) / sizeof(u16) - 1, 1);
        }
        else if (keyData & PAD_KEY_LEFT)
        {
            (void)RotateU8(&menu.size_index, 0, sizeof(sendsize_table) / sizeof(u16) - 1, -1);
        }
        SetParentSendSize(sendsize_table[menu.size_index]);
        break;

        /* 子機接続最大数の設定 */
    case MENU_MAX_CHILDREN:
        {
            u8      entry = GetMaxEntry();
            if (keyData & PAD_KEY_RIGHT)
            {
                (void)RotateU8(&entry, 1, MB_MAX_CHILD, 1);
            }
            else if (keyData & PAD_KEY_LEFT)
            {
                (void)RotateU8(&entry, 1, MB_MAX_CHILD, -1);
            }
            SetMaxEntry(entry);
        }
        break;

        /* AUTO ACCEPTの設定 */
    case MENU_AUTO_ACCEPT:
        if (keyData & (PAD_KEY_RIGHT | PAD_KEY_LEFT))
        {
            menu.auto_accept_flag ^= 1;
        }
        break;

        /* AUTO SENDの設定 */
    case MENU_AUTO_SENDFILE:
        if (keyData & (PAD_KEY_RIGHT | PAD_KEY_LEFT))
        {
            menu.auto_send_flag ^= 1;
        }
        break;

        /* AUTO BOOTの設定 */
    case MENU_AUTO_BOOTREQUEST:
        if (keyData & (PAD_KEY_RIGHT | PAD_KEY_LEFT))
        {
            menu.auto_boot_flag ^= 1;
        }
        break;
    default:
        break;
    }

    SetAutoOperation(menu.auto_accept_flag, menu.auto_send_flag, menu.auto_boot_flag);

}
