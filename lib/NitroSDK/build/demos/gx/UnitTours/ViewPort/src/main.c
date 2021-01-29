/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/ViewPort
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

  Revision 1.9  2005/11/21 10:46:02  kitase_hirotake
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.8  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.7  2004/11/02 08:27:36  takano_makoto
  fix comment.

  Revision 1.6  2004/05/27 09:28:16  takano_makoto
  Adjust indent.

  Revision 1.5  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.4  2004/04/06 12:48:08  yada
  fix header comment

  Revision 1.3  2004/03/15 02:03:34  takano_makoto
  modify comments and codes a little.

  Revision 1.2  2004/03/03 02:03:53  terui
  Measure to THUMB compiler problem

  Revision 1.1  2004/03/01 10:45:38  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//  A sample to change viewport size.
//
//  One cube is displayed on viewport which is specified in screen.
//
//  HOWTO:
//  1.  Call "G3_ViewPort" to set viewport size.
//
//  OPERATION:
//  1.  Push A button and UP or DOWN button to change up side position of viewport.
//  2.  Push A button and LEFT or RIGHT button to change left side position of viewport.
//  3.  Push B button and UP or DOWN button to chnage down side position of viewport.
//  4.  Push B button and LEFT or RIGHT button to change right side position of viewport.
//  5.  Push start button to change position of viewport default.
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
// Texture data
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
    int     vp_x1 = 0, vp_y1 = 0;
    int     vp_x2 = 255, vp_y2 = 191;
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

    // Print information
    OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n", count++, vp_x1, vp_y1, vp_x2, vp_y2);

    // main loop
    while (1)
    {
        G3X_Reset();
        Rotate += 256;

        // Read input
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {PAD_BUTTON_A | PAD_KEY_DOWN | PAD_KEY_RIGHT, 30},
            {PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_LEFT, 30},
            {PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_LEFT, 30}, {0, 0}
            };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        // Change viewport size
        if (DEMO_IS_PRESS(PAD_BUTTON_A))
        {
            // Change up side position
            if (DEMO_IS_PRESS(PAD_KEY_UP))
            {
                if (vp_y1 > 0)
                {
                    vp_y1--;
                }
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
            else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            {
                if (vp_y1 < 191 && vp_y1 < vp_y2 - 1)
                {
                    vp_y1++;
                }
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
            // Change left side position
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            {
                if (vp_x1 < 255 && vp_x1 < vp_x2 - 1)
                {
                    vp_x1++;
                }
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
            else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            {
                if (vp_x1 > 0)
                {
                    vp_x1--;
                }
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
        }
        if (DEMO_IS_PRESS(PAD_BUTTON_B))
        {
            // Change down side position
            if (DEMO_IS_PRESS(PAD_KEY_UP))
            {
                if (vp_y2 > 0 && vp_y1 < vp_y2 - 1)
                    vp_y2--;
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
            else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
            {
                if (vp_y2 < 191)
                    vp_y2++;
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
            // Change right side position
            if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
            {
                if (vp_x2 < 255)
                    vp_x2++;
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
            else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
            {
                if (vp_x2 > 0 && vp_x1 < vp_x2 - 1)
                    vp_x2--;
                OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n",
                          count++, vp_x1, vp_y1, vp_x2, vp_y2);
            }
        }
        // Change to default position
        if (DEMO_IS_TRIG(PAD_BUTTON_START))
        {
            vp_x1 = 0;
            vp_y1 = 0;
            vp_x2 = 255;
            vp_y2 = 191;
            OS_Printf("mssg%d:ViewPort=(%d, %d, %d, %d)\n", count++, vp_x1, vp_y1, vp_x2, vp_y2);
        }

        //---------------------------------------------------------------------------
        // Set viewport size
        G3_ViewPort(vp_x1, vp_y1, vp_x2, vp_y2);
        //---------------------------------------------------------------------------

        // Camera setting
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Eye position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);
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
        EXT_TestScreenShot(100, 0x66E2DF30);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
