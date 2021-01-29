/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - demo - characterRecognition-2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.15  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.14  2005/11/21 10:57:25  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.13  2005/10/04 02:59:20  seiki_masashi
  パターンデータを共有化

  Revision 1.12  2005/03/04 13:36:09  seiki_masashi
  PRC_InitPrototypeDB 及び PRC_InitInputPattern の返り値の型変更に対応

  Revision 1.11  2005/03/02 03:17:40  seiki_masashi
  TP_RequestSetStability 関数の呼び出しを削除

  Revision 1.10  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.9  2005/02/18 11:53:34  seiki_masashi
  warning 対策

  Revision 1.8  2004/11/04 07:55:46  seiki_masashi
  regularize を normalize に修正

  Revision 1.7  2004/09/03 11:48:54  yasu
  fix typo

  Revision 1.6  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.5  2004/07/29 13:53:03  seiki_masashi
  small fix

  Revision 1.4  2004/07/19 06:52:06  seiki_masashi
  識別子名の整理

  Revision 1.3  2004/07/09 00:11:30  seiki_masashi
  最適結果の表示は、fine と superfine から選ぶように

  Revision 1.2  2004/06/30 08:49:34  seiki_masashi
  algo_simple -> algo_light
  algo_standard -> algo_fine
  algo_accurate -> algo_superfine
  新しい algo_standard の追加

  Revision 1.1  2004/06/29 13:12:20  seiki_masashi
  move characterRecognition-1 to characterRecognition-2

  Revision 1.7  2004/06/29 08:49:31  seiki_masashi
  再描画処理の実装

  Revision 1.6  2004/06/29 02:17:32  seiki_masashi
  pdic2c.pl の最新フォーマットに対応
  認識結果のもっとも得点がよかったものを画面に表示

  Revision 1.5  2004/06/28 23:38:17  seiki_masashi
  FINALROM ビルド時の warning 除去

  Revision 1.4  2004/06/28 13:16:56  seiki_masashi
  パターンデータの出力機能の追加

  Revision 1.3  2004/06/28 11:12:26  seiki_masashi
  見本パターンの一覧に regularizeSize を追加
  各種パラメータ類を整理
  デモの整備

  Revision 1.2  2004/06/25 13:32:53  seiki_masashi
  認識アルゴリズム 'Standard' の追加
  全ての認識アルゴリズムの認識結果を並列で表示

  Revision 1.1  2004/06/23 12:02:11  seiki_masashi
  add 'prc/characterRecognition-1'

  Revision 1.4  2004/06/22 11:19:00  seiki_masashi
  動作可能バージョン(lightのみ)

  Revision 1.3  2004/06/17 08:14:52  seiki_masashi
  PRC_GetPrototypeDBBufferSizeEx

  Revision 1.2  2004/06/17 00:52:36  seiki_masashi
  PRC_InitPrototypeDBEX, PRC_InitInputPatternEX

  Revision 1.1  2004/06/16 23:56:09  seiki_masashi
  rename ptrcg to prc.

  Revision 1.3  2004/06/15 08:10:55  seiki_masashi
  ダミーのライブラリでサンプルプログラムが動作可能に

  Revision 1.2  2004/06/15 05:55:09  seiki_masashi
  インターフェイスの改善

  Revision 1.1  2004/06/15 00:54:42  seiki_masashi
  インターフェイスの設計


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/prc/algo_standard.h>
#include <nitro/prc/algo_light.h>
#include <nitro/prc/algo_fine.h>
#include <nitro/prc/algo_superfine.h>

#include "patterns.h"

//----- 認識結果を何位まで出力するか
#define RESULT_NUM              3

//----- 1入力あたりの最大点数
//-----  ※標準的には毎秒60点が入力されます
#define POINT_PER_INPUT_MAX     1024
//----- 1入力あたりの最大画数
#define STROKE_PER_INPUT_MAX    20
//----- リサンプル後の最大点数
#define POINT_AFTER_RESAMPLING_MAX 40

#define VRAM_BASE_ADDR          ((u16*)HW_LCDC_VRAM_C)
#define VRAM_LINE_SIZE          256
#define DRAW_COLOR              0

#define MEMORY_MAIN_MAX_HEAPS   1
#define MEMORY_MAIN_HEAP_SIZE   0x200000

#define N_INPUT_PARAM           8

static void InitOS(void);
static void InitTP(void);
static void InitDisplay(void);
static void DrawLine(int x1, int y1, int x2, int y2, u16 col);
static void PutDot(int x, int y, u16 col);
static void ClearScreen(void);
void    DrawStrokes(const PRCStrokes *strokes, int sx, int sy);
static void PrintRecognitionResult(const char *name, PRCPrototypeEntry **result, fx32 *scores);
static void VBlankIntr(void);
void    PrintStrokesAsEntryData(const PRCStrokes *strokes,
                                const char *name, u32 kind, fx16 correction, int normalizeSize);

static int sInputPatternParamThresholds[N_INPUT_PARAM][3] = {
    {6, 2048, 1},
    {8, 3072, 2},
    {12, 4096, 3},
    {16, 6144, 4},
    {24, 8192, 5},
    {32, 12288, 8},
    {48, 16384, 12},
    {64, 21846, 16},
};

void NitroMain(void)
{
    InitOS();
    InitDisplay();
    InitTP();

    OS_InitTick();
    OS_InitAlarm();
    OS_InitThread();

    {
        PRCPrototypeDB_Common protoDB;
        int     normalizeSize;
        PRCPoint points[POINT_PER_INPUT_MAX];
        PRCStrokes strokes;
        u16     prevKey, currKey, trgKey;
        BOOL    touching = FALSE;
        TPData  prevSample = { 0, 0, 0, 0 };
        void   *prototypeListBuffer;
        void   *patternBuffer;
        void   *recognitionBuffer;
        int     dictIndex = 0;
        int     inputParamIndex = N_INPUT_PARAM / 2;
        BOOL    fDictShown = FALSE;
        int     maxPointCount;

        PRC_Init();

        // 見本DBの展開
        // PrototypeList は別ファイルで定義された見本パターンの一覧
        {
            // 後に逆引きしやすいよう entry.data に entry 番号を埋め込んでおく
            int     iEntry;
            for (iEntry = 0; iEntry < PrototypeList.entrySize; iEntry++)
            {
                ((PRCPrototypeEntry *)&PrototypeList.entries[iEntry])->data = (void *)iEntry;
            }
        }
        {
            u32     size;
            size = PRC_GetPrototypeDBBufferSize_Common(&PrototypeList);
            OS_Printf("required buffer for InitPrototypeDB: %d bytes\n", size);
            if (size == 0)
                size = 1;
            prototypeListBuffer = OS_Alloc(size);
            SDK_ASSERT(prototypeListBuffer);
        }
        if (!PRC_InitPrototypeDB_Common(&protoDB, prototypeListBuffer, &PrototypeList))
        {
            OS_Printf("cannot initialize the prototype DB.\n");
            OS_Terminate();
        }

        normalizeSize = protoDB.normalizeSize;
        maxPointCount = POINT_AFTER_RESAMPLING_MAX;
        while (TRUE)
        {
            {
                u32     size, maxSize;;
                size = PRC_GetInputPatternBufferSize_Common(maxPointCount, STROKE_PER_INPUT_MAX);
                OS_Printf("maxPointCount: %d, maxStrokeCount: %d\n", maxPointCount,
                          STROKE_PER_INPUT_MAX);
                OS_Printf("required buffer for InitInputPattern: %d bytes\n", size);
                if (size == 0)
                    size = 1;
                patternBuffer = OS_Alloc(size);
                if (patternBuffer == NULL)
                {
                    OS_Printf("OS_Alloc failed.\n");
                    maxPointCount = POINT_AFTER_RESAMPLING_MAX;
                    continue;
                }
                size =
                    PRC_GetRecognitionBufferSize_Light(maxPointCount, STROKE_PER_INPUT_MAX,
                                                       &protoDB);
                OS_Printf("required buffer for Recognition of 'Light': %d bytes\n", size);
                maxSize = size;
                size =
                    PRC_GetRecognitionBufferSize_Standard(maxPointCount, STROKE_PER_INPUT_MAX,
                                                          &protoDB);
                OS_Printf("required buffer for Recognition of 'Standard': %d bytes\n", size);
                if (maxSize < size)
                    maxSize = size;
                size =
                    PRC_GetRecognitionBufferSize_Fine(maxPointCount, STROKE_PER_INPUT_MAX,
                                                      &protoDB);
                OS_Printf("required buffer for Recognition of 'Fine': %d bytes\n", size);
                if (maxSize < size)
                    maxSize = size;
                size =
                    PRC_GetRecognitionBufferSize_Superfine(maxPointCount, STROKE_PER_INPUT_MAX,
                                                           &protoDB);
                OS_Printf("required buffer for Recognition of 'Superfine': %d bytes\n", size);
                if (maxSize < size)
                    maxSize = size;
                if (maxSize == 0)
                    maxSize = 1;
                recognitionBuffer = OS_Alloc(maxSize);
                if (recognitionBuffer == NULL)
                {
                    OS_Printf("OS_Alloc failed.\n");
                    maxPointCount = POINT_AFTER_RESAMPLING_MAX;
                    OS_Free(patternBuffer);
                    continue;
                }
            }

            PRC_InitStrokes(&strokes, points, POINT_PER_INPUT_MAX);

            ClearScreen();
            fDictShown = FALSE;

            //---- Dummy READ workaround for pushing 'A' at IPL
            currKey = PAD_Read();

            while (TRUE)
            {
                BOOL    fClearScreen = FALSE;
                BOOL    fClearScreenAfterRecognition = FALSE;
                BOOL    fRecognizeChar = FALSE;
                BOOL    fShowPrototypeList = FALSE;
                BOOL    fPrintPattern = FALSE;
                TPData  sample;

                prevKey = currKey;
                currKey = PAD_Read();
                trgKey = (u16)(currKey & ~prevKey);

                (void)TP_RequestCalibratedSampling(&sample);

                if (sample.touch && (sample.validity == 0))
                {
                    // ペンは下りている
                    if (!PRC_IsFull(&strokes))
                    {
                        if (touching)
                        {
                            // 書いてる最中
                            DrawLine(prevSample.x, prevSample.y, sample.x, sample.y, DRAW_COLOR);
                        }
                        else
                        {
                            // ペンを下ろした瞬間
                            PutDot(sample.x, sample.y, DRAW_COLOR);
                            touching = TRUE;
                        }
                        // Store stroke data
                        PRC_AppendPoint(&strokes, sample.x, sample.y);
                    }
                }
                else
                {
                    // ペンは上がっている
                    if (touching)
                    {
                        // 上がった瞬間
                        if (!PRC_IsFull(&strokes))
                        {
                            PRC_AppendPenUpMarker(&strokes);
                        }
                        touching = FALSE;
                    }
                }

                // A ボタンが押されたら認識を開始
                if (trgKey & PAD_BUTTON_A)
                {
                    fRecognizeChar = TRUE;
                    fClearScreenAfterRecognition = TRUE;
                }

                // B ボタンが押されたら画面を消去
                if (trgKey & (PAD_BUTTON_START | PAD_BUTTON_B))
                {
                    fClearScreen = TRUE;
                }

                // X ボタンが押されたら認識をするものの画面消去はしない
                if (trgKey & PAD_BUTTON_X)
                {
                    fRecognizeChar = TRUE;
                }

                // Y ボタンが押されたら認識しつつ、パターンデータをデバック出力に
                if (trgKey & PAD_BUTTON_Y)
                {
                    fRecognizeChar = TRUE;
                    fPrintPattern = TRUE;
                }

                // L ボタンで見本DB内の前のエントリを表示
                if (trgKey & PAD_BUTTON_L)
                {
                    if (fDictShown)
                    {
                        dictIndex--;
                        if (dictIndex < 0)
                        {
                            dictIndex = protoDB.patternCount - 1;
                        }
                    }
                    fShowPrototypeList = TRUE;
                }

                // R ボタンで見本DB内の次のエントリを表示
                if (trgKey & PAD_BUTTON_R)
                {
                    if (fDictShown)
                    {
                        dictIndex++;
                        if (dictIndex >= protoDB.patternCount)
                        {
                            dictIndex = 0;
                        }
                    }
                    fShowPrototypeList = TRUE;
                }

                // 上キーが押されたら maxPointCount を +4 する
                if (trgKey & PAD_KEY_UP)
                {
                    maxPointCount += 4;
                    break;
                }

                // 下キーが押されたら maxPointCount を -4 する
                if (trgKey & PAD_KEY_DOWN)
                {
                    maxPointCount -= 4;
                    if (maxPointCount < 4)
                    {
                        maxPointCount = 4;
                    }
                    break;
                }

                // 右キーが押されたらリサンプリングパラメータを荒い側に変更する
                if (trgKey & PAD_KEY_RIGHT)
                {
                    if (inputParamIndex < N_INPUT_PARAM - 1)
                    {
                        inputParamIndex++;
                        OS_Printf("change InputPatternParam.\n");
                    }
                    if (!PRC_IsEmpty(&strokes))
                    {
                        // 何か入力されていたら、認識してみる
                        fRecognizeChar = TRUE;
                    }
                }

                // 左キーが押されたらリサンプリングパラメータを細かい側に変更する
                if (trgKey & PAD_KEY_LEFT)
                {
                    if (inputParamIndex > 0)
                    {
                        inputParamIndex--;
                        OS_Printf("change InputPatternParam.\n");
                    }
                    if (!PRC_IsEmpty(&strokes))
                    {
                        // 何か入力されていたら、認識してみる
                        fRecognizeChar = TRUE;
                    }
                }

                if (fRecognizeChar)
                {
                    if (PRC_IsEmpty(&strokes))
                    {
                        OS_Printf("***********************************************\n");
                        OS_Printf("No input.\n");
                    }
                    else
                    {
                        // 図形認識
                        int     iParamType;
                        OSTick  start, end, initPatternTime, recognizeTime_Light,
                            recognizeTime_Standard, recognizeTime_Fine, recognizeTime_Superfine;
                        PRCInputPattern_Common inputPattern;
                        PRCInputPatternParam_Common inputParam;
                        PRCPrototypeEntry *results[RESULT_NUM];
                        fx32    scores[RESULT_NUM];
                        fx32    maxScore;
                        int     maxDictIndex;

                        ClearScreen();
                        fDictShown = FALSE;
                        if (!fClearScreenAfterRecognition)
                        {
                            DrawStrokes(&strokes, 0, 0);
                        }

                        maxScore = 0;
                        maxDictIndex = -1;

                        OS_Printf("***********************************************\n");
                        for (iParamType = 0; iParamType < 3; iParamType++)
                        {
                            OS_Printf("===============================================\n");
                            inputParam.normalizeSize = normalizeSize;
                            switch (iParamType)
                            {
                            case 0:
                                inputParam.resampleMethod = PRC_RESAMPLE_METHOD_DISTANCE;
                                inputParam.resampleThreshold =
                                    sInputPatternParamThresholds[inputParamIndex][iParamType];
                                OS_Printf("Resample Method: PRC_RESAMPLE_METHOD_DISTANCE\n");
                                break;
                            case 1:
                                inputParam.resampleMethod = PRC_RESAMPLE_METHOD_ANGLE;
                                inputParam.resampleThreshold =
                                    sInputPatternParamThresholds[inputParamIndex][iParamType];
                                OS_Printf("Resample Method: PRC_RESAMPLE_METHOD_ANGLE\n");
                                break;
                            case 2:
                                inputParam.resampleMethod = PRC_RESAMPLE_METHOD_RECURSIVE;
                                inputParam.resampleThreshold =
                                    sInputPatternParamThresholds[inputParamIndex][iParamType];
                                OS_Printf("Resample Method: PRC_RESAMPLE_METHOD_RECURSIVE\n");
                                break;
                            }
                            OS_Printf("Resample Threshold: %d\n", inputParam.resampleThreshold);

                            start = OS_GetTick();
                            if (PRC_InitInputPatternEx_Common
                                (&inputPattern, patternBuffer, &strokes, maxPointCount,
                                 STROKE_PER_INPUT_MAX, &inputParam) == FALSE)
                            {
                                end = OS_GetTick();
                                initPatternTime = end - start;
                                OS_Printf("  time: %lldus.\n",
                                          OS_TicksToMicroSeconds(initPatternTime));
                                OS_Printf("  fail to initialize the input pattern.\n");
                                continue;
                            }
                            end = OS_GetTick();
                            initPatternTime = end - start;
                            OS_Printf("  time: %lldus.\n", OS_TicksToMicroSeconds(initPatternTime));

                            if (fPrintPattern)
                            {
                                PRCStrokes outputStrokes;
                                PRC_GetInputPatternStrokes(&outputStrokes, &inputPattern);
                                PrintStrokesAsEntryData(&outputStrokes,
                                                        (fDictShown) ? PatternName[PrototypeList.
                                                                                   entries
                                                                                   [dictIndex].
                                                                                   code] :
                                                        "<New Pattern>",
                                                        (fDictShown) ? PrototypeList.
                                                        entries[dictIndex].kind : 1,
                                                        (fDictShown) ? PrototypeList.
                                                        entries[dictIndex].correction : (fx16)0,
                                                        normalizeSize);
                            }
                            else
                            {
                                // prototypeList と inputPattern を比べて認識を行う
                                start = OS_GetTick();
                                (void)PRC_GetRecognizedEntriesEx_Light(results, scores, RESULT_NUM,
                                                                       recognitionBuffer,
                                                                       &inputPattern, &protoDB,
                                                                       PRC_KIND_ALL, NULL);
                                end = OS_GetTick();
                                recognizeTime_Light = end - start;
                                PrintRecognitionResult("Light", results, scores);
                                OS_Printf("  time: %lldus ( + %lldus (resample) ).\n",
                                          OS_TicksToMicroSeconds(recognizeTime_Light),
                                          OS_TicksToMicroSeconds(initPatternTime));

                                start = OS_GetTick();
                                (void)PRC_GetRecognizedEntriesEx_Standard(results, scores,
                                                                          RESULT_NUM,
                                                                          recognitionBuffer,
                                                                          &inputPattern, &protoDB,
                                                                          PRC_KIND_ALL, NULL);
                                end = OS_GetTick();
                                recognizeTime_Standard = end - start;
                                PrintRecognitionResult("Standard", results, scores);
                                OS_Printf("  time: %lldus ( + %lldus (resample) ).\n",
                                          OS_TicksToMicroSeconds(recognizeTime_Standard),
                                          OS_TicksToMicroSeconds(initPatternTime));

                                start = OS_GetTick();
                                (void)PRC_GetRecognizedEntriesEx_Fine(results, scores, RESULT_NUM,
                                                                      recognitionBuffer,
                                                                      &inputPattern, &protoDB,
                                                                      PRC_KIND_ALL, NULL);
                                end = OS_GetTick();
                                recognizeTime_Fine = end - start;
                                PrintRecognitionResult("Fine", results, scores);
                                OS_Printf("  time: %lldus ( + %lldus (resample) ).\n",
                                          OS_TicksToMicroSeconds(recognizeTime_Fine),
                                          OS_TicksToMicroSeconds(initPatternTime));
                                if (results[0] != NULL && scores[0] > maxScore)
                                {
                                    maxScore = scores[0];
                                    maxDictIndex = (int)results[0]->data;
                                }

                                start = OS_GetTick();
                                (void)PRC_GetRecognizedEntriesEx_Superfine(results, scores,
                                                                           RESULT_NUM,
                                                                           recognitionBuffer,
                                                                           &inputPattern, &protoDB,
                                                                           PRC_KIND_ALL, NULL);
                                end = OS_GetTick();
                                recognizeTime_Superfine = end - start;
                                PrintRecognitionResult("Superfine", results, scores);
                                OS_Printf("  time: %lldus ( + %lldus (resample) ).\n",
                                          OS_TicksToMicroSeconds(recognizeTime_Superfine),
                                          OS_TicksToMicroSeconds(initPatternTime));
                                if (results[0] != NULL && scores[0] > maxScore)
                                {
                                    maxScore = scores[0];
                                    maxDictIndex = (int)results[0]->data;
                                }
                            }

                            {
                                PRCStrokes drawingStrokes;
                                PRC_GetInputPatternStrokes(&drawingStrokes, &inputPattern);
                                DrawStrokes(&drawingStrokes, iParamType * 64, 0);       // normalizeSize <= 128
                            }
                        }
                        if (maxDictIndex >= 0)
                        {
                            PRCStrokes drawingStrokes;
                            dictIndex = maxDictIndex;
                            PRC_GetEntryStrokes(&drawingStrokes, &PrototypeList,
                                                &PrototypeList.entries[dictIndex]);
                            DrawStrokes(&drawingStrokes, 256 - normalizeSize, 0);
                            fDictShown = TRUE;
                        }
                        if (fClearScreenAfterRecognition)
                        {
                            PRC_Clear(&strokes);
                        }
                    }
                }

                if (fShowPrototypeList && PrototypeList.entrySize > 0)
                {
                    // 見本パターンの表示
                    PRCStrokes drawingStrokes;
                    ClearScreen();
                    PRC_Clear(&strokes);

                    PRC_GetEntryStrokes(&drawingStrokes, &PrototypeList,
                                        &PrototypeList.entries[dictIndex]);
                    DrawStrokes(&drawingStrokes, 256 - normalizeSize, 0);
                    OS_Printf("***********************************************\n");
                    OS_Printf("#%d: '%s'\n", dictIndex,
                              PatternName[PrototypeList.entries[dictIndex].code]);
//                    PRCi_PrintPatternData_Common(&protoDB.patterns[dictIndex].data);
                    fDictShown = TRUE;
                }

                if (fPrintPattern && fDictShown && PrototypeList.entrySize > 0)
                {
                    PRCStrokes outputStrokes;
                    PRC_GetEntryStrokes(&outputStrokes, &PrototypeList,
                                        &PrototypeList.entries[dictIndex]);
                    OS_Printf("===============================================\n");
                    OS_Printf("Original PrototypeList:\n");
                    PrintStrokesAsEntryData(&outputStrokes,
                                            PatternName[PrototypeList.entries[dictIndex].code],
                                            PrototypeList.entries[dictIndex].kind,
                                            PrototypeList.entries[dictIndex].correction,
                                            normalizeSize);
                }

                // Wait V-Blank interrupt
                OS_WaitVBlankIntr();

                if (fClearScreen)
                {
                    ClearScreen();
                    fDictShown = FALSE;
                    PRC_Clear(&strokes);
                }

                prevSample = sample;
            }
            OS_Free(recognitionBuffer);
            OS_Free(patternBuffer);
        }

        OS_Free(prototypeListBuffer);
    }
}

void InitOS(void)
{
    void   *nstart;
    void   *heapStart;
    OSHeapHandle handle;

    OS_Init();

    nstart =
        OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(),
                     MEMORY_MAIN_MAX_HEAPS);
    OS_SetMainArenaLo(nstart);

    heapStart = OS_AllocFromMainArenaLo(MEMORY_MAIN_HEAP_SIZE, 32);
    handle =
        OS_CreateHeap(OS_ARENA_MAIN, heapStart, (void *)((u32)heapStart + MEMORY_MAIN_HEAP_SIZE));

    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle);

}

void InitDisplay(void)
{

    GX_Init();
    FX_Init();

    GX_DispOff();
    GXS_DispOff();

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
    (void)OS_EnableIrq();

    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     // clear the standard palette

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
    MI_DmaFill32(3, (void *)HW_LCDC_VRAM_C, 0x7FFF7FFF, 256 * 192 * sizeof(u16));

    GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,      // VRAM mode.
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();
}

void InitTP(void)
{
    TPCalibrateParam calibrate;

    TP_Init();

    // get CalibrateParameter from FlashMemory
    if (!TP_GetUserInfo(&calibrate))
    {
        OS_Panic("Can't get Calibration Parameter from NVRAM\n");
    }
    else
    {
        OS_Printf("Get Calibration Parameter from NVRAM\n");
    }

    TP_SetCalibrateParam(&calibrate);
}

void DrawLine(int x1, int y1, int x2, int y2, u16 col)
{
    u16     sx, sy, ey, wx, wy;
    u16     i;
    int     err, plus, minus;
    u16    *point = VRAM_BASE_ADDR;

    SDK_ASSERT(x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0);

    // swap (x1, y1) <-> (x2, y2) if x1 > x2
    if (x1 <= x2)
    {
        sx = (u16)x1;
        sy = (u16)y1;
        ey = (u16)y2;
        wx = (u16)(x2 - x1);           // width
    }
    else
    {
        sx = (u16)x2;
        sy = (u16)y2;
        ey = (u16)y1;
        wx = (u16)(x1 - x2);           // width
    }

    point += sy * VRAM_LINE_SIZE + sx;
    if (sy <= ey)
    {
        /* line goes to upper */
        wy = (u16)(ey - sy);           // height
        if (wx > wy)
        {
            /* if (X size > Y size) draw a Dot per Xdot */
            plus = wy * 2;
            minus = -wx * 2;
            err = -wx;
            for (i = 0; i <= wx; i++)
            {
                *point = col;          // PutDot
                ++point;
                err += plus;
                if (err >= 0)
                {
                    point += VRAM_LINE_SIZE;
                    err += minus;
                }
            }

        }
        else
        {
            /* if (X size <= Y size) draw a Dot per Ydot */
            plus = wx * 2;
            minus = -wy * 2;
            err = -wy;
            for (i = 0; i <= wy; i++)
            {
                *point = col;          // PutDot
                point += VRAM_LINE_SIZE;
                err += plus;
                if (err >= 0)
                {
                    ++point;
                    err += minus;
                }
            }
        }
    }
    else
    {
        /* line goes to lower */
        wy = (u16)(sy - ey);           // height
        if (wx > wy)
        {
            /* if (X size > Y size) draw a Dot per Xdot */
            plus = wy * 2;
            minus = -wx * 2;
            err = -wx;
            for (i = 0; i <= wx; i++)
            {
                *point = col;          // PutDot
                ++point;
                err += plus;
                if (err >= 0)
                {
                    point -= VRAM_LINE_SIZE;
                    err += minus;
                }
            }

        }
        else
        {
            /* if (X size <= Y size) draw a Dot per Ydot */
            plus = wx * 2;
            minus = -wy * 2;
            err = -wy;
            for (i = 0; i <= wy; i++)
            {
                *point = col;          // PutDot
                point -= VRAM_LINE_SIZE;
                err += plus;
                if (err >= 0)
                {
                    ++point;
                    err += minus;
                }
            }
        }
    }
}

void PutDot(int x, int y, u16 col)
{
    *(u16 *)((u32)VRAM_BASE_ADDR + y * 256 * 2 + x * 2) = col;
}

void ClearScreen(void)
{
    MI_DmaFill32(3, (void *)HW_LCDC_VRAM_C, 0x7FFF7FFF, 256 * 192 * sizeof(u16));
}

void DrawStrokes(const PRCStrokes *strokes, int sx, int sy)
{
    int     iPoint;
    PRCPoint prev;
    BOOL    newFlag;
    const PRCPoint *point;

    newFlag = TRUE;
    point = strokes->points;
    for (iPoint = 0; iPoint < strokes->size; iPoint++, point++)
    {
        if (!PRC_IsPenUpMarker(point))
        {
            if (newFlag)
            {
                PutDot(sx + point->x, sy + point->y, 1);
            }
            else
            {
                DrawLine(sx + prev.x, sy + prev.y, sx + point->x, sy + point->y, 1);
            }
            prev = *point;
            newFlag = FALSE;
        }
        else
        {
            newFlag = TRUE;
        }
    }
}

void PrintRecognitionResult(const char *name, PRCPrototypeEntry **results, fx32 *scores)
{
    int     iResult;
    (void)name;
    (void)scores;

    OS_Printf("-----------------------------------------------\n");
    OS_Printf("Result(%s):\n", name);
//    PRCi_PrintPatternData_Common(&inputPattern.data);

    // 認識結果出力
    if (results[0] == NULL)
    {
        OS_Printf("  can't recognize as char\n");
    }
    else
    {
        for (iResult = 0; iResult < RESULT_NUM; iResult++)
        {
            int     code;

            if (results[iResult] == NULL)
                break;

            code = PRC_GetEntryCode(results[iResult]);
            OS_Printf("  '%s' (score: %d.%03d)\n", PatternName[code], FX_Whole(scores[iResult]),
                      (scores[iResult] & FX16_DEC_MASK) * 1000 / (1 << FX16_DEC_SIZE));
        }
    }
}

void PrintStrokesAsEntryData(const PRCStrokes *strokes,
                             const char *name, u32 kind, fx16 correction, int normalizeSize)
{
    int     iPoint;
    BOOL    newFlag;
    const PRCPoint *point;
    (void)name;
    (void)kind;
    (void)correction;
    (void)normalizeSize;

    OS_Printf("\"%s\" %d %d %d |", name, kind, correction, normalizeSize);

    newFlag = TRUE;
    point = strokes->points;
    for (iPoint = 0; iPoint < strokes->size; iPoint++, point++)
    {
        if (!PRC_IsPenUpMarker(point))
        {
            OS_Printf(" (%d, %d)", point->x, point->y);
            newFlag = FALSE;
        }
        else
        {
            OS_Printf(" |");
            newFlag = TRUE;
        }
    }
    if (!newFlag)
    {
        OS_Printf(" |");
    }
    OS_Printf("\n");
}

void VBlankIntr(void)
{
    // Set IRQ check flag
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
