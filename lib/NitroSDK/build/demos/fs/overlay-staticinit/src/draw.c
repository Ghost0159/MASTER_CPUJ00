/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     draw.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: draw.c,v $
  Revision 1.4  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.3  2005/07/22 07:25:38  yosizaki
  fix around OSThredQueue.

  Revision 1.2  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:45  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include "draw.h"



/********************************************************************/
/* constant */

#define	DEMO_DRAW_INIT		(1 << 0)
#define	DEMO_DRAW_ENABLE	(1 << 1)
#define	DEMO_DRAW_FLIPPING	(1 << 2)


/********************************************************************/
/* variable */

Point   cur_pt;                        /* current position */
RGB555  col_text;                      /* current text color */
RGB555  col_gnd;                       /* current ground color */
RGB555  col_clear;


u16     tmp_surface[GX_LCD_SIZE_X * GX_LCD_SIZE_Y] ATTRIBUTE_ALIGN(32);

typedef struct DrawiCommon
{
    Rect    clip;
    RGB555 *next_surface;              /* VRAM A/B/C/D */
    u32     draw_flag;

    OSThreadQueue flip_wait_q[1];      /* flip wait */
#if defined(OS_SIZEOF_OSTHREADQUEUE) && (OS_SIZEOF_OSTHREADQUEUE == 16)
    u8      padding[2];
#endif
}
DrawiCommon;

static DrawiCommon drawi_common;


/********************************************************************/
/* function */


static inline RGB555 *GetSurface(int x, int y)
{
    return drawi_common.next_surface + x + y * GX_LCD_SIZE_X;
}

static inline BOOL IsBoundX(DrawiCommon * p, int x)
{
    return (x >= p->clip.o.x) && (x < p->clip.t.x);
}
static inline BOOL IsBoundY(DrawiCommon * p, int y)
{
    return (y >= p->clip.o.y) && (y < p->clip.t.y);
}

static BOOL NormalizeRegion(Region * p)
{
    DrawiCommon *const pc = &drawi_common;
    if (p->pos.x >= pc->clip.t.x)
        return FALSE;
    else
    {
        if (p->pos.x - pc->clip.o.x < 0)
            p->wid.x += p->pos.x - pc->clip.o.x, p->pos.x = pc->clip.o.x;
        if (p->wid.x <= 0)
            return FALSE;
        else if (p->wid.x > pc->clip.t.x - p->pos.x)
            p->wid.x = pc->clip.t.x - p->pos.x;
    }
    if (p->pos.y >= pc->clip.t.y)
        return FALSE;
    else
    {
        if (p->pos.y - pc->clip.o.y < 0)
            p->wid.y += p->pos.y - pc->clip.o.y, p->pos.y = pc->clip.o.y;
        if (p->wid.y <= 0)
            return FALSE;
        else if (p->wid.y > pc->clip.t.y - p->pos.y)
            p->wid.y = pc->clip.t.y - p->pos.y;
    }
    return TRUE;
}

static void OnVBlank(void)
{
    DrawiCommon *const p = &drawi_common;
    if ((p->draw_flag & DEMO_DRAW_FLIPPING) != 0)
    {
        RGB555 *const dst_vram = (RGB555 *) HW_LCDC_VRAM_C;
        /* さっきまで書いていたサーフェスを転送 */
        MI_CpuCopyFast(tmp_surface, dst_vram, sizeof(tmp_surface));
        /* 初期化色が指定されていればクリア */
        if (col_clear != RGB555_CLEAR)
        {
            MI_CpuFillFast(tmp_surface,
                           (u32)((col_clear << 0) | (col_clear << 16)), sizeof(tmp_surface));
        }
        p->draw_flag &= ~DEMO_DRAW_FLIPPING;
        p->draw_flag |= DEMO_DRAW_ENABLE;
        OS_WakeupThread(p->flip_wait_q);
    }
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}


void InitDraw(void)
{
    DrawiCommon *const p = &drawi_common;
    BOOL    bak_irq = OS_DisableIrq();
    if (!p->draw_flag)
    {
        p->draw_flag |= DEMO_DRAW_INIT;
        GX_Init();
        GX_SetPower(GX_POWER_ALL);
        OS_SetIrqFunction(OS_IE_V_BLANK, OnVBlank);
        (void)OS_EnableIrqMask(OS_IE_V_BLANK);

#if	!defined(SDK_NO_THREAD)
        OS_InitThreadQueue(p->flip_wait_q);
#endif

        /* 共通の描画変数設定 */
        col_clear = RGB555_CLEAR;
        cur_pt.x = 0, cur_pt.y = 0;
        col_text = RGB555_WHITE;
        col_gnd = RGB555_CLEAR;
        p->draw_flag |= DEMO_DRAW_ENABLE;
        p->clip.o.x = 0;
        p->clip.o.y = 0;
        p->clip.t.x = GX_LCD_SIZE_X;
        p->clip.t.y = GX_LCD_SIZE_Y;

        /* 半角でダンプ表示などするために VRAM-C の直接表示 */
        GX_SetGraphicsMode(GX_DISPMODE_VRAM_C, (GXBGMode)0, (GXBG0As)0);
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);

        p->next_surface = (RGB555 *) tmp_surface;
        MI_CpuFillFast(tmp_surface,
                       (u32)((col_clear << 0) | (col_clear << 16)), sizeof(tmp_surface));

        (void)GX_VBlankIntr(TRUE);
        GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    }
    (void)OS_RestoreIrq(bak_irq);
}

void DrawBegin(void)
{
    DrawiCommon *const p = &drawi_common;
    OSIntrMode bak_psr = OS_DisableInterrupts();
    while (!(p->draw_flag & DEMO_DRAW_ENABLE))
        OS_SleepThread(p->flip_wait_q);
    (void)OS_RestoreInterrupts(bak_psr);
}

void DrawEnd(void)
{
    DrawiCommon *const p = &drawi_common;
    OSIntrMode bak_psr;

    DC_FlushRange(tmp_surface, sizeof(tmp_surface));
    DC_WaitWriteBufferEmpty();

    bak_psr = OS_DisableInterrupts();
    p->draw_flag &= ~DEMO_DRAW_ENABLE;
    p->draw_flag |= DEMO_DRAW_FLIPPING;
    (void)OS_RestoreInterrupts(bak_psr);
}

void ClipWindow(int ox, int oy, int tx, int ty, Rect * p_bak)
{
    DrawiCommon *const p = &drawi_common;
    if (p_bak)
    {
        p_bak->o.x = p->clip.o.x;
        p_bak->o.y = p->clip.o.y;
        p_bak->t.x = p->clip.t.x;
        p_bak->t.y = p->clip.t.y;
    }
    if (ox < 0)
        ox = 0;
    if (tx > GX_LCD_SIZE_X)
        tx = GX_LCD_SIZE_X;
    if (oy < 0)
        oy = 0;
    if (ty > GX_LCD_SIZE_Y)
        ty = GX_LCD_SIZE_Y;
    p->clip.o.x = ox;
    p->clip.o.y = oy;
    p->clip.t.x = tx;
    p->clip.t.y = ty;
}

void RestoreClipWindow(const Rect * p_bak)
{
    DrawiCommon *const p = &drawi_common;
    if (p_bak)
    {
        p->clip.o.x = p_bak->o.x;
        p->clip.o.y = p_bak->o.y;
        p->clip.t.x = p_bak->t.x;
        p->clip.t.y = p_bak->t.y;
    }
}

void ClearFrame(RGB555 col)
{
    DrawiCommon *const p = &drawi_common;
    MI_CpuFillFast(p->next_surface,
                   (u32)((col << 0) | (col << 16)), GX_LCD_SIZE_X * GX_LCD_SIZE_Y * sizeof(RGB555));
}

void FillRect(int x, int y, int wx, int wy, RGB555 col)
{
    Region  r;
    r.pos.x = x, r.pos.y = y;
    r.wid.x = wx, r.wid.y = wy;
    if (NormalizeRegion(&r))
    {
        RGB555 *p = GetSurface(r.pos.x, r.pos.y);
        while (--r.wid.y >= 0)
        {
            MI_CpuFill16(p, col, r.wid.x * sizeof(RGB555));
            p += GX_LCD_SIZE_X;
        }
    }
}

void BlitRect(int x, int y, int wx, int wy, RGB555 * src, int stroke)
{
    Region  r;
    r.pos.x = x, r.pos.y = y;
    r.wid.x = wx, r.wid.y = wy;
    if (NormalizeRegion(&r))
    {
        RGB555 *p = GetSurface(r.pos.x, r.pos.y);
        while (--r.wid.y >= 0)
        {
            MI_CpuCopy16(src, p, r.wid.x * sizeof(RGB555));
            src += stroke;
            p += GX_LCD_SIZE_X;
        }
    }
}

void TransRect(int x, int y, int wx, int wy, RGB555 * src, int stroke)
{
    Region  r;
    r.pos.x = x, r.pos.y = y;
    r.wid.x = wx, r.wid.y = wy;
    if (NormalizeRegion(&r))
    {
        RGB555 *p = GetSurface(r.pos.x, r.pos.y);
        RGB555  trans = col_gnd;
        while (--r.wid.y >= 0)
        {
            int     i;
            for (i = 0; i < r.wid.x; ++i)
            {
                if (p[i] != trans)
                    p[i] = src[i];
            }
            src += stroke;
            p += GX_LCD_SIZE_X;
        }
    }
}

void DrawLine(int sx, int sy, int tx, int ty, RGB555 col)
{
    DrawiCommon *const pc = &drawi_common;

    RGB555 *p;
    int     tmp;
    int     wx, wy;
    if (sx > tx)
    {
        tmp = sx + 1, sx = tx + 1, tx = tmp;
        tmp = sy, sy = ty, ty = tmp;
    }
    wx = tx - sx, wy = ty - sy;

    if (!wx)
    {
        if (wy < 0)
        {
            wy = -wy;
            tmp = sy + 1, sy = ty + 1, ty = tmp;
        }
        if (!IsBoundX(pc, sx) || (sy >= pc->clip.t.y))
            return;
        else
        {
            if (sy < pc->clip.o.y)
                wy += sy, sy = pc->clip.o.y;
            if (wy > pc->clip.t.y - sy)
                wy = pc->clip.t.y - sy;
            p = GetSurface(sx, sy);
            while (--wy >= 0)
            {
                *p = col;
                p += GX_LCD_SIZE_X;
            }
        }
    }
    else if (!wy)
    {
        if (!IsBoundY(pc, sy) || (sx >= pc->clip.t.x))
            return;
        else
        {
            if (sx < pc->clip.o.x)
                wx += sx, sx = pc->clip.o.x;
            if (wx > pc->clip.t.x - sx)
                wx = pc->clip.t.x - sx;
            p = GetSurface(sx, sy);
            MI_CpuFill16(p, col, wx * sizeof(RGB555));
        }
    }
    else
    {
        int     n, dx, dy;
        int     y_delta = +1;
        int     y_ofs = GX_LCD_SIZE_X;
        if (wy < 0)
        {
            wy = -wy;
            y_delta = -y_delta;
            y_ofs = -y_ofs;
        }
        p = GetSurface(sx, sy);
        dx = wy - 1, dy = wx - 1;
        --sx, sy -= y_delta;
        for (n = wx * wy; --n >= 0;)
        {
            BOOL    moved = FALSE;
            if (++dx >= wy)
                moved = TRUE, dx = 0, ++sx, p += 1;
            if (++dy >= wx)
                moved = TRUE, dy = 0, sy += y_delta, p += y_ofs;
            if (moved && IsBoundX(pc, sx) && IsBoundY(pc, sy))
                *p = col;
        }
    }
}

void DrawCircle(int ox, int oy, int r)
{
    DrawiCommon *const pc = &drawi_common;

    int     n, dx, dy, vx, vy;
    RGB555  col = col_text;
    vx = 65536 * r, vy = 0;
    dx = ox * 65536, dy = (oy - r) * 65536;

    for (n = (int)(2 * 3.14 * 256); n > 0; --n)
    {
        int     ax, ay, x, y;
        ax = -vy / 256, ay = +vx / 256;
        vx += ax, vy += ay;
        dx += ay, dy -= ax;
        x = dx / 65536, y = dy / 65536;
        if (IsBoundX(pc, x) && IsBoundY(pc, y))
            *GetSurface(x, y) = col;
    }
}

void DrawText(int x, int y, const char *s, ...)
{
    va_list vlist;
    static char temp[512];

    va_start(vlist, s);
    (void)OS_VSNPrintf(temp, sizeof(temp) - 2, s, vlist);
    va_end(vlist);
    DrawTextLen(x, y, temp, sizeof(temp) - 1);
}

void DrawTextLen(int x, int y, const char *s, int n)
{
    DrawiCommon *const pc = &drawi_common;

    RGB555 *p = GetSurface(x, y);
    RGB555  txt = col_text, gnd = col_gnd;
    int     px = 0;
    int     rep = 0;

    for (; n > 0; ++s, --n)
    {

        int     c = MI_ReadByte(s);

        switch (c)
        {

        case '\0':
            return;

        case '\r':
            c = MI_ReadByte(s + 1);
            if (c == '\n')
                ++s, --n;
        case '\n':
            y += 8;
            p += GX_LCD_SIZE_X * 8;
            px = 0;
            break;

        case '\t':
            {
                const int align = 4;
                rep = align - ((px / 8) & (align - 1));
                c = ' ';
            }
            goto put_char;

        default:
            rep = 1;
            goto put_char;
          put_char:
            while (--rep >= 0)
            {
                int     tx = x + px;
                if ((tx > pc->clip.o.x - 8) && (y > pc->clip.o.y - 8) &&
                    (tx < pc->clip.t.x) && (y < pc->clip.t.y))
                {
                    const u32 *pf = sampleCharData + c * 8;
                    RGB555 *dst = p + px;
                    int     base = 0;
                    int     wx = 8, wy = 8;
                    if (pc->clip.o.x - tx > 0)
                        base = pc->clip.o.x - tx;
                    else if (wx > pc->clip.t.x - tx)
                        wx = pc->clip.t.x - tx;
                    if (pc->clip.o.y - y > 0)
                    {
                        wy -= pc->clip.o.y - y;
                        pf += pc->clip.o.y - y;
                        dst += GX_LCD_SIZE_X * (pc->clip.o.y - y);
                    }
                    else if (wy > pc->clip.t.y - y)
                        wy = pc->clip.t.y - y;
                    for (; --wy >= 0; ++pf, dst += GX_LCD_SIZE_X)
                    {
                        int     i;
                        const u32 d = *pf;
                        for (i = base; i < wx; ++i)
                        {
                            if ((d & (0xF << (i * 4))) != 0)
                                dst[i] = txt;
                            else if (gnd != RGB555_CLEAR)
                                dst[i] = gnd;
                        }
                    }
                }
                px += 8;
            }
            break;
        }
    }
}
