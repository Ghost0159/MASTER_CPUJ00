/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - CPS - demos
  File:     nbns.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.16 $ $Date: 2005/09/20 07:21:48 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//	NetBios name service responder
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>
#include "screen.h"

#pragma warn_implicitconv off

#define USE_CALLBACK

static CPSSoc   soc;
static u8       rcvbuf[128];
static u8       sndbuf[128];

u8  my_nbname_ascii[15 + 1];
u8  my_nbname[32 + 1];

static void make_uniq_nbname(char* name)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        if (i == 0)
            (void)OS_SPrintf((char*)my_nbname_ascii, "%s", name);
        else
            (void)OS_SPrintf((char*)my_nbname_ascii, "%s-%d", name, i + 1);
        if (CPS_NbResolve((char*)my_nbname_ascii) == 0)
        {
            return;
        }
    }

    my_nbname_ascii[0] = '\0';
}

static void respond(u16 id)
{
    u8*     res = sndbuf + 14 + 20 + 8;

    CPS_SETUSHORT2(res, id);
    CPS_SETUSHORT2(res + 2, 0x8500);            // response, authoritative, recursion
    CPS_SETUSHORT2(res + 4, 0);                 // 0 question
    CPS_SETUSHORT2(res + 6, 1);                 // 1 answer
    CPS_SETUSHORT2(res + 8, 0);                 // 0 autority
    CPS_SETUSHORT2(res + 10, 0);                // 0 additional
    res[12] = 32;                               // length of nb name
    MI_CpuCopy8(my_nbname, res + 13, 32 + 1);
    CPS_SETUSHORT2(res + 46, 0x0020);           // nb
    CPS_SETUSHORT2(res + 48, 0x0001);           // inet
    CPS_SETULONG2(res + 50, 2 * 24 * 60 * 60);  // 2 days
    CPS_SETUSHORT2(res + 54, 6);                // length
    CPS_SETUSHORT2(res + 56, 0x6000);           // uniquename, H-node
    CPS_SETULONG2(res + 58, CPSMyIp);           // IP address
    (void)CPS_SocWrite(res, 62);
}

#ifdef USE_CALLBACK
static int  got_udp;

static int udpread_callback(u8* data, u32 len, CPSSoc* soc)
{
#pragma unused(data)
#pragma unused(len)
#pragma unused(soc)
#if 0
    int i;

    OS_TPrintf("%d bytes received\n", len);
    for (i = 0; i < len; i++)
    {
        OS_TPrintf("%02x  ", data[i]);
    }

    OS_TPrintf("\n");
#endif
#if 0
    //
    //	Just print, do nothing more...
    //
    soc->remote_ip = soc->remote_ip_bound;      // re-bind to original remote IP
    return 1;                   // tell caller to throw this away
#else
    got_udp = 1;                // say we got UDP packet
    return 0;                   // tell caller to keep this
#endif
}

static void wait_udp()
{
    while (!got_udp)
        OS_Sleep(10);
    got_udp = 0;
}
#endif

static void nbns(void* arg)
{
    u8*     s;
    u32     len;
    u16     id;
    int     need_response;

    while (CPSMyIp == 0)        // wait till IP established
        OS_Sleep(1000);
    MI_CpuClear8(&soc, sizeof(CPSSoc));
    soc.rcvbuf.size = sizeof(rcvbuf);
    soc.rcvbuf.data = rcvbuf;
    soc.sndbuf.size = sizeof(sndbuf);
    soc.sndbuf.data = sndbuf;
    CPS_SocRegister(&soc);
    make_uniq_nbname((char*)arg);
    if (my_nbname_ascii[0] == '\0')
    {
        OS_TPrintf("NetBios name conflicts, aborting....\n");
        CPS_SocUnRegister();
        return;
    }

    PrintSubScreen("I am <");
    PrintSubScreen((char*)my_nbname_ascii);
    PrintSubScreen(">\n");
    CPS_EncodeNbName(my_nbname, my_nbname_ascii);
    CPS_SocUse();
    CPS_SocDatagramMode();
#ifdef USE_CALLBACK
    CPS_SetUdpCallback(udpread_callback);
#endif
    for (;;)
    {
        CPS_SocBind(137, 137, 0);
#ifdef USE_CALLBACK
        wait_udp();
#endif
        s = CPS_SocRead(&len);
        id = CPS_GETUSHORT2(s); // remember id
        need_response = len >= 50 &&
            CPS_GETUSHORT2(s + 2) == 0x0110 &&
            CPS_GETUSHORT2(s + 4) == 0x0001 &&
            s[12] == 32 &&
            memcmp(s + 13, my_nbname, 32 + 1) == 0;
        CPS_SocConsume(len);
        if (need_response)
        {
            respond(id);
        }
    }

    CPS_SocRelease();           // just in case, will never reach here...
    CPS_SocUnRegister();
}

#define STACK_SIZE  2048
static OSThread thread;
static u8       stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

extern void     start_nbns(char* );

void start_nbns(char* nb_name)
{
    MI_CpuClear8(&thread, sizeof(OSThread));
    OS_CreateThread(&thread, nbns, nb_name, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
}

extern void stop_nbns(void);

void stop_nbns()
{
    OS_DestroyThread(&thread);
}
