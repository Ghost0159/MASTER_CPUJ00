/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_Animation.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.30 $
 *---------------------------------------------------------------------------*/

#include <nnsys/g2d/g2d_Animation.h>
#include <stdlib.h>


//------------------------------------------------------------------------------
// private funcs


//------------------------------------------------------------------------------
static NNS_G2D_INLINE const NNSG2dAnimFrame* GetFrameBegin_( const NNSG2dAnimSequence* pSequence )
{    
    NNS_G2D_NULL_ASSERT( pSequence );
    return pSequence->pAnmFrameArray;
}
//------------------------------------------------------------------------------
static NNS_G2D_INLINE const NNSG2dAnimFrame* GetFrameEnd_( const NNSG2dAnimSequence* pSequence )
{    
    NNS_G2D_NULL_ASSERT( pSequence );
    return pSequence->pAnmFrameArray + ( pSequence->numFrames );
}
//------------------------------------------------------------------------------
static NNS_G2D_INLINE const NNSG2dAnimFrame* GetFrameLoopBegin_( const NNSG2dAnimSequence* pSequence )
{
    NNS_G2D_NULL_ASSERT( pSequence );
    
    return pSequence->pAnmFrameArray + pSequence->loopStartFrameIdx;
}
//------------------------------------------------------------------------------
// TODO: NOT EFFICEANT !
static NNS_G2D_INLINE u16 GetCurrentFrameIdx_( const NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    return (u16)(( (u32)pAnimCtrl->pCurrent - (u32)pAnimCtrl->pAnimSequence->pAnmFrameArray ) 
                    / sizeof( NNSG2dAnimFrameData ));
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE NNSG2dAnimationPlayMode GetAnimationPlayMode_
( 
    const NNSG2dAnimController* pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    
    //
    // �����ApAnimCtrl->overriddenPlayMode���ݒ肳��Ă����
    // �Đ����[�h�̓I�[�o�[���C�h�����B
    // 
    if( pAnimCtrl->overriddenPlayMode != NNS_G2D_ANIMATIONPLAYMODE_INVALID )
    {
        return pAnimCtrl->overriddenPlayMode;
    }else{
        return pAnimCtrl->pAnimSequence->playMode;
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL IsLoopAnimSequence_( const NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    
    {
    const NNSG2dAnimationPlayMode   playMode = GetAnimationPlayMode_( pAnimCtrl );
    

    return ( playMode == NNS_G2D_ANIMATIONPLAYMODE_FORWARD_LOOP || 
             playMode == NNS_G2D_ANIMATIONPLAYMODE_REVERSE_LOOP ) ? TRUE : FALSE;
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL IsReversePlayAnim_( const NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    
    {
    const NNSG2dAnimationPlayMode   playMode = GetAnimationPlayMode_( pAnimCtrl );
    

    return ( playMode == NNS_G2D_ANIMATIONPLAYMODE_REVERSE || 
             playMode == NNS_G2D_ANIMATIONPLAYMODE_REVERSE_LOOP ) ? TRUE : FALSE;
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL IsAnimCtrlMovingForward_( const NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );    
    return ( pAnimCtrl->speed > 0 )^(pAnimCtrl->bReverse) ? TRUE : FALSE;
}




//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL ShouldAnmCtrlMoveNext_( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pCurrent );
    
    if( pAnimCtrl->bActive && 
        (pAnimCtrl->currentTime >= FX32_ONE * (int)pAnimCtrl->pCurrent->frames) )
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------------------
static NNS_G2D_INLINE  void CallbackFuncHandling_( const NNSG2dCallBackFunctor*  pFunctor, u16 currentFrameIdx )
{
    NNS_G2D_NULL_ASSERT( pFunctor );
    
    switch( pFunctor->type )
    {
    // call at the specified frame.
    case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:
        if( currentFrameIdx == pFunctor->frameIdx )
        {
            (*pFunctor->pFunc)( pFunctor->param, currentFrameIdx );
        }   
        break;
    // call at every frames.
    case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:        
        (*pFunctor->pFunc)( pFunctor->param, currentFrameIdx );
        break;
    }
}

//------------------------------------------------------------------------------
// �O�����J�̂��߂̃��b�p�[
void NNSi_G2dCallbackFuncHandling( const NNSG2dCallBackFunctor*  pFunctor, u16 currentFrameIdx )
{
    CallbackFuncHandling_( pFunctor, currentFrameIdx );
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL IsReachStartEdge_( const NNSG2dAnimController* pAnimCtrl, const NNSG2dAnimFrame* pFrame )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pFrame );
    
    return ( pFrame <= ( GetFrameLoopBegin_( pAnimCtrl->pAnimSequence ) - 1 ) ) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE BOOL IsReachEdge_( const NNSG2dAnimController* pAnimCtrl, const NNSG2dAnimFrame* pFrame )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pFrame );
    
    
    if( IsAnimCtrlMovingForward_( pAnimCtrl ) )
    {
        // tail edge of the animation sequence.
        return ( pFrame >= GetFrameEnd_( pAnimCtrl->pAnimSequence ) ) ? TRUE : FALSE;
    }else{
        // head edge of the animation sequence.
        return IsReachStartEdge_( pAnimCtrl, pFrame );
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void MoveNext_( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    if( IsAnimCtrlMovingForward_( pAnimCtrl ) )
    {
        pAnimCtrl->pCurrent++;
        
    }else{
        pAnimCtrl->pCurrent--;    
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void SequenceEdgeHandleCommon_( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    //
    // NNS_G2D_ANMCALLBACKTYPE_LAST_FRM �^�C�v��
    // �R�[���o�b�N�Ăяo���̏���
    // 
    if( pAnimCtrl->callbackFunctor.type == NNS_G2D_ANMCALLBACKTYPE_LAST_FRM )
    {
        NNS_G2D_NULL_ASSERT( pAnimCtrl->callbackFunctor.pFunc );
        (*pAnimCtrl->callbackFunctor.pFunc)( pAnimCtrl->callbackFunctor.param, pAnimCtrl->currentTime );
    }
    
    // 
    // ���Z�b�g����
    //
    if( !IsLoopAnimSequence_( pAnimCtrl ) )
    {
        // ���[�v�Đ��Ŗ�����΁A�X�V���X�g�b�v����
        NNS_G2dStopAnimCtrl( pAnimCtrl );
    }else{
        NNS_G2dResetAnimCtrlState( pAnimCtrl );
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void SequenceEdgeHandleReverse_( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    // �Đ������𔽓]����
    pAnimCtrl->bReverse = pAnimCtrl->bReverse^TRUE;
    
    //
    // �A�j���[�V�����X�^�[�g�t���[�����H
    // 
    if( IsReachStartEdge_( pAnimCtrl, pAnimCtrl->pCurrent ) )
    {
        SequenceEdgeHandleCommon_( pAnimCtrl );
    }
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void SequenceEdgeHandleNormal_( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    SequenceEdgeHandleCommon_( pAnimCtrl );
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void ValidateAnimFrame_( NNSG2dAnimController* pAnimCtrl, const NNSG2dAnimFrame** pFrame )
{
    if( *pFrame > GetFrameEnd_( pAnimCtrl->pAnimSequence ) - 1 )
    {
        *pFrame = GetFrameEnd_( pAnimCtrl->pAnimSequence ) - 1;
    }else if( *pFrame < GetFrameBegin_( pAnimCtrl->pAnimSequence ) ){
        *pFrame = GetFrameBegin_( pAnimCtrl->pAnimSequence );
    }
}

//------------------------------------------------------------------------------
static void SequenceEdgeHandle_( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    if( IsReversePlayAnim_( pAnimCtrl ) )
    {
        SequenceEdgeHandleReverse_( pAnimCtrl );
    }else{
        SequenceEdgeHandleNormal_( pAnimCtrl );
    }
    
    ValidateAnimFrame_( pAnimCtrl, &pAnimCtrl->pCurrent );
}

//------------------------------------------------------------------------------
// SetAnimCtrlCurrentFrame�̎����ł�
// NNS_G2dSetAnimCtrlCurrentFrame()
// NNS_G2dSetAnimCtrlCurrentFrameNoResetCurrentTime()
// ����Ă΂�܂��B
//
static BOOL SetAnimCtrlCurrentFrameImpl_
( 
    NNSG2dAnimController*     pAnimCtrl, 
    u16                       index
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    
    
    
    // �L����Index���H
    if( index < pAnimCtrl->pAnimSequence->numFrames )
    {
        // set
        pAnimCtrl->pCurrent = &pAnimCtrl->pAnimSequence->pAnmFrameArray[index];
        if( pAnimCtrl->pCurrent->frames != 0 )
        {
           pAnimCtrl->pActiveCurrent = pAnimCtrl->pCurrent;
        }
        return TRUE;   
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlCurrentElement
                NNS_G2dGetAnimCtrlNextElement

  Description:  ���݁i���́j�̃A�j�����ʂ��擾���܂��B
                ���[�U�� �{�֐��̌��ʂ� NNSG2dAnimController ���̂Ɋ֘A�t�����Ă���A
                �A�j���[�V�����f�[�^�̎�ނɉ����ēK�؂Ȍ^�ɃL���X�g�㗘�p���܂��B
                
  Arguments:    pAnimCtrl:           [IN]  NNSG2dAnimController ����
                
  Returns:      ���݂̃A�j�����ʁi���w���|�C���^�j
  
 *---------------------------------------------------------------------------*/
void* NNS_G2dGetAnimCtrlCurrentElement( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pActiveCurrent );
    NNS_G2D_NULL_ASSERT( (void*)pAnimCtrl->pActiveCurrent->pContent );
    
    // �L���ȕ\���Ώۂ�Ԃ��悤�ɂ��܂��B(pCurrent�͕\���t���[�������[���̏ꍇ������܂�)
    return (void*)pAnimCtrl->pActiveCurrent->pContent;
}
//------------------------------------------------------------------------------
void* NNS_G2dGetAnimCtrlNextElement( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    {
        const NNSG2dAnimFrame*      pNext = NULL;
        
        if( IsAnimCtrlMovingForward_( pAnimCtrl ) )
        {
            pNext = pAnimCtrl->pCurrent + 1;
        }else{
            pNext = pAnimCtrl->pCurrent - 1;
        }
        
        ValidateAnimFrame_( pAnimCtrl, &pNext );
        NNS_G2D_NULL_ASSERT( pNext->pContent );
        
        return (void*)pNext->pContent;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlNormalizedTime

  Description:  ���ݕ\�����̃A�j���[�V�����t���[���̕\���ςݎ��Ԃ�
                0.0 �� �\���J�n�� 1.0 �� �� �\���������Ƃ��� fx32 �^�Ŏ擾���܂��B 
                
  Arguments:    pAnimCtrl:          [IN]  NNSG2dAnimController ����
                
  Returns:      �A�j���[�V�����t���[���̕\������
  
 *---------------------------------------------------------------------------*/
fx32 NNS_G2dGetAnimCtrlNormalizedTime(  NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pCurrent );
    
    return FX_Div( pAnimCtrl->currentTime, FX32_ONE * pAnimCtrl->pCurrent->frames );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTickAnimCtrl

  Description:  NNSG2dAnimController �� ���Ԃ�i�߂܂�
                
  Arguments:    pAnimCtrl:          [OUT]  NNSG2dAnimController ����
                frames:             [IN]   �i�߂鎞�ԁi�t���[���j
                
                
  Returns:      �Đ��g���b�N���ύX�ɂȂ�����
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dTickAnimCtrl( NNSG2dAnimController* pAnimCtrl, fx32 frames ) 
{
    BOOL    bChangeFrame = FALSE;
    
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pCurrent );
    NNS_G2D_ASSERTMSG( frames >=  0, "frames must be Greater than zero");
    
    
    if( pAnimCtrl->bActive != TRUE )
    {
        return FALSE;
    } 
       
    //
    // update current time.
    //
    pAnimCtrl->currentTime += abs( FX_Mul( pAnimCtrl->speed, frames ) );
    
    
    //    
    // do actual works for changing current animation frame.
    //
    while( ShouldAnmCtrlMoveNext_( pAnimCtrl ) )
    {
        bChangeFrame = TRUE; // �ύX��������
        
        pAnimCtrl->currentTime -= FX32_ONE * (int)pAnimCtrl->pCurrent->frames;    
        MoveNext_( pAnimCtrl );
        
        // if we reach the edge of animation, we have to reset status.
        if( IsReachEdge_( pAnimCtrl, pAnimCtrl->pCurrent ) )
        {
            SequenceEdgeHandle_( pAnimCtrl );
        }

        // �\���t���[�����[���Ŗ�����΁A
        // �L���ȕ\���t���[���Ƃ��܂��B
        if( pAnimCtrl->pCurrent->frames != 0 )
        {
           pAnimCtrl->pActiveCurrent = pAnimCtrl->pCurrent;
        }
        
        // call the callback function.
        if( pAnimCtrl->callbackFunctor.type != NNS_G2D_ANMCALLBACKTYPE_NONE )
        {
            CallbackFuncHandling_( &pAnimCtrl->callbackFunctor, GetCurrentFrameIdx_( pAnimCtrl ) );
        }    
    }    
    return bChangeFrame;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlCurrentFrame

  Description:  �A�j���[�V�����R���g���[���̍Đ��A�j���[�V�����t���[����ݒ肵�܂��B
                
  Arguments:    pAnimCtrl:        [OUT]   NNSG2dAnimController ����
                index    :        [IN]    �A�j���[�V�����V�[�P���X�ԍ�
                        
  Returns:      �ύX�����������ꍇ��TRUE
  
 *---------------------------------------------------------------------------*/
BOOL           NNS_G2dSetAnimCtrlCurrentFrame
( 
    NNSG2dAnimController*     pAnimCtrl, 
    u16                       index
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    {
        const BOOL result = SetAnimCtrlCurrentFrameImpl_( pAnimCtrl, index );
        
        if( result )
        {
            // ���݂̃A�j���t���[���\�����Ԃ����Z�b�g����
            pAnimCtrl->currentTime   = 0;
        }
        return result;
    }       
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlCurrentFrameNoResetCurrentTime

  Description:  �A�j���[�V�����R���g���[���̍Đ��A�j���[�V�����t���[����ݒ肵�܂��B
                �t���[���ύX���ɃA�j���[�V�����R���g���[�������̌��݂̃t���[���\��
                ���ԃJ�E���^�����Z�b�g���܂���B
                
                ���̂��߁A1�A�j���[�V�����t���[���̕\�����Ԃ��Z�����ԂŃA�j���[�V
                �����t���[����؂�ւ���悤�ȏꍇ�ł��A�A�j���[�V�����V�[�P���X��
                �i�s�����邱�Ƃ��\�ł��B

                
  Arguments:    pAnimCtrl:           [OUT] NNSG2dAnimController ����
                index    :           [IN]  �A�j���[�V�����V�[�P���X�ԍ�
                        
  Returns:      �ύX�����������ꍇ��TRUE
  
 *---------------------------------------------------------------------------*/
BOOL           NNS_G2dSetAnimCtrlCurrentFrameNoResetCurrentTime
( 
    NNSG2dAnimController*     pAnimCtrl, 
    u16                       index
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    
    //
    // ���݂̃A�j���t���[���\�����Ԃ����Z�b�g���Ȃ�
    //
    return SetAnimCtrlCurrentFrameImpl_( pAnimCtrl, index );
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlCurrentFrame

  Description:  NNSG2dAnimController ���݂̃A�j���[�V�����t���[���ԍ����擾���܂��B
                
  Arguments:    pAnimCtrl:          [OUT] NNSG2dAnimController ����
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
u16 NNS_G2dGetAnimCtrlCurrentFrame
(
    const NNSG2dAnimController*     pAnimCtrl
)
{
    return GetCurrentFrameIdx_( pAnimCtrl );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitAnimCtrl

  Description:  NNSG2dAnimController �����������܂�
                
  Arguments:    pAnimCtrl:          [OUT] NNSG2dAnimController ����
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitAnimCtrl( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    NNS_G2dInitAnimCallBackFunctor( &pAnimCtrl->callbackFunctor );
    
    pAnimCtrl->pCurrent              = NULL;
    pAnimCtrl->pActiveCurrent        = NULL;
    
    pAnimCtrl->bReverse              = FALSE;
    pAnimCtrl->bActive               = TRUE;
    
    pAnimCtrl->currentTime           = 0;
    pAnimCtrl->speed                 = FX32_ONE;
    
    pAnimCtrl->overriddenPlayMode    = NNS_G2D_ANIMATIONPLAYMODE_INVALID;
    pAnimCtrl->pAnimSequence         = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitAnimCtrlCallBackFunctor

  Description:  NNSG2dAnimController ���� �R�[���o�b�N �����������܂�
                
  Arguments:    pAnimCtrl:          [OUT] NNSG2dAnimController ����
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitAnimCtrlCallBackFunctor( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    NNS_G2dInitAnimCallBackFunctor( &pAnimCtrl->callbackFunctor );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitAnimCallBackFunctor

  Description:  �R�[���o�b�N�t�@���N�^ �����������܂�
                
  Arguments:    pCallBack:          [OUT] NNSG2dCallBackFunctor ����
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitAnimCallBackFunctor( NNSG2dCallBackFunctor* pCallBack )
{
    NNS_G2D_NULL_ASSERT( pCallBack );
    
    pCallBack->type     = NNS_G2D_ANMCALLBACKTYPE_NONE;
    pCallBack->param    = 0x0;
    pCallBack->pFunc    = NULL;  
    pCallBack->frameIdx = 0;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetAnimCtrlState

  Description:  NNSG2dAnimController �̏�Ԃ����Z�b�g���܂�
                
  Arguments:    pAnimCtrl:          [OUT] NNSG2dAnimController ����
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dResetAnimCtrlState( NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    
    if( IsAnimCtrlMovingForward_( pAnimCtrl ) )
    {
        pAnimCtrl->pCurrent      = GetFrameLoopBegin_( pAnimCtrl->pAnimSequence );
    }else{
        pAnimCtrl->pCurrent      = GetFrameEnd_( pAnimCtrl->pAnimSequence ) - 1;
    }
    // �\���t���[�����[���t���[���݂̂���\�������V�[�P���X�̏ꍇ
    // ��NULL�ɐݒ肳��邱�Ƃ͂���܂���B
    pAnimCtrl->pActiveCurrent = pAnimCtrl->pCurrent;
    
    //
    // currentTime �����Z�b�g���܂�
    //
    pAnimCtrl->currentTime   = 0;
    
    //
    // �[���t���[���J�E���^��i�߂܂�
    // �\���t���[�����[���̃A�j���[�V�����t���[�����擪�ɑ��݂���
    // �A�j���[�V�����V�[�P���X�ɑΉ����邽�߂̏����ł��B
    //
    (void)NNS_G2dTickAnimCtrl( pAnimCtrl, 0 );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dBindAnimCtrl

  Description:  NNSG2dAnimController ���� �� �A�j���[�V�����f�[�^���֘A�t���܂�
                
  Arguments:    pAnimCtrl:           [OUT]  NNSG2dAnimController ����
                pAnimSequence:       [IN]   �A�j���[�V�����f�[�^
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dBindAnimCtrl( NNSG2dAnimController* pAnimCtrl, const NNSG2dAnimSequence* pAnimSequence )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimSequence );
    
    pAnimCtrl->pAnimSequence = pAnimSequence;
    
    NNS_G2dResetAnimCtrlState( pAnimCtrl );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlCallBackFunctor

  Description:  �R�[���o�b�N��ݒ肵�܂��B
                �w��t���[���Ăяo���^�C�v�̃R�[���o�b�N�ݒ��
                NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame()�𗘗p���Ă�������
                
  Arguments:    pAnimCtrl:         [OUT] NNSG2dAnimController ����
                type:              [IN]  �R�[���o�b�N�̎��
                param:             [IN]  ���[�U��`���
                pFunc:             [IN]  �R�[���o�b�N�֐��|�C���^
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetAnimCtrlCallBackFunctor
( 
    NNSG2dAnimController*   pAnimCtrl, 
    NNSG2dAnmCallbackType   type, 
    u32                     param, 
    NNSG2dAnmCallBackPtr    pFunc 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( (void*)pFunc );
    NNS_G2D_MINMAX_ASSERT( type, NNS_G2D_ANMCALLBACKTYPE_NONE, AnmCallbackType_MAX );
    NNS_G2D_ASSERTMSG( type != NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM, 
        "Use NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame() instead." );
        
    pAnimCtrl->callbackFunctor.pFunc     = pFunc;
    pAnimCtrl->callbackFunctor.param     = param;
    pAnimCtrl->callbackFunctor.type      = type;
    pAnimCtrl->callbackFunctor.frameIdx  = 0; // meanless for these kind of type.
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame

  Description:  �R�[���o�b�N��ݒ肵�܂��B
                �w��t���[���Ăяo���^�C�v�̃R�[���o�b�N�ݒ��
                NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame()�𗘗p���Ă�������
                
  Arguments:    pAnimCtrl:         [OUT]    NNSG2dAnimController ����
                param:             [IN]     ���[�U��`���
                pFunc:             [IN]     �R�[���o�b�N�֐��|�C���^
                frameIdx:          [IN]     �R�[���o�b�N���Ăяo���A�j���[�V�����t���[���ԍ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame
( 
    NNSG2dAnimController*   pAnimCtrl, 
    u32                     param, 
    NNSG2dAnmCallBackPtr    pFunc, 
    u16                     frameIdx 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( (void*)pFunc );
    
    pAnimCtrl->callbackFunctor.type  = NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM;
    
    pAnimCtrl->callbackFunctor.pFunc     = pFunc;
    pAnimCtrl->callbackFunctor.param     = param;
    pAnimCtrl->callbackFunctor.frameIdx  = frameIdx;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dIsAnimCtrlLoopAnim

  Description:  �A�j���[�V�����R���g���[�������[�v�A�j���[�V�������Đ����Ă��邩
                �������܂��B
                
                [���C�u�����������J�֐�]
               
  Arguments:    pAnimCtrl:          [IN] NNSG2dAnimController ����
                
  Returns:      ���[�v�A�j���[�V�������Đ����Ă���ꍇ��TRUE��Ԃ��܂��B
  
 *---------------------------------------------------------------------------*/
BOOL NNSi_G2dIsAnimCtrlLoopAnim( const NNSG2dAnimController* pAnimCtrl )
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_NULL_ASSERT( pAnimCtrl->pAnimSequence );
    
    return IsLoopAnimSequence_( pAnimCtrl );
}
