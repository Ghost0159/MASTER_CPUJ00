/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.3  2006/01/16 01:41:12  kitase_hirotake
  ���J�p�� sitedefs �̏C��

  Revision 1.2  2006/01/06 04:31:38  kitase_hirotake
  �ʐM�̍�����

  Revision 1.1  2006/01/06 00:43:29  kitase_hirotake
  httpserver �̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <nitro/fs.h>

#include "netconnect.h"

#include "server_thread.h"

#define SITEDEFS_APCLASS    "WiFi.LAN.1.AP.1"

static const u32    default_dma_no = MI_DMA_MAX_NUM;

OSMessage       mesgBuffer[10];
OSMessageQueue      mesgQueue;

/*---------------------------------------------------------------------------*
 �ʏ�̃Q�[���ŗp������ʐM���������̃f�[�^�𑗐M����̂�
 �\�P�b�g�ɂ����郁�������p�p�����[�^���I�[�o�[���C�h����D
 *---------------------------------------------------------------------------*/

// TCP
SDK_WEAK_SYMBOL SOCLSocketParam SOCLSocketParamTCP =
{
    SOCL_FLAGMODE_TCP,                      // TCP
    SOCL_FLAGBLOCK_BLOCK,                   // BLOCK
    {                                       // cps
        0,                                  //SOCL_TCP_SOCKET_CPS_RCVBUF_SIZE,       // rcvbuf_size

        //****Set in SOCL_Startup()
        0,                                  //SOCL_TCP_SOCKET_CPS_RCVBUF_CONSUME_MIN,    // rcvbuf_consume_min

        //****Set in SOCL_Startup()
        SOCL_TCP_SOCKET_CPS_SNDBUF_SIZE,    // sndbuf_size
        SOCL_TCP_SOCKET_CPS_LINBUF_SIZE,    // linbuf_size
        SOCL_TCP_SOCKET_CPS_OUTBUF_SIZE,    // outbuf_size
        3029,   // �f�t�H���g�ł� ���M�X���[�v�b�g�œK���p�X�C�b�` ���ݒ肳��Ă���.
        0,      // udpbuf_size
    },

    {           // recv_thread
        SOCL_TCP_SOCKET_RECV_THREAD_STACK_SIZE, // stack_size
        SOCL_TCP_SOCKET_RECV_THREAD_PRIORITY,   // priority
        SOCL_TCP_SOCKET_RECV_THREAD_QUEUE_MAX,  // queue_max
    },

    {   // send_thread
        SOCL_TCP_SOCKET_SEND_THREAD_STACK_SIZE, // stack_size
        SOCL_TCP_SOCKET_SEND_THREAD_PRIORITY,   // priority
        SOCL_TCP_SOCKET_SEND_THREAD_QUEUE_MAX,  // queue_max
    },
};

static void VBlankIntr(void)
{
    OSThread*   t;

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    //OS_CheckStack(OS_GetCurrentThread());
    for (t = OS_GetThreadList(); t != NULL; t = OS_GetNextThread(t))
    {
        OS_CheckStack(t);
    }
}

static void RecursiveEnumDir(int tab, FSDirEntry* pe)
{
    FSFile  d;
    FS_InitFile(&d);

    /* in the recursive function, it costs much stack.
       so we use one FSDirEntry like this. */
    OS_Printf("%*s%s/\n", tab, "", pe->name);
    if (FS_SeekDir(&d, &pe->dir_id))
    {
        tab += 4;
        while (FS_ReadDir(&d, pe))
        {
            if (pe->is_directory)
                RecursiveEnumDir(tab, pe);
            else
                OS_Printf("%*s%s\n", tab, "", pe->name);
        }
    }
}

/* ���C����������̃A���[�i�ɂă����������ăV�X�e���������� */
static void InitializeAllocateSystem(void)
{
    void*           tempLo;
    OSHeapHandle    hh;

    /* OS_Init�͌Ă΂�Ă���Ƃ����O�� */
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }

    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

static void SetHostSetting(SOSockAddrIn* sa_in, const char* addr, u16 port)
{
    SOHostEnt*  host_ent;

    /* sockaddr_in ��ݒ� */
    sa_in->len = sizeof(*sa_in);
    sa_in->family = SOC_AF_INET;
    if (port > 0)
    {
        sa_in->port = SOC_HtoNs(port);
    }

    if (addr && (host_ent = SOC_GetHostByName(addr)) != 0)
    {
        MI_CpuCopy8(host_ent->addrList[0], &sa_in->addr, SOC_ALEN);
    }
}

/*---------------------------------------------------------------------------*
    http �T�[�o�Ƃ��ē��삷��T���v���v���O�����ł��B
    �Ή��ł��郊�N�G�X�g�� GET METHOD �݂̂ł��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    int server_socket;
    int result;
    int i;
    static SOSockAddrIn server_addr;
    static SOSockAddrIn client_addr;

    OS_Init();
    OS_InitThread();

    OS_InitMessageQueue(&mesgQueue, &mesgBuffer[0], 10);

    /* ���ʏ������֐��Ăяo�� */
    NcGlobalInit();
    NcStart(SITEDEFS_APCLASS);

    /* V�u�����N�ݒ� */
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    FS_Init(default_dma_no);

    {
        u32     need_size = FS_GetTableSize();
        void*   p_table = OS_Alloc(need_size);
        SDK_ASSERT(p_table != NULL);
        (void)FS_LoadTable(p_table, need_size);
    }

    /* server �����J�n */
    SetHostSetting(&server_addr, "0.0.0.0" /*ANY*/, 80);

    OS_TPrintf("Create....");
    server_socket = SOC_Socket(SOC_PF_INET, SOC_SOCK_STREAM, 0);
    OS_TPrintf("socket=0x%08lx\n", server_socket);
    OS_TPrintf("Bind....");
    result = SOC_Bind(server_socket, &server_addr);
    OS_TPrintf("Listen....");
    result = SOC_Listen(server_socket, 1);
    OS_TPrintf("Accept....\n");

    /* thread ��p���ĕ�����t���s�� */

    /* OS_InitThread() ���Ă΂�Ă��Ȃ��Ȃ�A�Ăяo�� */
    if (!OS_IsThreadAvailable())
    {
        OS_InitThread();
    }

    /* �ʐM�p�X���b�h�����炩����10����Ă��� */
    for (i = 0; i < 10; i++)
    {
        CreateServerThread();
    }

    /* ������ client ���󂯕t���� */
    while (1)
    {
        result = SOC_Accept(server_socket, &client_addr);
        if (result > 0)
        {
            /* �ڑ����󂯕t������A���b�Z�[�W�L���[�Ƀ\�P�b�g�𑗂��āA�ʐM�̓X���b�h�ɂ܂����� */
            (void)OS_SendMessage(&mesgQueue, (OSMessage) (&result), OS_MESSAGE_BLOCK);
        }
    }

    /* �����ɂ��邱�Ƃ͂Ȃ� */
    NcFinish();
    OS_TPrintf("Terminate....\n");
    OS_Terminate();
}
