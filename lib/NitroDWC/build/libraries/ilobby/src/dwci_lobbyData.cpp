/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./src/lobby/dwci_lobbyBase.cpp

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
 * @brief �A���P�[�^�\�[�X
 */

#include "dwci_lobbyData.h"

inline void DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(const DWCi_String& channelName)
{
    DWC_ASSERTMSG(channelName.find("#") == 0, "Internal error. channelName is not modified.");
    (void)channelName;
}

/** 
 * @brief �������r�[���C�u�����I�u�W�F�N�g�R���X�g���N�^
 * 
 * @param[in] 
 */
DWCi_Lobby::DWCi_Lobby(const DWCi_String& _gameName, const DWCi_String& _secretKey, DWC_LOBBY_CHANNEL_PREFIX _prefix, s32 _pid,
                       const DWCi_String& _chatNick, const DWCi_String& _chatUser, const DWCi_String& _chatName, const DWCi_LobbyGlobalCallbacks* _globalCallbacks)
    : chat(NULL)
    , state(DWCi_LOBBY_STATE_NOTINITIALIZED)
    , pid(_pid)
    , chatNick(_chatNick)
    , chatUser(_chatUser)
    , chatName(_chatName)
    , gameName(_gameName)
    , secretKey(_secretKey)
    , baseChannelName()
    , retryCount(0)
    , error(DWCi_LOBBY_ERROR_NONE)
    , connectingWaitCount(0)
    , serverDisconnectedMe(FALSE)
    , globalCallbacks(*_globalCallbacks)
    , users()
    , channels()
    , callbackManager()
{
    const char* const channelPrefix[] =
    {
        "R",    // DWC_LOBBY_CHANNEL_PREFIX_RELEASE
        "D",    // DWC_LOBBY_CHANNEL_PREFIX_DEBUG
        "D1",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG1
        "D2",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG2
        "D3",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG3
        "D4",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG4
        "D5",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG5
        "D6",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG6
        "D7",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG7
        "D8",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG8
        "D9",   // DWC_LOBBY_CHANNEL_PREFIX_DEBUG9
        "T"     // DWC_LOBBY_CHANNEL_PREFIX_TEST
    };
    
    // #GSP!gameName!T|R|D_ �`���̃`�����l���������
    baseChannelName = DWCi_String(DWCi_LOBBY_CHANNEL_NAME_PREFIX) + DWCi_LOBBY_CHANNEL_NAME_SEPARATOR
        + gameName + DWCi_LOBBY_CHANNEL_NAME_SEPARATOR
        + DWCi_String(channelPrefix[_prefix]) + DWCi_LOBBY_CHANNEL_NAME_SEPARATOR;
    DWC_Printf(DWC_REPORTFLAG_INFO, "Created DWCi_Lobby\n");
}

/** 
 * @brief �������r�[���C�u�����I�u�W�F�N�g�f�X�g���N�^
 */
DWCi_Lobby::~DWCi_Lobby()
{
    DWC_Printf(DWC_REPORTFLAG_INFO, "Deleted DWCi_Lobby\n");
}

/** 
 * @brief �ȗ����ꂽ�`�����l�������t���̖��O�ɖ߂��܂��B
 * 
 * @param[in] channelNameSuffix �ȗ����ꂽ�`�����l�����B
 * 
 * @retval �߂����`�����l�����B
 */
DWCi_String DWCi_Lobby::ModifyChannelName(const DWCi_String& channelNameSuffix) const
{
    return baseChannelName + channelNameSuffix;
}

/** 
 * @brief �`�����l������#GSP!<gamename>!���ȗ��������̂ɕϊ����܂��B
 * 
 * @param[in] channelName �`�����l�����B
 * 
 * @retval �ϊ����ꂽ�`�����l�����B
 */
DWCi_String DWCi_Lobby::UnmodifyChannelName(const DWCi_String& channelName) const
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    return channelName.substr(baseChannelName.length());
}

/** 
 * @brief �����̃`�����l������#GSP!<gamename>!���ȗ��������̂ɕϊ����܂��B
 * 
 * @param[in] numChannels �`�����l�����B
 * @param[in] channelNames �`�����l�����ւ̃|�C���^�̔z��B
 * 
 * @retval �ϊ����ꂽ�`�����l�����B
 */
std::vector<char*, DWCi_Allocator<char*> > DWCi_Lobby::UnmodifyChannelNames(int numChannels, const char** channelNames) const
{
    // channelNames�̃|�C���^��baseChannelName.length()�����炵�����̂�vector�ɃR�s�[����
    std::vector<char*, DWCi_Allocator<char*> > result;

    for(int i = 0; i<numChannels; i++)
    {
        result.push_back((char*)(channelNames[i] + baseChannelName.length()));
    }
    //std::transform(test.begin(), test.end(), result.begin(), std::bind1st(std::plus<char*>(), (int)baseChannelName.length()));
    
    return result;
}

/** 
 * @brief ��ӂƂȂ�j�b�N�l�[���𐶐����܂��B
 * 
 * @param[in] pid �������ɂȂ�PID�B
 * 
 * @retval ���������j�b�N�l�[���B
 */
DWCi_String DWCi_Lobby::CreateNickName(s32 pid)
{
    // �j�b�N�l�[����"N_(PID(�ő�10��))_(����(�ő�3��))�ɂ���
    return DWCi_SNPrintf<DWCi_String>(DWCi_MAX_CHAT_NICK_SIZE, _T("N_%d_%u"), pid, (unsigned long)current_time() % 1000);
}

/** 
 * @brief �j�b�N�l�[����PID�ɕϊ����܂��B
 * 
 * @param[in] nick �j�b�N�l�[��
 * 
 * @retval pid ����
 * @retval DWC_LOBBY_INVALID_USER_ID ���s
 */
s32 DWCi_Lobby::NickNameToPid(const DWCi_String& nick)
{
    std::vector<DWCi_String, DWCi_Allocator<DWCi_String> > ret(DWCi_SplitByChars<DWCi_String>(nick, "_"));
    if(ret.size() < 2)
    {
        return DWC_LOBBY_INVALID_USER_ID;
    }
    int pid = atoi(ret[1].c_str());
    if(pid == 0)
    {
        return DWC_LOBBY_INVALID_USER_ID;
    }
    return pid;
}

/** 
 * @brief �`�����l����ǉ����܂��B
 * 
 * @param[in] channelName �`�����l����
 * @param[in] channelCallbacks �`�����l���Ɋ֘A�Â���R�[���o�b�N
 * 
 * @retval TRUE �����B
 * @retval FALSE ���s�B���łɓo�^�ς݁B
 */
BOOL DWCi_Lobby::AddChannel(const DWCi_String channelName, const DWCi_LobbyChannelCallbacks& channelCallbacks, s32 limit)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    std::pair<ChannelContainer::iterator, bool> result;
    result = channels.insert(std::make_pair(channelName, DWCi_LobbyChannel(channelCallbacks, limit)));
    return (BOOL)result.second;
}

/** 
 * @brief �`�����l���R�[���o�b�N���擾���܂��B
 * 
 * @param[in] channelName �`�����l����
 * 
 * @retval �`�����l���R�[���o�b�N�B
 * @retval NULL ���s�B�`�����l����������Ȃ��B
 */
const DWCi_LobbyChannelCallbacks* DWCi_Lobby::GetChannelCallbacks(const DWCi_String& channelName) const
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    ChannelContainer::const_iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        return NULL;
    }
    return &it->second.channelCallbacks;
}

/** 
 * @brief ���[�U��ǉ����܂��B
 * 
 * @param[in] nick �j�b�N�l�[��
 * @param[in] channelName �`�����l����
 * 
 * @retval �ǉ������l�̃��[�UID
 * @retval DWC_LOBBY_INVALID_USER_ID ���s�B�s����nick�A�������̓`�����l����������Ȃ��B
 */
s32 DWCi_Lobby::AddUser(const DWCi_String& nick, const DWCi_String& channelName)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    s32 userId = NickNameToPid(nick);
    
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        return DWC_LOBBY_INVALID_USER_ID;
    }
    
    // �`�����l�����ɒǉ�����B
    ChannelContainer::iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        DWC_ASSERTMSG(FALSE, "DWCi_Lobby::AddUser: no such channelName.");
        return DWC_LOBBY_INVALID_USER_ID;
    }
    it->second.userIds.insert(userId);
    
    // ���[�U���ɒǉ�����B��������ID�������ꍇ�͏㏑������B(�Đڑ������Ƃ��ɐ̂̂��̂��c���Ă��邱�Ƃ�����)
    users[userId] = nick;
    return userId;
}

/** 
 * @brief ���[�U�𕡐��l�ǉ����܂��B
 * 
 * @param[in] numUsers ���[�U��
 * @param[in] users �j�b�N�l�[���̔z��
 * @param[in] channelName �`�����l����
 * 
 * @retval �ǉ��������͏㏑�������l�̃��[�UID��vector�B�ǉ����Ă��Ȃ��l��vector���Ɋ܂܂�Ȃ��B
 */
std::vector<s32, DWCi_Allocator<s32> > DWCi_Lobby::AddUsers(int numUsers, const char** users, const DWCi_String& channelName)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    std::vector<s32, DWCi_Allocator<s32> > userIds;
    userIds.reserve((std::size_t)numUsers);
	for(int i = 0; i < numUsers; i++)
    {
        // �ߋ��̎����͓o�^���Ȃ��B���[�UID�������Ńj�b�N���Ⴄ���̂��ߋ��̎���
        if(NickNameToPid(users[i]) == GetPid() && GetChatNick() != users[i])
        {
            continue;
        }
        s32 userId = AddUser(users[i], channelName);
        if(userId != DWC_LOBBY_INVALID_USER_ID)
        {
            userIds.push_back(userId);
        }
    }
    return userIds;
}

/** 
 * @brief �����S�Ẵ`�����l���Ɏw�肵�����[�U�����Ȃ������烆�[�U��񂩂烆�[�U���폜���܂��B
 * 
 * @param[in] userId �폜���郆�[�UID�B
 * 
 * @retval TRUE �폜�����B
 * @retval FALSE �܂��`�����l���Ɏc���Ă����̂ō폜���Ȃ������B
 */
BOOL DWCi_Lobby::RemoveIfNoUserInChannels(s32 userId)
{
    // �����S�Ẵ`�����l�����ɂ��̃��[�U�����Ȃ���΃��[�U��񂩂���폜
    ChannelContainer::const_iterator it;
    it = std::find_if(channels.begin(), channels.end(), ExistUserInChannelContainer(userId));
    if(it == channels.end())
    {
        users.erase(userId);
        return TRUE;
    }
    return FALSE;
}

/** 
 * @brief �w�肵�����[�U���`�����l������폜���܂��B
 * 
 * @param[in] nick �폜���郆�[�UID�B
 * @param[in] channelName �폜����`�����l�����B
 * 
 * @retval TRUE �폜�����B
 * @retval FALSE �폜���Ȃ������B�j�b�N�l�[�����s���A�������̓`�����l����������Ȃ�(internal error)�A�������̓`�����l�����ɂ��̃��[�U�����Ȃ��B
 */
BOOL DWCi_Lobby::RemoveUser(const DWCi_String& nick, const DWCi_String& channelName)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    s32 userId = NickNameToPid(nick);
    
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        return FALSE;
    }
    
    // �`�����l����񂩂�폜
    ChannelContainer::iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        DWC_ASSERTMSG(FALSE, "DWCi_Lobby::RemoveUser: no such channelName.");
        return FALSE;
    }
    BOOL result = (BOOL)it->second.userIds.erase(userId);
    
    if(!result)
    {
        return FALSE;
    }
    
    // �����S�Ẵ`�����l�����ɂ��̃��[�U�����Ȃ���΃��[�U��񂩂���폜
    RemoveIfNoUserInChannels(userId);
    return TRUE;
}

/** 
 * @brief �w�肵���`�����l���ɂ��郆�[�U�ƃ`�����l���̏����폜���܂��B
 * 
 * n^2�̃I�[�_�[�Ȃ̂Œx��
 * 
 * @param[in] channelName �폜����`�����l�����B
 * 
 * @retval TRUE �폜�����B
 * @retval FALSE �w�肵���`�����l����������Ȃ��B
 */
BOOL DWCi_Lobby::RemoveChannel(const DWCi_String& channelName)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    ChannelContainer::iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        return FALSE;
    }
    
    // �ꎞ�o�b�t�@��ID��ۑ�
    std::vector<s32, DWCi_Allocator<s32> > tempBuf;
    tempBuf.assign(it->second.userIds.begin(), it->second.userIds.end());
    
    channels.erase(channelName);
    
    // �����S�Ẵ`�����l�����ɂ��̃��[�U�����Ȃ���΃��[�U��񂩂���폜
    std::for_each(tempBuf.begin(), tempBuf.end(), std::bind1st(std::mem_fun(&DWCi_Lobby::RemoveIfNoUserInChannels), this));
    return TRUE;
}

/** 
 * @brief �w�肵���j�b�N�l�[���̃��[�U��{���܂�(�j�b�N�l�[���̈�v���m�F����)
 * 
 * @param[in] nick �j�b�N�l�[���B
 * 
 * @retval ���[�UID
 * @retval DWC_LOBBY_INVALID_USER_ID ������Ȃ������B
 */
s32 DWCi_Lobby::FindUser(const DWCi_String& nick) const
{
    s32 userId = NickNameToPid(nick);
    
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        return DWC_LOBBY_INVALID_USER_ID;
    }
    
    // ���݂��Ă��邩�m�F����
    UserContainer::const_iterator it = users.find(userId);
    if(it == users.end())
    {
        return DWC_LOBBY_INVALID_USER_ID;
    }
    // ���[�U�����m�F����B(�Đڑ��������[�U��nick���ω�����̂ł��̊m�F)
    if(it->second != nick)
    {
        return DWC_LOBBY_INVALID_USER_ID;
    }
    return userId;
}

/** 
 * @brief �w�肵�����[�UID�̃��[�U��{���܂��B
 * 
 * @param[in] userId ���[�UID�B
 * 
 * @retval �j�b�N�l�[���B
 * @retval "" ������Ȃ������B
 */
DWCi_String DWCi_Lobby::FindUser(s32 userId) const
{
    UserContainer::const_iterator it = users.find(userId);
    if(it == users.end())
    {
        return "";
    }
    return it->second;
}

/** 
 * @brief �j�b�N�l�[����ύX���܂��B
 * 
 * @param[in] oldNick �Â��j�b�N�l�[���B 
 * @param[in] newNick �V�����j�b�N�l�[���B
 * 
 * @retval �ύX���ꂽ���[�UID�B
 * @retval DWC_LOBBY_INVALID_USER_ID InternalError�B
 */
s32 DWCi_Lobby::RenameUser(const DWCi_String& oldNick, const DWCi_String& newNick)
{
    s32 userId = FindUser(oldNick);
    users.erase(userId);
    std::pair<UserContainer::iterator, bool> ret;

    ret = users.insert(std::make_pair(userId, newNick));
    if(!ret.second)
    {
        DWC_ASSERTMSG(FALSE, "DWCi_Lobby::RenameUser: Internal error. There is same user.");
        return DWC_LOBBY_INVALID_USER_ID;
    }
    return userId;
}

/** 
 * @brief �w�肵���`�����l���ɎQ�����Ă���S�Ẵ��[�UID��Ԃ��܂��B
 * 
 * @param[in] channelName �`�����l�����B
 * 
 * @retval ���[�UID��vector�B
 * @retval ���vector �w�肵���`�����l���ɎQ�����Ă��Ȃ��B
 */
std::vector<s32, DWCi_Allocator<s32> > DWCi_Lobby::EnumUsers(const DWCi_String& channelName) const
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    ChannelContainer::const_iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        // ������Ȃ��Ƃ���FALSE
        return std::vector<s32, DWCi_Allocator<s32> >();
    }
    
    std::vector<s32, DWCi_Allocator<s32> > userIds;
    userIds.assign(it->second.userIds.begin(), it->second.userIds.end());
    return userIds;
}

/** 
 * @brief �`�����l�����L��������Ă��邩���ׂ܂��B
 * 
 * @param[in] channelName �C�����ꂽ�`�����l�����B 
 * 
 * @retval TRUE �L��������Ă���B
 * @retval FALSE �L��������Ă��Ȃ����`�����l����������Ȃ��B
 */
BOOL DWCi_Lobby::IsChannelActivated(const DWCi_String& channelName) const
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    ChannelContainer::const_iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        // ������Ȃ��Ƃ���FALSE
        return FALSE;
    }
    return it->second.activated;
}

/** 
 * @brief �`�����l�����L�������܂��B
 * 
 * @param[in] channelName �`�����l�����B 
 * 
 * @retval TRUE �����B
 * @retval FALSE �`�����l����������Ȃ��B
 */
BOOL DWCi_Lobby::ActivateChannel(const DWCi_String& channelName)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    ChannelContainer::iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        // ������Ȃ��Ƃ���FALSE
        return FALSE;
    }
    it->second.activated = TRUE;
    return TRUE;
}

/** 
 * @brief �`�����l���̃f�t�H���g�����l��(::DWCi_LobbyJoinChannelLimitAsync�֐��Ŏw�肵������)���擾���܂��B
 * 
 * @param[in] channelName �`�����l�����B 
 * 
 * @retval 0�ȏ� �����B�����l���B
 * @retval -1 �`�����l����������Ȃ��B
 */
s32 DWCi_Lobby::GetDefaultChannelLimit(const DWCi_String& channelName)
{
    DWCi_LOBBY_ASSERT_IF_NOT_MODIFIED_CHANNEL_NAME(channelName);
    ChannelContainer::iterator it = channels.find(channelName);
    if(it == channels.end())
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "DWCi_Lobby::GetDefaultChannelLimit: No such channel: %s\n", channelName.c_str());
        return -1;
    }
    return it->second.limit;
}
