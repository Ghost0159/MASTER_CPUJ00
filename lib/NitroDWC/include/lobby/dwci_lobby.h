/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./build/libraries/lobby/include/dwci_lobby.h

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
 */

#ifndef DWCi_LOBBY_H_
#define DWCi_LOBBY_H_

#include <lobby/dwc_lobby.h>


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
#pragma pack(push, 4)
#endif

// �����ȃ`�����l��ID
#define DWCi_LOBBY_INVALID_CHANNEL_ID       0xffffffff

// �ő�j�b�N�l�[���T�C�Y
#define DWCi_MAX_CHAT_NICK_SIZE             20

// �ڑ���ɑ҂t���[���B
#define DWCi_LOBBY_CONNECTINGWAIT_TIME      60

// �`�����l�����̋�؂蕶��
#define DWCi_LOBBY_CHANNEL_NAME_SEPARATOR   "!"

// ���r�[���C�u�����Ŏg�p����G���[�R�[�h�̊�ԍ�
#define DWCi_LOBBY_ERROR_CODE_BASE          42000

// ���r�[���C�u�����Ŏg�p����`���b�g�T�[�o�h���C��
#define DWCi_LOBBY_SERVER_DOMAIN            "peerchat.gs.nintendowifi.net"

/// ��ԃt���O
typedef enum DWCi_LOBBY_STATE
{
    DWCi_LOBBY_STATE_NOTINITIALIZED,        ///< ���C�u����������������Ă��Ȃ��B
    DWCi_LOBBY_STATE_CONNECTING,            ///< �T�[�o�ɐڑ���
    DWCi_LOBBY_STATE_CONNECTINGWAIT,        ///< �T�[�o�ɐڑ��㏈��
    DWCi_LOBBY_STATE_CONNECTED,             ///< �T�[�o�ɐڑ�����
    DWCi_LOBBY_STATE_CLOSING,               ///< ���C�u�����I���������B
    DWCi_LOBBY_STATE_ERROR                  ///< ���C�u�����ɒv���I�ȃG���[���������B::DWCi_LobbyGetLastError�֐��ŃG���[���擾��V���b�g�_�E�����Ă��������B
}
DWCi_LOBBY_STATE;

/// �������ʃt���O�B
typedef enum DWCi_LOBBY_RESULT
{
    DWCi_LOBBY_RESULT_SUCCESS,              ///< �����B
    DWCi_LOBBY_RESULT_ERROR_ALLOC,          ///< �������m�ۂɎ��s�B
    DWCi_LOBBY_RESULT_ERROR_SESSION,        ///< �ʐM�G���[�B
    DWCi_LOBBY_RESULT_ERROR_PARAM,          ///< �w�肵���������s���B
    DWCi_LOBBY_RESULT_ERROR_STATE,          ///< �Ăяo���Ă͂����Ȃ���ԂŊ֐����Ăяo�����B
    DWCi_LOBBY_RESULT_ERROR_CHANNEL,        ///< �w�肵���`�����l���ɓ����Ă��Ȃ��B
    DWCi_LOBBY_RESULT_ERROR_NODATA,         ///< �w�肵�����͑��݂��Ȃ��B
    DWCi_LOBBY_RESULT_ERROR_CONDITION,      ///< �v���I�G���[���������Ȃ̂ł��̊֐����Ăяo���Ȃ��B
    DWCi_LOBBY_RESULT_MAX                   ///< ���C�u���������Ŏg�p����܂��B
}
DWCi_LOBBY_RESULT;

/// �G���[�t���O
typedef enum DWCi_LOBBY_ERROR
{
    DWCi_LOBBY_ERROR_NONE,                  ///< �G���[�����B
    DWCi_LOBBY_ERROR_UNKNOWN,               ///< �s���ȃG���[�B
    DWCi_LOBBY_ERROR_ALLOC,                 ///< �������m�ۂɎ��s�B
    DWCi_LOBBY_ERROR_SESSION,               ///< �v���I�ȒʐM�G���[�B
    DWCi_LOBBY_ERROR_MAX                    ///< ���C�u���������Ŏg�p����܂��B
}
DWCi_LOBBY_ERROR;

/// �`�����l����������
typedef enum
{
	DWCi_LOBBY_ENTER_RESULT_SUCCESS,            ///< �����B
	DWCi_LOBBY_ENTER_RESULT_BAD_CHANNEL_NAME,   ///< �`�����l�������s���ł��B
	DWCi_LOBBY_ENTER_RESULT_FULL,               ///< �����B
	DWCi_LOBBY_ENTER_RESULT_INVITE_ONLY,        ///< ���̃`�����l���͏��҂ł̂ݓ���܂��B
	DWCi_LOBBY_ENTER_RESULT_BANNED,             ///< ���̃`�����l������Ban����Ă��܂��B
	DWCi_LOBBY_ENTER_RESULT_BAD_PASSWORD,       ///< ���̃`�����l���ɂ̓p�X���[�h���������Ă��܂��B�������̓p�X���[�h���Ⴂ�܂��B
	DWCi_LOBBY_ENTER_RESULT_TOO_MANY_CHANNELS,  ///< ����ȏ�̑����̃`�����l���ɓ���܂���B
	DWCi_LOBBY_ENTER_RESULT_TIME_OUT,           ///< �^�C���A�E�g�B
	DWCi_LOBBY_ENTER_RESULT_MAX                 ///< ���C�u���������Ŏg�p����܂��B
}
DWCi_LOBBY_ENTER_RESULT;

/// �`�����l�����[�h
typedef struct DWCi_LobbyChannelMode
{
    BOOL inviteOnly;                        ///< �`�����l���ւ̎Q�������҂݂̂ɂ���BFALSE���w�肵�Ă��������B
    BOOL privateMode;                       ///< �`�����l�������ɑ΂��ă`�����l�����𕚂���BsecretMode�ƕ��p���Ȃ��ł��������B
    BOOL secretMode;                        ///< �`�����l�������ɑ΂��ă`�����l�������S�ɉB���BprivateMode�ƕ��p���Ȃ��ł��������B
    BOOL moderated;                         ///< �`�����l���I�y���[�^���������ł��Ȃ�����B
    BOOL noExternalMessages;                ///< �`�����l���ɓ����Ă��Ȃ��l�̔��������ۂ���B
    BOOL onlyOpsChangeTopic;                ///< �`�����l���I�y���[�^�����g�s�b�N��ύX�ł��Ȃ�����BTRUE���w�肵�Ă��������B
    BOOL opsObeyChannelLimit;               ///< �`�����l���I�y���[�^�ɂ��l��������K�p����BTRUE���w�肵�Ă��������B
    s32 limit;                              ///< �`�����l���ɓ����l���𐧌�����B0�ȉ��̏ꍇ�͐������܂���B
}
DWCi_LobbyChannelMode;


// �R�[���o�b�N�^�錾

/**
 * @brief �������`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �������`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] success �����ɐ����������B
 * @param[in] result �������ʃt���O�B
 * @param[in] channelName ���������`�����l���̖��O�B
 * @param[in] param ::DWCi_LobbyJoinChannel*Async�֐���param�p�����[�^�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyJoinChannelCallback)(BOOL success, DWCi_LOBBY_ENTER_RESULT result, const char* channelName, void* param);

/**
 * @brief ���̐l���`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̐l���`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] channelName ���������`�����l���̖��O�B
 * @param[in] userId ���������l�̃��[�UID�B
 * @param[in] param ::DWCi_LobbyChannelCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyPlayerConnectedCallback)(const char* channelName, s32 userId, void* param);

/**
 * @brief ���̐l���`�����l������ގ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̐l���`�����l������ގ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] channelName �ގ������`�����l���̖��O�B
 * @param[in] userId �ގ������l�̃��[�UID�B
 * @param[in] param ::DWCi_LobbyChannelCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyPlayerDisconnectedCallback)(const char* channelName, s32 userId, void* param);

/**
 * @brief �`�����l���ւ̃��b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l���ւ̃��b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] channelName ��M�����`�����l���̖��O�B
 * @param[in] userId ���M�����l�̃��[�UID�B
 * @param[in] message ��M�������b�Z�[�W�B
 * @param[in] param ::DWCi_LobbyChannelCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyChannelMassageReceivedCallback)(const char* channelName, s32 userId, const char* message, void* param);

/**
 * @brief �l���̃��b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �l���̃��b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId ���M�����l�̃��[�UID�B
 * @param[in] message ��M�������b�Z�[�W�B
 * @param[in] param ::DWCi_LobbyGlobalCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyUserMassageReceivedCallback)(s32 userId, const char* message, void* param);

/**
 * @brief �`�����l���f�[�^����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l���f�[�^����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * ::DWCi_LobbyGetChannelDataAsync�֐��̌��ʂ��ʒm����܂��B
 * 
 * @param[in] success �����������B
 * @param[in] channelName ��M�����`�����l���f�[�^�Ɋ֘A�Â����Ă���`�����l�����B
 * @param[in] userId ��M�����`�����l���f�[�^�Ɋ֘A�Â����Ă��郆�[�UID�B
 * @param[in] key ��M�����L�[������B
 * @param[in] value key�Ɋ֘A�Â���ꂽ�f�[�^�B
 * @param[in] param ::DWCi_LobbyGetChannelDataAsync�֐���param�p�����[�^�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyGetChannelDataCallback)(BOOL success, const char* channelName, s32 userId,
                                                 const char* key, const char* value, void* param);

/**
 * @brief �u���[�h�L���X�g���ꂽ�`�����l���f�[�^����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �u���[�h�L���X�g���ꂽ�`�����l���f�[�^����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] channelName ��M�����`�����l���f�[�^�Ɋ֘A�Â����Ă���`�����l�����B
 * @param[in] userId �u���[�h�L���X�g���s�������[�UID�B
 * @param[in] key ��M�����L�[������B
 * @param[in] value key�Ɋ֘A�Â���ꂽ�f�[�^�B
 * @param[in] param ::DWCi_LobbyChannelCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyBroadcastDataCallback)(const char* channelName, s32 userId,
                                                 const char* key, const char* value, void* param);

/**
 * @brief �`�����l���̗񋓂����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l���̗񋓂����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * ::DWCi_LobbyEnumChannelsAsync�֐��̌��ʂ��ʒm����܂��B
 * 
 * @param[in] success �����������B
 * @param[in] numChannels �񋓂����`�����l�����B
 * @param[in] channels numChannel���̃`�����l�����̔z��B
 * @param[in] topics channels�ɑΉ�����g�s�b�N�̔z��B
 * @param[in] numUsers channels�ɑΉ�����`�����l���ɎQ�����Ă���l�̐l���B
 * @param[in] param ::DWCi_LobbyEnumChannelsAsync�֐���param�p�����[�^�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyEnumChannelsCallback)(BOOL success, s32 numChannels, const char** channels, const char** topics, const s32* numUsers, void* param);

/**
 * @brief �`�����l���ɎQ�����Ă���l�̗񋓂����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l���ɎQ�����Ă���l�̗񋓂����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * ::DWCi_LobbyEnumUsersAsync�֐��̌��ʂ��ʒm����܂��B
 * 
 * @param[in] success �����������B
 * @param[in] channelName �񋓂����Ώۂ̃`�����l�����B
 * @param[in] numUsers �`�����l���ɎQ�����Ă���l���B
 * @param[in] userIds �`�����l���ɎQ�����Ă���l�̃��[�UID�̔z��B
 * @param[in] param ::DWCi_LobbyEnumUsersAsync�֐���param�p�����[�^�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyEnumUsersCallback)(BOOL success, const char* channelName, s32 numUsers, const s32* userIds, void* param);

/**
 * @brief �`�����l�����[�h���擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l�����[�h���擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * ::DWCi_LobbyGetChannelModeAsync�֐��̌��ʂ��ʒm����܂��B\n
 * ::DWCi_LobbySetChannelMode�֐����Ă񂾒���ɖ{�֐��ɂ���ă`�����l�����[�h���擾�����ꍇ�A
 * ���ۂɃ��[�h���Z�b�g�����܂łɎ኱�̃^�C�����O�����邽�߁A�ύX�O�̃`�����l�����[�h���擾�����ꍇ������܂��B
 * 
 * @param[in] success �����������B
 * @param[in] channelName �擾�����Ώۂ̃`�����l�����B
 * @param[in] mode �`�����l�����[�h�B
 * @param[in] param ::DWCi_LobbyGetChannelModeAsync�֐���param�p�����[�^�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyGetChannelModeCallback)(BOOL success, const char* channelName, const DWCi_LobbyChannelMode* mode, void* param);

/**
 * @brief �`�����l���g�s�b�N���ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l���g�s�b�N���ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] channelName �擾�����Ώۂ̃`�����l�����B
 * @param[in] topic �ύX��̃g�s�b�N�B
 * @param[in] param ::DWCi_LobbyChannelCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyTopicChangedCallback)(const char* channelName, const char* topic, void* param);

/**
 * @brief �T�[�o�Ƃ̒ʐM���ߏ�ɍs�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �T�[�o�Ƃ̒ʐM���ߏ�ɍs�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * floodWeight�͒ʐM�𔭐������郍�r�[���C�u�����̊֐��̌Ăяo���ɂ���ď㏸���A
 * 300�𒴂����::DWCi_LobbyGetChannelDataAsync�֐������������悤�ɂȂ�A600�𒴂���ƃT�[�o����ؒf����܂��B
 * floodWeight��1�b��5�������܂��B
 * ������DWCi_LobbySend*Message�֐��̌Ăяo����floodWeight���㏸�����܂���B
 * ���̃R�[���o�b�N�����������Ƃ��̓`�����l���f�[�^�Z�b�g��擾���T����悤�ɂ��Ă��������B
 * 
 * @param[in] floodWeight �ʐM�ߑ��̊����������d�݁B
 * @param[in] param ::DWCi_LobbyGlobalCallbacks�\���̂�param�����o�Ɏw�肵���l�B
 */
typedef void (*DWCi_LobbyExcessFloodCallback)(u32 floodWeight, void* param);

/// �O���[�o���R�[���o�b�N�o�^�p�\����
typedef struct DWCi_LobbyGlobalCallbacks
{
    /// �l���̃��b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyUserMassageReceivedCallback lobbyUserMassageReceivedCallback;
    /// �T�[�o�Ƃ̒ʐM���ߏ�ɍs�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyExcessFloodCallback lobbyExcessFloodCallback;
    /// �R�[���o�b�N�֐���param�p�����[�^�ɓn�����l�B
    void* param;
}
DWCi_LobbyGlobalCallbacks;

/// �`�����l���R�[���o�b�N�o�^�p�\����
typedef struct DWCi_LobbyChannelCallbacks
{
    /// ���̐l���`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyPlayerConnectedCallback lobbyPlayerConnectedCallback;
    /// ���̐l���`�����l������ގ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyPlayerDisconnectedCallback lobbyPlayerDisconnectedCallback;
    /// �`�����l���ւ̃��b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyChannelMassageReceivedCallback lobbyChannelMassageReceivedCallback;
    /// �u���[�h�L���X�g���ꂽ�`�����l���f�[�^����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyBroadcastDataCallback lobbyBroadcastDataCallback;
    /// �`�����l���g�s�b�N���ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    DWCi_LobbyTopicChangedCallback lobbyTopicChangedCallback;
    /// �R�[���o�b�N�֐���param�p�����[�^�ɓn�����l�B
    void* param;
}
DWCi_LobbyChannelCallbacks;

// ���r�[���C�u�������������B
DWCi_LOBBY_RESULT   DWCi_LobbyInitializeAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                         const DWCi_LobbyGlobalCallbacks* globalCallbacks, const DWCUserData* userdata);

// ���r�[���C�u�������w�肵��PID�ŏ������B
DWCi_LOBBY_RESULT   DWCi_LobbyInitializePidAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                         const DWCi_LobbyGlobalCallbacks* globalCallbacks, s32 pid);
// ���r�[���C�u�������I���B
DWCi_LOBBY_RESULT   DWCi_LobbyShutdownAsync();

// �v���Z�X�֐��B
DWCi_LOBBY_STATE    DWCi_LobbyProcess();

// �G���[���擾�B
DWCi_LOBBY_ERROR    DWCi_LobbyGetLastError();

// ���������G���[(::DWCi_LOBBY_ERROR)����\�����ׂ��G���[�R�[�h�𓾂�B
s32                 DWCi_LobbyToErrorCode(DWCi_LOBBY_ERROR err);

// �����̃��[�UID���擾�B
s32                 DWCi_LobbyGetMyUserId();

// �w�肵���`�����l���ɓ����Ă��邩���ׂ�B
BOOL                DWCi_LobbyInChannel(const char* channelName);

// �`�����l���ɓ���B
DWCi_LOBBY_RESULT   DWCi_LobbyJoinChannelAsync(const char* channelName, const char* password, const DWCi_LobbyChannelCallbacks* channelCallbacks,
                                               DWCi_LobbyJoinChannelCallback callback, void* param);

// �ő�l�����w�肵�ă`�����l���ɓ���B
DWCi_LOBBY_RESULT   DWCi_LobbyJoinChannelLimitAsync(const char* channelName, const char* password, s32 limit, const DWCi_LobbyChannelCallbacks* channelCallbacks,
                                                  DWCi_LobbyJoinChannelCallback callback, void* param);
    
// �`�����l������o��B
DWCi_LOBBY_RESULT   DWCi_LobbyLeaveChannel(const char* channelName);

// �`�����l���ɎQ�����Ă���l�Ƀ��b�Z�[�W�𑗐M����B
DWCi_LOBBY_RESULT   DWCi_LobbySendChannelMessage(const char* channelName, const char* message);

// �w�肵�����[�U�փ��b�Z�[�W�𑗐M����B
DWCi_LOBBY_RESULT   DWCi_LobbySendUserMessage(s32 userId, const char* message);

// �`�����l���f�[�^���Z�b�g����B
DWCi_LOBBY_RESULT   DWCi_LobbySetChannelData(const char* channelName, s32 userId, const char* key, const char* value);

// �`�����l���f�[�^���擾����B
DWCi_LOBBY_RESULT   DWCi_LobbyGetChannelDataAsync(const char* channelName, s32 userId, const char* key, DWCi_LobbyGetChannelDataCallback callback, void* param);

// �`�����l����񋓂���B
DWCi_LOBBY_RESULT   DWCi_LobbyEnumChannelsAsync(const char* filter, DWCi_LobbyEnumChannelsCallback callback, void* param);

// �`�����l���ɎQ�����Ă���l��񋓂���B
DWCi_LOBBY_RESULT   DWCi_LobbyEnumUsersAsync(const char* channelName, DWCi_LobbyEnumUsersCallback callback, void* param);

// �`�����l���ɎQ�����Ă���l��񋓂���B
DWCi_LOBBY_RESULT   DWCi_LobbyEnumUsers(const char* channelName, s32* userIds, u32* userIdNum);

// �`�����l�����[�h���Z�b�g����B
DWCi_LOBBY_RESULT   DWCi_LobbySetChannelMode(const char* channelName, const DWCi_LobbyChannelMode* mode);

// �`�����l�����[�h���擾����B
DWCi_LOBBY_RESULT   DWCi_LobbyGetChannelModeAsync(const char* channelName, DWCi_LobbyGetChannelModeCallback callback, void* param);

// �`�����l���g�s�b�N���Z�b�g����B
DWCi_LOBBY_RESULT   DWCi_LobbySetChannelTopic(const char* channelName, const char* topic);

// �`�����l���p�X���[�h���Z�b�g����B
DWCi_LOBBY_RESULT DWCi_LobbySetChannelPassword(const char* channelName, BOOL enable, const char* password);

#ifdef WIN32
#pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif

#endif
