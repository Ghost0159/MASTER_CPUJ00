/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Command_Pack
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.38  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.37  2005/11/21 10:33:17  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.36  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.35  2004/11/02 09:36:09  takano_makoto
  fix typo.

  Revision 1.34  2004/05/24 10:31:28  takano_makoto
  autotestコードの修正

  Revision 1.33  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.32  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.31  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.30  2004/02/12 07:06:26  nishida_kenji
  Avoid generating STRB in THUMB mode.

  Revision 1.28  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.27  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.26  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.25  2004/01/30 11:34:10  kitani_toshikazu
  Removed Tab codes.

  Revision 1.24  2004/01/30 11:15:14  kitani_toshikazu
  Added DEMO_Set3DDefaultShininessTable() to control  specular reflection.

  Revision 1.23  2004/01/29 07:46:50  kitani_toshikazu
  Removed unnecessary MI_WaitDma() after MI_SendGXCommand().

  Revision 1.22  2004/01/28 06:20:40  kitani_toshikazu
  Adjusted light intensity

  Revision 1.21  2004/01/27 09:00:04  nishida_kenji
  moves G3_LightVector to follow G3_LookAt.

  Revision 1.20  2004/01/22 02:03:25  nishida_kenji
  swap G3_SwapBuffers and SVC_WaitVBlankIntr

  Revision 1.19  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.18  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.17  2004/01/18 08:19:33  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.16  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.15  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.14  2004/01/06 00:50:13  nishida_kenji
  MI_SendGXCommand takes size in bytes

  Revision 1.13  2004/01/05 12:05:03  nishida_kenji
  revises comments

  Revision 1.12  2003/12/26 06:58:31  nishida_kenji
  revise comments

  Revision 1.11  2003/12/25 11:00:09  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.10  2003/12/25 06:25:02  nishida_kenji
  revise a part of geometry command APIs

  Revision 1.9  2003/12/19 06:32:13  nishida_kenji
  (none)

  Revision 1.8  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  $NoKeywords: $
*---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// Use the command pack data generated in advance:
//
// Send the data by DMA/CPU transfer, and display a sphere rotating and lighted.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "commandList.h"

void NitroMain(void)
{
    u16     count = 0;
    u16     Rotate = 0;                // for rotating spheresa(0-65535)

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
        count = (u16)((count + 1) & 0x00ff);
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

        //---------------------------------------------------------------------------
        // Set up light colors and direction.
        // Notice that light vector is transformed by the current vector matrix
        // immediately after LightVector command is issued.
        //
        // GX_LIGHTID_0: white, downward
        // GX_LIGHTID_1: red, leftward
        // GX_LIGHTID_2: green, upward
        // GX_LIGHTID_3: blue, rightward
        //---------------------------------------------------------------------------
        G3_LightVector(GX_LIGHTID_0, 0, (fx16)-FX32_SQRT1_2, (fx16)-FX32_SQRT1_2);
        G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));

        G3_LightVector(GX_LIGHTID_1, (fx16)-FX32_SQRT1_2, 0, (fx16)-FX32_SQRT1_2);
        G3_LightColor(GX_LIGHTID_1, GX_RGB(31, 0, 0));

        G3_LightVector(GX_LIGHTID_2, 0, (fx16)FX32_SQRT1_2, (fx16)-FX32_SQRT1_2);
        G3_LightColor(GX_LIGHTID_2, GX_RGB(0, 31, 0));

        G3_LightVector(GX_LIGHTID_3, (fx16)FX32_SQRT1_2, 0, (fx16)-FX32_SQRT1_2);
        G3_LightColor(GX_LIGHTID_3, GX_RGB(0, 0, 31));

        G3_PushMtx();
        G3_Translate(0, 0, -2 * FX32_ONE);

        //---------------------------------------------------------------------------
        // Rotate a sphere
        //---------------------------------------------------------------------------
        G3_PushMtx();
        {
            fx16    s = FX_SinIdx(Rotate);
            fx16    c = FX_CosIdx(Rotate);

            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        // Set the material color( diffuse, ambient , specular ) as basic white 
        DEMO_Set3DDefaultMaterial(TRUE, TRUE);
        DEMO_Set3DDefaultShininessTable();

        G3_PolygonAttr((GXLightMask)(count >> 4),       // no lights
                       GX_POLYGONMODE_MODULATE, // modulation mode
                       GX_CULL_BACK,   // cull back
                       0,              // polygon ID(0 - 63)
                       31,             // alpha(0 - 31)
                       0               // OR of GXPolygonAttrMisc's value
            );

        //---------------------------------------------------------------------------
        // Draw a sphere:
        // The geometry data for a sphere is generated in advance(commandList.c).
        // Send them to the geometry FIFO by DMA/CPU, and display a sphere.
        //---------------------------------------------------------------------------
        G3_Begin(GX_BEGIN_TRIANGLES);

#if 1
        // Transfer by DMA
        MI_SendGXCommand(3, sphere1, sphere_size);

#else
        // Transfer by CPU
        {
            int     i;
            for (i = 0; i < sphere_size / 4; ++i)
            {
                reg_G3X_GXFIFO = sphere1[i];
            }
        }
#endif
        G3_End();

        G3_PopMtx(1);

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0xE481B9D5);
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
