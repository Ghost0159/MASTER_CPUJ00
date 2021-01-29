/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/lobby/include/dwci_lobby.h

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
 * @brief Wi-Fi ���r�[���C�u���� �`�����l���f�[�^�w�b�_
 */

#ifndef DWCi_LOBBY_DATA_H_
#define DWCi_LOBBY_DATA_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <algorithm>
#include "dwci_lobbyNonport.h"
#include "lobby/dwci_lobby.h"
#include "dwci_lobbyBase.h"
#include "dwci_lobbyUtil.h"
#include "dwci_lobbyCallback.h"

// �`�����l����
static const char DWCi_LOBBY_CHANNEL_NAME_PREFIX[] = "#GSP";


// ���r�[���(�����g�p)
class DWCi_Lobby : public DWCi_Base
{
    CHAT chat;
    DWCi_LOBBY_STATE state;
    const s32 pid;
    DWCi_String chatNick;
    const DWCi_String chatUser;
    const DWCi_String chatName;
    const DWCi_String gameName;
    const DWCi_String secretKey;
    DWCi_String baseChannelName;
    u32 retryCount;
    DWCi_LOBBY_ERROR error;
    u32 connectingWaitCount;
    BOOL serverDisconnectedMe;
    
    // �ÓI�R�[���o�b�N
    const DWCi_LobbyGlobalCallbacks globalCallbacks;
    
    //
    // ���[�UID�R���e�i
    //
    typedef std::map<s32, DWCi_String, std::less<s32>, DWCi_Allocator<std::pair<s32, DWCi_String> > > UserContainer;
    UserContainer users;
    
    //
    // �`�����l���R���e�i
    //
    // key: �`�����l����
    // value: DWCi_LobbyChannel = { �A�N�e�B�x�[�g�ς݂�, �`�����l�����̃��[�UID, �`�����l���̏����ő�l��, �`�����l���R�[���o�b�N }
    //
    typedef std::set<s32, std::less<s32>, DWCi_Allocator<s32> > UserIdContainer;
    
    struct DWCi_LobbyChannel : public DWCi_Base
    {
        BOOL activated;
        UserIdContainer userIds;
        s32 limit;
        const DWCi_LobbyChannelCallbacks channelCallbacks;
        DWCi_LobbyChannel()
            : channelCallbacks()
        {}
        DWCi_LobbyChannel(const DWCi_LobbyChannelCallbacks& _channelCallbacks, s32 _limit)
            : activated(FALSE)
            , userIds()
            , limit(_limit)
            , channelCallbacks(_channelCallbacks)
        {}
    };
    typedef std::map<DWCi_String, DWCi_LobbyChannel, std::less<DWCi_String>, DWCi_Allocator<std::pair<DWCi_String, DWCi_LobbyChannel> > > ChannelContainer;
    ChannelContainer channels;
    
    // ���I�R�[���o�b�N�Ǘ�
    DWCi_CallbackManager callbackManager;
public:
    DWCi_Lobby(const DWCi_String& _gameName, const DWCi_String& _secretKey, DWC_LOBBY_CHANNEL_PREFIX _prefix, s32 _pid,
               const DWCi_String& _chatNick, const DWCi_String& _chatUser, const DWCi_String& _chatName, const DWCi_LobbyGlobalCallbacks* _globalCallbacks);
    
    ~DWCi_Lobby();
    
    CHAT GetChat() const
    {
        return chat;
    }
    
    void SetChat(CHAT _chat)
    {
        chat = _chat;
    }
    
    DWCi_LOBBY_STATE GetState() const
    {
        return state;
    }
    
    void SetState(DWCi_LOBBY_STATE _state)
    {
        // �G���[���͑J�ڂ��Ȃ�
        if(state == DWCi_LOBBY_STATE_ERROR)
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "SetState: Can' set state because state is already error.");
            return;
        }
        state = _state;
    }
    
    s32 GetPid() const
    {
        return pid;
    }
    
    DWCi_String GetChatNick() const
    {
        return chatNick;
    }
    
    void SetChatNick(const DWCi_String& _chatNick)
    {
        chatNick = _chatNick;
    }
    
    DWCi_String GetChatUser() const
    {
        return chatUser;
    }
    
    DWCi_String GetChatName() const
    {
        return chatName;
    }
    
    DWCi_String GetGameName() const
    {
        return gameName;
    }
    
    DWCi_String GetSecretKey() const
    {
        return secretKey;
    }
    
    DWCi_String ModifyChannelName(const DWCi_String& channelNameSuffix) const;
    
    DWCi_String UnmodifyChannelName(const DWCi_String& channelName) const;
    
    std::vector<char*, DWCi_Allocator<char*> > UnmodifyChannelNames(int numChannels, const char** channelNames) const;
    
    u32 GetRetryCount() const
    {
        return retryCount;
    }
    
    void ResetRetryCount()
    {
        retryCount = 0;
    }
    
    void IncrementRetryCount()
    {
        retryCount++;
    }
    
    DWCi_LobbyGlobalCallbacks GetGlobalCallbacks() const
    {
        return globalCallbacks;
    }
    
    DWCi_LOBBY_ERROR GetError() const
    {
        return error;
    }
    
    void SetError(DWCi_LOBBY_ERROR _error)
    {
        error = _error;
        
        // �X�e�[�g���G���[�ɂ��ĂقƂ�ǂ̊֐����ĂׂȂ�����B
        state = DWCi_LOBBY_STATE_ERROR;
    }
    
    BOOL WaitAfterConnect()
    {
        if(connectingWaitCount++ == DWCi_LOBBY_CONNECTINGWAIT_TIME)
        {
            connectingWaitCount = 0;
            SetState(DWCi_LOBBY_STATE_CONNECTED);
            return TRUE;
        }
        return FALSE;
    }
    
    BOOL GetServerDisconnectedMe() const
    {
        return serverDisconnectedMe;
    }
    
    void SetServerDisconnectedMe()
    {
        serverDisconnectedMe = TRUE;
    }
    
    DWCi_CallbackManager& GetCallbackManager()
    {
        return callbackManager;
    }
    
    static DWCi_String CreateNickName(s32 pid);
    
    static s32 NickNameToPid(const DWCi_String& nick);
    
    BOOL AddChannel(const DWCi_String channelName, const DWCi_LobbyChannelCallbacks& channelCallbacks, s32 limit);
    
    const DWCi_LobbyChannelCallbacks* GetChannelCallbacks(const DWCi_String& channelName) const;
    
    s32 AddUser(const DWCi_String& nick, const DWCi_String& channelName);
    
    std::vector<s32, DWCi_Allocator<s32> > AddUsers(int numUsers, const char** users, const DWCi_String& channelName);
    
private:
    // find_if�p�֐��I�u�W�F�N�g ChannelContainer::value_type == std::pair<DWCi_String, UserIdContainer>
    // �`�����l���R���e�i�Ɏw�肵�����[�U�����邩
    struct ExistUserInChannelContainer : public std::unary_function<ChannelContainer::value_type, bool>
    {
        const s32 userId;
        ExistUserInChannelContainer(s32 _userId)
            : userId(_userId)
        {}
        
        bool operator() (const ChannelContainer::value_type& rhs) const
        {
            return rhs.second.userIds.find(userId) != rhs.second.userIds.end();
        }
    };
    
    BOOL RemoveIfNoUserInChannels(s32 userId);
    
public:
    BOOL RemoveUser(const DWCi_String& nick, const DWCi_String& channelName);
    
    BOOL RemoveChannel(const DWCi_String& channelName);
    
    s32 FindUser(const DWCi_String& nick) const;

    // �Y�����[�UID�̃��[�U��{��
    DWCi_String FindUser(s32 userId) const;
    
    s32 RenameUser(const DWCi_String& oldNick, const DWCi_String& newNick);
    
    std::vector<s32, DWCi_Allocator<s32> > EnumUsers(const DWCi_String& channelName) const;
    
    BOOL IsChannelActivated(const DWCi_String& channelName) const;
    
    BOOL ActivateChannel(const DWCi_String& channelName);
    
    s32 GetDefaultChannelLimit(const DWCi_String& channelName);
};

#endif
