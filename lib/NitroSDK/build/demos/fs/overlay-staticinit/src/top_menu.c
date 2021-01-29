/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     top_menu.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: top_menu.c,v $
  Revision 1.3  2006/01/18 02:12:29  kitase_hirotake
  do-indent

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

/* �g�b�v���j���[����I������e���[�h�̃I�[�o�[���C ID */
FS_EXTERN_OVERLAY(mode_1);
FS_EXTERN_OVERLAY(mode_2);
FS_EXTERN_OVERLAY(mode_3);

enum
{
    MENU_MODE_1,
    MENU_MODE_2,
    MENU_MODE_3,
    MENU_BEGIN = 0,
    MENU_END = MENU_MODE_3,
    MENU_MAX = MENU_END + 1
};


/*****************************************************************************/
/* variable */

/* ���j���[�J�[�\�� */
static int menu_cursor;


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

    /* �㉺�L�[�Ń��j���[��I�� */
    if (IS_INPUT_(input[own_player_id], PAD_KEY_UP, push))
    {
        if (--menu_cursor < MENU_BEGIN)
        {
            menu_cursor = MENU_END;
        }
    }
    if (IS_INPUT_(input[own_player_id], PAD_KEY_DOWN, push))
    {
        if (++menu_cursor > MENU_END)
        {
            menu_cursor = MENU_BEGIN;
        }
    }
    /* A �{�^���Ō��� */
    return !IS_INPUT_(input[own_player_id], PAD_BUTTON_A, push);
}

/*---------------------------------------------------------------------------*
  Name:         MyDrawFrame

  Description:  ���݂̃��[�h�œ�����Ԃ����Ƃɕ`��X�V���܂�.

  Arguments:    frame_count      ���݂̓���t���[���J�E���g.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MyDrawFrame(int frame_count)
{
    int     i;

    (void)frame_count;

    /* �t�@�C�����ƊȈՑ�������̕\�� */
    SetTextColor(GX_RGB(31, 31, 31));
    DrawText(0, 10, "%s", __FILE__);
    DrawText(30, 40, "up/down: select menu");
    DrawText(30, 50, "   A   : run selected mode");
    /* ���j���[�ƃJ�[�\���̕\�� */
    SetTextColor(GX_RGB(31, 31, 16));
    for (i = MENU_BEGIN; i <= MENU_END; ++i)
    {
        DrawText(40, 80 + i * 10, "%c mode %d", ((i == menu_cursor) ? '>' : ' '), i + 1);
    }
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
    /* �I���������[�h�𖾎��I�ɐݒ肵�ĕԂ� */
    switch (menu_cursor)
    {
    case MENU_MODE_1:
        *p_next_mode = FS_OVERLAY_ID(mode_1);
        break;
    case MENU_MODE_2:
        *p_next_mode = FS_OVERLAY_ID(mode_2);
        break;
    case MENU_MODE_3:
        *p_next_mode = FS_OVERLAY_ID(mode_3);
        break;
    }
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

    menu_cursor = 0;
    SetClearColor(GX_RGB(0, 0, 0));
}
