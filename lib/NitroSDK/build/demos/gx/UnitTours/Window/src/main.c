/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Window
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:46:15  kitase_hirotake
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.7  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.6  2004/05/27 09:28:24  takano_makoto
  Adjust indent.

  Revision 1.5  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.4  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.3  2004/03/15 02:03:40  takano_makoto
  modify comments and codes a little.

  Revision 1.2  2004/03/03 02:00:41  terui
  Modify one byte access problem

  Revision 1.1  2004/03/01 10:46:36  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//      A sample which transform window in HBlank period.
//
//      Back ground image is displayed only in transformed window.
//
//      HOWTO:
//      1.      On HBlank interruption, Change window position.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"
#include "data.h"

//---------------------------------------------------------------------------
//      Array for window position per line
//---------------------------------------------------------------------------
static u16 s_WindowSizePerLine[192];

//---------------------------------------------------------------------------
//      Create window horizontal position table per line
//---------------------------------------------------------------------------
static void CreateWindowStyle(void)
{
    int     i;
    u16     left_pos;
    u16     right_pos;
    int     size;

    for (i = 0; i < 96; i++)
    {
        size = (256 * i) / 96;
        left_pos = (u16)(128 - (size / 2));
        right_pos = (u16)(128 + (size / 2));
        s_WindowSizePerLine[i] = (u16)((left_pos << 8) | right_pos);
    }
    for (i = 96; i < 192; i++)
    {
        s_WindowSizePerLine[i] = s_WindowSizePerLine[191 - i];
    }
}

//---------------------------------------------------------------------------
//      HBlank interrupt
//      Set window horizontal position along position table.
//---------------------------------------------------------------------------
static void HBlankIntr(void)
{
    const int count = GX_GetVCount();

    // case not VBlank
    if (count < 192)
    {
        //---------------------------------------------------------------------------
        // Set window horizontal position
        G2_SetWnd0Position(s_WindowSizePerLine[count] >> 8,
                           0, s_WindowSizePerLine[count] & 0xff, 192);
        //---------------------------------------------------------------------------
    }
    // Set flag which checks HBlank interrupt.
    OS_SetIrqCheckFlag(OS_IE_H_BLANK);
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    //---------------------------------------------------------------------------
    // Set Window horizontal position (first line)
    G2_SetWnd0Position(s_WindowSizePerLine[0] >> 8, 0, s_WindowSizePerLine[0] & 0xff, 192);
    //---------------------------------------------------------------------------

    // Set flag which checks VBlank interrupt.
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//---------------------------------------------------------------------------
//      main
//---------------------------------------------------------------------------
void NitroMain(void)
{
    // Initialize
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayBG0Only();

    // setting interrupt
    (void)OS_DisableIrq();             // Disable IRQ interrupt
    OS_SetIrqFunction(OS_IE_H_BLANK,   // Add function on H-Blank interrupt
                      HBlankIntr);
    (void)OS_SetIrqMask(OS_IE_V_BLANK  // Set interrupt setting flag
                        | OS_IE_H_BLANK);
    (void)GX_HBlankIntr(TRUE);         // Rise a H-Blank interrupt
    (void)OS_EnableIrq();              // Enabled IRQ interrupt

    // Load 2D graphic data
    GX_LoadBG0Char(d_natsunoumi_schDT, 0, sizeof(d_natsunoumi_schDT));
    GX_LoadBGPltt(d_natsunoumi_sclDT, 0, sizeof(d_natsunoumi_sclDT));
    GX_LoadBG0Scr(d_natsunoumi_sscDT, 0, sizeof(d_natsunoumi_sscDT));

    // Window setting
    GX_SetVisibleWnd(GX_WNDMASK_W0);   // Turn on WINDOW-0
    // Set inside of window visible BG0
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0, FALSE);
    // Set outside of window unvisible
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);

    // Create window horizontal position table
    CreateWindowStyle();

    DEMOStartDisplay();

    // main loop
    while (1)
    {
        // Wait VBlank
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x7C44DB20);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
