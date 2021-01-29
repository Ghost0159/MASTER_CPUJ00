/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Sub_CharBg_9
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

  Revision 1.7  2005/11/21 10:44:03  kitase_hirotake
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

  Revision 1.1  2004/02/02 06:29:21  nishida_kenji
  Based on 2D_CharBg_9.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that controls brightness:
//
// USAGE:
//   UP, DOWN: Controls brightness
//
// HOWTO:
// 1. Set up the character/palette/screen data same as Sub_CharBG_1.
// 2. Specify the planes and brightness by G2S_SetBlendBrightness().
// 3. You can also control brightness by G2S_ChangeBlendBrightness().
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

void NitroMain(void)
{
    int     evy = 0;

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for text mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplaySubBG0Only();

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GXS_LoadBG0Char(d_yuuyake_schDT, 0, sizeof(d_yuuyake_schDT));
    GXS_LoadBGPltt(d_yuuyake_sclDT, 0, sizeof(d_yuuyake_sclDT));

    //---------------------------------------------------------------------------
    // Transmitting the screen data
    //---------------------------------------------------------------------------
    GXS_LoadBG0Scr(d_yuuyake_sscDT, 0, sizeof(d_yuuyake_sscDT));

    //---------------------------------------------------------------------------
    // Setting up brightness control
    //
    // Specify planes and brightness(-16...+16)
    //---------------------------------------------------------------------------
    G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG0, 0);

    DEMOStartDisplay();
    //---------------------------------------------------------------------------
    // Main Loop
    //---------------------------------------------------------------------------
    while (1)
    {
        DEMOReadKey();

        if (DEMO_IS_PRESS(PAD_KEY_UP))
            evy++;
        if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            evy--;

        if (evy < -16)
            evy = -16;
        if (evy > 16)
            evy = 16;

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Change the coefficient for brightness
        //---------------------------------------------------------------------------
        G2S_ChangeBlendBrightness(evy);
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
