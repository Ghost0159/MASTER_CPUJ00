/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     mode_1.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mode_1.c,v $
  Revision 1.2  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:45  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include	"mode.h"
#include	"draw.h"

/*
 * NitroStaticInit() �� static initializer �Ɏw�肷��ɂ�
 * ���̃w�b�_���C���N���[�h���܂�.
 */
#include <nitro/sinit.h>


/*****************************************************************************/
/* constant */


/*****************************************************************************/
/* variable */


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         MyUpdateFrame

  Description:  ���݂̃��[�h�� 1 �t���[��������������Ԃ��X�V���܂�.

  Arguments:    frame_count      ���݂̓���t���[���J�E���g.
                input            ���͏��̔z��.
                player_count     ���݂̃v���[���[����. (input �̗L���v�f��)
                own_player_id    ���g�̃v���[���[�ԍ�.

  Returns:      ���݂̃��[�h������̃t���[���ŏI������Ȃ�� FALSE ��,
                �����łȂ���� TRUE ��Ԃ��܂�.
 *---------------------------------------------------------------------------*/
static BOOL MyUpdateFrame(int frame_count,
                          const InputData * input, int player_count, int own_player_id)
{
    (void)frame_count;
    (void)player_count;

    /* ��������������I�� */
    return !IS_INPUT_(input[own_player_id], ~0, push);
}

/*---------------------------------------------------------------------------*
  Name:         MyDrawFrame

  Description:  ���݂̃��[�h�œ�����Ԃ����Ƃɕ`��X�V���܂�.

  Arguments:    frame_count      ���݂̓���t���[���J�E���g.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MyDrawFrame(int frame_count)
{
    /* �t�@�C�����ƊȈՑ�������̕\�� */
    SetTextColor(GX_RGB(31, 31, 31));
    DrawText(0, 10, "%s", __FILE__);
    DrawText(30, 40, "press any key to return");
    /* �P�Ƀt���[���J�E���g�̕\�� */
    SetTextColor(GX_RGB(31, 31, 16));
    DrawText(40, 80, "frame count = %d", frame_count);
}

/*---------------------------------------------------------------------------*
  Name:         MyEndFrame

  Description:  ���݂̃��[�h���I�����܂�.

  Arguments:    p_next_mode      ���̃��[�h�𖾎��I�Ɏw�肷��ꍇ��.
                                 ���̃|�C���^���w�����ID���㏑�����܂�.
                                 ���Ɏw�肵�Ȃ��ꍇ, ���݂̃��[�h���Ăяo����
                                 ���[�h���I������܂�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MyEndFrame(FSOverlayID *p_next_mode)
{
    /* ���������g�b�v���j���[�ɕԂ� */
    (void)p_next_mode;
}

/*---------------------------------------------------------------------------*
  Name:         NitroStaticInit

  Description:  static initializer �Ƃ��Ă̎����������֐�.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void NitroStaticInit(void)
{
    UpdateFrame = MyUpdateFrame;
    DrawFrame = MyDrawFrame;
    EndFrame = MyEndFrame;

    /* ���[�h���ƂɕK�v�ȏ����������������ōs���܂�. */

    SetClearColor(GX_RGB(0, 0, 0));
}
