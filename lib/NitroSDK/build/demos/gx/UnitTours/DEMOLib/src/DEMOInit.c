/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
  File:     DEMOInit.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DEMOInit.c,v $
  Revision 1.57  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.56  2005/11/21 10:24:30  kitase_hirotake
  SVC_WaitVBlankIntr ‚ð OS_WaitVBlankIntr ‚É•ÏX

  Revision 1.55  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.54  2004/08/23 05:09:11  takano_makoto
  add incude DEMOKey.h

  Revision 1.53  2004/08/23 02:25:01  yasu
  Add dummy key read "DEMOReadKey()" in DEMOInitCommon()

  Revision 1.52  2004/07/14 10:56:15  takano_makoto
  Change default ClearColor 0 -> 31

  Revision 1.51  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.50  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.49  2004/02/24 05:26:47  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.48  2004/02/13 11:15:09  nishida_kenji
  Use GX_VBlankIntr(TRUE) instead of a magic number.

  Revision 1.47  2004/02/13 06:10:26  nishida_kenji
  Init OAM and PLTT on the sub engine.

  Revision 1.46  2004/02/06 05:08:26  nishida_kenji
  Exec GX_SetPower(GX_POWER_ALL) in GX_Init().
  Exec G3X_InitTable in G3X_Init().

  Revision 1.45  2004/02/05 23:52:12  nishida_kenji
  GX_SetPower before GX_Init.

  Revision 1.44  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.43  2004/02/02 06:34:21  nishida_kenji
  Add functions for the sub display.

  Revision 1.42  2004/01/29 01:16:16  nishida_kenji
  Remove GX_BlankScr and use GX_DispOn/GX_DispOff instead.

  Revision 1.41  2004/01/18 09:20:26  nishida_kenji
  add DEMOStartDisplay

  Revision 1.40  2004/01/18 07:04:01  nishida_kenji
  GX_SetOBJVRAMModeChar->GX_SetOBJVRamModeChar
  GX_SetOBJVRAMModeBmp->GX_SetOBJVRmaModeBmp

  Revision 1.39  2004/01/16 10:57:21  nishida_kenji
  add FX_Init

  Revision 1.38  2004/01/14 11:35:03  nishida_kenji
  G3X_InitTables->G3X_InitTable

  Revision 1.37  2004/01/13 00:54:53  nishida_kenji
  add G3X_InitTables()

  Revision 1.36  2004/01/13 00:44:10  nishida_kenji
  small fix(comments)

  Revision 1.35  2004/01/09 09:57:16  yada
  VBlank interrupt configuration by OS_xxx API,
  and add OS_Init().

  Revision 1.34  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.33  2003/12/25 11:00:10  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.32  2003/12/25 07:33:05  yada
  renamed OS_IRQxxx to OS_Irqxxx

  Revision 1.31  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  Revision 1.30  2003/12/17 01:29:35  nishida_kenji
  change 2D Blending APIs

  Revision 1.29  2003/12/16 09:57:18  nishida_kenji
  add 'Set' to some functions

  Revision 1.28  2003/12/16 08:08:50  nishida_kenji
  now revising interface for BGxControl

  Revision 1.27  2003/12/16 06:15:36  nishida_kenji
  now revising interface for BGxControl

  Revision 1.26  2003/12/16 02:35:43  nishida_kenji
  revise APIs for OBJ memory mode at DISPCNT

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "DEMOInit.h"
#include "DEMOKey.h"

void DEMOInitCommon(void)
{
    OS_Init();
    FX_Init();

//    GX_SetPower(GX_POWER_ALL);
    GX_Init();

    GX_DispOff();
    GXS_DispOff();

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    DEMOReadKey();                     // Dummy read to save previous key input

    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
}

void DEMOInitVRAM(void)
{
    //---------------------------------------------------------------------------
    // All VRAM banks to LCDC
    //---------------------------------------------------------------------------
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

    //---------------------------------------------------------------------------
    // Clear all LCDC space
    //---------------------------------------------------------------------------
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    //---------------------------------------------------------------------------
    // Disable the banks on LCDC
    //---------------------------------------------------------------------------
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     // clear the standard palette

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
}


void DEMOInitDisplayBG0Only(void)
{

    GX_SetBankForBG(GX_VRAM_BG_128_A); // VRAM-A for BGs

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // graphics mode
                       GX_BGMODE_0,    // BGMODE is 0
                       GX_BG0_AS_2D);  // BG #0 is for 2D

    GX_SetVisiblePlane(GX_PLANEMASK_BG0);       // display only BG #0

    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,        // 256pix x 256pix text
                     GX_BG_COLORMODE_256,       // use 256 colors mode
                     GX_BG_SCRBASE_0x0000,      // screen base offset + 0x0000 is the address for BG #0 screen
                     GX_BG_CHARBASE_0x04000,    // character base offset + 0x04000 is the address for BG #0 characters
                     GX_BG_EXTPLTT_01  // use BGExtPltt slot #0 if BGExtPltt is enabled
        );

    G2_SetBG0Priority(0);

    G2_BG0Mosaic(FALSE);
}

void DEMOInitDisplaySubBG0Only(void)
{

    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);   // VRAM-C for BGs

    GXS_SetGraphicsMode(GX_BGMODE_0);  // BGMODE is 0

    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);      // display only BG #0

    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,       // 256pix x 256pix text
                      GX_BG_COLORMODE_256,      // use 256 colors mode
                      GX_BG_SCRBASE_0x0000,     // screen base offset + 0x0000 is the address for BG #0 screen
                      GX_BG_CHARBASE_0x04000,   // character base offset + 0x04000 is the address for BG #0 characters
                      GX_BG_EXTPLTT_01 // use BGExtPltt slot #0 if BGExtPltt is enabled
        );

    G2S_SetBG0Priority(0);

    G2S_BG0Mosaic(FALSE);
}



void DEMOInitDisplayBG2Only(void)
{
    GX_SetBankForBG(GX_VRAM_BG_128_A); // VRAM-A for BGs

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // graphics mode
                       GX_BGMODE_2,    // BGMODE is 2
                       GX_BG0_AS_2D    // BG #0 is for 2D
        );

    GX_SetVisiblePlane(GX_PLANEMASK_BG2);       // display only BG #2

    G2_SetBG2ControlAffine(GX_BG_SCRSIZE_AFFINE_128x128,        // 128pix x 128pix affine
                           GX_BG_AREAOVER_XLU,  // no repeat(transparent)
                           GX_BG_SCRBASE_0x0000,        // screen base offset + 0x0000 is the address for BG #2 screen
                           GX_BG_CHARBASE_0x04000       // character base offset + 0x04000 is the address for BG #2 characters
        );

    G2_SetBG2Priority(0);

    G2_BG2Mosaic(FALSE);
}


void DEMOInitDisplaySubBG2Only(void)
{
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);   // VRAM-C for BGs

    GXS_SetGraphicsMode(GX_BGMODE_2);  // BGMODE is 2

    GXS_SetVisiblePlane(GX_PLANEMASK_BG2);      // display only BG #2

    G2S_SetBG2ControlAffine(GX_BG_SCRSIZE_AFFINE_128x128,       // 128pix x 128pix affine
                            GX_BG_AREAOVER_XLU, // no repeat(transparent)
                            GX_BG_SCRBASE_0x0000,       // screen base offset + 0x0000 is the address for BG #2 screen
                            GX_BG_CHARBASE_0x04000      // character base offset + 0x04000 is the address for BG #2 characters
        );

    G2S_SetBG2Priority(0);

    G2S_BG2Mosaic(FALSE);
}



void DEMOInitDisplayOBJOnly(void)
{
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);        // VRAM-A for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // graphics mode
                       GX_BGMODE_0,    // BGMODE is 0
                       GX_BG0_AS_2D);  // BG #0 is for 2D

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // display only OBJ

    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);      // 2D mapping mode
}


void DEMOInitDisplaySubOBJOnly(void)
{
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D); // VRAM-D for OBJ

    GXS_SetGraphicsMode(GX_BGMODE_0);  // BGMODE is 0

    GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);      // display only OBJ

    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);     // 2D mapping mode
}

void DEMOInitDisplay3D(void)
{
    G3X_Init();                        // initialize the 3D graphics states
//    G3X_InitTable();                                       // initialize tables

    G3X_InitMtxStack();                // initialize the matrix stack

    GX_SetBankForTex(GX_VRAM_TEX_0_A); // VRAM-A for texture images

    GX_SetBankForBG(GX_VRAM_BG_128_D); // VRAM-D for BGs

    GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);       // VRAM-E for texture palettes

    GX_SetBankForOBJ(GX_VRAM_OBJ_32_FG);        // VRAM-FG for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,    // graphics mode
                       GX_BGMODE_4,    // BGMODE is 4
                       GX_BG0_AS_3D);  // BG #0 is for 3D

    GX_SetVisiblePlane(GX_PLANEMASK_BG0 |       // BG #0 for 3D
                       GX_PLANEMASK_BG1 |       // BG #1 for text
                       GX_PLANEMASK_OBJ // OBJ
        );

    G2_SetBG0Priority(1);

    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256,        // 256pix x 256pix text
                     GX_BG_COLORMODE_16,        // use 16 colors mode
                     GX_BG_SCRBASE_0x3800,      // screen base offset + 0x3800 is the address for BG #1 screen
                     GX_BG_CHARBASE_0x00000,    // character base offset + 0x00000 is the address for BG #1 characters
                     GX_BG_EXTPLTT_01  // use BGExtPltt slot #1 if BGExtPltt is enabled
        );

    G2_SetBG1Priority(0);

    G2_BG1Mosaic(FALSE);

    G3X_SetShading(GX_SHADING_TOON);   // shading mode is toon

    G3X_AntiAlias(TRUE);               // enable antialias(without additional computing costs)

    G2_BlendNone();                    // no 2D alpha blending or brightness change

    //---------------------------------------------------------------------------
    // After you start the geometry engine and the rendering one
    // (by GX_SetPower etc.), G3_SwapBuffers must be called once before use
    //---------------------------------------------------------------------------
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

    //---------------------------------------------------------------------------
    // Set clear color, depth and polygon ID.
    //---------------------------------------------------------------------------
    G3X_SetClearColor(GX_RGB(0, 0, 0), // clear color
                      31,              // clear alpha
                      0x7fff,          // clear depth
                      63,              // clear polygon ID
                      FALSE            // fog
        );

    G3_ViewPort(0, 0, 255, 191);       // Viewport

    //---------------------------------------------------------------------------
    // Set up the projection matrix
    //---------------------------------------------------------------------------
    {
        fx32    right = FX32_ONE;
        fx32    top = FX32_ONE * 3 / 4;
        fx32    near = FX32_ONE;
        fx32    far = FX32_ONE * 400;

        //---------------------------------------------------------------------------
        // Switch MTXMODE to GX_MTXMODE_PROJECTION, and
        // set a projection matrix onto the current projection matrix on the matrix stack
        //---------------------------------------------------------------------------
        G3_Perspective(FX32_SIN30, FX32_COS30,  // sine and cosine of FOVY
                       FX32_ONE * 4 / 3,        // aspect
                       near,           // near
                       far,            // far
                       NULL            // a pointer to a matrix if you use it
            );

        G3_StoreMtx(0);
    }
}

void DEMOInit(void)
{
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();
}

void DEMOStartDisplay(void)
{
    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();
}
