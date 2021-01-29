/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.33  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.32  2005/11/21 10:26:45  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.31  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.30  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.29  2004/11/02 09:34:15  takano_makoto
  fix comment.

  Revision 1.28  2004/05/24 10:29:01  takano_makoto
  autotestコードの修正

  Revision 1.27  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.26  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.25  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.24  2004/02/18 01:09:52  yasu
  add comment for code32.h

  Revision 1.23  2004/02/12 07:06:26  nishida_kenji
  Avoid generating STRB in THUMB mode.

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
// A sample that use BG #2 as affine mode(rotate):
//
// USAGE:
//   UP, DOWN, LEFT, RIGHT: Change the center position of rotation
//   L,R:                   Rotate BG #2
//
// HOWTO:
// 1. Set up the character/palette/screen data same as 2D_CharBG_1.
// 2. Set up a matrix and points by G2_SetBGxAffine().
//---------------------------------------------------------------------------

#include <nitro/code32.h>              // avoid byte access problems
#include <nitro.h>
#include "DEMO.h"
#include "data.h"

static u8 sScrnBuf[SCREEN_SIZE];       // Buffer for screen data(BG #2)

void NitroMain(void)
{
    s16     x_0 = 0, y_0 = 0;
    s16     x_1 = 0, y_1 = 0;

    u16     rotate = 0;

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #2 for affine mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayBG2Only();

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GX_LoadBG2Char(d_64_256_bg_schDT, 0, sizeof(d_64_256_bg_schDT));
    GX_LoadBGPltt(d_64_256_bg_sclDT, 0, sizeof(d_64_256_bg_sclDT));

    {
        int     i, j;
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                sScrnBuf[(i * 16) + j] = (u8)((i * 0x10) + j);
            }
        }
    }
    // Store the data onto the main memory, and invalidate the cache.
    DC_FlushRange(sScrnBuf, sizeof(sScrnBuf));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();

    // DMA Transfer to BG #2 screen
    GX_LoadBG2Scr(sScrnBuf, 0, sizeof(sScrnBuf));

    DEMOStartDisplay();
    //---------------------------------------------------------------------------
    // Main Loop
    //---------------------------------------------------------------------------
    while (1)
    {
        MtxFx22 mtx;
        fx16    sinVal, cosVal;

        DEMOReadKey();

        if (DEMO_IS_PRESS(PAD_KEY_UP))
            y_0 -= 2;
        if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            y_0 += 2;
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            x_0 += 2;
        if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            x_0 -= 2;
        if (DEMO_IS_PRESS(PAD_BUTTON_L))
            rotate -= 256;
        if (DEMO_IS_PRESS(PAD_BUTTON_R))
            rotate += 256;
        if (DEMO_IS_TRIG(PAD_BUTTON_SELECT))
        {
            x_0 = 32;
            y_0 = 32;
            rotate = 0;
        }

        sinVal = FX_SinIdx(rotate);
        cosVal = FX_CosIdx(rotate);

        mtx._00 = (fx32)cosVal;
        mtx._01 = (fx32)sinVal;
        mtx._10 = -(fx32)sinVal;
        mtx._11 = (fx32)cosVal;

#ifdef SDK_AUTOTEST
        x_0 = 10;
        y_0 = 15;
        rotate = 4096;                 // default params for testing.
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xD4F05FD3);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Set up affine transformation of BG #2
        //---------------------------------------------------------------------------
        G2_SetBG2Affine(&mtx,          // a matrix for rotation and scaling
                        x_0, y_0,      // the center of rotation
                        x_1, y_1       // the reference point before rotation and scaling applied
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
