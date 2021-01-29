/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Window_HDMA
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.12  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.11  2005/11/21 10:46:28  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.10  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.9  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.8  2004/08/10 13:51:32  takano_makoto
  Give priority of main memory access to ARM9, for HDMA.

  Revision 1.7  2004/06/21 09:50:15  terui
  Change logic not to use DMA before load function has finished.

  Revision 1.6  2004/05/27 09:28:36  takano_makoto
  Add Autotest code.

  Revision 1.5  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.4  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.3  2004/03/15 02:03:46  takano_makoto
  modify comments and codes a little.

  Revision 1.2  2004/03/03 02:00:31  terui
  Modify one byte access problem

  Revision 1.1  2004/03/01 10:47:21  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//  A sample which transform window by HDMA.
//
//  Back ground image is displayed only in transformed window.
//
//  HOWTO:
//  1.  Set HDMA copy function in VBlank period.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"
#include "data.h"


//---------------------------------------------------------------------------
//  DMA No. for window transforming HDMA
//---------------------------------------------------------------------------
#define WINDOW_TRANSFORM_HDMA_NO    3

//---------------------------------------------------------------------------
//  Array for window position per line
//---------------------------------------------------------------------------
u16     s_WindowSizePerLine[192];
BOOL    preparedGraphic;

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
    // Don't use DMA No.3 before "Load graphic data" function finished.
    if (preparedGraphic)
    {
        // Stop HDMA
        MI_StopDma(WINDOW_TRANSFORM_HDMA_NO);
        // Set Window horizontal position ( first line )
        G2_SetWnd0Position(s_WindowSizePerLine[0] >> 8, 0, s_WindowSizePerLine[0] & 0x0ff, 192);
        // Start HDMA which copy window position array to "Window size register"
        MI_HBlankDmaCopy16(WINDOW_TRANSFORM_HDMA_NO,
                           &(s_WindowSizePerLine[1]), (void *)REG_WIN0H_ADDR, 2);
    }

    // Set flag which checks VBlank interrupt
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//---------------------------------------------------------------------------
//  Create window horizontal position table per line
//---------------------------------------------------------------------------
static void CreateWindowStyle(void)
{
    int     i;
    int     size;
    u16     left_pos;
    u16     right_pos;

    (void)OS_DisableIrq();
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
    (void)OS_EnableIrq();

    // Store the data onto the main memory, and invalidate the cache.
    DC_FlushRange((void *)s_WindowSizePerLine, sizeof(s_WindowSizePerLine));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
}

//---------------------------------------------------------------------------
//  main
//---------------------------------------------------------------------------
void NitroMain(void)
{
    // Initialize
    preparedGraphic = FALSE;
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplayBG0Only();

    MI_SetMainMemoryPriority(MI_PROCESSOR_ARM9);

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

    // Turn on flag that notify graphic data has been loaded.
    preparedGraphic = TRUE;

    DEMOStartDisplay();

    // main loop
    while (1)
    {
        // Wait VBlank
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x7490C85D);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
