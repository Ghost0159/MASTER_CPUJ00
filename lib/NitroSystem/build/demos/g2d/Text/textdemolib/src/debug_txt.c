/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib
  File:     debug_txt.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

#include "g2d_textdemolib.h"
#include <nnsys/g2d/g2d_TextCanvas.h>


#define DEBUG_CANVAS_WIDTH      32
#define DEBUG_CANVAS_HEIGHT     24
#define DEBUG_CANVAS_LEFT       0
#define DEBUG_CANVAS_TOP        0
#define DEBUG_CANVAS_HSPACE     1
#define DEBUG_CANVAS_VSPACE     1

#define LINEOFFSET_X            1
#define LINEOFFSET_Y            1

#define DEBUG_CHARACTER_OFFSET  0

static NNSG2dTextCanvas         sDebugTextCanvas;
static NNSG2dCharCanvas         sDebugCanvas;
static GXCharFmt16              sDebugOffBuffer[DEBUG_CANVAS_HEIGHT][DEBUG_CANVAS_WIDTH];
static NNSG2dFont               sDebugFont;

static int sCurrentLine = 0;
static BOOL sbUpdated = FALSE;

//****************************************************************************
//
//****************************************************************************

static void ResetOffScreen(void)
{
    NNS_G2dCharCanvasClear(&sDebugCanvas, 0);
    sCurrentLine = 0;
    sbUpdated = FALSE;
}

//****************************************************************************
//
//****************************************************************************

void DTX_Init(void)
{
    static GXRgb bgColorPlt[] =
    {
        GX_RGB(31, 31, 31), GX_RGB(0, 0, 0)
    };

   TXT_LoadFont( &sDebugFont, DEBUG_FONTRESOURCE_NAME );

    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);

    G2S_SetBG0Control(
        GX_BG_SCRSIZE_TEXT_256x256,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0x7800,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01
    );

    GXS_LoadBGPltt(bgColorPlt, 0, sizeof(bgColorPlt));

    NNS_G2dCharCanvasInitForBG(
        &sDebugCanvas,
        *sDebugOffBuffer,
        DEBUG_CANVAS_WIDTH,
        DEBUG_CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    NNS_G2dTextCanvasInit(
        &sDebugTextCanvas,
        &sDebugCanvas,
        &sDebugFont,
        DEBUG_CANVAS_HSPACE,
        DEBUG_CANVAS_VSPACE
    );

    NNS_G2dMapScrToCharText(
        G2S_GetBG0ScrPtr(),
        DEBUG_CANVAS_WIDTH,
        DEBUG_CANVAS_HEIGHT,
        DEBUG_CANVAS_LEFT,
        DEBUG_CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        DEBUG_CHARACTER_OFFSET,
        0
    );

    ResetOffScreen();
}


//****************************************************************************
//
//****************************************************************************


void DTX_Reflect(void)
{
    if( sbUpdated )
    {
        DC_StoreRange(sDebugOffBuffer, sizeof(sDebugOffBuffer));
        GXS_LoadBG0Char(
            sDebugOffBuffer,
            sizeof(GXCharFmt16) * DEBUG_CHARACTER_OFFSET,
            sizeof(sDebugOffBuffer)
        );

        ResetOffScreen();
    }
}

void DTX_Print( int x, int y, const char* str )
{
    NNS_G2dTextCanvasDrawText(
        &sDebugTextCanvas,
        x, y, 1,
        NNS_G2D_VERTICALORIGIN_TOP | NNS_G2D_HORIZONTALORIGIN_LEFT | NNS_G2D_HORIZONTALALIGN_LEFT,
        str
    );
    sbUpdated = TRUE;
}

void DTX_PrintLine( const char* fmt, ... )
{
    const int y = sCurrentLine * (NNS_G2dFontGetHeight(&sDebugFont) + NNS_G2dTextCanvasGetVSpace(&sDebugTextCanvas)) + LINEOFFSET_Y;
    char buf[1024];
    const char* pos;
    va_list vlist;

    va_start( vlist, fmt );
    (void)OS_VSNPrintf(buf, sizeof(buf), fmt, vlist);
    va_end( vlist );

    DTX_Print( LINEOFFSET_X, y, buf );

    for( pos = buf; *pos != '\0'; pos++ )
    {
        if( *pos == '\n' )
        {
            sCurrentLine++;
        }
    }

    sCurrentLine++;
    sbUpdated = TRUE;
}

