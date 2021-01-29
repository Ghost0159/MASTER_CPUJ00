#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"

#include "dbs.h"



//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_NAME        "dwctest" // ���̃T���v�����g�p����Q�[����
#define GAME_SECRET_KEY  "d4q9GZ"  // ���̃T���v�����g�p����V�[�N���b�g�L�[
#define GAME_PRODUCTID   10824      // ���̃T���v�����g�p����v���_�N�gID

#define GAME_FRAME       1         // �z�肷��Q�[���t���[���i1/60��1�Ƃ���j

#define GAME_NUM_MATCH_KEYS 3      // �}�b�`���C�N�p�ǉ��L�[��

#define NETCONFIG_USE_HEAP 1

// DWC_MATCH_OPTION_SC_CONNECT_BLOCK�}�b�`���C�N�I�v�V�����������X�C�b�`
//#define GAME_MATCH_OP_SC_CONNECT_BLOCK

// �X�g���[�W�T�[�o�ւ̃Z�[�u�E���[�h�������X�C�b�`
//#define GAME_USE_STORAGE_SERVER

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
// �}�b�`���C�N�P��������A���̃}�b�`���C�N���󂯕t����܂ł̎��ԁi�P�ʁF�b�j
#define GAME_MATCH_OP_SC_BLOCK_TIME 10
#endif

// �F�؃T�[�o���f�t�H���g�̊J�������F�؃T�[�o���琻�i�����F�؃T�[�o�ɕύX����
// �ꍇ�́A���L�̒�`��L���ɂ��Ă��������B
//#define USE_AUTHSERVER_RELEASE 1

// �������m��/�J�����Ƀ������𖄂߂�f�o�b�O�I�v�V����
#undef DEBUG_OPTION_FILL_HEAP

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
typedef struct tagRegFriendParam
{
    int step;    // 0:�R�}���h�I�𒆁A1:�o�^�E�폜�F�B�����͒�
    u32 figure;  // ���͒��̃v���t�@�C��ID�̌�
    u32 value;   // ���͒��̒l
} RegFriendParam;


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static GameControl stGameCnt;  // �Q�[��������\����
static KeyControl stKeyCnt;    // �L�[���͐���\����
static DWCFriendsMatchControl stDwcCnt;    // FriendsMatch����\����

static int stNumEntry  = 2;    // �l�b�g���[�N�\���l���w��
static int stServerIdx = 0;    // �ڑ��������T�[�oDS�̗F�B���X�g�C���f�b�N�X

static BOOL stCloseFlag = FALSE;    // �����ŃN���[�Y�����ꍇ��TRUE

// �F�B���w��s�A�}�b�`���C�N�p�ǉ��L�[�f�[�^�\����
static GameMatchExtKeys stMatchKeys[GAME_NUM_MATCH_KEYS] = { 0, };

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
static OSTick stMatchFinishTime = 0;  // �T�[�o�N���C�A���g�}�b�`���C�N�Ń}�b�`���C�N���I���������
#endif

// ���[�U�f�[�^���i�[����\���́B
static DWCUserData stUserData;
static DWCInetControl stConnCtrl;

// �f�o�b�O�o�͂̃I�[�o�[���C�h
static BOOL sPrintOverride;

static u8 stNumBackup;
static u16 card_lock_id;

static u32 loopcnt = 0;
static char loopstr[] = "/|\\-";

// Wi-Fi�R�l�N�V�����ݒ�̎g�p����
static int utilityLangage = DWC_LANGUAGE_JAPANESE;

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void InitGameData(void);
static void SetGameMatchKeys(void);

static GameMode GameMain(void);
static GameMode GameRegisterFriendMain(void);
static GameMode GameLogonMain(void);
static GameMode GameMatchMain(void);
static GameMode GameConnectedMain(void);
static GameMode NetConfigLanguageMain(void);
static GameMode NetConfigMain(void);
static GameMode StartIPMain(void);
static GameMode StopIPMain(void);
static GameMode BackupUserdataMain(void);

static int GetAvailableFriendListIndex(void);

static void LoginCallback(DWCError error, int profileID, void* param);

static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param);
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param);
static void DeleteFriendListCallback(int deletedIndex, int srcIndex, void* param);
static void BuddyFriendCallback(int index, void* param);

static void LoginToStorageCallback(DWCError error, void* param);
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param);
static void LoadFromServerCallback(BOOL success, int index, char* data, int len, void* param);

static void ConnectToAnybodyCallback(DWCError error, BOOL cancel, void* param);
static void ConnectToFriendsCallback(DWCError error, BOOL cancel, void* param);
static void SetupGameServerCallback(DWCError error,
                                    BOOL cancel,
                                    BOOL self,
                                    BOOL isServer,
                                    int index,
                                    void* param);
static void ConnectToGameServerCallback(DWCError error,
                                        BOOL cancel,
                                        BOOL self,
                                        BOOL isServer,
                                        int index,
                                        void* param);
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8 aid,
                                     int index,
                                     void* param);
static void NewClientCallback(int index, void* param);
static int  EvaluateAnybodyCallback(int index, void* param);
static int  EvaluateFriendCallback(int index, void* param);
#ifdef DWC_STOP_SC_SERVER
static void StopSCMatchingCallback(void* param);
#endif

static DWCError HandleDWCError(GameMode* gameMode);

static void DispErrorMessage(DWCError error, int errorCode, DWCErrorType errorType);
static void DispMenuMsgWithCursor(const GameSequence* gameSeq, int index, void* param);
static void DispFriendList(BOOL overLoad);
static void DispFriendListData(void);

static void RegFriendModeDispCallback(int curIdx, void* param);
static void LogonModeDispCallback(int curIdx, void* param);
static void MatchModeDispCallback(int curIdx, void* param);
static void BackupModeDispCallback(int curIdx, void* param);

static void GameWaitVBlankIntr(void);
static void VBlankIntr(void);


//----------------------------------------------------------------------------
// initialized variable
//----------------------------------------------------------------------------
// �Q�[���V�[�P���X���X�g
static GameSequence gameSeqList[GAME_MODE_NUM] =
{
    {
        "GAME MAIN MODE",
        GameMain,
        NULL,
        6,
        {
            "Login", GAME_MODE_LOGIN, TRUE,
            "Register friend", GAME_MODE_REG_FRIEND, FALSE,
            "NetConfig", GAME_MODE_NETCONFIG_LANGUAGE, FALSE,
            "StartIP", GAME_MODE_STARTIP, FALSE,
            "StopIP", GAME_MODE_STOPIP, FALSE,
            "UserData Backup", GAME_MODE_BACKUP, FALSE,
        },
    },
    {
        "REGISTER FRIEND MODE",
        GameRegisterFriendMain,
        RegFriendModeDispCallback,
        3,
        {
            "Set", GAME_MODE_NUM, FALSE,
            "Delete", GAME_MODE_NUM, FALSE,
            "Return", GAME_MODE_MAIN, FALSE,
        },
    },
    {
        "GAME LOGON MODE",
        GameLogonMain,
        LogonModeDispCallback,
        5,
        {
            "Connect to anybody", GAME_MODE_CONNECTED, TRUE,
            "Connect to friends", GAME_MODE_CONNECTED, TRUE,
            "Setup game server", GAME_MODE_CONNECTED, TRUE,
            "Connect to game server", GAME_MODE_CONNECTED, TRUE,
            "Logout", GAME_MODE_MAIN, FALSE
        },
    },
    {
        "GAME CONNECTED MODE",
        GameConnectedMain,
        NULL,
        3,
        {
            "DWC_SendReliable", GAME_MODE_NUM, FALSE,
            "DWC_SendUnreliable", GAME_MODE_NUM, FALSE,            
            "Close connections", GAME_MODE_LOGIN, FALSE
        },
    },
    {
        "LANGUAGE",
        NetConfigLanguageMain,
        NULL,
        8,
        {
            "Japanese", GAME_MODE_NETCONFIG, FALSE,
            "English", GAME_MODE_NETCONFIG, FALSE,
            "French", GAME_MODE_NETCONFIG, FALSE,
            "German", GAME_MODE_NETCONFIG, FALSE,
            "Italian", GAME_MODE_NETCONFIG, FALSE,
            "Spanish", GAME_MODE_NETCONFIG, FALSE,
            "Hangul", GAME_MODE_NETCONFIG, FALSE,
            "Return", GAME_MODE_MAIN, FALSE
        },
    },
    {
        "NET CONFIG GUI",
        NetConfigMain,        NULL,        0,
    },
    {
        "START TO CONNECT INTERNET",
        StartIPMain,        NULL,        0,
    },
    {
        "STOP TO CONNECT INTERNET",
        StopIPMain,        NULL,        0,
    },
    {
        "BACKUP USERDATA",
        BackupUserdataMain,
        BackupModeDispCallback,
        4,
        {
            "Load UserData", GAME_MODE_NUM, FALSE,
            "Save UserData", GAME_MODE_NUM, FALSE,
            "Report UserData", GAME_MODE_NUM, FALSE,
            "return", GAME_MODE_MAIN, FALSE,
        }
    },
};


// �T���v���ł̓Z�[�u�ł��Ȃ��̂ł�����Z�[�u�f�[�^�̑���Ƃ݂Ȃ�
static GameSaveData saveData =
{
    {
        "Player1", "Player2", "Player3", "Player4"
    },
    0, FALSE,
    {
        0,
    }
};

// GameSpy Status�f�[�^�Ƃ��ăZ�b�g����T���v���f�[�^
static const char gsStatusSample[16] = { 10, 9, 8, 0, 0, 1, 2, 3, };

static const struct
{
    CARDBackupType type;
    const char *comment;
} stBackupTypeTable[] = {
    { CARD_BACKUP_TYPE_EEPROM_4KBITS,   "EEPROM    4 kb" },
    { CARD_BACKUP_TYPE_EEPROM_64KBITS,  "EEPROM   64 kb" },
    { CARD_BACKUP_TYPE_EEPROM_512KBITS, "EEPROM  512 kb" },
    { CARD_BACKUP_TYPE_FLASH_2MBITS,    "FLASH     2 Mb" },
    { CARD_BACKUP_TYPE_FRAM_256KBITS,   "FRAM    256 kb" },
} ;
enum { BACKUP_TABLE_MAX = sizeof(stBackupTypeTable) / sizeof(*stBackupTypeTable) } ;


/*---------------------------------------------------------------------------*
  �������m�ۊ֐�
 *---------------------------------------------------------------------------*/
static void*
AllocFunc( DWCAllocType name, u32   size, int align )
{
    void * ptr;
    OSIntrMode old;
    (void)name;
    (void)align;

    old = OS_DisableInterrupts();

    ptr = OS_AllocFromMain( size );

#ifdef DEBUG_OPTION_FILL_HEAP
    MI_CpuFill8( ptr, 0xac, size );
#endif

    //OS_TPrintf( "alloc: ptr=0x%08X size=%d\n", ptr, size );

    OS_RestoreInterrupts( old );

    return ptr;
}

/*---------------------------------------------------------------------------*
  �������J���֐�
 *---------------------------------------------------------------------------*/
static void
FreeFunc( DWCAllocType name, void* ptr,  u32 size  )
{
    OSIntrMode old;
    (void)name;
    (void)size;

    if ( !ptr ) return;

    old = OS_DisableInterrupts();

#ifdef DEBUG_OPTION_FILL_HEAP
    MI_CpuFill8( ptr, 0xfe, size );
#endif

    OS_FreeToMain( ptr );

    //OS_TPrintf( "free : ptr=0x%08X size=%d\n", ptr, size );

    OS_RestoreInterrupts( old );
}

/** --------------------------------------------------------------------
  OS_TPrintf()�̃R���\�[���o�͑Ή�
  ----------------------------------------------------------------------*/
#ifdef SDK_FINALROM
#undef OS_TVPrintf
#undef OS_TPrintf
#endif
extern void OS_TVPrintf(const char *fmt, va_list vlist );
static char common_buffer[256];        // thread unsafe, but less use of stack

void OS_TVPrintf(const char *fmt, va_list vlist )
{
    (void)OS_VSNPrintf(common_buffer, sizeof(common_buffer), fmt, vlist);
#ifndef SDK_FINALROM
    OS_PutString(common_buffer);
#endif

    if ( sPrintOverride )
    {
        dbs_CVPrintf( NULL, fmt, vlist );
    }
}
#ifdef SDK_FINALROM
extern void OS_TPrintf(const char *fmt, ...);
void OS_TPrintf(const char *fmt, ...)
{
    va_list vlist;

    va_start(vlist, fmt);
    OS_TVPrintf(fmt, vlist);
    va_end(vlist);
}
#endif


/*---------------------------------------------------------------------------*
  ���C�����[�`��
 *---------------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
void NitroMain ()
{
    int friendIdx = 0;
    int ret;

	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    CARD_Init();
    ret = OS_GetLockID();
    if (ret == OS_LOCK_ID_ERROR)
    {
        OS_Panic("demo fatal error! OS_GetLockID() failed");
    }
    card_lock_id = (u16)ret;

    // �X�^�b�N���`�F�b�N�����ݒ�
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    
    PXI_Init();
    
    Heap_Init();
	Alarm_Init();
//    Heap_SetDebug( TRUE );

    GX_Init();
    FS_Init( MI_DMA_MAX_NUM ); 

    GX_DispOff();
    GXS_DispOff();

    // V�u�����N���荞�݋���
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);
	GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    // �o�͏�����
    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()�̏o�͂�console�ɂȂ�.

    // �\���J�n
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // �L�[���͏�����
    InitKeyData(&stKeyCnt);

    InitGameData();

    // �f�o�b�O�p�ɃR���\�[���̏����폜����B
    //DWC_Debug_DWCInitError( s_Work, DWC_INIT_RESULT_DESTROY_OTHER_SETTING );

    // �f�o�b�O�\�����x���w��
    DWC_SetReportLevel((unsigned long)(DWC_REPORTFLAG_ALL & ~DWC_REPORTFLAG_QR2_REQ));
	gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_Verbose);

    // DWC���C�u����������
    ret = DWC_Init( s_Work );
    OS_TPrintf( "DWC_Init() result = %d\n", ret );

    if ( ret == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        OS_TPrintf( "Wi-Fi setting might be broken.\n" );
    }
    
    // ���[�U�f�[�^�쐬
    DWC_CreateUserData( &stUserData, 'ABCD' );

#if 0
    OS_TPrintf("+++VALID CONSOLE %d +++", DWC_CheckValidConsole( &stUserData ) );
    {
        char test[16];
        sprintf(test,"%u",0xa1546344);

        OS_PutString(test);
    }
#endif

    // ���[�U�f�[�^�\��
    DWC_ReportUserData( &stUserData );

    // �q�[�v�g�p�ʕ\��ON
    //Heap_SetDebug(TRUE);

    // �������m�ۊ֐��ݒ�
    DWC_SetMemFunc( AllocFunc, FreeFunc );
        
    // ���C�����[�v
    while (1){
        // ���j���[�\��
        DispMenuMsgWithCursor(&gameSeqList[stGameCnt.mode], 0, NULL);

        // �V�[�P���X�ɑΉ����郋�[�v�֐����Ăяo��
        stGameCnt.mode = gameSeqList[stGameCnt.mode].seqFunc();
    }

	OS_Terminate();
}


/*---------------------------------------------------------------------------*
  �Q�[���f�[�^�ƃz�X�g��񏉊����֐�
 *---------------------------------------------------------------------------*/
static void InitGameData(void)
{

    // �Q�[������\���̏�����
    stGameCnt.mode     = GAME_MODE_MAIN;
    stGameCnt.blocking = FALSE;
    MI_CpuClear8(stGameCnt.friendStatus, sizeof(stGameCnt.friendStatus));

    // �Z�[�u�f�[�^����v���C���[�f�[�^�����[�h�����Ƒz��B
    // �Ƃ肠�����ǂ̖{�̂ł��C���f�b�N�X0�̃f�[�^���g���B
    stGameCnt.userData.playerIdx = 0;
    MI_CpuCopy32(saveData.playerData[stGameCnt.userData.playerIdx].playerName,
                 stGameCnt.userData.playerName, sizeof(stGameCnt.userData.playerName));
    stGameCnt.userData.profileID = saveData.profileID;
    stGameCnt.userData.isValidProfile = saveData.isValidProfile;
    MI_CpuCopy32(&saveData.friendList, &stGameCnt.friendList, sizeof(GameFriendList));
}


/*---------------------------------------------------------------------------*
  �Q�[����`�̃}�b�`���C�N�p�ǉ��L�[�Z�b�g�֐�
 *---------------------------------------------------------------------------*/
static void SetGameMatchKeys(void)
{
    int ranking = (int)((OS_GetTick()*1592653589UL+453816691UL) & 0xff);

    // �����L���O�𐮐��L�[�Ƃ��Đݒ�
    stMatchKeys[0].isStr  = 0;
    strcpy(stMatchKeys[0].keyStr, "game_rank");
    stMatchKeys[0].ivalue = ranking;
    stMatchKeys[0].keyID  =
        DWC_AddMatchKeyInt(stMatchKeys[0].keyID,
                           stMatchKeys[0].keyStr,
                           &stMatchKeys[0].ivalue);
    if (stMatchKeys[0].keyID == 0) OS_TPrintf("AddMatchKey failed 0.\n");

    // �}�b�`���C�N�ɗp����]���^�C�v�𕶎���L�[�Ƃ��Đݒ�
    stMatchKeys[1].isStr  = 1;
    strcpy(stMatchKeys[1].keyStr, "game_mtype");
    strcpy(stMatchKeys[1].svalue, "eval_rank");
    stMatchKeys[1].keyID  =
        DWC_AddMatchKeyString(stMatchKeys[1].keyID,
                              stMatchKeys[1].keyStr,
                              stMatchKeys[1].svalue);
    if (stMatchKeys[1].keyID == 0) OS_TPrintf("AddMatchKey failed 1.\n");

    // �Ƃ肠������������炢�����L�[��ݒ�
    stMatchKeys[2].isStr  = 0;
    strcpy(stMatchKeys[2].keyStr, "game_manner");
    stMatchKeys[2].ivalue = ranking%11;
    stMatchKeys[2].keyID  =
        DWC_AddMatchKeyInt(stMatchKeys[2].keyID,
                           stMatchKeys[2].keyStr,
                           &stMatchKeys[2].ivalue);
    if (stMatchKeys[2].keyID == 0) OS_TPrintf("AddMatchKey failed 2.\n");
}


/*---------------------------------------------------------------------------*
  �I�t���C�������C���֐�
 *---------------------------------------------------------------------------*/
static GameMode GameMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_MAIN];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

#if 0
// �T���v����MP�����p�F�B���\���e�X�g
{
    DWCFriendData friendData;
    int i;
    
    DWC_CreateExchangeToken(&stUserData, &friendData);
    for (i = 0; i < 12; i++){
        OS_TPrintf("0x%02x, ", ((u8 *)&friendData)[i]);
    }
    OS_TPrintf("\n");
}
#endif


    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;
		ReadKeyData();  // �L�[�f�[�^�擾

        DWC_ProcessFriendsMatch();  // FriendsMatch�ʐM�����X�V

        HandleDWCError(&returnSeq);  // �G���[����

        if (stGameCnt.blocking){
            // DWC�������̓L�[������֎~����
            // V�u�����N�҂�����
            GameWaitVBlankIntr();
            continue;
        }

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă�����F�؊���
        if (returnSeq == GAME_MODE_LOGIN){
            // ���O�C�����I������Ă����ꍇ
            if (stGameCnt.userData.isValidProfile){
                // �F�ؐ����Ńv���t�@�C��ID���擾�ł����ꍇ�̓��[�v�𔲂���
                break;
            }
            else {
                // �F�؎��s�̏ꍇ�̓��[�h��i�߂Ȃ��悤�ɂ���
                returnSeq = GAME_MODE_NUM;
            }
        }
        else if (returnSeq != GAME_MODE_NUM){
            // ����ȊO�̏ꍇ�͕K��������
            break;
        }
            
        ////////// �ȉ��L�[���쏈��
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // A�{�^���Ń��j���[����
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (returnSeq){
            case GAME_MODE_LOGIN:  // ���O�C��

                // FriendsMatch���C�u����������
                // ���C�u�������g���\�P�b�g�̑���M�o�b�t�@�T�C�Y��
                // �f�t�H���g�̂܂܁i8KByte�j�ɂ��Ă���
                DWC_InitFriendsMatch(&stDwcCnt, &stUserData, GAME_PRODUCTID, GAME_NAME,
                                     GAME_SECRET_KEY, 0, 0,
                                     stGameCnt.friendList.keyList, GAME_MAX_FRIEND_LIST);

                // �F�ؗp�֐����g���ă��O�C��
                if (!DWC_LoginAsync(L"_INGAMESN_", NULL, LoginCallback, &returnSeq)){
                    // ���O�C�����J�n�ł��Ȃ������ꍇ�̓u���b�L���O��Ԃɂ��Ȃ��B
                    // �G���[����������d���O�C���łȂ����FALSE�ɂ͂Ȃ�Ȃ��B
                    OS_TPrintf("Can't call DWC_LoginAsync().\n");
                    returnSeq          = GAME_MODE_NUM;
                    stGameCnt.blocking = 0;
                }
                break;
            default:
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // �\���L�[��ŃJ�[�\���ړ�
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // �\���L�[���ŃJ�[�\���ړ�
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if ( stKeyCnt.trg & PAD_BUTTON_Y){
            Heap_Dump();
        }
        ////////// �L�[���쏈�������܂�

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  �F�B�o�^���C���֐�
 *---------------------------------------------------------------------------*/
static GameMode GameRegisterFriendMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_REG_FRIEND];
    GameMode returnSeq = GAME_MODE_NUM;
    RegFriendParam cntParam;
    int curIdx = 0;
    int friendIdx;
    int maxFigure;
    int validFlg;

    cntParam.step   = 0;
    cntParam.figure = 0;

    while (1){
        ReadKeyData();  // �L�[�f�[�^�擾

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă����甲����
        if (returnSeq != GAME_MODE_NUM){
            break;
        }
            
        ////////// �ȉ��L�[���쏈��
        if (cntParam.step == 0){
            // �R�}���h�I��
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // A�{�^���Ń��j���[����
                returnSeq = gameSeq->menuList[curIdx].mode;
                stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

                if ((curIdx == 0) || (curIdx == 1)){
                    // �F�B�̃v���t�@�C��ID�o�^�i���͐��̃v���t�@�C��ID�j�A
                    // �������̓C���f�b�N�X���w�肵�ėF�B�����폜
                    cntParam.step   = 1;
                    cntParam.figure = 0;
                    cntParam.value  = 0;
                    
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);

                    if (curIdx == 0){
                        maxFigure = 10;  // �v���t�@�C��ID�͍ō��P�O��
                    }
                    else {
                        maxFigure = 2;   // �F�B���X�g�C���f�b�N�X�͍ō��Q��
                    }
                }
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // �\���L�[��ŃJ�[�\���ړ�
                curIdx--;
                if (curIdx < 0) curIdx = gameSeq->numMenu-1;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // �\���L�[���ŃJ�[�\���ړ�
                curIdx++;
                if (curIdx >= gameSeq->numMenu) curIdx = 0;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        else {
            // ���l���͒�
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // A�{�^���ŗF�B�̓o�^�E�폜���s��
                if (curIdx == 0){
                    // �F�B�o�^
                    if (cntParam.value && DWC_CanChangeFriendList()){
                        friendIdx = GetAvailableFriendListIndex();
                        if (friendIdx == -1){
                            OS_TPrintf("Failed to register new friend. Friend list is full.\n");
                        }
                        else {
                            DWC_SetGsProfileId(&stGameCnt.friendList.keyList[friendIdx],
                                               (int)cntParam.value);
                        }
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // �v���t�@�C��ID=0�͖���
                    }
                }
                else {
                    // �F�B�폜
                    if ((cntParam.value < GAME_MAX_FRIEND_LIST) &&
                        DWC_CanChangeFriendList()){
                        // �F�B���X�g����F�B���폜
                        DWC_DeleteBuddyFriendData(&stGameCnt.friendList.keyList[cntParam.value]);
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // �F�B���X�g�͈͊O
                    }
                }

                if (validFlg){
                    // �L���ȓ��͂ł���΃R�}���h�I���ɖ߂�
                    cntParam.step = 0;
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
            }
            else if (stKeyCnt.trg & PAD_BUTTON_B){
                // B�{�^���ŃR�}���h�I���ɖ߂�
                cntParam.step = 0;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // �\���L�[�㉺�őI�𒆂̐��l�̑����i2^32�z���̔���͂Ȃ��j
                if ((cntParam.value/UtilPow(10, cntParam.figure))%10 == 9){
                    cntParam.value -= UtilPow(10,  cntParam.figure)*9;
                }
                else {
                    cntParam.value += UtilPow(10, cntParam.figure);
                }
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // �\���L�[�㉺�őI�𒆂̐��l�̑����i2^32�z���̔���͂Ȃ��j
                if ((cntParam.value/UtilPow(10, cntParam.figure))%10 == 0){
                    cntParam.value += UtilPow(10, cntParam.figure)*9;
                }
                else {
                    cntParam.value -= UtilPow(10, cntParam.figure);
                }
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_LEFT){
                // �\���L�[���E�őI�������̈ړ�
                if (cntParam.figure < maxFigure-1) cntParam.figure++;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_RIGHT){
                // �\���L�[���E�őI�������̈ړ�
                if (cntParam.figure > 0) cntParam.figure--;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        ////////// �L�[���쏈�������܂�

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  ���O�C���チ�C���֐�
 *---------------------------------------------------------------------------*/
static GameMode GameLogonMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_LOGIN];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;
    BOOL boolRet;

    stNumEntry  = 2;
    stServerIdx = 0;

#if 0
// �T���v���̗F�B�o�^���\���e�X�g
{
    char tmpstr[DWC_FRIENDKEY_STRING_BUFSIZE];
    
    DWC_FriendKeyToString(tmpstr, DWC_CreateFriendKey(&stUserData));
    OS_TPrintf("friendKeyToken '%s'\n", tmpstr);
}
#endif

#ifdef GAME_USE_STORAGE_SERVER
// GameSpy�X�g���[�W�T�[�o�ւ̃Z�[�u�E���[�h���s�Ȃ��e�X�g�̂��߂̃��O�C������
{
    if (!DWC_LoginToStorageServerAsync(LoginToStorageCallback, NULL)){
        OS_TPrintf("DWC_LoginToStorageServerAsync() failed.\n");
    }
}
#endif

#if 0
// �w��l���ȉ��ł̃}�b�`���C�N�����I�v�V�����ݒ�e�X�g
{
    DWCMatchOptMinComplete optval;

    optval.valid    = 1;
    optval.minEntry = 2;
    optval.timeout  = 30000;

    if (DWC_SetMatchingOption(DWC_MATCH_OPTION_MIN_COMPLETE, &optval, sizeof(optval))){
        OS_TPrintf("Failed to set matching option\n");
    }
}
#endif

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
// �ڑ�������A�V�K�ڑ����u���b�N����I�v�V�����ݒ�e�X�g
{
    BOOL boolval = TRUE;

    DWC_SetMatchingOption(DWC_MATCH_OPTION_SC_CONNECT_BLOCK, &boolval, sizeof(boolval));

    stMatchFinishTime = 0;  // �}�b�`���C�N�I���㎞�ԏ�����
}
#endif

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;

        ReadKeyData();  // �L�[�f�[�^�擾

        DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

        HandleDWCError(&returnSeq);  // �G���[����

        if (stGameCnt.blocking){
            // DWC�������̓L�[������֎~����i�L�����Z���̂݉j
            if (stKeyCnt.trg & PAD_BUTTON_B){
                // B�{�^���Ń}�b�`���C�N���L�����Z������
                boolRet = DWC_CancelMatching();
                if (!boolRet){
                    OS_TPrintf("Now unable to cancel matching.\n");
                }
            }

            // V�u�����N�҂�����
            GameWaitVBlankIntr();
            continue;
        }

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă����烋�[�v�𔲂���
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// �ȉ��L�[���쏈��
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // A�{�^���Ń��j���[����
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;
            boolRet   = TRUE;

            switch (curIdx){
            case 0:  // �F�B���w��s�A�}�b�`���C�N�Ăяo��
                if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                    // X�{�^����������Ă��Ȃ���΁A�}�b�`���C�N�p�ǉ��L�[�f�[�^�ݒ�
                    SetGameMatchKeys();
                }
                
                boolRet =
                    DWC_ConnectToAnybodyAsync((u8)stNumEntry,
                                              "game_mtype = 'eval_rank'",
                                              ConnectToAnybodyCallback,
                                              &returnSeq,
                                              EvaluateAnybodyCallback,
                                              NULL);
                break;
            case 1:  // �F�B�w��s�A�}�b�`���C�N�Ăяo��
                if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                    // X�{�^����������Ă��Ȃ���΁A�}�b�`���C�N�p�ǉ��L�[�f�[�^�ݒ�
                    SetGameMatchKeys();
                }

                // ����friendIdxList��NULL���w�肷���
                // �F�B���X�g���̑S�����猟������
                boolRet = 
                    DWC_ConnectToFriendsAsync(NULL,
                                              0,
                                              (u8)stNumEntry,
                                              TRUE,//FALSE,
                                              ConnectToFriendsCallback,
                                              &returnSeq,
                                              EvaluateFriendCallback,
                                              NULL);
                break;
            case 2:  // �T�[�oDS�Ƃ��ăT�[�o�N���C�A���g�}�b�`���C�N�J�n
                boolRet =
                    DWC_SetupGameServer((u8)stNumEntry,
                                        SetupGameServerCallback,
                                        &returnSeq,
                                        NewClientCallback,
                                        NULL);
                break;
            case 3:  // �N���C�A���gDS�Ƃ��ăT�[�o�N���C�A���g�}�b�`���C�N�J�n
                boolRet =
                    DWC_ConnectToGameServerAsync(stServerIdx,
                                                 ConnectToGameServerCallback,
                                                 &returnSeq,
                                                 NewClientCallback,
                                                 NULL);
                break;
            case 4:  // ���O�A�E�g
                DWC_ShutdownFriendsMatch();   // DWC FriendsMatch���C�u�����I��
                break;
            default:
                break;
            }

            if (!boolRet){
                // �}�b�`���C�N���J�n�ł��Ȃ������ꍇ�̓u���b�L���O��Ԃɂ��Ȃ��B
                // �G���[�����������O�C���O�A�ڑ���A��d�Ăяo��������
                // FALSE�ɂ͂Ȃ�Ȃ�
                OS_TPrintf("Can't call matching function.\n");
                returnSeq          = GAME_MODE_NUM;
                stGameCnt.blocking = 0;
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_Y){
            if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                // X�{�^����������Ă��Ȃ���΁A
                // Y�{�^���Ō��݂̗F�B�̏�Ԃ��擾�\���istatusString�j
                DispFriendList(TRUE);
                OS_TPrintf("\n");
            }
            else {
                // X�{�^����������Ă���΁A
                // Y�{�^���Ō��݂̗F�B�̏�Ԃ��擾�\���istatusData�j
                DispFriendListData();
                OS_TPrintf("\n");
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_SELECT){
#ifndef GAME_USE_STORAGE_SERVER  // GameSpy��Status�Z�b�g�������ꍇ
            if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                // X�{�^����������Ă��Ȃ���΁A
                // �Z���N�g�{�^����GameSpy Status��������Z�b�g
                if (DWC_SetGsStatusString("SampleString-00")){
                    OS_TPrintf("Set GsStatusString.\n");
                }
                else {
                    OS_TPrintf("Failed to set GsStatusString.\n");
                }
            }
            else {
                // X�{�^����������Ă���΁A
                // �Z���N�g�{�^����GameSpy Status�f�[�^���Z�b�g
                if (DWC_SetGsStatusData(gsStatusSample, sizeof(gsStatusSample))){
                    OS_TPrintf("Set GsStatusData.\n");
                }
                else {
                    OS_TPrintf("Failed to set GsStatusData.\n");
                }
            }

#else  // �X�g���[�W�T�[�o�ւ̃Z�[�u�������ꍇ
            // �Z���N�g�{�^���ŃX�g���[�W�T�[�o�Ƀe�X�g�L�[���Z�[�u
            char keyvalues[12];
            BOOL boolResult;

            if (stKeyCnt.cont & PAD_BUTTON_X){
                // X�{�^����������Ă����Private�f�[�^���Z�[�u
                sprintf(keyvalues, "\\test_key_prv\\%lld", OS_GetTick());
                boolResult = DWC_SavePrivateDataAsync(keyvalues, NULL);
            }
            else {
                // X�{�^����������Ă��Ȃ����Public�f�[�^���Z�[�u
                sprintf(keyvalues, "\\test_key_pub\\%lld", OS_GetTick());
                boolResult = DWC_SavePublicDataAsync(keyvalues, NULL);
            }

            if (boolResult){
                OS_TPrintf("Saved '%s'.\n", keyvalues);
            }
            else {
                OS_TPrintf("Can't save to storage server.\n");
            }
#endif
        }
        else if (stKeyCnt.trg & PAD_BUTTON_START){
#ifndef GAME_USE_STORAGE_SERVER  // GameSpy��Status�Z�b�g�������ꍇ
            char statusData[DWC_GS_STATUS_STRING_LEN];
            
            if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                // X�{�^����������Ă��Ȃ���΁A
                // �X�^�[�g�{�^����GameSpy Status��������擾
                if (DWC_GetGsStatusString(statusData)){
                    OS_TPrintf("Got my GsStatusString : %s\n", statusData);
                }
                else {
                    OS_TPrintf("Failed to get GsStatusString.\n");
                }
            }
            else {
                // X�{�^����������Ă���΁A
                // �X�^�[�g�{�^����GameSpy Status�f�[�^���擾
                int size, i;

                size = DWC_GetGsStatusData(statusData);
                if (size != -1){
                    OS_TPrintf("Got my GsStatusData\n");
                    for (i = 0; i < size; i++){
                        OS_TPrintf("%02d, ", statusData[i]);
                    }
                    OS_TPrintf("\n");
                }
                else {
                    OS_TPrintf("Failed to get GsStatusData.\n");
                }
            }

#else  // �X�g���[�W�T�[�o����̃��[�h�������ꍇ
            // �X�^�[�g�{�^���ŃX�g���[�W�T�[�o����e�X�g�f�[�^�����[�h
            BOOL boolResult;

#if 1
            if (stKeyCnt.cont & PAD_BUTTON_X){
                // X�{�^����������Ă���Ύ�����Private�f�[�^�����[�h
                boolResult = DWC_LoadOwnPrivateDataAsync("\\test_key_prv\\test_key_pub", NULL);
            }
            else {
                // X�{�^����������Ă��Ȃ���Ύ�����Public�f�[�^�����[�h
                boolResult = DWC_LoadOwnPublicDataAsync("\\test_key_prv\\test_key_pub", NULL);
            }
#else
            // �F�B���X�g���̃z�X�g�̃f�[�^�����[�h
            boolResult = DWC_LoadOthersDataAsync("\\test_key", stServerIdx, NULL);
#endif
            if (!boolResult) OS_TPrintf("Can't load from storage server.\n");
#endif
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // �\���L�[��ŃJ�[�\���ړ�
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // �\���L�[���ŃJ�[�\���ړ�
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_LEFT){
            // �\���L�[���ŃG���g���[�l������
            if (stNumEntry > 2){
                stNumEntry--;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_RIGHT){
            // �\���L�[�E�ŃG���g���[�l������
            if (stNumEntry < DWC_MAX_CONNECTIONS){
                stNumEntry++;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_L){
            // LR�{�^���ŃT�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS��
            // �F�B���X�g�C���f�b�N�X��I��
            if (curIdx == 3){
                if (stServerIdx > 0){
                    stServerIdx--;
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
                }
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_R){
            // LR�{�^���ŃT�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS��
            // �F�B���X�g�C���f�b�N�X��I��
            if (curIdx == 3){
                if (stServerIdx < GAME_MAX_FRIEND_LIST-1){
                    stServerIdx++;
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
                }
            }
        }
        ////////// �L�[���쏈�������܂�

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/** -------------------------------------------------------------------------
  DWC Transport
  ---------------------------------------------------------------------------*/
const int		SIZE_SEND_BUFFER = 512;
const int		SIZE_RECV_BUFFER = 4 * 1024;

static u8		sSendBuffer[ SIZE_SEND_BUFFER ];
static u8		sRecvBuffer[ DWC_MAX_CONNECTIONS-1 ][ SIZE_RECV_BUFFER ];

/** -------------------------------------------------------------------------
  ���M�����R�[���o�b�N  
  ---------------------------------------------------------------------------*/
static void
SendDoneCallback( int size, u8 aid )
{
	OS_TPrintf( "to aid = %d  size = %d\n", aid, size );
}

/** -------------------------------------------------------------------------
  ��M�����R�[���o�b�N  
  ---------------------------------------------------------------------------*/
static void
UserRecvCallback( u8 aid, u8* buffer, int size )
{
	OS_TPrintf( "from aid = %d  size = %d  buffer[0] = %X\n", aid, size, buffer[0] );
}

/** -------------------------------------------------------------------------
  DWC Transport�p�@��M�o�b�t�@��ݒ�  
  ---------------------------------------------------------------------------*/
static void
SetRecvBuffer( void )
{
    u8* pAidList;
	int i;
    int j = 0;
	int connectingNum = DWC_GetAIDList(&pAidList);

    OS_TPrintf( "connection host = %d\n", connectingNum );
    
    for ( i = 0; i < connectingNum; ++i )
    {
		if ( pAidList[i] == DWC_GetMyAID() )
        {
            j++;
			continue;
        }

        DWC_SetRecvBuffer( pAidList[i], &sRecvBuffer[i-j], SIZE_RECV_BUFFER );
    }
}

/** -------------------------------------------------------------------------
  Reliabe���M
  ---------------------------------------------------------------------------*/
static void
BroadCastReliable( void )
{
#if 1
    // AID���X�g���g���đS�����M���s���ꍇ
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID()) continue;

        DWC_SendReliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }

#else
    // AID�r�b�g�}�b�v���g���đS�����M���s���ꍇ
    u32 bitmap = DWC_SendReliableBitmap( DWC_GetAIDBitmap(), sSendBuffer, SIZE_SEND_BUFFER );
    OS_TPrintf("Sent to %x.\n", bitmap);
#endif
}

/** -------------------------------------------------------------------------
  Unreliable���M
  ---------------------------------------------------------------------------*/
static void
BroadCastUnreliable( void )
{
#if 1
    // AID���X�g���g���đS�����M���s���ꍇ
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID()) continue;

        DWC_SendUnreliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }

#else
    // AID�r�b�g�}�b�v���g���đS�����M���s���ꍇ
    u32 bitmap = DWC_SendUnreliableBitmap( DWC_GetAIDBitmap(), sSendBuffer, SIZE_SEND_BUFFER );
    OS_TPrintf("Sent to %x.\n", bitmap);
#endif
}

/*---------------------------------------------------------------------------*
  ���b�V���ڑ������チ�C���֐�
 *---------------------------------------------------------------------------*/
static GameMode GameConnectedMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_CONNECTED];
    GameMode returnSeq = GAME_MODE_NUM;
    int closeResult;
    int curIdx = 0;
    
    // ���M�R�[���o�b�N�̐ݒ�	
    DWC_SetUserSendCallback( SendDoneCallback ); 

    // ��M�R�[���o�b�N�̐ݒ�	
    DWC_SetUserRecvCallback( UserRecvCallback ); 
    
    // ��M�o�b�t�@�̐ݒ�
    // �����ŃZ�b�g����ƃT�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ��
    // �ォ������Ă���N���C�A���g�̎�M�o�b�t�@���Z�b�g����Ȃ��I
    //SetRecvBuffer();

    // �R�l�N�V�����N���[�Y�R�[���o�b�N��ݒ�
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);

#if 0
	// �p�P�b�g���X����ݒ�
	if ( DWC_GetMyAID() != 0 )
    {
		DWC_SetSendDrop( 30, 0 );
		DWC_SetRecvDrop( 30, 0 );        
    }
#endif
#if 0
    // �x����ݒ�i�P��: ms�j
	if ( DWC_GetMyAID() != 0 )
    {
		DWC_SetSendDelay( 500, 0 );
		DWC_SetRecvDelay( 500, 0 );        
    }
#endif    

    OS_TPrintf("AID bitmap = %x\n", DWC_GetAIDBitmap());
    OS_TPrintf( "my aid = %d\n", DWC_GetMyAID() );

    while (1){        
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;

        ReadKeyData();  // �L�[�f�[�^�擾

        DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

        HandleDWCError(&returnSeq);  // �G���[����

        if (stGameCnt.blocking){
            // DWC�������̓L�[������֎~����
            // V�u�����N�҂�����
            GameWaitVBlankIntr();
            continue;
	    }

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
        if ((stMatchFinishTime != 0) &&
            (OS_TicksToSeconds(OS_GetTick()-stMatchFinishTime) > GAME_MATCH_OP_SC_BLOCK_TIME)){
            // �}�b�`���C�N�P��������A��莞�Ԍo������V�K�ڑ��̃u���b�N����������
            DWC_ClearMOSCConnectBlock();

            stMatchFinishTime = 0;
            OS_TPrintf("Clear matching block!.\n");
        }
#endif

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă����烋�[�v�𔲂���
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// �ȉ��L�[���쏈��
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // A�{�^���Ń��j���[����
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (curIdx){
            case 0:             // Reliable���M
                BroadCastReliable();
				break;
            case 1:             // Unreliable���M
				BroadCastUnreliable();
                break;
            case 2:             // �R�l�N�V�����N���[�Y
                stCloseFlag = TRUE;
                closeResult = DWC_CloseAllConnectionsHard();
                break;
            default:
                break;
            }
        }
#ifdef DWC_STOP_SC_SERVER
        else if (stKeyCnt.trg & PAD_BUTTON_START){
            // �X�^�[�g�{�^���ŃT�[�o�N���C�A���g�}�b�`���C�N����ߐ؂�
            BOOL boolResult = DWC_StopSCMatchingAsync(StopSCMatchingCallback, NULL);
            OS_TPrintf("DWC_StopSCMatchingAsync : result %d\n", boolResult);
        }
#endif
        else if (stKeyCnt.trg & PAD_BUTTON_SELECT){
            // �Z���N�g�{�^����aid = 1�̃z�X�g�������ؒf����B
            // �ʐM���ɓd����؂����z�X�g��ؒf���鎞�̃e�X�g
            u32 bitmap = 0x02;
#if 1
            // AID�w���
            closeResult = DWC_CloseConnectionHard(1);
#else
            // AID�r�b�g�}�b�v�w���
            closeResult = DWC_CloseConnectionHardBitmap(&bitmap);
#endif

            OS_TPrintf("Closed connection to aid = 1, result = %d, bitmap = %x\n",
                       closeResult, bitmap);

            // ���ʂɊւ�炸�����𑱂���̂Ńu���b�L���O���Ȃ��B
            // �܂��A�N���[�Y�R�[���o�b�N��CloseConnectionHard�֐����ŌĂ΂��B
            stGameCnt.blocking = FALSE;
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // �\���L�[��ŃJ�[�\���ړ�
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // �\���L�[���ŃJ�[�\���ړ�
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        ////////// �L�[���쏈�������܂�

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�̋󂫃C���f�b�N�X�擾�֐�
 *---------------------------------------------------------------------------*/
static int GetAvailableFriendListIndex(void)
{
    int i;

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i]))
            return i;
    }

    return -1;
}


/*---------------------------------------------------------------------------*
  ���O�C���R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void LoginCallback(DWCError error, int profileID, void *param)
{
    BOOL result;
	RTCTime time;
	RTCDate date;

	if (error == DWC_ERROR_NONE){
        // �F�ؐ����A�v���t�@�C��ID�擾
        OS_TPrintf("Login succeeded. profileID = %u\n\n", profileID);

        // ingamesn�`�F�b�N�̌��ʂ��擾����
		if(DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID)
			OS_TPrintf("BAD ingamesn is detected by NAS.\n");
		else if(DWC_GetIngamesnCheckResult() == DWC_INGAMESN_VALID)
			OS_TPrintf("GOOD ingamesn is detected by NAS.\n");
		else
			OS_TPrintf("ingamesn is not checked yet.\n");
		
		// �F�؃T�[�o�̎�����\������
		if(DWC_GetDateTime(&date, &time) == TRUE)
			OS_TPrintf("NasTime = %02d/%02d/%02d %02d:%02d:%02d\n", date.year+2000, date.month, date.day, time.hour, time.minute, time.second);
		else
			DWC_Printf(DWC_REPORTFLAG_AUTH, "DWC_GetNasTime failed\n");


        // stUserData���X�V����Ă��邩�ǂ������m�F�B
        if ( DWC_CheckDirtyFlag( &stUserData ) )
        {
            DWC_ClearDirtyFlag( &stUserData );

            OS_TPrintf("*******************************\n");
            OS_TPrintf("You must save the DWCUserData!!\n");
            OS_TPrintf("*******************************\n");

            // �K�����̃^�C�~���O�Ń`�F�b�N���āAdirty flag���L���ɂȂ��Ă�����A
            // DWCUserData��DS�J�[�h�̃o�b�N�A�b�v�ɕۑ�����悤�ɂ��Ă��������B
            // saveUserDataToDSCard( &stUserData );
        }

        stGameCnt.userData.profileID = profileID;
        stGameCnt.userData.isValidProfile = TRUE;

        // GameSpy Status������Z�b�g�e�X�g
        //DWC_SetGsStatusData(gsStatusSample, sizeof(gsStatusSample));
        

        // �F�B���X�g���������J�n�B
        result = DWC_UpdateServersAsync(NULL,//stGameCnt.userData.playerName,
                                        UpdateServersCallback, param,
                                        FriendStatusCallback, param,
                                        DeleteFriendListCallback, param);
        if (result == FALSE){
            // �A�b�v�f�[�g���J�n�ł��Ȃ������ꍇ�̓u���b�L���O��Ԃ���������B
            // �G���[�����������O�C���O�A��d�Ăяo��������FALSE�ɂ͂Ȃ�Ȃ��B
            OS_TPrintf("Can't call DWC_UpdateServersAsync().\n");
             *(GameMode *)param = GAME_MODE_NUM;
            stGameCnt.blocking  = 0;
        }
        else {
            // GameSpy�T�[�o��o�f�B�����R�[���o�b�N��o�^����
            DWC_SetBuddyFriendCallback(BuddyFriendCallback, NULL);

            // �X�g���[�W�T�[�o�Z�[�u�E���[�h�����ʒm�R�[���o�b�N��o�^����
            DWC_SetStorageServerCallback(SaveToServerCallback,
                                         LoadFromServerCallback);
        }
    }
    else {
        // �F�؎��s
        OS_TPrintf("Login failed. %d\n\n", error);

        stGameCnt.blocking = FALSE;  // �u���b�L���O����
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�������������R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param)
{
#pragma unused(param)

    if (error == DWC_ERROR_NONE){
        // �F�B���X�g������������
        OS_TPrintf("Updating servers succeeded.\n");

        if (isChanged){
            // �F�B���X�g���ύX����Ă�����Z�[�u����
            // �i�Z�[�u�̂���j
            MI_CpuCopy32(&stGameCnt.friendList, &saveData.friendList, sizeof(GameFriendList));
        }

        // �F�B���X�g��\������
        DispFriendList(TRUE);
    }
    else {
        // ���s���Ă����ɉ������Ȃ�
        OS_TPrintf("Failed to update GameSpy servers. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // �u���b�L���O����
}


/*---------------------------------------------------------------------------*
  �F�B��ԕω��ʒm�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param)
{
#pragma unused(param)

    OS_TPrintf("Friend[%d] changed status -> %d (statusString : %s).\n",
               index, status, statusString);
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�폜�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void DeleteFriendListCallback(int deletedIndex, int srcIndex, void* param)
{
#pragma unused(param)

    OS_TPrintf("friend[%d] was deleted (equal friend[%d]).\n",
               deletedIndex, srcIndex);
}


/*---------------------------------------------------------------------------*
  GameSpy�o�f�B�����R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void BuddyFriendCallback(int index, void* param)
{
#pragma unused(param)

    OS_TPrintf("I was authorized by friend[%d].\n", index);
}


/*---------------------------------------------------------------------------*
  �X�g���[�W�T�[�o�ւ̃��O�C�������R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void LoginToStorageCallback(DWCError error, void* param)
{
#pragma unused(param)

    OS_TPrintf("Login to storage server: result %d\n", error);
}


/*---------------------------------------------------------------------------*
  �X�g���[�W�T�[�o�ւ̃f�[�^�Z�[�u�����R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param)
{

    OS_TPrintf("Saved data to server. %08x\n", (u32)param);
    OS_TPrintf("result %d, isPublic %d.\n", success, isPublic);
}


/*---------------------------------------------------------------------------*
  �X�g���[�W�T�[�o����̃f�[�^���[�h�����R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void LoadFromServerCallback(BOOL success, int index, char* data, int len, void* param)
{

    OS_TPrintf("Loaded data from server. %08x\n", (u32)(param));
    OS_TPrintf("result %d, index %d, data '%s', len %d\n",
               success, index, data, len);
}


/*---------------------------------------------------------------------------*
  �F�B���w��ڑ��R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void ConnectToAnybodyCallback(DWCError error, BOOL cancel, void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            // ���m��ʐl�����Ƃ̃R�l�N�V�����ݗ�����
            OS_TPrintf("Succeeded to connect to anybody\n\n");

            // ��M�o�b�t�@�Z�b�g
            SetRecvBuffer();
        }
        else {
            OS_TPrintf("Canceled matching.\n");
            // ���O�C�����Ԃɖ߂�
            *(GameMode *)param = GAME_MODE_LOGIN;
        }
    }
    else {
        OS_TPrintf("Failed to connect to anybody. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // �u���b�L���O����
}


/*---------------------------------------------------------------------------*
  �F�B�w��ڑ��R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void ConnectToFriendsCallback(DWCError error, BOOL cancel, void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            // �F�B�Ƃ̃R�l�N�V�����ݗ�����
            OS_TPrintf("Succeeded to connect to friends\n\n");

            // ��M�o�b�t�@�Z�b�g
            SetRecvBuffer();
        }
        else {
            OS_TPrintf("Canceled matching.\n");
            // ���O�C�����Ԃɖ߂�
            *(GameMode *)param = GAME_MODE_LOGIN;
        }
    }
    else {
        OS_TPrintf("Failed to connect to friends. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // �u���b�L���O����
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�E�T�[�o�N���R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void SetupGameServerCallback(DWCError error,
                                    BOOL cancel,
                                    BOOL self,
                                    BOOL isServer,
                                    int  index,
                                    void* param)
{
#pragma unused(isServer)

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            // �l�b�g���[�N�ɐV�K�N���C�A���g���������
            OS_TPrintf("A friend of mine joined the game.\n");
            OS_TPrintf("friendListIndex = %d.\n\n", index);

            // ��M�o�b�t�@�Z�b�g
            SetRecvBuffer();

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
            // �}�b�`���C�N�P���������̎��Ԃ��L�^
            stMatchFinishTime = OS_GetTick();
#endif
        }
        else {
            if (self){
                // �������}�b�`���C�N���L�����Z������
                OS_TPrintf("Canceled matching.\n\n");
                // ���O�C�����Ԃɖ߂�
                // param�̎Q�Ɛ��auto�ϐ�returnSeq�����A�������}�b�`���C�N��
                // �L�����Z���ł��鎞�̓X�R�[�v��
                *(GameMode *)param = GAME_MODE_LOGIN;
            }
            else {
                // �N���C�A���gDS���}�b�`���C�N���L�����Z������
                OS_TPrintf("Client (friendListIndex = %d) canceled matching.\n\n", index);

                if (DWC_GetNumConnectionHost() == 1){
                    // �܂��N�Ƃ��ڑ����Ă��Ȃ���΃u���b�L���O���������Ȃ��B
                    // ��x�ł��}�b�`���C�N������������ɐڑ�����1�ɂȂ����ꍇ��
                    // ���X�u���b�L���O������ԂȂ̂Ŗ��Ȃ��B
                    return;
                }
            }
        }
    }
    else {
        // �G���[����������
        OS_TPrintf("Game server error occured. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // �u���b�L���O����
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�E�T�[�o�ڑ��R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void ConnectToGameServerCallback(DWCError error,
                                        BOOL cancel,
                                        BOOL self,
                                        BOOL isServer,
                                        int  index,
                                        void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            if (self){
                // �������T�[�oDS�Ƃ����ɂł��Ă���l�b�g���[�N�ւ̐ڑ���
                // ���������ꍇ
                OS_TPrintf("Succeeded to join the game.\n\n");
            }
            else {
                // �l�b�g���[�N�ɐV�K�N���C�A���g����������ꍇ
                OS_TPrintf("New client joined the game.\n");
                if (index != -1){
                    OS_TPrintf("he is my friend [%d].\n\n", index);
                }
                else {
                    OS_TPrintf("he is not my friend.\n\n");
                }
            }

            // ��M�o�b�t�@�Z�b�g
            SetRecvBuffer();
        }
        else {
            if (self){
                // �������}�b�`���C�N���L�����Z������
                OS_TPrintf("Canceled matching.\n\n");
                // ���O�C�����Ԃɖ߂�
                *(GameMode *)param = GAME_MODE_LOGIN;
            }
            else {
                if (isServer){
                    // �T�[�oDS���}�b�`���C�N���L�����Z������
                    OS_TPrintf("GameServer canceled matching.\n\n");
                    // �}�b�`���C�N���I�����ă��O�C�����Ԃɖ߂�
                    *(GameMode *)param = GAME_MODE_LOGIN;
                }
                else {
                    // ���̃N���C�A���gDS���}�b�`���C�N���L�����Z������
                    OS_TPrintf("Host (friendListIndex = %d) canceled matching.\n\n", index);
                }
            }
        }
    }
    else {
        // �G���[����������
        OS_TPrintf("Failed to join the game. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // �u���b�L���O����
}


/*---------------------------------------------------------------------------*
  �R�l�N�V�����N���[�Y�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8  aid,
                                     int index,
                                     void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!isLocal && isServer){
            // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�������N���C�A���g�̎��ɁA
            // �T�[�oDS���R�l�N�V�������N���[�Y�����B
            // �T�[�oDS���N���[�Y�����̂Ŏ������N���[�Y����B
            stCloseFlag = TRUE;
            DWC_CloseAllConnectionsHard();
        }
        
        if (DWC_GetNumConnectionHost() == 1){
            // �S�ẴR�l�N�V�������N���[�Y���ꂽ�ꍇ
            if ((DWC_GetLastMatchingType() != DWC_MATCH_TYPE_SC_SV) || stCloseFlag){
                // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS�������ŃN���[�Y�֐���
                // �Ă�ŃN���[�Y�����ꍇ���A���̑��̃}�b�`���C�N�^�C�v�̏ꍇ�́A
                // �����Ŏ��̃��[�h���Z�b�g����
                *(GameMode *)param = GAME_MODE_LOGIN;
                stCloseFlag = FALSE;

                // ���̃}�b�`���C�N�ɔ����Ď��z�X�g�f�[�^��GameSpy�T�[�o�ɓo�^����
                // �T�[�o�N���C�A���g�}�b�`���C�N�ł͂��̊֐����Ă�ł������N��Ȃ�
                DWC_RegisterMatchingStatus();
            }
            // �������T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS�ŁA
            // �N���C�A���g���S���N���[�Y�����ꍇ�͉������Ȃ�
        }

        if (isLocal){
            OS_TPrintf("Closed connection to aid %d (friendListIndex = %d) Rest %d.\n",
                       aid, index, DWC_GetNumConnectionHost());
        }
        else {
            OS_TPrintf("Connection to aid %d (friendListIndex = %d) was closed. Rest %d.\n",
                       aid, index, DWC_GetNumConnectionHost());
        }
    }
    else {
        OS_TPrintf("Failed to close connections. %d\n\n", error);
    }
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N���̐V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void NewClientCallback(int index, void* param)
{
#pragma unused(param)

    // �V�K�ڑ��N���C�A���g�̐ڑ��������I���܂ł́A
    // �R���g���[��������֎~����
    // �����ۂ̃Q�[���̏����ɑ����ău���b�L���O���Ȃ��悤�ɂ���
    //stGameCnt.blocking = TRUE;
    
    OS_TPrintf("New client started to connect.\n");
    if (index != -1){
        OS_TPrintf("He is my friend [%d].\n", index);
    }
    else {
        OS_TPrintf("He is not my friend.\n");
    }
}


/*---------------------------------------------------------------------------*
  �F�B���w��s�A�}�b�`���C�N���v���C���[�]���R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static int  EvaluateAnybodyCallback(int index, void* param)
{
#pragma unused(param)
    const char* matchType = DWC_GetMatchStringValue(index, stMatchKeys[1].keyStr, "NONE");
    int rank;
    int diff;
#if 0  // �]���̃e�X�g���s�������ꍇ�͂�����1�ɂ���
    int full = 200;
#else  // ���i�̓}�b�`���C�N���Ȃ����Ƃ�����Ɩʓ|�Ȃ̂ł�����ɂ���
    int full = 300;
#endif
    
    if (!strncmp(matchType, "eval_rank", strlen("eval_rank"))){
        // ������}�b�`���C�N�^�C�v�Ƃ���"eval_rank"���Z�b�g���Ă���ꍇ
        rank = DWC_GetMatchIntValue(index, stMatchKeys[0].keyStr, -1);
        if (rank == -1){
            return 0;  // "game_rank"�L�[���Ȃ���΃}�b�`���C�N���Ȃ�
        }

        diff = (stMatchKeys[0].ivalue > rank) ? stMatchKeys[0].ivalue-rank : rank-stMatchKeys[0].ivalue;

        OS_TPrintf("Player[%d]'s rank is %d (mine %d). %d point.\n",
                   index, rank, stMatchKeys[0].ivalue, full-diff);

        // �����Ƃ̏��ʍ����߂��قǍ����_�ɂȂ�悤�ɁA200�_���_�œ_��t����B
        // 200�ʍ��ȏ�Ȃ�}�b�`���C�N���Ȃ��i0�ȉ��̒l��Ԃ��j
        return full-diff;
    }
    else if (!strncmp(matchType, "NONE", strlen("NONE"))){
        return 1;  // "game_mtype"�L�[���Ȃ���΂Ƃ肠���������Ƀ}�b�`���C�N����
    }

    return 1;
}


/*---------------------------------------------------------------------------*
  �F�B�w��s�A�}�b�`���C�N���v���C���[�]���R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static int  EvaluateFriendCallback(int index, void* param)
{
#pragma unused(param)
    const char* matchType = DWC_GetMatchStringValue(index, stMatchKeys[1].keyStr, "NONE");
    
    if (!strncmp(matchType, "eval_rank", strlen("eval_rank"))){
        // ������}�b�`���C�N�^�C�v�Ƃ���"eval_rank"���Z�b�g���Ă���ꍇ
        return 1;  // �}�b�`���C�NOK
    }
    else {
        return 0;  // "game_mtype"�L�[���Ȃ���΃}�b�`���C�N���Ȃ�
    }
}


#ifdef DWC_STOP_SC_SERVER
/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N���ߐ؂芮���R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void StopSCMatchingCallback(void* param)
{
#pragma unused(param)

    OS_TPrintf("StopSCMatchingCallback was called.\n");
}
#endif


/*---------------------------------------------------------------------------*
  DWC���C�u�����G���[�����֐�
 *---------------------------------------------------------------------------*/
static DWCError HandleDWCError(GameMode* gameMode)
{
    int errorCode;
    DWCError error;
    DWCErrorType errorType;

    // DWC�G���[�擾
    error = DWC_GetLastErrorEx(&errorCode, &errorType);

    // �G���[���Ȃ���΂����ɖ߂�
    if (error == DWC_ERROR_NONE) return DWC_ERROR_NONE;

    // �G���[�ɑΉ��������b�Z�[�W��\��
    DispErrorMessage(error, errorCode, errorType);

    DWC_ClearError();        // �G���[���N���A
    stGameCnt.blocking = 0;  // �u���b�L���O����

    switch (errorType){
    case DWC_ETYPE_LIGHT:
        // ���̂Ƃ���}�b�`���C�N���ɂ����o�Ȃ��G���[�^�C�v�Ȃ̂ŁA
        // �K�����O�C�����Ԃɖ߂�
        *gameMode = GAME_MODE_LOGIN;
        break;

    case DWC_ETYPE_SHUTDOWN_FM:
        // FriendsMatch�������I�����AGameSpy�̃q�[�v���������
        DWC_ShutdownFriendsMatch();

        // ���O�C���O�̏�Ԃɖ߂�
        *gameMode = GAME_MODE_MAIN;
        break;

    case DWC_ETYPE_DISCONNECT:
        // FriendsMatch�������I�����AGameSpy�̃q�[�v���������
        DWC_ShutdownFriendsMatch();

        // �C���^�[�l�b�g�ڑ������S�ɏ���������
        DWC_CleanupInet();

        // ���O�C���O�̏�Ԃɖ߂�
        *gameMode = GAME_MODE_MAIN;
        break;

    case DWC_ETYPE_FATAL:
        // Fatal Error �Ȃ̂Œ�~����
        OS_Panic("========== Stop process ==========\n");
        break;
    }

    return error;
}

/*---------------------------------------------------------------------------*
  �ʐM�G���[�\���֐�
 *---------------------------------------------------------------------------*/
static void DispErrorMessage(DWCError error, int errorCode, DWCErrorType errorType)
{

    if (error == DWC_ERROR_NONE) return;

    OS_TPrintf("ERROR: %d, code:%d, type:%d\n", error, -1*errorCode, errorType);
    
    switch (error){
    case DWC_ERROR_DS_MEMORY_ANY:
        OS_TPrintf("ERROR: DS_MEMORY_ANY.\n");
        break;
    case DWC_ERROR_AUTH_ANY:
        OS_TPrintf("ERROR: AUTH_ANY.\n");
        break;
    case DWC_ERROR_AUTH_OUT_OF_SERVICE:
        OS_TPrintf("ERROR: This game's network service has ended.\n");
        break;
    case DWC_ERROR_AUTH_STOP_SERVICE:
        OS_TPrintf("ERROR: Now server is temporarily unavailable.\nPlease re-login later.\n");
        break;
    case DWC_ERROR_AC_ANY:
        OS_TPrintf("ERROR: AC ANY.\n");
        break;
    case DWC_ERROR_NETWORK:
        OS_TPrintf("ERROR: Network error.\n");
        break;
    case DWC_ERROR_DISCONNECTED:
        OS_TPrintf("ERROR: Connection to AP was lost.\n");
        break;
    case DWC_ERROR_FATAL:
        OS_TPrintf("FATAL ERROR: Please turn the power off.\n");
        break;
    case DWC_ERROR_FRIENDS_SHORTAGE:
        OS_TPrintf("ERROR: Your friends seem not to login yet.\n");
        break;
    case DWC_ERROR_NOT_FRIEND_SERVER:
        OS_TPrintf("ERROR: He is not buddy or not game server.\n");
        break;
    case DWC_ERROR_MO_SC_CONNECT_BLOCK:
        OS_TPrintf("ERROR: Game server is busy now.\n");
        break;
    case DWC_ERROR_SERVER_FULL:
        OS_TPrintf("ERROR: Game server is fully occupied.\n");
        break;
    default:
        OS_TPrintf("ERROR: Undefined yet.\n");
        break;
    }
}


/*---------------------------------------------------------------------------*
  ���b�Z�[�W���X�g�\���i�J�[�\���t���j�֐�
 *---------------------------------------------------------------------------*/
static void DispMenuMsgWithCursor(const GameSequence* gameSeq, int index, void* param)
{
    int i;

    OS_TPrintf("===============================\n");
    OS_TPrintf("%s\n", gameSeq->modeName);

    for (i = 0; i < gameSeq->numMenu; i++){
        if (i == index) OS_TPrintf("> ");
        else OS_TPrintf("  ");

        OS_TPrintf("%s\n", gameSeq->menuList[i].msg);
    }

    // ���[�h���Ƃ̒ǉ��\���p�R�[���o�b�N�֐����Ăяo��
    if (gameSeq->dispCallback) gameSeq->dispCallback(index, param);

    OS_TPrintf("===============================\n\n");
    OS_TPrintf("+++VALID CONSOLE %d +++\n", DWC_CheckValidConsole( &stUserData ) );
    Heap_Print();
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�\���֐��iGameSpy Status������\���Łj
 *---------------------------------------------------------------------------*/
static void DispFriendList(BOOL overLoad)
{
    u8   maxEntry = 0;
    u8   numEntry;
    char statusString[DWC_GS_STATUS_STRING_LEN];
    int  i;

    statusString[0] = '\0';

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i]))
            continue;
        
        if (overLoad){
            // �㏑���w��̏ꍇ�͗F�B���̍X�V���s��
            stGameCnt.friendStatus[i] =
                DWC_GetFriendStatusSC(&stGameCnt.friendList.keyList[i],
                                      &maxEntry, &numEntry, statusString);
        }
        
        if (maxEntry){
            // �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS�̏ꍇ�́A
            // �i���ݐڑ��l���^�ő�ڑ��l���j��\������
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, server (%d/%d), statusString : %s\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i],
                       numEntry, maxEntry, statusString);
        }
        else {
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, statusString : %s\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i], statusString);
        }
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�\���֐��iGameSpy Status�f�[�^�\���Łj
 *---------------------------------------------------------------------------*/
static void DispFriendListData(void)
{
    u8   maxEntry = 0;
    u8   numEntry;
    char statusData[DWC_GS_STATUS_STRING_LEN];
    int  size;
    int  i, j;

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i]))
            continue;
        
        // �F�B���̍X�V
        stGameCnt.friendStatus[i] =
            DWC_GetFriendStatusDataSC(&stGameCnt.friendList.keyList[i],
                                      &maxEntry, &numEntry, statusData, &size);
        
        if (maxEntry){
            // �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS�̏ꍇ�́A
            // �i���ݐڑ��l���^�ő�ڑ��l���j��\������
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, server (%d/%d)\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i],
                       numEntry, maxEntry);
        }
        else {
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i]);
        }

        // GameSpy Status�������\������
        OS_TPrintf("StatusData : ");
        for (j = 0; j < size; j++){
            OS_TPrintf("%02d, ", statusData[j]);
        }
        OS_TPrintf("\n");
    }
}


/*---------------------------------------------------------------------------*
  �F�B�o�^�E�������[�h�̒ǉ��\���p�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void RegFriendModeDispCallback(int curIdx, void* param)
{
    RegFriendParam cntParam;
    int i;

    if (!param){
        // �����param = NULL�ŌĂ΂��
        cntParam.step = 0;
    }
    else {
        cntParam = *(RegFriendParam *)param;
    }

    OS_TPrintf("\n");

    if (cntParam.step == 0){
        // �R�}���h�I�𒆂͗F�B���X�g��\��
        DispFriendList(FALSE);
    }
    else {
        // ���l���͒�
        if (curIdx == 0){
            // �F�B�o�^�B�v���t�@�C��ID���͒�
            OS_TPrintf("Set profileID to friend list : %010u\n", cntParam.value);

            // �I�𒆂̐����������ׂ̉�����\������
            for (i = 0; i < 9-cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("                               -\n");
        }
        else {
            // �F�B�폜�B�F�B���X�g�C���f�b�N�X���͒�
            DispFriendList(FALSE);  // �F�B���X�g��\��
            
            OS_TPrintf("Delete index : %02u\n", cntParam.value);

            for (i = 0; i < 1-cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("               -\n");
        }
    }
}


/*---------------------------------------------------------------------------*
  ���O�C���ヂ�[�h�̒ǉ��\���p�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void LogonModeDispCallback(int curIdx, void* param)
{
#pragma unused(param)

    // �����ȊO�̃G���g���[�l���ւ̃|�C���^��\������
    OS_TPrintf("\nnumber of entry = ");
    if (stNumEntry == 2){
        OS_TPrintf("   %d >>\n", stNumEntry);
    }
    else if (stNumEntry == DWC_MAX_CONNECTIONS){
        OS_TPrintf("<< %d\n", stNumEntry);
    }
    else {
        OS_TPrintf("<< %d >>\n", stNumEntry);
    }

    if (curIdx == 3){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS�ւ̐ڑ����́A
        // �F�B���X�g�C���f�b�N�X���\������
        OS_TPrintf("GameServerIndex = ");
        if (DWC_GetNumFriend(stGameCnt.friendList.keyList, GAME_MAX_FRIEND_LIST) == 1){
            OS_TPrintf("   %d\n", stServerIdx);
        }
        else if (stServerIdx == 0){
            OS_TPrintf("   %d >>\n", stServerIdx);
        }
        else if (stServerIdx == GAME_MAX_FRIEND_LIST-1){
            OS_TPrintf("<< %d\n", stServerIdx);
        }
        else {
            OS_TPrintf("<< %d >>\n", stServerIdx);
        }
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N���[�h�̒ǉ��\���p�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void MatchModeDispCallback(int curIdx, void* param)
{
#pragma unused(curIdx)
#pragma unused(param)
    
}


/*---------------------------------------------------------------------------*
  Wi-Fi�R�l�N�V�����ݒ�̎g�p����I��
 *---------------------------------------------------------------------------*/
static GameMode NetConfigLanguageMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_NETCONFIG_LANGUAGE];
    int curIdx = 0;
    const int languageTable[] = {
        DWC_LANGUAGE_JAPANESE,
        DWC_LANGUAGE_ENGLISH,
        DWC_LANGUAGE_FRENCH,
        DWC_LANGUAGE_GERMAN,
        DWC_LANGUAGE_ITALIAN,
        DWC_LANGUAGE_SPANISH,
        DWC_LANGUAGE_HANGUL
    };

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState());
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost());
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel());
        dbs_Print(10, 1, "p:%d", stGameCnt.userData.profileID);
        dbs_Print(30, 0, "%c",  loopstr[((loopcnt++) >> 2) & 3]);
        ReadKeyData();
        if (stKeyCnt.trg & PAD_BUTTON_A){
            utilityLangage = languageTable[curIdx];
            break;
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu - 1;
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        GameWaitVBlankIntr();
    }

    return gameSeq->menuList[curIdx].mode;
}


/*---------------------------------------------------------------------------*
  Wi-Fi�R�l�N�V�����ݒ�
 *---------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static GameMode NetConfigMain(void)
{
    GameMode returnSeq = GAME_MODE_NETCONFIG_LANGUAGE;

    (void)FS_LoadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    DWC_CleanupInet(); // �l�b�g���[�N����ؒf����B
    sPrintOverride = FALSE; // OS_TPrintf()�̏o�͂��ꎞ�I�Ɍ��ɖ߂��B
    dbs_DemoFinalize();

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

#if defined( NETCONFIG_USE_HEAP )
    {
        void* work = OS_Alloc(DWC_UTILITY_WORK_SIZE);

        (void)DWC_StartUtility(
            work,
            utilityLangage,
            utilityLangage == DWC_LANGUAGE_JAPANESE ? DWC_UTILITY_TOP_MENU_FOR_JPN : DWC_UTILITY_TOP_MENU_COMMON
        );

        OS_Free(work);
    }
#else
    (void)DWC_StartUtilityEx(
        utilityLangage,
        utilityLangage == DWC_LANGUAGE_JAPANESE ? DWC_UTILITY_TOP_MENU_FOR_JPN : DWC_UTILITY_TOP_MENU_COMMON
    );
#endif

    (void)FS_UnloadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr( TRUE );

    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()�̏o�͂�console�ɂȂ�.
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    return returnSeq;
}

static GameMode StartIPMain(void)
{
	DWCApInfo apinfo;
	
    DWC_InitInet( &stConnCtrl );

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

    
    DWC_ConnectInetAsync();
    // ����ȃX�e�[�g�܂ő҂B
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    switch ( DWC_GetInetStatus() )
    {
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_TPrintf("   Error occurred %d %d.\n", err, errcode );
        }
        DWC_ClearError();

        DWC_CleanupInet();
        break;
    case DWC_CONNECTINET_STATE_FATAL_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_Panic("   Error occurred %d %d.\n", err, errcode );
        }
        break;

    case DWC_CONNECTINET_STATE_CONNECTED:
        OS_TPrintf("   CONNECTED!!!.\n");
		if(DWC_GetApInfo(&apinfo) == TRUE) {
			OS_TPrintf("DWCApInfo.aptype = %d\n", apinfo.aptype);
			OS_TPrintf("DWCApInfo.area   = %d\n", apinfo.area);
			OS_TPrintf("DWCApInfo.spotinfo = %s\n", apinfo.spotinfo);
			OS_TPrintf("DWCApInfo.essid = %s\n", apinfo.essid);
			OS_TPrintf("DWCApInfo.bssid = %02x:%02x:%02x:%02x:%02x:%02x\n", apinfo.bssid[0], apinfo.bssid[1], apinfo.bssid[2], apinfo.bssid[3], apinfo.bssid[4], apinfo.bssid[5]);
		}
		else {
			OS_TPrintf("Failed to get DWCApInfo\n");
		}
			
        break;
    }

    return GAME_MODE_MAIN;
}

static GameMode StopIPMain(void)
{
    DWC_CleanupInet();

    return GAME_MODE_MAIN;
}

static GameMode BackupUserdataMain( void )
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_BACKUP];
    int curIdx = 0;
    GameMode returnSeq = GAME_MODE_NUM;

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;

        ReadKeyData();  // �L�[�f�[�^�擾

        HandleDWCError(&returnSeq);  // �G���[����

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă����烋�[�v�𔲂���
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// �ȉ��L�[���쏈��
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // A�{�^���Ń��j���[����
            BOOL end = FALSE;
            BOOL error = FALSE;
            int last_result;
            CARD_LockBackup((u16)card_lock_id);
            end = CARD_IdentifyBackup(stBackupTypeTable[stNumBackup].type);
            if(!end)
            {
                error = TRUE;
                last_result = CARD_GetResultCode();
            }
            CARD_UnlockBackup(card_lock_id);


            switch (curIdx){
            case 0:  // Load
                if ( error ) {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                CARD_LockBackup((u16)card_lock_id);
                if (CARD_IsBackupEeprom())
                {
                    CARD_ReadEeprom(0, &stUserData, sizeof(stUserData) );
					CARD_ReadEeprom(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFlash())
                {
                    CARD_ReadFlash(0, &stUserData, sizeof(stUserData) );
					CARD_ReadFlash(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFram())
                {
                    CARD_ReadFram(0, &stUserData, sizeof(stUserData) );
					CARD_ReadFram(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                CARD_UnlockBackup(card_lock_id);
                last_result = CARD_GetResultCode();

                if (last_result != CARD_RESULT_SUCCESS)
                {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                else
                {
                    OS_TPrintf("Load OK!\n");
                    returnSeq = GAME_MODE_MAIN;
                }
                break;
            case 1:  // Save
                if ( error ) {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                CARD_LockBackup((u16)card_lock_id);
                if (CARD_IsBackupEeprom())
                {
                    CARD_WriteAndVerifyEeprom(0, &stUserData, sizeof(stUserData) );
					CARD_WriteAndVerifyEeprom(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFlash())
                {
                    CARD_WriteAndVerifyFlash(0, &stUserData, sizeof(stUserData) );
					CARD_WriteAndVerifyFlash(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFram())
                {
                    CARD_WriteAndVerifyFram(0, &stUserData, sizeof(stUserData) );
					CARD_WriteAndVerifyFram(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                CARD_UnlockBackup(card_lock_id);
                last_result = CARD_GetResultCode();

                if (last_result != CARD_RESULT_SUCCESS)
                {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                else
                {
                    OS_TPrintf("Save OK!\n");
                    returnSeq = GAME_MODE_MAIN;
                }
                break;
            case 2: // report
                DWC_ReportUserData( &stUserData );
                break;
            case 3:
                returnSeq = GAME_MODE_MAIN;
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // �\���L�[��ŃJ�[�\���ړ�
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // �\���L�[���ŃJ�[�\���ړ�
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_LEFT){
            // �\���L�[���ŃG���g���[�l������
            stNumBackup--;
            stNumBackup %= BACKUP_TABLE_MAX;

            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_RIGHT){
            // �\���L�[�E�ŃG���g���[�l������
            stNumBackup++;
            stNumBackup %= BACKUP_TABLE_MAX;

            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        ////////// �L�[���쏈�������܂�

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  ���O�C���ヂ�[�h�̒ǉ��\���p�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void BackupModeDispCallback(int curIdx, void* param)
{
#pragma unused(param)
#pragma unused(curIdx)
    OS_TPrintf("\n BACKUP: %s\n", stBackupTypeTable[stNumBackup].comment);
}


/*---------------------------------------------------------------------------*
  V�u�����N�҂��֐�
 *---------------------------------------------------------------------------*/
static void GameWaitVBlankIntr(void)
{
    int i;

    dbs_DemoUpdate();

    // �z�肷��Q�[���t���[���ɍ��킹��V�u�����N�҂��̉񐔂�ς���
    for (i = 0; i < GAME_FRAME; i++){
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    }

    Heap_Debug();
    dbs_DemoLoad();

    // �X�^�b�N���`�F�b�N
    OS_CheckStack(OS_GetCurrentThread());
}


/*---------------------------------------------------------------------------*
  V�u�����N���荞�݊֐�
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
