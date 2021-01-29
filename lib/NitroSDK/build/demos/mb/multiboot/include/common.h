/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.11  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.10  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.9  2004/10/05 06:47:54  sato_masaki
  �ݒ胁�j���[��ǉ��B���̑��F�X�ׂ��ȏC���B

  Revision 1.8  2004/10/04 13:40:25  terui
  �Q�[���O���[�vID��u32�^�ɓ���B

  Revision 1.7  2004/09/30 08:28:35  yosizaki
  add SDK_MAKEGGID_SYSTEM macro.

  Revision 1.6  2004/09/30 03:05:17  sato_masaki
  - �e�@�̖��O��NVRAM����擾����悤�ɕύX�B
  - ������������x�ɁA�`�����l��������ύX����悤�ɕύX�B

  Revision 1.5  2004/09/10 03:02:47  sato_masaki
  �`�����l���Ɋւ���R�����g�ύX�B

  Revision 1.4  2004/09/10 02:29:51  miya
  �o�̓`�����l���ύX

  Revision 1.3  2004/09/08 00:23:27  sato_masaki
  MBGameRegistry�̓o�^����2�ɖ߂��B

  Revision 1.2  2004/08/25 04:28:20  sato_masaki
  MB_CommStartSending�֐�������

  Revision 1.1  2004/08/10 07:32:03  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if !defined(NITRO_MB_DEMO_MULTIBOOT_DATA_H_)
#define NITRO_MB_DEMO_MULTIBOOT_DATA_H_


#include <nitro.h>
#include <nitro/mb.h>


/*
 * multiboot �f���S�̂̋��ʒ�`.
 */


/******************************************************************************/
/* constant */

/*
 * �Q�[���O���[�v ID.
 *
 * MBGameRegistry �\���̂ɂ����ČX�̃t�@�C����
 * GGID ��ݒ�ł���悤�ɂȂ�������,
 * MB_Init() �ł̎w��͏����I�ɈӖ��𐬂��Ȃ��Ȃ邩������܂���.
 */
#define SDK_MAKEGGID_SYSTEM(num)    (0x003FFF00 | (num))
#define MY_GGID         SDK_MAKEGGID_SYSTEM(0x20)

/*
 * �e�@���z�M����`�����l��.
 *
 * �}���`�u�[�g�q�@�͑S�Ẳ\�ȃ`�����l�������񂵂܂��̂�
 * WM ���C�u�����������l(���� 1, 7, 13)�̂����ꂩ���g�p����悤�ɂ��Ă�������.
 *
 * ���[�U�A�v���P�[�V�����ɂ����ĒʐM�̍��G�������ꍇ�͉ςɂ��܂�.
 * �`�����l���ύX�̃^�C�~���O�ɂ��Ă̓A�v���P�[�V�����̍ٗʂɂȂ�܂���,
 * �Ⴆ��, ���X�|���X�̈����ɑ΂����[�U���u�ăX�^�[�g�v�������Ȃǂ�
 * �_�@��݂���Ƃ��������@���l�����܂�.
 */
#define PARENT_CHANNEL  13

/* �\������q�@���X�g�ő吔 */
#define DISP_LIST_NUM   (15)

/* MB ���C�u�����Ɋ��蓖�Ă� DMA �ԍ� */
#define MB_DMA_NO       2

/* ���̃f�����o�^����Q�[������ */
#define GAME_PROG_MAX   2

/* ���̃f�����񋟂���}���`�u�[�g�Q�[���v���O������� */
extern const MBGameRegistry mbGameList[GAME_PROG_MAX];

/* ���̃f���̃Q�[���V�[�P���X��Ԓl */
enum
{
    STATE_NONE,
    STATE_MENU,
    STATE_MB_PARENTINIT,
    STATE_MB_PARENT
};


/******************************************************************************/
/* variable */

/* MB ���C�u�����֊��蓖�Ă郏�[�N�̈� */
extern u32 cwork[MB_SYSTEM_BUF_SIZE / sizeof(u32)];

/* ���̃f���̃Q�[���V�[�P���X��� */
extern u8 prog_state;


/******************************************************************************/
/* function */

/* �L�[�g���K���o */
u16     ReadKeySetTrigger(u16 keyset);

/* min - max �͈̔͂�val�l�� offset�����[�e�[�V���� */
BOOL    RotateU8(u8 *val, u8 min, u8 max, s8 offset);

/* �e�@������ */
void    ParentInit(void);

/* 1 �t���[�����Ƃ̐e�@���C������ */
void    ParentMain(void);

/* �t�@�C���o�b�t�@�|�C���^�������� */
void    InitFileBuffer(void);

/* �e�@MP���M�T�C�Y���Z�b�g */
void    SetParentSendSize(u16 p_size);

/* �e�@MP���M�T�C�Y���擾 */
u16     GetParentSendSize(void);

/* �q�@�ő�ڑ������Z�b�g */
void    SetMaxEntry(u8 num);

/* �q�@�ő�ڑ������擾 */
u8      GetMaxEntry(void);

/* �e�@�̃I�[�g����ݒ� */
void    SetAutoOperation(u8 fAccept, u8 fSend, u8 fBoot);

#endif /* !defined(NITRO_MB_DEMO_MULTIBOOT_DATA_H_) */
