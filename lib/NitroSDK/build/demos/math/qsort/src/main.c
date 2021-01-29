/*---------------------------------------------------------------------------*
  Project:  NitroSDK - demos - math - qsort
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.6  2007/10/19 03:34:56  yosizaki
  fix comparator implementation.
  add result-check.

  Revision 1.5  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 10:47:53  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.3  2005/09/09 09:11:07  takano_makoto
  比較の条件式を変更

  Revision 1.2  2005/04/13 23:45:42  takano_makoto
  fix warning.

  Revision 1.1  2005/04/12 10:33:57  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro.h>

/*---------------------------------------------------------------------------*
    Prototype definition
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);
static void KeyInit(void);
static void KeyRead(void);


// Key State
static struct
{
    u16     con;
    u16     trg;
}
keys;

#define DATA_NUM    1024

/*---------------------------------------------------------------------------*
    Static variables definition
 *---------------------------------------------------------------------------*/
static u32 gDataArray[DATA_NUM];
static u8 *gSortBuf;


/*---------------------------------------------------------------------------*
  Name:         KeyInit

  Description:  Initialize Pad Keys.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyInit(void)
{

    keys.trg = 0;
    keys.con = 0;

}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  Read Pad Keys.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(void)
{
    u16     r = PAD_Read();

    keys.trg = (u16)(~keys.con & r);
    keys.con = r;
}


/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  メインメモリ上のアリーナにてメモリ割当てシステムを初期化する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    // OS_Initは呼ばれているという前提
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}



/*---------------------------------------------------------------------------*
  Name:         DisplayInit

  Description:  Graphics Initialization

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DisplayInit()
{

    GX_Init();
    FX_Init();

    GX_DispOff();
    GXS_DispOff();

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
    (void)OS_EnableIrq();


    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     // clear the standard palette

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
    MI_DmaFill32(3, (void *)HW_LCDC_VRAM_C, 0x7FFF7FFF, 256 * 192 * sizeof(u16));


    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);       // Set VRAM-A,B for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // 2D / 3D Mode
                       GX_BGMODE_0,    // BGMODE 0
                       GX_BG0_AS_2D);  // Set BG0 as 2D

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GX_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();

}







/* ユーザデータ比較関数 */
static s32 compare(void *elem1, void *elem2)
{
    // 符号ビットへオーバーフローしないよう、s64へ昇格して比較する。
    s64     diff = (s64)*(u32 *)elem1 - (s64)*(u32 *)elem2;
    // 大小関係のみを{+1,0,-1}に正規化して返す。
    return (diff > 0) ? +1 : ((diff < 0) ? -1 : 0);
}

/* 配列データのデバッグ出力関数 */
static void PrintArray(u32 *array, u32 size)
{
#pragma unused( array )
    u32     i;

    for (i = 0; i < size; i++)
    {
        if ((i & 0x7) == 0)
        {
            OS_TPrintf("\n");
        }
        OS_TPrintf("%08lx,", array[i]);
    }
    OS_TPrintf("\n");
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  Initialization and main loop

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    MATHRandContext32 context;

    // Initialization
    KeyInit();
    OS_Init();
    TP_Init();
    OS_InitTick();

    // メモリ割当
    InitializeAllocateSystem();

    DisplayInit();

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    KeyRead();
    // 乱数でデータを埋める
    MATH_InitRand32(&context, 0);

    OS_TPrintf("Press A Button to start MATH_QSort() test\n");

    while (TRUE)
    {
        static u32 cnt = 0;
        static OSTick sum = 0;

        KeyRead();

        if ((keys.trg & PAD_BUTTON_A) != 0)
        {
            u32     i;

            {
                for (i = 0; i < DATA_NUM; i++)
                {
                    gDataArray[i] = MATH_Rand32(&context, 0);
                }
            }
            {
                OSTick  before, after;

                // クイックソート
                gSortBuf = (u8 *)OS_Alloc(MATH_QSortStackSize(DATA_NUM));

                before = OS_GetTick();
                MATH_QSort(gDataArray, DATA_NUM, sizeof(u32), compare, NULL);
                after = OS_GetTick();
                OS_Free(gSortBuf);

                // 正しく昇順ソートされていることを確認
                for (i = 0; i < DATA_NUM; i++)
                {
                    u32 lower = gDataArray[MATH_MAX(i - 1, 0)];
                    u32 upper = gDataArray[i];
                    if (lower > upper)
                    {
                        OS_TPanic("result array is not sorted correctly!");
                    }
                }

                // ソート結果のプリント表示
                PrintArray(gDataArray, DATA_NUM);
                cnt++;
                sum += OS_TicksToMicroSeconds(after - before);

                OS_TPrintf("time = %lld us (avg %lld us)\n", OS_TicksToMicroSeconds(after - before),
                           sum / cnt);
                OS_TPrintf("Press A Button to start MATH_QSort() test again\n");
            }
        }
        // Wait V-Blank interrupt
        OS_WaitVBlankIntr();
    }
}


/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V-Blank function

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{

    // Set IRQ check flag
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}



/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
