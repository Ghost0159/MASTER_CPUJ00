//==============================================================================
/**
 * @file	d_matsu.c
 * @brief	���c�f�o�b�O�p�t�@�C��(��풓��)
 * @author	matsuda
 * @date	2005.06.20(��)
 */
//==============================================================================

#ifdef PM_DEBUG	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


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
#include "field/fld_bmp.h"
#include "application/app_tool.h"

#include "savedata/misc.h"

#include "application/balloon.h"
#include "field\ev_mapchange.h"

#include "include/application/imageClip/imc_sys.h"

#include "wifi\dwc_rap.h"
#include "application/br_sys.h"



//==============================================================================
//	�I�[�o�[���CID��extern�錾
//==============================================================================
/* overlay id in *.lcf */
FS_EXTERN_OVERLAY(test_first);
FS_EXTERN_OVERLAY(test_second);
FS_EXTERN_OVERLAY(test_itcm);
FS_EXTERN_OVERLAY(test_second_abc);

//==============================================================================
//	�萔��`
//==============================================================================
enum{
	DEBUG_GDS_START = 100,
	DEBUG_GDS_BV = DEBUG_GDS_START,		//BR_MODE_GDS_BV		
	DEBUG_GDS_BV_RANK,                  //BR_MODE_GDS_BV_RANK	
	DEBUG_GDS_RANK,                     //BR_MODE_GDS_RANK	
	DEBUG_GDS_DRESS,                    //BR_MODE_GDS_DRESS	
	DEBUG_GDS_BOX,                      //BR_MODE_GDS_BOX		
};

//==============================================================================
//	�\���̐錾
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���c�f�o�b�O�p�\����(�O���[�o���ϐ�)
 */
//--------------------------------------------------------------
typedef struct{
	u8 dance_rotation_off;		///<1:�_���X�̃��[�e�[�V�����Ȃ�
}DMATSU_GLOBAL_WORK;

///�R���e�X�g�̃����N�A�^�C�v�I����ʗp���[�N
typedef struct{
	GF_BGL_BMPWIN win;
	FIELDSYS_WORK *fsys;

	MSGDATA_MANAGER* msgman;
	STRBUF *rank_str;
	STRBUF *type_str;
	
	TCB_PTR tcb;
	s8 cursor_y;
	s8 rank;
	s8 type;
	s8 debug_mode;
}DEBUG_CONMODE_WORK;

#define FCOLS_BLACK	(GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL))
#define FCOLS_GREEN	(GF_PRINTCOLOR_MAKE(FBMP_COL_GREEN,FBMP_COL_GRN_SDW,FBMP_COL_WHITE))

//==============================================================================
//	�O���[�o���ϐ��錾
//==============================================================================
///1:���[�e�[�V�����Ȃ��B�@0:���[�e�[�V��������@2:�_���X�񐔐����Ȃ�
extern u8 DmatsuWork_DanceRotationOff;

//==============================================================================
//	�^�錾
//==============================================================================
typedef void (*pDMFunc)(DMATSU_WORK *);


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void DMRes_BalloonStart(FIELDSYS_WORK *fsys, int proc_flag, int mode);


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void DM_MenuMain(TCB_PTR tcb, void *work);
static void DM_MenuExit(DMATSU_WORK *dm);
static void DM_FssTest(DMATSU_WORK *dm);
static void DM_ExtraSave(DMATSU_WORK *dm);
static void DM_ExtraLoad(DMATSU_WORK *dm);
static void DM_ActingStart(DMATSU_WORK *dm);
static void DM_VisualStart(DMATSU_WORK *dm);
static void DM_ClipVisualStart(DMATSU_WORK *dm);
static void DM_DanceStart(DMATSU_WORK *dm);
static void DM_DanceNoneLimitStart(DMATSU_WORK *dm);
static void DM_DanceRotatinOn(DMATSU_WORK *dm);
static void DM_DanceRotationOff(DMATSU_WORK *dm);
static void DM_ContestSioStart(DMATSU_WORK *dm);
static void DM_ResultStart(DMATSU_WORK *dm);
static void TestOverlaySet(DMATSU_WORK *dm);
static void TestSet(DMATSU_WORK *dm);
static void FieldParticleTest(DMATSU_WORK *dm);
static void DM_Capture(DMATSU_WORK *dm);
static void CaptureTask(TCB_PTR tcb, void *work);
static void DM_ExtraSaveFlagReset(DMATSU_WORK *dm);
static void DM_SysFlagON_Style(DMATSU_WORK *dm);
static void DM_SysFlagOFF_Style(DMATSU_WORK *dm);
static void DM_SysFlagON_Beautiful(DMATSU_WORK *dm);
static void DM_SysFlagOFF_Beautiful(DMATSU_WORK *dm);
static void DM_SysFlagON_Cute(DMATSU_WORK *dm);
static void DM_SysFlagOFF_Cute(DMATSU_WORK *dm);
static void DM_SysFlagON_Clever(DMATSU_WORK *dm);
static void DM_SysFlagOFF_Clever(DMATSU_WORK *dm);
static void DM_SysFlagON_Strong(DMATSU_WORK *dm);
static void DM_SysFlagOFF_Strong(DMATSU_WORK *dm);
void DebugMatsudaInit(FIELDSYS_WORK *fsys);
static void DM_ConModeSelectInit(DMATSU_WORK *dm, int debug_mode);
static void DM_ConModeSelectEnd(DEBUG_CONMODE_WORK *dcw);
static void DM_ConModeSelectMain(TCB_PTR tcb, void *work);
static void DM_ConModeSelectMsgWrite(DEBUG_CONMODE_WORK *dcw);
static void DM_BalloonStart(DMATSU_WORK *dm);
static void DM_GDSRecorderStart(DMATSU_WORK *dm);
static void DM_GDSLibStart(DMATSU_WORK *dm);
static void DM_FootprintStart(DMATSU_WORK *dm);
static void DebugContest_FieldConnectStart(FIELDSYS_WORK *fsys,int debug_mode, int rank, int type);

#if DEBUG_MATSU_CONTEST
extern BOOL GMEVENT_DebugConStart(GMEVENT_CONTROL * event);
#endif

//==============================================================================
//	�f�o�b�O���j���[���X�g�p�f�[�^��
//==============================================================================
///�f�o�b�O���j���[�̍���
static const struct {
	u32  str_id;
	u32  param;
}DebugMenuParamList[] = {
	{ DMMSG_CAPTURE, (u32)DM_Capture },
	{ DMMSG_EXTRA_CLEAR, (u32)DM_ExtraSaveFlagReset },
	{ DMMSG_BALLOON, (u32)DM_BalloonStart },
	{ DMMSG_GDS_RECORDER, (u32)DM_GDSRecorderStart },
	{ DMMSG_GDSLIB, (u32)DM_GDSLibStart },
	{ DMMSG_FOOTPRINT, (u32)DM_FootprintStart },
	{ DMMSG_FSS_TEST, (u32)DM_FssTest },
	{ DMMSG_EXTRA_SAVE, (u32)DM_ExtraSave },
	{ DMMSG_EXTRA_LOAD, (u32)DM_ExtraLoad },
	{ DMMSG_ACTIN, (u32)DM_ActingStart },
	{ DMMSG_VISUAL, (u32)DM_VisualStart},
	{ DMMSG_CLIP_VISUAL, (u32)DM_ClipVisualStart},
	{ DMMSG_DANCE, (u32)DM_DanceStart},
	{ DMMSG_DANCE_NONE_LIMIT, (u32)DM_DanceNoneLimitStart},
	{ DMMSG_ROTE_ON, (u32)DM_DanceRotatinOn},
	{ DMMSG_ROTE_OFF, (u32)DM_DanceRotationOff},
	{ DMMSG_TUUSHIN, (u32)DM_ContestSioStart},
	{ DMMSG_RESULT, (u32)DM_ResultStart},
	{ DMMSG_PARTICLE, (u32)FieldParticleTest },
	{ DMMSG_OVERLAY, (u32)TestOverlaySet },
	{ DMMSG_SYSFLAG_ON_STYLE, (u32)DM_SysFlagON_Style },
	{ DMMSG_SYSFLAG_OFF_STYLE, (u32)DM_SysFlagOFF_Style },
	{ DMMSG_SYSFLAG_ON_BEAUTIFUL, (u32)DM_SysFlagON_Beautiful },
	{ DMMSG_SYSFLAG_OFF_BEAUTIFUL, (u32)DM_SysFlagOFF_Beautiful },
	{ DMMSG_SYSFLAG_ON_CUTE, (u32)DM_SysFlagON_Cute },
	{ DMMSG_SYSFLAG_OFF_CUTE, (u32)DM_SysFlagOFF_Cute },
	{ DMMSG_SYSFLAG_ON_CLEVER, (u32)DM_SysFlagON_Clever },
	{ DMMSG_SYSFLAG_OFF_CLEVER, (u32)DM_SysFlagOFF_Clever },
	{ DMMSG_SYSFLAG_ON_STRONG, (u32)DM_SysFlagON_Strong },
	{ DMMSG_SYSFLAG_OFF_STRONG, (u32)DM_SysFlagOFF_Strong },
};


#define LIST_MENU_MAX		(NELEMS(DebugMenuParamList))

///�f�o�b�O���j���[�̃��X�g
static const BMPLIST_HEADER DebugMenuListHeader = {
	NULL,			// �\�������f�[�^�|�C���^
	NULL,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
	NULL,
	LIST_MENU_MAX,	// ���X�g���ڐ�
	11,//LIST_MENU_MAX,						// �\���ő區�ڐ�
	0,						// ���x���\���w���W
	8,						// ���ڕ\���w���W
	0,						// �J�[�\���\���w���W
	0,						// �\���x���W
	FBMP_COL_WHITE,			// �����F
	FBMP_COL_BLACK,			// �w�i�F
	FBMP_COL_BLK_SDW,		// �����e�F
	0,						// �����Ԋu�w
	16,						// �����Ԋu�x
	BMPLIST_LRKEY_SKIP,		// �y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,				// �����w��
	0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};




//--------------------------------------------------------------
/**
 * @brief   �I�[�o�[���C�e�X�g
 *
 * @param   tcb		
 * @param   work		
 */
//--------------------------------------------------------------
static void TestOverlayStart(TCB_PTR tcb, void *work)
{
	DMATSU_WORK *dm = work;
	
	if(dm->wait == 0){
		dm->wait++;
		/* address of function is specified in main.lsf */
	#ifdef DEBUG_ONLY_FOR_matsuda
		OS_Printf("func_1() : addr = 0x%08X.\n", test_overlay_func_1);
		OS_Printf("func_2() : addr = 0x%08X.\n", test_overlay_func_2);
		OS_Printf("func_3() : addr = 0x%08X.\n", test_overlay_itcm);
	#endif
		return;
	}
	
	//OS_PrintServer();

	//�e�\���L�[�ɂ���ăI�[�o�[���C�ǂݏo��
	if(sys.trg & PAD_KEY_RIGHT){
		//�܂�������Ԃ��g�p���Ă���I�[�o�[���C��S�ăA�����[�h
		Overlay_UnloadSameArea(FS_OVERLAY_ID(test_first));
		//�������g���I�[�o�[���C���s
		Overlay_Load(FS_OVERLAY_ID(test_first), OVERLAY_LOAD_NOT_SYNCHRONIZE);
	}
	else if(sys.trg & PAD_KEY_DOWN){
		Overlay_UnloadSameArea(FS_OVERLAY_ID(test_second));
		Overlay_Load(FS_OVERLAY_ID(test_second), OVERLAY_LOAD_SYNCHRONIZE_2);
		Overlay_Load(FS_OVERLAY_ID(test_second_abc), OVERLAY_LOAD_SYNCHRONIZE_2);
	}
	else if(sys.trg & PAD_KEY_LEFT){
		Overlay_UnloadSameArea(FS_OVERLAY_ID(test_itcm));
		Overlay_Load(FS_OVERLAY_ID(test_itcm), OVERLAY_LOAD_SYNCHRONIZE);
	}
	else if(sys.trg & PAD_KEY_UP){
		Overlay_UnloadSameArea(FS_OVERLAY_ID(test_second_abc));
		Overlay_Load(FS_OVERLAY_ID(test_second_abc), OVERLAY_LOAD_SYNCHRONIZE_2);
	}

	//�e�{�^���ɂ���ăI�[�o�[���C����Ă���t�@�C�����̊֐��Ăяo��
	if (sys.trg & PAD_BUTTON_A){
		test_overlay_func_1();
	}
	else if (sys.trg & PAD_BUTTON_B){
		test_overlay_func_2();
	}
	else if(sys.trg & PAD_BUTTON_Y){
		test_overlay_itcm();
	}
	else if(sys.trg & PAD_BUTTON_X){
		test_overlay_func_3();
	}
	
	if(sys.trg & PAD_BUTTON_START){
		sys_FreeMemory(HEAPID_BASE_DEBUG, dm);
		TCB_Delete(tcb);
		FieldSystemProc_SeqHoldEnd();
		return;
	}
}


//--------------------------------------------------------------
/**
 * @brief   ���c�f�o�b�O�����A�Ăяo����
 */
//--------------------------------------------------------------
void DebugMatsudaInit(FIELDSYS_WORK *fsys)
{
	DMATSU_WORK *dm;
	BMPLIST_HEADER list_h;
	
	dm = sys_AllocMemory(HEAPID_BASE_DEBUG, sizeof(DMATSU_WORK));
	memset(dm, 0, sizeof(DMATSU_WORK));
	
	//BMP�E�B���h�E����
	GF_BGL_BmpWinAdd(fsys->bgl, &dm->bmpwin, 
		FLD_MBGFRM_FONT, 1, 1, 25, 22, 13, 1);
	dm->fsys = fsys;
	
	dm->listmenu = BMP_MENULIST_Create(LIST_MENU_MAX, HEAPID_BASE_DEBUG);

	//�����񃊃X�g�쐬
	{
		MSGDATA_MANAGER *man;
		STRBUF * str_buf;
		int i;
		
		man = MSGMAN_Create(MSGMAN_TYPE_NORMAL, ARC_MSG, 
			NARC_msg_debug_matsu_dat, HEAPID_BASE_DEBUG);
		
		for(i = 0; i < LIST_MENU_MAX; i++){
			BMP_MENULIST_AddArchiveString(dm->listmenu, man, DebugMenuParamList[i].str_id, 
				DebugMenuParamList[i].param);
		}
		
		MSGMAN_Delete(man);
	}
	
	list_h = DebugMenuListHeader;
	list_h.win = &dm->bmpwin;
    list_h.list = dm->listmenu;
	dm->lw = BmpListSet(&list_h, 0, 0, HEAPID_BASE_DEBUG);
	
	GF_BGL_BmpWinOn(&dm->bmpwin);
	
	TCB_Add(DM_MenuMain, dm, 10);
}

//--------------------------------------------------------------
/**
 * @brief   ���c�p�f�o�b�O���j���[���X�g�I��
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		�f�o�b�O�p���c���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void DM_MenuMain(TCB_PTR tcb, void *work)
{
	DMATSU_WORK *dm = work;
	u32	ret;

	ret = BmpListMain(dm->lw);
	switch(dm->main_seq){
	case 0:
		dm->main_seq++;
		break;
	case 1:
		switch(ret){
		case BMPLIST_NULL:
			break;
		case BMPLIST_CANCEL:
			DM_MenuExit(dm);
			TCB_Delete(tcb);
			sys_FreeMemoryEz(dm);
			FieldSystemProc_SeqHoldEnd();
			return;
		default:
			{
				pDMFunc func = (pDMFunc)ret;

				DM_MenuExit(dm);
				TCB_Delete(tcb);
				func(dm);
				sys_FreeMemoryEz(dm);
				return;
			}
			break;
		}
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���c�p�f�o�b�O���j���[���X�g�����
 * @param   dm		�f�o�b�O�p���c���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void DM_MenuExit(DMATSU_WORK *dm)
{
	BMP_MENULIST_Delete(dm->listmenu);
	BmpListExit(dm->lw, NULL, NULL);

	GF_BGL_BmpWinOff(&dm->bmpwin);
	GF_BGL_BmpWinDel(&dm->bmpwin);
}

static void TestOverlaySet(DMATSU_WORK *dm)
{
	TCB_Add(TestOverlayStart, dm, 20);
}

extern void BattleSystemProc(void);
static void TestSet(DMATSU_WORK *dm)
{
#if 0
	sys.mainSys_sequence = 5;//SEQ_EXIT;
	sys.pReturnProc = BattleSystemProc;
	//FieldSysWork.general_seqno = 0;
#else
//	if(sys.trg & START_BUTTON){
		sys_FreeMemory(HEAPID_BASE_DEBUG, dm);
//		TCB_Delete(tcb);
		FieldSystemProc_SeqHoldEnd();
		return;
//	}
#endif
}

#include "savedata\battle_rec.h"

static void DM_ExtraSave(DMATSU_WORK *dm)
{
	u32 *buff;
	int i;
	LOAD_RESULT result;
	
//	buff = sys_AllocMemory(HEAPID_WORLD, 0x8000);
	(void*)buff = SaveData_Extra_LoadBattleRecData(dm->fsys->savedata, HEAPID_WORLD, &result,
		LOADDATA_MYREC);
	GF_ASSERT(result != LOAD_RESULT_NG);
	for(i = 0; i < 100; i++){
		buff[i] = i;
	}
	for(i = 0x7800/4; i < 0x7800/4+100; i++){
		buff[i] = i;
	}
	SaveData_Extra_SaveBattleRecData(dm->fsys->savedata, (void*)buff, LOADDATA_MYREC);
	sys_FreeMemoryEz(buff);
#ifdef DEBUG_ONLY_FOR_matsuda
	OS_TPrintf("�Z�[�u����\n");
#endif


	FieldSystemProc_SeqHoldEnd();
}

static void DM_ExtraLoad(DMATSU_WORK *dm)
{
	u32 *buff;
	int i;
	LOAD_RESULT result;
	
	(void*)buff = SaveData_Extra_LoadBattleRecData(dm->fsys->savedata, HEAPID_WORLD, &result,
		LOADDATA_MYREC);
	GF_ASSERT(result != LOAD_RESULT_NG);
	for(i = 0; i < 50; i++){
	#ifdef DEBUG_ONLY_FOR_matsuda
		OS_TPrintf("load buff %d = %d\n", i, buff[i]);
	#endif
	}
	for(i = 0x7800/4; i < 0x7800/4+50; i++){
	#ifdef DEBUG_ONLY_FOR_matsuda
		OS_TPrintf("load buff %d = %d\n", i, buff[i]);
	#endif
	}
	sys_FreeMemoryEz(buff);
	


	FieldSystemProc_SeqHoldEnd();
}

//--------------------------------------------------------------
/**
 * @brief   ���D����~�j�Q�[���Ăяo��
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_BalloonStart(DMATSU_WORK *dm)
{
	dm->proc_flag = DMATSU_PROC_FLAG_BALLOON;
	DMRes_BalloonStart(dm->fsys, dm->proc_flag, dm->proc_debug_mode);
}

//--------------------------------------------------------------
/**
 * @brief   GDS(�o�g�����R�[�_�[)�Ăяo��
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_GDSRecorderStart(DMATSU_WORK *dm)
{
	if(mydwc_checkMyGSID(dm->fsys->savedata) == FALSE){
		//������GSID�������Ă��Ȃ��Ƃ��̃f�o�b�O�͎g���Ȃ�
		GF_ASSERT(0);
	}
	DM_ConModeSelectInit(dm, DEBUG_GDS_START);
}

//--------------------------------------------------------------
/**
 * @brief   GDS���C�u�����e�X�g�Ăяo��
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_GDSLibStart(DMATSU_WORK *dm)
{
	if(mydwc_checkMyGSID(dm->fsys->savedata) == FALSE){
		//������GSID�������Ă��Ȃ��Ƃ��̃f�o�b�O�͎g���Ȃ�
		GF_ASSERT(0);
	}
	dm->proc_flag = DMATSU_PROC_FLAG_GDS;
	dm->proc_debug_mode = 0xff;
	DMRes_BalloonStart(dm->fsys, dm->proc_flag, dm->proc_debug_mode);
}

//--------------------------------------------------------------
/**
 * @brief   ���Ճ{�[�h�Ăяo��
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_FootprintStart(DMATSU_WORK *dm)
{
	dm->proc_flag = DMATSU_PROC_FLAG_FOOTPRINT;
	DMRes_BalloonStart(dm->fsys, dm->proc_flag, dm->proc_debug_mode);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�̃����N�A�^�C�v�I����ʏ�����
 *
 * @param   dm		
 * @param   debug_mode		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_ConModeSelectInit(DMATSU_WORK *dm, int debug_mode)
{
	DEBUG_CONMODE_WORK *dcw;
	MSGDATA_MANAGER* pMan;
	STRBUF*			tmp;
	
	dcw = sys_AllocMemory(HEAPID_BASE_DEBUG, sizeof(DEBUG_CONMODE_WORK));
	MI_CpuClear8(dcw, sizeof(DEBUG_CONMODE_WORK));
	dcw->fsys = dm->fsys;
	dcw->debug_mode = debug_mode;
	
	//BMP�E�B���h�E��ǉ�
	GF_BGL_BmpWinAdd(dcw->fsys->bgl, &dcw->win, FLD_MBGFRM_FONT, 2, 2, 12, 4,
			FLD_SYSFONT_PAL, 1);
	GF_BGL_BmpWinDataFill(&dcw->win,APP_WINCLR_COL(FBMP_COL_WHITE));
	GF_BGL_BmpWinOn(&dcw->win);

	//�����Z�b�g�ǉ�
	dcw->msgman = MSGMAN_Create(MSGMAN_TYPE_NORMAL, ARC_MSG,
			NARC_msg_debug_matsu_dat, HEAPID_BASE_DEBUG);
	dcw->rank_str = STRBUF_Create(30, HEAPID_BASE_DEBUG);
	dcw->type_str = STRBUF_Create(30, HEAPID_BASE_DEBUG);
	
	DM_ConModeSelectMsgWrite(dcw);
	
	dcw->tcb = TCB_Add(DM_ConModeSelectMain, dcw, 100);
}

///�R���e�X�g�̃����N�A�^�C�v�I���I������
static void DM_ConModeSelectEnd(DEBUG_CONMODE_WORK *dcw)
{
	GF_BGL_BmpWinOff(&dcw->win);
	GF_BGL_BmpWinDel(&dcw->win);

	STRBUF_Delete(dcw->rank_str);
	STRBUF_Delete(dcw->type_str);
	MSGMAN_Delete(dcw->msgman);
	
	TCB_Delete(dcw->tcb);
	sys_FreeMemoryEz(dcw);
}

///�R���e�X�g�̃����N�A�^�C�v�I�����C��
static void DM_ConModeSelectMain(TCB_PTR tcb, void *work)
{
	DEBUG_CONMODE_WORK *dcw = work;
	int add_num;
	int jyoudan_max, gedan_max;
	
	if(dcw->debug_mode < DEBUG_GDS_START){
		jyoudan_max = CONRANK_MAX;
	}
	else{
		jyoudan_max = BR_MODE_GDS_BOX - BR_MODE_GDS_BV + 1;
	}
	gedan_max = CONTYPE_MAX;
	
	if((dcw->debug_mode < DEBUG_GDS_START) && (sys.trg & (PAD_KEY_DOWN | PAD_KEY_UP))){
		dcw->cursor_y ^= 1;
		DM_ConModeSelectMsgWrite(dcw);
	}
	
	if(sys.trg & (PAD_KEY_LEFT|PAD_KEY_RIGHT)){
		add_num = (sys.trg & PAD_KEY_LEFT) ? -1 : 1;
		if(dcw->cursor_y == 0){
			dcw->rank += add_num;
			if(dcw->rank<0){
				dcw->rank = jyoudan_max-1;
			}
			else if(dcw->rank >= jyoudan_max){
				dcw->rank = 0;
			}
		}
		else{
			dcw->type += add_num;
			if(dcw->type<0){
				dcw->type = gedan_max-1;
			}
			else if(dcw->type >= gedan_max){
				dcw->type = 0;
			}
		}
		DM_ConModeSelectMsgWrite(dcw);
	}
	
	if(sys.trg & (PAD_BUTTON_A|PAD_BUTTON_B|PAD_BUTTON_START)){
		FIELDSYS_WORK *fsys;
		int rank,type,debug_mode;
		
		fsys = dcw->fsys;
		rank = dcw->rank;
		type = dcw->type;
		debug_mode = dcw->debug_mode;
		
		DM_ConModeSelectEnd(dcw);
		
		if(debug_mode >= DEBUG_GDS_START){	//GDS
			DMRes_BalloonStart(dcw->fsys, 
				DMATSU_PROC_FLAG_GDS_RECORDER, dcw->rank + BR_MODE_GDS_BV);
		}
		else{	//�R���e�X�g
			DebugContest_FieldConnectStart(fsys, debug_mode, rank, type);
		}
		return;
	}
}

///�����N�A���[�h�̃��b�Z�[�W�ĕ`��
static void DM_ConModeSelectMsgWrite(DEBUG_CONMODE_WORK *dcw)
{
	GF_PRINTCOLOR rank_col, type_col;
	
	if(dcw->cursor_y == 0){
		rank_col = FCOLS_GREEN;
		type_col = FCOLS_BLACK;
	}
	else{
		rank_col = FCOLS_BLACK;
		type_col = FCOLS_GREEN;
	}

	if(dcw->debug_mode < DEBUG_GDS_START){
		MSGMAN_GetString(dcw->msgman, DMMSG_RANK0001 + dcw->rank, dcw->rank_str);
		MSGMAN_GetString(dcw->msgman, DMMSG_TYPE0001 + dcw->type, dcw->type_str);
		
		GF_STR_PrintColor(&dcw->win, FONT_SYSTEM,
			dcw->rank_str, 0,0, MSG_NO_PUT, rank_col, NULL);
		GF_STR_PrintColor(&dcw->win, FONT_SYSTEM,
			dcw->type_str, 0,16, MSG_NO_PUT, type_col, NULL);
	}
	else{
		MSGMAN_GetString(dcw->msgman, DMMSG_GDS_MODE_0000 + dcw->rank, dcw->rank_str);
		GF_STR_PrintColor(&dcw->win, FONT_SYSTEM,
			dcw->rank_str, 0,0, MSG_NO_PUT, rank_col, NULL);
	}

	GF_BGL_BmpWinOn(&dcw->win);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F���Z�͕���J�n
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_ActingStart(DMATSU_WORK *dm)
{
//	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_ACTIN, 
//		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
	DM_ConModeSelectInit(dm, DEBUG_CON_ACTIN);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�r�W���A������J�n
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_VisualStart(DMATSU_WORK *dm)
{
//	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_VISUAL, 
//		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
	DM_ConModeSelectInit(dm, DEBUG_CON_VISUAL);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�C���[�W�N���b�v���r�W���A������J�n
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_ClipVisualStart(DMATSU_WORK *dm)
{
//	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_CLIP_VISUAL, 
//		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
	DM_ConModeSelectInit(dm, DEBUG_CON_CLIP_VISUAL);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�_���X����J�n
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_DanceStart(DMATSU_WORK *dm)
{
	DmatsuWork_DanceRotationOff = 0;
#if 0
	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_DANCE, 
		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
#endif

	DM_ConModeSelectInit(dm, DEBUG_CON_DANCE);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�_���X����J�n(�S�Ă̎l�������Ń_���X)
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_DanceNoneLimitStart(DMATSU_WORK *dm)
{
	DmatsuWork_DanceRotationOff = 2;
	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_DANCE, 
		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�_���X����J�n(���[�e�[�V��������)
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_DanceRotatinOn(DMATSU_WORK *dm)
{
	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_DANCE, 
		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�_���X����J�n(���[�e�[�V�����Ȃ�)
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_DanceRotationOff(DMATSU_WORK *dm)
{
	DmatsuWork_DanceRotationOff = 1;
	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_DANCE, 
		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F�_���X����J�n
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_ContestSioStart(DMATSU_WORK *dm)
{
	DmatsuWork_DanceRotationOff = 0;
	EventSet_Script(dm->fsys, SCRID_CONNECT_CONTEST, NULL );
}

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�F���ʔ��\�J�n
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void DM_ResultStart(DMATSU_WORK *dm)
{
	DebugContest_FieldConnectStart(dm->fsys, DEBUG_CON_RESULT, 
		CONRANK_NORMAL, gf_rand() % CONTYPE_MAX);
}

//--------------------------------------------------------------
/**
 * @brief   �f�o�b�O�R���e�X�g�Ăяo��
 *
 * @param   fsys		
 * @param   debug_mode		DEBUG_CON_???
 */
//--------------------------------------------------------------
static void DebugContest_FieldConnectStart(FIELDSYS_WORK *fsys, int debug_mode, int rank, int type)
{
#if DEBUG_MATSU_CONTEST
	EV_ACTING_WORK *eaw;
	
	eaw = sys_AllocMemoryLo(HEAPID_WORLD, sizeof(EV_ACTING_WORK));
	MI_CpuClear8(eaw, sizeof(EV_ACTING_WORK));
	eaw->debug_mode = debug_mode;
	eaw->rank = rank;
	eaw->type = type;
	FieldEvent_Set(fsys, GMEVENT_DebugConStart, eaw);
#endif
}




//==============================================================================
//	
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   
 * @param   dm		
 */
//--------------------------------------------------------------
static void DM_Capture(DMATSU_WORK *dm)
{
	FIELD_OBJ_PTR fldobj;		//�Ώۂ̃t�B�[���hOBJ�̃|�C���^
	int *task_work;
	
	//���@���L���v�`���[�ɓ���Ȃ��悤�ɔ�\���ɂ���
	fldobj	= FieldOBJSys_OBJIDSearch(dm->fsys->fldobjsys, FLDOBJ_ID_PLAYER);
	GF_ASSERT(fldobj != NULL);
	FieldOBJ_StatusBitSet_Vanish(fldobj, TRUE);

	task_work = sys_AllocMemory(HEAPID_BASE_DEBUG, sizeof(int));
	*task_work = 0;
	TCB_Add(CaptureTask, task_work, 10);


	FieldSystemProc_SeqHoldEnd();
}

static void CaptureTask(TCB_PTR tcb, void *work)
{
	int *task_work =work;
	
	switch(*task_work){
	case 0:		//���@�̔�\�����K�����f�����悤��1��҂�
		break;
	case 1:
		MI_CpuClearFast( (void*)HW_LCDC_VRAM_C, HW_VRAM_C_SIZE );
		GX_SetCapture(
				GX_CAPTURE_SIZE_256x192,			// �L���v�`���T�C�Y
				GX_CAPTURE_MODE_A,			// �L���v�`�����[�h
				GX_CAPTURE_SRCA_3D,				// �L���v�`���u�����hA
				GX_CAPTURE_SRCB_VRAM_0x00000,				// �L���v�`���u�����hB
				GX_CAPTURE_DEST_VRAM_C_0x00000,			// �]��Vram
				0,						// �u�����h�W��A
				0);						// �u�����h�W��B

		//LCDC�Ƀ������}�b�s���O
		GX_SetBankForLCDC(GX_VRAM_LCDC_C);
	//	GX_SetGraphicsMode(GX_DISPMODE_VRAM_C, GX_BGMODE_0, GX_BG0_AS_3D);
	
		sys_FreeMemoryEz(task_work);
		TCB_Delete(tcb);
		return;
	}
	
	(*task_work)++;
}

//--------------------------------------------------------------
/**
 * @brief   
 * @param   dm		
 */
//--------------------------------------------------------------
static void DM_ExtraSaveFlagReset(DMATSU_WORK *dm)
{
	MISC *misc;
	misc = SaveData_GetMisc(dm->fsys->savedata);
	MISC_ClearExtraInitFlag(misc);

	FieldSystemProc_SeqHoldEnd();
}

//--------------------------------------------------------------
/**
 * @brief   
 * @param   dm		
 */
//--------------------------------------------------------------
static void DM_SysFlagON_Style(DMATSU_WORK *dm)
{
	SysFlag_ConMasterSet(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_STYLE);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagOFF_Style(DMATSU_WORK *dm)
{
	SysFlag_ConMasterReset(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_STYLE);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagON_Beautiful(DMATSU_WORK *dm)
{
	SysFlag_ConMasterSet(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_BEAUTIFUL);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagOFF_Beautiful(DMATSU_WORK *dm)
{
	SysFlag_ConMasterReset(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_BEAUTIFUL);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagON_Cute(DMATSU_WORK *dm)
{
	SysFlag_ConMasterSet(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_CUTE);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagOFF_Cute(DMATSU_WORK *dm)
{
	SysFlag_ConMasterReset(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_CUTE);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagON_Clever(DMATSU_WORK *dm)
{
	SysFlag_ConMasterSet(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_CLEVER);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagOFF_Clever(DMATSU_WORK *dm)
{
	SysFlag_ConMasterReset(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_CLEVER);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagON_Strong(DMATSU_WORK *dm)
{
	SysFlag_ConMasterSet(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_STRONG);
	FieldSystemProc_SeqHoldEnd();
}

static void DM_SysFlagOFF_Strong(DMATSU_WORK *dm)
{
	SysFlag_ConMasterReset(SaveData_GetEventWork(dm->fsys->savedata), CONTYPE_STRONG);
	FieldSystemProc_SeqHoldEnd();
}

//==============================================================================
//
//	
//
//==============================================================================

extern void DMRes_FssTest(DMATSU_WORK *dm);

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
static void DM_FssTest(DMATSU_WORK *dm)
{
	DMRes_FssTest(dm);
}



//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���e�X�g
 *
 * @param   dm		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void FieldParticleTest(DMATSU_WORK *dm);
static void TestTCB_Particle(TCB_PTR tcb, void *work);
static int TestParticle_Main(void);
static u32 sAllocTex(u32 size, BOOL is4x4comp);
static u32 sAllocTexPalette(u32 size, BOOL is4pltt);

typedef struct{
	PTC_PTR ptc;
	u8 seq;
	
}TEST_PARTICLE_WORK;

///�퓬�p�[�e�B�N���̃J�����j�A�ݒ�
#define BP_NEAR			(FX32_ONE)
///�퓬�p�[�e�B�N���̃J�����t�@�[�ݒ�
#define BP_FAR			(FX32_ONE * 900)

static void FieldParticleTest(DMATSU_WORK *dm)
{
	sys_FreeMemory(HEAPID_BASE_DEBUG, dm);
	FieldSystemProc_SeqHoldEnd();
	
	{
		TEST_PARTICLE_WORK *tpw;
		
		tpw = sys_AllocMemory(HEAPID_BASE_DEBUG, sizeof(TEST_PARTICLE_WORK));
		MI_CpuClear8(tpw, sizeof(TEST_PARTICLE_WORK));
		TCB_Add(TestTCB_Particle, tpw, 20);
	}
}

static void TestTCB_Particle(TCB_PTR tcb, void *work)
{
	TEST_PARTICLE_WORK *tpw = work;
	int heap_id = HEAPID_BASE_DEBUG;
	void *heap;
	GF_CAMERA_PTR camera_ptr;
	void *resource;
	
	switch(tpw->seq){
	case 0:		//������
		Particle_SystemWorkInit();	//�p�[�e�B�N���V�X�e��������
		tpw->seq++;
		break;
	case 1:		//�p�[�e�B�N���V�X�e���쐬
		heap = sys_AllocMemory(heap_id, PARTICLE_LIB_HEAP_SIZE);
		tpw->ptc = Particle_SystemCreate(sAllocTex, sAllocTexPalette, heap, 
			PARTICLE_LIB_HEAP_SIZE, TRUE, heap_id);
		camera_ptr = Particle_GetCameraPtr(tpw->ptc);
		GFC_SetCameraClip(BP_NEAR, BP_FAR, camera_ptr);

		tpw->seq++;
		break;
	case 2:		//���\�[�X�ǂݍ��݁��o�^
		resource = Particle_ArcResourceLoad(
			ARC_PARTICLE_DATA, NARC_particledata_w_001_spa, heap_id);
		Particle_ResourceSet(tpw->ptc, resource, PTC_AUTOTEX_LNK | PTC_AUTOPLTT_LNK, TRUE);
		
		tpw->seq++;
		break;
	case 3:		//�p�[�e�B�N������
		Particle_CreateEmitterCallback(tpw->ptc, 0, NULL, NULL);
		break;
	case 4:		//�p�[�e�B�N�����s
		if(TestParticle_Main() == FALSE){
			tpw->seq++;
		}
		break;
	case 5:		//�p�[�e�B�N���V�X�e���j��
		heap = Particle_HeapPtrGet(tpw->ptc);
		Particle_SystemExit(tpw->ptc);
		sys_FreeMemoryEz(heap);
		
		tpw->seq++;
		break;
	default:
		sys_FreeMemoryEz(tpw);
		TCB_Delete(tcb);
		return;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�p�p�[�e�B�N���V�X�e���E���C���֐�(�v�Z�E�`�揈���Ȃǂ����s)
 *
 * @param   ptc		�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static int TestParticle_Main(void)
{
	int draw_num;
	const MtxFx43 *camera_ptr;
	
	GF_G3X_Reset();


	if(Particle_GetActionNum() == 0){
		return FALSE;
	}
	

	draw_num = Particle_DrawAll();	//�p�[�e�B�N���`��

	if(draw_num > 0){
		//�p�[�e�B�N���̕`�悪�I�������̂ŁA�Ăу\�t�g�E�F�A�X�v���C�g�p�J�����ɐݒ�
		GF_G3X_Reset();
	}

	Particle_CalcAll();	//�p�[�e�B�N���v�Z

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`��VRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param   size		�e�N�X�`���T�C�Y
 * @param   is4x4comp	4x4���k�e�N�X�`���ł��邩�ǂ����̃t���O(TRUE=���k�e�N�X�`��)
 *
 * @retval  �ǂݍ��݂��J�n����VRAM�̃A�h���X
 */
//--------------------------------------------------------------
static u32 sAllocTex(u32 size, BOOL is4x4comp)
{
	u32 address;
	NNSGfdTexKey key;
	
	key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
	GF_ASSERT(key != NNS_GFD_ALLOC_ERROR_TEXKEY);
	Particle_LnkTexKeySet(key);		//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	
	address = NNS_GfdGetTexKeyAddr(key);
#ifdef DEBUG_ONLY_FOR_matsuda
	OS_TPrintf("vram=%d\n", address);
#endif
	return address;
}

//--------------------------------------------------------------
/**
 * @brief	�e�N�X�`���p���b�gVRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param	size		�e�N�X�`���T�C�Y
 * @param	is4pltt		4�F�p���b�g�ł��邩�ǂ����̃t���O
 *
 * @retval	�ǂݍ��݂��J�n����VRAM�̃A�h���X
 *
 * direct�`���̃e�N�X�`���̏ꍇ�ASPL_LoadTexPlttByCallbackFunction��
 * �R�[���o�b�N�֐����Ăяo���܂���B
 */
//--------------------------------------------------------------
static u32 sAllocTexPalette(u32 size, BOOL is4pltt)
{
	NNSGfdPlttKey key;
	u32 address;
	
	key = NNS_GfdAllocPlttVram(size, is4pltt, NNS_GFD_ALLOC_FROM_LOW);
	if(key == NNS_GFD_ALLOC_ERROR_PLTTKEY){
		GF_ASSERT(0 && "�p�[�e�B�N���Ńp���b�g�̊m�ۂ��o���܂���I\n");
	}
	Particle_PlttLnkTexKeySet(key);	//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	address = NNS_GfdGetPlttKeyAddr(key);
#ifdef DEBUG_ONLY_FOR_matsuda
	OS_TPrintf("vram=%d\n", address);
#endif
	return address;
}

#endif	// PM_DEBUG +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


