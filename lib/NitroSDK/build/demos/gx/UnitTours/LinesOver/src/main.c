/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/LinesOver
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.9  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 10:41:40  kitase_hirotake
  SVC_WaitVBlankIntr ‚ð OS_WaitVBlankIntr ‚É•ÏX

  Revision 1.7  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.6  2004/10/22 13:09:35  takano_makoto
  LineBufferOverflow -> LineBufferUnderflow‚Ö‰ü–¼

  Revision 1.5  2004/08/04 07:07:41  takano_makoto
  insert geometry wait, and move statement which print rendering result after vblank intr.

  Revision 1.4  2004/05/27 09:26:37  takano_makoto
  Add Autotest code.

  Revision 1.3  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.2  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.1  2004/03/01 10:42:36  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//  A sample to confirm linesover.
//
//  200 cubes is displayed on same position.
//  When there is over limit of polygons per line, the polygon breaks off
//  on that line.
//
//  HOWTO:
//  1.  Draw many cubes over limit.
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
//      i_idx:  ID of vertex data
//---------------------------------------------------------------------------
inline void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//  Set normal setting.
//  input:
//      i_idx:  ID of normal data
//---------------------------------------------------------------------------
inline void Normal(int i_idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[i_idx]);
}

//---------------------------------------------------------------------------
//  Set texture coordinate.
//  input:
//      i_idx:  ID of texture data
//---------------------------------------------------------------------------
inline void TextureCoord(int i_idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[i_idx]);
}

//---------------------------------------------------------------------------
//  Draw cube
//  input:
//      i_TexAddr:  Address of texture which is pasted on cube
//---------------------------------------------------------------------------
static void Cube(u32 i_TexAddr)
{
    // Draw setting
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // Diffuse
                            GX_RGB(16, 16, 16), // Ambient
                            TRUE);     // Color
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // Specular
                            GX_RGB(0, 0, 0),    // Emission
                            FALSE);    // Shininess
    G3_TexImageParam(GX_TEXFMT_DIRECT, // Texture format
                     GX_TEXGEN_TEXCOORD,        // Texture generation
                     GX_TEXSIZE_S64,   // Texture width
                     GX_TEXSIZE_T64,   // Texture height
                     GX_TEXREPEAT_NONE, // Texture repeat
                     GX_TEXFLIP_NONE,  // Texture flip
                     GX_TEXPLTTCOLOR0_USE,      // Palette color
                     i_TexAddr);       // Texture address
    G3_PolygonAttr(GX_LIGHTMASK_0,     // Light
                   GX_POLYGONMODE_MODULATE,     // Polygon mode
                   GX_CULL_NONE,       // Culling
                   0,                  // Polygon ID
                   31,                 // Alpha
                   0);                 // Misc

    // Draw cube
    G3_Begin(GX_BEGIN_QUADS);
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
    G3_End();
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
void NitroMain(void)
{
    u16     Rotate = 0;
    u32     texAddr = 0x01000;         // Address of texture image slot

    // Initialize
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    // If boot on ensata , print waring message.
    if (OS_IsRunOnEmulator())
    {
        OS_Panic("LinesOver is not emulated on \"ensata\".\n");
    }

    // Load texture image into texture image slot.
    GX_BeginLoadTex();
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // Src address
                   texAddr,            // Destination address
                   8192);              // Size to load
    }
    GX_EndLoadTex();

    DEMOStartDisplay();

    // main loop
    while (1)
    {
        int     i;

        G3X_Reset();
        Rotate += 256;

        // Camera setting
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Sight position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up
            G3_LookAt(&Eye, &vUp, &at, NULL);   // Sight setting
        }

        // Light setting
        G3_LightVector(GX_LIGHTID_0, 0, 0, -FX32_ONE + 1);
        G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

        // Matrix setting
        G3_MtxMode(GX_MTXMODE_TEXTURE);
        G3_Identity();
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);

        // Draw 200 cubes
        for (i = 0; i < 200; i++)
        {
            // Rotate and translate cubes
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_PushMtx();

            G3_Translate(0, 0, -5 * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);

            // Draw cube
            Cube(texAddr);

            G3_PopMtx(1);
        }

        while (G3X_IsGeometryBusy())
        {
        }

        // Print ListRam information
        OS_Printf("PolygonListRAM = %d/2048\n", G3X_GetPolygonListRamCount());
        OS_Printf("VertexListRAM  = %d/6144\n", G3X_GetVtxListRamCount());

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // Wait VBlank
        OS_WaitVBlankIntr();

        // Print Rendering information
        OS_Printf("LineCnt=%d\n", G3X_GetRenderedLineCount());
        OS_Printf("LinesOverFlow  = %s\n\n", (G3X_IsLineBufferUnderflow()? "TRUE" : "FALSE"));


#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x9ECAB1AD);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
