/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-Model
  File:     disp.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: disp.h,v $
  Revision 1.3  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.1  2004/09/16 14:07:11  takano_makoto
  Initial Update

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef MB_DEMO_DISP_H_
#define MB_DEMO_DISP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

/*
 * 文字列描画全般の処理.
 */

/* OBJ番号の割り振り */
enum
{
    TITLE_STRING_OBJNO = 0,
    PARENT_STRING_OBJNO = 10,
    CHILD_STRING_OBJNO = 22,
    PLIST_STRING_OBJNO = 33,
    CLIST_STRING_OBJNO = 39,

    CURSOR_OBJNO = 45,
    PARENT_NAME_OBJNO = 46,
    FILENAME_OBJNO = 82,
    MESSAGE_OBJNO = 104
};

/* テキスト用パレットカラー */
enum
{
    PLTT_BLACK = 0,
    PLTT_BLUE = 1,
    PLTT_RED = 2,
    PLTT_PURPLE = 3,
    PLTT_GREEN = 4,
    PLTT_CYAN = 5,
    PLTT_YELLOW = 6,
    PLTT_WHITE = 7
};



//============================================================================
//  関数宣言
//============================================================================

void    DispInit(void);
void    DispOn(void);
void    DispOff(void);
void    DispVBlankFunc(void);
void    ObjSet(s32 objNo, s32 x, s32 y, s32 charNo, s32 paletteNo);
void    ObjSetString(s32 startobjNo, s32 x, s32 y, const char *string, s32 paletteNo);
void    ObjClear(s32 objNo);
void    ObjClearRange(s32 start, s32 end);
void    ObjSetMessage(const char *message);
void    BgPutChar(s32 x, s32 y, s32 palette, s8 c);
void    BgPutStringN(s32 x, s32 y, s32 palette, const char *text, s32 num);
void    BgPutString(s32 x, s32 y, s32 palette, const char *text);
void    BgPrintStr(s32 x, s32 y, s32 palette, const char *text, ...);
void    BgSetMessage(s32 palette, const char *text, ...);
void    BgClear(void);
void    BgColorString(s16 x, s16 y, s16 length, u8 palette);



#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // MB_DEMO_DISP_H_
