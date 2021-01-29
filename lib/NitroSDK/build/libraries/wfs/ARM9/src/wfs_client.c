/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WFS - libraries
  File:     wfs_client.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/


#include <nitro/wfs/client.h>


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WFSi_NotifyEvent

  Description:  WFS�T�[�o�����̃C�x���g��ʒm����.

  Arguments:    context          WFSClientContext�\����.
                event            �ʒm����C�x���g���.
                argument         �C�x���g����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
inline static void WFSi_NotifyEvent(WFSClientContext *context,
                                    WFSEventType event, void *argument)
{
    if (context->callback)
    {
        context->callback(context, event, argument);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_ReallocBitmap

  Description:  �w�肳�ꂽ�T�C�Y��M�\�ȃr�b�g�}�b�v���Ċm��.

  Arguments:    context          WFSClientContext�\����.
                length           ��M�\��̃t�@�C���T�C�Y.
                                 ���̒l���w�肷��ƌ��݂̒l�ōĊm��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_ReallocBitmap(WFSClientContext *context, int length)
{
    if (length < 0)
    {
        length = (int)context->max_file_size;
        context->max_file_size = 0;
    }
    if (context->max_file_size < length)
    {
        const int packet = WBT_GetParentPacketLength(context->wbt);
        const u32 newsize = WBT_PACKET_BITMAP_SIZE((u32)length, packet);
        context->max_file_size = (u32)length;
        MI_CallFree(context->allocator, context->recv_pkt_bmp_buf);
        context->recv_pkt_bmp_buf = (u32 *)MI_CallAlloc(context->allocator,
                                    sizeof(u32) * newsize, sizeof(u32));
        context->recv_buf_packet_bmp_table.packet_bitmap[0] = context->recv_pkt_bmp_buf;
    }
    SDK_ASSERT(context->recv_pkt_bmp_buf);
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_ReceiveTableSequence

  Description:  ROM�t�@�C���e�[�u������M����V�[�P���X.
                �T�[�o�Ƃ̐ڑ������1�񂾂����s�����.

  Arguments:    userdata         WFSClientContext�\����.
                callback         WBT�����R�[���o�b�N����.
                                 �V�[�P���X�J�n����NULL���w�肵�ČĂяo��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_ReceiveTableSequence(void *userdata, WBTCommand *callback)
{
    WFSClientContext * const    context = (WFSClientContext *)userdata;
    WBTContext * const          wbt = context->wbt;

    if ((callback == NULL) || (callback->result == WBT_RESULT_SUCCESS))
    {
        BOOL    post = FALSE;
        const int   bitmap = 0x0001;
        /* �V�[�P���X�J�n => WBT-SYNC() �𔭍s���ăp�P�b�g�T�C�Y�𓯊� */
        if (callback == NULL)
        {
            WFS_DEBUG_OUTPUT(("WBT-SYNC():post"));
            post = WBT_PostCommandSYNC(wbt, bitmap, WFSi_ReceiveTableSequence);
        }
        /* WBT-SYNC ���� => WBT-INFO(0) �𔭍s����ROM�t�@�C���e�[�u���̃T�C�Y���擾 */
        else if (callback->event == WBT_CMD_RES_SYNC)
        {
            WFS_DEBUG_OUTPUT(("WBT-SYNC():done [server = %d, client = %d]",
                             callback->sync.peer_packet_size + WBT_PACKET_SIZE_MIN,
                             callback->sync.my_packet_size + WBT_PACKET_SIZE_MIN));
            WFS_DEBUG_OUTPUT(("WBT-INFO(0):post"));
            post = WBT_PostCommandINFO(wbt, bitmap, WFSi_ReceiveTableSequence,
                                       0, &context->block_info_table);
        }
        /* WBT-INFO(0) ���� => WBT-GET(0x20000) �𔭍s����ROM�t�@�C���e�[�u�����擾 */
        else if (callback->event == WBT_CMD_RES_GET_BLOCKINFO)
        {
            const int length = context->block_info_table.block_info[0]->block_size;
            WFS_DEBUG_OUTPUT(("WBT-INFO(0):done [table-length = %d]", length));
            context->table->length = (u32)length;
            context->table->buffer = (u8 *)MI_CallAlloc(context->allocator, (u32)length, 1);
            SDK_ASSERT(context->table->buffer);
            WFSi_ReallocBitmap(context, length);
            context->recv_buf_table.recv_buf[0] = context->table->buffer;
            WFS_DEBUG_OUTPUT(("WBT-GET(0x%08X):post", WFS_TABLE_BLOCK_INDEX));
            post = WBT_PostCommandGET(wbt, bitmap, WFSi_ReceiveTableSequence,
                                      WFS_TABLE_BLOCK_INDEX, context->table->length,
                                      &context->recv_buf_table,
                                      &context->recv_buf_packet_bmp_table);
        }
        /* WBT-GET(0x20000) ���� => �}�E���g���������C�x���g�ʒm */
        else if (callback->event == WBT_CMD_RES_GET_BLOCK)
        {
            WFS_DEBUG_OUTPUT(("WBT-GET(0x%08X):done [ready for mount]", WFS_TABLE_BLOCK_INDEX));
            WFS_ParseTable(context->table);
            context->fat_ready = TRUE;
            WFSi_NotifyEvent(context, WFS_EVENT_CLIENT_READY, NULL);
            post = TRUE;    /* �֋X�� */
        }
        /* WBT�R�}���h���s���s (WFS�����s��ɂ��R�}���h�L���[�s��) */
        if (!post)
        {
            WFS_DEBUG_OUTPUT(("internal-error (failed to post WBT command)"));
        }
    }
    /* ���炩�̓����G���[ */
     else
    {
        /* WFS�X�^�[�g�A�b�v���̃L�����Z�� (�����ł͓��ɉ������Ȃ��Ă悢) */
        if (callback->event == WBT_CMD_CANCEL)
        {
        }
        /* �\������WBT�G���[ (WFS�����s��ɂ���ԊǗ��̕s����) */
        else
        {
            WFS_DEBUG_OUTPUT(("internal-error (unexpected WBT result)"));
            WFS_DEBUG_OUTPUT(("  command = %d", callback->command));
            WFS_DEBUG_OUTPUT(("  event   = %d", callback->event));
            WFS_DEBUG_OUTPUT(("  result  = %d", callback->result));
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_ReadRomSequence

  Description:  �T�[�o����ROM�C���[�W����M����V�[�P���X.
                �N���C�A���g�����FS_ReadFile�֐��Ăяo���̂��тɎ��s�����.

  Arguments:    userdata         WFSClientContext�\����.
                callback         WBT�����R�[���o�b�N����.
                                 �V�[�P���X�J�n����NULL���w�肵�ČĂяo��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_ReadRomSequence(void *userdata, WBTCommand *callback)
{
    WFSClientContext * const    context = (WFSClientContext *)userdata;
    WBTContext * const          wbt = context->wbt;

    if ((callback == NULL) || (callback->result == WBT_RESULT_SUCCESS))
    {
        BOOL    post = FALSE;
        const int   bitmap = 0x0001;
        /* �V�[�P���X�J�n => WBT-MSG(LOCK) �𔭍s���ē]���͈͂����b�N */
        if (callback == NULL)
        {
            WFS_DEBUG_OUTPUT(("WBT-MSG(LOCK):post"));
            post = WFS_SendMessageLOCK_REQ(wbt, WFSi_ReadRomSequence, bitmap,
                                           context->request_region.offset + context->table->origin,
                                           context->request_region.length);
        }
        /* WBT-MSG() ���s���� (�T�[�o�̉�����҂̂ł����ł͉������Ȃ�) */
        else if (callback->event == WBT_CMD_RES_USER_DATA)
        {
            /* waiting for response from server... */
            post = TRUE;    /* �֋X�� */
        }
        else
        {
            const WFSMessageFormat *const msg = (const WFSMessageFormat *)callback->user_data.data;
            /* WBT-MSG(LOCK) ������M */
            if ((callback->event == WBT_CMD_REQ_USER_DATA) &&
                (msg->type == WFS_MSG_LOCK_ACK))
            {
                BOOL    accepted = (BOOL)MI_LEToH32(msg->arg2);
                /* ���ۉ��� => WBT-SYNC() ���Ĕ��s���ăp�P�b�g�T�C�Y�𓯊� */
                if (!accepted)
                {
                    WFS_DEBUG_OUTPUT(("WBT-MSG(LOCK):failed [packet-length renewal]"));
                    WFS_DEBUG_OUTPUT(("WBT-SYNC():post"));
                    post = WBT_PostCommandSYNC(wbt, bitmap, WFSi_ReadRomSequence);
                }
                /* ������ => WBT-GET(id) ���Ĕ��s���ău���b�N����M */
                else
                {
                    u32     id = MI_LEToH32(msg->arg1);
                    WFS_DEBUG_OUTPUT(("WBT-MSG(LOCK):done [lock-id = 0x%08X]", id));
                    context->block_id = id;
                    context->recv_buf_table.recv_buf[0] = context->request_buffer;
                    WFS_DEBUG_OUTPUT(("WBT-GET(0x%08X):post", id));
                    post = WBT_PostCommandGET(wbt, bitmap, WFSi_ReadRomSequence,
                                              context->block_id, context->request_region.length,
                                              &context->recv_buf_table,
                                              &context->recv_buf_packet_bmp_table);
                }
            }
            /* WBT-SYNC() ���� => �V�[�P���X���Ď��s (��i�����̍ċA�Ăяo���ƂȂ�) */
            else if (callback->event == WBT_CMD_RES_SYNC)
            {
                WFS_DEBUG_OUTPUT(("WBT-SYNC():done [server = %d, client = %d]",
                                 callback->sync.peer_packet_size + WBT_PACKET_SIZE_MIN,
                                 callback->sync.my_packet_size + WBT_PACKET_SIZE_MIN));
                WFSi_ReallocBitmap(context, -1);
                WFSi_ReadRomSequence(context, NULL);
                post = TRUE;    /* �֋X�� */
            }
            /* WBT-GET(id) ���� => WBT-MSG(UNLOCK, id) �𔭍s���ē]���͈͂���� */
            else if (callback->event == WBT_CMD_RES_GET_BLOCK)
            {
                u32     id = context->block_id;
                WFS_DEBUG_OUTPUT(("WBT-GET(0x%08X):done", id));
                WFS_DEBUG_OUTPUT(("WBT-MSG(UNLOCK,0x%08X):post", id));
                post = WFS_SendMessageUNLOCK_REQ(wbt, WFSi_ReadRomSequence, bitmap, id);
            }
            /* WBT-MSG(UNLOCK, id) ������M => ���[�h�����C�x���g�ʒm */
            else if ((callback->event == WBT_CMD_REQ_USER_DATA) &&
                (msg->type == WFS_MSG_UNLOCK_ACK))
            {
                u32     id = context->block_id;
                WFS_DEBUG_OUTPUT(("WBT-MSG(UNLOCK,0x%08X):done [read-operation completed]", id));
                context->request_buffer = NULL;
                {
                    WFSRequestClientReadDoneCallback callback = context->request_callback;
                    void   *argument = context->request_argument;
                    context->request_callback = NULL;
                    context->request_argument = NULL;
                    if (callback)
                    {
                        (*callback)(context, TRUE, argument);
                    }
                }
                post = TRUE;    /* �֋X�� */
            }
        }
        /* WBT�R�}���h���s���s (WFS�����s��ɂ��R�}���h�L���[�s��) */
        if (!post)
        {
            WFS_DEBUG_OUTPUT(("internal-error (failed to post WBT command)"));
        }
    }
    /* ���炩�̓����G���[ */
     else
    {
        /* WFS���[�h�������̃L�����Z�� (�����ł͓��ɉ������Ȃ��Ă悢) */
        if (callback->event == WBT_CMD_CANCEL)
        {
        }
        /* �\������WBT�G���[ (WFS�����s��ɂ���ԊǗ��̕s����) */
        else
        {
            WFS_DEBUG_OUTPUT(("internal-error (unexpected WBT result)"));
            WFS_DEBUG_OUTPUT(("  command = %d", callback->command));
            WFS_DEBUG_OUTPUT(("  event   = %d", callback->event));
            WFS_DEBUG_OUTPUT(("  result  = %d", callback->result));
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_WBTSystemCallback

  Description:  �N���C�A���g����WBT�V�X�e���R�[���o�b�N.

  Arguments:    userdata         WFSServerContext�\����.
                callback         WBT�̃C�x���g����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_WBTSystemCallback(void *userdata, WBTCommand *callback)
{
    WFSClientContext * const context = (WFSClientContext *)userdata;
    /* �T�[�o����̉������b�Z�[�W�� WFSi_ReadRomSequence �փ��_�C���N�g */
    if ((callback->event == WBT_CMD_REQ_USER_DATA) &&
        (context->request_buffer))
    {
        WFSi_ReadRomSequence(context, callback);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientConnectHook

  Description:  �N���C�A���g���̐ڑ��ʒm.

  Arguments:    context          WFSClientContext�\����.
                peer             �ڑ������ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallClientConnectHook(WFSClientContext *context, const WFSPeerInfo *peer)
{
    (void)context;
    (void)peer;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientDisconnectHook

  Description:  �N���C�A���g���̐ؒf�ʒm.

  Arguments:    context          WFSClientContext�\����.
                peer             �ؒf�����ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallClientDisconnectHook(WFSClientContext *context, const WFSPeerInfo *peer)
{
    (void)context;
    (void)peer;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientPacketSendHook

  Description:  �N���C�A���g���̃p�P�b�g���M�\�^�C�~���O�ʒm.

  Arguments:    context          WFSClientContext�\����.
                packet           ���M�p�P�b�g�ݒ�.

  Returns:      ���ۂ̃p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
void WFS_CallClientPacketSendHook(WFSClientContext *context, WFSPacketBuffer *packet)
{
    packet->length = WBT_CallPacketSendHook(context->wbt, packet->buffer, packet->length, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientPacketRecvHook

  Description:  �N���C�A���g���̃p�P�b�g��M�^�C�~���O�ʒm.

  Arguments:    context          WFSClientContext�\����.
                packet           ���M���p�P�b�g���.

  Returns:      ���ۂ̃p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
void WFS_CallClientPacketRecvHook(WFSClientContext *context, const WFSPacketBuffer *packet)
{
    int aid = (int)MATH_CTZ((u32)packet->bitmap);
    WBT_CallPacketRecvHook(context->wbt, aid, packet->buffer, packet->length);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_InitClient

  Description:  WFS�N���C�A���g�R���e�L�X�g��������.

  Arguments:    context          WFSClientContext�\����.
                userdata         �R���e�L�X�g�Ɋ֘A�t����C�ӂ̃��[�U��`�l.
                callback         �V�X�e���C�x���g�ʒm�R�[���o�b�N.
                                 �s�v�Ȃ�NULL���w�肷��.
                allocator        �����Ŏg�p����A���P�[�^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_InitClient(WFSClientContext *context,
                    void *userdata, WFSEventCallback callback,
                    MIAllocator *allocator)
{
    int     i;
    context->userdata = userdata;
    context->callback = callback;
    context->allocator = allocator;
    context->fat_ready = FALSE;
    /* WBT�p�ϐ��̏����� */
    for (i = 0; i < WBT_NUM_OF_AID; ++i)
    {
        context->block_info_table.block_info[i] = &context->block_info[i];
        context->recv_buf_table.recv_buf[i] = NULL;
        context->recv_buf_packet_bmp_table.packet_bitmap[i] = NULL;
    }
    context->recv_pkt_bmp_buf = NULL;
    context->max_file_size = 0;
    context->block_id = 0;
    context->request_buffer = NULL;
    context->table->length = 0;
    context->table->buffer = NULL;
    /* WBT �̏����� */
    WBT_InitContext(context->wbt, context, WFSi_WBTSystemCallback);
    WBT_AddCommandPool(context->wbt, context->wbt_list,
                       sizeof(context->wbt_list) / sizeof(*context->wbt_list));
}

/*---------------------------------------------------------------------------*
  Name:         WFS_EndClient

  Description:  WFS�N���C�A���g�R���e�L�X�g�����.

  Arguments:    context          WFSClientContext�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_EndClient(WFSClientContext *context)
{
    MI_CallFree(context->allocator, context->recv_pkt_bmp_buf);
    WBT_ResetContext(context->wbt, NULL);
    if (context->table->buffer)
    {
        MI_CallFree(context->allocator, context->table->buffer);
        context->table->buffer = NULL;
    }
    if (context->request_callback)
    {
        (*context->request_callback)(context->request_argument, FALSE, context->request_argument);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_StartClient

  Description:  WFS�N���C�A���g�R���e�L�X�g�̒ʐM���J�n.

  Arguments:    context          WFSClientContext�\����.
                peer             ���g�̐ڑ����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_StartClient(WFSClientContext *context, const WFSPeerInfo *peer)
{
    WBT_StartChild(context->wbt, peer->aid);
    WFSi_ReceiveTableSequence(context, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_RequestClientRead

  Description:  �T�[�o��ROM�C���[�W�ǂݍ��ݗv�����J�n.
                �������ɂ� WFS_EVENT_CLIENT_READ �ʒm����������.

  Arguments:    context          WFSClientContext�\����.
                buffer           �ǂݍ��݃f�[�^�̊i�[�惁����.
                offset           �f�o�C�X�̓ǂݍ��݊J�n�ʒu.
                length           �ǂݍ��݃T�C�Y.
                callback         �ǂݍ��݊����R�[���o�b�N.
                                 �s�v�Ȃ�NULL.
                arg              �ǂݍ��݊����R�[���o�b�N�ɗ^�������.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_RequestClientRead(WFSClientContext *context, void *buffer, u32 offset,
                           u32 length, WFSRequestClientReadDoneCallback callback,
                           void *arg)
{
    if (context->fat_ready)
    {
        context->request_buffer = buffer;
        context->request_region.offset = offset;
        context->request_region.length = length;
        context->request_callback = callback;
        context->request_argument = arg;
        WFSi_ReallocBitmap(context, (int)length);
        WFSi_ReadRomSequence(context, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetClientReadProgress

  Description:  ROM�C���[�W�ǂݍ��ݗv���̐i���󋵂��擾.

  Arguments:    context          WFSClientContext�\����.
                current          ��M�ς݃p�P�b�g�����擾����ϐ�.
                total            ��M�\��̃p�P�b�g�������擾����ϐ�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_GetClientReadProgress(WFSClientContext *context,int *current, int *total)
{
    WBT_GetDownloadProgress(context->wbt, context->block_id, 0, current, total);
}


/*---------------------------------------------------------------------------*
  $Log: wfs_client.c,v $
  Revision 1.2  2007/06/11 06:39:24  yosizaki
  change WFS_RequestClientRead().

  Revision 1.1  2007/04/13 04:12:37  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
