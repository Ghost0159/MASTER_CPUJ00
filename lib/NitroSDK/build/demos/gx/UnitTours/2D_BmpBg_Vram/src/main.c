/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_BmpBg_Vram
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.27  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.26  2005/11/21 10:26:18  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.25  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.24  2004/11/02 09:46:43  takano_makoto
  fix typo.

  Revision 1.23  2004/11/02 09:34:01  takano_makoto
  fix comment.

  Revision 1.22  2004/07/28 07:38:35  yasu
  Let pictures exchangeable

  Revision 1.21  2004/05/24 10:28:39  takano_makoto
  autotestコードの修正

  Revision 1.20  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.19  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.18  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.17  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.16  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.15  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.14  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.13  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.12  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.11  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.10  2003/12/25 11:00:09  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.9  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that displays VRAM image:
//
// If display mode is VRAM display mode, a bitmap image on VRAM is displayed.
// This sample loads different four images to VRAM A-D, and displays them by turns.
//
// HOWTO:
// 1. Set the target VRAM banks to LCDC mode.
// 2. Load images to them (by MI_DmaCopy32()).
// 3. Start to display the VRAM image by GX_SetGraphicsMode(GX_DISPMODE_VRAM_A, ...).
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "image.h"

void NitroMain(void)
{
    int     vram_slot = 0, count = 0;

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, and initialize VRAM.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();

    //---------------------------------------------------------------------------
    // Map VRAM bank A-D onto LCDC.
    //---------------------------------------------------------------------------
    GX_SetBankForLCDC(GX_VRAM_LCDC_A | GX_VRAM_LCDC_B | GX_VRAM_LCDC_C | GX_VRAM_LCDC_D);

    //---------------------------------------------------------------------------
    // Download images
    //---------------------------------------------------------------------------
    MI_DmaCopy32(3, IMAGE_VRAM256x192[0], (void *)HW_LCDC_VRAM_A,
                 256 * 192 * sizeof(unsigned short));
    MI_DmaCopy32(3, IMAGE_VRAM256x192[1], (void *)HW_LCDC_VRAM_B,
                 256 * 192 * sizeof(unsigned short));
    MI_DmaCopy32(3, IMAGE_VRAM256x192[2], (void *)HW_LCDC_VRAM_C,
                 256 * 192 * sizeof(unsigned short));
    MI_DmaCopy32(3, IMAGE_VRAM256x192[3], (void *)HW_LCDC_VRAM_D,
                 256 * 192 * sizeof(unsigned short));

    //---------------------------------------------------------------------------
    // Set graphics mode VRAM display mode
    //---------------------------------------------------------------------------
    GX_SetGraphicsMode(GX_DISPMODE_VRAM_A,      // display VRAM-A
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    DEMOStartDisplay();
    while (1)
    {

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xEFA8B182);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Change the VRAM slot displayed every 90 frames
        //---------------------------------------------------------------------------
        if (count++ > 90)
        {
            vram_slot++;
            vram_slot &= 0x03;
            switch (vram_slot)
            {
            case 0:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_A,  // display VRAM-A
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 1:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_B,  // display VRAM-B
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 2:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,  // display VRAM-C
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 3:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_D,  // display VRAM-D
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            }
            // reset a counter
            count = 0;
        }
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
