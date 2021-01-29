/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - DrawTaggedText
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
//      NNS_G2dTextCanvasDrawTaggedText() �̎g�p�T���v���B
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

// �����񖄂ߍ��ݗp�F�ԍ�
#define WHITE   "\x1"
#define BLACK   "\x2"
#define RED     "\x3"
#define GREEN   "\x4"
#define BLUE    "\x5"
#define CYAN    "\x6"
#define MAGENTA "\x7"
#define YELLOW  "\x8"


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont          gFont;          // �t�H���g
NNSG2dCharCanvas    gCanvas;        // CharCanvas
NNSG2dTextCanvas    gTextCanvas;    // TextCanvas




//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         SimpleTagCallback

  Description:  NNSG2dTagCallback �̃T���v���B

  Arguments:    c:      �R�[���o�b�N�����v���̕����R�[�h�B
                pInfo:  �`����\���̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SimpleTagCallback(u16 c, NNSG2dTagCallbackInfo* pInfo)
{
    if( c == 0x1B )
    {
        const char* pos = (const char*)pInfo->str;

        switch( *pos++ )
        {
        // 0x1B C 0xXX
        case 'C':   // Color
            {
                pInfo->clr = *(u8*)pos;
                pos++;
            }
            break;

        // 0x1B B
        case 'B':   // callBack
            {
                NNS_G2D_POINTER_ASSERT( pInfo->cbParam );

                pos++;  // alignment for NNS_G2D_UNICODE
                pInfo->str = (const NNSG2dChar*)pos;

                ((NNSG2dTagCallback)pInfo->cbParam)( c, pInfo );
            }
            return;

        // 0x1B H 0xXX
        case 'H':   // Horizontal
            {
                s8 hSpace;

                hSpace = *(s8*)pos;
                pos++;

                NNS_G2dTextCanvasSetHSpace(&(pInfo->txn), hSpace);
            }
            break;

        // 0x1B V 0xXX
        case 'V':   // Vertical
            {
                s8 vSpace;

                vSpace = *(s8*)pos;
                pos++;

                NNS_G2dTextCanvasSetVSpace(&(pInfo->txn), vSpace);
           }
            break;

        // 0x1B X 0xXX
        case 'X':   // X
            {
                pInfo->x += *(s8*)pos;
                pos++;
            }
            break;

        // 0x1B Y 0xXX
        case 'Y':   // Y
            {
                pInfo->y += *(s8*)pos;
                pos++;
            }
            break;

        default:
            OS_Warning("NNS_G2dTextCanvasDrawTaggedText: Unknown Tag: %c (0x%02X)\n", *(u8*)(pos-1), *(u8*)(pos-1));
            pos++;  // alignment for NNS_G2D_UNICODE
            break;
        }

        pInfo->str = (const NNSG2dChar*)pos;
    }
}



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
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForBG(
        &gCanvas,
        pCharBase + cOffset,
        CANVAS_WIDTH,
        CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_16
    );

    // TextCanvas �̏�����
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
        cOffset,
        TXT_CPALETTE_MAIN
    );
}




/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
#define TAG_BEGIN       "\x1B"
#define CHG_COLOR(clr)  TAG_BEGIN  "C" clr
#define CHG_LFEED(h)    TAG_BEGIN  "L" #h
#define CHG_HSP(h)      TAG_BEGIN  "H" #h
#define CHG_VSP(v)      TAG_BEGIN  "V" #v
#define ADD_X(x)        TAG_BEGIN  "X" #x
#define ADD_Y(y)        TAG_BEGIN  "Y" #y

static void SampleMain(void)
{
    const static char text[] =
        "change color to " CHG_COLOR(RED) "red " CHG_COLOR(GREEN) "green " CHG_COLOR(CYAN) "cyan" CHG_COLOR(BLACK) "\n"
        CHG_HSP(\x5) "change hspace to 5" CHG_HSP(\x1) "\n"
        ADD_X(\x32) ADD_Y(\x32) "jump x+50 y+50";

    InitScreen();
    InitCanvas();

    // �w�i�N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // �^�O��������`��
    NNS_G2dTextCanvasDrawTaggedText(
        &gTextCanvas,           // �`�悷�� TextCanvas
        0,                      // �`��J�n���W
        0,                      // �`��J�n���W
        TXT_COLOR_BLACK,        // �����F
        text,                   // �`�悷�镶����
        SimpleTagCallback,      // �^�O�����R�[���o�b�N
        NULL                    // �^�O�����R�[���o�b�N�p�p�����[�^
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

