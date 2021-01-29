/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib
  File:     txt.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include "g2d_textdemolib.h"


static NNSFndAllocator sAllocator;

#define GRAY(x) GX_RGB(x, x, x)

// デモ共通のカラーパレット
GXRgb TXTColorPalette[TXT_NUM_CPALEETE * 16] =
{
    GX_RGB( 0,  0,  0), GX_RGB(31, 31, 31), GX_RGB( 0,  0,  0), GX_RGB(31,  0,  0),
    GX_RGB( 0, 31,  0), GX_RGB( 0,  0, 31), GX_RGB( 0, 31, 31), GX_RGB(31,  0, 31),
    GX_RGB(31, 31,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0),
    GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0), GX_RGB( 0,  0,  0),

    GX_RGB( 0,  0,  0), GX_RGB(12, 16, 19), GX_RGB(23,  9,  0), GX_RGB(31,  0,  3),
    GX_RGB(31, 17, 31), GX_RGB(31, 18,  0), GX_RGB(30, 28,  0), GX_RGB(21, 31,  0),
    GX_RGB( 0, 20,  7), GX_RGB( 9, 27, 17), GX_RGB( 6, 23, 30), GX_RGB( 0, 11, 30),
    GX_RGB( 0,  0, 18), GX_RGB(17,  0, 26), GX_RGB(26,  0, 29), GX_RGB(31,  0, 18),

    GRAY(31),           GRAY(29),           GRAY(27),           GRAY(25),
    GRAY(23),           GRAY(21),           GRAY(19),           GRAY(17),
    GRAY(15),           GRAY(14),           GRAY(12),           GRAY(10),
    GRAY( 8),           GRAY( 6),           GRAY( 3),           GRAY( 0),
};

//****************************************************************************
//
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         AssignVramBanks

  Description:  VRAMの割り当てを行います。

                        A B C D E F G H I
                2DA BG      o o           256 KB    デモ＆背景用
                    OBJ o o               256 KB    デモ用
                2DB BG                o    32 KB    情報表示用
                    OBJ
                未使用          o o o   o 112 KB

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void AssignVramBanks( void )
{
    GX_SetBankForBG(GX_VRAM_BG_256_CD);
    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);

    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Vブランク割り込みを処理します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}


//****************************************************************************
//
//****************************************************************************


/*---------------------------------------------------------------------------*
  Name:         TXT_Init

  Description:  サンプル共通の初期化。
                    VRAM初期化
                    VRAM割り当て
                    メモリアロケータ初期化
                    ファイルシステム初期化
                    グラフィックスモード設定
                    可視面設定
                    優先度設定
                    BGメモリオフセット設定

                メイン画面 BG
                    BGモード4
                        BG0 テキスト        背景用  可視
                        BG1 テキスト        デモ用  不可視
                        BG2 アフィン        デモ用  不可視
                        BG3 アフィン拡張    デモ用  不可視
                        OBJ                 デモ用  不可視
                    BG VRAM 256 KB
                        00000-  192 KB  デモ用キャラクタ
                        30000-   30 KB  デモ用スクリーン
                        37800-    2 KB  背景用スクリーン
                        38000-   32 KB  背景用キャラクタ
                    OBJ VRAM 256 KB
                        00000-  256 KB  デモ用キャラクタ

                サブ画面
                    BGモード0
                        BG0 テキスト        情報出力用  可視
                        BG1 未使用
                        BG2 未使用
                        BG3 未使用


  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_Init(void)
{
    // Common initialization.
    {
        OS_Init();
        FX_Init();
        GX_Init();

        GX_DispOff();
        GXS_DispOff();
    }

    {
        CMN_InitInterrupt();
        CMN_BeginVBlankIntr(VBlankIntr);
        CMN_ClearVram();
        CMN_InitAllocator( &sAllocator );
        CMN_InitFileSystem( &sAllocator );
    }

    AssignVramBanks();

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_4, GX_BG0_AS_2D);
    GX_SetBGScrOffset(GX_BGSCROFFSET_0x30000);
    GX_SetBGCharOffset(GX_BGCHAROFFSET_0x00000);

    DTX_Init();
}



/*---------------------------------------------------------------------------*
  Name:         TXT_SetupBackground

  Description:  メイン画面BG0に背景画像をロード＆表示します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_SetupBackground( void )
{
    void*                   pPltFile;
    void*                   pChrFile;
    void*                   pScnFile;
    NNSG2dPaletteData*      pPltData;
    NNSG2dCharacterData*    pChrData;
    NNSG2dScreenData*       pScnData;

    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(3);

    pPltFile = LoadNCLR( &pPltData, "/data/BG.NCLR", &sAllocator );
    SDK_NULL_ASSERT( pPltFile );

    pChrFile = LoadNCGR( &pChrData, "/data/BG.NCGR", &sAllocator );
    SDK_NULL_ASSERT( pChrFile );

    pScnFile = LoadNSCR( &pScnData, "/data/BG.NSCR", &sAllocator );
    SDK_NULL_ASSERT( pScnFile );

    NNS_G2dBGSetup(
        NNS_G2D_BGSELECT_MAIN0,
        pScnData,
        pChrData,
        pPltData,
        GX_BG_SCRBASE_0x7800,
        GX_BG_CHARBASE_0x38000
    );

    NNS_FndFreeToAllocator( &sAllocator, pPltFile );
    NNS_FndFreeToAllocator( &sAllocator, pChrFile );
    NNS_FndFreeToAllocator( &sAllocator, pScnFile );
}



/*---------------------------------------------------------------------------*
  Name:         TXT_Alloc

  Description:  メモリを確保します。

  Arguments:    size:   確保するメモリのサイズ

  Returns:      確保したメモリ領域へのポインタ。
 *---------------------------------------------------------------------------*/
void* TXT_Alloc(u32 size)
{
    return NNS_FndAllocFromAllocator( &sAllocator, size );
}



/*---------------------------------------------------------------------------*
  Name:         TXT_Free

  Description:  TXT_Alloc() で確保したメモリを解放します。

  Arguments:    ptr:    開放するメモリ領域へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_Free(void* ptr)
{
    NNS_FndFreeToAllocator( &sAllocator, ptr );
}



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFont

  Description:  フォントをファイルからメモリにロードします。

  Arguments:    pFname: フォントリソースのパス。

  Returns:      ロードしたフォントへのポインタ。
 *---------------------------------------------------------------------------*/
void TXT_LoadFont( NNSG2dFont* pFont, const char* pFname )
{
    void* pBuf;

    pBuf = LoadNFTR( pFont, pFname, &sAllocator );
    if( pBuf == NULL )
    {
        OS_Panic("Fail to load font file(%s).", pFname);
    }

    return;
}



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFile

  Description:  ファイルをメモリにロードします。

  Arguments:    ppFile: ファイルをロードしたメモリアドレスを受け取る
                        バッファへのポインタ。
                fpath:  ロードするファイルのパス

  Returns:      ロードしたファイルのファイルサイズを返します。
                0 の場合はファイルロードに失敗した事を表します。
                この場合 *ppFile の値は無効です。
 *---------------------------------------------------------------------------*/
u32 TXT_LoadFile(void** ppFile, const char* fpath)
{
    return CMN_LoadFile(ppFile, fpath, &sAllocator);
}



/*---------------------------------------------------------------------------*
  Name:         TXT_SetCharCanvasOBJAttrs

  Description:  連続するOAMのNNS_G2dArrangeOBJ* で設定されないパラメータを
                まとめて設定します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void TXT_SetCharCanvasOBJAttrs(
    GXOamAttr * oam,
    int num,
    int priority,
    GXOamMode mode,
    BOOL mosaic,
    GXOamEffect effect,
    int cParam,
    int rsParam
)
{
    int i;

    for( i = 0; i < num; i++ )
    {
        G2_SetOBJPriority(oam, priority);
        G2_SetOBJMode(oam, mode, cParam);
        G2_SetOBJEffect(oam, effect, rsParam);
        G2_OBJMosaic(oam, mosaic);

        oam++;
    }
}

