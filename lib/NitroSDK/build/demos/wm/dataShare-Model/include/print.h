/*
  Project:  NitroSDK - WM - demos - wmsample
  File:     print.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: print.h,v $
  Revision 1.6  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.5  2005/11/21 00:41:10  adachi_hiroaki
  親機選択画面に参加人数とニックネームを表示するようにした

  Revision 1.4  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.3  2004/11/02 07:12:55  sasakis
  コード整理。

  Revision 1.2  2004/10/22 07:35:18  sasakis
  共有化した wh に対応。

  Revision 1.1  2004/09/22 09:39:33  sasakis
  デバグ画面を追加。

  $NoKeywords$
*/

#ifndef __PRINT_H__
#define __PRINT_H__

#define PR_SCREEN_HEIGHT (23)
#define PR_SCREEN_WIDTH  (32)
#define PR_SCREEN_SIZE   (PR_SCREEN_WIDTH * PR_SCREEN_HEIGHT)

typedef struct PRScreen_
{
    u16     screen[PR_SCREEN_SIZE];
    int     curx;
    int     cury;
    int     scroll;
}
PRScreen;

extern void PR_ClearScreen(PRScreen * scr);
extern void PR_Locate(PRScreen * scr, int x, int y);
extern void PR_PutString(PRScreen * scr, const char *text);
extern void PR_VPrintString(PRScreen * scr, const char *fmt, va_list vlist);
extern void PR_PrintString(PRScreen * scr, const char *fmt, ...);
extern int PR_GetStringLengthA(const char *str);

#endif
