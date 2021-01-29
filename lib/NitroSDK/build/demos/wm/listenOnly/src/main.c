/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - listenOnly
  File:     main.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2007/11/15 13:26:43  seiki_masashi
  警告に対処

  Revision 1.3  2007/11/12 04:33:17  seiki_masashi
  画面表示を追加

  Revision 1.2  2007/11/09 14:24:41  seiki_masashi
  small fix

  Revision 1.1  2007/11/09 14:09:00  seiki_masashi
  initial upload


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    WMライブラリを受信限定モードで初期化し、スキャンを行う単機能サンプルです。
 *---------------------------------------------------------------------------*/

#include    <nitro.h>
#include    <nitro/wm.h>

#include    "font.h"

/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define     BEACON_INFO_MAX     256    // スキャンで蓄積する AP 情報の最大数

#define     WIRELESS_DMA_NO     2      // GX の DMA 番号と被らないように注意

#define     KEY_REPEAT_START    25     // キーリピート開始までのフレーム数
#define     KEY_REPEAT_SPAN     10     // キーリピートの間隔フレーム数

enum {
    STATE_NONE = 0,
    STATE_INITIALIZING,
    STATE_IDLE,
    STATE_SCANNING,
    STATE_TERMINATING_SCAN,
    STATE_FATAL
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

// AP 情報
typedef struct BeaconInfo
{
    u32 timeStamp_found;
    u32 timeStamp_updated;
    BOOL isDS;
    u8 bssid[WM_SIZE_BSSID];
    u8 ssid[WM_SIZE_SSID];
    u8 padding[2];
    u32 channel;
}
BeaconInfo;

/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void);          // Vブランク割込みハンドラ

static void SetState(u32 state);
static u32 GetState(void);

static BOOL InitializeWMForListeningAndStartScan(void);
static void InitializeCb(void *arg);
static BOOL StartScan(void);
static void StartScanExCb(void *arg);
static void EndScanCb(void* arg);

// 汎用サブルーチン
static void KeyRead(KeyInfo * pKey);
static void ClearString(void);
static void PrintString(s16 x, s16 y, u8 palette, char *text, ...);
static void ColorString(s16 x, s16 y, s16 length, u8 palette);
static void InitializeAllocateSystem(void);


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static u32 sState = STATE_NONE;
static BeaconInfo sBeaconInfo[BEACON_INFO_MAX];

/* WM 用システムワーク領域バッファ */
static u8 sWmBuffer[WM_SYSTEM_BUF_SIZE] ATTRIBUTE_ALIGN(32);
static u8 sScanBuf[WM_SIZE_SCAN_EX_BUF] ATTRIBUTE_ALIGN(32);
static WMScanExParam sScanExParam ATTRIBUTE_ALIGN(32);

static u16 sScreen[32 * 32];           // 仮想スクリーン
static KeyInfo sKey;                   // キー入力


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
    MI_CpuFillFast((void *)sScreen, 0, sizeof(sScreen));
    DC_FlushRange(sScreen, sizeof(sScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(sScreen, 0, sizeof(sScreen));

    // 割込み設定
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();

    // メモリ割当
    InitializeAllocateSystem();

    MI_CpuClear8(sBeaconInfo, sizeof(sBeaconInfo));

    // 無線初期化
    if (! InitializeWMForListeningAndStartScan() )
    {
        OS_Panic("Cannot initialize WM");
    }

    // LCD表示開始
    GX_DispOn();
    GXS_DispOn();

    // デバッグ文字列出力
    OS_Printf("ARM9: WM listenOnly demo started.\n");

    // キー入力情報取得の空呼び出し(IPL での A ボタン押下対策)
    KeyRead(&sKey);

    // メインループ
    while (GetState() != STATE_FATAL)
    {
        // キー入力情報取得
        KeyRead(&sKey);

        // スクリーンクリア
        ClearString();

        // 現在のステートの表示
        switch (GetState())
        {
        case STATE_IDLE:
            PrintString( 14, 0, 6, "IDLE" );
            break;

        case STATE_SCANNING:
            PrintString( 14, 0, 5, "SCAN" );
            break;

        case STATE_FATAL:
            PrintString( 14, 0, 1, "ERROR" );
            break;

        default:
            PrintString( 13, 0, 7, "Busy..." );
            break;

        }

        // 各チャンネルの状況表示
        {
            int i;
            u32 beaconCountAP[13];
            u32 beaconCountDS[13];
            BOOL isNewAP[13];
            BOOL isNewDS[13];
            u32 currentVCount = OS_GetVBlankCount();
            OSIntrMode e;

            MI_CpuClear8(beaconCountAP, sizeof(beaconCountAP));
            MI_CpuClear8(beaconCountDS, sizeof(beaconCountDS));
            MI_CpuClear8(isNewAP, sizeof(isNewAP));
            MI_CpuClear8(isNewDS, sizeof(isNewDS));

            // 割り込みで更新されるデータに触るので排他する
            e = OS_DisableInterrupts();
            for (i = 0; i < BEACON_INFO_MAX; i++)
            {
                if (sBeaconInfo[i].timeStamp_found != 0)
                {
                    if (1 <= sBeaconInfo[i].channel && sBeaconInfo[i].channel <= 13)
                    {
                        if ((u32)(currentVCount - sBeaconInfo[i].timeStamp_updated) > 60 * 60)
                        {
                            // 1分更新の無かったエントリは削除
                            MI_CpuClear8(&sBeaconInfo[i], sizeof(sBeaconInfo[i]));
                        }
                        else
                        {
                            if (sBeaconInfo[i].isDS)
                            {
                                beaconCountDS[sBeaconInfo[i].channel-1]++;
                            }
                            else
                            {
                                beaconCountAP[sBeaconInfo[i].channel-1]++;
                            }
                            // 1秒以内に見つかったものがあれば、表示を強調する
                            if ((u32)(currentVCount - sBeaconInfo[i].timeStamp_found) < 60 * 1)
                            {
                                if (sBeaconInfo[i].isDS)
                                {
                                    isNewDS[sBeaconInfo[i].channel-1] = TRUE;
                                }
                                else
                                {
                                    isNewAP[sBeaconInfo[i].channel-1] = TRUE;
                                }
                            }
                        }
                    }
                }
            }
            (void)OS_RestoreInterrupts(e);

            for (i = 0; i < 13; i++)
            {
                s16 y = (s16)(5 + i);
                u8 color;
                PrintString( 5, y, 4, "Ch%2d: ", i+1 );
                color = (u8)((beaconCountAP[i] > 0) ? ((isNewAP[i]) ? 1 : 15) : 13);
                PrintString( 5 + 7, y, color, "%3d APs", beaconCountAP[i]);
                color = (u8)((beaconCountDS[i] > 0) ? ((isNewDS[i]) ? 1 : 15) : 13);
                PrintString( 5 + 7 + 8, y, color, "%3d DSs", beaconCountDS[i]);
            }
        }

        // Ｖブランク待ち
        OS_WaitVBlankIntr();
    }

    OS_Terminate();
}

static void SetState(u32 state)
{
    if (sState != STATE_FATAL)
    {
        sState = state;
    }
}

static inline u32 GetState(void)
{
    return sState;
}

static BOOL FindBeaconInfo(const u8* bssid, BeaconInfo** beaconInfo)
{
    int i, j;
    BeaconInfo* emptyEntry = NULL;

    for (i = 0; i < BEACON_INFO_MAX; i++)
    {
        if (sBeaconInfo[i].timeStamp_found != 0)
        {
            for (j = 0; j < WM_SIZE_BSSID; j++)
            {
                if (bssid[j] != sBeaconInfo[i].bssid[j])
                {
                    break;
                }
            }
            if (j == WM_SIZE_BSSID)
            {
                if (beaconInfo != NULL)
                {
                    *beaconInfo = &sBeaconInfo[i];
                }
                return TRUE;
            }
        }
        else
        {
            emptyEntry = &sBeaconInfo[i];
        }
    }
    if (beaconInfo != NULL)
    {
        *beaconInfo = emptyEntry;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         InitalizeWMForListeningAndStartScan

  Description:  WM を受信専用モードで初期化し、スキャンを開始する

  Arguments:    None.

  Returns:      初期化に成功したら TRUE
 *---------------------------------------------------------------------------*/
static BOOL InitializeWMForListeningAndStartScan(void)
{
    WMErrCode result;

    SetState( STATE_INITIALIZING );

    result = WM_InitializeForListening(&sWmBuffer, InitializeCb, WIRELESS_DMA_NO,
                                       FALSE /* LED を点滅させない */);
    if (result != WM_ERRCODE_OPERATING)
    {
        OS_TPrintf("WM_Initialize failed: %d\n", result);
        SetState( STATE_FATAL );
        return FALSE;
    }

    return TRUE;
}

static void InitializeCb(void *arg)
{
    // 電源投入後状態です。
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        OS_TPrintf("WM_Initialize failed: %d\n", cb->errcode);
        SetState( STATE_FATAL );
        return;
    }

    SetState( STATE_IDLE );

    (void)StartScan();

    return;
}

static BOOL StartScan(void)
{
    WMErrCode result;
    u16     chanpat;
    static u32 channel = 0;

    chanpat = WM_GetAllowedChannel();

    // 無線が使用可能かどうかをチェックする
    if (chanpat == 0x8000)
    {
        // 0x8000 が返ってきた場合は、無線が初期化されていないなど
        // 無線ライブラリの状態異常を表しているのでエラーにします。
        OS_TPrintf("WM_GetAllowedChannel returns %08x\n", chanpat);
        SetState( STATE_FATAL );
        return FALSE;
    }
    if (chanpat == 0)
    {
        // 無線が使えない状態。
        OS_TPrintf("WM_GetAllowedChannel returns %08x\n", chanpat);
        SetState( STATE_FATAL );
        return FALSE;
    }

    {
        /* 現在の指定から昇順に、可能なチャンネルを検索します */
        while (TRUE)
        {
            channel++;
            if (channel > 16)
            {
                channel = 1;
            }

            if (chanpat & (0x0001 << (channel - 1)))
            {
                break;
            }
        }
        sScanExParam.channelList = (u16)(1 << (channel - 1));
    }

    sScanExParam.maxChannelTime = WM_GetDispersionScanPeriod();
    sScanExParam.scanBuf = (WMbssDesc *)sScanBuf;
    sScanExParam.scanBufSize = WM_SIZE_SCAN_EX_BUF;
    sScanExParam.scanType = WM_SCANTYPE_PASSIVE;
    sScanExParam.ssidLength = 0;
    MI_CpuFill8(sScanExParam.ssid, 0xFF, WM_SIZE_SSID);
    MI_CpuFill8(sScanExParam.bssid, 0xff, WM_SIZE_BSSID);

    SetState( STATE_SCANNING );

    result = WM_StartScanEx(StartScanExCb, &sScanExParam);

    if (result != WM_ERRCODE_OPERATING)
    {
        OS_TPrintf("WM_StartScanEx failed: %d\n", result);
        SetState( STATE_FATAL );
        return FALSE;
    }

    return TRUE;
}

static void StartScanExCb(void *arg)
{
    WMStartScanExCallback *cb = (WMStartScanExCallback *)arg;
    int i;
    WMErrCode result;

    // スキャンコマンドに失敗した場合
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        OS_TPrintf("WM_StartScanEx failed: %d\n", cb->errcode);
        SetState( STATE_FATAL );
        return;
    }

    if (GetState() != STATE_SCANNING)
    {
        // 状態が変更されていればスキャン終了
        SetState( STATE_TERMINATING_SCAN );
        result = WM_EndScan(EndScanCb);
                OS_TPrintf("WM_EndScan returns %d\n", result);
        if (result != WM_ERRCODE_OPERATING)
        {
            OS_TPrintf("WM_EndScan failed: %d\n", result);
            SetState( STATE_FATAL );
        }
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;

    case WM_STATECODE_PARENT_FOUND:
        // 親機が見つかった場合

        OS_TPrintf("found %d beacons\n", cb->bssDescCount);

        if ( cb->bssDescCount > 0 )
        {
            // BssDescの情報がARM7側から書き込まれているため
            // バッファに設定されたBssDescのキャッシュを破棄
            DC_InvalidateRange(&sScanBuf, sizeof(sScanBuf));
        }

        for ( i = 0; i < cb->bssDescCount; i++ )
        {
            WMBssDesc* bd = cb->bssDesc[i];
            BOOL isDS;

            OS_TPrintf(" Ch%2d %02x:%02x:%02x:%02x:%02x:%02x ",
                       bd->channel,
                       bd->bssid[0], bd->bssid[1], bd->bssid[2],
                       bd->bssid[3], bd->bssid[4], bd->bssid[5]);

            // まず、WMBssDesc.gameInfoLength を確認し、
            // ggid に有効な値が入っていることから調べる必要があります。
            if (WM_IsValidGameInfo(&bd->gameInfo, bd->gameInfoLength))
            {
                // DS 親機
                OS_TPrintf("GGID=%08x TGID=%04x\n", bd->gameInfo.ggid, bd->gameInfo.tgid);
                isDS = TRUE;
            }
            else
            {
                // AP
                char str[WM_SIZE_SSID+1];
                u32 ssidLength = bd->ssidLength;
                // ライブラリがフィルタしてくれているかわからない時には、
                // 基本的にはネットワークから来た値は信用しない
                if (ssidLength > WM_SIZE_SSID)
                {
                    ssidLength = WM_SIZE_SSID;
                }
                MI_CpuCopy8(bd->ssid, str, ssidLength);
                str[ssidLength] = 0;
                OS_TPrintf("SSID=%s\n", str);
                isDS = FALSE;
            }

            // 見つけたビーコンの情報を記録
            {
                BOOL found;
                BeaconInfo* beaconInfo;
                // 割り込みコールバック中なので重すぎる処理はしないように注意
                found = FindBeaconInfo(bd->bssid, &beaconInfo);
                if (beaconInfo != NULL)
                {
                    u32 timeStamp = OS_GetVBlankCount();
                    if ( timeStamp == 0 ) { timeStamp = (u32)-1; }
                    if ( !found )
                    {
                        // 新しいエントリ
                        beaconInfo->timeStamp_found = timeStamp;
                    }
                    beaconInfo->timeStamp_updated = timeStamp;
                    beaconInfo->isDS      = isDS;
                    beaconInfo->channel   = bd->channel;
                    MI_CpuCopy8(bd->bssid, beaconInfo->bssid, WM_SIZE_BSSID);
                    MI_CpuCopy8(bd->ssid,  beaconInfo->ssid,  WM_SIZE_SSID);
                }
                else
                {
                    OS_TPrintf("Too many beacons; cannot record beacon info.\n");
                }
            }
        }

        break;
    }

    // チャンネルを変更して再スキャンを開始します。
    (void)StartScan();
}


static void EndScanCb(void* arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        OS_TPrintf("EndScanCb failed: %d\n", cb->errcode);
        SetState( STATE_FATAL );
        return;
    }

    SetState( STATE_IDLE );
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
    DC_FlushRange(sScreen, sizeof(sScreen));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GX_LoadBG0Scr(sScreen, 0, sizeof(sScreen));

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
    MI_CpuClearFast((void *)sScreen, sizeof(sScreen));
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
        sScreen[((y * 32) + x + i) % (32 * 32)] = (u16)((palette << 12) | temp[i]);
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
        temp = sScreen[index];
        temp &= 0x0fff;
        temp |= (palette << 12);
        sScreen[index] = temp;
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
