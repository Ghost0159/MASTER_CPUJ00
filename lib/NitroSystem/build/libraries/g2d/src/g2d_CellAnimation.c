/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_CellAnimation.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.40 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys/g2d/g2d_CellAnimation.h>
#include <nnsys/g2d/g2d_SRTControl.h>

#include <nnsys/g2d/load/g2d_NCE_load.h>
#include <nnsys/g2d/fmt/g2d_Oam_data.h>

#include "g2d_Internal.h"

//------------------------------------------------------------------------------
// �A�j���[�V�����ύX���ʂ�K�p����
//
// �������\�����A�ǂ�ȃA�j���t�H�[�}�b�g�ł����Ă�NNSG2dAnimDataSRT�ŃA�N�Z�X���Ă��܂�
// (�A�j���t�H�[�}�b�g��NNS_G2D_ANIMELEMENT_INDEX�̏ꍇ�� SRT �����ɂ͕s���ȃf�[�^�����鎖�ƂȂ�j
//     
static void ApplyCurrentAnimResult_( NNSG2dCellAnimation* pCellAnim )
{
    const NNSG2dAnimDataSRT*   pAnimResult  = NULL;
    const NNSG2dCellDataBank*  pCellBank    = NULL;
    
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pCellAnim->pCellDataBank );
    
    //
    // �\�����Ԃ��[���ł���A�A�j���[�V�����t���[���̏ꍇ�́A�X�V��Ƃ������s���܂���B
    //
    if( pCellAnim->animCtrl.pActiveCurrent->frames == 0 )
    {
        return; 
    }
    pAnimResult = (const NNSG2dAnimDataSRT*)NNS_G2dGetAnimCtrlCurrentElement( &pCellAnim->animCtrl );
    NNS_G2D_NULL_ASSERT( pAnimResult );

    
    //
    // �����̃t�H�[�}�b�g�ɑΉ����邱�ƂɂȂ�\��ł�
    //
    pAnimResult = (const NNSG2dAnimDataSRT*)NNS_G2dGetAnimCtrlCurrentElement( &pCellAnim->animCtrl );
    pCellBank   = pCellAnim->pCellDataBank;
    
    NNSI_G2D_DEBUGMSG0( "pAnimResult->index = %d\n", pAnimResult->index );
    
    pCellAnim->pCurrentCell 
        = NNS_G2dGetCellDataByIdx( pCellBank, pAnimResult->index );
    NNS_G2D_NULL_ASSERT( pCellAnim->pCurrentCell );
    
    //
    // SRT �� ���f 
    //
    {
       const NNSG2dAnimationElement elemType 
           = NNSi_G2dGetAnimSequenceElementType( pCellAnim->animCtrl.pAnimSequence->animType );
       NNSi_G2dSrtcInitControl  ( &pCellAnim->srtCtrl, NNS_G2D_SRTCONTROLTYPE_SRT );
       
       if( elemType != NNS_G2D_ANIMELEMENT_INDEX )
       {
           if( elemType == NNS_G2D_ANIMELEMENT_INDEX_T )
           {
              const NNSG2dAnimDataT*  pAnmResT = (const NNSG2dAnimDataT*)pAnimResult;
              NNSi_G2dSrtcSetTrans     ( &pCellAnim->srtCtrl, pAnmResT->px, pAnmResT->py );// T
           }else{
              NNSi_G2dSrtcSetSRTScale  ( &pCellAnim->srtCtrl, pAnimResult->sx, pAnimResult->sy );// S    
              NNSi_G2dSrtcSetSRTRotZ   ( &pCellAnim->srtCtrl, pAnimResult->rotZ );// R    
              NNSi_G2dSrtcSetTrans     ( &pCellAnim->srtCtrl, pAnimResult->px, pAnimResult->py );// T    
           }
       }
    }
    
    
   
    //
    // ����VRAM�]����񂪐ݒ肳��Ă���Ȃ�...
    //
    if( NNS_G2dCellDataBankHasVramTransferData( pCellBank ) && 
        NNSi_G2dIsCellAnimVramTransferHandleValid( pCellAnim ) )
    {
        const NNSG2dCellVramTransferData*   pCellTransferData 
            = NNSi_G2dGetCellVramTransferData( pCellBank, pAnimResult->index );   
        //
        // �]�������N�G�X�g���܂�
        //
        NNS_G2dSetCellTransferStateRequested( pCellAnim->cellTransferStateHandle,
                                              pCellTransferData->srcDataOffset,
                                              pCellTransferData->szByte );
    }
}

//------------------------------------------------------------------------------
// �Z���A�j���[�V���������������܂��B
// NNS_G2dInitCellAnimation �̎��������ł��B
static NNS_G2D_INLINE void InitCellAnimationImpl_
( 
    NNSG2dCellAnimation*        pCellAnim, 
    const NNSG2dAnimSequence*   pAnimSeq, 
    const NNSG2dCellDataBank*   pCellDataBank,
    u32                         cellTransferStateHandle 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pAnimSeq );
    NNS_G2D_NULL_ASSERT( pCellDataBank );
    

    pCellAnim->pCellDataBank            = pCellDataBank;
    pCellAnim->cellTransferStateHandle   = cellTransferStateHandle;
    
    // TODO: srtCtrl �� ��� �� �A�j���[�V�����̌`���ɉ����āA�ݒ肷��K�v������
    //       �o�C���h�̂��тɍēx�ݒ肷��K�v������B
    NNSi_G2dSrtcInitControl( &pCellAnim->srtCtrl, NNS_G2D_SRTCONTROLTYPE_SRT );
    
    NNS_G2dInitAnimCtrl( &pCellAnim->animCtrl );
    NNS_G2dSetCellAnimationSequence( pCellAnim, pAnimSeq );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitCellAnimation

  Description:  NNSG2dCellAnimation  ���̂����������܂�
                
  Arguments:    pCellAnim:           [OUT]  �Z���A�j���[�V��������
                pAnimSeq:            [IN]   �A�j���[�V�����f�[�^
                pCellDataBank:       [IN]   �Z���f�[�^�o���N
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitCellAnimation
( 
    NNSG2dCellAnimation*        pCellAnim, 
    const NNSG2dAnimSequence*   pAnimSeq, 
    const NNSG2dCellDataBank*   pCellDataBank 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pAnimSeq );
    NNS_G2D_NULL_ASSERT( pCellDataBank );
    
    InitCellAnimationImpl_( pCellAnim, 
                            pAnimSeq, 
                            pCellDataBank, 
                            NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE ); 
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitCellAnimationVramTransfered

  Description:  VRAM�]���A�j���[�V����������A�Z���A�j���[�V���������������܂��B
                
  Arguments:    pCellAnim:           [OUT] �Z���A�j���[�V��������
                pAnimSeq:            [IN]  �A�j���[�V�����f�[�^
                pCellBank:           [IN]  �Z���f�[�^�o���N
                vramStateHandle:     [IN]  �Z��VRAM�]����ԃI�u�W�F�N�g�̃n���h��
                dstAddr3D:           [IN]  �]����f�[�^(3D �p)
                dstAddr2DMain:       [IN]  �]����f�[�^(2D Main�p)
                dstAddr2DSub:        [IN]  �]����f�[�^(2D Sub �p)
                pSrcNCGR:            [IN]  �]�����f�[�^(NCGR)
                pSrcNCBR:            [IN]  �]�����f�[�^(NCBR)
                szSrcData:           [IN]  �]�����f�[�^�T�C�Y
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitCellAnimationVramTransfered
(
    NNSG2dCellAnimation*        pCellAnim, 
    const NNSG2dAnimSequence*   pAnimSeq, 
    const NNSG2dCellDataBank*   pCellBank,
    u32                         vramStateHandle,
    u32                         dstAddr3D,
    u32                         dstAddr2DMain,
    u32                         dstAddr2DSub,
    const void*                pSrcNCGR,
    const void*                pSrcNCBR,
    u32                         szSrcData
     
)
{
    //
    // VRAM�]�����������Ă��邱��
    //
    NNS_G2D_ASSERT( NNS_G2dCellDataBankHasVramTransferData( pCellBank ) );
    
    //
    // Vram�]���֘A�̏�����
    //
    {                                                
        const NNSG2dVramTransferData*    pVramData = 
            (const NNSG2dVramTransferData*)pCellBank->pVramTransferData;
        
        // �ݒ胏�[�N�̏�����
        NNSi_G2dInitCellTransferState( vramStateHandle,
                                        dstAddr3D,              // dst �]����Vram�A�h���X�̐ݒ�
                                        dstAddr2DMain,          // dst �]����Vram�A�h���X�̐ݒ�
                                        dstAddr2DSub,           // dst �]����Vram�A�h���X�̐ݒ�
                                        pVramData->szByteMax,   // dst �]������ő�f�[�^�T�C�Y
                                        pSrcNCGR,               // src 2D char-data
                                        pSrcNCBR,               // src 3D Tex-data
                                        szSrcData );            // src �T�C�Y 
           
        NNSi_G2dSetCellAnimVramTransferHandle( pCellAnim, vramStateHandle );
    }
    
    //
    // Cell �A�j���[�V�����̏�����
    //
    InitCellAnimationImpl_( pCellAnim, pAnimSeq, pCellBank, vramStateHandle );        
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetCellAnimationSequence

  Description:  �Z���A�j���[�V�����ɃA�j���[�V�����V�[�P���X��ݒ肵�܂��B

  Arguments:    pCellAnim   :      [OUT]     �Z���A�j���[�V��������
                pAnimSeq    �F     [IN]      �A�j���[�V�����V�[�P���X
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetCellAnimationSequence
( 
    NNSG2dCellAnimation*        pCellAnim, 
    const NNSG2dAnimSequence*   pAnimSeq 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pAnimSeq );
    NNS_G2D_ASSERTMSG( NNS_G2dGetAnimSequenceAnimType( pAnimSeq ) == NNS_G2D_ANIMATIONTYPE_CELL, 
                       "A cell-Animation's Data is expected");
    
    NNS_G2dBindAnimCtrl( &pCellAnim->animCtrl, pAnimSeq );
    ApplyCurrentAnimResult_( pCellAnim );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetCellAnimationSequenceNoReset

  Description:  �Z���A�j���[�V�����ɃA�j���[�V�����V�[�P���X��ݒ肵�܂��B
                �����̍Đ��A�j���[�V�����t���[���ԍ��A���݃t���[���\�����Ԃ����Z�b�g���܂���B

  Arguments:    pCellAnim   :    [OUT]      �Z���A�j���[�V��������
                pAnimSeq    �F   [IN]       �A�j���[�V�����V�[�P���X
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetCellAnimationSequenceNoReset
( 
    NNSG2dCellAnimation*        pCellAnim, 
    const NNSG2dAnimSequence*   pAnimSeq 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pAnimSeq );
    NNS_G2D_ASSERTMSG( NNS_G2dGetAnimSequenceAnimType( pAnimSeq ) == NNS_G2D_ANIMATIONTYPE_CELL,
                       "A cell-Animation's Data is expected");
    
    {
        const u16 frameIdx = NNS_G2dGetAnimCtrlCurrentFrame( &pCellAnim->animCtrl );
        
        // �J�E���^�[�̃��Z�b�g�����Ȃ�
        pCellAnim->animCtrl.pAnimSequence = pAnimSeq;
        
        if( !NNS_G2dSetAnimCtrlCurrentFrameNoResetCurrentTime( &pCellAnim->animCtrl, frameIdx ) )
        {
            NNS_G2dResetAnimationState( &pCellAnim->animCtrl );
        }
    }
    
    ApplyCurrentAnimResult_( pCellAnim );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTickCellAnimation

  Description:  NNSG2dCellAnimation �� ���Ԃ�i�߂܂�
                
  Arguments:    pCellAnim:          [OUT] �Z���A�j���[�V��������
                frames:             [IN]  �i�߂鎞�ԁi�t���[���j
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dTickCellAnimation( NNSG2dCellAnimation* pCellAnim, fx32 frames )
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pCellAnim->animCtrl.pAnimSequence );
    NNS_G2D_ASSERTMSG( NNS_G2dGetAnimSequenceAnimType( pCellAnim->animCtrl.pAnimSequence ) 
        == NNS_G2D_ANIMATIONTYPE_CELL, "A cell-Animation's Data is expected");
    
    if( NNS_G2dTickAnimCtrl( &pCellAnim->animCtrl, frames ) )
    {
        //
        // Frame�X�V���N�����Ȃ�A�A�j���[�V�������ʂ𔽉f����
        //
        ApplyCurrentAnimResult_( pCellAnim );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetCellAnimationCurrentFrame

  Description:  �Z���A�j���[�V�����̃A�j���[�V�����t���[����ݒ肵�܂��B
                �s���ȃt���[���ԍ����w�肳�ꂽ�ꍇ�́A�������܂���B
                
  Arguments:    pCellAnim   :           [OUT] �Z���A�j���[�V��������
                frameIndex  �F          [IN]  �A�j���[�V�����t���[���ԍ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetCellAnimationCurrentFrame
( 
    NNSG2dCellAnimation*    pCellAnim, 
    u16                     frameIndex 
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pCellAnim->animCtrl.pAnimSequence );
    
    if( NNS_G2dSetAnimCtrlCurrentFrame( &pCellAnim->animCtrl, frameIndex ) )
    {
        //
        // Frame�X�V���N�����Ȃ�A�A�j���[�V�������ʂ𔽉f����
        //
        ApplyCurrentAnimResult_( pCellAnim );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRestartCellAnimation

  Description:  �Z���A�j���[�V�����̍Đ���擪����ăX�^�[�g���܂��B
                ���]�Đ����Ă���ꍇ�́A�V�[�P���X�I�[����Đ����ăX�^�[�g���܂��B
                
  Arguments:    pCellAnim   :           [OUT] �Z���A�j���[�V��������
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRestartCellAnimation
( 
    NNSG2dCellAnimation*        pCellAnim
)
{
    NNSG2dAnimController* pAnmCtrl = NULL;
    NNS_G2D_NULL_ASSERT( pCellAnim );   
    
    // �Đ��A�j���[�V�����t���[����擪�ɃZ�b�g���܂��B
    pAnmCtrl = NNS_G2dGetCellAnimationAnimCtrl( pCellAnim );
    NNS_G2dResetAnimCtrlState( pAnmCtrl );
    
    // �Đ����X�^�[�g���܂��B
    NNS_G2dStartAnimCtrl( pAnmCtrl );
    
    // �A�j���[�V�������ʂ𔽉f���܂��B
    ApplyCurrentAnimResult_( pCellAnim );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetCellAnimationSpeed

  Description:  �Z���A�j���[�V�����̍Đ��X�s�[�h��ݒ肵�܂��B
                
  Arguments:    pCellAnim   :           [OUT] �Z���A�j���[�V��������
                speed       �F          [IN]  �Đ��X�s�[�h
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetCellAnimationSpeed
(
    NNSG2dCellAnimation*     pCellAnim,
    fx32                     speed  
)
{
    NNS_G2D_NULL_ASSERT( pCellAnim );
    NNS_G2D_NULL_ASSERT( pCellAnim->animCtrl.pAnimSequence );
    
    NNS_G2dSetAnimCtrlSpeed( &pCellAnim->animCtrl, speed );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dMakeCellToOams

  Description:  Cell��Oam��ɕϊ����܂�
                
  Arguments:    pDstOams:           [OUT] Oam���i�[�����o�b�t�@
                numDstOam:          [IN] �o�b�t�@��
                pCell:              [IN] �ϊ�����Cell
                pMtxSR:             [IN] Cell�ɐݒ肷��ϊ�     �i�C�Ӂj
                pBaseTrans:         [IN] Cell�ɐݒ肷�镽�s�ړ� �i�C�Ӂj
                bDoubleAffine:      [IN] DoubleAffine ���[�h���ǂ���
                
  Returns:      ���ۂɕϊ�����OamAttribute��
  
 *---------------------------------------------------------------------------*/
u16 NNS_G2dMakeCellToOams
( 
    GXOamAttr*              pDstOams,
    u16                     numDstOam, 
    const NNSG2dCellData*   pCell, 
    const MtxFx22*          pMtxSR, 
    const NNSG2dFVec2*      pBaseTrans,
    u16                     affineIndex,
    BOOL                    bDoubleAffine 
)
{
    u16             i = 0;
    NNSG2dFVec2     objTrans;
    GXOamAttr*      pDstOam = NULL;
    const u16       numOBJ 
        = ( numDstOam < pCell->numOAMAttrs ) ? numDstOam : pCell->numOAMAttrs;
    
    
    for( i = 0; i < numOBJ; i++ )
    {
        pDstOam = &pDstOams[i];
        
        NNS_G2dCopyCellAsOamAttr( pCell, i, pDstOam );
        
        //
        // OBJ�̈ʒu��ύX����K�v������Ȃ�...
        //
        if( pMtxSR != NULL || pBaseTrans != NULL )
        {
            //
            // �ʒu�̎擾
            //
            NNS_G2dGetOamTransFx32( pDstOam, &objTrans );
                    
            //
            // affine �ϊ����w�肳��Ă���Ȃ�...
            // 
            if( pMtxSR != NULL )
            {   
                //
                // �{�p�A�t�B�����ݒ肳��Ă���OBJ�Ȃ�A
                // NITRO-CHARACTER ���t�����Ă���␳�l����U�A��苎��܂�
                //
                NNSi_G2dRemovePositionAdjustmentFromDoubleAffineOBJ( pDstOam, 
                                                                     &objTrans );               
                {
                    // �㏑��
                    const GXOamEffect effectTypeAfter = ( bDoubleAffine ) ? 
                                             GX_OAM_EFFECT_AFFINE_DOUBLE : GX_OAM_EFFECT_AFFINE;                
                    const BOOL bShouldAdjust = ( effectTypeAfter  == GX_OAM_EFFECT_AFFINE_DOUBLE );
        
                    MulMtx22( pMtxSR, &objTrans, &objTrans );
                    
                    // affine Index �̐ݒ�
                    G2_SetOBJEffect( pDstOam, effectTypeAfter, affineIndex );
                    
                    NNSi_G2dAdjustDifferenceOfRotateOrientation( pDstOam, 
                                                                 pMtxSR, 
                                                                 &objTrans, 
                                                                 bShouldAdjust );
                }
            }
            //
            // ���s�ړ��������w�肳��Ă���Ȃ�...
            // 
            if( pBaseTrans != NULL )
            {
                objTrans.x += pBaseTrans->x;
                objTrans.y += pBaseTrans->y;
            }
            
            //
            // �����߂�
            //
            // 0x800 => �l�̌ܓ����Ӑ}���Ă��܂�
            G2_SetOBJPosition( pDstOam, 
                               ( objTrans.x + 0x800 )>> FX32_SHIFT, 
                               ( objTrans.y + 0x800 )>> FX32_SHIFT );
        }
    }
    return numOBJ;
}

