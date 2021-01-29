/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/2D_CharBg_7
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.37  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.36  2005/11/21 10:28:33  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.35  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.34  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.33  2004/11/02 09:35:04  takano_makoto
  fix typo.

  Revision 1.32  2004/07/06 04:51:05  yosizaki
  change DmaFill to CpuFill (fix cache bug)

  Revision 1.31  2004/05/24 10:29:58  takano_makoto
  autotestコードの修正

  Revision 1.30  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.29  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.28  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.27  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.26  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.25  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.24  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.23  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.22  2004/01/18 07:12:43  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.21  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.20  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.19  2003/12/25 11:00:09  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.18  2003/12/17 11:35:17  nishida_kenji
  use DC_FlushRange

  Revision 1.17  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that blends two text BG planes:
//
// USAGE:
//   UP, DOWN, LEFT, RIGHT: Controls blending coefficients
//
// HOWTO:
// 1. Set up the character/palette/screen data same as 2D_CharBG_1.
// 2. Specify the planes blended and blend ratio by G2_SetBlendAlpha.
// 3. You can also control blend ratio by G2_ChangeBlendAlpha.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

static u16 sScrnBuf1[SCREEN_SIZE];     // Buffer for screen data(BG #0)
static u16 sScrnBuf2[SCREEN_SIZE];     // Buffer for screen data(BG #1)

void NitroMain(void)
{

    int     eva = 0, evb = 0;

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for text mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayBG0Only();

    //---------------------------------------------------------------------------
    // Setting up BG #0 controls:
    //---------------------------------------------------------------------------
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_256,
                     GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);

    //---------------------------------------------------------------------------
    // Setting up BG #1 controls:
    //---------------------------------------------------------------------------
    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_256,
                     GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01);

    G2_SetBG1Priority(1);
    G2_BG1Mosaic(FALSE);

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GX_LoadBG0Char(d_al_1_schDT, 0, sizeof(d_al_1_schDT));
    GX_LoadBG1Char(d_al_2_schDT, 0, sizeof(d_al_2_schDT));
    GX_LoadBGPltt(d_al_1_sclDT, 0, sizeof(d_al_1_sclDT));

    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1);    // Displays BG #0 and BG #1

    {
        int     i, j;

        for (i = 0; i < 18; i++)
        {
            for (j = 0; j < 12; j++)
            {
                sScrnBuf1[((i + 3) * 32) + (j + 11)] = (u16)((i * 0x10) + j);
            }
        }
        // Store the data onto the main memory, and invalidate the cache.
        DC_FlushRange(sScrnBuf1, sizeof(sScrnBuf1));

        MI_CpuFill16(sScrnBuf2, 0xffff, SCREEN_SIZE * sizeof(u16));

        for (i = 0; i < 13; i++)
        {
            for (j = 0; j < 16; j++)
            {
                sScrnBuf2[((i + 6) * 32) + (j + 9)] = (u16)((i * 0x10) + j);
            }
        }
        // Store the data onto the main memory, and invalidate the cache.
        DC_FlushRange(sScrnBuf2, sizeof(sScrnBuf2));
    }

    // DMA Transfer to BG0 and BG1 screen
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(sScrnBuf1, 0, sizeof(sScrnBuf1));
    GX_LoadBG1Scr(sScrnBuf2, 0, sizeof(sScrnBuf2));

    //---------------------------------------------------------------------------
    // Setting up alpha blending:
    //
    // The first plane is BG #0 and the second one is BG #1.
    // Alpha blending occurs if and only if BG #1 is just behind BG #0.
    //---------------------------------------------------------------------------
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0,    // specifies only BG #0
                     GX_BLEND_PLANEMASK_BG1,    // specifies only BG #1
                     0,                // eva
                     0                 // evb
        );

    DEMOStartDisplay();
    //---------------------------------------------------------------------------
    // Main Loop
    //---------------------------------------------------------------------------
    while (1)
    {
        DEMOReadKey();

        if (DEMO_IS_PRESS(PAD_KEY_UP))
            eva++;
        if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            eva--;
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            evb++;
        if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            evb--;
        if (eva < 0x00)
            eva = 0x00;
        if (eva > 0x10)
            eva = 0x10;
        if (evb < 0x00)
            evb = 0x00;
        if (evb > 0x10)
            evb = 0x10;

#ifdef SDK_AUTOTEST
        eva = 0x08;
        evb = 0x09;                    // default params for testing.
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x2A2878AE);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Change the coefficients for alpha blending
        //---------------------------------------------------------------------------
        G2_ChangeBlendAlpha(eva, evb);
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
