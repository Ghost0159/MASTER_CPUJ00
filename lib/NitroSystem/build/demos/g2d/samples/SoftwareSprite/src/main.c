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
//  デモの内容
//      ソフトウエアスプライトの描画サンプル
//      ･描画方法を切り替えて描画をおこない、処理効率の変化を確認することができ
//       ます。
//      ･表示されている処理時間は、小さなスプライトの描画にかかった時間です。
//       (中央の大きなスプライトは除く)
//
//  操作方法
//      B 描画方法の変更
// ============================================================================

#include <stdlib.h>
#include <nitro.h>
#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Load.h>

#include "g2d_demolib.h"


#define SPRITE_ROT_2PI          0x10000 // ソフトウェアスプライト回転における2π rad.相当値
#define SPRITE_ALPHA_MAX        31      // ソフトウェアスプライトの最大α値
#define SCREEN_WIDTH            256     // 画面幅
#define SCREEN_HEIGHT           192     // 画面高

#define NUM_OF_SIMPLE_SPRITE    512     // シンプルスプライトの数
#define SPL_SPRITE_SIZE         8       // シンプルスプライトの大きさ
#define EXT_SPRITE_SIZE         96      // 拡張スプライトの大きさ
#define TEXTURE_SIZE            8       // テクスチャの大きさ

#define ROT_DX                  0xFF    // X方向回転の増分
#define ROT_DY                  0x7F    // Y方向回転の増分
#define ROT_DZ                  0xBF    // Z方向回転の増分

#define SPRITE_ROT_PI_PER_2     (SPRITE_ROT_2PI / 4)

#define TEX_BASE                0x0     // テクスチャベースアドレス
#define TEX_PLTT_BASE           0x0     // テクスチャパレットベースアドレス
#define TEX_PLTT_NO             0x2     // 基本のテクスチャパレット番号


//------------------------------------------------------------------------------
// 描画方法
typedef enum MyDrawType
{
    MyDrawType_DrawDirect = 0,  // パラメータを直接指定して描画を実行するAPI
    MyDrawType_DrawSprite,      // 通常の描画
    MyDrawType_DrawSpriteFast,  // カレント行列を保存しない高速描画
    MyDrawType_MAX

}MyDrawType;

const char* MyDrawTypeStr [] =
{
    "DirectParams",
    "Normal      ",
    "Fast        ",
};


//------------------------------------------------------------------------------
// 速度を持つスプライトオブジェクト

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
// プロトタイプ宣言

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
// グローバル変数

static SimpleSpriteObject          sBalls[NUM_OF_SIMPLE_SPRITE];    // シンプルスプライト
static NNSG2dExtendedSprite        sSquare;                         // 拡張スプライト

static NNSG2dImageAttr texAttr_ =       // 共通テクスチャ設定
{
    GX_TEXSIZE_S256,        // Sサイズ
    GX_TEXSIZE_T256,        // Tサイズ
    GX_TEXFMT_PLTT16,       // テクスチャフォーマット
    FALSE,                  // 拡張パレットかどうか
    GX_TEXPLTTCOLOR0_TRNS,  // 透過色有効
    GX_OBJVRAMMODE_CHAR_2D  // キャラクタマッピングタイプ
};


MyDrawType                      sDrawType = MyDrawType_DrawDirect; // 描画方法

/*---------------------------------------------------------------------------*
  Name:         IsInvisibleRot

  Description:  回転角 rot がソフトウェアスプライトが真横を向くために
                表示されなくなる角度、すなわち 90度もしくは270度で
                あるかどうかを誤差を考慮して判定します。

  Arguments:    rot:    判定対象の回転角
                margin: 判定許容誤差

  Returns:      非表示となる回転角であればTRUE、そうでなければFALSE。
 *---------------------------------------------------------------------------*/
static inline BOOL IsInvisibleRot(u16 rot, u16 margin)
{
    return ( (SPRITE_ROT_PI_PER_2 - margin) < rot && rot < (SPRITE_ROT_PI_PER_2 + margin) )
        || ( (SPRITE_ROT_PI_PER_2 * 3 - margin) < rot && rot < (SPRITE_ROT_PI_PER_2* 3 + margin) );
}

/*---------------------------------------------------------------------------*
  Name:         GetSpeedRandom

  Description:  ランダムな速度を得ます。

  Arguments:    なし。

  Returns:      ランダムに決定された速度。
 *---------------------------------------------------------------------------*/
static inline short GetSpeedRandom(void)
{
    return (short)((rand() & 0x3) + 1);
}

/*---------------------------------------------------------------------------*
  Name:         GetAlphaRandom

  Description:  ランダムなα値を得ます。

  Arguments:    なし。

  Returns:      ランダムに決定されたα値。
 *---------------------------------------------------------------------------*/
static inline u8 GetAlphaRandom(void)
{
    return (u8)(rand() & 0x1F);
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  キャラクタデータとパレットデータをファイルから読み込み
                それぞれのVRAMにロードします。

  Arguments:    なし。

  Returns:      なし。
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

        // キャラクタデータを VRAM にコピーしたので
        // バッファを開放します。
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

  Description:  スプライトのデフォルト属性を設定します。

  Arguments:    なし。

  Returns:      なし。
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

    // デフォルト値として設定します。
    // 以降の NNSG2dSimpleSprite NNSG2dBasicSprite 描画では不足する情報を
    // ここで設定した、デフォルト値から参照して描画します。
    NNS_G2dSetSpriteDefaultAttr( &defaultSetting );
}

/*---------------------------------------------------------------------------*
  Name:         InitBallObject

  Description:  SimpleSpriteObject を初期化します。

  Arguments:    pObj:  初期化する SimpleSpriteObject へのポインタ

  Returns:      なし。
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

  Description:  拡張スプライトを四角形表示用に初期化します。

  Arguments:    pes:  初期化する NNSG2dExtendedSprite へのポインタ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitSquareSprite(NNSG2dExtendedSprite *pes)
{
    NNSG2dSimpleSprite *simpleParams = &(pes->basicParams.simpleParams);

    // デフォルト値を使って初期化します。
    *pes = *NNS_G2dGetSpriteDefaultAttr();

    pes->uvUL.x = TEXTURE_SIZE << FX32_SHIFT;
    pes->uvLR.x = (TEXTURE_SIZE * 2) << FX32_SHIFT;
    pes->uvUL.y = 0 << FX32_SHIFT;
    pes->uvLR.y = TEXTURE_SIZE << FX32_SHIFT;

    pes->rotO.x = - EXT_SPRITE_SIZE/2;                  // スプライトの左上隅を
    pes->rotO.y = - EXT_SPRITE_SIZE/2;                  // 回転の中心点にします。

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

  Description:  ソフトウェアスプライトの環境とデータを初期化します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitSoftwareSprite(void)
{
    G2DDemo_CameraSetup();
    G2DDemo_MaterialSetup();

    // 描画の際に有効となる、属性を設定します。
    // 不必要な、属性を無効設定することによって、パフォーマンスが向上します。
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

  Description:  SimpleSpriteObject を、その speed に応じて移動します。
                画面端での反射と反射時のα値変更も行います。

  Arguments:    pSSObj:  移動させるオブジェクト

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void MoveSimpleSpriteObject(SimpleSpriteObject *pSSObj)
{
    BOOL bReflected = FALSE;
    NNSG2dSVec2 *pPos = &(pSSObj->sprite.pos);
    struct Speed *pSpeed = &(pSSObj->speed);

    // 位置の更新
    pPos->x += pSpeed->x;
    pPos->y += pSpeed->y;

    // 画面端で反射する
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

    // 反射時にα値を変更する
    if( bReflected )
    {
        pSSObj->sprite.alpha = GetAlphaRandom();
    }
}


/*---------------------------------------------------------------------------*
  Name:         DrawDirect

  Description:  パラメータを直接指定するAPI を利用した描画。
                描画APIはUV値を指定して、テクスチャ付四角ポリゴンを描画するだけなので、
                3D グラフィックエンジンの設定をユーザが行う必要がある。
                もっとも高速に動作する。
  

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void DrawDirect()
{
    G3_PushMtx();
    {
        int i;
        //------------------------------------------------------------------------------
        // 3Dグラフィックスエンジン：テクスチャ関連パラメータの設定
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
        // 描画：パラメータを直接引数に指定して描画するAPI
        for( i = 0;i < NUM_OF_SIMPLE_SPRITE; i++ )
        {
            //
            // アルファ値がゼロなら描画しません
            //
            if( sBalls[i].sprite.alpha == 0 )
            {
                continue;
            }
            
            //
            // アルファ値の設定
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

  Description:  通常の描画関数による描画
  

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void DrawNormal()
{
    //
    // 通常の描画関数による描画
    //
    // アトリビュートを設定します
    NNS_G2dSetSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA | NNS_G2D_SPRITEATTR_TEXTURE );
    NNS_G2dDrawSpriteSimple( &(sBalls[0].sprite) );
    // スプライト毎に変化するパラメータのみを有効に設定します。
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

  Description:  NNS_G2dDrawSprite*Fast関数を使用した描画。
                カレント行列を描画関数コールの前後で保存しないので、高速に動作します。
  

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void DrawFast()
{
    G3_PushMtx();
        
        {
            int i;
            
            NNS_G2dSetSpriteAttrEnable( NNS_G2D_SPRITEATTR_ALPHA | NNS_G2D_SPRITEATTR_TEXTURE );
            NNS_G2dDrawSpriteSimple( &(sBalls[0].sprite) );
            // スプライト毎に変化するパラメータのみを有効に設定します。
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

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
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
        // キー操作
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
            // スプライト位置の更新
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
        // デバック情報の表示
        // 
        {
            G2DDemo_PrintOutf( 0, 21, "TIME:%06ld usec\n", OS_TicksToMicroSeconds(time) );
            G2DDemo_PrintOut ( 0, 22, MyDrawTypeStr[sDrawType] );
        }
        
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();
        
        // 表示情報
        G2DDemo_PrintApplyToHW();
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

