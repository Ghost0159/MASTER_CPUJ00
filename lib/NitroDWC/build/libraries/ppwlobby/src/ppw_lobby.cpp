/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./ppwlobby/src/ppw_lobby.cpp

  Copyright 2005-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
/**
 * @file ppw_lobby.h
 * 
 */

/**
 * @page PPW_LOBBY �|�P�����v���`�i Wi-Fi ���r�[���C�u����
 *
 * @section Introduction
 *
 * ���̃��C�u�����ɂ���āAGameSpy��IRC�T�[�o����đ��̃v���C���[�ƒʐM���邱�Ƃ��ł��܂��B
 *
 * ���ׂĂ̒ʐM�̓T�[�o�𒆌p����̂ŁA�ł������ʐM�ʂ����Ȃ��Ȃ�悤�ɂ��Ă��������B\n
 * �ڈ��Ƃ��ẮA�`�����l���S�̂ɑ��M�������(::PPW_LobbySendChannel*Message�֐���::PPW_LobbySetChannelData�֐��ɂ��u���[�h�L���X�g)�́A
 * ���������b�Z�[�W�̏ꍇ�͈�l�ɂ����b10���b�Z�[�W�ȉ��A�傫�����b�Z�[�W�̏ꍇ�͖��b5���b�Z�[�W�ȉ��ɗ}���Ă��������B\n
 * ���C�u���������ł̃������m�ۂ�DWC_Alloc�֐����g�p���Ă��܂��B\n
 * �{���C�u�����̓X���b�h�Z�[�t�ł͂���܂���B\n
 * ��ȏ�̃X���b�h�œ����Ƀ��C�u�����̊֐����Ăяo���Ȃ��ł��������B\n
 * �{���C�u�����Ńv���C���[�����ʂ��邽�߂Ɏg�p���郆�[�UID��GameSpyProfileID(pid)�ƈ�v���Ă��܂��B\n
 * �����̃��[�UID�͏��������::PPW_LobbyGetMyUserId�֐����g�p���邱�Ƃɂ��擾�ł��܂��B\n
 * ����̓v���C���[���ƂɃ��j�[�N�Ȓl�ƂȂ�܂��B\n
 * �������A�f�o�b�O�p��::PPW_LobbyInitializePidAsync�֐����g���ď����������Ƃ��A�����̃v���C���[��
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
 * ::PPW_LobbyInitializeAsync�֐����g�����C�u���������������܂��B
 * �����ŔF�؃T�[�o�̎������Q�Ƃ��Ă��܂��̂ł��炩����DWC_LoginAsync�֐�����
 * DWC_NASLoginAsync�֐���Wi-Fi�R�l�N�V�����ւ̐ڑ����������Ă���K�v������܂��B \n
 * ��������A���C�u�����͂܂�����ɒB���Ă��Ȃ��`�����l����T���A����܂��B\n
 * �`�����l����������Ȃ������ꍇ�͎����Ń`�����l�����쐬���A����܂��B\n
 * ���̃`�����l�������C���`�����l���ƌĂт܂��B\n
 * ���C���`�����l���ɓ����::PPW_LobbyJoinChannelCallback���R�[���o�b�N����A
 * ���C�u�����̃X�e�[�g��::PPW_LOBBY_STATE_READY�ƂȂ�܂��B
 *
 *
 * ���T�u�`�����l��
 * 
 * ���C���`�����l���̑��ɂ�����ʂ̃`�����l���ɓ��邱�Ƃ��ł��܂��B\n
 * ������T�u�`�����l���ƌĂт܂��B
 * �T�u�`�����l���ɓ����Ă���Ԃ����C���`�����l���Ɋւ��郁�b�Z�[�W����M���܂��B\n
 * ���Ճ{�[�h�A���E���v�A���r�[�j���[�X�͂��̃T�u�`�����l�����g���������Ă��������B
 *
 * 
 * �����b�Z�[�W�̑���M
 *
 * ���C�u�����̃X�e�[�g��::PPW_LOBBY_STATE_READY�̂Ƃ��Ɋe�탁�b�Z�[�W�̑���M�A
 * �`�����l���f�[�^�̑���M���ł��܂��B
 *
 * 
 * ���~�j�Q�[���̕�W�E�Q��
 * 
 * �~�j�Q�[���̕�W��Anybody�}�b�`�𕹗p���čs���܂��B\n
 * ���r�[�ŕ�W�������Ȃ���}�b�`���O������i�߂܂��B��W�ґ��ƎQ���ґ��ɕ����ď����̗�����L���܂��B
 * 
 * - ��W��\n
 * ::PPW_LobbyStartRecruit�֐����ĂсA�������ꂽ�}�b�`���O�w�W�������DWC_AddMatchKeyString�֐��ŃZ�b�g���܂��B\n
 * �Z�b�g�����}�b�`���O�w�W������ƈ�v����悤��filter�p�����[�^���Z�b�g��DWC_ConnectToAnybodyAsync�֐����Ăяo���܂��B\n
 * �}�b�`���C�N���͐ڑ������l����DWC_GetNumConnectionHost�֐��ŎQ�Ƃ��A
 * ���݂̎Q���l�����ϓ�������::PPW_LobbyUpdateRecruitInfo�֐��Ō��ݏW�܂��Ă���l��(currentNum)��ύX���܂��B\n
 * �}�b�`���C�N������������::PPW_LobbyStopRecruit�֐����Ăяo���Ă��������B
 * 
 * - �Q����\n
 * ::PPW_LobbyRecruitCallback�R�[���o�b�N���Ă΂ꂽ��A
 * ���̃~�j�Q�[���ɎQ������Ȃ�::PPW_LobbyRecruitCallback�R�[���o�b�N�Ɋ܂܂�Ă���}�b�`���O�w�W������Ɉ�v����悤��
 * filter�p�����[�^���Z�b�g����DWC_ConnectToAnybodyAsync�֐����Ăяo���܂��B\n
 * �}�b�`���C�N����������܂ő҂��܂��B
 *
 *
 * �����C�u�����̏I��
 *
 * ::PPW_LobbyShutdownAsync�֐����Ăяo���ă��C�u�������I�����܂��B\n
 * ���̊֐����Ăяo������A::PPW_LobbyProcess�֐���::PPW_LOBBY_STATE_NOTINITIALIZED��Ԃ��ƃV���b�g�_�E�������ł��B
 * ::PPW_LobbyShutdownAsync�֐��Ɠ�����DWC_CleanupInetAsync�֐����Ăяo���Ȃ��悤�ɂ��Ă��������B
 * ���r�[���C�u�����̏I�����m�F�������DWC_CleanupInetAsync�֐����Ăяo���Ă��������B
 * 
 * 
 * ���G���[�����ƕ\��
 *
 * �v���I�ȃG���[�����������ꍇ��::PPW_LobbyProcess�֐���::PPW_LOBBY_STATE_ERROR��Ԃ��܂��B\n
 * ���̏�ԂɂȂ�ƂقƂ�ǂ̊֐����g�p�s�\�ɂȂ�܂��B\n
 * �܂��A�����Ń������m�ۂɎ��s�����ꍇ�A���̊֐��ł̏����͐���Ɋ������܂����A���C�u�����̃X�e�[�g��::PPW_LOBBY_STATE_ERROR�ɕύX���܂��B\n
 * ���̂��ߎ��̊֐��Ăяo����::PPW_LOBBY_RESULT_ERROR_CONDITION��::PPW_LOBBY_STATE_ERROR���Ԃ�܂��B\n
 * ���̏�ԂɂȂ����ꍇ��::PPW_LobbyGetLastError�֐��ŃG���[���e���m�F��A::PPW_LobbyShutdownAsync�֐��ŃV���b�g�_�E�����Ă��������B\n
 * �܂��A���̂Ƃ��uWi-Fi�Ђ�΂ŒʐM�G���[���������܂����B�v�Ƃ������b�Z�[�W�Ƌ���::PPW_LobbyToErrorCode�֐����g�p���ē���ꂽ�G���[�R�[�h��\�����Ă��������B\n
 * "Wi-Fi�Ђ��"�Ƃ������͓̂K�X���r�[�@�\�̖��̂ɏ��������Ă��������B\n
 * P2P�}�b�`���O���ɔ��������G���[��DWC���C�u�����̋K��ɉ����ăG���[���b�Z�[�W��\�����Ă��������B\n
 * ���̑��A�A�v���P�[�V�������Ń��r�[�@�\�𑱍s�ł��Ȃ��Ȃ����Ɣ��f�����ꍇ�̃��b�Z�[�W�\���ɂ��Ă̓A�v���P�[�V�����ɔC����܂��B
 *
 *</td></tr></table>
 */

#include <lobby/dwci_lobby.h>
#include "dwci_lobbyBase.h"
#include "dwci_lobbyUtil.h"
#include "ppw_data.h"
#include "ppw_parser.h"
#include "dwci_encsessioncpp.h"
#include "WFLBY_USER_PROFILE.h"

// �萔��`
static const char* const PPW_LOBBY_CHANNEL_NAME[] =
{
    "M",    // Main
    "F1",   // Foot1
    "F2",   // Foot2
    "C",    // Clock
    "N",    // News
    "Invalid"
};

// ENC�pINIT_DATA
static const char* ENC_INIT_DATA = "uLMOGEiiJogofchScpXb000244fd00006015100000005b440e7epokemondpds";


// ENC�Ŏ��s�����
static const u32 ENC_RETRY_COUNT = 3;

// �T�[�o�̌��ʃR�[�h��\���񋓎q
typedef enum ENC_SERVER_RESPONSE_CODE
{
    ENC_SERVER_RESPONSE_CODE_OK             = 0,
    ENC_SERVER_RESPONSE_CODE_INVALID_PARAM  = 6,
    ENC_SERVER_RESPONSE_CODE_INTERNAL_ERROR = 7,
    ENC_SERVER_RESPONSE_CODE_DATABASE_ERROR = 8,
    ENC_SERVER_RESPONSE_CODE_MAINTENANCE    = 9,
    ENC_SERVER_RESPONSE_CODE_UNKNOWN_ERROR  = 10
}
ENC_SERVER_RESPONSE_CODE;


// �s���`�F�b�N������A�h���X
static const char* ENC_URL_CHECK_PROFILE = "/web/enc/lobby/checkProfile.asp";

// �X�P�W���[�����擾����A�h���X
static const char* ENC_URL_GET_SCHEDULE = "/web/enc/lobby/getSchedule.asp";

// VIP�����擾����A�h���X
static const char* ENC_URL_GET_VIP = "/web/enc/lobby/getVIP.asp";

// �A���P�[�g�����擾����A�h���X
static const char* ENC_URL_GET_QUESTIONNAIRE = "/web/enc/lobby/getQuestionnaire.asp";

static const char* ENC_URL_SUBMIT_QUESTIONNAIRE = "/web/enc/lobby/submitQuestionnaire.asp";

// �^�C�}�[ID
static const u32 PPW_LOBBY_TIMER_ID_SYSTEM_CHANNEL_DATA                 = 1;
static const u32 PPW_LOBBY_TIMER_ID_USER_CHANNEL_DATA                   = 2;
static const u32 PPW_LOBBY_TIMER_ID_RECRUIT                             = 3;
static const u32 PPW_LOBBY_TIMER_ID_CHECK_MAINCHANNEL_MODE              = 4;
static const u32 PPW_LOBBY_TIMER_ID_CHECK_MAINCHANNEL_SYSTEMDATA        = 5;
static const u32 PPW_LOBBY_TIMER_ID_CHECK_LOBBY_INFO                    = 6;

// �^�C�}�[�C���^�[�o��(�b)
static const u32 PPW_LOBBY_TIMER_INTERVAL_SYSTEM_CHANNEL_DATA           = 2;
static const u32 PPW_LOBBY_TIMER_INTERVAL_USER_CHANNEL_DATA             = 3;
static const u32 PPW_LOBBY_TIMER_INTERVAL_RECRUIT                       = 5;
static const u32 PPW_LOBBY_TIMER_INTERVAL_CHECK_MAINCHANNEL_MODE        = 3;
static const u32 PPW_LOBBY_TIMER_INTERVAL_CHECK_MAINCHANNEL_SYSTEMDATA  = 4;
static const u32 PPW_LOBBY_TIMER_INTERVAL_CHECK_LOBBY_INFO              = 4;



// �X�^�e�B�b�N�ϐ��錾
static PPW_Lobby* s_lobby = NULL;

// �O���[�o���֐�
PPW_Lobby* PPW_GetLobby()
{
    return s_lobby;
}

// ���[�J���֐��v���g�^�C�v
static PPW_LOBBY_RESULT PPW_LobbyInitializeImplAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                                     const PPW_LobbyCallbacks* lobbyCallbacks, s32 pid, const u8* userProfile, u32 userProfileSize,
                                                     BOOL isValidInitialize);
static PPW_LOBBY_RESULT PPW_LobbyEnterMainChannel();
static void StartDownload();
static DWCi_String PPW_LobbyCreateBaseChannelName(PPW_LOBBY_CHANNEL_KIND nameKind, const DWCi_String& uniqueName);
static DWCi_String PPW_LobbyGenerateUniqueChannelName();
static BOOL PPW_IsMainChannelReady();
static void PPW_ChannelDataHandler(BOOL success, BOOL broadcast, const char* channelName, s32 userId, const char* key, const char* value, void* param);
static BOOL PPW_SystemChannelDataHandler(BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channelKind, s32 userId,
                                         const char* key, const u8* data, u32 dataSize, void* param);

// �^�C�}�[�R�[���o�b�N
static BOOL PPW_LobbyBroadcastMyUserProfile(void* param);
static BOOL PPW_LobbyBroadcastMySystemProfile(void* param);
static BOOL PPW_LobbySendRecruitInfo(void* param);
static BOOL PPW_LobbyCheckMainChannelMode(void* param);
static BOOL PPW_LobbyCheckLobbyInfo(void* param);

// ENC�R�[���o�b�N
static void CheckProfileCompletedCallback(BOOL success, const u8* data, u32 size, void* param);
static void GetScheduleCompletedCallback(BOOL success, const u8* data, u32 size, void* param);
static void GetVipCompletedCallback(BOOL success, const u8* data, u32 size, void* param);
static void GetQuestionnaireCompletedCallback(BOOL success, const u8* data, u32 size, void* param);
static void SubmitQuestionnaireCompletedCallback(BOOL success, const u8* data, u32 size, void* param);

// ���ʃ��C�u��������̃R�[���o�b�N
static void LobbyJoinMainChannelCallback(BOOL success, DWCi_LOBBY_ENTER_RESULT result, const char* channelName, void* param);
static void LobbyJoinSubChannelCallback(BOOL success, DWCi_LOBBY_ENTER_RESULT result, const char* channelName, void* param);
static void LobbyPlayerConnectedCallback(const char* channelName, s32 userId, void* param);
static void LobbyPlayerDisconnectedCallback(const char* channelName, s32 userId, void* param);
static void LobbyChannelMassageReceivedCallback(const char* channelName, s32 userId, const char* message, void* param);
static void LobbyUserMassageReceivedCallback(s32 userId, const char* message, void* param);
static void LobbyExcessFloodCallback(u32 floodWeight, void* param);
static void LobbyBroadcastDataCallback(const char* channelName, s32 userId,
                                       const char* key, const char* value, void* param);
static void LobbyGetChannelDataCallback(BOOL success, const char* channelName, s32 userId,
                                        const char* key, const char* value, void* param);
static void LobbyChannelDataCallback(BOOL success, BOOL broadcast, const char* channelName, s32 userId,
                                        const char* key, const char* value, void* param);
static void LobbyEnumChannelsCallback(BOOL success, s32 numChannels, const char** channels, const char** topics, const s32* numUsers, void* param);
static void LobbyEnumUsersCallback(BOOL success, const char* channelName, s32 numUsers, const s32* userIds, void* param);
static void LobbyCheckMainChannelModeCallback(BOOL success, const char* channelName, const DWCi_LobbyChannelMode* mode, void* param);
static void LobbyCheckMainChannelSystemProfileCallback(BOOL success, const char* channelName, s32 userId, const char* key, const char* value, void* param);
static void LobbyCheckLobbyInfoCallback(BOOL success, const char* channelName, s32 userId, const char* key, const char* value, void* param);
static void LobbyTopicChangedCallback(const char* channelName, const char* topic, void* param);


// result�R�[�h�ϊ�
static inline PPW_LOBBY_RESULT ToPpwResult(DWCi_LOBBY_RESULT result)
{
    // �����ŃR���p�C���G���[�����������ꍇ��result�R�[�h�̐��������Ƃ�Ă��Ȃ��̂ŗv�C��
    DWCi_STATIC_ASSERT(DWCi_LOBBY_RESULT_MAX == PPW_LOBBY_RESULT_MAX);
    
    PPW_LOBBY_RESULT convertedResult = (PPW_LOBBY_RESULT)result;
    
    // �v���I�ȃG���[���������Ă���Ƃ��͂�������G���[���Z�b�g����B
    s_lobby->SetError(convertedResult);
    
    return convertedResult;
}

// ����������Ă��邩�`�F�b�N����
#define RETURN_IF_NOTINITIALIZED(retval) if(!s_lobby){return retval;}

// �v���I�ȃG���[���N���Ă��Ȃ����`�F�b�N����
#define RETURN_IF_ERROR_STATE(retval) \
    if(s_lobby && s_lobby->GetState() == PPW_LOBBY_STATE_ERROR)\
    {\
        return retval;\
    }

// �X�e�[�g��READY�ł��邩�`�F�b�N����
#define RETURN_IF_NOTREADY(retval) \
    if(s_lobby && s_lobby->GetState() != PPW_LOBBY_STATE_READY)\
    {\
        return retval;\
    }

// ENC�ő��M�����{���
class EncCommonRequest : public DWCi_Base
{
    s32 pid;
    u16 requestVersion;
    u8 romVersion;
    u8 language;
    u8 macAddr[6];
    u16 reserve1;
    u32 reserve2;
    
public:
    EncCommonRequest()
    {
        const PPW_LobbyInnerProfile::UserProfile& userProfile = s_lobby->GetProfileManager().GetMyProfile().GetUserProfile();
        const WFLBY_USER_PROFILE* userProfileDetail = (const WFLBY_USER_PROFILE*)DWCi_GetVectorBuffer(userProfile.data);
    
        pid = s_lobby->GetPid();
        requestVersion = PPW_LOBBY_ENC_REQUEST_VERSION;
        romVersion = userProfileDetail->rom_code;
        language = userProfileDetail->region_code;
        DWCi_Np_GetMacAddress(macAddr);
    }
    // �p���֎~
    ~EncCommonRequest(){};
};


/** 
 * @brief ���r�[���C�u���������������܂��B
 * 
 * ���r�[���C�u���������������܂��B\n
 * ���̊֐����ĂԑO�ɂ��炩����DWC_LoginAsync�֐�����DWC_NASLoginAsync�֐���Wi-Fi�R�l�N�V�����ւ̐ڑ����������Ă���K�v������܂��B\n
 * ���̊֐������������ꍇ��::PPW_LobbyProcess�֐��𖈃Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B
 * 
 * @param[in] gameName NULL�I�[���ꂽ�Q�[�����B
 * @param[in] secretKey NULL�I�[���ꂽ�V�[�N���b�g�L�[�B
 * @param[in] prefix �`�����l�����ɕt������v���t�B�b�N�X�B���i�p�A�f�o�b�O�p�A�e�X�g�p�œ���`�����l���𕪂��邱�Ƃ��ł��܂��B\n
 *                   ���i�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_RELEASE���A�f�o�b�O�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_DEBUG���w�肵�Ă��������B\n
 *                   ���̒l�̓X�P�W���[����VIP�ݒ�f�[�^�̎擾��ɂ��e�����܂��B::DWC_LOBBY_CHANNEL_PREFIX_RELEASE�̓����[�X�T�[�o����A
 *                   ����ȊO�ł̓f�o�b�O�T�[�o����f�[�^���擾���܂��B
 * @param[in] lobbyCallbacks �Z�b�g����R�[���o�b�N�B
 * @param[in] dwcUserData �L����DWC���[�U�f�[�^�I�u�W�F�N�g�B
 * @param[in] userProfile �����I�ɋ��L����郆�[�U��`�v���t�B�[���̏����f�[�^�B
 * @param[in] userProfileSize userProfile�̃T�C�Y�B1�ȏ�::PPW_LOBBY_MAX_BINARY_SIZE�ȉ����w�肵�Ă��������B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_ALLOC �������m�ۂɎ��s�B
 * @retval ::PPW_LOBBY_RESULT_ERROR_SESSION �ʐM�G���[�B
 */
PPW_LOBBY_RESULT PPW_LobbyInitializeAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                     const PPW_LobbyCallbacks* lobbyCallbacks, const DWCUserData* dwcUserData, const u8* userProfile, u32 userProfileSize)
{
    // ���[�U�f�[�^�̐�����������
    if( !DWC_CheckUserData( dwcUserData )
        || !DWC_CheckHasProfile( dwcUserData ))
    {
        // �s���ȃ��[�U�f�[�^
        DWC_Printf(DWC_REPORTFLAG_ERROR, "PPW_LobbyInitializeAsync: Invalid dwcUserData.\n");
        return PPW_LOBBY_RESULT_ERROR_PARAM;
    }
    
    return PPW_LobbyInitializeImplAsync(gameName, secretKey, prefix, lobbyCallbacks, dwcUserData->gs_profile_id, userProfile, userProfileSize, TRUE);
}

/** 
 * @brief �C�ӂ�GameSpyProfileID�Ń��r�[���C�u���������������܂��B(�f�o�b�O�p)
 * 
 * ���r�[���C�u���������������܂��B(�f�o�b�O�p)\n
 * ���̊֐����ĂԑO�ɂ��炩����DWC_LoginAsync�֐�����DWC_NASLoginAsync�֐���Wi-Fi�R�l�N�V�����ւ̐ڑ����������Ă���K�v������܂��B\n
 * ::PPW_LobbyInitializeAsync�֐���DWCUserData���ɋL�^����Ă���GameSpyProfileID���g�p����̂ɑ΂��A������͔C�ӂ�GameSpyProfileID�ŏ������ł��܂��B\n
 * ���̊֐������������ꍇ��::PPW_LobbyProcess�֐��𖈃Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B
 * 
 * @param[in] gameName NULL�I�[���ꂽ�Q�[�����B
 * @param[in] secretKey NULL�I�[���ꂽ�V�[�N���b�g�L�[�B
 * @param[in] prefix �`�����l�����ɕt������v���t�B�b�N�X�B���i�p�A�f�o�b�O�p�A�e�X�g�p�œ���`�����l���𕪂��邱�Ƃ��ł��܂��B\n
 *                   ���i�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_RELEASE���A�f�o�b�O�p�ɂ�::DWC_LOBBY_CHANNEL_PREFIX_DEBUG���w�肵�Ă��������B
 * @param[in] lobbyCallbacks �Z�b�g����R�[���o�b�N�B
 * @param[in] pid GameSpyProfileID�B10���ȏ�̐��̐����w�肵�Ă��������B
 * @param[in] userProfile �����I�ɋ��L����郆�[�U��`�v���t�B�[���B
 * @param[in] userProfileSize userProfile�̃T�C�Y�B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE ���łɏ���������Ă��܂��B
 * @retval ::PPW_LOBBY_RESULT_ERROR_ALLOC �������m�ۂɎ��s�B
 * @retval ::PPW_LOBBY_RESULT_ERROR_SESSION �ʐM�G���[�B
 */
PPW_LOBBY_RESULT PPW_LobbyInitializePidAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                     const PPW_LobbyCallbacks* lobbyCallbacks, s32 pid, const u8* userProfile, u32 userProfileSize)
{
    return PPW_LobbyInitializeImplAsync(gameName, secretKey, prefix, lobbyCallbacks, pid, userProfile, userProfileSize, FALSE);
}

PPW_LOBBY_RESULT PPW_LobbyInitializeImplAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                     const PPW_LobbyCallbacks* lobbyCallbacks, s32 pid, const u8* userProfile, u32 userProfileSize, BOOL isValidInitialize)
{
    if(s_lobby)
    {
        // ���łɏ���������Ă���
        DWC_Printf(DWC_REPORTFLAG_ERROR, "PPW_LobbyInitializeImplAsync: Already initialized.");
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }
    
    DWC_ASSERTMSG(userProfile && userProfileSize != 0, "PPW_LobbyInitializeImplAsync: userProfile must be set.");
    
    DWCi_LobbyGlobalCallbacks gc;
    gc.lobbyUserMassageReceivedCallback = LobbyUserMassageReceivedCallback;
    gc.lobbyExcessFloodCallback = LobbyExcessFloodCallback;
    gc.param = (void*)PPW_LOBBY_CHANNEL_KIND_INVALID;
    
    s_lobby = new PPW_Lobby(*lobbyCallbacks, pid, prefix, isValidInitialize);
    if(!s_lobby)
    {
        return PPW_LOBBY_RESULT_ERROR_ALLOC;
    }

    PPW_LOBBY_RESULT result = ToPpwResult(DWCi_LobbyInitializePidAsync(gameName, secretKey, prefix, &gc, pid));
    if(result != PPW_LOBBY_RESULT_SUCCESS)
    {
        DWC_SAFE_DELETE(s_lobby);
        return result;
    }

    // �����̐ݒ�B���������̓��C���`�����l���������ɉ��߂ăZ�b�g�����B
    s_lobby->GetProfileManager().InitializeMyProfile(pid, userProfile, userProfileSize);

    // �`���b�g�T�[�o�ڑ��Ɠ����Ƀ_�E�����[�h�J�n
    StartDownload();
    
    s_lobby->SetState(PPW_LOBBY_STATE_CONNECTING);
    return PPW_LOBBY_RESULT_SUCCESS;
}

/** 
 * @brief ���r�[���C�u�������I�����܂��B
 * 
 * ���r�[���C�u�������I�����܂��B\n
 * ::PPW_LobbyProcess�֐���::PPW_LOBBY_STATE_NOTINITIALIZED��Ԃ��ƃV���b�g�_�E�������ł��B
 * ::PPW_LobbyShutdownAsync�֐��Ɠ�����DWC_CleanupInetAsync�֐����Ăяo���Ȃ��悤�ɂ��Ă��������B
 * ���r�[���C�u�����̏I�����m�F�������DWC_CleanupInetAsync�֐����Ăяo���Ă��������B
 * ����ȊO�ɌĂяo���^�C�~���O�ɐ����͂���܂���B���łɃ��r�[���C�u�������I�����Ă���ꍇ�͉������܂���B\n
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 */
PPW_LOBBY_RESULT PPW_LobbyShutdownAsync()
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_SUCCESS);
    
    // ���łɃV���b�g�_�E�����̂Ƃ��͉������Ȃ�
    if(s_lobby->GetState() == PPW_LOBBY_STATE_CLOSING)
    {
        return PPW_LOBBY_RESULT_SUCCESS;
    }
    
    // ����ȏ�^�C�}�[���������Ȃ��悤�Ƀ^�C�}�[�j��
    s_lobby->GetTimerManager().ClearTimer();
    
    s_lobby->SetState(PPW_LOBBY_STATE_CLOSING);
    return ToPpwResult(DWCi_LobbyShutdownAsync());
}

/** 
 * @brief ���r�[���C�u�����̏�����i�߂܂��B
 * 
 * ���r�[���C�u�����̏�����i�߂܂��B\n
 * ���Q�[���t���[�����x�̕p�x�ŌĂяo���Ă��������B\n
 * ::PPW_LOBBY_STATE_ERROR��Ԃ����Ƃ���PPW_LobbyGetLastError�֐��ŃG���[���e���擾��::PPW_LobbyShutdownAsync�֐����Ă�ŃV���b�g�_�E�����Ă��������B\n
 * ::PPW_LOBBY_STATE_READY�̂Ƃ��̂݃��b�Z�[�W���M�A�`�����l�������Ȃǂ̊֐����Ăׂ܂��B
 * 
 * @retval ::PPW_LOBBY_STATE_NOTINITIALIZED       ���C�u����������������Ă��܂���B
 * @retval ::PPW_LOBBY_STATE_CONNECTING           �`���b�g�T�[�o�ɐڑ����B
 * @retval ::PPW_LOBBY_STATE_CONNECTED            �`���b�g�T�[�o�ɐڑ������B
 * @retval ::PPW_LOBBY_STATE_DOWNLOAD             �T�[�o����ݒ���_�E�����[�h�J�n�B
 * @retval ::PPW_LOBBY_STATE_DOWNLOAD_WAIT        �T�[�o����ݒ���_�E�����[�h���B
 * @retval ::PPW_LOBBY_STATE_SEARCHINGCHANNEL     ���C���`�����l�����������B
 * @retval ::PPW_LOBBY_STATE_ENTERMAINCHANNEL     ���C���`�����l���ɓ����J�n�B
 * @retval ::PPW_LOBBY_STATE_PREPARINGMAINCHANNEL ���C���`�����l���ɓ����y�я������B
 * @retval ::PPW_LOBBY_STATE_READY                ���C���`�����l���ɓ��������B
 * @retval ::PPW_LOBBY_STATE_CLOSING              ���C�u�����I���������B
 * @retval ::PPW_LOBBY_STATE_ERROR                ���C�u�����ɒv���I�ȃG���[���������B
 */
PPW_LOBBY_STATE PPW_LobbyProcess()
{
    {
        // �_�E�����[�h����
        BOOL bEncSession = DWCi_EncSession::Process();
        
        if(bEncSession)
        {
            // ���r�[���C�u�������V���b�g�_�E���ς݂ł��_�E�����[�h�����܂ő҂B
            RETURN_IF_NOTINITIALIZED(PPW_LOBBY_STATE_CLOSING);
        }
        else
        {
            // NOTINITIALIZED��Ԃ��̂͂����̂�
            RETURN_IF_NOTINITIALIZED(PPW_LOBBY_STATE_NOTINITIALIZED);
        }
    }
    
    RETURN_IF_ERROR_STATE(PPW_LOBBY_STATE_ERROR);

    // �C���i�[���C�u�����̏�Ԋm�F
    switch(DWCi_LobbyProcess())
    {
    case DWCi_LOBBY_STATE_NOTINITIALIZED:
        // ���ʃ��C�u�������I�����Ă���̂ł��̃��C�u�������I��
        PPW_LobbyShutdownAsync();    // �O�̂��߃V���b�g�_�E�����Ă�
        DWC_SAFE_DELETE(s_lobby);
        return PPW_LOBBY_STATE_CLOSING;
    case DWCi_LOBBY_STATE_CONNECTING:
        break;
    case DWCi_LOBBY_STATE_CONNECTINGWAIT:
        break;
    case DWCi_LOBBY_STATE_CONNECTED:
        if(s_lobby->GetState() == PPW_LOBBY_STATE_CONNECTING)
        {
            // �ŏ��ɐڑ������Ƃ��̂݃��C���`�����l�������̃X�e�b�v�ɏ�ԑJ�ڂ�����
            s_lobby->SetState(PPW_LOBBY_STATE_CONNECTED);
        }
        break;
    case DWCi_LOBBY_STATE_CLOSING:
        break;
    case DWCi_LOBBY_STATE_ERROR:
        s_lobby->SetError(DWCi_LobbyGetLastError());
        break;
    default:
        DWC_ASSERTMSG(FALSE, "PPW_LobbyProcess: Inner lib switch is invalid state");
        break;
    }

    // �Ǘ��I�u�W�F�N�g����
    s_lobby->ProcessManagers();

    // ���̃��C�u�����̏�ԍX�V
    switch(s_lobby->GetState())
    {
    case PPW_LOBBY_STATE_NOTINITIALIZED:
        // �����ɂ͗��Ȃ�
        DWC_ASSERTMSG(FALSE, "PPW_LobbyProcess: Internal Error.");
        return PPW_LOBBY_STATE_CLOSING;
    case PPW_LOBBY_STATE_CONNECTING:
        break;
    case PPW_LOBBY_STATE_CONNECTED:
        // �������Ȃ��B�_�E�����[�h�J�n������Initialize���ɍs���Ă���B
        s_lobby->SetState(PPW_LOBBY_STATE_DOWNLOAD);
        break;
    case PPW_LOBBY_STATE_DOWNLOAD:
        // �_�E�����[�h�������������`�F�b�N
        if(s_lobby->GetLobbyInnerInfoManager().IsDownloaded())
        {
            s_lobby->SetState(PPW_LOBBY_STATE_DOWNLOAD_WAIT);
        }
        break;
    case PPW_LOBBY_STATE_DOWNLOAD_WAIT: // state�̖��O�������������ǂ��̂܂�
        // �`�����l���T�[�`�J�n
        DWCi_LobbyEnumChannelsAsync((PPW_LobbyCreateBaseChannelName(PPW_LOBBY_CHANNEL_KIND_MAIN, "*")).c_str(), LobbyEnumChannelsCallback, NULL);
        s_lobby->SetState(PPW_LOBBY_STATE_SEARCHINGCHANNEL);
        break;
    case PPW_LOBBY_STATE_SEARCHINGCHANNEL:
        break;
    case PPW_LOBBY_STATE_ENTERMAINCHANNEL:
        {
            // ���C���`�����l���ɓ���B�����G���[������΃Z�b�g����B
            PPW_LOBBY_RESULT ret = PPW_LobbyEnterMainChannel();
            if(ret != PPW_LOBBY_RESULT_SUCCESS)
            {
                s_lobby->SetError(ret);
            }
            else
            {
                s_lobby->SetState(PPW_LOBBY_STATE_PREPARINGMAINCHANNEL);
            }
        }
        break;
    case PPW_LOBBY_STATE_PREPARINGMAINCHANNEL:
        // ���C���`�����l���ڑ��m�F
        if(PPW_IsMainChannelReady())
        {
            // �S�Ă̏��擾���I������̂ŏ��������𑼂̃v���C���[�ɒʒm����
            PPW_LobbyInnerProfile& profile = s_lobby->GetProfileManager().GetMyProfile();
            PPW_LobbyInnerProfile::SystemProfile systemProfile = profile.GetSystemProfileConfirm();
            systemProfile.ready = TRUE;
            profile.SetSystemProfile(systemProfile);
            
            DWC_Printf(DWC_REPORTFLAG_INFO, "Main channel is ready.\n");
            s_lobby->SetState(PPW_LOBBY_STATE_READY);
            s_lobby->GetCallbacks().lobbyJoinChannelCallback(TRUE, PPW_LOBBY_CHANNEL_KIND_MAIN);
        }
        break;
    case PPW_LOBBY_STATE_READY:
        break;
    case PPW_LOBBY_STATE_CLOSING:
        break;
    case PPW_LOBBY_STATE_ERROR:
        break;
    default:
        DWC_ASSERTMSG(FALSE, "PPW_LobbyProcess: Invalid state");
        break;
    }
    
    return s_lobby->GetState();
}

/** 
 * @brief ���Ճ{�[�h�A���E���v�A���r�[�j���[�X(�T�u�`�����l��)�ɎQ�����܂��B
 * 
 * ���Ճ{�[�h�A���E���v�A���r�[�j���[�X�ɎQ�����܂��B�����������܂��͎��s�����::PPW_LobbyJoinChannelCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] channelKind �Q������`�����l���̎�ށB
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂Ȃ����A���łɂ����ꂩ�̃T�u�`�����l���ɓ����Ă��܂��B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyJoinSubChannelAsync(PPW_LOBBY_CHANNEL_KIND channelKind)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    if(s_lobby && s_lobby->GetSubChannel().Exist())
    {
        // ���łɃT�u�`�����l���ɎQ�����Ă���
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }

    DWCi_LobbyChannelCallbacks cc;
    cc.lobbyPlayerConnectedCallback = LobbyPlayerConnectedCallback;
    cc.lobbyPlayerDisconnectedCallback = LobbyPlayerDisconnectedCallback;
    cc.lobbyChannelMassageReceivedCallback = LobbyChannelMassageReceivedCallback;
    cc.lobbyBroadcastDataCallback = LobbyBroadcastDataCallback;
    cc.lobbyTopicChangedCallback = LobbyTopicChangedCallback;
    cc.param = (void*)channelKind;
    
    // �T�u�`�����l���ɓ���B
    DWCi_String channelName = PPW_LobbyCreateBaseChannelName(channelKind, s_lobby->GetUniqueChannelName());
    const DWCi_LobbyChannelMode* mode = PPW_LobbyGetChannelMode(channelKind);
    PPW_LOBBY_RESULT result = ToPpwResult(DWCi_LobbyJoinChannelLimitAsync(channelName.c_str(), NULL, mode->limit,
                                                                          &cc, LobbyJoinSubChannelCallback, (void*)channelKind));
    if(result != PPW_LOBBY_RESULT_SUCCESS)
    {
        return result;
    }
    s_lobby->SetSubChannel(PPW_Channel(channelKind, channelName));
    return PPW_LOBBY_RESULT_SUCCESS;
}

/** 
 * @brief �Q�����̑��Ճ{�[�h�A���E���v�A���r�[�j���[�X(�T�u�`�����l��)����o�܂��B
 * 
 * �Q�����̑��Ճ{�[�h�A���E���v�A���r�[�j���[�X(�T�u�`�����l��)����o�܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂Ȃ����A�T�u�`�����l���ɓ����Ă��܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyLeaveSubChannel()
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    if(!(s_lobby && s_lobby->GetSubChannel().Exist()))
    {
        // �T�u�`�����l���ɎQ�����Ă��Ȃ�
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }

    PPW_LOBBY_RESULT result = ToPpwResult(DWCi_LobbyLeaveChannel(s_lobby->GetSubChannel().GetName().c_str()));
    if(result != PPW_LOBBY_RESULT_SUCCESS)
    {
        DWC_ASSERTMSG(FALSE, "PPW_LobbyLeaveSubChannel: Internal error.");
    }
    
    s_lobby->GetSubChannel().Clear();
    
    // �V�X�e���f�[�^���X�V
    PPW_LobbyInnerProfile& profile = s_lobby->GetProfileManager().GetMyProfile();
    PPW_LobbyInnerProfile::SystemProfile systemProfile = profile.GetSystemProfileConfirm();
    systemProfile.subChannelKind = PPW_LOBBY_CHANNEL_KIND_INVALID;
    profile.SetSystemProfile(systemProfile);
    
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief ���������擾���܂��B
 * 
 * ���������擾���܂��B
 * 
 * @param[out] timeInfo ���������i�[����̈���w�肵�Ă��������B���������b�N����Ă��Ȃ��ꍇ�AlockedTime�����o��::PPW_LOBBY_INVALID_TIME�ƂȂ�܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyGetTimeInfo(PPW_LobbyTimeInfo* timeInfo)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    s_lobby->GetLobbyInnerInfoManager().GetTimeInfo(*timeInfo);
    
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �X�P�W���[���������I�ɃZ�b�g���܂��B(�f�o�b�O�p)
 * 
 * �X�P�W���[���������I�ɃZ�b�g���܂��B(�f�o�b�O�p)\n
 * ::PPW_LobbyInitializeAsync�֐��܂���::PPW_LobbyInitializePidAsync�֐��ŏ�������������ɌĂяo���Ă��������B
 * 
 * @param[in] schedule �Z�b�g����X�P�W���[���B�C�x���g�Ƃ���::PPW_LOBBY_TIME_EVENT_LOCK���w�肷�邱�Ƃ͂ł��܂���B
 * @param[in] scheduleSize schedule�Ɋm�ۂ�����������(byte)�B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM schedule��scheduleRecordNum�����o����v�Z�����\���̗e�ʂ�size�������Ă��܂��B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE ����������Ă��܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySetSchedule(const PPW_LobbySchedule* schedule, u32 scheduleSize)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);

    return s_lobby->GetLobbyInnerInfoManager().SetLobbyInfo((u8*)schedule, scheduleSize) ? PPW_LOBBY_RESULT_SUCCESS : PPW_LOBBY_RESULT_ERROR_PARAM;
}

/**
 * @brief �X�P�W���[�����擾���܂��B
 * 
 * �X�P�W���[�����擾���܂��B\n
 * ::PPW_LobbyJoinChannelCallback�R�[���o�b�N���������A���C���`�����l���ɓ��������Ƃ��ɃX�P�W���[�����m�肵�܂��̂ł���ȍ~�Ɏg�p���邱�Ƃ��ł��܂��B
 * ����ȑO�ɌĂяo�����ꍇ�͏����ݒ�̃X�P�W���[�����擾����邩�A::PPW_LobbySetSchedule�֐��ŃZ�b�g���Ă���΂��̃X�P�W���[�����擾����܂��B\n
 * �X�P�W���[����ۑ�����̈�������ɓn���Ă��������B\n
 * schedule��NULL�A�������͕K�v�ȃT�C�Y��scheduleSize��������Ă����ꍇ��scheduleSize�ɕK�v�ȃT�C�Y���i�[����܂��B
 * 
 * @param[in, out] schedule scheduleSize�̗̈���m�ۂ����A�X�P�W���[�����i�[����|�C���^�B�K�v�ȃT�C�Y�𖞂����Ă���ꍇ�̂݃X�P�W���[�����i�[����܂��B
 * @param[in, out] scheduleSize �m�ۂ���schedule�̃T�C�Y(byte)���w�肵�Ă��������B�֐��͓����Ŏ��ۂ�schedule�̃T�C�Y�������܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����Bschedule�ɃX�P�W���[�����i�[����AscheduleSize�ɂ��̃T�C�Y���i�[����܂��B
 *                                  ������scheduleSize��0�������ꍇ��schedule�͕ω����܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM schedule��NULL��������size���K�v�T�C�Y��������Ă��܂��BscheduleSize�ȏ�̃��������m�ۂ��čĎ��s���Ă��������B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyGetSchedule(PPW_LobbySchedule* schedule, u32* scheduleSize)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    return s_lobby->GetLobbyInnerInfoManager().GetLobbyInfo(schedule, *scheduleSize) ? PPW_LOBBY_RESULT_SUCCESS : PPW_LOBBY_RESULT_ERROR_PARAM;
}

/**
 * @brief VIP�����擾���܂��B
 * 
 * VIP�����擾���܂��B\n
 * ::PPW_LobbyGetVipCallback�R�[���o�b�N���Ăяo���ꂽ�ォ��g�p���邱�Ƃ��ł��܂��B����ȑO�ɌĂяo�����ꍇ��recordNum���K��0�ɂȂ�܂��B
 * VIP����ۑ�����̈�������ɓn���Ă��������B\n
 * records��NULL�A�������͕K�v�Ȍ���recordNum��������Ă����ꍇ��recordNum�ɕK�v�Ȍ����i�[����܂��B
 * 
 * @param[in, out] records recordNum*sizeof(::PPW_LobbyVipRecord)�̗̈���m�ۂ����AVIP�����i�[����|�C���^�B
 *                         �K�v�ȃT�C�Y�𖞂����Ă���ꍇ�̂�VIP��񂪊i�[����܂��B
 * @param[in, out] recordNum �m�ۂ���records�̌����w�肵�Ă��������B�֐��͎��ۂ�recordNum�̌��������܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����Brecords��VIP��񂪊i�[����ArecordNum�ɂ��̃T�C�Y���i�[����܂��B
 *                                  ������recordNum��0�������ꍇ��records�͕ω����܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM records��NULL��������recordNum���K�v�T�C�Y��������Ă��܂��B
 *                                        recordNum*sizeof(::PPW_LobbyVipRecord)�ȏ�̃��������m�ۂ��čĎ��s���Ă��������B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyGetVip(PPW_LobbyVipRecord* records, u32* recordNum)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    
    return s_lobby->GetLobbyInnerInfoManager().GetVipRecords(records, *recordNum) ? PPW_LOBBY_RESULT_SUCCESS : PPW_LOBBY_RESULT_ERROR_PARAM;
}

/**
 * @brief �Q�����̑��Ճ{�[�h�A���E���v�A���r�[�j���[�X(�T�u�`�����l��)�̏�Ԃ��擾���܂��B
 * 
 * �Q�����̑��Ճ{�[�h�A���E���v�A���r�[�j���[�X(�T�u�`�����l��)�̏�Ԃ��擾���܂��B
 * 
 * @retval ::PPW_LOBBY_CHANNEL_STATE_NONE �T�u�`�����l���ɎQ�����Ă��܂���B
 * @retval ::PPW_LOBBY_CHANNEL_STATE_UNAVAILABLE �T�u�`�����l���ɓ����������ł��B
 * @retval ::PPW_LOBBY_CHANNEL_STATE_AVAILABLE �T�u�`�����l���ɓ����ς݂ł��B
 * @retval ::PPW_LOBBY_CHANNEL_STATE_ERROR �v���I�ȃG���[�ɂ���Ԃ��擾�ł��܂���ł����B�܂��̓X�e�[�g��READY�ł͂���܂���B
 */
PPW_LOBBY_CHANNEL_STATE PPW_LobbyGetSubChannelState()
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_CHANNEL_STATE_ERROR);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_CHANNEL_STATE_ERROR);
    RETURN_IF_NOTREADY(PPW_LOBBY_CHANNEL_STATE_ERROR);
    
    if(!s_lobby->GetSubChannel().Exist())
    {
        // �T�u�`�����l���ɎQ�����Ă��Ȃ�
        return PPW_LOBBY_CHANNEL_STATE_NONE;
    }

    BOOL state = DWCi_LobbyInChannel(s_lobby->GetSubChannel().GetName().c_str());
    return state ? PPW_LOBBY_CHANNEL_STATE_AVAILABLE : PPW_LOBBY_CHANNEL_STATE_UNAVAILABLE;
}

/**
 * @brief �`�����l���f�[�^���Z�b�g���܂��B
 * 
 * �`�����l���f�[�^���Z�b�g���܂��B\n
 * �`�����l���f�[�^�͎Q�����Ă���`�����l���̃v���C���[��l��l�ƃ`�����l�����̂ɃZ�b�g���邱�Ƃ��ł��܂��B\n
 * ���C�u�����Ŏg�p���Ă�����̂ƍ��킹�Ă��ꂼ��ɍő�20�̃`�����l���f�[�^���Z�b�g���邱�Ƃ��ł��܂��B\n
 * key��"b_"����n�܂镶������w�肷��ƁA�T�[�o�ɃZ�b�g����Ɠ����ɑ��̃v���C���[�ɂ����ʒm���܂��B(�u���[�h�L���X�g)\n
 * ���̃u���[�h�L���X�g�͎����ɂ����[�v�o�b�N����܂��B\n
 * �ʒm��::PPW_LobbyGetChannelDataCallback�R�[���o�b�N�ɂ��s���܂��B���̂Ƃ�������(broadcast)��TRUE�ƂȂ�܂��B
 * 
 * @param[in] userId �`�����l���f�[�^���Z�b�g���郆�[�UID�B::PPW_LOBBY_INVALID_USER_ID���w�肷��ƃ`�����l�����̂ɃZ�b�g���܂��B
 * @param[in] channelKind �`�����l���f�[�^���Z�b�g����`�����l���̎�ށB
 * @param[in] key �`�����l���f�[�^�ɃA�N�Z�X���邽�߂̃L�[�B
 * @param[in] data key�Ɍ��т���f�[�^�B
 * @param[in] dataSize data�̃T�C�Y�B1�ȏ�::PPW_LOBBY_MAX_BINARY_SIZE�ȉ��̒l���w�肵�Ă��������B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySetChannelData(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char* key, const u8* data, u32 dataSize)
{
    // TODO: channelKind�ł͂Ȃ��A���C���T�u���w�肷��悤�ɂ���
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);

    return PPW_LobbySetChannelDataImpl(userId, channelKind, key, data, dataSize);
}

/**
 * @brief �`�����l���f�[�^���擾���܂��B
 * 
 * �`�����l���f�[�^���擾���܂��B\n
 * ���������::PPW_LobbyGetChannelDataCallback�R�[���o�b�N���Ă΂�܂��B
 * 
 * @param[in] userId �`�����l���f�[�^���擾���郆�[�UID�B::PPW_LOBBY_INVALID_USER_ID���w�肷��ƃ`�����l�����̂���擾���܂��B
 * @param[in] channelKind �`�����l���f�[�^���擾����`�����l���̎�ށB
 * @param[in] key �`�����l���f�[�^�ɃA�N�Z�X���邽�߂̃L�[�B
 * @param[in] param ���̃��N�G�X�g�Ɋ֘A�Â���C�ӂ̒l�B::PPW_LobbyGetChannelDataCallback�R�[���o�b�N���Ă΂ꂽ�Ƃ���param�ɑ������܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyGetChannelDataAsync(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char* key, void* param)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    DWCi_String channelName = s_lobby->GetChannelManager().GetChannelName(channelKind);
    if(channelName == "")
    {
        return PPW_LOBBY_RESULT_ERROR_CHANNEL;
    }

    return ToPpwResult(DWCi_LobbyGetChannelDataAsync(channelName.c_str(), userId, key, LobbyGetChannelDataCallback, param));
}

/**
 * @brief Ascii������̃��b�Z�[�W���w�肵���`�����l���ɎQ�����Ă���S�Ẵv���C���[�ɑ��M���܂��B
 * 
 * Ascii������̃��b�Z�[�W���w�肵���`�����l���ɎQ�����Ă���S�Ẵv���C���[�ɑ��M���܂��B\n
 * ���b�Z�[�W�͎����ɂ����[�v�o�b�N����܂��B
 * 
 * @param[in] channelKind ���M��̃`�����l���B
 * @param[in] type ���̃��b�Z�[�W��\���C�ӂ̒l�B
 * @param[in] message ���M����NULL�I�[���ꂽAscii������B�I�[���݂�::PPW_LOBBY_MAX_STRING_SIZE�ȉ��̒����ł���K�v������܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySendChannelStringMessage(PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const char* message)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    return PPW_LobbySendMessage(PPW_LOBBY_INVALID_USER_ID, channelKind, PPW_LobbyMessage::FORMAT_STRING, PPW_LobbyMessage::TARGET_APPLICATION,
                                type, (u8*)message, strlen(message) + 1);
}

/**
 * @brief Ascii������̃��b�Z�[�W����l�̃v���C���[�ɑ��M���܂��B
 * 
 * Ascii������̃��b�Z�[�W����l�̃v���C���[�ɑ��M���܂��B
 * 
 * @param[in] userId ���M��̃��[�UID�B
 * @param[in] type ���̃��b�Z�[�W��\���C�ӂ̒l�B
 * @param[in] message ���M����NULL�I�[���ꂽAscii������B�I�[���݂�::PPW_LOBBY_MAX_STRING_SIZE�ȉ��̒����ł���K�v������܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySendPlayerStringMessage(s32 userId, s32 type, const char* message)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    return PPW_LobbySendMessage(userId, PPW_LOBBY_CHANNEL_KIND_INVALID, PPW_LobbyMessage::FORMAT_STRING, PPW_LobbyMessage::TARGET_APPLICATION,
                                type, (u8*)message, strlen(message) + 1);
}

/**
 * @brief �o�C�i���f�[�^���w�肵���`�����l���ɎQ�����Ă���S�Ẵv���C���[�ɑ��M���܂��B
 * 
 * �o�C�i���f�[�^���w�肵���`�����l���ɎQ�����Ă���S�Ẵv���C���[�ɑ��M���܂��B\n
 * ���b�Z�[�W�͎����ɂ����[�v�o�b�N����܂��B
 * 
 * @param[in] channelKind ���M��̃`�����l���B
 * @param[in] type ���̃��b�Z�[�W��\���C�ӂ̒l�B
 * @param[in] data ���M����o�C�i���f�[�^�B
 * @param[in] dataSize data�̃T�C�Y�B1�ȏ�::PPW_LOBBY_MAX_BINARY_SIZE�ȉ����w�肵�Ă��������B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySendChannelBinaryMessage(PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const u8* data, u32 dataSize)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    return PPW_LobbySendMessage(PPW_LOBBY_INVALID_USER_ID, channelKind, PPW_LobbyMessage::FORMAT_BASE64, PPW_LobbyMessage::TARGET_APPLICATION,
                                type, data, dataSize);
}

/**
 * @brief �o�C�i���f�[�^����l�̃v���C���[�ɑ��M���܂��B
 * 
 * �o�C�i���f�[�^����l�̃v���C���[�ɑ��M���܂��B
 * 
 * @param[in] userId ���M��̃��[�UID�B
 * @param[in] type ���̃��b�Z�[�W��\���C�ӂ̒l�B
 * @param[in] data ���M����o�C�i���f�[�^�B
 * @param[in] dataSize data�̃T�C�Y�B1�ȏ�::PPW_LOBBY_MAX_BINARY_SIZE�ȉ����w�肵�Ă��������B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySendPlayerBinaryMessage(s32 userId, s32 type, const u8* data, u32 dataSize)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    return PPW_LobbySendMessage(userId, PPW_LOBBY_CHANNEL_KIND_INVALID, PPW_LobbyMessage::FORMAT_BASE64, PPW_LobbyMessage::TARGET_APPLICATION,
                                type, data, dataSize);
}

/**
 * @brief ���r�[���C�u�����Ŏg�p���Ă��鎩���̃��[�UID���擾���܂��B
 * 
 * ���r�[���C�u�����Ŏg�p���Ă��鎩���̃��[�UID���擾���܂��B
 * 
 * @retval �����̃��[�UID�B
 * @retval ::PPW_LOBBY_INVALID_USER_ID ���������Ă��܂���B
 */
s32 PPW_LobbyGetMyUserId()
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_INVALID_USER_ID);
    return s_lobby->GetPid();
}

/**
 * @brief ���݃��C���`�����l���ɂ��鎩���ȊO�̃��[�UID��񋓂��܂��B
 * 
 * ���݃��C���`�����l���ɂ���l�̃��[�UID��񋓂��܂��B\n
 * ���[�UID�̃��X�g��ۑ�����̈�������ɓn���Ă��������B\n
 * userIds��NULL�A�������͕K�v�ȃT�C�Y��num��������Ă����ꍇ��num�ɕK�v�ȃT�C�Y���i�[����܂��B
 * 
 * @param[in, out] userIds ���[�UID�̃��X�g��ۑ�����̈��n���Ă��������BNULL���w�肷���num�Ƀ��[�UID�̌����i�[����܂��B
 * @param[in, out] num userIds�Ɋm�ۂ���s32�̌����w�肵�Ă��������B�o�C�g���ł͂���܂���B\n
 *                 �{�֐���::PPW_LOBBY_RESULT_SUCCESS��������::PPW_LOBBY_RESULT_ERROR_PARAM��Ԃ��ƃ��[�UID�̌����i�[����܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����BuserIds�Ƀ��[�UID�̃��X�g���i�[����Anum�ɂ��̌����i�[����܂��B\n
 *                                  ������num��0�������ꍇ��userIds�͕ω����܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM userIds��NULL��������num���K�v�T�C�Y��������Ă��܂��Bsizeof(s32)*num�ȏ�̃��������m�ۂ��čĎ��s���Ă��������B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyEnumUserId(s32* userIds, u32* num)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    u32 orgNum = *num;

    std::vector<s32, DWCi_Allocator<s32> > result = s_lobby->GetProfileManager().EnumId();
    *num = result.size();
    if(*num == 0)
    {
        return PPW_LOBBY_RESULT_SUCCESS;
    }

    if(orgNum < *num || userIds == NULL)
    {
        return PPW_LOBBY_RESULT_ERROR_PARAM;
    }

    DWCi_Np_CpuCopy8(DWCi_GetVectorBuffer(result), userIds, result.size() * sizeof(result[0]));
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief ���[�J���ɃL���b�V�����Ă��郆�[�U��`�v���t�B�[�����Q�Ƃ��܂��B
 * 
 * ���[�J���ɃL���b�V�����Ă��郆�[�U��`�v���t�B�[�����Q�Ƃ��܂��B\n
 * ���[�U��`�v���t�B�[����ۑ�����̈�������ɓn���Ă��������B\n
 * userProfile��NULL�A�������͕K�v�ȃT�C�Y��userProfileSize��������Ă����ꍇ��userProfileSize�ɕK�v�ȃT�C�Y���i�[����܂��B
 * 
 * @param[in] userId �擾���鑊��̃��[�UID�B::PPW_LOBBY_INVALID_USER_ID���w�肷��Ǝ����̃v���t�B�[�����Q�Ƃł��܂�
 * @param[in, out] userProfile userProfileSize�̗̈���m�ۂ����A���[�U��`�v���t�B�[�����i�[����|�C���^�B
 *                 �K�v�ȃT�C�Y�𖞂����Ă���ꍇ�̂݃��[�U��`�v���t�B�[�����i�[����܂��B
 * @param[in, out] userProfileSize �m�ۂ���userProfile�̃T�C�Y���w�肵�Ă��������B�֐��͎��ۂ�userProfile�̃T�C�Y�������܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����BuserProfile�Ƀ��[�U��`�v���t�B�[�����i�[����AuserProfileSize�ɂ��̃T�C�Y���i�[����܂��B
 *                                  ������userProfileSize��0�������ꍇ��userProfile�͕ω����܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM userProfile��NULL��������userProfileSize���K�v�T�C�Y��������Ă��܂��B
 *                                      userProfileSize�ȏ�̃��������m�ۂ��čĎ��s���Ă��������B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_NODATA ���[�U��`�v���t�B�[����������܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyGetProfile(s32 userId, u8* userProfile, u32* userProfileSize)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);

    if(userId == PPW_LOBBY_INVALID_USER_ID)
    {
        userId = s_lobby->GetPid();
    }

    PPW_LobbyInnerProfile* profile = s_lobby->GetProfileManager().FindProfile(userId);
    if(!profile)
    {
        return PPW_LOBBY_RESULT_ERROR_NODATA;
    }

    return profile->GetUserProfile(userProfile, userProfileSize) ? PPW_LOBBY_RESULT_SUCCESS : PPW_LOBBY_RESULT_ERROR_PARAM;
}

/**
 * @brief �����̃��[�U��`�v���t�B�[�����X�V���܂��B
 * 
 * �����̃��[�U��`�v���t�B�[�����X�V���܂��B�X�V�����f�[�^�͎����I�ɑ��̃v���C���[�ɒʒm����܂��B\n
 * ���ۂɒʒm�����ɂ͎኱�̎��Ԃ�������܂��B
 * 
 * @param[in] userProfile �Z�b�g���郆�[�U��`�v���t�B�[���B
 * @param[in] userProfileSize userProfile�̃T�C�Y�B
 * @param[in] force FALSE���w�肷��ƃf�[�^���O�񂩂���e���ύX����Ă��Ȃ��ꍇ�͏��𑗐M���܂���B\n
 *                  TRUE���w�肷��ƕύX����Ă��Ȃ��Ă������I�ɏ��𑗐M���܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyUpdateMyProfile(const u8* userProfile, u32 userProfileSize, BOOL force)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    PPW_LobbyInnerProfile& myProfile = s_lobby->GetProfileManager().GetMyProfile();

    myProfile.SetUserProfile(userProfile, userProfileSize, force);
    
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �~�j�Q�[�����W���܂��B
 * 
 * �~�j�Q�[�����W���܂��B\n
 * �֐������������recruitInfo��matchMakingString�����o�Ƀ}�b�`���C�N�w�W�����񂪊i�[����܂��̂�\n
 * DWC_AddMatchKeyString�֐���keyString��::PPW_LOBBY_MATCHMAKING_KEY�AvalueSrc��matchMakingString���w�肵�Ăяo������A\n
 * DWC_ConnectToAnybodyAsync�֐���addFilter�Ɂu::PPW_LOBBY_MATCHMAKING_KEY = 'matchMakingString'�v�ƂȂ镶������Z�b�g���Ăяo���Ă��������B\n
 * �{�֐��Ăяo����͎����I�Ɉ��Ԋu��recruitInfo�����̃v���C���[�ɑ��M����A::PPW_LobbyRecruitCallback�֐����R�[���o�b�N����܂��B\n
 * ��W���s���ɂ̓��r�[���C�u������::PPW_LobbyInitializeAsync�֐��ŏ���������Ă���K�v������܂��B
 * ::PPW_LobbyInitializePidAsync�֐��ŏ���������Ă����ꍇ��Assert���܂��B
 * 
 * @param[in, out] recruitInfo ���̃v���C���[�ɒʒm�����W�v���ł��B��W����Q�[���̎�ށA�ő�l���A���ݏW�܂��Ă���l��(���߂�0���w�肵�Ă�������)�A\n
 *                             ���̑��ʒm�������C�ӂ̒l���Z�b�g�������̂��w�肵�Ă��������B\n
 *                             matchMakingString�͖{�֐����ŃZ�b�g����܂��̂ŏ���������K�v�͂���܂���B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyStartRecruit(PPW_LobbyRecruitInfo* recruitInfo)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    DWC_ASSERTMSG(s_lobby->GetIsValidInitialize(), "PPW_LobbyStartRecruit: You must initialize by PPW_LobbyInitializeAsync() to recruit.");
    
    DWCi_String tempMatchMakingString;
    if(!s_lobby->StartRecruit(*recruitInfo, tempMatchMakingString))
    {
        // ���łɕ�W��
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }

    strncpy(recruitInfo->matchMakingString, tempMatchMakingString.c_str(), PPW_LOBBY_MAX_MATCHMAKING_STRING_LENGTH-1);
    
    // ��W�󋵑��M�^�C�}�[�N��
    s_lobby->GetTimerManager().AddTimer(PPW_LOBBY_TIMER_ID_RECRUIT, PPW_LOBBY_TIMER_INTERVAL_RECRUIT,
                                        PPW_LobbySendRecruitInfo, NULL, TRUE);
    
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �~�j�Q�[����W�v�����X�V���܂��B
 * 
 * ���Ԋu�ő��̃v���C���[�ɑ��M���Ă���~�j�Q�[����W�v����ύX���܂��B\n
 * �Ⴆ�Ό��ݏW�܂��Ă���l�����ω������Ƃ��ȂǂɎg�p���Ă��������B
 * 
 * @param[in] recruitInfo �X�V����f�[�^�B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �������s���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂Ȃ����A��W�����Ă��܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyUpdateRecruitInfo(const PPW_LobbyRecruitInfo* recruitInfo)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    if(!s_lobby->UpdateRecruitInfo(*recruitInfo))
    {
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �~�j�Q�[���̕�W���I�����܂��B
 * 
 * �~�j�Q�[���̕�W���I�����܂��B
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �X�e�[�g��READY�ł͂Ȃ����A��W�����Ă��܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbyStopRecruit()
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    RETURN_IF_NOTREADY(PPW_LOBBY_RESULT_ERROR_STATE);
    
    if(!s_lobby->StopRecruit())
    {
        // ��W���Ă��Ȃ�
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }
    
    // ��W�I����ʒm
    PPW_LobbySendMessage(s_lobby->GetPid(), PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LobbyMessage::FORMAT_BASE64, PPW_LobbyMessage::TARGET_SYSTEM,
                         PPW_LOBBY_MESSAGE_TYPE_RECRUIT_STOPPED, (u8*)&s_lobby->GetRecruitInfo(), sizeof(PPW_LobbyRecruitInfo));
    
    // ��W�v�����M�^�C�}�[���~
    s_lobby->GetTimerManager().RemoveTimer(PPW_LOBBY_TIMER_ID_RECRUIT);
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �A���P�[�g�̓������o���܂��B
 * 
 * �A���P�[�g�̓������o���܂��B����̃A���P�[�g���Ԃɉ���ł���o�ł��܂����A�O���o�����񓚂͖����ɂȂ�܂��B
 * ��o�������ɍēx���̊֐����ĂԂ�::PPW_LOBBY_RESULT_ERROR_STATE���Ԃ�܂��B
 * 
 * @param answerNo �񓚔ԍ�[0-2]
 * 
 * @retval ::PPW_LOBBY_RESULT_SUCCESS �����B
 * @retval ::PPW_LOBBY_RESULT_ERROR_PARAM �񓚔ԍ���[0-2]�ł͂���܂���B
 * @retval ::PPW_LOBBY_RESULT_ERROR_STATE �܂��A���P�[�g�����擾���Ă��܂���B�������͌��ݒ�o�������ł��B
 * @retval ::PPW_LOBBY_RESULT_ERROR_CONDITION �v���I�ȃG���[���������Ă��܂��B
 */
PPW_LOBBY_RESULT PPW_LobbySubmitQuestionnaire(s32 answerNo)
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_RESULT_ERROR_STATE);
    RETURN_IF_ERROR_STATE(PPW_LOBBY_RESULT_ERROR_CONDITION);
    
    if(answerNo<0 || answerNo>2)
    {
        return PPW_LOBBY_RESULT_ERROR_PARAM;
    }
    
    PPW_LobbyQuestionnaire questionnaire;
    if(!s_lobby->GetLobbyInnerInfoManager().GetQuestionnaire(&questionnaire)
      || s_lobby->GetSubmitQuestionnaireStarted())
    {
        // �܂��A���P�[�g�����擾���Ă��Ȃ��A�܂��͒�o���Ȃ�G���[
        return PPW_LOBBY_RESULT_ERROR_STATE;
    }
    
    // ���N�G�X�g�쐬����
    const PPW_LobbyInnerProfile::UserProfile& userProfile = s_lobby->GetProfileManager().GetMyProfile().GetUserProfile();
    const WFLBY_USER_PROFILE* userProfileDetail = (const WFLBY_USER_PROFILE*)DWCi_GetVectorBuffer(userProfile.data);
    
    // ���N�G�X�g�쐬
    struct Request
    {
        EncCommonRequest commonRequest;
        s32 questionSerialNo;
        s32 questionNo;
        s32 answerNo;
        u32 trainerId;
        s32 sex;
        u16 country;
        u16 region;
    }
    request = { EncCommonRequest(),
                questionnaire.currentQuestionnaireRecord.questionSerialNo,
                questionnaire.currentQuestionnaireRecord.questionNo,
                answerNo,
                userProfileDetail->trainerid,
                userProfileDetail->sex,
                userProfileDetail->nation,
                userProfileDetail->area
              };
    
    DWCi_EncSession::StartSessionAsync(s_lobby->GetEncServer(), ENC_INIT_DATA, ENC_URL_SUBMIT_QUESTIONNAIRE, s_lobby->GetPid(),
                                       (u8*)&request, sizeof(request), ENC_RETRY_COUNT, SubmitQuestionnaireCompletedCallback, NULL);
    
    s_lobby->SetSubmitQuestionnaireStarted(TRUE);
    return PPW_LOBBY_RESULT_SUCCESS;
}

/**
 * @brief �������Ă���v���I�ȃG���[�̏����擾���܂��B
 * 
 * �������Ă���v���I�ȃG���[�̏����擾���܂��B
 * 
 * @retval ::PPW_LOBBY_ERROR_NONE �G���[�����B
 * @retval ::PPW_LOBBY_ERROR_UNKNOWN �s���ȃG���[�B
 * @retval ::PPW_LOBBY_ERROR_ALLOC �������m�ۂɎ��s�B
 * @retval ::PPW_LOBBY_ERROR_SESSION �ʐM�G���[�B
 */
PPW_LOBBY_ERROR PPW_LobbyGetLastError()
{
    RETURN_IF_NOTINITIALIZED(PPW_LOBBY_ERROR_NONE);
    
    return s_lobby->GetError();
}

/**
 * @brief ���������G���[(::PPW_LOBBY_ERROR)����\�����ׂ��G���[�R�[�h�𓾂܂��B
 * 
 * ::PPW_LOBBY_ERROR����\�����ׂ��G���[�R�[�h�ɕϊ����܂��B
 * 
 * @param err ���������G���[�B
 * 
 * @retval �\�����ׂ��G���[�R�[�h�B
 */
s32 PPW_LobbyToErrorCode(PPW_LOBBY_ERROR err)
{
    if(err >= DWCi_LOBBY_ERROR_MAX)
    {
        // �|�P�����Ǝ��R�[�h
        return err + PPW_LOBBY_ERROR_CODE_BASE;
    }
    else
    {
        // ���ʃ��r�[���C�u�����p�R�[�h
        return DWCi_LobbyToErrorCode((DWCi_LOBBY_ERROR)err);
    }
}

/**
 * @brief FriendsMatch���ɁA�w�肵���v���C���[�Ɛڑ����Ă��邩�𒲂ׂ܂��B
 * 
 * FriendsMatch���ɁA�w�肵���v���C���[�Ɛڑ����Ă��邩�𒲂ׂ܂��B(�f�o�b�O�p)\n
 * FriendsMatch���C�u������ғ����A�������̓��r�[���C�u����������������Ă��Ȃ��Ƃ��ɖ{�֐����Ăяo�����ꍇ��FALSE��Ԃ��܂��B\n
 * FriendsMatch���C�u�����ғ����Ɏ����̃��[�UID���w�肵���ꍇ��TRUE��Ԃ��܂��B\n
 * �~�j�Q�[�����W���Ă���l�Ɩ{���ɐڑ����Ă��邩���m�F���邽�߂Ɏg�p���Ă��������B\n
 * ���̊֐����g�p����ɂ̓��r�[���C�u������::PPW_LobbyInitializeAsync�֐��ŏ���������Ă���K�v������܂��B
 * ::PPW_LobbyInitializePidAsync�֐��ŏ���������Ă����ꍇ��Assert���܂��B
 * Nitro�ȊO�ł͎g�p�ł��܂���B
 * 
 * @param[in] dwcFriendMatchCtrl �t�����h�}�b�`����I�u�W�F�N�g�B
 * @param[in] userId �ڑ����ł��邩�𔻒肷�郆�[�UID�B
 * 
 * @retval TRUE �ڑ����Ă���B
 * @retval FALSE �ڑ����Ă��Ȃ��B
 */
BOOL PPW_IsConnectedUserId(const DWCFriendsMatchControl* dwcFriendMatchCtrl, s32 userId)
{
    RETURN_IF_NOTINITIALIZED(FALSE);    // ���r�[���C�u������ғ�
    
#ifndef _NITRO  // NITRO�ȊO�͓K����TRUE��Ԃ�
    DWC_ASSERTMSG(FALSE, "PPW_IsConnectedUserId is only for NITRO.");
    (void)dwcFriendMatchCtrl;
    (void)userId;
    return TRUE;
#else
    DWC_ASSERTMSG(s_lobby->GetIsValidInitialize() && s_lobby->GetPid() == dwcFriendMatchCtrl->profileID,
                  "PPW_IsConnectedUserId: You must initialize by PPW_LobbyInitializeAsync() to join mini game.");
    
    int currentHostNum = DWC_GetNumConnectionHost();
    if(currentHostNum == 0 || !dwcFriendMatchCtrl)
    {
        // FriendsMatch��ғ�
        return FALSE;
    }
    
    if(userId == s_lobby->GetPid())
    {
        return TRUE;
    }
    
    // �ڑ��ς�pid���X�g�Ɋ܂܂�Ă��邩���ׂ�
    const int* sbPidList = dwcFriendMatchCtrl->matchcnt.sbPidList;
    const int* ret = std::find(sbPidList, sbPidList + currentHostNum, userId);
    
    return ret != sbPidList + currentHostNum;
#endif
}

/*
 * @brief �f�o�b�O�p�ɔC�ӂ�EncInitData���w�肵�܂��B
 * 
 * �f�o�b�O�p�ɔC�ӂ�EncInitData���w�肵�܂��B�������O�ɌĂ�ł��������B
 * 
 * @param[in] initData ENC�p��InitData�B�w�肵��������͐ÓI�ɕێ����Ă��������B
 */
void PPW_LobbySetEncInitData(const char* initData)
{
    ENC_INIT_DATA = initData;
}

//
// �������J�֐���`
//

// ���b�Z�[�W�𑗐M
PPW_LOBBY_RESULT PPW_LobbySendMessage(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, PPW_LobbyMessage::FORMAT format,
                                             PPW_LobbyMessage::TARGET target, s32 type, const u8* data, u32 dataSize)
{
    if(userId == PPW_LOBBY_INVALID_USER_ID && channelKind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        return PPW_LOBBY_RESULT_ERROR_PARAM;
    }
    
    DWCi_String sendString;
    if(!PPW_LobbyMessage::Build(format, target, type, (const char*)data, dataSize, sendString))
    {
        return PPW_LOBBY_RESULT_ERROR_PARAM;
    }

    if(channelKind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        return ToPpwResult(DWCi_LobbySendUserMessage(userId, sendString.c_str()));
    }
    else
    {
        DWCi_String channelName = s_lobby->GetChannelManager().GetChannelName(channelKind);
        if(channelName == "")
        {
            return PPW_LOBBY_RESULT_ERROR_CHANNEL;
        }
        return ToPpwResult(DWCi_LobbySendChannelMessage(channelName.c_str(), sendString.c_str()));
    }
}

// �`�����l���f�[�^�𑗐M
PPW_LOBBY_RESULT PPW_LobbySetChannelDataImpl(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char* key, const u8* data, u32 dataSize)
{
    DWCi_String channelName = s_lobby->GetChannelManager().GetChannelName(channelKind);
    if(channelName == "")
    {
        return PPW_LOBBY_RESULT_ERROR_CHANNEL;
    }

    if(dataSize > PPW_LOBBY_MAX_BINARY_SIZE)
    {
        return PPW_LOBBY_RESULT_ERROR_PARAM;
    }

    std::vector<char, DWCi_Allocator<char> > tempBuf(PPW_LobbyToBase64Size(dataSize) + 1);
    int writtenSize = DWC_Base64Encode((char*)data, dataSize, DWCi_GetVectorBuffer(tempBuf), tempBuf.size());
    DWC_ASSERTMSG(writtenSize != -1, "PPW_LobbySetChannelDataImpl: Internal error.");
    tempBuf[(std::size_t)writtenSize] = '\0';

    return ToPpwResult(DWCi_LobbySetChannelData(channelName.c_str(), userId, key, DWCi_GetVectorBuffer(tempBuf)));
}

//
// ���[�J���֐���`
//

// �����ݒ�_�E�����[�h�J�n
static void StartDownload()
{
    // ���N�G�X�g�쐬����
    const PPW_LobbyInnerProfile::UserProfile& userProfile = s_lobby->GetProfileManager().GetMyProfile().GetUserProfile();
    const WFLBY_USER_PROFILE* userProfileDetail = (const WFLBY_USER_PROFILE*)DWCi_GetVectorBuffer(userProfile.data);
    
    struct Request
    {
        EncCommonRequest commonRequest;
        WFLBY_USER_PROFILE profile;
    }
    request;
    request.profile = *userProfileDetail;
    
    // CheckProfileCompletedCallback�֐����ŃX�P�W���[����M�Ɉڍs
    DWCi_EncSession::StartSessionAsync(s_lobby->GetEncServer(), ENC_INIT_DATA, ENC_URL_CHECK_PROFILE, s_lobby->GetPid(),
                                  (u8*)&request, sizeof(request), ENC_RETRY_COUNT, CheckProfileCompletedCallback, NULL);
}

// kind!uniqueName �̌`���̃`�����l���������
static DWCi_String PPW_LobbyCreateBaseChannelName(PPW_LOBBY_CHANNEL_KIND nameKind, const DWCi_String& uniqueName)
{
    DWC_ASSERTMSG(nameKind < PPW_LOBBY_CHANNEL_KIND_INVALID, "PPW_LobbyCreateBaseChannelName: Invalid channel kind.");
    return DWCi_String(PPW_LOBBY_CHANNEL_NAME[nameKind]) + DWCi_LOBBY_CHANNEL_NAME_SEPARATOR + uniqueName;
}

static DWCi_String PPW_LobbyGenerateUniqueChannelName()
{
    return DWCi_SPrintf<DWCi_String>("C%d_%u", s_lobby->GetPid(), (unsigned long)current_time() % 1000);
}

// ���C���`�����l���ɓ���B
static PPW_LOBBY_RESULT PPW_LobbyEnterMainChannel()
{
    DWCi_LobbyChannelCallbacks cc;
    cc.lobbyPlayerConnectedCallback = LobbyPlayerConnectedCallback;
    cc.lobbyPlayerDisconnectedCallback = LobbyPlayerDisconnectedCallback;
    cc.lobbyChannelMassageReceivedCallback = LobbyChannelMassageReceivedCallback;
    cc.lobbyBroadcastDataCallback = LobbyBroadcastDataCallback;
    cc.lobbyTopicChangedCallback = LobbyTopicChangedCallback;
    cc.param = (void*)PPW_LOBBY_CHANNEL_KIND_MAIN;
    
    DWCi_String channelName = PPW_LobbyCreateBaseChannelName(PPW_LOBBY_CHANNEL_KIND_MAIN, s_lobby->GetUniqueChannelName());
    PPW_LOBBY_RESULT result = ToPpwResult(DWCi_LobbyJoinChannelLimitAsync(channelName.c_str(), NULL, PPW_LOBBY_MAX_PLAYER_NUM_MAIN,
                                                                          &cc, LobbyJoinMainChannelCallback, (void*)PPW_LOBBY_CHANNEL_KIND_MAIN));
    if(result != PPW_LOBBY_RESULT_SUCCESS)
    {
        return result;
    }
    s_lobby->SetMainChannel(PPW_Channel(PPW_LOBBY_CHANNEL_KIND_MAIN, channelName));
    return result;
}

// �`�����l���f�[�^����
static void PPW_ChannelDataHandler(BOOL success, BOOL broadcast, const char* channelName, s32 userId,
                                        const char* key, const char* value, void* param)
{
    // �`�����l�����m�F
    PPW_LOBBY_CHANNEL_KIND kind = s_lobby->GetChannelManager().GetChannelKind(channelName);
    if(kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "PPW_ChannelDataHandler: No such channel.");
        return;
    }
    
    // Base64�f�R�[�h
    std::vector<u8, DWCi_Allocator<u8> > tempBuf;
    if(!DWCi_Base64Decode(value, tempBuf))
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "PPW_ChannelDataHandler: ignored invalid data.\n");
        return;
    }
    
    // �T�C�Y0�͖���
    if(tempBuf.empty())
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "PPW_ChannelDataHandler: ignored empty data.\n");
        return;
    }
    
    // ���C�u�������T�|�[�g�̃��[�U��`�v���t�B�[��������
    if(PPW_SystemChannelDataHandler(success, broadcast, kind, userId, key, DWCi_GetVectorBuffer(tempBuf), tempBuf.size(), param))
    {
        return;
    }
    
    // �`�����l�����̂̃f�[�^�ȊO�̏ꍇ�A�A�N�e�B�x�[�g���Ă��Ȃ��v���C���[����̃��b�Z�[�W�͖���
    if(userId != PPW_LOBBY_INVALID_USER_ID && !s_lobby->GetProfileManager().IsActivatedProfile(userId))
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "PPW_ChannelDataHandler: Ignored not activated user's data.\n");
        return;
    }
    
    // ���[�U���ŏ���
    s_lobby->GetCallbacks().lobbyGetChannelDataCallback(success, broadcast, kind, userId, key, DWCi_GetVectorBuffer(tempBuf), tempBuf.size(), param);
}

// �V�X�e����`�̃`�����l���f�[�^����
// ���C�u�������ŏ���������TRUE��Ԃ�
static BOOL PPW_SystemChannelDataHandler(BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channelKind, s32 userId,
                                        const char* key, const u8* data, u32 dataSize, void* param)
{
    // �����ŏ������Ă���̂͂���������C���`�����l��
    if(channelKind != PPW_LOBBY_CHANNEL_KIND_MAIN)
    {
        return FALSE;
    }
    
    if(strcmp(key, PPW_LOBBY_CHANNEL_KEY_USER) == 0)
    {
        //
        // ���[�U��`�v���t�B�[�����X�V���ꂽ
        //
        if(userId == s_lobby->GetPid())
        {
            // �����̃f�[�^�͖���
            return TRUE;
        }
        
        PPW_LobbyInnerProfile* profile = s_lobby->GetProfileManager().FindProfile(userId);
        if(!profile)
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "PPW_SystemChannelDataHandler: profile is NULL. userId: %d\n", userId);
            return TRUE;
        }
    
        profile->SetUserProfile(data, (u32)dataSize);
        
        // �ڑ��������Ă���Ƃ��̂݃R�[���o�b�N���Ă�
        if(profile->IsActivated())
        {
            s_lobby->GetCallbacks().lobbyUserProfileUpdatedCallback(userId, data, dataSize);
        }
        return TRUE;
    }
    else if(strcmp(key, PPW_LOBBY_CHANNEL_KEY_SYSTEM) == 0)
    {
        //
        // �V�X�e����`�v���t�B�[�����X�V���ꂽ
        //
        if(userId == s_lobby->GetPid())
        {
            // �����̃f�[�^�͖���
            DWC_Printf(DWC_REPORTFLAG_INFO, "PPW_SystemChannelDataHandler: Ignored my data. userId: %d\n", userId);
            return TRUE;
        }
        
        if(dataSize != sizeof(PPW_LobbyInnerProfile::SystemProfile))   // TODO: �f�[�^�̏o��������V���A���C�Y�֐��ƃf�V���A���C�Y�֐��ɂ���
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "PPW_SystemChannelDataHandler: Invalid system data size. size: %d\n", sizeof(PPW_LobbyInnerProfile::SystemProfile));
            // �s���ȃf�[�^�͖�������
            return TRUE;
        }
        PPW_LobbyInnerProfile* profile = s_lobby->GetProfileManager().FindProfile(userId);
        if(!profile)
        {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "PPW_SystemChannelDataHandler: profile is NULL. userId: %d\n", userId);
            return TRUE;
        }
        profile->SetSystemProfile(*(PPW_LobbyInnerProfile::SystemProfile*)data);
        
        // �ڑ��������Ă���Ƃ��ƕύX���������Ƃ��̂݃R�[���o�b�N���Ă�
        if(profile->IsActivated() && profile->IsSystemProfileUpdated())
        {
            const PPW_LobbyInnerProfile::SystemProfile& systemProfile = profile->GetSystemProfileConfirm();  // ��������ۂ��ߕۑ������f�[�^�������Ă���
            PPW_LobbySystemProfile publicSystemProfile = systemProfile.ToPublicSystemProfile();
            s_lobby->GetCallbacks().lobbySystemProfileUpdatedCallback(userId, &publicSystemProfile);
        }
        return TRUE;
    }
    else if(strcmp(key, PPW_LOBBY_CHANNEL_KEY_CHANNEL_TIME) == 0)
    {
        //
        // �`�����l���V�X�e���f�[�^���X�V���ꂽ(LobbyCheckMainChannelSystemProfileCallback()�ł��Z�b�g���Ă�̂ł�����͂Ȃ��Ă��悢���A�����̑��x���オ��)
        //
        if(dataSize != sizeof(PPW_LobbyInnerInfoManager::TimeData))
        {
            // �s���ȃf�[�^�͖�������
            return TRUE;
        }
        PPW_LobbyInnerInfoManager::TimeData* systemProfile = (PPW_LobbyInnerInfoManager::TimeData*)data;
        DWC_Printf(DWC_REPORTFLAG_INFO, "PPW_SystemChannelDataHandler: openedTime is %lld\n", systemProfile->openedTime);
        s_lobby->GetLobbyInnerInfoManager().SetTimeData(*systemProfile);
        return TRUE;
    }
    else if(strcmp(key, PPW_LOBBY_CHANNEL_KEY_CHANNEL_LOBBY) == 0)
    {
        //
        // ���r�[�f�[�^���X�V���ꂽ(LobbyCheckLobbyInfoCallback()�ł��Z�b�g���Ă�̂ł�����͂Ȃ��Ă��悢���A�����̑��x���オ��)
        //
        if(!s_lobby->GetLobbyInnerInfoManager().SetLobbyInfo(data, dataSize))
        {
            // �s���ȃf�[�^�͖�������
            return TRUE;
        }
        s_lobby->GetLobbyInnerInfoManager().Shared();
        return TRUE;
    }
    
    (void)success;
    (void)broadcast;
    (void)param;
    
    return FALSE;
}

// �V�X�e����`�̃��b�Z�[�W�����B�K��TRUE��Ԃ�
static BOOL PPW_SystemMessageHandler(const PPW_LobbyMessage::Result& result, s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, u8* data, u32 dataSize)
{
    // �����ŏ������Ă���̂͂���������C���`�����l��
    if(channelKind != PPW_LOBBY_CHANNEL_KIND_MAIN)
    {
        return TRUE;
    }
    
    switch(result.type)
    {
    case PPW_LOBBY_MESSAGE_TYPE_RECRUIT:
        // ��W�̓��[�v�o�b�N�����Ȃ�
        if(s_lobby->GetPid() == userId)
        {
            break;
        }
        
        // �A�N�e�B�x�[�g���Ă��Ȃ����[�U����̃��b�Z�[�W�͖���
        if(!s_lobby->GetProfileManager().IsActivatedProfile(userId))
        {
            DWC_Printf( DWC_REPORTFLAG_INFO, "PPW_SystemMessageHandler: Ignored not activated user's message.\n" );
            break;
        }
        
        if(result.format == PPW_LobbyMessage::FORMAT_BASE64 && dataSize == sizeof(PPW_LobbyRecruitInfo))
        {
            s_lobby->GetCallbacks().lobbyRecruitCallback(userId, (PPW_LobbyRecruitInfo*)data);
        }
        break;
    case PPW_LOBBY_MESSAGE_TYPE_RECRUIT_STOPPED:
        // ��W�I�������[�v�o�b�N�����Ȃ�
        if(s_lobby->GetPid() == userId)
        {
            break;
        }
        
        // �A�N�e�B�x�[�g���Ă��Ȃ����[�U����̃��b�Z�[�W�͖���
        if(!s_lobby->GetProfileManager().IsActivatedProfile(userId))
        {
            DWC_Printf( DWC_REPORTFLAG_INFO, "PPW_SystemMessageHandler: Ignored not activated user's message.\n" );
            break;
        }
        
        if(result.format == PPW_LobbyMessage::FORMAT_BASE64 && dataSize == sizeof(PPW_LobbyRecruitInfo))
        {
            s_lobby->GetCallbacks().lobbyRecruitStoppedCallback(userId, (PPW_LobbyRecruitInfo*)data);
        }
        break;
    case PPW_LOBBY_MESSAGE_TYPE_LOCK:
        if(result.format == PPW_LobbyMessage::FORMAT_BASE64 && dataSize == sizeof(PPW_LobbyDummyData))
        {
            s_lobby->GetLobbyInnerInfoManager().Lock();
        }
        break;
    default:
        // �s���ȃ^�C�v�͖���
        DWC_Printf( DWC_REPORTFLAG_INFO, "PPW_SystemMessageHandler: Ignored unknown type.\n" );
        break;
    }
    return TRUE;
}

// ���C���`�����l���̏������ł��Ă��邩���m�F
BOOL PPW_IsMainChannelReady()
{
    // ���C���`�����l���ɓ����ς݂�
    if(!DWCi_LobbyInChannel(s_lobby->GetMainChannel().GetName().c_str()))
    {
        return FALSE;
    }
    
    // �Q���҂�񋓂�����
    if(!s_lobby->GetMainChannel().GetUserListIsEnumerated())
    {
        return FALSE;
    }
    
    // �`�����l�����[�h���Z�b�g����Ă��邩
    if(!s_lobby->GetMainChannel().GetModeIsSet())
    {
        return FALSE;
    }
    
    // �`�����l���Ǘ��f�[�^���Z�b�g����Ă��邩
    if(!s_lobby->GetLobbyInnerInfoManager().GetTimeDataIsSet())
    {
        return FALSE;
    }
    
    // �X�P�W���[�����Z�b�g����Ă��邩
    if(!s_lobby->GetLobbyInnerInfoManager().IsShared())
    {
        return FALSE;
    }
    
    // �Q���ґS�Ẵ��[�U��`�v���t�B�[�����󂯎������
    if(!s_lobby->GetProfileManager().CheckAllProfilesRecved())
    {
        return FALSE;
    }
    return TRUE;
}

//
// �^�C�}�[�R�[���o�b�N
//

// �����̃��[�U��`�v���t�B�[���𑗐M
static BOOL PPW_LobbyBroadcastMyUserProfile(void* param)
{
    PPW_LobbyInnerProfile& myProfile = s_lobby->GetProfileManager().GetMyProfile();
    
    if(!myProfile.IsUserProfileUpdated())
    {
        // �X�V����Ă��Ȃ���Α���Ȃ�
        return TRUE;
    }
    
    const PPW_LobbyInnerProfile::UserProfile& userProfile = myProfile.GetUserProfileConfirm();
    
    PPW_LobbySetChannelDataImpl(s_lobby->GetPid(), PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LOBBY_CHANNEL_KEY_USER, (u8*)DWCi_GetVectorBuffer(userProfile.data),
                                userProfile.data.size());
    
    (void)param;
    return TRUE;
}

// �����̃V�X�e����`�v���t�B�[���𑗐M
static BOOL PPW_LobbyBroadcastMySystemProfile(void* param)
{
    PPW_LobbyInnerProfile& myProfile = s_lobby->GetProfileManager().GetMyProfile();
    
    if(!myProfile.IsSystemProfileUpdated())
    {
        // �X�V����Ă��Ȃ���Α���Ȃ�
        return TRUE;
    }
    
    const PPW_LobbyInnerProfile::SystemProfile& systemProfile = myProfile.GetSystemProfileConfirm();
    
    PPW_LobbySetChannelDataImpl(s_lobby->GetPid(), PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LOBBY_CHANNEL_KEY_SYSTEM, (u8*)&systemProfile, sizeof(systemProfile));
    
    (void)param;
    return TRUE;
}

// ��W���𑗐M
static BOOL PPW_LobbySendRecruitInfo(void* param)
{
    // �O�̂��ߕ�W���Ă��邩�m�F
    if(!s_lobby->IsRecruiting())
    {
        DWC_ASSERTMSG(FALSE, "PPW_LobbySendRecruitInfo: Not recruiting?");
        return FALSE;   // FALSE��Ԃ��Ƃ��̃^�C�}�[���폜�ł���
    }
    
    PPW_LobbySendMessage(s_lobby->GetPid(), PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LobbyMessage::FORMAT_BASE64, PPW_LobbyMessage::TARGET_SYSTEM,
                         PPW_LOBBY_MESSAGE_TYPE_RECRUIT, (u8*)&s_lobby->GetRecruitInfo(), sizeof(PPW_LobbyRecruitInfo));
    
    (void)param;
    return TRUE;
}

// ���C���`�����l�����[�h���`�F�b�N
static BOOL PPW_LobbyCheckMainChannelMode(void* param)
{
    DWCi_LobbyGetChannelModeAsync(s_lobby->GetMainChannel().GetName().c_str(), LobbyCheckMainChannelModeCallback, NULL);
    
    (void)param;
    return TRUE;
}

// ���C���`�����l���V�X�e���f�[�^�`�F�b�N
static BOOL PPW_LobbyCheckMainChannelSystemProfile(void* param)
{
    // ���łɃZ�b�g����Ă����璆�f����
    if(s_lobby->GetLobbyInnerInfoManager().GetTimeDataIsSet())
    {
        // FALSE��Ԃ��ƃ^�C�}�[�폜
        return FALSE;
    }
    DWCi_LobbyGetChannelDataAsync(s_lobby->GetMainChannel().GetName().c_str(), PPW_LOBBY_INVALID_USER_ID,
                                  PPW_LOBBY_CHANNEL_KEY_CHANNEL_TIME, LobbyCheckMainChannelSystemProfileCallback, NULL);
    
    (void)param;
    return TRUE;
}


// ���r�[�f�[�^�`�F�b�N
static BOOL PPW_LobbyCheckLobbyInfo(void* param)
{
    // ���łɃZ�b�g����Ă����璆�f����
    if(s_lobby->GetLobbyInnerInfoManager().IsShared())
    {
        // FALSE��Ԃ��ƃ^�C�}�[�폜
        return FALSE;
    }
    DWCi_LobbyGetChannelDataAsync(s_lobby->GetMainChannel().GetName().c_str(), PPW_LOBBY_INVALID_USER_ID,
                             PPW_LOBBY_CHANNEL_KEY_CHANNEL_LOBBY, LobbyCheckLobbyInfoCallback, NULL);
    
    (void)param;
    return TRUE;
}

static PPW_LOBBY_STATS_RESULT EncResultCodeToStatsResult(u32 code)
{
    switch(code)
    {
    case ENC_SERVER_RESPONSE_CODE_OK:
        return PPW_LOBBY_STATS_RESULT_SUCCESS;
    case ENC_SERVER_RESPONSE_CODE_INVALID_PARAM:
        return PPW_LOBBY_STATS_RESULT_INVALID_PARAM;
    case ENC_SERVER_RESPONSE_CODE_INTERNAL_ERROR:
    case ENC_SERVER_RESPONSE_CODE_DATABASE_ERROR:
    case ENC_SERVER_RESPONSE_CODE_MAINTENANCE:
    case ENC_SERVER_RESPONSE_CODE_UNKNOWN_ERROR:
        return PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
    default:
        DWC_ASSERTMSG(FALSE, "EncResultCodeToStatsResult: unknown code.");
        return PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
    }
}

// Enc���X�|���X�`�F�b�N
static PPW_LOBBY_STATS_RESULT CheckResponseHeader(BOOL success,
                                                  const u8* data,
                                                  u32 size,
                                                  const u8** responseData,  // out
                                                  u32* responseSize)        // out
{
    PPW_LOBBY_STATS_RESULT result = PPW_LOBBY_STATS_RESULT_SUCCESS;
    *responseData = NULL;
    *responseSize = 0;
    
    if(!success)
    {
        return PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
    }

    // ���ʃR�[�h���܂܂�Ă��邩�`�F�b�N
    if(size < sizeof(u32))
    {
        return PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
    }

    // �擪�̌��ʃR�[�h���m�F
    u32 code = (u32)*data;
    result = EncResultCodeToStatsResult(code);
    if(result == PPW_LOBBY_STATS_RESULT_SERVER_ERROR)
    {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "CheckResponseHeader: server returned error: %u\n", code);
        return result;
    }
    else if(result == PPW_LOBBY_STATS_RESULT_INVALID_PARAM)
    {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "CheckResponseHeader: You specified invalid parameter.\n");
        return result;
    }

    *responseData = data + sizeof(u32);
    *responseSize = size - sizeof(u32);
    
    return result;
}

// ENC�����R�[���o�b�N
// �s���`�F�b�N�����R�[���o�b�N
static void CheckProfileCompletedCallback(BOOL success, const u8* data, u32 size, void* param)
{
    RETURN_IF_NOTINITIALIZED((void)0);
    if(s_lobby->GetState() == PPW_LOBBY_STATE_CLOSING)
    {
        return;
    }
    
    const u8* responseData = NULL;
    u32 responseSize = 0;
    PPW_LOBBY_STATS_RESULT result = CheckResponseHeader(success, data, size, &responseData, &responseSize);

    if(result == PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        // �R�[���o�b�N���ĂԑO�Ƀf�[�^���`�F�b�N
        if(responseSize != sizeof(WFLBY_USER_PROFILE))
        {
            // �_�E�����[�h�����f�[�^����������
            DWC_Printf(DWC_REPORTFLAG_ERROR, "CheckProfileCompletedCallback: returned profile is invalid.\n");
            result = PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
        }
    }
    
    if(result == PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        // �u�����ꂽ���[�U��`�v���t�B�[���Z�b�g
        PPW_LobbyUpdateMyProfile(responseData, responseSize, FALSE);
    }
    
    // �R�[���o�b�N�Ăяo��
    s_lobby->GetCallbacks().lobbyCheckProfileCallback(result, responseData, responseSize);
    
    if(result != PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        // ���s��(�s���`�F�b�N�Ɉ������������Ƃ���T�[�o�G���[�̂Ƃ�)�̓G���[��Ԃɂ��ďI��
        s_lobby->SetError(PPW_LOBBY_ERROR_STATS_CHECKPROFILE);
        return;
    }
    
    // ���N�G�X�g�쐬
    EncCommonRequest request;
    
    // �X�P�W���[���_�E�����[�h�J�n
    DWCi_EncSession::StartSessionAsync(s_lobby->GetEncServer(), ENC_INIT_DATA, ENC_URL_GET_SCHEDULE, s_lobby->GetPid(),
                                       (u8*)&request, sizeof(request), ENC_RETRY_COUNT, GetScheduleCompletedCallback, NULL, TRUE);
    (void)param;
}

// �X�P�W���[���_�E�����[�h�����R�[���o�b�N
static void GetScheduleCompletedCallback(BOOL success, const u8* data, u32 size, void* param)
{
    RETURN_IF_NOTINITIALIZED((void)0);
    if(s_lobby->GetState() == PPW_LOBBY_STATE_CLOSING)
    {
        return;
    }
    
    const u8* responseData = NULL;
    u32 responseSize = 0;
    PPW_LOBBY_STATS_RESULT result = CheckResponseHeader(success, data, size, &responseData, &responseSize);

    if(result == PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        // �R�[���o�b�N���ĂԑO�Ƀf�[�^���`�F�b�N
        if(!s_lobby->GetLobbyInnerInfoManager().CheckLobbyInfo(responseData, responseSize))
        {
            // �_�E�����[�h�����f�[�^����������
            DWC_Printf(DWC_REPORTFLAG_ERROR, "GetScheduleCompletedCallback: room setting data is invalid.\n");
            result = PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
        }
    }
    
    // �R�[���o�b�N�Ăяo��
    BOOL useDownloadedData = s_lobby->GetCallbacks().lobbyNotifyScheduleCallback(result, (PPW_LobbySchedule*)responseData);
    
    if(result != PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        s_lobby->SetError(PPW_LOBBY_ERROR_STATS_SCHEDULE);
        return;
    }
    
    if(useDownloadedData)
    {
        // �X�P�W���[���Z�b�g�B��xCheckLobbyInfo���Ă���̂ŕK����������B
        s_lobby->GetLobbyInnerInfoManager().SetLobbyInfo(responseData, responseSize);
    }
    
    // ���N�G�X�g�쐬����
    const PPW_LobbyInnerProfile::UserProfile& userProfile = s_lobby->GetProfileManager().GetMyProfile().GetUserProfile();
    const WFLBY_USER_PROFILE* userProfileDetail = (const WFLBY_USER_PROFILE*)DWCi_GetVectorBuffer(userProfile.data);
    
    // ���N�G�X�g�쐬
    struct Request
    {
        EncCommonRequest commonRequest;
        u32 trainerId;
    }
    request =
    {
        EncCommonRequest(),
        userProfileDetail->trainerid
    };
    
    // VIP���_�E�����[�h�J�n
    DWCi_EncSession::StartSessionAsync(s_lobby->GetEncServer(), ENC_INIT_DATA, ENC_URL_GET_VIP, s_lobby->GetPid(),
                                       (u8*)&request, sizeof(request), ENC_RETRY_COUNT, GetVipCompletedCallback, NULL, TRUE);
    (void)param;
}

// VIP���_�E�����[�h�����R�[���o�b�N
static void GetVipCompletedCallback(BOOL success, const u8* data, u32 size, void* param)
{
    RETURN_IF_NOTINITIALIZED((void)0);
    if(s_lobby->GetState() == PPW_LOBBY_STATE_CLOSING)
    {
        return;
    }
    
    const u8* responseData = NULL;
    u32 responseSize = 0;
    PPW_LOBBY_STATS_RESULT result = CheckResponseHeader(success, data, size, &responseData, &responseSize);

    if(!s_lobby->GetLobbyInnerInfoManager().CheckVipRecord(responseData, responseSize))
    {
        // �_�E�����[�h�����f�[�^����������
        DWC_Printf(DWC_REPORTFLAG_ERROR, "GetVipCompletedCallback: vip data is invalid.\n");
        result = PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
    }
    
    if(result != PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        s_lobby->SetError(PPW_LOBBY_ERROR_STATS_VIP);
        return;
    }
    
    // VIP���Z�b�g
    s_lobby->GetLobbyInnerInfoManager().SetVipRecord(responseData, responseSize);
    
    // �R�[���o�b�N
    s_lobby->GetCallbacks().lobbyGetVipCallback(result, (PPW_LobbyVipRecord*)responseData, responseSize / sizeof(PPW_LobbyVipRecord));
    
    // ���N�G�X�g�쐬
    EncCommonRequest request;
    
    // �A���P�[�g���_�E�����[�h�J�n
    DWCi_EncSession::StartSessionAsync(s_lobby->GetEncServer(), ENC_INIT_DATA, ENC_URL_GET_QUESTIONNAIRE, s_lobby->GetPid(),
                                       (u8*)&request, sizeof(request), ENC_RETRY_COUNT, GetQuestionnaireCompletedCallback, NULL, TRUE);
    (void)param;
}

// �A���P�[�g���_�E�����[�h�����R�[���o�b�N
static void GetQuestionnaireCompletedCallback(BOOL success, const u8* data, u32 size, void* param)
{
    RETURN_IF_NOTINITIALIZED((void)0);
    if(s_lobby->GetState() == PPW_LOBBY_STATE_CLOSING)
    {
        return;
    }
    
    const u8* responseData = NULL;
    u32 responseSize = 0;
    PPW_LOBBY_STATS_RESULT result = CheckResponseHeader(success, data, size, &responseData, &responseSize);

    if(responseSize != sizeof(PPW_LobbyQuestionnaire))
    {
        // �_�E�����[�h�����f�[�^����������
        DWC_Printf(DWC_REPORTFLAG_ERROR, "GetQuestionnaireCompletedCallback: questionnaire data is invalid.\n");
        result = PPW_LOBBY_STATS_RESULT_SERVER_ERROR;
    }
    
    if(result != PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        s_lobby->SetError(PPW_LOBBY_ERROR_STATS_QUESTIONNAIRE);
        return;
    }
    
    // �A���P�[�g���ۑ�
    s_lobby->GetLobbyInnerInfoManager().SetQuestionnaire((PPW_LobbyQuestionnaire*)responseData);
    
    // �R�[���o�b�N
    s_lobby->GetCallbacks().lobbyGetQuestionnaireCallback(result, (PPW_LobbyQuestionnaire*)responseData);
    
    s_lobby->GetLobbyInnerInfoManager().Downloaded();
    (void)param;
}

// �A���P�[�g��o�����R�[���o�b�N
static void SubmitQuestionnaireCompletedCallback(BOOL success, const u8* data, u32 size, void* param)
{
    RETURN_IF_NOTINITIALIZED((void)0);
    if(s_lobby->GetState() == PPW_LOBBY_STATE_CLOSING)
    {
        return;
    }
    
    s_lobby->SetSubmitQuestionnaireStarted(FALSE);
    s_lobby->GetCallbacks().lobbySubmitQuestionnaireCallback(success ? PPW_LOBBY_STATS_RESULT_SUCCESS : PPW_LOBBY_STATS_RESULT_SERVER_ERROR);
    (void)data;
    (void)size;
    (void)param;
}


//
// ���ʃ��C�u��������̃R�[���o�b�N��`
//

// ���C���`�����l�������R�[���o�b�N
static void LobbyJoinMainChannelCallback(BOOL success, DWCi_LOBBY_ENTER_RESULT result, const char* channelName, void* param)
{
    // VS2005���ƈ�x(u32)�ɃL���X�g����K�v������
    PPW_LOBBY_CHANNEL_KIND kind = (PPW_LOBBY_CHANNEL_KIND)(u32)param;
    if(kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_ASSERTMSG(FALSE, "LobbyJoinChannelCallback: Internal error. No such channel.");
        return;
    }
    
    if(success)
    {
        s_lobby->ClearMainChannelRetryCount();
        
        // �����̃V�X�e���f�[�^(���������Ȃ�)������������
        s_lobby->GetProfileManager().GetMyProfile().SetSystemProfile(PPW_LobbyInnerProfile::SystemProfile());
        
        // �����o�[�̊m�F������
        DWCi_LobbyEnumUsersAsync(channelName, LobbyEnumUsersCallback, (void*)kind);
        
        // �����f�[�^���M�p�^�C�}�[�Z�b�g
        s_lobby->GetTimerManager().AddTimer(PPW_LOBBY_TIMER_ID_SYSTEM_CHANNEL_DATA, PPW_LOBBY_TIMER_INTERVAL_SYSTEM_CHANNEL_DATA,
                                            PPW_LobbyBroadcastMySystemProfile, NULL, TRUE);
        s_lobby->GetTimerManager().AddTimer(PPW_LOBBY_TIMER_ID_USER_CHANNEL_DATA, PPW_LOBBY_TIMER_INTERVAL_USER_CHANNEL_DATA,
                                            PPW_LobbyBroadcastMyUserProfile, NULL, TRUE);
        
        // �`�����l���Ǘ��f�[�^�m�F�p�^�C�}�[�Z�b�g
        s_lobby->GetTimerManager().AddTimer(PPW_LOBBY_TIMER_ID_CHECK_MAINCHANNEL_SYSTEMDATA, PPW_LOBBY_TIMER_INTERVAL_CHECK_MAINCHANNEL_SYSTEMDATA,
                                            PPW_LobbyCheckMainChannelSystemProfile, NULL, TRUE);
        
        // �`�����l�����[�h�m�F�p�^�C�}�[�Z�b�g
        s_lobby->GetTimerManager().AddTimer(PPW_LOBBY_TIMER_ID_CHECK_MAINCHANNEL_MODE, PPW_LOBBY_TIMER_INTERVAL_CHECK_MAINCHANNEL_MODE,
                                            PPW_LobbyCheckMainChannelMode, NULL, TRUE);
        
        // ���r�[�f�[�^�m�F�p�^�C�}�[�Z�b�g
        s_lobby->GetTimerManager().AddTimer(PPW_LOBBY_TIMER_ID_CHECK_LOBBY_INFO, PPW_LOBBY_TIMER_INTERVAL_CHECK_LOBBY_INFO,
                                          PPW_LobbyCheckLobbyInfo, NULL, TRUE);
        
        // �����ɑ΂��郆�[�U�ڑ������R�[���o�b�N���Ă�
        const PPW_LobbyInnerProfile& myProfile = s_lobby->GetProfileManager().GetMyProfile();
        const PPW_LobbyInnerProfile::SystemProfile& systemProfile = myProfile.GetSystemProfile();
        const PPW_LobbyInnerProfile::UserProfile& userProfile = myProfile.GetUserProfile();
        PPW_LobbySystemProfile publicSystemProfile = systemProfile.ToPublicSystemProfile();
        s_lobby->GetCallbacks().lobbyPlayerConnectedCallback(myProfile.GetPid(), &publicSystemProfile, DWCi_GetVectorBuffer(userProfile.data),
                                                             userProfile.data.size());
    }
    else
    {
        s_lobby->GetMainChannel().Clear();
        
        if(s_lobby->IncrementMainChannelRetryCount())
        {
            // �l���������ς���������������Ȃ��̂Ń��g���C����B
            // �`�����l���T�[�`�����蒼��
            s_lobby->SetState(PPW_LOBBY_STATE_DOWNLOAD_WAIT);
        }
        else
        {
            // ���C���`�����l�������G���[�͒v���I�G���[
            s_lobby->SetError(PPW_LOBBY_ERROR_SESSION);
        }
    }
    
    (void)result;
}

// �T�u�`�����l�������R�[���o�b�N
static void LobbyJoinSubChannelCallback(BOOL success, DWCi_LOBBY_ENTER_RESULT result, const char* channelName, void* param)
{
    // VS2005���ƈ�x(u32)�ɃL���X�g����K�v������
    PPW_LOBBY_CHANNEL_KIND kind = (PPW_LOBBY_CHANNEL_KIND)(u32)param;
    if(kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_ASSERTMSG(FALSE, "LobbyJoinChannelCallback: Internal error. No such channel.");
        return;
    }
    
    if(success)
    {
        // �����o�[�̊m�F������
        //DWCi_LobbyEnumUsersAsync(channelName, LobbyEnumUsersCallback, kind);
        
        // �`�����l�����[�h�̕ύX������
        const DWCi_LobbyChannelMode* mode = PPW_LobbyGetChannelMode(kind);
        if(!mode)
        {
            s_lobby->SetError(PPW_LOBBY_ERROR_UNKNOWN);
        }
        DWCi_LobbySetChannelMode(channelName, mode);
        
        // �V�X�e���f�[�^�X�V
        PPW_LobbyInnerProfile& profile = s_lobby->GetProfileManager().GetMyProfile();
        PPW_LobbyInnerProfile::SystemProfile systemProfile = profile.GetSystemProfileConfirm();
        systemProfile.subChannelKind = kind;
        profile.SetSystemProfile(systemProfile);
    }
    else
    {
        s_lobby->GetSubChannel().Clear();
    }
    // VS2005���ƈ�x(u32)�ɃL���X�g����K�v������
    s_lobby->GetCallbacks().lobbyJoinChannelCallback(success, (PPW_LOBBY_CHANNEL_KIND)(u32)param);
    
    (void)result;
}

// �v���C���[�ڑ��R�[���o�b�N
static void LobbyPlayerConnectedCallback(const char* channelName, s32 userId, void* param)
{
    // param��PPW_LOBBY_CHANNEL_KIND_INVALID�Œ�
    
    PPW_LOBBY_CHANNEL_KIND kind = s_lobby->GetChannelManager().GetChannelKind(channelName);
    if(kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_ASSERTMSG(FALSE, "LobbyPlayerConnectedCallback: Internal error. No such channel.");
        return;
    }
    
    if(kind == PPW_LOBBY_CHANNEL_KIND_MAIN)
    {
        // ���C���`�����l���ɓ����Ă����l�̓R���e�i�ɓ����
        s_lobby->GetProfileManager().AddProfile(userId);
        
        // �V�X�e���E���[�U��`�v���t�B�[�������M���ꂽ��A�ڑ��R�[���o�b�N���Ă΂��
    }
    else
    {
        // �����������Ă���T�u�`�����l���ɏo���肵���Ƃ��̂݃V�X�e���f�[�^���s���čX�V���ʒm����
        PPW_LobbyInnerProfile* profile = s_lobby->GetProfileManager().FindProfile(userId);
        if(!profile)
        {
            // ���łɃR���e�i�ɓ����Ă���͂�
            DWC_ASSERTMSG(FALSE, "LobbyPlayerConnectedCallback: no profile.");
            s_lobby->GetProfileManager().AddProfile(userId);
            profile = s_lobby->GetProfileManager().FindProfile(userId);
        }
        PPW_LobbyInnerProfile::SystemProfile systemProfile = profile->GetSystemProfileConfirm();
        systemProfile.subChannelKind = kind;
        profile->SetSystemProfile(systemProfile);
        // �ύX���m�肷�邽�߂ɂ�����xGetSystemProfileConfirm()���Ă�
        const PPW_LobbyInnerProfile::SystemProfile& confirmedSystemProfile = profile->GetSystemProfileConfirm();
        PPW_LobbySystemProfile publicSystemProfile = confirmedSystemProfile.ToPublicSystemProfile();
        
        if(profile->IsActivated())
        {
            // �T�u�`�����l���ύX��ʒm
            s_lobby->GetCallbacks().lobbySystemProfileUpdatedCallback(userId, &publicSystemProfile);
        }
    }
    
    (void)param;
}

// �v���C���[�ؒf�R�[���o�b�N
static void LobbyPlayerDisconnectedCallback(const char* channelName, s32 userId, void* param)
{
    // param��PPW_LOBBY_CHANNEL_KIND_INVALID�Œ�
    
    PPW_LOBBY_CHANNEL_KIND kind = s_lobby->GetChannelManager().GetChannelKind(channelName);
    if(kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_ASSERTMSG(FALSE, "LobbyPlayerDisconnectedCallback: Internal error. No such channel.");
        return;
    }
    
    if(kind == PPW_LOBBY_CHANNEL_KIND_MAIN)
    {
        PPW_LobbyInnerProfile* profile = s_lobby->GetProfileManager().FindProfile(userId);
        if(!profile || !profile->IsActivated())
        {
            // �ڑ��R�[���o�b�N���Ă΂��O�ɑގ������ꍇ�͂Ȃɂ����Ȃ�
            DWC_Printf(DWC_REPORTFLAG_INFO, "LobbyPlayerDisconnectedCallback: user disconnected but did not call connect callback.\n");
            return;
        }
        
        // �R�[���o�b�N���Ă�
        s_lobby->GetCallbacks().lobbyPlayerDisconnectedCallback(userId);
        
        // ���C���`�����l������o���l�̓R���e�i����폜
        s_lobby->GetProfileManager().RemoveProfile(userId);
    }
    else
    {
        // �����������Ă���T�u�`�����l���ɏo���肵���Ƃ��̂݃V�X�e���f�[�^���s���čX�V���ʒm����
        PPW_LobbyInnerProfile* profile = s_lobby->GetProfileManager().FindProfile(userId);
        if(!profile)
        {
            // ���łɃR���e�i�ɓ����Ă���͂�
#ifdef _LINUX  // linux�Ŕ�������ꍇ������B
            DWC_Printf(DWC_REPORTFLAG_ERROR, "LobbyPlayerDisconnectedCallback: no profile.");
#else
            DWC_ASSERTMSG(FALSE, "LobbyPlayerDisconnectedCallback: no profile.");
#endif
            return;
        }
        PPW_LobbyInnerProfile::SystemProfile systemProfile = profile->GetSystemProfileConfirm();
        systemProfile.subChannelKind = PPW_LOBBY_CHANNEL_KIND_INVALID;
        profile->SetSystemProfile(systemProfile);
        // �ύX���m�肷�邽�߂ɂ�����xGetSystemProfileConfirm()���Ă�
        const PPW_LobbyInnerProfile::SystemProfile& confirmedSystemProfile = profile->GetSystemProfileConfirm();
        PPW_LobbySystemProfile publicSystemProfile = confirmedSystemProfile.ToPublicSystemProfile();
        
        if(profile->IsActivated())
        {
            // �T�u�`�����l���ύX��ʒm
            s_lobby->GetCallbacks().lobbySystemProfileUpdatedCallback(userId, &publicSystemProfile);
        }
    }
    (void)param;
}

// �v���C�x�[�g���b�Z�[�W��M�R�[���o�b�N
static void LobbyUserMassageReceivedCallback(s32 userId, const char* message, void* param)
{
    LobbyChannelMassageReceivedCallback(NULL, userId, message, param);
}

// ���b�Z�[�W��M�R�[���o�b�N
// channelName��NULL�̂Ƃ��̓v���C�x�[�g���b�Z�[�W
static void LobbyChannelMassageReceivedCallback(const char* channelName, s32 userId, const char* message, void* param)
{
    PPW_LOBBY_CHANNEL_KIND kind = PPW_LOBBY_CHANNEL_KIND_INVALID;
    if(channelName)
    {
        kind = s_lobby->GetChannelManager().GetChannelKind(channelName);
    }
    
    if(channelName != NULL && kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_ASSERTMSG(FALSE, "LobbyChannelMassageReceivedCallback: Internal error. No such channel.");
        return;
    }
    
    
    // ���b�Z�[�W���p�[�X
    PPW_LobbyMessage::Result result;
    if(!PPW_LobbyMessage::Parse(message, result))
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "LobbyChannelMassageReceivedCallback: parse failed\n");
        return;
    }
    
    switch(result.target)
    {
    // ���[�U��`���b�Z�[�W�Ȃ�R�[���o�b�N���Ă�
    case PPW_LobbyMessage::TARGET_APPLICATION:
    {
        // �A�N�e�B�x�[�g���Ă��Ȃ��v���C���[����̃��b�Z�[�W�͖���
        if(!s_lobby->GetProfileManager().IsActivatedProfile(userId))
        {
            DWC_Printf(DWC_REPORTFLAG_INFO, "LobbyChannelMassageReceivedCallback: Ignored not activated user's message.\n");
            return;
        }
        switch(result.format)
        {
        case PPW_LobbyMessage::FORMAT_BASE64:
            s_lobby->GetCallbacks().lobbyBinaryMassageReceivedCallback(userId, kind, result.type,
                                                                       (u8*)DWCi_GetVectorBuffer(result.data), result.data.size());
            break;
        case PPW_LobbyMessage::FORMAT_STRING:
            s_lobby->GetCallbacks().lobbyStringMassageReceivedCallback(userId, kind, result.type, (char*)DWCi_GetVectorBuffer(result.data));
            break;
        default:
            DWC_ASSERTMSG( FALSE, "LobbyChannelMassageReceivedCallback: Unknown message format." );
            break;
        }
        break;
    }
    // ���C�u�������ŏ���
    case PPW_LobbyMessage::TARGET_SYSTEM:
        PPW_SystemMessageHandler(result, userId, kind, (u8*)DWCi_GetVectorBuffer(result.data), result.data.size());
        break;
    default:
        DWC_ASSERTMSG( FALSE, "LobbyChannelMassageReceivedCallback: Unknown target." );
    }
    (void)param;
}

// ExcessFlood��M�R�[���o�b�N
static void LobbyExcessFloodCallback(u32 floodWeight, void* param)
{
    s_lobby->GetCallbacks().lobbyExcessFloodCallback(floodWeight);
    (void)param;
}

// �`�����l���f�[�^�u���[�h�L���X�g��M�R�[���o�b�N
static void LobbyBroadcastDataCallback(const char* channelName, s32 userId, const char* key, const char* value, void* param)
{
    // �u���[�h�L���X�g��param��NULL�ɂ���B
    PPW_ChannelDataHandler(TRUE, TRUE, channelName, userId, key, value, NULL);
    (void)param;
}

// �`�����l���f�[�^��M�����R�[���o�b�N
static void LobbyGetChannelDataCallback(BOOL success, const char* channelName, s32 userId, const char* key, const char* value, void* param)
{
    PPW_ChannelDataHandler(success, FALSE, channelName, userId, key, value, param);
}

// �`�����l���񋓃R�[���o�b�N
static void LobbyEnumChannelsCallback(BOOL success, s32 numChannels, const char** channels, const char** topics, const s32* numUsers, void* param)
{
    if(!success)
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "LobbyEnumChannelsCallback: failed\n");
        return;
    }
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "LobbyEnumChannelsCallback: found %d channels\n", numChannels);
    
    for(s32 i=0; i<numChannels; i++)
    {
        DWC_Printf(DWC_REPORTFLAG_INFO, "channel:%s, topic:%s, numUser:%u\n", channels[i], topics[i], numUsers[i]);
        if(numUsers[i] < PPW_LOBBY_MAX_PLAYER_NUM_MAIN)
        {
            // �ŏ���M!���������A���j�[�N���݂̂����o��
            std::vector<DWCi_String, DWCi_Allocator<DWCi_String> > result = DWCi_SplitByStr<DWCi_String>(channels[i], DWCi_LOBBY_CHANNEL_NAME_SEPARATOR);
            if(result.size() < 2)
            {
                continue;
            }
            
            s_lobby->SetUniqueChannelName(result[1].c_str());
            s_lobby->SetCreatedNewMainChannel(FALSE);
            s_lobby->SetState(PPW_LOBBY_STATE_ENTERMAINCHANNEL);
            return;
        }
    }
    
    // �K���ȃ`�����l����������Ȃ������̂Ŏ����Ń`�����l����V�K�ɍ��
    s_lobby->SetUniqueChannelName(PPW_LobbyGenerateUniqueChannelName());
    s_lobby->SetCreatedNewMainChannel(TRUE);
    s_lobby->SetState(PPW_LOBBY_STATE_ENTERMAINCHANNEL);
    
    (void)param;
    (void)topics;
    return;
}

// ���[�U�񋓃R�[���o�b�N
static void LobbyEnumUsersCallback(BOOL success, const char* channelName, s32 numUsers, const s32* userIds, void* param)
{
    if(!success)
    {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "LobbyEnumUsersCallback: failed\n");
        return;
    }
    
    DWC_Printf(DWC_REPORTFLAG_INFO, "LobbyEnumUsersCallback: exist %u users\n", numUsers);
    
    PPW_LOBBY_CHANNEL_KIND kind = s_lobby->GetChannelManager().GetChannelKind(channelName);
    if(kind == PPW_LOBBY_CHANNEL_KIND_INVALID)
    {
        DWC_ASSERTMSG(FALSE, "LobbyEnumUsersCallback: Internal error. No such channel.");
        return;
    }
    
    if(kind == PPW_LOBBY_CHANNEL_KIND_MAIN)
    {
        // ���C���`�����l���ɂ���l�̓R���e�i�ɓ���A�ڍ׏����擾����
        for(s32 i=0; i<numUsers; i++)
        {
            if(userIds[i] == s_lobby->GetPid())
            {
                // �������g�̏��̓X�L�b�v����
                continue;
            }
            s_lobby->GetProfileManager().AddProfile(userIds[i]);
            DWCi_LobbyGetChannelDataAsync(channelName, userIds[i], PPW_LOBBY_CHANNEL_KEY_USER, LobbyGetChannelDataCallback, NULL);
            DWCi_LobbyGetChannelDataAsync(channelName, userIds[i], PPW_LOBBY_CHANNEL_KEY_SYSTEM, LobbyGetChannelDataCallback, NULL);
        }
        
        s_lobby->GetMainChannel().SetUserListIsEnumerated();
    }
    else
    {
    }
    (void)param;
}

// ���C���`�����l���̃`�����l�����[�h���m�F����
static void LobbyCheckMainChannelModeCallback(BOOL success, const char* channelName, const DWCi_LobbyChannelMode* mode, void* param)
{
    if(!success)
    {
        // TODO: ���g���C�J�E���g����
        return;
    }
    
    if(mode->limit == PPW_LOBBY_MAX_PLAYER_NUM_MAIN)
    {
        // �����ݒ�ς�
        s_lobby->GetMainChannel().SetModeIsSet();
        s_lobby->GetTimerManager().RemoveTimer(PPW_LOBBY_TIMER_ID_CHECK_MAINCHANNEL_MODE);
    }
    else
    {
        // �����ݒ肳��Ă��Ȃ�
        
        // �`�����l���̏����ݒ������
        DWCi_LobbySetChannelMode(channelName, &PPW_LOBBY_CHANNEL_MODE_MAIN_OPEN);
    }
    (void)param;
}

// ���C���`�����l���̃V�X�e���f�[�^���m�F����
static void LobbyCheckMainChannelSystemProfileCallback(BOOL success, const char* channelName, s32 userId, const char* key, const char* value, void* param)
{
    if(!success)
    {
        // TODO: ���g���C�J�E���g����
        return;
    }
    
    // Base64�f�R�[�h
    std::vector<u8, DWCi_Allocator<u8> > tempBuf;
    BOOL result = DWCi_Base64Decode(value, tempBuf);
    if(!result || tempBuf.size() != sizeof(PPW_LobbyInnerInfoManager::TimeData))
    {
        // �����ݒ肳��Ă��Ȃ��̂Ŏ����̃f�[�^���X�g�A
        PPW_LobbyInnerInfoManager::TimeData systemProfile = s_lobby->GetLobbyInnerInfoManager().InitializeTimeData();
        PPW_LobbySetChannelDataImpl(PPW_LOBBY_INVALID_USER_ID, PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LOBBY_CHANNEL_KEY_CHANNEL_TIME,
                                    (u8*)&systemProfile, sizeof(systemProfile));
        return;
    }
    else
    {
        // �����ݒ�ς�
        PPW_LobbyInnerInfoManager::TimeData* systemProfile = (PPW_LobbyInnerInfoManager::TimeData*)DWCi_GetVectorBuffer(tempBuf);
        DWC_Printf(DWC_REPORTFLAG_INFO, "LobbyCheckMainChannelSystemProfileCallback: openedTime is %lld\n", systemProfile->openedTime);
        s_lobby->GetLobbyInnerInfoManager().SetTimeData(*systemProfile);
        s_lobby->GetTimerManager().RemoveTimer(PPW_LOBBY_TIMER_ID_CHECK_MAINCHANNEL_SYSTEMDATA);
    }
    (void)channelName;
    (void)userId;
    (void)key;
    (void)param;
}

// ���C���`�����l���̃V�X�e���f�[�^(���r�[�f�[�^)���m�F����
static void LobbyCheckLobbyInfoCallback(BOOL success, const char* channelName, s32 userId, const char* key, const char* value, void* param)
{
    if(!success)
    {
        // TODO: ���g���C�J�E���g����
        return;
    }
    
    // Base64�f�R�[�h
    std::vector<u8, DWCi_Allocator<u8> > tempBuf;
    BOOL result = DWCi_Base64Decode(value, tempBuf);

    // �T�C�Y�`�F�b�N�Ƒ�����܂�
    if(!result || tempBuf.empty()
       || !s_lobby->GetLobbyInnerInfoManager().SetLobbyInfo(DWCi_GetVectorBuffer(tempBuf), tempBuf.size()))
    {
        // �����ݒ肳��Ă��Ȃ��̂Ŏ����̃f�[�^���X�g�A
        std::vector<u8, DWCi_Allocator<u8> > buf = s_lobby->GetLobbyInnerInfoManager().Serialize();
        PPW_LobbySetChannelDataImpl(PPW_LOBBY_INVALID_USER_ID, PPW_LOBBY_CHANNEL_KIND_MAIN, PPW_LOBBY_CHANNEL_KEY_CHANNEL_LOBBY,
                                    DWCi_GetVectorBuffer(buf), buf.size());
        return;
    }
    else
    {
        // �ݒ�ς�
        s_lobby->GetLobbyInnerInfoManager().Shared();
        s_lobby->GetTimerManager().RemoveTimer(PPW_LOBBY_TIMER_ID_CHECK_LOBBY_INFO);
    }
    (void)channelName;
    (void)userId;
    (void)key;
    (void)param;
}

// �g�s�b�N�ύX�R�[���o�b�N
static void LobbyTopicChangedCallback(const char* channelName, const char* topic, void* param)
{
    (void)channelName;
    (void)topic;
    (void)param;
}
