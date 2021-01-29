//==============================================================================
/**
 * @file	ending.c
 * @brief	�G���f�B���O ���C������
 * @author	matsuda
 * @date	2008.04.07(��)
 */
//==============================================================================
#include "common.h"
#include "gflib\camera.h"
#include "system\procsys.h"
#include "system\msgdata.h"
#include "system\font_arc.h"
#include "system\brightness.h"
#include "system\clact_util.h"
#include "system\render_oam.h"
#include "system\snd_tool.h"
#include "savedata\zukanwork.h"
#include "system/brightness.h"
#include "system/d3dobj.h"
#include "system/wipe.h"
#include "system/wipe_def.h"
#include "system/procsys.h"

#include "msgdata\msg.naix"

#include "demo\ending.h"
#include "graphic/ending.naix"
#include "ending_common.h"
#include "ending_setup.h"
#include "ending_tool.h"
#include "ending_list.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�G���f�B���O�Ŏg�p����q�[�v�T�C�Y
#define HEAPSIZE_ENDING_DEMO	(0x80000)


//--------------------------------------------------------------
//	CL_ACT�p�̒萔��`
//--------------------------------------------------------------
///���C��	OAM�Ǘ��̈�E�J�n
#define ENDING_OAM_START_MAIN			(0)
///���C��	OAM�Ǘ��̈�E�I��
#define ENDING_OAM_END_MAIN				(128)
///���C��	�A�t�B���Ǘ��̈�E�J�n
#define ENDING_OAM_AFFINE_START_MAIN		(0)
///���C��	�A�t�B���Ǘ��̈�E�I��
#define ENDING_OAM_AFFINE_END_MAIN		(32)
///�T�u	OAM�Ǘ��̈�E�J�n
#define ENDING_OAM_START_SUB				(0)
///�T�u	OAM�Ǘ��̈�E�I��
#define ENDING_OAM_END_SUB				(128)
///�T�u �A�t�B���Ǘ��̈�E�J�n
#define ENDING_OAM_AFFINE_START_SUB		(0)
///�T�u	�A�t�B���Ǘ��̈�E�I��
#define ENDING_OAM_AFFINE_END_SUB		(32)

///�L�����}�l�[�W���F�L�����N�^ID�Ǘ���(���ʁ{�����)
#define ENDING_CHAR_MAX					(48 + 48)
///�L�����}�l�[�W���F���C����ʃT�C�Y(byte�P��)
#define ENDING_CHAR_VRAMSIZE_MAIN		(1024 * 0x40)	//64K
///�L�����}�l�[�W���F�T�u��ʃT�C�Y(byte�P��)
#define ENDING_CHAR_VRAMSIZE_SUB			(512 * 0x20)	//32K

///���C����ʁ{�T�u��ʂŎg�p����A�N�^�[����
#define ENDING_ACTOR_MAX					(64 + 64)	//���C����� + �T�u���

///OBJ�Ŏg�p����p���b�g�{��(���ʁ{�����)
#define ENDING_OAM_PLTT_MAX				(16 + 16)

///OAM���\�[�X�F�L�����o�^�ő吔(���C����� + �T�u���)
#define ENDING_OAMRESOURCE_CHAR_MAX		(ENDING_CHAR_MAX)
///OAM���\�[�X�F�p���b�g�o�^�ő吔(���C����� + �T�u���)
#define ENDING_OAMRESOURCE_PLTT_MAX		(ENDING_OAM_PLTT_MAX)
///OAM���\�[�X�F�Z���o�^�ő吔
#define ENDING_OAMRESOURCE_CELL_MAX		(64)
///OAM���\�[�X�F�Z���A�j���o�^�ő吔
#define ENDING_OAMRESOURCE_CELLANM_MAX	(64)
///OAM���\�[�X�F�}���`�Z���o�^�ő吔
#define ENDING_OAMRESOURCE_MCELL_MAX		(8)
///OAM���\�[�X�F�}���`�Z���A�j���o�^�ő吔
#define ENDING_OAMRESOURCE_MCELLANM_MAX	(8)



//--------------------------------------------------------------
//	�J�����ݒ�
//--------------------------------------------------------------
#define FOOTPRINT_CAMERA_MODE			GF_CAMERA_PERSPECTIV	//(GF_CAMERA_ORTHO)

#define FOOTPRINT_CAMERA_PERSPWAY		(FX_GET_ROTA_NUM(22))
#define FOOTPRINT_CAMERA_NEAR			( FX32_ONE )
#define FOOTPRINT_CAMERA_FAR			( FX32_ONE * 900 )
#define FOOTPRINT_CAMERA_TX				( 0 )		/// target
#define FOOTPRINT_CAMERA_TY				( -FX32_ONE * 8 )
#define FOOTPRINT_CAMERA_TZ				( 0 )

///�J�����̒����_�܂ł̋���
#define FOOTPRINT_CAMERA_DISTANCE		(0x7c000)	//(0x96 << FX32_SHIFT)

///�J�����A���O��
static const CAMERA_ANGLE EndingCameraAngle = {
	FX_GET_ROTA_NUM(0), FX_GET_ROTA_NUM(0), FX_GET_ROTA_NUM(0),
};

//==============================================================================
//	CLACT�p�f�[�^
//==============================================================================
static	const TCATS_OAM_INIT EndingTcats = {
	ENDING_OAM_START_MAIN, ENDING_OAM_END_MAIN,
	ENDING_OAM_AFFINE_START_MAIN, ENDING_OAM_AFFINE_END_MAIN,
	ENDING_OAM_START_SUB, ENDING_OAM_END_SUB,
	ENDING_OAM_AFFINE_START_SUB, ENDING_OAM_AFFINE_END_SUB,
};

static	const TCATS_CHAR_MANAGER_MAKE EndingCcmm = {
	ENDING_CHAR_MAX,
	ENDING_CHAR_VRAMSIZE_MAIN,
	ENDING_CHAR_VRAMSIZE_SUB,
	GX_OBJVRAMMODE_CHAR_1D_128K,
	GX_OBJVRAMMODE_CHAR_1D_128K
};

static const TCATS_RESOURCE_NUM_LIST EndingResourceList = {
	ENDING_OAMRESOURCE_CHAR_MAX,
	ENDING_OAMRESOURCE_PLTT_MAX,
	ENDING_OAMRESOURCE_CELL_MAX,
	ENDING_OAMRESOURCE_CELLANM_MAX,
	ENDING_OAMRESOURCE_MCELL_MAX,
	ENDING_OAMRESOURCE_MCELLANM_MAX,
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void Ending_Update(TCB_PTR tcb, void *work);
static void VBlankFunc( void * work );
static void Ending_VramBankSet(GF_BGL_INI *bgl);
static void BgExit( GF_BGL_INI * ini );
static void Model3D_Update(ENDING_MAIN_WORK *emw);
static void Ending_CameraInit(ENDING_MAIN_WORK *emw);
static void Ending_CameraExit(ENDING_MAIN_WORK *emw);
static GF_G3DMAN * Ending_3D_Init(int heap_id);
static void EndingSimpleSetUp(void);
static void Ending_3D_Exit(GF_G3DMAN *g3Dman);
static void Debug_CameraMove(ENDING_MAIN_WORK *emw);
static void Ending_DefaultBGLoad(ENDING_MAIN_WORK *emw);
static void EndingSceneVramExit(ENDING_MAIN_WORK *emw);
static void FinalSceneVramExit(ENDING_MAIN_WORK *emw);
static void Ending_FinalVramBankSet(GF_BGL_INI *bgl);


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
PROC_RESULT EndingProc_Init( PROC * proc, int * seq )
{
	ENDING_MAIN_WORK *emw;
	
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

	sys_CreateHeap(HEAPID_BASE_APP, HEAPID_ENDING_DEMO, HEAPSIZE_ENDING_DEMO);

	emw = PROC_AllocWork(proc, sizeof(ENDING_MAIN_WORK), HEAPID_ENDING_DEMO );
	MI_CpuClear8(emw, sizeof(ENDING_MAIN_WORK));
	emw->parent_work = PROC_GetParentWork( proc );
	
//	simple_3DBGInit(HEAPID_ENDING_DEMO);
	emw->g3Dman = Ending_3D_Init(HEAPID_ENDING_DEMO);

	//�p���b�g�t�F�[�h�V�X�e���쐬
	emw->pfd = PaletteFadeInit(HEAPID_ENDING_DEMO);
	PaletteFadeWorkAllocSet(emw->pfd, FADE_MAIN_BG, 0x200, HEAPID_ENDING_DEMO);
	PaletteFadeWorkAllocSet(emw->pfd, FADE_SUB_BG, 0x200, HEAPID_ENDING_DEMO);
	PaletteFadeWorkAllocSet(emw->pfd, FADE_MAIN_OBJ, 0x200-0x40, HEAPID_ENDING_DEMO);	//�ʐM�A�C�R��-���[�J���C�Y�p
	PaletteFadeWorkAllocSet(emw->pfd, FADE_SUB_OBJ, 0x200, HEAPID_ENDING_DEMO);
	PaletteTrans_AutoSet(emw->pfd, TRUE);
	
	emw->bgl = GF_BGL_BglIniAlloc(HEAPID_ENDING_DEMO);

	initVramTransferManagerHeap(64, HEAPID_ENDING_DEMO);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	Ending_VramBankSet(emw->bgl);

	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);

	//���b�Z�[�W�}�l�[�W���쐬
	emw->msgman = MSGMAN_Create( 
		MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_stafflist_dat, HEAPID_ENDING_DEMO );

	//�t�@�C���n���h���J�����ςȂ��ɂ���
	emw->ending_hdl = ArchiveDataHandleOpen(ARC_ENDING, HEAPID_ENDING_DEMO);

	//�J�����쐬
	Ending_CameraInit(emw);

	//�A�N�^�[�V�X�e���쐬
	emw->csp=CATS_AllocMemory(HEAPID_ENDING_DEMO);
	CATS_SystemInit(emw->csp,&EndingTcats,&EndingCcmm,ENDING_OAM_PLTT_MAX);
	//�ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(
		NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
	emw->crp=CATS_ResourceCreate(emw->csp);
	CATS_ClactSetInit(emw->csp, emw->crp, ENDING_ACTOR_MAX);
	CATS_ResourceManagerInit(emw->csp,emw->crp,&EndingResourceList);
	CLACT_U_SetSubSurfaceMatrix(CATS_EasyRenderGet(emw->csp), 0, ENDING_SUB_ACTOR_DISTANCE);

	//�풓�O���t�B�b�N�Z�b�g
	Ending_DefaultBGLoad(emw);

	//�X�^�b�t���[������V�X�e��
	emw->list_work = PlEnding_ListWork_Create( 
		emw->bgl, -240, FRAME_M_STAFFROLL, SYSTEM_FONT_PAL, emw->msgman );
	emw->list_work_sub = PlEnding_ListWork_Create( 
		emw->bgl, -240-192-16, FRAME_S_STAFFROLL, SYSTEM_FONT_PAL, emw->msgman );

	// �X�^�b�t���[���̉��͈͐ݒ�
//	G2_SetWnd0Position( 0, 94, 255, 192 );
	G2_SetWnd0Position( 0, 192-24, 255, 192 );
	G2_SetWnd0InsidePlane( GX_WND_PLANEMASK_ALL ^ GX_WND_PLANEMASK_BG1, TRUE );
	G2_SetWndOutsidePlane( GX_WND_PLANEMASK_ALL, TRUE );

	G2S_SetWnd0Position( 0, 0, 255, 24 );
	G2S_SetWnd0InsidePlane( GX_WND_PLANEMASK_ALL ^ GX_WND_PLANEMASK_BG1, TRUE );
	G2S_SetWndOutsidePlane( GX_WND_PLANEMASK_ALL, TRUE );

	GX_SetVisibleWnd(GX_WNDMASK_W0);
	GXS_SetVisibleWnd(GX_WNDMASK_W0);
	
	//���C����ʐݒ�
	sys.disp3DSW = DISP_3D_TO_SUB;
	GF_Disp_DispSelect();
	GF_Disp_DispOn();
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);
	GF_Disp_GXS_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	emw->update_tcb = TCB_Add(Ending_Update, emw, 60000);

	sys_VBlankFuncChange(VBlankFunc, emw);

	Snd_DataSetByScene( SND_SCENE_ENDING, SEQ_BLD_ENDING, 1 );	//�G���f�B���O�ȍĐ�

	//�^���Âɂ��Ă���
	SetBrightness(BRIGHTNESS_BLACK, ENDING_FADE_PLANE, MASK_DOUBLE_DISPLAY);
	WIPE_SetMstBrightness( WIPE_DISP_MAIN, 0 );
	WIPE_SetMstBrightness( WIPE_DISP_SUB, 0 );

	GF_BGL_VisibleSet( FRAME_M_STAFFROLL, VISIBLE_ON );
	GF_BGL_VisibleSet( FRAME_S_STAFFROLL, VISIBLE_ON );

#ifdef PM_DEBUG
//	emw->scene_no = 4;
//	emw->main_counter = ENDING_COUNTER_CAP_SCENE2_END;
#endif

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
PROC_RESULT EndingProc_Main( PROC * proc, int * seq )
{
	ENDING_MAIN_WORK * emw  = PROC_GetWork( proc );
	
	enum{
		SKIPSEQ_NO_SKIP = 0,
		SKIPSEQ_START,
		SKIPSEQ_SKIP_END,
	};
	
	enum{
		SEQ_SCENE_LOAD = 0,
		SEQ_SCENE_MAIN,
		SEQ_OUT,
	};
	
	//�X�^�b�t���[������
	if(emw->list_work != NULL){
		PlEnding_ListWork_Scroll( emw->list_work, 1 );
		emw->list_end_flag = PlEnding_ListWork_Scroll( emw->list_work_sub, 1 );
	}

	if(emw->parent_work->clearFlag && (sys.trg & PAD_BUTTON_START)){
		if(emw->skip_seq == SKIPSEQ_NO_SKIP && emw->scene_no < ENDING_SCENE_COPYRIGHT){
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_ENDING_DEMO );
			emw->skip_seq = SKIPSEQ_START;
		}
	}
	//�X�L�b�v����
	switch(emw->skip_seq){
	case SKIPSEQ_NO_SKIP:
		break;
	case SKIPSEQ_START:
		if( WIPE_SYS_EndCheck() ){
			if(emw->scene_init_ok == TRUE){
				EndingSceneSetup_Unload(emw, emw->scene_no);
				emw->scene_init_ok = FALSE;
			}
			//�P�x�ύX���N�G�X�g���������Ă����ꍇ�͋����I��
			if(IsFinishedBrightnessChg(MASK_DOUBLE_DISPLAY) == FALSE){
				BrightnessChgReset(MASK_DOUBLE_DISPLAY);
			}
			//Vram�ݒ�ύX(���ɕύX�ς݂̏ꍇ�͒��ł͂������)
			FinalSceneVramSetup(emw);
			
			emw->scene_no = ENDING_SCENE_COPYRIGHT;
			GF_Disp_GX_VisibleControlDirect(0);
			GF_Disp_GXS_VisibleControlDirect(0);
//			WIPE_SetMstBrightness( WIPE_DISP_MAIN, 0 );
//			WIPE_SetMstBrightness( WIPE_DISP_SUB, 0 );
			MI_CpuClear8(&emw->scene_work, sizeof(ENDING_SCENE_WORK));
			
			emw->skip_seq = SKIPSEQ_SKIP_END;
			*seq = SEQ_SCENE_LOAD;
		}
		break;
	}
	
	
	//���C��
	switch(*seq){
	case SEQ_SCENE_LOAD:
		if(emw->skip_seq == SKIPSEQ_NO_SKIP || emw->skip_seq == SKIPSEQ_SKIP_END){
			EndingSceneSetup_Load(emw, emw->scene_no);
			emw->scene_init_ok = TRUE;
			(*seq)++;
		}
		break;
	case SEQ_SCENE_MAIN:
		if(EndingSceneSetup_MainFunc(emw, emw->scene_no) == TRUE){
			EndingSceneSetup_Unload(emw, emw->scene_no);
			emw->scene_init_ok = FALSE;
			emw->scene_no++;
			if(emw->scene_no < ENDING_SCENE_MAX){
				(*seq) = SEQ_SCENE_LOAD;
			}
			else{
				(*seq) = SEQ_OUT;
			}
		}
		break;
	case SEQ_OUT:
		return PROC_RES_FINISH;
	}
	
#ifdef PM_DEBUG
#ifdef DEBUG_ENDING_CAMERA
	if(emw->debug_mode == 0){
		//�f�o�b�O�J�����ړ�
		Debug_CameraMove(emw);
	}
	else{
		//�f�o�b�O�z�u���̔z�u�Ԋu����
		EndingTool_Debug_ModelLineArrangeSetting(emw, MODEL_LINE_0);
	}
	if(sys.trg & PAD_BUTTON_SELECT){
		emw->debug_mode ^= 1;
	}
#endif
#endif
	
	emw->main_counter++;
	if(emw->main_counter % 30 == 0){
		OS_TPrintf("main_counter = %d\n", emw->main_counter / 30);
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
PROC_RESULT EndingProc_End( PROC * proc, int * seq )
{
	ENDING_MAIN_WORK * emw  = PROC_GetWork( proc );

	TCB_Delete(emw->update_tcb);

	if(emw->list_work != NULL){
		EndingSceneVramExit(emw);
	}
	else{
		FinalSceneVramExit(emw);
	}

	// ���b�Z�[�W�}�l�[�W���[�E���[�h�Z�b�g�}�l�[�W���[���
	MSGMAN_Delete( emw->msgman );
	

	//�A�N�^�[�V�X�e���폜
	CATS_ResourceDestructor_S(emw->csp,emw->crp);
	CATS_FreeMemory(emw->csp);

	//�p���b�g�t�F�[�h�V�X�e���폜
	PaletteFadeWorkAllocFree(emw->pfd, FADE_MAIN_BG);
	PaletteFadeWorkAllocFree(emw->pfd, FADE_SUB_BG);
	PaletteFadeWorkAllocFree(emw->pfd, FADE_MAIN_OBJ);
	PaletteFadeWorkAllocFree(emw->pfd, FADE_SUB_OBJ);
	PaletteFadeFree(emw->pfd);

	//�J�����폜
	Ending_CameraExit(emw);

	//�n���h������
	ArchiveDataHandleClose( emw->ending_hdl );

	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	//Vram�]���}�l�[�W���[�폜
	DellVramTransferManager();

	StopTP();		//�^�b�`�p�l���̏I��

	PROC_FreeWork( proc );				// PROC���[�N�J��
	sys_DeleteHeap( HEAPID_ENDING_DEMO );

	return PROC_RES_FINISH;
}


//==============================================================================
//	
//==============================================================================
static void Ending_Update(TCB_PTR tcb, void *work)
{
	ENDING_MAIN_WORK *emw = work;
	
	if(emw->draw_3d == TRUE){
		Model3D_Update(emw);
		GF_G3_RequestSwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
	}

	CATS_Draw(emw->crp);
	CATS_UpdateTransfer();
}

//--------------------------------------------------------------
/**
 * @brief   
 *
 * @param   emw		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void EndingSceneVramExit(ENDING_MAIN_WORK *emw)
{
	GF_ASSERT(emw->list_work != NULL);
	
	PlEnding_ListWork_Delete( emw->list_work_sub );
	PlEnding_ListWork_Delete( emw->list_work );
	emw->list_work_sub = NULL;
	emw->list_work = NULL;
	GF_BGL_VisibleSet( FRAME_M_STAFFROLL, VISIBLE_OFF );
	GF_BGL_VisibleSet( FRAME_S_STAFFROLL, VISIBLE_OFF );

	// BG_SYSTEM���
	GF_BGL_BGControlExit( emw->bgl, FRAME_S_BG );
	GF_BGL_BGControlExit( emw->bgl, FRAME_S_STAFFROLL );
	GF_BGL_BGControlExit( emw->bgl, FRAME_S_EFF );
	GF_BGL_BGControlExit( emw->bgl, FRAME_M_CAPTURE );
	GF_BGL_BGControlExit( emw->bgl, FRAME_M_BG );
	GF_BGL_BGControlExit( emw->bgl, FRAME_M_STAFFROLL );
	sys_FreeMemoryEz( emw->bgl );

	//simple_3DBGExit();
	Ending_3D_Exit(emw->g3Dman);
}

//--------------------------------------------------------------
/**
 * @brief   �t�B�i�[���p��VRAM�o���N�ݒ��ύX
 *
 * @param   emw		
 */
//--------------------------------------------------------------
void FinalSceneVramSetup(ENDING_MAIN_WORK *emw)
{
	if(emw->list_work == NULL){
		return;	//���ɕύX����Ă���
	}
	EndingSceneVramExit(emw);
	
	emw->bgl = GF_BGL_BglIniAlloc(HEAPID_ENDING_DEMO);
	Ending_FinalVramBankSet(emw->bgl);
}

//--------------------------------------------------------------
/**
 * @brief   
 *
 * @param   emw		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void FinalSceneVramExit(ENDING_MAIN_WORK *emw)
{
	GF_ASSERT(emw->list_work == NULL);
	
	// BG_SYSTEM���
	GF_BGL_BGControlExit( emw->bgl, FRAME_FIN_M_BG );
	GF_BGL_BGControlExit( emw->bgl, FRAME_FIN_M_CAP_A );
	GF_BGL_BGControlExit( emw->bgl, FRAME_FIN_M_CAP_B );
	GF_BGL_BGControlExit( emw->bgl, FRAME_FIN_S_BG );
	sys_FreeMemoryEz( emw->bgl );
}

//--------------------------------------------------------------------------------------------
/**
 * VBlank�֐�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VBlankFunc( void * work )
{
	ENDING_MAIN_WORK *emw = work;
	
	// �Z���A�N�^�[Vram�]���}�l�[�W���[���s
	DoVramTransferManager();

	// �����_�����LOAM�}�l�[�W��Vram�]��
	CATS_RenderOamTrans();
	PaletteFadeTrans(emw->pfd);
	
	GF_BGL_VBlankFunc(emw->bgl);

	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Ending_VramBankSet(GF_BGL_INI *bgl)
{
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_B,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_23_G,			// ���C��2D�G���W����BG�g���p���b�g

			GX_VRAM_BG_128_C,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

			GX_VRAM_TEX_0_A,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_0_F			// �e�N�X�`���p���b�g�X���b�g
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
			GX_DISPMODE_GRAPHICS, GX_BGMODE_3, GX_BGMODE_0, GX_BG0_AS_3D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME_M_STAFFROLL	�X�^�b�t���[��
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_M_STAFFROLL, 0, 0, FALSE
			},
			///<FRAME_M_BG			�w�i
			{
				0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_M_BG, 0, 0, FALSE
			},
			///<FRAME_M_CAPTURE		�L���v�`���摜(�g��BG)
			{
				0, 0, 0, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_256,
				GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_23,
				FRAME_BGPRI_M_CAPTURE, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FRAME_M_STAFFROLL, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_M_STAFFROLL );
		GF_BGL_ScrollSet(bgl, FRAME_M_STAFFROLL, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_M_STAFFROLL, GF_BGL_SCROLL_Y_SET, 0);

		GF_BGL_BGControlSet(bgl, FRAME_M_BG, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_M_BG );
		GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, FRAME_M_CAPTURE, &TextBgCntDat[2], GF_BGL_MODE_256X16 );
		//GF_BGL_ScrClear(bgl, FRAME_M_CAPTURE );
		GF_BGL_ScrollSet(bgl, FRAME_M_CAPTURE, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_M_CAPTURE, GF_BGL_SCROLL_Y_SET, 0);

		//3D��
		G2_SetBG0Priority(FRAME_BGPRI_3D);
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME_S_STAFFROLL	�X�^�b�t���[��
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_S_STAFFROLL, 0, 0, FALSE
			},
			///<FRAME_S_EFF			�G�t�F�N�g(���󖢎g�p)
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_S_EFF, 0, 0, FALSE
			},
			///<FRAME_S_BG			�w�i
			{
				0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_S_BG, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FRAME_S_STAFFROLL, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_S_STAFFROLL );
		GF_BGL_ScrollSet(bgl, FRAME_S_STAFFROLL, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_S_STAFFROLL, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, FRAME_S_EFF, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_S_EFF );
		GF_BGL_ScrollSet(bgl, FRAME_S_EFF, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_S_EFF, GF_BGL_SCROLL_Y_SET, 0);

		GF_BGL_BGControlSet(bgl, FRAME_S_BG, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_S_BG );
		GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_Y_SET, 0);
	}

	GF_BGL_ClearCharSet( FRAME_M_STAFFROLL, 32, 0, HEAPID_ENDING_DEMO );
	GF_BGL_ClearCharSet( FRAME_S_STAFFROLL, 32, 0, HEAPID_ENDING_DEMO );
	GF_BGL_ClearCharSet( FRAME_S_EFF, 32, 0, HEAPID_ENDING_DEMO );
	GF_BGL_ClearCharSet( FRAME_S_BG, 32, 0, HEAPID_ENDING_DEMO );
	
	GF_BGL_VisibleSet( FRAME_M_CAPTURE, VISIBLE_OFF );
	GF_BGL_VisibleSet( FRAME_S_EFF, VISIBLE_OFF );
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��(�t�B�i�[���p)
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Ending_FinalVramBankSet(GF_BGL_INI *bgl)
{
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_256_AB,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_23_G,			// ���C��2D�G���W����BG�g���p���b�g

			GX_VRAM_BG_128_C,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

			GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
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
			GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME_FIN_M_BG			�w�i
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_M_BG, 0, 0, FALSE
			},
			///<FRAME_FIN_M_CAP_A		�L���v�`���摜(�g��BG)
			{
				0, 0, 0, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_256,
				GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x20000, GX_BG_EXTPLTT_23,
				FRAME_BGPRI_FIN_M_CAPTURE_TOP, 0, 0, FALSE
			},
			///<FRAME_FIN_M_CAP_B		�L���v�`���摜(�g��BG)
			{
				0, 0, 0, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_256,
				GX_BG_SCRBASE_0x2800, GX_BG_CHARBASE_0x30000, GX_BG_EXTPLTT_23,
				FRAME_BGPRI_FIN_M_CAPTURE_BOTTOM, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FRAME_FIN_M_BG, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_FIN_M_BG );
		GF_BGL_ScrollSet(bgl, FRAME_FIN_M_BG, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_FIN_M_BG, GF_BGL_SCROLL_Y_SET, 0);

		GF_BGL_BGControlSet(bgl, FRAME_FIN_M_CAP_A, &TextBgCntDat[1], GF_BGL_MODE_256X16 );
		GF_BGL_ScrClear(bgl, FRAME_FIN_M_CAP_A );
		GF_BGL_ScrollSet(bgl, FRAME_FIN_M_CAP_A, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_FIN_M_CAP_A, GF_BGL_SCROLL_Y_SET, 0);
		
		GF_BGL_BGControlSet(bgl, FRAME_FIN_M_CAP_B, &TextBgCntDat[2], GF_BGL_MODE_256X16 );
		GF_BGL_ScrClear(bgl, FRAME_FIN_M_CAP_B );
		GF_BGL_ScrollSet(bgl, FRAME_FIN_M_CAP_B, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_FIN_M_CAP_B, GF_BGL_SCROLL_Y_SET, 0);
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME_FIN_S_BG			�w�i
			{
				0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
				FRAME_BGPRI_S_BG, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FRAME_FIN_S_BG, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FRAME_FIN_S_BG );
		GF_BGL_ScrollSet(bgl, FRAME_FIN_S_BG, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FRAME_FIN_S_BG, GF_BGL_SCROLL_Y_SET, 0);
	}

	GF_BGL_ClearCharSet( FRAME_FIN_S_BG, 32, 0, HEAPID_ENDING_DEMO );
}

//--------------------------------------------------------------------------------------------
/**
 * BG���
 *
 * @param	ini		BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgExit( GF_BGL_INI * ini )
{
	GF_BGL_BGControlExit( ini, FRAME_S_BG );
	GF_BGL_BGControlExit( ini, FRAME_S_STAFFROLL );
	GF_BGL_BGControlExit( ini, FRAME_S_EFF );
	GF_BGL_BGControlExit( ini, FRAME_M_CAPTURE );
	GF_BGL_BGControlExit( ini, FRAME_M_BG );
	GF_BGL_BGControlExit( ini, FRAME_M_STAFFROLL );

}

//--------------------------------------------------------------
/**
 * @brief   �S�V�[�����ʂŎg�p����O���t�B�b�N�f�[�^��ݒ�
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Ending_DefaultBGLoad(ENDING_MAIN_WORK *emw)
{
	//�V�X�e���t�H���g�p���b�g�]��
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_font_NCLR, HEAPID_ENDING_DEMO, 
		FADE_MAIN_BG, 0x20, SYSTEM_FONT_PAL * 16);
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_font_NCLR, HEAPID_ENDING_DEMO, 
		FADE_SUB_BG, 0x20, SYSTEM_FONT_PAL * 16);
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���X�V
 *
 * @param   emw		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void Model3D_Update(ENDING_MAIN_WORK *emw)
{
	VecFx32 scale_vec, trans;
	MtxFx33 rot;
	
	scale_vec.x = FX32_ONE;
	scale_vec.y = FX32_ONE;
	scale_vec.z = FX32_ONE;
	
	trans.x = 0;
	trans.y = 0;
	trans.z = 0;
	
	MTX_Identity33(&rot);

	//�R�c�`��J�n
	GF_G3X_Reset();
	
	GFC_AttachCamera(emw->camera);
	GFC_SetCameraView(FOOTPRINT_CAMERA_MODE, emw->camera); //�ˉe�ݒ�
	GFC_CameraLookAt();

	// ���C�g�ƃA���r�G���g
	switch(emw->scene_no){
	case ENDING_SCENE_MAP1:
		NNS_G3dGlbLightVector( 0, 1897, -3600, -466 );
		NNS_G3dGlbLightVector( 2, 0, 0, 4096 );
		NNS_G3dGlbLightColor( 0, GX_RGB( 28, 12, 6 ) );	//GX_RGB( 19,16,12 ) );
		NNS_G3dGlbLightColor( 2, GX_RGB( 0,0,0 ) );
		NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 15,15,15 ), GX_RGB( 11,12,12 ), FALSE );
		NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 17,17,17 ), GX_RGB( 8,8,7 ), FALSE );
		break;
	case ENDING_SCENE_MAP2:
		NNS_G3dGlbLightVector( 0, -2043, -3548, 110 );
		NNS_G3dGlbLightVector( 2, 0, 0, 4096 );
		NNS_G3dGlbLightColor( 0, GX_RGB( 22,22,20 ) );
		NNS_G3dGlbLightColor( 2, GX_RGB( 0,0,0 ) );
		NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 15,15,15 ), GX_RGB( 9,11,11 ), FALSE );
		NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 16,16,16 ), GX_RGB( 14,14,14 ), FALSE );
		break;
	case ENDING_SCENE_MAP3:
		NNS_G3dGlbLightVector( 0, 1897, -3600, -466 );
		NNS_G3dGlbLightVector( 2, 0, 0, 4096 );
		NNS_G3dGlbLightColor( 0, GX_RGB( 11,11,16 ) );
		NNS_G3dGlbLightColor( 2, GX_RGB( 0,0,0 ) );
		NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 14,14,16 ), GX_RGB( 10,10,10 ), FALSE );
		NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 14,14,16 ), GX_RGB( 8,8,11 ), FALSE );
		break;
	default:
		NNS_G3dGlbLightVector( 0, 0, -FX32_ONE, 0 );
		NNS_G3dGlbLightColor( 0, GX_RGB( 28,28,28 ) );
		NNS_G3dGlbMaterialColorDiffAmb( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
		NNS_G3dGlbMaterialColorSpecEmi( GX_RGB( 31,31,31 ), GX_RGB( 31,31,31 ), FALSE );
		break;
	}
	//�펞ON�̃��C�g
	NNS_G3dGlbLightVector( 3, 0, -FX32_ONE, 0 );
	NNS_G3dGlbLightColor( 3, GX_RGB( 28,28,28 ) );
	
	// �ʒu�ݒ�
	NNS_G3dGlbSetBaseTrans(&trans);
	// �p�x�ݒ�
	NNS_G3dGlbSetBaseRot(&rot);
	// �X�P�[���ݒ�
	NNS_G3dGlbSetBaseScale(&scale_vec);

//	NNS_G3dGlbFlush();
	
	// �`��
	NNS_G3dGePushMtx();
	{
		int line, i;
		
		for(line = 0; line < MODEL_LINE_MAX; line++){
			for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
				D3DOBJ_Draw( &emw->obj[line][i] );
			}
		}
	}
	NNS_G3dGePopMtx(1);
}

//--------------------------------------------------------------
/**
 * @brief   �J�����쐬
 *
 * @param   emw		�Q�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Ending_CameraInit(ENDING_MAIN_WORK *emw)
{
	VecFx32	target = { FOOTPRINT_CAMERA_TX, FOOTPRINT_CAMERA_TY, FOOTPRINT_CAMERA_TZ };
	VecFx32 cam_pos = {-31712, -142304, 496744};
	VecFx32 t_pos = {-31712, -67780, -5704};
	
	emw->camera = GFC_AllocCamera( HEAPID_ENDING_DEMO );

	GFC_InitCameraTDA(&target, FOOTPRINT_CAMERA_DISTANCE, &EndingCameraAngle,
		FOOTPRINT_CAMERA_PERSPWAY, FOOTPRINT_CAMERA_MODE, FALSE, emw->camera);

//	GFC_SetCameraAngleRot(&cam_rotate, emw->camera);	//����������̃J�����ɂ���
//	GFC_SetCameraAngleRev(&cam_rev_ang, emw->camera);
	GFC_SetCameraPos(&cam_pos, emw->camera);
	GFC_SetLookTarget(&t_pos, emw->camera);
	//GFC_SetCamUp(&cam_up, emw->camera);

	GFC_SetCameraClip( FOOTPRINT_CAMERA_NEAR, FOOTPRINT_CAMERA_FAR, emw->camera);
	
	GFC_AttachCamera(emw->camera);
}

//--------------------------------------------------------------
/**
 * @brief   �J�������
 *
 * @param   emw		�Q�[�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Ending_CameraExit(ENDING_MAIN_WORK *emw)
{
	GFC_FreeCamera(emw->camera);
}

//--------------------------------------------------------------
/**
 * @brief   ���Ճ{�[�h�p3DBG�������֐�
 * 
 * @param   �q�[�vID
 */
//--------------------------------------------------------------
static GF_G3DMAN * Ending_3D_Init(int heap_id)
{
	GF_G3DMAN *g3Dman;
	
	g3Dman = GF_G3DMAN_Init(heap_id, GF_G3DMAN_LNK, GF_G3DTEX_128K, 
		GF_G3DMAN_LNK, GF_G3DPLT_16K, EndingSimpleSetUp);
	return g3Dman;
}

static void EndingSimpleSetUp(void)
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
 * @brief   ���Ճ{�[�h�p3DBG�I������
 *
 * @param   g3Dman		
 */
//--------------------------------------------------------------
static void Ending_3D_Exit(GF_G3DMAN *g3Dman)
{
	GF_G3D_Exit(g3Dman);
}

//--------------------------------------------------------------
/**
 * @brief   �f�o�b�O�@�\�F�J�����ړ�
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Debug_CameraMove(ENDING_MAIN_WORK *emw)
{
#ifdef PM_DEBUG
	VecFx32 move = {0,0,0};
	fx32 value = FX32_ONE;
	int add_angle = 64;
	CAMERA_ANGLE angle = {0,0,0,0};
	int mode = 0;
	VecFx32 print_vec;
	enum{
		MODE_ROTATE, 		//���]
		MODE_SHIFT,			//���s�ړ�
		MODE_DISTANCE,		//����
		MODE_ANGLE_REV,		//���]
	};
	static CAMERA_ANGLE cam_up = {0, 0, 0};
	
	if((sys.cont & PAD_BUTTON_L) && (sys.cont & PAD_BUTTON_R)){
		mode = MODE_DISTANCE;
	}
	else if(sys.cont & PAD_BUTTON_L){
		mode = MODE_SHIFT;
	}
	else if(sys.cont & PAD_BUTTON_R){
		mode = MODE_ANGLE_REV;
	}
	else{
		//return;
		mode = MODE_ROTATE;
	}
	
	switch(mode){
	case MODE_ROTATE:
		if(sys.cont & PAD_KEY_LEFT){
			cam_up.y += 0x100;
		}
		else if(sys.cont & PAD_KEY_RIGHT){
			cam_up.y -= 0x100;
		}
		else if(sys.cont & PAD_KEY_UP){
			cam_up.x -= 0x100;
		}
		else if(sys.cont & PAD_KEY_DOWN){
			cam_up.x += 0x100;
		}
		else if(sys.cont & PAD_BUTTON_X){
			cam_up.z -= 0x100;
		}
		else if(sys.cont & PAD_BUTTON_B){
			cam_up.z += 0x100;
		}
		else{
			return;
		}
		GFC_SetCameraAngleRot(&cam_up,emw->camera);
	//	GFC_SetCamUp(&cam_up, emw->camera);	//����������̃J�����ɂ���
		OS_TPrintf("cam_up.x = %d, y = %d, z = %d\n", cam_up.x, cam_up.y, cam_up.z);
		print_vec = GFC_GetLookTarget(emw->camera);
		OS_TPrintf("target.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		print_vec = GFC_GetCameraPos(emw->camera);
		OS_TPrintf("pos.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		break;
	case MODE_SHIFT:
		if(sys.cont & PAD_KEY_LEFT){
			move.x -= value;
		}
		if(sys.cont & PAD_KEY_RIGHT){
			move.x += value;
		}
		if(sys.cont & PAD_KEY_UP){
			move.y += value;
		}
		if(sys.cont & PAD_KEY_DOWN){
			move.y -= value;
		}
		if(sys.cont & PAD_BUTTON_X){
			move.z -= value;
		}
		if(sys.cont & PAD_BUTTON_B){
			move.z += value;
		}
		GFC_ShiftCamera(&move, emw->camera);
		move = GFC_GetCameraPos(emw->camera);
		OS_TPrintf("�J�����ʒu x=%d(16�i:%x), y=%d(16�i:%x), z=%d(16�i:%x)\n", move.x, move.x, move.y, move.y, move.z, move.z);
		print_vec = GFC_GetLookTarget(emw->camera);
		OS_TPrintf("target.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		print_vec = GFC_GetCameraPos(emw->camera);
		OS_TPrintf("pos.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		break;
	
	case MODE_ANGLE_REV:
		if(sys.cont & PAD_KEY_LEFT){
			angle.y -= add_angle;
		}
		if(sys.cont & PAD_KEY_RIGHT){
			angle.y += add_angle;
		}
		if(sys.cont & PAD_KEY_UP){
			angle.x += add_angle;
		}
		if(sys.cont & PAD_KEY_DOWN){
			angle.x -= add_angle;
		}
		if(sys.cont & PAD_BUTTON_X){
			angle.z -= add_angle;
		}
		if(sys.cont & PAD_BUTTON_B){
			angle.z += add_angle;
		}
		GFC_AddCameraAngleRev(&angle, emw->camera);
		angle = GFC_GetCameraAngle(emw->camera);
		OS_TPrintf("�J�����A���O���@x=%d, y=%d, z=%d\n", angle.x, angle.y, angle.z);
		print_vec = GFC_GetLookTarget(emw->camera);
		OS_TPrintf("target.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		print_vec = GFC_GetCameraPos(emw->camera);
		OS_TPrintf("pos.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		break;
	case MODE_DISTANCE:
		if(sys.cont & PAD_KEY_UP){
			GFC_AddCameraDistance(FX32_ONE, emw->camera);
		}
		if(sys.cont & PAD_KEY_DOWN){
			GFC_AddCameraDistance(-FX32_ONE, emw->camera);
		}
		OS_TPrintf("�J����������%d(16�i:%x)\n", GFC_GetCameraDistance(emw->camera), GFC_GetCameraDistance(emw->camera));
		print_vec = GFC_GetLookTarget(emw->camera);
		OS_TPrintf("target.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		print_vec = GFC_GetCameraPos(emw->camera);
		OS_TPrintf("pos.x = %d, y = %d, z = %d\n", print_vec.x, print_vec.y, print_vec.z);
		break;
	}
#endif
}

