/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_RendererCore.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.17 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_RENDERERCORE_H_
#define NNS_G2D_RENDERERCORE_H_


#include <nitro.h>

#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/fmt/g2d_Vec_data.h>

#include <nnsys/g2d/fmt/g2d_Oam_data.h>
#include <nnsys/g2d/fmt/g2d_Cell_data.h>



#ifdef __cplusplus
extern "C" {
#endif

#define MtxCache_NOT_AVAILABLE                  0xFFFF
#define MtxCache_NOT_AVAILABLE_ForMemFill       0xFFFFFFFF // �������t�B���p
#define NNS_G2D_NUMBER_OF_2DGRAPHICS_ENGINE     2
#define NNS_G2D_RNDCORE_INTERNAL_OAMBUFFER_SIZE 128
#define NNS_G2D_OAMFLIP_PATTERN_NUM             0x04 // (OAM_FLIP_NONE, OAM_FLIP_H, OAM_FLIP_V, OAM_FLIP_HV )
    

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRendererAffineTypeOverwiteMode

  Description:  OAM�A�g���r���[�g�̃A�t�B���ݒ蕔���̏㏑������
 *---------------------------------------------------------------------------*/ 
typedef enum NNSG2dRendererAffineTypeOverwiteMode
{
    NNS_G2D_RND_AFFINE_OVERWRITE_NONE,  // �㏑�����Ȃ�
    NNS_G2D_RND_AFFINE_OVERWRITE_NORMAL,// �ʏ�̃A�t�B���ϊ������ɐݒ�
    NNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE // �{�p�A�t�B���ϊ������ɐݒ�
    
}NNSG2dRendererAffineTypeOverwiteMode;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dSurfaceType

  Description:  �����_�����O�Ώ�
                
 *---------------------------------------------------------------------------*/
// enum �l �� �ύX���Ă͂Ȃ�Ȃ� 
// ��������Ɋ֘A���܂��B
typedef enum NNSG2dSurfaceType
{
    NNS_G2D_SURFACETYPE_MAIN3D = 0x00,  // 3D �O���t�B�b�N�X�G���W��   ���g�p
    NNS_G2D_SURFACETYPE_MAIN2D = 0x01,  // 2D �O���t�B�b�N�X�G���W�� A ���g�p
    NNS_G2D_SURFACETYPE_SUB2D  = 0x02,  // 2D �O���t�B�b�N�X�G���W�� B ���g�p
    NNS_G2D_SURFACETYPE_MAX    = 0x03
    
}NNSG2dSurfaceType;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRendererFlip

  Description:  �����_���A�t���b�v�`����
                
 *---------------------------------------------------------------------------*/
typedef enum NNSG2dRendererFlip
{
    NNS_G2D_RENDERERFLIP_NONE   = 0x00,
    NNS_G2D_RENDERERFLIP_H      = 0x01,
    NNS_G2D_RENDERERFLIP_V      = 0x02
    
}NNSG2dRendererFlip;


//------------------------------------------------------------------------------
// ���ۂ� 2D Graphics Enigine �ւ� Affine �p�����[�^�o�^�� �L���V������������ʂ����܂��B
// �d������ �o�^�� ���s���ꂸ�ߋ��̓o�^���ʂ��Ԃ���܂��B
// 
// ����ɂ���āA���� NNSG2dRndCore2DMtxCache Index �� �w�肵���ꍇ 
// ���� Affine �p�����[�^ ���Q�Ƃ��邱�ƂƂȂ�܂��B
//
// affineIndex �� �K��l�� Affine �p�����[�^�o�^ ���Ȃ���Ă��Ȃ��Ƃ����Ӗ��� 
// MtxCache_NOT_AVAILABLE �������܂��B
// 
typedef struct NNSG2dRndCore2DMtxCache
{
    MtxFx22         m22;
    u16             affineIndex[NNS_G2D_OAMFLIP_PATTERN_NUM][NNS_G2D_NUMBER_OF_2DGRAPHICS_ENGINE];
}NNSG2dRndCore2DMtxCache;


/*---------------------------------------------------------------------------*
  Name:         NNSG2dViewRect

  Description:  ���̈������킷��`
 *---------------------------------------------------------------------------*/ 
typedef struct NNSG2dViewRect
{
    NNSG2dFVec2            posTopLeft;      // ���̈� ����ʒu
    NNSG2dFVec2            sizeView;        // �r���[�̃T�C�Y
    
}NNSG2dViewRect;


 /*---------------------------------------------------------------------------*
  Name:         NNSG2dOamRegisterFunction

  Description:  Oam �o�^�Ɏg�p�����֐��|�C���^
                
  Arguments:    pOam :                  �V�����o�^���� Oam�A�g���r���[�g
                affineIndex :           affine �C���f�b�N�X
                                        ( 32 �ȏ�̏ꍇ������܂� )
                                        (���g�p�̏ꍇ��
                                        NNS_G2D_OAM_AFFINE_IDX_NONE���w�肵�܂�)
                                        
                bDoubleAffine :         �{�paffine���[�h�ŕ`�悷�邩
                                        (���݂͎g�p����Ă��܂���)
                
  Returns:      �o�^�̐���
  
 *---------------------------------------------------------------------------*/
typedef BOOL (*NNSG2dOamRegisterFunction)     
( 
    const GXOamAttr*    pOam, 
    u16                 affineIndex, 
    BOOL                bDoubleAffine 
);

typedef BOOL (*NNSG2dOamBlockRegisterFunction)     
( 
    const GXOamAttr*    pOam, 
    u16                 num 
);

                         
/*---------------------------------------------------------------------------*
  Name:         NNSG2dAffineRegisterFunction
  
  Description:  affine �p�����[�^ �o�^�Ɏg�p�����֐��|�C���^
  
  Arguments:    mtx :                  affine�ϊ��s��
               
  Returns:      �o�^���ꂽaffineIndex( 32 �ȏ�̏ꍇ������܂� )
  
 *---------------------------------------------------------------------------*/ 
typedef u16 (*NNSG2dAffineRegisterFunction)   
( 
    const MtxFx22* mtx 
);


struct NNSG2dRndCoreInstance;
struct NNSG2dRndCoreSurface;

typedef void(*NNSG2dRndCoreDrawCellCallBack)
(
    struct NNSG2dRndCoreInstance*   pRend,
    const NNSG2dCellData*           pCell
);

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRndDrawOamCallBack
  
  Description:  �Z�� ����Oam�A�g���r���[�g�`�掞�ɌĂяo�����R�[���o�b�N
                �i�`��O�ƕ`���ɌĂяo�����j
  
  Arguments:    pRend   : �����_���ւ̃|�C���^
                pSurface: �����_�T�[�t�F�X�ւ̃|�C���^
                pCell   : �Z��
                pMtx    : �ϊ��s��
               
  Returns:      ����
  
 *---------------------------------------------------------------------------*/ 
typedef void(*NNSG2dRndCoreDrawOamCallBack)
(
    struct NNSG2dRndCoreInstance*   pRend,
    const NNSG2dCellData*           pCell,
    u16                             oamIdx
);

/*---------------------------------------------------------------------------*
  Name:         NNSG2dRndCoreOamOverwriteCallBack
  
  Description:  Oam �p�����[�^���㏑�����邽�߂̃R�[���o�b�N
                �{�R�[���o�b�N�� Oam�A�g���r���[�g�`�掞 �R�[���o�b�N
                NNSG2dRndDrawOamCallBack�Ɠ�������A�p�~����܂����B
                
                NNSG2dRndDrawOamCallBack���ŁA
                pRend->currentOam�ɑ΂��ē����̃p�����[�^���㏑���������s���Ă��������B
                
  
  Arguments:    pRend   : �����_���ւ̃|�C���^
                pCell   : �ꎞ�Z���i���������\�j
                oamIdx  : �Z���ԍ�
               
  Returns:      ����
  
 *---------------------------------------------------------------------------*/ 



//------------------------------------------------------------------------------
typedef struct NNSG2dRndCoreSurface
{
    NNSG2dViewRect                    viewRect;                     // ���̈������킷��`
    BOOL                              bActive;                      // �L����
    NNSG2dSurfaceType                 type;                         // �T�[�t�F�X�̎�� 
    
    
    NNSG2dRndCoreDrawCellCallBack         pBeforeDrawCellBackFunc;  // ���J�����O�֐������˂�
    NNSG2dRndCoreDrawCellCallBack         pAfterDrawCellBackFunc;   
    
    NNSG2dRndCoreDrawOamCallBack          pBeforeDrawOamBackFunc;   // Oam �p�����[�^�㏑���p�R�[���o�b�N  
                                                                    // NNSG2dRndCoreOamOverwriteCallBack     
                                                                    // pOamOverwriteFunc �͖{�R�[���o�b�N�Ɠ�������܂����B
    NNSG2dRndCoreDrawOamCallBack          pAfterDrawOamBackFunc;    
}
NNSG2dRndCoreSurface;

struct NNSG2dImageProxy;
struct NNSG2dImagePaletteProxy;

//------------------------------------------------------------------------------
typedef struct NNSG2dRndCoreInstance
{
    NNSG2dRndCoreSurface*                   pCurrentTargetSurface; // �`��ΏۃT�[�t�F�X
    NNSG2dRendererAffineTypeOverwiteMode    affineOverwriteMode;   // �i�K��l�FNNS_G2D_RND_AFFINE_OVERWRITE_DOUBLE�j
    
    //
    // OBJ���Q�Ƃ��� �摜�f�[�^
    //
    const struct NNSG2dImageProxy*          pImgProxy;             // �摜���
    const struct NNSG2dImagePaletteProxy*   pPltProxy;             // �p���b�g���
    u32                                     base2DCharOffset;      // 2D �L�����N�^�x�[�X�I�t�Z�b�g
                                                                   // BeginRendering �Ăяo�����ɌĂ΂�܂��B
    u32                                     baseTexAddr3D;         // 3D �e�N�X�`���x�[�X�I�t�Z�b�g
    u32                                     basePltAddr3D;         // 3D �e�N�X�`���p���b�g�x�[�X�I�t�Z�b�g

    //
    // 2D �֘A�o�^�֐�
    // 2D HW�`����s���ۂɂ͕K���ݒ肳��Ă��邱��
    // pFuncOamRegister �� pFuncOamBlockRegister �� �r���I�Ɏg�p����܂��B
    //
    NNSG2dOamRegisterFunction             pFuncOamRegister;        // Oam �o�^�֐�
    NNSG2dAffineRegisterFunction          pFuncOamAffineRegister;  // Affine �o�^�֐�
    
    
    // NNSG2dRendererFlip 
    // �`��t���b�v���g�p���邩�ǂ���
    // �g�p����ꍇ�̓A�t�B���ϊ������p�ł��Ȃ��̂Œ���
    u32                                     flipFlag;              
    
    // 
    // pCurrentMtxCacheFor2D �� �ݒ肳��Ă���ꍇ�́A2D HW �ɂ��OBJ�`��̍ۂ� 
    // OBJ �ɑ΂��� �A�t�B���ϊ����{����܂��B
    // 2D�n�[�h�E�F�A�̐��񂩂�A�t�B���ϊ� �� �t���b�v�����͔r���I�Ɏg�p���ꖳ����΂Ȃ�Ȃ��̂ŁA
    // flipFlag != NNS_G2D_RENDERERFLIP_NONE �� pCurrentMtxCacheFor2D ���ݒ肳�ꂽ�ꍇ�A
    // �����_���R�A���W���[���͌x�����b�Z�[�W��\�����܂��B
    //
    NNSG2dRndCore2DMtxCache*                pCurrentMtxCacheFor2D;      // �J�����g�s��̃L���b�V��(2D HW �p )
    // 
    // pCurrentMxt �� 3D HW �`��̍ۂɎg�p�����J�����g�s��ł��B
    // �ݒ肳��Ȃ������ꍇ�ɂ́A�P�ʍs�񂪎g�p����܂��B
    //
    const MtxFx32*                          pCurrentMxt;           // �J�����g�s��
    
    
    
    BOOL                                    bDrawEnable;           // �`�悪�L�����H
    
    fx32                                    zFor3DSoftwareSprite;  // Z �l
    
    GXOamAttr                               currentOam;            // ���݂̕`�揈���Ώ�OAM
    
    MtxFx43                                 mtxFor3DGE;            // 3D Graphics Engine �p�̍s��
                                                                   // pCurrentMxt���琶������܂��B
    
    
    
                                                                       
}NNSG2dRndCoreInstance;



//------------------------------------------------------------------------------
// ������
void NNS_G2dInitRndCore( NNSG2dRndCoreInstance* pRnd );
void NNS_G2dInitRndCoreSurface( NNSG2dRndCoreSurface* pSurface );


//------------------------------------------------------------------------------
// �摜�v���L�V�̐ݒ�
struct NNSG2dImageProxy;
struct NNSG2dImagePaletteProxy;

void NNS_G2dSetRndCoreImageProxy
(
    NNSG2dRndCoreInstance*                     pRnd,
    const struct NNSG2dImageProxy*             pImgProxy, 
    const struct NNSG2dImagePaletteProxy*      pPltProxy
);

//------------------------------------------------------------------------------
// 2D HW �o�^�֐�
void NNS_G2dSetRndCoreOamRegisterFunc
(
    NNSG2dRndCoreInstance*          pRnd,
    NNSG2dOamRegisterFunction       pFuncOamRegister,
    NNSG2dAffineRegisterFunction    pFuncOamAffineRegister
);

//------------------------------------------------------------------------------
// �ݒ�
void NNS_G2dSetRndCoreAffineOverwriteMode
(   NNSG2dRndCoreInstance*                  pRnd, 
    NNSG2dRendererAffineTypeOverwiteMode    mode 
);

void NNS_G2dSetRndCore3DSoftSpriteZvalue
( 
    NNSG2dRndCoreInstance* pRnd, fx32 z 
);

void NNS_G2dSetRndCoreSurface
( 
    NNSG2dRndCoreInstance* pRnd, 
    NNSG2dRndCoreSurface* pSurface 
);

//------------------------------------------------------------------------------
// �s��֘A
void NNS_G2dSetRndCoreCurrentMtx3D
( 
    const MtxFx32*          pCurrentMxt
);
void NNS_G2dSetRndCoreCurrentMtx2D
( 
    const MtxFx32*                 pMtx,
    NNSG2dRndCore2DMtxCache*       pCurrentMtxCacheFor2D
);


//------------------------------------------------------------------------------
// Flip�֘A
BOOL NNS_G2dIsRndCoreFlipH( const NNSG2dRndCoreInstance* pRnd );
BOOL NNS_G2dIsRndCoreFlipV( const NNSG2dRndCoreInstance* pRnd );
void NNS_G2dSetRndCoreFlipMode( NNSG2dRndCoreInstance* pRnd, BOOL bFlipH, BOOL bFlipV );



//------------------------------------------------------------------------------
// �`��
void NNS_G2dRndCoreBeginRendering( NNSG2dRndCoreInstance* pRnd );
void NNS_G2dRndCoreEndRendering( void );

void NNS_G2dRndCoreDrawCell( const NNSG2dCellData* pCell );
void NNS_G2dRndCoreDrawCellVramTransfer( const NNSG2dCellData* pCell, u32 cellVramTransferHandle );

void NNS_G2dRndCoreDrawCellFast2D( const NNSG2dCellData* pCell );

void NNS_G2dSetRndCoreCellCloneSource3D ( const NNSG2dCellData* pCell );
void NNS_G2dRndCoreDrawCellClone3D      ( const NNSG2dCellData* pCell );

//------------------------------------------------------------------------------
// inline �֐�
//------------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitRndCore2DMtxCache

  Description:  �s��L���b�V�������������܂��B
                �s��L���b�V�������̃A�t�B���C���f�b�N�X�������������Ă��܂��B
                
                NNS_G2dSetRndCore2DMtxCacheMtxParams() �ɂ���āA
                �s��L���b�V���̓��e��ݒ肷��ۂɓ����ŌĂяo����܂��B
                
  Arguments:    pMtxCache:      �s��L���b�V��
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dInitRndCore2DMtxCache
( 
    NNSG2dRndCore2DMtxCache* pMtxCache 
)
{
    MI_CpuFillFast( pMtxCache->affineIndex, 
                    MtxCache_NOT_AVAILABLE_ForMemFill, 
                    sizeof( pMtxCache->affineIndex ) );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetRndCore2DMtxCacheMtxParams

  Description:  �s��L���b�V���ɍs���ݒ肵�܂��B
                �����ŁANNS_G2dInitRndCore2DMtxCache ���Ă΂�A�A�t�B���C���f�b�N�X��񂪏���������܂��B
                
                
  Arguments:    pMtxCache:      �Z���f�[�^
                pM:             �A�t�B���ϊ��s��
                                (2D�O���t�B�b�N�X�G���W���̃A�t�B���p�����[�^�ɐݒ肷��s��ł��B
                                 �X�P�[���v�f�̈�������ʂ̕ϊ��s��ƈقȂ�_�ɒ��ӂ��Ă��������B)
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dSetRndCore2DMtxCacheMtxParams
( 
    NNSG2dRndCore2DMtxCache* pMtxCache, 
    MtxFx22*          pM 
)
{
    NNS_G2dInitRndCore2DMtxCache( pMtxCache );
    pMtxCache->m22 = *pM;    
}



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_RENDERERCORE_H_