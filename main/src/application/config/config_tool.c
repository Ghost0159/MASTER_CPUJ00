/**
 *	@file	config.c
 *	@brief	�R���t�B�O���
 *	@date	06.01.30
 *	@author	Miyuki Iwasawa
 */
#include "common.h"
#include "system/procsys.h"
#include "system/arc_util.h"
#include "system/arc_tool.h"
#include "system/brightness.h"
#include "system/bmp_list.h"
#include "system/bmp_menu.h"
#include "system/window.h"
#include "system/pm_str.h"
#include "system/msgdata.h"
#include "system/fontproc.h"
#include "system/snd_tool.h"
#include "system/clact_util.h"
#include "system/clact_tool.h"
#include "system/render_oam.h"
#include "system/pmfprint.h"
#include "system/wipe.h"
#include "application/app_tool.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_config.h"

#include "application/config_tool.h"
#include "savedata/config.h"
#include "communication/communication.h"

#include "config_gra.naix"

#define CONFIG_DECIDE_END	(1)
#define CONFIG_CANCEL_END	(2)

#define FONT_TALK_PAL	(12)
#define FONT_SYS_PAL	(13)

#define WINCLR_COL(col)	(((col)<<4)|(col))

#define CONFIG_BMPCHAR_BASE	(10)
//20
#define BMPL_TITLE_PX	(1)
#define BMPL_TITLE_PY	(0)
#define BMPL_TITLE_SX	(12)
#define BMPL_TITLE_SY	(2)
#define BMPL_TITLE_PAL	(FONT_SYS_PAL)
#define BMPL_TITLE_CGX	(CONFIG_BMPCHAR_BASE)
#define BMPL_TITLE_FRM	(GF_BGL_FRAME1_M)

//600
#define BMPL_MENU_PX	(1)
#define BMPL_MENU_PY	(3)
#define BMPL_MENU_SX	(30)
#define BMPL_MENU_SY	(14)
#define BMPL_MENU_PAL	(FONT_SYS_PAL)
#define BMPL_MENU_CGX	(BMPL_TITLE_CGX+BMPL_TITLE_SX*BMPL_TITLE_SY)
#define BMPL_MENU_FRM	(GF_BGL_FRAME1_M)

//112
#define BMPL_SUM_PX	(2)
#define BMPL_SUM_PY	(19)
#define BMPL_SUM_SX	(27)
#define BMPL_SUM_SY	(4)
#define BMPL_SUM_PAL	(FONT_TALK_PAL)
#define BMPL_SUM_CGX	(BMPL_MENU_CGX+BMPL_MENU_SX*BMPL_MENU_SY)
#define BMPL_SUM_FRM	(GF_BGL_FRAME1_M)

#define BMPL_MENU_WIN_CGX	(BMPL_SUM_CGX+BMPL_SUM_SX*BMPL_SUM_SY)
#define BMPL_TALK_WIN_CGX	(BMPL_MENU_WIN_CGX+MENU_WIN_CGX_SIZ)
#define BMPL_MENU_WIN_PAL	(14)
#define BMPL_TALK_WIN_PAL	(15)

// YesNoWin
#define BMPL_YESNO_CX	(25)
#define BMPL_YESNO_CY	(13)
#define BMPL_YESNO_CSX	(6)
#define BMPL_YESNO_CSY	(4)
#define BMPL_YESNO_PAL	(FONT_SYS_PAL)
#define BMPL_YESNO_CGX	(BMPL_TALK_WIN_CGX+TALK_WIN_CGX_SIZ)
#define BMPL_YESNO_FRM	(GF_BGL_FRAME3_M)

enum{
	WIN_TITLE,
	WIN_MENU,
	WIN_SUM,
	WIN_MAX
};

#define MENU_SEL_MAX	(20)
#define MENU_LINE_MAX	(7)

#define MENU_STRSIZ	(10*2)
#define SUM_STRSIZ	(256)

#define MENU_SEL_OFSX	(12*8+4)
#define MENU_SEL_OFSY	(0)
#define MENU_SEL_DSIZ	(48)
#define MENU_SEL_ASIZX	(48*8)
#define MENU_SEL_ASIZY	(16)
#define MENU_WIN_OFSY	(24)


enum{
 MENU_MSG_SPD,
 MENU_SOUND,
 MENU_BTL_EFF,
 MENU_BTL_RULE,
 MENU_BTN_MODE,
 MENU_WIN,
 MENU_MAX,
};

enum{
	CONFIG_SEQ_INITFADEIN,			// ��ʂ̏������ƃt�F�[�h�C��
	CONFIG_SEQ_INITFADEINWAIT,		// �t�F�[�h�C���܂�
	CONFIG_SEQ_MAIN,				// ���C��
	CONFIG_SEQ_SETCHECKINIT,		// �ύX�𔽉f�����邩�`�F�b�N�@���b�Z�[�W�\��
	CONFIG_SEQ_SETCHECK,			// �ύX�𔽉f�����邩�`�F�b�N�@YESNO�E�B���h�E�\��
	CONFIG_SEQ_SETWAIT,				// �ύX�𔽉f�����邩�`�F�b�N�@YESNO�Z���N�g
	CONFIG_SEQ_FADEOUT,				// �t�F�[�h�A�E�g
	CONFIG_SEQ_FADEOUTWAIT,			// �t�F�[�h�A�E�g�܂�
	CONFIG_SEQ_RELEASE,				// ��ʂ̊J��
};

typedef struct _CONFIG_MENU_DAT{
	u16	msgid;
	u16	ofs;
}CONFIG_MENU_DAT;

typedef struct _CONFIG_MENU{
	u16	num;
	u16	sel;
	STRBUF	*dat[MENU_SEL_MAX];
}CONFIG_MENU;

typedef struct _CONFIG_MAIN_DAT
{
	int	heapID;
	int	seq;
	int	sub_seq;
	int	wipe_f;
	u32	end_f:2;
	u32	line:3;
	u32	side:16;
	u32	wincgx_f:1;
	u32	pad:10;

	GF_BGL_INI	*bgl;
	CONFIG_PARAM	param;
	CONFIG			*save;

	///���b�Z�[�W���\�[�X
	MSGDATA_MANAGER*	pMsg;
	
	void*	pScrBuf;
	NNSG2dScreenData*	pScr01;

	GF_BGL_BMPWIN	win[WIN_MAX];	///<BMP�E�B���h�E�f�[�^
	CONFIG_MENU		menu[MENU_LINE_MAX];

	BMPMENU_WORK* pYesNoWork;	///<BMPYESNO�E�B���h�E�f�[�^
	u32	msg_no;

	CATS_SYS_PTR	pCActSys;	///<�Z���A�N�^�[�V�X�e��
	CATS_RES_PTR	pCActRes;	///<�Z���A�N�^�[���\�[�X
	CATS_ACT_PTR	pCAct;		///<�A�N�g
}CONFIG_MAIN_DAT;

//�v���g�^�C�v�錾�@���[�J��
//================================================================
///�I�[�o�[���C�v���Z�X
PROC_RESULT ConfigProc_Init( PROC *proc,int *seq);
PROC_RESULT ConfigProc_Main( PROC *proc,int *seq);
PROC_RESULT ConfigProc_End( PROC *proc,int *seq);

///VBlank�֘A
static void ConfigVBlank(void * work);
///VRAM�o���N��`
static void ConfigVramBankSet(void);

//================================================================
///�v���g�^�C�v�G���A
//================================================================
static int ConfigInitCommon(CONFIG_MAIN_DAT* wk);
static int ConfigReleaseCommon(CONFIG_MAIN_DAT* wk);

static void ConfigBGLInit(CONFIG_MAIN_DAT* wk);
static void ConfigBGLRelease(CONFIG_MAIN_DAT* wk);
static void Config2DGraInit(CONFIG_MAIN_DAT* wk);
static void Config2DGraRelease(CONFIG_MAIN_DAT* wk);
static void ConfigBmpWinInit(CONFIG_MAIN_DAT* wk);
static void ConfigBmpWinRelease(CONFIG_MAIN_DAT* wk);
static void ConfigBmpWinWriteFirst(CONFIG_MAIN_DAT* wk);
static void ConfigBmpWinWriteMenu(CONFIG_MAIN_DAT* wk,u16 idx);
static void ConfigBmpWinWriteSum( CONFIG_MAIN_DAT* wk,u16 line, BOOL allput );
static void ConfigBmpWinWriteMessege( CONFIG_MAIN_DAT* wk, u16 msg_idx, BOOL allput );
static BOOL ConfigBmpWinWriteMessegeEndCheck( const CONFIG_MAIN_DAT* cp_wk );
static void ConfigMenuStrGet(CONFIG_MAIN_DAT* wk);
static void ConfigKeyIn(CONFIG_MAIN_DAT* wk);
static BOOL ConfigDiffParam( CONFIG_MAIN_DAT* p_wk );
static void ConfigYesNoWinInit( CONFIG_MAIN_DAT* p_wk );
static u32 ConfigYesNoWinMain( CONFIG_MAIN_DAT* p_wk );

//================================================================
///�f�[�^��`�G���A
//================================================================

//�X�N���[���T�C�Y�擾�}�N��
#define SCWS_GET(p)	((p)->screenWidth/8)
#define SCHS_GET(p)	((p)->screenHeight/8)

//--------------------------------------------------
///�v���O�����G���A
//----------------------------------------------------------------
/**
 *	@brief	�R���t�B�O �v���Z�X������
 *	@param	proc	�v���Z�X�f�[�^
 *	@param	seq		�V�[�P���X
 *
 *	@return	������
 */
PROC_RESULT ConfigProc_Init( PROC *proc,int *seq)
{
	CONFIG_MAIN_DAT *wk = NULL;
	CONFIG * sp;
	
	sp = (CONFIG*)PROC_GetParentWork(proc);

	//���[�N�G���A�擾
	HeapStatePush();

	//�q�[�v�쐬
	sys_CreateHeap(HEAPID_BASE_APP,HEAPID_CONFIG,0x10000);
	
	wk = PROC_AllocWork( proc,sizeof(CONFIG_MAIN_DAT),HEAPID_CONFIG);
	memset(wk,0,sizeof(CONFIG_MAIN_DAT));
	
	//�p�����[�^���p��
	wk->param.msg_spd		= CONFIG_GetMsgSpeed(sp);
	wk->param.btl_eff		= CONFIG_GetWazaEffectMode(sp);
	wk->param.btl_rule		= CONFIG_GetBattleRule(sp);
	wk->param.sound			= CONFIG_GetSoundMode(sp);
	wk->param.input_mode	= CONFIG_GetInputMode(sp);
	wk->param.win_type		= CONFIG_GetWindowType(sp);
	
	wk->heapID = HEAPID_CONFIG;
	wk->save = sp;


	// ���b�Z�[�W�\���֌W��ݒ�
	MsgPrintSkipFlagSet(MSG_SKIP_OFF);	// �X�L�b�v�ł��Ȃ�

	return PROC_RES_FINISH;
}

/**
 *	@brief	�R���t�B�O �v���Z�X�I��
 *	@param	proc	�v���Z�X�f�[�^
 *	@param	seq		�V�[�P���X
 *
 *	@return	������
 */
PROC_RESULT ConfigProc_End( PROC *proc,int *seq)
{
	CONFIG_MAIN_DAT* wk = PROC_GetWork(proc);

	//�Z�[�u�p�����[�^�����o��
	if(wk->end_f == CONFIG_DECIDE_END){
		wk->param.msg_spd = wk->menu[MENU_MSG_SPD].sel;
		wk->param.btl_eff = wk->menu[MENU_BTL_EFF].sel;
		wk->param.btl_rule = wk->menu[MENU_BTL_RULE].sel;
		wk->param.sound	 = wk->menu[MENU_SOUND].sel;
		wk->param.input_mode	 = wk->menu[MENU_BTN_MODE].sel;
		wk->param.win_type	 = wk->menu[MENU_WIN].sel;
	}

	// ���݂̐ݒ�𔽉f
	// �R���t�B�O���Őݒ肳�ꂽ�l�����ɖ߂�
	CONFIG_SetMsgSpeed(wk->save,wk->param.msg_spd);
	CONFIG_SetWazaEffectMode(wk->save,wk->param.btl_eff);
	CONFIG_SetBattleRule(wk->save,wk->param.btl_rule);
	CONFIG_SetSoundMode(wk->save,wk->param.sound);
	CONFIG_SetInputMode(wk->save,wk->param.input_mode);
	CONFIG_SetWindowType(wk->save,wk->param.win_type);
	Snd_SetMonoFlag(wk->param.sound);
	CONFIG_SetKeyConfig(wk->param.input_mode);

	// ���b�Z�[�W�\���֌W��ݒ�
	MsgPrintSkipFlagSet(MSG_SKIP_ON);	// �X�L�b�v�ł���

	//���[�N�G���A���
	PROC_FreeWork(proc);

	HeapStatePop();
	HeapStateCheck(wk->heapID);
	sys_DeleteHeap(wk->heapID);

	return PROC_RES_FINISH;
}

/**
 *	@brief	�R���t�B�O �v���Z�X���C��
 *	@param	proc	�v���Z�X�f�[�^
 *	@param	seq		�V�[�P���X
 *
 *	@return	������
 */
PROC_RESULT ConfigProc_Main( PROC *proc,int *seq)
{
	CONFIG_MAIN_DAT* wk = PROC_GetWork(proc);
	BOOL result;
	u32 yesno_res;

	switch(wk->seq){
	case CONFIG_SEQ_INITFADEIN:
		if(!ConfigInitCommon(wk)){
			return PROC_RES_CONTINUE;
		}
		WIPE_SYS_Start(WIPE_PATTERN_M,
				WIPE_TYPE_FADEIN,
				WIPE_TYPE_FADEIN,0x0000,COMM_BRIGHTNESS_SYNC,1,wk->heapID
				);
		break;
	case CONFIG_SEQ_INITFADEINWAIT:
		if(!WIPE_SYS_EndCheck()){
			return PROC_RES_CONTINUE;
		}
		break;
	case CONFIG_SEQ_MAIN:
		//���C��
		if( ((sys.trg & PAD_BUTTON_DECIDE) && (wk->line == (MENU_LINE_MAX-1))) ||
			(sys.trg & PAD_BUTTON_CANCEL) ){

			result = ConfigDiffParam( wk );
			
			// �ύX�����������`�F�b�N
			if( result == TRUE ){
				wk->seq = CONFIG_SEQ_SETCHECKINIT;
			}else{
				Snd_SePlay(SEQ_SE_DP_SELECT);
				wk->end_f = CONFIG_CANCEL_END;
				wk->seq = CONFIG_SEQ_FADEOUT;
			}
			return PROC_RES_CONTINUE;
		}
		ConfigKeyIn(wk);
		return PROC_RES_CONTINUE;
		
	case CONFIG_SEQ_SETCHECKINIT:
		// ���f�����邩����
		MsgPrintSkipFlagSet(MSG_SKIP_ON);	// �X�L�b�v�ł���
		ConfigBmpWinWriteMessege( wk, mes_config_comment07, FALSE );
		break;

	case CONFIG_SEQ_SETCHECK:
		// YESNO���o��
		if( ConfigBmpWinWriteMessegeEndCheck( wk ) ){
			MsgPrintSkipFlagSet(MSG_SKIP_OFF);	// �X�L�b�v�ł���
			ConfigYesNoWinInit( wk );
			wk->seq = CONFIG_SEQ_SETWAIT;
		}
		return PROC_RES_CONTINUE;
		
	case CONFIG_SEQ_SETWAIT:
		// �I�����Ă��炤
		yesno_res = ConfigYesNoWinMain( wk );
		if( yesno_res != BMPMENU_NULL ){
			if( yesno_res == 0 ){	// �Z�[�u����Ȃ�
				Snd_SeStopBySeqNo(SEQ_SE_DP_SELECT,0);		//�����ɖ�̂��������
				Snd_SePlay(SEQ_SE_DP_SAVE);
				wk->end_f = CONFIG_DECIDE_END;
			}else{
				wk->end_f = CONFIG_CANCEL_END;
			}
			wk->seq = CONFIG_SEQ_FADEOUT;
		}
		return PROC_RES_CONTINUE;
	
	case CONFIG_SEQ_FADEOUT:
		//add pl MSG�t���[�Y�΍� ���b�Z�[�W�\���^�X�N������~ 080701 kaga
		if( GF_MSG_PrintEndCheck(wk->msg_no) ){
			GF_STR_PrintForceStop( wk->msg_no );
		}
		
		WIPE_SYS_Start(WIPE_PATTERN_M,
				WIPE_TYPE_FADEOUT,
				WIPE_TYPE_FADEOUT,0x0000,COMM_BRIGHTNESS_SYNC,1,wk->heapID
				);
		break;
	case CONFIG_SEQ_FADEOUTWAIT:
		if(!WIPE_SYS_EndCheck()){
			return PROC_RES_CONTINUE;
		}
		break;
	case CONFIG_SEQ_RELEASE:
		//�I������
		if(!ConfigReleaseCommon(wk)){
			return PROC_RES_CONTINUE;
		}
		return PROC_RES_FINISH;
	}
	++wk->seq;
	return PROC_RES_CONTINUE;
}

/**
 *	@brief	�R���t�B�O���VramBank�Z�b�g
 */
static void ConfigVramBankSet(void)
{
	GF_BGL_DISPVRAM vramSetTable = {
		GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
		GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
		GX_VRAM_OBJ_16_G,				// ���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
		GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
		GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
	};

	GF_Disp_SetBank( &vramSetTable );
}

/**
 *	@brief	�R���t�B�O���VBlank
 */
static void ConfigVBlank(void * work)
{
	CONFIG_MAIN_DAT* wk = work;

	//��b�E�B���h�E�L�����N�^�̍ē]��
	if(wk->wincgx_f){
		TalkWinGraphicSet(wk->bgl,BMPL_SUM_FRM,
			BMPL_TALK_WIN_CGX, BMPL_TALK_WIN_PAL, wk->menu[MENU_WIN].sel, wk->heapID);

		wk->wincgx_f = 0;
	}
	CATS_RenderOamTrans();
	NNS_GfdDoVramTransfer();	//VRam�]���}�l�[�W�����s
	GF_BGL_VBlankFunc( wk->bgl );
	OS_SetIrqCheckFlag( OS_IE_V_BLANK);
}

/**
 *	@brief	�e�평��������
 */
static int ConfigInitCommon(CONFIG_MAIN_DAT *wk)
{
	switch(wk->sub_seq){
	case 0:
		sys_VBlankFuncChange(NULL,NULL);	
		sys_HBlankIntrStop();
	
		GF_Disp_GX_VisibleControlInit();
		GF_Disp_GXS_VisibleControlInit();
		GX_SetVisiblePlane(0);
		GXS_SetVisiblePlane(0);

		//Bank�Z�b�g
		ConfigVramBankSet();

		WIPE_ResetWndMask(WIPE_DISP_MAIN);
		WIPE_ResetWndMask(WIPE_DISP_SUB);

		//BGL�Z�b�g
		ConfigBGLInit(wk);
		break;
	case 1:
		//2D���\�[�X�擾
		Config2DGraInit(wk);

		//���b�Z�[�W���\�[�X�擾
		wk->pMsg = MSGMAN_Create(MSGMAN_TYPE_DIRECT,ARC_MSG,
				NARC_msg_config_dat,wk->heapID);
		ConfigMenuStrGet(wk);
		break;
	case 2:
		ConfigBmpWinInit(wk);
		ConfigBmpWinWriteFirst(wk);
		//�ʐM�A�C�R���p��OBJ��ON
		initVramTransferManagerHeap(32,wk->heapID);
		GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ,VISIBLE_ON);

		// ��M���x�A�C�R����ʐM�ڑ����Ȃ�\�����邵�A
		// ���j�I���̂悤�ɒʐM��H�͓��삵�Ă��邪�ڑ��͂��Ă��Ȃ���ԂȂ�o���Ȃ�
		// Change by Mori 2006/07/19
		WirelessIconEasyUnion();

		sys_VBlankFuncChange(ConfigVBlank,wk);	
		wk->sub_seq = 0;
		return 1;
		
	}
	++wk->sub_seq;
	return 0;
}

/**
 *	@brief	�e��I������
 */
static int ConfigReleaseCommon(CONFIG_MAIN_DAT *wk)
{
	int i = 0,j = 0;
	
	switch(wk->sub_seq){
	case 0:
		DellVramTransferManager();
		//Bmp�E�B���h�E�j��
		ConfigBmpWinRelease(wk);
		//���b�Z�[�W���\�[�X���
		for(i = 0;i < MENU_LINE_MAX;i++){
			for(j = 0;j < wk->menu[i].num;j++){
				STRBUF_Delete(wk->menu[i].dat[j]);
			}
		}
		MSGMAN_Delete(wk->pMsg);
		//2D���\�[�X�擾
		Config2DGraRelease(wk);
		ConfigBGLRelease(wk);
		break;
	case 1:
		sys_VBlankFuncChange(NULL,NULL);	
		sys_HBlankIntrStop();
	
		GF_Disp_GX_VisibleControlInit();
		GF_Disp_GXS_VisibleControlInit();
		GX_SetVisiblePlane(0);
		GXS_SetVisiblePlane(0);
		wk->sub_seq = 0;
		return 1;
	}
	++wk->sub_seq;
	return 0;
}

/**
 *	@brief	�R���t�B�O���BGL�V�X�e��������
 */
static void ConfigBGLInit(CONFIG_MAIN_DAT* wk)
{
	int i;

	wk->bgl = GF_BGL_BglIniAlloc(wk->heapID);

	{	//BG SYSTEM
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS,GX_BGMODE_0,GX_BGMODE_0,GX_BG0_AS_2D
		};
		GF_BGL_InitBG(&BGsys_data);
	}
	
	{
	GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
		{	//MAIN BG0
			0,0,0x800,0,GF_BGL_SCRSIZ_256x256,GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf800,GX_BG_CHARBASE_0x00000,GX_BG_EXTPLTT_01,
			1,0,0,FALSE},
		{	//MAIN BG1
			0,0,0x800,0,GF_BGL_SCRSIZ_256x256,GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf000,GX_BG_CHARBASE_0x00000,GX_BG_EXTPLTT_01,
			2,0,0,FALSE},
		{	//MAIN BG2
			0,0,0x800,0,GF_BGL_SCRSIZ_256x256,GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe800,GX_BG_CHARBASE_0x00000,GX_BG_EXTPLTT_01,
			3,0,0,FALSE},
		{	//MAIN BG3
			0,0,0x800,0,GF_BGL_SCRSIZ_256x256,GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xe000,GX_BG_CHARBASE_0x00000,GX_BG_EXTPLTT_01,
			0,0,0,FALSE},
		{	//SUB BG0
			0,0,0x800,0,GF_BGL_SCRSIZ_256x256,GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xf800,GX_BG_CHARBASE_0x00000,GX_BG_EXTPLTT_01,
			0,0,0,FALSE},
	};
	for(i = 0;i < 5;i++){
		static const frame[5] = {
			GF_BGL_FRAME0_M,GF_BGL_FRAME1_M,GF_BGL_FRAME2_M,GF_BGL_FRAME3_M,GF_BGL_FRAME0_S
		};
		GF_BGL_BGControlSet(wk->bgl,frame[i],&(TextBgCntDat[i]),GF_BGL_MODE_TEXT);
		GF_BGL_ScrClear(wk->bgl,frame[i]);
	}
	}
	GF_BGL_ClearCharSet(GF_BGL_FRAME0_M,32,0,wk->heapID);
	GF_BGL_ClearCharSet(GF_BGL_FRAME0_S,32,0,wk->heapID);
}

/**
 *	@brief	�R���t�B�O���BGL�V�X�e���j��
 */
static void ConfigBGLRelease(CONFIG_MAIN_DAT* wk)
{
	GF_BGL_BGControlExit(wk->bgl,GF_BGL_FRAME0_S);
	GF_BGL_BGControlExit(wk->bgl,GF_BGL_FRAME3_M);
	GF_BGL_BGControlExit(wk->bgl,GF_BGL_FRAME2_M);
	GF_BGL_BGControlExit(wk->bgl,GF_BGL_FRAME1_M);
	GF_BGL_BGControlExit(wk->bgl,GF_BGL_FRAME0_M);

	sys_FreeMemoryEz(wk->bgl);
}

/**
 *	@brief	�R���t�B�O��� 2D���\�[�X�擾
 */
static void Config2DGraInit(CONFIG_MAIN_DAT* wk)
{
	void* tmp;
	u32	size;
	ARCHANDLE* handle;
	void* pSrc;
	NNSG2dCharacterData* pChar;
	NNSG2dPaletteData*	pPal;
	
	handle = ArchiveDataHandleOpen(ARC_CONFIG_GRA,wk->heapID);

	//�L�����N�^�]��
	size = ArchiveDataSizeGetByHandle(handle,NARC_config_gra_config01_ncgr);
	pSrc = sys_AllocMemoryLo(wk->heapID,size);
	ArchiveDataLoadByHandle(handle,NARC_config_gra_config01_ncgr,(void*)pSrc);
	
	NNS_G2dGetUnpackedCharacterData(pSrc,&pChar); 
	GF_BGL_LoadCharacter(wk->bgl,GF_BGL_FRAME0_M,
			pChar->pRawData,pChar->szByte,0);
	GF_BGL_LoadCharacter(wk->bgl,GF_BGL_FRAME0_S,
			pChar->pRawData,pChar->szByte,0);
	sys_FreeMemoryEz(pSrc);

	//�p���b�g�]��
	size = ArchiveDataSizeGetByHandle(handle,NARC_config_gra_config01_nclr);
	pSrc = sys_AllocMemoryLo(wk->heapID,size);
	ArchiveDataLoadByHandle(handle,NARC_config_gra_config01_nclr,(void*)pSrc);

	NNS_G2dGetUnpackedPaletteData(pSrc,&pPal);
	GF_BGL_PaletteSet(GF_BGL_FRAME0_M,pPal->pRawData,0x20*1,0);
	GF_BGL_PaletteSet(GF_BGL_FRAME0_S,pPal->pRawData,0x20*1,0);
	sys_FreeMemoryEz(pSrc);

	//�X�N���[���擾
	size = ArchiveDataSizeGetByHandle(handle,NARC_config_gra_config01_nscr);
	wk->pScrBuf = sys_AllocMemory(wk->heapID,size);
	ArchiveDataLoadByHandle(handle,NARC_config_gra_config01_nscr,(void*)wk->pScrBuf);
	NNS_G2dGetUnpackedScreenData(wk->pScrBuf,&(wk->pScr01)); 

	ArchiveDataHandleClose( handle );
	
	GF_BGL_ScrFill(wk->bgl,GF_BGL_FRAME2_M,0x0001,0,0,32,32,GF_BGL_SCRWRT_PALIN );
	GF_BGL_ScrFill(wk->bgl,GF_BGL_FRAME0_S,0x0001,0,0,32,32,GF_BGL_SCRWRT_PALIN );
	
	//BG�`��
	GF_BGL_ScrWriteFree(wk->bgl,GF_BGL_FRAME0_M,
			0,0,32,2,
			wk->pScr01->rawData,
			0,0,
			wk->pScr01->screenWidth/8,wk->pScr01->screenHeight/8);
	GF_BGL_ScrollSet(wk->bgl,GF_BGL_FRAME0_M,GF_BGL_SCROLL_Y_SET,-MENU_WIN_OFSY);
	
	GF_BGL_LoadScreenV_Req(wk->bgl,GF_BGL_FRAME2_M);
	GF_BGL_LoadScreenV_Req(wk->bgl,GF_BGL_FRAME0_M);
	GF_BGL_LoadScreenV_Req(wk->bgl,GF_BGL_FRAME0_S);
}

/**
 *	@brief	�R���t�B�O��� 2D���\�[�X���
 */
static void Config2DGraRelease(CONFIG_MAIN_DAT* wk)
{
	sys_FreeMemoryEz(wk->pScrBuf);
}

/**
 *	@brief	�R���t�B�O��� BMP�E�B���h�E������
 */
static void ConfigBmpWinInit(CONFIG_MAIN_DAT* wk)
{
	GF_BGL_BmpWinAdd(
		wk->bgl, &wk->win[WIN_TITLE], BMPL_TITLE_FRM,
		BMPL_TITLE_PX, BMPL_TITLE_PY,
		BMPL_TITLE_SX, BMPL_TITLE_SY, BMPL_TITLE_PAL, BMPL_TITLE_CGX );

	GF_BGL_BmpWinAdd(
		wk->bgl, &wk->win[WIN_MENU], BMPL_MENU_FRM,
		BMPL_MENU_PX, BMPL_MENU_PY,
		BMPL_MENU_SX, BMPL_MENU_SY, BMPL_MENU_PAL, BMPL_MENU_CGX );
	
	GF_BGL_BmpWinAdd(
		wk->bgl, &wk->win[WIN_SUM], BMPL_SUM_FRM,
		BMPL_SUM_PX, BMPL_SUM_PY,
		BMPL_SUM_SX, BMPL_SUM_SY, BMPL_SUM_PAL, BMPL_SUM_CGX );

	//�E�B���h�E�O���t�B�b�N�Z�b�g
	MenuWinGraphicSet(wk->bgl, BMPL_MENU_FRM,
			BMPL_MENU_WIN_CGX, BMPL_MENU_WIN_PAL, MENU_TYPE_SYSTEM, wk->heapID);

	TalkWinGraphicSet(wk->bgl,BMPL_SUM_FRM,
			BMPL_TALK_WIN_CGX, BMPL_TALK_WIN_PAL, wk->param.win_type, wk->heapID);
	
	//�t�H���g�p�p���b�g�Z�b�g
	SystemFontPaletteLoad(PALTYPE_MAIN_BG,FONT_SYS_PAL*32,wk->heapID);
	SystemFontPaletteLoad(PALTYPE_SUB_BG,FONT_SYS_PAL*32,wk->heapID);
	TalkFontPaletteLoad(PALTYPE_MAIN_BG,FONT_TALK_PAL*32,wk->heapID);
	TalkFontPaletteLoad(PALTYPE_SUB_BG,FONT_TALK_PAL*32,wk->heapID);


	//BMP�E�B���h�E�N���A
	GF_BGL_BmpWinDataFill(&(wk->win[WIN_TITLE]),WINCLR_COL(FBMP_COL_NULL));
	GF_BGL_BmpWinDataFill(&(wk->win[WIN_MENU]),WINCLR_COL(FBMP_COL_WHITE));
	GF_BGL_BmpWinDataFill(&(wk->win[WIN_SUM]),WINCLR_COL(FBMP_COL_WHITE));
	GF_BGL_BmpWinClearScrn(&(wk->win[WIN_SUM]));
	GF_BGL_BmpWinClearScrn(&(wk->win[WIN_MENU]));
	GF_BGL_BmpWinClearScrn(&(wk->win[WIN_TITLE]));
	
	//�E�B���h�E�`��
	BmpMenuWinWrite( &wk->win[WIN_MENU], WINDOW_TRANS_OFF, BMPL_MENU_WIN_CGX, BMPL_MENU_WIN_PAL );
	BmpTalkWinWrite( &wk->win[WIN_SUM], WINDOW_TRANS_OFF, BMPL_TALK_WIN_CGX, BMPL_TALK_WIN_PAL);
}

/**
 *	@brief	�R���t�B�O��� BMP�E�B���h�E���
 */
static void ConfigBmpWinRelease(CONFIG_MAIN_DAT* wk)
{
	u16	i;
	
	//�E�B���h�E�N���A
	BmpMenuWinClear(&(wk->win[WIN_MENU]),WINDOW_TRANS_ON);
	BmpTalkWinClear(&(wk->win[WIN_SUM]),WINDOW_TRANS_ON);

	for(i = 0;i < WIN_MAX;i++){
		GF_BGL_BmpWinOff(&(wk->win[i]));
		GF_BGL_BmpWinDataFill(&(wk->win[i]),0);
		GF_BGL_BmpWinClearScrn(&(wk->win[i]));
		GF_BGL_BmpWinDel(&wk->win[i]);
	}
}

/**
 *	@brief	�R���t�B�O��� BMP�E�B���h�E�����`��
 */
static void ConfigBmpWinWriteFirst(CONFIG_MAIN_DAT* wk)
{
	u32	siz,ofs;
	u16	i;
	GF_PRINTCOLOR fcol,rcol,tcol;	
	STRBUF	*buf;
	STRBUF	*sum;
	static const u8 menu_str[ MENU_LINE_MAX ] = {
		mes_config_menu01,
		mes_config_menu04,
		mes_config_menu02,
		mes_config_menu03,
		mes_config_menu05,
		mes_config_menu06,
		mes_config_menu08,
	};
	
	tcol = GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL);
	fcol = GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE);
	rcol = GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_WHITE);

	buf = STRBUF_Create(SUM_STRSIZ,wk->heapID);

	//�����Ă���������
	MSGMAN_GetString(wk->pMsg,mes_config_title,buf);

	ofs = 2;
	GF_STR_PrintColor(	&wk->win[WIN_TITLE],FONT_SYSTEM,buf,
						ofs,2,
						MSG_ALLPUT,tcol,NULL );
	
	//���j���[
	ofs = 4;
	for(i = 0;i < MENU_LINE_MAX;i++){
		STRBUF_Clear(buf);
		MSGMAN_GetString(wk->pMsg,menu_str[i],buf);
	
		GF_STR_PrintColor(	&wk->win[WIN_MENU],FONT_SYSTEM,buf,
						ofs,16*i,
						MSG_NO_PUT,fcol,NULL );
	}
	for(i = 0;i < MENU_LINE_MAX;i++){
		ConfigBmpWinWriteMenu(wk,i);
	}

	//�T���l�C��
	ConfigBmpWinWriteSum( wk, 0, TRUE );
	
	GF_BGL_BmpWinOn(&wk->win[WIN_TITLE]);
	GF_BGL_BmpWinOn(&wk->win[WIN_MENU]);

	STRBUF_Delete(buf);
}

/**
 *	@brief	�R���t�B�O��� �Z���N�g���j���[������擾
 */
static void ConfigMenuStrGet(CONFIG_MAIN_DAT* wk)
{
	u16	i,j;
	static const selSiz[MENU_LINE_MAX] = {
		MSGSPEED_MAX,
		SOUNDMODE_MAX,
		WAZAEFF_MODE_MAX,
		BATTLERULE_MAX,
		INPUTMODE_MAX,
		WINTYPE_MAX,
		0
	};
	static const u8 sc_selMsg_start[MENU_LINE_MAX] = {
		mes_config_sm01_00,
		mes_config_sm04_00,
		mes_config_sm02_00,
		mes_config_sm03_00,
		mes_config_sm05_00,
		mes_config_sm06_00,
		0,
	};

	for(i = 0;i < MENU_LINE_MAX;i++){
		wk->menu[i].num = selSiz[i];
		for(j = 0;j < selSiz[i];j++){
			wk->menu[i].dat[j] = MSGMAN_AllocString(wk->pMsg,sc_selMsg_start[i]+j);
		}
	}
	//���݂̑I���i���o�[�擾
	wk->menu[MENU_MSG_SPD].sel = wk->param.msg_spd;
	wk->menu[MENU_BTL_EFF].sel = wk->param.btl_eff;
	wk->menu[MENU_BTL_RULE].sel = wk->param.btl_rule;
	wk->menu[MENU_SOUND].sel = wk->param.sound;
	wk->menu[MENU_BTN_MODE].sel = wk->param.input_mode;
	wk->menu[MENU_WIN].sel = wk->param.win_type;
}

/**
 *	@brief	�R���t�B�O��� �Z���N�g���j���[�`��
 */
static void ConfigBmpWinWriteMenu(CONFIG_MAIN_DAT* wk,u16 idx)
{
	GF_PRINTCOLOR fcol,rcol,scol;	
	u16	i;
	u8	wait;
	s8	x_ofs = 0;
	static const s8 ofs[] = { 0,0,0,0,0,0,0};

	fcol = GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE);
	rcol = GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_WHITE);
	
	//��U�N���A
	GF_BGL_BmpWinFill( &(wk->win[WIN_MENU]),
		WINCLR_COL(FBMP_COL_WHITE),
		MENU_SEL_OFSX+ofs[idx], MENU_SEL_OFSY+idx*MENU_SEL_ASIZY,MENU_SEL_ASIZX,MENU_SEL_ASIZY);

	//�����Ă���������
	if(idx == MENU_WIN){
		GF_STR_PrintColor(	&wk->win[WIN_MENU],FONT_SYSTEM,
						wk->menu[idx].dat[wk->menu[idx].sel],
						1*MENU_SEL_DSIZ+MENU_SEL_OFSX,
						MENU_SEL_ASIZY*idx+MENU_SEL_OFSY,
						MSG_NO_PUT,rcol,NULL );
		GF_BGL_BmpWinOn(&wk->win[WIN_MENU]);

		//�E�B���h�E�L�����N�^�đ��t���OOn
		wk->wincgx_f = 1;
		return;
	}

	//�T�E���h���[�h�̏ꍇ�A�X�e���I�ƃ��m������؂�ւ���
	if(idx == MENU_SOUND){
		Snd_SetMonoFlag(wk->menu[idx].sel);
	}
	// �{�^�����[�h���f
	else if(idx == MENU_BTN_MODE){
		CONFIG_SetKeyConfig(wk->menu[idx].sel);
	}
	// ���b�Z�[�W�X�s�[�h���f
	else if(idx == MENU_MSG_SPD ){
		CONFIG_SetMsgSpeed( wk->save, wk->menu[idx].sel );

		//�T���l�C�����ĕ`��
		ConfigBmpWinWriteSum( wk, idx, FALSE );
	}
	x_ofs = 0;
	for(i = 0;i < wk->menu[idx].num;i++){
		if(i == wk->menu[idx].sel){
			scol = rcol;
		}else{
			scol = fcol;
		}
		if(i == wk->menu[idx].num-1){
			wait = MSG_NO_PUT;
		}else{
			wait = MSG_NO_PUT;
		}
#if 1
		if(idx == MENU_BTN_MODE){
			GF_STR_PrintColor(	&wk->win[WIN_MENU],FONT_SYSTEM,wk->menu[idx].dat[i],
							MENU_SEL_OFSX-0+x_ofs,
							MENU_SEL_ASIZY*idx+MENU_SEL_OFSY,
							wait,scol,NULL );
			
			x_ofs += FontProc_GetPrintStrWidth(FONT_SYSTEM,wk->menu[idx].dat[i],0) + 12;
		}else{
#endif
			GF_STR_PrintColor(	&wk->win[WIN_MENU],FONT_SYSTEM,wk->menu[idx].dat[i],
							i*MENU_SEL_DSIZ+MENU_SEL_OFSX+ofs[idx],
							MENU_SEL_ASIZY*idx+MENU_SEL_OFSY,
							wait,scol,NULL );
		}
	}
	GF_BGL_BmpWinOn(&wk->win[WIN_MENU]);
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�̕\��
 *
 *	@param	wk		���[�N
 *	@param	msg_idx	���b�Z�[�WID
 *	@param	allput	��C�ɕ\�����邩
 */
//-----------------------------------------------------------------------------
static void ConfigBmpWinWriteMessege( CONFIG_MAIN_DAT* wk, u16 msg_idx, BOOL allput )
{
	GF_PRINTCOLOR fcol;	
	STRBUF	*buf;
	u8 msg_speed;

	// �b�r���̂Ƃ��͍폜����
	if( ConfigBmpWinWriteMessegeEndCheck( wk ) == FALSE ){
		// �����I��
		GF_STR_PrintForceStop( wk->msg_no );
	}

	// ���݂̃��b�Z�[�W�X�s�[�h�ݒ�
	msg_speed = CONFIG_GetMsgPrintSpeed( wk->save );

	// BMP�̃N���[��
	GF_BGL_BmpWinDataFill( &(wk->win[WIN_SUM]), FBMP_COL_WHITE);

	// �J���[�쐬
	fcol = GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE);

	// STRBUF�쐬
	buf = STRBUF_Create(SUM_STRSIZ,wk->heapID);

	// �ύX�������b�Z�[�W�X�s�[�h�ŏ�������
	MSGMAN_GetString(wk->pMsg,msg_idx,buf);

	if( allput == FALSE ){
		wk->msg_no = GF_STR_PrintColor(	&wk->win[WIN_SUM],FONT_TALK,buf,
							4,0,
							msg_speed,fcol,NULL );
	}else{
		GF_STR_PrintColor(	&wk->win[WIN_SUM],FONT_TALK,buf,
							4,0,
							MSG_NO_PUT,fcol,NULL );
		GF_BGL_BmpWinOnVReq( &wk->win[WIN_SUM] );
	}

	// STRBUF�j��
	STRBUF_Delete( buf );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�I���҂�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	���b�Z�[�W�I��
 *	@retval	FALSE	���b�Z�[�W�r��
 */
//-----------------------------------------------------------------------------
static BOOL ConfigBmpWinWriteMessegeEndCheck( const CONFIG_MAIN_DAT* cp_wk )
{
	if( GF_MSG_PrintEndCheck( cp_wk->msg_no ) == FALSE ){
		return TRUE;
	}
	return FALSE;
}

/**
 *	@brief	�ݒ��ʃL�[�擾
 */
static void ConfigKeyIn(CONFIG_MAIN_DAT* wk)
{
	CONFIG_MENU *mp;
	
	mp = &(wk->menu[wk->line]);
	if(wk->line != MENU_MAX){	//���胉�C���̎��͍��E�L�[���͖���
		if(sys.trg & PAD_KEY_RIGHT){
			mp->sel = (mp->sel+1)%mp->num;	
			ConfigBmpWinWriteMenu(wk,wk->line);
			Snd_SePlay(SEQ_SE_DP_SELECT);
		}else if(sys.trg & PAD_KEY_LEFT){
			mp->sel = (mp->sel+mp->num-1)%mp->num;	
			ConfigBmpWinWriteMenu(wk,wk->line);
			Snd_SePlay(SEQ_SE_DP_SELECT);
		}
	}
	if(sys.trg & PAD_KEY_UP){
		wk->line = (wk->line+MENU_LINE_MAX-1)%MENU_LINE_MAX;
		GF_BGL_ScrollReq(wk->bgl,GF_BGL_FRAME0_M,GF_BGL_SCROLL_Y_SET,
				-(wk->line*MENU_SEL_ASIZY+MENU_WIN_OFSY));

		//�T���l�C��
		ConfigBmpWinWriteSum( wk, wk->line, TRUE );
		Snd_SePlay(SEQ_SE_DP_SELECT);
	}else if(sys.trg & PAD_KEY_DOWN){
		wk->line = (wk->line+1)%MENU_LINE_MAX;
		GF_BGL_ScrollReq(wk->bgl,GF_BGL_FRAME0_M,GF_BGL_SCROLL_Y_SET,
				-(wk->line*MENU_SEL_ASIZY+MENU_WIN_OFSY));

		//�T���l�C��
		ConfigBmpWinWriteSum( wk, wk->line, TRUE );
		Snd_SePlay(SEQ_SE_DP_SELECT);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ݒ�p�����[�^�ɕύX�����邩�`�F�b�N
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�ݒ�p�����[�^�ɕύX����
 *	@retval	FALSE	�ݒ�p�����[�^�ɕύX�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL ConfigDiffParam( CONFIG_MAIN_DAT* p_wk )
{
	if( (p_wk->param.msg_spd	!=	p_wk->menu[MENU_MSG_SPD].sel) ||
		(p_wk->param.btl_eff	!=	p_wk->menu[MENU_BTL_EFF].sel) ||
		(p_wk->param.btl_rule	!=	p_wk->menu[MENU_BTL_RULE].sel) ||
		(p_wk->param.sound		!=	p_wk->menu[MENU_SOUND].sel) ||
		(p_wk->param.input_mode	!=	p_wk->menu[MENU_BTN_MODE].sel) ||
		(p_wk->param.win_type	!=	p_wk->menu[MENU_WIN].sel) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	YESNO�E�B���h�E�̏�����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void ConfigYesNoWinInit( CONFIG_MAIN_DAT* p_wk )
{
	static const BMPWIN_DAT c_data = {
		BMPL_YESNO_FRM,
		BMPL_YESNO_CX, BMPL_YESNO_CY,
		BMPL_YESNO_CSX, BMPL_YESNO_CSY,
		BMPL_YESNO_PAL, BMPL_YESNO_CGX
	};
	p_wk->pYesNoWork = BmpYesNoSelectInit( p_wk->bgl, &c_data, 
			BMPL_MENU_WIN_CGX, BMPL_MENU_WIN_PAL, p_wk->heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YESNO�E�B���h�E���C��
 *
 *	@param	p_wk	���[�N
 *
 * @retval	"BMPMENU_NULL	�I������Ă��Ȃ�"
 * @retval	"0				�͂���I��"
 * @retval	"BMPMENU_CANCEL	������or�L�����Z��"
 */
//-----------------------------------------------------------------------------
static u32 ConfigYesNoWinMain( CONFIG_MAIN_DAT* p_wk )
{
	return BmpYesNoSelectMain( p_wk->pYesNoWork, p_wk->heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T���l�C���`��
 *
 *	@param	wk		���[�N
 *	@param	line	���C��
 *	@param	allput	��C�ɂЂ傤�����邩
 */
//-----------------------------------------------------------------------------
static void ConfigBmpWinWriteSum( CONFIG_MAIN_DAT* wk,u16 line, BOOL allput )
{
	static const u8 line_msg[ MENU_LINE_MAX ] = {
		mes_config_comment01,
		mes_config_comment04,
		mes_config_comment02,
		mes_config_comment03,
		mes_config_comment05,
		mes_config_comment06,
		mes_config_comment10,
	};
	
	//�T���l�C��
	ConfigBmpWinWriteMessege( wk, line_msg[line], allput );
}
