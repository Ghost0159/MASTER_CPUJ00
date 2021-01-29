/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - demos - wcm-1
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.8  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.7  2005/10/25 04:19:19  terui
  SVC_WaitVBlankIntr -> OS_WaitVBlankIntr

  Revision 1.6  2005/09/05 11:27:43  terui
  Passive スキャンでサーチするように変更。

  Revision 1.5  2005/07/21 09:32:14  terui
  Fix comment.

  Revision 1.4  2005/07/21 08:41:08  terui
  デバッグ用表示を追加

  Revision 1.3  2005/07/18 02:27:13  terui
  Fix comment.

  Revision 1.2  2005/07/13 00:50:06  terui
  Fix comment.

  Revision 1.1  2005/07/11 12:07:09  terui
  新規追加。

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WCM ライブラリを使用して 一般的な無線アクセスポイント ( AP ) に接続する
    サンプルです。BSSID、ESSID、802.11 認証方式、WEP暗号化設定 が判明している
    固定の AP に対して 802.11 の DCF モードで接続します。
    AP の情報は NitroWiFi/build/demos/shared/ap_info.c に記述されておりますので、
    この情報を適宜書き換えて使用して下さい。BSSID、ESSID を固定値ではない任意値
    に設定すると、一番始めに見つかった AP に接続を試みます。

    HOWTO:  (1) 起動後、何かボタンを押すと AP へ接続します。
            (2) 接続中に何かボタンを押すと接続をキャンセルします。
            (3) 接続完了後、何かボタンを押すと AP から切断します。
            (4) 接続完了後に AP から切断された場合などは自動的に再接続を試みます。
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "screen.h"
#include "wcm_control.h"

/*---------------------------------------------------------------------------*
    定数 定義
 *---------------------------------------------------------------------------*/
#define KEY_REPEAT_START    25  // キーリピート開始までのフレーム数
#define KEY_REPEAT_SPAN     10  // キーリピートの間隔フレーム数

/*---------------------------------------------------------------------------*
    構造体 定義
 *---------------------------------------------------------------------------*/

// キー入力情報
typedef struct KeyInfo
{
    u16 cnt;    // 未加工入力値
    u16 trg;    // 押しトリガ入力
    u16 up;     // 離しトリガ入力
    u16 rep;    // 押し維持リピート入力
} KeyInfo;

/*---------------------------------------------------------------------------*
    内部変数 定義
 *---------------------------------------------------------------------------*/

// キー入力
static KeyInfo  gKey;

/*---------------------------------------------------------------------------*
    内部関数 プロトタイプ
 *---------------------------------------------------------------------------*/
static void     ProcMain(void);
static void     VBlankIntr(void);
static void     InitInterrupts(void);
static void     InitHeap(void);
static void     ReadKey(KeyInfo* pKey);

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
    OS_InitTick();  // OS_Sleep 関数を使用する為に必要
    OS_InitAlarm(); // OS_Sleep 関数を使用する為に必要
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // メモリ割り当て初期化
    InitHeap();

    // 表示設定初期化
    InitScreen();

    // 割り込み設定初期化
    InitInterrupts();

    // LCD 表示開始
    GX_DispOn();
    GXS_DispOn();

    // デバッグ文字列出力
    OS_TPrintf("\"wcm-1\" demo started.\n");
    PrintSubScreen("\"wcm-1\" demo started.\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    ReadKey(&gKey);

    while (TRUE)
    {
        // キー入力情報取得
        ReadKey(&gKey);

        // メイン画面クリア
        ClearMainScreen();

        ProcMain();

        // Ｖブランク待ち ( スレッド対応 )
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         ProcMain

  Description:  WCM ライブラリの接続管理状態の表示、及び入力による処理の
                切り替えを行う。メインループからピクチャーフレームごとに
                呼び出される。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ProcMain(void)
{
    switch (WCM_GetPhase())
    {
    case WCM_PHASE_NULL:
        // 初期状態 もしくは WCM コントロールスレッドが終了した状態
        ((u16*)HW_PLTT)[0] = 0x7fff;
        PutMainScreen(11, 10, 0x0, "Now offline");
        PutMainScreen(4, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to connect");
        if (gKey.trg | gKey.rep)
        {
            SetWcmScanType(WCM_OPTION_SCANTYPE_PASSIVE);
            InitWcmControl();
            PrintSubScreen("InitWcmControl()\n");
        }
        break;

    case WCM_PHASE_DCF:
        // 接続された状態
        ((u16*)HW_PLTT)[0] = 0x7ff0;
        PutMainScreen(11, 10, 0x0, "Now online");
        PutMainScreen(3, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to disconnect");
        if (gKey.trg | gKey.rep)
        {
            TerminateWcmControl();
            PrintSubScreen("TerminateWcmControl()\n");
        }
        break;

    case WCM_PHASE_FATAL_ERROR:
        // 復旧不可能な FATAL ERROR
        ((u16*)HW_PLTT)[0] = 0x001f;
        PutMainScreen(0, 7, 0xf, "================================");
        PutMainScreen(7, 10, 0xf, "WCM FATAL ERROR !");
        PutMainScreen(2, 12, 0xf, "Turn off power supply please.");
        PutMainScreen(0, 15, 0xf, "================================");
        OS_WaitVBlankIntr();
        OS_Terminate();
        break;

    default:
        // 処理中
        ((u16*)HW_PLTT)[0] = 0x6318;
        PutMainScreen(10, 10, 0x0, "Now working...");
        PutMainScreen(6, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to stop");
        if (gKey.trg | gKey.rep)
        {
            TerminateWcmControl();
            PrintSubScreen("TerminateWcmControl()\n");
        }
    }
}

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Ｖブランク割込みハンドラ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // テキスト表示を更新
    UpdateScreen();

    // IRQ チェックフラグをセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitInterrupts

  Description:  割り込み設定を初期化する。
                V ブランク割り込みを許可し、割り込みハンドラを設定する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitInterrupts(void)
{
    // V ブランク割り込み設定
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);

    // 割り込み許可
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

/*---------------------------------------------------------------------------*
  Name:         InitHeap

  Description:  メインメモリ上のアリーナにてメモリ割当てシステムを初期化する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitHeap(void)
{
    void*           tempLo;
    OSHeapHandle    hh;

    // メインメモリ上のアリーナにヒープをひとつ作成
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        // ヒープ作成に失敗した場合は異常終了
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         ReadKey

  Description:  キー入力情報を取得し、入力情報構造体を編集する。
                押しトリガ、離しトリガ、押し継続リピートトリガ を検出する。

  Arguments:    pKey  - 編集するキー入力情報構造体を指定する。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ReadKey(KeyInfo* pKey)
{
    static u16  repeat_count[12];
    int         i;
    u16         r;

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
                    repeat_count[i] = (u16) (KEY_REPEAT_START - KEY_REPEAT_SPAN);
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

    pKey->cnt = r;  // 未加工キー入力
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
