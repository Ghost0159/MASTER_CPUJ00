/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - load
  File:     g2d_NFT_load.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_G2D_NFT_LOAD_H_
#define NNS_G2D_NFT_LOAD_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/g2d_Data.h>
#include <nnsys/g2d/g2d_Font.h>

#ifdef __cplusplus
extern "C" {
#endif



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetUnpackedFont

  Description:  NFTR �t�@�C���� NITRO�t�H���g�ւƓW�J���܂��B

  Arguments:    pNftrFile:  NFTR �t�@�C���f�[�^�ւ̃|�C���^�B
                ppFont:     NITRO�t�H���g�ւ̃|�C���^���i�[����o�b�t�@�ւ�
                            �|�C���^�B

  Returns:      �W�J�ɐ��������� TRUE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
BOOL NNSi_G2dGetUnpackedFont( void* pNftrFile, NNSG2dFontInformation** ppFont );



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dUnpackNFT

  Description:  NFTR �t�@�C���f�[�^�����C�u�����ŗ��p�ł���`�ւƓW�J���܂��B
                �e�t�@�C���f�[�^�ɑ΂��ă��C�u�����ł̎g�p�̑O��1�񂾂�
                ���s����K�v������܂��B

  Arguments:    pHeader:    NFTR �t�@�C���̃o�C�i���t�@�C���w�b�_�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNSi_G2dUnpackNFT(NNSG2dBinaryFileHeader* pHeader);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dPrintFont

  Description:  NITRO�t�H���g�̏����f�o�b�O�o�͂ɏo�͂��܂��B

  Arguments:    pFont:  NITRO�t�H���g�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#ifdef SDK_FINALROM
    NNS_G2D_INLINE void NNS_G2dPrintFont( const NNSG2dFont* /*pFont*/ )
    {
    }
#else // SDK_FINALROM
    void NNS_G2dPrintFont( const NNSG2dFont* pFont );
#endif // SDK_FINALROM



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_NFT_LOAD_H_

