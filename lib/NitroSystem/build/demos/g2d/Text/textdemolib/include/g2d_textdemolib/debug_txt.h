/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib - include - g2d_textdemolib
  File:     debug_txt.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/
#ifndef DEBUG_TXT_H_
#define DEBUG_TXT_H_

#include <nnsys/g2d/g2d_Font.h>

#ifdef __cplusplus
extern "C" {
#endif


#define DEBUG_FONTRESOURCE_NAME     "/data/fontd.NFTR"

void DTX_Init(void);
void DTX_Reflect(void);
void DTX_Print( int x, int y, const char* str );
void DTX_PrintLine( const char* fmt, ... );

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // DEBUG_TXT_H_

