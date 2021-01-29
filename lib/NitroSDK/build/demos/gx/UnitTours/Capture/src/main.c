/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Capture
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.11  2006/01/18 02:11:27  kitase_hirotake
  do-indent

  Revision 1.10  2005/11/21 10:40:15  kitase_hirotake
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.9  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.8  2004/08/03 01:53:12  takano_makoto
  modify set correct value in GX_SetGraphicsMode().

  Revision 1.7  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.6  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.5  2004/03/15 02:01:54  takano_makoto
  modify comments and codes a little.

  Revision 1.4  2004/03/05 10:19:00  terui
  abolish restriction.
  print information about afterimage.

  Revision 1.3  2004/03/05 07:30:41  terui
  restrict blending parameter

  Revision 1.2  2004/03/03 01:09:13  yasu
  workaround for 'ConstantPools.c'line: 346

  Revision 1.1  2004/03/01 11:27:06  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//      A sample for capture display.
//
//      Rotating cube is displayed.
//      There is motion blur effect by capturing display.
//
//      HOWTO:
//      1.      Call "GX_SetCapture" to capture display and blend with rendered image.
//
//      OPERATION:
//      1.      Push cross button to move cube position.
//      2.      Push start button to reset cube position.
//      3.      Push A or B button to change blending parameter for captured data.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"
#include "tex_32768.h"

//---------------------------------------------------------------------------
//      Cube model data
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
//      Set vertex coordinate.
//      input:
//              i_idx:  ID of vertex data
//---------------------------------------------------------------------------
inline void Vertex(int i_idx)
{
    G3_Vtx(s_Vertex[i_idx * 3], s_Vertex[i_idx * 3 + 1], s_Vertex[i_idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//      Set normal setting.
//      input:
//              i_idx:  ID of normal data
//---------------------------------------------------------------------------
inline void Normal(int i_idx)
{
    G3_Direct1(G3OP_NORMAL, s_Normal[i_idx]);
}

//---------------------------------------------------------------------------
//      Set texture coordinate.
//      input:
//              i_idx:  ID of texture data
//---------------------------------------------------------------------------
inline void TextureCoord(int i_idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[i_idx]);
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
//      main
//---------------------------------------------------------------------------
#ifdef  SDK_CW_WA_CONSTPOOLS
#pragma optimization_level 1
#endif
void NitroMain(void)
{
    unsigned int count = 0;
    u16     Rotate = 0;
    u32     myTexAddr = 0x00000;       // Address of texture image slot
    fx32    pos_x = 0;
    fx32    pos_y = 0;
    int     eva = 0x04;
    int     evb = 0x0c;

    // Initialize
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    // Set VRAM-C to LCDC
    GX_SetBankForLCDC(GX_VRAM_LCDC_C);

    // Set display mode
    GX_SetGraphicsMode(GX_DISPMODE_VRAM_C, GX_BGMODE_4, // value used by capture circuit
                       GX_BG0_AS_3D);  // can't pass dummy value

    // Load texture image into texture image slot.
    GX_BeginLoadTex();
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // Src address
                   myTexAddr,          // Destination slot address
                   8192);              // Size to load
    }
    GX_EndLoadTex();

    DEMOStartDisplay();

    // Show information message
    OS_Printf("If EVB is over 8/16, the afterimage of blur stay eternally.\n");
    OS_Printf("eva=%d : evb=%d\n", eva, evb);

    // main loop
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        // Read input
        DEMOReadKey();
        // Move cube position
        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            pos_y += 500;
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            pos_y -= 500;
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            pos_x += 500;
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            pos_x -= 500;
        }
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            pos_x = pos_y = 0;
        }

        // Change blending parameter
        if (DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            if (++evb > 0x10)
            {
                evb = 0x10;
            }
            eva = 0x10 - evb;
            OS_Printf("eva=%d : evb=%d\n", eva, evb);
        }
        else if (DEMO_IS_TRIG(PAD_BUTTON_B))
        {
            if (--evb < 0x00)
            {
                evb = 0x00;
            }
            eva = 0x10 - evb;
            OS_Printf("eva=%d : evb=%d\n", eva, evb);
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

            G3_Translate(pos_x, pos_y, -5 * FX32_ONE);
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
                         GX_TEXSIZE_T64,        // Texture hight
                         GX_TEXREPEAT_NONE,     // Texture repeat
                         GX_TEXFLIP_NONE,       // Texture flip
                         GX_TEXPLTTCOLOR0_USE,  // Palette color
                         myTexAddr);   // Texture address
        G3_PolygonAttr(GX_LIGHTMASK_0, // Light
                       GX_POLYGONMODE_MODULATE, // Polygon mode
                       GX_CULL_NONE,   // Culling
                       0,              // Polygon ID
                       31,             // Alpha
                       0);             // Misc

        // Draw cube
        DrawCube();

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // Wait VBlank
        OS_WaitVBlankIntr();

        //---------------------------------------------------------------------------
        // Execute capture
        // Blend rendered image and displayed image (VRAM), and output to VRAM-C
        GX_SetCapture(GX_CAPTURE_SIZE_256x192,  // Capture size
                      GX_CAPTURE_MODE_AB,       // Capture mode
                      GX_CAPTURE_SRCA_2D3D,     // Blend src A
                      GX_CAPTURE_SRCB_VRAM_0x00000,     // Blend src B
                      GX_CAPTURE_DEST_VRAM_C_0x00000,   // Output VRAM
                      eva,             // Blend parameter for src A
                      evb);            // Blend parameter for src B
        //---------------------------------------------------------------------------
    }
}
