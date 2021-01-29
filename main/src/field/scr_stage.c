//============================================================================================
/**
 * @file	scr_stage.c
 * @bfief	�X�N���v�g�R�}���h�F�o�g���X�e�[�W(��t�܂��)
 * @author	Satoshi Nohara
 * @date	06.06.13
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
#include "savedata/frontier_savedata.h"
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
//�o�g���X�e�[�W
#include "scr_stage.h"
#include "scr_stage_sub.h"
#include "savedata/stage_savedata.h"
#include "../frontier/stage_def.h"
//�ʐM
#include "communication/comm_def.h"
#include "communication/comm_tool.h"
#include "communication/comm_system.h"

//#include "../frontier/comm_command_frontier.h"
#include "comm_command_field.h"
#include "frontier/frontier_tool.h"


//============================================================================================
//
//	�\���̐錾
//
//============================================================================================
///�|�P�����I���C�x���g���[�N
typedef struct _STAGE_POKESEL_EVENT{
	int	seq;
	u8	plist_type;							//�|�P�������X�g�^�C�v
	u8	pos;								//�|�P�������X�g�̌��݃J�[�\���ʒu
	u8	sel[STAGE_ENTRY_POKE_MAX];
	void** sp_wk;
}STAGE_POKESEL_EVENT;

///�|�P�����I���C�x���g�V�[�P���XID
typedef enum{
	STAGE_POKESEL_PLIST_CALL,				//�|�P�������X�g�Ăяo��
	STAGE_POKESEL_PLIST_WAIT,				//�|�P�������X�g�I���҂�
	STAGE_POKESEL_PST_CALL,					//�|�P�����X�e�[�^�X�Ăяo��
	STAGE_POKESEL_PST_WAIT,					//�|�P�����X�e�[�^�X�I���҂�
	STAGE_POKESEL_EXIT,						//�I��
};


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL EvCmdBattleStageTools(VM_MACHINE* core);
static BOOL StageScrTools_CheckEntryPokeNum(u16 num,SAVEDATA *savedata);
BOOL EvCmdBattleStageSetContinueNG( VM_MACHINE * core );

BOOL EvCmdBattleStageCommMonsNo(VM_MACHINE* core);
void EventCall_StageComm( GMEVENT_CONTROL* event, u16 monsno, u16* ret_wk );
static BOOL GMEVENT_StageComm( GMEVENT_CONTROL* event );

//�|�P�������X�g���|�P�����X�e�[�^�X
void EventCmd_StagePokeSelectCall( GMEVENT_CONTROL *event, void** buf, u8 plist_type );
static BOOL BtlStageEv_PokeSelect( GMEVENT_CONTROL *ev );
static int BtlStage_PokeListCall( STAGE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID );
static int BtlStage_PokeListWait( STAGE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys );
static int BtlStage_PokeStatusCall(STAGE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID );
static int BtlStage_PokeStatusWait( STAGE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�p�R�}���h�Q�Ăяo���C���^�[�t�F�[�X
 *
 *	@param	com_id		u16:�R�}���hID
 *	@param	retwk_id	u16:�Ԃ�l���i�[���郏�[�N��ID
 */
//--------------------------------------------------------------
BOOL EvCmdBattleStageTools(VM_MACHINE* core)
{
	u8 plist_type;
	u8 buf8[4];
	u16 buf16[4];
	u16	com_id,param,retwk_id;
	u16* ret_wk,*prm_wk;
	void** buf;
	BTOWER_SCRWORK* wk;
	STAGEDATA* stage_sv;
	STAGESCORE* score_sv;

	com_id	= VMGetU16(core);
	param	= VMGetWorkValue(core);
	retwk_id= VMGetU16(core);
	ret_wk	= GetEventWorkAdrs( core->fsys, retwk_id );

	stage_sv= SaveData_GetStageData( core->fsys->savedata );
	score_sv= SaveData_GetStageScore( core->fsys->savedata );
	buf		= GetEvScriptWorkMemberAdrs( core->fsys, ID_EVSCR_SUBPROC_WORK );

	switch( com_id ){

	//0:�Q���\�ȃ|�P�������̃`�F�b�N(�V���O����p�ȏ����ɂȂ��Ă���)
	case STAGE_TOOL_CHK_ENTRY_POKE_NUM:
		*ret_wk = StageScrTools_CheckEntryPokeNum( param, core->fsys->savedata );
		break;

	//1:�A�������擾
	case STAGE_TOOL_GET_CLEAR_FLAG:
		//WIFI�̂ݓ���
		if( param == STAGE_TYPE_WIFI_MULTI ){
			*ret_wk = FrontierRecord_Get(SaveData_GetFrontier(core->fsys->savedata), 
										FRID_STAGE_MULTI_WIFI_CLEAR_BIT,
										Frontier_GetFriendIndex(FRID_STAGE_MULTI_WIFI_CLEAR_BIT) );
		}else{
			*ret_wk = (u16)STAGESCORE_GetScoreData( score_sv, STAGESCORE_ID_CLEAR_FLAG, 
													param, 0, NULL);
		}
		break;

	//2:�A�����̃|�P�����i���o�[���擾
	case STAGE_TOOL_GET_CLEAR_MONSNO:
		*ret_wk = FrontierRecord_Get(SaveData_GetFrontier(core->fsys->savedata), 
									StageScr_GetMonsNoRecordID(param),
									Frontier_GetFriendIndex(StageScr_GetMonsNoRecordID(param)) );
		break;

	//3:�A�����t���O�N���A�A�A�����R�[�h�N���A�A�^�C�v�J�E���g�N���A
	case STAGE_TOOL_SET_NEW_CHALLENGE:
		BattleStageSetNewChallenge( core->fsys->savedata, score_sv, param );
		break;

	//4:�|�P�������X�g��ʌĂяo��
	case STAGE_TOOL_SELECT_POKE:

		//�o�g���^�C�v�ɂ���ĕ���
		if( param == STAGE_TYPE_SINGLE ){
			plist_type = PL_TYPE_SINGLE;
		}else if( param == STAGE_TYPE_DOUBLE ){
			plist_type = PL_TYPE_DOUBLE;
		}else if( param == STAGE_TYPE_MULTI ){
			plist_type = PL_TYPE_MULTI;
		}else{
			plist_type = PL_TYPE_MULTI;
		}

		OS_Printf( "stage_type = %d\n", param );
		EventCmd_StagePokeSelectCall( core->event_work, buf, plist_type );
		return 1;

	default:
		OS_Printf( "�n���ꂽcom_id = %d\n", com_id );
		GF_ASSERT( (0) && "com_id�����Ή��ł��I" );
		*ret_wk = 0;
		break;
	}

	return 0;
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�Q���\�ȃ|�P�������̃`�F�b�N
 *
 *	@param	num		�Q���ɕK�v�ȃ|�P������
 *
 *	@retval	true	�Q���\
 *	@retval	false	�Q���s��
 *
 *	LV30�ȏ�ŁA�^�}�S�łȂ��āA�`���n�ȊO�����C���邩
 */
//--------------------------------------------------------------
static BOOL StageScrTools_CheckEntryPokeNum(u16 num,SAVEDATA *savedata)
{
	u8 i,j,ct,pokenum;
	u16	monsno,level;
	POKEPARTY* party;
	POKEMON_PARAM* pp;
	u16 poke_tbl[6];
	
	party	= SaveData_GetTemotiPokemon( savedata );
	pokenum = PokeParty_GetPokeCount( party );

	//�莝���̐����K�v�ȃ|�P��������������
	if( pokenum < num ){
		return FALSE;
	}

	//������
	for( i=0; i < 6 ;i++ ){
		poke_tbl[i] = 0;
	}

	for( i=0, ct=0; i < pokenum ;i++ ){
		pp		= PokeParty_GetMemberPointer( party, i );
		monsno	= PokeParaGet( pp, ID_PARA_monsno, NULL );		//�|�P�����i���o�[
		level	= PokeParaGet( pp, ID_PARA_level, NULL );		//���x��
		
		//�^�}�S�łȂ����`�F�b�N
		if( PokeParaGet(pp,ID_PARA_tamago_flag,NULL) != 0 ){
			continue;
		}

		//���x������
		if( level < 30 ){
			continue;	
		}

		//�o�g���^���[�ɎQ���ł��Ȃ��|�P���������`�F�b�N
		if( BattleTowerExPokeCheck_MonsNo(monsno) == TRUE ){
			continue;
		}

		ct++;
		poke_tbl[i] = monsno;
	}

	//////////////////////////////////////////////////////////////////////
	//�X�e�[�W�ŁA2�C�Q���̎��͓����|�P�����łȂ���΂����Ȃ�
	if( num == 2 ){

		for( j=0; j < pokenum ;j++ ){

			for( i=0; i < pokenum ;i++ ){

				//�����v�f���͔�r���Ȃ�
				if( j != i ){

					//�l�������Ă�����
					if( poke_tbl[j] != 0 ){

						//�����|�P���������邩�`�F�b�N
						if( poke_tbl[j] == poke_tbl[i] ){
							return TRUE;
						}
					}
				}
			}
		}

		//�Q��������Ȃ�
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////

	//������ʉ߂����|�P���������`�F�b�N
	if( ct < num ){
		return FALSE;
	}

	return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief		�X�N���v�g�R�}���h�F�o�g���X�e�[�W�p��NG�Z�b�g
 * @param		core
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdBattleStageSetContinueNG( VM_MACHINE * core )
{
	STAGESCORE* score_wk;
	u16 type = VMGetWorkValue(core);

	score_wk = SaveData_GetStageScore( core->fsys->savedata );

	BattleStageSetNewChallenge( core->fsys->savedata, score_wk, type );
	return 0;
}


//============================================================================================
//
//	�ʐM
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�ʐM�}���`�f�[�^���M
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdBattleStageCommMonsNo(VM_MACHINE* core)
{
	u16 monsno	= VMGetWorkValue( core );
	u16* ret_wk	= VMGetWork( core );

	OS_Printf( "�o�g���X�e�[�W�ʐM�}���`�f�[�^���M\n" );
	OS_Printf( "�|�P�����i���o�[ monsno = %d\n", monsno );
	
	EventCall_StageComm( core->event_work, monsno, ret_wk );
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
void EventCall_StageComm( GMEVENT_CONTROL* event, u16 monsno, u16* ret_wk )
{
	FLDSCR_STAGE_COMM* comm_wk;

	comm_wk = sys_AllocMemory( HEAPID_WORLD, sizeof(FLDSCR_STAGE_COMM) );
	memset( comm_wk, 0, sizeof(FLDSCR_STAGE_COMM) );

	comm_wk->mine_monsno = monsno;
	comm_wk->ret_wk = ret_wk;
	//CommCommandFrontierInitialize( comm_wk );
	CommCommandFieldInitialize( comm_wk );

	FieldEvent_Call( event, GMEVENT_StageComm, comm_wk );
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
static BOOL GMEVENT_StageComm( GMEVENT_CONTROL* event )
{
	FLDSCR_STAGE_COMM* comm_wk = FieldEvent_GetSpecialWork( event );

	switch( comm_wk->seq ){

	case 0:
		if( CommSendData(CF_STAGE_MONSNO,comm_wk,sizeof(FLDSCR_STAGE_COMM)) == TRUE ){
			comm_wk->seq++;
		}
		break;
		
	case 1:
		if( comm_wk->recieve_count >= STAGE_COMM_PLAYER_NUM ){

			//�����|�P������I��ł��邩�`�F�b�N
			if( comm_wk->mine_monsno == comm_wk->pair_monsno ){
				*comm_wk->ret_wk = 0;
			}else{
				*comm_wk->ret_wk = 1;
			}

			OS_Printf( "comm_wk->mine_monsno = %d\n", comm_wk->mine_monsno );
			OS_Printf( "comm_wk->pair_monsno = %d\n", comm_wk->pair_monsno );
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
void EventCmd_StagePokeSelectCall( GMEVENT_CONTROL *event, void** buf, u8 plist_type )
{
	FIELDSYS_WORK* fsys		= FieldEvent_GetFieldSysWork( event );
	STAGE_POKESEL_EVENT* wk = sys_AllocMemory( HEAPID_WORLD, sizeof(STAGE_POKESEL_EVENT) );
	MI_CpuClear8( wk, sizeof(STAGE_POKESEL_EVENT) );

	wk->plist_type	= plist_type;
	wk->sp_wk		= buf;

	FieldEvent_Call( fsys->event, BtlStageEv_PokeSelect, wk );
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
static BOOL BtlStageEv_PokeSelect( GMEVENT_CONTROL *ev )
{
	FIELDSYS_WORK* fsys		= FieldEvent_GetFieldSysWork( ev );
	STAGE_POKESEL_EVENT* wk	= FieldEvent_GetSpecialWork( ev );

	switch( wk->seq ){

	//�|�P�������X�g�Ăяo��
	case STAGE_POKESEL_PLIST_CALL:
		wk->seq = BtlStage_PokeListCall( wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�������X�g�I���҂�
	case STAGE_POKESEL_PLIST_WAIT:
		wk->seq = BtlStage_PokeListWait( wk, fsys );
		break;

	//�|�P�����X�e�[�^�X�Ăяo��
	case STAGE_POKESEL_PST_CALL:
		wk->seq = BtlStage_PokeStatusCall( wk, fsys, HEAPID_WORLD );
		break;

	//�|�P�����X�e�[�^�X�I���҂�
	case STAGE_POKESEL_PST_WAIT:
		wk->seq = BtlStage_PokeStatusWait( wk, fsys );
		break;

	//�I��
	case STAGE_POKESEL_EXIT:
		sys_FreeMemoryEz( wk );		//STAGE_POKESEL_EVENT���J��
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�|�P�������X�g�Ăяo��
 */
//--------------------------------------------------------------------------------------------
static int BtlStage_PokeListCall( STAGE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID )
{
	u8 i;

	//PLIST_DATA* pld = PLISTDATA_Create(HEAPID_WORLD, fsys, wk->plist_type, PL_MODE_BATTLE_STAGE);
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

	pld->mode		= PL_MODE_BATTLE_STAGE;
	pld->fsys		= fsys;

	//�J�[�\���ʒu
	pld->ret_sel	= wk->pos;

	//�I�����Ă���ʒu(���X�g���X�e�[�^�X�����X�g�ŏ�Ԃ𕜋A������)
	for( i=0; i < STAGE_ENTRY_POKE_MAX ;i++ ){
		pld->in_num[i] = wk->sel[i];
	}

	pld->in_lv		= 30;			//�Q�����x��30-100

	pld->in_min		= 1;			//�Q���ŏ���
	pld->in_max		= 1;			//�Q���ő吔

	if( wk->plist_type == PL_TYPE_DOUBLE ){
		pld->in_min = 2;			//�Q���ŏ���
		pld->in_max = 2;			//�Q���ő吔
	}

	GameSystem_StartSubProc( fsys, &PokeListProcData, pld );

	*(wk->sp_wk) = pld;

	return STAGE_POKESEL_PLIST_WAIT;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�|�P�������X�g�I���҂�
 */
//--------------------------------------------------------------------------------------------
static int BtlStage_PokeListWait( STAGE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys )
{
	int	ret;
	PLIST_DATA* pld;

	// �T�u�v���Z�X�I���҂�
	if( FieldEvent_Cmd_WaitSubProcEnd(fsys) ) {
		return STAGE_POKESEL_PLIST_WAIT;
	}

	pld = *(wk->sp_wk);

	//�f�[�^�擾
	switch( pld->ret_sel ){

	case PL_SEL_POS_EXIT:					//��߂�(pld�͊J�����Ă��Ȃ�)
		return STAGE_POKESEL_EXIT;

	case PL_SEL_POS_ENTER:					//����(pld�͊J�����Ă��Ȃ�)
		return STAGE_POKESEL_EXIT;

	default:								//�悳���݂�
		break;
	}

	//�I�����Ă����Ԃ��A�|�P�����X�e�[�^�X���Ă񂾂��Ƃɕ��A�����邽�߂ɕK�v
	MI_CpuCopy8( pld->in_num, wk->sel, STAGE_ENTRY_POKE_MAX );	//���ݑI�΂�Ă���|�P������ۑ�

	//�|�W�V������ۑ�
	wk->pos = pld->ret_sel;

	sys_FreeMemoryEz(pld);
	*(wk->sp_wk) = NULL;
	return	STAGE_POKESEL_PST_CALL;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�|�P�����X�e�[�^�X�Ăяo��
 */
//--------------------------------------------------------------------------------------------
static int BtlStage_PokeStatusCall(STAGE_POKESEL_EVENT* wk,FIELDSYS_WORK* fsys,int heapID )
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
	return STAGE_POKESEL_PST_WAIT;
}

//--------------------------------------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�@�|�P�����X�e�[�^�X�I���҂�
 */
//--------------------------------------------------------------------------------------------
static int BtlStage_PokeStatusWait( STAGE_POKESEL_EVENT* wk, FIELDSYS_WORK* fsys )
{
	PSTATUS_DATA* psd;

	//�T�u�v���Z�X�I���҂�
	if( FieldEvent_Cmd_WaitSubProcEnd(fsys) ) {
		return STAGE_POKESEL_PST_WAIT;
	}

	psd = *(wk->sp_wk);
	
	//�؂�ւ���ꂽ�J�����g��ۑ�����
	wk->pos = psd->pos;

	sys_FreeMemoryEz( psd );
	*(wk->sp_wk) = NULL;

	return STAGE_POKESEL_PLIST_CALL;
}

//--------------------------------------------------------------------------------------------
/**
 * �S�|�P�����̘A�����̍��v���w��o�b�t�@�ɓo�^(u32)
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
#include "savedata/frontier_ex.h"
BOOL EvCmdBattleStageNumberName( VM_MACHINE * core );


typedef struct{
	u32 total_num;			//�g�[�^�����R�[�h
	u32 get_bp;				//�Ⴆ��BP
}STAGE_TOTAL_NUM_WORK;

static const STAGE_TOTAL_NUM_WORK stage_total_num[] = {
	{ 10,			1 },
	{ 30,			3 },
	{ 50,			5 },
	{ 100,			5 },
	{ 150,			5 },
	{ 200,			5 },
	{ 250,			5 },
	{ 300,			5 },
	{ 350,			5 },
	{ 400,			5 },
	{ 450,			5 },
	{ 500,			10 },
	{ 600,			10 },
	{ 700,			10 },
	{ 800,			10 },
	{ 900,			10 },
	{ 1000,			10 },
	{ 1200,			30 },
	{ 1400,			30 },
	{ 1600,			30 },
	{ 1800,			30 },
	{ 2000,			50 },
	{ 2500,			50 },
	{ 3000,			50 },
	{ 3500,			50 },
	{ 4000,			50 },
	{ 4500,			50 },
	{ 5000,			50 },
	{ 5500,			50 },
	{ 6000,			50 },
	{ 6500,			50 },
	{ 7000,			50 },
	{ 7500,			50 },
	{ 8000,			50 },
	{ 8500,			50 },
	{ 9000,			50 },
	{ 9500,			50 },
	{ 10000,		100 },
	{ 20000,		200 },
	{ 30000,		300 },
	{ 40000,		400 },
	{ 50000,		500 },
	{ 60000,		600 },
	{ 70000,		700 },
	{ 80000,		800 },
	{ 90000,		900 },
	{ 100000,		1000 },
};
#define STAGE_TOTAL_NUM_TBL_MAX		( NELEMS(stage_total_num) )

BOOL EvCmdBattleStageNumberName( VM_MACHINE * core ) 
{
	LOAD_RESULT load_result;
	SAVE_RESULT save_result;
	FRONTIER_EX_SAVEDATA* fes;
	FRONTIER_SAVEWORK* f_sv;
	u32 num,i,clear_lv;
	u16 get_bp,now_lv;
	FIELDSYS_WORK * fsys	= core->fsys;
	WORDSET** wordset		= GetEvScriptWorkMemberAdrs( fsys, ID_EVSCR_WORDSET );
	u8 idx_1				= VMGetU8(core);
	u8 idx_2				= VMGetU8(core);
	u8 idx_3				= VMGetU8(core);
	u8 idx_4				= VMGetU8(core);
	u16* lv_wk				= VMGetWork( core );
	u16* ret_wk				= VMGetWork( core );

	num	= 0;
	load_result = LOAD_RESULT_OK;
	save_result = SAVE_RESULT_OK;
	
	if(SaveData_GetExtraInitFlag(fsys->savedata) == FALSE){
		*ret_wk = 0;		//���R�[�h�Ȃ�
		return 0;	//�O���Z�[�u������������Ă��Ȃ��̂Ńf�[�^�����Ɣ���
	}

	fes = FrontierEx_Load( fsys->savedata, HEAPID_EVENT, &load_result );
	if( load_result != LOAD_RESULT_OK ){
		num = 0;	//�Z�[�u�j��or�L�[�ƈ�v���Ȃ��A�ꍇ�͐V�K�f�[�^�Ɖ���
	}else{
		//�S�|�P�����̘A�����̍��v���v�Z
		for( i=0; i < MONSNO_END ;i++ ){
			num += FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_SINGLE, i );
			OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_SINGLE, i ) );
		}
	}

	//�f�o�b�N//////////////////////////
	//num = 100000;
	////////////////////////////////////

	OS_Printf( "stage total num = %d\n", num );
	
	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}

	//�g�[�^�����R�[�h���
	WORDSET_RegisterNumber( *wordset, idx_1, num, GetNumKeta(num), 
							NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

#if 1
	get_bp	= 0;		//�Ⴆ��BP���[�N
	clear_lv= 0;		//�N���A����LV�����郏�[�N��������
	now_lv	= *lv_wk;	//���݂�LV��ޔ�
	//OS_Printf( "*lv_wk = %d\n", *lv_wk );
	
	for( i= (*lv_wk); i < STAGE_TOTAL_NUM_TBL_MAX ;i++ ){
		if( stage_total_num[i].total_num <= num ){
			get_bp += stage_total_num[i].get_bp;
			*lv_wk += 1;
			//OS_Printf( "*lv_wk = %d\n", *lv_wk );
			clear_lv = i;
		}
	}

	//���R�[�h�X�V
	RECORD_Add( SaveData_GetRecord(core->fsys->savedata), RECID_WIN_BP, get_bp );

	//�o�g���|�C���g�����Z����
	if( get_bp != 0 ){
		TowerScoreData_SetBattlePoint(
			SaveData_GetTowerScoreData(core->fsys->savedata),get_bp,BTWR_DATA_add);
	}

	OS_Printf( "*lv_wk = %d\n", *lv_wk );
	OS_Printf( "get_bp = %d\n", get_bp );
#endif

	if( num == 0 ){
		*ret_wk = 0;		//���R�[�h�Ȃ�
	}else{
		if( now_lv != (*lv_wk) ){
			*ret_wk = 1;	//���R�[�h�X�V
		}else{
			*ret_wk = 2;	//���R�[�h�X�V���ĂȂ�
		}

		if( now_lv >= STAGE_TOTAL_NUM_TBL_MAX ){
			*ret_wk = 3;	////�S�ăQ�b�g����
			return 0;
		}
	}

	//�������g�[�^�����R�[�h��LV�̒l����
	WORDSET_RegisterNumber( *wordset, idx_2, stage_total_num[clear_lv].total_num, 
							GetNumKeta(stage_total_num[clear_lv].total_num), 
							NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

	//���̃g�[�^�����R�[�h��LV�̒l����
	WORDSET_RegisterNumber( *wordset, idx_3, stage_total_num[*lv_wk].total_num, 
							GetNumKeta(stage_total_num[*lv_wk].total_num), 
							NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );

	//�Ⴆ��BP���
	WORDSET_RegisterNumber( *wordset, idx_4, get_bp, GetNumKeta(get_bp), 
							NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );
	
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * ���R�[�h������|�P���������擾
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 *
 * �t�@���Ŏg�p(�V���O���A�_�u���A�}���`���Ώ�)
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdBattleStageRecordPokeGet( VM_MACHINE * core );
BOOL EvCmdBattleStageRecordPokeGet( VM_MACHINE * core )
{
	LOAD_RESULT load_result;
	FRONTIER_EX_SAVEDATA* fes;
	u32 num,i,count;
	FIELDSYS_WORK * fsys	= core->fsys;
	u16* ret_wk				= VMGetWork( core );

	count = 0;
	load_result = LOAD_RESULT_OK;
	
	if(SaveData_GetExtraInitFlag(fsys->savedata) == FALSE){
		*ret_wk = 0;
		return 0;	//�O���Z�[�u������������Ă��Ȃ��̂Ńf�[�^�����Ɣ���
	}

	fes = FrontierEx_Load( fsys->savedata, HEAPID_EVENT, &load_result );
	if( load_result != LOAD_RESULT_OK ){
		count = 0;	//�Z�[�u�j��or�L�[�ƈ�v���Ȃ��A�ꍇ�͐V�K�f�[�^�Ɖ���
	}else{
		for( i=0; i < MONSNO_END ;i++ ){
			num = 0;

			//�V���O��
			num += FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_SINGLE, i );

			//�_�u��
			num += FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_DOUBLE, i );

			//�}���`
			num += FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_MULTI, i );

			//OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
			//									FREXID_STAGE_RENSHOU_SINGLE, i ) );

			if( num > 0 ){
				count++;
			}
		}
	}

	//�f�o�b�N//////////////////////////
	//count = 300;
	////////////////////////////////////

	OS_Printf( "stage record poke count = %d\n", count );
	
	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}

	*ret_wk = count;
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * �g�[�^�����R�[�h���擾(10000�ȏ��10000�Ƃ��Ď擾)
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdBattleStageTotalRecordGetEx( VM_MACHINE * core );
BOOL EvCmdBattleStageTotalRecordGetEx( VM_MACHINE * core ) 
{
	LOAD_RESULT load_result;
	SAVE_RESULT save_result;
	FRONTIER_EX_SAVEDATA* fes;
	FRONTIER_SAVEWORK* f_sv;
	u32 num,i;
	FIELDSYS_WORK * fsys	= core->fsys;
	u16* ret_wk				= VMGetWork( core );

	num	= 0;
	load_result = LOAD_RESULT_OK;
	save_result = SAVE_RESULT_OK;
	
	if(SaveData_GetExtraInitFlag(fsys->savedata) == FALSE){
		*ret_wk = 0;
		return 0;	//�O���Z�[�u������������Ă��Ȃ��̂Ńf�[�^�����Ɣ���
	}

	fes = FrontierEx_Load( fsys->savedata, HEAPID_EVENT, &load_result );
	if( load_result != LOAD_RESULT_OK ){
		num = 0;	//�Z�[�u�j��or�L�[�ƈ�v���Ȃ��A�ꍇ�͐V�K�f�[�^�Ɖ���
	}else{
		//�S�|�P�����̘A�����̍��v���v�Z
		for( i=0; i < MONSNO_END ;i++ ){
			num += FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_SINGLE, i );
			OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get( fsys->savedata, fes, 
												FREXID_STAGE_RENSHOU_SINGLE, i ) );
		}
	}

	//�f�o�b�N//////////////////////////
	//num = 100000;
	////////////////////////////////////

	OS_Printf( "stage total num = %d\n", num );
	
	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}

	//u32�ł̓��[�N���擾�ł��Ȃ��̂ŕK�v�Ȓl�ŕ␳�������Ă��܂�
	if( num > 10000 ){
		num = 10000;
	}

	*ret_wk = num;
	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * TV�F���ݒ��풆�̃|�P�����̌��݂̘A�������`�F�b�N
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 *
 * �V���O������
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdTVStageRensyouCheck( VM_MACHINE * core );
BOOL EvCmdTVStageRensyouCheck( VM_MACHINE * core )
{
	u16 before_monsno,num;
	//TV_WORK* tvwk;
	//TOPICTEMP_STAGE* ps;
	u16* ret_wk	= VMGetWork( core );
	
	//tvwk= SaveData_GetTvWork( core->fsys->savedata );
	//ps	= TVWORK_GetStageTemp(tvwk);
	//if( ps->monsno != 0 ){

	//���풆�̃|�P�����i���o�[���擾
	before_monsno = FrontierRecord_Get(	SaveData_GetFrontier(core->fsys->savedata), 
										StageScr_GetMonsNoRecordID(STAGE_TYPE_SINGLE),
										FRONTIER_RECORD_NOT_FRIEND );

	//if( ps->monsno == before_monsno ){
	
	//���݂̘A�������擾
	num = FrontierRecord_Get(	SaveData_GetFrontier(core->fsys->savedata), 
								StageScr_GetWinRecordID(STAGE_TYPE_SINGLE),
								FRONTIER_RECORD_NOT_FRIEND );

	*ret_wk = 0;
	if( num == 50 ){
		*ret_wk = 1;			//50�A�����Ă���(�A������ۑ����Ă��Ȃ��̂ňȏ�͂���)
	}

	OS_Printf( "before_monsno = %d\n", before_monsno );
	OS_Printf( "num = %d\n", num );
	return 0;
}


