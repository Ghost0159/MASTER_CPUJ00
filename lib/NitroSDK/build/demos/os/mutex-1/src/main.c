/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - mutex-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.23  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.22  2005/11/21 10:54:50  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.21  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.20  2005/03/03 12:55:41  takano_makoto
  exclude any codes which is available in specific compiler.

  Revision 1.19  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.18  2005/02/18 05:33:47  takano_makoto
  GX_Initの前にOS_Initを呼ぶように修正

  Revision 1.17  2004/06/18 13:08:29  terui
  Swap font data.

  Revision 1.16  2004/06/08 00:19:49  yada
  add '#pragma unused()' for FINALROM

  Revision 1.15  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.14  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.13  2004/02/26 09:42:45  yada
  OS_CreateThread() に起動関数の引数が追加され、
  それに伴う修正

  Revision 1.12  2004/02/10 05:47:46  yasu
  delete macro CODE32

  Revision 1.11  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.10  2004/02/02 05:34:12  yada
  OS_SetIrqMask()→OS_EnableIrqMask() へ

  Revision 1.9  2004/01/18 07:29:23  yada
  GX関数の名称変更に伴う修正

  Revision 1.8  2004/01/18 06:49:20  yada
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    VBlankIntr(void);
void    ObjSet(int objNo, int x, int y, int charNo, int paletteNo);

static GXOamAttr oamBak[128];
extern const u16 samplePlttData[16][16];
extern const u32 sampleCharData[8 * 0x100];

//---- thread
#define    STACK_SIZE     1024
#define    THREAD1_PRIO   1
#define    THREAD2_PRIO   2
OSThread thread1;
OSThread thread2;
u64     stack1[STACK_SIZE / sizeof(u64)];
u64     stack2[STACK_SIZE / sizeof(u64)];
void    proc1(void *p1);
void    proc2(void *p1);

OSMutex mutex;

void    common(u32 val);

//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    //================ 初期化
    OS_Init();
    //---- GX 初期化
    GX_Init();

    //================ Mutex 作成
    OS_InitMutex(&mutex);

    //================ スレッド起動
    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, NULL, stack2 + STACK_SIZE / sizeof(u64), STACK_SIZE,
                    THREAD2_PRIO);
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

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
    GX_SetBankForLCDC(0);

    //---- OAMとパレットクリア
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);


    //---- バンクＡをＯＢＪへ
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);        // バンクＡをＯＢＪに割り当てる

    //---- グラフィックス表示モードにする
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    //---- OBJの表示のみON
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);

    //---- 32KバイトのOBJで2Dマップモードで使用
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    //================ データロード
    MI_DmaCopy32(3, sampleCharData, (void *)HW_OBJ_VRAM, sizeof(sampleCharData));
    MI_DmaCopy32(3, samplePlttData, (void *)HW_OBJ_PLTT, sizeof(samplePlttData));

    GX_DispOn();

    //================ メインループ
    while (1)
    {
        u16     keyData;

        //---- Vブランク割込終了待ち
        OS_WaitVBlankIntr();

        //---- 非表示OBJは画面外へ移動
        MI_DmaFill32(3, oamBak, 0xc0, sizeof(oamBak));

        //---- パッドデータ読み込み
        keyData = PAD_Read();

        //---- パッド情報を OBJ で表示
        ObjSet(0, 180, 95, 'A', (keyData & PAD_BUTTON_A) ? 1 : 2);
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

    //---- 割り込み要求フラグリセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    //---- スレッド１起動
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);
}

//--------------------------------------------------------------------------------
//    スレッド１
//
void proc1(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        common(1);
        OS_SleepThread(NULL);
    }
}

//--------------------------------------------------------------------------------
//    スレッド２
//
void proc2(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        common(2);
    }
}

//--------------------------------------------------------------------------------
//    共通
//
void common(u32 val)
{
#ifdef SDK_FINALROM
#pragma unused( val )
#endif

    OS_Printf("common IN thread=%d\n", val);
    OS_LockMutex(&mutex);

    OS_Printf("<Critical routine> thread=%d\n", val);

    //---- Ａボタントリガ待ち
    while (!(PAD_Read() & PAD_BUTTON_A))
    {
        OS_SleepThread(NULL);
    }
    while (PAD_Read() & PAD_BUTTON_A)
    {
        OS_SleepThread(NULL);
    }

    OS_Printf("common OUT thread=%d\n", val);
    OS_UnlockMutex(&mutex);
}

/*====== End of main.c ======*/
