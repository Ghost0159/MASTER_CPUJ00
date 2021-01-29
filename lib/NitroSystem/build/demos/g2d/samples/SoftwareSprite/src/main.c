/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - SoftwareSprite
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.17 $
 *---------------------------------------------------------------------------*/


// ============================================================================
//  �f���̓��e
//      �\�t�g�E�G�A�X�v���C�g�̕`��T���v��
//      ��`����@��؂�ւ��ĕ`��������Ȃ��A���������̕ω����m�F���邱�Ƃ��ł�
//       �܂��B
//      ��\������Ă��鏈�����Ԃ́A�����ȃX�v���C�g�̕`��ɂ����������Ԃł��B
//       (�����̑傫�ȃX�v���C�g�͏���)
//
//  ������@
//      B �`����@�̕ύX
// ============================================================================

#include <stdlib.h>
#include <nitro.h>
#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Load.h>

#include "g2d_demolib.h"


#define SPRITE_ROT_2PI          0x10000 // �\�t�g�E�F�A�X�v���C�g��]�ɂ�����2�� rad.�����l
#define SPRITE_ALPHA_MAX        31      // �\�t�g�E�F�A�X�v���C�g�̍ő僿�l
#define SCREEN_WIDTH            256     // ��ʕ�
#define SCREEN_HEIGHT           192     // ��ʍ�

#define NUM_OF_SIMPLE_SPRITE    512     // �V���v���X�v���C�g�̐�
#define SPL_SPRITE_SIZE         8       // �V���v���X�v���C�g�̑傫��
#define EXT_SPRITE_SIZE         96      // �g���X�v���C�g�̑傫��
#define TEXTURE_SIZE            8       // �e�N�X�`���̑傫��

#define ROT_DX                  0xFF    // X������]�̑���
#define ROT_DY                  0x7F    // Y������]�̑���
#define ROT_DZ                  0xBF    // Z������]�̑���

#define SPRITE_ROT_PI_PER_2     (SPRITE_ROT_2PI / 4)

#define TEX_BASE                0x0     // �e�N�X�`���x�[�X�A�h���X
#define TEX_PLTT_BASE           0x0     // �e�N�X�`���p���b�g�x�[�X�A�h���X
#define TEX_PLTT_NO             0x2     // ��{�̃e�N�X�`���p���b�g�ԍ�


//------------------------------------------------------------------------------
// �`����@
typedef enum MyDrawType
{
    MyDrawType_DrawDirect = 0,  // �p�����[�^�𒼐ڎw�肵�ĕ`������s����API
    MyDrawType_DrawSprite,      // �ʏ�̕`��
    MyDrawType_DrawSpriteFast,  // �J�����g�s���ۑ����Ȃ������`��
    MyDrawType_MAX

}MyDrawType;

const char* MyDrawTypeStr [] =
{
    "DirectParams",
    "Normal      ",
    "Fast        ",
};


//------------------------------------------------------------------------------
// ���x�����X�v���C�g�I�u�W�F�N�g

typedef struct SimpleSpriteObject
{
    NNSG2dSimpleSprite sprite;

    struct Speed
    {
        short x;
        short y;
    } speed;
} SimpleSpriteObject;



//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

void VBlankIntr(void);

static inline BOOL IsInvisibleRot(u16 rot, u16 margin);
static inline short GetSpeedRandom(void);
static inline u8 GetAlphaRandom(void);

static void LoadResources(void);
static void MoveSimpleSpriteObject(SimpleSpriteObject *pSSObj);

static void SetSpriteDefaultSetting(void);
static void InitBallObject(SimpleSpriteObject objects[]);
static void InitSquareSprite(NNSG2dExtendedSprite *pes);
static void InitSoftwareSprite(void);


//------------------------------------------------------------------------------
// �O���[�o���ϐ�

static SimpleSpriteObject          sBalls[NUM_OF_SIMPLE_SPRITE];    // �V���v���X�v���C�g
static NNSG2dExtendedSprite        sSquare;                         // �g���X�v���C�g

static NNSG2dImageAttr texAttr_ =       // ���ʃe�N�X�`���ݒ�
{
    GX_TEXSIZE_S256,        // S�T�C�Y
    GX_TEXSIZE_T256,        // T�T�C�Y
    GX_TEXFMT_PLTT16,       // �e�N�X�`���t�H�[�}�b�g
    FALSE,                  // �g���p���b�g���ǂ���
    GX_TEXPLTTCOLOR0_TRNS,  // ���ߐF�L��
    GX_OBJVRAMMODE_CHAR_2D  // �L�����N�^�}�b�s���O�^�C�v
};


MyDrawType                      sDrawType = MyDrawType_DrawDirect; // �`����@

/*---------------------------------------------------------------------------*
  Name:         IsInvisibleRot

  Description:  ��]�p rot ���\�t�g�E�F�A�X�v���C�g���^�����������߂�
                �\������Ȃ��Ȃ�p�x�A���Ȃ킿 90�x��������270�x��
                ���邩�ǂ������덷���l�����Ĕ��肵�܂��B

  Arguments:    rot:    ����Ώۂ̉�]�p
                margin: ���苖�e�덷

  Returns:      ��\���ƂȂ��]�p�ł����TRUE�A�����łȂ����FALSE�B
 *---------------------------------------------------------------------------*/
static inline BOOL IsInvisibleRot(u16 rot, u16 margin)
{
    return ( (SPRITE_ROT_PI_PER_2 - margin) < rot && rot < (SPRITE_ROT_PI_PER_2 + margin) )
        || ( (SPRITE_ROT_PI_PER_2 * 3 - margin) < rot && rot < (SPRITE_ROT_PI_PER_2* 3 + margin) );
}

/*---------------------------------------------------------------------------*
  Name:         GetSpeedRandom

  Description:  �����_���ȑ��x�𓾂܂��B

  Arguments:    �Ȃ��B

  Returns:      �����_���Ɍ��肳�ꂽ���x�B
 *---------------------------------------------------------------------------*/
static inline short GetSpeedRandom(void)
{
    return (short)((rand() & 0x3) + 1);
}

/*---------------------------------------------------------------------------*
  Name:         GetAlphaRandom

  Description:  �����_���ȃ��l�𓾂܂��B

  Arguments:    �Ȃ��B

  Returns:      �����_���Ɍ��肳�ꂽ���l�B
 *---------------------------------------------------------------------------*/
static inline u8 GetAlphaRandom(void)
{
    return (u8)(rand() & 0x1F);
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �L�����N�^�f�[�^�ƃp���b�g�f�[�^���t�@�C������ǂݍ���
                ���ꂼ���VRAM�Ƀ��[�h���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources(void)
{
    NNSG2dImageProxy imgProxy;
    NNSG2dImagePaletteProxy pltProxy;
    void* pBuf;

    NNS_G2dInitImageProxy( &imgProxy );
    NNS_G2dInitImagePaletteProxy( &pltProxy );

    //------------------------------------------------------------------------------
    // load character data
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/SoftwareSprite.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &imgProxy );

        // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
        // �o�b�t�@���J�����܂��B
        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPltData;

        pBuf = G2DDemo_LoadNCLR( &pPltData, "data/SoftwareSprite.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        NNS_G2dLoadPalette(
            pPltData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &pltProxy);

        G2DDemo_Free( pBuf );
    }

}

/*---------------------------------------------------------------------------*
  Name:         SetSpriteDefaultSetting

  Description:  �X�v���C�g�̃f�t�H���g������ݒ肵�܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void SetSpriteDefaultSetting(void)
{
    NNSG2dExtendedSprite      defaultSetting;

    defaultSetting.uvUL.x = 0 << FX32_SHIFT;
    defaultSetting.uvLR.x = TEXTURE_SIZE << FX32_SHIFT;
    defaultSetting.uvUL.y = 0 << FX32_SHIFT;
    defaultSetting.uvLR.y = TEXTURE_SIZE << FX32_SHIFT;
    defaultSetting.flipH = FALSE;
    defaultSetting.flipV = FALSE;

    defaultSetting.rotO.x = 0;
    defaultSetting.rotO.y = 0;
    defaultSetting.rotX = 0;
    defaultSetting.rotY = 0;

    defaultSetting.basicParams.pTextureAttr = &texAttr_;
    defaultSetting.basicParams.texAddr      = TEX_BASE;
    defaultSetting.basicParams.plttAddr     = TEX_PLTT_BASE;
    defaultSetting.basicParams.plttOffset   = TEX_PLTT_NO;
    defaultSetting.basicParams.color        = GX_RGB( 0, 0, 0 );

    // �f�t�H���g�l�Ƃ��Đݒ肵�܂��B
    // �ȍ~�� NNSG2dSimpleSprite NNSG2dBasicSprite �`��ł͕s���������
    // �����Őݒ肵���A�f�t�H���g�l����Q�Ƃ��ĕ`�悵�܂��B
    NNS_G2dSetSpriteDefaultAttr( &defaultSetting );
}

/*---------------------------------------------------------------------------*
  Name:         InitBallObject

  Description:  SimpleSpriteObject �����������܂��B

  Arguments:    pObj:  ���������� SimpleSpriteObject �ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitBallObject(SimpleSpriteObject *pObj)
{
    NNSG2dSimpleSprite *sprite = &(pObj->sprite);

    pObj->speed.x = GetSpeedRandom();
    pObj->speed.y = GetSpeedRandom();

    sprite->pos.x     = 0;
    sprite->pos.y     = 0;
    sprite->size.x    = SPL_SPRITE_SIZE;
    sprite->size.y    = SPL_SPRITE_SIZE;
    sprite->rotZ      = 0;
    sprite->priority  = 0;
    sprite->alpha     = GetAlphaRandom();
}

/*---------------------------------------------------------------------------*
  Name:         InitSquareSprite

  Description:  �g���X�v���C�g���l�p�`�\���p�ɏ��������܂��B

  Arguments:    pes:  ���������� NNSG2dExtendedSprite �ւ̃|�C���^

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitSquareSprite(NNSG2dExtendedSprite *pes)
{
    NNSG2dSimpleSprite *simpleParams = &(pes->basicParams.simpleParams);

    // �f�t�H���g�l���g���ď��������܂��B
    *pes = *NNS_G2dGetSpriteDefaultAttr();

    pes->uvUL.x = TEXTURE_SIZE << FX32_SHIFT;
    pes->uvLR.x = (TEXTURE_SIZE * 2) << FX32_SHIFT;
    pes->uvUL.y = 0 << FX32_SHIFT;
    pes->uvLR.y = TEXTURE_SIZE << FX32_SHIFT;

    pes->rotO.x = - EXT_SPRITE_SIZE/2;                  // �X�v���C�g�̍������
    pes->rotO.y = - EXT_SPRITE_SIZE/2;                  // ��]�̒��S�_�ɂ��܂��B

    simpleParams->pos.x     = SCREEN_WIDTH/2;
    simpleParams->pos.y     = SCREEN_HEIGHT/2;
    simpleParams->size.x    = EXT_SPRITE_SIZE;
    simpleParams->size.y    = EXT_SPRITE_SIZE;
    simpleParams->rotZ      = 0;
    simpleParams->priority  = 0;
    simpleParams->alpha     = SPRITE_ALPHA_MAX;
}

/*---------------------------------------------------------------------------*
  Name:         InitSoftwareSprite

  Description:  �\�t�g�E�F�A�X�v���C�g�̊��ƃf�[�^�����������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitSoftwareSprite(void)
{
    G2DDemo_CameraSetup();
    G2DDemo_MaterialSetup();

    // �`��̍ۂɗL���ƂȂ�A������ݒ肵�܂��B
    // �s�K�v�ȁA�����𖳌��ݒ肷�邱�Ƃɂ���āA�p�t�H�[�}���X�����サ�܂��B
    NNS_G2dSetSpriteAttrEnable(
        NNS_G2D_SPRITEATTR_ALPHA  |
        NNS_G2D_SPRITEATTR_UV     |
        NNS_G2D_SPRITEATTR_TEXTURE|
        NNS_G2D_SPRITEATTR_ROTXY  |
        NNS_G2D_SPRITEATTR_ROTO
    );
    
    SetSpriteDefaultSetting();

    {
        int i;
        for( i = 0; i < NUM_OF_SIMPLE_SPRITE; i++ )
        {
            InitBallObject(&sBalls[i]);
        }
    }
    InitSquareSprite(&sSquare);
}


/*---------------------------------------------------------------------------*
  Name:         MoveSimpleSpriteObject

  Description:  SimpleSpriteObject ���A���� speed �ɉ����Ĉړ����܂��B
                ��ʒ[�ł̔��˂Ɣ��ˎ��̃��l�ύX���s���܂��B

  Arguments:    pSSObj:  �ړ�������I�u�W�F�N�g

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void MoveSimpleSpriteObject(SimpleSpriteObject *pSSObj)
{
    BOOL bReflected = FALSE;
    NNSG2dSVec2 *pPos = &(pSSObj->sprite.pos);
    struct Speed *pSpeed = &(pSSObj->speed);

    // �ʒu�̍X�V
    pPos->x += pSpeed->x;
    pPos->y += pSpeed->y;

    // ��ʒ[�Ŕ��˂���
    {
        if( pPos->x < 0 || (SCREEN_WIDTH - SPL_SPRITE_SIZE) <= pPos->x )
        {
            pPos->x -= pSpeed->x;
            pSpeed->x = (short)((pSpeed->x < 0) ? GetSpeedRandom(): - GetSpeedRandom());
            bReflected = TRUE;
        }

        if( pPos->y < 0 || (SCREEN_HEIGHT - SPL_SPRITE_SIZE) <= pPos->y )
        {
            pPos->y -= pSpeed->y;
            pSpeed->y = (short)((pSpeed->y < 0) ? GetSpeedRandom(): - GetSpeedRandom());
            bReflected = TRUE;
        }
    }

    // ���ˎ��Ƀ��l��ύX����
    if( bReflected )
    {
        pSSObj->sprite.alpha = GetAlphaRandom();
    }
}


/*---------------------------------------------------------------------------*
  Name:         DrawDirect

  Description:  �p�����[�^�𒼐ڎw�肷��API �𗘗p�����`��B
                �`��API��UV�l���w�肵�āA�e�N�X�`���t�l�p�|���S����`�悷�邾���Ȃ̂ŁA
                3D �O���t�B�b�N�G���W���̐ݒ�����[�U���s���K�v������B
                �����Ƃ������ɓ��삷��B
  

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void DrawDirect()
{
    G3_PushMtx();
    {
        int i;
        //------------------------------------------------------------------------------
        // 3D�O���t�B�b�N�X�G���W���F�e�N�X�`���֘A�p�����[�^�̐ݒ�
        {
            NNSG2dImageAttr*    pAttr = &texAttr_;
            
            G3_TexImageParam( pAttr->fmt,    // use alpha texture
                 GX_TEXGEN_TEXCOORD,         // use texcoord
                 pAttr->sizeS,               // 16 pixels
                 pAttr->sizeT,               // 16 pixels
                 GX_TEXREPEAT_NONE,          // no repeat
                 GX_TEXFLIP_NONE,            // no flip
                 pAttr->plttUse,             // use color 0 of the palette
                 TEX_BASE                    // the offset of the texture image
                 );
            
            if( NNS_G2dIsPaletteImageFmt( pAttr ) )
            {
                G3_TexPlttBase( (u32)( TEX_PLTT_BASE + 32 * TEX_PLTT_NO), pAttr->fmt );                                
            }
        }
        
        //------------------------------------------------------------------------------
        // �`��F�p�����[�^�𒼐ڈ����Ɏw�肵�ĕ`�悷��API
        for( i = 0;i < NUM_OF_SIMPLE_SPRITE; i++ )
        {
            //
            // �A���t�@�l���[���Ȃ�`�悵�܂���
            //
            if( sBalls[i].sprite.alpha == 0 )
            {
                continue;
            }
            
            //
            // �A���t�@�l�̐ݒ�
            //
            G3_PolygonAttr( GX_LIGHTMASK_NONE,       // disable lights
                            GX_POLYGONMODE_MODULATE, // modulation mode
                            GX_CULL_NONE,            // cull back
                            0,                       // polygon ID(0 - 63)
                            sBalls[i].sprite.alpha,  // alpha(0 - 31)
                            0                        // OR of GXPolygonAttrMisc's value
                            );
            
            
            G3_Identity();
            NNS_G2dDrawSpriteFast( sBalls[i].sprite.pos.x, sBalls[i].sprite.pos.y, -1, 
                                   SPL_SPRITE_SIZE, SPL_SPRITE_SIZE,
                                   0, 0, TEXTURE_SIZE , TEXTURE_SIZE );
        }
    }
    G3_PopMtx(1); 
}

/*---------------------------------------------------------------------------*
  Name:         DrawNormal

  Description:  �ʏ�̕`��֐��ɂ��`��
  

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void DrawNormal()
{
    //
    // �ʏ�̕`��֐��ɂ��`��
    //
    // �A�g���r���[�g��ݒ肵�܂�
    NNS_G2dSetSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA | NNS_G2D_SPRITEATTR_TEXTURE );
    NNS_G2dDrawSpriteSimple( &(sBalls[0].sprite) );
    // �X�v���C�g���ɕω�����p�����[�^�݂̂�L���ɐݒ肵�܂��B
    NNS_G2dSetSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA );
    
    {
        int i;
        for( i = 0;i < NUM_OF_SIMPLE_SPRITE; i++ )
        {
            G3_Identity();
            NNS_G2dDrawSpriteSimple( &(sBalls[i].sprite) );
        }
    }
    
    
}

/*---------------------------------------------------------------------------*
  Name:         DrawFast

  Description:  NNS_G2dDrawSprite*Fast�֐����g�p�����`��B
                �J�����g�s���`��֐��R�[���̑O��ŕۑ����Ȃ��̂ŁA�����ɓ��삵�܂��B
  

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void DrawFast()
{
    G3_PushMtx();
        
        {
            int i;
            
            NNS_G2dSetSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA | NNS_G2D_SPRITEATTR_TEXTURE );
            NNS_G2dDrawSpriteSimple( &(sBalls[0].sprite) );
            // �X�v���C�g���ɕω�����p�����[�^�݂̂�L���ɐݒ肵�܂��B
            NNS_G2dSetSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA );
    
            for( i = 0;i < NUM_OF_SIMPLE_SPRITE; i++ )
            {
                G3_Identity();
                NNS_G2dDrawSpriteSimpleFast( &(sBalls[i].sprite) );
            }
        }
        
        
    
    G3_PopMtx(1);
}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // Initialize App.
    {
        G2DDemo_CommonInit();
        LoadResources();
        InitSoftwareSprite();
        G2DDemo_PrintInit();
    }

    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    //
    // Main loop
    //
    while( TRUE )
    {
        static u16 rotX = 0x0;
        static u16 rotY = 0x0;
        static u16 rotZ = 0x0;
        static u16 plttNo = TEX_PLTT_NO;
        static OSTick time;
        

        //
        // �L�[����
        //
        {
            G2DDemo_ReadGamePad();
            
            if ( G2DDEMO_IS_TRIGGER(PAD_BUTTON_B) )
            {
                sDrawType++;
                sDrawType %= MyDrawType_MAX;
            }
        }   
        
    
        // draw
        {
            time = OS_GetTick();   
                switch( sDrawType )
                {
                case MyDrawType_DrawDirect:     DrawDirect();  break;
                case MyDrawType_DrawSprite:     DrawNormal();  break;
                case MyDrawType_DrawSpriteFast: DrawFast();    break;
                }
            time = OS_GetTick() - time;
            
            // draw extended sprite
            NNS_G2dSetSpriteAttrEnable(
                NNS_G2D_SPRITEATTR_ALPHA  |
                NNS_G2D_SPRITEATTR_UV     |
                NNS_G2D_SPRITEATTR_TEXTURE|
                NNS_G2D_SPRITEATTR_ROTXY  |
                NNS_G2D_SPRITEATTR_ROTO );
                
            sSquare.rotX = rotX;
            sSquare.rotY = rotY;
            sSquare.basicParams.simpleParams.rotZ = rotZ;
            sSquare.basicParams.plttOffset = plttNo;

            NNS_G2dDrawSpriteExtended( &sSquare );
        }
        
        {
            // �X�v���C�g�ʒu�̍X�V
            int i;
            for( i = 0;i < NUM_OF_SIMPLE_SPRITE; i++ )
            {
                MoveSimpleSpriteObject(&sBalls[i]);
            }
        
            // update animate values
            rotX += ROT_DX;
            rotY += ROT_DY;
            rotZ += ROT_DZ;
            
            if( IsInvisibleRot(rotX, ROT_DX/2) || IsInvisibleRot(rotY, ROT_DY/2) )
            {
                plttNo = (u16)(rand() & 0x7);
            }
        }
        //
        // �f�o�b�N���̕\��
        // 
        {
            G2DDemo_PrintOutf( 0, 21, "TIME:%06ld usec\n", OS_TicksToMicroSeconds(time) );
            G2DDemo_PrintOut ( 0, 22, MyDrawTypeStr[sDrawType] );
        }
        
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();
        
        // �\�����
        G2DDemo_PrintApplyToHW();
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

