/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamManagerEx3
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
//  デモの内容
//      拡張 OAM マネージャとレンダラを組み合わせて使用するサンプルです。
//      ・OBJ を時分割で表示し、見かけ上、実際の使用OAM数 より多くの OBJを表示します。
//      ・OBJの描画順指定を用い表示優先度つきで OBJ を表示します。
//      ・同一の表示優先度が指定された場合の、アフィン変換されたOAMと通常のOAMの描画順ルールについて示します。
//      ・時分割表示のアルゴリズムについて、使用するハードウェアリソース量(OBJ,アフィンパラメータ の数)
//        やプライオリティを変化させながら示します。
//
//  操作方法
//      十字キー左右    キャラクタを左右に動かします。
//      十字キー上下    キャラクタを前後に動かします。
//      A               OMAマネージャが使用するOBJ の数を増やします。
//      B               OMAマネージャが使用するOBJ の数を減らします。
//      X               OMAマネージャが使用するアフィンパラメータ の数を増やします。
//      Y               OMAマネージャが使用するアフィンパラメータ の数を減らします。
//      START           拡張OAMマネージャの描画順番の切替えをします。          
//      SELECT          右端のセルの描画プライオリティをその他2つのセルとは異なる値に指定します。
//
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"

// 画面の大きさ
#define SCREEN_WIDTH            256
#define SCREEN_HEIGHT           192




#define NUM_OF_OAM_CHUNK        32   // 拡張OAMマネージャの持つOAM チャンク数
#define NUM_OF_AFFINE_PROXY     32   // 拡張OAMマネージャの持つアフィンパラメータプロクシの数
#define NUM_OF_ORDERING_TBL     2    // 拡張OAMマネージャの持つ オーダリングテーブル長
                                     // (=指定可能なプライオリティ数)
                                     

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

//-----------------------------------------------------------------------------
// 拡張OAMマネージャ
static NNSG2dOamManagerInstanceEx       myOamMgrEx_;
// チャンク
// OAM を格納するリストのためのコンテナです。
static NNSG2dOamChunk                   sOamChunk[NUM_OF_OAM_CHUNK];
// チャンクリスト
// 表示優先度ごとのチャンクリストを保持します。
// 利用する表示優先度の数だけ必要です。
static NNSG2dOamChunkList               sOamChunkList[NUM_OF_ORDERING_TBL];
// Affineパラメータプロキシ
// Affine パラメータを格納し HW 反映時まで AffineIndex の決定を遅延させます。
static NNSG2dAffineParamProxy           sAffineProxy[NUM_OF_AFFINE_PROXY];



//-----------------------------------------------------------------------------
// OAMマネージャ
static NNSG2dOamManagerInstance         myOamMgr_;

//-----------------------------------------------------------------------------
// レンダラ関連
static NNSG2dRendererInstance           myRenderer_;
static NNSG2dRenderSurface              myRenderSurface_;
static NNSG2dImageProxy                 myImageProxy_;
static NNSG2dImagePaletteProxy          myPaletteProxy_;


//-----------------------------------------------------------------------------
// 
// セルアニメーション
static NNSG2dCellAnimation*             pMyCellAnim_;     

// 拡張OAMマネージャ登録時に使用される
// 表示優先度
static u8                               oamPriority_ = 0; 

// 拡張OAMマネージャが使用するOAMアトリビュート数
static u16 numOamReservedForManager_           =   8;     
// 拡張OAMマネージャが使用するアフィンパラメータ数
static u16 numOamAffineReservedForManager_     =   1;     

// 拡張OAMマネージャによって使用されたOAMの数
static u16 numOamUsedByManager_         = 0;              
// 拡張OAMマネージャによって使用されたアフィンパラメータの数
static u16 numOamAffineUsedByManager_   = 0;              

//-----------------------------------------------------------------------------
// プロトタイプ宣言
void NitroMain(void);
void VBlankIntr(void);

//------------------------------------------------------------------------------
// レンダラが使用する、OAM登録関数
//
// 拡張OAMマネージャのOAM登録関数を呼び出します。
//
static BOOL RndrCBFuncEntryOam_
( 
    const GXOamAttr* pOam, 
    u16 affineIndex, 
    BOOL /*bDoubleAffine*/ 
)
{
    SDK_NULL_ASSERT( pOam );
    
    numOamUsedByManager_++;
    
    if( NNS_G2D_OAM_AFFINE_IDX_NONE == affineIndex )
    {
        return NNS_G2dEntryOamManExOam( &myOamMgrEx_, pOam, oamPriority_ );
    }else{
        return NNS_G2dEntryOamManExOamWithAffineIdx( &myOamMgrEx_, pOam, oamPriority_, affineIndex );
    }
}

//------------------------------------------------------------------------------
// レンダラが使用する、OAMアフィンパラメータ登録関数
//
// 拡張OAMマネージャのOAMアフィンパラメータ登録関数を呼び出します。
//
static u16 RndrCBFuncEntryOamAffine_( const MtxFx22* mtx )
{
    SDK_NULL_ASSERT( mtx );
    
    numOamAffineUsedByManager_++;
    
    return NNS_G2dEntryOamManExAffine( &myOamMgrEx_, mtx );
}




//------------------------------------------------------------------------------
// 拡張OAMマネージャが外部モジュールへOAMアトリビュートやアフィンパラメータ
// を登録するときに使用する関数群
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 拡張OAMマネージャが利用可能な、OAMアトリビュートの個数を取得します
// 本関数での、OAMアトリビュートの個数は、実際にハードウェアOAMにロード可能な個数を表します。
// OAMチャンクの個数(NUM_OF_OAM_CHUNK)とは異なる点にご注意ください。
// 
// 登録チャンク数 > OamMgrExCBGetCapacityOam_()返り値 のときに、拡張OAMマネージャは時分割表示を行います。
// 
static u16 OamMgrExCBGetCapacityOam_(void)
{
    return numOamReservedForManager_;
}
//------------------------------------------------------------------------------
// 拡張OAMマネージャが利用可能な、アフィンパラメータの個数を取得します
static u16 OamMgrExCBGetCapacityOamAffine_(void)
{
    return numOamAffineReservedForManager_;
}

static BOOL OamMgrExCBEntryOam_(const GXOamAttr* pOam, u16 /*index*/)
{
    return NNS_G2dEntryOamManagerOam( &myOamMgr_, pOam, 1 );
}

static u16 OamMgrExCBEntryOamAffine_( const MtxFx22* pMtx, u16 /*index*/ )
{
    return NNS_G2dEntryOamManagerAffine( &myOamMgr_, pMtx );
}



//------------------------------------------------------------------------------
static void LoadResources(void)
{
    NNSG2dCellDataBank* pCellBank;
    void*               pBuf;

    NNSG2dAnimBankData*          pAnimBank = NULL;
    
    NNS_G2dInitImageProxy( &myImageProxy_ );
    NNS_G2dInitImagePaletteProxy( &myPaletteProxy_ );

    //------------------------------------------------------------------------------
    // load cell data
    {
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/Renderer_2LCD.NCER" );
        SDK_NULL_ASSERT( pBuf );
        // セルバンクは最後までメインメモリ上で使用するので
        // この pBuf は開放しません。
    }

    //------------------------------------------------------------------------------
    // load animation data
    {
        // セルアニメーションデータは最後までメインメモリ上で
        // 使用するのでこの pBuf は開放しません。
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/Renderer_2LCD.NANR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // セルアニメーションの実体を初期化します
        //
        {
            pMyCellAnim_ = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( pMyCellAnim_ );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0 ) );

            NNS_G2dInitCellAnimation(
                pMyCellAnim_,
                NNS_G2dGetAnimSequenceByIdx(pAnimBank, 0),
                pCellBank );
        }
    }

    //------------------------------------------------------------------------------
    // load character data for 2D
    {
        NNSG2dCharacterData* pCharData;

        pBuf = G2DDemo_LoadNCGR( &pCharData, "data/Renderer_2LCD.NCGR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For Main 2D Graphics Engine.
        NNS_G2dLoadImage2DMapping(
            pCharData,
            CHARA_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &myImageProxy_ );

        // キャラクタデータを VRAM にコピーしたので
        // この pBuf は開放します。以下同じ。
        G2DDemo_Free( pBuf );
    }
    
    //------------------------------------------------------------------------------
    // load palette data
    {
        NNSG2dPaletteData* pPlttData;

        pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/Renderer_2LCD.NCLR" );
        SDK_NULL_ASSERT( pBuf );

        // Loading For Main 2D Graphics Engine.
        NNS_G2dLoadPalette(
            pPlttData,
            PLTT_BASE,
            NNS_G2D_VRAM_TYPE_2DMAIN,
            &myPaletteProxy_ );

        G2DDemo_Free( pBuf );
    }
}

//------------------------------------------------------------------------------
// アプリケーションの初期化処理を行います
static void InitApp_()
{
    
    //
    // 描画データ素材を読み込みます。また、VRAM管理を行うデータの初期化を行います。
    //
    LoadResources();
    
    //
    // 拡張OAMマネージャの初期化を行います。
    // 以下の例では、拡張OAMマネージャのOAM登録作業を下請けするモジュールとして、
    // OAMマネージャモジュールを使用しています。
    // そのため、OAMマネージャモジュールも初期化しています。
    //
    {
        BOOL                        bSuccess;
        

        //
        // OAMマネージャの初期化
        //
        NNS_G2dInitOamManagerModule();
        bSuccess = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode( &myOamMgr_, 0, 128, NNS_G2D_OAMTYPE_MAIN );
        SDK_ASSERT( bSuccess );

        

        // 拡張 OAM マネージャ実体の初期化
        bSuccess = NNS_G2dGetOamManExInstance(
                        &myOamMgrEx_,
                        sOamChunkList,
                        (u8)NUM_OF_ORDERING_TBL,
                        NUM_OF_OAM_CHUNK,
                        sOamChunk,
                        NUM_OF_AFFINE_PROXY,
                        sAffineProxy);

        SDK_ASSERT( bSuccess );
        
        // NNS_G2dApplyOamManExToBaseModule() 呼び出しでコールバックされる関数群を登録します。
        // 本サンプルでは、コールバック関数の実装にOAMマネージャAPIを利用しています。    
        {
            NNSG2dOamExEntryFunctions   funcs;
            funcs.getOamCapacity    = OamMgrExCBGetCapacityOam_;
            funcs.getAffineCapacity = OamMgrExCBGetCapacityOamAffine_;
            funcs.entryNewOam       = OamMgrExCBEntryOam_;
            funcs.entryNewAffine    = OamMgrExCBEntryOamAffine_;

            NNS_G2dSetOamManExEntryFunctions( &myOamMgrEx_, &funcs );
        }
    }
    
    //
    // レンダラモジュールを初期化します
    // 本サンプルでは、OAM登録処理を行うモジュールに拡張OAMマネージャを使用しています。
    //
    {
        // レンダラの初期化
        NNS_G2dInitRenderer( &myRenderer_ );
        NNS_G2dInitRenderSurface( &myRenderSurface_ );
        
        // メイン画面 Surface を初期化します
        {
            NNSG2dViewRect* pRect = &(myRenderSurface_.viewRect);

            // 表示矩形
            pRect->posTopLeft.x = 0;
            pRect->posTopLeft.y = 0;
            pRect->sizeView.x = FX32_ONE * SCREEN_WIDTH;
            pRect->sizeView.y = FX32_ONE * SCREEN_HEIGHT;

            // コールバック関数
            myRenderSurface_.pFuncOamRegister          = RndrCBFuncEntryOam_;
            myRenderSurface_.pFuncOamAffineRegister    = RndrCBFuncEntryOamAffine_;

            // 表示先
            myRenderSurface_.type                      = NNS_G2D_SURFACETYPE_MAIN2D;
        }
        
        NNS_G2dAddRendererTargetSurface( &myRenderer_, &myRenderSurface_ );
        
        // プロキシ登録
        NNS_G2dSetRendererImageProxy( &myRenderer_, &myImageProxy_, &myPaletteProxy_ );
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
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        
        InitApp_();
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
        static Position pos = { 64, 128 };

        static OSTick          timeForApply;
        static u16             rotation = 0x0;
        static u8              priority = 0;
        
        static NNSG2dOamExDrawOrder drawOrderType = NNSG2D_OAMEX_DRAWORDER_BACKWARD;
        
        // 入力の処理
        {
            G2DDemo_ReadGamePad();

            // キャラクタの移動
            if( G2DDEMO_IS_PRESS( PAD_KEY_LEFT ) )
            {
                pos.x--;
            }
            if( G2DDEMO_IS_PRESS( PAD_KEY_RIGHT ) )
            {
                pos.x++;
            }
            if( G2DDEMO_IS_PRESS( PAD_KEY_UP ) )
            {
                pos.y--;
            }
            if( G2DDEMO_IS_PRESS( PAD_KEY_DOWN ) )
            {
                pos.y++;
            }
            
            if( G2DDEMO_IS_PRESS( PAD_BUTTON_L ) )
            {
                rotation -= 0xFF;
            }
            if( G2DDEMO_IS_PRESS( PAD_BUTTON_R ) )
            {
                rotation += 0xFF;
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_START ) )
            {
                //
                // 拡張OAMマネージャの描画順番を切り替えます
                //
                if( drawOrderType == NNSG2D_OAMEX_DRAWORDER_BACKWARD )
                {   
                    drawOrderType = NNSG2D_OAMEX_DRAWORDER_FORWARD;
                }else{
                    drawOrderType = NNSG2D_OAMEX_DRAWORDER_BACKWARD;
                }
                NNSG2d_SetOamManExDrawOrderType( &myOamMgrEx_, drawOrderType );
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_A ) )
            {
                if( numOamReservedForManager_+1 < 127 )
                {
                    numOamReservedForManager_++;
                }
            }
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_B ) )
            {
                if( numOamReservedForManager_ > 1 )
                {
                    numOamReservedForManager_--;
                }
            }
            
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_X ) )
            {
                if( numOamAffineReservedForManager_+1 < 31)
                {
                    numOamAffineReservedForManager_++;
                }
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_Y ) )
            {
                if( numOamAffineReservedForManager_ > 0 )
                {
                    numOamAffineReservedForManager_--;
                }
            }
            
            if( G2DDEMO_IS_TRIGGER( PAD_BUTTON_SELECT ) )
            {
                if( priority == 0 )
                {
                    priority = 1;
                }else{
                    priority = 0;
                }
            }
        }
        
        // 描画
        {
            NNS_G2dBeginRendering( &myRenderer_ );
                NNS_G2dPushMtx();
                    NNS_G2dTranslate( FX32_ONE*pos.x, FX32_ONE*pos.y, 0 );                        
                    // 左
                    oamPriority_ = 0;
                    NNS_G2dTranslate( FX32_ONE*30, 0, 0 );
                    NNS_G2dDrawCellAnimation( pMyCellAnim_ );
                    
                    // 中央
                    oamPriority_ = 0;
                    NNS_G2dTranslate( FX32_ONE*30, 0, 0 );
                    NNS_G2dScale( FX32_ONE + (FX32_ONE >> 1), FX32_ONE + (FX32_ONE >> 1), 0 );
                    NNS_G2dDrawCellAnimation( pMyCellAnim_ );
                    
                    // 右
                    oamPriority_ = priority;
                    NNS_G2dTranslate( FX32_ONE*30, 0, 0 );
                    if( rotation != 0 )
                    {
                        NNS_G2dRotZ( FX_SinIdx( rotation ), FX_CosIdx( rotation ) );
                    }
                    NNS_G2dDrawCellAnimation( pMyCellAnim_ );
                NNS_G2dPopMtx();
            NNS_G2dEndRendering();
        }

        // デバック文字列出力
        {
            G2DDemo_PrintOutf(0, 0, "OAM(used/Max)=(%3d/%3d)", 
                                    numOamUsedByManager_, 
                                    numOamReservedForManager_ );
                                    
            G2DDemo_PrintOutf(0, 1, "AFF(used/Max)=(%3d/%3d)", 
                                    numOamAffineUsedByManager_, 
                                    numOamAffineReservedForManager_ );        
                  
            G2DDemo_PrintOutf(0, 2, "DrawOrderType = %s", 
                                    (drawOrderType == NNSG2D_OAMEX_DRAWORDER_BACKWARD) ?
                                    "Backward":
                                    "Forward " );        
        }
        
        // V Blank 待ち
        SVC_WaitVBlankIntr();

        //
        // バッファの内容をHWに書き出します
        //
        {
            timeForApply = OS_GetTick();
            // 表示情報を書き出します
            G2DDemo_PrintApplyToHW();

            
            // OBJを書き出します
            NNS_G2dApplyOamManExToBaseModule( &myOamMgrEx_ );

            // 拡張 OAM マネージャをリセットします。
            // リセットしても時分割表示に関する情報は維持されます。
            NNS_G2dResetOamManExBuffer( &myOamMgrEx_ );
            timeForApply = OS_GetTick() - timeForApply;
            
            NNS_G2dApplyOamManagerToHW( &myOamMgr_ );
            NNS_G2dResetOamManagerBuffer( &myOamMgr_ );
            
            numOamUsedByManager_         = 0;
            numOamAffineUsedByManager_   = 0;
        }
        
        // アニメーションの更新
        NNS_G2dTickCellAnimation( pMyCellAnim_, FX32_ONE );
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


