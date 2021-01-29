/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wep-1
  File:     menu.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: menu.c,v $
  Revision 1.2  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model Ç… WEP Key ÇÃìÆìIê›íËÇâ¡Ç¶ÅAwep-1 ÉfÉÇÇ∆ÇµÇƒêVãKìoò^

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <string.h>
#include "main.h"
#include "menu.h"
#include "graphics.h"
#include "print.h"
#include "key.h"

static Window *sWindowRoot = NULL;

void initWindow(Window * win)
{
    Window *w;

    if (sWindowRoot == NULL)
    {
        (void)memset(win, 0, sizeof(Window));
        sWindowRoot = win;
        return;
    }

    w = sWindowRoot;

    for (; (w != NULL) && (w->next != NULL); w = w->next)
    {
        if (w == win)
        {
            Window *n;
            n = win->next;
            (void)memset(win, 0, sizeof(Window));
            win->next = n;
            return;
        }
    }

    (void)memset(win, 0, sizeof(Window));
    w->next = win;
}

void setupWindow(Window * win,
                 int x, int y, int flag, int lineheight, int leftmargin, int rightmargin)
{
    win->x = x;
    win->y = y;
    win->lineheight = lineheight;
    win->leftmargin = leftmargin;
    win->rightmargin = rightmargin;
    win->selected = 0;
    win->count = 0;
    win->state = WIN_STATE_CLOSED;
    win->flag = flag;
}

void addItemToWindow(Window * win, const char *item)
{
    int     l, w;

    l = (int)strlen(item);
    if (win->flag & WIN_FLAG_SELECTABLE)
    {
        ++l;
    }

    w = l * 8 + win->leftmargin + win->rightmargin;
    if (win->width < w)
    {
        win->width = w;
    }

    (void)strncpy(win->item[win->itemnum], item, ITEM_LENGTH_MAX);
    win->item[win->itemnum][ITEM_LENGTH_MAX - 1] = 0;
    win->height = win->itemnum * win->lineheight + 8;
    ++win->itemnum;
}

void openWindow(Window * win)
{
    if (win->state != WIN_STATE_CLOSED)
    {
        return;
    }

    win->count = 0;
    win->state = WIN_STATE_OPENING;
}

void closeWindow(Window * win)
{
    if (win->state != WIN_STATE_OPENED)
    {
        return;
    }

    win->count = win->height;
    win->state = WIN_STATE_CLOSING;
}

void drawWindow(Window * win)
{
    int     y, h;
    if (win->state == WIN_STATE_CLOSED)
    {
        return;
    }

    if (win->itemnum == 0)
    {
        return;
    }

    y = win->y + (win->height / 2) - (win->count / 2);
    h = win->count;

    setupPseudo2DCamera();

    G3X_AlphaTest(FALSE, 0);
    G3_PolygonAttr(GX_LIGHTMASK_NONE,
                   GX_POLYGONMODE_DECAL, GX_CULL_NONE, 0, 31, GX_POLYGON_ATTR_MISC_DISP_1DOT);
    drawPseudo2DColorQuad(win->x - 7, y - 4, win->width + 4, h + 11, GX_RGB(0, 0, 20));
    G3X_AlphaTest(TRUE, 0);
    G3_PolygonAttr(GX_LIGHTMASK_NONE,
                   GX_POLYGONMODE_DECAL, GX_CULL_NONE, 0, 0, GX_POLYGON_ATTR_MISC_DISP_1DOT);
    G3_TexImageParam(GX_TEXFMT_NONE,
                     GX_TEXGEN_NONE,
                     GX_TEXSIZE_S16,
                     GX_TEXSIZE_T16, GX_TEXREPEAT_NONE, GX_TEXFLIP_NONE, GX_TEXPLTTCOLOR0_USE, 0);
    drawPseudo2DColorQuad(win->x - 8, y - 5, win->width + 6, h + 13, GX_RGB(20, 20, 31));

    if (win->state == WIN_STATE_OPENED)
    {
        int     i;
        for (i = 0; i < win->itemnum; ++i)
        {
            PR_Locate(getInfoScreen(),
                      (win->x + win->leftmargin) / 8, (win->y + win->lineheight * i) / 8);
            if (win->flag & WIN_FLAG_SELECTABLE)
            {
                PR_PrintString(getInfoScreen(), "%c", (win->selected == i) ? '>' : ' ');
                PR_PutString(getInfoScreen(), win->item[i]);

            }
            else
            {
                PR_PutString(getInfoScreen(), win->item[i]);
            }
        }
    }
}

int updateWindow(Window * win)
{
    switch (win->state)
    {
    case WIN_STATE_OPENING:
        if (win->count >= win->height)
        {
            win->state = WIN_STATE_OPENED;
        }
        else
        {
            win->count += 6;
        }
        break;

    case WIN_STATE_OPENED:
        win->count = win->height;

        if (!(win->flag & WIN_FLAG_NOCONTROL))
        {
            if (!(win->flag & WIN_FLAG_SELECTABLE))
            {
                if (getKeyInfo()->trg & (PAD_BUTTON_A | PAD_BUTTON_B))
                {
                    closeWindow(win);
                }
            }
            else
            {
                KeyInfo *key;
                key = getKeyInfo();
                if (key->trg & PAD_BUTTON_A)
                {
                    closeWindow(win);
                }
                if (key->trg & PAD_BUTTON_B)
                {
                    win->selected = -1;
                    closeWindow(win);
                }
                if (key->trg & PAD_KEY_UP)
                {
                    --win->selected;
                    if (win->selected < 0)
                    {
                        win->selected += win->itemnum;
                    }
                }
                if (key->trg & PAD_KEY_DOWN)
                {
                    ++win->selected;
                    if (win->selected >= win->itemnum)
                    {
                        win->selected -= win->itemnum;
                    }
                }
            }
        }

        break;

    case WIN_STATE_CLOSING:
        if (win->count <= 0)
        {
            win->state = WIN_STATE_CLOSED;
        }
        else
        {
            win->count -= 6;
        }
        break;

    case WIN_STATE_CLOSED:
        win->count = 0;
        break;
    }

    if (win->count > win->height)
    {
        win->count = win->height;
    }
    if (win->count < 0)
    {
        win->count = 0;
    }

    return win->state;
}

void drawAllWindow(void)
{
    Window *w;
    for (w = sWindowRoot; w != NULL; w = w->next)
    {
        drawWindow(w);
    }
}

void updateAllWindow(void)
{
    Window *w;
    for (w = sWindowRoot; w != NULL; w = w->next)
    {
        (void)updateWindow(w);
    }
}

void closeAllWindow(void)
{
    Window *w;
    for (w = sWindowRoot; w != NULL; w = w->next)
    {
        closeWindow(w);
    }
}
