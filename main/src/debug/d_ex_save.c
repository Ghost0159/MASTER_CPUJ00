//==============================================================================
/**
 * @file	d_ex_save.c
 * @brief	�O���Z�[�u�j��
 * @author	matsuda
 * @date	2008.06.27(��)
 */
//==============================================================================
#ifdef	PM_DEBUG

#include "common.h"
#include "system/fontproc.h"
#include "system/bmp_list.h"
#include "system/lib_pack.h"
#include "system/arc_util.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "demo/title.h"
#include "system/gamestart.h"
#include "field/game_init.h"

#include "system/main.h"

#include "msgdata/msg.naix"

#include "wifi/dwc_rap.h"
#include "wifi/dwc_test.h"

#include "common.h"
#include "system/pm_overlay.h"
#include "system/test_overlay.h"

#include "system/bmp_list.h"
#include "field/fieldmap.h"
#include "field/fieldsys.h"
#include "system/fontproc.h"
#include "contest/contest.h"

#include "field/script.h"
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
//#include "field\ev_mapchange.h"

#include "include/application/imageClip/imc_sys.h"

#include "wifi\dwc_rap.h"
#include "application/br_sys.h"

#include "savedata/savedata_local.h"
#define	FIRST_MIRROR_START	(0)
#define	SECOND_MIRROR_START	(64)
#define	SECTOR_SIZE		(0x1000)


//============================================================================================
//	overlay��`
//============================================================================================

FS_EXTERN_OVERLAY( title );


//============================================================================================
//	�萔��`
//============================================================================================
#define FCOLS_BLACK	(GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL))
#define FCOLS_GREEN	(GF_PRINTCOLOR_MAKE(FBMP_COL_GREEN,FBMP_COL_GRN_SDW,FBMP_COL_WHITE))

#define	FONT_PAL_NO		( 0 )


#define	HEAPID_DEBUG_TITLE	HEAPID_DEBUG_APPLICATION

//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct {
	SAVEDATA *savedata;
	GF_BGL_INI * bgl;
	GF_BGL_BMPWIN	win;
	BMPLIST_WORK * lw;
	BMP_MENULIST_DATA *bmd;
	int select_id;
	int system_free;
	int main_seq;
	
	int local_seq;
	TCB_PTR local_tcb;
	
	u8 sub_seq;
}D_EXSAVE_WORK;


//==============================================================================
//	�^�錾
//==============================================================================
typedef void (*pDMFunc)(D_EXSAVE_WORK *);

//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================

static void VramBankSet(void);
static void BgInit( GF_BGL_INI * ini );
static void BmpListCB1( u32 param, u8 mode );
static void BmpListCB2( u8 index, u32 param, u8 y );
static void D_TitleMenuMemFree( GF_BGL_INI * ini );
static void DebugMenuSystemFree(D_EXSAVE_WORK *wk);

static void DM_VideoBreakCurrent_Mine(D_EXSAVE_WORK *wk);
static void DM_VideoBreakMirror_Mine(D_EXSAVE_WORK *wk);
static void DM_VideoBreakCurrent_0(D_EXSAVE_WORK *wk);
static void DM_VideoBreakMirror_0(D_EXSAVE_WORK *wk);
static void DM_VideoBreakCurrent_1(D_EXSAVE_WORK *wk);
static void DM_VideoBreakMirror_1(D_EXSAVE_WORK *wk);
static void DM_VideoBreakCurrent_2(D_EXSAVE_WORK *wk);
static void DM_VideoBreakMirror_2(D_EXSAVE_WORK *wk);
static void DM_StageBreakCurrent(D_EXSAVE_WORK *wk);
static void DM_StageBreakMirror(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeCurrent_Mine(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeMirror_Mine(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeCurrent_0(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeMirror_0(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeCurrent_1(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeMirror_1(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeCurrent_2(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeMirror_2(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeCurrent_Stage(D_EXSAVE_WORK *wk);
static void DM_VideoKeyChangeMirror_Stage(D_EXSAVE_WORK *wk);
static void DM_NormalSaveBreak(D_EXSAVE_WORK *wk);

//============================================================================================
//	�O���[�o���ϐ�
//============================================================================================

//gmm�t�@�C����ID�𕶎���w��Ɏg���\���̐錾
typedef struct
{
	u32	strID;
	u32	param;
}LIST_DATA;

static const LIST_DATA ListData[] =
{
	{DMMSG_EXSAVE_VIDEO_BREAK_MINE, (u32)DM_VideoBreakCurrent_Mine},
	{DMMSG_EXSAVE_VIDEO_BREAK_MINE_B, (u32)DM_VideoBreakMirror_Mine},
	{DMMSG_EXSAVE_VIDEO_BREAK_0001, (u32)DM_VideoBreakCurrent_0},
	{DMMSG_EXSAVE_VIDEO_BREAK_0001_B, (u32)DM_VideoBreakMirror_0},
	{DMMSG_EXSAVE_VIDEO_BREAK_0002, (u32)DM_VideoBreakCurrent_1},
	{DMMSG_EXSAVE_VIDEO_BREAK_0002_B, (u32)DM_VideoBreakMirror_1},
	{DMMSG_EXSAVE_VIDEO_BREAK_0003, (u32)DM_VideoBreakCurrent_2},
	{DMMSG_EXSAVE_VIDEO_BREAK_0003_B, (u32)DM_VideoBreakMirror_2},
	{DMMSG_EXSAVE_STAGE_BREAK_0001, (u32)DM_StageBreakCurrent},
	{DMMSG_EXSAVE_STAGE_BREAK_0001_B, (u32)DM_StageBreakMirror},
	{DMMSG_EXSAVE_VIDEO_BREAK_MINE_B0002, (u32)DM_VideoKeyChangeCurrent_Mine},
	{DMMSG_EXSAVE_VIDEO_BREAK_MINE_B0003, (u32)DM_VideoKeyChangeMirror_Mine},
	{DMMSG_EXSAVE_VIDEO_BREAK_0001_B0002, (u32)DM_VideoKeyChangeCurrent_0},
	{DMMSG_EXSAVE_VIDEO_BREAK_0001_B0003, (u32)DM_VideoKeyChangeMirror_0},
	{DMMSG_EXSAVE_VIDEO_BREAK_0002_B0002, (u32)DM_VideoKeyChangeCurrent_1},
	{DMMSG_EXSAVE_VIDEO_BREAK_0002_B0003, (u32)DM_VideoKeyChangeMirror_1},
	{DMMSG_EXSAVE_VIDEO_BREAK_0003_B0002, (u32)DM_VideoKeyChangeCurrent_2},
	{DMMSG_EXSAVE_VIDEO_BREAK_0003_B0003, (u32)DM_VideoKeyChangeMirror_2},
	{DMMSG_EXSAVE_STAGE_BREAK_0001_B0002, (u32)DM_VideoKeyChangeCurrent_Stage},
	{DMMSG_EXSAVE_STAGE_BREAK_0001_B0003, (u32)DM_VideoKeyChangeMirror_Stage},
	{DMMSG_NORMAL_SAVE_BREAK, (u32)DM_NormalSaveBreak},
};

static const BMPLIST_HEADER ListHeader = {
	NULL,
	NULL,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
	NULL,					// BMP�E�B���h�E�f�[�^
	NELEMS(ListData),		// ���X�g���ڐ�
	10,						// �\���ő區�ڐ�
	0,						// ���x���\���w���W
	12,						// ���ڕ\���w���W
	0,						// �J�[�\���\���w���W
	0,						// �\���x���W
	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						// �����Ԋu�w
	16,						// �����Ԋu�x
	BMPLIST_LRKEY_SKIP,		// �y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			// �����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};


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



//--------------------------------------------------------------------------------------------
/**
 * VRAM������
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VramBankSet(void)
{
	GF_BGL_DISPVRAM vramSetTable = {
		GX_VRAM_BG_128_C,				// ���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
		GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
		GX_VRAM_OBJ_16_F,				// ���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
		GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
		GX_VRAM_TEX_01_AB,				// �e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_0123_E			// �e�N�X�`���p���b�g�X���b�g
	};
	GF_Disp_SetBank( &vramSetTable );
}

//--------------------------------------------------------------------------------------------
/**
 * BG������
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgInit( GF_BGL_INI * ini )
{
	{	// BG SYSTEM
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	{	// MAIN DISP BG0
		GF_BGL_BGCNT_HEADER MBg0_Data = {
			0, 0, 0x800, 0,
			GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x00000,
			GX_BG_EXTPLTT_01, 0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, GF_BGL_FRAME0_M, &MBg0_Data, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, GF_BGL_FRAME0_M );
	}

	{	// MAIN DISP BG1
		GF_BGL_BGCNT_HEADER MBg1_Data = {
			0, 0, 0x800, 0,
			GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x00000,
			GX_BG_EXTPLTT_01, 1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, GF_BGL_FRAME1_M, &MBg1_Data, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, GF_BGL_FRAME1_M );
	}

	{	// MAIN DISP BG2
		GF_BGL_BGCNT_HEADER MBg2_Data = {
			0, 0, 0x800, 0,
			GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000,
			GX_BG_EXTPLTT_23, 2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, GF_BGL_FRAME2_M, &MBg2_Data, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, GF_BGL_FRAME2_M );
	}

	{	// MAIN DISP BG3
		GF_BGL_BGCNT_HEADER MBg3_Data = {
			0, 0, 0x800, 0,
			GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000,
			GX_BG_EXTPLTT_23, 3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, GF_BGL_FRAME3_M, &MBg3_Data, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, GF_BGL_FRAME3_M );
	}
}


//--------------------------------------------------------------------------------------------
/**
 * �������J��
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void D_TitleMenuMemFree( GF_BGL_INI * ini )
{
	// BGL�J��
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_M );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME1_M );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME2_M );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME3_M );
/*
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_S );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME1_S );
*/
}


//--------------------------------------------------------------------------------------------
/**
 * @brief	�l���I���v���Z�X�FBMPMENULISTDATA�̐���
 * @param	bmd		BMP_MENULIST_DATA�\���̂ւ̃|�C���^
 */
//--------------------------------------------------------------------------------------------
static	void	BMP_MENULIST_Make(BMP_MENULIST_DATA *bmd)
{
	int				i;
	MSGDATA_MANAGER	*man;

	man=MSGMAN_Create(MSGMAN_TYPE_NORMAL,ARC_MSG,NARC_msg_debug_matsu_dat,HEAPID_DEBUG_TITLE);

	for(i=0;i<NELEMS(ListData);i++){
		BMP_MENULIST_AddArchiveString(bmd,man,ListData[i].strID,ListData[i].param);
	}

	MSGMAN_Delete(man);
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	�l���I���v���Z�X�F������
 * @param	proc	�v���Z�X�ւ̃|�C���^
 * @param	seq		�V�[�P���X�p���[�N�ւ̃|�C���^
 * @return	PROC_RES_CONTINUE	����p����
 * @return	PROC_RES_FINISH		����I��
 */
//--------------------------------------------------------------------------------------------
static PROC_RESULT DebugExSaveMenuProc_Init(PROC * proc, int * seq)
{
	sys_CreateHeap(HEAPID_BASE_APP,HEAPID_WORLD,0x1c000);
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_DEBUG_TITLE, 0x10000);
	{
		D_EXSAVE_WORK * wk;
		//int id = Debug_GetMyID();

		wk = PROC_AllocWork(proc, sizeof(D_EXSAVE_WORK), HEAPID_DEBUG_TITLE);
		MI_CpuClear8(wk, sizeof(D_EXSAVE_WORK));
		wk->savedata = ((MAINWORK*)PROC_GetParentWork(proc))->savedata;

		wk->bgl = GF_BGL_BglIniAlloc( HEAPID_DEBUG_TITLE );

		GF_Disp_GX_VisibleControlInit();
		VramBankSet();
		BgInit( wk->bgl );

		MSG_PrintInit();

		SystemFontPaletteLoad( PALTYPE_MAIN_BG, FONT_PAL_NO * 32, HEAPID_DEBUG_TITLE );

		{
			BMPLIST_HEADER	list_h;

			wk->bmd=BMP_MENULIST_Create(NELEMS(ListData),HEAPID_DEBUG_TITLE);
			BMP_MENULIST_Make(wk->bmd);

			GF_BGL_BmpWinAdd(
				wk->bgl, &wk->win, GF_BGL_FRAME0_M, 1, 1, 24, 22, FONT_PAL_NO, 1 );

			list_h = ListHeader;
			list_h.list = wk->bmd;
			list_h.win = &wk->win;
			wk->lw = BmpListSet( &list_h, 0, 0, HEAPID_DEBUG_TITLE );
		}
	}

		WIPE_ResetBrightness( WIPE_DISP_MAIN );
		WIPE_ResetBrightness( WIPE_DISP_SUB );

		GF_Disp_DispOn();

	Snd_BgmChannelSetAndReverbSet( 0 );	//�g�p�\�`�����l������A���o�[�u�ݒ�(�N���A)

	return PROC_RES_FINISH;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	�l���I���v���Z�X�F���C��
 * @param	proc	�v���Z�X�ւ̃|�C���^
 * @param	seq		�V�[�P���X�p���[�N�ւ̃|�C���^
 * @return	PROC_RES_CONTINUE	����p����
 * @return	PROC_RES_FINISH		����I��
 */
//--------------------------------------------------------------------------------------------
static PROC_RESULT DebugExSaveMenuProc_Main(PROC * proc, int * seq)
{
	D_EXSAVE_WORK * wk = PROC_GetWork(proc);
	u32	ret;

	switch(wk->main_seq){
	case 0:
		ret = BmpListMain( wk->lw );

		switch( ret ){
		case BMPLIST_NULL:
			break;
		case BMPLIST_CANCEL:
			wk->main_seq = 0xff;
			break;
		default:
			{
				pDMFunc func = (pDMFunc)ret;
				if(func != NULL){
					func(wk);
				}
				else{
					wk->main_seq = 0xff;
					break;
				}
				wk->main_seq++;
			}
			break;
		}
		break;
	case 1:
		OS_TPrintf("���ǂ�\n");
		wk->main_seq--;
		break;
	default:
		wk->select_id = -1;
		OS_ResetSystem(0);
		return PROC_RES_FINISH;
	}
	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	�l���I���v���Z�X�F�I��
 * @param	proc	�v���Z�X�ւ̃|�C���^
 * @param	seq		�V�[�P���X�p���[�N�ւ̃|�C���^
 * @return	PROC_RES_CONTINUE	����p����
 * @return	PROC_RES_FINISH		����I��
 */
//--------------------------------------------------------------------------------------------
static PROC_RESULT DebugExSaveMenuProc_End(PROC * proc, int * seq)
{
	D_EXSAVE_WORK * wk = PROC_GetWork(proc);

	DebugMenuSystemFree(wk);

	((MAINWORK*)PROC_GetParentWork(proc))->select_id = wk->select_id;

	//�^�C�g���ɖ߂�
	Main_SetNextProc(FS_OVERLAY_ID(title), &TitleProcData);

	PROC_FreeWork(proc);

	sys_DeleteHeap(HEAPID_DEBUG_TITLE);
	sys_DeleteHeap(HEAPID_WORLD);
	return PROC_RES_FINISH;
}

static void DebugMenuSystemFree(D_EXSAVE_WORK *wk)
{
	if(wk->system_free == TRUE){
		return;
	}
	wk->system_free = TRUE;
	
	BMP_MENULIST_Delete(wk->bmd);
	BmpListExit( wk->lw, NULL, NULL );
	GF_BGL_BmpWinOff( &wk->win );
	GF_BGL_BmpWinDel( &wk->win );
	D_TitleMenuMemFree( wk->bgl );
	sys_FreeMemoryEz( wk->bgl );
}

//==============================================================================
//==============================================================================
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��
 *
 * @param   wk		
 * @param   id
 * @param   current_mirror			0:�J�����g�A1:�~���[
 */
//--------------------------------------------------------------
static void DM_ExSaveBreak(EXDATA_ID id, int current_mirror)
{
	const EXSAVEDATA_TABLE * extbl;
	EX_CERTIFY_SAVE_KEY key, old_key, new_key;
	u8 flag;
	u32 write_data = 0x4a81;
	
	sys_SleepNG(SLEEPTYPE_SAVELOAD);
	extbl = &ExtraSaveDataTable[id];
	GF_ASSERT(extbl->id == id);

	MISC_ExtraSaveKeyGet(SaveData_GetMisc(SaveData_GetPointer()), id, &key, &old_key, &flag);
	flag ^= 1;
	if(current_mirror){
		flag ^= 1;
	}
	
	if(flag == 1){
		PMSVLD_Save((FIRST_MIRROR_START + extbl->sector) * SECTOR_SIZE + 0x60, &write_data, sizeof(write_data));
	} else {
		PMSVLD_Save((SECOND_MIRROR_START + extbl->sector) * SECTOR_SIZE + 0x60, &write_data, sizeof(write_data));
	}
	
	sys_SleepOK(SLEEPTYPE_SAVELOAD);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��
 *
 * @param   wk		
 * @param   id
 * @param   current_mirror			0:�J�����g�A1:�~���[
 */
//--------------------------------------------------------------
static void DM_ExKeyChange(EXDATA_ID id, int current_mirror)
{
	EX_CERTIFY_SAVE_KEY key, old_key;
	u8 flag;
	
	MISC_ExtraSaveKeyGet(SaveData_GetMisc(SaveData_GetPointer()), id, &key, &old_key, &flag);
	if(current_mirror){
		old_key *= 503;
	}
	else{
		key *= 431;
	}
	MISC_ExtraSaveKeySet(SaveData_GetMisc(SaveData_GetPointer()), id, key, old_key, flag);
	
	SaveData_Save(SaveData_GetPointer());
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�J�����g
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakCurrent_Mine(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_MINE, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�~���[
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakMirror_Mine(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_MINE, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�J�����g
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeCurrent_Mine(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_MINE, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�~���[
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeMirror_Mine(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_MINE, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�J�����g
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakCurrent_0(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_DL_0, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�~���[
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakMirror_0(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_DL_0, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�J�����g
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeCurrent_0(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_DL_0, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�~���[
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeMirror_0(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_DL_0, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�J�����g
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakCurrent_1(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_DL_1, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�~���[
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakMirror_1(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_DL_1, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�J�����g
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeCurrent_1(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_DL_1, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�~���[
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeMirror_1(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_DL_1, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�J�����g
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakCurrent_2(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_DL_2, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�~���[
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoBreakMirror_2(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_BATTLE_REC_DL_2, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�J�����g
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeCurrent_2(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_DL_2, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�~���[
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeMirror_2(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_BATTLE_REC_DL_2, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�J�����g
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_StageBreakCurrent(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_FRONTIER, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�j��F�~���[
 *
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_StageBreakMirror(D_EXSAVE_WORK *wk)
{
	DM_ExSaveBreak(EXDATA_ID_FRONTIER, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�J�����g
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeCurrent_Stage(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_FRONTIER, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �O���Z�[�u�L�[�ύX�F�~���[
 * @param   wk		
 */
//--------------------------------------------------------------
static void DM_VideoKeyChangeMirror_Stage(D_EXSAVE_WORK *wk)
{
	DM_ExKeyChange(EXDATA_ID_FRONTIER, 1);
}

//--------------------------------------------------------------
/**
 * @brief   �ʏ�Z�[�u�j��
 * @param   wk		
 */
//--------------------------------------------------------------
extern int DEBUG_SaveCurrentSideGet(void);

static void DM_NormalSaveBreak(D_EXSAVE_WORK *wk)
{
	u32 write_data = 0x10f93ac1;
	
	if(DEBUG_SaveCurrentSideGet() == 0){
		PMSVLD_Save((FIRST_MIRROR_START + 2) * SECTOR_SIZE + 0x60, &write_data, sizeof(write_data));
	}
	else{
		PMSVLD_Save((SECOND_MIRROR_START + 2) * SECTOR_SIZE + 0x60, &write_data, sizeof(write_data));
	}
}


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
const PROC_DATA DebugExSaveMenuProcData = {
	DebugExSaveMenuProc_Init,
	DebugExSaveMenuProc_Main,
	DebugExSaveMenuProc_End,
	NO_OVERLAY_ID
};

#endif	//PM_DEBUG

