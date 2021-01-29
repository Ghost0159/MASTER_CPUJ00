/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - snd
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.41 $
 *---------------------------------------------------------------------------*/
#include <nnsys/snd/main.h>

#include <nitro/spi.h>
#include <nnsys/misc.h>
#include <nnsys/snd/capture.h>
#include <nnsys/snd/player.h>
#include <nnsys/snd/sndarc_stream.h>
#include <nnsys/snd/resource_mgr.h>
#include <nnsys/snd/config.h>

/******************************************************************************
	static variables
 ******************************************************************************/

static PMSleepCallbackInfo sPreSleepCallback;
static PMSleepCallbackInfo sPostSleepCallback;

static SNDDriverInfo sDriverInfo[2] ATTRIBUTE_ALIGN( 32 );
static u32 sDriverInfoCommandTag;
static s8 sCurDriverInfo;
static BOOL sDriverInfoFirstFlag;

/******************************************************************************
	static function declaration
 ******************************************************************************/
static void BeginSleep( void* );
static void EndSleep( void* );

static const SNDDriverInfo* GetCurDriverInfo( void );

/******************************************************************************
	inline functions
 ******************************************************************************/
static NNS_SND_INLINE const SNDDriverInfo* GetCurDriverInfo( void )
{
    if ( sCurDriverInfo < 0 ) return NULL;
    return &sDriverInfo[ sCurDriverInfo ];
}

/******************************************************************************
	public functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNS_SndInit

  Description:  �T�E���h���C�u�����̏�����

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndInit( void )
{
    {
        // �Q�d�������`�F�b�N
        static BOOL initialized = FALSE;
        if ( initialized ) return;
        initialized = TRUE;
    }
    
    // �T�E���h�h���C�o�̏�����
    SND_Init();
    
    // �X���[�v�R�[���o�b�N�̓o�^
    PM_SetSleepCallbackInfo( &sPreSleepCallback, BeginSleep, NULL );
    PM_SetSleepCallbackInfo( &sPostSleepCallback, EndSleep, NULL );
    
    PM_PrependPreSleepCallback( &sPreSleepCallback );
    PM_AppendPostSleepCallback( &sPostSleepCallback );
    
    // �e���C�u�����̏�����
    NNSi_SndInitResourceMgr();
    NNSi_SndCaptureInit();
    NNSi_SndPlayerInit();
    
    // �h���C�o���̏�����
    sCurDriverInfo = -1;
    sDriverInfoFirstFlag = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndMain

  Description:  �T�E���h���C�u�����̃t���[�����[�N

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndMain( void )
{
    // ARM7������M
    while ( SND_RecvCommandReply( SND_COMMAND_NOBLOCK ) != NULL ) {}
    
    // �e���C�u�����̃t���[�����[�N
    NNSi_SndPlayerMain();
    NNSi_SndCaptureMain();
#ifndef SDK_SMALL_BUILD    
    NNSi_SndArcStrmMain();
#endif /* SDK_SMALL_BUILD */
    
    // ARM7�R�}���h���s
    (void)SND_FlushCommand( SND_COMMAND_NOBLOCK );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndSetMasterVolume

  Description:  �}�X�^�[�{�����[���̐ݒ�

  Arguments:    volume - �}�X�^�[�{�����[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndSetMasterVolume( int volume )
{
    NNS_MINMAX_ASSERT( volume, 0, SND_MASTER_VOLUME_MAX );
    
    SND_SetMasterVolume( volume );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndSetMonoFlag

  Description:  ���m�����t���O�̕ύX

  Arguments:    flag - ���m������L���ɂ��邩�ǂ���

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndSetMonoFlag( BOOL flag )
{
    if ( flag ) SND_SetMasterPan( SND_CHANNEL_PAN_CENTER );
    else SND_ResetMasterPan();
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndStopSoundAll

  Description:  �S�T�E���h�̑�����~

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndStopSoundAll( void )
{
    u32 commandTag;
    
    NNS_SndPlayerStopSeqAll( 0 );
#ifndef SDK_SMALL_BUILD
    NNS_SndArcStrmStopAll( 0 );
#endif /* SDK_SMALL_BUILD */    
    NNSi_SndCaptureStop();
    
    SNDi_SetSurroundDecay( 0 );    
    SND_StopTimer( 0xffff, 0xffff, 0xffff, 0 );
    
    // ARM7�ł̏����҂�
    commandTag = SND_GetCurrentCommandTag();
    (void)SND_FlushCommand( SND_COMMAND_BLOCK );
    SND_WaitForCommandProc( commandTag );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndStopChannelAll

  Description:  �S�`�����l���̒�~

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndStopChannelAll( void )
{
    SND_StopUnlockedChannel( 0xffff, 0 );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndUpdateDriverInfo

  Description:  �h���C�o���̍X�V

  Arguments:    None.

  Returns:      �X�V�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndUpdateDriverInfo( void )
{
    if ( ! sDriverInfoFirstFlag )
    {
        // ARM7������M
        while ( SND_RecvCommandReply( SND_COMMAND_NOBLOCK ) != NULL ) {}
        
        if ( ! SND_IsFinishedCommandTag( sDriverInfoCommandTag ) ) {
            // �X�V������
            return FALSE;
        }
        
        // �X�V����
        if ( sCurDriverInfo < 0 ) sCurDriverInfo = 1;
        
        SND_ReadDriverInfo( &sDriverInfo[ sCurDriverInfo ] );
        sDriverInfoCommandTag = SND_GetCurrentCommandTag();
        
        if ( sCurDriverInfo == 0 ) sCurDriverInfo = 1;
        else sCurDriverInfo = 0;
        
        DC_InvalidateRange( &sDriverInfo[ sCurDriverInfo ], sizeof( SNDDriverInfo ) );
        
        // ARM7�R�}���h���s
        (void)SND_FlushCommand( SND_COMMAND_NOBLOCK );
        
        return TRUE;
    }
    else
    {
        // �n�߂Ă̍X�V
        SND_ReadDriverInfo( &sDriverInfo[0] );
        sDriverInfoCommandTag = SND_GetCurrentCommandTag();
        sDriverInfoFirstFlag = FALSE;
        return FALSE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndReadDriverChannelInfo

  Description:  �`�����l�����̎擾

  Arguments:    chNo - �`�����l���ԍ�
                info - �擾���������i�[����`�����l�����\����

  Returns:      �擾�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndReadDriverChannelInfo( int chNo, SNDChannelInfo* info )
{
    const SNDDriverInfo* driverInfo;
    
    NNS_NULL_ASSERT( info );
    
    driverInfo = GetCurDriverInfo();
    if ( driverInfo == NULL ) return FALSE;
    
    return SND_ReadChannelInfo( driverInfo, chNo, info );
}

/******************************************************************************
	private functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndReadDriverPlayerInfo

  Description:  �v���C���[���̎擾

  Arguments:    playerNo - �v���C���[�ԍ��i�����ԍ��j
                info - �擾���������i�[����v���C���[���\����

  Returns:      �擾�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNSi_SndReadDriverPlayerInfo( int playerNo, SNDPlayerInfo* info )
{
    const SNDDriverInfo* driverInfo;
    
    NNS_NULL_ASSERT( info );
    
    driverInfo = GetCurDriverInfo();
    if ( driverInfo == NULL ) return FALSE;

    return SND_ReadPlayerInfo( driverInfo, playerNo, info );
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndReadDriverTrackInfo

  Description:  �g���b�N���̎擾

  Arguments:    playerNo - �v���C���[�ԍ��i�����ԍ��j
                trackNo - �g���b�N�ԍ�
                info - �擾���������i�[����g���b�N���\����

  Returns:      �擾�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNSi_SndReadDriverTrackInfo( int playerNo, int trackNo, SNDTrackInfo* info )
{
    const SNDDriverInfo* driverInfo;
    
    NNS_NULL_ASSERT( info );
    
    driverInfo = GetCurDriverInfo();
    if ( driverInfo == NULL ) return FALSE;
    
    return SND_ReadTrackInfo( driverInfo, playerNo, trackNo, info );
}

/******************************************************************************
	static functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         BeginSleep

  Description:  �X���[�v���[�h�O����

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void BeginSleep( void* )
{
    u32 commandTag;
    
    NNSi_SndCaptureBeginSleep();
    
    // �R�}���h�o�b�t�@����ɂ��邽�߁A�_�~�[�R�}���h���o
    SND_StopTimer( 0, 0, 0, 0 );
    
    // ARM7�ł̏����҂�
    commandTag = SND_GetCurrentCommandTag();
    (void)SND_FlushCommand( SND_COMMAND_BLOCK );
    SND_WaitForCommandProc( commandTag );
}

/*---------------------------------------------------------------------------*
  Name:         EndSleep

  Description:  �X���[�v���[�h�㏈��

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void EndSleep( void* )
{
    NNSi_SndCaptureEndSleep();
}

/*====== End of main.c ======*/

