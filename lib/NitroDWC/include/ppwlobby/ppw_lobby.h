/*---------------------------------------------------------------------------*
  Project:  NitroDWC Libraries
  File:     ./include/ppwlobby/dwc_lobby.h

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
 * @brief �|�P�����v���`�i Wi-Fi ���r�[���C�u���� �w�b�_
 * 
 * �|�P�����v���`�i Wi-Fi ���r�[���C�u����
 * 
 * @author Ryo Yamaguchi (yamaguchi_ryo@nintendo.co.jp)
 */

#ifndef PPW_LOBBY_H_
#define PPW_LOBBY_H_

#include <lobby/dwc_lobby.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32
#pragma pack(push, 4)
#endif

/// PPW_LobbySend*StringMessage�֐��ő��M�ł���ő�̕������B(�I�[�܂�)
#define PPW_LOBBY_MAX_STRING_SIZE               400

/// PPW_LobbySend*BinaryMessage�֐��ő��M�ł���ő�̃f�[�^���B
#define PPW_LOBBY_MAX_BINARY_SIZE               300

/// ::PPW_LobbyRecruitInfo�\���̂�paramBuf�����o�Ɏw��ł���f�[�^���B
#define PPW_LOBBY_MAX_MATCHMAKING_BUF_SIZE      32

/// ::PPW_LobbyRecruitInfo�\���̂�matchMakingString�����o�Ɏw�肳���}�b�`���C�L���O�w�W������̕�����(�I�[�܂�)�B
#define PPW_LOBBY_MAX_MATCHMAKING_STRING_LENGTH 20

/// ::PPW_LobbyQuestionnaireRecord�\���̂�questionSentence�����o�̍ő啶����(�I�[�܂�)�B
#define PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH 110

/// ::PPW_LobbyQuestionnaireRecord�\���̂�answer�����o�̍ő啶����(�I�[�܂�)�B
#define PPW_LOBBY_MAX_ANSWER_LENGTH             18

/// �A���P�[�g�@�\�ł̑I�����̐�
#define PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM      3

/// �C�ӎ���̊J�n����ԍ�
#define PPW_LOBBY_FREE_QUESTION_START_NO        1000

/// �C�ӎ���ԍ��̌�
#define PPW_LOBBY_FREE_QUESTION_NUM             10

/// �A���P�[�g���J�Â���Ă��Ȃ����Ƃ���������ԍ�
#define PPW_LOBBY_INVALID_QUESTION_NO           0xffffffff

// ���r�[���C�u�����Ŏg�p����G���[�R�[�h�̊�ԍ�
#define PPW_LOBBY_ERROR_CODE_BASE               10620

/// ���C�u�������V�X�e����`�v���t�B�[���̑���M�Ɏg��"�e�v���C���[�Ɋ��蓖�Ă���"�`�����l���f�[�^�L�[�B(�A�v���P�[�V�����ł͎g�p���Ȃ��ł�������)
#define PPW_LOBBY_CHANNEL_KEY_SYSTEM            "b_lib_u_system"
    
/// ���C�u���������[�U��`�v���t�B�[���̑���M�Ɏg��"�e�v���C���[�Ɋ��蓖�Ă���"�`�����l���f�[�^�L�[�B(�A�v���P�[�V�����ł͎g�p���Ȃ��ł�������)
#define PPW_LOBBY_CHANNEL_KEY_USER              "b_lib_u_user"

/// ���C�u�������`�����l���̊Ǘ��Ɏg��"�`�����l�����̂Ɋ��蓖�Ă���"�`�����l���f�[�^�L�[�B(�A�v���P�[�V�����ł͎g�p���Ȃ��ł�������)
#define PPW_LOBBY_CHANNEL_KEY_CHANNEL_TIME      "b_lib_c_time"

/// ���C�u�������`�����l���̊Ǘ��Ɏg��"�`�����l�����̂Ɋ��蓖�Ă���"�`�����l���f�[�^�L�[�B(�A�v���P�[�V�����ł͎g�p���Ȃ��ł�������)
#define PPW_LOBBY_CHANNEL_KEY_CHANNEL_LOBBY     "b_lib_c_lobby"

/// �~�j�Q�[����W�Ŏg�p����}�b�`���C�N�w�W�L�[�B
#define PPW_LOBBY_MATCHMAKING_KEY               "ppw_lobby"

// ���C�u���������Ŏg�p����f�[�^�Ǘ��T�[�o�ւ̃��N�G�X�g�̃o�[�W����
#define PPW_LOBBY_ENC_REQUEST_VERSION           1

#define PPW_LOBBY_MAX_PLAYER_NUM_MAIN       20                          ///< ���C���`�����l���ɓ����ő�l���B
#define PPW_LOBBY_MAX_PLAYER_NUM_FOOT       8                           ///< ���Ճ{�[�h�����ő�l���B
#define PPW_LOBBY_MAX_PLAYER_NUM_CLOCK      4                           ///< ���E���v�ɓ����ő�l���B
#define PPW_LOBBY_MAX_PLAYER_NUM_NEWS       4                           ///< ���r�[�j���[�X�ɓ����ő�l���B

#define PPW_LOBBY_INVALID_USER_ID           DWC_LOBBY_INVALID_USER_ID   ///< �����ȃ��[�UID�B
#define PPW_LOBBY_INVALID_TIME              DWC_LOBBY_INVALID_TIME      ///< �����Ȏ��ԁB


/// �`�����l�����ɕt����v���t�B�b�N�X�B�����`�����l���𕪂��邱�Ƃ��ł��܂��BPPW_LOBBY_CHANNEL_PREFIX*���g�p�ł��܂����A�ł��邾��DWC_LOBBY_CHANNEL_PREFIX*�̕������g���������B
typedef DWC_LOBBY_CHANNEL_PREFIX            PPW_LOBBY_CHANNEL_PREFIX;
#define PPW_LOBBY_CHANNEL_PREFIX_RELEASE    DWC_LOBBY_CHANNEL_PREFIX_RELEASE    ///< ���i�p�B(->DWC_LOBBY_CHANNEL_PREFIX_RELEASE)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG      DWC_LOBBY_CHANNEL_PREFIX_DEBUG      ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG1     DWC_LOBBY_CHANNEL_PREFIX_DEBUG1     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG1)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG2     DWC_LOBBY_CHANNEL_PREFIX_DEBUG2     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG2)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG3     DWC_LOBBY_CHANNEL_PREFIX_DEBUG3     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG3)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG4     DWC_LOBBY_CHANNEL_PREFIX_DEBUG4     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG4)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG5     DWC_LOBBY_CHANNEL_PREFIX_DEBUG5     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG5)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG6     DWC_LOBBY_CHANNEL_PREFIX_DEBUG6     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG6)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG7     DWC_LOBBY_CHANNEL_PREFIX_DEBUG7     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG7)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG8     DWC_LOBBY_CHANNEL_PREFIX_DEBUG8     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG8)
#define PPW_LOBBY_CHANNEL_PREFIX_DEBUG9     DWC_LOBBY_CHANNEL_PREFIX_DEBUG9     ///< �f�o�b�O�p�B(->DWC_LOBBY_CHANNEL_PREFIX_DEBUG9)
#define PPW_LOBBY_CHANNEL_PREFIX_TEST       DWC_LOBBY_CHANNEL_PREFIX_TEST       ///< ���C�u�����J���p�B(�w�肵�Ȃ��ł�������)(->DWC_LOBBY_CHANNEL_PREFIX_TEST)
    

/// ���r�[���C�u������ԃt���O�B
typedef enum PPW_LOBBY_STATE
{
    PPW_LOBBY_STATE_NOTINITIALIZED,         ///< ���C�u����������������Ă��Ȃ��B
    PPW_LOBBY_STATE_CONNECTING,             ///< �`���b�g�T�[�o�ɐڑ����B
    PPW_LOBBY_STATE_CONNECTED,              ///< �`���b�g�T�[�o�ɐڑ������B
    PPW_LOBBY_STATE_DOWNLOAD,               ///< �T�[�o����ݒ���_�E�����[�h�J�n�B
    PPW_LOBBY_STATE_DOWNLOAD_WAIT,          ///< �T�[�o����ݒ���_�E�����[�h���B
    PPW_LOBBY_STATE_SEARCHINGCHANNEL,       ///< ���C���`�����l�����������B
    PPW_LOBBY_STATE_ENTERMAINCHANNEL,       ///< ���C���`�����l���ɓ����J�n�B
    PPW_LOBBY_STATE_PREPARINGMAINCHANNEL,   ///< ���C���`�����l���ɓ����y�я������B
    PPW_LOBBY_STATE_READY,                  ///< ���C���`�����l���ɓ��������B
    PPW_LOBBY_STATE_CLOSING,                ///< ���C�u�����I���������B
    PPW_LOBBY_STATE_ERROR                   ///< ���C�u�����ɒv���I�ȃG���[���������B::PPW_LobbyGetLastError�֐��ŃG���[���擾��V���b�g�_�E�����Ă��������B
}
PPW_LOBBY_STATE;

/// �`�����l����ԃt���O�B
typedef enum PPW_LOBBY_CHANNEL_STATE
{
    PPW_LOBBY_CHANNEL_STATE_NONE,           ///< �`�����l���ɓ����Ă��Ȃ��B
    PPW_LOBBY_CHANNEL_STATE_UNAVAILABLE,    ///< �`�����l���ɓ����������B
    PPW_LOBBY_CHANNEL_STATE_AVAILABLE,      ///< �`�����l���ɓ����ς݁B
    PPW_LOBBY_CHANNEL_STATE_ERROR           ///< �`�����l����Ԃ��擾�ł��܂���ł����B
}
PPW_LOBBY_CHANNEL_STATE;

/// �������ʃt���O�B
typedef enum PPW_LOBBY_RESULT
{
    PPW_LOBBY_RESULT_SUCCESS,               ///< �����B
    PPW_LOBBY_RESULT_ERROR_ALLOC,           ///< �������m�ۂɎ��s�B
    PPW_LOBBY_RESULT_ERROR_SESSION,         ///< �v���I�ȒʐM�G���[�B
    PPW_LOBBY_RESULT_ERROR_PARAM,           ///< �w�肵���������s���B
    PPW_LOBBY_RESULT_ERROR_STATE,           ///< �Ăяo���Ă͂����Ȃ���ԂŊ֐����Ăяo�����B
    PPW_LOBBY_RESULT_ERROR_CHANNEL,         ///< �w�肵���`�����l���ɓ����Ă��Ȃ��B
    PPW_LOBBY_RESULT_ERROR_NODATA,          ///< �w�肵�����͑��݂��Ȃ��B
    PPW_LOBBY_RESULT_ERROR_CONDITION,       ///< �v���I�G���[���������Ȃ̂ł��̊֐����Ăяo���Ȃ��B
    PPW_LOBBY_RESULT_MAX                    ///< ���C�u���������Ŏg�p����܂��B
}
PPW_LOBBY_RESULT;

/// �`�����l���̎�ށB
typedef enum PPW_LOBBY_CHANNEL_KIND
{
    PPW_LOBBY_CHANNEL_KIND_MAIN,            ///< ���C���`�����l���B
    PPW_LOBBY_CHANNEL_KIND_FOOT1,           ///< ���Ճ{�[�h1�B
    PPW_LOBBY_CHANNEL_KIND_FOOT2,           ///< ���Ճ{�[�h2�B
    PPW_LOBBY_CHANNEL_KIND_CLOCK,           ///< ���E���v�B
    PPW_LOBBY_CHANNEL_KIND_NEWS,            ///< ���r�[�j���[�X�B
    PPW_LOBBY_CHANNEL_KIND_INVALID          ///< �����ȃ`�����l���B
}
PPW_LOBBY_CHANNEL_KIND;

/// �G���[�t���O�B
typedef enum PPW_LOBBY_ERROR
{
    PPW_LOBBY_ERROR_NONE,                   ///< �G���[�����B
    PPW_LOBBY_ERROR_UNKNOWN,                ///< �s���ȃG���[�B
    PPW_LOBBY_ERROR_ALLOC,                  ///< �������m�ۂɎ��s�B
    PPW_LOBBY_ERROR_SESSION,                ///< �v���I�ȒʐM�G���[(�`���b�g�T�[�o)�B
    PPW_LOBBY_ERROR_STATS_SCHEDULE,         ///< �v���I�ȒʐM�G���[(�X�P�W���[���擾��)�B
    PPW_LOBBY_ERROR_STATS_VIP,              ///< �v���I�ȒʐM�G���[(VIP�ݒ�擾��)�B
    PPW_LOBBY_ERROR_STATS_CHECKPROFILE,     ///< �v���I�ȒʐM�G���[(�s���`�F�b�N��)�B
    PPW_LOBBY_ERROR_STATS_QUESTIONNAIRE,    ///< �v���I�ȒʐM�G���[(�A���P�[�g���擾��)�B
    PPW_LOBBY_ERROR_MAX                     ///< ���C�u���������Ŏg�p����܂��B
}
PPW_LOBBY_ERROR;

/// �^�C���C�x���g�̎�ށB::PPW_LobbyScheduleProgressCallback�R�[���o�b�N�Ŏg�p����܂��B
typedef enum PPW_LOBBY_TIME_EVENT
{
    PPW_LOBBY_TIME_EVENT_LOCK,              ///< �����̃��b�N�B
    PPW_LOBBY_TIME_EVENT_NEON_A0,           ///< �����l�I���~��0�B
    PPW_LOBBY_TIME_EVENT_NEON_A1,           ///< �����l�I���~��1�B
    PPW_LOBBY_TIME_EVENT_NEON_A2,           ///< �����l�I���~��2�B
    PPW_LOBBY_TIME_EVENT_NEON_A3,           ///< �����l�I���~��3�B
    PPW_LOBBY_TIME_EVENT_NEON_A4,           ///< �����l�I���~��4�B
    PPW_LOBBY_TIME_EVENT_NEON_A5,           ///< �����l�I���~��5�B
    PPW_LOBBY_TIME_EVENT_NEON_B0,           ///< ���l�I���~��0�B
    PPW_LOBBY_TIME_EVENT_NEON_B1,           ///< ���l�I���~��1�B
    PPW_LOBBY_TIME_EVENT_NEON_B2,           ///< ���l�I���~��2�B
    PPW_LOBBY_TIME_EVENT_NEON_B3,           ///< ���l�I���~��3�B
    PPW_LOBBY_TIME_EVENT_NEON_C0,           ///< ���j�������g�~��0�B
    PPW_LOBBY_TIME_EVENT_NEON_C1,           ///< ���j�������g�~��1�B
    PPW_LOBBY_TIME_EVENT_NEON_C2,           ///< ���j�������g�~��2�B
    PPW_LOBBY_TIME_EVENT_NEON_C3,           ///< ���j�������g�~��3�B
    PPW_LOBBY_TIME_EVENT_MINIGAME_END,      ///< �~�j�Q�[���I���B
    PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START,  ///< �t�@�C�A�[���[�N�X�J�n�B
    PPW_LOBBY_TIME_EVENT_FIRE_WORKS_END,    ///< �t�@�C�A�[���[�N�X�I���B
    PPW_LOBBY_TIME_EVENT_PARADE,            ///< �p���[�h�B
    PPW_LOBBY_TIME_EVENT_CLOSE              ///< �I���B
}
PPW_LOBBY_TIME_EVENT;

/// ::PPW_LobbySchedule�\���̂�roomFlag�����o�ɑΉ����镔���ݒ�t���O
typedef enum PPW_LOBBY_ROOM_FLAG
{
    PPW_LOBBY_ROOM_FLAG_ARCEUS  = 0x00000001    ///< �A���Z�E�X�������邩
}
PPW_LOBBY_ROOM_FLAG;

/// �X�P�W���[����VIP�ݒ�̃_�E�����[�h����
typedef enum PPW_LOBBY_STATS_RESULT
{
    PPW_LOBBY_STATS_RESULT_SUCCESS,         ///< �����B
    PPW_LOBBY_STATS_RESULT_INVALID_PARAM,   ///< ���M�����p�����[�^���s���ł��B
    PPW_LOBBY_STATS_RESULT_SERVER_ERROR     ///< �T�[�o���G���[��Ԃ��܂����B
}
PPW_LOBBY_STATS_RESULT;


/// �V�X�e����`�v���t�B�[���B
typedef struct PPW_LobbySystemProfile
{
    s64 enterTime;                          ///< ���r�[�ɓ����������B
    PPW_LOBBY_CHANNEL_KIND subChannelKind;  ///< �Q�����Ă���T�u�`�����l���B
}
PPW_LobbySystemProfile;

/// �~�j�Q�[����W�v���B
typedef struct PPW_LobbyRecruitInfo
{
    s32 gameKind;                           ///< ��W����~�j�Q�[���̎�ށB
    u32 maxNum;                             ///< ��W����ő�l���B
    u32 currentNum;                         ///< ���ݏW�܂��Ă���l���B
    u32 paramVal;                           ///< ���[�U��`�f�[�^1�B
    u8 paramBuf[PPW_LOBBY_MAX_MATCHMAKING_BUF_SIZE];    ///< ���[�U��`�f�[�^2�B
    
    /// �}�b�`���O�w�W������B::PPW_LobbyStartRecruit�֐������ɂăZ�b�g����܂��̂ŕύX���Ȃ��ł��������B
    char matchMakingString[PPW_LOBBY_MAX_MATCHMAKING_STRING_LENGTH];
}
PPW_LobbyRecruitInfo;

/// �������B
typedef struct PPW_LobbyTimeInfo
{
    s64 currentTime;                        ///< ���݂̎����B
    s64 openedTime;                         ///< �������쐬���������B
    s64 lockedTime;                         ///< ���������b�N���������B
}
PPW_LobbyTimeInfo;

/// �X�P�W���[�����R�[�h�B
typedef struct PPW_LobbyScheduleRecord
{
    s32 time;                               ///< �C�x���g���N�����鎞��(���C���`�����l�������b�N�����Ƃ�����̎���)�B
    PPW_LOBBY_TIME_EVENT event;             ///< �N������C�x���g�B
}
PPW_LobbyScheduleRecord;

/// �T�[�o����擾�����`�����l���̐ݒ���ł��B�ϒ��\���̂ł��B
typedef struct PPW_LobbySchedule
{
    u32 lockTime;                           ///< ���C���`�����l�����I�[�v�����Ă��烍�b�N����܂ł̎���(�b)�B
    u32 random;                             ///< �T�[�o�Ő��������32bit�͈͂̃����_���l�B
    u32 roomFlag;                           ///< ::PPW_LOBBY_ROOM_FLAG�ɑΉ�����e��bit�t���O�B
    u8 roomType;                            ///< �����̎�ށB
    u8 season;                              ///< �G�ߔԍ��B
    u16 scheduleRecordNum;                  ///< �X�P�W���[�����R�[�h�̐��B
    PPW_LobbyScheduleRecord scheduleRecords[1]; ///< �X�P�W���[�����R�[�h�̔z��(�ϒ�)�B
}
PPW_LobbySchedule;

/// VIP���R�[�h�B
typedef struct PPW_LobbyVipRecord
{
    s32 profileId;                          ///< VIP��ProfileID�B
    s32 key;                                ///< �����t�����p�L�[�B�����t�����̏ꍇ��0�ɂȂ�܂��B
}
PPW_LobbyVipRecord;

/// �A���P�[�g���e
typedef struct PPW_LobbyQuestionnaireRecord
{
    s32 questionSerialNo;                   ///< ����ʂ��ԍ��B0����X�^�[�g�B
    s32 questionNo;                         ///< ����ԍ��BROM������:0�`59 �C�ӎ���:PPW_LOBBY_FREE_QUESTION_START_NO�`PPW_LOBBY_FREE_QUESTION_START_NO+PPW_LOBBY_FREE_QUESTION_NUM
    u16 questionSentence[PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH];   ///< �C�ӎ���B����ԍ����C�ӎ���͈̔͂������Ƃ��̂݊i�[����܂��B
    u16 answer[PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM][PPW_LOBBY_MAX_ANSWER_LENGTH];    ///< �C�ӎ���̉񓚁B����ԍ����C�ӎ���͈̔͂������Ƃ��̂݊i�[����܂��B
    u8 multiLanguageSummarizeFlags[12];     ///< �W�v����B0�͖��W�v�A1�͏W�v�������Ƃ������܂��B�C���f�b�N�X�ɂ͌���R�[�h�ɑΉ����܂��B[0]��[6]��[9]�ȍ~�͏��0�ɂȂ�܂��B
    BOOL isSpecialWeek;                     ///< �X�y�V�����E�B�[�N��
}
PPW_LobbyQuestionnaireRecord;

/// �A���P�[�g���
typedef struct PPW_LobbyQuestionnaire
{
    PPW_LobbyQuestionnaireRecord currentQuestionnaireRecord;    ///< ���݂̃A���P�[�g���
    PPW_LobbyQuestionnaireRecord lastQuestionnaireRecord;       ///< �O��̃A���P�[�g���
    s32 lastResult[PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM];         ///< �O��̎����̌���̌���
    s32 lastMultiResult[PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM];    ///< �O��̕����W�v����ł̏W�v���ʁB::lastQuestionnaireRecord�\���̂�multiLanguageSummarizeFlags�����o�ŕ\����錾��ł̏W�v���ʂł��B
}
PPW_LobbyQuestionnaire;


// �R�[���o�b�N�֐��^��`

/**
 * @brief �������`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �������`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] success �����ɐ����������B�����Ɏ��s����̂̓T�u�`�����l���ɓ��낤�Ƃ����Ƃ��ɒ���I�[�o�[���Ă����ꍇ�݂̂ł��B
 * @param[in] channelKind ���������`�����l���̎�ށB
 */
typedef void (*PPW_LobbyJoinChannelCallback)(BOOL success, PPW_LOBBY_CHANNEL_KIND channelKind);

/**
 * @brief �������g�������͑��̃v���C���[�����C���`�����l���ɓ��������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �������g�������͑��̃v���C���[�����C���`�����l���ɓ��������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B\n
 * ���������C���`�����l���ɓ���Ƃ��A�������g�Ǝ�������������ȑO���炢���v���C���[�ɑ΂���R�[���o�b�N��\n
 * �X�e�[�g��::PPW_LOBBY_STATE_PREPARINGMAINCHANNEL�̎��ɔ������܂��B\n
 * �������g�ɑ΂���R�[���o�b�N�͕K����ԍŏ��ɍs���܂��B
 * 
 * @param[in] userId �����������[�UID�B
 * @param[in] systemProfile ���������v���C���[�̃V�X�e����`�v���t�B�[���B
 * @param[in] userProfile ���������̃��[�U��`�v���t�B�[���B
 * @param[in] userProfileSize userProfile�̃T�C�Y�B
 */
typedef void (*PPW_LobbyPlayerConnectedCallback)(s32 userId, const PPW_LobbySystemProfile* systemProfile, const u8* userProfile, u32 userProfileSize);

/**
 * @brief ���̃v���C���[�����C���`�����l������ގ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̃v���C���[�����C���`�����l������ގ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId �ގ��������[�UID�B
 */
typedef void (*PPW_LobbyPlayerDisconnectedCallback)(s32 userId);

/**
 * @brief �����񃁃b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �����񃁃b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId ���M���̃��[�UID�B
 * @param[in] channelKind ���M���ꂽ�`�����l���̎�ށBPPW_LobbySendUser*Message�֐��ő��M���ꂽ�Ƃ���::PPW_LOBBY_INVALID_USER_ID���w�肳��܂��B
 * @param[in] type ���M�҂��w�肵�����b�Z�[�W�̃^�C�v�B
 * @param[in] message ��M����NULL�I�[������B
 */
typedef void (*PPW_LobbyStringMassageReceivedCallback)(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const char* message);

/**
 * @brief �o�C�i�����b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �o�C�i�����b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId ���M���̃��[�UID�B
 * @param[in] channelKind ���M���ꂽ�`�����l���̎�ށBPPW_LobbySendUser*Message�֐��ő��M���ꂽ�Ƃ���::PPW_LOBBY_INVALID_USER_ID�ƂȂ�܂��B
 * @param[in] type ���M�҂��w�肵�����b�Z�[�W�̃^�C�v�B
 * @param[in] data ��M�����o�C�i���f�[�^�B
 * @param[in] dataSize data�̃T�C�Y�B
 */
typedef void (*PPW_LobbyBinaryMassageReceivedCallback)(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const u8* data, u32 dataSize);

/**
 * @brief �`�����l���f�[�^���擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �`�����l���f�[�^���擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * ::PPW_LobbyGetChannelDataAsync�֐��̌��ʂ܂��͑��̃v���C���[��::PPW_LobbySetChannelData�֐��Ńu���[�h�L���X�g�L�[("b_"�Ŏn�܂�L�[)���w�肵�A
 * �������M�����Ƃ��ɌĂяo����܂��B
 * 
 * @param[in] success �����������B�u���[�h�L���X�g����M�����ꍇ�͕K��TRUE�ƂȂ�܂��B
 * @param[in] broadcast ::PPW_LobbyGetChannelDataAsync�֐��Ŏ擾�������̂Ȃ�FALSE�A
 *                      ���̃v���C���[��::PPW_LobbySetChannelData�֐��Ńu���[�h�L���X�g���ꂽ���̂Ȃ�TRUE�B
 * @param[in] channelKind ���̃`�����l���f�[�^�Ɋ֘A�Â����Ă���`�����l���̎�ށB
 * @param[in] userId ���̃`�����l���f�[�^�������[�UID�B�`�����l�����̂̃f�[�^���擾�����Ƃ���::PPW_LOBBY_INVALID_USER_ID�ƂȂ�܂��B
 * @param[in] key ��M�����L�[������B
 * @param[in] data key�Ɋ֘A�Â���ꂽ�f�[�^�B
 * @param[in] dataSize data�̃T�C�Y�B
 * @param[in] param ::PPW_LobbyGetChannelDataAsync�֐��Ɏw�肵��param�B�u���[�h�L���X�g����M�����ꍇ��NULL�ɂȂ�܂��B
 */
typedef void (*PPW_LobbyGetChannelDataCallback)(BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channelKind, s32 userId,
                                                const char* key, const u8* data, u32 dataSize, void* param);

/**
 * @brief ���̃v���C���[�̃V�X�e����`�v���t�B�[��(�����T�u�`�����l���Ȃ�)���ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̃v���C���[�̃V�X�e����`�v���t�B�[��(�����T�u�`�����l���Ȃ�)���ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B\n
 * ���݂̓v���C���[���T�u�`�����l����ύX�����Ƃ��̂݌Ăяo����܂��B
 * 
 * @param[in] userId �V�X�e����`�v���t�B�[����ύX�������[�UID�B
 * @param[in] systemProfile �ύX���ꂽ�V�X�e����`�v���t�B�[���B
 */
typedef void (*PPW_LobbySystemProfileUpdatedCallback)(s32 userId, const PPW_LobbySystemProfile* systemProfile);

/**
 * @brief ���̃v���C���[�̃��[�U��`�v���t�B�[�����ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̃v���C���[�̃��[�U��`�v���t�B�[�����ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId ���[�U��`�v���t�B�[����ύX�������[�UID�B
 * @param[in] userProfile �ύX���ꂽ���[�U��`�v���t�B�[���B
 * @param[in] userProfileSize userProfile�̃T�C�Y�B
 */
typedef void (*PPW_LobbyUserProfileUpdatedCallback)(s32 userId, const u8* userProfile, u32 userProfileSize);

/**
 * @brief ���̃v���C���[���~�j�Q�[�����W���Ă��鎞�ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̃v���C���[���~�j�Q�[�����W���Ă��鎞�ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId ��W���Ă��郆�[�UID�B
 * @param[in] recruitInfo ��W�v���B
 */
typedef void (*PPW_LobbyRecruitCallback)(s32 userId, const PPW_LobbyRecruitInfo* recruitInfo);

/**
 * @brief ���̃v���C���[���~�j�Q�[���̕�W���I���������ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * ���̃v���C���[���~�j�Q�[���̕�W���I���������ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] userId ��W���Ă������[�UID�B
 * @param[in] recruitInfo �Ō�Ɏg�p���Ă�����W�v���B
 */
typedef void (*PPW_LobbyRecruitStoppedCallback)(s32 userId, const PPW_LobbyRecruitInfo* recruitInfo);

/**
 * @brief �C�x���g�����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �C�x���g�����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] event ���������C�x���g�̎�ށB
 */
typedef void (*PPW_LobbyScheduleProgressCallback)(PPW_LOBBY_TIME_EVENT event);

/**
 * @brief �s���`�F�b�N�̌��ʂ�ʒm����R�[���o�b�N�֐��^�B
 * 
 * �s���`�F�b�N�̌��ʂ�ʒm����R�[���o�b�N�֐��^�BROM�o�[�W�����Ȃǂɂ�����v���I�ȕs���������ꍇ��result��::PPW_LOBBY_STATS_RESULT_INVALID_PARAM�ɂȂ�A
 * �u���\�ȃp�����[�^���s���������ꍇ�͒u�����ꂽ���[�U��`�v���t�B�[�����ʒm����܂��B
 * 
 * @param[in] result �ʐM���ʁB::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�������Ƃ��̓��C�u�����̓G���[��ԂɂȂ�܂��B
 *                   ���̂Ƃ���::PPW_LobbyShutdownAsync�֐��Ń��C�u�������I�����Ă��������B
 *                   �T�[�o�ɑ��M�������[�U��`�v���t�B�[�����s���`�F�b�N�Ɉ������������ꍇ��::PPW_LOBBY_STATS_RESULT_INVALID_PARAM�ɂȂ�܂��B
 * @param[in] userProfile (�����u���ӏ��������)�u�����ꂽ���[�U��`�v���t�B�[���B
 *                        ���łɂ��̃v���t�B�[�����g�p����悤�ɃZ�b�g����Ă��܂��̂�::PPW_LobbyUpdateMyProfile�֐���p���čăZ�b�g����K�v�͂���܂���B
 *                        result��::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�̂Ƃ���NULL�ɂȂ�܂��B
 * @param[in] userProfileSize userProfile�̃T�C�Y�B
 */
typedef void (*PPW_LobbyCheckProfileCallback)(PPW_LOBBY_STATS_RESULT result, const u8* userProfile, u32 userProfileSize);

/**
 * @brief �T�[�o����X�P�W���[�����擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �T�[�o����X�P�W���[�����擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * ���̃R�[���o�b�N�œn���ꂽ�X�P�W���[�������ۂ�Wi-Fi�Ђ�΂̃X�P�W���[���ɍ̗p����邩�͊m�肵�Ă��܂���B
 * ���C���`�����l���Ɉ�ԍŏ��ɓ������l�̃X�P�W���[�����g�p����邱�ƂɂȂ�܂��B
 * ::PPW_LobbyJoinChannelCallback�R�[���o�b�N�����������C���`�����l���ɓ��������Ƃ��ɃX�P�W���[�����m�肵�܂��̂�
 * ���̂Ƃ���::PPW_LobbyGetSchedule�֐��Ŏ擾���Ă��������B
 * �Ԃ�l�łǂ̃f�[�^���̗p���ɂ��邩��I�����Ă��������B
 * 
 * @param[in] result �ʐM���ʁB::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�������Ƃ��̓��C�u�����̓G���[��ԂɂȂ�܂��B
 *                   ���̂Ƃ���::PPW_LobbyShutdownAsync�֐��Ń��C�u�������I�����Ă��������B
 * @param[in] schedule �擾�����X�P�W���[���Bresult��::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�̂Ƃ���NULL�ɂȂ�܂��B
 * 
 * @retval TRUE �_�E�����[�h���Ă������̃X�P�W���[�����g�p����B
 * @retval FALSE �f�o�b�O�p�Ƀ_�E�����[�h�������̃X�P�W���[���͎g�킸�A
 *               ���O�ɂ܂��͖{�R�[���o�b�N����::PPW_LobbySetSchedule�֐��ɂ��Z�b�g�����X�P�W���[�����g�p���邩�A�f�t�H���g�̃X�P�W���[�����g�p����B
 */
typedef BOOL (*PPW_LobbyNotifyScheduleCallback)(PPW_LOBBY_STATS_RESULT result, const PPW_LobbySchedule* schedule);

/**
 * @brief �T�[�o����VIP�����擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �T�[�o����VIP�����擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B����VIP���͊m��ł���A����ω����邱�Ƃ͂���܂���B
 * 
 * @param[in] result �ʐM���ʁB::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�������Ƃ��̓��C�u�����̓G���[��ԂɂȂ�܂��B
 *                   ���̂Ƃ���::PPW_LobbyShutdownAsync�֐��Ń��C�u�������I�����Ă��������B
 * @param[in] vipRecords �擾����VIP���̔z��Bresult��::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�̂Ƃ���NULL�ɂȂ�܂��B
 * @param[in] num �擾����VIP���̐��B0�̏ꍇ������܂��Bresult��::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�̂Ƃ���0�ɂȂ�܂��B
 */
typedef void (*PPW_LobbyGetVipCallback)(PPW_LOBBY_STATS_RESULT result, const PPW_LobbyVipRecord* vipRecords, u32 num);

/**
 * @brief �T�[�o����A���P�[�g�����擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �T�[�o����A���P�[�g�����擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] result �ʐM���ʁB::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�������Ƃ��̓��C�u�����̓G���[��ԂɂȂ�܂��B
 *                   ���̂Ƃ���::PPW_LobbyShutdownAsync�֐��Ń��C�u�������I�����Ă��������B
 * @param[in] questionnaire �擾�����A���P�[�g���Bresult��::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�̂Ƃ���NULL�ɂȂ�܂��B
 */
typedef void (*PPW_LobbyGetQuestionnaireCallback)(PPW_LOBBY_STATS_RESULT result, const PPW_LobbyQuestionnaire* questionnaire);

/**
 * @brief �A���P�[�g�̒�o�Ɋ��������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �A���P�[�g�̒�o�Ɋ��������Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * @param[in] result �ʐM���ʁB��o�Ɏ��s�����::PPW_LOBBY_STATS_RESULT_SUCCESS�ȊO�ɂȂ�܂����A���C�u�������̂̓G���[��ԂɂȂ炸���̂܂ܑ��s���\�ł��B
 */
typedef void (*PPW_LobbySubmitQuestionnaireCallback)(PPW_LOBBY_STATS_RESULT result);

/**
 * @brief �T�[�o�Ƃ̒ʐM���ߏ�ɍs�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * 
 * �T�[�o�Ƃ̒ʐM���ߏ�ɍs�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��^�B
 * floodWeight�͒ʐM�𔭐������郍�r�[���C�u�����̊֐��̌Ăяo���ɂ���ď㏸���A
 * 300�𒴂����::PPW_LobbyGetChannelDataAsync�֐������������悤�ɂȂ�A600�𒴂���ƃT�[�o����ؒf����܂��B
 * floodWeight��1�b��5�������܂��B
 * ������PPW_LobbySend*Message�֐��̌Ăяo����floodWeight���㏸�����܂���B
 * ���̃R�[���o�b�N�����������Ƃ��̓`�����l���f�[�^�Z�b�g��擾�A�v���t�B�[���̍X�V�A�`�����l���̏o������T����悤�ɂ��Ă��������B
 * �������͂��̃R�[���o�b�N���������Ȃ����x�ɒʐM����p�x�������Ă��������B
 * 
 * @param[in] floodWeight �ʐM�ߑ��̊����������d�݁B
 */
typedef void (*PPW_LobbyExcessFloodCallback)(u32 floodWeight);

/// �R�[���o�b�N�o�^�p�\����
typedef struct PPW_LobbyCallbacks
{
    /// �������`�����l���ɓ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyJoinChannelCallback lobbyJoinChannelCallback;
    /// �������g�������͑��̃v���C���[�����C���`�����l���ɓ��������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyPlayerConnectedCallback lobbyPlayerConnectedCallback;
    /// ���̃v���C���[�����C���`�����l������ގ������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyPlayerDisconnectedCallback lobbyPlayerDisconnectedCallback;
    /// �����񃁃b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyStringMassageReceivedCallback lobbyStringMassageReceivedCallback;
    /// �o�C�i�����b�Z�[�W����M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyBinaryMassageReceivedCallback lobbyBinaryMassageReceivedCallback;
    /// �`�����l���f�[�^���擾�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyGetChannelDataCallback lobbyGetChannelDataCallback;
    /// ���̃v���C���[�̃V�X�e����`�v���t�B�[�����ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbySystemProfileUpdatedCallback lobbySystemProfileUpdatedCallback;
    /// ���̃v���C���[�̃��[�U��`�v���t�B�[�����ύX���ꂽ�Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyUserProfileUpdatedCallback lobbyUserProfileUpdatedCallback;
    /// ���̃v���C���[���~�j�Q�[�����W���Ă��鎞�ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyRecruitCallback lobbyRecruitCallback;
    /// ���̃v���C���[���~�j�Q�[���̕�W���I���������ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyRecruitStoppedCallback lobbyRecruitStoppedCallback;
    /// �C�x���g�����������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyScheduleProgressCallback lobbyScheduleProgressCallback;
    /// �s���`�F�b�N�̌��ʂ�ʒm����R�[���o�b�N�֐��B
    PPW_LobbyCheckProfileCallback lobbyCheckProfileCallback;
    /// �X�P�W���[������M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyNotifyScheduleCallback lobbyNotifyScheduleCallback;
    /// VIP������M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyGetVipCallback lobbyGetVipCallback;
    /// �A���P�[�g������M�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyGetQuestionnaireCallback lobbyGetQuestionnaireCallback;
    /// �A���P�[�g�̒�o�Ɋ��������Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbySubmitQuestionnaireCallback lobbySubmitQuestionnaireCallback;
    /// �T�[�o�Ƃ̒ʐM���ߏ�ɍs�����Ƃ��ɌĂяo�����R�[���o�b�N�֐��B
    PPW_LobbyExcessFloodCallback lobbyExcessFloodCallback;
}
PPW_LobbyCallbacks;

// ���C�u����������
PPW_LOBBY_RESULT PPW_LobbyInitializeAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                          const PPW_LobbyCallbacks* lobbyCallbacks, const DWCUserData* dwcUserdata, const u8* userProfile, u32 userProfileSize);
// ���C�u����������(�f�o�b�O�p)
PPW_LOBBY_RESULT PPW_LobbyInitializePidAsync(const char* gameName, const char* secretKey, DWC_LOBBY_CHANNEL_PREFIX prefix,
                                        const PPW_LobbyCallbacks* lobbyCallbacks, s32 pid, const u8* userProfile, u32 userProfileSize);

// ���C�u�����I��
PPW_LOBBY_RESULT PPW_LobbyShutdownAsync();

// �v���Z�X�֐�
PPW_LOBBY_STATE PPW_LobbyProcess();

// �G���[���擾
PPW_LOBBY_ERROR PPW_LobbyGetLastError();

// ���������G���[(::PPW_LOBBY_ERROR)����\�����ׂ��G���[�R�[�h�𓾂�B
s32 PPW_LobbyToErrorCode(PPW_LOBBY_ERROR err);

// ���Ճ{�[�h�A���E���v�A���r�[�j���[�X�ɎQ������
PPW_LOBBY_RESULT PPW_LobbyJoinSubChannelAsync(PPW_LOBBY_CHANNEL_KIND channelKind);

// ���Ճ{�[�h�A���E���v�A���r�[�j���[�X����o��
PPW_LOBBY_RESULT PPW_LobbyLeaveSubChannel();

// ���������擾
PPW_LOBBY_RESULT PPW_LobbyGetTimeInfo(PPW_LobbyTimeInfo* timeInfo);

// �X�P�W���[�����Z�b�g����(�f�o�b�O�p)
PPW_LOBBY_RESULT PPW_LobbySetSchedule(const PPW_LobbySchedule* schedule, u32 scheduleSize);

// �X�P�W���[�����擾����
PPW_LOBBY_RESULT PPW_LobbyGetSchedule(PPW_LobbySchedule* schedule, u32* scheduleSize);

// VIP�����擾����
PPW_LOBBY_RESULT PPW_LobbyGetVip(PPW_LobbyVipRecord* records, u32* recordNum);

// �T�u�`�����l���̏�Ԃ��擾
PPW_LOBBY_CHANNEL_STATE PPW_LobbyGetSubChannelState();
    
// �`�����l���f�[�^�ۑ�
PPW_LOBBY_RESULT PPW_LobbySetChannelData(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char* key, const u8* data, u32 dataSize);

// �`�����l���f�[�^�擾
PPW_LOBBY_RESULT PPW_LobbyGetChannelDataAsync(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, const char* key, void* param);

// Ascii������̃��b�Z�[�W���`�����l���ɑ��M
PPW_LOBBY_RESULT PPW_LobbySendChannelStringMessage(PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const char* message);

// Ascii������̃��b�Z�[�W����l�̃v���C���[�ɑ��M
PPW_LOBBY_RESULT PPW_LobbySendPlayerStringMessage(s32 userId, s32 type, const char* message);

// �o�C�i�����b�Z�[�W���`�����l���ɑ��M
PPW_LOBBY_RESULT PPW_LobbySendChannelBinaryMessage(PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const u8* data, u32 dataSize);

// �o�C�i���f�[�^����l�̃v���C���[�ɑ��M
PPW_LOBBY_RESULT PPW_LobbySendPlayerBinaryMessage(s32 userId, s32 type, const u8* data, u32 dataSize);

// ���r�[���C�u�����Ŏg�p���Ă��鎩���̃��[�UID���擾
s32 PPW_LobbyGetMyUserId();

// ���݃��C���`�����l���ɂ��鎩���ȊO�̃��[�UID���
PPW_LOBBY_RESULT PPW_LobbyEnumUserId(s32* userIds, u32* num);

// ���[�U��`�v���t�B�[���Q�ƁBuserId�Ɏ�����GameSpy Profile ID��������PPW_LOBBY_INVALID_USER_ID��n���Ǝ����̃��[�U��`�v���t�B�[�����Q�Ƃł���
PPW_LOBBY_RESULT PPW_LobbyGetProfile(s32 userId, u8* userProfile, u32* userProfileSize);

// �����̃��[�U��`�v���t�B�[�����X�V
PPW_LOBBY_RESULT PPW_LobbyUpdateMyProfile(const u8* userProfile, u32 userProfileSize, BOOL force);

// �~�j�Q�[����W
PPW_LOBBY_RESULT PPW_LobbyStartRecruit(PPW_LobbyRecruitInfo* recruitInfo);

// �~�j�Q�[����W���X�V
PPW_LOBBY_RESULT PPW_LobbyUpdateRecruitInfo(const PPW_LobbyRecruitInfo* recruitInfo);

// �~�j�Q�[����W�I��
PPW_LOBBY_RESULT PPW_LobbyStopRecruit();

// �A���P�[�g���o����
PPW_LOBBY_RESULT PPW_LobbySubmitQuestionnaire(s32 answerNo);

// DWC�̓�����Ԏ擾
// �w�肵���l�Ɛڑ����Ă��邩
BOOL PPW_IsConnectedUserId(const DWCFriendsMatchControl* dwcFriendMatchCtrl, s32 userId);

// �f�o�b�O�p�ɔC�ӂ�EncInitData���w��
void PPW_LobbySetEncInitData(const char* initData);

#ifdef WIN32
#pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // PPW_LOBBY_H_
