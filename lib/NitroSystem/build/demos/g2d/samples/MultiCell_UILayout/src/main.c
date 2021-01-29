/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - MultiCell_UILayout
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  デモの内容
//      マルチセルを応用して、ゲームアプリケーションにおけるUI(ユーザインタフェース)
//      を処理するサンプルです。
//
//  操作方法
//      十字キー    カーソルの移動
//      
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

// バイナリコンバータ(-lblオプション)によって出力された、
// アニメーションシーケンス番号の別名を定義したヘッダファイル
#include "../data/win_pnl_NANR_LBLDEFS.h"
#include "g2d_demolib.h"


#define NUM_OF_OAM              128                 // OAMマネージャ管理対象のOAM数
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)      // OAMマネージャ管理対象のAffineパラメータ数

// マルチセルの位置
#define MY_MC_BASE_POS_X        128
#define MY_MC_BASE_POS_Y        96
// MultiCell 表示位置
static const NNSG2dFVec2           MULTICELL_POSITION 
            = {MY_MC_BASE_POS_X << FX32_SHIFT, MY_MC_BASE_POS_Y << FX32_SHIFT};
            
            
// ユーザが定義した、セルアニメ中のアニメフレーム番号の意味
#define APP_UI_ANIMFRAME_IDX_NORMAL 0   // 通常の状態
#define APP_UI_ANIMFRAME_IDX_PUSHED 1   // ボタンが押し込まれた状態


//
// MultiCellAnimation 実体
// 異なる2つの方式で実体を初期化します。
// 
static NNSG2dMultiCellAnimation   mcAnim;


// OAMアトリビュート一時バッファ
static GXOamAttr                tempOamBuffer[NUM_OF_OAM];

//
// マルチセルバンク
//
static NNSG2dMultiCellDataBank*        pMCBank = NULL;     // マルチセルデータ
static NNSG2dCellDataBank*             pCellBank = NULL;   // セルデータ
static NNSG2dCellAnimBankData*         pAnimBank = NULL;   // セルアニメーション
static NNSG2dMultiCellAnimBankData*    pMCABank = NULL;    // マルチセルアニメーション

// ポインタの位置
static NNSG2dSVec2          pointerPos_ = { 0, 0 };
static BOOL                 bHit_       = FALSE;


//------------------------------------------------------------------------------
// プロトタイプ宣言

static void InitOamManager(NNSG2dOamManagerInstance* pOamManager);
static void LoadResources();
static void ProcessInput(fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl);
void VBlankIntr(void);



/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAMマネージャシステムとOAMマネージャの初期化を行います。

  Arguments:    pOamManager:    初期化するOAMマネージャへのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager)
{
    BOOL    result;

    // Oam Manager システムの初期化
    NNS_G2dInitOamManagerModule();

    result = NNS_G2dGetNewManagerInstance(
                pOamManager,
                0,
                NUM_OF_OAM - 1,
                NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( result );

    result = NNS_G2dInitManagerInstanceAffine( pOamManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( result );
}



/*---------------------------------------------------------------------------*
  Name:         InitMultiCellInstance_

  Description:  マルチセル実体を初期化します。
  
  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitMultiCellInstance_
(
    NNSG2dMultiCellAnimation*       pMCAnim,
    NNSG2dCellAnimBankData*         pAnimBank,
    NNSG2dCellDataBank*             pCellBank,
    NNSG2dMultiCellDataBank*        pMCBank,
    NNSG2dMultiCellAnimBankData*    pMCABank,
    NNSG2dMCType                    mcType
)
{
    // 初期化に必要なワークメモリのサイズを取得します
    const u32 szWork = NNS_G2dGetMCWorkAreaSize( pMCBank, mcType );
    void* pWorkMem   = G2DDemo_Alloc( szWork );
    
    // 初期化
    NNS_G2dInitMCAnimationInstance( pMCAnim, 
                                    pWorkMem, 
                                    pAnimBank, 
                                    pCellBank, 
                                    pMCBank, 
                                    mcType );
    
    SDK_NULL_ASSERT( pMCAnim );
    
    // マルチセルアニメーションに再生するシーケンスをセット
    {
        const NNSG2dMultiCellAnimSequence* pSequence = NULL;    
        // 再生するシーケンスを取得
        pSequence = NNS_G2dGetAnimSequenceByIdx( pMCABank, 0 );
        SDK_ASSERT( pSequence );

        NNS_G2dSetAnimSequenceToMCAnimation( pMCAnim, pSequence);
    }
}

/*---------------------------------------------------------------------------*
  Name:         IsRectIntersect_

  Description:  矩形が点と衝突しているかどうか判定します。

  Arguments:    pRect:          判定対象となる矩形
                
                pPointerPos：   判定対象となる点
                
                pRectPos：      矩形の位置

  Returns:      衝突しているか？衝突している場合TRUEが返ります。
 *---------------------------------------------------------------------------*/
static BOOL IsRectIntersect_
( 
    const NNSG2dCellBoundingRectS16*    pRect,
    const NNSG2dSVec2*                  pPointerPos, 
    const NNSG2dSVec2*                  pRectPos 
)
{
    NNSG2dSVec2 posPtr = (*pPointerPos);
    
    posPtr.x -= pRectPos->x; 
    posPtr.y -= pRectPos->y; 
    
    if( pRect->maxX >= posPtr.x && 
        pRect->minX <= posPtr.x )
    {
        if( pRect->maxY >= posPtr.y && 
            pRect->minY <= posPtr.y )
        {
            // 衝突している
            return TRUE;        
        }
    }
    // 衝突していない
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         OnButtonHitCallBack_

  Description:  UIボタンの衝突判定処理を行うコールバック関数です。
                NNS_G2dTraverseMCNodes()への引数として使用されます。
                
                本関数はマルチセル中のすべてのノードに対して呼び出される
                コールバック関数として使用されます。

  Arguments:    userParamater:   ユーザパラメータ、
                                 本サンプルでは衝突が起きたかを記憶する
                                 フラグ変数へのポインタ値を格納しています。
                
                pNodeData：      ノード情報。セルアニメーション中のセルアニメの
                                 位置情報などが格納されています。
                
                pCellAnim：      セルアニメーション(セルアニメーション中の)
                
                nodeIdx：        ノード番号

  Returns:      以降のコールバック呼び出しを中止するかどうか？
                以降のコールバック呼び出しを中止したい場合は、FALSEを返します。
 *---------------------------------------------------------------------------*/
static BOOL OnButtonHitCallBack_
( 
    u32                                 userParamater,
    const NNSG2dMultiCellHierarchyData* pNodeData,
    NNSG2dCellAnimation*                pCellAnim, 
    u16                                 nodeIdx 
)
{
#pragma unused( nodeIdx )

    NNSG2dSVec2       rectPos;
    
    const NNSG2dCellData*       pCD      = NNS_G2dGetCellAnimationCurrentCell( pCellAnim );
    const NNSG2dAnimController* pAnmCtrl = NNS_G2dGetCellAnimationAnimCtrl( pCellAnim );
    const NNSG2dAnimSequence*   pAnmSeq  = NNS_G2dGetAnimCtrlCurrentAnimSequence( pAnmCtrl );
    const u16                   seqIdx   = NNS_G2dGetAnimSequenceIndex( pAnimBank, pAnmSeq );
    
    const NNSG2dCellBoundingRectS16* pRect = NNS_G2dGetCellBoundingRect( pCD );
    
    rectPos.x = (s16)(pNodeData->posX + MY_MC_BASE_POS_X);
    rectPos.y = (s16)(pNodeData->posY + MY_MC_BASE_POS_Y);
    
    // 衝突判定処理を行うべきセルアニメか？
    // 
    // ../data/win_pnl_NANR_LBLDEFS.h に記述されているアニメーションシーケンス番号
    // の define を利用しています。
    // このデータを出力するには、コンバータの -lbl オプションを指定します。
    if( seqIdx != NANR_win_pnl_Win_bse )
    {
        if( IsRectIntersect_( pRect, &pointerPos_, &rectPos ) )
        {
            // ユーザパラメータとして、フラグへのポインタが渡されています
            // ここでは、フラグの更新を行います。
            BOOL* pHitFlag = (BOOL*)userParamater;
            OS_Printf("hit index = %d\n", seqIdx );
            *pHitFlag = TRUE;
            
            // セルアニメの画像を押された状態に設定します
            NNS_G2dSetCellAnimationCurrentFrame( pCellAnim, APP_UI_ANIMFRAME_IDX_PUSHED );            
            
            
            // 以降のコールバック呼び出しを中止する。
            return FALSE;            
        }else{
            // セルアニメの画像を通常の状態に設定します
            NNS_G2dSetCellAnimationCurrentFrame( pCellAnim, APP_UI_ANIMFRAME_IDX_NORMAL );
        }
    }
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  マルチセルアニメーションの構築とキャラクタデータ、
                パレットデータの読み込みを行います。

  Arguments:    ppMCAnim:   構築したマルチセルアニメーションへのポインタを
                            受け取るポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void LoadResources()
{
    void* pBuf;

    //
    // MultiCell 関連初期化
    //
    {
        
        // セルデータ、セルアニメーション、マルチセルデータ、
        // マルチセルアニメーションをロード。
        // これらは最後までメインメモリ上で使用するので pBuf を解放しません。
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/win_pnl.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/win_pnl.NANR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMCR( &pMCBank, "data/win_pnl.NMCR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMAR( &pMCABank, "data/win_pnl.NMAR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // マルチセルアニメーションの実体を初期化します
        // 異なる方式で2つの実体を初期化します。
        //
        InitMultiCellInstance_( &mcAnim,
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_SHARE_CELLANIM  );
    }

    //
    // VRAM 関連の初期化
    //
    {
        // Loading NCG (Character Data)
        {
            NNSG2dCharacterData*        pCharData = NULL;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/win_pnl.NCGR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pCharData->pRawData, pCharData->szByte );
            GX_LoadOBJ( (void*)pCharData->pRawData, 0, pCharData->szByte );

            // キャラクタデータを VRAM にコピーしたので
            // この pBuf は開放します。以下同じ。
            G2DDemo_Free( pBuf );
        }

        // Loading NCL (Palette Data)
        {
            NNSG2dPaletteData*        pPltData = NULL;

            pBuf = G2DDemo_LoadNCLR( &pPltData, "data/win_pnl.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pPltData->pRawData, pPltData->szByte );
            GX_LoadOBJPltt( (void*)pPltData->pRawData, 0, pPltData->szByte );

            G2DDemo_Free( pBuf );
        }
    }
}


//--------------------------------------------------------------------------------------------------------
// アプリケーションの描画
static void AppDraw
( 
    const NNSG2dMultiCellAnimation* pMcAnim, 
    NNSG2dOamManagerInstance*       pOamMgr 
)
{
    u16                             numOamDrawn = 0;                  
    // ポインタの描画
    {
        const NNSG2dCellData* pCD = NNS_G2dGetCellDataByIdx( pCellBank, 0xd );
        NNSG2dFVec2           pointerPos; 
        
        pointerPos.x = pointerPos_.x << FX32_SHIFT;
        pointerPos.y = pointerPos_.y << FX32_SHIFT;
        
        //
        // MultiCell と等価な OBJリストをテンポラリに書き出します
        //
        numOamDrawn = NNS_G2dMakeCellToOams(
                            tempOamBuffer,              // 出力先Oamバッファ
                            (u16)(NUM_OF_OAM - numOamDrawn),   // 出力先バッファ長
                            pCD,                        // 出力対象 Cell
                            NULL,                       // Affine 変換
                            &pointerPos,                // オフセット位置
                            NULL,                       // Affine Index
                            FALSE );                    // 倍角Affineか？

        SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

        // OAMマネージャへOBJリストを登録します
        (void)NNS_G2dEntryOamManagerOam( pOamMgr, tempOamBuffer, numOamDrawn );
    }
    
    // マルチセル描画
    //
    // 低レベルな描画方法で MultiCell を描画します。
    // 上位モジュールの G2dRenderer を 利用する描画方法もあります。
    //
    //
    {
        
        
        const NNSG2dMultiCellInstance*  pMultiCell = &(pMcAnim->multiCellInstance); // 表示する MultiCell
        SDK_NULL_ASSERT( pMultiCell );

        //
        // MultiCell と等価な OBJリストをテンポラリに書き出します
        //
        numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                            tempOamBuffer,       // 出力先Oamバッファ
                            NUM_OF_OAM,          // 出力先バッファ長
                            pMultiCell,          // 出力対象 MultiCell
                            NULL,                // Affine 変換
                            &MULTICELL_POSITION, // オフセット位置
                            NULL,                // Affine Index
                            FALSE );             // 倍角Affineか？

        SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

        // OAMマネージャへOBJリストを登録します
        (void)NNS_G2dEntryOamManagerOam( pOamMgr, tempOamBuffer, numOamDrawn );
    }
    
    
}



   

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  メイン関数です。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    NNSG2dOamManagerInstance    oamManager;     // Oam マネージャ実体
    
        
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        LoadResources();
        
        // start display
        {
            SVC_WaitVBlankIntr();
            GX_DispOn();
            GXS_DispOn();
        }
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        NNSG2dMultiCellAnimation*      pMcAnim      = &mcAnim;
        
        //
        //  ------ Pad handling.
        //
        {
            G2DDemo_ReadGamePad();

            // change animation speed.
            {
                if (G2DDEMO_IS_PRESS(PAD_KEY_UP))
                {
                    pointerPos_.y -= 1;
                }
                
                if (G2DDEMO_IS_PRESS(PAD_KEY_DOWN))
                {
                    pointerPos_.y += 1;
                }
                
                if (G2DDEMO_IS_PRESS(PAD_KEY_LEFT))
                {
                    pointerPos_.x -= 1;
                }
                
                if (G2DDEMO_IS_PRESS(PAD_KEY_RIGHT))
                {
                    pointerPos_.x += 1;
                }
            }
        }
        
        //  ------ 衝突判定
        // マルチセル内の各ノードに対して、衝突判定と、
        // 衝突時のリアクション処理を行うコールバック呼び出しを行います。
        // ユーザパラメータとして、フラグへのポインタを渡し、コールバック内部で、衝突が起こった場合に
        // フラグを更新します。
        bHit_       = FALSE;
        NNS_G2dTraverseMCNodes( &pMcAnim->multiCellInstance, OnButtonHitCallBack_, (u32)&bHit_ );

        //  ------ 描画
        AppDraw( pMcAnim, &oamManager );
        
        
        
        // ------ デバック文字列を描画します
        {
            if( bHit_ )
            {
                G2DDemo_PrintOutf(0, 0, "Hit");
            }else{
                G2DDemo_PrintOutf(0, 0, "   ");
            }
        }

        // ------ V Blank 待ち
        SVC_WaitVBlankIntr();
        G2DDemo_PrintApplyToHW();   // デバック文字列情報をHWに書き出します

        // ------ マネージャの内容をHWに書き出します
        {
            NNS_G2dApplyOamManagerToHW          ( &oamManager );
            NNS_G2dResetOamManagerBuffer        ( &oamManager );
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

