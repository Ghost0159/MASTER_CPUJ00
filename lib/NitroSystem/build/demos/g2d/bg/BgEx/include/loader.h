/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - bg - BgEx
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

#ifdef __cplusplus
extern "C" {
#endif



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
void* LoadNCGR(
    NNSG2dCharacterData** ppCharData,
    const char* pFname,
    NNSFndAllocator* pAllocator
);
void* LoadNCLR(
    NNSG2dPaletteData** ppPltData,
    const char* pFname,
    NNSFndAllocator* pAllocator
);
void* LoadNSCR(
    NNSG2dScreenData** ppScrData,
    const char* pFname,
    NNSFndAllocator* pAllocator
);


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
);

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
);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // LOAD_H_

