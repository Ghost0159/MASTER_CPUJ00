/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DoubleBuffering
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
//      �_�u���o�b�t�@�����OON�܂���OFF�ŕ������\�����܂��B
//      ������̕`��ɂ͔��Ɏ��Ԃ�������̂Ń_�u���o�b�t�@�����O���s��Ȃ���
//      �\����������܂��B
//
//  ������@
//      A : �_�u���o�b�t�@�����O��ON/OFF��؂�ւ��܂��B
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include "g2d_textdemolib.h"



#define CANVAS_WIDTH        30      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS_HEIGHT       22      // �����`���̍���  (�L�����N�^�P��)
#define CANVAS_LEFT         1       // �����`���̈ʒuX (�L�����N�^�P��)
#define CANVAS_TOP          1       // �����`���̈ʒuY (�L�����N�^�P��)

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̊J�n�ԍ�


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

// �I���X�N���[���\���p
NNSG2dCharCanvas        gOnCanvas;
NNSG2dTextCanvas        gOnTextCanvas;

// �I�t�X�N���[���\���p
#define TRANS_TASK_NUM  1
NNSG2dCharCanvas        gOffCanvas;
NNSG2dTextCanvas        gOffTextCanvas;

  // �I�t�X�N���[���o�b�t�@
  //   CharCanvas�Ɠ��T�C�Y�̃L�����N�^�z��
GXCharFmt16             gOffBuffer[CANVAS_HEIGHT][CANVAS_WIDTH];
NNSGfdVramTransferTask  gTrasTask[TRANS_TASK_NUM];

// ����
NNSG2dFont              gFont;




//****************************************************************************
// Initialize etc.
//****************************************************************************


/*---------------------------------------------------------------------------*
  Name:         InitCanvasShare

  Description:  �I��/�I�t���L���\�[�X�����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitCanvasShare(void)
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

    // �t�H���g�����[�h
    TXT_LoadFont(&gFont, TXT_FONTRESOURCE_NAME);

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
  Name:         InitOnScreenCanvas

  Description:  �I���X�N���[���\����Canvas�����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
// �I���X�N���[���\�����̏�����
static void InitOnScreenCanvas(void)
{
    GXCharFmt16* const charBase = (GXCharFmt16*)G2_GetBG1CharPtr();

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gOnCanvas,
        charBase + CHARACTER_OFFSET,    // VRAM �̃A�h���X
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas �̏�����
    NNS_G2dTextCanvasInit(
        &gOnTextCanvas,
        &gOnCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );
}



/*---------------------------------------------------------------------------*
  Name:         InitOffScreenCanvas

  Description:  �I�t�X�N���[���\����Canvas�����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
// �I�t�X�N���[���\�����̏�����
static void InitOffScreenCanvas(void)
{
    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gOffCanvas,
        gOffBuffer,             // VRAM�̃A�h���X�ł͂Ȃ��I�t�X�N���[���o�b�t�@�̃A�h���X��n��
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas �̏�����
    NNS_G2dTextCanvasInit(
        &gOffTextCanvas,
        &gOffCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // �I�t�X�N���[���o�b�t�@�̓]����VRAM�]���}�l�[�W�����g���܂�
    NNS_GfdInitVramTransferManager(gTrasTask, TRANS_TASK_NUM);
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  �T���v���`��̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    // ���L���\�[�X�̏�����
    InitCanvasShare();

    // �I���X�N���[���\���p������
    InitOnScreenCanvas();

    // �I�t�X�N���[���\���p������
    InitOffScreenCanvas();
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  �T���v���̕`����s���܂��B

  Arguments:    pTextCanvas:    TextCanvas �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleDraw(NNSG2dTextCanvas *pTextCanvas)
{
    static int y = 0;

    // Canvas �N���A
    NNS_G2dCharCanvasClear(NNS_G2dTextCanvasGetCharCanvas(pTextCanvas), TXT_COLOR_NULL);

    // �`��
    NNS_G2dTextCanvasDrawText(
        pTextCanvas,
        0, y,
        TXT_COLOR_BLACK,
        TXT_DRAWTEXT_FLAG_DEFAULT,
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyz\n"
    );

    // ���������ցB
    y ++;
    if( y > 100 )
    {
        y = 0;
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
    BOOL bDoubleBuffering = FALSE;

    // Initilizing App.
    {
        // SDK �ƃf�����C�u������������
        OS_Init();
        TXT_Init();

        // �w�i��ݒ�
        TXT_SetupBackground();

        // �T���v���̏���������
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

        {
            // �I���I�t�`���؂�ւ��܂��B
            if( CMN_IsTrigger(PAD_BUTTON_A) )
            {
                bDoubleBuffering = ! bDoubleBuffering;
            }
            DTX_PrintLine(
                "Double Buffering Sample\n"
                "operation\n"
                "  A    on/off double buffering\n"
            );
            DTX_PrintLine(bDoubleBuffering ? "DoubleBuffering: ON": "DoubleBuffering: OFF");
        }

    //----------------------------------
    // LCD �\�����Ԓ�

        if( bDoubleBuffering )
        {
            // �I�t�X�N���[���\���ł͓]�����ȊO�͂��ł��`��ł��܂��B
            SampleDraw( &gOffTextCanvas );

            // V�u�����N���Ԓ��̓]���^�X�N��o�^���܂��B
            (void)NNS_GfdRegisterNewVramTransferTask(
                NNS_GFD_DST_2D_BG1_CHAR_MAIN,
                sizeof(GXCharFmt16) * CHARACTER_OFFSET,
                gOffBuffer,
                sizeof(gOffBuffer)
            );
        }
        else
        {
            // �I���X�N���[���\���ł͉������鎖�͂���܂���
        }

    //
    //----------------------------------

        CMN_WaitVBlankIntr();
        DTX_Reflect();

    //----------------------------------
    // V �u�����N���Ԓ�

        if( bDoubleBuffering )
        {
            // �I�t�X�N���[���\���ł�V�u�����N���ɕ`��ς݃C���[�W��VRAM�֓]�����܂��B
            NNS_GfdDoVramTransfer();
        }
        else
        {
            // �I���X�N���[���\���ł�V�u�����N����VRAM�ɕ`�悵�܂��B
            SampleDraw( &gOnTextCanvas );
        }

    //
    //----------------------------------
    }
}

