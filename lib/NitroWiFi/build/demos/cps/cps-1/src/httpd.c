/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - CPS - demos
  File:     httpd.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.14 $ $Date: 2005/09/01 06:24:45 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//	httpd server
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

static u8       buf[95 * 100];

extern CPSCertificate   mycert;
extern CPSPrivateKey    mykey;

void    httpd_main(CPSSoc* , int);

void httpd_main(CPSSoc* soc, int https)
{
    u8*     s;
    RTCDate date;
    RTCTime time;
    u32     i;
    u32     len;

    CPS_SocRegister(soc);
    for (;;)
    {
        CPS_SocUse();
        if (https)
        {
            CPS_SetMyCert(&mycert, &mykey);
            CPS_SocBind(443, 0, 0);
            CPS_SetSsl(1);
        }
        else
        {
            CPS_SocBind(80, 0, 0);
        }

        CPS_TcpListen();
        OS_TPrintf("established\n");
        while ((s = CPS_SocGets()) != NULL && *s != '\0')
            OS_TPrintf("%s\n", s);
        (void)RTC_GetDate(&date);
        (void)RTC_GetTime(&time);
        CPS_SocPuts(

                        "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-type: text/html\r\n\r\n<html><head><META http-equiv=\"Content-Type\" content=\"text/html; charset=Shift_JIS\"><title>Nintendo DS Web-server</title></head><body><h1>あなたの予想通り、このページが見えているでしょうか<br>");
        if (https)
        {
            CPS_SocPuts("そしてちゃんと鍵がかかっているでしょうか<br>");
        }

        CPS_SocPrintf(

            "%d/%d/%d %02d:%02d:%02d</h1></body></html>\r\n",

            date.year + 2000, date.month, date.day,
                      time.hour, time.minute, time.second);

        //		CPS_SocFlush();
        for (i = 0x20 - 0x20; i < 0x7f - 0x20; i++)
        {
            MI_CpuFill8(buf + i * 100, (u8) (i + 0x20), 94);
            MI_CpuCopy8("<br>\r\n", buf + i * 100 + 94, 6);
        }

        for (i = 0; i < 10; i++)
        {
            len = CPS_SocWrite(buf, (0x7f - 0x20) * 100);

            //			len = CPS_SocWrite(buf, 2871);
            OS_TPrintf("%d bytes written\n", len);
        }

        CPS_TcpShutdown();
        CPS_TcpClose();
        CPS_SocRelease();
    }

    CPS_SocUnRegister();    // just in case...
}

static void httpd(void* arg)
{
#pragma unused(arg)

    MI_CpuClear8(&soc, sizeof(CPSSoc));
    soc.rcvbuf.size = sizeof(rcvbuf);
    soc.rcvbuf.data = rcvbuf;
    soc.sndbuf.size = sizeof(sndbuf);
    soc.sndbuf.data = sndbuf;
    soc.linbuf.size = sizeof(linbuf);
    soc.linbuf.data = linbuf;
    soc.outbuf.size = sizeof(outbuf);
    soc.outbuf.data = outbuf;
    httpd_main(&soc, 0);
}

#define STACK_SIZE  2048
static OSThread thread;
static u8       stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

extern void     start_httpd(void);

void start_httpd()
{
    MI_CpuClear8(&thread, sizeof(OSThread));
    OS_CreateThread(&thread, httpd, NULL, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
}

extern void stop_httpd(void);

void stop_httpd()
{
    OS_DestroyThread(&thread);
}
