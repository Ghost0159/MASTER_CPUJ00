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
  Revision 1.5  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 10:47:13  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.3  2005/05/18 00:39:23  seiki_masashi
  警告に対応

  Revision 1.2  2005/05/17 13:00:03  seiki_masashi
  警告への対応

  Revision 1.1  2005/05/13 09:28:17  seiki_masashi
  FFT 関数の追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  MATH ライブラリ 高速フーリエ変換の動作確認デモ
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/fx/fx_trig.h>


static void VBlankIntr(void);
static void DisplayInit(void);
static void FillScreen(u16 col);
static BOOL FFTTest(void);
static void PrintFX32(fx32 f);
static void PrintRealArray(fx32 *data);
static void PrintComplexArray(fx32 *data);
static BOOL PrintError(fx32 *orig, fx32 *data);

static void InitBitRevTable(void);
static void FFT(fx32 *data, int nShift);
static void IFFT(fx32 *data, int nShift);

/*---------------------------------------------------------------------------*
    変数定義
 *---------------------------------------------------------------------------*/
#define FFT_NSHIFT         10
#define FFT_N              (1 << FFT_NSHIFT)
#define FFT_VALUE_MAX      ((1 << (31 - FFT_NSHIFT))-1)
#define FFT_VALUE_MIN      (-(1 << (31 - FFT_NSHIFT)))
#define FFT_VALUE_RANGE    (1U << (32 - FFT_NSHIFT))

#define N_RANDOM_INPUT     16
#define ERROR_THRETHOLD    (MATH_MAX(((double)FFT_VALUE_RANGE)/(1 << 23), (double)(1 << (FFT_NSHIFT-12))))

static fx32 gFFTCos[FFT_N], gFFTSin[FFT_N];
static int gBitRevTable[FFT_N];

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
    OS_InitTick();

    DisplayInit();

    if (FFTTest())
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
  Name:         FFTTest

  Description:  高速フーリエ変換のテストルーチン

  Arguments:    None.

  Returns:      テストが成功したら true.
 *---------------------------------------------------------------------------*/
#define PrintResultEq( a, b, f ) \
    { OS_TPrintf( ((a) == (b)) ? "[--OK--] " : "[**NG**] " ); (f) = (f) && ((a) == (b)); }

static BOOL FFTTest(void)
{
    static fx32 data[FFT_N * 2];
    static fx32 orig[FFT_N * 2];
    static fx16 sinTable[FFT_N - FFT_N / 4];
    static fx16 sinTable2[(FFT_N - FFT_N / 4) / 2];

    BOOL    flag = TRUE;
    int     i;
    OSTick  start, end;

    MATH_MakeFFTSinTable(sinTable, FFT_NSHIFT);
    MATH_MakeFFTSinTable(sinTable2, FFT_NSHIFT - 1);

    OS_TPrintf("N = %d\n", FFT_N);
    OS_TPrintf("\nMATH_FFT: Sin\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i * 2] = data[i * 2] = FX_SinIdx((65536 / FFT_N) * i);
            orig[i * 2 + 1] = data[i * 2 + 1] = 0;
        }
        start = OS_GetTick();
        MATH_FFT(data, FFT_NSHIFT, sinTable);
        end = OS_GetTick();
//        PrintComplexArray(data);
        MATH_IFFT(data, FFT_NSHIFT, sinTable);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFT: Cos\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i * 2] = data[i * 2] = FX_CosIdx((65536 / FFT_N) * i);
            orig[i * 2 + 1] = data[i * 2 + 1] = 0;
        }
        start = OS_GetTick();
        MATH_FFT(data, FFT_NSHIFT, sinTable);
        end = OS_GetTick();
//        PrintComplexArray(data);
        MATH_IFFT(data, FFT_NSHIFT, sinTable);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFT: Cos + Sin\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i * 2] = data[i * 2] =
                FX_CosIdx((65536 / FFT_N) * i) + FX_SinIdx((65536 / FFT_N) * i);
            orig[i * 2 + 1] = data[i * 2 + 1] = 0;
        }
        start = OS_GetTick();
        MATH_FFT(data, FFT_NSHIFT, sinTable);
        end = OS_GetTick();
//        PrintComplexArray(data);
        MATH_IFFT(data, FFT_NSHIFT, sinTable);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFT: Highest Freqency (Real)\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i * 2] = data[i * 2] = (i & 1) ? FFT_VALUE_MIN : FFT_VALUE_MAX;
            orig[i * 2 + 1] = data[i * 2 + 1] = 0;
        }
        start = OS_GetTick();
        MATH_FFT(data, FFT_NSHIFT, sinTable);
        end = OS_GetTick();
//        PrintComplexArray(data);
        MATH_IFFT(data, FFT_NSHIFT, sinTable);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFT: Highest Freqency (Complex)\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i * 2] = data[i * 2] = (i & 1) ? FFT_VALUE_MIN : FFT_VALUE_MAX;
            orig[i * 2 + 1] = data[i * 2 + 1] = (i & 1) ? FFT_VALUE_MIN : FFT_VALUE_MAX;
        }
        start = OS_GetTick();
        MATH_FFT(data, FFT_NSHIFT, sinTable);
        end = OS_GetTick();
//        PrintComplexArray(data);
        MATH_IFFT(data, FFT_NSHIFT, sinTable);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFT: Constant\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i * 2] = data[i * 2] = FFT_VALUE_MAX;
            orig[i * 2 + 1] = data[i * 2 + 1] = FFT_VALUE_MAX;
        }
        start = OS_GetTick();
        MATH_FFT(data, FFT_NSHIFT, sinTable);
        end = OS_GetTick();
//        PrintComplexArray(data);
        MATH_IFFT(data, FFT_NSHIFT, sinTable);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFT: Random Input\n");
    {
        u32     seed;
        for (seed = 0; seed < N_RANDOM_INPUT; seed++)
        {
            MATHRandContext32 rand;

            MATH_InitRand32(&rand, seed);
            for (i = 0; i < FFT_N; i++)
            {
                orig[i * 2] = data[i * 2] =
                    (fx32)(MATH_Rand32(&rand, FFT_VALUE_RANGE) - (FFT_VALUE_RANGE / 2));
                orig[i * 2 + 1] = data[i * 2 + 1] =
                    (fx32)(MATH_Rand32(&rand, FFT_VALUE_RANGE) - (FFT_VALUE_RANGE / 2));
            }
            start = OS_GetTick();
            MATH_FFT(data, FFT_NSHIFT, sinTable);
            end = OS_GetTick();
//            PrintComplexArray(data);
            MATH_IFFT(data, FFT_NSHIFT, sinTable);
            OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
            flag = PrintError(orig, data) && flag;
        }
    }

    OS_TPrintf("\nMATH_FFTReal: Sin\n");
    {
        for (i = 0; i < FFT_N; i++)
        {
            orig[i] = data[i] = FX_SinIdx((65536 / FFT_N) * i);
        }
        for (; i < FFT_N * 2; i++)
        {
            orig[i] = data[i] = 0;
        }
        start = OS_GetTick();
        MATH_FFTReal(data, FFT_NSHIFT, sinTable, sinTable2);
        end = OS_GetTick();
//        PrintRealArray(data);
        MATH_IFFTReal(data, FFT_NSHIFT, sinTable, sinTable2);
//        PrintRealArray(data);
        OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
        flag = PrintError(orig, data) && flag;
    }

    OS_TPrintf("\nMATH_FFTReal: Random Input\n");
    {
        u32     seed;
        for (seed = 0; seed < N_RANDOM_INPUT; seed++)
        {
            MATHRandContext32 rand;

            MATH_InitRand32(&rand, seed);
            for (i = 0; i < FFT_N; i++)
            {
                orig[i] = data[i] =
                    (fx32)(MATH_Rand32(&rand, FFT_VALUE_RANGE) - (FFT_VALUE_RANGE / 2));
            }
            for (; i < FFT_N * 2; i++)
            {
                orig[i] = data[i] = 0;
            }
            start = OS_GetTick();
            MATH_FFTReal(data, FFT_NSHIFT, sinTable, sinTable2);
            end = OS_GetTick();
//            PrintComplexArray(data);
            MATH_IFFTReal(data, FFT_NSHIFT, sinTable, sinTable2);
            OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));
            flag = PrintError(orig, data) && flag;
        }
    }

#if 0
    OS_TPrintf("\nLocal FFT Function\n");
    InitBitRevTable();
    {
        for (i = 0; i < FFT_N; i++)
        {
            data[i * 2] = FX_SinIdx((65536 / FFT_N) * i);
            data[i * 2 + 1] = 0;
        }
        FFT(data, FFT_NSHIFT);
//        PrintComplexArray(data);
    }

    {
        u32     seed;
        for (seed = 0; seed < N_RANDOM_INPUT; seed++)
        {
            MATHRandContext32 rand;

            MATH_InitRand32(&rand, seed);
            for (i = 0; i < FFT_N; i++)
            {
                orig[i * 2] = data[i * 2] =
                    (fx32)(MATH_Rand32(&rand, FFT_VALUE_RANGE) - (FFT_VALUE_RANGE / 2));
                orig[i * 2 + 1] = data[i * 2 + 1] =
                    (fx32)(MATH_Rand32(&rand, FFT_VALUE_RANGE) - (FFT_VALUE_RANGE / 2));
            }
            FFT(data, FFT_NSHIFT);
//            PrintComplexArray(data);
            IFFT(data, FFT_NSHIFT);
            flag = PrintError(orig, data) && flag;
        }
    }

#endif


    return flag;
}

static void PrintRealArray(fx32 *data)
{
    u32     i;
    for (i = 0; i < FFT_N; i++)
    {
        OS_TPrintf("%3x: ", i);
        PrintFX32(data[i]);
        OS_TPrintf("\n");
    }
}

static void PrintComplexArray(fx32 *data)
{
    u32     i;
    for (i = 0; i < FFT_N; i++)
    {
        OS_TPrintf("%3x: ", i);
        PrintFX32(data[i * 2]);
        OS_TPrintf(", ");
        PrintFX32(data[i * 2 + 1]);
        OS_TPrintf("\n");
    }
}

static void PrintFX32(fx32 f)
{
#pragma unused(f)                      // FINALROM Build 時に必要
    OS_Printf("%f", FX_FX32_TO_F32(f));
#if 0
    if (f >= 0)
    {
        OS_TPrintf(" %6d.%03d", f >> FX32_SHIFT, (f & FX32_DEC_MASK) * 1000 / 4096);
    }
    else
    {
        OS_TPrintf("-%6d.%03d", (-f) >> FX32_SHIFT, ((-f) & FX32_DEC_MASK) * 1000 / 4096);
    }
#endif
}

static BOOL PrintError(fx32 *orig, fx32 *data)
{
    u32     i;
    fx32    max_error;
    double  sum_sqd, e;

    max_error = 0;
    sum_sqd = 0;
    for (i = 0; i < FFT_N * 2; i++)
    {
        fx32    d = MATH_ABS(data[i] - orig[i]);
        double  dd = FX_FX32_TO_F32(d);
        double  od = FX_FX32_TO_F32(orig[i]);

        if (d > max_error)
        {
            max_error = d;
        }

        sum_sqd += dd * dd;
    }
    sum_sqd /= FFT_N * 2;
    e = FX_FX32_TO_F32(max_error);
    OS_Printf("Max Error: %f, Dist.^2: %.4g\n", e, sum_sqd);

    return (e <= ERROR_THRETHOLD);
}

static void InitBitRevTable(void)
{
    int     i, j, k;

    i = j = 0;
    for (;;)
    {
        gBitRevTable[i] = j;
        if (++i >= FFT_N)
            break;
        k = FFT_N / 2;
        while (k <= j)
        {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}

static void FFT(fx32 *data, int nShift)
{
    int     i, j, k, ik, h, d, k2;
    fx32    t, s, c, dx, dy;
    u32     n = 1U << nShift;
    OSTick  start, end;

    SDK_ASSERT(n == FFT_N);

    for (i = 0; i < FFT_N; i++)
    {
        gFFTCos[i] = data[i * 2];
        gFFTSin[i] = data[i * 2 + 1];
    }

    start = OS_GetTick();
    for (i = 0; i < FFT_N; i++)
    {                                  /* ビット反転 */
        j = gBitRevTable[i];
        if (i < j)
        {
            t = gFFTCos[i];
            gFFTCos[i] = gFFTCos[j];
            gFFTCos[j] = t;
            t = gFFTSin[i];
            gFFTSin[i] = gFFTSin[j];
            gFFTSin[j] = t;
        }
    }
    for (k = 1; k < FFT_N; k = k2)
    {                                  /* 変換 */
        h = 0;
        k2 = k + k;
        d = FFT_N / k2;
        for (j = 0; j < k; j++)
        {
            c = FX_CosIdx(h * (65536 / FFT_N));
            s = FX_SinIdx(h * (65536 / FFT_N));
            for (i = j; i < FFT_N; i += k2)
            {
                ik = i + k;
                dx = FX_Mul(s, gFFTSin[ik]) + FX_Mul(c, gFFTCos[ik]);
                dy = FX_Mul(c, gFFTSin[ik]) - FX_Mul(s, gFFTCos[ik]);
                gFFTCos[ik] = gFFTCos[i] - dx;
                gFFTCos[i] += dx;
                gFFTSin[ik] = gFFTSin[i] - dy;
                gFFTSin[i] += dy;
            }
            h += d;
        }
    }
    end = OS_GetTick();
    OS_Printf("%lld us, ", OS_TicksToMicroSeconds(end - start));

    for (i = 0; i < FFT_N; i++)
    {
        data[i * 2] = gFFTCos[i] >> nShift;
        data[i * 2 + 1] = gFFTSin[i] >> nShift;
    }
}

static void IFFT(fx32 *data, int nShift)
{
    int     i, j, k, ik, h, d, k2;
    fx32    t, s, c, dx, dy;
    u32     n = 1U << nShift;

    SDK_ASSERT(n == FFT_N);

    for (i = 0; i < FFT_N; i++)
    {
        gFFTCos[i] = data[i * 2];
        gFFTSin[i] = data[i * 2 + 1];
    }

    for (i = 0; i < FFT_N; i++)
    {                                  /* ビット反転 */
        j = gBitRevTable[i];
        if (i < j)
        {
            t = gFFTCos[i];
            gFFTCos[i] = gFFTCos[j];
            gFFTCos[j] = t;
            t = gFFTSin[i];
            gFFTSin[i] = gFFTSin[j];
            gFFTSin[j] = t;
        }
    }
    for (k = 1; k < FFT_N; k = k2)
    {                                  /* 変換 */
        h = 0;
        k2 = k + k;
        d = FFT_N / k2;
        for (j = 0; j < k; j++)
        {
            c = FX_CosIdx(h * (65536 / FFT_N));
            s = FX_SinIdx(h * (65536 / FFT_N));
            for (i = j; i < FFT_N; i += k2)
            {
                ik = i + k;
                dx = -FX_Mul(s, gFFTSin[ik]) + FX_Mul(c, gFFTCos[ik]);
                dy = FX_Mul(c, gFFTSin[ik]) + FX_Mul(s, gFFTCos[ik]);
                gFFTCos[ik] = gFFTCos[i] - dx;
                gFFTCos[i] += dx;
                gFFTSin[ik] = gFFTSin[i] - dy;
                gFFTSin[i] += dy;
            }
            h += d;
        }
    }

    for (i = 0; i < FFT_N; i++)
    {
        data[i * 2] = gFFTCos[i];
        data[i * 2 + 1] = gFFTSin[i];
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
