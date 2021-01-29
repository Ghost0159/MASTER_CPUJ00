//
//  libVCT 音声録音、ストリーミング再生プログラム 
//

#include <assert.h>
#include <nitro.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "dbs.h"
#include "screen.h"
#include "utility.h"

#include "sound.h"
#include "demo_util.h"

#pragma profile off

//
//  再生バッファの定義 
//

#define SAMPLING_RATE       VCT_AUDIO_FRAME_RATE    // = 8000Hz
#define SAMPLING_TIME       VCT_AUDIO_FRAME_LENGTH  // = 68ms
#define SAMPLING_BYTE       2                       // = 16bit

#define WAVE_SAMPLE ((int) (SAMPLING_RATE * SAMPLING_TIME * SAMPLING_BYTE) / 1000)

#define MAX_CHANNELS 3

static u8 sPlayBuffer[WAVE_SAMPLE * 2] ATTRIBUTE_ALIGN(32); // for Playback
static u8  sRecBuffer[WAVE_SAMPLE * 2] ATTRIBUTE_ALIGN(32); // for Recording

//  ソフトウェアでミキシングする場合、以下の定義をONにします 
//
#define USE_SOFT_MIXER

//
//  デバッグ用のビープ音再生関連 
//
#define PSG_BUFFER_LENGTH 1088
static u8 psgData[PSG_BUFFER_LENGTH];
static BOOL sPSG = FALSE;

//
//  NitroSystemのサウンドストリームと、サウンドアーカイブストリーム関連 
//
static struct NNSSndStrm sSndStream;
static NNSSndArc sArc;
static NNSSndHandle seHandle;
extern u32 sound_data_sdat[];

static MICAutoParam sMicParam;
static BOOL         sFirstCallback = TRUE;

extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;
extern KeyControl stKeyCnt;

/*---------------------------------------------------------------------------*
   デバッグ用にPSG波形を作成 
 *---------------------------------------------------------------------------*/
static void makePSG(void* buffer, u32 length)
{
	u32 i;
    s16 sample = 10000;

    s16* buf = (s16*)buffer;

    length /= 2;
    for (i = 0; i < length; ++i) {
        buf[i] = sample;
        if (i % 8 != 7) {
            continue;
        }
        sample = (s16)-sample;
    }
}

/*---------------------------------------------------------------------------*
   音声処理コールバック（NitroSystem NNS_SndStrmを利用） 
 *---------------------------------------------------------------------------*/
static void micCallback(MICResult result, void *arg)
{
#pragma unused(result, arg)
}

static void SndCallback(NNSSndStrmCallbackStatus sts,
                        int nChannels,
                        void* buffer[],
                        u32 length,
                        NNSSndStrmFormat format,
                        void* arg)
{
#pragma unused(format)

	OSTick      start;
    const void *micAddr;
    u32         offset;
    u8*         micSrc;
    u32         ch;
    static int  sCurBuffer = 0;

	
    micSrc = (u8*)arg;

    if (sts == NNS_SND_STRM_CALLBACK_SETUP) {
        for (ch = 0; ch < nChannels; ++ch) {
            MI_CpuClear8(buffer[ch], length);
        }
        return;
    }

    //
    // コールバック関数が最初に呼び出されたタイミングでマイクのサンプリングを開始することで、
    // 音声の再生と録音を確実に同期させます。エコーキャンセルを利用する場合は、かならずこの手順で
    // 初期化する必要があります。
    //
    if (sFirstCallback) {
        MIC_StartAutoSamplingAsync(&sMicParam, micCallback, NULL);
        sFirstCallback = FALSE;
    }

    start = OS_GetTick();

    //
    //  現在のマイクの録音位置から、マイクバッファの前半か後半かを判定 
    //
    micAddr = MIC_GetLastSamplingAddress();
    offset  = (u32)((u8*)micAddr - micSrc);

    if ( offset < length ) {
        micSrc = micSrc + length;
        if (sCurBuffer == 0) { OS_TPrintf("mic buffer is same\n"); }
        sCurBuffer = 0;
    }
    else {
        if (sCurBuffer == 1) { OS_TPrintf("mic buffer is same\n"); }
        sCurBuffer = 1;
    }

    //  デバッグ用の矩形波出力 
    //
    if (sPSG) {
        micSrc = psgData;
    }

    //
    // VCT_ReceiveAudioとVCT_SendAudioを利用して音声の送受信を行います。この関数は
    // 常に呼び出してください。関数内部では状態管理によって必要なときのみ実際の送受信処理が
    // 行われます。
    //
    // lengthは 8KHz/16bit 68ms分のサイズ（=1088バイト）のみ受け付けています。
    // それ以外のサイズを渡すとassertion（Release/Romビルド時は戻り値がFALSE）します。
    //
    VCT_ReceiveAudio(buffer[0], length, NULL);
    VCT_SendAudio(micSrc, length);

    // デバッグ表示用
    //
    AddAudioTick(OS_GetTick() - start);

    // キャッシュのフラッシュとクリア
    //
    for (ch = 0; ch < nChannels; ++ch) {
        DC_FlushRange(buffer[ch], length);
    }
    DC_InvalidateRange(micSrc, length);
    return;
}

/*---------------------------------------------------------------------------*
  音声録音・再生まわりの初期化（最初の１回のみ）
 *---------------------------------------------------------------------------*/
static void InitFirst()
{
    OS_TPrintf("Init sound system\n");
    // マイク関連の初期化
    //
    MIC_Init();
    PM_Init();
    (void)PM_SetAmp(PM_AMP_ON);
    PM_SetAmpGain(PM_AMPGAIN_160);

    // マイクの準備が整うまで待つ（プログラミングマニュアル v1.50 P.286）
    OS_Sleep(500);

    // サウンドシステムの初期化
    //
    NNS_SndInit();
    NNS_SndStrmInit(&sSndStream);

    //  サウンドアーカイブの初期化 
    //
    NNS_SndArcInitOnMemory(&sArc, sound_data_sdat);
    NNS_SndArcPlayerSetup(NULL);
    NNS_SndHandleInit(&seHandle);

    //  デバッグ用の矩形波生成 
    //
    makePSG(psgData, PSG_BUFFER_LENGTH);
    
    OS_TPrintf("Init sound system done.\n");
}

/*---------------------------------------------------------------------------*
   音声録音・再生まわりの初期化（VCT_Initの度に呼び出し） 
 *---------------------------------------------------------------------------*/
static void InitSound(NNSSndStrmCallback func, void *data, int sampling_time)
{
    static BOOL noInit = TRUE;
  
    u8 cArray[1] = {7};
    BOOL ret;
    NNSSndStrmFormat nns_format;
    u32 length;

    if (noInit) {
        InitFirst();
        noInit = FALSE;
    }
 
    nns_format = NNS_SND_STRM_FORMAT_PCM16;
    length = (u32)(SAMPLING_RATE * sampling_time * SAMPLING_BYTE) / 1000;
    
    //  マイクパラメータの初期化 
    //
    sMicParam.type   = MIC_SAMPLING_TYPE_SIGNED_12BIT;
    sMicParam.buffer = sRecBuffer;
    sMicParam.size   = length * 2;

    // 定数を使うのではなく、NNS_SND_STRM_TIMER_CLOCKから計算することで
    // 確実にサンプリング周波数を合わせる
    //
    //sMicParam.rate = MIC_SAMPLING_RATE_8K;
    sMicParam.rate = (u32)((NNS_SND_STRM_TIMER_CLOCK / SAMPLING_RATE) * 64);
    
    sMicParam.loop_enable = TRUE;
    sMicParam.full_callback = NULL;
    sMicParam.full_arg = NULL;
    //MIC_StartAutoSampling(&sMicParam);
    sFirstCallback = TRUE;
        
    //  サウンドストリーム再生の初期化 
    //
    NNS_SndStrmAllocChannel(&sSndStream, 1, cArray);
    NNS_SndStrmSetVolume(&sSndStream, 0);
    NNS_SndStrmSetChannelVolume(&sSndStream, 7, 0);
    
    ret = NNS_SndStrmSetup(&sSndStream,
                           nns_format,
                           sPlayBuffer,
                           length * 2,
                           NNS_SND_STRM_TIMER_CLOCK / SAMPLING_RATE,
                           2,
                           func,
                           data);
    SDK_ASSERT(ret);

    return;
}

/*---------------------------------------------------------------------------*
    //  サウンドストリーム再生の初期化 
    //
 *---------------------------------------------------------------------------*/
void TerminateSound()
{
    NNS_SndStrmFreeChannel(&sSndStream);
}

/*---------------------------------------------------------------------------*
   ストリーミング再生開始 
 *---------------------------------------------------------------------------*/
void StartSoundLoop()
{
    NNS_SndStrmStart(&sSndStream);
}

/*---------------------------------------------------------------------------*
   ストリーミング再生停止 
 *---------------------------------------------------------------------------*/
void StopSoundLoop()
{
    (void)MIC_StopAutoSampling();
    NNS_SndStrmStop(&sSndStream);
}

/*---------------------------------------------------------------------------*
   電話の呼び出し音やトランシーバーの音を再生 
 *---------------------------------------------------------------------------*/
void PlaySE(int id)
{
    NNS_SndArcPlayerStartSeqArc( &seHandle, 0, id);
}

void StopSE()
{
    NNS_SndPlayerStopSeq(&seHandle, 0);
}

/*---------------------------------------------------------------------------*
   デバッグ用のビープ音の On/Off 
 *---------------------------------------------------------------------------*/
void SendBeep(BOOL flag)
{
    sPSG = flag;
}

/*---------------------------------------------------------------------------*
   サウンドの初期化処理 
 *---------------------------------------------------------------------------*/
void SetupSound()
{
    InitSound(SndCallback, sRecBuffer, VCT_AUDIO_FRAME_LENGTH);
}
