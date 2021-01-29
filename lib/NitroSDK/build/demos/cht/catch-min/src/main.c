/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CHT - demos - catch-min
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.3  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/21 10:20:04  kitase_hirotake
  OS_InitPrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.1  2005/07/14 01:34:48  kitase_hirotake
  Initial Release

  Revision 1.1  2004/12/22 02:42:43  terui
  Initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    ピクトキャッチ機能により、ピクトチャット親機のビーコンを検出するデモです。
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/wm.h>
#include    <nitro/cht.h>

#include    "font.h"
#include    "icon.h"
#include    "wmscan.h"

/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // キーリピート開始までのフレーム数
#define     KEY_REPEAT_SPAN     10     // キーリピートの間隔フレーム数

#define     DEFAULT_GGID        0x003fff61

#define     PICTO_CATCH_LIFETIME    1200        // 20秒
#define     PICTO_SCAN_FREQUENCE    20 //  1/3秒 (3チャンネルで1秒)

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

typedef struct PictoCatchInfo
{
    int     lifeTime;
    int     clientNum;

}
PictoCatchInfo;


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
// 親機発見コールバック
static void FoundParentCallback(WMBssDesc *pBssDesc);

// Vブランク割込みハンドラ
static void VBlankIntr(void);

// 汎用サブルーチン
static void KeyRead(KeyInfo * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);
static void InitializeAllocateSystem(void);
static void DrawIcon(u8 index, int charName, int x, int y);


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static u16 gScreen[32 * 32] ATTRIBUTE_ALIGN(32);
static GXOamAttr gOam[128] ATTRIBUTE_ALIGN(32);
static KeyInfo gKey;                   // キー入力
static s32 gFrame;                     // フレームカウンタ
static s32 gScanCount;

static PictoCatchInfo gRoom[4];


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
    GX_SetBankForOBJ(GX_VRAM_OBJ_16_F);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    G2_SetBG0Priority(0);
    G2_BG0Mosaic(FALSE);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    GX_LoadOBJ(icons_character, 0, sizeof(icons_character));
    GX_LoadOBJPltt(icons_palette, 0, sizeof(icons_palette));
    MI_CpuFillFast((void *)gScreen, 0, sizeof(gScreen));
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // 割込み設定
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // メモリ割当
    InitializeAllocateSystem();

    while (1)
    {
        u8     *wmbuf;

        //********************************
        // 無線初期化
        wmbuf = (u8 *)OS_Alloc(WM_SYSTEM_BUF_SIZE);
        OS_TPrintf("Alloc %d byte\n", WM_SYSTEM_BUF_SIZE);

        if (!WS_Initialize(wmbuf, 3))
        {
            OS_TPanic("WS_Initialize failed.\n");
        }
        WS_TurnOnPictoCatch();

        {
            const u8 mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
            s32     i;

            for (i = 0; i < 4; i++)
            {
                gRoom[i].lifeTime = 0;
            }
            // 初期化完了待ち
            while (WS_GetSystemState() != WS_SYSSTATE_IDLE)
            {
            }
            // 親機探索開始
            if (!WS_StartScan(FoundParentCallback, mac, FALSE))
            {
                OS_TPanic("WS_StartScan failed.\n");
            }
            gScanCount = 0;
        }
        //********************************

        // LCD表示開始
        GX_DispOn();
        GXS_DispOn();

        // デバッグ文字列出力
        OS_Printf("ARM9: CHT catch-min demo started.\n");

        // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
        KeyRead(&gKey);

        // メインループ
        for (gFrame = 0; TRUE; gFrame++)
        {
            // キー入力情報取得
            KeyRead(&gKey);

            if (++gScanCount == PICTO_SCAN_FREQUENCE)
            {
                const u8 mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
                if (!WS_StartScan(FoundParentCallback, mac, FALSE))
                {
                    OS_TPanic("WS_StartScan failed.\n");
                }
                gScanCount = 0;
            }

            // スクリーンクリア
            ClearString();

            // 表示
            PrintString(1, 1, 0xf, "frame: %d", gFrame);
            {
                s32     i;

                for (i = 0; i < 4; i++)
                {
                    if (gRoom[i].lifeTime > 0)
                    {
                        gRoom[i].lifeTime--;
                        PrintString(1, (s16)(6 + (3 * i)), 0xf, "Discover pictochat room%d", i);
                        PrintString(2, (s16)(7 + (3 * i)), 0x2, "%d members", gRoom[i].clientNum);
                        // アイコンを表示
                        DrawIcon((u8)i, 9, 208, (s16)(48 + (24 * i)));
                    }
                    else
                    {
                        PrintString(10, (s16)(6 + (3 * i)), 0xe, "pictochat room%d", i);
                    }
                }
            }

            if (gKey.trg == PAD_BUTTON_B)
            {
                break;
            }

            // Ｖブランク待ち
            OS_WaitVBlankIntr();
            // ARM7のデバッグプリントを出力
        }

        // ピクトキャッチ終了処理
        if (!WS_EndScan())
        {
            OS_TPanic("WS_EndScan failed.\n");
        }
        while (WS_GetSystemState() != WS_SYSSTATE_IDLE)
        {
        }

        if (!WS_End())
        {
            OS_TPanic("WS_End failed.\n");
        }
        while (WS_GetSystemState() != WS_SYSSTATE_STOP)
        {
        }
        OS_Free(wmbuf);
    }
}

/*---------------------------------------------------------------------------*
  Name:         FoundParentCallback

  Description:  親機発見時に呼び出されるコールバック関数。

  Arguments:    pBssDesc    -   発見した親機のビーコン情報へのポインタ。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void FoundParentCallback(WMBssDesc *pBssDesc)
{
    if (CHT_IsPictochatParent(pBssDesc))
    {
        int     clientNum;
        int     roomNumber;

        clientNum = CHT_GetPictochatClientNum(pBssDesc);
        roomNumber = CHT_GetPictochatRoomNumber(pBssDesc);
        if (roomNumber < 4)
        {
            if (gRoom[roomNumber].lifeTime == 0)
            {
                /* SE should start here */
            }
            gRoom[roomNumber].clientNum = clientNum;
            gRoom[roomNumber].lifeTime = PICTO_CATCH_LIFETIME;
        }
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
    // 仮想スクリーンをVRAMに反映
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // 仮想 OAM を OAM に反映
    DC_FlushRange(gOam, sizeof(gOam));
    GX_LoadOAM(gOam, 0, sizeof(gOam));

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
    s32     i;

    MI_CpuClearFast((void *)gScreen, sizeof(gScreen));
    for (i = 0; i < 128; i++)
    {
        G2_SetOBJPosition(&gOam[i], 256, 192);
    }
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
  Name:         DrawIcon

  Description:  アイコンを表示する。

  Arguments:    index       - 仮想 OAM の配列 index を指定。
                charName    - アイコン番号を指定。
                                0 ～ 3 : 白地の電波受信強度アイコン
                                4 ～ 7 : 黒地の電波受信強度アイコン
                                8      : 通信アイコン
                                9      : ピクトチャットアイコン
                x           - X座標位置をドット単位で指定
                y           - Y座標位置をドット単位で指定

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DrawIcon(u8 index, int charName, int x, int y)
{
    G2_SetOBJAttr(&gOam[index],        // 編集する OAM 構造体へのポインタ
                  x,                   // X 位置
                  y,                   // Y 位置
                  0,                   // 表示プライオリティ 最高
                  GX_OAM_MODE_NORMAL,  // ノーマル OBJ
                  FALSE,               // モザイクなし
                  GX_OAM_EFFECT_NONE,  // エフェクトなし
                  GX_OAM_SHAPE_16x16,  // 2 × 2 キャラクタ
                  GX_OAM_COLORMODE_16, // 16色パレット
                  charName * 4,        // キャラクタ番号
                  0,                   // パレット番号 0
                  0                    // アフィン変換インデックス(無効)
        );
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
