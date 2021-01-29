//=============================================================================
/**
 * @file	comm_state.c
 * @brief	�ʐM��Ԃ��Ǘ�����T�[�r�X  �ʐM�̏�ʂɂ���
 *          �X���b�h�̂ЂƂƂ��ē����A�����̒ʐM��Ԃ⑼�̋@���
 *          �J�n��I�����Ǘ�����
 *          �t�B�[���h�Ǘ������Ȃ�������������񂱂��Ɉ��z��...2006.01.12
 * @author	k.ohno
 * @date    2006.01.12
 * @data	2007.12.06	Wi-Fi���r�[��p���O�C�������̒ǉ�	tomoya
 */
//=============================================================================

#include "common.h"
#include "communication/communication.h"
#include "wh.h"
#include "comm_local.h"
#include "system/gamedata.h"
#include "system/pm_str.h"

#include "wifi/dwc_rap.h"   //WIFI
#include "wifi/dwc_lobbylib.h"   //WIFIlobby
#include "system/snd_tool.h"  //sndTOOL

#include  "communication/wm_icon.h"

#include "communication/comm_state.h"

//==============================================================================
//	�^�錾
//==============================================================================
// �R�[���o�b�N�֐��̏���
typedef void (*PTRStateFunc)(void);

//==============================================================================
// ���[�N
//==============================================================================

typedef struct{
    void* pWifiFriendStatus;
    u8 select[6];
    MATHRandContext32 sRand; ///< �e�q�@�l�S�V�G�[�V�����p�����L�[
    PTRStateFunc state;
    SAVEDATA* pSaveData;
    MYSTATUS* pMyStatus;
    const REGULATION* pReg;
    COMMSTATE_DWCERROR aError;
    int timer;
    u16 reConnectTime;  // �Đڑ����Ɏg�p����^�C�}�[
#ifdef PM_DEBUG		// Debug ROM
    u16 debugTimer;
#endif
    u8 limitNum;      // ��t�����������ꍇ��LIMIT��
    u8 nowConnectNum;      // ���݂̐ڑ��l��
    u8 negotiation;   // �ڑ��F��
    u8 connectIndex;   // �q�@���ڑ�����e�@��index�ԍ�
    u8 serviceNo;      // �ʐM�T�[�r�X�ԍ�
    u8 regulationNo;   // �R���e�X�g��ʂ����߂�ԍ�
#ifdef PM_DEBUG		// Debug ROM
    u8 soloDebugNo;
    u8 bDebugStart;
#endif
    u8 disconnectIndex;  //wifi�ؒf���� �e�@�q�@
    s8 wifiTargetNo;   // WIFI�ڑ����s���l�̔ԍ�
    u8 bFirstParent;   // �J��Ԃ��e�q�؂�ւ����s���ꍇ�̍ŏ��̐e���
    u8 bDisconnectError; // �ؒf�G���[���G���[�ɂ���Ƃ�TRUE
    u8 bErrorAuto;     // �����G���[�����N��TRUE
    u8 bWorldWifi;     // DPW�������Ă���ꍇ
    u8 ResetStateType;    // �G���[��Ԃɓ���\�t�g�E�G�A���Z�b�g��҂��
    u8 bUnionPause;    // ���j�I�����[���Őڑ���h�������ꍇ
    u8 partyGameBit;   // ���Ђ�GGID���E��
    u8 bParentOnly;   // �e�@��Ԃɂ����Ȃ�Ȃ�
    u8 bChildOnly;   // �q�@��Ԃɂ����Ȃ�Ȃ�
    u8 bNotConnect;   // �ʐM��ԂɑJ�ڂ��Ȃ�
    u8 bWifiDisconnect; //WIFI�ؒf�p �R�}���h�󂯎�����ꍇ��1
    u8 stateError;         //�G���[�����ɂ���ꍇ1�ȏ�
    u8 bPauseFlg;

	// �v���`�i�Œǉ�
    const void* cp_lobby_init_profile;	// ���r�[�p�������v���t�B�[��
	u8 lobby_dwc_login;					// DWC_Login_Async�����`�F�b�N�p
	u8 lobby_dummy[3];
} _COMM_STATE_WORK;

static _COMM_STATE_WORK* _pCommState = NULL;  ///<�@���[�N�\���̂̃|�C���^




// WiFi���r�[�f�o�b�N�J�n�p
#ifdef COMMST_DEBUG_WFLBY_START
static u32 COMMST_DEBUG_WFLBY_START_season;
static u32 COMMST_DEBUG_WFLBY_START_room;
#endif
#ifdef PM_DEBUG
extern u32 D_Tomoya_WiFiLobby_ChannelPrefix;
extern BOOL D_Tomoya_WiFiLobby_ChannelPrefixFlag;
#endif

//==============================================================================
// ��`
//==============================================================================

//----#if TESTOHNO
#define _HEAPSIZE_BATTLE           (0x7080)  // �o�g���@�̈�
#define _HEAPSIZE_WIFILOBBY        (0x7080)  // Wi-Fi���r�[�̈�
#define _HEAPSIZE_UNDERGROUND      (0xF000)  // �n���̈�
#define _HEAPSIZE_UNION            (0x7080)  // ���j�I�����[���̈�
#define _HEAPSIZE_POKETCH          (0x7000)  // �ۂ������@�̈�
#define _HEAPSIZE_PARTY            (0x7000)  // �p�[�e�B�[�T�[�`
//#define _HEAPSIZE_WIFI            (0x2A000+0x7000)  //DWCRAP ���g�p����̈�(DP)
#define _HEAPSIZE_WIFI            (0x2A000+0xA000 + 0x1400)  //DWCRAP ���g�p����̈�(VCT�Ȃ�)
#define _HEAPSIZE_WIFI_LOBBY      (0x60000)  //LOBBY���܂�DWCRAP���g���̈�VCT�Ȃ�

//#define _HEAPSIZE_DWCLOBBY        (0x50000)  //DWC�ɋ�����HEAP��
#define _HEAPSIZE_DWCLOBBY        (0x58000)  //DWC�ɋ�����HEAP��
#define _HEAPSIZE_DWCP2P          (0x2B000+ 0x1400)

//#define _HEAPSIZE_WIFI            (0x2A000+0xB000)  //DWCRAP ���g�p����̈�(VCT����)
//#define _HEAPSIZE_WIFI            (0x2A000+0xB800)  //DWCRAP ���g�p����̈�(VCT����)
#define _HEAPSIZE_DPW              (0x100)   // ���E�����@�^���[�p

//----#endif //TESTOHNO

#define _PACKETSIZE_BATTLE         (512)//(1376)  // �o�g���@�̈�
#define _PACKETSIZE_UNDERGROUND     (500)  // �n���̈�
#define _PACKETSIZE_UNION          (512)//(1280)  // ���j�I�����[���̈�
#define _PACKETSIZE_POKETCH          (32)  // �ۂ������̈�
#define _PACKETSIZE_PARTY         (32)  // �ۂ������̈�


#define _START_TIME (50)     // �J�n����
#define _CHILD_P_SEARCH_TIME (32) ///�q�@�Ƃ��Đe��T������
#define _PARENT_WAIT_TIME (40) ///�e�Ƃ��Ă̂�т�҂���
#define _FINALIZE_TIME (2)
#define _EXIT_SENDING_TIME (5)
#define _EXIT_SENDING_TIME2 (15)
#define _PARENT_END_TIME (2)
#define _SEND_NAME_TIME (10)
#define _PARENTSCAN_PA (3)  // �e�@�Ƃ��Č�������m����1/3

#define _RETRY_COUNT_UNION  (3)  // ���j�I�����[���Ŏq�@���ڑ��ɗv�����

#define _TCB_COMMCHECK_PRT   (10)    ///< �t�B�[���h������ʐM�̊Ď����[�`����PRI


#define _NEGOTIATION_OK (1)      // �ڑ��m�F����
#define _NEGOTIATION_CHECK (0)   // �ڑ��m�F��
#define _NEGOTIATION_NG (2)     // �ڑ����s

// ���O�C�����̃^�C���A�E�g
#define MYDWC_TIMEOUTLOGIN (30*60*2)



//==============================================================================
// static�錾
//==============================================================================

// �X�e�[�g�̏�����

static void _changeStateDebug(PTRStateFunc state, int time, int line);  // �X�e�[�g��ύX����
static void _changeState(PTRStateFunc state, int time);  // �X�e�[�g��ύX����

#ifdef PM_DEBUG
#if 1
#define   _CHANGE_STATE(state, time)  _changeStateDebug(state, time, __LINE__)
#else
#define   _CHANGE_STATE(state, time)  _changeState(state, time)
#endif
#else  //PM_DEBUG
#define   _CHANGE_STATE(state, time)  _changeState(state, time)
#endif //PM_DEBUG


// �n���֘A�X�e�[�g
static void _underStart(void);           // �q�@�̏����� + �V�X�e���̏�����
static void _underChildPInit(void);      // �q�@�̏�����
static void _underChildFInit(void);
static void _underChildPSearching(void); // �q�@���e�@��T���Ă���
static void _underChildForceConnect(void); // �q�@���e�@�ɋ����ɐڑ���
static void _underChildConnecting(void);   // �q�@���e�@�ɐڑ���
static void _underChildConnect(void);    // �q�@�Ƃ��Đڑ���
static void _underChildReset(void);
static void _underChildFinalize(void);   // �q�@�ɂȂ�Ȃ������̂ŏI��������
static void _underParentFinalize(void);  // �e�@�ɂȂ�Ȃ������̂ŏI��������
static void _underParentInit(void);      // �e�@�Ƃ��ď�����
static void _underParentWait(void);      // �e�@�Ƃ��đҋ@���
static void _underParentConnectInit(void); // �ڑ������̂ŏ�������
static void _underParentConnect(void);   // �e�@�Ƃ��Đڑ���
static void _stateUnderGroundConnectEnd(void);  // �n���ؒf
static void _underSBReset(void);


// �o�g���֘A�X�e�[�g
static void _battleParentInit(void);     // �퓬�p�e�@�Ƃ��ď�����
static void _battleParentWaiting(void);  // �퓬�p�e�@�Ƃ��đҋ@��
static void _battleParentMoveRoomEnter(void);  // �퓬�O�̕����ɓ����đҋ@��
static void _battleParentMoveRoom(void);  // �퓬�O�̕����ɓ����đҋ@��
static void _battleParentSendName(void);  // �����̖��O���݂�Ȃɑ��M
static void _battleParentReTry(void);   // �퓬�p�e�@�𒆒f

static void _battleChildInit(void);     // �퓬�p�q�@�Ƃ��ď�����
static void _battleChildBconScanning(void);  // �퓬�p�q�@�Ƃ��Đe�@�I��
static void _battleChildConnecting(void);  // �ڑ������炢��
static void _battleChildSendName(void);  // �����̖��O���݂�Ȃɑ��M
static void _battleChildWaiting(void);  // �_�C���N�g�p�q�@�ҋ@��
static void _battleChildMoveRoomEnter(void); // �퓬�O�̕����ɓ����đҋ@��
static void _battleChildMoveRoom(void); // �퓬�O�̕����ɓ����đҋ@��
static void _battleChildReTry(void);   // �q�@�𒆒f
static void _battleChildReInit(void);   // �q�@���ċN��
static void _battleChildReset(void);
static void _battleChildReConnect(void);
static void _underChildOnline(void);


// UNION���[���֘A�X�e�[�g
static void _unionStart(void);
static void _unionChildSearching(void);
static void _unionChildFinalize(void);
static void _unionParentInit(void);
static void _unionParentWait(void);
static void _unionChildRestart(void);
static void _unionChildNegotiation(void);

static void _unionForceConnectStart(void);
static void _unionForceConnectStart2(void);
static void _unionForceConnect(void);
static void _unionChildConnecting(void);
static void _unionChildConnectSuccess(void);
static void _unionChildConnectFailed(void);
static void _unionChildReset(void);
static void _unionParentConnect(void);
static void _unionParentPause(void);

static void _mysteryParentInit(void);
static void _mysteryChildInit(void);

// ���̑���ʓI�ȃX�e�[�g
static void _stateNone(void);            // �������Ȃ�
static void _stateConnectError(void);    // �ڑ��G���[���
static void _stateEnd(void);             // �I������
static void _stateConnectChildEndWait(void);   // �q�@�̏I����҂��ďI���
static void _stateConnectChildEnd(void);
static void _stateConnectEnd(void);      // �ؒf�����J�n
static void _stateConnectAutoEnd(void);  // �����ؒf�����J�n


// WIFI�p
static void _wifiBattleLogin(void);
static void _stateWifiLogout(void);

// Wi-Fi���r�[��p
static void _wifiLobbyCommInit( void );		// �ʐM������
static void _wifiLobbyLogin( void );		// Lobby�T�[�o�Ƀ��O�C��
static void _wifiLobbyConnect( void );		// ���O�C������
static void _wifiLobbyLogout( void );		// Lobby�T�[�o���烍�O�A�E�g
static void _wifiLobbyLogoutWait( void );	// ���O�A�E�g�҂�
static void _wifiLobbyError( void );		// Lobby���L�̃G���[
static void _wifiLobbyP2PMatch( void );		// LobbyP2P�ڑ�����
static void _wifiLobbyP2PMatchWait( void );	// LobbyP2P�ڑ��҂�����
static void _wifiLobbyP2PDisconnect( void );
static void _wifiLobbyTimeout( void );

static BOOL _wifiLobbyDwcStepRetCheck( int dwc_err );
static BOOL _wifiLobbyDwcStepRetCheckEasy( int dwc_err );
static BOOL _wifiLobbyUpdate( void );
static BOOL _wifiLobbyUpdateCommon( void );	// Lobby��ԍX�V����
static BOOL _wifiLobbyUpdateMatchCommon( u32* p_matchret );	// Lobby��ԍX�V	�}�b�`���O���p



// �l�S�V�G�[�V�����p�m�FKEY
static u8 _negotiationMsg[]={"FREAK"};
static u8 _negotiationMsgReturnOK[]={" GAME"};
static u8 _negotiationMsgReturnNG[]={" FULL"};


#ifdef PM_DEBUG
static u8 _debugConnectNo = 0;
static u8 _debugParentOnly = 0;
static u8 _debugChildOnly = 0;

void DebugOhnoCommDebugUnderNo(int no)
{
    _debugConnectNo = no;
}

void DebugOhnoCommDebugUnderParentOnly(int no)
{
    _debugParentOnly = no;
}

void DebugOhnoCommDebugUnderChildOnly(int no)
{
    _debugChildOnly = no;
}

#endif //PM_DEBUG

//==============================================================================
/**
 * �ʐM�Ǘ��X�e�[�g�̏���������
 * @param   MYSTATUS* pMyStatus
 * @retval  none
 */
//==============================================================================

static void _commStateInitialize(SAVEDATA* pSaveData,int serviceNo)
{
    void* pWork;

    if(_pCommState!=NULL){   // ���łɓ��쒆�̏ꍇ�K�v�Ȃ�
        return;
    }
    GF_ASSERT(pSaveData);
    CommVRAMDInitialize();
    // ������
    _pCommState = (_COMM_STATE_WORK*)sys_AllocMemory(HEAPID_COMMUNICATION, sizeof(_COMM_STATE_WORK));
    MI_CpuFill8(_pCommState, 0, sizeof(_COMM_STATE_WORK));
    _pCommState->timer = _START_TIME;
    _pCommState->bFirstParent = TRUE;  // �e�̏��߂Ă̋N���̏ꍇTRUE
    _pCommState->pSaveData = pSaveData;
    _pCommState->pMyStatus = SaveData_GetMyStatus(pSaveData);
    _pCommState->limitNum = COMM_MODE_UNION_NUM_MIN + 1;   // ��l�͍Œ�ł��ڑ��\
    _pCommState->negotiation = _NEGOTIATION_CHECK;
    _pCommState->bUnionPause = FALSE;
    _pCommState->serviceNo = serviceNo;
    //_pCommState->bWifi = FALSE;
    CommRandSeedInitialize(&_pCommState->sRand);
    CommCommandInitialize(NULL, 0, NULL);

    if((serviceNo != COMM_MODE_UNION) && (serviceNo != COMM_MODE_PARTY) &&
       (serviceNo != COMM_MODE_MYSTERY)){
        WirelessIconEasy();
    }
}


//==============================================================================
/**
 * �ʐM�Ǘ��X�e�[�g�̏I������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateFinalize(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
//    TCB_Delete(_pCommState->pTcb);
    CommCommandFinalize();
    if(_pCommState->pWifiFriendStatus){
        sys_FreeMemoryEz(_pCommState->pWifiFriendStatus);
    }
    //  �o�g���t�@�N�g���[�ł̓��삪�ς���Ă��܂��s��̏C���̎��Ɍ������̂ŏC��
    if(CommStateIsWifiConnect()){
//    if(_pCommState->serviceNo >= COMM_MODE_BATTLE_SINGLE_WIFI){
        sys_DeleteHeap(HEAPID_WIFIMENU);
    }
    WirelessIconEasyEnd();
    CommVRAMDFinalize();
    sys_FreeMemoryEz(_pCommState);
    sys_DeleteHeap(HEAPID_COMMUNICATION);
    _pCommState = NULL;
}

//==============================================================================
/**
 * ������������ł��邩�ǂ���
 * @param   none
 * @retval  ����ł���ꍇTRUE
 */
//==============================================================================

BOOL CommStateIsInitialize(void)
{
    if(_pCommState){
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * �n���ɂ͂��������̒ʐM����
 * @param   MYSTATUS* pMyStatus
 * @retval  none
 */
//==============================================================================

void CommStateEnterUnderGround(SAVEDATA* pSaveData)
{
    if(_pCommState!=NULL){ // �Ȃ����Ă���ꍇ���͏��O����
        return;
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_UNDERGROUND );
    _commStateInitialize(pSaveData,COMM_MODE_UNDERGROUND);
//    _pCommState->serviceNo = COMM_MODE_UNDERGROUND;
    _pCommState->regulationNo = 0;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = _debugConnectNo + SOLO_DEBUG_NO;
    _pCommState->bParentOnly = _debugParentOnly;
    _pCommState->bChildOnly = _debugChildOnly;
#endif
    // �n���X�e�[�g�̑J�ڂ̂��ߏ�����
    _CHANGE_STATE(_underStart, _START_TIME);
}

//==============================================================================
/**
 * �n�����o��ꍇ�̒ʐM����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateExitUnderGround(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    // �ؒf�X�e�[�g�Ɉڍs����  �����ɐ؂�Ȃ�
    CommSystemShutdown();
    _CHANGE_STATE(_stateUnderGroundConnectEnd, 0);
}

//==============================================================================
/**
 * �r�[�R�����W���ĊJ����
 * @param   MYSTATUS* pMyStatus
 * @retval  none
 */
//==============================================================================

void CommStateUnderRestart(void)
{
    CommSystemReset();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
    // ���邮��܂킷
    _pCommState->reConnectTime = CommGetCurrentID();
    _CHANGE_STATE(_underChildReset, 0);  // �G���[�I���̏ꍇRESET����
}

//==============================================================================
/**
 * �r�[�R�����W���~����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStaetUnderPause(BOOL bFlg)
{
    _pCommState->bPauseFlg = bFlg;
}


//==============================================================================
/**
 * �r�[�R�����W���ĊJ���� �e�@���̃��X�^�[��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underQueueReset(void)
{

    if(!CommMPSwitchParentChild()){
        return;
    }
    CommSystemReset();         // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
    _CHANGE_STATE(_underChildFinalize, 0);

    /*
    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    if(CommIsEmptyQueue_ServerSize()){
        CommSystemResetQueue_Server();
    }
    OHNO_PRINT("���M�L���[����\n");
       */
//    _CHANGE_STATE(_underChildFinalize, _FINALIZE_TIME);
}

void CommStateUnderParentRestart(void)
{
   // _CHANGE_STATE(_underQueueReset, 0);
    _CHANGE_STATE(_underChildFinalize, 0);

}


//==============================================================================
/**
 * �n���ɂ����ă��Z�b�g���Ȃ̂��ǂ����Ԃ�
 * @param   none
 * @retval  RESET���Ȃ�TRUE
 */
//==============================================================================

BOOL CommIsUnderResetState(void)
{
    int i;
    u32 stateAddr = (u32)_pCommState->state;

    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return FALSE;
    }
    if(stateAddr == (u32)_underQueueReset){
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * �n���ɂ����Đڑ���STATE�Ȃ̂��ǂ����Ԃ�
 * @param   none
 * @retval  �ڑ����Ȃ�TRUE
 */
//==============================================================================

BOOL CommIsUnderGroundConnectingState(void)
{
    int i;
    u32 funcTbl[]={
        (u32)_underChildConnect,
        (u32)_underParentConnect,
        0,
    };
    u32 stateAddr = (u32)_pCommState->state;

    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return FALSE;
    }
    for(i = 0; funcTbl[i] != 0; i++ ){
        if(stateAddr == funcTbl[i]){
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================
/**
 * �ʐM��؂�閧��n�ɂ͂���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnderGroundOfflineSecretBase(void)
{
    // �ؒf�X�e�[�g�Ɉڍs����  �����ɐ؂�Ȃ�
    _CHANGE_STATE(_underSBReset, 0);  // �G���[�I���̏ꍇRESET����
}

//==============================================================================
/**
 * �͂��߂̃C�x���g
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateSetFirstEvent(void)
{
    _pCommState->bNotConnect = TRUE;
}





//==============================================================================
/**
 * �ʐM���q���閧��n����߂�
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnderGroundOnlineSecretBase(void)
{
    CommVRAMDInitialize();
    _CHANGE_STATE(_underChildOnline, 0);  // �G���[�I���̏ꍇRESET����
}

//==============================================================================
/**
 * �o�g�����̐e�Ƃ��Ă̒ʐM�����J�n
 * @param   pMyStatus  mystatus
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @param   regulationNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

#ifdef PM_DEBUG
void CommStateEnterBattleParent(SAVEDATA* pSaveData, int serviceNo, int regulationNo, const REGULATION* pReg, BOOL bWifi, int soloDebugNo)
#else
void CommStateEnterBattleParent(SAVEDATA* pSaveData, int serviceNo, int regulationNo, const REGULATION* pReg, BOOL bWifi)
#endif
{
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,serviceNo);
//    _pCommState->serviceNo = serviceNo;
    _pCommState->regulationNo = regulationNo;
    _pCommState->pReg = pReg;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = soloDebugNo;
#endif
    _CHANGE_STATE(_battleParentInit, 0);
}

//==============================================================================
/**
 * �o�g�����̎q�Ƃ��Ă̒ʐM�����J�n
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

#ifdef PM_DEBUG
void CommStateEnterBattleChild(SAVEDATA* pSaveData, int serviceNo, int regulationNo, const REGULATION* pReg,BOOL bWifi, int soloDebugNo)
#else
void CommStateEnterBattleChild(SAVEDATA* pSaveData, int serviceNo, int regulationNo, const REGULATION* pReg,BOOL bWifi)
#endif
{
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,serviceNo);
//    _pCommState->serviceNo = serviceNo;
    _pCommState->regulationNo = regulationNo;
    _pCommState->pReg = pReg;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = soloDebugNo;
#endif
    _CHANGE_STATE(_battleChildInit, 0);
}

//==============================================================================
/**
 * �o�g�����̎q�Ƃ��Ă̒ʐM�����J�n
 * @param   connectIndex �ڑ�����e�@��Index
 * @retval  none
 */
//==============================================================================

void CommStateConnectBattleChild(int connectIndex)
{
    _pCommState->connectIndex = connectIndex;
    _CHANGE_STATE(_battleChildConnecting, 0);
}

//==============================================================================
/**
 * �o�g�����̎q�Ƃ��Ă̒ʐM�ċN������
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateRebootBattleChild(void)
{
    CommSystemResetBattleChild();
    _CHANGE_STATE(_battleChildReTry, 0);
}

//==============================================================================
/**
 * �o�g�����̒ʐM�����I���葱��
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateExitBattle(void)
{
    if(_pCommState==NULL){
        return;      // ���łɏI�����Ă���ꍇ�͏��O
    }
    _CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);
    //_CHANGE_STATE(_stateConnectAutoEnd, _EXIT_SENDING_TIME);
}

//==============================================================================
/**
 * �q�@��t�ł��邩�ǂ���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateSetEntryChildEnable(BOOL bEntry)
{
    WHSetEntry(bEntry);
}


//==============================================================================
/**
 * �o�g���ڑ���STATE�Ȃ̂��ǂ����Ԃ�
 * @param   none
 * @retval  �ڑ���TRUE
 */
//==============================================================================

BOOL CommIsBattleConnectingState(void)
{
    int i;
    u32 funcTbl[]={
        (u32)_battleParentWaiting,
        (u32)_battleChildWaiting,
        0,
    };
    u32 stateAddr = (u32)_pCommState->state;

    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return FALSE;
    }
    for(i = 0; funcTbl[i] != 0; i++ ){
        if(stateAddr == funcTbl[i]){
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================
/**
 * �r�[�R�����W���J�n����
 * @param   MYSTATUS* pMyStatus
 * @retval  none
 */
//==============================================================================

void CommStateUnionBconCollection(SAVEDATA* pSaveData)
{
    if(_pCommState!=NULL){ // �Ȃ����Ă���ꍇ���͏��O����
        return;
    }
    // �ʐM�q�[�v�쐬
    if(sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_UNION )==0){
		// �ʐM�q�[�v���쐬�ł��Ȃ������B
		// ���s�s�\(�d���؂�G���[�ցj
		CommFatalErrorFunc_NoNumber();

	}
    _commStateInitialize(pSaveData,COMM_MODE_UNION);
    _pCommState->serviceNo = COMM_MODE_UNION;
    _pCommState->regulationNo = 0;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = SOLO_DEBUG_NO;
#endif
    // �X�e�[�g�̑J�ڂ̂��ߏ�����
    _CHANGE_STATE(_unionStart, 0);
}


//==============================================================================
/**
 * ��b���J�n�����̂Ŏq�@�ڑ�
 * @param   �ڑ�����e�@index
 * @retval  none
 */
//==============================================================================

void CommStateUnionConnectStart(int index)
{
    _pCommState->connectIndex = index;
    _pCommState->reConnectTime = _RETRY_COUNT_UNION;
    WHParentConnectPause(TRUE);  // �e�@�ɂ͂Ȃ�Ȃ�
    WirelessIconEasy();
    _CHANGE_STATE(_unionForceConnectStart, 0);
}

//==============================================================================
/**
 * �q�@�ڑ��ɐ����������ǂ���
 * @param   none
 * @retval  �ڑ������O�@�������P�@���s���|�P
 */
//==============================================================================

int CommStateIsUnionConnectSuccess(void)
{
    u32 stateAddr;

    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return -1;
    }
    stateAddr = (u32)_pCommState->state;
    if(stateAddr == (u32)_unionChildConnectSuccess){
        return 1;
    }
    if(stateAddr == (u32)_unionChildConnectFailed){
        return -1;
    }
    return 0;
}

//==============================================================================
/**
 * �e�@�ڑ��ɐ����������ǂ���
 * @param   none
 * @retval  �ڑ����Ȃ�TRUE
 */
//==============================================================================

BOOL CommStateIsUnionParentConnectSuccess(void)
{
    u32 stateAddr;

    if(_pCommState==NULL){  // ���łɏI�����Ă��� �������͏��������܂�
        return FALSE;
    }
    stateAddr = (u32)_pCommState->state;
    if(stateAddr == (u32)_unionParentConnect){
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * �r�[�R�����W���ĊJ����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionBconCollectionRestart(void)
{
	OS_TPrintf( "���X�^�[�g���܂����I\n" );

    WHParentConnectPause(FALSE);

	if( !(_pCommState->bDisconnectError == TRUE && _pCommState->bErrorAuto == TRUE) ||
		_pCommState->serviceNo == COMM_MODE_UNION ){
	   CommStateSetErrorCheck(FALSE,FALSE);
	}

    WirelessIconEasyEnd();
    _pCommState->serviceNo = COMM_MODE_UNION;
    _pCommState->bUnionPause = FALSE;
    WH_SetMaxEntry(_pCommState->limitNum);

    if(CommGetCurrentID() == COMM_PARENT_ID){
        WHParentConnectPauseSystem(TRUE);
        _CHANGE_STATE(_stateConnectChildEndWait, _EXIT_SENDING_TIME2);
    }
    else{
        _CHANGE_STATE(_stateConnectChildEnd, _EXIT_SENDING_TIME);
    }
}

//==============================================================================
/**
 * �r�[�R�����W���ĊJ���m�F����
 * @param   none
 * @retval  none
 */
//==============================================================================

BOOL CommStateUnionIsRestartSuccess(void)
{
    u32 stateAddr;

    if(_pCommState==NULL){
        return TRUE;  // 
    }
    stateAddr = (u32)_pCommState->state;
    if(stateAddr == (u32)_unionChildFinalize){
        return FALSE;
    }
    return TRUE;
}

//==============================================================================
/**
 * UNIONROOM���o��ꍇ�̒ʐM����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateExitUnion(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    OHNO_SP_PRINT(">> UNION��������ؒf\n");
    // �ؒf�X�e�[�g�Ɉڍs����  �����ɐ؂�Ȃ�
    
    _CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);
}

//==============================================================================
/**
 * �q�@�Ƃ��ĂȂ���\��Ȃ̂ŁA�e�@�ɂ͂Ȃ�Ȃ�
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateChildReserve(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    _pCommState->bUnionPause = TRUE;
    WHParentConnectPause(TRUE);
}

//==============================================================================
/**
 * UNIONROOM�ꎞ��~  �ĊJ��ReStart
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionPause(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    _pCommState->bUnionPause = TRUE;
    WHParentConnectPause(TRUE);
    _CHANGE_STATE(_unionParentPause, 0);
}

//==============================================================================
/**
 * UNIONROOM�A�v���P�[�V�����X�^�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionAppStart(void)
{
    _pCommState->serviceNo = COMM_MODE_UNION_APP;
}

//==============================================================================
/**
 * UNIONROOM�A�v���P�[�V�����G���h
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionAppEnd(void)
{
    _pCommState->serviceNo = COMM_MODE_UNION;
}

//==============================================================================
/**
 * �s�v�c�ʐM�Ƃ��Ă̒ʐM�����J�n
 * @param   pMyStatus  mystatus
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @param   regulationNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

void CommStateEnterMysteryParent(SAVEDATA* pSaveData, int serviceNo)
{
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,serviceNo);
//    _pCommState->serviceNo = serviceNo;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = 0;
#endif
    _CHANGE_STATE(_mysteryParentInit, 0);
}

//==============================================================================
/**
 * �s�v�c�ʐM�̎q�Ƃ��Ă̒ʐM�����J�n
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

void CommStateEnterMysteryChild(SAVEDATA* pSaveData, int serviceNo)
{
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,serviceNo);
//    _pCommState->serviceNo = serviceNo;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = 0;
#endif
    _CHANGE_STATE(_mysteryChildInit, 0);
}


//==============================================================================
/**
 * �ʐM�Ǘ��X�e�[�g�̏���
 * @param
 * @retval  none
 */
//==============================================================================

void CommStateCheckFunc(void)
{

    if(_pCommState){
        if(_pCommState->state != NULL){
            PTRStateFunc state = _pCommState->state;
            state();
        }
    }
    if(CommStateIsWifiConnect()){
        WirelessIconEasy_SetLevel(WM_LINK_LEVEL_3 - DWC_GetLinkLevel());
    }
    else if(CommMPIsInitialize()){
        WirelessIconEasy_SetLevel(WM_LINK_LEVEL_3 - WM_GetLinkLevel());
    }
}

//==============================================================================
/**
 * �ʐM�Ǘ��X�e�[�g�̕ύX
 * @param   state  �ς���X�e�[�g�̊֐�
 * @param   time   �X�e�[�g�ێ�����
 * @retval  none
 */
//==============================================================================

static void _changeState(PTRStateFunc state, int time)
{
    _pCommState->state = state;
    _pCommState->timer = time;
}

//==============================================================================
/**
 * �ʐM�Ǘ��X�e�[�g�̕ύX
 * @param   state  �ς���X�e�[�g�̊֐�
 * @param   time   �X�e�[�g�ێ�����
 * @retval  none
 */
//==============================================================================
#ifdef PM_DEBUG
static void _changeStateDebug(PTRStateFunc state, int time, int line)
{
    OHNO_PRINT("comm_state: %d\n",line);
    _changeState(state, time);
}
#endif

//==============================================================================
/**
 * �n���X�^�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underStart(void)
{
    void* pWork;

    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    if(!CommIsVRAMDInitialize()){
        return;  //
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    CommInfoInitialize(_pCommState->pSaveData, NULL);
    CommSetAloneMode(TRUE);
    CommEnableSendMoveData(); //commsystem�ɂ����Ă���L�[�f�[�^�̑��M������

    if(_pCommState->bParentOnly){
        if(CommParentModeInit(TRUE, _pCommState->bFirstParent, _PACKETSIZE_UNDERGROUND, TRUE))  {
            u32 rand = MATH_Rand32(&_pCommState->sRand, _PARENT_WAIT_TIME/2);
            _pCommState->bFirstParent = FALSE;
            _CHANGE_STATE(_underParentWait, _PARENT_WAIT_TIME/2+rand);
        }
    }
    else{
        // �܂��q�@�ɂȂ��Ă݂āA�e�@��T��  ���R�F�e�@�������炷�΂₭�Ȃ��邩��
        if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_UNDERGROUND)){
            if(_pCommState->bNotConnect){
                _CHANGE_STATE(_stateNone,0);
            }
            else{
                _CHANGE_STATE(_underChildPSearching, _CHILD_P_SEARCH_TIME*2);
            }
        }
    }
}

//==============================================================================
/**
 * ���Z�b�g��̍ăX�^�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildFInit(void)
{
    BOOL bSuccess;

    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    CommSetAloneMode(TRUE);  // ��ڑ����
    CommEnableSendMoveData(); //commsystem�ɂ����Ă���L�[�f�[�^�̑��M������

    if(_pCommState->bParentOnly){
        _CHANGE_STATE(_underParentInit, 0);
    }
    else{
        bSuccess = CommChildModeInit(FALSE, TRUE, _PACKETSIZE_UNDERGROUND);
        if(bSuccess){
            u32 rand = MATH_Rand32(&_pCommState->sRand, _CHILD_P_SEARCH_TIME*2);
            if(_pCommState->reConnectTime > 0){
                //OHNO_PRINT("�Đڑ����ԍl�� %d \n", _pCommState->reConnectTime);
                // �Đڑ����ԍl��
                rand = 2 * _CHILD_P_SEARCH_TIME * _pCommState->reConnectTime;
                _pCommState->reConnectTime = 0;
            }
            _CHANGE_STATE(_underChildPSearching, rand);
        }
    }
}

//==============================================================================
/**
 * �q�@�ăX�^�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildPInit(void)
{
    BOOL bSuccess;
    
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    bSuccess = CommChildModeInit(FALSE, FALSE, _PACKETSIZE_UNDERGROUND);
    if(bSuccess){
        u32 rand = MATH_Rand32(&_pCommState->sRand, _CHILD_P_SEARCH_TIME);
        _CHANGE_STATE(_underChildPSearching, _CHILD_P_SEARCH_TIME/2 + rand);
    }
}

//==============================================================================
/**
 * �q�@�ƂȂ��Đe�@��T����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildPSearching(void)
{
    int realParent;
    
    CommMPParentBconCheck();  // bcon�̌���

    if(_pCommState->bPauseFlg || _pCommState->bChildOnly){
        return;
    }
    
    realParent = CommMPGetFastConnectIndex();  //�Ȃ��������Ƃ�����l��������
    if(realParent != -1){
        _pCommState->connectIndex = realParent;  // �L��
        _CHANGE_STATE(_underChildForceConnect, _CHILD_P_SEARCH_TIME);
        return;
    }
    if(_pCommState->timer != 0){
        _pCommState->timer--;
        return;
    }
    realParent = CommMPGetNextConnectIndex();  //�Ȃ��������Ƃ�����l��������
    if(realParent != -1){
        _pCommState->connectIndex = realParent;  // �L��
        _CHANGE_STATE(_underChildForceConnect, _CHILD_P_SEARCH_TIME);
        return;
    }


    // �e�����Ȃ��ꍇ ���������e�ɂȂ�
    _CHANGE_STATE(_underChildFinalize, 0);
}


//==============================================================================
/**
 * �ڕW�ƂȂ�e�@�ɑ΂��Đڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildForceConnect(void)
{
    CommMPParentBconCheck();  // bcon�̌���
    if(CommMPGetParentConnectionNum(_pCommState->connectIndex)!=0){ // �e���������Ă���ꍇ
        if(CommChildIndexConnect(_pCommState->connectIndex)){  // �ڑ�����܂ŌĂё�����
            _CHANGE_STATE(_underChildConnecting, 100);
            return;
        }
    }
    if(CommIsError()){
        _CHANGE_STATE(_underChildFinalize, 0);
    }
    else if(_pCommState->timer!=0){
        _pCommState->timer--;
    }
    else{
        _CHANGE_STATE(_underChildFinalize, 0);
    }
}



//==============================================================================
/**
 * �q�@�ƂȂ��Đڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================
static void _underChildConnecting(void)
{
    if(CommIsConnect(CommGetCurrentID())){   // �������g���ڑ����Ă��邱�Ƃ��m�F�ł�����
        CommSystemReset();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
        CommSetAloneMode(FALSE);
        CommEnableSendMoveData();
        _CHANGE_STATE(_underChildConnect, 0);
        return;
    }
    
    if(CommIsError()){
        _CHANGE_STATE(_underChildFinalize, 0);
        return;
    }
    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    // ���Ԑ؂�
    _CHANGE_STATE(_underChildFinalize, 0);
}

//==============================================================================
/**
 * �q�@�I�� ���e�ɕς��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildFinalize(void)
{
    if(!CommMPSwitchParentChild()){
        return;
    }
    _CHANGE_STATE(_underParentInit, 0);
}

//==============================================================================
/**
 * �e�@�I���@�m���Őe�@�ɂȂ邩�q�@�ɂȂ邩���߂�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentFinalize(void)
{
    if(!CommMPSwitchParentChild()){
        return;
    }
    _CHANGE_STATE(_underChildPInit, 0);
}

//==============================================================================
/**
 * �e�@�J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentInit(void)
{
    MYSTATUS* pMyStatus;
    
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    // �e�@�ɂȂ��Ă݂�
    CommSetAloneMode(TRUE); // 
    if(CommParentModeInit(FALSE, _pCommState->bFirstParent, _PACKETSIZE_UNDERGROUND, TRUE))  {
        u32 rand = MATH_Rand32(&_pCommState->sRand, _PARENT_WAIT_TIME/2);
        _pCommState->bFirstParent = FALSE;
        _CHANGE_STATE(_underParentWait, _PARENT_WAIT_TIME/2+rand);
    }
}

//==============================================================================
/**
 * �e�@�ɂȂ�A�q�@���ڑ����Ă���̂�҂�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentWait(void)
{
   if(CommIsChildsConnecting()){   // �����ȊO���Ȃ�������e�@�Œ�
        _pCommState->bFirstParent = TRUE;  // �e�@�Ƃ��Čq�������̂Ńt���O��߂��Ă���
        _CHANGE_STATE(_underParentConnectInit, 0);
        return;
    }
    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    if(_pCommState->bParentOnly){
        return;
    }
    if( CommMPSwitchParentChild() ){
        _CHANGE_STATE(_underParentFinalize, _FINALIZE_TIME);
    }
}

//==============================================================================
/**
 * �e�@�Ƃ��Đڑ����������̂ŏ��������s��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentConnectInit(void)
{
    CommSetAloneMode(FALSE);
    CommMPSetNoChildError(TRUE);  // �q�@�����Ȃ��Ȃ�����Č������邽�߂�ERR�����ɂ���
    CommEnableSendMoveData();

    _CHANGE_STATE(_underParentConnect, 0);
}

//==============================================================================
/**
 * �e�@�Ƃ��Đڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentConnect(void)
{
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnect(void)
{
}


static void _underSBBoot(void)
{
    //  �o�g���t�@�N�g���[�ł̓��삪�ς���Ă��܂��s��̏C��
    //    if(_pCommState->serviceNo < COMM_MODE_BATTLE_SINGLE_WIFI){
    if(!CommStateIsWifiConnect()){
        if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
            return;
        }
        CommSetAloneMode(TRUE);  // ��ڑ����
        CommEnableSendMoveData(); //commsystem�ɂ����Ă���L�[�f�[�^�̑��M������
        CommMPStealth(TRUE);  // �閧���ɐؒf
    }
    else{
    }
    _CHANGE_STATE(_stateNone, 0);
}

//==============================================================================
/**
 * �閧��nOFFLINE�p�Ƀ��Z�b�g�������s��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underSBReset(void)
{
    if(!CommMPSwitchParentChild()){
        return;
    }
    CommSystemReset();         // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
    _CHANGE_STATE(_underSBBoot, 0);
}


//==============================================================================
/**
 * �C�N�j���[������������ʐM�N��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildOnline(void)
{
    if(!CommIsVRAMDInitialize()){
        return;  //
    }
    CommMPStealth(FALSE);

    CommSystemReset();         // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���

    _CHANGE_STATE(_underChildFInit, 0);
}

//==============================================================================
/**
 * ���Z�b�g�������s���A�q�@�Ƃ��čċN��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildReset(void)
{
    if(!CommMPSwitchParentChild()){
        return;
    }
    _CHANGE_STATE(_underChildFInit, 0);
}

//==============================================================================
/**
 * �e�@�Ƃ��ď��������s��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleParentInit(void)
{
    MYSTATUS* pMyStatus;
    
    if(!CommIsVRAMDInitialize()){
        return;
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    CommInfoInitialize(_pCommState->pSaveData, _pCommState->pReg);

    if(CommParentModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE,TRUE)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_battleParentSendName, 0);
    }
}


//==============================================================================
/**
 * �q�@�ҋ@���  �e�@��I��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleParentSendName(void)
{
    if(!CommIsConnect(CommGetCurrentID())){
        return;
    }
    _CHANGE_STATE(_battleParentWaiting, 0);
}

//==============================================================================
/**
 * �e�@�Ƃ��đҋ@��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleParentWaiting(void)
{
    if(!CommIsInitialize()){
        _CHANGE_STATE(_stateEnd,0);
    }
}

//==============================================================================
/**
 * �q�@�̏�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildInit(void)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    CommInfoInitialize(_pCommState->pSaveData, _pCommState->pReg);
    
    if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_battleChildBconScanning, 0);
    }
}

//==============================================================================
/**
 * �q�@�ҋ@���  �e�@�r�[�R�����W��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildBconScanning(void)
{
    CommMPParentBconCheck();
}

//==============================================================================
/**
 * �q�@�ҋ@���  �e�@�ɋ����炢��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildConnecting(void)
{
    CommMPParentBconCheck();
    if(CommChildIndexConnect(_pCommState->connectIndex)){  // �ڑ�����
        _CHANGE_STATE(_battleChildSendName, _SEND_NAME_TIME);
    }

}

//==============================================================================
/**
 * �q�@�ҋ@���  �e�@�ɏ��𑗐M
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildSendName(void)
{

    if(CommIsError()){
        //OHNO_PRINT("�G���[�̏ꍇ�߂�\n");
        _CHANGE_STATE(_battleChildReset, 0);
    }

    
    if(CommIsConnect(CommGetCurrentID()) && ( COMM_PARENT_ID != CommGetCurrentID())){
        _CHANGE_STATE(_battleChildWaiting, 0);
    }
}

//==============================================================================
/**
 * �q�@���Z�b�g   
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildReset(void)
{
    CommMPSwitchParentChild();
    _CHANGE_STATE(_battleChildReConnect, _FINALIZE_TIME);
}

//==============================================================================
/**
 * �q�@����������I�����Đڑ�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildReConnect(void)
{
    MYSTATUS* pMyStatus;

    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    if(!CommMPIsStateIdle()){  /// �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    if(CommChildModeInit(FALSE, TRUE, _PACKETSIZE_BATTLE)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_battleChildConnecting, _SEND_NAME_TIME);
    }
}


//==============================================================================
/**
 * �q�@�ҋ@���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildWaiting(void)
{
    if(!CommIsInitialize()){
        _CHANGE_STATE(_stateEnd,0);
    }
}

//==============================================================================
/**
 * �q�@����������I��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildReTry(void)
{
    CommMPSwitchParentChild();
    _CHANGE_STATE(_battleChildReInit, _FINALIZE_TIME);
}

//==============================================================================
/**
 * �q�@����������I�����ċN����������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _battleChildReInit(void)
{
    MYSTATUS* pMyStatus;

    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    if(!CommMPIsStateIdle()){  /// �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    if(CommChildModeInit(FALSE, TRUE, _PACKETSIZE_BATTLE)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_battleChildBconScanning, _SEND_NAME_TIME);
    }
}

//==============================================================================
/**
 * �������Ȃ��X�e�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateNone(void)
{
    // �Ȃɂ����Ă��Ȃ�
}

//==============================================================================
/**
 * @brief �G���[����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateConnectError(void)
{
}

//==============================================================================
/**
 * @brief  �I��������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateEnd(void)
{
    if(CommIsInitialize()){
        return;
    }
    _stateFinalize();
}

//==============================================================================
/**
 * @brief  �n�����E�����J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateUnderGroundConnectEnd(void)
{
    _CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);
}

//==============================================================================
/**
 * @brief  �����I�������J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateConnectAutoEnd(void)
{
    if(CommSendFixData(CS_AUTO_EXIT)){
        _CHANGE_STATE(_stateEnd, 0);
    }
}

//==============================================================================
/**
 * @brief   �ڑ��؂�ւ��̑O�ɐe�@�Ȃ�Α���̐ؒf���m�F����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateConnectChildEndWait(void)
{
    if(CommGetConnectNum() <= 1){  // ���������̐ڑ��ɂȂ�����
        WHParentConnectPauseSystem(FALSE);
        CommSystemResetDS();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
        _CHANGE_STATE(_unionChildFinalize, 0);
    }
    if(_pCommState->timer != 0){
        _pCommState->timer--;
        return;
    }
    // ����
    WHParentConnectPauseSystem(FALSE);
    CommSystemResetDS();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
    _CHANGE_STATE(_unionChildFinalize, 0);
}

//==============================================================================
/**
 * @brief   �ڑ��؂�ւ��̑O�Ɏq�@�Ȃ�ΐ؂ꂽ���ɏ�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateConnectChildEnd(void)
{
    if(!CommMPSwitchParentChild()){
        return;
    }
    CommSystemReset();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
    _CHANGE_STATE(_unionChildFinalize, 0);
}

//==============================================================================
/**
 * @brief  �I�������J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateConnectEnd(void)
{
    if(_pCommState->timer != 0){
        _pCommState->timer--;
    }
    if(!CommMPSwitchParentChild()){
        return;
    }
    if(_pCommState->timer != 0){
        return;
    }
    CommFinalize();
    _CHANGE_STATE(_stateEnd, 0);
}



//==============================================================================
/**
 * UNION�X�^�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionStart(void)
{
    void* pWork;

    if(!CommIsVRAMDInitialize()){
        return;  //
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    WH_SetMaxEntry(_pCommState->limitNum);
    CommInfoInitialize(_pCommState->pSaveData, NULL);
    
    // �܂��q�@�ɂȂ��Ă݂āA�e�@��T��   ���邮��񂵂�bcon�����߂�
    if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_UNION)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_unionChildSearching, _CHILD_P_SEARCH_TIME*2);
    }
}

//==============================================================================
/**
 * �q�@�ƂȂ��Đe�@��T����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionChildSearching(void)
{
    int realParent;

    CommMPParentBconCheck();  // bcon�̌���
    if(_pCommState->timer != 0){
        _pCommState->timer--;
        return;
    }

    if(!CommMPSwitchParentChild()){
        return;
    }
    _CHANGE_STATE(_unionParentInit, 0);
}

//==============================================================================
/**
 * �q�@�I�� ���e�ɕς��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionChildFinalize(void)
{
    if(!CommMPSwitchParentChild()){
        return;
    }
    _CHANGE_STATE(_unionParentInit, 0);
}

//==============================================================================
/**
 * �e�@�J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionParentInit(void)
{
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    // �e�@�ɂȂ��Ă݂�
    if(CommParentModeInit(FALSE, _pCommState->bFirstParent, _PACKETSIZE_UNION,TRUE))  {
        u32 rand = MATH_Rand32(&_pCommState->sRand, _PARENT_WAIT_TIME*2);
        CommSetTransmissonTypeDS();
        _pCommState->bFirstParent = FALSE;
        _CHANGE_STATE(_unionParentWait, 10000);
    }
}

//==============================================================================
/**
 * �e�@�ɂȂ�bcon���o
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionParentWait(void)
{
    if(CommMPIsParentBeaconSent()){  // �r�[�R���𑗂�I�������q�@�ɐ؂�ւ��
    }
    else{
        if(CommIsChildsConnecting()){   // �����ȊO���Ȃ�������e�@�Œ�
            _pCommState->bFirstParent = TRUE;  // �e�@�Ƃ��Čq�������̂Ńt���O��߂��Ă���
            WirelessIconEasy();
            _CHANGE_STATE(_unionParentConnect, 0);
            return;
        }
        if(_pCommState->timer!=0){
            _pCommState->timer--;
            return;
        }
    }
    if( CommMPSwitchParentChild() ){
        _CHANGE_STATE(_unionChildRestart, 0);
    }
}

//==============================================================================
/**
 * �q�@�ăX�^�[�g
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionChildRestart(void)
{
    u32 rand;
    
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    // ���x�̓r�[�R�����c�����܂�
    if(CommChildModeInit(FALSE, FALSE, _PACKETSIZE_UNION)){
        CommSetTransmissonTypeDS();
        rand = MATH_Rand32(&_pCommState->sRand, _CHILD_P_SEARCH_TIME);
        _CHANGE_STATE(_unionChildSearching, rand);
    }
}

//==============================================================================
/**
 * �b�������J�n�ŁA�q�@�ҋ@��ԂɂȂ�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionForceConnectStart(void)
{
    if(!CommMPSwitchParentChild()){  // ���̏�Ԃ��I��
        return;
    }
    _CHANGE_STATE(_unionForceConnectStart2, 0);
}


static void _unionForceConnectStart2(void)
{
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    // �q�@�ɂȂ�
    if(CommChildModeInit(FALSE, FALSE, _PACKETSIZE_UNION)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_unionForceConnect, 100);
    }
}

//==============================================================================
/**
 * �q�@�ƂȂ��ċ����ڑ�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionForceConnect(void)
{
    if(CommMPGetParentConnectionNum(_pCommState->connectIndex)!=0){ // �e���������Ă���ꍇ
        if(CommChildIndexConnect(_pCommState->connectIndex)){  // �ڑ�����܂ŌĂё�����
            _CHANGE_STATE(_unionChildConnecting, 100);
            return;
        }
    }
    if(CommIsError()){
        _CHANGE_STATE(_unionChildReset, 0);
    }
    else if(_pCommState->timer!=0){
        _pCommState->timer--;
    }
    else{
        _CHANGE_STATE(_unionChildReset, 0);
    }
}

//==============================================================================
/**
 * �q�@�ƂȂ��Đڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================
#if AFTER_MASTER_070420_GF_COMM_FIX
#define _NEGOTIATION_TIME (120)
#endif
static void _unionChildConnecting(void)
{
    if(CommIsError()){
        _CHANGE_STATE(_unionChildReset, 0);
        return;
    }
    if(CommIsConnect(CommGetCurrentID())){   // �������g���ڑ����Ă��邱�Ƃ��m�F�ł�����
        _pCommState->negotiation = _NEGOTIATION_CHECK;
#if AFTER_MASTER_070420_GF_COMM_FIX
        _CHANGE_STATE(_unionChildNegotiation, _NEGOTIATION_TIME);
#else
        CommSendFixSizeData(CS_COMM_NEGOTIATION, _negotiationMsg);
        _CHANGE_STATE(_unionChildNegotiation, 120);
#endif
        return;
    }
    
    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    // ���Ԑ؂�
    _CHANGE_STATE(_unionChildReset, 0);
}

//==============================================================================
/**
 * �q�@�ƂȂ��Đڑ���  �e�@�F�ؑ҂�
 * @param   none
 * @retval  none
 */
//==============================================================================
static void _unionChildNegotiation(void)
{
    if(CommIsError()){
        _CHANGE_STATE(_unionChildReset, 0);
        return;
    }
    if(_pCommState->negotiation == _NEGOTIATION_NG){
        _CHANGE_STATE(_unionChildConnectFailed, 0);
        return;
    }
    if(_pCommState->negotiation == _NEGOTIATION_OK){
        CommInfoSendPokeData();
        _CHANGE_STATE(_unionChildConnectSuccess, 0);
        return;
    }
#if AFTER_MASTER_070420_GF_COMM_FIX
    if(_pCommState->timer > (_NEGOTIATION_TIME-10)){
        CommSendFixSizeData(CS_COMM_NEGOTIATION, _negotiationMsg);
    }
#endif
    
    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }
    _CHANGE_STATE(_unionChildReset, 0);
}



static void _unionChildConnectSuccess(void)
{
    if(CommIsError()){
        _CHANGE_STATE(_unionChildReset, 0);
        return;
    }
}

static void _unionChildConnectFailed(void)
{
}

static void _unionChildReset(void)
{
    _pCommState->negotiation = _NEGOTIATION_CHECK;
    if(!CommMPSwitchParentChild()){
        return;
    }

    if(_pCommState->reConnectTime != 0){
        _pCommState->reConnectTime--;
        _CHANGE_STATE(_unionForceConnectStart2, 0);
    }
    else{  // �Ȃ���Ȃ������̂Ŏ��s�X�e�[�g��
        _CHANGE_STATE(_unionChildConnectFailed, 0);
    }
}

//==============================================================================
/**
 * �e�Ƃ��Đڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================
static void _unionParentConnect(void)
{
    if(!CommIsChildsConnecting()){   // �����ȊO���Ȃ����ĂȂ��΂����������
        if(!CommStateGetErrorCheck()){
            if( CommMPSwitchParentChild() ){
                _CHANGE_STATE(_unionChildRestart, 0);
            }
        }
    }
    else{
        if(CommGetCurrentID() == COMM_PARENT_ID){
            CommInfoSendArray_ServerSide();  // �q�@����₢���킹����������info�𑗐M
        }
    }
    if(CommIsError()){
        if(!CommStateGetErrorCheck()){
            _CHANGE_STATE(_unionChildReset, 0);
            return;
        }
    }
}

//==============================================================================
/**
 * �e�@�̂܂܈ꎞ��~
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _unionParentPause(void)
{
    u32 rand;
    
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    // �Ȃ���Ȃ��e�@�ɂȂ�
    if(CommParentModeInit(FALSE, _pCommState->bFirstParent, _PACKETSIZE_UNION, FALSE))  {
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_stateNone,0);
    }
}




static void _pictureBoardChangeParent(void);
static void _pictureBoardInitParent(void);
static void _pictureBoardParentWait(void);

static void _pictureBoardChangeChild(void);
static void _pictureBoardInitChild(void);
static void _pictureBoardForceConnect(void);
static void _pictureBoardChildConnecting(void);

//==============================================================================
/**
 * ���G�������[�h�Ɍq���Ȃ���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionPictureBoardChange(void)
{
    _pCommState->serviceNo = COMM_MODE_PICTURE;
    if(CommGetCurrentID() == COMM_PARENT_ID){
        u8 bDSMode = FALSE;
        CommSendFixSizeData(CS_DSMP_CHANGE, &bDSMode);  //�ړ����[�h�ɕύX����
//        _pCommState->limitNum = COMM_MODE_PICTURE_NUM_MAX+1;
    }
    else{
        u8 bDSMode = FALSE;
        CommSendFixSizeData(CS_DSMP_CHANGE, &bDSMode);  //�ړ����[�h�ɕύX����
    }
}

//==============================================================================
/**
 * ���G�������[�h�q�@�Ƃ��Čq��
 * @param   �ڑ�����e�@index
 * @retval  none
 */
//==============================================================================

void CommStateUnionPictureBoardChild(int index)
{
    _pCommState->connectIndex = index;
    _pCommState->reConnectTime = _RETRY_COUNT_UNION;
    _CHANGE_STATE(_pictureBoardChangeChild, 0);
}

//==============================================================================
/**
 * ���R�[�h�R�[�i�[�̏�Ԃɐ؂�ւ���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionRecordCornerChange(void)
{
    _pCommState->serviceNo = COMM_MODE_RECORD;
}

//==============================================================================
/**
 * ���R�[�h�R�[�i�[�q�@�Ƃ��Čq��
 * @param   �ڑ�����e�@index
 * @retval  none
 */
//==============================================================================

void CommStateUnionRecordCornerChild(int index)
{
    _pCommState->connectIndex = index;
    _pCommState->serviceNo = COMM_MODE_RECORD;
    _pCommState->reConnectTime = _RETRY_COUNT_UNION;
    WirelessIconEasy();
    _CHANGE_STATE(_unionForceConnectStart, 0);
}

//==============================================================================
/**
 * ���邮������̏�Ԃɐ؂�ւ���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateUnionGuru2Change(void)
{
    _pCommState->serviceNo = COMM_MODE_GURU2;
}

//==============================================================================
/**
 * ���邮������q�@�Ƃ��Čq��
 * @param   �ڑ�����e�@index
 * @retval  none
 */
//==============================================================================

void CommStateUnionGuru2Child(int index)
{
    _pCommState->connectIndex = index;
    _pCommState->serviceNo = COMM_MODE_GURU2;
    _pCommState->reConnectTime = _RETRY_COUNT_UNION;
    WirelessIconEasy();
    _CHANGE_STATE(_unionForceConnectStart, 0);
}

//==============================================================================
/**
 * ���G����state�Ɉڍs�������ǂ����̊m�F
 * @param   none
 * @retval  ���G����state�Ɉڍs�����ꍇTRUE
 */
//==============================================================================

BOOL CommStateIsUnionPictureBoardState(void)
{
    u32 stateAddr = (u32)_pCommState->state;

    if(CommIsTransmissonDSType()){
        return FALSE;
    }
    if(stateAddr == (u32)_unionParentConnect){
        return TRUE;
    }
    if(stateAddr == (u32)_unionChildConnectSuccess){
        return TRUE;
    }
    return FALSE;
}


static void _pictureBoardChangeParent(void)
{
    if( CommMPSwitchParentChild() ){
        _CHANGE_STATE(_pictureBoardInitParent, 0);
    }
}

static void _pictureBoardInitParent(void)
{
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    _pCommState->serviceNo = COMM_MODE_PICTURE;
    if(CommParentModeInit(FALSE, FALSE, _PACKETSIZE_UNION,TRUE)) {
        CommSetTransmissonTypeMP();
        _CHANGE_STATE(_pictureBoardParentWait, 0);
    }
}


static void _pictureBoardParentWait(void)
{
    if(CommIsChildsConnecting()){   // �����ȊO���Ȃ�������e�@�Œ�
        _pCommState->bFirstParent = TRUE;  // �e�@�Ƃ��Čq�������̂Ńt���O��߂��Ă���
        CommInfoSendPokeData();
        _CHANGE_STATE(_unionParentConnect, 0);
    }
}


static void _pictureBoardChangeChild(void)
{
    if( CommMPSwitchParentChild() ){
        _CHANGE_STATE(_pictureBoardInitChild, 0);
    }
}

static void _pictureBoardInitChild(void)
{
    if(!CommMPIsStateIdle()){  // �I��������������ƏI����Ă��邱�Ƃ��m�F
        return;
    }
    _pCommState->serviceNo = COMM_MODE_PICTURE;
    if(CommChildModeInit(FALSE, FALSE, _PACKETSIZE_UNION)){
        CommSetTransmissonTypeMP();
        _CHANGE_STATE(_pictureBoardForceConnect, 100);
    }
}


//==============================================================================
/**
 * �q�@�ƂȂ��ċ����ڑ�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _pictureBoardForceConnect(void)
{
    if(CommMPGetParentConnectionNum(_pCommState->connectIndex)!=0){ // �e���������Ă���ꍇ
        if(CommChildIndexConnect(_pCommState->connectIndex)){  // �ڑ�����܂ŌĂё�����
            _CHANGE_STATE(_pictureBoardChildConnecting, 100);
            return;
        }
    }
    if(CommIsError() || (_pCommState->timer == 0)){
        _pCommState->reConnectTime--;
        if(_pCommState->reConnectTime==0){
            _CHANGE_STATE(_unionChildConnectFailed, 0);
        }
        else{
            _CHANGE_STATE(_pictureBoardChangeChild, 0);
        }
    }
    else if(_pCommState->timer!=0){
        _pCommState->timer--;
    }
}

//==============================================================================
/**
 * �q�@�ƂȂ��Đڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================
static void _pictureBoardChildConnecting(void)
{
    if(_pCommState->timer > 90){
        _pCommState->timer--;
        return;
    }
    
    if(CommIsError()){
        _pCommState->reConnectTime--;
        if(_pCommState->reConnectTime==0){
            _CHANGE_STATE(_unionChildConnectFailed, 0);
        }
        else{
            _CHANGE_STATE(_pictureBoardChangeChild, 0);
        }
        return;
    }
    
    if(CommIsConnect(CommGetCurrentID())){   // �������g���ڑ����Ă��邱�Ƃ��m�F�ł�����
        _CHANGE_STATE(_unionChildConnectSuccess, 0);
        return;
    }
    if(_pCommState->timer!=0){
        _pCommState->timer--;
        return;
    }

    _pCommState->reConnectTime--;
    if(_pCommState->reConnectTime==0){
        _CHANGE_STATE(_unionChildConnectFailed, 0);
    }
    else{
        // ���Ԑ؂�
        _CHANGE_STATE(_pictureBoardChangeChild, 0);
    }
}

//==============================================================================
/**
 * �l�S�V�G�[�V�����p�R�[���o�b�N CS_COMM_NEGOTIATION
 * @param   callback�p����
 * @retval  none
 */
//==============================================================================

void CommRecvNegotiation(int netID, int size, void* pData, void* pWork)
{
    int i;
    u8* pMsg = pData;
    BOOL bMatch = TRUE;

    if(CommGetCurrentID() != COMM_PARENT_ID){  // �e�@�̂ݔ��f�\
        return;
    }
    bMatch = TRUE;
    for(i = 0; i < sizeof(_negotiationMsg); i++){
        if(pMsg[i] != _negotiationMsg[i]){
            bMatch = FALSE;
            break;
        }
    }
    if(bMatch  && (!_pCommState->bUnionPause)){   // �q�@����ڑ��m�F������
//        if(CommGetConnectNum() <= _pCommState->limitNum){  // �w��ڑ��l����艺��邱��
            _negotiationMsgReturnOK[0] = netID;
            CommSendFixSizeData_ServerSide(CS_COMM_NEGOTIATION_RETURN, _negotiationMsgReturnOK);
            return;
//        }
    }
    _negotiationMsgReturnNG[0] = netID;
    CommSendFixSizeData_ServerSide(CS_COMM_NEGOTIATION_RETURN, _negotiationMsgReturnNG);
}

//==============================================================================
/**
 * �l�S�V�G�[�V�����p�R�[���o�b�N CS_COMM_NEGOTIATION_RETURN
 * @param   callback�p����
 * @retval  none
 */
//==============================================================================

void CommRecvNegotiationReturn(int netID, int size, void* pData, void* pWork)
{
    u8 id;
    int i;
    u8* pMsg = pData;
    BOOL bMatch = TRUE;

    for(i = 1; i < sizeof(_negotiationMsgReturnOK); i++){
        if(pMsg[i] != _negotiationMsgReturnOK[i]){
            bMatch = FALSE;
            break;
        }
    }
    if(bMatch){   // �e�@����ڑ��F�؂�����
        id = pMsg[0];
        if(id == CommGetCurrentID()){
            _pCommState->negotiation = _NEGOTIATION_OK;
        }
        return;
    }
    bMatch = TRUE;
    for(i = 1; i < sizeof(_negotiationMsgReturnNG); i++){
        if(pMsg[i] != _negotiationMsgReturnNG[i]){
            bMatch = FALSE;
            break;
        }
    }
    if(bMatch){   // �e�@����ڑ��ے肪����
        id = pMsg[0];
        if(id == (u8)CommGetCurrentID()){
            _pCommState->negotiation = _NEGOTIATION_NG;
        }
        return;
    }
}

//==============================================================================
/**
 * �l�S�V�G�[�V�����p�R�[���o�b�N
 * @param   callback�p����
 * @retval  none
 */
//==============================================================================

int CommRecvGetNegotiationSize(void)
{
    return sizeof(_negotiationMsg);
}


//==============================================================================
/**
 * �����Ă���l���𐧌�����
 * @param   �l��
 * @retval  none
 */
//==============================================================================

void CommStateSetLimitNum(int num)
{
    if(_pCommState){
        _pCommState->limitNum = num;
        WH_SetMaxEntry(num);
    }
}


//==============================================================================
/**
 * �q�@�ҋ@���  �e�@�r�[�R�����W��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _poketchBconScanning(void)
{
    CommMPParentBconCheck();
}

//==============================================================================
/**
 * �|�P�b�`�q�@�̏�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _poketchChildInit(void)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    CommInfoInitialize(_pCommState->pSaveData, NULL);
    
    if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_POKETCH)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_poketchBconScanning, 0);
    }
}

//==============================================================================
/**
 * �ۂ������Ƃ��Ă̒ʐM�����J�n�i�q�@��Ԃ̂݁j
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

void CommStateEnterPockchChild(SAVEDATA* pSaveData)
{
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_POKETCH );
    _commStateInitialize(pSaveData,COMM_MODE_POKETCH);
//    _pCommState->serviceNo = COMM_MODE_POKETCH;
    _pCommState->regulationNo = 0;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = 0;
#endif
    _CHANGE_STATE(_poketchChildInit, 0);
}

//==============================================================================
/**
 * �ۂ������Ƃ��Ă̏I������
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateExitPoketch(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    // �ؒf�X�e�[�g�Ɉڍs����  �����ɐ؂�Ȃ�
    _CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);
}

//==============================================================================
/**
 * �ʐM�T�[�`���[�ɂ����ď����WSTATE�Ȃ̂��ǂ����Ԃ�
 * @param   none
 * @retval  ���W���Ȃ�TRUE
 */
//==============================================================================

BOOL CommIsPoketchSearchingState(void)
{
    int i;
    u32 funcTbl[]={
        (u32)_poketchBconScanning,
        0,
    };
    u32 stateAddr = (u32)_pCommState->state;

    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return FALSE;
    }
    for(i = 0; funcTbl[i] != 0; i++ ){
        if(stateAddr == funcTbl[i]){
            return TRUE;
        }
    }
    return FALSE;
}


//==============================================================================
/**
 * PARTY�T�[�`callback
 * @param   ggid
 * @retval  none
 */
//==============================================================================

static void _callbackGGIDScan(u32 ggid,int serviceNo)
{
    switch(ggid){
      case _MYSTERY_GGID:
        if(serviceNo == COMM_MODE_MYSTERY){
            _pCommState->partyGameBit |= PARTYGAME_MYSTERY_BCON;
        }
        break;
      case _BCON_DOWNLOAD_GGID:
        _pCommState->partyGameBit |= PARTYGAME_MYSTERY_BCON;
        break;
      case _RANGER_GGID:
        _pCommState->partyGameBit |= PARTYGAME_RANGER_BCON;
        break;
      case _WII_GGID:
        _pCommState->partyGameBit |= PARTYGAME_WII_BCON;
        break;
    }
}

//==============================================================================
/**
 * PARTY�T�[�`�q�@�̏�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _partySearchChildMain(void)
{
    if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_PARTY)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_poketchBconScanning, 0);
    }
}

//==============================================================================
/**
 * PARTY�T�[�`�q�@�̏�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _partySearchChildInit(void)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    CommMPInitialize(_pCommState->pMyStatus, FALSE);
    CommInfoInitialize(_pCommState->pSaveData, NULL);
    WHSetGGIDScanCallback(_callbackGGIDScan);
    
    _CHANGE_STATE(_partySearchChildMain, 0);
}

//==============================================================================
/**
 * �p�[�e�B�[�Q�[�������̒ʐM�����J�n�i�q�@��Ԃ̂݁j
 * @param   SAVEDATA  savedata
 * @retval  none
 */
//==============================================================================

void CommStateEnterPartyGameScanChild(SAVEDATA* pSaveData)
{
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_PARTY );
    _commStateInitialize(pSaveData,COMM_MODE_PARTY);
    _pCommState->regulationNo = 0;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = 0;
#endif
    _CHANGE_STATE(_partySearchChildInit, 0);
}

//==============================================================================
/**
 * �p�[�e�B�[�Q�[���T�[�`�̏I������
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateExitPartyGameScan(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    // �ؒf�X�e�[�g�Ɉڍs����  �����ɐ؂�Ȃ�
    _CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);
}

//==============================================================================
/**
 * �E�����r�[�R����BIT��Ԃ�
 * @param   none
 * @retval  none
 */
//==============================================================================

u8 CommStateGetPartyGameBit(void)
{
  return _pCommState->partyGameBit;
}

//==============================================================================
/**
 * ���Z�b�g�I������
 * @param   none
 * @retval  none
 */
//==============================================================================

BOOL CommStateExitReset(void)
{
    CommSystemShutdown();
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return TRUE;
    }
    if( (_pCommState->serviceNo == COMM_MODE_DPW_WIFI) || 
		(_pCommState->serviceNo == COMM_MODE_FUSIGI_WIFI) ||
		(_pCommState->serviceNo == COMM_MODE_EMAIL_WIFI) ){
        mydwc_Logout();
        return TRUE;
    }
    else if(CommStateIsWifiConnect()){
        OHNO_SP_PRINT(" ���O�A�E�g����\n");
		if( _pCommState->serviceNo == COMM_MODE_LOBBY_WIFI ){
			_CHANGE_STATE( _wifiLobbyLogout, 0 );
		}else{
	        _CHANGE_STATE(_stateWifiLogout,0);  // ���O�A�E�g����
		}
    }
    else{
        _CHANGE_STATE(_underSBReset, 0);  // �G���[�I���̏ꍇRESET����
    }
    return FALSE;
}

//==============================================================================
/**
 * �s�v�c�ʐM�e�@�Ƃ��ď��������s��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _mysteryParentInit(void)
{
    MYSTATUS* pMyStatus;
    
    if(!CommIsVRAMDInitialize()){
        return;
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    CommInfoInitialize(_pCommState->pSaveData, NULL);

    if(CommParentModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE,TRUE)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_battleParentSendName, 0);
    }
}

//==============================================================================
/**
 * �s�v�c�ʐM�q�@�̏�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _mysteryChildInit(void)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    CommMPInitialize(_pCommState->pMyStatus, TRUE);
    CommInfoInitialize(_pCommState->pSaveData, NULL);
    
    if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE)){
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_battleChildBconScanning, 0);
    }
}




//---------------------wifi


//==============================================================================
/**
 * �G���[�������������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleError(void){}

static void _wifiBattleTimeout(void)
{
   int ret;
    ret = mydwc_stepmatch(FALSE);

	if( ret < 0 ){
        // �G���[�����B
        _CHANGE_STATE(_wifiBattleError, 0);

	// �^�C���A�E�g�G���[���A���Ă��Ă�Ƃ���
	// ������WiFi�G���[���`�F�b�N����
    }else if( ret == STEPMATCH_TIMEOUT ){ 
#if 0   // ���̏�����dwc_rap���ɔ��f�����܂���   k.ohno 2008.05.23
		// �^�C���A�E�g�ɑJ�ڂ��Ă���ꍇ�A
		// �ʏ��WiFi�G���[�`�F�b�N���s���Ă��Ȃ��\���̂ق��������A
		// ���ׁ̈A�����Ń`�F�b�N����
		ret = mydwc_HandleError();
		if( ret < 0 ){
			// �G���[�����B
			_CHANGE_STATE(_wifiBattleError, 0);
		}
#endif
	}
}

static void _wifiBattleDisconnect(void){}

static void _wifiBattleFailed(void){}

static int _wifiLinkLevel(void)
{
    return WM_LINK_LEVEL_3 - DWC_GetLinkLevel();
}

//==============================================================================
/**
 * �ڑ���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleConnect(void)
{
    int ret,errCode;

    CommSetWifiConnect(TRUE);

    ret = mydwc_stepmatch(FALSE);

    if((ret >= DWC_ERROR_FRIENDS_SHORTAGE) && (STEPMATCH_SUCCESS > ret)){
        _CHANGE_STATE(_wifiBattleFailed, 0);
    }
    else if( ret < 0 ){
        // �G���[�����B
        // �o�g���ʐM�s��̃e�X�g�̂���ASSERT�ǉ�    81010 91010�͂��̂܂܏������܂��̂� ASSERT���͂����܂� 08/06/10
//        GF_ASSERT_MSG(0,"err%d %d %d",_pCommState->aError.errorCode,_pCommState->aError.errorType,_pCommState->aError.errorRet);
        _CHANGE_STATE(_wifiBattleError, 0);
    }		
    else if( ret == STEPMATCH_TIMEOUT ) {     // �^�C���A�E�g�i���肩��P�O�b�ȏチ�b�Z�[�W���͂��Ȃ��j
//        GF_ASSERT_MSG(0,"timeout");
        _CHANGE_STATE(_wifiBattleTimeout, 0);
    }
    else if(ret == STEPMATCH_DISCONNECT){
        if(_pCommState->bDisconnectError){
            _CHANGE_STATE(_wifiBattleError, 0);
        }
        else{
            _CHANGE_STATE(_wifiBattleDisconnect, 0);
        }
    }
    else if(ret == STEPMATCH_CANCEL){
        if(_pCommState->bDisconnectError){
            _CHANGE_STATE(_wifiBattleError, 0);
        }
        else{
            _CHANGE_STATE(_wifiBattleDisconnect, 0);
        }
	}


    if(_pCommState->bDisconnectError){
        if(_pCommState->nowConnectNum != CommGetConnectNum()){
            _CHANGE_STATE(_wifiBattleError, 0);
        }
    }
}

//==============================================================================
/**
 * �G���[�R�[�h��ێ�����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateSetWifiError(int code, int type, int ret)
{
    // �G���[�����B	
    int errorcode;
    if(_pCommState){
        if( (code == ERRORCODE_HEAP) || (code == ERRORCODE_0)){
            errorcode = code;
        }
        else {
            errorcode = -code;
        }
        _pCommState->aError.errorCode = errorcode;
        _pCommState->aError.errorType = type;
        _pCommState->aError.errorRet = ret;
    }
}


//==============================================================================
/**
 * �ΐ푊���T����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleMaching(void)
{
    int ret = mydwc_stepmatch( 0 );

    if((ret >= DWC_ERROR_FRIENDS_SHORTAGE) && (STEPMATCH_SUCCESS > ret)){
        _CHANGE_STATE(_wifiBattleFailed, 0);
    }
    else if( ret < 0 ){
        //_errcodeConvert(ret);
        _CHANGE_STATE(_wifiBattleError, 0);
    }
    else if ( ret == STEPMATCH_SUCCESS )  {
        OS_TPrintf("�ΐ푊�肪������܂����B\n");
        _CHANGE_STATE(_wifiBattleConnect, 0);
    }
    else if ( ret == STEPMATCH_CANCEL ){
        OS_TPrintf("�L�����Z�����܂����B\n");	
        _CHANGE_STATE(_wifiBattleDisconnect, 0);
    }
    else if( ret == STEPMATCH_FAIL){
        _CHANGE_STATE(_wifiBattleFailed, 0);
    }
    else if( ret == STEPMATCH_DISCONNECT){
//        _CHANGE_STATE(_wifiBattleError, 0);
        _CHANGE_STATE(_wifiBattleDisconnect, 0);
    }
}



//==============================================================================
/**
 * �L�����Z����  �e����߂鎞��؂�ւ��鎞�Ɏg�p
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleCanceling(void)
{

    int ret = mydwc_stepmatch( 1 );  // �L�����Z����
				
    if( ret < 0 ){
        // �G���[�����B
        //_errcodeConvert(ret);
        _CHANGE_STATE(_wifiBattleError, 0);
    }
    else{
        int ret = mydwc_startgame( _pCommState->wifiTargetNo,CommLocalGetServiceMaxEntry( _pCommState->serviceNo ) + 1 ,FALSE);
        switch(ret){
          case DWCRAP_STARTGAME_OK:    //�ŏ���VCT�҂���ԂɂȂ�
            CommSystemReset();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
            if( _pCommState->wifiTargetNo < 0 ){
                OS_TPrintf("�Q�[���Q���҂��W���܂��B\n");
            } else {
                OS_TPrintf(" %d�Ԗڂ̗F�B�ɐڑ����܂��B\n", _pCommState->wifiTargetNo);	        
            }
            _CHANGE_STATE(_wifiBattleMaching, 0);
            break;
          case DWCRAP_STARTGAME_NOTSTATE:
          case DWCRAP_STARTGAME_RETRY:
            break;
          case DWCRAP_STARTGAME_FAILED:
            _CHANGE_STATE(_wifiBattleError, 0);
            break;
        }
    }
}


//==============================================================================
/**
 * �Q�[�����J�n����B// 2006.4.13 �g���ǉ�
 * @param   target: ���Ȃ�e�A�O�ȏ�Ȃ�Ȃ��ɂ����e�@�̔ԍ�
 * @retval  1. �����@�@0. ���s
 */
//==============================================================================
int CommWifiBattleStart( int target )
{
    if( _pCommState->state != _wifiBattleMaching ) return 0;

    mydwc_setReceiver( CommRecvParentCallback, CommRecvCallback );

    _pCommState->wifiTargetNo = target;
    _CHANGE_STATE(_wifiBattleCanceling,0);  // ���̏�Ԃ�j��
    return 1;
}

//==============================================================================
/**
 * �S�l�p�Q�[�����J�n����
 * @param   target: ���Ȃ�e�A�O�ȏ�Ȃ�Ȃ��ɂ����e�@�̔ԍ�
 * @retval  1. �����@�@0. ���s
 */
//==============================================================================
int CommWifiPofinStart( int target )
{
    if( _pCommState->state != _wifiBattleMaching ) return 0;

	_pCommState->serviceNo = COMM_MODE_WIFI_POFIN;
    mydwc_setReceiver( CommRecvOtherCallback, CommRecvOtherCallback );
    _pCommState->wifiTargetNo = target;
    _CHANGE_STATE(_wifiBattleCanceling,0);  // ���̏�Ԃ�j��
    return 1;
}


//==============================================================================
/**
 * �}�b�`���O�������������ǂ����𔻒�
 * @param   none
 * @retval  1. �����@�@0. �ڑ���   2. �G���[��L�����Z���Œ��f
 */
//==============================================================================
int CommWifiIsMatched(void)
{
	if( _pCommState->state == _wifiBattleMaching ) return 0;
	if( _pCommState->state == _wifiBattleConnect ) return 1;
    if(  _pCommState->state == _wifiBattleTimeout ) return 3;
    if(  _pCommState->state == _wifiBattleDisconnect ) return 4;
    if(  _pCommState->state == _wifiBattleFailed ) return 5;

	// ���r�[�̂Ƃ�
	if( _pCommState->state == _wifiLobbyP2PMatchWait ) return 0;
	if( _pCommState->state == _wifiLobbyP2PMatch ) return 1;
    if(  _pCommState->state == _wifiLobbyTimeout ) return 3;
	
//	if( _pCommState->state == _wifiBattleError ) return 5;
    return 2;
}

//==============================================================================
/**
 * �e�@�Ƃ��đҋ@��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleLogin(void)
{
    int ret;
	int err;

    mydwc_setReceiver( CommRecvParentCallback, CommRecvCallback );

    ret = mydwc_startgame( -1,4,TRUE );
    switch(ret){
      case DWCRAP_STARTGAME_OK:    //�ŏ���VCT�҂���ԂɂȂ�
        _pCommState->bWifiDisconnect = FALSE;
        CommSystemReset();   // ���܂ł̒ʐM�o�b�t�@���N���[���ɂ���
        OS_TPrintf("VCT�Q���҂��W���܂��B\n");
        _CHANGE_STATE(_wifiBattleMaching, 0);
        break;
      case DWCRAP_STARTGAME_NOTSTATE:
      case DWCRAP_STARTGAME_RETRY:
#ifdef PM_DEBUG
		{
			int dwc_state =DWC_GetState(); 
			OHNO_PRINT( "DWC_GetState == %d\n", dwc_state );
		}
#endif
        break;
      case DWCRAP_STARTGAME_FAILED:
        _CHANGE_STATE(_wifiBattleError, 0);
        break;
      case DWCRAP_STARTGAME_FIRSTSAVE:
        return;
    }


	// �G���[�`�F�b�N���s��
	err = mydwc_HandleError();
	if( err < 0 ){
        _CHANGE_STATE(_wifiBattleError, 0);
	}else if( err == ERRORCODE_HEAP ){
        _CHANGE_STATE(_wifiBattleError, 0);
	}
}

//==============================================================================
/**
 * �}�b�`���O���I������
 * @param   none
 * @retval  none
 */
//==============================================================================


static void _stateWifiMatchEnd(void)
{
    int ret;

    CommSetWifiConnect(FALSE);
    if(mydwc_disconnect( _pCommState->disconnectIndex )){
        if(mydwc_returnLobby()){
            CommInfoFinalize();
//            CommStateSetErrorCheck(FALSE,FALSE);
//            CommStateSetErrorCheck(FALSE,TRUE);   ///335//�����ł���Ă��܂���WiFi�N���u�̃{�C�X�`���b�g�̏I�����ɂ��I�[�g�G���[TRUE�ɂ��Ă��܂��̂ŁA�R�����g�A�E�g
            _CHANGE_STATE(_wifiBattleLogin, 0);
            return;
        }
    }
    ret = mydwc_stepmatch(FALSE);
    if( ret < 0 ){
        _CHANGE_STATE(_wifiBattleError, 0);
    }
}

//==============================================================================
/**
 * WIFI�I���R�}���h   �q�@���e�@�ɂ�߂�悤�ɑ��M �Ԃ��肫�� CS_WIFI_EXIT
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommRecvExit(int netID, int size, void* pData, void* pWork)
{
    if(CommGetCurrentID() == COMM_PARENT_ID){
        _pCommState->disconnectIndex = 0;
        _CHANGE_STATE(_stateWifiMatchEnd, 0);
    }
    else{
        _pCommState->disconnectIndex = 1;
        _CHANGE_STATE(_stateWifiMatchEnd, 0);
    }
    _pCommState->bWifiDisconnect = TRUE;
}

//==============================================================================
/**
 * Wifi�ؒf�R�}���h���󂯎�����ꍇTRUE
 * @param   none
 * @retval  WifiLogin�Ɉڍs�����ꍇTRUE
 */
//==============================================================================

BOOL CommStateIsWifiDisconnect(void)
{
    return _pCommState->bWifiDisconnect;
}

//==============================================================================
/**
 * WifiLogin�Ɉڍs�������ǂ����̊m�F
 * @param   none
 * @retval  WifiLogin�Ɉڍs�����ꍇTRUE
 */
//==============================================================================

BOOL CommStateIsWifiLoginState(void)
{
    u32 stateAddr = (u32)_pCommState->state;

    if(stateAddr == (u32)_wifiBattleLogin){
        return TRUE;
    }
    if(stateAddr == (u32)_wifiLobbyConnect){	// ���r�[�̂Ƃ�
        return TRUE;
    }
    return FALSE;
}


BOOL CommStateIsWifiLoginMatchState(void)
{
    u32 stateAddr = (u32)_pCommState->state;

    if(stateAddr == (u32)_wifiBattleMaching){
        return TRUE;
    }
    if(stateAddr == (u32)_wifiBattleLogin){
        return TRUE;
    }
    if(stateAddr == (u32)_wifiLobbyConnect){	// ���r�[�̂Ƃ�
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * WifiError�̏ꍇ���̔ԍ���Ԃ�  �G���[�ɂȂ��Ă邩�ǂ������m�F���Ă�������o������
 * @param   none
 * @retval  Error�ԍ�
 */
//==============================================================================

COMMSTATE_DWCERROR* CommStateGetWifiError(void)
{
    GF_ASSERT(_pCommState);
    return &_pCommState->aError;
}

//==============================================================================
/**
 * WIFI�Ń��O�A�E�g���s���ꍇ�̏���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _stateWifiLogout(void)
{
    int ret;
    
    CommSetWifiConnect(FALSE);
    if(mydwc_disconnect( 0 )){ mydwc_returnLobby();
        _CHANGE_STATE(_stateConnectEnd, 0);
    }
    ret = mydwc_stepmatch(FALSE);
    if( ret < 0 ){
        // �G���[�����B
        _CHANGE_STATE(_wifiBattleError, 0);
    }
}

//==============================================================================
/**
 * WIFI�Ń��O�A�E�g���s���ꍇ�̏�
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiLogout(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    sys_SoftResetOK(SOFTRESET_TYPE_WIFI);
    CommInfoFinalize();
    _CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);
}

//==============================================================================
/**
 * WIFI�Ń}�b�`���O��؂�ꍇ�̏���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiMatchEnd(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    _pCommState->disconnectIndex = 0;
    _CHANGE_STATE(_stateWifiMatchEnd, 0);
}

//==============================================================================
/**
 * WIFI�Ń}�b�`���O��؂�ꍇ�̏��� ��������
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiTradeMatchEnd(void)
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    CommStateSetErrorCheck(FALSE,TRUE);  // �ď����������܂� �G���[�Ď�
    if(CommGetCurrentID() == COMM_PARENT_ID){
        _pCommState->disconnectIndex = 0;
    }
    else{
        _pCommState->disconnectIndex = 1;
    }
    _CHANGE_STATE(_stateWifiMatchEnd, 0);
}





void CommStateWifiBattleMatchEnd(void)
{
    u8 id = CommGetCurrentID();
    
    CommSendFixSizeData(CS_WIFI_EXIT,&id);
}


//==============================================================================
/**
 * WifiError���ǂ���
 * @param   none
 * @retval  Error
 */
//==============================================================================

BOOL CommStateIsWifiError(void)
{
    if(_pCommState){
        u32 stateAddr = (u32)_pCommState->state;
        if(stateAddr == (u32)_wifiBattleError){
            return TRUE;
        }
        if((stateAddr == (u32)_wifiBattleTimeout) && _pCommState->bDisconnectError){
            return TRUE;
        }
        if((stateAddr == (u32)_wifiLobbyTimeout) && _pCommState->bDisconnectError){
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================
/**
 * ���E�����A���E�o�g���^���[�̊J�n  �G���[�Ǘ��p
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiDPWStart(SAVEDATA* pSaveData)
{
    if(!_pCommState){
        sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_DPW );
        _pCommState = (_COMM_STATE_WORK*)sys_AllocMemory(HEAPID_COMMUNICATION, sizeof(_COMM_STATE_WORK));
        MI_CpuFill8(_pCommState, 0, sizeof(_COMM_STATE_WORK));
        _pCommState->serviceNo = COMM_MODE_DPW_WIFI;
        _pCommState->bWorldWifi = TRUE;
        _pCommState->pSaveData = pSaveData;
        CommStateSetErrorCheck(FALSE,TRUE);
//        DWC_SetReportLevel( DWC_REPORTFLAG_ALL);
        sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    }
}

//==============================================================================
/**
 * ���E�����A���E�o�g���^���[�̏I��
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiDPWEnd(void)
{
    if(_pCommState){
        sys_SoftResetOK(SOFTRESET_TYPE_WIFI);
        CommStateSetErrorCheck(FALSE,FALSE);
        sys_FreeMemoryEz(_pCommState);
        _pCommState = NULL;
        sys_DeleteHeap(HEAPID_COMMUNICATION);
    }
}

//==============================================================================
/**
 * ���E�ӂ�������  �G���[�Ǘ��p
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiFusigiStart(SAVEDATA* pSaveData)
{
    if(!_pCommState){
        sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_DPW );
        _pCommState = (_COMM_STATE_WORK*)sys_AllocMemory(HEAPID_COMMUNICATION, sizeof(_COMM_STATE_WORK));
        MI_CpuFill8(_pCommState, 0, sizeof(_COMM_STATE_WORK));
        _pCommState->serviceNo = COMM_MODE_FUSIGI_WIFI;
        _pCommState->bWorldWifi = TRUE;
        _pCommState->pSaveData = pSaveData;
        CommStateSetErrorCheck(FALSE,TRUE);
        sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    }
}

//==============================================================================
/**
 * ���E�����A���E�o�g���^���[�̏I��
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateWifiFusigiEnd(void)
{
    if(_pCommState){
        sys_SoftResetOK(SOFTRESET_TYPE_WIFI);
        CommStateSetErrorCheck(FALSE,FALSE);
        sys_FreeMemoryEz(_pCommState);
        _pCommState = NULL;
        sys_DeleteHeap(HEAPID_COMMUNICATION);
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	Email�ݒ��Wi-Fi�G���[�`�F�b�N�J�n
 *
 *	@param	pSaveData	�Z�[�u�f�[�^
 */
//-----------------------------------------------------------------------------
void CommStateWifiEMailStart(SAVEDATA* pSaveData)
{
    if(!_pCommState){
        sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_DPW );
        _pCommState = (_COMM_STATE_WORK*)sys_AllocMemory(HEAPID_COMMUNICATION, sizeof(_COMM_STATE_WORK));
        MI_CpuFill8(_pCommState, 0, sizeof(_COMM_STATE_WORK));
        _pCommState->serviceNo = COMM_MODE_EMAIL_WIFI;
        _pCommState->bWorldWifi = TRUE;
        _pCommState->pSaveData = pSaveData;
        CommStateSetErrorCheck(FALSE,TRUE);
//        DWC_SetReportLevel( DWC_REPORTFLAG_ALL);
        sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	Email�ݒ��Wi-Fi�G���[�`�F�b�N�I��
 *
 *	@param	pSaveData	�Z�[�u�f�[�^
 */
//-----------------------------------------------------------------------------
void CommStateWifiEMailEnd(void)
{
    if(_pCommState){
        sys_SoftResetOK(SOFTRESET_TYPE_WIFI);
        CommStateSetErrorCheck(FALSE,FALSE);
        sys_FreeMemoryEz(_pCommState);
        _pCommState = NULL;
        sys_DeleteHeap(HEAPID_COMMUNICATION);
    }
}





//==============================================================================
/**
 * ���E�����A���E�o�g���^���[�ŃG���[���ǂ���
 * @param   none
 * @retval  none
 */
//==============================================================================

BOOL CommStateGetWifiDPWError(void)
{
	int errorCode,ret;
	DWCErrorType myErrorType;
    
    if(!_pCommState){
        return FALSE;
    }

    if((_pCommState->serviceNo == COMM_MODE_DPW_WIFI) || (_pCommState->serviceNo == COMM_MODE_EMAIL_WIFI)){
        ret = DWC_GetLastErrorEx( &errorCode, &myErrorType );
        if(ret!=0){
            DWC_ClearError();
            return TRUE;
        }
    } else if(_pCommState->serviceNo == COMM_MODE_FUSIGI_WIFI){
        ret = DWC_GetLastErrorEx( &errorCode, &myErrorType );
        if(ret!=0){
			if(myErrorType == DWC_ETYPE_FATAL){
				OS_TPrintf("�G���[��ߊl: %d, %d\n", errorCode, myErrorType);
				return TRUE;
			}
		}
    }
    return FALSE;
}


//==============================================================================
/**
 * �q�@�ҋ@���  �e�@��I��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleConnecting(void)
{
    // �ڑ���  
    int ret = mydwc_connect();

    _pCommState->timer--;

    if ( ret == STEPMATCH_CONNECT )  {
        OS_TPrintf("WiFi�R�l�N�V�����ɐڑ����܂����B\n	");
		if( _pCommState->serviceNo == COMM_MODE_LOBBY_WIFI ){
			BOOL result;
			// Wi-Fi���r�[�Ƀ��O�C���J�n
#ifndef PM_DEBUG
			// �ʏ�LOGIN
			result = DWC_LOBBY_Login( _pCommState->cp_lobby_init_profile );
#else
			// DEBUGLOGIN
			if( D_Tomoya_WiFiLobby_ChannelPrefixFlag == TRUE ){
				OS_TPrintf( "prefix room %d\n", D_Tomoya_WiFiLobby_ChannelPrefix );
				result = DWC_LOBBY_LoginEx( _pCommState->cp_lobby_init_profile, D_Tomoya_WiFiLobby_ChannelPrefix );
			}else{
				result = DWC_LOBBY_Login( _pCommState->cp_lobby_init_profile );
			}
#endif
			// ���O�C���ɐ�������܂Ń��[�v
            OHNO_PRINT("LOGIN��  %d\n",result);
			if( result == TRUE ){
#ifdef COMMST_DEBUG_WFLBY_START
                {
                    // ����̕����f�[�^��ݒ�	
                    DWC_LOBBY_DEBUG_SetRoomData( 20*60, 0, COMMST_DEBUG_WFLBY_START_room, COMMST_DEBUG_WFLBY_START_season );
                }
#endif
				
                // DWC_LoginAsync�ɐ���
                _pCommState->lobby_dwc_login = TRUE;
                _CHANGE_STATE(_wifiLobbyLogin, _pCommState->timer);
                return;
            }
            else{
                OHNO_PRINT("LOGIN ERROR  %d\n",result);
                _CHANGE_STATE(_wifiLobbyError, 0);
                return;
            }
        }else{
	        _CHANGE_STATE(_wifiBattleLogin, 0);
            return;
		}
    }
    else if(ret != 0){
	    _CHANGE_STATE(_wifiBattleError, 0);
    }
    if(_pCommState->timer <= 0){
	    _CHANGE_STATE(_wifiBattleError, 0);
    }
}


//==============================================================================
/**
 * �e�@�Ƃ��ď��������s��
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleParentInit(void)
{
    MYSTATUS* pMyStatus;
    
    if(!CommIsVRAMDInitialize()){
        return;
    }
    {
        sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_WIFIMENU, _HEAPSIZE_WIFI);
    }

    if(CommParentModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE,TRUE)){
        mydwc_startConnect(_pCommState->pSaveData, HEAPID_WIFIMENU,_HEAPSIZE_DWCP2P, CommLocalGetServiceMaxEntry( _pCommState->serviceNo ) + 1); //CommLocalGetServiceMaxEntry(COMM_MODE_LOGIN_WIFI)+1);
        mydwc_setFetalErrorCallback(CommFatalErrorFunc);

        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_wifiBattleConnecting, MYDWC_TIMEOUTLOGIN);
    }
}

//==============================================================================
/**
 * �q�@�̏�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _wifiBattleChildInit(void)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }

    {
        sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_WIFIMENU, _HEAPSIZE_WIFI);
    }
    
    if(CommChildModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE)){
        mydwc_startConnect(_pCommState->pSaveData, HEAPID_WIFIMENU,_HEAPSIZE_DWCP2P, CommLocalGetServiceMaxEntry(_pCommState->serviceNo)+1);
        CommSetTransmissonTypeDS();
        _CHANGE_STATE(_wifiBattleConnecting, MYDWC_TIMEOUTLOGIN);
    }
}

//==============================================================================
/**
 * �o�g�����̎q�Ƃ��Ă̒ʐM�����J�n
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

#ifdef PM_DEBUG
void CommStateWifiEnterBattleChild(SAVEDATA* pSaveData, int serviceNo, int regulationNo, int soloDebugNo)
#else
void CommStateWifiEnterBattleChild(SAVEDATA* pSaveData, int serviceNo, int regulationNo)
#endif
{
    MYSTATUS* pMyStatus = SaveData_GetMyStatus(pSaveData);
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    // �ʐM�q�[�v�쐬
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,serviceNo);
//    _pCommState->serviceNo = serviceNo;
    _pCommState->regulationNo = regulationNo;
    _pCommState->pSaveData = pSaveData;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = soloDebugNo;
#endif
    
    _CHANGE_STATE(_wifiBattleChildInit, 0);
}


//==============================================================================
/**
 * �o�g�����̐e�Ƃ��Ă̒ʐM�����J�n
 * @param   pMyStatus  mystatus
 * @param   serviceNo  �ʐM�T�[�r�X�ԍ�
 * @param   regulationNo  �ʐM�T�[�r�X�ԍ�
 * @retval  none
 */
//==============================================================================

#ifdef PM_DEBUG
void CommStateWifiEnterBattleParent(SAVEDATA* pSaveData, int serviceNo, int regulationNo, int soloDebugNo)
#else
void CommStateWifiEnterBattleParent(SAVEDATA* pSaveData, int serviceNo, int regulationNo)
#endif
{
    MYSTATUS* pMyStatus = SaveData_GetMyStatus(pSaveData);
    if(CommIsInitialize()){
        return;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,serviceNo);
//    _pCommState->serviceNo = serviceNo;
    _pCommState->regulationNo = regulationNo;
    _pCommState->pSaveData = pSaveData;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = soloDebugNo;
#endif
    _CHANGE_STATE(_wifiBattleParentInit, 0);
}

//==============================================================================
/**
 * WIFI���Ƀ��O�C������ׂ̒ʐM�J�n
 * @param   pSaveData
 * @retval  none
 */
//==============================================================================

void* CommStateWifiEnterLogin(SAVEDATA* pSaveData, int wifiFriendStatusSize)
{
    MYSTATUS* pMyStatus = SaveData_GetMyStatus(pSaveData);
    if(CommIsInitialize()){
        return NULL;      // �Ȃ����Ă���ꍇ���͏��O����
    }
    // �ʐM�q�[�v�쐬
    sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_BATTLE );
    _commStateInitialize(pSaveData,COMM_MODE_LOGIN_WIFI);
    _pCommState->pWifiFriendStatus = sys_AllocMemory( HEAPID_COMMUNICATION, wifiFriendStatusSize );
    MI_CpuFill8( _pCommState->pWifiFriendStatus, 0, wifiFriendStatusSize );
//    _pCommState->serviceNo = COMM_MODE_LOGIN_WIFI;
    _pCommState->regulationNo = 0;
    _pCommState->pSaveData = pSaveData;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = 0;
#endif

    
    _CHANGE_STATE(_wifiBattleParentInit, 0);
    OHNO_PRINT("pMatchAddr %x\n",(u32)_pCommState->pWifiFriendStatus);
    return _pCommState->pWifiFriendStatus;
}


#ifdef COMMST_DEBUG_WFLBY_START
void CommStateWifiLobbyLogin_Debug( SAVEDATA* p_save, const void* cp_initprofile, u32 season, u32 room )
{
	CommStateWifiLobbyLogin( p_save, cp_initprofile );

	COMMST_DEBUG_WFLBY_START_season = season;
	COMMST_DEBUG_WFLBY_START_room	= room;
}
#endif

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[��p	���O�C���֐�
 *
 *	@param	p_save				�Z�[�u�f�[�^
 *	@param	cp_initprofile		�������v���t�B�[���f�[�^
 */
//-----------------------------------------------------------------------------
void CommStateWifiLobbyLogin( SAVEDATA* p_save, const void* cp_initprofile )
{
    MYSTATUS* pMyStatus = SaveData_GetMyStatus( p_save );
    if(CommIsInitialize()){
		TOMOYA_PRINT( "now status  wifiLobby connecting\n" );
        return ;      // �Ȃ����Ă���ꍇ���͏��O����
    }

#ifdef COMMST_DEBUG_WFLBY_START
	COMMST_DEBUG_WFLBY_START_season = 0;
	COMMST_DEBUG_WFLBY_START_room = 0;
#endif

    // �ʐM�q�[�v�쐬
    sys_SoftResetNG(SOFTRESET_TYPE_WIFI);
    sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_COMMUNICATION, _HEAPSIZE_WIFILOBBY );
    _commStateInitialize( p_save, COMM_MODE_LOBBY_WIFI );
    _pCommState->pWifiFriendStatus = NULL;

	_pCommState->cp_lobby_init_profile = cp_initprofile;	// �������v���t�B�[����ۑ�
	_pCommState->lobby_dwc_login = FALSE;	// DWC_LoginASync�����O

    _pCommState->regulationNo = 0;
    _pCommState->pSaveData = p_save;
#ifdef PM_DEBUG
    _pCommState->soloDebugNo = 0;
#endif

    
    _CHANGE_STATE(_wifiLobbyCommInit, 0);
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�̃��O�A�E�g
 */
//-----------------------------------------------------------------------------
void CommStateWifiLobbyLogout( void )
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return;
    }
    _CHANGE_STATE(_wifiLobbyLogout, 0);
}

//----------------------------------------------------------------------------
/**
 *	@brief	DWC_LoginAsync�̃��O�C���܂ł������������`�F�b�N
 *	@retval	TRUE	����
 *	@retval	FALSE	���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL CommStateWifiLobbyDwcLoginCheck( void )
{
    if(_pCommState==NULL){  // ���łɏI�����Ă���
        return FALSE;
    }

	return _pCommState->lobby_dwc_login;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[���L�̃G���[��ԂɂȂ��Ă��邩�`�F�b�N
 *
 *	@retval	TRUE	WiFi���r�[���L�̃G���[���
 *	@retval	FALSE	�G���[��Ԃł͂Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL CommStateWifiLobbyError( void )
{
    if(_pCommState){
        u32 stateAddr = (u32)_pCommState->state;
        if(stateAddr == (u32)_wifiLobbyError){
            return TRUE;
        }
    }
    return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�~�j�Q�[���J�n
 *
 *	@param	type		�Q�[���^�C�v
 */
//-----------------------------------------------------------------------------
void CommStateWifiP2PStart( DWC_LOBBY_MG_TYPE type )
{
	GF_ASSERT( _pCommState );
	GF_ASSERT( CommStateIsWifiLoginMatchState() == TRUE );

	// �ڑ��J�n
	if( DWC_LOBBY_MG_CheckRecruit( type ) == FALSE ){
		// �N����W���Ă��Ȃ��̂ŁA��������W����
	    DWC_LOBBY_MG_StartRecruit( type, 4 );
		_CHANGE_STATE(_wifiLobbyP2PMatchWait, 0);
	}else{
		// ��W�ɎQ������
		DWC_LOBBY_MG_Entry( type );
		_CHANGE_STATE(_wifiLobbyP2PMatchWait, 0);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[�~�j�Q�[���I��
 *			P2P�ؒf�`�F�b�N�́ACommStateWifiP2PGetConnectState�ōs���Ă�������
 */
//-----------------------------------------------------------------------------
void CommStateWifiP2PEnd( void )
{
	u32 status;

	if( DWC_LOBBY_MG_MyParent() == TRUE ){
		// �e�Ȃ��W����j��
		DWC_LOBBY_MG_EndRecruit();
	}
	// P2P�ڑ��I��
	DWC_LOBBY_MG_EndConnect();

	// �ؒf������
	_CHANGE_STATE(_wifiLobbyP2PDisconnect, 0);
}

//----------------------------------------------------------------------------
/**
 *	@brief	P2P�ɐڑ����Ă��邩�`�F�b�N
 *
 *	@retval	COMMSTATE_WIFIP2P_CONNECT_NONE,			// �ڑ����ĂȂ�
 *	@retval	COMMSTATE_WIFIP2P_CONNECT_MATCHWAIT,	// �ڑ��҂�
 *	@retval	COMMSTATE_WIFIP2P_CONNECT_MATCH,		// �ڑ�����
 *	@retval	COMMSTATE_WIFIP2P_CONNECT_DISCONNECT,	// �ؒf��
 */
//-----------------------------------------------------------------------------
u32 CommStateWifiP2PGetConnectState( void )
{
    if(_pCommState){
        u32 stateAddr = (u32)_pCommState->state;
        if(stateAddr == (u32)_wifiLobbyP2PMatch){
            return COMMSTATE_WIFIP2P_CONNECT_MATCH;
        }else if(stateAddr == (u32)_wifiLobbyP2PMatchWait){
            return COMMSTATE_WIFIP2P_CONNECT_MATCHWAIT;
		}else if( stateAddr == (u32)_wifiLobbyP2PDisconnect ){
			return COMMSTATE_WIFIP2P_CONNECT_DISCONNECT;
		}
    }
    return COMMSTATE_WIFIP2P_CONNECT_NONE;
}


//==============================================================================
/**
 * WIFI�t�����h�pWORK��n��
 * @param   pSaveData
 * @retval  none
 */
//==============================================================================

void* CommStateGetMatchWork(void)
{
    OHNO_PRINT("pMatchAddr %x\n",(u32)_pCommState->pWifiFriendStatus);
    return _pCommState->pWifiFriendStatus;
}

//==============================================================================
/**
 * ���������G���[�̌�����ʐM���������邩�ǂ�����ݒ�
 * @param   bFlg    �ؒf=�G���[�ɂ���
 * @param   bAuto  TRUE�Ō����J�n
 * @retval  none
 */
//==============================================================================

void CommStateSetErrorCheck(BOOL bFlg,BOOL bAuto)
{
    OHNO_PRINT("CommStateSetErrorCheckChange %d %d\n",bFlg, bAuto);
    if(_pCommState){
        _pCommState->bDisconnectError = bFlg;
        _pCommState->bErrorAuto = bAuto;
        if(bFlg){
            _pCommState->nowConnectNum = CommGetConnectNum();
        }
        else{
            _pCommState->nowConnectNum = 0;
        }
    }
    CommMPSetNoChildError(bFlg);  // �q�@�����Ȃ��Ȃ�����Č������邽�߂�ERR�����ɂ���
    CommMPSetDisconnectOtherError(bFlg);
}


//==============================================================================
/**
 * ���������G���[�̌�����ʐM���������邩�ǂ�����ݒ�
 * @param   bFlg  TRUE�Ō����J�n
 * @retval  none
 */
//==============================================================================

BOOL CommStateGetErrorCheck(void)
{
    if(_pCommState){
        if(_pCommState->stateError!=0){
            return TRUE;
        }
        return _pCommState->bErrorAuto;
    }
    return FALSE;
//    CommMPSetDisconnectOtherError(bFlg);
}


#ifdef PM_DEBUG
//==============================================================================
/**
 * �f�o�b�O�p�ʐM�ڑ��J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommStateRecvDebugStart(int netID, int size, void* pData, void* pWork)
{
    if(_pCommState){
        _pCommState->bDebugStart = TRUE;
    }
}

BOOL CommStateDBattleIsReady(void)
{
    if(_pCommState){
        return _pCommState->bDebugStart;
    }
    return FALSE;
}

static void _commConnectChildDebug(TCB_PTR tcb, void* work)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    if(CommMPGetGFBss(_pCommState->connectIndex)!=NULL){
        CommStateConnectBattleChild(_pCommState->connectIndex);  // �ڑ�
        TCB_Delete(tcb);
    }
}

static void _commConnectParentDebug(TCB_PTR tcb, void* work)
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    if(CommGetConnectNum() == (CommLocalGetServiceMaxEntry(CommStateGetServiceNo())+1) ){
        CommSendFixData(CS_DEBUG_START);  // �S���ɃR�}���h���M
        TCB_Delete(tcb);
    }
}

void CommStateDBattleConnect(BOOL bParent, int gameMode, SAVEDATA* pSaveData)
{
    MYSTATUS* pMyStatus = SaveData_GetMyStatus(pSaveData);
    if(COMM_MODE_BATTLE_SINGLE_WIFI <= gameMode){
/*  ���͕���
        DwcOverlayStart();
        if(!bParent){
            CommStateWifiEnterBattleChild(pSaveData, gameMode, 0, SOLO_DEBUG_NO + COMMDIRECT_DEBUG_NO);
            _pCommState->connectIndex = 0;
            TCB_Add(_commConnectChildDebug, NULL, _TCB_COMMCHECK_PRT-2);
        }
        else{
            CommStateWifiEnterBattleParent(pSaveData, gameMode, 0, SOLO_DEBUG_NO + COMMDIRECT_DEBUG_NO);
            TCB_Add(_commConnectParentDebug, NULL, _TCB_COMMCHECK_PRT-2);
        }
   */
    }
    else{
        if(!bParent){
            CommStateEnterBattleChild(pSaveData, gameMode, 0, NULL, FALSE, SOLO_DEBUG_NO + COMMDIRECT_DEBUG_NO);
            _pCommState->connectIndex = 0;
            TCB_Add(_commConnectChildDebug, NULL, _TCB_COMMCHECK_PRT-1);
        }
        else{
            CommStateEnterBattleParent(pSaveData, gameMode, 0, NULL, FALSE, SOLO_DEBUG_NO + COMMDIRECT_DEBUG_NO);
            TCB_Add(_commConnectParentDebug, NULL, _TCB_COMMCHECK_PRT-1);
        }
    }
    _pCommState->bDebugStart = FALSE;
}

#endif //PM_DEBUG



int CommStateGetServiceNo(void)
{
    if(_pCommState){
        return _pCommState->serviceNo;
    }
    return COMM_MODE_BATTLE_SINGLE;
}

int CommStateGetRegulationNo(void)
{
    if(_pCommState){
        return _pCommState->regulationNo;
    }
    return COMM_REG_NORMAL;
}

void CommStateSetPokemon(u8* sel)
{
    MI_CpuCopy8(sel,_pCommState->select,NELEMS(_pCommState->select));

    OHNO_SP_PRINT("%d %d %d %d %d %d\n",sel[0],sel[1],sel[2],sel[3],sel[4],sel[5]);
}

void CommStateGetPokemon(u8* sel)
{
    MI_CpuCopy8(_pCommState->select,sel,NELEMS(_pCommState->select));
}


#ifdef PM_DEBUG
int CommStateGetSoloDebugNo(void)
{
    if(_pCommState){
        return _pCommState->soloDebugNo;
    }
    return 0;
}
#endif


//==============================================================================
/**
 * �ʐM��WIFI�ڑ����Ă��邩�ǂ�����Ԃ�
 * @param   none
 * @retval  �ڑ����Ă�����TRUE
 */
//==============================================================================

BOOL CommStateIsWifiConnect(void)
{
    return CommLocalIsWiFiGroup(CommStateGetServiceNo());
}


//==============================================================================
/**
 * �G���[��Ԃɓ���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommSetErrorReset(u8 type)
{
    if(_pCommState){
        _pCommState->ResetStateType = type;
    }
}

//==============================================================================
/**
 * ���Z�b�g�G���[��ԂɂȂ������ǂ����m�F����
 * @param   none
 * @retval  ���Z�b�g�G���[��ԂȂ��ނ�Ԃ� �G���[�łȂ����FALSE=0
 */
//==============================================================================

u8 CommIsResetError(void)
{
    if(sys_SioErrorNG_Get()){
        return FALSE;
    }
    if(_pCommState){
        return _pCommState->ResetStateType;
    }
    return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   fatal�G���[�p�֐�  ���̊֐������甲�����Ȃ�
 * @param   no   �G���[�ԍ�
 * @retval  none
 */
//--------------------------------------------------------------

void CommFatalErrorFunc(int no)
{
    int i=0;
    
    ComErrorWarningResetCall(HEAPID_BASE_SYSTEM,COMM_ERRORTYPE_POWEROFF, no);
    while(1){
        i++;
    }
}

void CommFatalErrorFunc_NoNumber( void )
{
    int i=0;
    
    ComErrorWarningResetCall( HEAPID_BASE_SYSTEM, 4, 0 );
    while(1){
        i++;
    }
}

//--------------------------------------------------------------
/**
 * @brief   �G���[�����m���� �ʐM�G���[�p�E�C���h�E���o��
 * @param   heapID    �������[�m�ۂ���HEAPID
 * @param   bgl       GF_BGL_INI
 * @retval  none
 */
//--------------------------------------------------------------

void CommErrorCheck(int heapID, GF_BGL_INI* bgl)
{
// ���̊֐��ł͏������Ȃ����ƂɂȂ�܂���
}


//--------------------------------------------------------------
/**
 * @brief   �G���[�����m���� �ʐM�G���[�p�E�C���h�E���o��
 * @param   heapID    �������[�m�ۂ���HEAPID
 * @param   bgl       GF_BGL_INI
 * @retval  none
 */
//--------------------------------------------------------------

void CommErrorDispCheck(int heapID)
{
    if(CommStateGetErrorCheck()){

        if(CommIsError() || CommStateIsWifiError() || CommStateGetWifiDPWError()
           || (_pCommState->stateError!=0) || CommStateWifiLobbyError() ){
            if( !sys_SioErrorNG_Get() ){
                if(!CommIsResetError() ){   // ���Z�b�g�G���[��ԂŖ����ꍇ
                    Snd_Stop();
                    SaveData_DivSave_Cancel(_pCommState->pSaveData); // �Z�[�u���Ă���~�߂�
                    sys.tp_auto_samp = 1;  // �T���v�����O���~�߂�
                    
                    if(_pCommState->stateError == COMM_ERROR_RESET_GTS){
                        CommSetErrorReset(COMM_ERROR_RESET_GTS);  // �G���[���Z�b�g��ԂɂȂ�
                    }
                    else if((_pCommState->serviceNo == COMM_MODE_FUSIGI_WIFI) ||
                            (_pCommState->serviceNo == COMM_MODE_MYSTERY) ||
                            (_pCommState->serviceNo == COMM_MODE_EMAIL_WIFI) ){
                        CommSetErrorReset(COMM_ERROR_RESET_TITLE);  // �G���[���Z�b�g��ԂɂȂ�
                    }
                    else{
                        CommSetErrorReset(COMM_ERROR_RESET_SAVEPOINT);  // �G���[���Z�b�g��ԂɂȂ�
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   ���Z�b�g�ł����Ԃ��ǂ���
 * @param   none
 * @retval  TRUE �Ȃ烊�Z�b�g
 */
//--------------------------------------------------------------

BOOL CommStateIsResetEnd(void)
{
    if(CommMPIsConnectStalth() || !CommStateIsInitialize()){ // �ʐM�I��
        return TRUE;
    }
    if(!CommMPIsConnect()){
        return TRUE;
    }
    return FALSE;
}

//==============================================================================
/**
 * �G���[�ɂ���ꍇ���̊֐����ĂԂƃG���[�ɂȂ�܂�
 * @param   �G���[���
 * @retval  �󂯕t�����ꍇTRUE
 */
//==============================================================================

BOOL CommStateSetError(int no)
{

    if(_pCommState){
        _pCommState->stateError = no;
        CommSystemShutdown();
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM��Ԃ�WiFi�|�t�B���S�l�ڑ��p�ɂ���
 */
//-----------------------------------------------------------------------------
void CommStateChangeWiFiPofin( void )
{
	_pCommState->serviceNo = COMM_MODE_WIFI_POFIN;


    mydwc_setReceiver( CommRecvOtherCallback, CommRecvOtherCallback );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM��Ԃ�WiFi�N���u�p�ɂ���
 */
//-----------------------------------------------------------------------------
void CommStateChangeWiFiClub( void )
{
	_pCommState->serviceNo = COMM_MODE_CLUB_WIFI;
    mydwc_setReceiver( CommRecvOtherCallback, CommRecvOtherCallback );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM��Ԃ�WiFi�~�j�Q�[���l�ڑ��p�ɂ���
 */
//-----------------------------------------------------------------------------
void CommStateChangeWiFiLobbyMinigame( void )
{
	_pCommState->serviceNo = COMM_MODE_LOBBY_WIFI;

    mydwc_setReceiver( CommRecvOtherCallback, CommRecvOtherCallback );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM��Ԃ�WiFi��l�p��Ԃɖ߂�
 */
//-----------------------------------------------------------------------------
void CommStateChangeWiFiLogin( void )
{
	_pCommState->serviceNo = COMM_MODE_LOGIN_WIFI;
    mydwc_setReceiver( CommRecvParentCallback, CommRecvCallback );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM��Ԃ��o�g���p�ɕύX
 */
//-----------------------------------------------------------------------------
void CommStateChangeWiFiBattle( void )
{
	_pCommState->serviceNo = COMM_MODE_BATTLE_SINGLE_WIFI;
    mydwc_setReceiver( CommRecvParentCallback, CommRecvCallback );
}


//-------------------------------------
///	WiFi���r�[�Œǉ�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM������
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyCommInit( void )
{
    if(!CommIsVRAMDInitialize()){
        return;
    }
    {
        sys_CreateHeapLo( HEAPID_BASE_APP, HEAPID_WIFIMENU, _HEAPSIZE_WIFI_LOBBY);
    }

    if(CommParentModeInit(TRUE, TRUE, _PACKETSIZE_BATTLE,TRUE)){
		mydwc_startConnect(_pCommState->pSaveData, HEAPID_WIFIMENU, _HEAPSIZE_DWCLOBBY, CommLocalGetServiceMaxEntry(_pCommState->serviceNo)+1); 
		mydwc_setFetalErrorCallback(CommFatalErrorFunc);
		CommSetTransmissonTypeDS();

		// �{�C�X�`���b�g�͂Ȃ�
		mydwc_setVChat( FALSE );
			
		// BothNet
		CommSetWifiBothNet( FALSE );
	
		_CHANGE_STATE(_wifiBattleConnecting, MYDWC_TIMEOUTLOGIN);
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�T�[�o�Ƀ��O�C��
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyLogin( void )
{
	BOOL result;

	// �^�C���A�E�g������ǉ�
	_pCommState->timer--;
	if( _pCommState->timer <= 0 ){
		// �ʐM�G���[��
	    _CHANGE_STATE(_wifiBattleError, 0);
		return ;
	}


    CommSetWifiConnect(TRUE);

	result = _wifiLobbyUpdateCommon();
	if( result == FALSE ){ return ; }	// �G���[�N����

	// ���O�C�������҂�
	result = DWC_LOBBY_LoginWait();
	if( result ){

		//�ڑ�����
		_CHANGE_STATE(_wifiLobbyConnect, 0);
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�ڑ���
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyConnect( void )
{
	BOOL result;

	result = _wifiLobbyUpdateCommon();
	if( result == FALSE ){ return ; }	// �G���[�N����
}

//----------------------------------------------------------------------------
/**
 *	@brief	dwc_step�̖߂�l���`�F�b�N����
 *
 *	@param	dwc_err		�߂�l
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�G���[
 */
//-----------------------------------------------------------------------------
static BOOL _wifiLobbyDwcStepRetCheck( int dwc_err )
{
	BOOL ret = TRUE;
    if((dwc_err >= DWC_ERROR_FRIENDS_SHORTAGE) && (STEPMATCH_SUCCESS > dwc_err)){
        _CHANGE_STATE(_wifiBattleFailed, 0);	// �y�x�ȃG���[
		ret = FALSE;
    }
    else if( dwc_err < 0 ){
        // �G���[�����B
        //_errcodeConvert(dwc_err);
        _CHANGE_STATE(_wifiBattleError, 0);
		ret = FALSE;
    }		
    else if( dwc_err == STEPMATCH_TIMEOUT ) {     // �^�C���A�E�g�i���肩��P�O�b�ȏチ�b�Z�[�W���͂��Ȃ��j
        _CHANGE_STATE(_wifiLobbyTimeout, 0);
		ret = FALSE;
    }
    else if(dwc_err == STEPMATCH_DISCONNECT){
		ret = FALSE;
        if(_pCommState->bDisconnectError){
            _CHANGE_STATE(_wifiBattleError, 0);	// �ؒf���G���[����
        }
        else{	
            _CHANGE_STATE(_wifiBattleDisconnect, 0);	// �ؒf�͐ؒf�G���[
        }
    }

    if(_pCommState->bDisconnectError){
        if(_pCommState->nowConnectNum != CommGetConnectNum()){
            _CHANGE_STATE(_wifiBattleError, 0);
			ret = FALSE;
        }
    }
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief		���r�[�T�[�o�A�b�v�f�[�g����
 */
//-----------------------------------------------------------------------------
static BOOL _wifiLobbyUpdate( void )
{
	DWC_LOBBY_CHANNEL_STATE lobby_err;
	BOOL ret = TRUE;

	// Wi-Fi���r�[���t���[������
	lobby_err = DWC_LOBBY_Update();
	switch( lobby_err ){
	//  ����
    case DWC_LOBBY_CHANNEL_STATE_NONE:           // �`�����l���ɓ����Ă��Ȃ��B
    case DWC_LOBBY_CHANNEL_STATE_LOGINWAIT:		// �`�����l���ɓ������B
    case DWC_LOBBY_CHANNEL_STATE_CONNECT:		// �`�����l���ɓ����ς݁B
    case DWC_LOBBY_CHANNEL_STATE_LOGOUTWAIT:     // �`�����l���ɑގ����B
		break;
	
	// �G���[����
	case DWC_LOBBY_CHANNEL_STATE_ERROR:           // �`�����l����Ԃ��擾�ł��܂���ł����B
		_CHANGE_STATE(_wifiLobbyError, 0);
		ret = FALSE;
		break;
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[��ԍX�V����	
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�G���[����
 */
//-----------------------------------------------------------------------------
static BOOL _wifiLobbyUpdateCommon( void )
{
	int dwc_err;
	BOOL ret;

	dwc_err = mydwc_stepmatch(FALSE);

	// �G���[�`�F�b�N
	ret = _wifiLobbyDwcStepRetCheck( dwc_err );
	if( ret == FALSE ){
		return ret;
	}

	// ���r�[�̃A�b�v�f�[�g
	ret = _wifiLobbyUpdate();
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi���r�[��ԍX�V����	
 *
 *	@param	p_matchret		�}�b�`���O�ɑ΂��鉞��
 * 
 *	@retval	STEPMATCH_CONTINUE�c�}�b�`���O��
 *	@retval	STEPMATCH_SUCCESS�c����
 *	@retval	STEPMATCH_CANCEL�c�L�����Z��
 *	@retval	STEPMATCH_FAIL  �c���肪�e����߂����߁A�ڑ��𒆒f
 */
//-----------------------------------------------------------------------------
static BOOL _wifiLobbyUpdateMatchCommon( u32* p_matchret )
{
	int dwc_err;
	BOOL ret;

	dwc_err = mydwc_stepmatch( FALSE );

	// �}�b�`���O���ʂ�ݒ�
	if( dwc_err >= STEPMATCH_SUCCESS ){
		*p_matchret = dwc_err;
	}else{
		*p_matchret = STEPMATCH_CONTINUE;

		// �G���[�`�F�b�N
		ret = _wifiLobbyDwcStepRetCheck( dwc_err );
		if( ret == FALSE ){
			return ret;
		}
	}

	// ���r�[�̃A�b�v�f�[�g
	ret = _wifiLobbyUpdate();
	return ret;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���O�A�E�g�J�n
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyLogout( void )
{
	// Wi-Fi���r�[���烍�O�A�E�g
	DWC_LOBBY_Logout();

	_CHANGE_STATE(_wifiLobbyLogoutWait, 0);
}

//----------------------------------------------------------------------------
/**
 *	@brief	Wi-Fi���r�[���O�A�E�g
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyLogoutWait( void )
{
	BOOL result;
	
	// ���r�[��DWC�𓮂���������
	mydwc_stepmatch(FALSE);
	_wifiLobbyUpdate();

	// ���O�A�E�g�I���҂�
	result  = DWC_LOBBY_LogoutWait();
	if( result ){
		sys_SoftResetOK(SOFTRESET_TYPE_WIFI);
		CommInfoFinalize();
		_CHANGE_STATE(_stateConnectEnd, _EXIT_SENDING_TIME);

		CommSetWifiConnect(FALSE);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[���L�̃G���[
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyError( void ){}



//----------------------------------------------------------------------------
/**
 *	@brief	LobbyP2P�ڑ�����
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyP2PMatch( void )
{
	BOOL result;
	//  ���r�[�T�[�o�Ƃ̐ڑ�����
	_wifiLobbyUpdateCommon();

	// ForceEnd�ŏI��
	result = DWC_LOBBY_MG_ForceEnd();
	if( result == TRUE ){
		// P2P�ڑ��I��
		DWC_LOBBY_MG_EndConnect();

		// �ؒf������
		_CHANGE_STATE(_wifiLobbyP2PDisconnect, 0);
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	LobbyP2P�e�ڑ��҂�����
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyP2PMatchWaitParent( void )
{
	BOOL result;
	DWC_LOBBY_MG_ENTRYRET entrywait;

	// ForceEnd�ŏI��
	result = DWC_LOBBY_MG_ForceEnd();
	if( result == TRUE ){
		// P2P�ڑ��I��
		DWC_LOBBY_MG_EndConnect();

		// �ؒf������
		_CHANGE_STATE(_wifiLobbyP2PDisconnect, 0);
		return ;
	}

	//  ���r�[�T�[�o�Ƃ̐ڑ�����
	_wifiLobbyUpdateCommon();

	// �ڑ������`�F�b�N
	entrywait = DWC_LOBBY_MG_EntryWait();
	switch( entrywait ){
	case DWC_LOBBY_MG_ENTRYWAIT:		// �G���g���[��
		break;
	
	case DWC_LOBBY_MG_ENTRYOK:			// �G���g���[����
		_CHANGE_STATE(_wifiLobbyP2PMatch, 0);
		break;
	
	case DWC_LOBBY_MG_ENTRYNG:			// �G���g���[���s
	case DWC_LOBBY_MG_ENTRYNONE:		// �������Ă��Ȃ�
	default:							// ���̑�
		// P2P�ڑ��I��
		DWC_LOBBY_MG_EndConnect();
		// ��Ԃ�߂�
		_CHANGE_STATE(_wifiLobbyP2PDisconnect, 0);
		break;
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	LobbyP2P�q�ڑ��҂�����
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyP2PMatchWait( void )
{
	BOOL result;
	u32 matchret;

	// ForceEnd�ŏI��
	result = DWC_LOBBY_MG_ForceEnd();
	if( result == TRUE ){
		// P2P�ڑ��I��
		DWC_LOBBY_MG_EndConnect();

		// �ؒf������
		_CHANGE_STATE(_wifiLobbyP2PDisconnect, 0);
		return ;
	}

	//  ���r�[�T�[�o�Ƃ̐ڑ�����
	_wifiLobbyUpdateMatchCommon( &matchret );

	// �ڑ�����������q�ڑ�������
	switch( matchret ){
	case STEPMATCH_CONTINUE:		// �G���g���[��
		break;
	
	case STEPMATCH_SUCCESS:			// �G���g���[����
		_CHANGE_STATE(_wifiLobbyP2PMatch, 0);
		break;
	
	default:							// ���̑�
		// P2P�ڑ��I��
		DWC_LOBBY_MG_EndConnect();
		// ��Ԃ�߂�
		_CHANGE_STATE(_wifiLobbyConnect, 0);
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	LobbyP2P�ؒf����
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyP2PDisconnect( void )
{
	BOOL result;
	int ret;

    OS_TPrintf("_wifiLobbyP2PDisconnect\n");
	ret = mydwc_stepmatch( TRUE );	// �ؒf����
    if( ret < 0 ){
        // �G���[�����B
        _CHANGE_STATE(_wifiBattleError, 0);
		return ;
    }
	 
	// ���r�[�̃A�b�v�f�[�g
	result = _wifiLobbyUpdate();
	if( result == FALSE ){ return ; }
	

    if(mydwc_disconnect( 0 )){
		ret = mydwc_returnLobby();
        if( ret ){
			CommSystemReset();
			CommSetWifiBothNet( FALSE );
			_CHANGE_STATE(_wifiLobbyConnect, 0);
		}
    }
}


//----------------------------------------------------------------------------
/**
 *	@brieof	WiFi�L���p�^�C���A�E�g����
 */
//-----------------------------------------------------------------------------
static void _wifiLobbyTimeout( void )
{
   int ret;
    ret = mydwc_stepmatch(FALSE);

	if( ret < 0 ){
        // �G���[�����B
        _CHANGE_STATE(_wifiBattleError, 0);
    }		

	// ���r�[�̃A�b�v�f�[�g
	_wifiLobbyUpdate();
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM��Ԃ��t�@�N�g���[�o�g���p�ɕύX
 */
//-----------------------------------------------------------------------------
void CommStateChangeWiFiFactory( void )
{
	_pCommState->serviceNo = COMM_MODE_WIFI_FRONTIER_COMMON;
}



//==============================================================================
/**
 * �|�P�b�`�̎��ȊO�ŒʐM���Ă��邩�ǂ�����Ԃ�
 * @retval  �ʐM��Ԓ���TRUE
 */
//==============================================================================
BOOL CommStateIsInitializeOtherPoketch(void)
{
    if(_pCommState && (_pCommState->serviceNo == COMM_MODE_POKETCH)){
        return FALSE;
    }
    return CommIsInitialize();
}

