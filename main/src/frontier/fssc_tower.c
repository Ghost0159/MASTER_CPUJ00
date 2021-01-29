//============================================================================================
/**
 * @file	fssc_tower.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�F�^���[
 * @author	Satoshi Nohara
 * @date	07.05.25
 *
 * scr_btower.c�̏������ړ�
 *
 * �֘A�\�[�X	field/scr_btower.c		�풓
 *				field/b_tower_scr.c		�t�B�[���h�T�u���풓
 *				field/b_tower_ev.c		�풓
 *				field/b_tower_wifi.c	�풓
 *				field/b_tower_fld.c		�t�B�[���h�T�u���풓
 */
//============================================================================================
#include "common.h"
#include <nitro/code16.h> 
#include "system/pm_str.h"
#include "system\msgdata.h"					//MSGMAN_GetString
#include "system/brightness.h"				//ChangeBrightnessRequest
#include "system\wordset.h"					//WordSet_RegistPlayerName
#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "system/get_text.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/wipe.h"
#include "system/lib_pack.h"
#include "system/procsys.h"
#include "system/snd_tool.h"
#include "savedata/misc.h"
#include "savedata/b_tower.h"
#include "msgdata\msg.naix"					//NARC_msg_??_dat
//�t�B�[���h								//field_encount.h	LOCATION�K�v�ABTLTOWER_PLAYWORK�K�v
#include "field/location.h"
#include "../field/field_encount.h"
#include "../field/field_battle.h"			//BattleParam_IsWinResult
#include "../field/field_subproc.h"			//TestBattleProcData
#include "field/evwkdef.h"
#include "field/eventflag.h"
//�t�����e�B�A�V�X�e��
#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "frontier_tool.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"
#include "fss_task.h"
//�o�g���^���[
#include "../field/tower_scr_common.h"		//BTOWER_SCRWORK(�V�K)
#include "../field/b_tower_scr_def.h"		//�o�g���^���[�Ŏg�p�����`

#include "fssc_tower_sub.h"
#include "tower_tool.h"
//�ʐM
#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"
//TV
#include "savedata/tv_work.h"
#include "field/tvtopic_extern.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL FSSC_TowerScrWork( FSS_TASK * core );
BOOL FSSC_TowerTalkMsgAppear(FSS_TASK* core);
BOOL FSSC_TowerWorkRelease(FSS_TASK* core);
BOOL FSSC_TowerBattleCall( FSS_TASK * core);
BOOL FSSC_TowerCallGetResult( FSS_TASK * core);
BOOL FSSC_TowerSendBuf(FSS_TASK* core);
BOOL FSSC_TowerRecvBuf(FSS_TASK* core);
static BOOL WaitTowerRecvBuf( FSS_TASK * core );
static u16 btltower_GetMineObj(BTOWER_SCRWORK* wk,u8 mode);
static BOOL BtlTowerEv_SioRecvBuf( FSS_TASK* core, BTOWER_SCRWORK* bt_scr_wk, SAVEDATA* sv, u16 mode, u16 ret_wkno );

#if	PL_T0855_080710_FIX
void BtlTower_BattleRecordUpdateEx(BTOWER_SCRWORK* wk,const BATTLE_TOWER_RECORD* rec);
#endif

//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FBTOWER_SCRWORK�̃Z�b�g�A�Q�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 *
 * �߂�l���[�N�̈����̂Ƃ����BTWR_NULL_PARAM�ɂ���ƃ_���I
 */
//--------------------------------------------------------------
BOOL FSSC_TowerScrWork( FSS_TASK * core )
{
	TV_WORK * tvwk;
	POKEMON_PARAM* pp;
	BTOWER_SCRWORK*	bt_scr_wk;		///<�o�g���^���[����p���[�N�|�C���^
	int i,j;
	u16 code,param;
	u16* ret_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	code	= FSSTGetU16( core );
	OS_Printf( "FSSC_TowerScrWork �n���ꂽ�R�[�h = %d\n", code );
	param	= FSSTGetWorkValue( core );
	ret_wk	= FSSTGetWork( core );
	//retwk_id	= FSSTGetU16(core);
	//ret_wk	= FSS_GetEventWorkAdrs( core->fsys, retwk_id );

	bt_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	switch( code ){

#if 0
	//0:�v���C���[�h�ʂ̃����o�[�����擾����
	case BTWR_TOOL_GET_MEMBER_NUM:
		*ret_wk = TowerScrTools_GetMemberNum(param);
		break;
#endif

	//2:���Z�b�g���ă^�C�g���ɖ߂�
	case BTWR_TOOL_SYSTEM_RESET:
		TowerScrTools_SystemReset();
		break;

#if 0
	//3:new game���Ƀv���C�f�[�^���N���A����
	case BTWR_TOOL_CLEAR_PLAY_DATA:
		TowerScrTools_ClearPlaySaveData(SaveData_GetTowerPlayData(ex_param->savedata));
		break;

	//4:�Z�[�u�f�[�^���L�����ǂ����Ԃ�
	case BTWR_TOOL_IS_SAVE_DATA_ENABLE:
		*ret_wk = TowerScrTools_IsSaveDataEnable(SaveData_GetTowerPlayData(ex_param->savedata));
		break;

	//8:�v���C���[�h�ʃ��R�[�h�Z�[�u�f�[�^�A������Ԃ�
	case BTWR_TOOL_GET_RENSHOU_RECORD:
		*ret_wk = TowerScrTools_GetRenshouRecord(ex_param->savedata,param);
		break;

	//9:���݂�WIFI�����N�𑀍삵�ĕԂ�
	case BTWR_TOOL_WIFI_RANK_DOWN:
		//"NULL"��n���Ă��邪���v�Ȃ̂��H
		*ret_wk = TowerScr_SetWifiRank(NULL,ex_param->savedata,2);
		break;

	//10:���݂�WIFI�����N�𑀍삵�ĕԂ�
	case BTWR_TOOL_GET_WIFI_RANK:
		//"NULL"��n���Ă��邪���v�Ȃ̂��H
		*ret_wk = TowerScr_SetWifiRank(NULL,ex_param->savedata,0);
		break;

	//11:Wifi���т̖��A�b�v���[�h�t���O�𐧌䂷��
	case BTWR_TOOL_SET_WIFI_UPLOAD_FLAG:
		TowerScrTools_SetWifiUploadFlag(ex_param->savedata,param);
		break;

	//12:WiFi�v���C���т̖��A�b�v���[�h�t���O���擾����
	case BTWR_TOOL_GET_WIFI_UPLOAD_FLAG:
		*ret_wk = TowerScrTools_GetWifiUploadFlag(ex_param->savedata);
		break;

	//14:�Z�[�u�����Ɏ~�߂��Ƃ��̃G���[����
	case BTWR_TOOL_SET_NG_SCORE:
		*ret_wk = TowerScrTools_SetNGScore(ex_param->savedata);
		break;

	//15:WIFI�̃v���C���[DL�f�[�^�����邩�ǂ����`�F�b�N
	case BTWR_TOOL_IS_PLAYER_DATA_ENABLE:
		*ret_wk = TowerScrTools_IsPlayerDataEnable(ex_param->savedata);
		break;
#endif

	//46:�o�g���|�C���g�ǉ�����(��t�O�ŌĂ΂�Ă������̂���t���Ɉړ�)
	case BTWR_SUB_ADD_BATTLE_POINT:
		*ret_wk = TowerScr_AddBtlPoint( bt_scr_wk );

		//TV:�o�g���|�C���g���l
		tvwk = SaveData_GetTvWork( ex_param->savedata );
		TVTOPIC_BPTemp_Set( tvwk, *ret_wk );
		//TVTOPIC_Entry_Record_BP( ex_param->savedata );

		//���R�[�h�X�V
		RECORD_Add( SaveData_GetRecord(ex_param->savedata), RECID_WIN_BP, *ret_wk );

		//��frontier_scrcmd.c FSSC_AddBtlPoint���`�F�b�N����

		break;

	//55:�K�v�Ȑl��OBJ�R�[�h��Ԃ�
	case BTWR_SUB_GET_MINE_OBJ:
		*ret_wk = btltower_GetMineObj(bt_scr_wk,param);
		break;

	//33:���݂̃��E���h�����X�N���v�g���[�N�Ɏ擾����
	case BTWR_SUB_GET_NOW_ROUND:
		*ret_wk = TowerScr_GetNowRound(bt_scr_wk);
		break;

	//4:���E���h�����C���N�������g
	case BTWR_SUB_INC_ROUND:
		*ret_wk = FSTowerScr_IncRound(bt_scr_wk);
		break;

	//35:7�A�����Ă��邩�ǂ����`�F�b�N
	case BTWR_SUB_IS_CLEAR:
		*ret_wk = TowerScr_IsClear(bt_scr_wk);
		break;

	//36:�v���C���̘A�������擾
	case BTWR_SUB_GET_RENSHOU_CNT:
		*ret_wk = FSTowerScr_GetRenshouCount(bt_scr_wk);
		break;

	//43:���݂̃v���C���[�h���擾
	case BTWR_SUB_GET_PLAY_MODE:
		*ret_wk = (u16)TowerScr_GetPlayMode(bt_scr_wk);
		break;

	//40:�ΐ�g���[�i�[���I
	case BTWR_SUB_CHOICE_BTL_PARTNER:
		FSTowerScr_ChoiceBtlPartner(bt_scr_wk,ex_param->savedata);
		break;

	//41:�ΐ�g���[�i�[OBJ�R�[�h�擾
	case BTWR_SUB_GET_ENEMY_OBJ:
		*ret_wk = FSTowerScr_GetEnemyObj(bt_scr_wk,param);
		break;

	//44:���[�_�[�N���A�t���O�𗧂Ă�
	case BTWR_SUB_SET_LEADER_CLEAR_FLAG:
		FSTowerScr_SetLeaderClearFlag(bt_scr_wk,param);
		break;

	//37:�s�폈��
	case BTWR_SUB_SET_LOSE_SCORE:
		TowerScr_SetLoseScore(bt_scr_wk,ex_param->savedata);
		break;

	//38:�N���A����
	case BTWR_SUB_SET_CLEAR_SCORE:
		TowerScr_SetClearScore(bt_scr_wk, ex_param->savedata, ex_param->fnote_data );
		break;

	//39:�x�ނƂ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
	case BTWR_SUB_SAVE_REST_PLAY_DATA:
		TowerScr_SaveRestPlayData(bt_scr_wk);
		break;

	//58:��M�o�b�t�@�N���A
	case BTWR_SUB_RECV_BUF_CLEAR:
		MI_CpuClear8(bt_scr_wk->recv_buf,BTWR_SIO_BUF_LEN);
		break;

	//59:����������Ȃ������Z�b�g
	case BTWR_SUB_SET_FIRST_BTL_FLAG:
		bt_scr_wk->first_btl_flag = 1;
		break;

	//100:���[�N���s�����`�F�b�N
	case BTWR_DEB_IS_WORK_NULL:
		if(bt_scr_wk == NULL){
			*ret_wk = 1;
		}else{
			*ret_wk = 0;
		}
		break;

	//-----------------------------------------------------------------------------------	
	//�G���[�`�F�b�N
	default:
		OS_Printf( "���Ή� �n���ꂽ�R�[�h = %d\n", code );
		GF_ASSERT(0);
		break;
	};

	return 0;
}

//--------------------------------------------------------------
/**
 *	@biref	�K�v�Ȑl��OBJ�R�[�h��Ԃ�
 */
//--------------------------------------------------------------
static u16 btltower_GetMineObj(BTOWER_SCRWORK* wk,u8 mode)
{
	static const u16 five_cord[] = {
	 SEVEN1,SEVEN5,SEVEN2,SEVEN3,SEVEN4
	};
	if(mode == BTWR_PTCODE_OBJ){
		return wk->partner;
	}
	if(mode == BTWR_PTCODE_MINE2){
		if(wk->play_mode == BTWR_MODE_MULTI){
			return five_cord[wk->partner];
		}else{
			if(wk->pare_sex){
				return HEROINE;
			}else{
				return HERO;
			}
		}
	}
	if(wk->my_sex){
		return HEROINE;
	}else{
		return HERO;
	}
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g���^���[�ΐ�O���b�Z�[�W��p�\��
 */
//--------------------------------------------------------------
BOOL FSSC_TowerTalkMsgAppear(FSS_TASK* core)
{
	u8 spd;
	u16	*msg;
	BTOWER_SCRWORK*	bt_scr_wk;		///<�o�g���^���[����p���[�N�|�C���^
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );
	u16	tr_idx = FSSTGetU8(core);	//��l�ڂ���l�ڂ��H

	bt_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	//wk = core->fsys->btower_wk;
	if(bt_scr_wk == NULL){
		return 0;
	}

	msg = bt_scr_wk->tr_data[tr_idx].bt_trd.appear_word;

	FrontierTalkMsgSub2( core, msg, BtlTower_GetTrMsgArc(bt_scr_wk->play_mode) );
	return 1;
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g���^���[�p���[�N���
 *
 *	@return 0
 */
//--------------------------------------------------------------
BOOL FSSC_TowerWorkRelease(FSS_TASK* core)
{
	BTOWER_SCRWORK*	bt_scr_wk;		///<�o�g���^���[����p���[�N�|�C���^
	bt_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	
	//���[�N�̈���
	TowerScr_WorkRelease( bt_scr_wk );
	//TowerScr_WorkRelease(core->fsys->btower_wk);
	//core->fsys->btower_wk = NULL;
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�퓬�Ăяo��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"1"
 */
//--------------------------------------------------------------
BOOL FSSC_TowerBattleCall( FSS_TASK * core)
{
	BATTLE_PARAM* param;
	BTOWER_SCRWORK* bt_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	bt_scr_wk = Frontier_SysWorkGet( core->fss->fmain );

	//�o�g���f�[�^������
	param = BtlTower_CreateBattleParam( bt_scr_wk, ex_param );
	bt_scr_wk->p_work = param;

	//�퓬�؂�ւ�
	Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );		//�o�g���ȍĐ�
    Frontier_SubProcSet( core->fss->fmain, &TestBattleProcData, param, FALSE, NULL );
	OS_Printf( "�X�N���v�g�^���[�퓬�Ăяo��\n" );					//field_encount.c
	return 1;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�^���[�퓬���ʎ擾���ĊJ��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_TowerCallGetResult( FSS_TASK * core)
{
	BTOWER_SCRWORK* bt_scr_wk;
	BATTLE_PARAM* param;
	u16* ret_wk;

	bt_scr_wk	= Frontier_SysWorkGet( core->fss->fmain );
	ret_wk		= FSSTGetWork( core );
	param		= bt_scr_wk->p_work;

	bt_scr_wk->winlose_flag = BattleParam_IsWinResult( param->win_lose_flag );
	OS_Printf( "bt_scr_wk->winlose_flag = %d\n", bt_scr_wk->winlose_flag );
	*ret_wk	= bt_scr_wk->winlose_flag;

#ifdef PM_DEBUG
	if( sys.cont & PAD_BUTTON_L ){
		bt_scr_wk->winlose_flag = TRUE;
		*ret_wk = TRUE;
		OS_Printf( "�f�o�b�N����ŏ�������ɂȂ�܂����I\n" );
	}
#endif

#if	PL_T0855_080710_FIX
	//field_encount.c����ړ�(08.07.10)
	//�^���[�p�ΐ팋�ʃA�b�v�f�[�g����
	{
		FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );
		BtlTower_BattleRecordUpdateEx( bt_scr_wk, &(param->btr) );
	}
#endif

	//BATTLE_PARAM�̊J��
	BattleParam_Delete( param );
	return 0;
}





#if	PL_T0855_080710_FIX
void BtlTower_BattleRecordUpdateEx(BTOWER_SCRWORK* wk,const BATTLE_TOWER_RECORD* rec)
{
	wk->rec_turn += rec->total_turn;
	wk->rec_down += rec->total_hinshi;
	wk->rec_damage += rec->total_damage;

	OS_Printf( "********************\n" );
	OS_Printf( "wk->rec_turn = %d\n", wk->rec_turn );
	OS_Printf( "wk->rec_down = %d\n", wk->rec_down );
	OS_Printf( "wk->rec_damage = %d\n", wk->rec_damage );
}
#endif



//--------------------------------------------------------------
/**
 *	@brief	�ʐM�}���`�f�[�^���M
 */
//--------------------------------------------------------------
BOOL FSSC_TowerSendBuf(FSS_TASK* core)
{
	BTOWER_SCRWORK* bt_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );
	u16 mode = FSSTGetWorkValue(core);
	u16 param = FSSTGetWorkValue(core);
	u16* ret_wk	= FSSTGetWork( core );

	bt_scr_wk	= Frontier_SysWorkGet( core->fss->fmain );

	//MI_CpuClear8(bt_scr_wk->recv_buf,BTWR_SIO_BUF_LEN);

	switch(mode){
#if 0
	case 0:	//�|�P�����I��
		BTowerComm_SendPlayerData(bt_scr_wk,ex_param->savedata);
		break;
	case 1:	//���I�g���[�i�[
		BTowerComm_SendTrainerData(bt_scr_wk);
		break;
#endif
	case 2:	//���^�C�A��I�Ԃ��H
		FSBTowerComm_SendRetireSelect(bt_scr_wk,param);
		break;
	default:
		OS_Printf( "���Ή� %d\n", mode );
		GF_ASSERT(0);
		break;
	}

	OS_Printf(">>btwr send = %d,%d,%d\n",
					bt_scr_wk->send_buf[0],bt_scr_wk->send_buf[1],bt_scr_wk->send_buf[2]);

	if( CommToolSetTempData(CommGetCurrentID(),bt_scr_wk->send_buf) == TRUE ){
		*ret_wk = 1;
	}else{
		*ret_wk = 0;
		OS_Printf( "**** tower send fail(��retry)\n" );
		return 1;		//���ӁI
	}

	//�đ��M���Ă��A�����҂��A��M�l���҂����Ȃ��̂ŁA
	//���̌オ����邩������Ȃ����A
	//WIFI�̎�������̂ŁA�đ��M�o����悤�ɂ��Ă���

	return 0;
}

#if 1
//--------------------------------------------------------------
/**
 *	@brief	�ʐM�}���`�f�[�^��M
 */
//--------------------------------------------------------------
BOOL FSSC_TowerRecvBuf(FSS_TASK* core)
{
	u16 mode		= FSSTGetU16( core );		//�ʐMtype
	u16 ret_wk_id	= FSSTGetU16( core );		//�߂�l���i�[���郏�[�NID

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = mode;
	core->reg[1] = ret_wk_id;
	OS_Printf( "================\n�o�g���^���[�ʐM�f�[�^��M\n" );
	OS_Printf( "mode = %d\n", mode );
	OS_Printf( "ret_wk_id = %d\n", ret_wk_id );

	FSST_SetWait( core, WaitTowerRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitTowerRecvBuf( FSS_TASK * core )
{
	BTOWER_SCRWORK* bt_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	bt_scr_wk = Frontier_SysWorkGet( core->fss->fmain );

	//�f�[�^��M�҂�(���ӁIreg[0]�Areg[1])
	if(BtlTowerEv_SioRecvBuf(core,bt_scr_wk,ex_param->savedata,core->reg[0],core->reg[1]) == TRUE){
		return 1;
	}

	return 0;
}

static BOOL BtlTowerEv_SioRecvBuf( FSS_TASK* core, BTOWER_SCRWORK* bt_scr_wk, SAVEDATA* sv, u16 mode, u16 ret_wkno )
{
	u16	*buf16;
	const void* recv;

	//�f�[�^��M�҂�
	recv = CommToolGetTempData(1-CommGetCurrentID());
	if(recv == NULL){
		return FALSE;
	}
	//�߂�l�w��o�b�t�@�A�h���X�擾
	//buf16 = GetEventWorkAdrs(fsys,ret_wkno);
	//buf16 = EventWork_GetEventWorkAdrs( SaveData_GetEventWork(sv), ret_wkno );
	buf16 = FSS_GetEventWorkAdrs( core, ret_wkno );

	//��M�f�[�^���[�h�ʉ��
	switch(mode){
#if 0
	case 0:
		*buf16 = BTowerComm_RecvPlayerData(bt_scr_wk,recv);
		break;
	case 1:
		*buf16 = BTowerComm_RecvTrainerData(bt_scr_wk,recv);
		break;
#endif
	case 2:
		*buf16 = FSBTowerComm_RecvRetireSelect(bt_scr_wk,recv);
		break;
	default:
		OS_Printf( "���Ή� %d\n", mode );
		GF_ASSERT(0);
		break;
	}

	return TRUE;
}
#else
//--------------------------------------------------------------
/**
 * ��M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_TowerRecvBuf( FSS_TASK * core )
{
	u16 wk_id		= FSSTGetU16( core );	//�ʐMtype���i�[���Ă��郏�[�NID
	u16 ret_wk_id	= FSSTGetU16( core );	//�߂�l���i�[���郏�[�NID

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;
	core->reg[1] = ret_wk_id;

	FSST_SetWait( core, WaitTowerRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitTowerRecvBuf( FSS_TASK * core )
{
	BTOWER_SCRWORK* bt_scr_wk;
	u16 type	= FSS_GetEventWorkValue( core, core->reg[0] );		//���ӁI
	u16* ret_wk = FSS_GetEventWorkAdrs( core, core->reg[1] );		//���ӁI

	bt_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	if( FactoryScr_CommWaitRecvBuf(bf_scr_wk,type) ){ 
		return 1;
	}

	return 0;
}
#endif


//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


