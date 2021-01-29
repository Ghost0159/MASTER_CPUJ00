//=============================================================================
/**
 * @file	comm_player.c
 * @bfief	�ʐM�ő��葤�̃v���[���[��\������
 *            �R�~���j�P�[�V�����v���[���[
 * @author	katsumi ohno
 * @date	05/07/14
 */
//=============================================================================

#include <nitro/os.h>
#include "common.h"
#include "gflib/blact.h"

#include "communication/communication.h"
#include "communication/comm_state.h"
#include "field/comm_field_state.h"
#include "field/underground/ug_manager.h"

#include "system/bmp_menu.h"
#include "field/worldmap.h"
#include "field/fieldobj.h"
#include "field/fieldobj_code.h" //AC_WALK...�� �A�j���[�V�����R�[�h���܂Ƃ߂Ă���܂��B
#include "field/fieldsys.h"
#include "field/fieldmap.h"
#include "field/fld_bmp.h"
#include "system/pm_str.h"
#include "system/fontproc.h"
#include "system/bmp_list.h"
#include "system/window.h"

#include "field/player.h"
#include "field/comm_player_condition.h"
#include "field/comm_direct_counter.h"
#include "system/snd_tool.h"
#include "system/wordset.h"
#include "msgdata/msg_underworld.h"
#include "msgdata/msg_uwflag.h"

#include "field/field_effect.h"
#include "field/syswork.h"

#include "field/talk_msg.h"
#include "field/fldeff_flag.h"
#include "system/clact_tool.h"
#include "field/field_clact.h"
#include "field/fld_comact.h"
#include "field/underground/ug_local.h"
#include "field/eventdata.h"
#include "field/tv_topic.h"
#include "field/sysflag.h"

#include "field/d_ohno.h"  // �f�o�b�O�p

#include <nnsys/g3d/glbstate.h>


//==============================================================================
// ��`
//==============================================================================
#include "field/comm_player_local.h"

extern CommPlayerWork* CommPlayerGetInstance(void);

//==============================================================================
/**
 * �閧��n�Ɍ�����Ă����ꍇ�̏���
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerManagerDeletePlayers(void)
{
    int i;
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP==NULL){
        return;
    }
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        CommPlayerDestroy( i, FALSE ,FALSE);
        OHNO_SP_PRINT("���Ȃ����܂� %d \n",i);
        CommPlayerResetHole( i );
    }
    pCP->bResetting = TRUE;
}


//--------------------------------------------------------------
/**
 * ����ۑ�����
 * @param	netID   playerID
 * @retval	��������TRUE
 */
//--------------------------------------------------------------
static void _commPlayerDeliveryFlagData(int netID)
{
    int i;
    CommPlayerWork* pCP = CommPlayerGetInstance();

    if(netID == CommGetCurrentID()){  // �L�^
        if(pCP->pNowFlag[netID] != NULL){

            RECORD_Score_Add(SaveData_GetRecord(pCP->pFSys->savedata), SCORE_ID_NEW_FLAG_TAKE_OUT);

#if AFTERMASTER_070206_UNDERGROUND_PCFLAGLIST2ND_FIX
#else
            for(i = 0; i < _KEEP_FLAG_MAX; i++){
                if(pCP->pKeepFlag[i]){
                    if(MyStatus_Compare(pCP->pNowFlag[netID], pCP->pKeepFlag[i]) == TRUE){
                        // ���S��v �͍��̊�����������
                        CommPlayerDeleteFlagData(netID);
                        return;
                    }
                }
            }
#endif
            if(pCP->pKeepFlag[_KEEP_FLAG_MAX-1]){
                sys_FreeMemoryEz(pCP->pKeepFlag[_KEEP_FLAG_MAX-1]);
            }
            for(i = _KEEP_FLAG_MAX - 1; i >= 1; i--){
                pCP->pKeepFlag[i] = pCP->pKeepFlag[i-1];
            }
            pCP->pKeepFlag[0] = pCP->pNowFlag[netID];
            UnderGroundAddFlag(SaveData_GetUnderGroundData(GameSystem_GetSaveData(pCP->pFSys)),
                               pCP->pNowFlag[netID]);
            pCP->pNowFlag[netID] = NULL;
            pCP->sendFlagData[netID].netID = INVALID_NETID;
        }
    }
    else{
        CommPlayerDeleteFlagData(netID);
    }
}

//--------------------------------------------------------------
/**
 * @brief   ����̓�����������
 * @param   myNetID   �����Ƃ�l
 * @param   targetID  �����Ƃ���l
 * @retval  �Ƃ�����TRUE
 */
//--------------------------------------------------------------

static BOOL _getHeadFlag(int myNetID, int targetNetID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();

    
    if((pCP->pNowFlag[myNetID] == NULL) &&
       (pCP->pNowFlag[targetNetID] != NULL)){
        pCP->pNowFlag[myNetID] = pCP->pNowFlag[targetNetID];
        pCP->pNowFlag[targetNetID] = NULL;
        pCP->hedFE[myNetID] = _HFE_FLAG;
        pCP->hedFE[targetNetID] = _HFE_NONE;
//        OHNO_PRINT("�͂��Ƃ�\n");
        pCP->sendFlagData[myNetID].netID = myNetID;
        pCP->sendFlagData[targetNetID].netID = INVALID_NETID;
        MyStatus_Copy(pCP->pNowFlag[myNetID], (MYSTATUS*)&pCP->sendFlagData[myNetID].mystatus);
        return TRUE;
    }
    return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   ���b�Z�[�W�������\�������ꍇ�̏I�����R�[���o�b�N
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------
static void _msgEndCallBack(int num)
{
    CommPlayerHoldEnd();
}

static void _msgEndCallBackDel(int num)
{
    CommPlayerHoldEnd();
}

static void _msgEndCallBackNoTalk(int num)
{
    CommPlayerHoldBitEnd(_HOLD_FLAG_NO_TALK);
}

static void _msgEndCallBackNoDig(int num)
{
    CommPlayerHoldBitEnd(_HOLD_FLAG_NO_DIG);
}

static void _msgEndCallBackRipOff(int num)
{
    CommPlayerHoldBitEnd(_HOLD_FLAG_RIP_OFF);
}


static void _msgEndCallBackStolenMy(int num)
{
    CommPlayerHoldBitEnd(_HOLD_FLAG_STOLEN_MY);
}

//==============================================================================
/**
 * ��b���X�g���I��������ɌĂяo���Ă��炤
 * @param   none
 * @retval  none
 */
//==============================================================================

static void _talkMenuEnd(int selectNo)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP){
        pCP->bTalkMenuDisp=FALSE;
    }
}

typedef struct{
    u8 bOpen;
    u8 netID;
    u8 targetID;
} _ugMenuOpen;

//--------------------------------------------------------------
/**
 * @brief   �Y�z�t�B�[���h���j���[���J���Ă悢���ǂ���  CF_CHECK_FIELDMENU_OPEN
 * @param   �R�[���o�b�N�p�����[�^�[
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerRecvCheckFieldMenuOpen(int netID, int size, void* pData, void* pWork)
{
    _ugMenuOpen menuOpen;
    CommPlayerWork* pCP = CommPlayerGetInstance();

    menuOpen.bOpen = _RESULT_NG;
    menuOpen.netID = netID;

    if(pCP->bMoveControl[ netID ] && !CommPlayerIsTrapBind(netID)){
        if(CommPlayerIsFlagData(netID)){
            menuOpen.bOpen = _RESULT_DELFLAG_OPEN;
        }
        else{
            menuOpen.bOpen = _RESULT_OK;
        }
    }
    if(CommSendData_ServerSide(CF_RESULT_FIELDMENU_OPEN, &menuOpen, 2)){
        if(menuOpen.bOpen!=_RESULT_NG){
            CommPlayerSetMoveControl_Server(netID, FALSE);            
//            _pCommPlayerWork->bMoveControl[ netID ] = FALSE;
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   START���j���[���J���Ă������ǂ����̃R�[���o�b�N CF_RESULT_FIELDMENU_OPEN
 * @param   
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerRecvResultFieldMenuOpen(int netID, int size, void* pData, void* pWork)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _ugMenuOpen* pMenuOpen = pData;

    if(!CommIsSendMoveData()){
        return;
    }
    if((pMenuOpen->bOpen == _RESULT_OK) && (pMenuOpen->netID == CommGetCurrentID())){
        if(!pCP->bTalkMenuDisp){
            pCP->bTalkMenuDisp = TRUE;
            //�n����p���j���[���o��
            CommUnderBagInit(_talkMenuEnd, pCP->pFSys);
        }
    }
    else if((pMenuOpen->bOpen == _RESULT_DELFLAG_OPEN) && (pMenuOpen->netID == CommGetCurrentID())){
        if(!pCP->bTalkMenuDisp){
            pCP->bTalkMenuDisp = TRUE;
            CommUnderBagFlagStart(_talkMenuEnd, pCP->pFSys);
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   �₢�������j���[���J���Ă������ǂ����̃R�[���o�b�N
 *          CF_RESULT_UGMENU_OPEN
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerRecvResultUGMenuOpen(int netID, int size, void* pData, void* pWork)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _ugMenuOpen* pMenuOpen = pData;
    EVENTWORK* pEV = SaveData_GetEventWork( pCP->pFSys->savedata );
    GF_ASSERT_RETURN(pMenuOpen->netID < COMM_MACHINE_MAX,);

//    OHNO_PRINT("��b�J�n �N���C�A���g\n");

    if((pMenuOpen->bOpen == _RESULT_FOSSIL) && (pMenuOpen->netID == CommGetCurrentID())){
        CommPlayerHold();
        CommMsgTalkWindowStart(CommUnderGetMsgUnderWorld(),
                               msg_underworld_72,TRUE,_msgEndCallBack);
    }
    if(pMenuOpen->bOpen == _RESULT_OK){    // ����������������
        CommPlayerSetLook(pMenuOpen->netID, pMenuOpen->targetID);
    }
    if((pMenuOpen->bOpen == _RESULT_OK) && (pMenuOpen->netID == CommGetCurrentID())){
        CommUnderTalkInit(pMenuOpen->targetID , _talkMenuEnd, pCP->pFSys);
        pCP->talkMenuInfo.netID = pMenuOpen->netID ;
        pCP->talkMenuInfo.targetID = pMenuOpen->targetID;
        pCP->talkMenuInfo.bItemMenu = FALSE;
        pCP->talkMenuInfo.bPersonalInformation = FALSE;
        if(pCP->handCount[pMenuOpen->targetID] == 0){
            SecretBaseRecordSetTalkNum(SaveData_GetSecretBaseRecord(GameSystem_GetSaveData(pCP->pFSys)),pMenuOpen->targetID); // �b�������J�n
            SysWork_UGTalkCountSet(pEV,SysWork_UGTalkCountGet(pEV)+1);
            SysWork_UGTalkCount2Set(pEV,SysWork_UGTalkCount2Get(pEV)+1);
            pCP->handCount[pMenuOpen->targetID]++;
        }
    }
    if((pMenuOpen->bOpen == _RESULT_OK) && (pMenuOpen->targetID == CommGetCurrentID())){
        CommUnderTalkBind(pMenuOpen->targetID, pMenuOpen->netID, pCP->pFSys);
    }
    if((pMenuOpen->bOpen == _RESULT_NG) && (pMenuOpen->netID == CommGetCurrentID())){
        CommPlayerHold();
        CommMsgTalkWindowStart(CommUnderGetMsgUnderWorld(),
                               msg_underworld_03,TRUE,_msgEndCallBack);
    }
    if(pMenuOpen->bOpen == _RESULT_OK){
        UgMgrSetTalkLog(pMenuOpen->netID, pMenuOpen->targetID);
    }
}

//--------------------------------------------------------------
/**
 * @brief   ���Ɋւ����Ԃ��T�[�o�[����Ԃ��ꍇ�̃R�}���h�T�C�Y
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

typedef struct{
    u8 state;
    u8 myNetID;
    u8 targetID;
} _flagStateCallBack;

int CommPlayerRecvFlagStateServerRetGetSize(void)
{
    return sizeof(_flagStateCallBack);
}

//--------------------------------------------------------------
/**
 * @brief   ���Ɋւ����Ԃ��T�[�o�[�ɓ͂���  CF_FLAG_STATE
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerRecvFlagState(int netID, int size, void* pData, void* pWork)
{
    u8* pState = pData;
    _flagStateCallBack ret;

    ret.myNetID = netID;
    switch(pState[0]){
      case UNDER_FLAG_DEL:
        ret.state = UNDER_FLAG_DEL;
        CommSendFixSizeData_ServerSide(CF_FLAG_STATE_RET,&ret);  // ���������ׂɃN���C�A���g�ɑ��M
//        CommPlayerSetMoveControl_Server(netID,FALSE);
        break;
      case UNDER_FLAG_DELIVERY:
        ret.state = UNDER_FLAG_DELIVERY;
        CommSendFixSizeData_ServerSide(CF_FLAG_STATE_RET,&ret);  // ���������ׂɃN���C�A���g�ɑ��M
        CommPlayerSetMoveControl_Server(netID,FALSE);
        break;
    }
}

//--------------------------------------------------------------
/**
 * @brief   ���Ɋւ����Ԃ��T�[�o�[�ɓ͂���  CF_PLAYER_FREEZE_END
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

//void CommPlayerRecvFreezeEnd(int netID, int size, void* pData, void* pWork)

static void _commPlayerFreeze(int num)
{
//    u8* pBuff = pData;

    CommPlayerSetFENoneAdd(CommGetCurrentID()); // �I����
    CommMsgTalkWindowEnd(CommUnderGetMsgUnderFlag());
    CommPlayerHoldBitEnd(_HOLD_FLAG_STOLEN);
}


//--------------------------------------------------------------
/**
 * @brief   �n�^�����ꂽ�̂ň�b�ȏ�ł܂�
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

static void _freezePlayer(TCB_PTR tcb, void* work)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _FREEZE_PLAYER* pFreeze = work;
    u8 id = pFreeze->targetID;
    u8 bEnd = FALSE;

    if(!CommIsConnect(id)){
        sys_FreeMemoryEz(pFreeze);
        TCB_Delete(tcb);
        return;
    }
    else{
        pFreeze->timer++;
        if(pFreeze->timer > 60){
            bEnd = TRUE;
        }
    }
    if(bEnd){
        pCP->bMoveControlFlag[ id ]=TRUE;
        sys_FreeMemoryEz(pFreeze);
        TCB_Delete(tcb);
    }
}

//--------------------------------------------------------------
/**
 * @brief   �n�^�����ꂽ�̂ň�b�ȏ�ł܂�
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

static void _freezeConditionStart(int targetID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _FREEZE_PLAYER* pFreeze = sys_AllocMemory(HEAPID_WORLD, sizeof(_FREEZE_PLAYER));
    pFreeze->timer = 0;
    pFreeze->targetID = targetID;
    pCP->bMoveControlFlag[ targetID ] = FALSE;
    TCB_Add(_freezePlayer, pFreeze, TCB_PRIORITY_NORMAL);
    CommPlayerSetMoveControl_Server(targetID,FALSE);
    
}

//--------------------------------------------------------------
/**
 * @brief   ���������Ă���l�Ƃ̌���
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

BOOL CommPlayerFlagTalkCheck(int netID, int targetID, BOOL bNPCTalk)
{
    _flagStateCallBack ret;

    if(bNPCTalk & 0x01){
        ret.state = UNDER_FLAG_NOTALK;
    }
    else if(CommPlayerIsFlagData(netID)){ // ���������������Ă���ꍇ�͂���
        ret.state = UNDER_FLAG_NOTALK;
    }
    else{
        if(targetID == COMM_INVALID_ID){
            return FALSE;
        }
        else if(!CommPlayerIsFlagData(targetID)){ // ���肪���������ĂȂ�
            return FALSE;
        }
        else{
            ret.state = UNDER_FLAG_RIP_OFF;
            _freezeConditionStart(targetID);
        }
    }
    ret.myNetID = netID;
    ret.targetID = targetID;
    CommPlayerSetMoveControl_Server(netID,FALSE);
    CommSendFixSizeData_ServerSide(CF_FLAG_STATE_RET,&ret);  // �N���C�A���g�ɑ��M
    return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �ق�邩�ǂ���  �������Ă���ق�Ȃ�
 * @param   id     �l
 * @retval  �ʐM��Ԃ�����TRUE
 */
//--------------------------------------------------------------

BOOL CommPlayerFlagDigCheck(int netID)
{
    _flagStateCallBack ret;

    if(CommPlayerIsFlagData(netID)){ // ���������������Ă���ꍇ�͂���
        ret.state = UNDER_FLAG_NODIG;
    }
    else{
        return FALSE;
    }
    ret.myNetID = netID;
    CommSendFixSizeData_ServerSide(CF_FLAG_STATE_RET,&ret);  // �N���C�A���g�ɑ��M
    return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �n�^�𓐂܂ꂽ�l�̏���  
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

static void _stolenFlagPlayerFunc(_flagStateCallBack* pRet, BOOL bGet, SB_RECORD_DATA* pSBD)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();

    UgMgrForceExitNowTCB();
#if T1656_060815_FIX
    CommPlayerSetMoveControl(FALSE);  // �T�[�o�[�ɑ��M
#endif  //T1656_060815_FIX
    CommPlayerHoldBit(_HOLD_FLAG_STOLEN);
    CommMsgRegisterTargetName(CommUnderGetMsgUnderFlag(),CommInfoGetMyStatus(pRet->myNetID));
    if(bGet){
        pCP->hedFE[CommGetCurrentID()] = _HFE_NONE;
        CommPlayerSetFENoneAdd(CommGetCurrentID()); // ������
        CommPlayerSetFEExclamationAdd(CommGetCurrentID()); //!
        CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),  // �D��ꂽ
                               mes_uw_flag_15, TRUE, _commPlayerFreeze);
    }
    else{
        CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),  // �D��ꂽ
                               mes_uw_flag_16, TRUE, _commPlayerFreeze);
    }
    Snd_FadeOutNextPlaySet(SND_SCENE_FIELD,SEQ_TANKOU,BGM_FADE_TANKOU_TIME,0,0xff,NULL);
    Snd_SePlay(UG_SE_FLAG_STEAL);
}

//--------------------------------------------------------------
/**
 * @brief   ���Ɋւ����Ԃ��N���C�A���g�ɓ͂���  CF_FLAG_STATE_RET
 * @param   �ʐM�R�[���o�b�N
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerRecvFlagStateServerRet(int netID, int size, void* pData, void* pWork)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _flagStateCallBack* pRet = pData;
    BOOL bDeliv;
    int point = 0;
    SB_RECORD_DATA* pSBD = SaveData_GetSecretBaseRecord(GameSystem_GetSaveData(pCP->pFSys));

    switch(pRet->state){
      case UNDER_FLAG_DEL:
        if(pCP->pNowFlag[pRet->myNetID]){ // �����̃n�^������ꍇ
            CommPlayerDeleteFlagData(pRet->myNetID);
            CommPlayerSetFENoneAdd(pRet->myNetID);
            if(pRet->myNetID == CommGetCurrentID()){
                CommPlayerHold();
                CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),
                                       mes_uw_flag_07, TRUE, _msgEndCallBackDel);
                Snd_FadeOutNextPlaySet(SND_SCENE_FIELD,SEQ_TANKOU,BGM_FADE_TANKOU_TIME,0,0xff,NULL);
            }
        }
//        OHNO_PRINT("�͂��̂Ă�\n");
        break;
      case UNDER_FLAG_RIP_OFF:  // �D������
        // �N���C�A���g�Ǘ��Ȃ̂Ŏ����������Ă����ꂽ���Ƃɂ���
        if(CommInfoGetMyStatus(pRet->myNetID)==NULL){
            //�D�����l�����Ȃ��ꍇ  ����̃n�^���������Ċ���
            CommPlayerDeleteFlagData(pRet->targetID);
//            OHNO_PRINT("�D�����������Ă����\n");
            return;
        }
        if(pCP->pNowFlag[pRet->targetID]==NULL){ // ����̃n�^�������ꍇ
            // �I��
            if(pRet->myNetID==CommGetCurrentID()){
                CommPlayerHoldEnd();
            }
//            OHNO_PRINT("�͂��Ȃ�\n");
            return;
        }
        if(MyStatus_Compare(pCP->pNowFlag[pRet->targetID],
                            CommInfoGetMyStatus(pRet->myNetID)) == TRUE){  // ���͎����̊�������
            // ����̊�������
            CommPlayerDeleteFlagData(pRet->targetID);
            if(pRet->myNetID == CommGetCurrentID()){
                SecretBaseRecordSetFlagReverseNum(pSBD);   // �D�����������߂���
                CommPlayerHold();
                CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),  // �Ƃ���ǂ���
                                              mes_uw_flag_18, TRUE, _msgEndCallBack);
                Snd_SePlay(UG_SE_FLAG_CONQUER);
            }
            else if(pRet->targetID == CommGetCurrentID()){
                _stolenFlagPlayerFunc(pRet, FALSE, pSBD); // ���ꂽ��
            }
            UgSecretBaseSetFlagReverseLog(pRet->myNetID);//�Ƃ���ǂ���
        }
        else if(_getHeadFlag(pRet->myNetID,pRet->targetID)){  // �n�^����ꂽ
            if(pRet->myNetID == CommGetCurrentID()){
                SecretBaseRecordSetFlagConquerNum(pSBD);  // �D����
                CommPlayerHoldBit(_HOLD_FLAG_RIP_OFF);
                CommMsgRegisterTargetName(CommUnderGetMsgUnderFlag(),CommInfoGetMyStatus(pRet->targetID));
                CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),  // �D����
                                              mes_uw_flag_17, TRUE, _msgEndCallBackRipOff);
                Snd_BgmPlay( SEQ_HATANIGE );
                Snd_SePlay(UG_SE_FLAG_CONQUER);
            }
            else if(pRet->targetID == CommGetCurrentID()){
                _stolenFlagPlayerFunc(pRet, TRUE, pSBD); // ���ꂽ��
            }
//            OHNO_PRINT("�D����\n");
            UgSecretBaseSetFlagConquerLog(pRet->myNetID, pRet->targetID);//���΂���
        }
        break;
      case UNDER_FLAG_NOTALK:
        if(pRet->myNetID == CommGetCurrentID()){
            CommPlayerHoldBit(_HOLD_FLAG_NO_TALK);
            CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),
                                   mes_uw_flag_11, TRUE, _msgEndCallBackNoTalk);
            //UgMgrSetNowTCB();//
        }
        break;
      case UNDER_FLAG_NODIG:
        if(pRet->myNetID == CommGetCurrentID()){
            CommPlayerHoldBit(_HOLD_FLAG_NO_DIG);
            CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),
                                   mes_uw_flag_10, TRUE, _msgEndCallBackNoDig);
        }
        break;
      case UNDER_FLAG_DELIVERY:
        pCP->hedFE[pRet->myNetID] = _HFE_NONE;
        CommPlayerSetFENoneAdd(pRet->myNetID); // ������

        if(pRet->myNetID == CommGetCurrentID()){  // �\��
            SB_RECORD_DATA* pSBD = SaveData_GetSecretBaseRecord(GameSystem_GetSaveData(pCP->pFSys));
            u8 type = SecretBaseRecordGetFlagType(pSBD);
            SecretBaseRecordSetFlagDeliveryNum(pSBD);  // �^��
            SysFlag_UgHataSet(SaveData_GetEventWork(pCP->pFSys->savedata)); // �^��
            if(pCP->pNowFlag[pRet->myNetID]){            // �n�^���^�Ԃ��тɌĂ΂��
                TVTOPIC_Entry_Watch_GetFlag(pCP->pFSys,pCP->pNowFlag[pRet->myNetID]);
                if(pCP->myDeliveryFlagNum!=0xffff){
                    pCP->myDeliveryFlagNum++;
                }
            }
            CommPlayerHold();
            Snd_SePlay(UG_SE_FLAG_DELIVERY);
            if(type == SecretBaseRecordGetFlagType(pSBD)){  // �n�^����������
                CommMsgTalkWindowStart(CommUnderGetMsgUnderFlag(),
                                       mes_uw_flag_12, TRUE, _msgEndCallBack);
            }
            else{
                CommSendFixSizeData(CF_LVUP_FLAG, &type);  // LVUP���o
            }
            Snd_FadeOutNextPlaySet(SND_SCENE_FIELD,SEQ_TANKOU,BGM_FADE_TANKOU_TIME,0,0xff,NULL);
        }
        if(pCP->pNowFlag[pRet->myNetID]){
            if(MyStatus_Compare(pCP->pNowFlag[pRet->myNetID],
                                CommInfoGetMyStatus(CommGetCurrentID())) == TRUE){  // ���͎����̊�������
                TVTOPIC_Entry_Watch_LoseFlag(pCP->pFSys,CommInfoGetMyStatus(pRet->myNetID));
            }
        }
        _commPlayerDeliveryFlagData(pRet->myNetID);  // �L�^
        break;
    }
}

//--------------------------------------------------------------
/**
 * @brief   �e�@���̃n�^�v�[���f�[�^������
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerParentFlagDataReset(void)
{
    int i;
    CommPlayerWork* pCP = CommPlayerGetInstance();
    
    for(i = 0;i < COMM_MACHINE_MAX;i++){
        pCP->sendFlagData[i].netID = INVALID_NETID;
    }
}

//--------------------------------------------------------------
/**
 * @brief   �n�^���󂯎��o�b�t�@
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

u8* CommPlayerGetFlagRecvBuff( int netID, void* pWork, int size)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    return (u8*)&pCP->sendFlagData[COMM_MACHINE_MAX];
}

//--------------------------------------------------------------
/**
 * @brief   ����̃n�^�f�[�^���M
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommPlayerNowFlagSend(void)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();

    pCP->bFlagDataRecv = FALSE;
    if(pCP->pNowFlag[CommGetCurrentID()]){
        CommSendFixSizeData(CF_FLAG_NOWARRAY_REQ, pCP->pNowFlag[CommGetCurrentID()]); //�n�^���X�V
    }
    else{
        MYSTATUS* pMy = MyStatus_AllocWork(HEAPID_COMMUNICATION);
        STRBUF* pBuf = STRBUF_Create(20,HEAPID_COMMUNICATION);
        GF_ASSERT(pMy);
        GF_ASSERT(pBuf);
        MyStatus_SetMyNameFromString(pMy,pBuf);
        CommSendFixSizeData(CF_FLAG_NOWARRAY_REQ, pMy); //�n�^���X�V
        STRBUF_Delete(pBuf);
        sys_FreeMemoryEz(pMy);
    }
}

//--------------------------------------------------------------
/**
 * @brief   ����̃n�^�f�[�^�T�C�Y������
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

int CommPlayerNowFlagSize(void)
{
    return COMM_SEND_MYSTATUS_SIZE;
}

//--------------------------------------------------------------
/**
 * @brief   ����̃n�^�f�[�^��S���ɑ���
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommPlayerRecvNowFlagDataReq(int netID, int size, void* pData, void* pWork)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    int i;
    const MYSTATUS* pMyStatus = pData;
    STRBUF* pStrBuf;
    u8 id = netID;

//    OHNO_PRINT("size %d\n",size);
    

    if(pCP){
        MyStatus_Copy(pMyStatus, (MYSTATUS*)pCP->sendFlagData[netID].mystatus);

        pStrBuf = MyStatus_CreateNameString(pMyStatus, HEAPID_COMMUNICATION);
        if(STRBUF_GetLen(pStrBuf)!=0){  // ���O������ΐ������f�[�^
            pCP->sendFlagData[netID].netID = netID;
        }
        for(i = 0; i < COMM_MACHINE_MAX;i++){
//            OHNO_PRINT("������ %d\n", pCP->sendFlagData[i].netID);
            if(pCP->sendFlagData[i].netID != INVALID_NETID){


                pCP->sendFlagData[i].netID = i;
                CommSendHugeData_ServerSide(CF_FLAG_NOWARRAY,
                                            &pCP->sendFlagData[i],
                                            sizeof(_SEND_FLAG));
            }
        }
        CommSendData_ServerSide(CF_FLAG_NOWARRAY_END, &id,1);
        STRBUF_Delete(pStrBuf);
        pCP->bDataSendFlag = TRUE;
    }
    else{
        GF_ASSERT(0);
    }
}


//--------------------------------------------------------------
/**
 * @brief   ����̃n�^�f�[�^������
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommPlayerRecvNowFlagDataArray(int netID, int size, void* pData, void* pWork)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _SEND_FLAG* pBuff = pData;
    int i,id;

    if(pCP){
        id = pBuff->netID;
        if(pCP->pNowFlag[id]){
            sys_FreeMemoryEz(pCP->pNowFlag[id]);
        }
        pCP->pNowFlag[id] = MyStatus_AllocWork(HEAPID_COMMUNICATION);
        MyStatus_Copy((MYSTATUS*)pBuff->mystatus, pCP->pNowFlag[id]);

        pCP->hedFE[id] = _HFE_FLAG;
    }
}

//--------------------------------------------------------------
/**
 * @brief   ����̃n�^�f�[�^�T�C�Y
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

int CommPlayerNowFlagDataArraySize(void)
{
    return sizeof(_SEND_FLAG);
}

//--------------------------------------------------------------
/**
 * @brief   ���f�[�^��M�R�[���o�b�N
 */
//--------------------------------------------------------------

void CommPlayerRecvNowFlagDataEnd(int netID, int size, void* pData, void* pWork)
{
    u8* pBuff = pData;
    CommPlayerWork* pCP = CommPlayerGetInstance();

//    OHNO_PRINT("CommPlayerRecvNowFlagDataEnd�R�}���h��M %d\n",pBuff[0]);
    if(pBuff[0] == CommGetCurrentID()){
        pCP->bFlagDataRecv = TRUE;
    }
    pCP->bDataSendFlag = FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   ���f�[�^����M�������ǂ���
 * @retval  TRUE�Ȃ��M����
 */
//--------------------------------------------------------------

BOOL CommPlayerIsNowFlagDataEnd(void)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    return pCP->bFlagDataRecv;
}


//--------------------------------------------------------------
/**
 * @brief   �q�@�Ƃ��Ď����Ă�������e�@�i�\���j�Ɍq��
 *          �����̈ȊO�͖���
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerFlagChange(void)
{
    MYSTATUS* pMy = NULL;
    int i,j;
    int id = CommGetCurrentID();
    CommPlayerWork* pCP = CommPlayerGetInstance();

    // ���������Ă���u���Ă���
    if(pCP->pNowFlag[id]){
//        OHNO_PRINT("���������������Ă���\n");
        pMy = pCP->pNowFlag[id];
        pCP->pNowFlag[id] = NULL;
        pCP->hedFE[id] = _HFE_NONE;
        pCP->sendFlagData[id].netID = INVALID_NETID;
    }
    // �S������
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        CommPlayerDeleteFlagData(i);
    }
    if(pMy){
//        OHNO_PRINT("�ؒf�@�n�^���n\n");
        pCP->pNowFlag[COMM_PARENT_ID] = pMy;
        pCP->hedFE[COMM_PARENT_ID] = _HFE_FLAG;
        MyStatus_Copy(pMy, (MYSTATUS*)&pCP->sendFlagData[COMM_PARENT_ID].mystatus);
        pCP->sendFlagData[COMM_PARENT_ID].netID = COMM_PARENT_ID;
    }
}

//==============================================================================
/**
 * 㩂ɂ������Ă邩�ǂ���
 * @param   targetID
 * @retval  �������Ă�����TRUE
 */
//==============================================================================

BOOL CommPlayerIsTrapBind(int targetID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();

    if(pCP->pCondMgr){
        return !CommPlayerIsNormalCondition_ServerSide(
            CommPlayerCondGetWork(pCP->pCondMgr, targetID));
    }
    return FALSE;
}



//--------------------------------------------------------------
/**
 * @brief   �q�@����NPC�ɘb���������ۂɔ��ł��܂�
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------
void CommPlayerRecvOtherTalk(int netID, int size, void* pData, void* pWork)
{
    u8 id = netID;
    
//    CommPlayerSetMoveControl_Server(netID,TRUE);
    CommSendFixSizeData_ServerSide(CF_OTHER_TALK_SERVER_SIDE,&id);
}

//--------------------------------------------------------------
/**
 * @brief   �e�@����NPC�ɘb���������q�@�������ۂɔ��ł��܂� CF_OTHER_TALK_SERVER_SIDE
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------
void CommPlayerRecvOtherTalk_ServerSide(int netID, int size, void* pData, void* pWork)
{
    u8* pBuff = pData;

    UgMgrSetTalkOtherLog(pBuff[0]);  //LOG
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������X�̈ʒu��Ԃ�  �������@���p
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosDigFossilX(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();

    if(!pCP){
        return COMM_PLAYER_INVALID_GRID;
    }
    else if(CommGetCurrentID() == netID){
        return pCP->sPlayerPlace[netID].xpos;
    }
    else if(!CommPlayerIsControl()){
        return COMM_PLAYER_INVALID_GRID;
    }
    return pCP->sPlayerPlace[netID].xpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������Z�̈ʒu��Ԃ�  �������@���p
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosDigFossilZ(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();

    if(!pCP){
        return COMM_PLAYER_INVALID_GRID;
    }
    else if(CommGetCurrentID() == netID){
        return pCP->sPlayerPlace[netID].zpos;
    }
    else if(!CommPlayerIsControl()){
        return COMM_PLAYER_INVALID_GRID;
    }
    return pCP->sPlayerPlace[netID].zpos;
}

//--------------------------------------------------------------
/**
 * @brief   㩂ɂ����������̃v���[���[�̏�Ԃ��Z�b�g������
 * @param   victimNetID  㩂ɂ��������l
 * @param   trapType     㩃^�C�v
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetCondition(int victimNetID, int condition)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
//    OHNO_PRINT("set�R���f�B�V���� %d \n", victimNetID);
    CommPlayerCondSetCondition(
        CommPlayerCondGetWork(pCP->pCondMgr, victimNetID),
        condition);
}

//--------------------------------------------------------------
/**
 * @brief   㩂ɂ����������̃v���[���[�̏�Ԃ�����
 * @param   victimNetID  㩂ɂ��������l
 * @param   trapType     㩃^�C�v
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerResetCondition(int victimNetID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
//    OHNO_PRINT("���Z�b�g�R���f�B�V���� %d \n", victimNetID);
    CommPlayerCondSetCondition(
        CommPlayerCondGetWork(pCP->pCondMgr, victimNetID),
        PLAYER_COND_NONE);
}

//--------------------------------------------------------------
/**
 * @brief   �ꏊ�ړ�����
 * @param   netID
 * @param   x
 * @param   y
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerBaseTeleportServer(int netID, int x, int z, int dir)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _PlayerPlace* pPP = &pCP->sPlayerPlaceServer[netID];
    pPP->xpos = x + FieldOBJ_DirAddValueGX(dir);
    pPP->zpos = z + FieldOBJ_DirAddValueGZ(dir);
    pPP->dir = dir;
 //   _pCommPlayerWork->bMoveChange[netID]=TRUE;  // ����閧��n�ɓ��鎞�ɏo�������̂� ���̏u�Ԃ�client�ɒʒm���Ȃ� 2006.05.16
}

//--------------------------------------------------------------
/**
 * @brief   �ꏊ�ړ�����
 * @param   netID
 * @param   x
 * @param   y
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerBaseTeleportClient(int netID, int x, int z, int dir)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    _PlayerPlace* pPP;
    
    if(!pCP){
        return;
    }
    pPP = &pCP->sPlayerPlace[netID];
    if(!pCP->bResetting){
        FE_FldOBJTrapsmogSet(pCP->pFSys, pPP->xpos, 0, pPP->zpos);
    }
    pPP->xpos = x;
    pPP->zpos = z;
    pPP->dir = dir;
    if(pCP->pPlayer[netID]){
        Player_GPosInit( pCP->pPlayer[netID],
                         x, z, dir );
    }
    if(!pCP->bResetting){
        FE_FldOBJTrapsmogSet(pCP->pFSys, pPP->xpos, 0, pPP->zpos);
    }
#if T1648_060818_FIX
    if(!pCP->bResetting){
        CommPlayerSetFENoneAdd(netID);
    }
#endif
}

//--------------------------------------------------------------
/**
 * ��������
 * @param	netID   playerID
 * @retval	��������TRUE
 */
//--------------------------------------------------------------
BOOL CommPlayerDeleteFlagData(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP->pNowFlag[netID] != NULL){
        sys_FreeMemoryEz(pCP->pNowFlag[netID]);
        pCP->pNowFlag[netID] = NULL;
        pCP->hedFE[netID] = _HFE_NONE;
        pCP->sendFlagData[netID].netID = INVALID_NETID;
        return TRUE;
    }
    return FALSE;
}

//--------------------------------------------------------------
/**
 * ���������Ă��邩�ǂ���
 * @param	netID   playerID
 * @retval	�����Ă�����TRUE
 */
//--------------------------------------------------------------
BOOL CommPlayerIsFlagData(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP->pNowFlag[netID] != NULL){
        return TRUE;
    }
    return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   ����̃p�\�R������������
 * @param   myNetID   �����Ƃ�l
 * @param   targetID  �����Ƃ���l
 * @retval  none
 */
//--------------------------------------------------------------

BOOL CommPlayerGetFlag(int myNetID, int targetNetID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP->pNowFlag[myNetID] == NULL){
        MYSTATUS* pTargetStatus = CommInfoGetMyStatus(targetNetID);
        if(pTargetStatus){
            pCP->pNowFlag[myNetID] = MyStatus_AllocWork(HEAPID_COMMUNICATION);
            MyStatus_Copy(pTargetStatus, pCP->pNowFlag[myNetID]);
            pCP->hedFE[myNetID] = _HFE_FLAG;
//            OHNO_PRINT("�͂�tor %d\n",myNetID);
            pCP->sendFlagData[myNetID].netID = myNetID;
            MyStatus_Copy(pTargetStatus, (MYSTATUS*)&pCP->sendFlagData[myNetID].mystatus);
            if(targetNetID == CommGetCurrentID()){
                SB_RECORD_DATA* pSBD = SaveData_GetSecretBaseRecord(pCP->pFSys->savedata);
                SecretBaseRecordSetFlagStealNum(pSBD);  // �Ƃ�ꂽ
            }
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================
/**
 * �т�����}�[�N�������֐�
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerMarkDelete(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP->pMark[netID] != NULL){
        if(EOA_StatusBit_UseCheck( pCP->pMark[netID] )){
            EOA_Delete(pCP->pMark[netID]);
        }
        else{
            GF_ASSERT(0);
        }
        pCP->pMark[netID] = NULL;
    }
}

//--------------------------------------------------------------
/**
 * @brief   �w��̐l�Ɋ����o��
 * @param   netID        �������l
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetFlagDisp(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(!pCP->bResetting){
        if(pCP->pPlayer[netID]){
            switch(pCP->hedFE[netID]){
              case _HFE_OK:
                if(pCP->pMark[netID]==NULL){
                    pCP->pMark[netID] = FE_Ok_Add( Player_FieldOBJGet(pCP->pPlayer[netID]) );
                }
                pCP->hedFE[netID] = _HFE_NONE;
                break;
              case _HFE_EXCLAMATION:
                if(pCP->pMark[netID]==NULL){
                    pCP->pMark[netID] =
                        FE_Exclamation_Add( Player_FieldOBJGet(pCP->pPlayer[netID]) );
                }
                break;
              case _HFE_FLAG:
                if(pCP->pMark[netID]==NULL){
                    pCP->pMark[netID] =
                        FE_Flag_Add( Player_FieldOBJGet(pCP->pPlayer[netID]) );
                }
                break;
              case _HFE_NONE:
                CommPlayerMarkDelete(netID);
                break;
            }
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   �w��̐l�ɂт�����}�[�N���o��
 * @param   netID        �т�����}�[�N�����l
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetFEExclamationAdd(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
//    OHNO_PRINT("�т�����}�[�N���o��\n");
    if(pCP->hedFE[ netID ] != _HFE_FLAG){
        pCP->hedFE[ netID ] = _HFE_EXCLAMATION;
    }
}

//--------------------------------------------------------------
/**
 * @brief   �w��̐l��OK�}�[�N���o��
 * @param   netID        �т�����}�[�N�����l
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetFEOkAdd(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP->hedFE[ netID ] != _HFE_FLAG){
        pCP->hedFE[ netID ] = _HFE_OK;
    }
    CommPlayerMarkDelete(netID);
}

//--------------------------------------------------------------
/**
 * @brief   �w��̐l�Ƀ}�[�N������
 * @param   netID        �����l
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetFENoneAdd(int netID)
{
    CommPlayerWork* pCP = CommPlayerGetInstance();
    if(pCP->hedFE[ netID ] != _HFE_FLAG){
        pCP->hedFE[ netID ] = _HFE_NONE;
    }
    CommPlayerMarkDelete(netID);
}

