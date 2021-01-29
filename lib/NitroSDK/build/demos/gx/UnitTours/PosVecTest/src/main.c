/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/PosVecTest
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.10  2005/11/21 10:42:04  kitase_hirotake
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.9  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.8  2004/11/02 08:27:11  takano_makoto
  fix comment.

  Revision 1.7  2004/05/27 09:26:54  takano_makoto
  Add Autotest code.

  Revision 1.6  2004/05/25 07:06:26  takano_makoto
  Remove Code for Ensata, for version up of ensata.

  Revision 1.5  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.4  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.3  2004/03/15 02:02:40  takano_makoto
  modify comments and codes a little.

  Revision 1.2  2004/03/03 02:04:07  terui
  Measure to THUMB compiler problem

  Revision 1.1  2004/03/01 10:43:52  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//  A sample to do "VectorTest" and "PositionTest".
//
//  One cube is displayed.
//  And you can do "VectorTest" and "PositionTest", and can see result.
//
//  HOWTO:
//  1.  Call "G3_PositionTest" to start PositionTest.
//  2.  Call "G3X_GetPositionTestResult" to get result of test.
//  3.  Call "G3_VectorTest" to start VectorTest.
//  4.  Call "G3X_GetVectorTestResult" to get result of test.
//
//  OPERATION:
//  1.  Push A button to do PositionTest.
//  2.  Push B button to do VectorTest.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"
#include "tex_32768.h"

//---------------------------------------------------------------------------
//  Cube model data
//---------------------------------------------------------------------------
// Vertex data
static s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};
// Normal data
static VecFx10 s_Normal[6] = {
    GX_VECFX10(0, 0, FX32_ONE - 1),
    GX_VECFX10(0, FX32_ONE - 1, 0),
    GX_VECFX10(FX32_ONE - 1, 0, 0),
    GX_VECFX10(0, 0, -FX32_ONE + 1),
    GX_VECFX10(0, -FX32_ONE + 1, 0),
    GX_VECFX10(-FX32_ONE + 1, 0, 0)
};
// Texture coordinate data
static GXSt s_TextureCoord[] = {
    GX_ST(0, 0),
    GX_ST(0, 64 * FX32_ONE),
    GX_ST(64 * FX32_ONE, 0),
    GX_ST(64 * FX32_ONE, 64 * FX32_ONE)
};

//---------------------------------------------------------------------------
//  Set vertex coordinate.
//  input:
//      idx:  ID of vertex data
//---------------------------------------------------------------------------
inline void Vertex(int idx)
{
    G3_Vtx(s_Vertex[idx * 3], s_Vertex[idx * 3 + 1], s_Vertex[idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  Set normal setting.
//  input:
//      idx:  ID of normal data
//---------------------------------------------------------------------------
inline void Normal(int idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[idx]);
}

//---------------------------------------------------------------------------
//  Set texture coordinate.
//  input:
//      idx:  ID of texture data
//---------------------------------------------------------------------------
inline void TextureCoord(int idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[idx]);
}

//---------------------------------------------------------------------------
//  Draw a cube and set texture.
//---------------------------------------------------------------------------
static void DrawCube(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // start to set vertex.(use square polygon)
    {
        TextureCoord(1);
        Normal(0);
        Vertex(2);
        TextureCoord(0);
        Normal(0);
        Vertex(0);
        TextureCoord(2);
        Normal(0);
        Vertex(4);
        TextureCoord(3);
        Normal(0);
        Vertex(6);

        TextureCoord(1);
        Normal(3);
        Vertex(7);
        TextureCoord(0);
        Normal(3);
        Vertex(5);
        TextureCoord(2);
        Normal(3);
        Vertex(1);
        TextureCoord(3);
        Normal(3);
        Vertex(3);

        TextureCoord(1);
        Normal(5);
        Vertex(6);
        TextureCoord(0);
        Normal(5);
        Vertex(4);
        TextureCoord(2);
        Normal(5);
        Vertex(5);
        TextureCoord(3);
        Normal(5);
        Vertex(7);

        TextureCoord(1);
        Normal(2);
        Vertex(3);
        TextureCoord(0);
        Normal(2);
        Vertex(1);
        TextureCoord(2);
        Normal(2);
        Vertex(0);
        TextureCoord(3);
        Normal(2);
        Vertex(2);

        TextureCoord(1);
        Normal(1);
        Vertex(5);
        TextureCoord(0);
        Normal(1);
        Vertex(4);
        TextureCoord(2);
        Normal(1);
        Vertex(0);
        TextureCoord(3);
        Normal(1);
        Vertex(1);

        TextureCoord(1);
        Normal(4);
        Vertex(6);
        TextureCoord(0);
        Normal(4);
        Vertex(7);
        TextureCoord(2);
        Normal(4);
        Vertex(3);
        TextureCoord(3);
        Normal(4);
        Vertex(2);
    }
    G3_End();                          // end
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
    // Set flag which checks VBlank interrupt.
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//---------------------------------------------------------------------------
//  main
//---------------------------------------------------------------------------
#ifdef  SDK_CW_WA_CONSTPOOLS
#pragma optimization_level 1
#endif
void NitroMain(void)
{
    unsigned int count = 0;
    u32     myTexAddr = 0x00000;       // Address of texture image slot
    u16     Rotate = 0;

    // Initialize
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    // Load texture image into texture image slot.
    GX_BeginLoadTex();
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // Src address
                   myTexAddr,          // Destination address
                   8192);              // Size to load
    }
    GX_EndLoadTex();

    DEMOStartDisplay();

    // main loop
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        // Read input
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {0, 40}, {PAD_BUTTON_A, 10},
            {0, 40}, {PAD_BUTTON_B, 10}, {0, 0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        // Position test
        if (DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            VecFx32 m;
            fx32    w;
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            // Set matrix for test
            G3_MtxMode(GX_MTXMODE_TEXTURE);
            G3_Identity();
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
            G3_PushMtx();
            G3_Translate(0, 0, -5 * FX32_ONE);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
            //---------------------------------------------------------------------------
            // Start position test
            G3_PositionTest(FX16_ONE, FX16_ONE, FX16_ONE);
            //---------------------------------------------------------------------------
            G3_PopMtx(1);

            //---------------------------------------------------------------------------
            // Get result of position test
            while (G3X_GetPositionTestResult(&m, &w))
            {
            }
            //---------------------------------------------------------------------------

            // Print result of position test
            OS_Printf("mssg%d:Position Test : Pos(%d, %d, %d) W(%d)\n", count++, m.x, m.y, m.z, w);

#ifdef SDK_AUTOTEST
            EXT_CompPrint("Pos(3611, 456, 5817) W(13960)", "Pos(%d, %d, %d) W(%d)", m.x, m.y, m.z,
                          w);
#endif
        }

        /* G3_VectorTest()ÇçsÇ§ */
        // Vector test
        if (DEMO_IS_TRIG(PAD_BUTTON_B))
        {
            fx16    vec[3];

            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            // Set matrix for test
            G3_MtxMode(GX_MTXMODE_TEXTURE);
            G3_Identity();
            G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
            G3_PushMtx();
            G3_Translate(0, 0, -5 * FX32_ONE);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
            //---------------------------------------------------------------------------
            // Start vector test
            G3_VectorTest(0, 0, FX16_ONE - 1);
            //---------------------------------------------------------------------------
            G3_PopMtx(1);

            //---------------------------------------------------------------------------
            // Get result of vector test
            while (G3X_GetVectorTestResult(vec))
            {
            }
            //---------------------------------------------------------------------------

            // Print result of vector test
            OS_Printf("mssg%d:Vector Test : Vec(%d, %d, %d)\n", count++, vec[0], vec[1], vec[2]);

#ifdef SDK_AUTOTEST
            EXT_CompPrint("Vec(3222, 1982, 1546)", "Vec(%d, %d, %d)", vec[0], vec[1], vec[2]);
#endif
        }

        // Camera setting
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Sight position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up
            G3_LookAt(&Eye, &vUp, &at, NULL);   // Sight setting
        }

        // Light setting
        G3_LightVector(GX_LIGHTID_0, 0, -FX32_ONE + 1, 0);
        G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

        // Matrix setting
        G3_MtxMode(GX_MTXMODE_TEXTURE);
        G3_Identity();
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
        G3_PushMtx();

        // Rotate and translate cube
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Translate(0, 0, -5 * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        // Draw setting
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // Diffuse
                                GX_RGB(16, 16, 16),     // Ambient
                                TRUE); // Color
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // Specular
                                GX_RGB(0, 0, 0),        // Emission
                                FALSE); // Shininess
        G3_TexImageParam(GX_TEXFMT_DIRECT,      // Texture format
                         GX_TEXGEN_TEXCOORD,    // Texture generation
                         GX_TEXSIZE_S64,        // Texture width
                         GX_TEXSIZE_T64,        // Texture height
                         GX_TEXREPEAT_NONE,     // Texture repeat
                         GX_TEXFLIP_NONE,       // Texture flip
                         GX_TEXPLTTCOLOR0_USE,  // Palette color
                         myTexAddr);   // Texture address
        G3_PolygonAttr(GX_LIGHTMASK_0, // Light
                       GX_POLYGONMODE_MODULATE, // Polygon mode
                       GX_CULL_NONE,   // Culling
                       0,              // Polygon ID
                       31,             // Alpha
                       GX_POLYGON_ATTR_MISC_NONE);      // Misc

        // Draw cube
        DrawCube();

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // Wait VBlank
        OS_WaitVBlankIntr();
#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x8C9F5E42);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
