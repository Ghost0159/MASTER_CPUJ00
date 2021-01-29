/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - demos - switchover
  File:     graphic_main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: graphic_main.c,v $
  Revision 1.5  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.4  2005/11/21 11:08:57  kitase_hirotake
  OS_PrintServer の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.3  2005/06/27 11:10:11  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.2  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.1  2005/02/16 12:47:54  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
 * WVR ライブラリ使用の有無を動的に切り替えるサンプルです.
 * ボタンにより両画面 3D 表示モードとワイヤレス通信モードを切り替えます.
 *
 * このサンプルは以下の2つのサンプルを足し合わせただけのものです.
 * 個々のモードにおける動作の詳細は各々のソースコードを参照ください.
 *    /build/demos/wvr/simple
 *    /build/demos/gx/UnitTours/Sub_Double3D
 *---------------------------------------------------------------------------*/


#include <nitro.h>
#include    "common.h"


/*****************************************************************************/
/* constant */

/* 立方体の頂点座標 */
static const s16 gCubeGeometry[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

/* 立方体の頂点カラー */
static const GXRgb gCubeColor[8] = {
    GX_RGB(31, 31, 31),
    GX_RGB(31, 31, 0),
    GX_RGB(31, 0, 31),
    GX_RGB(31, 0, 0),
    GX_RGB(0, 31, 31),
    GX_RGB(0, 31, 0),
    GX_RGB(0, 0, 31),
    GX_RGB(0, 0, 0)
};


/*****************************************************************************/
/* variable */

/* キャプチャした画面をビットマップ OBJ として表示するための OAM 情報 */
static GXOamAttr sOamBak[128];


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         GetPadTrigger

  Description:  キー入力情報を更新し, 最新の押下トリガビットを返す。
                押しトリガ、離しトリガ、押し継続リピートを検出。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static int GetPadTrigger(void)
{
    static u16 pad_bak = 0;
    const u16 pad_cur = PAD_Read();
    const int trig = (u16)(~pad_bak & pad_cur);
    return (pad_bak = pad_cur), trig;
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V ブランク割り込みハンドラ (DEMO ライブラリが設定)

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         Color

  Description:  立方体の指定頂点カラーを設定

  Arguments:    idx             頂点インデックス

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void Color(int idx)
{
    G3_Color(gCubeColor[idx]);
}

/*---------------------------------------------------------------------------*
  Name:         Vtx

  Description:  立方体の指定頂点座標を設定

  Arguments:    idx             頂点インデックス

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void Vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}

/*---------------------------------------------------------------------------*
  Name:         ColVtxQuad

  Description:  指定の頂点による座標および色を連続設定して矩形を生成

  Arguments:    idx0            0 番目の頂点インデックス
                idx1            1 番目の頂点インデックス
                idx2            2 番目の頂点インデックス
                idx3            3 番目の頂点インデックス
                bOwnIndexColor  TRUE なら各インデックスの色を使用,
                                FALSE なら全て idx0 の色を使用.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ColVtxQuad(int idx0, int idx1, int idx2, int idx3, BOOL bOwnIndexColor)
{
    if (bOwnIndexColor)
    {
        Color(idx0);
        Vtx(idx0);
        Color(idx1);
        Vtx(idx1);
        Color(idx2);
        Vtx(idx2);
        Color(idx3);
        Vtx(idx3);
    }
    else
    {
        Color(idx0);
        Vtx(idx0);
        Vtx(idx1);
        Vtx(idx2);
        Vtx(idx3);
    }
}

/*---------------------------------------------------------------------------*
  Name:         drawCube

  Description:  画面左側に立方体を描画

  Arguments:    Rotate          回転角度
                bIsRight        右なら TRUE, 左なら FALSE

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void drawCube(u16 Rotate, BOOL bIsRight)
{
    G3_PushMtx();
    {
        const int pos = bIsRight ? +3 : -3;

        /* 並行移動および回転移動 */
        G3_Translate(pos << (FX32_SHIFT - 1), 0, 0);
        {
            const fx16 s = FX_SinIdx(Rotate);
            const fx16 c = FX_CosIdx(Rotate);
            G3_RotX(s, c);
            G3_RotY(s, c);
            G3_RotZ(s, c);
        }

        /*
         * 以下の設定でマテリアルおよびポリゴン属性を指定.
         *   拡散反射色  = GX_RGB(31, 31, 31) (頂点色として使用しない)
         *   環境反射色  = GX_RGB(16, 16, 16)
         *   鏡面反射色  = GX_RGB(16, 16, 16) (テーブルを使用しない)
         *   放射光色    = GX_RGB(0, 0, 0)
         *   ライト      = 全無効
         *   モード      = モジュレーション
         *   カリング    = 裏面非表示
         *   ポリゴン ID = 0
         *   不透明度    = 31
         *   その他      = 無し
         */
        G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(16, 16, 16), FALSE);
        G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
        G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_BACK,
                       0, 31, GX_POLYGON_ATTR_MISC_NONE);

        /* 立方体を描画 */
        G3_Begin(GX_BEGIN_QUADS);
        {
            ColVtxQuad(2, 0, 4, 6, bIsRight);
            ColVtxQuad(7, 5, 1, 3, bIsRight);
            ColVtxQuad(6, 4, 5, 7, bIsRight);
            ColVtxQuad(3, 1, 0, 2, bIsRight);
            ColVtxQuad(5, 4, 0, 1, bIsRight);
            ColVtxQuad(6, 7, 3, 2, bIsRight);
        }
        G3_End();

    }
    G3_PopMtx(1);
}

/*---------------------------------------------------------------------------*
  Name:         setupFrame

  Description:  フレームの描画設定を初期化

  Arguments:    bIsRight        右なら TRUE, 左なら FALSE

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void setupFrame(BOOL bIsRight)
{
    /*
     * 2 つの画面で 1 ピクチャフレームごとに交互に 3D エンジンを使用する.
     * 3D エンジンを使用できないフレームでは前フレームのキャプチャ結果を使用する.
     * VRAM-C には左寄り立方体が, VRAM-D には右寄り立方体がキャプチャされるが,
     * ただしそれぞれのVRAM は BG または OBJ だけにしか使用できないため,
     * VRAM-C ではダイレクトビットマップで, VRAM-D ではテクスチャ OBJ で表示する.
     */

    /*
     * 以下の設定で LCD を初期化.
     *   メイン BG モード : 0
     *     BG-0 : 3D (優先度 0)
     *     BG-1 : -
     *     BG-2 : -
     *     BG-3 : -
     *     OBJ  : -
     */
    (void)GX_ResetBankForSubOBJ();
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);
    GXS_SetGraphicsMode(GX_BGMODE_5);

    if (bIsRight)
    {
        /*
         *   上にメイン, 下にサブ LCD を表示
         *   サブ BG モード : 5
         *     BG-0 : -
         *     BG-1 : -
         *     BG-2 : ダイレクトビットマップ
         *     BG-3 : -
         *     OBJ  : -
         *   キャプチャモード : (256, 192), 3D のみ 100%
         *   VRAM-A : -
         *   VRAM-B : -
         *   VRAM-C : サブ OBJ
         *   VRAM-D : LCDC
         */
        (void)GX_DisableBankForLCDC();
        (void)GX_DisableBankForSubOBJ();
        GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
        GX_SetBankForLCDC(GX_VRAM_LCDC_D);
        GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                      GX_CAPTURE_MODE_A, GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0,
                      GX_CAPTURE_DEST_VRAM_D_0x00000, 16, 0);
        GXS_SetVisiblePlane(GX_PLANEMASK_BG2);
        G2S_SetBG2Priority(0);
        G2S_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256,
                               GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x00000);
        G2S_BG2Mosaic(FALSE);
    }
    else
    {
        /*
         *   下にメイン, 上にサブ LCD を表示
         *   サブ BG モード : 5
         *     BG-0 : -
         *     BG-1 : -
         *     BG-2 : -
         *     BG-3 : -
         *     OBJ  : ON
         *   キャプチャモード : (256, 192), 3D のみ 100%
         *   VRAM-A : -
         *   VRAM-B : -
         *   VRAM-C : LCDC
         *   VRAM-D : サブ OBJ
         */
        (void)GX_DisableBankForLCDC();
        (void)GX_DisableBankForSubBG();
        GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);
        GX_SetBankForLCDC(GX_VRAM_LCDC_C);
        GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                      GX_CAPTURE_MODE_A, GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0,
                      GX_CAPTURE_DEST_VRAM_C_0x00000, 16, 0);
        GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);
    }

    G3X_Reset();

    /* カメラ行列設定 */
    {
        const VecFx32 Eye = { 0, 0, FX32_ONE * 5 };
        const VecFx32 at = { 0, 0, 0 };
        const VecFx32 vUp = { 0, FX32_ONE, 0 };
        G3_LookAt(&Eye, &vUp, &at, NULL);
    }
}

/*---------------------------------------------------------------------------*
  Name:         setupSubOAM

  Description:  VRAM-D のサブ画面表示用に OAM を格子状に配置設定

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void setupSubOAM(void)
{
    int     i;
    int     x, y;
    int     idx = 0;

    GXS_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_2D_W256);

    for (i = 0; i < 128; ++i)
    {
        sOamBak[i].attr01 = 0;
        sOamBak[i].attr23 = 0;
    }

    for (y = 0; y < 192; y += 64)
    {
        for (x = 0; x < 256; x += 64, idx++)
        {
            G2_SetOBJAttr(&sOamBak[idx], x, y, 0,
                          GX_OAM_MODE_BITMAPOBJ, FALSE,
                          GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_64x64, GX_OAM_COLOR_16,
                          (y / 8) * 32 + (x / 8), 15, 0);
        }
    }
    DC_FlushRange(&sOamBak[0], sizeof(sOamBak));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadOAM(&sOamBak[0], 0, sizeof(sOamBak));
}

/*---------------------------------------------------------------------------*
  Name:         GraphicMain

  Description:  両画面 3D 表示モードの初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void GraphicMain(void)
{
    GX_DispOff();
    GXS_DispOff();

    /* VRAM, OAM, パレットRAM の全クリア */
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    /* V ブランク割り込みハンドラ設定 */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    /* GX 初期化 */
    G3X_Init();
    G3X_InitTable();
    G3X_InitMtxStack();

    /* 起動時のみ必要な描画設定の初期化 */
    G3X_AntiAlias(TRUE);
    setupSubOAM();
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    G3X_SetClearColor(GX_RGB(0, 0, 0), 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);

    /* プロジェクション行列の初期化 */
    {
        const fx32 right = FX32_ONE;
        const fx32 top = FX32_ONE * 3 / 4;
        const fx32 near = FX32_ONE;
        const fx32 far = FX32_ONE * 400;
        G3_Perspective(FX32_SIN30, FX32_COS30, FX32_ONE * 4 / 3, near, far, NULL);
        G3_StoreMtx(0);
    }

    /* 描画ループの開始 */
    OS_WaitVBlankIntr();
    GX_DispOn();
    GXS_DispOn();
    {
        u16     Rotate = 0;
        BOOL    bIsRight;

        (void)GetPadTrigger();
        for (bIsRight = TRUE;; bIsRight = !bIsRight)
        {
            if (GetPadTrigger() & PAD_BUTTON_START)
            {
                break;
            }
            /* フレーム描画 */
            setupFrame(bIsRight);
            drawCube(Rotate, bIsRight);

            Rotate += 256;

            G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
            OS_WaitVBlankIntr();
            /*
             * 描画結果はバッファスワップ後の V ブランクから反映されるので
             * 意図する表示先 LCD 設定はこの時点で設定する.
             * 右寄りの立方体を下画面に出すよう指定する.
             */
            GX_SetDispSelect(bIsRight ? GX_DISP_SELECT_SUB_MAIN : GX_DISP_SELECT_MAIN_SUB);

        }
    }
}
