/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     main.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.h,v $
  Revision 1.7  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/25 01:35:27  seiki_masashi
  �\���̖��� GameInfo ���� MyGameInfo �ɕύX

  Revision 1.5  2005/11/21 00:41:10  adachi_hiroaki
  �e�@�I����ʂɎQ���l���ƃj�b�N�l�[����\������悤�ɂ���

  Revision 1.4  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.3  2004/11/05 09:16:48  sasakis
  �E�B���h�E�̋��������ǂ��A�R�[�h�𐮗������B

  Revision 1.2  2004/11/05 04:27:40  sasakis
  ���r�[��ʁA�e�@�I����ʂ̒ǉ��ƁA����ɕK�v�ȉ����iscan�֘A�Ȃǁj�B

  Revision 1.1  2004/11/02 01:07:34  sasakis
  �Ӗ��̖����R�[�h�ɂȂ��Ă��܂��Ă���������߂������A�������B

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

typedef struct MyGameInfo_
{
    u8      nickName[10 * 2];
    u8      nickNameLength;
    u8      entryCount;
    u16     periodicalCount;
}
MyGameInfo;

struct PRScreen_;

extern BOOL isDataReceived(int index);
extern ShareData *getRecvData(int index);
extern ShareData *getSendData(void);
extern void changeSysMode(int s);
extern struct PRScreen_ *getInfoScreen(void);

#endif
