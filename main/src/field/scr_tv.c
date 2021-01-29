//============================================================================================
/**
 * @file	scr_tv.c
 * @brief	�X�N���v�g�R�}���h�F�e���r�֘A
 * @author	tamada GAME FREAK inc.
 * @date	2006.05.22
 */
//============================================================================================

#include "common.h"
#include "script.h"
#include "fieldsys.h"
#include "fieldmap_work.h"
#include "field_subproc.h"

#include "tv.h"
#include "tv_topic.h"
#include "field/tvtopic_extern.h"
#include "scrcmd_def.h"

#include "savedata/mystatus.h"
#include "savedata/randomgroup.h"
#include "savedata/imageclip_data.h"
#include "savedata/record.h"

#include "ev_pokemon.h"

#include "msgdata\msg.naix"			//NARC_msg_??_dat
#include "msgdata\msg_tv_interview.h"

#include "tv_define.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"
#include "communication/comm_info.h"

//============================================================================================
//============================================================================================
#include "scr_tv.h"

//============================================================================================
//
//
//		�X�N���v�g�R�}���h�F�e���r��������
//
//
//============================================================================================
//-----------------------------------------------------------------------------
/**
 * @brief	�e���r��������
 */
//-----------------------------------------------------------------------------
BOOL EvCmdBroadcastTV(VM_MACHINE * core)
{

	switch (VMGetU16(core)) {

	case TV_CMD_GET_STATUS:				//��Ԏ擾
		{
			u16 * ret_wk = VMGetWork(core);
			*ret_wk = TV_GetBroadcastStatus(core->fsys);
		}
		break;

	case TV_CMD_GET_PROG_MSG:			//���܂蕶��̃Z�b�g
		{
			u16 id = VMGetU16(core);
			u16 * arc_id = VMGetWork(core);
			u16 * msg_id = VMGetWork(core);
			*arc_id = NARC_msg_tv_program_dat;
			*msg_id = TV_GetProgramMsg(core->fsys, id);

		}
		break;
	case TV_CMD_GET_MSG:				//���b�Z�[�W�f�[�^����
		{
			WORDSET** wordset = GetEvScriptWorkMemberAdrs( core->fsys, ID_EVSCR_WORDSET );
			u16 idx = VMGetWorkValue(core);
			u16 * arc_id = VMGetWork(core);
			u16 * msg_id = VMGetWork(core);
			TV_GetProgramID(core->fsys, *wordset, idx, arc_id, msg_id);
		}
		break;

	case TV_CMD_SET_END_FLAG:			//�I���t���O�Z�b�g
		TV_SetEndFlag(core->fsys);
		break;

	case TV_CMD_GET_CM_MSG:				//�R�}�[�V�������b�Z�[�W�擾
		{
			u16 * arc_id = VMGetWork(core);
			u16 * msg_id = VMGetWork(core);
			*arc_id = NARC_msg_tvcm_dat;
			*msg_id = TVCM_ChooseID(core->fsys);
		}
		break;
	case TV_CMD_GET_DATA:				//�w��f�[�^�擾
		{
			u16 mine = VMGetWorkValue(core);
			u16 see = VMGetWorkValue(core);
			u16 * ret_wk = VMGetWork(core);
			*ret_wk = TV_ChooseBroadcastTopic(core->fsys, mine, see);
		}
		break;
	case TV_CMD_GET_DATA_TOTAL:			//�w��f�[�^�擾�i�S�p�^�[���j
		{
			u16 * ret_wk = VMGetWork(core);
			*ret_wk = TV_ChooseBroadcastTopicTotal(core->fsys);
		}
		break;
	}

	return 0;
}

//============================================================================================
//
//
//		�X�N���v�g�R�}���h�F�g�s�b�N����
//
//
//============================================================================================
//-----------------------------------------------------------------------------
/**
 * @brief	�g�s�b�N�����F�B���A�C�e�������I
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVEntryWatchHideItem(VM_MACHINE * core)
{
	TVTOPIC_Entry_Watch_HideItem( core->fsys, VMGetWorkValue(core) );
	return 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief	�g�s�b�N�����F�������f
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVEntryWatchChangeName(VM_MACHINE * core)
{
	POKEPARTY * party = SaveData_GetTemotiPokemon(core->fsys->savedata);
	POKEMON_PARAM * pp = PokeParty_GetMemberPointer(party, VMGetWorkValue(core));
	TVTOPIC_Entry_Watch_ChangeName(core->fsys, pp);
	return 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief	�g�s�b�N�����F�I�X�����I���X�����I
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVEntryWatchOsuMesu(VM_MACHINE * core)
{
	TVTOPIC_Entry_Watch_OsuMesu(core->fsys);
	return 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief	�g�s�b�N�����F�ʑ��w��
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVEntryWatchVilla(VM_MACHINE * core)
{
	TVTOPIC_Entry_Watch_Villa(core->fsys);
	return 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief	�g�s�b�N�����F�ʑ����z
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVEntryWatchReconstruction(VM_MACHINE * core)
{
	u16 no = VMGetWorkValue(core);
	TVTOPIC_Entry_Watch_Reconstruction(core->fsys, no);
	return 0;
}


//============================================================================================
//
//
//		�X�N���v�g�R�}���h�F�e���r�C���^�r���[
//
//
//============================================================================================
static int GetInterviewMsg(int id, FIELDSYS_WORK * fsys, WORDSET * ws);
static void EntryInterviewTopic(FIELDSYS_WORK * fsys, int id, u16 ans1, u16 ans2);
static BOOL CheckInterviewReady(FIELDSYS_WORK * fsys, int id);

typedef void (*INTERVIEW_ENTRY_FUNC)(FIELDSYS_WORK * fsys, PMS_WORD word);
typedef void (*INTERVIEW_MSG_FUNC)(FIELDSYS_WORK * fsys, WORDSET * ws);
typedef BOOL (*INTERVIEW_CHECK_FUNC)(FIELDSYS_WORK * fsys);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
typedef struct {
	INTERVIEW_ENTRY_FUNC entry_func;
	INTERVIEW_MSG_FUNC msg_func;
	INTERVIEW_CHECK_FUNC check_func;
	u32 msg_id;
}QANDA_DATA;

static const QANDA_DATA QandAData[TVTOPIC_INTERVIEW_TYPE_MAX - 1];
//-----------------------------------------------------------------------------
/**
 * @brief	�e���r�F�C���^�r���[����
 *
 * tv_interview.ev	TV_INTERVIEW_MSG
 *					TV_INTERVIEW_ENTRY
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVInterview(VM_MACHINE * core)
{
	WORDSET ** wordset;
	wordset = GetEvScriptWorkMemberAdrs( core->fsys, ID_EVSCR_WORDSET );
	switch (VMGetU16(core)) {

	/*�C���^�r���[�̓������b�Z�[�W*/
	case TV_INTERVIEW_CMD_GET_MSG:
		{
			int id;
			u16 * arc_id;
			u16 * msg_id;

			id = VMGetWorkValue(core);
			arc_id = VMGetWork(core);
			msg_id = VMGetWork(core);
			*arc_id = NARC_msg_tv_interview_dat;
			*msg_id = GetInterviewMsg(id, core->fsys, *wordset);
		}
		break;

	/*�C���^�r���[���ʓo�^�����Ăяo��*/
	case TV_INTERVIEW_CMD_SET_TOPIC:
		{
			u16 id;
			u16 ans_wk1, ans_wk2;
			RECORD * rec = SaveData_GetRecord(core->fsys->savedata);
			id = VMGetWorkValue(core);
			ans_wk1 = VMGetWorkValue(core);
			ans_wk2 = VMGetWorkValue(core);

			//ans_wk2�͎g�p����Ă��Ȃ�
			EntryInterviewTopic(core->fsys, id, ans_wk1, ans_wk2);
			RECORD_Score_Add(rec, SCORE_ID_INTERVIEW);
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief	�e���r�F�C���^�r���[�\���ǂ����̃`�F�b�N
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVInterviewerCheck(VM_MACHINE * core)
{
	u16 id;
	u16 * ret_wk;
	id = VMGetWorkValue(core);
	ret_wk = VMGetWork(core);
	*ret_wk = CheckInterviewReady(core->fsys, id);
	return 0;
}

//-----------------------------------------------------------------------------
/**
 * @brief	�e���r�F�A����������p�f�[�^�o�^
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVEntryParkInfo(VM_MACHINE * core)
{
	TV_WORK * tvwk = SaveData_GetTvWork(core->fsys->savedata);

	switch (VMGetU16(core)) {
	case TV_ENTRYPARK_CMD_INIT:
		{
			POKEPARTY * party = SaveData_GetTemotiPokemon(core->fsys->savedata);
			POKEMON_PARAM * pp = PokeParty_GetMemberPointer(party, VMGetWorkValue(core));
			TVTOPIC_ParkTemp_Init(tvwk, pp, HEAPID_FIELD);
		}
		break;
	case TV_ENTRYPARK_CMD_ITEM:
		TVTOPIC_ParkTemp_SetKinomi(tvwk, VMGetWorkValue(core));
		break;
	case TV_ENTRYPARK_CMD_ACCE:
		TVTOPIC_ParkTemp_SetAccessories(tvwk, VMGetWorkValue(core));
		break;
	}
	return 0;
}


//============================================================================================
//============================================================================================

//--------------------------------------------------------------------
/**
 * @brief	����^�g�s�b�N���������̌Ăяo��
 * @param	id		�g�s�b�N�w��ID
 * @param	ans1	����̓����iPMS_WORD of u16)
 * @param	ans2	����A���݂͎g�p���Ă��Ȃ�
 */
//--------------------------------------------------------------------
static void EntryInterviewTopic(FIELDSYS_WORK * fsys, int id, u16 ans1, u16 ans2)
{
	INTERVIEW_ENTRY_FUNC func;
	func = QandAData[id - 1].entry_func;
	if (func != NULL) {
		func(fsys, ans1);
	}
}

//--------------------------------------------------------------------
/**
 * @brief	�C���^�r���[�J�n���b�Z�[�W�̐���
 * @param	fsys	�t�B�[���h���䃏�[�N�ւ̃|�C���^
 * @param	ws		WORDSET�ւ̃|�C���^
 * @return	int		MSG��ID
 *
 * ���b�Z�[�WID�ɂ���ẮA�����������̒P��⊮���s��
 */
//--------------------------------------------------------------------
static int GetInterviewMsg(int id, FIELDSYS_WORK * fsys, WORDSET * ws)
{
	INTERVIEW_MSG_FUNC func;
	func = QandAData[id - 1].msg_func;
	if (func != NULL) {
		func (fsys, ws);
	}
	return QandAData[id - 1].msg_id;
	//return msg_tv_interview_01 + id - 1;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
static BOOL CheckInterviewReady(FIELDSYS_WORK * fsys, int id)
{
	INTERVIEW_CHECK_FUNC func;
	TV_WORK *tvwk = SaveData_GetTvWork(fsys->savedata);

	if (TVWORK_EnableMakeTopic(tvwk, TVTOPICTYPE_INTERVIEW, id) == FALSE) {
		//�g�s�b�N�����Ԃ��Ă��č쐬�ł��Ȃ��ꍇ
		return FALSE;
	}
	func = QandAData[id - 1].check_func;
	if (func == NULL) {
		//�`�F�b�N�֐����Ȃ��ꍇ�A��������OK
		return TRUE;
	}
	return func(fsys);
}

//============================================================================================
//
//
//
//============================================================================================


#include "gflib/strbuf_family.h"
//--------------------------------------------------------------------
//--------------------------------------------------------------------
static void WORDSET_RegisterSTRCODE(WORDSET * ws, int buf_id,
		const STRCODE * str, int sex, int region, int single)
{
	STRBUF * strbuf = STRBUF_Create(64, HEAPID_FIELD);
	STRBUF_SetStringCode(strbuf, str);
	WORDSET_RegisterWord(ws, buf_id, strbuf, sex, single, region);
	STRBUF_Delete(strbuf);
}

//-----------------------------------------------------------------------------
//	(�^�}�S��������)�擪�̃|�P��������buf0�ɃZ�b�g
//-----------------------------------------------------------------------------
static void Interview04(FIELDSYS_WORK * fsys, WORDSET * ws)
{
	POKEPARTY * party = SaveData_GetTemotiPokemon(fsys->savedata);
	POKEMON_PARAM * pp = EvPoke_GetPokemonTop(party);
	WORDSET_RegisterPokeMonsName(ws, 0, PPPPointerGet(pp));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void Interview06(FIELDSYS_WORK * fsys, WORDSET * ws)
{
	int id;
	id = Poketch_GetRunningApplication(fsys->fldmap->poketch);
	WORDSET_RegisterPoketchAppName(ws, 0, id);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void Interview13(FIELDSYS_WORK * fsys, WORDSET * ws)
{
	STRCODE monsname[MONS_NAME_SIZE + EOM_SIZE];
	TV_WORK * tvwk = SaveData_GetTvWork(fsys->savedata);
	TOPICTEMP_PARK * ptemp = TVWORK_GetParkTemp(tvwk);
	MSGDAT_MonsNameGet(ptemp->monsno, HEAPID_FIELD, monsname);
	WORDSET_RegisterSTRCODE(ws, 0, monsname, PM_MALE, PM_LANG, TRUE);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void Interview14(FIELDSYS_WORK * fsys, WORDSET * ws)
{
	STRCODE monsname[MONS_NAME_SIZE + EOM_SIZE];
	TV_WORK * tvwk = SaveData_GetTvWork(fsys->savedata);
	TOPICTEMP_STAGE * ptemp = TVWORK_GetStageTemp(tvwk);
	MSGDAT_MonsNameGet(ptemp->monsno, HEAPID_FIELD, monsname);
	WORDSET_RegisterSTRCODE(ws, 0, monsname, PM_MALE, PM_LANG, TRUE);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static void Interview19(FIELDSYS_WORK * fsys, WORDSET * ws)
{
	STRBUF * strbuf;
	TV_WORK * tvwk = SaveData_GetTvWork(fsys->savedata);
	TOPICTEMP_FRIEND * ptemp = TVWORK_GetFriendTemp(tvwk);

	strbuf = STRBUF_Create(64, HEAPID_FIELD);

	STRBUF_SetStringCode(strbuf, ptemp->friend_name);
	WORDSET_RegisterWord(ws, 0, strbuf, ptemp->friend_sex, TRUE, PM_LANG);

	STRBUF_Delete(strbuf);
}

//-----------------------------------------------------------------------------
//	�o�g���^���[�p�󋵕ێ����[�N�̃f�[�^���݃t���O���`�F�b�N
//-----------------------------------------------------------------------------
static BOOL InterviewCheck02(FIELDSYS_WORK * fsys)
{
	TOPICTEMP_BTOWER * bt = TVWORK_GetBTowerTemp(SaveData_GetTvWork(fsys->savedata));
	return bt->exist_flag;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static BOOL InterviewCheck06(FIELDSYS_WORK * fsys)
{
	POKETCH_DATA* poketch_data = SaveData_GetPoketchData( fsys->savedata );
	return PoketchData_GetPoketchGetFlag( poketch_data );

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static BOOL InterviewCheck07(FIELDSYS_WORK * fsys)
{
	TOPICTEMP_CONTEST * ct = TVWORK_GetContestTemp(SaveData_GetTvWork(fsys->savedata));
	return ct->exist_flag;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static BOOL InterviewCheck09(FIELDSYS_WORK * fsys)
{
	IMC_SAVEDATA* iw = SaveData_GetImcSaveData(fsys->savedata);
	return ImcSaveData_CheckTelevisionSaveData(iw, 0);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static BOOL InterviewCheck11(FIELDSYS_WORK * fsys)
{
	TOPICTEMP_PORUTO * poruto = TVWORK_GetPorutoTemp(SaveData_GetTvWork(fsys->savedata));
	return poruto->exist_flag;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static BOOL InterviewCheck13(FIELDSYS_WORK * fsys)
{
	TOPICTEMP_PARK * park = TVWORK_GetParkTemp(SaveData_GetTvWork(fsys->savedata));
	return park->exist_flag;
}

//-----------------------------------------------------------------------------
//	�o�g���X�e�[�W�p�󋵕ێ����[�N�̃f�[�^���݃t���O���`�F�b�N
//-----------------------------------------------------------------------------
static BOOL InterviewCheck14(FIELDSYS_WORK * fsys)
{
	TOPICTEMP_STAGE * stage = TVWORK_GetStageTemp(SaveData_GetTvWork(fsys->savedata));
	return stage->exist_flag;
}

//-----------------------------------------------------------------------------
//	�o�g���t�����e�B�A�p�󋵕ێ����[�N�̃f�[�^���݃t���O���`�F�b�N
//-----------------------------------------------------------------------------
static BOOL InterviewCheck19(FIELDSYS_WORK * fsys)
{
	TOPICTEMP_FRIEND * friend = TVWORK_GetFriendTemp(SaveData_GetTvWork(fsys->savedata));
	return friend->exist_flag;
}


//-----------------------------------------------------------------------------
//	����^�̃f�[�^
//		entry_func			�g�s�b�N��������(src/field/tv_topic.c)
//		msg_func			����msg�ɕK�v��wordset����
//		check_func			�f�[�^���݂��邩�A�t���O�����Ă��邩�Ȃ�
//		msg_id				����msg_id
//-----------------------------------------------------------------------------
static const QANDA_DATA QandAData[TVTOPIC_INTERVIEW_TYPE_MAX - 1] = {
	{//TVTOPIC_INTERVIEW_GYM_BADGE = 1,///<1:�W�����e�I	1	�W���̂���V�e�B	�W�����[�_�[��|��������
		NULL,
		NULL,
		NULL,
		msg_tv_interview_01,
	},
	{//TVTOPIC_INTERVIEW_BTOWER,		///<2:����I�o�g���^���[	1	�o�g���^���[	����I����
		TVTOPIC_Entry_QandA_BTower,
		NULL,
		InterviewCheck02,
		msg_tv_interview_02,
	},
	{//TVTOPIC_INTERVIEW_BFACTORY,		///<3:����I�t�@�N�g���[	1	�o�g���t�@�N�g���[	����I����
		NULL,
		NULL,
		NULL,
		msg_tv_interview_03,
	},
	{//TVTOPIC_INTERVIEW_POKEMON,		///<4:���Ȃ��̃|�P����	1	�|�P������D���N���u	
		TVTOPIC_Entry_QandA_Pokemon,
		Interview04,
		NULL,
		msg_tv_interview_04,
	},
	{//TVTOPIC_INTERVIEW_ITEM,			///<5:�������߂̂ǂ����I	2	�t�����h���B�V���b�v	
		NULL,
		NULL,
		NULL,
		msg_tv_interview_05,
	},
	{//TVTOPIC_INTERVIEW_POKETCH,		///<6:���C�ɓ���̃|�P�b�`	2	�J���p�j�[	
		TVTOPIC_Entry_QandA_Poketch,
		Interview06,
		InterviewCheck06,
		msg_tv_interview_06,
	},
	{//TVTOPIC_INTERVIEW_CONTEST,		///<7:����I�R���e�X�g		�R���e�X�g���	�R���e�X�g�I����
		TVTOPIC_Entry_QandA_Contest,
		NULL,
		InterviewCheck07,
		msg_tv_interview_07,
	},
	{//TVTOPIC_INTERVIEW_TATOE,		///<8:�|�P�����ɗႦ����H	2	��D���N���u	
		NULL,
		NULL,
		NULL,
		msg_tv_interview_08,
	},
	{//TVTOPIC_INTERVIEW_CLIP,			///<9:�|�P�����N���b�v�I	2	�N���b�v�����ꏊ	�N���b�v�����������
		TVTOPIC_Entry_QandA_Clip,
		NULL,
		InterviewCheck09,
		msg_tv_interview_09,
	},
	{//TVTOPIC_INTERVIEW_CHARACTOR,	///<10:���i�f�f�I	2		
		TVTOPIC_Entry_QandA_Character,
		NULL,
		NULL,
		msg_tv_interview_10,
	},
	{//TVTOPIC_INTERVIEW_PORUTO,		///<11:�|���g����		�|���g�����ꏊ	�|���g���������
		TVTOPIC_Entry_QandA_Poruto,
		NULL,
		InterviewCheck11,
		msg_tv_interview_11,
	},
	{//TVTOPIC_INTERVIEW_KOUKAN,		///<12:���������|�P����	1		
		NULL,
		NULL,
		NULL,
		msg_tv_interview_12,
	},
	{//TVTOPIC_INTERVIEW_TUREARUKI,	///<13:�A������𖞋i�I		���R�����Q�[�g	�A�������
		TVTOPIC_Entry_QandA_Park,
		Interview13,
		InterviewCheck13,
		msg_tv_interview_13,
	},
	{//TVTOPIC_INTERVIEW_STAGE,		///<14:�o�g���X�e�[�W
		TVTOPIC_Entry_QandA_Stage,
		Interview14,
		InterviewCheck14,
		msg_tv_interview_14,
	},
	{//TVTOPIC_INTERVIEW_STRANGE1,	///<15:�ւ�ȃA���P�[�g
		TVTOPIC_Entry_QandA_Strange1,
		NULL,
		NULL,
		msg_tv_interview_15,
	},
	{//TVTOPIC_INTERVIEW_STRANGE2,	///<16:�ւ�ȃA���P�[�g
		TVTOPIC_Entry_QandA_Strange2,
		NULL,
		NULL,
		msg_tv_interview_16,
	},
	{//TVTOPIC_INTERVIEW_STRANGE3,	///<17:�ւ�ȃA���P�[�g
		TVTOPIC_Entry_QandA_Strange3,
		NULL,
		NULL,
		msg_tv_interview_17,
	},
	{//TVTOPIC_INTERVIEW_STRANGE4,	///<18:�ւ�ȃA���P�[�g
		TVTOPIC_Entry_QandA_Strange4,
		NULL,
		NULL,
		msg_tv_interview_18,
	},
	{//TVTOPIC_INTERVIEW_FRIEND,	///<19:�t�����e�B�A�Œ��ǂ�
		TVTOPIC_Entry_QandA_Friend,
		Interview19,
		InterviewCheck19,
		msg_tv_interview_19,
	},
};

//-----------------------------------------------------------------------------
/**
 * @brief	�e���r�F�󋵕ێ����[�N�̃T�t�@������߂܂��������擾
 */
//-----------------------------------------------------------------------------
BOOL EvCmdTVSafariTempCaptureCountGet(VM_MACHINE * core);
BOOL EvCmdTVSafariTempCaptureCountGet(VM_MACHINE * core)
{
	TV_WORK* tvwk;
	TOPICTEMP_SAFARI* psaf;
	u16* ret_wk	= VMGetWork(core);

	tvwk = SaveData_GetTvWork( core->fsys->savedata );
	psaf = TVWORK_GetSafariTemp(tvwk);

	*ret_wk = psaf->capture_count;
	OS_Printf( "capture_count = %d\n", psaf->capture_count );
	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�����e�B�A�Œ��ǂ��C���^�r���A�[�̍��W���擾
 *
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 *	@return "0"
 */
//-----------------------------------------------------------------------------
#include "frontier/frontier_def.h"

BOOL EvCmdTVFrFriendSxyGet( VM_MACHINE * core );
BOOL EvCmdTVFrFriendSxyGet( VM_MACHINE * core )
{
	TV_WORK* tvwk;
	TOPICTEMP_FRIEND* pff;
	u16* ret_wk1 = VMGetWork(core);				//X
	u16* ret_wk2 = VMGetWork(core);				//Z
	u16* ret_wk3 = VMGetWork(core);				//DIR
	u16* ret_wk4 = VMGetWork(core);				//MV
	
	tvwk = SaveData_GetTvWork( core->fsys->savedata );
	pff = TVWORK_GetFriendTemp(tvwk);
	
	switch( pff->bf_no ){

	case FRONTIER_NO_STAGE:
		*ret_wk1 = 25;
		*ret_wk2 = 36;
		*ret_wk3 = DIR_UP;
		*ret_wk4 = MV_UP;
		break;

	case FRONTIER_NO_CASTLE:
		*ret_wk1 = 37;
		*ret_wk2 = 61;
		*ret_wk3 = DIR_DOWN;
		*ret_wk4 = MV_DOWN;
		break;

	case FRONTIER_NO_ROULETTE:
		*ret_wk1 = 59;
		*ret_wk2 = 61;
		*ret_wk3 = DIR_DOWN;
		*ret_wk4 = MV_DOWN;
		break;

	case FRONTIER_NO_FACTORY_LV50:
	case FRONTIER_NO_FACTORY_LV100:
		*ret_wk1 = 72;
		*ret_wk2 = 36;
		*ret_wk3 = DIR_UP;
		*ret_wk4 = MV_UP;
		break;

	case FRONTIER_NO_TOWER:
	default:
		*ret_wk1 = 47;
		*ret_wk2 = 21;
		*ret_wk3 = DIR_UP;
		*ret_wk4 = MV_UP;
		break;

	};

	return 0;
}


