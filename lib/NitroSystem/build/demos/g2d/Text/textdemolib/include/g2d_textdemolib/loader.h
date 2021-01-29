/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib - include - g2d_textdemolib
  File:     loader.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/
#ifndef LOAD_H_
#define LOAD_H_

#include <nnsys/fnd.h>
#include <nnsys/g2d.h>
#include <nnsys/g2d/g2d_Font.h>
#include <nnsys/g2d/load/g2d_NFT_load.h>

#ifdef __cplusplus
extern "C" {
#endif


void* LoadNCER( NNSG2dCellDataBank** ppCellBank, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNANR( NNSG2dAnimBankData** ppAnimBank, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNMCR( NNSG2dMultiCellDataBank** ppMCBank, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNMAR( NNSG2dMultiCellAnimBankData** ppMCABank, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNCGR( NNSG2dCharacterData** ppCharData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNCLR( NNSG2dPaletteData** ppPltData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNENR( NNSG2dEntityDataBank** ppEntityBank, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNCBR( NNSG2dCharacterData** ppCharData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNSCR( NNSG2dScreenData** ppScrData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNCGRforBG( NNSG2dCharacterData** ppCharData, const char* pFname, NNSFndAllocator* pAllocator );
void* LoadNFTR( NNSG2dFont* pFont, const char* pFname, NNSFndAllocator* pAllocator );



#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // LOAD_H_

