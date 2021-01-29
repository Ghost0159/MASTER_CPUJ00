/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Edge_Marking
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.34  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.33  2005/11/21 10:33:27  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.32  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.31  2004/11/02 09:36:15  takano_makoto
  fix typo.

  Revision 1.30  2004/05/24 10:31:40  takano_makoto
  autotestコードの修正

  Revision 1.29  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.28  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.27  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.26  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.25  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.24  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.23  2004/01/30 11:34:10  kitani_toshikazu
  Removed Tab codes.

  Revision 1.22  2004/01/30 11:15:14  kitani_toshikazu
  Added DEMO_Set3DDefaultShininessTable() to control  specular reflection.

  Revision 1.21  2004/01/28 06:20:40  kitani_toshikazu
  Adjusted light intensity

  Revision 1.20  2004/01/27 09:00:04  nishida_kenji
  moves G3_LightVector to follow G3_LookAt.

  Revision 1.19  2004/01/22 02:03:25  nishida_kenji
  swap G3_SwapBuffers and SVC_WaitVBlankIntr

  Revision 1.18  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.17  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.16  2004/01/18 08:19:33  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.15  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.14  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.13  2004/01/05 12:05:03  nishida_kenji
  revises comments

  Revision 1.12  2003/12/26 06:58:31  nishida_kenji
  revise comments

  Revision 1.11  2003/12/25 11:00:09  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.10  2003/12/25 06:25:02  nishida_kenji
  revise a part of geometry command APIs

  Revision 1.9  2003/12/25 00:19:29  nishida_kenji
  convert INLINE to inline

  Revision 1.8  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample for edge marking:
//
// Display three rotating cubes with edge marking in three colors.
//
// HOWTO:
// 1. Enable edge marking by G3X_EdgeMarking(TRUE).
// 2. Set up the edge color table by G3X_SetEdgeColorTable(table).
//    The color table[n] is used for the polygons with polygonID = 8*n to 8*n + 7.
// 3. Render polygons.
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

VecFx10 gCubeNormal[8] = {
    GX_VECFX10(FX32_SQRT1_3, FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, -FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(FX32_SQRT1_3, -FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, FX32_SQRT1_3, -FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, -FX32_SQRT1_3, FX32_SQRT1_3),
    GX_VECFX10(-FX32_SQRT1_3, -FX32_SQRT1_3, -FX32_SQRT1_3)
};

inline void vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}

inline void normal(int idx)
{
    G3_Direct1(G3OP_NORMAL, gCubeNormal[idx]);
    // use G3_Normal(x, y, z) if not packed
}


static void NrmVtxQuad(int idx0, int idx1, int idx2, int idx3)
{
    normal(idx0);
    vtx(idx0);
    normal(idx1);
    vtx(idx1);
    normal(idx2);
    vtx(idx2);
    normal(idx3);
    vtx(idx3);
}


GXRgb   gEdgeColor[8] = {
    GX_RGB(31, 0, 0),
    GX_RGB(0, 31, 0),
    GX_RGB(0, 0, 31),
    GX_RGB(0, 0, 0),
    GX_RGB(0, 0, 0),
    GX_RGB(0, 0, 0),
    GX_RGB(0, 0, 0),
    GX_RGB(0, 0, 0)
};

static void drawCube(int polygonID, u16 Rotate)
{
    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    // Set the Material As basic whilte 
    DEMO_Set3DDefaultMaterial(TRUE, TRUE);
    DEMO_Set3DDefaultShininessTable();

    G3_PolygonAttr(GX_LIGHTMASK_0,     // Light #0 is on
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   polygonID,          // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   0                   // OR of GXPolygonAttrMisc's value
        );


    G3_Begin(GX_BEGIN_QUADS);
    {
        NrmVtxQuad(2, 0, 4, 6);
        NrmVtxQuad(7, 5, 1, 3);
        NrmVtxQuad(6, 4, 5, 7);
        NrmVtxQuad(3, 1, 0, 2);
        NrmVtxQuad(5, 4, 0, 1);
        NrmVtxQuad(6, 7, 3, 2);
    }
    G3_End();
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

    G3X_EdgeMarking(TRUE);             // Enable edge marking



    //---------------------------------------------------------------------------
    // Set up the edge color table:
    //
    // Use different edge colors from each other
    //---------------------------------------------------------------------------
    G3X_SetEdgeColorTable(&gEdgeColor[0]);

    DEMOStartDisplay();
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        //---------------------------------------------------------------------------
        // Set up camera matrix
        //---------------------------------------------------------------------------
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Eye Position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);
        }

        //---------------------------------------------------------------------------
        // Set up light colors and direction.
        // Notice that light vector is transformed by the current vector matrix
        // immediately after LightVector command is issued.
        //
        // GX_LIGHTID_0: white, downward
        //---------------------------------------------------------------------------
        G3_LightVector(GX_LIGHTID_0, FX16_SQRT1_3, -FX16_SQRT1_3, -FX16_SQRT1_3);
        G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

        G3_PushMtx();
        G3_Translate(0, 0, -5 * FX32_ONE);

        //---------------------------------------------------------------------------
        // Draw cube #1 with polygonID= 0
        //---------------------------------------------------------------------------
        G3_PushMtx();
        G3_Translate(-1 * (FX32_ONE >> 1), -1 * (FX32_ONE >> 1), 0);
        drawCube(0, Rotate);
        G3_PopMtx(1);

        //---------------------------------------------------------------------------
        // Draw cube #2 with polygonID= 8
        //---------------------------------------------------------------------------
        G3_PushMtx();
        G3_Translate(1 * (FX32_ONE >> 1), -1 * (FX32_ONE >> 1), 0);
        drawCube(8, Rotate);
        G3_PopMtx(1);

        //---------------------------------------------------------------------------
        // Draw cube #3 with polygonID= 16
        //---------------------------------------------------------------------------
        G3_PushMtx();
        G3_Translate(0, 1 * (FX32_ONE >> 1), -2 * FX32_ONE);
        drawCube(16, Rotate);
        G3_PopMtx(1);

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x4AD014FA);
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
