/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/lobby/include/ppw_sender.h

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
 * @brief �|�P�������r�[���C�u���� �������J�w�b�_
 */

#ifndef PPW_SENDER_H_
#define PPW_SENDER_H_

#include <lobby/dwci_lobby.h>
#include "ppw_parser.h"

// �`�����l���ݒ�
const DWCi_LobbyChannelMode PPW_LOBBY_CHANNEL_MODE_MAIN_OPEN    = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, PPW_LOBBY_MAX_PLAYER_NUM_MAIN};
const DWCi_LobbyChannelMode PPW_LOBBY_CHANNEL_MODE_MAIN_LOCK    = {FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, PPW_LOBBY_MAX_PLAYER_NUM_MAIN};
const DWCi_LobbyChannelMode PPW_LOBBY_CHANNEL_MODE_FOOT         = {FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, PPW_LOBBY_MAX_PLAYER_NUM_FOOT};
const DWCi_LobbyChannelMode PPW_LOBBY_CHANNEL_MODE_CLOCK        = {FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, PPW_LOBBY_MAX_PLAYER_NUM_CLOCK};
const DWCi_LobbyChannelMode PPW_LOBBY_CHANNEL_MODE_NEWS         = {FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE, PPW_LOBBY_MAX_PLAYER_NUM_NEWS};

// �V�X�e���Ŏg�p���郁�b�Z�[�W�^�C�v
typedef enum PPW_LOBBY_MESSAGE_TYPE
{
    PPW_LOBBY_MESSAGE_TYPE_LOCK,
    PPW_LOBBY_MESSAGE_TYPE_RECRUIT,
    PPW_LOBBY_MESSAGE_TYPE_RECRUIT_STOPPED
}
PPW_LOBBY_MESSAGE_TYPE;

// ���M����f�[�^���Ȃ��ꍇ�Ɏw�肷��_�~�[�f�[�^
typedef struct PPW_LobbyDummyData
{
    u32 dummy;
    PPW_LobbyDummyData()
        : dummy(0)
    {}
}
PPW_LobbyDummyData;

inline const DWCi_LobbyChannelMode* PPW_LobbyGetChannelMode(PPW_LOBBY_CHANNEL_KIND kind)
{
    switch(kind)
    {
    case PPW_LOBBY_CHANNEL_KIND_FOOT1:
    case PPW_LOBBY_CHANNEL_KIND_FOOT2:
        return &PPW_LOBBY_CHANNEL_MODE_FOOT;
    case PPW_LOBBY_CHANNEL_KIND_CLOCK:
        return &PPW_LOBBY_CHANNEL_MODE_CLOCK;
    case PPW_LOBBY_CHANNEL_KIND_NEWS:
        return &PPW_LOBBY_CHANNEL_MODE_NEWS;
    default:
        DWC_ASSERTMSG(FALSE, "PPW_LobbyGetChannelMode: Invalid channel kind.");
        return NULL;
    }
}

PPW_LOBBY_RESULT PPW_LobbySendMessage(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, PPW_LobbyMessage::FORMAT format,
                                                PPW_LobbyMessage::TARGET target, s32 type, const u8* data, u32 size);
PPW_LOBBY_RESULT PPW_LobbySetChannelDataImpl(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char* key, const u8* data, u32 dataSize);

#endif
