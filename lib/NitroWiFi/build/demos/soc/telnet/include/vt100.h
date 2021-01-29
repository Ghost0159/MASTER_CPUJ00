/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     vt100.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vt100.h,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#ifndef TELNET_VT100_
#define TELNET_VT100_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  定数定義
 *---------------------------------------------------------------------------*/
enum VT100_STATE
{
    VT100_STATE_NORMAL          = 0,    /* 通常 */
    VT100_STATE_ESC             = 1,    /* ESC */
    VT100_STATE_CSI             = 2,    /* CSI (ESC[) */
    VT100_STATE_SCS94           = 3,    /* ESC{()*+} */
    VT100_STATE_SCS94_PERCENT   = 4,    /* ESC{()*+}% */
    VT100_STATE_SCS96           = 5,    /* ESC{-./} */
    VT100_STATE_SCS96_PERCENT   = 6,    /* ESC{-./}% */
    VT100_STATE_ESC_HASH        = 7,    /* ESC# */
    VT100_STATE_ESC_SPC         = 8,    /* ESC<SPC> */
    VT100_STATE_STRING          = 9,    /* 文字列待ち (ESC\) */
    VT100_STATE_STRING_ESC      = 10,   /* 文字列待ち, ESC受信の場合 */
    VT100_STATE_DCS             = 11,   /* DCS (ESC P) */
    VT100_STATE_DCS_DOLLAR      = 12    /* DCS<p>$ */
};

#define VT100_FLAG_LASTCHAR     0x001
#define VT100_FLAG_INSERTMODE   0x002
#define VT100_FLAG_APPLKEYPAD   0x004
#define VT100_FLAG_APPLCURSOR   0x008
#define VT100_FLAG_DECOM        0x010
#define VT100_FLAG_DECAWM       0x020
#define VT100_FLAG_CURSORON     0x040
#define VT100_FLAG_NATCHARSET   0x080
#define VT100_FLAG_SAVEDCURS    0x100

#define VTMODE_SET              33
#define VTMODE_RESET            44
#define VTMODE_REPORT           55

#define CONSOLE_COLOR_BLACK     0
#define CONSOLE_COLOR_RED       1
#define CONSOLE_COLOR_GREEN     2
#define CONSOLE_COLOR_BROWN     3
#define CONSOLE_COLOR_BLUE      4
#define CONSOLE_COLOR_MAGENTA   5
#define CONSOLE_COLOR_CYAN      6
#define CONSOLE_COLOR_WHITE     7

#define WSATTR_REVERSE          1
#define WSATTR_HILIT            2
#define WSATTR_BLINK            4
#define WSATTR_UNDERLINE        8
#define WSATTR_WSCOLORS         16

#define VT100_EMUL_NARGS        10

/*---------------------------------------------------------------------------*
  構造体定義
 *---------------------------------------------------------------------------*/
typedef struct _ConsoleState
{
    int nX, nY;
    int nTop;
    int fgcol, bgcol;
    int attrflags;
} ConsoleState;

typedef struct _ConsoleHandle
{
    u16*        pBuffer;
    void*           pScreen;
    u32         nTransferSize;
    int         nWidth, nHeight;
    int         stateMode;
    ConsoleState    current, saved;
    int     chartab0, chartab1, sschartab;

    int     flags;
    int     nargs;
    int     args[VT100_EMUL_NARGS]; /* command args (CSI/DCS) */

    char    modif1;                 /* {>?} in VT100_STATE_CSI */
    char    modif2;                 /* {!"$&} in VT100_STATE_CSI */
    u16     dummy;
} ConsoleHandle;

/*---------------------------------------------------------------------------*
  関数プロトタイプ定義
 *---------------------------------------------------------------------------*/
int     ConsoleOutputCSI(ConsoleHandle* pHandle, char c);
int     ConsoleOutputEscape(ConsoleHandle* pHandle, char c);
void    ConsoleHandleCSI(ConsoleHandle* pHandle, char c);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif /* __cplusplus */
#endif /* TELNET_VT100_ */
