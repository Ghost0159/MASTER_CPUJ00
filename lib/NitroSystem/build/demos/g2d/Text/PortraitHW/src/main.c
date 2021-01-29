/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - PortraitHW
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
//      �c��ʂ̂��߂̉�ʕ`����s���܂��B
//      ����ʗp�ɕϊ������t�H���g��p���ĕ`����s���A
//      BG �̉�]���g�p���ďc��ʕ\�����s���܂��B
//
//  ������@
//      �\���{�^���̉�������������ʏ�ƂȂ�悤�� BG ����]���܂��B
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"


#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define CANVAS_WIDTH        21      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS_HEIGHT       13      // �����`���̍���  (�L�����N�^�P��)
#define CANVAS_LEFT         2       // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS_TOP          7       // �����`���̈ʒuY (�L�����N�^�P��)

#define BG_WIDTH            32      // �������`�悷�� BG �̕�   (�L�����N�^�P��)
#define BG_HEIGHT           32      // �������`�悷�� BG �̍��� (�L�����N�^�P��)

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̊J�n�ԍ�


//------------------------------------------------------------------------------
// �^

typedef enum Direction
{
    DIR_H_0,    // ������ ��ʂ̍�����E��
    DIR_H_90,   // ������ ��ʂ̏ォ�牺
    DIR_H_180,  // ������ ��ʂ̉E���獶��
    DIR_H_270,  // ������ ��ʂ̉�������
    DIR_NULL
}
Direction;


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont              gFont;          // �t�H���g
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas


// �\������e�L�X�g
const char* const sSampleText = "abcdefghijklmnopqrstuvwxyz\n"
                                "The quick brown fox jumps over\n"
                                "the lazy dog\n";



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
    // BG 3 ��ݒ�
    // Affine BG �� 256x16�p���b�g BG �łȂ����
    // BG �̉�]�͎g�p�ł��܂���
    G2_SetBG3Control256x16Pltt(
        GX_BG_SCRSIZE_256x16PLTT_256x256,   // �X�N���[���T�C�Y 256x256
        GX_BG_AREAOVER_XLU,                 // �G���A�I�[�o�[����
        GX_BG_SCRBASE_0x0000,               // �X�N���[���x�[�X
        GX_BG_CHARBASE_0x00000              // �L�����N�^�x�[�X
    );

    // BG 3 ������
    CMN_SetPlaneVisible( GX_PLANEMASK_BG3 );

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
    GXCharFmt256* pChrBase = (GXCharFmt256*)G2_GetBG3CharPtr();
    GXScrFmtText* pScrBase = (GXScrFmtText*)G2_GetBG3ScrPtr();
    int cOffset = CHARACTER_OFFSET;

    // �X�N���[�����N���A
    MI_CpuClear16(G2_GetBG3ScrPtr(), sizeof(GXScrFmtText) * BG_WIDTH * BG_HEIGHT);

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pChrBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_256
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
    NNS_G2dMapScrToChar256x16Pltt(
        pScrBase + CANVAS_TOP * BG_WIDTH
                 + CANVAS_LEFT,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_256x16PLTT_BG_WIDTH_256,
        cOffset,
        0           // 0 �Ԃ� 256�F�J���[�p���b�g���w��
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  �T���v���������`�悵�܂��B

  Arguments:    x:      �`�悷��ʒu x
                y:      �`�悷��ʒu y

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleDraw(int x, int y)
{
    // �w�i�N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // �e�L�X�g�`��
    NNS_G2dTextCanvasDrawText(&gTextCanvas, x, y, TXT_COLOR_BLACK,
            TXT_DRAWTEXT_FLAG_DEFAULT, sSampleText);
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  �T���v���̏����ݒ菈���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    // �t�H���g�ǂݍ���
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // �X�N���[���ݒ�
    InitScreen();

    // �L�����o�X�\�z
    InitCanvas();

    // �e�L�X�g�`��
    SampleDraw(0, 0);
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    static Direction rotation = DIR_NULL;
    BOOL bUpdated = FALSE;

    //---- ���͂ɉ����ĕ\���̕�����ύX���܂��B
    if( CMN_IsTrigger(PAD_KEY_UP) || rotation == DIR_NULL )
    {
        rotation = DIR_H_0;
        bUpdated = TRUE;
    }
    if( CMN_IsTrigger(PAD_KEY_DOWN) )
    {
        rotation = DIR_H_180;
        bUpdated = TRUE;
    }
    if( CMN_IsTrigger(PAD_KEY_LEFT) )
    {
        rotation = DIR_H_270;
        bUpdated = TRUE;
    }
    if( CMN_IsTrigger(PAD_KEY_RIGHT) )
    {
        rotation = DIR_H_90;
        bUpdated = TRUE;
    }

    //---- �������ύX���ꂽ�ꍇ�ɕύX�𔽉f���܂��B
    if( bUpdated )
    {
        int centerX, centerY;
        MtxFx22 m;

        switch( rotation )
        {
        case DIR_H_0:
            {
                MTX_Rot22(&m, FX32_SIN0,   FX32_COS0);
                centerX      = 0;
                centerY      = 0;
            }
            break;

        case DIR_H_90:
            {
                MTX_Rot22(&m, FX32_SIN90, FX32_COS90);
                centerX      = GX_LCD_SIZE_X / 2;
                centerY      = GX_LCD_SIZE_X / 2;
            }
            break;

        case DIR_H_180:
            {
                MTX_Rot22(&m, FX32_SIN180, FX32_COS180);
                centerX      = GX_LCD_SIZE_X / 2;
                centerY      = GX_LCD_SIZE_Y / 2;
            }
            break;

        case DIR_H_270:
            {
                MTX_Rot22(&m, FX32_SIN270, FX32_COS270);
                centerX      = GX_LCD_SIZE_Y / 2;
                centerY      = GX_LCD_SIZE_Y / 2;
            }
            break;

        default:
            return;
        }

        // BG �̉�]��ݒ�
        G2_SetBG3Affine(&m, centerX, centerY, 0, 0);


        //---- ���b�Z�[�W�\��
        DTX_PrintLine("PortraitHW demo");
        DTX_PrintLine("  operation:");
        DTX_PrintLine("    +: change direction");
        DTX_PrintLine("");
        DTX_PrintLine("direction:");
        switch( rotation )
        {
        case DIR_H_0:   DTX_PrintLine("  left to right"); break;
        case DIR_H_90:  DTX_PrintLine("  top to bottom"); break;
        case DIR_H_180: DTX_PrintLine("  right to left"); break;
        case DIR_H_270: DTX_PrintLine("  bottom to up");  break;
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

        CMN_WaitVBlankIntr();
        SampleMain();

        // ���o�͂̕\��
        DTX_Reflect();
    }
}

