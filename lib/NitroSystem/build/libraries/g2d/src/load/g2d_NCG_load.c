/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d - src - load
  File:     g2d_NCG_load.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.24 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys/g2d/load/g2d_NCG_load.h>
#include <nnsys/g2d/g2d_Load.h>
#include "g2di_Debug.h"


#define NNS_G2D_NCGR_VER_FOR_CHARDATA_BLK_LOADING   NNS_G2dMakeVersionData( (u8)1, (u8)0 )

#ifndef SDK_FINALROM
static const char* StrNNSG2dPixelFmt_ [] =
{
    "INVALID_PixelFmt !! ", //  GX_TEXFMT_NONE       
    "INVALID_PixelFmt !! ", //  GX_TEXFMT_A3I5   
    "INVALID_PixelFmt !! ", //  GX_TEXFMT_PLTT4  
    "GX_TEXFMT_PLTT16"    , //  GX_TEXFMT_PLTT16 
    "GX_TEXFMT_PLTT256"   , //  GX_TEXFMT_PLTT256
    "INVALID_PixelFmt !! ", //  GX_TEXFMT_COMP4x4
    "INVALID_PixelFmt !! ", //  GX_TEXFMT_A5I3   
    "INVALID_PixelFmt !! "  //  GX_TEXFMT_DIRECT   
};

static const char* StrNNSG2dCharacterOrder_ [] =
{
    "NNS_G2D_CHARACTER_FMT_CHAR",
    "NNS_G2D_CHARACTER_FMT_BMP",
    "NNS_G2D_CHARACTER_FMT_MAX"
};

static const char*  GetStrNNSG2dCharacterDataMapingType_ ( GXOBJVRamModeChar type )
{
    switch( type )
    {
    case GX_OBJVRAMMODE_CHAR_2D:        return "GX_OBJVRAMMODE_CHAR_2D           ";
    case GX_OBJVRAMMODE_CHAR_1D_32K:    return "GX_OBJVRAMMODE_CHAR_1D_32K       ";
    case GX_OBJVRAMMODE_CHAR_1D_64K:    return "GX_OBJVRAMMODE_CHAR_1D_64K       ";
    case GX_OBJVRAMMODE_CHAR_1D_128K:   return "GX_OBJVRAMMODE_CHAR_1D_128K      ";
    case GX_OBJVRAMMODE_CHAR_1D_256K:   return "GX_OBJVRAMMODE_CHAR_1D_256K      ";
    default: NNS_G2D_ASSERT(FALSE); return "INVALID_TYPE";
    }
}
#endif // SDK_FINALROM



 
//------------------------------------------------------------------------------
// OBJ �p  (BG�p�̃L�����N�^�f�[�^�ɂ́ANNS_G2dGetUnpackedBGCharacterData()�������p���������B)
//------------------------------------------------------------------------------
BOOL NNS_G2dGetUnpackedCharacterData( void* pNcgrFile, NNSG2dCharacterData** ppCharData )
{
    NNS_G2D_NULL_ASSERT( pNcgrFile );
    NNS_G2D_NULL_ASSERT( ppCharData );
    
    NNS_G2D_ASSERTMSG( NNSi_G2dIsBinFileSignatureValid( pNcgrFile, 
                                                        NNS_G2D_BINFILE_SIG_CHARACTERDATA ),
                     "Input file signature is invalid for this method." );
                     
    NNS_G2D_ASSERTMSG( NNSi_G2dIsBinFileVersionValid( pNcgrFile, 
                                                      NNS_G2D_NCGR_VER_FOR_CHARDATA_BLK_LOADING ), 
                       "Input file is obsolete. Please use the new g2dcvtr.exe." );
                       
    {
        NNSG2dBinaryFileHeader*     pBinFile    = (NNSG2dBinaryFileHeader*)pNcgrFile;
        {
            NNSG2dCharacterDataBlock* pBinBlk   =
                (NNSG2dCharacterDataBlock*)NNS_G2dFindBinaryBlock( pBinFile, 
                                                                   NNS_G2D_BINBLK_SIG_CHARACTERDATA );
            if( pBinBlk )
            {
                
                NNS_G2dUnpackNCG( (void*)&pBinBlk->characterData );
                //
                // OK 
                //
                *ppCharData = &pBinBlk->characterData;
                return TRUE;
            }else{
                //
                // NG
                //
                *ppCharData = NULL;
                return FALSE;
            }
        }
    }
}

//------------------------------------------------------------------------------
void NNS_G2dUnpackNCG( NNSG2dCharacterData* pCharData )
{
    NNS_G2D_NULL_ASSERT( pCharData );
    
    pCharData->pRawData
        = NNS_G2D_UNPACK_OFFSET_PTR( pCharData->pRawData, pCharData );
    
    //
    // �p�����[�^�Q�̐�����
    //
    NNS_G2D_MINMAX_ASSERT( pCharData->pixelFmt
        , GX_TEXFMT_PLTT16, GX_TEXFMT_PLTT256 );
        
    NNS_G2D_MINMAX_ASSERT( pCharData->mapingType
        , GX_OBJVRAMMODE_CHAR_2D, GX_OBJVRAMMODE_CHAR_1D_256K );
        
    NNS_G2D_MINMAX_ASSERT( NNSi_G2dGetCharacterFmtType( pCharData->characterFmt )
        , NNS_G2D_CHARACTER_FMT_CHAR, NNS_G2D_CHARACTER_FMT_MAX );
    
    //
    // 2D �}�b�s���O���[�h�̎��ɂ͗L���ȃT�C�Y���m�F������
    //
    if( (pCharData->characterFmt == NNS_G2D_CHARACTER_FMT_CHAR)
    	&& (pCharData->mapingType == GX_OBJVRAMMODE_CHAR_2D) )
    {
        NNS_G2D_ASSERTMSG( ( pCharData->pixelFmt == GX_TEXFMT_PLTT16 && 
                             pCharData->W == 32 ) || 
                           ( pCharData->pixelFmt == GX_TEXFMT_PLTT256 && 
                             pCharData->W == 16 ) 
                             , "Invalid OBJ character data size for 2D mapping mode." );
    }
    
    NNSI_G2D_DEBUGMSG0("Unpacking NCGR file is successful.\n" );
}



//------------------------------------------------------------------------------
// BG �p (OBJ�p�̃L�����N�^�f�[�^�ɂ́ANNS_G2dGetUnpackedCharacterData()�������p���������B)
//------------------------------------------------------------------------------
BOOL NNS_G2dGetUnpackedBGCharacterData( void* pNcgrFile, NNSG2dCharacterData** ppCharData )
{
    NNS_G2D_NULL_ASSERT( pNcgrFile );
    NNS_G2D_NULL_ASSERT( ppCharData );
    
    NNS_G2D_ASSERTMSG( NNSi_G2dIsBinFileSignatureValid( pNcgrFile, 
                                                        NNS_G2D_BINFILE_SIG_CHARACTERDATA ),
                     "Input file signature is invalid for this method." );
                     
    NNS_G2D_ASSERTMSG( NNSi_G2dIsBinFileVersionValid( pNcgrFile, 
                                                      NNS_G2D_NCGR_VER_FOR_CHARDATA_BLK_LOADING ), 
                       "Input file is obsolete. Please use the new g2dcvtr.exe." );
                       
    {
        NNSG2dBinaryFileHeader*     pBinFile    = (NNSG2dBinaryFileHeader*)pNcgrFile;
        {
            NNSG2dCharacterDataBlock* pBinBlk   =
                (NNSG2dCharacterDataBlock*)NNS_G2dFindBinaryBlock( pBinFile, 
                                                                   NNS_G2D_BINBLK_SIG_CHARACTERDATA );
            if( pBinBlk )
            {
                
                NNS_G2dUnpackBGNCG( (void*)&pBinBlk->characterData );
                //
                // OK 
                //
                *ppCharData = &pBinBlk->characterData;
                return TRUE;
            }else{
                //
                // NG
                //
                *ppCharData = NULL;
                return FALSE;
            }
        }
    }
}

//------------------------------------------------------------------------------
// �ʒu���u���b�N�̎擾
//------------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetUnpackedCharacterPosInfo

  Description:  NCBR NCGR �t�@�C������A�L�����N�^�f�[�^���[�h�ʒu���u���b�N���擾���܂��B
                
                �L�����N�^�f�[�^���[�h�ʒu���u���b�N�́A�L�����N�^�f�[�^��VRAM�ɓǂݍ��ލ�
                �ǂ̈ʒu�Ƀ��[�h�����ׂ��f�[�^�ł��邩�Ƃ�������\���Ă��܂��B
                
                �L�����N�^�f�[�^���[�h�ʒu���u���b�N�́A
                2D �}�b�s���O���[�h�Ŋi�[����Ă��� NCGR, NCBR �t�@�C���݂̂Ŏ擾�\�ł��B
                
  Arguments:    pNcgrFile         NCG�t�@�C���������|�C���^
                ppCharPosInfo     �L�����N�^�f�[�^���[�h�ʒu���
                                    
  Returns:      �擾�ɐ��������ꍇ TRUE
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dGetUnpackedCharacterPosInfo
( 
    void*                        pNcgrFile, 
    NNSG2dCharacterPosInfo**     ppCharPosInfo 
)
{
    NNS_G2D_NULL_ASSERT( pNcgrFile );
    NNS_G2D_NULL_ASSERT( ppCharPosInfo );
    
    NNS_G2D_ASSERTMSG( NNSi_G2dIsBinFileSignatureValid( pNcgrFile, 
                                                        NNS_G2D_BINFILE_SIG_CHARACTERDATA ),
                     "Input file signature is invalid for this method." );
                     
    NNS_G2D_ASSERTMSG( NNSi_G2dIsBinFileVersionValid( pNcgrFile, 
                                                      BIN_FILE_VERSION( NCGR ) ), 
                       "Input file is obsolete. Please use the new g2dcvtr.exe." );
                       
    {
        NNSG2dBinaryFileHeader*     pBinFile    = (NNSG2dBinaryFileHeader*)pNcgrFile;
        {
            NNSG2dCharacterPosInfoBlock* pBinBlk   =
                (NNSG2dCharacterPosInfoBlock*)NNS_G2dFindBinaryBlock( pBinFile, 
                                                                      NNS_G2D_BINBLK_SIG_CHAR_POSITION );
            if( pBinBlk )
            {
                *ppCharPosInfo = &pBinBlk->posInfo;
                return TRUE;
            }else{
                //
                // NG
                //
                *ppCharPosInfo = NULL;
                return FALSE;
            }
        }
    }
}

//------------------------------------------------------------------------------
void NNS_G2dUnpackBGNCG( NNSG2dCharacterData* pCharData )
{
    NNS_G2D_NULL_ASSERT( pCharData );
    
    pCharData->pRawData
        = NNS_G2D_UNPACK_OFFSET_PTR( pCharData->pRawData, pCharData );
    
    //
    // �p�����[�^�Q�̐�����
    //
    NNS_G2D_MINMAX_ASSERT( pCharData->pixelFmt
        , GX_TEXFMT_PLTT16, GX_TEXFMT_PLTT256 );
        
    NNS_G2D_MINMAX_ASSERT( pCharData->mapingType
        , GX_OBJVRAMMODE_CHAR_2D, GX_OBJVRAMMODE_CHAR_1D_256K );
        
	// �L�����N�^�`���̂�OK�ABMP�`����NG
    NNS_G2D_ASSERT( NNSi_G2dGetCharacterFmtType( pCharData->characterFmt )
    	== NNS_G2D_CHARACTER_FMT_CHAR );

    NNSI_G2D_DEBUGMSG0("Unpacking NCGR file is successful.\n" );
}



//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
#ifndef SDK_FINALROM 

//------------------------------------------------------------------------------
// �f�o�b�N���o��(�L�����N�^�f�[�^)
void NNS_G2dPrintCharacterData( const NNSG2dCharacterData* pCharData )
{
    NNS_G2D_NULL_ASSERT( pCharData );
    {
        const NNSG2dCharacterFmt charFmt = NNSi_G2dGetCharacterFmtType( pCharData->characterFmt );
        
        OS_Printf( "---------------------------------------------\n" );
        OS_Printf( " Character Data (NCG) \n" );
        OS_Printf( " szByte         = %d \n", pCharData->szByte );
        
        //
        // �PD�}�b�s���O���ɂ� H,W �ɂ� NNS_G2D_1D_MAPPING_CHAR_SIZE ���R���o�[�^�c�[���ɂĐݒ肳��܂�
        //
        {
            if( pCharData->W == NNS_G2D_1D_MAPPING_CHAR_SIZE )
            {
                OS_Printf( " W              = %s \n", "Not used( 1D mapping data.)" );
            }else{
                OS_Printf( " W              = %d \n", pCharData->W );
            }
            
            if( pCharData->H == NNS_G2D_1D_MAPPING_CHAR_SIZE )
            {
                OS_Printf( " H              = %s \n", "Not used( 1D mapping data.)" );
            }else{
                OS_Printf( " H              = %d \n", pCharData->H );
            }
        }
        
        OS_Printf( " pixelFmt       = %s \n", StrNNSG2dPixelFmt_[ pCharData->pixelFmt ] );
        OS_Printf( " mapingType     = %s \n", GetStrNNSG2dCharacterDataMapingType_( pCharData->mapingType ) );
        OS_Printf( " characterFmt   = %s \n", StrNNSG2dCharacterOrder_[ charFmt ] );
        OS_Printf( " isVramTransfer = %d \n", NNSi_G2dIsCharacterVramTransfered( pCharData->characterFmt ) );
        OS_Printf( "---------------------------------------------\n" );
    }
}

//------------------------------------------------------------------------------
// �f�o�b�N���o��(�L�����N�^�ʒu���)
void NNS_G2dPrintCharacterPosInfo( const NNSG2dCharacterPosInfo* pPosInfo )
{
    NNS_G2D_NULL_ASSERT( pPosInfo );
    {
        OS_Printf( "---------------------------------------------\n" );
        OS_Printf( " Character PosInfo (NCG) \n" );
        OS_Printf( " srcPosX    = %d \n", pPosInfo->srcPosX );
        OS_Printf( " srcPosY    = %d \n", pPosInfo->srcPosY );
        OS_Printf( " srcW       = %d \n", pPosInfo->srcW );
        OS_Printf( " srcH       = %d \n", pPosInfo->srcH );
        OS_Printf( "---------------------------------------------\n" );
    }
}

#endif // SDK_FINALROM 

