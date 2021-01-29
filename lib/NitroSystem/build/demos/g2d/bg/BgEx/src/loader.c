/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - bg - BgEx
  File:     loader.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "loader.h"


#define LoadXXXX(ppData, pFname, pAlloc, unpack, print) \
    void*   pFile = NULL;                               \
                                                        \
    SDK_NULL_ASSERT( ppData );                          \
    SDK_NULL_ASSERT( pFname );                          \
                                                        \
    pFile = LoadFileToMainMem_( pFname, pAlloc );       \
    if( pFile != NULL )                                 \
    {                                                   \
        if( unpack( pFile, ppData ) )                   \
        {                                               \
            print( *ppData );                           \
            return pFile;                               \
        }                                               \
                                                        \
        NNS_FndFreeToAllocator( pAlloc, pFile );        \
    }                                                   \
                                                        \
    return NULL;                                        \


/*---------------------------------------------------------------------------*
  Name:         LoadFileToMainMem_

  Description:  �w�肳�ꂽ�t�@�C�����������Ƀ��[�h���܂��B

  Arguments:    pFname:     ���[�h����t�@�C���̃t�@�C�����B
                pAllocator: �������A���P�[�^�ւ̃|�C���^�B

  Returns:      �֐������������ꍇ�� pAllocator �Ŋm�ۂ����������̈�擪�ւ�
                �|�C���^��Ԃ��܂��B
                �֐������s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static void* LoadFileToMainMem_( const char* pFname, NNSFndAllocator* pAllocator )
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


        pFile = NNS_FndAllocFromAllocator( pAllocator, szFile );
        SDK_NULL_ASSERT( pFile );

        if( szFile != (u32)FS_ReadFile( &file, pFile, (s32)szFile ) )
        {
            NNS_FndFreeToAllocator( pAllocator, pFile );
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
  Name:         LoadN**R

  Description:  G2D�o�C�i���t�@�C�����������Ƀ��[�h���A
                �����^�C���\���̂��\�z���܂��B

  Arguments:    ppCharData: �L�����N�^�f�[�^�ւ̃|�C���^���i�[����o�b�t�@�ւ�
                            �|�C���^�B
                ppPltData:  �p���b�g�f�[�^�ւ̃|�C���^���i�[����o�b�t�@�ւ�
                            �|�C���^�B
                ppScrData:  �X�N���[���f�[�^�ւ̃|�C���^���i�[����o�b�t�@�ւ�
                            �|�C���^�B
                pFname:     G2D �o�C�i���t�@�C���̃t�@�C�����B
                pAllocator: �������A���P�[�^�ւ̃|�C���^�B

  Returns:      �֐������������ꍇ�� pAllocator �Ŋm�ۂ����������̈�擪�ւ�
                �|�C���^��Ԃ��܂��B
                �֐������s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* LoadNSCR( NNSG2dScreenData** ppScrData, const char* pFname, NNSFndAllocator* pAllocator )
{
    LoadXXXX(ppScrData, pFname, pAllocator, NNS_G2dGetUnpackedScreenData, NNS_G2dPrintScreenData);
}

void* LoadNCLR( NNSG2dPaletteData** ppPltData, const char* pFname, NNSFndAllocator* pAllocator )
{
    LoadXXXX(ppPltData, pFname, pAllocator, NNS_G2dGetUnpackedPaletteData, NNS_G2dPrintPaletteData);
}

void* LoadNCGR( NNSG2dCharacterData** ppCharData, const char* pFname, NNSFndAllocator* pAllocator )
{
    LoadXXXX(ppCharData, pFname, pAllocator, NNS_G2dGetUnpackedBGCharacterData, NNS_G2dPrintCharacterData);
}



/*---------------------------------------------------------------------------*
  Name:         LoadNCGREx

  Description:  �����L�����N�^��G2D�o�C�i���t�@�C�����������Ƀ��[�h���A
                �����^�C���\���̂��\�z���܂��B

  Arguments:    ppCharData: �L�����N�^�f�[�^�ւ̃|�C���^���i�[����o�b�t�@�ւ�
                            �|�C���^�B
                ppPosInfo:  �����L�����N�^�ʒu���f�[�^�ւ̃|�C���^���i�[����
                            �o�b�t�@�ւ̃|�C���^�B
                pFname:     �����L�����N�^�� G2D �o�C�i���t�@�C���̃t�@�C�����B
                pAllocator: �������A���P�[�^�ւ̃|�C���^�B

  Returns:      �֐������������ꍇ�� pAllocator �Ŋm�ۂ����������̈�擪�ւ�
                �|�C���^��Ԃ��܂��B
                �֐������s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* LoadNCGREx(
    NNSG2dCharacterData** ppCharData,
    NNSG2dCharacterPosInfo** ppPosInfo,
    const char* pFname,
    NNSFndAllocator* pAllocator
)
{
    void* pFile;
    BOOL bSuccess;

    pFile = LoadNCGR(ppCharData, pFname, pAllocator);
    bSuccess = NNS_G2dGetUnpackedCharacterPosInfo(pFile, ppPosInfo);

    if( ! bSuccess )
    {
        *ppPosInfo = NULL;
    }

    return pFile;
}



/*---------------------------------------------------------------------------*
  Name:         LoadNCLREx

  Description:  �����L�����N�^��G2D�o�C�i���t�@�C�����������Ƀ��[�h���A
                �����^�C���\���̂��\�z���܂��B

  Arguments:    ppCharData: �p���b�g�f�[�^�ւ̃|�C���^���i�[����o�b�t�@�ւ�
                            �|�C���^�B
                ppPosInfo:  �p���b�g���k�f�[�^�ւ̃|�C���^���i�[����
                            �o�b�t�@�ւ̃|�C���^�B
                pFname:     ���k�p���b�g�� G2D �o�C�i���t�@�C���̃t�@�C�����B
                pAllocator: �������A���P�[�^�ւ̃|�C���^�B

  Returns:      �֐������������ꍇ�� pAllocator �Ŋm�ۂ����������̈�擪�ւ�
                �|�C���^��Ԃ��܂��B
                �֐������s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void* LoadNCLREx(
    NNSG2dPaletteData** ppPltData,
    NNSG2dPaletteCompressInfo** ppCmpInfo,
    const char* pFname,
    NNSFndAllocator* pAllocator
)
{
    void* pFile;
    BOOL bSuccess;

    pFile = LoadNCLR(ppPltData, pFname, pAllocator);
    bSuccess = NNS_G2dGetUnpackedPaletteCompressInfo(pFile, ppCmpInfo);

    if( ! bSuccess )
    {
        *ppCmpInfo = NULL;
    }

    return pFile;
}

