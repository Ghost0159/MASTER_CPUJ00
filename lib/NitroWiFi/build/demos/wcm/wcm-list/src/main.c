/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - demos - wcm-list
  File:     main.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.5  2005/10/25 04:19:19  terui
  SVC_WaitVBlankIntr -> OS_WaitVBlankIntr

  Revision 1.4  2005/08/29 06:39:36  yosizaki
  add SetWcmManualConnect().

  Revision 1.3  2005/07/25 07:48:46  adachi_hiroaki
  一覧リセットボタン追加

  Revision 1.2  2005/07/22 01:07:40  adachi_hiroaki
  デフォルトのBSSIDがANYの場合は選択モードにしか移行しないようにした

  Revision 1.1  2005/07/21 08:21:06  adachi_hiroaki
  新規追加


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WCM ライブラリを使用して 一般的な無線アクセスポイント ( AP ) を検索して
    リストアップし、接続するサンプルです。
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

//デフォルトの接続先BSSID
static u8       gBssidDefault[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static int      gSelectedAP = 0;
static BOOL     gConnectAuto = FALSE;

/*---------------------------------------------------------------------------*
    内部関数 プロトタイプ
 *---------------------------------------------------------------------------*/
static void     ProcMain(void);
static void     SelectAP(void);
static const WMBssDesc*     GetBssDesc(const u8* bssid);
static void VBlankIntr(void);
static void InitInterrupts(void);
static void InitHeap(void);
static void ReadKey(KeyInfo* pKey);

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
    OS_TPrintf("\"wcm-list\" demo started.\n");
    PrintSubScreen("\"wcm-list\" demo started.\n");

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
        PutMainScreen(4, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to search AP");
        if (gKey.trg | gKey.rep)
        {
            SetWcmManualConnect(TRUE);
            InitWcmControl();
        }

        if (!WCM_CompareBssID(WCM_BSSID_ANY, gBssidDefault))
        {
            PutMainScreen(5, 23, 0x0, "Start button: auto connect");
            if (gKey.trg & PAD_BUTTON_START)
            {
                gConnectAuto = TRUE;
            }
            else
            {
                gConnectAuto = FALSE;
            }
        }
        break;

    case WCM_PHASE_SEARCH:
        // APを検索中の状態
        if (gConnectAuto)
        {
            // 自動接続モード
            PutMainScreen(11, 10, 0x0, "Now online");
            PutMainScreen(4, 11, 0x0, "connecting to default AP");
            PutMainScreen(3, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push any key to select AP");
            if (gKey.trg | gKey.rep)
            {
                // キーが押されていればAP選択モードへ
                gConnectAuto = FALSE;
            }
            else
            {
                //デフォルトのBssIDを持つAPを探す
                const WMBssDesc*    pbd = GetBssDesc(gBssidDefault);
                if (pbd)
                {
                    //見つかれば接続する
                    ConnectAP(pbd);
                }
            }
        }
        else
        {
            // AP選択モード
            SelectAP();
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
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         SelectAP

  Description:  利用できるアクセスポイント一覧を表示し選択する

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SelectAP(void)
{
    int     nIndex = 0;
    int     nAPCount = 0;
    char    buffer[256];
    s32     nLevel;
    BOOL    bDefault = FALSE;

    PutMainScreen(0, 0, 0xf, " bssid      + ssid/link");
    PutMainScreen(0, 23, 0xf, "A:connect, Sel:set def, St:clear");

    (void)WCM_LockApList(WCM_APLIST_LOCK);
    nAPCount = WCM_CountApList();
    if (nAPCount == 0)
    {
        //APが一個も見つからなければ表示・選択しない
        (void)WCM_LockApList(WCM_APLIST_UNLOCK);
        return;
    }

    //選択カーソルの移動
    if (gKey.trg & PAD_KEY_DOWN)
    {
        ++gSelectedAP;
    }

    if (gKey.trg & PAD_KEY_UP)
    {
        --gSelectedAP;
    }

    gSelectedAP = MATH_CLAMP(gSelectedAP, 0, nAPCount - 1);
    {
        WMBssDesc*  pbdSelected = WCM_PointApList(gSelectedAP);
        if (pbdSelected)
        {
            if (gKey.trg & PAD_BUTTON_A)
            {
                ConnectAP(pbdSelected);
            }

            if (gKey.trg & PAD_BUTTON_SELECT)
            {
                MI_CpuCopy8(pbdSelected->bssid, gBssidDefault, WM_SIZE_BSSID);
            }
        }
    }

    for (nIndex = 0; nIndex < nAPCount; ++nIndex)
    {
        WMBssDesc*  pbd = WCM_PointApList(nIndex);
        bDefault = WCM_CompareBssID(gBssidDefault, pbd->bssid);
        nLevel = WCM_PointApListLinkLevel(nIndex);
        (void)OS_SPrintf(buffer, "%02X%02X%02X%02X%02X%02X%s%s/%d", pbd->bssid[0], pbd->bssid[1], pbd->bssid[2],
                         pbd->bssid[3], pbd->bssid[4], pbd->bssid[5], bDefault ? "*" : " ", pbd->ssid, nLevel);
        PutMainScreen(0, 1 + nIndex, (u8) (nIndex == gSelectedAP ? 0x1 : 0xf), buffer);
    }
    (void)WCM_LockApList(WCM_APLIST_UNLOCK);

    if (gKey.trg & PAD_BUTTON_START)
    {
        WCM_ClearApList();
    }

}

/*---------------------------------------------------------------------------*
  Name:         GetBssDesc

  Description:  指定されたBssIDを持ったAPが見つかっていればそのBssDescを得る。

  Arguments:    BssDescを得たいAPのBssID

  Returns:      見つかったAPのBssDescへのポインタ
 *---------------------------------------------------------------------------*/
static const WMBssDesc* GetBssDesc(const u8* bssid)
{
    int nAPCount = 0;
    int nIndex = 0;

    (void)WCM_LockApList(WCM_APLIST_LOCK);
    nAPCount = WCM_CountApList();
    for (nIndex = 0; nIndex < nAPCount; ++nIndex)
    {
        WMBssDesc*  pbd = WCM_PointApList(nIndex);
        if (WCM_CompareBssID((u8*)bssid, pbd->bssid))
        {
            (void)WCM_LockApList(WCM_APLIST_UNLOCK);
            return pbd;
        }
    }
    (void)WCM_LockApList(WCM_APLIST_UNLOCK);
    return NULL;
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
