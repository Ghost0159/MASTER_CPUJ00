//============================================================================================
/**
 * @file	fssc_roulette.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�F���[���b�g
 * @author	Satoshi Nohara
 * @date	07.09.06
 */
//============================================================================================
#include "common.h"
#include <nitro/code16.h> 
#include "system/pm_str.h"
#include "system\msgdata.h"						//MSGMAN_GetString
#include "system/brightness.h"					//ChangeBrightnessRequest
#include "system\wordset.h"						//WordSet_RegistPlayerName
#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "system/get_text.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/wipe.h"
#include "system/lib_pack.h"
#include "system/procsys.h"
#include "system/snd_tool.h"
#include "system/particle.h"

#include "savedata/misc.h"
#include "savedata/b_tower.h"
#include "savedata/frontier_savedata.h"
#include "msgdata\msg.naix"						//NARC_msg_??_dat

#include "field/fieldobj.h"
#include "field/scr_tool.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "gflib/strbuf_family.h"				//����

#include "../field/field_battle.h"				//BattleParam_IsWinResult
#include "../field/field_subproc.h"				//TestBattleProcData
#include "../application/roulette.h"

#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier_scr.h"
#include "frontier_tool.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"
#include "frontier_def.h"
#include "roulette_tool.h"
#include "roulette_def.h"
#include "fss_task.h"
#include "fssc_roulette_sub.h"
#include "comm_command_frontier.h"

#include "field/eventflag.h"

#include "communication/comm_system.h"
#include "communication/comm_def.h"
#include "communication/comm_info.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL FSSC_RouletteWorkAlloc( FSS_TASK * core );
BOOL FSSC_RouletteWorkInit( FSS_TASK * core );
BOOL FSSC_RouletteWorkFree( FSS_TASK * core );
BOOL FSSC_RouletteRankUpCall( FSS_TASK * core );
BOOL FSSC_RouletteCallGetResult( FSS_TASK * core );
BOOL FSSC_RouletteBattleCall( FSS_TASK * core );
BOOL FSSC_RouletteBtlBeforePokePartySet( FSS_TASK * core );
BOOL FSSC_RouletteBtlAfterPokePartySet( FSS_TASK * core );
BOOL FSSC_RouletteScrWork( FSS_TASK * core );
BOOL FSSC_RouletteLoseCheck( FSS_TASK * core );
BOOL FSSC_RouletteSendBuf( FSS_TASK * core );
BOOL FSSC_RouletteRecvBuf( FSS_TASK * core );
static BOOL WaitRouletteRecvBuf( FSS_TASK * core );
static void RouletteCallWorkSet( ROULETTE_CALL_WORK* roulette_call, ROULETTE_SCRWORK* br_scr_wk );
BOOL FSSC_RouletteTalkMsgAppear(FSS_TASK* core);
BOOL FSSC_CPWinWrite( FSS_TASK * core );
BOOL FSSC_CPWinDel( FSS_TASK * core );
BOOL FSSC_CPWrite( FSS_TASK * core );
BOOL FSSC_RouletteDecideEvNoFunc( FSS_TASK * core );
BOOL FSSC_RouletteChgItemKeepVanish(FSS_TASK* core);

//�p�[�e�B�N������
static void Particle_CallBack02( EMIT_PTR emit );
static void ParticleAddEmitterEx( FSS_TASK *core, ROULETTE_SCRWORK* br_scr_wk, u16 emit_no );

//�n�������[�N�̉������
static void RouletteFreeMemory( void *parent_work );

//static void Roulette_ResColorChange( void );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�L���b�X�����[�N�m��
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteWorkAlloc( FSS_TASK * core ) 
{
	ROULETTE_SCRWORK* wk;
	FRONTIER_EX_PARAM* ex_param;
	u16 init = FSSTGetWorkValue( core );
	u16 type = FSSTGetWorkValue( core );
	u16 pos1 = FSSTGetWorkValue( core );
	u16 pos2 = FSSTGetWorkValue( core );
	u16 pos3 = FSSTGetWorkValue( core );
	u16* work= FSSTGetWork( core );

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	wk = RouletteScr_WorkAlloc( ex_param->savedata, init, type, pos1, pos2, pos3, work );
	Frontier_SysWorkSet( core->fss->fmain, wk );

	OS_Printf( "�X�N���v�g�L���b�X�����[�N�m��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�L���b�X�����[�N������
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteWorkInit( FSS_TASK * core ) 
{
	ROULETTE_SCRWORK* br_scr_wk;
	u16 init = FSSTGetWorkValue( core );

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	RouletteScr_WorkInit( br_scr_wk, init );

	OS_Printf( "�X�N���v�g�L���b�X�����[�N������\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�L���b�X�����[�N�폜
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteWorkFree( FSS_TASK * core ) 
{
	ROULETTE_SCRWORK* br_scr_wk;
	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	RouletteScr_WorkRelease( br_scr_wk );
	OS_Printf( "�X�N���v�g�L���b�X�����[�N�J��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�L���b�X�������N�A�b�v��ʌĂяo��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteRankUpCall( FSS_TASK * core)
{
	int i;
	ROULETTE_SCRWORK* br_scr_wk;
	ROULETTE_CALL_WORK* roulette_call;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	//�I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY( roulette );
	
	//�L���b�X���v���Z�X�f�[�^
	static const PROC_DATA RouletteProc = {	
		RouletteProc_Init,
		RouletteProc_Main,
		RouletteProc_End,
		FS_OVERLAY_ID( roulette ),
	};

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	OS_Printf( "�L���b�X�������N�A�b�v��ʌĂяo��\n" );

	roulette_call = sys_AllocMemory( HEAPID_WORLD, sizeof(ROULETTE_CALL_WORK) );
	MI_CpuClear8( roulette_call, sizeof(ROULETTE_CALL_WORK) );
	roulette_call->sv			= ex_param->savedata;
	RouletteCallWorkSet( roulette_call, br_scr_wk );

    Frontier_SubProcSet( core->fss->fmain, &RouletteProc, roulette_call, FALSE, RouletteFreeMemory);
	return 1;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�L���b�X���퓬���ʎ擾���ĊJ��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteCallGetResult( FSS_TASK * core)
{
	u8 m_max;
	int i,no1,no2,poke_max,temp_buf;
	u32 exp,level,item;
	ROULETTE_SCRWORK* br_scr_wk;
	BATTLE_PARAM* param;
	POKEMON_PARAM* pp;
	POKEMON_PARAM* btl_pp;

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	param = br_scr_wk->p_work;

	//�퓬�ɏo�����|�P�����Ő퓬���ʂ̃|�C���g���v�Z(�|�P�����`�F���W�C�x���g������̂Œ���)
	br_scr_wk->point = RouletteScr_GetBtlWinPoint(	br_scr_wk, 
													param->poke_party[POKEPARTY_MINE], 
													param->poke_party[POKEPARTY_MINE_PAIR], 
													param->total_turn );

	br_scr_wk->winlose_flag = BattleParam_IsWinResult( param->win_lose_flag );
	OS_Printf( "br_scr_wk->winlose_flag = %d\n", br_scr_wk->winlose_flag );

	//�|�P�����`�F���W�p�l�����I�΂�Ă�����G���̃f�[�^����擾
	if( br_scr_wk->decide_ev_no == ROULETTE_EV_EX_POKE_CHANGE ){
		no1 = POKEPARTY_ENEMY;
		no2 = POKEPARTY_ENEMY_PAIR;
	}else{
		//�ʏ�̗���
		no1 = POKEPARTY_MINE;
		no2 = POKEPARTY_MINE_PAIR;
	}

	//�p�[�e�B���F�퓬���ʂ̃A�C�e���𔽉f
	//�Z�b�g����ʒu�́A��ʐM(0,1,2)�A�ʐM(0,1,2,3)
	RouletteScr_BattleAfterItemSet( param->poke_party[no1], 
									br_scr_wk->p_m_party, 0, 0 );

	RouletteScr_BattleAfterItemSet( param->poke_party[no1], 
									br_scr_wk->p_m_party, 1, 1 );

	if( Castle_CommCheck(br_scr_wk->type) == FALSE ){
		RouletteScr_BattleAfterItemSet( param->poke_party[no1], 
										br_scr_wk->p_m_party, 2, 2 );
	}else{
		RouletteScr_BattleAfterItemSet( param->poke_party[no2], 
										br_scr_wk->p_m_party, 0, 2);

		RouletteScr_BattleAfterItemSet( param->poke_party[no2], 
										br_scr_wk->p_m_party, 1, 3);
	}

	//�����̃|�P�����̃��x�����グ��p�l�����I�΂�Ă�����
	if( br_scr_wk->decide_ev_no == ROULETTE_EV_MINE_LV_UP ){
		m_max = Roulette_GetMinePokeNum( br_scr_wk->type, ROULETTE_FLAG_TOTAL );

		for( i=0; i < m_max ;i++ ){
			pp =  PokeParty_GetMemberPointer( br_scr_wk->p_m_party, i );
			level = PokeParaGet( pp, ID_PARA_level, NULL );
			level-=3;

			exp = PokeLevelExpGet( PokeParaGet(pp,ID_PARA_monsno,NULL), level );
			PokeParaPut( pp, ID_PARA_exp, &exp );
			PokeParaCalc( pp );
		}
		OS_Printf( "�����|�P�����̃��x�������ɖ߂��܂���\n" );
	}

#if 1	//�w�͒l�����f���Ȃ��Ή�(08.06.20)
	poke_max = PokeParty_GetPokeCount( br_scr_wk->p_m_party );
	for( i=0; i < poke_max ;i++ ){

		pp = PokeParty_GetMemberPointer(br_scr_wk->p_m_party, i );

		//�ޔ����Ă�����HP�Ȃǂŏ㏑��(�w�͒l�𔽉f�����Ȃ�����)
		temp_buf = br_scr_wk->temp_hp[i];
		PokeParaPut( pp, ID_PARA_hpmax, &temp_buf );
		PokeParaPut( pp, ID_PARA_hp, &temp_buf );

		temp_buf = br_scr_wk->temp_pow[i];
		PokeParaPut( pp, ID_PARA_pow, &temp_buf );

		temp_buf = br_scr_wk->temp_def[i];
		PokeParaPut( pp, ID_PARA_def, &temp_buf );

		temp_buf = br_scr_wk->temp_agi[i];
		PokeParaPut( pp, ID_PARA_agi, &temp_buf );

		temp_buf = br_scr_wk->temp_spepow[i];
		PokeParaPut( pp, ID_PARA_spepow, &temp_buf );

		temp_buf = br_scr_wk->temp_spedef[i];
		PokeParaPut( pp, ID_PARA_spedef, &temp_buf );
	}
#endif

	//BATTLE_PARAM�̊J��
	BattleParam_Delete( param );

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
BOOL FSSC_RouletteBattleCall( FSS_TASK * core)
{
	BATTLE_PARAM* param;
	ROULETTE_SCRWORK* br_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//�o�g���f�[�^������
	param = BtlRoulette_CreateBattleParam( br_scr_wk, ex_param );
	br_scr_wk->p_work = param;

	//�퓬�؂�ւ�
//	Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );		//�o�g���ȍĐ�
    Frontier_SubProcSet(core->fss->fmain, &TestBattleProcData, param, FALSE, NULL );
	OS_Printf( "�X�N���v�g�L���b�X���퓬�Ăяo��\n" );			//field_encount.c
	return 1;
}

static void RouletteCallWorkSet( ROULETTE_CALL_WORK* roulette_call, ROULETTE_SCRWORK* br_scr_wk )
{
	int i,poke_max;
	POKEMON_PARAM* pp;

	roulette_call->type			= br_scr_wk->type;

	//roulette_call->lap			= br_scr_wk->lap;
	roulette_call->lap			= RouletteScr_CommGetLap( br_scr_wk );	//�������񐔂��Z�b�g

	roulette_call->p_rand_pos		= &br_scr_wk->rand_pos;
	roulette_call->p_decide_ev_no	= &br_scr_wk->decide_ev_no;
	roulette_call->point			= br_scr_wk->point;
	roulette_call->rensyou			= br_scr_wk->rensyou;
	roulette_call->pair_rensyou		= br_scr_wk->pair_rensyou;

	roulette_call->p_csr_speed_level = &br_scr_wk->csr_speed_level;
//	roulette_call->level			= br_scr_wk->level;
//	roulette_call->mode			= ROULETTE_MODE_RENTAL;
	roulette_call->p_m_party		= br_scr_wk->p_m_party;
	roulette_call->p_e_party		= br_scr_wk->p_e_party;
	roulette_call->p_work			= br_scr_wk;
	br_scr_wk->weather				= 0;						//�N���A���Ă���
	roulette_call->p_weather		= &br_scr_wk->weather;
	roulette_call->random_flag		= br_scr_wk->random_flag;
	br_scr_wk->random_flag			= 0;						//�N���A���Ă���
	br_scr_wk->poke_safe_flag		= 0;						//�N���A���Ă���

#if 1	//�w�͒l�����f���Ȃ��Ή�(08.06.20)
	poke_max = PokeParty_GetPokeCount( br_scr_wk->p_m_party );
	for( i=0; i < poke_max ;i++ ){

		pp = PokeParty_GetMemberPointer(br_scr_wk->p_m_party, i );

		//�p�l���Ăяo���O�̏󋵂�ޔ�
		br_scr_wk->temp_hp[i] = PokeParaGet( pp, ID_PARA_hpmax, NULL );
		br_scr_wk->temp_pow[i] = PokeParaGet( pp, ID_PARA_pow, NULL );
		br_scr_wk->temp_def[i] = PokeParaGet( pp, ID_PARA_def, NULL );
		br_scr_wk->temp_agi[i] = PokeParaGet( pp, ID_PARA_agi, NULL );
		br_scr_wk->temp_spepow[i] = PokeParaGet( pp, ID_PARA_spepow, NULL );
		br_scr_wk->temp_spedef[i] = PokeParaGet( pp, ID_PARA_spedef, NULL );
	}
#endif

	return;
}

//�L���b�X����ʂœn�������[�N�̉������
static void RouletteFreeMemory( void *parent_work )
{
	int i;
	ROULETTE_CALL_WORK* roulette_call = parent_work;

	//���ʂ��擾
	RouletteScr_GetResult( roulette_call->p_work, parent_work );

	//ROULETTE_CALL_WORK�̍폜
	sys_FreeMemoryEz( parent_work );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�o�g���O��POKEPARTY�̃Z�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteBtlBeforePokePartySet( FSS_TASK * core )
{
	ROULETTE_SCRWORK* br_scr_wk;
	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	RouletteScr_BtlBeforePartySet( br_scr_wk );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�o�g�����POKEPARTY�̃Z�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteBtlAfterPokePartySet( FSS_TASK * core )
{
	ROULETTE_SCRWORK* br_scr_wk;
	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	RouletteScr_BtlAfterPartySet( br_scr_wk );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FROULETTE_SCR_WORK�̃Z�b�g�A�Q�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteScrWork( FSS_TASK * core )
{
	STRBUF* strbuf;
	POKEMON_PARAM* pp;
	POKEPARTY* party;
	ROULETTE_SCRWORK* br_scr_wk;
	u8 m_max,e_max,color;
	u32 type_tbl[9][2];
	u32 type,type_num;
	u16 offset_y;
	int i,j,flag;
	FRONTIER_EX_PARAM* ex_param;
	FMAP_PTR fmap;
	MYSTATUS* my;

	u8 code		= FSSTGetU8( core );
	u8 param1	= FSSTGetWorkValue( core );
	u8 param2	= FSSTGetWorkValue( core );
	u16* work	= FSSTGetWork( core );

	br_scr_wk	= Frontier_SysWorkGet( core->fss->fmain );
	ex_param	= Frontier_ExParamGet( core->fss->fmain );
	fmap		= FSS_GetFMapAdrs( core->fss );

	switch( code ){

	//�^�C�v(�V���O���A�_�u���A�}���`�Awifi�}���`)�̃Z�b�g
	case FR_ID_SET_TYPE:
		br_scr_wk->type = param1;
		break;

	//ret_work�̎擾
	case FR_ID_GET_RET_WORK:
		*work = br_scr_wk->ret_work[param1];
		break;

	//�A�����̎擾
	case FR_ID_GET_RENSYOU:
		*work = br_scr_wk->rensyou;
		break;

	//�A�����̃C���N�������g
	case FR_ID_INC_RENSYOU:
		if( br_scr_wk->rensyou < ROULETTE_RENSYOU_MAX ){
			br_scr_wk->rensyou++;
		}
		break;

	//���Z�b�g���ă^�C�g���ɖ߂�
	case FR_ID_SYSTEM_RESET:
		OS_ResetSystem( 0 );
		break;

	//------------------------------------
	//	ROULETTEDATA�ɃA�N�Z�X
	//------------------------------------
	//�Z�[�u�f�[�^���L�����ǂ����Ԃ�
	case FR_ID_IS_SAVE_DATA_ENABLE:
		*work = ROULETTEDATA_GetSaveFlag( br_scr_wk->roulette_savedata );
		break;

	//�x�ގ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
	case FR_ID_SAVE_REST_PLAY_DATA:
		RouletteScr_SaveRestPlayData( br_scr_wk, FR_MODE_REST );
		break;

	//------------------------------------
	//	
	//------------------------------------
	//�Ⴆ��BP���擾(���[���b�g�p�l����BP�Q�b�g���I�΂ꂽ��Ɏg�p)
	case FR_ID_GET_PANEL_BP:
		*work = Roulette_GetPanelBP( br_scr_wk, br_scr_wk->decide_ev_no );
		break;

	//���E���h�����C���N�������g
	case FR_ID_INC_ROUND:
		*work = RouletteScr_IncRound( br_scr_wk );
		break;

	//�A�C�e���i���o�[�擾(���[���b�g�p�l���ŃA�C�e���Q�b�g���I�΂ꂽ��Ɏg�p)
	case FR_ID_GET_ITEM:
		if( param1 == 0 ){
			pp = PokeParty_GetMemberPointer(br_scr_wk->p_m_party, 0 );
		}else{
			pp = PokeParty_GetMemberPointer(br_scr_wk->p_e_party, 0 );
		}
		*work = PokeParaGet( pp, ID_PARA_item, NULL );
		break;

	//���肵���C�x���g�i���o�[���擾
	case FR_ID_GET_DECIDE_EV_NO:
		*work = br_scr_wk->decide_ev_no;
		break;

	//�ۑ����Ă������A�C�e���i���o�[���Z�b�g����
	case FR_ID_SET_TEMP_ITEM:
		party = SaveData_GetTemotiPokemon( ex_param->savedata );	//�莝���p�[�e�B�擾
		for( i=0; i < ROULETTE_ENTRY_POKE_MAX ;i++ ){
			pp = PokeParty_GetMemberPointer( party, br_scr_wk->mine_poke_pos[i] );
			PokeParaPut( pp, ID_PARA_item, &br_scr_wk->itemno[i] );
		}
		break;

	//���񐔂��擾
	case FR_ID_GET_LAP:
		*work = RouletteScr_CommGetLap( br_scr_wk );
		break;

	//�G�g���[�i�[��OBJ�R�[�h���擾
	case FR_ID_GET_TR_OBJ_CODE:
		*work = RouletteScr_GetEnemyObjCode( br_scr_wk, param1 );
		break;

	//�s�킵�����̏���
	case FR_ID_SET_LOSE:
		RouletteScr_SetLose( br_scr_wk );
		break;

	//7�A��(�N���A)�������̏���
	case FR_ID_SET_CLEAR:
		RouletteScr_SetClear( br_scr_wk );
		break;

	//���E���h�����擾
	case FR_ID_GET_ROUND:
		*work = RouletteScr_GetRound( br_scr_wk );
		break;

	//���^�C���t���O���擾
	case FR_ID_GET_RETIRE_FLAG:
		OS_Printf( "br_scr_wk->pair_retire_flag = %d\n", br_scr_wk->pair_retire_flag );
		*work = br_scr_wk->pair_retire_flag;
		break;

	//�A�C�R���`�F���W(�|�P�����`�F���W�C�x���g)
	case FR_ID_ICON_CHANGE:
		if( br_scr_wk->decide_ev_no == ROULETTE_EV_EX_POKE_CHANGE ){
			m_max = Roulette_GetMinePokeNum( br_scr_wk->type, ROULETTE_FLAG_TOTAL );
			e_max = Roulette_GetEnemyPokeNum( br_scr_wk->type, ROULETTE_FLAG_TOTAL );

			{
				VecFx32 m_vec;
				VecFx32 e_vec;
				CLACT_WORK_PTR p_m_clact;			//�Z���A�N�^�[���[�N�|�C���^
				CLACT_WORK_PTR p_e_clact;			//�Z���A�N�^�[���[�N�|�C���^

				for( i=0; i < m_max ;i++ ){	
					//�|�P�����A�C�R���̍��W�擾
					p_m_clact = br_scr_wk->m_pokeicon[i]->act;
					m_vec = *( CLACT_GetMatrix( (CONST_CLACT_WORK_PTR)p_m_clact ) );

					p_e_clact = br_scr_wk->e_pokeicon[i]->act;
					e_vec = *( CLACT_GetMatrix( (CONST_CLACT_WORK_PTR)p_e_clact ) );

					//���W�𔽑΂ɂ���
					CLACT_SetMatrix( p_m_clact, &e_vec );
					CLACT_SetMatrix( p_e_clact, &m_vec );

					//�A�C�e�������Ă��邩�A�C�R���̍��W�擾
					p_m_clact = br_scr_wk->m_itemkeep[i]->act;
					m_vec = *( CLACT_GetMatrix( (CONST_CLACT_WORK_PTR)p_m_clact ) );

					p_e_clact = br_scr_wk->e_itemkeep[i]->act;
					e_vec = *( CLACT_GetMatrix( (CONST_CLACT_WORK_PTR)p_e_clact ) );

					//���W�𔽑΂ɂ���
					CLACT_SetMatrix( p_m_clact, &e_vec );
					CLACT_SetMatrix( p_e_clact, &m_vec );
				}
			}

		}
		break;

	//�ʐM�ݒ�
	case FR_ID_COMM_COMMAND_INITIALIZE:
		CommCommandFrontierInitialize( br_scr_wk );
		break;

	//�ʐM�^�C�v���`�F�b�N
	case FR_ID_CHECK_COMM_TYPE:
		*work = Roulette_CommCheck( br_scr_wk->type );
		break;

	//�^�C�v�擾
	case FR_ID_GET_TYPE:
		*work = br_scr_wk->type;
		break;

	//�|�P�����A�C�R���̃��\�[�X�Z�b�g
	case FR_ID_RESOURCE_SET_POKE_ICON:
		FSSC_Sub_PokeIconCommonResourceSet( fmap );
		break;

	//�|�P�����A�C�R���̃��\�[�X�폜
	case FR_ID_RESOURCE_FREE_POKE_ICON:
		FSSC_Sub_PokeIconCommonResourceFree( fmap );
		break;

	//�|�P�����A�C�R���ǉ�
	case FR_ID_ADD_POKE_ICON:
		RouletteScr_AddPokeIcon( br_scr_wk, fmap, param1 );

		m_max = Roulette_GetMinePokeNum( br_scr_wk->type, ROULETTE_FLAG_TOTAL );
		e_max = Roulette_GetEnemyPokeNum( br_scr_wk->type, ROULETTE_FLAG_TOTAL );

		if( param1 == 0 ){
			for( i=0; i < m_max ;i++ ){	
				CLACT_BGPriorityChg( br_scr_wk->m_pokeicon[i]->act, 2 );
			}
		}else{
			for( i=0; i < e_max ;i++ ){	
				CLACT_BGPriorityChg( br_scr_wk->e_pokeicon[i]->act, 2 );
			}
		}
		break;

	//�|�P�����A�C�R���폜
	case FR_ID_DEL_POKE_ICON:
		RouletteScr_DelPokeIcon( br_scr_wk, fmap, param1 );
		break;

	//�A�C�e�������Ă��邩�A�C�R���̃��\�[�X�Z�b�g
	case FR_ID_RESOURCE_SET_ITEMKEEP_ICON:
		FSSC_Sub_ItemIconLoad( fmap );										//
		break;

	//�A�C�e�������Ă��邩�A�C�R���̃��\�[�X�폜
	case FR_ID_RESOURCE_FREE_ITEMKEEP_ICON:
		FSSC_Sub_ItemIconFree( fmap );
		break;

	//�A�C�e�������Ă��邩�A�C�R���ǉ�
	case FR_ID_ADD_ITEMKEEP_ICON:
		RouletteScr_AddItemKeepIcon( br_scr_wk, fmap, param1 );
		break;

	//�A�C�e�������Ă��邩�A�C�R���폜
	case FR_ID_DEL_ITEMKEEP_ICON:
		RouletteScr_DelItemKeepIcon( br_scr_wk, fmap, param1 );
		break;

	//�}�b�v�Ƀp�l���Z�b�g
	case FR_ID_SET_PANEL_BG:
		RouletteScr_BGPanelChange( br_scr_wk, fmap );
		break;

	//�w�i��ʐM�p�ɐ؂�ւ�
	case FR_ID_SET_MAIN_BG:
		if( Roulette_CommCheck(br_scr_wk->type) == TRUE ){
			RouletteScr_MainBGChange( br_scr_wk, fmap );
		}
		break;

	//�g���[�i�[������
	case FR_ID_TR_NAME:
		//�����ƃy�A�̖��O�A�G�g���[�i�[�̎�������
		color = Roulette_GetEvPanelColor( br_scr_wk->decide_ev_no );

		if( color == ROULETTE_PANEL_COLOR_RED ){
			WORDSET_RegisterTowerTrainerName(	core->fss->wordset, param1, 
								br_scr_wk->tr_index[RouletteScr_GetTrIndex(br_scr_wk,param2)] );
		}else{
			if( Roulette_CommCheck(br_scr_wk->type) == FALSE ){
				my = SaveData_GetMyStatus( ex_param->savedata);
			}else{
				my = CommInfoGetMyStatus( param2 );
			}

			WORDSET_RegisterPlayerName( core->fss->wordset, param1, my );
		}
		break;

	//�G�~�b�^����
	case FR_ID_ADD_EMITTER:
		br_scr_wk->reverce_flag = param2;						//���]�t���O�i�[
		ParticleAddEmitterEx( core, br_scr_wk, param1 );
		break;

	//�퓬�O��b�̂��߂�B_TOWER_PARTNER_DATA�̃Z�b�g
	case FR_ID_SET_B_TOWER_PARTNER_DATA:
		FSRomBattleTowerTrainerDataMake2( &(br_scr_wk->tr_data[0]), 
					br_scr_wk->tr_index[br_scr_wk->round], HEAPID_WORLD, ARC_PL_BTD_TR );
		FSRomBattleTowerTrainerDataMake2( &(br_scr_wk->tr_data[1]),
					br_scr_wk->tr_index[br_scr_wk->round+ROULETTE_LAP_ENEMY_MAX], HEAPID_WORLD,
					ARC_PL_BTD_TR );
		break;

	//(����)�|�P�����A�C�R���̃o�j�b�V������
	case FR_ID_MINE_CHG_POKE_ICON_VANISH:
		RouletteScr_MineChgPokeIconVanish( br_scr_wk, fmap, param1, param2 );
		break;

	//(�G)�|�P�����A�C�R���̃o�j�b�V������
	case FR_ID_ENEMY_CHG_POKE_ICON_VANISH:
		RouletteScr_EnemyChgPokeIconVanish( br_scr_wk, fmap, param1, param2 );
		break;

	//�g���p���b�g����
	case FR_ID_EFF_PAL_CHG:
#if 0
		//�ȉ��̊֐��́A�g���p���b�g���Ή����Ă��Ȃ��̂Ŏg���Ȃ�
						
		//�J���[�P��
		//SoftFadePfd( fmap->pfd, FADE_MAIN_OBJ, 0, 256, 8, 0x0000 );
		//SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 0, 256, 8, 0x0000 );				//

		//�p���b�g�P��
		//ColorConceChangePfd( fmap->pfd, FADE_MAIN_OBJ, 16, 8, 0x0000 );
		//ColorConceChangePfd( fmap->pfd, FADE_MAIN_BG_EX3, 16, 8, 0x0000 );
#else
		RouletteScr_ResColorChange( param1, param2 );
#endif
		break;

	//OBJ�p���b�g����
	case FR_ID_OBJ_PAL_CHG:

		//OBJ�̃p���b�g�̂ǂ����g���Ă��邩�m�F
		//�����v						= 1
		//������						= 1
		//�A�C�e�������Ă���A�C�R��	= 1
		//�ʐM�A�C�R��					= 1
		//�|�P�����A�C�R��(3�{)			= 3
		//								= 7�{�͌��̂܂�
		
		//���ɖ߂�(���́AOBJ�p���b�g�S�Ă�Ώۂɂ��Ă���
		if( param1 == 0 ){
			SoftFadePfd( fmap->pfd, FADE_MAIN_OBJ, 0, 16*16, param1, 0x0000 );

		//�Â�����(���́A����OBJ�̃p���b�g1�{�݂̂�Ώۂɂ��Ă���)
		}else{
			{
				u32 palno;
				FSS_ACTOR_WORK *fss_actor;
				CLACT_WORK_PTR act;

				fss_actor = Frontier_ActorWorkSearch(core->fss->fmain, param2);
				palno = WF2DMAP_OBJDrawWkPaletteNoGet(fss_actor->drawwk);
				//OS_Printf( "�g�p PalNo = %d\n", palno );

				SoftFadePfd( fmap->pfd, FADE_MAIN_OBJ, (palno * 16), 16, param1, 0x0000 );
			}
		}
		break;

	//�Ⴆ��o�g���|�C���g���擾
	case FR_ID_GET_BP_POINT:
		*work = RouletteScr_GetAddBtlPoint( br_scr_wk );
		break;

	//�p�l��BG���\���ɂ���
	case FR_ID_PANEL_BG_OFF:
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��
		break;

	//�N���C�x���g�̌��ʂ𔭊����Ȃ��������`�F�b�N
	case FR_ID_GET_POKE_SAFE_FLAG:
		*work = br_scr_wk->poke_safe_flag;
		break;

	//�u���[���o�ꂩ�`�F�b�N(0=�Ȃ��A1=���o��A2=2���)
	case FR_ID_CHECK_BRAIN:
		*work = 0;

		//�V���O���̂�
		if( br_scr_wk->type == ROULETTE_TYPE_SINGLE ){

			if( (br_scr_wk->rensyou+1) == ROULETTE_LEADER_SET_1ST ){
				*work = 1;
			}else if( (br_scr_wk->rensyou+1) == ROULETTE_LEADER_SET_2ND ){
				*work = 2;
			}
		}
		break;

	//�G�|�P��������x�ɐ���
	case FR_ID_ENEMY_POKE_CREATE:
		//B_TOWER_POKEMON enemy_poke[ROULETTE_ENEMY_POKE_MAX];

		//�G�|�P��������x�ɐ���
		Frontier_PokemonParamCreateAll(	br_scr_wk->enemy_poke, br_scr_wk->enemy_poke_index, 
										br_scr_wk->enemy_pow_rnd, br_scr_wk->enemy_personal_rnd, 
										NULL, 
										ROULETTE_ENEMY_POKE_MAX, HEAPID_WORLD, ARC_PL_BTD_PM );
		break;

	//�G�|�P��������x�ɐ���2
	case FR_ID_ENEMY_POKE_CREATE_2:
		//�G�p�[�e�B�̃Z�b�g
		Roulette_EnemyPartySet( br_scr_wk );
		break;

	//�u���[���o�ꃁ�b�Z�[�W�\��������
	case FR_ID_BRAIN_APPEAR_MSG_CHK:
		*work = br_scr_wk->brain_appear_msg_flag;
		br_scr_wk->brain_appear_msg_flag = 1;
		break;

	//���V�[�u�J�E���^���N���A(�ی�����)
	case FR_ID_RECIEVE_COUNT_CLEAR:
		br_scr_wk->recieve_count = 0;
		break;

	};

	return 0;
}

//--------------------------------------------------------------
/**
 * @brief   �G�~�b�^�𐶐�����
 *
 * @param   core		
 */
//--------------------------------------------------------------
static void ParticleAddEmitterEx( FSS_TASK *core, ROULETTE_SCRWORK* br_scr_wk, u16 emit_no )
{
	FSS_PTR fss = core->fss;
	FMAP_PTR fmap = FSS_GetFMapAdrs( core->fss );
	PTC_PTR ptc;

	ptc = FRParticle_PTCPtrGet( fmap->frp, SPAWORK_0 );		//workID�Œ�Ȃ̂Œ��ӁI
	Particle_CreateEmitterCallback( ptc, emit_no, Particle_CallBack02, br_scr_wk );
	return;
}

//����g�p����̂́AX�������Ǝv����
//���]�����鎞�̂݃I�t�Z�b�g�𑫂�����
static const VecFx32 vec_tbl[] = {							//EMIT�i���o�[�Ƒ΂ɂȂ�e�[�u��
	{ FX32_ONE*-1, 0, 0 },			//0
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//5
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//10
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//15
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//20
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//25
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//30
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//35
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },
	{ FX32_ONE*-1, 0, 0 },			//39
};

static void Particle_CallBack02( EMIT_PTR emit )
{
	VecFx32 vec;
	VecFx16 axis;
	ROULETTE_SCRWORK* br_scr_wk = Particle_GetTempPtr();
	const VecFx32 vec_tbl2[] = { FX32_ONE*-1, 0, 0 };

	//SplSub_GetEmitterPosition( emit, &vec );
	//OS_Printf( "vec.x = %d\n", vec.x );
	//OS_Printf( "vec.y = %d\n", vec.y );
	//OS_Printf( "vec.z = %d\n", vec.z );

	//�Ō�ɐ��������G�~�b�^�̃|�C���^���擾
	//emit = Particle_TempEmitterPtrGet( ptc );
	//OS_Printf( "emit = %d\n", emit );

	//���]�t���O�`�F�b�N
	if( br_scr_wk->reverce_flag == 1 ){
		//�G�~�b�^�̌������擾
		SplSub_GetEmitterAxis( emit, &axis );
		//OS_Printf( "axis.x = %d\n", axis.x );
		//OS_Printf( "axis.y = %d\n", axis.y );
		//OS_Printf( "axis.z = %d\n", axis.z );

		//�G�~�b�^�̌����𔽓]������
		axis.x *= -1;
		//VEC_Fx16Set( &axis, 0, 0, 0 );
		//VEC_Fx16Set( &axis, 3776, 2112, 0 );
		//VEC_Fx16Set( &axis, -6000, -2200, 0 );
		SPL_SetEmitterAxis( emit, &axis );

		//�G�~�b�^�̈ʒu�p�����[�^���Z�b�g����B�I�t�Z�b�g�l�͏�ɔ��f�����B
		//SPL_SetEmitterPosition( emit, &vec_tbl[0] );
		SPL_SetEmitterPosition( emit, vec_tbl2 );
	}

	//SplSub_GetEmitterPosition( emit, &vec );
	//OS_Printf( "vec.x = %d\n", vec.x );
	//OS_Printf( "vec.y = %d\n", vec.y );
	//OS_Printf( "vec.z = %d\n", vec.z );
	return;
}

//--------------------------------------------------------------
/**
 * �g���[�i�[�s�k�`�F�b�N
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteLoseCheck( FSS_TASK * core )
{
	ROULETTE_SCRWORK* br_scr_wk;
	u16* work = FSSTGetWork( core );

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*work = br_scr_wk->winlose_flag;
	OS_Printf( "�s�k�`�F�b�N*work TRUE���� FALSE�s�k = %d\n", *work );
	return 0;
}


//==============================================================================================
//
//	���M�A��M�̌Ăяo��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * ���M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteSendBuf( FSS_TASK * core )
{
	ROULETTE_SCRWORK* br_scr_wk;
	u16 type	= FSSTGetWorkValue( core );
	u16 param	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*ret_wk = RouletteScr_CommSetSendBuf( br_scr_wk, type, param );
	return 1;
}

//--------------------------------------------------------------
/**
 * ��M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteRecvBuf( FSS_TASK * core )
{
	u16 wk_id = FSSTGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	FSST_SetWait( core, WaitRouletteRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitRouletteRecvBuf( FSS_TASK * core )
{
	ROULETTE_SCRWORK* br_scr_wk;
	u16 type = FSS_GetEventWorkValue( core, core->reg[0] );		//���ӁI

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//OS_Printf( "br_scr_wk->recieve_count = %d\n", br_scr_wk->recieve_count );
	if( br_scr_wk->recieve_count >= ROULETTE_COMM_PLAYER_NUM ){
		br_scr_wk->recieve_count = 0;
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g�����[���b�g�ΐ�O���b�Z�[�W��p�\��
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteTalkMsgAppear(FSS_TASK* core)
{
	u16	*msg;
	ROULETTE_SCRWORK*	br_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );
	u16	tr_idx = FSSTGetU8(core);	//��l�ڂ���l�ڂ��H

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	if(br_scr_wk == NULL){
		return 0;
	}

	msg = br_scr_wk->tr_data[tr_idx].bt_trd.appear_word;

	FrontierTalkMsgSub( core, msg );
	return 1;
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g�����[���b�g�Ō��肵���C�x���g�������s��
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteDecideEvNoFunc(FSS_TASK* core)
{
	ROULETTE_SCRWORK* br_scr_wk;
	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	Roulette_EvSet( br_scr_wk, br_scr_wk->decide_ev_no );
	return 1;
}

//--------------------------------------------------------------
/**
 *	@brief	�A�C�e�������Ă���A�C�R���̃o�j�b�V������(FSSC_RouletteScrWork�ň���������Ȃ�����)
 */
//--------------------------------------------------------------
BOOL FSSC_RouletteChgItemKeepVanish(FSS_TASK* core)
{
	ROULETTE_SCRWORK* br_scr_wk;
	FMAP_PTR fmap	= FSS_GetFMapAdrs( core->fss );
	u16 param1		= FSSTGetWorkValue( core );				//0=�����A1=�G
	u16 param2		= FSSTGetWorkValue( core );				//���Ԗڂ�
	u16 param3		= FSSTGetWorkValue( core );				//1=�\���A0=��\��

	br_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	RouletteScr_ChgItemKeepIconVanish( br_scr_wk, fmap, param1, param2, param3 );
	return 0;
}


