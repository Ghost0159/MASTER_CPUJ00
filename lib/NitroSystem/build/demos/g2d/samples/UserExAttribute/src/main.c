/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - UserExAttribute
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
//      ユーザ拡張アトリビュート情報の利用の仕方を示すサンプルです。
//      コンバータの新機能である、-oua オプションを利用して
//      ユーザ拡張アトリビュート情報を出力します。
//      
//      ユーザ拡張アトリビュート情報を利用するには、セル、アニメーションフレームに
//      対して拡張コメントを付加する必要があります。
//      そのため、拡張コメントを付加できる新しいバージョンのNITRO=CHARACTERで
//      データを作成する必要があります。
//
//      本サンプルでは、マルチセル中のセルアニメーションごとに
//      複数の種類のアトリビュート値を取得し、その結果をデバック文字列で表示します。
//      
//      実際のゲームプロジェクトでは、
//          ･サウンド再生のトリガ
//          ･衝突判定サイズの格納
//      など色々な応用が考えられます。
//
//  操作方法
//      十字キー上下    再生速度変更
//      Y               ユーザ拡張アトリビュートの取得方法の変更
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

#include "g2d_demolib.h"


#define NUM_OF_OAM              128                  // OAMマネージャ管理対象のOAM数
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)      // OAMマネージャ管理対象のAffineパラメータ数

#define ANIM_SPEED_UNIT         (FX32_ONE >> 3)     // アニメーション速度の変化差分

#define NUM_CELL_ANIMATION      10

//
// アニメーションコントローラのコールバックパラメータに
// データを渡す際に使用する、データ構造体
// コントローラには本構造体のポインタを渡す。
//
typedef struct MyAnimCtrlData
{
    NNSG2dAnimController*       pAnmCtrl;       // アニメーションコントローラ
    u16                         idx;            // セルアニメーション番号
    u16                         pad16;          // パディング
    BOOL                        bAttributeIsNotZero;// ゼロ値でないアトリビュートが発見された?
    
}MyAnimCtrlData;
static MyAnimCtrlData              animDataArray[NUM_CELL_ANIMATION];


// Cell 表示位置
static const NNSG2dFVec2           MULTICELL_POSITION = {120 << FX32_SHIFT, 150 << FX32_SHIFT};
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

//
// ユーザ拡張アトリビュート関連
//
static const NNSG2dUserExCellAttrBank*  pCellAttrBank = NULL;
static const NNSG2dUserExAnimAttrBank*  pAnimAttrBank = NULL;

//
// 拡張アトリビュートの取得方法
//
typedef enum MY_GET_ATTRIBUTE_MODE
{
    MY_GET_ATTRIBUTE_MODE_CELL,    // セル拡張アトリビュートを参照します
    MY_GET_ATTRIBUTE_MODE_FRAME,   // アニメーションフレーム
                                   // 拡張アトリビュートを参照します
    MY_GET_ATTRIBUTE_MODE_MAX

}MY_GET_ATTRIBUTE_MODE;
   
static MY_GET_ATTRIBUTE_MODE    attributeMode = MY_GET_ATTRIBUTE_MODE_CELL;


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

//------------------------------------------------------------------------------
// マルチセル中のセルアニメーションに設定されるコールバック関数
// アプリケーションの動作モードにしたがって
// ユーザ拡張アトリビュートを取得し、
// アトリビュートの値が規定値以外かどうかをデータとして保存します。
//
static void CellAnimCallBackFunction( u32 param, fx32 currentFrame )
{
#pragma unused(currentFrame)
    
    MyAnimCtrlData*             pAnmCtrlData    = (MyAnimCtrlData*)param;
    const NNSG2dAnimController* pAnmCtrl        = pAnmCtrlData->pAnmCtrl;
     
    u32 attrVal;
    
    //
    // アトリビュート取得方法によって...
    //
    switch( attributeMode )
    {
    // ----- セル拡張アトリビュートを参照
    case MY_GET_ATTRIBUTE_MODE_CELL:
        {
            // 現在のセル番号を取得
            const u16 cellIdx = NNS_G2dGetAnimCtrlCurrentElemIdxVal( pAnmCtrl );
            // セル番号に対応するアトリビュートを取得
            const NNSG2dUserExCellAttr* pAttr 
                = NNS_G2dGetUserExCellAttr( pCellAttrBank, cellIdx );
            attrVal = NNS_G2dGetUserExCellAttrValue( pAttr );
            
            break;
        }
    // ----- アニメーションフレーム拡張アトリビュートを参照
    case MY_GET_ATTRIBUTE_MODE_FRAME:
        {
            // 現在のシーケンス番号を取得
            const NNSG2dAnimSequence* pSeq 
                = NNS_G2dGetAnimCtrlCurrentAnimSequence( pAnmCtrl );
            const u16 seqIdx 
                = NNS_G2dGetAnimSequenceIndex( pAnimBank, pSeq );
            // アニメーションシーケンスに対応したシーケンス拡張アトリビュートを取得
            // （本サンプルでは利用しませんが、シーケンス拡張アトリビュートからも
            // アトリビュート値を取得可能です。）
            const NNSG2dUserExAnimSequenceAttr* pSeqAttr 
                = NNS_G2dGetUserExAnimSequenceAttr( pAnimAttrBank, seqIdx );
                
            // 再生中のアニメーションフレーム番号を取得
            const u16 currenFrmIdx = NNS_G2dGetAnimCtrlCurrentFrame( pAnmCtrl );
            // シーケンス拡張アトリビュートとフレーム番号から
            // フレーム拡張アトリビュートを取得
            const NNSG2dUserExAnimFrameAttr*  pFrmAttr 
                = NNS_G2dGetUserExAnimFrameAttr( pSeqAttr, currenFrmIdx );                                                        
            attrVal = NNS_G2dGetUserExAnimFrmAttrValue( pFrmAttr );
            
            break;
        }
    }
        
    //
    // 取得したアトリビュート値がゼロではないなら
    // そのことを記録しておく
    //
    if( attrVal != 0 )
    { 
        pAnmCtrlData->bAttributeIsNotZero = TRUE;
    }else{
        pAnmCtrlData->bAttributeIsNotZero = FALSE;
    }
}

//------------------------------------------------------------------------------
// 
/*---------------------------------------------------------------------------*
  Name:         SetupMCCellAnimCallBackFunc

  Description:  NNS_G2dTraverseMCCellAnims()(マルチセル巡回関数)
                にパラメータとして渡される、コールバック関数
                NNS_G2dTraverseMCCellAnims()内で各セルアニメーションについて
                呼び出されます。
                
                本関数内で、セルアニメーションの持つアニメーションコントローラに
                コールバック関数を設定しています。
                 

  Arguments:    userParamaters: ユーザパラメータ
                                （本サンプルでは、animDataArrayへのポインタが代入されています）
                pCellAnim:      マルチセルアニメーション中のセルアニメーション
                idx：           セルアニメーション番号

  Returns:      コールバック呼び出しを中断するか？
                コールバック呼び出しを中断したい場合にFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL SetupMCCellAnimCallBackFunc
(
    u32                  userParamaters,
    NNSG2dCellAnimation* pCellAnim, 
    u16                  idx 
)
{
#pragma unused( userParamaters )     
    
    //
    // 本サンプルデモでは、userParamatersとして、
    // animDataArrayへのポインタが代入されています。
    //
    MyAnimCtrlData* pAnmDataArray = (MyAnimCtrlData*)userParamaters;
    
    // セルアニメーションからアニメーションコントローラを
    // 取得します
    NNSG2dAnimController* pAnmCtrl 
        = NNS_G2dGetCellAnimationAnimCtrl( pCellAnim );

    // アニメーションコントローラのファンクタを初期化します    
    NNS_G2dInitAnimCtrlCallBackFunctor( pAnmCtrl );
    
    // コールバックファンクタのパラメータ
    // として利用されるユーザ定義の構造体にデータを格納します。
    pAnmDataArray[idx].pAnmCtrl = pAnmCtrl;
    pAnmDataArray[idx].idx      = idx;
    
    // アニメーションコントローラのファンクタにパラメータ
    // とコールバック関数を設定します
    NNS_G2dSetAnimCtrlCallBackFunctor
    ( 
        pAnmCtrl,
        NNS_G2D_ANMCALLBACKTYPE_EVER_FRM,
        (u32)&pAnmDataArray[idx],
        CellAnimCallBackFunction 
    );
    
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
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/MultiCellAnimation.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/MultiCellAnimation.NANR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMCR( &pMCBank, "data/MultiCellAnimation.NMCR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMAR( &pMCABank, "data/MultiCellAnimation.NMAR" );
        SDK_NULL_ASSERT( pBuf );

        
        //
        // 拡張アトリビュートバンクを取得します
        // コンバータオプション -oua を指定してデータをコンバートする
        // 必要があります。
        //
        // ここでは、セルバンク、アニメバンクから取得を行っていますが、
        // 他に、マルチセルバンク、マルチセルアニメバンク、からも同様
        // の方法でデータが取得可能です。
        //
        // セル拡張アトリビュートバンクを取得します
        pCellAttrBank = NNS_G2dGetUserExCellAttrBankFromCellBank( pCellBank ); 
        // アニメ拡張アトリビュートバンクを取得します
        pAnimAttrBank = NNS_G2dGetUserExAnimAttrBank( pAnimBank );

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
        
        //
        // マルチセルを構成するセルアニメーションを巡回し
        // 各セルアニメーションに対して
        // ユーザ固有の初期化を行うコールバック関数を呼び出します。
        //
        // ユーザパラメータとして、animDataArrayへのポインタを渡しています。
        // このポインタを利用して、コールバック内部でanimDataArrayのデータを書き換えています。
        NNS_G2dTraverseMCCellAnims( &mcAnim.multiCellInstance,
                                    SetupMCCellAnimCallBackFunc,
                                    (u32)(&animDataArray) );
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
        u16                             numOamDrawn = 0;                  
        
        // 表示する MultiCell
        const NNSG2dMultiCellInstance*  pMultiCell = &(pMcAnim->multiCellInstance); 
        SDK_NULL_ASSERT( pMultiCell );

        //
        // MultiCell と等価な OBJリストをテンポラリに書き出します
        //
        numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                            tempOamBuffer,         // 出力先Oamバッファ
                            NUM_OF_OAM,            // 出力先バッファ長
                            pMultiCell,            // 出力対象 MultiCell
                            NULL,                  // Affine 変換
                            &MULTICELL_POSITION,   // オフセット位置
                            NULL,                  // Affine Index
                            FALSE );               // 倍角Affineか？

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
        static fx32                    animSpeed    = FX32_ONE; // アニメーションスピード
        NNSG2dMultiCellAnimation*      pMcAnim      = &mcAnim;
        
        //
        //  ------ Pad handling.
        //
        {
            G2DDemo_ReadGamePad();

            // change animation speed.
            {
                if (G2DDEMO_IS_TRIGGER(PAD_KEY_UP))
                {
                    animSpeed +=ANIM_SPEED_UNIT;
                }

                if (G2DDEMO_IS_TRIGGER(PAD_KEY_DOWN))
                {
                    animSpeed -= ANIM_SPEED_UNIT;

                    if( animSpeed < 0 )
                    {
                        animSpeed = 0;
                    }
                }
            }
            
            // ---- 拡張アトリビュートの取得方法を変更します
            if (G2DDEMO_IS_TRIGGER(PAD_BUTTON_Y ))
            {
                attributeMode 
                    = (MY_GET_ATTRIBUTE_MODE)((++attributeMode) % MY_GET_ATTRIBUTE_MODE_MAX);
            }
        }

        //  ------ 描画
        AppDraw( pMcAnim, &oamManager );
        //  ------ アニメーションの更新
        NNS_G2dTickMCAnimation( &mcAnim, animSpeed );
        
        
        
        // ------ デバック文字列を描画します
        {
            const NNSG2dAnimController* pAnimCtrl = NNS_G2dGetMCAnimAnimCtrl(pMcAnim);

            G2DDemo_PrintOutf(0, 0, "speed: %7.3f", (float)animSpeed / FX32_ONE );
            G2DDemo_PrintOutf(0, 1, "frame: %3d / %3d",
                pAnimCtrl->pCurrent - pAnimCtrl->pAnimSequence->pAnmFrameArray,
                pAnimCtrl->pAnimSequence->numFrames );
            
            //
            // セルアニメーションが
            // 何かしら規定値ゼロ以外の拡張アトリビュートを認識している場合は
            // o を 表示し、それ以外の場合は x を表示します。
            //
            {
                int i;
                G2DDemo_PrintOutf( 0, 3,      "-----------------" );
                G2DDemo_PrintOutf( 0, 4,      "attribute " );
                G2DDemo_PrintOutf( 0, 5,      "-----------------" );
                
                for( i = 0; i < NUM_CELL_ANIMATION; i++ )
                {
                    if( animDataArray[i].bAttributeIsNotZero == TRUE )
                    {
                        G2DDemo_PrintOutf( 0, 6+i, "%02d:o", i );
                    }else{
                        G2DDemo_PrintOutf( 0, 6+i, "%02d:x", i );
                    }
                }
                G2DDemo_PrintOutf( 0, 6 + NUM_CELL_ANIMATION, "----- " );
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

