//==============================================================================
/**
 * @file	frontier_main.c
 * @brief	�t�����e�B�A���C������
 * @author	matsuda
 * @date	2007.04.05(��)
 */
//==============================================================================
#include "common.h"
#include "system/pm_overlay.h"
#include "system/procsys.h"
#include "system/pmfprint.h"
#include "system/palanm.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
//#include "script.h"
#include "..\fielddata\script\connect_def.h"
#include "system/pm_overlay.h"

#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_matsu.h"

#include "frontier_scr.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"



//==============================================================================
//	
//==============================================================================
FS_EXTERN_OVERLAY(frontier_common);
FS_EXTERN_OVERLAY(factory);
FS_EXTERN_OVERLAY(wifi_2dmapsys);

//==============================================================================
//	�\���̒�`
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �Q�[����ʂ��\�����邽�߂ɕK�v�ȃV�X�e���f�[�^�ނ����\����
 *
 * �ʐM���ł��S�Ẵ}�V���Ōʂɍ쐬����܂�
 * �e�X�^�b�t�����ߎ�M���A���̃}�V���X�Ő����������̃��[�N�o�b�t�@��n�����ɂȂ�܂�
 */
//--------------------------------------------------------------
typedef struct _FRONTIER_MAIN_WORK{
	FRONTIER_EX_PARAM *ex_param;		///<�t�����e�B�A�Ăяo��������n�����f�[�^�ւ̃|�C���^
	
	PROC *sub_proc;		///<�T�u�v���b�N�̃|�C���^
	void *sub_proc_parent_work;	///<�T�u�v���b�N�Ăяo�����ɓn����ʃ|�C���^
	FS_SUBPROC_END_FUNC sub_proc_end_func;
	u8 sub_proc_auto_free;	///<�T�u�v���b�N�I�����Ƀ��[�N�������
	
	FSS_PTR fss;		///<�X�N���v�g�V�X�e���ւ̃|�C���^
	FMAP_PTR fmap;		///<�}�b�v�V�X�e���ւ̃|�C���^
	u8 seq;
	u8 map_status;		///<TRUE:�}�b�v�V�X�e�����N�����Ă���
	u8 map_change;		///<TRUE:�}�b�v�؂�ւ����N�G�X�g����
	u16 map_change_event_id;	///<�}�b�v�؂�ւ���ɋN������C�x���gID
	
	u8 finish_req;		///<TRUE:�I�����N�G�X�g
	
	//�L�������A�p�Ƀ��\�[�X��A�N�^�[�̓o�^�����o�b�N�A�b�v
	//�}�b�v�V�X�e�����j������Ă����͎c��悤�Ƀo�b�t�@�̓��C���ɁA
	//�풓�̈���������Ȃ��悤�Ƀ}�l�[�W���֐���frontier_map.c���Ɏ������Ă���B
	FSS_CHAR_RESOURCE_DATA char_res[FIELD_OBJ_RESOURCE_MAX];	///<�o�^���\�[�X�Ǘ�
	//FSS_CHAR_ACTOR_DATA char_actor[FIELD_OBJ_MAX];				///<�o�^�A�N�^�[�Ǘ�
	FSS_ACTOR_WORK fss_actor[FIELD_OBJ_MAX];					///<�o�^�A�N�^�[���[�N
	FSS_OBJMOVE_WORK objmove_work[FIELD_OBJ_MAX];				///<OBJ���샏�[�N
	
	//�z�u���A�N�^�[��Push,Pop�p�ޔ��o�b�t�@
	PUSH_ARTICLE_WORK push_article;	///<�ޔ�p�z�u���A�N�^�[�o�b�t�@
}FRONTIER_MAIN_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
PROC_RESULT FrontierMainProc_Init( PROC * proc, int * seq );
PROC_RESULT FrontierMainProc_Main( PROC * proc, int * seq );
PROC_RESULT FrontierMainProc_End( PROC * proc, int * seq );
static void Frontier_MapCreate(FMAIN_PTR fmw);
static void Frontier_MapFree(FMAIN_PTR fmw);
static void Frontier_OverlayLoad(void);
static void Frontier_OverlayUnload(void);
static void Frontier_ManageWorkClear(FMAIN_PTR fmw);


//==============================================================================
//	PROC
//==============================================================================
//�v���Z�X��`�f�[�^
const PROC_DATA FrontierMainProcData = {
	FrontierMainProc_Init,
	FrontierMainProc_Main,
	FrontierMainProc_End,
//	FS_OVERLAY_ID(frontier_common),
	NO_OVERLAY_ID,
};


//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F������
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT FrontierMainProc_Init( PROC * proc, int * seq )
{
	FRONTIER_MAIN_WORK *fmw;
	int i;
	
	Frontier_OverlayLoad();
	
	fmw = PROC_AllocWork(proc, sizeof(FRONTIER_MAIN_WORK), HEAPID_WORLD );
	MI_CpuClear8(fmw, sizeof(FRONTIER_MAIN_WORK));
	Frontier_ManageWorkClear(fmw);
	Frontier_ArticlePushBufferInit(fmw);
	
	//�O������ŏ��̃V�[��ID�Ȃǂ������Ă���f�[�^���󂯎��
	fmw->ex_param = PROC_GetParentWork(proc);
	GF_ASSERT(fmw->ex_param != NULL);
	
	//���X�N���v�g�ݒ�
	fmw->fss = FSS_SystemCreate(fmw, HEAPID_WORLD, fmw->ex_param->scene_id);
	FSS_ScrTaskAdd(fmw->fss, fmw->ex_param->scene_id, 0);

	//�}�b�v�ݒ�
	Frontier_MapCreate(fmw);
	
//	WIPE_SYS_Start(WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK, 
//		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WORLD);

	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F���C��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT FrontierMainProc_Main( PROC * proc, int * seq )
{
	FRONTIER_MAIN_WORK * fmw  = PROC_GetWork( proc );
	int ret;
	enum{
		SEQ_IN,
		SEQ_MAIN,
		SEQ_OUT,
		
		SEQ_SUB_PROC_INIT,
		SEQ_SUB_PROC_MAIN,
		
		SEQ_MAP_CHANGE,
		SEQ_MAP_CHANGE_LOAD,
	};
	
	switch(*seq){
	case SEQ_IN:
//		if(WIPE_SYS_EndCheck() == TRUE){
			*seq = SEQ_MAIN;
//		}
		break;
	case SEQ_MAIN:
		if(fmw->finish_req == TRUE){
			*seq = SEQ_OUT;
			break;
		}
		if(fmw->map_status == FALSE){
			break;
		}
		if(fmw->map_change == TRUE){
			*seq = SEQ_MAP_CHANGE;
			break;
		}
		
		//���X�N���v�g���s
		if(FSS_ScriptMain(fmw->fss) == TRUE){
			if(sys.trg & PAD_BUTTON_B){
				*seq = SEQ_OUT;
			}
			
			//�L�[���̓`�F�b�N
			
			//�GAI����
			
		}
		
		if(fmw->sub_proc != NULL){
			*seq = SEQ_SUB_PROC_INIT;
		}
		break;
	case SEQ_OUT:
		return PROC_RES_FINISH;
	
	case SEQ_SUB_PROC_INIT:
		FrontierMap_Push(fmw->fmap);
		Frontier_MapFree(fmw);
		Frontier_OverlayUnload();
		*seq = SEQ_SUB_PROC_MAIN;
		break;
	case SEQ_SUB_PROC_MAIN:
	    if(ProcMain(fmw->sub_proc) == TRUE) {
			PROC_Delete(fmw->sub_proc);
			Frontier_OverlayLoad();
			if(fmw->sub_proc_end_func != NULL){
				fmw->sub_proc_end_func(fmw->sub_proc_parent_work);
			}
			if(fmw->sub_proc_parent_work != NULL && fmw->sub_proc_auto_free == TRUE){
				sys_FreeMemoryEz(fmw->sub_proc_parent_work);
			}
			fmw->sub_proc = NULL;
			fmw->sub_proc_end_func = NULL;
			fmw->sub_proc_parent_work = NULL;
			
			Frontier_MapCreate(fmw);
			FrontierMap_Pop(fmw->fmap);
			*seq = SEQ_MAIN;
		}
		break;
	
	case SEQ_MAP_CHANGE:
		Frontier_MapFree(fmw);
		Frontier_ManageWorkClear(fmw);
		*seq = SEQ_MAP_CHANGE_LOAD;
		break;
	case SEQ_MAP_CHANGE_LOAD:
		Frontier_MapCreate(fmw);
		if(fmw->map_change_event_id == FS_MAPCHANGE_EVENTID_NULL){
			//�ʏ�̃}�b�v�`�F���W(.s�͂��̂܂܌p�����Ďg�p)
			
			FSS_MsgManageSceneChange(fmw->fss, fmw->ex_param->scene_id, HEAPID_WORLD);
		}
		else{	//�X�N���v�g�t�@�C��(.s)�܂邲�Ɛ؂�ւ���
			FS_PARAM_WORK *pw;
			
			//param_work�͌p��������׃o�b�N�A�b�v
			pw = FSS_ParamWorkBackup(fmw->fss, HEAPID_WORLD);
			//���ݓǂݍ���ł���X�N���v�g��j��
			FSS_SystemFree(fmw->fss);
			
			//�V�����X�N���v�g�ǂݍ���
			fmw->fss = FSS_SystemCreate(fmw, HEAPID_WORLD, fmw->ex_param->scene_id);
			FSS_ScrTaskAdd(fmw->fss, fmw->ex_param->scene_id, fmw->map_change_event_id);
			//��Ńo�b�N�A�b�v����param_work��V�����X�N���v�g�V�X�e���ɃZ�b�g����
			FSS_ParamWorkRecover(fmw->fss, pw);
		}
		fmw->map_change = FALSE;
		*seq = SEQ_MAIN;
		break;
	}

	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �v���Z�X�֐��F�I��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
PROC_RESULT FrontierMainProc_End( PROC * proc, int * seq )
{
	FRONTIER_MAIN_WORK * fmw = PROC_GetWork( proc );

	//���X�N���v�g���
	FSS_SystemFree(fmw->fss);

	//�}�b�v���
	Frontier_MapFree(fmw);

	PROC_FreeWork(proc);				// ���[�N�J��
	
	Frontier_OverlayUnload();
	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief   �}�b�v�V�X�e���쐬
 *
 * @param   fmain		
 */
//--------------------------------------------------------------
static void Frontier_MapCreate(FMAIN_PTR fmw)
{
	fmw->fmap = FrontierMap_Init(fmw);
	fmw->map_status = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �}�b�v�V�X�e�����
 *
 * @param   fmw		
 */
//--------------------------------------------------------------
static void Frontier_MapFree(FMAIN_PTR fmw)
{
	FrontierMap_End(fmw->fmap);
	fmw->map_status = FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   Push,Pop�p�̊Ǘ��p���[�N������������
 *
 * @param   fmw		
 */
//--------------------------------------------------------------
static void Frontier_ManageWorkClear(FMAIN_PTR fmw)
{
	int i;
	
	for(i = 0; i < FIELD_OBJ_RESOURCE_MAX; i++){
		fmw->char_res[i].charid = OBJCODEMAX;
	}
	
	MI_CpuClear8(fmw->fss_actor, sizeof(FSS_ACTOR_WORK) * FIELD_OBJ_MAX);
	for(i = 0; i < FIELD_OBJ_MAX; i++){
		fmw->fss_actor[i].param.playid = FIELD_PLAYID_NULL;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �I�[�o�[���C���[�h
 *
 * ���ǐ퓬��ʂւ̈ڍs�A���A���ɃI�[�o�[���C�̃��[�h�A�A�����[�h�����O�ł��Ȃ���΂����Ȃ��̂ŁA
 * PROC_DATA�ł̃I�[�o�[���CID�͎g�p���Ă��Ȃ��ŁA���͂ł�����������Ă��܂�
 */
//--------------------------------------------------------------
static void Frontier_OverlayLoad(void)
{
	Overlay_Load(FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);
	Overlay_Load(FS_OVERLAY_ID(factory), OVERLAY_LOAD_NOT_SYNCHRONIZE);
	Overlay_Load(FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);
}

//--------------------------------------------------------------
/**
 * @brief   �I�[�o�[���C�A�����[�h
 */
//--------------------------------------------------------------
static void Frontier_OverlayUnload(void)
{
	Overlay_UnloadID(FS_OVERLAY_ID(frontier_common));
	Overlay_UnloadID(FS_OVERLAY_ID(factory));
	Overlay_UnloadID(FS_OVERLAY_ID(wifi_2dmapsys));
}

//--------------------------------------------------------------
/**
 * @brief   ExParam���擾����
 *
 * @param   fmain		
 *
 * @retval  �|�C���^
 */
//--------------------------------------------------------------
FRONTIER_EX_PARAM * Frontier_ExParamGet(FMAIN_PTR fmain)
{
	return fmain->ex_param;
}

//--------------------------------------------------------------
/**
 * @brief   FMAP�A�h���X���擾����
 * @param   fmain		
 * @retval  
 */
//--------------------------------------------------------------
FMAP_PTR Frontier_FMapAdrsGet(FMAIN_PTR fmain)
{
	return fmain->fmap;
}

//--------------------------------------------------------------
/**
 * @brief   FSS�A�h���X���擾����
 * @param   fmain		
 * @retval  
 */
//--------------------------------------------------------------
FSS_PTR Frontier_FSSAdrsGet(FMAIN_PTR fmain)
{
	return fmain->fss;
}

//--------------------------------------------------------------
/**
 * @brief   ���C�������ێ����Ă���A�v���p�V�X�e�����[�N���擾����
 *
 * @param   fmain		
 *
 * @retval  �A�v���p�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void * Frontier_SysWorkGet(FMAIN_PTR fmain)
{
	return fmain->ex_param->syswork;
}

//--------------------------------------------------------------
/**
 * @brief   �A�v���p�V�X�e�����[�N���Z�b�g����
 *
 * @param   fmain		
 * @param   syswork		�A�v���p�V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Frontier_SysWorkSet(FMAIN_PTR fmain, void *syswork)
{
	fmain->ex_param->syswork = syswork;
}

//--------------------------------------------------------------
/**
 * @brief   �T�uPROC�o�^
 *
 * @param   fmain			
 * @param   data			�v���Z�X����֐�
 * @param   parent_work		��ʂ�������n�����[�N�ւ̃|�C���^
 * @param   auto_free		TRUE:�T�uPROC�I�����ɁAparent_work�œn���ꂽ���[�N��FreeMemory����
 * @param   end_func		�T�uPROC�I�����ɌĂяo���֐�(NULL�B parent_work��������FreeMemory��
 * 							����o���Ȃ����Ȃǂ́A�ʉ�������Ȃ񂩂Ɏg���Ă�������
 */
//--------------------------------------------------------------
void Frontier_SubProcSet(FMAIN_PTR fmain, const PROC_DATA * proc_data, void * parent_work, int auto_free, FS_SUBPROC_END_FUNC end_func)
{
	GF_ASSERT(fmain->sub_proc == NULL);
	fmain->sub_proc = PROC_Create(proc_data, parent_work, HEAPID_WORLD);
	fmain->sub_proc_parent_work = parent_work;
	fmain->sub_proc_auto_free = auto_free;
	fmain->sub_proc_end_func = end_func;
}

//--------------------------------------------------------------
/**
 * @brief   �I�����N�G�X�g�ݒ�
 *
 * @param   fmain		
 */
//--------------------------------------------------------------
void Frontier_FinishReq(FMAIN_PTR fmain)
{
	fmain->finish_req = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �}�b�v�؂�ւ����N�G�X�g�ݒ�
 *
 * @param   fmain		
 * @param   scene_id		�؂�ւ���̃V�[��ID
 * @param   event_id		�C�x���gID�w��(�w�肵�Ȃ��ꍇ��FS_MAPCHANGE_EVENTID_NULL)
 *
 * event_id��FS_MAPCHANGE_EVENTID_NULL�ȊO�ɂ����ꍇ�́A�}�b�v�؂�ւ��ō��ǂݍ���ł���X�N���v�g
 * ����x�j�����āAscene_id�Ŏw�肵�Ă���SceneData���Q�Ƃ��ăX�N���v�g�t�@�C����ǂݍ��݂Ȃ����܂�
 */
//--------------------------------------------------------------
void Frontier_MapChangeReq(FMAIN_PTR fmain, u16 scene_id, u16 event_id)
{
	fmain->ex_param->scene_id = scene_id;
	fmain->map_change = TRUE;
	fmain->map_change_event_id = event_id;
}

//--------------------------------------------------------------
/**
 * @brief   ���\�[�X�Ǘ��o�b�t�@�̃|�C���^���擾����
 * @param   fmain		
 * @retval  ���\�[�X�Ǘ��o�b�t�@�̃|�C���^
 */
//--------------------------------------------------------------
FSS_CHAR_RESOURCE_DATA * Frontier_CharResourceBufferGet(FMAIN_PTR fmain)
{
	return fmain->char_res;
}

//--------------------------------------------------------------
/**
 * @brief   �A�N�^�[�Ǘ��o�b�t�@�̃|�C���^���擾����
 * @param   fmain		
 * @retval  �A�N�^�[�Ǘ��o�b�t�@�̃|�C���^
 */
//--------------------------------------------------------------
FSS_ACTOR_WORK * Frontier_ActorBufferGet(FMAIN_PTR fmain)
{
	return fmain->fss_actor;
}

//--------------------------------------------------------------
/**
 * @brief   �A�N�^�[�ԍ����w�肵�ăA�N�^�[�Ǘ��o�b�t�@�̃|�C���^���擾����
 *
 * @param   fmain			
 * @param   actor_no		�A�N�^�[�ԍ�
 *
 * @retval  �A�N�^�[�Ǘ��o�b�t�@�̃|�C���^
 *
 *
 */
//--------------------------------------------------------------
FSS_ACTOR_WORK * Frontier_ActorWorkPtrGet(FMAIN_PTR fmain, int actor_no)
{
	return &fmain->fss_actor[actor_no];
}

//--------------------------------------------------------------
/**
 * @brief   �z�u���A�N�^�[�ޔ�p�o�b�t�@�̃|�C���^���擾����
 * @param   fmain		
 * @retval  �z�u���A�N�^�[�ޔ�p�o�b�t�@�̃|�C���^
 */
//--------------------------------------------------------------
PUSH_ARTICLE_WORK * Frontier_ArticlePushBufferGet(FMAIN_PTR fmain)
{
	return &fmain->push_article;
}

//--------------------------------------------------------------
/**
 * @brief   �z�u���A�N�^�[�ޔ�p�o�b�t�@������������
 * @param   fmain		
 */
//--------------------------------------------------------------
void Frontier_ArticlePushBufferInit(FMAIN_PTR fmain)
{
	int i;
	
	MI_CpuClear8(&fmain->push_article, sizeof(PUSH_ARTICLE_ACT_PARAM));
	for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
		fmain->push_article.resource_id[i] = SCR_ACTWORK_RESOURCE_ID_NULL;
	}
}

