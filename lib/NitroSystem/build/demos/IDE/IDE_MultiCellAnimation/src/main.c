/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_MultiCellAnimation
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
//      マルチセルアニメーションを描画、更新するサンプル
//      異なる方式で実体を初期化し、パフォーマンスを比較することができます。
//
//  操作方法
//      十字キー上下    再生速度変更
//      A               再生<=>停止切り替え
//      B               順方向<=>逆方向再生切り替え
//      X               マルチセルアニメーションの実体の種類を変更する
//      Y               マルチセルアニメーションのリセット
// ============================================================================

// プリコンパイルヘッダを使用する。
// #include <nitro.h>
// #include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define NUM_OF_OAM              128                  // OAMマネージャ管理対象のOAM数
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)      // OAMマネージャ管理対象のAffineパラメータ数

#define ANIM_SPEED_UNIT         (FX32_ONE >> 3)     // アニメーション速度の変化差分

//
// マルチセル実体の種類
// pMCAnim_ 配列にアクセスする際に使用されます
//
typedef enum 
{
    MCANIM_SHARE_CELLANIM = 0x0,
    MCANIM_DONOT_SHARE_CELLANIM = 0x1
    
}MyMCAnimType;

static const char* mcAnimTypeStr [] =
{
    "     ShareCellAnim",
    "DoNotShareCellAnim"
};

static const int CvtTblToNNSG2dMCType [] = 
{
    NNS_G2D_MCTYPE_SHARE_CELLANIM,
    NNS_G2D_MCTYPE_DONOT_SHARE_CELLANIM
};


// Cell 表示位置
static const NNSG2dFVec2           CELL_POSITION = {120 << FX32_SHIFT, 150 << FX32_SHIFT};
//
// MultiCellAnimation 実体
// 異なる2つの方式で実体を初期化します。
// 
static NNSG2dMultiCellAnimation   mcAnim[2];


// OAMアトリビュート一時バッファ
static GXOamAttr                tempOamBuffer[NUM_OF_OAM];

//
// マルチセルバンク
//
static NNSG2dMultiCellDataBank*        pMCBank = NULL;     // マルチセルデータ


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
        NNSG2dCellDataBank*             pCellBank = NULL;   // セルデータ
        NNSG2dCellAnimBankData*         pAnimBank = NULL;   // セルアニメーション
        NNSG2dMultiCellAnimBankData*    pMCABank = NULL;    // マルチセルアニメーション

        // セルデータ、セルアニメーション、マルチセルデータ、
        // マルチセルアニメーションをロード。
        // これらは最後までメインメモリ上で使用するので pBuf を解放しません。
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/MultiCellAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/MultiCellAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMCR( &pMCBank, "data/MultiCellAnimation.NMCR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMAR( &pMCABank, "data/MultiCellAnimation.NMAR" );
        SDK_NULL_ASSERT( pBuf );

        
        
        //
        // マルチセルアニメーションの実体を初期化します
        // 異なる方式で2つの実体を初期化します。
        //
        InitMultiCellInstance_( &mcAnim[MCANIM_SHARE_CELLANIM],
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_SHARE_CELLANIM  );

        
        InitMultiCellInstance_( &mcAnim[MCANIM_DONOT_SHARE_CELLANIM],
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_DONOT_SHARE_CELLANIM  );
    }

    //
    // VRAM 関連の初期化
    //
    {
        // Loading NCG (Character Data)
        {
            NNSG2dCharacterData*        pCharData = NULL;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/MultiCellAnimation.NCGR" );
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

            pBuf = G2DDemo_LoadNCLR( &pPltData, "data/MultiCellAnimation.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pPltData->pRawData, pPltData->szByte );
            GX_LoadOBJPltt( (void*)pPltData->pRawData, 0, pPltData->szByte );

            G2DDemo_Free( pBuf );
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ProcessInput

  Description:  パッド入力を読み込み、入力に応じてアニメーション
                パラメータを変更します。

  Arguments:    pAnimSpeed: アニメーションスピードへのポインタ。
                            上下キーによりスピードを変更します。
                pAnimCtrl:  アニメーションコントローラへのポインタ。
                            A / B ボタンにより 停止 / 反転 を操作します。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void ProcessInput(fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl)
{
    G2DDemo_ReadGamePad();

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
}
//--------------------------------------------------------------------------------------------------------
//マルチセルの描画
static void AppDraw
( 
    const NNSG2dMultiCellAnimation* pMcAnim, 
    NNSG2dOamManagerInstance*       pOamMgr 
)
{
    // 描画
    //
    // 低レベルな描画方法で MultiCell を描画します。
    // 上位モジュールの G2dRenderer を 利用する描画方法もあります。
    //
    //
    {
        u16                             numOamDrawn = 0;                  // 残り OAM 数
        
        const NNSG2dMultiCellInstance*  pMultiCell = &(pMcAnim->multiCellInstance); // 表示する MultiCell
        SDK_NULL_ASSERT( pMultiCell );

        //
        // MultiCell と等価な OBJリストをテンポラリに書き出します
        //
        numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                            tempOamBuffer,  // 出力先Oamバッファ
                            NUM_OF_OAM,     // 出力先バッファ長
                            pMultiCell,     // 出力対象 MultiCell
                            NULL,           // Affine 変換
                            &CELL_POSITION, // オフセット位置
                            NULL,           // Affine Index
                            FALSE );        // 倍角Affineか？

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
    MyMCAnimType                mcAnimType = MCANIM_SHARE_CELLANIM;
        
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
        static fx32                    animSpeed    = FX32_ONE; // アニメーションスピード
        static OSTick                  time         = 0;
        NNSG2dMultiCellAnimation*      pMcAnim      = &mcAnim[mcAnimType];
        
        //
        //  ------ Pad handling.
        //
        {
            ProcessInput(&animSpeed, NNS_G2dGetMCAnimAnimCtrl(pMcAnim));
            
            // ---- マルチセルアニメーション実体の種類を切り替えます。
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_X ))
            {
                if( mcAnimType == MCANIM_SHARE_CELLANIM )
                {
                    mcAnimType = MCANIM_DONOT_SHARE_CELLANIM;
                }else{
                    mcAnimType = MCANIM_SHARE_CELLANIM;
                }
            }
            
            // ---- リセット
            if (G2DDEMO_IS_PRESS(PAD_BUTTON_Y ))
            {
                NNS_G2dRestartMCAnimation( pMcAnim );
            }
        }

        //  ------ 描画
        //
        // 下位レベルの描画APIを利用して MultiCell を描画します。
        // 上位モジュールの G2dRenderer を 利用する描画方法もあります。
        //
        //
        AppDraw( pMcAnim, &oamManager );
        
        
        //  ------ アニメーションの更新
        // 描画されている方の更新にかかった時間を計測します
        {
            u32 i;
            for( i = 0; i < 2; i++ )
            {
                if( i == mcAnimType ) 
                {
                    time = OS_GetTick();
                        NNS_G2dTickMCAnimation( &mcAnim[i], animSpeed );
                    time = OS_GetTick()- time;
                }else{
                        NNS_G2dTickMCAnimation( &mcAnim[i], animSpeed );
                }
            }
        }
        
        
        // ------ デバック文字列を描画します
        {
            const NNSG2dAnimController* pAnimCtrl = NNS_G2dGetMCAnimAnimCtrl(pMcAnim);

            G2DDemo_PrintOutf(0, 0, "speed: %7.3f", (float)animSpeed / FX32_ONE );
            G2DDemo_PrintOutf(0, 1, "frame: %3d / %3d",
                pAnimCtrl->pCurrent - pAnimCtrl->pAnimSequence->pAnmFrameArray,
                pAnimCtrl->pAnimSequence->numFrames );
            
            G2DDemo_PrintOutf( 0, 2, "TIMEForUpdate     :%06ld usec", OS_TicksToMicroSeconds(time) );
            
            // 現在の実体の初期化に必要なワークメモリのサイズ
            G2DDemo_PrintOutf( 0, 3, "szWorkMem:%06ld byte", 
                               NNS_G2dGetMCWorkAreaSize( pMCBank, 
                               (NNSG2dMCType)CvtTblToNNSG2dMCType[ mcAnimType ] ) );
                               
            // 現在の実体の種類
            G2DDemo_PrintOutf( 0, 4, "Type     :%s", mcAnimTypeStr[mcAnimType] );
            
            G2DDemo_PrintOut(19, 0, "Active:");
            G2DDemo_PrintOut(28, 0, NNS_G2dIsAnimCtrlActive(pAnimCtrl) ? "yes": " no");
            G2DDemo_PrintOut(19, 1, "Reverse:");
            G2DDemo_PrintOut(28, 1, pAnimCtrl->bReverse ? "yes": " no");
        }

        // ------ V Blank 待ち
        SVC_WaitVBlankIntr();
        G2DDemo_PrintApplyToHW();   // アニメーション情報をHWに書き出します

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

