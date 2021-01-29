/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_Animation_inline.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/
 
#ifndef NNS_G2D_ANIMATION_INLINE_H_
#define NNS_G2D_ANIMATION_INLINE_H_

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// inline functions 
//------------------------------------------------------------------------------



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlType

  Description:  �A�j���[�V�����R���g���[�� �� �A�j����ނ��擾���܂��B
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      �R���g���[���̎��
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSG2dAnimationType NNS_G2dGetAnimCtrlType
( 
    const NNSG2dAnimController*  pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    if( pAnimCtrl->pAnimSequence != NULL )
    {
        return NNS_G2dGetAnimSequenceAnimType( pAnimCtrl->pAnimSequence );
    }else{
        return NNS_G2D_ANIMATIONTYPE_MAX;
    }   
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlSpeed

  Description:  �A�j���[�V�����R���g���[�� �� �A�j���X�s�[�h��ݒ肵�܂�
                
  Arguments:    pAnimCtrl   :            �A�j���[�V�����R���g���[�� ����
                speed       :           �X�s�[�h
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dSetAnimCtrlSpeed
( 
    NNSG2dAnimController*       pAnimCtrl, 
    fx32                        speed 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    pAnimCtrl->speed = speed;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlSpeed

  Description:  �A�j���[�V�����R���g���[�� �� �A�j���X�s�[�h���擾���܂�
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      �A�j���X�s�[�h
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE fx32 NNS_G2dGetAnimCtrlSpeed
( 
    const NNSG2dAnimController*       pAnimCtrl
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    return pAnimCtrl->speed;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dStartAnimCtrl

  Description:  �A�j���[�V�����R���g���[�� �� �A�j���Đ����X�^�[�g���܂��B
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dStartAnimCtrl
( 
    NNSG2dAnimController*       pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    pAnimCtrl->bActive = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dStopAnimCtrl

  Description:  �A�j���[�V�����R���g���[�� �� �A�j���Đ����I�����܂��B
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      �R���g���[���̎��
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dStopAnimCtrl
( 
    NNSG2dAnimController*       pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    pAnimCtrl->bActive = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsAnimCtrlActive

  Description:  �A�j���[�V�����R���g���[�� �� �A�j���Đ������������܂��B
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      �A�j���Đ����Ȃ�TRUE
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNS_G2dIsAnimCtrlActive
( 
    const NNSG2dAnimController*       pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    return pAnimCtrl->bActive;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlPlayModeOverridden

  Description:  �t�@�C�����Őݒ肳�ꂽ�A�j���[�V�����V�[�P���X�̍Đ�������
                �I�[�o�[���C�h���܂��B
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                playMode :            �A�j���Đ�����
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dSetAnimCtrlPlayModeOverridden
( 
    NNSG2dAnimController*       pAnimCtrl,
    NNSG2dAnimationPlayMode     playMode 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    NNS_G2D_MINMAX_ASSERT( playMode, NNS_G2D_ANIMATIONPLAYMODE_FORWARD,
                                 NNS_G2D_ANIMATIONPLAYMODE_REVERSE_LOOP );
                                 
    pAnimCtrl->overriddenPlayMode = playMode;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetAnimCtrlPlayModeOverridden

  Description:  �I�[�o�[���C�h���ꂽ�A�j���Đ����������ɖ߂��܂��B
                �i�t�@�C�����Őݒ肳�ꂽ�Đ������ɖ߂��܂��j
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dResetAnimCtrlPlayModeOverridden
( 
    NNSG2dAnimController*       pAnimCtrl
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    pAnimCtrl->overriddenPlayMode = NNS_G2D_ANIMATIONPLAYMODE_INVALID;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlCurrentTime

  Description:  �A�j���[�V�����R���g���[�� �� ���݂̃A�j���t���[���\�����Ԃ��擾���܂�
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
                
  Returns:      ���݂̃A�j���t���[���\������
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE fx32 NNS_G2dGetAnimCtrlCurrentTime
( 
    const NNSG2dAnimController*       pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    return pAnimCtrl->currentTime;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetAnimCtrlCurrentTime

  Description:  �A�j���[�V�����R���g���[�� �� ���݂̃A�j���t���[���\�����Ԃ�ݒ肵�܂�
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                time     �F           ���݂̃A�j���t���[���\������
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dSetAnimCtrlCurrentTime
( 
    NNSG2dAnimController*       pAnimCtrl,
    fx32                        time
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    pAnimCtrl->currentTime = time;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlCurrentElemIdxVal

  Description:  �A�j���[�V�����R���g���[�� �� 
                ���݂̃A�j�����ʂ���C���f�b�N�X�̒l���擾���܂�
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
  Returns:      ���݂̃A�j�����ʂ̃C���f�b�N�X�l
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u16 NNS_G2dGetAnimCtrlCurrentElemIdxVal
( 
    const NNSG2dAnimController*       pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    //
    // �ǂ̌`���̃A�j���[�V�����ł����Ă��A�擪��2�o�C�g�͕K���A
    // �C���f�b�N�X�l�Ƃ��ė��p����Ă��܂��B
    //
    {
        const NNSG2dAnimData* pAnmRes 
           = (const NNSG2dAnimData*)pAnimCtrl->pCurrent->pContent;
    
        return (*pAnmRes);        
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetAnimCtrlCurrentAnimSequence

  Description:  �A�j���[�V�����R���g���[�� �� 
                ���݂̃A�j���[�V�����V�[�P���X���擾���܂�
                
  Arguments:    pAnimCtrl:            �A�j���[�V�����R���g���[�� ����
                
                
  Returns:      ���݂̃A�j���[�V�����V�[�P���X
  
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dAnimSequence* 
NNS_G2dGetAnimCtrlCurrentAnimSequence
( 
    const NNSG2dAnimController*       pAnimCtrl 
)
{
    NNS_G2D_NULL_ASSERT( pAnimCtrl );
    return pAnimCtrl->pAnimSequence;
}


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif // NNS_G2D_ANIMATION_INLINE_H_

