/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-PowerSave
  File:     dispfunc.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dispfunc.c,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.1  2005/04/28 07:15:24  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include "dispfunc.h"


/*****************************************************************************/
/* constant */

/* 各種描画用定数配列 */
extern const u32 sampleCharData[8 * 0xe0];
extern const u16 samplePlttData[16][16];


/*****************************************************************************/
/* variable */

/* Vブランク転送用の仮想 BG スクリーン */
static u16 g_screen[2][32 * 32] ATTRIBUTE_ALIGN(32);

/* 現在の描画対象 */
static u16 *draw_target;

/* メッセージログの最新行番号 */
static int cur_line = 0;


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         BgiGetTargetScreen

  Description:  メイン/サブ画面 BG スクリーンへのポインタを取得.

  Arguments:    is_lcd_main      メイン画面 なら TRUE, サブ画面なら FALSE.

  Returns:      BG スクリーンへのポインタ.
 *---------------------------------------------------------------------------*/
static  inline u16 *BgiGetTargetScreen(BOOL is_lcd_main)
{
    return g_screen[!is_lcd_main];
}

/*---------------------------------------------------------------------------*
  Name:         BgiSetTargetScreen

  Description:  文字列を表示するメイン/サブ画面 BG スクリーンを切り替え.

  Arguments:    is_lcd_main      メイン画面 なら TRUE, サブ画面なら FALSE.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static  inline void BgiSetTargetScreen(BOOL is_lcd_main)
{
    draw_target = BgiGetTargetScreen(is_lcd_main);
}

/*---------------------------------------------------------------------------*
  Name:         BgInit

  Description:  BG を文字列表示用に初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgInit(void)
{
    /* 内部変数の初期化 */
    cur_line = 0;
    MI_CpuClearFast(g_screen, sizeof(g_screen));
    DC_StoreRange(g_screen, sizeof(g_screen));
    DC_WaitWriteBufferEmpty();
    draw_target = BgiGetTargetScreen(TRUE);

    /*
     * メイン LCD 設定.
     * BG0 : ASCII 文字の表示に使用.
     *   -256x256x16 テキストBG.
     *   -スクリーンベース 30(0F000-0F800).
     *   -キャラクタベース  0(00000-04000).
     */
    GX_SetBankForBG(GX_VRAM_BG_64_E);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_SetBG0Offset(0, 0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_LoadBG0Scr(g_screen[0], 0, sizeof(g_screen[0]));
    GX_LoadBG0Char(sampleCharData, 0, sizeof(sampleCharData));
    GX_LoadBGPltt(samplePlttData, 0, sizeof(samplePlttData));
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);

    /*
     * サブ LCD 設定.
     * BG0 : ASCII 文字の表示に使用.
     *   -256x256x16 テキストBG.
     *   -スクリーンベース 31(0F800-10000).
     *   -キャラクタベース  4(10000-14000).
     */
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                      GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    G2S_SetBG0Offset(0, 0);
    G2S_BG0Mosaic(FALSE);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_LoadBG0Scr(g_screen[1], 0, sizeof(g_screen[1]));
    GXS_LoadBG0Char(sampleCharData, 0, sizeof(sampleCharData));
    GXS_LoadBGPltt(samplePlttData, 0, sizeof(samplePlttData));
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);
}

/*---------------------------------------------------------------------------*
  Name:         BgClear

  Description:  BG メイン画面スクリーンを全てキャラクタ 0 で初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgClear(void)
{
    MI_CpuClearFast(g_screen[0], sizeof(g_screen[0]));
}

/*---------------------------------------------------------------------------*
  Name:         BgUpdate

  Description:  BG スクリーンを実メモリに反映.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgUpdate(void)
{
    /* BG を実メモリに反映 */
    DC_FlushRange(g_screen, sizeof(g_screen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(g_screen[0], 0, sizeof(g_screen[0]));
    GXS_LoadBG0Scr(g_screen[1], 0, sizeof(g_screen[1]));
}

/*---------------------------------------------------------------------------*
  Name:         BgPutString

  Description:  BG の指定グリッドから横方向に ASCII 文字列を表示.

  Arguments:    x                表示する左上 x グリッド. (8pixel単位)
                y                表示する左上 y グリッド. (8pixel単位)
                pal              パレット番号. (0-15)
                str              表示する ASCII 文字列.
                len              表示する文字列長.
                                 str がこれよりも短い位置で NUL を含む場合
                                 その位置までの長さが採用される.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgPutString(int x, int y, int pal, const char *str, int len)
{
    u16    *const dst = draw_target;
    x += y * 32;
    str -= x, len += x;
    for (; str[x] && (x < len); ++x)
    {
        dst[x & ((32 * 32) - 1)] = (u16)((pal << 12) | (u8)str[x]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         BgPrintf

  Description:  BG の指定グリッドから横方向に書式付き ASCII 文字列を表示.

  Arguments:    x                表示する左上 x グリッド. (8pixel単位)
                y                表示する左上 y グリッド. (8pixel単位)
                pal              パレット番号. (0-15)
                str              表示する ASCII 文字列.
                                 対応書式は OS_VSNPrintf() に準じる.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgPrintf(int x, int y, int pal, const char *str, ...)
{
    char    tmp[32 + 1];
    va_list vlist;
    va_start(vlist, str);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), str, vlist);
    va_end(vlist);
    BgPutString(x, y, pal, tmp, sizeof(tmp));
}

/*---------------------------------------------------------------------------*
  Name:         BgSetMessage

  Description:  メイン/サブ画面両方の (4, 22) の位置に文字列表示.

  Arguments:    pal              パレット番号. (0-15)
                str              表示する ASCII 文字列.
                                 対応書式は OS_VSNPrintf() に準じる.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void BgSetMessage(int pal, const char *str, ...)
{
    char    tmp[32 + 1];
    va_list vlist;
    va_start(vlist, str);
    (void)OS_VSNPrintf(tmp, sizeof(tmp), str, vlist);
    va_end(vlist);

    /* メイン画面 (4, 22) に文字列表示 */
    BgPutString(4, 22, pal, "                            ", 28);
    BgPutString(4, 22, pal, tmp, 28);
    /* サブ画面の最新行に文字列を追加表示 */
    {
        /* 必要に応じてスクロール */
        const int BG_LINES = 24;
        if (cur_line == BG_LINES)
        {
            u16    *const sub_screen = BgiGetTargetScreen(FALSE);
            MI_CpuCopy16(&sub_screen[32 * 1], sub_screen, sizeof(u16) * 32 * (BG_LINES - 1));
            MI_CpuClear16(&sub_screen[32 * (BG_LINES - 1)], sizeof(u16) * 32 * 1);
            --cur_line;
        }
        /* 一時的に出力先を切り替える */
        BgiSetTargetScreen(FALSE);
        BgPutString(0, cur_line, pal, tmp, sizeof(tmp));
        BgiSetTargetScreen(TRUE);
        if (cur_line < BG_LINES)
            ++cur_line;
    }
}
