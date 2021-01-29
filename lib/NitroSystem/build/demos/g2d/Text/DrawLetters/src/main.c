/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DrawLetters
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
//      ������`��֐��̗��p�T���v���ł��B
//      NNS_G2dTextCanvasDrawString
//      NNS_G2dTextCanvasDrawText
//      NNS_G2dTextCanvasDrawTextRect
//      �� 3 �֐��ɂ����ăt���O�ɂ��`��̈Ⴂ�������܂��B
//
//  ������@
//      �f�����s���̉���ʎQ��
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"

#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define CANVAS_WIDTH        22      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS_HEIGHT       16      // �����`���̍���  (�L�����N�^�P��)
#define CANVAS_LEFT         5       // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS_TOP          4       // �����`���̈ʒuY (�L�����N�^�P��)

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̊J�n�ԍ�

#define TRANS_TASK_NUM  1


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont              gFont;          // �t�H���g
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas

// �I�t�X�N���[���o�b�t�@
GXCharFmt16             gOffBuffer[CANVAS_HEIGHT][CANVAS_WIDTH];
NNSGfdVramTransferTask  gTrasTask[TRANS_TASK_NUM];

// �\������e�L�X�g
static const char sSampleText[] =
    "short string\n"
    "It's a long long long long string.\n"
    "\n"
    "after null line";



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
    // �t�H���g��ǂݍ��݂܂�
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        &gOffBuffer,
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
  Name:         SampleInit

  Description:  �f�������̏������B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    InitScreen();
    InitCanvas();

    // �I�t�X�N���[���o�b�t�@�̓]����VRAM�]���}�l�[�W�����g���܂�
    NNS_GfdInitVramTransferManager(gTrasTask, TRANS_TASK_NUM);
}



/*---------------------------------------------------------------------------*
  Name:         SampleString

  Description:  NNS_G2dTextCanvasDrawString() �̃T���v��

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleString(void)
{
    const char* str = sSampleText;
    const int linefeed = NNS_G2dFontGetLineFeed(&gFont) + NNS_G2dTextCanvasGetVSpace(&gTextCanvas);
    const int baselinePos = NNS_G2dFontGetBaselinePos(&gFont);
    int x, y;

    // ���o��
    {
        DTX_PrintLine("NNS_G2dTextCanvasDrawString Sample");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    X:     next mode");
        DTX_PrintLine("");
    }

    // �`��ʒu�����l
    x = 0;
    y = baselinePos;

    // �w�i���N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // ������`��
    while( str != NULL )
    {
        NNS_G2dTextCanvasDrawString(
            &gTextCanvas,           // �`�悷��TextCanvas�ւ̃|�C���^
            x,                      // �`��J�n���W x
            y - baselinePos,        // �`��J�n���W y
            TXT_COLOR_CYAN,         // �����F
            str,                    // �`�悷�镶����
            &str                    // �`��I���ʒu���󂯎��o�b�t�@�ւ̃|�C���^
        );

        y += linefeed;
    }
}



/*---------------------------------------------------------------------------*
  Name:         SampleText

  Description:  NNS_G2dTextCanvasDrawText() �̃T���v���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleText(void)
{
    u32 flag = 0;

    // �w�i���N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // ��_��ʂ鐅��/��������\��
    {
        const int canvas_pw = CANVAS_WIDTH * CHARACTER_WIDTH;
        const int canvas_ph = CANVAS_HEIGHT * CHARACTER_HEIGHT;
        const int origin_x = canvas_pw / 2;
        const int origin_y = canvas_ph / 2;

        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED, 0, origin_y, canvas_pw, 1);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED, origin_x, 0, 1, canvas_ph);
    }

    // �L�[���͏����Ə��o��
    {
        DTX_PrintLine("NNS_G2dTextCanvasDrawText Sample");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    up:    origin bottom");
        DTX_PrintLine("    down:  origin top");
        DTX_PrintLine("    left:  origin right");
        DTX_PrintLine("    right: origin left");
        DTX_PrintLine("    Y:     left align");
        DTX_PrintLine("    A:     right align");
        DTX_PrintLine("    X:     next mode");
        DTX_PrintLine("");

        if( CMN_IsPress(PAD_KEY_UP) )
        {
            flag |= NNS_G2D_VERTICALORIGIN_BOTTOM;
            DTX_PrintLine("  NNS_G2D_VERTICALORIGIN_BOTTOM");
        }
        else if( CMN_IsPress(PAD_KEY_DOWN) )
        {
            flag |= NNS_G2D_VERTICALORIGIN_TOP;
            DTX_PrintLine("  NNS_G2D_VERTICALORIGIN_TOP");
        }
        else
        {
            flag |= NNS_G2D_VERTICALORIGIN_MIDDLE;
            DTX_PrintLine("  NNS_G2D_VERTICALORIGIN_MIDDLE");
        }

        if( CMN_IsPress(PAD_KEY_LEFT) )
        {
            flag |= NNS_G2D_HORIZONTALORIGIN_RIGHT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALORIGIN_RIGHT");
        }
        else if( CMN_IsPress(PAD_KEY_RIGHT) )
        {
            flag |= NNS_G2D_HORIZONTALORIGIN_LEFT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALORIGIN_LEFT");
        }
        else
        {
            flag |= NNS_G2D_HORIZONTALORIGIN_CENTER;
            DTX_PrintLine("  NNS_G2D_HORIZONTALORIGIN_CENTER");
        }

        if( CMN_IsPress(PAD_BUTTON_Y) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_LEFT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_LEFT");
        }
        else if( CMN_IsPress(PAD_BUTTON_A) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_RIGHT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_RIGHT");
        }
        else
        {
            flag |= NNS_G2D_HORIZONTALALIGN_CENTER;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_CENTER");
        }
    }

    // ������`��
    NNS_G2dTextCanvasDrawText(
        &gTextCanvas,                           // �`�悷��TextCanvas�ւ̃|�C���^
        CANVAS_WIDTH * CHARACTER_WIDTH / 2,     // ��_���W   CharCanvas���S
        CANVAS_HEIGHT * CHARACTER_HEIGHT / 2,   // ��_���W   CharCanvas���S
        TXT_COLOR_MAGENTA,                      // �����F
        flag,                                   // �`��ʒu�t���O
        sSampleText                             // �`�悷�镶����
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleTextRect

  Description:  NNS_G2dTextCanvasDrawTextRect() �̃T���v���B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleTextRect(void)
{
    const int canvas_pw = CANVAS_WIDTH * CHARACTER_WIDTH;
    const int canvas_ph = CANVAS_HEIGHT * CHARACTER_HEIGHT;
    const int rect_x    = canvas_pw * 1/6;
    const int rect_y    = canvas_ph * 1/6;
    const int rect_w    = canvas_pw * 2/3;
    const int rect_h    = canvas_ph * 2/3;
    u32 flag = 0;

    // �w�i���N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // ���`��\��
    {
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x - 1,      rect_y - 1,      rect_w + 2, 1);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x - 1,      rect_y + rect_h, rect_w + 2, 1);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x - 1,      rect_y,          1,          rect_h);
        NNS_G2dCharCanvasClearArea(&gCanvas, TXT_COLOR_RED,
            rect_x + rect_w, rect_y,          1,          rect_h);
    }

    // �L�[���͏����Ə��o��
    {
        DTX_PrintLine("NNS_G2dTextCanvasDrawTextRect Sample");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    up:    top align");
        DTX_PrintLine("    down:  bottom align");
        DTX_PrintLine("    left:  left align");
        DTX_PrintLine("    right: right align");
        DTX_PrintLine("    X:     next mode");
        DTX_PrintLine("");

        if( CMN_IsPress(PAD_KEY_UP) )
        {
            flag |= NNS_G2D_VERTICALALIGN_TOP;
            DTX_PrintLine("  NNS_G2D_VERTICALALIGN_TOP");
        }
        else if( CMN_IsPress(PAD_KEY_DOWN) )
        {
            flag |= NNS_G2D_VERTICALALIGN_BOTTOM;
            DTX_PrintLine("  NNS_G2D_VERTICALALIGN_BOTTOM");
        }
        else
        {
            flag |= NNS_G2D_VERTICALALIGN_MIDDLE;
            DTX_PrintLine("  NNS_G2D_VERTICALALIGN_MIDDLE");
        }

        if( CMN_IsPress(PAD_KEY_LEFT) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_LEFT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_LEFT");
        }
        else if( CMN_IsPress(PAD_KEY_RIGHT) )
        {
            flag |= NNS_G2D_HORIZONTALALIGN_RIGHT;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_RIGHT");
        }
        else
        {
            flag |= NNS_G2D_HORIZONTALALIGN_CENTER;
            DTX_PrintLine("  NNS_G2D_HORIZONTALALIGN_CENTER");
        }
    }

    // ������̕`��
    NNS_G2dTextCanvasDrawTextRect(
        &gTextCanvas,                       // �`�悷��TextCanvas�ւ̃|�C���^
        rect_x,                             // ��`�ʒu���W
        rect_y,                             // ��`�ʒu���W
        rect_w,                             // ���`��
        rect_h,                             // ���`����
        TXT_COLOR_GREEN,                    // �����F
        flag,                               // �`��ʒu�t���O
        sSampleText                         // �`�悷�镶����
    );
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
    // �\�����[�h
    enum
    {
        MODE_STRING,        // NNS_G2dTextCanvasDrawString  �̃f��
        MODE_TEXT,          // NNS_G2dTextCanvasDrawText    �̃f��
        MODE_TEXT_RECT,     // NNS_G2dTextCanvasDrawTextRect�̃f��
        NUM_OF_MODE
    }
    mode = MODE_TEXT_RECT;

    // Initilizing App.
    {
        // SDK �ƃf�����C�u������������
        OS_Init();
        TXT_Init();

        // �w�i��ݒ�
        TXT_SetupBackground();

        // �f���p�̑O����
        SampleInit();
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

        // X�{�^���Ń��[�h�ύX
        if( CMN_IsTrigger(PAD_BUTTON_X) )
        {
            mode ++;
            mode %= NUM_OF_MODE;
        }

        // ���[�h�ɉ����ď�������
        switch( mode )
        {
        case MODE_STRING:       SampleString();     break;
        case MODE_TEXT:         SampleText();       break;
        case MODE_TEXT_RECT:    SampleTextRect();   break;
        }

        // �]���^�X�N�̓o�^
        (void)NNS_GfdRegisterNewVramTransferTask(
            NNS_GFD_DST_2D_BG1_CHAR_MAIN,
            sizeof(GXCharFmt16) * CHARACTER_OFFSET,
            gOffBuffer,
            sizeof(gOffBuffer)
        );

        CMN_WaitVBlankIntr();

        // ���o�͂̕\��
        DTX_Reflect();

        // �I�t�X�N���[���o�b�t�@�̓]��
        NNS_GfdDoVramTransfer();
    }
}

