/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - CPS - demos
  File:     httpsd.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.9 $ $Date: 2005/08/03 07:02:05 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//	http secure server
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#include <nitro.h>
#include <nitroWiFi.h>

static CPSSoc       soc;
static u8       rcvbuf[512];
static u8       sndbuf[1514];
static u8       linbuf[128];
static u8       outbuf[1460 * 2];

static CPSSslConnection con;

extern void httpd_main(CPSSoc* , int);

static void httpsd(void* arg)
{
#pragma unused(arg)

    MI_CpuClear8(&soc, sizeof(CPSSoc));
    MI_CpuClear8(&con, sizeof(CPSSslConnection));
    soc.rcvbuf.size = sizeof(rcvbuf);
    soc.rcvbuf.data = rcvbuf;
    soc.sndbuf.size = sizeof(sndbuf);
    soc.sndbuf.data = sndbuf;
    soc.linbuf.size = sizeof(linbuf);
    soc.linbuf.data = linbuf;
    soc.outbuf.size = sizeof(outbuf);
    soc.outbuf.data = outbuf;
    soc.con = &con;
    httpd_main(&soc, 1);
}

#define STACK_SIZE  2048
static OSThread thread;
static u8       stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

extern void     start_httpsd(void);

void start_httpsd()
{
    MI_CpuClear8(&thread, sizeof(OSThread));
    OS_CreateThread(&thread, httpsd, NULL, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
}

extern void stop_httpsd(void);

void stop_httpsd()
{
    OS_DestroyThread(&thread);
}
