/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamManagerEx
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.11 $
 *---------------------------------------------------------------------------*/



// ============================================================================
//  デモの内容
//      拡張 OAM マネージャを利用して
//      ・128 個より多くの OBJ を時分割で表示します。
//      ・OBJの描画順指定を用い表示優先度つきで OBJ を表示します。
//
//  操作方法
//      十字キー左右    キャラクタを左右に動かします。
//      十字キー上下    キャラクタを前後に動かします。
//      A               OBJ の数を増やします。
//      B               OBJ の数を減らします。
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

// 画面の大きさ
#define SCREEN_WIDTH            256
#define SCREEN_HEIGHT           192

#define HW_OAM_USE              128     // HW OAM の利用する数

#define INIT_NUM_OF_LAYER       10      // 前後方向に並ぶバーの初期数
#define MAX_NUM_OF_LAYER        27      // 前後方向に並ぶバーの最大数
#define NUM_OF_LANE             10      // 左右方向に並ぶバーの数

#define NUM_OF_OAM_CHUNK        (MAX_NUM_OF_LAYER * NUM_OF_LANE + 1)
#define NUM_OF_CHUNK_LIST       (MAX_NUM_OF_LAYER + 1)
#define NUM_OF_AFFINE_PROXY     1

// キャラクタとバーの OBJ サイズ
#define BAR_WIDTH               32
#define BAR_HEIGHT              16
#define CHARA_WIDTH             32
#define CHARA_HEIGHT            32

// 前後方向のバーの表示位置の差
#define BAR_SHIFT_X             12      // X 方向に12ずれて
#define BAR_SHIFT_Y             6       // さらに Y 方向に6ずれる

// 左右方向のバーと表示位置の差
#define LANE_SHIFT              (BAR_WIDTH + 24)

// 全てのバーを内包する最小の矩形領域サイズ
#define DRAW_AREA_WIDTH         (BAR_SHIFT_X * (MAX_NUM_OF_LAYER - 1) + LANE_SHIFT * (NUM_OF_LANE - 1) + BAR_WIDTH)
#define DRAW_AREA_HEIGHT        (BAR_SHIFT_Y * (MAX_NUM_OF_LAYER - 1) + BAR_HEIGHT)

// 一番左下のバーの表示位置
// 他のバーの表示位置はこの値から計算で求められます。
#define BAR_OFFSET_X            ((SCREEN_WIDTH - DRAW_AREA_WIDTH) / 2)
#define BAR_OFFSET_Y            (((SCREEN_HEIGHT + DRAW_AREA_HEIGHT) / 2) + 10)

// キャラの初期表示位置
// 一番左下のバーを原点とします。
#define CHARA_INIT_POS_X        ((LANE_SHIFT * (NUM_OF_LANE - 1) + BAR_WIDTH) / 2)
#define CHARA_INIT_POS_Y        (- (((BAR_SHIFT_Y * (MAX_NUM_OF_LAYER - 1)) * BAR_SHIFT_X / BAR_SHIFT_Y) / 2))


// リソースのロードアドレスオフセット
#define CHARA_BASE              0x0000
#define PLTT_BASE               0x0000



//-----------------------------------------------------------------------------
// 構造体定義

// 位置を表す構造体
typedef struct Position
{
    s16 x;
    s16 y;
} Position;



//-----------------------------------------------------------------------------
// グローバル変数

// チャンク
// OAM を格納するリストのためのコンテナです。
static NNSG2dOamChunk           sOamChunk[NUM_OF_OAM_CHUNK];

// チャンクリスト
// 表示優先度ごとのチャンクリストを保持します。
// 利用する表示優先度の数だけ必要です。
static NNSG2dOamChunkList       sOamChunkList[NUM_OF_CHUNK_LIST];

// Affineパラメータプロキシ
// Affine パラメータを格納し HW 反映時まで AffineIndex の決定を遅延させます。
// このデモでは利用していません。
//static NNSG2dAffineParamProxy   sAffineProxy[NUM_OF_AFFINE_PROXY];


//-----------------------------------------------------------------------------
// プロトタイプ宣言

static u16 CallBackGetOamCapacity(void);
static void InitOamManagerEx(NNSG2dOamManagerInstanceEx* pExMan );
static void LoadResources(void);
static void InitTemplateOam( GXOamAttr* pChara, GXOamAttr* pBar );
static const Position*  GetPosition(void);
void NitroMain(void);
void VBlankIntr(void);



/*---------------------------------------------------------------------------*
  Name:         CallBackGetOamCapacity

  Description:  拡張 OAM マネージャからコールバックされ、拡張 OAM マネージャに
                利用可能な OAM 数を伝えます。

  Arguments:    なし。

  Returns:      利用できる OAM 数。
 *---------------------------------------------------------------------------*/
static u16 CallBackGetOamCapacity(void)
{
    return HW_OAM_USE;
}

/*---------------------------------------------------------------------------*
  Name:         CallBackEntryNewOam

  Description:  拡張 OAM マネージャからコールバックされ、OAM を出力します。

  Arguments:    pOam:   出力すべき OAM へのポインタ。
                index:  pOam の OBJ ナンバー

  Returns:      OAM の出力に成功したかどうか。
 *---------------------------------------------------------------------------*/
static BOOL CallBackEntryNewOam(const GXOamAttr* pOam, u16 index)
{
    const u32 szByte = sizeof( GXOamAttr );

    DC_FlushRange( (void*)pOam, szByte );
    GX_LoadOAM( pOam, index * szByte, szByte );

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         InitOamManagerEx

  Description:  拡張 OAM マネージャ実体を初期化します。

  Arguments:    pExMan:     初期化する拡張 OAM マネージャ実体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitOamManagerEx(NNSG2dOamManagerInstanceEx* pExMan )
{
    BOOL bSuccess;
    NNSG2dOamExEntryFunctions funcs;

    // 拡張 OAM マネージャのみ利用の場合は
    // NNS_G2dInitOamManagerModule() 
    // の呼び出しは不要です。
    // 拡張 OAM マネージャのコールバック関数の実装にOAMマネージャを使用
    // する場合には、NNS_G2dInitOamManagerModule()を実行してください。

    // 拡張 OAM マネージャ実体の初期化
    bSuccess = NNS_G2dGetOamManExInstance(
                    pExMan,
                    sOamChunkList,
                    (u8)NUM_OF_CHUNK_LIST,
                    NUM_OF_OAM_CHUNK,
                    sOamChunk,
                    0,                      // アフィンパラメータは管理しない
                    NULL);                  // アフィンパラメータは管理しない

    SDK_ASSERT( bSuccess );
    
    // 拡張OAMマネージャに登録関数を設定します。
    // 拡張OAMマネージャはデフォルトの登録関数のを持っていませんので、
    // 利用前に必ず本関数を実行して登録関数を設定する必要があります。
    {
        // NNS_G2dApplyOamManExToBaseModule() 呼び出しで
        // コールバックされる関数群を登録します。
        funcs.getOamCapacity    = CallBackGetOamCapacity;
        funcs.entryNewOam       = CallBackEntryNewOam;
        funcs.getAffineCapacity = NULL;
        funcs.entryNewAffine    = NULL;
        
        NNS_G2dSetOamManExEntryFunctions( pExMan, &funcs );
    }
}



/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  キャラクタデータとパレットデータをファイルから読み込み
                VRAM にロードします。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadResources(void)
{
    void* pBuf;

    //-------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/OamManagerEx.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        DC_FlushRange( pCharData->pRawData, pCharData->szByte );
        GX_LoadOBJ( pCharData->pRawData, CHARA_BASE, pCharData->szByte );

        // キャラクタデータを VRAM にコピーしたので
        // この pBuf は開放します。以下同じ。
        G2DDemo_Free( pBuf );
    }

    //-------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/OamManagerEx.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For 2D Graphics Engine.
        DC_FlushRange( pPlttData->pRawData, pPlttData->szByte );
        GX_LoadOBJPltt( pPlttData->pRawData, PLTT_BASE, pPlttData->szByte );

        G2DDemo_Free( pBuf );
    }
}



/*---------------------------------------------------------------------------*
  Name:         InitTemplateOam

  Description:  表示する OBJ の雛型になる OAM を作成します。

  Arguments:    pChara:     キャラクタ OBJ の雛型になる OAM へのポインタ。
                pBar:       バー OBJ の雛型になる OAM へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitTemplateOam( GXOamAttr* pChara, GXOamAttr* pBar )
{
    // 十字キーで動かせるキャラクタの雛型です
    G2_SetOBJAttr(
        pChara,                 // OAMデータ
        0,                      // X 座標
        0,                      // Y 座標
        0,                      // 表示優先順位
        GX_OAM_MODE_NORMAL,     // OBJ モード
        FALSE,                  // モザイク
        GX_OAM_EFFECT_NONE,     // エフェクト
        GX_OAM_SHAPE_32x32,     // 形状
        GX_OAM_COLORMODE_16,    // カラーモード
        0x204,                  // キャラクタネーム
        0x8,                    // カラーパラメータ
        0);                     // アフィンインデックス

    // 多数配置されるバーの雛型です
    G2_SetOBJAttr(
        pBar,                   // OAMデータ
        0,                      // X 座標
        0,                      // Y 座標
        0,                      // 表示優先順位
        GX_OAM_MODE_NORMAL,     // OBJ モード
        FALSE,                  // モザイク
        GX_OAM_EFFECT_NONE,     // エフェクト
        GX_OAM_SHAPE_32x16,     // 形状
        GX_OAM_COLORMODE_16,    // カラーモード
        0x200,                  // キャラクタネーム
        0xD,                    // カラーパラメータ
        0);                     // アフィンインデックス
}



/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  パッド入力を読み取り、キャラクタの位置とバーのレイヤー数を
                変更します。

  Arguments:    pPos:       キャラクタ位置として入力を反映させる
                            Position へのポインタ。
                pNumLayer:  バーのレイヤー数として入力を反映させる
                            s8 へのポインタ

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ProcessInput(Position* pPos, s8* pNumLayer)
{
    G2DDemo_ReadGamePad();

    // バー数の増減
    if( G2DDEMO_IS_PRESS( PAD_BUTTON_A ) )
    {
        (*pNumLayer)++;
    }
    if( G2DDEMO_IS_PRESS( PAD_BUTTON_B ) )
    {
        (*pNumLayer)--;
    }
    if( *pNumLayer > MAX_NUM_OF_LAYER )
    {
        *pNumLayer = MAX_NUM_OF_LAYER;
    }
    if( *pNumLayer < 0 )
    {
        *pNumLayer = 0;
    }

    // キャラクタの移動
    if( G2DDEMO_IS_PRESS( PAD_KEY_LEFT ) )
    {
        pPos->x--;
    }
    if( G2DDEMO_IS_PRESS( PAD_KEY_RIGHT ) )
    {
        pPos->x++;
    }
    if( G2DDEMO_IS_PRESS( PAD_KEY_UP ) )
    {
        pPos->y--;
    }
    if( G2DDEMO_IS_PRESS( PAD_KEY_DOWN ) )
    {
        pPos->y++;
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
    NNSG2dOamManagerInstanceEx  oamManagerEx;   // 拡張 OAM マネージャ
    GXOamAttr chara;                            // キャラクタ雛型 OAM
    GXOamAttr bar;                              // バー雛型 OAM


    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        LoadResources();
        InitOamManagerEx( &oamManagerEx );
        InitTemplateOam( &chara, &bar );
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
        // キャラクタ位置
        static Position pos = {CHARA_INIT_POS_X, CHARA_INIT_POS_Y};

        static s8 numLayer = INIT_NUM_OF_LAYER; // バーのレイヤー数
        u8 charaPriority;                       // キャラクタの表示優先度
        int numObjDrawn = 0;                    // 表示しているOBJ数
        static OSTick          timeForEntery, timeForApply;;
        


        // 入力の処理
        {
            ProcessInput(&pos, &numLayer);
        }


        // キャラの表示優先度を計算します
        {
            charaPriority = (u8)((- pos.y + BAR_SHIFT_X) / BAR_SHIFT_X);

            if( charaPriority >= MAX_NUM_OF_LAYER )
            {
                charaPriority = MAX_NUM_OF_LAYER;
            }
        }


        //
        // OAM マネージャへの登録
        //
        {
            BOOL bSuccess;
            int i, r;

            // キャラを OAM マネージャへ登録します
            {
                G2_SetOBJPosition(
                    &chara,
                    BAR_OFFSET_X + pos.x - pos.y,
                    BAR_OFFSET_Y - CHARA_HEIGHT + (pos.y * BAR_SHIFT_Y) / BAR_SHIFT_X
                );

                // 表示優先度つきで登録します
                bSuccess = NNS_G2dEntryOamManExOam( &oamManagerEx, &chara, charaPriority );
                SDK_ASSERT( bSuccess );

                numObjDrawn++;
            }

            // バーを OAM マネージャへ登録します
            for( i = 0; i < numLayer; ++i )
            {
                int x, y;               // バーの表示位置
                u8 priority = (u8)i;    // バーの表示優先度

                // 優先度がキャラの表示優先度と被らないようにする
                // 値が小さい方が優先度が高い（=前に表示される）
                if( priority >= charaPriority )
                {
                    priority++;
                }

                // 画面外のものは表示しない（Y）
                y = BAR_OFFSET_Y - BAR_HEIGHT - i * BAR_SHIFT_Y;

                if( y < - BAR_HEIGHT || SCREEN_HEIGHT <= y )
                {
                    continue;
                }

                timeForEntery = OS_GetTick();
                for( r = 0; r < NUM_OF_LANE; ++r )
                {
                    // 画面外のものは表示しない（X)
                    x = BAR_OFFSET_X + i * BAR_SHIFT_X + r * LANE_SHIFT;

                    if( x < - BAR_WIDTH || SCREEN_WIDTH <= x )
                    {
                        continue;
                    }

                    G2_SetOBJPosition(&bar, x, y);

                    // 表示優先度つきで登録します
                    bSuccess = NNS_G2dEntryOamManExOam( &oamManagerEx, &bar, priority );
                    SDK_ASSERT( bSuccess );

                    numObjDrawn++;
                }
                timeForEntery = OS_GetTick() - timeForEntery;
            }
        }


        // 表示情報を出力します
        {
            G2DDemo_PrintOutf( 0, 0, "Chara Position: %4d / %4d", pos.x, pos.y);
            G2DDemo_PrintOutf( 0, 1, "Chara Priority: %4d", charaPriority);
            G2DDemo_PrintOutf( 0, 2, "Number of OBJ:  %4d", numObjDrawn);
            
            G2DDemo_PrintOutf( 0, 3, "Entry:%06ld usec\n", OS_TicksToMicroSeconds(timeForEntery) );
            G2DDemo_PrintOutf( 0, 4, "Apply:%06ld usec\n", OS_TicksToMicroSeconds(timeForApply) );
        }


        // V Blank 待ち
        SVC_WaitVBlankIntr();


        //
        // バッファの内容をHWに書き出します
        //
        {
            // 表示情報を書き出します
            G2DDemo_PrintApplyToHW();

            timeForApply = OS_GetTick();
            // OBJを書き出します
            NNS_G2dApplyOamManExToBaseModule( &oamManagerEx );

            // 拡張 OAM マネージャをリセットします。
            // リセットしても時分割表示に関する情報は維持されます。
            NNS_G2dResetOamManExBuffer( &oamManagerEx );
            timeForApply = OS_GetTick() - timeForApply;
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
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );    // checking VBlank interrupt
}


