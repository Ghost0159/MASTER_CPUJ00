#ifndef __MAIN_H__
#define __MAIN_H__

// [nakata] GameSpy SDK�Ƀv���b�g�t�H�[���������邽�߂̒萔
#ifndef _NITRO
#define _NITRO
#endif

#ifndef GSI_COMMON_DEBUG
#define GSI_COMMON_DEBUG
#endif


#ifdef __cplusplus
extern "C" {
#endif
    

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_MAX_SAVE_FILE   4   // �Q�[�����̃Z�[�u�t�@�C�����i���v���C���[���j
#define GAME_MAX_FRIEND_LIST 32  // �F�B�o�^�ő吔
#define GAME_MAX_PLAYER_NAME 16  // �Q�[�����v���C���[�l�[����
    

// �Q�[�����[�h�񋓎q
typedef enum
{
    GAME_MODE_MAIN = 0,    // �I�t���C���̃��C���Q�[��
    GAME_MODE_REG_FRIEND,  // �F�B�o�^�E����
    GAME_MODE_LOGIN,       // ���O�C���\��
    GAME_MODE_CONNECTED,   // �}�b�`���C�N�I����
    GAME_MODE_NETCONFIG_LANGUAGE,   // Wi-Fi�R�l�N�V�����ݒ�̎g�p����I��
    GAME_MODE_NETCONFIG,   // Wi-Fi�R�l�N�V�����ݒ�
    GAME_MODE_STARTIP,     // IP�擾
    GAME_MODE_STOPIP,      // IP�J��
    GAME_MODE_BACKUP,      // �o�b�N�A�b�v�֘A����
    GAME_MODE_NUM
} GameMode;


//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
// �Q�[���V�[�P���X�֐��^
// �߂�l�͎��ɐi�ރQ�[�����[�h
typedef GameMode (*GameSeqFunc)(void);

// ���[�h���Ƃɒǉ��\�����s�����߂̃R�[���o�b�N�^
typedef void (*GameAddedDispCallback)(int curIdx, void* param);


typedef struct tagSavedPlayerData
{
    char playerName[GAME_MAX_PLAYER_NAME];  // �Q�[�����v���C���[��
} SavedPlayerData;

// �Q�[���Ŏg���f�[�^��t�������F�B���X�g
typedef struct tagGameFriendList
{
    DWCFriendData keyList[GAME_MAX_FRIEND_LIST];  // DWC�`���̗F�B���X�g
    char playerName[GAME_MAX_FRIEND_LIST][GAME_MAX_PLAYER_NAME];  // �v���C���[��
} GameFriendList;

// �Q�[���Z�[�u�f�[�^�\����
typedef struct tagGameSaveData
{
    SavedPlayerData playerData[GAME_MAX_SAVE_FILE];
    int  profileID;        // ���[�U�ŗL�̃v���t�@�C��ID
    BOOL isValidProfile;   // �v���t�@�C��ID���L�����ǂ���
    GameFriendList friendList;  // �Q�[���Ŏg���f�[�^��t�������F�B���X�g
} GameSaveData;


// �Q�[���V�[�P���X���䃊�X�g�v�f�\����
typedef struct tagGameMenuElement
{
    const char* msg;  // �Ή����郁�b�Z�[�W
    GameMode mode;    // �Ή�����Q�[�����[�h�B���[�h���ς��Ȃ�����GAME_MODE_NUM���w��
    BOOL blocking;    // �I�����ɏ�ԕω������܂ő���s�\�ɂ���K�v�����邩
} GameMenuElement;

// �Q�[���V�[�P���X���䃊�X�g�\����
typedef struct tagGameSequence
{
    const char* modeName;  // ���[�h��
    GameSeqFunc seqFunc;   // �Ή�����֐�
    GameAddedDispCallback dispCallback;  // �Ή�����ǉ��\���R�[���o�b�N
    int numMenu;           // ���j���[�̐�
    GameMenuElement menuList[8];  // ���j���[���X�g
} GameSequence;


typedef struct tagGameUserData
{
    int  playerIdx;       // �Q�[�����v���C���[�C���f�b�N�X
    char playerName[GAME_MAX_PLAYER_NAME];  // ���݃v���C���̃Q�[�����v���C���[��
    int  profileID;       // ���[�U�ŗL�̃v���t�@�C��ID
    BOOL isValidProfile;  // �v���t�@�C��ID���L�����ǂ���
} GameUserData;

// �Q�[��������\����
typedef struct tagGameControl
{
    GameUserData userData;  // �v���C���̃��[�U�f�[�^
    GameMode mode;          // ���݂̃Q�[�����[�h
    BOOL blocking;          // TRUE�Ȃ瑀��s�\�i�l�b�g���[�N��ԕω��҂��j���
    const char* essID;      // �g�p����AP��ESSID
    GameFriendList friendList;  // �Q�[���Ŏg���f�[�^��t�������F�B���X�g
    u8  friendStatus[GAME_MAX_FRIEND_LIST];  // �F�B�̒ʐM��ԁB�C���f�b�N�X�͗F�B���X�g�̂��̂ƑΉ�
} GameControl;

// �F�B���w��s�A�}�b�`���C�N�p�ǉ��L�[�f�[�^�\����
typedef struct tagGameMatchExtKeys
{
    u8  keyID;        // �}�b�`���C�N�p�L�[ID
    u8  isStr;        // 0:�l��int�^�A1:�l��������
    u16 pad;          // �p�f�B���O
    char keyStr[16];  // �}�b�`���C�N�p�L�[������
    char svalue[16];  // �L�[�ɑΉ�����l�i������j
    int ivalue;       // �L�[�ɑΉ�����l�iint�^�j
} GameMatchExtKeys;


//----------------------------------------------------------------------------
// function
//----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif

#endif  // __MAIN_H__
