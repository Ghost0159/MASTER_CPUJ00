/*
  Project:  NitroSDK - WM - demos - wmsample
  File:     print.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: print.c,v $
  Revision 1.9  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.8  2005/11/21 00:41:10  adachi_hiroaki
  親機選択画面に参加人数とニックネームを表示するようにした

  Revision 1.7  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 07:12:55  sasakis
  コード整理。

  Revision 1.5  2004/10/22 07:35:18  sasakis
  共有化した wh に対応。

  Revision 1.4  2004/10/21 00:43:34  yosizaki
  add SHARED_WH switch.

  Revision 1.3  2004/10/06 05:11:35  sasakis
  グラフ表示の追加など。

  Revision 1.2  2004/09/24 06:12:35  sasakis
  チャンネル手動選択機能を追加。

  Revision 1.1  2004/09/22 09:39:33  sasakis
  デバグ画面を追加。

  $NoKeywords: $
*/

#include <nitro.h>
#include "print.h"
#include "wh.h"

static u16 PRi_GetFontIndex(u16 sjisCode);
static u16 PRi_SplitCharShiftJIS(const void **ppChar);

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
    u16     code;

    p = text;
    pal = 0x0f;

    while ((code = PRi_SplitCharShiftJIS((const void **)&p)) != '\0')
    {
        if (code == '\n')
        {
            scr->curx = 0;
            ++scr->cury;
            if (scr->cury > PR_SCREEN_HEIGHT - 1)
            {
                scr->cury = PR_SCREEN_HEIGHT - 1;
                PR_ScrollUp(scr);
            }
            continue;
        }
        else if (code == '\\')
        {
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
        scr->screen[scr->cury * PR_SCREEN_WIDTH + scr->curx] =
            (u16)((pal << 12) | (0x0fff & PRi_GetFontIndex(code)));

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

    // vsnprintf だと動かない。
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

int PR_GetStringLengthA(const char *str)
{
    int     n = 0;
    while (PRi_SplitCharShiftJIS((const void **)&str))
    {
        n++;
    }
    return n;
}

static u16 PRi_GetFontIndex(u16 sjisCode)
{
    //sjis to sdkcode
    const static u8 table[0x100 - 0x40] = {
        //   0     1     2     3     4     5     6     7
/*0x40*/ 0xa7, 0xb1, 0xa8, 0xb2, 0xa9, 0xb3, 0xaa, 0xb4,
        0xab, 0xb5, 0xb6, 0xf2, 0xb7, 0xf3, 0xb8, 0xf4,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0x50*/ 0xb9, 0xf5, 0xba, 0xf6, 0xbb, 0xf7, 0xbc, 0xf8,
        0xbd, 0xf9, 0xbe, 0xfa, 0xbf, 0xfb, 0xc0, 0xfc,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0x60*/ 0xc1, 0xfd, 0xaf, 0xc2, 0xfe, 0xc3, 0xff, 0xc4,
        0x01, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0x02,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0x70*/ 0x02, 0xcb, 0x03, 0x03, 0xcc, 0x04, 0x04, 0xcd,
        0x05, 0x1e, 0xce, 0x06, 0x06, 0xcf, 0xd0, 0x00,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0x80*/ 0xd1, 0xd2, 0xd3, 0xac, 0xd4, 0xad, 0xd5, 0xae,
        0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0x00, 0xdc,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0x90*/ 0x00, 0x00, 0xa6, 0xdd, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0xa0*/ 0x86, 0x7d, 0x87, 0x7e, 0x88, 0x7f, 0x89, 0x80,
        0x8a, 0x8b, 0x07, 0x8c, 0x08, 0x8d, 0x09, 0x8e,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0xb0*/ 0x0a, 0x8f, 0x0b, 0x90, 0x0c, 0x91, 0x0d, 0x92,
        0x0e, 0x93, 0x0f, 0x94, 0x10, 0x95, 0x11, 0x96,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0xc0*/ 0x12, 0x84, 0x97, 0x13, 0x98, 0x14, 0x99, 0x15,
        0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0x16, 0x1b,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0xd0*/ 0xa0, 0x17, 0x1c, 0xe0, 0x18, 0x1d, 0xe1, 0x19,
        0x1e, 0xe2, 0x1a, 0x1f, 0xe3, 0xe4, 0xe5, 0xe6,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0xe0*/ 0xe7, 0x81, 0xe8, 0x82, 0xe9, 0x83, 0xea, 0xeb,
        0xec, 0xed, 0xee, 0xef, 0x00, 0xf0, 0x00, 0x00,
        //   8     9     a     b     c     d     e     f

        //   0     1     2     3     4     5     6     7
/*0xf0*/ 0x7b, 0xf1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            //   8     9     a     b     c     d     e     f
    };
    if (sjisCode <= 0xff)
    {
        return sjisCode;
    }

    switch (sjisCode)
    {
    case 0x8140:                      //　
        return ' ';
    case 0x8141:                      //、
        return 0xa4;
    case 0x8142:                      //。
        return 0xa1;
    case 0x815e:                      //／
        return '/';
    case 0x8175:                      //「
        return 0xa2;
    case 0x8176:                      //」
        return 0xa3;
    case 0x8145:                      //」
        return 0xa5;
    case 0x815b:                      //ー
        return 0xb0;
    case 0x8160:                      //～
        return 0x85;
    case 0x818f:                      //￥
        return '\\';
    }
    return table[(sjisCode - 0x40) & 0xff];
}

// code from NitroSystem
///////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------*
  Name:         IsSjisLeadByte

  Description:  SJIS のリードバイトかどうか判定します。

  Arguments:    c:  判定対象のバイトデータ。

  Returns:      c がSJISのリードバイトならTRUEを返します。
 *---------------------------------------------------------------------------*/
#define SJIS_LOW_WIDTH  0xBC
#define SJIS_LOW_BASE   0x40
#define SJIS_HIGH_BASE  0x81
#define SHIGH_AREA0_ST  0x81
#define SHIGH_AREA0_ED  0x85
#define SHIGH_AREA1     0x87
#define SHIGH_AREA2_ST  0x88
#define SHIGH_AREA2_ED  0xA0
#define SHIGH_AREA3_ST  0xE0
#define SHIGH_AREA3_ED  0x100
#define ASCII_ST    0x20
#define ASCII_ED    0x80
#define HANKANA_ST  0xA0
#define HANKANA_ED  0xE0

static inline BOOL IsSjisLeadByte(u8 c)
{
    return (((SJIS_HIGH_BASE <= c) && (c < SHIGH_AREA2_ED))
            || (SHIGH_AREA3_ST <= c)) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         PRi_SplitCharShiftJIS

  Description:  文字コード切り出し関数です。
                バイトストリームから最初の文字の文字コードを取得するとともに
                ストリームポインタを次の文字に移動させます。

  Arguments:    ppChar: バイト配列へのポインタを格納しているバッファへのポインタ。
                        関数から返ると、このポインタの先のバッファには
                        次の文字の先頭へのポインタが格納されます。

  Returns:      *ppChar の最初の文字の文字コード。
 *---------------------------------------------------------------------------*/

// Shift_JIS
static u16 PRi_SplitCharShiftJIS(const void **ppChar)
{
    const u8 *pChar;
    u16     c;

    SDK_NULL_ASSERT(ppChar);
    SDK_NULL_ASSERT(*ppChar);

    pChar = (const u8 *)*ppChar;

    if (IsSjisLeadByte(*pChar))
    {
        c = (u16)(((*pChar) << 8) | (*(pChar + 1)));
        *(u32 *)ppChar += 2;
    }
    else
    {
        c = *pChar;
        *(u32 *)ppChar += 1;
    }

    return c;
}
