/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Pol_Color
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

  Revision 1.31  2005/11/21 10:34:46  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.30  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.29  2004/11/02 09:36:30  takano_makoto
  fix typo.

  Revision 1.28  2004/05/24 10:31:59  takano_makoto
  autotestコードの修正

  Revision 1.27  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.26  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.25  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.24  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.23  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.22  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.21  2004/01/22 02:03:25  nishida_kenji
  swap G3_SwapBuffers and SVC_WaitVBlankIntr

  Revision 1.20  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.19  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.18  2004/01/18 08:19:33  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.17  2004/01/13 08:08:13  nishida_kenji
  simplification

  Revision 1.16  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.15  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.14  2004/01/05 12:05:03  nishida_kenji
  revises comments

  Revision 1.13  2003/12/26 06:58:31  nishida_kenji
  revise comments

  Revision 1.12  2003/12/25 11:00:10  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.11  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample for vertex color specification.
//
// The left cube has a color for each plane,
// and the right cube has a color for each vertex.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"

s16     gCubeGeometry[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

GXRgb   gCubeColor[8] = {
    GX_RGB(31, 31, 31),
    GX_RGB(31, 31, 0),
    GX_RGB(31, 0, 31),
    GX_RGB(31, 0, 0),
    GX_RGB(0, 31, 31),
    GX_RGB(0, 31, 0),
    GX_RGB(0, 0, 31),
    GX_RGB(0, 0, 0)
};

static void Color(int idx)
{
    G3_Color(gCubeColor[idx]);
}

static void Vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}

static void Quad(int idx0, int idx1, int idx2, int idx3)
{
    Vtx(idx0);
    Vtx(idx1);
    Vtx(idx2);
    Vtx(idx3);
}

static void ColVtxQuad(int idx0, int idx1, int idx2, int idx3)
{
    Color(idx0);
    Vtx(idx0);
    Color(idx1);
    Vtx(idx1);
    Color(idx2);
    Vtx(idx2);
    Color(idx3);
    Vtx(idx3);
}

static void drawLeftCube(u16 Rotate)
{
    G3_PushMtx();

    // Rotate and translate
    G3_Translate(-3 << (FX32_SHIFT - 1), 0, 0);
    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // disable lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   0                   // OR of GXPolygonAttrMisc's value
        );

    //---------------------------------------------------------------------------
    // Draw a cube:
    // Specify different colors for the planes.
    //---------------------------------------------------------------------------

    G3_Begin(GX_BEGIN_QUADS);

    {
        Color(3);
        Quad(2, 0, 4, 6);

        Color(4);
        Quad(7, 5, 1, 3);

        Color(5);
        Quad(6, 4, 5, 7);

        Color(2);
        Quad(3, 1, 0, 2);

        Color(6);
        Quad(5, 4, 0, 1);

        Color(1);
        Quad(6, 7, 3, 2);
    }

    G3_End();

    G3_PopMtx(1);
}

static void drawRightCube(u16 Rotate)
{
    G3_PushMtx();

    // Rotate and translate
    G3_Translate(3 << (FX32_SHIFT - 1), 0, 0);

    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // disable lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   0                   // OR of GXPolygonAttrMisc's value
        );

    //---------------------------------------------------------------------------
    // Draw a cube:
    // Specify different colors for the vertices.
    //---------------------------------------------------------------------------
    G3_Begin(GX_BEGIN_QUADS);
    {
        ColVtxQuad(2, 0, 4, 6);
        ColVtxQuad(7, 5, 1, 3);
        ColVtxQuad(6, 4, 5, 7);
        ColVtxQuad(3, 1, 0, 2);
        ColVtxQuad(5, 4, 0, 1);
        ColVtxQuad(6, 7, 3, 2);
    }
    G3_End();

    G3_PopMtx(1);

}


void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating cubes(0-65535)

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for 3D mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    DEMOStartDisplay();
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        //---------------------------------------------------------------------------
        // Set up camera matrix
        //---------------------------------------------------------------------------
        {
            VecFx32 Eye = { 0, 0, FX32_ONE * 5 };       // Eye Position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);
        }

        G3_PushMtx();

        drawLeftCube(Rotate);
        drawRightCube(Rotate);

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xD0FF2B5B);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt
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
