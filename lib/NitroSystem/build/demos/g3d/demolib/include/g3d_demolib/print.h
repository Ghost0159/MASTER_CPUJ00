/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib - include - g3d_demolib
  File:     print.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_G3D_DEMOLIB_PRINT_H_ 
#define NNS_G3D_DEMOLIB_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif


enum {
	G3DDEMO_COLOR_BLACK	= 0,
	G3DDEMO_COLOR_BLUE,
	G3DDEMO_COLOR_RED,
	G3DDEMO_COLOR_MAGENTA,
	G3DDEMO_COLOR_GREEN,
	G3DDEMO_COLOR_CYAN,
	G3DDEMO_COLOR_YELLOW,
	G3DDEMO_COLOR_WHITE
};

void G3DDemo_InitConsole(void);
void G3DDemo_ClearConsole(void);

void G3DDemo_Print(int x, int y, int color, const char* string);
void G3DDemo_Printf(int x, int y, int color, const char* format, ...);
void G3DDemo_PrintApplyToHW(void);


#ifdef __cplusplus
}/* extern "C" */
#endif

// NNS_G3D_DEMOLIB_PRINT_H_
#endif
