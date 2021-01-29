/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - CellAnimation
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
//      セルアニメーションを描画、更新するサンプル
//
//  操作方法
//      十字キー上下    再生速度変更
//      十字キー左右    再生シーケンス変更
//      A               再生<=>停止切り替え
//      B               順方向<=>逆方向再生切り替え
//      X               コールバックタイプ切り替え
//      Y               再生の再スタート
//  説明
//      セルアニメーションを再生します。また、さまざまな再生制御の機能、
//      コールバック関数呼び出し機能を示します。
// ============================================================================


#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


//------------------------------------------------------------------------------
// 各種定数
#define NUM_OF_OAM              48                      // OAMマネージャ管理対象のOAM数

#define ANIM_SPEED_UNIT         (FX32_ONE >> 3)         // アニメーション速度の変化差分

#define CALLBACK_PARAM          0x12345678              // コールバック関数のパラメータ
#define CALL_CALLBACK_FRAME     3                       // NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM の時に
                                                        // コールバック関数を呼び出すフレーム番号
                                                        
#define TEX_BASE                0x0                     // テクスチャベースアドレス
#define TEX_PLTT_BASE           0x0                     // テクスチャパレットベースアドレス

//------------------------------------------------------------------------------
// グローバルメンバ
static NNSG2dOamManagerInstance objOamManager;          // OBJ 出力用 OAM マネージャ

static NNSG2dImageProxy         sImageProxy;            // Cell 用キャラクタ/テクスチャプロキシ
static NNSG2dImagePaletteProxy  sPaletteProxy;          // Cell 用パレットプロキシ

// コールバックタイプ名
static const char* CALL_BACK_TYPE_NAME[] = {
    "NONE    ",     // NNS_G2D_ANMCALLBACKTYPE_NONE
    "LAST_FRM",     // NNS_G2D_ANMCALLBACKTYPE_LAST_FRM
    "SPEC_FRM",     // NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM
    "EVER_FRM",     // NNS_G2D_ANMCALLBACKTYPE_EVER_FRM
};

// Cell 表示位置
static const NNSG2dFVec2        CELL_POSITION = {120 << FX32_SHIFT, 120 << FX32_SHIFT};

// アニメーションコールバックが呼び出された回数
static int                      callCount = 0;

// アニメーションコールバックの種類
static NNSG2dAnmCallbackType    callBackType = NNS_G2D_ANMCALLBACKTYPE_NONE;


/*---------------------------------------------------------------------------*
  Name:         CallBackFunction

  Description:  アニメーションコントローラからコールバックされる関数です。

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
#pragma unused( param )
#pragma unused( currentFrame )

    // 呼ばれた回数を更新
    callCount++;
    
    OS_Printf("CallBackFunction is called: param=0x%p, frame=0x%p\n", param, currentFrame);
}

/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM マネージャシステムを初期化します。

  Arguments:    pObjManager:    OBJ 出力用に初期化するOAMマネージャへの
                                ポインタ。
  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void InitOamManager( NNSG2dOamManagerInstance* pObjManager )
{
    BOOL    result;

    // Oam Manager システムの初期化
    NNS_G2dInitOamManagerModule();

    
    result = NNS_G2dGetNewOamManagerInstanceAsFastTransferMode( pObjManager, 
                                                                0, 
                                                                NUM_OF_OAM, 
                                                                NNS_G2D_OAMTYPE_MAIN );    
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
static void LoadResources(NNSG2dAnimBankData** ppAnimBank, NNSG2dCellAnimation** ppCellAnim)
{
    void* pBuf;

    NNS_G2dInitImageProxy( &sImageProxy );
    NNS_G2dInitImagePaletteProxy( &sPaletteProxy );

    //
    // Cell 関連初期化
    //
    {
        NNSG2dCellDataBank*          pCellBank = NULL;

        // セルデータおよびセルアニメーションデータをロード。
        // セルデータおよびセルアニメーションデータは最後まで
        // メインメモリ上で使用するのでこれらの pBuf は解放しません。
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/CellAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( ppAnimBank, "data/CellAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // セルアニメーションの実体を初期化します
        //
        {
            *ppCellAnim = G2DDemo_GetNewCellAnimation(1);
            SDK_NULL_ASSERT( *ppCellAnim );

            SDK_NULL_ASSERT( NNS_G2dGetAnimSequenceByIdx(*ppAnimBank, 0) );

            NNS_G2dInitCellAnimation(
                *ppCellAnim,
                NNS_G2dGetAnimSequenceByIdx(*ppAnimBank, 0),
                pCellBank );
        }
    }

    //
    // VRAM 関連の初期化
    //
    {
        //------------------------------------------------------------------------------
        // load character data for 2D
        {
            NNSG2dCharacterData*        pCharData;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/CellAnimation.NCGR" );
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

            pBuf = G2DDemo_LoadNCLR( &pPlttData, "data/CellAnimation.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            DC_FlushRange( pPlttData->pRawData, pPlttData->szByte );
            GX_LoadOBJPltt( (void*)pPlttData->pRawData, TEX_PLTT_BASE, pPlttData->szByte );
            
            G2DDemo_Free( pBuf );

        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ChangeCallBackType

  Description:  コールバックタイプの変更を行います。

  Arguments:    pAnimCtrl:      コールバックタイプを変更するアニメーション
                                コントローラへのポインタ。
                callBackType:   新しいコールバックタイプ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ChangeCallBackType
(
    NNSG2dAnimController*   pAnimCtrl,
    NNSG2dAnmCallbackType   callBackType
)
{
    //
    // 初期化 = リセット
    //
    NNS_G2dInitAnimCtrlCallBackFunctor( pAnimCtrl );


    switch( callBackType )
    {
    case NNS_G2D_ANMCALLBACKTYPE_NONE:
        break;

    case NNS_G2D_ANMCALLBACKTYPE_LAST_FRM:
    case NNS_G2D_ANMCALLBACKTYPE_EVER_FRM:
        NNS_G2dSetAnimCtrlCallBackFunctor(
            pAnimCtrl,
            callBackType,
            CALLBACK_PARAM,
            CallBackFunction );
        break;

    case NNS_G2D_ANMCALLBACKTYPE_SPEC_FRM:
        NNS_G2dSetAnimCtrlCallBackFunctorAtAnimFrame(
            pAnimCtrl,
            CALLBACK_PARAM,
            CallBackFunction,
            CALL_CALLBACK_FRAME );
        break;

    default:
        SDK_ASSERT(FALSE);
        break;
    }
}

/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  パッド入力を読み込み、入力に応じてアニメーション
                パラメータを変更します。

  Arguments:    pIndex:         シーケンスインデックスへのポインタ。
                                左右キーによりシーケンスを切り替えます。
                pAnimSpeed:     アニメーションスピードへのポインタ。
                                上下キーによりスピードを変更します。
                pAnimCtrl:      アニメーションコントローラへのポインタ。
                                A / B ボタンにより 停止 / 反転 を操作します。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ProcessInput(u16* pIndex, fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl )
{
    G2DDemo_ReadGamePad();

    // change animation sequence.
    {
        if(G2DDEMO_IS_TRIGGER(PAD_KEY_RIGHT))
        {
            (*pIndex)++;
        }

        if(G2DDEMO_IS_TRIGGER(PAD_KEY_LEFT))
        {
            (*pIndex)--;
        }
    }

    // change animation speed.
    {
        if (G2DDEMO_IS_TRIGGER(PAD_KEY_UP))
        {
            *pAnimSpeed +=ANIM_SPEED_UNIT;
        }

        if (G2DDEMO_IS_TRIGGER(PAD_KEY_DOWN))
        {
            *pAnimSpeed -= ANIM_SPEED_UNIT;

            if( *pAnimSpeed < 0 )
            {
                *pAnimSpeed = 0;
            }
        }
    }

    // 再生制御
    {
        if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_B ))
        {
            pAnimCtrl->bReverse = ! pAnimCtrl->bReverse;
        }

        if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_A ))
        {
            pAnimCtrl->bActive = ! pAnimCtrl->bActive;
        }
    }

    // コールバック種類の変更
    if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_X) )
    {
        callBackType++;
        if( callBackType >= AnmCallbackType_MAX )
        {
            callBackType = NNS_G2D_ANMCALLBACKTYPE_NONE;
        }
        ChangeCallBackType(pAnimCtrl, callBackType);
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
    NNSG2dAnimBankData*         pAnimBank = NULL;   // Cellアニメーションデータ
    NNSG2dCellAnimation*        pCellAnim = NULL;   // CellAnimation 実体
    

    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        G2DDemo_CameraSetup();
        G2DDemo_MaterialSetup();
        InitOamManager( &objOamManager );
        LoadResources( &pAnimBank, &pCellAnim );
        
        // start display
        SVC_WaitVBlankIntr();
        GX_DispOn();
        GXS_DispOn();
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        static u16      currentSeqIdx = 0;       // 描画シーケンス番号
        static fx32     animSpeed     = FX32_ONE;// アニメーションスピード

        //-------------------------------------------------
        // パッド入力
        //
        {
            const u16 oldSeqIndex = currentSeqIdx;
            const u16 numSequence = NNS_G2dGetNumAnimSequence( pAnimBank );

            ProcessInput(
                &currentSeqIdx,
                &animSpeed,
                NNS_G2dGetCellAnimationAnimCtrl(pCellAnim) );

            if( (s16)currentSeqIdx >= numSequence )
            {
                currentSeqIdx= 0;
            }

            if( (s16)currentSeqIdx < 0 )
            {
                currentSeqIdx = (u16)(numSequence - 1);
            }
             
            //
            // 変更があれば適用します
            //
            if( oldSeqIndex != currentSeqIdx )
            {
                const NNSG2dAnimSequence* pSeq = NULL;

                pSeq = NNS_G2dGetAnimSequenceByIdx( pAnimBank, currentSeqIdx );
                SDK_NULL_ASSERT( pSeq );
                NNS_G2dSetCellAnimationSequence( pCellAnim, pSeq );
                NNS_G2dStartAnimCtrl( NNS_G2dGetCellAnimationAnimCtrl( pCellAnim ) );
            }
            
            // 再生の再スタート
            if( G2DDEMO_IS_TRIGGER(PAD_BUTTON_Y) )
            {
                NNS_G2dRestartCellAnimation( pCellAnim );
            }
        }
        
        
        //-------------------------------------------------
        // 描画
        //
        {
            u16                     numOamDrawn = 0;  // 描画された OAM 数
            static GXOamAttr        temp[NUM_OF_OAM]; // テンポラリ OAM バッファ
            const NNSG2dCellData*   pCell = NULL;     // 表示する Cell

            pCell = NNS_G2dGetCellAnimationCurrentCell( pCellAnim );
            SDK_NULL_ASSERT( pCell );

            //
            // Cell と等価な OBJリストをテンポラリに書き出します
            //
            numOamDrawn = NNS_G2dMakeCellToOams(
                                temp,           // 出力先Oamバッファ
                                NUM_OF_OAM,     // 出力先バッファ長
                                pCell,          // 出力対象 Cell
                                NULL,           // Affine 変換
                                &CELL_POSITION, // オフセット位置
                                NULL,           // Affine Index
                                FALSE );        // 倍角Affineか？

            SDK_ASSERT( numOamDrawn < NUM_OF_OAM);

            // OAM マネージャへOBJリストを登録します
            (void)NNS_G2dEntryOamManagerOam( &objOamManager, temp, numOamDrawn );
        }

        //-------------------------------------------------
        // デバック文字列描画
        //
        {
            const NNSG2dAnimController* pAnimCtrl = NNS_G2dGetCellAnimationAnimCtrl(pCellAnim);
            
            G2DDemo_PrintOutf(0, 0, "sequence: %3d / %3d", currentSeqIdx, NNS_G2dGetNumAnimSequence( pAnimBank ));
            G2DDemo_PrintOutf(0, 1, "speed:    %7.3f", (float)animSpeed / FX32_ONE );
            G2DDemo_PrintOutf(0, 2, "frame:    %3d / %3d",
                pAnimCtrl->pCurrent - pAnimCtrl->pAnimSequence->pAnmFrameArray,
                pAnimCtrl->pAnimSequence->numFrames );
            G2DDemo_PrintOutf(0, 3, "time:     %3d / %3d",
                pAnimCtrl->currentTime >> FX32_SHIFT,
                pAnimCtrl->pCurrent->frames);
            G2DDemo_PrintOut(22, 0, "Active:");
            G2DDemo_PrintOut(29, 1, NNS_G2dIsAnimCtrlActive(pAnimCtrl) ? "yes": " no");
            G2DDemo_PrintOut(22, 2, "Reverse:");
            G2DDemo_PrintOut(29, 3, pAnimCtrl->bReverse ? "yes": " no");
            
            G2DDemo_PrintOutf(0, 21, "callback type:  %s",
                CALL_BACK_TYPE_NAME[callBackType]);
            G2DDemo_PrintOutf(0, 22, "callback count: %4d", 0);
            
            G2DDemo_PrintOutf(16, 22, "%5d", callCount);
        }
        
        //-------------------------------------------------
        // V Blank 待ち
        G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
        SVC_WaitVBlankIntr();
            
        //-------------------------------------------------
        // マネージャの内容をHWに書き出します
        //            
        {
            NNS_G2dApplyOamManagerToHW( &objOamManager );
            NNS_G2dResetOamManagerBuffer( &objOamManager );
        }

        //-------------------------------------------------
        // デバック情報文字列をHWに書き出します   
        //
        G2DDemo_PrintApplyToHW();   
        
        //-------------------------------------------------
        // アニメーションを更新します。
        // 
        {
            NNS_G2dTickCellAnimation( pCellAnim, animSpeed );
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

