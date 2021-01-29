//==============================================================================
/**
 * @file	frontier_scr.c
 * @brief	�t�����e�B�A�p�X�N���v�g����
 * @author	matsuda
 * @date	2007.03.28(��)
 */
//==============================================================================
#include "common.h"
#include "poketool\poke_tool.h"	// POKEMON_PASO_PARAM �Q�Ƃ̂���
#include "battle/battle_common.h"
#include "system/clact_tool.h"
#include "system/arc_tool.h"

#include "frontier_types.h"
#include "fs_usescript.h"
#include "frontier_scr.h"
#include "fss_task.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier_objmove.h"
#include "frontier_def.h"


//============================================================================================
//
//	��`
//
//============================================================================================
#define SCR_MSG_BUF_SIZE	(1024)					//���b�Z�[�W�o�b�t�@�T�C�Y

//typedef void (*fsysFunc)(FIELDSYS_WORK* fsys);		//�֐��|�C���^�^

enum{
	WORDSET_SCRIPT_SETNUM = 8,		//�f�t�H���g�o�b�t�@��
	WORDSET_SCRIPT_BUFLEN = 64,		//�f�t�H���g�o�b�t�@���i�������j
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static BOOL FSS_TaskPointerSet(FSS_PTR fss, FSS_TASK *fss_task);
static void FSS_ScrTaskDel(FSS_PTR fss, FSS_TASK *core);
static void FssScriptWork_Init(FSS_PTR fss, u16 scene_id, TARGET_OBJ_PTR obj, void* ret_wk );
static void FSS_SetScriptData(
	FSS_CODE **code, MSGDATA_MANAGER **msgman, int scene_id, int heap_id);
static void FSS_EventDataIDJump( FSS_TASK * core, int event_id );


//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���쐬
 *
 * @param   heap_id		�X�N���v�g�V�X�e�������Ŏg�p����q�[�vID
 *
 * @retval  �쐬���ꂽ�X�N���v�g�V�X�e���̃|�C���^
 */
//--------------------------------------------------------------
FSS_PTR FSS_SystemCreate(FMAIN_PTR fmain, int heap_id, int scene_id)
{
	FSS_PTR fss;
	
	fss = sys_AllocMemory(heap_id, sizeof(FS_SYSTEM));
	MI_CpuClear8(fss, sizeof(FS_SYSTEM));

	fss->fmain = fmain;
	fss->heap_id = heap_id;
	fss->def_scene_id = scene_id;

	//�W���X�N���v�g�f�[�^�A�W�����b�Z�[�W�f�[�^�ǂݍ���
	FSS_SetScriptData(&fss->def_script, &fss->def_msgman, scene_id, heap_id);

	fss->wordset = WORDSET_CreateEx(WORDSET_SCRIPT_SETNUM, WORDSET_SCRIPT_BUFLEN, heap_id);
	fss->msg_buf = STRBUF_Create(SCR_MSG_BUF_SIZE, heap_id);
	fss->tmp_buf = STRBUF_Create(SCR_MSG_BUF_SIZE, heap_id);

	//��check�@�܂����쐬
	FssScriptWork_Init(fss, scene_id, NULL, NULL);		//�����ݒ�

	return fss;
}

#include "../field/scr_tool.h"
//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g���䃁�C��
 *
 * @param	event		GMEVENT_CONTROL�^
 *
 * @retval	"FALSE = �X�N���v�g���s��"
 * @retval	"TRUE = �X�N���v�g�͓����Ă��Ȃ�(���z�}�V���̐��O)"
 */
//--------------------------------------------------------------
//static BOOL FSEVENT_ControlScript(GMEVENT_CONTROL * event)
BOOL FSS_ScriptMain(FSS_PTR fss)
{
	int i;
	FSS_TASK *core;

	if(fss->fss_task_count == 0){
		return TRUE;
	}
	
#ifdef PM_DEBUG
#ifdef DEBUG_FRONTIER_LOOP
	if( sys.cont & PAD_BUTTON_START ){
		debug_frontier_key_set = 1;
	}
	
	if( debug_frontier_key_set ){
		sys.cont |= PAD_BUTTON_A;
		sys.trg |= PAD_BUTTON_A;
	}
#endif
#endif

	switch(fss->seq){
	case 0:
		//���z�}�V���R���g���[��
		for(i = 0; i < FSS_TASK_MAX; i++){
			core = fss->fss_task[i];
			if(core != NULL){
				if( FSST_Control(core) == FALSE ){				//���䃁�C��
					FSS_ScrTaskDel(fss, core);					//�X�N���v�g�폜
					fss->fss_task[i] = NULL;
					fss->fss_task_count--;
				}
			}
		}

		//OBJ����R�[�h���s
		FSS_ObjMoveFuncAll(fss->fmain);
		
		break;
	
	default:
		break;
	};

	if(fss->fss_task_count == 0){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e�����
 * @param   fss		�X�N���v�g�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void FSS_SystemFree(FSS_PTR fss)
{
	GF_ASSERT(fss->fss_task_count == 0);	//���z�}�V���������Ă����Ԃł͏I���o���Ȃ�
	
	//�f�o�b�N����
	//debug_script_flag = 0;
//	fss->magic_no = 0;

	WORDSET_Delete( fss->wordset );
	STRBUF_Delete( fss->msg_buf );
	STRBUF_Delete( fss->tmp_buf );

	sys_FreeMemoryEz(fss->def_script);
	MSGMAN_Delete(fss->def_msgman);
	
	sys_FreeMemoryEz(fss);								//�X�N���v�g���[�N
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�^�X�N�쐬
 *
 * @param   fss			�X�N���v�g�V�X�e���ւ̃|�C���^
 * @param   scr_id		�X�N���v�gID(FSS_SCENEID_DEFAULT�̏ꍇ�͕W���X�N���v�g�g�p)
 * @param   event_id	�C�x���gID
 *
 * @retval  
 */
//--------------------------------------------------------------
void FSS_ScrTaskAdd(FSS_PTR fss, int scene_id, int event_id)
{
	FSS_TASK* core;

	core = sys_AllocMemory( fss->heap_id, sizeof(FSS_TASK) );
	MI_CpuClear8(core, sizeof(FSS_TASK));
	FSST_Init( core, FSSCmdTable, FSSCmdTableMax);

	core->fss = fss;
	
	if(scene_id == FSS_SCENEID_DEFAULT || fss->def_scene_id == scene_id){
		core->pScript = fss->def_script;
		core->msgman = fss->def_msgman;
	}
	else{
		//�X�N���v�g�f�[�^�A���b�Z�[�W�f�[�^�ǂݍ���
		FSS_SetScriptData(&core->pScript, &core->msgman, scene_id, fss->heap_id);
	}
	
	FSST_Start( core, core->pScript );				//���z�}�V���ɃR�[�h�ݒ�
	FSS_EventDataIDJump( core, event_id );
//	VM_SetWork( core, (void *)fsys->event );		//�R�}���h�ȂǂŎQ�Ƃ��郏�[�N�Z�b�g

	FSS_TaskPointerSet(fss, core);
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e�����[�N�ɐV�������z�}�V���̃|�C���^��ǉ�����
 *
 * @param   fss				�X�N���v�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   fss_task		�ǉ����鉼�z�}�V���̃|�C���^
 *
 * @retval  TRUE:����
 * @retval  FALSE:���s
 */
//--------------------------------------------------------------
static BOOL FSS_TaskPointerSet(FSS_PTR fss, FSS_TASK *fss_task)
{
	int i;

	for(i = 0; i < FSS_TASK_MAX; i++){
		if(fss->fss_task[i] == NULL){
			fss->fss_task[i] = fss_task;
			fss->fss_task_count++;
			return TRUE;
		}
	}
	
	GF_ASSERT(0 && "�^�X�N�����ɂ����ς�");
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g���ʍ폜����
 * @param   core		
 */
//--------------------------------------------------------------
static void FSS_ScrTaskDel(FSS_PTR fss, FSS_TASK *core)
{
	if(core->msgman != fss->def_msgman){
		MSGMAN_Delete( core->msgman );
	}
	if(core->pScript != fss->def_script){
		sys_FreeMemoryEz( core->pScript );			//�X�N���v�g�f�[�^
	}
	
	sys_FreeMemoryEz( core );
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���v�g���䃏�[�N�����ݒ�
 *
 * @param	fsys		FIELDSYS_WORK�^�̃|�C���^
 * @param	fss			�X�N���v�g�V�X�e�����[�N�ւ̃|�C���^
 * @param	scr_id		�X�N���v�gID
 * @param	obj			�b�������Ώ�OBJ�̃|�C���^
 * @param	ret_wk		�X�N���v�g���ʂ������郏�[�N�̃|�C���^
 *
 * @retval	"�X�N���v�g���䃏�[�N�̃A�h���X"
 */
//--------------------------------------------------------------
static void FssScriptWork_Init(FSS_PTR fss, u16 scene_id, TARGET_OBJ_PTR obj, void* ret_wk )
{
//	u16* objid = GetEvScriptWorkMemberAdrs_Sub( fss, ID_EVSCR_WK_TARGET_OBJID );

	//DIR_NOT = -1
//	fss->player_dir = Player_DirGet(fsys->player);	//�C�x���g�N�����̎�l���̌����Z�b�g
//	fss->target_obj = obj;							//�b�������Ώ�OBJ�̃|�C���^�Z�b�g
//	fss->script_id  = scr_id;						//���C���̃X�N���v�gID
//	fss->ret_script_wk = ret_wk;						//�X�N���v�g���ʂ������郏�[�N

	if( obj != NULL ){
//		*objid = FieldOBJ_OBJIDGet(obj);			//�b�������Ώ�OBJID�̃Z�b�g
	}
}

//--------------------------------------------------------------
/**
 * @brief   �]�[���X�N���v�g�f�[�^�Z�b�g
 *
 * @param   fss			
 * @param   core		
 * @param   scr_id		�X�N���v�gID
 */
//--------------------------------------------------------------
static void FSS_SetScriptData(FSS_CODE **code, MSGDATA_MANAGER **msgman, int scene_id, int heap_id)
{
	int scr_id, msg_id;
	
	scr_id = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_SCRID);
	msg_id = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MSGID);

	//�X�N���v�g�t�@�C�������[�h
	OS_TPrintf("scene = %d, scr = %d, msg = %d\n", scene_id, scr_id, msg_id);
	*code = ArchiveDataLoadMalloc(ARC_FRONTIER_SCRIPT, scr_id, heap_id);

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	*msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, msg_id, heap_id );
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���Ŏg�p���Ă���W�����b�Z�[�W�}�l�[�W����V�����V�[����
 * 			���b�Z�[�W�f�[�^�Ń}�l�[�W������蒼���āA�g�p���Ă��鉼�z�}�V����
 *			���b�Z�[�W�}�l�[�W���̃|�C���^��V�����ق��Őݒ肵�Ȃ���
 *
 * @param   fss					�X�N���v�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   new_scene_id		�V�����V�[����ID
 * @param   heap_id				�q�[�vID
 *
 * �W�����b�Z�[�W�}�l�[�W�����g�p���Ă��Ȃ����z�}�V���ɂ��Ă͉����s���܂���
 *
 * ���̊֐��̓X�N���v�g����MAP_CHANGE�ŃV�[��ID��ύX�����ꍇ�A���̃V�[���f�[�^�̃��b�Z�[�W��
 * �g���悤�ɂ���ׂɗp�ӂ��ꂽ���̂ł��B
 */
//--------------------------------------------------------------
void FSS_MsgManageSceneChange(FSS_PTR fss, int new_scene_id, int heap_id)
{
	int i;
	FSS_TASK *core;
	int now_msg_id, new_msg_id;
	MSGDATA_MANAGER *new_msgman;
	
	now_msg_id = FSS_SceneParamGet(fss->def_scene_id, FSS_SCENE_DATA_MSGID);
	new_msg_id = FSS_SceneParamGet(new_scene_id, FSS_SCENE_DATA_MSGID);
	if(now_msg_id == new_msg_id){
		return;
	}
	
	new_msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, new_msg_id, heap_id );
	
	for(i = 0; i < FSS_TASK_MAX; i++){
		core = fss->fss_task[i];
		if(core != NULL){
			if(core->msgman == fss->def_msgman){
				//�f�t�H���g�̃��b�Z�[�W�}�l�[�W�����g���Ă���Ȃ�V�����V�[����
				//���b�Z�[�W�}�l�[�W���ɐݒ肵�Ȃ���
				core->msgman = new_msgman;
			}
		}
	}
	
	MSGMAN_Delete(fss->def_msgman);
	fss->def_msgman = new_msgman;
}

//--------------------------------------------------------------
/**
 * @brief   param_work�̒l�����������m�ۂ��āA�����Ƀo�b�N�A�b�v���܂�
 *
 * @param   fss			
 * @param   heap_id		�q�[�vID
 *
 * @retval  �o�b�N�A�b�v�f�[�^�������Ă���o�b�t�@�̃|�C���^
 */
//--------------------------------------------------------------
FS_PARAM_WORK *FSS_ParamWorkBackup(FSS_PTR fss, int heap_id)
{
	FS_PARAM_WORK *pw;
	
	pw = sys_AllocMemory(heap_id, sizeof(FS_PARAM_WORK));
	*pw = fss->param_work;
	return pw;
}

//--------------------------------------------------------------
/**
 * @brief   FSS_ParamWorkBackup�֐��Ńo�b�N�A�b�v����param_work�𕜋A�����܂�
 *
 * @param   fss		
 * @param   pw		param_work�ւ̃|�C���^
 *
 * FSS_ParamWorkBackup�֐��ƃZ�b�g�Ŏg�p���邱�Ƃ��O��ɂȂ��Ă��܂��B
 * �֐�����pw�̓������������܂�
 */
//--------------------------------------------------------------
void FSS_ParamWorkRecover(FSS_PTR fss, FS_PARAM_WORK *pw)
{
	fss->param_work = *pw;
	sys_FreeMemoryEz(pw);
}

//--------------------------------------------------------------
/**
 * @brief   �C�x���gID�W�����v
 *
 * @param   core		
 * @param   event_id	�C�x���gID
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void FSS_EventDataIDJump( FSS_TASK * core, int event_id )
{
	core->PC += (event_id * 4);			//ID���i�߂�(adrs��long�Ȃ̂�*4)
	core->PC += FSSTGetU32( core );		//���x���I�t�Z�b�g���i�߂�
}

//--------------------------------------------------------------
/**
 * @brief   param_work�̃A�h���X���擾����
 *
 * @param   fss			
 * @param   work_no		���[�N�ԍ�
 *
 * @retval  ���[�N�̃A�h���X
 */
//--------------------------------------------------------------
u16 * FSS_ParamWorkAdrsGet(FSS_PTR fss, int work_no)
{
	return &fss->param_work.work[work_no];
}

//--------------------------------------------------------------
/**
 * @brief   fmap�̃|�C���^���擾����
 *
 * @param   fss		
 *
 * @retval  fmap�|�C���^
 */
//--------------------------------------------------------------
FMAP_PTR FSS_GetFMapAdrs(FSS_PTR fss)
{
	return Frontier_FMapAdrsGet(fss->fmain);
}

