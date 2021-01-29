/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     draw.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: draw.h,v $
  Revision 1.3  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:06  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:53  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if	!defined(NITRO_DEMOS_FS_OVERLAY_STATICINIT_DRAW_H_)
#define NITRO_DEMOS_FS_OVERLAY_STATICINIT_DRAW_H_


#include <nitro.h>



/* ビットマップモードで直接描画するための関数群です */



#if	defined(__cplusplus)
extern  "C"
{
#endif


/********************************************************************/
/* structure */

    typedef u16 RGB555;

    typedef struct Point
    {
        int     x;
        int     y;
    }
    Point;

    typedef struct Region
    {
        Point   pos;
        Point   wid;
    }
    Region;

    typedef struct Rect
    {
        Point   o;
        Point   t;
    }
    Rect;


/********************************************************************/
/* constant */

#define	RGB555_(r, g, b)	(RGB555)(((r) << 0) | ((g) << 5) | ((b) << 10))

#define	RGB555_CLEAR	(RGB555)(1 << 15)
#define	RGB555_BLACK	RGB555_(0x00, 0x00, 0x00)
#define	RGB555_WHITE	RGB555_(0x1F, 0x1F, 0x1F)
#define	RGB555_RED		RGB555_(0x1F, 0x00, 0x00)
#define	RGB555_GREEN	RGB555_(0x00, 0xFF, 0x00)
#define	RGB555_BLUE		RGB555_(0x00, 0x00, 0x1F)
#define	RGB555_PURPLE	RGB555_(0x1F, 0x00, 0x1F)
#define	RGB555_YELLOW	RGB555_(0x1F, 0x1F, 0x00)
#define	RGB555_GREY		RGB555_(0x10, 0x10, 0x10)
#define	RGB555_MILK		RGB555_(0x18, 0x18, 0x18)

    extern const u32 sampleCharData[8 * 0x100];


/********************************************************************/
/* variable */

/* current position (MoveTo, LineTo, ... use) */
    extern Point cur_pt;
/* current text color (DrawText uses) */
    extern RGB555 col_text;
/* current ground color (DrawText, TransRect, ... use) */
    extern RGB555 col_gnd;
/* current clear-frame color (if RGB555_CLEAR, keep prev-frame) */
    extern RGB555 col_clear;


/********************************************************************/
/* function */

    void    InitDraw(void);
    void    DrawBegin(void);
    void    DrawEnd(void);

    void    ClipWindow(int ox, int oy, int tx, int ty, Rect * p_bak);
    void    RestoreClipWindow(const Rect * p_bak);

    static inline void SetTextColor(RGB555 col)
    {
        col_text = col;
    }

    static inline void SetGroundColor(RGB555 col)
    {
        col_gnd = col;
    }

    static inline void SetClearColor(RGB555 col)
    {
        col_clear = col;
    }

    static inline RGB555 GetTextColor(void)
    {
        return col_text;
    }

    static inline RGB555 GetGroundColor(void)
    {
        return col_gnd;
    }

    static inline RGB555 GetClearColor(void)
    {
        return col_clear;
    }

    void    ClearFrame(RGB555 col);
    void    FillRect(int x, int y, int wx, int wy, RGB555 col);
    void    BlitRect(int x, int y, int wx, int wy, RGB555 * src, int stroke);
    void    TransRect(int x, int y, int wx, int wy, RGB555 * src, int stroke);
    void    DrawLine(int sx, int sy, int tx, int ty, RGB555 col);
    void    DrawCircle(int ox, int oy, int r);

    static inline void MoveTo(int x, int y)
    {
        cur_pt.x = x, cur_pt.y = y;
    }

    static inline void LineTo(int x, int y)
    {
        DrawLine(cur_pt.x, cur_pt.y, x, y, col_text);
        MoveTo(x, y);
    }

    void    DrawTextLen(int x, int y, const char *s, int n);

    void    DrawText(int x, int y, const char *s, ...);

    static inline void DrawChar(int x, int y, char c)
    {
        DrawTextLen(x, y, &c, 1);
    }


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


#endif /* NITRO_DEMOS_FS_OVERLAY_STATICINIT_DRAW_H_ */
