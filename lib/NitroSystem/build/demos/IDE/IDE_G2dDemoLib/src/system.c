/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_G2dDemoLib
  File:     system.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#include "g2d_demolib.h"
#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Load.h>

// リソースプールのサイズ
#define SIZE_OF_RES_POOL        1000

// アライメントにあわせて切り上げ
#define ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

// アライメントにあわせて切り下げ
#define ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))

G2DDemoGamePad         gGamePad;         // ゲームパッド
NNSFndHeapHandle       gHeapHandle;      // ヒープハンドル

/*---------------------------------------------------------------------------*
    初期化関係。
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         Init3DStuff_

  Description:  ソフトウェアスプライトのためのグラフィックスエンジンの
                初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
static void Init3DStuff_(void)
{
    G3X_Init();                                            // initialize the 3D graphics states
    G3X_InitMtxStack();                                    // initialize the matrix stack

    // メイン画面
    {
        GX_SetBankForTex(GX_VRAM_TEX_0_A);                          // VRAM-A for texture images
        GX_SetBankForOBJ(GX_VRAM_OBJ_128_B);                        // VRAM-B for OBJ
        GX_SetBankForBG(GX_VRAM_BG_128_C);                          // VRAM-C for BGs
        GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);               // VRAM-E for texture palettes
        GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS,                    // graphics mode
                           GX_BGMODE_4,                             // BGMODE is 4
                           GX_BG0_AS_3D);                           // BG #0 is for 3D
        GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D );             // 2D mapping mode
        GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ );  // display BG0 & OBJ
    }

    // サブ画面
    {
        GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);            // VRAM-D for OBJ
        GXS_SetGraphicsMode(GX_BGMODE_0);                      // BGMODE is 0
        GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);        // 2D mapping mode
        GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);                 // display only OBJ
    }


    G2_SetBG0Priority(1);

    G3X_SetShading(GX_SHADING_TOON);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);

    // 3D 面は特殊効果OFFでも他のBG面とαブレンディングが行われますが、
    // そのためにはαブレンディング対象面の指定を行う必要があります。
    // SDK には対象面のみを設定する API が存在しないので、代わりに G2_SetBlendAlpha を使います。
    // この場合 G2_SetBlendAlpha の後ろ2つの引数は無視されます。
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BD, 0, 0);

    // clear color
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 0, FALSE );

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_CommonInit

  Description:  OS、グラフィックスエンジン、ヒープ、ファイルシステムの
                初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_CommonInit(void)
{
    // Common initialization.
    {
        OS_Init();

        FX_Init();

        GX_Init();

        GX_DispOff();
        GXS_DispOff();

        OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
        (void)OS_EnableIrqMask( OS_IE_V_BLANK );
        (void)OS_EnableIrq();

        (void)GX_VBlankIntr(TRUE);
    }

    // Clear VRAM
    {
        //---------------------------------------------------------------------------
        // All VRAM banks to LCDC
        //---------------------------------------------------------------------------
        GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

        //---------------------------------------------------------------------------
        // Clear all LCDC space
        //---------------------------------------------------------------------------
        MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

        //---------------------------------------------------------------------------
        // Disable the banks on LCDC
        //---------------------------------------------------------------------------
        (void)GX_DisableBankForLCDC();

        MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);      // clear OAM
        MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);        // clear the standard palette

        MI_CpuFillFast((void*)HW_DB_OAM, 192, HW_DB_OAM_SIZE); // clear OAM
        MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);  // clear the standard palette
    }

    //------------------------------------------------------------------------------
    // Initializing the 3D Sprite Engine.
    //------------------------------------------------------------------------------
    Init3DStuff_();
    NNS_G2dSetupSoftwareSpriteCamera();


    //------------------------------------------------------------------------------
    // misc.
    {
        *(GXRgb*)(HW_BG_PLTT) = GX_RGB(15, 15, 15);
        *(GXRgb*)(HW_DB_BG_PLTT) = GX_RGB(15, 15, 15);
        OS_InitTick();
    }
    //------------------------------------------------------------------------------
    // Heap 確保
    {
        u32   arenaLow      = ROUND_UP  (OS_GetMainArenaLo(), 16);
        u32   arenaHigh     = ROUND_DOWN(OS_GetMainArenaHi(), 16);
        u32   heapSize      = arenaHigh - arenaLow;
        void* heapMemory    = OS_AllocFromMainArenaLo(heapSize, 16);

        gHeapHandle = NNS_FndCreateExpHeap(heapMemory, heapSize);
        SDK_ASSERT( gHeapHandle != NNS_FND_HEAP_INVALID_HANDLE );
    }

    //------------------------------------------------------------------------------
    // File System
    {
        (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);

        /* initialize file-system */
        FS_Init( FS_DMA_NOT_USE );

        /* always preload FS table for faster directory access. */
        {
            const u32   need_size = FS_GetTableSize();
            void    *p_table = G2DDemo_Alloc(need_size);
            SDK_ASSERT(p_table != NULL);
            (void)FS_LoadTable(p_table, need_size);
        }
    }
    
    // ゲームパッドを空読みします。
    G2DDemo_ReadGamePad();
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_CameraSetup

  Description:  カメラの共通設定を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_CameraSetup(void)
{
    {
        // 単位行列と等価
        VecFx32 Eye = { 0, 0, 0 };                  // Eye position
        VecFx32 vUp = { 0, FX32_ONE, 0 };           // Up
        VecFx32 at = { 0, 0, -FX32_ONE };           // Viewpoint

        // Matrix mode is changed to GX_MTXMODE_POSITION_VECTOR internally,
        // and the camera matrix is loaded to the current matrix.
        G3_LookAt(&Eye, &vUp, &at, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_MaterialSetup

  Description:  マテリアルの共通設定を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_MaterialSetup(void)
{
    // for software sprite-setting
    {
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31),        // diffuse
                                GX_RGB(16, 16, 16),        // ambient
                                TRUE                       // use diffuse as vtx color if TRUE
                                );

        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16),        // specular
                                GX_RGB(0, 0, 0),           // emission
                                FALSE                      // use shininess table if TRUE
                                );

        G3_PolygonAttr(GX_LIGHTMASK_NONE,                  // no lights
                       GX_POLYGONMODE_MODULATE,            // modulation mode
                       GX_CULL_NONE,                       // cull back
                       0,                                  // polygon ID(0 - 63)
                       31,                                 // alpha(0 - 31)
                       0                                   // OR of GXPolygonAttrMisc's value
                       );
    }
}


/*---------------------------------------------------------------------------*
    その他
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewCellAnimation

  Description:  プールから NNSG2dCellAnimation を取得します。

  Arguments:    num:  取得する NNSG2dCellAnimation の数。

  Returns:      取得した NNSG2dCellAnimation へのポインタ。
 *---------------------------------------------------------------------------*/
NNSG2dCellAnimation* G2DDemo_GetNewCellAnimation( u16 num )
{
    static NNSG2dCellAnimation        sPoolAnimatedObj[SIZE_OF_RES_POOL];
    static u16                        sNumAnimatedObj = 0;

    const u16 retIdx = sNumAnimatedObj;

    sNumAnimatedObj += num;
    SDK_ASSERT( sNumAnimatedObj <= SIZE_OF_RES_POOL );

    return &sPoolAnimatedObj[ retIdx ];
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewNode

  Description:  プールから NNSG2dNode を取得します。

  Arguments:    num:  取得する NNSG2dNode の数。

  Returns:      取得した NNSG2dNode へのポインタ。
 *---------------------------------------------------------------------------*/
NNSG2dNode* G2DDemo_GetNewNode( u16 num )
{
    static NNSG2dNode                 sPoolObjNode[SIZE_OF_RES_POOL];
    static u16                        sNumUsedPoolObjNode = 0;

    const u16 retIdx = sNumUsedPoolObjNode;

    sNumUsedPoolObjNode += num;
    SDK_ASSERT( sNumUsedPoolObjNode <= SIZE_OF_RES_POOL );

    return &sPoolObjNode[ retIdx ];
}

/*---------------------------------------------------------------------------*
  Name:         getNewMultiCellAnim_

  Description:  プールから NNSG2dMultiCellAnimation を取得します。

  Arguments:    num:  取得する NNSG2dMultiCellAnimation の数。

  Returns:      取得した NNSG2dMultiCellAnimation へのポインタ。
 *---------------------------------------------------------------------------*/
static NNSG2dMultiCellAnimation*     GetNewMultiCellAnim_( u16 num )
{
    static NNSG2dMultiCellAnimation   sMcAnimPool_[SIZE_OF_RES_POOL];
    static u16                        sNumUsedMcAnimPool = 0;

    const u16 retIdx = sNumUsedMcAnimPool;

    sNumUsedMcAnimPool += num;
    SDK_ASSERT( sNumUsedMcAnimPool <= SIZE_OF_RES_POOL );

    return &sMcAnimPool_[retIdx];
}


/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewMultiCellAnimation

  Description:  新しい NNSG2dMultiCellAnimation を取得して初期化します。

  Arguments:    numNode:            ノード数
                pAnimBank:          アニメーションデータバンク
                pCellDataBank:      セルデータバンク
                pMultiCellDataBank: マルチセルデータバンク

  Returns:      プールから取得した初期化済みの NNSG2dMultiCellAnimation へ
                のポインタ
 *---------------------------------------------------------------------------*/
/* 古いAPIを使用する以前のバージョン
NNSG2dMultiCellAnimation*     G2DDemo_GetNewMultiCellAnimation
(
    u16                                numNode,
    const NNSG2dCellAnimBankData*      pAnimBank,
    const NNSG2dCellDataBank*          pCellDataBank ,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank
)
{
    // プールから NNSG2dMultiCellAnimation を取得
    NNSG2dMultiCellAnimation* pMCAnim = GetNewMultiCellAnim_( 1 );

    // プールから numNode ぶんの NNSG2dNode と NNSG2dCellAnimation を取得
    NNSG2dNode*               pNodeArray     = G2DDemo_GetNewNode( numNode );
    NNSG2dCellAnimation*      pCellAnimArray = G2DDemo_GetNewCellAnimation( numNode );

    NNS_G2dInitMCAnimation( pMCAnim,              // マルチセルアニメーション
                                  pNodeArray,           // ノード配列
                                  pCellAnimArray,       // セルアニメーション配列
                                  numNode,              // ノード数
                                  pAnimBank,            // アニメーションデータバンク
                                  pCellDataBank,        // セルデータバンク
                                  pMultiCellDataBank ); // マルチセルデータバンク
    return pMCAnim;
}
*/

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_GetNewMultiCellAnimation

  Description:  新しい NNSG2dMultiCellAnimation を取得して初期化します。

  Arguments:    pAnimBank:          アニメーションデータバンク
                pCellDataBank:      セルデータバンク
                pMultiCellDataBank: マルチセルデータバンク
                mcType:             マルチセル実体の種類
                
  Returns:      プールから取得した初期化済みの NNSG2dMultiCellAnimation へ
                のポインタ
 *---------------------------------------------------------------------------*/
NNSG2dMultiCellAnimation*     G2DDemo_GetNewMultiCellAnimation
(
    const NNSG2dCellAnimBankData*      pAnimBank,
    const NNSG2dCellDataBank*          pCellDataBank ,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank,
    NNSG2dMCType                       mcType
)
{
    const u32 szWork = NNS_G2dGetMCWorkAreaSize( pMultiCellDataBank, mcType );
    void* pWorkMem   = NNS_FndAllocFromExpHeap( gHeapHandle, szWork );
    
    
    // プールから NNSG2dMultiCellAnimation を取得
    NNSG2dMultiCellAnimation* pMCAnim = GetNewMultiCellAnim_( 1 );

    // 初期化
    NNS_G2dInitMCAnimationInstance( pMCAnim, 
                                    pWorkMem, 
                                    pAnimBank, 
                                    pCellDataBank, 
                                    pMultiCellDataBank, 
                                    mcType );
    return pMCAnim;
}




/*---------------------------------------------------------------------------*
    ゲームパッド関係。
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_ReadGamePad

  Description:  キーを読み込み、トリガとリリーストリガを求めます。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G2DDemo_ReadGamePad(void)
{
    u16 status = PAD_Read();

    gGamePad.trigger = (u16)(status          & (status ^ gGamePad.button));
    gGamePad.release = (u16)(gGamePad.button & (status ^ gGamePad.button));
    gGamePad.button  = status;
}

