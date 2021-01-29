/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_Renderer.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.61 $
 *---------------------------------------------------------------------------*/
#include <nnsys/g2d/g2d_Renderer.h>

#include "g2d_Internal.h"
#include "g2di_RendererMtxStack.hpp"    // For MatrixStack 
#include "g2di_RendererMtxState.h"      // For MatrixCache & State


#include <nnsys/g2d/fmt/g2d_Oam_data.h>
#include <nnsys/g2d/g2d_OamSoftwareSpriteDraw.h> // Auto Z offset 
#include <nnsys/g2d/g2d_SRTControl.h>








//------------------------------------------------------------------------------
#define NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pRnd ) \
    NNS_G2D_ASSERTMSG( (pRnd) != NULL, "Please call this method between Begin - End Rendering" ) \


//------------------------------------------------------------------------------
// ���݂̃����_������
// Begin-End Rendering() �Ăяo�����ɐݒ肳��܂�
static NNSG2dRendererInstance*      pCurrentInstance_   = NULL; 


//------------------------------------------------------------------------------
// �ǉ��ϐ��FVram�]���A�j���̂��߂ɒǉ�
static u32      currenVramTransferHandle_ = NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE;




typedef struct MCRenderState
{
    u16                         currentCellAnimIdx;
    u16                         pad16_;
    NNSG2dRndCore2DMtxCache*    cellAnimMtxCacheTbl[256];
    BOOL                        bDrawMC;

}MCRenderState;

static MCRenderState        mcRenderState_;







//------------------------------------------------------------------------------
// NNSG2dRenderSurface ���X�g���z�Q�Ƃ��Ă��Ȃ����������܂�
// NNSG2dRenderSurface �o�^�Ȃǂ��s�� ����������ASSERT�}�N�����ŌĂ΂�܂��B
// 
static BOOL IsNotCircularLinked_
( 
    const NNSG2dRenderSurface*      pList, 
    const NNSG2dRenderSurface*      pNew 
)
{
    const NNSG2dRenderSurface*  pCursor = pList;
    
    while( pCursor != NULL )
    {
        // �����|�C���^�𔭌�����
        if( pCursor == pNew ) 
        {
            // NG 
            return FALSE;
        }    
        pCursor = (const NNSG2dRenderSurface*)pCursor->pNextSurface;
    }
    
    // �z�Q�Ƃ��������Ă��Ȃ�
    return TRUE;
}




//------------------------------------------------------------------------------
// OBJ �� OAMAttiburte ���� �p���b�g�ԍ����������� �������܂�
// DrawCellToSurface2D_() 
// DrawCellToSurface3D_()
// ���Ŏ��ۂ� �`�施�߂����s����钼�O�Ɏ��s����܂�
// 
static NNS_G2D_INLINE void OBJPaletteChangeHandling_( GXOamAttr* pOam )
{
    const NNSG2dPaletteSwapTable* pTbl 
        = NNS_G2dGetRendererPaletteTbl( pCurrentInstance_ );
    
    NNS_G2D_NULL_ASSERT( pOam );
    
    if( pTbl != NULL )
    {
        // �p���b�g�ԍ������������܂�
        const u16 newIdx 
           = NNS_G2dGetPaletteTableValue( pTbl, NNSi_G2dGetOamColorParam( pOam ) );
        pOam->cParam = newIdx;
    }
}

//------------------------------------------------------------------------------
// �����_���̃t���b�v�󋵂𔻒肵�āA���s�ړ���ݒ肵�܂�
static NNS_G2D_INLINE void FlipTranslate_( int x, int y )
{
    const int x_ = NNS_G2dIsRndCoreFlipH( &pCurrentInstance_->rendererCore ) ? -x : x;    
    const int y_ = NNS_G2dIsRndCoreFlipV( &pCurrentInstance_->rendererCore ) ? -y : y;
                            
    NNS_G2dTranslate( x_ << FX32_SHIFT , y_ << FX32_SHIFT, 0 );
}

//------------------------------------------------------------------------------
// NNSG2dSRTControl �� ���� affine�ϊ������J�����g�s��ɏ�Z���܂�
// NNSG2dSRTControl �� ��ނ� NNS_G2D_SRTCONTROLTYPE_SRT �ł���K�v������܂��B
//
//
static NNS_G2D_INLINE void SetSrtControlToMtxStack_( const NNSG2dSRTControl* pSrtCtrl )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pSrtCtrl );
    NNS_G2D_ASSERTMSG( pSrtCtrl->type == NNS_G2D_SRTCONTROLTYPE_SRT, "TODO: show msg " );
    
    // T
    if( NNSi_G2dSrtcIsAffineEnable( pSrtCtrl, NNS_G2D_AFFINEENABLE_TRANS ) )
    {
        //
        // �t���b�v�̌��ʂ𔽉f���āA���s�ړ��p�����[�^���X�V���܂��B
        //
        FlipTranslate_( pSrtCtrl->srtData.trans.x, pSrtCtrl->srtData.trans.y );                 
    }
    
    // R 
    if( NNSi_G2dSrtcIsAffineEnable( pSrtCtrl, NNS_G2D_AFFINEENABLE_ROTATE ) )
    {
        NNS_G2dRotZ( FX_SinIdx( pSrtCtrl->srtData.rotZ ), FX_CosIdx( pSrtCtrl->srtData.rotZ ) );
    }
    
    // S
    if( NNSi_G2dSrtcIsAffineEnable( pSrtCtrl, NNS_G2D_AFFINEENABLE_SCALE ) )
    {
        NNS_G2dScale( pSrtCtrl->srtData.scale.x, pSrtCtrl->srtData.scale.y, FX32_ONE );
    }

}


//------------------------------------------------------------------------------
// Vram �]���𗘗p����Cell�̕`��̑O��ŃR�[�����܂�
static NNS_G2D_INLINE void BeginDrawVramTransferedCell_( u32 cellVramTransferHandle )
{
    NNS_G2D_ASSERT( currenVramTransferHandle_ == NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
    NNS_G2D_ASSERT( cellVramTransferHandle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
    
    currenVramTransferHandle_ = cellVramTransferHandle;
}

//------------------------------------------------------------------------------
// Vram �]���𗘗p����Cell�̕`��̑O��ŃR�[�����܂�
static NNS_G2D_INLINE void EndDrawVramTransferedCell_( )
{
    NNS_G2D_ASSERT( currenVramTransferHandle_ != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );    
    currenVramTransferHandle_ = NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE;
}

//------------------------------------------------------------------------------
// �����_����Vram�]���A�j��Cell��`�撆���H
static NNS_G2D_INLINE BOOL IsRendererDrawingVramTransferedCell_( )
{
    return (BOOL)(currenVramTransferHandle_ != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE);
}

//------------------------------------------------------------------------------
// ���݂�ram�]���A�j��Cell�̃n���h�����擾���܂��B
static NNS_G2D_INLINE u32 GetCurrentVramTransfereHandle_()
{
    return currenVramTransferHandle_;
}



//------------------------------------------------------------------------------
// �����_���R�A���W���[���ɃR�[���o�b�N��o�^���A������J�X�^�}�C�Y���܂��B
// �����_���R�A���W���[���̓J�X�^�}�C�Y�ɂ���āA�]���̃����_�����W���[���Ɠ����̓�����������܂��B
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// �Z���`��O�R�[���o�b�N
// ���J�����O����
// ��2D�}�g���N�X�L���b�V���ւ̃J�����g�s��̃��[�h
// �������_�����W���[���̃Z���`��O�R�[���o�b�N�̌Ăяo��
//
static void RndCoreCBFuncBeforeCell_
(
    struct NNSG2dRndCoreInstance*   pRend,
    const NNSG2dCellData*           pCell
)    
{   
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCurrentInstance_->pCurrentSurface );
    {
       NNSG2dRenderSurface* pCurrentSurface = pCurrentInstance_->pCurrentSurface;
        
       //
       // �J�����O����
       //
       if( pCurrentSurface->pFuncVisibilityCulling != NULL )
       { 
            
           if( !(*pCurrentSurface->pFuncVisibilityCulling)( pCell, 
                                                     NNSi_G2dGetCurrentMtx() , 
                                                     &pCurrentSurface->coreSurface.viewRect ) )
           {
              // 
              // pRend �� �`����X�L�b�v����悤�ɐݒ肷��
              // 
              pRend->bDrawEnable = FALSE;
              return;
           }else{
              pRend->bDrawEnable = TRUE;
           }
       }
       
       //
       // �]���̃R�[���o�b�N�̌Ăяo��
       //
       if( *pCurrentSurface->pBeforeDrawCellBackFunc )
       {   
           (*pCurrentSurface->pBeforeDrawCellBackFunc)( pCurrentInstance_,
                                                 pCurrentSurface,
                                                 pCell,
                                                 NNSi_G2dGetCurrentMtx() );
       }
    }
}
//------------------------------------------------------------------------------
// �Z���`���R�[���o�b�N
//
// �����_�R�A���W���[���ɃR�[���o�b�N�֐��Ƃ��ēo�^����܂��B
static void RndCoreCBFuncAfterCell_
(
    struct NNSG2dRndCoreInstance*   pRend,
    const NNSG2dCellData*           pCell
)    
{
#pragma unused( pRend )
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCurrentInstance_->pCurrentSurface );
    {
       NNSG2dRenderSurface* pCurrentSurface = pCurrentInstance_->pCurrentSurface;
    
       if( *pCurrentSurface->pAfterDrawCellBackFunc )
       {
           (*pCurrentSurface->pAfterDrawCellBackFunc)( pCurrentInstance_,
                                                 pCurrentSurface,
                                                 pCell,
                                                 NNSi_G2dGetCurrentMtx() );
       }
    }
}
//------------------------------------------------------------------------------
// OBJ�`��O�R�[���o�b�N
//
// �����_�R�A���W���[���ɃR�[���o�b�N�֐��Ƃ��ēo�^����܂��B
//
// Oam�p�����[�^�̏㏑���������s���܂��B
// 
// ���݂̎����ł́ApBeforeDrawOamBackFunc �Ăяo���̑O�� �p�����[�^�����������s���Ă���̂�
// pBeforeDrawOamBackFunc ����OBJ�P�ʃJ�����O�����{����ꍇ�ȂǂɁA�`�悳��Ȃ�OBJ
// �ɑ΂���p�����[�^�㏑���������������Ă��܂��\��������܂��B
//
// OBJ�P�ʃJ�����O�����{�����OBJ�͏����̂͂��ŁA���̏ꍇ�̏����������d�v�����Ȃ��Ƃ������f�̂��Ƃ�
// ���݂̎����ɂȂ��Ă��܂��B
//
// 
static void RndCoreCBFuncBeforeOBJ_
(
    struct NNSG2dRndCoreInstance*   pRend,
    const NNSG2dCellData*           pCell,
    u16                             oamIdx
)
{
    GXOamAttr*    pTempOam = &pRend->currentOam;
    
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCurrentInstance_->pCurrentSurface );
    
    //
    // �p�����[�^��������
    //
    // �p���b�g�ϊ��e�[�u��
    OBJPaletteChangeHandling_( pTempOam );
    
    if( pCurrentInstance_->overwriteEnableFlag != NNS_G2D_RND_OVERWRITE_NONE )
    {
       // �`��D��    
       if( NNS_G2dIsRendererOverwriteEnable( pCurrentInstance_, NNS_G2D_RND_OVERWRITE_PRIORITY ) )
       {
           pTempOam->priority = pCurrentInstance_->overwritePriority;
       }
        
       // �p���b�g�ԍ�
       // ���ӁF������̕����D�悳��܂��B�i�p���b�g�ϊ��e�[�u���̌��ʂ��㏑�����܂��j
       if( NNS_G2dIsRendererOverwriteEnable( pCurrentInstance_, NNS_G2D_RND_OVERWRITE_PLTTNO ) )
       {
           pTempOam->cParam = pCurrentInstance_->overwritePlttNo;
       }
       
       // �p���b�g�ԍ�(�I�t�Z�b�g�����Z����)
       if( NNS_G2dIsRendererOverwriteEnable( pCurrentInstance_, NNS_G2D_RND_OVERWRITE_PLTTNO_OFFS ) )
       {
           pTempOam->cParam = 0xF & ( pTempOam->cParam + pCurrentInstance_->overwritePlttNoOffset );
       }
       
       // ���U�C�N
       if( NNS_G2dIsRendererOverwriteEnable( pCurrentInstance_, NNS_G2D_RND_OVERWRITE_MOSAIC ) )
       {
           G2_OBJMosaic( pTempOam, pCurrentInstance_->overwriteMosaicFlag );
       }
        
       // OBJ���[�h
       if( NNS_G2dIsRendererOverwriteEnable( pCurrentInstance_, NNS_G2D_RND_OVERWRITE_OBJMODE ) )
       {
           G2_SetOBJMode( pTempOam, pCurrentInstance_->overwriteObjMode, G2_GetOBJColorParam(pTempOam));
       }
    }
    
    //
    // �R�[���o�b�N�Ăяo��
    //
    {
       NNSG2dRenderSurface* pCurrentSurface = pCurrentInstance_->pCurrentSurface;
       if( *pCurrentSurface->pBeforeDrawOamBackFunc )
       {
           (*pCurrentSurface->pBeforeDrawOamBackFunc)( pCurrentInstance_,
                                                 pCurrentSurface,
                                                 pCell,
                                                 oamIdx,
                                                 NNSi_G2dGetCurrentMtx() );
       }
    }
}
//------------------------------------------------------------------------------
// OBJ�`���R�[���o�b�N
//
// �����_�R�A���W���[���ɃR�[���o�b�N�֐��Ƃ��ēo�^����܂��B
static void RndCoreCBFuncAfterOBJ_
(
    struct NNSG2dRndCoreInstance*   pRend,
    const NNSG2dCellData*           pCell,
    u16                             oamIdx
)
{
#pragma unused( pRend )
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCurrentInstance_->pCurrentSurface );
    {
       NNSG2dRenderSurface* pCurrentSurface = pCurrentInstance_->pCurrentSurface;
       if( *pCurrentSurface->pAfterDrawOamBackFunc )
       {
           (*pCurrentSurface->pAfterDrawOamBackFunc)( pCurrentInstance_,
                                                 pCurrentSurface,
                                                 pCell,
                                                 oamIdx,
                                                 NNSi_G2dGetCurrentMtx() );
       }
    }
}



//------------------------------------------------------------------------------
// �����_�R�A 2D �T�[�t�F�X�ɑ΂��郌���_�����O���J�n���܂��B
static NNS_G2D_INLINE void BeginRndCoreRendering2D_
(
    NNSG2dRendererInstance*  pRnd,
    NNSG2dRenderSurface*     pSurface 
)
{
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_ASSERT( pRnd->pCurrentSurface == NULL ); 
    NNS_G2D_ASSERT( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D );
    NNS_G2D_NULL_ASSERT( pSurface );
    
    //
    // �����_�R�A�ɁA�T�[�t�F�X�ݒ���s��
    //
    pRnd->pCurrentSurface = pSurface;
    NNS_G2dSetRndCoreSurface( &pRnd->rendererCore, &pSurface->coreSurface );
       
    //
    // 2D �`��p�ɓo�^�֐��̐ݒ�
    //
    {
        {
            NNS_G2dSetRndCoreOamRegisterFunc( &pRnd->rendererCore,
                                              pSurface->pFuncOamRegister,
                                              pSurface->pFuncOamAffineRegister );
        }
    }
    
    NNS_G2dRndCoreBeginRendering( &pRnd->rendererCore );
}

//------------------------------------------------------------------------------
// �����_�R�A 3D �T�[�t�F�X�ɑ΂��郌���_�����O���J�n���܂��B
static NNS_G2D_INLINE void BeginRndCoreRendering3D_
(
    NNSG2dRendererInstance*  pRnd, 
    NNSG2dRenderSurface*     pSurface 
)
{
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_ASSERT( pRnd->pCurrentSurface == NULL ); 
    NNS_G2D_ASSERT( pSurface->type == NNS_G2D_SURFACETYPE_MAIN3D );
    NNS_G2D_NULL_ASSERT( pSurface );
    
    //
    // �����_�R�A�ɁA�T�[�t�F�X�ݒ���s��
    //
    pRnd->pCurrentSurface = pSurface;
    NNS_G2dSetRndCoreSurface( &pRnd->rendererCore, &pSurface->coreSurface );
       
    
    NNS_G2dRndCoreBeginRendering( &pRnd->rendererCore );
}

//------------------------------------------------------------------------------
// �����_�R�A�̕`����I�����܂��B
static NNS_G2D_INLINE void EndRndCoreRendering_( void )
{
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCurrentInstance_->pCurrentSurface );
    
    pCurrentInstance_->pCurrentSurface = NULL;
    NNS_G2dRndCoreEndRendering( );
}


//------------------------------------------------------------------------------
// �����_�R�A���W���[�� 2D�`��
// DrawCellImpl_ �֐��̉ǐ�����̂��߂ɕʊ֐��Ƃ��Ă��܂��B
static NNS_G2D_INLINE void DoRenderByRndCore2D_( 
    const NNSG2dCellData*    pCell ,
    NNSG2dRndCoreInstance*   pRndCore
)
{
#pragma unused( pRndCore )
    NNS_G2D_NULL_ASSERT( pCell );
    NNS_G2D_NULL_ASSERT( pRndCore );
    //
    // �����_���Ƀ}�g���N�X�L���b�V����ݒ肵�܂��B
    // (���O��Z�l��ݒ肵�Ă����K�v������܂��B)
    //
    {
       NNSG2dRndCore2DMtxCache* pMtx2D = NULL;

       //
       // �A�t�B���ϊ����Ă���΁A2D�A�t�B���ϊ��p��MtxCache��ݒ肷��B
       //
       if( NNSi_G2dIsRndCurrentMtxSRTransformed() )
       {
           //
           // �}���`�Z���̕`�撆�Ȃ��...
           //
           if( mcRenderState_.bDrawMC )
           {
               //
               // �ȑO�ɓǂݍ��܂�Ă��Ȃ����e�[�u�����m�F����
               // �}���`�Z�����̓���Z���A�j���[�V�����͕K�������A�t�B���p�����[�^���Q�Ƃ���͂��B
               // �����_���͈�x�A�t�B���ϊ����g�p���ĕ`�悵���Z���A�j���[�V�����̍s��L���b�V�����e�[�u���ɋL�����Ă���B
               //
               pMtx2D 
                  = mcRenderState_.cellAnimMtxCacheTbl[mcRenderState_.currentCellAnimIdx];
               //
               // �ȑO�ɕ`�悳�ꂽ���Ƃ̂Ȃ��Z���A�j���[�V�����Ȃ��...
               //
               if( pMtx2D == NULL )
               {
                  //
                  // �J�����g�s����}�g���N�X�L���b�V���Ƀ��[�h���܂��B
                  //
                  NNSi_G2dMCMStoreCurrentMtxToMtxCache();
                  //
                  // ���[�h�����������A�}�g���N�X�L���b�V�����擾���܂�
                  //
                  pMtx2D = NNSi_G2dMCMGetCurrentMtxCache();
                  //
                  // �s��L���b�V�����e�[�u���ɋL�����܂�
                  //
                  mcRenderState_.cellAnimMtxCacheTbl[mcRenderState_.currentCellAnimIdx] = pMtx2D;
              }
           }else{
               //
               // �J�����g�s����}�g���N�X�L���b�V���Ƀ��[�h���܂��B
               //
               NNSi_G2dMCMStoreCurrentMtxToMtxCache();
               //
               // ���[�h�����������A�}�g���N�X�L���b�V�����擾���܂�
               //
               pMtx2D = NNSi_G2dMCMGetCurrentMtxCache();
           }
       }                                                   
       //
       // �A�t�B���ϊ��p�̍s��L���b�V����ݒ�
       //
       NNS_G2dSetRndCoreCurrentMtx2D( NNSi_G2dGetCurrentMtx(), pMtx2D );
    }
    
    
    //
    // �����_�R�A�ɕ`�揈�����Ϗ�����
    //
    if( IsRendererDrawingVramTransferedCell_( ) )
    {
        NNS_G2dRndCoreDrawCellVramTransfer( pCell, GetCurrentVramTransfereHandle_() );
    }else{
        NNS_G2dRndCoreDrawCell( pCell );
    }
}
              
//------------------------------------------------------------------------------
// �����_�R�A���W���[�� 3D�`��
// DrawCellImpl_ �֐��̉ǐ�����̂��߂ɕʊ֐��Ƃ��Ă��܂��B
static NNS_G2D_INLINE void DoRenderByRndCore3D_
( 
    const NNSG2dCellData*    pCell ,
    NNSG2dRndCoreInstance*   pRndCore
)
{
    NNS_G2D_NULL_ASSERT( pCell );
    NNS_G2D_NULL_ASSERT( pRndCore );
    
    //
    // 3D �p Z�l��ݒ肵�܂��B
    //
    NNS_G2dSetRndCore3DSoftSpriteZvalue( pRndCore, NNSi_G2dGetCurrentZ() );
    //
    // �����_���R�A�ɃJ�����g�s���ݒ肵�܂��B
    // (���O��Z�l��ݒ肵�Ă����K�v������܂��B)
    //
    NNS_G2dSetRndCoreCurrentMtx3D( NNSi_G2dGetCurrentMtx() );
    //
    // �����_�R�A�ɕ`�揈�����Ϗ�����
    //
    if( IsRendererDrawingVramTransferedCell_( ) )
    {
        NNS_G2dRndCoreDrawCellVramTransfer( pCell, GetCurrentVramTransfereHandle_() );
    }else{
        NNS_G2dRndCoreDrawCell( pCell );
    }
}

//------------------------------------------------------------------------------
// �Z����`�悵�܂��B
static void DrawCellImpl_( const NNSG2dCellData* pCell )
{
    NNSG2dRndCoreInstance*   pRndCore   = NULL;
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCell );

    pRndCore = &pCurrentInstance_->rendererCore;
    
    {
        NNSG2dRenderSurface*      pSurface = pCurrentInstance_->pTargetSurfaceList;
        //
        // �T�[�t�F�X���ꖇ�������p���Ȃ��ꍇ�́A�Z���`�悲�Ƃɍs����A�����_�R�A�ւ̃T�[�t�F�X
        // �p�����[�^�̐ݒ������\�ł��B
        //
        if( pCurrentInstance_->opzHint & NNS_G2D_RDR_OPZHINT_LOCK_PARAMS )
        {
           // 
           // �`��֐����Ƃ� BeginRndCoreRenderingXX_() EndRndCoreRendering_()
           // �Ăяo�����s���܂���B���̕������ɓ��삵�܂��B
           // 
           //
           // 2D �T�[�t�F�X�ɑ΂���`��Ȃ��...
           //       
           if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
           {
              //
              // ���ۂ̕`�揈��( 2D )
              //                    
              DoRenderByRndCore2D_( pCell, pRndCore );    
           }else{
              //
              // ���ۂ̕`�揈��( 3D )
              //                    
              DoRenderByRndCore3D_( pCell, pRndCore );
           }
        }else{
           // for each render surfaces...     
           while( pSurface )
           {
              if( pSurface->bActive )
              {                
                  //
                  // 2D �T�[�t�F�X�ɑ΂���`��Ȃ��...
                  //
                  if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
                  {
                     //
                     // ���ۂ̕`�揈��( 2D )
                     //                    
                     BeginRndCoreRendering2D_( pCurrentInstance_, pSurface );
                         DoRenderByRndCore2D_( pCell, pRndCore );
                     EndRndCoreRendering_();
                  }else{
                     //
                     // ���ۂ̕`�揈��( 3D )
                     //                    
                     BeginRndCoreRendering3D_( pCurrentInstance_, pSurface );
                         DoRenderByRndCore3D_( pCell, pRndCore );
                     EndRndCoreRendering_();
                  }
              }
              // Next surface ... 
              pSurface = pSurface->pNextSurface;
           }
        }
    }
}



//------------------------------------------------------------------------------
static void DrawCellAnimationImpl_( const NNSG2dCellAnimation* pCellAnim )
{
    NNSG2dCellData*      pCell = NULL;
    NNS_G2D_NULL_ASSERT( pCellAnim );
    
    pCell = (NNSG2dCellData*)NNS_G2dGetCellAnimationCurrentCell( pCellAnim );
    NNS_G2D_NULL_ASSERT( pCell );
           
    //
    // SRT�A�j���[�V�������g�p����Ă��Ȃ��ꍇ�́A���p�� Push Pop �� ������܂��B
    //
    if( pCellAnim->srtCtrl.srtData.SRT_EnableFlag == NNS_G2D_AFFINEENABLE_NONE )
    {
           //
           // VRAM �]���A�j���𗘗p����Cell �Ȃ��...
           //
           if( NNSi_G2dIsCellAnimVramTransferHandleValid( pCellAnim ) )
           {
                
              BeginDrawVramTransferedCell_( NNSi_G2dGetCellAnimVramTransferHandle( pCellAnim ) );
                  DrawCellImpl_( pCell );
              EndDrawVramTransferedCell_();
                
           }else{
                  DrawCellImpl_( pCell );
           }
    }else{
       NNS_G2dPushMtx(); 
           SetSrtControlToMtxStack_( &pCellAnim->srtCtrl );        
           //
           // VRAM �]���A�j���𗘗p����Cell �Ȃ��...
           //
           if( NNSi_G2dIsCellAnimVramTransferHandleValid( pCellAnim ) )
           {
                
              BeginDrawVramTransferedCell_( NNSi_G2dGetCellAnimVramTransferHandle( pCellAnim ) );
                  DrawCellImpl_( pCell );
              EndDrawVramTransferedCell_();
                
           }else{
                  DrawCellImpl_( pCell );
           }     
       NNS_G2dPopMtx(1);
    }
}


//------------------------------------------------------------------------------
// Node �� �`������܂�
// �O�����J�֐���������֐��ɕύX�ɂȂ�܂����B
static void DrawNode_( const NNSG2dNode* pNode )
{
    NNS_G2D_NULL_ASSERT( pNode );
    NNS_G2D_ASSERTMSG( pNode->type == NNS_G2D_NODETYPE_CELL, "NNS_G2D_NODETYPE_CELL is expected." );
    
    if( pNode->bVisible )
    {
        //
        // TODO:
        // codes below this line should be "pNode->type" depended.
        // for now, we expect the "pNode->type" is always "NNS_G2D_NODETYPE_CELL"
        //        
        NNSG2dCellAnimation*    pCellAnim = (NNSG2dCellAnimation*)pNode->pContent;
        NNS_G2D_NULL_ASSERT( pCellAnim );
        
        //
        // pNode->srtCtrl.srtData.SRT_EnableFlag == NNS_G2D_AFFINEENABLE_NONE
        // �ƂȂ�ꍇ�͏��Ȃ��͂��Ȃ̂ŁA�����̂悤�ɏ�������ɂ����Push Pop ���������
        // ���������܂���B
        // 
        NNS_G2dPushMtx();
            SetSrtControlToMtxStack_( &pNode->srtCtrl );    
            {                       
                DrawCellAnimationImpl_( pCellAnim );
            }
        NNS_G2dPopMtx(1);
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void DrawNode2_
( 
    const NNSG2dMultiCellInstance*         pMC, 
    const NNSG2dMultiCellHierarchyData*    pNodeData 
)
{
    const u16 cellAnimIdx          = NNSi_G2dGetMC2NodeCellAinmIdx( pNodeData );
    const NNSG2dMCCellAnimation*   cellAnimArray = pMC->pCellAnimInstasnces;
    NNS_G2D_MINMAX_ASSERT( cellAnimIdx, 0, pMC->pCurrentMultiCell->numCellAnim );
    mcRenderState_.currentCellAnimIdx = cellAnimIdx;
    
    NNS_G2dPushMtx();
       FlipTranslate_( pNodeData->posX, pNodeData->posY );
       {                       
           DrawCellAnimationImpl_( &cellAnimArray[cellAnimIdx].cellAnim );
       }
    NNS_G2dPopMtx(1);
}

//------------------------------------------------------------------------------
// ����Z�l�I�t�Z�b�g���L�����H
static NNS_G2D_INLINE BOOL IsAutoZoffsetEnable_( void )
{
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    return (BOOL)pCurrentInstance_->spriteZoffsetStep;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitRenderer

  Description:  �����_�� ���̂����������܂�
                
                
  Arguments:    pRend:      [OUT] �����_�� ����
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitRenderer( NNSG2dRendererInstance* pRend )
{
    NNS_G2D_NULL_ASSERT( pRend );
    
    NNS_G2dInitRndCore( &pRend->rendererCore );
    
    pRend->pTargetSurfaceList   = NULL;
    pRend->pCurrentSurface      = NULL;
    pRend->pPaletteSwapTbl      = NULL;
    
    //
    // �œK���q���g�A�K��l�ł͂��ׂĖ����ƂȂ��Ă��܂�(�œK���͍s���Ȃ�)
    //
    pRend->opzHint = NNS_G2D_RDR_OPZHINT_NONE;
    
    pRend->spriteZoffsetStep = 0;
    
    // overwriteEnableFlag �� NNSG2dRendererOverwriteParam
    pRend->overwriteEnableFlag = NNS_G2D_RND_OVERWRITE_NONE; 
    pRend->overwritePriority   = 0;
    pRend->overwritePlttNo     = 0;  
    pRend->overwriteObjMode    = GX_OAM_MODE_NORMAL;
    pRend->overwriteMosaicFlag = FALSE;
    pRend->overwritePlttNoOffset = 0;
    
    //
    // �}�g���N�X�L���b�V���̏�����
    //
    NNSi_G2dMCMInitMtxCache();
    //
    // �}�g���N�X�X�^�b�N���W���[���̗��p���[�h��ݒ肷��B
    //
    NNSi_G2dSetRndMtxStackSRTransformEnableFlag( TRUE );
    
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dAddRendererTargetSurface

  Description:  �����_�� �� NNSG2dRenderSurface��ǉ����܂�
                
                
  Arguments:    pRend:      [OUT] �����_�� ����
                pNew:       [IN]  �ǉ����� NNSG2dRenderSurface
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dAddRendererTargetSurface( NNSG2dRendererInstance* pRend, NNSG2dRenderSurface* pNew )
{
    NNS_G2D_NULL_ASSERT( pRend );
    NNS_G2D_NULL_ASSERT( pNew );
    NNS_G2D_ASSERTMSG( IsNotCircularLinked_( pRend->pTargetSurfaceList, pNew ),
        "Circular linked lists is detected in NNS_G2dAddRendererTargetSurface()" );
    
    // add_front
    pNew->pNextSurface          = pRend->pTargetSurfaceList;
    pRend->pTargetSurfaceList   = pNew;
    
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitRenderSurface

  Description:  �����_�T�[�t�F�X �����������܂��B
                �����Ń����_���R�A���W���[���ɃR�[���o�b�N�֐���o�^���Ă��܂��B
                
                
  Arguments:    pSurface:      [OUT] �����_�T�[�t�F�X ����
                
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitRenderSurface( NNSG2dRenderSurface* pSurface )
{
    NNS_G2D_NULL_ASSERT( pSurface );
    
    MI_CpuFill16( pSurface, 0x0, sizeof( NNSG2dRenderSurface ) );
    
    pSurface->coreSurface.bActive = TRUE;
    
    pSurface->coreSurface.type = NNS_G2D_SURFACETYPE_MAX;
    
    //
    // �����_���R�A���W���[���ɃR�[���o�b�N�֐���o�^���܂�
    //
    {
        NNSG2dRndCoreSurface* pS = &pSurface->coreSurface;
        
        pS->pBeforeDrawCellBackFunc   = RndCoreCBFuncBeforeCell_;
        pS->pAfterDrawCellBackFunc    = RndCoreCBFuncAfterCell_;
        pS->pBeforeDrawOamBackFunc    = RndCoreCBFuncBeforeOBJ_;
        pS->pAfterDrawOamBackFunc     = RndCoreCBFuncAfterOBJ_;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBeginRendering

  Description:  �����_�� �`��O�̐ݒ���s���܂�
                �����_�� �`�惁�\�b�h���ĂԑO�ɌĂ�ł��������B
                Begin End Rendering �� �� �Ăяo���Ȃ��ł��������B
                
                ���ɁA�{�֐��Ɠ��l�̖������ʂ����A�`��œK���̂��߂̃q���g�t���O���w��\��
                NNS_G2dBeginRenderingEx()�֐����p�ӂ���Ă��܂��B
                
                
  Arguments:    pRendererInstance:      [IN]  �����_�� ����
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dBeginRendering( NNSG2dRendererInstance* pRendererInstance )
{
    NNS_G2D_NULL_ASSERT( pRendererInstance );
    NNS_G2D_ASSERTMSG( pCurrentInstance_ == NULL, 
        "Must be NULL, Make sure calling Begin - End correctly." );
    
    pCurrentInstance_ = pRendererInstance;
    
    NNSi_G2dMCMCleanupMtxCache();
    
    G3_PushMtx();            
    
    G3_Identity();
    NNSi_G2dIdentity();
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBeginRenderingEx

  Description:  �����_�� �`��O�̐ݒ���s���܂�
                �����_�� �`�惁�\�b�h���ĂԑO�ɌĂ�ł��������B
                Begin End Rendering �� �� �Ăяo���Ȃ��ł��������B
                �{�֐��́A�`��œK���̂��߂̃q���g�t���O���w��\�ł��B
                
                �q���g�� NNSG2dRendererOptimizeHint �񋓎q�̘_���a
                �ō쐬���܂��B
                
                �œK���q���g�t���O��NNS_G2dEndRendering()�ɂ���ă��Z�b�g����܂��B
                �܂�A�œK���q���g�t���O �� �����_���� Begin - End Rendering 
                �u���b�N���ł̂ݗL���ƂȂ�܂��B
                
                �{�֐��́A�œK���̂��߂̑O�������s�������ƁA
                �ʏ��NNS_G2dBeginRendering()���Ăяo���Ă��܂��B
                
                
  Arguments:    pRendererInstance       [IN]  �����_�� 
                opzHint                 [IN]  �œK���q���g
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dBeginRenderingEx 
( 
    NNSG2dRendererInstance* pRendererInstance, 
    u32                     opzHint 
)
{
    NNS_G2D_NULL_ASSERT( pRendererInstance );
    NNS_G2D_ASSERT( pRendererInstance->opzHint == NNS_G2D_RDR_OPZHINT_NONE );
    
    pRendererInstance->opzHint = opzHint;
    //
    // �`��œK���̂��߂̑O����
    //
    {
       if( opzHint & NNS_G2D_RDR_OPZHINT_NOT_SR )
       {
           NNSi_G2dSetRndMtxStackSRTransformEnableFlag( FALSE );
       }
       
       //
       // �T�[�t�F�X���ꖇ�������p���Ȃ��ꍇ�́A�Z���`�悲�Ƃɍs����A�����_�R�A�ւ̃T�[�t�F�X
       // �p�����[�^�̐ݒ�� Begin - End �̃^�C�~���O�ň��s���݂̂Ɍy���\�ł��B
       //
       if( opzHint & NNS_G2D_RDR_OPZHINT_LOCK_PARAMS )
       {
           NNSG2dRndCoreInstance*  pRndCore = &pRendererInstance->rendererCore;
           NNSG2dRenderSurface*    pSurface = pRendererInstance->pTargetSurfaceList;
            
           NNS_G2D_ASSERTMSG( pSurface->pNextSurface == NULL,
              "The number of target surface must be ONE. when you spesified the NNS_G2D_RDR_OPZHINT_LOCK_PARAMS flag." );  
              
           if( pSurface->bActive )
           {                
              //
              // 2D �T�[�t�F�X�ɑ΂���`��Ȃ��...
              //
              if( pSurface->type != NNS_G2D_SURFACETYPE_MAIN3D )
              {
                  BeginRndCoreRendering2D_( pRendererInstance, pSurface );
              }else{
                  BeginRndCoreRendering3D_( pRendererInstance, pSurface );
              }
           }
       }
    }

    
    NNS_G2dBeginRendering( pRendererInstance );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEndRendering

  Description:  �����_�� �`���̐ݒ���s���܂�
                �`���ɕύX���ꂽ������Ԃ����ɖ߂��܂��B
                
                �œK���q���g���w�肳��Ă���(NNS_G2dBeginRenderingEx()���g�p����)
                �ꍇ�ɂ́A�œK�������̌㏈�����s���܂��B
                
                �œK���q���g�t���O�͖{�֐��ɂ���ă��Z�b�g����܂��B
                �܂�A�œK���q���g�t���O �� �����_���� Begin - End Rendering 
                �u���b�N���ł̂ݗL���ƂȂ�܂��B
                
                
  Arguments:    �Ȃ�
  
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dEndRendering()
{
    // check push pop are called correctly.
    NNS_G2D_NULL_ASSERT( pCurrentInstance_ );
    
    
    G3_PopMtx(1);
    
    {
        //
        // �œK���t���O���ݒ肳��Ă���΁A���̏����������Ȃ�
        //
        const u32 opzHint = pCurrentInstance_->opzHint;
        if( opzHint != NNS_G2D_RDR_OPZHINT_NONE )
        {
            //
            // �X�^�b�N�� SR�ϊ��ݒ�����Ƃɖ߂��܂��B
            //
            if( opzHint & NNS_G2D_RDR_OPZHINT_NOT_SR )
            {
                NNSi_G2dSetRndMtxStackSRTransformEnableFlag( TRUE );
            }
            
            //
            // �T�[�t�F�X���ꖇ�������p���Ȃ��ꍇ�́A�Z���`�悲�Ƃɍs����A�����_�R�A�ւ̃T�[�t�F�X
            // �p�����[�^�̐ݒ�� �����_���� Begin - End Rendering�̃^�C�~���O�ň��s���݂̂Ɍy���\�ł��B
            //
            if( opzHint & NNS_G2D_RDR_OPZHINT_LOCK_PARAMS )
            {
                EndRndCoreRendering_();
            }
            //
            // �œK���t���O�����Z�b�g���܂��B
            // ( �܂�œK���t���O �� Begin�|End���ł̂ݗL���ł��B)
            //
            pCurrentInstance_->opzHint = NNS_G2D_RDR_OPZHINT_NONE;
        }    
    }
    
    pCurrentInstance_ = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawCell

  Description:  �Z�� �� �`������܂��B
                
                
  Arguments:    pCell           : [IN]  �`�悷�� �Z�� ����             
                                  
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawCell( const NNSG2dCellData* pCell )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCell );
    
    if( IsAutoZoffsetEnable_() )
    {
        const fx32 offset = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( TRUE );
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep( pCurrentInstance_->spriteZoffsetStep );
        
        DrawCellImpl_( pCell );
        
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( FALSE );
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep( offset );
        NNSi_G2dResetOamSoftEmuAutoZOffset();
    }else{
        DrawCellImpl_( pCell );
    }
}
   


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawCellAnimation

  Description:  �Z���A�j���[�V���� �� �`������܂��B
                
                
  Arguments:    pMC             : [IN]  �`�悷�� �Z���A�j���[�V����        
                                  
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawCellAnimation( const NNSG2dCellAnimation* pCellAnim )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pCellAnim );
    
    if( IsAutoZoffsetEnable_() )
    {
        const fx32 offset = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( TRUE );
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep( pCurrentInstance_->spriteZoffsetStep );
        
        DrawCellAnimationImpl_( pCellAnim );
        
        NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( FALSE );
        NNSi_G2dSetOamSoftEmuAutoZOffsetStep( offset );
        NNSi_G2dResetOamSoftEmuAutoZOffset();
    }else{
        DrawCellAnimationImpl_( pCellAnim );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawMultiCell

  Description:  �}���`�Z�� �� �`������܂��B
                
                
  Arguments:    pMC             : [IN]  �`�悷�� �}���`�Z�� ����             
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawMultiCell
( 
    const NNSG2dMultiCellInstance*      pMC 
)
{
    u16 i;
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pMC );
    if( pMC->mcType == NNS_G2D_MCTYPE_SHARE_CELLANIM )
    {
       //
       // �}���`�Z��2D�`��p�̍s��L���b�V���e�[�u�����g�p����Z���A�j�������������������܂��B
       //
       for( i = 0; i < pMC->pCurrentMultiCell->numCellAnim; i++ )
       {
           mcRenderState_.cellAnimMtxCacheTbl[i] = NULL;
       }
       mcRenderState_.bDrawMC = TRUE;
        
       if( IsAutoZoffsetEnable_() )
       {
           const fx32 offset = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
           NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( TRUE );
           NNSi_G2dSetOamSoftEmuAutoZOffsetStep( pCurrentInstance_->spriteZoffsetStep );
            
           for( i = 0; i < pMC->pCurrentMultiCell->numNodes; i++ )
           {
              DrawNode2_( pMC, &pMC->pCurrentMultiCell->pHierDataArray[i] );
           }
            
           NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( FALSE );
           NNSi_G2dSetOamSoftEmuAutoZOffsetStep( offset );
           NNSi_G2dResetOamSoftEmuAutoZOffset();
       }else{
           for( i = 0; i < pMC->pCurrentMultiCell->numNodes; i++ )
           {
              DrawNode2_( pMC, &pMC->pCurrentMultiCell->pHierDataArray[i] );
           }
       }
        
       mcRenderState_.bDrawMC = FALSE;
    
    }else{
       const NNSG2dNode* pNode = pMC->pCellAnimInstasnces;
       if( IsAutoZoffsetEnable_() )
       {
           const fx32 offset = NNSi_G2dGetOamSoftEmuAutoZOffsetStep();
           
           NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( TRUE );
           NNSi_G2dSetOamSoftEmuAutoZOffsetStep( pCurrentInstance_->spriteZoffsetStep );
            
           for( i = 0; i < pMC->pCurrentMultiCell->numNodes; i++ )
           {
              DrawNode_( &pNode[i] );
           }
            
           NNSi_G2dSetOamSoftEmuAutoZOffsetFlag( FALSE );
           NNSi_G2dSetOamSoftEmuAutoZOffsetStep( offset );
           NNSi_G2dResetOamSoftEmuAutoZOffset();
       }else{
           for( i = 0; i < pMC->pCurrentMultiCell->numNodes; i++ )
           {
              DrawNode_( &pNode[i] );
           }
       }
    }
}






/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawMultiCellAnimation

  Description:  �}���`�Z���A�j���[�V���� �� �`������܂��B
                NNS_G2dDrawMultiCell() ������ŌĂяo���Ă��܂��B
                
                �}���`�Z���A�j���[�V������SRT�A�j���[�V�������ʂ�
                �l������_���ANNS_G2dDrawMultiCell()�ƈقȂ�_�ł��B
                
                
  Arguments:    pMCAnim         : [IN]  �`�悷�� �}���`�Z���A�j���[�V����
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawMultiCellAnimation
( 
    const NNSG2dMultiCellAnimation*     pMCAnim 
)
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pMCAnim );
    
    if( pMCAnim->srtCtrl.srtData.SRT_EnableFlag == NNS_G2D_AFFINEENABLE_NONE )
    {
        NNS_G2dDrawMultiCell( &pMCAnim->multiCellInstance );    
    }else{
        NNS_G2dPushMtx();
            SetSrtControlToMtxStack_( &pMCAnim->srtCtrl );
            NNS_G2dDrawMultiCell( &pMCAnim->multiCellInstance );    
        NNS_G2dPopMtx(1);
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dDrawEntity

  Description:  Entity �� �`������܂��B
                �G���e�B�e�B�f�[�^���p���b�g�ϊ��e�[�u���������Ă����ꍇ�́A
                �p���b�g�ϊ��e�[�u�����g�p���ĕ`����s���܂��B
                
                
  Arguments:    pEntity          : [IN]  �`�悷�� Entity ����             
                               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dDrawEntity( NNSG2dEntity* pEntity )
{
    BOOL bAffined = FALSE;
    
    BOOL bPaletteChange                 = FALSE;
    const NNSG2dPaletteSwapTable* pTbl  = NULL;
    
    
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_ASSERT_ENTITY_VALID( pEntity );
    
    //
    // �p���b�g��ݒ肵�܂�
    //
    bPaletteChange = NNS_G2dIsEntityPaletteTblEnable( pEntity );
    if( bPaletteChange )
    {
        pTbl = NNS_G2dGetRendererPaletteTbl( pCurrentInstance_ );
        NNS_G2dSetRendererPaletteTbl( pCurrentInstance_, pEntity->pPaletteTbl );
    }
    
    //
    // Entity �� ��ނɉ����ĕ`������܂�
    //
    {       
        switch( pEntity->pEntityData->type )
        {
        case NNS_G2D_ENTITYTYPE_CELL:
            {
                NNSG2dCellAnimation*  
                  pCellAnim = (NNSG2dCellAnimation*)pEntity->pDrawStuff;
                NNS_G2D_NULL_ASSERT( pCellAnim );
                NNS_G2dDrawCellAnimation( pCellAnim );
            }
            break;
        case NNS_G2D_ENTITYTYPE_MULTICELL:
            {
                NNSG2dMultiCellAnimation*  
                  pMCAnim = (NNSG2dMultiCellAnimation*)pEntity->pDrawStuff;
                NNS_G2D_NULL_ASSERT( pMCAnim );
                NNS_G2dDrawMultiCellAnimation( pMCAnim );
            }
            break;
        default:
            NNS_G2D_ASSERTMSG( FALSE, "TODO: msg ");
        }    
    }
    
    //
    // �p���b�g�����ɂ��ǂ��܂�
    //
    if( bPaletteChange )
    {
        if( pTbl != NULL )
        {
            NNS_G2dSetRendererPaletteTbl( pCurrentInstance_, pTbl );
        }else{
            NNS_G2dResetRendererPaletteTbl( pCurrentInstance_ );
        }
    }
}




/*---------------------------------------------------------------------------*
  Name:         NNS_G2dPushMtx 

  Description:  �����_�������̍s��X�^�b�N�𑀍삵�܂��B
                �s��X�^�b�N��CPU�ɂ���ď�������܂��B
                
                2D 3D Graphics engine �� scale �p�����[�^�̉��߂��قȂ邽��
                ������ 2D �p 3D �p 2�̍s��X�^�b�N�������Ă��܂�(NNS_G2dScale)�B
                
                
  Arguments:    �Ȃ�
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dPushMtx()
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    //
    // �����_�����W���[���̏ꍇ�́A�p�t�H�[�}���X�I�ɗL���Ȃ̂悤�Ȃ̂ŁA
    // �O���t�B�b�N�X�G���W���̎g�p�󋵂ɂ�炸CPU �� �s��v�Z���s���悤�ɂ����B
    //
    if( !(pCurrentInstance_->opzHint & NNS_G2D_RDR_OPZHINT_NOT_SR) )
    {
        const u16 lastPos = NNSi_G2dGetMtxStackPos();
        NNSi_G2dMtxPush();
        {
            const u16 newPos = NNSi_G2dGetMtxStackPos();
            //
            // �J�����g�s��̏�Ԃ��X�V����    
            //
            NNSi_G2dMCMSetMtxStackPushed( newPos, lastPos );
        }
    }else{
        NNSi_G2dMtxPush();
    }    
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dPopMtx 

  Description:  �����_�������� �s��X�^�b�N���|�b�v���܂�
                
  Arguments:    �Ȃ�
                 
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dPopMtx()
{ 
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNSi_G2dMtxPop();
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTranslate 

  Description:  �����_�������� �J�����g�s��� ���s�ړ� �s�����Z���܂�
                
  Arguments:    x:      [IN]  trans x
                y:      [IN]  trans y
                z:      [IN]  trans z
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dTranslate(fx32 x, fx32 y, fx32 z )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNSi_G2dTranslate( x, y, z );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTranslate 

  Description:  �����_�������� �J�����g�s��� ���s�ړ� ������ݒ肵�܂��B
                
  Arguments:    x:      [IN]  trans x
                y:      [IN]  trans y
                z:      [IN]  trans z
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetTrans(fx32 x, fx32 y, fx32 z )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNSi_G2dSetTrans( x, y, z );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dScale 

  Description:  �����_�������� �J�����g�s��� �X�P�[�� �s�����Z���܂�
                
  Arguments:    x:      [IN]  scale x
                y:      [IN]  scale y
                z:      [IN]  scale z
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dScale(fx32 x, fx32 y, fx32 z )
{
#pragma unused( z )
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNSi_G2dScale( x, y );
    // 
    // affine transform �� �܂ޏꍇ�AMtx�� HW �ɓK�p����K�v������( 2D Graphics Engine )
    // ���� SR transform �� �܂܂Ȃ� ��Ԃł���� �V���� 
    //      SR transform �� �܂�     ��Ԃɐݒ肷��
    //
    if( !NNSi_G2dIsRndCurrentMtxSRTransformed() )
    {
        NNS_G2D_WARNING( pCurrentInstance_->rendererCore.flipFlag == NNS_G2D_RENDERERFLIP_NONE, 
                    "You can't use affine transformation using flip function." );
        NNSi_G2dSetRndMtxStackSRTransformed();
    }
    
    NNSi_G2dMCMSetCurrentMtxSRChanged();
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRotZ 

  Description:  �����_�������� �J�����g�s��� ��] �s�����Z���܂�
                
  Arguments:    sin:      [IN]  sin �l
                cos:      [IN]  cos �l
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRotZ( fx32 sin, fx32 cos )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT( pCurrentInstance_ );
    NNSi_G2dRotate( sin, cos );
    
    // 
    // SR transform �� �܂ޏꍇ�AMtx�� HW �ɓK�p����K�v������( 2D Graphics Engine )
    // ���� SR transform �� �܂܂Ȃ� ��Ԃł���� �V���� 
    //      SR transform �� �܂�     ��Ԃɐݒ肷��
    //
    if( !NNSi_G2dIsRndCurrentMtxSRTransformed() )
    {
        NNS_G2D_WARNING( pCurrentInstance_->rendererCore.flipFlag == NNS_G2D_RENDERERFLIP_NONE, 
                    "You can't use affine transformation using flip function." );
        NNSi_G2dSetRndMtxStackSRTransformed();
    }
    
    NNSi_G2dMCMSetCurrentMtxSRChanged();   
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRendererFlipMode 

  Description:  �����_���̃t���b�v�`��ݒ��ݒ肵�܂��B
                ���ӁF
                    �t���b�v�`�悪�L���ɂȂ��Ă���ꍇ�̓A�t�B���ϊ��@�\�𗘗p�ł��܂���B
                
                Begin - End rendering �u���b�N�̓��O�ǂ���ł��Ăяo�����Ƃ��\�ł��B
                
                
  Arguments:    pRend:       [OUT] �����_�� ����
                bFlipH:      [IN]  H�t���b�v�𗘗p���邩�H
                bFlipV:      [IN]  V�t���b�v�𗘗p���邩�H
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRendererFlipMode
( 
    NNSG2dRendererInstance* pRend, 
    BOOL bFlipH, 
    BOOL bFlipV 
)
{    
    NNS_G2D_WARNING( !NNSi_G2dIsRndCurrentMtxSRTransformed(), 
       "You can't use the flip function using affine transformation." );
    NNS_G2D_NULL_ASSERT( pRend );
    
    NNS_G2dSetRndCoreFlipMode( &pRend->rendererCore, bFlipH, bFlipV );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRendererPaletteTbl 

  Description:  �����_�� ���̂�  �J���[�p���b�g�ύX�e�[�u���ݒ��ݒ肵�܂�
  
                Begin - End rendering �u���b�N�̓��O�ǂ���ł��Ăяo�����Ƃ��\�ł��B
                
  Arguments:    pRend:      [OUT] �����_�� ����
                pTbl:       [IN]  �p���b�g�ϊ��e�[�u��
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRendererPaletteTbl
( 
    NNSG2dRendererInstance*         pRend, 
    const NNSG2dPaletteSwapTable*   pTbl 
)
{
    NNS_G2D_NULL_ASSERT( pRend );
    NNS_G2D_NULL_ASSERT( pTbl );
    
    pRend->pPaletteSwapTbl = pTbl;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetRendererPaletteTbl TODO:

  Description:  �����_�� ���� �� �J���[�p���b�g�ύX�e�[�u���ݒ���擾���܂�
                
                Begin - End rendering �u���b�N�̓��O�ǂ���ł��Ăяo�����Ƃ��\�ł��B
                
  Arguments:    pRend:      [OUT] �����_�� ����
                
  Returns:      �J���[�p���b�g�ύX�e�[�u��
  
 *---------------------------------------------------------------------------*/
const NNSG2dPaletteSwapTable* 
NNS_G2dGetRendererPaletteTbl( NNSG2dRendererInstance* pRend )
{
    NNS_G2D_NULL_ASSERT( pRend );
    
    return pRend->pPaletteSwapTbl;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetRendererPaletteTbl 

  Description:  �����_�� ���̂� �� �J���[�p���b�g�ύX�e�[�u���ݒ�����Z�b�g���܂�
  
                Begin - End rendering �u���b�N�̓��O�ǂ���ł��Ăяo�����Ƃ��\�ł��B
                
  Arguments:    pRend:      [OUT] �����_�� ����
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dResetRendererPaletteTbl( NNSG2dRendererInstance* pRend )
{
    NNS_G2D_NULL_ASSERT( pRend );
    pRend->pPaletteSwapTbl = NULL;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRendererImageProxy 

  Description:  �����_�� ���̂� �摜���A�p���b�g����ݒ肵�܂��B
                �����_�� ���̂��g�p����O�Ɏ��s���Ă��������B
                
                
                �{�֐��́ABegin End Rendering()�u���b�N���ł��g�p���\�ł��B
                
                ���ӁF
                �œK���t���O�ANNS_G2D_RDR_OPZHINT_LOCK_PARAMS �t���O���w�肵��
                �����_�����O���s���Ă���ꍇ�́ABegin End Rendering()�u���b�N���ł�
                �g�p���֎~����܂��B
                ����́A�����_���R�A���W���[����Begin-End Rendering()�Ăяo����
                �����_�����W���[����Begin-End Rendering()�Ɠ����^�C�~���O�ŌĂяo�����
                �l�ɂȂ邩��ł��B
                
                �����_���R�A���W���[���ł́A
                Begin-End Rendering()���ŉ摜�v���N�V�ݒ��؂�ւ��邱�Ƃ͋֎~����Ă��܂��B
                ( ImageProxy�Ɋ֘A����p�����[�^�̑O�v�Z��Begin Rendering()�ōs���Ă��邽�߂ł��B�j
                
                
                
                
                
  Arguments:    pRend:      [OUT] �����_�� ����
                pImgProxy:  [IN]  �����_�� �ɐݒ肷�� �摜���
                pPltProxy:  [IN]  �����_�� �ɐݒ肷�� �p���b�g���
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRendererImageProxy
( 
    NNSG2dRendererInstance*             pRend, 
    const NNSG2dImageProxy*             pImgProxy, 
    const NNSG2dImagePaletteProxy*      pPltProxy
)
{
    NNS_G2D_NULL_ASSERT( pRend );
    NNS_G2D_NULL_ASSERT( pImgProxy );
    NNS_G2D_NULL_ASSERT( pPltProxy );
    
    SDK_WARNING( pImgProxy->attr.bExtendedPlt == pPltProxy->bExtendedPlt, 
        "Palette type mismatching was detected.\n Make sure that you use the correct palette." );
    
    NNS_G2D_WARNING( !(pRend->opzHint & NNS_G2D_RDR_OPZHINT_LOCK_PARAMS),
        "Avoid calling this function, when you specified the optimize flag NNS_G2D_RDR_OPZHINT_LOCK_PARAMS." );
    
    NNS_G2dSetRndCoreImageProxy( &pRend->rendererCore, pImgProxy, pPltProxy );
}

