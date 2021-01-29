/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_OAMEx.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.18 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_OAMEX_H_
#define NNS_G2D_OAMEX_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>

#ifdef __cplusplus
extern "C" {
#endif


//
// �������s�����֐��̕ʖ�
// �݊����ێ��̂��ߕʖ��Ƃ��ĈȑO�̊֐���錾���܂��B
// 
#define NNS_G2dEntryNewOamEx                   NNS_G2dEntryOamManExOam
#define NNS_G2dEntryNewOamWithAffineIdxEx      NNS_G2dEntryOamManExOamWithAffineIdx
#define NNS_G2dEntryNewAffineParamsEx          NNS_G2dEntryOamManExAffine
#define NNS_G2dApplyToBaseModuleEx             NNS_G2dApplyOamManExToBaseModule

#define NNS_G2dResetOamInstanceEx              NNS_G2dResetOamManExBuffer
#define NNS_G2dGetNewOamInstanceEx             NNS_G2dGetOamManExInstance

#define NNS_G2dSetOamEntryFunctionsEx          NNS_G2dSetOamManExEntryFunctions



//
// HW OAM Affine �e�[�u���̃C���f�b�N�X������������Ă��Ȃ�
// NNSG2dAffineParamProxy.affineHWIndex �� �����l�ł��B
//
#define NNS_G2D_OAMEX_HW_ID_NOT_INIT       0xFFFF


//------------------------------------------------------------------------------
//
// �`��o�^���̎��
//
typedef enum NNSG2dOamExDrawOrder
{
    NNSG2D_OAMEX_DRAWORDER_BACKWARD = 0x0, // �o�^�������ԂƋt�̏��ԂɊO�����W���[���ւ̓o�^�������Ȃ���
    NNSG2D_OAMEX_DRAWORDER_FORWARD  = 0x1  // �o�^�������ԂƓ����̏��ԂɊO�����W���[���ւ̓o�^�������Ȃ���

}NNSG2dOamExDrawOrder;


//------------------------------------------------------------------------------
// �e��o�^�֐�
typedef u16     (*NNSG2dGetOamCpacityFuncPtr)(); 
typedef u16     (*NNSG2dGetOamAffineCpacityFuncPtr)(); 
typedef BOOL    (*NNSG2dEntryNewOamFuncPtr)( const GXOamAttr* pOam, u16 index ); 
typedef u16     (*NNSG2dEntryNewOamAffineFuncPtr)( const MtxFx22* mtx, u16 index ); 

typedef struct NNSG2dOamExEntryFunctions
{
    NNSG2dGetOamCpacityFuncPtr        getOamCapacity;
    NNSG2dGetOamCpacityFuncPtr        getAffineCapacity;
    NNSG2dEntryNewOamFuncPtr          entryNewOam;
    NNSG2dEntryNewOamAffineFuncPtr    entryNewAffine;
    
}NNSG2dOamExEntryFunctions;



//------------------------------------------------------------------------------
// �����Ŏg�p����
// OAM�A�g���r���[�g�Ǘ��\��
typedef struct NNSG2dOamChunk
{
    
    GXOamAttr               oam;             // OAM�A�g���r���[�g
    u16                     affineProxyIdx;  // Oam �� �Q�Ƃ��� NNSG2dAffineParamProxy Index �l
    u16                     pad16_;          // �p�f�B���O
    struct NNSG2dOamChunk*  pNext;           // ���X�g�\�����\�z���邽�߂̃����N
    
}NNSG2dOamChunk; 

//------------------------------------------------------------------------------
// NNSG2dOamChunk ���܂Ƃ߂� �T�O
// �{�\���̔z�񂪕`��D��x���ɂȂ��� �I�[�_�����O�e�[�u���ƂȂ�܂�
//
// �����͂Q�� NNSG2dOamChunk ���X�g�������܂��B
// �ЂƂ� �ʏ�� OBJ �p �ЂƂ� affine�ϊ����ꂽ OBJ �p �ł�
//
typedef struct NNSG2dOamChunkList
{
    u16                 numChunks;         // pChunks ���X�g��NNSG2dOamChunk����
    u16                 numLastFrameDrawn; // �O�t���[���ŕ`��o�^���ꂽ NNSG2dOamChunk �ԍ�
    
    u16                 numDrawn;          // �`�悷�ׂ��`�����N��
    u16                 bDrawn;            // �`��o�^���ׂ�ChunkList���ǂ����H
    
    NNSG2dOamChunk*     pChunks;           // NNSG2dOamChunk ���X�g
    NNSG2dOamChunk*     pAffinedChunks;    // affine�ϊ����ꂽ NNSG2dOamChunk ���X�g
    
    NNSG2dOamChunk*     pLastChunk;        // NNSG2dOamChunk ���X�g�̏I�[
    NNSG2dOamChunk*     pLastAffinedChunk; // affine�ϊ����ꂽ NNSG2dOamChunk ���X�g�̏I�[
    
}NNSG2dOamChunkList;


/*---------------------------------------------------------------------------*
  Name:         NNSG2dAffineParamProxy

  Description:  OAMEx �����o�b�t�@ �� Affine �p�����[�^������킷�f�[�^�\��
                
                NNSG2dAffineParamProxy.affineHWIndex �� �����l�� NNS_G2D_OAMEX_HW_ID_NOT_INIT �ł��B
                NNSG2dAffineParamProxy.affineHWIndex �� CopyAffineToOamManager_() �� HW�Ƀ��[�h���ꂽ���             
                ���ۂ�HW�Ƀ��[�h���ꂽ�C���f�b�N�X�ɏ㏑������܂�
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dAffineParamProxy
{
    MtxFx22             mtxAffine;          // Affine Paramaters
    u16                 affineHWIndex;      // HW �Ƀ��[�h���ꂽ����AffineIndex�l
    u16                 pad16_;             // �p�f�B���O
}
NNSG2dAffineParamProxy;

//------------------------------------------------------------------------------
// �g���}�l�[�W���Q����
typedef struct NNSG2dOamManagerInstanceEx
{
    
    NNSG2dOamChunkList*             pOamOrderingTbl;     // �`��D��x�e�[�u���擪�ւ̃|�C���^
    
    u16                             numPooledOam;        // OamChunks ����
    u16                             numUsedOam;          // �g�p�ς�OamChunks ��
    NNSG2dOamChunk*                 pPoolOamChunks;      // ���L OamChunks �z��ւ̃|�C���^
    
    u16                             lengthOfOrderingTbl; // �`��D��x�e�[�u���̒���:( 0 - 255 �Fu8 �l�ŏ���������܂��j
    u16                             lengthAffineBuffer;  // NNSG2dAffineParamProxy ����
    u16                             numAffineBufferUsed; // �g�p�ς�AffineParamProxy ��
    u16                             lastFrameAffineIdx;  // �O�t���[�� �o�^ NNSG2dAffineParamProxy �ԍ�
    NNSG2dAffineParamProxy*         pAffineBuffer;       // ���L AffineParamProxy �z��
    
    NNSG2dOamExEntryFunctions       oamEntryFuncs;       // �o�^�֐�
    
    
    
    u16                             lastRenderedOrderingTblIdx;
    u16                             lastRenderedChunkIdx;
    NNSG2dOamExDrawOrder            drawOrderType;       // �`�揇�^�C�v
                                                         // (�ߋ��̃o�[�W�����Ƃ̌݊����ێ��̂��߂ɁA�f�t�H���g�͋t��)

}NNSG2dOamManagerInstanceEx;




//------------------------------------------------------------------------------
void NNS_G2dResetOamManExBuffer  ( NNSG2dOamManagerInstanceEx* pOam );
//------------------------------------------------------------------------------
BOOL NNS_G2dGetOamManExInstance( 
    NNSG2dOamManagerInstanceEx*    pOam, 
    NNSG2dOamChunkList*             pOamOrderingTbl, 
    u8                              lengthOfOrderingTbl,
    u16                             numPooledOam,
    NNSG2dOamChunk*                 pPooledOam,
    u16                             lengthAffineBuffer,
    NNSG2dAffineParamProxy*         pAffineBuffer );
//------------------------------------------------------------------------------
BOOL NNS_G2dEntryOamManExOam
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    const GXOamAttr*                pOam, 
    u8                              priority 
);
//------------------------------------------------------------------------------
BOOL NNS_G2dEntryOamManExOamWithAffineIdx
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    const GXOamAttr*                pOam, 
    u8                              priority, 
    u16                             affineIdx 
);
//------------------------------------------------------------------------------
u16  NNS_G2dEntryOamManExAffine
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    const MtxFx22*                  mtx 
);
//------------------------------------------------------------------------------
void NNS_G2dApplyOamManExToBaseModule 
( 
    NNSG2dOamManagerInstanceEx* pMan 
);

//------------------------------------------------------------------------------
void NNSG2d_SetOamManExDrawOrderType
( 
    NNSG2dOamManagerInstanceEx*    pOam, 
    NNSG2dOamExDrawOrder           drawOrderType
);

//------------------------------------------------------------------------------
void NNS_G2dSetOamManExEntryFunctions
( 
    NNSG2dOamManagerInstanceEx*        pMan, 
    const NNSG2dOamExEntryFunctions*   pSrc 
);





#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_OAMEX_H_