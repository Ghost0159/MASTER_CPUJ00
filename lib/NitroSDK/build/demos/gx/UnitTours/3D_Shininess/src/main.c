/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/3D_Shininess
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.40  2006/01/18 02:11:27  kitase_hirotake
  do-indent

  Revision 1.39  2005/11/21 10:39:46  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.38  2005/03/03 12:39:10  takano_makoto
  static const 定数をdefineに変更

  Revision 1.37  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.36  2004/11/02 10:17:29  takano_makoto
  fix typo.

  Revision 1.35  2004/11/02 09:39:06  takano_makoto
  fix typo.

  Revision 1.34  2004/05/24 10:35:57  takano_makoto
  autotestコードの修正

  Revision 1.33  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.32  2004/04/06 12:48:06  yada
  fix header comment

  Revision 1.31  2004/03/30 08:27:15  yasu
  fix check sum code

  Revision 1.30  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.29  2004/02/18 01:09:52  yasu
  add comment for code32.h

  Revision 1.28  2004/02/12 07:06:26  nishida_kenji
  Avoid generating STRB in THUMB mode.

  Revision 1.25  2004/02/09 01:44:47  kitani_toshikazu
  Add default setting codes for auto testing.

  Revision 1.24  2004/02/06 10:51:23  kitani_toshikazu
  fix comments.

  Revision 1.23  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.22  2004/02/05 01:15:24  kitani_toshikazu
  Added codes for autoTesting.

  Revision 1.21  2004/01/30 11:34:10  kitani_toshikazu
  Removed Tab codes.

  Revision 1.20  2004/01/30 11:11:43  kitani_toshikazu
  Added variations of shininess tables,  and demonstrate these difference of effect .

  Revision 1.19  2004/01/28 06:20:40  kitani_toshikazu
  Adjusted light intensity

  Revision 1.18  2004/01/27 09:00:04  nishida_kenji
  moves G3_LightVector to follow G3_LookAt.

  Revision 1.17  2004/01/22 02:03:25  nishida_kenji
  swap G3_SwapBuffers and SVC_WaitVBlankIntr

  Revision 1.16  2004/01/19 02:15:56  nishida_kenji
  fix comments

  Revision 1.15  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.14  2004/01/18 08:19:33  nishida_kenji
  change the place of SVC_WaitVBlankIntr

  Revision 1.13  2004/01/09 12:02:34  nishida_kenji
  use OS_SetIrqCheckFlag at VBlankIntr()

  Revision 1.12  2004/01/07 09:38:05  nishida_kenji
  revises comments

  Revision 1.11  2004/01/06 12:54:09  nishida_kenji
  revise comments

  Revision 1.10  2003/12/25 11:00:10  nishida_kenji
  converted by GX_APIChangeFrom031212-2.pl

  Revision 1.9  2003/12/25 06:25:02  nishida_kenji
  revise a part of geometry command APIs

  Revision 1.8  2003/12/25 00:19:29  nishida_kenji
  convert INLINE to inline

  Revision 1.7  2003/12/17 09:00:20  nishida_kenji
  Totally revised APIs.
  build/buildtools/GX_APIChangeFrom031212.pl would help you change your program to some extent.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that uses the shininess table:
//
// There are nine spheres displayed. Each of them displayed with different shininess adjustment
// by the shininess table.
// 
// HOWTO:
// 1. Set up the shininess table by G3_Shininess().
// 2. Prepare to use the shininess table by G3_MaterialColorSpecEmi(..., TRUE).
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"
#include "commandList.h"

#define NUM_SHININESS_TYPES     9
static u32 shininess_table[NUM_SHININESS_TYPES][32];


static void drawSphere(BOOL shininess)
{
    //---------------------------------------------------------------------------
    // GX_RGB(0, 0, 0) set except specular color for comparison
    //---------------------------------------------------------------------------
    G3_MaterialColorDiffAmb(GX_RGB(23, 0, 0),   // diffuse
                            GX_RGB(8, 0, 0),    // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(31, 31, 31), // specular
                            GX_RGB(0, 0, 0),    // emission
                            shininess  // use shininess table if TRUE
        );


    G3_PolygonAttr(GX_LIGHTMASK_0,     // lighting
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   0                   // OR of GXPolygonAttrMisc's value
        );

    // Transfer by DMA
    G3_Begin(GX_BEGIN_TRIANGLES);
    MI_SendGXCommand(3, sphere1, sphere_size);
    G3_End();

}

// Power fixed point format( 0.8 ) number 
static inline u8 powFixedPoint8_(u8 base, u8 exponent)
{
    u16     result = (u16)base;
    const u16 LIGHTING_L_DOT_H_SHIFT = 8;

    if (exponent == 0)
        return base;

    while (exponent - 1 > 0)
    {
        result *= (u16)base;
        result = (u16)(result >> LIGHTING_L_DOT_H_SHIFT);
        exponent--;
    }

    return (u8)result;
}

#include <nitro/code32.h>              // avoid byte access problems
// Initialize the shininess table.
static void InitShininessTables_()
{
    u8      i, j;
    u8     *pShininess = NULL;
    const u8 tableLength = 32 * sizeof(u32);

    for (i = 0; i < NUM_SHININESS_TYPES; i++)
    {
        pShininess = (u8 *)&shininess_table[i][0];

        for (j = 0; j < tableLength - 1; j++)
        {
            pShininess[j] = powFixedPoint8_((u8)(j * 2 + 1), i);
        }

        // set the max value specially 
        pShininess[tableLength - 1] = 0xFF;
    }
}


void NitroMain(void)
{
    u8      count = 0;
    u16     Rotate = 0;                // for rotating cubes(0-65535)

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for 3D mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();

    // Initialize shininess tables 
    InitShininessTables_();

    DEMOStartDisplay();
    while (1)
    {
        G3X_Reset();
        count++;
        Rotate += 256;

        //---------------------------------------------------------------------------
        // Set up the camera matrix
        //---------------------------------------------------------------------------
        {
            VecFx32 Eye = { 0, 0, FX32_ONE };   // Eye position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);
        }

        //---------------------------------------------------------------------------
        // Set up light colors and direction.
        // Notice that light vector is transformed by the current vector matrix
        // immediately after LightVector command is issued.
        //---------------------------------------------------------------------------
        {
            const fx32 XY_R = FX_Mul(FX32_SQRT1_3, FX32_SQRT2);

            G3_LightVector(GX_LIGHTID_0,
                           (fx16)FX_Mul(XY_R, FX_SinIdx(Rotate)),
                           (fx16)FX_Mul(XY_R, FX_CosIdx(Rotate)), (fx16)(FX32_SQRT1_3 * -1));

            G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
        }

        G3_PushMtx();

        //---------------------------------------------------------------------------
        // Draw spheres , changing its shininess table.
        //---------------------------------------------------------------------------
        G3_Translate(0, 0, -15 * FX32_ONE);
        {
            u8      i = 0;
            const fx32 step = FX32_ONE * 5;
            const u8 num_row = 3;
            const u8 num_cor = NUM_SHININESS_TYPES / num_row;
            for (i = 0; i < NUM_SHININESS_TYPES; i++)
            {
                int     ix = i % num_row;
                int     iy = i / num_row;
                //---------------------------------------------------------------------------
                // Set up the shininess table
                //---------------------------------------------------------------------------
                G3_Shininess(&shininess_table[i][0]);


                G3_PushMtx();
                G3_Translate(step * ix - step * (num_row / 2), -step * iy + step * (num_cor / 2),
                             0);
                drawSphere(TRUE);
                G3_PopMtx(1);
            }
        }

        G3_PopMtx(1);

        // swapping the polygon list RAM, the vertex RAM, etc.
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);

#ifdef SDK_AUTOTEST
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        EXT_TestSetVRAMForScreenShot(GX_VRAM_LCDC_C);
        EXT_TestScreenShot(100, 0x44C99A39);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt
    }
}

#include <nitro/codereset.h>

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
