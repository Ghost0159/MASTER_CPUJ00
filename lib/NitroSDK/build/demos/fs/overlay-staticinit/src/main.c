/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.3  2005/11/21 10:23:49  kitase_hirotake
  OS_InitPrintServer の削除

  Revision 1.2  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:45  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#include <nitro.h>

/* 特に明示的に使用しているライブラリ */
#include <nitro/os.h>
#include <nitro/fs.h>
#include <nitro/pad.h>
#include <nitro/spi.h>


/* 画面描画用のルーチン */
#include "draw.h"

/* 各モードのインタフェース */
#include "mode.h"


/*****************************************************************************/
/* constant */

/* 自動サンプリング用定数 */
#define     SAMPLING_FREQUENCE      4  // Touch panel sampling frequence per frame
#define     SAMPLING_BUFSIZE        (SAMPLING_FREQUENCE + 1)    // AutoSampling buffer size
#define     SAMPLING_START_VCOUNT   0  // base vcount value in auto sampling.


/*****************************************************************************/
/* variable */

/* 各モードのインタフェース */
BOOL    (*UpdateFrame) (int frame_count, const InputData * input, int player_count,
                        int own_player_id);
void    (*DrawFrame) (int frame);
void    (*EndFrame) (FSOverlayID *p_next_mode);

/* タッチパネル用自動サンプリングバッファ */
static TPData tp_auto_buf[SAMPLING_BUFSIZE];


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  デフォルトのVブランク処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitApp

  Description:  アプリケーション基本部分の初期化.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitApp(void)
{
    /* OS の初期化 */
    OS_Init();
    OS_InitThread();
    OS_InitTick();
    /* アリーナの初期化 */
    {
        OSHeapHandle hh;
        void   *tmp = OS_InitAlloc(OS_ARENA_MAIN,
                                   OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
        OS_SetArenaLo(OS_ARENA_MAIN, tmp);
        hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
        if (hh < 0)
            OS_Panic("ARM9: Fail to create heap...\n");
        hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
    }
    /* 割り込みを有効に */
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    /* タッチパネルの初期化 */
    {
        TPCalibrateParam calibrate;
        TP_Init();
        if (TP_GetUserInfo(&calibrate))
            TP_SetCalibrateParam(&calibrate);
        (void)TP_RequestAutoSamplingStart(SAMPLING_START_VCOUNT, SAMPLING_FREQUENCE, tp_auto_buf,
                                          SAMPLING_BUFSIZE);
    }
    /* ファイルシステムの初期化 */
    FS_Init(FS_DMA_NOT_USE);
}

/*---------------------------------------------------------------------------*
  Name:         GetInput

  Description:  現在の入力状態を取得します.

  Arguments:    input             取得した情報の格納先.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GetInput(InputData * input)
{
    if (input)
    {
        const u16 hold_bak = input->hold_bits;
        u16     hold_bits;
        /* タッチパネル情報を更新 */
        const TPData *const cur_tp = tp_auto_buf + TP_GetLatestIndexInAuto();
        if (cur_tp->touch == TP_TOUCH_OFF)
            input->tp.touch = TP_TOUCH_OFF;
        else if (cur_tp->validity == TP_VALIDITY_VALID)
            TP_GetCalibratedPoint(&input->tp, cur_tp);
        /* パッド情報(およびタッチペン接触ビット)を更新 */
        hold_bits = (u16)(PAD_Read() | (input->tp.touch ? PAD_BUTTON_TP : 0));
        input->push_bits = (u16)(~hold_bak & hold_bits);
        input->hold_bits = hold_bits;
        input->release_bits = (u16)(hold_bak & ~hold_bits);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  アプリケーションのメインエントリポイント.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    /* 画面遷移の共通変数 */
    enum
    { PLAYER_MAX = 1 };                /* プレーヤー最大数 (通信対戦等を考慮) */
    int     player_count;              /* プレーヤー総数 */
    int     own_player_id;             /* 自身のプレーヤー番号 */
    InputData input[PLAYER_MAX];       /* 入力情報配列 */
    FSOverlayID cur_mode;              /* 前回のモード */
    FSOverlayID prev_mode;             /* 現在のモード */
    FS_EXTERN_OVERLAY(top_menu);       /* 初期モード (トップメニュー) */

    /*
     * アプリケーションフレームワークと画面遷移状態を初期化します.
     */
    {
        InitApp();
        InitDraw();
        SetClearColor(RGB555_CLEAR);
        cur_mode = prev_mode = FS_OVERLAY_ID(top_menu);
        UpdateFrame = NULL;
        DrawFrame = NULL;
        EndFrame = NULL;
        MI_CpuClear8(input, sizeof(input));
        player_count = 1;
        own_player_id = 0;
    }

    /*
     * オーバーレイとして各モードを構成し, それらを繰り返し遷移します.
     */
    for (;;)
    {

        /*
         * 現在のモードを開始します.
         * ロード完了とともに NitroStaticInit() で自動初期化され
         * 必要なインタフェースが自己設定されます.
         * モードの終了条件は, 現在のモード自身が UpdateFrame で返します.
         */
        {
            int     frame = 0;
            if (!FS_LoadOverlay(MI_PROCESSOR_ARM9, cur_mode))
            {
                OS_TPanic("failed to load specified mode(%08X)", cur_mode);
            }
            GetInput(&input[own_player_id]);
            for (;;)
            {
                GetInput(&input[own_player_id]);
                if (!UpdateFrame(frame, input, player_count, own_player_id))
                    break;
                DrawBegin();
                DrawFrame(frame);
                DrawEnd();
                ++frame;
            }
        }

        /*
         * 現在のモードを終了し, 次のモードへ移行します.
         * 現在のモードが移行先を明示的に設定しなかった場合,
         * 前回のモードへ戻ることになります.
         */
        {
            FSOverlayID next_mode = prev_mode;
            EndFrame(&next_mode);
            if (!FS_UnloadOverlay(MI_PROCESSOR_ARM9, cur_mode))
            {
                OS_TPanic("failed to unload specified mode(%08X)", cur_mode);
            }
            prev_mode = cur_mode;
            cur_mode = next_mode;
        }

    }

}
