/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./src/lobby/lobby.c

  Copyright 2005-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
/**
 * @file
 *
 * @brief ���r�[���C�u�����̃��b�p�[���[�e�B���e�B�[ �\�[�X
 */

#include <lobby/dwci_lobby.h>
#include "dwci_lobbyBase.h"
#include "dwci_lobbyUtil.h"
#include "ppw_data.h"
#include "ppw_parser.h"
#include "ppw_innerInfo.h"

PPW_LobbyInnerInfoManager::PPW_LobbyInnerInfoManager()
    : state(NOTINITIALIZED)
    , isTimeDataSet(FALSE)
    , timeData()
    , lockedTime(PPW_LOBBY_INVALID_TIME)
    , bLockReserved(FALSE)
    , roomType()
    , season()
    , lockTime()
    , random()
    , scheduleRecords()
    , scheduleRecordsRemain()
{
}

BOOL PPW_LobbyInnerInfoManager::GetLobbyInfo(PPW_LobbySchedule* info, u32& size) const
{
    u32 requireSize = sizeof(PPW_LobbySchedule) + sizeof(PPW_LobbyScheduleRecord) * (scheduleRecords.size() - 1);
    
    u32 orgSize = size;
    size = requireSize;
    if(orgSize < requireSize || !info)
    {
        return FALSE;
    }
    info->lockTime = lockTime;
    info->random = random;
    info->roomFlag = roomFlag;
    info->roomType = roomType;
    info->season = season;
    info->scheduleRecordNum = (u16)scheduleRecords.size();
    if(info->scheduleRecordNum != 0)
    {
        DWCi_Np_CpuCopy8(DWCi_GetVectorBuffer(scheduleRecords), info->scheduleRecords, sizeof(ScheduleRecordContainer::value_type) * scheduleRecords.size());
    }
    return TRUE;
}

BOOL PPW_LobbyInnerInfoManager::GetVipRecords(PPW_LobbyVipRecord* records, u32& num) const
{
    u32 orgSize = num;
    num = vipRecords.size();
    if(orgSize < num || !records)
    {
        // �K�v���̂ݒʒm
        return FALSE;
    }
    
    if(num == 0)
    {
        return TRUE;
    }
    DWCi_Np_CpuCopy8(DWCi_GetVectorBuffer(vipRecords), records, sizeof(VipRecordContainer::value_type) * num);
    
    return TRUE;
}

std::vector<u8, DWCi_Allocator<u8> > PPW_LobbyInnerInfoManager::Serialize() const
{
    u32 size = 0;
    GetLobbyInfo(NULL, size);
    std::vector<u8, DWCi_Allocator<u8> > buf(size);
    GetLobbyInfo((PPW_LobbySchedule*)DWCi_GetVectorBuffer(buf), size);
    return buf;
}

BOOL PPW_LobbyInnerInfoManager::SetLobbyInfo(const u8* buf, u32 size)
{
    if(!CheckLobbyInfo(buf, size))
    {
        return FALSE;
    }
    PPW_LobbySchedule* info = (PPW_LobbySchedule*)buf;
    
    roomType = info->roomType;
    season = info->season;
    lockTime = info->lockTime;
    random = info->random;
    scheduleRecords.assign(info->scheduleRecords, info->scheduleRecords + info->scheduleRecordNum);
    std::stable_sort(scheduleRecords.begin(), scheduleRecords.end(), LessScheduleRecord());
    // �c��C�x���g�L���[��������
    scheduleRecordsRemain.assign(scheduleRecords.begin(), scheduleRecords.end());
    return TRUE;
}

// ���̊֐��Ń`�F�b�N��ʂ����f�[�^��SetLobbyInfo�֐��ŕK��TRUE��Ԃ�
BOOL PPW_LobbyInnerInfoManager::CheckLobbyInfo(const u8* buf, u32 size)
{
    if(size < sizeof(PPW_LobbySchedule) - sizeof(PPW_LobbyScheduleRecord))
    {
        return FALSE;
    }
    
    PPW_LobbySchedule* info = (PPW_LobbySchedule*)buf;
    
    if(size < sizeof(PPW_LobbySchedule) + sizeof(PPW_LobbyScheduleRecord) * (info->scheduleRecordNum - 1))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL PPW_LobbyInnerInfoManager::SetVipRecord(const u8* buf, u32 size)
{
    CheckVipRecord(buf, size);
    
    u32 num = size / sizeof(PPW_LobbyVipRecord);
    
    PPW_LobbyVipRecord* tempRecords = (PPW_LobbyVipRecord*)buf;
    vipRecords.assign(tempRecords, tempRecords + num);
    return TRUE;
}

BOOL PPW_LobbyInnerInfoManager::CheckVipRecord(const u8* buf, u32 size)
{
    if(size % sizeof(PPW_LobbyVipRecord) != 0)
    {
        return FALSE;
    }
    
    u32 num = size / sizeof(PPW_LobbyVipRecord);
    
    if(size == 0)
    {
        vipRecords.clear();
        return TRUE;
    }
    (void)buf;
    return TRUE;
}

// �A���P�[�g�����擾����B�܂��擾���Ă��Ȃ�������FALSE��Ԃ�
BOOL PPW_LobbyInnerInfoManager::GetQuestionnaire(PPW_LobbyQuestionnaire* questionnaire_)
{
    if(!bSetQuestionnaire)
    {
        return FALSE;
    }
    
    DWCi_Np_CpuCopy8(&questionnaire, questionnaire_, sizeof(questionnaire));
    return TRUE;
}

void PPW_LobbyInnerInfoManager::SetQuestionnaire(const PPW_LobbyQuestionnaire* questionnaire_)
{
    DWCi_Np_CpuCopy8(questionnaire_, &questionnaire, sizeof(questionnaire));
    bSetQuestionnaire = TRUE;
}

void PPW_LobbyInnerInfoManager::Process()
{    
    switch(state)
    {
    case NOTINITIALIZED:
        break;
    case DOWNLOADING:
        break;
    case DOWNLOADED:
        break;
    case SHARING:
        break;
    case SHARED:
        if(NeedLock())
        {
            DWCi_LobbySetChannelMode(PPW_GetLobby()->GetMainChannel().GetName().c_str(), &PPW_LOBBY_CHANNEL_MODE_MAIN_LOCK);
            
            // �݂�Ȃɒʒm����
            PPW_LobbyDummyData dummyData;
            PPW_LobbySendMessage(PPW_LOBBY_INVALID_USER_ID, PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LobbyMessage::FORMAT_BASE64, PPW_LobbyMessage::TARGET_SYSTEM,
                                 PPW_LOBBY_MESSAGE_TYPE_LOCK, (u8*)&dummyData, sizeof(dummyData));
            
            // �����I�Ƀ��b�N����
            Lock();
        }
        break;
    case LOCKED:
    {
        // �C�x���g�̎��Ԃ��`�F�b�N����
        if(scheduleRecordsRemain.empty())
        {
            break;
        }
        PPW_LobbyScheduleRecord& record = scheduleRecordsRemain.front();
        s64 currentTime = DWCi_Np_GetCurrentSecondByUTC();
        while(record.time <= currentTime - lockedTime)
        {
            PPW_GetLobby()->GetCallbacks().lobbyScheduleProgressCallback(record.event);
            scheduleRecordsRemain.pop_front();
            if(scheduleRecordsRemain.empty())
            {
                break;
            }
            record = scheduleRecordsRemain.front();
        }
        break;
    }
    default:
        DWC_ASSERTMSG(FALSE, "PPW_LobbyInnerInfoManager::Process: Invalid State.");
        break;
    }
}

// ���������K�v�����邩
BOOL PPW_LobbyInnerInfoManager::NeedLock() const
{
    // ���r�[�̃X�e�[�g��Ready�ɂȂ�܂Ń��b�N����������
    if(state == LOCKED || PPW_GetLobby()->GetState() != PPW_LOBBY_STATE_READY)
    {
        return FALSE;
    }
    
    if(lockedTime != PPW_LOBBY_INVALID_TIME)
    {
        return FALSE;
    }
    
    // ���b�N�\��ς݂Ȃ烍�b�N������
    if(bLockReserved)
    {
        return TRUE;
    }
    
    // �Q���l�����m�F
    if(PPW_GetLobby()->GetProfileManager().IsFull())
    {
        return TRUE;
    }
    
    // ���Ԓ��߂��m�F
    if(DWCi_Np_GetCurrentSecondByUTC() > timeData.openedTime + lockTime)
    {
        return TRUE;
    }
    return FALSE;
}

// ����������ꂽ
// @retval TRUE ���b�N����
// @retval FALSE ���b�N��\�񂵂��̂ł��ƂŃ��b�N�����
BOOL PPW_LobbyInnerInfoManager::Lock()
{
    // ���r�[�̃X�e�[�g��Ready�ɂȂ�܂Ń��b�N����������
    if(state != LOCKED && PPW_GetLobby()->GetState() == PPW_LOBBY_STATE_READY)
    {
        lockedTime = DWCi_Np_GetCurrentSecondByUTC();
        state = LOCKED;
        bLockReserved = FALSE;
        
        // ���b�N��ʒm
        PPW_GetLobby()->GetCallbacks().lobbyScheduleProgressCallback(PPW_LOBBY_TIME_EVENT_LOCK);
        return TRUE;
    }
    
    // ���b�N���邱�Ƃ�\�񂷂�
    bLockReserved = TRUE;
    
    return FALSE;
}
