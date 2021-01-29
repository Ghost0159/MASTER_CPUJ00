/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WFS - libraries
  File:     wfs_server.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#include <nitro/wfs/server.h>


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WFSi_NotifySegmentEvent

  Description:  WFS_EVENT_SERVER_SEGMENT_REQUEST�C�x���g��ʒm����.

  Arguments:    context          WFSServerContext�\����.
                argument         �C�x���g����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_NotifySegmentEvent(WFSServerContext *context, void *argument)
{
    if (context->thread_work)
    {
        (*context->thread_hook)(context->thread_work, argument);
    }
    else if (context->callback)
    {
        (*context->callback)(context, WFS_EVENT_SERVER_SEGMENT_REQUEST, argument);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_WBTCallback

  Description:  �T�[�o����WBT�C�x���g�ʒm.

  Arguments:    userdata         WFSServerContext�\����.
                uc               WBT�̃C�x���g����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_WBTCallback(void *userdata, WBTCommand *uc)
{
    WFSServerContext *work = (WFSServerContext *)userdata;
    const int aid = (int)MATH_CTZ(uc->peer_bmp);

    switch (uc->command)
    {

    default:
        WFS_DEBUG_OUTPUT(("invalid WBT callback (command = %d)", uc->command));
        break;

    case WBT_CMD_REQ_USER_DATA:
        /* �S���փ��b�Z�[�W���M�����������m�F */
        if (!uc->target_bmp)
        {
            work->msg_busy = FALSE;
        }
        break;

    case WBT_CMD_SYSTEM_CALLBACK:
        /* �C���W�P�[�V���� */

        switch (uc->event)
        {

        default:
            WFS_DEBUG_OUTPUT(("unknown WBT system callback (event = %d)", uc->event));
            break;

        case WBT_CMD_REQ_SYNC:
        case WBT_CMD_REQ_GET_BLOCK_DONE:
            break;

        case WBT_CMD_REQ_USER_DATA:
            /* ���b�Z�[�W��M�ʒm */
            {
                /* ��M�������b�Z�[�W��ۑ� */
                const WFSMessageFormat *const msg = (const WFSMessageFormat *)uc->user_data.data;
                work->recv_msg[aid] = *msg;

                switch (msg->type)
                {

                case WFS_MSG_LOCK_REQ:
                    /* �ǂݍ��݃��b�N�v�� */
                    {
                        const u32 offset = MI_LEToH32(msg->arg1);
                        const u32 length = MI_LEToH32_BITFIELD(24, msg->arg2);
                        const int packet = (int)((msg->packet_hi << 8) | msg->packet_lo);

                        WFS_DEBUG_OUTPUT(("WBT-MSG(LOCK):recv [offset=0x%08X, length=0x%08X] (AID=%d, packet=%d)",
                                         offset, length, aid, packet));
                        /* �p�P�b�g�T�C�Y�ύX���̏ꍇ�͋��ۉ��� */
                        if (work->is_changing ||
                            (packet != WBT_GetParentPacketLength(work->wbt) + WBT_PACKET_SIZE_MIN))
                        {
                            work->is_changing = TRUE;
                            work->deny_bitmap |= (1 << aid);
                        }
                        /* �����̃��b�N�̈�ƈ�v����Ȃ炻����Q�� */
                        else
                        {
                            WFSLockInfo *file = NULL;
                            int     index;
                            for (index = 0; (1 << index) <= work->use_bitmap; ++index)
                            {
                                if(((1 << index) & work->use_bitmap) != 0)
                                {
                                    if ((work->list[index].offset == offset) &&
                                        (work->list[index].length == length))
                                    {
                                        file = &work->list[index];
                                        ++file->ref;
                                        break;
                                    }

                                }

                            }
                            /* �V�K�̃��b�N�̈�ł���΋󂫃��X�g����m�� */
                            if (!file)
                            {
                                index = (int)MATH_CTZ((u32)~work->use_bitmap);
                                if (index < WFS_LOCK_HANDLE_MAX)
                                {
                                    PLATFORM_ENTER_CRITICALSECTION();
                                    work->use_bitmap |= (1 << index);
                                    file = &work->list[index];
                                    file->ref = 1;
                                    file->offset = offset;
                                    file->length = length;
                                    /* �V�K�t�@�C���Ȃ̂œo�^�ς݃��X�g�ֈڍs */
                                    WBT_RegisterBlockInfo(work->wbt, &file->info,
                                                          (u32)(WFS_LOCKED_BLOCK_INDEX + index),
                                                          NULL, NULL, (int)file->length);
                                    file->ack_seq = 0;
                                    PLATFORM_LEAVE_CRITICALSECTION();
                                    /* �擪�����͐�ǂݗv�����Ă��� */
                                    {
                                        WFSSegmentBuffer    segment[1];
                                        segment->offset = file->offset;
                                        segment->length = (u32)WBT_GetParentPacketLength(work->wbt);
                                        segment->buffer = NULL;
                                        WFSi_NotifySegmentEvent(work, segment);
                                    }
                                }
                                else
                                {
                                    OS_TPanic("internal-error (no available lock handles)");
                                }
                            }
                            work->ack_bitmap |= (1 << aid);
                            work->recv_msg[aid].arg1 = MI_HToLE32((u32)(WFS_LOCKED_BLOCK_INDEX + index));
                        }
                        work->busy_bitmap |= (1 << aid);
                    }
                    break;

                case WFS_MSG_UNLOCK_REQ:
                    /* CLOSEFILE �v�� */
                    {
                        PLATFORM_ENTER_CRITICALSECTION();
                        u32     id = MI_LEToH32(msg->arg1);
                        u32     index = id - WFS_LOCKED_BLOCK_INDEX;
                        if (index < WFS_LOCK_HANDLE_MAX)
                        {
                            WFSLockInfo *file = &work->list[index];
                            /* �S�Ă̎Q�Ƃ��������������� */
                            if (--file->ref <= 0)
                            {
                                (void)WBT_UnregisterBlockInfo(work->wbt, id);
                                work->use_bitmap &= ~(1 << index);
                            }
                        }
                        work->ack_bitmap |= (1 << aid);
                        PLATFORM_LEAVE_CRITICALSECTION();
                        WFS_DEBUG_OUTPUT(("WBT-MSG(UNLOCK):recv [id=0x%08X] (AID=%d)", id, aid));
                    }
                    break;

                }
            }
            break;

        case WBT_CMD_PREPARE_SEND_DATA:
            /* GETBLOCK �o�b�t�@�����v�� */
            {
                WBTPrepareSendDataCallback *const p_prep = &uc->prepare_send_data;
                u32     id = p_prep->block_id;
                p_prep->data_ptr = NULL;
                /* �����ȃt�@�C���ł���Ή��� */
                id -= WFS_LOCKED_BLOCK_INDEX;
                if (id < WFS_LOCK_HANDLE_MAX)
                {
                    WFSLockInfo *file = &work->list[id];
                    /* �O��̗v�������񉞓�, ����̗v���͎��񉞓� */
                    WFSSegmentBuffer    segment[1];
                    const u32 length = p_prep->own_packet_size;
                    const u32 current = file->ack_seq;
                    const u32 next = (u32)p_prep->block_seq_no;
                    file->ack_seq = next;
                    /* ����̏��� */
                    segment->offset = file->offset + length * next;
                    segment->length = length;
                    segment->buffer = NULL;
                    WFSi_NotifySegmentEvent(work, segment);
                    /* ����̗v�� */
                    segment->offset = file->offset + length * current;
                    segment->length = length;
                    segment->buffer = work->cache_hit_buf;
                    WFSi_NotifySegmentEvent(work, segment);
                    if (segment->buffer != NULL)
                    {
                        p_prep->block_seq_no = (s32)current;
                        p_prep->data_ptr = segment->buffer;
                    }
                }
            }
            break;

        }
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerConnectHook

  Description:  �T�[�o���̐ڑ��ʒm.

  Arguments:    context          WFSServerContext�\����.
                peer             �ڑ������ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerConnectHook(WFSServerContext *context, const WFSPeerInfo *peer)
{
    /*
     * �S�ẴN���C�A���g��WFS���g�p����Ƃ͌���Ȃ��̂�,
     * �����ł̐ڑ��ʒm�͖�������PacketRecv��ڑ��ʒm�Ƃ݂Ȃ�.
     */
    (void)context;
    (void)peer;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerDisconnectHook

  Description:  �T�[�o���̐ؒf�ʒm.

  Arguments:    context          WFSServerContext�\����.
                peer             �ؒf�����ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerDisconnectHook(WFSServerContext *context, const WFSPeerInfo *peer)
{
    const int bit = (1 << peer->aid);
    context->all_bitmap &= ~bit;
    (void)WBT_CancelCommand(context->wbt, bit);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerPacketSendHook

  Description:  �T�[�o���̃p�P�b�g���M�\�^�C�~���O�ʒm.

  Arguments:    context          WFSServerContext�\����.
                packet           ���M�p�P�b�g�ݒ�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerPacketSendHook(WFSServerContext *context, WFSPacketBuffer *packet)
{
    /* ���b�Z�[�W���M�\�ł���Ζ������̏������ĊJ */
    if (!context->msg_busy)
    {
        /* ���݂̗L���ȒʐM�󋵂��e��r�b�g�}�b�v�֔��f���� */
        context->ack_bitmap &= context->all_bitmap;
        context->sync_bitmap &= context->all_bitmap;
        context->busy_bitmap &= context->all_bitmap;
        context->deny_bitmap &= context->all_bitmap;
        /* �p�P�b�g�T�C�Y�ύX�͎󗝒��̏������S�Ċ�������܂ő҂� */
        if (context->is_changing && !context->use_bitmap)
        {
            /* �p�P�b�g�T�C�Y�ύX */
            context->is_changing = FALSE;
            (void)WBT_SetPacketLength(context->wbt, context->new_packet, WBT_PACKET_SIZE_MIN);
            /* ���T�C�Y�ɂ��v���ւ͂����ł܂Ƃ߂ċ��ۉ��� */
            if (context->deny_bitmap)
            {
                WFS_DEBUG_OUTPUT(("WBT-MSG(LOCK):deny [packet-length renewal] (BITMAP=%d)", context->deny_bitmap));
                (void)WFS_SendMessageLOCK_ACK(context->wbt, WFSi_WBTCallback, context->deny_bitmap, 0);
                context->msg_busy = TRUE;
                context->deny_bitmap = 0;
            }
        }
        /* �p�P�b�g�T�C�Y�ύX�ȑO�Ɏ󂯕t�����v���͂��̂܂ܑΉ����� */
        else if (context->ack_bitmap)
        {
            int     bitmap = context->ack_bitmap;
            WFSMessageFormat *msg = NULL;
            int     i;
            const int sync = context->sync_bitmap;
            const BOOL is_sync = (sync && ((bitmap & sync) == sync));
            /* �����w����󂯂��q�@�Q�������Έꊇ���� */
            if (is_sync)
            {
                bitmap = sync;
            }
            /* �����łȂ���Βʏ퉞�� */
            else
            {
                bitmap &= ~sync;
            }
            /* �Y�����鉞���\�q�@���������� */
            for (i = 0;; ++i)
            {
                const int bit = (1 << i);
                if (bit > bitmap)
                {
                    break;
                }
                if ((bit & bitmap) != 0)
                {
                    /* aid �̏��������ɉ����\�q�@������ */
                    if (!msg)
                    {
                        msg = &context->recv_msg[i];
                    }
                    /* ����̉����ł���Έꊇ���M. */
                    else if ((msg->type == context->recv_msg[i].type) &&
                             (msg->arg1 == context->recv_msg[i].arg1))
                    {
                    }
                    /* �����łȂ��q�@�ɑ΂��Ă͍���͕ۗ�. */
                    else
                    {
                        bitmap &= ~bit;
                    }
                }
            }
            /*
             * NOTE:
             *   �u�������w�肵���ɂ��ւ�炸�v�����e���قȂ�v
             *   �Ƃ����A�v�����s��̏�Ԃɂ����Ă�
             *   �E�P�ɂǂꂩ�̓������e���Y���Ă��邾���Ȃ̂�
             *   �E�Y���Ă���̂Ȃ�ǂ���ɉ�������Ε�������̂�
             *   �E���������Y���łȂ��v�����e���v���I�ɈقȂ�̂�
             *   �𔻒f���邱�Ƃ��s�\�Ȃ̂�,
             *   �u�x���𔭂������w������������v
             *   �Ƃ����Ώ��ŕ�������.
             */
            if (is_sync && (bitmap != sync))
            {
                context->sync_bitmap = 0;
                OS_TWarning("[WFS] specified synchronous-access failed! "
                            "(then synchronous-setting was reset)");
            }
            /* ����I�����ꂽ�����𑗐M */
            if (msg)
            {
                switch (msg->type)
                {
                case WFS_MSG_LOCK_REQ:
                    (void)WFS_SendMessageLOCK_ACK(context->wbt, WFSi_WBTCallback, bitmap,
                                                  MI_LEToH32(msg->arg1));
                    break;
                case WFS_MSG_UNLOCK_REQ:
                    (void)WFS_SendMessageUNLOCK_ACK(context->wbt, WFSi_WBTCallback, bitmap,
                                                    MI_LEToH32(msg->arg1));
                    context->busy_bitmap &= ~bitmap;
                    break;
                }
                context->msg_busy = TRUE;
                context->ack_bitmap &= ~bitmap;
            }
        }
    }

    /* �ЂƂƂ���̍ŐV�󋵂��X�V���Ă���WBT���Ăяo�� */
    packet->length = WBT_CallPacketSendHook(context->wbt, packet->buffer, packet->length, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerPacketRecvHook

  Description:  �T�[�o���̃p�P�b�g��M�ʒm.

  Arguments:    context          WFSServerContext�\����.
                packet           ���M���p�P�b�g���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerPacketRecvHook(WFSServerContext *context, const WFSPacketBuffer *packet)
{
    int aid = (int)MATH_CTZ((u32)packet->bitmap);
    const void *buffer = packet->buffer;
    int length = packet->length;
    /*
     * �e�@����̕s�v�ȃ|�[�g�ʐM�Ŗ��֌W�ȃN���C�A���g�ɉe����^���Ȃ��悤
     * �N���C�A���g����̎��ۂ̃p�P�b�g��M�ɂ���Đڑ������o����.
     */
    context->all_bitmap |= (1 << aid);
    WBT_CallPacketRecvHook(context->wbt, aid, buffer, length);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_InitServer

  Description:  WFS�T�[�o�R���e�L�X�g��������.

  Arguments:    context          WFSServerContext�\����.
                userdata         �R���e�L�X�g�Ɋ֘A�t����C�ӂ̃��[�U��`�l.
                callback         �V�X�e���C�x���g�ʒm�R�[���o�b�N.
                                 �s�v�Ȃ�NULL���w�肷��.
                allocator        �����Ŏg�p����A���P�[�^.
                packet           �e�@�̏����p�P�b�g�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_InitServer(WFSServerContext *context,
                    void *userdata, WFSEventCallback callback,
                    MIAllocator *allocator, int packet)
{
    /* ��{�ݒ�̏����� */
    MI_CpuClear8(context, sizeof(*context));
    context->userdata = userdata;
    context->callback = callback;
    context->allocator = allocator;

    /* ������Ԃ̏����� */
    context->new_packet = packet;
    context->table->buffer = NULL;
    context->table->length = 0;
    context->sync_bitmap = 0;
    context->ack_bitmap = 0;
    context->msg_busy = FALSE;
    context->all_bitmap = 1;
    context->busy_bitmap = 0;
    context->is_changing = FALSE;
    context->deny_bitmap = 0;
    context->use_bitmap = 0;
    context->thread_work = NULL;
    context->thread_hook = NULL;

    /* WBT �̏����� */
    WBT_InitContext(context->wbt, context, WFSi_WBTCallback);
    WBT_AddCommandPool(context->wbt, context->wbt_list,
                       sizeof(context->wbt_list) / sizeof(*context->wbt_list));
    WBT_StartParent(context->wbt, packet, WBT_PACKET_SIZE_MIN);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_EndServer

  Description:  WFS�T�[�o�R���e�L�X�g�����.

  Arguments:    context          WFSServerContext�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_EndServer(WFSServerContext *context)
{
    if (context->thread_work)
    {
        (*context->thread_hook)(context->thread_work, NULL);
        MI_CallFree(context->allocator, context->thread_work);
        context->thread_work = NULL;
    }
    WBT_ResetContext(context->wbt, NULL);
    if (context->table->buffer)
    {
        MI_CallFree(context->allocator, context->table->buffer);
        context->table->buffer = NULL;
        context->table->length = 0;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_RegisterServerTable

  Description:  �f�o�C�X����ROM�t�@�C���e�[�u�������[�h���ăT�[�o�ɓo�^.

  Arguments:    context          WFSServerContext�\����.
                device           NTR�o�C�i�����i�[����Ă���f�o�C�X.
                fatbase          NTR�o�C�i�����z�u����Ă���f�o�C�X���I�t�Z�b�g.
                overlay          �}�[�W�������I�[�o�[���C���܂�NTR�o�C�i����
                                 �z�u����Ă���f�o�C�X���I�t�Z�b�g.
                                 (������ROM�t�@�C���e�[�u�����}�[�W���Ȃ��Ȃ�
                                  ���̒l��fatbase�Ɠ���)

  Returns:      ROM�t�@�C���e�[�u���𐳂������[�h���o�^�ł����TRUE.
 *---------------------------------------------------------------------------*/
BOOL WFS_RegisterServerTable(WFSServerContext *context,
                             MIDevice *device, u32 fatbase, u32 overlay)
{
    BOOL    retval = FALSE;
    /* �����̃t�@�C���e�[�u����o�^���邱�Ƃ͂ł��Ȃ� */
    if (context->table->buffer)
    {
        OS_TWarning("table is already registered.\n");
    }
    /* �f�o�C�X����ROM�t�@�C���e�[�u�������[�h */
    else if (WFS_LoadTable(context->table, context->allocator, device, fatbase, overlay))
    {
        /* ROM�t�@�C���e�[�u����o�^ */
        WBT_RegisterBlockInfo(context->wbt, context->table_info,
                              WFS_TABLE_BLOCK_INDEX, NULL,
                              context->table->buffer,
                              (int)context->table->length);
        retval = TRUE;
    }
    return retval;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetServerPacketLength

  Description:  �T�[�o���M�p�P�b�g�T�C�Y���擾.

  Arguments:    context          WFSServerContext�\����.

  Returns:      ���ݐݒ肳��Ă���p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
int WFS_GetServerPacketLength(const WFSServerContext *context)
{
    return context->new_packet;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_SetServerPacketLength

  Description:  �T�[�o���M�p�P�b�g�T�C�Y��ݒ�.

  Arguments:    context          WFSServerContext�\����.
                length           �ݒ肷��p�P�b�g�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_SetServerPacketLength(WFSServerContext *context, int length)
{
    SDK_ASSERT(length >= WBT_PACKET_SIZE_MIN);
    {
        PLATFORM_ENTER_CRITICALSECTION();
        context->new_packet = length;
        context->is_changing = TRUE;
        PLATFORM_LEAVE_CRITICALSECTION();
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_SetServerSync

  Description:  �T�[�o���ŃA�N�Z�X���������N���C�A���g�Q��ݒ�.
                ���̊֐���, ����̃t�@�C���Q�𓯂������ŃA�N�Z�X���邱�Ƃ�
                ���炩�ɕۏ؂���Ă���N���C�A���g�Q�։����𓯊�����ɂ��
                WBT���C�u�����̓������������������I�ȓ]������������.
                ������, �����J�n�̃^�C�~���O���_���I�Ɉ��S�łȂ��ꍇ
                ����������ăf�b�h���b�N����_�ɒ��ӂ���K�v������.

  Arguments:    context          WFSServerContext�\����.
                bitmap           ����������N���C�A���g��AID�r�b�g�}�b�v.
                                 0���w�肷��Ɠ����͍s���Ȃ�. (�f�t�H���g)
                                 �ŉ��ʃr�b�g 1 �͏�ɖ��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_SetServerSync(WFSServerContext *context, int bitmap)
{
    PLATFORM_ENTER_CRITICALSECTION();
    context->sync_bitmap = (bitmap & ~1);
    PLATFORM_LEAVE_CRITICALSECTION();
}


/*---------------------------------------------------------------------------*
  $Log: wfs_server.c,v $
  Revision 1.6  2007/10/04 05:36:31  yosizaki
  fix about WFS_EndServer

  Revision 1.5  2007/06/14 13:15:27  yosizaki
  add hook to a thread-proc

  Revision 1.4  2007/06/11 10:23:32  yosizaki
  small fix.

  Revision 1.3  2007/06/11 06:40:00  yosizaki
  add WFS_GetServerPacketLength().

  Revision 1.2  2007/04/17 00:01:06  yosizaki
  rename some structures.

  Revision 1.1  2007/04/13 04:12:37  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
