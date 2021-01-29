/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - stream-3
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : start stream
//  X : start stream (effect)
//  B : stop stream
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nnsys/snd.h>

#include "../data/sound_data.sadl"

#define STREAM_THREAD_PRIO 10

#define SOUND_HEAP_SIZE 0x80000
#define FILTER_SIZE 8

static NNSSndArc arc;
static u8 sndHeap[ SOUND_HEAP_SIZE ];

static NNSSndHeapHandle heap;

static NNSSndStrmHandle strmHandle;

typedef s8 smp_t;
typedef struct EffectCallbackInfo
{
    smp_t sample[ FILTER_SIZE-1 ][2];
} EffectCallbackInfo;

static EffectCallbackInfo callbackInfo;

u16 Cont;
u16 Trg;

void VBlankIntr( void );
void StrmCallback(
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
    
    // Initialize stream handle
    NNS_SndStrmHandleInit( &strmHandle );
    
    // dummy pad read
    Cont = PAD_Read();
    
    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start stream\n");
    OS_Printf(" X : start stream (effect)\n");
    OS_Printf(" B : stop stream\n");
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
            // start stream
            (void)NNS_SndArcStrmStart(
                &strmHandle,   /* stream handle */
                STRM_FANFARE,  /* stream number */
                0              /* offset */
            );
        }
        if ( Trg & PAD_BUTTON_B ) {
            // stop stream
            NNS_SndArcStrmStop( &strmHandle, 0 );
        }
        if ( Trg & PAD_BUTTON_X ) {
            // start stream (effect)
            (void)NNS_SndArcStrmStartEx2(
                &strmHandle,   /* stream handle */
                -1,            /* stream player */
                -1,            /* priority */
                STRM_FANFARE,  /* stream number */
                0,             /* offset */
                StrmCallback,  /* stream callback */
                &callbackInfo, /* callback arg */
                NULL,          /* sound archive stream callback */  
                NULL           /* callback arg */
            );
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

void StrmCallback(
    NNSSndStrmCallbackStatus status,
    int numChannles,
    void* buffer[],
    u32 len,
    NNSSndStrmFormat format,
    void* arg
)
{
#pragma unused( status )    
    EffectCallbackInfo* info = (EffectCallbackInfo*)arg;
    smp_t** buf = (smp_t**)buffer;
    smp_t org[ FILTER_SIZE-1 ][2];
    u32 samples;
    int x;
    int ch;
    long i, j;
    
    samples = ( format == NNS_SND_STRM_FORMAT_PCM8 ) ? len : ( len >> 1 );
    
    // store original sample data
    for( i = 0; i < FILTER_SIZE-1; i++ ) {
        for( ch = 0; ch < numChannles ; ch++ ) {
            org[i][ch] = buf[ch][ i + samples - FILTER_SIZE + 1 ];
        }
    }
    
    // filtering
    for( i = (long)( samples - 1 ); i >= FILTER_SIZE - 1 ; i-- )
    {
        for( ch = 0; ch < numChannles ; ch++ ) {
            x = 0;
            for( j = 0 ; j < FILTER_SIZE ; j++ ) {
                x += buf[ch][ i - j ];
            }
            x /= FILTER_SIZE;
            buf[ch][ i ] = (smp_t)x;
        }
    }
    
#if FILTER_SIZE >= 2    
    for( i = FILTER_SIZE - 2 ; i >= 0 ; i-- )
    {
        for( ch = 0 ; ch < numChannles ; ch++ ) {
            x = buf[ch][ i ];
            for( j = 1 ; j < FILTER_SIZE ; j++ ) {
                x += GetSample( buf[ch], i-j, ch, info );
            }
            x /= FILTER_SIZE;
            buf[ch][ i ] = (smp_t)x;
        }
    }
#endif
    
    // store last samples for next callback
    for( i = 0; i < FILTER_SIZE-1; i++ ) {
        for( ch = 0 ; ch < numChannles ; ch++ ) {
            info->sample[i][ch] = org[i][ch];
        }
    }
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
