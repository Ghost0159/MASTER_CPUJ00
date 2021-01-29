/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/AntiAlias
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.12  2006/08/31 02:00:46  okubata_ryoma
  èâä˙èÛë‘Ç∆ï\é¶ÇÃêHÇ¢à·Ç¢ÇèCê≥

  Revision 1.11  2006/01/18 02:11:27  kitase_hirotake
  do-indent

  Revision 1.10  2005/11/21 10:39:57  kitase_hirotake
  SVC_WaitVBlankIntr Ç OS_WaitVBlankIntr Ç…ïœçX

  Revision 1.9  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.8  2004/11/02 08:23:38  takano_makoto
  fix comment.

  Revision 1.7  2004/05/27 09:25:30  takano_makoto
  Add Autotest code.

  Revision 1.6  2004/05/25 07:05:21  takano_makoto
  Remove Code for Ensata, for version up of ensata.

  Revision 1.5  2004/04/30 06:38:32  takano_makoto
  insert Set ClearColor to blend for AntiAlias effect.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.2  2004/03/15 02:01:49  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 10:38:10  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//      A sample for anti-alias testing.
//
//      One square is displayed along the setting of anti-alias.
//
//      HOWTO:
//      1.      Call "G3X_AntiAlias(TRUE)", which switches anti-alias setting to ON.
//      2.      Call "G3X_AntiAlias(FALSE)", which switches anti-alias setting to OFF.
//
//      OPERATION:
//      1.      Push start button to change anti-alias setting.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"
#include "tex_32768.h"

//---------------------------------------------------------------------------
//      Square model data
//---------------------------------------------------------------------------
//      Vertex data
static s16 s_Vertex[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
};
// Normal data
static VecFx10 s_Normal = GX_VECFX10(0, 0, FX32_ONE - 1);
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
//---------------------------------------------------------------------------
inline void Normal(void)
{
    G3_Direct1(G3OP_NORMAL, s_Normal);
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
//      Draw a rectangle and set texture.
//---------------------------------------------------------------------------
static void DrawRectangle(void)
{
    G3_Begin(GX_BEGIN_QUADS);          // start to set vertexes.(use square polygon)
    {
        TextureCoord(1);
        Normal();
        Vertex(1);
        TextureCoord(3);
        Normal();
        Vertex(3);
        TextureCoord(2);
        Normal();
        Vertex(2);
        TextureCoord(0);
        Normal();
        Vertex(0);
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
    BOOL    trg = TRUE;
    u16     Rotate = 0;
    u32     myTexAddr = 0x00000;       // Address of texture image slot

    // Initialize
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    // Set ClearColor to get effect of AntiAlias
    G3X_SetClearColor(GX_RGB(0, 0, 0), // Color
                      0x1,             // Alpha
                      0x7fff,          // Depth
                      0x3f,            // Polygon ID
                      FALSE);          // Fog setting toward 2D face

    // Load texture image into texture image slot.
    GX_BeginLoadTex();
    {
        GX_LoadTex((void *)&tex_32768_64x64[0], // Src address
                   myTexAddr,          // Destination slot address
                   8192);              // Size to load
    }
    GX_EndLoadTex();

    DEMOStartDisplay();

    OS_Printf("mssg%d:AntiAlias ON\n", count++);

    // main loop
    while (1)
    {
        G3X_Reset();
        Rotate += 32;

        // Read input
        DEMOReadKey();
#ifdef SDK_AUTOTEST                    // code for auto test
        {
            const EXTKeys keys[8] = { {0, 10}, {PAD_BUTTON_START, 10}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif

        if (gKeyWork.trigger & PAD_BUTTON_START)
        {
            trg = (trg + 1) & 0x01;
            //---------------------------------------------------------------------------
            // Anti-alias setting
            G3X_AntiAlias(trg);
            //---------------------------------------------------------------------------
            OS_Printf("mssg%d:AntiAlias %s\n", count++, (trg ? "ON" : "OFF"));
        }

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
        G3_PushMtx();

        // Rotate and translate square
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Translate(0, 0, -3 * FX32_ONE);
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
        EXT_TestScreenShot(100, 0x72A1C04D);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST
    }
}
