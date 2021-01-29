/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Sub_CharBg_8
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.7  2005/11/21 10:43:50  kitase_hirotake
  SVC_WaitVBlankIntr ‚ð OS_WaitVBlankIntr ‚É•ÏX

  Revision 1.6  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.5  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.4  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.3  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.2  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/02/02 06:28:42  nishida_kenji
  Based on 2D_CharBg_8.

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
// 1. Set windows visible by GXS_SetVisibleWnd().
// 2. Specify planes inside a window by G2S_SetWndxInsidePlane().
// 3. Specify planes outside windows by G2S_SetWndOutsidePlane().
// 4. Set the size and the position of a window by G2S_SetWnd0Position().
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
    DEMOInitDisplaySubBG0Only();

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GXS_LoadBG0Char(d_natsunoumi_schDT, 0, sizeof(d_natsunoumi_schDT));
    GXS_LoadBGPltt(d_natsunoumi_sclDT, 0, sizeof(d_natsunoumi_sclDT));
    GXS_LoadBG0Scr(d_natsunoumi_sscDT, 0, sizeof(d_natsunoumi_sscDT));

    GXS_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);   // makes window #0 and window #1 visible

    //---------------------------------------------------------------------------
    // Setting the visible plane inside each window
    //---------------------------------------------------------------------------
    G2S_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0,        // BG #0 is visible inside window #0
                           FALSE       // you can limit the zone of alpha blending applied if TRUE
        );

    G2S_SetWnd1InsidePlane(GX_WND_PLANEMASK_BG0,        // BG #0 is visible inside window #1
                           FALSE       // you can limit the zone of alpha blending applied if TRUE
        );

    //---------------------------------------------------------------------------
    // Setting the visible plane outside the windows
    //---------------------------------------------------------------------------
    G2S_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, FALSE);       // not visible

    DEMOStartDisplay();
    //---------------------------------------------------------------------------
    // Main Loop
    //---------------------------------------------------------------------------
    while (1)
    {
        DEMOReadKey();

        if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP) && win1_y > 0)
                win1_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN) && win1_y + size1_y < 192)
                win1_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                win1_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                win1_x--;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP) && win2_y > 0)
                win2_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN) && win2_y + size2_y < 192)
                win2_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                win2_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                win2_x--;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP) && size1_y > 0)
                size1_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN) && size1_y + win1_y < 192)
                size1_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                size1_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                size1_x--;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            if (DEMO_IS_PRESS(PAD_KEY_UP) && size2_y > 0)
                size2_y--;
            if (DEMO_IS_PRESS(PAD_KEY_DOWN) && size2_y + win2_y < 192)
                size2_y++;
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
                size2_x++;
            if (DEMO_IS_PRESS(PAD_KEY_LEFT))
                size2_x--;
        }

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Specifying the position of the window #0
        //---------------------------------------------------------------------------
        G2S_SetWnd0Position(win1_x, win1_y,     // the upper left
                            win1_x + size1_x, win1_y + size1_y  // the lower right
            );

        //---------------------------------------------------------------------------
        // Specifying the position of the window #1
        //---------------------------------------------------------------------------
        G2S_SetWnd1Position(win2_x, win2_y,     // the upper left
                            win2_x + size2_x, win2_y + size2_y  // the lower right
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
