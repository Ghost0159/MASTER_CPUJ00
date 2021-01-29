/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - wfs
  File:     wfs_common.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wfs_common.c,v $
  Revision 1.20  2007/06/14 13:57:26  yosizaki
  change to use official WFS library.

  Revision 1.19  2007/02/20 00:28:08  kitase_hirotake
  indent source

  Revision 1.18  2006/06/27 08:04:26  yosizaki
  fix about initialization.

  Revision 1.17  2006/05/22 02:35:24  yosizaki
  add temporary test code.

  Revision 1.16  2006/05/21 23:55:55  yosizaki
  change WFS_Init() to inline function.

  Revision 1.15  2006/02/27 13:17:10  yosizaki
  fix WFS_InitParent().
  add WBT_CancelCurrentCommand() on disconnection.
  fix WFSi_MoveList().

  Revision 1.14  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.13  2006/01/10 02:51:04  adachi_hiroaki
  16�i�\�L�̏o�͂ɂ͑S�Ė����I��0x�������B/([^xX])(%\d*[xX][^hH])/ -> \10x\2

  Revision 1.12  2005/12/26 07:15:30  yosizaki
  fix  about wfsi_work pointer.

  Revision 1.11  2005/11/14 08:32:35  yosizaki
  fix  about wfsi_work pointer.

  Revision 1.10  2005/10/28 11:25:15  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF �̒ǉ��ɑΉ�
  WM_STATECODE_PORT_INIT �̒ǉ��ɑΉ�

  Revision 1.9  2005/10/20 08:12:06  yosizaki
  fix WFS_Init().
  change to allocate wok-memory in WFS_Init.

  Revision 1.8  2005/05/09 04:38:07  yosizaki
  fix around WFS_End().

  Revision 1.7  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.6  2005/02/18 09:14:36  yosizaki
  fix around hidden warnings.

  Revision 1.5  2005/02/07 00:14:08  yosizaki
  fix WFSi_MoveList.
  ������Ԃ̍X�V���������̊����܂Œx�点��悤�C��.

  Revision 1.4  2005/02/04 11:02:15  yasu
  ����N�̏C��

  Revision 1.3  2005/02/01 03:56:08  yosizaki
  fix around WFSi_SendMessage.

  Revision 1.2  2004/12/02 04:09:52  terui
  Add port callback NULL clear on WFS_End.

  Revision 1.1  2004/11/16 01:12:55  yosizaki
  moved from /demos/wbt/wbt-fs.


 -[old log at /demos/wbt/wbt-fs/wfs]-

  Revision 1.23  2004/10/25 04:20:09  yosizaki
  add WFS_InitParent and WFS_InitChild.
  add some getter functions.

  Revision 1.22  2004/10/18 00:17:29  yosizaki
  fix WFSi_OnChildSystemCallback().

  Revision 1.21  2004/10/15 09:02:09  yosizaki
  merge from branch.

  Revision 1.20  2004/10/13 02:17:39  yosizaki
  fix WFS_end in child-mode. (add async-cancelling)

  Revision 1.19  2004/09/29 07:31:45  yosizaki
  change WFS_Init.
  change assertion of getter functions.

  Revision 1.18  2004/09/29 07:11:38  yosizaki
  merge from branch.

  Revision 1.17  2004/09/15 09:10:25  yosizaki
  add Seek&Read.

  Revision 1.16  2004/09/15 01:11:06  yosizaki
  add WFS_SetPacketSize().

  Revision 1.15  2004/09/11 08:11:01  yosizaki
  fix around recv_idle.

  Revision 1.14  2004/09/11 04:13:10  yosizaki
  unify MP-size definition.

  Revision 1.13  2004/09/09 14:15:48  yosizaki
  fix WFS_GetCurrentDownloadProgress().

  Revision 1.12  2004/09/09 05:03:18  yosizaki
  fix file page-cache.

  Revision 1.11  2004/09/07 04:08:59  yosizaki
  change to delay ack-timing.

  Revision 1.10  2004/09/07 03:05:51  yosizaki
  source code Maintenance.

  Revision 1.9  2004/09/06 08:47:48  yosizaki
  fix archive. (around result of FS_ReadFile())

  Revision 1.8  2004/09/06 06:23:41  yosizaki
  add double-buffering system.

  Revision 1.7  2004/09/04 13:23:55  yosizaki
  change to load TABLE-memory with WFSi_Alloc.

  Revision 1.6  2004/09/04 08:29:53  yosizaki
  change around PREPARE-response.

  Revision 1.5  2004/09/04 01:45:21  yosizaki
  delete obsolete codes and waste parameters.

  Revision 1.4  2004/09/03 12:24:05  yosizaki
  change names. (WBT_RegisterUserDefinedBlock, WBT_UnRegisterBlock)

  Revision 1.3  2004/09/03 11:35:48  yosizaki
  support page-cache (test version)

  Revision 1.2  2004/09/03 09:58:38  yosizaki
  add wfs_thread.c

  Revision 1.1  2004/09/03 01:10:17  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "wfs.h"


/*---------------------------------------------------------------------------*/
/* constants */

/* ���C�����X�C�x���g */
typedef enum WirelessEvent
{
    WIRELESS_EVENT_END,             /* �I�� */
    WIRELESS_EVENT_LINKUP,          /* MP�ʐM�J�n */
    WIRELESS_EVENT_CONNECTED,       /* �ڑ��ʒm */
    WIRELESS_EVENT_DISCONNECTED,    /* �ؒf�ʒm */
    WIRELESS_EVENT_PACKET_SEND,     /* �p�P�b�g���M */
    WIRELESS_EVENT_PACKET_RECV      /* �p�P�b�g��M */
}
WirelessEvent;


/*---------------------------------------------------------------------------*/
/* declarations */

typedef void  (*WirelessEventCallback)(void *userdata, WirelessEvent event, void *argument);
typedef struct StaticWork
{
    /* ������Ԃ̊Ǘ� */
    WFSState                state;
    WFSStateCallback        state_func;
    u32                     available:1;
    u32                     mp_availale:1;
    u32                     initialized:1;
    u32                     linkup_done:1;
    u32                     mp_operating:1;
    u32                     debug_enable:1;
    u32                     flags:26;
    /* �A���P�[�^�`���ϊ��p��� */
    MIAllocator             allocator[1];
    WFSAllocator            user_alloc;
    void                   *user_alloc_arg;
    /* �e�q�ʂ�WFS�����̂��߂̓��I������ */
    WirelessEventCallback   callback;
    void                   *context;
    WFSServerContext       *server;
    WFSClientContext       *client;
    u8                      padding[12];
    /* MP�ʐM�̂��߂̐ݒ� */
    int                     port;
    u8                      packet_buf[1024] ATTRIBUTE_ALIGN(32);
    WMStatus                status_buf[1] ATTRIBUTE_ALIGN(32);
}
StaticWork;


/*---------------------------------------------------------------------------*/
/* variables */

static StaticWork           static_work[1] ATTRIBUTE_ALIGN(32);


/*---------------------------------------------------------------------------*/
/* functions */

static void WFSi_PortSendCallback(void *callback);

/*---------------------------------------------------------------------------*
  Name:         InternalAlloc

  Description:  �T���v���f���̋��d�l�A���P�[�^��ϊ���������֐�.

  Arguments:    userdata         StaticWork�\����.
                length           �m�ۂ���T�C�Y.
                alignment        �K�v�ȋ��E���� (2�ׂ̂���ł���K�v������)

  Returns:      �m�ۂ��ꂽ�������܂���NULL.
 *---------------------------------------------------------------------------*/
static void *InternalAlloc(void *userdata, u32 length, u32 alignment)
{
    StaticWork *work = (StaticWork*)userdata;
    u32     newlen;
    void   *newptr;
    void   *aligned_ptr;
    alignment = MATH_ROUNDUP(alignment, sizeof(u32));
    newlen = MATH_ROUNDUP(length, alignment) + alignment;
    newptr = (*work->user_alloc)(work->user_alloc_arg, newlen, NULL);
    if (newptr)
    {
        aligned_ptr = (void*)MATH_ROUNDUP((u32)newptr + 1, alignment);
        ((u32*)aligned_ptr)[-1] = (u32)aligned_ptr - (u32)newptr;
    }
    return aligned_ptr;
}

/*---------------------------------------------------------------------------*
  Name:         InternalFree

  Description:  �T���v���f���̋��d�l�A���P�[�^��ϊ���������֐�.

  Arguments:    userdata         StaticWork�\����.
                buffer           ������郁����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InternalFree(void *userdata, void *buffer)
{
    StaticWork *work = (StaticWork*)userdata;
    if (buffer)
    {
        void   *rare_ptr = (u8*)buffer - ((u32*)buffer)[-1];
        (void)(*work->user_alloc)(work->user_alloc_arg, 0, rare_ptr);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NotifyWirelessEvent

  Description:  ���C�����X�C�x���g�ʒm.

  Arguments:    work             StaticWork�\����
                event            �ʒm����C�x���g
                argument         �C�x���g����

  Returns:      None.
 *---------------------------------------------------------------------------*/
SDK_INLINE
void NotifyWirelessEvent(StaticWork *work, WirelessEvent event, void *argument)
{
    (*work->callback)(work->context, event, argument);
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_NotifySend

  Description:  �p�P�b�g���M�ʒm.

  Arguments:    work             StaticWork�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_NotifySend(StaticWork *work)
{
    if (work->mp_availale && work->linkup_done && !work->mp_operating)
    {
        /* ���݉\�ȍő�MP�ݒ�Ńp�P�b�g���M�C�x���g��ʒm */
        WFSPacketBuffer packet[1];
        packet->buffer = work->packet_buf;
        packet->bitmap = (work->status_buf->aid == 0) ? 0xFFFE : 0x0001;
        packet->length = (work->status_buf->aid == 0) ?
            work->status_buf->mp_parentMaxSize :
            work->status_buf->mp_childMaxSize;
        NotifyWirelessEvent(work, WIRELESS_EVENT_PACKET_SEND, packet);
        /* ���M���ׂ��p�P�b�g���ݒ肳��Ă���Ύ��ۂɑ��M */
        if (packet->length > 0)
        {
            int     ret = WM_SetMPDataToPortEx(WFSi_PortSendCallback,
                                               work,
                                               (const u16 *)packet->buffer,
                                               (u16)packet->length,
                                               (u16)packet->bitmap,
                                               (u16)work->port,
                                               WM_PRIORITY_LOW);
            work->mp_operating = (ret == WM_ERRCODE_OPERATING);
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_NotifyLinkup

  Description:  �����N�A�b�v�ʒm.

  Arguments:    wireless         StaticWork�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_NotifyLinkup(StaticWork *work)
{
    if (work->mp_availale && work->initialized && !work->linkup_done)
    {
        WFSPeerInfo peer[1];
        peer->aid = work->status_buf->aid;
        MI_CpuCopy8(work->status_buf->MacAddress, peer->mac, 6);
        NotifyWirelessEvent(work, WIRELESS_EVENT_LINKUP, peer);
        work->linkup_done = TRUE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_PortSendCallback

  Description:  SetMPData ����������.

  Arguments:    arg              WMPortSendCallback�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_PortSendCallback(void *arg)
{
    const WMPortSendCallback * const callback = (const WMPortSendCallback *)arg;
    StaticWork * const work = callback->arg;
    if (work->mp_availale)
    {
        work->mp_operating = FALSE;
        WFSi_NotifySend(work);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_PortCallback

  Description:  �|�[�g��M�R�[���o�b�N.

  Arguments:    arg              WMPortRecvCallback�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_PortCallback(void *arg)
{
    const WMPortRecvCallback *const callback = (const WMPortRecvCallback *)arg;
    StaticWork * const              work = callback->arg;
    if (work->mp_availale && work->linkup_done)
    {
        /* WFS�ɍ����R�[���o�b�N�`���֕ϊ� */
        WFSPeerInfo     peer[1];
        WFSPacketBuffer packet[1];
        peer->aid = callback->aid;
        MI_CpuCopy8(callback->macAddress, peer->mac, 6);
        packet->buffer = (u8 *)callback->data;
        packet->length = callback->length;
        packet->bitmap = (1 << peer->aid);
        switch (callback->state)
        {
        case WM_STATECODE_CONNECTED:
            NotifyWirelessEvent(work, WIRELESS_EVENT_CONNECTED, peer);
            break;

        case WM_STATECODE_DISCONNECTED:
        case WM_STATECODE_DISCONNECTED_FROM_MYSELF:
            NotifyWirelessEvent(work, WIRELESS_EVENT_DISCONNECTED, peer);
            break;

        case WM_STATECODE_PORT_RECV:
            NotifyWirelessEvent(work, WIRELESS_EVENT_PACKET_RECV, packet);
            /* MP��~���ł������Ȃ�΂��̋@��ɋN�� */
            WFSi_NotifySend(work);
            break;

        default:
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFSi_InitCommon

  Description:  WFS�̐e�q���ʏ���������.

  Arguments:    callback         ��Ԓʒm�R�[���o�b�N.
                allocator        �������A���P�[�^.
                allocator_arg    �A���P�[�^����.
                port             �g�p����|�[�g�ԍ�.
                worksize         �e�q�ʂ̃��[�N�������T�C�Y.
                wireless_proc    �e�q�ʂ̃��C�����X�C�x���g�n���h��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WFSi_InitCommon(WFSStateCallback callback,
                            WFSAllocator allocator, void *allocator_arg,
                            int port, u32 worksize,
                            WirelessEventCallback wireless_proc)
{
    /* �e�q�Ƃ�FS���C�u�������K�v */
    if (!FS_IsAvailable())
    {
        OS_TWarning("FS is initialized by WFS (with FS_DMA_NOT_USE)");
        FS_Init(FS_DMA_NOT_USE);
    }
    {
        OSIntrMode  prev_irq = OS_DisableInterrupts();
        if (!static_work->available)
        {
            /* �A���P�[�^�������� */
            static_work->user_alloc = allocator;
            static_work->user_alloc_arg = allocator_arg;
            MI_InitAllocator(static_work->allocator, static_work,
                             InternalAlloc, InternalFree);
            /* �e�q�ʂ̓��I���[�N���������m�� */
            static_work->context = MI_CallAlloc(static_work->allocator, worksize, 32);
            if (!static_work->context)
            {
                OS_TPanic("cannot allocate enough memory for WFS");
            }
            /* ������Ԃ������� */
            static_work->available = TRUE;
            static_work->state = WFS_STATE_STOP;
            static_work->state_func = callback;
            static_work->debug_enable = FALSE;
            static_work->mp_availale = FALSE;
            static_work->initialized = FALSE;
            static_work->linkup_done = FALSE;
            static_work->mp_operating = FALSE;
            static_work->callback = wireless_proc;
            static_work->port = port;
            static_work->status_buf->aid = 0;
            static_work->status_buf->mp_parentMaxSize = 0;
            static_work->status_buf->mp_childMaxSize = 0;
        }
        (void)OS_RestoreInterrupts(prev_irq);
    }
}


/*---------------------------------------------------------------------------*
  Name:         ParentWFSEvent

  Description:  WFS�T�[�o�C�x���g�ʒm.

  Arguments:    userdata         ���[�U��`����.
                event            �C�x���g���.
                argument         �C�x���g�ŗL�̈���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentWFSEvent(void *userdata, WFSEventType event, void *argument)
{
    /* WFS_ExecuteRomServerThread�֐����g�p���Ă���̂œ��ɂ��ׂ����Ƃ͖��� */
    (void)userdata;
    (void)event;
    (void)argument;
}

/*---------------------------------------------------------------------------*
  Name:         ParentWirelessEvent

  Description:  ���C�����X�h���C�o����̃C�x���g�ʒm.

  Arguments:    userdata         ���[�U��`����.
                event            �C�x���g���.
                argument         �C�x���g�ŗL�̈���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentWirelessEvent(void *userdata, WirelessEvent event, void *argument)
{
    WFSServerContext * const context = (WFSServerContext *)userdata;
    switch (event)
    {
    case WIRELESS_EVENT_END:
        WFS_EndServer(context);
        break;

    case WIRELESS_EVENT_LINKUP:
        /* �������� READY �X�e�[�g�� */
        static_work->state = WFS_STATE_READY;
        if (static_work->state_func)
        {
            (*static_work->state_func) (NULL);
        }
        break;

    case WIRELESS_EVENT_CONNECTED:
        WFS_CallServerConnectHook(context, (const WFSPeerInfo*)argument);
        break;

    case WIRELESS_EVENT_DISCONNECTED:
        WFS_CallServerDisconnectHook(context, (const WFSPeerInfo*)argument);
        break;

    case WIRELESS_EVENT_PACKET_SEND:
        WFS_CallServerPacketSendHook(context, (WFSPacketBuffer *)argument);
        break;

    case WIRELESS_EVENT_PACKET_RECV:
        WFS_CallServerPacketRecvHook(context, (const WFSPacketBuffer *)argument);
        break;

    default:
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChildWFSEvent

  Description:  WFS�N���C�A���g�C�x���g�ʒm.

  Arguments:    userdata         ���[�U��`����.
                event            �C�x���g���.
                argument         �C�x���g�ŗL�̈���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ChildWFSEvent(void *userdata, WFSEventType event, void *argument)
{
    switch (event)
    {
    case WFS_EVENT_CLIENT_READY:
        /* ROM�t�@�C���e�[�u���̎�M���� */
        {
            /* ���݂� "rom" �A�[�J�C�u��WFS�֒u�� */
            WFSClientContext * const context = (WFSClientContext*)userdata;
            WFS_ReplaceRomArchive(context);
            /* WFS�����p�\�ɂȂ������Ƃ��A�v���P�[�V�����֒ʒm */
            static_work->state = WFS_STATE_READY;
            if (static_work->state_func)
            {
                (*static_work->state_func) (NULL);
            }
        }
        break;
    default:
        break;
    }
    (void)argument;
}

/*---------------------------------------------------------------------------*
  Name:         ChildWirelessEvent

  Description:  ���C�����X�h���C�o����̃C�x���g�ʒm.

  Arguments:    userdata         ���[�U��`����.
                event            �C�x���g���.
                argument         �C�x���g�ŗL�̈���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ChildWirelessEvent(void *userdata, WirelessEvent event, void *argument)
{
    WFSClientContext * const context = (WFSClientContext *)userdata;
    switch (event)
    {
    case WIRELESS_EVENT_END:
        WFS_EndClient(context);
        break;

    case WIRELESS_EVENT_LINKUP:
        WFS_StartClient(context, (const WFSPeerInfo*)argument);
        break;

    case WIRELESS_EVENT_CONNECTED:
        WFS_CallClientConnectHook(context, (const WFSPeerInfo*)argument);
        break;

    case WIRELESS_EVENT_DISCONNECTED:
        WFS_CallClientDisconnectHook(context, (const WFSPeerInfo*)argument);
        break;

    case WIRELESS_EVENT_PACKET_SEND:
        WFS_CallClientPacketSendHook(context, (WFSPacketBuffer*)argument);
        break;

    case WIRELESS_EVENT_PACKET_RECV:
        WFS_CallClientPacketRecvHook(context, (const WFSPacketBuffer*)argument);
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_InitParent

  Description:  initialize Wireless File System parent parameters.

  Arguments:    port             available WM port for WFS
                callback         callback of vavious state-notifications.
                allocator        dynamic memory allocator for WFS
                allocator_arg    argument of 'allocator'
                parent_packet    available WM packet for WFS
                p_rom            target file which directs SRL-program
                                 to export FAT/FNT and OVT.
                                 if specified NULL, use own.
                use_parent_fs    if TRUE, ignore p_rom's FAT/FNT and use own.
                                 this setting enables both independent
                                 program to share FAT/FNT (except OVT).
                                 if p_rom is NULL, always as TRUE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_InitParent(int port, WFSStateCallback callback,
                    WFSAllocator allocator, void *allocator_arg, int parent_packet,
                    FSFile *p_rom, BOOL use_parent_fs)
{
    /* ���ʂ̏����� */
    WFSi_InitCommon(callback, allocator, allocator_arg, port,
                    sizeof(WFSServerContext), ParentWirelessEvent);
    /* WFS�T�[�o������ */
    {
        WFSServerContext   *server = (WFSServerContext*)static_work->context;
        static_work->server = server;
        WFS_InitServer(server, server, ParentWFSEvent, static_work->allocator, parent_packet);
        if (!WFS_ExecuteRomServerThread(server, p_rom, use_parent_fs))
        {
            OS_TPanic("WFS_ExecuteRomServerThread() failed! (specified SRL file seems to be wrong)");
        }
    }
    static_work->state = WFS_STATE_IDLE;
    static_work->initialized = TRUE;
    /* ���łɏ��������ɒʐM�\��ԂɂȂ��Ă���΃p�P�b�g���M���J�n */
    WFSi_NotifyLinkup(static_work);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_InitChild

  Description:  WFS�𗘗p���邽�߂̎q�@���C�����X����������.

  Arguments:    port             WFS�̂��߂Ɋ��蓖�Ă�WM�|�[�g�ԍ�
                callback         ��Ԓʒm�R�[���o�b�N
                allocator        WFS�̂��߂̃A���P�[�^
                allocator_arg    allocator�̈���

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_InitChild(int port, WFSStateCallback callback, WFSAllocator allocator, void *allocator_arg)
{
    /* ���ʂ̏����� */
    WFSi_InitCommon(callback, allocator, allocator_arg, port,
                    sizeof(WFSClientContext), ChildWirelessEvent);
    /* WFS�N���C�A���g������ */
    {
        WFSClientContext   *client = (WFSClientContext*)static_work->context;
        static_work->client = client;
        WFS_InitClient(client, client, ChildWFSEvent, static_work->allocator);
    }
    static_work->state = WFS_STATE_IDLE;
    static_work->initialized = TRUE;
    /* ���łɏ��������ɒʐM�\��ԂɂȂ��Ă���΃p�P�b�g���M���J�n */
    WFSi_NotifyLinkup(static_work);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_Start

  Description:  WFS��MP�ʐM�𗘗p�\�ł��邱�Ƃ�ʒm.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_Start(void)
{
    StaticWork *work = static_work;
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    if (work->available && !work->mp_availale)
    {
        /* WM�Ƀ|�[�g�R�[���o�b�N��ݒ� */
        int     result;
        result = WM_SetPortCallback((u16)work->port, WFSi_PortCallback, work);
        if (result != WM_ERRCODE_SUCCESS)
        {
            OS_TPanic("WM_SetPortCallback failed! (result=%d)", result);
        }
        /* WM�̓�����Ԃ��擾 */
        result = WM_ReadStatus(work->status_buf);
        if (result != WM_ERRCODE_SUCCESS)
        {
            OS_TPanic("WM_ReadStatus failed! (result=%d)", result);
        }
        work->mp_availale = TRUE;
        WFSi_NotifyLinkup(work);
        WFSi_NotifySend(work);
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_End

  Description:  WFS���I��.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_End(void)
{
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    if (static_work->available)
    {
        StaticWork *const work = static_work;
        work->state = WFS_STATE_STOP;
        work->state_func = NULL;
        work->available = FALSE;
        /* WM�̃|�[�g�R�[���o�b�N����� */
        if (work->mp_availale)
        {
            (void)WM_SetPortCallback((u16)work->port, NULL, NULL);
            work->mp_availale = FALSE;
            work->status_buf->aid = 0;
        }
        /* �e�q�ʂ̏I������ */
        if (work->initialized)
        {
            NotifyWirelessEvent(work, WIRELESS_EVENT_END, NULL);
            static_work->server = NULL;
            static_work->client = NULL;
            work->initialized = FALSE;
        }
        /* �m�ۂ������I����������� */
        MI_CallFree(work->allocator, work->context);
    }
    (void)OS_RestoreInterrupts(bak_cpsr);
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetStatus

  Description:  WFS �̌��݂̏�Ԃ��擾.

  Arguments:    None.

  Returns:      WFS �̌��݂̏��.
 *---------------------------------------------------------------------------*/
WFSState WFS_GetStatus(void)
{
    return static_work->state;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_SetDebugMode

  Description:  WFS �����̃f�o�b�O�o�͂�L�� / �����ɐݒ�.

  Arguments:    enable            �f�o�b�O�o�͂�L���ɂ���Ȃ�TRUE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_SetDebugMode(BOOL enable)
{
    static_work->debug_enable = enable;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetCurrentBitmap

  Description:  WBT ���F�����Ă��錻�݂̎q�@�Q�̃r�b�g�}�b�v���擾���܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    None.

  Returns:      ���ݔF������Ă���q�@�Q.
 *---------------------------------------------------------------------------*/
int WFS_GetCurrentBitmap(void)
{
    return static_work->server ? WFS_GetServerConnectedBitmap(static_work->server) : 0;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetSyncBitmap

  Description:  �A�N�Z�X�������w�肳�ꂽ�q�@�Q�̃r�b�g�}�b�v���擾���܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    None.

  Returns:      �A�N�Z�X���������悤�w�肳�ꂽ�q�@�Q.
 *---------------------------------------------------------------------------*/
int WFS_GetSyncBitmap(void)
{
    return static_work->server ? WFS_GetServerSyncBitmap(static_work->server) : 0;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetBusyBitmap

  Description:  ���ݐe�@�ɃA�N�Z�X���̎q�@�Q�̃r�b�g�}�b�v���擾���܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    None.

  Returns:      ���ݐe�@�ɃA�N�Z�X���̎q�@�Q.
 *---------------------------------------------------------------------------*/
int WFS_GetBusyBitmap(void)
{
    return static_work->server ? WFS_GetServerBusyBitmap(static_work->server) : 0;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_IsBusy

  Description:  �w�肵�� aid �̎q�@�����݃A�N�Z�X�������肵�܂�.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    aid              ���肷��q�@�� aid.

  Returns:      �w��̎q�@���A�N�Z�X���Ȃ� TRUE, �����łȂ���� FALSE.
 *---------------------------------------------------------------------------*/
BOOL WFS_IsBusy(int aid)
{
    return ((1 << aid) & WFS_GetBusyBitmap()) != 0;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetPacketSize

  Description:  WFS �ɐݒ肳�ꂽ�e�@�� MP �ʐM�p�P�b�g�T�C�Y���擾.

  Arguments:    None.

  Returns:      WFS �ɐݒ肳�ꂽ�e�@�� MP �ʐM�p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
int WFS_GetPacketSize(void)
{
    return static_work->server ? WFS_GetServerPacketLength(static_work->server) : 0;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_SetPacketSize

  Description:  �e�@����̑��M�p�P�b�g�T�C�Y��ݒ肵�܂�.
                ���̊֐���, WFS_Start() ���Ăяo���O��
                �S�Ă̎q�@���������ڑ��������Ă����Ԃł̂�
                �Ăяo�����Ƃ��ł��܂�.
                ����ȊO�ł��̊֐����Ăяo�����ꍇ, ����͕ۏ؂���܂���.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    size             �ݒ肷��p�P�b�g�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_SetPacketSize(int size)
{
    if(static_work->server)
    {
        WFS_SetServerPacketLength(static_work->server, size);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_EnableSync

  Description:  �e�@���ŃA�N�Z�X���������q�@�Q�̃Z�b�g��ݒ肵�܂�.
                ���̊֐���, ����̃t�@�C���Q�𓯂������ŃA�N�Z�X���邱�Ƃ�
                ���炩�ɕۏ؂���Ă���q�@�Q�ɑ΂��ĉ����𓯊����邱�Ƃ�
                WBT ���C�u�����̓������������������I�ȓ]�����������܂�.
                ������, �����J�n�̃^�C�~���O���_���I�Ɉ��S�łȂ��ꍇ
                �q�@�Q�ւ̉���������ăf�b�h���b�N����_�ɒ��ӂ��Ă�������.
                ���̊֐��͐e�@���̂݌Ăяo�����Ƃ��ł��܂�.

  Arguments:    sync_bitmap      �A�N�Z�X���������q�@�Q�� aid �r�b�g�}�b�v.
                                 �e�@���g�������ŉ��ʃr�b�g 1 �͖�������܂�.
                                 ���̒l�� 0 ���w�肷��Ɠ����͍s���܂���.
                                 �f�t�H���g�ł͂��̏�Ԃł�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_EnableSync(int sync_bitmap)
{
    if(static_work->server)
    {
        WFS_SetServerSync(static_work->server, sync_bitmap);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetCurrentDownloadProgress

  Description:  ����ReadFile�����s���Ă���t�@�C���̐i�����擾.

  Arguments:    current          ��M�ς݃p�P�b�g��.
                total            ��M�\��̃p�P�b�g����.

  Returns:      ReadFile�����s���Ȃ�TRUE.
 *---------------------------------------------------------------------------*/
BOOL WFS_GetCurrentDownloadProgress(int *current, int *total)
{
    return static_work->client && (WFS_GetClientReadProgress(static_work->client, current, total), TRUE);
}
