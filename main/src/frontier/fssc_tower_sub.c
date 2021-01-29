//============================================================================================
/**
 * @file	fss_tower_sub.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�T�u�F�^���[
 * @author	Satoshi Nohara
 * @date	07.05.25
 *
 * b_tower_scr.c�̏������ړ�
 *
 * �֘A�\�[�X	field/scr_btower.c		�풓
 *				field/b_tower_scr.c		�t�B�[���h�T�u���풓
 *				field/b_tower_ev.c		�풓
 *				field/b_tower_wifi.c	�풓
 *				field/b_tower_fld.c		�t�B�[���h�T�u���풓
 */
//============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/record.h"
#include "savedata/frontier_savedata.h"
#include "savedata/fnote_mem.h"
#include "system/buflen.h"
#include "savedata/tv_work.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "system/procsys.h"
#include "system/gamedata.h"
#include "gflib/strbuf_family.h"
#include "application/pokelist.h"
#include "../field/fieldsys.h"
#include "field/situation.h"
#include "field/location.h"
#include "../field/tv_topic.h"
#include "field/tvtopic_extern.h"
#include "field/field_encount.h"
#include "savedata/undergrounddata.h"
#include "savedata/b_tower.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "../field/sysflag.h"
#include "../field/syswork.h"
//�t�����e�B�A�V�X�e��
#include "frontier_tool.h"
#include "../field/scr_tool.h"				//FactoryScr_GetWinRecordID
#include "itemtool/itemsym.h"
//�ʐM
//#include "communication/comm_system.h"
//#include "communication/comm_tool.h"
//#include "communication/comm_def.h"
//�o�g���^���[
#include "../field/b_tower_scr.h"			//BTOWER_SCRWORK�̐錾������
#include "../field/b_tower_ev.h"
#include "tower_tool.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//	�֐��̓���FS������(�t�����e�B�A�V�X�e���̗�)
//
//	(b_tower_scr.c��Tools�����ɂ��Ă���̂Ƃ��Ă��Ȃ��̂�����̂́H)
//
//============================================================================================
//����Tools�����Ă��Ȃ�
void FSTowerScr_ChoiceBtlPartner(BTOWER_SCRWORK* wk,SAVEDATA* sv);
u16 FSTowerScr_GetEnemyObj(BTOWER_SCRWORK* wk,u16 idx);
void FSTowerScr_SetLeaderClearFlag(BTOWER_SCRWORK* wk,u16 mode);
u16	FSTowerScr_IncRound(BTOWER_SCRWORK* wk);
u16 FSTowerScr_GetRenshouCount(BTOWER_SCRWORK* wk);


//============================================================================================
//
//	�֐�
//
//============================================================================================

//--------------------------------------------------------------
/**
 *	@brief	�ΐ�g���[�i�[���I
 */
//--------------------------------------------------------------
void FSTowerScr_ChoiceBtlPartner(BTOWER_SCRWORK* wk,SAVEDATA* sv)
{
	int i;
	u16	monsno[2];
	u16	itemno[2];
	
	switch(wk->play_mode){
	case BTWR_MODE_WIFI:
	case BTWR_MODE_RETRY:
		btltower_BtlPartnerSelectWifi(sv,wk->tr_data,wk->now_round-1);
		break;
	case BTWR_MODE_MULTI:
	case BTWR_MODE_COMM_MULTI:
	case BTWR_MODE_WIFI_MULTI:
		FSRomBattleTowerTrainerDataMake(wk,&(wk->tr_data[0]),
			wk->trainer[(wk->now_round-1)*2+0],wk->member_num,NULL,NULL,NULL,wk->heapID);
		
		//�����X�^�[No�̏d���`�F�b�N
		for(i = 0;i < wk->member_num;i++){
			monsno[i] = wk->tr_data[0].btpwd[i].mons_no;
			itemno[i] = wk->tr_data[0].btpwd[i].item_no;
		}
		FSRomBattleTowerTrainerDataMake(wk,&(wk->tr_data[1]),
			wk->trainer[(wk->now_round-1)*2+1],wk->member_num,monsno,itemno,NULL,wk->heapID);
		break;
	case BTWR_MODE_DOUBLE:
	case BTWR_MODE_SINGLE:
	default:
		FSRomBattleTowerTrainerDataMake(wk,&(wk->tr_data[0]),
			wk->trainer[wk->now_round-1],wk->member_num,NULL,NULL,NULL,wk->heapID);
		break;
	}
}

//--------------------------------------------------------------
/**
 *	@brief	�ΐ�g���[�i�[OBJ�R�[�h�擾
 */
//--------------------------------------------------------------
u16 FSTowerScr_GetEnemyObj(BTOWER_SCRWORK* wk,u16 idx)
{
	//�g���[�i�[�^�C�v����OBJ�R�[�h���擾���Ă���
	return Frontier_TrType2ObjCode(wk->tr_data[idx].bt_trd.tr_type);
}

//--------------------------------------------------------------
/**
 *	@brief	���[�_�[�N���A�t���O�𗧂Ă�
 */
//--------------------------------------------------------------
void FSTowerScr_SetLeaderClearFlag(BTOWER_SCRWORK* wk,u16 mode)
{
	wk->leader_f = mode;
}

//--------------------------------------------------------------
/**
 *	@brief	���E���h�����C���N�������g
 */
//--------------------------------------------------------------
u16	FSTowerScr_IncRound(BTOWER_SCRWORK* wk)
{
	++wk->now_win;	//���������C���N�������g

	//�v���`�i�Œǉ����ꂽWIFI�}���`�͏���
	if( wk->play_mode != BTWR_MODE_WIFI_MULTI ){
		//�����_���̎�X�V
		wk->play_rnd_seed = BtlTower_PlayFixRand(wk->play_rnd_seed);
	}

	OS_Printf("TowerRoundInc = %d\n",wk->play_rnd_seed);

	return ++wk->now_round;
}

//--------------------------------------------------------------
/**
 *	@brief	�v���C���̘A�������擾����
 */
//--------------------------------------------------------------
u16 FSTowerScr_GetRenshouCount(BTOWER_SCRWORK* wk)
{
	OS_Printf( "wk->renshou = %d\n", wk->renshou );
	OS_Printf( "wk->now_win = %d\n", wk->now_win );

	if((u32)wk->renshou+wk->now_win > 0xFFFF){
		return 0xFFFF;
	}else{
		return wk->renshou+wk->now_win;
	}
}

