/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_Animation.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.24 $
 *---------------------------------------------------------------------------*/
 
#ifndef NNS_G2D_ANIMATION_H_
#define NNS_G2D_ANIMATION_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/g2d_Data.h>

#ifdef __cplusplus
extern "C" {
#endif


//
// �������s�����֐��̕ʖ�
// �݊����ێ��̂��ߕʖ��Ƃ��ĈȑO�̊֐���錾���܂��B
// 
#define NNS_G2dGetCurrentElement               NNS_G2dGetAnimCtrlCurrentElement
#define NNS_G2dGetNextElement                  NNS_G2dGetAnimCtrlNextElement
#define NNS_G2dGetNormalizedTime               NNS_G2dGetAnimCtrlNormalizedTime
#define NNS_G2dSetCallBackFunctor              NNS_G2dSetAnimCtrlCallBackFunctor
#define NNS_G2dSetCallBackFunctorAtAnimFrame   NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame
#define NNS_G2dGetAnimSpeed                    NNS_G2dGetAnimCtrlSpeed
#define NNS_G2dSetAnimSpeed                    NNS_G2dSetAnimCtrlSpeed
#define NNS_G2dResetAnimationState             NNS_G2dResetAnimCtrlState
#define NNS_G2dInitCallBackFunctor             NNS_G2dInitAnimCallBackFunctor

#define NNS_G2dBindAnimController 						NNS_G2dBindAnimCtrl
#define NNS_G2dGetAnimControllerType 					NNS_G2dGetAnimCtrlType
#define NNS_G2dInitAnimController 						NNS_G2dInitAnimCtrl
#define NNS_G2dInitAnimControllerCallBackFunctor 	NNS_G2dInitAnimCtrlCallBackFunctor
#define NNS_G2dIsAnimControllerActive 					NNS_G2dIsAnimCtrlActive
#define NNS_G2dStartAnimController 					NNS_G2dStartAnimCtrl
#define NNS_G2dStopAnimController 						NNS_G2dStopAnimCtrl
#define NNS_G2dTickAnimController 						NNS_G2dTickAnimCtrl



#define NNS_G2D_ASSERT_ANIMATIONTYPE_VALID( val )   \
        NNS_G2D_MINMAX_ASSERT( val, NNS_G2D_ANIMATIONTYPE_CELL, NNS_G2D_ANIMATIONTYPE_MULTICELLLOCATION )


/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimFrame

  Description:  �A�j���[�V�������̂ւ̃|�C���^�ƁA�\���t���[�����̃y�A�ł�
                �A�j���[�V�����̍ŏ��P�ʂł��B
                ������Frame��NNSG2dAnimSequence���`�����܂��B
                
 *---------------------------------------------------------------------------*/
typedef NNSG2dAnimFrameData NNSG2dAnimFrame;


/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnimSequence

  Description:  ��A�̃A�j���[�V�����f�[�^������킵�܂��B
                ������Frame��������܂�
                Sequence���ł� ���ׂĂ� NNSG2dAnimFrame �� ����NNSG2dAnimationType�������܂��B
                
 *---------------------------------------------------------------------------*/
typedef NNSG2dAnimSequenceData NNSG2dAnimSequence;

/*---------------------------------------------------------------------------*
  Name:         NNSG2dAnmCallBackPtr

  Description:  �A�j���[�V���� �R�[���o�b�N
                
 *---------------------------------------------------------------------------*/
// data = NNSG2dAnimCallBackFunctor.param
typedef void (*NNSG2dAnmCallBackPtr)( u32 data, fx32 currentFrame );


/*---------------------------------------------------------------------------*
  
  Name:         NNSG2dAnmCallbackType

  Description:  �A�j���[�V���� �R�[���o�b�N �̎��
                
 *---------------------------------------------------------------------------*/
typedef enum NNSG2dAnmCallbackType
{    
    NNS_G2D_ANMCALLBACKTYPE_NONE = 0,  
    NNS_G2D_ANMCALLBACKTYPE_LAST_FRM,  // �A�j���[�V�����V�[�P���X�̍ŏI�t���[���I�����ɂ�т���
    NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM,  // �w��t���[���̍Đ����ɌĂт����B
    NNS_G2D_ANMCALLBACKTYPE_EVER_FRM,  // ���t���[���Ăяo���B
    AnmCallbackType_MAX

}NNSG2dAnmCallbackType;

/*---------------------------------------------------------------------------*
  
  Name:         NNSG2dAnimCallBackFunctor

  Description:  �A�j���[�V���� �R�[���o�b�N �̊֐��|�C���^�ƁA
                �Ăяo�����Ɉ����Ƃ��ēn�����A���[�U��`u32�f�[�^���O���[�v������T�O
                NNSG2dAnimController �ɕێ�����܂�
                // �ʖ� NNSG2dAnimCallBackFunctor ���ǉ�����܂���
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dCallBackFunctor
{   
    NNSG2dAnmCallbackType          type;            // �R�[���o�b�N�̎��
    u32                            param;           // ���[�U���p�\�p�����[�^
    NNSG2dAnmCallBackPtr           pFunc;           // �R�[���o�b�N�֐��|�C���^
    u16                            frameIdx;        // �t���[���ԍ�( type == NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM���Ɏg�p)
    u16                            pad16_;          // �p�f�B���O
    
}NNSG2dCallBackFunctor, NNSG2dAnimCallBackFunctor;



/*---------------------------------------------------------------------------*
  
  Name:         NNSG2dAnimController

  Description:  �A�j���[�V�����̏�Ԃ�ێ�����\��
                �A�j���[�V�����f�[�^��ێ����܂�
                
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dAnimController
{
    const NNSG2dAnimFrame*      pCurrent;               // ���݂̃A�j���[�V�����t���[��
    const NNSG2dAnimFrame*      pActiveCurrent;         // ���݂̃A�j���[�V�����t���[��(�L���ȕ\���ΏۂɌ���)
                                                        // �ʏ�́ApCurrent�Ɠ���̒l�������܂��B
                                                        // �\���t���[�����[���t���[���̃A�j���[�V�����t���[��
                                                        // �𐳂����\�����邽�߂ɗ��p����܂��B
                                                        // ��{�I�ɁA�\���t���[�����[���t���[���ł͖����t���[����
                                                        // �����܂����A�ȑO�̃o�[�W�����Ƃ̌݊����ێ��̂��߁A
                                                        // �\���t���[�����[���t���[���݂̂���\�������V�[�P���X�̏ꍇ
                                                        // ��pCurrent�̏����ʒu�Ɠ���̈ʒu���w�������A
                                                        // NULL�ɐݒ肳��邱�Ƃ͂���܂���B
    
    BOOL                        bReverse;               // �t�Đ��t���O
    BOOL                        bActive;                // �A�N�e�B�u�t���O

    fx32                        currentTime;            // ���݂̎���
    fx32                        speed;                  // �X�s�[�h
    
    NNSG2dAnimationPlayMode     overriddenPlayMode;     // �Đ����[�h���v���O���}���I�[�o�[���C�h����ۂɐݒ肳��܂��B
                                                        // �K��l�FNNS_G2D_ANIMATIONPLAYMODE_INVALID
                                                        
    const NNSG2dAnimSequence*   pAnimSequence;          // �֘A�t�����ꂽ�A�A�j���[�V�����V�[�P���X
    NNSG2dAnimCallBackFunctor   callbackFunctor;        // �R�[���o�b�N�t�@���N�^
    
}NNSG2dAnimController;


//------------------------------------------------------------------------------
// �֐��v���g�^�C�v
//------------------------------------------------------------------------------

void NNSi_G2dCallbackFuncHandling
( 
    const NNSG2dCallBackFunctor*  pFunctor, 
    u16 currentFrameIdx 
);


BOOL           NNS_G2dTickAnimCtrl
( 
    NNSG2dAnimController*     pAnimCtrl, 
    fx32                      frames 
);

BOOL           NNS_G2dSetAnimCtrlCurrentFrame
( 
    NNSG2dAnimController*     pAnimCtrl, 
    u16                       index
);

BOOL           NNS_G2dSetAnimCtrlCurrentFrameNoResetCurrentTime
( 
    NNSG2dAnimController*     pAnimCtrl, 
    u16                       index
);

u16 NNS_G2dGetAnimCtrlCurrentFrame
(
    const NNSG2dAnimController*     pAnimCtrl
);

void            NNS_G2dInitAnimCtrl
( 
    NNSG2dAnimController*     pAnimCtrl 
);

void            NNS_G2dInitAnimCtrlCallBackFunctor
( 
    NNSG2dAnimController*     pAnimCtrl 
);

void            NNS_G2dInitAnimCallBackFunctor
( 
    NNSG2dAnimCallBackFunctor*   pCallBack 
);

void            NNS_G2dResetAnimCtrlState
( 
    NNSG2dAnimController*     pAnimCtrl
);

void            NNS_G2dBindAnimCtrl
( 
    NNSG2dAnimController*        pAnimCtrl, 
    const NNSG2dAnimSequence*    pAnimSequence 
);

void* NNS_G2dGetAnimCtrlCurrentElement  ( NNSG2dAnimController* pAnimCtrl );
void* NNS_G2dGetAnimCtrlNextElement     ( NNSG2dAnimController* pAnimCtrl );// testing
fx32  NNS_G2dGetAnimCtrlNormalizedTime  ( NNSG2dAnimController* pAnimCtrl );// testing

void NNS_G2dSetAnimCtrlCallBackFunctor
( 
    NNSG2dAnimController*   pAnimCtrl, 
    NNSG2dAnmCallbackType   type, 
    u32                     param, 
    NNSG2dAnmCallBackPtr    pFunc 
);

void NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame
( 
    NNSG2dAnimController*   pAnimCtrl, 
    u32                     param, 
    NNSG2dAnmCallBackPtr    pFunc, 
    u16                     frameIdx 
);

//------------------------------------------------------------------------------
// ���C�u��������������J�֐�
BOOL NNSi_G2dIsAnimCtrlLoopAnim( const NNSG2dAnimController* pAnimCtrl );

//------------------------------------------------------------------------------
// inline functions 
//------------------------------------------------------------------------------
static NNSG2dAnimationType NNS_G2dGetAnimCtrlType
( 
    const NNSG2dAnimController*  pAnimCtrl 
);

static void NNS_G2dSetAnimCtrlSpeed
( 
    NNSG2dAnimController*       pAnimCtrl, 
    fx32                        speed 
);

static fx32 NNS_G2dGetAnimCtrlSpeed
( 
    const NNSG2dAnimController*       pAnimCtrl
);

static void NNS_G2dStartAnimCtrl
( 
    NNSG2dAnimController*       pAnimCtrl 
);

static void NNS_G2dStopAnimCtrl
( 
    NNSG2dAnimController*       pAnimCtrl 
);

static BOOL NNS_G2dIsAnimCtrlActive
( 
    const NNSG2dAnimController*       pAnimCtrl 
);

static void NNS_G2dSetAnimCtrlPlayModeOverridden
( 
    NNSG2dAnimController*       pAnimCtrl,
    NNSG2dAnimationPlayMode     playMode 
);

static void NNS_G2dResetAnimCtrlPlayModeOverridden
( 
    NNSG2dAnimController*       pAnimCtrl
);

static fx32 NNS_G2dGetAnimCtrlCurrentTime
( 
    const NNSG2dAnimController*       pAnimCtrl 
);

static void NNS_G2dSetAnimCtrlCurrentTime
( 
    NNSG2dAnimController*       pAnimCtrl,
    fx32                        time
);

static u16 NNS_G2dGetAnimCtrlCurrentElemIdxVal
( 
    const NNSG2dAnimController*       pAnimCtrl 
);

static const NNSG2dAnimSequence* 
NNS_G2dGetAnimCtrlCurrentAnimSequence
( 
    const NNSG2dAnimController*       pAnimCtrl 
);



#ifdef __cplusplus
} /* extern "C" */
#endif

#include <nnsys/g2d/g2d_Animation_inline.h>
#endif // NNS_G2D_ANIMATION_H_

