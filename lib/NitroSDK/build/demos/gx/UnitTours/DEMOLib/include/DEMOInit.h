/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
  File:     DEMOInit.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DEMOInit.h,v $
  Revision 1.12  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.11  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.10  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.9  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.8  2004/02/10 02:20:07  yasu
  append extern C

  Revision 1.7  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.6  2004/02/02 06:34:17  nishida_kenji
  Add functions for the sub display.

  Revision 1.5  2004/01/18 09:20:26  nishida_kenji
  adds DEMOStartDisplay

  Revision 1.4  2004/01/07 09:38:05  nishida_kenji
  revises comments

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DEMO_INIT_H_
#define DEMO_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void VBlankIntr(void);

void    DEMOInitCommon(void);
void    DEMOInitVRAM(void);
void    DEMOInitDisplayBG0Only(void);
void    DEMOInitDisplaySubBG0Only(void);
void    DEMOInitDisplayBG2Only(void);
void    DEMOInitDisplaySubBG2Only(void);
void    DEMOInitDisplayOBJOnly(void);
void    DEMOInitDisplaySubOBJOnly(void);
void    DEMOInitDisplay3D(void);
void    DEMOInit(void);

void    DEMOStartDisplay(void);

#ifdef __cplusplus
}/* extern "C" */
#endif

/* DEMO_INIT_H_ */
#endif
