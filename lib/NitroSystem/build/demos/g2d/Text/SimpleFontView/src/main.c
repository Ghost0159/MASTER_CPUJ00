/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - SimpleFontView
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �t�H���g���\�[�X���̃O���t�C���[�W��\�����܂��B
//      data �f�B���N�g������ fonts.NFTR ���폜���A�ʂ̃t�H���g�����Ă���
//      ���r���h����΁A����ւ����t�H���g�̓��e��\�����܂��B
//      data �f�B���N�g������ fontd.NFTR �͍폜���Ȃ��ŉ������B
//
//  ������@
//      ����    1�����ړ�
//      ����    1�s���ړ�
//      B       �����Ă���ԃL�[���s�[�g�L��
//      X       �g�嗦�ύX 4��1��2��3��4��1���c
// ============================================================================

#include "g2d_textdemolib.h"
#include <string.h>
#include <nnsys/gfd.h>

#define SCREEN_WIDTH        256                     // ��ʕ�
#define SCREEN_HEIGHT       192                     // ��ʍ���

#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8



#define LIMIT_GLYPH_INDEX(gi)   \
    while( gi > giMax )         \
    {                           \
        gi -= giMax + 1;        \
    }                           \
    while( gi < 0 )             \
    {                           \
        gi += giMax + 1;        \
    }


//------------------------------------------------------------------------------
// �O���[�o���ϐ�
NNSG2dCharCanvas    gCc;        // �O���t���X�g/�O���t�C���f�b�N�X�\���p CharCanvas
NNSG2dTextCanvas    gTxn;       // �O���t���X�g�\���p TextCanvas
NNSG2dTextCanvas    gITxn;      // �O���t�C���f�b�N�X�\���p TextCanvas

NNSG2dCharCanvas    gDCc;       // �O���t�C���[�W�g��\���p CharCanvas
NNSG2dTextCanvas    gDTxn;      // �O���t�C���[�W�g��\���p TextCanvas

NNSG2dFont          gFont;      // �\���Ώۃt�H���g
NNSG2dFont          gInfoFont;  // ���\���p�t�H���g

int                 giCenter;   // �O���t���X�g�����̃O���t�C���f�b�N�X
int                 giMax;      // gFont �̂��O���t�C���f�b�N�X�ő�l
int                 giLine;     // �O���t���X�g�ɕ\������O���t��

static GXCharBGText256          gBG1OffScreen;      // �O���t���X�g/�O���t�C���f�b�N�X�\���p
static GXCharBGAffine256        gBG2OffScreen;      // �O���t�C���[�W�g��\���p
static NNSGfdVramTransferTask   gTransferTask[2];   // �I�t�X�N���[���]���p

const static int BG1_CANVAS_WIDTH   = 32;
const static int BG1_CANVAS_HEIGHT  = 24;
const static int BG1_CANVAS_LEFT    =  0;
const static int BG1_CANVAS_TOP     =  0;
const static int BG1_CANVAS_OFFSET  =  1;

const static int BG2_CANVAS_WIDTH   = 16;
const static int BG2_CANVAS_HEIGHT  = 12;
const static int BG2_CANVAS_LEFT    =  0;
const static int BG2_CANVAS_TOP     =  0;
const static int BG2_CANVAS_OFFSET  =  1;

const static int CHAR_SPACE         =  5;
const static int MARGIN             =  5;

const static int REPEAT_THRESHOLD   = 22;

static int scale = 4;
static char fontName[FS_FILE_NAME_MAX + 1];



//****************************************************************************
// wrapper
//****************************************************************************
static inline GetCharWidth(const NNSG2dCharWidths* pWidths)
{
    return pWidths->charWidth;
}


//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         ClearAreaSafe

  Description:  ������K�؂Ȕ͈͂Ɏ��߂Ă��� NNS_G2dCharCanvasClearArea ��
                �Ăяo���܂��B

  Arguments:    pCC:    CharCanvas �ւ̃|�C���^�B
                cl:     �h��Ԃ��̂ɗp����J���[�ԍ��B
                x:      ��`����� x ���W�B
                y:      ��`����� y ���W�B
                w:      ��`���B
                h:      ��`�����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ClearAreaSafe(
    const NNSG2dCharCanvas* pCC,
    int cl,
    int x,
    int y,
    int w,
    int h
)
{
    // �����Ɏw��\�ȍő�l
    const int x_max = pCC->areaWidth * CHARACTER_WIDTH;
    const int y_max = pCC->areaHeight * CHARACTER_HEIGHT;

    if( x < 0 )
    {
        x = 0;
    }
    if( y < 0 )
    {
        y = 0;
    }
    if( x_max <= x )
    {
        x = x_max - 1;
    }
    if( y_max <= y )
    {
        y = y_max - 1;
    }
    if( x_max < w )
    {
        w = x_max;
    }
    if( y_max < h )
    {
        h = y_max;
    }
    if( x_max < x + w )
    {
        w = x_max - x;
    }
    if( y_max < y + h )
    {
        h = y_max - y;
    }
    if( w < 0 || h < 0 )
    {
        return;
    }

    NNS_G2dCharCanvasClearArea(pCC, cl, x, y, w, h);
}



/*---------------------------------------------------------------------------*
  Name:         LoadFont

  Description:  �t�H���g�����[�h���܂��B
                �g���q .NFTR �̃t�@�C����T����1�ԍŏ��Ɍ����������̂�
                ���[�h���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadFont(void)
{
    BOOL bSuccess;
    FSFile dir;
    FSDirEntry entry;

    FS_InitFile(&dir);

    bSuccess = FS_FindDir(&dir, "/data");
    SDK_ASSERT( bSuccess );

    // �f�B���N�g������T��
    while( FS_ReadDir(&dir, &entry) != FALSE )
    {
        if( entry.name_len >= 5 )
        {
            // �g���q�� .NFTR �ł�����
            if( strcmp( entry.name + entry.name_len - 5, ".NFTR" ) == 0 )
            {
                // �f�o�b�O�\���p�̃t�H���g�ł͂Ȃ�
                if( strcmp( entry.name, "fontd.NFTR" ) != 0 )
                {
                    char fname[6 + FS_FILE_NAME_MAX + 1] = "/data/";

                    (void)strcat(fname, entry.name);
                    (void)strcpy(fontName, entry.name);

                    // �t�H���g�����[�h
                    TXT_LoadFont( &gFont, fname );
                    return;
                }
            }
        }
    }

    SDK_ASSERTMSG(FALSE, "There are no fonts.");
}



/*---------------------------------------------------------------------------*
  Name:         InitScreenCommon

  Description:  �eBG�ʂŋ��ʂ̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitScreenCommon(void)
{
    // �o�b�N�h���b�v�F + ���\�������F + �g�F + �ő�t�H���g�K���� (32�K���܂�)
    const int max_colors = 32;
    static u16 colorPalette[1 + 1 + max_colors] =
    {
        GX_RGB(0, 0, 31), GX_RGB(9, 27, 17), GX_RGB(27, 2, 2)
    };

    // ���\���t�H���g�����[�h
    TXT_LoadFont( &gInfoFont, DEBUG_FONTRESOURCE_NAME );

    // �\���Ώۃt�H���g�����[�h
    LoadFont();

    // ���[�h�����t�H���g�ɉ����ăJ���[�p���b�g���쐬
    {
        const int nColors = MATH_IMin((1 << NNS_G2dFontGetBpp(&gFont)), max_colors);
        int i;

        for( i = 0; i < nColors; ++i )
        {
            int level = ((nColors - 1 - i) * (max_colors - 1) / (nColors - 1));

            colorPalette[i+3] = GX_RGB(level, level, level);
        }
    }

    // �J���[�p���b�g�����[�h
    GX_LoadBGPltt(colorPalette, 0, sizeof(colorPalette));
}



/*---------------------------------------------------------------------------*
  Name:         InitEnumScreen

  Description:  �������X�g/�O���t�C���f�b�N�X�\���p Canvas ������

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitEnumScreen(void)
{
    const static int margin = 1;

    // BG 1 ���e�L�X�gBG�ɐݒ�
    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,
        GX_BG_COLORMODE_256,
        GX_BG_SCRBASE_0x0000,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01
    );

    // BG1 ������
    CMN_SetPlaneVisible(GX_PLANEMASK_BG1);

    // CharCanvas������
    NNS_G2dCharCanvasInitForBG(
        &gCc,
        &gBG1OffScreen,
        BG1_CANVAS_WIDTH,
        BG1_CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_256
    );

    // �X�N���[���ݒ�
    NNS_G2dMapScrToCharText(
        G2_GetBG1ScrPtr(),
        BG1_CANVAS_WIDTH,
        BG1_CANVAS_HEIGHT,
        BG1_CANVAS_LEFT,
        BG1_CANVAS_TOP,
        NNS_G2D_TEXT_BG_WIDTH_256,
        BG1_CANVAS_OFFSET,
        0
    );

    // CharCanvas�N���A
    NNS_G2dCharCanvasClear(&gCc, 0);

    // TextCanvas������
    NNS_G2dTextCanvasInit(&gTxn, &gCc, &gFont, margin, margin);
    NNS_G2dTextCanvasInit(&gITxn, &gCc, &gInfoFont, margin, margin);

    // �O���t�C���f�b�N�X�ő�l���擾
    giMax = (int)NNS_G2D_FONT_MAX_GLYPH_INDEX(&gFont) - 1;

    // �O���t���X�g�\�������v�Z
    {
        const int cw = NNS_G2dFontGetMaxCharWidth(&gFont);

        giLine = ((SCREEN_WIDTH/2) / (cw + CHAR_SPACE)) * 2 + 1;
    }
}



/*---------------------------------------------------------------------------*
  Name:         UpdateScreenScale

  Description:  �O���t�C���[�W�g�嗦�̕ύX�K�p�B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void UpdateScreenScale(void)
{
    const int cw = NNS_G2dFontGetMaxCharWidth(&gFont);
    const int ch = NNS_G2dFontGetHeight(&gFont);
    const int ew = cw + MARGIN * 2;
    const int eh = ch + MARGIN * 2;
    const int info_height = MARGIN + ch + MARGIN;
    const int x = (SCREEN_WIDTH - ew * scale + 1) / 2;
    const int y = (SCREEN_HEIGHT - eh * scale + 1 - info_height) / 2;
    MtxFx22 mtx;

    // scale �{�Ɋg�傷��A�t�B���s��
    mtx._00 = FX32_ONE / scale;
    mtx._01 = 0;
    mtx._10 = 0;
    mtx._11 = FX32_ONE / scale;


    // BG2 �ɃA�t�B���s���ݒ�
    G2_SetBG2Affine(&mtx, 0, 0, -x, -y);
}



/*---------------------------------------------------------------------------*
  Name:         InitDoubleScaleScreen

  Description:  �O���t�C���[�W�g��\���p Canvas ������

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitDoubleScaleScreen(void)
{
    const static int margin = 1;

    // BG 2 ���A�t�B��BG�ɐݒ�
    G2_SetBG2ControlAffine(
        GX_BG_SCRSIZE_AFFINE_256x256,
        GX_BG_AREAOVER_XLU,
        GX_BG_SCRBASE_0x0800,
        GX_BG_CHARBASE_0x10000
    );

    UpdateScreenScale();

    // BG2 ������
    CMN_SetPlaneVisible(GX_PLANEMASK_BG2);


    // CharCanvas������
    NNS_G2dCharCanvasInitForBG(
        &gDCc,
        &gBG2OffScreen,
        BG2_CANVAS_WIDTH,
        BG2_CANVAS_HEIGHT,
        NNS_G2D_CHARA_COLORMODE_256
    );

    // �X�N���[���ݒ�
    NNS_G2dMapScrToCharAffine(
        &( ((GXScrAffine32x32*)G2_GetBG2ScrPtr())->scr[BG2_CANVAS_TOP][BG2_CANVAS_LEFT] ),
        BG2_CANVAS_WIDTH,
        BG2_CANVAS_HEIGHT,
        NNS_G2D_AFFINE_BG_WIDTH_256,
        BG2_CANVAS_OFFSET
    );

    // CharCanvas�N���A
    NNS_G2dCharCanvasClear(&gDCc, 0);

    // TextCanvas������
    NNS_G2dTextCanvasInit(&gDTxn, &gDCc, &gFont, margin, margin);
}



/*---------------------------------------------------------------------------*
  Name:         UpdateDisplay

  Description:  �`�揈�����s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void UpdateDisplay(void)
{
    const u8 barColor       = 1;    // �ΐ� GX_RGB(9, 27, 17)
    const u8 borderColor    = 2;    // ��F GX_RGB(27, 17, 9)
    const u8 bgColor        = 3;    // ���i���������̐擪�j
    const u8 txtColor       = 4;    // ����������2�Ԗڈȍ~

    const int ch = NNS_G2dFontGetHeight(&gFont);
    const int cw = NNS_G2dFontGetMaxCharWidth(&gFont);
    const int dw = cw + CHAR_SPACE;

    const int l_px = (SCREEN_WIDTH - (giLine * dw - CHAR_SPACE)) / 2;
    const int l_py = SCREEN_HEIGHT - (ch + MARGIN);


    {
        // �g��\��
        {
            NNSG2dGlyph g;

            NNS_G2dFontGetGlyphFromIndex(&g, &gFont, (u16)(giCenter));

            {
                // �O���t�������ɂ���悤�ɒ���
            	const int cWidth = GetCharWidth(g.pWidths);
                const int e_pxc = MARGIN + g.pWidths->left + (cw - cWidth) / 2;
                const int bar_x = e_pxc - g.pWidths->left;
                const int bar_y = MARGIN + ch + 1;

                ClearAreaSafe(&gDCc, bgColor, 0, 0, (cw + MARGIN * 2), (ch + MARGIN * 2) );
                NNS_G2dCharCanvasDrawGlyph(&gDCc, &gFont, e_pxc, MARGIN, txtColor, &g);

                // NNS_G2dCharCanvasClearArea ���g���ĕ�����`��
                ClearAreaSafe(&gDCc, barColor, bar_x, bar_y, cWidth, 1);
            }
        }

        // �O���t���X�g�\��
        {
            int i;

            ClearAreaSafe(&gCc, bgColor, 0, (u16)(l_py - MARGIN), SCREEN_WIDTH, (u16)(ch + MARGIN * 2));

            // giCenter �𒆐S�Ƃ��� giLine �̃O���t��\��
            for( i = 0; i < giLine; ++i )
            {
                NNSG2dGlyph g;
                int gi = giCenter + i - giLine / 2;

                LIMIT_GLYPH_INDEX(gi);

                NNS_G2dFontGetGlyphFromIndex(&g, &gFont, (u16)(gi));

                {
                    // �␳x = �s�I�t�Z�b�g + �����ʒu + ���X�y�[�X + �t�H���g���������΃I�t�Z�b�g
                    const int pxc = l_px + dw * i + g.pWidths->left + (cw - GetCharWidth(g.pWidths)) / 2;

                    NNS_G2dCharCanvasDrawGlyph(&gCc, &gFont, pxc, l_py, txtColor, &g);
                }
            }

            // NNS_G2dCharCanvasClearArea ���g���Ęg��`��
            {
                int border_x = l_px + dw * (giLine / 2) - 2;
                int border_y = l_py - 2;
                int border_w = cw + 2;
                int border_h = ch + 2;

                ClearAreaSafe(&gCc, borderColor, border_x,                  border_y,                   border_w + 2,   1);
                ClearAreaSafe(&gCc, borderColor, border_x,                  border_y + border_h + 1,    border_w + 2,   1);

                ClearAreaSafe(&gCc, borderColor, border_x,                  border_y + 1,               1,              border_h);
                ClearAreaSafe(&gCc, borderColor, border_x + border_w + 1,   border_y + 1,               1,              border_h);
            }
        }
    }

    // VRAM�]���^�X�N��o�^
    {
        (void)NNS_GfdRegisterNewVramTransferTask(
            NNS_GFD_DST_2D_BG1_CHAR_MAIN,
            sizeof(GXCharFmt256) * BG1_CANVAS_OFFSET,
            &gBG1OffScreen,
            sizeof(GXCharFmt256) * BG1_CANVAS_WIDTH * BG1_CANVAS_HEIGHT );

        (void)NNS_GfdRegisterNewVramTransferTask(
            NNS_GFD_DST_2D_BG2_CHAR_MAIN,
            sizeof(GXCharFmt256) * BG2_CANVAS_OFFSET,
            &gBG2OffScreen,
            sizeof(GXCharFmt256) * BG2_CANVAS_WIDTH * BG2_CANVAS_HEIGHT );
    }
}



/*---------------------------------------------------------------------------*
  Name:         PrintSampleInfo

  Description:  �T���v���̏�������ʂɕ\�����܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void PrintSampleInfo(void)
{
    const static char* encodingName[] =
    {
        "UTF-8",
        "UTF-16",
        "Shift_JIS",
        "CP1252",
    };
    NNSG2dGlyph g;
    int right;

    NNS_G2dFontGetGlyphFromIndex(&g, &gFont, (u16)(giCenter));
	right = GetCharWidth(g.pWidths) - (g.pWidths->left + g.pWidths->glyphWidth);

    DTX_PrintLine(
        "Simple Font Viewer Sample\n"
        "operation\n"
        "  left, right  move one character\n"
        "  up, down     move one line\n"
        "  X            change scale\n"
    );

    DTX_PrintLine("font file name:  %s", fontName);
    DTX_PrintLine("font encoding:   %s", encodingName[NNSi_G2dFontGetEncoding(&gFont)]);
    DTX_PrintLine("font cell size:  %5d x %5d", NNS_G2dFontGetCellWidth(&gFont), NNS_G2dFontGetCellHeight(&gFont));
    DTX_PrintLine("font bpp:        %5d", NNS_G2dFontGetBpp(&gFont));
    DTX_PrintLine("font baseline:   %5d", NNS_G2dFontGetBaselinePos(&gFont));
    DTX_PrintLine("font linefeed:   %5d", NNS_G2dFontGetLineFeed(&gFont));
    DTX_PrintLine("font alter char: %5d", NNS_G2dFontGetAlternateGlyphIndex(&gFont));
    DTX_PrintLine("glyph index/sum: %5d / %5d", giCenter, giMax);
    DTX_PrintLine("glyph widths:     %4d + %3d + %4d = %4d",
        g.pWidths->left, g.pWidths->glyphWidth, right, g.pWidths->charWidth);
    DTX_PrintLine("scale:              x%d", scale);
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  �\���p�̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    InitScreenCommon();
    InitEnumScreen();
    InitDoubleScaleScreen();

    // �I�t�X�N���[���]���p
    NNS_GfdInitVramTransferManager(gTransferTask, ARY_SIZEOF(gTransferTask));

    // �����\���ʒu
    giCenter = 0;
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  ���t���[���̕`�揈�����s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    static int iCenter = -1;

    // �X�V����Ă��鎞�����`�悷��
    if( giCenter != iCenter )
    {
        iCenter = giCenter;
        PrintSampleInfo();
        UpdateDisplay();
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

        // �\��������
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
        static u32 repeat_count = 0;

        CMN_ReadGamePad();

        // �g�嗦�̕ύX
        if( CMN_IsTrigger(PAD_BUTTON_X) )
        {
            scale ++;
            if( scale > 4 )
            {
                scale = 1;
            }
            UpdateScreenScale();
            PrintSampleInfo();
        }

        // �\���O���t�̕ύX
        if( CMN_IsPress(PAD_PLUS_KEY_MASK) )
        {
#define REPEAT(key)     ( (repeat_count >= REPEAT_THRESHOLD) ? CMN_IsPress(key): CMN_IsTrigger(key) )
            if( REPEAT(PAD_KEY_LEFT) )
            {
                giCenter--;
            }
            if( REPEAT(PAD_KEY_RIGHT) )
            {
                giCenter++;
            }
            if( REPEAT(PAD_KEY_UP) )
            {
                giCenter -= giLine;
            }
            if( REPEAT(PAD_KEY_DOWN) )
            {
                giCenter += giLine;
            }

            LIMIT_GLYPH_INDEX(giCenter);

            repeat_count++;
        }
        else
        {
            repeat_count = 0;
        }

        // �`�揈��
        SampleMain();

        CMN_WaitVBlankIntr();

        // ���o�͂̕\��
        DTX_Reflect();

        // �I�t�X�N���[���o�b�t�@��]��
        NNS_GfdDoVramTransfer();
    }
}
