//==============================================================================
/**
 * @file	�t�@�C����
 * @brief	�ȒP�Ȑ���������
 * @author	matsuda
 * @date	2007.04.06(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#include "common.h"

#include "system/arc_tool.h"
#include "system/bmp_menu.h"
#include "system/brightness.h"
#include "system/fontproc.h"
#include "system/fontoam.h"
#include "system/font_arc.h"
#include "system/snd_def.h"
#include "system/snd_perap.h"
#include "system/snd_tool.h"
#include "system/window.h"
#include "system/msg_ds_icon.h"
#include "system/wipe.h"
#include "system/pmfprint.h"

#include "gflib/msg_print.h"						//STRCODE
#include "system\msgdata.h"							//MSGMAN_TYPE_DIRECT
#include "system/bmp_list.h"
#include "system/pm_str.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"							//WORDSET_Create
#include "system/gra_tool.h"

#include "frontier_types.h"
#include "frontier_main.h"
#include "frontier_scr.h"
#include "fss_task.h"
#include "fs_usescript.h"
#include "frontier_map.h"
#include "frontier_scrcmd.h"
#include "frontier_scrcmd_sub.h"
#include "frontier_id.h"
#include "frontier_act_pri.h"

#include "msgdata/msg.naix"
#include "poketool/pokeicon.h"
#include "application/pokelist.h"


//==============================================================================
//	�萔��`
//==============================================================================
//�J�[�\����
#define FSEV_WIN_CURSOR_WIDTH		(12)			//�J�[�\����

#define FSEV_WIN_DEFAULT_WAIT		(3)				//�L�[���삪�����ɓ���Ȃ��悤�Ɋ�{�E�F�C�g

//BMP���j���[
#define FSEV_WIN_MENU_MAX			(28)			//���j���[���ڂ̍ő吔

//BMP���X�g
#define FSEV_WIN_LIST_MAX			(28)			//���X�g���ڂ̍ő吔

#define	FSEV_WIN_MSG_MAX			(28)			//MSG�o�b�t�@�̍ő吔

//BMP���X�g�w�b�_�[��`
#define EV_LIST_LINE				(8)				//�\���ő區�ڐ�
#define EV_LIST_RABEL_X				(1)				//���x���\���w���W
#define EV_LIST_DATA_X				(12)			//���ڕ\���w���W
#define EV_LIST_CURSOR_X			(2)				//�J�[�\���\���w���W
#define EV_LIST_LINE_Y				(1)				//�\���x���W

#define FSEV_FONT					(FONT_SYSTEM)

#define FSEVWIN_MSG_BUF_SIZE		(40*2)			//���b�Z�[�W�o�b�t�@�T�C�Y


//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct _FSEV_WIN{
	FSS_PTR fss;
	TCB_PTR	tcb;

 	GF_BGL_BMPWIN bmpwin;						//BMP�E�B���h�E�f�[�^
 	GF_BGL_BMPWIN* talk_bmpwin;					//BMP��b�E�B���h�E�f�[�^
	
	STRBUF* msg_buf[FSEV_WIN_MSG_MAX];			//���b�Z�[�W�f�[�^�̃|�C���^
	MSGDATA_MANAGER* msgman;					//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;							//�P��Z�b�g

	u8  wait;									//�E�F�C�g
	u8  menu_id;								//BMP���j���[ID
	u8  cursor_pos;								//�J�[�\���ʒu
	u8  cancel:1;								//�L�����Z��
	u8  msgman_del_flag:1;						//���b�Z�[�W�}�l�[�W���[�폜�t���O
	u8  dmy:6;									//

	u8  x;										//�E�B���h�E�ʒuX
	u8  y;										//�E�B���h�E�ʒuY
	u8  dmyy;									//���j���[�i���o�[
	u8  list_no;								//���j���[���ڂ̉��Ԗڂ�

	u16* pMsg;									//���b�Z�[�W�f�[�^
	u16* work;									//���ʎ擾���[�N
 
	//BMP���j���[(bmp_menu.h)
	BMPMENU_HEADER MenuH;						//BMP���j���[�w�b�_�[
	BMPMENU_WORK * mw;							//BMP���j���[���[�N
	BMPMENU_DATA Data[FSEV_WIN_MENU_MAX];			//BMP���j���[�f�[�^

	//BMP���X�g
	BMPLIST_HEADER ListH;						//BMP���X�g�w�b�_�[
	BMPLIST_WORK* lw;							//BMP���X�g�f�[�^
	u16 list_bak;								//���X�g�ʒu�o�b�N�A�b�v
	u16 cursor_bak;								//�J�[�\���ʒu�o�b�N�A�b�v
	BMPLIST_DATA list_Data[FSEV_WIN_LIST_MAX];	//���X�g�f�[�^
	u16 talk_msg_id[FSEV_WIN_LIST_MAX];			//���X�g�f�[�^�ɑ΂ɂȂ��b���b�Z�[�WID

	u16 pos_bak;

}FSEV_WIN_WORK;


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�A�j���R�}���h�f�[�^�e�[�u���\����
typedef struct{
	u16 cmd;		///<�A�j���[�V�����R�}���h
	u16 way;		///<����
}ANM_CMD_TBL_DATA;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void AddBitmapWin(FSS_PTR fss);
static void LoadPrintString(FSS_PTR fss, const MSGDATA_MANAGER* msgman, u32 msg_id );
static void ScrTalkMsgCore(FSS_PTR fss, int font, int speed, int skip_flag, int auto_flag );

static void EvWin_Init(FSS_PTR fss, FSEVWIN_PTR wk, u8 x, u8 y, u8 cursor, u8 cancel, 
	u16* work, WORDSET* wordset, MSGDATA_MANAGER* msgman );
FSEVWIN_PTR FSSC_Sub_BmpMenu_Init( FSS_PTR fss, u8 x, u8 y, u8 cursor, u8 cancel, 
	u16* work, WORDSET* wordset, MSGDATA_MANAGER* msgman );
void FSSC_Sub_BmpMenu_MakeList( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  );
void FSSC_Sub_BmpMenu_Start( FSEVWIN_PTR wk );
static void BmpMenu_list_make( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  );
static u32 BmpMenu_length_get( FSEVWIN_PTR wk );
static void BmpMenu_h_default_set(FSEVWIN_PTR wk);
static void EvBmpMenu_MainTCB( TCB_PTR tcb, void* wk );
static void EvBmpMenu_Del( FSEVWIN_PTR wk );
static void menu_talk_msg_update( FSEVWIN_PTR wk );
void FSSC_Sub_BmpMenu_Del( FSEVWIN_PTR wk );
static void talk_msg_print( FSEVWIN_PTR wk, u16 talk_msg_id, u32 wait );

//�ȈՉ�b
void FSTalkMsgPMSParam( FS_SYSTEM* fss, u8 spd, u16 stype, u16 sid, u16 word0, s16 word1, u8 skip );
static void CreatePrintStringByPmsParam( STRBUF* buf, u16 stype, u16 sid, PMS_WORD word0, PMS_WORD word1 );

//==============================================================================
//
//	��b���b�Z�[�W
//
//==============================================================================
//--------------------------------------------------------------
/**
 *	��b���b�Z�[�W�E�B���h�E�o��
 *
 * @param	core		FSS_TASK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	skip		���b�Z�[�W�X�L�b�v�t���O
 * @param   ex_param	�g���p�����[�^(�s�v�ȏꍇ��NULL)
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FSSC_Sub_ScrTalkMsg(FSS_PTR fss, const MSGDATA_MANAGER* msgman, u16 msg_id, u8 skip,  FSCR_TALK_EX_PARAM *ex_param )
{
	u8 msg_speed;
	u8 auto_flag;
	u8 font;

	AddBitmapWin(fss);
	LoadPrintString(fss, msgman, msg_id );

	if(ex_param == NULL){
		FMAP_PTR fmap = FSS_GetFMapAdrs(fss);
		FRONTIER_EX_PARAM *ex_param;
		
		ex_param = Frontier_ExParamGet(fmap->fmain);

		msg_speed = CONFIG_GetMsgPrintSpeed(ex_param->config);
		auto_flag = MSG_AUTO_OFF;
		font = FONT_TALK;
	}
	else{
		msg_speed = ex_param->msg_speed;
		auto_flag = ex_param->auto_flag;
		font = ex_param->font;
	}

	ScrTalkMsgCore( fss, font, msg_speed, skip, auto_flag );
}

//------------------------------------------------------------------
/**
 * ��b�E�B���h�E�o�^�E�\��
 *
 * @param   fsys		
 * @param   param		
 *
 */
//------------------------------------------------------------------
static void AddBitmapWin(FSS_PTR fss)
{
	FMAP_PTR fmap = FSS_GetFMapAdrs(fss);
	
	if(fss->win_open_flag == 0){
		GF_BGL_BmpWinAdd(
			fmap->bgl, &fss->bmpwin_talk, FRMAP_FRAME_WIN, FFD_MSG_WIN_PX, FFD_MSG_WIN_PY,
			FFD_MSG_WIN_SX, FFD_MSG_WIN_SY, FFD_MSG_WIN_PAL, FFD_MSG_WIN_CGX );
		GF_BGL_BmpWinDataFill( &fss->bmpwin_talk, FBMP_COL_WHITE );
		BmpTalkWinWrite(&fss->bmpwin_talk, WINDOW_TRANS_ON, FR_TALK_WIN_CGX_NUM, FR_TALK_WIN_PAL );

		fss->win_open_flag = 1;
	}
	else{
		GF_BGL_BmpWinDataFill( &fss->bmpwin_talk, FBMP_COL_WHITE );
	}
}

//------------------------------------------------------------------
/**
 * �o�͕�������p�����[�^�����o�b�t�@�ɓǂݍ���
 *
 * @param   param		
 * @param   msgman		
 * @param   msg_id		
 *
 */
//------------------------------------------------------------------
static void LoadPrintString(FSS_PTR fss, const MSGDATA_MANAGER* msgman, u32 msg_id )
{
	MSGMAN_GetString( msgman, msg_id, fss->tmp_buf );
	WORDSET_ExpandStr( fss->wordset, fss->msg_buf, fss->tmp_buf );
}

//------------------------------------------------------------------
/**
 * ��b�E�B���h�E�Ƀ��b�Z�[�W�𗬂��������J�n
 *
 * @param   msg_param		
 * @param   font		
 * @param   speed		
 * @param   skip_flag		
 * @param   auto_flag		
 *
 */
//------------------------------------------------------------------
static void ScrTalkMsgCore(FSS_PTR fss, int font, int speed, int skip_flag, int auto_flag )
{
	MsgPrintSkipFlagSet( skip_flag );
	MsgPrintAutoFlagSet( auto_flag );
	MsgPrintTouchPanelFlagSet( MSG_TP_OFF );
	fss->msg_talk_index = GF_STR_PrintSimple( &fss->bmpwin_talk, font, 
		fss->msg_buf, 0, 0, speed, NULL );
}

//--------------------------------------------------------------
/**
 * @brief   ��b�E�B���h�E�����
 *
 * @param   fss		
 */
//--------------------------------------------------------------
void FSSC_Sub_ScrTalkClose(FSS_PTR fss)
{
	GF_ASSERT(fss->win_open_flag == 1);
	
	BmpTalkWinClear( &fss->bmpwin_talk, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &fss->bmpwin_talk );
	
	fss->win_open_flag = 0;
}

//==============================================================================
//	�ȈՉ�b
//==============================================================================

//--------------------------------------------------------------
/**
 * �p�����[�^����ȈՉ�b�f�[�^�𐶐��A���b�Z�[�W�o�͂���
 *
 * @param	core		VM_MACHINE�^�̃|�C���^
 * @param	stype		
 * @param	sid
 * @param	word0
 * @param	word1
 * @param	skip
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FSTalkMsgPMSParam( FS_SYSTEM* fss, u8 spd, u16 stype, u16 sid, u16 word0, s16 word1, u8 skip )
{
	PMS_DATA pms;

	AddBitmapWin( fss );

	//�ȈՉ�b�f�[�^���當����𐶐����A�p�����[�^�����o�b�t�@�ɓǂݍ���
	CreatePrintStringByPmsParam( fss->msg_buf, stype, sid, word0, word1 );

	if(skip != 0xFF){
		ScrTalkMsgCore( fss, FONT_TALK, spd, skip, MSG_AUTO_OFF );
	}else{
		// skip == 0xff �������� �ꊇ�\������
		ScrTalkMsgCore( fss, FONT_TALK, MSG_ALLPUT, skip, MSG_AUTO_OFF );
	}
}

//------------------------------------------------------------------
/**
 * �ȈՉ�b�f�[�^���當����𐶐����A�p�����[�^�����o�b�t�@�ɓǂݍ���
 *
 * @param   param		
 * @param   stype		
 * @param   sid		
 * @param   word0		
 * @param   word1		
 *
 */
//------------------------------------------------------------------
static void CreatePrintStringByPmsParam( STRBUF* buf, u16 stype, u16 sid, PMS_WORD word0, PMS_WORD word1 )
{
	PMS_DATA     pms;
	STRBUF* tmpBuf;

	// �ȈՉ�b�f�[�^����
	PMSDAT_Clear(&pms);
	PMSDAT_SetSentence( &pms, stype, sid );
	PMSDAT_SetWord( &pms, 0, word0 );
	PMSDAT_SetWord( &pms, 1, word1 );

	tmpBuf = PMSDAT_ToString( &pms, HEAPID_EVENT );
	STRBUF_Copy( buf, tmpBuf );
	STRBUF_Delete( tmpBuf );
}

//==============================================================================
//	BMP���j���[
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief	�C�x���g�E�B���h�E�@���[�N������
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvWin_Init(FSS_PTR fss, FSEVWIN_PTR wk, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, MSGDATA_MANAGER* msgman )
{
	int i;

	if( msgman == NULL ){
		//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
		wk->msgman = MSGMAN_Create(MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_ev_win_dat, fss->heap_id);
		wk->msgman_del_flag = 1;
	}else{
		wk->msgman = msgman;	//���p��
		wk->msgman_del_flag = 0;
	}

	//wk->wordset = WORDSET_Create( fss->heap_id );
	wk->wordset		= wordset;		//�X�N���v�g�������p��

	wk->fss			= fss;
	wk->work		= work;
#if 1
	*wk->work		= 0;			//�����I���Ń��[�N�̒l������悤�ɂȂ����̂ŏ����������Ă���
#endif
	wk->cancel		= cancel;
	wk->cursor_pos	= cursor;
	wk->x			= x;
	wk->y			= y;
	wk->list_no		= 0;
	wk->talk_bmpwin	= &fss->bmpwin_talk;
	wk->wait		= FSEV_WIN_DEFAULT_WAIT;
	wk->pos_bak		= cursor;

	for( i=0; i < FSEV_WIN_MENU_MAX ;i++ ){
		wk->Data[i].str			= NULL;
		wk->Data[i].param		= 0;
	}

	for( i=0; i < FSEV_WIN_LIST_MAX ;i++ ){
		wk->list_Data[i].str	= NULL;
		wk->list_Data[i].param	= 0;
		wk->talk_msg_id[i] = FSEV_WIN_TALK_MSG_NONE;
	}

	//MSGMAN_Create�̌�ɏ���
	for( i=0; i < FSEV_WIN_MSG_MAX ;i++ ){
		wk->msg_buf[i] = STRBUF_Create( FSEVWIN_MSG_BUF_SIZE, fss->heap_id );
	}

	//�I�������l���擾���郏�[�N��������
	*wk->work = FSEV_WIN_NOTHING;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[�@������
 *
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	"FSEV_WIN_WORK�^�̃A�h���X�ANULL�͎��s"
 */
//--------------------------------------------------------------
FSEVWIN_PTR FSSC_Sub_BmpMenu_Init( FSS_PTR fss, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, MSGDATA_MANAGER* msgman )
{
	FSEVWIN_PTR wk;
	int i;

	wk = sys_AllocMemory( fss->heap_id, sizeof(FSEV_WIN_WORK) );
	if( wk == NULL ){
		OS_Printf( "ev_win.c Alloc ERROR!" );
		return NULL;
	}
	memset( wk, 0, sizeof(FSEV_WIN_WORK) );

	//���[�N������	
	EvWin_Init( fss, wk, x, y, cursor, cancel, work, wordset, msgman );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[�@���X�g�쐬
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	param		BMPMENU�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FSSC_Sub_BmpMenu_MakeList( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	BmpMenu_list_make( wk, msg_id, talk_msg_id, param  );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[�@�J�n
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	param		BMPMENU�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FSSC_Sub_BmpMenu_Start( FSEVWIN_PTR wk )
{
	u32 len;
	FMAP_PTR fmap = FSS_GetFMapAdrs(wk->fss);

	//(�ő啶�����{�J�[�\��) * �t�H���g�T�C�Y
	//len = (BmpMenu_length_get(wk)+1) * FontHeaderGet(FSEV_FONT, FONT_HEADER_SIZE_X);
	len = BmpMenu_length_get(wk);

	if( (len % 8) == 0 ){
		len = (len / 8);
	}else{
		len = (len / 8)+1;
	}

	GF_BGL_BmpWinAdd( fmap->bgl, &wk->bmpwin, FRMAP_FRAME_WIN, wk->x, wk->y, 
									len, wk->list_no*2, FFD_SYSFONT_PAL, FFD_FREE_CGX );

	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
//	MenuWinGraphicSet(
//		wk->fsys->bgl, FFD_MBGFRM_FONT, FR_MENU_WIN_CGX_NUM, FR_MENU_WIN_PAL, 0, fss->heap_id );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite(&wk->bmpwin, WINDOW_TRANS_OFF, FR_MENU_WIN_CGX_NUM, FR_MENU_WIN_PAL);

	//���j���[�w�b�_�[�����ݒ�
	BmpMenu_h_default_set(wk);
	wk->mw = BmpMenuAdd( &wk->MenuH, wk->cursor_pos, wk->fss->heap_id );

	//��b�E�B���h�E�X�V
	menu_talk_msg_update( wk );

	//TCB�ǉ�
	wk->tcb	= TCB_Add( EvBmpMenu_MainTCB, wk, 0 );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[	���X�g�쐬
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPMENU�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpMenu_list_make( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( wk->list_no < FSEV_WIN_MENU_MAX, "���j���[���ڐ��I�[�o�[�I" );

	{
		//�W�J����
		
		STRBUF* tmp_buf2= STRBUF_Create( FSEVWIN_MSG_BUF_SIZE, wk->fss->heap_id );//�R�s�[�p�o�b�t�@

		MSGMAN_GetString( wk->msgman, msg_id, tmp_buf2 );
		WORDSET_ExpandStr( wk->wordset, wk->msg_buf[wk->list_no], tmp_buf2 );	//�W�J
		wk->Data[ wk->list_no ].str = (const void *)wk->msg_buf[wk->list_no];

		STRBUF_Delete( tmp_buf2 );
	}

	wk->talk_msg_id[ wk->list_no ] = talk_msg_id;
	wk->Data[ wk->list_no ].param = param;
	wk->list_no++;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[	���ڂ̒�����ő啶�������擾
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static u32 BmpMenu_length_get( FSEVWIN_PTR wk )
{
	int i;
	u32 ret,tmp_ret;

	ret		= 0;
	tmp_ret = 0;

	for( i=0; i < wk->list_no ;i++ ){
		if( wk->Data[i].str == NULL ){
			break;
		}

		//ret = FontProc_GetPrintStrWidth( FSEV_FONT, wk->msg_buf[i], 0 );
		ret = FontProc_GetPrintStrWidth( FSEV_FONT, (STRBUF*)wk->Data[i].str, 0 );

		if( tmp_ret < ret ){
			tmp_ret = ret;
		}
	}

	return tmp_ret + FSEV_WIN_CURSOR_WIDTH;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j��	�w�b�_�[�����ݒ�
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpMenu_h_default_set(FSEVWIN_PTR wk)
{
	wk->MenuH.menu		= wk->Data;

	wk->MenuH.win		= &wk->bmpwin;

	wk->MenuH.font		= FSEV_FONT;
	wk->MenuH.x_max		= 1;
	wk->MenuH.y_max		= wk->list_no;
	wk->MenuH.line_spc	= 0;
	wk->MenuH.c_disp_f	= 0;

	//���j���[���ڂ�4��ވȏゾ�������̓J�[�\���̃��[�v�L��ɂ���
	if( wk->list_no >= 4 ){
		wk->MenuH.loop_f	= 1;			//�L
	}else{
		wk->MenuH.loop_f	= 0;			//��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[�@���C��
 *
 * @param	tcb			TCB_PTR
 * @param	wk			���[�N�̃A�h���X
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvBmpMenu_MainTCB( TCB_PTR tcb, void* wk )
{
	u32 ret;
	FSEV_WIN_WORK *swk = wk;

	//�E�F�C�g
	if( swk->wait != 0 ){
		swk->wait--;
		return;
	}

	//���C�v���͏������Ȃ�
	if( WIPE_SYS_EndCheck() == FALSE ){
		return;
	}

	ret = BmpMenuMain( swk->mw );

	//��b�E�B���h�E�̃��b�Z�[�W�X�V
	if( (sys.repeat & PAD_KEY_UP) || (sys.repeat & PAD_KEY_DOWN) ||
		(sys.repeat & PAD_KEY_LEFT) || (sys.repeat & PAD_KEY_RIGHT) ){
		menu_talk_msg_update( swk );
	}

	//�t�����e�B�A�̎{�݂ŁA�����폜�̒l�������Ă�����
	if( *swk->work == FSEV_WIN_COMPULSION_DEL ){

		EvBmpMenu_Del(wk);

	}else{

		switch( ret ){
		case BMPMENU_NULL:
			break;
		case BMPMENU_CANCEL:
			if( swk->cancel == TRUE ){			//TRUE = B�L�����Z���L��
				*swk->work = FSEV_WIN_B_CANCEL;	//�I�������l�����[�N�ɑ��
				EvBmpMenu_Del(wk);
			}
			break;
		default:
			*swk->work = ret;					//�I�������l�����[�N�ɑ��
			EvBmpMenu_Del(wk);
			break;
		};

	}

	return;
};

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[�@�I��
 *
 * @param	wk			FSEV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 *
 * SEQ_SE_DP_SELECT��炵�Ă���I
 */
//--------------------------------------------------------------
static void EvBmpMenu_Del( FSEVWIN_PTR wk )
{
	int i;

	Snd_SePlay(SEQ_SE_DP_SELECT);	//���ӁI

	BmpMenuExit( wk->mw, NULL );
	BmpMenuWinClear( wk->MenuH.win, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( wk->MenuH.win );

	for( i=0; i < FSEV_WIN_MSG_MAX ;i++ ){
		STRBUF_Delete( wk->msg_buf[i] );
	}

	if( wk->msgman_del_flag == 1 ){
		//WORDSET_Delete( wk->wordset );
		MSGMAN_Delete( wk->msgman );
	}

	TCB_Delete( wk->tcb );
	sys_FreeMemoryEz( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	��b�E�B���h�E�Ƀ��b�Z�[�W���ꊇ�\��
 *
 * @param	wk			EV_WIN_WORK�^�̃A�h���X
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	wait		���b�Z�[�W�E�F�C�g
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void menu_talk_msg_update( FSEVWIN_PTR wk )
{
	u8 csr_pos;

	csr_pos = BmpMenuCursorPosGet( wk->mw );			//�J�[�\�����W�擾
	//OS_Printf( "csr_pos = %d\n", csr_pos );

	if( wk->talk_msg_id[csr_pos] != FSEV_WIN_TALK_MSG_NONE ){
		talk_msg_print( wk, wk->talk_msg_id[csr_pos], MSG_ALLPUT );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[	�����I��
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 *
 * �o�g���L���b�X���݂̂Ŏg�p����
 */
//--------------------------------------------------------------
void FSSC_Sub_BmpMenu_Del( FSEVWIN_PTR wk )
{
	FSEV_WIN_WORK* swk;

	if( wk == NULL ){
		return;
	}

	swk = (FSEV_WIN_WORK*)wk;

	*swk->work = FSEV_WIN_B_CANCEL;	//�I�������l�����[�N�ɑ��

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	EvWin_ActorRelease( wk );		//�폜
#endif

	EvBmpMenu_Del(wk);
	return;
}


#if 1
//==============================================================================================
//
//	BMP���X�g�֘A
//
//==============================================================================================
FSEVWIN_PTR FSSC_Sub_BmpList_Init( FSS_PTR fss, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, MSGDATA_MANAGER* msgman );
void FSSC_Sub_BmpList_MakeList( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  );
void FSSC_Sub_BmpList_Start( FSEVWIN_PTR wk );
static void BmpList_list_make( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  );
static u32 BmpList_length_get( FSEVWIN_PTR wk );
static void BmpList_h_default_set(FSEVWIN_PTR wk);
static void	BmpList_CallBack(BMPLIST_WORK* lw,u32 param,u8 y);
static void BmpList_CursorMoveCallBack( BMPLIST_WORK* wk, u32 param, u8 mode );
static void EvBmpList_MainTCB( TCB_PTR tcb, void* wk );
static void EvBmpList_Del( FSEVWIN_PTR wk, u8 se_flag );
static void list_talk_msg_update( FSEVWIN_PTR wk );
void FSSC_Sub_BmpList_Del( FSEVWIN_PTR wk );

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@������
 *
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	"EV_WIN_WORK�^�̃A�h���X�ANULL�͎��s"
 */
//--------------------------------------------------------------
FSEVWIN_PTR FSSC_Sub_BmpList_Init( FSS_PTR fss, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, MSGDATA_MANAGER* msgman )
{
	return FSSC_Sub_BmpMenu_Init( fss, x, y, cursor, cancel, work, wordset, msgman );
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g�쐬
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPLIST�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FSSC_Sub_BmpList_MakeList( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	BmpList_list_make( wk, msg_id, talk_msg_id, param  );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@�J�n
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void FSSC_Sub_BmpList_Start( FSEVWIN_PTR wk )
{
	u32 len;
	FMAP_PTR fmap = FSS_GetFMapAdrs(wk->fss);

	//(�ő啶�����{�J�[�\��) * �t�H���g�T�C�Y
	//len = (BmpList_length_get(wk)+1) * FontHeaderGet(FSEV_FONT, FONT_HEADER_SIZE_X);
	len = BmpList_length_get(wk);

	if( (len % 8) == 0 ){
		len = (len / 8);
	}else{
		len = (len / 8)+1;
	}

	//�\���ő區�ڐ��`�F�b�N
	if( wk->list_no > EV_LIST_LINE ){
		GF_BGL_BmpWinAdd( fmap->bgl, &wk->bmpwin, FRMAP_FRAME_WIN, wk->x, wk->y, 
									len, EV_LIST_LINE*2, FFD_SYSFONT_PAL, FFD_FREE_CGX );
	}else{
		GF_BGL_BmpWinAdd( fmap->bgl, &wk->bmpwin, FRMAP_FRAME_WIN, wk->x, wk->y, 
									len, wk->list_no*2, FFD_SYSFONT_PAL, FFD_FREE_CGX );
	}

	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
//	MenuWinGraphicSet(
//		wk->fsys->bgl, FLD_MBGFRM_FONT, MENU_WIN_CGX_NUM, MENU_WIN_PAL, 0, HEAPID_FIELD );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite(&wk->bmpwin, WINDOW_TRANS_OFF, FR_MENU_WIN_CGX_NUM, FR_MENU_WIN_PAL);

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	EvWin_ActorInit( wk );
#endif

	//���X�g�w�b�_�[�����ݒ�
	BmpList_h_default_set(wk);

	wk->lw = BmpListSet( (const BMPLIST_HEADER*)&wk->ListH, 0, wk->cursor_pos, wk->fss->heap_id );

	//��b�E�B���h�E�X�V
	list_talk_msg_update( wk );

	//TCB�ǉ�
	wk->tcb	= TCB_Add( EvBmpList_MainTCB, wk, 0 );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	���X�g�쐬
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPLIST�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_list_make( FSEVWIN_PTR wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( wk->list_no < EV_WIN_LIST_MAX, "���X�g���ڐ��I�[�o�[�I" );

#if 1
	{
		//�W�J����
		
		STRBUF* tmp_buf2= STRBUF_Create( FSEVWIN_MSG_BUF_SIZE, wk->fss->heap_id );//�R�s�[�p�o�b�t�@

		MSGMAN_GetString( wk->msgman, msg_id, tmp_buf2 );
		WORDSET_ExpandStr( wk->wordset, wk->msg_buf[wk->list_no], tmp_buf2 );	//�W�J
		wk->list_Data[ wk->list_no ].str = (const void *)wk->msg_buf[wk->list_no];

		STRBUF_Delete( tmp_buf2 );
	}
#else

	MSGMAN_GetString( wk->msgman, msg_id, wk->msg_buf[wk->list_no] );
	wk->list_Data[ wk->list_no ].str = (const void *)wk->msg_buf[wk->list_no];
	//*st = (const void *)&wk->msg_buf[wk->list_no][0];
#endif
	if( param == FSEV_WIN_LIST_LABEL ){
		wk->list_Data[ wk->list_no ].param = BMPLIST_RABEL;
		//*pa = BMPLIST_RABEL;
	}else{
		wk->list_Data[ wk->list_no ].param = param;
		//*pa = param;
	}

	wk->talk_msg_id[ wk->list_no ] = talk_msg_id;
	wk->list_no++;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	���ڂ̒�����ő啶�������擾
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static u32 BmpList_length_get( FSEVWIN_PTR wk )
{
	int i;
	u32 ret,tmp_ret;

	ret		= 0;
	tmp_ret = 0;

	for( i=0; i < wk->list_no ;i++ ){
		if( wk->list_Data[i].str == NULL ){
			break;
		}

		//ret = FontProc_GetPrintStrWidth( FSEV_FONT, wk->msg_buf[i], 0 );
		ret = FontProc_GetPrintStrWidth( FSEV_FONT, (STRBUF*)wk->list_Data[i].str, 0 );
		
		if( tmp_ret < ret ){
			tmp_ret = ret;
		}
	}

	return tmp_ret + FSEV_WIN_CURSOR_WIDTH;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�w�b�_�[�����ݒ�
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_h_default_set(FSEVWIN_PTR wk)
{
	wk->ListH.list		= wk->list_Data;
	wk->ListH.call_back = BmpList_CursorMoveCallBack;
	wk->ListH.icon		= BmpList_CallBack;
	wk->ListH.win		= &wk->bmpwin;

	wk->ListH.count		= wk->list_no;
	wk->ListH.line		= EV_LIST_LINE;

	wk->ListH.rabel_x	= EV_LIST_RABEL_X;
	wk->ListH.data_x	= EV_LIST_DATA_X;
	wk->ListH.cursor_x	= EV_LIST_CURSOR_X;
	wk->ListH.line_y	= EV_LIST_LINE_Y;

	wk->ListH.f_col		= FBMP_COL_BLACK;
	wk->ListH.b_col		= FBMP_COL_WHITE;		
	wk->ListH.s_col		= FBMP_COL_BLK_SDW;

	wk->ListH.msg_spc	= 0;
	wk->ListH.line_spc	= 16;
#if 0
	wk->ListH.page_skip	= BMPLIST_LRKEY_SKIP;
#else
	wk->ListH.page_skip	= BMPLIST_NO_SKIP;				//08.05.31�ύX
#endif
	wk->ListH.font		= FSEV_FONT;
	wk->ListH.c_disp_f	= 0;

	wk->ListH.work		= (void*)wk;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�V�X�e������̃R�[���o�b�N����s�\������
 *
 * @param	lw			BMPLIST_WORK�^�̃|�C���^
 * @param	param		�I�������^�[���p�����[�^
 * @param	y			
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void	BmpList_CallBack(BMPLIST_WORK* lw,u32 param,u8 y)
{
	if( param == BMPLIST_RABEL ){
		BmpListTmpColorChange( lw, FBMP_COL_RED, FBMP_COL_WHITE, FBMP_COL_RED_SDW );
	}else{
		BmpListTmpColorChange( lw, FBMP_COL_BLACK, FBMP_COL_WHITE, FBMP_COL_BLK_SDW );
	}
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_CursorMoveCallBack( BMPLIST_WORK* wk, u32 param, u8 mode )
{
	u32 count,line;
	u16 list_bak = 0;
	u16 cursor_bak = 0;
	FSEVWIN_PTR evwin_wk = (FSEVWIN_PTR)BmpListParamGet(wk,BMPLIST_ID_WORK);

#ifdef EV_WIN_SCROLL_CURSOR	//070228

	//��������
	if( mode == 1 ){
		//
	}

	count = BmpListParamGet( wk, BMPLIST_ID_COUNT );	//���X�g���ڐ�
	line  = BmpListParamGet( wk, BMPLIST_ID_LINE );		//�\���ő區�ڐ�

	//�S�Ă̍��ڂ�\�����Ă��Ȃ���
	if( count > line ){

		BmpListPosGet( wk, &list_bak, &cursor_bak );

		if( list_bak == 0 ){
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\����\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\���\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], FALSE );	//OBJ�̔�\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], TRUE );		//OBJ�̕\���ݒ�
		}else if( list_bak == (count-line) ){
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\���\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\����\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], TRUE );		//OBJ�̕\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], FALSE );	//OBJ�̔�\���ݒ�
		}else{
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\���\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\���\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], TRUE );		//OBJ�̕\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], TRUE );		//OBJ�̕\���ݒ�
		}
	}

#endif

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g���C��
 *
 * @param	tcb			TCB_PTR
 * @param	wk			���[�N�̃A�h���X
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvBmpList_MainTCB( TCB_PTR tcb, void* wk )
{
	u16 tmp_pos_bak;
	u32	ret;
	FSEV_WIN_WORK* swk;
	swk = (FSEV_WIN_WORK*)wk;

	//�E�F�C�g
	if( swk->wait != 0 ){
		swk->wait--;
		return;
	}

	//���C�v���͏������Ȃ�
	if( WIPE_SYS_EndCheck() == FALSE ){
		return;
	}

	ret = BmpListMain( swk->lw );

	//BMP���X�g�̑S�̈ʒu���擾
	tmp_pos_bak = swk->pos_bak;		//�ޔ�
	BmpListDirectPosGet( swk->lw, &swk->pos_bak );

	//�J�[�\�������������`�F�b�N
	if( tmp_pos_bak != swk->pos_bak ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

	//��b�E�B���h�E�̃��b�Z�[�W�X�V
	if( (sys.repeat & PAD_KEY_UP) || (sys.repeat & PAD_KEY_DOWN) ||
		(sys.repeat & PAD_KEY_LEFT) || (sys.repeat & PAD_KEY_RIGHT) ){
		list_talk_msg_update( swk );
	}

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	CLACT_Draw( swk->fcat.cas );
#endif

	//�t�����e�B�A�̎{�݂ŁA�����폜�̒l�������Ă�����
	if( *swk->work == FSEV_WIN_COMPULSION_DEL ){

#ifdef EV_WIN_SCROLL_CURSOR	//070228
		EvWin_ActorRelease( wk );				//�폜
#endif
		EvBmpList_Del(wk,0);

	}else{

		switch( ret ){
		case BMPLIST_NULL:
			break;
		case BMPLIST_CANCEL:
			if( swk->cancel == TRUE ){			//TRUE = B�L�����Z���L��
				Snd_SePlay( SEQ_SE_DP_SELECT );
				*swk->work = FSEV_WIN_B_CANCEL;	//�I�������l�����[�N�ɑ��
#ifdef EV_WIN_SCROLL_CURSOR	//070228
				EvWin_ActorRelease( wk );		//�폜
#endif
				EvBmpList_Del(wk,1);
			}
			break;
		default:
			Snd_SePlay( SEQ_SE_DP_SELECT );
			*swk->work = ret;					//�I�������l�����[�N�ɑ��
#ifdef EV_WIN_SCROLL_CURSOR	//070228
			EvWin_ActorRelease( wk );			//�폜
#endif
			EvBmpList_Del(wk,1);
			break;
		};

	}

	return;
};

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g�I��
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 *
 * SEQ_SE_DP_SELECT��炵�Ă���I
 */
//--------------------------------------------------------------
static void EvBmpList_Del( FSEVWIN_PTR wk, u8 se_flag )
{
	int i;

	if( se_flag == 1 ){
		Snd_SePlay(SEQ_SE_DP_SELECT);	//���ӁI
	}

	BmpListExit( wk->lw, NULL, NULL );
	BmpMenuWinClear( wk->ListH.win, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &wk->bmpwin );

	for( i=0; i < FSEV_WIN_MSG_MAX ;i++ ){
		STRBUF_Delete( wk->msg_buf[i] );
	}

	if( wk->msgman_del_flag == 1 ){
		//WORDSET_Delete( wk->wordset );
		MSGMAN_Delete( wk->msgman );
	}

	TCB_Delete( wk->tcb );
	sys_FreeMemoryEz( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	��b�E�B���h�E�Ƀ��b�Z�[�W���ꊇ�\��
 *
 * @param	wk			EV_WIN_WORK�^�̃A�h���X
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	wait		���b�Z�[�W�E�F�C�g
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void talk_msg_print( FSEVWIN_PTR wk, u16 talk_msg_id, u32 wait )
{
	STRBUF* tmp_buf = STRBUF_Create( FSEVWIN_MSG_BUF_SIZE, wk->fss->heap_id );
	STRBUF* tmp_buf2= STRBUF_Create( FSEVWIN_MSG_BUF_SIZE, wk->fss->heap_id );

	//���b�Z�[�W�N���A
	GF_BGL_BmpWinDataFill( wk->talk_bmpwin, (FBMP_COL_WHITE) );

	MSGMAN_GetString( wk->msgman, talk_msg_id, tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, tmp_buf2, tmp_buf );

	GF_STR_PrintSimple( wk->talk_bmpwin, FONT_TALK, tmp_buf2, 0, 0, wait, NULL );

	STRBUF_Delete( tmp_buf );
	STRBUF_Delete( tmp_buf2 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	��b�E�B���h�E�Ƀ��b�Z�[�W���ꊇ�\��
 *
 * @param	wk			EV_WIN_WORK�^�̃A�h���X
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	wait		���b�Z�[�W�E�F�C�g
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void list_talk_msg_update( FSEVWIN_PTR wk )
{
	BmpListDirectPosGet( wk->lw, &wk->cursor_bak );		//�J�[�\�����W�擾
	if( wk->talk_msg_id[wk->cursor_bak] != FSEV_WIN_TALK_MSG_NONE ){
		talk_msg_print( wk, wk->talk_msg_id[wk->cursor_bak], MSG_ALLPUT );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�����I��
 *
 * @param	wk			EV_WIN_WORK�^�̃|�C���^
 *
 * @retval	none
 *
 * �o�g���L���b�X���݂̂Ŏg�p����
 */
//--------------------------------------------------------------
void FSSC_Sub_BmpList_Del( FSEVWIN_PTR wk )
{
	FSEV_WIN_WORK* swk;

	if( wk == NULL ){
		return;
	}

	swk = (FSEV_WIN_WORK*)wk;

	*swk->work = FSEV_WIN_B_CANCEL;	//�I�������l�����[�N�ɑ��

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	EvWin_ActorRelease( wk );		//�폜
#endif

	EvBmpList_Del(wk,0);
	return;
}

#endif


//==============================================================================
//
//	OBJ�A�j���[�V����
//
//==============================================================================

//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�A�j���[�V�����E�F�C�g�e�[�u��
ALIGN4 static const u8 AnmCmdWaitTbl[] = {
	1,		//WAIT_1F
	2,		//WAIT_2F
	4,		//WAIT_4F
	8,		//WAIT_8F
	15,		//WAIT_15F
	16,		//WAIT_16F
	32,		//WAIT_32F
};

///�A�j���[�V�����R�}���h�f�[�^�e�[�u��
static ANM_CMD_TBL_DATA AnmCmdTbl[] = {
	{WF2DMAP_CMD_NONE,	WF2DMAP_WAY_UP},	//FC_DIR_U	
	{WF2DMAP_CMD_NONE,	WF2DMAP_WAY_DOWN},	//FC_DIR_D	
	{WF2DMAP_CMD_NONE,	WF2DMAP_WAY_LEFT},	//FC_DIR_L	
	{WF2DMAP_CMD_NONE,	WF2DMAP_WAY_RIGHT},	//FC_DIR_R	
	{WF2DMAP_CMD_WALK,	WF2DMAP_WAY_UP},	//FC_WALK_U_8F
	{WF2DMAP_CMD_WALK,	WF2DMAP_WAY_DOWN},	//FC_WALK_D_8F
	{WF2DMAP_CMD_WALK,	WF2DMAP_WAY_LEFT},	//FC_WALK_L_8F
	{WF2DMAP_CMD_WALK,	WF2DMAP_WAY_RIGHT},	//FC_WALK_R_8F
	{WF2DMAP_CMD_SLOWWALK,	WF2DMAP_WAY_UP},	//FC_WALK_U_16F
	{WF2DMAP_CMD_SLOWWALK,	WF2DMAP_WAY_DOWN},	//FC_WALK_D_16F
	{WF2DMAP_CMD_SLOWWALK,	WF2DMAP_WAY_LEFT},	//FC_WALK_L_16F
	{WF2DMAP_CMD_SLOWWALK,	WF2DMAP_WAY_RIGHT},	//FC_WALK_R_16F
	{WF2DMAP_CMD_WALK4,	WF2DMAP_WAY_UP},	//FC_WALK_U_4F
	{WF2DMAP_CMD_WALK4,	WF2DMAP_WAY_DOWN},	//FC_WALK_D_4F
	{WF2DMAP_CMD_WALK4,	WF2DMAP_WAY_LEFT},	//FC_WALK_L_4F
	{WF2DMAP_CMD_WALK4,	WF2DMAP_WAY_RIGHT},	//FC_WALK_R_4F
	{WF2DMAP_CMD_STAYWALK8,	WF2DMAP_WAY_UP},	//FC_STAYWALK_U_8F
	{WF2DMAP_CMD_STAYWALK8,	WF2DMAP_WAY_DOWN},	//FC_STAYWALK_D_8F
	{WF2DMAP_CMD_STAYWALK8,	WF2DMAP_WAY_LEFT},	//FC_STAYWALK_L_8F
	{WF2DMAP_CMD_STAYWALK8,	WF2DMAP_WAY_RIGHT},	//FC_STAYWALK_R_8F
	{WF2DMAP_CMD_STAYWALK16,WF2DMAP_WAY_UP},	//FC_STAYWALK_U_16F
	{WF2DMAP_CMD_STAYWALK16,WF2DMAP_WAY_DOWN},	//FC_STAYWALK_D_16F
	{WF2DMAP_CMD_STAYWALK16,WF2DMAP_WAY_LEFT},	//FC_STAYWALK_L_16F
	{WF2DMAP_CMD_STAYWALK16,WF2DMAP_WAY_RIGHT},	//FC_STAYWALK_R_16F
	{WF2DMAP_CMD_STAYWALK2,	WF2DMAP_WAY_UP},	//FC_STAYWALK_U_2F
	{WF2DMAP_CMD_STAYWALK2,	WF2DMAP_WAY_DOWN},	//FC_STAYWALK_D_2F
	{WF2DMAP_CMD_STAYWALK2,	WF2DMAP_WAY_LEFT},	//FC_STAYWALK_L_2F
	{WF2DMAP_CMD_STAYWALK2,	WF2DMAP_WAY_RIGHT},	//FC_STAYWALK_R_2F
	{WF2DMAP_CMD_STAYWALK4,	WF2DMAP_WAY_UP},	//FC_STAYWALK_U_4F
	{WF2DMAP_CMD_STAYWALK4,	WF2DMAP_WAY_DOWN},	//FC_STAYWALK_D_4F
	{WF2DMAP_CMD_STAYWALK4,	WF2DMAP_WAY_LEFT},	//FC_STAYWALK_L_4F
	{WF2DMAP_CMD_STAYWALK4,	WF2DMAP_WAY_RIGHT},	//FC_STAYWALK_R_4F
};

//--------------------------------------------------------------
/**
 * @brief   �t�B�[���hOBJ�A�j���[�V�������s���C��
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		FSSC_ANIME_MOVE_WORK�\����
 */
//--------------------------------------------------------------
void FSSC_Sub_AnimeListMain(TCB_PTR tcb, void *work)
{
	FSSC_ANIME_MOVE_WORK *move = work;
	WF2DMAP_ACTCMD act;
	u16 code, num;
	enum{
		SEQ_DATA_CHECK,
		SEQ_ANM_REQ,
		SEQ_END,
	};
	
	code = move->anm_list->code;
	num = move->anm_list->num;
	switch(move->seq){
	case SEQ_DATA_CHECK:
		if(code == FSS_DATA_END_CODE){
			move->seq = SEQ_END;
			break;
		}
		//break;
	case SEQ_ANM_REQ:
		if(WF2DMAP_OBJWkDataGet(move->fss_actor->objwk, WF2DMAP_OBJPM_ST) == WF2DMAP_OBJST_NONE){
			//wait�`�F�b�N
			if(code >= FC_WAIT_1F && code <= FC_WAIT_32F){
				move->wait++;
				if(move->wait >= AnmCmdWaitTbl[code - FC_WAIT_1F]){
					move->wait = 0;
					move->anm_list++;
				}
				break;
			}
			else if(code >= FC_SYSCMD_START && code < FC_SYSCMD_END){
				switch(code){
				case FC_SYSCMD_VISIBLE_ON:
					WF2DMAP_OBJDrawWkDrawFlagSet(move->fss_actor->drawwk, ON);
					break;
				case FC_SYSCMD_VISIBLE_OFF:
					WF2DMAP_OBJDrawWkDrawFlagSet(move->fss_actor->drawwk, OFF);
					break;
				default:
					GF_ASSERT(0);	//���݂��Ȃ��R�}���h
					break;
				}
				move->anm_list++;
				break;
			}
			
		#if 0	//�X�N���v�g��OBJ�A�j���̓}�b�v��l���Ƃ̓����蔻��𖳎����邽��
				//�A�N�V�����R�}���h���g�p����悤�ɕύX
			req.cmd = AnmCmdTbl[code].cmd;
			req.way = AnmCmdTbl[code].way;
			req.playid = move->playid;
			WF2DMAP_REQCMDQSysCmdPush(move->reqcmd_q, &req);
		#else
			FSSC_Sub_ActCmdCreate(&act, move->fss_actor->objwk, move->playid, code);
			WF2DMAP_ACTCMDQSysCmdPush(move->actcmd_q, &act);
		#endif
		
			move->num++;
			if(move->num >= num){
				move->num = 0;
				move->anm_list++;
			}
			move->seq = SEQ_DATA_CHECK;
		}
		break;
	case SEQ_END:
		if(WF2DMAP_OBJWkDataGet(move->fss_actor->objwk, WF2DMAP_OBJPM_ST) == WF2DMAP_OBJST_NONE){
			(*(move->anm_count_ptr))--;
			move->fss_actor->anime_tcb = NULL;
			sys_FreeMemoryEz(move);
			TCB_Delete(tcb);
			return;
		}
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �A�N�V�����R�}���h���쐬
 *
 * @param   act			�����
 * @param   objwk		�Ώۂ̃I�u�W�F�N�g���[�N
 * @param   playid		�F��ID
 * @param   code		�A�j���R�[�h(FC_DIR_U�AFC_WAIT_1F�A��)
 */
//--------------------------------------------------------------
void FSSC_Sub_ActCmdCreate(WF2DMAP_ACTCMD *act, WF2DMAP_OBJWK *objwk, int playid, int code)
{
	//act->playid = WF2DMAP_OBJWkDataGet(objwk, WF2DMAP_OBJPM_PLID );
	act->playid = playid;
	act->way = AnmCmdTbl[code].way;
	//act->way = WF2DMAP_OBJWkDataGet(objwk, WF2DMAP_OBJPM_WAY );
	act->cmd = AnmCmdTbl[code].cmd;
	if( (act->cmd == WF2DMAP_CMD_WALK) || (act->cmd == WF2DMAP_CMD_RUN) 
			|| (act->cmd == WF2DMAP_CMD_SLOWWALK)){
		act->pos = WF2DMAP_OBJWkLastMatrixGet(objwk);
	}else{
		act->pos = WF2DMAP_OBJWkMatrixGet(objwk);
	}
}

//==============================================================================
//	�|�P�����\��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�N�^�[�𐶐�����
 *
 * @param   fmap			
 * @param   pp				�\������|�P�����̃f�[�^
 * @param   heap_id			�q�[�vID(�֐����Ńe���|�����Ƃ��Ďg�p)
 * @param   manager_id		�A�N�^�[�����\�[�X�Ǘ�ID (POKE_DISP_MANAGER_ID_???)
 * @param   x				��΍��WX
 * @param   y				��΍��WY
 * @param   soft_pri		�\�t�g�v���C�I���e�B
 * @param   bg_pri			BG�v���C�I���e�B
 * @param   evy				�J���[�����ZEVY�l
 * @param   next_rgb		�J���[�����Z��̐F
 */
//--------------------------------------------------------------
void FSSC_Sub_PokemonActorAdd(FMAP_PTR fmap, POKEMON_PARAM *pp, int heap_id, 
	int manager_id, int x, int y, int soft_pri, int bg_pri, int evy, u16 next_rgb)
{
	CATS_SYS_PTR csp = fmap->clactsys.csp;
	CATS_RES_PTR crp = fmap->clactsys.crp;
	PALETTE_FADE_PTR pfd = fmap->pfd;
	SOFT_SPRITE_ARC  ssa;
	void *decord_buf;
	CATS_ACT_PTR cap;
	
	//�_�~�[�̃��\�[�X�f�[�^���}�l�[�W���ɓo�^
	{
		ARCHANDLE* hdl;

		hdl  = ArchiveDataHandleOpen( ARC_BATT_OBJ,  heap_id ); 

		CATS_LoadResourceCharArcH(csp, crp, hdl, POKE_OAM128K_NCGR, 0, 
			NNS_G2D_VRAM_TYPE_2DMAIN, manager_id);
		CATS_LoadResourcePlttWorkArcH(pfd, FADE_MAIN_OBJ, csp, crp, hdl, POKE_OAM128K_NCLR, 0, 
			NNS_G2D_VRAM_TYPE_2DMAIN, 1, manager_id);
		CATS_LoadResourceCellArcH(csp, crp, hdl, POKE_OAM128K_NCER, 0, manager_id);
		CATS_LoadResourceCellAnmArcH(csp, crp, hdl, POKE_OAM128K_NANR, 0, manager_id);

		ArchiveDataHandleClose( hdl );
	}

	//�A�N�^�[����
	{
		int n;
		TCATS_OBJECT_ADD_PARAM_S coap;
		
		MI_CpuClear8(&coap, sizeof(TCATS_OBJECT_ADD_PARAM_S));
		coap.x		= x;
		coap.y		= y;
		coap.z		= 0;
		coap.anm	= 0;
		coap.pri	= soft_pri;
		coap.pal	= 0;
		coap.d_area = NNS_G2D_VRAM_TYPE_2DMAIN;
		coap.bg_pri = bg_pri;
		coap.vram_trans = 0;
		for (n = 0; n < CLACT_U_RES_MAX; n++){
			coap.id[ n ] = manager_id;
		}
		cap = CATS_ObjectAdd_S(csp, crp, &coap);
		CATS_ObjectUpdateCap(cap);
	}

	//�|�P�����O���t�B�b�N�f�[�^�����[�N�ɓǂݍ���
	{
		u32 personal_rnd, monsno;

		decord_buf = sys_AllocMemory(heap_id, POKE_TEX_SIZE);
		personal_rnd = PokeParaGet(pp, ID_PARA_personal_rnd, NULL);
		monsno = PokeParaGet(pp, ID_PARA_monsno, NULL);
		
		PokeGraArcDataGetPP(&ssa, pp, PARA_FRONT);
		
		Ex_ChangesInto_OAM_from_PokeTex(ssa.arc_no, ssa.index_chr, heap_id, 
			POKE_TEX_X, POKE_TEX_Y, POKE_TEX_W, POKE_TEX_H, decord_buf, 
			personal_rnd, FALSE, PARA_FRONT, monsno);
	}

	//�_�~�[�̃��\�[�X�ɐ����f�[�^��]�����Ȃ���
	{
		NNSG2dImageProxy * image;
		NNSG2dImagePaletteProxy* palette;
		int pal_no;
		
		//�L�����N�^
		image = CLACT_ImageProxyGet(cap->act);
		DC_FlushRange(decord_buf, sizeof(POKE_TEX_SIZE));
		GX_LoadOBJ(decord_buf, 
			image->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN], POKE_TEX_SIZE);
		
		//�p���b�g
		palette = CLACT_PaletteProxyGet(cap->act);
		pal_no = GetPlttProxyOffset(palette, NNS_G2D_VRAM_TYPE_2DMAIN);
		PaletteWorkSet_Arc(pfd, ssa.arc_no, ssa.index_pal, heap_id, 
			FADE_MAIN_OBJ, 0x20, pal_no*16);
		//�J���[�����Z
		if(evy > 0){
			SoftFadePfd(pfd, FADE_MAIN_OBJ, pal_no * 16, 16, evy, next_rgb);
		}
	}

	sys_FreeMemoryEz(decord_buf);
	GF_ASSERT(fmap->poke_cap[manager_id - POKE_DISP_MANAGER_ID_START] == NULL);
	fmap->poke_cap[manager_id - POKE_DISP_MANAGER_ID_START] = cap;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�N�^�[���폜����
 *
 * @param   fmap			
 * @param   manager_id		�}�l�[�W��ID(POKE_DISP_MANAGER_ID_???)
 */
//--------------------------------------------------------------
void FSSC_Sub_PokemonDispDelete(FMAP_PTR fmap, int manager_id)
{
	//�A�N�^�[�폜
	CATS_ActorPointerDelete_S(fmap->poke_cap[manager_id - POKE_DISP_MANAGER_ID_START]);
	fmap->poke_cap[manager_id - POKE_DISP_MANAGER_ID_START] = NULL;
	
	//���\�[�X�폜
	CATS_FreeResourceChar(fmap->clactsys.crp, manager_id);
	CATS_FreeResourcePltt(fmap->clactsys.crp, manager_id);
	CATS_FreeResourceCell(fmap->clactsys.crp, manager_id);
	CATS_FreeResourceCellAnm(fmap->clactsys.crp, manager_id);
}


//==============================================================================
//	�|�P�����A�C�R��
//==============================================================================
///�|�P�����A�C�R���F�A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S PokeIconObjParam = {
	0, 0, 0,		//x, y, z
	0, 100, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		FR_CHARID_POKEICON_0,				//�L����
		FR_PLTTID_POKEICON,				//�p���b�g
		FR_CELLID_POKEICON,				//�Z��
		FR_CELLANMID_POKEICON,				//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	1,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�A�C�e���A�C�R���F�A�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S ItemIconObjParam = {
	0, 0, 0,		//x, y, z
	0, 99, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		FR_CHARID_ITEMICON,				//�L����
		FR_PLTTID_ITEMICON,				//�p���b�g
		FR_CELLID_ITEMICON,				//�Z��
		FR_CELLANMID_ITEMICON,				//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	1,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�C�R���̋��ʃ��\�[�X��o�^
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
void FSSC_Sub_PokeIconCommonResourceSet(FMAP_PTR fmap)
{
	//�p���b�g
	CATS_LoadResourcePlttWorkArc(fmap->pfd, FADE_MAIN_OBJ, fmap->clactsys.csp, fmap->clactsys.crp,
		ARC_POKEICON, PokeIconPalArcIndexGet(), 0, POKEICON_PAL_MAX, 
		NNS_G2D_VRAM_TYPE_2DMAIN, FR_PLTTID_POKEICON);
	//�Z��
	CATS_LoadResourceCellArc(fmap->clactsys.csp, fmap->clactsys.crp, ARC_POKEICON, 
		PokeIconAnmCellArcIndexGet(), 0, FR_CELLID_POKEICON);
	//�Z���A�j��
	CATS_LoadResourceCellAnmArc(fmap->clactsys.csp, fmap->clactsys.crp, ARC_POKEICON, 
		PokeIconAnmCellAnmArcIndexGet(), 0, FR_CELLANMID_POKEICON);
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�C�R���̋��ʃ��\�[�X��j������
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
void FSSC_Sub_PokeIconCommonResourceFree(FMAP_PTR fmap)
{
	CATS_FreeResourceCell(fmap->clactsys.crp, FR_CELLID_POKEICON);
	CATS_FreeResourceCellAnm(fmap->clactsys.crp, FR_CELLANMID_POKEICON);
	CATS_FreeResourcePltt(fmap->clactsys.crp, FR_PLTTID_POKEICON);
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�C�R���A�N�^�[�𐶐�(�L�����N�^�̃��\�[�X�o�^������)
 *
 * @param   fmap		
 * @param   pp			��������|�P�����ւ̃|�C���^
 * @param   pos			�Ǘ��ԍ�
 * @param   x			�\�����WX
 * @param   y			�\�����WY
 *
 * @retval  ���������A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
CATS_ACT_PTR Frontier_PokeIconAdd(FMAP_PTR fmap, POKEMON_PARAM *pp, int pos, int x, int y)
{
	CATS_ACT_PTR cap;
	TCATS_OBJECT_ADD_PARAM_S obj_param;
	
	GF_ASSERT(pos < (FR_CHARID_POKEICON_MAX - FR_CHARID_POKEICON_0));
	
	OS_Printf( "pos = %d\n", (FR_CHARID_POKEICON_0 + pos) );

#if 1
	//�L�������\�[�X	���L�������\�[�X�ƃA�N�^�[��pos���Ɍʓo�^
	CATS_LoadResourceCharArcModeAdjustAreaCont(
		fmap->clactsys.csp, fmap->clactsys.crp, ARC_POKEICON, 
		PokeIconCgxArcIndexGetByPP(pp), 0, NNS_G2D_VRAM_TYPE_2DMAIN, FR_CHARID_POKEICON_0 + pos);
#else
	//�L�������\�[�X	���L�������\�[�X�ƃA�N�^�[��pos���Ɍʓo�^
	CATS_LoadResourceCharArc(fmap->clactsys.csp, fmap->clactsys.crp, ARC_POKEICON, 
		PokeIconCgxArcIndexGetByPP(pp), 0, NNS_G2D_VRAM_TYPE_2DMAIN, FR_CHARID_POKEICON_0 + pos);
#endif

	//�A�N�^�[�o�^
	obj_param = PokeIconObjParam;
	obj_param.id[CLACT_U_CHAR_RES] += pos;
	obj_param.x		= x;
	obj_param.y		= y;
	obj_param.pri	= ACT_SOFTPRI_ROULETTE_POKE;
	cap = CATS_ObjectAdd_S(fmap->clactsys.csp, fmap->clactsys.crp, &obj_param);
	//�p���b�g�؂�ւ�
	CLACT_PaletteOffsetChgAddTransPlttNo(cap->act, PokeIconPalNumGetByPP(pp));
	
	CATS_ObjectUpdateCap(cap);
	return cap;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�C�R���̍폜���s��(�L�����N�^���\�[�X�̉�����s��)
 *
 * @param   fmap		
 * @param   cap			�폜����|�P�����A�C�R���A�N�^�[�ւ̃|�C���^
 * @param   pos			�Ǘ��ԍ�
 */
//--------------------------------------------------------------
void FSSC_Sub_PokeIconDel(FMAP_PTR fmap, CATS_ACT_PTR cap, int pos)
{
	CATS_FreeResourceChar(fmap->clactsys.crp, FR_CHARID_POKEICON_0 + pos);
	CATS_ActorPointerDelete_S(cap);
}

//--------------------------------------------------------------
/**
 * @brief   �A�C�e���A�C�R��(���X�g�p)�̃��\�[�X�o�^
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
void FSSC_Sub_ItemIconLoad(FMAP_PTR fmap)
{
	ARCHANDLE* hdl;
	
	hdl = ArchiveDataHandleOpen( ARC_PLIST_GRA, HEAPID_FRONTIERMAP); 
	
	// �p���b�g
	CATS_LoadResourcePlttWorkArcH(
		fmap->pfd, FADE_MAIN_OBJ, fmap->clactsys.csp, fmap->clactsys.crp, hdl,
		Pokelist_ItemIconPalArcGet(), 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, FR_PLTTID_ITEMICON);
	// �Z��
	CATS_LoadResourceCellArcH(
		fmap->clactsys.csp, fmap->clactsys.crp, hdl, 
		Pokelist_ItemIconCellArcGet(), 0, FR_CELLID_ITEMICON );
	// �Z���A�j��
	CATS_LoadResourceCellAnmArcH(
		fmap->clactsys.csp, fmap->clactsys.crp, hdl, 
		Pokelist_ItemIconCAnmArcGet(), 0, FR_CELLANMID_ITEMICON );
	// �L����
//	CATS_LoadResourceCharArcH(
//		fmap->clactsys.csp, fmap->clactsys.crp, hdl,
//		Pokelist_ItemIconCgxArcGet(), 0, NNS_G2D_VRAM_TYPE_2DMAIN, FR_CHARID_ITEMICON);
	CATS_LoadResourceCharArcModeAdjustAreaCont(
		fmap->clactsys.csp, fmap->clactsys.crp, ARC_PLIST_GRA,
		Pokelist_ItemIconCgxArcGet(), 0, NNS_G2D_VRAM_TYPE_2DMAIN, FR_CHARID_ITEMICON);
		
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------
/**
 * @brief   �A�C�e���A�C�R��(���X�g�p)�̃��\�[�X���
 *
 * @param   fmap		
 */
//--------------------------------------------------------------
void FSSC_Sub_ItemIconFree(FMAP_PTR fmap)
{
	CATS_FreeResourceChar(fmap->clactsys.crp, FR_CHARID_ITEMICON);
	CATS_FreeResourceCell(fmap->clactsys.crp, FR_CELLID_ITEMICON);
	CATS_FreeResourceCellAnm(fmap->clactsys.crp, FR_CELLANMID_ITEMICON);
	CATS_FreeResourcePltt(fmap->clactsys.crp, FR_PLTTID_ITEMICON);
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����A�C�R���A�N�^�[�𐶐�(�L�����N�^�̃��\�[�X�o�^������)
 *
 * @param   fmap		
 * @param   x			�\�����WX
 * @param   y			�\�����WY
 *
 * @retval  ���������A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
CATS_ACT_PTR Frontier_ItemIconAdd(FMAP_PTR fmap, int x, int y)
{
	CATS_ACT_PTR cap;
	TCATS_OBJECT_ADD_PARAM_S obj_param;
	
	//�A�N�^�[�o�^
	obj_param = ItemIconObjParam;
	obj_param.x		= x;
	obj_param.y		= y;
	obj_param.pri	= ACT_SOFTPRI_ROULETTE_ITEM;
	cap = CATS_ObjectAdd_S(fmap->clactsys.csp, fmap->clactsys.crp, &obj_param);
	
	CATS_ObjectUpdateCap(cap);
	return cap;
}

//--------------------------------------------------------------
/**
 * @brief   �A�C�e���A�C�R���̍폜���s��
 *
 * @param   fmap		
 * @param   cap			�폜����A�C�e���A�C�R���A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
void FSSC_Sub_ItemIconDel(FMAP_PTR fmap, CATS_ACT_PTR cap)
{
	CATS_ActorPointerDelete_S(cap);
}

//--------------------------------------------------------------
/**
 * @brief   �ΐ�O�g���[�i�[�f�[�^�Z�b�g
 *
 * @param   
 */
//--------------------------------------------------------------
static BOOL TowerTalkMsgWait(FSS_TASK * core);
void FrontierTalkMsgSub( FSS_TASK* core, u16* msg );
void FrontierTalkMsgSub2( FSS_TASK* core, u16* msg, u32 datID );

void FrontierTalkMsgSub( FSS_TASK* core, u16* msg )
{
	FrontierTalkMsgSub2( core, msg, NARC_msg_tower_trainerpl_dat );		//�v���`�i
	return;
}

void FrontierTalkMsgSub2( FSS_TASK* core, u16* msg, u32 datID )
{
	u8 spd;
	MSGDATA_MANAGER * man;
	FRONTIER_EX_PARAM* ex_param = Frontier_ExParamGet( core->fss->fmain );

	if(msg[0] == 0xFFFF){	//ROMMSG----------------------------------------------

		man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, datID, HEAPID_EVENT );

		//ScrTalkMsg(core, man,msg[1], 1, NULL);
		FSSC_Sub_ScrTalkMsg(core->fss, man, msg[1], 1, NULL);
		MSGMAN_Delete(man);
	}else{					//�ȈՉ�b--------------------------------------------

		spd = CONFIG_GetMsgPrintSpeed( SaveData_GetConfig(ex_param->savedata) );
		FSTalkMsgPMSParam(core->fss,spd,msg[0],msg[1],msg[2],msg[3],1);
	}

	FSST_SetWait( core, TowerTalkMsgWait );
	return;
}

static BOOL TowerTalkMsgWait(FSS_TASK * core)
{
	if(GF_MSG_PrintEndCheck(core->fss->msg_talk_index) == 0){
		return 1;
	}
	return 0;
}


//--------------------------------------------------------------
/**
 * @brief   �n�k���s�^�X�N
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		FMAP_SHAK_WORK
 */
//--------------------------------------------------------------
void FSSC_Sub_ShakeMove(TCB_PTR tcb, void *work)
{
	FMAP_SHAKE_WORK *shake = work;
	
	shake->wait++;
	if(shake->wait > shake->wait_max){
		shake->wait = 0;
		shake->loop--;
		if(shake->loop < 0){
			TCB_Delete(tcb);
			MI_CpuClear8(shake, sizeof(FMAP_SHAKE_WORK));
			return;
		}
		shake->shake_x = -shake->shake_x;
		shake->shake_y = -shake->shake_y;
	}
}

//--------------------------------------------------------------
/**
 * @brief   Window���s�^�X�N
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		FMAP_WND_WORK
 */
//--------------------------------------------------------------
void FSSC_Sub_Window(TCB_PTR tcb, void *work)
{
	FMAP_WND_WORK *wnd = work;
	
	if(wnd->wait <= 0){
		if(wnd->on_off == TRUE){
			G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | 
				GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, TRUE);

			G2_SetWndOutsidePlane(	GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | 
				GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 |
				GX_WND_PLANEMASK_OBJ, TRUE );

			G2_SetWnd0Position(wnd->x1, wnd->y1, wnd->x2, wnd->y2);
			GX_SetVisibleWnd(GX_WNDMASK_W0);
		}
		else{
			GX_SetVisibleWnd(GX_WNDMASK_NONE);
		}
		
		TCB_Delete(tcb);
		MI_CpuClear8(wnd, sizeof(FMAP_WND_WORK));
		return;
	}
	else{
		wnd->wait--;
	}
}
