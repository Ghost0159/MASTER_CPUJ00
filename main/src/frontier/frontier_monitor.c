//==============================================================================================
/**
 * @file	frontier_monitor.c
 * @brief	�u�o�g���t�����e�B�A�v���j�^�[
 * @author	Satoshi Nohara
 * @date	2007.12.6
 *
 * type��0=SINGLE�ƍl���Ă��܂�
 */
//==============================================================================================
#include "common.h"
#include "system/main.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/brightness.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "system/bmp_menu.h"
#include "system/window.h"
#include "system/fontproc.h"

#include "savedata/config.h"
#include "savedata/frontier_savedata.h"
#include "savedata/frontier_ex.h"

#include "savedata/factory_savedata.h"
#include "savedata/stage_savedata.h"
#include "savedata/castle_savedata.h"
#include "savedata/roulette_savedata.h"
#include "savedata/b_tower.h"

#include "msgdata\msg.naix"					//NARC_msg_??_dat
#include "msgdata\msg_bf_seiseki.h"			//msg_??

#include "../field/fieldsys.h"				//scr_tool.h�ɕK�v
#include "../field/scr_tool.h"				//FactoryScr_GetWinRecordID
#include "../field/poketch/poketch_arc.h"	//�|�P�b�`�ɂ���{�[���̊G���g���܂킵

#include "application/frontier_monitor.h"
#include "frontier_monitor_bmp.h"
#include "frontier_def.h"
#include "graphic/frontier_obj_def.h"
#include "graphic/frontier_bg_def.h"

#include "factory_def.h"
#include "stage_def.h"
#include "castle_def.h"
#include "roulette_def.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�V�[�P���X��`
enum {
	SEQ_GAME_INIT,											//������
	SEQ_GAME_START,											//�Q�[���J�n
	SEQ_GAME_END,											//�I��
};

//�\���ʒu
enum{
	BF_STR_X	= (1*8),									//�o�g����(�{�ݖ�)

	TITLE_STR_SINGLE_X_TOWER	= (17*8),					//�V���O����������
	TITLE_STR_SINGLE_X_FACTORY	= (17*8),					//�V���O����������
	TITLE_STR_SINGLE_X_STAGE	= (17*8),					//�V���O����������
	TITLE_STR_SINGLE_X_CASTLE	= (17*8),					//�V���O����������
	TITLE_STR_SINGLE_X_ROULETTE	= (17*8),					//�V���O����������

	TITLE_STR_X_TOWER	= (18*8),							//�_�u���A�}���`��������
	TITLE_STR_X_FACTORY = (18*8),							//�_�u���A�}���`��������
	TITLE_STR_X_STAGE	= (18*8),							//�_�u���A�}���`��������
	TITLE_STR_X_CASTLE	= (18*8),							//�_�u���A�}���`��������
	TITLE_STR_X_ROULETTE= (18*8),							//�_�u���A�}���`��������

	WIN_NUM_X	= (8*8),									//�A�����������ʒu
	CP_STR_X	= (22*8),
	TRD_NUM_X	= (21*8),
	//TRD_STR_X	= (27*8),
};


//==============================================================================================
//
//	���b�Z�[�W�֘A�̒�`
//
//==============================================================================================
#define FRONTIER_MSG_BUF_SIZE		(800)//(1024)			//���b�Z�[�W�T�C�Y


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
typedef struct{
	PROC* proc;											//PROC�ւ̃|�C���^

	u8	sub_seq;										//�V�[�P���X
	u8	type;											//�V���O���A�_�u���A�}���`�Awifi�}���`
	u8	fr_no;											//�{�݃i���o�[
	u8	dummy;

	u16 monsno;
	u16 dummy2;

	GF_BGL_INI*	bgl;									//BGL�ւ̃|�C���^
	//GF_BGL_BMPWIN bmpwin[FRONTIER_MONITOR_BMPWIN_MAX];	//BMP�E�B���h�E�f�[�^
	GF_BGL_BMPWIN bmpwin[16];	//BMP�E�B���h�E�f�[�^

	MSGDATA_MANAGER* msgman;							//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;									//�P��Z�b�g
	STRBUF* msg_buf;									//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;									//�e���|�����o�b�t�@�|�C���^

	PALETTE_FADE_PTR pfd;								//�p���b�g�t�F�[�h

	const CONFIG* config;								//�R���t�B�O�|�C���^
	SAVEDATA* sv;										//�Z�[�u�f�[�^�|�C���^
	FRONTIER_SAVEWORK* f_sv;
}FRONTIER_MONITOR_WORK;


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//PROC
PROC_RESULT FrontierMonitorProc_Init( PROC * proc, int * seq );
PROC_RESULT FrontierMonitorProc_Main( PROC * proc, int * seq );
PROC_RESULT FrontierMonitorProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameInit( FRONTIER_MONITOR_WORK* wk );
static BOOL Seq_GameStart( FRONTIER_MONITOR_WORK* wk );
static BOOL Seq_GameEnd( FRONTIER_MONITOR_WORK* wk );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//���ʏ������A�I��
static void Frontier_BgInit( FRONTIER_MONITOR_WORK* wk );
static void Frontier_BgExit( GF_BGL_INI * ini );

//�c�[��
static void NextSeq( FRONTIER_MONITOR_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( void );
static u8 GetBmpNo( u8 fr_no );

//BG�O���t�B�b�N�f�[�^
static void Frontier_SetMainBgGraphic( FRONTIER_MONITOR_WORK * wk, u32 frm  );
static void Frontier_SetMainBgPalette( void );
static void Frontier_SetSubBgGraphic( FRONTIER_MONITOR_WORK * wk, u32 frm  );

//���b�Z�[�W�֘A
static u8 FrontierWriteMsg( FRONTIER_MONITOR_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u8 fill );
static u8 EasyMsg( FRONTIER_MONITOR_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u8 fill );
static void SetNumber( FRONTIER_MONITOR_WORK* wk, u32 bufID, s32 number );
static void Frontier_Write( FRONTIER_MONITOR_WORK* wk );

//�t�@�N�g���[
static void Factory_SetLv50Info( FRONTIER_MONITOR_WORK* wk );
static void Factory_SetLv100Info( FRONTIER_MONITOR_WORK* wk );
static u32 Factory_GetMsg( FRONTIER_MONITOR_WORK* wk, u8 level );

//�X�e�[�W
static void Stage_SetInfo( FRONTIER_MONITOR_WORK* wk );
static u32 Stage_GetMsg( FRONTIER_MONITOR_WORK* wk );
static void FrontierMonitor_PokeName( FRONTIER_MONITOR_WORK* wk );

//�L���b�X��
static void Castle_SetInfo( FRONTIER_MONITOR_WORK* wk );
static u32 Castle_GetMsg( FRONTIER_MONITOR_WORK* wk );

//���[���b�g
static void Roulette_SetInfo( FRONTIER_MONITOR_WORK* wk );
static u32 Roulette_GetMsg( FRONTIER_MONITOR_WORK* wk );

//�^���[
static void Tower_SetInfoSingleDouble( FRONTIER_MONITOR_WORK* wk );
static void Tower_SetInfoMulti( FRONTIER_MONITOR_WORK* wk );
static u32 Tower_GetMsg( FRONTIER_MONITOR_WORK* wk, u8 type );


//==============================================================================================
//
//	PROC
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�v���Z�X�֐��F������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	"������"
 */
//--------------------------------------------------------------
PROC_RESULT FrontierMonitorProc_Init( PROC * proc, int * seq )
{
	int i;
	FRONTIER_MONITOR_WORK* wk;
	FRONTIER_MONITOR_CALL* bfm_call;

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_HBlankIntrSet( NULL,NULL );						//HBlank�Z�b�g
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane( 0 );
	GXS_SetVisiblePlane( 0 );

	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_FRONTIER_MONITOR, 0x20000 );

	wk = PROC_AllocWork( proc, sizeof(FRONTIER_MONITOR_WORK), HEAPID_FRONTIER_MONITOR );
	memset( wk, 0, sizeof(FRONTIER_MONITOR_WORK) );

	wk->proc	= proc;
	wk->bgl		= GF_BGL_BglIniAlloc( HEAPID_FRONTIER_MONITOR );
	bfm_call	= (FRONTIER_MONITOR_CALL*)PROC_GetParentWork( proc );
	wk->sv		= bfm_call->sv;
	wk->f_sv	= SaveData_GetFrontier( wk->sv );
	wk->type	= bfm_call->type;
	wk->fr_no	= bfm_call->fr_no;
	wk->monsno	= bfm_call->monsno;
	wk->config	= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾

	Frontier_BgInit( wk );								//BG������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_bf_seiseki_dat, HEAPID_FRONTIER_MONITOR );

	wk->wordset = WORDSET_Create( HEAPID_FRONTIER_MONITOR );
	wk->msg_buf = STRBUF_Create( FRONTIER_MSG_BUF_SIZE, HEAPID_FRONTIER_MONITOR );
	wk->tmp_buf = STRBUF_Create( FRONTIER_MSG_BUF_SIZE, HEAPID_FRONTIER_MONITOR );

	//�t�H���g�p���b�g
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, BFM_FONT_PAL * 32, HEAPID_FRONTIER_MONITOR );
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, BFM_MSGFONT_PAL * 32, HEAPID_FRONTIER_MONITOR );

	//�r�b�g�}�b�v�ǉ�
	FrontierMonitorAddBmpWin( wk->bgl, wk->bmpwin, GetBmpNo(wk->fr_no) );

	sys_VBlankFuncChange( VBlankFunc, (void*)wk );		//VBlank�Z�b�g
	
	(*seq) = SEQ_GAME_INIT;

	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief	�v���Z�X�֐��F���C��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	"������"
 */
//--------------------------------------------------------------
PROC_RESULT FrontierMonitorProc_Main( PROC * proc, int * seq )
{
	FRONTIER_MONITOR_WORK * wk  = PROC_GetWork( proc );

	switch( *seq ){

	//������
	case SEQ_GAME_INIT:
		if( Seq_GameInit(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_START );
		}
		break;

	//�Q�[���J�n
	case SEQ_GAME_START:
		if( Seq_GameStart(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_END );		//�I��
		}
		break;

	//�I��
	case SEQ_GAME_END:
		if( Seq_GameEnd(wk) == TRUE ){
			return PROC_RES_FINISH;
		}
	}

	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief	�v���Z�X�֐��F�I��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	"������"
 */
//--------------------------------------------------------------
PROC_RESULT FrontierMonitorProc_End( PROC * proc, int * seq )
{
	int i;
	FRONTIER_MONITOR_WORK* wk = PROC_GetWork( proc );

	//�p���b�g�t�F�[�h�J��
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );

	//�p���b�g�t�F�[�h�V�X�e���J��
	PaletteFadeFree( wk->pfd );
	wk->pfd = NULL;

	MSGMAN_Delete( wk->msgman );						//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );						//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );						//���b�Z�[�W�o�b�t�@�J��

	FrontierMonitorExitBmpWin( wk->bmpwin, GetBmpNo(wk->fr_no) );	//BMP�E�B���h�E�J��

	Frontier_BgExit( wk->bgl );							//BGL�폜

	PROC_FreeWork( proc );								//���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_DeleteHeap( HEAPID_FRONTIER_MONITOR );

	return PROC_RES_FINISH;
}


//==============================================================================================
//
//	�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�Q�[��������
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( FRONTIER_MONITOR_WORK* wk )
{
	switch( wk->sub_seq ){

	case 0:
		wk->sub_seq++;
		break;

	//�u���b�N�C��
	case 1:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_FRONTIER_MONITOR );
		Frontier_Write( wk );
		wk->sub_seq++;
		break;
		
	//�t�F�[�h�I���҂�
	case 2:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�n
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameStart( FRONTIER_MONITOR_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//
	case 0:
		wk->sub_seq++;
		break;

	//
	case 1:
		//BgCheck();									//BG��ON�EOFF(�f�o�b�N)

		//�I���`�F�b�N
		if( (sys.cont & PAD_BUTTON_A) ||
			(sys.cont & PAD_BUTTON_B) ){
			return TRUE;
		}

		//wk->sub_seq++;
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�I����
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( FRONTIER_MONITOR_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//�t�F�[�h�A�E�g
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FRONTIER_MONITOR );
		wk->sub_seq++;
		break;

	//�t�F�[�h�I���҂�
	case 1:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;
	};

	return FALSE;
}


//==============================================================================================
//
//	�ݒ�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	VBlank�֐�
 *
 * @param	work	���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------
static void VBlankFunc( void * work )
{
	FRONTIER_MONITOR_WORK* wk = work;

	//�p���b�g�]��
	if( wk->pfd != NULL ){
		PaletteFadeTrans( wk->pfd );
	}

	GF_BGL_VBlankFunc( wk->bgl );

	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------
/**
 * @brief	VRAM�ݒ�
 *
 * @param	none
 *
 * @return	none
 *
 * �ׂ����ݒ肵�Ă��Ȃ��̂Œ��ӁI
 */
//--------------------------------------------------------------
static void SetVramBank(void)
{
	GF_BGL_DISPVRAM tbl = {
		GX_VRAM_BG_128_A,				//���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			//���C��2D�G���W����BG�g���p���b�g

		GX_VRAM_SUB_BG_128_C,			//�T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		//�T�u2D�G���W����BG�g���p���b�g

		GX_VRAM_OBJ_128_B,				//���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		//���C��2D�G���W����OBJ�g���p���b�g

		GX_VRAM_SUB_OBJ_16_I,			//�T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	//�T�u2D�G���W����OBJ�g���p���b�g

		GX_VRAM_TEX_NONE,				//�e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_NONE			//�e�N�X�`���p���b�g�X���b�g
	};

	GF_Disp_SetBank( &tbl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BG�ݒ�
 *
 * @param	init	BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetBgHeader( GF_BGL_INI * ini )
{
	/******************/
	//CHRA,SCR�͗v�m�F�I
	/******************/

	{	//BG SYSTEM
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	{	//BG(�t�H���g)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BFM_FRAME_WIN, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BFM_FRAME_WIN, 32, 0, HEAPID_FRONTIER_MONITOR );
		GF_BGL_ScrClear( ini, BFM_FRAME_WIN );
	}

	{	//����
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BFM_FRAME_BG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BFM_FRAME_BG, 32, 0, HEAPID_FRONTIER_MONITOR );
		GF_BGL_ScrClear( ini, BFM_FRAME_BG );
	}

	//--------------------------------------------------------------------------------
	{	//�����(�{�[��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BFM_FRAME_SUB, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BFM_FRAME_SUB, 32, 0, HEAPID_FRONTIER_MONITOR );
		GF_BGL_ScrClear( ini, BFM_FRAME_SUB );
	}

	G2_SetBG0Priority( 0 );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
	return;
}


//==============================================================================================
//
//	���ʏ������A�I��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	BG�֘A������
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Frontier_BgInit( FRONTIER_MONITOR_WORK* wk )
{
	SetVramBank();
	SetBgHeader( wk->bgl );

	//�p���b�g�t�F�[�h�V�X�e�����[�N�쐬
	wk->pfd = PaletteFadeInit( HEAPID_FRONTIER_MONITOR );

	//���N�G�X�g�f�[�^��malloc���ăZ�b�g
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_OBJ, FADE_PAL_ALL_SIZE, HEAPID_FRONTIER_MONITOR );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ALL_SIZE, HEAPID_FRONTIER_MONITOR );

	//���ʔw�i
	Frontier_SetMainBgGraphic( wk, BFM_FRAME_BG );
	Frontier_SetMainBgPalette();

	//����ʔw�i�A�p���b�g�Z�b�g
	Frontier_SetSubBgGraphic( wk, BFM_FRAME_SUB );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BG���
 *
 * @param	ini		BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Frontier_BgExit( GF_BGL_INI * ini )
{
	//���C����ʂ̊e�ʂ̕\���R���g���[��(�\��OFF)
	GF_Disp_GX_VisibleControl(	GX_PLANEMASK_BG0 | 
								GX_PLANEMASK_BG1 | 
								GX_PLANEMASK_BG2 |
								GX_PLANEMASK_BG3 | 
								GX_PLANEMASK_OBJ, 
								VISIBLE_OFF );

	//�T�u��ʂ̊e�ʂ̕\���R���g���[��(�\��OFF)
	GF_Disp_GXS_VisibleControl(	GX_PLANEMASK_BG0 | 
								GX_PLANEMASK_BG1 | 
								GX_PLANEMASK_BG2 | 
								GX_PLANEMASK_BG3 | 
								GX_PLANEMASK_OBJ, 
								VISIBLE_OFF );

	//GF_BGL_BGControlSet�Ŏ擾�������������J��
	GF_BGL_BGControlExit( ini, BFM_FRAME_WIN );
	GF_BGL_BGControlExit( ini, BFM_FRAME_BG );
	GF_BGL_BGControlExit( ini, BFM_FRAME_SUB );

	sys_FreeMemoryEz( ini );
	return;
}


//==============================================================================================
//
//	�c�[��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�ύX
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( FRONTIER_MONITOR_WORK* wk, int* seq, int next )
{
	wk->sub_seq = 0;
	*seq		= next;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�L�[�`�F�b�N
 *
 * @param	key		�`�F�b�N����L�[
 *
 * @return	"����"
 */
//--------------------------------------------------------------
static int KeyCheck( int key )
{
	return (sys.cont & key);
}

//--------------------------------------------------------------
/**
 * @brief	BG�`�F�b�N
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck(void)
{
	if( sys.cont & PAD_KEY_DOWN ){
		if( sys.cont & PAD_BUTTON_L ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_RIGHT ){
		if( sys.cont & PAD_BUTTON_L ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�{�݃i���o�[����BMP�i���o�[���擾
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static u8 GetBmpNo( u8 fr_no )
{
	u8 bmp_no;

	bmp_no = 0;

	switch( fr_no ){

	case FRONTIER_NO_FACTORY_LV50:
	case FRONTIER_NO_FACTORY_LV100:
		bmp_no = 0;
		break;

	case FRONTIER_NO_STAGE:
		bmp_no = 1;
		break;

	case FRONTIER_NO_CASTLE:
		bmp_no = 2;
		break;

	case FRONTIER_NO_ROULETTE:
		bmp_no = 3;
		break;

	case FRONTIER_NO_TOWER:
		bmp_no = 4;
		break;
	};

	return bmp_no;
}


//==============================================================================================
//
//	BG�O���t�B�b�N�f�[�^
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʔw�i(�V���O���A�_�u��)
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Frontier_SetMainBgGraphic( FRONTIER_MONITOR_WORK * wk, u32 frm  )
{
	ARCHANDLE* hdl;
	hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FRONTIER_MONITOR );

	ArcUtil_HDL_BgCharSet(	hdl, SEISEKI_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FRONTIER_MONITOR );

	ArcUtil_HDL_ScrnSet(hdl, SEISEKI_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_FRONTIER_MONITOR );

	ArchiveDataHandleClose( hdl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���ʔw�i�p���b�g�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Frontier_SetMainBgPalette(void)
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet(ARC_FRONTIER_BG, SEISEKI_NCLR, &dat,HEAPID_FRONTIER_MONITOR);
		
	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*2) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*2) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i�A�p���b�g�Z�b�g
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Frontier_SetSubBgGraphic( FRONTIER_MONITOR_WORK * wk, u32 frm  )
{
	ARCHANDLE* hdl;
		
	//�Ƃ肠�����|�P�b�`�ɂ��鉺��ʂ��g���܂킷
	hdl = ArchiveDataHandleOpen( ARC_POKETCH_IMG, HEAPID_FRONTIER_MONITOR );

	ArcUtil_HDL_BgCharSet(	hdl, NARC_poketch_before_lz_ncgr, wk->bgl, frm,
							0, 0, TRUE, HEAPID_FRONTIER_MONITOR );

	ArcUtil_HDL_ScrnSet(hdl, NARC_poketch_before_lz_nscr, wk->bgl, frm,
						0, 0, TRUE, HEAPID_FRONTIER_MONITOR );

	ArcUtil_HDL_PalSet(	hdl, NARC_poketch_before_nclr, PALTYPE_SUB_BG,
						0, 0x20, HEAPID_FRONTIER_MONITOR );
		
	ArchiveDataHandleClose( hdl );
	return;
}


//==============================================================================================
//
//	���b�Z�[�W�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�\��
 *
 * @param	wk		FRONTIER_MONITOR_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	msg_id	���b�Z�[�WID
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 * @param	fill	�h��Ԃ��L�薳��
 *
 * @return	"�����`�惋�[�`���̃C���f�b�N�X"
 */
//--------------------------------------------------------------
static u8 FrontierWriteMsg( FRONTIER_MONITOR_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u8 fill )
{
	if( fill != 0 ){
		GF_BGL_BmpWinDataFill( win, b_col );			//�h��Ԃ�
	}

	MSGMAN_GetString( wk->msgman, msg_id, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	return GF_STR_PrintColor( win, font, wk->msg_buf, x, y, MSG_ALLPUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );
}

//--------------------------------------------------------------
/**
 * @brief	�ȒP���b�Z�[�W�\��
 *
 * @param	win		GF_BGL_BMPWIN�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 EasyMsg( FRONTIER_MONITOR_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u8 fill )
{
	return FrontierWriteMsg(	wk, win, msg_id, 1, 1,
							FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, fill );
}

//--------------------------------------------------------------
/**
 * @brief	���l���Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * 4���Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void SetNumber( FRONTIER_MONITOR_WORK* wk, u32 bufID, s32 number )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, 4, 
							NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�e�{�݂��Ƃ̏�����������
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Frontier_Write( FRONTIER_MONITOR_WORK* wk )
{
	int msg_id;

	switch( wk->fr_no ){

	//�t�@�N�g���[
	case FRONTIER_NO_FACTORY_LV50:
	case FRONTIER_NO_FACTORY_LV100:
		Factory_SetLv50Info( wk );	//�u�k�u�T�O�v
		Factory_SetLv100Info( wk );	//�u�I�[�v���v
		break;

	//�X�e�[�W
	case FRONTIER_NO_STAGE:
		Stage_SetInfo( wk );
		break;

	case FRONTIER_NO_CASTLE:
		Castle_SetInfo( wk );
		break;

	case FRONTIER_NO_ROULETTE:
		Roulette_SetInfo( wk );
		break;

	case FRONTIER_NO_TOWER:
		if( (wk->type == BTWR_MODE_SINGLE) ||
			(wk->type == BTWR_MODE_DOUBLE) ){
			Tower_SetInfoSingleDouble( wk );
		}else{
			Tower_SetInfoMulti( wk );
		}
		break;

	//�G���[���
	default:
		Roulette_SetInfo( wk );
		break;

	};

	return;
}


//==============================================================================================
//
//	�t�@�N�g���[
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	LV50�����Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_SetLv50Info( FRONTIER_MONITOR_WORK* wk )
{
	u8 no,clear_flag;
	int msg_id;
	u32 x;

	//----------------------------------------------------------
	no = BF_BMPWIN_TITLE;

	GF_BGL_BmpWinDataFill( &wk->bmpwin[no], FBMP_COL_NULL );			//�h��Ԃ�

	//�u�o�g���t�@�N�g���[�v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list02, BF_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	if( wk->type == FACTORY_TYPE_SINGLE ){
		msg_id = msg_bf_seiseki_list06;
		x = TITLE_STR_SINGLE_X_FACTORY;
	}else if( wk->type == FACTORY_TYPE_DOUBLE ){
		msg_id = msg_bf_seiseki_list07;
		x = TITLE_STR_X_FACTORY;
	}else{
		msg_id = msg_bf_seiseki_list08;
		x = TITLE_STR_X_FACTORY;
	}

	//�u�V���O�����������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_id, x, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	
	//----------------------------------------------------------
	no = BF_BMPWIN_ENTRY1;
	//�u�k�u�T�O�v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list13, 1 );
	//�u�����^���^��������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list15, 16*8, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BF_BMPWIN_BEFORE;
	//�u���񂩂��v�u���񂴂��v
	EasyMsg( wk, &wk->bmpwin[no], Factory_GetMsg(wk,0), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv, FactoryScr_GetWinRecordID(FACTORY_LEVEL_50,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u���������v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv,FactoryScr_GetTradeRecordID(FACTORY_LEVEL_50,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list19, TRD_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BF_BMPWIN_MAX;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
		FrontierRecord_Get(	wk->f_sv,FactoryScr_GetMaxWinRecordID(FACTORY_LEVEL_50,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u���������v
	SetNumber(	wk, 0,
		FrontierRecord_Get( wk->f_sv,FactoryScr_GetMaxTradeRecordID(FACTORY_LEVEL_50,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list19, TRD_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	LV100�����Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_SetLv100Info( FRONTIER_MONITOR_WORK* wk )
{
	u8 no;
	u32 x;

	//----------------------------------------------------------
	no = BF_BMPWIN_ENTRY2;
	//�u�I�[�v�����x���v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list14, 1 );

	//�u�����^���^��������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list15, 19*8, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BF_BMPWIN_BEFORE2;
	//�u���񂩂��v�u���񂴂��v
	EasyMsg( wk, &wk->bmpwin[no], Factory_GetMsg(wk,1), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv, FactoryScr_GetWinRecordID(FACTORY_LEVEL_OPEN,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u���������v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv, FactoryScr_GetTradeRecordID(FACTORY_LEVEL_OPEN,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list19, TRD_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BF_BMPWIN_MAX2;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv,FactoryScr_GetMaxWinRecordID(FACTORY_LEVEL_OPEN,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u���������v
	SetNumber(	wk, 0,
		FrontierRecord_Get( wk->f_sv, FactoryScr_GetMaxTradeRecordID(FACTORY_LEVEL_OPEN,wk->type),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list19, TRD_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

static u32 Factory_GetMsg( FRONTIER_MONITOR_WORK* wk, u8 level )
{
	u8 clear_flag;
	u32 clear_id;

	if( level == 0 ){
		clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR_BIT;
	}else{
		clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT;
	}

	//WIFI�̂ݓ���
	if( wk->type == FACTORY_TYPE_WIFI_MULTI ){

		clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), clear_id,
										Frontier_GetFriendIndex(clear_id) );
	}else{
		//�N���A�������t���O���擾
		clear_flag = (u8)FACTORYSCORE_GetScoreData(	SaveData_GetFactoryScore(wk->sv), 
													FACTORYSCORE_ID_CLEAR_FLAG, 
													(level*FACTORY_TYPE_MAX)+wk->type, NULL );
	}

	if( clear_flag == 1 ){
		//�u���񂴂��v
		return msg_bf_seiseki_list23;
	}

	//�u���񂩂��v
	return msg_bf_seiseki_list09;
}


//==============================================================================================
//
//	�X�e�[�W
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�����Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Stage_SetInfo( FRONTIER_MONITOR_WORK* wk )
{
	u8 no;
	u16 before_monsno;
	int msg_id;
	u32 x;
	int num;
	FRONTIER_EX_SAVEDATA * fes;
	LOAD_RESULT load_result;

	//----------------------------------------------------------
	no = BS_BMPWIN_TITLE;

	GF_BGL_BmpWinDataFill( &wk->bmpwin[no], FBMP_COL_NULL );			//�h��Ԃ�

	//�u�o�g���X�e�[�W�v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list04, BF_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	if( wk->type == STAGE_TYPE_SINGLE ){
		msg_id = msg_bf_seiseki_list06;
		x = TITLE_STR_SINGLE_X_STAGE;
	}else if( wk->type == STAGE_TYPE_DOUBLE ){
		msg_id = msg_bf_seiseki_list07;
		x = TITLE_STR_X_STAGE;
	}else{
		msg_id = msg_bf_seiseki_list08;
		x = TITLE_STR_X_STAGE;
	}

	//�u�V���O�����������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_id, x, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	
	//----------------------------------------------------------
	no = BS_BMPWIN_ENTRY1;
	//�|�P�������Z�b�g
	FrontierMonitor_PokeName( wk );
	//EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_03_01, 1 );

	//FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list18, 19*8, 0,
	//FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list18, 4*8, 0,
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list18, 0, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BS_BMPWIN_BEFORE;
	//�u���񂩂��v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list09, 1 );

	//���풆�̃|�P�����i���o�[���擾
	before_monsno = FrontierRecord_Get(	SaveData_GetFrontier(wk->sv), 
							StageScr_GetMonsNoRecordID(wk->type),
							Frontier_GetFriendIndex(StageScr_GetMonsNoRecordID(wk->type)) );

	//���풆�ȊO�̃|�P�����̌��݂̘A������0�ɂ���
	if( wk->monsno != before_monsno ){
		num = 0;
	}else{
		num = FrontierRecord_Get(	wk->f_sv, StageScr_GetWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND );
	}
	OS_Printf( "monsno = %d\n", wk->monsno );
	OS_Printf( "num = %d\n", num );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, num );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BS_BMPWIN_MAX;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

#if 0
	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, StageScr_GetMaxWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
#else
	fes = FrontierEx_Load( wk->sv, HEAPID_WORLD, &load_result );
	if( load_result != LOAD_RESULT_OK ){
		num = 0;	//�Z�[�u�j��or�L�[�ƈ�v���Ȃ��A�ꍇ�͐V�K�f�[�^�Ɖ��߂��A�J�E���^0�Ƃ���
	}else{
		num = FrontierEx_StageRenshou_Get(	wk->sv, fes, 
											StageScr_GetExMaxWinRecordID(wk->type), wk->monsno );
	}
	OS_Printf( "num = %d\n", num );

#if 0
	//�f�o�b�N
	OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get(	wk->sv, fes, 
										StageScr_GetExMaxWinRecordID(wk->type), MONSNO_BIIBARU ) );
	OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get(	wk->sv, fes, 
										StageScr_GetExMaxWinRecordID(wk->type), MONSNO_KAIRIKII ) );
	OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get(	wk->sv, fes, 
										StageScr_GetExMaxWinRecordID(wk->type), MONSNO_HIKOZARU ) );
#endif

	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}

	SetNumber(	wk, 0, num );
#endif
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

static u32 Stage_GetMsg( FRONTIER_MONITOR_WORK* wk )
{
#if 0
	u8 clear_flag;

	//WIFI�̂ݓ���
	if( wk->type == STAGE_TYPE_WIFI_MULTI ){
		clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
										FRID_STAGE_MULTI_WIFI_CLEAR_BIT,
										Frontier_GetFriendIndex(FRID_STAGE_MULTI_WIFI_CLEAR_BIT) );
	}else{
		//�N���A�������t���O���擾
		clear_flag = (u8)STAGESCORE_GetScoreData(	SaveData_GetStageScore(wk->sv), 
													STAGESCORE_ID_CLEAR_FLAG, 
													wk->type, 0, NULL );
	}

	if( clear_flag == 1 ){
		//�u���񂴂��v
		return msg_bf_seiseki_list23;
	}
#endif

	//�u���񂩂��v
	return msg_bf_seiseki_list09;
}

//--------------------------------------------------------------------------------------------
/**
 * �����X�^�[�i���o�[����|�P���������w��o�b�t�@�ɓo�^
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------------------------------------
static void FrontierMonitor_PokeName( FRONTIER_MONITOR_WORK* wk )
{
	MSGDATA_MANAGER* man;
	STRBUF* buf;

	man = MSGMAN_Create(MSGMAN_TYPE_DIRECT,ARC_MSG,NARC_msg_monsname_dat,HEAPID_FRONTIER_MONITOR);
	buf = MSGMAN_AllocString( man, wk->monsno );
	MSGMAN_Delete( man );

	//�P��Z�b�g
	//�o�b�t�@ID
	//������
	//���ʃR�[�h
	//�P�^���iTRUE�ŒP���j
	//����R�[�h
	WORDSET_RegisterWord( wk->wordset, 0, buf, 0, 0, PM_LANG );
	STRBUF_Delete( buf );
	return;
}


//==============================================================================================
//
//	�L���b�X��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�����Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Castle_SetInfo( FRONTIER_MONITOR_WORK* wk )
{
	u8 no;
	u16 now_cp;
	int msg_id;
	u32 x;

	//----------------------------------------------------------
	no = BC_BMPWIN_TITLE;

	GF_BGL_BmpWinDataFill( &wk->bmpwin[no], FBMP_COL_NULL );			//�h��Ԃ�

	//�u�o�g���L���b�X���v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list03, BF_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	if( wk->type == CASTLE_TYPE_SINGLE ){
		msg_id = msg_bf_seiseki_list06;
		x = TITLE_STR_SINGLE_X_CASTLE;
	}else if( wk->type == CASTLE_TYPE_DOUBLE ){
		msg_id = msg_bf_seiseki_list07;
		x = TITLE_STR_X_CASTLE;
	}else{
		msg_id = msg_bf_seiseki_list08;
		x = TITLE_STR_X_CASTLE;
	}

	//�u�V���O�����������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_id, x, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BC_BMPWIN_ENTRY1;
	//�u�L���b�X���|�C���g�v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list16, 1 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BC_BMPWIN_BEFORE;
	//�u���񂩂��v�u���񂴂��v
	EasyMsg( wk, &wk->bmpwin[no], Castle_GetMsg(wk), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, CastleScr_GetWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list21, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u�����b�o�v�c��CP��\��
	now_cp = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );
	//OS_Printf( "�O��c��cp = %d\n", now_cp );
	SetNumber(	wk, 0, now_cp );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list20, CP_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BC_BMPWIN_MAX;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, CastleScr_GetMaxWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list21, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u�����b�o�v�c��CP��\��
	now_cp = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
							CastleScr_GetRemainderCPRecordID(wk->type),
							Frontier_GetFriendIndex(CastleScr_GetRemainderCPRecordID(wk->type)) );
	//OS_Printf( "�ō��c��cp = %d\n", now_cp );
	SetNumber(	wk, 0, now_cp );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list20, CP_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

static u32 Castle_GetMsg( FRONTIER_MONITOR_WORK* wk )
{
	u8 clear_flag;

	//WIFI�̂ݓ���
	if( wk->type == CASTLE_TYPE_WIFI_MULTI ){
		clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
										FRID_CASTLE_MULTI_WIFI_CLEAR_BIT,
										Frontier_GetFriendIndex(FRID_CASTLE_MULTI_WIFI_CLEAR_BIT) );
	}else{
		//�N���A�������t���O���擾
		clear_flag = (u8)CASTLESCORE_GetScoreData(	SaveData_GetCastleScore(wk->sv), 
													CASTLESCORE_ID_CLEAR_FLAG, 
													wk->type, 0, NULL );
	}

	if( clear_flag == 1 ){
		//�u���񂴂��v
		return msg_bf_seiseki_list23;
	}

	//�u���񂩂��v
	return msg_bf_seiseki_list09;
}


//==============================================================================================
//
//	���[���b�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�����Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Roulette_SetInfo( FRONTIER_MONITOR_WORK* wk )
{
	u8 no;
	int msg_id;
	u32 x;

	//----------------------------------------------------------
	no = BR_BMPWIN_TITLE;


	GF_BGL_BmpWinDataFill( &wk->bmpwin[no], FBMP_COL_NULL );			//�h��Ԃ�

	//�u�o�g�����[���b�g�v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list05, BF_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	if( wk->type == ROULETTE_TYPE_SINGLE ){
		msg_id = msg_bf_seiseki_list06;
		x = TITLE_STR_SINGLE_X_ROULETTE;
	}else if( wk->type == ROULETTE_TYPE_DOUBLE ){
		msg_id = msg_bf_seiseki_list07;
		x = TITLE_STR_X_ROULETTE;
	}else{
		msg_id = msg_bf_seiseki_list08;
		x = TITLE_STR_X_ROULETTE;
	}

	//�u�V���O�����������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_id, x, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BR_BMPWIN_BEFORE;
	//�u���񂩂��v�u���񂴂��v
	EasyMsg( wk, &wk->bmpwin[no], Roulette_GetMsg(wk), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, RouletteScr_GetWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list22, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BR_BMPWIN_MAX;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, RouletteScr_GetMaxWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list22, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

static u32 Roulette_GetMsg( FRONTIER_MONITOR_WORK* wk )
{
	u8 clear_flag;

	//WIFI�̂ݓ���
	if( wk->type == ROULETTE_TYPE_WIFI_MULTI ){
		clear_flag = FrontierRecord_Get(SaveData_GetFrontier(wk->sv), 
									FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
									Frontier_GetFriendIndex(FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT) );
	}else{
		//�N���A�������t���O���擾
		clear_flag = (u8)ROULETTESCORE_GetScoreData(SaveData_GetRouletteScore(wk->sv), 
													ROULETTESCORE_ID_CLEAR_FLAG, 
													wk->type, 0, NULL );
	}

	if( clear_flag == 1 ){
		//�u���񂴂��v
		return msg_bf_seiseki_list23;
	}

	//�u���񂩂��v
	return msg_bf_seiseki_list09;
}


//==============================================================================================
//
//	�^���[
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�V���O���A�_�u�������Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Tower_SetInfoSingleDouble( FRONTIER_MONITOR_WORK* wk )
{
	u8 no;
	int msg_id;
	u32 x;

	//----------------------------------------------------------
	no = BT_BMPWIN_TITLE;

	GF_BGL_BmpWinDataFill( &wk->bmpwin[no], FBMP_COL_NULL );			//�h��Ԃ�

	//�u�o�g���^���[�v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list01, BF_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	if( wk->type == BTWR_MODE_SINGLE ){
		msg_id = msg_bf_seiseki_list06;
		x = TITLE_STR_SINGLE_X_TOWER;
	}else{
		msg_id = msg_bf_seiseki_list07;
		x = TITLE_STR_X_TOWER;
	}

	//�u�V���O�����������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_id, x, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BT_BMPWIN_BEFORE0;
	//�u���񂩂��v
	EasyMsg( wk, &wk->bmpwin[no], Tower_GetMsg(wk,wk->type), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, TowerScr_GetWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BT_BMPWIN_MAX0;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, TowerScr_GetMaxWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�}���`�����Z�b�g
 *
 * @param	wk		FRONTIER_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Tower_SetInfoMulti( FRONTIER_MONITOR_WORK* wk )
{
	u8 no;
	u32 x;

	//----------------------------------------------------------
	no = BT_BMPWIN_TITLE;

	GF_BGL_BmpWinDataFill( &wk->bmpwin[no], FBMP_COL_NULL );			//�h��Ԃ�

	//�u�o�g���^���[�v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list01, BF_STR_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	//�u�V���O�����������v
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list08, TITLE_STR_X_TOWER, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BT_BMPWIN_ENTRY1;
	//�u�g���[�i�[�ƃ}���`�v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list11, 1 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BT_BMPWIN_BEFORE;
	//�u���񂩂��v
	EasyMsg( wk, &wk->bmpwin[no], Tower_GetMsg(wk,wk->type), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, TowerScr_GetWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
		
	//----------------------------------------------------------
	no = BT_BMPWIN_MAX;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
				FrontierRecord_Get( wk->f_sv, TowerScr_GetMaxWinRecordID(wk->type),
									FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BT_BMPWIN_ENTRY2;
	//�u�Ƃ������ƃ}���`�v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list12, 1 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BT_BMPWIN_BEFORE2;
	//�u���񂩂��v
	EasyMsg( wk, &wk->bmpwin[no], Tower_GetMsg(wk,BTWR_MODE_COMM_MULTI), 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv, TowerScr_GetWinRecordID(BTWR_MODE_COMM_MULTI),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );

	//----------------------------------------------------------
	no = BT_BMPWIN_MAX2;
	//�u���������v
	EasyMsg( wk, &wk->bmpwin[no], msg_bf_seiseki_list10, 1 );

	//�u������񂵂傤�v
	SetNumber(	wk, 0, 
		FrontierRecord_Get( wk->f_sv,TowerScr_GetMaxWinRecordID(BTWR_MODE_COMM_MULTI),
							FRONTIER_RECORD_NOT_FRIEND) );
	FrontierWriteMsg(wk, &wk->bmpwin[no], msg_bf_seiseki_list17, WIN_NUM_X, 0,
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM, 0 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[no] );
	return;
}

static u32 Tower_GetMsg( FRONTIER_MONITOR_WORK* wk, u8 type )
{
	u16 id;
	int flag;
	BTLTOWER_SCOREWORK* scoreSave;

	scoreSave = SaveData_GetTowerScoreData( wk->sv );

	//���[�h�ɂ���ĕK�v��ID���擾
	switch( type ){
	case BTWR_MODE_SINGLE:
		id = BTWR_SFLAG_SINGLE_RECORD;
		break;

	case BTWR_MODE_DOUBLE:
		id = BTWR_SFLAG_DOUBLE_RECORD;
		break;

	case BTWR_MODE_MULTI:
		id = BTWR_SFLAG_MULTI_RECORD;
		break;

	case BTWR_MODE_COMM_MULTI:
		id = BTWR_SFLAG_CMULTI_RECORD;
		break;

	case BTWR_MODE_WIFI:
		id = BTWR_SFLAG_WIFI_RECORD;
		break;

	//case BTWR_MODE_RETRY:
	//	break;

	case BTWR_MODE_WIFI_MULTI:		//07.08.06 �v���`�i�ǉ�(COMM_MULTI��WIFI�� �F�B�蒠)
		id = BTWR_SFLAG_WIFI_MULTI_RECORD;		//�v���`�i�ǉ�
		break;

	//�G���[���
	default:
		id = BTWR_SFLAG_SINGLE_RECORD;
		break;
	};

	flag = TowerScoreData_SetFlags( scoreSave, id, BTWR_DATA_get );
	OS_Printf( "�^���[�̃N���A��� = %d\n", flag );

	if( flag == 1 ){
		//�u���񂴂��v
		return msg_bf_seiseki_list23;
	}

	//�u���񂩂��v
	return msg_bf_seiseki_list09;
}


