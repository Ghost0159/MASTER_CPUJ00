/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_Oam_5
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.32  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.31  2005/11/21 10:31:00  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.30  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.29  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.28  2004/11/02 09:35:52  takano_makoto
  fix typo.

  Revision 1.27  2004/05/25 06:59:58  takano_makoto
  Add FlushRange

  Revision 1.26  2004/05/24 10:30:58  takano_makoto
  autotestコードの修正

  Revision 1.25  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.24  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.23  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.22  2004/02/12 09:33:51  nishida_kenji
  GXOamColor->GXOamColorMode.

  Revision 1.21  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.20  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.19  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.18  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.17  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.16  2004/01/18 07:45:04  nishida_kenji
  small bug fix

  Revision 1.15  2004/01/18 07:33:33  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.14  2004/01/14 11:24:30  nishida_kenji
  add G2_OBJMosaic,
  remove GXOamMosaic

  Revision 1.13  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.12  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.11  2003/12/25 11:00:09  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.10  2003/12/17 12:21:36  nishida_kenji
  use DC_FlushRange

  Revision 1.9  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that mosaics an OBJ:
//
// USAGE:
//   UP, DOWN:    Control the size of mosaic in vertical direction
//   LEFT, RIGHT: Control the size of mosaic in horizontal direction
//
// HOWTO:
// 1. Set up the character/palette/attribute data same as 2D_Oam_1.
// 2. Specify the size of mosaics by G2_SetOBJMosaicSize().
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

static GXOamAttr sOamBak[128];         // Buffer for OAM

void NitroMain(void)
{
    int     mosaic_H = 0, mosaic_V = 0;

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and make OBJ visible.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayOBJOnly();

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GX_LoadOBJ(d_64_256_obj_schDT, 0, sizeof(d_64_256_obj_schDT));
    GX_LoadOBJPltt(d_64_256_obj_sclDT, 0, sizeof(d_64_256_obj_sclDT));

    MI_DmaFill32(3, sOamBak, 192, sizeof(sOamBak));     // let out of the screen if not display

    G2_SetOBJAttr(&sOamBak[0],         // a pointer to the attributes
                  0,                   // x
                  0,                   // y
                  0,                   // priority
                  GX_OAM_MODE_NORMAL,  // OBJ mode
                  TRUE,                // mosaic
                  GX_OAM_EFFECT_NONE,  // flip/affine/no display/affine(double)
                  GX_OAM_SHAPE_64x64,  // size and shape
                  GX_OAM_COLORMODE_256, // OBJ character data are in 256 colors format
                  0,                   // character name
                  0,                   // color param
                  0                    // affine param
        );

    DEMOStartDisplay();
    //---------------------------------------------------------------------------
    // Main Loop
    //---------------------------------------------------------------------------
    while (1)
    {
        DEMOReadKey();

        if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            mosaic_V++;
        if (DEMO_IS_PRESS(PAD_KEY_UP))
            mosaic_V--;
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            mosaic_H++;
        if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            mosaic_H--;
        if (mosaic_H > 15)
            mosaic_H = 15;
        if (mosaic_H < 0)
            mosaic_H = 0;
        if (mosaic_V > 15)
            mosaic_V = 15;
        if (mosaic_V < 0)
            mosaic_V = 0;
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            mosaic_H = 0;
            mosaic_V = 0;
        }

#ifdef SDK_AUTOTEST
        mosaic_V = 8;
        mosaic_H = 9;                  // default params for testing.
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xA6B3F948);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Specify the horizontal/vertical size of mosaics on OBJs
        //---------------------------------------------------------------------------
        G2_SetOBJMosaicSize(mosaic_H, mosaic_V);

        // Store the data onto the main memory, and invalidate the cache.
        DC_FlushRange(sOamBak, sizeof(sOamBak));
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        GX_LoadOAM(sOamBak, 0, sizeof(sOamBak));
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
