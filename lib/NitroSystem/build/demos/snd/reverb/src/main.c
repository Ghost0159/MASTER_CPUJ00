/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - reverb
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.16 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : start BGM
//  B : stop BGM
//  start  : start reverb
//  select : stop reverb
//  up     : up reverb volume
//  down   : down reverb volume
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nnsys/snd.h>

#include "../../data/sound_data.sadl"

#define SOUND_HEAP_SIZE 0x80000
#define REVERB_BUFSIZE 0x4000
#define STREAM_THREAD_PRIO 10

static NNSSndArc arc;
static u8 sndHeap[ SOUND_HEAP_SIZE ];
static u32 sReverbBuffer[ REVERB_BUFSIZE/sizeof(u32) ] ATTRIBUTE_ALIGN( 32 );

static NNSSndHeapHandle heap;

static NNSSndHandle bgmHandle;
static NNSSndHandle seHandle;

u16 Cont;
u16 Trg;

void VBlankIntr( void );

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

    // Initialize sound library
    NNS_SndInit();
    heap = NNS_SndHeapCreate( & sndHeap, sizeof( sndHeap ) );
    NNS_SndArcInit( &arc, "/sound_data.sdat", heap, FALSE );
    (void)NNS_SndArcPlayerSetup( heap );
    NNS_SndArcStrmInit( STREAM_THREAD_PRIO, heap );

    // Load sound data
    (void)NNS_SndArcLoadSeq( SEQ_MARIOKART64_TITLE, heap );
    
    // Initialize sound handles
    NNS_SndHandleInit( &bgmHandle );
    NNS_SndHandleInit( &seHandle );
    
    // dummy pad read
    Cont = PAD_Read();
    
    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start Bgm\n");
    OS_Printf(" B : stop Bgm\n");
    OS_Printf(" start  : start reverb\n");
    OS_Printf(" select : stop reverb\n");
    OS_Printf(" Up     : up reverb volume\n");
    OS_Printf(" Down   : down reverb volume\n");
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
            (void)NNS_SndPlayerStopSeq( &bgmHandle, 1 );
        }
        
        if ( Trg & PAD_BUTTON_START ) {
            // start reverb
            (void)NNS_SndCaptureStartReverb(
                sReverbBuffer, sizeof( sReverbBuffer ),
                NNS_SND_CAPTURE_FORMAT_PCM16, 16000,
                48
            );
        }
        if ( Trg & PAD_BUTTON_SELECT ) {
            // stop reverb
            NNS_SndCaptureStopReverb( 240 );
        }
        
        if ( Trg & PAD_KEY_UP ) {
            // up reverb volume
            NNS_SndCaptureSetReverbVolume( 63, 240 );
        }
        if ( Trg & PAD_KEY_DOWN ) {
            // down reverb volume
            NNS_SndCaptureSetReverbVolume( 16, 240 );
        }        
        
        //---- framework
        NNS_SndMain();
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

