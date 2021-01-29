/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_Renderer.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.34 $
 *---------------------------------------------------------------------------*/
//
// The abstraction layer of rendering HWs.
// Rendering HWs include 2D Graphic Engine A - B, and 3D Graphic Engine.
// 
#ifndef NNS_G2D_RENDERER_H_
#define NNS_G2D_RENDERER_H_


#include <nitro.h>

#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Entity.h>
#include <nnsys/g2d/g2d_Node.h>
#include <nnsys/g2d/g2d_MultiCellAnimation.h>
#include <nnsys/g2d/g2d_PaletteTable.h>
#include <nnsys/g2d/fmt/g2d_Character_data.h>
#include <nnsys/g2d/g2d_RendererCore.h>


#ifdef __cplusplus
extern "C" {
#endif

//
// �������s�����֐��̕ʖ�
// �݊����ێ��̂��ߕʖ��Ƃ��ĈȑO�̊֐���錾���܂��B
// 
#define NNS_G2dAddTargetSurface             NNS_G2dAddRendererTargetSurface
#define NNS_G2dSetCurrentImageProxy         NNS_G2dSetRendererImageProxy






/*---------------------------------------------------------------------------*
  Name:         NNSG2dRndCellCullingFunction
  
  Description:   �����J�����O����֐��|�C���^
  
  Arguments:    pCell   : �Z��
                pMtx    : �ϊ��s��
                pTrans  : ���s�ړ��l
               
  Returns:      ���Ȃ�TRUE
  
 *---------------------------------------------------------------------------*/ 
typedef BOOL(*NNSG2dRndCellCullingFunction)
(
    const NNSG2dCellData*   pCell,
    const MtxFx32*          pMtx,
    const NNSG2dViewRect*   pViewRect
);

struct NNSG2dRendererInstance;
struct NNSG2dRenderSurface;
/*---------------------------------------------------------------------------*
  Name:         NNSG2dRndDrawCellCallBack
  
  Description:   �`�掞�ɌĂяo�����R�[���o�b�N
                �i�`��O�ƕ`���ɌĂяo�����j
  
  Arguments:    pRend   : �����_���ւ̃|�C���^
                pSurface: �����_�T�[�t�F�X�ւ̃|�C���^
                pCell   : �Z��
                pMtx    : �ϊ��s��
               
  Returns:      ����
  
 *---------------------------------------------------------------------------*/ 
typedef void(*NNSG2dRndDrawCellCallBack)
(
    struct NNSG2dRendererInstance*         pRend,
    struct NNSG2dRenderSurface*            pSurface,
    const NNSG2dCellData*                  pCell,
    const MtxFx32*                         pMtx
);


/*---------------------------------------------------------------------------*
  Name:         NNSG2dRndDrawOamCallBack
  
  Description:   ����Oam�A�g���r���[�g�`�掞�ɌĂяo�����R�[���o�b�N
                �i�`��O�ƕ`���ɌĂяo�����j
  
  Arguments:    pRend   : �����_���ւ̃|�C���^
                pSurface: �����_�T�[�t�F�X�ւ̃|�C���^
                pCell   : �Z��
                pMtx    : �ϊ��s��
               
  Returns:      ����
  
 *---------------------------------------------------------------------------*/ 
typedef void(*NNSG2dRndDrawOamCallBack)
(
    struct NNSG2dRendererInstance*         pRend,
    struct NNSG2dRenderSurface*            pSurface,
    const NNSG2dCellData*                  pCell,
    u16                                    oamIdx,
    const MtxFx32*                         pMtx
);



/*---------------------------------------------------------------------------*
  Name:         NNSG2dRenderSurface

  Description:  �����_�����O�Ώۂ�����킷�\��
                
                Oam��o�^����֐��AOamAffineParamater��o�^����֐��̃|�C���^�������A
                ���ۂɕ`�施�߂𔭍s����ӔC�������܂��B
                ( "�`�施�߂𔭍s����" ���� �� �ʊT�O�Ƃ��ĕ������邩������Ȃ� )
               
                ���̎��̂ւ̃|�C���^���������X�g�\�����\�z���܂��B
                
 *---------------------------------------------------------------------------*/    
typedef struct NNSG2dRenderSurface
{
    //
    // NNSG2dRndCoreSurface 
    // �����_�����O�ΏۃT�[�t�F�X�̊�{�I�ȓ�����ێ����܂��B
    //
    union
    {
        NNSG2dRndCoreSurface              coreSurface;
        struct
        {
            NNSG2dViewRect                    viewRect;                 
            BOOL                              bActive;                  
            NNSG2dSurfaceType                 type;                     
            
            NNSG2dRndCoreDrawCellCallBack     pBeforeCoreDrawCellBack;  
            NNSG2dRndCoreDrawCellCallBack     pAfterCoreDrawCellBack;
    
            NNSG2dRndCoreDrawOamCallBack      pBeforeCoreDrawOamBack;
            NNSG2dRndCoreDrawOamCallBack      pAfterCoreDrawOamBack;
        };
    };
    
    NNSG2dOamRegisterFunction         pFuncOamRegister;         // Oam �o�^�֐�
    NNSG2dAffineRegisterFunction      pFuncOamAffineRegister;   // Affine �o�^�֐�
    
    void*                             pNextSurface;             // ���̃T�[�t�F�X
    
    
    NNSG2dRndCellCullingFunction      pFuncVisibilityCulling;   // ���J�����O�֐�
    
    NNSG2dRndDrawCellCallBack         pBeforeDrawCellBackFunc;
    NNSG2dRndDrawCellCallBack         pAfterDrawCellBackFunc;
    
    NNSG2dRndDrawOamCallBack          pBeforeDrawOamBackFunc;
    NNSG2dRndDrawOamCallBack          pAfterDrawOamBackFunc;
    
              
}
NNSG2dRenderSurface;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRendererOptimizeHint

  Description:  Renderer �`��œK���Ɋւ�����
                
 *---------------------------------------------------------------------------*/
typedef enum NNSG2dRendererOptimizeHint
{
    NNS_G2D_RDR_OPZHINT_NONE              = 0x0,
    NNS_G2D_RDR_OPZHINT_NOT_SR            = 0x1, // SR(Scale Rotate) �ϊ����g�p���Ȃ�
    NNS_G2D_RDR_OPZHINT_LOCK_PARAMS       = 0x2  // �p�����[�^�����b�N����B
                                                 // 
                                                 // �T�[�t�F�X���ꖇ�̏ꍇ�̂ݎw��\�B
                                                 // �摜�v���N�V�A�T�[�t�F�X��ނ� 
                                                 // �����_�� Begin-End Renderring �u���b�N���ŕύX�s�\�ƂȂ�܂��B
                                                 // ���̌��ʁA�����_���R�A�� Begin-End Renderring �̎��s�� 
                                                 // �����_�� Begin-End Renderring �Ɠ����^�C�~���O�Ŏ��s���邱�Ƃ��\�ƂȂ�܂��B
                                                 //
                                                 // �t���O���w�肵�Ȃ��ꍇ�A�����_���R�A�� Begin-End Renderring �̎��s�́A
                                                 // �Z���̕`�斈�ɍs���܂��B

}NNSG2dRendererOptimizeHint;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRendererOverwriteParam

  Description:  OBJ�A�g���r���[�g�㏑������

 *---------------------------------------------------------------------------*/
typedef enum NNSG2dRendererOverwriteParam
{
    NNS_G2D_RND_OVERWRITE_NONE          = 0x0, // OBJ�A�g���r���[�g�㏑�����s��Ȃ�
    NNS_G2D_RND_OVERWRITE_PRIORITY      = 0x1, // �`��D��x
    NNS_G2D_RND_OVERWRITE_PLTTNO        = 0x2, // �p���b�g�ԍ�
    NNS_G2D_RND_OVERWRITE_MOSAIC        = 0x4, // ���U�C�NON OFF
    NNS_G2D_RND_OVERWRITE_OBJMODE       = 0x8, // �I�u�W�F���[�h
    NNS_G2D_RND_OVERWRITE_PLTTNO_OFFS   = 0x10, // �p���b�g�ԍ�(�I�t�Z�b�g���Z)
    NNS_G2D_RND_OVERWRITE_MAX
    
}NNSG2dRendererOverwriteParam;
// 
// ���̂ق��̌��
// OBJMode
// ���U�C�NONOFF

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRendererInstance

  Description:  Renderer ����
                NNSG2dRenderSurface�̃��X�g��ێ����܂�

 *---------------------------------------------------------------------------*/
typedef struct NNSG2dRendererInstance
{
    NNSG2dRndCoreInstance             rendererCore;        // �����_���R�A
        
    
    NNSG2dRenderSurface*              pTargetSurfaceList;  // �`��ΏۃT�[�t�F�X���X�g
    NNSG2dRenderSurface*              pCurrentSurface;     // ���ݕ`�揈�����̃T�[�t�F�X
                                                           // ( �����_���R�A���`�揈�����ȊO�� NULL �ɐݒ肳��� )
    
    const NNSG2dPaletteSwapTable*     pPaletteSwapTbl;     // �J���[�p���b�g�ύX�e�[�u��(NULL����)                                                       
    u32                               opzHint;             // �`��œK���̂��߂̏��
                                                           // NNSG2dRendererOptimizeHint
                                                           
    fx32                              spriteZoffsetStep;   // �\�t�g�E�F�A�X�v���C�g����Z�I�t�Z�b�g�̗�
    
    u32                               overwriteEnableFlag; // NNSG2dRendererOverwriteParam
    u16                               overwritePriority;   // �`��D��x
    u16                               overwritePlttNo;     // �p���b�g�ԍ�
    GXOamMode                         overwriteObjMode;                      
    BOOL                              overwriteMosaicFlag;
    
    u16                               overwritePlttNoOffset;// �p���b�g�ԍ�(�I�t�Z�b�g���Z)
    u16                               pad16_;
}
NNSG2dRendererInstance;






//------------------------------------------------------------------------------
// �������֘A
void NNS_G2dInitRenderer( NNSG2dRendererInstance* pRend );
void NNS_G2dAddRendererTargetSurface
( 
    NNSG2dRendererInstance* pRend, 
    NNSG2dRenderSurface* pNew 
);
void NNS_G2dInitRenderSurface( NNSG2dRenderSurface* pSurface );
void NNS_G2dSetRendererFlipMode
( 
    NNSG2dRendererInstance* pRend, 
    BOOL bFlipH, 
    BOOL bFlipV 
);

//------------------------------------------------------------------------------
// �`��֘A


void NNS_G2dBeginRendering   ( NNSG2dRendererInstance* pRendererInstance );
void NNS_G2dBeginRenderingEx ( NNSG2dRendererInstance* pRendererInstance, u32 opzHint );

void NNS_G2dDrawCell           ( const NNSG2dCellData* pCell );
void NNS_G2dDrawCellAnimation  ( const NNSG2dCellAnimation* pCellAnim );
void NNS_G2dDrawMultiCell      ( const NNSG2dMultiCellInstance* pMC );
void NNS_G2dDrawMultiCellAnimation( const NNSG2dMultiCellAnimation* pMCAnim );

void NNS_G2dDrawEntity         ( NNSG2dEntity* pEntity );

void NNS_G2dEndRendering   ( );

//------------------------------------------------------------------------------
// �s�񑀍�
void NNS_G2dPushMtx();
void NNS_G2dPopMtx();
void NNS_G2dTranslate(fx32 x, fx32 y, fx32 z );
void NNS_G2dSetTrans(fx32 x, fx32 y, fx32 z );
void NNS_G2dScale(fx32 x, fx32 y, fx32 z );
void NNS_G2dRotZ( fx32 sin, fx32 cos );





//------------------------------------------------------------------------------
// �p���b�g�ύX�֘A
void NNS_G2dSetRendererPaletteTbl
( 
    NNSG2dRendererInstance*         pRend, 
    const NNSG2dPaletteSwapTable*   pTbl 
);
const NNSG2dPaletteSwapTable* NNS_G2dGetRendererPaletteTbl
( 
    NNSG2dRendererInstance* pRend 
);
void NNS_G2dResetRendererPaletteTbl
( 
    NNSG2dRendererInstance* pRend 
);

//------------------------------------------------------------------------------
// �摜���֘A
void NNS_G2dSetRendererImageProxy
( 
    NNSG2dRendererInstance*             pRend, 
    const NNSG2dImageProxy*             pImgProxy, 
    const NNSG2dImagePaletteProxy*      pPltProxy
);





//------------------------------------------------------------------------------
// inline 
//------------------------------------------------------------------------------
NNS_G2D_INLINE void NNS_G2dSetRendererSpriteZoffset
( 
    NNSG2dRendererInstance* pRend, 
    fx32                    spriteZoffset 
)
{
    pRend->spriteZoffsetStep = spriteZoffset;
}



//------------------------------------------------------------------------------
NNS_G2D_INLINE void NNS_G2dSetRendererAffineOverwriteMode
( 
    NNSG2dRendererInstance*                 pRend, 
    NNSG2dRendererAffineTypeOverwiteMode    affineMode
)
{
    NNS_G2D_NULL_ASSERT( pRend );
    pRend->rendererCore.affineOverwriteMode = affineMode;
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE NNSG2dRendererAffineTypeOverwiteMode 
NNS_G2dGetRendererAffineOverwriteMode
( 
    const NNSG2dRendererInstance* pRend
)
{
    NNS_G2D_NULL_ASSERT( pRend );
    return pRend->rendererCore.affineOverwriteMode;
}

//------------------------------------------------------------------------------
// Core �� �ړ�
NNS_G2D_INLINE void NNS_G2dSetRendererSurfaceActiveFlag
( 
    NNSG2dRenderSurface*    pSurface, 
    BOOL                    bActive
)
{
    NNS_G2D_NULL_ASSERT( pSurface );
    pSurface->coreSurface.bActive = bActive;
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE BOOL NNS_G2dGetRendererSurfaceActiveFlag
( 
    const NNSG2dRenderSurface*    pSurface
)
{
    NNS_G2D_NULL_ASSERT( pSurface );
    return pSurface->coreSurface.bActive;
}

//------------------------------------------------------------------------------
// �A�N�Z�T �`��D��x
NNS_G2D_INLINE void NNS_G2dSetRendererOverwritePriority
(
    NNSG2dRendererInstance*     pRend,
    u16                         Priority
)
{
    pRend->overwritePriority = Priority;
}

NNS_G2D_INLINE u16 NNS_G2dGetRendererOverwritePriority
(
    const NNSG2dRendererInstance* pRend
)
{
    return pRend->overwritePriority;
}
//------------------------------------------------------------------------------
// �A�N�Z�T
// �p���b�g�ԍ�
NNS_G2D_INLINE void NNS_G2dSetRendererOverwritePlttNo
(
    NNSG2dRendererInstance*     pRend,
    u16                         plttNo
)
{
    pRend->overwritePlttNo = plttNo;
}

NNS_G2D_INLINE u16 NNS_G2dGetRendererOverwritePlttNo
(
    const NNSG2dRendererInstance* pRend    
)
{
    return pRend->overwritePlttNo;
}

//------------------------------------------------------------------------------
// �J�����ł�
NNS_G2D_INLINE void NNS_G2dSetRendererOverwritePlttNoOffset
(
    NNSG2dRendererInstance*     pRend,
    u16                         plttNoOffset
)
{
    pRend->overwritePlttNoOffset = plttNoOffset;
}

//------------------------------------------------------------------------------
// �J�����ł�
NNS_G2D_INLINE u16 NNS_G2dGetRendererOverwritePlttNoOffset
(
    const NNSG2dRendererInstance* pRend    
)
{
    return pRend->overwritePlttNoOffset;
}


NNS_G2D_INLINE void NNS_G2dSetRendererOverwriteMosaicFlag
(
    NNSG2dRendererInstance*     pRend,
    BOOL                        overwriteMosaicFlag
)
{
    pRend->overwriteMosaicFlag = overwriteMosaicFlag;
}

NNS_G2D_INLINE BOOL NNS_G2dGetRendererOverwriteMosaicFlag
(
    const NNSG2dRendererInstance* pRend    
)
{
    return pRend->overwriteMosaicFlag;
}

NNS_G2D_INLINE void NNS_G2dSetRendererOverwriteOBJMode
(
    NNSG2dRendererInstance*     pRend,
    GXOamMode                   overwriteObjMode
)
{
    pRend->overwriteObjMode = overwriteObjMode;
}

NNS_G2D_INLINE GXOamMode NNS_G2dGetRendererOverwriteOBJMode
(
    const NNSG2dRendererInstance* pRend    
)
{
    return pRend->overwriteObjMode;
}



//------------------------------------------------------------------------------
// OAM�㏑���t���O����
NNS_G2D_INLINE void 
NNS_G2dSetRendererOverwriteEnable
( 
    NNSG2dRendererInstance* pRnd, 
    NNSG2dRendererOverwriteParam flag 
)
{
    pRnd->overwriteEnableFlag |= flag;
}

NNS_G2D_INLINE void 
NNS_G2dSetRendererOverwriteDisable
( 
    NNSG2dRendererInstance* pRnd, 
    NNSG2dRendererOverwriteParam flag 
)
{
    pRnd->overwriteEnableFlag &= ~flag;
}
 
NNS_G2D_INLINE BOOL 
NNS_G2dIsRendererOverwriteEnable
( 
    const NNSG2dRendererInstance* pRnd, 
    NNSG2dRendererOverwriteParam flag 
)
{
    return (BOOL)( pRnd->overwriteEnableFlag & flag );
}


//------------------------------------------------------------------------------
// �`��œK���֘A
/*---------------------------------------------------------------------------*
  Name:         NNS_G2dOptimizeRenderer

  Description:  �����_���̍œK�����s���܂��B
                �{API�͔p�~�ɂȂ�܂����B
                �{�֐����Ăяo���ƁA�x�����b�Z�[�W���o�͂���܂��B
                
                �{�֐��̑���ɁANNS_G2dBeginRenderingEx()���p�ӂ���܂����B
                
                
  Arguments:    pRend:      �����_������
                
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dOptimizeRenderer( NNSG2dRendererInstance* pRend )
{
#pragma unused( pRend )
    OS_Warning("Sorry, NNS_G2dOptimizeRenderer() is obsolete.Please avoid calling this func.");
}



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif // NNS_G2D_RENDERER_H_
