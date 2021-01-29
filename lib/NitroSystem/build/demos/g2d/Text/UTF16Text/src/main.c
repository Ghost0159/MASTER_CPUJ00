/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - UTF16Text
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
//      UTF-16������̕\�����@�T���v���B
//
//  ������@
//      �Ȃ��B
// ============================================================================

// ������`��֐�������������Unicode�ɂ���
#define NNS_G2D_UNICODE

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

NNSG2dFont              gFont;          // �t�H���g
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas



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
    {
//      ���̃f���Ɠ����悤�� TXT_LoadFont ���g���Ă��t�H���g��ǂݍ��߂܂��B
//      ���̃f���ł� NNS_G2dFontInitUTF16 ���g���ꍇ�������Ă��܂��B
//        TXT_LoadFont( &gFont, TXT_UTF16_FONTRESOURCE_NAME );

        void* pFontFile;
        u32 size;

        size = TXT_LoadFile( &pFontFile, TXT_UTF16_FONTRESOURCE_NAME );
        NNS_G2D_ASSERT( size > 0 );

        NNS_G2dFontInitUTF16(&gFont, pFontFile);
        NNS_G2dPrintFont(&gFont);
    }

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pCharBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas�̏�����
    NNS_G2dTextCanvasInit(
        &gTextCanvas,
        &gCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // �X�N���[����ݒ�
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        CANVAS_LEFT,
        CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        CHARACTER_OFFSET,
        TXT_CPALETTE_MAIN
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    InitScreen();
    InitCanvas();

    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);
    NNS_G2dTextCanvasDrawText(&gTextCanvas, 0, 0,
        TXT_COLOR_BLACK, TXT_DRAWTEXT_FLAG_DEFAULT,
        L"UTF-16\n"
        L"0123456789\n"                                     // <- ����
        L"abcdefg ABCDEFG\n"                                // <- �A���t�@�x�b�g
        L"\x00E0\x00E1\x00E2\x00E3\x00E4\x00E5\n"           // <- ���B����
        L"\x3042\x3044\x3046\x3048\x304A\x6F22\x5B57"       // <- ���{�ꕶ��
    );
    // ���{�ꕶ�������� ShiftJIS �ŋL�q���鎖���o���܂���
    // ShiftJIS �ɑΉ����Ă��Ȃ����̂��߂ɕ����R�[�h�ŋL�q���Ă��܂�
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

