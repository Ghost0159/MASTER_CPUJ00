/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
  File:     DEMOUtility.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DEMOUtility.h,v $
  Revision 1.8  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.7  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.6  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.5  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.4  2004/02/10 02:20:07  yasu
  append extern C

  Revision 1.3  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/30 11:15:14  kitani_toshikazu
  Added DEMO_Set3DDefaultShininessTable() to control  specular reflection.

  Revision 1.1  2004/01/28 06:20:40  kitani_toshikazu
  Adjusted light intensity

  
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DEMO_UTILITY_H_
#define DEMO_UTILITY_H_
#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

void    DEMO_Set3DDefaultMaterial(BOOL bUsediffuseAsVtxCol, BOOL bUseShininessTbl);
void    DEMO_Set3DDefaultShininessTable();

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif /* DEMO_UTILITY_H_ */
