/*---------------------------------------------------------------------------*
  Project:  NitroSDK - RTC - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 11:02:12  kitase_hirotake
  OS_InitPrintServer , OS_PrintServer , PXI_Init の削除
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.7  2005/02/28 05:26:11  yosizaki
  do-indent.

  Revision 1.6  2004/10/06 00:27:21  terui
  Fix comment.

  Revision 1.5  2004/09/16 05:45:55  terui
  FINALROM版かつIS-NITRO-EMULATORでアラームが正常に動作しない注意書きを追加。

  Revision 1.4  2004/08/23 07:27:27  yasu
  Workaround for pushing A button in IPL

  Revision 1.3  2004/08/19 11:15:50  terui
  時間表記の24時間表記固定化に伴う改造。

  Revision 1.2  2004/05/24 02:23:33  terui
  Bug fix. (Called GX_LoadBGxxxx in IRQ before initialize.)

  Revision 1.1  2004/05/12 02:38:49  terui
  initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    NOTICE: 当サンプルにはRTCのアラーム機能の動作を確認する機能が含まれており
            ますが、2004/10/06現在、IS-NITRO-DEBUGGERソフトウェアのバージョンが
            Ver 1.11 より前のバージョンであると正常に動作しない場合があります。
            具体的には発生するべきアラームが発生しないという現象が起こります。
            この現象はNintendoDS本体とIS-NITRO-EMULATORのハードウェア上での
            競合に起因しておりますので、量産試作品などの実機では問題なく動作
            します。
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/rtc.h>

#include    "font.h"


/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define     KEY_REPEAT_START    25     // キーリピート開始までのフレーム数
#define     KEY_REPEAT_SPAN     10     // キーリピートの間隔フレーム数


/*---------------------------------------------------------------------------*
    文字列定数定義
 *---------------------------------------------------------------------------*/
// 曜日
const char *StrWeek[7] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

// アラーム状態
const char *StrAlarmStatus[2] = {
    "OFF",
    "ON"
};

// アラーム有効フラグ
const char *StrAlarmEnable[8] = {
    "- - -",                           // 有効フラグなし
    "W - -",                           // 週       設定が有効
    "- H -",                           //    時    設定が有効
    "W H -",                           // 週 時    設定が有効
    "- - M",                           //       分 設定が有効
    "W - M",                           // 週    分 設定が有効
    "- H M",                           //    時 分 設定が有効
    "W H M"                            // 週 時 分 設定が有効
};


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

// 編集モード
typedef enum EditMode
{
    EDIT_MODE_NONE = 0,                // 編集なしモード
    EDIT_MODE_DATE_TIME,               // 日付＆時刻編集モード
    EDIT_MODE_ALARM_1,                 // アラーム１状態＆設定値編集モード
    EDIT_MODE_ALARM_2,                 // アラーム２状態＆設定値編集モード
    EDIT_MODE_MAX
}
EditMode;


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);
static void AlarmIntrCallback(void);

static void IncValue(void);
static void DecValue(void);
static void IncSelect(void);
static void DecSelect(void);

static void KeyRead(KeyInformation * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static u16 gScreen[32 * 32];           // 仮想スクリーン
static KeyInformation gKey;            // キー入力

static RTCDate gRtcDate;               // 日付
static RTCTime gRtcTime;               // 時刻
static RTCAlarmStatus gRtcAlarmStatus1; // アラーム１状態
static RTCAlarmParam gRtcAlarmParam1;  // アラーム１設定値
static RTCAlarmStatus gRtcAlarmStatus2; // アラーム２状態
static RTCAlarmParam gRtcAlarmParam2;  // アラーム２設定値

static EditMode gEditMode;             // 編集モード
static u32 gSelect;                    // 選択箇所インデックス
static s32 gMosaic;                    // モザイク表示カウンタ


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

    // 割込み設定
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();


    //****************************************************************
    // RTC初期化
    RTC_Init();
    // アラーム発生時の飛び先ベクトルを設定
    RTC_SetAlarmInterrupt(AlarmIntrCallback);
    //****************************************************************

    // 内部変数初期化
    gEditMode = EDIT_MODE_NONE;
    gSelect = 0;
    gMosaic = 0;

    // LCD表示開始
    GX_DispOn();
    GXS_DispOn();

    // デバッグ文字列出力
    OS_Printf("ARM9: RTC demo started.\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    KeyRead(&gKey);

    // メインループ
    while (TRUE)
    {
        // キー入力情報取得
        KeyRead(&gKey);

        // スクリーンクリア
        ClearString();

        //****************************************************************
        // 現在の状態をRTCから読み出し
        if (gEditMode != EDIT_MODE_DATE_TIME)
        {
            // 日付・時刻を読み出し
            (void)RTC_GetDateTime(&gRtcDate, &gRtcTime);
        }
        if (gEditMode != EDIT_MODE_ALARM_1)
        {
            // アラーム１状態を読み出し
            (void)RTC_GetAlarmStatus(RTC_ALARM_CHAN_1, &gRtcAlarmStatus1);
            if (gRtcAlarmStatus1 == RTC_ALARM_STATUS_ON)
            {
                // アラーム１が ON ならば設定値を読み出し
                (void)RTC_GetAlarmParam(RTC_ALARM_CHAN_1, &gRtcAlarmParam1);
            }
        }
        if (gEditMode != EDIT_MODE_ALARM_2)
        {
            // アラーム２状態を読み出し
            (void)RTC_GetAlarmStatus(RTC_ALARM_CHAN_2, &gRtcAlarmStatus2);
            if (gRtcAlarmStatus2 == RTC_ALARM_STATUS_ON)
            {
                // アラーム２が ON ならば設定値を読み出し
                (void)RTC_GetAlarmParam(RTC_ALARM_CHAN_2, &gRtcAlarmParam2);
            }
        }
        //****************************************************************

        // 日付＆時刻を表示
        PrintString(7, 3, 0xf, "%04d/%02d/%02d/%s",
                    gRtcDate.year + 2000, gRtcDate.month, gRtcDate.day, StrWeek[gRtcDate.week]);
        PrintString(9, 5, 0xf, " %02d:%02d:%02d", gRtcTime.hour, gRtcTime.minute, gRtcTime.second);
        // 日付＆時刻の編集モードの場合、選択箇所の色を変更
        if (gEditMode == EDIT_MODE_DATE_TIME)
        {
            switch (gSelect)
            {
            case 0:
                ColorString(7, 3, 4, 0x1);
                break;
            case 1:
                ColorString(12, 3, 2, 0x1);
                break;
            case 2:
                ColorString(15, 3, 2, 0x1);
                break;
            case 3:
                ColorString(18, 3, 9, 0x1);
                break;
            case 4:
                ColorString(10, 5, 2, 0x1);
                break;
            case 5:
                ColorString(13, 5, 2, 0x1);
                break;
            case 6:
                ColorString(16, 5, 2, 0x1);
                break;
            }
        }

        // アラーム１状態＆設定値を表示
        PrintString(3, 8, 0x4, "ALARM-1 %s", StrAlarmStatus[gRtcAlarmStatus1]);
        PrintString(4, 10, 0xf, "% 9s  %02d:%02d  %s",
                    StrWeek[gRtcAlarmParam1.week],
                    gRtcAlarmParam1.hour,
                    gRtcAlarmParam1.minute, StrAlarmEnable[gRtcAlarmParam1.enable]);
        // アラーム１の編集モードの場合、選択箇所の色を変更
        if (gEditMode == EDIT_MODE_ALARM_1)
        {
            switch (gSelect)
            {
            case 0:
                ColorString(11, 8, 6, 0x1);
                break;
            case 1:
                ColorString(4, 10, 9, 0x1);
                break;
            case 2:
                ColorString(15, 10, 2, 0x1);
                break;
            case 3:
                ColorString(18, 10, 2, 0x1);
                break;
            case 4:
                ColorString(22, 10, 5, 0x1);
                break;
            }
        }

        // アラーム２状態＆設定値を表示
        PrintString(3, 12, 0x4, "ALARM-2 %s", StrAlarmStatus[gRtcAlarmStatus2]);
        PrintString(4, 14, 0xf, "% 9s  %02d:%02d  %s",
                    StrWeek[gRtcAlarmParam2.week],
                    gRtcAlarmParam2.hour,
                    gRtcAlarmParam2.minute, StrAlarmEnable[gRtcAlarmParam2.enable]);
        // アラーム２編集モードの場合、選択箇所の色を変更
        if (gEditMode == EDIT_MODE_ALARM_2)
        {
            switch (gSelect)
            {
            case 0:
                ColorString(11, 12, 6, 0x1);
                break;
            case 1:
                ColorString(4, 14, 9, 0x1);
                break;
            case 2:
                ColorString(15, 14, 2, 0x1);
                break;
            case 3:
                ColorString(18, 14, 2, 0x1);
                break;
            case 4:
                ColorString(22, 14, 5, 0x1);
                break;
            }
        }

        // ボタン操作説明文を表示
        if (gEditMode == EDIT_MODE_NONE)
        {
            PrintString(1, 19, 0x3, "DOWN  > Change Date & Time.");
            PrintString(1, 20, 0x3, "LEFT  > Change Alarm1 Setting.");
            PrintString(1, 21, 0x3, "RIGHT > Change Alarm2 Setting.");
        }
        else
        {
            PrintString(1, 18, 0x3, "UP    > Increment value.");
            PrintString(1, 19, 0x3, "DOWN  > Decrement value.");
            PrintString(1, 20, 0x3, "LEFT  > Move selector left.");
            PrintString(1, 21, 0x3, "RIGHT > Move selector right.");
            PrintString(1, 22, 0x3, "A     > Commit editing.");
            PrintString(1, 23, 0x3, "B     > Cancel editing.");
        }

        // ボタン入力操作
        if (gEditMode == EDIT_MODE_NONE)
        {
            // ↓ キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                // 日付＆時刻編集モードへ移行
                gSelect = 0;
                gEditMode = EDIT_MODE_DATE_TIME;
            }
            // ← キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_LEFT)
            {
                // アラーム１状態＆設定値編集モードへ移行
                gSelect = 0;
                gEditMode = EDIT_MODE_ALARM_1;
            }
            // → キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_RIGHT)
            {
                // アラーム２状態＆設定値編集モードへ移行
                gSelect = 0;
                gEditMode = EDIT_MODE_ALARM_2;
            }
        }
        else                           // 編集モード共通キー操作
        {
            // ↑ キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_UP)
            {
                IncValue();
            }
            // ↓ キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_DOWN)
            {
                DecValue();
            }
            // ← キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_LEFT)
            {
                DecSelect();
            }
            // → キー
            if ((gKey.trg | gKey.rep) & PAD_KEY_RIGHT)
            {
                IncSelect();
            }
            // Ａ ボタン
            if ((gKey.trg | gKey.rep) & PAD_BUTTON_A)
            {
                //****************************************************************
                // RTCに変更した変数を書き込み
                switch (gEditMode)
                {
                case EDIT_MODE_DATE_TIME:
                    // 日付・時刻を書き込み
                    (void)RTC_SetDateTime(&gRtcDate, &gRtcTime);
                    break;
                case EDIT_MODE_ALARM_1:
                    // アラーム１の状態を書き込み
                    (void)RTC_SetAlarmStatus(RTC_ALARM_CHAN_1, &gRtcAlarmStatus1);
                    if (gRtcAlarmStatus1 == RTC_ALARM_STATUS_ON)
                    {
                        // アラーム１が ON ならば設定値を書き込み
                        (void)RTC_SetAlarmParam(RTC_ALARM_CHAN_1, &gRtcAlarmParam1);
                    }
                    break;
                case EDIT_MODE_ALARM_2:
                    // アラーム２の状態を書き込み
                    (void)RTC_SetAlarmStatus(RTC_ALARM_CHAN_2, &gRtcAlarmStatus2);
                    if (gRtcAlarmStatus2 == RTC_ALARM_STATUS_ON)
                    {
                        // アラーム２が ON ならば設定値を書き込み
                        (void)RTC_SetAlarmParam(RTC_ALARM_CHAN_2, &gRtcAlarmParam2);
                    }
                    break;
                }
                //****************************************************************

                // 編集なしモードへ移行
                gEditMode = EDIT_MODE_NONE;
            }
            // Ｂ ボタン
            if ((gKey.trg | gKey.rep) & PAD_BUTTON_B)
            {
                // 編集なしモードへ移行
                gEditMode = EDIT_MODE_NONE;
            }
        }

        // Ｖブランク待ち
        OS_WaitVBlankIntr();
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
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    // モザイク表示カウンタに従いモザイク表示
    if (gMosaic)
    {
        int     w;
        int     i = 5;

        w = gMosaic / 4 % (i * 2);
        w = w - (w - ((w - i) * 2)) * (w / (i + 1));
        gMosaic--;

        G2_BG0Mosaic(TRUE);
        G2_SetBGMosaicSize(w, w);
    }
    else
    {
        G2_BG0Mosaic(FALSE);
    }

    // IRQ チェックフラグをセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         AlarmIntrCallback

  Description:  RTCアラーム発生時の割込みベクトル。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void AlarmIntrCallback(void)
{
    // デバッグ文字列出力
    OS_Printf("ARM9: Alarm interrupt on %04d/%02d/%02d (%s) %02d:%02d:%02d .\n",
              gRtcDate.year + 2000,
              gRtcDate.month,
              gRtcDate.day,
              StrWeek[gRtcDate.week], gRtcTime.hour, gRtcTime.minute, gRtcTime.second);
    // モザイクアニメーション開始
    gMosaic = 300;
}

/*---------------------------------------------------------------------------*
  Name:         IncValue

  Description:  編集モードにより、選択されている値を＋１する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void IncValue(void)
{
    switch (gEditMode)
    {

        // 日付＆時刻編集モード
    case EDIT_MODE_DATE_TIME:
        {
            RTCDate *d = &gRtcDate;
            RTCTime *t = &gRtcTime;

            switch (gSelect)
            {
            case 0:
                d->year = (d->year + 1) % 100;
                break;
            case 1:
                d->month = (d->month % 12) + 1;
                break;
            case 2:
                d->day = (d->day % 31) + 1;
                break;
            case 3:
                d->week = (RTCWeek)((d->week + 1) % RTC_WEEK_MAX);
                break;
            case 4:
                t->hour = (t->hour + 1) % 24;
                break;
            case 5:
                t->minute = (t->minute + 1) % 60;
                break;
            case 6:
                t->second = (t->second + 1) % 60;
                break;
            }
        }
        break;

        // アラーム１or２編集モード
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        {
            RTCAlarmStatus *s;
            RTCAlarmParam *p;

            s = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmStatus1 : &gRtcAlarmStatus2);
            p = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmParam1 : &gRtcAlarmParam2);

            switch (gSelect)
            {
            case 0:
                *s = (RTCAlarmStatus)((*s + 1) % RTC_ALARM_STATUS_MAX);
                break;
            case 1:
                p->week = (RTCWeek)((p->week + 1) % RTC_WEEK_MAX);
                break;
            case 2:
                p->hour = (p->hour + 1) % 24;
                break;
            case 3:
                p->minute = (p->minute + 1) % 60;
                break;
            case 4:
                p->enable = (p->enable + 1) % (RTC_ALARM_ENABLE_ALL + 1);
                break;
            }
        }
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         DecValue

  Description:  編集モードにより、選択されている値を－１する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DecValue(void)
{
    switch (gEditMode)
    {

        // 日付＆時刻編集モード
    case EDIT_MODE_DATE_TIME:
        {
            RTCDate *d = &gRtcDate;
            RTCTime *t = &gRtcTime;

            switch (gSelect)
            {
            case 0:
                d->year = (d->year + 99) % 100;
                break;
            case 1:
                d->month = ((d->month + 10) % 12) + 1;
                break;
            case 2:
                d->day = ((d->day + 29) % 31) + 1;
                break;
            case 3:
                d->week = (RTCWeek)((d->week + (RTC_WEEK_MAX - 1)) % RTC_WEEK_MAX);
                break;
            case 4:
                t->hour = (t->hour + 23) % 24;
                break;
            case 5:
                t->minute = (t->minute + 59) % 60;
                break;
            case 6:
                t->second = (t->second + 59) % 60;
                break;
            }
        }
        break;

        // アラーム１or２編集モード
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        {
            RTCAlarmStatus *s;
            RTCAlarmParam *p;

            s = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmStatus1 : &gRtcAlarmStatus2);
            p = (gEditMode == EDIT_MODE_ALARM_1 ? &gRtcAlarmParam1 : &gRtcAlarmParam2);

            switch (gSelect)
            {
            case 0:
                *s = (RTCAlarmStatus)((*s + (RTC_ALARM_STATUS_MAX - 1)) % RTC_ALARM_STATUS_MAX);
                break;
            case 1:
                p->week = (RTCWeek)((p->week + (RTC_WEEK_MAX - 1)) % RTC_WEEK_MAX);
                break;
            case 2:
                p->hour = (p->hour + 23) % 24;
                break;
            case 3:
                p->minute = (p->minute + 59) % 60;
                break;
            case 4:
                p->enable = (p->enable + RTC_ALARM_ENABLE_ALL) % (RTC_ALARM_ENABLE_ALL + 1);
                break;
            }
        }
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         IncSelect

  Description:  編集モードにより、選択インデックスを＋１する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void IncSelect(void)
{
    switch (gEditMode)
    {

        // 日付＆時刻編集モード
    case EDIT_MODE_DATE_TIME:
        gSelect = (gSelect + 1) % 7;
        break;

        // アラーム１or２編集モード
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        gSelect = (gSelect + 1) % 5;
        break;

    }
}

/*---------------------------------------------------------------------------*
  Name:         DecSelect

  Description:  編集モードにより、選択インデックスを－１する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DecSelect(void)
{
    switch (gEditMode)
    {

        // 日付＆時刻編集モード
    case EDIT_MODE_DATE_TIME:
        gSelect = (gSelect + 6) % 7;
        break;

        // アラーム１or２編集モード
    case EDIT_MODE_ALARM_1:
    case EDIT_MODE_ALARM_2:
        gSelect = (gSelect + 4) % 5;
        break;

    }
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
  End of file
 *---------------------------------------------------------------------------*/
