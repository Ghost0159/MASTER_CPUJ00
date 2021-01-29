/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./include/lobby/dwc_lobby.h

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
 * @brief Wi-Fi ���r�[���C�u���� �w�b�_
 * 
 * Wi-Fi ���r�[���C�u����
 * 
 */

#ifndef DWC_LOBBY_H_
#define DWC_LOBBY_H_

#include <dwc.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
#pragma pack(push, 4)
#endif

#define DWC_LOBBY_INVALID_USER_ID       -1  ///< �����ȃ��[�UID�B
#define DWC_LOBBY_INVALID_TIME          0   ///< �����Ȏ��ԁB

/// DWCi_LobbySend*Message�֐��y��DWCi_LobbySetChannelData�֐��ő��M�ł���ő�̕������B(NULL�I�[�܂�)
#define DWC_LOBBY_MAX_STRING_SIZE       400

/// �`�����l�����Ɏw��ł���ő�̕������B(NULL�I�[�܂�)
#define DWC_LOBBY_MAX_CHANNEL_NAME_SIZE 20
    
/// DWCi_LobbySetChannelTopic�֐��Ŏw��ł���g�s�b�N�̍ő�̕������B(NULL�I�[�܂�)
#define DWC_LOBBY_MAX_CHANNEL_TOPIC_SIZE DWC_LOBBY_MAX_STRING_SIZE

/// �`�����l���f�[�^�̃L�[�Ƃ��Ďw��ł���ő�̕������B(NULL�I�[�܂�)
#define DWC_LOBBY_MAX_CHANNEL_KEY_SIZE  20

/// �`�����l���̃p�X���[�h�Ƃ��Ďw��ł���ő�̕������B(NULL�I�[�܂�)
#define DWC_LOBBY_MAX_CHANNEL_PASSWORD_SIZE 20

/// �`�����l�����ɕt����v���t�B�b�N�X�B�����`�����l���𕪂��邱�Ƃ��ł��܂��B
typedef enum DWC_LOBBY_CHANNEL_PREFIX
{
    DWC_LOBBY_CHANNEL_PREFIX_RELEASE,       ///< ���i�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG,         ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG1,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG2,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG3,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG4,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG5,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG6,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG7,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG8,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_DEBUG9,        ///< �f�o�b�O�p�B
    DWC_LOBBY_CHANNEL_PREFIX_TEST           ///< ���C�u�����J���p�B(�w�肵�Ȃ��ł�������)
}
DWC_LOBBY_CHANNEL_PREFIX;

#ifdef WIN32
#pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // DWC_LOBBY_H_
