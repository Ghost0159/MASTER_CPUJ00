/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamSoftwareSpriteDraw
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
//  デモの内容
//      Oam ソフトウエアスプライト描画のサンプル
//      各方式においての描画負荷の違いを比較します。
//      NUM_OBJ_X * NUM_OBJ_Y 枚のスプライトを描画します。
//  操作方法
//
//      A ボタン        表示する OBJ を切り替えます(切り替えるOBJが存在するならば)。
//      B ボタン        描画方式を切り替えます
//      上下左右        描画 スプライト数を増減します。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

//------------------------------------------------------------------------------
// 各種定数の定義
#define AFFINE_IDX                  3                       // Cell 用 Affine Index
#define CELL_ROTATE_UNIT            0xFF                    // Cell の単位回転量
#define CELL_SCALE_UNIT             ((FX32_ONE >> 7) * 3)   // Cell の単位拡大縮小率
                                                            // scale 値が 0 付近の値にならないようにしてあります
#define CELL_INIT_POS_X             (120 << FX32_SHIFT)     // Cell の初期位置 X
#define CELL_INIT_POS_Y             (120 << FX32_SHIFT)     // Cell の初期位置 Y
#define NUM_OBJ_X                   48                       
#define NUM_OBJ_Y                   32
#define STEP_OBJ                    10

#define INIT_OUTPUT_TYPE            NNS_G2D_OAMTYPE_MAIN    // 出力方法の初期値

#define NUM_OF_OAM                  128                     // OAM マネージャに割り当てる OAM の数
#define NUM_OF_AFFINE               (NUM_OF_OAM / 4)        // OAM マネージャに割り当てる Affine パラメータの数

#define TEX_BASE                    0x0                     // テクスチャベースアドレス
#define TEX_PLTT_BASE               0x0                     // テクスチャパレットベースアドレス



//------------------------------------------------------------------------------
// 構造体定義



//------------------------------------------------------------------------------
// グローバル変数

static NNSG2dImageProxy         sImageProxy;    // Cell 用キャラクタ/テクスチャプロキシ
static NNSG2dImagePaletteProxy  sPaletteProxy;  // Cell 用パレットプロキシ

//------------------------------------------------------------------------------
// プロトタイプ宣言

void VBlankIntr(void);
static NNSG2dCellDataBank* LoadResources(void);

//------------------------------------------------------------------------------
// 構造体定義

typedef struct CellInfo
{
    u16          cellIdx;   // 描画 Cell 番号
    u16          rotate;    // 回転角
    NNSG2dFVec2  scale;     // 拡大率
    NNSG2dFVec2  pos;       // Cell 描画位置

} CellInfo;

//------------------------------------------------------------------------------
// 描画種類
typedef enum MyDrawMetod
{
    MyDrawMetod_Normal,         // 通常
    MyDrawMetod_Fast,           // 高速
    MyDrawMetod_UsingCache,     // キャッシュ利用、高速
    MyDrawMetod_Max
}MyDrawMetod;

const char* strDrawMetod[]=
{
    "Normal    ",
    "Fast      ",
    "UsingCache",
    "NG",
};

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  セルバンク、キャラクタデータ、パレットデータをファイルから
                読み込み、キャラクタデータとパレットデータを
                それぞれVRAMにロードします。

  Arguments:    なし。

  Returns:      読み込んだセルバンクへのポインタ。
 *---------------------------------------------------------------------------*/
static NNSG2dCellDataBank* LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/OamSoftwareSpriteDraw.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }


    // load character data for 3D (software sprite)
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCBR( &pCharData, "data/OamSoftwareSpriteDraw.NCBR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            TEX_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sImageProxy );

        G2DDemo_Free( pBuf );
    }

    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/OamSoftwareSpriteDraw.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &sPaletteProxy);

        // Loading For 3D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            TEX_PLTT_BASE,
            NNS_G2D_VRAM_TYPE_3DMAIN,
            &sPaletteProxy);

        G2DDemo_Free( pBuf );
    }

    return pCellBank;
}



/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    NNSG2dCellDataBank*         pCellBank;      // セルデータバンク

    // Initialize App.
    {
        BOOL result = TRUE;

        G2DDemo_CommonInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        G2DDemo_PrintInit();
        
        pCellBank = LoadResources();
        G2_SetBG1Priority( 0 );
        G2_SetBG0Priority( 1 );
    }

    // start display
    {
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        static CellInfo cellInfo = 
        {
            0,                                  // 描画 Cell 番号
            0,                                  // 回転角
            {FX32_ONE, FX32_ONE},               // 拡大率
            {CELL_INIT_POS_X, CELL_INIT_POS_Y}  // Cell 描画位置
        };
        static NNSG2dOamType        outputType = INIT_OUTPUT_TYPE;  // Cell 出力方法
        static OSTick time;
        static MyDrawMetod          drawMethod = MyDrawMetod_Normal;
        static int numX = 24, numY = 16;
        //
        // キー操作
        //
        {
            G2DDemo_ReadGamePad();

            // Change current Cell
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_A) )
            {
                cellInfo.cellIdx++;
                if( cellInfo.cellIdx >= pCellBank->numCells )
                {
                    cellInfo.cellIdx = 0;
                }
            }
            
            if ( G2DDEMO_IS_TRIGGER(PAD_BUTTON_B) )
            {
                drawMethod++;
                drawMethod %= MyDrawMetod_Max;
            }
            
            if ( G2DDEMO_IS_PRESS(PAD_KEY_UP) )
            {
                numY--;
            }
            if ( G2DDEMO_IS_PRESS(PAD_KEY_DOWN) )
            {
                numY++;
            }
            if ( G2DDEMO_IS_PRESS(PAD_KEY_LEFT) )
            {
                numX--;
            }
            if ( G2DDEMO_IS_PRESS(PAD_KEY_RIGHT) )
            {
                numX++;
            }
            
        }   

        if( NUM_OBJ_X < numX )
        {
            numX = NUM_OBJ_X;
        }
        
        if( 0 > numX )
        {
            numX = 0;
        }
        
        if( NUM_OBJ_Y < numY )
        {
            numY = NUM_OBJ_Y;
        }
        
        if( 0 > numY )
        {
            numY = 0;
        }  

        // 描画
        //
        // OBJを描画します。
        // また、描画処理にかかった時間を計測します。
        //
        //
        time = OS_GetTick();
        {
            GXOamAttr      tempOam;
            int i, j;
            
            // 0番目のOBJを描画します
            NNS_G2dCopyCellAsOamAttr( NNS_G2dGetCellDataByIdx( pCellBank, cellInfo.cellIdx ), 0, &tempOam );
            NNS_G2dSetupSoftwareSpriteCamera();
            
            switch( drawMethod )
            {
            //------------------------------------------------------------------------------
            // 通常の描画
            case MyDrawMetod_Normal:
                for( i = 0; i < numX; i ++ )
                {
                    for( j = 0;j < numY; j++ )
                    {
                        NNS_G2dDrawOneOam3DDirectWithPos( (s16)(STEP_OBJ*i), (s16)(STEP_OBJ*j), (s16)(-1), &tempOam, 
                                           &sImageProxy.attr,
                                           NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN ),
                                           NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) );
                    }
                }
                break;
            //------------------------------------------------------------------------------
            // 高速描画：カレント行列のスタックへの退避、復元を行わない
            case MyDrawMetod_Fast:
                G3_PushMtx();
                for( i = 0; i < numX; i ++ )
                {
                    for( j = 0;j < numY; j++ )
                    {
                        // カレント行列が保存されないので、カレント行列の初期化が描画前に必要である。
                        G3_Identity();
                        NNS_G2dDrawOneOam3DDirectWithPosFast( (s16)(STEP_OBJ*i), (s16)(STEP_OBJ*j), (s16)(-1), &tempOam, 
                                           &sImageProxy.attr,
                                           NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN ),
                                           NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) );
                    }
                }
                G3_PopMtx(1);
                break;
            //------------------------------------------------------------------------------
            // キャッシュ利用高速描画：最も高速、ただし同一の画像を持つOBJしか描画できない
            // （サイズは異なってもよい）                
            case MyDrawMetod_UsingCache: 
                NNS_G2dSetOamSoftEmuSpriteParamCache( &tempOam, 
                                           &sImageProxy.attr,
                                           NNS_G2dGetImageLocation( &sImageProxy, NNS_G2D_VRAM_TYPE_3DMAIN ),
                                           NNS_G2dGetImagePaletteLocation( &sPaletteProxy, NNS_G2D_VRAM_TYPE_3DMAIN ) );    
                G3_PushMtx();
                for( i = 0; i < numX; i ++ )
                {
                    for( j = 0;j < numY; j++ )
                    {
                        // カレント行列が保存されないので、カレント行列の初期化が描画前に必要である。
                        G3_Identity();
                        NNS_G2dDrawOneOam3DDirectUsingParamCacheFast( (s16)(STEP_OBJ*i), (s16)(STEP_OBJ*j), (s16)(-1), &tempOam );
                    }
                }
                G3_PopMtx(1);
                break;
            }
        }
        time = OS_GetTick() - time;

        // 表示情報の出力
        {
            
            G2DDemo_PrintOutf( 0, 21, "TIME:%06ld usec\n", OS_TicksToMicroSeconds(time) );
            G2DDemo_PrintOut ( 0, 22, strDrawMetod[drawMethod] );
            G2DDemo_PrintOutf( 0, 23, "NUM_SPRITE:%04ld usec\n", numX * numY );
        }

        //
        // マネージャの内容をHWに書き出します
        //
        {
            // V Blank 待ち
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
            SVC_WaitVBlankIntr();
            
            // 表示情報
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

