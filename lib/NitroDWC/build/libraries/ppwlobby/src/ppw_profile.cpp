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
#include "ppw_profile.h"


/** 
 * @brief ��������B��Ԃ�J�ڂ����܂��B
 */
void PPW_LobbyInnerProfile::Process()
{
    switch(state)
    {
    case STATE_PREPARING:
        state = (systemProfileInitialized && userProfileInitialized) ? STATE_DATARECVED : STATE_PREPARING;
        if(state == STATE_PREPARING)
        {
            break;
        }
        // STATE_DATARECVED�ɂȂ����ꍇ��ready�̃`�F�b�N���s���̂�break���Ȃ��B�X�e�[�g�̑J�ڂ��x��A��Ɏ�����join����̂�h���B
    case STATE_DATARECVED:
        if(systemProfile.Get().ready)
        {
            state = STATE_READY;
        }
        break;
    case STATE_READY:
        if(activated)
        {
            state = STATE_ACTIVATED;
        }
        break;
    case STATE_ACTIVATED:
        break;
    default:
        DWC_ASSERTMSG(FALSE, "PPW_LobbyInnerProfile::Process: Invalid state.");
        break;
    }
}



/** 
 * @brief ���łɓo�^����Ă��Ȃ���΃��[�U��o�^���܂��B
 * 
 * @param id ���[�UID
 * 
 * @retval TRUE �o�^�����B
 * @retval FALSE �o�^�ς݁B
 */
BOOL PPW_LobbyProfileManager::AddProfile(s32 id)
{
    std::pair<PPW_ProfileContainer::iterator, bool> ret;
    PPW_LobbyInnerProfile* newProfile = new PPW_LobbyInnerProfile(id);

    ret = profiles.insert(newProfile);
    if(!ret.second)
    {
        // �o�^�ς�
        delete newProfile;
        return TRUE;
    }
    return TRUE;
}

/** 
 * @brief ���[�U���폜���܂��B
 * 
 * @param id ���[�UID
 * 
 * @retval TRUE �폜�����B
 * @retval FALSE ���݂��Ȃ������B
 */
BOOL PPW_LobbyProfileManager::RemoveProfile(s32 id)
{
    for(PPW_ProfileContainer::iterator it = profiles.begin(); it != profiles.end(); it++)
    {
        if((*it)->GetPid() == id)
        {
            delete *it;
            profiles.erase(it);
            return TRUE;
        }
    }
    return FALSE;
}

/** 
 * @brief ���[�U��T���܂��B
 * 
 * @param id ���[�UID
 * 
 * @retval �v���t�B�[�� ���������B
 * @retval NULL ������Ȃ������B
 */
PPW_LobbyInnerProfile* PPW_LobbyProfileManager::FindProfile(s32 id)
{
    if(id == myProfile.GetPid())
    {
        return &myProfile;
    }
    PPW_ProfileContainer::const_iterator it = std::find_if(profiles.begin(), profiles.end(), PPW_LobbyInnerProfile::Equal(id));
    return it == profiles.end() ? NULL : *it;
}

/** 
 * @brief ���[�U����S�č폜���܂��B
 */
void PPW_LobbyProfileManager::ClearProfile()
{
    for(PPW_ProfileContainer::iterator it = profiles.begin(); it != profiles.end();)
    {
        delete *it;
        profiles.erase(it++);
    }
}

/** 
 * @brief Activate�ςݎQ���҂������ł��邩���ׂ܂��B
 * 
 * @retval TRUE �����B
 * @retval FALSE �܂��B
 */
BOOL PPW_LobbyProfileManager::IsFull() const
{
    // ���ׂ��y�����邽�߂ɂ܂�vector�̑傫���Ŕ�r���A����activate�ς݂̃T�C�Y���m�F����
    // �����̕���1�v���X���Đ�����
    return profiles.size()+1 == PPW_LOBBY_MAX_PLAYER_NUM_MAIN && EnumId().size()+1 == PPW_LOBBY_MAX_PLAYER_NUM_MAIN;
}

/** 
 * @brief Activate�ςݎQ���ґS����pid��񋓂��܂��B
 * 
 * @retval ���[�UID��vector�B
 */
std::vector<s32, DWCi_Allocator<s32> > PPW_LobbyProfileManager::EnumId() const
{
    std::vector<s32, DWCi_Allocator<s32> > userIds;
    userIds.reserve(profiles.size());
    for(PPW_ProfileContainer::const_iterator it = profiles.begin(); it != profiles.end(); it++)
    {
        if((*it)->IsActivated())
        {
            userIds.push_back((*it)->GetPid());
        }
    }
    return userIds;
}

/** 
 * @brief �����̃v���t�B�[�������������܂��B
 * 
 * @param id ���[�UID�B
 * @param userProfile ���[�U��`�v���t�B�[���B
 * @param userProfileSize ���[�U��`�v���t�B�[���T�C�Y�B
 */
void PPW_LobbyProfileManager::InitializeMyProfile(s32 id, const u8* userProfile, u32 userProfileSize)
{
    myProfile = PPW_LobbyInnerProfile(id);
    myProfile.SetSystemProfile(PPW_LobbyInnerProfile::SystemProfile());
    myProfile.SetUserProfile(userProfile, userProfileSize, FALSE);
    myProfile.Activate();
}

/** 
 * @brief �Q���ґS���̃v���t�B�[������M���������ׂ܂��B
 * 
 * @retval TRUE ��M�����B
 * @retval FALSE �܂��B
 */
BOOL PPW_LobbyProfileManager::CheckAllProfilesRecved() const
{
    // �v���t�B�[������M���Ă��Ȃ����̂�T��
    PPW_ProfileContainer::const_iterator it = std::find_if(profiles.begin(), profiles.end(), std::not1(std::mem_fun(&PPW_LobbyInnerProfile::IsDataRecved)));
    return it == profiles.end();
}

// ������s
// �K�v�ɉ����ă��[�U�ڑ��R�[���o�b�N���Ă�
void PPW_LobbyProfileManager::Process()
{
    // �ڑ������l�̃v���t�B�[���o�^�m�F
    for(PPW_ProfileContainer::iterator it = profiles.begin(); it != profiles.end(); it++)
    {
        (*it)->Process();
        switch((*it)->GetState())
        {
        case PPW_LobbyInnerProfile::STATE_PREPARING:
            break;
        case PPW_LobbyInnerProfile::STATE_DATARECVED:
            break;
        case PPW_LobbyInnerProfile::STATE_READY:
        {
            const PPW_LobbyInnerProfile::SystemProfile& systemProfile = (*it)->GetSystemProfileConfirm();
            const PPW_LobbyInnerProfile::UserProfile& userProfile = (*it)->GetUserProfileConfirm();
            
            PPW_LobbySystemProfile publicSystemProfile = systemProfile.ToPublicSystemProfile();
            
            // ���[�U�ڑ������R�[���o�b�N���Ă�
            PPW_GetLobby()->GetCallbacks().lobbyPlayerConnectedCallback((*it)->GetPid(), &publicSystemProfile, DWCi_GetVectorBuffer(userProfile.data),
                                                                            userProfile.data.size());
            (*it)->Activate();
            break;
        }
        case PPW_LobbyInnerProfile::STATE_ACTIVATED:
            break;
        default:
            DWC_ASSERTMSG(FALSE, "PPW_LobbyProfileManager::Process: Invalid state.");
        }
    }
}
