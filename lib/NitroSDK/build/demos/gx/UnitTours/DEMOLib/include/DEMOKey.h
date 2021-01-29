/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/DEMOLib
  File:     DEMOKey.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DEMOKey.h,v $
  Revision 1.11  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.10  2005/02/28 05:26:05  yosizaki
  do-indent.

  Revision 1.9  2004/04/07 01:23:26  yada
  fix header comment

  Revision 1.8  2004/04/06 12:48:07  yada
  fix header comment

  Revision 1.7  2004/02/10 02:20:07  yasu
  append extern C

  Revision 1.6  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.5  2004/01/07 09:38:05  nishida_kenji
  revises comments

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DEMO_KEY_H_
#define DEMO_KEY_H_
#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    u16     trigger;
    u16     press;
}
DEMOKeyWork;

extern DEMOKeyWork gKeyWork;
void    DEMOReadKey(void);

#define DEMO_IS_TRIG(key)  (gKeyWork.trigger & key)
#define DEMO_IS_PRESS(key) (gKeyWork.press & key)

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif /* DEMO_KEY_H_ */
