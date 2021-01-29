/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wep-1
  File:     main.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.h,v $
  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model �� WEP Key �̓��I�ݒ�������Awep-1 �f���Ƃ��ĐV�K�o�^

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef __MAIN_H__
#define __MAIN_H__

enum
{
    SYSMODE_SELECT_ROLE,               // �����i�e�q�̂ǂ���Ƃ��ĊJ�n���邩�j�̑I�����
    SYSMODE_OPTION,                    // �I�v�V�������
    SYSMODE_SELECT_PARENT,             // �e�@�I����ʁi�q�@��p�j
    SYSMODE_LOBBY,                     // ���r�[��ʁi�e�@��p�j
    SYSMODE_LOBBYWAIT,                 // ���r�[�ҋ@��ʁi�q�@��p�j
    SYSMODE_SELECT_CHANNEL,            // �`�����l���I�����
    SYSMODE_SCAN_PARENT,               // �e�@�������
    SYSMODE_ERROR,                     // �G���[�񍐉��
    SYSMODE_PARENT,                    // �e�@���[�h���
    SYSMODE_CHILD,                     // �q�@���[�h���
    SYSMODE_NUM
};

enum
{
    SHARECMD_NONE = 0,                 // ���ɖ����i�m�[�}���j
    SHARECMD_EXITLOBBY,                // ���r�[��ʏI���̍��}
    SHARECMD_NUM
};

typedef struct ShareData_
{
    unsigned int command:3;            // �w�߁i�Q�[����Ԃ̈�Đ؂芷���ȂǂɎg�p�j
    unsigned int level:2;              // �d�g��M���x
    unsigned int data:3;               // �O���t�p
    unsigned int key:16;               // �L�[�f�[�^
    unsigned int count:24;             // �t���[����
}
ShareData;

struct PRScreen_;

extern BOOL isDataReceived(int index);
extern ShareData *getRecvData(int index);
extern ShareData *getSendData(void);
extern void changeSysMode(int s);
extern struct PRScreen_ *getInfoScreen(void);

#endif
