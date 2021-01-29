/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     main.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2007/03/09 01:16:16  kitase_hirotake
  indent source

  Revision 1.9  2006/03/15 04:44:46  adachi_hiroaki
  表示をssl-1にした

  Revision 1.8  2006/03/15 04:14:35  adachi_hiroaki
  alloc/free must be givenの警告が出ないようにした

  Revision 1.7  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.6  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.5  2005/10/25 04:19:19  terui
  SVC_WaitVBlankIntr -> OS_WaitVBlankIntr

  Revision 1.4  2005/08/09 00:59:08  seiki_masashi
  行末の'能'を回避

  Revision 1.3  2005/08/08 05:55:36  seiki_masashi
  CPS_AddRandomSeed 関数をメインループ内で呼び出すように変更

  Revision 1.2  2005/07/29 09:49:16  seiki_masashi
  small fix

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 に移動

  Revision 1.3  2005/07/21 11:18:45  seiki_masashi
  random_seed を RTC から取得するように変更

  Revision 1.2  2005/07/18 23:56:12  seiki_masashi
  ボタンごとに機能割付け

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 デモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    一般的な無線アクセスポイント ( AP ) に接続した上で、CPS ライブラリの SSL
    機能を使用して SSL クライアントとして SSL サーバと通信を行うサンプルです。
    AP の情報は NitroWiFi/build/demos/shared/ap_info.c に記述されておりますので、
    この情報を適宜書き換えて使用して下さい。BSSID、ESSID を固定値ではない任意値
    に設定すると、一番始めに見つかった AP に接続を試みます。
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "screen.h"
#include "wcm_control.h"
#include "wcm_cpsif.h"
#include "tcp_client.h"

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

static void     InitNetwork(void);
static void     inform_ip(void);
static void     GetPage(void);

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
    OS_InitTick();          // OS_Sleep 関数を使用する為に必要
    OS_InitAlarm();         // OS_Sleep 関数を使用する為に必要
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    RTC_Init();

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
    OS_TPrintf("\"ssl-1\" demo started.\n");
    PrintSubScreen("\"ssl-1\" demo started.\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    ReadKey(&gKey);

    InitNetwork();

    while (TRUE)
    {
        // キー入力情報取得
        ReadKey(&gKey);

        // メイン画面クリア
        ClearMainScreen();

        ProcMain();
        inform_ip();

        // 16 フレームごとに SSL 用の乱数の種をかき混ぜる
        if (OS_GetVBlankCount() % 16 == 0)
        {
            u32 data[OS_LOW_ENTROPY_DATA_SIZE/sizeof(u32)];
            OS_GetLowEntropyData(data);

            // CPS_SslAddRandomSeed は CPS 初期化前でも呼び出し可能 
            CPS_SslAddRandomSeed(data, sizeof(data));
        }

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
        PutMainScreen(7, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push A to connect");

        //if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
        {
            InitWcmControl();
        }
        break;

    case WCM_PHASE_DCF:
        // 接続された状態
        ((u16*)HW_PLTT)[0] = 0x7ff0;
        PutMainScreen(11, 10, 0x0, "Now online");
        PutMainScreen(6, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push A to disconnect");
        PutMainScreen(6, 13, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push B to get a page");
        if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
        {
            TerminateWcmControl();
        }
        else if ((gKey.trg | gKey.rep) & PAD_BUTTON_B)
        {
            GetPage();
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
        PutMainScreen(9, 12, (u8) (((OS_GetVBlankCount() / 30) % 2) + 7), "Push A to stop");
        if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
        {
            TerminateWcmControl();
        }
    }

    if ((gKey.trg | gKey.rep) & PAD_BUTTON_X)
    {
        OS_DumpThreadList();
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

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
//  Initialize network
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
static CPSConfig    netconfig;

//
//  Callback routine called:
//
//  1) immediately after CPS_Startup() if CPS_DONOTUSE_DHCP is on
//
//      or
//
//  2) after the attempt to look for a DHCP server fails
//     if CPS_DONOTUSE_DHCP is off

//
static void dhcp_callback()
{
    OS_TPrintf("dhcp_callback() entered\n");
    CPSMyIp = CPS_MK_IPv4(192, 168, 11, 200);
    CPSNetMask = CPS_MK_IPv4(255, 255, 255, 0);
}

//
//  Returns TRUE if the link is ON

//
static BOOL link_is_on()
{
    return WCM_GetApMacAddress() != NULL;
}

static void* my_alloc(u32 size)
{
    void*       p;

    OSIntrMode  enabled = OS_DisableInterrupts();
    p = OS_Alloc(size);
    (void)OS_RestoreInterrupts(enabled);
    return p;
}

static void my_free(void* p)
{
    OSIntrMode  enabled = OS_DisableInterrupts();
    OS_Free(p);
    (void)OS_RestoreInterrupts(enabled);
}

static u8   lan_buf[16384];

static void InitNetwork()
{
    WCM_SetRecvDCFCallback(CPSi_RecvCallbackFunc);
    netconfig.dhcp_callback = dhcp_callback;
    netconfig.link_is_on = link_is_on;
    netconfig.alloc = my_alloc;
    netconfig.free = my_free;
    netconfig.lan_buf = lan_buf;
    netconfig.lan_buflen = sizeof(lan_buf);
    OS_Printf("OS_GetTick() == %lld\n", OS_GetTick());
    {
        RTCDate date;
        RTCTime time;
        s64     sec;
        (void)RTC_GetDateTime(&date, &time);
        sec = RTC_ConvertDateTimeToSecond(&date, &time);
        if (sec >= 0)
        {
            netconfig.random_seed = (u32) sec;
        }
        else
        {
            netconfig.random_seed = 0ULL;
        }
    }
    {
        u32 data[OS_LOW_ENTROPY_DATA_SIZE/sizeof(u32)];
        OS_GetLowEntropyData(data);
        CPS_SslAddRandomSeed(data, sizeof(data));
    }

    //  netconfig.mode = CPS_DONOTUSE_DHCP;
    CPS_Startup(&netconfig);
}

static void inform_ip()
{
    char        tmp[128];
    static int  have_ip = 0;

    if (have_ip)
    {
        if (CPSMyIp == 0)
        {
            have_ip = 0;    // lost IP
        }

        return;
    }
    else if (CPSMyIp == 0)
    {
        return;             // IP not yet established
    }

    have_ip = 1;
    (void)OS_SPrintf(tmp, "MAC  = %02x:%02x:%02x:%02x:%02x:%02x\n", CPSMyMac[0], CPSMyMac[1], CPSMyMac[2], CPSMyMac[3],
                     CPSMyMac[4], CPSMyMac[5]);
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "ip   = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSMyIp));
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "mask = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSNetMask));
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "gwip = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSGatewayIp));
    PrintSubScreen(tmp);
    (void)OS_SPrintf(tmp, "DNS1 = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSDnsIp[0]));
    PrintSubScreen(tmp);
    if (CPSDnsIp[1] != 0)
    {
        (void)OS_SPrintf(tmp, "   2 = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSDnsIp[1]));
        PrintSubScreen(tmp);
    }
    (void)OS_SPrintf(tmp, "DHCP = %d.%d.%d.%d\n", CPS_CV_IPv4(CPSDhcpServerIp));
    PrintSubScreen(tmp);
}

static void GetPage(void)
{
    // 再入可能ではない
    static HttpClientRequest    req;
    req.host = "192.168.0.10";
    req.path = "/";
#if 1
    req.port = 4433;
    req.https = TRUE;
#else
    req.port = 80;
    req.https = FALSE;
#endif
    StartHttpClient(&req);
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
