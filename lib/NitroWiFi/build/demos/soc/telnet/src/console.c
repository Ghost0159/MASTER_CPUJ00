/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     console.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: console.c,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetƒfƒ‚‚Ì’Ç‰Á

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <string.h>

#include "vt100.h"
#include "console.h"

#define bufHeight   24

const int   lineBytes = ((HW_LCD_WIDTH / 8) * 2);
static ConsoleHandle*   ConsoleCurrent = NULL;
static u16  ConsoleBuffer[bufHeight][HW_LCD_WIDTH / 8] ATTRIBUTE_ALIGN(32);

inline int ConsoleGetLeftColumns(const ConsoleHandle* pHandle)
{
    return(pHandle->nWidth - pHandle->current.nX - 1);
}

static inline void ConsoleSetChar(char c, int nX, int nY, int fgcol)
{
    ConsoleBuffer[nY][nX] = (u16) ((u8) c | (fgcol << 12));
}

void ConsoleSetDefault(ConsoleHandle* pHandle)
{
    ConsoleCurrent = pHandle;
}

static inline u16* ConsoleGetBufferAddress(ConsoleHandle* pHandle, int nX, int nY)
{
#pragma unused(pHandle)
    return &ConsoleBuffer[nY][nX];
}

void ConsoleEraceColumn(ConsoleHandle* pHandle, int nStartColumn, int nEndColumn)
{
    int nColumn;
    SDK_ASSERT(nStartColumn <= nEndColumn);
    for (nColumn = nStartColumn; nColumn <= nEndColumn; ++nColumn)
    {
        ConsoleSetChar(' ', nColumn, pHandle->current.nY, 0);
    }
}

void ConsoleEraceLine(ConsoleHandle* pHandle, int nStartLine, int nEndLine)
{
    int     nLines = (nEndLine + 1) - nStartLine;
    u16*    pStartAddress = ConsoleGetBufferAddress(pHandle, nStartLine, 0);
    SDK_ASSERT(nStartLine <= nEndLine);
    MI_CpuClear16(pStartAddress, lineBytes * (u32) nLines);
}

inline int ConsoleGetBottom(const ConsoleHandle* pHandle)
{
    return pHandle->current.nTop + pHandle->nHeight;
}

void ConsoleUpdate(const ConsoleHandle* pHandle)
{
    (void)pHandle;
    DC_FlushRange(ConsoleBuffer, sizeof(ConsoleBuffer));
    GX_LoadBG0Scr(ConsoleBuffer, 0, sizeof(ConsoleBuffer));
    if (pHandle->flags & VT100_FLAG_APPLCURSOR && (OS_GetVBlankCount() & (1 << 5)))
    {
        u16 cursor = (7 << 12) | '_';

        //(u16**)G2_GetBG0ScrPtr())[pHandle->current.nY][pHandle->current.nX] = (7 << 12) | '_';
        GX_LoadBG0Scr(&cursor, (u32) (pHandle->current.nY * pHandle->nWidth + pHandle->current.nX) * 2, sizeof(cursor));
    }
}

void ConsoleFlush()
{
    ConsoleUpdate(ConsoleCurrent);
}

void ConsoleInit(ConsoleHandle* pHandle)
{
    MI_CpuClear8(pHandle, sizeof(ConsoleHandle));
    MI_CpuClearFast((void*)ConsoleBuffer, sizeof(ConsoleBuffer));

    pHandle->nWidth = 32;
    pHandle->nHeight = 24;
    pHandle->current.nX = 0;
    pHandle->current.nY = 0;
    pHandle->current.attrflags = 0;
    pHandle->current.fgcol = CONSOLE_COLOR_WHITE;
    pHandle->current.bgcol = CONSOLE_COLOR_BLACK;
    pHandle->flags = VT100_FLAG_APPLCURSOR;

    ConsoleCurrent = pHandle;
}

void ConsoleClear()
{
    ConsoleInit(ConsoleCurrent);
}

void ConsoleScrollUp(ConsoleHandle* pHandle, int nLine)
{
#pragma unused(pHandle)
    int len = (lineBytes * nLine);
    (void)memmove(ConsoleBuffer, (u8*)ConsoleBuffer + len, sizeof(ConsoleBuffer) - len);
    MI_CpuClear32((u8*)ConsoleBuffer + sizeof(ConsoleBuffer) - len, (u32) len);
}

void ConsoleScrollDown(ConsoleHandle* pHandle, int nLine)
{
#pragma unused(pHandle)
    int len = (lineBytes * nLine);
    (void)memmove((u8*)ConsoleBuffer + len, (u8*)ConsoleBuffer, sizeof(ConsoleBuffer) - len);
    MI_CpuClear32((u8*)ConsoleBuffer, (u32) len);
}

void ConsolePutChar(char c)
{
    ConsoleHandle*  pHandle = ConsoleCurrent;

    switch (pHandle->stateMode)
    {
    case VT100_STATE_NORMAL:
        break;

    case VT100_STATE_ESC:
        pHandle->stateMode = ConsoleOutputEscape(pHandle, c);
        return;

    case VT100_STATE_CSI:
        pHandle->stateMode = ConsoleOutputCSI(pHandle, c);

        return;

    case VT100_STATE_SCS94:
    case VT100_STATE_SCS94_PERCENT:
    case VT100_STATE_SCS96:
    case VT100_STATE_SCS96_PERCENT:
    case VT100_STATE_ESC_HASH:
    case VT100_STATE_ESC_SPC:
    case VT100_STATE_STRING:
    case VT100_STATE_STRING_ESC:
    case VT100_STATE_DCS:
    case VT100_STATE_DCS_DOLLAR:
        break;
    }

    switch (c)
    {
    case '~':
        c = '\x85';
        break;

    case '{':
    case '}':
        c = ' ';
        break;
    }

    switch (c)
    {
    default:
        if (ConsoleGetLeftColumns(pHandle) < 0)
        {
            pHandle->current.nX = 0;
            ConsoleNextLine(pHandle);
        }

        ConsoleSetChar(c, pHandle->current.nX, pHandle->current.nY, pHandle->current.fgcol);
        ++pHandle->current.nX;
        break;

    case ASCII_NUL:
        /* ignore */
        break;

    case ASCII_BEL:
        break;

    case ASCII_BS:
        if (pHandle->current.nX > 0)
        {
            pHandle->current.nX--;
            pHandle->flags &= ~VT100_FLAG_LASTCHAR;
        }
        break;

    case ASCII_CR:
        pHandle->current.nX = 0;
        pHandle->flags &= ~VT100_FLAG_LASTCHAR;
        break;

    case ASCII_HT:
        {
            int nMovedColumn = pHandle->current.nX + MATH_MIN(8 - (pHandle->current.nX & 7),
                                                              ConsoleGetLeftColumns(pHandle));
            for (pHandle->current.nX = 0; pHandle->current.nX < nMovedColumn; ++pHandle->current.nX)
            {
                ConsoleSetChar(c, pHandle->current.nX, pHandle->current.nY, 0);
            }
            break;
        }

    case ASCII_SO:  /* LS1 */
        pHandle->chartab0 = 1;
        break;

    case ASCII_SI:  /* LS0 */
        pHandle->chartab0 = 0;
        break;

    case ASCII_ESC:
        if (pHandle->stateMode == VT100_STATE_STRING)
        {
            pHandle->stateMode = VT100_STATE_STRING_ESC;
        }
        else
        {
            pHandle->stateMode = VT100_STATE_ESC;
        }
        break;

    case ASCII_LF:
    case ASCII_VT:
    case ASCII_FF:
        ConsoleNextLine(pHandle);
        break;
    }

}

void ConsolePutString(const char* s)
{
    for (; *s; ++s)
    {
        ConsolePutChar(*s);
        if ((*(s + 1) == '\n' && *s != '\r') || *s == '\n')
        {
            ConsolePutChar('\r');
        }
    }
}

void ConsolePrintf(const char* fmt, ...)
{
    char    buffer[128];
    va_list vlist;

    va_start(vlist, fmt);
    (void)OS_VSNPrintf(buffer, sizeof(buffer), fmt, vlist);
    va_end(vlist);

    ConsolePutString(buffer);
}

void ConsoleNextLine(ConsoleHandle* pHandle)
{
    if (pHandle->current.nY >= pHandle->nHeight - 1)
    {
        ConsoleScrollUp(pHandle, 1);
    }

    pHandle->current.nY = MATH_MIN(pHandle->current.nY + 1, pHandle->nHeight - 1);
}
