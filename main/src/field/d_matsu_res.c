//==============================================================================
/**
 * @file	d_matsu_res.c
 * @brief	���c�f�o�b�O�p�t�@�C��(�풓��)
 * @author	matsuda
 * @date	2007.04.18(��)
 */
//==============================================================================

#ifdef PM_DEBUG	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
#include "common.h"
#include "system/pm_overlay.h"
#include "system/test_overlay.h"

#include "system/bmp_list.h"
#include "fieldmap.h"
#include "fieldsys.h"
#include "system/fontproc.h"
#include "contest/contest.h"

#include "script.h"
#include "..\fielddata\script\connect_def.h"

#include "system/particle.h"
#include "particledata/particledata.h"
#include "particledata/particledata.naix"

#include "system/fontproc.h"
#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_matsu.h"

#include "field/sysflag.h"

#include "debug/d_matsu.h"

#include "application/balloon.h"




void DMRes_FssTest(DMATSU_WORK *dm);
static BOOL GMEVENT_DebugFssTestStart(GMEVENT_CONTROL * event);
static BOOL GMEVENT_Sub_FssTest(GMEVENT_CONTROL * event);

void DMRes_BalloonStart(FIELDSYS_WORK *fsys, int proc_flag, int mode);
static BOOL GMEVENT_DebugBalloonStart(GMEVENT_CONTROL * event);
static BOOL GMEVENT_Sub_DebugBalloon(GMEVENT_CONTROL * event);
static BOOL GMEVENT_Sub_DebugGds(GMEVENT_CONTROL * event);
static BOOL GMEVENT_Sub_DebugFootprint(GMEVENT_CONTROL * event);

#if DEBUG_MATSU_CONTEST
BOOL GMEVENT_DebugConStart(GMEVENT_CONTROL * event);
static void EventCmd_DebugConProc(GMEVENT_CONTROL * event, int debug_mode, int rank, int type);
static BOOL GMEVENT_Sub_DebugCon(GMEVENT_CONTROL * event);
static void ContestDebug_ImageClipDebugAccessorySet(CONTEST_SYSTEM *consys);
static void ContestDebug_ImageClipDebugAccessoryFree(CONTEST_SYSTEM *consys);
#endif

FS_EXTERN_OVERLAY(frontier_common);

FS_EXTERN_OVERLAY(balloon);
FS_EXTERN_OVERLAY(minigame_common);
FS_EXTERN_OVERLAY(gds_comm);
FS_EXTERN_OVERLAY(footprint_board);

#include "ev_mapchange.h"
#include "frontier/frontier_types.h"
#include "frontier/frontier_main.h"
#include "frontier/fss_scene.h"
#include "battle/attr_def.h"

#include "application/br_sys.h"
#include "application/gds_main.h"

#include "application/wifi_lobby/wflby_system_def.h"
#include "application/footprint_main.h"

#include "contest/contest.h"
#include "include/application/imageClip/imc_sys.h"

typedef struct{
	int seq;
	FRONTIER_EX_PARAM ex_param;
}DEBUG_FSS_WORK;


///���D����~�j�Q�[���Ăяo�����Ɏg�p���郏�[�N
typedef struct{
	int main_seq;
	int debug_mode;
	int proc_flag;		///<�ǂ�PROC���Ăяo����
	int mode;			///<PROC�ɃI�v�V����
}DMATSU_EV_BALLOON_WORK;

///���D����~�j�Q�[���Ăяo�����Ɏg�p���郏�[�N
typedef struct{
	int seq;
	int proc_flag;
	BALLOON_PROC_WORK *p_work;
	int mode;			///<PROC�ɃI�v�V����
}DEBUG_BALLOON_LOCAL;


//==============================================================================
//	�O���[�o���ϐ��錾
//==============================================================================
///1:���[�e�[�V�����Ȃ��B�@0:���[�e�[�V��������@2:�_���X�񐔐����Ȃ�
u8 DmatsuWork_DanceRotationOff = 0;


//==============================================================================
//	�O���f�[�^
//==============================================================================
extern const PROC_DATA ContestActinProcData;
extern const PROC_DATA ContestDanceProcData;
extern const PROC_DATA ContestVisualProcData;
extern const PROC_DATA ContestResultProcData;
extern const PROC_DATA IMC_SYS_Proc;

//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void Contest_ImageClipInitDataCreate(CONTEST_SYSTEM *consys);
extern void Contest_ImageClipInitDataFree(CONTEST_SYSTEM *consys);



//--------------------------------------------------------------
/**
 * @brief   FSS�n�X�N���v�g�e�X�g���s
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
void DMRes_FssTest(DMATSU_WORK *dm)
{
	u32 *al_work;
	
	al_work = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(u32));
	MI_CpuClear8(al_work, sizeof(u32));
	
	FieldEvent_Set(dm->fsys, GMEVENT_DebugFssTestStart, al_work);
}

static BOOL GMEVENT_DebugFssTestStart(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys;
	u32 *al_work;
	
	fsys = FieldEvent_GetFieldSysWork(event);
	al_work = FieldEvent_GetSpecialWork(event);

	switch (*al_work) {
	case 0:
		EventCmd_FieldFadeOut(event);
		(*al_work)++;
		break;
	case 1:
		{
			DEBUG_FSS_WORK *sub_work = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(DEBUG_FSS_WORK));
			MI_CpuClear8(sub_work, sizeof(DEBUG_FSS_WORK));
			FieldEvent_Call(event, GMEVENT_Sub_FssTest, sub_work);
		}
		(*al_work)++;
		break;
	case 2:
		EventCmd_FieldFadeIn(event);
		(*al_work)++;
		break;
	case 3:
		sys_FreeMemoryEz(al_work);
		return TRUE;
	}
	return FALSE;
}

static BOOL GMEVENT_Sub_FssTest(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys = FieldEvent_GetFieldSysWork(event);
	DEBUG_FSS_WORK *sub_work = FieldEvent_GetSpecialWork(event);
	
	switch (sub_work->seq) {
	case 0:
		EventCmd_FinishFieldMap(event);
		sub_work->seq++;
		break;
	case 1:
		//ex_param�쐬
		sub_work->ex_param.syswork = NULL;
		sub_work->ex_param.config = SaveData_GetConfig(fsys->savedata);
		sub_work->ex_param.savedata = GameSystem_GetSaveData(fsys);
		sub_work->ex_param.scene_id = FSS_SCENEID_TESTSCR;
		sub_work->ex_param.fnote_data = fsys->fnote;
		sub_work->ex_param.bag_cursor = fsys->bag_cursor;
		sub_work->ex_param.bg_id = BG_ID_ROOM_A;
		sub_work->ex_param.ground_id = GROUND_ID_FLOOR;
		sub_work->ex_param.zone_id = fsys->location->zone_id;
		sub_work->ex_param.battle_cursor = fsys->battle_cursor;
		
		EventCmd_CallSubProc(event, &FrontierMainProcData, &sub_work->ex_param);
		sub_work->seq++;
		break;
	case 2:
		EventCmd_StartFieldMap(event);
		sub_work->seq++;
		break;
	case 3:
		sys_FreeMemoryEz(sub_work);
		return TRUE;
	}
	return FALSE;
}




//--------------------------------------------------------------
/**
 * @brief   ���D���聕GDS�e�X�g�F�Ăяo��
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
void DMRes_BalloonStart(FIELDSYS_WORK *fsys, int proc_flag, int mode)
{
	DMATSU_EV_BALLOON_WORK *eaw;
	
	eaw = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(DMATSU_EV_BALLOON_WORK));
	MI_CpuClear8(eaw, sizeof(DMATSU_EV_BALLOON_WORK));
	eaw->proc_flag = proc_flag;
	eaw->mode = mode;
	FieldEvent_Set(fsys, GMEVENT_DebugBalloonStart, eaw);
}

//-----------------------------------------------------------------------------
/**
 * @brief	���D����F�C�x���g�V�[�P���X
 * @param	event		�C�x���g���䃏�[�N�ւ̃|�C���^
 * @retval	TRUE		�C�x���g�I��
 * @retval	FALSE		�C�x���g�p����
 *
 * �G���J�E���g�G�t�F�N�g�����t�B�[���h���A���s���B
 * �Q�[���I�[�o�[�����ւ̕��������i�\��j
 */
//-----------------------------------------------------------------------------
static BOOL GMEVENT_DebugBalloonStart(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys;
	DMATSU_EV_BALLOON_WORK *eaw;
	
	fsys = FieldEvent_GetFieldSysWork(event);
	eaw = FieldEvent_GetSpecialWork(event);

	switch (eaw->main_seq) {
	case 0:
		EventCmd_FieldFadeOut(event);
		eaw->main_seq++;
		break;
	case 1:
		{
			DEBUG_BALLOON_LOCAL *sub_work = sys_AllocMemoryLo(HEAPID_WORLD, 
				sizeof(DEBUG_BALLOON_LOCAL));
			MI_CpuClear8(sub_work, sizeof(DEBUG_BALLOON_LOCAL));
			sub_work->proc_flag = eaw->proc_flag;
			sub_work->mode = eaw->mode;
			switch(eaw->proc_flag){
			case DMATSU_PROC_FLAG_BALLOON:
				FieldEvent_Call(event, GMEVENT_Sub_DebugBalloon, sub_work);
				break;
			case DMATSU_PROC_FLAG_GDS:
			case DMATSU_PROC_FLAG_GDS_RECORDER:
				FieldEvent_Call(event, GMEVENT_Sub_DebugGds, sub_work);
				break;
			case DMATSU_PROC_FLAG_FOOTPRINT:
				FieldEvent_Call(event, GMEVENT_Sub_DebugFootprint, sub_work);
				break;
			}
			eaw->main_seq++;
		}
		break;
	case 2:
		EventCmd_FieldFadeIn(event);
		eaw->main_seq++;
		break;
	case 3:
		sys_FreeMemoryEz(eaw);
		return TRUE;
	}
	return FALSE;
}

static BOOL GMEVENT_Sub_DebugBalloon(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys = FieldEvent_GetFieldSysWork(event);
	DEBUG_BALLOON_LOCAL *sub_work = FieldEvent_GetSpecialWork(event);
	
	switch (sub_work->seq) {
	case 0:
		EventCmd_FinishFieldMap(event);
		sub_work->seq ++;
		break;
	case 1:
		{
			// �v���Z�X��`�f�[�^
			static const PROC_DATA BalloonProcData = {
				BalloonProc_Init,
				BalloonProc_Main,
				BalloonProc_End,
				FS_OVERLAY_ID(balloon),
			};
			sub_work->p_work = sys_AllocMemory( HEAPID_WORLD, sizeof(BALLOON_PROC_WORK) );
			MI_CpuClear8(sub_work->p_work, sizeof(BALLOON_PROC_WORK));
			sub_work->p_work->vchat		= 0;
			sub_work->p_work->wifi_lobby	= FALSE;
			sub_work->p_work->p_save		= NULL;
			sub_work->p_work->debug_offline = TRUE;
			
			// �~�j�Q�[�����ʃI�[�o�[���C��ǂݍ���
			Overlay_Load( FS_OVERLAY_ID(minigame_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);
			
			//GameSystem_StartSubProc(fsys, &BalloonProcData, sub_work->p_work);
			EventCmd_CallSubProc(event, &BalloonProcData, sub_work->p_work);
		}
		
		sub_work->seq ++;
		break;
	case 2:
		sys_FreeMemoryEz(sub_work->p_work);
		Overlay_UnloadID( FS_OVERLAY_ID(minigame_common) );
		
		EventCmd_StartFieldMap(event);
		sub_work->seq ++;
		break;
	case 3:
		sys_FreeMemoryEz(sub_work);
		return TRUE;
	}
	return FALSE;
}

extern PROC_RESULT GdsMainProc_Init( PROC * proc, int * seq );
extern PROC_RESULT GdsMainProc_Main( PROC * proc, int * seq );
extern PROC_RESULT GdsMainProc_End( PROC * proc, int * seq );

static BOOL GMEVENT_Sub_DebugGds(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys = FieldEvent_GetFieldSysWork(event);
	DEBUG_BALLOON_LOCAL *sub_work = FieldEvent_GetSpecialWork(event);
	
	switch (sub_work->seq) {
	case 0:
		EventCmd_FinishFieldMap(event);
		sub_work->seq ++;
		break;
	case 1:
		{
			GDSPROC_PARAM *param;
			// �v���Z�X��`�f�[�^
			static const PROC_DATA GdsProcData = {
				GdsMainProc_Init,
				GdsMainProc_Main,
				GdsMainProc_End,
				FS_OVERLAY_ID(gds_comm),
			};

			param = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(GDSPROC_PARAM));
			MI_CpuClear8(param, sizeof(GDSPROC_PARAM));
			param->fsys = fsys;
			param->savedata = fsys->savedata;
			param->connect = 0;
			param->gds_mode = sub_work->mode;
	
			EventCmd_CallSubProc(event, &GdsProcData, param);
		}
		
		sub_work->seq ++;
		break;
	case 2:
		EventCmd_StartFieldMap(event);
		sub_work->seq ++;
		break;
	case 3:
		sys_FreeMemoryEz(sub_work);
		return TRUE;
	}
	return FALSE;
}

static BOOL GMEVENT_Sub_DebugFootprint(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys = FieldEvent_GetFieldSysWork(event);
	DEBUG_BALLOON_LOCAL *sub_work = FieldEvent_GetSpecialWork(event);
	
	switch (sub_work->seq) {
	case 0:
		EventCmd_FinishFieldMap(event);
		sub_work->seq ++;
		break;
	case 1:
		{
			// �v���Z�X��`�f�[�^
			static const PROC_DATA FootprintProcData = {
				FootPrintProc_Init,
				FootPrintProc_Main,
				FootPrintProc_End,
				FS_OVERLAY_ID(footprint_board),
			};
			FOOTPRINT_PARAM *footparam = sys_AllocMemory(HEAPID_WORLD, sizeof(FOOTPRINT_PARAM));
			MI_CpuClear8(footparam, sizeof(FOOTPRINT_PARAM));
			footparam->debug_sv = fsys->savedata;
			EventCmd_CallSubProc(event, &FootprintProcData, footparam);
		}
		
		sub_work->seq ++;
		break;
	case 2:
		EventCmd_StartFieldMap(event);
		sub_work->seq ++;
		break;
	case 3:
		sys_FreeMemoryEz(sub_work);
		return TRUE;
	}
	return FALSE;
}








#if DEBUG_MATSU_CONTEST
//-----------------------------------------------------------------------------
/**
 * @brief	�R���e�X�g�F���Z�͕���C�x���g�V�[�P���X
 * @param	event		�C�x���g���䃏�[�N�ւ̃|�C���^
 * @retval	TRUE		�C�x���g�I��
 * @retval	FALSE		�C�x���g�p����
 *
 * �G���J�E���g�G�t�F�N�g�����t�B�[���h���A���s���B
 * �Q�[���I�[�o�[�����ւ̕��������i�\��j
 */
//-----------------------------------------------------------------------------
BOOL GMEVENT_DebugConStart(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys;
	EV_ACTING_WORK *eaw;
	
	fsys = FieldEvent_GetFieldSysWork(event);
	eaw = FieldEvent_GetSpecialWork(event);

	switch (eaw->seq) {
	case 0:
		EventCmd_FieldFadeOut(event);
		eaw->seq++;
		break;
	case 1:
		EventCmd_DebugConProc(event, eaw->debug_mode, eaw->rank, eaw->type);
		eaw->seq++;
		break;
	case 2:
		EventCmd_FieldFadeIn(event);
		eaw->seq++;
		break;
	case 3:
		sys_FreeMemoryEz(eaw);
		return TRUE;
	}
	return FALSE;
}

static void EventCmd_DebugConProc(GMEVENT_CONTROL * event, int debug_mode, int rank, int type)
{
	EV_SUB_ACTING_WORK *esaw = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(EV_SUB_ACTING_WORK));
	MI_CpuClear8(esaw, sizeof(EV_SUB_ACTING_WORK));
	esaw->debug_mode = debug_mode;
	esaw->rank = rank;
	esaw->type = type;
	FieldEvent_Call(event, GMEVENT_Sub_DebugCon, esaw);
}

static BOOL GMEVENT_Sub_DebugCon(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * fsys = FieldEvent_GetFieldSysWork(event);
	EV_SUB_ACTING_WORK *esaw = FieldEvent_GetSpecialWork(event);
	
	switch (esaw->seq) {
	case 0:
		EventCmd_FinishFieldMap(event);
		esaw->seq ++;
		break;
	case 1:
		//��check	����������������Ȃ��̂ŁA�Ƃ肠���������ŃR���e�X�g�S�̃q�[�v�쐬
		{
			POKEMON_PARAM * poke;
			STRBUF *myname;
			const MYSTATUS *my_status;
			CONTEST_INIT_DATA cid;
			
			poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(fsys->savedata), 0);
			my_status = SaveData_GetMyStatus(GameSystem_GetSaveData(fsys));
			myname = MyStatus_CreateNameString(my_status, HEAPID_WORLD);
			
			cid.type = esaw->type;//gf_rand() % CONTYPE_MAX;//CONTYPE_STYLE;
			cid.rank = esaw->rank;//CONRANK_NORMAL;
			cid.mode = CONMODE_GRAND;
			cid.my_pp = poke;
			cid.player_name_str = myname;
			cid.my_status = my_status;
			cid.imc_save = SaveData_GetImcSaveData(fsys->savedata);
			cid.config = SaveData_GetConfig(fsys->savedata);
			cid.sv = fsys->savedata;
			esaw->consys = Contest_SystemCreate(&cid);
			
			STRBUF_Delete(myname);
		}
		
		esaw->seq ++;
		break;
	case 2:		//���ꂼ��̕�����Ăяo��
		switch(esaw->debug_mode){
		case DEBUG_CON_ACTIN:
			EventCmd_CallSubProc(event, &ContestActinProcData, esaw->consys);
			esaw->seq++;
			break;
		case DEBUG_CON_VISUAL:
			//�f�o�b�O���[�h�ł͎����̃N���b�v�f�[�^�����Ȃ��̂ŁA�����ŃZ�b�g
			ImcSaveData_SetContestPokeData_Easy(esaw->consys->c_game.imc_data[0], 
				esaw->consys->c_game.pp[0], -1);
			ImcSaveData_SetContestBgId(esaw->consys->c_game.imc_data[0], 0);
			ImcSaveData_SetContestRank(esaw->consys->c_game.imc_data[0], 
				esaw->consys->c_game.rank);

			EventCmd_CallSubProc(event, &ContestVisualProcData, esaw->consys);
			esaw->seq++;
			break;
		case DEBUG_CON_CLIP_VISUAL:
			switch(esaw->local_seq){
			case 0:
				Contest_ImageClipInitDataCreate(esaw->consys);
				ContestDebug_ImageClipDebugAccessorySet(esaw->consys);	//�f�o�b�O�f�[�^�Z�b�g
				EventCmd_CallSubProc(event, &IMC_SYS_Proc, esaw->consys->icpw);
				esaw->local_seq++;
				break;
			case 1:
				ContestDebug_ImageClipDebugAccessoryFree(esaw->consys);	//�f�o�b�O�f�[�^�J��
				Contest_ImageClipInitDataFree(esaw->consys);
				esaw->local_seq++;
				break;
			default:
				EventCmd_CallSubProc(event, &ContestVisualProcData, esaw->consys);
				esaw->local_seq = 0;
				esaw->seq++;
				break;
			}
			break;
		case DEBUG_CON_DANCE:
			//�f�o�b�O���[�h�ł͎����̃N���b�v�f�[�^�����Ȃ��̂ŁA�����ŃZ�b�g
			ImcSaveData_SetContestPokeData_Easy(esaw->consys->c_game.imc_data[0], 
				esaw->consys->c_game.pp[0], -1);
			ImcSaveData_SetContestBgId(esaw->consys->c_game.imc_data[0], 0);
			ImcSaveData_SetContestRank(esaw->consys->c_game.imc_data[0], 
				esaw->consys->c_game.rank);

			EventCmd_CallSubProc(event, &ContestDanceProcData, esaw->consys);
			esaw->seq++;
			break;
		case DEBUG_CON_RESULT:
			EventCmd_CallSubProc(event, &ContestResultProcData, esaw->consys);
			esaw->seq++;
			break;
		}
		break;
	case 3:
		//��check�@�o�������������Ȃ��̂ŁA�Ƃ肠���������ŃR���e�X�g�S�̃q�[�v���
		Contest_SystemExit(esaw->consys);
		
		EventCmd_StartFieldMap(event);
		esaw->seq ++;
		break;
	case 4:
		sys_FreeMemoryEz(esaw);
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �C���[�W�N���b�v�f�o�b�O�p�ɃA�N�Z�T���[��S�ăZ�b�g
 *
 * @param   consys		�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 *
 * Contest_ImageClipInitDataCreate�̊֐���ɌĂяo���Ă�������
 */
//--------------------------------------------------------------
static void ContestDebug_ImageClipDebugAccessorySet(CONTEST_SYSTEM *consys)
{
	IMC_CONTEST_PROC_WORK *icpw;
	IMC_ITEM_SAVEDATA *imcsave;
	
	GF_ASSERT(consys->icpw != NULL);
	icpw = consys->icpw;
	
	imcsave = ImcSaveData_ItemAllocWork(HEAPID_CONTEST);
	Debug_ImcSaveData_SetItemData(imcsave);

	icpw->cp_imc_item = imcsave;
}

//--------------------------------------------------------------
/**
 * @brief   �f�o�b�O�p�ɃZ�b�g�����A�N�Z�T���̈���J��
 *
 * @param   consys		�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 *
 * Contest_ImageClipInitDataFree�֐����ĂԂ�����ɂ���Ńf�o�b�O�̈�̉�������Ă�������
 */
//--------------------------------------------------------------
static void ContestDebug_ImageClipDebugAccessoryFree(CONTEST_SYSTEM *consys)
{
	IMC_CONTEST_PROC_WORK *icpw;
	IMC_ITEM_SAVEDATA *imcsave;
	icpw = consys->icpw;
	imcsave = (void*)icpw->cp_imc_item;
	sys_FreeMemoryEz(imcsave);
}
#endif	//DEBUG_MATSU_CONTEST

#endif	// PM_DEBUG +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
