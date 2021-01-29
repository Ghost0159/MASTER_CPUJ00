/*
  Project:  NitroSDK - WM - demos - wep-1
  File:     print.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: print.h,v $
  Revision 1.2  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model Ç… WEP Key ÇÃìÆìIê›íËÇâ¡Ç¶ÅAwep-1 ÉfÉÇÇ∆ÇµÇƒêVãKìoò^

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

#endif
