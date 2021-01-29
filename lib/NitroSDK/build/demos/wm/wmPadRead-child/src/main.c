/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wmPadRead-child
  File:     main.c

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2007/10/29 00:12:20  seiki_masashi
  small fix

  Revision 1.3  2007/10/27 14:54:14  seiki_masashi
  WH を使用するように変更
  マルチブートした際にダウンロード元の親機に接続に行くように修正
  コードの整理

  Revision 1.2  2007/10/27 11:20:04  seiki_masashi
  データシェアリングを使用するように変更

  Revision 1.1  2007/04/18 09:27:37  kitase_hirotake
  Initial Upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//
// Wii の SDK に収録されているデモと通信し、パッド入力を送信するデモです。
//

#include <nitro.h>
#include "wh.h"

#include "data.h"
#include "tpdata.h"

/*---------------------------------------------------------------------------*
    定数定義
 *---------------------------------------------------------------------------*/
#define PICTURE_FRAME_PER_GAME_FRAME    1

/* このデモで使用する GGID */
#define MY_GGID SDK_MAKEGGID_SYSTEM(0x20)

#define DMA_NO  3

#define SAMPLING_BUFFER_SIZE ( 1024 * 1024 ) // 1M

#define FFT_NSHIFT      9
#define FFT_N           (1 << FFT_NSHIFT)

/*---------------------------------------------------------------------------*
    変数定義
 *---------------------------------------------------------------------------*/

static WMBssDesc mbParentBssDesc ATTRIBUTE_ALIGN(32);
static BOOL isMultiBooted;

static GXOamAttr oamBak[128];

typedef struct MyPadData {
    u16 keyData;
    u8 touchPanel_x;
    u8 touchPanel_y;
    u8 mic;
    u8 touch;
    u8 padding[2];
} MyPadData;

// 表示用送受信バッファ
static MyPadData gRecvData[1 + WH_CHILD_MAX]; // 親機が配信する全プレイヤーのタッチ情報
static BOOL gRecvFlag[1 + WH_CHILD_MAX];

u16     keyData;

static GXOamAttr gOam[128];

TPData  raw_point;
TPData  disp_point;
TPCalibrateParam calibrate;

static BOOL input_mic;
static MICAutoParam gMicAutoParam;
static u8 *gMicData;


// FFT 用のバッファ群
static fx16 sinTable[FFT_N - FFT_N / 4];
#ifdef USE_FFTREAL
static fx16 sinTable2[(FFT_N - FFT_N / 4) / 2];
static fx32 data[FFT_N];
#else
static fx32 data[FFT_N * 2];
#endif
static s32 power[FFT_N/2+1];
static s32 smoothedPower[FFT_N/2+1];

/*---------------------------------------------------------------------------*
    関数宣言
 *---------------------------------------------------------------------------*/
static void InitializeAllocateSystem(void);
static void SetPoint16x16(int objNo, u16 pos_x, u16 pos_y, int charNo, int paletteNo);
static void Initialize(void);
static BOOL DoConnect(void);
static void DoPadSharing(void);
static BOOL CheckMicData(void *address);
static void ObjSet(int objNo, int x, int y, int charNo, int paletteNo);
static void VBlankIntr(void);


//================================================================================
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    Initialize();

    // 共有データの初期化
    MI_CpuClear8( gRecvData, sizeof(gRecvData) );

    // 自分がマルチブートから起動した子機であるかどうかをチェックします。
    isMultiBooted = MB_IsMultiBootChild();

    if (isMultiBooted)
    {
        //--------------------------------------------------------------
        // マルチブートで起動した場合、一旦リセットされ通信が切断されます。
        // ブート後もブートした親機のBssDescを保持しているため、この情報を使って
        // 親機へ再接続してください。
        // この時、BssDescからMACアドレスのみを取り出してMACアドレス指定で
        // 親のスキャンから接続を行なう場合は特に問題ありませんが、保持されている
        // BssDescを使って直に親機に接続を行なう場合には、通信サイズや転送モードを
        // あらかじめ親子の間で合わせて設定しておく必要があります。
        //--------------------------------------------------------------

        /* 
         * 親機と再接続するために親機の情報を取得します。                   
         * 接続に利用するWMBssDescは32バイトにアラインされている必要があります。
         * 親機のMACアドレスで再スキャンすることなしに再接続させる場合は、
         * 親機/子機の最大送信サイズ、KS/CSフラグはあらかじめ合わせておいてください。
         * 再スキャンを行なってから接続する場合はこれらの値はすべて0で構いません。
         */
        MB_ReadMultiBootParentBssDesc(&mbParentBssDesc,
                                      WH_PARENT_MAX_SIZE, // 親機最大送信サイズ
                                      WH_CHILD_MAX_SIZE,  // 子機最大送信サイズ
                                      0,   // 予約領域(常に0)
                                      0);  // 予約領域(常に0)
    }

    while (TRUE)
    {
        int connectedFlag = FALSE;

        // 無線初期化
        (void)WH_Initialize();

        connectedFlag = DoConnect();
        // ここまでで子機状態に移行完了

        if (connectedFlag)
        {
            DoPadSharing();
            // 通信モードが抜けるまで戻ってきません
        }

        WH_Finalize();
    }
}

void Initialize(void)
{
    //================ 初期化
    //---- OS 初期化
    OS_Init();

    //---- TP 初期化
    TP_Init();

    // get CalibrateParameter from FlashMemory
    if (!TP_GetUserInfo(&calibrate))
    {
        OS_Panic("FATAL ERROR: can't read UserOwnerInfo\n");
    }
    else
    {
        OS_Printf("Get Calibration Parameter from NVRAM\n");
    }

    TP_SetCalibrateParam(&calibrate);

    //---- GX 初期化
    GX_Init();

    //================ 初期設定
    //---- 電源全てオン
    GX_SetPower(GX_POWER_ALL);

    //----  Vブランク割込許可
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    //---- Vブランク発生設定
    (void)GX_VBlankIntr(TRUE);

    //---- VRAM クリア
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    //---- OAMとパレットクリア
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    //---- バンクＡをＯＢＪへ
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);

    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);

    //---- グラフィックス表示モードにする
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);

    //---- OBJの表示のみON
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);
    
    //---- 32KバイトのOBJで2Dマップモードで使用
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_2D);

    //---- データロード
    GX_LoadOBJ( sampleCharData, 0, sizeof(sampleCharData) );
    GX_LoadOBJPltt( samplePlttData, 0, sizeof(samplePlttData) );

    GXS_SetGraphicsMode(GX_BGMODE_0);    // BGMODE 0

    GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GXS_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    /* Load character bitmap data */
    GXS_LoadOBJ( sampleCharData, 0, sizeof(sampleCharData) );
    GXS_LoadOBJPltt( samplePlttData, 0, sizeof(samplePlttData) );

    //---- 非表示OBJは画面外へ移動
    MI_DmaFill32(DMA_NO, oamBak, 0xc0, sizeof(oamBak));

    //================ その他初期化

    /////////////////////
    // メモリ割当
    InitializeAllocateSystem();

    /////////////////////
    // マイク関係初期化

    //---- MIC 初期化
    MIC_Init();

    //---- PMIC 初期化
    PM_Init();
    // AMPオン
    (void)PM_SetAmp(PM_AMP_ON);
    // AMPゲイン調整
    (void)PM_SetAmpGain(PM_AMPGAIN_80);

    // オートサンプリングの設定
    // OS_Allocで確保したメモリは32バイトALIGNになっているので、
    // キャッシュ操作しても他のメモリを壊さない。
    gMicData = (u8 *)OS_Alloc(SAMPLING_BUFFER_SIZE);
    // フーリエ変換の精度を上げるために 12bit でサンプリングを行う
    gMicAutoParam.type = MIC_SAMPLING_TYPE_12BIT;
    gMicAutoParam.buffer = (void *)gMicData;
    gMicAutoParam.size = SAMPLING_BUFFER_SIZE;
    gMicAutoParam.rate = MIC_SAMPLING_RATE_8K;
    gMicAutoParam.loop_enable = TRUE;
    gMicAutoParam.full_callback = NULL;
    (void)MIC_StartAutoSampling(&gMicAutoParam);

    /////////////////////
    // FFT 関連

    MATH_MakeFFTSinTable(sinTable, FFT_NSHIFT);
#ifdef USE_FFTREAL
    MATH_MakeFFTSinTable(sinTable2, FFT_NSHIFT - 1);
#endif

    //================ 表示開始
    OS_WaitVBlankIntr();
    GX_DispOn();

    GXS_DispOn();
}

BOOL DoConnect(void)
{
    s32 retry = 0;
    enum
    {
        MAX_RETRY = 5
    };

    ObjSet(0, 100, 80, 'C', 4);
    ObjSet(1, 110, 80, 'O', 4);
    ObjSet(2, 120, 80, 'N', 4);
    ObjSet(3, 130, 80, 'N', 4);
    ObjSet(4, 140, 80, 'E', 4);
    ObjSet(5, 150, 80, 'C', 4);
    ObjSet(6, 160, 80, 'T', 4);
    MI_CpuClear8( (GXOamAttr *)&oamBak[7], sizeof(GXOamAttr) * 12);

    //---- Vブランク割込終了待ち
    OS_WaitVBlankIntr();

    // マルチブートの場合は親機が申告していた GGID を利用します。
    //   通常は、MY_GGID と一致しているはずです。
    WH_SetGgid(isMultiBooted ? mbParentBssDesc.gameInfo.ggid : MY_GGID);
    retry = 0;

    while (TRUE)
    {
        // 通信状態により処理を振り分け
        switch (WH_GetSystemState())
        {
        case WH_SYSSTATE_CONNECT_FAIL:
            {
                // WM_StartConnect()に失敗した場合にはWM内部のステートが不正になっている為
                // 一度WM_ResetでIDLEステートにリセットする必要があります。
                WH_Reset();
            }
            break;
        case WH_SYSSTATE_IDLE:
            {
                if (retry < MAX_RETRY)
                {
                    // 親機をスキャンして接続します。
                    //   マルチブートされていた場合は、覚えていた親機の BSSID を利用して
                    //   マルチブート元の親機だけに繋ぎに行くようにします。
                    (void)WH_ChildConnectAuto(WH_CONNECTMODE_DS_CHILD, isMultiBooted ? mbParentBssDesc.bssid : NULL, 0);
                    retry++;
                    break;
                }
                // MAX_RETRY で親機に接続できなければERROR表示
            }
        case WH_SYSSTATE_ERROR:
            return FALSE;

        case WH_SYSSTATE_DATASHARING:
            return TRUE;

        case WH_SYSSTATE_BUSY:
        case WH_SYSSTATE_SCANNING:
        case WH_SYSSTATE_CONNECTED:
        default:
            break;
        }

        OS_WaitVBlankIntr();
    }

    // can't reach here
    return FALSE;
}

void DoPadSharing(void)
{
    int myAid;
    myAid = WM_GetAID();

    //================ メインループ
    while (TRUE)
    {
        // Draw Marker by calibrated point.
        while (TP_RequestRawSampling(&raw_point) != 0)
        {
        };
        TP_GetCalibratedPoint(&disp_point, &raw_point);

        if (disp_point.touch)
        {
            SetPoint16x16(0, disp_point.x, disp_point.y, myAid + '0', 1);

            switch (disp_point.validity)
            {
            case TP_VALIDITY_VALID:
                OS_TPrintf("( %d, %d ) -> ( %d, %d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            case TP_VALIDITY_INVALID_X:
                OS_TPrintf("( *%d, %d ) -> ( *%d, %d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            case TP_VALIDITY_INVALID_Y:
                OS_TPrintf("( %d, *%d ) -> ( %d, *%d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            case TP_VALIDITY_INVALID_XY:
                OS_TPrintf("( *%d, *%d ) -> ( *%d, *%d )\n", raw_point.x, raw_point.y, disp_point.x,
                           disp_point.y);
                break;
            }
        }
        //---- Vブランク割込終了待ち
        OS_WaitVBlankIntr();

        //---- パッドデータ読み込み
        keyData = PAD_Read();

        {
            static MyPadData sendData ATTRIBUTE_ALIGN(32);
            int i;

            // 送信データを準備
            MI_CpuClear8(&sendData, sizeof(sendData));
            sendData.keyData = (u16)(keyData | (PAD_DetectFold() ? PAD_DETECT_FOLD_MASK : 0));
            sendData.touch        = (u8)disp_point.touch;
            sendData.touchPanel_x = (u8)disp_point.x;
            sendData.touchPanel_y = (u8)disp_point.y;
            sendData.mic = (u8)input_mic;

            if ( WH_StepDS((void*)&sendData) )
            {
                for (i = 0; i < (1 + WH_CHILD_MAX); i++)
                {
                    u8* data;

                    data = (u8 *)WH_GetSharedDataAdr((u16)i);

                    if (data != NULL)
                    {
                        gRecvFlag[i] = TRUE;
                        MI_CpuCopy8(data, &gRecvData[i], sizeof(gRecvData[0]));
                    }
                    else
                    {
                        gRecvFlag[i] = FALSE;
                    }
                }
            }
        }

        //---- パッド情報を OBJ で表示
        ObjSet(0, 200, 90, 'A', (keyData & PAD_BUTTON_A) ? 1 : 2);
        ObjSet(1, 180, 95, 'B', (keyData & PAD_BUTTON_B) ? 1 : 2);

        ObjSet(2, 60, 50, 'L', (keyData & PAD_BUTTON_L) ? 1 : 2);
        ObjSet(3, 180, 50, 'R', (keyData & PAD_BUTTON_R) ? 1 : 2);

        ObjSet(4, 60, 80, 'U', (keyData & PAD_KEY_UP) ? 1 : 2);
        ObjSet(5, 60, 100, 'D', (keyData & PAD_KEY_DOWN) ? 1 : 2);
        ObjSet(6, 50, 90, 'L', (keyData & PAD_KEY_LEFT) ? 1 : 2);
        ObjSet(7, 70, 90, 'R', (keyData & PAD_KEY_RIGHT) ? 1 : 2);

        ObjSet(8, 130, 95, 'S', (keyData & PAD_BUTTON_START) ? 1 : 2);
        ObjSet(9, 110, 95, 'S', (keyData & PAD_BUTTON_SELECT) ? 1 : 2);

        ObjSet(10, 200, 75, 'X', (keyData & PAD_BUTTON_X) ? 1 : 2);
        ObjSet(11, 180, 80, 'Y', (keyData & PAD_BUTTON_Y) ? 1 : 2);

        //---- 折り畳み検知状態を OBJ で表示
        ObjSet(12, 120, 30, 'F', (PAD_DetectFold())? 1 : 2);

        ObjSet(13, 100, 5, 'A', 4);
        ObjSet(14, 110, 5, 'I', 4);
        ObjSet(15, 120, 5, 'D', 4);
        ObjSet(16, 130, 5, (myAid / 10) ? (myAid / 10) + '0' : ' ', 4);
        ObjSet(17, 140, 5, (myAid % 10) + '0', 4);

        // マイク入力チェック
        input_mic = CheckMicData(MIC_GetLastSamplingAddress());
        ObjSet(18, 120, 120, 'M', input_mic ? 1 : 2);

        if( WH_GetSystemState() != WH_SYSSTATE_DATASHARING )
        {
            break;
        }

        // 受信情報から全プレイヤーのタッチ情報を描画
        {
            int i;

            for(i = 0; i < WH_CHILD_MAX + 1; i++)
            {
                if( i != myAid && gRecvData[i].touch )
                {
                    SetPoint16x16(i+1, gRecvData[i].touchPanel_x, gRecvData[i].touchPanel_y, '0'+i, 2);
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  OBJ セット
//
void ObjSet(int objNo, int x, int y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&oamBak[objNo],
                  x,
                  y,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}


//--------------------------------------------------------------------------------
//    Ｖブランク割り込み処理
//
void VBlankIntr(void)
{
    //---- ＯＡＭの更新
    DC_FlushRange(oamBak, sizeof(oamBak));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    MI_DmaCopy32(DMA_NO, oamBak, (void *)HW_OAM, sizeof(oamBak));

    /* flush cache of OAM buffers to main memory */
    DC_FlushRange(gOam, sizeof(gOam));
    /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
    // DC_WaitWriteBufferEmpty();
    GXS_LoadOAM(gOam, 0, sizeof(gOam));
    MI_DmaFill32(3, gOam, 192, sizeof(gOam));       // Clear OAM buffer

    //---- 割り込みチェックフラグ
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
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
  Name:         SetPoint16x16

  Description:  Display a 16x16 OBJ on indicated point.

  Arguments:    x - position X.
                y - position Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SetPoint16x16(int objNo, u16 pos_x, u16 pos_y, int charNo, int paletteNo)
{
    G2_SetOBJAttr((GXOamAttr *)&gOam[objNo],
                  pos_x - 8,
                  pos_y - 8,
                  0,
                  GX_OAM_MODE_NORMAL,
                  FALSE,
                  GX_OAM_EFFECT_NONE, GX_OAM_SHAPE_8x8, GX_OAM_COLOR_16, charNo, paletteNo, 0);
}

/*---------------------------------------------------------------------------*
  Name:         CheckMicData

  Arguments:    address - mic sampling data.

  Returns:      Whether mic input exists.
 *---------------------------------------------------------------------------*/
static BOOL CheckMicData(void *address)
{
    s32     i;

    u16    *p;

    // サンプリングが一回も行われていない場合は何もせずに終了
    // (マイクと無関係のメモリのキャッシュを破棄してしまう為)
    if ((address < gMicData) || (address >= (gMicData + SAMPLING_BUFFER_SIZE)))
    {
        return FALSE;
    }

    // 直近の FFT_N 個のサンプリング値に対して FFT を適用
    // 12bit サンプリングは各値が2バイトとなる
    p = (u16 *)((u32)address - (FFT_N-1)*2);
    if ((u32)p < (u32)gMicData)
    {
        p = (u16 *)((u32)p + SAMPLING_BUFFER_SIZE);
    }
    DC_InvalidateRange((void *)((u32)p & 0xffffffe0), 32);
    for (i = 0; i < FFT_N; i++)
    {
#ifdef USE_FFTREAL
        data[i] = ((*p) << (FX32_SHIFT - (16 - 12)));
#else
        data[i * 2] = ((*p) << (FX32_SHIFT - (16 - 12)));
        data[i * 2 + 1] = 0; // 虚部には 0 を代入
#endif
        p++;
        if ((u32)p >= (u32)(gMicData + SAMPLING_BUFFER_SIZE))
        {
            p = (u16 *)((u32)p - SAMPLING_BUFFER_SIZE);
        }
        if (((u32)p % 32) == 0)
        {
            DC_InvalidateRange(p, 32);
        }
    }

#ifdef USE_FFTREAL
    MATH_FFTReal(data, FFT_NSHIFT, sinTable, sinTable2);
#else
    MATH_FFT(data, FFT_NSHIFT, sinTable);
#endif

    // FFT_N/2 より上はそれ以下を共役して反転させた値しか得られないので計算しない
    for (i = 0; i <= FFT_N/2; i++)
    {
        fx32 real;
        fx32 imm;

#ifdef USE_FFTREAL
        if (0 < i  && i < FFT_N/2)
        {
            real = data[i * 2];
            imm  = data[i * 2 + 1];
        }
        else
        {
            if (i == 0)
            {
                real = data[0];
            }
            else // i == FFT_N/2
            {
                real = data[1];
            }
            imm  = 0;
        }
#else
        real = data[i * 2];
        imm  = data[i * 2 + 1];
#endif

        // 各周波数のパワーを計算する
        // 値の大小のみが必要な場合は Sqrt を省略することで高速化が可能 
        power[i] = FX_Whole(FX_Sqrt(FX_MUL(real, real) + FX_MUL(imm, imm)));

        // 各周波数のパワーの時間累積値を記録する
        // 少しずつ減衰させていく
        smoothedPower[i] += power[i] - (smoothedPower[i]/8);
    }

    // マイク入力をチェック
    {
        s32 totalPower = 0;

#define FILTER_LOW 12    // ((8000Hz/2)/(FFT_N/2)) * 12 = 187.5Hz
#define FILTER_HIGH 64   // ((8000Hz/2)/(FFT_N/2)) * 64 = 1000Hz
        for (i = FILTER_LOW; i < FILTER_HIGH; i++)
        {
            totalPower += smoothedPower[i];
        }
        //OS_TPrintf("totalPower = %d, ave = %d\n", totalPower, totalPower/(FILTER_HIGH-FILTER_LOW));
        // 一定量以上の入力値があるかチェック
        if (totalPower > (FILTER_HIGH-FILTER_LOW)*64)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*====== End of main.c ======*/
