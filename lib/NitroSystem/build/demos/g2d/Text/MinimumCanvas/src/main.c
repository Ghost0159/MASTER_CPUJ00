/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - MinimumCanvas
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �\�����镶����̑傫���ɉ����āA���̕������\�����鎖���o����
//      �ŏ��� CharCanvas ���\�z���A�`����s���܂��B
//      NNS_G2dFontGetTextRect �ɂ���ē���ꂽ������̃T�C�Y��
//      CharCanvas �̃T�C�Y�Ƃ��Ė����ł���ꍇ�́A����ȏォ�ŏ���
//      �L���T�C�Y���v�Z���� CharCanvas ���\�z���܂��B
//
//  ������@
//      A �{�^��    �J���[���[�h��ύX����B
//      B �{�^��    VRAM���[�h��ύX����B
// ============================================================================


#include <nnsys/g2d/g2d_TextCanvas.h>
#include <nnsys/g2d/g2d_CharCanvas.h>
#include <nnsys/misc.h>
#include "g2d_textdemolib.h"


#define ROUNDUP(x, div) (((x) + (div) - 1) & ~((div) - 1))

#define CHARACTER_WIDTH     8       // �L�����N�^�̃s�N�Z���P�ʂł̕�
#define CHARACTER_HEIGHT    8       // �L�����N�^�̃s�N�Z���P�ʂł̍���
#define SCREEN_WIDTH        256     // �X�N���[����
#define SCREEN_HEIGHT       192     // �X�N���[������

#define TEXT_HSPACE         1       // ������`�掞�̕����� (�s�N�Z���P��)
#define TEXT_VSPACE         1       // ������`�掞�̍s��   (�s�N�Z���P��)

#define CHARACTER_OFFSET    1       // �g�p����L�����N�^��̊J�n�ԍ�

#define MAX_OBJ_NUM         128     // OBJ �̍ő吔
#define MAX_CHARA_NAME      1024    // �L�����N�^�l�[���̍ő�l


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

NNSG2dFont              gFont;          // �t�H���g

GXOamColorMode gColorMode       = GX_OAM_COLORMODE_16;
NNSG2dOBJVramMode gVramMode     = NNS_G2D_OBJVRAMMODE_32K;

int gUsingCharas = 0;
int gUsingOam    = 0;

const static NNSG2dChar* sampleText[] =
{
    NNS_G2D_TRANSCODE("This sample program make temporary minimum\n"
                      "CharCanvas to draw a text and draw the\n"
                      "text to it."),
    NNS_G2D_TRANSCODE("sample text"),
    NNS_G2D_TRANSCODE("this text require\n11x3 size."),
    NNS_G2D_TRANSCODE("line 1\n" "line 2\n" "line 3\n" "line 4\n"),
    NNS_G2D_TRANSCODE("A"),
};

//****************************************************************************
// Initialize etc.
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         GetMinValidCanvasSize

  Description:  �^����ꂽ�T�C�Y�ȏ�� CharCanvas �̃T�C�Y�Ƃ��ėL����
                �ŏ��̃T�C�Y���擾���܂��B

  Arguments:    areaWidth:  CharCanvas �̕��i�L�����N�^�P�ʁj
                areaHeight: CharCanvas �̍����i�L�����N�^�P�ʁj
                colorMode:  CharCanvas �̃J���[���[�h
                vramMode:   OBJ VRAM �e��

  Returns:      �^����ꂽ�T�C�Y�ȏ�� CharCanvas �̃T�C�Y�Ƃ��ėL����
                �ŏ��̃T�C�Y��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static NNSG2dTextRect GetMinValidCanvasSize(
    int                 areaWidth,
    int                 areaHeight,
    GXOamColorMode      colorMode,
    NNSG2dOBJVramMode   vramMode
)
{
    NNSG2dTextRect rect;
    NNS_G2D_MIN_ASSERT( areaWidth,  1 );
    NNS_G2D_MIN_ASSERT( areaHeight, 1 );
    NNS_G2D_ASSERT( (colorMode == GX_OAM_COLORMODE_16)
                 || (colorMode == GX_OAM_COLORMODE_256) );
    NNS_G2D_ASSERT( (vramMode == NNS_G2D_OBJVRAMMODE_32K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_64K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_128K)
                 || (vramMode == NNS_G2D_OBJVRAMMODE_256K) );

    rect.width  = areaWidth;
    rect.height = areaHeight;

    if(    (colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_128K)
        || (colorMode != GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K) )
    {
        // �u��Ɓu4�Ŋ������]�肪3�̒l�v�̑g�ݍ��킹�͕s�v�̃P�[�X�B
        // �����ɓ��Ă͂܂�ꍇ�͓K�؂Ȃǂ��炩�Е��� +1 ����΂悢
        if( (rect.width % 2) != 0 && (rect.height % 2) != 0 )
        {
            if( (rect.width % 4) != 1 )
            {
                // width % 4 == 3 && height % 4 == 1,3
                rect.width++;
            }
            else if( (rect.height % 4) != 1 )
            {
                // width % 4 == 1 && height % 4 == 3
                rect.height++;
            }
        }
    }
    else if( colorMode == GX_OAM_COLORMODE_16 && vramMode == NNS_G2D_OBJVRAMMODE_256K )
    {
        // ���ɋ����łȂ���΂Ȃ�Ȃ��B
        rect.width  = ROUNDUP(rect.width,  2);
        rect.height = ROUNDUP(rect.height, 2);
    }

    return rect;
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

  Description:  OAM��S�ĉ�������Z�b�g���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ResetOAM(void)
{
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    gUsingOam = 0;
}



/*---------------------------------------------------------------------------*
  Name:         ResetOBJChr

  Description:  OBJ�L�����N�^��S�ĉ�����܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ResetOBJChr(void)
{
    gUsingCharas = 0;
}



/*---------------------------------------------------------------------------*
  Name:         AllocCharacters

  Description:  �ȈՃL�����N�^�}�l�[�W���B
                �w�肵�����̃L�����N�^���m�ۂ��A�m�ۂ����L�����N�^���
                �擪�L�����N�^�l�[����Ԃ��܂��B

  Arguments:    num:    �m�ۂ���L�����N�^�̐��B

  Returns:      �m�ۂɐ��������ꍇ�͊m�ۂ����L�����N�^���
                �擪�L�����N�^�l�[����Ԃ��܂��B
                ���s�����ꍇ�� -1 ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static int AllocCharacters(
    int num,
    GXOamColorMode colorMode,
    NNSG2dOBJVramMode vramMode )
{
    int ret;
    int realNum = ROUNDUP_DIV(num, (1 << vramMode));

    if( colorMode == GX_OAM_COLORMODE_256 )
    {
        realNum *= 2;
    }

    if( gUsingCharas + realNum > MAX_CHARA_NAME )
    {
        return -1;
    }

    ret = gUsingCharas;
    gUsingCharas += realNum;

    return ret;
}



/*---------------------------------------------------------------------------*
  Name:         AllocOAM

  Description:  �Ȉ� OAM �}�l�[�W���B
                �w�肵������ OAM ���m�ۂ��A�m�ۂ��� OAM ���
                �擪�ւ̃|�C���^��Ԃ��܂�

  Arguments:    num:    �m�ۂ��� OAM �̐��B

  Returns:      �m�ۂɐ��������ꍇ�͊m�ۂ����L�����N�^���
                �擪�ւ̃|�C���^��Ԃ��܂��B
                ���s�����ꍇ�� NULL ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static GXOamAttr* AllocOAM(int num)
{
    GXOamAttr* const pObjBase = (GXOamAttr*)HW_OAM;
    GXOamAttr* ret;

    if( gUsingOam + num > MAX_OBJ_NUM )
    {
        return NULL;
    }

    ret = pObjBase + gUsingOam;
    gUsingOam += num;

    return ret;
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
  Name:         PutText

  Description:  �w�肳�ꂽ�ꏊ�Ɏw�肳�ꂽ�e�L�X�g��\�����܂��B

  Arguments:    x:          �e�L�X�g��\������ʒu
                y:          �e�L�X�g��\������ʒu
                pFont:      �e�L�X�g��`�悷�邽�߂̃t�H���g
                colorMode:  ���C��OBJ�̃J���[���[�h���w�肵�܂��B
                vramMode:   ���C��OBJ��VRAM���[�h���w�肵�܂��B
                color:      �e�L�X�g�̐F�ԍ����w�肵�܂��B
                txt:        �\������e�L�X�g���w�肵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void PutText(
    int                 x,
    int                 y,
    const NNSG2dFont*   pFont,
    GXOamColorMode      colorMode,
    NNSG2dOBJVramMode   vramMode,
    u8                  color,
    const NNSG2dChar*   txt
)
{
    void* const pCharBase = G2_GetOBJCharPtr();
    NNSG2dCharCanvas cc;
    NNSG2dTextCanvas tc;
    NNSG2dTextRect textRect;
    NNSG2dTextRect canvasSize;
    int areaWidth, areaHeight;
    GXOamAttr* pOam;
    int numOBJ;
    int charNo;

    // �e�L�X�g�̈�T�C�Y�����߂܂�
    textRect   = NNS_G2dFontGetTextRect(pFont, TEXT_HSPACE, TEXT_VSPACE, txt);
    areaWidth  = ROUNDUP_DIV(textRect.width,  CHARACTER_WIDTH);
    areaHeight = ROUNDUP_DIV(textRect.height, CHARACTER_HEIGHT);
    canvasSize = GetMinValidCanvasSize(areaWidth, areaHeight, colorMode, vramMode);

    // OAM �� �L�����N�^���m�ۂ��܂�
    numOBJ = NNS_G2dCalcRequiredOBJ1D( canvasSize.width, canvasSize.height );
    charNo = AllocCharacters( canvasSize.width * canvasSize.height, colorMode, vramMode );
    pOam   = AllocOAM( numOBJ );
    NNS_G2D_ASSERT( charNo >= 0 );
    NNS_G2D_POINTER_ASSERT( pOam );

    // CharCanvas �� 1D OBJ �p�� �e�L�X�g�̈�T�C�Y�ŏ�����
    NNS_G2dCharCanvasInitForOBJ1D(
        &cc,
        OffsetCharName(pCharBase, charNo, vramMode),
        canvasSize.width,
        canvasSize.height,
        SDKToNNSColorMode(colorMode)
    );

    // CharCanvas �p�� OBJ ��z�u
    (void)NNS_G2dArrangeOBJ1D(
        pOam,
        canvasSize.width,
        canvasSize.height,
        x, y,
        colorMode,
        charNo,
        vramMode
    );

    // OBJ��ݒ�
    TXT_SetCharCanvasOBJAttrs(
        pOam,                  // OAM�z��ւ̃|�C���^
        numOBJ,                 // �ΏۂƂ���OAM�̐�
        0,                      // priority
        GX_OAM_MODE_NORMAL,     // OBJ���[�h
        FALSE,                  // ���U�C�N
        GX_OAM_EFFECT_NONE,     // OBJ����
        TXT_CPALETTE_MAIN,      // �p���b�g�ԍ�
        0                       // �A�t�B���p�����[�^�C���f�b�N�X
    );

    // TextCanvas ���\�z
    NNS_G2dTextCanvasInit(
        &tc,
        &cc,
        pFont,
        TEXT_HSPACE,
        TEXT_VSPACE
    );

    // Canvas ���N���A���ĕ�����`��
    NNS_G2dCharCanvasClear(
        &cc,
        TXT_COLOR_WHITE
    );
    NNS_G2dTextCanvasDrawText(
        &tc,
        0, 0,
        color,
        TXT_DRAWTEXT_FLAG_DEFAULT,
        txt
    );


    DTX_PrintLine("%3dx%3d  %2dx%2d  %2dx%2d  %4d  %3d",
        textRect.width, textRect.height,
        areaWidth, areaHeight,
        canvasSize.width, canvasSize.height,
        canvasSize.width * canvasSize.height, numOBJ
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
        "Text size dependents Canvas Sample\n"
        "operation\n"
        "  A  change color mode\n"
        "  B  change vram mode\n"
    );
    DTX_PrintLine("color mode:  %s", colorModeString[gColorMode]);
    DTX_PrintLine("vram mode:   %s", vramModeString[gVramMode]);
    DTX_PrintLine(
        "\n"
        "Size                    Using\n"
        "text     canvas adjust  char  OBJ"
    );
}



/*---------------------------------------------------------------------------*
  Name:         SampleDraw

  Description:  �T���v���̕`�揈���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SampleDraw(void)
{
    int i;

    PrintSampleInfo();

    switch( gVramMode )
    {
    case NNS_G2D_OBJVRAMMODE_32K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    case NNS_G2D_OBJVRAMMODE_64K:  GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_64K);   break;
    case NNS_G2D_OBJVRAMMODE_128K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);  break;
    case NNS_G2D_OBJVRAMMODE_256K: GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_256K);  break;
    default:                       GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);   break;
    }

    ResetOAM();
    ResetOBJChr();

    for( i = 0; i < ARY_SIZEOF(sampleText); ++i )
    {
        int x = (int)(SCREEN_WIDTH / ARY_SIZEOF(sampleText)) * i;
        int y = (int)(SCREEN_HEIGHT / ARY_SIZEOF(sampleText)) * i;
        u8 color = (u8)(TXT_COLOR_RED + i);

        PutText(x, y, &gFont, gColorMode, gVramMode, color, sampleText[i]);
    }
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

    InitScreen();
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
    if( HandleInput() )
    {
        SampleDraw();
        DC_StoreAll();
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

