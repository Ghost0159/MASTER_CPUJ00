/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     vt100.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vt100.c,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:24  adachi_hiroaki
  telnetƒfƒ‚‚Ì’Ç‰Á

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <string.h>
#include "vt100.h"
#include "console.h"

#define ROWS_ABOVE      0
#define ROWS_BELOW      24
#define COLS_LEFT       32
#define NCOLS           32
#define NROWS           24

#define ARG(n)          (pHandle->args[(n)])
#define DEF1_ARG(n)     (ARG(n) ? ARG(n) : 1)
#define DEFx_ARG(n, x)  (ARG(n) ? ARG(n) : (x))

void    ConsoleVt100El(ConsoleHandle* pHandle, int arg);
void    ConsoleVt100Ed(ConsoleHandle* pHandle, int arg);
void    ConsoleVt100SetMode(ConsoleHandle* pHandle, int arg, int oper);

int ConsoleOutputEscape(ConsoleHandle* pHandle, char c)
{
    int newstate = VT100_STATE_NORMAL;
    int i = 0;

    switch (c)
    {
    case '[':       /* CSI */
        pHandle->nargs = 0;
        MI_CpuClear8(pHandle->args, sizeof(pHandle->args));
        pHandle->modif1 = pHandle->modif2 = '\0';
        newstate = VT100_STATE_CSI;
        break;

    case '7':       /* DECSC */
        pHandle->flags |= VT100_FLAG_SAVEDCURS;
        pHandle->saved = pHandle->current;
        break;

    case '8':       /* DECRC */
        if ((pHandle->flags & VT100_FLAG_SAVEDCURS) == 0)
        {
            break;
        }

        pHandle->current = pHandle->saved;
        break;

    case '=':       /* DECKPAM application mode */
        pHandle->flags |= VT100_FLAG_APPLKEYPAD;
        break;

    case '>':       /* DECKPNM numeric mode */
        pHandle->flags &= ~VT100_FLAG_APPLKEYPAD;
        break;

    case 'E':       /* NEL */
        pHandle->current.nX = 0;

    case 'D':       /* IND */
        ConsoleNextLine(pHandle);
        break;

    case '~':       /* LS1R */
        pHandle->chartab1 = 1;
        break;

    case 'n':       /* LS2 */
        pHandle->chartab0 = 2;
        break;

    case '}':       /* LS2R */
        pHandle->chartab1 = 2;
        break;

    case 'o':       /* LS3 */
        pHandle->chartab0 = 3;
        break;

    case '|':       /* LS3R */
        pHandle->chartab1 = 3;
        break;

    case 'N':       /* SS2 */
        pHandle->sschartab = 2;
        break;

    case 'O':       /* SS3 */
        pHandle->sschartab = 3;
        break;

    case 'M':       /* RI */
        if (pHandle->current.nY > 0)
        {
            --pHandle->current.nY;
            break;
        }

        ConsoleScrollDown(pHandle, 1);
        break;

    case 'P':       /* DCS */
        pHandle->nargs = 0;
        MI_CpuClear8(pHandle->args, sizeof(pHandle->args));

        //newstate = VT100_STATE_DCS;
        break;

    case 'c':       /* RIS */
        pHandle->current.nX = 0;
        pHandle->current.nY = 0;
        break;

    case '(':
    case ')':
    case '*':
    case '+':       /* SCS */
    case '-':
    case '.':
    case '/':       /* SCS */
        break;

    case ']':       /* OSC operating system command */
    case '^':       /* PM privacy message */
    case '_':       /* APC application program command */
        /* ignored */
        newstate = VT100_STATE_STRING;
        break;

    case '<':       /* exit VT52 mode - ignored */
        break;

    default:
        OS_TWarning("ESC%c unknown\n", c);
        break;
    }

    return(newstate);
}

void ConsoleVt100El(ConsoleHandle* pHandle, int arg)
{
    switch (arg)
    {
    case 0:         /* cursor to end */
        ConsoleEraceColumn(pHandle, pHandle->current.nX, pHandle->nWidth - 1);
        break;

    case 1:         /* beginning to cursor */
        ConsoleEraceColumn(pHandle, 0, pHandle->current.nX);
        break;

    case 2:         /* complete line */
        ConsoleEraceColumn(pHandle, 0, pHandle->nWidth - 1);
        break;

    default:
        OS_TWarning("el(%d) unknown\n", arg);
        break;
    }
}

void ConsoleVt100Ed(ConsoleHandle* pHandle, int arg)
{
    switch (arg)
    {
    case 0:         /* cursor to end */
        ConsoleEraceColumn(pHandle, pHandle->current.nX, pHandle->nWidth - 1);
        if (pHandle->current.nY < pHandle->nHeight - 1)
        {
            ConsoleEraceLine(pHandle, pHandle->current.nY + 1, pHandle->nHeight - 1);
        }
        break;

    case 1:         /* beginning to cursor */
        if (pHandle->current.nY > 0)
        {
            ConsoleEraceLine(pHandle, 0, pHandle->current.nY - 1);
        }

        ConsoleEraceColumn(pHandle, 0, pHandle->current.nX);
        break;

    case 2:         /* complete display */
        ConsoleEraceLine(pHandle, 0, pHandle->nHeight - 1);
        break;

    default:
        OS_TWarning("ed(%d) unknown\n", arg);
        break;
    }
}

void ConsoleVt100SetMode(ConsoleHandle* pHandle, int arg, int oper)
{
    int res = 0;    /* default: unknown */
    int flags;

    flags = pHandle->flags;
    switch (arg)
    {
    case 1:         /* DECCKM application/nomal cursor keys */
        if (oper == VTMODE_SET)
        {
            flags |= VT100_FLAG_APPLCURSOR;
        }
        else if (oper == VTMODE_RESET)
        {
            flags &= ~VT100_FLAG_APPLCURSOR;
        }

        res = ((flags & VT100_FLAG_APPLCURSOR) ? 1 : 2);
        break;

    case 2:         /* DECANM ANSI vt100/vt52 */
        res = 3;    /* permanently set ??? */
        break;

    case 3:         /* DECCOLM 132/80 cols */
    case 4:         /* DECSCLM smooth/jump scroll */
    case 5:         /* DECSCNM light/dark background */
        res = 4;    /* all permanently reset ??? */
        break;

    case 6:         /* DECOM move within/outside margins */
        if (oper == VTMODE_SET)
        {
            flags |= VT100_FLAG_DECOM;
        }
        else if (oper == VTMODE_RESET)
        {
            flags &= ~VT100_FLAG_DECOM;
        }

        res = ((flags & VT100_FLAG_DECOM) ? 1 : 2);
        break;

    case 7:         /* DECAWM autowrap */
        if (oper == VTMODE_SET)
        {
            flags |= VT100_FLAG_DECAWM;
        }
        else if (oper == VTMODE_RESET)
        {
            flags &= ~VT100_FLAG_DECAWM;
        }

        res = ((flags & VT100_FLAG_DECAWM) ? 1 : 2);
        break;

    case 8:         /* DECARM keyboard autorepeat */
        break;

    case 18:        /* DECPFF print form feed */
        break;

    case 19:        /* DECPEX printer extent: screen/scrolling region */
        break;

    case 25:        /* DECTCEM text cursor on/off */
        if (oper == VTMODE_SET)
        {
            flags |= VT100_FLAG_CURSORON;
        }
        else if (oper == VTMODE_RESET)
        {
            flags &= ~VT100_FLAG_CURSORON;
        }

        /*        if (flags != pHandle->flags)
            (*pHandle->emulopers->cursor) (pHandle->emulcookie,
                                           flags & VT100_FLAG_CURSORON, pHandle->crow, pHandle->ccol);*/
        res = ((flags & VT100_FLAG_CURSORON) ? 1 : 2);
        break;

    case 42:        /* DECNRCM use 7-bit NRC /
                                          7/8 bit from DEC multilingual or ISO-latin-1 */
        if (oper == VTMODE_SET)
        {
            flags |= VT100_FLAG_NATCHARSET;
        }
        else if (oper == VTMODE_RESET)
        {
            flags &= ~VT100_FLAG_NATCHARSET;
        }

        res = ((flags & VT100_FLAG_NATCHARSET) ? 1 : 2);
        break;

    case 66:        /* DECNKM numeric keypad */
        break;

    case 68:        /* DECKBUM keyboard usage data processing/typewriter */
        break;

    default:
#ifdef VT100_PRINTUNKNOWN
        printf("DEC mode %d unknown\n", nr);
#endif
        break;
    }

    pHandle->flags = flags;

    return;         // (res);
}

/*
 * CSI (ESC[)
 */
void ConsoleHandleCSI(ConsoleHandle* pHandle, char c)
{
    int     n, help, flags, fgcol, bgcol;
    long    attr = 0, bkgdattr = 0;

#define A3(a, b, c) (((a) << 16) | ((b) << 8) | (c))
    switch (A3(pHandle->modif1, pHandle->modif2, c))
    {
    case A3('\0', '\0', 'J'):         /* ED selective erase in display */
    case A3('?', '\0', 'J'):          /* DECSED selective erase in display */
        ConsoleVt100Ed(pHandle, ARG(0));
        break;

    case A3('\0', '\0', 'K'):         /* EL selective erase in line */
    case A3('?', '\0', 'K'):          /* DECSEL selective erase in line */
        ConsoleVt100El(pHandle, ARG(0));
        break;
#if 0

    case A3('\0', '\0', 'h'):         /* SM */
        for (n = 0; n < pHandle->nargs; n++)
            vt100_ansimode(pHandle, ARG(n), VTMODE_SET);
        break;
#endif

    case A3('?', '\0', 'h'):          /* DECSM */
        for (n = 0; n < pHandle->nargs; n++)
            ConsoleVt100SetMode(pHandle, ARG(n), VTMODE_SET);
        break;
#if 0

    case A3('\0', '\0', 'l'):         /* RM */
        for (n = 0; n < pHandle->nargs; n++)
            vt100_ansimode(pHandle, ARG(n), VTMODE_RESET);
        break;
#endif

    case A3('?', '\0', 'l'):          /* DECRM */
        for (n = 0; n < pHandle->nargs; n++)
            ConsoleVt100SetMode(pHandle, ARG(n), VTMODE_RESET);
        break;
#if 0

    case A3('\0', '$', 'p'):          /* DECRQM request mode ANSI */
        vt100_ansimode(pHandle, ARG(0), VTMODE_REPORT);
        break;

    case A3('?', '$', 'p'):           /* DECRQM request mode DEC */
        vt100_decmode(pHandle, ARG(0), VTMODE_REPORT);
        break;

    case A3('\0', '\0', 'i'):         /* MC printer controller mode */
    case A3('?', '\0', 'i'):          /* MC printer controller mode */
        switch (ARG(0))
        {
        case 0:         /* print screen */
        case 1:         /* print cursor line */
        case 4:         /* off */
        case 5:         /* on */
            OS_TWarning("CSI%di ignored\n", ARG(0));
            break;

        default:
            OS_TWarning("CSI%di unknown\n", ARG(0));
            break;
        }
        break;
#endif

    case 'A':           /* CUU */
        pHandle->current.nY -= MATH_MIN(DEF1_ARG(0), ROWS_ABOVE);
        break;

    case 'B':           /* CUD */
        pHandle->current.nY += MATH_MIN(DEF1_ARG(0), ROWS_BELOW);
        break;

    case 'C':           /* CUF */
        pHandle->current.nX += MATH_MIN(DEF1_ARG(0), COLS_LEFT);
        break;

    case 'D':           /* CUB */
        pHandle->current.nX -= MATH_MIN(DEF1_ARG(0), pHandle->current.nX);
        pHandle->flags &= ~VT100_FLAG_LASTCHAR;
        break;

    case 'H':           /* CUP */
    case 'f':           /* HVP */
        /*        if (pHandle->flags & VT100_FLAG_DECOM)
            pHandle->current.nY = pHandle->scrreg_startrow + MATH_MIN(DEF1_ARG(0), pHandle->scrreg_nrows) - 1;
        else*/
        pHandle->current.nY = MATH_MIN(DEF1_ARG(0), pHandle->nHeight) - 1;

        pHandle->current.nX = MATH_MIN(DEF1_ARG(1), NCOLS) - 1;
        pHandle->flags &= ~VT100_FLAG_LASTCHAR;
        break;

    case 'L':           /* IL insert line */
    case 'M':           /* DL delete line */
        n = MATH_MIN(DEF1_ARG(0), ROWS_BELOW + 1);
        {
            /*int     savscrstartrow, savscrnrows;
               savscrstartrow = pHandle->scrreg_startrow;
               savscrnrows = pHandle->scrreg_nrows;
               pHandle->scrreg_nrows -= ROWS_ABOVE;
               pHandle->scrreg_startrow = pHandle->current.nY;
             */
            if (c == 'L')
            {
                ConsoleScrollDown(pHandle, n);
                pHandle->current.nY -= n;
            }
            else
            {
                ConsoleScrollUp(pHandle, n);
                pHandle->current.nY += n;
            }

            /*pHandle->scrreg_startrow = savscrstartrow;
               pHandle->scrreg_nrows = savscrnrows; */
        }
        break;

    case 'P':           /* DCH delete character */
        n = MATH_MIN(DEF1_ARG(0), COLS_LEFT + 1);
        help = NCOLS - (pHandle->current.nX + n);

        //if (help > 0)
        //          COPYCOLS(pHandle->current.nX + n, pHandle->current.nX, help);
        //ERASECOLS(NCOLS - n, n, pHandle->bkgdattr);
        break;

    case 'X':           /* ECH erase character */
        n = MATH_MIN(DEF1_ARG(0), COLS_LEFT + 1);

        //ERASECOLS(pHandle->current.nX, n, pHandle->bkgdattr);
        break;

    case 'm':           /* SGR select graphic rendition */
        flags = pHandle->current.attrflags;
        fgcol = pHandle->current.fgcol;
        bgcol = pHandle->current.bgcol;
        for (n = 0; n < pHandle->nargs; n++)
        {
            switch (ARG(n))
            {
            case 0:     /* reset */
                if (n == pHandle->nargs - 1)
                {
                    //pHandle->current.bkgdattr = pHandle->curattr = pHandle->current.defattr;
                    pHandle->current.attrflags = 0;
                    pHandle->current.fgcol = CONSOLE_COLOR_WHITE;
                    pHandle->current.bgcol = CONSOLE_COLOR_BLACK;
                    return;
                }

                flags = 0;
                fgcol = CONSOLE_COLOR_WHITE;
                bgcol = CONSOLE_COLOR_BLACK;
                break;

            case 1:     /* bold */
                flags |= WSATTR_HILIT;
                break;

            case 4:     /* underline */
                flags |= WSATTR_UNDERLINE;
                break;

            case 5:     /* blink */
                flags |= WSATTR_BLINK;
                break;

            case 7:     /* reverse */
                flags |= WSATTR_REVERSE;
                break;

            case 22:    /* ~bold VT300 only */
                flags &= ~WSATTR_HILIT;
                break;

            case 24:    /* ~underline VT300 only */
                flags &= ~WSATTR_UNDERLINE;
                break;

            case 25:    /* ~blink VT300 only */
                flags &= ~WSATTR_BLINK;
                break;

            case 27:    /* ~reverse VT300 only */
                flags &= ~WSATTR_REVERSE;
                break;

            case 30:
            case 31:
            case 32:
            case 33:
            case 34:
            case 35:
            case 36:
            case 37:
                /* fg color */
                flags |= WSATTR_WSCOLORS;
                fgcol = ARG(n) - 30;
                break;

            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 45:
            case 46:
            case 47:
                /* bg color */
                flags |= WSATTR_WSCOLORS;
                bgcol = ARG(n) - 40;
                break;

            default:
                OS_TWarning("CSI%dm unknown\n", ARG(n));
                break;
            }
        }
        {
            //            pHandle->current.curattr = attr;
            //            pHandle->current.bkgdattr = bkgdattr;
            pHandle->current.attrflags = flags;
            pHandle->current.fgcol = fgcol;
            pHandle->current.bgcol = bgcol;
        }
        break;
#if 0

    case 'r':
        help = min(DEF1_ARG(0), pHandle->nHeight) - 1;
        n = min(DEFx_ARG(1, pHandle->nHeight), pHandle->nHeight) - help;
        if (n < 2)
        {
            return;
        }
        else
        {
            pHandle->scrreg_startrow = help;
            pHandle->scrreg_nrows = n;
        }

        pHandle->current.nY = ((pHandle->flags & VT100_FLAG_DECOM) ? pHandle->scrreg_startrow : 0);
        pHandle->current.nX = 0;
        break;
#endif

    default:
        OS_TWarning("CSI %c (%d, %d) unknown\n", c, ARG(0), ARG(1));
        break;
    }
}

int ConsoleOutputCSI(ConsoleHandle* pHandle, char c)
{
    int newstate = VT100_STATE_CSI;

    switch (c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        /* argument digit */
        if (pHandle->nargs < VT100_EMUL_NARGS)
        {
            pHandle->args[pHandle->nargs] = (pHandle->args[pHandle->nargs] * 10) + (c - '0');
        }
        break;

    case ';':           /* argument terminator */
        pHandle->nargs++;
        break;

    case '?':           /* DEC specific */
    case '>':           /* DA query */
        pHandle->modif1 = c;
        break;

    case '!':
    case '"':
    case '$':
    case '&':
        pHandle->modif2 = c;
        break;

    default:            /* end of escape sequence */
        pHandle->nargs++;
        if (pHandle->nargs > VT100_EMUL_NARGS)
        {
            OS_TWarning("vt100: too many arguments\n");
            pHandle->nargs = VT100_EMUL_NARGS;
        }

        ConsoleHandleCSI(pHandle, c);
        newstate = VT100_STATE_NORMAL;
        break;
    }

    return(newstate);
}
