/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.6  2005/12/22 06:05:16  yosizaki
  add test for CTZ/LSB/MSB.

  Revision 1.5  2005/11/21 10:47:43  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.4  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.3  2005/02/18 11:52:41  seiki_masashi
  warning 対策

  Revision 1.2  2004/12/16 02:48:50  seiki_masashi
  表記や評価順の変更

  Revision 1.1  2004/12/16 02:27:15  seiki_masashi
  misc-1 デモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  MATH ライブラリ API の動作確認デモ
 *---------------------------------------------------------------------------*/

#include    <nitro.h>



static void VBlankIntr(void);
static void DisplayInit(void);
static void FillScreen(u16 col);
static BOOL MathTest(void);

/*---------------------------------------------------------------------------*
    変数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // 各種初期化
    OS_Init();

    DisplayInit();

    if (MathTest())
    {
        // 成功
        OS_TPrintf("------ Test Succeeded ------\n");
        FillScreen(GX_RGB(0, 31, 0));
    }
    else
    {
        // 失敗
        OS_TPrintf("****** Test Failed ******\n");
        FillScreen(GX_RGB(31, 0, 0));
    }

    // メインループ
    while (TRUE)
    {
        // Ｖブランク待ち
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Ｖブランク割込みベクトル。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // IRQ チェックフラグをセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         DisplayInit

  Description:  Graphics Initialization

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DisplayInit(void)
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

    GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,      // VRAM mode.
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GX_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();

}


/*---------------------------------------------------------------------------*
  Name:         FillScreen

  Description:  画面を塗りつぶす

  Arguments:    col - FillColor.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void FillScreen(u16 col)
{
    MI_CpuFill16((void *)HW_LCDC_VRAM_C, col, 256 * 192 * 2);
}

/*---------------------------------------------------------------------------*
  Name:         MathTest

  Description:  数学関数のテストルーチン

  Arguments:    None.

  Returns:      テストが成功したら true.
 *---------------------------------------------------------------------------*/
#define PrintResultEq( a, b, f ) \
    { OS_TPrintf( ((a) == (b)) ? "[--OK--] " : "[**NG**] " ); (f) = (f) && ((a) == (b)); }

static BOOL MathTest(void)
{
    int     i;
    BOOL    flag = TRUE;

    {
        int     a[] = { 0, 1, 0x7fffffff, -1, -0x80000000 };
        int     result_abs[] = { 0, 1, 0x7fffffff, 1, -0x80000000 };
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_ABS(a[i]);
            PrintResultEq(result, result_abs[i], flag);
            OS_TPrintf("MATH_ABS(%d) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_IAbs(a[i]);
            PrintResultEq(result, result_abs[i], flag);
            OS_TPrintf("MATH_IAbs(%d) = %d\n", a[i], result);
        }
    }

    {
        // TODO: NaN や非正規化数, -0.0 も check
        double  a[] = { 0, 1, -1, 1.0e20, 1.123e-20, -1.0e20, -1.123e-20 };
        double  result_abs[] = { 0, 1, 1, 1.0e20, 1.123e-20, 1.0e20, 1.123e-20 };
        for (i = 0; i < sizeof(a) / sizeof(double); i++)
        {
            double  result;
            result = MATH_ABS(a[i]);
            PrintResultEq(result, result_abs[i], flag);
            OS_Printf("MATH_ABS(%g) = %g\n", a[i], result);
        }
    }

    {
        int     a[] = { 0, 1, 0, -1, 0, 32, 31, -32, -31 };
        int     b[] = { 0, 0, 1, 0, -1, 31, 32, -31, -32 };
        int     result_min[] = { 0, 0, 0, -1, -1, 31, 31, -32, -32 };
        int     result_max[] = { 0, 1, 1, 0, 0, 32, 32, -31, -31 };
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_MIN(a[i], b[i]);
            PrintResultEq(result, result_min[i], flag);
            OS_TPrintf("MATH_MIN(%d, %d) = %d\n", a[i], b[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_IMin(a[i], b[i]);
            PrintResultEq(result, result_min[i], flag);
            OS_TPrintf("MATH_IMin(%d, %d) = %d\n", a[i], b[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_MAX(a[i], b[i]);
            PrintResultEq(result, result_max[i], flag);
            OS_TPrintf("MATH_MAX(%d, %d) = %d\n", a[i], b[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_IMax(a[i], b[i]);
            PrintResultEq(result, result_max[i], flag);
            OS_TPrintf("MATH_IMax(%d, %d) = %d\n", a[i], b[i], result);
        }
    }

    {
        // TODO: NaN や非正規化数, -0.0 も check
        double  a[] = { 0, 1, 0, -1, 0, 1.0e15, 1.0e-15, 1.0e-15, 1.0e-16, -1.0e-15, -1.1e-15 };
        double  b[] = { 0, 0, 1, 0, -1, 1.0e-15, 1.0e15, 1.0e-16, 1.0e-15, -1.1e-15, -1.0e-15 };
        double  result_min[] =
            { 0, 0, 0, -1, -1, 1.0e-15, 1.0e-15, 1.0e-16, 1.0e-16, -1.1e-15, -1.1e-15 };
        double  result_max[] =
            { 0, 1, 1, 0, 0, 1.0e15, 1.0e15, 1.0e-15, 1.0e-15, -1.0e-15, -1.0e-15 };
        for (i = 0; i < sizeof(a) / sizeof(double); i++)
        {
            double  result;
            result = MATH_MIN(a[i], b[i]);
            PrintResultEq(result, result_min[i], flag);
            OS_Printf("MATH_MIN(%g, %g) = %g\n", a[i], b[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(double); i++)
        {
            double  result;
            result = MATH_MAX(a[i], b[i]);
            PrintResultEq(result, result_max[i], flag);
            OS_Printf("MATH_MAX(%g, %g) = %g\n", a[i], b[i], result);
        }
    }

    {
        int     a[] = { -2, -1, 0, 1, 2, 0, 10, -2, -10, -0x80000000, 0x7fffffff };
        int     low[] = { -1, -1, -1, -1, -1, 1, 2, -8, -8, -0x80000000, -0x80000000 };
        int     high[] = { 1, 1, 1, 1, 1, 1, 8, -4, -4, 0x7fffffff, 0x7fffffff };
        int     result_clp[] = { -1, -1, 0, 1, 1, 1, 8, -4, -8, -0x80000000, 0x7fffffff };
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_CLAMP(a[i], low[i], high[i]);
            PrintResultEq(result, result_clp[i], flag);
            OS_TPrintf("MATH_CLAMP(%d, %d, %d) = %d\n", a[i], low[i], high[i], result);
        }
    }

    {
        int     a[] = { 0, 1, 31, 32, 33, 128, -1, -31, -32, -33, 0x7fffffff, -0x80000000 };
        int     result_rup[] = { 0, 32, 32, 32, 64, 128, 0, 0, -32, -32, -0x80000000, -0x80000000 };
        int     result_rdn[] =
            { 0, 0, 0, 32, 32, 128, -32, -32, -32, -64, 0x7fffffe0, -0x80000000 };
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_ROUNDUP(a[i], 32);
            PrintResultEq(result, result_rup[i], flag);
            OS_TPrintf("MATH_ROUNDUP(%d, 32) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_ROUNDUP32(a[i]);
            PrintResultEq(result, result_rup[i], flag);
            OS_TPrintf("MATH_ROUNDUP32(%d) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_ROUNDDOWN(a[i], 32);
            PrintResultEq(result, result_rdn[i], flag);
            OS_TPrintf("MATH_ROUNDDOWN(%d, 32) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_ROUNDDOWN32(a[i]);
            PrintResultEq(result, result_rdn[i], flag);
            OS_TPrintf("MATH_ROUNDDOWN32(%d) = %d\n", a[i], result);
        }
    }

    {
        u32     a[] =
            { 0, 1, 0xffffffff, 0xffff0000, 0x0000ffff, 0xaaaaaaaa, 0x55555555, 0x1234cdef };
        int     result_clz[] = { 32, 31, 0, 0, 16, 0, 1, 3 };
        int     result_ctz[] = { 32, 0, 0, 16, 0, 1, 0, 0 };
        u32     result_lsb[] = { 0, 1, 1, 0x00010000, 1, 0x00000002, 0x00000001, 0x00000001 };
        u32     result_msb[] =
            { 0, 1, 0x80000000, 0x80000000, 0x00008000, 0x80000000, 0x40000000, 0x10000000 };
        int     result_pop[] = { 0, 1, 32, 16, 16, 16, 16, 17 };
        int     result_lg2[] = { -1, 0, 31, 31, 15, 31, 30, 28 };
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            u32     result;
            result = MATH_CountLeadingZeros(a[i]);
            PrintResultEq(result, result_clz[i], flag);
            OS_TPrintf("MATH_CountLeadingZeros(0x%08x) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            u32     result;
            result = MATH_CLZ(a[i]);
            PrintResultEq(result, result_clz[i], flag);
            OS_TPrintf("MATH_CLZ(0x%08x) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            u32     result;
            result = MATH_CTZ(a[i]);
            PrintResultEq(result, result_ctz[i], flag);
            OS_TPrintf("MATH_CTZ(0x%08x) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            u32     result;
            result = MATH_LSB(a[i]);
            PrintResultEq(result, result_lsb[i], flag);
            OS_TPrintf("MATH_LSB(0x%08x) = %08x\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            u32     result;
            result = MATH_MSB(a[i]);
            PrintResultEq(result, result_msb[i], flag);
            OS_TPrintf("MATH_MSB(0x%08x) = %08x\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_CountPopulation(a[i]);
            PrintResultEq(result, result_pop[i], flag);
            OS_TPrintf("MATH_CountPoulation(0x%08x) = %d\n", a[i], result);
        }
        for (i = 0; i < sizeof(a) / sizeof(int); i++)
        {
            int     result;
            result = MATH_ILog2(a[i]);
            PrintResultEq(result, result_lg2[i], flag);
            OS_TPrintf("MATH_ILog2(0x%08x) = %d\n", a[i], result);

        }
    }

    return flag;
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
