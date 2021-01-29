/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DrawLetter
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �����`��֐��̎g�p�T���v���ł��B
//      1�������F��ς��Ȃ���K�i��ɕ�����`�悵�܂��B
//
//  ������@
//      ����Ȃ��B
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


#define CANVAS_WIDTH        22      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS_HEIGHT       16      // �����`���̍���  (�L�����N�^�P��)
#define CANVAS_LEFT         5       // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS_TOP          4       // �����`���̈ʒuY (�L�����N�^�P��)

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̊J�n�ԍ�

//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont                  gFont;          // �t�H���g
static NNSG2dCharCanvas     gCanvas;            // CharCanvas



//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  BG �ʂ�ݒ肵�܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitScreen(void)
{
    // BG 1 ��ݒ�
    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,     // �X�N���[���T�C�Y 256x256
        GX_BG_COLORMODE_16,             // �J���[���[�h     16�F
        GX_BG_SCRBASE_0x0000,           // �X�N���[���x�[�X
        GX_BG_CHARBASE_0x00000,         // �L�����N�^�x�[�X
        GX_BG_EXTPLTT_01                // �g���p���b�g�X���b�g
    );

    // BG1 ������
    CMN_SetPlaneVisible( GX_PLANEMASK_BG1 );

    // �J���[�p���b�g��ݒ�
    GX_LoadBGPltt(TXTColorPalette, 0, sizeof(TXTColorPalette));
}



/*---------------------------------------------------------------------------*
  Name:         InitCanvas

  Description:  ������`��̏����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitCanvas(void)
{
    GXCharFmt16* pCharBase = (GXCharFmt16*)G2_GetBG1CharPtr();
    int cOffset = CHARACTER_OFFSET;

    // �t�H���g��ǂݍ��݂܂�
    // ���̃f���ƈႢ�A�킩��₷���悤�ɍ��X�y�[�X�����t�H���g���g�p���܂�
    TXT_LoadFont( &gFont, DEBUG_FONTRESOURCE_NAME );

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pCharBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // �X�N���[����ݒ�
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        CANVAS_LEFT,
        CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        cOffset,
        TXT_CPALETTE_USERCOLOR
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#define NEXT_COLOR(c)   ((u8)(((c) + 1) & 0xF))
static void SampleMain(void)
{
    // �\�����镶����
    const char* const str = "abcdefghijklmnopqrstuvwxyz";
    const char* pos;
    const u8 bgColor = TXT_UCOLOR_GRAY;

    int x = 0;
    int y = 0;
    u8 clr = 0;
    NNSG2dGlyph glyph;

    InitScreen();
    InitCanvas();

    // �w�i�N���A
    NNS_G2dCharCanvasClear(&gCanvas, bgColor);

    // DrawChar �ŕ`��
    for( pos = str; *pos != '\0'; pos++ )
    {
        int width;

        // NNS_G2dCharCanvasDrawChar �̕Ԃ�l�͕�����(=���X�y�[�X�� + �O���t�� + �E�X�y�[�X��) �ł�
        width = NNS_G2dCharCanvasDrawChar(&gCanvas, &gFont, x, y, clr, *pos);

        // �����������Z���܂�
        x += width;

        x += TEXT_HSPACE;
        y += TEXT_VSPACE;

        // ���̃J���[
        clr = NEXT_COLOR(clr);
        if( clr == bgColor )
        {
            clr = NEXT_COLOR(clr);
        }
    }

    // DrawGlyph �ŕ`��
    x = 0;
    for( pos = str; *pos != '\0'; pos++ )
    {
        NNS_G2dFontGetGlyph(&glyph, &gFont, *pos);
        NNS_G2dCharCanvasDrawGlyph(&gCanvas, &gFont, x, y, clr, &glyph);

        // �O���t���݂̂����Z���܂��B
        x += glyph.pWidths->glyphWidth;

        x += TEXT_HSPACE;
        y += TEXT_VSPACE;

        // ���̃J���[
        clr = NEXT_COLOR(clr);
        if( clr == bgColor )
        {
            clr = NEXT_COLOR(clr);
        }
    }
}




//****************************************************************************
// Main
//****************************************************************************


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // Initilizing App.
    {
        // SDK �ƃf�����C�u������������
        OS_Init();
        TXT_Init();

        // �w�i��ݒ�
        TXT_SetupBackground();

        // �T���v���̃��C������
        SampleMain();
    }

    // start display
    {
        CMN_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        CMN_ReadGamePad();

        CMN_WaitVBlankIntr();

        // ���o�͂̕\��
        DTX_Reflect();
    }
}

