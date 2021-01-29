/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - effect
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.12 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : start Bgm
//  B : stop Bgm
//  Up   : start one shot SE No.0
//  start  : effect start
//  select : effect end
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nnsys/snd.h>

#include "../data/sound_data.sadl"

#define CAPTURE_THREAD_PRIO 3
#define STREAM_THREAD_PRIO 10

#define CAPTURE_BUFSIZE 0x1000
#define SAMPLING_RATE   32000
#define CAPTURE_FORMAT  NNS_SND_CAPTURE_FORMAT_PCM16
typedef s16 smp_t;

#define FILTER_SIZE 8

#define SOUND_HEAP_SIZE 0x80000

typedef struct EffectCallbackInfo
{
    smp_t sample[ FILTER_SIZE-1 ][2];
} EffectCallbackInfo;

static u8 sCaptureBuffer[ CAPTURE_BUFSIZE ] ATTRIBUTE_ALIGN(32);

static NNSSndArc arc;
static u8 sndHeap[ SOUND_HEAP_SIZE ];

static NNSSndHeapHandle heap;

static NNSSndHandle bgmHandle;
static NNSSndHandle seHandle;

static EffectCallbackInfo callbackInfo;

u16 Cont;
u16 Trg;

void VBlankIntr( void );
void EffectCallback( void* lp, void* rp, u32 len, NNSSndCaptureFormat format, void* arg );

//----------------------------------------------------------------
//   NitroMain
//
void NitroMain()
{
    OS_Init();
    GX_Init();
    
    // VBlank settings
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr(TRUE);
    
    FS_Init( MI_DMA_MAX_NUM );
    
    // Initialize sound
    NNS_SndInit();
    heap = NNS_SndHeapCreate( & sndHeap, sizeof( sndHeap ) );
    NNS_SndArcInit( &arc, "/sound_data.sdat", heap, FALSE );
    (void)NNS_SndArcPlayerSetup( heap );
    NNS_SndArcStrmInit( STREAM_THREAD_PRIO, heap );
    NNS_SndCaptureCreateThread( CAPTURE_THREAD_PRIO );
    
    // Load sound data
    (void)NNS_SndArcLoadSeq( SEQ_MARIOKART64_TITLE, heap );
    (void)NNS_SndArcLoadSeqArc( SEQ_SE, heap );
    (void)NNS_SndArcLoadBank( BANK_SE, heap );
    
    // Initialize sound handle
    NNS_SndHandleInit( &bgmHandle );
    NNS_SndHandleInit( &seHandle );
    
    // dummy pad read
    Cont = PAD_Read();
    
    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start Bgm\n");
    OS_Printf(" B : stop Bgm\n");
    OS_Printf(" Up : start SE\n");
    OS_Printf(" start  : start effect\n");
    OS_Printf(" select : stop effect\n");
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
            // start BGM
            (void)NNS_SndArcPlayerStartSeq( &bgmHandle, SEQ_MARIOKART64_TITLE );
        }
        if ( Trg & PAD_BUTTON_B ) {
            // stop BGM
            (void)NNS_SndPlayerStopSeq( &bgmHandle, 0 );
        }
        
        if ( Trg & PAD_KEY_UP ) {
            // start SE
            (void)NNS_SndArcPlayerStartSeqArc( &seHandle, SEQ_SE, SE_COIN );
        }
        
        if ( Trg & PAD_BUTTON_START ) {
            // start effect
            MI_CpuClear8( & callbackInfo, sizeof( callbackInfo ) );
            
            (void)NNS_SndCaptureStartEffect(
                sCaptureBuffer,
                sizeof( sCaptureBuffer ),
                CAPTURE_FORMAT,
                SAMPLING_RATE,
                2,
                EffectCallback,
                &callbackInfo
            );
        }
        
        if ( Trg & PAD_BUTTON_SELECT ) {
            // stop effect
            NNS_SndCaptureStopEffect();
        }
        
        //---- framework
        NNS_SndMain();
    }
}

static inline smp_t GetSample( smp_t* p, int x, int n, const EffectCallbackInfo* info )
{
    if ( x >= 0 ) return p[x];
    x += FILTER_SIZE-1;
    return info->sample[x][n];
}

void EffectCallback( void* bufferL_p, void* bufferR_p, u32 len, NNSSndCaptureFormat format, void* arg )
{
    smp_t* lp = (smp_t*)bufferL_p;
    smp_t* rp = (smp_t*)bufferR_p;
    EffectCallbackInfo* info = (EffectCallbackInfo*)arg;
    smp_t org[ FILTER_SIZE-1 ][2];
    u32 samples;
    int x;
    long i, j;
    
    samples = ( format == NNS_SND_CAPTURE_FORMAT_PCM8 ) ? len : ( len >> 1 );
    
    // store original sample data
    for( i = 0; i < FILTER_SIZE-1; i++ ) {
        org[i][0] = lp[ i + samples - FILTER_SIZE + 1 ];
        org[i][1] = rp[ i + samples - FILTER_SIZE + 1 ];
    }
    
    // filtering
    for( i = (long)( samples - 1 ); i >= FILTER_SIZE - 1 ; i-- )
    {
        x = 0;
        for( j = 0 ; j < FILTER_SIZE ; j++ ) {
            x += lp[ i - j ];
        }
        x /= FILTER_SIZE;
        lp[ i ] = (smp_t)x;
        
        x = 0;
        for( j = 0 ; j < FILTER_SIZE ; j++ ) {
            x += rp[ i - j ];
        }
        x /= FILTER_SIZE;
        rp[ i ] = (smp_t)x;
    }
    
#if FILTER_SIZE >= 2    
    for( i = FILTER_SIZE - 2 ; i >= 0 ; i-- )
    {
        x = lp[ i ];
        for( j = 1 ; j < FILTER_SIZE ; j++ ) {
            x += GetSample( lp, i-j, 0, info );
        }
        x /= FILTER_SIZE;
        lp[ i ] = (smp_t)x;
        
        x = rp[ i ];
        for( j = 1 ; j < FILTER_SIZE ; j++ ) {
            x += GetSample( rp, i-j, 1, info );
        }
        x /= FILTER_SIZE;
        rp[ i ] = (smp_t)x;
    }
#endif
    
    // store last samples for next callback
    for( i = 0; i < FILTER_SIZE-1; i++ ) {
        info->sample[i][0] = org[i][0];
        info->sample[i][1] = org[i][1];
    }

    DC_FlushRange( bufferL_p, len );
    DC_FlushRange( bufferR_p, len );
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}

