/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - BGText
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      TEXT BG �ł̕�����\���T���v���i16�F���[�h�j
//      �A�t�B���g��BG�� TEXT BG �Ɠ��l�ɂ��ĕ\�����鎖���o���܂��B
//
//  ������@
//      ����Ȃ��B
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


// TEXT BG �y�уA�t�B���g��BG�ł� 1024 �L�����N�^�܂Ŏg�p�ł��܂��B

#define CANVAS0_WIDTH       10      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS0_HEIGHT      5       // �����`���̍���  (�L�����N�^�P��)
#define CANVAS0_LEFT        1       // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS0_TOP         1       // �����`���̈ʒuY (�L�����N�^�P��)

#define CANVAS1_WIDTH       25      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS1_HEIGHT      8       // �����`���̍���  (�L�����N�^�P��)
#define CANVAS1_LEFT        3       // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS1_TOP         15      // �����`���̈ʒuY (�L�����N�^�P��)

#define CANVAS2_WIDTH       15      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS2_HEIGHT      10      // �����`���̍���  (�L�����N�^�P��)
#define CANVAS2_LEFT        15      // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS2_TOP         3       // �����`���̈ʒuY (�L�����N�^�P��)

#define TEXT_HSPACE0        4       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE0        4       // ������`�掞�̍s��   (�s�N�Z���P��)

#define TEXT_HSPACE1        1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE1        1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̊J�n�ԍ�


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

static NNSG2dFont           gFont;              // �t�H���g
static NNSG2dCharCanvas     gCanvas[3];         // CharCanvas x3
static NNSG2dTextCanvas     gTextCanvas[3];     // TextCanvas x3



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
    // BG�L�����N�^�x�[�X�ւ̃|�C���^
    GXCharFmt16* const pCharBase = (GXCharFmt16*)G2_GetBG1CharPtr();
    int cOffset = CHARACTER_OFFSET;

    // �t�H���g��ǂݍ��݂܂�
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // CharCanvas �̏������� BG �X�N���[���̐ݒ�
    // CharCanvas 0
    NNS_G2dCharCanvasInitForBG(
        &gCanvas[0],                    // CharCanvas�ւ̃|�C���^
        pCharBase + cOffset,            // �g�p����L�����N�^��擪�ւ̃|�C���^
        CANVAS0_WIDTH,                  // CharCanvas��
        CANVAS0_HEIGHT,                 // CharCanvas����
        NNS_G2D_CHARA_COLORMODE_16      // �J���[���[�h
    );
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),              // �X�N���[���x�[�X�ւ̃|�C���^
        CANVAS0_WIDTH,                  // CharCanvas��
        CANVAS0_HEIGHT,                 // CharCanvas����
        CANVAS0_LEFT,                   // CharCanvas�\���ʒu
        CANVAS0_TOP,                    // CharCanvas�\���ʒu
        NNS_G2D_TEXT_BG_WIDTH_256,      // �X�N���[����
        cOffset,                        // �g�p����L�����N�^��擪�̃L�����N�^�ԍ�
        TXT_CPALETTE_MAIN               // �J���[�p���b�g�ԍ�
    );

    // CharCanvas 0 ���g�����̃L�����N�^�����I�t�Z�b�g�ɉ��Z
    cOffset += CANVAS0_WIDTH * CANVAS0_HEIGHT;

    // CharCanvas 1
    NNS_G2dCharCanvasInitForBG(
        &gCanvas[1],
        pCharBase + cOffset,
        CANVAS1_WIDTH,
        CANVAS1_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        CANVAS1_WIDTH,
        CANVAS1_HEIGHT,
        CANVAS1_LEFT,
        CANVAS1_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        cOffset,
        TXT_CPALETTE_MAIN
    );

    // CharCanvas 1 ���g�����̃L�����N�^�����I�t�Z�b�g�ɉ��Z
    cOffset += CANVAS1_WIDTH * CANVAS1_HEIGHT;

    // CharCanvas 2
    NNS_G2dCharCanvasInitForBG(
        &gCanvas[2],
        pCharBase + cOffset,
        CANVAS2_WIDTH,
        CANVAS2_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        CANVAS2_WIDTH,
        CANVAS2_HEIGHT,
        CANVAS2_LEFT,
        CANVAS2_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        cOffset,
        TXT_CPALETTE_MAIN
    );

    // TextCanvas �̏�����
    // CharCanvas ���� TextCanvas ��p�ӂ��邱�Ƃ��ł��܂����A
    // ������ TextCanvas ��1�� CharCanvas �����L���鎖���ł��܂��B
    // ������ CharCanvas ��1�� TextCanvas �Ŏg���ꍇ��
    // TextCanvas �� CharCanvas ��u�������Ȃ���g�����ɂȂ�܂��B

    // 1) ������ TextCanvas ��1�� CharCanvas �����L
    // CharCanvas 1 �� TextCanvas 0, 1 �ŋ��L
    {
        NNS_G2dTextCanvasInit(
            &gTextCanvas[0],        // TextCanvas�ւ̃|�C���^
            &gCanvas[1],            // �`����CharCanvas�ւ̃|�C���^
            &gFont,                 // �`��ɗp����t�H���g�ւ̃|�C���^
            TEXT_HSPACE0,           // ������
            TEXT_VSPACE0            // �s��
        );
        NNS_G2dTextCanvasInit(
            &gTextCanvas[1],
            &gCanvas[1],
            &gFont,
            TEXT_HSPACE1,
            TEXT_VSPACE1
        );
    }

    // 2) CharCanvas ���� TextCanvas ��p��
    // CharCanvas 2 �� TextCanvas 2 �P�ƂŎg�p
    {
        NNS_G2dTextCanvasInit(
            &gTextCanvas[2],
            &gCanvas[2],
            &gFont,
            TEXT_HSPACE0,
            TEXT_VSPACE0
        );
    }
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    InitScreen();
    InitCanvas();

    // ���ꂼ��� CharCanvas �� TXT_COLOR_WHITE �œh��Ԃ�
    NNS_G2dCharCanvasClear(&gCanvas[0], TXT_COLOR_WHITE);
    NNS_G2dCharCanvasClear(&gCanvas[1], TXT_COLOR_WHITE);
    NNS_G2dCharCanvasClear(&gCanvas[2], TXT_COLOR_WHITE);

    // ������`��
    // TextCanvas 0 (->CharCanvas 1) �ɕ`��
    NNS_G2dTextCanvasDrawText(&gTextCanvas[0], 0, 0, TXT_COLOR_CYAN, TXT_DRAWTEXT_FLAG_DEFAULT,
        "TextCanvas0 CharCanvas1\n"
        "abcdefg"
    );

    // TextCanvas 1 (->CharCanvas 1) �ɕ`��
    NNS_G2dTextCanvasDrawText(&gTextCanvas[1], 0, 30, TXT_COLOR_MAGENTA, TXT_DRAWTEXT_FLAG_DEFAULT,
        "TextCanvas1 CharCanvas1\n"
        "hijklmn"
    );

    // TextCanvas 2 (->CharCanvas 2) �ɕ`��
    NNS_G2dTextCanvasDrawText(&gTextCanvas[2], 0, 0, TXT_COLOR_GREEN, TXT_DRAWTEXT_FLAG_DEFAULT,
        "TextCanvas2\n"
        "CharCanvas2\n"
        "opqrstu"
    );


    // 3) ������ CharCanvas ��1�� TextCanvas �Ŏg��
    {
        // TextCanvas 1 �� CharCanvas �����ւ�
        NNS_G2dTextCanvasSetCharCanvas(&gTextCanvas[1], &gCanvas[0]);

        // TextCanvas 1 (->CharCanvas 0) �ɕ`��
        NNS_G2dTextCanvasDrawText(&gTextCanvas[1], 0, 0, TXT_COLOR_BLUE, TXT_DRAWTEXT_FLAG_DEFAULT,
            "TextCanvas1\n"
            "CharCanvas0\n"
            "vwxyz"
        );
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

