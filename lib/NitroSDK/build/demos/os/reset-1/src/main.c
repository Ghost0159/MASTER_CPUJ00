/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - reset-1
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:55:22  kitase_hirotake
  OS_InitPrintServer の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/02/28 05:26:10  yosizaki
  do-indent.

  Revision 1.6  2004/09/28 12:26:06  yada
  not call PXI_Init() because OS_Init() called

  Revision 1.5  2004/09/01 09:54:16  yada
  add gimmick to clear screen and reset

  Revision 1.4  2004/09/01 09:34:21  yada
  delete unused define

  Revision 1.3  2004/09/01 08:41:12  yada
  fix a little

  Revision 1.2  2004/09/01 08:37:31  yada
  OS_Reset -> OS_ResetSystem

  Revision 1.1  2004/09/01 04:40:38  yada
  initial release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include  <nitro.h>
#include  <nitro/rtc.h>
#include  "font.h"
#include  "screen.h"

//---- for RTC
static RTCTime myCurrentTime;
static int myResult;

static void myInit(void);
static void myVBlankIntr(void);

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    u32     param = OS_GetResetParameter();
    u16     trigger;
    u16     preButton = PAD_Read();
    u16     button;

    //---------------- initialize
    myInit();
    RTC_Init();

    //---------------- main loop
    while (TRUE)
    {
        button = PAD_Read();
        trigger = (u16)((button ^ preButton) & button);
        preButton = button;

        //---- clear screen buffer
        ClearScreen();

        //---- display key description
        PrintString(3, 2, 15, "test sample");
        PrintString(3, 3, 15, "parameter is %d", param);
        PrintString(3, 5, 15, "Push [A] or [B] to reset");
        PrintString(3, 6, 15, "[A] to reset immediately");
        PrintString(3, 7, 15, "[B] to clear screen and");
        PrintString(3, 8, 15, "    reset");

        //---- display time
        myResult = RTC_GetTime(&myCurrentTime);
        if (myResult == 0 /*no error */ )
        {
            PrintString(5, 20, 8, "%02d:%02d:%02d",
                        myCurrentTime.hour, myCurrentTime.minute, myCurrentTime.second);
        }


        //---- display counter
        PrintString(18, 20, 4, "%08X", OS_GetVBlankCount());


        //---- push A to reset (reset immediately)
        if (trigger & PAD_BUTTON_A)
        {
            OS_ResetSystem(param + 1);
        }

        //---- push B to reset (clear screen and reset)
        if (trigger & PAD_BUTTON_B)
        {
            //---- clear screen
            OS_WaitVBlankIntr();
            ClearScreen();
            OS_WaitVBlankIntr();
            OS_WaitVBlankIntr();
            (void)PM_SetLCDPower(PM_LCD_POWER_OFF);

            //---- wait a moment 
            OS_Sleep(300);

            OS_ResetSystem(param + 1);
        }


        OS_WaitVBlankIntr();
    }
}

//----------------------------------------------------------------
//  myInit
//
void myInit(void)
{
    //---- init
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    //---- init displaying
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    //---- setting 2D for top screen
    GX_SetBankForBG(GX_VRAM_BG_128_A);

    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);

    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));



    //---- setting 2D for bottom screen
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);

    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_BG0Mosaic(FALSE);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);

    GXS_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));


    //---- screen
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
    GXS_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    //---- init interrupt
    OS_SetIrqFunction(OS_IE_V_BLANK, myVBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    //---- start displaying
    GX_DispOn();
    GXS_DispOn();
}

//----------------------------------------------------------------
//  myVBlankIntr
//             vblank interrupt handler
//
static void myVBlankIntr(void)
{
    //---- upload pseudo screen to VRAM
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
    GXS_LoadBG0Scr(gScreen, 0, sizeof(gScreen));


    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
