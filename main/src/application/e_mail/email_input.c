//==============================================================================
/**
 * @file	email_input.c
 * @brief	�ȒP�Ȑ���������
 * @author	matsuda
 * @date	2007.10.19(��)
 */
//==============================================================================
#include "common.h"
#include <dwc.h>
#include "libdpw/dpw_tr.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/wipe.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "system/lib_pack.h"
#include "system/fontoam.h"
#include "system/window.h"
#include "gflib/touchpanel.h"
#include "system/bmp_menu.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "savedata/wifilist.h"
#include "savedata/zukanwork.h"
#include "communication/communication.h"


//#include "application/worldtrade.h"
//#include "worldtrade_local.h"

#include "msgdata/msg_wifi_lobby.h"
#include "msgdata/msg_wifi_gtc.h"
#include "msgdata/msg_wifi_system.h"

#include "application/email_main.h"
#include "msgdata/msg_email.h"
#include "system/bmp_list.h"
#include "system/pmfprint.h"
#include "savedata/config.h"
#include "savedata\system_data.h"

#include "../wifi_p2pmatch/wifip2pmatch.naix"			// �O���t�B�b�N�A�[�J�C�u��`


//==============================================================================
//	�萔��`
//==============================================================================
enum{
	FUNCSEQ_CONTINUE,		///<�����p����
	FUNCSEQ_FINISH,			///<���͊���
	FUNCSEQ_CANCEL,			///<�L�����Z�����ďI�����ꂽ
};

#define INPUT_KETA_MAX		(7)

// ��b�E�C���h�E������o�b�t�@��
#define TALK_MESSAGE_BUF_NUM	( 90*2 )

#define EMAIL_MESFRAME_PAL	 ( 10 )
#define EMAIL_MENUFRAME_PAL ( 11 )
#define EMAIL_MESFRAME_CHR	 (  1 )
#define EMAIL_MENUFRAME_CHR ( EMAIL_MESFRAME_CHR + TALK_WIN_CGX_SIZ )
#define EMAIL_TALKFONT_PAL	 ( 13 )

//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct{
	EMAIL_SYSWORK *esys;
	int num_code;
	int number[INPUT_KETA_MAX];
	int num_pos;
	int keta_max;
	int num_msgid;
	
	GF_BGL_INI		*bgl;								// GF_BGL_INI
	WORDSET			*WordSet;							// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	MSGDATA_MANAGER *MsgManager;						// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[
	STRBUF			*TalkString;						// ��b���b�Z�[�W�p
	STRBUF			*ErrorString;
	int				MsgIndex;							// �I�����o�p���[�N
	
	// BMPWIN�`�����
	GF_BGL_BMPWIN			MsgWin;					// ��b�E�C���h�E
	GF_BGL_BMPWIN			SubWin;					// �u���R�[�h�R�[�i�[�@�ڂ��イ���イ�I�v�Ȃ�
}EMAIL_INPUT_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
PROC_RESULT EmailInput_Init( PROC * proc, int * seq );
PROC_RESULT EmailInput_Main( PROC * proc, int * seq );
PROC_RESULT EmailInput_End(PROC *proc, int *seq);
static void VramBankSet(GF_BGL_INI *bgl);
static void BgExit( GF_BGL_INI * ini );
static void BgGraphicSet( EMAIL_INPUT_WORK * wk );
static void BmpWinInit( EMAIL_INPUT_WORK *wk );
static void BmpWinDelete( EMAIL_INPUT_WORK *wk );
static void Input_MessagePrint( EMAIL_INPUT_WORK *wk, MSGDATA_MANAGER *msgman, int msgno, int wait, u16 dat );
static int NumInputFunc(EMAIL_INPUT_WORK *wk);




//============================================================================================
//	�v���Z�X�֐�
//============================================================================================
//==============================================================================
/**
 * $brief   ���E�����������ʏ�����
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailInput_Init( PROC * proc, int * seq )
{
	EMAIL_INPUT_WORK *wk;

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

	//E���[����ʗp�q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_EMAIL, 0x70000 );
	
	wk = PROC_AllocWork(proc, sizeof(EMAIL_INPUT_WORK), HEAPID_EMAIL );
	MI_CpuClear8(wk, sizeof(EMAIL_INPUT_WORK));

	wk->esys = PROC_GetParentWork(proc);

	wk->bgl = GF_BGL_BglIniAlloc( HEAPID_EMAIL );

	initVramTransferManagerHeap(64, HEAPID_EMAIL);

	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	//VRAM���蓖�Đݒ�
	VramBankSet(wk->bgl);

	// �^�b�`�p�l���V�X�e��������
	InitTPSystem();
	InitTPNoBuff(4);
	
	//���b�Z�[�W�}�l�[�W���쐬
	wk->WordSet    		 = WORDSET_Create( HEAPID_EMAIL );
	wk->MsgManager       = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_email_dat, HEAPID_EMAIL );

	// ������o�b�t�@�쐬
	wk->ErrorString  = STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_EMAIL );
	wk->TalkString  = STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_EMAIL );

	// BG�O���t�B�b�N�]��
	BgGraphicSet( wk );

	// BMPWIN�m��
	BmpWinInit( wk );

	// Wifi�ʐM�A�C�R��
    WirelessIconEasy();

	// ���C�v�t�F�[�h�J�n
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK, 
		WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_EMAIL );

	// BG�ʕ\��ON
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );

	MsgPrintSkipFlagSet(MSG_SKIP_ON);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

//	sys_VBlankFuncChange(ActinVBlank, apw);

	return PROC_RES_FINISH;
}


//==============================================================================
/**
 * $brief   ���E�����������ʃ��C��
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailInput_Main( PROC * proc, int * seq )
{
	EMAIL_INPUT_WORK * wk  = PROC_GetWork( proc );
	int ret;
	enum{
		SEQ_INIT,
		SEQ_MAIN,
		SEQ_END,
	};
	
	switch(*seq){
	case SEQ_INIT:
		if(WIPE_SYS_EndCheck() == TRUE){
			switch(wk->esys->sub_process_mode){
			case EMAIL_MODE_INPUT_AUTHENTICATE:
				wk->keta_max = 4;
				wk->num_msgid = msg_email_200;
				break;
			case EMAIL_MODE_INPUT_PASSWORD:
			default:
				wk->keta_max = 4;
				wk->num_msgid = msg_email_201;
				break;
			}
			WORDSET_RegisterNumber(wk->WordSet, 1, Email_AuthenticateRandCodeGet(wk->esys), 3, 
				NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);
			Input_MessagePrint(wk, wk->MsgManager, wk->num_msgid, MSG_ALLPUT, 0);
			*seq = SEQ_MAIN;
		}
		break;
	case SEQ_MAIN:
		// �V�[�P���X�J�ڂŎ��s
		ret = NumInputFunc(wk);
		switch(ret){
		case FUNCSEQ_CONTINUE:
			break;
		case FUNCSEQ_FINISH:
			{
				u32 code;
				
				code = Email_AuthenticateRandCodeGet(wk->esys) * 10000 + wk->num_code;
				Email_AuthenticateCodeSet(wk->esys, code);
				OS_TPrintf("�ŏIcode = %d\n", code);
			}
			*seq= SEQ_END;
			break;
		case FUNCSEQ_CANCEL:
			Email_AuthenticateCodeSet(wk->esys, EMAIL_AUTHENTICATE_CODE_CANCEL);
			OS_TPrintf("�L�����Z���I��\n");
			*seq= SEQ_END;
			break;
		}
		break;
	case SEQ_END:
		return PROC_RES_FINISH;
	}
	return PROC_RES_CONTINUE;
}


//==============================================================================
/**
 * $brief   ���E�����������ʏI��
 *
 * @param   wk		
 * @param   seq		
 *
 * @retval  int		
 */
//==============================================================================
PROC_RESULT EmailInput_End(PROC *proc, int *seq)
{
	EMAIL_INPUT_WORK * wk  = PROC_GetWork( proc );


	//�T�uPROC�����X�g�ɖ߂����߂ɃZ�b�g
	Email_SubProcessChange( wk->esys, EMAIL_SUBPROC_MENU, 0 );


	// ���b�Z�[�W�}�l�[�W���[�E���[�h�Z�b�g�}�l�[�W���[���
	MSGMAN_Delete( wk->MsgManager );
	WORDSET_Delete( wk->WordSet );

	STRBUF_Delete( wk->ErrorString ); 
	STRBUF_Delete( wk->TalkString ); 
	
	BmpWinDelete( wk );
	
	// BG_SYSTEM���
	sys_FreeMemoryEz( wk->bgl );
	BgExit( wk->bgl );

	sys_VBlankFuncChange( NULL, NULL );		// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	//Vram�]���}�l�[�W���[�폜
	DellVramTransferManager();

	StopTP();		//�^�b�`�p�l���̏I��

	MsgPrintSkipFlagSet(MSG_SKIP_OFF);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);

	WirelessIconEasyEnd();

	PROC_FreeWork( proc );				// PROC���[�N�J��
	sys_DeleteHeap( HEAPID_EMAIL );

	return PROC_RES_FINISH;
}


//--------------------------------------------------------------
/**
 * @brief   Vram�o���N�ݒ���s��
 *
 * @param   bgl		BGL�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void VramBankSet(GF_BGL_INI *bgl)
{
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	
	//VRAM�ݒ�
	{
		GF_BGL_DISPVRAM vramSetTable = {
			GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
			GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

			GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
			GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

			GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
			GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

			GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
			GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

			GX_VRAM_TEX_0_B,				// �e�N�X�`���C���[�W�X���b�g
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
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	//���C����ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME0_M	�e�L�X�g��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME0_M, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME0_M );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_M, GF_BGL_SCROLL_Y_SET, 0);
	}
	//�T�u��ʃt���[���ݒ�
	{
		GF_BGL_BGCNT_HEADER TextBgCntDat[] = {
			///<FRAME0_S	�e�L�X�g��
			{
				0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
				GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
				0, 0, 0, FALSE
			},
		};
		GF_BGL_BGControlSet(bgl, GF_BGL_FRAME0_S, &TextBgCntDat[0], GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear(bgl, GF_BGL_FRAME0_S );
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_S, GF_BGL_SCROLL_X_SET, 0);
		GF_BGL_ScrollSet(bgl, GF_BGL_FRAME0_S, GF_BGL_SCROLL_Y_SET, 0);
	}

	GF_BGL_ClearCharSet( GF_BGL_FRAME0_M, 32, 0, HEAPID_EMAIL );
	GF_BGL_ClearCharSet( GF_BGL_FRAME0_S, 32, 0, HEAPID_EMAIL );
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
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_S );
	GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_M );

}


//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g
 *
 * @param	wk		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet( EMAIL_INPUT_WORK * wk )
{
	GF_BGL_INI *bgl = wk->bgl;
	ARCHANDLE* p_handle;

	p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, HEAPID_EMAIL );

	// �㉺��ʂa�f�p���b�g�]��
	ArcUtil_HDL_PalSet(    p_handle, NARC_wifip2pmatch_conect_NCLR, PALTYPE_MAIN_BG, 0, 0,  HEAPID_EMAIL);
	ArcUtil_HDL_PalSet(    p_handle, NARC_wifip2pmatch_conect_NCLR, PALTYPE_SUB_BG,  0, 0,  HEAPID_EMAIL);
	
	// ��b�t�H���g�p���b�g�]��
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, EMAIL_TALKFONT_PAL*0x20, HEAPID_EMAIL );
	TalkFontPaletteLoad( PALTYPE_SUB_BG,  EMAIL_TALKFONT_PAL*0x20, HEAPID_EMAIL );

	// ��b�E�C���h�E�O���t�B�b�N�]��
	TalkWinGraphicSet(	bgl, GF_BGL_FRAME0_M, EMAIL_MESFRAME_CHR, 
						EMAIL_MESFRAME_PAL,  CONFIG_GetWindowType(wk->esys->config), HEAPID_EMAIL );

	MenuWinGraphicSet(	bgl, GF_BGL_FRAME0_M, EMAIL_MENUFRAME_CHR,
						EMAIL_MENUFRAME_PAL, 0, HEAPID_EMAIL );



	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_M, 0 );
	GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_S, 0 );

	ArchiveDataHandleClose( p_handle );
}

#define MENULSIT_X		( 5 )
#define MENULIST_Y		( 5 )
#define MENULIST_SX		( 10 )
#define MENULIST_SY		( 10 )

#define SUB_TEXT_X		(  4 )
#define SUB_TEXT_Y		(  4 )
#define SUB_TEXT_SX		( 23 )
#define SUB_TEXT_SY		( 16 )

#define CONNECT_TEXT_X	(  4 )
#define CONNECT_TEXT_Y	(  1 )
#define CONNECT_TEXT_SX	( 24 )
#define CONNECT_TEXT_SY	(  2 )

// ��b�E�C���h�E�\���ʒu��`
#define TALK_WIN_X		(  2 )
#define TALK_WIN_Y		( 19 )
#define	TALK_WIN_SX		( 27 )
#define	TALK_WIN_SY		(  4 )

#define TALK_MESSAGE_OFFSET	 ( EMAIL_MENUFRAME_CHR + MENU_WIN_CGX_SIZ )
#define ERROR_MESSAGE_OFFSET ( TALK_MESSAGE_OFFSET   + TALK_WIN_SX*TALK_WIN_SY )
#define TITLE_MESSAGE_OFFSET ( ERROR_MESSAGE_OFFSET  + SUB_TEXT_SX*SUB_TEXT_SY )
#define YESNO_OFFSET 		 ( TITLE_MESSAGE_OFFSET  + CONNECT_TEXT_SX*CONNECT_TEXT_SY )
#define MENULIST_MESSAGE_OFFSET	(ERROR_MESSAGE_OFFSET)	//�G���[���b�Z�[�W�ƈꏏ�ɂ͏o�Ȃ��̂� ��check YESNO_OFFSET�̒l�𒲂ׂāA�[���ȋ󂫂�����Ȃ�A���̌��ɂ���

//------------------------------------------------------------------
/**
 * BMPWIN�����i�����p�l���Ƀt�H���g�`��j
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void BmpWinInit( EMAIL_INPUT_WORK *wk )
{
	// ---------- ���C����� ------------------

	// BG0��BMPWIN(�G���[����)�E�C���h�E�m�ہE�`��
	GF_BGL_BmpWinAdd(wk->bgl, &wk->SubWin, GF_BGL_FRAME0_M,
	SUB_TEXT_X, SUB_TEXT_Y, SUB_TEXT_SX, SUB_TEXT_SY, EMAIL_TALKFONT_PAL,  ERROR_MESSAGE_OFFSET );

	GF_BGL_BmpWinDataFill( &wk->SubWin, 0x0000 );

	// BG0��BMP�i��b�E�C���h�E�j�m��
	GF_BGL_BmpWinAdd(wk->bgl, &wk->MsgWin, GF_BGL_FRAME0_M,
		TALK_WIN_X, 
		TALK_WIN_Y, 
		TALK_WIN_SX, 
		TALK_WIN_SY, EMAIL_TALKFONT_PAL,  TALK_MESSAGE_OFFSET );
	GF_BGL_BmpWinDataFill( &wk->MsgWin, 0x0000 );
}

//------------------------------------------------------------------
/**
 * $brief   �m�ۂ���BMPWIN�����
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void BmpWinDelete( EMAIL_INPUT_WORK *wk )
{
	GF_BGL_BmpWinDel( &wk->MsgWin );
	GF_BGL_BmpWinDel( &wk->SubWin );
}

//------------------------------------------------------------------
/**
 * $brief   ��b�E�C���h�E�\��
 *
 * @param   wk		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void Input_MessagePrint( EMAIL_INPUT_WORK *wk, MSGDATA_MANAGER *msgman, int msgno, int wait, u16 dat )
{
	// ������擾
	STRBUF *tempbuf;

	// ������擾
	tempbuf = MSGMAN_AllocString(  msgman, msgno );

	// WORDSET�W�J
	WORDSET_ExpandStr( wk->WordSet, wk->TalkString, tempbuf );

	STRBUF_Delete(tempbuf);

	// ��b�E�C���h�E�g�`��
	GF_BGL_BmpWinDataFill( &wk->MsgWin,  0x0f0f );
	BmpTalkWinWrite( &wk->MsgWin, WINDOW_TRANS_ON, EMAIL_MESFRAME_CHR, EMAIL_MESFRAME_PAL );

	// ������`��J�n
	wk->MsgIndex = GF_STR_PrintSimple( &wk->MsgWin, FONT_TALK, wk->TalkString, 0, 0, wait, NULL);
}


//==============================================================================
//	
//==============================================================================
static int NumInputFunc(EMAIL_INPUT_WORK *wk)
{
	int update = 0;
	int i, k;
	
	if(sys.trg & PAD_BUTTON_A){
		OS_TPrintf("���肵�� ��4�� = %d\n", wk->num_code);
		return FUNCSEQ_FINISH;
	}
	if(sys.trg & PAD_BUTTON_B){
		return FUNCSEQ_CANCEL;
	}
	
	if(sys.trg & PAD_KEY_RIGHT){
		wk->num_pos--;
		if(wk->num_pos < 0){
			wk->num_pos = wk->keta_max - 1;
		}
	}
	if(sys.trg & PAD_KEY_LEFT){
		wk->num_pos++;
		if(wk->num_pos >= wk->keta_max){
			wk->num_pos = 0;
		}
	}
	
	if(sys.trg & PAD_KEY_DOWN){
		wk->number[wk->num_pos]--;
		if(wk->number[wk->num_pos] < 0){
			wk->number[wk->num_pos] = 9;
		}
		update = 1;
	}
	if(sys.trg & PAD_KEY_UP){
		wk->number[wk->num_pos]++;
		if(wk->number[wk->num_pos] > 9){
			wk->number[wk->num_pos] = 0;
		}
		update = 1;
	}
	
	if(update){
		wk->num_code = 0;
		k = 1;
		for(i = 0; i < wk->keta_max; i++){
			wk->num_code += wk->number[i] * k;
			k *= 10;
		}

		WORDSET_RegisterNumber(wk->WordSet, 0, wk->num_code, 4, 
				NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);
		Input_MessagePrint(wk, wk->MsgManager, wk->num_msgid, MSG_ALLPUT, 0);
	}
	
	return FUNCSEQ_CONTINUE;
}

