//==============================================================================
/**
 * @file	fss_test.c
 * @brief	FSS�n�V�X�e���e�X�g
 * @author	matsuda
 * @date	2007.04.04(��)
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

#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_matsu.h"

#include "fss_test.h"

#include "frontier_scr.h"
#include "fss_scene.h"



//==============================================================================
//	
//==============================================================================
FS_EXTERN_OVERLAY(frontier_common);

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
typedef struct{
//	CATS_SYS_PTR		csp;
//	CATS_RES_PTR		crp;
	GF_BGL_INI *bgl;
//	GF_BGL_BMPWIN win[CONRES_BMPWIN_MAX];

//	MSGDATA_MANAGER *conres_msg;	///<�_���X���僁�b�Z�[�W�p���b�Z�[�W�}�l�[�W���̃|�C���^
//	WORDSET *wordset;
//	STRBUF *msg_buf;				///<Alloc����������o�b�t�@�ւ̃|�C���^

	PALETTE_FADE_PTR pfd;			///<�p���b�g�t�F�[�h�V�X�e���ւ̃|�C���^
	
	FSS_PTR fss;
}TEST_SYSTEM_PARAM;

//--------------------------------------------------------------
/**
 * @brief   �_���X����Ǘ����[�N
 *
 * �_���X����S�̂��Ǘ�����匳�̃��[�N�ɂȂ�܂��B
 * �ʐM���ł��S�Ẵ}�V���Ōʂɍ쐬����܂�
 */
//--------------------------------------------------------------
typedef struct _TEST_PROC_WORK{
	TCB_PTR update_tcb;				///<�V�X�e���X�V����
	TCB_PTR vintr_tcb;				///<V�u�����N���荞�݃^�X�N
	GF_G3DMAN *g3Dman;
	
	u16 seq;
	u16 tbl_seq_no;					///<���C���V�[�P���X�e�[�u���̔ԍ�
	u8 proc_mode;					///<���݂̓����

	u8 main_end;					///<TRUE:���C�������I��
	
	TEST_SYSTEM_PARAM sys;
}TEST_PROC_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void FssTestVBlank(void *work);
static void FssTestHBlank(void *work);
static void VBlankTCB_IntrTask(TCB_PTR tcb, void *work);
static void FssTestUpdate(TCB_PTR tcb, void *work);
static void FssTest_VramBankSet(GF_BGL_INI *bgl);
PROC_RESULT FssTestProc_Init( PROC * proc, int * seq );
PROC_RESULT FssTestProc_Main( PROC * proc, int * seq );
PROC_RESULT FssTestProc_End( PROC * proc, int * seq );



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
PROC_RESULT FssTestProc_Init( PROC * proc, int * seq )
{
	TEST_PROC_WORK *tpw;

	Overlay_Load(FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);

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

	sys_CreateHeap(HEAPID_BASE_APP, HEAPID_CONRES, 0x70000);

	tpw = PROC_AllocWork(proc, sizeof(TEST_PROC_WORK), HEAPID_CONRES );
	MI_CpuClear8(tpw, sizeof(TEST_PROC_WORK));

//	tpw->g3Dman = ADV_Contest_3D_Init(HEAPID_CONRES);
	
	//�p���b�g�t�F�[�h�V�X�e���쐬
	tpw->sys.pfd = PaletteFadeInit(HEAPID_CONRES);
	PaletteTrans_AutoSet(tpw->sys.pfd, TRUE);
	PaletteFadeWorkAllocSet(tpw->sys.pfd, FADE_MAIN_BG, 0x200, HEAPID_CONRES);
	PaletteFadeWorkAllocSet(tpw->sys.pfd, FADE_SUB_BG, 0x200, HEAPID_CONRES);
	PaletteFadeWorkAllocSet(tpw->sys.pfd, FADE_MAIN_OBJ, 0x200 - 0x40, HEAPID_CONRES);
	PaletteFadeWorkAllocSet(tpw->sys.pfd, FADE_SUB_OBJ, 0x200, HEAPID_CONRES);
	
	tpw->sys.bgl = GF_BGL_BglIniAlloc(HEAPID_CONRES);

	initVramTransferManagerHeap(64, HEAPID_CONRES);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	FssTest_VramBankSet(tpw->sys.bgl);

	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);

	WIPE_SYS_Start(WIPE_PATTERN_FMAS, WIPE_TYPE_SCREWIN, WIPE_TYPE_SCREWIN, WIPE_FADE_BLACK, 
		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_CONRES);

	tpw->update_tcb = TCB_Add(FssTestUpdate, tpw, 80000);

	GF_Disp_DispOn();
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);
	GF_Disp_GXS_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	//�T�E���h�f�[�^���[�h(�R���e�X�g)
	Snd_DataSetByScene( SND_SCENE_CONTEST, SEQ_CON_TEST, 1 );

	MsgPrintAutoFlagSet(MSG_AUTO_ON);
	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);
	
	//���X�N���v�g�ݒ�
	tpw->sys.fss = FSS_SystemCreate(HEAPID_CONRES, FSS_SCENEID_TESTSCR);
	FSS_ScrTaskAdd(tpw->sys.fss, FSS_SCENEID_TESTSCR, 2);

	sys_VBlankFuncChange(FssTestVBlank, tpw);
	//��Wipe��H�u�����N���g�p����̂ŁAWipe���I��������Z�b�g����悤�ɕύX
//	sys_HBlankIntrSet(FssTestHBlank, tpw);
	tpw->vintr_tcb = VIntrTCB_Add(VBlankTCB_IntrTask, tpw, 10);
	
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
PROC_RESULT FssTestProc_Main( PROC * proc, int * seq )
{
	TEST_PROC_WORK * tpw  = PROC_GetWork( proc );
	int ret;
	enum{
		SEQ_IN,
		SEQ_MAIN,
		SEQ_OUT,
	};
	
	switch( *seq ){
	case SEQ_IN:
//		if( IsFinishedBrightnessChg(MASK_DOUBLE_DISPLAY) ){
		if(WIPE_SYS_EndCheck() == TRUE){
			sys_HBlankIntrSet(FssTestHBlank, tpw);
			*seq = SEQ_MAIN;
		}
		break;

	case SEQ_MAIN:
		//���X�N���v�g���s
		if(FSS_ScriptMain(tpw->sys.fss) == TRUE){
			if(sys.trg & PAD_BUTTON_B){
				WIPE_SYS_Start(WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_CONRES);
				*seq = SEQ_OUT;
			}
		}
		break;

	case SEQ_OUT:
//		if(IsFinishedBrightnessChg(MASK_DOUBLE_DISPLAY)){
		if(WIPE_SYS_EndCheck() == TRUE){
			return PROC_RES_FINISH;
		}
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
PROC_RESULT FssTestProc_End( PROC * proc, int * seq )
{
	TEST_PROC_WORK * tpw = PROC_GetWork( proc );
	int i;

	//BMP�J��
//	for(i = 0; i < CONRES_BMPWIN_MAX; i++){
//		GF_BGL_BmpWinDel(&tpw->sys.win[i]);
//	}

	//���C�����BG�폜
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
	GF_BGL_BGControlExit(tpw->sys.bgl, FSSTEST_FRAME_WIN );
	GF_BGL_BGControlExit(tpw->sys.bgl, FSSTEST_FRAME_EFF );
	GF_BGL_BGControlExit(tpw->sys.bgl, FSSTEST_FRAME_BACKGROUND );
	//�T�u���BG�폜
	GF_BGL_VisibleSet(FSSTEST_FRAME_SUB_AUDIENCE, VISIBLE_OFF);
	GF_BGL_BGControlExit(tpw->sys.bgl, FSSTEST_FRAME_SUB_AUDIENCE);

	//Vram�]���}�l�[�W���[�폜
	DellVramTransferManager();

	//�p���b�g�t�F�[�h�V�X�e���폜
	PaletteFadeWorkAllocFree(tpw->sys.pfd, FADE_MAIN_BG);
	PaletteFadeWorkAllocFree(tpw->sys.pfd, FADE_SUB_BG);
	PaletteFadeWorkAllocFree(tpw->sys.pfd, FADE_MAIN_OBJ);
	PaletteFadeWorkAllocFree(tpw->sys.pfd, FADE_SUB_OBJ);
	PaletteFadeFree(tpw->sys.pfd);

	//BGL�J��
	sys_FreeMemoryEz(tpw->sys.bgl);

	//���X�N���v�g���
	FSS_SystemFree(tpw->sys.fss);

	TCB_Delete(tpw->update_tcb);
	TCB_Delete(tpw->vintr_tcb);
	
//	ADV_Contest_3D_Exit(tpw->g3Dman);

	StopTP();		//�^�b�`�p�l���̏I��

	PROC_FreeWork(proc);				// ���[�N�J��
	
	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);
	
	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	sys_DeleteHeap(HEAPID_CONRES);

	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);
	
	MI_CpuFill16((void*)HW_BG_PLTT, 0x7fff, 0x200);
	MI_CpuFill16((void*)HW_OBJ_PLTT, 0x7fff, 0x200);
	MI_CpuFill16((void*)HW_DB_BG_PLTT, 0x7fff, 0x200);
	MI_CpuFill16((void*)HW_DB_OBJ_PLTT, 0x7fff, 0x200);

//	WirelessIconEasyEnd();

	Overlay_UnloadID(FS_OVERLAY_ID(frontier_common));
	
	return PROC_RES_FINISH;
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
static void FssTestVBlank(void *work)
{
	TEST_PROC_WORK *tpw = work;

	DoVramTransferManager();	// Vram�]���}�l�[�W���[���s
	PaletteFadeTrans(tpw->sys.pfd);
	
	GF_BGL_VBlankFunc(tpw->sys.bgl);
	
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
static void FssTestHBlank(void *work)
{
	TEST_PROC_WORK *tpw = work;

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
	TEST_PROC_WORK *tpw = work;
	
}

//--------------------------------------------------------------
/**
 * @brief   ���C�����[�v�̍Ō�ɍs���V�X�e���֘A�̍X�V����
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		tpw
 */
//--------------------------------------------------------------
static void FssTestUpdate(TCB_PTR tcb, void *work)
{
	TEST_PROC_WORK *tpw = work;
	
//	G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void FssTest_VramBankSet(GF_BGL_INI *bgl)
{
	GF_Disp_GX_VisibleControlInit();

	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_C,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
			GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
			GX_VRAM_TEX_01_AB,				// �e�N�X�`���C���[�W�X���b�g
			GX_VRAM_TEXPLTT_01_FG			// �e�N�X�`���p���b�g�X���b�g
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
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME1_M	�E�B���h�E
			{
				0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
//				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
				FSSTEST_BGPRI_WIN, 0, 0, FALSE
			},
			///<FRAME2_M	�G�t�F�N�g
			{
				0, 0, 0x2000, 0, GF_BGL_SCRSIZ_512x512, GX_BG_COLORMODE_16,
//				GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
				GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				FSSTEST_BGPRI_EFF, 0, 0, FALSE
			},
			///<FRAME3_M	�w�i
			{
				0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
				FSSTEST_BGPRI_BACKGROUND, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FSSTEST_FRAME_WIN, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FSSTEST_FRAME_WIN );
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_WIN, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_WIN, GF_BGL_SCROLL_Y_SET, 0);
		GF_BGL_BGControlSet(bgl, FSSTEST_FRAME_EFF, &TextBgCntDat[1], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FSSTEST_FRAME_EFF );
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_EFF, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_EFF, GF_BGL_SCROLL_Y_SET, 0);
		GF_BGL_BGControlSet(bgl, FSSTEST_FRAME_BACKGROUND, &TextBgCntDat[2], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FSSTEST_FRAME_BACKGROUND );
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_BACKGROUND, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_BACKGROUND, GF_BGL_SCROLL_Y_SET, 0);

		G2_SetBG0Priority(FSSTEST_3DBG_PRIORITY);
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER SubBgCntDat[] = {
			///<FRAME0_S	�ϋq
			{
				0, 0, 0x0800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				FSSTEST_BGPRI_SUB_AUDIENCE, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, FSSTEST_FRAME_SUB_AUDIENCE, &SubBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, FSSTEST_FRAME_SUB_AUDIENCE );
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_SUB_AUDIENCE, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, FSSTEST_FRAME_SUB_AUDIENCE, GF_BGL_SCROLL_Y_SET, 0);
	}
}

