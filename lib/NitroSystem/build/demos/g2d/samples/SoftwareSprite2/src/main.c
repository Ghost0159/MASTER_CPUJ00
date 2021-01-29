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
//  デモの内容
//      回転するソフトウェアスプライト上で
//      テクスチャUV値とモジュレーションカラーを動的に変更する。
//
//  操作方法
//      十字キー上下    RGB選択
//      十字キー左右    RGB値変更（モジュレーションカラーを変更します）
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define SCREEN_WIDTH        256                     // 画面幅
#define SCREEN_HEIGHT       192                     // 画面高さ

#define SPRITE_ALPHA_MAX    31                      // α値の最大値


// スプライトの大きさ（正方形）
#define SPRITE_SIZE         128

// カラーバーの大きさ
#define BAR_HEIGHT          6
#define BAR_WIDTH_UNIT      6

// カラーバーの位置（ x 座標）
#define BAR_POS_RX          45
#define BAR_POS_GX          BAR_POS_RX
#define BAR_POS_BX          BAR_POS_RX

// カラーバーの位置（ y 座標）
#define LINE_HEIGHT         8
#define LINE_MARGIN         ((LINE_HEIGHT - BAR_HEIGHT) / 2)
#define BAR_POS_RY          (BAR_POS_GY - LINE_HEIGHT)
#define BAR_POS_GY          (BAR_POS_BY - LINE_HEIGHT)
#define BAR_POS_BY          (SCREEN_HEIGHT - LINE_HEIGHT + LINE_MARGIN)

// 色
#define COLOR_RED           GX_RGB(31, 0, 0)
#define COLOR_GREEN         GX_RGB(0, 31, 0)
#define COLOR_BLUE          GX_RGB(0, 0, 31)

// UV 値の変化速度
#define MOPH_SPEED1         -(FX32_ONE >> 3)
#define MOPH_SPEED2         -(FX32_ONE >> 2)
#define MOPH_SPEED3         (FX32_ONE >> 1)
#define MOPH_SPEED4         (FX32_ONE >> 0)

// UV 値の変化範囲
#define MOPH_MIN            0
#define MOPH_MAX            (SPRITE_SIZE << FX32_SHIFT)
#define MOPH_CENTER         ((MOPH_MAX - MOPH_MIN) / 2)

// スプライトの回転速度
#define ROT_UNIT            0x3F

// テクスチャのアドレスなど
#define TEX_BASE            0
#define TEX_PLTT_BASE       0
#define TEX_PLTT_NO         0

// カラーバーの数
#define NUM_OF_BAR          3


//------------------------------------------------------------------------------
// 構造体定義

// RGB をそれぞれ別にもつカラーデータ構造体
typedef struct RGB
{
    u8 r;
    u8 g;
    u8 b;
} RGB;

//------------------------------------------------------------------------------
// グローバル変数


//------------------------------------------------------------------------------
// プロトタイプ宣言

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

  Description:  テクスチャおよびテクスチャパレットをロードし
                VRAM に格納します。
                プロクシの初期化も行います。

  Arguments:    pImgProxy:  テクスチャの VRAM 情報を受け取る画像プロクシ
                pPltProxy:  テクスチャパレットの VRAM 情報を受け取る
                            パレットプロクシ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadResources(NNSG2dImageProxy* pImgProxy, NNSG2dImagePaletteProxy* pPltProxy)
{
    void* pBuf;
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    // プロクシの初期化
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

        // キャラクタデータを VRAM にコピーしたので
        // バッファを開放します。
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

  Description:  スプライトのデフォルト値を設定します。

  Arguments:    pImgProxy:  スプライトデフォルト設定のための画像プロクシ
                pPltProxy:  スプライトデフォルト設定のためのパレットプロクシ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SetSpriteDefaultSetting(
                NNSG2dImageProxy* pImgProxy,
                const NNSG2dImagePaletteProxy* pPltProxy)
{
    NNSG2dExtendedSprite      defaultSetting;
    SDK_NULL_ASSERT( pImgProxy );
    SDK_NULL_ASSERT( pPltProxy );

    // UV、フッリプ 無効
    defaultSetting.uvUL.x = 0 << FX32_SHIFT;
    defaultSetting.uvLR.x = 0 << FX32_SHIFT;
    defaultSetting.uvUL.y = 0 << FX32_SHIFT;
    defaultSetting.uvLR.y = 0 << FX32_SHIFT;
    defaultSetting.flipH = FALSE;
    defaultSetting.flipV = FALSE;

    // 回転なし
    defaultSetting.rotO.x = 0;
    defaultSetting.rotO.y = 0;
    defaultSetting.rotX = 0;
    defaultSetting.rotY = 0;

    // テクスチャの設定
    defaultSetting.basicParams.pTextureAttr = &(pImgProxy->attr);
    defaultSetting.basicParams.texAddr      = NNS_G2dGetImageLocation(pImgProxy, NNS_G2D_VRAM_TYPE_3DMAIN);
    defaultSetting.basicParams.plttAddr     = NNS_G2dGetImagePaletteLocation(pPltProxy, NNS_G2D_VRAM_TYPE_3DMAIN);
    defaultSetting.basicParams.plttOffset   = TEX_PLTT_NO;
    defaultSetting.basicParams.color        = GX_RGB( 0, 0, 0 );

    // デフォルト値として設定します。
    // 以降の NNSG2dSimpleSprite NNSG2dBasicSprite 描画では不足する情報を
    // ここで設定した、デフォルト値から参照して描画します。
    NNS_G2dSetSpriteDefaultAttr( &defaultSetting );
}



/*---------------------------------------------------------------------------*
  Name:         InitSoftwareSpriteEnv

  Description:  ソフトウェアスプライト表示のための設定を行います。

  Arguments:    pImgProxy:  スプライトデフォルト設定のための画像プロクシ
                pPltProxy:  スプライトデフォルト設定のためのパレットプロクシ

  Returns:      なし。
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

  Description:  デモ対象のスプライトを初期化します。

  Arguments:    pSprite:    初期化するスプライトへのポインタ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitSprite( NNSG2dExtendedSprite* pSprite )
{
    NNSG2dSimpleSprite *simpleParams = &(pSprite->basicParams.simpleParams);
    SDK_NULL_ASSERT( pSprite );

    // デフォルト値を使って初期化します。
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

  Description:  カラーバーとして表示するスプライトを初期化します。

  Arguments:    pBar:   初期化するスプライトへのポインタ。
                color:  スプライトの色。
                x:      スプライトを表示する位置の x 座標
                y:      スプライトを表示する位置の y 座標

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitBar( NNSG2dBasicSprite* pBar, GXRgb color, s16 x, s16 y )
{
    NNSG2dSimpleSprite *simpleParams = &(pBar->simpleParams);
    SDK_NULL_ASSERT( pBar );

    // デフォルト値を使って初期化します。
    *pBar = NNS_G2dGetSpriteDefaultAttr()->basicParams;

    pBar->color = color;

    simpleParams->pos.x     = x;
    simpleParams->pos.y     = y;
    simpleParams->size.x    = 0;        // SetColorBarValue() で指定されます
    simpleParams->size.y    = BAR_HEIGHT;
    simpleParams->rotZ      = 0;
    simpleParams->priority  = 1;        // デモ対象のスプライトより前に表示します
    simpleParams->alpha     = SPRITE_ALPHA_MAX;
}



/*---------------------------------------------------------------------------*
  Name:         SetColorBarValue

  Description:  カラーバーとして表示しているスプライトの長さを変更します。

  Arguments:    pBar:   長さを変更するスプライトへのポインタ。
                value:  pBar に対応する色成分の大きさ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void SetColorBarValue( NNSG2dBasicSprite* pBar, u8 value )
{
    SDK_NULL_ASSERT( pBar );
    SDK_ASSERT( 0 <= value && value <= 31 );

    pBar->simpleParams.size.x = (s16)(BAR_WIDTH_UNIT * value);
}



/*---------------------------------------------------------------------------*
  Name:         BounceValue

  Description:  *v に *dv を加えたうえで、 min <= *v <= max の範囲を超えたら
                *dv の正負を反転させます。
                毎フレーム呼ぶ事で *v の値が下限と上限の間を往復します。

  Arguments:    v:      値へのポインタ。
                dv:     値の増分へのポインタ
                min:    値の下限。
                max:    値の上限。

  Returns:      なし。
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

  Description:  キー入力に応じてモジュレーションカラーを変更します

  Arguments:    color:  モジュレーションカラーへのポインタ。
                pos:    操作中のカラーバー番号へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ProcessInput(RGB* color, s8* pos)
{
    G2DDemo_ReadGamePad();
    SDK_NULL_ASSERT( color );
    SDK_NULL_ASSERT( pos );

    // 操作対象のカラーバー変更
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

    // カラーバー操作
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

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dExtendedSprite    sprite;             // デモ対象のスプライト
    NNSG2dBasicSprite       barR, barG, barB;   // カラーバー
    NNSG2dImageProxy        imgProxy;           // 画像プロキシ
    NNSG2dImagePaletteProxy pltProxy;           // パレットプロキシ
    GXTexFmt                orgTexFmt;          // テクスチャの本来のフォーマット

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

        // 本来のテクスチャフォーマットを記録
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
        static RGB modColor = { 31, 31, 31 };   // モジュレーションカラー
        static s8 pointerPos = 0;               // 操作中の色
        static fx32 lx = MOPH_SPEED1;           // uvUL.x の変化速度
        static fx32 ly = MOPH_SPEED2;           // uvUL.y の変化速度
        static fx32 rx = MOPH_SPEED3;           // uvLR.x の変化速度
        static fx32 ry = MOPH_SPEED4;           // uvLR.y の変化速度


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
            // モジュレーションカラーの設定
            sprite.basicParams.color = GX_RGB(modColor.r, modColor.g, modColor.b);

            // UV 値の変更
            BounceValue( &(sprite.uvUL.x), &lx, MOPH_MIN, MOPH_CENTER );
            BounceValue( &(sprite.uvUL.y), &ly, MOPH_MIN, MOPH_CENTER );
            BounceValue( &(sprite.uvLR.x), &rx, MOPH_CENTER, MOPH_MAX );
            BounceValue( &(sprite.uvLR.y), &ry, MOPH_CENTER, MOPH_MAX );

            // 回転
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

        // 表示情報の描画
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


        // V ブランク待ちへ
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();


        // BG の反映
        {
            G2DDemo_PrintApplyToHW();
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Vブランク割り込みを処理します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}


