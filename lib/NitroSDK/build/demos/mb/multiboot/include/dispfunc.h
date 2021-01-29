/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot
  File:     dispfunc.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dispfunc.h,v $
  Revision 1.5  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.3  2004/10/05 06:49:11  sato_masaki
  small fix

  Revision 1.2  2004/10/05 06:47:54  sato_masaki
  設定メニューを追加。その他色々細かな修正。

  Revision 1.1  2004/08/31 11:00:12  sato_masaki
  initial check-in


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef _DISPFUNC_H_
#define _DISPFUNC_H_

#include <nitro.h>

/* テキスト用パレットカラー */
enum
{
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    PURPLE,
    LIGHT_GREEN,
    HI_YELLOW,
    HI_CYAN,
    HI_WHITE,
    HI_BLUE,
    HI_PURPLE,
    HI_LIGHT_GREEN,
    BROWN,
    HI_BROWN,
    COLOR_NUM
};

/* 各種描画用定数配列 */
extern const u32 sampleCharData[8 * 0xe0];
extern const u16 samplePlttData[16][16];

/*
	=================================================================================

	OBJ文字描画

	=================================================================================
*/

// --- global value
extern GXOamAttr oamBak[128];

//      OBJ セット
void    ObjSet(int objNo, int x, int y, int charNo, int paletteNo);

//      OBJ文字列のセット
void    ObjSetString(int startobjNo, int x, int y, char *string, int paletteNo);

//      OBJ クリア
void    ObjClear(int objNo);

//      OBJ クリア(範囲指定)
void    ObjClearRange(int start, int end);

//      メッセージセット
void    ObjSetMessage(int objNo, char *message);

/*
	=================================================================================

	BG文字描画 (BG0固定)

	=================================================================================
*/

// --- definition

// --- initialize
void    BgInitForPrintStr(void);

// --- for main LCD
void    BgPrintf(s16 x, s16 y, u8 palette, char *text, ...);
void    BgPutString(s16 x, s16 y, u8 palette, char *text);
void    BgPutStringN(s16 x, s16 y, u8 palette, char *text, u32 num);
void    BgPutChar(s16 x, s16 y, u8 palette, char c);
void    BgClear(void);

// --- for sub LCD
void    BgPrintfSub(s16 x, s16 y, u8 palette, char *text, ...);
void    BgPutStringSub(s16 x, s16 y, u8 palette, char *text);
void    BgPutStringNSub(s16 x, s16 y, u8 palette, char *text, u32 num);
void    BgPutCharSub(s16 x, s16 y, u8 palette, char c);
void    BgClearSub(void);

// --- v blank process
void    BgScrSetVblank(void);

// --- message print
void    BgSetMessage(u8 palette, char *message, ...);
void    BgSetMessageSub(u8 palette, char *message, ...);
void    BgSetMessageLogSub(u8 palette, char *text);

// --- get screen buffer
u16    *BgGetScreenBuf(void);
u16    *BgGetScreenBufSub(void);

#endif /*       _DISPFUNC_H_    */
