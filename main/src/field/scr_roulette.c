//============================================================================================
/**
 * @file	scr_roulette.c
 * @bfief	�X�N���v�g�R�}���h�F�o�g�����[���b�g(��t�܂��)
 * @author	Satoshi Nohara
 * @date	07.09.05
 */
//============================================================================================
#include "common.h"

#include <nitro/code16.h> 
#include "system/pm_str.h"
#include "system\msgdata.h"			//MSGMAN_GetString
#include "system\wordset.h"			//WordSet_RegistPlayerName
#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "system/get_text.h"
#include "system/lib_pack.h"
#include "poketool/poke_number.h"	//PMNumber_GetMode
#include "savedata/sp_ribbon.h"		//SaveData_GetSpRibbon
#include "gflib/strbuf_family.h"	//����
//�t�B�[���h
#include "fieldsys.h"
#include "field_subproc.h"
//�X�N���v�g
#include "script.h"
#include "scrcmd.h"
#include "scrcmd_def.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "sysflag.h"
#include "syswork.h"
#include "scr_tool.h"
//�o�g���L���b�X��
#include "savedata/frontier_savedata.h"
#include "scr_roulette.h"
#include "scr_roulette_sub.h"
#include "savedata/roulette_savedata.h"
#include "../frontier/roulette_def.h"
//#include "../frontier/comm_command_frontier.h"
#include "comm_command_field.h"
//�ʐM
#include "communication/comm_def.h"
#include "communication/comm_tool.h"
#include "communication/comm_system.h"


//============================================================================================
//
//	�\���̐錾
//
//============================================================================================
///�|�P�����I���C�x���g���[�N
typedef struct _ROULETTE_POKESEL_EVENT{
	int	seq;
	u8	plist_type;							//�|�P�������X�g�^�C�v
	u8	pos;								//�|�P�������X�g�̌��݃J�[�\���ʒu
	u8	sel[3];
	void** sp_wk;
}ROULETTE_POKESEL_EVENT;

///�|�P�����I���C�x���g�V�[�P���XID
typedef enum{
	ROULETTE_POKESEL_PLIST_CALL,				//�|�P�������X�g�Ăяo��
	ROULETTE_POKESEL_PLIST_WAIT,				//�|�P�������X�g�I���҂�
	ROULETTE_POKESEL_PST_CALL,					//�|�P�����X�e�[�^�X�Ăяo��
	ROULETTE_POKESEL_PST_WAIT,					//�|�P�����X�e�[�^�X�I���҂�
	ROULETTE_POKESEL_EXIT,						//�I��
};


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL EvCmdBattleRouletteTools(VM_MACHINE* core);
BOOL EvCmdBattleRouletteSetContinueNG( VM_MACHINE * core );
static void BattleRouletteSetNewChallenge( SAVEDATA* sv, ROULETTESCORE* wk, u8 type );

//�|�P�������X�g���|�P�����X�e�[�^�X
void EventCmd_RoulettePokeSelectCall( GMEVENT_CONTROL *event, void** buf, u8 plist_type );
static BOOL BtlRouletteEv_PokeSelect( GMEVENT_CONTROL *ev );
static int BtlRoulette_PokeListCall( ROULETTE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID );
static int BtlRoulette_PokeListWait( ROULETTE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys );
static int BtlRoulette_PokeStatusCall(ROULETTE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID );
static int BtlRoulette_PokeStatusWait( ROULETTE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------
/**
 *	@brief	�o�g���L���b�X���p�R�}���h�Q�Ăяo���C���^�[�t�F�[�X
 *
 *	@param	com_id		u16:�R�}���hID
 *	@param	retwk_id	u16:�Ԃ�l���i�[���郏�[�N��ID
 */
//--------------------------------------------------------------
BOOL EvCmdBattleRouletteTools(VM_MACHINE* core)
{
	u8 plist_type;
	u8 buf8[4];
	u16 buf16[4];
	u16	com_id,param,retwk_id;
	u16* ret_wk,*prm_wk;
	void** buf;
	BTOWER_SCRWORK* wk;
	ROULETTEDATA* roulette_sv;
	ROULETTESCORE* score_sv;

	com_id	= VMGetU16(core);
	param	= VMGetWorkValue(core);
	retwk_id= VMGetU16(core);
	ret_wk	= GetEventWorkAdrs( core->fsys, retwk_id );

	roulette_sv= SaveData_GetRouletteData( core->fsys->savedata );
	score_sv= SaveData_GetRouletteScore( core->fsys->savedata );
	buf		= GetEvScriptWorkMemberAdrs( core->fsys, ID_EVSCR_SUBPROC_WORK );

	switch( com_id ){

	//0:�Q���\�ȃ|�P�������̃`�F�b�N(�A�C�e���`�F�b�N�Ȃ�)
	case ROULETTE_TOOL_CHK_ENTRY_POKE_NUM:
		//*ret_wk = FrontierScrTools_CheckEntryPokeNum( param, core->fsys->savedata );
		*ret_wk = TowerScrTools_CheckEntryPokeNum( param, core->fsys->savedata, 0 );
		break;

	//1:�A�������擾
	case ROULETTE_TOOL_GET_CLEAR_FLAG:
		//WIFI�̂ݓ���
		if( param == ROULETTE_TYPE_WIFI_MULTI ){
			*ret_wk = FrontierRecord_Get(SaveData_GetFrontier(core->fsys->savedata), 
									FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
									Frontier_GetFriendIndex(FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT) );
		}else{
			*ret_wk = (u16)ROULETTESCORE_GetScoreData(	score_sv, ROULETTESCORE_ID_CLEAR_FLAG, 
														param, 0, NULL);
		}
		break;

	//3:�A�����t���O�N���A�A�A�����R�[�h�N���A�A�^�C�v�J�E���g�N���A�A�����N�N���A
	case ROULETTE_TOOL_SET_NEW_CHALLENGE:
		BattleRouletteSetNewChallenge( core->fsys->savedata, score_sv, param );
		break;

	//4:�|�P�������X�g��ʌĂяo��
	case ROULETTE_TOOL_SELECT_POKE:

		//�o�g���^�C�v�ɂ���ĕ���
		if( param == ROULETTE_TYPE_SINGLE ){
			plist_type = PL_TYPE_SINGLE;
		}else if( param == ROULETTE_TYPE_DOUBLE ){
			plist_type = PL_TYPE_DOUBLE;
		}else if( param == ROULETTE_TYPE_MULTI ){
			plist_type = PL_TYPE_MULTI;
		}else{
			plist_type = PL_TYPE_MULTI;
		}

		OS_Printf( "roulette_type = %d\n", param );
		EventCmd_RoulettePokeSelectCall( core->event_work, buf, plist_type );
		return 1;

	default:
		OS_Printf( "�n���ꂽcom_id = %d\n", com_id );
		GF_ASSERT( (0) && "com_id�����Ή��ł��I" );
		*ret_wk = 0;
		break;
	}

	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief		�X�N���v�g�R�}���h�F�o�g���L���b�X���p��NG�Z�b�g
 * @param		core
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdBattleRouletteSetContinueNG( VM_MACHINE * core )
{
	ROULETTESCORE* score_wk;
	u16 type = VMGetWorkValue(core);

	score_wk = SaveData_GetRouletteScore( core->fsys->savedata );

	BattleRouletteSetNewChallenge( core->fsys->savedata, score_wk, type );
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief		�A�����t���O�N���A�A�A�����R�[�h�N���A�A�^�C�v�J�E���g�N���A�A�����N�N���A
 * @param		core
 */
//--------------------------------------------------------------------------------------------
static void BattleRouletteSetNewChallenge( SAVEDATA* sv, ROULETTESCORE* wk, u8 type )
{
	int i;
	u16 buf16[4];
	u8 buf8[4];

	//"7�A��(�N���A)�������t���O"�̃N���A�������o��
	buf8[0] = 0;
	ROULETTESCORE_PutScoreData( wk, ROULETTESCORE_ID_CLEAR_FLAG, type, 0, buf8 );

	//WIFI�̂ݓ���
	if( type == ROULETTE_TYPE_WIFI_MULTI ){
		FrontierRecord_Set(	SaveData_GetFrontier(sv), 
							FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
							Frontier_GetFriendIndex(FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT), 0 );
	}

	//"���݂̘A����"��0�ɂ���
	FrontierRecord_Set(	SaveData_GetFrontier(sv), 
						RouletteScr_GetWinRecordID(type),
						Frontier_GetFriendIndex(RouletteScr_GetWinRecordID(type)), 0 );

	return;
}


//============================================================================================
//
//	�ʐM
//
//============================================================================================
BOOL EvCmdBattleRouletteCommMonsNo(VM_MACHINE* core);
void EventCall_RouletteComm( GMEVENT_CONTROL* event, u16 monsno, u16 monsno2, u16* ret_wk );
static BOOL GMEVENT_RouletteComm( GMEVENT_CONTROL* event );

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�ʐM�}���`�f�[�^���M
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdBattleRouletteCommMonsNo(VM_MACHINE* core)
{
	u16 monsno	= VMGetWorkValue( core );
	u16 monsno2	= VMGetWorkValue( core );
	u16* ret_wk	= VMGetWork( core );

	OS_Printf( "�o�g���L���b�X���ʐM�}���`�f�[�^���M\n" );
	OS_Printf( "�|�P�����i���o�[ monsno = %d\n", monsno );
	OS_Printf( "�|�P�����i���o�[ monsno2= %d\n", monsno2 );
	
	EventCall_RouletteComm( core->event_work, monsno, monsno2, ret_wk );
	return 1;
}
	
//--------------------------------------------------------------------------------------------
/**
 * @brief	�C�x���g�[���R�}���h�F����M
 *
 * @param	event		�C�x���g���䃏�[�N�ւ̃|�C���^
 * @param	monsno		�|�P�����i���o�[
 */
//--------------------------------------------------------------------------------------------
void EventCall_RouletteComm( GMEVENT_CONTROL* event, u16 monsno, u16 monsno2, u16* ret_wk )
{
	FLDSCR_ROULETTE_COMM* comm_wk;

	comm_wk = sys_AllocMemory( HEAPID_WORLD, sizeof(FLDSCR_ROULETTE_COMM) );
	memset( comm_wk, 0, sizeof(FLDSCR_ROULETTE_COMM) );

	comm_wk->mine_monsno[0]	= monsno;
	comm_wk->mine_monsno[1]	= monsno2;
	comm_wk->ret_wk			= ret_wk;
	//CommCommandFrontierInitialize( comm_wk );
	CommCommandFieldInitialize( comm_wk );

	FieldEvent_Call( event, GMEVENT_RouletteComm, comm_wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	����M
 *
 * @param	event		GMEVENT_CONTROL�^
 *
 * @retval	"FALSE = ���s��"
 * @retval	"TRUE = ���s�I��"
 */
//--------------------------------------------------------------------------------------------
static BOOL GMEVENT_RouletteComm( GMEVENT_CONTROL* event )
{
	FLDSCR_ROULETTE_COMM* comm_wk = FieldEvent_GetSpecialWork( event );

	switch( comm_wk->seq ){

	case 0:
		if( CommSendData(CF_ROULETTE_MONSNO,comm_wk,sizeof(FLDSCR_ROULETTE_COMM)) == TRUE ){
			comm_wk->seq++;
		}
		break;
		
	case 1:
		if( comm_wk->recieve_count >= ROULETTE_COMM_PLAYER_NUM ){

			*comm_wk->ret_wk = 0;

			//�o��|�P���������Ԃ��Ă��邩�`�F�b�N
			if( (comm_wk->mine_monsno[0] == comm_wk->pair_monsno[0]) ||
				(comm_wk->mine_monsno[0] == comm_wk->pair_monsno[1]) ){
				*comm_wk->ret_wk+=1;
			}

			if( (comm_wk->mine_monsno[1] == comm_wk->pair_monsno[0]) ||
				(comm_wk->mine_monsno[1] == comm_wk->pair_monsno[1]) ){
				*comm_wk->ret_wk+=2;
			}

			OS_Printf( "comm_wk->mine_monsno[0] = %d\n", comm_wk->mine_monsno[0] );
			OS_Printf( "comm_wk->mine_monsno[1] = %d\n", comm_wk->mine_monsno[1] );
			OS_Printf( "comm_wk->pair_monsno[0] = %d\n", comm_wk->pair_monsno[0] );
			OS_Printf( "comm_wk->pair_monsno[1] = %d\n", comm_wk->pair_monsno[1] );
			OS_Printf( "*comm_wk->ret_wk = %d\n", *comm_wk->ret_wk );
			comm_wk->seq++;
		}
		break;

	case 2:
		sys_FreeMemoryEz( comm_wk );
		return TRUE;
	};

	return FALSE;
}


//============================================================================================
//
//	�|�P�������X�g���|�P�����X�e�[�^�X
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�|�P�����I���@�T�u�C�x���g�Ăяo��
 *
 * @param	event	GMEVENT_CONTROL*
 *
 * @retval	nothing
 */
//--------------------------------------------------------------
void EventCmd_RoulettePokeSelectCall( GMEVENT_CONTROL *event, void** buf, u8 plist_type )
{
	FIELDSYS_WORK* fsys		= FieldEvent_GetFieldSysWork( event );
	ROULETTE_POKESEL_EVENT* wk = sys_AllocMemory( HEAPID_WORLD, sizeof(ROULETTE_POKESEL_EVENT) );
	MI_CpuClear8( wk, sizeof(ROULETTE_POKESEL_EVENT) );

	wk->plist_type	= plist_type;
	wk->sp_wk		= buf;

	FieldEvent_Call( fsys->event, BtlRouletteEv_PokeSelect, wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	�Q�[���C�x���g�R���g���[���@�|�P�������X�g���X�e�[�^�X
 *
 * @param	ev	GMEVENT_CONTROL *
 *
 * @retval	BOOL	TRUE=�C�x���g�I��
 */
//--------------------------------------------------------------------------------------------
static BOOL BtlRouletteEv_PokeSelect( GMEVENT_CONTROL *ev )
{
	FIELDSYS_WORK* fsys		= FieldEvent_GetFieldSysWork( ev );
	ROULETTE_POKESEL_EVENT* wk	= FieldEvent_GetSpecialWork( ev );

	switch( wk->seq ){

	//�|�P�������X�g�Ăяo��
	case ROULETTE_POKESEL_PLIST_CALL:
		wk->seq = BtlRoulette_PokeListCall( wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�������X�g�I���҂�
	case ROULETTE_POKESEL_PLIST_WAIT:
		wk->seq = BtlRoulette_PokeListWait( wk, fsys );
		break;

	//�|�P�����X�e�[�^�X�Ăяo��
	case ROULETTE_POKESEL_PST_CALL:
		wk->seq = BtlRoulette_PokeStatusCall( wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�����X�e�[�^�X�I���҂�
	case ROULETTE_POKESEL_PST_WAIT:
		wk->seq = BtlRoulette_PokeStatusWait( wk, fsys );
		break;

	//�I��
	case ROULETTE_POKESEL_EXIT:
		sys_FreeMemoryEz( wk );		//ROULETTE_POKESEL_EVENT���J��
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���L���b�X���@�|�P�������X�g�Ăяo��
 */
//--------------------------------------------------------------------------------------------
static int BtlRoulette_PokeListCall( ROULETTE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID )
{
	u8 i;

	//PLIST_DATA* pld = PLISTDATA_Create(HEAPID_WORLD, fsys, wk->plist_type, PL_MODE_BATTLE_TOWER);
	PLIST_DATA* pld = sys_AllocMemory( HEAPID_WORLD, sizeof(PLIST_DATA) );
	MI_CpuClearFast( pld, sizeof(PLIST_DATA) );

	//PILSTDATA_Create�̒��g�Ɠ���
	pld->pp			= SaveData_GetTemotiPokemon( fsys->savedata );
	pld->myitem		= SaveData_GetMyItem( fsys->savedata );
	pld->mailblock	= SaveData_GetMailBlock( fsys->savedata );
	pld->cfg		= SaveData_GetConfig( fsys->savedata );

	//�^�C�v�̓V���O���Œ�ł悳����(�m�F����)
	//pld->type		= wk->plist_type;
	pld->type		= PL_TYPE_SINGLE;

	pld->mode		= PL_MODE_BATTLE_ROULETTE;
	pld->fsys		= fsys;

	//�J�[�\���ʒu
	pld->ret_sel	= wk->pos;

	//�I�����Ă���ʒu(���X�g���X�e�[�^�X�����X�g�ŏ�Ԃ𕜋A������)
	for( i=0; i < 3 ;i++ ){
		pld->in_num[i] = wk->sel[i];
	}

	pld->in_lv		= 100;			//�Q�����x��
	pld->in_min		= 3;			//�Q���ŏ���
	pld->in_max		= 3;			//�Q���ő吔

	if( wk->plist_type == PL_TYPE_MULTI ){
		pld->in_min = 2;			//�Q���ŏ���
		pld->in_max = 2;			//�Q���ő吔
	}

	GameSystem_StartSubProc( fsys, &PokeListProcData, pld );

	*(wk->sp_wk) = pld;

	return ROULETTE_POKESEL_PLIST_WAIT;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�|�P�������X�g�I���҂�
 */
//--------------------------------------------------------------------------------------------
static int BtlRoulette_PokeListWait( ROULETTE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys )
{
	int	ret;
	PLIST_DATA* pld;

	// �T�u�v���Z�X�I���҂�
	if( FieldEvent_Cmd_WaitSubProcEnd(fsys) ) {
		return ROULETTE_POKESEL_PLIST_WAIT;
	}

	pld = *(wk->sp_wk);

	//�f�[�^�擾
	switch( pld->ret_sel ){

	case PL_SEL_POS_EXIT:					//��߂�(pld�͊J�����Ă��Ȃ�)
		return ROULETTE_POKESEL_EXIT;

	case PL_SEL_POS_ENTER:					//����(pld�͊J�����Ă��Ȃ�)
		return ROULETTE_POKESEL_EXIT;

	default:								//�悳���݂�
		break;
	}

	//�I�����Ă����Ԃ��A�|�P�����X�e�[�^�X���Ă񂾂��Ƃɕ��A�����邽�߂ɕK�v
	MI_CpuCopy8( pld->in_num, wk->sel, 3 );	//���ݑI�΂�Ă���|�P������ۑ�

	//�|�W�V������ۑ�
	wk->pos = pld->ret_sel;

	sys_FreeMemoryEz(pld);
	*(wk->sp_wk) = NULL;
	return	ROULETTE_POKESEL_PST_CALL;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���L���b�X���@�|�P�����X�e�[�^�X�Ăяo��
 */
//--------------------------------------------------------------------------------------------
static int BtlRoulette_PokeStatusCall(ROULETTE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID )
{
	PSTATUS_DATA* psd;
	SAVEDATA* sv;

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
	
	sv = fsys->savedata;

	//�|�P�����X�e�[�^�X���Ăяo��
	psd = sys_AllocMemoryLo( heapID, sizeof(PSTATUS_DATA) );
	MI_CpuClear8( psd,sizeof(PSTATUS_DATA) );

	psd->cfg		= SaveData_GetConfig( sv );
	psd->ppd		= SaveData_GetTemotiPokemon( sv );
	psd->zukan_mode	= PMNumber_GetMode( sv );
	psd->ev_contest	= PokeStatus_ContestFlagGet( sv );
	psd->ppt		= PST_PP_TYPE_POKEPARTY;
	psd->pos		= wk->pos;
	psd->max		= (u8)PokeParty_GetPokeCount( psd->ppd );
	psd->waza		= 0;
	psd->mode		= PST_MODE_NORMAL;
	psd->ribbon		= SaveData_GetSpRibbon( sv );

	PokeStatus_PageSet( psd, PST_PageTbl );
	PokeStatus_PlayerSet( psd, SaveData_GetMyStatus(sv) );
	
	GameSystem_StartSubProc( fsys, &PokeStatusProcData, psd );
	
	*(wk->sp_wk) = psd;
	return ROULETTE_POKESEL_PST_WAIT;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���L���b�X���@�|�P�����X�e�[�^�X�I���҂�
 */
//--------------------------------------------------------------------------------------------
static int BtlRoulette_PokeStatusWait( ROULETTE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys )
{
	PSTATUS_DATA* psd;

	//�T�u�v���Z�X�I���҂�
	if( FieldEvent_Cmd_WaitSubProcEnd(fsys) ) {
		return ROULETTE_POKESEL_PST_WAIT;
	}

	psd = *(wk->sp_wk);
	
	//�؂�ւ���ꂽ�J�����g��ۑ�����
	wk->pos = psd->pos;

	sys_FreeMemoryEz( psd );
	*(wk->sp_wk) = NULL;

	return ROULETTE_POKESEL_PLIST_CALL;
}


