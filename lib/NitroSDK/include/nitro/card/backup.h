/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - include
  File:     backup.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: backup.h,v $
  Revision 1.17  2007/11/13 04:20:24  yosizaki
  support erase-SUBSECTOR.

  Revision 1.16  2007/10/04 13:08:51  yosizaki
  add test commands.

  Revision 1.15  2007/02/20 00:28:12  kitase_hirotake
  indent source

  Revision 1.14  2006/05/25 02:51:15  yosizaki
  add erase-chip, write-sector.

  Revision 1.13  2006/04/10 04:12:42  yosizaki
  beautify sourcecode indent.

  Revision 1.12  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.11  2005/09/02 07:35:02  yosizaki
  small fix.

  Revision 1.10  2005/09/02 07:14:55  yosizaki
  add CARD_CancelBackupAsync().

  Revision 1.9  2005/05/12 11:21:18  yosizaki
  add some internal parameters.

  Revision 1.8  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.7  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.6  2004/12/08 06:46:28  yosizaki
  add comments.

  Revision 1.5  2004/09/06 13:44:31  yosizaki
  move CARD_(Lock/Unlock)Backup

  Revision 1.4  2004/08/23 10:39:07  yosizaki
  add write-command.
  delete erase-command. (FLASH)

  Revision 1.3  2004/08/18 13:40:49  yosizaki
  add FLASH 2Mbit.

  Revision 1.2  2004/07/28 13:18:29  yosizaki
  asynchronous mode support.

  Revision 1.1  2004/07/23 08:32:18  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#if	!defined(NITRO_CARD_BACKUP_H_)
#define NITRO_CARD_BACKUP_H_


#include <nitro/types.h>


#ifdef __cplusplus
extern  "C"
{
#endif


/*---------------------------------------------------------------------------*
  Name:         CARD_LockBackup

  Description:  lock the CARD-bus before backup operation.

  Arguments:    lock id.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARD_LockBackup(u16 lock_id);

/*---------------------------------------------------------------------------*
  Name:         CARD_UnlockBackup

  Description:  unlock the CARD-bus after backup operation.

  Arguments:    lock id which is used by CARD_LockBackup().

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARD_UnlockBackup(u16 lock_id);


/*---------------------------------------------------------------------------*
  Name:         CARD_TryWaitBackupAsync

  Description:  check if asynchronous backup operation has been completed.

  Arguments:    None.

  Returns:      if operation has been completed, TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARD_TryWaitBackupAsync(void);

/*---------------------------------------------------------------------------*
  Name:         CARD_TryWaitBackupAsync

  Description:  wait for completion of current asynchronous backup operation.

  Arguments:    None.

  Returns:      if the newest operation is successful, TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARD_WaitBackupAsync(void);

/*---------------------------------------------------------------------------*
  Name:         CARD_CancelBackupAsync

  Description:  request to cancel current asynchronous backup operation.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    CARD_CancelBackupAsync(void);

/* internal chip command as Serial-Bus-Interface */


/*---------------------------------------------------------------------------*
  Name:         CARDi_RequestStreamCommand

  Description:  �f�[�^��]������R�}���h�̃��N�G�X�g�𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE
                req_type   �R�}���h���N�G�X�g�^�C�v
                req_retry  �R�}���h���N�G�X�g�̎��s�����g���C�ő��
                req_mode   �R�}���h���N�G�X�g�̓��샂�[�h

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARDi_RequestStreamCommand(u32 src, u32 dst, u32 len,
                                   MIDmaCallback callback, void *arg, BOOL is_async,
                                   CARDRequest req_type, int req_retry, CARDRequestMode req_mode);

/*---------------------------------------------------------------------------*
  Name:         CARDi_RequestWriteSectorCommand

  Description:  �Z�N�^���� + �v���O�����̃��N�G�X�g�𔭍s.

  Arguments:    src        �]�����������A�h���X
                dst        �]����I�t�Z�b�g
                len        �]���T�C�Y
                verify     �x���t�@�C���s���ꍇ�� TRUE
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL    CARDi_RequestWriteSectorCommand(u32 src, u32 dst, u32 len, BOOL verify,
                                        MIDmaCallback callback, void *arg, BOOL is_async);

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadBackup

  Description:  �`�b�v�R�}���h "read" �𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_ReadBackup(u32 src, void *dst, u32 len,
                                 MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len,
                                      callback, arg, is_async,
                                      CARD_REQ_READ_BACKUP, 1, CARD_REQUEST_MODE_RECV);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ProgramBackup

  Description:  �`�b�v�R�}���h "program" ���g�p����ǂݍ��݃R�}���h�𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_ProgramBackup(u32 dst, const void *src, u32 len,
                                    MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_PROGRAM_BACKUP, CARD_RETRY_COUNT_MAX,
                                      CARD_REQUEST_MODE_SEND);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WriteBackup

  Description:  �`�b�v�R�}���h "write" �����g�p���鏑�����݃R�}���h�𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_WriteBackup(u32 dst, const void *src, u32 len,
                                  MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_WRITE_BACKUP, CARD_RETRY_COUNT_MAX,
                                      CARD_REQUEST_MODE_SEND);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_VerifyBackup

  Description:  �`�b�v�R�}���h "read" ���g�p����x���t�@�C�R�}���h�𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_VerifyBackup(u32 dst, const void *src, u32 len,
                                   MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_VERIFY_BACKUP, 1, CARD_REQUEST_MODE_SEND);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_ProgramAndVerifyBackup

  Description:  �`�b�v�R�}���h "program" ���g�p����v���O���� + �x���t�@�C�R�}���h�𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_ProgramAndVerifyBackup(u32 dst, const void *src, u32 len,
                                             MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_PROGRAM_BACKUP, CARD_RETRY_COUNT_MAX,
                                      CARD_REQUEST_MODE_SEND_VERIFY);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WriteAndVerifyBackup

  Description:  �`�b�v�R�}���h "write" ���g�p����v���O���� + �x���t�@�C�R�}���h�𔭍s.

  Arguments:    src        �]�����I�t�Z�b�g�܂��̓������A�h���X
                dst        �]����I�t�Z�b�g�܂��̓������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_WriteAndVerifyBackup(u32 dst, const void *src, u32 len,
                                           MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand((u32)src, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_WRITE_BACKUP, CARD_RETRY_COUNT_MAX,
                                      CARD_REQUEST_MODE_SEND_VERIFY);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EraseBackupSector

  Description:  �`�b�v�R�}���h "erase sector" ���g�p����Z�N�^�����R�}���h�𔭍s.

  Arguments:    dst        ������I�t�Z�b�g
                len        �����T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_EraseBackupSector(u32 dst, u32 len,
                                        MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand(0, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_ERASE_SECTOR_BACKUP, CARD_RETRY_COUNT_MAX,
                                      CARD_REQUEST_MODE_SPECIAL);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EraseBackupSubSector

  Description:  �`�b�v�R�}���h "erase subsector" ���g�p����Z�N�^�����R�}���h�𔭍s.

  Arguments:    dst        ������I�t�Z�b�g
                len        �����T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_EraseBackupSubSector(u32 dst, u32 len,
                                           MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand(0, (u32)dst, len, callback, arg, is_async,
                                      CARD_REQ_ERASE_SUBSECTOR_BACKUP, CARD_RETRY_COUNT_MAX,
                                      CARD_REQUEST_MODE_SPECIAL);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_EraseBackupChip

  Description:  �`�b�v�R�}���h "erase chip" ���g�p����Z�N�^�����R�}���h�𔭍s.

  Arguments:    callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)
                is_async   �񓯊�������w�肳��Ă���� TRUE

  Returns:      ���������������� TRUE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_EraseBackupChip(MIDmaCallback callback, void *arg, BOOL is_async)
{
    return CARDi_RequestStreamCommand(0, 0, 0, callback, arg, is_async,
                                      CARD_REQ_ERASE_CHIP_BACKUP, 1, CARD_REQUEST_MODE_SPECIAL);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ReadBackupAsync

  Description:  �񓯊��o�b�N�A�b�v���[�h (�`�b�v�R�}���h "read" �ɑ���)

  Arguments:    src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_ReadBackupAsync(u32 src, void *dst, u32 len, MIDmaCallback callback, void *arg)
{
    (void)CARDi_ReadBackup(src, dst, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ReadBackup

  Description:  �����o�b�N�A�b�v���[�h (�`�b�v�R�}���h "read" �ɑ���)

  Arguments:    src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_ReadBackup(u32 src, void *dst, u32 len)
{
    return CARDi_ReadBackup(src, dst, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramBackupAsync

  Description:  �񓯊��o�b�N�A�b�v�v���O���� (�`�b�v�R�}���h "program" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_ProgramBackupAsync(u32 dst, const void *src, u32 len,
                                        MIDmaCallback callback, void *arg)
{
    (void)CARDi_ProgramBackup(dst, src, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramBackup

  Description:  �����o�b�N�A�b�v�v���O���� (�`�b�v�R�}���h "program" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_ProgramBackup(u32 dst, const void *src, u32 len)
{
    return CARDi_ProgramBackup(dst, src, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteBackupAsync

  Description:  �񓯊��o�b�N�A�b�v���C�g (�`�b�v�R�}���h "write" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteBackupAsync(u32 dst, const void *src, u32 len,
                                      MIDmaCallback callback, void *arg)
{
    (void)CARDi_WriteBackup(dst, src, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteBackup

  Description:  �����o�b�N�A�b�v���C�g (�`�b�v�R�}���h "write" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteBackup(u32 dst, const void *src, u32 len)
{
    return CARDi_WriteBackup(dst, src, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_VerifyBackupAsync

  Description:  �񓯊��o�b�N�A�b�v�x���t�@�C (�`�b�v�R�}���h "read" �ɑ���)

  Arguments:    src        ��r���I�t�Z�b�g
                dst        ��r�惁�����A�h���X
                len        ��r�T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_VerifyBackupAsync(u32 dst, const void *src, u32 len,
                                       MIDmaCallback callback, void *arg)
{
    (void)CARDi_VerifyBackup(dst, src, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_VerifyBackup

  Description:  �����o�b�N�A�b�v�x���t�@�C (�`�b�v�R�}���h "read" �ɑ���)

  Arguments:    src        ��r���I�t�Z�b�g
                dst        ��r�惁�����A�h���X
                len        ��r�T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_VerifyBackup(u32 dst, const void *src, u32 len)
{
    return CARDi_VerifyBackup(dst, src, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramAndVerifyBackupAsync

  Description:  �񓯊��o�b�N�A�b�v�v���O���� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_ProgramAndVerifyBackupAsync(u32 dst, const void *src, u32 len,
                                                 MIDmaCallback callback, void *arg)
{
    (void)CARDi_ProgramAndVerifyBackup(dst, src, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramAndVerifyBackup

  Description:  �����o�b�N�A�b�v�v���O���� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_ProgramAndVerifyBackup(u32 dst, const void *src, u32 len)
{
    return CARDi_ProgramAndVerifyBackup(dst, src, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyBackupAsync

  Description:  �񓯊��o�b�N�A�b�v���C�g + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteAndVerifyBackupAsync(u32 dst, const void *src, u32 len,
                                               MIDmaCallback callback, void *arg)
{
    (void)CARDi_WriteAndVerifyBackup(dst, src, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyBackup

  Description:  �����o�b�N�A�b�v���C�g + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteAndVerifyBackup(u32 dst, const void *src, u32 len)
{
    return CARDi_WriteAndVerifyBackup(dst, src, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseBackupSectorAsync

  Description:  �񓯊��Z�N�^����.

  Arguments:    dst        ������I�t�Z�b�g
                len        �����T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_EraseBackupSectorAsync(u32 dst, u32 len, MIDmaCallback callback, void *arg)
{
    (void)CARDi_EraseBackupSector(dst, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseBackupSector

  Description:  �����Z�N�^����.

  Arguments:    dst        ������I�t�Z�b�g
                len        �����T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_EraseBackupSector(u32 dst, u32 len)
{
    return CARDi_EraseBackupSector(dst, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseBackupSubSectorAsync

  Description:  �񓯊��T�u�Z�N�^����.

  Arguments:    dst        ������I�t�Z�b�g
                len        �����T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_EraseBackupSubSectorAsync(u32 dst, u32 len, MIDmaCallback callback, void *arg)
{
    (void)CARDi_EraseBackupSubSector(dst, len, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseBackupSubSector

  Description:  �����T�u�Z�N�^����.

  Arguments:    dst        ������I�t�Z�b�g
                len        �����T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_EraseBackupSubSector(u32 dst, u32 len)
{
    return CARDi_EraseBackupSubSector(dst, len, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseBackupChipAsync

  Description:  �񓯊��`�b�v����.

  Arguments:    callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_EraseBackupChipAsync(MIDmaCallback callback, void *arg)
{
    (void)CARDi_EraseBackupChip(callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseBackupChip

  Description:  �����Z�N�^����.

  Arguments:    None.

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_EraseBackupChip(void)
{
    return CARDi_EraseBackupChip(NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteBackupSectorAsync

  Description:  �Z�N�^�P�ʏ��� + �Z�N�^�P�ʃv���O����.

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteBackupSectorAsync(u32 dst, const void *src, u32 len,
                                            MIDmaCallback callback, void *arg)
{
    (void)CARDi_RequestWriteSectorCommand((u32)src, dst, len, FALSE, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteBackupSector

  Description:  �Z�N�^�P�ʏ��� + �Z�N�^�P�ʃv���O����.

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteBackupSector(u32 dst, const void *src, u32 len)
{
    return CARDi_RequestWriteSectorCommand((u32)src, dst, len, FALSE, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyBackupSectorAsync

  Description:  �Z�N�^�P�ʏ��� + �Z�N�^�P�ʃv���O���� + �x���t�@�C.

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteAndVerifyBackupSectorAsync(u32 dst, const void *src, u32 len,
                                                     MIDmaCallback callback, void *arg)
{
    (void)CARDi_RequestWriteSectorCommand((u32)src, dst, len, TRUE, callback, arg, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyBackupSector

  Description:  �Z�N�^�P�ʏ��� + �Z�N�^�P�ʃv���O���� + �x���t�@�C.

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteAndVerifyBackupSector(u32 dst, const void *src, u32 len)
{
    return CARDi_RequestWriteSectorCommand((u32)src, dst, len, TRUE, NULL, NULL, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_AccessStatus

  Description:  �X�e�[�^�X���[�h�܂��̓��C�g (�e�X�g�p)

  Arguments:    command    CARD_REQ_READ_STATUS�܂���CARD_REQ_WRITE_STATUS
                value      CARD_REQ_WRITE_STATUS�ł���Ώ������ޒl

  Returns:      ���������0�ȏ�̒l�A���s����Ε��̒l
 *---------------------------------------------------------------------------*/
int CARDi_AccessStatus(CARDRequest command, u8 value);

/*---------------------------------------------------------------------------*
  Name:         CARDi_ReadStatus

  Description:  �X�e�[�^�X���[�h (�e�X�g�p)

  Arguments:    None.

  Returns:      ���������0�ȏ�̒l�A���s����Ε��̒l
 *---------------------------------------------------------------------------*/
SDK_INLINE int CARDi_ReadStatus(void)
{
    return CARDi_AccessStatus(CARD_REQ_READ_STATUS, 0);
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_WriteStatus

  Description:  �X�e�[�^�X���C�g (�e�X�g�p)

  Arguments:    value      �������ޒl

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARDi_WriteStatus(u8 value)
{
    return (CARDi_AccessStatus(CARD_REQ_WRITE_STATUS, value) >= 0);
}


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_CARD_BACKUP_H_ */
