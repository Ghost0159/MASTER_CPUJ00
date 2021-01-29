//=============================================================================
/**
 * @file	comm_field_state.c
 * @brief	�ʐM��Ԃ��Ǘ�����T�[�r�X  �ʐM�̏�ʂɂ���
 *          �X���b�h�̂ЂƂƂ��ē����A�����̒ʐM��Ԃ⑼�̋@���
 *          �J�n��I�����Ǘ�����
 *          �t�B�[���h��Œ��ڊǗ��������̂�communication����field�Ɉړ� 2005.09.28
 * @author	Katsumi Ohno
 * @date    2005.08.02
 */
//=============================================================================

#include "common.h"
#include "communication/communication.h"
#include "communication/comm_state.h"
#include "system/gamedata.h"
#include "system/pm_str.h"

#include "comm_field_state.h"
#include "underground/ug_manager.h"
#include "field/field.h"
#include "fieldsys.h"
#include "comm_command_field.h"
//#include "comm_direct_counter.h"
//#include "comm_vsroom.h"
#include "sysflag.h"

#include "script.h"							//StartFieldScript
#include "..\fielddata\script\common_scr_def.h"		//SCRID_TRAINER_MOVE_SCRIPT
#include "..\fielddata\script\connect_def.h"		//SCRID_CONNECT_COLOSSEUM
#include "../fielddata/script/battle_room_def.h"
#include "savedata/trainercard_data.h"
#include "application/trainer_card.h"
#include "tr_card_setup.h"
#include "system/wipe.h"
#include "system/pm_overlay.h"
#include "system/brightness.h"

#include "field/union_beacon_tool.h"
#include "field_encount.h"

#ifdef PM_DEBUG
#include "d_ohno.h"
#include "fld_debug.h"
#endif

#include "comm_field_state_local.h"
//==============================================================================
// ��`
//==============================================================================
#define _START_TIME (50)     // �J�n����
#define _CHILD_P_SEARCH_TIME (12) ///�q�@�Ƃ��Đe��T������
#define _PARENT_WAIT_TIME (40) ///�e�Ƃ��Ă̂�т�҂���
#define _FINALIZE_TIME (2)
#define _EXIT_SENDING_TIME (5)
#define _PARENT_END_TIME (2)
#define _SEND_NAME_TIME (10)
#define _PARENTSCAN_PA (3)  // �e�@�Ƃ��Č�������m����1/3

#define _TCB_COMMCHECK_PRT   (10)    ///< �t�B�[���h������ʐM�̊Ď����[�`����PRI


//==============================================================================
// static�錾
//==============================================================================

// �X�e�[�g�̏�����

static void _commCheckFunc(TCB_PTR tcb, void* work);  // �X�e�[�g�����s���Ă���^�X�N
static void _changeState(PTRStateFunc state, int time);  // �X�e�[�g��ύX����
static void _changeStateDebug(PTRStateFunc state, int time, int line);  // �X�e�[�g��ύX����

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
static void _underStart(void);           // ������ + �V�X�e���̏�����
static void _underOpenning(void);       //�C�x���g��
static void _underConnecttingAloneSend(void);  // �������]������
static void _underConnectting(void);     // �ڑ���
static void _underChildConnecting(void);   // �q�@���e�@�ɐڑ���
static void _underChildConnectInfoWait(void);  // �q�@�Ƃ��Đڑ��� info���W
static void _underChildConnectFlagWait(void);
static void _underChildConnectSecret(void);    // �q�@�Ƃ��Đڑ���
static void _underChildConnectDig(void);    // �q�@�Ƃ��Đڑ���
static void _underChildConnect(void);    // �q�@�Ƃ��Đڑ���
static void _underChildConnect_JumpFieldStart(void);    // �q�@�Ƃ��Đڑ��� Jump�J�n
static void _underChildConnect_JumpField(void);    // �q�@�Ƃ��Đڑ��� Jump��
static void _underChildConnect_JumpFieldEnd(void);    // �q�@�Ƃ��Đڑ��� Jump�����
static void _underParentRestart(void);
#if (T1645_060815_FIX|T1644_060816_FIX)
static void _underParentRestartAloneWait(void);
#endif //(T1645_060815_FIX|T1644_060816_FIX)
static void _underChildRestart(void);
static void _underChildRestart_Base(void);
static void _underChildStalth(void);

static void _underParentWait(void);      // �e�@�Ƃ��đҋ@���
static void _underParentConnect(void);   // �e�@�Ƃ��Đڑ���
static void _underParentConnect_JumpFieldStart(void);  // �e�@�Ƃ��Đڑ��� Jump�J�n
static void _underParentConnect_JumpField(void);  // �e�@�Ƃ��Đڑ��� Jump��
static void _underParentConnect_JumpFieldEnd(void);  // �e�@�Ƃ��Đڑ��� Jump�I���
static void _stateUnderGroundConnectEnd(void);  // �n���ؒf

static void _underSoloConnect_JumpFieldStart(void);    // solo�@�Ƃ��Đڑ��� Jump�J�n
static void _underSoloConnect_JumpField(void);    // solo�@�Ƃ��Đڑ��� Jump��
static void _underSoloConnect_JumpFieldEnd(void);    // solo�@�Ƃ��Đڑ��� Jump�����
static void _underStalthConnect_JumpFieldStart(void);    // stalth�@�Ƃ��Đڑ��� Jump�J�n
static void _underStalthConnect_JumpField(void);    // stalth�@�Ƃ��Đڑ��� Jump��
static void _underStalthConnect_JumpFieldEnd(void);    // stalth�@�Ƃ��Đڑ��� Jump�����

static void _underChildStalthDeadLoop(void);



// ���̑���ʓI�ȃX�e�[�g
static void _stateNone(void);            // �������Ȃ�
static void _stateConnectError(void);    // �ڑ��G���[���
static void _stateEnd(void);             // �I������
static void _stateConnectEnd(void);      // �ؒf�����J�n
static void _underChildRebootFunc(void);

#ifdef PM_DEBUG		// Debug ROM
static void _debugTimerStart(void);   // �f�o�b�O�p
static void _debugTimerDisp(char* msg);
#else
static void _debugTimerStart(void){}   // �f�o�b�O�p
static void _debugTimerDisp(char* msg){}
#endif


//==============================================================================
/**
 * �n���ɂ͂��������̒ʐM����
 * @param   pFSys FIELDSYS_WORK
 * @retval  none
 */
//==============================================================================

void CommFieldStateEnterUnderGround(FIELDSYS_WORK* pFSys)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    
    if(pCFS != NULL){ // �Ȃ����Ă���ꍇ���͏��O����
        return;
    }
    // �ڑ����s
    CommStateEnterUnderGround(GameSystem_GetSaveData(pFSys));
    _commStateInitialize(pFSys);
    pCFS = _getCommFieldStateWork();
    sys_CreateHeap( HEAPID_BASE_APP, HEAPID_UNDERGROUND, 0xe800 );  //�O�b�Y���j���[�ł��Ȃ�m��
    if(!SysFlag_UgFirstCheck(SaveData_GetEventWork(pCFS->pFSys->savedata))){
        OS_TPrintf("����n���C�x���g\n");
        CommStateSetFirstEvent();
    }

}

//==============================================================================
/**
 * �n���ɂ͂����āA�t�B�[���h���n���ɕς�������̏���
 * @param   pFSys FIELDSYS_WORK
 * @retval  none
 */
//==============================================================================

void CommFieldStateArrivalUnderGround(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();

    pCFS->bUGOverlay = TRUE;
    RECORD_Score_Add(SaveData_GetRecord(pCFS->pFSys->savedata), SCORE_ID_MINE_IN);

    CommUnderOptionInitialize(pCFS->pFSys);  // �n���T�[�r�X�̋N��

#ifdef PM_DEBUG
    if(DebugOhnoGetCommDebugDisp()){
        DebugIchiCheckOn(pCFS->pFSys);
    }
#endif
    
    _CHANGE_STATE(_underStart, 0);
}

//==============================================================================
/**
 * �n�����o��ꍇ�̒ʐM����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommFieldStateExitUnderGround(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS==NULL){  // ���łɏI�����Ă���
        return;
    }
    // �ؒf�X�e�[�g�Ɉڍs����  �����ɐ؂�Ȃ�
    
    _CHANGE_STATE(_stateUnderGroundConnectEnd, 3);
}

//==============================================================================
/**
 * �n����ʐM��~�ɂ���ꍇ�̒ʐM����
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommFieldStateStalthUnderGround(void)
{
//    OHNO_PRINT("stalth \n");
//    CommUnderOptionFinalize();  // �������͂���@@OO
    CommStateUnderGroundOfflineSecretBase();
    _CHANGE_STATE(_underChildStalth, 0);
}

//==============================================================================
/**
 * �n�����Đڑ�����ꍇ�̏���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underStalthEndCheck(void)
{
    if(!CommMPIsConnectStalth()){
        CommSecretBaseInfoJumpEnd();
        _CHANGE_STATE(_underStart,0);
    }

}


void CommFieldStateUnderGroundReConnect(void)
{
//    CommSecretBaseInfoChildSendStart();
    //_underChildConnect_JumpFieldEnd();
    CommStateUnderGroundOnlineSecretBase();  // �ʐM��Ԃ�ONLINE�ɂ��ǂ�

    _CHANGE_STATE(_underStalthEndCheck,0);
//    _CHANGE_STATE(_underStart,1)
    
//    CommTrapInfoChildSendStart();  //Trap���������g�ɑ��M�J�n
//    _CHANGE_STATE(_underConnectting, _CHILD_P_SEARCH_TIME*2);
}

//==============================================================================
/**
 * �n����JUMP����ꍇ�̒ʐM����
 * @param   none
 * @retval  none
 */
//==============================================================================

BOOL CommStateJumpUnderGround(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    int i;
    u32 stateAddr = (u32)pCFS->state;
    u32 parentAddr = (u32)_underParentConnect;
    u32 childAddr = (u32)_underChildConnect;
    u32 soloAddr = (u32)_underConnectting;
    u32 stalthAddr = (u32)_underChildStalthDeadLoop;
    u32 jumpState[] = {
        (u32)_underParentConnect_JumpFieldStart,
        (u32)_underChildConnect_JumpFieldStart,
        (u32)_underSoloConnect_JumpFieldStart,
        (u32)_underStalthConnect_JumpFieldStart,
    };


    
    if(stateAddr == parentAddr){
        _CHANGE_STATE(_underParentConnect_JumpFieldStart, 0);
        return TRUE;
    }
    else if(childAddr == stateAddr){
        _CHANGE_STATE(_underChildConnect_JumpFieldStart, 0);
        return TRUE;
    }
    else if(soloAddr == stateAddr){
        _CHANGE_STATE(_underSoloConnect_JumpFieldStart, 0);
        return TRUE;
    }
    else if(stalthAddr == stateAddr){
        _CHANGE_STATE(_underStalthConnect_JumpFieldStart, 0);
        return TRUE;
    }
    for(i = 0 ; i < NELEMS(jumpState); i++){
        if(jumpState[i] == stateAddr){
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================
/**
 * �n����JUMP���I������ꍇ�̏��� �X�e�[�g��Active��ԂɂȂ�
 * @param   none
 * @retval  none
 */
//==============================================================================

BOOL CommStateJumpEndUnderGround(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    u32 stateAddr = (u32)pCFS->state;
    u32 parentAddr = (u32)_underParentConnect_JumpField;
    u32 childAddr = (u32)_underChildConnect_JumpField;
    u32 soloAddr = (u32)_underSoloConnect_JumpField;
    u32 stalthAddr = (u32)_underStalthConnect_JumpField;

    if(stateAddr == parentAddr){
        _CHANGE_STATE(_underParentConnect_JumpFieldEnd, 0);
        return TRUE;
    }
    else if(childAddr == stateAddr){
        _CHANGE_STATE(_underChildConnect_JumpFieldEnd, 0);
        return TRUE;
    }
    else if(soloAddr == stateAddr){
        _CHANGE_STATE(_underSoloConnect_JumpFieldEnd, 0);
        return TRUE;
    }
    else if(stalthAddr == stateAddr){
        _CHANGE_STATE(_underStalthConnect_JumpFieldEnd, 0);
        return TRUE;
    }
    {
//        GF_ASSERT(0 && "�z��O��state:�v�C��");
    }
    return FALSE;
}

//==============================================================================
/**
 * �ړ��\�X�e�[�g�Ȃ̂��ǂ����Ԃ�
 * @param   none
 * @retval  �ړ��\�Ȃ�TRUE
 */
//==============================================================================

BOOL CommIsUnderGroundMoveState(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    int i;
    u32 funcTbl[]={
//        (u32)_battleMoveRoom,
        (u32)_underChildConnect,
        (u32)_underParentConnect,
        (u32)_underChildConnectInfoWait,
        (u32)_underChildConnectFlagWait,
        (u32)_underChildConnectSecret,
        (u32)_underChildConnectDig,
        (u32)_underChildConnect_JumpFieldStart,
        (u32)_underChildConnect_JumpField,
        (u32)_underChildConnect_JumpFieldEnd,
        (u32)_underParentConnect_JumpFieldStart,
        (u32)_underParentConnect_JumpField,
        (u32)_underParentConnect_JumpFieldEnd,
        0,
    };
    u32 stateAddr = (u32)pCFS->state;

    if(pCFS==NULL){  // ���łɏI�����Ă���
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
 * �ʐM�Ǘ��X�e�[�g�̏���
 * @param
 * @retval  none
 */
//==============================================================================

static void _commCheckFunc(TCB_PTR tcb, void* work)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
#ifdef PM_DEBUG
    if(pCFS){
        pCFS->debugTimer++;
    }
#endif

    if(pCFS==NULL){
        TCB_Delete(tcb);
    }
    else{
        if(pCFS->state != NULL){
            PTRStateFunc state = pCFS->state;
            if(!pCFS->bStateNoChange){
                state();
            }
        }
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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    pCFS->state = state;
    pCFS->timer = time;
}

#ifdef PM_DEBUG
static void _changeStateDebug(PTRStateFunc state, int time, int line)
{
    OHNO_PRINT("CF field_state: %d %d\n",line, CommGetCurrentID());
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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer!=0){
        pCFS->timer--;
        return;
    }
    if(!CommIsInitialize()){
        return;  //
    }
//    if(!CommGetAloneMode()){   // alone���[�h�̏ꍇ
//        return;
//    }    
    CommUnderOptionReInit(pCFS->pFSys);  // �n���T�[�r�X�ēx������

    // �������g�ɂ������񑗂�
    CommInfoSendPokeData();
    CommPlayerSendPos(FALSE);
    CommTrapInfoChildSendStart();  //Trap���������g�ɑ��M�J�n
    CommSecretBaseInfoChildSendStart(); // �����̔閧��n�f�[�^�����̐e���ɑ��M�J�n

    if(!SysFlag_UgFirstCheck(SaveData_GetEventWork(pCFS->pFSys->savedata))){
        // �Ђ傤���悤�X�e�[�g
        _CHANGE_STATE(_underOpenning, 0);
    }
    else{
        CommPlayerFirstMoveEnable();
        _CHANGE_STATE(_underConnecttingAloneSend, _CHILD_P_SEARCH_TIME*2);
    }
}

//==============================================================================
/**
 * openiing���I���܂łȂɂ����Ȃ�  �Ȃ���Ȃ��悤�ɂ����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underOpenning(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(!SysFlag_UgFirstCheck(SaveData_GetEventWork(pCFS->pFSys->savedata))){
        return;
    }
    CommStateUnderRestart();
    CommPlayerFirstMoveEnable();
    
    _CHANGE_STATE(_underConnectting, _CHILD_P_SEARCH_TIME*2);
}



//==============================================================================
/**
 * �����Ŏ����Ƀf�[�^�𑗐M���I���܂ő҂�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underConnecttingAloneSend(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer!=0){
        pCFS->timer--;
        return;
    }
    _CHANGE_STATE(_underConnectting, _CHILD_P_SEARCH_TIME*2);
}

//==============================================================================
/**
 * �l�b�g���[�N���Ȃ���܂őҋ@
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underConnectting(void)
{
    CommUnderProcess();

    CommPlayerFirstMoveEnable();
    
    if(CommIsUnderGroundConnectingState()){  // �ڑ�����܂ŌĂё�����
        if(CommGetCurrentID() == COMM_PARENT_ID){
          //  OHNO_PRINT("�e�ɂȂ���\n");
            CommSecretBaseInfoParentDel();

            _CHANGE_STATE(_underParentWait, 60);
        }
        else{
          //  OHNO_PRINT("�q�ɂȂ���\n");
            _CHANGE_STATE(_underChildConnecting, 120);
        }
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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer!=0){
        pCFS->timer--;
    }

    if(CommMPParentDisconnect() || CommIsError() || (pCFS->timer==0) ){
        CommStateUnderRestart();
        _CHANGE_STATE(_underChildRestart, 0);  // �q�@�ɂȂ�O�ɃG���[�ɂȂ���
    }
    else if(CommIsConnect(CommGetCurrentID())){   // �������g���ڑ����Ă��邱�Ƃ��m�F�ł�����
        if(pCFS->timer!=0){
            pCFS->timer--;
            if(CommGetSendRestSize() != COMM_COMMAND_SEND_SIZE_MAX){  // ��ɂȂ�܂ő҂� �^�C�}�[��
                return;
            }
        }
        UgMgrForceExitNowTCB();
        CommPlayerMyDataCopy( COMM_PARENT_ID );  // �e�f�[�^���玩���������Ă���
        CommTrapRecvForceEnd(COMM_PARENT_ID,0,NULL,NULL);  // 㩂��~�߂�
        UgTrapForceExit(COMM_PARENT_ID,TRUE);
        CommInfoDeletePlayer(COMM_PARENT_ID);   // �������e�������̂ł��������
        CommPlayerDestroy( COMM_PARENT_ID, FALSE, FALSE );
//        CommPlayerSendPos(FALSE);
        //_debugTimerDisp("�q�@�ڑ�----!!!-----");
  //      OHNO_PRINT("�q�@�ɂȂ��� %d \n",CommGetCurrentID());
        CommPlayerFirstMoveDisable();
        _CHANGE_STATE(_underChildConnectInfoWait, 10);
        return;
    }
//    pCFS->timer--;
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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommUnderProcess();

    if(pCFS->timer!=0){
        pCFS->timer--;
    }

    if(CommIsChildsConnecting() || (pCFS->timer==0)){
        // �����ȊO���Ȃ�������e�@�Œ�
        // �������͎q�@���ؒf���Ă��܂����ꍇ�̃^�C���A�E�g�G���[�I�Ɉړ�
        if(UgMgrForceExitNowTCB()){
            CommPlayerHoldEnd();
        }
        _debugTimerDisp("�e�@�ڑ�");
//        CommInfoSendPokeData();
        CommPlayerSendPos(FALSE);
//        CommTrapInfoChildSendStart();  //Trap���������g�ɑ��M�J�n
        CommPlayerNowFlagSend();
        CommSecretBaseInfoChildSendStart(); // �����̔閧��n�f�[�^�����̐e���ɑ��M�J�n
//        UgDigFossilPlayerInit();
        UgInitialDataSend();   // ���΁{�G�X�P�[�v�ꏊ�𑗐M
        _CHANGE_STATE(_underParentConnect, 0);
        return;
    }

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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommUnderProcess();

    // �S���ڑ����؂ꂽ�珄��ɖ߂邪�C�x���g���͋֎~
//    if((pCFS->pFSys->event != NULL) || UgSecretBaseIsMoveTickets()){  // �n���ړ��C�x���g���ɃX�e�[�g��ς���̂͋֎~
    if(pCFS->pFSys->event != NULL){  // �n���ړ��C�x���g���ɃX�e�[�g��ς���̂͋֎~
        return;
    }

    if(CommIsError() || !CommMPIsChildsConnecting() || ((!CommIsConnect(CommGetCurrentID()) && !CommGetAloneMode())) ){

//        OHNO_PRINT("�Ȃ����q�@���ؒf���� %d %d\n",CommIsError(),CommMPIsChildsConnecting());
        
        // �閧��n����߂�E�C���h�E���o�Ă���Ɛe�@�̈ړ��t���O�������Ă���̂ł����Ń��Z�b�g
        UgSecretBaseResetPlayer(COMM_PARENT_ID); // �C�x���g�������ĂȂ��Ȃ烊�Z�b�g

        CommPlayerParentFlagDataReset();
        UgSecretBaseErrorMoveTickets();
        CommSendFixData(CF_TRAP_END_FORCE);    // �T�[�o�[������������
        UgTrapForceExit(CommGetCurrentID(),TRUE);
        UgMgrForceExitNowTCB();
        CommStateUnderParentRestart();
#if (T1645_060815_FIX | T1644_060816_FIX)
        _CHANGE_STATE(_underParentRestartAloneWait, 0);
#else // (T1645_060815_FIX | T1644_060816_FIX)
        _CHANGE_STATE(_underParentRestart, 1);
#endif // (T1645_060815_FIX | T1644_060816_FIX)
    }
}

//==============================================================================
/**
 * �e�@�Ƃ��Đڑ���   �閧��n�ւ̈ړ��J�n
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentConnect_JumpFieldStart(void)
{
    //CommSecretBaseInfoReset();
    //CommTrapInfoReset();
    CommPlayerManagerReset();
    _CHANGE_STATE(_underParentConnect_JumpField, 0);
}

//==============================================================================
/**
 * �e�@�Ƃ��Đڑ���   �閧��n�ւ̈ړ���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentConnect_JumpField(void)
{
    CommUnderProcess();
}

//==============================================================================
/**
 * �e�@�Ƃ��Đڑ���   �閧��n�ւ̈ړ��I���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentConnect_JumpFieldEnd(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommSecretBaseInfoJumpEnd();
    //CommSecretBaseInfoReboot();
    //CommTrapInfoReboot();
    CommPlayerManagerReboot();

#ifdef PM_DEBUG
    if(DebugOhnoGetCommDebugDisp()){
        DebugIchiCheckOn(pCFS->pFSys);
    }
#endif
    _CHANGE_STATE(_underParentConnect, 0);
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ��������Ă��ā@info�f�[�^���W�߂Ă���Ƃ���
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnectInfoWait(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer == 9){
        CommInfoSendPokeData();  //info�f�[�^�𑗂�
        UgInitialDataSend();   // ���΁{�G�X�P�[�v�ꏊ�𑗐M
    }
    if(pCFS->timer == 1){
        //UgDigFossilPlayerInit();// ���Ό@���Ă邩�ǂ����𑗂�
        CommPlayerSendPosServer(FALSE);  //�e�X�g
    }
    if(pCFS->timer!=0){
        pCFS->timer--;
        return;
    }
    if(CommInfoIsInfoCollected()){
        _debugTimerDisp("info�f�[�^��M����");
        CommPlayerNowFlagSend();
        _CHANGE_STATE(_underChildConnectFlagWait,200);
        return;
    }
    _underChildConnect();
}


//==============================================================================
/**
 * �q�@�Ƃ��Đڑ��������Ă��ā@�n�^�f�[�^�𑗂鏊
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnectFlagWait(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(CommPlayerIsNowFlagDataEnd()){
        CommTrapInfoChildSendStart();  // 㩏�񑗐M
        _CHANGE_STATE(_underChildConnectSecret,0);
    }
    else{
        _underChildConnect();
        if(pCFS->timer!=0){
            pCFS->timer--;
        }
        else{
            // �^�C���A�E�g�ɂ��ؒf
        //    OHNO_PRINT("�����Ƃ��ĂȂ�\n");
            _underChildRebootFunc();
        }
    }
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ�  㩑��M����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnectSecret(void)
{
    if(CommTrapInfoIsParentRecv()){
        _debugTimerDisp("�e�@㩃f�[�^�͂����炵��");
        CommTrapInfoResetParentRecv();
        CommSecretBaseInfoChildSendStart(); // �q�@�̔閧��n�f�[�^���M�J�n
        _CHANGE_STATE(_underChildConnectDig,0);
        return;
    }
    _underChildConnect();
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ� �閧��n���M����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnectDig(void)
{
    if(CommSecretBaseInfoIsPosRecv()){
        _debugTimerDisp("�閧��n�̈ʒu������");
        CommSecretBaseResetPosRecv();
     //   CommPlayerSetMoveControl(TRUE);
        CommPlayerSendPos(TRUE);  // �L�������o���ׂɂ�����񑗂�@�e�X�g��
        CommSendFixData(CF_PLAYER_INIT_END);  // �����������𑗐M
        _CHANGE_STATE(_underChildConnect,0);
        return;
    }
    _underChildConnect();
}

#if (T1645_060815_FIX | T1644_060816_FIX)

//==============================================================================
/**
 * �e�@�Ƃ��Đؒf������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentRestartAloneWait(void)
{
    if(!CommGetAloneMode()){
//        OHNO_PRINT("s wait\n");
        return;
    }
#if T1644_060816_FIX
    CommPlayerSetMoveControl_Server(COMM_PARENT_ID,FALSE);
    _CHANGE_STATE(_underParentRestart,30);
#else //T1644_060816_FIX
    _CHANGE_STATE(_underParentRestart,10);
#endif  //T1644_060816_FIX

}

//==============================================================================
/**
 * �e�@�Ƃ��Đؒf������̂�҂�
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentRestart(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
#if T1644_060816_FIX
    if(pCFS->timer!=0){
        pCFS->timer--;
        return;
    }
#else //T1644_060816_FIX
    if(pCFS->timer!=0){
        pCFS->timer--;
        if(!CommIsEmptyQueue()){
        //    OHNO_PRINT("not e\n");
            return;
        }
    }
#endif //T1644_060816_FIX
    if(CommGetCurrentID() == COMM_PARENT_ID){  // �e�ɂȂ���
      //  OHNO_PRINT("�ăX�^�[�g\n");
        _CHANGE_STATE(_underStart, 0);
    }
}

#else  //(T1645_060815_FIX | T1644_060816_FIX)

//==============================================================================
/**
 * �e�@�Ƃ��Đؒf������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underParentRestart(void)
{
//    if(CommIsUnderResetState()){
  //      return;
 //   }
//    if(!CommIsConnect(CommGetCurrentID()) && !CommGetAloneMode()){
    if(!CommGetAloneMode()){
     //   OHNO_PRINT("s wait\n");
        return;
    }
    if(!CommIsEmptyQueue()){
       // OHNO_PRINT("not e\n");
        return;
    }
    
    if(CommGetCurrentID() == COMM_PARENT_ID){  // �e�ɂȂ���
     //   OHNO_PRINT("�ăX�^�[�g\n");
        _CHANGE_STATE(_underStart, 0);
    }
}

#endif //(T1645_060815_FIX | T1644_060816_FIX)

//==============================================================================
/**
 * �q�@�Ƃ��Đؒf������
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildRestart(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer!=0){
        pCFS->timer--;
        return;
    }
    if(CommGetCurrentID() == COMM_PARENT_ID){  // �e�ɂȂ���
   //     OHNO_PRINT("�ăX�^�[�g\n");
        _CHANGE_STATE(_underStart, 0);
    }
}

static void _underChildRestart_Base(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->pFSys->event == NULL ){  // �n���ړ��C�x���g�I��
        CommStateUnderRestart();  // �n���̒ʐM��e�q�؂�ւ���
        _CHANGE_STATE(_underChildRestart, 0);
    }
}



static void _underChildStalthDeadLoop(void)
{
    CommUnderProcess();
}

static void _underChildStalth(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(CommGetCurrentID() == COMM_PARENT_ID){
//        CommUnderOptionInitialize(pCFS->pFSys);  // �n���T�[�r�X�̋N��

        CommUnderOptionReInit(pCFS->pFSys);  // �n���T�[�r�X�ēx������

        // �������g�ɂ������񑗂�
        CommInfoSendPokeData();
        CommPlayerSendPos(FALSE);
        CommTrapInfoChildSendStart();  //Trap���������g�ɑ��M�J�n
        CommSecretBaseInfoChildSendStart(); // �����̔閧��n�f�[�^�����̐e���ɑ��M�J�n
        _CHANGE_STATE(_underChildStalthDeadLoop, 0);
    }
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ�����
 * @param   none
 * @retval  none
 */
//==============================================================================



//==============================================================================
/**
 * �q�@�Ƃ��Đڑ�����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildRebootFunc(void)
{
#if AFTER_MASTER_070202_FOSSILLOG_FIX
    UgTrapForceExit(CommGetCurrentID(),TRUE);
    UgSecretBaseRemovePlayer_Client(CommGetCurrentID(),TRUE);
    UgDigFossilDeleteLog();
    CommPlayerManagerStop();  // �ړ���~
    UgMgrForceExitNowTCB();   // �o�^�C�x���g���I��������
    CommStateUnderRestart();  // �n���̒ʐM��e�q�؂�ւ���
    CommPlayerFlagChange();   // �����̃n�^��}���ւ���
    _CHANGE_STATE(_underChildRestart, 0);//
#else
    UgTrapForceExit(CommGetCurrentID(),TRUE);
    if(UgSecretBaseRemovePlayer_Client(CommGetCurrentID(),TRUE)){
        CommPlayerManagerStop();  // �ړ���~
        UgMgrForceExitNowTCB();   // �o�^�C�x���g���I��������
        CommStateUnderRestart();  // �n���̒ʐM��e�q�؂�ւ���
        CommPlayerFlagChange();   // �����̃n�^��}���ւ���
        _CHANGE_STATE(_underChildRestart, 0);//
    }
    else{
  //      OHNO_PRINT("�ؒf�ċN�� %d\n",CommGetCurrentID());
        CommPlayerManagerStop();  // �ړ���~
        UgMgrForceExitNowTCB();   // �o�^�C�x���g���I��������
        CommStateUnderRestart();  // �n���̒ʐM��e�q�؂�ւ���
        CommPlayerFlagChange();   // �����̃n�^��}���ւ���
        _CHANGE_STATE(_underChildRestart, 0);
    }
#endif
}


static void _underChildConnect(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommUnderProcess();

    CommPlayerFirstMoveEnable();
    
    // �e�@�����Ȃ��Ȃ�����I�����������ĕʐe��T���ɂ���
    if(pCFS->pFSys->event != NULL ){  // �n���ړ��C�x���g���ɃX�e�[�g��ς���̂͋֎~
        return;
    }
    if(CommMPParentDisconnect() || CommIsError()){
        _underChildRebootFunc();  // �ċN��
    }
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnect_JumpFieldStart(void)
{
    //CommSecretBaseInfoReset();
    //CommTrapInfoReset();
    CommPlayerManagerReset();
    _CHANGE_STATE(_underChildConnect_JumpField,0);
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnect_JumpField(void)
{
    // �o���邾���������Ȃ�
    UgSecretBaseErrorDisconnectTickets();
}

//==============================================================================
/**
 * �q�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underChildConnect_JumpFieldEnd(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommSecretBaseInfoJumpEnd();
 //   CommSecretBaseInfoReboot();
//    CommTrapInfoReboot();
    CommPlayerManagerReboot();
#ifdef PM_DEBUG
    if(DebugOhnoGetCommDebugDisp()){
        DebugIchiCheckOn(pCFS->pFSys);
    }
#endif

    if(CommMPParentDisconnect() || CommIsError()){
   //     OHNO_PRINT("�ؒf�ċN�� %d\n",CommGetCurrentID());
        
        CommSecretBaseInfoReInit();  // 
        CommPlayerManagerStop();  // �ړ���~
        UgMgrForceExitNowTCB();   // �o�^�C�x���g���I��������
        CommStateUnderRestart();  // �n���̒ʐM��e�q�؂�ւ���
        CommPlayerFlagChange();   // �����̃n�^��}���ւ���
        _CHANGE_STATE(_underChildRestart, 0);
    }
    else{
        _CHANGE_STATE(_underChildConnect, 0);
    }
}



//==============================================================================
/**
 * solo�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underSoloConnect_JumpFieldStart(void)
{
    CommPlayerManagerReset();
    _CHANGE_STATE(_underSoloConnect_JumpField,0);
}

//==============================================================================
/**
 * solo�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underSoloConnect_JumpField(void)
{
}

//==============================================================================
/**
 * solo�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underSoloConnect_JumpFieldEnd(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommSecretBaseInfoJumpEnd();
    CommPlayerManagerReboot();
#ifdef PM_DEBUG
    if(DebugOhnoGetCommDebugDisp()){
        DebugIchiCheckOn(pCFS->pFSys);
    }
#endif
    _CHANGE_STATE(_underConnectting, 0);
}

//==============================================================================
/**
 * stalth�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underStalthConnect_JumpFieldStart(void)
{
    CommPlayerManagerDeletePlayers();
    _CHANGE_STATE(_underStalthConnect_JumpField,0);
}

//==============================================================================
/**
 * stalth�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underStalthConnect_JumpField(void)
{
}

//==============================================================================
/**
 * solo�@�Ƃ��Đڑ����@�@�@JUMP����
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _underStalthConnect_JumpFieldEnd(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    CommSecretBaseInfoJumpEnd();
    CommPlayerManagerReboot();
#ifdef PM_DEBUG
    if(DebugOhnoGetCommDebugDisp()){
        DebugIchiCheckOn(pCFS->pFSys);
    }
#endif
    _CHANGE_STATE(_underChildStalthDeadLoop, 0);
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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer != 0){
        pCFS->timer--;
        return;
    }

    CommUnderOptionFinalize();
    CommStateExitUnderGround();

    sys_DeleteHeap(HEAPID_UNDERGROUND);

    pCFS->bUGOverlay = FALSE; // �n�����~�߂�

    _stateFinalize();   // state�͂����ŏI�� �ʐM�͌ォ��؂��
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
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    if(pCFS->timer != 0){
        pCFS->timer--;
        return;
    }
    // �ؒf����
    CommFinalize();
    _CHANGE_STATE(_stateEnd, 0);
}

#ifdef PM_DEBUG
static void _debugTimerStart(void)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    pCFS->debugTimer=0;
}

static void _debugTimerDisp(char* msg)
{
    _COMM_FSTATE_WORK *pCFS = _getCommFieldStateWork();
    OHNO_PRINT("%s �J�n���� %d sync ������܂���\n", msg, pCFS->debugTimer * 2);
}

#endif


