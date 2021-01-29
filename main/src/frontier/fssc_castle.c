//============================================================================================
/**
 * @file	fssc_castle.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�F�L���b�X��
 * @author	Satoshi Nohara
 * @date	07.07.04
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
#include "../application/castle.h"

#include "frontier_types.h"
#include "frontier_map.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "frontier_tool.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"
#include "frontier_def.h"
#include "castle_tool.h"
#include "castle_def.h"
#include "fss_task.h"
#include "fssc_castle_sub.h"
#include "comm_command_frontier.h"

#include "field/eventflag.h"

#include "communication/comm_system.h"
#include "communication/comm_def.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL FSSC_CastleWorkAlloc( FSS_TASK * core );
BOOL FSSC_CastleWorkInit( FSS_TASK * core );
BOOL FSSC_CastleWorkFree( FSS_TASK * core );
BOOL FSSC_CastleMineCall( FSS_TASK * core );
BOOL FSSC_CastleRankUpCall( FSS_TASK * core );
BOOL FSSC_CastleCallGetResult( FSS_TASK * core );
BOOL FSSC_CastleBattleCall( FSS_TASK * core );
BOOL FSSC_CastleEnemyCall( FSS_TASK * core );
BOOL FSSC_CastleBtlBeforePokePartySet( FSS_TASK * core );
BOOL FSSC_CastleBtlAfterPokePartySet( FSS_TASK * core );
BOOL FSSC_CastleScrWork( FSS_TASK * core );
BOOL FSSC_CastleLoseCheck( FSS_TASK * core );
BOOL FSSC_CastleSendBuf( FSS_TASK * core );
BOOL FSSC_CastleRecvBuf( FSS_TASK * core );
static BOOL WaitCastleRecvBuf( FSS_TASK * core );
static void CastleCallWorkSet( CASTLE_CALL_WORK* castle_call, CASTLE_SCRWORK* bc_scr_wk );
BOOL FSSC_CastleParentCheckWait( FSS_TASK * core );
static BOOL WaitCastleParentCheckWait( FSS_TASK * core );
BOOL FSSC_CPWinWrite( FSS_TASK * core );
BOOL FSSC_CPWinDel( FSS_TASK * core );
BOOL FSSC_CPWrite( FSS_TASK * core );
BOOL FSSC_CastleGetCP( FSS_TASK * core );
BOOL FSSC_CastleSubCP( FSS_TASK * core );
BOOL FSSC_CastleAddCP( FSS_TASK * core );
BOOL FSSC_CastleTalkMsgAppear(FSS_TASK* core);

//�n�������[�N�̉������
static void CastleFreeMemory( void *parent_work );


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
BOOL FSSC_CastleWorkAlloc( FSS_TASK * core ) 
{
	CASTLE_SCRWORK* wk;
	FRONTIER_EX_PARAM* ex_param;
	u16 init = FSSTGetWorkValue( core );
	u16 type = FSSTGetWorkValue( core );
	u16 pos1 = FSSTGetWorkValue( core );
	u16 pos2 = FSSTGetWorkValue( core );
	u16 pos3 = FSSTGetWorkValue( core );
	u16* work= FSSTGetWork( core );

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	wk = CastleScr_WorkAlloc( ex_param->savedata, init, type, pos1, pos2, pos3, work );
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
BOOL FSSC_CastleWorkInit( FSS_TASK * core ) 
{
	CASTLE_SCRWORK* bc_scr_wk;
	u16 init = FSSTGetWorkValue( core );

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	CastleScr_WorkInit( bc_scr_wk, init );

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
BOOL FSSC_CastleWorkFree( FSS_TASK * core ) 
{
	CASTLE_SCRWORK* bc_scr_wk;
	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	CastleScr_WorkRelease( bc_scr_wk );
	OS_Printf( "�X�N���v�g�L���b�X�����[�N�J��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�L���b�X���莝����ʌĂяo��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleMineCall( FSS_TASK * core)
{
	int i;
	CASTLE_SCRWORK* bc_scr_wk;
	CASTLE_CALL_WORK* castle_call;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	//�I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY( castle );
	
	//�L���b�X���v���Z�X�f�[�^
	static const PROC_DATA CastleProc = {	
		CastleMineProc_Init,
		CastleMineProc_Main,
		CastleMineProc_End,
		FS_OVERLAY_ID( castle ),
	};

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	OS_Printf( "�L���b�X���莝����ʌĂяo��\n" );

	castle_call = sys_AllocMemory( HEAPID_WORLD, sizeof(CASTLE_CALL_WORK) );
	MI_CpuClear8( castle_call, sizeof(CASTLE_CALL_WORK) );
	castle_call->sv			= ex_param->savedata;
	CastleCallWorkSet( castle_call, bc_scr_wk );

    Frontier_SubProcSet( core->fss->fmain, &CastleProc, castle_call, FALSE, CastleFreeMemory);
	return 1;
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
BOOL FSSC_CastleRankUpCall( FSS_TASK * core)
{
	int i;
	CASTLE_SCRWORK* bc_scr_wk;
	CASTLE_CALL_WORK* castle_call;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	//�I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY( castle );
	
	//�L���b�X���v���Z�X�f�[�^
	static const PROC_DATA CastleProc = {	
		NULL,
		NULL,
		NULL,
		FS_OVERLAY_ID( castle ),
	};

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	OS_Printf( "�L���b�X�������N�A�b�v��ʌĂяo��\n" );

	castle_call = sys_AllocMemory( HEAPID_WORLD, sizeof(CASTLE_CALL_WORK) );
	MI_CpuClear8( castle_call, sizeof(CASTLE_CALL_WORK) );
	castle_call->sv			= ex_param->savedata;
	CastleCallWorkSet( castle_call, bc_scr_wk );

    Frontier_SubProcSet( core->fss->fmain, &CastleProc, castle_call, FALSE, CastleFreeMemory);
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
BOOL FSSC_CastleCallGetResult( FSS_TASK * core)
{
	int i;
	CASTLE_SCRWORK* bc_scr_wk;
	BATTLE_PARAM* param;
	POKEMON_PARAM* pp;

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	param = bc_scr_wk->p_work;

	//�p�[�e�B���F�퓬���ʂ𔽉f
	
	//�Z�b�g����ʒu�́A��ʐM(0,1,2)�A�ʐM(0,1,2,3)
	pp = PokeParty_GetMemberPointer( param->poke_party[POKEPARTY_MINE], 0 );
	PokeParty_SetMemberData( bc_scr_wk->p_m_party, 0, pp );

	pp = PokeParty_GetMemberPointer( param->poke_party[POKEPARTY_MINE], 1 );
	PokeParty_SetMemberData( bc_scr_wk->p_m_party, 1, pp );

	if( Castle_CommCheck(bc_scr_wk->type) == FALSE ){

		pp = PokeParty_GetMemberPointer( param->poke_party[POKEPARTY_MINE], 2 );
		PokeParty_SetMemberData( bc_scr_wk->p_m_party, 2, pp );

	}else{
		pp = PokeParty_GetMemberPointer( param->poke_party[POKEPARTY_MINE_PAIR], 0 );
		PokeParty_SetMemberData( bc_scr_wk->p_m_party, 2, pp );

		pp = PokeParty_GetMemberPointer( param->poke_party[POKEPARTY_MINE_PAIR], 1 );
		PokeParty_SetMemberData( bc_scr_wk->p_m_party, 3, pp );
	}

	bc_scr_wk->winlose_flag = BattleParam_IsWinResult( param->win_lose_flag );
	OS_Printf( "bc_scr_wk->winlose_flag = %d\n", bc_scr_wk->winlose_flag );

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
BOOL FSSC_CastleBattleCall( FSS_TASK * core)
{
	BATTLE_PARAM* param;
	CASTLE_SCRWORK* bc_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//�o�g���f�[�^������
	param = BtlCastle_CreateBattleParam( bc_scr_wk, ex_param );
	bc_scr_wk->p_work = param;

	//�퓬�؂�ւ�
//	Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );		//�o�g���ȍĐ�
    Frontier_SubProcSet(core->fss->fmain, &TestBattleProcData, param, FALSE, NULL );
	OS_Printf( "�X�N���v�g�L���b�X���퓬�Ăяo��\n" );			//field_encount.c
	return 1;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�L���b�X���G�g���[�i�[��ʌĂяo��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleEnemyCall( FSS_TASK * core )
{
	int i;
	CASTLE_SCRWORK* bc_scr_wk;
	CASTLE_CALL_WORK* castle_call;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	//�I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY( castle );
	
	//�L���b�X���v���Z�X�f�[�^
	static const PROC_DATA CastleProc = {	
		CastleEnemyProc_Init,
		CastleEnemyProc_Main,
		CastleEnemyProc_End,
		FS_OVERLAY_ID( castle ),
	};

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	OS_Printf( "�L���b�X���G�g���[�i�[��ʌĂяo��\n" );

	castle_call = sys_AllocMemory( HEAPID_WORLD, sizeof(CASTLE_CALL_WORK) );
	MI_CpuClear8( castle_call, sizeof(CASTLE_CALL_WORK) );
	castle_call->sv			= ex_param->savedata;
	CastleCallWorkSet( castle_call, bc_scr_wk );

    Frontier_SubProcSet( core->fss->fmain, &CastleProc, castle_call, FALSE, CastleFreeMemory);
	return 1;
}

static void CastleCallWorkSet( CASTLE_CALL_WORK* castle_call, CASTLE_SCRWORK* bc_scr_wk )
{
	int i;

	castle_call->type			= bc_scr_wk->type;
//	castle_call->level			= bc_scr_wk->level;
//	castle_call->mode			= CASTLE_MODE_RENTAL;
	castle_call->p_m_party		= bc_scr_wk->p_m_party;
	castle_call->p_e_party		= bc_scr_wk->p_e_party;
	castle_call->p_work			= bc_scr_wk;
	castle_call->pair_cp		= bc_scr_wk->pair_cp;

	//��񂪌��J���ꂽ���t���O
	for( i=0; i < CASTLE_ENEMY_POKE_MAX ;i++ ){
		castle_call->enemy_temoti_flag[i]	= bc_scr_wk->enemy_temoti_flag[i];
		castle_call->enemy_level_flag[i]	= bc_scr_wk->enemy_level_flag[i];
		castle_call->enemy_tuyosa_flag[i]	= bc_scr_wk->enemy_tuyosa_flag[i];
		castle_call->enemy_waza_flag[i]		= bc_scr_wk->enemy_waza_flag[i];
	}

	return;
}


//�L���b�X����ʂœn�������[�N�̉������
static void CastleFreeMemory( void *parent_work )
{
	int i;
	CASTLE_CALL_WORK* castle_call = parent_work;

	//���ʂ��擾
	CastleScr_GetResult( castle_call->p_work, parent_work );

	//CASTLE_CALL_WORK�̍폜
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
BOOL FSSC_CastleBtlBeforePokePartySet( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	CastleScr_BtlBeforePartySet( bc_scr_wk );
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
BOOL FSSC_CastleBtlAfterPokePartySet( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	CastleScr_BtlAfterPartySet( bc_scr_wk );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FCASTLE_SCR_WORK�̃Z�b�g�A�Q�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleScrWork( FSS_TASK * core )
{
	STRBUF* strbuf;
	POKEMON_PARAM* pp;
	POKEPARTY* party;
	CASTLE_SCRWORK* bc_scr_wk;
	u8 e_max;
	u32 type_tbl[9][2];
	u32 type,type_num;
	int i,j,flag;
	FRONTIER_EX_PARAM* ex_param;
	FMAP_PTR fmap;
	u8 buf8[4];

	u8 code		= FSSTGetU8( core );
	u8 param1	= FSSTGetU8( core );
	u8 param2	= FSSTGetU8( core );
	u16* work	= FSSTGetWork( core );

	bc_scr_wk	=  Frontier_SysWorkGet( core->fss->fmain );
	ex_param	= Frontier_ExParamGet( core->fss->fmain );

	switch( code ){

	//�^�C�v(�V���O���A�_�u���A�}���`�Awifi�}���`)�̃Z�b�g
	case FC_ID_SET_TYPE:
		bc_scr_wk->type = param1;
		break;

	//ret_work�̎擾
	case FC_ID_GET_RET_WORK:
		*work = bc_scr_wk->ret_work[param1];
		break;

	//�A�����̎擾
	case FC_ID_GET_RENSYOU:
		*work = bc_scr_wk->rensyou;
		break;

	//�A�����̃C���N�������g
	case FC_ID_INC_RENSYOU:
		if( bc_scr_wk->rensyou < CASTLE_RENSYOU_MAX ){
			bc_scr_wk->rensyou++;
		}
		break;

	//���Z�b�g���ă^�C�g���ɖ߂�
	case FC_ID_SYSTEM_RESET:
		OS_ResetSystem( 0 );
		break;

	//------------------------------------
	//	CASTLEDATA�ɃA�N�Z�X
	//------------------------------------
	//�Z�[�u�f�[�^���L�����ǂ����Ԃ�
	case FC_ID_IS_SAVE_DATA_ENABLE:
		*work = CASTLEDATA_GetSaveFlag( bc_scr_wk->castle_savedata );
		break;

	//�x�ގ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
	case FC_ID_SAVE_REST_PLAY_DATA:
		CastleScr_SaveRestPlayData( bc_scr_wk, FR_MODE_REST );
		break;

	//------------------------------------
	//	
	//------------------------------------
	//���E���h�����C���N�������g
	case FC_ID_INC_ROUND:
		*work = CastleScr_IncRound( bc_scr_wk );
		break;

	//�����X�^�[�i���o�[���擾
	case FC_ID_GET_MONSNO:
		*work = bc_scr_wk->enemy_poke[param1].mons_no;
		break;

	//�Z�i���o�[���擾
	case FC_ID_GET_WAZANO:
		*work = bc_scr_wk->enemy_poke[param1].waza[param2];
		break;

	//�ۑ����Ă������A�C�e���i���o�[���Z�b�g����
	case FC_ID_SET_TEMP_ITEM:
		party = SaveData_GetTemotiPokemon( ex_param->savedata );	//�莝���p�[�e�B�擾
		for( i=0; i < 3 ;i++ ){
			pp = PokeParty_GetMemberPointer( party, bc_scr_wk->mine_poke_pos[i] );
			PokeParaPut( pp, ID_PARA_item, &bc_scr_wk->itemno[i] );
		}
		break;

	//���񐔂��擾
	case FC_ID_GET_LAP:
		*work = CastleScr_CommGetLap( bc_scr_wk );
		break;

	//�G�g���[�i�[��OBJ�R�[�h���擾
	case FC_ID_GET_TR_OBJ_CODE:
		*work = CastleScr_GetEnemyObjCode( bc_scr_wk, param1 );
		break;

	//�s�킵�����̏���
	case FC_ID_SET_LOSE:
		CastleScr_SetLose( bc_scr_wk );
		break;

	//7�A��(�N���A)�������̏���
	case FC_ID_SET_CLEAR:
		CastleScr_SetClear( bc_scr_wk );
		break;

	//���E���h�����擾
	case FC_ID_GET_ROUND:
		*work = CastleScr_GetRound( bc_scr_wk );
		break;

	//���^�C���t���O���擾
	case FC_ID_GET_RETIRE_FLAG:
		OS_Printf( "bc_scr_wk->pair_retire_flag = %d\n", bc_scr_wk->pair_retire_flag );
		*work = bc_scr_wk->pair_retire_flag;
		break;

	//�������������t���O���擾
	case FC_ID_GET_TRADE_YESNO_FLAG:
		*work = bc_scr_wk->pair_trade_yesno_flag;
		break;

	//�Ⴆ��CP�|�C���g���擾�ACP�𑫂�
	case FC_ID_BTL_WIN_CP_POINT:
		*work = CastleScr_GetBtlWinCpPoint( bc_scr_wk );
		CastleScr_AddCP( bc_scr_wk->sv, bc_scr_wk->type, *work );
		break;

	//PP�Ȃǂ̃f�[�^���X�V
	case FC_ID_BTL_WIN_POKE_DATA:
		CastleScr_BtlWinPokeData( bc_scr_wk );
		break;

	//�ʐM�ݒ�
	case FC_ID_COMM_COMMAND_INITIALIZE:
		CommCommandFrontierInitialize( bc_scr_wk );
		break;

	//�e�̌�����擾
	case FC_ID_GET_PARENT_CHECK_FLAG:
		if( bc_scr_wk->parent_check_flag >= CASTLE_SCR_MENU_MAX ){
			*work = (bc_scr_wk->parent_check_flag - CASTLE_SCR_MENU_MAX);	//�q�I�����̗p
			OS_Printf( "�q�̗p�@���� = %d\n", *work );
		}else{
			*work = bc_scr_wk->parent_check_flag;							//�e�I�����̗p
			OS_Printf( "�e�̗p�@���� = %d\n", *work );
		}
		break;

	//�e�̌�����N���A�A�y�A�̑I�����N���A�A�����̑I�����N���A
	case FC_ID_CLEAR_PARENT_CHECK_FLAG:
		bc_scr_wk->parent_check_flag = 0;
		bc_scr_wk->pair_sel_type = 0;										//08.07.05�ǉ�
		bc_scr_wk->sel_type = 0;											//08.07.05�ǉ�
		break;

	//�����N�A�������ȂǑI�������l��ۑ�
	case FC_ID_SET_SEL_TYPE:
		bc_scr_wk->sel_type = param1;
		break;

	//�e�q�̂ǂ���̌��肪�̗p���ꂽ���擾����CP�폜
	case FC_ID_NUKEMITI_CP_SUB:
		flag = 0;
		OS_Printf( "bc_scr_wk->parent_check_flag = %d\n", bc_scr_wk->parent_check_flag );

		if( bc_scr_wk->parent_check_flag >= CASTLE_SCR_MENU_MAX ){
			//*work = 1;													//�q�I�����̗p
			if( CommGetCurrentID() != COMM_PARENT_ID ){						//����=�q
				flag = 1;
			}
		}else{
			//*work = 0;													//�e�I�����̗p
			if( CommGetCurrentID() == COMM_PARENT_ID ){						//����=�e
				flag = 1;
			}
		}

		if( flag == 1 ){
			Castle_CPRecord_Sub(SaveData_GetFrontier(ex_param->savedata), 
								bc_scr_wk->type, CP_USE_NUKEMITI );
		}else{
			bc_scr_wk->pair_cp -= CP_USE_NUKEMITI;
		}

		break;

	//����̌��肪�̗p���ꂽ���擾
	case FC_ID_GET_PAIR_DECIDE_CHECK:
		*work = 0;

		if( Castle_CommCheck(bc_scr_wk->type) == TRUE ){
			if( bc_scr_wk->parent_check_flag >= CASTLE_SCR_MENU_MAX ){
				//*work = 1;												//�q�I�����̗p
				if( CommGetCurrentID() == COMM_PARENT_ID ){					//����=�e
					*work = 1;
				}
			}else{
				//*work = 0;												//�e�I�����̗p
				if( CommGetCurrentID() != COMM_PARENT_ID ){					//����=�q
					*work = 1;
				}
			}
		}
		break;

	//�ʐM�^�C�v���`�F�b�N
	case FC_ID_CHECK_COMM_TYPE:
		*work = Castle_CommCheck( bc_scr_wk->type );
		break;

	//�^�C�v�擾
	case FC_ID_GET_TYPE:
		*work = bc_scr_wk->type;
		break;

	//�m���̃|�P���������邩�擾
	case FC_ID_GET_HINSI_FLAG:
		*work = bc_scr_wk->hinsi_flag;
		break;

	//���񐔂ɂ���Ĕw�i�̈ꕔ������������
	case FC_ID_LAP_NUM_WRITE:
		fmap = FSS_GetFMapAdrs( core->fss );
		Castle_LapNumWrite( fmap->bgl, bc_scr_wk, GF_BGL_FRAME3_M );
		break;

	//�퓬�O��b�̂��߂�B_TOWER_PARTNER_DATA�̃Z�b�g
	case FC_ID_SET_B_TOWER_PARTNER_DATA:
		FSRomBattleTowerTrainerDataMake2( &(bc_scr_wk->tr_data[0]), 
						bc_scr_wk->tr_index[bc_scr_wk->round], HEAPID_WORLD, ARC_PL_BTD_TR );
		FSRomBattleTowerTrainerDataMake2( &(bc_scr_wk->tr_data[1]),
						bc_scr_wk->tr_index[bc_scr_wk->round+CASTLE_LAP_ENEMY_MAX], HEAPID_WORLD, 
						ARC_PL_BTD_TR );
		break;

	//�Ⴆ��o�g���|�C���g���擾
	case FC_ID_GET_BP_POINT:
		*work = CastleScr_GetAddBtlPoint( bc_scr_wk );
		break;

	//�������󂯂����擾(�擾��ɃZ�b�g���Ă���̂Œ��ӁI)
	case FC_ID_GET_GUIDE_FLAG:
		*work = (u16)CASTLESCORE_GetScoreData(	SaveData_GetCastleScore(ex_param->savedata),
												CASTLESCORE_ID_GUIDE_FLAG, 
												0, 0, NULL);

		buf8[0] = 1;
		CASTLESCORE_PutScoreData(	SaveData_GetCastleScore(ex_param->savedata),
									CASTLESCORE_ID_GUIDE_FLAG, 0, 0, buf8 );
		break;

	//�u���[���o�ꂩ�`�F�b�N(0=�Ȃ��A1=���o��A2=2���)
	case FC_ID_CHECK_BRAIN:
		*work = 0;

		//�V���O���̂�
		if( bc_scr_wk->type == CASTLE_TYPE_SINGLE ){

			if( (bc_scr_wk->rensyou+1) == CASTLE_LEADER_SET_1ST ){
				*work = 1;
			}else if( (bc_scr_wk->rensyou+1) == CASTLE_LEADER_SET_2ND ){
				*work = 2;
			}
		}
		break;

	//�G�|�P�����𐶐�
	case FC_ID_ENEMY_POKE_CREATE:
		//�G�|�P��������x�ɐ���
		Frontier_PokemonParamCreateAll(	bc_scr_wk->enemy_poke, bc_scr_wk->enemy_poke_index, 
										bc_scr_wk->enemy_pow_rnd, bc_scr_wk->enemy_personal_rnd, 
										NULL, 
										CASTLE_ENEMY_POKE_MAX, HEAPID_WORLD, ARC_PL_BTD_PM );
		break;

	//�G�|�P�����𐶐�2
	case FC_ID_ENEMY_POKE_CREATE_2:
		//�G�p�[�e�B�̃Z�b�g
		Castle_EnemyPartySet( bc_scr_wk );
		break;

	//�u���[�����o�ꂷ�郁�b�Z�[�W��\��������
	case FC_ID_BRAIN_APPEAR_MSG_CHK:
		*work = bc_scr_wk->brain_appear_msg_flag;
		bc_scr_wk->brain_appear_msg_flag = 1;
		break;

	//WIFI�}���`�Ř^��Z�[�u�Ō��݂�CP�����f���Ȃ����߂̑Ώ�
	case FC_ID_WIFI_MULTI_CP_TEMP:

		//fssc_castle_sub.c�Œ���J�n����CP��ޔ����Ă���

		//WIFI�}���`�ȊO�͕s���ȏI���N���A������̂Ŗ��Ȃ�
		if( bc_scr_wk->type == CASTLE_TYPE_WIFI_MULTI ){

			//�^��Z�[�u�O�ɌĂ�
			if( param1 == 0 ){
				//���݂�CP�ޔ�
				bc_scr_wk->temp_now_cp = 
						FrontierRecord_Get(SaveData_GetFrontier(ex_param->savedata),
						CastleScr_GetCPRecordID(bc_scr_wk->type),
						Frontier_GetFriendIndex(CastleScr_GetCPRecordID(bc_scr_wk->type)) );

				//�ޔ����Ă������펞��CP�ŏ㏑��
				FrontierRecord_Set(SaveData_GetFrontier(ex_param->savedata),
						CastleScr_GetCPRecordID(bc_scr_wk->type),
						Frontier_GetFriendIndex(CastleScr_GetCPRecordID(bc_scr_wk->type)),
						bc_scr_wk->temp_start_cp );

			//�^��Z�[�u��ɌĂ�
			}else{
				//�ޔ����Ă������݂�CP�ŏ㏑��
				FrontierRecord_Set(SaveData_GetFrontier(ex_param->savedata),
						CastleScr_GetCPRecordID(bc_scr_wk->type),
						Frontier_GetFriendIndex(CastleScr_GetCPRecordID(bc_scr_wk->type)),
						bc_scr_wk->temp_now_cp );
			}
		}
		break;

	};

	return 0;
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
BOOL FSSC_CastleLoseCheck( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	u16* work = FSSTGetWork( core );

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*work = bc_scr_wk->winlose_flag;
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
BOOL FSSC_CastleSendBuf( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	u16 type	= FSSTGetWorkValue( core );
	u16 param	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*ret_wk = CastleScr_CommSetSendBuf( bc_scr_wk, type, param );
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
BOOL FSSC_CastleRecvBuf( FSS_TASK * core )
{
	u16 wk_id = FSSTGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	FSST_SetWait( core, WaitCastleRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitCastleRecvBuf( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	u16 type = FSS_GetEventWorkValue( core, core->reg[0] );		//���ӁI

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//OS_Printf( "bc_scr_wk->recieve_count = %d\n", bc_scr_wk->recieve_count );
	if( bc_scr_wk->recieve_count >= CASTLE_COMM_PLAYER_NUM ){
		bc_scr_wk->recieve_count = 0;
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------
/**
 * �����N�A�������ȂǁA�ǂ�����s���邩�҂�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleParentCheckWait( FSS_TASK * core )
{
	u16 wk_id = FSSTGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	FSST_SetWait( core, WaitCastleParentCheckWait );
	return 1;
}

//return 1 = �I��
static BOOL WaitCastleParentCheckWait( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	u16* ret_wk = FSS_GetEventWorkAdrs( core, core->reg[0] );	//���ӁI

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	if( bc_scr_wk->parent_check_flag == CASTLE_SCR_MENU_NONE ){
		return 0;
	}

	bc_scr_wk->recieve_count = 0;
	*ret_wk = bc_scr_wk->parent_check_flag;
	return 1;
}

#if 1
//==============================================================================
//	CP�E�B���h�E
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief	CP�E�B���h�E�\��
 *
 * @param	none
 *
 * @return	0
 */
//--------------------------------------------------------------
BOOL FSSC_CPWinWrite( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	FSSC_Sub_CPWinWrite( core->fss, bc_scr_wk );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	CP�E�B���h�E�폜
 *
 * @param	none
 *
 * @return	0
 */
//--------------------------------------------------------------
BOOL FSSC_CPWinDel( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	FSSC_Sub_CPWinDel( core->fss, bc_scr_wk );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	CP�\��
 *
 * @param	none
 *
 * @return	0
 */
//--------------------------------------------------------------
BOOL FSSC_CPWrite( FSS_TASK * core )
{
	CASTLE_SCRWORK* bc_scr_wk;
	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	FSSC_Sub_CPWrite( core->fss, bc_scr_wk );
	return 0;
}

#endif

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FCP�擾
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleGetCP( FSS_TASK * core )
{
	FRONTIER_EX_PARAM* ex_param;
	u16 type	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	*ret_wk = FrontierRecord_Get(	SaveData_GetFrontier(ex_param->savedata), 
								CastleScr_GetCPRecordID(type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(type)) );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FCP���炷
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleSubCP( FSS_TASK * core )
{
	FRONTIER_EX_PARAM* ex_param;
	u16 type= FSSTGetWorkValue( core );
	u16 num	= FSSTGetWorkValue( core );

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	Castle_CPRecord_Sub( SaveData_GetFrontier(ex_param->savedata), type, num );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FCP���₷
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_CastleAddCP( FSS_TASK * core )
{
	u16 now_cp;
	FRONTIER_EX_PARAM* ex_param;
	u16 type= FSSTGetWorkValue( core );
	u16 num	= FSSTGetWorkValue( core );

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	CastleScr_AddCP( ex_param->savedata, type, num );
	return 0;
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g���L���b�X���ΐ�O���b�Z�[�W��p�\��
 */
//--------------------------------------------------------------
BOOL FSSC_CastleTalkMsgAppear(FSS_TASK* core)
{
	u16	*msg;
	CASTLE_SCRWORK*	bc_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );
	u16	tr_idx = FSSTGetU8(core);	//��l�ڂ���l�ڂ��H

	bc_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	if(bc_scr_wk == NULL){
		return 0;
	}

	msg = bc_scr_wk->tr_data[tr_idx].bt_trd.appear_word;

	FrontierTalkMsgSub( core, msg );
	return 1;
}


