/*---------------------------------------------------------------------------*
  Project:  NitroSDK - GX - demos - UnitTours/Sub_Double3D
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.22  2007/10/11 02:20:22  yosizaki
  fix comparator function.
  add check for the result of MATH_QSort().

  Revision 1.21  2007/04/26 04:08:30  okubata_ryoma
  2画面に別々のfogを設定する際のコードを追加

  Revision 1.20  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.19  2005/11/21 10:44:16  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.18  2005/10/07 09:02:12  okubata_ryoma
  微修正

  Revision 1.17  2005/10/07 05:22:48  okubata_ryoma
  コメントの修正

  Revision 1.16  2005/10/07 05:11:45  okubata_ryoma
  コメントの修正

  Revision 1.14  2005/08/03 02:03:11  okubata_ryoma
  再度回りこみの不具合の修正

  Revision 1.13  2005/07/07 06:16:11  okubata_ryoma
  オブジェクトの回り込み現象を修正しました

  Revision 1.10  2005/07/07 04:47:46  okubata_ryoma
  微修正

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.6  2004/04/07 01:23:27  yada
  fix header comment

  Revision 1.5  2004/04/06 12:48:08  yada
  fix header comment

  Revision 1.4  2004/02/24 10:48:30  yasu
  fix initial value of "flag"

  Revision 1.3  2004/02/24 05:25:21  yasu
  Use OS_EnableIrqMask instead of OS_SetIrqMask

  Revision 1.2  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.1  2004/02/04 11:11:04  nishida_kenji
  new

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that displays moving 3D models on both LCDs.
//
// Both LCDs display a different rotating 3D cube.
// The sub 2D engine displays the captured image rendered by the main engine,
// making use of direct color bitmap BG/bitmap OBJ.
// Note that the output LCDs are swapped every frame.
//
// HOWTO:
// 1. Set up OAM for the sub 2D engine, to display captured images correctly.
// 2. (Frame 2N):   Assign VRAM-C to BG of the sub 2D engine,
//                  and display the captured image on VRAM-C
//                  through direct color bitmap BG.
//                  Capture the 3D plane and store it on VRAM-D.
//    (Frame 2N+1): Assign VRAM-D to OBJ of the sub 2D engine,
//                  and display the captured image on VRAM-D
//                  through bitmap OBJs.
//                  Capture the 3D plane and store it on VRAM-C.
//---------------------------------------------------------------------------

#include <nitro.h>
#include "DEMO.h"

#define    STACK_SIZE     1024
#define    THREAD1_PRIO   (OS_THREAD_LAUNCHER_PRIORITY - 6)
void    proc1(void *p1);
OSThread thread1;
u32     stack1[STACK_SIZE / sizeof(u32)];

static GXOamAttr sOamBak[128];

BOOL    flip_flag = TRUE;              // flip切り替えフラグ
BOOL    swap = FALSE;                  // SwapBuffers実行フラグ
int     attr = GX_POLYGON_ATTR_MISC_NONE;

s16     gCubeGeometry[3 * 8] = {
    FX16_ONE, FX16_ONE, FX16_ONE,
    FX16_ONE, FX16_ONE, -FX16_ONE,
    FX16_ONE, -FX16_ONE, FX16_ONE,
    FX16_ONE, -FX16_ONE, -FX16_ONE,
    -FX16_ONE, FX16_ONE, FX16_ONE,
    -FX16_ONE, FX16_ONE, -FX16_ONE,
    -FX16_ONE, -FX16_ONE, FX16_ONE,
    -FX16_ONE, -FX16_ONE, -FX16_ONE
};

GXRgb   gCubeColor[8] = {
    GX_RGB(31, 31, 31),
    GX_RGB(31, 31, 0),
    GX_RGB(31, 0, 31),
    GX_RGB(31, 0, 0),
    GX_RGB(0, 31, 31),
    GX_RGB(0, 31, 0),
    GX_RGB(0, 0, 31),
    GX_RGB(0, 0, 0)
};

static void Color(int idx)
{
    G3_Color(gCubeColor[idx]);
}

static void Vtx(int idx)
{
    G3_Vtx(gCubeGeometry[idx * 3], gCubeGeometry[idx * 3 + 1], gCubeGeometry[idx * 3 + 2]);
}

static void Quad(int idx0, int idx1, int idx2, int idx3)
{
    Vtx(idx0);
    Vtx(idx1);
    Vtx(idx2);
    Vtx(idx3);
}

static void ColVtxQuad(int idx0, int idx1, int idx2, int idx3)
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

static void drawLeftCube(u16 Rotate)
{
    G3_PushMtx();

    // Rotate and translate
    G3_Translate(-3 << (FX32_SHIFT - 1), 0, 0);
    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // disable lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   attr                // OR of GXPolygonAttrMisc's value
        );

    //---------------------------------------------------------------------------
    // Draw a cube:
    // Specify different colors for the planes.
    //---------------------------------------------------------------------------

    G3_Begin(GX_BEGIN_QUADS);

    {
        Color(3);
        Quad(2, 0, 4, 6);

        Color(4);
        Quad(7, 5, 1, 3);

        Color(5);
        Quad(6, 4, 5, 7);

        Color(2);
        Quad(3, 1, 0, 2);

        Color(6);
        Quad(5, 4, 0, 1);

        Color(1);
        Quad(6, 7, 3, 2);
    }

    G3_End();

    G3_PopMtx(1);
}

static void drawRightCube(u16 Rotate)
{
    G3_PushMtx();

    // Rotate and translate
    G3_Translate(3 << (FX32_SHIFT - 1), 0, 0);

    {
        fx16    s = FX_SinIdx(Rotate);
        fx16    c = FX_CosIdx(Rotate);

        G3_RotX(s, c);
        G3_RotY(s, c);
        G3_RotZ(s, c);
    }

    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), // diffuse
                            GX_RGB(16, 16, 16), // ambient
                            FALSE      // use diffuse as vtx color if TRUE
        );

    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), // specular
                            GX_RGB(0, 0, 0),    // emission
                            FALSE      // use shininess table if TRUE
        );

    G3_PolygonAttr(GX_LIGHTMASK_NONE,  // disable lights
                   GX_POLYGONMODE_MODULATE,     // modulation mode
                   GX_CULL_BACK,       // cull back
                   0,                  // polygon ID(0 - 63)
                   31,                 // alpha(0 - 31)
                   attr                // OR of GXPolygonAttrMisc's value
        );

    //---------------------------------------------------------------------------
    // Draw a cube:
    // Specify different colors for the vertices.
    //---------------------------------------------------------------------------
    G3_Begin(GX_BEGIN_QUADS);
    {
        ColVtxQuad(2, 0, 4, 6);
        ColVtxQuad(7, 5, 1, 3);
        ColVtxQuad(6, 4, 5, 7);
        ColVtxQuad(3, 1, 0, 2);
        ColVtxQuad(5, 4, 0, 1);
        ColVtxQuad(6, 7, 3, 2);
    }
    G3_End();

    G3_PopMtx(1);

}

static void setupFrame2N(void)
{

    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);

    (void)GX_ResetBankForSubOBJ();
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    GX_SetBankForLCDC(GX_VRAM_LCDC_D);
    GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                  GX_CAPTURE_MODE_A,
                  GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0, GX_CAPTURE_DEST_VRAM_D_0x00000, 16, 0);

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);

    GXS_SetGraphicsMode(GX_BGMODE_5);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG2);
    G2S_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256,
                           GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x00000);
    G2S_SetBG2Priority(0);
    G2S_BG2Mosaic(FALSE);
}

static void setupFrame2N_1(void)
{
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    (void)GX_ResetBankForSubBG();
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);
    GX_SetBankForLCDC(GX_VRAM_LCDC_C);
    GX_SetCapture(GX_CAPTURE_SIZE_256x192,
                  GX_CAPTURE_MODE_A,
                  GX_CAPTURE_SRCA_3D, (GXCaptureSrcB)0, GX_CAPTURE_DEST_VRAM_C_0x00000, 16, 0);

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);

    GXS_SetGraphicsMode(GX_BGMODE_5);
    GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);
}

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
            G2_SetOBJAttr(&sOamBak[idx],
                          x,
                          y,
                          0,
                          GX_OAM_MODE_BITMAPOBJ,
                          FALSE,
                          GX_OAM_EFFECT_NONE,
                          GX_OAM_SHAPE_64x64, GX_OAM_COLOR_16, (y / 8) * 32 + (x / 8), 15, 0);
        }
    }

    DC_FlushRange(&sOamBak[0], sizeof(sOamBak));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadOAM(&sOamBak[0], 0, sizeof(sOamBak));

}

//---------------------------------------------------------------------------
// SwapBuffers実行フラグを立てる
//---------------------------------------------------------------------------
static void SetSwapBuffersflag(void)
{
    OSIntrMode old = OS_DisableInterrupts();    // 割り込み禁止
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    swap = TRUE;
    (void)OS_RestoreInterrupts(old);
}

static BOOL IgnoreRemoval(void)
{
    return FALSE;
}

void NitroMain(void)
{
//#define CPU_TEST                      // CPUに負荷を与えます(テスト用)
//#define GE_TEST                       // ジオメトリエンジンに負荷を与えます(テスト用)

//#define SET_FOG                       // Double3Dを使用し2画面で別々のfogを設定したい場合は
                                        // このdefineを有効にした際のコードをご参考ください。

    u16     Rotate = 0;                // for rotating cubes(0-65535)

    int     i = 0;

#if defined(CPU_TEST) || defined(GE_TEST)
    MATHRandContext32 rnd;             // CPUに負荷を与える乱数を生成するための変数（テスト用）
#endif
    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, initialize VRAM, and set BG #0 for 3D mode.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    CARD_SetPulledOutCallback(IgnoreRemoval);

    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_WakeupThreadDirect(&thread1);

    G3X_Init();
    G3X_InitTable();
    G3X_InitMtxStack();
    G3X_AntiAlias(TRUE);
    setupSubOAM();

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    G3X_SetClearColor(GX_RGB(0, 0, 0), // clear color
                      0,               // clear alpha
                      0x7fff,          // clear depth
                      63,              // clear polygon ID
                      FALSE            // fog
        );

    G3_ViewPort(0, 0, 255, 191);       // Viewport

    //---------------------------------------------------------------------------
    // Set up the projection matrix
    //---------------------------------------------------------------------------
    {
        fx32    right = FX32_ONE;
        fx32    top = FX32_ONE * 3 / 4;
        fx32    near = FX32_ONE;
        fx32    far = FX32_ONE * 400;

        //---------------------------------------------------------------------------
        // Switch MTXMODE to GX_MTXMODE_PROJECTION, and
        // set a projection matrix onto the current projection matrix on the matrix stack
        //---------------------------------------------------------------------------
        G3_Perspective(FX32_SIN30, FX32_COS30,  // sine and cosine of FOVY
                       FX32_ONE * 4 / 3,        // aspect
                       near,           // near
                       far,            // far
                       NULL            // a pointer to a matrix if you use it
            );

        G3_StoreMtx(0);
    }

#if defined(SET_FOG)
    /* フォグ設定 */
    {
        u32     fog_table[8];
        int     i;

        // フォグ属性の設定
        G3X_SetFog(TRUE,               // フォグを有効に
                   GX_FOGBLEND_COLOR_ALPHA,     // カラーとアルファにフォグ
                   GX_FOGSLOPE_0x2000, // フォグ勾配の設定
                   0x4800);            // フォグ計算デプス値
        G3X_SetFogColor(GX_RGB(31, 31, 31), 0); // フォグカラーの設定

        // フォグテーブルの設定(値が大きいほど濃いフォグになる)
        for (i = 0; i < 8; i++)
        {
            fog_table[i] = (u32)(((i * 16) << 0) |
                                 ((i * 16 + 4) << 8) |
                                 ((i * 16 + 8) << 16) | ((i * 16 + 12) << 24));
        }
        G3X_SetFogTable(&fog_table[0]);
    }
#endif

    DEMOStartDisplay();

#if defined(CPU_TEST) || defined(GE_TEST)
    MATH_InitRand32(&rnd, OS_GetVBlankCount()); // CPUに負荷を与えるための乱数の生成（テスト用）
#endif

    while (1)
    {
        DEMOReadKey();


        G3X_Reset();

        Rotate += 256;

        //---------------------------------------------------------------------------
        // Set up camera matrix
        //---------------------------------------------------------------------------
        {
            VecFx32 Eye = { 0, 0, FX32_ONE * 5 };       // Eye Position
            VecFx32 at = { 0, 0, 0 };  // Viewpoint
            VecFx32 vUp = { 0, FX32_ONE, 0 };   // Up

            G3_LookAt(&Eye, &vUp, &at, NULL);
        }
        G3_PushMtx();

#if defined(SET_FOG)
        if(DEMO_IS_TRIG(PAD_BUTTON_A))
        {
            if(attr)
            {
                // fog を無効にします
                attr = GX_POLYGON_ATTR_MISC_NONE;
            }
            else
            {
                // fog を有効にします
                attr = GX_POLYGON_ATTR_MISC_FOG;
            }
        }
#endif

        if (flip_flag)
            drawRightCube(Rotate);
        else
            drawLeftCube(Rotate);
        G3_PopMtx(1);

#ifdef GE_TEST
        while (GX_GetVCount() != 191)
        {;
        }
#endif

#ifdef CPU_TEST
        // 乱数的にCPUに負荷を与えます（テスト用）
        if (DEMO_IS_PRESS(PAD_BUTTON_R))
        {
            OS_SpinWait(MATH_Rand32(&rnd, 1000000));
        }
#endif

#ifdef GE_TEST
        // 乱数的にジオメトリエンジンに負荷を与えます (テスト用)
        while (i <= MATH_Rand32(&rnd, 1000000))
        {
            G3_PushMtx();
            G3_PopMtx(1);
            i++;
        }
#endif

#if defined(SET_FOG)
        // Vカウント180まで wait
        // (このwaitの後からVブランク開始ラインまでにフォグの処理が完了するようにする必要がある。
        //  今回の場合は余裕をもって180ラインに設定)
        while (GX_GetVCount() != 180)
        {;
        }

        // SwapBuffers コマンドは確実に処理落ちが発生しないフレームでのみ発行
        // (Vカウント180で描画が完了していれば処理落ちは発生しないため)
        if (!G3X_IsGeometryBusy())
        {
            SetSwapBuffersflag();          // SwapBuffers コマンドを発行
            
            if (flip_flag)                 // flipの切り替えフラグ
            {
                // 下画面のフォグの色を設定
                G3X_SetFogColor(GX_RGB(0, 0, 0), 0);
            }
            else
            {
                // 上画面のフォグの色を設定
                G3X_SetFogColor(GX_RGB(31, 31, 31), 0);
            }
        }
#else
        SetSwapBuffersflag();          // SwapBuffers実行フラグを立てる
#endif
        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

    }
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt

    //---- スレッド１起動
    OS_WakeupThreadDirect(&thread1);
}
/*
---------------------------------------------------------------------------------------------------

▼SwapBuffersコマンドとジオメトリエンジンの動作の説明

・SwapBuffersコマンドはG3_SwapBuffers関数を呼んだときに、ジオメトリコマンドFIFOに格納され
  SwapBuffersコマンドはどのタイミングでジオメトリコマンドFIFOから呼ばれたとしても、
  次のVブランクの開始時に、バッファのスワップ処理が実行されます。
  (つまり、Vブランクの開始時にしか実行できません)
・そのため、もし描画処理などがVブランク期間内までずれ込んで、
  SwapBuffersコマンドの実行がVブランクの開始時までにできなかった場合、
  次のフレームのVブランク開始時までジオメトリエンジンはビジーのまま約1フレーム待ち続ける。
  この間は画像の描画やスワップなどはできないでそのフレームは前と同じ画像を表示する。


s : SwapBuffersコマンド発行
S : バッファのスワップ処理実行
w : SwapBuffersコマンド発行からバッファのスワップ処理実行開始までのウェイト
G : ジオメトリエンジンが描画などの処理中

                          |
-VBlank(end)------------------------------------------
               +-----+    |     +-----+
               |     |    |     |     |
               |     |    |     |     |
               |  G  |    |     |  G  |
               |     |    |     |     |
               |     |    |     |     |
               |     |    |     |     |
               |     |    |     |     |
               |     |    |     |     |
               +-----+    |     |     |
               |  s  |    |     |     |
               +-----+    |     |     |
               |  w  |    |     |     |
-VBlank(start)-+-----+----+-----+     +-----------
  ※784(cycle) |  S  |    |     |     |            ※このスワップ処理にCPUサイクルで784サイクルかかります
               +-----+    |     +-----+           
    ※check  →           |     |  s  |            ※このcheckの部分でG3X_IsGeometryBusy関数を用いて
                          |     +-----+              スワップ処理が終わったかどうかを判定しています。　　
-VBlank(end)--------------+-----+     +---------
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |  w  |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
                          |     |     |
-VBlank(start)------------+-----+-----+----------
                          |     |  S  |
                          |     +-----+
                          |
                          |
-VBlank(end)--------------+-----------------------


★問題が発生する条件
    ・Vブランク内でバッファのスワップ処理が完了していないのに、上下画面の切り替えを行った場合
      上下で同じ画面が表示されてしまいます。

▲問題点を防ぐためVブランク内でやるべきこと
    ・Vブランクに入る前までに、次の画像の描画が終了していることを確認
      描画が終了しているなら、次のVブランク開始時にバッファのスワップ処理が実行されます。
    ・Vブランク内で、バッファのスワップ処理が終了したことを確認
      スワップ処理が終了しているなら、上下画面の切り替えを行います。

    そのために
    ・Vブランクに入る前に、次の画像の描画が終了していること(次のVブランクでスワップ処理が確実に始まる)
    ・Vブランク内で、バッファのスワップ処理が終了したこと
    ↑この２つを確認する必要があります。

■具体的にVブランク内で行っている処理

        ・ if(GX_GetVCount() <= 193)               スワップ処理するには1ラインもかからないので
                                                   この時点で193ラインに達していれば、
                                                   スワップ処理は終わっていると判断でき、
                                                   OS_SpinWait関数で784サイクル待つ必要がありません。
                                                   
        ・ OS_SpinWait(784)                        784サイクル待ちます。
        
        ・ if (!G3X_IsGeometryBusy() && swap)      Vブランク前に SwapBuffers コマンドが発行済みで、
                                                   ジオメトリエンジンがビジーでない場合
                                                   (画像の描画処理やバッファのスワップ処理実行中でない)
                                                
●処理の意味
    バッファのスワップ処理(レンダリングエンジンが参照するデータを切り替える)には、
    CPUサイクルで784(392×2)サイクル必要なため、
    もし、Vブランクの開始時にバッファのスワップ処理が開始されたのだとしたら、784サイクル待てば
    バッファのスワップ処理は終わっていると判断できます。

    Vブランク開始から784サイクル待ってジオメトリエンジンがビジーな場合
        Vブランク前の画像の描画処理が遅れたため、バッファのスワップ処理がまだ終了していない。
        →このときはスワップ処理も上下画面切り替えもするべきではありません。
    Vブランク開始から784サイクル待ってジオメトリエンジンがビジーでない場合
        バッファのスワップ処理も画像の描画処理も終了している。
        →画像の描画もスワップ処理も終了しているので、上下画面の切り替えをするべきです。

もし、Vブランク中でのこの784サイクルがどうしてももったいないと思う場合は
この OS_SpinWait(784) の前に何か、784サイクル以上の処理を入れてください。

もしくは、この方法はポリゴンを1つも描画していないときは使えない方法なのですが、

void VBlankIntr(void)
{
    if ( (G3X_GetVtxListRamCount() == 0) && swap )
    {
        if (flag)  // flipの切り替え
        {
            setupFrame2N_1();
        }
        else
        {
            setupFrame2N();
        }


上のように修正しても問題を防ぐことができます。
この方法はスワップバッファ処理がVBlank開始時に実行されたとき
頂点RAMに格納されている頂点数が 0 になることを
G3X_GetVtxListRamCount関数(頂点RAMに格納されている頂点の数を返す関数) で確認することによって
スワップバッファ処理が実行されたという判定をしています。


注意：もし、非常に長い割り込み処理をVブランクの割り込み前に発生せて、
      Vブランクの開始が遅れ、Vブランクの時間が非常に短く(約3ライン以下)なってしまった場合
      表示中に上下画面の切り替えが行われたり、そのフレームだけ前の画像が表示されることがあります。

---------------------------------------------------------------------------------------------------
*/
void proc1(void *p1)
{
#pragma unused(p1)

    while (1)
    {
#define SPIN_WAIT                      // OS_SpinWait関数を用いる方法と用いない方法を切り替えます。

    // 画像の描画もバッファのスワップ処理も完了していることを確認します。
#ifdef SPIN_WAIT

    if (GX_GetVCount() <= 193)
    {
        OS_SpinWait(784);
    }
    if (!G3X_IsGeometryBusy() && swap)

#else

    if ((G3X_GetVtxListRamCount() == 0) && swap)

#endif
    // 画像の描画もスワップ処理も完了している場合、上下画面の切り替えを行います。
    {
        if (flip_flag)                 // flipの切り替え(上下画面の切り替え処理)
        {
            setupFrame2N_1();
        }
        else
        {
            setupFrame2N();
        }
        swap = FALSE;
        flip_flag = !flip_flag;
    }
    OS_SleepThread(NULL);   // このスレッドを眠らせる。次に起きるのは OS_WakeupThreadDirect() が呼ばれたとき。
    }
}
