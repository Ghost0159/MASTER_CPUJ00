/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     common.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef WM_DEMO_WIRELESS_ALL_COMMON_H_
#define WM_DEMO_WIRELESS_ALL_COMMON_H_


#include <nitro.h>

#include "mbp.h"
#include "wh.h"
#include "wfs.h"


/*****************************************************************************/
/* declaration */

/* �f�[�^�V�F�A�����O�Ɏg�p����L�[���͍\���� */
typedef struct
{
    u32     count;                     /* �t���[����� */
    u16     key;                       /* �L�[��� */
    u16     padding;
}
GShareData;

SDK_COMPILER_ASSERT(sizeof(GShareData) <= WH_DS_DATA_SIZE);


/*****************************************************************************/
/* constant */

/* �e�L�X�g�p�p���b�g�J���[ */
enum
{
    PLTT_BLACK = 0,
    PLTT_BLUE = 1,
    PLTT_RED = 2,
    PLTT_PURPLE = 3,
    PLTT_GREEN = 4,
    PLTT_CYAN = 5,
    PLTT_YELLOW = 6,
    PLTT_WHITE = 7,

    PLTT_LINK_ICON                     /* ���C�����X�����N���x�\���A�C�R���p�p���b�g */
};


/*****************************************************************************/
/* function */

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
  Name:         InitCommon

  Description:  ���ʏ������֐�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitCommon(void);

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_PRESS

  Description:  ���g�̃L�[����.

  Arguments:    ���肷��L�[�t���O.

  Returns:      �w�肵���L�[����������Ă���ꍇ�� TRUE,
                ����Ă��Ȃ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
BOOL    IS_PAD_PRESS(u16 flag);

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_TRIGGER

  Description:  ���g�̃L�[�g���K����.

  Arguments:    ���肷��L�[�t���O.

  Returns:      �w�肵���L�[�̃g���K�������Ă���ꍇ�� TRUE,
                �����Ă��Ȃ��ꍇ�� FALSE.
 *---------------------------------------------------------------------------*/
BOOL    IS_PAD_TRIGGER(u16 flag);

/*---------------------------------------------------------------------------*
  Name:         GetCurrentInput

  Description:  �w��v���C���[�̌��݂̃L�[���͂��擾.

  Arguments:    aid               �L�[���͂��擾����v���C���[.

  Returns:      �ŐV�̃f�[�^�V�F�A�����O�ɐ������Ă���ΗL���ȃA�h���X,
                �����łȂ���� NULL.
 *---------------------------------------------------------------------------*/
GShareData *GetCurrentInput(int aid);

/*---------------------------------------------------------------------------*
  Name:         GetCurrentChannel

  Description:  ���ݑI�����Ă��郏�C�����X�`�����l�����擾.

  Arguments:    None

  Returns:      ���ݑI�����Ă��郏�C�����X�`�����l��.
 *---------------------------------------------------------------------------*/
u16     GetCurrentChannel(void);

/*---------------------------------------------------------------------------*
  Name:         SetCurrentChannel

  Description:  ���݂̃��C�����X�`�����l����ݒ�.

  Arguments:    channel           �ݒ肷��`�����l��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SetCurrentChannel(u16 channel);

/*---------------------------------------------------------------------------*
  Name:         LoadLinkIcon

  Description:  �����N�A�C�R����VRAM�Ƀ��[�h.

  Arguments:    id                ���[�h��̃L�����N�^ID.
                palette           ���[�h��̃p���b�g.
                level             �����N���x.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    LoadLinkIcon(int id, int palette, int level);

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  BG�e�L�X�g��`��.

  Arguments:    x                 �`�� x �O���b�h�ʒu.
                y                 �`�� x �O���b�h�ʒu.
                palette           �p���b�g�C���f�b�N�X.
                format            ����������.

  Returns:      None
 *---------------------------------------------------------------------------*/
void    PrintString(int x, int y, int palette, const char *format, ...);

/*---------------------------------------------------------------------------*
  Name:         WaitNextFrame

  Description:  ���̃s�N�`���[�t���[����҂��`�������������.

  Arguments:    None.

  Returns:      �L�[���͂��X�V���ꂽ�� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    WaitNextFrame(void);

/*---------------------------------------------------------------------------*
  Name:         WaitWHState

  Description:  WH �̎w��̃X�e�[�g�ɂȂ�܂őҋ@.

  Arguments:    state             �J�ڂ�҂X�e�[�g.

  Returns:      None
 *---------------------------------------------------------------------------*/
void    WaitWHState(int state);

/*---------------------------------------------------------------------------*
  Name:         MeasureChannel

  Description:  �g�p�\�Ȋe�`�����l���̃��C�����X�g�p�����v��.
                ��������܂œ����ŕ\�����s���u���b�L���O����.

  Arguments:    None.

  Returns:      �����Ƃ����C�����X�g�p���̏������`�����l��.
 *---------------------------------------------------------------------------*/
u16     MeasureChannel(void);

/*---------------------------------------------------------------------------*
  Name:         ExecuteDownloadServer

  Description:  DS�_�E�����[�h�v���C�e�@���������s.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ExecuteDownloadServer(void);

/*---------------------------------------------------------------------------*
  Name:         StartWirelessParent

  Description:  �f�[�^�V�F�A�����O�e�@���[�h���N��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    StartWirelessParent(void);

/*---------------------------------------------------------------------------*
  Name:         StartWirelessChild

  Description:  �f�[�^�V�F�A�����O�q�@���[�h���N��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    StartWirelessChild(void);

/*---------------------------------------------------------------------------*
  Name:         ExecuteDataSharing

  Description:  �f�[�^�V�F�A�����O�̃��C������.
                �o�b�N�O���E���h�� WFS ����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ExecuteDataSharing(void);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // WM_DEMO_WIRELESS_ALL_COMMON_H_
