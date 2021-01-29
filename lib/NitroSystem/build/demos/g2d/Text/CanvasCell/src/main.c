/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - CanvasCell
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
//      1D�}�b�s���OOBJ�p�ɍ\�z���ꂽCharCanvas���Z���Ƃ��ĕ\�����܂�
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include <nnsys/g2d/g2d_Renderer.h>
#include "g2d_textdemolib.h"

#define SCREEN_WIDTH        256
#define SCREEN_HEIGHT       192

#define CANVAS_WIDTH        6       // �����`���̕�    (�L�����N�^�P��)
#define CANVAS_HEIGHT       4       // �����`���̍���  (�L�����N�^�P��)
#define CANVAS_LEFT         13      // �����`���̈ʒuX (�s�N�Z���P��)
#define CANVAS_TOP          17      // �����`���̈ʒuY (�s�N�Z���P��)

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̐擪�L�����N�^�l�[��
                                    //

#define MAX_OBJ_NUM         128
#define REPEAT_THRESHOLD    22

#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8

#define NUM_OF_OAM          128                 // OAM �}�l�[�W���Ɋ��蓖�Ă� OAM �̐�


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont                  gFont;          // �t�H���g
NNSG2dCharCanvas            gCanvas;        // CharCanvas
NNSG2dTextCanvas            gTextCanvas;    // TextCanvas

NNSG2dRendererInstance      gRenderer;
NNSG2dRenderSurface         gSurface;
NNSG2dImageProxy            gImageProxy;
NNSG2dImagePaletteProxy     gPaletteProxy;
NNSG2dOamManagerInstance    gOamManager;

NNSG2dCellData*             gpCanvasCell;



//****************************************************************************
// �����_���֘A�̏���
//****************************************************************************
/*---------------------------------------------------------------------------*
  Name:         CallBackAddOam

  Description:  NNS_G2dDraw* �֐������� OAM ��ǉ����邽�߂ɌĂ΂��֐��ł��B

  Arguments:    pOam:           �ǉ����ׂ� OAM �ւ̃|�C���^�B
                affineIndex:    ���� OAM ���g�p���� Affine �C���f�b�N�X�B
                                32 �ȏ�̒l�ł���\��������܂��B
                                Affine �𖢎g�p�̏ꍇ��
                                NNS_G2D_OAM_AFFINE_IDX_NONE ���w�肵�܂��B
                bDoubleAffine:  �{�pAffine���[�h���ǂ����B

  Returns:      ���������ꍇ�� TRUE�A�����łȂ���� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL CallBackAddOam( const GXOamAttr* pOam, u16 affineIndex, BOOL bDoubleAffine )
{
#pragma unused( bDoubleAffine )
    SDK_NULL_ASSERT( pOam );

    return NNS_G2dEntryOamManagerOamWithAffineIdx( &gOamManager, pOam, affineIndex );
}

/*---------------------------------------------------------------------------*
  Name:         CallBackAddAffine

  Description:  NNS_G2dDraw* �֐������� Affine �p�����[�^ ��ǉ����邽�߂�
                �Ă΂��֐��ł��B

  Arguments:    mtx:        �ǉ����ׂ� Affine �ϊ��s��ւ̃|�C���^

  Returns:      �ǉ����� Affine �p�����[�^���Q�Ƃ��邽�߂� AffineIndex ��
                �Ԃ��܂��B�B
 *---------------------------------------------------------------------------*/
static u16 CallBackAddAffine( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    return NNS_G2dEntryOamManagerAffine( &gOamManager, mtx );
}


/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM �}�l�[�W���V�X�e���̏������ƁA1��OAM�}�l�[�W��
                ���̂̏��������s���܂��B

  Arguments:    pOamManager:    ���������� OAM �}�l�[�W���ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance*   pOamManager)
{
    BOOL result;
    SDK_NULL_ASSERT( pOamManager );

    NNS_G2dInitOamManagerModule();

    result = NNS_G2dGetNewOamManagerInstance(
                pOamManager,
                0, NUM_OF_OAM,
                0, NUM_OF_OAM/4,
                NNS_G2D_OAMTYPE_MAIN
    );
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         InitRenderer

  Description:  Renderer �� Surface �����������܂��B

  Arguments:    pRenderer:  ���������� Renderer �ւ̃|�C���^�B
                pSurface:   ���������� Surface �ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitRenderer(
                NNSG2dRendererInstance*     pRenderer,
                NNSG2dRenderSurface*        pSurface,
                NNSG2dImageProxy*           pImageProxy,
                NNSG2dImagePaletteProxy*    pPaletteProxy
)
{
    NNSG2dViewRect* pRect;

    SDK_NULL_ASSERT( pImageProxy );
    SDK_NULL_ASSERT( pPaletteProxy );
    SDK_NULL_ASSERT( pRenderer );
    SDK_NULL_ASSERT( pSurface );

    //---- ImageProxy ���_�~�[�A�h���X�ŏ�����
    NNS_G2dInitImageProxy( pImageProxy );
    NNS_G2dSetImageLocation( pImageProxy, NNS_G2D_VRAM_TYPE_2DMAIN, NULL );

    //---- PaletteProxy �_�~�[�A�h���X�ŏ�����
    NNS_G2dInitImagePaletteProxy( pPaletteProxy );
    NNS_G2dSetImagePaletteLocation( pPaletteProxy, NNS_G2D_VRAM_TYPE_2DMAIN, NULL );

    //---- �T�[�t�F�X��������
    NNS_G2dInitRenderSurface( pSurface );
    pRect = &(pSurface->viewRect);

    pRect->posTopLeft.x = FX32_ONE * 0;
    pRect->posTopLeft.y = FX32_ONE * 0;
    pRect->sizeView.x   = FX32_ONE * SCREEN_WIDTH;
    pRect->sizeView.y   = FX32_ONE * SCREEN_HEIGHT;

    pSurface->pFuncOamRegister          = CallBackAddOam;
    pSurface->pFuncOamAffineRegister    = CallBackAddAffine;
    pSurface->type                      = NNS_G2D_SURFACETYPE_MAIN2D;

    //---- �����_����������
    NNS_G2dInitRenderer( pRenderer );
    NNS_G2dAddRendererTargetSurface( pRenderer, pSurface );
    NNS_G2dSetRendererImageProxy( pRenderer, pImageProxy, pPaletteProxy );
}



//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  OBJ�\���̏����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitScreen(void)
{
    // �}�b�s���O���[�h�ݒ� 1D 256 KB
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_256K);

    // OBJ ������
    CMN_SetPlaneVisible( GX_PLANEMASK_OBJ );

    // �J���[�p���b�g��ݒ�
    GX_LoadOBJPltt(TXTColorPalette, 0, sizeof(TXTColorPalette));
}



/*---------------------------------------------------------------------------*
  Name:         InitCanvas

  Description:  ������`��̏����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitCanvas(void)
{
    GXCharFmt16* const pCharBase = (GXCharFmt16*)G2_GetOBJCharPtr();
    GXOamAttr* const pObjBase = (GXOamAttr*)HW_OAM;
    const int cOffset = CHARACTER_OFFSET;
    const NNSG2dOBJVramMode vramMode = NNS_G2D_OBJVRAMMODE_256K;

    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForOBJ1D(
        &gCanvas,
        pCharBase + (cOffset << vramMode),
            // VRAM�Q�Ɖ\�T�C�Y�ɂ���ăL�����N�^�l�[�����E���قȂ邽�ߕ␳����

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

    // �Z�����\�z
    {
        //---- ��]���S�� CharCanvas �̒��S�ɂ���
        const int x                 = CANVAS_WIDTH  * CHARACTER_WIDTH  / 2;
        const int y                 = CANVAS_HEIGHT * CHARACTER_HEIGHT / 2;
        const int priority          = 0;
        const GXOamMode mode        = GX_OAM_MODE_NORMAL;
        const BOOL mosaic           = FALSE;
        const GXOamEffect effect    = GX_OAM_EFFECT_AFFINE_DOUBLE;
        const GXOamColorMode color  = GX_OAM_COLORMODE_16;
        const int cParam            = TXT_CPALETTE_MAIN;
        const BOOL makeBR           = FALSE;

        const size_t szByte         = NNS_G2dCharCanvasCalcCellDataSize1D(&gCanvas, makeBR);

        //---- �Z���f�[�^�p�������m��
        gpCanvasCell = (NNSG2dCellData*)TXT_Alloc(szByte);
        SDK_NULL_ASSERT( gpCanvasCell );

        NNS_G2dCharCanvasMakeCell1D(
            gpCanvasCell,
            &gCanvas,
            x,
            y,
            priority,
            mode,
            mosaic,
            effect,
            color,
            cOffset,
            cParam,
            vramMode,
            makeBR
        );
    }
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  �T���v���̕`�揈���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleDraw(void)
{
    // Canvas�N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_NULL);

    // �����`��
    NNS_G2dTextCanvasDrawText(&gTextCanvas, 1, 1, TXT_COLOR_BLACK, TXT_DRAWTEXT_FLAG_DEFAULT,
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
        "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz\n"
    );
}



/*---------------------------------------------------------------------------*
  Name:         PrintSampleInfo

  Description:  �T���v���̏���������ʂɏo�͂��܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void PrintSampleInfo(void)
{
    DTX_PrintLine(
        "1D mapping OBJ CharCanvas Cell Sample\n"
        "operation\n"
        "  (none)\n"
    );
    DTX_PrintLine("canvas size character:  %3d x %3d", CANVAS_WIDTH, CANVAS_HEIGHT);
    DTX_PrintLine("canvas size pixel:      %3d x %3d", CANVAS_WIDTH * 8, CANVAS_HEIGHT * 8);
    DTX_PrintLine("bounding sphere radius: %3d", NNS_G2dGetCellBoundingSphereR(gpCanvasCell));
}



/*---------------------------------------------------------------------------*
  Name:         SampleInit

  Description:  �T���v���̏����������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleInit(void)
{
    // �t�H���g��ǂݍ��݂܂�
    TXT_LoadFont( &gFont, TXT_FONTRESOURCE_NAME );

    InitOamManager( &gOamManager );
    InitRenderer( &gRenderer, &gSurface, &gImageProxy, &gPaletteProxy );

    InitScreen();
    InitCanvas();

    SampleDraw();

    PrintSampleInfo();
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    static fx32 x       = SCREEN_WIDTH /2 * FX32_ONE;
    static fx32 y       = SCREEN_HEIGHT/2 * FX32_ONE;
    static fx32 scale   = FX32_ONE;
    static u16  rot     = 0;
    static fx32 vx      = FX32_ONE;
    static fx32 vy      = FX32_ONE;
    static fx32 vs      = FX32_ONE/120;
    static u16  vr      = 0x10000/0xFF;

    //---- �Z���̕\���p�����[�^���X�V���܂�
    {
        const int bsr = NNS_G2dGetCellBoundingSphereR( gpCanvasCell );

        x       += vx;
        y       += vy;
//      scale   += vs;
        rot     += vr;

        //---- �f�B�X�v���C�[�Ŕ��˂���
        if( (x < bsr * scale) || (SCREEN_WIDTH * FX32_ONE - bsr * scale < x) )
        {
            vx *= -1;
        }
        if( (y < bsr * scale) || (SCREEN_HEIGHT * FX32_ONE - bsr * scale < y) )
        {
            vy *= -1;
        }
        if( (scale < FX32_ONE/2) || (FX32_ONE * 2 < scale) )
        {
            vs *= -1;
        }
    }

    //---- CharCanvas �Z����`�悵�܂�
    NNS_G2dBeginRendering( &gRenderer );
    {
        NNS_G2dPushMtx();
        {
            NNS_G2dTranslate( x, y, 0 );
            NNS_G2dRotZ( FX_SinIdx(rot), FX_CosIdx(rot) );
            NNS_G2dScale( scale, scale, FX32_ONE );

            NNS_G2dDrawCell( gpCanvasCell );
        }
        NNS_G2dPopMtx();
    }
    NNS_G2dEndRendering();
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

        // �T���v���̃��C������
        SampleMain();

        CMN_WaitVBlankIntr();

        // �Z���f�[�^�̏����o��
        NNS_G2dApplyOamManagerToHW( &gOamManager );
        NNS_G2dResetOamManagerBuffer( &gOamManager );

        // ���o�͂̕\��
        DTX_Reflect();
    }
}

