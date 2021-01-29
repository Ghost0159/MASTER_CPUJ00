/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - micThrough
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : start mic through
//  B : stop mic through
//  R hold : voice changer
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nnsys/snd.h>

//-----------------------------------------------------------------------------
// サンプルフォーマット
//   この定義を外すと、8ビットになる
#define FORMAT_PCM16

//-----------------------------------------------------------------------------
// サンプリングレート
#define SAMPLING_RATE   8000

//-----------------------------------------------------------------------------
// サンプルバッファ一周の時間（再生遅延時間に相当）
#define SAMPLING_TIME     32 /* msec */

//-----------------------------------------------------------------------------
// 再生チャンネル番号
#define CHANNEL_NO 7


#define WAVE_SAMPLE ( (int)( SAMPLING_RATE * SAMPLING_TIME / 1000 ) )

#ifdef FORMAT_PCM16
#define MIC_FORMAT MIC_SAMPLING_TYPE_SIGNED_12BIT
#define STRM_FORMAT NNS_SND_STRM_FORMAT_PCM16
typedef s16 smp_t;
#else
#define MIC_FORMAT MIC_SAMPLING_TYPE_SIGNED_8BIT
#define STRM_FORMAT NNS_SND_STRM_FORMAT_PCM8
typedef s8 smp_t;
#endif

typedef struct StrmInfo {
    int curBuffer;
    volatile BOOL voiceChanger;
} StrmInfo;

static smp_t sRecBuffer[ WAVE_SAMPLE ] ATTRIBUTE_ALIGN(32);
static smp_t sStrmBuffer[ WAVE_SAMPLE ] ATTRIBUTE_ALIGN(32);
static MICAutoParam sMicParam;
static NNSSndStrm sSndStrm;
static StrmInfo sStrmInfo;

static u16 Cont;
static u16 Trg;

static void VBlankIntr( void );
static void StrmCallback(
      NNSSndStrmCallbackStatus status,
      int numChannles,
      void* buffer[],
      u32 len,
      NNSSndStrmFormat format,
      void* arg
);
    
//----------------------------------------------------------------
//   NitroMain
//
void NitroMain()
{
    const int timer = NNS_SND_STRM_TIMER_CLOCK / SAMPLING_RATE;
    
    OS_Init();
    GX_Init();

    // VBlank settings
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);
    
    MIC_Init();
    
#ifdef  SDK_TS
    // PMIC初期化
    PM_Init();
    // AMPオン
    (void)PM_SetAmp( PM_AMP_ON );
#if ( SDK_TS_VERSION >= 100 )
    // AMPゲイン調整
    (void)PM_SetAmpGain( PM_AMPGAIN_80 );
#endif
#if ( SDK_TS_VERSION == 100 )
    // ノイズ対策の為、LCDバックライトオフ
    (void)PM_SetBackLight( PM_LCD_ALL, PM_BACKLIGHT_OFF );
#endif
#endif
    
    // Initialize sound
    NNS_SndInit();
    
    // Initialize waveout
	{
        const u8 chNoList[] = { CHANNEL_NO };
        
        NNS_SndStrmInit( &sSndStrm );
        if ( ! NNS_SndStrmAllocChannel( &sSndStrm, sizeof(chNoList)/sizeof(chNoList[0]), chNoList ) ) {
            OS_Panic("NNS_SndStrmAllocChannel is Failed\n");
        }
    }
    
    // Initialize mic parameter
    sMicParam.type = MIC_FORMAT;
    sMicParam.buffer = sRecBuffer;
    sMicParam.size = sizeof( sRecBuffer );
    sMicParam.rate = timer * 64;
    sMicParam.loop_enable = TRUE;
    sMicParam.full_callback = NULL;
    sMicParam.full_arg = NULL;
    
    // dummy pad read
    Cont = PAD_Read();
    
    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start mic through\n");
    OS_Printf(" B : stop mic through\n");
    OS_Printf(" R hold : voice changer\n");
    OS_Printf("=================================\n");
    
    //================ Main Loop
    while(1)
    {
        u16 ReadData;
        
        SVC_WaitVBlankIntr();
        
        ReadData = PAD_Read();
        Trg  = (u16)(ReadData & (ReadData ^ Cont));
        Cont = ReadData;
        
        if ( Trg & PAD_BUTTON_A ) {
            // Start mic and stream
            sStrmInfo.curBuffer = 0;
            sStrmInfo.voiceChanger = FALSE;
            if ( ! NNS_SndStrmSetup(
                     &sSndStrm,
                     STRM_FORMAT,
                     sStrmBuffer,
                     sizeof( sStrmBuffer ),
                     timer,
                     2,
                     StrmCallback,
                     & sStrmInfo
                 ) ) {
                OS_Panic("NNS_SndStrmSetup is Failed\n");
            }
            (void)MIC_StartAutoSampling( & sMicParam );
            NNS_SndStrmStart( &sSndStrm );
        }
        
        if ( Trg & PAD_BUTTON_B ) {
            // Stop mic and stream
            NNS_SndStrmStop( &sSndStrm );
            (void)MIC_StopAutoSampling();
        }
        
        if ( Cont & PAD_BUTTON_R ) {
            sStrmInfo.voiceChanger = TRUE;
        }
        else {
            sStrmInfo.voiceChanger = FALSE;
        }
        
        //---- framework
        NNS_SndMain();
    }
}

static void StrmCallback(
      NNSSndStrmCallbackStatus status,
      int numChannles,
      void* buffer[],
      u32 len,
      NNSSndStrmFormat format,
      void* arg
)
{
    StrmInfo* info = (StrmInfo*)arg;
    const u32 samples = len / sizeof(smp_t);
    
    NNS_ASSERT( numChannles == 1 );
    NNS_ASSERT( format == STRM_FORMAT );
#ifndef NNS_DEBUG
#pragma unused( numChannles, format )
#endif
    
    if ( status == NNS_SND_STRM_CALLBACK_SETUP ) {
        MI_CpuClear8( buffer[0], len );
    }
    else {
        const void* const curAddr = MIC_GetLastSamplingAddress();
        
        if ( curAddr == NULL ) {
            MI_CpuClear8( buffer[0], len );
        }
        else {
            const u32 ofs = (u32)( (smp_t*)curAddr - sRecBuffer );
            smp_t* src;
            smp_t* dest = (smp_t*)buffer[0];
            
            if ( ofs < WAVE_SAMPLE/2 ) {
                src = &sRecBuffer[ WAVE_SAMPLE / 2 ];
                 if ( info->curBuffer == 0 ) {
                    OS_Printf("loss samples\n");
                }
                 info->curBuffer = 0;
            }
            else {
                src = &sRecBuffer[ 0 ];
                 if ( info->curBuffer == 1 ) {
                    OS_Printf("loss samples\n");
                }
                info->curBuffer = 1;
            }
            
            DC_InvalidateRange( src, len );
            if ( ! info->voiceChanger ) {
                MI_CpuCopy8( src, dest, len );
            }
            else {
                int i;
                for( i = 0 ; i < samples/2 ; i++ ) {
                    dest[ i ] = src[ i * 2 ];
                }
                MI_CpuCopy8( &dest[0], &dest[samples/2], len/2 );
            }
        }
    }
    
    DC_FlushRange( buffer[0], len );
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}

