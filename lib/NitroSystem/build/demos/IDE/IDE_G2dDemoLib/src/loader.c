/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_G2dDemoLib
  File:     loader.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#include "g2d_demolib.h"
#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Load.h>


/*---------------------------------------------------------------------------*
  Name:         LoadFileToMainMem_

  Description:  �o�b�t�@���m�ۂ��ăt�@�C����ǂݍ��݂܂��B

  Arguments:    pFname:        �ǂݍ��ރt�@�C����

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���̓��e���i�[���Ă���o�b�t�@�ւ�
                �|�C���^��Ԃ��܂��B���̃o�b�t�@�͕s�K�v�ɂȂ����Ƃ��� 
                G2DDemo_Free �ŊJ�����Ȃ���΂Ȃ�܂���B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static void* LoadFileToMainMem_( const char* pFname )
{
    FSFile  file;
    void*   pFile = NULL;
    BOOL    bSuccess;

    SDK_NULL_ASSERT( pFname );

    FS_InitFile( &file );
    bSuccess = FS_OpenFile( &file, pFname );
    
    if( bSuccess )
    {
    	const u32 szFile    = FS_GetLength( &file );


        pFile = G2DDemo_Alloc( szFile );
        SDK_NULL_ASSERT( pFile );

        if( szFile != (u32)FS_ReadFile( &file, pFile, (s32)szFile ) )
        {
            G2DDemo_Free( pFile );
            pFile = NULL;
        }

        bSuccess = FS_CloseFile( &file );
        SDK_ASSERT( bSuccess );
    }else{
        OS_Warning(" Can't find the file : %s ", pFname );
    }

    return pFile;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCER

  Description:  �����Ńo�b�t�@���m�ۂ��� NCER �t�@�C������Z���f�[�^�o���N��
                �ǂݍ��݂܂��B
                �֐�������A�Z���f�[�^�o���N���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppCellBank: �Z���f�[�^�o���N���i�[�����o�b�t�@�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                pFname:     �Z���f�[�^�o���N���i�[����Ă���NCER�t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCER( NNSG2dCellDataBank** ppCellBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCellBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCellBank( pFile, ppCellBank ) )
        {
            NNS_G2dPrintCellBank( *ppCellBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNANR

  Description:  �����Ńo�b�t�@���m�ۂ��� NANR �t�@�C������A�j���[�V����
                �f�[�^�o���N��ǂݍ��݂܂��B
                �֐�������A�A�j���[�V�����f�[�^�o���N���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppAnimBank: �A�j���[�V�����f�[�^�o���N���i�[�����o�b�t�@�ւ�
                            �|�C���^���󂯎��|�C���^�B
                pFname:     �A�j���[�V�����f�[�^�o���N���i�[����Ă���
                            NANR �t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNANR( NNSG2dAnimBankData** ppAnimBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppAnimBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedAnimBank( pFile, ppAnimBank ) )
        {
            NNS_G2dPrintAnimBank( *ppAnimBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}
/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNMCR

  Description:  �����Ńo�b�t�@���m�ۂ��� NMCR �t�@�C������}���`�Z���f�[�^
                �o���N��ǂݍ��݂܂��B
                �֐�������A�}���`�Z���f�[�^�o���N���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppMCBank:   �}���`�Z���f�[�^�o���N���i�[�����o�b�t�@�ւ�
                            �|�C���^���󂯎��|�C���^�B
                pFname:     �}���`�Z���f�[�^�o���N���i�[����Ă���
                            NMCR �t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNMCR( NNSG2dMultiCellDataBank** ppMCBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppMCBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedMultiCellBank( pFile, ppMCBank ) )
        {
            NNS_G2dPrintMultiCellBank ( *ppMCBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNMAR

  Description:  �����Ńo�b�t�@���m�ۂ��� NMAR �t�@�C������}���`�Z��
                �A�j���[�V�����f�[�^�o���N��ǂݍ��݂܂��B
                �֐�������A�}���`�A�j���[�V�����f�[�^�o���N���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppMCABank:  �}���`�Z���A�j���[�V�����f�[�^�o���N���i�[����
                            �o�b�t�@�ւ̃|�C���^���󂯎��|�C���^�B
                pFname:     �}���`�Z���A�j���[�V�����f�[�^�o���N��
                            �i�[����Ă��� NMAR �t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNMAR( NNSG2dMultiCellAnimBankData** ppMCABank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppMCABank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedMCAnimBank( pFile, ppMCABank ) )
        {
            NNS_G2dPrintAnimBank( *ppMCABank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCGR

  Description:  �����Ńo�b�t�@���m�ۂ��� NCGR �t�@�C������L�����N�^�`����
                �L�����N�^�f�[�^��ǂݍ��݂܂��B
                �֐�������A�L�����N�^�f�[�^���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppCharData: �L�����N�^�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                pFname:     �L�����N�^�f�[�^���i�[����Ă���NCGR�t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCGR( NNSG2dCharacterData** ppCharData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCharData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCharacterData( pFile, ppCharData ) )
        {
            NNS_G2dPrintCharacterData( *ppCharData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCGR

  Description:  �����Ńo�b�t�@���m�ۂ��� NCGR �t�@�C������L�����N�^�`����
                �L�����N�^�f�[�^��ǂݍ��݂܂��B
                �֐�������A�L�����N�^�f�[�^���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppCharData:     �L�����N�^�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^��
                                �󂯎��|�C���^�B
                ppCharPosInfo:  �L�����N�^�ǂݍ��݈ʒu�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^��
                                �󂯎��|�C���^�B
                
                pFname:     �L�����N�^�f�[�^���i�[����Ă���NCGR�t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCGREx
( 
    NNSG2dCharacterData**       ppCharData, 
    NNSG2dCharacterPosInfo**    ppCharPosInfo,
    const char*                 pFname 
)
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCharData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCharacterData( pFile, ppCharData ) )
        {
            NNS_G2dPrintCharacterData( *ppCharData );
            
            if( NNS_G2dGetUnpackedCharacterPosInfo( pFile, ppCharPosInfo ) )
            {
                NNS_G2dPrintCharacterPosInfo( *ppCharPosInfo );   
                return pFile;
            }
        }    
    }
    
    G2DDemo_Free( pFile );
    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCLR

  Description:  �����Ńo�b�t�@���m�ۂ��� NCLR �t�@�C������p���b�g�f�[�^��
                �ǂݍ��݂܂��B
                �֐�������A�p���b�g�f�[�^���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppPltData: �p���b�g�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                pFname:     �p���b�g�f�[�^���i�[����Ă���NCLR�t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCLR( NNSG2dPaletteData** ppPltData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppPltData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedPaletteData( pFile, ppPltData ) )
        {
            NNS_G2dPrintPaletteData( *ppPltData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNENR

  Description:  �����Ńo�b�t�@���m�ۂ��� NENR �t�@�C������G���e�B�e�B�f�[�^
                �o���N��ǂݍ��݂܂��B
                �֐�������A�G���e�B�e�B�f�[�^�o���N���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppEntityBank: �G���e�B�e�B�f�[�^�o���N���i�[�����o�b�t�@�ւ�
                            �|�C���^���󂯎��|�C���^�B
                pFname:     �G���e�B�e�B�f�[�^�o���N���i�[����Ă���
                            NENR �t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNENR( NNSG2dEntityDataBank** ppEntityBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppEntityBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedEntityBank( pFile, ppEntityBank ) )
        {
            NNS_G2dPrintEntityDataBank( *ppEntityBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCBR

  Description:  �����Ńo�b�t�@���m�ۂ��� NCBR �t�@�C������r�b�g�}�b�v�`����
                �L�����N�^�f�[�^��ǂݍ��݂܂��B
                �֐�������A�L�����N�^�f�[�^���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppCharData: �L�����N�^�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                pFname:     �L�����N�^�f�[�^���i�[����Ă���NCBR�t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCBR( NNSG2dCharacterData** ppCharData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCharData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCharacterData( pFile, ppCharData ) )
        {
            NNS_G2dPrintCharacterData( *ppCharData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNSCR

  Description:  �����Ńo�b�t�@���m�ۂ��� NCBR �t�@�C������r�b�g�}�b�v�`����
                �L�����N�^�f�[�^��ǂݍ��݂܂��B
                �֐�������A�L�����N�^�f�[�^���s�v�ɂȂ�������
                G2DDemo_Free( �Ԃ�l ) �Ńo�b�t�@���J������K�v������܂��B

  Arguments:    ppCharData: �L�����N�^�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^��
                            �󂯎��|�C���^�B
                pFname:     �L�����N�^�f�[�^���i�[����Ă���NCBR�t�@�C�����B

  Returns:      �ǂݍ��݂ɐ�������ƃt�@�C���f�[�^���i�[����Ă���
                �����Ŋm�ۂ����o�b�t�@�ւ̃|�C���^��Ԃ��܂��B
                �ǂݍ��݂Ɏ��s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNSCR( NNSG2dScreenData** ppScreenData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppScreenData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedScreenData( pFile, ppScreenData ) )
        {
            NNS_G2dPrintScreenData( *ppScreenData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}
