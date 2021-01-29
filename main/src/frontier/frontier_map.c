//==============================================================================
/**
 * @file	frontier_map.c
 * @brief	�t�����e�B�A�}�b�v
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
#include "system/font_arc.h"
#include "system/window.h"
//#include "script.h"
#include "..\fielddata\script\connect_def.h"
#include "system/clact_tool.h"
#include "communication/wm_icon.h"

#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_matsu.h"

#include "frontier_types.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier_tcb_pri.h"
#include "frontier_particle.h"
#include "frontier_actor.h"
#include "frontier_tool.h"

#include "graphic/frontier_bg_def.h"




//==============================================================================
//	�萔��`
//==============================================================================
///�}�b�v�ʃX�N���[���x�[�X
#define MAPBG_SCRBASE		(GX_BG_SCRBASE_0x2800)
///�}�b�v�ʃL�����N�^�x�[�X
#define MAPBG_CHARBASE		(GX_BG_CHARBASE_0x30000)

///�G�t�F�N�g��(���d��)�X�N���[���x�[�X
#define EFFBG_SCRBASE		(GX_BG_SCRBASE_0x0800)
///�G�t�F�N�g��(���d��)�L�����N�^�x�[�X
#define EFFBG_CHARBASE		(GX_BG_CHARBASE_0x20000)

///���N�G�X�g�R�}���h�L���[��
#define FMAP_REQCMDQ_NUM		(128)
///�A�N�V�����R�}���h�L���[��
#define FMAP_ACTCMDQ_NUM		(128)

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
#if 0
typedef struct{
//	CATS_SYS_PTR		csp;
//	CATS_RES_PTR		crp;
	GF_BGL_INI *bgl;
//	GF_BGL_BMPWIN win[CONRES_BMPWIN_MAX];

//	MSGDATA_MANAGER *conres_msg;	///<�_���X���僁�b�Z�[�W�p���b�Z�[�W�}�l�[�W���̃|�C���^
//	WORDSET *wordset;
//	STRBUF *msg_buf;				///<Alloc����������o�b�t�@�ւ̃|�C���^

	PALETTE_FADE_PTR pfd;			///<�p���b�g�t�F�[�h�V�X�e���ւ̃|�C���^
	
	FMAIN_PTR fmain;
	
	//2D�}�b�v�V�X�e�����W���[��
	WF2DMAP_OBJSYS *objsys;		///<�I�u�W�F�N�g�Ǘ����W���[��
	
}FRMAP_SYSTEM_PARAM;
#endif


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void FrontierMapVBlank(void *work);
static void FrontierMapHBlank(void *work);
static void VBlankTCB_IntrTask(TCB_PTR tcb, void *work);
static void FrontierMapUpdate(TCB_PTR tcb, void *work);
static void FrontierMapOBJSysUpdate(TCB_PTR tcb, void *work);
static void FrontierMapOBJCmdJudgeUpdate(TCB_PTR tcb, void *work);
static void FrontierMap_VramBankSet(GF_BGL_INI *bgl, int scene_id);
static void MapSystem2D_ModuleSet(FMAP_PTR fmap, int scene_id, const MYSTATUS *my_status);
static void MapSystem2D_ModuleFree(FMAP_PTR fmap);
static void SystemPaletteLoad(FMAP_PTR fmap);
static void ActorSysInit(FMAP_PTR fmap);
static void ActorSysFree(FMAP_PTR fmap);
FMAP_PTR FrontierMap_Init(FMAIN_PTR fmain);
void FrontierMap_End(FMAP_PTR fmap);
static void ObjCmdJudgeMove(FMAP_PTR fmap);
static void SubDisp_BGSet(FMAP_PTR fmap);
static GF_G3DMAN * Frontier_3D_Init(int heap_id);
static void Frontier_3D_SimpleSetUp(void);
static void Frontier_3D_Exit(GF_G3DMAN *g3Dman);
static void FMap_ArticleActPush(FMAP_PTR fmap);
static void FMap_ArticleActPop(FMAP_PTR fmap);
static void Frontier_ActWork_to_ActData(FSS_ACTOR_WORK *fss_actor, FSS_CHAR_ACTOR_DATA *act_data);
static void Frontier_ActWorkDataSet(FMAIN_PTR fmain, int act_no, WF2DMAP_OBJWK *objwk, WF2DMAP_OBJDRAWWK *drawwk, const FSS_CHAR_ACTOR_DATA *res);
static void FrontierMapScroll(FMAP_PTR fmap);
static void FrontierMapScroll_Easy(FMAP_PTR fmap);


//==============================================================================
//	�f�[�^
//==============================================================================
//==============================================================================
//	CLACT�p�f�[�^
//==============================================================================
static	const TCATS_OAM_INIT MapTcats = {
	FMAP_OAM_START_MAIN, FMAP_OAM_END_MAIN,
	FMAP_OAM_AFFINE_START_MAIN, FMAP_OAM_AFFINE_END_MAIN,
	FMAP_OAM_START_SUB, FMAP_OAM_END_SUB,
	FMAP_OAM_AFFINE_START_SUB, FMAP_OAM_AFFINE_END_SUB,
};

static	const TCATS_CHAR_MANAGER_MAKE MapCcmm = {
	FMAP_CHAR_MAX,
	FMAP_CHAR_VRAMSIZE_MAIN,
	FMAP_CHAR_VRAMSIZE_SUB,
	GX_OBJVRAMMODE_CHAR_1D_128K,	//�l��OBJ�̃A�j����128k�Ȃ̂ŁBVRAM��64k�������ĂĂ��Ȃ��̂Ől���A�j����64k�ł��p�ӏo����̂Ȃ�΁A�}�b�s���O��64k�ɂ����������Ԗ���VRAM���g����
	GX_OBJVRAMMODE_CHAR_1D_32K
};

static const TCATS_RESOURCE_NUM_LIST MapResourceList = {
	FMAP_OAMRESOURCE_CHAR_MAX,
	FMAP_OAMRESOURCE_PLTT_MAX,
	FMAP_OAMRESOURCE_CELL_MAX,
	FMAP_OAMRESOURCE_CELLANM_MAX,
	FMAP_OAMRESOURCE_MCELL_MAX,
	FMAP_OAMRESOURCE_MCELLANM_MAX,
};

//--------------------------------------------------------------
//	�_�~�[�}�b�v�f�[�^
//--------------------------------------------------------------
static const WF2DMAP_MAP DumyMap[MAP_GRID_MAX_X * MAP_GRID_MAX_Y] = {
	0,
};


//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�}�b�v�V�X�e��������
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
FMAP_PTR FrontierMap_Init(FMAIN_PTR fmain)
{
	FMAP_PTR fmap;
	int i, scene_id;
	FRONTIER_EX_PARAM *ex_param = Frontier_ExParamGet(fmain);
	const MYSTATUS *my_status;
	
	my_status = SaveData_GetMyStatus(ex_param->savedata);
	scene_id = ex_param->scene_id;
	
//	Overlay_Load(FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);

	sys_VBlankFuncChange(NULL, NULL);	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane(0);
	GXS_SetVisiblePlane(0);
	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);
	G2_BlendNone();
	G2S_BlendNone();

	sys_CreateHeap(HEAPID_BASE_APP, HEAPID_FRONTIERMAP, FRONTIERMAP_ALLOC_SIZE);

	fmap = sys_AllocMemory(HEAPID_FRONTIERMAP, sizeof(FRONTIERMAP_WORK));
	MI_CpuClear8(fmap, sizeof(FRONTIERMAP_WORK));
	fmap->fmain = fmain;
	
	//�I������֐�EndFunc���Ă΂��O��scene_id���ς��ꍇ������̂ł����ŏ�������A
	//�I������֐��Ăяo���悤�̃V�[��ID���L��
	fmap->func_scene_id = scene_id;
	
	for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
		fmap->article.resource_id[i] = SCR_ACTWORK_RESOURCE_ID_NULL;
	}

	fmap->g3Dman = Frontier_3D_Init(HEAPID_FRONTIERMAP);
	
	//�p���b�g�t�F�[�h�V�X�e���쐬
	fmap->pfd = PaletteFadeInit(HEAPID_FRONTIERMAP);
	PaletteTrans_AutoSet(fmap->pfd, TRUE);
	PaletteFadeWorkAllocSet(fmap->pfd, FADE_MAIN_BG, 0x200, HEAPID_FRONTIERMAP);
	PaletteFadeWorkAllocSet(fmap->pfd, FADE_SUB_BG, 0x200, HEAPID_FRONTIERMAP);
	PaletteFadeWorkAllocSet(fmap->pfd, FADE_MAIN_OBJ, 0x200 - 0x40, HEAPID_FRONTIERMAP);
	PaletteFadeWorkAllocSet(fmap->pfd, FADE_SUB_OBJ, 0x200, HEAPID_FRONTIERMAP);
	//�t�@�N�g���[�ΐ핔���̏�������֐��ŁAFADE_MAIN_BG_EX3�̒ǉ������Ă��܂�(07.11.27)(���g�p)

	fmap->bgl = GF_BGL_BglIniAlloc(HEAPID_FRONTIERMAP);

	initVramTransferManagerHeap(64, HEAPID_FRONTIERMAP);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	FrontierMap_VramBankSet(fmap->bgl, scene_id);

	//BG�ǂݍ���
	SystemPaletteLoad(fmap);
	SubDisp_BGSet(fmap);
	
	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);

	//�A�N�^�[�V�X�e���쐬
	ActorSysInit(fmap);

	//�p�[�e�B�N���V�X�e��������
	fmap->frp = FRParticle_Init(HEAPID_FRONTIERMAP);

	//2D�}�b�v�V�X�e�����W���[���ݒ�
	MapSystem2D_ModuleSet(fmap, scene_id, my_status);

//	WIPE_SYS_Start(WIPE_PATTERN_FMAS, WIPE_TYPE_SCREWIN, WIPE_TYPE_SCREWIN, WIPE_FADE_BLACK, 
//		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FRONTIERMAP);

	fmap->objupdate_tcb = TCB_Add(FrontierMapOBJSysUpdate, fmap, TCBPRI_OBJSYS_UPDATE);
	fmap->cmdjudgeupdate_tcb = TCB_Add(FrontierMapOBJCmdJudgeUpdate, fmap, TCBPRI_CMDJUDGE_UPDATE);
	fmap->update_tcb = TCB_Add(FrontierMapUpdate, fmap, TCBPRI_MAP_UPDATE);

	GF_Disp_DispOn();
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);
	GF_Disp_GXS_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	//�T�E���h�f�[�^���[�h(�R���e�X�g)
	Snd_DataSetByScene( SND_SCENE_FIELD, 
						FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_BGMID), 1 );

	MsgPrintAutoFlagSet(MSG_AUTO_ON);
	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);
	
	sys_VBlankFuncChange(FrontierMapVBlank, fmap);
	//��Wipe��H�u�����N���g�p����̂ŁAWipe���I��������Z�b�g����悤�ɕύX
//	sys_HBlankIntrSet(FrontierMapHBlank, fmap);
	fmap->vintr_tcb = VIntrTCB_Add(VBlankTCB_IntrTask, fmap, 10);
	
	//�V�[���f�[�^�ɐݒ肳��Ă��鏉������֐��Ăяo��
	
	FSS_SceneInitFuncCall(fmap, &fmap->scene_func_work_ptr, fmap->func_scene_id);
	
	WirelessIconEasy();	//�ʐM�A�C�R��

	return fmap;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�}�b�v�V�X�e���I��
 *
 * @param   proc		�v���Z�X�f�[�^
 * @param   seq			�V�[�P���X
 *
 * @retval  ������
 */
//--------------------------------------------------------------
void FrontierMap_End(FMAP_PTR fmap)
{
	int i;

	//�V�[���f�[�^�ɐݒ肳��Ă���I������֐��Ăяo��
	{
		FRONTIER_EX_PARAM *ex_param = Frontier_ExParamGet(fmap->fmain);
		FSS_SceneEndFuncCall(fmap, &fmap->scene_func_work_ptr, fmap->func_scene_id);
	}

	//2D�}�b�v�V�X�e�����W���[�����
	MapSystem2D_ModuleFree(fmap);

	//BMP�J��
//	for(i = 0; i < CONRES_BMPWIN_MAX; i++){
//		GF_BGL_BmpWinDel(&fmap->win[i]);
//	}

	//���C�����BG�폜
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
	GF_BGL_BGControlExit(fmap->bgl, FRMAP_FRAME_WIN );
	GF_BGL_BGControlExit(fmap->bgl, FRMAP_FRAME_EFF );
	GF_BGL_BGControlExit(fmap->bgl, FRMAP_FRAME_MAP );
	//�T�u���BG�폜
	GF_BGL_VisibleSet(FRMAP_FRAME_SUB_AUDIENCE, VISIBLE_OFF);
	GF_BGL_BGControlExit(fmap->bgl, FRMAP_FRAME_SUB_AUDIENCE);

	//�A�N�^�[�V�X�e���폜
	ActorSysFree(fmap);

	//�p�[�e�B�N���V�X�e���I��
	FRParticle_Exit(fmap->frp);

	//Vram�]���}�l�[�W���[�폜
	DellVramTransferManager();

	//�p���b�g�t�F�[�h�V�X�e���폜
	PaletteFadeWorkAllocFree(fmap->pfd, FADE_MAIN_BG);
	PaletteFadeWorkAllocFree(fmap->pfd, FADE_SUB_BG);
	PaletteFadeWorkAllocFree(fmap->pfd, FADE_MAIN_OBJ);
	PaletteFadeWorkAllocFree(fmap->pfd, FADE_SUB_OBJ);
	//�t�@�N�g���[�ΐ핔���̏I������֐��ŁAFADE_MAIN_BG_EX3�̍폜�����Ă��܂�(07.11.27)(���g�p)

	PaletteFadeFree(fmap->pfd);

	//BGL�J��
	sys_FreeMemoryEz(fmap->bgl);

	TCB_Delete(fmap->objupdate_tcb);
	TCB_Delete(fmap->cmdjudgeupdate_tcb);
	TCB_Delete(fmap->update_tcb);
	TCB_Delete(fmap->vintr_tcb);
	
	Frontier_3D_Exit(fmap->g3Dman);

	StopTP();		//�^�b�`�p�l���̏I��

	sys_FreeMemoryEz(fmap);
	
	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);
	
	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	sys_DeleteHeap(HEAPID_FRONTIERMAP);

	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	WirelessIconEasyEnd();	//�ʐM�A�C�R���폜
	
	MI_CpuFill16((void*)HW_BG_PLTT, 0x7fff, 0x200);
	MI_CpuFill16((void*)HW_OBJ_PLTT, 0x7fff, 0x200);
	MI_CpuFill16((void*)HW_DB_BG_PLTT, 0x7fff, 0x200);
	MI_CpuFill16((void*)HW_DB_OBJ_PLTT, 0x7fff, 0x200);

	G2_BlendNone();
	G2S_BlendNone();

//	Overlay_UnloadID(FS_OVERLAY_ID(frontier_common));
}

//--------------------------------------------------------------
/**
 * @brief   �ʉ�ʈڍs���A���A�p�ɉ�ʏ��ޔ�����
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
void FrontierMap_Push(FMAP_PTR fmap)
{
	int i;

	//���\�[�X�ޔ�
	{
		//�Z�b�g������ޔ�p���[�N�̒l���ς�鎖�͂Ȃ��̂œ��ɂ��鎖�͂Ȃ�
	}
	
	//�A�N�^�[�ޔ�
	{
		FSS_ACTOR_WORK *fss_actor;
		
		for(i = 0; i < FIELD_OBJ_MAX; i++){
			fss_actor = Frontier_ActorWorkPtrGet(fmap->fmain, i);
			if(fss_actor->objwk != NULL){
				//�ύX�̉\���̂�����̂����đޔ�
				fss_actor->param.way = WF2DMAP_OBJWkDataGet(fss_actor->objwk, WF2DMAP_OBJPM_WAY);
				fss_actor->param.status = WF2DMAP_OBJWkDataGet(fss_actor->objwk, WF2DMAP_OBJPM_ST);
				fss_actor->param.x = WF2DMAP_OBJWkDataGet(fss_actor->objwk, WF2DMAP_OBJPM_X);
				fss_actor->param.y = WF2DMAP_OBJWkDataGet(fss_actor->objwk, WF2DMAP_OBJPM_Y);
				
				fss_actor->param.visible = WF2DMAP_OBJDrawWkDrawFlagGet(fss_actor->drawwk);
			}
		}
	}

	//�z�u���A�N�^�[�ޔ�
	FMap_ArticleActPush(fmap);
}

//--------------------------------------------------------------
/**
 * @brief   �ʉ�ʂ���߂��Ă������̉�ʏ�񕜋A����
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
void FrontierMap_Pop(FMAP_PTR fmap)
{
	int i;
	
	//���\�[�X�ēo�^
	{
		FSS_CHAR_RESOURCE_DATA *res_manage;
		
		res_manage = Frontier_CharResourceBufferGet(fmap->fmain);
		for(i = 0; i < FIELD_OBJ_RESOURCE_MAX; i++){
			if(res_manage[i].charid != OBJCODEMAX){
				//FMap_CharResourceSet(fmap, &res_manage[i]);
				WF2DMAP_OBJDrawSysResSet(fmap->drawsys, res_manage[i].charid,
					res_manage[i].movetype, HEAPID_FRONTIERMAP);
//					NNS_G2D_VRAM_TYPE_2DMAIN, res_manage[i].movetype, HEAPID_FRONTIERMAP);[tomoya�ύX]	
			}
		}
	}
	
	//�A�N�^�[�ēo�^
	{
		FSS_ACTOR_WORK *fss_actor;
		FSS_CHAR_ACTOR_DATA act_data;
		
		for(i = 0; i < FIELD_OBJ_MAX; i++){
			fss_actor = Frontier_ActorWorkPtrGet(fmap->fmain, i);
			if(fss_actor->param.playid != FIELD_PLAYID_NULL){
				OS_TPrintf("playid = %d, charid = %d\n", 
					fss_actor->param.playid, fss_actor->param.charid);
				Frontier_ActWork_to_ActData(fss_actor, &act_data);
				FMap_ActorSet(fmap, &act_data, i);
			}
		}
	}

	//�z�u���A�N�^�[�ēo�^
	FMap_ArticleActPop(fmap);
}

//--------------------------------------------------------------
/**
 * @brief	VBLANK�֐�
 *
 * @param	work	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void FrontierMapVBlank(void *work)
{
	FMAP_PTR fmap = work;

	DoVramTransferManager();	// Vram�]���}�l�[�W���[���s
	CATS_RenderOamTrans();
	PaletteFadeTrans(fmap->pfd);
	
	GF_BGL_VBlankFunc(fmap->bgl);
	
	OS_SetIrqCheckFlag( OS_IE_V_BLANK);
}

//--------------------------------------------------------------
/**
 * @brief	HBLANK�֐�
 *
 * @param	work	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void FrontierMapHBlank(void *work)
{
	FMAP_PTR fmap = work;

}

//--------------------------------------------------------------
/**
 * @brief   VBlank�^�X�N�֐�(1/60�ŉ��܂�)
 *
 * @param   tcb		TCB�ւ̃|�C���^
 * @param   work	���ʔ��\�Ǘ����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void VBlankTCB_IntrTask(TCB_PTR tcb, void *work)
{
	FMAP_PTR fmap = work;
	
}

//--------------------------------------------------------------
/**
 * @brief   2D���W���[���F�I�u�W�F�N�g����TCB
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		fmap
 *
 * �e�I�u�W�F�N�g�̓���֐��̎��s������WF2DMAP_OBJSysMain�ƃR�}���h���f�̊Ԃ�
 * TCB�Ŏ��s�o����悤�ɂ��邽�߁A�Ɨ�����TCB�ɂ��Ă��܂�
 */
//--------------------------------------------------------------
static void FrontierMapOBJSysUpdate(TCB_PTR tcb, void *work)
{
	FMAP_PTR fmap = work;
	
	//�I�u�W�F�N�g���상�C��
	WF2DMAP_OBJSysMain(fmap->objsys);
}

//--------------------------------------------------------------
/**
 * @brief   2D���W���[���F�R�}���h���f�����sTCB
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		fmap
 *
 * �e�I�u�W�F�N�g�̓���֐��̎��s������WF2DMAP_OBJSysMain�ƃR�}���h���f�̊Ԃ�
 * TCB�Ŏ��s�o����悤�ɂ��邽�߁A�Ɨ�����TCB�ɂ��Ă��܂�
 */
//--------------------------------------------------------------
static void FrontierMapOBJCmdJudgeUpdate(TCB_PTR tcb, void *work)
{
	FMAP_PTR fmap = work;
	
	//�R�}���h���f�����s
	ObjCmdJudgeMove(fmap);
}

//--------------------------------------------------------------
/**
 * @brief   ���C�����[�v�̍Ō�ɍs���V�X�e���֘A�̍X�V����
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		fmap
 */
//--------------------------------------------------------------
static void FrontierMapUpdate(TCB_PTR tcb, void *work)
{
	FMAP_PTR fmap = work;
	
	{//2D���W���[���FBG�X�N���[��
		FSS_ACTOR_WORK *player_actor;
		
		player_actor = Frontier_ActorWorkPtrGet(fmap->fmain, FIELD_OBJ_PLAYER);
		if(player_actor->objwk != NULL){
			WF2DMAP_SCRContSysMain(&fmap->scrollsys, player_actor->objwk);
		}
		FrontierMapScroll(fmap);
	}
	
	{//2D���W���[���F�I�u�W�F�N�g�`��
		WF2DMAP_OBJDrawSysUpdata(fmap->drawsys);
	}

	//�z�u���A�N�^�[�A�j��
	{
		u32 anmbit = fmap->article.anmbit;
		int i;
		
		for(i = 0; i < ACTWORK_MAX; i++){
			if(fmap->article.act[i] != NULL && (anmbit & 1)){
				CATS_ObjectUpdateCap(fmap->article.act[i]);
			}
			anmbit >>= 1;
		}
	}

	//�A�N�^�[
	CATS_Draw(fmap->clactsys.crp);
	CATS_UpdateTransfer();

	//�p�[�e�B�N��
	FRParticle_Main();
	GF_G3_RequestSwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

//--------------------------------------------------------------
/**
 * @brief   �}�b�v�X�N���[������
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void FrontierMapScroll(FMAP_PTR fmap)
{
	int scroll_mode;
	FRONTIER_EX_PARAM *ex_param = Frontier_ExParamGet(fmap->fmain);
	
	scroll_mode = FSS_SceneParamGet(ex_param->scene_id, FSS_SCENE_DATA_SCROLL_MODE);
	switch(scroll_mode){
	case SCROLL_MODE_NORMAL:
	default:
		//�X�N���[���\������
		if(fmap->scrdrawsys != NULL){
			WF2DMAP_SCRDrawSysMain(fmap->scrdrawsys, &fmap->scrollsys);
		}
		if(fmap->multi_scrdrawsys != NULL
			 && FSS_SceneParamGet(ex_param->scene_id, FSS_SCENE_DATA_MULTI_SCROLL_MODE) == TRUE){
			WF2DMAP_SCRDrawSysMain(fmap->multi_scrdrawsys, &fmap->scrollsys);
		}
		break;
	case SCROLL_MODE_EASY:
		FrontierMapScroll_Easy(fmap);
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �}�b�v�ȈՃX�N���[������
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void FrontierMapScroll_Easy(FMAP_PTR fmap)
{
	s16 scr_x;
	s16 scr_y;
	FRONTIER_EX_PARAM *ex_param = Frontier_ExParamGet(fmap->fmain);
	
	// �X�N���[�����W����X�N���[�����W���e�\���V�X�e���ɐݒ�
	scr_y = WF2DMAP_SCROLLSysTopGet(&fmap->scrollsys) + fmap->shake_work.shake_y;
	scr_x = WF2DMAP_SCROLLSysLeftGet(&fmap->scrollsys) + fmap->shake_work.shake_x;

	// �T�[�t�F�[�X
	CLACT_U_SetMainSurfaceMatrix(CATS_EasyRenderGet(fmap->clactsys.csp), 
		FX32_CONST(scr_x), FX32_CONST(scr_y));

	//BGL�X�N���[�����W�ݒ�
	GF_BGL_ScrollReq(fmap->bgl, FRMAP_FRAME_MAP, GF_BGL_SCROLL_X_SET, scr_x);
	GF_BGL_ScrollReq(fmap->bgl, FRMAP_FRAME_MAP, GF_BGL_SCROLL_Y_SET, scr_y);
	if(FSS_SceneParamGet(ex_param->scene_id, FSS_SCENE_DATA_MULTI_MAP_SCREENID) != MULTI_BG_NULL
			&& FSS_SceneParamGet(ex_param->scene_id, FSS_SCENE_DATA_MULTI_SCROLL_MODE) == TRUE){
		GF_BGL_ScrollReq(fmap->bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, scr_x);
		GF_BGL_ScrollReq(fmap->bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_Y_SET, scr_y);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �R�}���h���W���[�����f���R�}���h���s����
 *
 * @param   fmap
 */
//--------------------------------------------------------------
static void ObjCmdJudgeMove(FMAP_PTR fmap)
{
	WF2DMAP_REQCMD req;
	WF2DMAP_ACTCMD act;
	BOOL result;

	//�A�N�V�����R�}���h�L���[
	while(WF2DMAP_ACTCMDQSysCmdPop(fmap->actcmd_q, &act) == TRUE){
		//�A�N�V�����R�}���h�ݒ�
		WF2DMAP_OBJSysCmdSet(fmap->objsys, &act);
	}

	//���N�G�X�g�R�}���h�L���[
	while(WF2DMAP_REQCMDQSysCmdPop(fmap->reqcmd_q, &req) == TRUE){
		result = WF2DMAP_JUDGESysCmdJudge(fmap->mapsys, fmap->objsys, &req, &act);
		if(result == TRUE){
			//�A�N�V�����R�}���h�ݒ�
			WF2DMAP_OBJSysCmdSet(fmap->objsys, &act);
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void FrontierMap_VramBankSet(GF_BGL_INI *bgl, int scene_id)
{
	int text_mode;
	
	text_mode = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_BG_MODE);
	
	GF_Disp_GX_VisibleControlInit();

	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_256_BC,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_23_G,			// ���C��2D�G���W����BG�g���p���b�g
			GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
			GX_VRAM_TEX_0_A,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_0_F				// �e�N�X�`���p���b�g�X���b�g
		};
		GF_Disp_SetBank( &vramSetTable );

		//VRAM�N���A
		MI_CpuClear32((void*)HW_BG_VRAM, HW_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_BG_VRAM, HW_DB_BG_VRAM_SIZE);
		MI_CpuClear32((void*)HW_OBJ_VRAM, HW_OBJ_VRAM_SIZE);
		MI_CpuClear32((void*)HW_DB_OBJ_VRAM, HW_DB_OBJ_VRAM_SIZE);
	}

	// BG SYSTEM
	{
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_3D,
		};
		BGsys_data.bgMode = text_mode;
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME1_M	�E�B���h�E
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
				FRMAP_BGPRI_WIN, 0, 0, FALSE
			},
			///<FRAME2_M	�G�t�F�N�g
			{
				0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_256,
				EFFBG_SCRBASE, EFFBG_CHARBASE, GX_BG_EXTPLTT_23,
				FRMAP_BGPRI_EFF, 0, 0, FALSE
			},
			///<FRAME3_M	�}�b�v
			{
				0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_256,
				MAPBG_SCRBASE, MAPBG_CHARBASE, GX_BG_EXTPLTT_23,
				FRMAP_BGPRI_MAP, 0, 0, FALSE
			},
		};
		u16 screen_size;
		
		if(text_mode == GX_BGMODE_0){
			TextBgCntDat[1].colorMode = GX_BG_COLORMODE_16;
			TextBgCntDat[2].colorMode = GX_BG_COLORMODE_16;
			TextBgCntDat[1].bgExtPltt = GX_BG_EXTPLTT_01;
			TextBgCntDat[2].bgExtPltt = GX_BG_EXTPLTT_01;
		}
		screen_size = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_SCREEN_SIZE);
		TextBgCntDat[2].screenSize = screen_size;
		if(FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_SCREENID) != MULTI_BG_NULL){
			//���d�ʎg�p�̏ꍇ�̓}�b�v�ƃX�N���[���T�C�Y�����킹��
			TextBgCntDat[1].screenSize = screen_size;
		}
		
		if(text_mode == GX_BGMODE_0){
			GF_BGL_BGControlSet(bgl, FRMAP_FRAME_WIN, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
			GF_BGL_ScrClear(bgl, FRMAP_FRAME_WIN );
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_WIN, GF_BGL_SCROLL_X_SET, 0);
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_WIN, GF_BGL_SCROLL_Y_SET, 0);
			GF_BGL_BGControlSet(bgl, FRMAP_FRAME_EFF, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
			GF_BGL_ScrClear(bgl, FRMAP_FRAME_EFF );
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, 0);
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_Y_SET, 0);
			GF_BGL_BGControlSet(bgl, FRMAP_FRAME_MAP, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
			GF_BGL_ScrClear(bgl, FRMAP_FRAME_MAP );
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_MAP, GF_BGL_SCROLL_X_SET, 0);
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_MAP, GF_BGL_SCROLL_Y_SET, 0);
		}
		else{
			GF_BGL_BGControlSet(bgl, FRMAP_FRAME_WIN, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
			GF_BGL_ScrClear(bgl, FRMAP_FRAME_WIN );
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_WIN, GF_BGL_SCROLL_X_SET, 0);
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_WIN, GF_BGL_SCROLL_Y_SET, 0);
			GF_BGL_BGControlSet(bgl, FRMAP_FRAME_EFF, &TextBgCntDat[1], GF_BGL_MODE_256X16 );
			GF_BGL_ScrClear(bgl, FRMAP_FRAME_EFF );
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_X_SET, 0);
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_EFF, GF_BGL_SCROLL_Y_SET, 0);
			GF_BGL_BGControlSet(bgl, FRMAP_FRAME_MAP, &TextBgCntDat[2], GF_BGL_MODE_256X16 );
			GF_BGL_ScrClear(bgl, FRMAP_FRAME_MAP );
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_MAP, GF_BGL_SCROLL_X_SET, 0);
			GF_BGL_ScrollSet(bgl, FRMAP_FRAME_MAP, GF_BGL_SCROLL_Y_SET, 0);
		}
	
		G2_SetBG0Priority(FRMAP_3DBG_PRIORITY);
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER SubBgCntDat[] = {
			///<FRAME0_S	�ϋq
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				FRMAP_BGPRI_SUB_AUDIENCE, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FRMAP_FRAME_SUB_AUDIENCE, &SubBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRMAP_FRAME_SUB_AUDIENCE );
		GF_BGL_ScrollSet(bgl, FRMAP_FRAME_SUB_AUDIENCE, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRMAP_FRAME_SUB_AUDIENCE, GF_BGL_SCROLL_Y_SET, 0);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �V�X�e���n�p���b�g�̓ǂݍ���
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void SystemPaletteLoad(FMAP_PTR fmap)
{
	//�V�X�e���t�H���g
	PaletteWorkSet_Arc(fmap->pfd, ARC_FONT, NARC_font_system_ncrl, 
		HEAPID_FRONTIERMAP, FADE_MAIN_BG, 0x20, FFD_MENU_WIN_PAL * 16);
	//���b�Z�[�W�t�H���g
	PaletteWorkSet_Arc(fmap->pfd, ARC_FONT, NARC_font_talk_ncrl, 
		HEAPID_FRONTIERMAP, FADE_MAIN_BG, 0x20, FFD_MSG_WIN_PAL * 16);
	
	//��b�E�B���h�E�@�L�������p���b�g
	{
		FRONTIER_EX_PARAM *ex_param;
		
		ex_param = Frontier_ExParamGet(fmap->fmain);
		TalkWinGraphicSet(fmap->bgl, FRMAP_FRAME_WIN, FR_TALK_WIN_CGX_NUM, FR_TALK_WIN_PAL, 
			CONFIG_GetWindowType(ex_param->config), HEAPID_FRONTIERMAP);
		PaletteWorkSet_VramCopy(fmap->pfd, FADE_MAIN_BG, FR_TALK_WIN_PAL * 16, 0x20);
	}
	
	//�V�X�e���E�B���h�E�@�L�������p���b�g
	MenuWinGraphicSet(fmap->bgl, FRMAP_FRAME_WIN, FR_MENU_WIN_CGX_NUM, FR_MENU_WIN_PAL,
		MENU_TYPE_SYSTEM, HEAPID_FRONTIERMAP);
	PaletteWorkSet_VramCopy(fmap->pfd, FADE_MAIN_BG, FR_MENU_WIN_PAL * 16, 0x20);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ�BG�Z�b�g
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void SubDisp_BGSet(FMAP_PTR fmap)
{
	ARCHANDLE* hdl;
		
	//�Ƃ肠�����|�P�b�`�ɂ��鉺��ʂ��g���܂킷
	hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FRONTIERMAP );

	ArcUtil_HDL_BgCharSet(	hdl, BATT_FRONTIER_NCGR_BIN, fmap->bgl, GF_BGL_FRAME0_S,
							0, 0, TRUE, HEAPID_FRONTIERMAP );

	ArcUtil_HDL_ScrnSet(hdl, BATT_FRONTIER_NSCR_BIN, fmap->bgl, GF_BGL_FRAME0_S,
						0, 0, TRUE, HEAPID_FRONTIERMAP );

//	ArcUtil_HDL_PalSet(	hdl, NARC_poketch_before_nclr, PALTYPE_SUB_BG,
//						0, 0x20, HEAPID_FRONTIERMAP );
	PaletteWorkSet_Arc(fmap->pfd, ARC_FRONTIER_BG, BATT_FRONTIER_NCLR, 
		HEAPID_FRONTIERMAP, FADE_SUB_BG, 0x20, 0 * 16);
		
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------
/**
 * @brief   2D�}�b�v�V�X�e�����W���[���ݒ�
 *
 * @param   fmap		
 * @param   scene_id	
 * @param   my_status	
 */
//--------------------------------------------------------------
static void MapSystem2D_ModuleSet(FMAP_PTR fmap, int scene_id, const MYSTATUS *my_status)
{
	//�I�u�W�F�N�g�Ǘ����W���[��
	fmap->objsys = WF2DMAP_OBJSysInit(FIELD_OBJ_MAX, HEAPID_FRONTIERMAP);
	
	//�}�b�v�f�[�^�Ǘ����W���[��
	fmap->mapsys = WF2DMAP_MAPSysInit(MAP_GRID_MAX_X, MAP_GRID_MAX_Y, HEAPID_FRONTIERMAP);
	
	//�X�N���[���f�[�^�Ǘ����W���[��
	WF2DMAP_SCROLLSysDataInit(&fmap->scrollsys);
	
	//�I�u�W�F�N�g�f�[�^�`�惂�W���[��
	fmap->drawsys = WF2DMAP_OBJDrawSysInit_Shadow(CATS_GetClactSetPtr(fmap->clactsys.crp) ,
		fmap->pfd, FIELD_OBJ_MAX, FrontierTool_MyObjCodeGet(my_status), WF2DC_C_MOVERUN, 
		NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_FRONTIERMAP);
//		fmap->pfd, FIELD_OBJ_MAX, HEAPID_FRONTIERMAP);	[tomoya�ύX]

	//�X�N���[���f�[�^�`�惂�W���[��
	{
		WF2DMAP_SCRDRAWINIT init = {
			CLACT_U_EASYRENDER_SURFACE_MAIN,
			FRMAP_FRAME_MAP,
			GX_BG_COLORMODE_16,
			MAPBG_SCRBASE,
			MAPBG_CHARBASE,
			GX_BG_EXTPLTT_01,
			FRMAP_BGPRI_MAP,
			FALSE,
			0,
			0,
			TRUE,
		};
		int scroll_mode;
		
		init.arcid_scrn = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_ARCID);
		init.dataid_scrn = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_SCREENID);
		scroll_mode = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_SCROLL_MODE);
		if(scroll_mode == SCROLL_MODE_NORMAL){
			fmap->scrdrawsys = WF2DMAP_SCRDrawSysInit(CATS_EasyRenderGet(fmap->clactsys.csp), 
				fmap->bgl, &init, HEAPID_FRONTIERMAP);
		}

		//���d�ʃX�N���[���f�[�^�`�惂�W���[��
		if(FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_SCREENID) != MULTI_BG_NULL){
			init.dataid_scrn = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_SCREENID);
			init.bg_frame = FRMAP_FRAME_EFF;
			init.screenBase = EFFBG_SCRBASE;
			init.charbase = EFFBG_CHARBASE;
			init.priority = FRMAP_BGPRI_EFF;
			if(scroll_mode == SCROLL_MODE_NORMAL){
				fmap->multi_scrdrawsys = WF2DMAP_SCRDrawSysInit(
					CATS_EasyRenderGet(fmap->clactsys.csp), fmap->bgl, &init, HEAPID_FRONTIERMAP);
			}
		}
	}
	
	//�L���[���W���[��
	fmap->reqcmd_q = WF2DMAP_REQCMDQSysInit(FMAP_REQCMDQ_NUM, HEAPID_FRONTIERMAP);
	fmap->actcmd_q = WF2DMAP_ACTCMDQSysInit(FMAP_ACTCMDQ_NUM, HEAPID_FRONTIERMAP);


	//-- �f�[�^�Z�b�g --//
	
	//�}�b�v�f�[�^�ݒ�
	WF2DMAP_MAPSysDataSet(fmap->mapsys, DumyMap);
	
	//�}�b�v�O���t�B�b�N�ǂݍ���(�L�������p���b�g)
	//�L����
	{
		u32 arc_id;
		ARCHANDLE *hdl_bg;
		int text_mode = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_BG_MODE);
		
		arc_id = FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_ARCID);
		hdl_bg = ArchiveDataHandleOpen(arc_id, HEAPID_FRONTIERMAP);
		
		ArcUtil_HDL_BgCharSet(hdl_bg, FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_CHARID), 
			fmap->bgl, FRMAP_FRAME_MAP, 0, 0, TRUE, HEAPID_FRONTIERMAP);
		if(text_mode == GX_BGMODE_0){
			PaletteWorkSet_Arc(fmap->pfd, arc_id, 
				FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_PLTTID), HEAPID_FRONTIERMAP, 
				FADE_MAIN_BG, FFD_FIELD_PAL_SIZE, FFD_FIELD_PAL_START * 16);
		}
		else{
			// �p���b�g�f�[�^���g���p���b�gVRAM�ɓ]��
			NNSG2dPaletteData *palData;
			void *p_work;
			
			p_work = ArcUtil_HDL_PalDataGet(hdl_bg, 
				FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_PLTTID), 
				&palData, HEAPID_FRONTIERMAP);
			DC_FlushRange(palData->pRawData, palData->szByte);
			GX_BeginLoadBGExtPltt();	   // �p���b�g�f�[�^�̓]������
			GX_LoadBGExtPltt(palData->pRawData, 0x6000, 0x2000);
		    GX_EndLoadBGExtPltt();         // �p���b�g�f�[�^�̓]������
		    sys_FreeMemoryEz(p_work);
		}
		
		//��ʊO��^�����ɂ���ׁA�擪�̃p���b�g�ɋ����ō����Z�b�g
		PaletteWork_Clear(fmap->pfd, FADE_MAIN_BG, FADEBUF_ALL, 0x0000, 0, 1);

		ArcUtil_HDL_ScrnSet(hdl_bg, FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MAP_SCREENID),
			fmap->bgl, FRMAP_FRAME_MAP, 0, 0, 1, HEAPID_FRONTIERMAP);

		//���d��
		if(FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_SCREENID) != MULTI_BG_NULL){
			ArcUtil_HDL_BgCharSet(hdl_bg, 
				FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_CHARID), 
				fmap->bgl, FRMAP_FRAME_EFF, 0, 0, TRUE, HEAPID_FRONTIERMAP);
			ArcUtil_HDL_ScrnSet(hdl_bg, 
				FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_SCREENID),
				fmap->bgl, FRMAP_FRAME_EFF, 0, 0, 1, HEAPID_FRONTIERMAP);
			if(text_mode == GX_BGMODE_0){
				;
			}
			else{
				// �p���b�g�f�[�^���g���p���b�gVRAM�ɓ]��
				NNSG2dPaletteData *palData;
				void *p_work;
				
				p_work = ArcUtil_HDL_PalDataGet(hdl_bg, 
					FSS_SceneParamGet(scene_id, FSS_SCENE_DATA_MULTI_MAP_PLTTID), 
					&palData, HEAPID_FRONTIERMAP);
				DC_FlushRange(palData->pRawData, palData->szByte);
				GX_BeginLoadBGExtPltt();	   // �p���b�g�f�[�^�̓]������
				GX_LoadBGExtPltt(palData->pRawData, 0x4000, 0x2000);
			    GX_EndLoadBGExtPltt();         // �p���b�g�f�[�^�̓]������
			    sys_FreeMemoryEz(p_work);
			}
		}

		GF_BGL_LoadScreenV_Req(fmap->bgl, FRMAP_FRAME_MAP);
		
		ArchiveDataHandleClose(hdl_bg);
	}
}

//--------------------------------------------------------------
/**
 * @brief   2D�}�b�v�V�X�e�����W���[�����
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void MapSystem2D_ModuleFree(FMAP_PTR fmap)
{
	int i;
	
	//�I�u�W�F�N�g�Ǘ����W���[��
	{
		FSS_ACTOR_WORK *fss_actor;
		
		fss_actor = Frontier_ActorBufferGet(fmap->fmain);
		for(i = 0; i < FIELD_OBJ_MAX; i++){
			if(fss_actor[i].objwk != NULL){
				WF2DMAP_OBJWkDel(fss_actor[i].objwk);
				GF_ASSERT(fss_actor[i].anime_tcb == NULL);
			}
		}
	}
	WF2DMAP_OBJSysExit(fmap->objsys);
	
	//�}�b�v�f�[�^�Ǘ����W���[��
	WF2DMAP_MAPSysExit(fmap->mapsys);
	
	//�X�N���[���f�[�^�Ǘ����W���[��
	//���ꂾ�����̂Ȃ̂ŉ���̕K�v�Ȃ�
	
	//�I�u�W�F�N�g�f�[�^�`�惂�W���[��
//	for(i = 0; i < FIELD_OBJ_MAX; i++){
//		if(fmap->drawwk[i] != NULL){
//			WF2DMAP_OBJDrawWkDel(fmap->drawwk[i]);
//		}
//	}
//	WF2DMAP_OBJDrawSysResDel
	WF2DMAP_OBJDrawSysExit(fmap->drawsys);	//�S�����Ŏ����j��
	
	//�X�N���[���f�[�^�`�惂�W���[��
	if(fmap->scrdrawsys != NULL){
		WF2DMAP_SCRDrawSysExit(fmap->scrdrawsys);
	}
	if(fmap->multi_scrdrawsys != NULL){
		WF2DMAP_SCRDrawSysExit(fmap->multi_scrdrawsys);
	}

	//�L���[���W���[��
	WF2DMAP_REQCMDQSysExit(fmap->reqcmd_q);
	WF2DMAP_ACTCMDQSysExit(fmap->actcmd_q);
}


//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�p3DBG�������֐�
 * 
 * @param   �q�[�vID
 */
//--------------------------------------------------------------
static GF_G3DMAN * Frontier_3D_Init(int heap_id)
{
	GF_G3DMAN *g3Dman;
	
	//simple_3DBGInit(HEAPID_FRONTIERMAP);
	
	//�g��BG�ƃp���b�gVRAM�����p���邽�߁A�p���b�g16K�w��
	g3Dman = GF_G3DMAN_Init(heap_id, GF_G3DMAN_LNK, GF_G3DTEX_128K, 
		GF_G3DMAN_LNK, GF_G3DPLT_16K, Frontier_3D_SimpleSetUp);
	return g3Dman;
}

static void Frontier_3D_SimpleSetUp(void)
{
	// �R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority(1);

	// �e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );	// �A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );		// �A���t�@�u�����h�@�I��
	G3X_EdgeMarking( FALSE );
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	// �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(0,0,0),0,0x7fff,63,FALSE);	//color,alpha,depth,polygonID,fog

	// �r���[�|�[�g�̐ݒ�
    G3_ViewPort(0, 0, 255, 191);
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�p3DBG�I������
 *
 * @param   g3Dman		
 */
//--------------------------------------------------------------
static void Frontier_3D_Exit(GF_G3DMAN *g3Dman)
{
	//simple_3DBGExit();
	
	GF_G3D_Exit(g3Dman);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�V�X�e��������
 *
 *	@param	p_clact		�Z���A�N�^�[���[�N
 *	@param	heapID		�q�[�v
 *	@param	p_handle	�A�[�J�C�u�n���h��
 */
//-----------------------------------------------------------------------------
static void ActorSysInit(FMAP_PTR fmap)
{
	fmap->clactsys.csp=CATS_AllocMemory(HEAPID_FRONTIERMAP);
	CATS_SystemInit(fmap->clactsys.csp, &MapTcats,&MapCcmm, FMAP_OAM_PLTT_MAX);
	
	//�ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(
		NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
	
	fmap->clactsys.crp=CATS_ResourceCreate(fmap->clactsys.csp);
	CATS_ClactSetInit(fmap->clactsys.csp, fmap->clactsys.crp, FMAP_ACTOR_MAX);
	CATS_ResourceManagerInit(fmap->clactsys.csp,fmap->clactsys.crp,&MapResourceList);
	CLACT_U_SetSubSurfaceMatrix(
		CATS_EasyRenderGet(fmap->clactsys.csp), 0, FMAP_SUB_ACTOR_DISTANCE);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�V�X�e���j��
 *	
 *	@param	p_clact		�Z���A�N�^�[���[�N
 */
//-----------------------------------------------------------------------------
static void ActorSysFree(FMAP_PTR fmap)
{
	//�z�u���A�N�^�[�����\�[�X�폜
	{
		int i;
		
		//�A�N�^�[
		for(i = 0; i < ACTWORK_MAX; i++){
			if(fmap->article.act[i] != NULL){
				FAct_ActorDel(fmap->article.act[i]);
			}
		}
		//���\�[�X
		for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
			if(fmap->article.resource_id[i] != SCR_ACTWORK_RESOURCE_ID_NULL){
				FAct_ResourceFree(fmap->clactsys.crp, fmap->article.resource_id[i]);
			}
		}
	}
	
	//�|�P�����A�N�^�[�����\�[�X�폜
	{
		int i;
		
		for(i = 0; i < POKE_DISP_MANAGER_ID_NUM; i++){
			if(fmap->poke_cap[i] != NULL){
				//�A�N�^�[�폜
				CATS_ActorPointerDelete_S(fmap->poke_cap[i]);
				//���\�[�X�폜
				CATS_FreeResourceChar(fmap->clactsys.crp, POKE_DISP_MANAGER_ID_START + i);
				CATS_FreeResourcePltt(fmap->clactsys.crp, POKE_DISP_MANAGER_ID_START + i);
				CATS_FreeResourceCell(fmap->clactsys.crp, POKE_DISP_MANAGER_ID_START + i);
				CATS_FreeResourceCellAnm(fmap->clactsys.crp, POKE_DISP_MANAGER_ID_START + i);
			}
		}
	}

	CATS_ResourceDestructor_S(fmap->clactsys.csp,fmap->clactsys.crp);
	CATS_FreeMemory(fmap->clactsys.csp);
}

//--------------------------------------------------------------
/**
 * @brief   �L�����N�^���\�[�X�o�^
 *
 * @param   fmap		
 * @param   res			�o�^���\�[�X�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
void FMap_CharResourceSet(FMAP_PTR fmap, const FSS_CHAR_RESOURCE_DATA *res)
{
	FSS_CHAR_RESOURCE_DATA *manage;
	int i, setno;
	
	manage = Frontier_CharResourceBufferGet(fmap->fmain);
	
	//2�d�o�^�`�F�b�N
	for(i = 0; i < FIELD_OBJ_RESOURCE_MAX; i++){
		if(manage[i].charid == res->charid){
			OS_TPrintf("���\�[�X�o�^�ς�\n");
			return;		//���ɓo�^�ς�
		}
	}
	//�󂫃`�F�b�N
	for(i = 0; i < FIELD_OBJ_RESOURCE_MAX; i++){
		if(manage[i].charid == OBJCODEMAX){
			break;
		}
	}
	GF_ASSERT(i != FIELD_OBJ_RESOURCE_MAX);	//�󂫂Ȃ�

	setno = i;
	manage[setno] = *res;
	WF2DMAP_OBJDrawSysResSet(fmap->drawsys, res->charid, 
		res->movetype, HEAPID_FRONTIERMAP);
//		NNS_G2D_VRAM_TYPE_2DMAIN, res->movetype, HEAPID_FRONTIERMAP);	[tomoya�ύX]
}

//--------------------------------------------------------------
/**
 * @brief   �L�����N�^���\�[�X���
 *
 * @param   fmap		
 * @param   charid		�L�����N�^ID
 */
//--------------------------------------------------------------
void FMap_CharResourceFree(FMAP_PTR fmap, int charid)
{
	int i;
	FSS_CHAR_RESOURCE_DATA *manage;

	manage = Frontier_CharResourceBufferGet(fmap->fmain);
	for(i = 0; i < FIELD_OBJ_RESOURCE_MAX; i++){
		if(manage[i].charid == charid){
			WF2DMAP_OBJDrawSysResDel(fmap->drawsys, charid);
			manage[i].charid = OBJCODEMAX;
			return;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �A�N�^�[�o�^
 *
 * @param   fmap		
 * @param   res		 �o�^�A�N�^�[�f�[�^�ւ̃|�C���^
 * @param   pos_auto FMAP_ACT_POS_AUTO:�󂫗̈���������ēo�^�B
 * 					 FMAP_ACT_POS_AUTO�ȊO:pos_auto�œn����Ă���z��ԍ��ɓo�^(���@�o�^����POP�p)
 *
 * @retval  �������ꂽ�I�u�W�F�N�g���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
WF2DMAP_OBJWK * FMap_ActorSet(FMAP_PTR fmap, const FSS_CHAR_ACTOR_DATA *res, int pos_auto)
{
	FSS_ACTOR_WORK *fss_actor;
	int i, setno;
	WF2DMAP_OBJDATA objdata;
	WF2DMAP_OBJWK *objwk;
	WF2DMAP_OBJDRAWWK *drawwk;
	
	fss_actor = Frontier_ActorBufferGet(fmap->fmain);
	
	if(pos_auto == FMAP_ACT_POS_AUTO){
		//�󂫃`�F�b�N
		for(i = 0; i < FIELD_OBJ_MAX; i++){
			if(fss_actor[i].objwk == NULL){
				break;
			}
		}
		GF_ASSERT(i != FIELD_OBJ_MAX);	//�󂫂Ȃ�
		setno = i;
	}
	else{
		setno = pos_auto;
	}
	
	objdata.x = res->x;
	objdata.y = res->y;
	objdata.playid = res->playid;
	objdata.status = res->status;
	objdata.way = res->way;
	objdata.charaid = res->charid;
	
	objwk = WF2DMAP_OBJWkNew(fmap->objsys, &objdata);
	drawwk = WF2DMAP_OBJDrawWkNew(fmap->drawsys, objwk, FALSE, HEAPID_FRONTIERMAP);
	WF2DMAP_OBJDrawWkDrawFlagSet(drawwk, res->visible);
	
	Frontier_ActWorkDataSet(fmap->fmain, setno, objwk, drawwk, res);
	
	return objwk;
}

//--------------------------------------------------------------
/**
 * @brief   �A�N�^�[�폜
 *
 * @param   fmap		
 * @param   objwk		�I�u�W�F�N�g���[�N�̃|�C���^
 */
//--------------------------------------------------------------
void FMap_ActorFree(FMAP_PTR fmap, WF2DMAP_OBJWK *objwk)
{
	int i;
	FSS_ACTOR_WORK *fss_actor;

	fss_actor = Frontier_ActorBufferGet(fmap->fmain);
	for(i = 0; i < FIELD_OBJ_MAX; i++){
		if(fss_actor[i].objwk == objwk){
			WF2DMAP_OBJWkDel(fss_actor[i].objwk);
			WF2DMAP_OBJDrawWkDel(fss_actor[i].drawwk);
			
			GF_ASSERT(fss_actor[i].anime_tcb == NULL);
			MI_CpuClear8(&fss_actor[i], sizeof(FSS_ACTOR_WORK));
			fss_actor[i].param.playid = FIELD_PLAYID_NULL;
			return;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   playid����OBJWK�ADRAWWK�̃|�C���^���擾����
 *
 * @param   fmap		
 * @param   playid		�v���C���[����ID
 * @param   objwk		OBJWK�����(�s�v�ȏꍇ��NULL)
 * @param   drawwk		DRAWWK�����(�s�v�ȏꍇ��NULL)
 */
//--------------------------------------------------------------
void FMap_OBJWkGet(FMAP_PTR fmap, u16 playid, WF2DMAP_OBJWK **objwk, WF2DMAP_OBJDRAWWK **drawwk)
{
	int i;
	FSS_ACTOR_WORK *fss_actor;

	fss_actor = Frontier_ActorBufferGet(fmap->fmain);
	for(i = 0; i < FIELD_OBJ_MAX; i++){
		if(fss_actor[i].param.playid == playid){
			if(objwk != NULL){
				*objwk = fss_actor[i].objwk;
			}
			if(drawwk != NULL){
				*drawwk = fss_actor[i].drawwk;
			}
			return;
		}
	}
	
	OS_TPrintf("������Ȃ�playid = %d\n", playid);
	GF_ASSERT(0);
}

//==============================================================================
//	�z�u���A�N�^�[
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�Ő��������A�N�^�[�̃��\�[�XID���Ǘ��̈�ɋL������
 *
 * @param   fmap			
 * @param   act_id		�A�N�^�[ID
 *
 * ID�̊Ǘ������Ń��\�[�X�̃Z�b�g�͍s��Ȃ����Ƃɒ���
 */
//--------------------------------------------------------------
void FMap_ArticleActResourceIDSet(FMAP_PTR fmap, u16 act_id)
{
	int i;
	
	for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
		if(fmap->article.resource_id[i] == SCR_ACTWORK_RESOURCE_ID_NULL){
			fmap->article.resource_id[i] = act_id;
			return;
		}
	}
	
	GF_ASSERT(0);	//ID�Ǘ��̈�̋󂫂��Ȃ�
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�Ő��������A�N�^�[�̃��\�[�XID���Ǘ��̈悩��폜����
 *
 * @param   fmap		
 * @param   act_id		�A�N�^�[ID
 *
 * ID�̍폜�����Ń��\�[�X���͍̂폜���Ȃ����Ƃɒ���
 */
//--------------------------------------------------------------
void FMap_ArticleActResourceIDDel(FMAP_PTR fmap, u16 act_id)
{
	int i;
	
	for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
		if(fmap->article.resource_id[i] == act_id){
			fmap->article.resource_id[i] = SCR_ACTWORK_RESOURCE_ID_NULL;
			return;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���ŊǗ�����A�N�^�[�𐶐�����
 *
 * @param   fmap			
 * @param   work_no		���[�N�ԍ�(ACTWORK_?)
 * @param   cap			�A�N�^�[�|�C���^
 */
//--------------------------------------------------------------
CATS_ACT_PTR FMap_ArticleActCreate(FMAP_PTR fmap, u16 work_no, u16 act_id)
{
	CATS_ACT_PTR cap;
	
	GF_ASSERT(work_no < ACTWORK_MAX);
	GF_ASSERT(fmap->article.act[work_no] == NULL);
	
	cap = FAct_ActorSet(fmap->clactsys.csp, fmap->clactsys.crp, act_id);
	fmap->article.act[work_no] = cap;
	fmap->article.act_id[work_no] = act_id;
	
	FMap_ArticleActAnmBitSet(fmap, work_no, OFF);
	return cap;
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���ŊǗ�����A�N�^�[���폜����
 *
 * @param   fmap		
 * @param   work_no		���[�N�ԍ�(ACTWORK_?)
 */
//--------------------------------------------------------------
void FMap_ArticleActDel(FMAP_PTR fmap, u16 work_no)
{
	GF_ASSERT(work_no < ACTWORK_MAX);
	GF_ASSERT(fmap->article.act[work_no] != NULL);
	
	FAct_ActorDel(fmap->article.act[work_no]);
	fmap->article.act[work_no] = NULL;
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���ŊǗ����Ă���A�N�^�[�|�C���^���擾����
 *
 * @param   fmap		
 * @param   work_no		���[�N�ԍ�(ACTWORK_?)
 *
 * @retval  �A�N�^�[�|�C���^
 */
//--------------------------------------------------------------
CATS_ACT_PTR FMap_ArticleActGet(FMAP_PTR fmap, u16 work_no)
{
	return fmap->article.act[work_no];
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���ŊǗ�����A�N�^�[�̃A�j������ݒ�
 *
 * @param   work_no			���[�N�ԍ�(ACTWORK_?)
 * @param   anm_start		ON:�A�j��������B�@OFF�F�����Ȃ�
 */
//--------------------------------------------------------------
void FMap_ArticleActAnmBitSet(FMAP_PTR fmap, u16 work_no, int anm_start)
{
	if(anm_start == ON){
		fmap->article.anmbit |= 1 << work_no;
	}
	else{
		fmap->article.anmbit &= 0xffffffff ^ (1 << work_no);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�V�X�e���ŊǗ�����A�N�^�[�̃A�j������ݒ���擾����
 *
 * @param   work_no			���[�N�ԍ�(ACTWORK_?)
 * 
 * @retval  1=�A�j������L���B�@0=�A�j������ݒ肪����Ă��Ȃ�
 */
//--------------------------------------------------------------
u32 FMap_ArticleActAnmBitGet(FMAP_PTR fmap, u16 work_no)
{
	return (fmap->article.anmbit >> work_no) & 1;
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�Ő��������A�N�^�[��ޔ�
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void FMap_ArticleActPush(FMAP_PTR fmap)
{
	int i;
	PUSH_ARTICLE_WORK *push_buf;
	FMAP_ARTICLE_ACT *article;
	
	push_buf = Frontier_ArticlePushBufferGet(fmap->fmain);
	article = &fmap->article;
	
	//���\�[�XID�ޔ�
	for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
		if(article->resource_id[i] != SCR_ACTWORK_RESOURCE_ID_NULL){
			push_buf->resource_id[i] = article->resource_id[i];
			i++;
		}
	}
	
	//�A�N�^�[�p�����[�^�ޔ�
	i = 0;
	for(i = 0; i < ACTWORK_MAX; i++){
		if(article->act[i] != NULL){
			push_buf->act_param[i].anm_seq = CATS_ObjectAnimeSeqGetCap(article->act[i]);
			push_buf->act_param[i].anm_frame = CATS_ObjectAnimeFrameGetCap(article->act[i]);
			push_buf->act_param[i].anm_stop = FMap_ArticleActAnmBitGet(fmap, i);
			push_buf->act_param[i].visible = CATS_ObjectEnableGetCap(article->act[i]);
			push_buf->act_param[i].act_id = article->act_id[i];
			CATS_ObjectPosGetCap(article->act[i], 
				&push_buf->act_param[i].x, &push_buf->act_param[i].y);
			
			push_buf->act_param[i].occ = TRUE;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���v�g�Ő��������A�N�^�[�𕜋A
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
static void FMap_ArticleActPop(FMAP_PTR fmap)
{
	int i;
	ARCHANDLE *hdl_obj;
	PUSH_ARTICLE_WORK *push_buf;
	CATS_ACT_PTR cap;
	
	push_buf = Frontier_ArticlePushBufferGet(fmap->fmain);

	hdl_obj = ArchiveDataHandleOpen(ARC_FRONTIER_OBJ, HEAPID_FRONTIERMAP);
	
	//���\�[�X���A
	for(i = 0; i < ACTWORK_RESOURCE_MAX; i++){
		if(push_buf->resource_id[i] != SCR_ACTWORK_RESOURCE_ID_NULL){
			FAct_ResourceLoad(fmap->clactsys.csp, fmap->clactsys.crp, 
				hdl_obj, fmap->pfd, push_buf->resource_id[i]);
			FMap_ArticleActResourceIDSet(fmap, push_buf->resource_id[i]);
		}
	}
	
	//�A�N�^�[���A
	for(i = 0; i < ACTWORK_MAX; i++){
		if(push_buf->act_param[i].occ == TRUE){
			cap = FMap_ArticleActCreate(fmap, i, push_buf->act_param[i].act_id);
			CATS_ObjectPosSetCap(cap, push_buf->act_param[i].x, push_buf->act_param[i].y);
			CATS_ObjectEnableCap(cap, push_buf->act_param[i].visible);
			FMap_ArticleActAnmBitSet(fmap, i, push_buf->act_param[i].anm_stop);
			
			CATS_ObjectAnimeSeqSetCap(cap, push_buf->act_param[i].anm_seq);
			CATS_ObjectAnimeFrameSetCap(cap, push_buf->act_param[i].anm_frame);
		}
	}
	
	ArchiveDataHandleClose(hdl_obj);

	//���A�����������̂őޔ��o�b�t�@��������
	Frontier_ArticlePushBufferInit(fmap->fmain);
}

//--------------------------------------------------------------
/**
 * @brief   �X�N���[�����W�擾
 *
 * @param   fmap		
 * @param   ret_x		X���W�����
 * @param   ret_y		Y���W�����
 */
//--------------------------------------------------------------
void FMap_ScrollPosGet(FMAP_PTR fmap, s16 *ret_x, s16 *ret_y)
{
	// �X�N���[�����W����X�N���[�����W���e�\���V�X�e���ɐݒ�
	*ret_y = WF2DMAP_SCROLLSysTopGet(&fmap->scrollsys);
	*ret_x = WF2DMAP_SCROLLSysLeftGet(&fmap->scrollsys);
	OS_TPrintf("surface scroll x = %d, y = %d\n", *ret_x, *ret_y);
}

//--------------------------------------------------------------
/**
 * @brief   FSS_ACTOR_WORK�̃f�[�^�����ɂ���FSS_CHAR_ACTOR_DATA�����o��
 *
 * @param   fss_actor		�쐬���f�[�^�ɂȂ�FSS_ACTOR_WORK
 * @param   act_data		�f�[�^�o�͐�
 */
//--------------------------------------------------------------
static void Frontier_ActWork_to_ActData(FSS_ACTOR_WORK *fss_actor, FSS_CHAR_ACTOR_DATA *act_data)
{
	*act_data = fss_actor->param;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�A�N�^�[���[�N�Ƀf�[�^���Z�b�g����
 *
 * @param   act_no		�A�N�^�[�ԍ�
 * @param   objwk		OBJWK
 * @param   drawwk		DRAWWK
 * @param   res			
 */
//--------------------------------------------------------------
static void Frontier_ActWorkDataSet(FMAIN_PTR fmain, int act_no, WF2DMAP_OBJWK *objwk, WF2DMAP_OBJDRAWWK *drawwk, const FSS_CHAR_ACTOR_DATA *res)
{
	FSS_ACTOR_WORK *fss_actor;
	
	fss_actor = Frontier_ActorWorkPtrGet(fmain, act_no);

//POP����move�������Ȃ��悤�ɂ���ׁA�S�̃N���A���Ȃ��B
//ActorFree����Clear���Ă���̂ŁA�����ŃN���A���Ȃ����͖��Ȃ��B
//	MI_CpuClear8(fss_actor, sizeof(FSS_ACTOR_WORK));
	
	fss_actor->objwk = objwk;
	fss_actor->drawwk = drawwk;
	fss_actor->param = *res;
}

//--------------------------------------------------------------
/**
 * @brief   playid����t�����e�B�A�A�N�^�[���[�N�|�C���^���擾����
 *
 * @param   fmain		
 * @param   playid		
 *
 * @retval  �t�����e�B�A�A�N�^�[���[�N
 */
//--------------------------------------------------------------
FSS_ACTOR_WORK * Frontier_ActorWorkSearch(FMAIN_PTR fmain, int playid)
{
	int i;
	FSS_ACTOR_WORK *fss_actor;
	
	fss_actor = Frontier_ActorBufferGet(fmain);
	for(i = 0; i < FIELD_OBJ_MAX; i++){
		if(fss_actor->objwk != NULL && fss_actor->param.playid == playid){
			return fss_actor;
		}
		fss_actor++;
	}
	GF_ASSERT(0);
	return NULL;
}
