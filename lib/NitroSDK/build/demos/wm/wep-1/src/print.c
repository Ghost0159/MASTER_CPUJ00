/*
  Project:  NitroSDK - WM - demos - wep-1
  File:     print.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: print.c,v $
  Revision 1.2  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model Ç… WEP Key ÇÃìÆìIê›íËÇâ¡Ç¶ÅAwep-1 ÉfÉÇÇ∆ÇµÇƒêVãKìoò^

  $NoKeywords: $
*/

#include <nitro.h>
#include "print.h"
#include "wh.h"

void PR_ClearScreen(PRScreen * scr)
{
    MI_CpuClear16((void *)scr->screen, PR_SCREEN_SIZE * sizeof(u16));
    scr->curx = 0;
    scr->cury = 0;
}

void PR_Locate(PRScreen * scr, int x, int y)
{
    scr->curx = x;
    scr->cury = y;
}

static void PR_ScrollUp(PRScreen * scr)
{
    int     y;

    if (!scr->scroll)
    {
        PR_ClearScreen(scr);
        return;
    }

    for (y = 0; y < PR_SCREEN_HEIGHT - 1; ++y)
    {
        MI_CpuCopy16((void *)(scr->screen + (y + 1) * PR_SCREEN_WIDTH),
                     (void *)(scr->screen + y * PR_SCREEN_WIDTH), PR_SCREEN_WIDTH * sizeof(u16));
    }

    (void)MI_CpuClear16((void *)(scr->screen
                                 + (PR_SCREEN_HEIGHT - 1) * PR_SCREEN_WIDTH),
                        PR_SCREEN_WIDTH * sizeof(u16));
}

void PR_PutString(PRScreen * scr, const char *text)
{
    const char *p;
    u8      pal;

    p = text;
    pal = 0x0f;

    while (*p != '\0')
    {
        if (*p == '\n')
        {
            scr->curx = 0;
            ++scr->cury;
            if (scr->cury > PR_SCREEN_HEIGHT - 1)
            {
                scr->cury = PR_SCREEN_HEIGHT - 1;
                PR_ScrollUp(scr);
            }
            ++p;
            continue;

        }
        else if (*p == '\\')
        {
            ++p;

            if (('0' <= *p) && (*p <= '9'))
            {
                pal = (u8)(*p - '0');
                ++p;

            }
            else if (('a' <= *p) && (*p <= 'f'))
            {
                pal = (u8)(*p - 'a' + 10);
                ++p;
            }

            continue;
        }

        scr->screen[scr->cury * PR_SCREEN_WIDTH + scr->curx] = (u16)((pal << 12) | (0x0fff & *p));
        ++p;

        ++scr->curx;
        if (scr->curx > PR_SCREEN_WIDTH - 1)
        {
            scr->curx = 0;
            ++scr->cury;
            if (scr->cury > PR_SCREEN_HEIGHT - 1)
            {
                scr->cury = PR_SCREEN_HEIGHT - 1;
                PR_ScrollUp(scr);
            }
        }
    }
}

void PR_VPrintString(PRScreen * scr, const char *fmt, va_list vlist)
{
    char    buf[256];

    // vsnprintf ÇæÇ∆ìÆÇ©Ç»Ç¢ÅB
    // (void)vsnprintf(buf, 256 - 2, fmt, vlist);

    (void)OS_VSNPrintf(buf, 256 - 2, fmt, vlist);
    buf[255] = '\0';

    PR_PutString(scr, buf);
    // OS_Printf("%s", buf);
}

void PR_PrintString(PRScreen * scr, const char *fmt, ...)
{
    va_list vlist;
    va_start(vlist, fmt);
    PR_VPrintString(scr, fmt, vlist);
    va_end(vlist);
}
