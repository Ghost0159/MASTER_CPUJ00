/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
  File:     DEMOUtility.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DEMOUtility.c,v $
  Revision 1.12  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.11  2005/03/03 12:45:03  takano_makoto
  static const ’è”‚ðdefine‚É•ÏX

  Revision 1.10  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.9  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.8  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.7  2004/02/18 01:01:16  yasu
  add comment for code32.h

  Revision 1.6  2004/02/12 07:06:26  nishida_kenji
  Avoid generating STRB in THUMB mode.

  Revision 1.4  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.3  2004/01/30 11:34:10  kitani_toshikazu
  Removed Tab codes.

  Revision 1.2  2004/01/30 11:15:14  kitani_toshikazu
  Added DEMO_Set3DDefaultShininessTable() to control  specular reflection.

  Revision 1.1  2004/01/28 06:20:40  kitani_toshikazu
  Adjusted light intensity

  
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "DEMOUtility.h"

#define DEMO_INTENSITY_DF   23
#define DEMO_INTENSITY_AM   8
#define DEMO_INTENSITY_SP   31

static const GXRgb DEMO_DIFFUSE_COL =
GX_RGB(DEMO_INTENSITY_DF, DEMO_INTENSITY_DF, DEMO_INTENSITY_DF);
static const GXRgb DEMO_AMBIENT_COL =
GX_RGB(DEMO_INTENSITY_AM, DEMO_INTENSITY_AM, DEMO_INTENSITY_AM);
static const GXRgb DEMO_SPECULAR_COL =
GX_RGB(DEMO_INTENSITY_SP, DEMO_INTENSITY_SP, DEMO_INTENSITY_SP);
static const GXRgb DEMO_EMISSION_COL = GX_RGB(0, 0, 0);

static const u32 LIGHTING_L_DOT_S_SHIFT = 8;


void DEMO_Set3DDefaultMaterial(BOOL bUsediffuseAsVtxCol, BOOL bUseShininessTbl)
{
    G3_MaterialColorDiffAmb(DEMO_DIFFUSE_COL,   // diffuse
                            DEMO_AMBIENT_COL,   // ambient
                            bUsediffuseAsVtxCol // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(DEMO_SPECULAR_COL,  // specular
                            DEMO_EMISSION_COL,  // emission
                            bUseShininessTbl    // use shininess table if TRUE
        );
}


#include <nitro/code32.h>              // avoid byte access problems
void DEMO_Set3DDefaultShininessTable()
{
    u8      i;
    u32     shininess_table[32];
    u8     *pShininess = (u8 *)&shininess_table[0];
    const u8 tableLength = 32 * sizeof(u32);

    for (i = 0; i < tableLength - 1; i++)
    {
        // ShininessTable is ( 0.8 ) fixed point foramt, so we have to right-shift 8 bit for One multiply.
        // pShininess = (i*2+1)^4
        pShininess[i] = (u8)(((s64)(i * 2 + 1) * (i * 2 + 1) * (i * 2 + 1) * (i * 2 + 1)) >> (LIGHTING_L_DOT_S_SHIFT * (4 - 1)));       // byte access
    }

    // set the max value specialy   
    pShininess[tableLength - 1] = 0xFF;

    G3_Shininess(&shininess_table[0]);
}

#include <nitro/codereset.h>
