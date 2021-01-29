/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     common.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.4  2005/10/20 07:49:56  yosizaki
  fix port number.

  Revision 1.3  2005/10/03 09:54:20  yosizaki
  fix port number.

  Revision 1.2  2005/08/09 13:55:24  yosizaki
  fix GGID.

  Revision 1.1  2005/06/23 09:05:57  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __NITROSDK_DEMO_MB_MULTIBOOT_WFS_COMMON_H
#define __NITROSDK_DEMO_MB_MULTIBOOT_WFS_COMMON_H


#include <nitro.h>
#include "util.h"


/******************************************************************************/
/* constant */

/* �����ʐM�p�����[�^ */
#define		WC_PARENT_DATA_SIZE_MAX		128
#define		WC_CHILD_DATA_SIZE_MAX		256

#if	!defined(SDK_MAKEGGID_SYSTEM)
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#endif
#define GGID_WBT_FS                 SDK_MAKEGGID_SYSTEM(0x23)

extern const WMParentParam def_parent_param[1];


/* WFS �p�̒ʐM�ݒ� */
#define	port_wbt            5
#define	parent_packet_max   WC_PARENT_DATA_SIZE_MAX


/* ��ʑJ�ڃ��[�h */
enum
{
    MODE_SELECT,                       /* �N���I�v�V�����I�� */
    MODE_ERROR,                        /* �G���[�����ɂ���~ */
    MODE_BUSY,                         /* �N�������� */
    MODE_PARENT,                       /* �e�@�������s�� */
    MODE_CHILD,                        /* �q�@�������s�� */
    MODE_MAX
};


/******************************************************************************/
/* function */

#if	defined(__cplusplus)
extern  "C"
{
#endif

/*---------------------------------------------------------------------------*
  Name:         StateCallbackForWFS

  Description:  WFS �̃R�[���o�b�N�֐�.
                ��Ԃ� WFS_STATE_READY �ɂȂ������_�ŌĂяo����܂�.
                ���̃R�[���o�b�N�Œʒm���󂯂�, �P�ɔC�ӂ̈ʒu��
                WFS_GetStatus() ���g�p���Ĕ��肷�邱�Ƃ��\�ł�.

  Arguments:    arg       �R�[���o�b�N�Ɏw�肵�����[�U��`����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    StateCallbackForWFS(void *arg);

/*---------------------------------------------------------------------------*
  Name:         AllocatorForWFS

  Description:  WFS �Ɏw�肷�郁�����̓��I�m�ۊ֐�.

  Arguments:    arg       �A���P�[�^�Ɏw�肵�����[�U��`����.
                size      �������m�ۗv���Ȃ�ΕK�v�ȃT�C�Y.
                ptr       NULL �̏ꍇ�̓������m��, �����łȂ���Ή���v��.

  Returns:      ptr �� NULL �Ȃ� size ���̃��������m�ۂ��ă|�C���^��Ԃ�����.
                �����łȂ���� ptr �̃�������������邱��.
                ����̏ꍇ, �Ԃ�l�͒P�ɖ��������.
 *---------------------------------------------------------------------------*/
    void   *AllocatorForWFS(void *arg, u32 size, void *ptr);

/*---------------------------------------------------------------------------*
  Name:         WCCallbackForWFS

  Description:  WC �̖��������쓮�����Ԓʒm���󂯂�R�[���o�b�N.

  Arguments:    arg     - �ʒm�� WM �֐��̃R�[���o�b�N�|�C���^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    WCCallbackForWFS(WMCallback *arg);

/*---------------------------------------------------------------------------*
  Name:         InitFrameLoop

  Description:  �Q�[���t���[�����[�v�̏����������B

  Arguments:    p_key           ����������L�[���\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    InitFrameLoop(KeyInfo * p_key);

/*---------------------------------------------------------------------------*
  Name:         WaitForNextFrame

  Description:  ���̕`��t���[���܂őҋ@�B

  Arguments:    p_key           �X�V����L�[���\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    WaitForNextFrame(KeyInfo * p_key);

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  �G���[�\����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeError(void);

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  �r�W�[��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeWorking(void);

/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  �e�@/�q�@ �I����ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeSelect(void);

/*---------------------------------------------------------------------------*
  Name:         ModeMultiboot

  Description:  DS�_�E�����[�h�v���C�e�@�����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeMultiboot(void);

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  �e�@ �ʐM��ʂł̏����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeParent(void);

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  �q�@ �ʐM��ʁB

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    void    ModeChild(void);


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


/******************************************************************************/


#endif                                 /* __NITROSDK_DEMO_WBT_WBT_FS_COMMON_H */
