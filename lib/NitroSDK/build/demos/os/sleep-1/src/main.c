/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - sleep-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.10  2005/11/21 10:55:34  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.9  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.8  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.7  2004/09/30 09:32:35  yada
  exchange OS_InitAlarm() and OS_InitTick()

  Revision 1.6  2004/09/30 05:08:02  yada
  add OS_InitAlarm().  to remove it was mistake

  Revision 1.5  2004/09/28 12:25:26  yada
  not call OS_InitAlarm() because OS_Init() called

  Revision 1.4  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.3  2004/03/30 08:38:22  yada
  小修正

  Revision 1.2  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.1  2004/03/03 09:19:42  yada
  firstRelease

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "data.h"

static GXOamAttr oamBak[128];

void    VBlankIntr(void);
void    ObjSet(int objNo, int x, int y, int charNo, int paletteNo);


//---- OBJ 表示用に 0～9,A～F のコードを返す
inline int ObjChar(u32 cnt, int shift)
{
    u32     d = (cnt >> shift) & 0xf;
    return (int)((d < 10) ? '0' + d : 'A' + d - 10);
}


//---- サブスレッド用の定義・宣言
#define STACK_SIZE  1024
#define THREAD_PRIO 20

OSThread thread;
void    proc(void *arg);

u64     stack[STACK_SIZE / sizeof(u64)];


//---- 表示カウンタ
static u32 count1;                     // ラウンチスレッド用
static u32 count2;                     // サブスレッド用

//================================================================================
//   ラウンチスレッド用のカウンタがインクリメントされ、表示されています。
//   A ボタンを押すと、このスレッドは１秒間スリープします。
//   その間、サブスレッドが起動します。
//   普段はサブスレッドは優先度が低いので動作しません。
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    //================ 初期化
    OS_Init();

    OS_InitTick();
    OS_InitAlarm();
    OS_InitThread();

    GX_Init();

    //---- サブスレッド起動
    OS_CreateThread(&thread, proc, NULL, stack + STACK_SIZE / sizeof(u64), STACK_SIZE, THREAD_PRIO);
    OS_WakeupThreadDirect(&thread);


    //================ 設定
    //---- 電源全てオン
    GX_SetPower(GX_POWER_ALL);

    //----  Vブランク割込許可
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    //---- Vブランク発生設定
    (void)GX_VBlankIntr(TRUE);

    //---- VRAM クリア
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    //---- OAMとパレットクリア
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);

    //---- バンクＡをＯＢＪへ
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);

    //---- グラフィックス表示モードにする
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    //---- OBJの表示のみON
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);

    //---- 32KバイトのOBJで2Dマップモードで使用
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    //---- データロード
    MI_DmaCopy32(3, sampleCharData, (void *)HW_OBJ_VRAM, sizeof(sampleCharData));
    MI_DmaCopy32(3, samplePlttData, (void *)HW_OBJ_PLTT, sizeof(samplePlttData));

    //---- 非表示OBJは画面外へ移動
    MI_DmaFill32(3, oamBak, 0xc0, sizeof(oamBak));

    GX_DispOn();

    //================ メインループ
    while (1)
    {
        u16     keyData;

        //---- Vブランク割込終了待ち
        OS_WaitVBlankIntr();

        //---- パッドデータ読み込み
        keyData = PAD_Read();

        //---- カウント増加
        count1++;

        //---- カウント表示
        ObjSet(0, 50, 100, ObjChar(count1, 28), 2);
        ObjSet(1, 60, 100, ObjChar(count1, 24), 2);
        ObjSet(2, 70, 100, ObjChar(count1, 20), 2);
        ObjSet(3, 80, 100, ObjChar(count1, 16), 2);
        ObjSet(4, 90, 100, ObjChar(count1, 12), 2);
        ObjSet(5, 100, 100, ObjChar(count1, 8), 2);
        ObjSet(6, 110, 100, ObjChar(count1, 4), 2);
        ObjSet(7, 120, 100, ObjChar(count1, 0), 2);

        ObjSet(8, 140, 100, ObjChar(count2, 28), 2);
        ObjSet(9, 150, 100, ObjChar(count2, 24), 2);
        ObjSet(10, 160, 100, ObjChar(count2, 20), 2);
        ObjSet(11, 170, 100, ObjChar(count2, 16), 2);
        ObjSet(12, 180, 100, ObjChar(count2, 12), 2);
        ObjSet(13, 190, 100, ObjChar(count2, 8), 2);
        ObjSet(14, 200, 100, ObjChar(count2, 4), 2);
        ObjSet(15, 210, 100, ObjChar(count2, 0), 2);


        //---- A ボタンを押したら１秒寝る
        if (keyData & PAD_BUTTON_A)
        {
            OS_Sleep(1000);
        }
    }
}

//--------------------------------------------------------------------------------
//  OBJ セット
//
void ObjSet(int objNo, int x, int y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&oamBak[objNo],
                  x,
                  y,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}


//--------------------------------------------------------------------------------
//    Ｖブランク割り込み処理
//
void VBlankIntr(void)
{
    //---- ＯＡＭの更新
    DC_FlushRange(oamBak, sizeof(oamBak));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    MI_DmaCopy32(3, oamBak, (void *)HW_OAM, sizeof(oamBak));

    //---- 割り込みチェックフラグ
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//--------------------------------------------------------------------------------
//    
//
void proc(void *arg)
{
#pragma unused( arg )

    while (1)
    {
        //---- Vブランク割込終了待ち
        OS_WaitVBlankIntr();

        //---- カウント増加
        count2++;
    }
}

/*====== End of main.c ======*/
