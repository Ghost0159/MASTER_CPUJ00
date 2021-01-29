/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb_child.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_child.h,v $
  Revision 1.5  2005/09/02 08:29:14  yosizaki
  add compatible symbol 'MB_COMM_CSTATE_CANCELED'

  Revision 1.4  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.3  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.2  2004/11/24 23:46:03  takano_makoto
  MB_COMM_CSTATE_FAKE_END��ǉ�

  Revision 1.1  2004/11/22 12:57:34  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef NITRO_MB_MB_CHILD_H_
#define NITRO_MB_MB_CHILD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>
#include <nitro/mb/mb.h>

/* ---------------------------------------------------------------------

        �^��`

   ---------------------------------------------------------------------*/

//---------------------------------------------------------
// �q�@�_�E�����[�h�̏��
//---------------------------------------------------------

typedef enum
{
    MB_COMM_CSTATE_NONE,               // �������O���
    MB_COMM_CSTATE_INIT_COMPLETE,      // MB�q�@�������������
    MB_COMM_CSTATE_CONNECT,            // �e�@�ւ̐ڑ��������������
    MB_COMM_CSTATE_CONNECT_FAILED,     // �e�@�ւ̐ڑ������s�������
    MB_COMM_CSTATE_DISCONNECTED_BY_PARENT,      // �e�@����ؒf���ꂽ���
    MB_COMM_CSTATE_REQ_ENABLE,         // MP���m�����ăf�[�^���N�G�X�g���\�ɂȂ������
    MB_COMM_CSTATE_REQ_REFUSED,        // ���N�G�X�g�ɑ΂��Đe�@����KICK���ꂽ���
    MB_COMM_CSTATE_DLINFO_ACCEPTED,    // �e�@�Ƀ��N�G�X�g���󗝂��ꂽ���
    MB_COMM_CSTATE_RECV_PROCEED,       // �_�E�����[�h�f�[�^�̎�M�J�n���
    MB_COMM_CSTATE_RECV_COMPLETE,      // �_�E�����[�h�f�[�^�̎�M�������
    MB_COMM_CSTATE_BOOTREQ_ACCEPTED,   // �e�@����̃u�[�g�v����M���
    MB_COMM_CSTATE_BOOT_READY,         // �e�@�Ƃ̒ʐM��ؒf���ău�[�g�����������
    MB_COMM_CSTATE_CANCELED,           // �r���ŃL�����Z������A�e�@�Ƃ̐ؒf�������������
    MB_COMM_CSTATE_CANCELLED = MB_COMM_CSTATE_CANCELED,
    MB_COMM_CSTATE_AUTHENTICATION_FAILED,       // 
    MB_COMM_CSTATE_MEMBER_FULL,        // �e�@�ւ̃G���g���[��������𒴂��Ă����ꍇ�̏��
    MB_COMM_CSTATE_GAMEINFO_VALIDATED, // �e�@�̃r�[�R����M���
    MB_COMM_CSTATE_GAMEINFO_INVALIDATED,        // ���Ɏ擾���Ă����e�@�̃r�[�R���̏�Ԃ��s���S�ȏ�ԂɂȂ����ꍇ
    MB_COMM_CSTATE_GAMEINFO_LOST,      // �e�@�̃r�[�R��������������Ԃ̒ʒm
    MB_COMM_CSTATE_GAMEINFO_LIST_FULL, // ����ȏ�e�@���擾�ł��Ȃ��Ȃ�����Ԃ̒ʒm
    MB_COMM_CSTATE_ERROR,              // �r���ŃG���[�������������

    MB_COMM_CSTATE_FAKE_END,           // �t�F�C�N�q�@�̊����ʒm(mb_fake_child�g�p���̂ݑJ�ڂ�����)

    /*  �����g�p�̗񋓒l�ł��B
       ���̏�Ԃɂ͑J�ڂ��܂���B */
    MB_COMM_CSTATE_WM_EVENT = 0x80000000
}
MBCommCState;

#define MBCommCStateCallback    MBCommCStateCallbackFunc
/*  �q�@�C�x���g�ʒm�R�[���o�b�N�^ */
typedef void (*MBCommCStateCallbackFunc) (u32 status, void *arg);


#ifdef __cplusplus
}
#endif


#endif // NITRO_MB_MB_CHILD_H_
