/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 10:58:08  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.1  2005/05/17 13:01:19  seiki_masashi
  変換関数の追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  RTC の変換関数動作確認デモ
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <string.h>


static void VBlankIntr(void);
static void DisplayInit(void);
static void FillScreen(u16 col);
static BOOL ConvTest(void);
static void FormatDateTime(char *str, const RTCDate *date, const RTCTime *time);

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

    if (ConvTest())
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
  Name:         ConvTest

  Description:  RTC 変換関数のテストルーチン

  Arguments:    None.

  Returns:      テストが成功したら true.
 *---------------------------------------------------------------------------*/
static char *sWeekName[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
#define PrintResultEq( a, b, f ) \
    { OS_TPrintf( ((a) == (b)) ? "[--OK--] " : "[**NG**] " ); (f) = (f) && ((a) == (b)); }
#define PrintResultStringEq( a, b, f ) \
    { OS_TPrintf( (strcmp((a), (b)) == 0) ? "[--OK--] " : "[**NG**] " ); (f) = (f) && (strcmp((a), (b)) == 0); }

static BOOL ConvTest(void)
{
    int     i;
    BOOL    flag = TRUE;

    {
        s64     a[] = {
            0, 3155759999LL, -1, 3155760000LL,
            5097600, 5184000, 154335600,
        };
        char   *result_str[] = {
            "2000/01/01(Sat) 00:00:00", "2099/12/31(Thu) 23:59:59",
            "2000/01/01(Sat) 00:00:00", "2099/12/31(Thu) 23:59:59",
            "2000/02/29(Tue) 00:00:00", "2000/03/01(Wed) 00:00:00",
            "2004/11/21(Sun) 07:00:00",
        };
        s64     result_sec[] = {
            0, 3155759999LL, 0, 3155759999LL,
            5097600, 5184000, 154335600,
        };
        for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        {
            RTCDate date;
            RTCTime time;
            char    datestr[64];
            s64     result;

            RTC_ConvertSecondToDateTime(&date, &time, a[i]);
            FormatDateTime(datestr, &date, &time);
            PrintResultStringEq(datestr, result_str[i], flag);
            OS_TPrintf("RTC_ConvertSecondToDateTime(%lld) = %s\n", a[i], datestr);
            result = RTC_ConvertDateTimeToSecond(&date, &time);
            PrintResultEq(result, result_sec[i], flag);
            OS_TPrintf("RTC_ConvertDateTimeToSecond(%s) = %lld\n", datestr, result);
        }
    }


    return flag;
}

static void FormatDateTime(char *str, const RTCDate *date, const RTCTime *time)
{
    (void)OS_SPrintf(str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
                     date->year + 2000, date->month, date->day, sWeekName[date->week],
                     time->hour, time->minute, time->second);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
