/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - snd - driverInfo
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// USAGE:
//  A : start Bgm
//  B : stop Bgm
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nitro/fs.h>
#include <nnsys/snd.h>

#include "font.h"

#include "../../data/sound_data.sadl"

#define SOUND_HEAP_SIZE 0x80000
#define STREAM_THREAD_PRIO 10

static NNSSndArc arc;
static u8 sndHeap[ SOUND_HEAP_SIZE ];

static NNSSndHeapHandle heap;

static NNSSndHandle bgmHandle;

static u16      gScreen[ 32 * 32 ];     // ���z�X�N���[��

u16 Cont;
u16 Trg;

static void     ClearString( void );
static void     PrintString( int x , int y , u8 palette , char* text , ... );
static void     ColorString( int x , int y , s16 length , u8 palette );

void VBlankIntr( void );

//----------------------------------------------------------------
//   NitroMain
//
void NitroMain()
{
    OS_Init();
    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    // �\���ݒ菉����
    GX_SetBankForLCDC( GX_VRAM_LCDC_ALL );
    MI_CpuClearFast( (void*)HW_LCDC_VRAM , HW_LCDC_VRAM_SIZE );
    (void)GX_DisableBankForLCDC();
    MI_CpuFillFast( (void*)HW_OAM,192 , HW_OAM_SIZE );
    MI_CpuClearFast( (void*)HW_PLTT , HW_PLTT_SIZE );
    MI_CpuFillFast( (void*)HW_DB_OAM , 192,HW_DB_OAM_SIZE );
    MI_CpuClearFast( (void*)HW_DB_PLTT , HW_DB_PLTT_SIZE );

    // ������\���p�ɂQ�c�\���ݒ�
    GX_SetBankForBG( GX_VRAM_BG_128_A );
    G2_SetBG0Control(
        GX_BG_SCRSIZE_TEXT_256x256 ,
        GX_BG_COLORMODE_16 ,
        GX_BG_SCRBASE_0xf800 ,      // SCR �x�[�X�u���b�N 31
        GX_BG_CHARBASE_0x00000 ,    // CHR �x�[�X�u���b�N 0
        GX_BG_EXTPLTT_01
    );
    G2_SetBG0Priority( 0 );
    G2_BG0Mosaic( FALSE );
    GX_SetGraphicsMode( GX_DISPMODE_GRAPHICS , GX_BGMODE_0,GX_BG0_AS_2D );
    GX_SetVisiblePlane( GX_PLANEMASK_BG0 );
    GX_LoadBG0Char( d_CharData , 0 , sizeof( d_CharData ) );
    GX_LoadBGPltt( d_PaletteData , 0 , sizeof( d_PaletteData ) );
    MI_CpuFillFast( (void*)gScreen , 0 , sizeof( gScreen ) );
    DC_FlushRange( gScreen , sizeof( gScreen ) );
    GX_LoadBG0Scr( gScreen , 0 , sizeof( gScreen ) );
    
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
    
    // Load sound data
    (void)NNS_SndArcLoadSeq( SEQ_MARIOKART64_TITLE, heap );
    
    // Initialize sound handle
    NNS_SndHandleInit( &bgmHandle );
    
    // dummy pad read
    Cont = PAD_Read();
    
    // print usage
    OS_Printf("=================================\n");
    OS_Printf("USAGE:\n");
    OS_Printf(" A : start Bgm\n");
    OS_Printf(" B : stop Bgm\n");
    OS_Printf("=================================\n");
    
    // LCD�\���J�n
    GX_DispOn();
    GXS_DispOn();

    //================ Main Loop
    while(1)
    {
        u16 ReadData;
        SNDPlayerInfo playerInfo;
        SNDTrackInfo trackInfo;
        
        SVC_WaitVBlankIntr();
        
        ReadData = PAD_Read();
        Trg  = (u16)(ReadData & (ReadData ^ Cont));
        Cont = ReadData;
        
        ClearString();
        
        // �h���C�o���X�V
        (void)NNS_SndUpdateDriverInfo();
        
        // �h���C�o���\��
        PrintString( 1, 1, 0xf, "Player" );
        PrintString( 1, 5, 0xf, "Trk Vol Pan ChNum" );
        
        if ( NNS_SndPlayerReadDriverPlayerInfo( &bgmHandle, &playerInfo ) )
        {
            if ( playerInfo.activeFlag )
            {
                int trackNo;
                
                PrintString( 2, 2, 0xf, "tempo: %4d", playerInfo.tempo );
                PrintString( 2, 3, 0xf, "volume: %3d", playerInfo.volume );
                
                for( trackNo = 0; trackNo < 16 ; trackNo++ )
                {
                    PrintString( 1, 6 + trackNo, 0xf, "%2d:", trackNo );
                    if ( NNS_SndPlayerReadDriverTrackInfo( &bgmHandle, trackNo, &trackInfo ) ) {
                        PrintString( 5, 6 + trackNo, 0xf, "%3d %3d %2d", trackInfo.volume, trackInfo.pan, trackInfo.chCount );
                    }
                }
            }
        }
        
        if ( Trg & PAD_BUTTON_A ) {
            // start BGM
            (void)NNS_SndArcPlayerStartSeq( &bgmHandle, SEQ_MARIOKART64_TITLE );
        }
        if ( Trg & PAD_BUTTON_B ) {
            // stop BGM
            (void)NNS_SndPlayerStopSeq( &bgmHandle, 1 );
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
    // ���z�X�N���[����VRAM�ɔ��f
    DC_FlushRange( gScreen , sizeof( gScreen ) );
    GX_LoadBG0Scr( gScreen , 0 , sizeof( gScreen ) );

    // IRQ �`�F�b�N�t���O���Z�b�g
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

/*---------------------------------------------------------------------------*
  Name:         ClearString

  Description:  ���z�X�N���[�����N���A����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void
ClearString(void)
{
    MI_CpuClearFast( (void*)gScreen , sizeof( gScreen ) );
}

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  ���z�X�N���[���ɕ������z�u����B�������32�����܂ŁB

  Arguments:    x       - ������̐擪��z�u���� x ���W( �~ 8 �h�b�g )�B
                y       - ������̐擪��z�u���� y ���W( �~ 8 �h�b�g )�B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B
                text    - �z�u���镶����B�I�[������NULL�B
                ...     - ���z�����B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void
PrintString( int x , int y , u8 palette , char* text , ... )
{
    va_list vlist;
    char    temp[ 32 + 2 ];
    s32     i;

    va_start( vlist , text );
    (void)vsnprintf( temp , 33 , text , vlist );
    va_end( vlist );

    *(u16*)( &temp[ 32 ] ) = 0x0000;
    for( i = 0 ; ; i ++ )
    {
        if( temp[ i ] == 0x00 )
        {
            break;
        }
        gScreen[ ( ( y * 32 ) + x + i ) % ( 32 * 32 ) ] = (u16)(
            ( palette << 12 ) | temp[ i ]
        );
    }
}

/*---------------------------------------------------------------------------*
  Name:         ColorString

  Description:  ���z�X�N���[���ɔz�u����������̐F��ύX����B

  Arguments:    x       - �F�ύX���J�n���� x ���W( �~ 8 �h�b�g )�B
                y       - �F�ύX���J�n���� y ���W( �~ 8 �h�b�g )�B
                length  - �A�����ĐF�ύX���镶�����B
                palette - �����̐F���p���b�g�ԍ��Ŏw��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void
ColorString( int x , int y , s16 length , u8 palette )
{
    s32     i;
    u16     temp;
    s32     index;

    if( length < 0 ) return;

    for( i = 0 ; i < length ; i ++ )
    {
        index = ( ( y * 32 ) + x + i ) % ( 32 * 32 );
        temp = gScreen[ index ];
        temp &= 0x0fff;
        temp |= ( palette << 12 );
        gScreen[ index ] = temp;
    }
}

