/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/FlipRepeat
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

  Revision 1.8  2005/11/21 10:41:29  kitase_hirotake
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.7  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 08:26:15  takano_makoto
  fix comment.

  Revision 1.5  2004/05/27 09:26:19  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.2  2004/03/15 02:02:23  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 10:41:58  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//      A sample to confirm drawing out of texture range.
//
//      One square which has double length side of texture is displayed.
//
//      HOWTO:
//      1.      Call "G3_TexImageParam" to set mode for out of texture range.
//
//      OPERATION:
//      1.      Push A button to change texture repeat mode.
//      2.      Push B button to change texture flip mode.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"
#include "tex_32768.h"

//---------------------------------------------------------------------------
//      Square model data
//---------------------------------------------------------------------------
// Vertex data
static s16 s_Vertex[3 * 4] = {
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};
// Normal data
static VecFx10 s_Normal = GX_VECFX10(0, 0, -FX32_ONE + 1);
// Texture coordinate data
#define FX32_TWO        (FX32_ONE * 2)
static GXSt s_TextureCoord[] = {
    GX_ST(0, 0),
    GX_ST(0, 64 * FX32_TWO),
    GX_ST(64 * FX32_TWO, 0),
    GX_ST(64 * FX32_TWO, 64 * FX32_TWO)
};

//---------------------------------------------------------------------------
//      Set vertex coordinate.
//      input:
//              idx:  ID of vertex data
//---------------------------------------------------------------------------
inline void Vertex(int idx)
{
    G3_Vtx(s_Vertex[idx * 3], s_Vertex[idx * 3 + 1], s_Vertex[idx * 3 + 2]);
}

//---------------------------------------------------------------------------
//      Set normal setting.
//---------------------------------------------------------------------------
inline void Normal(void)
{
    G3_Direct1(G3OP_NORMAL, s_Normal);
}

//---------------------------------------------------------------------------
//      Set texture coordinate.
//      input:
//              idx:  ID of texture data
//---------------------------------------------------------------------------
inline void TextureCoord(int idx)
{
    G3_Direct1(G3OP_TEXCOORD, s_TextureCoord[idx]);
}

//---------------------------------------------------------------------------
//      Draw a rectangle and set texture.
//---------------------------------------------------------------------------
static void DrawRectangle(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // start to set vertexes.(use square polygon)
    {
        TextureCoord(1);
        Normal();
        Vertex(3);
        TextureCoord(0);
        Normal();
        Vertex(2);
        TextureCoord(2);
        Normal();
        Vertex(0);
        TextureCoord(3);
        Normal();
        Vertex(1);
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
void NitroMain(void)
{
    unsigned int count = 0;
    GXTexRepeat repeat = GX_TEXREPEAT_NONE;
    GXTexFlip flip = GX_TEXFLIP_NONE;
    u32     myTexAddr = 0x00000;       // Address of texture image slot

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

    // Print information
    OS_Printf("mssg%d:Repeat=S:%s T:%s  Flip=S:%s T:%s\n", count++,
              ((repeat & 0x01) ? "TRUE" : "FALSE"),
              ((repeat & 0x02) ? "TRUE" : "FALSE"),
              ((flip & 0x01) ? "TRUE" : "FALSE"), ((flip & 0x02) ? "TRUE" : "FALSE"));

    DEMOStartDisplay();

    // main loop
    while (1)
    {
        G3X_Reset();

        // Read input
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {0, 10}, {PAD_BUTTON_A | PAD_BUTTON_B, 10},
            {0, 10}, {PAD_BUTTON_A | PAD_BUTTON_B, 10}, {0, 0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        // Change repeat mode
        if (DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            repeat = (GXTexRepeat)(((int)repeat + 1) % 4);
        }
        // Change flip mode
        if (DEMO_IS_TRIG(PAD_BUTTON_B))
        {
            flip = (GXTexFlip)(((int)flip + 1) % 4);
        }
        // Print information
        if (DEMO_IS_TRIG((PAD_BUTTON_A | PAD_BUTTON_B)))
        {
            OS_Printf("mssg%d:Repeat=S:%s T:%s  Flip=S:%s T:%s\n", count++,
                      ((repeat & 0x01) ? "TRUE" : "FALSE"),
                      ((repeat & 0x02) ? "TRUE" : "FALSE"),
                      ((flip & 0x01) ? "TRUE" : "FALSE"), ((flip & 0x02) ? "TRUE" : "FALSE"));
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

        // Draw setting
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // Diffuse
                                GX_RGB(16, 16, 16),     // Ambient
                                TRUE); // Color
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // Specular
                                GX_RGB(0, 0, 0),        // Emission
                                FALSE); // Shininess
        //---------------------------------------------------------------------------
        // Texture drawing setting
        G3_TexImageParam(GX_TEXFMT_DIRECT,      // Texture format
                         GX_TEXGEN_TEXCOORD,    // Texture generation
                         GX_TEXSIZE_S64,        // Texture width
                         GX_TEXSIZE_T64,        // Texture height
                         repeat,       // --- Texture REPEAT mode ---
                         flip,         // --- Texture FLIP mode ---
                         GX_TEXPLTTCOLOR0_USE,  // Palette color
                         myTexAddr);   // Texture address
        //---------------------------------------------------------------------------
        G3_PolygonAttr(GX_LIGHTMASK_0, // Light
                       GX_POLYGONMODE_MODULATE, // Polygon mode
                       GX_CULL_NONE,   // Culling
                       0,              // Polygon ID
                       31,             // Alpha
                       GX_POLYGON_ATTR_MISC_NONE);      // Misc

        // Draw square
        DrawRectangle();

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // Wait VBlank
        OS_WaitVBlankIntr();

#ifdef SDK_AUTOTEST                    // code for auto test
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xA7CD1F92);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
