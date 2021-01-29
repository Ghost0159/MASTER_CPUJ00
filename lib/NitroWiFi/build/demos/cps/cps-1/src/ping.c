/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - CPS - demos
  File:     ping.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.5 $ $Date: 2005/10/21 11:28:30 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//	Ping client
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>

#pragma warn_implicitconv off

static CPSSoc   soc;
static u8       rcvbuf[128];
static u8       sndbuf[128];
static char     rname[128];

static void ping_sub(void* arg)
{
    CPSInAddr   ip;
    u32         start;
    u32         now;
    u32         len;
    int         i;
    u8*         s;

    MI_CpuClear8(&soc, sizeof(CPSSoc));
    soc.rcvbuf.size = sizeof(rcvbuf);
    soc.rcvbuf.data = rcvbuf;
    soc.sndbuf.size = sizeof(sndbuf);
    soc.sndbuf.data = sndbuf;
    CPS_SocRegister(&soc);
    ip = CPS_Resolve((char*)arg);
    if (ip != 0)
    {
        i = CPS_RevResolve(ip, rname, 128);
        OS_TPrintf("%d <%s>\n", i, rname);
        CPS_SocUse();
        CPS_SocPingMode();
        CPS_SocBind(0, 0, ip);
        (void)CPS_SocWrite((u8*)"1234567890", 10);
        start = (u32) OS_GetTick();
        for (;;)
        {
            now = (u32) OS_GetTick();
            if ((len = CPS_SocGetLength()) != 0)
            {
                s = CPS_SocRead(&len);
                OS_TPrintf("Reply from %d.%d.%d.%d: bytes=%d time=%dms\n", CPS_CV_IPv4(ip), len,
                           OS_TicksToMilliSeconds(now - start));
                for (i = 0; i < len; i++)
                    OS_TPrintf("%c", *s++);
                OS_TPrintf("\n");
                CPS_SocConsume(len);
                break;
            }
            else if ((now - start) > OS_MilliSecondsToTicks(3000))
            {
                OS_TPrintf("timeout\n");
                break;
            }
            else
                OS_YieldThread_();
        }

        CPS_SocRelease();
    }
    else
    {
        OS_TPrintf("Failed to find host %s\n", (char*)arg);
    }

    CPS_SocUnRegister();
}

#define STACK_SIZE  2048
static OSThread thread;
static u8       stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

extern void     ping(char* );

void ping(char* target)
{
    OS_CreateThread(&thread, ping_sub, target, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
}
