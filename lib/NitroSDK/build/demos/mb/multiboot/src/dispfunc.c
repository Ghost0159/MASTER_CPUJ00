/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     dispfunc.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dispfunc.c,v $
  Revision 1.7  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.6  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.5  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.4  2005/02/18 10:33:52  yosizaki
  fix around hidden warnings.

  Revision 1.3  2004/10/05 06:48:32  sato_masaki
  small fix

  Revision 1.2  2004/10/05 06:47:54  sato_masaki
  設定メニューを追加。その他色々細かな修正。

  Revision 1.1  2004/08/31 11:00:12  sato_masaki
  initial check-in


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <string.h>

#include "dispfunc.h"

/*
	=================================================================================

	OBJ文字描画

	=================================================================================
*/

// --- global value
GXOamAttr oamBak[128];

// --- constant value
static const char NumString[] = "0123456789ABCDEF";


//      OBJ セット
void ObjSet(int objNo, int x, int y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&oamBak[objNo],
                  x,
                  y,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}

//      OBJ文字列のセット
void ObjSetString(int startobjNo, int x, int y, char *string, int paletteNo)
{
    int     i;
    for (i = 0; string[i] != 0 && i < 24; i++)
    {
        ObjSet(startobjNo + i, x + i * 8, y, (int)string[i], paletteNo);
    }
}

//      OBJ クリア
void ObjClear(int objNo)
{
    ObjSet(objNo, 256 * 8, 192 * 8, 0, 0);
}

//      OBJ クリア(範囲指定)
void ObjClearRange(int start, int end)
{
    int     i;
    for (i = start; i <= end; i++)
        ObjClear(i);
}


//      メッセージセット
void ObjSetMessage(int objNo, char *message)
{
    ObjSetString(objNo, 4 * 8, 22 * 8, message, WHITE);
}

/*
	=================================================================================

	BG文字描画 (BG0固定)

	=================================================================================
*/


#define BG_LINES	(24)
#define PROMPT		">"

// --- static value
static u16 vscr[32 * 32] ATTRIBUTE_ALIGN(32);   // 仮想スクリーン
static u16 vscr_sub[32 * 32] ATTRIBUTE_ALIGN(32);       // 仮想スクリーン(sub)
static u16 cur_line = 0;

// --- static functions
static void BgiPutChar(s16 x, s16 y, u8 palette, char c, u8 lcd);
static void BgiPutStringN(s16 x, s16 y, u8 palette, char *text, u32 num, u8 lcd);


static void BgiPutChar(s16 x, s16 y, u8 palette, char c, u8 lcd)
{
    if (lcd)
    {
        vscr[((y * 32) + x) % (32 * 32)] = (u16)((palette << 12) | c);
    }
    else
    {
        vscr_sub[((y * 32) + x) % (32 * 32)] = (u16)((palette << 12) | c);
    }
}

static void BgiPutStringN(s16 x, s16 y, u8 palette, char *text, u32 num, u8 lcd)
{
    s32     i;

    for (i = 0; i < num; i++)
    {
        if (text[i] == 0x00)
        {
            break;
        }

        BgiPutChar((s16)(x + i), y, palette, text[i], lcd);
    }

}

/*
	--- initialize
*/
void BgInitForPrintStr(void)
{
    // main lcd
    GX_SetBankForBG(GX_VRAM_BG_64_E);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000,      // SCR ベースブロック 31
                     GX_BG_CHARBASE_0x00000,    // CHR ベースブロック 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    GX_LoadBG0Char(sampleCharData, 0, sizeof(sampleCharData));
    GX_LoadBGPltt(samplePlttData, 0, sizeof(samplePlttData));

    MI_CpuFillFast((void *)vscr, 0, sizeof(vscr));
    DC_FlushRange(vscr, sizeof(vscr));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(vscr, 0, sizeof(vscr));

    // sub lcd
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,     // SCR ベースブロック 31
                      GX_BG_CHARBASE_0x10000,   // CHR ベースブロック 0
                      GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_BG0Mosaic(FALSE);
    GXS_SetGraphicsMode(GX_BGMODE_0);

    GXS_LoadBG0Char(sampleCharData, 0, sizeof(sampleCharData));
    GXS_LoadBGPltt(samplePlttData, 0, sizeof(samplePlttData));
    MI_CpuFillFast((void *)vscr_sub, 0, sizeof(vscr_sub));
    DC_FlushRange(vscr_sub, sizeof(vscr_sub));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadBG0Scr(vscr_sub, 0, sizeof(vscr_sub));

    cur_line = 0;
}

/* 
	--- for main LCD
*/

// Format付き文字列出力
void BgPrintf(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 * 2 + 2];

    MI_CpuClear8(&temp[0], sizeof(temp));

    va_start(vlist, text);
    (void)OS_VSNPrintf(temp, 32 * 2 + 2, text, vlist);
    va_end(vlist);

    BgPutString(x, y, palette, temp);
}


// 文字列出力
void BgPutString(s16 x, s16 y, u8 palette, char *text)
{
    BgPutStringN(x, y, palette, text, 32);
}

// 文字数制限付き文字列出力
void BgPutStringN(s16 x, s16 y, u8 palette, char *text, u32 num)
{
    BgiPutStringN(x, y, palette, text, num, 1);
}


// 1文字出力
void BgPutChar(s16 x, s16 y, u8 palette, char c)
{
    BgiPutChar(x, y, palette, c, 1);
}

// BGのクリア
void BgClear(void)
{
    MI_CpuClearFast((void *)vscr, sizeof(vscr));
}

/* 
	--- for sub LCD
*/

// Format付き文字列出力
void BgPrintfSub(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 + 2];

    MI_CpuClear8(&temp[0], sizeof(temp));

    va_start(vlist, text);
    (void)OS_VSNPrintf(temp, 33, text, vlist);
    va_end(vlist);

    BgPutStringSub(x, y, palette, temp);
}

// 文字列出力
void BgPutStringSub(s16 x, s16 y, u8 palette, char *text)
{
    BgPutStringNSub(x, y, palette, text, 32);
}

// 文字数制限付き文字列出力
void BgPutStringNSub(s16 x, s16 y, u8 palette, char *text, u32 num)
{
    BgiPutStringN(x, y, palette, text, num, 0);
}

// 1文字出力
void BgPutCharSub(s16 x, s16 y, u8 palette, char c)
{
    BgiPutChar(x, y, palette, c, 0);
}

// BGのクリア
void BgClearSub(void)
{
    MI_CpuClearFast((void *)vscr_sub, sizeof(vscr_sub));
}

/*
	--- v blank process
*/

void BgScrSetVblank(void)
{
    // 仮想スクリーンをVRAMに反映
    DC_FlushRange(vscr, sizeof(vscr));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(vscr, 0, sizeof(vscr));
    DC_FlushRange(vscr_sub, sizeof(vscr_sub));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadBG0Scr(vscr_sub, 0, sizeof(vscr_sub));
}


/*
	--- message print
*/

// メッセージを出力
void BgSetMessage(u8 palette, char *message, ...)
{
    va_list vlist;
    char    temp[32 * 2 + 2];
    MI_CpuClear8(&temp[0], sizeof(temp));

    va_start(vlist, message);
    (void)OS_VSNPrintf(temp, 32 * 2 + 2, message, vlist);
    va_end(vlist);

    BgPutString(4, 22, palette, "                            ");
    BgPutString(4, 22, palette, temp);


    BgSetMessageLogSub(palette, temp);

}

void BgSetMessageSub(u8 palette, char *message, ...)
{
    va_list vlist;
    char    temp[32 + 2];
    MI_CpuClear8(&temp[0], sizeof(temp));

    va_start(vlist, message);
    (void)OS_VSNPrintf(temp, 33, message, vlist);
    va_end(vlist);

    BgSetMessageLogSub(palette, temp);
}

// sub LCDにメッセージログを出力
void BgSetMessageLogSub(u8 palette, char *text)
{
    if (cur_line == BG_LINES)
    {
        MI_CpuCopy16((void *)&vscr_sub[1 * 32], (void *)vscr_sub,
                     sizeof(u16) * 32 * (BG_LINES - 1));
        cur_line--;
    }

    BgPutStringSub(0, (s16)cur_line, palette, "                                ");
    BgPutStringSub(0, (s16)cur_line, palette, PROMPT);
    BgPutStringSub((s16)(1 + strlen(PROMPT)), (s16)cur_line, palette, text);

    if (cur_line < BG_LINES)
        cur_line++;
}

// main LCD スクリーンバッファの取得
u16    *BgGetScreenBuf(void)
{
    return (u16 *)vscr;
}

// sub LCD スクリーンバッファの取得
u16    *BgGetScreenBufSub(void)
{
    return (u16 *)vscr_sub;
}
