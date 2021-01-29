/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2di_RendererMtxCache.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.11 $
 *---------------------------------------------------------------------------*/
 
#ifndef NNS_G2DI_RENDERERMTXCACHE_H_
#define NNS_G2DI_RENDERERMTXCACHE_H_

#include <nnsys/g2d/g2d_RendererCore.h> // For NNSG2dRndCore2DMtxCache, NNSG2dSurfaceType
#include "g2d_Internal.h"
#include "g2di_RendererMtxStack.hpp"// For MatrixStack 


//------------------------------------------------------------------------------
typedef enum OAM_FLIP
{
    OAM_FLIP_NONE   = 0,
    OAM_FLIP_H      = 0x01,
    OAM_FLIP_V      = 0x02,
    OAM_FLIP_HV     = 0x03,
    OAM_FLIP_MAX    = NNS_G2D_OAMFLIP_PATTERN_NUM

}OAM_FLIP;

#define OAM_FLIP_ASSERT( val )                                       \
        NNS_G2D_MINMAX_ASSERT( (val), OAM_FLIP_NONE, OAM_FLIP_HV )    \

// convert from flags to OAM_FLIP
#define TO_OAM_FLIP( bFlipH, bFlipV )           (OAM_FLIP)(( bFlipV << 1 ) | ( bFlipH ))


//------------------------------------------------------------------------------

#define G2Di_NUM_MTX_CACHE                      32



//------------------------------------------------------------------------------
// Renderer 2DMatrix Cache 
// ���ۂ� 2D Graphics Enigine �ւ� Affine �p�����[�^�o�^�� �L���V������������ʂ����܂��B
// �d������ �o�^�� ���s���ꂸ�ߋ��̓o�^���ʂ��Ԃ���܂��B
// 
// ����ɂ���āA���� NNSG2dRndCore2DMtxCache Index �� �w�肵���ꍇ 
// ���� Affine �p�����[�^ ���Q�Ƃ��邱�ƂƂȂ�܂��B
//
// affineIndex �� �K��l�� Affine �p�����[�^�o�^ ���Ȃ���Ă��Ȃ��Ƃ����Ӗ��� 
// MtxCache_NOT_AVAILABLE �������܂��B
// 
// �K��l �ւ� ���Z�b�g �� NNS_G2dEndRendering() �ŌĂ΂�� NNSi_G2dMCMCleanupMtxCache() �ɂ���čs���܂��B
// �܂�AAffine�p�����[�^�����L�\�Ȃ̂� 
// ���� NNS_G2dBeginRendering() NNS_G2dEndRendering() �u���b�N�� �Ɍ����邱�ƂƂȂ�܂��B
//
// 
// �{���W���[���̓����_�����W���[�����璼�ڑ��삳��邱�Ƃ͂Ȃ��A
// RendererMtxState ���W���[�����\�b�h�o�R�ő��삳��܂��B
//
//
// �֐����� NNSi_RMC.....()          
static NNSG2dRndCore2DMtxCache             mtxCacheBuffer_[G2Di_NUM_MTX_CACHE];

//------------------------------------------------------------------------------
// �ǉ��ϐ��FNNSG2dRndCore2DMtxCache�����[�U����B�����邽�߂ɓ���
static u16                          currentMtxCachePos_ = 0;



//------------------------------------------------------------------------------
// ���W���[����������֐�
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// ���W���[���O�����J�֐�
//------------------------------------------------------------------------------
NNS_G2D_INLINE void NNSi_RMCInitMtxCache()
{
    int i;
    for( i = 0; i < G2Di_NUM_MTX_CACHE; i++ )
    {
        NNS_G2dInitRndCore2DMtxCache( &mtxCacheBuffer_[i] );
    }
    currentMtxCachePos_ = 0;
}

NNS_G2D_INLINE void NNSi_RMCResetMtxCache()
{
    int i;
    //
    // �g�p�����Ƃ���܂ŁA����������
    //
    for( i = 0; i < currentMtxCachePos_; i++ )
    {
        NNS_G2dInitRndCore2DMtxCache( &mtxCacheBuffer_[i] );
    }
    currentMtxCachePos_ = 0;
}

//------------------------------------------------------------------------------
// �C���f�b�N�X�ōs��L���b�V�����擾����
NNS_G2D_INLINE NNSG2dRndCore2DMtxCache* NNSi_RMCGetMtxCacheByIdx( u16 idx )
{
    NNS_G2D_MINMAX_ASSERT( idx, 0, G2Di_NUM_MTX_CACHE - 1);
    return &mtxCacheBuffer_[idx];
}             

//------------------------------------------------------------------------------
// �V���ɂЂƂs��L���b�V�����g�p����
NNS_G2D_INLINE u16 NNSi_RMCUseNewMtxCache()
{
    const u16 ret = currentMtxCachePos_;
    
    if( currentMtxCachePos_ < G2Di_NUM_MTX_CACHE - 1)
    {
       currentMtxCachePos_++;
    }else{
       // �s��L���b�V���̌͊�
       NNS_G2D_WARNING( FALSE, "MtxCache is running out. G2d ignores the user request"
                               ", and uses MtxCache-Idx = 31.");
    }
    
    return ret;
}

#endif // NNS_G2DI_RENDERERMTXCACHE_H_