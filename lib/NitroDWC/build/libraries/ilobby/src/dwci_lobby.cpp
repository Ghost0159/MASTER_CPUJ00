/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./src/lobby/dwci_lobby.c

  Copyright 2005-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
/**
 * @file dwci_lobby.h
 *
 * @brief Wi-Fi ���r�[���C�u���� �\�[�X
 * 
 * Wi-Fi ���r�[���C�u����
 */

/**
 * @page DWCi_LOBBY Wi-Fi ���r�[���C�u����
 *
 * @section Introduction
 *
 * ���̃��C�u�����ɂ���āAGameSpy��IRC�T�[�o����đ��̃v���C���[�ƒʐM���邱�Ƃ��ł��܂��B
 *
 * ���ׂĂ̒ʐM�̓T�[�o�𒆌p����̂ŁA�ł������ʐM�ʂ����Ȃ��Ȃ�悤�ɂ��Ă��������B\n
 * �ڈ��Ƃ��ẮA�`�����l���S�̂ɑ��M�������(::DWCi_LobbySendChannelMessage�֐���::DWCi_LobbySetChannelData�֐��ɂ��u���[�h�L���X�g)�́A
 * ���������b�Z�[�W�̏ꍇ�͈�l�ɂ����b10���b�Z�[�W�ȉ��A�傫�����b�Z�[�W�̏ꍇ�͖��b5���b�Z�[�W�ȉ��ɗ}���Ă��������B\n
 * ����ɁA�ꕪ�Ԃ�50���b�Z�[�W�𒴂��Ȃ��悤�ɂ��Ă��������B
 * ���C�u���������ł̃������m�ۂ�DWC_Alloc�֐����g�p���Ă��܂��B\n
 * �{���C�u�����̓X���b�h�Z�[�t�ł͂���܂���B\n
 * ��ȏ�̃X���b�h�œ����Ƀ��C�u�����̊֐����Ăяo���Ȃ��ł��������B\n
 * �{���C�u�����Ńv���C���[�����ʂ��邽�߂Ɏg�p���郆�[�UID��GameSpyProfileID(pid)�ƈ�v���Ă��܂��B\n
 * �����̃��[�UID�͏��������::DWCi_LobbyGetMyUserId�֐����g�p���邱�Ƃɂ��擾�ł��܂��B\n
 * ����̓v���C���[���ƂɃ��j�[�N�Ȓl�ƂȂ�܂��B\n
 * �������A�f�o�b�O�p��::DWCi_LobbyInitializePidAsync�֐����g���ď����������Ƃ��A�����̃v���C���[��
 * ����pid���w�肵���ꍇ���[�UID�����j�[�N�ł͂Ȃ��Ȃ��Ă��܂��܂��B\n
 * ���̏ꍇ�̓���͕s��ł��̂ŕK���قȂ���pid���w�肷��悤�ɂ��Ă��������B\n
 * ���ׂĂ�char�^�̃p�����[�^�ɂ�0x5C������0x22-0x7E�͈̔͂̕����y�яI�[��0x00���w�肵�Ă��������B\n
 * 
 * 
 * @section Usage
 *
 *<table border=1 bgcolor="mistyrose"><tr><td>
 *
 * �����C�u�����̏�����
 *
 * ::DWCi_LobbyInitializeAsync�֐����g�����C�u���������������܂��B\n
 * ���炩����DWC_LoginAsync�֐�����DWC_NASLoginAsync�֐���Wi-Fi�R�l�N�V�����ւ̐ڑ����������Ă���K�v������܂��B \n
 * ���C�u�����̃X�e�[�g��::DWCi_LOBBY_STATE_CONNECTED�ɂȂ�ƃT�[�o�ւ̐ڑ��������ł��B\n
 *
 * ���`�����l���ւ̎Q��
 * 
 * �`�����l���ւ̎Q����::DWCi_LobbyJoinChannelAsync�֐��ōs���܂��B��ʓI��IRC�Ɠ��l�ɔC�ӂ̃`�����l�������w�肵�čs���܂��B\n
 * �����w�肵���`�����l�����̃`�����l�������݂����ꍇ�͂��̃`�����l���ɓ���܂��B�������������ꍇ�͐V�K�Ƀ`�����l�����쐬������܂��B\n
 * �`�����l�����̑S�Ẵ��[�U�̓I�y���[�^�����������Ă��܂��B\n
 * 
 * �����b�Z�[�W�̑���M
 *
 * ���C�u�����̃X�e�[�g��::DWCi_LOBBY_STATE_CONNECTED�̂Ƃ��Ɋe�탁�b�Z�[�W�̑���M�A�`�����l���f�[�^�̑���M���ł��܂��B\n
 * �`�����l���f�[�^�Ƃ͎Q�����Ă���`�����l���̃v���C���[��l��l�ƃ`�����l�����̂Ɋ֘A�Â�����f�[�^�ł��B\n
 *
 * �����C�u�����̏I��
 *
 * ::DWCi_LobbyShutdownAsync�֐����Ăяo���ă��C�u�������I�����܂��B\n
 * ���̊֐����Ăяo������A::DWCi_LobbyProcess�֐���::DWCi_LOBBY_STATE_NOTINITIALIZED��Ԃ��ƃV���b�g�_�E�������ł��B\n
 * 
 * 
 * ���G���[����
 *
 * �v���I�ȃG���[�����������ꍇ��::DWCi_LobbyProcess�֐���::DWCi_LOBBY_STATE_ERROR��Ԃ��܂��B\n
 * ���̏�ԂɂȂ�ƂقƂ�ǂ̊֐����g�p�s�\�ɂȂ�܂��B\n
 * �܂��A�����Ń������m�ۂɎ��s�����ꍇ�A���̊֐��ł̏����͐���Ɋ������܂����A���C�u�����̃X�e�[�g��::DWCi_LOBBY_STATE_ERROR�ɕύX���܂��B\n
 * ���̂��ߎ��̊֐��Ăяo����::DWCi_LOBBY_RESULT_ERROR_CONDITION��::DWCi_LOBBY_STATE_ERROR���Ԃ�܂��B\n
 * ���̏�ԂɂȂ����ꍇ��::DWCi_LobbyGetLastError�֐��ŃG���[���e���m�F��A::DWCi_LobbyShutdownAsync�֐��ŃV���b�g�_�E�����Ă��������B\n
 * �v���I�ȃG���[�ɂ̓T�[�o�ւ̐ڑ����s(::DWCi_LOBBY_ERROR_SESSION)�A�������m�ێ��s(::DWCi_LOBBY_ERROR_ALLOC)�Ȃǂ�����܂��B
 * �܂��A���̂Ƃ��u�T�[�o�[�Ƃ̒ʐM���ɃG���[���������܂����B�v�Ƃ������b�Z�[�W�Ƌ���::DWCi_LobbyToErrorCode�֐����g�p���ē���ꂽ�G���[�R�[�h��\�����Ă��������B\n
 * ���Ƃ��΁A::DWCi_LobbyGetLastError�֐���::DWCi_LOBBY_ERROR_SESSION���擾�����ꍇ�́u42003�v��\�����Ă��������B
 * ���̑��A�`�����l���ւ̎Q���Ɏ��s�����ꍇ��`�����l���f�[�^�̎擾�Ɏ��s�����ꍇ�ȂǁA�y���G���[�̏ꍇ�̓��b�Z�[�W��\������K�v�͂���܂���B
 * �A�v���P�[�V�������Ń��r�[�@�\�𑱍s�ł��Ȃ��Ȃ����Ɣ��f�����ꍇ�̃��b�Z�[�W�\���ɂ��Ă̓A�v���P�[�V�����ɔC����܂��B
 *
 *</td></tr></table>
 */

#include <lobby/dwci_lobby.h>
#include "dwci_lobbyBase.h"
#include "dwci_lobbyData.h"
#include "dwci_lobbyCallback.h"
#include "dwci_lobbyNewHandler.h"

// �萔��`
static const u32 DWCi_MAX_RETRY_COUNT = 3;
static const int DWCi_LOBBY_MESSAGE_TYPE_NORMAL_COMMAND = CHAT_UTM;

// �X�^�e�B�b�N�ϐ��錾
static DWCi_Lobby*  s_iLobby      = NULL;

// ����������Ă��邩�`�F�b�N����
#define RETURN_IF_NOTINITIALIZED(retval) if(!s_iLobby){return retval;}

// �v���I�ȃG���[���N���Ă��Ȃ����`�F�b�N����
#define RETURN_IF_ERROR_STATE(retval) \
    if(s_iLobby && s_iLobby->GetState() == DWCi_LOBBY_STATE_ERROR)\
    {\
        return retval;\
    }

// �X�e�[�g��CONNECTED�ł��邩�`�F�b�N����
#define RETURN_IF_NOTCONNECTED(retval) \
    if(s_iLobby && s_iLobby->GetState() != DWCi_LOBBY_STATE_CONNECTED)\
    {\
        return retval;\
    }

// �ÓI�`�F�b�N��p�_�~�[�֐�
static void DummyStaticCheck()
{
    // �������ʃt���O�����������`�F�b�N
    DWCi_STATIC_ASSERT(DWCi_LOBBY_ENTER_RESULT_MAX == CHATBadChannelMask);
}

static void Raw(CHAT chat, const char* raw, void* param)
{
    RETURN_IF_NOTINITIALIZED((void)0);
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "RAW: %s\n", raw);
	
	(void)chat;
	(void)raw;
	(void)param;
}

static void Disconnected(CHAT chat, const char* reason, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

    // ���K�̎菇�ŃV���b�g�_�E�����������m�F����
    if(s_iLobby->GetState() == DWCi_LOBBY_STATE_CLOSING)
    {
	    DWC_Printf(DWC_REPORTFLAG_INFO, "Disconnected: %s\n", reason);
        DWC_SAFE_DELETE(s_iLobby);
    }
    else
    {
	    DWC_Printf(DWC_REPORTFLAG_ERROR, "Server disconnect me: %s\n", reason);
    
        // ������؂ꂽ(�v���I�G���[)
        s_iLobby->SetError(DWCi_LOBBY_ERROR_SESSION);
        
        // ������chatDisconnect()���ĂׂȂ��̂�(�ĂԂ�assert�ɂ�����)����Ƀt���O���Z�b�g����B
        s_iLobby->SetServerDisconnectedMe();
    }
	(void)chat;
	(void)reason;
	(void)param;
}

static void ChangedNickCallback(CHAT chat, CHATBool success, const char* oldNick, const char* newNick, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

	if(success)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "Successfully changed");
	    DWC_Printf(DWC_REPORTFLAG_INFO, " nick from %s to %s\n", oldNick, newNick);
		s_iLobby->SetChatNick(newNick);
	}
	else
    {
		DWC_Printf(DWC_REPORTFLAG_ERROR, "Failed to change");
    }
	(void)chat;
	(void)oldNick;
	(void)param;
}

static void PrivateMessage(CHAT chat, const char* user, const char* message, int type, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

	DWC_Printf(DWC_REPORTFLAG_INFO, "Private message from %s: %s\n", user, message);
    
    // �T�[�o����̃��b�Z�[�W���m�F����
    if(DWCi_StrnicmpChar(user, "SERVER", 6) == 0)
    {
        // Excess Flood�̏ꍇ�x������
        if(DWCi_StrnicmpChar(message, "Excess Flood", 12) != 0)
        {
            return;
        }
        std::vector<DWCi_String, DWCi_Allocator<DWCi_String> > result = DWCi_SplitByChars<DWCi_String>(message, " :");
        if(result.empty())
        {
            return;
        }
        u32 floodWeight = (u32)atoi(result.back().c_str());
        if(floodWeight == 0)
        {
            return;
        }
        // �R�[���o�b�N���Ă�
        s_iLobby->GetGlobalCallbacks().lobbyExcessFloodCallback(floodWeight, param);
        return;
    }

    if(type != DWCi_LOBBY_MESSAGE_TYPE_NORMAL_COMMAND)
    {
        return;
    }
    
    // userId���擾
    s32 userId = s_iLobby->FindUser(user);
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        return;
    }

    // �R�[���o�b�N���Ă�
    s_iLobby->GetGlobalCallbacks().lobbyUserMassageReceivedCallback(userId, message, param);
    (void)chat;
	(void)type;
	(void)param;
}

static void Invited(CHAT chat, const char* channel, const char* user, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "Invited by %s to %s\n", user, channel);
	
	(void)chat;
	(void)channel;
	(void)user;
	(void)param;
}

static void ChannelMessage(CHAT chat, const char* channel, const char* user, const char* message, int type, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "%s, in %s, said \"%s\"\n", user, channel, message);

    if(type != DWCi_LOBBY_MESSAGE_TYPE_NORMAL_COMMAND)
    {
        return;
    }
    
    // userId���擾
    s32 userId = s_iLobby->FindUser(user);

    // �R�[���o�b�N���Ă�
    const DWCi_LobbyChannelCallbacks* callbacks = s_iLobby->GetChannelCallbacks(channel);
    DWC_ASSERTMSG(callbacks, "ChannelMessage: Internal error. No callback info. channel: %s", channel);
    callbacks->lobbyChannelMassageReceivedCallback(s_iLobby->UnmodifyChannelName(channel).c_str(), userId, message, callbacks->param);
    
    (void)chat;
	(void)type;
	(void)param;
}

static void Kicked(CHAT chat, const char* channel, const char* user, const char* reason, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "Kicked from %s by %s: %s\n", channel, user, reason);

    // �`�����l�����폜
    s_iLobby->RemoveChannel(channel);
    
	(void)chat;
	(void)user;
	(void)reason;
	(void)param;
}

static void UserJoined(CHAT chat, const char* channel, const char* user, int mode, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

    // JoinChannelLimitAsync�̎��Ɏw�肵���f�t�H���g�̐l�����擾
    s32 limit = s_iLobby->GetDefaultChannelLimit(channel);
    
    // �����l���̃`�F�b�N
    if(limit != 0 && chatGetChannelNumUsers(s_iLobby->GetChat(), channel) > limit)
    {
        // �I�[�o�[�����l�������I�ɑގ�����̂Őڑ��R�[���o�b�N�͌Ă΂Ȃ��B
        DWC_Printf(DWC_REPORTFLAG_INFO, "%s joined %s but ignored: limit over.\n", user, channel);
        return;
    }
    
    // ����userId(�Đڑ��O�̖S��)�����݂��邩�m�F
    // ��xPID�ɒ����Ă���ߋ��̃j�b�N�l�[������������
    DWCi_String oldNick = s_iLobby->FindUser(DWCi_Lobby::NickNameToPid(user));
    
    // �Y���j�b�N�l�[�������̃`�����l���ɂ��邩�T���BRemoveUser�ō폜������ő��݂��������m�F����B
    if(s_iLobby->RemoveUser(oldNick, channel))
    {
        // �Đڑ����[�U�������̂ň�[�ؒf�R�[���o�b�N���Ă�
        const DWCi_LobbyChannelCallbacks* callbacks = s_iLobby->GetChannelCallbacks(channel);
        DWC_ASSERTMSG(callbacks, "UserJoined: Internal error. No callback info. channel: %s", channel);
        callbacks->lobbyPlayerDisconnectedCallback(s_iLobby->UnmodifyChannelName(channel).c_str(), DWCi_Lobby::NickNameToPid(user), callbacks->param);
    }
    
    // ���[�U��ǉ�
    s32 userId = s_iLobby->AddUser(user, channel);

    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        //DWC_ASSERTMSG(FALSE, "UserJoined: s_iLobby->AddUser failed.");
        DWC_Printf(DWC_REPORTFLAG_WARNING, "UserJoined: ignored unrecognized user: %s\n", user);
        return;
    }
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "%s joined %s\n", user, channel);

    if(s_iLobby->IsChannelActivated(channel))
    {
        // �`�����l�����L���Ȃ���(JoinChannel�R�[���o�b�N���Ă񂾂���)�̂ݐڑ��R�[���o�b�N���Ă�
        const DWCi_LobbyChannelCallbacks* callbacks = s_iLobby->GetChannelCallbacks(channel);
        DWC_ASSERTMSG(callbacks, "UserJoined: Internal error. No callback info. channel: %s", channel);
        callbacks->lobbyPlayerConnectedCallback(s_iLobby->UnmodifyChannelName(channel).c_str(), userId, callbacks->param);
    }
	(void)chat;
	(void)param;
	(void)mode;
}

static void UserParted(CHAT chat, const char* channel, const char* user, int why, const char* reason, const char* kicker, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	switch(why)
    {
    case CHAT_LEFT:
		DWC_Printf(DWC_REPORTFLAG_INFO, "%s left %s\n", user, channel);
        break;
	case CHAT_QUIT:
		DWC_Printf(DWC_REPORTFLAG_INFO, "%s quit: %s\n", user, reason);
        break;
	case CHAT_KICKED:
		DWC_Printf(DWC_REPORTFLAG_INFO, "%s was kicked from %s by %s: %s\n", user, channel, kicker, reason);
        break;
	case CHAT_KILLED:
		DWC_Printf(DWC_REPORTFLAG_INFO, "%s was killed: %s\n", user, reason);
        break;
	default:
		DWC_Printf(DWC_REPORTFLAG_INFO, "UserParted() called with unknown part-type\n");
        break;
    }
	
    // userId���m�F
    s32 userId = s_iLobby->FindUser(user);

    // ���łɊY�����[�U���Đڑ��ς݂Ŗ��O���ω����ă��X�g�ɑ��݂��Ȃ��Ƃ��͐ؒf�R�[���o�b�N�͌Ă΂Ȃ�
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
		DWC_Printf(DWC_REPORTFLAG_INFO, "Disconnected reconnected user.: %s\n", user);
        return;
    }
    
    // �R�[���o�b�N���Ă�
    const DWCi_LobbyChannelCallbacks* callbacks = s_iLobby->GetChannelCallbacks(channel);
    DWC_ASSERTMSG(callbacks, "UserParted: Internal error. No callback info. channel: %s", channel);
    callbacks->lobbyPlayerDisconnectedCallback(s_iLobby->UnmodifyChannelName(channel).c_str(), userId, callbacks->param);

    // ���[�U���폜
    s_iLobby->RemoveUser(user, channel);
	(void)chat;
	(void)reason;
	(void)kicker;
	(void)param;
}

static void UserChangedNick(CHAT chat, const char* channel, const char* oldNick, const char* newNick, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "%s changed nicks to %s\n", oldNick, newNick);
	
    // nick��ύX���邱�Ƃ͂Ȃ����O�̂��ߑΉ�
    
    // ���[�U��ID�͂��̂܂܂Ŗ��O��ύX
    s_iLobby->RenameUser(oldNick, newNick);
    
	(void)chat;
	(void)channel;
	(void)param;
}

static void UserModeChanged(CHAT chat, const char* channel, const char* user, int mode, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
    DWCi_String modeStr;
	switch(mode)
    {
    case CHAT_NORMAL:
        modeStr = "normal";
        break;
    case CHAT_VOICE:
        modeStr = "voice";
        break;
	case CHAT_OP:
        modeStr = "ops";
        break;
    case (CHAT_VOICE | CHAT_OP):
		modeStr = "voice+ops";
        break;
    default:
        modeStr = "invalid";
        break;
    }
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "%s's new mode in %s is %s\n", user, channel, modeStr.c_str());;
	
	(void)chat;
	(void)channel;
	(void)user;
	(void)param;
}

static void TopicChanged(CHAT chat, const char* channel, const char* topic, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "The topic in %s changed to %s\n", channel, topic);
	
    const DWCi_LobbyChannelCallbacks* callbacks = s_iLobby->GetChannelCallbacks(channel);
    DWC_ASSERTMSG(callbacks, "TopicChanged: Internal error. No callback info. channel: %s", channel);
    callbacks->lobbyTopicChangedCallback(s_iLobby->UnmodifyChannelName(channel).c_str(), topic, callbacks->param);
    
	(void)chat;
	(void)channel;
	(void)topic;
	(void)param;
}

static void ChannelModeChanged(CHAT chat, const char* channel, CHATChannelMode* mode, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "The mode in %s has changed:\n", channel);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   InviteOnly: %d\n", mode->InviteOnly);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   Private: %d\n", mode->Private);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   Secret: %d\n", mode->Secret);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   Moderated: %d\n", mode->Moderated);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   NoExternalMessages: %d\n", mode->NoExternalMessages);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   OnlyOpsChangeTopic: %d\n", mode->OnlyOpsChangeTopic);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   OpsObeyChannelLimit: %d\n", mode->OpsObeyChannelLimit);
	if(mode->Limit == 0)
    {
		DWC_Printf(DWC_REPORTFLAG_INFO, "   Limit: N/A\n");
	}
    else
    {
		DWC_Printf(DWC_REPORTFLAG_INFO, "   Limit: %d\n", mode->Limit);
	}
	(void)chat;
	(void)channel;
	(void)param;
}

static void UserListUpdated(CHAT chat, const char* channel, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "User list updated\n");
	
	(void)chat;
	(void)channel;
	(void)param;
}

static void ConnectCallback(CHAT chat, CHATBool success, int failureReason, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	if (success == CHATFalse)
    {
		DWC_Printf(DWC_REPORTFLAG_ERROR, "Failed to connect (%d)\n", failureReason);
        s_iLobby->SetError(DWCi_LOBBY_ERROR_SESSION);
    }
	else
    {
		DWC_Printf(DWC_REPORTFLAG_INFO, "Connected\n");
        s_iLobby->SetState(DWCi_LOBBY_STATE_CONNECTINGWAIT);
    }
	(void)chat;
	(void)success;
	(void)failureReason;
	(void)param;
}


static void FillInUserCallback(CHAT chat, unsigned int IP, char user[128], void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

    strncpy(user, s_iLobby->GetChatUser().c_str(), 128 - 1);

	(void)chat;
	(void)IP;
	(void)param;
}

static void NickErrorCallback(CHAT chat, int type, const char* nick, int numSuggestedNicks, const char** suggestedNicks, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

	if(type == CHAT_IN_USE)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "The nick %s is already being used.\n", nick);
		s_iLobby->SetChatNick(DWCi_Lobby::CreateNickName(s_iLobby->GetPid()));
		chatRetryWithNick(chat, s_iLobby->GetChatNick().c_str());
	}
	else if(type == CHAT_INVALID)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "The nick %s is invalid!\n", nick);
		// chatDisconnect(chat); THIS CRASHES
	
		// 10-14-2004: Added By Saad Nader
		// this is necessary as the function will fail if a new nick is not retries.
		////////////////////////////////////////////////////////////////////////////
		s_iLobby->SetChatNick(DWCi_Lobby::CreateNickName(s_iLobby->GetPid()));
		chatRetryWithNick(chat, s_iLobby->GetChatNick().c_str());
	}
	else if((type == CHAT_UNIQUENICK_EXPIRED) || (type == CHAT_NO_UNIQUENICK))
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "This account has no uniquenick or an expired uniquenick!\n");

		s_iLobby->SetChatNick(DWCi_Lobby::CreateNickName(s_iLobby->GetPid()));
		chatRegisterUniqueNick(chat, 2, s_iLobby->GetChatNick().c_str(), "");
	}
	else if(type == CHAT_INVALID_UNIQUENICK)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "The uniquenick %s is invalid or in use\n", nick);
		DWC_Printf(DWC_REPORTFLAG_INFO, "There are %d suggested nicks:\n", numSuggestedNicks);

		for(int i = 0 ; i < numSuggestedNicks ; i++)
			DWC_Printf(DWC_REPORTFLAG_INFO, "   %s\n", suggestedNicks[i]);
	}

	// 10-14-2004: Added By Saad Nader
	// added for the addition of a new error code.
	////////////////////////////////////////////////////////////////////////////
	else if(type == CHAT_NICK_TOO_LONG)
	{
        DWC_ASSERTMSG( FALSE, "NickErrorCallback: Internal error. Nick name is too long." );
		s_iLobby->SetChatNick(DWCi_Lobby::CreateNickName(s_iLobby->GetPid()));
		chatRetryWithNick(chat, s_iLobby->GetChatNick().c_str());
	}
	(void)nick;
	(void)suggestedNicks;
	(void)param;
}

static void GetUserInfoCallback(CHAT chat, CHATBool success, const char* nick, const char* user, const char* name, const char* address,
                                int numChannels, const char** channels, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	if(!success)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "GetUserInfo failed\n");
		return;
	}

	DWC_Printf(DWC_REPORTFLAG_INFO, "%s's Info:\n", nick);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   User: %s\n", user);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   Name: %s\n", name);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   Address: %s\n", address);
	DWC_Printf(DWC_REPORTFLAG_INFO, "   Channels (%d):\n", numChannels);
	for(int i = 0 ; i < numChannels ; i++)
    {
		DWC_Printf(DWC_REPORTFLAG_INFO, "      %s\n", channels[i]);
	}
    
	(void)chat;	
	(void)nick;	
	(void)user;	
	(void)name;	
	(void)address;	
	(void)numChannels;	
	(void)channels;
	(void)param;
}

static void EnumChannelsAllCallback(CHAT chat, CHATBool success, int numChannels, const char** channels, const char** topics, int* numUsers, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

    std::vector<char*, DWCi_Allocator<char*> > result = s_iLobby->UnmodifyChannelNames(numChannels, channels);

    // �R�[���o�b�N���Ă�
    DWCi_Callback<DWCi_LobbyEnumChannelsCallback>* callbackObj = s_iLobby->GetCallbackManager().FindCallback<DWCi_LobbyEnumChannelsCallback>((u32)param);
    callbackObj->GetCallback()(success == CHATTrue, numChannels, (const char**)DWCi_GetVectorBuffer(result),
                               topics, (s32*)numUsers, callbackObj->GetParam());
    s_iLobby->GetCallbackManager().RemoveCallback((u32)param);

	(void)chat;
}

static void EnumChannelsEachCallback(CHAT chat, CHATBool success, int index, const char* channel, const char* topic, int numUsers, void* param )
{
	if(!success)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "EnumChannels failed\n");
		return;
	}

    DWC_Printf(DWC_REPORTFLAG_INFO, "ChatEnumChannelsCallbackEach called. index:%d\n", index);
    DWC_Printf(DWC_REPORTFLAG_INFO, "    Channel: %s\n", channel);
    DWC_Printf(DWC_REPORTFLAG_INFO, "    Topic: %s\n", topic);
    DWC_Printf(DWC_REPORTFLAG_INFO, "    NumUser: %d\n", numUsers);
    
	(void)param;
	(void)index;
	(void)channel;
	(void)topic;
	(void)numUsers;
	(void)chat;
}

static void EnumUsersCallback(CHAT chat, CHATBool success, const char* channel, int numUsers, const char** users, int* modes, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

    std::vector<s32, DWCi_Allocator<s32> > userIds;
    
    if(numUsers != 0)
    {
        userIds = s_iLobby->AddUsers(numUsers, users, channel);
    }
    
    

    // �R�[���o�b�N���Ă�
    DWCi_Callback<DWCi_LobbyEnumUsersCallback>* callbackObj = s_iLobby->GetCallbackManager().FindCallback<DWCi_LobbyEnumUsersCallback>((u32)param);
    callbackObj->GetCallback()(success == CHATTrue, s_iLobby->UnmodifyChannelName(channel).c_str(), (s32)userIds.size(),
                               DWCi_GetVectorBuffer(userIds), callbackObj->GetParam());
    s_iLobby->GetCallbackManager().RemoveCallback((u32)param);
    (void)chat;
    (void)modes;
}

static void EnumUsersAfterEnteredCallback(CHAT chat, CHATBool success, const char* channel, int numUsers, const char** users, int* modes, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
    if(success == CHATTrue)
    {
        s_iLobby->AddUsers(numUsers, users, channel);
    }

    // �`�����l����L����
    s_iLobby->ActivateChannel(channel);

    // �����R�[���o�b�N���Ă�
    const DWCi_Callback<DWCi_LobbyJoinChannelCallback>* callbackObj =
        s_iLobby->GetCallbackManager().FindCallback<DWCi_LobbyJoinChannelCallback>((u32)param);
    callbackObj->GetCallback()(success == CHATTrue, DWCi_LOBBY_ENTER_RESULT_SUCCESS, s_iLobby->UnmodifyChannelName(channel).c_str(), callbackObj->GetParam());
    s_iLobby->GetCallbackManager().RemoveCallback((u32)param);

    if(success == CHATFalse)
    {
        // ���s�����̂ŕ������o��B
        DWCi_LobbyLeaveChannel(s_iLobby->UnmodifyChannelName(channel).c_str());
    }
    (void)chat;
    (void)modes;
}

static void EnterChannelCallback(CHAT chat, CHATBool success, CHATEnterResult result, const char* channel, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
    if(success == CHATTrue)
    {
        // JoinChannelLimitAsync�̎��Ɏw�肵���f�t�H���g�̐l�����擾
        s32 limit = s_iLobby->GetDefaultChannelLimit(channel);
    
        // DWCi_LobbyJoinChannelLimitAsync��Join�������̊m�F
        if(limit != 0)
        {
            // ���݂̐l�����擾
            s32 currentNum = chatGetChannelNumUsers(s_iLobby->GetChat(), channel);
    
            // ���݂̐l���������l���I�[�o�[���Ă�����ގ�����B�����O��Leave�����Ƃ�(limit==-1)�ƁA
            // �����ł��Ă��Ȃ��Ƃ�(currentNum==-1)���O�̂��ߑގ�(DWCi_LobbyLeaveChannel)����)
            if(limit == -1)
            {
                success = CHATFalse;
                result = CHATEnterTimedOut; // �Y�����闝�R���Ȃ��̂Ń^�C���A�E�g�ɂ���B
            }
            else if(currentNum > limit || currentNum == -1)
            {
                success = CHATFalse;
                result = CHATChannelIsFull;
            }
            else
            {
                // �I�[�o�[���Ă��Ȃ�������l��������������
                DWC_Printf(DWC_REPORTFLAG_INFO, "SetChannelLimit channel: %s, limit: %d\n", channel, limit);
                chatSetChannelLimit(s_iLobby->GetChat(), channel, limit);
            }
            DWC_Printf(DWC_REPORTFLAG_INFO, "CHATEnterResult: %d, currentNum %d\n", result, currentNum);
        }
    
    }
    
    // ���̃��C�u�������œ������s������ꍇ������̂ōēx�������������
	if (success == CHATTrue)
    {
        // �������g�����[�U�ɒǉ�
        s32 userId = s_iLobby->AddUser(s_iLobby->GetChatNick(), channel);
    
        // �S�Ẵ��[�UID���擾(��ɑS�����̃��[�UID���擾���Ă����K�v������)
        chatEnumUsers(chat, channel, EnumUsersAfterEnteredCallback, param, CHATFalse);
    }
    else
    {
        // �`�����l�������폜(�Q�����Ă��Ȃ��`�����l���ɑ΂���Leave���Ă������N����Ȃ��̂ŁA�{���ɓ������s�����ꍇ�ł��Ă�ŗǂ�)
        // �{���ɓ������s�����ꍇ�ɉ��ʃ��C�u�������炻�̑��̃R�[���o�b�N���Ă΂�邱�Ƃ�����̂ł�����h�~�B
        DWCi_LobbyLeaveChannel(s_iLobby->UnmodifyChannelName(channel).c_str());
        
        // �������s�R�[���o�b�N���Ă�
        const DWCi_Callback<DWCi_LobbyJoinChannelCallback>* callbackObj =
            s_iLobby->GetCallbackManager().FindCallback<DWCi_LobbyJoinChannelCallback>((u32)param);
        callbackObj->GetCallback()(FALSE, (DWCi_LOBBY_ENTER_RESULT)result, s_iLobby->UnmodifyChannelName(channel).c_str(), callbackObj->GetParam());
        s_iLobby->GetCallbackManager().RemoveCallback((u32)param);
    }
	(void)chat;
	(void)result;
	(void)channel;
}

static void GetChannelKeysCallback(CHAT chat, CHATBool success, const char* channel, const char* user, int num, const char** keys, const char** values, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
    // channel��success == CHATFalse�̂Ƃ���NULL�ɂȂ�ꍇ������B
    if(!channel)
    {
        //DWC_ASSERTMSG( channel, "GetChannelKeysCallback: channel is NULL. Please check excess flood.");
        DWC_Printf( DWC_REPORTFLAG_WARNING, "GetChannelKeysCallback: channel is NULL. success: %d, num: %d.", success, num);
        DWC_Printf( DWC_REPORTFLAG_WARNING, "                      : channel: %s, user: %s.", channel ? channel : "NULL", user ? user : "NULL");
        return;
    }
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "GetChannelKeysCallback called\n");
    
    s32 userId;

    if(user)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "Channel user key changed. User:%s\n", user);
        // userId���m�F
        userId = s_iLobby->FindUser(user);
    }
    else
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "Channel key changed\n");
        userId = DWC_LOBBY_INVALID_USER_ID;
    }

    // �R�[���o�b�N�����o��
    DWCi_Callback<DWCi_LobbyGetChannelDataCallback>* callbackObj = s_iLobby->GetCallbackManager().FindCallback<DWCi_LobbyGetChannelDataCallback>((u32)param);
    
    // �R�[���o�b�N���Ă�
    char* key = NULL;
    char* value = NULL;
    if(success == CHATTrue && num > 0)
    {
        key = (char*)keys[0];
        value = (char*)values[0];
    }
    callbackObj->GetCallback()(success == CHATTrue, s_iLobby->UnmodifyChannelName(channel).c_str(),
                               userId, key, value, callbackObj->GetParam());
    
    // �R�[���o�b�N���폜����
    s_iLobby->GetCallbackManager().RemoveCallback((u32)param);

    for(int i=0; i<num; i++)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "index:%d, Key:%s, Value:%s\n", i, keys[i], values[i]);
    }
    (void)chat;
}

static void BroadcastKeyChanged(CHAT chat, const char* channel, const char* user, const char* key, const char* value, void* param )
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "BroadcastKeyChanged called\n");
    
    s32 userId;

    if(user)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "Channel user key changed. User:%s\n", user);
        // userId���m�F
        userId = s_iLobby->FindUser(user);
    }
    else
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "Channel key changed\n", user);
        userId = DWC_LOBBY_INVALID_USER_ID;
    }
    DWC_Printf(DWC_REPORTFLAG_INFO, "Key:%s, Value:%s\n", key, value);
    
    const DWCi_LobbyChannelCallbacks* callbacks = s_iLobby->GetChannelCallbacks(channel);
    DWC_ASSERTMSG(callbacks, "BroadcastKeyChanged: Internal error. No callback info. channel: %s", channel);
    callbacks->lobbyBroadcastDataCallback(s_iLobby->UnmodifyChannelName(channel).c_str(), userId, key, value, callbacks->param);
    
    (void)chat;
    (void)param;
}

static void GetUserModeCallback(CHAT chat, CHATBool success, const char* channel, const char* user, int mode, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);

	DWC_Printf(DWC_REPORTFLAG_INFO, "chatGetUserModeCallback called\n");

	if(success == CHATTrue)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "name:%s, mode:%d\n", user, mode);
	}
	else
	{
		DWC_Printf(DWC_REPORTFLAG_ERROR, "chatGetUserModeCallback failed\n");
	}

	(void)param;
	(void)channel;
	(void)user;
	(void)mode;
	(void)chat;
}

static void GetGlobalKeysCallback(CHAT chat, CHATBool success, const char* user, int num, const char** keys, const char** values, void* param )
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "chatGetGlobalKeysCallback called\n");

	if(!success)
	{
		DWC_Printf(DWC_REPORTFLAG_INFO, "GetGlobalKeys failed\n");
		return;
	}

    if(user)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "Global user key changed. User:%s\n", user);
    }
    else
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "Global key changed\n", user);
    }
    
    for(int i=0; i<num; i++)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "index:%d, Key:%s, Value:%s\n", i, keys[i], values[i]);
    }
    
	(void)chat;
	(void)param;
	(void)user;
	(void)keys;
	(void)values;
}

static void GetChannelModeCallback(CHAT chat, CHATBool success, const char* channel, CHATChannelMode* mode, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	if(success)
	{
	    ChannelModeChanged(chat, channel, mode, param);
    }
    else
    {
		DWC_Printf(DWC_REPORTFLAG_ERROR, "GetChannelModeCallback failed\n");
		return;
	}
    
    // �R�[���o�b�N���Ă�
    DWCi_Callback<DWCi_LobbyGetChannelModeCallback>* callbackObj = s_iLobby->GetCallbackManager().FindCallback<DWCi_LobbyGetChannelModeCallback>((u32)param);
    callbackObj->GetCallback()(success == CHATTrue, s_iLobby->UnmodifyChannelName(channel).c_str(),
                               (DWCi_LobbyChannelMode*)mode, callbackObj->GetParam());
    s_iLobby->GetCallbackManager().RemoveCallback((u32)param);

    (void)chat;
}

static void GetChannelBasicUserInfoCallback(CHAT chat, CHATBool success, const char* channel, const char* nick, const char* user, const char* address, void* param)
{
    DWC_ASSERTMSG( s_iLobby, "s_iLobby: Invalid state. s_iLobby is NULL." );
    RETURN_IF_ERROR_STATE((void)0);
    
	DWC_Printf(DWC_REPORTFLAG_INFO, "GetChannelBasicUserInfo called\n");

	if(success == CHATFalse)
	{
		DWC_Printf(DWC_REPORTFLAG_ERROR, "GetChannelBasicUserInfo failed\n");
		return;
	}

	DWC_Printf(DWC_REPORTFLAG_INFO, "Basic info Nick:%s, User:%s, Addr:%s\n", nick, user, address);
    (void)chat;
    (void)channel;
    (void)nick;
    (void)user;
    (void)address;
    (void)param;
}

/**
 * @brief ���r�[���C�u�����̏�����i�߂܂��B
 * 
 * ���r�[���C�u�����̏�����i�߂܂��B\n
 * ���Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B\n
 * ::DWCi_LOBBY_STATE_ERROR��Ԃ����Ƃ���::DWCi_LobbyGetLastError�֐��ŃG���[���e���擾��::DWCi_LobbyShutdownAsync�֐����Ă�ŃV���b�g�_�E�����Ă��������B\n
 * ::DWCi_LOBBY_STATE_CONNECTED�̂Ƃ��̂݃��b�Z�[�W���M�A�`�����l�������Ȃǂ̊֐����Ăׂ܂��B
 * 
 * @retval ::DWCi_LOBBY_STATE_NOTINITIALIZED ���r�[���C�u����������������Ă��܂���B
 * @retval ::DWCi_LOBBY_STATE_CONNECTING �T�[�o�ɐڑ����J�n���Ă��܂��B
 * @retval ::DWCi_LOBBY_STATE_CONNECTINGWAIT �T�[�o�ɐڑ����ł��B
 * @retval ::DWCi_LOBBY_STATE_CONNECTED �T�[�o�ɐڑ����܂����B�e��֐����Ăׂ܂��B
 * @retval ::DWCi_LOBBY_STATE_CLOSING ���r�[���C�u�������I�����ł��B
 * @retval ::DWCi_LOBBY_STATE_ERROR ���r�[���C�u����������������Ă��܂���B
 */
DWCi_LOBBY_STATE DWCi_LobbyProcess()
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_STATE_NOTINITIALIZED);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_STATE_ERROR);
    
    if(s_iLobby->GetChat())
    {
        // �����Ŋe��R�[���o�b�N���Ă΂��
        chatThink(s_iLobby->GetChat());
    }
    
    // �R�[���o�b�N���ɒv���I�ȃG���[���������Ă��Ȃ����`�F�b�N
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_STATE_ERROR);
    
    // ���g���C�J�E���g���I�[�o�[���Ă���Ƃ��̓Z�b�V�����G���[��Ԃ�
    if(s_iLobby->GetRetryCount() >= DWCi_MAX_RETRY_COUNT)
    {
        s_iLobby->ResetRetryCount();
        s_iLobby->SetError(DWCi_LOBBY_ERROR_SESSION);
        return s_iLobby->GetState();
    }
    
    // ������i�߂�
    switch(s_iLobby->GetState())
    {
    case DWCi_LOBBY_STATE_NOTINITIALIZED:
        DWC_ASSERTMSG(FALSE, "DWCi_LobbyProcess: Invalid state: NOTINITIALIZED");
        break;
    case DWCi_LOBBY_STATE_CONNECTING:
        break;
    case DWCi_LOBBY_STATE_CONNECTINGWAIT:
        s_iLobby->WaitAfterConnect();
        break;
    case DWCi_LOBBY_STATE_CONNECTED:
        break;
    case DWCi_LOBBY_STATE_CLOSING:
        break;
    default:
        DWC_ASSERTMSG(FALSE, "DWCi_LobbyProcess: Invalid state");
        break;
    }
    
    return s_iLobby->GetState();
}

/**
 * @brief �������Ă���v���I�ȃG���[�̏����擾���܂��B
 * 
 * �������Ă���v���I�ȃG���[�̏����擾���܂��B
 * 
 * @retval ::DWCi_LOBBY_ERROR_NONE �G���[�����B
 * @retval ::DWCi_LOBBY_ERROR_UNKNOWN �s���ȃG���[�B
 * @retval ::DWCi_LOBBY_ERROR_ALLOC �������m�ۂɎ��s�B
 * @retval ::DWCi_LOBBY_ERROR_SESSION �ʐM�G���[�B
 */
DWCi_LOBBY_ERROR DWCi_LobbyGetLastError()
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_ERROR_NONE);
    
    return s_iLobby->GetError();
}

/**
 * @brief ���������G���[(::DWCi_LOBBY_ERROR)����\�����ׂ��G���[�R�[�h�𓾂܂��B
 * 
 * ::DWCi_LOBBY_ERROR����\�����ׂ��G���[�R�[�h�ɕϊ����܂��B
 * 
 * @param err ���������G���[�B
 * 
 * @retval �\�����ׂ��G���[�R�[�h�B
 */
s32 DWCi_LobbyToErrorCode(DWCi_LOBBY_ERROR err)
{
    return err + DWCi_LOBBY_ERROR_CODE_BASE;
}

/**
 * @brief ���r�[���C�u�����Ŏg�p���Ă��鎩���̃��[�UID���擾���܂��B
 * 
 * ���r�[���C�u�����Ŏg�p���Ă��鎩���̃��[�UID���擾���܂��B
 * 
 * @retval �����̃��[�UID�B
 * @retval ::DWC_LOBBY_INVALID_USER_ID ���������Ă��܂���B
 */
s32 DWCi_LobbyGetMyUserId()
{
    RETURN_IF_NOTINITIALIZED(DWC_LOBBY_INVALID_USER_ID);
    
    return s_iLobby->GetPid();
}

/**
 * @brief �w�肵���`�����l���ɎQ�����Ă��邩�𒲂ׂ܂��B
 * 
 * �w�肵���`�����l���ɎQ�����Ă��邩�𒲂ׂ܂��B
 * 
 * @param[in] channelName ���ׂ�`�����l����
 * 
 * @retval TRUE �Q�����Ă���B
 * @retval FALSE �Q�����Ă��Ȃ��B
 */
BOOL DWCi_LobbyInChannel(const char* channelName)
{
    RETURN_IF_NOTINITIALIZED(FALSE);
    
    //s_iLobby->IsChannelActivated(s_iLobby->ModifyChannelName(channelName).c_str());
    return chatInChannel(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str()) == CHATTrue;
}

/** 
 * @brief ���r�[���C�u���������������܂��B
 * 
 * ���r�[���C�u���������������܂��B\n
 * ���̊֐����ĂԑO�ɂ��炩����DWC_LoginAsync�֐�����DWC_NASLoginAsync�֐���Wi-Fi�R�l�N�V�����ւ̐ڑ����������Ă���K�v������܂��B\n
 * ���̊֐������������ꍇ��::DWCi_LobbyProcess�֐��𖈃Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B
 * 
 * @param[in] gameName NULL�I�[���ꂽ�Q�[�����B
 * @param[in] secretKey NULL�I�[���ꂽ�V�[�N���b�g�L�[�B
 * @param[in] prefix �`�����l�����ɕt������v���t�B�b�N�X�B���i�p�A�f�o�b�O�p�A�e�X�g�p�œ���`�����l���𕪂��邱�Ƃ��ł��܂��B\n
 *                   ���i�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_RELEASE���A�f�o�b�O�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_DEBUG���w�肵�Ă��������B
 * @param[in] globalCallbacks �`�����l���Ɋւ��Ȃ��R�[���o�b�N�֐����Z�b�g���܂��B
 * @param[in] dwcUserData �L����DWC���[�U�f�[�^�I�u�W�F�N�g�B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE ���łɏ���������Ă��܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_ALLOC �������m�ۂɎ��s�B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_SESSION �ʐM�G���[�B
 */
DWCi_LOBBY_RESULT DWCi_LobbyInitializeAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                            const DWCi_LobbyGlobalCallbacks* globalCallbacks, const DWCUserData* dwcUserData)
{
	// ���[�U�f�[�^�̐�����������
	if( !DWC_CheckUserData( dwcUserData )
		|| !DWC_CheckHasProfile( dwcUserData ))
    {
		// �s���ȃ��[�U�f�[�^
        DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_LobbyInitializeAsync: Invalid dwcUserData.\n");
		return DWCi_LOBBY_RESULT_ERROR_PARAM;
	}
    return DWCi_LobbyInitializePidAsync(gameName, secretKey, prefix, globalCallbacks, dwcUserData->gs_profile_id);
}

/** 
 * @brief ���r�[���C�u���������������܂��B(�f�o�b�O�p)
 * 
 * ���r�[���C�u���������������܂��B(�f�o�b�O�p)\n
 * ���̊֐����ĂԑO�ɂ��炩����DWC_LoginAsync�֐�����DWC_NASLoginAsync�֐���Wi-Fi�R�l�N�V�����ւ̐ڑ����������Ă���K�v������܂��B\n
 * ::DWCi_LobbyInitializeAsync�֐���DWCUserData���ɋL�^����Ă���GameSpyProfileID���g�p����̂ɑ΂��A������͔C�ӂ�GameSpyProfileID�ŏ������ł��܂��B\n
 * ���̊֐������������ꍇ��::DWCi_LobbyProcess�֐��𖈃Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B
 * 
 * @param[in] gameName NULL�I�[���ꂽ�Q�[�����B
 * @param[in] secretKey NULL�I�[���ꂽ�V�[�N���b�g�L�[�B
 * @param[in] prefix �`�����l�����ɕt������v���t�B�b�N�X�B���i�p�A�f�o�b�O�p�A�e�X�g�p�œ���`�����l���𕪂��邱�Ƃ��ł��܂��B\n
 *                   ���i�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_RELEASE���A�f�o�b�O�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_DEBUG���w�肵�Ă��������B
 * @param[in] globalCallbacks �`�����l���Ɋւ��Ȃ��R�[���o�b�N�֐����Z�b�g���܂��B
 * @param[in] pid GameSpyProfileID�B10���ȏ�̐��̐����w�肵�Ă��������B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE ���łɏ���������Ă��܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_ALLOC �������m�ۂɎ��s�B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_SESSION �ʐM�G���[�B
 */
DWCi_LOBBY_RESULT DWCi_LobbyInitializePidAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                               const DWCi_LobbyGlobalCallbacks* globalCallbacks, s32 pid)
{
    // ��ԃ`�F�b�N
    if(s_iLobby)
    {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_LobbyInitializePidAsync: Already initialized.\n");
        return DWCi_LOBBY_RESULT_ERROR_STATE;
    }
    
	DWCi_String chatNick(DWCi_Lobby::CreateNickName(pid));
	DWCi_String chatUser("DWCUser");
	DWCi_String chatName("DWCName");
    CHAT chat = NULL;

    // �������r�[�f�[�^������
    s_iLobby = new DWCi_Lobby(gameName, secretKey, prefix, pid, chatNick, chatUser, chatName, globalCallbacks);
    if(!s_iLobby)
    {
        return DWCi_LOBBY_RESULT_ERROR_ALLOC;
    }
    
    
    DWCi_LOBBY_RESULT result = DWCi_LOBBY_RESULT_SUCCESS;

    // new�n���h��������
    if(!DWCi_LobbyInitializeNewHandler(s_iLobby))
    {
        result = DWCi_LOBBY_RESULT_ERROR_ALLOC;
        goto ERROR_ALLOC;
    }
    
	chatGlobalCallbacks gsGlobalCallbacks;
	DWCi_Np_CpuClear8(&gsGlobalCallbacks, sizeof(chatGlobalCallbacks));
	gsGlobalCallbacks.raw = Raw;
	gsGlobalCallbacks.disconnected = Disconnected;
	gsGlobalCallbacks.privateMessage = PrivateMessage;
	gsGlobalCallbacks.invited = Invited;
	gsGlobalCallbacks.param = globalCallbacks->param;

    DWC_Printf(DWC_REPORTFLAG_INFO, "chatConnectSecure start\n");
	/*
    // �Â��o�[�W�����͈����̎d�l���Ⴄ
    chat = chatConnectSecure(serverAddress[0]?serverAddress:NULL, port, chatNick, chatName,
                                  gamename, secretKey, &gsGlobalCallbacks, NickErrorCallback, FillInUserCallback, ConnectCallback, NULL, CHATFalse);
    */
	chat = chatConnectSecure((s_iLobby->GetGameName() + "." + DWCi_LOBBY_SERVER_DOMAIN).c_str(), 6667, s_iLobby->GetChatNick().c_str(), s_iLobby->GetChatName().c_str(),
                                     s_iLobby->GetGameName().c_str(), s_iLobby->GetSecretKey().c_str(), &gsGlobalCallbacks,
                                     NickErrorCallback, FillInUserCallback, ConnectCallback, globalCallbacks->param, CHATFalse);
	if(!chat)
	{
		DWC_Printf(DWC_REPORTFLAG_ERROR, "chatConnectSecure failed\n");
		result = DWCi_LOBBY_RESULT_ERROR_SESSION;
        goto ERROR_ALLOC;
	}
    s_iLobby->SetChat(chat);
    s_iLobby->SetState(DWCi_LOBBY_STATE_CONNECTING);
    
    return DWCi_LOBBY_RESULT_SUCCESS;
    
ERROR_ALLOC:
    DWCi_LobbyDestroyNewHandler();
    DWC_SAFE_DELETE(s_iLobby);
    return result;
}

/** 
 * @brief ���r�[���C�u�������I�����܂��B
 * 
 * ���r�[���C�u�������I�����܂��B\n
 * �C�ӂ̃^�C�~���O�ŌĂԂ��Ƃ��ł��܂��B���łɏI�����Ă���ꍇ�͉������܂���B\n
 * ::DWCi_LobbyProcess�֐���::DWCi_LOBBY_STATE_NOTINITIALIZED��Ԃ��ƃV���b�g�_�E�������ł��B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 */
DWCi_LOBBY_RESULT DWCi_LobbyShutdownAsync()
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_SUCCESS);
    
    // ���łɔ\���I�ɏI�����BDiconnect�R�[���o�b�N�҂�
    if(s_iLobby->GetState() == DWCi_LOBBY_STATE_CLOSING)
    {
        return DWCi_LOBBY_RESULT_SUCCESS;
    }
    
    // new�n���h���I��
    DWCi_LobbyDestroyNewHandler();
    
    if(s_iLobby->GetServerDisconnectedMe())
    {
        // �T�[�o������ؒf�����Disconnected�R�[���o�b�N���Ă΂ꂽ�Ƃ��̏ꍇ
        // GS�`���b�g���C�u�������I������B���̂Ƃ���Disconnected�R�[���o�b�N�͌Ă΂�Ȃ��B
        chatDisconnect(s_iLobby->GetChat());
        DWC_SAFE_DELETE(s_iLobby);
    }
    else
    {
        // �\���I�ɏI������ꍇ
        s_iLobby->SetState(DWCi_LOBBY_STATE_CLOSING);
        chatDisconnect(s_iLobby->GetChat());
        // Disconnected�R�[���o�b�N��s_iLobby���폜
    }
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/** 
 * @brief �`�����l���ɎQ�����܂��B
 * 
 * �`�����l���ɎQ�����܂��B�Q���������܂��͎��s�����::DWCi_LobbyJoinChannelCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] channelName �Q������`�����l���̖��O�B�I�[���݂�::DWC_LOBBY_MAX_CHANNEL_NAME_SIZE�ȉ��̒����ł���K�v������܂��B\n
 *                        ���ۂɎQ������`�����l�����́u\#GSP!(gameName)!(::DWC_LOBBY_CHANNEL_PREFIX)!(channelName)�v�ƂȂ�܂��B\n
 *                        ��F�Q�[������"dwctest"�A::DWC_LOBBY_CHANNEL_PREFIX��::DWC_LOBBY_CHANNEL_PREFIX_DEBUG�A�`�����l������"TestChannel"�̏ꍇ�A
 *                        �u\#GSP!dwctest!D!TestChannel�v�ƂȂ�܂��B
 * @param[in] password �`�����l���̃p�X���[�h�B::DWCi_LobbySetChannelPassword�֐��ŃZ�b�g���ꂽ�p�X���[�h���w�肵�Ă��������B
 *                     �p�X���[�h���ݒ肳��Ă��Ȃ��ꍇ�͖�������܂��B
 * @param[in] channelCallbacks �`�����l���Ɋւ���R�[���o�b�N�֐����Z�b�g���܂��B
 * @param[in] callback �`�����l���ɎQ���܂��͎��s�����Ƃ��ɌĂ΂��R�[���o�b�N�B
 * @param[in] param callback��param�p�����[�^�ɂ��̂܂ܓn����郆�[�U��`���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyJoinChannelAsync(const char* channelName, const char* password, const DWCi_LobbyChannelCallbacks* channelCallbacks,
                                             DWCi_LobbyJoinChannelCallback callback, void* param)
{
    return DWCi_LobbyJoinChannelLimitAsync(channelName, password, 0, channelCallbacks, callback, param);
}

/** 
 * @brief �����l�����w�肵�ă`�����l���ɎQ�����܂��B
 * 
 * �`�����l���ɎQ�����܂��B�Q���������܂��͎��s�����::DWCi_LobbyJoinChannelCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] channelName �Q������`�����l���̖��O�B�I�[���݂�::DWC_LOBBY_MAX_CHANNEL_NAME_SIZE�ȉ��̒����ł���K�v������܂��B\n
 *                        ���ۂɎQ������`�����l�����́u\#GSP!(gameName)!(::DWC_LOBBY_CHANNEL_PREFIX)!(channelName)�v�ƂȂ�܂��B\n
 *                        ��F�Q�[������"dwctest"�A::DWC_LOBBY_CHANNEL_PREFIX��::DWC_LOBBY_CHANNEL_PREFIX_DEBUG�A�`�����l������"TestChannel"�̏ꍇ�A
 *                        �u\#GSP!dwctest!D!TestChannel�v�ƂȂ�܂��B
 * @param[in] password �`�����l���̃p�X���[�h�B::DWCi_LobbySetChannelPassword�֐��ŃZ�b�g���ꂽ�p�X���[�h���w�肵�Ă��������B
 *                     �p�X���[�h���ݒ肳��Ă��Ȃ��ꍇ�͖�������܂��B
 * @param[in] limit �`�����l���ɎQ���ł���l�����w�肵�܂��B�����̃`�����l���ɎQ������ꍇ���w�肵���l���ɐ������ύX�����_�ɂ����Ӊ������B
 *                  0���w�肷��Ɛ������܂���B(::DWCi_LobbyJoinChannelAsync�֐��Ɠ�������ɂȂ�܂��B)
 * @param[in] channelCallbacks �`�����l���Ɋւ���R�[���o�b�N�֐����Z�b�g���܂��B
 * @param[in] callback �`�����l���ɎQ���܂��͎��s�����Ƃ��ɌĂ΂��R�[���o�b�N�B
 * @param[in] param callback��param�p�����[�^�ɂ��̂܂ܓn����郆�[�U��`���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyJoinChannelLimitAsync(const char* channelName, const char* password, s32 limit, const DWCi_LobbyChannelCallbacks* channelCallbacks,
                                             DWCi_LobbyJoinChannelCallback callback, void* param)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(channelName) + 1 > DWC_LOBBY_MAX_CHANNEL_NAME_SIZE)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyJoinChannelAsync: channelName is too long.\n");
    }
    
    // �����R�[���o�b�N����ۑ�
    u32 joinChannelOperationId = s_iLobby->GetCallbackManager().AddCallback(callback, param);
    
    // �`�����l������ǉ�
    s_iLobby->AddChannel(s_iLobby->ModifyChannelName(channelName).c_str(), *channelCallbacks, limit);
    
	chatChannelCallbacks gsChannelCallbacks;
	DWCi_Np_CpuClear8(&gsChannelCallbacks, sizeof(chatChannelCallbacks));
	gsChannelCallbacks.channelMessage = ChannelMessage;
	gsChannelCallbacks.channelModeChanged = ChannelModeChanged;
	gsChannelCallbacks.kicked = Kicked;
	gsChannelCallbacks.topicChanged = TopicChanged;
	gsChannelCallbacks.userParted = UserParted;
	gsChannelCallbacks.userJoined = UserJoined;
	gsChannelCallbacks.userListUpdated = UserListUpdated;
	gsChannelCallbacks.userModeChanged = UserModeChanged;
	gsChannelCallbacks.userChangedNick = UserChangedNick;
    gsChannelCallbacks.broadcastKeyChanged = BroadcastKeyChanged;
	gsChannelCallbacks.param = (void*)channelCallbacks->param;
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyJoinChannelAsync start to: %s\n", s_iLobby->ModifyChannelName(channelName).c_str());
	chatEnterChannel(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), password,
                     &gsChannelCallbacks, EnterChannelCallback, (void*)joinChannelOperationId, CHATFalse);
    
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/** 
 * @brief �`�����l������o�܂��B
 * 
 * �`�����l������o�܂��B
 * 
 * @param[in] channelName �ގ��������`�����l�����B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyLeaveChannel(const char* channelName)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyLeaveChannel start\n");
	chatLeaveChannel(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), NULL);
    
    // �`�����l�������폜
    s_iLobby->RemoveChannel(s_iLobby->ModifyChannelName(channelName));
    
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief Ascii������̃��b�Z�[�W���w�肵���`�����l���ɎQ�����Ă���S�Ẵv���C���[�ɑ��M���܂��B
 * 
 * Ascii������̃��b�Z�[�W���w�肵���`�����l���ɎQ�����Ă���S�Ẵv���C���[�ɑ��M���܂��B\n
 * ���b�Z�[�W�͎����ɂ����[�v�o�b�N����܂��B
 * 
 * @param[in] channelName ���M��̃`�����l�����B
 * @param[in] message ���M����NULL�I�[���ꂽAscii������B�I�[���݂�::DWC_LOBBY_MAX_STRING_SIZE�ȉ��̒����ł���K�v������܂��B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM message���������܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbySendChannelMessage(const char* channelName, const char* message)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(message) + 1 > DWC_LOBBY_MAX_STRING_SIZE)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySendChannelMessage: message is too long.\n");
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
	chatSendChannelMessage(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), message, DWCi_LOBBY_MESSAGE_TYPE_NORMAL_COMMAND);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief Ascii������̃��b�Z�[�W����l�̃v���C���[�ɑ��M���܂��B
 * 
 * Ascii������̃��b�Z�[�W����l�̃v���C���[�ɑ��M���܂��B�������Q�����Ă���`�����l���ɂ���l�ȊO�ɂ͑��M�ł��܂���B
 * 
 * @param[in] userId ���M��̃��[�UID�B
 * @param[in] message ���M����NULL�I�[���ꂽAscii������BNULL�I�[���݂�::DWC_LOBBY_MAX_STRING_SIZE�ȉ��̒����ł���K�v������܂��B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �w�肵�����[�U�����܂���A��������message���������܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbySendUserMessage(s32 userId, const char* message)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(message) + 1 > DWC_LOBBY_MAX_STRING_SIZE)
    {
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
    DWCi_String userName = s_iLobby->FindUser(userId);
    if(userName == "")
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySendUserMessage: There is no such userId: %d\n", userId);
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
	chatSendUserMessage(s_iLobby->GetChat(), userName.c_str(), message, DWCi_LOBBY_MESSAGE_TYPE_NORMAL_COMMAND);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l���f�[�^���Z�b�g���܂��B
 * 
 * �`�����l���f�[�^���Z�b�g���܂��B\n
 * �`�����l���f�[�^�͎Q�����Ă���`�����l���̃v���C���[��l��l�ƃ`�����l�����̂ɃZ�b�g���邱�Ƃ��ł��܂��B\n
 * ���ꂼ��ɍő�20�̃`�����l���f�[�^���Z�b�g���邱�Ƃ��ł��܂��B\n
 * key��"b_"����n�܂镶������w�肷��ƁA�T�[�o�ɃZ�b�g����Ɠ����ɑ��̃v���C���[�ɂ����ʒm���܂��B(�u���[�h�L���X�g)\n
 * ���̃u���[�h�L���X�g�͎����ɂ����[�v�o�b�N����܂��B\n
 * �ʒm��::DWCi_LobbyGetChannelDataCallback�R�[���o�b�N�ɂ��s���܂��B���̂Ƃ�������(broadcast)��TRUE�ƂȂ�܂��B
 * 
 * @param[in] channelName �`�����l���f�[�^���Z�b�g����`�����l�����B
 * @param[in] userId �`�����l���f�[�^���Z�b�g���郆�[�UID�B::DWC_LOBBY_INVALID_USER_ID���w�肷��ƃ`�����l�����̂ɃZ�b�g���܂��B
 * @param[in] key �`�����l���f�[�^�ɃA�N�Z�X���邽�߂�NULL�I�[���ꂽ������BNULL�I�[���݂�::DWC_LOBBY_MAX_CHANNEL_KEY_SIZE�܂ł̒����ł���K�v������܂��B
 * @param[in] value key�Ɍ��т���NULL�I�[���ꂽ������BNULL�I�[���݂�::DWC_LOBBY_MAX_STRING_SIZE�܂ł̒����ł���K�v������܂��B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �w�肵�����[�U�����܂���A��������key�܂���value���������܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbySetChannelData(const char* channelName, s32 userId, const char* key, const char* value)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(value) + 1 > DWC_LOBBY_MAX_STRING_SIZE || strlen(key) + 1 > DWC_LOBBY_MAX_CHANNEL_KEY_SIZE)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelData: key or value is too long.\n");
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
    DWCi_String userName = "";
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        // �`�����l�����̂̃f�[�^���Z�b�g
        userName = "";
    }
    else
    {
        // ����̃��[�U�̃f�[�^���Z�b�g
        userName = s_iLobby->FindUser(userId);
        if(userName == "")
        {
            DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelData: There is no such userId: %d\n", userId);
            return DWCi_LOBBY_RESULT_ERROR_PARAM;
        }
    }
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelData start. key: %s, value: %s\n", key, value);
    chatSetChannelKeys(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), userName.c_str(), 1, &key, &value);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l���f�[�^���擾���܂��B
 * 
 * �`�����l���f�[�^���擾���܂��B\n
 * ���������::DWCi_LobbyGetChannelDataCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] channelName �`�����l���f�[�^���擾����`�����l�����B
 * @param[in] userId �`�����l���f�[�^���擾���郆�[�UID�B::DWC_LOBBY_INVALID_USER_ID���w�肷��ƃ`�����l�����̂���擾���܂��B
 * @param[in] key �`�����l���f�[�^�ɃA�N�Z�X���邽�߂̃L�[�BNULL�I�[���݂�::DWC_LOBBY_MAX_CHANNEL_KEY_SIZE�܂ł̒����ł���K�v������܂��B
 * @param[in] callback �擾�������A���s�����Ƃ��ɌĂяo�����R�[���o�b�N�B
 * @param[in] param callback��param�p�����[�^�ɂ��̂܂ܓn����郆�[�U��`���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �w�肵�����[�U�����܂���A��������key���������܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyGetChannelDataAsync(const char* channelName, s32 userId, const char* key, DWCi_LobbyGetChannelDataCallback callback, void* param)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(key) + 1 > DWC_LOBBY_MAX_CHANNEL_KEY_SIZE)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyGetChannelDataAsync: key is too long.\n");
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
    DWCi_String userName = "";
    if(userId == DWC_LOBBY_INVALID_USER_ID)
    {
        // �`�����l�����̂̃f�[�^���擾
        userName = "";
    }
    else
    {
        // ����̃��[�U�̃f�[�^���擾
        userName = s_iLobby->FindUser(userId);
        if(userName == "")
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "DWCi_LobbyGetChannelDataAsync: There is no such userId: %d\n", userId);
            return DWCi_LOBBY_RESULT_ERROR_PARAM;
        }
    }
    
    // �R�[���o�b�N����ۑ�
    u32 operationId = s_iLobby->GetCallbackManager().AddCallback(callback, param);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyGetChannelDataAsync start: channel: %s, key: %s\n", channelName, key);
    chatGetChannelKeys(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), userName.c_str(),
                       1, &key, GetChannelKeysCallback, (void*)operationId, CHATFalse);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �T�[�o��̃`�����l�����������܂��B
 * 
 * �T�[�o��̃`�����l�����������܂��B\n
 * ���������::DWCi_LobbyEnumChannelsCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] filter ����������B���C���h�J�[�h"*"���g�p�ł��܂��B�Ⴆ�΂��̃A�v���P�[�V�����Ŏg�p���Ă���S�Ẵ`�����l����񋓂���ꍇ��"*"���w�肵�܂��B
 *                   test����n�܂�`�����l��������������ꍇ��"test*"�Ƃ��܂��B
 * @param[in] callback �����������A���s�����Ƃ��ɌĂяo�����R�[���o�b�N�B
 * @param[in] param callback��param�p�����[�^�ɂ��̂܂ܓn����郆�[�U��`���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyEnumChannelsAsync(const char* filter, DWCi_LobbyEnumChannelsCallback callback, void* param)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    // �R�[���o�b�N����ۑ�
    u32 operationId = s_iLobby->GetCallbackManager().AddCallback(callback, param);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyEnumChannelsAsync start. searching:%s\n", s_iLobby->ModifyChannelName(filter).c_str());
    chatEnumChannels(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(filter).c_str(), EnumChannelsEachCallback,    // �o�O?�ɂ��Each���ݒ肵�Ȃ���΂Ȃ�Ȃ��B
                     EnumChannelsAllCallback, reinterpret_cast<void*>(operationId), CHATFalse);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l���ɎQ�����Ă���l��񋓂��܂��B
 * 
 * @deprecated ::DWCi_LobbyEnumUsers�֐����g�p���Ă��������B
 * 
 * �`�����l���ɎQ�����Ă���l��񋓂��܂��B\n
 * �������Q�����Ă���`�����l���̂ݗ񋓂ł��܂��B\n
 * ���������::DWCi_LobbyEnumUsersCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] channelName �񋓂���`�����l�����B
 * @param[in] callback �񋓂������A���s�����Ƃ��ɌĂяo�����R�[���o�b�N�B
 * @param[in] param callback��param�p�����[�^�ɂ��̂܂ܓn����郆�[�U��`���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CHANNEL �w�肵���`�����l���ɎQ�����Ă��܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyEnumUsersAsync(const char* channelName, DWCi_LobbyEnumUsersCallback callback, void* param)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    // �������Q�����Ă��Ȃ��`�����l���͗񋓕s�\
    //if(!DWCi_LobbyInChannel(channelName))   // �Ȃ��������炾��GCC3.2.2�Ŋ֐��Ăяo���������Ƃ΂����
    if(!chatInChannel(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str()))
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyEnumUsersAsync called2 %s\n", channelName);
        return DWCi_LOBBY_RESULT_ERROR_CHANNEL;
    }
    
    // �R�[���o�b�N����ۑ�
    u32 operationId = s_iLobby->GetCallbackManager().AddCallback(callback, param);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyEnumUsersAsync start\n");
    chatEnumUsers(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(),
                  EnumUsersCallback, reinterpret_cast<void*>(operationId), CHATFalse);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l���ɎQ�����Ă���l��񋓂��܂��B
 * 
 * �`�����l���ɎQ�����Ă���l��񋓂��܂��B\n
 * �������Q�����Ă���`�����l���̂ݗ񋓂ł��܂��B\n
 * userIds��NULL��������userIdNum�Ɏ��ۂɕK�v�Ȍ���菭�Ȃ�����n����::DWCi_LOBBY_RESULT_ERROR_PARAM���Ԃ�A
 * userIdNum�Ɏ��ۂɕK�v�Ȍ�(�܂�`�����l���ɎQ�����Ă��郆�[�U�̐�)���i�[����܂��B
 * 
 * @param[in] channelName �񋓂���`�����l�����B
 * @param[in,out] userIds ���[�UID�̔z����i�[����o�b�t�@�B
 * @param[in,out] userIdNum userIds�ɗp�ӂ����z��̃T�C�Y�Bbyte�P�ʂł͂���܂���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CHANNEL �w�肵���`�����l���ɎQ�����Ă��܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM userIdNum���K�v�Ȍ���������Ă��邩�AuserIds��NULL�ł��BuserIdNum�ɕK�v�Ȍ����i�[���܂����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyEnumUsers(const char* channelName, s32* userIds, u32* userIdNum)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    u32 inputSize = *userIdNum;
    
    std::vector<s32, DWCi_Allocator<s32> > resultIds = s_iLobby->EnumUsers(s_iLobby->ModifyChannelName(channelName));
    if(resultIds.empty())
    {
        return DWCi_LOBBY_RESULT_ERROR_CHANNEL;
    }
    
    *userIdNum = resultIds.size();
    if(*userIdNum > inputSize || userIds == NULL)
    {
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
    std::copy(resultIds.begin(), resultIds.end(), userIds);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l�����[�h���Z�b�g���܂��B
 * 
 * �`�����l�����[�h���Z�b�g���܂��B
 * 
 * @param[in] channelName �Z�b�g����`�����l�����B
 * @param[in] mode �Z�b�g����`�����l�����[�h�B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbySetChannelMode(const char* channelName, const DWCi_LobbyChannelMode* mode)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelMode start\n");
    chatSetChannelMode(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), (CHATChannelMode*)mode);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l�����[�h���擾���܂��B
 * 
 * �`�����l�����[�h���擾���܂��B
 * 
 * @param[in] channelName �擾����`�����l�����B
 * @param[in] callback �擾�������A���s�����Ƃ��ɌĂяo�����R�[���o�b�N�B
 * @param[in] param callback��param�p�����[�^�ɂ��̂܂ܓn����郆�[�U��`���B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbyGetChannelModeAsync(const char* channelName, DWCi_LobbyGetChannelModeCallback callback, void* param)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    // �R�[���o�b�N����ۑ�
    u32 operationId = s_iLobby->GetCallbackManager().AddCallback(callback, param);
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbyGetChannelModeAsync start\n");
	chatGetChannelMode(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(),
                       GetChannelModeCallback, reinterpret_cast<void*>(operationId), CHATFalse);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l���g�s�b�N���Z�b�g���܂��B
 * 
 * �`�����l���g�s�b�N���Z�b�g���܂��B���̃g�s�b�N��::DWCi_LobbyEnumChannelsAsync�֐��ŎQ�Ƃ��邱�Ƃ��ł��܂��B\n
 * �w�肵���`�����l���ɓ������Ă��Ȃ��ꍇ�̓Z�b�g�ł��܂���B
 * 
 * @param[in] channelName �g�s�b�N���Z�b�g����`�����l�����B
 * @param[in] topic �ݒ肷��g�s�b�N�BNULL�I�[���܂�::DWC_LOBBY_MAX_CHANNEL_TOPIC_SIZE�ȉ��̒����܂Ŏw��ł��܂��B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �w�肵��topic���������܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CHANNEL �w�肵���`�����l���ɓ����Ă��܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbySetChannelTopic(const char* channelName, const char* topic)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(topic) + 1 > DWC_LOBBY_MAX_CHANNEL_TOPIC_SIZE)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelTopic: topic length is too long.\n");
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
    if(!DWCi_LobbyInChannel(channelName))
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelTopic: You are not in the channel.\n");
        return DWCi_LOBBY_RESULT_ERROR_CHANNEL;
    }
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelTopic start\n");
    chatSetChannelTopic(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), topic);
    return DWCi_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �`�����l���p�X���[�h���Z�b�g�܂��͉������܂��B
 * 
 * �`�����l���Ƀp�X���[�h���Z�b�g�܂��͉������܂��B�p�X���[�h���Z�b�g���ꂽ�ꍇ�A::DWCi_LobbyJoinChannelAsync�֐��Ńp�X���[�h���w�肵�Ȃ��Ɠ����ł��܂���B�B\n
 * �w�肵���`�����l���ɓ������Ă��Ȃ��ꍇ�̓Z�b�g�ł��܂���B
 * 
 * @param[in] channelName �p�X���[�h���Z�b�g����`�����l�����B
 * @param[in] enable �p�X���[�h���Z�b�g����ꍇ��TRUE�A��������ꍇ��FALSE���w�肵�Ă��������B
 * @param[in] password �ݒ�܂��͉�������p�X���[�h�BNULL�I�[���܂�::DWC_LOBBY_MAX_CHANNEL_PASSWORD_SIZE�ȉ��̒����܂Ŏw��ł��܂��B
 *                     ��������ꍇ�͌��݂̃p�X���[�h���w�肵�Ă��������B
 * 
 * @retval ::DWCi_LOBBY_RESULT_SUCCESS �����B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_STATE �X�e�[�g��CONNECTED�ł͂���܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_PARAM �w�肵��password���������܂��B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CHANNEL �w�肵���`�����l���ɓ����Ă��܂���B
 * @retval ::DWCi_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
DWCi_LOBBY_RESULT DWCi_LobbySetChannelPassword(const char* channelName, BOOL enable, const char* password)
{
    RETURN_IF_NOTINITIALIZED(DWCi_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(DWCi_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTCONNECTED(DWCi_LOBBY_RESULT_ERROR_STATE);
    
    if(strlen(password) + 1 > DWC_LOBBY_MAX_CHANNEL_PASSWORD_SIZE)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelTopic: topic length is too long.\n");
        return DWCi_LOBBY_RESULT_ERROR_PARAM;
    }
    
    if(!DWCi_LobbyInChannel(channelName))
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelTopic: You are not in the channel.\n");
        return DWCi_LOBBY_RESULT_ERROR_CHANNEL;
    }
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_LobbySetChannelPassword start\n");
    chatSetChannelPassword(s_iLobby->GetChat(), s_iLobby->ModifyChannelName(channelName).c_str(), enable ? CHATTrue : CHATFalse, password);
    return DWCi_LOBBY_RESULT_SUCCESS;
}
