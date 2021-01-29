/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos - mic-2
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2007/05/14 06:39:09  yosizaki
  add option for tool.

  Revision 1.13  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.12  2005/11/21 11:03:53  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.11  2005/04/08 01:49:02  yosizaki
  add DC_InvalidateRange before reading the caputure-buffer.

  Revision 1.10  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.9  2004/11/04 07:18:57  terui
  MIC_GetLastSamplingAddressの仕様修正に伴う微修正。

  Revision 1.8  2004/09/17 09:36:03  terui
  MIC_StartAutoSampling(Async)のパラメータ変更に伴う修正。

  Revision 1.7  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.6  2004/08/20 02:44:31  terui
  TS_VERSION=0時にもPM_SetAmpするよう修正。

  Revision 1.5  2004/08/05 07:38:59  takano_makoto
  列挙型変更MICSamplingTypeの変更に伴う修正

  Revision 1.4  2004/08/04 23:44:28  terui
  LCDバックライト操作関数の変更

  Revision 1.3  2004/08/02 00:47:45  yada
  PM_GAINAMP_80 -> PM_AMPGAIN_80

  Revision 1.2  2004/07/31 12:46:19  terui
  PMICをマイク用に初期化するコードを追加

  Revision 1.1  2004/06/17 04:42:46  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    A sample that controls mic sampling status.

    USAGE:
        UP , DOWN    : Control sampling span.
        LEFT , RIGHT : Control sampling bit range. ( 8bit or 12 bit )
        A            : start / stop sampling. (toggle)
        B            : force-stop. (stop and ignore rest data)

    HOWTO:
        1. Initialize memory allocate system to get 32byte aligned big buffer.
        2. Initialize MIC library.
        3. When sampling is stopped, you can change status
           and start auto sampling.
           Debug-output is sampling data for tool "mic2wav.exe".
        4. Debug-output log can make its waveform files by tool "mic2wav.exe".
           > $(NITROSDK_ROOT)/tools/bin/mic2wav [logfile] [,directory]
           each sampling data (separated by A-Button) creates a waveform file
           in "[directory]/%08X.wav".

    NOTE:
        1. The speed of debug-output is later than sampling.
           When you stop sampling, then please wait all data is printed.

 *---------------------------------------------------------------------------*/

#include    <nitro.h>

#ifdef  SDK_TS
#if     ( SDK_TS_VERSION >= 100 )
#include    <nitro/spi/common/pm_common.h>
#include    <nitro/spi/ARM9/pm.h>
#endif
#endif


/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // キーリピート開始までのフレーム数
#define     KEY_REPEAT_SPAN     10     // キーリピートの間隔フレーム数
#define     TEST_BUFFER_SIZE    ( 1024 * 1024 ) // 1M


/*---------------------------------------------------------------------------*
    構造体定義
 *---------------------------------------------------------------------------*/
// キー入力情報
typedef struct KeyInformation
{
    u16     cnt;                       // 未加工入力値
    u16     trg;                       // 押しトリガ入力
    u16     up;                        // 離しトリガ入力
    u16     rep;                       // 押し維持リピート入力

}
KeyInformation;

/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void);
static void Init3D(void);
static void Draw3D(void);
static void DrawLine(s16 sx, s16 sy, s16 ex, s16 ey);
static void VBlankIntr(void);
static void KeyRead(KeyInformation * pKey);
static void SetDrawData(void *address, MICSamplingType type);
static void PrintfVariableData(void);

/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static KeyInformation gKey;
static MICAutoParam gMicAutoParam;
static u8 *gMicData;
static u8 gDrawData[192];


/* mic-1 との違いは, データをデバッグ出力することと
   常にワンショット録音することです。 */

/* 波形出力用変数 */
static volatile BOOL g_sample_busy = FALSE;
static const void *g_record_smps = NULL;

static void StartSampling(void);
static void OnSampleDone(MICResult result, void *arg);
static void OutputSampleWave(void *dat, MICSamplingType type);
static void StopSamplingOutput(void);

/* サンプリングを開始 */
static void StartSampling(void)
{
    gMicAutoParam.buffer = (void *)gMicData;
    gMicAutoParam.size = TEST_BUFFER_SIZE;
    gMicAutoParam.loop_enable = FALSE;
    gMicAutoParam.full_callback = OnSampleDone;
    g_sample_busy = TRUE;
    g_record_smps = gMicData;
    PrintfVariableData();
    (void)MIC_StartAutoSampling(&gMicAutoParam);
}

/* サンプリング出力を終了 */
static void StopSamplingOutput(void)
{
    OS_PutString("$end\n");
    OS_PutString("\n");
    g_record_smps = NULL;
}

/* サンプリング完了通知, または停止時の波形出力処理 */
static void OnSampleDone(MICResult result, void *arg)
{
    (void)result;
    (void)arg;
    if (g_sample_busy)
    {
        g_sample_busy = FALSE;
    }
}

/* サンプリング中の波形をログ出力 */
static void OutputSampleWave(void *dat, MICSamplingType type)
{
    /* 16 サンプル単位で 1 回に 2 行まで.
       それ以上の量を出力すると、ログが欠落することがある. */
    enum
    { smps_per_line = 16, max_line_per_frame = 2 };
    if (!g_record_smps || !dat)
        return;

    DC_InvalidateRange((void *)g_record_smps, (u32)((u8 *)dat - (u8 *)g_record_smps));
    switch (type)
    {
    case MIC_SAMPLING_TYPE_8BIT:
        {
            typedef u8 SMP;
            /* 以下は MIC_SAMPLING_TYPE_12BIT と同じ */
            char    buf[1 + (sizeof(SMP) * 2 + 1) * smps_per_line + 1 + 1], *s;
            const SMP *from = (const SMP *)g_record_smps;
            const SMP *to = (const SMP *)dat;
            int     lines = 0;
            while ((lines < max_line_per_frame) && (from + smps_per_line <= to))
            {
                int     i, j;
                s = buf;
                *s++ = '|';
                for (i = 0; i < smps_per_line; ++i)
                {
                    u32     unit = from[i];
                    for (j = sizeof(SMP) * 8; (j -= 4) >= 0;)
                    {
                        u32     c = (u32)((unit >> j) & 0x0F);
                        c += (u32)((c < 10) ? ('0' - 0) : ('A' - 10));
                        MI_WriteByte(s++, (u8)c);
                    }
                    MI_WriteByte(s++, (u8)',');
                }
                MI_WriteByte(s++, (u8)'\n');
                MI_WriteByte(s++, (u8)'\0');
                OS_PutString(buf);
                from += smps_per_line;
                ++lines;
            }
            g_record_smps = from;

        }
        break;
    case MIC_SAMPLING_TYPE_12BIT:
        {
            typedef u16 SMP;
            /* 以下は MIC_SAMPLING_TYPE_8BIT と同じ */
            char    buf[1 + (sizeof(SMP) * 2 + 1) * smps_per_line + 1 + 1], *s;
            const SMP *from = (const SMP *)g_record_smps;
            const SMP *to = (const SMP *)dat;
            int     lines = 0;
            while ((lines < max_line_per_frame) && (from + smps_per_line <= to))
            {
                int     i, j;
                s = buf;
                *s++ = '|';
                for (i = 0; i < smps_per_line; ++i)
                {
                    u32     unit = from[i];
                    for (j = sizeof(SMP) * 8; (j -= 4) >= 0;)
                    {
                        u32     c = (u32)((unit >> j) & 0x0F);
                        c += (u32)((c < 10) ? ('0' - 0) : ('A' - 10));
                        MI_WriteByte(s++, (u8)c);
                    }
                    MI_WriteByte(s++, (u8)',');
                }
                MI_WriteByte(s++, (u8)'\n');
                MI_WriteByte(s++, (u8)'\0');
                OS_PutString(buf);
                from += smps_per_line;
                ++lines;
            }
            g_record_smps = from;

        }
        break;
    }

    /* サンプリング停止後も残りのデータを出力. */
    if (!g_sample_busy && g_record_smps)
    {
        if ((u8 *)g_record_smps + smps_per_line * 2 >= (u8 *)dat)
        {
            StopSamplingOutput();
        }
    }

}


/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // 各種初期化
    OS_Init();
    FX_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // 表示設定初期化
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    // 3D関連初期化
    Init3D();

    // 割込み設定
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);

    //****************************************************************
    // Initialize MIC.
    InitializeAllocateSystem();
    // OS_Allocで確保したメモリは32バイトALIGNになっているので、
    // キャッシュ操作しても他のメモリを壊さない。
    gMicData = (u8 *)OS_Alloc(TEST_BUFFER_SIZE);
    gMicAutoParam.type = MIC_SAMPLING_TYPE_12BIT;
    gMicAutoParam.rate = MIC_SAMPLING_RATE_11K;
    MIC_Init();

#ifdef  SDK_TS
    // PMIC初期化
    PM_Init();
    // AMPオン
    (void)PM_SetAmp(PM_AMP_ON);
#if ( SDK_TS_VERSION >= 100 )
    // AMPゲイン調整
    (void)PM_SetAmpGain(PM_AMPGAIN_80);
#endif
#if ( SDK_TS_VERSION == 100 )
    // ノイズ対策の為、LCDバックライトオフ
    (void)PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);
#endif
#endif

    //****************************************************************

    // 内部変数初期化
    {
        s32     i;
        for (i = 0; i < 192; i++)
        {
            gDrawData[i] = 0x80;
        }
    }

    // LCD表示開始
    GX_DispOn();
    GXS_DispOn();

    // 開始メッセージ出力
    OS_Printf("#ARM9: MIC demo started.\n");
    OS_Printf("#   up/down    -> change sampling span\n");
    OS_Printf("#   left/right -> change bit range\n");
    OS_Printf("#   A          -> start / stop\n");
    OS_Printf("#   B          -> force-stop\n");
    OS_Printf("#   select     -> terminate\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    KeyRead(&gKey);

    // main loop
    while (TRUE)
    {
        // キー入力情報取得
        KeyRead(&gKey);

        // A が押されたら開始, 再度 A が押されたら停止.
        if ((gKey.trg & PAD_BUTTON_A) != 0)
        {
            if (!g_sample_busy)
            {
                StartSampling();
            }
            else
            {
                (void)MIC_StopAutoSampling();
                OnSampleDone(MIC_RESULT_SUCCESS, &gMicAutoParam);
            }
        }
        // B が押されたら, 停止して残りのデータも無視.
        if ((gKey.trg & PAD_BUTTON_B) != 0)
        {
            if (g_sample_busy)
            {
                (void)MIC_StopAutoSampling();
                OnSampleDone(MIC_RESULT_SUCCESS, &gMicAutoParam);
            }
            if (g_record_smps)
            {
                StopSamplingOutput();
            }
        }
        // SELECT が押されたらプログラムを矯正終了.
        if ((gKey.trg & PAD_BUTTON_SELECT) != 0)
        {
            OS_Exit(0);
        }

        // サンプリング出力中でなければ可変パラメータ変更
        if (!g_record_smps)
        {
            // サンプリング種別(ビット幅)変更
            if ((gKey.trg | gKey.rep) & (PAD_KEY_LEFT | PAD_KEY_RIGHT))
            {
                //****************************************************************
                if (gMicAutoParam.type == MIC_SAMPLING_TYPE_8BIT)
                {
                    gMicAutoParam.type = MIC_SAMPLING_TYPE_12BIT;
                }
                else
                {
                    gMicAutoParam.type = MIC_SAMPLING_TYPE_8BIT;
                }
                //****************************************************************
                if (!g_record_smps)
                    PrintfVariableData();
            }
            // サンプリングレート変更
            if ((gKey.trg | gKey.rep) & PAD_KEY_UP)
            {
                //****************************************************************
                switch (gMicAutoParam.rate)
                {
                case MIC_SAMPLING_RATE_8K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_11K;
                    break;
                case MIC_SAMPLING_RATE_11K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_16K;
                    break;
                case MIC_SAMPLING_RATE_16K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_22K;
                    break;
                case MIC_SAMPLING_RATE_22K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_32K;
                    break;
                case MIC_SAMPLING_RATE_32K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
                    break;
                }
                //****************************************************************
                if (!g_record_smps)
                    PrintfVariableData();
            }
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                //****************************************************************
                switch (gMicAutoParam.rate)
                {
                case MIC_SAMPLING_RATE_8K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_32K;
                    break;
                case MIC_SAMPLING_RATE_11K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
                    break;
                case MIC_SAMPLING_RATE_16K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_11K;
                    break;
                case MIC_SAMPLING_RATE_22K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_16K;
                    break;
                case MIC_SAMPLING_RATE_32K:
                    gMicAutoParam.rate = MIC_SAMPLING_RATE_22K;
                    break;
                }
                //****************************************************************
                if (!g_record_smps)
                    PrintfVariableData();
            }
        }

        // 波形をログ出力.
        OutputSampleWave(MIC_GetLastSamplingAddress(), gMicAutoParam.type);

        // 波形描画
        if (g_sample_busy)
        {
            SetDrawData(MIC_GetLastSamplingAddress(), gMicAutoParam.type);
            Draw3D();
        }

        // Ｖブランク待ち
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         InitializeAllocateSystem

  Description:  メインメモリ上のアリーナにてメモリ割当てシステムを初期化する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    // OS_Initは呼ばれているという前提
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         Init3D

  Description:  3Dにて表示するための初期化処理

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void Init3D(void)
{
    G3X_Init();
    G3X_InitMtxStack();
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    G2_SetBG0Priority(0);
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AlphaTest(FALSE, 0);
    G3X_AlphaBlend(TRUE);
    G3X_AntiAlias(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_SetFog(FALSE, (GXFogBlend)0, (GXFogSlope)0, 0);
    G3X_SetClearColor(0, 0, 0x7fff, 63, FALSE);
    G3_ViewPort(0, 0, 255, 191);
}

/*---------------------------------------------------------------------------*
  Name:         Draw3D

  Description:  3D表示にて波形を表示

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void Draw3D(void)
{
    G3X_Reset();

    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_Identity();

    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);

    if (g_sample_busy)
    {
        s32     i;

        for (i = 0; i < 191; i++)
        {
            DrawLine((s16)(gDrawData[i]), (s16)i, (s16)(gDrawData[i + 1]), (s16)(i + 1));
        }
    }

    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
}

/*---------------------------------------------------------------------------*
  Name:         DrawLine

  Description:  三角ポリゴンで線を描画

  Arguments:    sx  - 描画する線の開始点のｘ座標
                sy  - 描画する線の開始点のｙ座標
                ex  - 描画する線の終点のｘ座標
                ey  - 描画する線の終点のｙ座標

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DrawLine(s16 sx, s16 sy, s16 ex, s16 ey)
{
    fx16    fsx, fsy, fex, fey;

    fsx = (fx16)(((sx - 128) * 0x1000) / 128);
    fsy = (fx16)(((96 - sy) * 0x1000) / 96);
    fex = (fx16)(((ex - 128) * 0x1000) / 128);
    fey = (fx16)(((96 - ey) * 0x1000) / 96);

    G3_Begin(GX_BEGIN_TRIANGLES);
    {
        G3_Color(GX_RGB(31, 31, 31));
        G3_Vtx(fsx, fsy, 0);
        G3_Color(GX_RGB(31, 31, 31));
        G3_Vtx(fex, fey, 0);
        G3_Color(GX_RGB(31, 31, 31));
        G3_Vtx(fsx, fsy, 1);
    }
    G3_End();
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Ｖブランク割込みベクトル。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // IRQ チェックフラグをセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  キー入力情報を編集する。
                押しトリガ、離しトリガ、押し継続リピートを検出。

  Arguments:    pKey  - 編集するキー入力情報構造体。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(KeyInformation * pKey)
{
    static u16 repeat_count[12];
    int     i;
    u16     r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);     // 押しトリガ
                repeat_count[i] = 1;
            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i); // 押し継続リピート
                    repeat_count[i] = KEY_REPEAT_START - KEY_REPEAT_SPAN;
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);      // 離しトリガ
            }
        }
    }
    pKey->cnt = r;                     // 未加工キー入力
}

/*---------------------------------------------------------------------------*
  Name:         SetDrawData

  Description:  現状での最新サンプリング済みデータを表示に反映させるバッファに
                格納する。

  Arguments:    address - コンポーネントによって最新のサンプリングデータが
                          格納されたメインメモリ上の位置。
                type    - サンプリング種別(ビット幅)

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SetDrawData(void *address, MICSamplingType type)
{
    s32     i;

    if (!address)
        return;

    if (type == MIC_SAMPLING_TYPE_8BIT)
    {
        u8     *p;

        p = (u8 *)((u32)address - 191);
        if (p < gMicData)
        {
            p = (u8 *)((u32)p + TEST_BUFFER_SIZE);
        }
        DC_InvalidateRange((void *)((u32)p & 0xffffffe0), 32);
        for (i = 0; i < 192; i++)
        {
            gDrawData[i] = *p;
            p++;
            if ((u32)p >= (u32)(gMicData + TEST_BUFFER_SIZE))
            {
                p -= TEST_BUFFER_SIZE;
            }
            if (((u32)p % 32) == 0)
            {
                DC_InvalidateRange(p, 32);
            }
        }
    }

    if (type == MIC_SAMPLING_TYPE_12BIT)
    {
        u16    *p;

        p = (u16 *)((u32)address - 382);
        if ((u32)p < (u32)gMicData)
        {
            p = (u16 *)((u32)p + TEST_BUFFER_SIZE);
        }
        DC_InvalidateRange((void *)((u32)p & 0xffffffe0), 32);
        for (i = 0; i < 192; i++)
        {
            gDrawData[i] = (u8)((*p >> 8) & 0x00ff);
            p++;
            if ((u32)p >= (u32)(gMicData + TEST_BUFFER_SIZE))
            {
                p = (u16 *)((u32)p - TEST_BUFFER_SIZE);
            }
            if (((u32)p % 32) == 0)
            {
                DC_InvalidateRange(p, 32);
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrintfVariableData

  Description:  可変なサンプリング設定をプリント出力する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintfVariableData(void)
{
    s32     range = 0;
    s32     rate = 0;

    switch (gMicAutoParam.type)
    {
    case MIC_SAMPLING_TYPE_8BIT:
        range = 8;
        break;
    case MIC_SAMPLING_TYPE_12BIT:
        range = 16;
        break;
    }

    switch (gMicAutoParam.rate)
    {
    case MIC_SAMPLING_RATE_8K:
        rate = 8000;
        break;
    case MIC_SAMPLING_RATE_11K:
        rate = 11025;
        break;
    case MIC_SAMPLING_RATE_16K:
        rate = 16000;
        break;
    case MIC_SAMPLING_RATE_22K:
        rate = 22050;
        break;
    case MIC_SAMPLING_RATE_32K:
        rate = 32000;
        break;
    }

    OS_Printf("$rate=%d\n$bits=%d\n", rate, range);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
