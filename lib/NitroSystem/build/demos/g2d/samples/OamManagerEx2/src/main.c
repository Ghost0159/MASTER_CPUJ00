/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - OamManagerEx2
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
//      拡張OAMマネージャを使用して、セルアニメーションを描画するサンプルです。
//      拡張OAMマネージャとOAMマネージャを組み合わせて使用します。
//
//  操作方法
//      A               爆発アニメーションを再生します。
//
// ============================================================================


#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


//-----------------------------------------------------------------------------
//
// エフェクト構造体
//
struct MyEffectObject;
typedef struct MyEffectObject
{
    
    NNSG2dCellAnimation*        pCellAnm;   // セルアニメーション
    NNSG2dFVec2                 pos;        // 位置
    BOOL                        bActive;    // アクティブかどうか？
    struct MyEffectObject*      pNext;      // 次のエフェクト
    
}MyEffectObject;


//-----------------------------------------------------------------------------
#define LENGTH_EFFECT_ARRAY     32              // エフェクト構造体のプール用配列の長さ

#define NUM_OF_OAM              16              // OAMマネージャ管理対象のOAM数
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)  // OAMマネージャ管理対象のAffineパラメータ数

#define TEX_BASE                0x0             // テクスチャベースアドレス
#define TEX_PLTT_BASE           0x0             // テクスチャパレットベースアドレス


#define NUM_OF_OAM_CHUNK        256             // OAMチャンク数
                                                // (拡張OAMマネージャに登録可能なOAMの最大数)
#define NUM_OF_CHUNK_LIST      (u8) 1           // プライオリティ
#define NUM_OF_AFFINE_PROXY     1               // アフィンプロクシ数
                                                // (拡張OAMマネージャに登録可能な
                                                //  アフィンパラメータの最大数)

//-----------------------------------------------------------------------------
// グローバル変数

static MyEffectObject    effectArray_[LENGTH_EFFECT_ARRAY]; // エフェクト構造体のプール用配列

static MyEffectObject*   pActiveEffectListHead = NULL;      // アクティブ状態であるエフェクトのリスト
static MyEffectObject*   pFreeEffectListHead   = NULL;      // 非アクティブ状態であるエフェクトのリスト

// チャンク
// OAM を格納するリストのためのコンテナです。
static NNSG2dOamChunk           oamChunk_[NUM_OF_OAM_CHUNK];

// チャンクリスト
// 表示優先度ごとのチャンクリストを保持します。
// 利用する表示優先度の数だけ必要です。
static NNSG2dOamChunkList       oamChunkList_[NUM_OF_CHUNK_LIST];

// Affineパラメータプロキシ
// Affine パラメータを格納し HW 反映時まで AffineIndex の決定を遅延させます。
// このデモでは利用していません。
static NNSG2dAffineParamProxy   affineProxy_[NUM_OF_AFFINE_PROXY];


//
// OAMマネージャ、拡張OAMマネージャ
//
static NNSG2dOamManagerInstance         oamManager_;
static NNSG2dOamManagerInstanceEx       oamExManager_;


    
//------------------------------------------------------------------------------
// プロトタイプ宣言


/*---------------------------------------------------------------------------*
  Name:         CallBackFunction

  Description:  アニメーションコントローラからコールバックされる関数です。
                
                パラメータには、アニメーションが再生しているエフェクト構造体
                へのポインタが設定されています。
                コールバック関数内では、そのエフェクト構造体のアクティブフラグを
                FALSEに設定しています。

  Arguments:    param:          コールバック関数登録時に指定した
                                ユーザパラメータです。
                currentFrame:   コールバックタイプが
                                NNS_G2D_ANMCALLBACKTYPE_LAST_FRM の場合は
                                アニメーションコントローラの currentTime が
                                fx32 型で渡されます。
                                それ以外では u16 型の現在のフレーム番号が
                                渡されます。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void CallBackFunction( u32 param, fx32 currentFrame )
{
#pragma unused( currentFrame )
    
    MyEffectObject*     pE = (MyEffectObject*)param;
    pE->bActive = FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         CallBackGetOamCapacity

  Description:  拡張 OAM マネージャからコールバックされ、拡張 OAM マネージャに
                利用可能な OAM 数を伝えます。

  Arguments:    なし。

  Returns:      利用できる OAM 数。
 *---------------------------------------------------------------------------*/
static u16 CallBackGetOamCapacity(void)
{
    return NNS_G2dGetOamManagerOamCapacity( &oamManager_ );
}



/*---------------------------------------------------------------------------*
  Name:         CallBackGetAffineCapacity

  Description:  拡張 OAM マネージャからコールバックされ、拡張 OAM マネージャに
                利用可能な Affine パラメータ格納領域数を伝えます。

  Arguments:    なし。

  Returns:      利用できる Affine パラメータ格納領域数。
 *---------------------------------------------------------------------------*/
static u16 CallBackGetAffineCapacity(void)
{
    return 0;
}



/*---------------------------------------------------------------------------*
  Name:         CallBackEntryNewOam

  Description:  拡張 OAM マネージャからコールバックされ、OAM を出力します。

  Arguments:    pOam:   出力すべき OAM へのポインタ。
                index:  pOam の OBJ ナンバー

  Returns:      OAM の出力に成功したかどうか。
 *---------------------------------------------------------------------------*/
static BOOL CallBackEntryNewOam(const GXOamAttr* pOam, u16 index )
{
#pragma unused( index )
    return NNS_G2dEntryOamManagerOam( &oamManager_, pOam, 1 );
}



/*---------------------------------------------------------------------------*
  Name:         CallBackEntryNewAffine

  Description:  拡張 OAM マネージャからコールバックされ、Affine パラメータを
                登録します。

  Arguments:    pMtx:   登録すべき Affine パラメータ。
                index:  pMtx の AffineIndex。

  Returns:      実際に登録した pMtx の AffineIndex。
 *---------------------------------------------------------------------------*/
static u16 CallBackEntryNewAffine( const MtxFx22* pMtx, u16 index )
{
#pragma unused( pMtx )
#pragma unused( index )
	// この関数は常に無効な値を返します
    return NNS_G2D_OAMEX_HW_ID_NOT_INIT;
}

//------------------------------------------------------------------------------
static void InitApp()
{
    BOOL        result = TRUE;
    
    //
    // OAMマネージャを初期化します
    //
    {
        // OAMマネージャ モジュール の初期化
        NNS_G2dInitOamManagerModule();
        // OAMマネージャ 実体 の初期化
        result &= NNS_G2dGetNewOamManagerInstanceAsFastTransferMode( &oamManager_, 
                                                                     0, 
                                                                     NUM_OF_OAM,
                                                                     NNS_G2D_OAMTYPE_MAIN );    
    }
    
    
    // 拡張 OAM マネージャ実体の初期化
    {
        NNSG2dOamExEntryFunctions       funcs;// !
        
        result &= NNS_G2dGetOamManExInstance( &oamExManager_, 
                                                oamChunkList_, NUM_OF_CHUNK_LIST,
                                                NUM_OF_OAM_CHUNK, oamChunk_,
                                                NUM_OF_AFFINE_PROXY, affineProxy_ );

        // NNS_G2dApplyOamManExToBaseModule() 呼び出しでコールバックされる関数群を登録します。
        funcs.getOamCapacity    = CallBackGetOamCapacity;
        funcs.getAffineCapacity = CallBackGetAffineCapacity;
        funcs.entryNewOam       = CallBackEntryNewOam;
        funcs.entryNewAffine    = CallBackEntryNewAffine;

        NNS_G2dSetOamManExEntryFunctions( &oamExManager_, &funcs );
    }
    SDK_ASSERT( result );
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  以下の初期化を行います。
                  セルアニメーションデータの読み込み
                  セルアニメーションの初期化
                  キャラクタデータ、パレットデータの読み込み

  Arguments:    ppAnimBank: セルアニメーションデータへのポインタを
                            受け取るポインタ。
                ppCellAnim: セルアニメーションへのポインタを受け取るポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadResources_( )
{
    //
    // Cell 関連初期化
    //
    {
        void* pBuf;
        
        NNSG2dCellDataBank*          pCellBank  = NULL;
        NNSG2dAnimBankData*          pAnimBank  = NULL;
        
        // セルデータおよびセルアニメーションデータをロード。
        // セルデータおよびセルアニメーションデータは最後まで
        // メインメモリ上で使用するのでこれらの pBuf は解放しません。
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/bomb.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/bomb.NANR" );
        SDK_NULL_ASSERT( pBuf );
        SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0) );
        
        //
        // セルアニメーションの実体を初期化します
        //
        {
            
            NNSG2dCellAnimation*         pCellAnm = NULL;
            int i = 0;
            
            for( i = 0; i < LENGTH_EFFECT_ARRAY; i++ )
            {
                effectArray_[i].pCellAnm = G2DDemo_GetNewCellAnimation(1);
                pCellAnm = effectArray_[i].pCellAnm;
                
                
                SDK_NULL_ASSERT( pCellAnm );
                
                NNS_G2dInitCellAnimation( pCellAnm, NNS_G2dGetAnimSequenceByIdx( pAnimBank, 0), pCellBank );
                
                //
                // コールバックの設定
                //
                NNS_G2dSetAnimCtrlCallBackFunctor(
                    NNS_G2dGetCellAnimationAnimCtrl(pCellAnm),
                    NNS_G2D_ANMCALLBACKTYPE_LAST_FRM,
                    (u32)&effectArray_[i],
                    CallBackFunction );
            }
        }
    }

    //
    // VRAM 関連の初期化
    //
    {
        void* pBuf;

        // load character data for 2D
        {
            NNSG2dCharacterData*        pCharData;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/bomb.NCGR" );
            SDK_NULL_ASSERT( pBuf );
            
            // Loading For 2D Graphics Engine.
            DC_FlushRange( pCharData->pRawData, pCharData->szByte );
            GX_LoadOBJ( (void*)pCharData->pRawData, TEX_BASE, pCharData->szByte );
                
            // キャラクタデータを VRAM にコピーしたので
            // この pBuf は開放します。以下同じ。
            G2DDemo_Free( pBuf );
        }

        //------------------------------------------------------------------------------
        // load palette data
        {
            NNSG2dPaletteData*        pPlttData;

            pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/bomb.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pPlttData->pRawData, pPlttData->szByte );
            GX_LoadOBJPltt( (void*)pPlttData->pRawData, TEX_PLTT_BASE, pPlttData->szByte );
            
            G2DDemo_Free( pBuf );
        }
    }
}

//------------------------------------------------------------------------------
// シーンのエフェクト登録リストをリセットします
static void ResetEffectList_()
{
    int i;
    for( i = 0; i < LENGTH_EFFECT_ARRAY - 1; i++ )
    {
        effectArray_[i].bActive = FALSE;
        effectArray_[i].pNext = &effectArray_[i+1];
        NNS_G2dSetCellAnimationCurrentFrame( effectArray_[i].pCellAnm, 0 );
    }
    effectArray_[LENGTH_EFFECT_ARRAY - 1].bActive = FALSE;
    effectArray_[LENGTH_EFFECT_ARRAY - 1].pNext = NULL;
    
    pActiveEffectListHead   = NULL;
    pFreeEffectListHead     = effectArray_;
}

//------------------------------------------------------------------------------
// 新しいエフェクトをシーンに追加します
static void AddNewEffect_()
{
    MyEffectObject*             pE = pFreeEffectListHead;
    if( pE )
    {
        SDK_ASSERT( pE->bActive == FALSE );
        
        pE->bActive = TRUE;
        pE->pos.x = (fx32)((OS_GetTick() % 255) * FX32_ONE);
        pE->pos.y = (fx32)((OS_GetTick() % 192) * FX32_ONE);
        
        
        // フリーリストから取り出し
        pFreeEffectListHead = pE->pNext;
        
        // アクティブリスト先頭に追加
        pE->pNext             = pActiveEffectListHead;
        pActiveEffectListHead = pE;
    }
}

//------------------------------------------------------------------------------
// シーンを更新します。
static void TickScene_()
{
    MyEffectObject*             pE = pActiveEffectListHead;
    MyEffectObject*             pPreE = NULL;
    MyEffectObject*             pNext = NULL;
    
    
    while( pE )
    {    
        // セルアニメーションの更新
        NNS_G2dTickCellAnimation( pE->pCellAnm, FX32_ONE );
        pE      = pE->pNext;
    }
    
    pE = pActiveEffectListHead;
    while( pE )
    {
        pNext = pE->pNext;
        if( !pE->bActive )    
        {    
            // アクティブリストから取り出し
            if( pPreE )
            {
                pPreE->pNext          = pNext;
            }else{
                pActiveEffectListHead = pNext;
            }
            // フリーリスト先頭に追加
            pE->pNext           = pFreeEffectListHead;
            pFreeEffectListHead = pE;
            
            pE      = pNext;
        }else{
            pPreE   = pE;
            pE      = pNext;
        }
    }   
}

//------------------------------------------------------------------------------
// ひとつの爆発エフェクトを描画します
static u16 DrawOneEffect_( const MyEffectObject* pE )
{
    {
        int i;
        u16                     numOamBuffer;       // 残り OAM 数
        static GXOamAttr        temp[NUM_OF_OAM];   // テンポラリ OAM バッファ
        const NNSG2dCellData*   pCell;              // 表示する Cell

        pCell = NNS_G2dGetCellAnimationCurrentCell( pE->pCellAnm );
        SDK_NULL_ASSERT( pCell );

        //
        // Cell と等価な OBJリストをテンポラリに書き出します
        //
        numOamBuffer = NNS_G2dMakeCellToOams(
                            temp,           // 出力先Oamバッファ
                            NUM_OF_OAM,     // 出力先バッファ長
                            pCell,          // 出力対象 Cell
                            NULL,           // Affine 変換
                            &pE->pos,       // オフセット位置
                            NULL,           // Affine Index
                            FALSE );        // 倍角Affineか？

        SDK_ASSERT( numOamBuffer < NUM_OF_OAM);

        for( i = 0; i < numOamBuffer; i++ )
        {
            if( !NNS_G2dEntryOamManExOam( &oamExManager_, &temp[i], 0 ) )
            {   
                return (u16)i;
            }
        }
        return numOamBuffer;
    }
    
}

//------------------------------------------------------------------------------
// シーンを描画します
// 使用したOAMの数が返り値となります。
//
static u16 DrawScene_()
{
    u16     totalOam = 0;
    MyEffectObject*     pE;
    pE = pActiveEffectListHead;
    while( pE )
    {
        totalOam += DrawOneEffect_( pE );
        pE = pE->pNext;
    }
    
    return totalOam;
}

//------------------------------------------------------------------------------
// 爆発エフェクトの数を数えます
static u16 GetNumEffect_( MyEffectObject* pListHead )
{
    u16 count = 0;
    MyEffectObject*     pE = pListHead;
    while( pE )
    {
        count++;
        pE = pE->pNext;
    }
    return count;
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        
        InitApp();
        LoadResources_();
        ResetEffectList_();
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
        static OSTick               time;                           // OAMマネージャパフォーマンス計測に使用します。
        u16     totalOam = 0;
        //
        // Pad handling.
        //
        {
            G2DDemo_ReadGamePad();
            // シーンに 新たに エフェクトを 追加します
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_A ))
            {
                AddNewEffect_();
            }
            
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_B ))
            {
                ResetEffectList_();
            }
            
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_L ))
            {
                OS_Printf( "num-active = %d\n", GetNumEffect_( pActiveEffectListHead ) );
                OS_Printf( "num-free   = %d\n", GetNumEffect_( pFreeEffectListHead ) );
            }
        }
        
        // シーンを描画します
        // 使用したOAMの数が返り値となります。
        totalOam = DrawScene_();
        

        //
        // デバック文字列描画
        //
        {
            G2DDemo_PrintOutf( 0, 0, "Total-OAM:%d", totalOam );
        }
        
        //
        // マネージャの内容をHWに書き出します
        // また、その際にかかった時間を計測し、デバック情報として出力します。
        //            
        {
            // V Blank 待ち
            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
            SVC_WaitVBlankIntr();
            
            time = OS_GetTick();
            {
                NNS_G2dApplyOamManExToBaseModule( &oamExManager_ );
                NNS_G2dResetOamManExBuffer( &oamExManager_ );
                
                NNS_G2dApplyOamManagerToHW( &oamManager_ );
                NNS_G2dResetOamManagerBuffer( &oamManager_ );
            }
            time = OS_GetTick() - time;
        }
        
        //
        // アニメーション情報をHWに書き出します   
        //
        G2DDemo_PrintApplyToHW();   
        
        //
        // シーンを更新します。
        // 
        TickScene_();   
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

