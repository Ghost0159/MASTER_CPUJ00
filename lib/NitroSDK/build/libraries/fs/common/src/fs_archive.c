/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - libraries
  File:     fs_archive.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#include <nitro/mi.h>
#include <nitro/os.h>

#include <nitro/fs.h>

#include "../include/util.h"
#include "../include/command.h"


/* �A�[�J�C�u�͊�{�I�� ARM7 �Ɋ܂܂Ȃ� */
#if defined(FS_IMPLEMENT)


/*****************************************************************************/
/* variable */

/* �A�[�J�C�u���X�g */
static FSArchive *arc_list = NULL;

/* �J�����g�f�B���N�g���̈ʒu */
FSDirPos current_dir_pos;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         FSi_GetPackedName

  Description:  �����֐�.
                �w�蕶�������������ɂ��� u32 �Ƀp�b�N����.
                ������ FS_ARCHIVE_NAME_LEN_MAX ������ꍇ�� 0 ��Ԃ�.

  Arguments:    name             �p�b�N���閼�O�̕�����.
                name_len         name �̕�����.

  Returns:      �p�b�N���ꂽ���O.
 *---------------------------------------------------------------------------*/
static u32 FSi_GetPackedName(const char *name, int name_len)
{
    u32     ret = 0;
    if (name_len <= FS_ARCHIVE_NAME_LEN_MAX)
    {
        int     i = 0;
        for (; i < name_len; ++i)
        {
            u32     c = (u32)MI_ReadByte(name + i);
            if (!c)
                break;
            c = (u32)(c - 'A');
            if (c <= (u32)('Z' - 'A'))
                c = (u32)(c + 'a');
            else
                c = (u32)(c + 'A');
            ret |= (u32)(c << (i * 8));
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*
  Name:         FSi_ReadMemCallback

  Description:  �����֐�.
                �f�t�H���g�̃��������[�h�R�[���o�b�N.

  Arguments:    p_arc            ���삷��A�[�J�C�u.
                dst              �ǂݍ��ރ������̊i�[��.
                pos              �ǂݍ��݈ʒu.
                size             �ǂݍ��݃T�C�Y.

  Returns:      ��� FS_RESULT_SUCCESS.
 *---------------------------------------------------------------------------*/
static FSResult FSi_ReadMemCallback(FSArchive *p_arc, void *dst, u32 pos, u32 size)
{
    MI_CpuCopy8((const void *)FS_GetArchiveOffset(p_arc, pos), dst, size);
    return FS_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         FSi_WriteMemCallback

  Description:  �����֐�.
                �f�t�H���g�̃��������C�g�R�[���o�b�N.

  Arguments:    p_arc            ���삷��A�[�J�C�u.
                dst              �������ރ������̎Q�Ɛ�.
                pos              �������݈ʒu.
                size             �������݃T�C�Y.

  Returns:      ��� FS_RESULT_SUCCESS.
 *---------------------------------------------------------------------------*/
static FSResult FSi_WriteMemCallback(FSArchive *p_arc, const void *src, u32 pos, u32 size)
{
    MI_CpuCopy8(src, (void *)FS_GetArchiveOffset(p_arc, pos), size);
    return FS_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         FSi_ReadMemoryCore

  Description:  �����֐�.
                �v�����[�h�e�[�u������ǂݍ���.

  Arguments:    p_arc            ���삷��A�[�J�C�u.
                dst              �ǂݍ��ރ������̊i�[��.
                pos              �ǂݍ��݈ʒu.
                size             �ǂݍ��݃T�C�Y.

  Returns:      ��� FS_RESULT_SUCCESS.
 *---------------------------------------------------------------------------*/
static FSResult FSi_ReadMemoryCore(FSArchive *p_arc, void *dst, u32 pos, u32 size)
{
    (void)p_arc;
    MI_CpuCopy8((const void *)pos, dst, size);
    return FS_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         FSi_NextCommand

  Description:  �����֐�.
                ���ɏ������ׂ��R�}���h��I������.
                �񓯊��R�}���h���I�����ꂽ�炻�̃|�C���^��Ԃ�.
                NULL �ȊO���Ԃ��ꂽ�ꍇ�͂��̏�ŏ������K�v.

  Arguments:    p_arc            ���̃R�}���h���擾����A�[�J�C�u.

  Returns:      ���̏�ŏ�����K�v�Ƃ��鎟�̃R�}���h.
 *---------------------------------------------------------------------------*/
FSFile *FSi_NextCommand(FSArchive *p_arc)
{
    /* �L�����Z��, �R�}���h�I���̂��߂Ɋ��荞�݂��֎~ */
    OSIntrMode bak_psr = OS_DisableInterrupts();

    /* �܂�, �S�ẴL�����Z���v�����`�F�b�N */
    if (FSi_IsArchiveCanceling(p_arc))
    {
        FSFile *p, *q;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_CANCELING;
        for (p = p_arc->list.next; p; p = q)
        {
            q = p->link.next;
            if (FS_IsCanceling(p))
            {
                if (p_arc->list.next == p)
                    p_arc->list.next = q;
                FSi_ReleaseCommand(p, FS_RESULT_CANCELED);
                if (!q)
                    q = p_arc->list.next;
            }
        }
    }

    /* �T�X�y���h���łȂ���Ύ��̃R�}���h��I�� */
    if (!FSi_IsArchiveSuspending(p_arc) && !FS_IsArchiveSuspended(p_arc) && p_arc->list.next)
    {
        FSFile *p_file = p_arc->list.next;
        const BOOL is_start = !FSi_IsArchiveRunning(p_arc);
        if (is_start)
            p_arc->flag |= FS_ARCHIVE_FLAG_RUNNING;
        (void)OS_RestoreInterrupts(bak_psr);
        if (is_start)
        {
            /*
             * ACTIVATE ���b�Z�[�W�ʒm.
             * (�Ԃ�l�͈Ӗ��������Ȃ�)
             */
            if ((p_arc->proc_flag & FS_ARCHIVE_PROC_ACTIVATE) != 0)
                (void)(*p_arc->proc) (p_file, FS_COMMAND_ACTIVATE);
        }
        bak_psr = OS_DisableInterrupts();
        p_file->stat |= FS_FILE_STATUS_OPERATING;
        /* �����R�}���h�Ȃ�ҋ@�X���b�h�Ɉڏ� */
        if (FS_IsFileSyncMode(p_file))
        {
            OS_WakeupThread(p_file->queue);
            (void)OS_RestoreInterrupts(bak_psr);
            return NULL;
        }
        /* �񓯊��R�}���h�Ȃ�Ăяo�����ɏ��������� */
        else
        {
            (void)OS_RestoreInterrupts(bak_psr);
            return p_file;
        }
    }

    /* ���ǃR�}���h�����s���Ȃ��ꍇ�̓A�C�h����Ԃ� */
    if (FSi_IsArchiveRunning(p_arc))
    {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_RUNNING;
        if ((p_arc->proc_flag & FS_ARCHIVE_PROC_IDLE) != 0)
        {
            FSFile  tmp;
            FS_InitFile(&tmp);
            tmp.arc = p_arc;
            /*
             * IDLE ���b�Z�[�W�ʒm.
             * (�Ԃ�l�͈Ӗ��������Ȃ�)
             */
            (void)(*p_arc->proc) (&tmp, FS_COMMAND_IDLE);
        }
    }

    /* �T�X�y���h�ڍs�ɔ����A�C�h����Ԃł���Ύ��s�҂��N���� */
    if (FSi_IsArchiveSuspending(p_arc))
    {
        p_arc->flag &= ~FS_ARCHIVE_FLAG_SUSPENDING;
        p_arc->flag |= FS_ARCHIVE_FLAG_SUSPEND;
        OS_WakeupThread(&p_arc->stat_q);
    }
    (void)OS_RestoreInterrupts(bak_psr);

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         FSi_ExecuteAsyncCommand

  Description:  �����֐�.
                �񓯊��n�R�}���h�̎��s.
                �ŏ��� 1 ��̓��[�U�X���b�h���犄�荞�݋��ŌĂ΂��.
                �A�[�J�C�u�������I�ɓ��삷����肱���ŃR�}���h�������J��Ԃ�,
                1 ��ł��񓯊������ɂȂ�Α����� NotifyAsyncEnd() �ōs��.

                �����, �A�[�J�C�u���������� / �񓯊��Ő؂�ւ��ꍇ��
                NotifyAsyncEnd() �̌Ăяo�����ɒ��ӂ���K�v������.

  Arguments:    p_file           ���s����񓯊��R�}���h���i�[���� FSFile �\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FSi_ExecuteAsyncCommand(FSFile *p_file)
{
    FSArchive *const p_arc = p_file->arc;
    while (p_file)
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        /* �Ăяo�����œ����҂����Ȃ炻����N�����ď�����n�� */
        p_file->stat |= FS_FILE_STATUS_OPERATING;
        if (FS_IsFileSyncMode(p_file))
        {
            OS_WakeupThread(p_file->queue);
            (void)OS_RestoreInterrupts(bak_psr);
            break;
        }
        /* �����łȂ���΂����Ŕ񓯊����[�h�m�� */
        else
        {
            p_file->stat |= FS_FILE_STATUS_ASYNC;
        }
        (void)OS_RestoreInterrupts(bak_psr);
        /* �������񓯊��Ȃ�ꎞ�I�� */
        if (FSi_TranslateCommand(p_file, p_file->command) == FS_RESULT_PROC_ASYNC)
            break;
        /* ���ʂ����������Ȃ炱���ő�����I�� */
        p_file = FSi_NextCommand(p_arc);
    }
}

/*---------------------------------------------------------------------------*
  Name:         FSi_ExecuteSyncCommand

  Description:  �����֐�.
                �������[�h�ɂ�����R�}���h���s.

  Arguments:    p_file           ���s���铯���R�}���h���i�[���� FSFile �\����.

  Returns:      �R�}���h����������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL FSi_ExecuteSyncCommand(FSFile *p_file)
{
    FSFile *p_target;
    FSResult ret = FSi_TranslateCommand(p_file, p_file->command);
    FSi_ReleaseCommand(p_file, ret);
    p_target = FSi_NextCommand(p_file->arc);
    if (p_target)
        FSi_ExecuteAsyncCommand(p_target);
    return FS_IsSucceeded(p_file);
}

/*---------------------------------------------------------------------------*
  Name:         FSi_SendCommand

  Description:  �����֐�.
                �A�[�J�C�u�փR�}���h�𔭍s����.
                �N���^�C�~���O�̒����ƂƂ���, �����n�Ȃ炱���Ńu���b�L���O.

  Arguments:    p_file           �R�}���h�������w�肳�ꂽ FSFile �\����.
                command          �R�}���h ID.

  Returns:      �R�}���h����������� TRUE.
 *---------------------------------------------------------------------------*/
BOOL FSi_SendCommand(FSFile *p_file, FSCommandType command)
{
    FSArchive *const p_arc = p_file->arc;
    const int bit = (1 << command);

    /*
     * �t�@�C�����̂��̂̃X���b�h�Z�[�t�܂ł� SDK �ŕۏ؂��Ȃ�.
     * �܂�, ��Ԕ���͌Ăяo�����̃C���^�t�F�[�X�Ŋ����ς�.
     */
    p_file->command = command;
    p_file->error = FS_RESULT_BUSY;
    p_file->stat |= FS_FILE_STATUS_BUSY;

    {
        /* ���X�g�ǉ��ƋN������̂��߂Ɋ��荞�݂��֎~ */
        OSIntrMode bak_psr = OS_DisableInterrupts();

        /* �A�[�J�C�u���A�����[�h���Ȃ�L�����Z�� */
        if (p_arc->flag & FS_ARCHIVE_FLAG_UNLOADING)
        {
            FSi_ReleaseCommand(p_file, FS_RESULT_CANCELED);
            (void)OS_RestoreInterrupts(bak_psr);
            return FALSE;
        }

        /* ���X�g�ɒǉ� */
        if ((bit & FS_ARCHIVE_PROC_SYNC) != 0)
            p_file->stat |= FS_FILE_STATUS_SYNC;
        FSi_AppendToList(p_file, (FSFile *)&p_arc->list);

        /* �A�C�h�����Ȃ�A�[�J�C�u���N������ */
        if (!FS_IsArchiveSuspended(p_arc) && !FSi_IsArchiveRunning(p_arc))
        {
            p_arc->flag |= FS_ARCHIVE_FLAG_RUNNING;
            (void)OS_RestoreInterrupts(bak_psr);
            /*
             * ACTIVATE ���b�Z�[�W�ʒm.
             * (�Ԃ�l�͈Ӗ��������Ȃ�)
             */
            if ((p_arc->proc_flag & FS_ARCHIVE_PROC_ACTIVATE) != 0)
                (void)(*p_arc->proc) (p_file, FS_COMMAND_ACTIVATE);
            /* �񓯊����[�h�Ȃ�R�}���h�����s���Ă����߂� */
            bak_psr = OS_DisableInterrupts();
            p_file->stat |= FS_FILE_STATUS_OPERATING;
            if (!FS_IsFileSyncMode(p_file))
            {
                (void)OS_RestoreInterrupts(bak_psr);
                FSi_ExecuteAsyncCommand(p_file);
                return TRUE;
            }
            (void)OS_RestoreInterrupts(bak_psr);
        }

        /* �N�������񓯊����[�h�Ȃ��𗬂�ɔC���� */
        else if (!FS_IsFileSyncMode(p_file))
        {
            (void)OS_RestoreInterrupts(bak_psr);
            return TRUE;
        }

        /* �N�������������[�h�ł���΂����ŏ��ԑ҂� */
        else
        {
            do
            {
                OS_SleepThread(p_file->queue);
            }
            while (!(p_file->stat & FS_FILE_STATUS_OPERATING));
            (void)OS_RestoreInterrupts(bak_psr);
        }

    }

    /*
     * �������[�h�͑S�Ă����ɗ���, ���̏�Ŋ�������.
     * ReadFileAsync + WaitAsync �Ȃǂ������֗���.
     */
    return FSi_ExecuteSyncCommand(p_file);
}

/*---------------------------------------------------------------------------*
  Name:         FSi_EndArchive

  Description:  �S�ẴA�[�J�C�u���I�������ĉ������.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FSi_EndArchive(void)
{
    OSIntrMode bak_psr = OS_DisableInterrupts();
    FSArchive *volatile *p_list = &arc_list;
    for (;;)
    {
        FSArchive *p_arc = arc_list;
        if (!p_arc)
            break;
        arc_list = arc_list->next;
        (void)FS_UnloadArchive(p_arc);
        FS_ReleaseArchiveName(p_arc);
    }
    (void)OS_RestoreInterrupts(bak_psr);
}

/*---------------------------------------------------------------------------*
  Name:         FS_InitArchive

  Description:  �A�[�J�C�u�\���̂�������.

  Arguments:    p_arc            ����������A�[�J�C�u.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FS_InitArchive(FSArchive *p_arc)
{
    FS_ASSERT_ARG(p_arc, FALSE);
    MI_CpuClear8(p_arc, sizeof(FSArchive));
#if !defined(SDK_NO_THREAD)
    OS_InitThreadQueue(&p_arc->sync_q);
    OS_InitThreadQueue(&p_arc->stat_q);
#endif
}

/*---------------------------------------------------------------------------*
  Name:         FS_FindArchive

  Description:  �A�[�J�C�u������������.
                ��v���閼�O��������� NULL ��Ԃ�.

  Arguments:    name             ��������A�[�J�C�u���̕�����.
                name_len         name �̕�����.

  Returns:      �������Č��������A�[�J�C�u�̃|�C���^�� NULL.
 *---------------------------------------------------------------------------*/
FSArchive *FS_FindArchive(const char *name, int name_len)
{
    u32     pack = FSi_GetPackedName(name, name_len);
    OSIntrMode bak_psr = OS_DisableInterrupts();
    FSArchive *p_arc = arc_list;
    while (p_arc && (p_arc->name.pack != pack))
        p_arc = p_arc->next;
    (void)OS_RestoreInterrupts(bak_psr);

    return p_arc;
}

/*---------------------------------------------------------------------------*
  Name:         FS_RegisterArchiveName

  Description:  �A�[�J�C�u�����t�@�C���V�X�e���֓o�^��, �֘A�t����.
                �A�[�J�C�u���̂͂܂��t�@�C���V�X�e���Ƀ��[�h����Ȃ�.
                �A�[�J�C�u�� "rom" �̓t�@�C���V�X�e���ɗ\��ς�.

  Arguments:    p_arc            ���O���֘A�t����A�[�J�C�u.
                name             �o�^���閼�O�̕�����.
                name_len         name �̕�����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
BOOL FS_RegisterArchiveName(FSArchive *p_arc, const char *name, u32 name_len)
{
    FS_ASSERT_INIT(FALSE);
    FS_ASSERT_ARG(p_arc && name, FALSE);
    FS_ASSERT_ARC_FREE(p_arc, FALSE);

    {
        BOOL    ret = FALSE;
        OSIntrMode bak_intr = OS_DisableInterrupts();
        if (!FS_FindArchive(name, (s32)name_len))
        {
            FSArchive *p_tail = arc_list;
            if (!p_tail)
            {
                arc_list = p_arc;
                current_dir_pos.arc = p_arc;
                current_dir_pos.pos = 0;
                current_dir_pos.index = 0;
                current_dir_pos.own_id = 0;
            }
            else
            {
                while (p_tail->next)
                    p_tail = p_tail->next;
                p_tail->next = p_arc;
                p_arc->prev = p_tail;
            }
            p_arc->name.pack = FSi_GetPackedName(name, (s32)name_len);
            p_arc->flag |= FS_ARCHIVE_FLAG_REGISTER;
            ret = TRUE;
        }
        (void)OS_RestoreInterrupts(bak_intr);
        return ret;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FS_ReleaseArchiveName

  Description:  �o�^�ς݂̃A�[�J�C�u�����������.
                �t�@�C���V�X�e������A�����[�h����Ă���K�v������.

  Arguments:    p_arc            ���O���������A�[�J�C�u.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FS_ReleaseArchiveName(FSArchive *p_arc)
{
    FS_ASSERT_INIT(void);
    FS_ASSERT_ARG(p_arc, void);
    FS_ASSERT_ARC_NOT_ROM(p_arc, void);
    if (p_arc->name.pack)
    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        // ���X�g����ؒf.
        if (p_arc->next)
            p_arc->next->prev = p_arc->prev;
        if (p_arc->prev)
            p_arc->prev->next = p_arc->next;
        p_arc->name.pack = 0;
        p_arc->next = p_arc->prev = NULL;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_REGISTER;
        // ��������A�[�J�C�u�����J�����g�f�B���N�g���Ȃ�,
        //  �����I�� "rom" �̃��[�g�ֈړ�����.
        if (current_dir_pos.arc == p_arc)
        {
            current_dir_pos.arc = arc_list;
            current_dir_pos.pos = 0;
            current_dir_pos.index = 0;
            current_dir_pos.own_id = 0;
        }
        (void)OS_RestoreInterrupts(bak_psr);
    }
}

/*---------------------------------------------------------------------------*
  Name:         FS_LoadArchive

  Description:  �A�[�J�C�u���t�@�C���V�X�e���Ƀ��[�h����.
                ���łɃA�[�J�C�u���X�g�ɖ��O���o�^����Ă���K�v������.

  Arguments:    p_arc            ���[�h����A�[�J�C�u.
                base             �Ǝ��Ɏg�p�\�ȔC�ӂ� u32 �l.
                fat              FAT �e�[�u���̐擪�I�t�Z�b�g.
                fat_size         FAT �e�[�u���̃T�C�Y.
                fnt              FNT �e�[�u���̐擪�I�t�Z�b�g.
                fnt_size         FNT �e�[�u���̃T�C�Y.
                read_func        ���[�h�A�N�Z�X�R�[���o�b�N.
                write_func       ���C�g�A�N�Z�X�R�[���o�b�N.

  Returns:      �A�[�J�C�u�����������[�h������ TRUE.
 *---------------------------------------------------------------------------*/
BOOL FS_LoadArchive(FSArchive *p_arc, u32 base,
                    u32 fat, u32 fat_size,
                    u32 fnt, u32 fnt_size,
                    FS_ARCHIVE_READ_FUNC read_func, FS_ARCHIVE_WRITE_FUNC write_func)
{
    FS_ASSERT_INIT(FALSE);
    FS_ASSERT_ARG(p_arc, FALSE);
    FS_ASSERT_ARC_UNLOADED(p_arc, FALSE);

    // �x�[�X�I�t�Z�b�g��ݒ�.
    p_arc->base = base;
    p_arc->fat_size = fat_size;
    p_arc->fat = p_arc->fat_bak = fat;
    p_arc->fnt_size = fnt_size;
    p_arc->fnt = p_arc->fnt_bak = fnt;
    // �A�N�Z�X�R�[���o�b�N. (NULL �Ȃ烁��������)
    p_arc->read_func = read_func ? read_func : FSi_ReadMemCallback;
    p_arc->write_func = write_func ? write_func : FSi_WriteMemCallback;
    // �ŏ��̓v�����[�h�������.
    p_arc->table_func = p_arc->read_func;
    p_arc->load_mem = NULL;
    // 
    p_arc->flag |= FS_ARCHIVE_FLAG_LOADED;
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FS_UnloadArchive

  Description:  �A�[�J�C�u���t�@�C���V�X�e������A�����[�h����.
                ���ݏ������̃^�X�N���S�Ċ�������܂Ńu���b�L���O����.

  Arguments:    p_arc            �A�����[�h����A�[�J�C�u.

  Returns:      �A�[�J�C�u���������A�����[�h������ TRUE.
 *---------------------------------------------------------------------------*/
BOOL FS_UnloadArchive(FSArchive *p_arc)
{
    FS_ASSERT_INIT(FALSE);
    FS_ASSERT_ARG(p_arc, FALSE);

    {
        OSIntrMode bak_psr = OS_DisableInterrupts();
        /* ���[�h����Ă��Ȃ��Ȃ疳�� */
        if (FS_IsArchiveLoaded(p_arc))
        {
            /*
             * �e�[�u�����A�����[�h���Ȃ��܂܂���
             * ���������[�N�̉\�������邱�Ƃ��x��.
             */
            if (FS_IsArchiveTableLoaded(p_arc))
            {
                OS_TWarning("memory may leak. preloaded-table of archive \"%s\" (0x%08X)",
                            p_arc->name.ptr, p_arc->load_mem);
            }
            {
                FSFile *p, *q;
                /* �܂��T�X�y���h */
                BOOL    bak_state = FS_SuspendArchive(p_arc);
                /* �A�����[�h���t���O�𗧂ĂđS�R�}���h���L�����Z�� */
                p_arc->flag |= FS_ARCHIVE_FLAG_UNLOADING;
                for (p = p_arc->list.next; p; p = q)
                {
                    q = p->link.next;
                    FSi_ReleaseCommand(p, FS_RESULT_CANCELED);
                }
                p_arc->list.next = NULL;
                /* �O��Ԃ�ێ� */
                if (bak_state)
                    (void)FS_ResumeArchive(p_arc);
            }
            /* ���ۂ̃A�����[�h���� */
            p_arc->base = 0;
            p_arc->fat = 0;
            p_arc->fat_size = 0;
            p_arc->fnt = 0;
            p_arc->fnt_size = 0;
            p_arc->fat_bak = p_arc->fnt_bak = 0;
            p_arc->flag &= ~(FS_ARCHIVE_FLAG_CANCELING |
                             FS_ARCHIVE_FLAG_LOADED | FS_ARCHIVE_FLAG_UNLOADING);
        }
        (void)OS_RestoreInterrupts(bak_psr);
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         FS_LoadArchiveTables

  Description:  �A�[�J�C�u�� FAT + FNT ����������Ƀv�����[�h����.
                �w��T�C�Y�ȓ��̏ꍇ�̂ݓǂݍ��݂����s��, �K�v�T�C�Y��Ԃ�.

  Arguments:    p_arc            �e�[�u�����v�����[�h����A�[�J�C�u.
                p_mem            �e�[�u���f�[�^�̊i�[��o�b�t�@.
                max_size         p_mem �̃T�C�Y.

  Returns:      ���, ���v�̃e�[�u���T�C�Y��Ԃ�.
 *---------------------------------------------------------------------------*/
u32 FS_LoadArchiveTables(FSArchive *p_arc, void *p_mem, u32 max_size)
{
    FS_ASSERT_INIT(0);
    FS_ASSERT_ARG(p_arc, 0);

    {
        // �v�����[�h�T�C�Y�� 32 BYTE �A���C��.
        u32     total_size = ALIGN_BYTE(p_arc->fat_size + p_arc->fnt_size + 32, 32);
        if (total_size <= max_size)
        {
            // �T�C�Y���[���Ȃ烁�����փ��[�h.
            u8     *p_cache = (u8 *)ALIGN_BYTE((u32)p_mem, 32);
            FSFile  tmp;
            FS_InitFile(&tmp);
            /*
             * �e�[�u���̓��[�h�ł��Ȃ����Ƃ�����.
             * ���̏ꍇ��, ���X�e�[�u���ɃA�N�Z�X�ł��Ȃ��̂ŉ������Ȃ�.
             */
            // FAT ���[�h.
            if (FS_OpenFileDirect(&tmp, p_arc, p_arc->fat, p_arc->fat + p_arc->fat_size, (u32)~0))
            {
                if (FS_ReadFile(&tmp, p_cache, (s32)p_arc->fat_size) < 0)
                {
                    MI_CpuFill8(p_cache, 0x00, p_arc->fat_size);
                }
                (void)FS_CloseFile(&tmp);
            }
            p_arc->fat = (u32)p_cache;
            p_cache += p_arc->fat_size;
            // FNT ���[�h.
            if (FS_OpenFileDirect(&tmp, p_arc, p_arc->fnt, p_arc->fnt + p_arc->fnt_size, (u32)~0))
            {
                if (FS_ReadFile(&tmp, p_cache, (s32)p_arc->fnt_size) < 0)
                {
                    MI_CpuFill8(p_cache, 0x00, p_arc->fnt_size);
                }
                (void)FS_CloseFile(&tmp);
            }
            p_arc->fnt = (u32)p_cache;
            // ���蓖�Ă�ꂽ��������ۑ�.
            p_arc->load_mem = p_mem;
            // �ȍ~�̓e�[�u�����[�h�n�Ńv�����[�h������������.
            p_arc->table_func = FSi_ReadMemoryCore;
            p_arc->flag |= FS_ARCHIVE_FLAG_TABLE_LOAD;
        }
        return total_size;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FS_UnloadArchiveTables

  Description:  �A�[�J�C�u�̃v�����[�h���������������.

  Arguments:    p_arc            �v�����[�h���������������A�[�J�C�u.

  Returns:      �v�����[�h�������Ƃ��ă��[�U����^�����Ă����o�b�t�@.
 *---------------------------------------------------------------------------*/
void   *FS_UnloadArchiveTables(FSArchive *p_arc)
{
    FS_ASSERT_INIT(0);
    FS_ASSERT_ARG(p_arc, 0);

    {
        void   *ret = NULL;
        if (FS_IsArchiveLoaded(p_arc))
        {
            BOOL    bak_stat = FS_SuspendArchive(p_arc);
            if (FS_IsArchiveTableLoaded(p_arc))
            {
                p_arc->flag &= ~FS_ARCHIVE_FLAG_TABLE_LOAD;
                ret = p_arc->load_mem;
                p_arc->load_mem = NULL;
                p_arc->fat = p_arc->fat_bak;
                p_arc->fnt = p_arc->fnt_bak;
                p_arc->table_func = p_arc->read_func;
            }
            if (bak_stat)
                (void)FS_ResumeArchive(p_arc);
        }
        return ret;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FS_SuspendArchive

  Description:  �A�[�J�C�u�̏����@�\���̂��~����.
                ���ݎ��s���̏����������, ���̊�����ҋ@.

  Arguments:    p_arc            ��~����A�[�J�C�u.

  Returns:      �Ăяo���ȑO�ɃT�X�y���h��ԂłȂ���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL FS_SuspendArchive(FSArchive *p_arc)
{
    FS_ASSERT_INIT(0);
    FS_ASSERT_ARG(p_arc, 0);

    {
        /* ���łɃT�X�y���h���Ȃ疳�� */
        OSIntrMode bak_psr = OS_DisableInterrupts();
        const BOOL bak_stat = !FS_IsArchiveSuspended(p_arc);
        if (bak_stat)
        {
            /* ���s���Ȃ�t���O�𗧂ĂĒ�~��҂� */
            if (FSi_IsArchiveRunning(p_arc))
            {
                p_arc->flag |= FS_ARCHIVE_FLAG_SUSPENDING;
                do
                    OS_SleepThread(&p_arc->stat_q);
                while (FSi_IsArchiveSuspending(p_arc));
            }
            /* �A�C�h�����Ȃ璼�ڒ�~ */
            else
            {
                p_arc->flag |= FS_ARCHIVE_FLAG_SUSPEND;
            }
        }
        (void)OS_RestoreInterrupts(bak_psr);
        return bak_stat;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FS_ResumeArchive

  Description:  ��~���Ă����A�[�J�C�u�̏������ĊJ����.

  Arguments:    p_arc            �ĊJ����A�[�J�C�u.

  Returns:      �Ăяo���ȑO�ɃT�X�y���h��ԂłȂ���� TRUE.
 *---------------------------------------------------------------------------*/
BOOL FS_ResumeArchive(FSArchive *p_arc)
{
    FS_ASSERT_INIT(0);
    FS_ASSERT_ARG(p_arc, 0);

    {
        FSFile *p_target = NULL;
        /* �T�X�y���h�t���O�𗎂Ƃ� */
        OSIntrMode bak_irq = OS_DisableInterrupts();
        const BOOL bak_stat = !FS_IsArchiveSuspended(p_arc);
        if (!bak_stat)
        {
            p_arc->flag &= ~FS_ARCHIVE_FLAG_SUSPEND;
            /* �N���^�C�~���O�Ȃ�R�}���h�J�n */
            p_target = FSi_NextCommand(p_arc);
        }
        (void)OS_RestoreInterrupts(bak_irq);
        if (p_target)
            FSi_ExecuteAsyncCommand(p_target);
        return bak_stat;
    }
}

/*---------------------------------------------------------------------------*
  Name:         FS_SetArchiveProc

  Description:  �A�[�J�C�u�̃��[�U�v���V�[�W����ݒ肷��.
                proc == NULL �܂��� flags = 0 �Ȃ�
                �P�Ƀ��[�U�v���V�[�W���𖳌��ɂ���.

  Arguments:    p_arc            ���[�U�v���V�[�W����ݒ肷��A�[�J�C�u.
                proc             ���[�U�v���V�[�W��.
                flags            �v���V�[�W���փt�b�N����R�}���h�̃r�b�g�W��.

  Returns:      ���, ���v�̃e�[�u���T�C�Y��Ԃ�.
 *---------------------------------------------------------------------------*/
void FS_SetArchiveProc(struct FSArchive *p_arc, FS_ARCHIVE_PROC_FUNC proc, u32 flags)
{
    if (!flags)
        proc = NULL;
    else if (!proc)
        flags = 0;
    p_arc->proc = proc;
    p_arc->proc_flag = flags;
}

/*---------------------------------------------------------------------------*
  Name:         FS_NotifyArchiveAsyncEnd

  Description:  �񓯊��Ŏ��s���Ă����A�[�J�C�u�����̊�����ʒm���邽�߂�
                �A�[�J�C�u����������Ăяo��.

  Arguments:    p_arc            ������ʒm����A�[�J�C�u.
                ret              ��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FS_NotifyArchiveAsyncEnd(FSArchive *p_arc, FSResult ret)
{
    /* �񓯊��R�}���h���Ȃ犮�����Ӗ����� */
    if (FSi_IsArchiveAsync(p_arc))
    {
        FSFile *p_file = p_arc->list.next;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_ASYNC;
        FSi_ReleaseCommand(p_file, ret);
        /* ���̂��񓯊��R�}���h�Ȃ炱���Ŏ��s */
        p_file = FSi_NextCommand(p_arc);
        if (p_file)
            FSi_ExecuteAsyncCommand(p_file);
    }
    /* �����R�}���h�Ȃ�u���b�L���O���̃X���b�h�𕜋A */
    else
    {
        FSFile *p_file = p_arc->list.next;
        OSIntrMode bak_psr = OS_DisableInterrupts();
        p_file->error = ret;
        p_arc->flag &= ~FS_ARCHIVE_FLAG_IS_SYNC;
#if !defined(SDK_NO_THREAD)
        OS_WakeupThread(&p_arc->sync_q);
#endif
        (void)OS_RestoreInterrupts(bak_psr);
    }
}


#endif /* FS_IMPLEMENT */

/*---------------------------------------------------------------------------*
  $Log: fs_archive.c,v $
  Revision 1.34  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.33  2005/06/28 02:11:39  yosizaki
  remove some assertions.

  Revision 1.32  2005/06/17 13:07:41  yosizaki
  fix FS_WaitAsync. (add FS_FILE_STATUS_OPERATING)

  Revision 1.31  2005/05/30 04:17:17  yosizaki
  add FSi_EndArchive().
  add comments.

  Revision 1.30  2005/04/15 08:42:58  yosizaki
  fix assertion macros format. (remove ';' )

  Revision 1.29  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.28  2005/02/18 07:26:49  yasu
  Signed/Unsigned �ϊ��x���}��

  Revision 1.27  2005/01/26 02:58:51  yasu
  Copyright �\�L�̏C��

  Revision 1.26  2005/01/25 11:22:23  yosizaki
  fix around Suspend-state.

  Revision 1.25  2004/11/12 13:36:42  yosizaki
  fix around LoadTable (ignore if empty archive)

  Revision 1.24  2004/11/02 10:06:15  yosizaki
  fix typo.

  Revision 1.23  2004/09/02 11:03:00  yosizaki
  fix FS include directives.

  Revision 1.22  2004/07/23 08:29:52  yosizaki
  add cast to OS_LockCard.

  Revision 1.21  2004/07/19 13:17:36  yosizaki
  add all commands.

  Revision 1.20  2004/07/13 08:00:14  yosizaki
  fix FS_ResumeArchive

  Revision 1.19  2004/07/13 02:54:05  yosizaki
  add argument of FS_OpenFileDirect.

  Revision 1.18  2004/07/12 13:04:45  yosizaki
  change FS_COMMAND_READFILE & FS_COMMAND_WRITEFILE (add argument)

  Revision 1.17  2004/07/09 00:56:32  yosizaki
  fix argument of FS_COMMAND_IDLE procedure-message.

  Revision 1.16  2004/07/08 13:42:00  yosizaki
  change archive commands.

  Revision 1.15  2004/06/30 04:32:30  yosizaki
  implement user-procedure system.

  Revision 1.14  2004/06/22 01:48:48  yosizaki
  fix archive command operation.

  Revision 1.13  2004/06/07 10:26:33  yosizaki
  fix around archive asynchronous operation.

  Revision 1.12  2004/06/03 13:33:07  yosizaki
  fix around archive asynchronous operation.

  Revision 1.11  2004/06/01 10:17:56  yosizaki
  change around assertion code.

  Revision 1.10  2004/05/31 02:56:13  yosizaki
  change FSi_ASSERT_ARG() to FS_ASSERT_ARG().

  Revision 1.9  2004/05/21 10:55:27  yosizaki
  change OS_IsThreadAvailable() to SDK_NO_THREAD.

  Revision 1.8  2004/05/20 09:59:22  yosizaki
  fix asynchronous operation.

  Revision 1.7  2004/05/20 06:31:16  yosizaki
  add FS_ConvertPathToFileID.

  Revision 1.6  2004/05/19 02:30:14  yosizaki
  add FS_GetPathName().

  Revision 1.5  2004/05/17 08:32:26  yosizaki
  add comment.
  add assertion of each interface function.

  Revision 1.4  2004/05/12 05:22:55  yosizaki
  prepare a option "SDK_ARM7FS".

  Revision 1.3  2004/05/11 09:36:36  yosizaki
  fix FS_SeekFile().

  Revision 1.2  2004/05/11 04:30:57  yosizaki
  change archive system.

  Revision 1.1  2004/05/10 06:27:56  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
