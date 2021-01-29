/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - demolib - include - g2d_demolib
  File:     loader.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/

#ifndef G2D_DEMOLIB_LOADER_H_ 
#define G2D_DEMOLIB_LOADER_H_

#ifdef __cplusplus
extern "C" {
#endif


void* G2DDemo_LoadNCER( NNSG2dCellDataBank** ppCellBank, const char* pFname );
void* G2DDemo_LoadNANR( NNSG2dAnimBankData** ppAnimBank, const char* pFname );
void* G2DDemo_LoadNMCR( NNSG2dMultiCellDataBank** ppMCBank, const char* pFname );
void* G2DDemo_LoadNMAR( NNSG2dMultiCellAnimBankData** ppMCABank, const char* pFname );
void* G2DDemo_LoadNCGR( NNSG2dCharacterData** ppCharData, const char* pFname );
void* G2DDemo_LoadNCLR( NNSG2dPaletteData** ppPltData, const char* pFname );
void* G2DDemo_LoadNENR( NNSG2dEntityDataBank** ppEntityBank, const char* pFname );
void* G2DDemo_LoadNCBR( NNSG2dCharacterData** ppCharData, const char* pFname );
void* G2DDemo_LoadNSCR( NNSG2dScreenData** ppScreenData, const char* pFname );

void* G2DDemo_LoadNCGREx( NNSG2dCharacterData** ppCharData, NNSG2dCharacterPosInfo** ppCharPosInfo, const char* pFname );

#ifdef __cplusplus
}/* extern "C" */
#endif

// G2D_DEMOLIB_LOADER_H_
#endif
