/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - OBJ1D
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      1D�}�b�s���OOBJ��ł̕����`��T���v���B
//      CharCanvas�̃T�C�Y�ɉ����ĕK�v��OBJ����\�����܂��B
//      OBJ1D �ł̓J���[���[�h�AVRAM���[�h�ƃL�����o�X�̕ӂ̒�����
//      �g�ݍ��킹�ɂ���Ă͐������������\������Ȃ����Ƃɒ��ӂ��Ă��������B
//        �Q�ƁFNNS_G2dCharCanvasInitForOBJ1D() API���t�@�����X
//
//  ������@
//      A �{�^��    �J���[���[�h��ύX����B
//      B �{�^��    VRAM���[�h��ύX����B
//      �\���{�^��  CharCanvas�̃T�C�Y��ύX����B
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include <nnsys/misc.h>
#include "g2d_textdemolib.h"


#define CANVAS_WIDTH        28      // �����`���̕�    (�L�����N�^�P��)
#define CANVAS_HEIGHT       10      // �����`���̍���  (�L�����N�^�P��)
#define CANVAS_LEFT         13      // �����`���̈ʒuX (�s�N�Z���P��)
#define CANVAS_TOP          17      // �����`���̈ʒuY (�s�N�Z���P��)

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̐擪�L�����N�^�l�[��

#define MAX_OBJ_NUM         128
#define REPEAT_THRESHOLD    22


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont              gFont;          // �t�H���g
NNSG2dCharCanvas        gCanvas;        // CharCanvas
NNSG2dTextCanvas        gTextCanvas;    // TextCanvas

const char* gErrMsg         = NULL;
int gCanvasWidth            = CANVAS_WIDTH;
int gCanvasHeight           = CANVAS_HEIGHT;
GXOamColorMode gColorMode   = GX_OAM_COLORMODE_16;
NNSG2dOBJVramMode gVramMode = NNS_G2D_OBJVRAMMODE_32K;

//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         IsValidCharCanvasSize

  Description:  CharCanvas �̃T�C�Y������ɕ\���\�Ȃ��̂��ǂ������肵�܂��B
                �������A�T�C�Y���߂͔��肵�܂���B

  Arguments:    areaWidth:  CharCanvas �̕��i�L�����N�^�P�ʁj
                areaHeight: CharCanvas �̍����i�L�����N�^�P�ʁj
                colorMode:  CharCanvas �̃J���[���[�h
                vramMode:   OBJ VRAM �e��

  Returns:      ����ɕ\���\�Ȃ� TRUE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL IsValidCharCanvasSize(
    int                 areaWidth,
    int                 areaHeight,
    GXOamColorMode      colorMode,
    NNSG2dOBJVramMode   vramMode
)
{
    NNS_G2D_ASSERT( (colorMode == GX_OAM_COLORMODE_16)
                 || (colorMode == GX_OAM_COLORMODE_256) );
    NNS_G2D_ASSERT( (vramMode == NNS_G2D_OBJVRAMMODE_32K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_64K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_128K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_256K) );

    // �T�C�Y�͐��łȂ���΂Ȃ�Ȃ�
    if( areaWidth <= 0 || areaHeight <= 0 )
    {
        return FALSE;
    }

    if(    (colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_128K)
        || (colorMode != GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K) )
    {
        // �u��Ɓu4�Ŋ������]�肪3�̒l�v�̑g�ݍ��킹�͕s�v�̃P�[�X�B
        // ����́u�ǂ��炩�������܂��͋���4�Ŋ��������܂肪3�łȂ��Ȃ�v�Ɠ��l�Ȃ̂�
        // ����Ŕ��肷��B
        return ((areaWidth  % 2) == 0)
            || ((areaHeight % 2) == 0)
            || ((areaWidth  % 4) != 3 && (areaHeight % 4) != 3);
    }
    else if( colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K )
    {
        // ���ɋ����łȂ���΂Ȃ�Ȃ��B
        return (areaWidth % 2) == 0 && (areaHeight % 2) == 0;
    }
    else
    {
        return TRUE;
    }
}




/*---------------------------------------------------------------------------*
  Name:         OffsetCharName

  Description:  1D �}�b�s���OOBJ�ŃL�����N�^�l�[�����w�������A�h���X���擾���܂��B

  Arguments:    pCharBase:  OBJ�̃L�����N�^�x�[�X
                charName:   �Ώۂ̃L�����N�^�l�[��
                vramMode:   VRAM ���[�h

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void* OffsetCharName(
    void*               pCharBase,
    int                 charName,
    NNSG2dOBJVramMode   vramMode )
{
    u32 addr = (u32)pCharBase;
    addr += sizeof(GXCharFmt16) * (charName << vramMode);
    return (void*)addr;
}



/*---------------------------------------------------------------------------*
  Name:         SDKToNNSColorMode

  Description:  NITRO-SDK �̃J���[���[�h�񋓎q���� NITRO-System ��
                �J���[���[�h�񋓎q�ɕϊ����܂��B

  Arguments:    sdk:    NITRO-SDK�̃J���[���[�h�񋓎q

  Returns:      sdk �ɑΉ����� NITRO-System �̃J���[���[�h�񋓎q��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static inline NNSG2dCharaColorMode SDKToNNSColorMode(GXOamColorMode sdk)
{
    static const NNSG2dCharaColorMode nns[] =
        { NNS_G2D_CHARA_COLORMODE_16, NNS_G2D_CHARA_COLORMODE_256 };

    NNS_MINMAX_ASSERT(sdk, 0, 1);
    return nns[sdk];
}



/*---------------------------------------------------------------------------*
  Name:         ResetOAM

  Description:  OAM�����Z�b�g���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ResetOAM(void)
{
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
}



/*---------------------------------------------------------------------------*
  Name:         HandleInput

  Description:  ���͂�ǂݎ��܂��B

  Arguments:    �Ȃ��B

  Returns:      ���͂ɂ���ē�����Ԃ��ύX���ꂽ��TRUE�B
 *---------------------------------------------------------------------------*/
static BOOL HandleInput(void)
{
    BOOL bChanged = FALSE;

#define REPEAT(key)   ((repeat_count >= REPEAT_THRESHOLD) ? CMN_IsPress(key): CMN_IsTrigger(key))
    {
        static int repeat_count = 0;
        int old_cw = gCanvasWidth;
        int old_ch = gCanvasHeight;

        if( CMN_IsTrigger(PAD_BUTTON_A) )
        {
            gColorMode = (gColorMode != GX_OAM_COLORMODE_16) ?
                            GX_OAM_COLORMODE_16:
                            GX_OAM_COLORMODE_256;
            bChanged = TRUE;
        }
        if( CMN_IsTrigger(PAD_BUTTON_B) )
        {
            gVramMode++;
            if( gVramMode > NNS_G2D_OBJVRAMMODE_256K )
            {
                gVramMode = NNS_G2D_OBJVRAMMODE_32K;
            }
            bChanged = TRUE;
        }
        if( REPEAT(PAD_KEY_UP) )
        {
            gCanvasHeight--;
        }
        if( REPEAT(PAD_KEY_DOWN) )
        {
            gCanvasHeight++;
        }
        if( REPEAT(PAD_KEY_LEFT) )
        {
            gCanvasWidth--;
        }
        if( REPEAT(PAD_KEY_RIGHT) )
        {
            gCanvasWidth++;
        }
        if( gCanvasWidth < 1 )
        {
            gCanvasWidth = 1;
        }
        if( gCanvasHeight < 1 )
        {
            gCanvasHeight = 1;
        }
        if( MAX_OBJ_NUM < NNS_G2dCalcRequiredOBJ1D(gCanvasWidth, gCanvasHeight) )
        {
            // �g���� OBJ �̍ő吔�𒴂���
            gCanvasWidth = old_cw;
            gCanvasHeight = old_ch;
            gErrMsg = "OBJ OVERFLOW";
            bChanged = TRUE;
        }
        {
            const int maxChr = ((1024 << gVramMode) >> gColorMode) - CHARACTER_OFFSET;

            // 
            if( maxChr < (gCanvasWidth + 1) * (gCanvasHeight + 1) )
            {
                // �g����L�����N�^�̍ő吔�𒴂���
                gCanvasWidth = old_cw;
                gCanvasHeight = old_ch;
                gErrMsg = "Character OVERFLOW";
                bChanged = TRUE;
            }
        }

        if( gCanvasWidth != old_cw || gCanvasHeight != old_ch )
        {
            bChanged = TRUE;
        }
        if( CMN_IsPress(PAD_PLUS_KEY_MASK) )
        {
            repeat_count++;
        }
        else
        {
            repeat_count = 0;
        }
    }

    return bChanged;
}



/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  OBJ�\���̏����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitScreen(void)
{
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
    void*      const pCharBase = G2_GetOBJCharPtr();
    GXOamAttr* const pObjBase = (GXOamAttr*)HW_OAM;
    const int cOffset = CHARACTER_OFFSET;
    int nObjs;


    // �}�b�s���O���[�h�ݒ�
    switch( gVramMode )
    {
    case NNS_G2D_OBJVRAMMODE_32K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    case NNS_G2D_OBJVRAMMODE_64K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_64K);   break;
    case NNS_G2D_OBJVRAMMODE_128K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);  break;
    case NNS_G2D_OBJVRAMMODE_256K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_256K);  break;
    default:                       GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    }


    // CharCanvas �̏�����
    NNS_G2dCharCanvasInitForOBJ1D(
        &gCanvas,
        OffsetCharName(pCharBase, cOffset, gVramMode),
            // VRAM�Q�Ɖ\�T�C�Y�ɂ���ăL�����N�^�l�[�����E���قȂ邽�ߕ␳����

        gCanvasWidth,
        gCanvasHeight,
        SDKToNNSColorMode(gColorMode)
    );

    // Canvas�N���A
    NNS_G2dCharCanvasClear(&gCanvas, TXT_COLOR_WHITE);

    // OBJ��z�u
    nObjs = NNS_G2dArrangeOBJ1D(
        pObjBase,
        gCanvasWidth,
        gCanvasHeight,
        CANVAS_LEFT,
        CANVAS_WIDTH,
        gColorMode,
        cOffset,
        gVramMode
    );

    // OBJ��ݒ�
    TXT_SetCharCanvasOBJAttrs(
        pObjBase,               // OAM�z��ւ̃|�C���^
        nObjs,                  // �ΏۂƂ���OAM�̐�
        0,                      // priority
        GX_OAM_MODE_NORMAL,     // OBJ���[�h
        FALSE,                  // ���U�C�N
        GX_OAM_EFFECT_NONE,     // OBJ����
        TXT_CPALETTE_MAIN,      // �p���b�g�ԍ�
        0                       // �A�t�B���p�����[�^�C���f�b�N�X
    );

    // TextCanvas�̏�����
    NNS_G2dTextCanvasInit(
        &gTextCanvas,
        &gCanvas,
        &gFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // �g�p����OBJ��
    DTX_PrintLine("using OBJ:   %3d", nObjs);

    if( ! IsValidCharCanvasSize(gCanvasWidth, gCanvasHeight, gColorMode, gVramMode) )
    {
        gErrMsg = "INVALID CharCanvas size";
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
    // �����`��
    NNS_G2dTextCanvasDrawText(&gTextCanvas, 1, 1, TXT_COLOR_BLACK, TXT_DRAWTEXT_FLAG_DEFAULT,
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\n"
        "bcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789a\n"
        "cdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ab\n"
        "defghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abc\n"
        "efghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcd\n"
        "fghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcde\n"
        "ghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdef\n"
        "hijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefg\n"
        "ijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefgh\n"
        "jklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghi\n"
        "klmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghij\n"
        "lmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijk\n"
        "mnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijkl\n"
        "nopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklm\n"
        "opqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmn\n"
        "pqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmno\n"
        "qrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnop\n"
        "rstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopq\n"
        "stuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqr\n"
        "tuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrs\n"
        "uvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrst\n"
        "vwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstu\n"
        "wxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuv\n"
        "xyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvw\n"
        "yzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwx\n"
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
    static const char* colorModeString[] =
    {
        "GX_OAM_COLORMODE_16",
        "GX_OAM_COLORMODE_256"
    };
    static const char* vramModeString[] =
    {
        "NNS_G2D_OBJVRAMMODE_32K",
        "NNS_G2D_OBJVRAMMODE_64K",
        "NNS_G2D_OBJVRAMMODE_128K",
        "NNS_G2D_OBJVRAMMODE_256K"
    };

    DTX_PrintLine(
        "1D mapping OBJ CharCanvas Sample\n"
        "operation\n"
        "  A  change color mode\n"
        "  B  change vram mode\n"
        "  +  change canvas size\n"
    );
    DTX_PrintLine("color mode:  %s", colorModeString[gColorMode]);
    DTX_PrintLine("vram mode:   %s", vramModeString[gVramMode]);
    DTX_PrintLine("canvas size: %2d x %2d", gCanvasWidth, gCanvasHeight);
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

    PrintSampleInfo();

    InitScreen();
    InitCanvas();
    SampleDraw();
}



/*---------------------------------------------------------------------------*
  Name:         SampleMain

  Description:  �T���v���̃��C�������ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleMain(void)
{
    gErrMsg = NULL;

    if( HandleInput() )
    {
        PrintSampleInfo();

        ResetOAM();
        InitCanvas();

        SampleDraw();
        DC_StoreAll();

        if( gErrMsg != NULL )
        {
            DTX_PrintLine("\n\n\n\n     !!! %s !!!", gErrMsg);
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

        // ���o�͂̕\��
        DTX_Reflect();
    }
}

