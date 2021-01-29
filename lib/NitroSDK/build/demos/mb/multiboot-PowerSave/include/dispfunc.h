/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - demos - multiboot-PowerSave
  File:     dispfunc.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dispfunc.h,v $
  Revision 1.2  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.1  2005/04/28 07:15:17  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#if !defined(NITROSDK_DEMOS_MB_MULTIBOOT_POWERSAVE_DISPFUNC_H_)
#define NITROSDK_DEMOS_MB_MULTIBOOT_POWERSAVE_DISPFUNC_H_


#include <nitro.h>


/*****************************************************************************/
/* constant */

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


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         BgInit

  Description:  BG を文字列表示用に初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgInit(void);

/*---------------------------------------------------------------------------*
  Name:         BgClear

  Description:  BG メイン画面スクリーンを全てキャラクタ 0 で初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgClear(void);

/*---------------------------------------------------------------------------*
  Name:         BgUpdate

  Description:  BG スクリーンを実メモリに反映.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgUpdate(void);

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
void    BgPutString(int x, int y, int pal, const char *str, int len);

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
void    BgPrintf(int x, int y, int pal, const char *str, ...);

/*---------------------------------------------------------------------------*
  Name:         BgPutChar

  Description:  BG の指定グリッドに文字を表示.

  Arguments:    x                表示する左上 x グリッド. (8pixel単位)
                y                表示する左上 y グリッド. (8pixel単位)
                pal              パレット番号. (0-15)
                c                表示する BG キャラクタ番号.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static  inline void BgPutChar(int x, int y, int pal, int c)
{
    const char tmp = (char)c;
    BgPutString(x, y, pal, &tmp, 1);
}

/*---------------------------------------------------------------------------*
  Name:         BgSetMessage

  Description:  メイン/サブ画面両方の (4, 22) の位置に文字列表示.

  Arguments:    pal              パレット番号. (0-15)
                str              表示する ASCII 文字列.
                                 対応書式は OS_VSNPrintf() に準じる.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    BgSetMessage(int pal, const char *str, ...);


#endif /* NITROSDK_DEMOS_MB_MULTIBOOT_POWERSAVE_DISPFUNC_H_ */
