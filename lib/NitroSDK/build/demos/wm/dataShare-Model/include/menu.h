/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     menu.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: menu.h,v $
  Revision 1.5  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 00:41:10  adachi_hiroaki
  親機選択画面に参加人数とニックネームを表示するようにした

  Revision 1.3  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.2  2004/11/05 09:16:48  sasakis
  ウィンドウの挙動を改良し、コードを整理した。

  Revision 1.1  2004/11/05 04:27:40  sasakis
  ロビー画面、親機選択画面の追加と、それに必要な改造（scan関連など）。

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef __MENU_H__
#define __MENU_H__

#define ITEM_LENGTH_MAX (30)
#define ITEM_NUM_MAX    (20)

enum
{
    WIN_STATE_OPENING,
    WIN_STATE_OPENED,
    WIN_STATE_CLOSING,
    WIN_STATE_CLOSED,
    WIN_STATE_NUM
};

enum
{
    WIN_FLAG_NONE = 0,
    WIN_FLAG_SELECTABLE = (1 << 0),
    WIN_FLAG_NOCONTROL = (1 << 1),
    WIN_FLAG_LAST = (1 << 8)
};

typedef struct Window_
{
    int     x, y;
    int     width, height;
    int     count;
    int     itemnum;
    int     lineheight;
    int     leftmargin;
    int     rightmargin;
    int     selected;
    int     state;
    int     flag;
    char    item[ITEM_NUM_MAX][ITEM_LENGTH_MAX];
    struct Window_ *next;
}
Window;

extern void initWindow(Window * win);
extern void setupWindow(Window * win,
                        int x, int y, int flag, int lineheight, int leftmargin, int rightmargin);
extern void addItemToWindow(Window * win, const char *item);
extern void setItemToWindow(Window * win, const char *item, int index);
extern void drawWindow(Window * win);
extern int updateWindow(Window * win);
extern void openWindow(Window * win);
extern void closeWindow(Window * win);
extern int getWindowSelected(Window * win);
extern void drawAllWindow(void);
extern void updateAllWindow(void);
extern void closeAllWindow(void);

#endif
