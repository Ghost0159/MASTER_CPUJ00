/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/RamOverFlow
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

  Revision 1.8  2005/11/21 10:42:18  kitase_hirotake
  SVC_WaitVBlankIntr ‚ð OS_WaitVBlankIntr ‚É•ÏX

  Revision 1.7  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.6  2004/10/22 13:09:52  takano_makoto
  LineBufferOverflow -> LineBufferUnderflow‚Ö‰ü–¼

  Revision 1.5  2004/05/27 09:27:06  takano_makoto
  Add Autotest code.

  Revision 1.4  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.3  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.2  2004/03/15 02:02:53  takano_makoto
  modify comments and codes a little.

  Revision 1.1  2004/03/01 10:44:23  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//  A sample to be over limit of ListRam.
//
//  One circle, which is devided by parameter, is displayed.
//
//  HOWTO:
//  1.  Draw circle which is too devied over limit.
//
//  OPERATION:
//  1.  Push cross button to change devision parameter.
//  2.  Push L or R button to change circle scale.
//
//---------------------------------------------------------------------------
#include <nitro.h>
#include "DEMO.h"

//---------------------------------------------------------------------------
//  Calculate normal from original point and vertex information.
//  And set vertex and normal information.
//  input:
//      x,y,z:  vertex information
//---------------------------------------------------------------------------
static void CircleVertex(const s32 x, const s32 y, const s32 z)
{
    u32     length;
    fx16    nx, ny, nz;

    // Square root calculation
    CP_SetSqrt32((u32)(x * x + y * y + z * z));
    length = CP_GetSqrtResult32();

    // Normalize
    nx = (fx16)((x * 511) / length);
    ny = (fx16)((y * 511) / length);
    nz = (fx16)((z * 511) / length);

    // Set information
    G3_Normal(nx, ny, nz);
    G3_Vtx((fx16)x, (fx16)y, (fx16)z);
}

//---------------------------------------------------------------------------
//  Draw circle
//  input:
//      i_numMajor: vertical division of circle
//      i_numMinor: horizontal division of circle
//---------------------------------------------------------------------------
static void Circle(const u32 i_numMajor, const u32 i_numMinor)
{
    const int majorStep = (32767 / (s32)(i_numMajor - 1));
    const int minorStep = (65535 / (s32)i_numMinor);
    int     a, b;
    int     x0, z0, x1, z1;
    int     r0, y0;
    int     x0r0, x1r0, z0r0, z1r0;
    int     i, j;

    for (i = 0; i < i_numMinor; i++)
    {
        a = i * minorStep;
        x0 = FX_SinIdx(a);
        z0 = FX_CosIdx(a);

        if (i < i_numMinor - 1)
        {
            a += minorStep;
            x1 = FX_SinIdx(a);
            z1 = FX_CosIdx(a);
        }
        else
        {
            x1 = FX_SinIdx(0);
            z1 = FX_CosIdx(0);
        }

        // Draw circle
        G3_Begin(GX_BEGIN_TRIANGLE_STRIP);

        {
            CircleVertex(0, 4096, 0);
            for (j = 1; j < i_numMajor - 1; j++)
            {
                b = (j * majorStep) + 16383;
                r0 = FX_CosIdx(b);
                y0 = FX_SinIdx(b);

                x0r0 = (x0 * r0) >> 12;
                z0r0 = (z0 * -r0) >> 12;
                CircleVertex(x0r0, y0, z0r0);

                x1r0 = (x1 * r0) >> 12;
                z1r0 = (z1 * -r0) >> 12;
                CircleVertex(x1r0, y0, z1r0);
            }
            CircleVertex(0, -4096, 0);
        }
        G3_End();
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
    // Set flag which checks VBlank interrupt.
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//---------------------------------------------------------------------------
//  main
//---------------------------------------------------------------------------
void NitroMain(void)
{
    fx32    scale = FX32_ONE;
    u16     Rotate = 0;                // for rotating cubes(0-65535)
    u32     numMajor = 3, numMinor = 2;

    // Initialize
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

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
            const EXTKeys keys[8] = { {PAD_KEY_UP | PAD_KEY_RIGHT | PAD_BUTTON_R, 40}, {0, 0} };
            EXT_AutoKeys(keys, &gKeyWork.press, &gKeyWork.trigger);
        }
#endif


        // Change devision of circle
        if (DEMO_IS_PRESS(PAD_KEY_UP))
        {
            numMajor++;
        }
        else if (DEMO_IS_PRESS(PAD_KEY_DOWN))
        {
            numMajor--;
        }
        if (DEMO_IS_PRESS(PAD_KEY_RIGHT))
        {
            numMinor++;
        }
        else if (DEMO_IS_PRESS(PAD_KEY_LEFT))
        {
            numMinor--;
        }
        if (numMajor < 3)
            numMajor = 3;
        if (numMinor < 2)
            numMinor = 2;
        // Change scale of circle
        if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            scale += 256;
        }
        else if (DEMO_IS_PRESS(PAD_BUTTON_L))
        {
            scale -= 256;
            if (scale < 1)
            {
                scale = 1;
            }
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

        // Rotate and translate circle
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Translate(0, 0, -5 * FX32_ONE);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);

            G3_Scale(scale, scale, scale);
        }

        // Draw setting
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),     // Diffuse
                                GX_RGB(16, 16, 16),     // Ambient
                                TRUE); // Color
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),     // Specular
                                GX_RGB(0, 0, 0),        // Emission
                                FALSE); // Shininess
        G3_TexImageParam(GX_TEXFMT_NONE,        // Texture format
                         GX_TEXGEN_TEXCOORD,    // Texture generation
                         GX_TEXSIZE_S64,        // Texture width
                         GX_TEXSIZE_T64,        // Texture height
                         GX_TEXREPEAT_NONE,     // Texture repeat
                         GX_TEXFLIP_NONE,       // Texture flip
                         GX_TEXPLTTCOLOR0_USE,  // Palette color
                         0);           // Texture address
        G3_PolygonAttr(GX_LIGHTMASK_0, // Light
                       GX_POLYGONMODE_MODULATE, // Polygon mode
                       GX_CULL_BACK,   // Culling
                       0,              // Polygon ID
                       31,             // Alpha
                       GX_POLYGON_ATTR_MISC_NONE);      // Misc

        // Draw circle
        Circle(numMajor, numMinor);

        G3_PopMtx(1);

        // If boot on ensata , print message.
        if (OS_IsRunOnEmulator())
        {
            OS_Warning("On \"ensata\" , list count is not over limit.\n");
        }

        // Print information
        OS_Printf("NumMajor:NumMinor;Scale = %d : %d : %d\n", numMajor, numMinor, scale);
        OS_Printf("PolygonListRAM = %d/2048\n", G3X_GetPolygonListRamCount());
        OS_Printf("VertexListRAM  = %d/6144\n", G3X_GetVtxListRamCount());
        OS_Printf("LinesOverFlow  = %s\n\n", (G3X_IsLineBufferUnderflow()? "TRUE" : "FALSE"));
        if (G3X_IsListRamOverflow())
        {
            OS_Printf("!!!! ListRAM OverFlow !!!!\n\n");
        }

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

        // Wait VBlank
        OS_WaitVBlankIntr();
    }
}
