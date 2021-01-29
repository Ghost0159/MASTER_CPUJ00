/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SND - libraries
  File:     snd_work.c

  Copyright 2004-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: snd_work.c,v $
  Revision 1.3  2006/01/18 02:12:29  kitase_hirotake
  do-indent

  Revision 1.2  2005/08/18 04:33:45  ida
  �L���b�V�������������̍œK��

  Revision 1.1  2005/04/12 06:16:12  ida
  SND��NITRO-SDK�ֈڐ�

  Revision 1.13  2005/02/28 04:12:11  ida
  prgNo�����o�ǉ�

  Revision 1.12  2005/02/17 07:50:51  ida
  �֐���Get��Read�֕ύX

  Revision 1.11  2005/02/16 07:48:10  ida
  �h���C�o���擾�֐��ǉ�

  Revision 1.10  2004/08/02 05:32:15  ida
  add capture status

  Revision 1.9  2004/07/16 06:42:49  ida
  fix Project name

  Revision 1.8  2004/07/16 04:18:52  ida
  invalidate range in every time

  Revision 1.7  2004/07/09 06:52:06  ida
  add flush shared work

  Revision 1.6  2004/07/05 06:31:54  ida
  workaround shared work

  Revision 1.5  2004/06/23 06:29:51  ida
  workaround shared work

  Revision 1.4  2004/06/21 01:16:24  ida
  change function interface

  Revision 1.3  2004/05/10 05:29:29  ida
  add tickCounter

  Revision 1.2  2004/05/10 04:58:47  ida
  add SND_SetPlayerVariable & SND_SetGlobalVariable

  Revision 1.1  2004/05/06 04:48:20  ida
  snd_work.c : ARM7/src -> commom/src

  Revision 1.2  2004/05/06 00:44:45  ida
  add SNDi_SharedWork

  Revision 1.1  2004/03/26 06:25:52  ida
  create snd_drv

  Revision 1.2  2004/03/24 06:25:37  ida
  SNDi_Snd -> SNDi_

  Revision 1.1  2004/03/08 05:40:27  ida
  rename files

  Revision 1.2  2004/02/19 07:19:44  ida
  NNS_ -> SND_

  Revision 1.1  2004/02/19 07:07:52  ida
  add NITRO System snd libraries

  Revision 1.1  2004/02/13 01:42:05  ida
  initial version

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro/snd/common/work.h>

#ifndef SDK_FROM_TOOL

#else

#define REG_SND_SOUND0CNT_E_SHIFT                          31
#define REG_SND_SOUND0CNT_E_SIZE                           1
#define REG_SND_SOUND0CNT_E_MASK                           0x80000000

#define REG_SND_SOUND0CNT_FORMAT_SHIFT                     29
#define REG_SND_SOUND0CNT_FORMAT_SIZE                      2
#define REG_SND_SOUND0CNT_FORMAT_MASK                      0x60000000

#define REG_SND_SOUND0CNT_REPEAT_SHIFT                     27
#define REG_SND_SOUND0CNT_REPEAT_SIZE                      2
#define REG_SND_SOUND0CNT_REPEAT_MASK                      0x18000000

#define REG_SND_SOUND0CNT_DUTY_SHIFT                       24
#define REG_SND_SOUND0CNT_DUTY_SIZE                        3
#define REG_SND_SOUND0CNT_DUTY_MASK                        0x07000000

#define REG_SND_SOUND0CNT_PAN_SHIFT                        16
#define REG_SND_SOUND0CNT_PAN_SIZE                         7
#define REG_SND_SOUND0CNT_PAN_MASK                         0x007f0000

#define REG_SND_SOUND0CNT_HOLD_SHIFT                       15
#define REG_SND_SOUND0CNT_HOLD_SIZE                        1
#define REG_SND_SOUND0CNT_HOLD_MASK                        0x00008000

#define REG_SND_SOUND0CNT_SHIFT_SHIFT                      8
#define REG_SND_SOUND0CNT_SHIFT_SIZE                       2
#define REG_SND_SOUND0CNT_SHIFT_MASK                       0x00000300

#define REG_SND_SOUND0CNT_VOLUME_SHIFT                     0
#define REG_SND_SOUND0CNT_VOLUME_SIZE                      7
#define REG_SND_SOUND0CNT_VOLUME_MASK                      0x0000007f

#define SDK_ASSERT(exp)           ((void) 0)
#define SDK_NULL_ASSERT(exp)           ((void) 0)
#define SDK_MINMAX_ASSERT(exp, min, max)           ((void) 0)

#endif

/******************************************************************************
	private variable
 ******************************************************************************/

SNDSharedWork *SNDi_SharedWork;

#ifdef SDK_ARM7

SNDWork SNDi_Work;

#endif /* SDK_ARM7 */

/******************************************************************************
	static function declaration
 ******************************************************************************/
static const void *ConvAddr(const void *p, const void *baseFrom, const void *baseTo);

/******************************************************************************
	inline function
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         ConvAddr

  Description:  �A�h���X�̕ϊ����s���܂��B

  Arguments:    p - �ϊ����錳�̃A�h���X
                baseFrom - ���̃A�h���X�̋N�_�A�h���X
                baseTo - �ϊ���̃A�h���X�̋N�_�A�h���X

  Returns:      �ϊ���̃A�h���X
 *---------------------------------------------------------------------------*/
#ifndef _MSC_VER
static  inline
#else
__inline
#endif
const void *ConvAddr(const void *p, const void *baseFrom, const void *baseTo)
{
    if (p == NULL)
        return NULL;
    return (const char *)baseTo + ((const char *)p - (const char *)baseFrom);
}

/******************************************************************************
	public function
 ******************************************************************************/

/*---------------------------------------------------------------------------*
  Name:         SND_GetPlayerStatus

  Description:  �v���C���[�̃X�e�[�^�X���擾���܂��B

  Arguments:    None.

  Returns:      �v���C���[�X�e�[�^�X�̃r�b�g�}�X�N
                �r�b�g�������Ă���ƁA�A�N�e�B�u
 *---------------------------------------------------------------------------*/
u32 SND_GetPlayerStatus(void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->playerStatus,
                       sizeof(SNDi_SharedWork->playerStatus));
#endif

    return SNDi_SharedWork->playerStatus;
}

/*---------------------------------------------------------------------------*
  Name:         SND_GetChannelStatus

  Description:  �`�����l���̃X�e�[�^�X���擾���܂��B

  Arguments:    None.

  Returns:      �`�����l���X�e�[�^�X�̃r�b�g�}�X�N
                �r�b�g�������Ă���ƁA�A�N�e�B�u
 *---------------------------------------------------------------------------*/
u32 SND_GetChannelStatus(void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->channelStatus,
                       sizeof(SNDi_SharedWork->channelStatus));
#endif

    return SNDi_SharedWork->channelStatus;
}

/*---------------------------------------------------------------------------*
  Name:         SND_GetCaptureStatus

  Description:  �L���v�`���̃X�e�[�^�X���擾���܂��B

  Arguments:    None.

  Returns:      �L���v�`���X�e�[�^�X�̃r�b�g�}�X�N
                �r�b�g�������Ă���ƁA�A�N�e�B�u
 *---------------------------------------------------------------------------*/
u32 SND_GetCaptureStatus(void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->captureStatus,
                       sizeof(SNDi_SharedWork->captureStatus));
#endif

    return SNDi_SharedWork->captureStatus;
}

/*---------------------------------------------------------------------------*
  Name:         SND_GetPlayerTickCounter

  Description:  �v���C���[�̃`�b�N�J�E���^���擾���܂��B

  Arguments:    playerNo - �v���C���[�ԍ�

  Returns:      �`�b�N�J�E���^�̒l
 *---------------------------------------------------------------------------*/
u32 SND_GetPlayerTickCounter(int playerNo)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);
    SDK_MINMAX_ASSERT(playerNo, SND_PLAYER_MIN, SND_PLAYER_MAX);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->player[playerNo].tickCounter,
                       sizeof(SNDi_SharedWork->player[playerNo].tickCounter));
#endif

    return SNDi_SharedWork->player[playerNo].tickCounter;
}

/*---------------------------------------------------------------------------*
  Name:         SND_GetPlayerLocalVariable

  Description:  �V�[�P���X���[�J���ϐ����擾���܂��B

  Arguments:    playerNo - �v���C���[�ԍ�
                varNo - �ϐ��ԍ�

  Returns:      �ϐ��̒l
 *---------------------------------------------------------------------------*/
s16 SND_GetPlayerLocalVariable(int playerNo, int varNo)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);
    SDK_MINMAX_ASSERT(playerNo, SND_PLAYER_MIN, SND_PLAYER_MAX);
    SDK_MINMAX_ASSERT(varNo, 0, SND_PLAYER_VARIABLE_NUM - 1);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->player[playerNo].variable[varNo],
                       sizeof(SNDi_SharedWork->player[playerNo].variable[varNo]));
#endif

    return SNDi_SharedWork->player[playerNo].variable[varNo];
}

/*---------------------------------------------------------------------------*
  Name:         SND_GetPlayerGlobalVariable

  Description:  �V�[�P���X�O���[�o���ϐ����擾���܂��B

  Arguments:    varNo - �ϐ��ԍ�

  Returns:      �ϐ��̒l
 *---------------------------------------------------------------------------*/
s16 SND_GetPlayerGlobalVariable(int varNo)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);
    SDK_MINMAX_ASSERT(varNo, 0, SND_GLOBAL_VARIABLE_NUM - 1);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->globalVariable[varNo],
                       sizeof(SNDi_SharedWork->globalVariable[varNo]));
#endif

    return SNDi_SharedWork->globalVariable[varNo];
}

/*---------------------------------------------------------------------------*
  Name:         SND_ReadChannelInfo

  Description:  �`�����l������ǂ݂Ƃ�܂��B

  Arguments:    driverInfo - �h���C�o���\����
                chNo - �`�����l���ԍ�
                chInfo - �擾���������i�[����`�����l�����\����

  Returns:      �ǂ݂Ƃ�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL SND_ReadChannelInfo(const SNDDriverInfo * driverInfo, int chNo, SNDChannelInfo * chInfo)
{
    u32     lockedChannel;

    SDK_NULL_ASSERT(driverInfo);
    SDK_NULL_ASSERT(chInfo);

    if (chNo < SND_CHANNEL_MIN || SND_CHANNEL_MAX < chNo)
        return FALSE;

    lockedChannel = driverInfo->lockedChannels;
    chInfo->lockFlag = (lockedChannel & (1 << chNo)) ? TRUE : FALSE;

    if (chInfo->lockFlag)
    {
        const u32 chCtrl = driverInfo->chCtrl[chNo];

        chInfo->activeFlag = (chCtrl & REG_SND_SOUND0CNT_E_MASK) ? TRUE : FALSE;

        {
            const u8 volume =
                (u8)((chCtrl & REG_SND_SOUND0CNT_VOLUME_MASK) >> REG_SND_SOUND0CNT_VOLUME_SHIFT);
            const SNDChannelDataShift shift =
                (SNDChannelDataShift)((chCtrl & REG_SND_SOUND0CNT_SHIFT_MASK) >>
                                      REG_SND_SOUND0CNT_SHIFT_SHIFT);

            chInfo->volume = volume;
            chInfo->volume <<= 4;
            switch (shift)
            {
            case SND_CHANNEL_DATASHIFT_NONE:
                break;
            case SND_CHANNEL_DATASHIFT_1BIT:
                chInfo->volume >>= 1;
                break;
            case SND_CHANNEL_DATASHIFT_2BIT:
                chInfo->volume >>= 2;
                break;
            case SND_CHANNEL_DATASHIFT_4BIT:
                chInfo->volume >>= 4;
                break;
            }
        }

        chInfo->pan = (u8)((chCtrl & REG_SND_SOUND0CNT_PAN_MASK) >> REG_SND_SOUND0CNT_PAN_SHIFT);
    }
    else
    {
        const SNDExChannel *chp = &driverInfo->work.channel[chNo];
        SDK_NULL_ASSERT(chp);

        chInfo->activeFlag = chp->active_flag;
        chInfo->envStatus = (SNDEnvStatus)(chp->env_status);

        {
            const u8 volume = (u8)(chp->volume & 0xff);
            const SNDChannelDataShift shift = (SNDChannelDataShift)(chp->volume >> 8);

            chInfo->volume = volume;
            chInfo->volume <<= 4;
            switch (shift)
            {
            case SND_CHANNEL_DATASHIFT_NONE:
                break;
            case SND_CHANNEL_DATASHIFT_1BIT:
                chInfo->volume >>= 1;
                break;
            case SND_CHANNEL_DATASHIFT_2BIT:
                chInfo->volume >>= 2;
                break;
            case SND_CHANNEL_DATASHIFT_4BIT:
                chInfo->volume >>= 4;
                break;
            }
        }

        chInfo->pan = chp->pan;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SND_ReadPlayerInfo

  Description:  �v���C���[����ǂ݂Ƃ�܂��B

  Arguments:    driverInfo - �h���C�o���\����
                playerNo - �v���C���[�ԍ�
                playerInfo - �擾���������i�[����v���C���[���\����

  Returns:      �ǂ݂Ƃ�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL SND_ReadPlayerInfo(const SNDDriverInfo * driverInfo, int playerNo, SNDPlayerInfo * playerInfo)
{
    const SNDPlayer *player;
    int     trackNo;

    SDK_NULL_ASSERT(driverInfo);
    SDK_NULL_ASSERT(playerInfo);

    if (playerNo < SND_PLAYER_MIN || SND_PLAYER_MAX < playerNo)
        return FALSE;

    player = &driverInfo->work.player[playerNo];
    SDK_NULL_ASSERT(player);

    playerInfo->trackBitMask = 0;
    for (trackNo = 0; trackNo < SND_TRACK_NUM_PER_PLAYER; trackNo++)
    {
        if (player->tracks[trackNo] != SND_INVALID_TRACK_INDEX)
        {
            playerInfo->trackBitMask |= (1 << trackNo);
        }
    }

    playerInfo->activeFlag = player->active_flag;
    playerInfo->pauseFlag = player->pause_flag;
    playerInfo->tempo = player->tempo;
    playerInfo->volume = player->volume;

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         SND_ReadTrackInfo

  Description:  �g���b�N����ǂ݂Ƃ�܂��B

  Arguments:    driverInfo - �h���C�o���\����
                playerNo - �v���C���[�ԍ�
                trackNo - �g���b�N�ԍ�
                trackInfo - �擾���������i�[����g���b�N���\����

  Returns:      �ǂ݂Ƃ�ɐ����������ǂ���
 *---------------------------------------------------------------------------*/
BOOL SND_ReadTrackInfo(const SNDDriverInfo * driverInfo, int playerNo, int trackNo,
                       SNDTrackInfo * trackInfo)
{
    const SNDPlayer *player;
    const SNDTrack *track;
    u8      trackID;

    SDK_NULL_ASSERT(driverInfo);
    SDK_NULL_ASSERT(trackInfo);

    if (playerNo < SND_PLAYER_MIN || SND_PLAYER_MAX < playerNo)
        return FALSE;
    if (trackNo < 0 || SND_TRACK_NUM_PER_PLAYER - 1 < trackNo)
        return FALSE;

    player = &driverInfo->work.player[playerNo];
    SDK_NULL_ASSERT(player);

    trackID = player->tracks[trackNo];
    if (trackID == SND_INVALID_TRACK_INDEX)
        return FALSE;

    track = &driverInfo->work.track[trackID];
    SDK_NULL_ASSERT(track);

    trackInfo->prgNo = track->prgNo;
    trackInfo->volume = track->volume;
    trackInfo->volume2 = track->volume2;
    trackInfo->pitchBend = track->pitch_bend;
    trackInfo->bendRange = track->bend_range;
    trackInfo->pan = (u8)(track->pan + SND_CHANNEL_PAN_CENTER);
    trackInfo->transpose = track->transpose;

    {
        const SNDExChannel *chp =
            (const SNDExChannel *)ConvAddr(track->channel_list, driverInfo->workAddress,
                                           &driverInfo->work);

        trackInfo->chCount = 0;
        while (chp != NULL)
        {
            trackInfo->channel[trackInfo->chCount] = chp->myNo;
            trackInfo->chCount++;

            chp =
                (const SNDExChannel *)ConvAddr(chp->nextLink, driverInfo->workAddress,
                                               &driverInfo->work);
        }
    }

    return TRUE;
}

#ifdef SDK_ARM7

void SND_SetPlayerLocalVariable(int playerNo, int varNo, s16 var)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);
    SDK_MINMAX_ASSERT(playerNo, SND_PLAYER_MIN, SND_PLAYER_MAX);
    SDK_MINMAX_ASSERT(varNo, 0, SND_PLAYER_VARIABLE_NUM - 1);

    SNDi_SharedWork->player[playerNo].variable[varNo] = var;
}

void SND_SetPlayerGlobalVariable(int varNo, s16 var)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);
    SDK_MINMAX_ASSERT(varNo, 0, SND_GLOBAL_VARIABLE_NUM - 1);

    SNDi_SharedWork->globalVariable[varNo] = var;
}

void SND_UpdateSharedWork(void)
{
    u16     channelStatus = 0;
    u16     captureStatus = 0;
    int     chNo;

    if (SNDi_SharedWork == NULL)
        return;

    for (chNo = 0; chNo < SND_CHANNEL_NUM; chNo++)
    {
        if (SND_IsChannelActive(chNo))
        {
            channelStatus |= (1 << chNo);
        }
    }

    if (SND_IsCaptureActive(SND_CAPTURE_0))
        captureStatus |= (1 << 0);
    if (SND_IsCaptureActive(SND_CAPTURE_1))
        captureStatus |= (1 << 1);

    SNDi_SharedWork->channelStatus = channelStatus;
    SNDi_SharedWork->captureStatus = captureStatus;
}

#endif /* SDK_ARM7 */

/******************************************************************************
	private function
 ******************************************************************************/

u32 SNDi_GetFinishedCommandTag(void)
{
    SDK_NULL_ASSERT(SNDi_SharedWork);

#ifdef SDK_ARM9
    DC_InvalidateRange((void *)&SNDi_SharedWork->finishCommandTag,
                       sizeof(SNDi_SharedWork->finishCommandTag));
#endif

    return SNDi_SharedWork->finishCommandTag;
}

#ifdef SDK_ARM9

void SNDi_InitSharedWork(SNDSharedWork *work)
{
    int     playerNo, variableNo;

    work->playerStatus = 0;
    work->channelStatus = 0;
    work->captureStatus = 0;
    work->finishCommandTag = 0;

    for (playerNo = 0; playerNo < SND_PLAYER_NUM; playerNo++)
    {
        work->player[playerNo].tickCounter = 0;
        for (variableNo = 0; variableNo < SND_PLAYER_VARIABLE_NUM; variableNo++)
        {
            work->player[playerNo].variable[variableNo] = SND_DEFAULT_VARIABLE;
        }
    }
    for (variableNo = 0; variableNo < SND_GLOBAL_VARIABLE_NUM; variableNo++)
    {
        work->globalVariable[variableNo] = SND_DEFAULT_VARIABLE;
    }

    DC_FlushRange(work, sizeof(SNDSharedWork));
}

#endif /* SDK_ARM9 */

/*====== End of work.c ======*/
