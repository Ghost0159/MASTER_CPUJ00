/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - httpclient-1
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.5  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.4  2005/11/02 04:13:42  terui
  Fix comment.

  Revision 1.3  2005/09/14 06:20:07  seiki_masashi
  SOCL_AddRandomSeed 関数を使用するように変更

  Revision 1.2  2005/08/10 00:05:50  seiki_masashi
  警告の抑止

  Revision 1.1  2005/08/06 14:27:49  seiki_masashi
  httpsclient-1 デモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "wcm_control.h"
#include <string.h>

#define HTTPS_SERVER_IP CPS_MK_IPv4(192, 168, 0, 1)
#define HTTPS_PORT      443

static void         Init(void);
static int          SslAuthCallback(int result, SOCLSslConnection* con, int level);

SOCLSslConnection   con;

static char         ca_name[] = "Test CA, JP, Kyoto, Nintendo Co.,Ltd.";

static unsigned char    ca_modulo[] = {
    0xc5,0xfa,0x5d,0x50,0x46,0xc7,0x76,0xde,
    0x08,0x73,0x13,0x48,0xff,0xad,0x8b,0x84,
    0x7e,0xdf,0x75,0x93,0xf6,0x02,0xc4,0x65,
    0x97,0xef,0x17,0x51,0x8a,0x36,0xe1,0xe5,
    0x97,0xaa,0x26,0x50,0xf0,0x1c,0x80,0x1b,
    0x8d,0x0a,0xcf,0x24,0x51,0x71,0x40,0xc0,
    0xd6,0x6b,0x98,0x90,0xb7,0xb2,0x80,0x9e,
    0x40,0xaa,0x8d,0x34,0x08,0xa7,0x88,0x39,
    0xb3,0x34,0xe2,0xa8,0x73,0x70,0xe4,0x56,
    0xa1,0xba,0x31,0x35,0xfa,0x23,0x3a,0x7a,
    0xb0,0x95,0x7a,0xc3,0xca,0x7b,0x26,0xaf,
    0x77,0x32,0xab,0x86,0x00,0x80,0x1d,0x68,
    0x56,0x62,0xba,0x4e,0x6c,0x4b,0x83,0x6e,
    0xe5,0x1e,0xfe,0x62,0x94,0xdf,0x1b,0x18,
    0xaa,0x10,0x6e,0x75,0xc0,0x1a,0x51,0x43,
    0x99,0xdb,0x85,0x98,0x9a,0x7b,0xbb,0x97
};

static unsigned char    ca_exponent[] = {
    0x01,0x00,0x01
};

SOCLCaInfo      ca_info = {
    ca_name,
    sizeof(ca_modulo),
    ca_modulo,
    sizeof(ca_exponent),
    ca_exponent
};

SOCLCaInfo*     ca_list[] = { &ca_info };

void NitroMain(void)
{
    int     socket;
    int     result;
    u8      read_buffer[256];
    char*   write_buffer = "GET / HTTP/1.0\n\n";

    Init();

    OS_TPrintf("Create....");
    socket = SOCL_TcpSocket();
    OS_TPrintf("socket=0x%08lx\n", socket);

    OS_TPrintf("Enable SSL....");
    {
        u32 data[OS_LOW_ENTROPY_DATA_SIZE/sizeof(u32)];
        OS_GetLowEntropyData(data);
        SOCL_AddRandomSeed(data, sizeof(data));
    }

    MI_CpuClear8(&con, sizeof(SOCLSslConnection));
    con.auth_callback = SslAuthCallback;
    con.server_name = "secure.example.org";
    con.ca_info = ca_list;
    con.ca_builtins = sizeof(ca_list) / sizeof(ca_list[0]);
    result = SOCL_EnableSsl(socket, &con);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("Bind....");
    result = SOCL_Bind(socket, 0);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("Connect....");
    result = SOCL_Connect(socket, HTTPS_PORT, HTTPS_SERVER_IP);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("Write....");
    result = SOCL_Write(socket, write_buffer, (int)strlen(write_buffer), 0, 0);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("Read....\n");
    OS_TPrintf("----\n");
    while (1)
    {
        result = SOCL_Read(socket, read_buffer, sizeof(read_buffer) - 1, NULL, NULL);

        if (result <= 0)
        {
            break;
        }

        read_buffer[result] = '\0';
        OS_PutString((char*)read_buffer);
    }

    OS_TPrintf("\n----\n");

    OS_TPrintf("Shutdown....");
    result = SOCL_Shutdown(socket);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("Close....");
    result = SOCL_Close(socket);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("Terminate....\n");
    OS_Terminate();
}

/*---------------------------------------------------------------------------*
   初期化
 *---------------------------------------------------------------------------*/
static void     Heap_Setup(void);
static void*    myAlloc(u32 size);
static void     myFree(void* ptr);

SOCLConfig      socl_config =
{
    TRUE,
    {
        0
    },
    myAlloc,
    myFree,
    SOCL_CMDPACKET_MAX
};

void Init(void)
{
    int result;

    OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    Heap_Setup();
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    while (WCM_GetPhase() != WCM_PHASE_NULL)
    {
        OS_Sleep(100);
    }

    InitWcmControl();

    OS_TPrintf("LINK UP....");
    while (WCM_GetPhase() != WCM_PHASE_DCF)
    {
        OS_Sleep(100);
    }

    result = SOCL_Startup(&socl_config);
    OS_TPrintf("%d\n", result);

    OS_TPrintf("DHCP....\n");
    while ((volatile CPSInAddr)CPSMyIp == 0)
    {
        OS_Sleep(100);
    }

    OS_TPrintf("IP      = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSMyIp));
    OS_TPrintf("NetMask = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSNetMask));
    OS_TPrintf("GW IP   = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSGatewayIp));
    OS_TPrintf("DNS[0]  = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSDnsIp[0]));
    OS_TPrintf("DNS[1]  = %3d.%3d.%3d.%3d\n", CPS_CV_IPv4(CPSDnsIp[1]));
}

/*---------------------------------------------------------------------------*
  ヒープ関連
 *---------------------------------------------------------------------------*/

// MainRAM アリーナに対して メモリ割り当てシステム初期化
static void Heap_Setup(void)
{
    void*           nstart;
    OSHeapHandle    handle;

    nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetMainArenaLo(nstart);
    handle = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle);
}

static void* myAlloc(u32 size)
{
    OSIntrMode  enable = OS_DisableInterrupts();
    void*       ptr = OS_Alloc(size);
    (void)OS_RestoreInterrupts(enable);
    return ptr;
}

static void myFree(void* ptr)
{
    OSIntrMode  enable = OS_DisableInterrupts();
    OS_Free(ptr);
    (void)OS_RestoreInterrupts(enable);
    return;
}

/*---------------------------------------------------------------------------*
  SSL サーバ認証関連
 *---------------------------------------------------------------------------*/
static int SslAuthCallback(int result, SOCLSslConnection* con, int level)
{
#pragma unused(con)
#pragma unused(level)
    OS_TPrintf("level = %d\n", level);
    OS_TPrintf("subject = <%s>\n", con->subject);
    OS_TPrintf("issuer = <%s>\n", con->issuer);
    OS_TPrintf("CN = <%s>\n", con->cn);
    if (result & SOCL_CERT_OUTOFDATE)
        OS_TPrintf("Certificate is out-of-date\n");
    if (result & SOCL_CERT_BADSERVER)
    {
        OS_TPrintf("Server name does not match\n");
        result &= ~SOCL_CERT_BADSERVER;
    }

    switch (result & SOCL_CERT_ERRMASK)
    {
    case SOCL_CERT_NOROOTCA:
        OS_TPrintf("no root CA installed\n");
        OS_TPrintf(" but ignore this error\n");
        result = 0; // ignore this error
        break;

    case SOCL_CERT_BADSIGNATURE:
        OS_TPrintf("bad signature\n");
        break;

    case SOCL_CERT_UNKNOWN_SIGALGORITHM:
        OS_TPrintf("unknown signature algorithm\n");
        break;

    case SOCL_CERT_UNKNOWN_PUBKEYALGORITHM:
        OS_TPrintf("unknown public key alrorithm\n");
        break;
    }

    return result;
}

/*====== End of main.c ======*/
