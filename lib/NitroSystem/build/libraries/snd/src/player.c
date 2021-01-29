/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - snd
  File:     player.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.84 $
 *---------------------------------------------------------------------------*/
#include <nnsys/snd/player.h>

#include <nitro/os.h>
#include <nitro/snd.h>
#include <nnsys/misc.h>
#include <nnsys/snd/main.h>

/******************************************************************************
    macro definition
 ******************************************************************************/

#define FADER_SHIFT 8

/******************************************************************************
    structure definition
 ******************************************************************************/

typedef struct NNSSndPlayerHeap
{
    NNSFndLink link;
    NNSSndHeapHandle handle;
    NNSSndSeqPlayer* player;
    int playerNo;
} NNSSndPlayerHeap;

/******************************************************************************
    static variables
 ******************************************************************************/

static NNSSndSeqPlayer sSeqPlayer[ SND_PLAYER_NUM ];
static NNSSndPlayer sPlayer[ NNS_SND_PLAYER_NUM ];
static NNSFndList sPrioList;
static NNSFndList sFreeList;

/******************************************************************************
    static function declarations
 ******************************************************************************/

static void InitPlayer( NNSSndSeqPlayer* seqPlayer );
static void ShutdownPlayer( NNSSndSeqPlayer* seqPlayer );
static void ForceStopSeq( NNSSndSeqPlayer* seqPlayer );
static NNSSndSeqPlayer* AllocSeqPlayer( int prio );
static void InsertPlayerList( NNSSndPlayer* player, NNSSndSeqPlayer* seqPlayer );
static void InsertPrioList( NNSSndSeqPlayer* seqPlayer );
static void SetPlayerPriority( NNSSndSeqPlayer* seqPlayer, int priority );
static void PlayerHeapDisposeCallback( void* mem, u32 size, u32 data1, u32 data2 );

/******************************************************************************
    public functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetPlayerVolume

  Description:  �v���C���[���̃{�����[���ݒ�

  Arguments:    playerNo - �v���C���[�ԍ�
                volume   - �{�����[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetPlayerVolume( int playerNo, int volume )
{
    NNS_MINMAX_ASSERT( playerNo, 0, NNS_SND_PLAYER_NO_MAX );
    NNS_MINMAX_ASSERT( volume, 0, SND_CALC_DECIBEL_SCALE_MAX );
    
    sPlayer[ playerNo ].volume = (u8)volume;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetPlayableSeqCount

  Description:  �V�[�P���X�ő哯���Đ����̐ݒ�

  Arguments:    playerNo - �v���C���[�ԍ�
                seqCount - �V�[�P���X�ő哯���Đ���

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetPlayableSeqCount( int playerNo, int seqCount )
{
    NNS_MINMAX_ASSERT( playerNo, 0, NNS_SND_PLAYER_NO_MAX );
    NNS_MINMAX_ASSERT( seqCount, 0, SND_PLAYER_NUM );
    
    sPlayer[ playerNo ].playableSeqCount = (u16)seqCount;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetAllocatableChannel

  Description:  �m�ۉ\�ȃ`�����l���̐ݒ�

  Arguments:    playerNo - �v���C���[�ԍ�
                chBitFlag - �m�ۉ\�ȃ`�����l���̃r�b�g�t���O
                            �O���w�肷��ƁA�S�Ċm�ۉ\.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetAllocatableChannel( int playerNo, u32 chBitFlag )
{
    NNS_MINMAX_ASSERT( playerNo, 0, NNS_SND_PLAYER_NO_MAX );
    NNS_MINMAX_ASSERT( chBitFlag, 0, 0xffff );
    
    sPlayer[ playerNo ].allocChBitFlag = chBitFlag;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerCreateHeap

  Description:  �v���C���[�q�[�v���쐬

  Arguments:    playerNo - �v���C���[�ԍ�
                heap - �T�E���h�q�[�v
                size - �v���C���[�q�[�v�̃T�C�Y

  Returns:      �v���C���[�q�[�v�̍쐬�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerCreateHeap( int playerNo, NNSSndHeapHandle heap, u32 size )
{
    NNSSndHeapHandle playerHeapHandle;
    NNSSndPlayerHeap* playerHeap;    
    void* buffer;
    
    NNS_MINMAX_ASSERT( playerNo, 0, NNS_SND_PLAYER_NO_MAX );
    
    /* �v���C���[�q�[�v�p�o�b�t�@�̊m�� */
    buffer = NNS_SndHeapAlloc( heap, sizeof( NNSSndPlayerHeap ) + size, PlayerHeapDisposeCallback, 0, 0 );
    if ( buffer == NULL ) {
        return FALSE;
    }
    
    /* �v���C���[�q�[�v�\���̏����� */
    playerHeap = (NNSSndPlayerHeap*)buffer;
    
    playerHeap->player = NULL;
    playerHeap->playerNo = playerNo;
    playerHeap->handle = NNS_SND_HEAP_INVALID_HANDLE;
    
    /* �v���C���[�q�[�v�\�z */
    playerHeapHandle = NNS_SndHeapCreate(
        (u8*)buffer + sizeof( NNSSndPlayerHeap ),
        size
    );
    if ( playerHeapHandle == NNS_SND_HEAP_INVALID_HANDLE ) {
        return FALSE;
    }
    
    playerHeap->handle = playerHeapHandle;    
    NNS_FndAppendListObject( &sPlayer[ playerNo ].heapList, playerHeap );
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerStopSeq

  Description:  �V�[�P���X��~

  Arguments:    handle    - �T�E���h�n���h��
                fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerStopSeq( NNSSndHandle* handle, int fadeFrame )
{
    NNS_NULL_ASSERT( handle );
    
    NNSi_SndPlayerStopSeq( handle->player, fadeFrame );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerStopSeqByPlayerNo

  Description:  �V�[�P���X��~

  Arguments:    playerNo  - �v���C���[�ԍ�
                fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerStopSeqByPlayerNo( int playerNo, int fadeFrame )
{
    NNSSndSeqPlayer* seqPlayer;
    int i;
    
    NNS_MINMAX_ASSERT( playerNo , 0, NNS_SND_PLAYER_NO_MAX );
    
    for( i = 0; i < SND_PLAYER_NUM ; i++ )
    {
        seqPlayer = & sSeqPlayer[ i ];
        
        if ( seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_STOP &&
             seqPlayer->player == & sPlayer[ playerNo ] )
        {
            NNSi_SndPlayerStopSeq( seqPlayer, fadeFrame );            
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerStopSeqBySeqNo

  Description:  �V�[�P���X��~

  Arguments:    seqNo     - �V�[�P���X�ԍ�
                fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerStopSeqBySeqNo( int seqNo, int fadeFrame )
{
    NNSSndSeqPlayer* seqPlayer;
    int i;
    
    for( i = 0; i < SND_PLAYER_NUM ; i++ )
    {
        seqPlayer = & sSeqPlayer[ i ];
        
        if ( seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_STOP &&
             seqPlayer->seqType == NNS_SND_PLAYER_SEQ_TYPE_SEQ &&
             seqPlayer->seqNo == seqNo )
        {
            NNSi_SndPlayerStopSeq( seqPlayer ,fadeFrame );
        }
    }    
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerStopSeqBySeqArcNo

  Description:  �V�[�P���X��~

  Arguments:    seqArcNo  - �V�[�P���X�A�[�J�C�u�ԍ�
                fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerStopSeqBySeqArcNo( int seqArcNo, int fadeFrame )
{
    NNSSndSeqPlayer* seqPlayer;
    int i;
    
    for( i = 0; i < SND_PLAYER_NUM ; i++ )
    {
        seqPlayer = & sSeqPlayer[ i ];
        
        if ( seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_STOP &&
             seqPlayer->seqType == NNS_SND_PLAYER_SEQ_TYPE_SEQARC &&
             seqPlayer->seqNo == seqArcNo )
        {
            NNSi_SndPlayerStopSeq( seqPlayer ,fadeFrame );
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerStopSeqBySeqArcIdx

  Description:  �V�[�P���X��~

  Arguments:    seqArcNo  - �V�[�P���X�A�[�J�C�u�ԍ�
                index     - �C���f�b�N�X�ԍ�
                fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerStopSeqBySeqArcIdx( int seqArcNo, int index, int fadeFrame )
{
    NNSSndSeqPlayer* seqPlayer;
    int i;
    
    for( i = 0; i < SND_PLAYER_NUM ; i++ )
    {
        seqPlayer = & sSeqPlayer[ i ];
        
        if ( seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_STOP &&
             seqPlayer->seqType == NNS_SND_PLAYER_SEQ_TYPE_SEQARC &&
             seqPlayer->seqNo == seqArcNo &&
             seqPlayer->seqArcIndex == index )
        {
            NNSi_SndPlayerStopSeq( seqPlayer ,fadeFrame );
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerStopSeqAll

  Description:  �S�V�[�P���X��~

  Arguments:    fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerStopSeqAll( int fadeFrame )
{
    NNSSndSeqPlayer* seqPlayer;
    int i;
    
    for( i = 0; i < SND_PLAYER_NUM ; i++ )
    {
        seqPlayer = & sSeqPlayer[ i ];
        
        if ( seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_STOP )
        {
            NNSi_SndPlayerStopSeq( seqPlayer, fadeFrame );
        }
    }    
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerPause

  Description:  �V�[�P���X�̈ꎞ��~�܂��͍ĊJ

  Arguments:    handle - �T�E���h�n���h��
                flag   - �ꎞ��~���ĊJ��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerPause( NNSSndHandle* handle, BOOL flag )
{
    NNS_NULL_ASSERT( handle );

    NNSi_SndPlayerPause( handle->player, flag );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerPauseByPlayerNo

  Description:  �V�[�P���X�̈ꎞ��~�܂��͍ĊJ

  Arguments:    playerNo - �v���C���[�ԍ�
                flag     - �ꎞ��~���ĊJ��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerPauseByPlayerNo( int playerNo, BOOL flag )
{
    NNSSndSeqPlayer* seqPlayer;
    NNSSndSeqPlayer* next;
    
    NNS_MINMAX_ASSERT( playerNo , 0, NNS_SND_PLAYER_NO_MAX );
    
    for( seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPlayer[ playerNo ].playerList, NULL );
         seqPlayer != NULL ; seqPlayer = next )
    {
        next = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPlayer[ playerNo ].playerList, seqPlayer );
        
        NNSi_SndPlayerPause( seqPlayer, flag );
    }    
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerPauseAll

  Description:  �S�V�[�P���X�̈ꎞ��~�܂��͍ĊJ

  Arguments:    flag - �ꎞ��~���ĊJ��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerPauseAll( BOOL flag )
{
    NNSSndSeqPlayer* seqPlayer;
    NNSSndSeqPlayer* next;
    
    for( seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, NULL );
         seqPlayer != NULL ; seqPlayer = next )
    {
        next = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, seqPlayer );
        
        NNSi_SndPlayerPause( seqPlayer, flag );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHandleInit

  Description:  �T�E���h�n���h���̏�����

  Arguments:    handle - �T�E���h�n���h��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndHandleInit( NNSSndHandle* handle )
{
    NNS_NULL_ASSERT( handle );
    
    handle->player = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndHandleReleaseSeq

  Description:  �T�E���h�n���h������V�[�P���X�����

  Arguments:    handle - �T�E���h�n���h��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndHandleReleaseSeq( NNSSndHandle* handle )
{
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    NNS_ASSERT( handle == handle->player->handle );
    
    handle->player->handle = NULL;  
    handle->player = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerCountPlayingSeqByPlayerNo

  Description:  �Đ����̃V�[�P���X���擾

  Arguments:    playerNo - �v���C���[�ԍ�

  Returns:      �Đ����̃V�[�P���X��
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerCountPlayingSeqByPlayerNo( int playerNo )
{
    NNS_MINMAX_ASSERT( playerNo , 0, NNS_SND_PLAYER_NO_MAX );
    
    return sPlayer[ playerNo ].playerList.numObjects;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerCountPlayingSeqBySeqNo

  Description:  �Đ����̃V�[�P���X���擾

  Arguments:    seqNo - �V�[�P���X�ԍ�

  Returns:      �Đ����̃V�[�P���X��
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerCountPlayingSeqBySeqNo( int seqNo )
{
    int count = 0;
    
    NNSSndSeqPlayer* seqPlayer = NULL;
    while ( ( seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, seqPlayer ) ) != NULL )
    {
        if ( seqPlayer->seqType == NNS_SND_PLAYER_SEQ_TYPE_SEQ &&
             seqPlayer->seqNo == seqNo )
        {
            count++;
        }
    }
    
    return count;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerCountPlayingSeqBySeqArcNo

  Description:  �Đ����̃V�[�P���X���擾

  Arguments:    seqArcNo - �V�[�P���X�A�[�J�C�u�ԍ�

  Returns:      �Đ����̃V�[�P���X��
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerCountPlayingSeqBySeqArcNo( int seqArcNo )
{
    int count = 0;
    
    NNSSndSeqPlayer* seqPlayer = NULL;
    while ( ( seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, seqPlayer ) ) != NULL )
    {
        if ( seqPlayer->seqType == NNS_SND_PLAYER_SEQ_TYPE_SEQARC &&
             seqPlayer->seqNo == seqArcNo )
        {
            count++;
        }
    }
    
    return count;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerCountPlayingSeqBySeqArcIdx

  Description:  �Đ����̃V�[�P���X���擾

  Arguments:    seqArcNo - �V�[�P���X�A�[�J�C�u�ԍ�
                index    - �C���f�b�N�X�ԍ�

  Returns:      �Đ����̃V�[�P���X��
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerCountPlayingSeqBySeqArcIdx( int seqArcNo, int index )
{
    int count = 0;
    
    NNSSndSeqPlayer* seqPlayer = NULL;
    while ( ( seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, seqPlayer ) ) != NULL )
    {
        if ( seqPlayer->seqType == NNS_SND_PLAYER_SEQ_TYPE_SEQARC &&
             seqPlayer->seqNo == seqArcNo &&
             seqPlayer->seqArcIndex == index )
        {
            count++;
        }
    }
    
    return count;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetVolume

  Description:  �V�[�P���X�{�����[���̕ύX

  Arguments:    handle - �T�E���h�n���h��
                volume - �{�����[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetVolume( NNSSndHandle* handle, int volume )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( volume, 0, SND_CALC_DECIBEL_SCALE_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    handle->player->extVolume = (u8)volume;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetInitialVolume

  Description:  �V�[�P���X�����{�����[���̐ݒ�

  Arguments:    handle - �T�E���h�n���h��
                volume - �{�����[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetInitialVolume( NNSSndHandle* handle, int volume )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( volume, 0, SND_CALC_DECIBEL_SCALE_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    handle->player->initVolume = (u8)volume;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerMoveVolume

  Description:  �V�[�P���X�{�����[�������ԕω�

  Arguments:    handle       - �T�E���h�n���h��
                targetVolume - �^�[�Q�b�g�{�����[��
                frames       - �ω��t���[����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerMoveVolume( NNSSndHandle* handle, int targetVolume, int frames )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( targetVolume, 0, SND_CALC_DECIBEL_SCALE_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    // �t�F�[�h�A�E�g���͋֎~    
    if ( handle->player->status == NNS_SND_SEQ_PLAYER_STATUS_FADEOUT ) return;
    
    NNSi_SndFaderSet( & handle->player->fader, targetVolume << FADER_SHIFT, frames );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetPlayerPriority

  Description:  �v���C���[�v���C�I���e�B��ύX

  Arguments:    handle   - �T�E���h�n���h��
                priority - �v���C���[�v���C�I���e�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetPlayerPriority( NNSSndHandle* handle, int priority )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( priority, 0, NNS_SND_PLAYER_PRIO_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SetPlayerPriority( handle->player, priority );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetChannelPriority

  Description:  �����v���C�I���e�B��ύX

  Arguments:    handle   - �T�E���h�n���h��
                priority - �����v���C�I���e�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetChannelPriority( NNSSndHandle* handle, int priority )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( priority, 0, 127 );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetPlayerChannelPriority( handle->player->playerNo, priority );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackMute

  Description:  �g���b�N�~���[�g

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                flag         - �~���[�g�t���O

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackMute( NNSSndHandle* handle, u16 trackBitMask, BOOL flag )
{
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackMute(
        handle->player->playerNo,
        trackBitMask,
        flag
    );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackMuteEx

  Description:  �g���b�N�~���[�g

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                mute         - �~���[�g�ݒ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
#if SDK_CURRENT_VERSION_NUMBER >= SDK_VERSION_NUMBER(3,1,0)
void NNS_SndPlayerSetTrackMuteEx( NNSSndHandle* handle, u16 trackBitMask, NNSSndSeqMute mute )
{
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackMuteEx(
        handle->player->playerNo,
        trackBitMask,
        (SNDSeqMute)mute
    );
}
void SND_SetTrackMuteEx(int playerNo, u32 trackBitMask, SNDSeqMute mute);
SDK_WEAK_SYMBOL void SND_SetTrackMuteEx(int playerNo, u32 trackBitMask, SNDSeqMute mute)
{
    (void)playerNo;
    (void)trackBitMask;
    (void)mute;
    
    NNS_WARNING( FALSE, "SND_SetTrackMuteEx is not supported." );
}
#endif

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackVolume

  Description:  �g���b�N�{�����[���̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                volume       - �{�����[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackVolume( NNSSndHandle* handle, u16 trackBitMask, int volume )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( volume, 0, SND_CALC_DECIBEL_SCALE_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackVolume(
        handle->player->playerNo,
        trackBitMask,
        SND_CalcDecibel( volume )
    );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackPitch

  Description:  �g���b�N�s�b�`�̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                pitch        - ����

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackPitch( NNSSndHandle* handle, u16 trackBitMask, int pitch )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( pitch, NNS_SND_PLAYER_TRACK_PITCH_MIN, NNS_SND_PLAYER_TRACK_PITCH_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackPitch( handle->player->playerNo, trackBitMask, pitch );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackPan

  Description:  �g���b�N�p���̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                pan          - �p��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackPan( NNSSndHandle* handle, u16 trackBitMask, int pan )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( pan, NNS_SND_PLAYER_TRACK_PAN_MIN, NNS_SND_PLAYER_TRACK_PAN_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackPan( handle->player->playerNo, trackBitMask, pan );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackPanRange

  Description:  �g���b�N�p�������W�̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                panRange     - �p�������W

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackPanRange( NNSSndHandle* handle, u16 trackBitMask, int panRange )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( panRange, NNS_SND_PLAYER_TRACK_PAN_RANGE_MIN, NNS_SND_PLAYER_TRACK_PAN_RANGE_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackPanRange( handle->player->playerNo, trackBitMask, panRange );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackModDepth

  Description:  ���W�����[�V�����f�v�X�̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                depth        - ���W�����[�V�����f�v�X

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackModDepth( NNSSndHandle* handle, u16 trackBitMask, int depth )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( depth, 0, 255 );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackModDepth( handle->player->playerNo, trackBitMask, depth );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackModSpeed

  Description:  ���W�����[�V�����X�s�[�h�̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                speed        - ���W�����[�V�����X�s�[�h

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackModSpeed( NNSSndHandle* handle, u16 trackBitMask, int speed )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( speed, 0, 255 );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackModSpeed( handle->player->playerNo, trackBitMask, speed );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTrackAllocatableChannel

  Description:  �m�ۂł���`�����l���̕ύX

  Arguments:    handle       - �T�E���h�n���h��
                trackBitMask - �g���b�N�r�b�g�}�X�N
                chBitFlag    - �m�ۂł���`�����l���̃r�b�g�t���O
                               �O���w�肷��ƁA�S�Ċm�ۉ\.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTrackAllocatableChannel( NNSSndHandle* handle, u16 trackBitMask, u32 chBitFlag )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( chBitFlag, 0, 0xffff );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetTrackAllocatableChannel( handle->player->playerNo, trackBitMask, chBitFlag );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetTempoRatio

  Description:  �e���|�䗦�̕ύX

  Arguments:    handle - �T�E���h�n���h��
                ratio  - �e���|�̔䗦

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetTempoRatio( NNSSndHandle* handle, int ratio )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( ratio, 1, 65535 );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    SND_SetPlayerTempoRatio( handle->player->playerNo, ratio );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetSeqNo

  Description:  �V�[�P���X�ԍ��̐ݒ�

  Arguments:    handle - �T�E���h�n���h��
                seqNo  - �V�[�P���X�ԍ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetSeqNo( NNSSndHandle* handle, int seqNo )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( seqNo, 0, NNS_SND_PLAYER_SEQ_NO_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    handle->player->seqType = NNS_SND_PLAYER_SEQ_TYPE_SEQ;
    handle->player->seqNo = (u16)seqNo;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerSetSeqArcNo

  Description:  �V�[�P���X�A�[�J�C�u�ԍ��̐ݒ�

  Arguments:    handle   - �T�E���h�n���h��
                seqArcNo - �V�[�P���X�A�[�J�C�u�ԍ�
                index    - �C���f�b�N�X�ԍ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndPlayerSetSeqArcNo( NNSSndHandle* handle, int seqArcNo, int index )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( seqArcNo, 0, NNS_SND_PLAYER_SEQ_NO_MAX );
    NNS_MINMAX_ASSERT( index, 0, NNS_SND_PLAYER_SEQARC_INDEX_MAX );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return;
    
    handle->player->seqType = NNS_SND_PLAYER_SEQ_TYPE_SEQARC;
    handle->player->seqNo       = (u16)seqArcNo;
    handle->player->seqArcIndex = (u16)index;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerGetSeqType

  Description:  �V�[�P���X�^�C�v�̎擾

  Arguments:    handle - �T�E���h�n���h��

  Returns:      �V�[�P���X�^�C�v
 *---------------------------------------------------------------------------*/
NNSSndPlayerSeqType NNS_SndPlayerGetSeqType( NNSSndHandle* handle )
{
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return NNS_SND_PLAYER_SEQ_TYPE_INVALID;
    
    return (NNSSndPlayerSeqType)( handle->player->seqType );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerGetSeqNo

  Description:  �V�[�P���X�ԍ��̎擾

  Arguments:    handle - �T�E���h�n���h��

  Returns:      �V�[�P���X�ԍ��܂��́A-1
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerGetSeqNo( NNSSndHandle* handle )
{
    NNS_NULL_ASSERT( handle );

    if ( ! NNS_SndHandleIsValid( handle ) ) return -1;
    
    if ( handle->player->seqType != NNS_SND_PLAYER_SEQ_TYPE_SEQ ) return -1;

    return handle->player->seqNo;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerGetSeqArcNo

  Description:  �V�[�P���X�A�[�J�C�u�ԍ��̎擾

  Arguments:    handle - �T�E���h�n���h��

  Returns:      �V�[�P���X�A�[�J�C�u�ԍ��܂��́A-1
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerGetSeqArcNo( NNSSndHandle* handle )
{
    NNS_NULL_ASSERT( handle );

    if ( ! NNS_SndHandleIsValid( handle ) ) return -1;
    
    if ( handle->player->seqType != NNS_SND_PLAYER_SEQ_TYPE_SEQARC ) return -1;

    return handle->player->seqNo;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerGetSeqArcIdx

  Description:  �V�[�P���X�A�[�J�C�u�C���f�b�N�X�̎擾

  Arguments:    handle - �T�E���h�n���h��

  Returns:      �V�[�P���X�A�[�J�C�u�C���f�b�N�X�܂��́A-1
 *---------------------------------------------------------------------------*/
int NNS_SndPlayerGetSeqArcIdx( NNSSndHandle* handle )
{
    NNS_NULL_ASSERT( handle );

    if ( ! NNS_SndHandleIsValid( handle ) ) return -1;
    
    if ( handle->player->seqType != NNS_SND_PLAYER_SEQ_TYPE_SEQARC ) return -1;
    
    return handle->player->seqArcIndex;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerReadVariable

  Description:  �V�[�P���X���[�J���ϐ��̓ǂ݂Ƃ�

  Arguments:    handle - �T�E���h�n���h��
                varNo  - �ϐ��ԍ�
                var    - �ǂݍ��ݐ�A�h���X

  Returns:      �����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerReadVariable( NNSSndHandle* handle, int varNo, s16* var )
{
    NNSSndSeqPlayer* seqPlayer;
    
    NNS_NULL_ASSERT( handle );
    NNS_NULL_ASSERT( var );
    NNS_MINMAX_ASSERT( varNo, 0, SND_PLAYER_VARIABLE_NUM-1 );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return FALSE;
    
    seqPlayer = handle->player;
    
    if ( ! seqPlayer->startFlag ) {
        *var = SND_DEFAULT_VARIABLE;
        return TRUE;
    }
    
    *var = SND_GetPlayerLocalVariable( seqPlayer->playerNo, varNo );
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerReadGlobalVariable

  Description:  �V�[�P���X�O���[�o���ϐ��̓ǂ݂Ƃ�

  Arguments:    varNo - �ϐ��ԍ�
                var   - �ǂݍ��ݐ�A�h���X

  Returns:      �����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerReadGlobalVariable( int varNo, s16* var )
{
    NNS_NULL_ASSERT( var );
    NNS_MINMAX_ASSERT( varNo, 0, SND_GLOBAL_VARIABLE_NUM-1 );
    
    *var = SND_GetPlayerGlobalVariable( varNo );
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerWriteVariable

  Description:  �V�[�P���X���[�J���ϐ��̏�������

  Arguments:    handle - �T�E���h�n���h��
                varNo  - �ϐ��ԍ�
                var    - �������ޒl

  Returns:      �����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerWriteVariable( NNSSndHandle* handle, int varNo, s16 var )
{
    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( varNo, 0, SND_PLAYER_VARIABLE_NUM-1 );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return FALSE;
    
    SND_SetPlayerLocalVariable( handle->player->playerNo, varNo, var );

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerWriteGlobalVariable

  Description:  �V�[�P���X�O���[�o���ϐ��̏�������

  Arguments:    varNo - �ϐ��ԍ�
                var   - �������ޒl

  Returns:      �����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerWriteGlobalVariable( int varNo, s16 var )
{
    NNS_MINMAX_ASSERT( varNo, 0, SND_GLOBAL_VARIABLE_NUM-1 );
    
    SND_SetPlayerGlobalVariable( varNo, var );

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerGetTick

  Description:  �Đ��J�n����̃e�B�b�N���̎擾

  Arguments:    handle - �T�E���h�n���h��

  Returns:      �e�B�b�N��
 *---------------------------------------------------------------------------*/
u32 NNS_SndPlayerGetTick( NNSSndHandle* handle )
{
    NNSSndSeqPlayer* seqPlayer;
    
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return 0;
    
    seqPlayer = handle->player;
    
    if ( ! seqPlayer->startFlag ) {
        // pre start
        return 0;
    }
    
    return SND_GetPlayerTickCounter( seqPlayer->playerNo );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerReadDriverPlayerInfo

  Description:  �h���C�o�̃v���C���[���̎擾

  Arguments:    handle - �T�E���h�n���h��
                info - �擾�����v���C���[�����i�[����v���C���[���\����

  Returns:      ���̎擾�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerReadDriverPlayerInfo( NNSSndHandle* handle, SNDPlayerInfo* info )
{
    NNSSndSeqPlayer* seqPlayer;
    
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return FALSE;
    
    seqPlayer = handle->player;
    NNS_NULL_ASSERT( seqPlayer );
    
    return NNSi_SndReadDriverPlayerInfo( seqPlayer->playerNo, info );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndPlayerReadDriverTrackInfo

  Description:  �h���C�o�̃g���b�N���̎擾

  Arguments:    handle - �T�E���h�n���h��
                trackNo - �g���b�N�ԍ�
                info - �擾�����v���C���[�����i�[����g���b�N���\����

  Returns:      ���̎擾�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndPlayerReadDriverTrackInfo( NNSSndHandle* handle, int trackNo, SNDTrackInfo* info )
{
    NNSSndSeqPlayer* seqPlayer;
    
    NNS_NULL_ASSERT( handle );
    
    if ( ! NNS_SndHandleIsValid( handle ) ) return FALSE;
    
    seqPlayer = handle->player;
    NNS_NULL_ASSERT( seqPlayer );
    
    return NNSi_SndReadDriverTrackInfo( seqPlayer->playerNo, trackNo, info );
}

/******************************************************************************
    private functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerInit

  Description:  �v���C���[���C�u�����̏�����

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndPlayerInit( void )
{
    NNSSndPlayer* player;
    int playerNo;
    
    NNS_FND_INIT_LIST( &sPrioList, NNSSndSeqPlayer, prioLink );
    NNS_FND_INIT_LIST( &sFreeList, NNSSndSeqPlayer, prioLink );
    
    for( playerNo = 0; playerNo < SND_PLAYER_NUM ; playerNo++ )
    {
        sSeqPlayer[ playerNo ].status = NNS_SND_SEQ_PLAYER_STATUS_STOP;
        sSeqPlayer[ playerNo ].playerNo = (u8)playerNo;
        NNS_FndAppendListObject( & sFreeList, & sSeqPlayer[ playerNo ] );
    }
    
    for( playerNo = 0; playerNo < NNS_SND_PLAYER_NUM ; playerNo++ )
    {
        player = &sPlayer[ playerNo ];
        
        NNS_FND_INIT_LIST( & player->playerList, NNSSndSeqPlayer, playerLink );
        NNS_FND_INIT_LIST( & player->heapList, NNSSndPlayerHeap, link );
        player->volume = 127;
        player->playableSeqCount = 1;
        player->allocChBitFlag = 0;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerMain

  Description:  �v���C���[���C�u�����̃t���[�����[�N

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndPlayerMain( void )
{
    NNSSndSeqPlayer* seqPlayer;
    NNSSndSeqPlayer* next;
    u32 status;
    int fader;
    
    // �v���C���[�X�e�[�^�X�̎擾
    status = SND_GetPlayerStatus();
    
    for( seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, NULL );
         seqPlayer != NULL ; seqPlayer = next )
    {
        next = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, seqPlayer );
        
        // �X�^�[�g�t���O�`�F�b�N
        if ( ! seqPlayer->startFlag ) {
            if ( SND_IsFinishedCommandTag( seqPlayer->commandTag ) )
            {
                seqPlayer->startFlag = TRUE;
            }
        }
        
        // ARM7������̒�~�`�F�b�N
        if ( seqPlayer->startFlag )
        {
            if ( ( status & ( 1 << seqPlayer->playerNo ) ) == 0 ) {
                ShutdownPlayer( seqPlayer );
                continue;
            }
        }
        
        // �t�F�[�_�[�X�V
        NNSi_SndFaderUpdate( & seqPlayer->fader );
        
        // �p�����[�^�̍X�V
        fader
            = SND_CalcDecibel( seqPlayer->initVolume )
            + SND_CalcDecibel( seqPlayer->extVolume )
            + SND_CalcDecibel( seqPlayer->player->volume )
            + SND_CalcDecibel( NNSi_SndFaderGet( & seqPlayer->fader ) >> FADER_SHIFT )
            ;
        if ( fader < -32768 ) fader = -32768;
        else if ( fader > 32767 ) fader = 32767;
        
        if ( fader != seqPlayer->volume )
        {
            SND_SetPlayerVolume( seqPlayer->playerNo, fader );
            seqPlayer->volume = (s16)fader;
        }
        
        // �t�F�[�h�A�E�g�����`�F�b�N
        if ( seqPlayer->status == NNS_SND_SEQ_PLAYER_STATUS_FADEOUT )
        {
            if ( NNSi_SndFaderIsFinished( & seqPlayer->fader ) )
            {
                ForceStopSeq( seqPlayer );
            }
        }
        
        // ���������t���O
        if ( seqPlayer->prepareFlag ) {
            SND_StartPreparedSeq( seqPlayer->playerNo );
            seqPlayer->prepareFlag = FALSE;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerAllocSeqPlayer

  Description:  �V�[�P���X�v���C���[���m��

  Arguments:    handle   - �T�E���h�n���h��
                playerNo - �v���C���[�ԍ�
                prio     - �v���C���[�v���C�I���e�B

  Returns:      �m�ۂ����V�[�P���X�v���C���[
                ���s����NULL
 *---------------------------------------------------------------------------*/
NNSSndSeqPlayer* NNSi_SndPlayerAllocSeqPlayer( NNSSndHandle* handle, int playerNo, int prio )
{
    NNSSndSeqPlayer* seqPlayer;
    NNSSndPlayer* player;

    NNS_NULL_ASSERT( handle );
    NNS_MINMAX_ASSERT( playerNo, 0, NNS_SND_PLAYER_NO_MAX );
    NNS_MINMAX_ASSERT( prio, 0, NNS_SND_PLAYER_PRIO_MAX );
    
    player = & sPlayer[ playerNo ];
    
    // �T�E���h�n���h���̐ؒf
    if ( NNS_SndHandleIsValid( handle ) ) {
        NNS_SndHandleReleaseSeq( handle );
    }
    
    // �v���C���[���̍ő哯���Đ����̃`�F�b�N
    if ( player->playerList.numObjects >= player->playableSeqCount )
    {
        // ��ԃv���C�I���e�B�̒Ⴂ���̂��擾
        seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & player->playerList, NULL );
        if ( seqPlayer == NULL ) return NULL;
        if ( prio < seqPlayer->prio ) return NULL;
        
        ForceStopSeq( seqPlayer );
    }
    
    // �m�ۏ���
    seqPlayer = AllocSeqPlayer( prio );
    if ( seqPlayer == NULL ) return NULL;
    
    // ������
    InsertPlayerList( player, seqPlayer );
    
    // �T�E���h�n���h���Ƃ̐ڑ�
    seqPlayer->handle = handle;
    handle->player = seqPlayer;
    
    return seqPlayer;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerFreeSeqPlayer

  Description:  �V�[�P���X�v���C���[�����

  Arguments:    seqPlayer - �V�[�P���X�v���C���[

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndPlayerFreeSeqPlayer( NNSSndSeqPlayer* seqPlayer )
{
    NNS_NULL_ASSERT( seqPlayer );

    ShutdownPlayer( seqPlayer );
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerStartSeq

  Description:  �V�[�P���X�J�n

  Arguments:    seqPlayer     - �V�[�P���X�v���C���[
                seqDataBase   - �V�[�P���X�f�[�^�x�[�X�A�h���X
                seqDataOffset - �V�[�P���X�f�[�^�̃I�t�Z�b�g
                bank          - �o���N�f�[�^

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndPlayerStartSeq(
    NNSSndSeqPlayer* seqPlayer,
    const void* seqDataBase,
    u32 seqDataOffset,
    const SNDBankData* bank
)
{
    NNSSndPlayer* player;
    
    NNS_NULL_ASSERT( seqPlayer );
    NNS_NULL_ASSERT( seqDataBase );
    NNS_NULL_ASSERT( bank );
    
    player = seqPlayer->player;
    NNS_NULL_ASSERT( player );
    
    SND_PrepareSeq(
        seqPlayer->playerNo,
        seqDataBase,
        seqDataOffset,
        bank
    );
    if ( player->allocChBitFlag ) {
        SND_SetTrackAllocatableChannel(
            seqPlayer->playerNo,
            0xffff,
            player->allocChBitFlag
        );
    }
    
    // ����������
    InitPlayer( seqPlayer );
    seqPlayer->commandTag = SND_GetCurrentCommandTag();
    seqPlayer->prepareFlag = TRUE;
    seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_PLAY;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerStopSeq

  Description:  �V�[�P���X��~

  Arguments:    seqPlayer - �V�[�P���X�v���C���[
                fadeFrame - �t�F�[�h�A�E�g�t���[��

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndPlayerStopSeq( NNSSndSeqPlayer* seqPlayer, int fadeFrame )
{
    if ( seqPlayer == NULL ) return;
    if ( seqPlayer->status == NNS_SND_SEQ_PLAYER_STATUS_STOP ) return;
    
    if ( fadeFrame == 0 )
    {
        ForceStopSeq( seqPlayer );
        return;
    }
    
    NNSi_SndFaderSet( & seqPlayer->fader, 0, fadeFrame );
    
    SetPlayerPriority( seqPlayer, 0 );
    
    seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_FADEOUT;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerPause

  Description:  �V�[�P���X�̈ꎞ��~�܂��͍ĊJ

  Arguments:    seqPlayer - �V�[�P���X�v���C���[
                flag      - �ꎞ��~�܂��͍ĊJ

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndPlayerPause( NNSSndSeqPlayer* seqPlayer, BOOL flag )
{
    if ( seqPlayer == NULL ) return;
    
    if ( flag != seqPlayer->pauseFlag )
    {
        SND_PauseSeq( seqPlayer->playerNo, flag );        
        seqPlayer->pauseFlag = (u8)flag;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndPlayerAllocHeap

  Description:  �v���C���[�q�[�v�̊m��

  Arguments:    playerNo - �v���C���[�ԍ�
                seqPlayer - �q�[�v���g�p����V�[�P���X�v���C���[

  Returns:      �m�ۂ����q�[�v
 *---------------------------------------------------------------------------*/
NNSSndHeapHandle NNSi_SndPlayerAllocHeap( int playerNo, NNSSndSeqPlayer* seqPlayer )
{
    NNSSndPlayer* player;
    NNSSndPlayerHeap* heap;
    
    NNS_MINMAX_ASSERT( playerNo, 0, NNS_SND_PLAYER_NO_MAX );
    
    player = & sPlayer[ playerNo ];
    
    heap = (NNSSndPlayerHeap*)NNS_FndGetNextListObject( & player->heapList, NULL );
    if ( heap == NULL ) return NULL;
    
    NNS_FndRemoveListObject( & player->heapList, heap );
    
    heap->player = seqPlayer;
    seqPlayer->heap = heap;
    
    NNS_SndHeapClear( heap->handle );
    
    return heap->handle;
}

/******************************************************************************
    static functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         InitPlayer

  Description:  �V�[�P���X�v���C���[�̏�����

  Arguments:    seqPlayer - �V�[�P���X�v���C���[

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitPlayer( NNSSndSeqPlayer* seqPlayer )
{
    NNS_NULL_ASSERT( seqPlayer );
    
    seqPlayer->pauseFlag = FALSE;
    seqPlayer->startFlag = FALSE;
    seqPlayer->prepareFlag = FALSE;
    
    seqPlayer->seqType = NNS_SND_PLAYER_SEQ_TYPE_INVALID;
    
    seqPlayer->volume = 0;
    
    seqPlayer->initVolume = 127;
    seqPlayer->extVolume = 127;
    
    NNSi_SndFaderInit( & seqPlayer->fader );
    NNSi_SndFaderSet( & seqPlayer->fader, 127 << FADER_SHIFT, 1 );
}

/*---------------------------------------------------------------------------*
  Name:         InsertPlayerList

  Description:  �v���C���[���X�g�ւ̒ǉ�

  Arguments:    player    - �v���C���[
                seqPlayer - �V�[�P���X�v���C���[

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InsertPlayerList( NNSSndPlayer* player, NNSSndSeqPlayer* seqPlayer )
{
    NNSSndSeqPlayer* next = NULL;
    while ( ( next = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & player->playerList, next ) ) != NULL )
    {
        if ( seqPlayer->prio < next->prio ) break;
    }
    
    NNS_FndInsertListObject( & player->playerList, next, seqPlayer );
    
    seqPlayer->player = player;
}

/*---------------------------------------------------------------------------*
  Name:         InsertPrioList

  Description:  �v���C�I���e�B���X�g�ւ̒ǉ�

  Arguments:    seqPlayer - �V�[�P���X�v���C���[

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InsertPrioList( NNSSndSeqPlayer* seqPlayer )
{
    NNSSndSeqPlayer* next = NULL;
    while ( ( next = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, next ) ) != NULL )
    {
        if ( seqPlayer->prio < next->prio ) break;
    }

    NNS_FndInsertListObject( & sPrioList, next, seqPlayer );
}

/*---------------------------------------------------------------------------*
  Name:         ForceStopSeq

  Description:  �V�[�P���X�̋�����~

  Arguments:    seqPlayer - �V�[�P���X�v���C���[

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ForceStopSeq( NNSSndSeqPlayer* seqPlayer )
{
    if ( seqPlayer->status == NNS_SND_SEQ_PLAYER_STATUS_FADEOUT ) {
        SND_SetPlayerVolume( seqPlayer->playerNo, SND_VOLUME_DB_MIN );
    }
    SND_StopSeq( seqPlayer->playerNo );
    ShutdownPlayer( seqPlayer );
}

/*---------------------------------------------------------------------------*
  Name:         AllocSeqPlayer

  Description:  �V�[�P���X�v���C���[�̊m��

  Arguments:    prio - �v���C���[�v���C�I���e�B

  Returns:      �m�ۂ����V�[�P���X�v���C���[
                ���s����NULL
 *---------------------------------------------------------------------------*/
static NNSSndSeqPlayer* AllocSeqPlayer( int prio )
{
    NNSSndSeqPlayer* seqPlayer;
    
    NNS_MINMAX_ASSERT( prio, 0, NNS_SND_PLAYER_PRIO_MAX );
    
    seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sFreeList, NULL );
    if ( seqPlayer == NULL )
    {
        // get lowest priority player
        seqPlayer = (NNSSndSeqPlayer*)NNS_FndGetNextListObject( & sPrioList, NULL );        
        NNS_NULL_ASSERT( seqPlayer );
        
        if ( prio < seqPlayer->prio ) return NULL;
        
        ForceStopSeq( seqPlayer );
    }
    NNS_FndRemoveListObject( & sFreeList, seqPlayer );
    
    seqPlayer->prio = (u8)prio;
    
    InsertPrioList( seqPlayer );
    
    return seqPlayer;
}

/*---------------------------------------------------------------------------*
  Name:         ShutdownPlayer

  Description:  �V�[�P���X��~�̌�n��

  Arguments:    seqPlayer - �V�[�P���X�v���C���[

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ShutdownPlayer( NNSSndSeqPlayer* seqPlayer )
{
    NNSSndPlayer* player;
    
    NNS_NULL_ASSERT( seqPlayer );
    
    // �T�E���h�n���h���Ƃ̐ؒf
    if ( seqPlayer->handle != NULL ) {
        NNS_ASSERT( seqPlayer == seqPlayer->handle->player );
        seqPlayer->handle->player = NULL;
        seqPlayer->handle = NULL;
    }
    
    // �v���C���[�Ƃ̐ؒf
    player = seqPlayer->player;
    NNS_NULL_ASSERT( player );
    NNS_FndRemoveListObject( & player->playerList, seqPlayer );        
    seqPlayer->player = NULL;
    
    // �q�[�v�̉��
    if ( seqPlayer->heap != NULL ) {
        NNS_FndAppendListObject( & player->heapList, seqPlayer->heap );
        seqPlayer->heap->player = NULL;
        seqPlayer->heap = NULL;
    }
    
    // �t���[���X�g�ֈړ�
    NNS_FndRemoveListObject( &sPrioList, seqPlayer );
    NNS_FndAppendListObject( &sFreeList, seqPlayer );
    
    seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_STOP;
}

/*---------------------------------------------------------------------------*
  Name:         PlayerHeapDisposeCallback

  Description:  �v���C���[�q�[�v�j�����̏���

  Arguments:    mem   - �������u���b�N�̐擪�A�h���X
                size  - �������u���b�N�̃T�C�Y
                data1 - ���[�U�[�f�[�^�i���g�p�j
                data2 - ���[�U�[�f�[�^�i���g�p�j

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PlayerHeapDisposeCallback( void* mem, u32 /*size*/, u32 /*data1*/, u32 /*data2*/ )
{
    NNSSndPlayerHeap* heap = (NNSSndPlayerHeap*)mem;
    NNSSndSeqPlayer* seqPlayer;
    
    if ( heap->handle == NNS_SND_HEAP_INVALID_HANDLE ) return;
    
    // �q�[�v�N���A
    NNS_SndHeapDestroy( heap->handle );
    
    seqPlayer = heap->player;
    if ( seqPlayer != NULL ) {
        // �g�p���́A�v���C���[�Ɛؒf
        seqPlayer->heap = NULL;
    }
    else {
        // ���g�p���ɂ́A�q�[�v���X�g����폜
        NNS_FndRemoveListObject( &sPlayer[ heap->playerNo ].heapList, heap );
    }
}

/*---------------------------------------------------------------------------*
  Name:         SetPlayerPriority

  Description:  �v���C���[�v���C�I���e�B��ύX

  Arguments:    seqPlayer - �V�[�P���X�v���C���[
                priority  - �v���C���[�v���C�I���e�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void SetPlayerPriority( NNSSndSeqPlayer* seqPlayer, int priority )
{
    NNSSndPlayer* player;
    
    NNS_NULL_ASSERT( seqPlayer );
    NNS_MINMAX_ASSERT( priority, 0, NNS_SND_PLAYER_PRIO_MAX );
    
    player = seqPlayer->player;
    if ( player != NULL ) {
        NNS_FndRemoveListObject( & player->playerList, seqPlayer );
        seqPlayer->player = NULL;
    }
    NNS_FndRemoveListObject( & sPrioList, seqPlayer );
    
    seqPlayer->prio = (u8)priority;
    
    if ( player != NULL ) {
        InsertPlayerList( player, seqPlayer );
    }
    InsertPrioList( seqPlayer );
}


/*====== End of player.c ======*/

