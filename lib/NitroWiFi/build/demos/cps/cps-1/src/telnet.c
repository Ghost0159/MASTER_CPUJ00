/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - CPS - demos
  File:     telnet.c

  Copyright 2001-2005 Ubiquitous Corporation.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Ubiquitous Corporation, and are protected
  by Federal copyright law.  They may not be disclosed to third parties
  or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Ubiquitous Corporation.

  $Revision: 1.19 $ $Date: 2005/10/20 07:24:59 $

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>

#define OSi_SPECIFIC_CPS    0

#define USE_SOCDUP_FOR_OUTPUT

// #define USE_SOCGETLENGTH_FOR_INPUT
#pragma warn_implicitconv off

typedef struct
{
    char*   url;
    char*   path;
    int     https;
} HTTPC;

extern void start_httpc(HTTPC* );

//
//	TELNET capability exchange constants
//
#define WILL            (u8) 251
#define WONT            (u8) 252
#define DO              (u8) 253
#define DONT            (u8) 254
#define IAC             (u8) 255

#define ECHO            1
#define SUP_GO_AHEAD    3

//
//	Global variables
//
static int      sent_WILLECHO;          // true if WILLECHO already sent
static u32      adrsav = 0x02000000;    // address value when omitted
static char     buf[128];               // line input buffer
static u32      done;                   // true if exiting from TELNET
static OSThread thread;
#ifdef USE_SOCDUP_FOR_OUTPUT
static OSThread thread2;

static OSMessageQueue   mesgQueue;
static OSMessage        mesgBuffer[10];
#endif

static u8 mon_charin()
{
    int c;

    CPS_SocFlush();
#ifdef USE_SOCGETLENGTH_FOR_INPUT
    while ((c = CPS_SocGetLength()) == 0)
    {
        OS_YieldThread_();
    }

    if (c == -1)
    {
        OS_TPrintf("EOF detected by CPS_SocGetLength()\n");
    }
#endif
    c = CPS_SocGetChar();
    if (c == EOF)
    {
        OS_TPrintf("disconnected from host\n");
        done = 1;
        c = '\r';
    }

    return(u8) c;
}

static void reply_IAC(u8 cmd, u8 param)
{
    CPS_SocPutChar(IAC);
    CPS_SocPutChar(cmd);
    CPS_SocPutChar(param);
}

static void handle_IAC(void)
{
    u8  c;

    switch (mon_charin())
    {
    case WILL:
        switch (c = mon_charin())
        {
        case SUP_GO_AHEAD:
        case ECHO:
            reply_IAC(DO, c);
            break;

        default:
            reply_IAC(DONT, c);
            break;
        }
        break;

    case DO:
        switch (c = mon_charin())
        {
        case ECHO:
            if (sent_WILLECHO)
                break;
            sent_WILLECHO = 1;

        // fall into next...
        case SUP_GO_AHEAD:
            reply_IAC(WILL, c);
            break;

        default:
            reply_IAC(WONT, c);
            break;
        }
        break;

    default:
        break;
    }
}

static char* mon_bufin(int echo)
{
    int i;
    u8  c;

    i = 0;
    while ((c = mon_charin()) != '\r')
    {
        if (c == IAC)
        {
            handle_IAC();
        }
        else if (c == 0x08)
        {
            if (i != 0)
            {
                i--;
                if (echo)
                {
                    CPS_SocPuts("\b \b");
                }
            }
        }
        else if (c >= ' ' && i < sizeof(buf) - 1)
        {
            buf[i++] = c;
            if (echo)
            {
                CPS_SocPutChar(c);
            }
        }
    }

    buf[i] = '\0';
    CPS_SocPuts("\r\n");
    return buf;
}

static void mon_ipout(CPSInAddr ip, int crlf)
{
    CPS_SocPrintf("%d.%d.%d.%d", CPS_CV_IPv4(ip));
    if (crlf)
        CPS_SocPuts("\r\n");
}

static char* skipsp(char* s)
{
    while (*s == ' ')
        s++;
    return s;
}

//
//	Decimal only version of standard strtol()

//
static u32 strtol10(char* s, char ** endptr)
{
    u32     acc;
    u8      c;
    char*   dummy;

    if (endptr == NULL)
    {
        endptr = &dummy;
    }

    *endptr = s;    // in case nothing parsed
    acc = 0;
    for (;;)
    {
        c = (u8) * s;
        if ('0' <= c && c <= '9')
        {
            c = (u8) (c - '0');
        }
        else
        {
            break;
        }

        acc = acc * 10 + c;
        *endptr = ++s;
    }

    return acc;
}

//
//	Hexadecimal only version of standard strtol()

//
static u32 strtol16(char* s, char ** endptr)
{
    u32     acc;
    u8      c;
    char*   dummy;

    if (endptr == NULL)
    {
        endptr = &dummy;
    }

    *endptr = s;    // in case nothing parsed
    acc = 0;
    for (;;)
    {
        c = (u8) * s;
        if ('0' <= c && c <= '9')
        {
            c = (u8) (c - '0');
        }
        else if ('a' <= c && c <= 'f' || 'A' <= c && c <= 'F')
        {
            c = (u8) ((c | 0x20) - 'a' + 10);
        }
        else
        {
            break;
        }

        acc = acc * 16 + c;
        *endptr = ++s;
    }

    return acc;
}

static u32 mon_gethex(char ** ps)
{
    char*   s;
    char*   next;
    u32     v;

    s = skipsp(*ps);
    if (*s == '\0')
    {
        v = adrsav;
    }
    else
    {
        v = strtol16(s, &next);
        s = next;
    }

    *ps = s;
    return v;
}

////////////////////////////////////////////////
//
//	monitor commands

//
////////////////////////////////////////////////
static void dumpa_cmd(char* s)
{
    u32 adr;
    int cnt;
    u8  c;

    adr = mon_gethex(&s);
    for (cnt = 0; cnt < 8; cnt++)
    {
        CPS_SocPrintf("%08lx ", adr);
        do
        {
            c = *(u8*)adr++;
            if (c < ' ' || 0x7e < c)
                CPS_SocPutChar('.');
            else
                CPS_SocPutChar((char)c);
        }
        while (adr & 0x3f);
        CPS_SocPuts("\r\n");
    }

    adrsav = adr;
}

static void dumpb_cmd(char* s)
{
    u32 adr;
    int cnt;

    adr = mon_gethex(&s);
    for (cnt = 0; cnt < 8; cnt++)
    {
        CPS_SocPrintf("%08lx", adr);
        do
            CPS_SocPrintf(" %02x", *(u8*)adr++);
        while (adr & 0x0f)
            ;
        CPS_SocPuts("\r\n");
    }

    adrsav = adr;
}

static void dumph_cmd(char* s)
{
    u32 adr;
    int cnt;

    adr = mon_gethex(&s);
    for (cnt = 0; cnt < 8; cnt++)
    {
        CPS_SocPrintf("%08lx", adr);
        do
        {
            CPS_SocPrintf(" %04x", *(u16*)adr);
            adr += 2;
        }

        while ((adr & 0x0f) > 1)
            ;
        CPS_SocPuts("\r\n");
    }

    adrsav = adr;
}

static void dumpw_cmd(char* s)
{
    u32 adr;
    int cnt;

    adr = mon_gethex(&s);
    for (cnt = 0; cnt < 8; cnt++)
    {
        CPS_SocPrintf("%08lx", adr);
        do
        {
            CPS_SocPrintf(" %08lx", *(long*)adr);
            adr += 4;
        }

        while ((adr & 0x0f) > 3)
            ;
        CPS_SocPuts("\r\n");
    }

    adrsav = adr;
}

static void modb_cmd(char* s)
{
    u8*     adr;

    adr = (u8*)mon_gethex(&s);
    while (*(s = skipsp(s)) != '\0')
    {
        *adr++ = (u8) mon_gethex(&s);
    }
}

static void modh_cmd(char* s)
{
    u16*    adr;

    adr = (u16*)mon_gethex(&s);
    while (*(s = skipsp(s)) != '\0')
    {
        *adr++ = (u16) mon_gethex(&s);
    }
}

static void modw_cmd(char* s)
{
    u32*    adr;

    adr = (u32*)mon_gethex(&s);
    while (*(s = skipsp(s)) != '\0')
    {
        *adr++ = (u32) mon_gethex(&s);
    }
}

static void exit_cmd()
{
    done = 1;
}

#ifndef SDK_THREAD_INFINITY
static void thread_info(int id, int seq, OSThread* t)
{
    static char*    statestr[2] = {"WAITING,", "READY,  "};

    CPS_SocPrintf("%2d: %s priority = %2d", seq, statestr[t->state], t->priority);
    if (CPSSocTab[id] != NULL)
    {
        CPS_SocPuts(", CPS");
        if (CPSSocTab[id]->thread != NULL)
        {
            CPS_SocPuts(", in use");
            switch (CPSSocTab[id]->block_type)
            {
            case 1:
                CPS_SocPuts(", TCPCON");
                break;

            case 2:
                CPS_SocPuts(", TCPREAD");
                break;

            case 3:
                CPS_SocPuts(", UDPREAD");
                break;
            }
        }
    }

    CPS_SocPuts("\r\n");
}

static void p_cmd()
{
    OSThread **  entry = OS_GetSystemWork()->threadinfo_mainp->entry;
    int         i;
    int         seq;

    seq = 0;
    for (i = 0; i < OS_THREAD_MAX_NUM; i++)
        if (entry[i] != NULL)
            thread_info(i, seq++, entry[i]);
}

#else
static void thread_info(int seq, OSThread* t)
{
    static char*    statestr[2] = {"WAITING,", "READY,  "};
    CPSSoc*         soc;

    CPS_SocPrintf("%2d: %s priority = %2d", seq, statestr[t->state], t->priority);
    if ((soc = (CPSSoc*)OSi_GetSpecificData(t, OSi_SPECIFIC_CPS)) != NULL)
    {
        CPS_SocPuts(", CPS");
        if (soc->thread != NULL)
        {
            CPS_SocPuts(", in use");
            switch (soc->block_type)
            {
            case 1:
                CPS_SocPuts(", TCPCON");
                break;

            case 2:
                CPS_SocPuts(", TCPREAD");
                break;

            case 3:
                CPS_SocPuts(", UDPREAD");
                break;
            }
        }
    }

    CPS_SocPuts("\r\n");
}

static void p_cmd()
{
    OSThread*   t;
    int         seq;

    seq = 0;
    for (t = OS_GetThreadList(); t != NULL; t = OS_GetNextThread(t))
        thread_info(seq++, t);
}
#endif

static void ip_cmd()
{
    CPS_SocPrintf("MAC address     = %02x:%02x:%02x:%02x:%02x:%02x\r\n", CPSMyMac[0], CPSMyMac[1], CPSMyMac[2],
                  CPSMyMac[3], CPSMyMac[4], CPSMyMac[5]);
    CPS_SocPuts("IP address      = ");
    mon_ipout(CPSMyIp, 1);
    CPS_SocPuts("Subnet mask     = ");
    mon_ipout(CPSNetMask, 1);
    CPS_SocPuts("Default gateway = ");
    mon_ipout(CPSGatewayIp, 1);
    CPS_SocPuts("DNS Server      = ");
    mon_ipout(CPSDnsIp[0], 1);
    if (CPSDnsIp[1] != 0)
    {
        CPS_SocPuts("                  ");
        mon_ipout(CPSDnsIp[1], 1);
    }
}

static HTTPC    httpc;      // don't make this an auto variable...
static void wget_cmd_common(char* s, int https)
{
    httpc.url = s;
    while (*s != ' ' && *s != '\0')
        s++;
    *s = '\0';
    httpc.path = s + 1;
    httpc.https = https;
    start_httpc(&httpc);
}

static void wget_cmd(char* s)
{
    wget_cmd_common(s, 0);
}

static void wgets_cmd(char* s)
{
    wget_cmd_common(s, 1);
}

extern void ping(char* );

static void ping_cmd(char* s)
{
    ping(s);
}

static void get_cmd()
{
    CPS_SocPrintf("Priority = %d\r\n", CPS_GetThreadPriority());
}

static void set_cmd(char* s)
{
    CPS_SetThreadPriority(strtol10(s, NULL));
}

typedef struct
{
    char*   name;
    void (*func) ();
}
CMDTBL;

//
//	Command table
//
static const CMDTBL cmdtbl[] = {
    {"da", dumpa_cmd},
    {"db", dumpb_cmd},
    {"dh", dumph_cmd},
    {"d", dumpw_cmd},
    {"mb", modb_cmd},
    {"mh", modh_cmd},
    {"m", modw_cmd},
    {"exit", exit_cmd},
    {"p", p_cmd},
    {"ip", ip_cmd},
    {"wget", wget_cmd},
    {"wgets", wgets_cmd},
    {"ping", ping_cmd},
    {"get", get_cmd},
    {"set", set_cmd},

    {0, 0}                  // end of table
};

static void do_command(char* s)
{
    const CMDTBL*   p;
    u32 cmdlen;

    if (*(s = skipsp(s)) == '\0')
        return;
    p = cmdtbl;
    while (p->name != NULL)
    {
        cmdlen = strlen(p->name);
        if (strncmp(s, p->name, cmdlen) == 0 && (s[cmdlen] & 0xdf) == 0)
        {
            (*p->func) (skipsp(s + cmdlen));
            return;
        }

        p++;
    }

    CPS_SocPuts("?\r\n");
}

static void monitor_main()
{
    OSMessage   msg = 0;
    u32         start_time;

    sent_WILLECHO = 0;
    start_time = CPSi_GetTick();
    do
    {
        if (CPS_SocGetLength() != 0 && mon_charin() == IAC)
            handle_IAC();
        OS_Sleep(1);
    }

    while ((s32) (CPSi_GetTick() - start_time) < 3)
        ;
    if (!sent_WILLECHO)
        reply_IAC(WILL, ECHO);
    sent_WILLECHO = 1;
    done = 0;
    while (!done)
    {
#ifdef USE_SOCDUP_FOR_OUTPUT
        ++((u8*)msg);
        (void)OS_SendMessage(&mesgQueue, msg, OS_MESSAGE_BLOCK);
#else
        CPS_SocPuts("*");
#endif
        do_command(mon_bufin(1));
    }
}

static CPSSoc   soc;
static u8       rcvbuf[64];
static u8       sndbuf[1514];
static u8       outbuf[1000];

static void telnet(void* arg)
{
#pragma unused(arg)

    MI_CpuClear8(&soc, sizeof(CPSSoc));
    soc.rcvbuf.size = sizeof(rcvbuf);
    soc.rcvbuf.data = rcvbuf;
    soc.sndbuf.size = sizeof(sndbuf);
    soc.sndbuf.data = sndbuf;
    soc.outbuf.size = sizeof(outbuf);
    soc.outbuf.data = outbuf;
    CPS_SocRegister(&soc);
    for (;;)
    {
        CPS_SocUse();
        CPS_SocBind(23, 0, 0);
        CPS_TcpListen();
#ifdef USE_SOCDUP_FOR_OUTPUT
        CPS_SocDup(&thread2);
#endif
        OS_TPrintf("entering telnet...\n");
        monitor_main();
        OS_TPrintf("leaving telnet...\n");
        CPS_TcpShutdown();
        CPS_TcpClose();
        CPS_SocRelease();
    }

    CPS_SocUnRegister();    // just in case
}

#ifdef USE_SOCDUP_FOR_OUTPUT
static void telnet2(void* arg)
{
#pragma unused(arg)

    OSMessage   msg;

    for (;;)
    {
        (void)OS_ReceiveMessage(&mesgQueue, &msg, OS_MESSAGE_BLOCK);
        CPS_SocPrintf("<%d>", (u32) msg);
    }
}
#endif

#define STACK_SIZE  2048
static u8   stack[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM    (stack + STACK_SIZE)

#ifdef USE_SOCDUP_FOR_OUTPUT
static u8   stack2[STACK_SIZE] ATTRIBUTE_ALIGN(32);
#define STACK_BOTTOM2   (stack2 + STACK_SIZE)
#endif

extern void start_telnet(void);

void start_telnet()
{
#ifdef USE_SOCDUP_FOR_OUTPUT
    OS_InitMessageQueue(&mesgQueue, mesgBuffer, sizeof(mesgBuffer) / sizeof(OSMessage));
#endif
    MI_CpuClear8(&thread, sizeof(OSThread));
    OS_CreateThread(&thread, telnet, NULL, STACK_BOTTOM, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread);
#ifdef USE_SOCDUP_FOR_OUTPUT
    OS_CreateThread(&thread2, telnet2, NULL, STACK_BOTTOM2, STACK_SIZE, 16);
    OS_WakeupThreadDirect(&thread2);
#endif
}

extern void stop_telnet(void);

void stop_telnet()
{
    OS_DestroyThread(&thread);
#ifdef USE_SOCDUP_FOR_OUTPUT
    OS_DestroyThread(&thread2);
#endif
}
