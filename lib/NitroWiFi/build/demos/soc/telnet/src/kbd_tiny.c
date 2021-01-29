/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     kbd_tiny.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: kbd_tiny.c,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include "kbd_tiny.h"

#define FONTWIDTH   8

static const char*  unshiftKeys[] = {"1234567890-^\\", "qwertyuiop@[", "asdfghjkl;:]", "zxcvbnm,./ "};
static const char*  shiftKeys[] = {"!\"#$%&'() =  ", "QWERTYUIOP` " , "ASDFGHJKL+* " , "ZXCVBNM<>?_"};
static u16          TinykbdScreen[32 * 24];
static TPData       TinykbdTpCurrent, TinykbdTpOld;
static const u8     TinykbdFontColor = 7;
static BOOL         TinykbdIsShift = FALSE;
static u16          TinykbdPadCurrent = 0, TinykbdPadPrevious = 0, TinykbdPadTrigger = 0;

static void TinykbdDraw(BOOL isShift)
{
    const char **    keys = NULL;
    int row = 0;

    keys = isShift ? shiftKeys : unshiftKeys;
    for (row = 0; row < 4; ++row)
    {
        const char*     cursor = NULL;
        int col = 0;
        for (cursor = keys[row]; *cursor; ++cursor, ++col)
        {
            TinykbdScreen[(row * 2 * 32) + (col * 2 + row)] = (u16) (*cursor | (TinykbdFontColor << 12));
        }
    }

    GXS_LoadBG1Scr(TinykbdScreen, 0, sizeof(TinykbdScreen));
}

void TinykbdUpdate()
{
    TPData  raw_point, disp_point;

    // タッチ入力取得
    while (TP_RequestRawSampling(&raw_point) != 0)
    {
    }

    TP_GetCalibratedPoint(&disp_point, &raw_point);

    TinykbdPadPrevious = TinykbdPadCurrent;
    TinykbdPadCurrent = PAD_Read();
    TinykbdPadTrigger = (u16) ((TinykbdPadPrevious ^ TinykbdPadCurrent) & TinykbdPadCurrent);

    TinykbdTpOld = TinykbdTpCurrent;
    TinykbdTpCurrent = disp_point;
    TinykbdIsShift = (TinykbdPadCurrent & PAD_BUTTON_R) ? TRUE : FALSE;

    TinykbdDraw(TinykbdIsShift);
}

void TinykbdInit()
{
    TinykbdDraw(FALSE);
}

u8 TinykbdGetInput()
{
    if (!TinykbdTpCurrent.touch && TinykbdTpOld.touch &&
        (TinykbdTpCurrent.validity == TP_VALIDITY_VALID || TinykbdTpOld.validity == TP_VALIDITY_VALID))
    {
        TPData  tpValid = (TinykbdTpCurrent.validity == TP_VALIDITY_VALID) ? TinykbdTpCurrent : TinykbdTpOld;
        {
            int rel_x = (tpValid.x) & 0xf;
            int rel_y = (tpValid.y) & 0xf;
            if (rel_x < 2 || rel_x > 0xd || rel_y < 2 || rel_y > 0xd)
            {
                return 0;
            }
        }
        {
            int row = (tpValid.y) / (FONTWIDTH * 2);
            if (0 <= row && row < 4)
            {
                int col = (((tpValid.x) / FONTWIDTH) - row) / 2;
                if (0 <= col && col < STD_StrLen(unshiftKeys[row]))
                {
                    if (TinykbdIsShift)
                    {
                        return(u8) shiftKeys[row][col];
                    }
                    else
                    {
                        return(u8) unshiftKeys[row][col];
                    }
                }
            }
            else if (row > 4)
            {
                return 0x20;
            }
        }
    }
    else if (TinykbdPadTrigger & PAD_BUTTON_B)
    {
        return 0x08;
    }

    return 0;
}
