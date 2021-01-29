/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - stream
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
//  B : stop stream
//  L + B : fade-out stream
//  start : pause stream
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nnsys/snd.h>

#include "../data/sound_data.sadl"

#define STREAM_THREAD_PRIO 10

#define SOUND_HEAP_SIZE 0x80000

static NNSSndArc arc;
static u8 sndHeap[ SOUND_HEAP_SIZE ];

static NNSSndHeapHandle heap;

static NNSSndStrmHandle strmHandle;

static BOOL pauseFlag;
static u32 playingPos;

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
    
    // Initialize sound
    NNS_SndInit();
    heap = NNS_SndHeapCreate( & sndHeap, sizeof( sndHeap ) );
    NNS_SndArcInit( &arc, "/sound_data.sdat", heap, FALSE );
    (void)NNS_SndArcPlayerSetup( heap );
    NNS_SndArcStrmInit( STREAM_THREAD_PRIO, heap );
    
    // Initialize stream handle
    NNS_SndStrmHandleInit( &strmHandle );
    
    pauseFlag = FALSE;
    
    // dummy pad read
    Cont = PAD_Read();
    
    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start stream\n");
    OS_Printf(" B : stop stream\n");
    OS_Printf(" L + B : fade-out stream\n");
    OS_Printf(" start  : pause stream\n");
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
            (void)NNS_SndArcStrmStart( &strmHandle, STRM_MARIOKART, 0 );
        }
        if ( Trg & PAD_BUTTON_B ) {
            // stop stream
            NNS_SndArcStrmStop( &strmHandle, ( Cont & PAD_BUTTON_L ) ? 120 : 0 );
        }

        if ( Trg & PAD_BUTTON_START ) {
            // pause stream
            if ( pauseFlag ) {
                (void)NNS_SndArcStrmStart( &strmHandle, STRM_MARIOKART, playingPos );
                pauseFlag = FALSE;
            }
            else {
                playingPos = NNS_SndArcStrmGetCurrentPlayingPos( &strmHandle );
                NNS_SndArcStrmStop( &strmHandle, 4 );
                pauseFlag = TRUE;
            }
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

