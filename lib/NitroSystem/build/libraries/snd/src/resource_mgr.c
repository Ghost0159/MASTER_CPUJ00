/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - snd
  File:     resource_mgr.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/
#include <nnsys/snd/resource_mgr.h>

#include <nitro/snd.h>
#include <nnsys/misc.h>

/******************************************************************************
	static variable
 ******************************************************************************/

static u32 sChannelLock;
static u32 sCaptureLock;
static u32 sAlarmLock;

/******************************************************************************
	public functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNS_SndLockChannel

  Description:  �`�����l���̃��b�N

  Arguments:    chBitFlag - �`�����l���r�b�g�t���O

  Returns:      ���b�N�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndLockChannel( u32 chBitFlag )
{
    if ( chBitFlag == 0 ) return TRUE;
    
    if ( chBitFlag & sChannelLock ) return FALSE;
    
    SND_LockChannel( chBitFlag, 0 );
    
    sChannelLock |= chBitFlag;
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndUnlockChannel

  Description:  �`�����l���̃A�����b�N

  Arguments:    chBitFlag - �`�����l���r�b�g�t���O

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndUnlockChannel( u32 chBitFlag )
{
    NNS_ASSERT( ( chBitFlag & sChannelLock ) == chBitFlag );

    if ( chBitFlag == 0 ) return;
    
    SND_UnlockChannel( chBitFlag, 0 );
    
    sChannelLock &= ~chBitFlag;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndLockCapture

  Description:  �L���v�`���[�̃��b�N

  Arguments:    capBitFlag - �L���v�`���[�r�b�g�t���O

  Returns:      ���b�N�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL NNS_SndLockCapture( u32 capBitFlag )
{
    if ( capBitFlag & sCaptureLock ) return FALSE;
    
    sCaptureLock |= capBitFlag;
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndUnlockCapture

  Description:  �L���v�`���[�̃A�����b�N

  Arguments:    capBitFlag - �L���v�`���[�r�b�g�t���O

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndUnlockCapture( u32 capBitFlag )
{
    NNS_ASSERT( ( capBitFlag & sCaptureLock ) == capBitFlag );
    
    sCaptureLock &= ~capBitFlag;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndAllocAlarm

  Description:  �A���[���̊m��

  Arguments:    None.

  Returns:      �m�ۂɐ���������A�A���[���ԍ���Ԃ�
                �m�ۂɎ��s������A-1��Ԃ�
 *---------------------------------------------------------------------------*/
int NNS_SndAllocAlarm( void )
{
    int alarmNo;
    u32 mask = 1;
    
    for( alarmNo = 0; alarmNo < SND_ALARM_NUM ; alarmNo++, mask <<= 1 )
    {
        if ( ( sAlarmLock & mask ) == 0 ) {
            sAlarmLock |= mask;
            return alarmNo;
        }
    }
    
    return -1;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_SndFreeAlarm

  Description:  �A���[���̉��

  Arguments:    alarmNo - �A���[���ԍ�

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNS_SndFreeAlarm( int alarmNo )
{
    NNS_MINMAX_ASSERT( alarmNo, SND_ALARM_MIN, SND_ALARM_MAX );
    NNS_ASSERT( sAlarmLock & ( 1 << alarmNo ) );
    
    sAlarmLock &= ~( 1 << alarmNo );
}

/******************************************************************************
	private functions
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         NNSi_SndInitResourceMgr

  Description:  ���\�[�X�}�l�[�W���̏�����

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NNSi_SndInitResourceMgr( void )
{
    sChannelLock = 0;
    sCaptureLock = 0;
    sAlarmLock   = 0;
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_GetLockedChannel

  Description:  ���b�N�ς݃`�����l���ԍ��̎擾

  Arguments:    None.

  Returns:      ���b�N�ς݃`�����l���̃r�b�g�t���O
 *---------------------------------------------------------------------------*/
u32 NNSi_GetLockedChannel( void )
{
    return sChannelLock;
}

/*====== End of resource_mgr.c ======*/

