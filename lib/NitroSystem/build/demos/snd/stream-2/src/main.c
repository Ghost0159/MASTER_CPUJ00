/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - stream-2
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : play real-time connected stream data
//     ( intro + A-melo x A_MELO_REPEAT_COUNT )
//  B : stop stream
//  X : play intro stream data
//  Y : play A-melo stream data
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nnsys/snd.h>

#include "data/sound_data.sadl"

#define A_MELO_REPEAT_COUNT 3

#define STREAM_THREAD_PRIO 10

#define SOUND_HEAP_SIZE 0x80000

typedef struct {
    int play_count;
} StrmCallbackInfo;

static NNSSndArc arc;
static u8 sndHeap[ SOUND_HEAP_SIZE ];

static NNSSndHeapHandle heap;

static NNSSndStrmHandle strmHandle;

static BOOL pauseFlag;
static u32 playingPos;

static StrmCallbackInfo strmCallbackInfo;

u16 Cont;
u16 Trg;

void VBlankIntr( void );
BOOL StrmCallback(
    NNSSndArcStrmCallbackStatus status,
    const NNSSndArcStrmCallbackInfo* info,
    NNSSndArcStrmCallbackParam* param,
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
    
    pauseFlag = FALSE;
    
    // dummy pad read
    Cont = PAD_Read();

    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : play real-time connected stream data\n" );
    OS_Printf("    ( intro + A-melo x %d )\n", A_MELO_REPEAT_COUNT );
    OS_Printf(" B : stop stream\n");
    OS_Printf(" X : play intro stream data\n");
    OS_Printf(" Y : play A-melo stream data\n");
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
            // play stream with real-time connected data ( intro + A-melo x A_MELO_REPEAT_COUNT )
            (void)NNS_SndArcStrmStartEx2(
                &strmHandle,       /* stream handle */
                PLAYER_STRM,       /* stream player */
                -1,                /* priority */
                KART_TITLE_INTRO,  /* stream number */
                0,                 /* offset */
                NULL,              /* stream callback */
                NULL,              /* callback arg */
                StrmCallback,      /* sound archive stream callback */  
                &strmCallbackInfo  /* callback arg */
            );
        }
        if ( Trg & PAD_BUTTON_X ) {
            // play intro stream data
            (void)NNS_SndArcStrmStart( &strmHandle, KART_TITLE_INTRO, 0 );
        }
        if ( Trg & PAD_BUTTON_Y ) {
            // play A-melo stream data
            (void)NNS_SndArcStrmStart( &strmHandle, KART_TITLE_A_MELO, 0 );
        }
        if ( Trg & PAD_BUTTON_B ) {
            // stop stream
            NNS_SndArcStrmStop( &strmHandle, 0 );
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

BOOL StrmCallback(
    NNSSndArcStrmCallbackStatus status,
    const NNSSndArcStrmCallbackInfo* info,
    NNSSndArcStrmCallbackParam* param,
    void* arg
)
{
    StrmCallbackInfo* cbinfo = (StrmCallbackInfo*)arg;
    
    if ( status == NNS_SND_ARC_STRM_CALLBACK_DATA_END )
    {
        if ( info->strmNo == KART_TITLE_INTRO ) {
            cbinfo->play_count = 0;
        }
        else{
            cbinfo->play_count++;
            if ( cbinfo->play_count == A_MELO_REPEAT_COUNT ) {
                /* finish */
                return FALSE;
            }
        }
        
        /* continue */
        param->strmNo = KART_TITLE_A_MELO;
        param->offset = 0;
        return TRUE;
    }
    
	return FALSE;
}
