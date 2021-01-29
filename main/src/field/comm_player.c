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
#include "comm_field_state.h"
#include "underground/ug_manager.h"

#include "system/bmp_menu.h"
#include "worldmap.h"
#include "fieldobj.h"
#include "fieldobj_code.h" //AC_WALK...�� �A�j���[�V�����R�[�h���܂Ƃ߂Ă���܂��B
#include "fieldsys.h"
#include "fieldmap.h"
#include "fld_bmp.h"
#include "system/pm_str.h"
#include "system/fontproc.h"
#include "system/bmp_list.h"
#include "system/window.h"

#include "player.h"
#include "comm_player_condition.h"
#include "comm_direct_counter.h"
#include "system/snd_tool.h"
#include "system/wordset.h"
#include "msgdata/msg_underworld.h"
#include "msgdata/msg_uwflag.h"

#include "field_effect.h"
#include "syswork.h"

#include "talk_msg.h"
#include "fldeff_flag.h"
#include "system/clact_tool.h"
#include "field_clact.h"
#include "fld_comact.h"
#include "underground/ug_local.h"
#include "eventdata.h"
#include "tv_topic.h"
#include "sysflag.h"

#include "d_ohno.h"  // �f�o�b�O�p

#include <nnsys/g3d/glbstate.h>


//==============================================================================
// ��`
//==============================================================================
#include "comm_player_local.h"

// �V���O���g��
static CommPlayerWork* _pCommPlayerWork = NULL;


// ug_player.c�݂̂ŗ��p
CommPlayerWork* CommPlayerGetInstance(void)
{
    return _pCommPlayerWork;
}


//==============================================================================
// static�錾
//==============================================================================

static int _getBlowMoveCount( int counter,int bBigBlow );
static BOOL _blowMove( int netID, int cnt );
static BOOL _blowAnim( int netID,int padTrg,int padCont,int speed );
static void _commPlayerDataSendTask(void *work);
static void _commPlayerDataSendTaskToChild(void *work);
static void _commPlayerAdd(u8 netID);
static void _commPlayerMove(TCB_PTR tcb, void *work);
static void _commPlayerTask(TCB_PTR tcb, void *work);
static void _commPlayerAutoStep(void *work);
static void _talkWindowFunc(TCB_PTR tcb, void* work);
static void _playerMoveClient(int netID);
static void CommPlayerCheckBattleJump_Server(void);

#ifdef PM_DEBUG
static void _debugGyoeDisp(void);
#endif

//==============================================================================
// �֐�
//==============================================================================

//==============================================================================
/**
 * �l�b�g�v���[���[�Ǘ��}�l�[�W���[�̏�����
 *   �l�b�g�v���[���[�̎q�@ID�ƃO���t�B�b�N�����т��ĊǗ����Ă���
 * @param   work_area  ���[�N�G���A
 * @retval  BOOL TRUE ����   FALSE ���s ���[�N�G���A�������Ă�������
 */
//==============================================================================

BOOL CommPlayerManagerInitialize(void* work_area, FIELDSYS_WORK* pFSys, BOOL bUnder)
{
    int i,j;

    if(_pCommPlayerWork){  // ���̂Ƃ��뉽�x�������������̂�
        return FALSE;
    }
    if(_pCommPlayerWork == NULL){
        _pCommPlayerWork = work_area;
        MI_CpuFill8(_pCommPlayerWork,0,sizeof(CommPlayerWork));
    }
    _pCommPlayerWork->bUnder = bUnder;
    _pCommPlayerWork->pCondMgr = NULL;
    if(bUnder){
        _pCommPlayerWork->pCondMgr = sys_AllocMemory(HEAPID_COMMUNICATION,
                                                     CommPlayCondGetWorkSize());
        CommPlayerCondInitialize(_pCommPlayerWork->pCondMgr);
    }
    _pCommPlayerWork->pFSys = pFSys;
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        _pCommPlayerWork->sPlayerPlace[i].dir = DIR_NOT;
        _pCommPlayerWork->sPlayerPlace[i].xpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlace[i].zpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlace[i].speed = _SPEED_8;
        _pCommPlayerWork->sPlayerPlaceServer[i].dir = DIR_NOT;
        _pCommPlayerWork->sPlayerPlaceServer[i].xpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlaceServer[i].zpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlaceServer[i].speed = _SPEED_8;
        _pCommPlayerWork->blowDir[i] = DIR_NOT;
        _pCommPlayerWork->bMoveControl[i] = FALSE;
        _pCommPlayerWork->bMoveControlFlag[i] = TRUE;
        _pCommPlayerWork->bMoveControlVS[ i ] = FALSE;

        _pCommPlayerWork->sendFlagData[i].netID = INVALID_NETID;
 //       _pCommPlayerWork->sendFlagDataID[i] = i;
    }
    _pCommPlayerWork->bMyMoveControl = FALSE;//TRUE;
    _pCommPlayerWork->bFirstMoveEnable = FALSE;

    _pCommPlayerWork->pTask = TCB_Add(_commPlayerTask, NULL, _TASK_PRI_NET_PLAYER_CREATE);
    CommEnableSendMoveData();

    CommPlayerMyDataInitialize();



    return TRUE;
}

//==============================================================================
/**
 * �l�b�g�v���[���[�Ǘ��}�l�[�W���[�̒��f����
 *   �\�����Ă����l�b�g�v���[���[������
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerManagerReset(void)
{
    int i;
    if(_pCommPlayerWork==NULL){
        return;
    }
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        if(_pCommPlayerWork->bUnder){
            CommPlayerDestroy( i, TRUE ,FALSE);
//            OHNO_PRINT("CommPlayerDestroy--2 %d\n", i);
        }
        else{
            CommPlayerDestroy( i, TRUE ,TRUE);
        }
    }
    if(_pCommPlayerWork->bUnder){
        _pCommPlayerWork->bResetting = TRUE;
    }
//    OHNO_PRINT("CommPlayerManagerReset\n");
}

//==============================================================================
/**
 * �l�b�g�v���[���[�Ǘ��}�l�[�W���[�̕��A����
 *   �����Ă����l�b�g�v���[���[���ĕ\��
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerManagerReboot(void)
{
    int x,z;
    
    if(_pCommPlayerWork==NULL){
        return;
    }
    _pCommPlayerWork->bPosAllSend = TRUE;
    _pCommPlayerWork->bResetting = FALSE;
    CommPlayerMyDataInitialize();
    CommPlayerSendPos(TRUE);

//    OHNO_PRINT("�����̈ʒuC  %d %d \n",CommPlayerGetPosX(CommGetCurrentID()),CommPlayerGetPosZ(CommGetCurrentID()));
//    x = Player_NowGPosXGet( _pCommPlayerWork->pFSys->player );
//    z = Player_NowGPosZGet( _pCommPlayerWork->pFSys->player );
//    OHNO_PRINT("�����̈ʒuF  %d %d \n",x,z);
}

//==============================================================================
/**
 * �l�b�g�v���[���[�Ǘ��}�l�[�W���[�̏I������
 *   �\�����Ă����l�b�g�v���[���[������
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerManagerFinalize(BOOL bDel)
{
    int i,j;
    if(_pCommPlayerWork){

        for(j = 0; j < _KEEP_FLAG_MAX; j++){
            if(_pCommPlayerWork->pKeepFlag[j]){
                sys_FreeMemoryEz(_pCommPlayerWork->pKeepFlag[j]);
                _pCommPlayerWork->pKeepFlag[j] = NULL;
            }
        }

        if(_pCommPlayerWork->myDeliveryFlagNum >= _FLAG_TVTOPIC_NUM){
            TVTOPIC_Entry_Record_GetFlag(_pCommPlayerWork->pFSys,_pCommPlayerWork->myDeliveryFlagNum);
        }
        for(i = 0; i < COMM_MACHINE_MAX; i++){
            CommPlayerDestroy( i, FALSE ,bDel);
//            OHNO_PRINT("CommPlayerDestroy--3 %d\n", i);
        }
        TCB_Delete(_pCommPlayerWork->pTask);
        if(_pCommPlayerWork->pCondMgr){
            CommPlayerCondFinalize(_pCommPlayerWork->pCondMgr);
            sys_FreeMemoryEz(_pCommPlayerWork->pCondMgr);
        }
        sys_FreeMemoryEz(_pCommPlayerWork);
        _pCommPlayerWork = NULL;
    }
}

//==============================================================================
/**
 * �l�b�g�v���[���[�Ǘ��}�l�[�W���[�̍ď�����  COMM_PARENT_ID�ɖ߂�
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerManagerReInit(void)
{
    int i,j;
    
    for(i = 0; i < COMM_MACHINE_MAX; i++){

        if(_pCommPlayerWork->bUnder){
            CommPlayerMarkDelete(i);
            CommPlayerResetCondition(i);
            if(i != COMM_PARENT_ID){
                CommPlayerDeleteFlagData(i);
            }
        }

        if(_pCommPlayerWork->pPlayer[i] != NULL){
            if(_pCommPlayerWork->pFSys->player != _pCommPlayerWork->pPlayer[i]){
                if(_pCommPlayerWork->bUnder){
                    Player_DeleteAll(_pCommPlayerWork->pPlayer[i]);
                }
            }
            _pCommPlayerWork->pPlayer[i] = NULL;
        }

        _pCommPlayerWork->bActive[i] = FALSE;
        _pCommPlayerWork->sPlayerPlace[i].dir = DIR_NOT;
        _pCommPlayerWork->sPlayerPlace[i].xpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlace[i].zpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlace[i].speed = _SPEED_8;
        _pCommPlayerWork->sPlayerPlaceServer[i].dir = DIR_NOT;
        _pCommPlayerWork->sPlayerPlaceServer[i].xpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlaceServer[i].zpos = COMM_PLAYER_INVALID_GRID;
        _pCommPlayerWork->sPlayerPlaceServer[i].speed = _SPEED_8;
        _pCommPlayerWork->blowDir[i] = DIR_NOT;
        _pCommPlayerWork->blowCounterS[i] = 0;
        _pCommPlayerWork->bMoveControl[i] = FALSE;
        _pCommPlayerWork->bMoveControlFlag[i] = TRUE;
        _pCommPlayerWork->bMoveControlVS[ i ] = FALSE;
        _pCommPlayerWork->walkCount[i] = 0;
        _pCommPlayerWork->walkCountClient[i] = 0;
    }
    _pCommPlayerWork->bFirstMoveEnable = FALSE;
    CommPlayerMyDataInitialize();
    if(_pCommPlayerWork->pTask == NULL){
        _pCommPlayerWork->pTask = TCB_Add(_commPlayerTask, NULL, _TASK_PRI_NET_PLAYER_CREATE);
    }
}

//==============================================================================
/**
 * �^�X�N���ً}��~
 * @param   none
 * @retval  none
 */
//==============================================================================

void CommPlayerManagerStop(void)
{
    if(_pCommPlayerWork->pTask){
        _commPlayerTask(NULL,NULL);
        TCB_Delete(_pCommPlayerWork->pTask);
    }
    _pCommPlayerWork->pTask = NULL;
}

//==============================================================================
/**
 * ���̃N���X�����������ꂽ���ǂ�����Ԃ�
 * @retval  ����������Ă����TRUE
 */
//==============================================================================

BOOL CommPlayerManagerIsInitialize(void)
{
    return (_pCommPlayerWork!=NULL);
}

//==============================================================================
/**
 * �����̃f�[�^��������
 * @param   none
 * @retval  none
 */
//==============================================================================
void CommPlayerMyDataInitialize(void)
{
    _pCommPlayerWork->pPlayer[CommGetCurrentID()] = _pCommPlayerWork->pFSys->player;
    _pCommPlayerWork->bActive[CommGetCurrentID()] = TRUE;
    _pCommPlayerWork->sPlayerPlace[CommGetCurrentID()].xpos = Player_NowGPosXGet( _pCommPlayerWork->pFSys->player );
    _pCommPlayerWork->sPlayerPlace[CommGetCurrentID()].zpos = Player_NowGPosZGet( _pCommPlayerWork->pFSys->player );
    _pCommPlayerWork->sPlayerPlace[CommGetCurrentID()].dir = Player_DirGet( _pCommPlayerWork->pFSys->player );
    _pCommPlayerWork->sPlayerPlaceServer[CommGetCurrentID()].xpos = Player_NowGPosXGet( _pCommPlayerWork->pFSys->player );
    _pCommPlayerWork->sPlayerPlaceServer[CommGetCurrentID()].zpos = Player_NowGPosZGet( _pCommPlayerWork->pFSys->player );
    _pCommPlayerWork->sPlayerPlaceServer[CommGetCurrentID()].dir = Player_DirGet( _pCommPlayerWork->pFSys->player );

//    OHNO_PRINT("----------CommGetCurrentID() == %d  %d %d %d \n",
//               CommGetCurrentID(),Player_DirGet( _pCommPlayerWork->pFSys->player ),
//               _pCommPlayerWork->sPlayerPlace[CommGetCurrentID()].xpos,
//               _pCommPlayerWork->sPlayerPlace[CommGetCurrentID()].zpos);

    
}

//==============================================================================
/**
 * �����̃f�[�^���R�s�[
 * @param   none
 * @retval  none
 */
//==============================================================================
void CommPlayerMyDataCopy(int netID)
{
    int id;

    _pCommPlayerWork->bFirstMoveEnable = FALSE;
    id = CommGetCurrentID();

    _pCommPlayerWork->pPlayer[id] = _pCommPlayerWork->pFSys->player;
    _pCommPlayerWork->bActive[id] = TRUE;
    _pCommPlayerWork->sPlayerPlace[id].xpos = _pCommPlayerWork->sPlayerPlace[netID].xpos;
    _pCommPlayerWork->sPlayerPlace[id].zpos = _pCommPlayerWork->sPlayerPlace[netID].zpos;
    _pCommPlayerWork->sPlayerPlace[id].dir = _pCommPlayerWork->sPlayerPlace[netID].dir;
    _pCommPlayerWork->sPlayerPlaceServer[id].xpos = _pCommPlayerWork->sPlayerPlaceServer[netID].xpos;
    _pCommPlayerWork->sPlayerPlaceServer[id].zpos = _pCommPlayerWork->sPlayerPlaceServer[netID].zpos;
    _pCommPlayerWork->sPlayerPlaceServer[id].dir = _pCommPlayerWork->sPlayerPlaceServer[netID].dir;

    _pCommPlayerWork->pNowFlag[id] = _pCommPlayerWork->pNowFlag[netID];
    _pCommPlayerWork->pNowFlag[netID] = NULL;

    if(_pCommPlayerWork->pNowFlag[id]){
        MyStatus_Copy(_pCommPlayerWork->pNowFlag[id],
                      (MYSTATUS*)&_pCommPlayerWork->sendFlagData[id].mystatus);
    }
    _pCommPlayerWork->sendFlagData[netID].netID = INVALID_NETID;
    
//    OHNO_PRINT("----------copy() == %d  %d %d \n",
//               CommGetCurrentID(),
//               _pCommPlayerWork->sPlayerPlace[id].xpos,
//               _pCommPlayerWork->sPlayerPlace[id].zpos);

    
}

//==============================================================================
/**
 * ���݂̈ʒu�𑗐M  �q�@���e�@�Ɏ����̈ʒu��`����
 * @param   none
 * @retval  none
 */
//==============================================================================

#define _BIT_LIVE (0x80)

void CommPlayerSendPosXZ(BOOL bLiveMessage,int xpos,int zpos)
{
    u8 data[_PLACEDATA_SEND_SIZE+1];
    int dir = Player_DirGet( _pCommPlayerWork->pFSys->player );

    data[0] = xpos;
    data[1] = xpos >> 8;
    data[2] = zpos;
    data[3] = zpos >> 8;
    data[4] = dir;

    if(bLiveMessage){
        data[4] = data[4] | _BIT_LIVE;
    }
    
    CommSendFixSizeData(CF_PLAYER_POS, data);
    _pCommPlayerWork->bPosAllSend = TRUE;
 //   _pCommPlayerWork->bMyMoveControl = FALSE;
  //  CommPlayerSetMoveControl(TRUE);
//    _pCommPlayerWork->bMyMoveControl = TRUE;
//    CommSendFixSizeData(CF_MOVE_CONTROL,&_pCommPlayerWork->bMyMoveControl);   // ���M����
   OHNO_SP_PRINT("�q�@���ړ��f�[�^���M %d %d %d\n",CommGetCurrentID(), xpos, zpos);
}

//==============================================================================
/**
 * ���݂̈ʒu�𑗐M  �q�@���e�@�Ɏ����̈ʒu��`����
 * @param   none
 * @retval  none
 */
//==============================================================================

#define _BIT_LIVE (0x80)

void CommPlayerSendPos(BOOL bLiveMessage)
{
    int xpos = Player_NowGPosXGet( _pCommPlayerWork->pFSys->player );
    int zpos = Player_NowGPosZGet( _pCommPlayerWork->pFSys->player );
    CommPlayerSendPosXZ(bLiveMessage, xpos, zpos);
}

//==============================================================================
/**
 * ���݂̈ʒu�𑗐M  �q�@���e�@�Ɏ����̈ʒu��`����
 * @param   none
 * @retval  none
 */
//==============================================================================

//#define _BIT_LIVE (0x80)

void CommPlayerSendPosServer(BOOL bLiveMessage)
{
//    int xpos = Player_NowGPosXGet( _pCommPlayerWork->pFSys->player );
//    int zpos = Player_NowGPosZGet( _pCommPlayerWork->pFSys->player );
    int xpos = _pCommPlayerWork->sPlayerPlaceServer[CommGetCurrentID()].xpos;
    int zpos = _pCommPlayerWork->sPlayerPlaceServer[CommGetCurrentID()].zpos;
//    OHNO_PRINT("�������MS %d %d %d\n",xpos,zpos,bLiveMessage);
    CommPlayerSendPosXZ(bLiveMessage, xpos, zpos);
}

//==============================================================================
/**
 * ���݂̈ʒu�@�w�߂𑗐M
 * @param   netID  �L������ID
 * @param   xpos  �O���[�o��X���W
 * @param   zpos  �O���[�o��Y���W
 * @param   dir   ����
 * @param   dash  �_�b�V����1
 * @retval  none
 */
//==============================================================================
static void _sendIDPlayerPos(int netID,const _PlayerPlace* pPP)
{
    u8 data[_IDPLACEDATA_SEND_SIZE+1];
    int x = pPP->xpos,z = pPP->zpos;

    if(pPP->xpos < 0){   // �͈͊O�̃f�[�^�͕␳����
        x = 0;
    }
    else if(pPP->xpos >= 0xf000){
        x = 0xf000-1;
    }
    if(pPP->zpos < 0){
        z = 0;
    }
    else if(pPP->zpos >= 0xf000){
        z = 0xf000-1;
    }

    data[0] = netID & 0xf;

    data[1] = x;
    data[2] = ((x >> 8)&0x1) + ((z >> 7)&0x02);
    data[3] = z;

    if(pPP->bHit){
        data[2] |= 0x80;
    }
    
    data[0] += ((pPP->dir % DIR_4_MAX) << 4);
    data[0] += (pPP->speed << 6);

    OHNO_PRINT("SP POS���M %d  %d %d\n",netID, x, z);
    CommSendData_ServerSide(CF_PLAYER_POS_ID, data, 0);
}

//==============================================================================
/**
 * �����̃v���[���[�̈ʒu�������I�ɑ��M���邽�߂̃^�X�N
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		none
 * @retval  none
 */
//==============================================================================
void _commPlayerDataSendTask(void *work)
{
    // add�̎������g�p����Ă��Ȃ��̂ŁA�ʒu�␳�͂������Ă��Ȃ� 05.09.01
    // �ʒu���𑗐M
    int mv = Player_MoveValueGet( _pCommPlayerWork->pFSys->player );
    int ms = Player_MoveStateGet( _pCommPlayerWork->pFSys->player );
    
    
    if( (OBJ_MOVE_VALUE_STOP == mv) && CommIsConnect(CommGetCurrentID())){
        if(!_pCommPlayerWork->moveSync){
            CommPlayerSendPos(TRUE);
            _pCommPlayerWork->moveSync = TRUE;
        }
    }
    if((OBJ_MOVE_VALUE_WALK == mv )&&(OBJ_MOVE_STATE_START == ms)){
        // DS�����̏ꍇ�������͂���
        _pCommPlayerWork->moveSync = FALSE;
    }
}

//==============================================================================
/**
 * �����̃v���[���[�̈ʒu�����q�@�ɏ�ɑ��M���邽�߂̃^�X�N
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		none
 * @retval  none
 */
//==============================================================================
static void _commPlayerDataSendTaskToChild(void *work)
{
    int i;  // �ʒu���𑗐M

    for(i = 0; i < COMM_MACHINE_MAX; i++){
        if(_pCommPlayerWork->bActive[i]){
            _PlayerPlace* pPP = &_pCommPlayerWork->sPlayerPlaceServer[i];

            if(_pCommPlayerWork->bMoveChange[i] || _pCommPlayerWork->bPosAllSend){
                _pCommPlayerWork->bMoveChange[i]= FALSE;
                _sendIDPlayerPos( i, pPP);
            }
        }
    }
    _pCommPlayerWork->bPosAllSend = FALSE; // �t���O�𗎂Ƃ��Ă���
}

//==============================================================================
/**
 * �l�b�g�v���[���[�Ǘ��}�l�[�W���[�̃��[�N�T�C�Y�擾
 * @param   none
 * @retval  u32   ���[�N�T�C�Y
 */
//==============================================================================

u32 CommPlayerGetWorkSize(void)
{
    return sizeof(CommPlayerWork);
}

//==============================================================================
/**
 * �l�b�g�v���[���[��ǉ�
 * @param   netID  �q�@�e�@��ID
 * @retval  none
 */
//==============================================================================

static void _commPlayerAdd(u8 netID)
{
    fx32 xpos, zpos;
    PLAYER_STATE_PTR pPlayer;

    if(_pCommPlayerWork->pPlayer[netID]!=NULL){
        return;  //���łɂ���ꍇ
    }
    if(_pCommPlayerWork->bResetting){
//        OHNO_PRINT("reset�� %d  \n", netID);
        return;  // 
    }
    {
        MYSTATUS* pMy = CommInfoGetMyStatus(netID);
        if(pMy==NULL){
//            OHNO_PRINT("mystatus���Ȃ����� %d\n",netID);
        }
        if(pMy){
            if(!_pCommPlayerWork->bUnder){
                if(netID != CommGetCurrentID()){
                    // �����O�̂��̂�������
                    FIELD_OBJ_PTR pObj = FieldOBJSys_OBJIDSearch(_pCommPlayerWork->pFSys->fldobjsys,FLDOBJ_ID_PLAYER+netID+1);
                    if(pObj){
                        FieldOBJ_Delete(pObj);
                    }
                }
            }
            {
                int ver = HEROVER_PL;
                if(0 == MyStatus_GetRomCode(pMy)){
                    ver = HEROVER_DP;
                }
                pPlayer = Player_Init( _pCommPlayerWork->pFSys->fldobjsys,
                                       _pCommPlayerWork->sPlayerPlace[netID].xpos,
                                       _pCommPlayerWork->sPlayerPlace[netID].zpos,
                                       _pCommPlayerWork->sPlayerPlace[netID].dir,
                                       HERO_FORM_NORMAL, MyStatus_GetMySex(pMy), ver, NULL );
            }
            GF_ASSERT_RETURN(pPlayer && "NetPlayer�����ł��Ȃ�����",);
            _pCommPlayerWork->pPlayer[netID] = pPlayer;

                                                   //����ID
            FieldOBJ_OBJIDSet(Player_FieldOBJGet(pPlayer),FLDOBJ_ID_PLAYER+netID+1);
            if(_pCommPlayerWork->bUnder){
                UgMgrSetReturnLog(netID);  //LOG
            }
            if(_pCommPlayerWork->bUnder && !_pCommPlayerWork->bActive[netID]){
                if(!_pCommPlayerWork->bResetting){
                    FE_FldOBJTrapsmogSet(_pCommPlayerWork->pFSys,
                                         _pCommPlayerWork->sPlayerPlace[netID].xpos,
                                         0,
                                         _pCommPlayerWork->sPlayerPlace[netID].zpos);
                }
                _pCommPlayerWork->bActive[netID] = TRUE;
            }
            else if(!_pCommPlayerWork->bUnder){
                _pCommPlayerWork->bActive[netID] = TRUE;
            }
        }
    }
	
    // �ʒu���𑗂�Ԃ�
//    _pCommPlayerWork->moveSync = FALSE;
 /*   OHNO_PRINT("�l�b�g�L������o�^ %d (%d,%d)\n", netID,
               _pCommPlayerWork->sPlayerPlace[netID].xpos,
               _pCommPlayerWork->sPlayerPlace[netID].zpos);
*/
}

//==============================================================================
/**
 * �l�b�g�v���[���[���폜
 * @param   netID  �q�@�e�@��ID
 * @retval  none
 */
//==============================================================================

void CommPlayerDestroy(u8 netID, BOOL bExit,BOOL bDel)
{
    int i;

    if(_pCommPlayerWork == NULL){
        return;
    }
    MI_CpuClear8(_pCommPlayerWork->handCount,COMM_MACHINE_MAX);
    if(_pCommPlayerWork->bUnder){
        CommPlayerMarkDelete(netID);
    }
    if(_pCommPlayerWork->pPlayer[netID] != NULL){
        if(_pCommPlayerWork->pFSys->player != _pCommPlayerWork->pPlayer[netID]){
            if(_pCommPlayerWork->bUnder || bDel){
                Player_DeleteAll(_pCommPlayerWork->pPlayer[netID]);
            }
            else{
                Player_Delete(_pCommPlayerWork->pPlayer[netID]);
            }
        }
        _pCommPlayerWork->pPlayer[netID] = NULL;
    }
    if((_pCommPlayerWork->bActive[netID]) && (!bExit)){
        _pCommPlayerWork->bActive[netID] = FALSE;
    }
    _pCommPlayerWork->bMoveChange[netID]=TRUE;

    if(!bExit){
        _pCommPlayerWork->hedFE[netID] = _HFE_NONE;
        if(_pCommPlayerWork->bUnder){
            CommPlayerDeleteFlagData(netID);
            if(netID != COMM_PARENT_ID){
//                UgSecretBaseDestroyDoor(netID);
//                UgSecretBaseTrapGoodsReset(netID);
            }
        }
    }
}


static void _speedSend()
{
    u8 speed = _SPEED_8;

    if(PAD_BUTTON_B & sys.cont){
        speed = _SPEED_4;
    }
//#ifdef DEBUG_ONLY_FOR_ohno  // �f�o�b�O�_�b�V���͂��낢��Ђ��������Ȃ̂ŃR�����g�ɂ��܂���
#ifdef PM_DEBUG   //�}���I�N���u����̗v�]�ŉ��ւ��܂����B�ςȏꏊ�����Ǝ~�܂�܂��̂Œ��ӁI
    else if(PAD_BUTTON_R & sys.cont){
        speed = _SPEED_2;
    }
#endif
    CommSetSpeed(speed);
}


//==============================================================================
/**
 * CommPlayer�̒ʏ폈��
 * @param   netID  �q�@�e�@��ID
 * @retval  none
 */
//==============================================================================

static void _commPlayerTask(TCB_PTR tcb, void *work)
{
    int i;

    if(CommIsInitialize()){
        _speedSend();
        if(CommGetCurrentID() == COMM_PARENT_ID){
            _commPlayerDataSendTaskToChild(work); // �q�@�Ɉʒu���M
            _commPlayerDataSendTask(work); // �b�������Ǘ�
            if(_pCommPlayerWork->pCondMgr){
                CommPlayerCondStep(_pCommPlayerWork->pCondMgr);  // �e�@�ŃR���f�B�V��������
            }
        }
        else{
            _commPlayerDataSendTask(work); // �b�������Ǘ�
        }
        _commPlayerAutoStep(work);  // �v���C���[��add delete�Ǘ�
    }

    for(i = 0; i < COMM_MACHINE_MAX; i++){
        if(_pCommPlayerWork->bUnder){
            if(CommGetCurrentID() == COMM_PARENT_ID){
                if(NULL==CommInfoGetMyStatus(i) ){
                    UgSecretBaseResetPlayer(i);
                }
            }
        }
    }
}


//==============================================================================
/**
 * �ʐM��Ԃ����āA�l�b�g�v���[���[��z�u������͂������肷��
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		none
 * @retval  none
 */
//==============================================================================

static void _commPlayerAutoStep(void *work)
{
    int i, dir;
    

    for(i = 0; i < COMM_MACHINE_MAX; i++){
		
/*
 *	�ʐM�o�O�@no.0052
�y�R���V�A����L�{�^���������ƂƂ܂�z

�u�R���V�A���v�ɓ���AL�{�^���������ƃr�[�v���Ƌ��ɂƂ܂�܂��B
#ifdef PM_DEBUG
        if(sys.trg & PAD_BUTTON_L){
            if(CommPlayerIsTrapBind(i)){
//                OHNO_PRINT("�R���f�B�V�����ُ� %d\n",i);
            }
        }
#endif
//*/
        if(!CommIsConnect(i)){
            if(!(CommGetAloneMode() && (i == COMM_PARENT_ID))){
                if((CommGetCurrentID() == COMM_PARENT_ID) && (_pCommPlayerWork->bUnder)){
                    // ��ɊĎ� �����l�̔閧��n�ɂ���l��ǂ��o��
                    UgSecretBaseRemovePlayer( i );
                }
            }
        }
        if(CommIsConnect(i)  || (CommGetAloneMode() && (i == COMM_PARENT_ID)) ){  // �I�����C���Ȃ�
            _playerMoveClient(i);
            if(_pCommPlayerWork->bUnder){
                CommPlayerSetFlagDisp(i);
            }
        }
        else{  // �I�t���C���Ȃ�
            if(_pCommPlayerWork->bActive[i]){
                if((CommGetCurrentID() == COMM_PARENT_ID) && (_pCommPlayerWork->bUnder)){
                    // �����l��㩏����Ȃ���
                    CommTrapDeletePlayer_Server(i);
                    UgDigFossilResetPlayer(i);
                }
                {
                    u8 creatureID = i;  // ����ɏ���
                    CommPlayerRecvDelete(0, 1, &creatureID, _pCommPlayerWork->pFSys);
                }
            }
        }
    }
}

//==============================================================================
/**
 * ���@�̈ړ��\��Ԃ��ǂ����𒲂ׂ�
 * @param   none
 * @retval  ���@���ړ��\��Ԃ̏ꍇTRUE
 */
//==============================================================================

BOOL CommPlayerIsMyMoveControl(void)
{
    if(_pCommPlayerWork != NULL){
        return _pCommPlayerWork->bMyMoveControl;
    }
    return FALSE;
}

//==============================================================================
/**
 * ���@�̈ړ��\��Ԃ��ǂ�����ݒ肷��
 *    ��Ԃ��L�����Ă����� �g���K�[��e�@�ɑ��M����
 * @param   bMoveControl  ���@���ړ��\��Ԃ��ǂ���
 * @retval  none
 */
//==============================================================================

void CommPlayerSetMoveControl(BOOL bMoveControl)
{
    if(_pCommPlayerWork != NULL){
        if(_pCommPlayerWork->bMyMoveControl != bMoveControl){
            _pCommPlayerWork->bMyMoveControl = bMoveControl;
        //    CommSendFixSizeData(CF_MOVE_CONTROL,&_pCommPlayerWork->bMyMoveControl);   // ���M����
        }
//        OHNO_PRINT("CF_MOVE_CONTROL \n");
        CommSendFixSizeData(CF_MOVE_CONTROL,&_pCommPlayerWork->bMyMoveControl);   // ���M����
    }
}

//==============================================================================
/**
 * ���@�̈ړ��\��Ԃ��ǂ�����ݒ肷��
 *    ��Ԃ��L�����Ă����� �g���K�[��e�@�ɑ��M����
 * @param   bMoveControl  ���@���ړ��\��Ԃ��ǂ���
 * @retval  none
 */
//==============================================================================

static void _setMoveControlVSRoom(BOOL bMoveControl)
{
    if(_pCommPlayerWork != NULL){
        if(_pCommPlayerWork->bMyMoveControl != bMoveControl){
            _pCommPlayerWork->bMyMoveControl = bMoveControl;
            CommSendFixSizeData(CF_MOVE_CONTROL,&_pCommPlayerWork->bMyMoveControl);   // ���M����
        }
    }
}

//==============================================================================
/**
 * �ړ��\��Ԃ��ǂ�������M����   CF_MOVE_CONTROL
 * @param   netID    ���M���Ă���ID
 * @param   size     �����Ă����f�[�^�T�C�Y
 * @param   pData    ���̃|�C���^
 * @retval  none
 */
//==============================================================================

void CommPlayerRecvMoveControl(int netID, int size, void* pData, void* pWork)
{
    u8* pBuff = (u8*)pData;

    if(_pCommPlayerWork){
        GF_ASSERT_RETURN(size == 1,);
        GF_ASSERT_RETURN(netID < COMM_MACHINE_MAX,);
        GF_ASSERT_RETURN((pBuff[0] == TRUE)||(pBuff[0] == FALSE),);
        CommPlayerSetMoveControl_Server(netID,pBuff[0]);
//        OHNO_PRINT("recvMoveControl %d \n",netID);
    }
}

//==============================================================================
/**
 * �S���ړ�����
 * @param   repw		  FIELDSYS_WORK�̃|�C���^
 * @retval  none
 */
//==============================================================================

void CommPlayersMove(FIELDSYS_WORK * repw, BOOL bMoveControl)
{

    if(_pCommPlayerWork==NULL){
        return;
    }
    if(!_pCommPlayerWork->bUnder){
        _setMoveControlVSRoom(bMoveControl);  // ���@�̈ړ���Ԃ�ݒ�
        _pCommPlayerWork->bMoveControlVSRoom = bMoveControl;
    }
    if(repw->player){
        _pCommPlayerWork->pPlayer[CommGetCurrentID()] = repw->player;
    }
    if(CommIsInitialize() && (CommGetCurrentID() == COMM_PARENT_ID)){
        // �e�@=SERVER�����ړ��������s���A�q�@�͂��̌��ʂœ���
        _commPlayerMove(NULL, _pCommPlayerWork);
    }
    if(CommIsInitialize() && (CommGetCurrentID() == COMM_PARENT_ID)){
        if(!_pCommPlayerWork->bUnder){
            CommPlayerCheckBattleJump_Server();
        }
    }
    
#ifdef PM_DEBUG
   // _debugGyoeDisp();
#endif
}

//--------------------------------------------------------------
/**
 * �L�[�g���K�A�v���X���牟����Ă���������擾
 * @param	key_trg 	�L�[�g���K
 * @param	key_prs		�L�[�v���X
 * @retval	int			������Ă�������BDIR_UP��
 */
//--------------------------------------------------------------
static int _keyDirGet( u16 key_trg, u16 key_cont )
{
	if( (key_cont & PAD_KEY_LEFT) ){
		return( DIR_LEFT );
	}
	
	if( (key_cont & PAD_KEY_RIGHT) ){
		return( DIR_RIGHT );
	}
	
	if( (key_cont & PAD_KEY_UP) ){
		return( DIR_UP );
	}
	
	if( (key_cont & PAD_KEY_DOWN) ){
		return( DIR_DOWN );
	}
	
	return( DIR_NOT );
}

//--------------------------------------------------------------
/**
 * ��������L�[��Ԃ�
 * @param	dir 	����
 * @retval	int		������Ă���L�[
 */
//--------------------------------------------------------------
static int _dirKeyGet( u16 dir )
{
    switch(dir){
      case DIR_LEFT:
        return PAD_KEY_LEFT;
      case DIR_RIGHT:
        return PAD_KEY_RIGHT;
      case DIR_UP:
        return PAD_KEY_UP;
      case DIR_DOWN:
        return PAD_KEY_DOWN;
	}
    return 0;
}

//==============================================================================
/**
 * NPC�����邩�ǂ����̌���
 * @param   limit   �e�[�u���̌��E
 * @param   pFunc   �e�[�u���ɃA�N�Z�X���邽�߂̊֐�
 * @retval  none
 */
//==============================================================================

BOOL CommPlayerNPCHitCheck(int x,int z)
{
    int num,i;
    const FIELD_OBJ_H* pFObj;

    num = EventData_GetNpcCount(_pCommPlayerWork->pFSys);
    pFObj = EventData_GetNpcData(_pCommPlayerWork->pFSys);
    for(i = 0; i < num; i++){
        if((pFObj[i].gx == x) && (pFObj[i].gz == z)){
            return TRUE;
        }
    }
    return FALSE;
}

//==============================================================================
/**
 * �n�`�ƃl�b�g�v���[���[�̂�����𔻒�
 * @param   tcb      �^�X�N�|�C���^
 * @param   work     ���[�N�\���̂̃|�C���^
 * @retval  HIT���݂���ꍇTRUE  ������ꍇ��FALSE
 */
//==============================================================================

static BOOL _commGetHitAttr(int x, int z, int netID)
{
    int i;

    if((x != COMM_PLAYER_INVALID_GRID) && (z != COMM_PLAYER_INVALID_GRID)){
        for(i = 0; i < COMM_MACHINE_MAX; i++){
            if(i == netID){
                continue;
            }
            if((x == CommPlayerGetPosSX(i)) && (z == CommPlayerGetPosSZ(i))){
//                OHNO_PRINT("�v���[���[���m�̂�����\n");
                return TRUE;
            }
        }
        if(CommPlayerNPCHitCheck(x,z)){
            return TRUE;
        }
        if(_pCommPlayerWork->bUnder){
            if(!UgSecretBaseMoveing(x, z)){
                return TRUE;
            }
        }
    }
    return GetHitAttr(_pCommPlayerWork->pFSys, x, z);
}

//--------------------------------------------------------------
/**
 * ������΂��̈ړ��J�E���g
 * @param	netID  id
 * @retval	�ړ��l
 */
//--------------------------------------------------------------

static int _getBlowMoveCount( int counter, int bBigBlow)
{
    int max;

    if(counter < 5){
        return 8;
    }
    else if(counter < 10){
        return 4;
    }
    //if(bBigBlow){
      //  max = _BIGBLOW_COUNTER_NUM_MAX;
    //}
    //else{
      //  max = _BLOW_COUNTER_NUM_MAX;
//    }
//    if(counter < (max - _BLOW_STOP_COUNT)){
        return 2;
  //  }
    //return 1;
}

//==============================================================================
/**
 * �X�s�[�h�ɂ��킹�Ĉړ�����t���[������Ԃ�
 * @param   speed  �ړ����x
 * @retval  �ړ�����t���[����
 */
//==============================================================================

static int _getWalkCount(int speed)
{
    int speedCount[_SPEED_MAX]={2,4,8,16,2};

    GF_ASSERT_RETURN(speed < _SPEED_MAX, 8);
    return speedCount[speed];
}

//==============================================================================
/**
 * ��M�����L�[�f�[�^�����Ɉړ�����
 * @param   tcb      �^�X�N�|�C���^
 * @param   work     ���[�N�\���̂̃|�C���^
 * @retval  none
 */
//==============================================================================

static void _commPlayerMove(TCB_PTR tcb, void *work)
{
    u16 cont;
    u8 speed;
    int dir, x, z, i, cnt;
    _PlayerPlace* pPP;
    PLAYER_CONDITION_PTR pCond=NULL;
    
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        if(_pCommPlayerWork->bActive[i] && _pCommPlayerWork->bMoveControl[i] &&
            _pCommPlayerWork->bMoveControlFlag[i] ){
            pPP = &_pCommPlayerWork->sPlayerPlaceServer[i];
            if(_pCommPlayerWork->pCondMgr){
                pCond = CommPlayerCondGetWork(_pCommPlayerWork->pCondMgr, i);
            }
            if(_pCommPlayerWork->bUnder){
                if(CommSecretBaseIsTraveling(i)){   // �ꏊ�ړ���
#ifdef DEBUG_ONLY_FOR_ohno
                    if(sys.cont & PAD_BUTTON_START){
//                        OHNO_PRINT("--�閧��n�ړ���\n");
                    }
#endif
                    continue;
                }
            }
            if(_pCommPlayerWork->walkCount[i] != 0){
                _pCommPlayerWork->walkCount[i]--;
            }

            if(_pCommPlayerWork->walkCount[i] == 0){
                if(_pCommPlayerWork->bUnder){
                    if(CommTrapCheckAndSendMessage( i )){ // 㩏���
                        continue;
                    }
                    if(!CommPlayerIsTrapBind(i)){
                        if(UgSecretBaseTrapGoodsCheck( i )){ // 㩏���
                            continue;
                        }
                    }
                }
            }

            if(pCond){
                if(!CommPlayerIsMoveCondition_ServerSide(pCond)){
                    continue;
                }
            }
            if(!_pCommPlayerWork->bMoveControlVS[i]){
                cont = CommGetPadCont(i);
            }
            else{
                cont = 0;
            }
            pPP->speed = CommGetSpeed(i);
            if(_pCommPlayerWork->walkCount[i]!=0){
                continue;
            }
            cnt = _getBlowMoveCount(_pCommPlayerWork->blowCounterS[i],_pCommPlayerWork->bBlowBigS[i] );
            if(_blowMove(i, cnt)){
                _pCommPlayerWork->walkCount[i] = cnt;
                _pCommPlayerWork->bMoveChange[i]=TRUE;
                continue;
            }
            if(pPP->bHit == TRUE){
                _pCommPlayerWork->bMoveChange[i] = TRUE;
            }
            pPP->bHit = FALSE;
//            if(_pCommPlayerWork->countHole[i] == 1){   //�����
//                u8 bRet = TRUE;
//                CommTrapRecvEnd(i, 1, &bRet,NULL);
//                _pCommPlayerWork->countHole[i] = 0;
//            }
            if(_pCommPlayerWork->countHole[i] == 2){
                u8 id = i;
                _pCommPlayerWork->countHole[i] = 1;
                CommSendFixSizeData_ServerSide(CF_TRAP_PRIEND,&id);
                continue;
            }
            dir = _keyDirGet(cont,cont);
            x = CommPlayerGetPosSXDirAdd(i);
            z = CommPlayerGetPosSZDirAdd(i);
            if((x!=COMM_PLAYER_INVALID_GRID) && (z !=COMM_PLAYER_INVALID_GRID) && (dir != DIR_NOT)){
                if(pPP->dir != dir){
                    // �ړ������ϊ��̂�
                    pPP->dir = dir;
                    _pCommPlayerWork->walkCount[i] = 4;
                    _pCommPlayerWork->bMoveChange[i]=TRUE;
                    if(_pCommPlayerWork->countHole[i] > 2){
                        _pCommPlayerWork->countHole[i]--;
                        continue;
                    }
                }
                else if(_pCommPlayerWork->countHole[i] != 0){
                    continue;
                }
                else if( _pCommPlayerWork->bUnder && (CommSecretBaseSingleStepFunc(i,x,z) == TRUE)){
                    continue;  // �閧��n�ړ��J�n
                }
#ifdef PM_DEBUG
                else if(_commGetHitAttr( x, z, i) && !(pPP->speed == _SPEED_2)){
#else
                else if(_commGetHitAttr( x, z, i)){
#endif
                    pPP->bHit = TRUE;
                    _pCommPlayerWork->bMoveChange[i] = TRUE;
                    _pCommPlayerWork->walkCount[i] = 4;
                }
                else{
                    _pCommPlayerWork->bMoveChange[i]=TRUE;
                    pPP->xpos = x;
                    pPP->zpos = z;
                    pPP->dir = dir;
                    OHNO_SP_PRINT("�T�[�o�[  MOVE  %d %d \n",x,z);
                    _pCommPlayerWork->walkCount[i] = _getWalkCount(pPP->speed);
                    if(_pCommPlayerWork->giddyCounter[i]!=0){
                        _pCommPlayerWork->giddyCounter[i]--;
                        if(_pCommPlayerWork->giddyCounter[i]==0){
                            u8 bRet = TRUE;
                            CommTrapRecvEnd(i, 1, &bRet,NULL);
                        }
                    }
                }
            }
        }
    }
}

//==============================================================================
/**
 * �ʒu�̏�����M����   CF_PLAYER_POS
 * @param   netID    ���M���Ă���ID
 * @param   size     �����Ă����f�[�^�T�C�Y
 * @param   pData    �ړ����|�C���^
 * @retval  none
 */
//==============================================================================

void CommPlayerRecvPlace(int netID, int size, void* pBuff, void* pWork)
{
    u8* pData = (u8*)pBuff;
    _PlayerPlace* pPP;

    if(_pCommPlayerWork==NULL){
        return;
    }
    pPP = &_pCommPlayerWork->sPlayerPlaceServer[netID];

    if(pData[4] & _BIT_LIVE){
       // if(_pCommPlayerWork->bActive[netID]){
            _pCommPlayerWork->bPosAllSend = TRUE;
//            _pCommPlayerWork->bMoveChange[netID] = TRUE;
            return;
     //   }
    }
    if(pPP->dir == DIR_NOT){  // �܂�������Ԃ̏ꍇ��
        // �ʒu�f�[�^���q�@�ɑ��M����
//        _pCommPlayerWork->bPosAllSend = TRUE;
        int i;
        for(i = 0; i < COMM_MACHINE_MAX; i++){
            _pCommPlayerWork->bMoveChange[i] = TRUE;
        }

    }
    pPP->xpos = 0;
    pPP->zpos = 0;
    pPP->xpos += ((u32)pData[0]) & 0xff;
    pPP->xpos += ((u32)pData[1] << 8) & 0xff00;
    pPP->zpos += ((u32)pData[2]) & 0xff;
    pPP->zpos += ((u32)pData[3] << 8) & 0xff00;
    pPP->dir = pData[4] & 0x0f;

    OHNO_SP_PRINT("�ړ����� %d %d %d %d\n", netID, pPP->xpos, pPP->zpos,pPP->dir);
    _pCommPlayerWork->bActive[netID] = TRUE;
    _pCommPlayerWork->bMoveChange[netID] = TRUE;


    if( GetHitAttr(_pCommPlayerWork->pFSys, pPP->xpos, pPP->zpos) ){ // �����Ă������W���s��
        GF_ASSERT_MSG(0,"pos %d %d\n",pPP->xpos, pPP->zpos);
    }
}


//==============================================================================
/**
 * �v���[���[���������߂���M����  CF_PLAYER_DELETE
 * @param   netID    ���M���Ă���ID
 * @param   size     �����Ă����f�[�^�T�C�Y
 * @param   pData    �ړ����|�C���^
 * @retval  none
 */
//==============================================================================

void CommPlayerRecvDelete(int netID, int size, void* pBuff, void* pWork)
{
    u8* pData = (u8*)pBuff;
    u8 creatureID = pData[0];
    
    if(_pCommPlayerWork == NULL){
        return;
    }
    if(creatureID == CommGetCurrentID()){
        return;
    }
    OHNO_SP_PRINT("�v���[���[���������߂���M %d \n",creatureID);

    if(_pCommPlayerWork->bUnder){
        UgMgrStartReturnLog(creatureID);
        if(!_pCommPlayerWork->bResetting){
            FE_FldOBJTrapsmogSet(_pCommPlayerWork->pFSys,
                                 _pCommPlayerWork->sPlayerPlace[creatureID].xpos,
                                 0,
                                 _pCommPlayerWork->sPlayerPlace[creatureID].zpos);

        }
    }
    CommPlayerDestroy(creatureID, FALSE, FALSE);
//    OHNO_PRINT("CommPlayerDestroy--1 %d\n", creatureID);
    CommInfoDeletePlayer(creatureID);
}

//==============================================================================
/**
 * �ꏊ�𑗂�f�[�^�̃T�C�Y��Ԃ��܂�
 * @param   none
 * @retval  �f�[�^�T�C�Y
 */
//==============================================================================

int CommPlayerGetRecvPlaceSize(void)
{
    return _PLACEDATA_SEND_SIZE;
}


//==============================================================================
/**
 * ID�w��ňʒu�̏�����M���� CF_PLAYER_POS_ID
 * @param   netID    ���M���Ă���ID
 * @param   size     �����Ă����f�[�^�T�C�Y
 * @param   pData    �ړ����|�C���^
 * @retval  none
 */
//==============================================================================

void CommPlayerRecvIDPlace(int netID, int size, void* pBuff, void* pWork)
{
    u16 pad;
    u8* pData = (u8*)pBuff;
    _PlayerPlace* pPP;
    int creatureID;

    creatureID = pData[0] & 0xf;
    if(_pCommPlayerWork == NULL){
        OHNO_SP_PRINT("----------�󂯎��Ȃ����� PlaceID %d\n", creatureID);
        return;
    }
    if(netID == CommGetCurrentID() && _pCommPlayerWork->bNotRecvMoveData){
        return;
    }
    pPP = &_pCommPlayerWork->sPlayerPlace[creatureID];

    if(pData[2] & 0x80){
        pPP->bHit = TRUE;
    }
    else{
        pPP->bHit = FALSE;
    }

    pPP->xpos = 0;
    pPP->zpos = 0;
    pPP->xpos += ((u32)pData[1]) & 0xff;
    pPP->xpos += ((u32)pData[2] << 8) & 0x0100;
    pPP->zpos += ((u32)pData[3]) & 0xff;
    pPP->zpos += ((u32)pData[2] << 7) & 0x0100;
    pPP->dir = ((pData[0] >> 4) & 0x3);
    pPP->speed = ((pData[0] >> 6) & 0x3);

    _commPlayerAdd(creatureID);
//    OHNO_PRINT("CF_PLAYER_POS_ID %d %d %d %d\n",creatureID,pPP->xpos,pPP->zpos,pPP->dir);
}

//--------------------------------------------------------------
/**
 * �������������J�E���g����
 * @param	dir		�ړ���������BDIR_UP��
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------

static void _walkCount(int netID)
{
    UNDERGROUNDDATA* pUGD = SaveData_GetUnderGroundData(GameSystem_GetSaveData(_pCommPlayerWork->pFSys));
    SB_RECORD_DATA* pSBD = SaveData_GetSecretBaseRecord(GameSystem_GetSaveData(_pCommPlayerWork->pFSys));

    if(_pCommPlayerWork->bUnder){
        if(netID == CommGetCurrentID()){
            UnderGroundCountWalk(pUGD);
            if(UnderGroundGetWalkNum(pUGD)==50){
                RECORD_Score_Add(SaveData_GetRecord(_pCommPlayerWork->pFSys->savedata),
                                 SCORE_ID_WLAK_100);
            }
        }
    }
}

//--------------------------------------------------------------
/**
 * ������΂���㩂𓥂񂾏ꍇ�̈ړ�
 * @param	dir		�ړ���������BDIR_UP��
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------
static BOOL _blowAnim( int netID,int padTrg,int padCont,int speed )
{
    FIELD_OBJ_PTR fldobj;
    int idx, dir;
    u8 walkcmd[]={
        AC_WALK_U_2F,				///<��Ɉړ��@2�t���[��
        AC_WALK_D_2F,				///<���Ɉړ��@2�t���[��
        AC_WALK_L_2F,				///<���Ɉړ��@2�t���[��
        AC_WALK_R_2F,				///<�E�Ɉړ��@2�t���[��
        AC_WALK_U_4F,				///<��Ɉړ��@4�t���[��
        AC_WALK_D_4F,				///<���Ɉړ��@4�t���[��
        AC_WALK_L_4F,				///<���Ɉړ��@4�t���[��
        AC_WALK_R_4F,				///<�E�Ɉړ��@4�t���[��
        AC_WALK_U_8F,				///<��Ɉړ��@8�t���[��
        AC_WALK_D_8F,				///<���Ɉړ��@8�t���[��
        AC_WALK_L_8F,				///<���Ɉړ��@8�t���[��
        AC_WALK_R_8F,				///<�E�Ɉړ��@8�t���[��
    };

#if T1662_060815_FIX
    if((_pCommPlayerWork->blowDir[netID]==DIR_NOT) ||
       (_pCommPlayerWork->pPlayer[netID]==NULL)){
        return FALSE;
    }
    if(padTrg == 0){
        return TRUE;
    }
#else //T1662_060815_FIX
    if((_pCommPlayerWork->blowDir[netID]==DIR_NOT) ||
       (padTrg == 0) ||
       (_pCommPlayerWork->pPlayer[netID]==NULL)){
        return FALSE;
    }
#endif //T1662_060815_FIX

    // �����o�����ɕ���������Ă�����ω�������
    fldobj = Player_FieldOBJGet( _pCommPlayerWork->pPlayer[netID] );   //���@�̃t�B�[���hOBJ�|�C���^�擾
    if(Player_DirGet(_pCommPlayerWork->pPlayer[netID])!=CommPlayerGetReverseDir(_pCommPlayerWork->blowDir[netID])){
        FieldOBJ_StatusBit_OFF( fldobj, FLDOBJ_STA_BIT_PAUSE_DIR );
        Player_DirSet(_pCommPlayerWork->pPlayer[netID], CommPlayerGetReverseDir(_pCommPlayerWork->blowDir[netID]));
        FieldOBJ_StatusBit_ON( fldobj, FLDOBJ_STA_BIT_PAUSE_DIR );
    }

    if( FieldOBJ_AcmdEndCheck(fldobj) == TRUE ){ //�A�j���I���`�F�b�N

	}

    if( FieldOBJ_AcmdSetCheck(fldobj) == TRUE ){//�A�j���Z�b�g�ł��邩?
        _walkCount(netID);
        OHNO_SP_PRINT("������΂�����C %d %d \n",netID, speed);
        switch( speed  ){
          case _SPEED_2:
            FieldOBJ_AcmdSet( fldobj, walkcmd[_pCommPlayerWork->blowDir[netID]] ); //�o����B�������Ɉړ�
            break;
          case _SPEED_4:
            FieldOBJ_AcmdSet( fldobj, walkcmd[_pCommPlayerWork->blowDir[netID]+4] ); //�o����B�������Ɉړ�
            break;
          case _SPEED_8:
            FieldOBJ_AcmdSet( fldobj, walkcmd[_pCommPlayerWork->blowDir[netID]+8] ); //�o����B�������Ɉړ�
            break;
        }
    }else{
        OHNO_SP_PRINT("�A�j���Z�b�g�ł��Ȃ�����\n");
        //�@�@�@�@�@�@���̃A�j�����s���ɂ��o���Ȃ��B�o����܂ő҂�
    }
    return TRUE;
}

//==============================================================================
/**
 * �N���C�A���g���̈ړ�����  �P�}�X�Â��������Ȃ��悤�ɂȂ��Ă���
 * @param   netID
 * @retval  none
 */
//==============================================================================

static void _playerMoveClient(int netID)
{
    BOOL bNoMove = FALSE, bHit = TRUE;
    u16 pad=0;
    _PlayerPlace* pPP;
    PLAYER_STATE_PTR pPlayerState;
    int speed;

    pPP = &_pCommPlayerWork->sPlayerPlace[netID];

    if(_pCommPlayerWork->walkCountClient[netID] != 0){  // ����
        _pCommPlayerWork->walkCountClient[netID]--;
        return;
    }

    if((pPP->xpos == COMM_PLAYER_INVALID_GRID) || (pPP->zpos == COMM_PLAYER_INVALID_GRID)){
        return;
    }
    if(_pCommPlayerWork->bResetting){
        return;
    }
    if(!_pCommPlayerWork->bUnder){
        if(netID == CommGetCurrentID()){
            if(!_pCommPlayerWork->bMoveControlVSRoom){
                return;
            }
        }
    }
    
    pPlayerState = _pCommPlayerWork->pPlayer[netID];
    if( pPlayerState ){
        int x = Player_NowGPosXGet( pPlayerState ) - pPP->xpos;
        int z = Player_NowGPosZGet( pPlayerState ) - pPP->zpos;
        int dir = Player_DirGet( pPlayerState );

        if((x == 0) && (z == 0)){
            pad = 0;
        }
        else if(abs(x) > abs(z)){
            if(x > 0){
                pad = PAD_KEY_LEFT;
            }
            else{
                pad = PAD_KEY_RIGHT;
            }
        }
        else{
            if(z > 0){
                pad = PAD_KEY_UP;
            }
            else{
                pad = PAD_KEY_DOWN;
            }
        }
        if(abs(x) + abs(z) > 2){
            bHit = FALSE;
        }

        if(_blowAnim(netID,pad,pad,pPP->speed)){
            return;
        }

        {
            u32 code = ACMD_NOT;
            int sp;

            speed = pPP->speed;
            switch( pPP->speed  ){
              case _SPEED_2:
                sp = JIKI_FRAME_SPEED_2F;
                break;
              case _SPEED_4:
                sp = JIKI_FRAME_SPEED_4F;
                pad |= PAD_BUTTON_B;
                break;
              case _SPEED_8:
                sp = JIKI_FRAME_SPEED_8F;
                break;
              case _SPEED_16:
                sp = JIKI_FRAME_SPEED_16F;
                break;
            }
            if(_pCommPlayerWork->forceDirFlg && (netID == CommGetCurrentID())){
                _pCommPlayerWork->forceDirFlg--;
//                code = FieldOBJ_AcmdCodeDirChange( pPP->dir, AC_STAY_WALK_U_4F );
            }
            else if(!CommIsSendMoveData() && _pCommPlayerWork->bUnder && (netID==CommGetCurrentID())){ //�ړ����b�N���ɂ͈ړ��A�j�����Ȃ�
                code = Player_KeyMoveSelectAcmdCodeGet(
                    pPlayerState, pad, pad, sp, TRUE, JIKI_MOVE_HIT_BIT_NON);
            }
            else if(((pad & ~PAD_BUTTON_B) == 0)  && (pPP->bHit) ){  // �ړ����Ȃ��ėǂ��Ă����肪����ꍇ
                speed = _SPEED_16;
                switch(pPP->dir){
                  case DIR_UP:
                    code = AC_STAY_WALK_U_16F;
                    break;
                  case DIR_DOWN:
                  default:
                    code = AC_STAY_WALK_D_16F;
                    break;
                  case DIR_LEFT:
                    code = AC_STAY_WALK_L_16F;
                    break;
                  case DIR_RIGHT:
                    code = AC_STAY_WALK_R_16F;
                    break;
                }
            }
            else if(((pad & ~PAD_BUTTON_B) == 0)  && (pPP->dir != dir) ){
                code = FieldOBJ_AcmdCodeDirChange( pPP->dir, AC_STAY_WALK_U_4F );
            }
            else{
                code = Player_KeyMoveSelectAcmdCodeGet(
                    pPlayerState, pad, pad, sp, TRUE, JIKI_MOVE_HIT_BIT_NON);
            }
            if( Player_AcmdSetCheck(pPlayerState) == FALSE ){          //�A�j���[�V�����R�}���h��
                if( Player_MoveCancellCheck(pPlayerState) == FALSE ){  //�L�����Z���s��
                    return;
                }
            }
            if(code != ACMD_NOT){
                Player_AcmdSet( pPlayerState, code, HEROACTYPE_STOP );
                if(pad & ~PAD_BUTTON_B){
                    if(_pCommPlayerWork->walkCountClient[netID] == 0){
                        _pCommPlayerWork->walkCountClient[netID] = _getWalkCount(speed);
                    }
                    if(_pCommPlayerWork->walkCountClient[netID] != 0){
                        _pCommPlayerWork->walkCountClient[netID]--;
                    }
                    _walkCount(netID);
                }
            }
        }
    }
}

//--------------------------------------------------------------
/**
 * ������΂���㩂𓥂񂾏ꍇ�̃T�[�o�[�ړ�
 * @param	netID  id
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------
static BOOL _blowMove( int netID , int cnt)
{
    int x,z;
    _PlayerPlace* pPP;
    
    if(_pCommPlayerWork->blowCounterS[netID]==0){
        return FALSE;
    }
    if(_pCommPlayerWork->blowCounterS[netID]==_BLOW_COUNTER_STOP){
        return TRUE;
    }
    _pCommPlayerWork->blowCounterS[netID]--;
    if(1 == cnt){        // ��~��
        return TRUE;
    }
    x = CommPlayerGetPosSX(netID);
    z = CommPlayerGetPosSZ(netID);
    x += FieldOBJ_DirAddValueGX(_pCommPlayerWork->blowDirS[netID]);
    z += FieldOBJ_DirAddValueGZ(_pCommPlayerWork->blowDirS[netID]);

    if(_pCommPlayerWork->blowCounterS[netID]!=0){
        if(_commGetHitAttr(x, z, netID)){
            _pCommPlayerWork->blowCounterS[netID] = 1;
        }
    }
    if(_pCommPlayerWork->blowCounterS[netID]==1){
        return TRUE;
    }
    else if(_pCommPlayerWork->blowCounterS[netID]==0){
        u8 bRet = TRUE;
        CommTrapRecvEnd(netID, 1, &bRet,NULL);
        return TRUE;
    }
    pPP = &_pCommPlayerWork->sPlayerPlaceServer[netID];
    pPP->xpos = x;
    pPP->zpos = z;
    switch(cnt){
      case 2:
        pPP->speed = _SPEED_2;
        break;
      case 4:
        pPP->speed = _SPEED_4;
        break;
      case 8:
        pPP->speed = _SPEED_8;
        break;
    }
    OHNO_SP_PRINT("������΂����� %d %d %d \n",netID, pPP->speed, _pCommPlayerWork->blowCounterS[netID]);
    return TRUE;
}

//--------------------------------------------------------------
/**
 * ������΂���㩂𓥂񂾏ꍇ�̃T�[�o���ړ����J�n
 * @param	netID   i
 * @param	dir		�ړ���������BDIR_UP��
 * @param	bBig    ������΂����傫��
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------
void CommPlayerBlowStart( int netID, int dir , BOOL bBig)
{
    _PlayerPlace* pPP = &_pCommPlayerWork->sPlayerPlaceServer[netID];
    u8 buff[3];
    
    pPP->dir = CommPlayerGetReverseDir(dir);  // ��������
    _pCommPlayerWork->blowDirS[netID] = dir;  // ��΂�������
    if(bBig){
        _pCommPlayerWork->bBlowBigS[netID] = TRUE;
        _pCommPlayerWork->blowCounterS[netID] = _BIGBLOW_COUNTER_NUM_MAX;// + _BLOW_STOP_COUNT;
    }
    else{
        _pCommPlayerWork->bBlowBigS[netID] = FALSE;
        _pCommPlayerWork->blowCounterS[netID] = _BLOW_COUNTER_NUM_MAX;// + _BLOW_STOP_COUNT;
    }
/*    buff[0]=netID;
    buff[1]=dir;
    buff[2]=bBig;
    CommSendFixSizeData_ServerSide(CF_TRAP_SECOND_START_RET,buff);*/
    _pCommPlayerWork->bMoveChange[netID]=TRUE;
}

//--------------------------------------------------------------
/**
 * ������΂���㩂������I������ꍇ�̏���
 * @param   victimNetID   ��Q��ID
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------
void CommPlayerBlowExit(int victimNetID)
{
    _pCommPlayerWork->blowCounterS[victimNetID] = 0;
}


//--------------------------------------------------------------
/**
 * ������΂���㩂��J�n����
 * @param   victimNetID   ��Q��ID
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------
void CommPlayerBlowFirst(int victimNetID)
{
    _pCommPlayerWork->blowCounterS[victimNetID] = _BLOW_COUNTER_STOP;
}

//--------------------------------------------------------------
/**
 * ������΂���㩂𓥂񂾏ꍇ�̈ړ����J�n
 * @param	dir		�ړ���������BDIR_UP��
 * @retval	�I��������TRUE
 */
//--------------------------------------------------------------
void CommPlayerBlowAnimStart( int netID, int dir , BOOL bBig)
{
	FIELD_OBJ_PTR fldobj;

    if(_pCommPlayerWork->pPlayer[netID]==NULL){
      return;
    }
    fldobj = Player_FieldOBJGet( _pCommPlayerWork->pPlayer[netID] );   //���@�̃t�B�[���hOBJ�|�C���^�擾

    FieldOBJ_StatusBit_ON( fldobj, FLDOBJ_STA_BIT_PAUSE_DIR );
    FieldOBJ_StatusBit_ON( fldobj, FLDOBJ_STA_BIT_PAUSE_ANM );
//    FE_DirTrap_Add(_pCommPlayerWork->pFSys,
  //                 CommPlayerGetPosX(netID),CommPlayerGetPosZ(netID),dir);

    _pCommPlayerWork->blowDir[netID] =dir;
}

//--------------------------------------------------------------
/**
 * ������΂���㩂𓥂񂾏ꍇ�̈ړ����~
 * @param	netID   id
 * @retval	none
 */
//--------------------------------------------------------------
void CommPlayerBlowAnimStop( int netID)
{
	FIELD_OBJ_PTR fldobj;
    if(_pCommPlayerWork->pPlayer[netID]==NULL){
      return;
    }
    
    if(_pCommPlayerWork->blowDir[netID] != DIR_NOT){
        fldobj = Player_FieldOBJGet( _pCommPlayerWork->pPlayer[netID] );   //���@�̃t�B�[���hOBJ�|�C���^�擾
        FieldOBJ_AcmdEnd(fldobj); 			 //�A�j�����S�I��
        FieldOBJ_StatusBit_OFF( fldobj, FLDOBJ_STA_BIT_PAUSE_DIR );
        FieldOBJ_StatusBit_OFF( fldobj, FLDOBJ_STA_BIT_PAUSE_ANM );
        _pCommPlayerWork->blowDir[netID] = DIR_NOT;
    }
}

//==============================================================================
/**
 * ID���ŏꏊ�𑗂�f�[�^�̃T�C�Y��Ԃ��܂�
 * @param   none
 * @retval  �f�[�^�T�C�Y
 */
//==============================================================================

int CommPlayerGetRecvIDPlaceSize(void)
{
    return _IDPLACEDATA_SEND_SIZE;
}

//==============================================================================
/**
 * ��b�f�[�^����M���܂�
 * @param   netID         ��b�𑗐M���Ă�������
 * @param   size          ��M�f�[�^�T�C�Y
 * @param   pData         ���b�Z�[�W���܂܂�Ă���f�[�^
 * @retval  none
 */
//==============================================================================

//static void _recvTalk(int netID, int size, void* pData, void* pWork)
//{
//}

//void CommPlayerRecvTalk(int netID, int size, void* pData, void* pWork)
//{
    // �����ɂ݂�ȂɕԐM
//    CommSendData_ServerSide(CF_TALK_DISP_SERVER_SIDE, pData, size);
//}

//void CommPlayerRecvTalk_ServerSide(int netID, int size, void* pData, void* pWork)
//{
///    _recvTalk(netID, size, pData, pWork);
//}


//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L�������ʐM�ɂ��ړ���Ԃɂ��邩�ǂ���
 * @param   none
 * @retval  �ʐM�ňړ�������ꍇTRUE
 */
//--------------------------------------------------------------

BOOL CommPlayerIsControl(void)
{
    if(CommGetAloneMode()){
        return TRUE;
    }
    if(!_pCommPlayerWork){
        return FALSE;
    }
    if(!CommIsConnect(CommGetCurrentID()) ){  // �I�����C���Ȃ�OK
        return FALSE;
    }
    if(_pCommPlayerWork->bUnder && !CommIsUnderGroundMoveState()){
        return FALSE;
    }
    return TRUE;

}

//--------------------------------------------------------------
/**
 * @brief   �����Ă邩�ǂ���
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  ActiveFlg
 */
//--------------------------------------------------------------

BOOL CommPlayerIsActive(int netID)
{
    return _pCommPlayerWork->bActive[netID];
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������X�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosX(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    else if(CommGetCurrentID() == netID){
        return _pCommPlayerWork->sPlayerPlace[netID].xpos;
    }
    else if(!CommPlayerIsControl() || !_pCommPlayerWork->bActive[netID]){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].xpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������Z�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosZ(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    else if(CommGetCurrentID() == netID){
        return _pCommPlayerWork->sPlayerPlace[netID].zpos;
    }
    else if(!CommPlayerIsControl() || !_pCommPlayerWork->bActive[netID]){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].zpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������X�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosXOrg(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].xpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������Z�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosZOrg(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].zpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L�����̖ڂ̑O�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosXDirAdd(int netID)
{
    if(CommPlayerGetPosX(netID) == COMM_PLAYER_INVALID_GRID){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].xpos +
        FieldOBJ_DirAddValueGX(_pCommPlayerWork->sPlayerPlace[netID].dir);
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L�����̖ڂ̑O�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosZDirAdd(int netID)
{
    if(CommPlayerGetPosZ(netID) == COMM_PLAYER_INVALID_GRID){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].zpos +
        FieldOBJ_DirAddValueGZ(_pCommPlayerWork->sPlayerPlace[netID].dir);
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������X�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosSX(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    else if(CommGetCurrentID() == netID){
        return _pCommPlayerWork->sPlayerPlaceServer[netID].xpos;
    }
    else if(!CommPlayerIsControl() || !_pCommPlayerWork->bActive[netID]){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].xpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������Z�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosSZ(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    else if(CommGetCurrentID() == netID){
        return _pCommPlayerWork->sPlayerPlaceServer[netID].zpos;
    }
    else if(!CommPlayerIsControl() || !_pCommPlayerWork->bActive[netID]){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].zpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������X�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosSXOrg(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].xpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������Z�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosSZOrg(int netID)
{
    if(!_pCommPlayerWork){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].zpos;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L�����̖ڂ̑O�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  X�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosSXDirAdd(int netID)
{
    if(CommPlayerGetPosSX(netID) == COMM_PLAYER_INVALID_GRID){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].xpos +
        FieldOBJ_DirAddValueGX(_pCommPlayerWork->sPlayerPlaceServer[netID].dir);
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L�����̖ڂ̑O�̈ʒu��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  Z�̏ꏊ
 */
//--------------------------------------------------------------

int CommPlayerGetPosSZDirAdd(int netID)
{
    if(CommPlayerGetPosSZ(netID) == COMM_PLAYER_INVALID_GRID){
        return COMM_PLAYER_INVALID_GRID;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].zpos +
        FieldOBJ_DirAddValueGZ(_pCommPlayerWork->sPlayerPlaceServer[netID].dir);
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������dir��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  dir
 */
//--------------------------------------------------------------

int CommPlayerGetDir(int netID)
{
    if(!CommPlayerIsControl() || !_pCommPlayerWork->bActive[netID]){
        return DIR_NOT;
    }
    return _pCommPlayerWork->sPlayerPlace[netID].dir;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�L������dir��Ԃ�
 * @param   netID �l�b�g���[�N�ł�ID
 * @retval  dir
 */
//--------------------------------------------------------------

int CommPlayerGetSDir(int netID)
{
    if(!CommPlayerIsControl() || !_pCommPlayerWork->bActive[netID]){
        return DIR_NOT;
    }
    return _pCommPlayerWork->sPlayerPlaceServer[netID].dir;
}

//--------------------------------------------------------------
/**
 * @brief   �^�[�Q�b�g�̊p�x��netID�Ƌt�����ɂ���  ����ƌ����������͂�
 * @param   netID �l�b�g���[�N�ł�ID
 * @param   targetID �l�b�g���[�N�ł̃^�[�Q�b�g��ID
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetLook_Server(int netID, int targetID)
{
    int dir = CommPlayerGetReverseDir(_pCommPlayerWork->sPlayerPlaceServer[netID].dir);

    _pCommPlayerWork->sPlayerPlaceServer[targetID].dir = dir;
    _pCommPlayerWork->bMoveChange[targetID] = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �^�[�Q�b�g�̊p�x��netID�Ƌt�����ɂ���  ����ƌ����������͂�
 * @param   netID �l�b�g���[�N�ł�ID
 * @param   targetID �l�b�g���[�N�ł̃^�[�Q�b�g��ID
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetLook(int netID, int targetID)
{
    int dir = CommPlayerGetReverseDir(_pCommPlayerWork->sPlayerPlace[netID].dir);
    _pCommPlayerWork->sPlayerPlace[targetID].dir = dir;
}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�������ɂ��邩�ǂ���
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

int CommPlayerIsAlive(int xpos, int zpos)
{
    int i,x,y;

    if(!_pCommPlayerWork){
        return COMM_INVALID_ID;
    }
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        if(_pCommPlayerWork->bActive[i] == FALSE){
            continue;
        }
        x = _pCommPlayerWork->sPlayerPlaceServer[i].xpos;
        y = _pCommPlayerWork->sPlayerPlaceServer[i].zpos;

        if((xpos == x) && (zpos == y)){
            return i;
        }
    }
    return COMM_INVALID_ID;
}

//--------------------------------------------------------------
/**
 * @brief   �ړ�����t���O�̐ݒ�
 * @param   
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetMoveControl_Server(int netID, BOOL bMoveControl)
{
    if(_pCommPlayerWork->bMoveControl[ netID ] != bMoveControl){
        _pCommPlayerWork->bMoveControl[ netID ] = bMoveControl;
//        OHNO_PRINT("ommPlayerSetMoveControl_Server %d  id %d\n",bMoveControl,netID);
        _pCommPlayerWork->sPlayerPlaceServer[netID].bHit = FALSE;
        _pCommPlayerWork->bMoveChange[netID] = TRUE;
        _pCommPlayerWork->sPlayerPlaceServer[netID].speed = _SPEED_8;

    }

}

//--------------------------------------------------------------
/**
 * @brief   �v���[���[�����񂫂ɂ��邩�ǂ���
 * @param   netID
 * @retval  �R���g���[���ł���Ȃ�TRUE ��荞�ݒ��Ȃ�FALSE
 */
//--------------------------------------------------------------

BOOL CommPlayerGetMoveControl(int netID)
{
    if(_pCommPlayerWork->bDataSendFlag){
        return FALSE;
    }
    if(!_pCommPlayerWork->bMoveControlFlag[ netID ]){
        return FALSE;
    }
    return _pCommPlayerWork->bMoveControl[ netID ];
}


//--------------------------------------------------------------
/**
 * @brief   �o�g�����[�����Ńv���C���[���������ʒu�ɂ������ǂ���
 * @param   none
 * @retval  �ʒu�ɂ�����TRUE
 */
//--------------------------------------------------------------

BOOL CommPlayerCheckBattleJump(void)
{
    _Grid battleGrid2[]={{4,7},{11,7}};
    _Grid battleGrid4[]={{4,6},{11,6},{4,8},{11,8}};
    int machineMax = CommGetMaxEntry(CommStateGetServiceNo());
    int i,j,count=0,stand[4],w;
    int ret = FALSE;
    _Grid* pBattleGrid;

    if(machineMax==2){
        pBattleGrid = battleGrid2;
    }
    else{
        pBattleGrid = battleGrid4;
    }

    for(i = 0; i < machineMax; i++){
        for(j = 0; j < machineMax; j++){
            if((CommPlayerGetPosX(j) == pBattleGrid[i].xpos) &&
               (CommPlayerGetPosZ(j) == pBattleGrid[i].zpos)){
                count++;
                stand[ j ] = i;
                if(j == CommGetCurrentID()){
                    ret = TRUE;
                }
                break;
            }
        }
    }
    if(count == machineMax){
        for(i = 0; i < machineMax; i++){
            CommSetStandNo(stand[ i ], i);
        }
    }
    return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �q�@���� �X�^�[�g���C�������\�t���O������
 * @param   none
 * @retval  �ʒu�ɂ�����TRUE
 */
//--------------------------------------------------------------

void CommPlayerRecvStartLineSet(int netID, int size, void* pData, void* pWork)
{
    u8* pBuff = pData;

    if(pBuff[0] == 3){
        _pCommPlayerWork->bMoveControlVS[ netID ] = FALSE;
    }
    else{
        _pCommPlayerWork->bStartLineChk[netID] = pBuff[0];
    }
}

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[�����Ńv���C���[���������ʒu�ɂ������ǂ���
 * @param   none
 * @retval  �ʒu�ɂ�����TRUE
 */
//--------------------------------------------------------------

static _Grid battleGrid2[]={{4,7},{11,7}};
static _Grid battleGrid4[]={{4,6},{11,6},{4,8},{11,8}};

static void CommPlayerCheckBattleJump_Server(void)
{
    int machineMax = CommGetMaxEntry(CommStateGetServiceNo());
    int i,j,count=0,stand[4];
    int ret = FALSE;
    _Grid* pBattleGrid;
    u8 netID;
    
    if(machineMax==2){
        pBattleGrid = battleGrid2;
    }
    else{
        pBattleGrid = battleGrid4;
    }

    for(netID = 0; netID < machineMax; netID++){
        if(!_pCommPlayerWork->bStartLineChk[netID]){
            continue;
        }
        for(i = 0; i < machineMax; i++){
            if((CommPlayerGetPosSX(netID) == pBattleGrid[i].xpos) &&
               (CommPlayerGetPosSZ(netID) == pBattleGrid[i].zpos)){
                _pCommPlayerWork->bMoveControlVS[ netID ] = TRUE;
                CommSendFixSizeData_ServerSide(CF_DIRECT_STARTPOS, &netID);
            }
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[�����Ńv���C���[���������ʒu�ɂ������̕�������
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerBattleDirSet(void)
{
    int id = CommGetCurrentID();
    int code;
    
    if(CommPlayerGetPosX(id) > 8){
        CommPlayerSetDir(DIR_LEFT);
        code = FieldOBJ_AcmdCodeDirChange( DIR_LEFT, AC_STAY_WALK_U_4F );
    }
    else{
        CommPlayerSetDir(DIR_RIGHT);
        code = FieldOBJ_AcmdCodeDirChange( DIR_RIGHT, AC_STAY_WALK_U_4F );
    }
    Player_AcmdSet( _pCommPlayerWork->pPlayer[id], code, HEROACTYPE_STOP );
    CommSetForceDir();
}

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[�����Ń|�W�V�����̌��Ɉړ�
 * @param   none
 * @retval  �ʒu�ɂ�����TRUE
 */
//--------------------------------------------------------------

BOOL CommPlayerMoveBattlePos(void)
{
    int machineMax = CommGetMaxEntry(CommStateGetServiceNo());
    int i,j,count=0,stand[4],dir;
    int ret = FALSE;

    for(i = 0; i < machineMax; i++){  // �S���̃f�[�^�����邱�Ƃ�����
#if PLFIX_T1370
		// �ȑO�̏��������ƁA�����ʒu���N���C�A���g�ɑ��M������
		// �ړ������l�𑗐M����̂ŁA�L�����N�^�����ňړ������悤�Ɍ����Ă����B
		// �����f�[�^����M����A��x�N���C�A���g�Ƀf�[�^�����M����Ă��邩�`�F�b�N����
		// �悤�ɕύX����
		if( (_pCommPlayerWork->bActive[ i ] == FALSE) || (_pCommPlayerWork->bMoveChange[ i ] == TRUE) ){
			return FALSE;
		}
#else	// PLFIX_T1370
        _PlayerPlace* pPP = &_pCommPlayerWork->sPlayerPlaceServer[i];
        if(pPP->dir == DIR_NOT){
            return FALSE;
        }
#endif	// PLFIX_T1370 
    }
    for(i = 0; i < machineMax; i++){
        _PlayerPlace* pPP = &_pCommPlayerWork->sPlayerPlaceServer[i];
        dir = CommPlayerGetReverseDir(pPP->dir);
        pPP->xpos += FieldOBJ_DirAddValueGX(dir);
        pPP->zpos += FieldOBJ_DirAddValueGZ(dir);
        pPP->speed = _SPEED_8;
        _pCommPlayerWork->bMoveChange[i] = TRUE;
        CommPlayerSetMoveControl_Server( i, TRUE);
    }
    return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���̕����Ɣ��΂̕�����Ԃ�
 * @param   dir ����
 * @retval  ���΂̕���
 */
//--------------------------------------------------------------

int CommPlayerGetReverseDir(int dir)
{
    if(dir == DIR_UP){
        return DIR_DOWN;
    }
    else if(dir == DIR_DOWN){
        return DIR_UP;
    }
    else if(dir == DIR_LEFT){
        return DIR_RIGHT;
    }
    else if(dir == DIR_RIGHT){
        return DIR_LEFT;
    }
    GF_ASSERT(0 && "NOT DIR CODE");
    return DIR_LEFT;
}


//--------------------------------------------------------------
/**
 * @brief   ������E�͂�Ă񂪑������ԃ^�C�}�[�Z�b�g
 * @param   netID   ID
 * @param   count   �J�E���g
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetGiddyStep(int netID,int count)
{
    _pCommPlayerWork->giddyCounter[netID] = count;
    _pCommPlayerWork->walkCount[netID] = 15;
}

//--------------------------------------------------------------
/**
 * @brief   ������E�͂�Ă񂪑������ԃ^�C�}�[�������I��
 * @param   netID   ID
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerResetGiddyStep(int netID)
{
    _pCommPlayerWork->giddyCounter[netID] = 0;
}

//--------------------------------------------------------------
/**
 * @brief   ���ȗp�@�@�����̂݋��t���O�̃Z�b�g
 * @param   netID   ID
 * @param   count   �J�E���g
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetHole(int netID,int count)
{
    OHNO_SP_PRINT("CommPlayerSetHole %d %d\n",netID,count);
    _pCommPlayerWork->countHole[netID] = count;
    _pCommPlayerWork->walkCount[netID] = 15;
}

//--------------------------------------------------------------
/**
 * @brief   ���ȗp�@�@�����̂݋��t���O�̃��Z�b�g
 * @param   netID   ID
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerResetHole(int netID)
{
    _pCommPlayerWork->countHole[netID] = 0;
}

//--------------------------------------------------------------
/**
 * @brief   WalkCount��������  �~�܂��Ă�̂������Ă���̂��Ɏg�p
 * @param   netID   ID
 * @retval  WalkCount
 */
//--------------------------------------------------------------

int CommPlayerGetWalkCount(int netID)
{
    return _pCommPlayerWork->walkCountClient[netID];
}

//--------------------------------------------------------------
/**
 * @brief   WalkCount��������  �~�܂��Ă�̂������Ă���̂��Ɏg�p
 * @param   netID   ID
 * @retval  WalkCount
 */
//--------------------------------------------------------------

int CommPlayerGetWalkCountServer(int netID)
{
    return _pCommPlayerWork->walkCount[netID];
}


//--------------------------------------------------------------
/**
 * @brief   WalkCount������  �ꎞ��~�Ɏg��
 * @param   netID   ID
 * @retval  WalkCount
 */
//--------------------------------------------------------------

void CommPlayerSetWalkCount(void)
{
    if(_pCommPlayerWork){
        if(!_pCommPlayerWork->bUnder){
            _pCommPlayerWork->walkCountClient[CommGetCurrentID()] = 112;
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   �ʐM������Ńv���[���[�̕�����ύX����
 * @param   int dir
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetDir(int dir)
{
    Player_DirSet(_pCommPlayerWork->pFSys->player, dir);
    _pCommPlayerWork->sPlayerPlace[ CommGetCurrentID() ].dir = dir;
    CommPlayerSendPos(FALSE);
}

//--------------------------------------------------------------
/**
 * @brief   �����ڂ̃v���[���[�̕�����ύX����
 * @param   int netID
 * @param   int dir
 * @retval  none
 */
//--------------------------------------------------------------

void CommPlayerSetClientDir(int netID, int dir)
{
    _pCommPlayerWork->sPlayerPlace[netID].dir = dir;
    Player_DirSet(_pCommPlayerWork->pPlayer[netID], dir);
}

//--------------------------------------------------------------
/**
 * @brief   �����ڂ̃v���[���[�̕����𓾂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------

int CommPlayerGetClientDir(int netID)
{
    if(!_pCommPlayerWork->pPlayer[netID]){
        return DIR_NOT;
    }
    return Player_DirGet(_pCommPlayerWork->pPlayer[netID]);
}

//--------------------------------------------------------------
/**
 * @brief   �������~�߂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------

void CommPlayerHoldBit(int bit)
{
    if(_HOLD_ALL != bit){
        _pCommPlayerWork->holdBit |= bit;
    }
    FieldSystemProc_SeqHold();
    _pCommPlayerWork->bMyMoveControl = FALSE; // ���@�̐ݒ�̂݁@���M���Ȃ�
}

//--------------------------------------------------------------
/**
 * @brief   �����ڂ̃v���[���[�̕����𓾂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------


void CommPlayerHoldBitEnd(int bit)
{
    if(_HOLD_ALL != bit){
        _pCommPlayerWork->holdBit &= ~(bit);
    }
//    OHNO_PRINT("bitend %x\n",_pCommPlayerWork->holdBit);

    if(_pCommPlayerWork->holdBit == _HOLD_TRAP){  // �g���b�v�݂̂ɂ������Ă���
        u8 bTRUE = TRUE;
//        OHNO_PRINT("�T�[�o�[�����̂ݑ��M\n");
        CommSendFixSizeData(CF_MOVE_CONTROL,&bTRUE);   // �T�[�o�[�����̂ݑ��M����
    }
    if(_pCommPlayerWork->holdBit == 0){
        if(_HOLD_TRAP != bit){
            FieldSystemProc_SeqHoldEnd();
//            OHNO_PRINT("----CommPlayerHoldBitEnd- %d--\n",bit);
            CommPlayerSetMoveControl(TRUE);  // ���@�̈ړ���Ԃ�ݒ�
        }
        else{  // �g���b�v�������ɂ̓t���O�������T�[�o�[�ɂ͑���Ȃ�
            FieldSystemProc_SeqHoldEnd();
            _pCommPlayerWork->bMyMoveControl = TRUE;
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   �������~�߂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------

void CommPlayerHoldBitOff(void)
{
    _pCommPlayerWork->holdBit = 0;
}

//--------------------------------------------------------------
/**
 * @brief   �������~�߂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------

void CommPlayerHold(void)
{
    FieldSystemProc_SeqHold();
    _pCommPlayerWork->bMyMoveControl = FALSE; // ���@�̐ݒ�̂݁@���M���Ȃ�
}

//--------------------------------------------------------------
/**
 * @brief   �������~�߂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------

void CommPlayerHold_Control(void)
{
    _pCommPlayerWork->bMyMoveControl = FALSE; // ���@�̐ݒ�̂݁@���M���Ȃ�
}

//--------------------------------------------------------------
/**
 * @brief   �����ڂ̃v���[���[�̕����𓾂�
 * @param   int netID
 * @retval  dir
 */
//--------------------------------------------------------------

void CommPlayerHoldEnd(void)
{
//    OHNO_PRINT("CommPlayerHoldEnd\n");
    FieldSystemProc_SeqHoldEnd();
    CommPlayerSetMoveControl(TRUE);  // ���@�̈ړ���Ԃ�ݒ�
}

//--------------------------------------------------------------
/**
 * @brief   �ŏ��̃R���g���[����������
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommPlayerFirstMoveEnable(void)
{
    if(_pCommPlayerWork->bFirstMoveEnable == FALSE){
        if(!_pCommPlayerWork->bUnder){
            CommPlayerHoldEnd();
            _pCommPlayerWork->bFirstMoveEnable = TRUE;
        }
        else if(UgMgrIsFirstMoveControlEnable(CommGetCurrentID())){
            CommPlayerHoldEnd();
            _pCommPlayerWork->bFirstMoveEnable = TRUE;
        }
        else{
//            CommPlayerSetMoveControl(FALSE); // �ڑ��֎~
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   �ŏ��̃R���g���[�����֎~
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommPlayerFirstMoveDisable(void)
{
    if(_pCommPlayerWork->bFirstMoveEnable == FALSE){
        if(_pCommPlayerWork->bUnder){
            if(!UgMgrIsFirstMoveControlEnable(CommGetCurrentID())){
                CommPlayerSetMoveControl(FALSE); // �ڑ��֎~
            }
        }
    }
}

//--------------------------------------------------------------
/**
 * @brief   ���j���[������ۂɃv���[���[�����ڈʒu��ݒ肷��
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommPlayerSetForcePos(void)
{
    int i,x,z,dir;
    FIELD_OBJ_PTR pFOBJ;
    
    if(!_pCommPlayerWork){
        return;
    }
    for(i = 0; i < COMM_MACHINE_MAX; i++){
        if(_pCommPlayerWork->pPlayer[i] == NULL){
            continue;
        }
        if(CommGetCurrentID()==i){
            continue;
        }
        pFOBJ = Player_FieldOBJGet(_pCommPlayerWork->pPlayer[i]);
        x = _pCommPlayerWork->sPlayerPlace[i].xpos;
        z = _pCommPlayerWork->sPlayerPlace[i].zpos;
        dir = _pCommPlayerWork->sPlayerPlace[i].dir;
//        OHNO_PRINT("�ʒu���Đݒ� %d = %d %d\n",i,x,z);
        FieldOBJ_NowPosGX_Set(pFOBJ, x);
        FieldOBJ_NowPosGZ_Set(pFOBJ, z);
        FieldOBJ_GPosDirInit(pFOBJ,x,0,z,dir);
    }
}

//--------------------------------------------------------------
/**
 * @brief   �p�x�������\�ɂ���
 * @param   xpos  �ꏊ
 * @param   zpos
 * @retval  player��netID
 */
//--------------------------------------------------------------

void CommSetForceDir(void)
{
    _pCommPlayerWork->forceDirFlg = 8;
}

//--------------------------------------------------------------
/**
 * @brief   �T�[�o����̈ړ��f�[�^�𖳎�����
 * @param   bNotMove  �ڑ����Ȃ��Ƃ�TRUE
 * @retval  none
 */
//--------------------------------------------------------------

void CommSetNotRecvData(BOOL bNotMove)
{
    _pCommPlayerWork->bNotRecvMoveData=bNotMove;
}

