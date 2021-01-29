/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - CPS - demos
  File:     httpc.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.21 $ $Date: 2005/10/13 11:08:09 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//	http client
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#include <nitro.h>
#include <nitroWiFi.h>
#include "screen.h"

typedef struct
{
    char*   url;
    char*   path;
    int     https;
} HTTPC;

static CPSSoc       soc;
static u8       rcvbuf[1460 * 2];
static u8       sndbuf[1514];
static u8       linbuf[128];
static u8       outbuf[512];

static CPSSslConnection con;

#if 1
static void dump_hex(char* title, u8* s, int len)
{
#pragma unused(title)
#pragma unused(s)
    int i;

    OS_TPrintf("%s", title);
    for (i = 0; i < len; i++)
    {
        if (i % 16 == 0)
            OS_TPrintf("\n\t");
        OS_TPrintf("%02x ", *s++);
    }

    OS_TPrintf("\n");
}
#endif

static int auth_callback(int result, CPSSslConnection* con, int level)
{
#pragma unused(level)
    OS_TPrintf("level = %d\n", level);
    OS_TPrintf("subject = <%s>\n", con->subject);
    OS_TPrintf("issuer = <%s>\n", con->issuer);
    OS_TPrintf("CN = <%s>\n", con->cn);
    if (result == 0)
        OS_TPrintf("Cert is OK\n");
    if (result & CPS_CERT_OUTOFDATE)
        OS_TPrintf("Certificate is out-of-date\n");
    if (result & CPS_CERT_BADSERVER)
    {
        OS_TPrintf("Server name does not match\n");
        result &= ~CPS_CERT_BADSERVER;
    }

    switch (result & CPS_CERT_ERRMASK)
    {
    case CPS_CERT_NOROOTCA:
        OS_TPrintf("no root CA installed\n");
        dump_hex("cert =", con->cert, con->certlen);
        result = 0;
        break;

    case CPS_CERT_BADSIGNATURE:
        OS_TPrintf("bad signature\n");
        break;

    case CPS_CERT_UNKNOWN_SIGALGORITHM:
        OS_TPrintf("unknown signature algorithm\n");
        break;

    case CPS_CERT_UNKNOWN_PUBKEYALGORITHM:
        OS_TPrintf("unknown public key alrorithm\n");
        break;
    }

    return result;
}

extern CPSCaInfo*   default_ca_info[];
extern int          default_ca_info_num;

static void httpc(void* arg)
{
    CPSInAddr   ip;
    u8*         s;
    u32         len;
    u32         i;
    HTTPC*      httpc_param = arg;
    char        c[2];
    int         max;
    int         total;
#pragma unused(c)
#pragma unused(max)
#pragma unused(total)

    MI_CpuClear8(&soc, sizeof(CPSSoc));
    soc.rcvbuf.size = sizeof(rcvbuf);
    soc.rcvbuf.data = rcvbuf;
    soc.sndbuf.size = sizeof(sndbuf);
    soc.sndbuf.data = sndbuf;
    soc.linbuf.size = sizeof(linbuf);
    soc.linbuf.data = linbuf;
    soc.outbuf.size = sizeof(outbuf);
    soc.outbuf.data = outbuf;
    CPS_SocRegister(&soc);
    ip = CPS_Resolve(httpc_param->url);
    if (ip != 0)
    {
        CPS_SocUse();
        if (httpc_param->https)
        {
            MI_CpuClear8(&con, sizeof(CPSSslConnection));
            con.auth_callback = auth_callback;
            con.server_name = httpc_param->url;
            soc.con = &con;
            CPS_SetRootCa(default_ca_info, default_ca_info_num);
            CPS_SocBind(0, 443, ip);
            CPS_SetSsl(1);
        }
        else
        {
            CPS_SocBind(0, 80, ip);
        }

        if (CPS_TcpConnect())
        {
            OS_TWarning("Failed to connect to server\n");
        }
        else
        {
            OS_TPrintf("Proper packet size = %d\n", CPS_GetProperSize());
            CPS_SocPrintf(

                "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", httpc_param->path,
                          httpc_param->url);
            CPS_SocFlush();
#if 0
            while ((s = CPS_SocGets()) != NULL && *s != '\0')
                OS_TPrintf("%s\n", s);
            OS_PutString("--------- end of header ----------\n");
            total = 0;
            max = 300;
            while ((s = CPS_SocRead(&len)) != NULL)
            {
                total += len;
                for (i = 0; i < len; i++)
                {
                    if (max == 0)
                        break;
                    max--;
                    c[0] = (char)s[i];
                    c[1] = '\0';
                    OS_PutString(c);
                    if (c[0] != '\r')
                        PrintSubScreen(c);
                }

                CPS_SocConsume(len);
            }

            if (c[0] != '\n')
            {
                OS_PutString("\n");
                PrintSubScreen("\n");
            }

            OS_PutString("--------- end of html data ----------\n");
            OS_TPrintf("Total bytes read = %d\n", total);
#else
            for (;;)
            {
#if 1
                u32 start = (u32) OS_GetTick();
                while ((len = (u32) CPS_SocGetLength()) == 0)
                {
                    if ((u32) OS_GetTick() - start > OS_MilliSecondsToTicks(10000))
                    {
                        OS_TPrintf("timeout...  ");
                        goto abort;
                    }

                    OS_YieldThread_();
                }

#else
                len = (u32) CPS_SocGetLength();
#endif
                if (len == -1)
                {
                    OS_TPrintf("EOF 1...  ");
                    break;
                }

                s = CPS_SocRead(&len);
                if (len == 0)
                {
                    OS_TPrintf("EOF 2...  ");
                    break;
                }

                for (i = 0; i < len && i < 400; i++)
                {
                    OS_TPrintf("%c", s[i]);
                }

                if (len > 400)
                {
                    OS_TPrintf("\n\t:\n\t:\n");
                    i = len - 400;
                    if (i < 400)
                        i = 400;
                    for (; i < len; i++)
                    {
                        OS_TPrintf("%c", s[i]);
                    }
                }

                OS_TPrintf("\n");
                CPS_SocConsume(len);
            }
#endif
        }

abort:
        OS_TPrintf("leaving httpc...\n");
        CPS_TcpShutdown();
        CPS_TcpClose();
        CPS_SocRelease();
        CPS_SocUnRegister();
    }
}

#define STACK_SIZE  2048
static OSThread thread;
static u8       stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

extern void     start_httpc(HTTPC* );

void start_httpc(HTTPC* httpc_param)
{
    OS_CreateThread(&thread, httpc, httpc_param, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
}
