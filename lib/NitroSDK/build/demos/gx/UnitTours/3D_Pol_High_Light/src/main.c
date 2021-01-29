/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Pol_High_Light
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.37  2006/01/18 02:11:22  kitase_hirotake
  do-indent

  Revision 1.36  2005/11/21 10:35:11  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.35  2005/02/28 05:26:14  yosizaki
  do-indent.

  Revision 1.34  2004/11/02 09:36:51  takano_makoto
  fix typo.

  Revision 1.33  2004/05/24 10:32:18  takano_makoto
  autotestコードの修正

  Revision 1.32  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.31  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.30  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.29  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.28  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.27  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.26  2004/01/30 11:34:10  kitani_toshikazu
  Removed Tab codes.

  Revision 1.25  2004/01/30 11:15:14  kitani_toshikazu
  Added DEMO_Set3DDefaultShininessTable() to control  specular reflection.

  Revision 1.24  2004/01/29 07:46:50  kitani_toshikazu
  Removed unnecessary MI_WaitDma() after MI_SendGXCommand().

  Revision 1.23  2004/01/28 06:20:40  kitani_toshikazu
  Adjusted light intensity

  Revision 1.22  2004/01/27 09:00:04  nishida_kenji
  moves G3_LightVector to follow G3_LookAt.

  Revision 1.21  2004/01/22 02:03:25  nishida_kenji
  swap G3_SwapBuffers and SVC_WaitVBlankIntr

  Revision 1.20  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.19  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.18  2004/01/18 08:19:33  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.17  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.16  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.15  2004/01/06 12:54:08  nishida_kenji
  revise comments

  Revision 1.14  2004/01/06 00:50:25  nishida_kenji
  MI_SendGXCommand takes size in bytes

  Revision 1.13  2004/01/05 12:05:03  nishida_kenji
  revises comments

  Revision 1.12  2003/12/26 08:06:32  nishida_kenji
  revise comments

  Revision 1.11  2003/12/25 11:00:10  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.10  2003/12/25 06:25:02  nishida_kenji
  revise a part of geometry command APIs

  Revision 1.9  2003/12/24 13:02:15  nishida_kenji
  G3Util_BeginLoadTexture->GX_BeginLoadTex

  Revision 1.8  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample for high-light shading
//
// Display two chairs.
// The left one is in modulation mode, and the right one is in high-light mode.
//
// HOWTO:
// 1. Switch to high light shading mode by G3X_SetShading(GX_SHADING_HIGHLIGHT).
// 2. Set up the high-light(toon) table by G3X_SetToonTable.
// 3. Speciy GX_POLYGON_MODE_TOON by G3X_PolygonAttr.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "data.h"

GXRgb   Toon_Table[32] = {
    GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0),
    GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0),
    GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0), GX_RGB(0, 0, 0),
    GX_RGB(4, 4, 4), GX_RGB(4, 4, 4), GX_RGB(4, 4, 4), GX_RGB(4, 4, 4),
    GX_RGB(16, 16, 16), GX_RGB(16, 16, 16), GX_RGB(16, 16, 16), GX_RGB(16, 16, 16),
    GX_RGB(19, 19, 19), GX_RGB(22, 22, 22), GX_RGB(25, 25, 25), GX_RGB(28, 28, 28),
    GX_RGB(29, 29, 29), GX_RGB(30, 30, 30), GX_RGB(31, 31, 31), GX_RGB(31, 31, 31),
    GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), GX_RGB(31, 31, 31)
};


void NitroMain(void)
{
    u16     Rotate = 0;                // for rotating cubes(0-65535)
    u32     myTexAddr = 0x1000;        // texture image address in texture image slots
    u32     myTexPlttAddr = 0x1000;    // texture palette address in texture palette slots

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for 3D mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    //---------------------------------------------------------------------------
    // Specify high-light mode
    //---------------------------------------------------------------------------
    G3X_SetShading(GX_SHADING_HIGHLIGHT);

    //---------------------------------------------------------------------------
    // Download the textures:
    //
    // Transfer the texture data on the main memory to the texture image slots.
    //---------------------------------------------------------------------------
    GX_BeginLoadTex();                 // map the texture image slots onto LCDC address space
    {
        GX_LoadTex((void *)&tex_isu1[0],        // a pointer to the texture data on the main memory(4 bytes aligned)
                   myTexAddr,          // an offset address in the texture image slots
                   tex_isu1_size       // the size of the texture(s)(in bytes)
            );
        GX_LoadTex((void *)&tex_isu2[0],        // a pointer to the texture data on the main memory(4 bytes aligned)
                   myTexAddr + tex_isu1_size,   // an offset address in the texture image slots
                   tex_isu2_size       // the size of the texture(s)(in bytes)
            );
    }
    GX_EndLoadTex();                   // restore the texture image slots

    GX_BeginLoadTexPltt();             // map the texture palette slots onto LCDC address space
    {
        GX_LoadTexPltt((void *)&pal_isu1[0],    // a pointer to the texture data on the main memory(4 bytes aligned)
                       myTexPlttAddr,  // an offset address in the texture palette slots
                       pal_isu1_size   // the size of the texture palette(s)(in bytes)
            );
        GX_LoadTexPltt((void *)&pal_isu2[0],    // a pointer to the texture data on the main memory(4 bytes aligned)
                       myTexPlttAddr + pal_isu1_size,   // an offset address in the texture palette slots
                       pal_isu2_size   // the size of the texture palette(s)(in bytes)
            );
    }
    GX_EndLoadTexPltt();               // restore the texture palette slots


    //---------------------------------------------------------------------------
    // Set up the high light(toon) table
    //---------------------------------------------------------------------------
    G3X_SetToonTable(Toon_Table);

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
        G3_LightVector(GX_LIGHTID_0, FX32_SQRT1_2, -FX32_SQRT1_2, 0);
        G3_LightColor(GX_LIGHTID_0, GX_RGB(16, 16, 16));

        G3_Translate(0, 0, -5 * FX32_ONE);

        G3_PushMtx();

        //---------------------------------------------------------------------------
        //
        // Left chair (in modulation mode)
        //
        //---------------------------------------------------------------------------
        G3_PushMtx();

        {
            // Rotate and translate the chair
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Translate(-2 * FX32_ONE, 0, 0);
            G3_RotZ(s, c);
            G3_RotY(s, c);
            G3_RotX(s, c);
        }

        G3_TexImageParam(GX_TEXFMT_PLTT16,      // 16 colors palette texture
                         GX_TEXGEN_NONE,        // no texgen
                         GX_TEXSIZE_S256,       // 256 pixels
                         GX_TEXSIZE_T64,        // 64 pixels
                         GX_TEXREPEAT_NONE,     // no repeat
                         GX_TEXFLIP_NONE,       // no flip
                         GX_TEXPLTTCOLOR0_USE,  // use color 0 of the palette
                         myTexAddr     // the offset of the texture image
            );

        G3_TexPlttBase(myTexPlttAddr,  // the offset of the texture palette
                       GX_TEXFMT_PLTT16 // 16 colors palette texture
            );

        // Set the material color( diffuse, ambient , specular ) as basic white 
        DEMO_Set3DDefaultMaterial(TRUE, TRUE);
        DEMO_Set3DDefaultShininessTable();

        G3_PolygonAttr(GX_LIGHTMASK_0, // Light #0 is on
                       GX_POLYGONMODE_MODULATE, // modulation mode
                       GX_CULL_BACK,   // cull back
                       0,              // polygon ID(0 - 63)
                       31,             // alpha(0 - 31)
                       0               // OR of GXPolygonAttrMisc's value
            );

        G3_Begin(GX_BEGIN_TRIANGLES);
        {
            MI_SendGXCommand(3, isu1, isu1_size);
        }
        G3_End();

        G3_TexImageParam(GX_TEXFMT_PLTT16,      // 16 colors palette texture
                         GX_TEXGEN_NONE,        // no texgen
                         GX_TEXSIZE_S128,       // 128 pixels
                         GX_TEXSIZE_T128,       // 128 pixels
                         GX_TEXREPEAT_NONE,     // no repeat
                         GX_TEXFLIP_NONE,       // no flip
                         GX_TEXPLTTCOLOR0_USE,  // use color 0 of the palette
                         myTexAddr + tex_isu1_size      // the offset of the texture image
            );

        G3_TexPlttBase(myTexPlttAddr + pal_isu1_size,   // the offset of the texture palette
                       GX_TEXFMT_PLTT16 // 16 colors palette texture
            );

        // Set the material color( diffuse, ambient , specular ) as basic white 
        DEMO_Set3DDefaultMaterial(TRUE, TRUE);
        DEMO_Set3DDefaultShininessTable();


        G3_PolygonAttr(GX_LIGHTMASK_0, // Light #0 is on
                       GX_POLYGONMODE_MODULATE, // modulation mode
                       GX_CULL_BACK,   // cull back
                       0,              // polygon ID(0 - 63)
                       31,             // alpha(0 - 31)
                       0               // OR of GXPolygonAttrMisc's value
            );

        G3_Begin(GX_BEGIN_TRIANGLES);
        {
            MI_SendGXCommand(3, isu2, isu2_size);
        }
        G3_End();

        G3_PopMtx(1);

        //---------------------------------------------------------------------------
        //
        // Right chair (in high-light mode)
        //
        //---------------------------------------------------------------------------
        G3_PushMtx();

        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_Translate(2 * FX32_ONE, 0, 0);
            G3_RotZ(s, c);
            G3_RotY(s, c);
            G3_RotX(s, c);
        }

        G3_TexImageParam(GX_TEXFMT_PLTT16,      // 16 colors palette texture
                         GX_TEXGEN_NONE,        // no texgen
                         GX_TEXSIZE_S256,       // 256 pixels
                         GX_TEXSIZE_T64,        // 64 pixels
                         GX_TEXREPEAT_NONE,     // no repeat
                         GX_TEXFLIP_NONE,       // no flip
                         GX_TEXPLTTCOLOR0_USE,  // use color 0 of the palette
                         myTexAddr     // the offset of the texture image
            );

        G3_TexPlttBase(myTexPlttAddr,  // the offset of the texture palette
                       GX_TEXFMT_PLTT16 // 16 colors palette texture
            );

        // Set the material color( diffuse, ambient , specular ) as basic white 
        DEMO_Set3DDefaultMaterial(TRUE, TRUE);
        DEMO_Set3DDefaultShininessTable();

        G3_PolygonAttr(GX_LIGHTMASK_0, // Light #0 is on
                       GX_POLYGONMODE_TOON,     // toon/high-light mode
                       GX_CULL_BACK,   // cull back
                       0,              // polygon ID(0 - 63)
                       31,             // alpha(0 - 31)
                       0               // OR of GXPolygonAttrMisc's value
            );

        G3_Begin(GX_BEGIN_TRIANGLES);
        {
            MI_SendGXCommand(3, isu1, isu1_size);
        }
        G3_End();

        G3_TexImageParam(GX_TEXFMT_PLTT16,      // 16 colors palette texture
                         GX_TEXGEN_NONE,        // no texgen
                         GX_TEXSIZE_S128,       // 128 pixels
                         GX_TEXSIZE_T128,       // 128 pixels
                         GX_TEXREPEAT_NONE,     // no repeat
                         GX_TEXFLIP_NONE,       // no flip
                         GX_TEXPLTTCOLOR0_USE,  // use color 0 of the palette
                         myTexAddr + tex_isu1_size      // the offset of the texture image
            );

        G3_TexPlttBase(myTexPlttAddr + pal_isu1_size,   // the offset of the texture palette
                       GX_TEXFMT_PLTT16 // 16 colors palette texture
            );

        // Set the material color( diffuse, ambient , specular ) as basic white 
        DEMO_Set3DDefaultMaterial(TRUE, TRUE);
        DEMO_Set3DDefaultShininessTable();

        G3_PolygonAttr(GX_LIGHTMASK_0, // Light #0 is on
                       GX_POLYGONMODE_TOON,     // TOON / HIGHLIGHT mode
                       GX_CULL_BACK,   // cull back
                       0,              // polygon ID(0 - 63)
                       31,             // alpha(0 - 31)
                       0               // OR of GXPolygonAttrMisc's value
            );

        G3_Begin(GX_BEGIN_TRIANGLES);
        {
            MI_SendGXCommand(3, isu2, isu2_size);
        }
        G3_End();

        G3_PopMtx(1);

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x87F02FC9);
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
