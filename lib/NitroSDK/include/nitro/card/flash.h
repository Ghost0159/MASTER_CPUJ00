/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - include
  File:     flash.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: flash.h,v $
  Revision 1.8  2007/09/24 23:51:11  yosizaki
  update copyright date.

  Revision 1.7  2007/08/22 08:30:42  yosizaki
  add EraseSector and Program API

  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.4  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.3  2004/11/02 10:07:39  yosizaki
  fix comment.

  Revision 1.2  2004/08/23 10:39:07  yosizaki
  add write-command.
  delete erase-command. (FLASH)

  Revision 1.1  2004/08/18 13:40:54  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#if	!defined(NITRO_CARD_FLASH_H_)
#define NITRO_CARD_FLASH_H_


#include <nitro/types.h>
#include <nitro/card/common.h>
#include <nitro/card/backup.h>


#ifdef __cplusplus
extern  "C"
{
#endif


/*---------------------------------------------------------------------------*
  Name:         CARD_IsBackupFlash

  Description:  ���ݐݒ肳��Ă���f�o�C�X��FLASH������

  Arguments:    None.

  Returns:      ���ݐݒ肳��Ă���f�o�C�X��FLASH�Ȃ�TRUE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_IsBackupFlash(void)
{
    const CARDBackupType t = CARD_GetCurrentBackupType();
         
         
        return (((t >> CARD_BACKUP_TYPE_DEVICE_SHIFT) &
                 CARD_BACKUP_TYPE_DEVICE_MASK) == CARD_BACKUP_TYPE_DEVICE_FLASH);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ReadFlash

  Description:  ����FLASH�ǂݍ��� (�`�b�v�R�}���h "read" �ɑ���)

  Arguments:    src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_ReadFlash(u32 src, void *dst, u32 len)
{
    return CARD_ReadBackup(src, dst, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ReadFlashAsync

  Description:  �񓯊�FLASH�ǂݍ��� (�`�b�v�R�}���h "read" �ɑ���)

  Arguments:    src        �]�����I�t�Z�b�g
                dst        �]���惁�����A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_ReadFlashAsync(u32 src, void *dst, u32 len,
                                    MIDmaCallback callback, void *arg)
{
    CARD_ReadBackupAsync(src, dst, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteFlash

  Description:  ����FLASH�������� (�`�b�v�R�}���h "write" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteFlash(u32 dst, const void *src, u32 len)
{
    return CARD_WriteBackup(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteFlashAsync

  Description:  �񓯊�FLASH�������� (�`�b�v�R�}���h "write" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteFlashAsync(u32 dst, const void *src, u32 len,
                                     MIDmaCallback callback, void *arg)
{
    CARD_WriteBackupAsync(dst, src, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_VerifyFlash

  Description:  ����FLASH�x���t�@�C

  Arguments:    dst        ��r��I�t�Z�b�g
                src        ��r���������A�h���X
                len        ��r�T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_VerifyFlash(u32 dst, const void *src, u32 len)
{
    return CARD_VerifyBackup(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_VerifyFlashAsync

  Description:  �񓯊�FLASH�x���t�@�C

  Arguments:    dst        ��r��I�t�Z�b�g
                src        ��r���������A�h���X
                len        ��r�T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_VerifyFlashAsync(u32 dst, const void *src, u32 len,
                                      MIDmaCallback callback, void *arg)
{
    CARD_VerifyBackupAsync(dst, src, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyFlash

  Description:  ����FLASH�������� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteAndVerifyFlash(u32 dst, const void *src, u32 len)
{
    return CARD_WriteAndVerifyBackup(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyFlashAsync

  Description:  ����FLASH�������� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteAndVerifyFlashAsync(u32 dst, const void *src, u32 len,
                                              MIDmaCallback callback, void *arg)
{
    CARD_WriteAndVerifyBackupAsync(dst, src, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseFlashSector

  Description:  �����Z�N�^����.

  Arguments:    dst        ������I�t�Z�b�g
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                len        �����T�C�Y
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_EraseFlashSector(u32 dst, u32 len)
{
    return CARD_EraseBackupSector(dst, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_EraseFlashSectorAsync

  Description:  �񓯊��Z�N�^����

  Arguments:    dst        ������I�t�Z�b�g
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                len        �����T�C�Y
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_EraseFlashSectorAsync(u32 dst, u32 len,
                                           MIDmaCallback callback, void *arg)
{
    CARD_EraseBackupSectorAsync(dst, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramFlash

  Description:  ����FLASH�v���O���� (�`�b�v�R�}���h "program" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_ProgramFlash(u32 dst, const void *src, u32 len)
{
    return CARD_ProgramBackup(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramFlashAsync

  Description:  �񓯊�FLASH�v���O���� (�`�b�v�R�}���h "program" �ɑ���)

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_ProgramFlashAsync(u32 dst, const void *src, u32 len,
                                       MIDmaCallback callback, void *arg)
{
    CARD_ProgramBackupAsync(dst, src, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramAndVerifyFlash

  Description:  ����FLASH�v���O���� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_ProgramAndVerifyFlash(u32 dst, const void *src, u32 len)
{
    return CARD_ProgramAndVerifyBackup(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_ProgramAndVerifyFlashAsync

  Description:  �񓯊�FLASH�v���O���� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                src        �]�����������A�h���X
                len        �]���T�C�Y
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_ProgramAndVerifyFlashAsync(u32 dst, const void *src, u32 len,
                                                MIDmaCallback callback, void *arg)
{
    CARD_ProgramAndVerifyBackupAsync(dst, src, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteFlashSector

  Description:  �Z�N�^���� + �v���O����

  Arguments:    dst        �]����I�t�Z�b�g
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                src        �]�����������A�h���X
                len        �]���T�C�Y
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE.
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteFlashSector(u32 dst, const void *src, u32 len)
{
    return CARD_WriteBackupSector(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteFlashSectorAsync

  Description:  �Z�N�^���� + �v���O����

  Arguments:    dst        �]����I�t�Z�b�g
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                src        �]�����������A�h���X
                len        �]���T�C�Y
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteFlashSectorAsync(u32 dst, const void *src, u32 len,
                                           MIDmaCallback callback, void *arg)
{
    CARD_WriteBackupSectorAsync(dst, src, len, callback, arg);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyFlashSector

  Description:  �Z�N�^���� + �v���O���� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                src        �]�����������A�h���X
                len        �]���T�C�Y
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������

  Returns:      CARD_RESULT_SUCCESS �Ŋ�������� TRUE, ����ȊO�� FALSE
 *---------------------------------------------------------------------------*/
SDK_INLINE BOOL CARD_WriteAndVerifyFlashSector(u32 dst, const void *src, u32 len)
{
    return CARD_WriteAndVerifyBackupSector(dst, src, len);
}

/*---------------------------------------------------------------------------*
  Name:         CARD_WriteAndVerifyFlashSectorAsync

  Description:  �Z�N�^���� + �v���O���� + �x���t�@�C

  Arguments:    dst        �]����I�t�Z�b�g
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                src        �]�����������A�h���X
                len        �]���T�C�Y
                           �Z�N�^�T�C�Y�̐����{�ł���K�v������
                callback   �����R�[���o�b�N (�s�g�p�Ȃ� NULL)
                arg        �����R�[���o�b�N�̈��� (�s�g�p�Ȃ疳�������)

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE void CARD_WriteAndVerifyFlashSectorAsync(u32 dst, const void *src, u32 len,
                                                    MIDmaCallback callback, void *arg)
{
    CARD_WriteAndVerifyBackupSectorAsync(dst, src, len, callback, arg);
}


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_CARD_EEPROM_H_ */
