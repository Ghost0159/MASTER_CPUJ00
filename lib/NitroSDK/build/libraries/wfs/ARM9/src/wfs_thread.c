/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WFS - libraries
  File:     wfs_thread.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#include <nitro.h>


/*---------------------------------------------------------------------------*/
/* constants */

/*
 * ROM�L���b�V���̊e�ݒ�l.
 * [�y�[�W�T�C�Y]
 * - ROM�y�[�W�T�C�Y(512BYTE)�̐����{���]�܂���.
 * - �����������ROM�A�N�Z�X�̃I�[�o�[�w�b�h����������.
 * - �傫������ƃT�[�o���g��ROM�A�N�Z�X(FS/SND/...)���s����ɑj�Q�����.
 * [�y�[�W��]
 * - �傫����Γ]�������̈��艻(�򈫂ȒʐM���ł̍đ�����������)�Ɋ�^����.
 * - �A�N�Z�X�̋Ǐ����������ꍇ�ɂ̓N���C�A���g�������傫���l���]�܂���.
 */
#define	WFS_FILE_CACHE_SIZE	    1024UL
#define WFS_CACHE_PAGE_TOTAL    8


/*---------------------------------------------------------------------------*/
/* declarations */

typedef struct WFSServerThread
{
    WFSEventCallback    hook;
    MIDevice            device[1];
    FSFile              file[1];
    MICache             cache[1];
    u8                  cache_buf[MI_CACHE_BUFFER_WORKSIZE(
                                  WFS_FILE_CACHE_SIZE, WFS_CACHE_PAGE_TOTAL)];
    OSMessageQueue      msg_queue[1];
    OSMessage           msg_array[1];
    OSThread            thread[1];
    u8                  thread_stack[0x400];
}
WFSServerThread;


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WFSi_ReadRomCallback

  Description:  ROM�f�o�C�X�ǂݍ��݃R�[���o�b�N.

  Arguments:    userdata         WFSServerThread�\����.
                buffer           �]���惁����.
                offset           �]�����I�t�Z�b�g.
                length           �]���T�C�Y.

  Returns:      ���������0�ȏ�̒l, ���s����Ε��̒l.
 *---------------------------------------------------------------------------*/
static int WFSi_ReadRomCallback(void *userdata, void *buffer, u32 offset, u32 length)
{
    WFSServerThread * const thread = (WFSServerThread*)userdata;

    /* NITRO-SDK �ŗL : �擪32kB�̓A�N�Z�X�s�Ȃ̂Ń���������ǂݍ��� */
    if(offset < sizeof(CARDRomRegion))
    {
        const u8 *header = CARD_GetRomHeader();
        if (length > sizeof(CARDRomHeader) - offset)
        {
            length = sizeof(CARDRomHeader) - offset;
        }
        MI_CpuCopy8(header + offset, buffer, length);
    }
    else
    {
        if (offset < 0x8000)
        {
            u32     memlen = length;
            if (memlen > 0x8000 - offset)
            {
                memlen = 0x8000 - offset;
            }
            MI_CpuFill8(buffer, 0x00, length);
            offset += memlen;
            length -= memlen;
        }
        if (length > 0)
        {
            (void)FS_SeekFile(thread->file, (int)offset, FS_SEEK_SET);
            (void)FS_ReadFile(thread->file, buffer, (int)length);
        }
    }
    return (int)length;
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_WriteRomCallback

  Description:  ROM�f�o�C�X�������݃R�[���o�b�N. (�_�~�[)

  Arguments:    userdata         WFSServerThread�\����.
                buffer           �]���惁����.
                offset           �]�����I�t�Z�b�g.
                length           �]���T�C�Y.

  Returns:      ���������0�ȏ�̒l, ���s����Ε��̒l.
 *---------------------------------------------------------------------------*/
static int WFSi_WriteRomCallback(void *userdata, const void *buffer, u32 offset, u32 length)
{
    (void)userdata;
    (void)buffer;
    (void)offset;
    (void)length;
    return -1;
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_ServerThreadProc

  Description:  WFS�T�[�o�p�X���b�h�v���V�[�W��.

  Arguments:    arg             WFSServerThread�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_ServerThreadProc(void *arg)
{
    WFSServerThread *const thread = (WFSServerThread*)arg;
    for (;;)
    {
        BOOL    busy = FALSE;
        (void)OS_ReceiveMessage(thread->msg_queue, (OSMessage*)&busy, OS_MESSAGE_BLOCK);
        if (!busy)
        {
            break;
        }
        MI_LoadCache(thread->cache, thread->device);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_ThreadHook

  Description:  WFS�T�[�o�̃Z�O�����g�R�[���o�b�N�Ɏd�|����t�b�N�֐�.

  Arguments:    work            WFSServerThread�\����.
                argument        WFSSegmentBuffer�\����.
                                NULL�Ȃ�I���ʒm.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_ThreadHook(void *work, void *argument)
{
    WFSServerThread * const thread = (WFSServerThread *)work;
    WFSSegmentBuffer * const segment = (WFSSegmentBuffer*)argument;
    /* NULL�Ȃ�I���ʒm */
    if (!segment)
    {
        (void)OS_SendMessage(thread->msg_queue, (OSMessage)FALSE, OS_MESSAGE_BLOCK);
        OS_JoinThread(thread->thread);
        (void)FS_CloseFile(thread->file);
    }
    /* NULL�łȂ���΃Z�O�����g�v�� (�܂��͂��̏����ʒm) */
    else if (!MI_ReadCache(thread->cache, segment->buffer, segment->offset, segment->length))
    {
        /* �L���b�V���~�X�����ꍇ�̓X���b�h�֓ǂݍ��ݎw�� */
        segment->buffer = NULL; /* == "could not prepare immediately" */
        (void)OS_SendMessage(thread->msg_queue, (OSMessage)TRUE, OS_MESSAGE_NOBLOCK);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_ExecuteRomServerThread

  Description:  �w���ROM�t�@�C����z�M����悤WFS���C�u�����֓o�^��,
                ������ROM�A�N�Z�X�p�̃X���b�h�������I�ɋN������.
                ���̃X���b�h��WFS_EndServer�֐��̓����Ŏ����I�ɔj�������.

  Arguments:    context          WFSServerContext�\����.
                file             �z�M����t�@�C���V�X�e��������SRL�t�@�C��.
                                 �N���[���u�[�g�Ȃ�NULL���w�肷��.
                sharedFS         �t�@�C���V�X�e����e�q�ŋ��L������Ȃ�TRUE.
                                 ���̏ꍇ, �e�@���̂̎��t�@�C���V�X�e����
                                 file�̎��I�[�o�[���C�����𒊏o���Ēǉ�����
                                 �����t�@�C���V�X�e����z�M����.
                                 file��NULL���w�肵���ꍇ�̓N���[���u�[�g�Ȃ̂�
                                 ���̈����͖��������. (���TRUE�Ɖ��߂����)


  Returns:      ROM�t�@�C���̓o�^�ƃX���b�h�̐����ɐ��������TRUE.
 *---------------------------------------------------------------------------*/
BOOL WFS_ExecuteRomServerThread(WFSServerContext *context, FSFile *file, BOOL sharedFS)
{
    BOOL    retval = FALSE;

    WFSServerThread *thread = MI_CallAlloc(context->allocator, sizeof(WFSServerThread), 32);
    if (!thread)
    {
        OS_TWarning("failed to allocate memory enough to create internal thread.");
    }
    else
    {
        /* �P��ROM�^, �N���[���u�[�g�^, FS���L�^�̂����ꂩ�𔻒肷�� */
        u32     pos = file ? (FS_GetFileImageTop(file) + FS_GetPosition(file)) : 0;
        u32     fatbase = (u32)((file && !sharedFS) ? pos : 0);
        u32     overlay = (u32)(file ? pos : 0);
        /* ROM�A�N�Z�X�p�̃f�o�C�X�ƃL���b�V���������� */
        FS_InitFile(thread->file);
        if (!FS_CreateFileFromRom(thread->file, 0, 0x7FFFFFFF))
        {
            OS_TPanic("failed to create ROM-file!");
        }
        MI_InitDevice(thread->device, thread,
                      WFSi_ReadRomCallback, WFSi_WriteRomCallback);
        MI_InitCache(thread->cache, WFS_FILE_CACHE_SIZE,
                     thread->cache_buf, sizeof(thread->cache_buf));
        /* �t�@�C���e�[�u����o�^ */
        if (WFS_RegisterServerTable(context, thread->device, fatbase, overlay))
        {
            /* ��������������΃t�b�N��ݒ肵�ăX���b�h�N�� */
            context->thread_work = thread;
            context->thread_hook = WFSi_ThreadHook;
            OS_InitMessageQueue(thread->msg_queue, thread->msg_array, 1);
            OS_CreateThread(thread->thread, WFSi_ServerThreadProc, thread,
                            thread->thread_stack + sizeof(thread->thread_stack),
                            sizeof(thread->thread_stack), 15);
            OS_WakeupThreadDirect(thread->thread);
            retval = TRUE;
        }
        else
        {
            MI_CallFree(context->allocator, thread);
        }
    }
    return retval;
}


/*---------------------------------------------------------------------------*
  $Log: wfs_thread.c,v $
  Revision 1.1  2007/06/14 13:14:46  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
