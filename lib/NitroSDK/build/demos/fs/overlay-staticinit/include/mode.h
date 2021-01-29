/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     mode.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mode.h,v $
  Revision 1.3  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:06  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:53  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if	!defined(NITRO_DEMOS_FS_OVERLAY_STATICINIT_MODE_H_)
#define NITRO_DEMOS_FS_OVERLAY_STATICINIT_MODE_H_


#include <nitro.h>


/*
 * �e���[�h�p�I�[�o�[���C�̃C���^�t�F�[�X���`���܂�.
 * �I�[�o�[���C�� static initializer �ł��������Ȑݒ肵�܂�.
 */


#if	defined(__cplusplus)
extern  "C"
{
#endif


/********************************************************************/
/* structure */

/* ���͏��\���� */
    typedef struct
    {
        TPData  tp;
        u16     push_bits;             /* �����ꂽ�u�Ԃ̃L�[�r�b�g */
        u16     hold_bits;             /* ������Ă����Ԃ̃L�[�r�b�g */
        u16     release_bits;          /* �����ꂽ�u�Ԃ̃L�[�r�b�g */
        u16     reserved;
    }
    InputData;


/********************************************************************/
/* constant */

/* �^�b�`�y���ڐG��Ԃ������Ǝ��g���r�b�g */
#define PAD_BUTTON_TP   0x4000


/********************************************************************/
/* function */

/* �w��̃L�[�ɑ΂����Ԕ���}�N�� */
#define	IS_INPUT_(input, key, action)	\
	(((input).action ## _bits & (key)) != 0)


/*---------------------------------------------------------------------------*
  Name:         UpdateFrame

  Description:  ���݂̃��[�h�� 1 �t���[��������������Ԃ��X�V���܂�.

  Arguments:    frame_count      ���݂̓���t���[���J�E���g.
                input            ���͏��̔z��.
                player_count     ���݂̃v���[���[����. (input �̗L���v�f��)
                own_player_id    ���g�̃v���[���[�ԍ�.

  Returns:      ���݂̃��[�h������̃t���[���ŏI������Ȃ�� FALSE ��,
                �����łȂ���� TRUE ��Ԃ��܂�.
 *---------------------------------------------------------------------------*/
    extern BOOL (*UpdateFrame) (int frame_count,
                                const InputData * input, int player_count, int own_player_id);

/*---------------------------------------------------------------------------*
  Name:         DrawFrame

  Description:  ���݂̃��[�h�œ�����Ԃ����Ƃɕ`��X�V���܂�.

  Arguments:    frame_count      ���݂̓���t���[���J�E���g.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    extern void (*DrawFrame) (int frame_count);

/*---------------------------------------------------------------------------*
  Name:         EndFrame

  Description:  ���݂̃��[�h���I�����܂�.

  Arguments:    p_next_mode      ���̃��[�h�𖾎��I�Ɏw�肷��ꍇ��.
                                 ���̃|�C���^���w�����ID���㏑�����܂�.
                                 ���Ɏw�肵�Ȃ��ꍇ, ���݂̃��[�h���Ăяo����
                                 ���[�h���I������܂�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    extern void (*EndFrame) (FSOverlayID *p_next_mode);


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


#endif                                 /* NITRO_DEMOS_FS_OVERLAY_STATICINIT_MODE_H_ */
