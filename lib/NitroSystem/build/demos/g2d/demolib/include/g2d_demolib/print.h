/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - demolib - include - g2d_demolib
  File:     print.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

#ifndef G2D_DEMOLIB_PRINT_H_ 
#define G2D_DEMOLIB_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif


void G2DDemo_PrintInit(void);
void G2DDemo_PrintClear(void);
void G2DDemo_PrintOut(int x, int y, const char* string);
void G2DDemo_PrintOutf(int x, int y, const char* format, ...);
void G2DDemo_PrintApplyToHW(void);


#ifdef __cplusplus
}/* extern "C" */
#endif

// G2D_DEMOLIB_PRINT_H_
#endif
