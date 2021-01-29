//============================================================================================
/**
 * @file	fssc_stage.c
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�F�X�e�[�W
 * @author	Satoshi Nohara
 * @date	07.06.07
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
#include "msgdata\msg.naix"						//NARC_msg_??_dat

#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "gflib/strbuf_family.h"				//����

#include "../field/field_battle.h"				//BattleParam_IsWinResult
#include "../field/field_subproc.h"				//TestBattleProcData
#include "../application/stage.h"

#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "frontier_tool.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"
#include "frontier_def.h"
#include "stage_tool.h"
#include "stage_def.h"
#include "fss_task.h"
#include "fssc_stage_sub.h"
#include "comm_command_frontier.h"

#include "field/eventflag.h"

#include "savedata/tv_work.h"
#include "../field/tv_topic.h"					//�e���r�g�s�b�N�����p
#include "field/tvtopic_extern.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL FSSC_StageWorkAlloc( FSS_TASK * core );
BOOL FSSC_StageWorkEnemySet( FSS_TASK * core );
BOOL FSSC_StageWorkFree( FSS_TASK * core );
BOOL FSSC_StagePokeTypeSelCall( FSS_TASK * core );
BOOL FSSC_StageCallGetResult( FSS_TASK * core );
BOOL FSSC_StageBattleCall( FSS_TASK * core );
BOOL FSSC_StageScrWork( FSS_TASK * core );
BOOL FSSC_StageLoseCheck( FSS_TASK * core );
BOOL FSSC_StageSendBuf( FSS_TASK * core );
BOOL FSSC_StageRecvBuf( FSS_TASK * core );
static BOOL WaitStageRecvBuf( FSS_TASK * core );
static u16 btlstage_GetMineObj( STAGE_SCRWORK* wk );
BOOL FSSC_StageTalkMsgAppear(FSS_TASK* core);
BOOL FSSC_StageRenshouCopyExtra( FSS_TASK * core );

//�n�������[�N�̉������
static void StageFreeMemory( void *parent_work );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�X�e�[�W���[�N�m��
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_StageWorkAlloc( FSS_TASK * core ) 
{
	STAGE_SCRWORK* wk;
	FRONTIER_EX_PARAM* ex_param;
	u16 init	= FSSTGetWorkValue( core );
	u16 type	= FSSTGetWorkValue( core );
	u16 pos		= FSSTGetWorkValue( core );		//�I�������|�P�����ʒu
	u16 pos2	= FSSTGetWorkValue( core );		//�I�������|�P�����ʒu

	ex_param = Frontier_ExParamGet( core->fss->fmain );

	wk = StageScr_WorkAlloc( ex_param->savedata, init, type, pos, pos2 );
	Frontier_SysWorkSet( core->fss->fmain, wk );

	OS_Printf( "�X�N���v�g�X�e�[�W���[�N�m��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�e�[�W���[�N�@�G�g���[�i�[�ƓG�|�P������I�o
 *
 * @param	core	���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_StageWorkEnemySet( FSS_TASK * core ) 
{
	STAGE_SCRWORK* bs_scr_wk;
	u16 init = FSSTGetWorkValue( core );

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	StageScr_WorkEnemySet( bs_scr_wk, init );

	OS_Printf( "�G�g���[�i�[�A�G�|�P�����̑I�o\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�e�[�W���[�N�폜
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_StageWorkFree( FSS_TASK * core ) 
{
	STAGE_SCRWORK* bs_scr_wk;
	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	StageScr_WorkRelease( bs_scr_wk );
	OS_Printf( "�X�N���v�g�X�e�[�W���[�N�J��\n" );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�^�C�v�I����ʌĂяo��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_StagePokeTypeSelCall( FSS_TASK * core)
{
	int i;
	POKEPARTY* party;
	POKEMON_PARAM* poke;
	STAGE_SCRWORK* bs_scr_wk;
	STAGE_CALL_WORK* stage_call;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	//�I�[�o�[���CID�錾
	FS_EXTERN_OVERLAY( stage );
	
	//�^�C�v�I����ʃv���Z�X�f�[�^
	static const PROC_DATA StageProc = {	
		StageProc_Init,
		StageProc_Main,
		StageProc_End,
		FS_OVERLAY_ID( stage ),
	};

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	OS_Printf( "�^�C�v�I����ʌĂяo��\n" );

	stage_call = sys_AllocMemory( HEAPID_WORLD, sizeof(STAGE_CALL_WORK) );
	MI_CpuClear8( stage_call, sizeof(STAGE_CALL_WORK) );
	stage_call->sv				= ex_param->savedata;
	stage_call->type			= bs_scr_wk->type;
	stage_call->p_work			= bs_scr_wk;
	stage_call->p_rank			= &bs_scr_wk->type_level[bs_scr_wk->type][0];	//0-9
	stage_call->p_party			= bs_scr_wk->p_party;
	//stage_call->btl_count		= ( bs_scr_wk->rensyou + bs_scr_wk->round );
	stage_call->btl_count		= bs_scr_wk->rensyou;
	OS_Printf( "�A���� = %d\n", bs_scr_wk->rensyou );
	//OS_Printf( "���l�� = %d\n", bs_scr_wk->round );
	stage_call->csr_pos			= bs_scr_wk->csr_pos;
	stage_call->p_pair_poke_level = &bs_scr_wk->pair_poke_level[0];

	PokeParty_InitWork( stage_call->p_party );
	party = SaveData_GetTemotiPokemon( ex_param->savedata );					//�莝���p�[�e�B�擾
	poke = PokeParty_GetMemberPointer( party, bs_scr_wk->mine_poke_pos[0] );
	PokeParty_Add( stage_call->p_party, poke );

	//�_�u��
	if( stage_call->type == STAGE_TYPE_DOUBLE ){
		poke = PokeParty_GetMemberPointer( party, bs_scr_wk->mine_poke_pos[1] );
		PokeParty_Add(	stage_call->p_party, poke );

	//�}���`�AWIFI
	}else if( (stage_call->type == STAGE_TYPE_MULTI) || 
					(stage_call->type == STAGE_TYPE_WIFI_MULTI) ){
		PokeParty_Add(	stage_call->p_party, bs_scr_wk->pair_poke );
	}

    Frontier_SubProcSet( core->fss->fmain, &StageProc, stage_call, FALSE, StageFreeMemory);
	return 1;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�F�X�e�[�W�퓬���ʎ擾���ĊJ��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_StageCallGetResult( FSS_TASK * core)
{
	STAGE_SCRWORK* bs_scr_wk;
	BATTLE_PARAM* param;

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	param = bs_scr_wk->p_work;

	bs_scr_wk->winlose_flag = BattleParam_IsWinResult( param->win_lose_flag );
	OS_Printf( "bs_scr_wk->winlose_flag = %d\n", bs_scr_wk->winlose_flag );

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
BOOL FSSC_StageBattleCall( FSS_TASK * core)
{
	BATTLE_PARAM* param;
	STAGE_SCRWORK* bs_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	//�o�g���f�[�^������
	param = BtlStage_CreateBattleParam( bs_scr_wk, ex_param );
	bs_scr_wk->p_work = param;

	//�퓬�؂�ւ�
	Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );		//�o�g���ȍĐ�
    Frontier_SubProcSet(core->fss->fmain, &TestBattleProcData, param, FALSE, NULL );
	OS_Printf( "�X�N���v�g�X�e�[�W�퓬�Ăяo��\n" );			//field_encount.c
	return 1;
}

//�X�e�[�W��ʂœn�������[�N�̉������
static void StageFreeMemory( void *parent_work )
{
	int i;
	POKEMON_PARAM* temp_poke;
	STAGE_CALL_WORK* stage_call = parent_work;

	//���ʂ��擾
	StageScr_GetResult( stage_call->p_work, parent_work );

	//STAGE_CALL_WORK�̍폜
	sys_FreeMemoryEz( parent_work );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g�R�}���h�FSTAGE_SCR_WORK�̃Z�b�g�A�Q�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_StageScrWork( FSS_TASK * core )
{
	FRONTIER_EX_PARAM* ex_param;
	POKEMON_PARAM* temp_poke;
	POKEPARTY* party;
	STAGE_SCRWORK* bs_scr_wk;
	u8 rank,m_max;
	u32 type;
	int i;
	u8 code		= FSSTGetU8( core );
	u8 param1	= FSSTGetU8( core );
	u8 param2	= FSSTGetU8( core );
	u16* work	= FSSTGetWork( core );

	ex_param	= Frontier_ExParamGet( core->fss->fmain );
	bs_scr_wk	= Frontier_SysWorkGet( core->fss->fmain );

	switch( code ){

	//�^�C�v(�V���O���A�_�u���A�}���`�Awifi�}���`)�̃Z�b�g
	case FS_ID_SET_TYPE:
		bs_scr_wk->type = param1;
		break;

	//ret_work�̎擾
	case FS_ID_GET_RET_WORK:
		*work = bs_scr_wk->ret_work;
		break;

	//�A�����̎擾
	case FS_ID_GET_RENSYOU:
		*work = bs_scr_wk->rensyou;
		break;

	//�A�����̃C���N�������g
	case FS_ID_INC_RENSYOU:
		if( bs_scr_wk->rensyou < STAGE_RENSYOU_MAX ){
			bs_scr_wk->rensyou++;
		}
		break;

	//���Z�b�g���ă^�C�g���ɖ߂�
	case FS_ID_SYSTEM_RESET:
		OS_ResetSystem( 0 );
		break;

	//------------------------------------
	//	STAGEDATA�ɃA�N�Z�X
	//------------------------------------
	//�Z�[�u�f�[�^���L�����ǂ����Ԃ�
	case FS_ID_IS_SAVE_DATA_ENABLE:
		*work = STAGEDATA_GetSaveFlag( bs_scr_wk->stage_savedata );
		break;

	//�x�ގ��Ɍ��݂̃v���C�󋵂��Z�[�u�ɏ����o��
	case FS_ID_SAVE_REST_PLAY_DATA:
		StageScr_SaveRestPlayData( bs_scr_wk, FR_MODE_REST );
		break;

	//���E���h�����C���N�������g
	case FS_ID_INC_ROUND:
		*work = StageScr_IncRound( bs_scr_wk );
		break;

	//���񐔂��擾
	case FS_ID_GET_LAP:
		//�ʐM�}���`�̂݃����N10�Œ�I
		if( bs_scr_wk->type == STAGE_TYPE_MULTI ){
			//*work = STAGE_LAP_MAX;
			*work = STAGE_TYPE_LEVEL_MAX;
		}else{
			*work = bs_scr_wk->lap;
		}
		break;

	//�G�g���[�i�[��OBJ�R�[�h���擾
	case FS_ID_GET_TR_OBJ_CODE:
		*work = StageScr_GetEnemyObjCode( bs_scr_wk, param1 );
		break;

	//�s�킵�����̏���
	case FS_ID_SET_LOSE:
		StageScr_SetLose( bs_scr_wk );
		break;

	//7�A��(�N���A)�������̏���
	case FS_ID_SET_CLEAR:
		StageScr_SetClear( bs_scr_wk );
		break;

	//���E���h�����擾
	case FS_ID_GET_ROUND:
		*work = StageScr_GetRound( bs_scr_wk );
		break;

	//���^�C���t���O���擾
	case FS_ID_GET_RETIRE_FLAG:
		*work = bs_scr_wk->pair_retire_flag;
		break;

	//�ۑ����Ă������A�C�e���i���o�[���Z�b�g����
	case FS_ID_SET_TEMP_ITEM:
		party = SaveData_GetTemotiPokemon( ex_param->savedata );	//�莝���p�[�e�B�擾
		m_max = Stage_GetMinePokeNum( bs_scr_wk->type );
		for( i=0; i < m_max ;i++ ){
			temp_poke = PokeParty_GetMemberPointer( party, bs_scr_wk->mine_poke_pos[i] );
			PokeParaPut( temp_poke, ID_PARA_item, &bs_scr_wk->itemno[i] );
		}
		break;

	//�I������Ă���^�C�v�̃����N���グ��(0-9��10�ɂȂ����炻�̂܂�)(�\����1-10��10�̂܂�)
	case FS_ID_INC_TYPE_LEVEL:
		//�������̓��R�[�h�ł͂Ȃ��I
		rank = Stage_GetTypeLevel( bs_scr_wk->csr_pos, &bs_scr_wk->type_level[bs_scr_wk->type][0] );
		if( rank < STAGE_TYPE_LEVEL_MAX ){
			Stage_SetTypeLevel( bs_scr_wk->csr_pos, 
								&bs_scr_wk->type_level[bs_scr_wk->type][0], (rank+1) );
		}
		break;

	//�ʐM�ݒ�
	case FS_ID_COMM_COMMAND_INITIALIZE:
		CommCommandFrontierInitialize( bs_scr_wk );
		break;

	//�ʐM�^�C�v���`�F�b�N
	case FS_ID_CHECK_COMM_TYPE:
		*work = Stage_CommCheck( bs_scr_wk->type );
		break;

	//�^�C�v�擾
	case FS_ID_GET_TYPE:
		*work = bs_scr_wk->type;
		break;

	//���d�ʂ�\������
	case FS_ID_EFF_FRAME_ON:
		if( param1 == 0 ){
			//GF_Disp_GX_VisibleControl( FRMAP_FRAME_EFF, VISIBLE_ON );		//BG�\��
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );		//BG�\��
		}else{
			//GF_Disp_GX_VisibleControl( FRMAP_FRAME_EFF, VISIBLE_ON );		//BG�\��
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��
		}
		break;

	//�퓬�O��b�̂��߂�B_TOWER_PARTNER_DATA�̃Z�b�g
	case FS_ID_SET_B_TOWER_PARTNER_DATA:
		FSRomBattleTowerTrainerDataMake2( &(bs_scr_wk->tr_data[0]), 
					bs_scr_wk->tr_index[bs_scr_wk->round*STAGE_ENTRY_POKE_MAX], HEAPID_WORLD, 
					ARC_PL_BTD_TR );
		FSRomBattleTowerTrainerDataMake2( &(bs_scr_wk->tr_data[1]),
					bs_scr_wk->tr_index[bs_scr_wk->round*STAGE_ENTRY_POKE_MAX+1], HEAPID_WORLD,
					ARC_PL_BTD_TR );
		break;

	//�I������Ă���^�C�v�̃����N���擾
	case FS_ID_GET_TYPE_LEVEL:
		//�������̓��R�[�h�ł͂Ȃ��I
		*work=Stage_GetTypeLevel( bs_scr_wk->csr_pos, &bs_scr_wk->type_level[bs_scr_wk->type][0] );
		break;

	//�Ⴆ��o�g���|�C���g���擾
	case FS_ID_GET_BP_POINT:
		*work = StageScr_GetAddBtlPoint( bs_scr_wk );
		break;

	//���킵�Ă���|�P�����i���o�[���擾
	case FS_ID_GET_MONSNO:
		party = SaveData_GetTemotiPokemon( ex_param->savedata );	//�莝���p�[�e�B�擾
		temp_poke = PokeParty_GetMemberPointer( party, bs_scr_wk->mine_poke_pos[0] );
		*work = PokeParaGet( temp_poke, ID_PARA_monsno, NULL );
		break;

	//�^�C�v�̃����N���S�Ē���ς݂̍ő�ɂȂ��Ă����璲����������
	case FS_ID_TYPE_LEVEL_MAX:
		StageScr_TypeLevelMaxOff( bs_scr_wk );
		break;

	//�Q���|�P�����ň�ԃ��x���������l�̕��������v�Z
	case FS_ID_SQRT:
		StageScr_Sqrt( bs_scr_wk );
		break;

	//�u���[���o�ꂩ�`�F�b�N(0=�Ȃ��A1=���o��A2=2���)
	case FS_ID_CHECK_BRAIN:
		*work = 0;

		//�V���O���̂�
		if( bs_scr_wk->type == STAGE_TYPE_SINGLE ){

			if( (bs_scr_wk->rensyou+1) == STAGE_LEADER_SET_1ST ){
				*work = 1;
			}else if( (bs_scr_wk->rensyou+1) == STAGE_LEADER_SET_2ND ){
				*work = 2;
			}
		}
		break;

	//�E�B���h�E����(�X�N���[���������̃|�P����OBJ�̉��z�͈͑Ή�)
	case FS_ID_WND_SET:
#if 0
		if( param1 == 1 ){
			GX_SetVisibleWnd( GX_WNDMASK_W0 );

			G2_SetWnd0InsidePlane(	GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | 
									GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, TRUE );

			//�u���[���̎��́A��l���������ԂȂ̂ŁA�����ɔ��Ȃ��悤�ɂ��Ȃ��Ƃ����Ȃ�
			//G2_SetWnd0Position( 96, 192-32, 210, 192 );
			//
			G2_SetWnd0Position( 90, 192-32, 210, 192 );

			//����X��2dot���A�|�P�����Ǝ�l�������Ԃ��Ă���

			//��l����������
			//G2_SetWnd0Position( 88, 192-32, 210, 192 );
		}else{
			GX_SetVisibleWnd( GX_WNDMASK_NONE );			//���ɖ߂�
		}
#endif
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
BOOL FSSC_StageLoseCheck( FSS_TASK * core )
{
	STAGE_SCRWORK* bs_scr_wk;
	u16* work = FSSTGetWork( core );

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*work = bs_scr_wk->winlose_flag;
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
BOOL FSSC_StageSendBuf( FSS_TASK * core )
{
	STAGE_SCRWORK* bs_scr_wk;
	u16 type	= FSSTGetWorkValue( core );
	u16 param	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	bs_scr_wk = Frontier_SysWorkGet( core->fss->fmain );

	*ret_wk	= StageScr_CommSetSendBuf( bs_scr_wk, type, param );
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
BOOL FSSC_StageRecvBuf( FSS_TASK * core )
{
	u16 wk_id = FSSTGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	FSST_SetWait( core, WaitStageRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitStageRecvBuf( FSS_TASK * core )
{
	STAGE_SCRWORK* bs_scr_wk;
	u16 type = FSS_GetEventWorkValue( core, core->reg[0] );		//���ӁI

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	if( bs_scr_wk->recieve_count >= STAGE_COMM_PLAYER_NUM ){
		bs_scr_wk->recieve_count = 0;
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------
/**
 *	@brief	�o�g���X�e�[�W�ΐ�O���b�Z�[�W��p�\��
 */
//--------------------------------------------------------------
BOOL FSSC_StageTalkMsgAppear(FSS_TASK* core)
{
	u16	*msg;
	STAGE_SCRWORK*	bs_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );
	u16	tr_idx = FSSTGetU8(core);	//��l�ڂ���l�ڂ��H

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	if(bs_scr_wk == NULL){
		return 0;
	}

	msg = bs_scr_wk->tr_data[tr_idx].bt_trd.appear_word;

	FrontierTalkMsgSub( core, msg );
	return 1;
}

//--------------------------------------------------------------------------------------------
/**
 * �o�g���X�e�[�W�̊O���A���L�^���������݁A�Z�[�u���s��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"1"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_StageRenshouCopyExtra( FSS_TASK * core )
{
	STAGE_SCRWORK* bs_scr_wk;
	u16* ret_wk	= FSSTGetWork( core );

	bs_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );
	StageScr_StageRenshouCopyExtra( bs_scr_wk, ret_wk );
	return 1;
}

//--------------------------------------------------------------------------------------------
/**
 * TV:�X�e�[�WTEMP�Z�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
BOOL FSSC_TVTempStageSet( FSS_TASK * core );
BOOL FSSC_TVTempStageSet( FSS_TASK * core )
{
	TV_WORK * tvwk;
	u16 monsno;
	POKEPARTY* party;
	POKEMON_PARAM* temp_poke;
	STAGE_SCRWORK* bs_scr_wk;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	bs_scr_wk	=  Frontier_SysWorkGet( core->fss->fmain );
	party		= SaveData_GetTemotiPokemon( ex_param->savedata );	//�莝���p�[�e�B�擾
	temp_poke	= PokeParty_GetMemberPointer( party, bs_scr_wk->mine_poke_pos[0] );
	monsno		= PokeParaGet( temp_poke, ID_PARA_monsno, NULL );

	//�V���O���̂�
	if( bs_scr_wk->type == STAGE_TYPE_SINGLE ){
		tvwk = SaveData_GetTvWork( ex_param->savedata );
		TVTOPIC_StageTemp_Set( tvwk, temp_poke );
	}

	return 0;
}


