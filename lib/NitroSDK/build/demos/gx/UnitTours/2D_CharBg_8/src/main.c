/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_8
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.31  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.30  2005/11/21 10:28:48  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.29  2005/09/29 08:37:00  kitase_hirotake
  ASSERT の回避

  Revision 1.28  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.27  2004/11/02 09:35:11  takano_makoto
  fix typo.

  Revision 1.26  2004/05/24 10:30:05  takano_makoto
  autotestコードの修正

  Revision 1.25  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.24  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.23  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.22  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.21  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.20  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.18  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.17  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.16  2004/01/18 07:12:43  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.15  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.14  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.13  2003/12/25 11:00:09  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.12  2003/12/17 11:35:17  nishida_kenji
  use DC_FlushRange

  Revision 1.11  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that controls two built-in windows:
//
// USAGE:
//   A + [UP, DOWN, LEFT, RIGHT]: Move the position of the window 1.
//   B + [UP, DOWN, LEFT, RIGHT]: Move the position of the window 2.
//   L + [UP, DOWN, LEFT, RIGHT]: Change the size of the window 1.
//   R + [UP, DOWN, LEFT, RIGHT]: Change the size of the window 2.
//
// HOWTO:
// 1. Set windows visible by GX_SetVisibleWnd().
// 2. Specify planes inside a window by G2_SetWndxInsidePlane().
// 3. Specify planes outside windows by G2_SetWndOutsidePlane().
// 4. Set the size and the position of a window by G2_SetWnd0Position().
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

void NitroMain(void)
{
    int     win1_x = 0, win2_x = 50, size1_x = 60, size2_x = 110;
    int     win1_y = 0, win2_y = 50, size1_y = 60, size2_y = 110;

    //---------------------------------------------------------------------------
    // Initialization:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for text mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayBG0Only();

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GX_LoadBG0Char(d_natsunoumi_schDT, 0, sizeof(d_natsunoumi_schDT));
    GX_LoadBGPltt(d_natsunoumi_sclDT, 0, sizeof(d_natsunoumi_sclDT));
    GX_LoadBG0Scr(d_natsunoumi_sscDT, 0, sizeof(d_natsunoumi_sscDT));

    GX_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);    // makes window #0 and window #1 visible

    //---------------------------------------------------------------------------
    // Setting the visible plane inside each window
    //---------------------------------------------------------------------------
    G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0, // BG #0 is visible inside window #0
                          FALSE        // you can limit the zone of alpha blending applied if TRUE
        );

    G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG0, // BG #0 is visible inside window #1
                          FALSE        // you can limit the zone of alpha blending applied if TRUE
        );

    //---------------------------------------------------------------------------
    // Setting the visible plane outside the windows
    //---------------------------------------------------------------------------
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);        // not visible

    DEMOStartDisplay();
    //---------------------------------------------------------------------------
    // Main Loop
    //---------------------------------------------------------------------------
    while (1)
    {
        DEMOReadKey();

        if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
                win1_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
                win1_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                win1_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                win1_x--;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
                win2_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
                win2_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                win2_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                win2_x--;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
                size1_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
                size1_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                size1_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                size1_x--;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP))
                size2_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN))
                size2_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                size2_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                size2_x--;
        }

        if (size1_y > 192)
            size1_y = 192;
        if (size1_x > 255)
            size1_x = 255;
        if (size1_y < 0)
            size1_y = 0;
        if (size1_x < 0)
            size1_x = 0;
        if (size2_y > 192)
            size2_y = 192;
        if (size2_x > 255)
            size2_x = 255;
        if (size2_y < 0)
            size2_y = 0;
        if (size2_x < 0)
            size2_x = 0;

        if (win1_y < 0)
            win1_y = 0;
        if (win1_y + size1_y > 192)
            win1_y = 192 - size1_y;
        if (win2_y < 0)
            win2_y = 0;
        if (win2_y + size2_y > 192)
            win2_y = 192 - size2_y;


#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x61C32E25);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Specifying the position of the window #0
        //---------------------------------------------------------------------------
        G2_SetWnd0Position(win1_x, win1_y,      // the upper left
                           win1_x + size1_x, win1_y + size1_y   // the lower right
            );

        //---------------------------------------------------------------------------
        // Specifying the position of the window #1
        //---------------------------------------------------------------------------
        G2_SetWnd1Position(win2_x, win2_y,      // the upper left
                           win2_x + size2_x, win2_y + size2_y   // the lower right
            );
    }
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
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}
