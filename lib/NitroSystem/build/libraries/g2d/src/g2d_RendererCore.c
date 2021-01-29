/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_RendererCore.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.34 $
 *---------------------------------------------------------------------------*/
#include <nnsys/g2d/g2d_RendererCore.h>
#include "g2di_RendererMtxCache.h"      // For MatrixCache & State


#include <nnsys/g2d/fmt/g2d_Cell_data.h>
#include <nnsys/g2d/g2d_OamSoftwareSpriteDraw.h>
#include <nnsys/g2d/g2d_CellTransferManager.h>




#define NNSi_G2D_RNDCORE_DUMMY_FLAG                FALSE
#define NNSi_G2D_RNDCORE_OAMENTORY_SUCCESSFUL      TRUE



static NNSG2dRndCoreInstance*     pTheInstance_ = NULL;
static const MtxFx32              mtxIdentity_ = { FX32_ONE,        0, 
                                                          0, FX32_ONE, 
                                                          0,        0 };
// �֐��� Begin - End Rendering �u���b�N �̒��ŌĂ΂�Ă��邩 
#define NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT(  ) \
       NNS_G2D_ASSERTMSG( pTheInstance_ != NULL, "Please call this method between Begin - End Rendering" ) 

// �֐��� Begin - End Rendering �u���b�N �̊O�ŌĂ΂�Ă��邩 
#define NNS_G2D_RND_OUTSIDE_BEGINEND_ASSERT(  ) \
       NNS_G2D_ASSERTMSG( pTheInstance_ == NULL, "Please call this method outside Begin - End Rendering" ) 



//------------------------------------------------------------------------------
// MtxChache affine Index �A�N�Z�T
// 
//    NNS_G2D_SURFACETYPE_MAIN3D = 0x00,  // 3D �O���t�B�b�N�X�G���W��   ���g�p
//    NNS_G2D_SURFACETYPE_MAIN2D = 0x01,  // 2D �O���t�B�b�N�X�G���W�� A ���g�p
//    NNS_G2D_SURFACETYPE_SUB2D  = 0x02,  // 2D �O���t�B�b�N�X�G���W�� B ���g�p
//    NNS_G2D_SURFACETYPE_MAX    = 0x03
//
// ���ӁFEnum �l �Ɩ��ڂ� �ˑ����Ă��鏈���ł� 
//
static NNS_G2D_INLINE u16 GetMtxCacheAffineIndex_
( 
    const NNSG2dRndCore2DMtxCache*     pMtxCache,
    NNSG2dSurfaceType                  surfaceType,
    OAM_FLIP                           flipType
)
{
    return pMtxCache->affineIndex[flipType][surfaceType - NNS_G2D_SURFACETYPE_MAIN2D];
}

//------------------------------------------------------------------------------
// 
//
static NNS_G2D_INLINE void SetMtxCacheAffineIndex_
( 
    NNSG2dRndCore2DMtxCache*    pMtxCache,
    NNSG2dSurfaceType           surfaceType,
    OAM_FLIP                    flipType,
    u16                         affineIndex
)
{
    pMtxCache->affineIndex[flipType][surfaceType - NNS_G2D_SURFACETYPE_MAIN2D] = affineIndex;
}

//------------------------------------------------------------------------------
// �o�^�O�ɈȑO�ɓo�^����Ă��邩�`�F�b�N���܂��B���̂��Ƃɂ���ďd���o�^������Ă��܂��B
// 
static NNS_G2D_INLINE BOOL IsMtxCacheRegisteredAsAffineParams_
(
    const u16 affineIdx
)
{
    return ( affineIdx != MtxCache_NOT_AVAILABLE ) ? TRUE : FALSE;
}



//------------------------------------------------------------------------------
static NNS_G2D_INLINE const MtxFx32* RndCoreGetCurrentMtx_()
{
    if( pTheInstance_->pCurrentMxt )
    { 
        //if( pTheInstance_->flipFlag != NNS_G2D_RENDERERFLIP_NONE )
        // TODO:�x����\������
        // �ݒ肳�ꂽ�s��
        return pTheInstance_->pCurrentMxt;
    }else{
        //
        // �P�ʍs��
        //    
        return &mtxIdentity_;
    }
}

//------------------------------------------------------------------------------
// ���݃����_���̓A�t�B���ϊ�(2D)���K�p����Ă����Ԃ��H
static NNS_G2D_INLINE BOOL IsRndCore2DAffineTransformed_
(
    const NNSG2dRndCoreInstance* pRnd 
)
{
    return (BOOL)( pRnd->flipFlag == NNS_G2D_RENDERERFLIP_NONE && 
                   pRnd->pCurrentMtxCacheFor2D != NULL );
}

//------------------------------------------------------------------------------
// �}�b�s���O�^�C�v�Ȃǂ���
// �o�C�g�T�C�Y���L�����N�^���ɕϊ�����V�t�g�l�����߂܂�
static NNS_G2D_INLINE int GetShiftToConvByteTo2DChar_( GXOBJVRamModeChar mappingType )
{
    int shift = ( REG_GX_DISPCNT_EXOBJ_MASK & mappingType ) >> REG_GX_DISPCNT_EXOBJ_SHIFT;
    
    return shift;
}

//------------------------------------------------------------------------------
// �V�t�g����l���V�t�g���̋��E�ォ���肵�܂�
static NNS_G2D_INLINE BOOL IsAlignedShiftValueBoundary_( u32 sizeByte, int shiftBit )
{
    const int mask = (0x1 << shiftBit) - 1;
    return (BOOL)( (mask & sizeByte) == 0);
}

//------------------------------------------------------------------------------
// �o�C�g���A�}�b�s���O�^�C�v�Ȃǂ���
// �L�����N�^�����Z�o���܂�
static NNS_G2D_INLINE u32 GetNum2DCharacter_( u32 sizeByte, GXOBJVRamModeChar mappingType )
{
   
    const int shiftBit  = 5 + GetShiftToConvByteTo2DChar_( mappingType ); // 5 means  /= 32 
    u32       numChar   = sizeByte >> shiftBit;
    
    NNS_G2D_ASSERT( IsAlignedShiftValueBoundary_( sizeByte, shiftBit ) ); 
    
    return numChar;
}

//------------------------------------------------------------------------------
// 3D Graphics Engine �̃e�N�X�`���ݒ���擾���܂�
// 
static NNS_G2D_INLINE u32 GetTexBaseAddr3D_( const NNSG2dImageProxy* pImgProxy )
{
    NNS_G2D_NULL_ASSERT( pImgProxy );
    if( NNS_G2dIsImageReadyToUse( pImgProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) )
    {
        return NNS_G2dGetImageLocation( pImgProxy, NNS_G2D_VRAM_TYPE_3DMAIN );
    }else{
        // TODO:�x��
        return 0;
    }
}

//------------------------------------------------------------------------------
// 3D Graphics Engine �̃p���b�g�ݒ���s���܂�
//
static NNS_G2D_INLINE u32 GetPltBaseAddr3D_( const NNSG2dImagePaletteProxy* pPltProxy )
{
    NNS_G2D_NULL_ASSERT( pPltProxy );
    if( NNS_G2dIsImagePaletteReadyToUse( pPltProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) )
    {
        return NNS_G2dGetImagePaletteLocation( pPltProxy, NNS_G2D_VRAM_TYPE_3DMAIN );
    }else{
        // TODO:�x��
        return 0;
    }
}


//------------------------------------------------------------------------------
// ���݂͂����L���X�g���邾���ł�
static NNS_G2D_INLINE NNS_G2D_VRAM_TYPE SurfaceTypeToVramType_( NNSG2dSurfaceType   surfaceType )
{
    return (NNS_G2D_VRAM_TYPE)surfaceType;
}

//------------------------------------------------------------------------------
// 2D Graphics Engine �̃L�����N�^�I�t�Z�b�g���擾���܂�
// 
static NNS_G2D_INLINE u32 GetCharacterBase2D_
( 
    const NNSG2dImageProxy* pImgProxy, 
    NNSG2dSurfaceType       type 
)
{
    
    NNS_G2D_NULL_ASSERT( pImgProxy );
    NNS_G2D_ASSERT( type == NNS_G2D_SURFACETYPE_MAIN2D || 
                    type == NNS_G2D_SURFACETYPE_SUB2D );

    
    {   
        const NNS_G2D_VRAM_TYPE       vramType = SurfaceTypeToVramType_( type );
   
        if( NNS_G2dIsImageReadyToUse( pImgProxy, vramType ) )
        {
            u32 baseAddr = NNS_G2dGetImageLocation( pImgProxy, vramType );
            
            return GetNum2DCharacter_( baseAddr, pImgProxy->attr.mappingType );     
            
        }else{
            // TODO:�x��
            return 0;
        }
    }
}



//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL IsOamAttrDoubleAffineMode_( const GXOamAttr* pAttr )
{
    return (BOOL)(pAttr->rsMode == 0x3);
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void HandleCellCallBackFunc_
( 
    NNSG2dRndCoreDrawCellCallBack   pFunc,
    const NNSG2dCellData*           pCell 
) 
{
    //
    // �R�[���o�b�N�֐��Ăяo�����s��
    //
    // �����ɍs��͂Ƃ�Ȃ����ƂƂ��� theInstance_ �� �n���Ă���̂����c
    if( pFunc )
    {
        (*pFunc)( pTheInstance_, pCell ); 
    } 
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void HandleCellOamBackFunc_
( 
    NNSG2dRndCoreDrawOamCallBack    pFunc,
    const NNSG2dCellData*           pCell,
    u16                             oamIdx 
) 
{
    //
    // �R�[���o�b�N�֐��Ăяo�����s��
    //
    // �����ɍs��͂Ƃ�Ȃ����ƂƂ��� theInstance_ �� �n���Ă���̂����c
    // 
    if( pFunc )
    {
        (*pFunc)( pTheInstance_, pCell, oamIdx ); 
    } 
}

//------------------------------------------------------------------------------
// �t���b�v�s����쐬���܂�
// ���[�h�̒��O�ɍ쐬�����l�ɓ��삪�ύX�ɂȂ�܂����B
static NNS_G2D_INLINE void MakeFlipMtx_
( 
    const MtxFx22*    pMtxSrc, 
    MtxFx22*          pMtxDst, 
    OAM_FLIP          type 
)
{
    NNS_G2D_NULL_ASSERT( pMtxSrc );
    NNS_G2D_NULL_ASSERT( pMtxDst );
    
    {
        *pMtxDst = *pMtxSrc;
        
        if( type & OAM_FLIP_H )
        {
            pMtxDst->_00 = -pMtxDst->_00;
            pMtxDst->_01 = -pMtxDst->_01;
        }
        
        if( type & OAM_FLIP_V )
        {
            pMtxDst->_10 = -pMtxDst->_10;
            pMtxDst->_11 = -pMtxDst->_11;
        }
    }

}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE u16 LoadMtxCacheAsAffineParams_
( 
    NNSG2dRndCore2DMtxCache*               pMtxCache,
    const NNSG2dRndCoreSurface*            pSurface, 
    OAM_FLIP                               type  
)
{
    u16    affineIdx  = GetMtxCacheAffineIndex_( pMtxCache, pSurface->type, type );
    
    //
    // �����ȑO�� Affine �p�����[�^�Ƃ��ĉ��ʃ��W���[���ɓo�^����Ă��Ȃ��s��Ȃ��...
    //
    if( !IsMtxCacheRegisteredAsAffineParams_( affineIdx ) )
    {
        NNS_G2D_NULL_ASSERT( pTheInstance_->pFuncOamAffineRegister );        
        
        //
        // �o�^���s���A���̌��ʂ��L���b�V���Ɋi�[���� 
        //
        {
            if( type == OAM_FLIP_NONE )
            {
                affineIdx = (*pTheInstance_->pFuncOamAffineRegister)( &pMtxCache->m22 );
            }else{
                MtxFx22     mtxTemp;
                MakeFlipMtx_( &pMtxCache->m22, &mtxTemp, type );
                affineIdx = (*pTheInstance_->pFuncOamAffineRegister)( &mtxTemp );
            }
                
            SetMtxCacheAffineIndex_( pMtxCache, pSurface->type, type, affineIdx );
        }
    }
    
    return affineIdx;
}


//------------------------------------------------------------------------------
static NNS_G2D_INLINE NNS_G2D_VRAM_TYPE 
ConvertSurfaceTypeToVramType_( NNSG2dSurfaceType surfaceType )
{
    return (NNS_G2D_VRAM_TYPE)(surfaceType);
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE u16 GetFlipedOBJPosX_( GXOamAttr* pOam, const GXOamShape shape )
{
    return NNS_G2dIsRndCoreFlipH( pTheInstance_ ) ? 
           (u16)( -NNS_G2dRepeatXinCellSpace(  (s16)pOam->x ) - NNS_G2dGetOamSizeX( &shape ) ):
           (u16)NNS_G2dRepeatXinCellSpace( (s16)pOam->x );
}
//------------------------------------------------------------------------------
static NNS_G2D_INLINE u16 GetFlipedOBJPosY_( GXOamAttr* pOam, const GXOamShape shape )
{
    return NNS_G2dIsRndCoreFlipV( pTheInstance_ ) ? 
           (u16)( -NNS_G2dRepeatYinCellSpace( (s16)pOam->y ) - NNS_G2dGetOamSizeY( &shape ) ) :
           (u16)NNS_G2dRepeatYinCellSpace( (s16)pOam->y );
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void OverwriteOamFlipFlag_( GXOamAttr* pOam )
{
    NNS_G2D_NULL_ASSERT( pOam );
    {
        //
        // �t���b�v���𑀍삷��
        //        
        const BOOL bFlipH = 
           NNS_G2dIsRndCoreFlipH( pTheInstance_ ) ^ NNSi_G2dGetOamFlipH( pOam );
        const BOOL bFlipV = 
           NNS_G2dIsRndCoreFlipV( pTheInstance_ ) ^ NNSi_G2dGetOamFlipV( pOam );
           
        pOam->attr01 &= ~GX_OAM_ATTR01_HF_MASK;
        pOam->attr01 |= bFlipH << GX_OAM_ATTR01_HF_SHIFT;
           
        pOam->attr01 &= ~GX_OAM_ATTR01_VF_MASK;
        pOam->attr01 |= bFlipV << GX_OAM_ATTR01_VF_SHIFT;
    }
}


//------------------------------------------------------------------------------
static void DoAffineTransforme_
( 
    const MtxFx22*    pMtxSR, 
    GXOamAttr*        pOam, 
    NNSG2dFVec2*      pBaseTrans
)
{
    NNSG2dFVec2          objTrans;
    GXOamEffect          effectTypeAfter;
    
    // Get oam's position data in Obj Loacal coordinate, and convert it to world coordinate.
    NNS_G2dGetOamTransFx32( pOam, &objTrans );
    
    //
    // �{�p�A�t�B�����ݒ肳��Ă���OBJ�̏ꍇ�A
    // �ʏ��OBJ�\���ʒu�ɑ΂��āA�ʒu�␳�l��t�����Ă���̂ŁA
    // �␳�l����U�A��苎��܂�
    //
    NNSi_G2dRemovePositionAdjustmentFromDoubleAffineOBJ( pOam, &objTrans );               
                        
    // �A�t�B���ϊ����s���܂�
    MulMtx22( pMtxSR, &objTrans, &objTrans );
    
    //
    // �A�t�B���ϊ�����Ă���Ȃ�A�A�t�B�����[�h�w��(�{�p or ����)
    // �i���ӁF�]���̃o�[�W�����ł͉��ʃ��W���[���ōs���Ă��������ł��j
    // 
    // �A�t�B�����[�h�݂̂�ݒ肵�����̂ŃA�t�B���p�����[�^�ԍ��ɂ͓K���Ȓl��ݒ肵�Ă��܂��B
    // �K����API��GX�ɗp�ӂ���Ă��Ȃ����߂ł��B
    // ������΍������\��ł��B
    //
    if( pTheInstance_->affineOverwriteMode != NNS_G2D_RND_AFFINE_OVERWRITE_NONE )
    {
        if( pTheInstance_->affineOverwriteMode == NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE  ) 
        {
           effectTypeAfter =  GX_OAM_EFFECT_AFFINE_DOUBLE;
        }else{
           effectTypeAfter =  GX_OAM_EFFECT_AFFINE;
        }
                            
        G2_SetOBJEffect( pOam, effectTypeAfter, 0 );
    }
    
    //
    // �{�p�A�t�B�����[�h�ƒʏ�A�t�B���ϊ����[�h�ł͕\���ʒu���قȂ邽��
    // �{�p�A�t�B�����[�h�ϊ����w�肳��Ă���OBJ�̏ꍇ��
    // �{�֐��ŕ␳���s���܂��B
    // 
    {
        // �{�p�A�t�B���Ȃ�{�p�A�t�B��OBJ�̂��߂̈ʒu�␳�������Ȃ�
        const BOOL bShouldAdjust = G2_GetOBJEffect( pOam ) == GX_OAM_EFFECT_AFFINE_DOUBLE;
                                 
        NNSi_G2dAdjustDifferenceOfRotateOrientation( pOam, 
                                                     pMtxSR, 
                                                     &objTrans, 
                                                     bShouldAdjust );
    }
    
    // add base offset( left top position of Obj )
    objTrans.x += pBaseTrans->x;
    objTrans.y += pBaseTrans->y;
    G2_SetOBJPosition( pOam, objTrans.x >> FX32_SHIFT, objTrans.y >> FX32_SHIFT  );
}

//------------------------------------------------------------------------------                                                             
static NNS_G2D_INLINE void DoFlipTransforme_( GXOamAttr* pOam, NNSG2dFVec2* pBaseTrans )
{    
    //
    // �t���b�v����
    //
    if( pTheInstance_->flipFlag != NNS_G2D_RENDERERFLIP_NONE )
    {    
        
        
        const GXOamShape shape = NNS_G2dGetOAMSize( pOam ); 
                                
        OverwriteOamFlipFlag_( pOam );
        
        if( NNS_G2dIsRndCoreFlipH( pTheInstance_ ) )
        {
            pOam->x = -pOam->x - NNS_G2dGetOamSizeX( &shape );
        }
        
        if( NNS_G2dIsRndCoreFlipV( pTheInstance_ ) )
        {
            pOam->y = (u8)(-pOam->y - NNS_G2dGetOamSizeY( &shape ));
        }
    }
    
    // add base offset( left top position of Obj )
    pOam->x += pBaseTrans->x >> FX32_SHIFT;
    pOam->y += pBaseTrans->y >> FX32_SHIFT;
}


//------------------------------------------------------------------------------
// �����_�����A�`��J�n�̏����������Ă��邩�𒲍����܂��B
// �f�o�b�N�r���h���ɃA�T�[�g���̒��ł̂ݎg�p����܂��B
//
static BOOL IsRndCoreReadyForBeginRendering_( NNSG2dRndCoreInstance* pRnd )
{
    NNS_G2D_NULL_ASSERT( pRnd );
    {
       //
       // �摜�v���N�V���ݒ肳��Ă��邩�H
       //
       if( !(pRnd->pImgProxy && pRnd->pPltProxy) )
       {
           OS_Warning("RendererCore:ImageProxy isn't ready.");
           return FALSE;
       }
       
       //
       // �`��ΏۃT�[�t�F�X���ݒ肳��Ă��邩�H
       //
       if( pRnd->pCurrentTargetSurface == NULL )
       {
           OS_Warning("RendererCore:TragetSurface isn't ready.");
           return FALSE;
       }else{
           // �T�[�t�F�X�̓A�N�e�B�u���H
           if( !pRnd->pCurrentTargetSurface->bActive )
           {
               OS_Warning("RendererCore:The current tragetSurface isn't active. Is it OK?");
               // �x���̂݁AFALSE �Ƃ͂��Ȃ��B
           }
       }
       
       //
       // 2D �`��̏ꍇOAM�o�^�֐����������ݒ肳��Ă��邩?       
       //
       if( pRnd->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN2D ||
           pRnd->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_SUB2D  )
       {
           if
           ( 
              ( pRnd->pFuncOamAffineRegister == NULL && 
                  pRnd->pFuncOamRegister == NULL      )            
           )
           {
               OS_Warning( "RendererCore:OAM-RegisterFunction For 2D Graphics Engine rendering isn't ready." );
               return FALSE;
           }
       }
    }
    // �������i
    return TRUE;
}

//------------------------------------------------------------------------------
// Cell �� 2D Graphics Engine ���g�p���� �`�悵�܂�
//
// 2D Graphics Engine �����^�����O��p�̏����ł��B
//
static void DrawCellToSurface2D_
( 
    const NNSG2dRndCoreSurface*    pSurface, 
    const NNSG2dCellData*          pCell
)
{
    NNSG2dFVec2          baseTrans;
    MtxFx22              mtxSR;
    const MtxFx32*       pCurrMtx = RndCoreGetCurrentMtx_();
    
    NNS_G2D_NULL_ASSERT( pSurface );
    NNS_G2D_NULL_ASSERT( pCell );
    
    
    // get current Mtx for affine transformation.
    NNSi_G2dGetMtxTrans( pCurrMtx, &baseTrans );  
    //
    // �r���[���[�J���n�ւ̕ϊ�
    //                  
    baseTrans.x -= pSurface->viewRect.posTopLeft.x;
    baseTrans.y -= pSurface->viewRect.posTopLeft.y;
    
    {
        const u32   baseCharOffset  = pTheInstance_->base2DCharOffset;
        const BOOL  bAffined        = IsRndCore2DAffineTransformed_( pTheInstance_ );
        
        u16          i;
        GXOamAttr*   pTempOam = &pTheInstance_->currentOam;
        u16          oamAffinIdx = NNS_G2D_OAM_AFFINE_IDX_NONE;
                        
        for( i = 0; i < pCell->numOAMAttrs; i++ )
        {
            //
            // Oam�A�g���r���[�g�`��O�R�[���o�b�N
            //
            pTheInstance_->bDrawEnable = TRUE;
            NNS_G2dCopyCellAsOamAttr( pCell, i, pTempOam );
            
            HandleCellOamBackFunc_( pSurface->pBeforeDrawOamBackFunc, pCell, i );
            
            //
            // �`��X�L�b�v����
            //
            if( !pTheInstance_->bDrawEnable )
            {
                continue;
            }
            
            
            
            //
            // �L�����N�^�ԍ��I�t�Z�b�g�̉��Z
            //
            pTempOam->charNo += baseCharOffset;
                
            
            //
            // ���W�ϊ�:�t���b�v�������A�t�B���ϊ�����
            //
            if( bAffined )
            {       
                //
                // �J�����g�s��X�^�b�N�����[�h�̕K�v�����邩���f���A
                // ���[�h���ăC���f�b�N�X��Ԃ��֐�
                // OBJ�̃t���b�v�t���O���`�F�b�N���A�K�؂ȃA�t�B���ϊ��s����쐬����̂�
                // OBJ���̃��[�v���Ŗ�����s����K�v������܂��B
                //
                oamAffinIdx = 
                LoadMtxCacheAsAffineParams_( pTheInstance_->pCurrentMtxCacheFor2D, 
                                             pSurface, 
                                             TO_OAM_FLIP( NNSi_G2dGetOamFlipH( pTempOam ), 
                                                          NNSi_G2dGetOamFlipV( pTempOam ) ));
                NNSi_G2dGetMtxRS( pCurrMtx, &mtxSR );
                DoAffineTransforme_( &mtxSR, pTempOam, &baseTrans );
            }else{
                oamAffinIdx = NNS_G2D_OAM_AFFINE_IDX_NONE;
                DoFlipTransforme_( pTempOam, &baseTrans );
            }

            //
            // �`��o�^�֐���т���
            // ��O�����͎g�p����Ă��Ȃ����߁A�_�~�[������^���Ă��܂��B
            //
            NNS_G2D_NULL_ASSERT( pTheInstance_->pFuncOamRegister );
            
            if( NNSi_G2D_RNDCORE_OAMENTORY_SUCCESSFUL 
                != (*pTheInstance_->pFuncOamRegister)( pTempOam, 
                                                      oamAffinIdx, 
                                                      NNSi_G2D_RNDCORE_DUMMY_FLAG ) )
            {
                // we have no capacity for new oam datas.
                return;
            }
             
            //
            // Oam�A�g���r���[�g�`���R�[���o�b�N
            //
            HandleCellOamBackFunc_( pSurface->pAfterDrawOamBackFunc, pCell, i );
        }
    }
}

//------------------------------------------------------------------------------
static void DrawOamToSurface3D_
( 
    GXOamAttr*                     pOam
)
{
    // �s��ݒ�
    G3_LoadMtx43( &pTheInstance_->mtxFor3DGE );
        
    if( pTheInstance_->flipFlag != NNS_G2D_RENDERERFLIP_NONE )
    {   
        const GXOamShape shape = NNS_G2dGetOAMSize( pOam );
    
        //
        // OBJ �� �`��ʒu���擾����
        //
    	const s16 posX = (s16)GetFlipedOBJPosX_( pOam, shape );
        const s16 posY = (s16)GetFlipedOBJPosY_( pOam, shape );
        const s16 posZ = -1;      
        
        //
        // �t���b�v���𑀍삷��
        //        
        OverwriteOamFlipFlag_( pOam );
        
        
        NNS_G2dDrawOneOam3DDirectWithPosFast( posX, posY, posZ, 
                                      pOam, 
                                      &pTheInstance_->pImgProxy->attr, 
                                       pTheInstance_->baseTexAddr3D, 
                                       pTheInstance_->basePltAddr3D );        
    }else{
        const s16 posX = (s16)NNS_G2dRepeatXinCellSpace( (s16)pOam->x );
        const s16 posY = (s16)NNS_G2dRepeatYinCellSpace( (s16)pOam->y );
        const s16 posZ = -1;
        
        NNS_G2dDrawOneOam3DDirectWithPosFast( posX, posY, posZ, 
                                      pOam, 
                                      &pTheInstance_->pImgProxy->attr, 
                                       pTheInstance_->baseTexAddr3D, 
                                       pTheInstance_->basePltAddr3D );    
    }
    
}
//------------------------------------------------------------------------------
// Cell �� 3D Graphics Engine ���g�p���� �`�悵�܂�
// 
static void DrawCellToSurface3D_
( 
    const NNSG2dRndCoreSurface*    pSurface, 
    const NNSG2dCellData*          pCell
)
{
    u16 i = 0;
    
    GXOamAttr*            pTempOam = &pTheInstance_->currentOam;
    
    
    NNS_G2D_NULL_ASSERT( pSurface );
    NNS_G2D_NULL_ASSERT( pCell );
    
    
    // Draw All Objects
    for( i = 0; i < pCell->numOAMAttrs; i++ )
    {
        //
        // Oam�A�g���r���[�g�`��O�R�[���o�b�N
        //
        pTheInstance_->bDrawEnable = TRUE;
        NNS_G2dCopyCellAsOamAttr( pCell, i, pTempOam );
        
        HandleCellOamBackFunc_( pSurface->pBeforeDrawOamBackFunc, pCell, i );
        if( pTheInstance_->bDrawEnable )            
        {            
            // �`��
            DrawOamToSurface3D_( pTempOam );
        }
        //
        // Oam�A�g���r���[�g�`��O�R�[���o�b�N
        //
        HandleCellOamBackFunc_( pSurface->pAfterDrawOamBackFunc, pCell, i );
    }
}



//------------------------------------------------------------------------------
static NNS_G2D_INLINE void DrawCellImpl_
( 
    const NNSG2dCellData*   pCell, 
    u32                     cellVramTransferHandle 
)
{
    const NNSG2dRndCoreSurface*   pSurface = pTheInstance_->pCurrentTargetSurface;
    
    //
    // �A�N�e�B�u�łȂ���Ε`�揈���͉������Ȃ�
    //
    if( !pSurface->bActive )
    {
        return;
    }
    
    // �J�����O�A�}�g���N�X���[�h���R�[���o�b�N�Ɋ܂܂��
    pTheInstance_->bDrawEnable = TRUE;
    HandleCellCallBackFunc_( pSurface->pBeforeDrawCellBackFunc, pCell );
    if( pTheInstance_->bDrawEnable )
    {        
        //
        // VRAM �]���Z���Ȃ�΁A�Z���̕`�悪�s��ꂽ���Ƃ�VRAM�]����ԃI�u�W�F�N�g�ɋL�^����
        //
        if( cellVramTransferHandle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE )
        {
            NNSi_G2dSetVramTransferCellDrawnFlag
            ( 
                cellVramTransferHandle, 
                ConvertSurfaceTypeToVramType_( pSurface->type ),
                TRUE 
            );
        }
        
        //
        // �T�[�t�F�X�̎�ނ��Ƃ�...
        //
        switch( pSurface->type )
        {
        case NNS_G2D_SURFACETYPE_MAIN3D:
            DrawCellToSurface3D_( pSurface, pCell ); 
            break;
        case NNS_G2D_SURFACETYPE_MAIN2D:
        case NNS_G2D_SURFACETYPE_SUB2D:
            DrawCellToSurface2D_( pSurface, pCell );
            break;
        case NNS_G2D_SURFACETYPE_MAX:
        default:
            NNS_G2D_ASSERT(FALSE);
        }
    }
    HandleCellCallBackFunc_( pSurface->pAfterDrawCellBackFunc, pCell );
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitRndCore

  Description:  �����_���R�A���W���[�������������܂��B
                NNSG2dRndCoreInstance�𗘗p����O�ɕK�����s����悤�ɂ��Ă��������B
                
                
  Arguments:    pRnd          [OUT] �����_���R�A
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitRndCore( NNSG2dRndCoreInstance* pRnd )
{
    MI_CpuFill16( pRnd, 0x0, sizeof( NNSG2dRndCoreInstance ) );
    
    pRnd->pCurrentTargetSurface = NULL;
    
    pRnd->affineOverwriteMode  = NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE;
    
    pRnd->pImgProxy = NULL;
    pRnd->pPltProxy = NULL;
    
    pRnd->flipFlag = NNS_G2D_RENDERERFLIP_NONE;
    
    pRnd->bDrawEnable = TRUE;
    
    pRnd->pCurrentMtxCacheFor2D = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitRndCoreSurface

  Description:  �����_���R�A�`��ΏۃT�[�t�F�X�����������܂��B
                NNSG2dRndCoreSurface�𗘗p����O�ɕK�����s����悤�ɂ��Ă��������B
                
                
  Arguments:    pSurface          [OUT] �����_���R�A�`��ΏۃT�[�t�F�X
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitRndCoreSurface( NNSG2dRndCoreSurface* pSurface )
{
    MI_CpuFill16( pSurface, 0x0, sizeof( NNSG2dRndCoreSurface ) );
    
    pSurface->bActive   = TRUE;
    pSurface->type      = NNS_G2D_SURFACETYPE_MAX;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreImageProxy

  Description:  �����_���R�A���W���[�����`��̍ۂɎQ�Ƃ���摜�v���N�V��ݒ肵�܂��B
                �����_�����W���[���Ƃ͈قȂ�ABegin-EndRendering�u���b�N����
                �摜�v���N�V��ύX���邱�Ƃ͕s���ł��B
                ����́A�����_���R�A���W���[����
                BeginRendering�̃^�C�~���O�Ńp�����[�^�v�Z���s�����߂ł��B
                
                
                �{�֐��́ABegin-EndRendering�u���b�N�̊O�ŌĂяo���K�v������܂��B
                                
  Arguments:    pRnd                      [OUT] �����_���R�A
                pImgProxy                 [IN]  �摜�v���N�V
                pPltProxy                 [IN]  �摜�p���b�g�v���N�V
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreImageProxy
( 
    NNSG2dRndCoreInstance*              pRnd,
    const NNSG2dImageProxy*             pImgProxy, 
    const NNSG2dImagePaletteProxy*      pPltProxy
)
{
    NNS_G2D_RND_OUTSIDE_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_NULL_ASSERT( pImgProxy );
    NNS_G2D_NULL_ASSERT( pPltProxy );
    
    pRnd->pImgProxy = pImgProxy;
    pRnd->pPltProxy = pPltProxy;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreOamRegisterFunc

  Description:  �����_���R�A���W���[�����`��OBJ��\��OAM���O�����W���[���ɓo�^����ۂɎg�p����
                �֐��|�C���^�Q��ݒ肵�܂��B
                
                OAM�o�^�֐��́A�ݒ肳��Ă���T�[�t�F�X���A
                2D �O���t�B�b�N�X�G���W���`��T�[�t�F�X�ł���ꍇ�A
                �K���ݒ肳��Ă���K�v������܂��B
                
                �{�֐��͕ʂɁAOAM�o�^�֐��Ƃ��ău���b�N�]���o�^�֐���ݒ肷��
                NNS_G2dSetRndCoreOamRegisterFuncEx()���p�ӂ���Ă��܂��B
                
                ���ӁF�{�֐����Ăяo���ƁA�u���b�N�]���o�^�֐������Z�b�g����܂��B
                
                
                �{�֐��́ABegin-EndRendering�u���b�N�̊O�ŌĂяo���K�v������܂��B
                                
  Arguments:    pRnd                             [OUT] �����_���R�A
                pFuncOamRegister                 [IN]  OAM�o�^�֐�
                pFuncOamAffineRegister           [IN]  �A�t�B���p�����[�^�o�^�֐�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreOamRegisterFunc
(
    NNSG2dRndCoreInstance*          pRnd, 
    NNSG2dOamRegisterFunction       pFuncOamRegister,
    NNSG2dAffineRegisterFunction    pFuncOamAffineRegister
)
{
    NNS_G2D_RND_OUTSIDE_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_NULL_ASSERT( pFuncOamRegister );
    NNS_G2D_NULL_ASSERT( pFuncOamAffineRegister );
    
    pRnd->pFuncOamRegister       = pFuncOamRegister;
    pRnd->pFuncOamAffineRegister = pFuncOamAffineRegister;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreAffineOverwriteMode

  Description:  �A�t�B���ϊ����[�h�̏㏑�������ݒ肵�܂��B
                
                �㏑������̎�ނ�\���񋓎q�̒�`�͈ȉ��̂Ƃ���ł��B
                
                typedef enum NNSG2dRendererAffineTypeOverwiteMode
                {
                    NNS_G2D_RND_AFFINE_OVERWRITE_NONE,  // �㏑�����Ȃ�
                    NNS_G2D_RND_AFFINE_OVERWRITE_NORMAL,// �ʏ�̃A�t�B���ϊ������ɐݒ�
                    NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE // �{�p�A�t�B���ϊ������ɐݒ�
                  
                }NNSG2dRendererAffineTypeOverwiteMode;
                
                
                �{�֐��́ABegin-EndRendering�u���b�N�̓��O�ŌĂяo�����\�ł��B
                
                
  Arguments:    pCurrentMxt         [IN]  �A�t�B���ϊ��s��
                mode                [IN]  �A�t�B���ϊ����[�h�̏㏑�����������킷�񋓎q
                                         �iNNSG2dRendererAffineTypeOverwiteMode�j
                                    
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreAffineOverwriteMode
( 
    NNSG2dRndCoreInstance*                  pRnd, 
    NNSG2dRendererAffineTypeOverwiteMode    mode 
)
{
    NNS_G2D_NULL_ASSERT( pRnd );
    
    pRnd->affineOverwriteMode = mode;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreCurrentMtx3D

  Description:  �A�t�B���ϊ��s���ݒ肵�܂��B
                �A�t�B���ϊ��s��́A
                 �E3D �O���t�B�b�N�X�G���W���`�掞�ɃW�I���g���G���W���ɐݒ肳���J�����g�s��
                �Ɏg�p����܂��B
                
                �ݒ肵���ϊ��s���Begin-EndRendering�u���b�N���ŗL���ł��B
                NNS_G2dRndCoreEndRendering()�Ăяo�����ɐݒ肪���Z�b�g����܂��̂ŁA
                �V���ɕ`�揈�����s���ۂɂ́A�ēx�{�֐����Ăяo���Đݒ���s���K�v������܂��B
                
                
                �����ŁA�W�I���g���G���W���̃J�����g�s��ɐݒ肷��4x3 �s����v�Z���܂��B
                �{�֐��́A3D �O���t�B�b�N�X�G���W���`�掞�݂̂Ɏg�p����܂��B
                2D �O���t�B�b�N�X�G���W���`�掞�ɖ{�֐����Ăяo�����ꍇ
                �x�������b�Z�[�W���o�͂���܂��B
                
                �{�֐��́ABegin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
                
  Arguments:    pCurrentMxt         [IN]  �J�����g�ϊ��s��
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreCurrentMtx3D
( 
    const MtxFx32*              pCurrentMxt
)
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pCurrentMxt );
    NNS_G2D_WARNING( pTheInstance_->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN3D,
       "This method works only for 3D graphics engine rendering." );
       
    {
       NNSG2dRndCoreInstance*      pRnd = pTheInstance_;
         
       pRnd->pCurrentMxt            = pCurrentMxt;
        
       //
       // 3D Graphics Engine �ŕ`�悷��ꍇ�A�W�I���g���G���W���̃J�����g�s��ɐݒ肷��
       // 4x3 �s����v�Z���Ă����B
       // 2D Graphics Engine �ŕ`�悷��ꍇ�́A
       // pRnd->pCurrentMxt�́A�Z���̃A�t�B���ϊ��Ɏg�p���邾���Ȃ̂� 3x2 �s��̂܂܂ŏ\���ł���B
       //
       {
           //
           // �W�I���g���G���W���Ƀ��[�h����4x3�s��̐���
           //
           {
              pRnd->mtxFor3DGE._00 = pCurrentMxt->_00;
              pRnd->mtxFor3DGE._01 = pCurrentMxt->_01;
              pRnd->mtxFor3DGE._02 = 0;

              pRnd->mtxFor3DGE._10 = pCurrentMxt->_10;
              pRnd->mtxFor3DGE._11 = pCurrentMxt->_11;
              pRnd->mtxFor3DGE._12 = 0;

              pRnd->mtxFor3DGE._20 = 0;
              pRnd->mtxFor3DGE._21 = 0;
              pRnd->mtxFor3DGE._22 = FX32_ONE;

              pRnd->mtxFor3DGE._30 = pCurrentMxt->_20;
              pRnd->mtxFor3DGE._31 = pCurrentMxt->_21;
              pRnd->mtxFor3DGE._32 = pRnd->zFor3DSoftwareSprite;
           }
       }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreCurrentMtx2D

  Description:  2D �O���t�B�b�N�X�G���W���`��̍ۂ�OBJ�ɓK�p����A�t�B���ϊ��s���
                �A�t�B���ϊ��p�����[�^��ݒ肵�܂��B
                �A�t�B���ϊ��s��́A
                 �E2D �O���t�B�b�N�X�G���W���`�掞��CPU��ōs����OBJ�ʒu�̍��W�ϊ�
                �Ɏg�p����܂��B
                
                �A�t�B���p�����[�^�͍s��L���b�V�����g�p���Đݒ肵�܂��B
                �s��L���b�V���ւ̃|�C���^�Ƃ���NULL���w�肳�ꂽ�ꍇ�̓A�t�B���ϊ����s��Ȃ����̂Ƃ���
                ���f���܂��B
                
                �ݒ肵���A�t�B���p�����[�^��Begin-EndRendering�u���b�N���ŗL���ł��B
                NNS_G2dRndCoreEndRendering()�Ăяo�����ɐݒ肪���Z�b�g����܂��̂ŁA
                �ēx�{�֐����Ăяo���Đݒ���s���K�v������܂��B
                
                
                �{�֐��́A2D �O���t�B�b�N�X�G���W���`�掞�݂̂Ɏg�p����܂��B
                3D �O���t�B�b�N�X�G���W���`�掞�ɖ{�֐����Ăяo�����ꍇ
                �x�������b�Z�[�W���o�͂���܂��B
                
                �{�֐��́ABegin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
                
  Arguments:    pMtx                          [IN]  �J�����g�ϊ��s��
                pCurrentMtxCacheFor2D         [IN]  �A�t�B���p�����[�^���i�[�����s��L���b�V��
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
static BOOL CheckMtx2DParamsValid
(
    const MtxFx32*                 pMtx,
    NNSG2dRndCore2DMtxCache*       pCurrentMtxCacheFor2D
)
{
    if( !( pMtx->_00 == FX32_ONE && pMtx->_01 == 0 &&
           pMtx->_10 == 0        && pMtx->_11 == FX32_ONE ) )
    {
       if( pCurrentMtxCacheFor2D == NULL )
       {
           NNS_G2D_WARNING( FALSE,
              "Make sure that you have to specified the affine-mtx for the 2D graphics engine when you use affine transformation." );
           return FALSE;
       }
    }else{
       //
       // �Ƃ肠�����A�x���͍s��Ȃ����Ƃ���
       //
       /*
       if( pCurrentMtxCacheFor2D != NULL )
       {
           NNS_G2D_WARNING( FALSE,
              "The affine mtx setting is useless when you don't use affine transformation." );
           return FALSE;
       }
       */
    }
    return TRUE;
}
//------------------------------------------------------------------------------
void NNS_G2dSetRndCoreCurrentMtx2D
( 
    const MtxFx32*                 pMtx,
    NNSG2dRndCore2DMtxCache*       pCurrentMtxCacheFor2D
)
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pMtx );
    
    NNS_G2D_WARNING( pTheInstance_->pCurrentTargetSurface->type != NNS_G2D_SURFACETYPE_MAIN3D,
       "This method works only for 2D graphics engine rendering." );
    NNS_G2D_ASSERT( CheckMtx2DParamsValid( pMtx, pCurrentMtxCacheFor2D ) );
    
           
    {
       NNSG2dRndCoreInstance*      pRnd = pTheInstance_;
       
       pRnd->pCurrentMxt            = pMtx;
       pRnd->pCurrentMtxCacheFor2D  = pCurrentMtxCacheFor2D;
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCore3DSoftSpriteZvalue

  Description:  �\�t�g�E�F�A�X�v���C�g�`��̍ۂɎg�p�����X�v���C�g��Z�l��ݒ肵�܂��B
                
  Arguments:    pRnd        [OUT] �����_���R�A
                z           [IN]  �\�t�g�E�F�A�X�v���C�g�`��Ɏg�p�����z�l
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCore3DSoftSpriteZvalue( NNSG2dRndCoreInstance* pRnd, fx32 z )
{
    pRnd->zFor3DSoftwareSprite = z;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreSurface

  Description:  �����_���R�A�ɕ`��ΏۃT�[�t�F�X��ݒ肵�܂��B
  
                Begin-EndRendering�u���b�N�̊O�ŌĂяo���K�v������܂��B
                
  Arguments:    pRnd        [OUT] �����_���R�A
                pSurface    [IN]  �`��ΏۃT�[�t�F�X
                
  Returns:      �Ȃ��B
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreSurface
( 
    NNSG2dRndCoreInstance*      pRnd, 
    NNSG2dRndCoreSurface*       pSurface 
)
{
    NNS_G2D_RND_OUTSIDE_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_NULL_ASSERT( pSurface );
    //
    // TODO:�A�N�e�B�u���ǂ����������A�x�����b�Z�[�W��\������
    //
    pRnd->pCurrentTargetSurface = pSurface;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsRndCoreFlipH

  Description:  �����_���R�A�̕`��t���b�v��Ԃ��擾���܂��B
                
  Arguments:    pRnd        [IN]  �����_���R�A
                
  Returns:      ���������t���b�v�� ON �̂Ƃ� TRUE
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dIsRndCoreFlipH( const NNSG2dRndCoreInstance* pRnd )
{
    NNS_G2D_NULL_ASSERT( pRnd );
    return (BOOL)( (pRnd->flipFlag & NNS_G2D_RENDERERFLIP_H) != 0 );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsRndCoreFlipV

  Description:  �����_���R�A�̕`��t���b�v��Ԃ��擾���܂��B
                
  Arguments:    pRnd        [IN]  �����_���R�A
                
  Returns:      ���������t���b�v�� ON �̂Ƃ� TRUE
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dIsRndCoreFlipV( const NNSG2dRndCoreInstance* pRnd )
{
    NNS_G2D_NULL_ASSERT( pRnd );
    return (BOOL)( (pRnd->flipFlag & NNS_G2D_RENDERERFLIP_V) != 0 );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreFlipMode

  Description:  �����_���R�A�̕`��t���b�v��Ԃ�ݒ肵�܂��B
                Begin-EndRendering�u���b�N�̓��O�ǂ���ł��Ăяo���\�ł��B
                �A�t�B���ϊ��@�\���p���̖{�֐��̌Ăяo���͋֎~����Ă���A�A�T�[�g�Ɏ��s���܂��B
                
  Arguments:    pRnd        [OUT] �����_���R�A
                bFlipH      [IN]  ���������t���b�v
                bFlipV      [IN]  ���������t���b�v
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreFlipMode
( 
    NNSG2dRndCoreInstance* pRnd, 
    BOOL bFlipH, 
    BOOL bFlipV 
)
{
    //
    // ����SR�ϊ�����Ă��Ȃ�����������
    // 
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_WARNING( pRnd->pCurrentMtxCacheFor2D == NULL, 
       "You can't use the flip function using affine transformation." );

    if( bFlipH )
    {
        pRnd->flipFlag |= NNS_G2D_RENDERERFLIP_H;
    }else{
        pRnd->flipFlag &= ~NNS_G2D_RENDERERFLIP_H;
    }
    
    if( bFlipV )
    {
        pRnd->flipFlag |= NNS_G2D_RENDERERFLIP_V;
    }else{
        pRnd->flipFlag &= ~NNS_G2D_RENDERERFLIP_V;
    }
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRndCoreBeginRendering

  Description:  �����_���R�A�̕`����J�n���܂��B
                �e��`��J�n�̂��߂̑O�������s���܂��B
                
                �{�֐����Ăяo������́A�摜�v���N�V�A�Ώۃ����_�T�[�t�F�X�̃p�����[�^��
                �ύX�͍s���܂���̂Œ��ӂ��Ă��������B
                �{�֐����Ă񂾌�A�`��I����ɕK�� NNS_G2dRndCoreEndRendering() ���Ăяo���Ă��������B
                
                Begin-EndRendering�u���b�N���ł�Begin-EndRendering�֐��̌Ăяo��(����q�Ăяo��)
                �͋֎~����Ă���A�Ăяo�����s�����ꍇ�́A�A�T�[�g�Ɏ��s���܂��B
                
  Arguments:    pRnd        [IN]  �����_���R�A
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRndCoreBeginRendering( NNSG2dRndCoreInstance* pRnd )
{
    NNS_G2D_NULL_ASSERT( pRnd );
    NNS_G2D_ASSERT( pTheInstance_ == NULL );
    NNS_G2D_ASSERTMSG( IsRndCoreReadyForBeginRendering_( pRnd ), 
       "NNSG2dRndCoreInstance isn't ready to begin rendering." );
    
    pTheInstance_ = pRnd;
    
    //
    // 3D �`��ŗL�O����
    //
    if( pRnd->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN3D )
    {
        // �J�����ݒ�    
        G3_MtxMode( GX_MTXMODE_PROJECTION );
        G3_PushMtx();
        G3_Translate( -pRnd->pCurrentTargetSurface->viewRect.posTopLeft.x, 
                      -pRnd->pCurrentTargetSurface->viewRect.posTopLeft.y, 0 );
        G3_MtxMode( GX_MTXMODE_POSITION );
        
        //
        // �x�[�X�A�h���X�̌v�Z
        //
        pRnd->baseTexAddr3D = GetTexBaseAddr3D_( pTheInstance_->pImgProxy );
        pRnd->basePltAddr3D = GetPltBaseAddr3D_( pTheInstance_->pPltProxy );
        
    }else{
    //
    // 2D �`��ŗL�O����
    //
        pTheInstance_->base2DCharOffset 
           = GetCharacterBase2D_( pTheInstance_->pImgProxy, 
                                  pTheInstance_->pCurrentTargetSurface->type );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRndCoreEndRendering

  Description:  �����_���R�A�̕`����I�����܂��B
                �e��`��I���̂��߂̌㏈�����s���܂��B
                
                OAM�o�^�֐��Ƃ���OAM�u���b�N�]���֐����ݒ肳��Ă���ꍇ��
                �{�֐����ŁA�u���b�N�]���֐����g�p���āA�O�����W���[���ւƓ���OAM�o�b�t�@��
                ���e���R�s�[����܂��B
                �]���͕K���������邱�Ƃ����҂���Ă���A�]���Ɏ��s�����ꍇ��
                �A�T�[�g�Ɏ��s���܂��B
                �i�]���Ɏ��s����ꍇ�́A����OAM�o�b�t�@�̃o�b�t�@�T�C�Y�̐ݒ��
                ��肪���邱�Ƃ��\�z����܂��B�j
                
  Arguments:    �Ȃ�
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRndCoreEndRendering( void )
{
    NNS_G2D_NULL_ASSERT( pTheInstance_ );
     
    //
    // 3D �`��ŗL�㏈��
    //
    if( pTheInstance_->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN3D )
    {
        // �J�����ݒ�(restore)
        G3_MtxMode( GX_MTXMODE_PROJECTION );
        G3_PopMtx(1);
        G3_MtxMode( GX_MTXMODE_POSITION );
        
    }else{
    //
    // 2D �`��ŗL�㏈��
    //    
        pTheInstance_->base2DCharOffset = 0;
        //
        // �A�t�B���ϊ��ݒ�̃��Z�b�g
        //
        pTheInstance_->pCurrentMxt = NULL;
        pTheInstance_->pCurrentMtxCacheFor2D = NULL;
    }    
    pTheInstance_ = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRndCoreDrawCell

  Description:  �Z����`�悵�܂��B
                Begin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
  Arguments:    pCell:                    [IN]  �Z���f�[�^
  
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRndCoreDrawCell( const NNSG2dCellData* pCell )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pCell );
    
    DrawCellImpl_( pCell, NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );   
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRndCoreDrawCellVramTransfer

  Description:  VRAM�]���Z����`�悵�܂��B
                Begin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
  Arguments:    pCell:                    [IN]  �Z���f�[�^
                cellVramTransferHandle:   [IN]  �Z��VRAM�]����ԃI�u�W�F�N�g�̃n���h��   
  
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRndCoreDrawCellVramTransfer( const NNSG2dCellData* pCell, u32 cellVramTransferHandle )
{
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    NNS_G2D_NULL_ASSERT( pCell );
    
    NNS_G2D_ASSERT( cellVramTransferHandle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE );
    DrawCellImpl_( pCell, cellVramTransferHandle );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRndCoreDrawCellFast2D

  Description:  2D �O���t�B�b�N�G���W��(OBJ�@�\)�𗘗p���ăZ���������ɕ`�悵�܂��B
                
                �{�֐��́A�R�[���o�b�N�֐��Ăяo���A�Z���̃A�t�B���ϊ����T�|�[�g���܂���B
                2D�p�A�t�B���ϊ��s��ipCurrentMtxCacheFor2D�j���ݒ肳�ꂽ��ԂŁA
                �{�֐����Ăяo�����ꍇ�A�A�T�[�g�Ɏ��s���܂��B
                
                Begin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
  Arguments:    pCell:      [IN]  �Z���f�[�^
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRndCoreDrawCellFast2D( const NNSG2dCellData* pCell )
{
    const NNSG2dRndCoreSurface*   pSurface = pTheInstance_->pCurrentTargetSurface;    
    NNSG2dFVec2          baseTrans;
    const MtxFx32*       pCurrMtx = RndCoreGetCurrentMtx_();
    
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    
    
    
    NNS_G2D_ASSERTMSG( pSurface->type == NNS_G2D_SURFACETYPE_MAIN2D || 
                      pSurface->type == NNS_G2D_SURFACETYPE_SUB2D,
                      "This method can work only for the 2D Graphics Engine." );
       
    NNS_G2D_ASSERTMSG( pTheInstance_->pCurrentMtxCacheFor2D == NULL,
                      "You can't use this method using affine transfomation." );
    NNS_G2D_NULL_ASSERT( pCell );   
    //------------------------------------------------------------------------------
    // �Z�� �� 2D Graphics Engine ���g�p���� �`�悵�܂�
    // �R�[���o�b�N�Ăяo���Ȃǂ��T�|�[�g���܂��񂪍����ɓ��삷��o�[�W�����ł��B
    // 

    // get current Mtx for affine transformation.
    NNSi_G2dGetMtxTrans( pCurrMtx, &baseTrans );  
       
    //
    // �r���[���[�J���n�ւ̕ϊ�
    //                  
    baseTrans.x -= pSurface->viewRect.posTopLeft.x;
    baseTrans.y -= pSurface->viewRect.posTopLeft.y;
    //
    // NNS_G2dMakeCellToOams() �Ƃقړ����������s���܂��B
    // �i�������A�t���b�v���T�|�[�g���܂��B�j
    //
    //
    // �u���b�N�]�����g�p���Ȃ��o�[�W����
    // OBJ�o�^�̓x�ɁA�`��o�^�֐���т����̃I�[�o�[�w�b�h���������܂��B
    // �u���b�N�]�����g�p����o�[�W�����Ɣ�ׂāA�p�t�H�[�}���X�I�ɂ͕s���ł����A
    // ���Ƃ��Ɩ{�֐��̕`��o�^������1�t���[���ɐ�߂銄���͏������̂�
    // �i128 ��X2 ��OBJ��o�^�����Ƃ��Ă��j�A���͂Ȃ��Ɣ��f���{�������̗p���邱�ƂƂ��܂��B
    // 
    {
       u16          i;
       GXOamAttr*   pTempOam;
       const u32    baseCharOffset = pTheInstance_->base2DCharOffset;

       for( i = 0; i < pCell->numOAMAttrs; i++ )
       {
           pTempOam = &pTheInstance_->currentOam;
                        
           NNS_G2dCopyCellAsOamAttr( pCell, i, pTempOam );
           
           DoFlipTransforme_( pTempOam, &baseTrans );
                        
           pTempOam->charNo += baseCharOffset;
                        
           //
           // �`��o�^�֐���т���
           //
           NNS_G2D_NULL_ASSERT( pTheInstance_->pFuncOamRegister );
           if( FALSE == (*pTheInstance_->pFuncOamRegister)( pTempOam, NNS_G2D_OAM_AFFINE_IDX_NONE, FALSE ) )
           {
              // we have no capacity for new oam datas.
              return;
           }
       }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCoreCellCloneSource3D

  Description:  NNS_G2dRndCoreDrawCellClone3D()�ŎQ�Ƃ����AUV�p�����[�^�����O�v�Z���A
                UV�p�����[�^�L���b�V���Ƃ��ĕۑ����܂��B
                ���͂��ꂽ�A�Z��������̉摜�f�[�^������OBJ����\������Ă���Ƒz�肵�āA
                �C���f�b�N�X0�Ԃ�OBJ�ɂ���UV�l���v�Z���A�L���b�V���Ƃ��ĕۑ����܂��B
                
                ���ӁF�t���b�v���ꂽ�X�v���C�g��UV�p�����[�^�͒ʏ��UV�p�����[�^�Ƃ͈قȂ�܂��B
                ���������ē���̉摜�f�[�^������OBJ�Ƃ݂͂Ȃ��Ȃ����ƂɂȂ�܂��B
                3D �T�[�t�F�X��p�̏����ł��B���݂̃T�[�t�F�X��ނ��A
                NNS_G2D_SURFACETYPE_MAIN3D�ȊO�̏ꍇ�̓A�T�[�g�Ɏ��s���܂��B
                
                Begin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
  Arguments:    pCell:      [IN]  �Z���f�[�^
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetRndCoreCellCloneSource3D( const NNSG2dCellData* pCell )
{
    
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    NNS_G2D_ASSERTMSG( pTheInstance_->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN3D,
                          "This method can work only for the 3D Graphics Engine." );
    NNS_G2D_NULL_ASSERT( pCell );         
    
    {   
        GXOamAttr*            pOam = &pTheInstance_->currentOam;
        // �擪��OBJ���\�Ƃ���...
        NNS_G2dCopyCellAsOamAttr( pCell, 0, pOam );
        // �p�����[�^�L���b�V���Ƃ��Đݒ肵�܂�
        // ���̌�̕`��ɂ��āA
        // �e�N�X�`���p�����[�^�͂��̒l���Q�Ƃ���܂��B
        
        
        //
        // �����_���̃t���b�v�ݒ��OAM�A�g���r���[�g�ɔ��f����
        //        
        OverwriteOamFlipFlag_( pOam );
               
               
        NNS_G2dSetOamSoftEmuSpriteParamCache
        (
           pOam, 
           &pTheInstance_->pImgProxy->attr, 
           pTheInstance_->baseTexAddr3D, 
           pTheInstance_->basePltAddr3D 
        );    
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dRndCoreDrawCellClone3D

  Description:  ����̉摜�f�[�^������OBJ����\�������Z�����\�t�g�E�F�A�X�v���C�g��
                �g�p���č����ɕ`�悵�܂��B
                
                ����̉摜�f�[�^������OBJ�Ƃ̓\�t�g�E�F�A�X�v���C�g�i�e�N�X�`���t�l�p�|���S���j
                �Ƃ��ĕ`�悷��ۂ�UV�l������ɂȂ�OBJ�̂��Ƃł��B
                
                ����̉摜�f�[�^������OBJ�ł���΁A�Z������OBJ�͒P���ł������ł����܂��܂���B
                �{�֐��́A���O�Ɍv�Z���ꂽ�AUV�p�����[�^�L���b�V���𗘗p���ĕ`��������Ȃ��܂��B
                �ʏ�̃\�t�g�E�F�A�X�v���C�g�`��ŕK�v�ƂȂ�AUV�l�̌v�Z�������Ȃ����Ƃ��\�ɂȂ邽��
                �A�����ɓ��삵�܂��B
                UV�p�����[�^�L���b�V����ݒ肷�邽�߂ɁA
                ���s�O��NNS_G2dSetRndCoreCellCloneSource3D()���Ăяo���āA�p�����[�^�̏�����
                ���ς܂��Ă����K�v������܂��B
                
                ���ӁF�t���b�v���ꂽ�X�v���C�g��UV�p�����[�^�͒ʏ��UV�p�����[�^�Ƃ͈قȂ�܂��B
                ���������ē���̉摜�f�[�^������OBJ�Ƃ݂͂Ȃ��Ȃ����ƂɂȂ�܂��B
                
                
                �{�֐��̓A�t�B���ϊ����T�|�[�g���܂��B�e��R�[���o�b�N�Ăяo�����T�|�[�g���܂���B
                
                3D �T�[�t�F�X��p�̏����ł��B���݂̃T�[�t�F�X��ނ��A
                NNS_G2D_SURFACETYPE_MAIN3D�ȊO�̏ꍇ�̓A�T�[�g�Ɏ��s���܂��B
                
                Begin-EndRendering�u���b�N�̓��ŌĂяo���K�v������܂��B
                
  Arguments:    pCell:      [IN]  �Z���f�[�^
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dRndCoreDrawCellClone3D( const NNSG2dCellData* pCell )
{
    u16 i = 0;
    
    GXOamAttr*            pOam = &pTheInstance_->currentOam;
    
    NNS_G2D_RND_BETWEEN_BEGINEND_ASSERT();
    NNS_G2D_ASSERTMSG( pTheInstance_->pCurrentTargetSurface->type == NNS_G2D_SURFACETYPE_MAIN3D,
                          "This method can work only for the 3D Graphics Engine." );
    NNS_G2D_NULL_ASSERT( pTheInstance_->pCurrentTargetSurface );
    NNS_G2D_NULL_ASSERT( pCell );
    
    if( pTheInstance_->flipFlag != NNS_G2D_RENDERERFLIP_NONE )
    {
        //
        // �t���b�v�`��
        //
        for( i = 0; i < pCell->numOAMAttrs; i++ )
        {
           NNS_G2dCopyCellAsOamAttr( pCell, i, pOam );
           
           // �s��ݒ�
           G3_LoadMtx43( &pTheInstance_->mtxFor3DGE );
    
           // �`��
           {               
               const GXOamShape shape = NNS_G2dGetOAMSize( pOam );
                
               //
               // OBJ �� �`��ʒu���擾����
               //
               const s16 posX = (s16)GetFlipedOBJPosX_( pOam, shape );
               const s16 posY = (s16)GetFlipedOBJPosY_( pOam, shape );
               const s16 posZ = -1;      
                
               //
               // �����_���̃t���b�v�ݒ��OAM�A�g���r���[�g�ɔ��f����
               //        
               OverwriteOamFlipFlag_( pOam );
               
                
               NNS_G2dDrawOneOam3DDirectUsingParamCacheFast( posX, posY, posZ, pOam );    
           }
        }
    }else{
        //
        // �t���b�v�����`��
        //
        for( i = 0; i < pCell->numOAMAttrs; i++ )
        {
           NNS_G2dCopyCellAsOamAttr( pCell, i, pOam );
           
           // �s��ݒ�
           G3_LoadMtx43( &pTheInstance_->mtxFor3DGE );
    
           {
               const s16 posX = (s16)NNS_G2dRepeatXinCellSpace( (s16)pOam->x );
               const s16 posY = (s16)NNS_G2dRepeatYinCellSpace( (s16)pOam->y );
               const s16 posZ = -1;
                               
               NNS_G2dDrawOneOam3DDirectUsingParamCacheFast( posX, posY, posZ, pOam );
           }
        }
    }
}
