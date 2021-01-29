/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - SoftwareSprite2
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      ��]����\�t�g�E�F�A�X�v���C�g���
//      �e�N�X�`��UV�l�ƃ��W�����[�V�����J���[�𓮓I�ɕύX����B
//
//  ������@
//      �\���L�[�㉺    RGB�I��
//      �\���L�[���E    RGB�l�ύX�i���W�����[�V�����J���[��ύX���܂��j
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define SCREEN_WIDTH        256                     // ��ʕ�
#define SCREEN_HEIGHT       192                     // ��ʍ���

#define SPRITE_ALPHA_MAX    31                      // ���l�̍ő�l


// �X�v���C�g�̑傫���i�����`�j
#define SPRITE_SIZE         128

// �J���[�o�[�̑傫��
#define BAR_HEIGHT          6
#define BAR_WIDTH_UNIT      6

// �J���[�o�[�̈ʒu�i x ���W�j
#define BAR_POS_RX          45
#define BAR_POS_GX          BAR_POS_RX
#define BAR_POS_BX          BAR_POS_RX

// �J���[�o�[�̈ʒu�i y ���W�j
#define LINE_HEIGHT         8
#define LINE_MARGIN         ((LINE_HEIGHT - BAR_HEIGHT) / 2)
#define BAR_POS_RY          (BAR_POS_GY - LINE_HEIGHT)
#define BAR_POS_GY          (BAR_POS_BY - LINE_HEIGHT)
#define BAR_POS_BY          (SCREEN_HEIGHT - LINE_HEIGHT + LINE_MARGIN)

// �F
#define COLOR_RED           GX_RGB(31, 0, 0)
#define COLOR_GREEN         GX_RGB(0, 31, 0)
#define COLOR_BLUE          GX_RGB(0, 0, 31)

// UV �l�̕ω����x
#define MOPH_SPEED1         -(FX32_ONE >> 3)
#define MOPH_SPEED2         -(FX32_ONE >> 2)
#define MOPH_SPEED3         (FX32_ONE >> 1)
#define MOPH_SPEED4         (FX32_ONE >> 0)

// UV �l�̕ω��͈�
#define MOPH_MIN            0
#define MOPH_MAX            (SPRITE_SIZE << FX32_SHIFT)
#define MOPH_CENTER         ((MOPH_MAX - MOPH_MIN) / 2)

// �X�v���C�g�̉�]���x
#define ROT_UNIT            0x3F

// �e�N�X�`���̃A�h���X�Ȃ�
#define TEX_BASE            0
#define TEX_PLTT_BASE       0
#define TEX_PLTT_NO         0

// �J���[�o�[�̐�
#define NUM_OF_BAR          3


//------------------------------------------------------------------------------
// �\���̒�`

// RGB �����ꂼ��ʂɂ��J���[�f�[�^�\����
typedef struct RGB
{
    u8 r;
    u8 g;
    u8 b;
} RGB;

//------------------------------------------------------------------------------
// �O���[�o���ϐ�


//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

static void LoadResources(NNSG2dImageProxy* pImgProxy, NNSG2dImagePaletteProxy* pPltProxy);
static void SetSpriteDefaultSetting(
                NNSG2dImageProxy* pImgProxy,
                const NNSG2dImagePaletteProxy* pPltProxy);
static void InitSoftwareSpriteEnv(
                NNSG2dImageProxy* pImgProxy,
                const NNSG2dImagePaletteProxy* pPltProxy);
static void InitSprite( NNSG2dExtendedSprite* pSprite );
static void InitBar( NNSG2dBasicSprite* pBar, GXRgb color, s16 x, s16 y );
static void SetColorBarValue( NNSG2dBasicSprite* pBar, u8 value );
static void BounceValue( fx32* v, fx32* dv, fx32 min, fx32 max );
static void ProcessInput(RGB* color, s8* pos);



/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �e�N�X�`������уe�N�X�`���p���b�g�����[�h��
                VRAM �Ɋi�[���܂��B
                �v���N�V�̏��������s���܂��B

  Arguments:    pImgProxy:  �e�N�X�`���� VRAM �����󂯎��摜�v���N�V
                pPltProxy:  �e�N�X�`���p���b�g�� VRAM �����󂯎��
                            �p���b�g�v���N�V

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources(NNSG2dImageProxy* pImgProxy, NNSG2dImagePaletteProxy* pPltProxy)
{
    void* pBuf;
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    // �v���N�V�̏�����
    NNS_G2dInitImageProxy( pImgProxy );
    NNS_G2dInitImagePaletteProxy( pPltProxy );

    //------------------------------------------------------------------------------
    // load character data
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/SoftwareSprite2.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            pImgProxy );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // �o�b�t�@���J�����܂��B
        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPltData;

        pBuf = G2DDemo_LoadNCLR( &pPltData, "data/SoftwareSprite2.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        NNS_G2dLoadPalette(
            pPltData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            pPltProxy);

        G2DDemo_Free( pBuf );
    }
}



/*---------------------------------------------------------------------------*
  Name:         SetSpriteDefaultSetting

  Description:  �X�v���C�g�̃f�t�H���g�l��ݒ肵�܂��B

  Arguments:    pImgProxy:  �X�v���C�g�f�t�H���g�ݒ�̂��߂̉摜�v���N�V
                pPltProxy:  �X�v���C�g�f�t�H���g�ݒ�̂��߂̃p���b�g�v���N�V

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetSpriteDefaultSetting(
                NNSG2dImageProxy* pImgProxy,
                const NNSG2dImagePaletteProxy* pPltProxy)
{
    NNSG2dExtendedSprite      defaultSetting;
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    // UV�A�t�b���v ����
    defaultSetting.uvUL.x = 0 << FX32_SHIFT;
    defaultSetting.uvLR.x = 0 << FX32_SHIFT;
    defaultSetting.uvUL.y = 0 << FX32_SHIFT;
    defaultSetting.uvLR.y = 0 << FX32_SHIFT;
    defaultSetting.flipH = FALSE;
    defaultSetting.flipV = FALSE;

    // ��]�Ȃ�
    defaultSetting.rotO.x = 0;
    defaultSetting.rotO.y = 0;
    defaultSetting.rotX = 0;
    defaultSetting.rotY = 0;

    // �e�N�X�`���̐ݒ�
    defaultSetting.basicParams.pTextureAttr = &(pImgProxy->attr);
    defaultSetting.basicParams.texAddr      = NNS_G2dGetImageLocation(pImgProxy, NNS_G2D_VRAM_TYPE_3DMAIN);
    defaultSetting.basicParams.plttAddr     = NNS_G2dGetImagePaletteLocation(pPltProxy, NNS_G2D_VRAM_TYPE_3DMAIN);
    defaultSetting.basicParams.plttOffset   = TEX_PLTT_NO;
    defaultSetting.basicParams.color        = GX_RGB( 0, 0, 0 );

    // �f�t�H���g�l�Ƃ��Đݒ肵�܂��B
    // �ȍ~�� NNSG2dSimpleSprite NNSG2dBasicSprite �`��ł͕s���������
    // �����Őݒ肵���A�f�t�H���g�l����Q�Ƃ��ĕ`�悵�܂��B
    NNS_G2dSetSpriteDefaultAttr( &defaultSetting );
}



/*---------------------------------------------------------------------------*
  Name:         InitSoftwareSpriteEnv

  Description:  �\�t�g�E�F�A�X�v���C�g�\���̂��߂̐ݒ���s���܂��B

  Arguments:    pImgProxy:  �X�v���C�g�f�t�H���g�ݒ�̂��߂̉摜�v���N�V
                pPltProxy:  �X�v���C�g�f�t�H���g�ݒ�̂��߂̃p���b�g�v���N�V

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitSoftwareSpriteEnv(
                NNSG2dImageProxy* pImgProxy,
                const NNSG2dImagePaletteProxy* pPltProxy)
{
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    NNS_G2dSetSpriteAttrEnable(
        NNS_G2D_SPRITEATTR_TEXTURE |
        NNS_G2D_SPRITEATTR_UV      |
        NNS_G2D_SPRITEATTR_COLOR
    );

    SetSpriteDefaultSetting(pImgProxy, pPltProxy);
}



/*---------------------------------------------------------------------------*
  Name:         InitSprite

  Description:  �f���Ώۂ̃X�v���C�g�����������܂��B

  Arguments:    pSprite:    ����������X�v���C�g�ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitSprite( NNSG2dExtendedSprite* pSprite )
{
    NNSG2dSimpleSprite *simpleParams = &(pSprite->basicParams.simpleParams);
    SDK_NULL_ASSERT( pSprite );

    // �f�t�H���g�l���g���ď��������܂��B
    *pSprite = *NNS_G2dGetSpriteDefaultAttr();

    pSprite->uvUL.x = MOPH_CENTER;
    pSprite->uvLR.x = MOPH_CENTER;
    pSprite->uvUL.y = MOPH_CENTER;
    pSprite->uvLR.y = MOPH_CENTER;


    simpleParams->pos.x     = (SCREEN_WIDTH - SPRITE_SIZE)/2;
    simpleParams->pos.y     = (SCREEN_HEIGHT - SPRITE_SIZE)/2;
    simpleParams->size.x    = SPRITE_SIZE;
    simpleParams->size.y    = SPRITE_SIZE;
    simpleParams->rotZ      = 0;
    simpleParams->priority  = 0;
    simpleParams->alpha     = SPRITE_ALPHA_MAX;
}



/*---------------------------------------------------------------------------*
  Name:         InitBar

  Description:  �J���[�o�[�Ƃ��ĕ\������X�v���C�g�����������܂��B

  Arguments:    pBar:   ����������X�v���C�g�ւ̃|�C���^�B
                color:  �X�v���C�g�̐F�B
                x:      �X�v���C�g��\������ʒu�� x ���W
                y:      �X�v���C�g��\������ʒu�� y ���W

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitBar( NNSG2dBasicSprite* pBar, GXRgb color, s16 x, s16 y )
{
    NNSG2dSimpleSprite *simpleParams = &(pBar->simpleParams);
    SDK_NULL_ASSERT( pBar );

    // �f�t�H���g�l���g���ď��������܂��B
    *pBar = NNS_G2dGetSpriteDefaultAttr()->basicParams;

    pBar->color = color;

    simpleParams->pos.x     = x;
    simpleParams->pos.y     = y;
    simpleParams->size.x    = 0;        // SetColorBarValue() �Ŏw�肳��܂�
    simpleParams->size.y    = BAR_HEIGHT;
    simpleParams->rotZ      = 0;
    simpleParams->priority  = 1;        // �f���Ώۂ̃X�v���C�g���O�ɕ\�����܂�
    simpleParams->alpha     = SPRITE_ALPHA_MAX;
}



/*---------------------------------------------------------------------------*
  Name:         SetColorBarValue

  Description:  �J���[�o�[�Ƃ��ĕ\�����Ă���X�v���C�g�̒�����ύX���܂��B

  Arguments:    pBar:   ������ύX����X�v���C�g�ւ̃|�C���^�B
                value:  pBar �ɑΉ�����F�����̑傫���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetColorBarValue( NNSG2dBasicSprite* pBar, u8 value )
{
    SDK_NULL_ASSERT( pBar );
    SDK_ASSERT( 0 <= value && value <= 31 );

    pBar->simpleParams.size.x = (s16)(BAR_WIDTH_UNIT * value);
}



/*---------------------------------------------------------------------------*
  Name:         BounceValue

  Description:  *v �� *dv �������������ŁA min <= *v <= max �͈̔͂𒴂�����
                *dv �̐����𔽓]�����܂��B
                ���t���[���ĂԎ��� *v �̒l�������Ə���̊Ԃ��������܂��B

  Arguments:    v:      �l�ւ̃|�C���^�B
                dv:     �l�̑����ւ̃|�C���^
                min:    �l�̉����B
                max:    �l�̏���B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void BounceValue( fx32* v, fx32* dv, fx32 min, fx32 max )
{
    SDK_NULL_ASSERT( v );
    SDK_NULL_ASSERT( dv );
    SDK_MINMAX_ASSERT(*v, min, max);

    *v += *dv;

    if( *v < min )
    {
        *dv = - *dv;
        *v = min;
    }
    if( max < *v )
    {
        *dv = - *dv;
        *v = max;
    }
}



/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  �L�[���͂ɉ����ă��W�����[�V�����J���[��ύX���܂�

  Arguments:    color:  ���W�����[�V�����J���[�ւ̃|�C���^�B
                pos:    ���쒆�̃J���[�o�[�ԍ��ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void ProcessInput(RGB* color, s8* pos)
{
    G2DDemo_ReadGamePad();
    SDK_NULL_ASSERT( color );
    SDK_NULL_ASSERT( pos );

    // ����Ώۂ̃J���[�o�[�ύX
    if( G2DDEMO_IS_TRIGGER(PAD_KEY_UP) )
    {
        (*pos)--;

        if( *pos < 0 )
        {
            *pos = NUM_OF_BAR - 1;
        }
    }
    if( G2DDEMO_IS_TRIGGER(PAD_KEY_DOWN) )
    {
        (*pos)++;

        if( *pos >= NUM_OF_BAR )
        {
            *pos = 0;
        }
    }

    // �J���[�o�[����
    {
        u8* value;

        switch( *pos ){
        case 0: value = &(color->r); break;
        case 1: value = &(color->g); break;
        case 2: value = &(color->b); break;
        default: return;
        }

        if( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
        {
            if( *value < 31 )
            {
	            (*value)++;
            }
        }
        if( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
        {
            if( *value > 0 )
            {
	            (*value)--;
            }
        }
    }
}



/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dExtendedSprite    sprite;             // �f���Ώۂ̃X�v���C�g
    NNSG2dBasicSprite       barR, barG, barB;   // �J���[�o�[
    NNSG2dImageProxy        imgProxy;           // �摜�v���L�V
    NNSG2dImagePaletteProxy pltProxy;           // �p���b�g�v���L�V
    GXTexFmt                orgTexFmt;          // �e�N�X�`���̖{���̃t�H�[�}�b�g

    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();

        LoadResources(&imgProxy, &pltProxy);
        InitSoftwareSpriteEnv(&imgProxy, &pltProxy);
        InitSprite(&sprite);
        InitBar(&barR, COLOR_RED, BAR_POS_RX, BAR_POS_RY);
        InitBar(&barG, COLOR_GREEN, BAR_POS_GX, BAR_POS_GY);
        InitBar(&barB, COLOR_BLUE, BAR_POS_BX, BAR_POS_BY);

        // �{���̃e�N�X�`���t�H�[�}�b�g���L�^
        orgTexFmt = imgProxy.attr.fmt;
    }

    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    // Main loop
    while( TRUE )
    {
        static RGB modColor = { 31, 31, 31 };   // ���W�����[�V�����J���[
        static s8 pointerPos = 0;               // ���쒆�̐F
        static fx32 lx = MOPH_SPEED1;           // uvUL.x �̕ω����x
        static fx32 ly = MOPH_SPEED2;           // uvUL.y �̕ω����x
        static fx32 rx = MOPH_SPEED3;           // uvLR.x �̕ω����x
        static fx32 ry = MOPH_SPEED4;           // uvLR.y �̕ω����x


        // input handling
        {
            ProcessInput(&modColor, &pointerPos);

            // update color bar length
            SetColorBarValue(&barR, modColor.r);
            SetColorBarValue(&barG, modColor.g);
            SetColorBarValue(&barB, modColor.b);
        }

        // update SoftwareSprite parameter
        {
            // ���W�����[�V�����J���[�̐ݒ�
            sprite.basicParams.color = GX_RGB(modColor.r, modColor.g, modColor.b);

            // UV �l�̕ύX
            BounceValue( &(sprite.uvUL.x), &lx, MOPH_MIN, MOPH_CENTER );
            BounceValue( &(sprite.uvUL.y), &ly, MOPH_MIN, MOPH_CENTER );
            BounceValue( &(sprite.uvLR.x), &rx, MOPH_CENTER, MOPH_MAX );
            BounceValue( &(sprite.uvLR.y), &ry, MOPH_CENTER, MOPH_MAX );

            // ��]
            sprite.basicParams.simpleParams.rotZ += ROT_UNIT;
        }

        // draw SoftwareSprite
        {
            // Set texture format to original format.
            // Enable texture.
            imgProxy.attr.fmt = orgTexFmt;
            NNS_G2dDrawSpriteExtended( &sprite );

            // Set texture format to none.
            // Disable texture.
            // We can't disable texture with NNS_G2dSetSpriteAttrEnable.
            imgProxy.attr.fmt = GX_TEXFMT_NONE;
            NNS_G2dDrawSpriteBasic( &barR );
            NNS_G2dDrawSpriteBasic( &barG );
            NNS_G2dDrawSpriteBasic( &barB );
        }

        // �\�����̕`��
        {
            G2DDemo_PrintOutf(0, 0, "UL: (%4d, %4d)",
                sprite.uvUL.x >> FX32_SHIFT, sprite.uvUL.y >> FX32_SHIFT);
            G2DDemo_PrintOutf(0, 1, "LR: (%4d, %4d)",
                sprite.uvLR.x >> FX32_SHIFT, sprite.uvLR.y >> FX32_SHIFT);

            G2DDemo_PrintOutf(19, 0, "rot: %5d",
                sprite.basicParams.simpleParams.rotZ);
            G2DDemo_PrintOutf(19, 1, "     (%3.f)",
                (float)(sprite.basicParams.simpleParams.rotZ * 360) / 0x10000);

            G2DDemo_PrintOutf(0, 21, " R:%2d", modColor.r);
            G2DDemo_PrintOutf(0, 22, " G:%2d", modColor.g);
            G2DDemo_PrintOutf(0, 23, " B:%2d", modColor.b);
            G2DDemo_PrintOut(0, 21 + pointerPos, "*");
        }


        // V �u�����N�҂���
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        // BG �̔��f
        {
            G2DDemo_PrintApplyToHW();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N���荞�݂��������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}


