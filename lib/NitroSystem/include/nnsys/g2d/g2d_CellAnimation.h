/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_CellAnimation.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.19 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_CELLANIMATION_H_
#define NNS_G2D_CELLANIMATION_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/fmt/g2d_SRTControl_data.h>
#include <nnsys/g2d/g2d_Animation.h>
#include <nnsys/g2d/fmt/g2d_Cell_data.h>

#include <nnsys/g2d/g2d_CellTransferManager.h>


#ifdef __cplusplus
extern "C" {
#endif

//
// �������s�����֐��̕ʖ�
// �݊����ێ��̂��ߕʖ��Ƃ��ĈȑO�̊֐���錾���܂��B
// 
#define NNS_G2dSetCellAnimSpeed                           NNS_G2dSetCellAnimationSpeed
#define NNS_G2dGetCellAnimAnimCtrl                        NNS_G2dGetCellAnimationAnimCtrl                        
#define NNS_G2dInitializeCellAnimation                    NNS_G2dInitCellAnimation
#define NNS_G2dInitializeCellAnimationVramTransfered      NNS_G2dInitCellAnimationVramTransfered 

//------------------------------------------------------------------------------
// �킩��₷���̂��߂ɕʖ����`���܂��B
typedef NNSG2dAnimSequence            NNSG2dCellAnimSequence;
typedef NNSG2dAnimBankData            NNSG2dCellAnimBankData;





/*---------------------------------------------------------------------------*
  Name:         NNSG2dCellAnimation

  Description:  �Z�� �A�j���[�V�����̎��̂�\������T�O�ł��B
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCellAnimation
{
    NNSG2dAnimController            animCtrl;               // �A�j���[�V�����R���g���[��
    const NNSG2dCellData*           pCurrentCell;           // ���ݕ\�����Ă��� �Z�� 
    const NNSG2dCellDataBank*       pCellDataBank;          // �Z���A�j���[�V�������\������Z���̒�`�o���N
    
    u32                             cellTransferStateHandle; // Vram �]�� �^�X�N�̃n���h��
                                                             // Vram �]�� �A�j���[�V�����Ŏg�p���܂�
                                                            
                                                            
    NNSG2dSRTControl                srtCtrl;                // SRT�A�j���[�V�����̌��� 
    
}NNSG2dCellAnimation;




void NNS_G2dInitCellAnimation( NNSG2dCellAnimation* pCellAnim, const NNSG2dAnimSequence* pAnimSeq, const NNSG2dCellDataBank* pCellDataBank );
void NNS_G2dInitCellAnimationVramTransfered
(
    NNSG2dCellAnimation*        pCellAnim, 
    const NNSG2dAnimSequence*   pAnimSeq, 
    const NNSG2dCellDataBank*   pCellBank,
    u32                         vramSettingHandle,
    u32                         dstAddr3D,
    u32                         dstAddr2DMain,
    u32                         dstAddr2DSub,
    const void*                pSrcNCGR,
    const void*                pSrcNCBR,
    u32                         szSrcData
);

void NNS_G2dSetCellAnimationSequence( NNSG2dCellAnimation* pCellAnim, const NNSG2dAnimSequence* pAnimSeq );
void NNS_G2dSetCellAnimationSequenceNoReset( NNSG2dCellAnimation* pCellAnim, const NNSG2dAnimSequence* pAnimSeq );

void NNS_G2dTickCellAnimation( NNSG2dCellAnimation* pCellAnim, fx32 frames );
void NNS_G2dSetCellAnimationCurrentFrame( NNSG2dCellAnimation* pCellAnim, u16 frameIndex );
void NNS_G2dRestartCellAnimation
( 
    NNSG2dCellAnimation*        pCellAnim
);

void NNS_G2dSetCellAnimationSpeed
(
    NNSG2dCellAnimation*     pCellAnim,
    fx32                     speed  
);


u16 NNS_G2dMakeCellToOams
( 
    GXOamAttr*              pDstOams,
    u16                     numDstOam, 
    const NNSG2dCellData*   pCell, 
    const MtxFx22*          pMtxSR, 
    const NNSG2dFVec2*      pBaseTrans,
    u16                     affineIndex,
    BOOL                    bDoubleAffine 
);


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetCellAnimationAnimCtrl

  Description:  �Z���A�j���[�V�����̃A�j���[�V�����R���g���[�����擾���܂��B
                
                
  Arguments:    pCellAnim             : �Z���A�j���[�V���� ����             
                                  
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSG2dAnimController* NNS_G2dGetCellAnimationAnimCtrl
( 
    NNSG2dCellAnimation* pCellAnim 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    return &pCellAnim->animCtrl;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetCellAnimationCurrentCell

  Description:  �Z���A�j���[�V�����̌��ݕ\�����Ă��� �Z�� ���擾���܂��B
                
  Arguments:    pCellAnim             : �Z���A�j���[�V���� ����             
                                  
  Returns:      ���ݕ\�����Ă��� �Z�� 
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dCellData* NNS_G2dGetCellAnimationCurrentCell
( 
    const NNSG2dCellAnimation* pCellAnim 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    return pCellAnim->pCurrentCell;
}

//------------------------------------------------------------------------------
// ���C�u������������J �֐�
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Vram�]���ݒ�p�n���h�����L�����`�F�b�N���܂�
NNS_G2D_INLINE BOOL 
NNSi_G2dIsCellAnimVramTransferHandleValid( const NNSG2dCellAnimation* pCellAnim )
{
    return (BOOL)( pCellAnim->cellTransferStateHandle 
                    != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
}

//------------------------------------------------------------------------------
// Vram�]���Z���A�j���[�V�������`�F�b�N���܂�
NNS_G2D_INLINE BOOL 
NNSi_G2dIsVramTransferCellAnim( const NNSG2dCellAnimation* pCellAnim )
{
    return NNSi_G2dIsCellAnimVramTransferHandleValid( pCellAnim );
}

//------------------------------------------------------------------------------
// �n���h���̐ݒ�
NNS_G2D_INLINE void 
NNSi_G2dSetCellAnimVramTransferHandle( NNSG2dCellAnimation* pCellAnim, u32 handle )
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_ASSERT( handle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
    
    pCellAnim->cellTransferStateHandle = handle;
}

//------------------------------------------------------------------------------
// �n���h���̎擾
NNS_G2D_INLINE u32 
NNSi_G2dGetCellAnimVramTransferHandle( const NNSG2dCellAnimation* pCellAnim )
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    return pCellAnim->cellTransferStateHandle; 
}



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_CELLANIMATION_H_

