/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-wfs
  File:     util.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util.c,v $
  Revision 1.3  2005/11/21 10:48:24  kitase_hirotake
  OS_InitPrintServer の削除

  Revision 1.2  2005/06/27 11:10:11  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.1  2005/06/23 09:05:57  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

#include    "util.h"


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/

static u16 gScreen[32 * 32];           /* 仮想スクリーン */
static u16 repeat_count[12];           /* キーリピート */
static char recent_output[24][32 + 1];


/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  メインメモリ上のアリーナにてメモリ割当てシステムを初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}


/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Ｖブランク割込みベクトル。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    /* 仮想スクリーンをVRAMに反映 */
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    /* IRQ チェックフラグをセット */
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


/*---------------------------------------------------------------------------*
  Name:         UTIL_Init

  Description:  サンプルのフレームワーク初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void UTIL_Init(void)
{
    /* 各種初期化 */
    OS_Init();
    OS_InitTick();

    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    /* 表示設定初期化 */
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    /* 文字列表示用に２Ｄ表示設定 */
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256,
                     GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));

    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    /* 割込み設定 */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    /* メモリ割当 */
    InitializeAllocateSystem();
}


/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  キーデータの取得.

  Arguments:    pKey : キー情報を格納するアドレス

  Returns:      None.
 *---------------------------------------------------------------------------*/
void KeyRead(KeyInfo * pKey)
{
    enum
    {
        KEY_BIT_MAX = 12,
        KEY_REPEAT_START = 25,         /* キーリピート開始までのフレーム数 */
        KEY_REPEAT_SPAN = 10           /* キーリピートの間隔フレーム数 */
    };

    int     i;
    const u16 r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < KEY_BIT_MAX; ++i)
    {
        const u16 bit = (u16)(1 << i);
        if ((r & bit) != 0)
        {
            if (!(pKey->cnt & bit))
            {
                pKey->trg |= bit;
                repeat_count[i] = 1;
            }
            else if (repeat_count[i] > KEY_REPEAT_START)
            {
                pKey->rep |= bit;
                repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
            }
            else
            {
                ++repeat_count[i];
            }
        }
        else if ((pKey->cnt & bit) != 0)
        {
            pKey->up |= bit;
        }
    }
    pKey->cnt = r;
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  表示用仮想スクリーンの初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearString(void)
{
    MI_CpuClearFast(gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  仮想スクリーンに32文字以内の文字列を表示.

  Arguments:    x       - 文字列の先頭を配置する x 座標 / 8 dot
                y       - 文字列の先頭を配置する y 座標 / 8 dot
                palette - 文字色を指定するパレット番号
                text    - 表示文字列
                ...     - 後続の可変引数

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintString(int x, int y, int palette, const char *text, ...)
{
    va_list vlist;
    static char temp[64 + 2];
    u8      d;
    int     i;

    va_start(vlist, text);
    (void)OS_VSNPrintf(temp, sizeof(temp) - 1, text, vlist);
    va_end(vlist);
    *(u16 *)&temp[32] = 0x0000;
    for (i = 0; (d = MI_ReadByte(temp + i)) != '\0'; ++i)
    {
        if ((d == '\r') || (d == '\n'))
            break;
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | d);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  仮想スクリーンに表示されている文字列の色を変更.

  Arguments:    x       - 色変更を開始する x 座標 / 8 dot
                y       - 色変更を開始する y 座標 / 8 dot
                length  - 色変更する文字数
                palette - 文字色を指定するパレット番号

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ColorString(int x, int y, int length, int palette)
{
    int     i;
    for (i = 0; i < length; ++i)
    {
        const int index = ((y * 32) + x + i) % (32 * 32);
        gScreen[index] = (u16)((gScreen[index] & 0x0FFF) | (palette << 12));
    }
}

/*---------------------------------------------------------------------------*
  Name:         ClearLine

  Description:  文字列描画を初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearLine(void)
{
    MI_CpuClear8(recent_output, sizeof(recent_output));
}

/*---------------------------------------------------------------------------*
  Name:         PrintLine

  Description:  指定ラインに1 フレーム以上残る不揮発な文字列を描画.

  Arguments:    x          表示位置 x グリッド. (8 pixel 単位)
                y          表示位置 y グリッド. (8 pixel 単位)
                text       後続の可変引数を受ける書式文字列.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PrintLine(int x, int y, const char *text, ...)
{
    va_list va;
    va_start(va, text);
    MI_CpuFill8(recent_output[y], ' ', sizeof(recent_output[y]));
    (void)OS_VSNPrintf(&recent_output[y][x], (size_t) (32 - x), text, va);
    va_end(va);
}

/*---------------------------------------------------------------------------*
  Name:         FlushLine

  Description:  通常の文字列描画に PrintLine() の不揮発文字列を上書き.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void FlushLine(void)
{
    int     y;
    for (y = 0; y < 24; ++y)
    {
        PrintString(0, y, COLOR_WHITE, recent_output[y]);
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
