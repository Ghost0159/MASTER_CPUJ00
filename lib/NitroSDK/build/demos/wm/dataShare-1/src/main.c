/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-1
  File:     main.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.15  2007/11/27 06:32:49  okubata_ryoma
  Copyright fix

  Revision 1.14  2007/10/29 09:29:24  seiki_masashi
  コメントの修正

  Revision 1.13  2006/01/20 02:05:39  seiki_masashi
  WMParentParam 構造体のコメントを現状に合うように修正

  Revision 1.12  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.11  2005/11/21 11:06:47  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.10  2005/06/27 11:10:13  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.9  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.8  2004/10/22 11:06:44  terui
  wcモジュールを無線共有領域に切り出し、I/Fが変更されたことに対応。

  Revision 1.7  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.6  2004/08/17 12:25:28  seiki_masashi
  WM_ERRCODE_FRAME_GAP の廃止に伴う修正

  Revision 1.5  2004/08/12 14:57:11  seiki_masashi
  WM_StepDataSharing の返り値の WM_ERRCODE_FRAME_GAP に対応

  Revision 1.4  2004/08/11 10:04:16  seiki_masashi
  StepDataSharing に失敗した際の待ちフレームを 1 に設定

  Revision 1.3  2004/08/11 08:02:06  terui
  WcInitにゲームフレームのVBlank回数を指定するよう改造。

  Revision 1.2  2004/08/10 06:38:40  terui
  Fix comment.

  Revision 1.1  2004/08/10 00:41:54  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WMライブラリを使用した無線によるデータシェア通信を行うサンプルです。
    自動的に周辺のdataShare-1デモと接続し、4バイトのゲームフレームカウントと
    4バイトのピクチャーフレームカウントの計8バイトを無線通信グループ内で
    共有します。

    HOWTO:
        1. 親機として通信を開始する場合はＡボタンを押して下さい。
           周辺に同じdataShare-1デモの子機が見つかり次第、自動的にその子機と
           データシェア通信を開始します。同時に通信できる子機は15台までです。
        2. 子機として通信を開始する場合はＢボタンを押して下さい。
           周辺に同じdataShare-1デモの親機が見つかり次第、自動的にその親機と
           通信を開始します。
        3. 接続中のビジー画面や、接続完了後の通信画面においてＳＴＡＲＴボタンを
           押すことで接続がリセットされ、初期画面に戻ります。
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/wm.h>

#include    "font.h"
#include    "wc.h"


/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // キーリピート開始までのフレーム数
#define     KEY_REPEAT_SPAN     10     // キーリピートの間隔フレーム数

#define     PICTURE_FRAME_PER_GAME_FRAME    2

#define     DEFAULT_GGID            0x003fff11
#define     NUM_MAX_CHILD           15
#define     DEFAULT_CHAN            1
#define     SHARED_DATA_SIZE        8
#define     PARENT_DATA_SIZE_MAX    ( SHARED_DATA_SIZE * ( 1 + NUM_MAX_CHILD ) + 4 )
#define     CHILD_DATA_SIZE_MAX     SHARED_DATA_SIZE

static const WMParentParam defaultParameter = {
    NULL, 0, 0,
    DEFAULT_GGID,                      // ゲームグループID
    0x0000,                            // temporary ID (毎回+1される)
    1,                                 // エントリー許可フラグ
    NUM_MAX_CHILD,                     // 最大接続子機数
    0,                                 // マルチブートフラグ (通常は0)
    0,                                 // (未使用)
    0,                                 // 連続転送フラグ (WM は解釈しませんが、WC がこの設定を MP 頻度設定に反映します)
    200,                               // beacon 間隔
    {0, 0, 0, 0},                      // ユーザー名
    {0, 0, 0, 0, 0, 0, 0, 0},          // ゲーム名
    DEFAULT_CHAN,                      // 接続チャンネル
    PARENT_DATA_SIZE_MAX,              // 親機送信データサイズ
    CHILD_DATA_SIZE_MAX                // 子機送信データサイズ
};

/*---------------------------------------------------------------------------*
    構造体定義
 *---------------------------------------------------------------------------*/
// キー入力情報
typedef struct KeyInfo
{
    u16     cnt;                       // 未加工入力値
    u16     trg;                       // 押しトリガ入力
    u16     up;                        // 離しトリガ入力
    u16     rep;                       // 押し維持リピート入力

}
KeyInfo;


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
static void GameFrame(void);           // ゲームフレームでのメインループ
static void ModeSelect(void);          // 親機/子機 選択画面
static void ModeError(void);           // エラー表示画面
static void ModeWorking(void);         // ビジー画面
static void ModeParent(void);          // 親機 通信画面
static void ModeChild(void);           // 子機 通信画面
static void VBlankIntr(void);          // Vブランク割込みハンドラ
static void PrintSharedData(u16 bitmap);        // シェアされたデータの表示

// 汎用サブルーチン
static void KeyRead(KeyInfo * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);
static void InitializeAllocateSystem(void);


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static u16 gScreen[32 * 32];           // 仮想スクリーン
static KeyInfo gKey;                   // キー入力
static vs32 gPictureFrame;             // ピクチャーフレーム カウンタ
static vs32 gGameFrame;                // ゲームフレーム カウンタ

// 送信データバッファ (32-Byte Align されている必要がある)
static u16 gSendBuf[SHARED_DATA_SIZE / sizeof(u16)] ATTRIBUTE_ALIGN(32);


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

    // 文字列表示用に２Ｄ表示設定
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800,      // SCR ベースブロック 31
                     GX_BG_CHARBASE_0x00000,    // CHR ベースブロック 0
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // 変数初期化
    gPictureFrame = 0;

    // 割込み設定
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // メモリ割当システム初期化
    InitializeAllocateSystem();

    //********************************
    // 無線初期化
    WcInit(&defaultParameter, TRUE, FALSE, PICTURE_FRAME_PER_GAME_FRAME);
    //********************************

    // LCD表示開始
    GX_DispOn();
    GXS_DispOn();

    // デバッグ文字列出力
    OS_Printf("ARM9: WM data sharing demo started.\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    KeyRead(&gKey);

    // メインループ
    GameFrame();                       // never return
}

/*---------------------------------------------------------------------------*
  Name:         GameFrame

  Description:  ゲームフレームのメインループ

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void GameFrame(void)
{
    for (gGameFrame = 0; TRUE; gGameFrame++)
    {
        // キー入力情報取得
        KeyRead(&gKey);

        // 通信状態により処理を振り分け
        switch (WcGetStatus())
        {
        case WC_STATUS_READY:
            ModeSelect();
            break;
        case WC_STATUS_ERROR:
            ModeError();
            break;
        case WC_STATUS_BUSY:
            ModeWorking();
            break;
        case WC_STATUS_PARENT:
            ModeParent();
            break;
        case WC_STATUS_CHILD:
            ModeChild();
            break;
        }

        // 複数回ピクチャーフレームの完了を待つ
        while (TRUE)
        {
            // Vブランク待ち
            OS_WaitVBlankIntr();
            if (!(gPictureFrame % PICTURE_FRAME_PER_GAME_FRAME))
            {
                break;
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  親機/子機 選択画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeSelect(void)
{
    // スクリーンクリア
    ClearString();
    // 文字表示
    PrintString(3, 10, 0xf, "Push A to connect as PARENT");
    PrintString(3, 12, 0xf, "Push B to connect as CHILD");

    if (gKey.trg & PAD_BUTTON_A)
    {
        //********************************
        // 親機通信開始
        WcStartParent();
        //********************************
    }
    else if (gKey.trg & PAD_BUTTON_B)
    {
        //********************************
        // 子機接続開始
        WcStartChild();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  エラー表示画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeError(void)
{
    // スクリーンクリア
    ClearString();
    // 文字表示
    PrintString(5, 10, 0x1, "======= ERROR! =======");
    PrintString(5, 13, 0xf, " Fatal error occured.");
    PrintString(5, 14, 0xf, "Please reboot program.");
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  ビジー画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeWorking(void)
{
    // スクリーンクリア
    ClearString();
    // 文字表示
    PrintString(9, 11, 0xf, "Now working...");

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        // 通信終了
        WcEnd();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  親機 通信画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeParent(void)
{
    BOOL    result;
    u16     bitmap;

    // 送信データ編集
    gSendBuf[0] = (u16)(gPictureFrame >> 16);
    gSendBuf[1] = (u16)(gPictureFrame & 0xffff);
    gSendBuf[2] = (u16)(gGameFrame >> 16);
    gSendBuf[3] = (u16)(gGameFrame & 0xffff);

    //********************************
    // データシェア同期
    result = WcStepDataSharing((void *)gSendBuf, &bitmap);
    //********************************

    // スクリーンクリア
    ClearString();
    // 文字表示
    PrintString(8, 1, 0x2, "Parent mode");
    // シェアされたデータを表示
    if (result)
    {
        PrintSharedData(bitmap);
    }
    else
    {
        // 次のフレームも StepDataSharing にトライ
        gPictureFrame--;
    }

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        // 通信終了
        WcEnd();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  子機 通信画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChild(void)
{
    BOOL    result;
    u16     bitmap;

    // 送信データ編集
    gSendBuf[0] = (u16)(gPictureFrame >> 16);
    gSendBuf[1] = (u16)(gPictureFrame & 0xffff);
    gSendBuf[2] = (u16)(gGameFrame >> 16);
    gSendBuf[3] = (u16)(gGameFrame & 0xffff);

    //********************************
    // データシェア同期
    result = WcStepDataSharing((void *)gSendBuf, &bitmap);
    //********************************

    // スクリーンクリア
    ClearString();
    // 文字表示
    PrintString(8, 1, 0x2, "Child mode");
    // シェアされたデータを表示
    if (result)
    {
        PrintSharedData(bitmap);
    }
    else
    {
        // 次のフレームも StepDataSharing にトライ
        gPictureFrame--;
    }

    if (gKey.trg & PAD_BUTTON_START)
    {
        //********************************
        // 通信終了
        WcEnd();
        //********************************
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Ｖブランク割込みベクトル。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // ピクチャーフレームカウンタをインクリメント
    gPictureFrame++;

    // ゲームフレームに合わせて再描画
    if (!(gPictureFrame % PICTURE_FRAME_PER_GAME_FRAME))
    {
        // 仮想スクリーンをVRAMに反映
        DC_FlushRange(gScreen, sizeof(gScreen));
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
    }

    // IRQ チェックフラグをセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         PrintSharedData

  Description:  共有された各端末からのデータを表示する。

  Arguments:    bitmap - データを受信した端末を示すIDビットマップを指定。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintSharedData(u16 bitmap)
{
    u16     i;
    u8     *p;
    u16     temp[SHARED_DATA_SIZE / sizeof(u16)];

    // 親機 + 子機最大数分をループ
    for (i = 0; i < (1 + WM_NUM_MAX_CHILD); i++)
    {
        if (bitmap & (0x0001 << i))
        {
            //********************************
            // IDが"i"の端末からのデータ取得
            p = WcGetSharedDataAddress(i);
            //********************************

            if (p != NULL)
            {
                // アラインを確実にするため、一旦コピー
                MI_CpuCopy8(p, temp, SHARED_DATA_SIZE);
                PrintString(2, (s16)(3 + i), 0x4,
                            "%04x%04x-%04x%04x", temp[0], temp[1], temp[2], temp[3]);
            }
            else
            {
                PrintString(2, (s16)(3 + i), 0x1, "xxxxxxxx-xxxxxxxx");
            }
        }
    }
    // 自機のデータだけ色を変更
    ColorString(2, (s16)(3 + WcGetAid()), 17, 0xf);
}

/*---------------------------------------------------------------------------*
  Name:         KeyRead

  Description:  キー入力情報を編集する。
                押しトリガ、離しトリガ、押し継続リピートを検出。

  Arguments:    pKey  - 編集するキー入力情報構造体。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void KeyRead(KeyInfo * pKey)
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
  Name:         ClearString

  Description:  仮想スクリーンをクリアする。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ClearString(void)
{
    MI_CpuClearFast((void *)gScreen, sizeof(gScreen));
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  仮想スクリーンに文字列を配置する。文字列は32文字まで。

  Arguments:    x       - 文字列の先頭を配置する x 座標( × 8 ドット )。
                y       - 文字列の先頭を配置する y 座標( × 8 ドット )。
                palette - 文字の色をパレット番号で指定。
                text    - 配置する文字列。終端文字はNULL。
                ...     - 仮想引数。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...)
{
    va_list vlist;
    char    temp[32 + 2];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    *(u16 *)(&temp[32]) = 0x0000;
    for (i = 0;; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }
        gScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  仮想スクリーンに配置した文字列の色を変更する。

  Arguments:    x       - 色変更を開始する x 座標( × 8 ドット )。
                y       - 色変更を開始する y 座標( × 8 ドット )。
                length  - 連続して色変更する文字数。
                palette - 文字の色をパレット番号で指定。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ColorString(s16 x, s16 y, s16 length, u8 palette)
{
    s32     i;
    u16     temp;
    s32     index;

    if (length < 0)
        return;

    for (i = 0; i < length; i++)
    {
        index = ((y * 32) + x + i) % (32 * 32);
        temp = gScreen[index];
        temp &= 0x0fff;
        temp |= (palette << 12);
        gScreen[index] = temp;
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
  End of file
 *---------------------------------------------------------------------------*/
