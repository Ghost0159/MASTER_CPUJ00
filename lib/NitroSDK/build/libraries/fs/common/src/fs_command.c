/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - libraries
  File:     fs_command.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "../include/util.h"
#include "../include/command.h"


/* �A�[�J�C�u�͊�{�I�� ARM7 �Ɋ܂܂Ȃ� */
#if defined(FS_IMPLEMENT)


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         FSi_ReleaseCommand

  Description:  �����֐�.
                �R�}���h���������ҋ@�X���b�h������Ε��A����.

  Arguments:    p_file           ���������R�}���h���i�[���� FSFile �\����.
                ret              �R�}���h�̌��ʒl.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FSi_ReleaseCommand(FSFile *p_file, FSResult ret)
{
    SDK_ASSERT(FS_IsBusy(p_file));
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        FSi_CutFromList(p_file);
        p_file->stat &= ~(FS_FILE_STATUS_CANCEL | FS_FILE_STATUS_BUSY |
                          FS_FILE_STATUS_SYNC | FS_FILE_STATUS_ASYNC | FS_FILE_STATUS_OPERATING);
        p_file->error = ret;
        OS_WakeupThread(p_file->queue);
        (void)OS_RestoreInterrupts(bak_psr);
    }
}

/*---------------------------------------------------------------------------*
  Name:         FSi_TranslateCommand

  Description:  �����֐�.
                ���[�U�v���V�[�W���܂��̓f�t�H���g�������Ăяo��, ���ʂ�Ԃ�.
                �����n�R�}���h���񓯊��I������Ԃ����ꍇ�͓����Ŋ�����҂�.
                �񓯊��n�R�}���h���񓯊��I������Ԃ����ꍇ�͂��̂܂ܕԂ�.

  Arguments:    p_file           ���s����R�}���h���i�[���� FSFile �\����.
                command          �R�}���h ID.

  Returns:      �R�}���h�̏�������.
 *---------------------------------------------------------------------------*/
FSResult FSi_TranslateCommand(FSFile *p_file, FSCommandType command)
{
    FSResult ret;

    FSArchive *const p_arc = p_file->arc;
    const int bit = (1 << command);

    /* ���� / �񓯊������̃t���O�𗧂Ă� */
    if (FS_IsFileSyncMode(p_file))
        p_arc->flag |= FS_ARCHIVE_FLAG_IS_SYNC;
    else
        p_arc->flag |= FS_ARCHIVE_FLAG_IS_ASYNC;

    /* �v���V�[�W�����Ή����Ă���ΌĂяo���ĉ������m�F���� */
    if ((p_arc->proc_flag & bit) != 0)
    {
        switch (ret = (*p_arc->proc) (p_file, command))
        {
        case FS_RESULT_SUCCESS:
        case FS_RESULT_FAILURE:
        case FS_RESULT_UNSUPPORTED:
            /* �v���V�[�W�����ŃR�}���h���������� */
            p_file->error = ret;
            break;
        case FS_RESULT_PROC_ASYNC:
            /* �񓯊������̈����͌�ōׂ������� */
            break;
        case FS_RESULT_PROC_UNKNOWN:
            /* ���m�̃R�}���h�Ȃ̂ō���ȍ~�f�t�H���g�ɐ؂�ւ��� */
            ret = FS_RESULT_PROC_DEFAULT;
            p_arc->proc_flag &= ~bit;
            break;
        }
    }
    else
    {
        ret = FS_RESULT_PROC_DEFAULT;
    }
    /* �K�v�Ȃ�f�t�H���g�̏������Ăяo�� */
    if (ret == FS_RESULT_PROC_DEFAULT)
    {
        ret = (*fsi_default_command[command]) (p_file);
    }
    /* �񓯊������ɓ������ꍇ, �������[�h�Ȃ�u���b�L���O */
    if (ret == FS_RESULT_PROC_ASYNC)
    {
        if (FS_IsFileSyncMode(p_file))
        {
            OSIntrMode bak_psr = OS_DisableInterrupts();
            while (FSi_IsArchiveSync(p_arc))
                OS_SleepThread(&p_arc->sync_q);
            ret = p_file->error;
            (void)OS_RestoreInterrupts(bak_psr);
        }
    }
    /* �񓯊����[�h�œ������������ꍇ�͂����ŉ�� */
    else if (!FS_IsFileSyncMode(p_file))
    {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_ASYNC;
        FSi_ReleaseCommand(p_file, ret);
    }
    /* �����n�R�}���h���������������ꍇ�͂��̂܂܌��ʂ��i�[ */
    else
    {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_SYNC;
        p_file->error = ret;
    }
    /* ���ʂ�Ԃ�. (�񓯊��n�̔񓯊������̂� FS_RESULT_PROC_ASYNC) */
    return ret;
}


#endif /* FS_IMPLEMENT */

/*---------------------------------------------------------------------------*
  $Log: fs_command.c,v $
  Revision 1.14  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.13  2005/06/17 13:02:55  yosizaki
  fix FS_WaitAsync. (add FS_FILE_STATUS_OPERATING)

  Revision 1.12  2005/05/30 04:18:26  yosizaki
  move some functions to fs_command_default.c.
  add comments.

  Revision 1.11  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.10  2005/02/18 07:26:49  yasu
  Signed/Unsigned �ϊ��x���}��

  Revision 1.9  2004/10/22 13:06:52  yosizaki

  Revision 1.8  2004/09/22 08:55:59  yosizaki
  fix FSi_SeekDirDirect().

  Revision 1.7  2004/09/03 06:54:20  yosizaki
  small fix.

  Revision 1.6  2004/09/02 11:03:00  yosizaki
  fix FS include directives.

  Revision 1.5  2004/08/05 03:26:28  yosizaki
  fix FS_COMMAND_READDIR (numbering of file-ID)

  Revision 1.4  2004/07/23 08:30:06  yosizaki
  fix command operation.

  Revision 1.3  2004/07/19 13:18:03  yosizaki
  change system (sync & async)

  Revision 1.2  2004/07/12 13:04:00  yosizaki
  small fix (FS_COMMAND_GETPATH)

  Revision 1.1  2004/07/08 13:41:00  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
