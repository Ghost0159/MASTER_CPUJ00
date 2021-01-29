/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - include
  File:     spinLock.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: spinLock.h,v $
  Revision 1.31  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.30  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.29  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.28  2004/09/02 06:28:15  yada
  only change comment

  Revision 1.27  2004/07/23 04:28:23  yada
  OS_UnLock* restored for compatibility

  Revision 1.26  2004/07/23 01:52:32  yada
  inline->static inline

  Revision 1.25  2004/07/23 00:58:43  yada
  rename OS_UnLock* -> OS_Unlock*

  Revision 1.24  2004/07/22 08:21:41  yada
  change some lockID's type u32->u16

  Revision 1.23  2004/06/21 11:02:32  yada
  fix comment. incorrect about return value.

  Revision 1.22  2004/04/07 02:03:17  yada
  fix header comment

  Revision 1.21  2004/03/26 10:37:49  yada
  OS_GetLockID(), OS_ReleaseLockID() �ǉ�

  Revision 1.20  2004/03/25 07:25:21  yada
  OS_LockCard() ���J�[�h�̃��b�N�ɂ��Ēǉ�

  Revision 1.19  2004/03/04 09:48:59  yada
  �J�[�g���b�W�̃��b�N�Ɍ���IRQ/FIQ�֎~�ɂ����B

  Revision 1.18  2004/02/05 07:27:19  yada
  ���𕶎����IRIS���������̂� NITRO ���� IRIS�ɖ߂����B

  Revision 1.17  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.16  2004/01/14 01:43:41  yada
  lock��unlock�̓r�����s�֐���ݒ�o����悤�ɂ���

  Revision 1.15  2003/12/25 07:29:56  yada
  �^���[������ɂ��ύX

  Revision 1.14  2003/12/22 13:42:13  yasu
  OS_LockWord/LockByte �� struct �^�O��ǉ�

  Revision 1.13  2003/12/18 07:25:26  yada
  �s�v�R�����g�폜

  Revision 1.12  2003/12/18 07:18:59  yada
  MAINP, SUBP �D��x�f�t�@�C�����폜

  Revision 1.11  2003/12/17 06:08:12  yasu
  �ׂ��ȏC��

  Revision 1.10  2003/12/11 00:35:38  yasu
  target.h �̃R�����g�A�E�g

  Revision 1.9  2003/12/10 10:56:18  yasu
  ISDPrint �ŕK�v�� SpinLock �֐��݂̂̈ڐA
  OS_LockWord() -> OS_LockByWord()
  volatile LockWord -> OS_LockWord �Ȃǂ̕ύX

  Revision 1.8  2003/12/08 12:21:03  yada
  12/3�@REDSDK �ւ̕ύX�Ή�

  Revision 1.7  2003/11/28 01:54:47  yada
  REDSDK��03-11-27���f

  Revision 1.6  2003/11/14 06:50:13  yada
  �V�X�e�����p�ӂ��Ă��� OS_Halt() �� OS_HaltA9() �ɁB

  Revision 1.5  2003/11/06 10:35:55  yada
  IRIS_BB����NITRO_TEG���̏����𕪗�

  Revision 1.4  2003/11/05 07:29:55  yada
  �t�@�C������l ����������L�啶����

  Revision 1.2  2003/11/04 09:08:43  yada
  (none)

  Revision 1.1  2003/10/31 04:12:03  yada
  ���ō쐬


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_OS_SPINLOCK_H_
#define NITRO_OS_SPINLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>

//======================================================================
//                      ���b�N�ϐ�
//
//�E�J�[�h�^�J�[�g���b�W�^�b�o�t�ԋ��L�������[�N�q�`�l�^�u�q�`�l�|�b�^�c
//  �Ȃǂ̂b�o�t�Ԃł̋��L���X�[�X��A�b�o�t�ԑ��M�e�h�e�n ���̐�L���\�[�X
//  �̔r�����䂽�߂ɂ��̊֐������g�p�������B
//======================================================================

// ���b�N�h�c

#define OS_UNLOCK_ID            0      // ���b�N�ϐ������b�N����Ă��Ȃ��ꍇ�̂h�c
#define OS_MAINP_LOCKED_FLAG    0x40   // ���C���v���Z�b�T�ɂ�郍�b�N�m�F�t���O
#define OS_MAINP_LOCK_ID_START  0x40   // ���C���v���Z�b�T�p���b�N�h�c�̊��蓖�ĊJ�n�ԍ�
#define OS_MAINP_LOCK_ID_END    0x6f   //                               ���蓖�ďI���ԍ�
#define OS_MAINP_DBG_LOCK_ID    0x70   //                               �f�o�b�K�\��ԍ�
#define OS_MAINP_SYSTEM_LOCK_ID 0x7f   //                               �V�X�e���\��ԍ�
#define OS_SUBP_LOCKED_FLAG     0x80   //   �T�u�v���Z�b�T�ɂ�郍�b�N�m�F�t���O
#define OS_SUBP_LOCK_ID_START   0x80   //   �T�u�v���Z�b�T�p���b�N�h�c�̊��蓖�ĊJ�n�ԍ�
#define OS_SUBP_LOCK_ID_END     0xaf   //                               ���蓖�ďI���ԍ�
#define OS_SUBP_DBG_LOCK_ID     0xb0   //                               �f�o�b�K�\��ԍ�
#define OS_SUBP_SYSTEM_LOCK_ID  0xbf   //                               �V�X�e���\��ԍ�

#define OS_LOCK_SUCCESS         0      // ���b�N����
#define OS_LOCK_ERROR           (-1)   // ���b�N�G���[

#define OS_UNLOCK_SUCCESS       0      // ���b�N��������
#define OS_UNLOCK_ERROR         (-2)   // ���b�N�����G���[

#define OS_LOCK_FREE            0      // ���b�N������

#define OS_LOCK_ID_ERROR        (-3)   // ���b�N�h�c�G���[


//---- structure of lock variable 
typedef volatile struct OSLockWord
{
    u32     lockFlag;
    u16     ownerID;
    u16     extension;
}
OSLockWord;

/*---------------------------------------------------------------------------*
  Name:         OS_InitLock

  Description:  initialize system lock variable 
                and privilege to access shared resources

                * cartridge exclusive control area is not cleared 
                  because debugger uses.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    OS_InitLock(void);

//----------------------------------------------------------------------
//          �X�s�����b�N
//
//�E�v���Z�b�T�Ԃ⃂�W���[���Ԃŋ��L���\�[�X��r�����䂷�邽�߂�
//  ���b�N�ϐ��̃X�s�����b�N���s���܂��B
//�E���b�N����������܂Ŏ��s�������܂��B
//�E�v���Z�b�T�Ԃ̋��L���\�[�X�͕K�����b�N��Ɏg�p���ĉ������B
//�E�v���Z�b�T��L�̃��\�[�X�̓^�C�~���O�I�ɒ����ł���̂ł����
//  ���b�N���Ȃ��Ă��\���܂���B
//  ��L���\�[�X�̓f�o�b�O���Ɍ��肵�ă��b�N���邱�Ƃ��ł��܂��B
//
//�E�����F
//  lockID              ���b�NID
//  lockp               ���b�N�ϐ��̃|�C���^
//
//�E�߂�l�F
//  OS_LOCK_SUCCESS     ���b�N����
//----------------------------------------------------------------------
s32     OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp) (void));
s32     OS_LockCartridge(u16 lockID);
s32     OS_LockCard(u16 lockID);

//----------------------------------------------------------------------
//          ���b�N�̉���
//
//�E���b�N���������A���L���\�[�X�̃A�N�Z�X�����T�u�v���Z�b�T�ɓn���܂��B
//�E���b�N���Ă��Ȃ����W���[�������s�����ꍇ�ɂ͉������ꂸ�A
//  OS_UNLOCK_ERROR ���Ԃ���܂��B
//
//�E�����F
//  lockID              ���b�NID
//  lockp               ���b�N�ϐ��̃|�C���^
//
//�E�߂�l�F
//  OS_UNLOCK_SUCCESS   ���b�N��������
//  OS_UNLOCK_ERROR     ���b�N�����G���[
//----------------------------------------------------------------------
s32     OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp) (void));
s32     OS_UnlockCartridge(u16 lockID);
s32     OS_UnlockCard(u16 lockID);

//---- for compatibility to old name ('UnLock' <-> 'Unlock')
//     because isd lib calls OS_UnLock*, these cannot be inline.
s32     OS_UnLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp) (void));
s32     OS_UnLockCartridge(u16 lockID);
s32     OS_UnLockCard(u16 lockID);

//----------------------------------------------------------------------
//          ���b�N�̎��s
//
//�E�P�񂾂��X�s�����b�N�����s���܂��B
//�E�v���Z�b�T�Ԃ̋��L���\�[�X�͕K�����b�N��Ɏg�p���ĉ������B
//�E�v���Z�b�T��L�̃��\�[�X�̓^�C�~���O�I�ɒ����ł���̂ł����
//  ���b�N���Ȃ��Ă��\���܂���B
//  ��L���\�[�X�̓f�o�b�O���Ɍ��肵�ă��b�N���邱�Ƃ��ł��܂��B
//
//�E�����F
//  lockID              ���b�NID
//  lockp               ���b�N�ϐ��̃|�C���^
//  CtrlFuncp           ���\�[�X����֐��̃|�C���^
//
//�E�߂�l�F
//  ��                  ���b�N���i���O�Ɋi�[����Ă���ID�j
//  OS_LOCK_SUCCESS     ���b�N����
//----------------------------------------------------------------------
s32     OS_TryLockByWord(u16 lockID, OSLockWord *lockp, void (*crtlFuncp) (void));
s32     OS_TryLockCartridge(u16 lockID);
s32     OS_TryLockCard(u16 lockID);

//----------------------------------------------------------------------
//          ���b�N�ϐ��̏��L���W���[��ID�̓ǂݍ���
//
//�E���b�N�ϐ��̏��L���W���[��ID ��ǂݍ��݂܂��B
//�E���W���[��ID ����0 �̏ꍇ�͂��̎��_�łǂ���̃v���Z�b�T����
//  ���L���������Ă���̂����m�F�ł��܂��B
//�E���L���\�[�X�̏ꍇ�́u���C���v���Z�b�T�������L���������Ă����ԁv
//  �݂̂����荞�݂��֎~���邱�Ƃɂ���Ĉێ����邱�Ƃ��ł��܂��B
//  ���̑��̏�Ԃ̓T�u�v���Z�b�T���ω������Ă��܂��\��������܂��B
//�E���L���W���[��ID �� 0 �ł����Ă����b�N�ϐ�����������Ă���Ƃ͌���܂���B
//
//�E�����F
//  lockp       ���b�N�ϐ��̃|�C���^
//
//�E�߂�l�F    ���L���W���[��ID
//
//���A���A���C���������ɑ΂��Ă̓L���b�V���o�R�łȂ����
//  �o�C�g�A�N�Z�X�͂ł��Ȃ����Ƃɒ��ӂ��ĉ������B
//  �ł��̂ŁA���C����������ł͊�{�I�� OS_ReadOwnerOfLockWord() ���g�p���ĉ������B
//----------------------------------------------------------------------
u16     OS_ReadOwnerOfLockWord(OSLockWord *lockp);
#define OS_ReadOwnerOfLockCartridge()  OS_ReadOwnerOfLockWord( (OSLockWord *)HW_CTRDG_LOCK_BUF )
#define OS_ReadOwnerOfLockCard()       OS_ReadOwnerOfLockWord( (OSLockWord *)HW_CARD_LOCK_BUF  )



/*---------------------------------------------------------------------------*
  Name:         OS_GetLockID

  Description:  get lock ID

  Arguments:    None.

  Returns:      OS_LOCK_ID_ERROR, if fail to get ID

                if ARM9
                   0x40�`0x6f       lockID
                else if ARM7
                   0x80�`0xaf       lockID
                endif

                *Notice:  ID is allocated only 48 pattern at a highest.

 *---------------------------------------------------------------------------*/
s32     OS_GetLockID(void);


/*---------------------------------------------------------------------------*
  Name:         OS_ReleaseLockID

  Description:  release lock ID

  Arguments:    lockID : id to tend to release

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    OS_ReleaseLockID(u16 lockID);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_OS_SPINLOCK_H_ */
#endif
