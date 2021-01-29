//
//  libVCT �����^���A�X�g���[�~���O�Đ��v���O���� 
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
//  �Đ��o�b�t�@�̒�` 
//

#define SAMPLING_RATE       VCT_AUDIO_FRAME_RATE    // = 8000Hz
#define SAMPLING_TIME       VCT_AUDIO_FRAME_LENGTH  // = 68ms
#define SAMPLING_BYTE       2                       // = 16bit

#define WAVE_SAMPLE ((int) (SAMPLING_RATE * SAMPLING_TIME * SAMPLING_BYTE) / 1000)

#define MAX_CHANNELS 3

static u8 sPlayBuffer[WAVE_SAMPLE * 2] ATTRIBUTE_ALIGN(32); // for Playback
static u8  sRecBuffer[WAVE_SAMPLE * 2] ATTRIBUTE_ALIGN(32); // for Recording

//  �\�t�g�E�F�A�Ń~�L�V���O����ꍇ�A�ȉ��̒�`��ON�ɂ��܂� 
//
#define USE_SOFT_MIXER

//
//  �f�o�b�O�p�̃r�[�v���Đ��֘A 
//
#define PSG_BUFFER_LENGTH 1088
static u8 psgData[PSG_BUFFER_LENGTH];
static BOOL sPSG = FALSE;

//
//  NitroSystem�̃T�E���h�X�g���[���ƁA�T�E���h�A�[�J�C�u�X�g���[���֘A 
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
   �f�o�b�O�p��PSG�g�`���쐬 
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
   ���������R�[���o�b�N�iNitroSystem NNS_SndStrm�𗘗p�j 
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
    // �R�[���o�b�N�֐����ŏ��ɌĂяo���ꂽ�^�C�~���O�Ń}�C�N�̃T���v�����O���J�n���邱�ƂŁA
    // �����̍Đ��Ƙ^�����m���ɓ��������܂��B�G�R�[�L�����Z���𗘗p����ꍇ�́A���Ȃ炸���̎菇��
    // ����������K�v������܂��B
    //
    if (sFirstCallback) {
        MIC_StartAutoSamplingAsync(&sMicParam, micCallback, NULL);
        sFirstCallback = FALSE;
    }

    start = OS_GetTick();

    //
    //  ���݂̃}�C�N�̘^���ʒu����A�}�C�N�o�b�t�@�̑O�����㔼���𔻒� 
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

    //  �f�o�b�O�p�̋�`�g�o�� 
    //
    if (sPSG) {
        micSrc = psgData;
    }

    //
    // VCT_ReceiveAudio��VCT_SendAudio�𗘗p���ĉ����̑���M���s���܂��B���̊֐���
    // ��ɌĂяo���Ă��������B�֐������ł͏�ԊǗ��ɂ���ĕK�v�ȂƂ��̂ݎ��ۂ̑���M������
    // �s���܂��B
    //
    // length�� 8KHz/16bit 68ms���̃T�C�Y�i=1088�o�C�g�j�̂ݎ󂯕t���Ă��܂��B
    // ����ȊO�̃T�C�Y��n����assertion�iRelease/Rom�r���h���͖߂�l��FALSE�j���܂��B
    //
    VCT_ReceiveAudio(buffer[0], length, NULL);
    VCT_SendAudio(micSrc, length);

    // �f�o�b�O�\���p
    //
    AddAudioTick(OS_GetTick() - start);

    // �L���b�V���̃t���b�V���ƃN���A
    //
    for (ch = 0; ch < nChannels; ++ch) {
        DC_FlushRange(buffer[ch], length);
    }
    DC_InvalidateRange(micSrc, length);
    return;
}

/*---------------------------------------------------------------------------*
  �����^���E�Đ��܂��̏������i�ŏ��̂P��̂݁j
 *---------------------------------------------------------------------------*/
static void InitFirst()
{
    OS_TPrintf("Init sound system\n");
    // �}�C�N�֘A�̏�����
    //
    MIC_Init();
    PM_Init();
    (void)PM_SetAmp(PM_AMP_ON);
    PM_SetAmpGain(PM_AMPGAIN_160);

    // �}�C�N�̏����������܂ő҂i�v���O���~���O�}�j���A�� v1.50 P.286�j
    OS_Sleep(500);

    // �T�E���h�V�X�e���̏�����
    //
    NNS_SndInit();
    NNS_SndStrmInit(&sSndStream);

    //  �T�E���h�A�[�J�C�u�̏����� 
    //
    NNS_SndArcInitOnMemory(&sArc, sound_data_sdat);
    NNS_SndArcPlayerSetup(NULL);
    NNS_SndHandleInit(&seHandle);

    //  �f�o�b�O�p�̋�`�g���� 
    //
    makePSG(psgData, PSG_BUFFER_LENGTH);
    
    OS_TPrintf("Init sound system done.\n");
}

/*---------------------------------------------------------------------------*
   �����^���E�Đ��܂��̏������iVCT_Init�̓x�ɌĂяo���j 
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
    
    //  �}�C�N�p�����[�^�̏����� 
    //
    sMicParam.type   = MIC_SAMPLING_TYPE_SIGNED_12BIT;
    sMicParam.buffer = sRecBuffer;
    sMicParam.size   = length * 2;

    // �萔���g���̂ł͂Ȃ��ANNS_SND_STRM_TIMER_CLOCK����v�Z���邱�Ƃ�
    // �m���ɃT���v�����O���g�������킹��
    //
    //sMicParam.rate = MIC_SAMPLING_RATE_8K;
    sMicParam.rate = (u32)((NNS_SND_STRM_TIMER_CLOCK / SAMPLING_RATE) * 64);
    
    sMicParam.loop_enable = TRUE;
    sMicParam.full_callback = NULL;
    sMicParam.full_arg = NULL;
    //MIC_StartAutoSampling(&sMicParam);
    sFirstCallback = TRUE;
        
    //  �T�E���h�X�g���[���Đ��̏����� 
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
    //  �T�E���h�X�g���[���Đ��̏����� 
    //
 *---------------------------------------------------------------------------*/
void TerminateSound()
{
    NNS_SndStrmFreeChannel(&sSndStream);
}

/*---------------------------------------------------------------------------*
   �X�g���[�~���O�Đ��J�n 
 *---------------------------------------------------------------------------*/
void StartSoundLoop()
{
    NNS_SndStrmStart(&sSndStream);
}

/*---------------------------------------------------------------------------*
   �X�g���[�~���O�Đ���~ 
 *---------------------------------------------------------------------------*/
void StopSoundLoop()
{
    (void)MIC_StopAutoSampling();
    NNS_SndStrmStop(&sSndStream);
}

/*---------------------------------------------------------------------------*
   �d�b�̌Ăяo������g�����V�[�o�[�̉����Đ� 
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
   �f�o�b�O�p�̃r�[�v���� On/Off 
 *---------------------------------------------------------------------------*/
void SendBeep(BOOL flag)
{
    sPSG = flag;
}

/*---------------------------------------------------------------------------*
   �T�E���h�̏��������� 
 *---------------------------------------------------------------------------*/
void SetupSound()
{
    InitSound(SndCallback, sRecBuffer, VCT_AUDIO_FRAME_LENGTH);
}
