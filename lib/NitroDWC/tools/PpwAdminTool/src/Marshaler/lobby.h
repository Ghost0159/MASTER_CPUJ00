
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
    PPW_LOBBY_ERROR_STATS_SESSION,          ///< �v���I�ȒʐM�G���[(���[���ݒ�T�[�o)�B
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


typedef enum LANGUAGE
{
	LANGUAGE_NONE,
	LANGUAGE_JAPANESE,
	LANGUAGE_US,
	LANGUAGE_FR,
	LANGUAGE_IT,
	LANGUAGE_DE,
	LANGUAGE_SP,
	LANGUAGE_RESERVE,
	LANGUAGE_KR,
	LANGUAGE_MAX
}
LANGUAGE;

/// �������
typedef struct PPW_LobbyQuestionCondition
{
    s32 questionNo;
    u32 multiLanguageSummarizeId;
}
PPW_LobbyQuestionCondition;

/// �X�y�V�����E�B�[�N���R�[�h
typedef struct PPW_LobbySpecialWeekRecord
{
    s32 questionSerialNo;
    PPW_LobbyQuestionCondition questionConditions[LANGUAGE_MAX];
}
PPW_LobbySpecialWeekRecord;

/// �C�ӎ���
typedef struct PPW_LobbyFreeQuestion
{
    s32 language;                       ///< �o�^���錾��
    s32 questionNo;                         ///< �X���b�g�ԍ�(����ԍ�-1000)
    u16 questionSentence[PPW_LOBBY_MAX_QUESTION_SENTENCE_LENGTH];   ///< �C�ӎ���B
    u16 answer[PPW_LOBBY_QUESTIONNAIRE_ANSWER_NUM][PPW_LOBBY_MAX_ANSWER_LENGTH];    ///< �C�ӎ���̉񓚁B
}
PPW_LobbyFreeQuestion;