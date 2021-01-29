/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - mbp
  File:     mbp.h


  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mbp.h,v $
  Revision 1.5  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.3  2004/12/03 08:13:32  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX �ɏC���A MBP_USING_PREVIOUS_DOWNLOAD�X�C�b�`��ǉ�

  Revision 1.2  2004/11/24 06:15:12  yosizaki
  rename MB_StartParentEx/MB_EndEx

  Revision 1.1  2004/11/12 09:02:39  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITROSDK_DEMO_WIRELESSSHARED_MBP_H_
#define NITROSDK_DEMO_WIRELESSSHARED_MBP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/mb.h>


/******************************************************************************/
/* constant */

/*
 * �Q�[���O���[�v ID.
 *
 * MBGameRegistry �\���̂ɂ����ČX�̃t�@�C����
 * GGID ��ݒ�ł���悤�ɂȂ�������,
 * MB_Init() �ł̎w��͏����I�ɈӖ��𐬂��Ȃ��Ȃ邩������܂���.
 */

/*
 * �e�@���z�M����`�����l��.
 *
 * �}���`�u�[�g�q�@�͑S�Ẳ\�ȃ`�����l�������񂵂܂��̂�
 * WM ���C�u�����������l(���� 3, 8, 13)�̂ǂ���̗p���Ă����܂��܂���.
 *
 * ���[�U�A�v���P�[�V�����ɂ����ĒʐM�̍��G�������ꍇ�͉ςɂ��܂�.
 * �`�����l���ύX�̃^�C�~���O�ɂ��Ă̓A�v���P�[�V�����̍ٗʂɂȂ�܂���,
 * �Ⴆ��, ���X�|���X�̈����ɑ΂����[�U���u�ăX�^�[�g�v�������Ȃǂ�
 * �_�@��݂���Ƃ��������@���l�����܂�.
 */

/* �ڑ��\�Ȏq�@�ő吔 */
#define MBP_CHILD_MAX       (15)

/* MB ���C�u�����Ɋ��蓖�Ă� DMA �ԍ� */
#define MBP_DMA_NO          (2)

/*
 * MB ���C�u������ MB_StartParentFromIdle() / MB_EndToIdle() ���g�p����X�C�b�`.
 * �}���`�u�[�g������ IDLE ��ԂŌo�R���邱�Ƃ��ł��܂�.
 */
#define MBP_USING_MB_EX

/*
 * �G���g���[���ė����q�@�ɑ΂��āA���炩���߃f�[�^���M���J�n���Ă����X�C�b�`.
 * �G���g���[����Ƒ����Ƀf�[�^���M���J�n���邽�߁A�q�@��MAX�l���ɂȂ�O�J�n����ꍇ�ɂ�
 * �_�E�����[�h�ׂ̈̑҂����Ԃ�Z�k���邱�Ƃ��ł��܂��B
 */
#define MBP_USING_PREVIOUS_DOWNLOAD


/* MB�e�@��� */
typedef struct
{
    u16     state;                     // �e�@���
    u16     connectChildBmp;           // �ڑ����̑S�q�@�t���O
    u16     requestChildBmp;           // �G���g���[���N�G�X�g���̎q�@�t���O
    u16     entryChildBmp;             // �f�[�^�҂����̎q�@�t���O
    u16     downloadChildBmp;          // �f�[�^�_�E�����[�h���̎q�@�t���O
    u16     bootableChildBmp;          // �_�E�����[�h���������q�@�t���O
    u16     rebootChildBmp;            // �u�[�g�𑗐M�����q�@�t���O
}
MBPState;


/* �ڑ��q�@��� */
typedef struct
{
    MBUserInfo user;
    u8      macAddress[6];
    u16     playerNo;
}
MBPChildInfo;


/* ���̃f���̃Q�[���V�[�P���X��Ԓl */
enum
{
    MBP_STATE_STOP,                    // ��~���
    MBP_STATE_IDLE,                    // �A�C�h�����(Init��)
    MBP_STATE_ENTRY,                   // �q�@�G���g���[��t��
    MBP_STATE_DATASENDING,             // MB�f�[�^���M��
    MBP_STATE_REBOOTING,               // �q�@���u�[�g��
    MBP_STATE_COMPLETE,                // �q�@���u�[�g����
    MBP_STATE_CANCEL,                  // �}���`�u�[�g�̃L�����Z��������
    MBP_STATE_ERROR,                   // �G���[����
    MBP_STATE_NUM
};

/* �ڑ���ԃr�b�g�}�b�v�̃^�C�v */
typedef enum
{
    MBP_BMPTYPE_CONNECT,               // �ڑ��q�@���
    MBP_BMPTYPE_REQUEST,               // �ڑ��v�����̎q�@���
    MBP_BMPTYPE_ENTRY,                 // �G���g���[���ă_�E�����[�h�҂��̎q�@���
    MBP_BMPTYPE_DOWNLOADING,           // �_�E�����[�h���̎q�@���
    MBP_BMPTYPE_BOOTABLE,              // �u�[�g�\�q�@
    MBP_BMPTYPE_REBOOT,                // ���u�[�g�����q�@
    MBP_BMPTYPE_NUM
}
MBPBmpType;

/* �}���`�u�[�g�q�@��� */
typedef enum
{
    MBP_CHILDSTATE_NONE,               // �ڑ��Ȃ�
    MBP_CHILDSTATE_CONNECTING,         // �ڑ���
    MBP_CHILDSTATE_REQUEST,            // �ڑ��v����
    MBP_CHILDSTATE_ENTRY,              // �G���g���[��
    MBP_CHILDSTATE_DOWNLOADING,        // �_�E�����[�h��
    MBP_CHILDSTATE_BOOTABLE,           // �u�[�g�ҋ@��
    MBP_CHILDSTATE_REBOOT,             // ���u�[�g���s
    MBP_CHILDSTATE_NUM
}
MBPChildState;


/******************************************************************************/
/* variable */

/* �e�@������ */
void    MBP_Init(u32 ggid, u16 tgid);
void    MBP_Start(const MBGameRegistry *gameInfo, u16 channel);

/* 1 �t���[�����Ƃ̐e�@���C������ */
const MBPState *MBP_Main(void);

void    MBP_KickChild(u16 child_aid);
void    MBP_AcceptChild(u16 child_aid);
void    MBP_StartRebootAll(void);
void    MBP_StartDownload(u16 child_aid);
void    MBP_StartDownloadAll(void);
BOOL    MBP_IsBootableAll(void);

void    MBP_Cancel(void);

u16     MBP_GetState(void);
u16     MBP_GetChildBmp(MBPBmpType bmpType);
void    MBP_GetConnectState(MBPState * state);
const u8 *MBP_GetChildMacAddress(u16 aid);
MBPChildState MBP_GetChildState(u16 aid);
u16     MBP_GetPlayerNo(const u8 *macAddress);
const MBPChildInfo *MBP_GetChildInfo(u16 child_aid);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // NITROSDK_DEMO_WIRELESSSHARED_MBP_H_
