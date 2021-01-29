/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - bg - MinimumBg
  File:     loader.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#ifndef LOAD_H_
#define LOAD_H_

#include <nnsys/fnd.h>
#include <nnsys/g2d.h>

#ifdef __cplusplus
extern "C" {
#endif


void* LoadNCGR( NNSG2dCharacterData** ppCharData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNCLR( NNSG2dPaletteData** ppPltData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNSCR( NNSG2dScreenData** ppScrData, const char* pFname, NNSFndAllocator* pAllocator );



#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // LOAD_H_

