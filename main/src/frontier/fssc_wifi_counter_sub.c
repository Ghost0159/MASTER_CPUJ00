//============================================================================================
/**
 * @file	fss_wifi_counter_sub.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�T�u�FWiFi��t
 * @author	Satoshi Nohara
 * @date	07.07.31
 */
//============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/record.h"
#include "savedata/fnote_mem.h"
#include "system/buflen.h"
#include "savedata/tv_work.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_number.h"
#include "system/procsys.h"
#include "system/gamedata.h"
#include "gflib/strbuf_family.h"
#include "application/pokelist.h"
#include "../field/fieldsys.h"
#include "savedata/frontier_savedata.h"
#include "savedata/sp_ribbon.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "../field/sysflag.h"
#include "../field/syswork.h"

#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "fss_task.h"
#include "fs_usescript.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"

#include "frontier_wifi.h"
#include "frontier_def.h"
#include "fssc_wifi_counter_sub.h"

#include "frontier_tool.h"
#include "comm_command_frontier.h"
#include "itemtool/itemsym.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
FRWIFI_SCRWORK* FrWiFiCounterScr_WorkAlloc( SAVEDATA* savedata );
void FrWiFiCounterScr_WorkFree( FRWIFI_SCRWORK* wk );
BOOL FrWiFiScr_CommSetSendBuf( FRWIFI_SCRWORK* wk, u16 type, u16 param, u16 param2 );
void FrWiFiCounter_PokeSelect( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, u16 seq );
static void FrWiFiCounter_PokeListCall( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys, int heapID );
//static void FrWiFiCounter_PokeListWait( void* parent_work );
static void FrWiFiCounter_PokeListWait( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys,int heapID);
static void FrWiFiCounter_PokeStatusCall( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys, int heapID );
//static void FrWiFiCounter_PokeStatusWait( void* parent_work );
static void FrWiFiCounter_PokeStatusWait( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys, int heapID );


//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A�m��
 *
 * @param	none
 *	
 * �����Ȃ炸 FrWiFiCounterScr_WorkRelease()�ŗ̈���J�����邱��
 */
//--------------------------------------------------------------
FRWIFI_SCRWORK* FrWiFiCounterScr_WorkAlloc( SAVEDATA* savedata )
{
	static FRWIFI_SCRWORK* wk;					//���ƂŊm�F]]]]]]]]]]]]]]]]]]]]]]]]]

	sys_PrintHeapFreeSize( HEAPID_WORLD );		//����q�[�v�̃������󂫗e�ʍ��v��\��
	sys_PrintHeapFreeSize( HEAPID_WORLD );		//���J���������̏����v�����g

	wk = sys_AllocMemory( HEAPID_WORLD, sizeof(FRWIFI_SCRWORK) );
	MI_CpuClear8( wk, sizeof(FRWIFI_SCRWORK) );

	//�Z�[�u�f�[�^�擾
	wk->sv = savedata;
	OS_Printf( "savedata = %d\n", savedata );
	OS_Printf( "wk->sv = %d\n", wk->sv );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	���[�N�G���A�J��
 *
 * @param	none
 */
//--------------------------------------------------------------
void FrWiFiCounterScr_WorkFree( FRWIFI_SCRWORK* wk )
{
	if( wk == NULL ){
		return;
	}

	sys_FreeMemoryEz( wk );
	wk = NULL;

	sys_PrintHeapFreeSize( HEAPID_WORLD );		//����q�[�v�̃������󂫗e�ʍ��v��\��
	sys_PrintHeapFreeSize( HEAPID_WORLD );		//���J���������̏����v�����g
	return;
}


//==============================================================================================
//
//	�ʐM(CommStart)
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���M�E�F�C�g�@
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL FrWiFiScr_CommSetSendBuf( FRWIFI_SCRWORK* wk, u16 type, u16 param, u16 param2 )
{
	int ret;

	switch( type ){

	//�{�݃i���o�[
	case FRONTIER_COMM_SEL:
		ret = CommFrWiFiCounterSendBufBFNo( wk );
		break;

	//��߂�
	case FRONTIER_COMM_YAMERU:
		ret = CommFrWiFiCounterSendBufRetireFlag( wk, param );
		break;

	//�I�񂾃����X�^�[�i���o�[�𑗐M
	case FRONTIER_COMM_MONSNO_ITEMNO:
		//pos��temoti_party����poke���擾�ł悢����
		//monsno1,monsno2,itemno1,itemno2
		ret = CommFrWiFiCounterSendBufMonsNoItemNo( wk, param, param2 );
		break;

	//�X�e�[�W�̋L�^�������Ă悢��
	case FRONTIER_COMM_STAGE_RECORD_DEL:
		ret = CommFrWiFiCounterSendBufStageRecordDel( wk, param );
		break;

	//���������V�Ԃ�
	case FRONTIER_COMM_GAME_CONTINUE:
		ret = CommFrWiFiCounterSendBufGameContinue( wk, param );
		break;
	};

	return ret;
}


//============================================================================================
//
//	�|�P�������X�g���|�P�����X�e�[�^�X
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * @brief	�|�P�������X�g���X�e�[�^�X
 *
 * @param	tcb
 *
 * @retval	none
 */
//--------------------------------------------------------------------------------------------
void FrWiFiCounter_PokeSelect( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, u16 seq )
{
	FIELDSYS_WORK* fsys	= wk->fsys;

	switch( seq ){

	//�|�P�������X�g�Ăяo��
	case FR_WIFI_POKESEL_PLIST_CALL:
		FrWiFiCounter_PokeListCall( fmain, wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�������X�g�I���`�F�b�N
	case FR_WIFI_POKESEL_PLIST_WAIT:
		FrWiFiCounter_PokeListWait( fmain, wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�����X�e�[�^�X�Ăяo��
	case FR_WIFI_POKESEL_PST_CALL:
		FrWiFiCounter_PokeStatusCall( fmain, wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�����X�e�[�^�X�I���`�F�b�N
	case FR_WIFI_POKESEL_PST_WAIT:
		FrWiFiCounter_PokeStatusWait( fmain, wk, fsys, HEAPID_WORLD );
		break;

	//�I��
	case FR_WIFI_POKESEL_EXIT:
		return;
	}

	return;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�|�P�������X�g�Ăяo��
 */
//--------------------------------------------------------------------------------------------
static void FrWiFiCounter_PokeListCall( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys,int heapID)
{
	u8 i;

	wk->pld = sys_AllocMemory( HEAPID_WORLD, sizeof(PLIST_DATA) );
	MI_CpuClearFast( wk->pld, sizeof(PLIST_DATA) );

	//PLISTDATA_Create�̒��g�Ɠ���
	wk->pld->pp			= SaveData_GetTemotiPokemon( wk->sv );
	wk->pld->myitem		= SaveData_GetMyItem( wk->sv );
	wk->pld->mailblock	= SaveData_GetMailBlock( wk->sv );

	OS_Printf( "wk->sv = %d\n", wk->sv );
	wk->pld->cfg		= SaveData_GetConfig( wk->sv );
	OS_Printf( "wk->pld->cfg = %d\n", wk->pld->cfg );

	//�^�C�v�̓V���O���Œ�ł悳����(�m�F����)
	wk->pld->type		= PL_TYPE_SINGLE;

	OS_Printf( "wk->bf_no = %d\n", wk->bf_no );

	//�^���[
	if( wk->bf_no == FRONTIER_NO_TOWER ){
		wk->pld->mode = PL_MODE_BATTLE_TOWER;
		OS_Printf( "wk->pld->mode = PL_MODE_BATTLE_TOWER\n" );

	//�X�e�[�W
	}else if( wk->bf_no == FRONTIER_NO_STAGE ){
		wk->pld->mode = PL_MODE_BATTLE_STAGE;
		OS_Printf( "wk->pld->mode = PL_MODE_BATTLE_STAGE\n" );

	//�L���b�X��
	}else if( wk->bf_no == FRONTIER_NO_CASTLE ){
		wk->pld->mode = PL_MODE_BATTLE_CASTLE;
		OS_Printf( "wk->pld->mode = PL_MODE_BATTLE_CASTLE\n" );

	//���[���b�g
	}else if( wk->bf_no == FRONTIER_NO_ROULETTE ){
		wk->pld->mode = PL_MODE_BATTLE_ROULETTE;
		OS_Printf( "wk->pld->mode = PL_MODE_BATTLE_ROULETTE\n" );

	}else{
		GF_ASSERT(0);
	}

	//wk->pld->mode		= wk->mode;
	wk->pld->fsys		= fsys;

	//�J�[�\���ʒu
	wk->pld->ret_sel	= wk->pos;

	//�I�����Ă���ʒu(���X�g���X�e�[�^�X�����X�g�ŏ�Ԃ𕜋A������)
	for( i=0; i < WIFI_COUNTER_ENTRY_POKE_MAX ;i++ ){
		wk->pld->in_num[i] = wk->sel[i];
	}

	if( wk->bf_no == FRONTIER_NO_TOWER ){
		wk->pld->in_min = 2;					//�Q���ŏ���
		wk->pld->in_max = 2;					//�Q���ő吔
		wk->pld->in_lv	= 100;					//(���Ƃ�LV50�ɐ��������)
	}else if( wk->bf_no == FRONTIER_NO_STAGE ){
		wk->pld->in_min = 1;					//�Q���ŏ���
		wk->pld->in_max = 1;					//�Q���ő吔
		wk->pld->in_lv	= 30;					//�Q�����x��30-100
	}else if( wk->bf_no == FRONTIER_NO_CASTLE ){
		wk->pld->in_min = 2;					//�Q���ŏ���
		wk->pld->in_max = 2;					//�Q���ő吔
		wk->pld->in_lv	= 100;					//���Ƃ�LV50�ɐ��������)
	}else if( wk->bf_no == FRONTIER_NO_ROULETTE ){
		wk->pld->in_min = 2;					//�Q���ŏ���
		wk->pld->in_max = 2;					//�Q���ő吔
		wk->pld->in_lv	= 100;					//���Ƃ�LV50�ɐ��������)
	}else{
		GF_ASSERT(0);
	}

	//Frontier_SubProcSet(fmain, &PokeListProcData, wk, 
	Frontier_SubProcSet(fmain, &PokeListProcData, wk->pld, 
						//FALSE, FrWiFiCounter_PokeListWait );
						FALSE, NULL );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�|�P�������X�g�I���҂�
 */
//--------------------------------------------------------------------------------------------
//static void FrWiFiCounter_PokeListWait( void* parent_work )
static void FrWiFiCounter_PokeListWait( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys,int heapID)
{
	int	ret;
	//FRWIFI_SCRWORK* wk = parent_work;
	//PLIST_DATA* wk = parent_work;

	//�f�[�^�擾
	switch( wk->pld->ret_sel ){

	case PL_SEL_POS_EXIT:					//��߂�(pld�͊J�����Ă��Ȃ�)
		*wk->ret_wk = FR_WIFI_POKESEL_EXIT;
		wk->next_seq = FR_WIFI_POKESEL_EXIT;
		return;

	case PL_SEL_POS_ENTER:					//����(pld�͊J�����Ă��Ȃ�)
		*wk->ret_wk = FR_WIFI_POKESEL_EXIT;
		wk->next_seq = FR_WIFI_POKESEL_EXIT;
		return;

	default:								//�悳���݂�
		break;
	}

	//�I�����Ă����Ԃ��A�|�P�����X�e�[�^�X���Ă񂾂��Ƃɕ��A�����邽�߂ɕK�v
	MI_CpuCopy8( wk->pld->in_num, wk->sel, WIFI_COUNTER_ENTRY_POKE_MAX );//���ݑI�΂�Ă���poke�ۑ�

	//�|�W�V������ۑ�
	wk->pos = wk->pld->ret_sel;

	sys_FreeMemoryEz(wk->pld);
	wk->pld = NULL;
	*wk->ret_wk = FR_WIFI_POKESEL_PST_CALL;
	wk->next_seq = FR_WIFI_POKESEL_PST_CALL;
	return;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�|�P�����X�e�[�^�X�Ăяo��
 */
//--------------------------------------------------------------------------------------------
static void FrWiFiCounter_PokeStatusCall( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys, int heapID )
{
	static const u8 PST_PageTbl[] = {
		PST_PAGE_INFO,			//�u�|�P�������傤�ق��v
		PST_PAGE_MEMO,			//�u�g���[�i�[�����v
		PST_PAGE_PARAM,			//�u�|�P�����̂���傭�v
		PST_PAGE_CONDITION,		//�u�R���f�B�V�����v
		PST_PAGE_B_SKILL,		//�u���������킴�v
		PST_PAGE_C_SKILL,		//�u�R���e�X�g�킴�v
		PST_PAGE_RIBBON,		//�u���˂񃊃{���v
		PST_PAGE_RET,			//�u���ǂ�v
		PST_PAGE_MAX
	};
	
	//�|�P�����X�e�[�^�X���Ăяo��
	wk->psd = sys_AllocMemoryLo( heapID, sizeof(PSTATUS_DATA) );
	MI_CpuClear8( wk->psd,sizeof(PSTATUS_DATA) );

	wk->psd->cfg		= SaveData_GetConfig( wk->sv );
	wk->psd->ppd		= SaveData_GetTemotiPokemon( wk->sv );
	wk->psd->zukan_mode	= PMNumber_GetMode( wk->sv );
	wk->psd->ev_contest	= PokeStatus_ContestFlagGet( wk->sv );
	wk->psd->ppt		= PST_PP_TYPE_POKEPARTY;
	wk->psd->pos		= wk->pos;
	wk->psd->max		= (u8)PokeParty_GetPokeCount( wk->psd->ppd );
	wk->psd->waza		= 0;
	wk->psd->mode		= PST_MODE_NORMAL;
	wk->psd->ribbon		= SaveData_GetSpRibbon( wk->sv );

	PokeStatus_PageSet( wk->psd, PST_PageTbl );
	PokeStatus_PlayerSet( wk->psd, SaveData_GetMyStatus(wk->sv) );
	
	//Frontier_SubProcSet(fmain, &PokeStatusProcData, wk, 
	Frontier_SubProcSet(fmain, &PokeStatusProcData, wk->psd, 
						//FALSE, FrWiFiCounter_PokeStatusWait );
						FALSE, NULL );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�|�P�����X�e�[�^�X�I���҂�
 */
//--------------------------------------------------------------------------------------------
//static void FrWiFiCounter_PokeStatusWait( void* parent_work )
static void FrWiFiCounter_PokeStatusWait( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, FIELDSYS_WORK* fsys, int heapID )
{
	//FRWIFI_SCRWORK* wk = parent_work;

	//�؂�ւ���ꂽ�J�����g��ۑ�����
	wk->pos = wk->psd->pos;

	sys_FreeMemoryEz( wk->psd );
	wk->psd = NULL;

	*wk->ret_wk = FR_WIFI_POKESEL_PLIST_CALL;
	wk->next_seq = FR_WIFI_POKESEL_PLIST_CALL;
	return;
}


