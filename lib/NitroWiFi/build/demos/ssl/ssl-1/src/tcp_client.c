/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     tcp_client.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.
  Copyright 2005 Nitendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.7 $ $Date: 2006/03/10 09:22:42 $

  $Log: tcp_client.c,v $
  Revision 1.7  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.5  2005/08/08 05:58:26  seiki_masashi
  警告の抑制

  Revision 1.4  2005/08/06 11:48:43  seiki_masashi
  string.h を include

  Revision 1.3  2005/07/29 09:49:11  seiki_masashi
  small fix

  Revision 1.2  2005/07/29 08:59:33  seiki_masashi
  auth_callback のインターフェイスの変更に対応

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 に移動

  Revision 1.2  2005/07/18 23:55:20  seiki_masashi
  デバッグ出力の追加

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 デモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//  http client
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>
#include "screen.h"
#include "ca_info.h"
#include "tcp_client.h"

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

        //result = 0;
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

static void httpc(void* arg)
{
    CPSInAddr       ip;
    u8*         s;
    u32         len;
    u32         i;
    HttpClientRequest*  httpc_param = arg;
    char    c[2];
    u32     port;
    OSTick  start, end;

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
    start = OS_GetTick();
    ip = CPS_Resolve(httpc_param->host);
    end = OS_GetTick();
    OS_TPrintf("Resolve costs %lld ms\n", OS_TicksToMilliSeconds(end - start));
    if (ip != 0)
    {
        CPS_SocUse();
        port = httpc_param->port;
        if (httpc_param->https)
        {
            MI_CpuClear8(&con, sizeof(CPSSslConnection));
            con.auth_callback = auth_callback;
            con.server_name = httpc_param->host;
            soc.con = &con;
            CPS_SetRootCa(ca_info, sizeof(ca_info) / sizeof(ca_info[0]));
            CPS_SetSsl(1);
            if (port == 0)
            {
                port = 443;
            }
        }
        else
        {
            if (port == 0)
            {
                port = 80;
            }
        }

        while (TRUE)
        {
            CPS_SocBind(0, (u16) port, ip);
            OS_PutString("SocBind done.\n");
            start = OS_GetTick();
            if (CPS_TcpConnect())
            {
                OS_TWarning("Failed to connect to server\n");
            }
            else
            {
                end = OS_GetTick();
                OS_PutString("TcpConnect done.\n");
                OS_TPrintf("TcpConnect costs %lld ms\n", OS_TicksToMilliSeconds(end - start));
                CPS_SocPrintf(

                    "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", httpc_param->path,
                              httpc_param->host);
                OS_PutString("SocPrintf done.\n");
                CPS_SocFlush();
                OS_PutString("SocFlush done.\n");
                while ((s = CPS_SocGets()) != NULL && *s != '\0')
                    OS_TPrintf("%s\n", s);
                OS_PutString("--------- end of header ----------\n");
                while ((s = CPS_SocRead(&len)) != NULL)
                {
                    for (i = 0; i < len; i++)
                    {
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
            }

            CPS_TcpShutdown();
            OS_PutString("TcpShutdown done.\n");
            CPS_TcpClose();
            OS_PutString("TcpClose done.\n");

            //OS_Sleep(5000);
            break;
        }

        CPS_SocRelease();
        CPS_SocUnRegister();
    }
}

#define STACK_SIZE  2048
static OSThread thread;
static u8       stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

void StartHttpClient(HttpClientRequest* httpc_param)
{
    OS_CreateThread(&thread, httpc, httpc_param, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
}
