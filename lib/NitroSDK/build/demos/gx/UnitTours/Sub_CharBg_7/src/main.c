/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Sub_CharBg_7
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:43:39  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/02/28 05:26:04  yosizaki
  do-indent.

  Revision 1.6  2004/07/06 05:07:42  takano_makoto
  change DmaFill to CpuFill (fix cache bug)

  Revision 1.5  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.4  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.3  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.2  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/02/02 06:27:48  nishida_kenji
  Based on 2D_CharBg_7.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that blends two text BG planes:
//
// USAGE:
//   UP, DOWN, LEFT, RIGHT: Controls blending coefficients
//
// HOWTO:
// 1. Set up the character/palette/screen data same as Sub_CharBG_1.
// 2. Specify the planes blended and blend ratio by G2S_SetBlendAlpha.
// 3. You can also control blend ratio by G2S_ChangeBlendAlpha.
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
    DEMOInitDisplaySubBG0Only();

    //---------------------------------------------------------------------------
    // Setting up BG #0 controls:
    //---------------------------------------------------------------------------
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_256,
                      GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_BG0Mosaic(FALSE);

    //---------------------------------------------------------------------------
    // Setting up BG #1 controls:
    //---------------------------------------------------------------------------
    G2S_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,
                      GX_BG_COLORMODE_256,
                      GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01);

    G2S_SetBG1Priority(1);
    G2S_BG1Mosaic(FALSE);

    //---------------------------------------------------------------------------
    // Transmitting the character data and the palette data
    //---------------------------------------------------------------------------
    GXS_LoadBG0Char(d_al_1_schDT, 0, sizeof(d_al_1_schDT));
    GXS_LoadBG1Char(d_al_2_schDT, 0, sizeof(d_al_2_schDT));
    GXS_LoadBGPltt(d_al_1_sclDT, 0, sizeof(d_al_1_sclDT));

    GXS_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1);   // Displays BG #0 and BG #1

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
    GXS_LoadBG0Scr(sScrnBuf1, 0, sizeof(sScrnBuf1));
    GXS_LoadBG1Scr(sScrnBuf2, 0, sizeof(sScrnBuf2));

    //---------------------------------------------------------------------------
    // Setting up alpha blending:
    //
    // The first plane is BG #0 and the second one is BG #1.
    // Alpha blending occurs if and only if BG #1 is just behind BG #0.
    //---------------------------------------------------------------------------
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0,   // specifies only BG #0
                      GX_BLEND_PLANEMASK_BG1,   // specifies only BG #1
                      0,               // eva
                      0                // evb
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

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Change the coefficients for alpha blending
        //---------------------------------------------------------------------------
        G2S_ChangeBlendAlpha(eva, evb);
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
