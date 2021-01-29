//============================================================================================
/**
 * @file	wifi_p2pmatch.c
 * @bfief	WIFIP2P�}�b�`���O���
 * @author	k.ohno
 * @date	06.04.07
 */
//============================================================================================

#include "common.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/pm_str.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_lobby.h"
#include "msgdata/msg_wifi_system.h"
#include "application\namein.h"
#include "system/wipe.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "system/fontoam.h"
#include "system/window.h"
#include "gflib/msg_print.h"
#include "gflib/touchpanel.h"
#include "poketool/monsno.h"
#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "application/wifi_p2pmatch.h"
#include "application/wifi_lobby/wflby_def.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "msgdata/msg_opening_name.h"
#include "itemtool/itemsym.h"
#include "system/brightness.h"
#include "system/pmfprint.h"
#include "system/pm_overlay.h"
#include "system/pm_rtc.h"

#include "wifi/dwc_rap.h"
#include "wifi_p2pmatch_local.h"
#include "wifi_p2pmatch_se.h"
#include "wifi/dwc_rapfriend.h"

#include "field/sysflag.h"

#include "communication/communication.h"
#include "communication/comm_state.h"
#include "msgdata/msg_ev_win.h"
#include "savedata/zukanwork.h"
#include "savedata/wifihistory.h"
#include "savedata/config.h"
#include "savedata/poruto.h"
#include "savedata/frontier_savedata.h"
#include "itemtool/myitem.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "system/snd_tool.h"  //sndTOOL
#include "savedata/email_savedata.h"

#include "wifip2pmatch.naix"			// �O���t�B�b�N�A�[�J�C�u��`

#include "include/system/pm_debug_wifi.h"

#include "application/connect_anm.h"

#include "comm_command_wfp2pmf_func.h"

// WIFI2DMAP�V�X�e���I�[�o�[���C
FS_EXTERN_OVERLAY(wifi_2dmapsys);

// WIFI�@�ΐ�AUTOӰ�ރf�o�b�N
#ifdef _WIFI_DEBUG_TUUSHIN
extern WIFI_DEBUG_BATTLE_WK WIFI_DEBUG_BATTLE_Work;
#endif	//_WIFI_DEBUG_TUUSHIN


//-------------------------------------
///	�}�b�`���O�p�g���q�[�v�T�C�Y
//=====================================
#define WIFI_P2PMATCH_EXHEAP_SIZE	( 0x10000 )
//#define WIFI_P2PMATCH_EXHEAP_SIZE	( 0x5000 )




///	�a�f�p���b�g��` 2005/09/15
//
//	0 �` 5  : �t�B�[���h�}�b�v�a�f�p
//  6       : �V��
//  7       : �n���E�C���h�E
//  8       : �ŔE�C���h�E�i�}�b�v�A�W���Ȃǁj
//  9       : �ŔE�C���h�E�i�g�A�t�H���g�j
//  10      : ���b�Z�[�W�E�C���h�E
//  11      : ���j���[�E�C���h�E
//  12      : ���b�Z�[�W�t�H���g
//  13      : �V�X�e���t�H���g
//	14		: ���g�p�i���[�J���C�Y�p�j
//	15		: �f�o�b�O�p�i���i�łł͖��g�p�j
#define FLD_WEATHER_PAL      (  6 )			//  �V��
#define FLD_PLACENAME_PAL    (  7 )         //  �n���E�C���h�E
#define FLD_BOARD1FRAME_PAL  (  8 )         //  �ŔE�C���h�E�i�}�b�v�A�W���Ȃǁj
#define FLD_BOARD2FRAME_PAL  (  9 )         //  �ŔE�C���h�E�i�g�A�t�H���g�j
#define FLD_MESFRAME_PAL     ( 10 )         //  ���b�Z�[�W�E�C���h�E
#define FLD_MENUFRAME_PAL    ( 11 )         //  ���j���[�E�C���h�E
#define FLD_MESFONT_PAL      ( 12 )         //  ���b�Z�[�W�t�H���g
#define FLD_SYSFONT_PAL	     ( 13 )         //  �V�X�e���t�H���g
#define FLD_LOCALIZE_PAL     ( 14 )         //	���g�p�i���[�J���C�Y�p�j
#define FLD_DEBUG_PAL        ( 15 )         //	�f�o�b�O�p�i���i�łł͖��g�p�j

/*********************************************************************************************
	���C����ʂ�CGX����U��		2006/01/12

		�E�B���h�E�g	�F	409 - 511
			��b�A���j���[�A�n���A�Ŕ�

		BMP�E�B���h�E�P	�F	297 - 408
			��b�i�ő�j�A�ŔA�c��{�[����

		BMP�E�B���h�E�Q	�F	55 - 296
			���j���[�i�ő�j�A�͂�/�������A�n��

*********************************************************************************************/

/*********************************************************************************************
	�E�B���h�E�g
*********************************************************************************************/
// ��b�E�B���h�E�L����
#define	TALK_WIN_CGX_SIZE	( 18+12 )
#define	TALK_WIN_CGX_NUM	( 512 - TALK_WIN_CGX_SIZE )
#define	TALK_WIN_PAL		( 10 )

// ���j���[�E�B���h�E�L����
#define	MENU_WIN_CGX_SIZE	( 9 )
#define	MENU_WIN_CGX_NUM	( TALK_WIN_CGX_NUM - MENU_WIN_CGX_SIZE )
#define	MENU_WIN_PAL		( 11 )

// �n���E�B���h�E�L����
#define	PLACE_WIN_CGX_SIZE	( 10 )
#define	PLACE_WIN_CGX_NUM	( MENU_WIN_CGX_NUM - PLACE_WIN_CGX_SIZE )
#define	PLACE_WIN_PAL		( 7 )

// �ŔE�B���h�E�L����
#define	BOARD_WIN_CGX_SIZE	( 18+12 + 24 )
#define	BOARD_WIN_CGX_NUM	( PLACE_WIN_CGX_NUM - BOARD_WIN_CGX_SIZE )
#define	BOARD_WIN_PAL		( FLD_BOARD2FRAME_PAL )

/*********************************************************************************************
	BMP�E�B���h�E
*********************************************************************************************/
// ��b�E�B���h�E�i���C���j
#define	FLD_MSG_WIN_PX		( 2 )
#define	FLD_MSG_WIN_PY		( 19 )
#define	FLD_MSG_WIN_SX		( 27 )
#define	FLD_MSG_WIN_SY		( 4 )
#define	FLD_MSG_WIN_PAL		( FLD_MESFONT_PAL )
#define	FLD_MSG_WIN_CGX		( BOARD_WIN_CGX_NUM - ( FLD_MSG_WIN_SX * FLD_MSG_WIN_SY ) )

// �ŔE�B���h�E�i���C���j�i��b�Ɠ����ʒu�i��b��菬�����j�j
#define	FLD_BOARD_WIN_PX	( 9 )
#define	FLD_BOARD_WIN_PY	( 19 )
#define	FLD_BOARD_WIN_SX	( 20 )
#define	FLD_BOARD_WIN_SY	( 4 )
#define	FLD_BOARD_WIN_PAL	( FLD_BOARD2FRAME_PAL )
#define	FLD_BOARD_WIN_CGX	( FLD_MSG_WIN_CGX )

// �c��{�[�����i��b�Ɠ����ʒu�i��b��菬�����j�j
#define	FLD_BALL_WIN_PX		( 1 )
#define	FLD_BALL_WIN_PY		( 1 )
#define	FLD_BALL_WIN_SX		( 12 )
#define	FLD_BALL_WIN_SY		( 4 )
#define	FLD_BALL_WIN_PAL	( FLD_SYSFONT_PAL )
#define	FLD_BALL_WIN_CGX	( FLD_MSG_WIN_CGX )

// ���j���[�E�B���h�E�i���C���j
#define	FLD_MENU_WIN_PX		( 20 )
#define	FLD_MENU_WIN_PY		( 1 )
#define	FLD_MENU_WIN_SX		( 11 )
#define	FLD_MENU_WIN_SY		( 22 )
#define	FLD_MENU_WIN_PAL	( FLD_SYSFONT_PAL )
#define	FLD_MENU_WIN_CGX	( FLD_MSG_WIN_CGX - ( FLD_MENU_WIN_SX * FLD_MENU_WIN_SY ) )

// �͂�/�������E�B���h�E�i���C���j�i���j���[�Ɠ����ʒu�i���j���[��菬�����j�j
#define	FLD_YESNO_WIN_PX	( 25 )
#define	FLD_YESNO_WIN_PY	( 13 )
#define	FLD_YESNO_WIN_SX	( 6 )
#define	FLD_YESNO_WIN_SY	( 4 )
#define	FLD_YESNO_WIN_PAL	( FLD_SYSFONT_PAL )
#define	FLD_YESNO_WIN_CGX	( FLD_MSG_WIN_CGX - ( FLD_YESNO_WIN_SX * FLD_YESNO_WIN_SY ) )

// �n���E�B���h�E�i���j���[�Ɠ����ʒu�i���j���[��菬�����j�j
#define FLD_PLACE_WIN_PX	( 0 )
#define FLD_PLACE_WIN_PY	( 0 )
#define	FLD_PLACE_WIN_SX	( 32 )
#define	FLD_PLACE_WIN_SY	( 3 )
#define	FLD_PLACE_WIN_CGX	( FLD_MSG_WIN_CGX - ( FLD_PLACE_WIN_SX * FLD_PLACE_WIN_SY ) )



// ���b�Z�[�W�E�B���h�E�i�T�u�j
#define	FLD_MSG_WIN_S_PX	( 2  )
#define	FLD_MSG_WIN_S_PY	( 19 )
#define	FLD_MSG_WIN_S_PAL	( FLD_MESFONT_PAL )
#define	FLD_MSG_WIN_S_CGX	( MENU_WIN_CGX_NUM - ( FLD_MSG_WIN_SX * FLD_MSG_WIN_SY ) )

// �ŔE�B���h�E�i�T�u�j
#define	FLD_BOARD_WIN_S_PX	( 9 )
#define	FLD_BOARD_WIN_S_PY	( 19 )
#define	FLD_BOARD_WIN_S_SX	( 21 )
#define	FLD_BOARD_WIN_S_SY	( 4 )
#define	FLD_BOARD_WIN_S_PAL	( FLD_BOARD2FRAME_PAL )
#define	FLD_BOARD_WIN_S_CGX	( FLD_MSG_WIN_S_CGX )

// ���j���[�E�B���h�E�i�T�u�j
#define	FLD_MENU_WIN_S_PX	( 25 )
#define	FLD_MENU_WIN_S_PY	( 1 )
#define	FLD_MENU_WIN_S_SX	( 6 )
#define	FLD_MENU_WIN_S_SY	( 16 )
#define	FLD_MENU_WIN_S_PAL	( FLD_SYSFONT_PAL )
#define	FLD_MENU_WIN_S_CGX	( MENU_WIN_CGX_NUM - ( FLD_MENU_WIN_SX * FLD_MENU_WIN_SY ) )



///	�r�b�g�}�b�v�]���֐��p��`
//------------------------------------------------------------------
#define	FBMP_TRANS_OFF	(0)
#define	FBMP_TRANS_ON	(1)
///	�����\���]���֐��p��`
//------------------------------------------------------------------
#define	FBMPMSG_WAITON_SKIPOFF	(0)
#define	FBMPMSG_WAITON_SKIPON	(1)
#define	FBMPMSG_WAITOFF			(2)

//-------------------------------------------------------------------------
///	�����\���F��`(default)	-> gflib/fntsys.h�ֈړ�
//------------------------------------------------------------------

#define WINCLR_COL(col)	(((col)<<4)|(col))

//-------------------------------------------------------------------------
///	�����\���X�s�[�h��`(default)
//------------------------------------------------------------------
#define	FBMP_MSG_SPEED_SLOW		(8)
#define	FBMP_MSG_SPEED_NORMAL	(4)
#define	FBMP_MSG_SPEED_FAST		(1)



//============================================================================================
//	�萔��`
//============================================================================================
enum {
    SEQ_IN = 0,
    SEQ_MAIN,
    SEQ_OUT,
};

enum {
    _MENU_LIST,
    _MENU_INPUT,
    _MENU_MYCODE,
    _MENU_EXIT,
};

typedef enum {
    _CLACT_UP_CUR,
    _CLACT_DOWN_CUR,
    _CLACT_LINE_CUR,
    _CLACT_VCT_STOP,
    _CLACT_VCT_MOVE,
} _OAM_ANIM_E;

typedef struct{  // �X�N���[���pRECT�\����
    u8 lt_x;
    u8 lt_y;
    u8 rb_x;
    u8 rb_y;
} _SCR_RECT;

#define WF_CLACT_WKNUM	( 8 )
#define WF_CLACT_RESNUM	( 1 )
#define WF_FONTOAM_NUM	( 1 )

#define _PRINTTASK_MAX (8)
#define _TIMING_GAME_CHECK  (13)// �Ȃ���������
#define _TIMING_GAME_CHECK2  (14)// �Ȃ���������
#define _TIMING_GAME_START  (15)// �^�C�~���O�����낦��
#define _TIMING_GAME_START2  (18)// �^�C�~���O�����낦��
#define _TIMING_BATTLE_END  (16)// �^�C�~���O�����낦��

#define _RECONECTING_WAIT_TIME (20)  //�Đڑ�����


// ���[�U�[�\���ʂ̐ݒ�
#define WIFIP2PMATCH_PLAYER_DISP_X	( 1 )
#define WIFIP2PMATCH_PLAYER_DISP_Y	( 1 )
#define WIFIP2PMATCH_PLAYER_DISP_SIZX	( 28 )
#define WIFIP2PMATCH_PLAYER_DISP_SIZY	( 2 )
#define WIFIP2PMATCH_PLAYER_DISP_NAME_X	( 32 )
#define WIFIP2PMATCH_PLAYER_DISP_ST_X	( 102 )
#define WIFIP2PMATCH_PLAYER_DISP_WINSIZ	(WIFIP2PMATCH_PLAYER_DISP_SIZX*WIFIP2PMATCH_PLAYER_DISP_SIZY)


//FRAME3
#define	_CGX_BOTTOM	( 512 )							// ��b�E�B���h�E�L����
#define _CGX_TITLE_BOTTOM	(_CGX_BOTTOM - 18*3)	// �^�C�g���E�B���h�E
#define _CGX_USER_BOTTOM	(_CGX_TITLE_BOTTOM - WIFIP2PMATCH_PLAYER_DISP_WINSIZ)	// ���[�U�[�E�B���h�E

// FRAME1�ɓ]�����郆�[�U�[�f�[�^�w�i
#define _CGX_USET_BACK_BOTTOM	( MENU_WIN_CGX_NUM - WIFIP2PMATCH_PLAYER_DISP_WINSIZ )

// FRAME1�ɓ]������ICON�f�[�^
#define PLAYER_DISP_ICON_PLTTOFS	(8)
#define PLAYER_DISP_ICON_PLTTOFS_SUB (13)
#define PLAYER_DISP_ICON_PLTTNUM	(2)
#define PLAYER_DISP_ICON_CG_SIZX	(12)
#define PLAYER_DISP_ICON_CG_SIZY	(4)
#define PLAYER_DISP_ICON_CGX		( 0 )
#define PLAYER_DISP_ICON_SCRN_X	( 2 )
#define PLAYER_DISP_ICON_SCRN_Y	( 2 )
#define PLAYER_DISP_ICON_POS_X	( 2 )
#define PLAYER_DISP_ICON_POS_Y	( 1 )
#define PLAYER_DISP_VCTICON_POS_X	( 26 )
#define PLAYER_DISP_VCTICON_POS_Y	( 1 )
enum{
	PLAYER_DISP_ICON_IDX_NONE00,
	PLAYER_DISP_ICON_IDX_VCTNOT,
	PLAYER_DISP_ICON_IDX_VCTBIG,
	PLAYER_DISP_ICON_IDX_VCT,
	PLAYER_DISP_ICON_IDX_NORMAL,
	PLAYER_DISP_ICON_IDX_UNK,
	PLAYER_DISP_ICON_IDX_FIGHT,
	PLAYER_DISP_ICON_IDX_CHANGE,
	PLAYER_DISP_ICON_IDX_NONE,
	PLAYER_DISP_ICON_IDX_POFIN,
	PLAYER_DISP_ICON_IDX_MINIGAME,
	PLAYER_DISP_ICON_IDX_FRONTIER,
	PLAYER_DISP_ICON_IDX_NUM,
};


// ��b�E�B���h�E	FRAME2
#define COMM_MESFRAME_PAL     ( 10 )         //  ���b�Z�[�W�E�C���h�E
#define COMM_MENUFRAME_PAL    ( 11 )         //  ���j���[�E�C���h�E
#define COMM_MESFONT_PAL      ( 12 )         //  ���b�Z�[�W�t�H���g
#define COMM_SYSFONT_PAL	  ( 13 )         //  �V�X�e���t�H���g
#define	COMM_TALK_WIN_CGX_SIZE	( 18+12 )
#define	COMM_TALK_WIN_CGX_NUM	( 512 - COMM_TALK_WIN_CGX_SIZE)
#define	COMM_MSG_WIN_PX		( 2 )
#define	COMM_MSG_WIN_PY		( 19 )
#define	COMM_MSG_WIN_SX		( 27 )
#define	COMM_MSG_WIN_SY		( 4 )
#define	COMM_MSG_WIN_PAL		( COMM_MESFONT_PAL )
#define	COMM_MSG_WIN_CGX		( (COMM_TALK_WIN_CGX_NUM - 73) - ( COMM_MSG_WIN_SX * COMM_MSG_WIN_SY ) )

#define	COMM_SYS_WIN_PX		( 4 )
#define	COMM_SYS_WIN_PY		( 4 )
#define	COMM_SYS_WIN_SX		( 23 )
#define	COMM_SYS_WIN_SY		( 16 )
#define	COMM_SYS_WIN_PAL		( COMM_MESFONT_PAL )
#define	COMM_SYS_WIN_CGX		( MENU_WIN_CGX_NUM - ( COMM_SYS_WIN_SX * COMM_SYS_WIN_SY ) )	// �ʐM�V�X�e���E�B���h�E�]����


// YesNoWin
#define YESNO_WIN_CGX	((COMM_SYS_WIN_CGX - (FLD_YESNO_WIN_SX*FLD_YESNO_WIN_SY)))
#define FRAME1_YESNO_WIN_CGX	((_CGX_USET_BACK_BOTTOM - (FLD_YESNO_WIN_SX*FLD_YESNO_WIN_SY)))


// ���C�����b�Z�[�W�L����
#define _NUKI_FONT_PALNO  (13)
#define	_COL_N_BLACK	( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )		// �t�H���g�J���[�F��
#define	_COL_N_WHITE	( GF_PRINTCOLOR_MAKE( 15, 14, 0 ) )		// �t�H���g�J���[�F��
#define	_COL_N_RED	    ( GF_PRINTCOLOR_MAKE( 3, 4, 0 ) )		// �t�H���g�J���[�F��
#define	_COL_N_BLUE	    ( GF_PRINTCOLOR_MAKE( 5, 6, 0 ) )		// �t�H���g�J���[�F��
#define	_COL_N_GRAY  	( GF_PRINTCOLOR_MAKE( 2, 14, 0 ) )		// �t�H���g�J���[�F�D

// ���O�\��BMP�i���ʁj
#define WIFIP2PMATCH_NAME_BMP_W	 ( 16 )
#define WIFIP2PMATCH_NAME_BMP_H	 (  2 )
#define WIFIP2PMATCH_NAME_BMP_SIZE (WIFIP2PMATCH_NAME_BMP_W * WIFIP2PMATCH_NAME_BMP_H)

// ��b�E�C���h�E�\���ʒu��`
#define WIFIP2PMATCH_TALK_X		(  2 )
#define WIFIP2PMATCH_TALK_Y		(  19 )

#define WIFIP2PMATCH_TITLE_X		(   3  )
#define WIFIP2PMATCH_TITLE_Y		(   1  )
#define WIFIP2PMATCH_TITLE_W		(  26  )
#define WIFIP2PMATCH_TITLE_H		(   2  )


#define WIFIP2PMATCH_MSG_WIN_OFFSET 		(1 + TALK_WIN_CGX_SIZ     + MENU_WIN_CGX_SIZ)
#define WIFIP2PMATCH_TITLE_WIN_OFFSET		( WIFIP2PMATCH_MSG_WIN_OFFSET   + FLD_MSG_WIN_SX*FLD_MSG_WIN_SY )
#define WIFIP2PMATCH_NAME_WIN_OFFSET		( WIFIP2PMATCH_TITLE_WIN_OFFSET + WIFIP2PMATCH_TITLE_W*WIFIP2PMATCH_TITLE_H )
#define WIFIP2PMATCH_YESNO_WIN_OFFSET		( WIFIP2PMATCH_NAME_WIN_OFFSET  + WIFIP2PMATCH_NAME_BMP_SIZE*5 )


// VIEW��
enum{
	MCV_BTTN_FRIEND_TYPE_NONE,	// �ݒ肵�Ă��Ȃ�
	MCV_BTTN_FRIEND_TYPE_RES,	// �\��
	MCV_BTTN_FRIEND_TYPE_IN,		// �o�^�ς�
	MCV_BTTN_FRIEND_TYPE_MAX,	// �^�C�v�ő�l
};
#define MCV_PAL_BACK		( 0 )	// �w�i�p���b�g�̊J�n�ʒu
#define MCV_PAL_FRMNO		( 0 )	// �w�i�̃o�b�g�p���b�g�J�n�ʒu
#define MCV_PAL_BTTN		( 4 )	// �{�^���p���b�g�̊J�n�ʒu
enum{
	MCV_PAL_BACK_0 = 0,
	MCV_PAL_BACK_1,
	MCV_PAL_BACK_2,
	MCV_PAL_BACK_3,
	MCV_PAL_BACK_NUM,

	MCV_PAL_BTTN_GIRL = 0,
	MCV_PAL_BTTN_MAN,
	MCV_PAL_BTTN_NONE,
	MCV_PAL_BTTNST_GIRL,
	MCV_PAL_BTTNST_MAN,
	MCV_PAL_BTTN_NUM	// ���̂Ƃ���]���
						// MCV_PAL_BTTN+MCV_PAL_BTTN_NUM�`(PLAYER_DISP_ICON_PLTTOFS_SUB-1�܂�
};
// �A�C�R���̓]���ʒu
#define MCV_ICON_CGX	(0)
#define MCV_ICON_CGSIZ	(48)
#define MCV_ICON_PAL		(PLAYER_DISP_ICON_PLTTOFS_SUB)

#define MCV_CGX_BTTN2	(MCV_ICON_CGX+MCV_ICON_CGSIZ)	// FRAME2���[�U�[�f�[�^
#define MCV_CGX_BACK	(0)// FRAME0�w�i
#define MCV_SYSFONT_PAL	( 15 )	// �V�X�e���t�H���g
// WIN�ݒ�
#define MCV_NAMEWIN_CGX		( 1 )	// ���O�E�B���h�E�J�n�ʒu
#define MCV_NAMEWIN_DEFX	( 4 )	// ��{�ʒu
#define MCV_NAMEWIN_DEFY	( 1 )
#define MCV_NAMEWIN_OFSX	( 16 )	// �ʒu�̈ړ��l
#define MCV_NAMEWIN_OFSY	( 6 )
#define MCV_NAMEWIN_SIZX	( 9 )	// �T�C�Y
#define MCV_NAMEWIN_SIZY	( 3 )
#define MCV_NAMEWIN_CGSIZ	( MCV_NAMEWIN_SIZX*MCV_NAMEWIN_SIZY )	// CG�T�C�Y
#define MCV_NAMEWIN_CGALLSIZ	((MCV_NAMEWIN_CGSIZ*WCR_MAPDATA_1BLOCKOBJNUM)+MCV_NAMEWIN_CGX)// CG�����T�C�Y

#define MCV_STATUSWIN_CGX	( MCV_NAMEWIN_CGALLSIZ+1 )
#define MCV_STATUSWIN_DEFX	( 1 )	// ��{�ʒu
#define MCV_STATUSWIN_DEFY	( 1 )
#define MCV_STATUSWIN_VCHATX ( 12 )	// VCHAT�ʒu
#define MCV_STATUSWIN_OFSX	( 16 )	// �ʒu�̈ړ��l
#define MCV_STATUSWIN_OFSY	( 6 )
#define MCV_STATUSWIN_SIZX	( 2 )	// �T�C�Y
#define MCV_STATUSWIN_SIZY	( 3 )
#define MCV_STATUSWIN_CGSIZ	( MCV_STATUSWIN_SIZX*MCV_STATUSWIN_SIZY )	// CG�T�C�Y

#define MCV_USERWIN_CGX		( 1 )
#define MCV_USERWIN_X		( 1 )
#define MCV_USERWIN_Y		( 1 )
#define MCV_USERWIN_SIZX	( 30 )
#define MCV_USERWIN_SIZY	( 21 )


#define MCV_BUTTON_SIZX	( 16 )
#define MCV_BUTTON_SIZY	( 6 )
#define MCV_BUTTON_DEFX	(0)
#define MCV_BUTTON_DEFY	(0)
#define MCV_BUTTON_OFSX	(16)
#define MCV_BUTTON_OFSY	(6)

#define MCV_BUTTON_ICON_OFS_X	( 1 )
#define MCV_BUTTON_VCTICON_OFS_X	( 13 )
#define MCV_BUTTON_ICON_OFS_Y	( 1 )

#define MCV_BUTTON_FRAME_NUM	(4)	// �{�^���A�j���t���[����

enum{
	MCV_BUTTON_TYPE_GIRL,
	MCV_BUTTON_TYPE_MAN,
	MCV_BUTTON_TYPE_NONE,
};

enum{
	MCV_USERDISP_OFF,	// ���[�U�[�\��OFF
	MCV_USERDISP_INIT,	// ���[�U�[�\��������
	MCV_USERDISP_ON,	// ���[�U�[�\��ON
	MCV_USERDISP_ONEX,	// ���[�U�[�\������ON
};

// PAGE 1
#define MCV_USERD_NAME_X	( 32 )
#define MCV_USERD_NAME_Y	( 8 )
#define MCV_USERD_ST_X	( 104 )
#define MCV_USERD_ST_Y	( 8 )
#define MCV_USERD_GR_X	( 8 )
#define MCV_USERD_GR_Y	( 32 )
#define MCV_USERD_VS_X	( 8 )
#define MCV_USERD_VS_Y	( 56 )
#define MCV_USERD_VS_WIN_X	( 120 )
#define MCV_USERD_VS_WIN_Y	( 56 )
#define MCV_USERD_VS_LOS_X	( 184 )
#define MCV_USERD_TR_X		( 8 )
#define MCV_USERD_TR_Y		( 80 )
#define MCV_USERD_TRNUM_X	( 152 )
#define MCV_USERD_DAY_X		( 8 )
#define MCV_USERD_DAY_Y		( 128 )
#define MCV_USERD_DAYNUM_X	( 152 )
#define MCV_USERD_ICON_X	( 2 )
#define MCV_USERD_VCTICON_X	( 28 )
#define MCV_USERD_ICON_Y	( 2 )
#define MCV_USERD_POFIN_X		( 8 )
#define MCV_USERD_POFIN_Y		( 104 )
#define MCV_USERD_POFINNUM_X	( 152 )

// PAGE2
#define MCV_USERD_BTTW_TITLE_X		(8)
#define MCV_USERD_BTTW_TITLE_Y		(0)
#define MCV_USERD_BTTW_LAST_X		(8)
#define MCV_USERD_BTTW_LAST_Y		(24)
#define MCV_USERD_BTTW_LASTNUM_X	(64)
#define MCV_USERD_BTTW_LASTNUM_Y	(24)
#define MCV_USERD_BTTW_MAX_X		(8)
#define MCV_USERD_BTTW_MAX_Y		(48)
#define MCV_USERD_BTTW_MAXNUM_X		(64)
#define MCV_USERD_BTTW_MAXNUM_Y		(48)

// PAGE3
#define MCV_USERD_BTFC_TITLE_X			(8)
#define MCV_USERD_BTFC_TITLE_Y			(0)
#define MCV_USERD_BTFC_LV50_X			(8)
#define MCV_USERD_BTFC_LV50_Y			(24)
#define MCV_USERD_BTFC_LV50K_X			(136)
#define MCV_USERD_BTFC_LV50K_Y			(24)
#define MCV_USERD_BTFC_LV50LAST_X		(8)
#define MCV_USERD_BTFC_LV50LAST_Y		(48)
#define MCV_USERD_BTFC_LV50LASTNUM_X	(64)
#define MCV_USERD_BTFC_LV50LASTNUM_Y	(48)
#define MCV_USERD_BTFC_LV50LASTTRNUM_X	(184)
#define MCV_USERD_BTFC_LV50LASTTRNUM_Y	(48)
#define MCV_USERD_BTFC_LV50MAX_X		(8)
#define MCV_USERD_BTFC_LV50MAX_Y		(64)
#define MCV_USERD_BTFC_LV50MAXNUM_X		(64)
#define MCV_USERD_BTFC_LV50MAXNUM_Y		(64)
#define MCV_USERD_BTFC_LV50MAXTRNUM_X	(184)
#define MCV_USERD_BTFC_LV50MAXTRNUM_Y	(64)
#define MCV_USERD_BTFC_OPN_X			(8)
#define MCV_USERD_BTFC_OPN_Y			(88)
#define MCV_USERD_BTFC_OPNK_X			(136)
#define MCV_USERD_BTFC_OPNK_Y			(88)
#define MCV_USERD_BTFC_OPNLAST_X		(8)
#define MCV_USERD_BTFC_OPNLAST_Y		(112)
#define MCV_USERD_BTFC_OPNLASTNUM_X	(64)
#define MCV_USERD_BTFC_OPNLASTNUM_Y	(112)
#define MCV_USERD_BTFC_OPNLASTTRNUM_X	(184)
#define MCV_USERD_BTFC_OPNLASTTRNUM_Y	(112)
#define MCV_USERD_BTFC_OPNMAX_X		(8)
#define MCV_USERD_BTFC_OPNMAX_Y		(128)
#define MCV_USERD_BTFC_OPNMAXNUM_X		(64)
#define MCV_USERD_BTFC_OPNMAXNUM_Y		(128)
#define MCV_USERD_BTFC_OPNMAXTRNUM_X	(184)
#define MCV_USERD_BTFC_OPNMAXTRNUM_Y	(128)


// PAGE4
#define MCV_USERD_BTKS_TITLE_X		(8)
#define MCV_USERD_BTKS_TITLE_Y		(0)
#define MCV_USERD_BTKS_K_X			(136)
#define MCV_USERD_BTKS_K_Y			(24)
#define MCV_USERD_BTKS_LAST_X		(8)
#define MCV_USERD_BTKS_LAST_Y		(48)
#define MCV_USERD_BTKS_LASTNUM_X	(64)
#define MCV_USERD_BTKS_LASTNUM_Y	(48)
#define MCV_USERD_BTKS_LASTCPNUM_X	(184)
#define MCV_USERD_BTKS_LASTCPNUM_Y	(48)
#define MCV_USERD_BTKS_MAX_X		(8)
#define MCV_USERD_BTKS_MAX_Y		(80)
#define MCV_USERD_BTKS_MAXNUM_X		(64)
#define MCV_USERD_BTKS_MAXNUM_Y		(80)
#define MCV_USERD_BTKS_MAXCPNUM_X	(184)
#define MCV_USERD_BTKS_MAXCPNUM_Y	(80)

// PAGE5
#define MCV_USERD_BTST_TITLE_X		(8)
#define MCV_USERD_BTST_TITLE_Y		(0)
#define MCV_USERD_BTST_K_X			(8)
#define MCV_USERD_BTST_K_Y			(24)
#define MCV_USERD_BTST_LAST_X		(8)
#define MCV_USERD_BTST_LAST_Y		(48)
#define MCV_USERD_BTST_LASTNUM_X	(64)
#define MCV_USERD_BTST_LASTNUM_Y	(48)
#define MCV_USERD_BTST_MAX_X		(8)
#define MCV_USERD_BTST_MAX_Y		(80)
#define MCV_USERD_BTST_MAXNUM_X		(64)
#define MCV_USERD_BTST_MAXNUM_Y		(80)

// PAGE6
#define MCV_USERD_BTRT_TITLE_X		(8)
#define MCV_USERD_BTRT_TITLE_Y		(0)
#define MCV_USERD_BTRT_LAST_X		(8)
#define MCV_USERD_BTRT_LAST_Y		(24)
#define MCV_USERD_BTRT_LASTNUM_X	(64)
#define MCV_USERD_BTRT_LASTNUM_Y	(24)
#define MCV_USERD_BTRT_MAX_X		(8)
#define MCV_USERD_BTRT_MAX_Y		(48)
#define MCV_USERD_BTRT_MAXNUM_X		(64)
#define MCV_USERD_BTRT_MAXNUM_Y		(48)

// PAGE7
#define MCV_USERD_MINI_TITLE_X		(8)
#define MCV_USERD_MINI_TITLE_Y		(0)
#define MCV_USERD_MINI_BC_X			(8)		// ���܂���
#define MCV_USERD_MINI_BC_Y			(24)	
#define MCV_USERD_MINI_BCNUM_X		(136)	// ���܂���@����
#define MCV_USERD_MINI_BCNUM_Y		(24)
#define MCV_USERD_MINI_BB_X			(8)		// ���܂̂�
#define MCV_USERD_MINI_BB_Y			(48)	
#define MCV_USERD_MINI_BBNUM_X		(136)	// ���܂̂�@����
#define MCV_USERD_MINI_BBNUM_Y		(48)
#define MCV_USERD_MINI_BL_X			(8)		// �ӂ�����
#define MCV_USERD_MINI_BL_Y			(72)	
#define MCV_USERD_MINI_BLNUM_X		(136)	// �ӂ�����@����
#define MCV_USERD_MINI_BLNUM_Y		(72)

// �t�����e�B�A��\���X�N���[���`��
#define MCV_USERD_NOFR_SCRN_X		( 0x1a )
#define MCV_USERD_NOFR_SCRN_Y		( 0 )
#define MCV_USERD_NOFR_SCRN_SIZX	( 0x1 )
#define MCV_USERD_NOFR_SCRN_SIZY	( 0x1 )

//  �t�����e�B�A�^�C�v
enum{
	MCV_FRONTIOR_TOWOR,
	MCV_FRONTIOR_FACTORY,
	MCV_FRONTIOR_FACTORY100,
	MCV_FRONTIOR_CASTLE,
	MCV_FRONTIOR_STAGE,
	MCV_FRONTIOR_ROULETTE,
	MCV_FRONTIOR_NUM,
} ;

#define VRANTRANSFERMAN_NUM	(32)	// VramTransferManager�^�X�N��



enum{
	WFP2P_POFIN_RET_OK,
	WFP2P_POFIN_RET_NUTSNONE,
	WFP2P_POFIN_RET_POFINMAX,
	WFP2P_POFIN_RET_POFINCASENONE,
};

enum{
	MCV_USERD_BTTN_ANM_LEFT_PUSH = 0,
	MCV_USERD_BTTN_ANM_LEFT_RELEASE,
	MCV_USERD_BTTN_ANM_BACK_PUSH,
	MCV_USERD_BTTN_ANM_BACK_RELEASE,
	MCV_USERD_BTTN_ANM_RIGHT_PUSH,
	MCV_USERD_BTTN_ANM_RIGHT_RELEASE,
};

enum{
	MCV_USERD_BTTN_MODE_WAIT,
	MCV_USERD_BTTN_MODE_NML,
	MCV_USERD_BTTN_MODE_NOBACK,
};

#define MCV_USERD_BTTN_RESCONTID	( 30 )	// ���\�[�XID
#define MCV_USERD_BTTN_BGPRI		(0)		// BG�D�揇��
#define MCV_USERD_BTTN_PRI			(128)		// BG�D�揇��
#define MCV_USERD_BTTN_Y			(172)	// �{�^��Y�ʒu
#define MCV_USERD_BTTN_LEFT_X		(40)	// ��
#define MCV_USERD_BTTN_BACK_X		(128)	// ���ǂ�
#define MCV_USERD_BTTN_RIGHT_X		(224)	// ��
#define MCV_USERD_BTTN_FONT_X		( -18 )	// ���ǂ�@�����ʒu
#define MCV_USERD_BTTN_FONT_Y		( -8 )	// ���ǂ�@�����ʒu
#define MCV_USERD_BTTN_FONT_SIZX	( 8 )	// ���ǂ�@�����`��͈�
#define MCV_USERD_BTTN_FONT_SIZY	( 2 )	// ���ǂ�@�����`��͈�
#define MCV_USERD_BTTN_FONT_CGXOFS	( 0 )	// CGX�I�t�Z�b�g
#define MCV_USERD_BTTN_FONT_COL		( 0 )	// �J���[�p���b�g
#define MCV_USERD_BTTN_FONT_OAMPRI	( 0 )
#define MCV_USERD_BTTN_LEFT_SIZX	( 56 )	// ���T�C�Y
#define MCV_USERD_BTTN_BACK_SIZX	( 96 )	// ���ǂ�T�C�Y
#define MCV_USERD_BTTN_RIGHT_SIZX	( 56 )	// ��������
#define MCV_USERD_BTTN_SIZY			( 32 )	// �c������
#define MCV_USERD_BTTN_LEFT_HIT_X	( 8 )	// �������蔻��pX
#define MCV_USERD_BTTN_BACK_HIT_X	( 80 )	// �������蔻��pX
#define MCV_USERD_BTTN_RIGHT_HIT_X	( 192 )	// �������蔻��pX
#define MCV_USERD_BTTN_RIGHT_HIT_Y	( 160 )	// �������蔻��pY
#define MCV_USERD_BTTN_ANMPUSHOK	( 3 )	// �������Ɣ��f����{�^���̃A�j��
#define MCV_USERD_BTTN_ANMMAX		( 5 )	// �A�j�����t���[����
#define MCV_USERD_BTTN_ANMMAX_0ORG	( MCV_USERD_BTTN_ANMMAX-1 )	// �A�j�����t���[����

// FONTOAMY���W�A�j���f�[�^
static const s8 c_MCV_USER_BTTN_FONT_YANM[ MCV_USERD_BTTN_ANMMAX ] = {
	MCV_USERD_BTTN_FONT_Y, 
	MCV_USERD_BTTN_FONT_Y - 1, 
	MCV_USERD_BTTN_FONT_Y - 2, 
	MCV_USERD_BTTN_FONT_Y - 2, 
	MCV_USERD_BTTN_FONT_Y - 1, 
};

// �{�^���V�[�P���X�@PUSH�A�j��
static const u8 c_MCV_USER_BTTN_ANM_PUSH[ MCV_USERD_BTTN_NUM ] = {
	MCV_USERD_BTTN_ANM_LEFT_PUSH,
	MCV_USERD_BTTN_ANM_BACK_PUSH,
	MCV_USERD_BTTN_ANM_RIGHT_PUSH,
};

// �{�^���V�[�P���X�@RELEASE�A�j��
static const u8 c_MCV_USER_BTTN_ANM_RELEASE[ MCV_USERD_BTTN_NUM ] = {
	MCV_USERD_BTTN_ANM_LEFT_RELEASE,
	MCV_USERD_BTTN_ANM_BACK_RELEASE,
	MCV_USERD_BTTN_ANM_RIGHT_RELEASE,
};
 
static const TP_HIT_TBL	c_MCV_USER_BTTN_OAM_Hit[ MCV_USERD_BTTN_NUM ] = {
	{	// ��
		MCV_USERD_BTTN_RIGHT_HIT_Y, MCV_USERD_BTTN_RIGHT_HIT_Y+MCV_USERD_BTTN_SIZY,
		MCV_USERD_BTTN_LEFT_HIT_X, MCV_USERD_BTTN_LEFT_HIT_X+MCV_USERD_BTTN_LEFT_SIZX,
	},
	{	// ���ǂ�
		MCV_USERD_BTTN_RIGHT_HIT_Y, MCV_USERD_BTTN_RIGHT_HIT_Y+MCV_USERD_BTTN_SIZY,
		MCV_USERD_BTTN_BACK_HIT_X, MCV_USERD_BTTN_BACK_HIT_X+MCV_USERD_BTTN_BACK_SIZX,
	},
	{	// ��
		MCV_USERD_BTTN_RIGHT_HIT_Y, MCV_USERD_BTTN_RIGHT_HIT_Y+MCV_USERD_BTTN_SIZY,
		MCV_USERD_BTTN_RIGHT_HIT_X, MCV_USERD_BTTN_RIGHT_HIT_X+MCV_USERD_BTTN_RIGHT_SIZX,
	},
};
enum{
	MCV_USERD_BTTN_RET_NONE,	// ���̔������Ȃ�
	MCV_USERD_BTTN_RET_LEFT,	// ���������ꂽ
	MCV_USERD_BTTN_RET_BACK,	// ���ǂ邪�����ꂽ
	MCV_USERD_BTTN_RET_RIGHT,	// �E�������ꂽ
};

#ifdef WFP2P_DEBUG	// �l�������ς��o��
//#define WFP2PM_MANY_OBJ
#endif


static int _seqBackup;

//============================================================================================
//	�v���g�^�C�v�錾
//============================================================================================
static _WIFI_MACH_STATUS* WifiFriendMatchStatusGet( WIFIP2PMATCH_WORK* wk, u32 idx );
static u8 WifiDwc_getFriendStatus( int idx );

static BOOL WifiP2PMatch_CommWifiBattleStart( WIFIP2PMATCH_WORK* wk, int friendno, int status );

/*** �֐��v���g�^�C�v ***/
static void VBlankFunc( void * work );
static void VramBankSet(void);
static void BgInit( GF_BGL_INI * ini );
static void InitWork( WIFIP2PMATCH_WORK *wk );
static void FreeWork( WIFIP2PMATCH_WORK *wk );
static void BgExit( GF_BGL_INI * ini );
static void BgGraphicSet( WIFIP2PMATCH_WORK * wk, ARCHANDLE* p_handle );
static void char_pltt_manager_init(void);
static void InitCellActor(WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle);
static void SetCellActor(WIFIP2PMATCH_WORK *wk);
static void BmpWinInit(WIFIP2PMATCH_WORK *wk, PROC* proc);
static void MainMenuMsgInit(WIFIP2PMATCH_WORK *wk);
static void SetCursor_Pos( CLACT_WORK_PTR act, int x, int y );
static void WindowSet(void);

// �}�b�`���O���[���Ǘ��p�֐�
static void MCRSYS_SetMoveObjWork( WIFIP2PMATCH_WORK* wk, MCR_MOVEOBJ* p_obj );
static void MCRSYS_DelMoveObjWork( WIFIP2PMATCH_WORK* wk, MCR_MOVEOBJ* p_obj );
static void MCRSYS_SetFriendObj( WIFIP2PMATCH_WORK* wk, u32 heapID );
static int MCRSYS_ContFiendInOut( WIFIP2PMATCH_WORK* wk );
static void MCRSYS_ContFriendStatus( WIFIP2PMATCH_WORK* wk, u32 heapID );
static MCR_MOVEOBJ* MCRSYS_GetMoveObjWork( WIFIP2PMATCH_WORK* wk, u32 friendNo );

static void WifiP2PMatchFriendListIconLoad( GF_BGL_INI* p_bgl, WIFIP2PMATCH_ICON* p_data, ARCHANDLE* p_handle, u32 heapID );
static void WifiP2PMatchFriendListIconRelease( WIFIP2PMATCH_ICON* p_data );
static void WifiP2PMatchFriendListIconWrite(  GF_BGL_INI* p_bgl, WIFIP2PMATCH_ICON* p_data, u32 frm, u8 cx, u8 cy, u32 icon_type, u32 col );
static void WifiP2PMatchFriendListStIconWrite( GF_BGL_INI* p_bgl, WIFIP2PMATCH_ICON* p_data, u32 frm, u8 cx, u8 cy, u32 status );
static void WifiP2PMatchFriendListBmpIconWrite(  GF_BGL_BMPWIN* p_bmp, WIFIP2PMATCH_ICON* p_data, u16 x, u16 y, u32 icon_type, u32 col );
static void WifiP2PMatchFriendListBmpStIconWrite( GF_BGL_BMPWIN* p_bmp, WIFIP2PMATCH_ICON* p_data, u16 x, u16 y, u32 status );
static int WifiP2PMatchBglFrmIconPalGet( u32 frm );

static int WifiP2PMatchFriendListStart( void );

// �F�B�f�[�^�̋����\������
static void WifiP2PMatch_UserDispOn( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 heapID );
static void WifiP2PMatch_UserDispOff( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void WifiP2PMatch_UserDispOff_Target( WIFIP2PMATCH_WORK *wk, u32 target_friend, u32 heapID );

static void WifiP2PMatch_UserDispOn_MyAcces( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 heapID );
static void WifiP2PMatch_UserDispOff_MyAcces( WIFIP2PMATCH_WORK *wk, u32 heapID );

// �F�B�f�[�^�r���[�A�[
static BOOL MCVSys_MoveCheck( const WIFIP2PMATCH_WORK *wk );
static void MCVSys_Init( WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle, u32 heapID );
static void MCVSys_Exit( WIFIP2PMATCH_WORK *wk );
static u32 MCVSys_Updata( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UpdataBttn( WIFIP2PMATCH_WORK *wk );
static BOOL MCVSys_UserDispEndCheck( WIFIP2PMATCH_WORK *wk, u32 oambttn_ret );
static u32	MCVSys_UserDispGetFriend( const WIFIP2PMATCH_WORK* cp_wk );
static void MCVSys_UserDispOn( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 heapID );
static void MCVSys_UserDispOff( WIFIP2PMATCH_WORK *wk );
static void MCVSys_UserDispPageChange( WIFIP2PMATCH_WORK *wk, u32 oambttn_ret );
static void MCVSys_BttnSet( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 type );
static void MCVSys_BttnDel( WIFIP2PMATCH_WORK *wk, u32 friendNo );
static u32 MCVSys_BttnTypeGet( const WIFIP2PMATCH_WORK *wk, u32 friendNo );
static void MCVSys_ReWrite( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_BttnAllWriteReq( WIFIP2PMATCH_WORK *wk );

static void MCVSys_BttnCallBack( u32 bttnid, u32 event, void* p_work );
static void MCVSys_BttnAnmMan( WIFIP2PMATCH_WORK *wk );
static void MCVSys_GraphicSet( WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle, u32 heapID );
static void MCVSys_GraphicDel( WIFIP2PMATCH_WORK *wk );
static void MCVSys_GraphicScrnCGOfsChange( NNSG2dScreenData* p_scrn, u8 cgofs );
static void MCVSys_BackDraw( WIFIP2PMATCH_WORK *wk );
static void MCVSys_BttnDraw( WIFIP2PMATCH_WORK *wk );
static void MCVSys_UserDispDraw( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType00( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType01( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType02( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType03( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType04( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType05( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawType06( WIFIP2PMATCH_WORK *wk, u32 heapID );
static void MCVSys_UserDispDrawFrontierOffScrn( WIFIP2PMATCH_WORK *wk );
static void MCVSys_UserDispFrontiorNumDraw( WIFIP2PMATCH_WORK *wk, u32 strid, u32 factoryid, u32 friendno, u32 x, u32 y );
static void MCVSys_UserDispFrontiorTitleStrGet( WIFIP2PMATCH_WORK *wk, STRBUF* p_str, u32 factoryid, u32 friendno );
static void MCVSys_UserDispNumDraw( WIFIP2PMATCH_WORK *wk, u32 strid, u32 num, u32 x, u32 y );
static void MCVSys_BttnWrite( WIFIP2PMATCH_WORK *wk, u8 cx, u8 cy, u8 type, u8 frame );
static u32 MCVSys_StatusMsgIdGet( u32 status, int* col );
static void MCVSys_BttnWinDraw( WIFIP2PMATCH_WORK *wk, GF_BGL_BMPWIN* p_bmp, u32 friendNo, u32 frame, u32 area_id );
static void MCVSys_BttnStatusWinDraw( WIFIP2PMATCH_WORK *wk, GF_BGL_BMPWIN* p_stbmp, u32 friendNo, u32 frame, u32 area_id );
static void MCVSys_OamBttnInit( WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle, u32 heapID );
static void MCVSys_OamBttnDelete( WIFIP2PMATCH_WORK *wk );
static void MCVSys_OamBttnOn( WIFIP2PMATCH_WORK *wk );
static void MCVSys_OamBttnOnNoBack( WIFIP2PMATCH_WORK *wk );
static void MCVSys_OamBttnOff( WIFIP2PMATCH_WORK *wk );
static u32 MCVSys_OamBttnMain( WIFIP2PMATCH_WORK *wk );
static void MCVSys_OamBttnCallBack( u32 bttn_no, u32 event, void* p_work );
static void MCVSys_OamBttnObjAnmStart( WIFIP2PMATCH_VIEW*  p_view, u32 bttn_no );
static BOOL MCVSys_OamBttnObjAnmMain( WIFIP2PMATCH_VIEW*  p_view, u32 bttn_no, u32 push_bttn, u32 event );
static s32 MCVSys_FirstFriendPageGet( const WIFIP2PMATCH_WORK * cp_wk );
static void MCVSys_FriendNameSet( WIFIP2PMATCH_WORK* p_wk, int friendno );

static void BmpWinDelete( WIFIP2PMATCH_WORK *wk );
static void _systemMessagePrint( WIFIP2PMATCH_WORK *wk, int msgno );
static void WifiP2PMatchMessagePrint( WIFIP2PMATCH_WORK *wk, int msgno , BOOL bSystem);
static void EndMessageWindowOff( WIFIP2PMATCH_WORK *wk );
static void WifiP2PMatchDataSendRecv( WIFIP2PMATCH_WORK *wk );
static void SequenceChange_MesWait( WIFIP2PMATCH_WORK *wk, int next );
static void _myStatusChange(WIFIP2PMATCH_WORK *wk, int status);
static void _myStatusChange_not_send(WIFIP2PMATCH_WORK *wk, int status);
static void _timeWaitIconDel(WIFIP2PMATCH_WORK *wk);
static void _friendNameExpand( WIFIP2PMATCH_WORK *wk, int friendNo);
static u32 _isPofinItemCheck(WIFIP2PMATCH_WORK* wk);


static void WifiP2P_SetLobbyBgm( void );
static BOOL WifiP2P_CheckLobbyBgm( void );


static void WifiP2P_Fnote_Set( WIFIP2PMATCH_WORK* wk, u32 idx );

static void _errorDisp(WIFIP2PMATCH_WORK* wk);

static void FriendRequestWaitOn( WIFIP2PMATCH_WORK* wk, BOOL msg_on );
static void FriendRequestWaitOff( WIFIP2PMATCH_WORK* wk );
static BOOL FriendRequestWaitFlagGet( const WIFIP2PMATCH_WORK* cp_wk );


// FuncTable����V�[�P���X�J�ڂŌĂ΂��֐�
static int WifiP2PMatch_MainInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _normalConnectYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _normalConnectWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _differMachineInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _differMachineNext( WIFIP2PMATCH_WORK *wk, int seq );
static int _differMachineOneMore( WIFIP2PMATCH_WORK *wk, int seq );
static int _firstYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _poweroffInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _poweroffYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _poweroffWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _retryInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _retryYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _retryWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _retry( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_ReConnectingWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_ConnectingInit( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_Connecting( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_FirstSaving( WIFIP2PMATCH_WORK *wk, int seq );
static int _firstConnectEndMsg( WIFIP2PMATCH_WORK *wk, int seq );
static int _firstConnectEndMsgWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_FriendListInit( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_FriendList( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTConnectInit2( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTConnectInit( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTConnectWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTConnect( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTConnectEndYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTConnectEndWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_VCTDisconnect(WIFIP2PMATCH_WORK *wk, int seq);
static int WifiP2PMatch_BattleDisconnect(WIFIP2PMATCH_WORK *wk, int seq);
static int WifiP2PMatch_Disconnect(WIFIP2PMATCH_WORK *wk, int seq);
static int WifiP2PMatch_BattleConnectInit( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_BattleConnectWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_BattleConnect( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_MainReturn( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeSelectMenuInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeSelectMenuWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentRestart( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeSubSelectMenuWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeSelectRelInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeSelectRelYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeSelectRelWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _childModeMatchMenuInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _childModeMatchMenuInit2( WIFIP2PMATCH_WORK *wk, int seq );
static int _childModeMatchMenuWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _childModeMatchMenuLoop( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_BCancelYesNoVCT( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_BCancelWaitVCT( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_BCancelYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_BCancelWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeCallMenuInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeCallMenuYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeCallMenuSend( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeCallMenuCheck( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeCallMyStatusWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _parentModeCallMenuWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _personalDataInit( WIFIP2PMATCH_WORK *wk, int seq );
static int _personalDataWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _personalDataEnd( WIFIP2PMATCH_WORK *wk, int seq );
static int _exitYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _exitWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _exitExiting( WIFIP2PMATCH_WORK *wk, int seq );
static int _exitEnd( WIFIP2PMATCH_WORK *wk, int seq );
static int _nextBattleYesNo( WIFIP2PMATCH_WORK *wk, int seq );
static int _nextBattleWait( WIFIP2PMATCH_WORK *wk, int seq );
static int _vchatNegoCheck( WIFIP2PMATCH_WORK *wk, int seq );
static int _vchatNegoWait( WIFIP2PMATCH_WORK *wk, int seq );

static BOOL _myVChatStatusToggle(WIFIP2PMATCH_WORK *wk);
static BOOL _myVChatStatusToggleOrg(WIFIP2PMATCH_WORK *wk);
static BOOL _myVChatStatusOrgSet(WIFIP2PMATCH_WORK *wk);
static int _vchatToggleWait( WIFIP2PMATCH_WORK *wk, int seq );


static int WifiP2PMatch_EndWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_CheckAndEnd( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_CancelEnableWait( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_CancelEnableWaitDP( WIFIP2PMATCH_WORK *wk, int seq );
static int WifiP2PMatch_FirstSaving2( WIFIP2PMATCH_WORK *wk, int seq );



// ���R�[�h�R�[�i�[���C���V�[�P���X�p�֐��z���`
static int (*FuncTable[])(WIFIP2PMATCH_WORK *wk, int seq)={
    WifiP2PMatch_MainInit,	// WIFIP2PMATCH_MODE_INIT  = 0,
    _normalConnectYesNo,    // WIFIP2PMATCH_NORMALCONNECT_YESNO
    _normalConnectWait,     // WIFIP2PMATCH_NORMALCONNECT_WAIT
    _differMachineInit,     // WIFIP2PMATCH_DIFFER_MACHINE_INIT
    _differMachineNext,     // WIFIP2PMATCH_DIFFER_MACHINE_NEXT
    _differMachineOneMore,  // WIFIP2PMATCH_DIFFER_MACHINE_ONEMORE
    _firstYesNo,            //WIFIP2PMATCH_FIRST_YESNO
    _poweroffInit,      // WIFIP2PMATCH_POWEROFF_INIT
    _poweroffYesNo,     // WIFIP2PMATCH_POWEROFF_YESNO
    _poweroffWait,      // WIFIP2PMATCH_POWEROFF_WAIT
    _retryInit,        //WIFIP2PMATCH_RETRY_INIT
    _retryYesNo,        //WIFIP2PMATCH_RETRY_YESNO
    _retryWait,         //WIFIP2PMATCH_RETRY_WAIT
    _retry,             //WIFIP2PMATCH_RETRY
    WifiP2PMatch_ConnectingInit,        // WIFIP2PMATCH_CONNECTING_INIT,
    WifiP2PMatch_Connecting,            // WIFIP2PMATCH_CONNECTING,
    _firstConnectEndMsg,                 //WIFIP2PMATCH_FIRST_ENDMSG
    _firstConnectEndMsgWait,    //WIFIP2PMATCH_FIRST_ENDMSG_WAIT
    WifiP2PMatch_FriendListInit,        // WIFIP2PMATCH_FRIENDLIST_INIT,
    WifiP2PMatch_FriendList,            // WIFIP2PMATCH_MODE_FRIENDLIST
    _vchatToggleWait,                   // WIFIP2PMATCH_VCHATWIN_WAIT
    WifiP2PMatch_VCTConnectInit2,        // WIFIP2PMATCH_MODE_VCT_CONNECT_INIT2
    WifiP2PMatch_VCTConnectInit,        // WIFIP2PMATCH_MODE_VCT_CONNECT_INIT
    WifiP2PMatch_VCTConnectWait,        // WIFIP2PMATCH_MODE_VCT_CONNECT_WAIT
    WifiP2PMatch_VCTConnect,        // WIFIP2PMATCH_MODE_VCT_CONNECT
    WifiP2PMatch_VCTConnectEndYesNo,   // WIFIP2PMATCH_MODE_VCT_CONNECTEND_YESNO
    WifiP2PMatch_VCTConnectEndWait,   // WIFIP2PMATCH_MODE_VCT_CONNECTEND_WAIT
    WifiP2PMatch_VCTDisconnect,   // WIFIP2PMATCH_MODE_VCT_DISCONNECT
    WifiP2PMatch_BattleDisconnect,   //WIFIP2PMATCH_MODE_BATTLE_DISCONNECT
    WifiP2PMatch_Disconnect,  //WIFIP2PMATCH_MODE_DISCONNECT
    WifiP2PMatch_BattleConnectInit,     // WIFIP2PMATCH_MODE_BATTLE_CONNECT_INIT
    WifiP2PMatch_BattleConnectWait,     // WIFIP2PMATCH_MODE_BATTLE_CONNECT_WAIT
    WifiP2PMatch_BattleConnect,        // WIFIP2PMATCH_MODE_BATTLE_CONNECT
    WifiP2PMatch_MainReturn,            // WIFIP2PMATCH_MODE_MAIN_MENU
    WifiP2PMatch_EndWait,			    // WIFIP2PMATCH_MODE_END_WAIT,
    WifiP2PMatch_CheckAndEnd,          //  WIFIP2PMATCH_MODE_CHECK_AND_END
    _parentModeSelectMenuInit,  // WIFIP2PMATCH_MODE_SELECT_INIT
    _parentModeSelectMenuWait,  // WIFIP2PMATCH_MODE_SELECT_WAIT
    _parentModeSubSelectMenuWait, // WIFIP2PMATCH_MODE_SUBBATTLE_WAIT
    _parentModeSelectRelInit,  //WIFIP2PMATCH_MODE_SELECT_REL_INIT
    _parentModeSelectRelYesNo, //WIFIP2PMATCH_MODE_SELECT_REL_YESNO
    _parentModeSelectRelWait,  //WIFIP2PMATCH_MODE_SELECT_REL_WAIT
    _childModeMatchMenuInit,   //WIFIP2PMATCH_MODE_MATCH_INIT
    _childModeMatchMenuInit2,   //WIFIP2PMATCH_MODE_MATCH_INIT
    _childModeMatchMenuWait,   //WIFIP2PMATCH_MODE_MATCH_WAIT
    _childModeMatchMenuLoop, //WIFIP2PMATCH_MODE_MATCH_LOOP
    WifiP2PMatch_BCancelYesNo, //WIFIP2PMATCH_MODE_BCANCEL_YESNO
    WifiP2PMatch_BCancelWait,    //WIFIP2PMATCH_MODE_BCANCEL_WAIT
    _parentModeCallMenuInit,//WIFIP2PMATCH_MODE_CALL_INIT
    _parentModeCallMenuYesNo,//WIFIP2PMATCH_MODE_CALL_YESNO
    _parentModeCallMenuSend, //WIFIP2PMATCH_MODE_CALL_SEND
    _parentModeCallMenuCheck, //WIFIP2PMATCH_MODE_CALL_CHECK
    _parentModeCallMyStatusWait, //WIFIP2PMATCH_MODE_MYSTATUS_WAIT   //51
    _parentModeCallMenuWait,//WIFIP2PMATCH_MODE_CALL_WAIT
    _personalDataInit,    //WIFIP2PMATCH_MODE_PERSONAL_INIT
    _personalDataWait,    //WIFIP2PMATCH_MODE_PERSONAL_WAIT
    _personalDataEnd,    //WIFIP2PMATCH_MODE_PERSONAL_END
    _exitYesNo,       //WIFIP2PMATCH_MODE_EXIT_YESNO
    _exitWait,        //WIFIP2PMATCH_MODE_EXIT_WAIT
    _exitExiting,        //WIFIP2PMATCH_MODE_EXITING
    _exitEnd,        //WIFIP2PMATCH_MODE_EXIT_END
    _nextBattleYesNo,  //WIFIP2PMATCH_NEXTBATTLE_YESNO
    _nextBattleWait,  //WIFIP2PMATCH_NEXTBATTLE_WAIT
    _vchatNegoCheck, //WIFIP2PMATCH_MODE_VCHAT_NEGO
    _vchatNegoWait, //WIFIP2PMATCH_MODE_VCHAT_NEGO_WAIT
    WifiP2PMatch_ReConnectingWait, //WIFIP2PMATCH_RECONECTING_WAIT
    WifiP2PMatch_BCancelYesNoVCT, //WIFIP2PMATCH_MODE_BCANCEL_YESNO_VCT
    WifiP2PMatch_BCancelWaitVCT,    //WIFIP2PMATCH_MODE_BCANCEL_WAIT_VCT
    _parentRestart,  //WIFIP2PMATCH_PARENT_RESTART
    WifiP2PMatch_FirstSaving, //WIFIP2PMATCH_FIRST_SAVING
    WifiP2PMatch_CancelEnableWait, //WIFIP2PMATCH_MODE_CANCEL_ENABLE_WAIT
    WifiP2PMatch_FirstSaving2, //WIFIP2PMATCH_FIRST_SAVING2
};

static const u8 ViewButtonFrame_y[ 4 ] = {
	8, 7, 5, 7
};


static const BMPWIN_DAT _yesNoBmpDat = {
    GF_BGL_FRAME2_M, FLD_YESNO_WIN_PX, FLD_YESNO_WIN_PY,
    FLD_YESNO_WIN_SX, FLD_YESNO_WIN_SY, FLD_YESNO_WIN_PAL, FLD_YESNO_WIN_CGX
    };

// �t�����e�B�A�t���O�������Ă��邩�`�F�b�N
static BOOL _frontierInCheck( WIFIP2PMATCH_WORK * wk )
{
	return SysFlag_ArriveGet(SaveData_GetEventWork(wk->pSaveData),FLAG_ARRIVE_D32R0101);
}

// �܂��܂������t���O�������Ă��邩�`�F�b�N
static BOOL _pofinCaseCheck( WIFIP2PMATCH_WORK * wk )
{

	// �|���g�P�[�X�����邩�`�F�b�N
	if( !MyItem_GetItemNum( SaveData_GetMyItem(wk->pSaveData),ITEM_POFINKEESU,HEAPID_WIFIP2PMATCH ) ){
		return FALSE;
	}
	return TRUE;
}

// �~�j�Q�[���t���O�������Ă��邩�`�F�b�N
static BOOL _miniGameInCheck( WIFIP2PMATCH_WORK * wk )
{
//	return FALSE;
	return TRUE;	// �~�j�Q�[���͏�Ɍ������Ԃɂ���
}

// �����X�e�[�^�X���擾����
static u32 _WifiMyStatusGet( WIFIP2PMATCH_WORK * p_wk, _WIFI_MACH_STATUS* p_status )
{
	u32 status;
	status = p_status->status;
	if( (status == WIFI_STATUS_FRONTIER) || 
		(status == WIFI_STATUS_FRONTIER_WAIT) ){

		if( _frontierInCheck( p_wk ) == FALSE ){
			status = WIFI_STATUS_UNKNOWN;	
		}
	}
	if( (status == WIFI_STATUS_BUCKET) ||
		(status == WIFI_STATUS_BUCKET_WAIT) ||
		(status == WIFI_STATUS_BALANCEBALL) ||
		(status == WIFI_STATUS_BALANCEBALL_WAIT) ||
		(status == WIFI_STATUS_BALLOON) ||
		(status == WIFI_STATUS_BALLOON_WAIT) ){

		if( _miniGameInCheck( p_wk ) == FALSE ){
			status = WIFI_STATUS_UNKNOWN;
		}
	}
	if( (status == WIFI_STATUS_POFIN) ||
		(status == WIFI_STATUS_POFIN_WAIT) ){

		if( _pofinCaseCheck( p_wk ) == FALSE ){
			status = WIFI_STATUS_UNKNOWN;
		}
	}
	return status;
}


// 2�`4�l�ł����ׂ邩
static BOOL _wait2to4Mode( int status )
{
    switch(status){
      case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
      case WIFI_STATUS_POFIN:		 // �|�t�B����
		  
      case WIFI_STATUS_BUCKET_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BUCKET:		 // �o�P�b�g��

      case WIFI_STATUS_BALLOON_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALLOON:		 // �o�P�b�g��

      case WIFI_STATUS_BALANCEBALL_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALANCEBALL:		 // �o�P�b�g��
        return TRUE;
      default:
        return FALSE;
    }
}

static u32 _get2to4ModeEndSeq( int status )
{
    switch(status){
      case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
      case WIFI_STATUS_POFIN:		 // �|�t�B����
		return WIFI_P2PMATCH_POFIN;
      case WIFI_STATUS_BUCKET_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BUCKET:		 // �o�P�b�g��
		return WIFI_P2PMATCH_BUCKET;
      case WIFI_STATUS_BALLOON_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALLOON:		 // �o�P�b�g��
		return WIFI_P2PMATCH_BALLOON;
      case WIFI_STATUS_BALANCEBALL_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALANCEBALL:		 // �o�P�b�g��
		return WIFI_P2PMATCH_BALANCEBALL;
      default:
		break;
    }

	GF_ASSERT(0);
	return 0;
}

// �X�e�[�g��BATTLEWAIT���ǂ���
static BOOL _modeBattleWait(int status)
{
    switch(status){
      case WIFI_STATUS_SBATTLE50_WAIT:   // �V���O��Lv50�ΐ��W��
      case WIFI_STATUS_SBATTLE100_WAIT:   // �V���O��Lv100�ΐ��W��
      case WIFI_STATUS_SBATTLE_FREE_WAIT:   // �V���O��Lv100�ΐ��W��
      case WIFI_STATUS_DBATTLE50_WAIT:   // �_�u��Lv50�ΐ��W��
      case WIFI_STATUS_DBATTLE100_WAIT:   // �_�u��Lv100�ΐ��W��
      case WIFI_STATUS_DBATTLE_FREE_WAIT:   // �_�u��Lv100�ΐ��W��
#ifdef WFP2P_DEBUG_EXON
      case WIFI_STATUS_MBATTLE_FREE_WAIT:   // �}���`�o�g���@�t���[�ΐ��W��
#endif
        return TRUE;
      default:
        return FALSE;
    }
}

// �X�e�[�g��WAIT���ǂ���
static BOOL _modeWait(int status)
{
    if(_modeBattleWait(status)){
        return TRUE;
    }
    switch(status){
      case WIFI_STATUS_TRADE_WAIT:    // ������W��
      case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
      case WIFI_STATUS_FRONTIER_WAIT:    // �t�����e�B�A��W��
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM_WAIT:    // �o�g�����[����W��
#endif
      case WIFI_STATUS_BUCKET_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALLOON_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALANCEBALL_WAIT:    // �o�P�b�g��W��
        return TRUE;
    }
    return FALSE;
}

// �X�e�[�g��BATTLE���ǂ���
static BOOL _modeBattle(int status)
{
    switch(status){
      case WIFI_STATUS_SBATTLE50:      // �V���O���ΐ풆
      case WIFI_STATUS_SBATTLE100:      // �V���O���ΐ풆
      case WIFI_STATUS_SBATTLE_FREE:      // �V���O���ΐ풆
      case WIFI_STATUS_DBATTLE50:      // �_�u���ΐ풆
      case WIFI_STATUS_DBATTLE100:      // �_�u���ΐ풆
      case WIFI_STATUS_DBATTLE_FREE:      // �_�u���ΐ풆
		  
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE:      // �}���`�ΐ풆
#endif
        return TRUE;
      default:
        return FALSE;
    }
}

// �X�e�[�g��ACTIVE���ǂ���
static BOOL _modeActive(int status)
{
    if(_modeBattle(status)){
        return TRUE;
    }
    switch(status){
      case WIFI_STATUS_TRADE:    // ������
      case WIFI_STATUS_POFIN:    // �|�t�B����
      case WIFI_STATUS_FRONTIER:    // �t�����e�B�A��
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM:    // �o�g�����[����
#endif
      case WIFI_STATUS_BUCKET:    // �o�P�b�g��
      case WIFI_STATUS_BALLOON:    // �o�P�b�g��
      case WIFI_STATUS_BALANCEBALL:    // �o�P�b�g��
      case WIFI_STATUS_VCT:    // ������
        return TRUE;
    }
    return FALSE;
}




static BOOL _is2pokeMode(int status)
{
    switch(status){
      case WIFI_STATUS_TRADE_WAIT:
      case WIFI_STATUS_DBATTLE50_WAIT:
      case WIFI_STATUS_DBATTLE100_WAIT:
      case WIFI_STATUS_DBATTLE_FREE_WAIT:
        return TRUE;
    }
    return FALSE;
}

static int _convertState(int state)
{
    int ret = WIFI_STATUS_UNKNOWN;

    switch(state){
      case WIFI_STATUS_POFIN_WAIT:
        ret = WIFI_STATUS_POFIN;
        break;
      case WIFI_STATUS_FRONTIER_WAIT:
        ret = WIFI_STATUS_FRONTIER;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM_WAIT:
        ret = WIFI_STATUS_BATTLEROOM;
        break;
#endif
      case WIFI_STATUS_BUCKET_WAIT:
        ret = WIFI_STATUS_BUCKET;
        break;
      case WIFI_STATUS_BALLOON_WAIT:
        ret = WIFI_STATUS_BALLOON;
        break;
      case WIFI_STATUS_BALANCEBALL_WAIT:
        ret = WIFI_STATUS_BALANCEBALL;
        break;
      case WIFI_STATUS_TRADE_WAIT:
        ret = WIFI_STATUS_TRADE;
        break;
      case WIFI_STATUS_SBATTLE50_WAIT:
        ret = WIFI_STATUS_SBATTLE50;
        break;
      case WIFI_STATUS_SBATTLE100_WAIT:
        ret = WIFI_STATUS_SBATTLE100;
        break;
      case WIFI_STATUS_SBATTLE_FREE_WAIT:
        ret = WIFI_STATUS_SBATTLE_FREE;
        break;
      case WIFI_STATUS_DBATTLE50_WAIT:
        ret = WIFI_STATUS_DBATTLE50;
        break;
      case WIFI_STATUS_DBATTLE100_WAIT:
        ret = WIFI_STATUS_DBATTLE100;
        break;
      case WIFI_STATUS_DBATTLE_FREE_WAIT:
        ret = WIFI_STATUS_DBATTLE_FREE;
        break;
      case WIFI_STATUS_LOGIN_WAIT:
        ret = WIFI_STATUS_VCT;
        break;
      case WIFI_STATUS_POFIN:
        ret = WIFI_STATUS_POFIN_WAIT;
        break;
      case WIFI_STATUS_FRONTIER:
        ret = WIFI_STATUS_FRONTIER_WAIT;
        break;
      case WIFI_STATUS_BUCKET:
        ret = WIFI_STATUS_BUCKET_WAIT;
        break;
      case WIFI_STATUS_BALLOON:
        ret = WIFI_STATUS_BALLOON_WAIT;
        break;
      case WIFI_STATUS_BALANCEBALL:
        ret = WIFI_STATUS_BALANCEBALL_WAIT;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM:
        ret = WIFI_STATUS_BATTLEROOM_WAIT;
        break;
#endif
      case WIFI_STATUS_TRADE:
        ret = WIFI_STATUS_TRADE_WAIT;
        break;
      case WIFI_STATUS_SBATTLE50:
        ret = WIFI_STATUS_SBATTLE50_WAIT;
        break;
      case WIFI_STATUS_SBATTLE100:
        ret = WIFI_STATUS_SBATTLE100_WAIT;
        break;
      case WIFI_STATUS_SBATTLE_FREE:
        ret = WIFI_STATUS_SBATTLE_FREE_WAIT;
        break;
      case WIFI_STATUS_DBATTLE50:
        ret = WIFI_STATUS_DBATTLE50_WAIT;
        break;
      case WIFI_STATUS_DBATTLE100:
        ret = WIFI_STATUS_DBATTLE100_WAIT;
        break;
      case WIFI_STATUS_DBATTLE_FREE:
        ret = WIFI_STATUS_DBATTLE_FREE_WAIT;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE:
        ret = WIFI_STATUS_MBATTLE_FREE_WAIT;
        break;
      case WIFI_STATUS_MBATTLE_FREE_WAIT:
        ret = WIFI_STATUS_MBATTLE_FREE;
        break;
#endif
      case WIFI_STATUS_VCT:
        ret = WIFI_STATUS_LOGIN_WAIT;
        break;
    }
    return ret;
}

static int _getBattlePokeNum(WIFIP2PMATCH_WORK* wk)
{
    POKEPARTY* pMyPoke = SaveData_GetTemotiPokemon(wk->pSaveData);
    POKEMON_PARAM* poke;
    int max = PokeParty_GetPokeCount(pMyPoke);
    int i,num = 0;

	for(i = 0 ; i < max ; i++){
		poke = PokeParty_GetMemberPointer(pMyPoke, i);
        if (PokeParaGet(poke, ID_PARA_hp, NULL) == 0) {
            continue;
        }
        if (PokeParaGet(poke, ID_PARA_tamago_flag, NULL)) {
            continue;
        }
        num++;
	}
    return num;
}

//--------------------------------------------------------------------------------------------
/**
 * �X�e�[�^�X�ɂ��킹�ĒʐM�X�e�[�g���ύX����
 * @param	status
 * @return	none
 */
//--------------------------------------------------------------------------------------------

static void _commStateChange(int status)
{
    if( (status == WIFI_STATUS_POFIN_WAIT) || (WIFI_STATUS_POFIN == status) ){    // �|�t�B����W��
        CommStateChangeWiFiPofin();
    }
    else if(_wait2to4Mode(status)){
        CommStateChangeWiFiClub();	// WiFilobbyMinigame�ɂ����
									// �ގ����������������Ȃ�
									
//        CommStateChangeWiFiLogin();���������䂤�����ɂ���ƃo�O��B�Q�l�����Ȃ���Ȃ�
    }
    else if(status == WIFI_STATUS_LOGIN_WAIT){
        CommStateChangeWiFiLogin();
    }
    else if( (status == WIFI_STATUS_FRONTIER_WAIT) || (status == WIFI_STATUS_FRONTIER) ){
		CommSetWifiBothNet(FALSE);
		CommStateChangeWiFiFactory();
    }
    else{
	    CommSetWifiBothNet(TRUE); // wifi�̒ʐM��񓯊����瓯����
        CommStateChangeWiFiBattle();
    }


}

//============================================================================================
//	�v���Z�X�֐�
//============================================================================================

static void _graphicInit(WIFIP2PMATCH_WORK * wk)
{
	ARCHANDLE* p_handle;
	
    sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
    sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

    GF_Disp_GX_VisibleControlInit();
    GF_Disp_GXS_VisibleControlInit();
    GX_SetVisiblePlane( 0 );
    GXS_SetVisiblePlane( 0 );
	sys_KeyRepeatSpeedSet( SYS_KEYREPEAT_SPEED_DEF, SYS_KEYREPEAT_WAIT_DEF );

	p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, HEAPID_WIFIP2PMATCH );

    wk->bgl = GF_BGL_BglIniAlloc( HEAPID_WIFIP2PMATCH );
    // ������}�l�[�W���[����
    wk->WordSet    = WORDSET_Create( HEAPID_WIFIP2PMATCH );
    wk->MsgManager = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_lobby_dat, HEAPID_WIFIP2PMATCH );
    wk->SysMsgManager = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_system_dat, HEAPID_WIFIP2PMATCH );

    // VRAM �o���N�ݒ�
    VramBankSet();

    // BGL���W�X�^�ݒ�
    BgInit( wk->bgl );

    //BG�O���t�B�b�N�Z�b�g
    BgGraphicSet( wk, p_handle );

	//�p���b�g�A�j���V�X�e���쐬
	ConnectBGPalAnm_Init(&wk->cbp, p_handle, NARC_wifip2pmatch_conect_anm_NCLR, HEAPID_WIFIP2PMATCH);

    // VBlank�֐��Z�b�g
    sys_VBlankFuncChange( VBlankFunc, wk );

    // OBJ�L�����A�p���b�g�}�l�[�W���[������
    char_pltt_manager_init();

    // CellActor�V�X�e��������
    InitCellActor(wk, p_handle);

    // CellActro�\���o�^
    SetCellActor(wk);

	ArchiveDataHandleClose( p_handle );
}


//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
PROC_RESULT WifiP2PMatchProc_Init( PROC * proc, int * seq )
{
    WIFIP2PMATCH_WORK * wk;
    WIFIP2PMATCH_PROC_PARAM* pParentWork;
	u32 result;

    switch(*seq){
      case 0:
		  
        pParentWork = PROC_GetParentWork(proc);

		// �ʐM�g���q�[�v�̃������m��
		// 080611	tomoya �ǉ�
		if(CommStateIsWifiConnect()){       // �ڑ���
			switch( pParentWork->seq ){	// 1�O�̗V�тŒʐM�g���q�[�v��j�����Ă�����Ċm��
			case WIFI_P2PMATCH_END:
			case WIFI_P2PMATCH_UTIL:
			case WIFI_P2PMATCH_DPW_END:
			case WIFI_P2PMATCH_SBATTLE50:
			case WIFI_P2PMATCH_SBATTLE100:
			case WIFI_P2PMATCH_SBATTLE_FREE:
			case WIFI_P2PMATCH_DBATTLE50:
			case WIFI_P2PMATCH_DBATTLE100:
			case WIFI_P2PMATCH_DBATTLE_FREE:
			case WIFI_P2PMATCH_FRONTIER:
			case WIFI_P2PMATCH_TRADE:
				// �o�g������߂��Ă����Ȃ�g���q�[�v�쐬
				sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCHEX, WIFI_P2PMATCH_EXHEAP_SIZE );
				break;

			default:
				break;
			}
		}else{
			// �ʐM�O�Ȃ��΂Ɋg���q�[�v���m��
			sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCHEX, WIFI_P2PMATCH_EXHEAP_SIZE );
		}

		// wifi_2dmap�I�[�o�[���C�Ǎ���
		Overlay_Load( FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);

        if(pParentWork->seq == WIFI_P2PMATCH_DPW){ // ���E�ΐ�Ȃ�
            sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCH, 0x70000 );
        }
        else{
//            sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCH, 0xb0000 );
            sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCH, 0xa0000 );
        }
        wk = PROC_AllocWork( proc, sizeof(WIFIP2PMATCH_WORK), HEAPID_WIFIP2PMATCH );
        MI_CpuFill8( wk, 0, sizeof(WIFIP2PMATCH_WORK) );

		// Vram�]���}�l�[�W���쐬
		initVramTransferManagerHeap( VRANTRANSFERMAN_NUM, HEAPID_WIFIP2PMATCH );

        wk->MsgIndex = _PRINTTASK_MAX;
        wk->pSaveData = pParentWork->pSaveData;
        wk->pList = SaveData_GetWifiListData(wk->pSaveData);
        wk->initSeq = pParentWork->seq;    // P2P��DPW��
        wk->endSeq = WIFI_P2PMATCH_END;
        wk->preConnect = -1;

        GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0|GX_PLANEMASK_BG1|GX_PLANEMASK_BG2|GX_PLANEMASK_BG3, VISIBLE_OFF );
        GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0|GX_PLANEMASK_BG1|GX_PLANEMASK_BG2|GX_PLANEMASK_BG3, VISIBLE_OFF );

#if AFTERMASTER_061215_WIFIP2P_FADE_FIX
        // ���[�N������
        InitWork( wk );
        // �O���t�B�b�N������
        _graphicInit(wk);
        // ���C�v�t�F�[�h�J�n
        WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);

#else
        
        // ���[�N������
        InitWork( wk );
        //        WIPE_ResetBrightness( WIPE_DISP_MAIN );
        WIPE_ResetBrightness( WIPE_DISP_SUB );
        WIPE_ResetWndMask( WIPE_DISP_SUB );
        // ���C�v�t�F�[�h�J�n
        WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);
        _graphicInit(wk);
#endif
        if(CommStateIsWifiConnect()){
			WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��
        }
        else{
            DwcOverlayStart();    // WIFI�I�[�o�[���C
        }
		
		// �^�b�`�p�l���J�n
		result = InitTPNoBuff( 4 );
		GF_ASSERT( result == TP_OK );
		
        (*seq)++;
        break;
      case 1:
        wk = PROC_GetWork( proc );
        (*seq) = SEQ_IN;
        return PROC_RES_FINISH;
        break;
    }
    return PROC_RES_CONTINUE;
}




//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F���C��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------

PROC_RESULT WifiP2PMatchProc_Main( PROC * proc, int * seq )
{
    WIFIP2PMATCH_WORK * wk  = PROC_GetWork( proc );

    switch( *seq ){
      case SEQ_IN:
        if( WIPE_SYS_EndCheck() ){
            // ���C�v�����҂�
            *seq = SEQ_MAIN;


        }
        break;

      case SEQ_MAIN:

#ifdef _WIFI_DEBUG_TUUSHIN
		if( WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_MODE != _WIFI_DEBUG_NONE ){
			sys.trg |= PAD_KEY_DOWN;	//  ������������
			sys.cont |= PAD_KEY_DOWN;
			sys.trg |= PAD_BUTTON_A;
			sys.cont |= PAD_BUTTON_A;
		}
#endif

		
        // �J�[�\���ړ�

        // �V�[�P���X���̓���
            if(FuncTable[wk->seq]!=NULL){
            static int seqBk=0;

            if(seqBk != wk->seq){
//                OHNO_PRINT("wp2p %d \n",wk->seq);
                OS_TPrintf("wp2p %d \n",wk->seq);
            }
            seqBk = wk->seq;

            *seq = (*FuncTable[wk->seq])( wk, *seq );
        }

        break;

      case SEQ_OUT:
        if( WIPE_SYS_EndCheck() ){
            return PROC_RES_FINISH;
        }
        break;
    }
#if 1
    if(wk->clactSet){
        CLACT_Draw( wk->clactSet );									// �Z���A�N�^�[�풓�֐�
    }
#endif
	if( WIFI_MCR_GetInitFlag( &wk->matchroom ) == TRUE ){
		WIFI_MCR_Draw( &wk->matchroom );
	}
	
	ConnectBGPalAnm_Main(&wk->cbp);
	
    return PROC_RES_CONTINUE;
}

#define DEFAULT_NAME_MAX		18

// �_�C���E�p�[���ŕς��񂾂낤
#define MALE_NAME_START			0
#define FEMALE_NAME_START		18


//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�ɂ�����镔���̏I������
 * @param	wk	WIFIP2PMATCH_WORK
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void _graphicEnd(WIFIP2PMATCH_WORK* wk)
{
    int i;

    sys_VBlankFuncChange( NULL, NULL );	// VBlankReset

	ConnectBGPalAnm_End(&wk->cbp);

	// �}�b�`���O���[���j��
	if( WIFI_MCR_GetInitFlag( &wk->matchroom ) == TRUE ){
		WIFI_MCR_Dest( &wk->matchroom );
	}
	// �r���[�A�[�j��
	if( MCVSys_MoveCheck( wk ) == TRUE ){
		MCVSys_Exit( wk );
	}

	// �A�C�R���j��
	WifiP2PMatchFriendListIconRelease( &wk->icon );

#if 0
    // �L�����]���}�l�[�W���[�j��
    CLACT_U_CharManagerDelete(wk->resObjTbl[MAIN_LCD][CLACT_U_CHAR_RES]);

    // �p���b�g�]���}�l�[�W���[�j��
    CLACT_U_PlttManagerDelete(wk->resObjTbl[MAIN_LCD][CLACT_U_PLTT_RES]);
#endif

	// FontOAM�j��
	FONTOAM_SysDelete( wk->fontoam );

    // �L�����E�p���b�g�E�Z���E�Z���A�j���̃��\�[�X�}�l�[�W���[�j��
    for(i=0;i<CLACT_RESOURCE_NUM;i++){
        CLACT_U_ResManagerDelete(wk->resMan[i]);
    }
    // �Z���A�N�^�[�Z�b�g�j��
    CLACT_DestSet(wk->clactSet);
	
    //OAM�����_���[�j��
    REND_OAM_Delete();

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

    // BMP�E�B���h�E�J��
    BmpWinDelete( wk );

    // BGL�폜
    BgExit( wk->bgl );

    // �e�̃v���b�N�̏���
    //    UnionRoomView_ObjInit( (COMM_UNIONROOM_VIEW*)PROC_GetParentWork( proc ) );

    // �^�b�`�p�l���V�X�e���I��
    //	StopTP();


    // ���b�Z�[�W�}�l�[�W���[�E���[�h�Z�b�g�}�l�[�W���[���
    MSGMAN_Delete( wk->SysMsgManager );
    MSGMAN_Delete( wk->MsgManager );
    WORDSET_Delete( wk->WordSet );

	// �I�[�o�[���C�j��
	Overlay_UnloadID( FS_OVERLAY_ID(wifi_2dmapsys) );
}


//--------------------------------------------------------------------------------------------
/**
 * �v���Z�X�֐��F�I��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	������
 */
//--------------------------------------------------------------------------------------------
PROC_RESULT WifiP2PMatchProc_End( PROC * proc, int * seq )
{
    WIFIP2PMATCH_WORK  *wk    = PROC_GetWork( proc );
    WIFIP2PMATCH_PROC_PARAM* pParentWork;


    _graphicEnd(wk);

    pParentWork = PROC_GetParentWork(proc);
    pParentWork->seq = wk->endSeq;
    if( (wk->endSeq == WIFI_P2PMATCH_END) || (wk->endSeq == WIFI_P2PMATCH_DPW_END) ){  // �I�[�o�[���C����
        DwcOverlayEnd();
    }
    else{
        pParentWork->targetID = mydwc_getFriendIndex();
    }
	
    // ���[�N���
    FreeWork( wk );

    PROC_FreeWork( proc );				// PROC���[�N�J��
	
	// �^�b�`�p�l����~
	StopTP();

	// VramTransfer�}�l�[�W���j��
	DellVramTransferManager();

    sys_DeleteHeap( HEAPID_WIFIP2PMATCH );
    //    WIPE_ResetBrightness( WIPE_DISP_MAIN );
    //  WIPE_ResetBrightness( WIPE_DISP_SUB );
	

	// �g���q�[�v�̔j��
	// �ʐM�̏I���ƁA�o�g�����͊g���q�[�v��j������
	// ����ȊO�͔j�����Ȃ�
	switch( wk->endSeq ){
	case WIFI_P2PMATCH_END:
	case WIFI_P2PMATCH_UTIL:
	case WIFI_P2PMATCH_DPW_END:
	case WIFI_P2PMATCH_SBATTLE50:
	case WIFI_P2PMATCH_SBATTLE100:
	case WIFI_P2PMATCH_SBATTLE_FREE:
	case WIFI_P2PMATCH_DBATTLE50:
	case WIFI_P2PMATCH_DBATTLE100:
	case WIFI_P2PMATCH_DBATTLE_FREE:
	case WIFI_P2PMATCH_FRONTIER:
	case WIFI_P2PMATCH_TRADE:

		sys_DeleteHeap( HEAPID_WIFIP2PMATCHEX );
		break;

	default:
		break;
	}

	
	// BGM���|�P�Z���̂܂܂Ȃ�BGM���ʂ�ύX����
	if( WifiP2P_CheckLobbyBgm() == TRUE ){
		Snd_PlayerSetInitialVolume( SND_HANDLE_FIELD, BGM_POKECEN_VOL );
	}

#if 0	//���ŉ��ʏ���������̂ł���Ȃ�
	// VCHAT���̒ʐM�G���[�̏ꍇBGM�̉��ʂ����ɖ߂�
	if( (wk->endSeq == WIFI_P2PMATCH_END) || (wk->endSeq == WIFI_P2PMATCH_DPW_END) ){  // �I�[�o�[���C����
		//���ʃZ�b�g
		Snd_PlayerSetInitialVolume( SND_HANDLE_FIELD, BGM_VOL_MAX );
    }
#endif


    return PROC_RES_FINISH;
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
    WIFIP2PMATCH_WORK* wk = work;

    // BG��������
	GF_BGL_VBlankFunc( wk->bgl );
    // �Z���A�N�^�[
    // Vram�]���}�l�[�W���[���s
    DoVramTransferManager();

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();

	ConnectBGPalAnm_VBlank(&wk->cbp);

	// _retry�֐����Ń}�X�^�[�P�x��ݒ肵�āA
	// ���ꂢ�ɕ��A�ł���悤�ɂ��邽�߂����ŕ��A
	if( wk->brightness_reset == TRUE ){
		WIPE_ResetBrightness( WIPE_DISP_MAIN );
		WIPE_ResetBrightness( WIPE_DISP_SUB );
		wk->brightness_reset = FALSE;
	}
	
// OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}


//--------------------------------------------------------------------------------------------
/**
 * VRAM�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void VramBankSet(void)
{
    GF_BGL_DISPVRAM tbl = {
        GX_VRAM_BG_128_A,				// ���C��2D�G���W����BG
        GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

        GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
        GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

//        GX_VRAM_OBJ_64_E,				// ���C��2D�G���W����OBJ
		GX_VRAM_OBJ_128_B,				// ���C��2D�G���W����OBJ
        GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

        GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
        GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

        GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
        GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
        };
    GF_Disp_SetBank( &tbl );
}

//--------------------------------------------------------------------------------------------
/**
 * BG�ݒ�
 *
 * @param	ini		BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgInit( GF_BGL_INI * ini )
{
    // BG SYSTEM
    {
        GF_BGL_SYS_HEADER BGsys_data = {
            GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
        };
        GF_BGL_InitBG( &BGsys_data );
    }

    // �w�i��
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            0, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME0_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME0_M, 32, 0, HEAPID_WIFIP2PMATCH);
        GF_BGL_ScrClear( ini, GF_BGL_FRAME0_M );


    }

    // ���C�����1
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
            1, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME1_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME1_M, 32, 0, HEAPID_WIFIP2PMATCH);
        GF_BGL_ScrClear( ini, GF_BGL_FRAME1_M );
    }

    // ���X�g�Ȃ�
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_23,
            2, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME2_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME2_M, 32, 0, HEAPID_WIFIP2PMATCH);
        GF_BGL_ScrClear( ini, GF_BGL_FRAME2_M );
    }

#if 1
    // ���낢��Ɏg��
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_23,
            3, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME3_M, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ClearCharSet( GF_BGL_FRAME3_M, 32, 0, HEAPID_WIFIP2PMATCH);
        GF_BGL_ScrClear( ini, GF_BGL_FRAME3_M );
    }
#endif

    // �T�u��ʃe�L�X�g��
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
            3, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME0_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ScrClear( ini, GF_BGL_FRAME0_S );
    }
	// ���[�U�[��ԕ\����
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
            0, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME1_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ScrClear( ini, GF_BGL_FRAME1_S );
    }
	// �{�^��or���[�U�[�f�[�^���̔w�i
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
            2, 0, 0, FALSE
        };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME2_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ScrClear( ini, GF_BGL_FRAME2_S );
    }
	// ���[�U�[�f�[�^�e�L�X�g
    {
        GF_BGL_BGCNT_HEADER TextBgCntDat = {
            0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
            0, 0, 0, FALSE
            };
        GF_BGL_BGControlSet( ini, GF_BGL_FRAME3_S, &TextBgCntDat, GF_BGL_MODE_TEXT );
        GF_BGL_ScrClear( ini, GF_BGL_FRAME3_S );
    }

    GF_BGL_ClearCharSet( GF_BGL_FRAME0_M, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME1_M, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME2_M, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME3_M, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME0_S, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME1_S, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME2_S, 32, 0, HEAPID_WIFIP2PMATCH );
    GF_BGL_ClearCharSet( GF_BGL_FRAME3_S, 32, 0, HEAPID_WIFIP2PMATCH );

    GF_BGL_PrioritySet(GF_BGL_FRAME0_M , 3);  //�͂�����
    GF_BGL_PrioritySet(GF_BGL_FRAME1_M , 1);   // yesno win�@���[�U�[�f�[�^�̔w�i
    GF_BGL_PrioritySet(GF_BGL_FRAME3_M , 0);  // ���b�Z�[�W
    GF_BGL_PrioritySet(GF_BGL_FRAME2_M , 0);   // menu���X�g

    G2_BlendNone();
    G2S_BlendNone();
}

static void WindowSet(void)
{
    GX_SetVisibleWnd( GX_WNDMASK_NONE );
    GXS_SetVisibleWnd( GX_WNDMASK_NONE );
    /*
    GX_SetVisibleWnd( GX_WNDMASK_W0 );
	G2_SetWnd0InsidePlane( GX_WND_PLANEMASK_NONE, 1 );
	G2_SetWndOutsidePlane( GX_WND_PLANEMASK_NONE, 1 );
	G2_SetWnd0Position( 0, 255, 0, 192 );

	GXS_SetVisibleWnd( GX_WNDMASK_W0 );
	G2S_SetWnd0InsidePlane( GX_WND_PLANEMASK_NONE, 1 );
	G2S_SetWndOutsidePlane( GX_WND_PLANEMASK_NONE, 1 );
	G2S_SetWnd0Position( 0, 255, 0, 192 );
     */
}

//----------------------------------------------------------------------------
/**
 *	@brief	������������I�u�W�F�N�g���[�N��ݒ肷��
 *
 *	@param	wk		���[�N
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void MCRSYS_SetMoveObjWork( WIFIP2PMATCH_WORK* wk, MCR_MOVEOBJ* p_obj )
{
	int i;

	for( i=0; i<MATCHROOM_IN_OBJNUM; i++ ){
		if( wk->p_matchMoveObj[i] == NULL ){
			wk->p_matchMoveObj[i] = p_obj;
			return ;
		}
	}
	GF_ASSERT( 0 );
}
//----------------------------------------------------------------------------
/**
 *	@brief	������OBJ�������Ă��郏�[�N��NULL�ɂ���
 *
 *	@param	wk		���[�N
 *	@param	p_obj	NULL�����Ăق������[�N�ɓ����Ă���|�C���^
 */
//-----------------------------------------------------------------------------
static void MCRSYS_DelMoveObjWork( WIFIP2PMATCH_WORK* wk, MCR_MOVEOBJ* p_obj )
{
	int i;

	for( i=0; i<MATCHROOM_IN_OBJNUM; i++ ){
		if( wk->p_matchMoveObj[i] == p_obj ){
			wk->p_matchMoveObj[i] = NULL;
			return ;
		}
	}
	GF_ASSERT( 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	friendNO�̃I�u�W�F�N�g���[�N���擾����
 *
 *	@param	wk			���[�N
 *	@param	friendNo	friendNO
 *
 *	@return	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static MCR_MOVEOBJ* MCRSYS_GetMoveObjWork( WIFIP2PMATCH_WORK* wk, u32 friendNo )
{
	int i;

	for( i=0; i<MATCHROOM_IN_OBJNUM; i++ ){
		if( wk->p_matchMoveObj[i] ){
			if( WIFI_MCR_GetFriendNo( wk->p_matchMoveObj[i] ) == friendNo ){
				return wk->p_matchMoveObj[i];
			}
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�R���f�[�^�Ǎ���
 *
 *	@param	p_bgl		BGL
 *	@param	p_data		�f�[�^�i�[��
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatchFriendListIconLoad( GF_BGL_INI* p_bgl, WIFIP2PMATCH_ICON* p_data, ARCHANDLE* p_handle, u32 heapID )
{
	// �p���b�g�]��
	ArcUtil_HDL_PalSet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCLR,
			PALTYPE_MAIN_BG, PLAYER_DISP_ICON_PLTTOFS*32, 
			PLAYER_DISP_ICON_PLTTNUM*32, heapID );

	ArcUtil_HDL_PalSet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCLR,
			PALTYPE_SUB_BG, PLAYER_DISP_ICON_PLTTOFS_SUB*32, 
			PLAYER_DISP_ICON_PLTTNUM*32, heapID );

	// �L�����N�^�f�[�^�]��
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCGR,
			p_bgl, GF_BGL_FRAME1_M, PLAYER_DISP_ICON_CGX, 0, FALSE, heapID );

	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCGR,
			p_bgl, GF_BGL_FRAME2_S, PLAYER_DISP_ICON_CGX, 0, FALSE, heapID );

	// ����ɃL�����N�^�f�[�^��ۑ����Ă���
	if( p_data->p_charbuff == NULL ){
		p_data->p_charbuff = ArcUtil_HDL_CharDataGet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NCGR, 
				FALSE, &p_data->p_char, heapID);
	}

	// �X�N���[���f�[�^�Ǎ���	
	// ���g���C���ɃX�N���[���f�[�^������ꍇ�͓ǂݍ��܂Ȃ�
	if( p_data->p_buff == NULL ){	
		p_data->p_buff = ArcUtil_HDL_ScrnDataGet( p_handle, NARC_wifip2pmatch_wf_match_all_icon_NSCR, 
				FALSE, &p_data->p_scrn, heapID);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	Icon�f�[�^�͂�
 *
 *	@param	p_data	���[�N
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatchFriendListIconRelease( WIFIP2PMATCH_ICON* p_data )
{
	if( p_data->p_buff != NULL ){
		sys_FreeMemoryEz( p_data->p_buff );
		p_data->p_buff = NULL;
	}
	if( p_data->p_charbuff != NULL ){
		sys_FreeMemoryEz( p_data->p_charbuff );
		p_data->p_charbuff = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�R������������
 *
 *	@param	p_bgl		BGL
 *	@param	p_data		�f�[�^
 *	@param	frm			�t���[��
 *	@param	cx			�L�����N�^�����W
 *	@param	cy			�L�����N�^�����W
 *	@param	icon_type	�A�C�R���^�C�v
 *	@param	col			0=�D 1=��
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatchFriendListIconWrite(  GF_BGL_INI* p_bgl, WIFIP2PMATCH_ICON* p_data, u32 frm, u8 cx, u8 cy, u32 icon_type, u32 col )
{
	int pal;
	// ��������
	GF_BGL_ScrWriteExpand( p_bgl, frm, cx, cy,
			PLAYER_DISP_ICON_SCRN_X, PLAYER_DISP_ICON_SCRN_Y,
			p_data->p_scrn->rawData, 
			PLAYER_DISP_ICON_SCRN_X*icon_type, 0,
			p_data->p_scrn->screenWidth/8, p_data->p_scrn->screenHeight/8 );

	
	// �p���b�g
	pal = WifiP2PMatchBglFrmIconPalGet( frm );

	// �p���b�g�i���o�[�����킹��
	GF_BGL_ScrPalChange( p_bgl, frm, cx, cy,
			PLAYER_DISP_ICON_SCRN_X, PLAYER_DISP_ICON_SCRN_Y, pal+col );

	// �]��
	GF_BGL_LoadScreenV_Req( p_bgl, frm );
}

//----------------------------------------------------------------------------
/**
 *	@brief	Icon��������
 *
 *	@param	p_bgl		BGL
 *	@param	p_data		�f�[�^
 *	@param	frm			�t���[��
 *	@param	cx			�L�����N�^�����W
 *	@param	cy			�L�����N�^�����W
 *	@param	status		���
 *	@param	vctIcon		�{�C�X�`���b�gONOFF�t���O
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatchFriendListStIconWrite( GF_BGL_INI* p_bgl, WIFIP2PMATCH_ICON* p_data, u32 frm, u8 cx, u8 cy, u32 status )
{
	u8 col=0;
	u8 scrn_idx=0;

	switch( status ){
	  case WIFI_STATUS_VCT:      // VCT��
		  scrn_idx = PLAYER_DISP_ICON_IDX_VCT;
		  break;

	  case WIFI_STATUS_SBATTLE50_WAIT:   // �V���O��Lv50�ΐ��W��
	  case WIFI_STATUS_SBATTLE100_WAIT:   // �V���O��Lv100�ΐ��W��
	  case WIFI_STATUS_SBATTLE_FREE_WAIT:   // �V���O��Lv100�ΐ��W��
	  case WIFI_STATUS_DBATTLE50_WAIT:   // �_�u��Lv50�ΐ��W��
	  case WIFI_STATUS_DBATTLE100_WAIT:   // �_�u��Lv100�ΐ��W��
	  case WIFI_STATUS_DBATTLE_FREE_WAIT:   // �_�u��Lv100�ΐ��W��
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_MBATTLE_FREE_WAIT:// �}���`�o�g����W��
#endif
		  col = 1;
	  case WIFI_STATUS_SBATTLE50:      // �V���O���ΐ풆
	  case WIFI_STATUS_SBATTLE100:      // �V���O���ΐ풆
	  case WIFI_STATUS_SBATTLE_FREE:      // �V���O���ΐ풆
	  case WIFI_STATUS_DBATTLE50:      // �_�u���ΐ풆
	  case WIFI_STATUS_DBATTLE100:      // �_�u���ΐ풆
	  case WIFI_STATUS_DBATTLE_FREE:      // �_�u���ΐ풆
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_MBATTLE_FREE:     // �}���`�o�g����
#endif
		  scrn_idx = PLAYER_DISP_ICON_IDX_FIGHT;
		  break;


	  case WIFI_STATUS_TRADE_WAIT:    // ������W��
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_BATTLEROOM_WAIT:// �o�g�����[����W��
#endif
		  col = 1;
	  case WIFI_STATUS_TRADE:          // ������
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_BATTLEROOM:     // �o�g�����[����
#endif
		  scrn_idx = PLAYER_DISP_ICON_IDX_CHANGE;
		  break;


		// �~�j�Q�[��
	  case WIFI_STATUS_BUCKET_WAIT:// �o�P�b�g��W��
	  case WIFI_STATUS_BALANCEBALL_WAIT:// �o�P�b�g��W��
	  case WIFI_STATUS_BALLOON_WAIT:// �o�P�b�g��W��
		  col = 1;

	  case WIFI_STATUS_BUCKET:     // �o�P�b�g��
	  case WIFI_STATUS_BALANCEBALL:     // �o�P�b�g��
	  case WIFI_STATUS_BALLOON:     // �o�P�b�g��
		  scrn_idx = PLAYER_DISP_ICON_IDX_MINIGAME;
		  break;
     

	  case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
		  col = 1;
	  case WIFI_STATUS_POFIN:          // �|�t�B��������
		  scrn_idx = PLAYER_DISP_ICON_IDX_POFIN;
		  break;

	  case WIFI_STATUS_FRONTIER_WAIT:    // �t�����e�B�A��W��
		  col = 1;
	  case WIFI_STATUS_FRONTIER:          // �t�����e�B�A��
		  scrn_idx = PLAYER_DISP_ICON_IDX_FRONTIER;
		  break;

	  case WIFI_STATUS_LOGIN_WAIT:    // �ҋ@���@���O�C������͂���
		  scrn_idx = PLAYER_DISP_ICON_IDX_NORMAL;
		  break;
	  
	  case WIFI_STATUS_NONE:		// ��������
		  scrn_idx = PLAYER_DISP_ICON_IDX_NONE;
		  break;
		  
	  case WIFI_STATUS_UNKNOWN:   // �V���ɍ�����炱�̔ԍ��ȏ�ɂȂ�
	  case WIFI_STATUS_PLAY_OTHER:
		  scrn_idx = PLAYER_DISP_ICON_IDX_UNK;
		  break;
	  default:
		  scrn_idx = PLAYER_DISP_ICON_IDX_UNK;
		  break;
	}

	// ��������
	WifiP2PMatchFriendListIconWrite( p_bgl, p_data, frm, cx, cy, scrn_idx, col );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�ɃA�C�R���f�[�^����������
 *
 *	@param	p_bmp		�r�b�g�}�b�v
 *	@param	p_data		�A�C�R���f�[�^
 *	@param	x			�������݂����W
 *	@param	y			�������݂����W
 *	@param	icon_type	�A�C�R���^�C�v
 *	@param	col			�J���[
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatchFriendListBmpIconWrite(  GF_BGL_BMPWIN* p_bmp, WIFIP2PMATCH_ICON* p_data, u16 x, u16 y, u32 icon_type, u32 col )
{
	int pal;
	
	// ��������
	GF_BGL_BmpWinPrint( p_bmp,
			p_data->p_char->pRawData, 
			(PLAYER_DISP_ICON_SCRN_X*8)*icon_type, 0,
			p_data->p_char->W*8, p_data->p_char->H*8,
			x, y,
			PLAYER_DISP_ICON_SCRN_X*8, PLAYER_DISP_ICON_SCRN_Y*8
			);

	// �p���b�gNo�擾
	pal = WifiP2PMatchBglFrmIconPalGet( p_bmp->frmnum );

	// �p���b�g�i���o�[�����킹��
	GF_BGL_BmpWinSet_Pal( p_bmp, pal+col );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�ɃA�C�R���f�[�^����������
 *
 *	@param	p_bmp		�r�b�g�}�b�v
 *	@param	p_data		�A�C�R���f�[�^
 *	@param	x			�������݂����W
 *	@param	y			�������݂����W
 *	@param	status		���
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatchFriendListBmpStIconWrite( GF_BGL_BMPWIN* p_bmp, WIFIP2PMATCH_ICON* p_data, u16 x, u16 y, u32 status )
{
	u8 col=0;
	u8 scrn_idx=0;

	switch( status ){
	  case WIFI_STATUS_VCT:      // VCT��
		  scrn_idx = PLAYER_DISP_ICON_IDX_VCT;
		  break;

	  case WIFI_STATUS_SBATTLE50_WAIT:   // �V���O��Lv50�ΐ��W��
	  case WIFI_STATUS_SBATTLE100_WAIT:   // �V���O��Lv100�ΐ��W��
	  case WIFI_STATUS_SBATTLE_FREE_WAIT:   // �V���O��Lv100�ΐ��W��
	  case WIFI_STATUS_DBATTLE50_WAIT:   // �_�u��Lv50�ΐ��W��
	  case WIFI_STATUS_DBATTLE100_WAIT:   // �_�u��Lv100�ΐ��W��
	  case WIFI_STATUS_DBATTLE_FREE_WAIT:   // �_�u��Lv100�ΐ��W��
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_MBATTLE_FREE_WAIT:// �}���`�o�g����W��
#endif
		  col = 1;
	  case WIFI_STATUS_SBATTLE50:      // �V���O���ΐ풆
	  case WIFI_STATUS_SBATTLE100:      // �V���O���ΐ풆
	  case WIFI_STATUS_SBATTLE_FREE:      // �V���O���ΐ풆
	  case WIFI_STATUS_DBATTLE50:      // �_�u���ΐ풆
	  case WIFI_STATUS_DBATTLE100:      // �_�u���ΐ풆
	  case WIFI_STATUS_DBATTLE_FREE:      // �_�u���ΐ풆
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_MBATTLE_FREE:     // �}���`�o�g����
#endif
		  scrn_idx = PLAYER_DISP_ICON_IDX_FIGHT;
		  break;


	  case WIFI_STATUS_TRADE_WAIT:    // ������W��
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_BATTLEROOM_WAIT:// �o�g�����[����W��
#endif
		  col = 1;
	  case WIFI_STATUS_TRADE:          // ������
#ifdef WFP2P_DEBUG_EXON 
	  case WIFI_STATUS_BATTLEROOM:     // �o�g�����[����
#endif
		  scrn_idx = PLAYER_DISP_ICON_IDX_CHANGE;
		  break;


	  case WIFI_STATUS_BUCKET_WAIT:// �o�P�b�g��W��
	  case WIFI_STATUS_BALANCEBALL_WAIT:// �o�P�b�g��W��
	  case WIFI_STATUS_BALLOON_WAIT:// �o�P�b�g��W��
		  col = 1;

	  case WIFI_STATUS_BUCKET:     // �o�P�b�g��
	  case WIFI_STATUS_BALANCEBALL:     // �o�P�b�g��
	  case WIFI_STATUS_BALLOON:     // �o�P�b�g��
		  scrn_idx = PLAYER_DISP_ICON_IDX_MINIGAME;
		  break;
		  

	  case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
		  col = 1;
	  case WIFI_STATUS_POFIN:          // �|�t�B��������
		  scrn_idx = PLAYER_DISP_ICON_IDX_POFIN;
		  break;

	  case WIFI_STATUS_FRONTIER_WAIT:    // �t�����e�B�A��W��
		  col = 1;
	  case WIFI_STATUS_FRONTIER:          // �t�����e�B�A��
		  scrn_idx = PLAYER_DISP_ICON_IDX_FRONTIER;
		  break;

	  case WIFI_STATUS_LOGIN_WAIT:    // �ҋ@���@���O�C������͂���
		  scrn_idx = PLAYER_DISP_ICON_IDX_NORMAL;
		  break;
	  
	  case WIFI_STATUS_NONE:   // ��������
		  scrn_idx = PLAYER_DISP_ICON_IDX_NONE;
		  break;
		  
	  case WIFI_STATUS_UNKNOWN:   // �V���ɍ�����炱�̔ԍ��ȏ�ɂȂ�
	  case WIFI_STATUS_PLAY_OTHER:   // �V���ɍ�����炱�̔ԍ��ȏ�ɂȂ�
		  scrn_idx = PLAYER_DISP_ICON_IDX_UNK;
		  break;
	  default:
		  scrn_idx = PLAYER_DISP_ICON_IDX_UNK;
		  break;
	}
	
	WifiP2PMatchFriendListBmpIconWrite( p_bmp, p_data, x, y, scrn_idx, col );
}

//----------------------------------------------------------------------------
/**
 *	@brief	frm�̃A�C�R���p���b�g�i���o�[��Ԃ�
 *
 *	@param	frm		BGL�@FRAME
 *
 *	@return �p���b�gNO
 */
//-----------------------------------------------------------------------------
static int WifiP2PMatchBglFrmIconPalGet( u32 frm )
{
	if( frm < GF_BGL_FRAME0_S ){
		return PLAYER_DISP_ICON_PLTTOFS;
	}
	return PLAYER_DISP_ICON_PLTTOFS_SUB;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�����h���X�g�J�n�錾
 */
//-----------------------------------------------------------------------------
static int WifiP2PMatchFriendListStart( void )
{
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
					COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);

	// ������BGM�ݒ�
	WifiP2P_SetLobbyBgm();	

	return WIFIP2PMATCH_FRIENDLIST_INIT;
}

#define TALK_MESSAGE_BUF_NUM	( 190*2 )
#define TITLE_MESSAGE_BUF_NUM	( 90*2 )

//------------------------------------------------------------------
/**
 * ���[�N������
 *
 * @param   wk		WIFIP2PMATCH_WORK*
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void InitWork( WIFIP2PMATCH_WORK *wk )
{
    int i;
    int flag;

#if 0
    for(i=0;i<WIFIP2PMATCH_MEMBER_MAX;i++){
        wk->TrainerName[i] = STRBUF_Create( PERSON_NAME_SIZE+EOM_SIZE, HEAPID_WIFIP2PMATCH );
        STRBUF_SetStringCode(wk->TrainerName[i] ,WifiList_GetFriendNamePtr(wk->pList,i));
    }
#endif
    wk->TalkString =  STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_WIFIP2PMATCH );
    wk->pTemp = STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_WIFIP2PMATCH );        // ���͓o�^���̈ꎞ�o�b�t�@


#if 0
    // ������o�b�t�@�쐬
    for(i = 0;i < 4;i++){
        wk->MenuString[i]  = STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_WIFIP2PMATCH );
    }
#endif
    wk->pExpStrBuf = STRBUF_Create( TALK_MESSAGE_BUF_NUM, HEAPID_WIFIP2PMATCH );
    wk->TitleString = STRBUF_Create( TITLE_MESSAGE_BUF_NUM, HEAPID_WIFIP2PMATCH );

    wk->seq = WIFIP2PMATCH_MODE_INIT;

}

//------------------------------------------------------------------
/**
 * $brief   ���[�N���
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void FreeWork( WIFIP2PMATCH_WORK *wk )
{
    int i;


#if 0
    for(i=0;i<WIFIP2PMATCH_MEMBER_MAX;i++){
        STRBUF_Delete( wk->TrainerName[i] );
    }
#endif
    STRBUF_Delete( wk->TitleString );
    STRBUF_Delete( wk->TalkString );
    STRBUF_Delete( wk->pTemp );

#if 0
    for(i = 0;i < 4;i++){
        STRBUF_Delete(wk->MenuString[i]);
    }
#endif
    STRBUF_Delete(wk->pExpStrBuf);

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

    GF_BGL_BGControlExit( ini, GF_BGL_FRAME3_S );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME2_S );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME1_S );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_S );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME3_M );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME2_M );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME1_M );
    GF_BGL_BGControlExit( ini, GF_BGL_FRAME0_M );

    sys_FreeMemoryEz( ini );
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


static void BgGraphicSet( WIFIP2PMATCH_WORK * wk, ARCHANDLE* p_handle )
{
    GF_BGL_INI *bgl = wk->bgl;

    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );
    GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );

    // �㉺��ʂa�f�p���b�g�]��
    ArcUtil_HDL_PalSet(    p_handle, NARC_wifip2pmatch_conect_NCLR, PALTYPE_MAIN_BG, 0, 0,  HEAPID_WIFIP2PMATCH);
    ArcUtil_HDL_PalSet(    p_handle, NARC_wifip2pmatch_conect_NCLR, PALTYPE_SUB_BG,  0, 0,  HEAPID_WIFIP2PMATCH);

    // ��b�t�H���g�p���b�g�]��
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, _NUKI_FONT_PALNO*0x20, HEAPID_WIFIP2PMATCH );
    TalkFontPaletteLoad( PALTYPE_MAIN_BG, COMM_MESFONT_PAL*0x20, HEAPID_WIFIP2PMATCH );
    //	TalkFontPaletteLoad( PALTYPE_SUB_BG,  13*0x20, HEAPID_WIFIP2PMATCH );


    // ���C�����BG2�L�����]��
    ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_conect_NCGR, bgl,
                       GF_BGL_FRAME0_M, 0, 0, 0, HEAPID_WIFIP2PMATCH);

    // ���C�����BG2�X�N���[���]��
    ArcUtil_HDL_ScrnSet(   p_handle, NARC_wifip2pmatch_conect_01_NSCR, bgl,
                       GF_BGL_FRAME0_M, 0, 0, 0, HEAPID_WIFIP2PMATCH);


    // �T�u���BG0�L�����]��
    ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_conect_sub_NCGR, bgl, GF_BGL_FRAME0_S, 0, 0, 0, HEAPID_WIFIP2PMATCH);

    // �T�u���BG0�X�N���[���]��
    ArcUtil_HDL_ScrnSet(   p_handle, NARC_wifip2pmatch_conect_sub_NSCR, bgl, GF_BGL_FRAME0_S, 0, 0, 0, HEAPID_WIFIP2PMATCH);

    {
        int type = CONFIG_GetWindowType(SaveData_GetConfig(wk->pSaveData));
        TalkWinGraphicSet(
            bgl, GF_BGL_FRAME2_M, COMM_TALK_WIN_CGX_NUM, COMM_MESFRAME_PAL,  type, HEAPID_WIFIP2PMATCH );
        MenuWinGraphicSet(
            bgl, GF_BGL_FRAME2_M, MENU_WIN_CGX_NUM, MENU_WIN_PAL, 0, HEAPID_WIFIP2PMATCH );
        MenuWinGraphicSet(
            bgl, GF_BGL_FRAME1_M, MENU_WIN_CGX_NUM, MENU_WIN_PAL, 0, HEAPID_WIFIP2PMATCH );
    }
    
}


//** CharManager PlttManager�p **//
#define WIFIP2PMATCH_CHAR_CONT_NUM				(20)
#define WIFIP2PMATCH_CHAR_VRAMTRANS_MAIN_SIZE		(128*1024)
#define WIFIP2PMATCH_CHAR_VRAMTRANS_SUB_SIZE		(16*1024)
#define WIFIP2PMATCH_PLTT_CONT_NUM				(20)

//-------------------------------------
//
//	�L�����N�^�}�l�[�W���[
//	�p���b�g�}�l�[�W���[�̏�����
//
//=====================================
static void char_pltt_manager_init(void)
{
    // �L�����N�^�}�l�[�W���[������
    {
        CHAR_MANAGER_MAKE cm = {
            WIFIP2PMATCH_CHAR_CONT_NUM,
            WIFIP2PMATCH_CHAR_VRAMTRANS_MAIN_SIZE,
            WIFIP2PMATCH_CHAR_VRAMTRANS_SUB_SIZE,
            HEAPID_WIFIP2PMATCH
        };
        InitCharManagerReg(&cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K );
    }
    // �p���b�g�}�l�[�W���[������
    InitPlttManager(WIFIP2PMATCH_PLTT_CONT_NUM, HEAPID_WIFIP2PMATCH);

    // �ǂݍ��݊J�n�ʒu��������
    CharLoadStartAll();
    PlttLoadStartAll();
	//�ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);
}


//------------------------------------------------------------------
/**
 * �Z���A�N�^�[������
 *
 * @param   wk		���[�_�[�\���̂̃|�C���^
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void InitCellActor(WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle)
{
    int i;


    // OAM�}�l�[�W���[�̏�����
    NNS_G2dInitOamManagerModule();

    // ���LOAM�}�l�[�W���쐬
    // �����_���pOAM�}�l�[�W���쐬
    // �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
    REND_OAMInit(
        0, 126,		// ���C�����OAM�Ǘ��̈�
        0, 32,		// ���C����ʃA�t�B���Ǘ��̈�
        0, 126,		// �T�u���OAM�Ǘ��̈�
        0, 32,		// �T�u��ʃA�t�B���Ǘ��̈�
        HEAPID_WIFIP2PMATCH);


    // �Z���A�N�^�[������
    wk->clactSet = CLACT_U_SetEasyInit( WF_CLACT_WKNUM, &wk->renddata, HEAPID_WIFIP2PMATCH );

    CLACT_U_SetSubSurfaceMatrix( &wk->renddata, 0, NAMEIN_SUB_ACTOR_DISTANCE );


    //���\�[�X�}�l�[�W���[������
    for(i=0;i<CLACT_RESOURCE_NUM;i++){		//���\�[�X�}�l�[�W���[�쐬
        wk->resMan[i] = CLACT_U_ResManagerInit(WF_CLACT_RESNUM, i, HEAPID_WIFIP2PMATCH);
    }

	// FontOAM�V�X�e���쐬
	wk->fontoam = FONTOAM_SysInit( WF_FONTOAM_NUM, HEAPID_WIFIP2PMATCH );

	
#if 0
    //---------���ʗp-------------------

    //chara�ǂݍ���
    wk->resObjTbl[MAIN_LCD][CLACT_U_CHAR_RES] =
        CLACT_U_ResManagerResAddArcChar_ArcHandle(wk->resMan[CLACT_U_CHAR_RES],
                                        p_handle,
                                        NARC_wifip2pmatch_conect_obj_NCGR, FALSE, 0, NNS_G2D_VRAM_TYPE_2DMAIN, HEAPID_WIFIP2PMATCH);

    //pal�ǂݍ���
    wk->resObjTbl[MAIN_LCD][CLACT_U_PLTT_RES] =
        CLACT_U_ResManagerResAddArcPltt_ArcHandle(wk->resMan[CLACT_U_PLTT_RES],
                                        p_handle,
                                        NARC_wifip2pmatch_conect_obj_NCLR, 0, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 7, HEAPID_WIFIP2PMATCH);

    //cell�ǂݍ���
    wk->resObjTbl[MAIN_LCD][CLACT_U_CELL_RES] =
        CLACT_U_ResManagerResAddArcKindCell_ArcHandle(wk->resMan[CLACT_U_CELL_RES],
                                            p_handle,
                                            NARC_wifip2pmatch_conect_NCER, FALSE, 0, CLACT_U_CELL_RES,HEAPID_WIFIP2PMATCH);

    //�����֐���anim�ǂݍ���
    wk->resObjTbl[MAIN_LCD][CLACT_U_CELLANM_RES] =
        CLACT_U_ResManagerResAddArcKindCell_ArcHandle(wk->resMan[CLACT_U_CELLANM_RES],
                                            p_handle,
                                            NARC_wifip2pmatch_conect_NANR, FALSE, 0, CLACT_U_CELLANM_RES,HEAPID_WIFIP2PMATCH);


    // ���\�[�X�}�l�[�W���[����]��

    // Chara�]��
    CLACT_U_CharManagerSetAreaCont( wk->resObjTbl[MAIN_LCD][CLACT_U_CHAR_RES] );
    //	CLACT_U_CharManagerSet( wk->resObjTbl[SUB_LCD][CLACT_U_CHAR_RES] );

    // �p���b�g�]��
    CLACT_U_PlttManagerSetCleanArea( wk->resObjTbl[MAIN_LCD][CLACT_U_PLTT_RES] );
    //	CLACT_U_PlttManagerSet( wk->resObjTbl[SUB_LCD][CLACT_U_PLTT_RES] );

#endif

}

#define TRAINER_NAME_POS_X		( 24 )
#define TRAINER_NAME_POS_Y		( 32 )
#define TRAINER_NAME_POS_SPAN	( 32 )

#define TRAINER_NAME_WIN_X		(  3 )
#define TRAINER1_NAME_WIN_Y		(  6 )
#define TRAINER2_NAME_WIN_Y		(  7 )

//------------------------------------------------------------------
/**
 * �Z���A�N�^�[�o�^
 *
 * @param   wk			WIFIP2PMATCH_WORK*
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void SetCellActor(WIFIP2PMATCH_WORK *wk)
{
#if 0
    int i;
    // �Z���A�N�^�[�w�b�_�쐬
    CLACT_U_MakeHeader(&wk->clActHeader_m, 0, 0, 0, 0, CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
                       0, 0,
                       wk->resMan[CLACT_U_CHAR_RES],
                       wk->resMan[CLACT_U_PLTT_RES],
                       wk->resMan[CLACT_U_CELL_RES],
                       wk->resMan[CLACT_U_CELLANM_RES],
                       NULL,NULL);

    /*
	CLACT_U_MakeHeader(&wk->clActHeader_s, 1, 1, 1, 1, CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
	0, 0,
	wk->resMan[CLACT_U_CHAR_RES],
	wk->resMan[CLACT_U_PLTT_RES],
	wk->resMan[CLACT_U_CELL_RES],
	wk->resMan[CLACT_U_CELLANM_RES],
	NULL,NULL);
     */
    {
        //�o�^���i�[
        CLACT_ADD add;

        add.ClActSet	= wk->clactSet;
        add.ClActHeader	= &wk->clActHeader_m;

        add.mat.z		= 0;
        add.sca.x		= FX32_ONE;
        add.sca.y		= FX32_ONE;
        add.sca.z		= FX32_ONE;
        add.rot			= 0;
        add.pri			= 1;
        add.DrawArea	= NNS_G2D_VRAM_TYPE_2DMAIN;
        add.heap		= HEAPID_WIFIP2PMATCH;

        //�Z���A�N�^�[�\���J�n
        // �T�u��ʗp(���̓o�^�j
        for(i=0;i < _OAM_NUM;i++){
            add.mat.x = FX32_ONE *   TRAINER_NAME_POS_X;
            add.mat.y = FX32_ONE * ( TRAINER_NAME_POS_Y + TRAINER_NAME_POS_SPAN*i ) + NAMEIN_SUB_ACTOR_DISTANCE;
            wk->MainActWork[i] = CLACT_Add(&add);
            CLACT_SetAnmFlag(wk->MainActWork[i],1);
            CLACT_AnmChg( wk->MainActWork[i], i );
            CLACT_BGPriorityChg(wk->MainActWork[i], 2);
            CLACT_SetDrawFlag( wk->MainActWork[i], 0 );
        }

    }
#endif
    GF_Disp_GX_VisibleControl(  GX_PLANEMASK_OBJ, VISIBLE_ON );	//���C�����OBJ�ʂn�m
    //	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );	//�T�u���OBJ�ʂn�m
}


//------------------------------------------------------------------
/**
 * BMPWIN�����i�����p�l���Ƀt�H���g�`��j
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void MainMenuMsgInit(WIFIP2PMATCH_WORK *wk)
{
	//�@�^�C�g���͂���Ȃ��Ȃ�܂���
#if 1	
	
    int i,col;

    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWinBack)){
        BmpMenuWinClear(&wk->MyInfoWinBack, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->MyInfoWinBack);
    }
    GF_BGL_BmpWinAdd(wk->bgl, &wk->MyInfoWinBack, GF_BGL_FRAME3_M, 5, 1, 22, 2,
                     _NUKI_FONT_PALNO,  _CGX_TITLE_BOTTOM );
    GF_BGL_BmpWinDataFill( &wk->MyInfoWinBack, 0x8000 );
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_018, wk->TitleString );
    GF_STR_PrintColor( &wk->MyInfoWinBack, FONT_TALK, wk->TitleString, 0, 0, MSG_NO_PUT,_COL_N_WHITE, NULL);
    GF_BGL_BmpWinOnVReq(&wk->MyInfoWinBack);
#endif

}


// �͂��E������BMP�i����ʁj
#define YESNO_WIN_FRAME_CHAR	( 1 + TALK_WIN_CGX_SIZ )
#define YESNO_CHARA_OFFSET		( 1 + TALK_WIN_CGX_SIZ + MENU_WIN_CGX_SIZ + FLD_MSG_WIN_SX*FLD_MSG_WIN_SY )
#define YESNO_WINDOW_X			( 22 )
#define YESNO_WINDOW_Y1			(  7 )
#define YESNO_WINDOW_Y2			( 13 )
#define YESNO_CHARA_W			(  8 )
#define YESNO_CHARA_H			(  4 )

static const BMPWIN_DAT TouchYesNoBmpDat[2]={
    {
        GF_BGL_FRAME0_M, YESNO_WINDOW_X, YESNO_WINDOW_Y1,
        YESNO_CHARA_W, YESNO_CHARA_H, 13, YESNO_CHARA_OFFSET
        },
    {
        GF_BGL_FRAME0_M, YESNO_WINDOW_X, YESNO_WINDOW_Y2,
        YESNO_CHARA_W, YESNO_CHARA_H, 13, YESNO_CHARA_OFFSET+YESNO_CHARA_W*YESNO_CHARA_H

        }
};

//------------------------------------------------------------------
/**
 * $brief   �m�ۂ���BMPWIN�����
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void BmpWinDelete( WIFIP2PMATCH_WORK *wk )
{
    int i;

	// ���b�Z�[�W��~
	EndMessageWindowOff( wk );

    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWinBack)){
        GF_BGL_BmpWinDel(&wk->MyInfoWinBack);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MsgWin)){
        GF_BGL_BmpWinDel(&wk->MsgWin);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MyWin)){
        GF_BGL_BmpWinDel(&wk->MyWin);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWin)){
        GF_BGL_BmpWinDel(&wk->MyInfoWin);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->ListWin)){
        GF_BGL_BmpWinDel(&wk->ListWin);
    }
    if(wk->lw){
        BmpListExit(wk->lw, NULL, NULL);
    }
    if(wk->menulist){
        BMP_MENULIST_Delete( wk->menulist );
        wk->menulist = NULL;
    }
    
    if(GF_BGL_BmpWinAddCheck(&wk->SysMsgWin)){
        GF_BGL_BmpWinDel(&wk->SysMsgWin);
    }
}


//------------------------------------------------------------------
/**
 * $brief   �J�n���̃��b�Z�[�W   WIFIP2PMATCH_MODE_INIT
 * @param   wk
 * @param   seq
 * @retval  none
 */
//------------------------------------------------------------------

static const BMPWIN_DAT _yesNoBmpDatSys = {
    GF_BGL_FRAME2_M, FLD_YESNO_WIN_PX, FLD_YESNO_WIN_PY,
    FLD_YESNO_WIN_SX, FLD_YESNO_WIN_SY, FLD_YESNO_WIN_PAL, YESNO_WIN_CGX
    };
static const BMPWIN_DAT _yesNoBmpDatSys2 = {
    GF_BGL_FRAME1_M, FLD_YESNO_WIN_PX, FLD_YESNO_WIN_PY+6,
    FLD_YESNO_WIN_SX, FLD_YESNO_WIN_SY, FLD_YESNO_WIN_PAL, FRAME1_YESNO_WIN_CGX
    };

static int WifiP2PMatch_MainInit( WIFIP2PMATCH_WORK *wk, int seq )
{
	u32 status;
	
    OHNO_PRINT("�T�E���h��������\n");
    if(CommStateIsWifiConnect()){       // �ڑ���

//        WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��
		
        _friendNameExpand(wk, mydwc_getFriendIndex());
        wk->pMatch = CommStateGetMatchWork();
        wk->pMatch->myMatchStatus.vchat = wk->pMatch->myMatchStatus.vchat_org;

		// status�擾
		status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );

        if( (status == WIFI_STATUS_TRADE) ||
			(status == WIFI_STATUS_POFIN) ||
			(status == WIFI_STATUS_FRONTIER) ||
#ifdef WFP2P_DEBUG_EXON 
			(status == WIFI_STATUS_BATTLEROOM) ||
#endif
			(status == WIFI_STATUS_BUCKET) ||
			(status == WIFI_STATUS_BUCKET_WAIT) ||
			(status == WIFI_STATUS_BALANCEBALL) ||
			(status == WIFI_STATUS_BALANCEBALL_WAIT) ||
			(status == WIFI_STATUS_BALLOON) ||
			(status == WIFI_STATUS_BALLOON_WAIT) ||
			(status == WIFI_STATUS_POFIN_WAIT)
			){	// ������or�|�t�B���Ȃ�

#if 0		//080611	tomoya �g���q�[�v���������}�b�v�̈�ԏ�ɂȂ�悤�ɍ쐬
			// �t�����e�B�A����߂��Ă��Ă��A�g���q�[�v���쐬
			if( (status == WIFI_STATUS_FRONTIER) ||
				(status == WIFI_STATUS_TRADE) ){
				// �o�g������߂��Ă����Ȃ�g���q�[�v�쐬
				sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCHEX, WIFI_P2PMATCH_EXHEAP_SIZE );
			}
#endif


			// 080605 tomoya BTS:249�̑Ώ��̂���
			// �e�̒ʐM���Z�b�g�����㒼���ɐڑ����Ă��܂��̂����
			// �X�e�[�^�X��LOGIN_WAIT�ɕύX
			_myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
			
            CommInfoFinalize();
            CommStateWifiTradeMatchEnd();	// �}�b�`���O��؂�
            wk->pMatch = CommStateGetMatchWork();
            wk->pMatch->myMatchStatus.vchat = wk->pMatch->myMatchStatus.vchat_org;
            wk->timer = _RECONECTING_WAIT_TIME;
            wk->seq = WIFIP2PMATCH_RECONECTING_WAIT;

        }
        else{
			// �o�g������߂��Ă����Ƃ�
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_044, FALSE);

			// ����[�`�F�b�N�ĊJ
		    CommStateSetErrorCheck(TRUE,TRUE);	
            CommTimingSyncStart(_TIMING_BATTLE_END);
            wk->seq = WIFIP2PMATCH_NEXTBATTLE_YESNO;

#if 0		//080611	tomoya �g���q�[�v���������}�b�v�̈�ԏ�ɂȂ�悤�ɍ쐬
			// �o�g������߂��Ă����Ȃ�g���q�[�v�쐬
			sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCHEX, WIFI_P2PMATCH_EXHEAP_SIZE );
#endif
        }
        /*        CommStateWifiMatchEnd();
        wk->pMatch = CommStateGetMatchWork();
        wk->seq = WIFIP2PMATCH_FRIENDLIST_INIT;   */
    }
    else{        // �ڑ����܂�

#if 0	//	080611	tomoya �g���q�[�v���������}�b�v�̈�ԏ�ɂȂ�悤�ɍ쐬
		// �ʐM�O�Ȃ�g���q�[�v�쐬
		sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFIP2PMATCHEX, WIFI_P2PMATCH_EXHEAP_SIZE );
#endif

		//08.07.03
		Snd_DataSetByScene( SND_SCENE_P2P, SEQ_WIFILOBBY, 1 );

        MainMenuMsgInit(wk);// BMPWIN�o�^�E�`��
        if( !DWC_CheckHasProfile( WifiList_GetMyUserInfo(wk->pList) ) ){
            GF_BGL_PrioritySet(GF_BGL_FRAME0_M , 3);  //�͂�����
            GF_BGL_PrioritySet(GF_BGL_FRAME1_M , 0);   // yesno win
            GF_BGL_PrioritySet(GF_BGL_FRAME3_M , 0);  // ���b�Z�[�W
            GF_BGL_PrioritySet(GF_BGL_FRAME2_M , 1);   // menulist
            _systemMessagePrint(wk, dwc_message_0003);
            wk->pYesNoWork =
                BmpYesNoSelectInit( wk->bgl,
                                    &_yesNoBmpDatSys2,
                                    MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                    HEAPID_WIFIP2PMATCH );
            wk->seq = WIFIP2PMATCH_FIRST_YESNO;
        }
        else if( !DWC_CheckValidConsole(WifiList_GetMyUserInfo(wk->pList)) ){
            _systemMessagePrint(wk, dwc_message_0005);
            wk->seq = WIFIP2PMATCH_DIFFER_MACHINE_INIT;
        }
        else{
            WifiP2PMatchMessagePrint(wk, dwc_message_0002, TRUE);
            wk->seq = WIFIP2PMATCH_NORMALCONNECT_YESNO;
        }
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ʏ�̐ڑ�  WIFIP2PMATCH_NORMALCONNECT_YESNO
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int _normalConnectYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDatSys,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_NORMALCONNECT_WAIT;
    }
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   �ʏ�̐ڑ�  WIFIP2PMATCH_NORMALCONNECT_WAIT
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int _normalConnectWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }
    else{
        //BmpTalkWinClear( &wk->SysMsgWin, WINDOW_TRANS_ON );
        if(ret == 0){ // �͂���I�������ꍇ
            //�ڑ��J�n
            wk->pMatch = CommStateWifiEnterLogin(wk->pSaveData,sizeof(TEST_MATCH_WORK));

            WifiP2PMatchMessagePrint(wk, dwc_message_0008, TRUE);
			GF_ASSERT( wk->timeWaitWork == NULL );
            wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
            wk->seq = WIFIP2PMATCH_CONNECTING_INIT;
        }
        else{  // ��������I�������ꍇ
            wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        }
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ʂ�DS�������ꍇ  WIFIP2PMATCH_DIFFER_MACHINE_INIT
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _differMachineInit( WIFIP2PMATCH_WORK *wk, int seq )
{
    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        _systemMessagePrint(wk,dwc_message_0006);
        GF_BGL_PrioritySet(GF_BGL_FRAME3_M , 0);  // ���b�Z�[�W
        GF_BGL_PrioritySet(GF_BGL_FRAME2_M , 1);   //
        GF_BGL_PrioritySet(GF_BGL_FRAME1_M , 0);   // yesno win
        GF_BGL_PrioritySet(GF_BGL_FRAME0_M , 3);  //�͂�����
        wk->pYesNoWork =
				BmpYesNoSelectInitEx( wk->bgl,
                                &_yesNoBmpDatSys2,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                1, HEAPID_WIFIP2PMATCH );	// �������@����n�܂�
/*            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDatSys2,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );//*/
        wk->seq = WIFIP2PMATCH_DIFFER_MACHINE_NEXT;
    }

    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ʂ�DS�������ꍇ  WIFIP2PMATCH_DIFFER_MACHINE_NEXT
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _differMachineNext( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }
    else{
        BmpTalkWinClear( &wk->SysMsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel( &wk->SysMsgWin );
        if(ret == 0){ // �͂���I�������ꍇ
            _systemMessagePrint(wk,dwc_message_0007);
            wk->pYesNoWork =
                BmpYesNoSelectInitEx( wk->bgl,
                                    &_yesNoBmpDatSys2,
                                    MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                    1, HEAPID_WIFIP2PMATCH );	// �������@����n�߂�
/*
                BmpYesNoSelectInit( wk->bgl,
                                    &_yesNoBmpDatSys2,
                                    MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                    HEAPID_WIFIP2PMATCH );//*/
            wk->seq = WIFIP2PMATCH_DIFFER_MACHINE_ONEMORE;
        }
        else{  // ��������I�������ꍇ
            wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        }
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ʂ�DS�������ꍇ  WIFIP2PMATCH_DIFFER_MACHINE_ONEMORE
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _differMachineOneMore( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }
    else{

		// �D�揇�ʂ�߂�
        GF_BGL_PrioritySet(GF_BGL_FRAME3_M , 0);  // ���b�Z�[�W
        GF_BGL_PrioritySet(GF_BGL_FRAME2_M , 0);   //
        GF_BGL_PrioritySet(GF_BGL_FRAME1_M , 1);   // yesno win
        GF_BGL_PrioritySet(GF_BGL_FRAME0_M , 3);  //�͂�����
		
        BmpTalkWinClear( &wk->SysMsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel( &wk->SysMsgWin );
        if(ret == 0){ // �͂���I�������ꍇ
            //�ڑ��J�n
            WifiList_Init(wk->pList);
            EMAILSAVE_Init(SaveData_Get(wk->pSaveData, GMDATA_ID_EMAIL));
            FrontierRecord_DataCrear(SaveData_GetFrontier(wk->pSaveData));

            wk->seq = WIFIP2PMATCH_CONNECTING_INIT;
            wk->pMatch = CommStateWifiEnterLogin(wk->pSaveData,sizeof(TEST_MATCH_WORK));
            WifiP2PMatchMessagePrint(wk, dwc_message_0008, TRUE);
			GF_ASSERT( wk->timeWaitWork == NULL );
            wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
            wk->bInitMessage = TRUE;
        }
        else{  // ��������I�������ꍇ
            wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        }
    }
    return seq;
}

//WIFIP2PMATCH_FIRST_YESNO
//------------------------------------------------------------------
/**
 * $brief   ����ڑ����̊m�F  WIFIP2PMATCH_FIRST_YESNO
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _firstYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }
    else{

		// �\���D�揇�ʂ�߂�
		GF_BGL_PrioritySet(GF_BGL_FRAME0_M , 3);  //�͂�����
		GF_BGL_PrioritySet(GF_BGL_FRAME1_M , 1);   // yesno win
		GF_BGL_PrioritySet(GF_BGL_FRAME3_M , 0);  // ���b�Z�[�W
		GF_BGL_PrioritySet(GF_BGL_FRAME2_M , 0);   // menulist
		
        BmpTalkWinClear( &wk->SysMsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel( &wk->SysMsgWin );
        if(ret == 0){ // �͂���I�������ꍇ
            //�ڑ��J�n
            wk->seq = WIFIP2PMATCH_CONNECTING_INIT;
            wk->pMatch = CommStateWifiEnterLogin(wk->pSaveData,sizeof(TEST_MATCH_WORK));
            WifiP2PMatchMessagePrint(wk, dwc_message_0008, TRUE);
			GF_ASSERT( wk->timeWaitWork == NULL );
            wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
            wk->bInitMessage = TRUE;
        }
        else{  // ��������I�������ꍇ
            wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        }
    }

    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ���O�C���ڑ���  WIFIP2PMATCH_CONNECTING_INIT
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int WifiP2PMatch_ConnectingInit( WIFIP2PMATCH_WORK *wk, int seq )
{
    GF_BGL_PrioritySet(GF_BGL_FRAME0_M , 3);  //�͂�����
    GF_BGL_PrioritySet(GF_BGL_FRAME1_M , 1);   // yesno win
    GF_BGL_PrioritySet(GF_BGL_FRAME3_M , 0);  // ���b�Z�[�W
    GF_BGL_PrioritySet(GF_BGL_FRAME2_M , 0);   //
    wk->seq = WIFIP2PMATCH_CONNECTING;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   MyMatchStatus�쐬
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static void _makeMyMatchStatus(WIFIP2PMATCH_WORK* wk, int status)
{
    MYSTATUS* pMyStatus = SaveData_GetMyStatus(wk->pSaveData);
    ZUKAN_WORK* pZukan = SaveData_GetZukanWork(wk->pSaveData);
    POKEPARTY* pPokeParty = SaveData_GetTemotiPokemon(wk->pSaveData);
    WIFI_HISTORY* pHistry = SaveData_GetWifiHistory(wk->pSaveData);
    int max,i;

    OHNO_PRINT("_WIFI_STATUS_MAX_SIZE  %d\n",sizeof(_WIFI_MACH_STATUS));
    GF_ASSERT(_WIFI_STATUS_MAX_SIZE == sizeof(_WIFI_MACH_STATUS));

    max = PokeParty_GetPokeCount(pPokeParty);
    for(i = 0;i < max;i++){
        POKEMON_PARAM* poke = PokeParty_GetMemberPointer( pPokeParty, i);
        wk->pMatch->myMatchStatus.pokemonType[i] = PokeParaGet( poke, ID_PARA_monsno, NULL );
        wk->pMatch->myMatchStatus.hasItemType[i] = PokeParaGet( poke, ID_PARA_item, NULL );
    }
    wk->pMatch->myMatchStatus.version = MyStatus_GetRomCode(pMyStatus);
    wk->pMatch->myMatchStatus.regionCode = MyStatus_GetRegionCode(pMyStatus);
    wk->pMatch->myMatchStatus.pokemonZukan = ZukanWork_GetZenkokuZukanFlag(pZukan);
//    wk->pMatch->myMatchStatus.status = status;
    wk->pMatch->myMatchStatus.status = WIFI_STATUS_UNKNOWN;
    _myStatusChange_not_send(wk, status);	// BGM��ԂȂǂ𒲐�
    wk->pMatch->myMatchStatus.regulation = _REGULATION_BATTLE_TOWER;
    wk->pMatch->myMatchStatus.trainer_view = MyStatus_GetTrainerView(pMyStatus);
    wk->pMatch->myMatchStatus.sex = MyStatus_GetMySex(pMyStatus);
    wk->pMatch->myMatchStatus.nation = WIFIHISTORY_GetMyNation(pHistry);
    wk->pMatch->myMatchStatus.area = WIFIHISTORY_GetMyArea(pHistry);
    wk->pMatch->myMatchStatus.vchat = TRUE;
    wk->pMatch->myMatchStatus.vchat_org = TRUE;

    mydwc_setMyInfo((const char *)&wk->pMatch->myMatchStatus, _WIFI_STATUS_MAX_SIZE);
    mydwc_setFriendStateBuffer(&wk->pMatch->friendMatchStatus[0], _WIFI_STATUS_MAX_SIZE);

}

//------------------------------------------------------------------
/**
 * $brief   �t�����h��status�ǂݍ���
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _readFriendMatchStatus(WIFIP2PMATCH_WORK* wk)
{
    int i,num = 0;
	_WIFI_MACH_STATUS* p_status;
	u32 status;

//    MI_CpuFill8(wk->index2No, 0, sizeof(WIFIP2PMATCH_MEMBER_MAX));
    for(i = 0;i < WIFIP2PMATCH_MEMBER_MAX;i++){
        wk->index2No[i] = 0;

		// ���̐l�͎����̗F�B���H
		if( wk->friend_num > i ){
			// ���̃`�F�b�N�������i��������
			// �ŐV������M���ď��߂ēo�^�����
			if(DWC_STATUS_OFFLINE != WifiDwc_getFriendStatus(i)){
				p_status = WifiFriendMatchStatusGet( wk, i );

				status = _WifiMyStatusGet( wk, p_status );
	//			if( (status != WIFI_STATUS_UNKNOWN) && (p_status->trainer_view != HERO) ){
				if( (status != WIFI_STATUS_NONE) && 
					(status != WIFI_STATUS_PLAY_OTHER) &&
					(p_status->trainer_view != HERO) ){
					wk->index2No[num] = i+1;
					num++;
				}else{

					if( (status == WIFI_STATUS_PLAY_OTHER) ){
						// �������̐l�̃{�^�������ɏo�Ă��������
						// BTS:�ʐM393�̑Ώ�
						if( MCVSys_BttnTypeGet( wk, i+1 ) != MCV_BTTN_FRIEND_TYPE_NONE ){
							MCVSys_BttnDel( wk, i+1 );
						}
					}
				}
			}
		}
    }
    return num;
}

//------------------------------------------------------------------
/**
 * $brief   �t�����h��status�����@������̏�Ԃƕς������ς��������Ԃ�
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _checkUserDataMatchStatus(WIFIP2PMATCH_WORK* wk)
{
    int i,num = 0;
	BOOL back_up_wait;
	BOOL now_wait;
	MCR_MOVEOBJ* p_obj;
	_WIFI_MACH_STATUS* p_status;
	u32 status;

    for(i = 0;i < WIFIP2PMATCH_MEMBER_MAX;i++){

		p_status = WifiFriendMatchStatusGet( wk, i );
		status = _WifiMyStatusGet( wk, p_status );
        if((wk->matchStatusBackup[i]  != status) ||
			(wk->matchVchatBackup[i]  != p_status->vchat) ){

			// �I�u�W�F�N�g���[�N
			p_obj = MCRSYS_GetMoveObjWork( wk, i+1 );

			// ���̃I�u�W�F���o�^����A�V������Ԃ��`��I�u�W�F�ɔ��f�����܂ł�
			// �o�b�N�A�b�v�X�e�[�^�X���X�V���Ȃ�
			if( p_obj ){

				// �X�e�[�^�X���҂���ԂɂȂ�����W�����v������
				// �҂���Ԃ��畁�ʂɖ߂�����ʏ�ɖ߂�
				back_up_wait = _modeWait( wk->matchStatusBackup[i] );
				now_wait = _modeWait( status );
				if( (back_up_wait == TRUE) && (now_wait == FALSE) ){
					//NPC�̒ʏ퓮��ɖ߂�
					WIFI_MCR_NpcMoveSet( &wk->matchroom, p_obj,  MCR_NPC_MOVE_NORMAL );
				}else if( (back_up_wait == FALSE) && (now_wait == TRUE) ){
					
					//NPC���W�����v����ɂ���
					WIFI_MCR_NpcMoveSet( &wk->matchroom, p_obj,  MCR_NPC_MOVE_JUMP );
				}

				wk->matchStatusBackup[i] = status;
				wk->matchVchatBackup[i] = p_status->vchat;
				// �ʐM�擾��������
				OHNO_PRINT("�ʐM�擾��������  %d %d\n", i, p_status->trainer_view);
				WifiList_SetFriendInfo(wk->pList, i,
									   WIFILIST_FRIEND_UNION_GRA,
									   p_status->trainer_view);
				WifiList_SetFriendInfo(wk->pList, i,
									   WIFILIST_FRIEND_SEX,
									   p_status->sex);
				num++;
			}
/*            { �o�g�����Ɉꊇ�ŏ������Ă���̂ŁA��������͍폜
                WIFI_HISTORY* pHistry = SaveData_GetWifiHistory(wk->pSaveData);
                int nation = wk->pMatch->friendMatchStatus[i].nation;
                int area = wk->pMatch->friendMatchStatus[i].area;
                WIFIHIST_STAT stat = WIFIHISTORY_GetStat(pHistry, nation, area);
                if((WIFIHIST_STAT_NODATA == stat) || (WIFIHIST_STAT_NEW == stat)){
                    WIFIHISTORY_SetStat(pHistry, nation, area, stat);
                }
            } */
        }
    }
    return num;
}

//------------------------------------------------------------------
/**
 * $brief   �G���[�\��
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static void _errorDisp(WIFIP2PMATCH_WORK* wk)
{
    COMMSTATE_DWCERROR* pErr = CommStateGetWifiError();
    int msgno,no = pErr->errorCode;
    int type =  mydwc_errorType(pErr->errorCode, pErr->errorType);

    if((type == 11) || (no == ERRORCODE_0)){
        msgno = dwc_error_0015;
        type = 11;
    }
    else if(no == ERRORCODE_HEAP){
        msgno = dwc_error_0014;
        type = 12;
    }
    else{
		if( type >= 0 ){
	        msgno = dwc_error_0001 + type;
		}else{
	        msgno = dwc_error_0012;
		}
    }
    OHNO_PRINT("�G���[���b�Z�[�W %d \n",msgno);
    EndMessageWindowOff(wk);
    WORDSET_RegisterNumber(wk->WordSet, 0, no,
                           5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);

	// �G���[���b�Z�[�W��\�����Ă����V���N��
    wk->timer = 30;

	// ���ʉ��̎�����񕶎�����N���A����
    _systemMessagePrint(wk, msgno);

    switch(type){
      case 1:
      case 4:
      case 5:
      case 11:
        wk->seq = WIFIP2PMATCH_RETRY_INIT;  // �Đڑ����t�B�[���h��
        break;
      case 6:
      case 7:
      case 8:
      case 9:
        wk->seq = WIFIP2PMATCH_RETRY_INIT;//WIFIP2PMATCH_POWEROFF_INIT;  // �d����؂邩�t�B�[���h
        break;
      case 10:
        wk->seq = WIFIP2PMATCH_RETRY_INIT;  // ���j���[�ꗗ��
        break;
      case 0:
      case 2:
      case 3:
      default:
        wk->seq = WIFIP2PMATCH_MODE_CHECK_AND_END;  // �t�B�[���h
        break;
    }
}

//------------------------------------------------------------------
/**
 * $brief   �d����؂���WIFI�ݒ��  WIFIP2PMATCH_POWEROFF_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _poweroffInit( WIFIP2PMATCH_WORK *wk, int seq )
{
// WIFI�ݒ�ɍs���Ȃ��悤��

    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        if(GF_BGL_BmpWinAddCheck(&wk->SysMsgWin)){
            BmpTalkWinClear(&wk->SysMsgWin, WINDOW_TRANS_ON );
            GF_BGL_BmpWinDel( &wk->SysMsgWin );
        }
//        WifiP2PMatchMessagePrint(wk, msg_wifilobby_051, FALSE);
        wk->seq = WIFIP2PMATCH_POWEROFF_YESNO;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �d����؂���WIFI�ݒ��  WIFIP2PMATCH_POWEROFF_YESNO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _poweroffYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
// WIFI�ݒ�ɍs���Ȃ��悤��
/*    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_POWEROFF_WAIT;
    }*/
    wk->seq = WIFIP2PMATCH_POWEROFF_WAIT;
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   �d����؂���WIFI�ݒ��  WIFIP2PMATCH_POWEROFF_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _poweroffWait( WIFIP2PMATCH_WORK *wk, int seq )
{
// WIFI�ݒ�ɍs���Ȃ��悤��
    /*
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }else if(ret == 0){ // �͂���I�������ꍇ
        CommStateWifiLogout();
        wk->endSeq = WIFI_P2PMATCH_UTIL;   //�\�t�g�E�G�A���Z�b�g
        wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        EndMessageWindowOff(wk);
    }
    else{  // ��������I�������ꍇ
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_052, FALSE);
        wk->seq = WIFIP2PMATCH_RETRY_YESNO;
    }
*/
    wk->seq = WIFIP2PMATCH_RETRY_YESNO;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ڑ������g���C����  WIFIP2PMATCH_RETRY_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _retryInit( WIFIP2PMATCH_WORK *wk, int seq )
{
	if( wk->timer > 0 ){
		wk->timer --;
		return seq;
	}
	
    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        if(GF_BGL_BmpWinAddCheck(&wk->SysMsgWin)){
            BmpTalkWinClear(&wk->SysMsgWin, WINDOW_TRANS_ON );
            GF_BGL_BmpWinDel( &wk->SysMsgWin );
        }
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_052, FALSE);
        wk->seq = WIFIP2PMATCH_RETRY_YESNO;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ڑ������g���C����  WIFIP2PMATCH_RETRY_YESNO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _retryYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_RETRY_WAIT;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ڑ������g���C����  WIFIP2PMATCH_RETRY_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _retryWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }else if(ret == 0){ // �͂���I�������ꍇ
        CommStateWifiLogout();
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
        wk->seq = WIFIP2PMATCH_RETRY;
    }
    else{  // ��������I�������ꍇ
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);	// VCHAT�̏�Ԃ����ɖ߂�����
        CommStateWifiLogout();
        CommInfoFinalize();   //Info������
        wk->endSeq = WIFI_P2PMATCH_END;
        wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
    }
    EndMessageWindowOff(wk);
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ڑ������g���C  WIFIP2PMATCH_RETRAY
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _retry( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
	ARCHANDLE* p_handle;
    
    if(!CommIsVRAMDInitialize()){
		
		if( WIFI_MCR_GetInitFlag( &wk->matchroom ) == TRUE ){

			// �u���b�N�A�E�g
			WIPE_SetBrightness( WIPE_DISP_MAIN, WIPE_FADE_BLACK );
			WIPE_SetBrightness( WIPE_DISP_SUB, WIPE_FADE_BLACK );

			WIFI_MCR_Dest( &wk->matchroom );
			for( i=0; i<MATCHROOM_IN_OBJNUM; i++ ){
				wk->p_matchMoveObj[i] = NULL;
			}

			// OAM�o�b�t�@�N���[��
			REND_OAM_UtilOamRamClear_Main( HEAPID_WIFIP2PMATCH );
			REND_OAM_UtilOamRamClear_Sub( HEAPID_WIFIP2PMATCH );
		}
		// �r���[�A�[�j��
		if( MCVSys_MoveCheck( wk ) == TRUE ){
			MCVSys_Exit( wk );

		}
		
		// �r�b�g�}�b�vOFF
		// ���[�U�[�f�[�^
		if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWinBack)){
			BmpMenuWinClear( &wk->MyInfoWinBack, WINDOW_TRANS_ON );
			GF_BGL_BmpWinDel(&wk->MyInfoWinBack);
		}
		if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWin)){
			GF_BGL_BmpWinDel(&wk->MyInfoWin);
		}
		
        //�ڑ��J�n
        GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME3_M);

		// �O���t�B�b�N���Z�b�g
		p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, HEAPID_WIFIP2PMATCH );
		BgGraphicSet( wk, p_handle );
		ArchiveDataHandleClose( p_handle );


		// �p���b�g�t�F�[�h�ĊJ
		ConnectBGPalAnm_OccSet(&wk->cbp, TRUE);

		// �u���C�g�l�X��ԃ��Z���g
		// MCR��ʔj�����ɈÂ�������������
		wk->brightness_reset = TRUE;
//		WIPE_ResetBrightness( WIPE_DISP_MAIN );
//		WIPE_ResetBrightness( WIPE_DISP_SUB );

        if( wk->menulist){
            GF_BGL_BmpWinDel(&wk->ListWin);
            BmpListExit(wk->lw, NULL, NULL);
            wk->lw=NULL;
            BMP_MENULIST_Delete( wk->menulist );
            wk->menulist=NULL;
        }

//        wk->friendMatchReadCount;
        MI_CpuClear8(wk->index2No, WIFIP2PMATCH_MEMBER_MAX);
        MI_CpuClear8(wk->index2NoBackUp, WIFIP2PMATCH_MEMBER_MAX);
        MI_CpuClear8(wk->matchStatusBackup, WIFIP2PMATCH_MEMBER_MAX*sizeof(int));
        MI_CpuClear8(wk->matchVchatBackup, WIFIP2PMATCH_MEMBER_MAX*sizeof(int));

#if 0
		for(i=0;i < _OAM_NUM;i++){
            CLACT_SetDrawFlag( wk->MainActWork[i], 0 );
        }
#endif
        MainMenuMsgInit(wk);
        wk->pMatch = CommStateWifiEnterLogin(wk->pSaveData,sizeof(TEST_MATCH_WORK));
        WifiP2PMatchMessagePrint(wk, dwc_message_0008, TRUE);
		GF_ASSERT( wk->timeWaitWork == NULL );
        wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
        wk->seq = WIFIP2PMATCH_CONNECTING_INIT;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ���O�C���ڑ���  WIFIP2PMATCH_CONNECTING
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int WifiP2PMatch_Connecting( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;

    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }
    if( mydwc_getSaving()) {
        wk->seq = WIFIP2PMATCH_FIRST_SAVING;
        WifiP2PMatchMessagePrint(wk, dwc_message_0015, TRUE);
		GF_ASSERT( wk->timeWaitWork == NULL );
        wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
    }
    
    if(CommStateIsWifiLoginState()){
        OHNO_PRINT("WIFI�ڑ�����\n");
        if( wk->bInitMessage ){  // ����ڑ����ɂ̓Z�[�u�V�[�P���X��
//            SaveData_SaveParts(wk->pSaveData, SVBLK_ID_NORMAL);  //�Z�[�u��
            wk->seq = WIFIP2PMATCH_FIRST_ENDMSG;
        }
        else{
            _makeMyMatchStatus(wk, WIFI_STATUS_LOGIN_WAIT);
            _readFriendMatchStatus( wk );
            wk->seq = WifiP2PMatchFriendListStart();
        }
    }
    else if(CommStateIsWifiError() || (CommWifiIsMatched() == 3)){
        _errorDisp(wk);
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ����Z�[�u����
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int WifiP2PMatch_FirstSaving( WIFIP2PMATCH_WORK *wk, int seq )

{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }
    if( mydwc_getSaving()) {
        ///      SaveData_SaveParts(wk->pSaveData, SVBLK_ID_NORMAL);  //�Z�[�u��
        SaveData_DivSave_Init(wk->pSaveData, SVBLK_ID_NORMAL); ///�Z�[�u�J�n
        wk->seq = WIFIP2PMATCH_FIRST_SAVING2;
        return seq;
    }
    if(CommStateIsWifiLoginState()){
        OHNO_PRINT("WIFI�ڑ�����\n");
        if( wk->bInitMessage ){
            wk->seq = WIFIP2PMATCH_FIRST_ENDMSG;
        }
        else{
            _makeMyMatchStatus(wk, WIFI_STATUS_LOGIN_WAIT);
            _readFriendMatchStatus( wk );
            wk->seq = WifiP2PMatchFriendListStart();
        }
    }
    else if(CommStateIsWifiError() || (CommWifiIsMatched() == 3)){
        _errorDisp(wk);
    }
    return seq;
}



//------------------------------------------------------------------
/**
 * $brief   ����Z�[�u����  WIFIP2PMATCH_FIRST_SAVING2
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int WifiP2PMatch_FirstSaving2( WIFIP2PMATCH_WORK *wk, int seq )
{
    if(mydwc_getSaving()){
        SAVE_RESULT result = SaveData_DivSave_Main(wk->pSaveData);
        if (result != SAVE_RESULT_CONTINUE && result != SAVE_RESULT_LAST) {
            mydwc_resetSaving();
        }
        else{
            return seq;
        }
    }
    if(CommStateIsWifiLoginState()){
        OHNO_PRINT("WIFI�ڑ�����\n");
        if( wk->bInitMessage ){ 
            wk->seq = WIFIP2PMATCH_FIRST_ENDMSG;
        }
        else{
            _makeMyMatchStatus(wk, WIFI_STATUS_LOGIN_WAIT);
            _readFriendMatchStatus( wk );
            wk->seq = WifiP2PMatchFriendListStart();
        }
    }
    else if(CommStateIsWifiError() || (CommWifiIsMatched() == 3)){
        _errorDisp(wk);
    }
    return seq;
}



//------------------------------------------------------------------
/**
 * $brief   �ؒf�{VCT�҂��Ɉڍs����܂ő҂�  WIFIP2PMATCH_RECONECTING_WAIT
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------
static int WifiP2PMatch_ReConnectingWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;

    if(wk->timer > 0){
        wk->timer--;
        return seq;
    }

    OHNO_PRINT("CommStateIsWifiLoginState %d  \n",CommStateIsWifiLoginState());
    OHNO_PRINT("CommWifiIsMatched %d  \n",CommWifiIsMatched());
    OHNO_PRINT("CommStateIsWifiDisconnect %d  \n",CommStateIsWifiDisconnect());
    OHNO_PRINT("CommIsConnect %d  \n",CommIsConnect(COMM_PARENT_ID));

    if(CommStateIsWifiLoginMatchState()){
        OHNO_PRINT("WIFI�ڑ�����\n");
        wk->seq = WifiP2PMatchFriendListStart();
    }
    else if(CommStateIsWifiError() || (CommWifiIsMatched() == 3)){
        _errorDisp(wk);
    }
    else if((CommWifiIsMatched() >= 4) || CommStateIsWifiDisconnect() || !CommIsConnect(COMM_PARENT_ID)){
        CommStateWifiMatchEnd();

        wk->seq = WifiP2PMatchFriendListStart();
    }
    return seq;
}




//------------------------------------------------------------------
/**
 * $brief   �ŏ��ڑ�����  WIFIP2PMATCH_FIRST_ENDMSG
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _firstConnectEndMsg( WIFIP2PMATCH_WORK *wk, int seq )
{
    EndMessageWindowOff(wk);
    _systemMessagePrint(wk, dwc_message_0004);
    wk->seq = WIFIP2PMATCH_FIRST_ENDMSG_WAIT;
#if AFTER_MASTER_070410_WIFIAPP_N22_EUR_FIX
    wk->bInitMessage = FALSE;
#endif //AFTER_MASTER_070410_WIFIAPP_N22_EUR_FIX
    return seq;

}

//WIFIP2PMATCH_FIRST_ENDMSG_WAIT
static int _firstConnectEndMsgWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        if(wk->initSeq == WIFI_P2PMATCH_DPW){ // ���E�ΐ�Ȃ�
            wk->endSeq = WIFI_P2PMATCH_DPW_END;   // �I��������
            wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
            CommStateWifiLogout();
        }
        else if( 0==WifiList_GetFriendDataNum(wk->pList) ){  //�t�����h�����Ȃ�
            WifiP2PMatchMessagePrint(wk, dwc_message_0011, TRUE);
            wk->seq = WIFIP2PMATCH_MODE_EXITING;
            wk->timer = 1;
        }
        else{
            _makeMyMatchStatus(wk, WIFI_STATUS_LOGIN_WAIT);
            _readFriendMatchStatus( wk );
            wk->seq = WifiP2PMatchFriendListStart();
            if(GF_BGL_BmpWinAddCheck(&wk->SysMsgWin)){
                BmpTalkWinClear(&wk->SysMsgWin, WINDOW_TRANS_ON );
                GF_BGL_BmpWinDel( &wk->SysMsgWin );
            }
        }
    }
    return seq;

}

//------------------------------------------------------------------
/**
 * $brief   �����̏�Ԃ�\��
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void	_userDataInfoDisp(WIFIP2PMATCH_WORK * wk)
{
    int col,sex, vct_icon;
	int msg_id;
    MYSTATUS* pMy = SaveData_GetMyStatus(wk->pSaveData);
    STRBUF* pSTRBUF = MyStatus_CreateNameString(pMy, HEAPID_WIFIP2PMATCH);
	u32 status;

    if(!GF_BGL_BmpWinAddCheck(&wk->MyInfoWin)){
        STRBUF_Delete(pSTRBUF);
        return;
    }

	// ������
    GF_BGL_BmpWinDataFill( &wk->MyInfoWin, 0 );

	sex = MyStatus_GetMySex(pMy);

	if( sex == PM_MALE ){
		col = _COL_N_BLUE;
	}
	else{
		col = _COL_N_RED;
	}
    GF_STR_PrintColor( &wk->MyInfoWin, FONT_SYSTEM,
                       pSTRBUF, WIFIP2PMATCH_PLAYER_DISP_NAME_X, 0,
                       MSG_NO_PUT, col, NULL);
    STRBUF_Delete(pSTRBUF);

	status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );

	// msg_id�ƃJ���[���擾
	msg_id = MCVSys_StatusMsgIdGet( status, &col );
    if( status == WIFI_STATUS_VCT ){      // VCT��
        if(mydwc_getFriendIndex() == -1){
            MSGMAN_GetString(wk->MsgManager, msg_id, wk->pExpStrBuf);
        }
        else{
            sex = WifiList_GetFriendInfo(wk->pList, mydwc_getFriendIndex(), WIFILIST_FRIEND_SEX);
            if( PM_MALE == sex){
                col = _COL_N_BLUE;
            }
            else if( PM_FEMALE == sex){
                col = _COL_N_RED;
            }
            STRBUF_SetStringCode(wk->pExpStrBuf ,WifiList_GetFriendNamePtr(wk->pList,mydwc_getFriendIndex()));
        }
	}else{
		MSGMAN_GetString(wk->MsgManager, msg_id, wk->pExpStrBuf);
	}
	GF_STR_PrintColor( &wk->MyInfoWin, FONT_SYSTEM, wk->pExpStrBuf,
					   WIFIP2PMATCH_PLAYER_DISP_ST_X, 0,
					   MSG_NO_PUT, col, NULL);

	//��ԃA�C�R���ݒ�
	// Frame1�̑Ή�����ʒu�ɃA�C�R����]��
	WifiP2PMatchFriendListStIconWrite( wk->bgl, &wk->icon, GF_BGL_FRAME1_M, 
			PLAYER_DISP_ICON_POS_X, PLAYER_DISP_ICON_POS_Y,
			status );
	if( wk->pMatch->myMatchStatus.vchat ){
		vct_icon = PLAYER_DISP_ICON_IDX_NONE;
	}else{
		vct_icon = PLAYER_DISP_ICON_IDX_VCTNOT;
	}

	WifiP2PMatchFriendListIconWrite( wk->bgl, &wk->icon, GF_BGL_FRAME1_M, 
			PLAYER_DISP_VCTICON_POS_X, PLAYER_DISP_VCTICON_POS_Y,
			vct_icon, 0 );
	
    GF_BGL_BmpWinOnVReq(&wk->MyInfoWin);
}

//------------------------------------------------------------------
/**
 * $brief   �ꃉ�C���R�[���o�b�N
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void	_lineCallBack(BMPLIST_WORK * lw, u32 param, u8 y)
{
    WIFIP2PMATCH_WORK* wk = (WIFIP2PMATCH_WORK*)BmpListParamGet(lw,BMPLIST_ID_WORK);
    int gmmNo = 0,col = _COL_N_BLACK;
	_WIFI_MACH_STATUS* p_status;
	u32 status;

    if((param == WIFIP2PMATCH_MEMBER_MAX) || (param == BMPLIST_CANCEL)){
        return;
    }

	p_status = WifiFriendMatchStatusGet( wk, param );
	status = _WifiMyStatusGet( wk,  p_status );
    switch(status){
      case WIFI_STATUS_VCT:      // VCT��
        gmmNo = msg_wifilobby_027;
        col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_SBATTLE50:      // �ΐ풆
      case WIFI_STATUS_SBATTLE100:      // �ΐ풆
      case WIFI_STATUS_SBATTLE_FREE:      // �ΐ풆
      case WIFI_STATUS_DBATTLE50:      // �ΐ풆
      case WIFI_STATUS_DBATTLE100:      // �ΐ풆
      case WIFI_STATUS_DBATTLE_FREE:      // �ΐ풆
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE:      // �ΐ풆
#endif
        gmmNo = msg_wifilobby_024;
        col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_SBATTLE50_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_060;
        break;
      case WIFI_STATUS_SBATTLE100_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_061;
        break;
      case WIFI_STATUS_SBATTLE_FREE_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_059;
        break;
      case WIFI_STATUS_DBATTLE50_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_063;
        break;
      case WIFI_STATUS_DBATTLE100_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_064;
        break;
      case WIFI_STATUS_DBATTLE_FREE_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_062;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE_WAIT:   // �ΐ��W��
        gmmNo = msg_wifilobby_062;
        break;
#endif
      case WIFI_STATUS_TRADE:          // ������
        gmmNo = msg_wifilobby_026;
        col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_TRADE_WAIT:    // ������W��
        gmmNo = msg_wifilobby_025;
        break;
      case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
        gmmNo = msg_wifilobby_100;
        break;
      case WIFI_STATUS_FRONTIER_WAIT:    // �t�����e�B�A��W��
        gmmNo = msg_wifilobby_138;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM_WAIT:    // �o�g�����[����W��
        gmmNo = msg_wifilobby_debug_00;
        break;
#endif
      case WIFI_STATUS_BUCKET_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALANCEBALL_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALLOON_WAIT:    // �o�P�b�g��W��
        gmmNo = msg_wifilobby_147;
        break;
      case WIFI_STATUS_LOGIN_WAIT:    // �ҋ@���@���O�C������͂���
        gmmNo = msg_wifilobby_046;
        break;
      case WIFI_STATUS_UNKNOWN:
      default:
        gmmNo = msg_wifilobby_056;
        break;
    }
    OHNO_PRINT("�����R�[���o�b�N�Ăт��� %d %d %d\n",param,y,gmmNo);

    if(gmmNo != 0){
        MSGMAN_GetString(wk->MsgManager, gmmNo, wk->pExpStrBuf);

        {
            int length = FontProc_GetPrintStrWidth( FONT_SYSTEM, wk->pExpStrBuf, 0 );
            int x      = 164 - length;

            GF_STR_PrintColor( &wk->ListWin, FONT_SYSTEM, wk->pExpStrBuf,
                               x, y, MSG_NO_PUT, col, NULL);
            GF_BGL_BmpWinOnVReq(&wk->ListWin);
        }
    }

    {
        int sex = WifiList_GetFriendInfo(wk->pList, param, WIFILIST_FRIEND_SEX);
        col = _COL_N_BLACK;
        if( PM_MALE == sex){
            col = _COL_N_BLUE;
        }
        else if( PM_FEMALE == sex){
            col = _COL_N_RED;
        }

//        GF_STR_PrintColor( &wk->ListWin, FONT_SYSTEM, wk->TrainerName[param],
  //                         8, y, MSG_NO_PUT, col, NULL);
        GF_BGL_BmpWinOnVReq(&wk->ListWin);
    }
}

//------------------------------------------------------------------
/**
 * $brief   �A�C�R���\��
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void	_iconDisp(WIFIP2PMATCH_WORK * wk, int status, int vctIcon, int x,int y,int x2)
{
    u16 btl[]={ 0x17,0x18,0xc18,0x20 };  //�΂Ƃ�
    u16 chg[]={ 0x25,0x26,0x2a,0xc25 };  //��������
    u16 wai[]={ 0x2f,0x42f,0x82f,0xc2f };  //������
    u16 vct[]={ 0x4a,0x4b,0x084a,0x084b };  //VCT
    u16 vctoff[]={ 0x48,0x49,0x0848,0x0849 };  //VCToff
    u16 hate[]={ 0x44,0x45,0x46,0x47 };  //?
    u16 none[]={ 0x6,0x6,0x6,0x6 };  //none
    u16* pData;
    // 0x06-----------�Ȃ�
    u16* pScrAddr = GF_BGL_ScreenAdrsGet( wk->bgl, GF_BGL_FRAME0_M );
    int pos,col = 0;

    switch(status){
      case WIFI_STATUS_VCT:      // VCT��
        pData = vct;
        break;
      case WIFI_STATUS_SBATTLE50_WAIT:   // �ΐ��W��
      case WIFI_STATUS_SBATTLE100_WAIT:   // �ΐ��W��
      case WIFI_STATUS_SBATTLE_FREE_WAIT:   // �ΐ��W��
      case WIFI_STATUS_DBATTLE50_WAIT:   // �ΐ��W��
      case WIFI_STATUS_DBATTLE100_WAIT:   // �ΐ��W��
      case WIFI_STATUS_DBATTLE_FREE_WAIT:   // �ΐ��W��
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE_WAIT:   // �ΐ��W��
#endif
        col = 0x1000;
      case WIFI_STATUS_SBATTLE50:      // �ΐ풆
      case WIFI_STATUS_SBATTLE100:      // �ΐ풆
      case WIFI_STATUS_SBATTLE_FREE:      // �ΐ풆
      case WIFI_STATUS_DBATTLE50:      // �ΐ풆
      case WIFI_STATUS_DBATTLE100:      // �ΐ풆
      case WIFI_STATUS_DBATTLE_FREE:      // �ΐ풆
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE:      // �ΐ풆
#endif
        pData = btl;
        break;
      case WIFI_STATUS_TRADE_WAIT:    // ������W��
      case WIFI_STATUS_POFIN_WAIT:    // ������W��
      case WIFI_STATUS_FRONTIER_WAIT:    // �t�����e�B�A��W��
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM_WAIT:// ������W��
#endif
      case WIFI_STATUS_BUCKET_WAIT:// ������W��
      case WIFI_STATUS_BALANCEBALL_WAIT:// ������W��
      case WIFI_STATUS_BALLOON_WAIT:// ������W��
        col = 0x1000;
      case WIFI_STATUS_TRADE:          // ������
      case WIFI_STATUS_POFIN:          // ������
      case WIFI_STATUS_FRONTIER:          // �t�����e�B�A��
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM:     // ������
#endif
      case WIFI_STATUS_BUCKET:     // ������
      case WIFI_STATUS_BALANCEBALL:     // ������
      case WIFI_STATUS_BALLOON:     // ������
        pData = chg;
        break;
      case WIFI_STATUS_LOGIN_WAIT:    // �ҋ@���@���O�C������͂���
        pData = wai;
        break;
      case WIFI_STATUS_UNKNOWN:
        pData = hate;
        break;
      default:
        pData = none;
        break;
    }

    pos = x + y * 32;
    pScrAddr[pos] = pData[0]+col;
    pScrAddr[pos+1] = pData[1]+col;
    pScrAddr[pos+32] = pData[2]+col;
    pScrAddr[pos+33] = pData[3]+col;

    if(vctIcon){
        pData = none;
    }
    else{
        pData = vctoff;
    }
    pos = x2 + y * 32;
    pScrAddr[pos] = pData[0];
    pScrAddr[pos+1] = pData[1];
    pScrAddr[pos+32] = pData[2];
    pScrAddr[pos+33] = pData[3];
}

//------------------------------------------------------------------
/**
 * $brief   ���\�����Ƃ̃R�[���o�b�N�֐�
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void	_iconMenuDisp(WIFIP2PMATCH_WORK * wk, int toppos)
{
    u16* pData;
    // 0x06-----------�Ȃ�
    u16* pScrAddr = GF_BGL_ScreenAdrsGet( wk->bgl, GF_BGL_FRAME0_M );
    int pos,status,i,vchat;
	_WIFI_MACH_STATUS* p_status;


    for(i = 0; i < WIFIP2PMATCH_DISP_MEMBER_MAX;i++){
        pos = toppos + i;
        if(pos >= WIFIP2PMATCH_MEMBER_MAX){
            status = WIFI_STATUS_NONE;
            vchat = 1;
        }
        else if(wk->index2No[pos]==0){
            status = WIFI_STATUS_NONE;
            vchat = 1;
        }
        else{
			p_status = WifiFriendMatchStatusGet( wk, wk->index2No[pos] - 1 );
			status = _WifiMyStatusGet( wk, p_status );
            vchat = p_status->vchat;
        }
        pos = 3 + ((5+(i*3)) * 32);
        _iconDisp(wk, status, vchat, 3, 5+(i*3), 27);
    }
	status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );
    vchat = wk->pMatch->myMatchStatus.vchat;
    if(status == WIFI_STATUS_VCT){
        status = WIFI_STATUS_NONE;
    }
    _iconDisp(wk, status, vchat, 3, 21, 27);
    GF_BGL_LoadScreenReq(wk->bgl, GF_BGL_FRAME0_M);  // �X�N���[���ɍX�V
}

//------------------------------------------------------------------
/**
 * $brief   �J�[�\���R�[���o�b�N
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void _curCallBack(BMPLIST_WORK * wk,u32 param,u8 mode)
{
    if(mode == 0){
        Snd_SePlay(_SE_CURSOR);
    }
}


///�I�����j���[�̃��X�g
static const BMPLIST_HEADER _friendInfoMenuListHeader = {
    NULL,			// �\�������f�[�^�|�C���^
    _curCallBack,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
    _lineCallBack,					// ���\�����Ƃ̃R�[���o�b�N�֐�
    NULL,                   // GF_BGL_BMPWIN
    (WIFIP2PMATCH_MEMBER_MAX+2),	// ���X�g���ڐ�
    WIFIP2PMATCH_DISP_MEMBER_MAX,	// �\���ő區�ڐ�
    0,						// ���x���\���w���W
    8,						// ���ڕ\���w���W
    0,						// �J�[�\���\���w���W
    0,						// �\���x���W
    FBMP_COL_BLACK,			// �����F
    0x8000,			// �w�i�F �ʂ��H
    FBMP_COL_BLK_SDW,		// �����e�F
    0,						// �����Ԋu�w
    24,						// �����Ԋu�x
    BMPLIST_LRKEY_SKIP,		// �y�[�W�X�L�b�v�^�C�v
    FONT_SYSTEM,				// �����w��
    1,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
    NULL                    // work
    };

static int _addListString(WIFIP2PMATCH_WORK* wk)
{
    int i,num=0;

    for(i = 0; i < WIFIP2PMATCH_MEMBER_MAX; i++){
        if( wk->index2No[i] != 0){
            BMP_MENULIST_AddString( wk->menulist, wk->pTemp, wk->index2No[i] - 1 );
            //            BMP_MENULIST_AddString( wk->menulist, wk->TrainerName[wk->index2No[i] - 1], wk->index2No[i] - 1 );
            num++;
        }
    }
    BMP_MENULIST_AddArchiveString( wk->menulist, wk->MsgManager,
                                   msg_wifilobby_028, WIFIP2PMATCH_MEMBER_MAX );
    BMP_MENULIST_AddArchiveString( wk->menulist, wk->MsgManager,
                                   msg_wifilobby_047, BMPLIST_CANCEL );
    return num;
}

static void _menuFriendList(WIFIP2PMATCH_WORK* wk)
{
    int i,num = 0;
    BMPLIST_HEADER list_h;

    wk->menulist = BMP_MENULIST_Create( WIFIP2PMATCH_MEMBER_MAX+2, HEAPID_WIFIP2PMATCH );


    num = _addListString(wk);

    if(GF_BGL_BmpWinAddCheck(&wk->ListWin)){
        BmpTalkWinClear( &wk->ListWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->ListWin);
    }
    //BMP�E�B���h�E����
    GF_BGL_BmpWinAdd(wk->bgl,&wk->ListWin,
                     GF_BGL_FRAME3_M, 6, 5, 21, 14, FLD_SYSFONT_PAL, 1);


    list_h = _friendInfoMenuListHeader;
    list_h.list = wk->menulist;
    list_h.win = &wk->ListWin;
    list_h.count = num + 2;
    list_h.work = wk;
    wk->lw = BmpListSet(&list_h, 0, 0, HEAPID_WIFIP2PMATCH);


}

//------------------------------------------------------------------
/**
 * $brief   �t�����h���X�g�\��������   WIFIP2PMATCH_FRIENDLIST_INIT
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------

static void _userDataDisp(WIFIP2PMATCH_WORK* wk)
{
    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWin)){
        GF_BGL_BmpWinDel(&wk->MyInfoWin);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWinBack)){
        BmpMenuWinClear( &wk->MyInfoWinBack, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->MyInfoWinBack);
    }

    GF_BGL_BmpWinAdd(wk->bgl,&wk->MyInfoWin,
                     GF_BGL_FRAME3_M, 
					 WIFIP2PMATCH_PLAYER_DISP_X, WIFIP2PMATCH_PLAYER_DISP_Y,
					 WIFIP2PMATCH_PLAYER_DISP_SIZX, WIFIP2PMATCH_PLAYER_DISP_SIZY,
                     FLD_SYSFONT_PAL, _CGX_USER_BOTTOM );


    GF_BGL_BmpWinAdd(wk->bgl,&wk->MyInfoWinBack,
                     GF_BGL_FRAME1_M, 
					 WIFIP2PMATCH_PLAYER_DISP_X, WIFIP2PMATCH_PLAYER_DISP_Y,
					 WIFIP2PMATCH_PLAYER_DISP_SIZX, WIFIP2PMATCH_PLAYER_DISP_SIZY,
                     FLD_SYSFONT_PAL, _CGX_USET_BACK_BOTTOM );

	// ������
    GF_BGL_BmpWinDataFill( &wk->MyInfoWinBack, 15 );
    GF_BGL_BmpWinOnVReq(&wk->MyInfoWinBack);
	

	// �V�X�e���E�B���h�E�ݒ�
	BmpMenuWinWrite( &wk->MyInfoWinBack, WINDOW_TRANS_ON, MENU_WIN_CGX_NUM, MENU_WIN_PAL );

}

// �_�~�[�I�u�W�F�N�g��o�^���܂���
static void DEBUG_DummyObjIn( WIFIP2PMATCH_WORK *wk, int num )
{
	int i;
	MCR_MOVEOBJ* p_moveobj;
	static const u8 view_list[ 32 ][2] = {
		{ BOY1, PM_MALE },
		{ BOY3, PM_MALE }, 
		{ MAN3, PM_MALE }, 
		{ BADMAN, PM_MALE }, 
		{ EXPLORE, PM_MALE }, 
		{ FIGHTER, PM_MALE },
		{ GORGGEOUSM, PM_MALE }, 
		{ MYSTERY, PM_MALE },   
		{ GIRL1, PM_FEMALE }, 
		{ GIRL2, PM_FEMALE }, 
		{ WOMAN2, PM_FEMALE }, 
		{ WOMAN3, PM_FEMALE }, 
		{ IDOL, PM_FEMALE }, 
		{ LADY, PM_FEMALE }, 
		{ COWGIRL, PM_FEMALE }, 
		{ GORGGEOUSW, PM_FEMALE },
		{ BOY1, PM_MALE },
		{ BOY3, PM_MALE }, 
		{ MAN3, PM_MALE }, 
		{ BADMAN, PM_MALE }, 
		{ EXPLORE, PM_MALE }, 
		{ FIGHTER, PM_MALE },
		{ GORGGEOUSM, PM_MALE }, 
		{ MYSTERY, PM_MALE },   
		{ GIRL1, PM_FEMALE }, 
		{ GIRL2, PM_FEMALE }, 
		{ WOMAN2, PM_FEMALE }, 
		{ WOMAN3, PM_FEMALE }, 
		{ IDOL, PM_FEMALE }, 
		{ LADY, PM_FEMALE }, 
		{ COWGIRL, PM_FEMALE }, 
		{ GORGGEOUSW, PM_FEMALE },
	};

	for( i=0; i<num; i++ ){
		// �������o��
		p_moveobj = WIFI_MCR_SetNpc( &wk->matchroom, view_list[i][0], i+1 );

		// �݂�Ȕ�ђ��˂�
		WIFI_MCR_NpcMoveSet( &wk->matchroom, p_moveobj, MCR_NPC_MOVE_JUMP );
		MCRSYS_SetMoveObjWork( wk, p_moveobj );	// �o�^�������[�N��ۑ����Ă���

	}
}

//------------------------------------------------------------------
/**
 * $brief   �t�����h���X�g�\��������   WIFIP2PMATCH_FRIENDLIST_INIT
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_FriendListInit( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i,x;
    int num = 0;
	ARCHANDLE* p_handle;
	MCR_MOVEOBJ* p_moveobj;
	int obj_code;

	// ���C�v�I���҂�
	if( WIPE_SYS_EndCheck() == FALSE ){
		return seq;
	}

	// �G���[�`�F�b�N
    if( CommStateIsWifiError() == FALSE ){
	
		// �ؒf�����VCHAT��W��Ԃɕς�����̂����`�F�b�N
		if(!CommStateIsWifiLoginMatchState()){
			return seq;
		}
	}

	// �R�[���o�b�N��ݒ�
	mydwc_setConnectModeCheckCallback( WIFIP2PModeCheck );

	// ���������͒ʐM�G���[���V�X�e���E�B���h�E�ŏo��
//	CommStateSetErrorCheck(FALSE,FALSE);	//���̊֐��̉��ɂ���̂ō폜
	
	ConnectBGPalAnm_OccSet(&wk->cbp, FALSE);

	p_handle = ArchiveDataHandleOpen( ARC_WIFIP2PMATCH_GRA, HEAPID_WIFIP2PMATCH );


	// �A�C�R���f�[�^�Ǎ���
	WifiP2PMatchFriendListIconLoad( wk->bgl, &wk->icon, p_handle, HEAPID_WIFIP2PMATCH );

    _myVChatStatusOrgSet(wk);
	
//    _timeWaitIconDel(wk);		timeWait����MsgWin��j�����Ă���Ƃ������Ƃ̓��b�Z�[�W�I���ł�OK��
	EndMessageWindowOff(wk);
	
    GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME3_M);

#if 0
    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWinBack)){
        BmpMenuWinClear(&wk->MyInfoWinBack, WINDOW_TRANS_ON);
        GF_BGL_BmpWinDel(&wk->MyInfoWinBack);
    }
#endif
    if(wk->menulist){
        GF_BGL_BmpWinDel(&wk->ListWin);
        BmpListExit(wk->lw, NULL, NULL);
        wk->lw = NULL;
        BMP_MENULIST_Delete( wk->menulist );
        wk->menulist = NULL;
    }
	if( WIFI_MCR_GetInitFlag( &wk->matchroom ) == FALSE ){

		if( wk->pMatch->myMatchStatus.sex == PM_MALE ){
			obj_code = HERO;
		}else{
			obj_code = HEROINE;
		}
		
		// �}�b�`���O���[��������
		wk->friend_num = WifiList_GetFriendDataLastIdx( wk->pList );
#ifdef WFP2PM_MANY_OBJ
		wk->friend_num = 32;
#endif
		WIFI_MCR_Init( &wk->matchroom, HEAPID_WIFIP2PMATCH, p_handle, wk->bgl, obj_code, wk->friend_num );

		// �������o��
		p_moveobj = WIFI_MCR_SetPlayer( &wk->matchroom, obj_code );
		MCRSYS_SetMoveObjWork( wk, p_moveobj );	// �o�^�������[�N��ۑ����Ă���
#ifdef WFP2PM_MANY_OBJ
	DEBUG_DummyObjIn( wk, wk->friend_num );
#endif
	}
	// �r���[�A�[������
	if( MCVSys_MoveCheck( wk ) == FALSE ){
		MCVSys_Init( wk, p_handle, HEAPID_WIFIP2PMATCH );
	}

	// ���̏�Ԃ���������
	MCVSys_ReWrite( wk, HEAPID_WIFIP2PMATCH );

#if 0
    GF_BGL_BmpWinAdd(wk->bgl, &wk->MyInfoWinBack, GF_BGL_FRAME3_M, 4, 1, 21, 2,
                     _NUKI_FONT_PALNO,  _CGX_TITLE_BOTTOM );
    GF_BGL_BmpWinDataFill( &wk->MyInfoWinBack, 0x8000 );
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_019, wk->TitleString );
    GF_STR_PrintColor( &wk->MyInfoWinBack, FONT_TALK, wk->TitleString, 0, 0, MSG_NO_PUT, _COL_N_WHITE, NULL);
    GF_BGL_BmpWinOnVReq(&wk->MyInfoWinBack);
#endif

	// ���[�U�[�f�[�^�\��
    EndMessageWindowOff(wk);
    _readFriendMatchStatus(wk);

	// ���[�U�[�f�[�^��BMP�쐬
	_userDataDisp(wk);

// WIFI�@�ΐ�AUTOӰ�ރf�o�b�N
#ifndef _WIFI_DEBUG_TUUSHIN
    _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
#else	//_WIFI_DEBUG_TUUSHIN

	if( WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_BATTLE == FALSE ){
		
		if( WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_MODE == _WIFI_DEBUG_MODE_X ){
			_myStatusChange(wk, WIFI_STATUS_SBATTLE50_WAIT);
			WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_A_REQ = FALSE;
			WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_B_REQ = FALSE;
		}else{
			_myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
			WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_A_REQ = TRUE;
			WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_B_REQ = FALSE;
		}

		WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_BATTLE = TRUE;
	}else{
		WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_BATTLE = FALSE;
		_myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
	}
#endif	//_WIFI_DEBUG_TUUSHIN

	// �ʐM��Ԃ����ɖ߂�
	CommStateChangeWiFiLogin();

    wk->preConnect = -1;

	// 080605 tomoya BTS:249�̑Ώ��̂���
	// �e�̒ʐM���Z�b�g�����㒼���ɐڑ����Ă��܂��̂����
	// �i�ڑ�����Ă�VCHAT�̏�Ԃ̂܂܁AFriendList�V�[�P���X�Ɉڍs������j
//	mydwc_ResetNewPlayer();	// ���Ȃ݂�mydwc_returnLobby���ł����������Ă�
#if 0
    CLACT_SetDrawFlag( wk->MainActWork[_CLACT_LINE_CUR], 1 );
#endif

	ArchiveDataHandleClose( p_handle );

	// ���C�v�C��
	WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, WIPE_FADE_BLACK,
					COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);
    wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
    CommStateSetErrorCheck(FALSE,FALSE);   ///335
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �N�����ڑ����Ă����ꍇ
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _checkParentConnect(WIFIP2PMATCH_WORK *wk)
{
    if( CommWifiIsMatched()==1 ){
        return TRUE;
    }
    return FALSE;
}

static BOOL _checkParentNewPlayer( WIFIP2PMATCH_WORK *wk )
{
	if( (mydwc_IsNewPlayer() != -1) ){
		// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
		// ������VCHAT��ON�ɂ��Ȃ���
		// DWC_RAP����VCHAT���J�n���Ȃ�
		mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);
		OS_TPrintf( "Connect VCHAT set\n" );
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------
/**
 * $brief   ���C�����X�g�̏�������
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static void _mainListReWrite(WIFIP2PMATCH_WORK *wk)
{
    int num,num2,checkMatch;
    u16 list,cursor;

    num = _readFriendMatchStatus(wk);
    num2 = _checkUserDataMatchStatus(wk);
    if((wk->friendMatchReadCount != num) || (num2 != 0)){
        OHNO_PRINT("�l�����ς���� %d %d\n", num, num2);
        //_readFriendMatchStatus(wk);  //�Čv�Z
        BMP_MENULIST_STRBUFDelete(wk->menulist);
        //BMP_MENULIST_Delete(wk->menulist);
        //wk->menulist = BMP_MENULIST_Create( WIFIP2PMATCH_MEMBER_MAX+2, HEAPID_WIFIP2PMATCH );
        BmpListParamSet(wk->lw ,BMPLIST_ID_COUNT, num + 2);
        if(num > (WIFIP2PMATCH_DISP_MEMBER_MAX-2)){
            BmpListParamSet(wk->lw ,BMPLIST_ID_LINE, WIFIP2PMATCH_DISP_MEMBER_MAX);
        }
        else{
            BmpListParamSet(wk->lw ,BMPLIST_ID_LINE, num+2);
        }
        _addListString(wk);

	// �u������@����v�̉��ɋ󔒍s���o����̂�}��
	BmpListPosGet(wk->lw, &list, &cursor);
	if(num >= 3 && (num - 3) <= list){
	  list = num - 3;
	  if(list < 0)
	    list = 0;
	}
        BmpListMainOutControl(wk->lw, NULL,list,cursor,0,NULL,NULL);
	
        BmpListRewrite(wk->lw);
        wk->friendMatchReadCount = num;
    }

    BmpListPosGet(wk->lw, &list, &cursor);
    // �J�[�\���ʒu�̍ĕ\��
    if((wk->friendMatchReadCount+2) <= (list+cursor)){
      while((wk->friendMatchReadCount+2) <= (list+cursor)){
        if(list != 0){
	  list--;
        }
        else if(cursor != 0){
	  cursor--;
        }
      }
      OHNO_PRINT("�J�[�\���ʒu�ύX %d %d \n",list,cursor);
      BmpListMainOutControl(wk->lw, NULL,list,cursor,0,NULL,NULL);
      BmpListRewrite(wk->lw);
    }

    _iconMenuDisp(wk, list);
#if 0
    SetCursor_Pos(wk->MainActWork[_CLACT_LINE_CUR], 2*8, 8 * (cursor * 3 + 6));

    if(list > 0){    // ��J�[�\��
        SetCursor_Pos(wk->MainActWork[_CLACT_UP_CUR], 0x80, 32);
        CLACT_SetDrawFlag( wk->MainActWork[_CLACT_UP_CUR], 1 );
    }
    else{
        CLACT_SetDrawFlag( wk->MainActWork[_CLACT_UP_CUR], 0 );
    }
    if((list < (wk->friendMatchReadCount - WIFIP2PMATCH_DISP_MEMBER_MAX + 2))){
        SetCursor_Pos(wk->MainActWork[_CLACT_DOWN_CUR], 0x80, 0x9c);
        CLACT_SetDrawFlag( wk->MainActWork[_CLACT_DOWN_CUR], 1 );
    }
    else{
        CLACT_SetDrawFlag( wk->MainActWork[_CLACT_DOWN_CUR], 0 );
    }
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�̏o������Ǘ�����
 *
 *	@param	wk	���[�N
 *
 *	@retval	���̗F�B�̐�
 */
//-----------------------------------------------------------------------------
static int MCRSYS_ContFiendInOut( WIFIP2PMATCH_WORK* wk )
{
	int friend_num;
	int i, j;
	MCR_MOVEOBJ* p_obj;
	BOOL match;
	_WIFI_MACH_STATUS* p_status;
	BOOL in_flag;
	BOOL out_flag;

	// �ŐV�ł̗F�B���ƁA�F�B�i���o�[�z����쐬
	friend_num = _readFriendMatchStatus( wk );
//	wk->friendMatchReadCount = friend_num;

	in_flag = FALSE;
	out_flag = FALSE;

	// �o�b�N�A�b�v�Ɣ�r����
	if( memcmp( wk->index2No, wk->index2NoBackUp, sizeof(u8)*WIFIP2PMATCH_MEMBER_MAX ) != 0 ){

		// �l���ς����
		// �o�b�N�A�b�v���ɂ��āA�ŐV�łɂ��Ȃ����OUT
		// �ŐV�łɂ��āA�o�b�N�A�b�v�łɂ��Ȃ����IN
		for( i=0; i<WIFIP2PMATCH_MEMBER_MAX; i++ ){
			
			// OUT�`�F�b�N
			match = FALSE;
			for( j=0; j<WIFIP2PMATCH_MEMBER_MAX; j++ ){
				if( wk->index2No[j] == wk->index2NoBackUp[i] ){
					match = TRUE;
					break;
				}
			}
			// ���Ȃ������̂ō폜
			// ��C��32�l�����Ă����Ƃ�0�����Ȃ��Ƃ������ɂȂ��Ă��܂��̂�
			// �O����Ȃ������`�F�b�N
			if( (match == FALSE) && (wk->index2NoBackUp[i] != 0) ){
				p_obj = MCRSYS_GetMoveObjWork( wk, wk->index2NoBackUp[i] );
				if( p_obj != NULL ){
					WIFI_MCR_DelPeopleReq( &wk->matchroom, p_obj );
					MCRSYS_DelMoveObjWork( wk, p_obj );
					MCVSys_BttnDel( wk, wk->index2NoBackUp[i] );
					WifiP2PMatch_UserDispOff_Target( wk, wk->index2NoBackUp[i], HEAPID_WIFIP2PMATCH );	// ���̐l��������ʂɏڍו\������Ă���Ȃ����
					out_flag = TRUE;
				}
			}

			// IN�`�F�b�N
			match = FALSE;
			for( j=0; j<WIFIP2PMATCH_MEMBER_MAX; j++ ){
				if( wk->index2No[i] == wk->index2NoBackUp[j] ){
					match = TRUE;
					break;
				}
			}
			// �O�͂��Ȃ������̂Œǉ�
			if( (match == FALSE) && (wk->index2No[i] != 0) ){
				p_status = WifiFriendMatchStatusGet( wk, wk->index2No[i]-1 );

				p_obj = WIFI_MCR_SetNpc( &wk->matchroom, 
						p_status->trainer_view, wk->index2No[i] );


				// �o�^�ł������`�F�b�N
				if( p_obj ){

					MCVSys_BttnSet( wk, wk->index2No[i], MCV_BTTN_FRIEND_TYPE_IN );
					MCRSYS_SetMoveObjWork( wk, p_obj );

					in_flag = TRUE;
				}else{

					// �I�u�W�F���o�^�ł��Ȃ������̂ŗ\��o�^
					// �i���ꂾ�ƃ{�^�����������Ƃ��o���Ȃ��j
					MCVSys_BttnSet( wk, wk->index2No[i], MCV_BTTN_FRIEND_TYPE_RES );
					wk->index2No[i] = 0;	// �����Ă����̂͂Ȃ��������Ƃɂ���
				}
			}
		}

		// �ŐV�łɕύX
		memcpy( wk->index2NoBackUp, wk->index2No, sizeof(u8)*WIFIP2PMATCH_MEMBER_MAX );

		// �{�^���S�`�惊�N�G�X�g
		MCVSys_BttnAllWriteReq( wk );

		if( in_flag == TRUE ){
			Snd_SePlay( _SE_INOUT );
		}else if( out_flag == TRUE ){
			Snd_SePlay( _SE_INOUT );
		}
	}	


	return friend_num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�̏�ԕω���\��
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MCRSYS_ContFriendStatus( WIFIP2PMATCH_WORK* wk, u32 heapID )
{
	int i;
	u32 friendID;
	u32 type;
	BOOL vchat;
	int change_num;

	// ��ԕύX�����擾�i���܂łƓ������ꏏ�ɂ��邽�߂ɂ������Ă����j
    change_num = _checkUserDataMatchStatus( wk );

	// �F�̏�ԕω���o�^����
	if( change_num > 0 ){
		MCVSys_ReWrite( wk, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�I�u�W�F�N�g�̓o�^�j���Ǘ�
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MCRSYS_SetFriendObj( WIFIP2PMATCH_WORK* wk, u32 heapID )
{
	// �F�B�i���o�[���ς���Ă����炢�Ȃ��Ȃ����F�B�������A�������F�B��o�^
	MCRSYS_ContFiendInOut( wk );

	// ��Ԃ��ς�����牽����ς���
	MCRSYS_ContFriendStatus( wk, heapID );
}


//------------------------------------------------------------------
/**
 * $brief   �t�����h���X�g�\�������� WIFIP2PMATCH_MODE_FRIENDLIST
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int WifiP2PMatch_FriendList( WIFIP2PMATCH_WORK *wk, int seq )
{
    u32	ret = MCR_RET_NONE;
    int checkMatch;
	u32 check_friend;
	MCR_MOVEOBJ* p_obj;
	u32 status;


	// ���C�v�I���҂�
	if( WIPE_SYS_EndCheck() == FALSE ){
		return seq;
	}

	// �G���[�`�F�b�N
    if( CommStateIsWifiError()){  
//        wk->localTime=0;
        _errorDisp(wk);
        return seq;
    }

	// �{�C�X�`���b�gONOFF�J��Ԃ��ł����̉�ʂł͂܂��
	// �����ԂɂȂ�����DISCONNECT�����ɑJ��
	if( CommWifiIsMatched()==4 ){
		// ���̏����́A_parentModeCallMenuInit��������Ă��܂���
        // �K�����Ȃ�=>�ؒf������
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
        CommStateWifiMatchEnd();
		return seq;
	}

	/*
	 * 080607 tomoya
	 * comm_state.c����AUTO�ɂ���Ƃ������߂āA
	 * ���ׂāAcomm_state�̊O���i�A�v�����j�Őݒ�ύX����悤�ɂ��܂����B
	 * 
	// �����ɗ��Ă���̂ɁACommStateErrorCheck��Wi-Fi��AUTO�Ȃ�
	// ���������
	// 080605 tomoya  comm_state.c _stateWifiMatchEnd����AUTO�ɂȂ邽��
	if( CommStateGetErrorCheck() == TRUE ){
		CommStateSetErrorCheck( FALSE, FALSE );
	}
	//*/

 //   _mainListReWrite(wk);
 
	// �F�B�����ꂽ��o��
	MCRSYS_SetFriendObj( wk, HEAPID_WIFIP2PMATCH );
	MCVSys_UpdataBttn( wk );


	// �p�\�R���A�j���������Ă�����I��
	WIFI_MCR_PCAnmOff( &wk->matchroom );	

	// �{�C�X�`���b�g��ONOFF�̓v���C���[��������Ƃ��ɂ����ݒ�ł���
	if( WIFI_MCR_PlayerMovePauseGet( &wk->matchroom ) == FALSE ){
		// �{�C�X�`���b�g
		if(mydwc_IsNewPlayer() == -1){
			if(PAD_BUTTON_X & sys.trg){
				if(_myVChatStatusToggleOrg(wk)){
					WifiP2PMatchMessagePrint(wk, msg_wifilobby_054, FALSE);
				}
				else{
					WifiP2PMatchMessagePrint(wk, msg_wifilobby_055, FALSE);
				}
				Snd_SePlay(_SE_DESIDE);
				_userDataInfoDisp(wk);
				wk->seq = WIFIP2PMATCH_VCHATWIN_WAIT;
				WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );	// ������ʏ�����
				return seq;
			}
		}


		// ������ʂ������Ȃ�
		check_friend = MCVSys_Updata( wk, HEAPID_WIFIP2PMATCH );	// ������ʂ�������
		p_obj = MCRSYS_GetMoveObjWork( wk, check_friend );
		if( check_friend != 0 ){
			if( p_obj ){
				WIFI_MCR_CursorOn( &wk->matchroom, p_obj );
			}else{
				WIFI_MCR_CursorOff( &wk->matchroom );
			}
		}else{
			WIFI_MCR_CursorOff( &wk->matchroom );
		}

	}

#ifdef WFP2P_DEBUG   /// �u�b�g�`�s�����؂�ւ�
#if 0
    if(PAD_BUTTON_Y & sys.trg){
        switch(wk->vctEnc){
          case VCHAT_2BIT_ADPCM:
            wk->vctEnc = VCHAT_3BIT_ADPCM;
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_debug_03, FALSE);
            break;
          case VCHAT_3BIT_ADPCM:
            wk->vctEnc = VCHAT_4BIT_ADPCM;
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_debug_04, FALSE);
            break;
          case VCHAT_4BIT_ADPCM:
          default:
            wk->vctEnc = VCHAT_2BIT_ADPCM;
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_debug_02, FALSE);
            break;
        }
        mydwc_setVchat(wk->vctEnc);
        wk->seq = WIFIP2PMATCH_VCHATWIN_WAIT;
		WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );	// ������ʏ�����
        return seq;
    }
#endif
#endif

	// �F�B���炱����ɐڑ����Ă����Ƃ��̏���
    checkMatch = _checkParentConnect(wk);
    if( (0 !=  checkMatch) && (wk->preConnect != -1) ){ // �ڑ����Ă���
        OHNO_PRINT("�ڑ����E���Ă��܂��Ă���-------------\n");
        Snd_SePlay(_SE_OFFER);
        wk->seq = WIFIP2PMATCH_MODE_CALL_INIT;
        return seq;
    }
    if((wk->preConnect == -1) && (mydwc_IsNewPlayer() != -1)){	// �ʏ�̃R�l�N�g�J�n
        wk->preConnect = mydwc_IsNewPlayer();
        _friendNameExpand(wk, wk->preConnect);
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_043, FALSE);

		// �{�C�X�`���b�g�ݒ�
		mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
		OS_TPrintf( "Connect VCHAT set\n" );

		// �Ȃ������l�̃f�[�^�\��
//		WifiP2PMatch_UserDispOn( wk, wk->preConnect+1, HEAPID_WIFIP2PMATCH );

        wk->localTime = 0;
    }
    else if((wk->preConnect != -1) && (mydwc_IsNewPlayer() == -1)){	// �����I�ɂ͐V�����R�l�N�g���Ă����̂ɁA���ۂ̓R�l�N�g���Ă��Ă��Ȃ������Ƃ��H

/*
        GF_BGL_BmpWinDel(&wk->ListWin);
        BmpListExit(wk->lw, NULL, NULL);
        wk->lw = NULL;
        BMP_MENULIST_Delete( wk->menulist );
        wk->menulist = NULL;
//*/

		// �{�C�X�`���b�g�ݒ�
		mydwc_setVChat(FALSE);// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
        _friendNameExpand(wk, wk->preConnect);
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        CommStateWifiMatchEnd();
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
        return seq;
    }

	// VCHAT�@ON�@��ԂȂ̂ɁA�V�����R�l�N�g��-1�Ȃ�VCHAT��OFF�ɂ���
	// 080615	tomoya
	if( (mydwc_IsVChat() == TRUE) && (mydwc_IsNewPlayer() == -1) ){
		mydwc_setVChat(FALSE);// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
	}
	
/*    if(wk->localTime){
        wk->localTime--;
        if(wk->localTime == 0){
            EndMessageWindowOff(wk);
        }
    }*/

//    _mainListReWrite(wk);


	// ��Ԃ��擾
	status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );

	// �N�������ɐڑ����Ă��Ă��Ȃ��Ƃ��������X�g�𓮂�����
    if(wk->preConnect == -1){

		// CANCEL�{�^���ł��ҋ@��Ԃ��N���A
		if( sys.trg & PAD_BUTTON_CANCEL ){
			if(_modeWait(status)){	// �҂���Ԃ̂Ƃ�
				Snd_SePlay(_SE_DESIDE);
				wk->seq = WIFIP2PMATCH_MODE_SELECT_REL_INIT;  // ����
				WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );	// ������ʏ�����
				return seq;
			}
		}
		
		ret = WIFI_MCR_Main( &wk->matchroom );
		WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��
//        ret = BmpListMain(wk->lw);
    }
    switch(ret){
      case MCR_RET_NONE:
        return seq;
      case MCR_RET_CANCEL:
        Snd_SePlay(_SE_DESIDE);
        if(_modeWait(status)){	// �҂���Ԃ̂Ƃ�
            wk->seq = WIFIP2PMATCH_MODE_SELECT_REL_INIT;  // �������ҋ@�̏ꍇ�K������
        }
        else{	// ���ꂩ�I���`�F�b�N��
            wk->endSeq = WIFI_P2PMATCH_END;
            wk->seq = WIFIP2PMATCH_MODE_EXIT_YESNO;
            WifiP2PMatchMessagePrint(wk, dwc_message_0010, TRUE);
            //        wk->localTime=0;
            ret = BMPLIST_CANCEL;
        }
		WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );	// ������ʏ�����
        return seq;

      case MCR_RET_MYSELECT:
//        wk->localTime=0;
        Snd_SePlay(_SE_DESIDE);
        if(_modeWait(status)){
			WIFI_MCR_PCAnmStart( &wk->matchroom );	// pc�A�j���J�n
            wk->seq = WIFIP2PMATCH_MODE_SELECT_REL_INIT;  // �������ҋ@�̏ꍇ�K������
        }
        else{  // ��W�̍s�őI�������Ƃ�
            if(status == WIFI_STATUS_LOGIN_WAIT){

				// �����̏�Ԃ��A�}�b�`���O�܂ł����Ă��邩�`�F�b�N����
				// 080628	tomoya
				if( CommStateIsWifiLoginMatchState() == TRUE ){
					WIFI_MCR_PCAnmStart( &wk->matchroom );	// pc�A�j���J�n
	                wk->seq = WIFIP2PMATCH_MODE_SELECT_INIT;
				}
            }
        }
		WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );	// ������ʏ�����
        return seq;
        break;

	case MCR_RET_SELECT:
        Snd_SePlay(_SE_DESIDE);
        if(_modeWait(status)){
            wk->seq = WIFIP2PMATCH_MODE_SELECT_REL_INIT;  // �������ҋ@�̏ꍇ�K������
        }
        else{  // �l�̖��O�[���}�b�`���O��
            wk->seq = WIFIP2PMATCH_MODE_MATCH_INIT;
        }
		WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );	// ������ʏ�����
		return seq;
		break;

	default:
		GF_ASSERT(0);
		break;
    }
	/*
    GF_BGL_BmpWinDel(&wk->ListWin);
    BmpListExit(wk->lw, NULL, NULL);
    wk->lw = NULL;
    BMP_MENULIST_Delete( wk->menulist );
    wk->menulist = NULL;
	//*/
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCTONOFF�E�C���h�E�����
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _vchatToggleWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( 0 !=  _checkParentNewPlayer(wk)){ // �ڑ����Ă���
        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
		// ��l���̓��������
		FriendRequestWaitOff( wk );
    }
    else if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
		// ��l���̓��������
		FriendRequestWaitOff( wk );
        EndMessageWindowOff(wk);
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCT�ڑ��J�n
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_VCTConnectInit2( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT_INIT2
{
	_WIFI_MACH_STATUS* p_status;
	int vchat;
	int status;

	// VChat�t���O���擾
	p_status = WifiFriendMatchStatusGet( wk, wk->friendNo - 1 );
	vchat	= p_status->vchat;
	status	= _WifiMyStatusGet( wk, p_status );

    wk->cancelEnableTimer--;
    if(wk->cancelEnableTimer < 0  ){
        wk->seq = WIFIP2PMATCH_MODE_CANCEL_ENABLE_WAIT;
    }
    else
        if(CommWifiIsMatched() == 5){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_012, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommWifiIsMatched() == 3 ){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if((CommWifiIsMatched() == 4) || (CommStateIsWifiDisconnect())){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_016, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommStateIsWifiError()){
        _errorDisp(wk);
    }
#if PLFIX_T869
	// VCHAT�t���O��OFF�ɂȂ��Ă�����ʐM�ؒf
    else if(vchat == FALSE){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
	}
#endif
	// ����̏�Ԃ��ʏ��ԂłȂ��ꍇ�ؒf
	else if( status != WIFI_STATUS_LOGIN_WAIT ){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
	}
	else if(CommWifiIsMatched()==1){  // �Ȃ�����
	//    _timeWaitIconDel(wk);		timeWait����MsgWin��j�����Ă���Ƃ������Ƃ̓��b�Z�[�W�I���ł�OK��
		EndMessageWindowOff(wk);
        _myStatusChange(wk, WIFI_STATUS_VCT);  // VCT���ɂȂ�
        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT; // ���ɍs���ۂɌĂ�
		WifiList_SetLastPlayDate( wk->pList, wk->friendNo - 1);	// �Ō�ɗV�񂾓��t�́AVCT���Ȃ������Ƃ��ɐݒ肷��
    }

    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCT�ڑ��J�n
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_VCTConnectInit( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT_INIT
{

	// 080703	tomoya takahashi
	// ��͂�͂܂�Ƃ��͂���悤�Ȃ̂ŁA
	//�@�����Ɏ��ɂ����悤�ɏC������
	_friendNameExpand(wk, mydwc_getFriendIndex());
	WifiP2PMatchMessagePrint(wk, msg_wifilobby_011, FALSE);
	_myStatusChange(wk, WIFI_STATUS_VCT);  // VCT���ɂȂ�
	wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT_WAIT; // ���ɍs���ۂɌĂ�

	WifiList_SetLastPlayDate( wk->pList, mydwc_getFriendIndex());	// �Ō�ɗV�񂾓��t�́AVCT���Ȃ������Ƃ��ɐݒ肷��

#if 0
    if(CommWifiIsMatched()==1){  // �Ȃ�����
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_011, FALSE);
        _myStatusChange(wk, WIFI_STATUS_VCT);  // VCT���ɂȂ�
        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT_WAIT; // ���ɍs���ۂɌĂ�
    }
#endif

    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCT�ڑ��܂�
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_VCTConnectWait( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT_WAIT
{

	_WIFI_MACH_STATUS* p_status;
	int vchat;
	int status;

	if(CommStateIsWifiError()){
        _errorDisp(wk);
		return seq;
    }

	// 080624�@�ǉ�
	// �ʐM���Z�b�g������index��0��菬�����Ȃ�
	// ��������ؒf
	if( mydwc_getFriendIndex() < 0 ){
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_016, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
		return seq;
	}

	// 080624�@�ǉ�
	// status���`�F�b�N
	p_status = WifiFriendMatchStatusGet( wk, mydwc_getFriendIndex() );
	status	= _WifiMyStatusGet( wk, p_status );
	if( (status != WIFI_STATUS_LOGIN_WAIT) && (status != WIFI_STATUS_VCT) ){
		_friendNameExpand(wk, mydwc_getFriendIndex());
		WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
		wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
		return seq;
	}
	
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
            EndMessageWindowOff(wk);
            wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT; // ���ɍs���ۂɌĂ�
        }
        else{
            WifiP2PMatch_VCTConnect(wk,seq);
        }
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCT�ڑ���  WIFIP2PMATCH_MODE_VCT_CONNECT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_VCTConnect( WIFIP2PMATCH_WORK *wk, int seq )
{
	_WIFI_MACH_STATUS* p_status;
	int status;


	// 080624�@�ύX
	//if( mydwc_getFriendIndex() < 0 ){
    //    wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
	//}
	// �ʐM���Z�b�g������index��0��菬�����Ȃ�
	// ��������ؒf
	if( mydwc_getFriendIndex() < 0 ){
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_016, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
		return seq;
	}

	
	// VChat�t���O���擾
	p_status = WifiFriendMatchStatusGet( wk, mydwc_getFriendIndex() );
	status	= _WifiMyStatusGet( wk, p_status );

//    _mainListReWrite(wk);
	// �F�B�����ꂽ��o��
	MCRSYS_SetFriendObj( wk, HEAPID_WIFIP2PMATCH );
	MCVSys_UpdataBttn( wk );

#if 0
    CLACT_SetDrawFlag( wk->MainActWork[_CLACT_VCT_MOVE], 0 );
    CLACT_SetDrawFlag( wk->MainActWork[_CLACT_VCT_STOP], 0 );
#endif
    if(CommWifiIsMatched() == 5){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_012, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommWifiIsMatched() == 3 ){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if((CommWifiIsMatched() == 4) || (CommStateIsWifiDisconnect())){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_016, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommStateIsWifiError()){
        _errorDisp(wk);
    }
	// ����̏�Ԃ��ʏ���orVCT��ԂłȂ��ꍇ�ؒf
	else if( (status != WIFI_STATUS_LOGIN_WAIT) && (status != WIFI_STATUS_VCT) ){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
	}
    else if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_017, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECTEND_YESNO;
    }
    else{
		// �A�C�R����������
        if(mydwc_IsSendVoiceAndInc()){
			WifiP2PMatchFriendListIconWrite( wk->bgl, &wk->icon, GF_BGL_FRAME1_M,
				PLAYER_DISP_ICON_POS_X, PLAYER_DISP_ICON_POS_Y, PLAYER_DISP_ICON_IDX_VCTBIG, 0 );
        }
        else{
			WifiP2PMatchFriendListIconWrite( wk->bgl, &wk->icon, GF_BGL_FRAME1_M,
				PLAYER_DISP_ICON_POS_X, PLAYER_DISP_ICON_POS_Y, PLAYER_DISP_ICON_IDX_VCT, 0 );
        }
    }

    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCT�ڑ�����
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_VCTConnectEndYesNo( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT
{
	if(CommStateIsWifiError()){
        _errorDisp(wk);
		return seq;
    }
	
    //Snd_PlayerSetInitialVolume( SND_HANDLE_BGM, PV_VOL_DEFAULT/3 );
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        WifiList_SetLastPlayDate( wk->pList, mydwc_getFriendIndex());	// �Ō�ɗV�񂾓��t�́AVCT���Ȃ������Ƃ��ɐݒ肷��
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECTEND_WAIT;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCT�ڑ��I��  WIFIP2PMATCH_MODE_VCT_CONNECTEND_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_VCTConnectEndWait( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);

    if(ret == BMPMENU_NULL){  // �܂��I��
        if((CommWifiIsMatched() >= 4) || CommStateIsWifiDisconnect() || !CommIsConnect(COMM_PARENT_ID)){

			// ���[�U�[�f�[�^OFF
//			WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );
//
			EndMessageWindowOff(wk);

            _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
            BmpYesNoWinDel( wk->pYesNoWork, HEAPID_WIFIP2PMATCH );
            wk->preConnect = -1;
            wk->timer = _RECONECTING_WAIT_TIME;
            wk->seq = WIFIP2PMATCH_RECONECTING_WAIT;
        }else if( CommStateIsWifiError() ){
			_errorDisp(wk);
		}
        return seq;
    }else if(ret == 0){ // �͂���I�������ꍇ

        if(!CommStateIsWifiError()){
            _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
            CommStateWifiBattleMatchEnd();
            wk->preConnect = -1;
            wk->timer = _RECONECTING_WAIT_TIME;
            wk->seq = WIFIP2PMATCH_RECONECTING_WAIT;
        }else{
			_errorDisp(wk);
		}
    }
    else{  // ��������I�������ꍇ
        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT;
    }
    EndMessageWindowOff(wk);
    return seq;
}



//WIFIP2PMATCH_MODE_VCT_DISCONNECT
static int WifiP2PMatch_VCTDisconnect(WIFIP2PMATCH_WORK *wk, int seq)
{
	u32 status;
	
	if(CommStateIsWifiError()){
        _errorDisp(wk);
		return seq;
    }
	
	
	// ���[�U�[�f�[�^OFF
//	WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );

	status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );
	
    if(status != WIFI_STATUS_LOGIN_WAIT){
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
    }
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        wk->timer = 30;
        return seq;
    }
    wk->timer--;
    if((sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)) || (wk->timer==0)){
        EndMessageWindowOff(wk);
        CommStateWifiMatchEnd();

		// �ʐM��Ԃ����ɖ߂�
		CommStateChangeWiFiLogin();
		
//        if(wk->menulist==NULL){
		if( WIFI_MCR_GetInitFlag( &wk->matchroom ) == FALSE ){
            wk->timer = _RECONECTING_WAIT_TIME;
            wk->seq = WIFIP2PMATCH_RECONECTING_WAIT;
        }
        else{

			// VCHAT�����ɖ߂�
			_myVChatStatusOrgSet( wk );
			_userDataInfoDisp(wk);
			
            wk->preConnect = -1;
            wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;

			// ��l���̓��������
			FriendRequestWaitOff( wk );
        }
    }
    return seq;
}

//WIFIP2PMATCH_MODE_BATTLE_DISCONNECT
static int WifiP2PMatch_BattleDisconnect(WIFIP2PMATCH_WORK *wk, int seq)
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }
    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        EndMessageWindowOff(wk);
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
        CommStateWifiMatchEnd();
        wk->timer = _RECONECTING_WAIT_TIME;
        wk->seq = WIFIP2PMATCH_RECONECTING_WAIT;
    }
    return seq;
}

//WIFIP2PMATCH_MODE_DISCONNECT
static int WifiP2PMatch_Disconnect(WIFIP2PMATCH_WORK *wk, int seq)
{
	// ���[�U�[�f�[�^OFF
//	WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );
//
	// �G���[�`�F�b�N
    if( CommStateIsWifiError() ){
        _errorDisp(wk);
        return seq;
    }


    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        wk->timer = 30;
        return seq;
    }
    wk->timer--;
    if((sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)) || (wk->timer==0)){
        EndMessageWindowOff(wk);
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);

		// �ʐM��Ԃ����ɖ߂�
		CommStateChangeWiFiLogin();
//        if(wk->menulist==NULL){
		if( WIFI_MCR_GetInitFlag( &wk->matchroom ) == FALSE ){
            wk->seq = WifiP2PMatchFriendListStart();
        }
        else{

			// VCHAT���I���W�i���ɖ߂�
			_myVChatStatusOrgSet( wk );
			_userDataInfoDisp(wk);
			
            wk->preConnect = -1;
            wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
			// ��l���̓��������
			FriendRequestWaitOff( wk );
        }
    }
    return seq;
}

static int WifiP2PMatch_BattleConnectInit( WIFIP2PMATCH_WORK *wk, int seq )    // WIFIP2PMATCH_MODE_BATTLE_CONNECT_INIT
{
    return seq;
}

static int WifiP2PMatch_BattleConnectWait( WIFIP2PMATCH_WORK *wk, int seq )     // WIFIP2PMATCH_MODE_BATTLE_CONNECT_WAIT
{
    return seq;
}

static int WifiP2PMatch_BattleConnect( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_BATTLE_CONNECT
{
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   �J�[�\���ʒu��ύX����
 *
 * @param   act		�A�N�^�[�̃|�C���^
 * @param   x
 * @param   y
 *
 * @retval  none
 */
//------------------------------------------------------------------
static void SetCursor_Pos( CLACT_WORK_PTR act, int x, int y )
{
    VecFx32 mat;

    mat.x = FX32_CONST( x );
    mat.y = FX32_CONST( y );
    mat.z = 0;
    CLACT_SetMatrix( act, &mat);

}

//------------------------------------------------------------------
/**
 * $brief   �{�^���������ƏI��  WIFIP2PMATCH_MODE_CHECK_AND_END
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int WifiP2PMatch_CheckAndEnd( WIFIP2PMATCH_WORK *wk, int seq )
{
	if( wk->timer > 0 ){
		wk->timer --;
		return seq;
	}

    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){


        WifiP2PMatchMessagePrint(wk, dwc_message_0011, TRUE);
        wk->seq = WIFIP2PMATCH_MODE_EXITING;
        wk->timer = 1;
		
/*
        wk->endSeq = WIFI_P2PMATCH_END;
        wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        CommStateWifiLogout();
//*/
    }
    return seq;
}



//------------------------------------------------------------------
/**
 * $brief   ��W�̎����� WIFIP2PMATCH_MODE_SELECT_REL_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _parentModeSelectRelInit( WIFIP2PMATCH_WORK *wk, int seq )
{
	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��
    WifiP2PMatchMessagePrint(wk, msg_wifilobby_007, FALSE);
    wk->seq = WIFIP2PMATCH_MODE_SELECT_REL_YESNO;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ��W�̎�����  WIFIP2PMATCH_MODE_SELECT_REL_YESNO
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _parentModeSelectRelYesNo( WIFIP2PMATCH_WORK* wk, int seq )
{
	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_MODE_SELECT_REL_WAIT;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ��W�̎�����  WIFIP2PMATCH_MODE_SELECT_REL_WAIT
 * @param   wk
 * @param   seq
 * @retval  int
 */
//------------------------------------------------------------------

static int _parentModeSelectRelWait( WIFIP2PMATCH_WORK* wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);

	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��

    if((wk->preConnect == -1) && (mydwc_IsNewPlayer() != -1)){  // �ڑ���������

		// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
		mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);
		OS_TPrintf( "Connect VCHAT set\n" );

		// ���ł�YesNoSelectMain�ŉ������ĂȂ����
		if( ret == BMPMENU_NULL ){
			BmpYesNoWinDel( wk->pYesNoWork, HEAPID_WIFIP2PMATCH );
		}
        ret = 1;  // �������ɕύX
    }
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }else if(ret == 0){ // �͂���I�������ꍇ
        //��������
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);

		// �ʐM��Ԃ����ɖ߂�
		CommStateChangeWiFiLogin();

		// ��l���̓��������
		FriendRequestWaitOff( wk );
    }
    else{  // ��������I�������ꍇ

		// ��l�������~���ēx�\��
		FriendRequestWaitOff( wk );
		FriendRequestWaitOn( wk, TRUE );
    }
    wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;

    return seq;
}



//------------------------------------------------------------------
/**
 * $brief   �ҋ@��ԂɂȂ�ׂ̑I�����j���[ WIFIP2PMATCH_MODE_SELECT_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
#define WIFI_STSET_SINGLEBATTLE	(WIFI_STATUS_NONE)		// �I�����̖������̂����蓖�Ă�
#define WIFI_STSET_DOUBLEBATTLE	(WIFI_STATUS_VCT)
#define WIFI_STSET_MINIGAME		(WIFI_STATUS_UNKNOWN)

typedef struct {
    u32  str_id;
    u32  param;
} _infoMenu;

//  ���j���[���X�g
#define WIFI_PARENTINFO_MENULIST_MAX	  ( 10 )
#ifdef WFP2P_DEBUG_EXON 
#define WIFI_PARENTINFO_MENULIST_COMMON	  ( 5 )	// �ϓ����Ȃ������̐�
#else
#define WIFI_PARENTINFO_MENULIST_COMMON	  ( 3 )	// �ϓ����Ȃ������̐�
#endif
_infoMenu _parentInfoMenuList[ WIFI_PARENTINFO_MENULIST_MAX ] = {
	// �ϓ����Ȃ�����
    { msg_wifilobby_057, (u32)WIFI_STSET_SINGLEBATTLE },
    { msg_wifilobby_058, (u32)WIFI_STSET_DOUBLEBATTLE },
    { msg_wifilobby_025, (u32)WIFI_STATUS_TRADE_WAIT },
#ifdef WFP2P_DEBUG_EXON 
    { msg_wifilobby_debug_00, (u32)WIFI_STATUS_BATTLEROOM_WAIT },
    { msg_wifilobby_debug_01, (u32)WIFI_STATUS_MBATTLE_FREE_WAIT },
#endif
};

// �ǉ����X�g�f�[�^
static _infoMenu _minigameMenu = {
	msg_wifilobby_147, (u32)WIFI_STSET_MINIGAME
};
static _infoMenu _frontierMenu = {
	msg_wifilobby_138, (u32)WIFI_STATUS_FRONTIER_WAIT
};
static _infoMenu _pofinMenu = {
	msg_wifilobby_100, (u32)WIFI_STATUS_POFIN_WAIT
};
static _infoMenu _endMenu = { 
	msg_wifilobby_032, (u32)BMPLIST_CANCEL
};


// �~�j�Q�[��
_infoMenu _parentMiniGameInfoMenuList[] = {
    { msg_wifilobby_mg02, (u32)WIFI_STATUS_BUCKET_WAIT },
    { msg_wifilobby_mg02, (u32)WIFI_STATUS_BALANCEBALL_WAIT },
    { msg_wifilobby_mg02, (u32)WIFI_STATUS_BALLOON_WAIT },
    { msg_wifilobby_032, (u32)BMPLIST_CANCEL },
};

_infoMenu _parentSingleInfoMenuList[] = {
    { msg_wifilobby_059, (u32)WIFI_STATUS_SBATTLE_FREE_WAIT },
    { msg_wifilobby_060, (u32)WIFI_STATUS_SBATTLE50_WAIT },
    { msg_wifilobby_061, (u32)WIFI_STATUS_SBATTLE100_WAIT },
    { msg_wifilobby_032, (u32)BMPLIST_CANCEL },
};

_infoMenu _parentDoubleInfoMenuList[] = {
    { msg_wifilobby_062, (u32)WIFI_STATUS_DBATTLE_FREE_WAIT },
    { msg_wifilobby_063, (u32)WIFI_STATUS_DBATTLE50_WAIT },
    { msg_wifilobby_064, (u32)WIFI_STATUS_DBATTLE100_WAIT },
    { msg_wifilobby_032, (u32)BMPLIST_CANCEL },
};


///�I�����j���[�̃��X�g
static BMPLIST_HEADER _parentInfoMenuListHeader = {
    NULL,			// �\�������f�[�^�|�C���^
    _curCallBack,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
    NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
    NULL,                   //
    WIFI_PARENTINFO_MENULIST_COMMON,// ���X�g���ڐ�
    WIFI_PARENTINFO_MENULIST_COMMON,// �\���ő區�ڐ�
    0,						// ���x���\���w���W
    8,						// ���ڕ\���w���W
    0,						// �J�[�\���\���w���W
    0,						// �\���x���W
    FBMP_COL_BLACK,			// �����F
    FBMP_COL_WHITE,			// �w�i�F
    FBMP_COL_BLK_SDW,		// �����e�F
    0,						// �����Ԋu�w
    16,						// �����Ԋu�x
    BMPLIST_NO_SKIP,		// �y�[�W�X�L�b�v�^�C�v
    FONT_SYSTEM,				// �����w��
    0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
    NULL                    // work
};

///�I�����j���[�̃��X�g
static const BMPLIST_HEADER _parentInfoBattleMenuListHeader = {
    NULL,			// �\�������f�[�^�|�C���^
    _curCallBack,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
    NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
    NULL,                   //
    NELEMS(_parentSingleInfoMenuList),	// ���X�g���ڐ�
    NELEMS(_parentSingleInfoMenuList),	// �\���ő區�ڐ�
    0,						// ���x���\���w���W
    8,						// ���ڕ\���w���W
    0,						// �J�[�\���\���w���W
    0,						// �\���x���W
    FBMP_COL_BLACK,			// �����F
    FBMP_COL_WHITE,			// �w�i�F
    FBMP_COL_BLK_SDW,		// �����e�F
    0,						// �����Ԋu�w
    16,						// �����Ԋu�x
    BMPLIST_NO_SKIP,		// �y�[�W�X�L�b�v�^�C�v
    FONT_SYSTEM,				// �����w��
    0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
    NULL                    // work
};

///�I�����j���[�̃��X�g
static const BMPLIST_HEADER _parentInfoMiniGameMenuListHeader = {
    NULL,			// �\�������f�[�^�|�C���^
    _curCallBack,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
    NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
    NULL,                   //
    NELEMS(_parentMiniGameInfoMenuList),	// ���X�g���ڐ�
    NELEMS(_parentMiniGameInfoMenuList),	// �\���ő區�ڐ�
    0,						// ���x���\���w���W
    8,						// ���ڕ\���w���W
    0,						// �J�[�\���\���w���W
    0,						// �\���x���W
    FBMP_COL_BLACK,			// �����F
    FBMP_COL_WHITE,			// �w�i�F
    FBMP_COL_BLK_SDW,		// �����e�F
    0,						// �����Ԋu�w
    16,						// �����Ԋu�x
    BMPLIST_NO_SKIP,		// �y�[�W�X�L�b�v�^�C�v
    FONT_SYSTEM,				// �����w��
    0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
    NULL                    // work
};


#define PARENTMENU_Y	( 3 )
static int _parentModeSelectMenuInit( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i,length;
    BMPLIST_HEADER list_h;
	_infoMenu* p_menu;

	// ���X�g��{���ݒ�
	length = WIFI_PARENTINFO_MENULIST_COMMON;
	list_h = _parentInfoMenuListHeader;
	p_menu = _parentInfoMenuList;
	
	// �����̗L��
	if( _pofinCaseCheck( wk ) == TRUE ){
		length ++;			//  ���ڐ��ǉ�
		list_h.count ++;	// ���X�g�ő區�ڐ��ǉ�
		list_h.line ++;		// ���X�g�\���ő吔�ǉ�

		p_menu[ length-1 ] = _pofinMenu;
	}

	// �t�����e�B�A�̗L��
	if( _frontierInCheck( wk ) == TRUE ){
		length ++;			//  ���ڐ��ǉ�
		list_h.count ++;	// ���X�g�ő區�ڐ��ǉ�
		list_h.line ++;		// ���X�g�\���ő吔�ǉ�

		p_menu[ length-1 ] = _frontierMenu;
	}

	// �~�j�Q�[���̗L��
	if( _miniGameInCheck( wk ) == TRUE ){
		length ++;			//  ���ڐ��ǉ�
		list_h.count ++;	// ���X�g�ő區�ڐ��ǉ�
		list_h.line ++;		// ���X�g�\���ő吔�ǉ�

		p_menu[ length-1 ] =_minigameMenu;
	}

	// �I���̒ǉ�
	{
		length ++;			//  ���ڐ��ǉ�
		list_h.count ++;	// ���X�g�ő區�ڐ��ǉ�
		list_h.line ++;		// ���X�g�\���ő吔�ǉ�

		p_menu[ length-1 ] =_endMenu;
	}

    wk->submenulist = BMP_MENULIST_Create( length , HEAPID_WIFIP2PMATCH );
    for(i=0; i< length ; i++){
        BMP_MENULIST_AddArchiveString( wk->submenulist, wk->MsgManager, p_menu[i].str_id, p_menu[i].param );
    }
    if(GF_BGL_BmpWinAddCheck(&wk->SubListWin)){
        BmpMenuWinClear(&wk->SubListWin,WINDOW_TRANS_ON);
        GF_BGL_BmpWinDel(&wk->SubListWin);
    }
    //BMP�E�B���h�E����
    GF_BGL_BmpWinAdd(wk->bgl,&wk->SubListWin,
                     GF_BGL_FRAME2_M, 16, PARENTMENU_Y, 15, length * 2, FLD_SYSFONT_PAL, FLD_MENU_WIN_CGX - length * 2);
    BmpMenuWinWrite(&wk->SubListWin, WINDOW_TRANS_OFF, MENU_WIN_CGX_NUM, MENU_WIN_PAL );
    list_h.list = wk->submenulist;
    list_h.win = &wk->SubListWin;

    wk->sublw = BmpListSet(&list_h, 0, wk->battleCur, HEAPID_WIFIP2PMATCH);
    GF_BGL_BmpWinOnVReq(&wk->SubListWin);


    WifiP2PMatchMessagePrint(wk, msg_wifilobby_006, FALSE);


	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��

    wk->seq = WIFIP2PMATCH_MODE_SELECT_WAIT;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �ҋ@��ԂɂȂ�ׂ̑I�����j���[ WIFIP2PMATCH_MODE_SELECT_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _battleSubMenuInit( WIFIP2PMATCH_WORK *wk, int ret );
static int _parentModeSelectMenuWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    u32	ret;
    int num = _getBattlePokeNum(wk);
	int pofin_ret = _isPofinItemCheck(wk);

	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��


    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){

		// �����Őڑ����Ă���\��������	080617	tomoya
		if( 0 !=  _checkParentNewPlayer(wk)){ // �ڑ����Ă���
			wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
			BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
			GF_BGL_BmpWinDel(&wk->SubListWin);
			BmpListExit(wk->sublw, NULL, &wk->battleCur);
			BMP_MENULIST_Delete( wk->submenulist );
	        EndMessageWindowOff(wk);
		}
        return seq;
    }
    ret = BmpListMain(wk->sublw);

    if( 0 !=  _checkParentNewPlayer(wk)){ // �ڑ����Ă���
        ret = BMPLIST_CANCEL;
    }
    switch(ret){
      case BMPLIST_NULL:
        return seq;
      case BMPLIST_CANCEL:
        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
        EndMessageWindowOff(wk);
        Snd_SePlay(_SE_DESIDE);
        break;
      default:
        Snd_SePlay(_SE_DESIDE);
		// �|�t�B���A�C�e���`�F�b�N
		if( (ret == WIFI_STATUS_POFIN_WAIT) && (pofin_ret != WFP2P_POFIN_RET_OK) ){
			if( pofin_ret == WFP2P_POFIN_RET_NUTSNONE ){
	            WifiP2PMatchMessagePrint(wk, msg_wifilobby_105, FALSE);			
			}else if( pofin_ret == WFP2P_POFIN_RET_POFINCASENONE ){
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_121, FALSE);
			}else{
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_106, FALSE);
			}
            wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
		}
		// �����`�F�b�N
		else if((ret == WIFI_STATUS_TRADE_WAIT) && (2 > num )){
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_053, FALSE);
            wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
        }
		// �_�u���o�g���`�F�b�N
        else if((ret == WIFI_STSET_DOUBLEBATTLE) && (2 > num)){
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_067, FALSE);
            wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
        }
		// �T�u���X�g������
        else if((ret == WIFI_STSET_SINGLEBATTLE) || 
				(ret == WIFI_STSET_DOUBLEBATTLE) || 
				(ret == WIFI_STSET_MINIGAME) ){
//            _myStatusChange(wk, ret);
            BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
            GF_BGL_BmpWinDel(&wk->SubListWin);
            BmpListExit(wk->sublw, NULL,  &wk->battleCur);
            BMP_MENULIST_Delete( wk->submenulist );
            _battleSubMenuInit(wk, ret);
            wk->seq = WIFIP2PMATCH_MODE_SUBBATTLE_WAIT;
            return seq;
//            EndMessageWindowOff(wk);
        }
        else{
            _myStatusChange(wk, ret);
            wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
            EndMessageWindowOff(wk);
        }
        // wifipofin�̎��́Awifi�ʐM��wifi pofin ���[�h�ɂ���
		// �|�t�B���ҍ�����
		{
			BOOL  msg_on = TRUE;
			
            if( _wait2to4Mode(ret) && (wk->seq == WIFIP2PMATCH_MODE_FRIENDLIST) ){
                wk->seq = WIFIP2PMATCH_PARENT_RESTART;  // �ċN����҂�
                WifiP2PMatch_CommWifiBattleStart( wk, -1, ret );
                _commStateChange( ret );
                _myStatusChange(wk, ret);
                wk->endSeq = _get2to4ModeEndSeq( ret );
				msg_on = FALSE;
			}
            else{
				WifiP2PMatch_CommWifiBattleStart( wk, -1, ret );	// �����ł��ʐM���@��ύX���邽�߂ɂ����Ɉړ�
                _commStateChange( ret );
/*				��commStateChange�ł悢
				if(ret == WIFI_STATUS_FRONTIER_WAIT){
//					WifiP2PMatch_CommWifiBattleStart( wk, -1, ret );
//
					CommSetWifiBothNet(FALSE);
					CommStateChangeWiFiFactory();
				}
*/
            }

			if( wk->seq == WIFIP2PMATCH_MODE_FRIENDLIST ){
				// �����~������
				FriendRequestWaitOn( wk, msg_on );
			}
		}
        break;
    }
    BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
    GF_BGL_BmpWinDel(&wk->SubListWin);
    BmpListExit(wk->sublw, NULL, &wk->battleCur);
    BMP_MENULIST_Delete( wk->submenulist );

    return seq;

}

//------------------------------------------------------------------
/**
 * $brief   �Q�[���̓��e�����܂����̂ŁA�e�Ƃ��čď�����  WIFIP2PMATCH_PARENT_RESTART
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _parentRestart( WIFIP2PMATCH_WORK *wk, int seq )
{
    u32	ret;

	// �G���[�`�F�b�N
    if( CommStateIsWifiError() ){
        _errorDisp(wk);
        return seq;
    }

	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��
    if( 0 !=  _checkParentNewPlayer(wk)){ // �ڑ����Ă���

        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
    }
    // wifipofin�̎��́Awifi�ʐM��wifi pofin ���[�h�ɂ��� �|�t�B���ҍ�����
    if(CommStateIsWifiLoginMatchState()){
//        CommStateChangeWiFiLogin();
//        _myStatusChange(wk, ret);
//

		// �ʐM�G���[�`�F�b�N
		// 080630	tomoya takahashi
		CommStateSetErrorCheck(FALSE,TRUE);	// �ؒf�̓G���[����Ȃ�

        CommSetWifiBothNet(FALSE);
        WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);
//        wk->endSeq = _get2to4ModeEndSeq( ret );
        wk->bRetryBattle = FALSE;

        CommInfoInitialize( wk->pSaveData, NULL );

        // ������������
        CommInfoSendPokeData();

        // �����̓G���g���[
        CommInfoSetEntry( CommGetCurrentID() );
        seq = SEQ_OUT;						//�I���V�[�P���X��
    }
    return seq;

}



//------------------------------------------------------------------
/**
 * $brief   �o�g���̏ڍו��������߂� WIFIP2PMATCH_MODE_SUBBATTLE_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _battleSubMenuInit( WIFIP2PMATCH_WORK *wk, int ret )
{
    int i,length;
    BMPLIST_HEADER list_h;
    _infoMenu* pMenu;
    
    switch(ret){
	case WIFI_STSET_SINGLEBATTLE:
        pMenu = _parentSingleInfoMenuList;
        length = NELEMS(_parentSingleInfoMenuList);
        wk->bSingle = 1;
		list_h = _parentInfoBattleMenuListHeader;
		break;

	case WIFI_STSET_DOUBLEBATTLE:
        pMenu = _parentDoubleInfoMenuList;
        length = NELEMS(_parentDoubleInfoMenuList);
        wk->bSingle = 0;
	    list_h = _parentInfoBattleMenuListHeader;
		break;

	case WIFI_STSET_MINIGAME:
        pMenu = _parentMiniGameInfoMenuList;
        length = NELEMS(_parentMiniGameInfoMenuList);
		list_h = _parentInfoMiniGameMenuListHeader;
		wk->bSingle = 2;
		break;
    }

    wk->submenulist = BMP_MENULIST_Create( length , HEAPID_WIFIP2PMATCH );
    for(i=0; i< length ; i++){
		if( pMenu[i].str_id != msg_wifilobby_mg02 ){
	        BMP_MENULIST_AddArchiveString( wk->submenulist, wk->MsgManager, pMenu[i].str_id, pMenu[i].param );
		}else{
			// �~�j�Q�[���̓^�O�ŕ\������i������Ƃނ��肷���E�E�E�j
			WORDSET_RegisterWiFiLobbyGameName( wk->WordSet, 0, i );
			MSGMAN_GetString(  wk->MsgManager, pMenu[i].str_id, wk->pExpStrBuf );
			WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );
			BMP_MENULIST_AddString( wk->submenulist, wk->TitleString, pMenu[i].param );
		}
    }
    if(GF_BGL_BmpWinAddCheck(&wk->SubListWin)){
        BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->SubListWin);
    }
    //BMP�E�B���h�E����
    GF_BGL_BmpWinAdd(wk->bgl,&wk->SubListWin,
                     GF_BGL_FRAME2_M, 16, 9, 15, length * 2, FLD_SYSFONT_PAL, FLD_MENU_WIN_CGX - length * 2);
    BmpMenuWinWrite(&wk->SubListWin, WINDOW_TRANS_OFF, MENU_WIN_CGX_NUM, MENU_WIN_PAL );
    list_h.list = wk->submenulist;
    list_h.win = &wk->SubListWin;
    wk->sublw = BmpListSet(&list_h, 0, wk->singleCur[wk->bSingle], HEAPID_WIFIP2PMATCH);
    GF_BGL_BmpWinOnVReq(&wk->SubListWin);

//    WifiP2PMatchMessagePrint(wk, msg_wifilobby_006, FALSE);

//    wk->seq = WIFIP2PMATCH_MODE_SELECT_WAIT;
    return TRUE;
}



//------------------------------------------------------------------
/**
 * $brief   �ҋ@��ԂɂȂ�ׂ̑I�����j���[ WIFIP2PMATCH_MODE_SUBBATTLE_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _parentModeSubSelectMenuWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    u32	ret;

	WIFI_MCR_PCAnmMain( &wk->matchroom );	// �p�\�R���A�j�����C��

	// �����Őڑ����Ă���\��������	080617	tomoya
	if( 0 !=  _checkParentNewPlayer(wk)){ // �ڑ����Ă���
		wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
		BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
		GF_BGL_BmpWinDel(&wk->SubListWin);
		BmpListExit(wk->sublw, NULL, &wk->singleCur[wk->bSingle]);
		BMP_MENULIST_Delete( wk->submenulist );
		EndMessageWindowOff(wk);
		return seq;
	}

    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
		return seq;
    }
    ret = BmpListMain(wk->sublw);
    switch(ret){
      case BMPLIST_NULL:
        return seq;
      case BMPLIST_CANCEL:
        Snd_SePlay(_SE_DESIDE);
        wk->seq = WIFIP2PMATCH_MODE_SELECT_INIT;
        break;
      default:
        Snd_SePlay(_SE_DESIDE);
        _myStatusChange(wk, ret);
        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
        EndMessageWindowOff(wk);
        break;
    }


	// wifipofin�̎��́Awifi�ʐM��wifi pofin ���[�h�ɂ���
	// �|�t�B���ҍ�����
//	{
	if( wk->seq == WIFIP2PMATCH_MODE_FRIENDLIST ){	// �����I�������Ƃ��̂݁ACANCEL�͂���
		BOOL msg_on = TRUE;
		if( _wait2to4Mode(ret) ){
            wk->seq = WIFIP2PMATCH_PARENT_RESTART;  // �ċN����҂�

			WifiP2PMatch_CommWifiBattleStart( wk, -1, ret );

			_commStateChange(ret);
            _myStatusChange(wk, ret);
            wk->endSeq = _get2to4ModeEndSeq( ret );
			msg_on = FALSE;

        }
        else {
			WifiP2PMatch_CommWifiBattleStart( wk, -1, ret );
            CommStateChangeWiFiBattle();
        }
		// �����~������
		FriendRequestWaitOn( wk, msg_on );
    //    _commStateChange(ret);
	}


	
    BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
    GF_BGL_BmpWinDel(&wk->SubListWin);
    BmpListExit(wk->sublw, NULL, &wk->singleCur[wk->bSingle]);
    BMP_MENULIST_Delete( wk->submenulist );
    return seq;

}

//------------------------------------------------------------------
/**
 * $brief   �}�b�`���O�\������ WIFIP2PMATCH_MODE_MATCH_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

enum{
    _CONNECTING = 1,
    _INFOVIEW,
};


static const struct {
    u32  str_id;
    u32  param;
}_childMatchMenuList[] = {
    { msg_wifilobby_030, (u32)_CONNECTING },
//    { msg_wifilobby_031, (u32)_INFOVIEW },
    { msg_wifilobby_032, (u32)BMPLIST_CANCEL },
};

///�I�����j���[�̃��X�g
static const BMPLIST_HEADER _childMatchMenuListHeader = {
    NULL,			// �\�������f�[�^�|�C���^
    _curCallBack,					// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
    NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
    NULL,                   //
    NELEMS(_childMatchMenuList),	// ���X�g���ڐ�
    NELEMS(_childMatchMenuList),	// �\���ő區�ڐ�
    0,						// ���x���\���w���W
    8,						// ���ڕ\���w���W
    0,						// �J�[�\���\���w���W
    0,						// �\���x���W
    FBMP_COL_BLACK,			// �����F
    FBMP_COL_WHITE,			// �w�i�F
    FBMP_COL_BLK_SDW,		// �����e�F
    0,						// �����Ԋu�w
    16,						// �����Ԋu�x
    BMPLIST_NO_SKIP,		// �y�[�W�X�L�b�v�^�C�v
    FONT_SYSTEM,				// �����w��
    0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
    NULL                    // work
    };


static int _childModeMatchMenuInit( WIFIP2PMATCH_WORK *wk, int seq )
{
	int gmmno;
	u16 friendNo,status;
	_WIFI_MACH_STATUS* p_status;
	MCR_MOVEOBJ* p_player;
	MCR_MOVEOBJ* p_npc;
	u32 way;
	
	friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
	//NPC�������̕����Ɍ�����
	p_player = MCRSYS_GetMoveObjWork( wk, 0 );
	GF_ASSERT( p_player );

	p_npc = MCRSYS_GetMoveObjWork( wk, friendNo );
	// ���肪���Ȃ��Ȃ�����
	// ����̃X�e�[�^�X���Ⴄ���̂ɕς������A
	// �\���������Č��ɖ߂�
	if( p_npc == NULL ){
		_friendNameExpand(wk, friendNo - 1);
		WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
		wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
		return seq;
	}

	way = WIFI_MCR_GetRetWay( p_player );
	WIFI_MCR_NpcPauseOn( &wk->matchroom, p_npc, way );


	// ��ԕۑ�
	p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
	status = _WifiMyStatusGet( wk, p_status );
	wk->keepStatus = status;


    _friendNameExpand(wk, friendNo - 1);
    if(status == WIFI_STATUS_TRADE_WAIT){
        gmmno = msg_wifilobby_004;
    }
    else if(status == WIFI_STATUS_POFIN_WAIT){
        gmmno = msg_wifilobby_141;
    }
    else if(status == WIFI_STATUS_POFIN){
        gmmno = msg_wifilobby_104;
    }
    else if(status == WIFI_STATUS_FRONTIER_WAIT){
        gmmno = msg_wifilobby_137;
    }
    else if(status == WIFI_STATUS_FRONTIER){
        gmmno = msg_wifilobby_140;
    }
#ifdef WFP2P_DEBUG_EXON 
    else if(status == WIFI_STATUS_BATTLEROOM_WAIT){
        gmmno = msg_wifilobby_debug_00;
    }
    else if(status == WIFI_STATUS_BATTLEROOM){
        gmmno = msg_wifilobby_debug_00;
    }
#endif
    else if(status == WIFI_STATUS_BUCKET_WAIT){
		WORDSET_RegisterWiFiLobbyGameName( wk->WordSet, 1, WFLBY_GAME_BALLSLOW );
        gmmno = msg_wifilobby_144;
    }
    else if(status == WIFI_STATUS_BUCKET){
        gmmno = msg_wifilobby_149;
    }
    else if(status == WIFI_STATUS_BALANCEBALL_WAIT){
		WORDSET_RegisterWiFiLobbyGameName( wk->WordSet, 1, WFLBY_GAME_BALANCEBALL );
        gmmno = msg_wifilobby_144;
    }
    else if(status == WIFI_STATUS_BALANCEBALL){
        gmmno = msg_wifilobby_149;
    }
    else if(status == WIFI_STATUS_BALLOON_WAIT){
		WORDSET_RegisterWiFiLobbyGameName( wk->WordSet, 1, WFLBY_GAME_BALLOON );
        gmmno = msg_wifilobby_144;
    }
    else if(status == WIFI_STATUS_BALLOON){
        gmmno = msg_wifilobby_149;
    }
    else if(_modeBattleWait(status)){
        gmmno = msg_wifilobby_003;
    }
    else if(status == WIFI_STATUS_TRADE){
        gmmno = msg_wifilobby_049;
    }
    else if(_modeBattle(status)){
        gmmno = msg_wifilobby_048;
    }
    else if(status == WIFI_STATUS_VCT){
        gmmno = msg_wifilobby_050;
    }
    else{
        gmmno = msg_wifilobby_005;
    }
    WifiP2PMatchMessagePrint(wk, gmmno, FALSE);
    wk->seq = WIFIP2PMATCH_MODE_MATCH_INIT2;
    return seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�̏I�������āA���X�g���o���܂�
 */
//-----------------------------------------------------------------------------
static int _childModeMatchMenuInit2( WIFIP2PMATCH_WORK *wk, int seq )
{
	MCR_MOVEOBJ* p_npc;
	u32 checkMatch;
	int friendNo, status;
	_WIFI_MACH_STATUS* p_status;


	// �G���[�`�F�b�N
    if( CommStateIsWifiError() ){
//        wk->localTime=0;
        _errorDisp(wk);
        return seq;
    }

	// �b�������Ă���F�B�i���o�[�擾
	friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
	p_npc = MCRSYS_GetMoveObjWork( wk, friendNo );

    checkMatch = _checkParentNewPlayer(wk);
    if( 0 !=  checkMatch ){ // �ڑ����Ă���
		// NPC�����ɖ߂�
		WIFI_MCR_NpcPauseOff( &wk->matchroom, p_npc );
        EndMessageWindowOff(wk);
		wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
		return seq;	
    }

	// ���肪���Ȃ��Ȃ�����
	// ����̃X�e�[�^�X���Ⴄ���̂ɕς������A
	// �\���������Č��ɖ߂�
	if( p_npc == NULL ){

		// NPC�����ɖ߂�
		WIFI_MCR_NpcPauseOff( &wk->matchroom, p_npc );

		BmpListExit( wk->sublw, NULL, NULL );
		_friendNameExpand(wk, friendNo - 1);
		WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
		wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
		return seq;
	}else{
		p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
		status = _WifiMyStatusGet( wk, p_status );

		// ��Ԃ����������
		if((wk->keepStatus != status)){

			// NPC�����ɖ߂�
			WIFI_MCR_NpcPauseOff( &wk->matchroom, p_npc );

			_friendNameExpand(wk, friendNo - 1);
			WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
			wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
			return seq;
		}
	}
	
	
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }

	{
		int i,length;
		BMPLIST_HEADER list_h;
		u16 friendNo,status,vchat;
		_WIFI_MACH_STATUS* p_status;

		length = NELEMS(_childMatchMenuList);
		list_h = _childMatchMenuListHeader;
		friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
		p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
		status = _WifiMyStatusGet( wk, p_status );
		vchat = p_status->vchat;
		wk->keepVChat = vchat;
		vchat += wk->pMatch->myMatchStatus.vchat;

		// ���̐l�̏���\��
	//	WifiP2PMatch_UserDispOn_MyAcces( wk, friendNo, HEAPID_WIFIP2PMATCH );


		wk->submenulist = BMP_MENULIST_Create( length , HEAPID_WIFIP2PMATCH );
		i = 0;

		for(i = 0; i < NELEMS(_childMatchMenuList);i++){
			if(i == 0){
				if(_modeActive(status) || (status == WIFI_STATUS_NONE) ||
					(status == WIFI_STATUS_PLAY_OTHER) || 
				   (status >= WIFI_STATUS_UNKNOWN)){  // ���������ނ��o���Ȃ�����
					list_h.line -= 1;
					list_h.count -= 1;
					length -= 1;
				}
				else if(status == WIFI_STATUS_LOGIN_WAIT){
					if(vchat == 2){
						BMP_MENULIST_AddArchiveString( wk->submenulist, wk->MsgManager,
													   msg_wifilobby_027,
													   _childMatchMenuList[i].param );
					}
					else{
						list_h.line -= 1;
						list_h.count -= 1;
						length -= 1;
					}
				}
				else{
					BMP_MENULIST_AddArchiveString( wk->submenulist, wk->MsgManager,
												   _childMatchMenuList[i].str_id,
												   _childMatchMenuList[i].param );
				}
			}
			else{
				BMP_MENULIST_AddArchiveString( wk->submenulist, wk->MsgManager,
											   _childMatchMenuList[i].str_id,
											   _childMatchMenuList[i].param );
			}
		}
		//BMP�E�B���h�E����
		GF_BGL_BmpWinAdd(wk->bgl,&wk->SubListWin,
						 GF_BGL_FRAME2_M, 16, 11+ ((3-length)*2), 15  , length * 2,
						 FLD_SYSFONT_PAL, FLD_MENU_WIN_CGX);
		BmpMenuWinWrite(&wk->SubListWin, WINDOW_TRANS_OFF, MENU_WIN_CGX_NUM, MENU_WIN_PAL );
		list_h.list = wk->submenulist;
		list_h.win = &wk->SubListWin;
		wk->sublw = BmpListSet(&list_h, 0, 0, HEAPID_WIFIP2PMATCH);
		GF_BGL_BmpWinOnVReq(&wk->SubListWin);
	}

    wk->seq = WIFIP2PMATCH_MODE_MATCH_WAIT;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ���m�̃A�C�e������
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static BOOL _isItemCheck(_WIFI_MACH_STATUS* pTargetStatus)
{
    int i;
    
    for(i = 0; i < _POKEMON_NUM; i++){
        if(MONSNO_DAMETAMAGO == pTargetStatus->pokemonType[i]){
            return FALSE;
        }
        if(MONSNO_MAX < pTargetStatus->pokemonType[i]){
            return FALSE;
        }
        if(ITEM_DATA_MAX < pTargetStatus->hasItemType[i]){
            return FALSE;
        }
    }
    return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�|�t�B���A�C�e���`�F�b�N
 *
 *	@param	wk		�V�X�e�����[�N
 *
 *	@retval	WFP2P_POFIN_RET_OK,			���v
 *	@retval	WFP2P_POFIN_RET_NUTSNONE,	�؂̎����Ȃ�
 *	@retval	WFP2P_POFIN_RET_POFINMAX,	�|�t�B���P�[�X�������ς�
 */
//-----------------------------------------------------------------------------
static u32 _isPofinItemCheck(WIFIP2PMATCH_WORK* wk)
{
	//�؂̎��������Ă��邩�`�F�b�N
	if(!MyItem_CheckItemPocket(SaveData_GetMyItem(wk->pSaveData),BAG_POKE_NUTS)){
		return WFP2P_POFIN_RET_NUTSNONE;
	}
	// �|���g�P�[�X�����邩�`�F�b�N
	if( !MyItem_GetItemNum( SaveData_GetMyItem(wk->pSaveData),ITEM_POFINKEESU,HEAPID_WIFIP2PMATCH ) ){
		return WFP2P_POFIN_RET_POFINCASENONE;
	}
	//�|���g�P�[�X�������ς��łȂ����`�F�b�N
	if(PORUTO_GetDataNum(SaveData_GetPorutoBlock(wk->pSaveData)) >= PORUTO_STOCK_MAX){
		return WFP2P_POFIN_RET_POFINMAX;
	}
	return WFP2P_POFIN_RET_OK;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�p��BGM��ݒ肷��
 */
//-----------------------------------------------------------------------------
static void WifiP2P_SetLobbyBgm( void )
{
	u32 bgm;

	// �{�C�X�`���b�g�Ȃ��ɂ���
	mydwc_setVChat(FALSE);		// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi

	if( WifiP2P_CheckLobbyBgm() == FALSE ){

		if( GF_RTC_IsNightTime() == FALSE ){			//FALSE=���ATRUE=��
			bgm = SEQ_PC_01;
		}else{
			bgm = SEQ_PC_02;
		}
		Snd_SceneSet( SND_SCENE_DUMMY );
		Snd_DataSetByScene( SND_SCENE_P2P, bgm, 1 );	//wifi���r�[�Đ�
	}else{

		// �����a�f�l����Ă��Ă��A���ʂ������ɖ߂�
		Snd_PlayerSetInitialVolumeBySeqNo( Snd_NowBgmNoGet(), BGM_WIFILOBBY_VOL );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�p��BGM���Đ������擾����
 *
 *	@retval	TRUE	�Đ���
 *	@retval	FALSE	����BGM���Đ���
 */
//-----------------------------------------------------------------------------
static BOOL WifiP2P_CheckLobbyBgm( void )
{
	u32 now_bgm;

	now_bgm = Snd_NowBgmNoGet();

	if( (now_bgm != SEQ_PC_01) &&
		(now_bgm != SEQ_PC_02) ){
		return FALSE;
	}
	return TRUE;
}

	

//----------------------------------------------------------------------------
/**
 *	@brief	�{�E�P���m�[�g�f�[�^�ݒ�	�F�B�ƗV��
 *
 *	@param	wk	���[�N
 *	@param	idx	�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WifiP2P_Fnote_Set( WIFIP2PMATCH_WORK* wk, u32 idx )
{
	void* p_buf;
	FNOTE_DATA* p_fnote;

	p_fnote		= SaveData_GetFNote( wk->pSaveData );
	p_buf = FNOTE_SioIDOnlyDataMake( HEAPID_WIFIP2PMATCH, FNOTE_ID_PL_WIFICLUB );
	FNOTE_DataSave( p_fnote, p_buf, FNOTE_TYPE_SIO );
}

//------------------------------------------------------------------
/**
 * $brief   �Ȃ��ɍs���I�����j���[ WIFIP2PMATCH_MODE_MATCH_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _childModeMatchMenuWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    u32	ret;
    int status,friendNo,message = 0,vchat;
    u16 mainCursor;
    int checkMatch;
	MCR_MOVEOBJ* p_npc;
	_WIFI_MACH_STATUS* p_status;

    ret = BmpListMain(wk->sublw);

    checkMatch = _checkParentNewPlayer(wk);
    if( 0 !=  checkMatch ){ // �ڑ����Ă���
        ret = BMPLIST_CANCEL;
    }

	// �b�������Ă���F�B�i���o�[�擾
	friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
	p_npc = MCRSYS_GetMoveObjWork( wk, friendNo );


	// �G�[���[�`�F�b�N
    if(CommStateIsWifiError()){
		BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
		GF_BGL_BmpWinDel(&wk->SubListWin);
		BmpListExit(wk->sublw, NULL, NULL);
		BMP_MENULIST_Delete( wk->submenulist );

		if( p_npc != NULL ){
			WIFI_MCR_NpcPauseOff( &wk->matchroom, p_npc );
		}

        _errorDisp(wk);
		return seq;
    }
	

    switch(ret){
      case BMPLIST_NULL:

		// ���肪���Ȃ��Ȃ�����
		// ����̃X�e�[�^�X���Ⴄ���̂ɕς������A
		// �\���������Č��ɖ߂�
		if( p_npc == NULL ){
			BmpListExit( wk->sublw, NULL, NULL );
			_friendNameExpand(wk, friendNo - 1);
			WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
			wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
			message = 1;
			break;
		}else{
			p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
			status = _WifiMyStatusGet( wk, p_status );
			vchat = p_status->vchat;

			// ��Ԃ����������
			// 4�l��W�̃Q�[������Ȃ��̂ɁAVCHAT�t���O���ς������ؒf
			if((wk->keepStatus != status) || 
				(( _wait2to4Mode(status) == FALSE ) && (wk->keepVChat != vchat)) ){

				// NPC�����ɖ߂�
				// �����ł���Ă��܂���PauseOff���Q�d�ł��ł��܂�
//				WIFI_MCR_NpcPauseOff( &wk->matchroom, p_npc );

				_friendNameExpand(wk, friendNo - 1);
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
				wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
				message = 1;
				break;
			}
		}
        return seq;

      case BMPLIST_CANCEL:
        Snd_SePlay(_SE_DESIDE);

        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
        break;
      default:
        Snd_SePlay(_SE_DESIDE);
        if(ret == _CONNECTING){  // �q�@���e�@�ɐڑ�
            wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
//            BmpListDirectPosGet(wk->lw,&mainCursor);
//		    friendNo = wk->index2No[mainCursor];
            wk->friendNo = friendNo;
            if(  friendNo != 0 ){
                int num = _getBattlePokeNum(wk);
				int pofin_check = _isPofinItemCheck( wk );

				p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
                status = _WifiMyStatusGet( wk, p_status );
                vchat = p_status->vchat;


				// ��Ԃ����������
				// 4�l��W�̃Q�[������Ȃ��̂ɁAVCHAT�t���O���ς������ؒf
				if((wk->keepStatus != status) || 
					(( _wait2to4Mode(status) == FALSE ) && (wk->keepVChat != vchat)) ){
                    _friendNameExpand(wk, friendNo - 1);
                    WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
                    wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
                    message = 1;
                    break;
                }
				// �s���A�C�e���`�F�b�N
                if(!_isItemCheck(p_status)){
                    _friendNameExpand(wk, friendNo - 1);
                    WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
                    wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
                    message = 1;
                    break;
                }
				// �����X�^�[�Q�΃`�F�b�N
                if(_is2pokeMode(status) && (2 > num)){
                    if(WIFI_STATUS_TRADE_WAIT==status){
                        WifiP2PMatchMessagePrint(wk, msg_wifilobby_053, FALSE);
                    }
                    else{
                        WifiP2PMatchMessagePrint(wk, msg_wifilobby_067, FALSE);
                    }
                    wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
                    message = 1;
                    break;
                }
				// �|�t�B���P�[�X�`�F�b�N
				if( (status == WIFI_STATUS_POFIN_WAIT) && (pofin_check != WFP2P_POFIN_RET_OK) ){
					if( pofin_check == WFP2P_POFIN_RET_NUTSNONE ){
                        WifiP2PMatchMessagePrint(wk, msg_wifilobby_105, FALSE);
					}else if( pofin_check == WFP2P_POFIN_RET_POFINCASENONE ){
                        WifiP2PMatchMessagePrint(wk, msg_wifilobby_121, FALSE);
					}else{
                        WifiP2PMatchMessagePrint(wk, msg_wifilobby_106, FALSE);
					}
                    wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
                    message = 1;
					break;
				}
				// VCT���[�h�`�F�b�N
				// 2�`4�l�}�b�`���O�̏ꍇVCT���[�h�����킹��K�v�͂Ȃ�
				if( _wait2to4Mode( status ) == FALSE ){
					
					if(vchat != wk->pMatch->myMatchStatus.vchat){  // ������VCHAT�Ə�Ԃ��Ⴄ�ꍇ���b�Z�[�W
						if(vchat){
							WifiP2PMatchMessagePrint(wk, msg_wifilobby_069, FALSE);
						}
						else{
							WifiP2PMatchMessagePrint(wk, msg_wifilobby_070, FALSE);
						}

						// 080703	tomoya takahashi
						// VCHAT��ON�EOFF�𕷂��ӏ��ɂ�������
						// �������咆�Ƃ������Ƃɂ��āA
						// �R�҂̉��傪�ł��Ȃ��悤�ɂ���
                        _myStatusChange(wk, _convertState(status));  // 
						
						wk->seq = WIFIP2PMATCH_MODE_VCHAT_NEGO;
						message = 1;
						break;
					}
				}
                status = _convertState(status);
                if(WIFI_STATUS_UNKNOWN == status){
                    break;
                }
                if( WifiDwc_getFriendStatus(friendNo - 1) == DWC_STATUS_MATCH_SC_SV ){
					TOMOYA_PRINT( "wifi�ڑ��� %d\n", friendNo - 1 );

                    if( WifiP2PMatch_CommWifiBattleStart( wk, friendNo - 1, status ) ){
                        wk->cancelEnableTimer = _CANCELENABLE_TIMER;
						_commStateChange(status);
                        _myStatusChange(wk, status);  // �ڑ����ɂȂ�
                        _friendNameExpand(wk, friendNo - 1);
                        WifiP2PMatchMessagePrint(wk,msg_wifilobby_014, FALSE);
						GF_ASSERT( wk->timeWaitWork == NULL );
                        wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
                        if(status != WIFI_STATUS_VCT){
                            wk->seq = WIFIP2PMATCH_MODE_MATCH_LOOP;  // �}�b�`���O��
                        }
                        else{
                            wk->cancelEnableTimer = _CANCELENABLE_TIMER;
                            wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT_INIT2;  //VCT��p��
                        }
                        message = 1;
                    }else{
						_friendNameExpand(wk, friendNo - 1);
						WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
						wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
						message = 1;
					}
                }else{

					_friendNameExpand(wk, friendNo - 1);
					WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
					wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
					message = 1;
					break;
				}
            }
        }
        else if(ret == _INFOVIEW){
            wk->seq = WIFIP2PMATCH_MODE_PERSONAL_INIT;
        }
        break;
    }


	// ��n��
	
    if(message==0){
        EndMessageWindowOff(wk);
    }
    BmpMenuWinClear(&wk->SubListWin, WINDOW_TRANS_ON );
    GF_BGL_BmpWinDel(&wk->SubListWin);
    BmpListExit(wk->sublw, NULL, NULL);
    BMP_MENULIST_Delete( wk->submenulist );

	// NPC�����ɖ߂�
	if( p_npc != NULL ){
		WIFI_MCR_NpcPauseOff( &wk->matchroom, p_npc );
	}

	// �߂邾���Ȃ�l�̏�������
	if( wk->seq == WIFIP2PMATCH_MODE_FRIENDLIST ){
		
		// ���̐l�̏�������
//		WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );
		
	}else{

		// �q����ɂ����Ƃ�

	}

    return seq;
}


//WIFIP2PMATCH_MODE_MATCH_LOOP
static int _childModeMatchMenuLoop( WIFIP2PMATCH_WORK *wk, int seq )
{
	int status;
	

    wk->cancelEnableTimer--;
    if(wk->cancelEnableTimer < 0  ){
        wk->seq = WIFIP2PMATCH_MODE_CANCEL_ENABLE_WAIT;
    }
    else if(CommWifiIsMatched() == 3 ){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommWifiIsMatched() == 5){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_012, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if((CommWifiIsMatched() == 4) || (CommStateIsWifiDisconnect())){
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommStateIsWifiError()){
        _errorDisp(wk);
    }
    else if(CommWifiIsMatched()==1){  // ����ɐڑ�����

		// �Q�`�S�l��W�łȂ��Ƃ�
		status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );
		if( _wait2to4Mode( status ) == FALSE ){
	        
			wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO; // �q�@�e�@����
		//    _timeWaitIconDel(wk);		timeWait����MsgWin��j�����Ă���Ƃ������Ƃ̓��b�Z�[�W�I���ł�OK��
			EndMessageWindowOff(wk);
			CommInfoInitialize(wk->pSaveData,NULL);   //Info������
			wk->timer = 30;
			
		}else if( CommIsConnect(COMM_PARENT_ID) == TRUE ){

			// ���b�Z�[�W��~
			EndMessageWindowOff( wk );

			CommInfoInitialize(wk->pSaveData,NULL);   //Info������

	
			// �ʐM�G���[�`�F�b�N
			// 080630 tomoya takahashi
			CommStateSetErrorCheck(FALSE,TRUE);	// �ؒf�̓G���[����Ȃ�


			// �Q�`�S�l��W�̎�
			// �|�t�B���Ȃ�ʐM�������|�t�B���p�ɂ���
            _commStateChange(status);
            wk->endSeq = _get2to4ModeEndSeq( status );
			CommSetWifiBothNet(FALSE); // wifi�̒ʐM�𓯊�����񓯊���
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
							COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);
            wk->bRetryBattle = FALSE;
			seq = SEQ_OUT;						//�I���V�[�P���X��
		}else{

			// �ʐM�o�O:324	
			// Matched()��1���A���Ă��āACommIsConnect�ł�FALSE���A���Ă��Ă���
			// WiFiP2P�ʐM���\�ȏ�ԂȂ̂ɐe�Ɛڑ����Ă��Ȃ��Ȃ�G���[������
			if( CommGetWifiConnect() == TRUE ){
				_friendNameExpand(wk, mydwc_getFriendIndex());
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
				wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
			}
		}
    }
    return seq;
}

//-------------------------------------VCT�̃L�����Z���@�\���}篎���
//-------------------------------------VCT�̃L�����Z���@�\���}篎��O��

//------------------------------------------------------------------
/**
 * $brief   B�L�����Z��  WIFIP2PMATCH_MODE_BCANCEL_YESNO_VCT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_BCancelYesNoVCT( WIFIP2PMATCH_WORK *wk, int seq )
{
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   B�L�����Z��  WIFIP2PMATCH_MODE_BCANCEL_WAIT_VCT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_BCancelWaitVCT( WIFIP2PMATCH_WORK *wk, int seq )
{
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   DWC�ؒf WIFIP2PMATCH_MODE_CANCEL_ENABLE_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_CancelEnableWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    _myVChatStatusOrgSet(wk);
    _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
    CommStateWifiMatchEnd();
    wk->preConnect = -1;
    wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
    // ��l���̓��������
    FriendRequestWaitOff( wk );
    EndMessageWindowOff(wk);
    return seq;
}


//------------------------------------------

//------------------------------------------------------------------
/**
 * $brief   B�L�����Z��  WIFIP2PMATCH_MODE_BCANCEL_YESNO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_BCancelYesNo( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT
{
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   B�L�����Z��  WIFIP2PMATCH_MODE_BCANCEL_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_BCancelWait( WIFIP2PMATCH_WORK *wk, int seq )        // WIFIP2PMATCH_MODE_VCT_CONNECT
{
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   �e�@��t�܂��B�q�@���牞�傪���������Ƃ�m�点��
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _parentModeCallMenuInit( WIFIP2PMATCH_WORK *wk, int seq )
{
    int mySt;
    int targetSt;
	_WIFI_MACH_STATUS* p_status;
	int myvchat;

	p_status = WifiFriendMatchStatusGet( wk, mydwc_getFriendIndex() );
    mySt = _WifiMyStatusGet( wk,&wk->pMatch->myMatchStatus );
    targetSt = _WifiMyStatusGet( wk,p_status );
	myvchat	= wk->pMatch->myMatchStatus.vchat;
    
    if((mySt == WIFI_STATUS_DBATTLE50_WAIT)&&(targetSt == WIFI_STATUS_DBATTLE50)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_DBATTLE100_WAIT)&&(targetSt == WIFI_STATUS_DBATTLE100)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_DBATTLE_FREE_WAIT)&&(targetSt == WIFI_STATUS_DBATTLE_FREE)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
#ifdef WFP2P_DEBUG_EXON 
    else if((mySt == WIFI_STATUS_MBATTLE_FREE_WAIT)&&(targetSt == WIFI_STATUS_MBATTLE_FREE)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
#endif
    else if((mySt == WIFI_STATUS_SBATTLE50_WAIT)&&(targetSt == WIFI_STATUS_SBATTLE50)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_SBATTLE100_WAIT)&&(targetSt == WIFI_STATUS_SBATTLE100)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_SBATTLE_FREE_WAIT)&&(targetSt == WIFI_STATUS_SBATTLE_FREE)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_TRADE_WAIT)&&(targetSt == WIFI_STATUS_TRADE)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_POFIN_WAIT)&&(targetSt == WIFI_STATUS_POFIN)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_FRONTIER_WAIT)&&(targetSt == WIFI_STATUS_FRONTIER)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
#ifdef WFP2P_DEBUG_EXON 
    else if((mySt == WIFI_STATUS_BATTLEROOM_WAIT)&&(targetSt == WIFI_STATUS_BATTLEROOM)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
#endif
    else if((mySt == WIFI_STATUS_BUCKET_WAIT)&&(targetSt == WIFI_STATUS_BUCKET)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_BALANCEBALL_WAIT)&&(targetSt == WIFI_STATUS_BALANCEBALL)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_BALLOON_WAIT)&&(targetSt == WIFI_STATUS_BALLOON)){
        wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;  //ok
    }
    else if((mySt == WIFI_STATUS_LOGIN_WAIT)&&(targetSt == WIFI_STATUS_VCT)&&(myvchat > 0)){	// 080703 tomoya VCHAT��ON�ł��邱�Ƃ������ɒǉ�
        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT_INIT; //o
        return seq;
    }
    else{
        // �K�����Ȃ�=>�ؒf������
        _friendNameExpand(wk, mydwc_getFriendIndex());
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);
        CommStateWifiMatchEnd();
        return seq;
    }
    _friendNameExpand(wk, mydwc_getFriendIndex());
    WifiP2PMatchMessagePrint(wk, msg_wifilobby_008, FALSE);
    CommInfoInitialize(wk->pSaveData,NULL);   //Info������
    wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;
    wk->timer = 30;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �R�l�N�V�����͂낤�Ƃ��Ă�����Ԓ��̃G���[�\��
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static BOOL _connectingErrFunc(WIFIP2PMATCH_WORK *wk)
{
    if(CommWifiIsMatched() >= 3){
        OS_TPrintf("_connectingErrFunc%d \n",CommWifiIsMatched());
        _friendNameExpand(wk, wk->preConnect);
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(CommStateIsWifiError()){
        _errorDisp(wk);
    }
    else{

#if PL_T0857_080711_FIX
		// �F�B��STATUS��VCHAT���`�F�b�N	����Ă�����ؒf
		{
			int mySt;
			int targetSt, targetSt_org;
			_WIFI_MACH_STATUS* p_status;
			int myvchat, targetvchat;

			p_status = WifiFriendMatchStatusGet( wk, mydwc_getFriendIndex() );
			mySt		= _WifiMyStatusGet( wk,&wk->pMatch->myMatchStatus );
			targetSt_org= _WifiMyStatusGet( wk,p_status );
			targetSt	= _convertState(targetSt_org);
			myvchat		= wk->pMatch->myMatchStatus.vchat;
			targetvchat	= p_status->vchat;

			OS_TPrintf( "check mystart=%d tastatus=%d tastatus_org=%d myvchat=%d tavchat=%d \n", 
					mySt, targetSt, targetSt_org, myvchat, targetvchat );

			if( ((mySt != targetSt) && (mySt != targetSt_org)) ||
				(myvchat != targetSt) ){
				_friendNameExpand(wk, mydwc_getFriendIndex());
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
				wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;

			}else{

				return FALSE;
			}
		}
#else
        return FALSE;
#endif
    }
    wk->bRetryBattle = FALSE;
    return TRUE;
}

//------------------------------------------------------------------
/**
 * $brief   �Ȃ���ׂ��X�e�[�g�m�F  WIFIP2PMATCH_MODE_CALL_YESNO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _parentModeCallMenuYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
    if((CommWifiIsMatched() >= 4) || CommStateIsWifiDisconnect() || !CommIsConnect(COMM_PARENT_ID)){
        OS_TPrintf("%d %d %d\n",CommWifiIsMatched(),CommStateIsWifiDisconnect(),CommIsConnect(COMM_PARENT_ID));
        
        if(wk->bRetryBattle){
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_065, FALSE);
        }
        else{
            _friendNameExpand(wk, wk->preConnect);
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);
        }
        wk->bRetryBattle = FALSE;
        wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
    }
    else if(_connectingErrFunc(wk)){
    }
    else{

		
        if(wk->timer==0){
            if(CommIsTimingSync(_TIMING_GAME_CHECK)){
                CommToolTempDataReset();
                CommTimingSyncStart(_TIMING_GAME_CHECK2);
                wk->bRetryBattle = FALSE;
                wk->seq = WIFIP2PMATCH_MODE_CALL_SEND;
            }
        }
        else{
            wk->timer--;
            if(wk->timer == 0){
                CommTimingSyncStart(_TIMING_GAME_CHECK);
            }
        }
    }
    return seq;
}

//
//------------------------------------------------------------------
/**
 * $brief   �Ȃ���ׂ��X�e�[�g�𑗐M  WIFIP2PMATCH_MODE_CALL_SEND
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _parentModeCallMenuSend( WIFIP2PMATCH_WORK *wk, int seq )
{
    if(_connectingErrFunc(wk)){
    }
    else if(CommIsTimingSync(_TIMING_GAME_CHECK2)){
        u16 status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );
		BOOL result;
		
        result = CommToolSetTempData(CommGetCurrentID() ,&status);
		if( result ){
	        wk->seq = WIFIP2PMATCH_MODE_CALL_CHECK;
		}
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �Ȃ���ׂ��X�e�[�g���  WIFIP2PMATCH_MODE_CALL_CHECK
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _parentModeCallMenuCheck( WIFIP2PMATCH_WORK *wk, int seq )
{
    const u16* pData;
    int id=0;
    if(CommGetCurrentID() == COMM_PARENT_ID){
        id = 1;
    }
    else{
        id = COMM_PARENT_ID;
    }
    pData = CommToolGetTempData(id);
    if(_connectingErrFunc(wk)){
    }
    else if(pData!=NULL){
        u16 org_status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );
		u16 status = _convertState(org_status);
        if((pData[0] == status) || (pData[0] == org_status)){
            CommTimingSyncStart(_TIMING_GAME_START);
            wk->seq = WIFIP2PMATCH_MODE_MYSTATUS_WAIT;
        }
        else{  // �قȂ�X�e�[�g��I�������ꍇ
            _friendNameExpand(wk, mydwc_getFriendIndex());
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
            wk->seq = WIFIP2PMATCH_MODE_VCT_DISCONNECT;
        }
    }
    return seq;
}



//------------------------------------------------------------------
/**
 * $brief   �^�C�~���O�R�}���h�҂� WIFIP2PMATCH_MODE_MYSTATUS_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _parentModeCallMyStatusWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int status;

    if(_connectingErrFunc(wk)){
    }
    else if(CommIsTimingSync(_TIMING_GAME_START)){
        CommInfoSendPokeData();
        CommStateSetErrorCheck(TRUE,TRUE);
        CommTimingSyncStart(_TIMING_GAME_START2);
        wk->seq = WIFIP2PMATCH_MODE_CALL_WAIT;
    }
    return seq;
}


static BOOL _parent_MsgEndCheck( WIFIP2PMATCH_WORK *wk )
{
	if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
		return TRUE;
	}

	if( GF_BGL_BmpWinAddCheck( &wk->MsgWin ) == FALSE ){
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------
/**
 * $brief   �^�C�~���O�R�}���h�҂� WIFIP2PMATCH_MODE_CALL_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static int _parentModeCallMenuWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int status;
    int checkFriend[COMM_MACHINE_MAX];
	_WIFI_MACH_STATUS* p_status;

    if(_connectingErrFunc(wk)){
    }
//    else if(CommIsTimingSync(_TIMING_GAME_START2)){
    else if(CommIsTimingSync(_TIMING_GAME_START2) && (_parent_MsgEndCheck( wk ) == TRUE) ){		// ���b�Z�[�W�̏I�����҂悤�ɕύX 08.06.01	tomoya
        dwc_friendAutoInputCheck(wk->pSaveData, checkFriend, HEAPID_WIFIP2PMATCH);
        
        EndMessageWindowOff(wk);

		p_status = WifiFriendMatchStatusGet( wk, mydwc_getFriendIndex() );
        status = _WifiMyStatusGet( wk, p_status );
        if((status == WIFI_STATUS_TRADE_WAIT) || (status == WIFI_STATUS_TRADE)){
            status = WIFI_STATUS_TRADE;
            wk->endSeq = WIFI_P2PMATCH_TRADE;
        }
        else if((status == WIFI_STATUS_POFIN_WAIT) || (status == WIFI_STATUS_POFIN)){
			status = WIFI_STATUS_POFIN;
			wk->endSeq = WIFI_P2PMATCH_POFIN;
        }
        else if((status == WIFI_STATUS_FRONTIER_WAIT) || (status == WIFI_STATUS_FRONTIER)){
			status = WIFI_STATUS_FRONTIER;
			wk->endSeq = WIFI_P2PMATCH_FRONTIER;
        }
#ifdef WFP2P_DEBUG_EXON 
        else if((status == WIFI_STATUS_BATTLEROOM_WAIT) || (status == WIFI_STATUS_BATTLEROOM)){
            status = WIFI_STATUS_BATTLEROOM;
            wk->endSeq = WIFI_P2PMATCH_BATTLEROOM;
        }
#endif
        else if((status == WIFI_STATUS_BUCKET_WAIT) || (status == WIFI_STATUS_BUCKET)){
            status = WIFI_STATUS_BUCKET;
            wk->endSeq = WIFI_P2PMATCH_BUCKET;
        }
        else if((status == WIFI_STATUS_BALANCEBALL_WAIT) || (status == WIFI_STATUS_BALANCEBALL)){
            status = WIFI_STATUS_BALANCEBALL;
            wk->endSeq = WIFI_P2PMATCH_BALANCEBALL;
        }
        else if((status == WIFI_STATUS_BALLOON_WAIT) || (status == WIFI_STATUS_BALLOON)){
            status = WIFI_STATUS_BALLOON;
            wk->endSeq = WIFI_P2PMATCH_BALLOON;
        }
        else if((status == WIFI_STATUS_SBATTLE50_WAIT) || (status == WIFI_STATUS_SBATTLE50)){
            status = WIFI_STATUS_SBATTLE50;
            wk->endSeq = WIFI_P2PMATCH_SBATTLE50;
        }
        else if((status == WIFI_STATUS_SBATTLE100_WAIT) || (status == WIFI_STATUS_SBATTLE100)){
            status = WIFI_STATUS_SBATTLE100;
            wk->endSeq = WIFI_P2PMATCH_SBATTLE100;
        }
        else if((status == WIFI_STATUS_SBATTLE_FREE_WAIT) || (status == WIFI_STATUS_SBATTLE_FREE)){
            status = WIFI_STATUS_SBATTLE_FREE;
            wk->endSeq = WIFI_P2PMATCH_SBATTLE_FREE;
        }
        else if((status == WIFI_STATUS_DBATTLE50_WAIT) || (status == WIFI_STATUS_DBATTLE50)){
            status = WIFI_STATUS_DBATTLE50;
            wk->endSeq = WIFI_P2PMATCH_DBATTLE50;
        }
        else if((status == WIFI_STATUS_DBATTLE100_WAIT) || (status == WIFI_STATUS_DBATTLE100)){
            status = WIFI_STATUS_DBATTLE100;
            wk->endSeq = WIFI_P2PMATCH_DBATTLE100;
        }
        else if((status == WIFI_STATUS_DBATTLE_FREE_WAIT) || (status == WIFI_STATUS_DBATTLE_FREE)){
            status = WIFI_STATUS_DBATTLE_FREE;
            wk->endSeq = WIFI_P2PMATCH_DBATTLE_FREE;
        }
#ifdef WFP2P_DEBUG_EXON 
		else if((status == WIFI_STATUS_MBATTLE_FREE_WAIT) || (status == WIFI_STATUS_MBATTLE_FREE)){
            status = WIFI_STATUS_MBATTLE_FREE;
            wk->endSeq = WIFI_P2PMATCH_MBATTLE_FREE;
        }
#endif

// WIFI�@�ΐ�AUTOӰ�ރf�o�b�N
#ifdef _WIFI_DEBUG_TUUSHIN
		WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_A_REQ = FALSE;
		WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_TOUCH_REQ = FALSE;
#endif	//_WIFI_DEBUG_TUUSHIN

		// �{�E�P���m�[�g
		WifiP2P_Fnote_Set( wk, mydwc_getFriendIndex() );

        _myStatusChange(wk, status);  // �ڑ����ɂȂ�
        WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);

		
        seq = SEQ_OUT;						//�I���V�[�P���X��
    }
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   ���C�����j���[�̖߂�ۂ̏�����
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------
static int WifiP2PMatch_MainReturn( WIFIP2PMATCH_WORK *wk, int seq )
{
    GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME3_M);
    wk->mainCur = 0;
    wk->seq = WifiP2PMatchFriendListStart();
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �l�f�[�^�\�������� WIFIP2PMATCH_MODE_PERSONAL_INIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _personalDataInit( WIFIP2PMATCH_WORK *wk, int seq )
{
    u16 mainCursor, friendNo;
    int num,length,x,width;

#if 0
    GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME3_M);
    ArcUtil_ScrnSet(   ARC_WIFIP2PMATCH_GRA, NARC_wifip2pmatch_conect_03_NSCR, wk->bgl,
                       GF_BGL_FRAME1_M, 0, 0, 0, HEAPID_WIFIP2PMATCH);

#if 0
    CLACT_SetDrawFlag( wk->MainActWork[_CLACT_DOWN_CUR], 0 );
    CLACT_SetDrawFlag( wk->MainActWork[_CLACT_LINE_CUR], 0 );
    CLACT_SetDrawFlag( wk->MainActWork[_CLACT_UP_CUR], 0 );
#endif
	
    if(GF_BGL_BmpWinAddCheck(&wk->MyWin)){
        BmpMenuWinClear(&wk->MyWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->MyWin);
    }
    GF_BGL_BmpWinAdd(wk->bgl,&wk->MyWin,
                     GF_BGL_FRAME3_M, 3, 2, 0x1a, 14, FLD_SYSFONT_PAL, 1);
    width = ( 0x1a * 8 )-2;
    GF_BGL_BmpWinDataFill( &wk->MyWin, 0x8000 );

//    BmpListDirectPosGet(wk->lw,&mainCursor);
//    friendNo = wk->index2No[mainCursor] - 1;
	friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
	friendNo --;
    OHNO_PRINT("�t�����h�ԍ�  %d\n", friendNo);
    _friendNameExpand(wk, friendNo);
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_033, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );
    GF_STR_PrintColor( &wk->MyWin, FONT_TALK, wk->TitleString, 0, 0, MSG_NO_PUT, _COL_N_BLACK, NULL);
        GF_BGL_BmpWinOnVReq(&wk->MyWin);

    WifiP2PMatchMessagePrint(wk, msg_wifilobby_042, FALSE);


    {
        MYSTATUS* pTarget = MyStatus_AllocWork(HEAPID_WIFIP2PMATCH);
        MyStatus_SetMyName(pTarget, WifiList_GetFriendGroupNamePtr(wk->pList,friendNo));
        WORDSET_RegisterPlayerName( wk->WordSet, 0, pTarget);
        sys_FreeMemoryEz(pTarget);
    }
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_034, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, 0, 8*2, MSG_NO_PUT, _COL_N_BLACK, NULL);

    // �ΐ퐬��
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_035, wk->TitleString );
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, 0, 8*4, MSG_NO_PUT, _COL_N_BLACK, NULL);
    // ����
    num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_BATTLE_WIN);
    WORDSET_RegisterNumber(wk->WordSet, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_036, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, 30, 8*6, MSG_NO_PUT, _COL_N_BLACK, NULL);
    // �܂�
    num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_BATTLE_LOSE);
    WORDSET_RegisterNumber(wk->WordSet, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_037, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );

    length = FontProc_GetPrintStrWidth( FONT_SYSTEM, wk->TitleString, 0 );
    x      = width - length;
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, x, 8*6, MSG_NO_PUT, _COL_N_BLACK, NULL);
    // �|�P��������
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_038, wk->TitleString );
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, 0,  8*8, MSG_NO_PUT, _COL_N_BLACK, NULL);

    num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_TRADE_NUM);
    WORDSET_RegisterNumber(wk->WordSet, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_039, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );

    length = FontProc_GetPrintStrWidth( FONT_SYSTEM, wk->TitleString, 0 );
    x      = width - length;
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, x, 8*8, MSG_NO_PUT, _COL_N_BLACK, NULL);
    //�Ō�̓��t
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_040, wk->TitleString );
    GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, 0, 8*10, MSG_NO_PUT, _COL_N_BLACK, NULL);

    num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_LASTBT_DAY);
    if(num!=0){
        WORDSET_RegisterNumber(wk->WordSet, 2, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
        num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_LASTBT_YEAR);
        WORDSET_RegisterNumber(wk->WordSet, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
        num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_LASTBT_MONTH);
        WORDSET_RegisterNumber(wk->WordSet, 1, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
        MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_041, wk->pExpStrBuf );
        WORDSET_ExpandStr( wk->WordSet, wk->TitleString, wk->pExpStrBuf );
        length = FontProc_GetPrintStrWidth( FONT_SYSTEM, wk->TitleString, 0 );
        x      = width - length;
        GF_STR_PrintColor( &wk->MyWin, FONT_SYSTEM, wk->TitleString, x, 8*12, MSG_NO_PUT, _COL_N_BLACK, NULL);
    }
    GF_BGL_BmpWinOnVReq(&wk->MyWin);
#endif
	
    wk->seq = WIFIP2PMATCH_MODE_PERSONAL_WAIT;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �l�f�[�^�\�� WIFIP2PMATCH_MODE_PERSONAL_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _personalDataWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }
    if( 0 !=  _checkParentConnect(wk)){ // �ڑ����Ă���
        wk->seq = WIFIP2PMATCH_MODE_PERSONAL_END;
    }
    if(sys.trg & (PAD_BUTTON_CANCEL|PAD_BUTTON_DECIDE)){
        wk->seq = WIFIP2PMATCH_MODE_PERSONAL_END;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �l�f�[�^�\������� WIFIP2PMATCH_MODE_PERSONAL_END
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _personalDataEnd( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i,x;
    int num = 0;

#if 0
    BmpMenuWinClear(&wk->MyWin, WINDOW_TRANS_ON );
    GF_BGL_BmpWinDel(&wk->MyWin);
    GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME1_M);
    GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME3_M);
#endif
    EndMessageWindowOff(wk);

#if 0
    GF_BGL_BmpWinDataFill( &wk->MyInfoWinBack, 0x8000 );
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_019, wk->TitleString );
    GF_STR_PrintColor( &wk->MyInfoWinBack, FONT_TALK, wk->TitleString, 0, 0, MSG_NO_PUT, _COL_N_WHITE, NULL);
        GF_BGL_BmpWinOnVReq(&wk->MyInfoWinBack);

    _readFriendMatchStatus(wk);
	 _userDataDisp(wk);
    _myStatusChange(wk, wk->pMatch->myMatchStatus.status);
#endif

    wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �I���m�F���b�Z�[�W  WIFIP2PMATCH_MODE_EXIT_YESNO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _exitYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_MODE_EXIT_WAIT;
    }
	
// WIFI�@�ΐ�AUTOӰ�ރf�o�b�N
#ifdef _WIFI_DEBUG_TUUSHIN
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_A_REQ = TRUE;
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_B_REQ = FALSE;
#endif	//_WIFI_DEBUG_TUUSHIN
	
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �I���m�F���b�Z�[�W  WIFIP2PMATCH_MODE_EXIT_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _exitWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);

    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }

	//  �ڑ��`�F�b�N
    if( 0 !=  _checkParentNewPlayer(wk)){ // �ڑ����Ă���
		if(ret == BMPMENU_NULL){
			BmpYesNoWinDel( wk->pYesNoWork, HEAPID_WIFIP2PMATCH );
		}
		ret = BMPMENU_CANCEL;	// CANCEL
	}
	
    if(ret == BMPMENU_NULL){  // �܂��I��
        return seq;
    }else if(ret == 0){ // �͂���I�������ꍇ
        WifiP2PMatchMessagePrint(wk, dwc_message_0011, TRUE);
        wk->seq = WIFIP2PMATCH_MODE_EXITING;
        wk->timer = 1;
    }
    else{  // ��������I�������ꍇ
        EndMessageWindowOff(wk);
        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;

		// ��l���̓��������
		FriendRequestWaitOff( wk );
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �I���m�F���b�Z�[�W  WIFIP2PMATCH_MODE_EXIT_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _exitExiting( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }
    if(wk->timer == 1){
        wk->timer = 0;
        CommStateWifiLogout();  // �I��
    }
    if(!CommStateIsInitialize()){
        OHNO_PRINT("�ؒf�������Ƀt�����h�R�[�h���l�߂�\n");
        WifiList_FormUpData(wk->pList);  // �ؒf�������Ƀt�����h�R�[�h���l�߂�
        //SaveData_SaveParts(wk->pSaveData, SVBLK_ID_NORMAL);  //�Z�[�u��
        WifiP2PMatchMessagePrint(wk, dwc_message_0012, TRUE);
        wk->seq = WIFIP2PMATCH_MODE_EXIT_END;
        wk->timer = 30;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   �I������  WIFIP2PMATCH_MODE_EXIT_END
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------


static int _exitEnd( WIFIP2PMATCH_WORK *wk, int seq )
{
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }
    wk->timer--;
    if(wk->timer==0){
        wk->endSeq = WIFI_P2PMATCH_END;
        wk->seq = WIFIP2PMATCH_MODE_END_WAIT;
        EndMessageWindowOff(wk);
    }

// WIFI�@�ΐ�AUTOӰ�ރf�o�b�N
#ifdef _WIFI_DEBUG_TUUSHIN
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_A_REQ = FALSE;
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_B_REQ = FALSE;
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_TOUCH_REQ = FALSE;
#endif	//_WIFI_DEBUG_TUUSHIN
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   ������x�ΐ킷�邩���� WIFIP2PMATCH_NEXTBATTLE_YESNO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _nextBattleYesNo( WIFIP2PMATCH_WORK *wk, int seq )
{
//    CommStateSetErrorCheck(FALSE,FALSE);
    if(CommIsTimingSync(_TIMING_BATTLE_END)==FALSE){
        return seq;
    }
	// �ʐM�������ɓd����؂�ꂽ��A�����Ɠ����҂����Ă��܂��̂ŁA�ʐM������ɃI�[�g�G���[�`�F�b�N��
	// �͂���
    CommStateSetErrorCheck(FALSE,TRUE);	
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_NEXTBATTLE_WAIT;
    }
	
// WIFI�@�ΐ�AUTOӰ�ރf�o�b�N
#ifdef _WIFI_DEBUG_TUUSHIN
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_A_REQ = FALSE;
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_TOUCH_REQ = FALSE;
	WIFI_DEBUG_BATTLE_Work.DEBUG_WIFI_B_REQ = TRUE;
#endif	//_WIFI_DEBUG_TUUSHIN
	
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   ������x�ΐ킷�邩���� WIFIP2PMATCH_NEXTBATTLE_WAIT
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _nextBattleWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret;

    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) != 0 ){
        return seq;
    }

    if(CommStateIsWifiLoginState() || CommStateIsWifiDisconnect() || (CommWifiIsMatched() >= 3)){  // �ؒf���������Ƃ�
        BmpYesNoWinDel(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
        WifiP2PMatchMessagePrint(wk, msg_wifilobby_065, FALSE);
        wk->seq = WIFIP2PMATCH_MODE_BATTLE_DISCONNECT;
    }
    else if(CommStateIsWifiError()){
        BmpYesNoWinDel(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
        _errorDisp(wk);
    }
    else{
        ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
        if(ret == BMPMENU_NULL){  // �܂��I��
            return seq;
        }else if(ret == 0){ // �͂���I�������ꍇ
            //EndMessageWindowOff(wk);
            WifiP2PMatchMessagePrint(wk, msg_wifilobby_073, FALSE);
			GF_ASSERT( wk->timeWaitWork == NULL );
            wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
            wk->bRetryBattle = TRUE;
            wk->seq = WIFIP2PMATCH_MODE_CALL_YESNO;
            wk->timer = 30;
        }
        else{  // ��������I�������ꍇ
            EndMessageWindowOff(wk);
            CommInfoFinalize();
            CommStateWifiBattleMatchEnd();
            wk->timer = _RECONECTING_WAIT_TIME;
            wk->seq = WIFIP2PMATCH_RECONECTING_WAIT;
        }
        wk->pMatch = CommStateGetMatchWork();
        wk->pMatch->myMatchStatus.vchat = wk->pMatch->myMatchStatus.vchat_org;
    }
    return seq;
}


//------------------------------------------------------------------
/**
 * $brief   VCAHT��ύX���Ă��������ǂ������� WIFIP2PMATCH_MODE_VCHAT_NEGO
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _vchatNegoCheck( WIFIP2PMATCH_WORK *wk, int seq )
{
	// ����̏�Ԃ���������肵����I���
	{
		int status,friendNo,vchat;
		MCR_MOVEOBJ* p_npc;
		_WIFI_MACH_STATUS* p_status;

		// �b�������Ă���F�B�i���o�[�擾
		friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
		p_npc = MCRSYS_GetMoveObjWork( wk, friendNo );

		// ���肪���Ȃ��Ȃ�����
		// ����̃X�e�[�^�X���Ⴄ���̂ɕς������A
		// �\���������Č��ɖ߂�
		if( p_npc == NULL ){
			_friendNameExpand(wk, friendNo - 1);
			WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
			wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
			return seq;
		}else{
			p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
			status = _WifiMyStatusGet( wk, p_status );
			vchat = p_status->vchat;

			// ��Ԃ����������
			if((wk->keepStatus != status) || (wk->keepVChat != vchat)){
				_friendNameExpand(wk, friendNo - 1);
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
				wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
				return seq;
			}
		}
	}	
	
    if( GF_MSG_PrintEndCheck( wk->MsgIndex ) == 0 ){
        // �͂��������E�C���h�E���o��
        wk->pYesNoWork =
            BmpYesNoSelectInit( wk->bgl,
                                &_yesNoBmpDat,
                                MENU_WIN_CGX_NUM, MENU_WIN_PAL,
                                HEAPID_WIFIP2PMATCH );
        wk->seq = WIFIP2PMATCH_MODE_VCHAT_NEGO_WAIT;
    }
    return seq;
}

//------------------------------------------------------------------
/**
 * $brief   VCAHT��ύX���Ă��������ǂ�������
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static int _vchatNegoWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    int i;
    int ret,status;

	// ����̏�Ԃ���������肵����I���
	{
		int friendNo,vchat;
		MCR_MOVEOBJ* p_npc;
		_WIFI_MACH_STATUS* p_status;

		// �b�������Ă���F�B�i���o�[�擾
		friendNo = WIFI_MCR_PlayerSelect( &wk->matchroom );
		p_npc = MCRSYS_GetMoveObjWork( wk, friendNo );

		// ���肪���Ȃ��Ȃ�����
		// ����̃X�e�[�^�X���Ⴄ���̂ɕς������A
		// �\���������Č��ɖ߂�
		if( p_npc == NULL ){
			BmpYesNoWinDel(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
			_friendNameExpand(wk, friendNo - 1);
			WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
			wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
			return seq;
		}else{
			p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
			status = _WifiMyStatusGet( wk, p_status );
			vchat = p_status->vchat;

			// ��Ԃ����������
			if((wk->keepStatus != status) || (wk->keepVChat != vchat)){
				BmpYesNoWinDel(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
				_friendNameExpand(wk, friendNo - 1);
				WifiP2PMatchMessagePrint(wk, msg_wifilobby_013, FALSE);
				wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
				return seq;
			}
		}
	}	


    if(CommWifiIsMatched() >= 3){
        BmpYesNoWinDel(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
		WifiP2PMatchMessagePrint(wk, msg_wifilobby_015, FALSE);	// �������Ȃ��E�E�E
        CommStateWifiMatchEnd();
        wk->seq = WIFIP2PMATCH_MODE_DISCONNECT;
		return seq;
    }
    else if(CommStateIsWifiError()){
        BmpYesNoWinDel(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
        _errorDisp(wk);
    }
    else{
        ret = BmpYesNoSelectMain(wk->pYesNoWork, HEAPID_WIFIP2PMATCH);
        if(ret == BMPMENU_NULL){  // �܂��I��
            return seq;
        }else if(ret == 0){ // �͂���I�������ꍇ
            // �ڑ��J�n
            status = _convertState(wk->keepStatus);
            if(WIFI_STATUS_UNKNOWN == status){   // ���m�̃X�e�[�g�̏ꍇ���������悤�ɖ߂�
            }
            else if( WifiDwc_getFriendStatus(wk->friendNo - 1) == DWC_STATUS_MATCH_SC_SV ){
				_myVChatStatusToggle(wk); // ������VCHAT�𔽓]
                if( WifiP2PMatch_CommWifiBattleStart( wk, wk->friendNo - 1, status ) ){
                    wk->cancelEnableTimer = _CANCELENABLE_TIMER;
					_commStateChange(status);
                    CommSetWifiBothNet(FALSE);  // VCT���͓������M�̕K�v�Ȃ�
                    _myStatusChange(wk, status);  // �ڑ����ɂȂ�
                    _friendNameExpand(wk, wk->friendNo - 1);
                    WifiP2PMatchMessagePrint(wk,msg_wifilobby_014, FALSE);
					GF_ASSERT( wk->timeWaitWork == NULL );
                    wk->timeWaitWork = TimeWaitIconAdd( &wk->MsgWin, COMM_TALK_WIN_CGX_NUM );
                    if(status != WIFI_STATUS_VCT){
                        wk->seq = WIFIP2PMATCH_MODE_MATCH_LOOP;  // �}�b�`���O��
                        return seq;
                    }
                    else{
                        wk->cancelEnableTimer = _CANCELENABLE_TIMER;
                        wk->seq = WIFIP2PMATCH_MODE_VCT_CONNECT_INIT2;  //VCT��p��
                        return seq;
                    }
                }
            }
        }
        // ��������I�������ꍇ  �������͓K�����Ȃ��ꍇ
        EndMessageWindowOff(wk);


		// ���̐l�̏�������
//		WifiP2PMatch_UserDispOff( wk, HEAPID_WIFIP2PMATCH );

		// �X�e�[�^�X�����ǂ�
		// 080703	tomoya takahashi
        _myStatusChange(wk, WIFI_STATUS_LOGIN_WAIT);

		// VCHAT���ɖ߂�
		_myVChatStatusOrgSet( wk );
		_userDataInfoDisp(wk);
        wk->seq = WIFIP2PMATCH_MODE_FRIENDLIST;
        return seq;
        
    }
    return seq;
}







// WIFIP2PMATCH_MODE_END_WAIT
//------------------------------------------------------------------
/**
 * $brief   �I��
 *
 * @param   wk
 * @param   seq
 *
 * @retval  int
 */
//------------------------------------------------------------------
static int 	WifiP2PMatch_EndWait( WIFIP2PMATCH_WORK *wk, int seq )
{
    if(!CommStateIsInitialize()){
        WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, WIPE_FADE_BLACK,
                        COMM_BRIGHTNESS_SYNC, 1, HEAPID_WIFIP2PMATCH);
        seq = SEQ_OUT;						//�I���V�[�P���X��
    }
    return seq;
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


static void WifiP2PMatchMessagePrint( WIFIP2PMATCH_WORK *wk, int msgno, BOOL bSystem )
{
    // ������擾
    u8 speed = CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(wk->pSaveData));

	// TimeWaitIcon�j��
	_timeWaitIconDel( wk );

    if(GF_BGL_BmpWinAddCheck(&wk->SysMsgWin)){
        BmpTalkWinClear(&wk->SysMsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel( &wk->SysMsgWin);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MsgWin)){
        BmpTalkWinClear(&wk->MsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->MsgWin);
    }
    if(wk->MsgIndex != _PRINTTASK_MAX){
        if(GF_MSG_PrintEndCheck(wk->MsgIndex)!=0){
            GF_STR_PrintForceStop(wk->MsgIndex);
            wk->MsgIndex = _PRINTTASK_MAX;
        }
    }
    
    GF_BGL_BmpWinAdd(wk->bgl, &wk->MsgWin,
                     GF_BGL_FRAME2_M,
                     COMM_MSG_WIN_PX, COMM_MSG_WIN_PY,
                     COMM_MSG_WIN_SX, COMM_MSG_WIN_SY,
                     COMM_MESFONT_PAL, COMM_MSG_WIN_CGX);
    if(bSystem){
        MSGMAN_GetString(  wk->SysMsgManager, msgno, wk->pExpStrBuf );
    }
    else{
        MSGMAN_GetString(  wk->MsgManager, msgno, wk->pExpStrBuf );
    }
    WORDSET_ExpandStr( wk->WordSet, wk->TalkString, wk->pExpStrBuf );
    // ��b�E�C���h�E�g�`��
    GF_BGL_BmpWinDataFill(&wk->MsgWin, 15 );
    BmpTalkWinWrite(&wk->MsgWin, WINDOW_TRANS_OFF,COMM_TALK_WIN_CGX_NUM, COMM_MESFRAME_PAL );
    // ������`��J�n
	MsgPrintSkipFlagSet( MSG_SKIP_ON );		// ���b�Z�[�W�X�L�b�vON
	MsgPrintAutoFlagSet( MSG_AUTO_OFF );	// ���b�Z�[�W��������OFF
    wk->MsgIndex = GF_STR_PrintSimple( &wk->MsgWin, FONT_TALK, wk->TalkString,
                                       0, 0, speed, NULL);
    GF_BGL_BmpWinOnVReq(&wk->MsgWin);
}

//------------------------------------------------------------------
/**
 * $brief   �V�X�e�����b�Z�[�W�E�C���h�E�\��
 *
 * @param   wk
 *
 * @retval  none
 */
//------------------------------------------------------------------

static void _systemMessagePrint( WIFIP2PMATCH_WORK *wk, int msgno )
{
    _timeWaitIconDel(wk);
    if(GF_BGL_BmpWinAddCheck(&wk->SysMsgWin)){
        BmpTalkWinClear(&wk->SysMsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel( &wk->SysMsgWin);
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MsgWin)){
        BmpTalkWinClear(&wk->MsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->MsgWin);
    }
/*	
// �@�v���`�i�ł́A�����̃X�e�[�^�X����ɏ�ɕ\������Ă���悤�Ɏd�l�ύX���ꂽ���߁A
// �@�����Ă��܂��ƍ���̂ŁA����
#if AFTER_MASTER_070410_WIFIAPP_N20_EUR_FIX
    if(GF_BGL_BmpWinAddCheck(&wk->MyInfoWin)){
        BmpTalkWinClear( &wk->MyInfoWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel(&wk->MyInfoWin);
    }
#endif //AFTER_MASTER_070410_WIFIAPP_N20_EUR_FIX
//*/
    if(wk->MsgIndex != _PRINTTASK_MAX){
        if(GF_MSG_PrintEndCheck(wk->MsgIndex)!=0){
            GF_STR_PrintForceStop(wk->MsgIndex);
            wk->MsgIndex = _PRINTTASK_MAX;
        }
    }

    GF_BGL_BmpWinAdd(wk->bgl, &wk->SysMsgWin,
                     GF_BGL_FRAME2_M,
                     COMM_SYS_WIN_PX, COMM_SYS_WIN_PY,
                     COMM_SYS_WIN_SX, COMM_SYS_WIN_SY,
                     COMM_MESFONT_PAL, COMM_SYS_WIN_CGX);
    MSGMAN_GetString(  wk->SysMsgManager, msgno, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->WordSet, wk->TalkString, wk->pExpStrBuf );
    // ��b�E�C���h�E�g�`��
    GF_BGL_BmpWinDataFill(&wk->SysMsgWin, 15 );
    BmpMenuWinWrite(&wk->SysMsgWin, WINDOW_TRANS_OFF, MENU_WIN_CGX_NUM, MENU_WIN_PAL );
    // ������`��J�n
    wk->MsgIndex = GF_STR_PrintSimple( &wk->SysMsgWin, FONT_TALK,
                                       wk->TalkString, 0, 0, MSG_NO_PUT, NULL);
    GF_BGL_BmpWinOnVReq(&wk->SysMsgWin);
}

//------------------------------------------------------------------
/**
 * $brief   �F�l�ԍ��̖��O��expand����
 * @param   msg_index
 * @retval  int		    int friend = mydwc_getFriendIndex();

 */
//------------------------------------------------------------------
static void _friendNameExpand( WIFIP2PMATCH_WORK *wk, int friendNo)
{
    if(friendNo != -1){
        MYSTATUS* pTarget = MyStatus_AllocWork(HEAPID_WIFIP2PMATCH);
        MyStatus_SetMyName(pTarget, WifiList_GetFriendNamePtr(wk->pList,friendNo));
        WORDSET_RegisterPlayerName( wk->WordSet, 0, pTarget);
        sys_FreeMemoryEz(pTarget);
    }
}


//------------------------------------------------------------------
/**
 * $brief
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void EndMessageWindowOff( WIFIP2PMATCH_WORK *wk )
{
    _timeWaitIconDel(wk);
    if(wk->MsgIndex != _PRINTTASK_MAX){
        if(GF_MSG_PrintEndCheck(wk->MsgIndex)!=0){
            GF_STR_PrintForceStop(wk->MsgIndex);
            wk->MsgIndex = _PRINTTASK_MAX;
        }
    }
    if(GF_BGL_BmpWinAddCheck(&wk->MsgWin)){
        BmpTalkWinClear(&wk->MsgWin, WINDOW_TRANS_ON );
        GF_BGL_BmpWinDel( &wk->MsgWin );
    }
}

//------------------------------------------------------------------
/**
 * $brief
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void _myStatusChange(WIFIP2PMATCH_WORK *wk, int status)
{
    if(wk->pMatch==NULL){  //0707
        return;
    }
	_myStatusChange_not_send( wk, status );
    mydwc_setMyInfo( &(wk->pMatch->myMatchStatus), sizeof(_WIFI_MACH_STATUS) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̃X�e�[�^�X�ݒ�		�������M�͂��Ȃ�
 *
 *	@param	wk
 *	@param	status
 */
//-----------------------------------------------------------------------------
static void _myStatusChange_not_send(WIFIP2PMATCH_WORK *wk, int status)
{
	int org_status;

    if(wk->pMatch==NULL){  //0707
        return;
    }
    
	org_status = _WifiMyStatusGet( wk, &wk->pMatch->myMatchStatus );
	
    if(org_status != status){

        wk->pMatch->myMatchStatus.status = status;
        if(_modeBattle(status) || (status == WIFI_STATUS_TRADE)|| (status == WIFI_STATUS_FRONTIER)||
#ifdef WFP2P_DEBUG_EXON 
				 (status == WIFI_STATUS_BATTLEROOM) ||
#endif
				 (status == WIFI_STATUS_BUCKET) || 
				 (status == WIFI_STATUS_BALANCEBALL) || 
				 (status == WIFI_STATUS_BALLOON) || 
				 (status == WIFI_STATUS_POFIN)){
            //     Snd_BgmFadeOut( 24, BGM_FADE_VCHAT_TIME);
		} 
        else if(status == WIFI_STATUS_VCT){
            Snd_BgmFadeOut( 0, BGM_FADE_VCHAT_TIME); // VCT��Ԃ�
			
        }
        else if(status == WIFI_STATUS_LOGIN_WAIT){    // �ҋ@���@���O�C������͂���

			// �{�C�X�`���b�g�Ȃ��ɂ���
			mydwc_setVChat(FALSE);		// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi

			if( WifiP2P_CheckLobbyBgm() == TRUE ){
				// WiFi�L��ɗ����Ƃ��͂ǂ�ȃ{�����[���ł��ʏ�{�����[���ɂ���
		        Snd_BgmFadeIn( BGM_WIFILOBBY_VOL, BGM_FADE_VCHAT_TIME, BGM_FADEIN_START_VOL_NOW);
			}
#if 0
			//�{�C�X�`���b�g���I�����Ă���̂ŁA�ݒ艹�ʂ������l�ɖ߂��I
			//�{���͐ݒ肵�Ă��鏉�����ʂɖ߂��Ȃ��Ƃ����Ȃ����B�B�Ƃ肠��������őΏ��I
			Snd_PlayerSetInitialVolumeBySeqNo( _BGM_MAIN, BGM_WIFILOBBY_VOL );
			Snd_PlayerMoveVolume( SND_HANDLE_BGM, BGM_WIFILOBBY_START_VOL, 0 );
            Snd_BgmFadeIn( BGM_VOL_MAX, BGM_FADE_VCHAT_TIME, BGM_FADEIN_START_VOL_NOW);
            OHNO_PRINT("Snd_BgmFadeIn\n");
#endif
        }
    }
    _userDataInfoDisp(wk);
}

//------------------------------------------------------------------
/**
 * $brief   VCHAT�t���O�̐؂�ւ�
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static BOOL _myVChatStatusToggle(WIFIP2PMATCH_WORK *wk)
{
    wk->pMatch->myMatchStatus.vchat = 1 - wk->pMatch->myMatchStatus.vchat;
//    mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);	// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
    mydwc_setMyInfo( &(wk->pMatch->myMatchStatus), sizeof(_WIFI_MACH_STATUS) );
    return wk->pMatch->myMatchStatus.vchat;
}

//------------------------------------------------------------------
/**
 * $brief   VCHAT�t���O�̐؂�ւ�
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static BOOL _myVChatStatusToggleOrg(WIFIP2PMATCH_WORK *wk)
{
    wk->pMatch->myMatchStatus.vchat_org = 1 - wk->pMatch->myMatchStatus.vchat_org;
    wk->pMatch->myMatchStatus.vchat = wk->pMatch->myMatchStatus.vchat_org;
//    mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);	// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
    mydwc_setMyInfo( &(wk->pMatch->myMatchStatus), sizeof(_WIFI_MACH_STATUS) );
    return wk->pMatch->myMatchStatus.vchat_org;
}

//------------------------------------------------------------------
/**
 * $brief   VCHAT�t���O���I���W�i���ɂ��ǂ�
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------

static BOOL _myVChatStatusOrgSet(WIFIP2PMATCH_WORK *wk)
{
	OS_TPrintf( "change org %d\n", wk->pMatch->myMatchStatus.vchat_org );
    wk->pMatch->myMatchStatus.vchat = wk->pMatch->myMatchStatus.vchat_org;
//    mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi
    mydwc_setMyInfo( &(wk->pMatch->myMatchStatus), sizeof(_WIFI_MACH_STATUS) );
    return wk->pMatch->myMatchStatus.vchat_org;
}


//------------------------------------------------------------------
/**
 * $brief
 * @param   wk
 * @retval  none
 */
//------------------------------------------------------------------
static void _timeWaitIconDel(WIFIP2PMATCH_WORK *wk)
{
    if(wk->timeWaitWork){
        TimeWaitIconTaskDel(wk->timeWaitWork);  // �^�C�}�[�~��
        //TimeWaitIconDel(wk->timeWaitWork);
        //        GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME2_M);
        wk->timeWaitWork = NULL;
        if(GF_BGL_BmpWinAddCheck(&wk->MsgWin)){
            BmpTalkWinClear( &wk->MsgWin, WINDOW_TRANS_ON );
            GF_BGL_BmpWinDel( &wk->MsgWin );
        }
        //      }
        //		GF_BGL_LoadScreenReq( wk->bgl, GF_BGL_FRAME2_M );
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[�f�[�^�����\������
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	friendNo	�F�B�ԍ�
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatch_UserDispOn( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 heapID )
{
	MCR_MOVEOBJ* p_obj;
	
	MCVSys_UserDispOn( wk, friendNo, heapID );
	p_obj = MCRSYS_GetMoveObjWork( wk, friendNo );
	if( p_obj ){
		WIFI_MCR_CursorOn( &wk->matchroom, p_obj );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[�f�[�^�����\�������I��
 *
 *	@param	wk			�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatch_UserDispOff( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	MCVSys_UserDispOff( wk );
	WIFI_MCR_CursorOff( &wk->matchroom );

	// �ĕ`��
	MCVSys_ReWrite( wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�����̐l�Ȃ狭���I�ɕ\���I��
 *
 *	@param	wk				�V�X�e�����[�N
 *	@param	target_friend	�^�[�Q�b�g�̐l���̃C���f�b�N�X
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatch_UserDispOff_Target( WIFIP2PMATCH_WORK *wk, u32 target_friend, u32 heapID )
{
	if( MCVSys_UserDispGetFriend( wk ) == target_friend ){
		WifiP2PMatch_UserDispOff( wk, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ŃA�N�Z�X�����Ƃ��̃��[�U�[�f�[�^�����\������
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatch_UserDispOn_MyAcces( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 heapID )
{
	MCVSys_UserDispOn( wk, friendNo, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ŃA�N�Z�X�����Ƃ��̃��[�U�[�f�[�^�����\�������I��
 */
//-----------------------------------------------------------------------------
static void WifiP2PMatch_UserDispOff_MyAcces( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	MCVSys_UserDispOff( wk );

	// �ĕ`��
	MCVSys_ReWrite( wk, HEAPID_WIFIP2PMATCH );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���삵�Ă��邩�`�F�b�N
 *
 *	@param	wk	�V�X�e�����[�N
 *
 *	@retval	TRUE	���쒆
 *	@retval	FALSE	�񓮍쒆
 */
//-----------------------------------------------------------------------------
static BOOL MCVSys_MoveCheck( const WIFIP2PMATCH_WORK *wk )
{
	if( wk->view.p_bttnman == NULL ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�r���[�A�[������
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MCVSys_Init( WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle, u32 heapID )
{
	static const TP_HIT_TBL bttndata[ WCR_MAPDATA_1BLOCKOBJNUM ] = {
		{	0,		47,		0,		119 },
		{	48,		95,		0,		119 },
		{	96,		143,	0,		119 },
		{	144,	191,	0,		119 },

		{	0,		47,		128,	255 },
		{	48,		95,		128,	255 },
		{	96,		143,	128,	255 },
		{	144,	191,	128,	255 },
	};

	memset( &wk->view, 0, sizeof(WIFIP2PMATCH_VIEW) );

	// �\���ݒ�
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	
	// �{�^���f�[�^������
	wk->view.p_bttnman = BMN_Create( bttndata, WCR_MAPDATA_1BLOCKOBJNUM, MCVSys_BttnCallBack, wk, heapID );

	// ���[�h�Z�b�g������
	wk->view.p_wordset = WORDSET_Create( heapID );

	// �O���t�B�b�N�f�[�^�Ǎ���
	MCVSys_GraphicSet( wk, p_handle, heapID );

	// �Ƃ肠�����X�V
	wk->view.bttn_allchg = TRUE;
	MCVSys_BackDraw( wk );
	MCVSys_BttnDraw( wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�r���[�A�[�j��
 *
 *	@param	wk			�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_Exit( WIFIP2PMATCH_WORK *wk )
{
	if( MCVSys_MoveCheck( wk ) == FALSE ){
		return;  
	}
	
	// �O���t�B�b�N�͂�
	MCVSys_GraphicDel( wk );

	// �{�^���}�l�[�W���j��
	BMN_Delete( wk->view.p_bttnman );	
	wk->view.p_bttnman = NULL;

	// ���[�h�Z�b�g�j��
	WORDSET_Delete( wk->view.p_wordset );
	wk->view.p_wordset = NULL;

	// BG�Q�A�R�͔�\���ɂ��Ă���
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�r���[�A�[�A�b�v�f�[�g
 *
 *	@param	wk			�V�X�e�����[�N
 *
 *	@retval	������Ă���F�B�ԍ�	+ 1
 *	@retval	0	������Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static u32 MCVSys_Updata( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	u32 map_param, map_param2;
	u32 oambttn_ret;
	BOOL userd_end;

	// ���݃t���[���ݒ�
	map_param = WIFI_MCR_GetPlayerOnMapParam( &wk->matchroom );
	
	// �I�u�W�F�N�g�̈ʒu���Ɖ������Ȃ��Ƃ����Ƃ����t���[���̈ʒu���擾�ł��Ȃ�
	map_param2 = WIFI_MCR_GetPlayerOnUnderMapParam( &wk->matchroom );
	if( (map_param2 >= MCR_MAPPM_MAP00) &&
		(map_param2 <= MCR_MAPPM_MAP03) ){
		map_param = map_param2;
	}
	
	if( (map_param >= MCR_MAPPM_MAP00) &&
		(map_param <= MCR_MAPPM_MAP03) ){
		if( wk->view.frame_no != (map_param - MCR_MAPPM_MAP00) ){
			wk->view.frame_no = (map_param - MCR_MAPPM_MAP00);

		
			Snd_SePlay( _SE_TBLCHANGE );

			// �w�i�J���[�ύX	
			MCVSys_BackDraw( wk );

			// �{�^��������������
			wk->view.button_on = TRUE;
			wk->view.bttn_allchg = TRUE;	// �{�^�������ύX 
		}
	}


	// ���[�U�[�f�[�^�\������
	if( wk->view.user_disp == MCV_USERDISP_INIT ){
		wk->view.user_disp = MCV_USERDISP_ON;
		wk->view.user_dispno = WF_USERDISPTYPE_NRML;
		MCVSys_UserDispDraw( wk, heapID );
		MCVSys_OamBttnOn( wk );	// OAM�{�^���\��
	}


	if( wk->view.user_disp == MCV_USERDISP_OFF ){

		// �{�^�����C��
		BMN_Main( wk->view.p_bttnman );

		// �{�^���A�j������
		MCVSys_BttnAnmMan( wk );

		// �\�����C��
		if( wk->view.button_on == TRUE ){
			MCVSys_BttnDraw( wk );
			wk->view.button_on = FALSE;
		}
	}

	// ��������A�^�b�`�p�l���ɐG��Ă����烆�[�U�[�\����OFF����
	if( (wk->view.user_disp == MCV_USERDISP_ON) || 
		(wk->view.user_disp == MCV_USERDISP_ONEX) ){

		// �������OAM�{�^�����C��
		oambttn_ret = MCVSys_OamBttnMain( wk );

		//  USERD�I���`�F�b�N
		userd_end = MCVSys_UserDispEndCheck( wk, oambttn_ret );
		
		// �Ȃɂ��L�[���������A�u���ǂ�v����������I������
		if( userd_end == TRUE ){
			wk->view.bttn_chg_friendNo = wk->view.touch_friendNo;	// ���̗F�B�̃{�^�����X�V���Ă��炤
			wk->view.touch_friendNo = 0;
			wk->view.touch_frame = 0;
			wk->view.button_on = TRUE;
			wk->view.bttn_allchg = TRUE;
			wk->view.user_disp = MCV_USERDISP_OFF;
			MCVSys_OamBttnOff( wk );	// �{�^��OFF
			Snd_SePlay( _SE_DESIDE );	// �L�����Z����
		}else{
			// ���E�̃{�^����������Ă�����y�[�W�؂�ւ��P
			if( oambttn_ret != MCV_USERD_BTTN_RET_NONE ){
				Snd_SePlay( _SE_DESIDE );						// �y�[�W�ύX��
				MCVSys_UserDispPageChange( wk, oambttn_ret );	// �ύX����
				MCVSys_UserDispDraw( wk, heapID );
			}
		}
	}

	return wk->view.touch_friendNo;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�r���[�A�[�A�b�v�f�[�g	�{�^���̕\���̂�
 *
 *	@param	wk	�V�X�e�����[�N 
 */
//-----------------------------------------------------------------------------
static void MCVSys_UpdataBttn( WIFIP2PMATCH_WORK *wk )
{
	if( wk->view.user_disp == MCV_USERDISP_OFF ){

		// �\�����C��
		if( wk->view.button_on == TRUE ){
			MCVSys_BttnDraw( wk );
			wk->view.button_on = FALSE;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[�f�B�X�v���C���I�����邩�`�F�b�N
 *
 *	@param	wk			���[�N
 *	@param	oambttn_ret	OAM�{�^�����C���߂�l
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL MCVSys_UserDispEndCheck( WIFIP2PMATCH_WORK *wk, u32 oambttn_ret )
{
	// �����\�����Ă���̂ŏI�����Ȃ�
	if( (wk->view.user_disp == MCV_USERDISP_ONEX) ){
		return FALSE;
	}
	
	//  �ړ���cont�{�^���̓g���K�[
	if( (sys.cont & (PAD_KEY_LEFT|PAD_KEY_RIGHT|PAD_KEY_UP|PAD_KEY_DOWN)) ||
		(sys.trg & (PAD_BUTTON_A|PAD_BUTTON_B|PAD_BUTTON_X)) ||
		(oambttn_ret == MCV_USERD_BTTN_RET_BACK) ){
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڍו\�����Ă���l�̗F�BIDX���擾
 *
 *	@param	cp_wk	���[�N
 *		
 *	@return	�F�B�C���f�b�N�X	(0�Ȃ�\�����ĂȂ�)
 */
//-----------------------------------------------------------------------------
static u32	MCVSys_UserDispGetFriend( const WIFIP2PMATCH_WORK* cp_wk )
{
	return cp_wk->view.touch_friendNo;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[�f�[�^�\��
 *
 *	@param	wk			���[�N
 *	@param	friendNo	�F�B�ԍ�
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispOn( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 heapID )
{
	if( MCVSys_MoveCheck(wk) == TRUE ){
		wk->view.touch_friendNo = friendNo;
		wk->view.touch_frame	= 2;
		wk->view.user_disp		= MCV_USERDISP_ONEX;
		wk->view.user_dispno = WF_USERDISPTYPE_NRML;
		MCVSys_UserDispDraw( wk, heapID );
		MCVSys_OamBttnOnNoBack( wk );	//  �����\���p�{�^���\��
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[�f�[�^OFF
 *
 *	@param	wk			���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispOff( WIFIP2PMATCH_WORK *wk )
{
	if( MCVSys_MoveCheck(wk) == TRUE ){
		wk->view.touch_friendNo = 0;
		wk->view.touch_frame = 0;
		wk->view.user_disp		= MCV_USERDISP_OFF;
		wk->view.button_on		= TRUE;
		wk->view.bttn_allchg = TRUE;
		MCVSys_OamBttnOff( wk );	// �{�^��OFF
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�y�[�W�ύX����
 *	
 *	@param	wk				���[�N	
 *	@param	oambttn_ret		�{�^���߂�l
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispPageChange( WIFIP2PMATCH_WORK *wk, u32 oambttn_ret )
{
	u32 typenum;

	// �t�����e�B�A�L���ŁA�\������ς���
	if( _frontierInCheck(wk) == TRUE ){
		typenum = WF_USERDISPTYPE_NUM;
	}else{
		typenum = WF_USERDISPTYPE_MINI+1;	// �~�j�Q�[���܂ŕ\��
	}
	
	if( oambttn_ret == MCV_USERD_BTTN_RET_RIGHT ){
		wk->view.user_dispno = (wk->view.user_dispno+1) % typenum;
	}else{
		wk->view.user_dispno --;
		if( wk->view.user_dispno < 0 ){
			wk->view.user_dispno += typenum;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���C�x���g�R�[���o�b�N
 *
 *	@param	bttnid		�{�^��ID
 *	@param	event		�C�x���g���
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnCallBack( u32 bttnid, u32 event, void* p_work )
{
	WIFIP2PMATCH_WORK *wk = p_work;
	u32 friendNo;

	// �F�B�ԍ��擾
	friendNo = (wk->view.frame_no * WCR_MAPDATA_1BLOCKOBJNUM) + bttnid;
	friendNo ++;	// �������O������

	// ���쒆���`�F�b�N
	if( wk->view.bttnfriendNo[ friendNo-1 ] != MCV_BTTN_FRIEND_TYPE_IN ){
		// ���삵�Ă��Ȃ��̂ŉ������Ȃ�
		return ;
	}

	// ���łɃ{�^�����쒆�Ȃ̂Ŕ������Ȃ�
	if( wk->view.touch_friendNo != 0 ){
		return ;
	}

	switch( event ){
	case BMN_EVENT_TOUCH:		///< �G�ꂽ�u��
		wk->view.touch_friendNo = friendNo;
		Snd_SePlay( _SE_DESIDE );
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�f�[�^�ݒ�
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	p_handle	�A�[�J�C�u�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MCVSys_GraphicSet( WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle, u32 heapID )
{
	int i, j;
	int x, y;
	
	// BG�ݒ�
	// FRAME0_S�X�N���[���f�[�^�N���[��
    GF_BGL_ScrClear( wk->bgl, GF_BGL_FRAME0_S );
	// �p���b�g�]��
	ArcUtil_HDL_PalSet( p_handle, NARC_wifip2pmatch_wf_match_btm_NCLR,	// �w�i�p
			PALTYPE_SUB_BG, MCV_PAL_BACK*32, MCV_PAL_BACK_NUM*32, heapID );
	ArcUtil_HDL_PalSet( p_handle, NARC_wifip2pmatch_wf_match_btm_button_NCLR,	// �{�^���p
			PALTYPE_SUB_BG, MCV_PAL_BTTN*32, MCV_PAL_BTTN_NUM*32, heapID );

	// �L�����N�^�]��
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wf_match_btm_NCGR,
			wk->bgl, GF_BGL_FRAME0_S, MCV_CGX_BACK, 0, FALSE, heapID );
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifip2pmatch_wf_match_btm_button_NCGR,
			wk->bgl, GF_BGL_FRAME2_S, MCV_CGX_BTTN2, 0, FALSE, heapID );

	// �X�N���[���Ǎ���or�]��
	// �w�i�̓L�����N�^�ʒu������Ă���̂ŃX�N���[���f�[�^������������	
	ArcUtil_HDL_ScrnSet( p_handle, NARC_wifip2pmatch_wf_match_btm_NSCR, wk->bgl, GF_BGL_FRAME0_S, 0, 0, FALSE, heapID );
	
	// �{�^���X�N���[���Ǎ���
	wk->view.p_bttnbuff = ArcUtil_HDL_ScrnDataGet( p_handle, NARC_wifip2pmatch_wf_match_btm_button_NSCR, FALSE, &wk->view.p_bttnscrn, heapID );
	MCVSys_GraphicScrnCGOfsChange( wk->view.p_bttnscrn, MCV_CGX_BTTN2 );

	// ���[�U�[�X�N���[���Ǎ���
	for( i=0; i<WF_USERDISPTYPE_NUM; i++ ){
		wk->view.p_userbuff[i] = ArcUtil_HDL_ScrnDataGet( p_handle, NARC_wifip2pmatch_wf_match_btm_result00_NSCR+i, FALSE, &wk->view.p_userscrn[i], heapID );
		MCVSys_GraphicScrnCGOfsChange( wk->view.p_userscrn[i], MCV_CGX_BTTN2 );
	}

	// �_�~�[�X�N���[���ǂݍ���
	wk->view.p_useretcbuff = ArcUtil_HDL_ScrnDataGet( p_handle, NARC_wifip2pmatch_wf_match_btm_etc_NSCR, FALSE, &wk->view.p_useretcscrn, heapID );
	MCVSys_GraphicScrnCGOfsChange( wk->view.p_useretcscrn, MCV_CGX_BTTN2 );
	
	

	// �t�H���g�p���b�g�Ǎ���
	TalkFontPaletteLoad( PALTYPE_SUB_BG, MCV_SYSFONT_PAL*32, heapID );

	// �r�b�g�}�b�v�쐬
	for( i=0; i<WCR_MAPDATA_1BLOCKOBJNUM; i++ ){
		x = i/4;	// �z�u��2*4
		y = i%4;	
		GF_BGL_BmpWinInit( &wk->view.nameWin[i] );
		GF_BGL_BmpWinAdd( wk->bgl, &wk->view.nameWin[i],
				GF_BGL_FRAME1_S, 
				MCV_NAMEWIN_DEFX+(MCV_NAMEWIN_OFSX*x),
				MCV_NAMEWIN_DEFY+(MCV_NAMEWIN_OFSY*y),
				MCV_NAMEWIN_SIZX, MCV_NAMEWIN_SIZY,
				MCV_SYSFONT_PAL, MCV_NAMEWIN_CGX+(MCV_NAMEWIN_CGSIZ*i) );
		// �����ɂ��ēW�J
		GF_BGL_BmpWinDataFill( &wk->view.nameWin[i], 0 );
		GF_BGL_BmpWinOnVReq( &wk->view.nameWin[i] );

		// ��Ԗʏ������ݐ�
		for( j=0; j<WF_VIEW_STATUS_NUM; j++ ){
			GF_BGL_BmpWinInit( &wk->view.statusWin[i][j] );
			GF_BGL_BmpWinAdd( wk->bgl, &wk->view.statusWin[i][j],
					GF_BGL_FRAME1_S, 
					MCV_STATUSWIN_DEFX+(MCV_STATUSWIN_OFSX*x)+(j*MCV_STATUSWIN_VCHATX),
					MCV_STATUSWIN_DEFY+(MCV_STATUSWIN_OFSY*y),
					MCV_STATUSWIN_SIZX, MCV_STATUSWIN_SIZY,
					PLAYER_DISP_ICON_PLTTOFS_SUB, 
					MCV_STATUSWIN_CGX+(MCV_STATUSWIN_CGSIZ*((i*2)+j)) );
			GF_BGL_BmpWinDataFill( &wk->view.statusWin[i][j], 0 );
			GF_BGL_BmpWinOnVReq( &wk->view.statusWin[i][j] );
		}
		
	}
	GF_BGL_BmpWinAdd( wk->bgl, &wk->view.userWin,
			GF_BGL_FRAME3_S, 
			MCV_USERWIN_X, MCV_USERWIN_Y,
			MCV_USERWIN_SIZX, MCV_USERWIN_SIZY,
			MCV_SYSFONT_PAL, MCV_USERWIN_CGX );
	// �����ɂ��ēW�J
	GF_BGL_BmpWinDataFill( &wk->view.userWin, 0 );
	GF_BGL_BmpWinOnVReq( &wk->view.userWin );


	// �T�u���OAM������
	MCVSys_OamBttnInit( wk, p_handle, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�f�[�^�j��
 *
 *	@param	wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_GraphicDel( WIFIP2PMATCH_WORK *wk )
{
	int i, j;

	// �T�u���OAM�j��
	MCVSys_OamBttnDelete( wk );
	
	// �r�b�g�}�b�v�j��
	for( i=0; i<WCR_MAPDATA_1BLOCKOBJNUM; i++ ){
		GF_BGL_BmpWinDel( &wk->view.nameWin[i] );
		for( j=0; j<WF_VIEW_STATUS_NUM; j++ ){
			GF_BGL_BmpWinDel( &wk->view.statusWin[i][j] );
		}
	}
	GF_BGL_BmpWinDel( &wk->view.userWin );
	
	// �{�^���X�N���[���j��
	sys_FreeMemoryEz( wk->view.p_bttnbuff );

	// ���[�U�[�E�C���h�E
	for( i=0; i<WF_USERDISPTYPE_NUM; i++ ){
		sys_FreeMemoryEz( wk->view.p_userbuff[i] );
	}

	// �_�~�[�X�N���[���j��
	sys_FreeMemoryEz( wk->view.p_useretcbuff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	SCRN�̃L�����N�^No��]����̃A�h���X������
 *
 *	@param	p_scrn	�X�N���[���f�[�^
 *	@param	cgofs	�L�����N�^�I�t�Z�b�g�i�L�����N�^�P�ʁj
 */
//-----------------------------------------------------------------------------
static void MCVSys_GraphicScrnCGOfsChange( NNSG2dScreenData* p_scrn, u8 cgofs )
{
	u16* p_scrndata;
	int i, j;
	int siz_x, siz_y;

	p_scrndata = (u16*)p_scrn->rawData;
	siz_x = p_scrn->screenWidth/8;
	siz_y = p_scrn->screenHeight/8;

	for( i=0; i<siz_y; i++ ){
		for( j=0; j<siz_x; j++ ){
			p_scrndata[ (i*siz_x)+j ] += cgofs;
		}  
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���ݒ�
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	friendNo	�F�B�ԍ�
 *	@param	type		�ݒ�^�C�v
 *
 *	type
 *		MCV_BTTN_FRIEND_TYPE_RES,	// �\��
 *		MCV_BTTN_FRIEND_TYPE_IN,	// �o�^�ς�
 *		
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnSet( WIFIP2PMATCH_WORK *wk, u32 friendNo, u32 type )
{
	GF_ASSERT( friendNo > 0 );
	GF_ASSERT( type > MCV_BTTN_FRIEND_TYPE_NONE );
	GF_ASSERT( type < MCV_BTTN_FRIEND_TYPE_MAX );
	wk->view.bttnfriendNo[ friendNo-1 ] = type;
	wk->view.button_on = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^������͂���
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	friendNo	�F�B�ԍ�
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnDel( WIFIP2PMATCH_WORK *wk, u32 friendNo )
{
	GF_ASSERT( friendNo > 0 );
	wk->view.bttnfriendNo[ friendNo-1 ] = MCV_BTTN_FRIEND_TYPE_NONE;
	wk->view.button_on = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���̕\���^�C�v���擾����
 *
 *	@param	wk			���[�N
 *	@param	friendNo	�F�Bnumber
 *	
 *	@return	�\���^�C�v
 *	type
 *		MCV_BTTN_FRIEND_TYPE_RES,	// �\��
 *		MCV_BTTN_FRIEND_TYPE_IN,	// �o�^�ς�
 *		MCV_BTTN_FRIEND_TYPE_NONE,	// �Ȃ�
 */
//-----------------------------------------------------------------------------
static u32 MCVSys_BttnTypeGet( const WIFIP2PMATCH_WORK *wk, u32 friendNo )
{
	GF_ASSERT( friendNo > 0 );
	return wk->view.bttnfriendNo[ friendNo-1 ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	������������
 *
 *	@param	wk			�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_ReWrite( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	if( wk->view.user_disp != MCV_USERDISP_OFF ){
		MCVSys_UserDispDraw( wk, heapID );
	}else{
		wk->view.bttn_allchg = TRUE;
		MCVSys_BttnDraw( wk );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^�������ׂď������N�G�X�g���o��
 *
 *	@param	wk			
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnAllWriteReq( WIFIP2PMATCH_WORK *wk )
{
	wk->view.bttn_allchg = TRUE;
	wk->view.button_on = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�w�i�`��
 *
 *	@param	wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_BackDraw( WIFIP2PMATCH_WORK *wk )
{
	// �w�i�̃J���[��ς���
	GF_BGL_ScrPalChange( wk->bgl, GF_BGL_FRAME0_S, 0, 0,
			32, 24, wk->view.frame_no+MCV_PAL_FRMNO );

	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME0_S );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���\��
 *
 *	@param	wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnDraw( WIFIP2PMATCH_WORK *wk )
{
	int i, j;
	int sex;
	int bttn_type;
	int x, y;
	int friend_no;
	int frame;
	BOOL write_change_masterflag;	// �����I�ɂ��ׂẴ{�^��������������
	BOOL write_change_localflag;	// �����̃{�^�����Ƃ̏��������`�F�b�N

	// �S���������`�F�b�N
	if( wk->view.bttn_allchg == TRUE ){
		wk->view.bttn_allchg = FALSE;
		write_change_masterflag = TRUE;

		// �X�N���[���N���A
		GF_BGL_ScrFill( wk->bgl, GF_BGL_FRAME2_S, 0, 0, 0, 32, 24, 0 );
	}else{
		write_change_masterflag = FALSE;
	}


	for( i=0; i<WCR_MAPDATA_1BLOCKOBJNUM; i++ ){
		
		friend_no = (wk->view.frame_no*WCR_MAPDATA_1BLOCKOBJNUM) + i + 1;
		// �\���^�C�v�ݒ�
		if( wk->view.bttnfriendNo[friend_no - 1] != MCV_BTTN_FRIEND_TYPE_NONE ){
			sex = WifiList_GetFriendInfo( wk->pList, friend_no - 1, WIFILIST_FRIEND_SEX );
			if(sex == PM_MALE){
				bttn_type = MCV_BUTTON_TYPE_MAN;
			}else{
				bttn_type = MCV_BUTTON_TYPE_GIRL;
			}
		}else{
			bttn_type = MCV_BUTTON_TYPE_NONE;
		}

		// �{�^���\�����W
		x = i/4;
		y = i%4;

		// ������Ă���{�^�����`�F�b�N
		if( friend_no == wk->view.touch_friendNo ){
			frame = wk->view.touch_frame;
		}else{
			frame = 0;
		}


		// �{�^���̌`���ς���Ă��邩�`�F�b�N
		if( (friend_no == wk->view.touch_friendNo) || 
			(friend_no == wk->view.bttn_chg_friendNo) ){
			write_change_localflag = TRUE;
		}else{
			write_change_localflag = FALSE;
		}

		// ���������邩�`�F�b�N
		if( (write_change_masterflag == TRUE) ||
			(write_change_localflag == TRUE) ){

			// �{�^��
			MCVSys_BttnWrite( wk, 
					MCV_BUTTON_DEFX+(MCV_BUTTON_OFSX*x), MCV_BUTTON_DEFY+(MCV_BUTTON_OFSY*y),
					bttn_type, frame );
			
			// ���O�\��
			if( bttn_type != MCV_BUTTON_TYPE_NONE ){

				// ���O�̕\��
				MCVSys_BttnWinDraw( wk, &wk->view.nameWin[i], friend_no, frame, i );	
				MCVSys_BttnStatusWinDraw( wk, wk->view.statusWin[i], friend_no, frame, i );
			}else{

				// �����ɂ���
				GF_BGL_BmpWinDataFill( &wk->view.nameWin[i], 0 );
				GF_BGL_BmpWinOnVReq( &wk->view.nameWin[i] );
				for( j=0; j<WF_VIEW_STATUS_NUM; j++ ){
					GF_BGL_BmpWinDataFill( &wk->view.statusWin[i][j], 0 );
					GF_BGL_BmpWinOnVReq( &wk->view.statusWin[i][j] );
				}
			}
		}
	}

	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME2_S );

	// ���b�Z�[�W�ʂ̕\���ݒ�
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�U�[�f�[�^�\��
 *
 *	@param	wk	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispDraw( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	static void (*writeFunc[ WF_USERDISPTYPE_NUM ])( WIFIP2PMATCH_WORK *wk, u32 heapID ) = 
	{
		MCVSys_UserDispDrawType00,
		MCVSys_UserDispDrawType06,
		MCVSys_UserDispDrawType01,
		MCVSys_UserDispDrawType02,
		MCVSys_UserDispDrawType03,
		MCVSys_UserDispDrawType04,
		MCVSys_UserDispDrawType05
	};
	int sex;
	int pal;

	sex = WifiList_GetFriendInfo( wk->pList, 
			wk->view.touch_friendNo - 1, WIFILIST_FRIEND_SEX );

	if(sex == PM_FEMALE){
		pal = MCV_PAL_BTTN+MCV_PAL_BTTNST_GIRL;
	}else{
		pal = MCV_PAL_BTTN+MCV_PAL_BTTNST_MAN;
	}
	
	GF_BGL_ScreenBufSet( wk->bgl, GF_BGL_FRAME2_S, wk->view.p_userscrn[wk->view.user_dispno]->rawData,
			wk->view.p_userscrn[wk->view.user_dispno]->szByte );

	GF_BGL_ScrPalChange( wk->bgl, GF_BGL_FRAME2_S, 0, 0,
			32, 24, pal );

	// BG�R�ʂ̃X�N���[�����N���A
    GF_BGL_ScrClear(wk->bgl, GF_BGL_FRAME3_S);

	// �t�����e�B�A��\�����[�h�`�F�b�N
	if( _frontierInCheck( wk ) == FALSE ){
		MCVSys_UserDispDrawFrontierOffScrn( wk );	// ��\���X�N���[���ݒ�
	}
	

	// ���̐l�̂��Ƃ�`��
    GF_BGL_BmpWinDataFill( &wk->view.userWin, 0x0 );

	// �`��
	writeFunc[ wk->view.user_dispno ]( wk, heapID );

	GF_BGL_LoadScreenV_Req( wk->bgl, GF_BGL_FRAME2_S );
    GF_BGL_BmpWinOnVReq(&wk->view.userWin);

	// ���b�Z�[�W�ʂ̕\���ݒ�
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );	
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );
}

// �ʏ�
static void MCVSys_UserDispDrawType00( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int sex;
	int col;
	int msg_id;
	int friendNo;
	int num;
	int vct_icon;
	_WIFI_MACH_STATUS* p_status;
	u32 status;
	
	friendNo = wk->view.touch_friendNo - 1;

	sex = WifiList_GetFriendInfo( wk->pList, friendNo, WIFILIST_FRIEND_SEX );

	// �g���[�i�[��
	if( sex == PM_MALE ){
		col = _COL_N_BLUE;
	}else{
		col = _COL_N_RED;
	}
    MCVSys_FriendNameSet(wk, friendNo);
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_033, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
    GF_STR_PrintColor( &wk->view.userWin, FONT_TALK, wk->TitleString, 
			MCV_USERD_NAME_X, MCV_USERD_NAME_Y, MSG_NO_PUT, col, NULL);

	p_status = WifiFriendMatchStatusGet( wk, friendNo );
	status = _WifiMyStatusGet( wk, p_status );

	// ���
	msg_id = MCVSys_StatusMsgIdGet( status, &col );
    MSGMAN_GetString(wk->MsgManager, msg_id, wk->pExpStrBuf);
	GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->pExpStrBuf,
					   MCV_USERD_ST_X, MCV_USERD_ST_Y,
					   MSG_NO_PUT, col, NULL);

	// �O���[�v
    {
        MYSTATUS* pTarget = MyStatus_AllocWork(HEAPID_WIFIP2PMATCH);
        MyStatus_SetMyName(pTarget, WifiList_GetFriendGroupNamePtr(wk->pList,friendNo));
        WORDSET_RegisterPlayerName( wk->view.p_wordset, 0, pTarget);
        sys_FreeMemoryEz(pTarget);
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_034, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_GR_X, MCV_USERD_GR_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
    }

    // �ΐ퐬��
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_035, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, 
				MCV_USERD_VS_X, MCV_USERD_VS_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
		// ����
		num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_BATTLE_WIN);
		WORDSET_RegisterNumber(wk->view.p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_036, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_VS_WIN_X, MCV_USERD_VS_WIN_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
		// �܂�
		num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_BATTLE_LOSE);
		WORDSET_RegisterNumber(wk->view.p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_037, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, 
				MCV_USERD_VS_LOS_X, MCV_USERD_VS_WIN_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
	}
	// �|�P��������
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_038, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_TR_X,  MCV_USERD_TR_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_TRADE_NUM);
		WORDSET_RegisterNumber(wk->view.p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_039, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );

		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_TRNUM_X, MCV_USERD_TR_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
	}

	// ������
	if( _pofinCaseCheck( wk ) == TRUE ){
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_102, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_POFIN_X,  MCV_USERD_POFIN_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_POFIN_NUM);
		WORDSET_RegisterNumber(wk->view.p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_103, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );

		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_POFINNUM_X, MCV_USERD_POFIN_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

	}
	
	// �Ō�ɗV�񂾓��t
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_040, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, 
				MCV_USERD_DAY_X, MCV_USERD_DAY_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_LASTBT_DAY);
		if(num!=0){
			WORDSET_RegisterNumber(wk->view.p_wordset, 2, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_LASTBT_YEAR);
			WORDSET_RegisterNumber(wk->view.p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			num = WifiList_GetFriendInfo(wk->pList, friendNo, WIFILIST_FRIEND_LASTBT_MONTH);
			WORDSET_RegisterNumber(wk->view.p_wordset, 1, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_041, wk->pExpStrBuf );
			WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
			GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, 
					MCV_USERD_DAYNUM_X, MCV_USERD_DAY_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
		}
	}

	// �A�C�R��
	WifiP2PMatchFriendListStIconWrite( wk->bgl, &wk->icon, GF_BGL_FRAME2_S, 
			MCV_USERD_ICON_X, MCV_USERD_ICON_Y,
			status );
	if( p_status->vchat ){
		vct_icon = PLAYER_DISP_ICON_IDX_NONE;
	}else{
		vct_icon = PLAYER_DISP_ICON_IDX_VCTNOT;
	}
	WifiP2PMatchFriendListIconWrite( wk->bgl, &wk->icon, GF_BGL_FRAME2_S, 
			MCV_USERD_VCTICON_X, MCV_USERD_ICON_Y,
			vct_icon, 0 );

}

// �o�g���^���[
static void MCVSys_UserDispDrawType01( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int friendNo;

	friendNo = wk->view.touch_friendNo - 1;

	// �^�C�g��
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf01, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTTW_TITLE_X,  MCV_USERD_BTTW_TITLE_Y, MSG_NO_PUT, _COL_N_WHITE, NULL);
	}

	// �O��L�^
	{
		
		MCVSys_UserDispFrontiorTitleStrGet( wk, wk->TitleString, MCV_FRONTIOR_TOWOR, friendNo );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTTW_LAST_X,  MCV_USERD_BTTW_LAST_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_TOWER_MULTI_WIFI_RENSHOU_CNT, friendNo, 
				MCV_USERD_BTTW_LASTNUM_X, MCV_USERD_BTTW_LASTNUM_Y );
	}

	// �ō��L�^
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf07, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTTW_MAX_X,  MCV_USERD_BTTW_MAX_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_TOWER_MULTI_WIFI_RENSHOU, friendNo, 
				MCV_USERD_BTTW_MAXNUM_X, MCV_USERD_BTTW_MAXNUM_Y );
	}
}

// �o�g���t�@�N�g���[
static void MCVSys_UserDispDrawType02( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int friendNo;

	friendNo = wk->view.touch_friendNo - 1;

	// �^�C�g��
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf02, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTTW_TITLE_X,  MCV_USERD_BTTW_TITLE_Y, MSG_NO_PUT, _COL_N_WHITE, NULL);
	}

	// LV50
	{
		// �^�C�g��
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf08, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_LV50_X,  MCV_USERD_BTFC_LV50_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf10, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_LV50K_X,  MCV_USERD_BTFC_LV50K_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		// �O��
		MCVSys_UserDispFrontiorTitleStrGet( wk, wk->TitleString, MCV_FRONTIOR_FACTORY, friendNo );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_LV50LAST_X,  MCV_USERD_BTFC_LV50LAST_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_FACTORY_MULTI_WIFI_RENSHOU_CNT, friendNo, 
				MCV_USERD_BTFC_LV50LASTNUM_X, MCV_USERD_BTFC_LV50LASTNUM_Y );

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf14, 
				FRID_FACTORY_MULTI_WIFI_TRADE_CNT, friendNo, 
				MCV_USERD_BTFC_LV50LASTTRNUM_X, MCV_USERD_BTFC_LV50LASTTRNUM_Y );

		// �ō�
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf07, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_LV50MAX_X,  MCV_USERD_BTFC_LV50MAX_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_FACTORY_MULTI_WIFI_RENSHOU, friendNo, 
				MCV_USERD_BTFC_LV50MAXNUM_X, MCV_USERD_BTFC_LV50MAXNUM_Y );

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf14, 
				FRID_FACTORY_MULTI_WIFI_TRADE, friendNo, 
				MCV_USERD_BTFC_LV50MAXTRNUM_X, MCV_USERD_BTFC_LV50MAXTRNUM_Y );
	}

	// OPEN
	{
		// �^�C�g��
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf09, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_OPN_X,  MCV_USERD_BTFC_OPN_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf10, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_OPNK_X,  MCV_USERD_BTFC_OPNK_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		// �O��
		MCVSys_UserDispFrontiorTitleStrGet( wk, wk->TitleString, MCV_FRONTIOR_FACTORY100, friendNo );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_OPNLAST_X,  MCV_USERD_BTFC_OPNLAST_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_FACTORY_MULTI_WIFI_RENSHOU100_CNT, friendNo, 
				MCV_USERD_BTFC_OPNLASTNUM_X, MCV_USERD_BTFC_OPNLASTNUM_Y );

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf14, 
				FRID_FACTORY_MULTI_WIFI_TRADE100_CNT, friendNo, 
				MCV_USERD_BTFC_OPNLASTTRNUM_X, MCV_USERD_BTFC_OPNLASTTRNUM_Y );

		// �ō�
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf07, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTFC_OPNMAX_X,  MCV_USERD_BTFC_OPNMAX_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_FACTORY_MULTI_WIFI_RENSHOU100, friendNo, 
				MCV_USERD_BTFC_OPNMAXNUM_X, MCV_USERD_BTFC_OPNMAXNUM_Y );

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf14, 
				FRID_FACTORY_MULTI_WIFI_TRADE100, friendNo, 
				MCV_USERD_BTFC_OPNMAXTRNUM_X, MCV_USERD_BTFC_OPNMAXTRNUM_Y );
	}
}

// �o�g���L���b�X��
static void MCVSys_UserDispDrawType03( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int friendNo;

	friendNo = wk->view.touch_friendNo - 1;

	// �^�C�g��
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf03, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTKS_TITLE_X,  MCV_USERD_BTKS_TITLE_Y, MSG_NO_PUT, _COL_N_WHITE, NULL);

		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf11, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTKS_K_X,  MCV_USERD_BTKS_K_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
	}

	// �O��L�^
	{
		MCVSys_UserDispFrontiorTitleStrGet( wk, wk->TitleString, MCV_FRONTIOR_CASTLE, friendNo );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTKS_LAST_X,  MCV_USERD_BTKS_LAST_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf16, 
				FRID_CASTLE_MULTI_WIFI_RENSHOU_CNT, friendNo, 
				MCV_USERD_BTKS_LASTNUM_X, MCV_USERD_BTKS_LASTNUM_Y );

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf15, 
				FRID_CASTLE_MULTI_WIFI_CP, friendNo, 
				MCV_USERD_BTKS_LASTCPNUM_X, MCV_USERD_BTKS_LASTCPNUM_Y );
	}

	// �ō��L�^
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf07, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTKS_MAX_X,  MCV_USERD_BTKS_MAX_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf16, 
				FRID_CASTLE_MULTI_WIFI_RENSHOU, friendNo, 
				MCV_USERD_BTKS_MAXNUM_X, MCV_USERD_BTKS_MAXNUM_Y );

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf15, 
				FRID_CASTLE_MULTI_WIFI_REMAINDER_CP, friendNo, 
				MCV_USERD_BTKS_MAXCPNUM_X, MCV_USERD_BTKS_MAXCPNUM_Y );
	}
}

// �o�g���X�e�[�W
static void MCVSys_UserDispDrawType04( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int friendNo;
	int num;
	FRONTIER_SAVEWORK* p_fsave;
	STRBUF* p_str;

	p_fsave = SaveData_GetFrontier( wk->pSaveData );

	friendNo = wk->view.touch_friendNo - 1;

	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf04, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTST_TITLE_X,  MCV_USERD_BTST_TITLE_Y, MSG_NO_PUT, _COL_N_WHITE, NULL);

		num = FrontierRecord_Get( p_fsave, FRID_STAGE_MULTI_WIFI_MONSNO, friendNo );
		p_str = MSGDAT_UTIL_GetMonsName( num, heapID );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, p_str, MCV_USERD_BTST_K_X,  MCV_USERD_BTST_K_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);
		STRBUF_Delete( p_str );
	}

	// �O��L�^
	{
		MCVSys_UserDispFrontiorTitleStrGet( wk, wk->TitleString, MCV_FRONTIOR_STAGE, friendNo );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTST_LAST_X,  MCV_USERD_BTST_LAST_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_STAGE_MULTI_WIFI_RENSHOU_CNT, friendNo, 
				MCV_USERD_BTST_LASTNUM_X, MCV_USERD_BTST_LASTNUM_Y );
	}

	// �ō��L�^
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf07, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTST_MAX_X,  MCV_USERD_BTST_MAX_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf13, 
				FRID_STAGE_MULTI_WIFI_RENSHOU, friendNo, 
				MCV_USERD_BTST_MAXNUM_X, MCV_USERD_BTST_MAXNUM_Y );
	}
}

// �o�g�����[���b�g
static void MCVSys_UserDispDrawType05( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int friendNo;

	friendNo = wk->view.touch_friendNo - 1;

	// �^�C�g��
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf05, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTRT_TITLE_X,  MCV_USERD_BTRT_TITLE_Y, MSG_NO_PUT, _COL_N_WHITE, NULL);
	}

	// �O��L�^
	{
		MCVSys_UserDispFrontiorTitleStrGet( wk, wk->TitleString, MCV_FRONTIOR_ROULETTE, friendNo );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTRT_LAST_X,  MCV_USERD_BTRT_LAST_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf17, 
				FRID_ROULETTE_MULTI_WIFI_RENSHOU_CNT, friendNo, 
				MCV_USERD_BTRT_LASTNUM_X, MCV_USERD_BTRT_LASTNUM_Y );
	}

	// �ō��L�^
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_bf07, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_BTRT_MAX_X,  MCV_USERD_BTRT_MAX_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispFrontiorNumDraw( wk, msg_wifilobby_bf17, 
				FRID_ROULETTE_MULTI_WIFI_RENSHOU, friendNo, 
				MCV_USERD_BTRT_MAXNUM_X, MCV_USERD_BTRT_MAXNUM_Y );
	}
}

// �~�j�Q�[��
static void MCVSys_UserDispDrawType06( WIFIP2PMATCH_WORK *wk, u32 heapID )
{
	int friendNo;
	WIFI_LIST* p_list;

	p_list	= SaveData_GetWifiListData( wk->pSaveData );

	friendNo = wk->view.touch_friendNo - 1;

	// �^�C�g��
	{
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_mg01, wk->TitleString );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_MINI_TITLE_X,  MCV_USERD_MINI_TITLE_Y, MSG_NO_PUT, _COL_N_WHITE, NULL);
	}

	// ���܂���
	{
		WORDSET_RegisterWiFiLobbyGameName( wk->view.p_wordset, 0, WFLBY_GAME_BALLSLOW );
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_mg02, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_MINI_BC_X,  MCV_USERD_MINI_BC_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispNumDraw( wk, msg_wifilobby_mg05, 
				WifiList_GetFriendInfo( p_list, friendNo, WIFILIST_FRIEND_BALLSLOW_NUM ), 
				MCV_USERD_MINI_BCNUM_X, MCV_USERD_MINI_BCNUM_Y );
	}

	// ���܂̂�
	{
		WORDSET_RegisterWiFiLobbyGameName( wk->view.p_wordset, 0, WFLBY_GAME_BALANCEBALL );
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_mg02, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_MINI_BB_X,  MCV_USERD_MINI_BB_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispNumDraw( wk, msg_wifilobby_mg05, 
				WifiList_GetFriendInfo( p_list, friendNo, WIFILIST_FRIEND_BALANCEBALL_NUM ),
				MCV_USERD_MINI_BBNUM_X, MCV_USERD_MINI_BBNUM_Y );
	}

	// �ӂ�����
	{
		WORDSET_RegisterWiFiLobbyGameName( wk->view.p_wordset, 0, WFLBY_GAME_BALLOON );
		MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_mg02, wk->pExpStrBuf );
		WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
		GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, MCV_USERD_MINI_BL_X,  MCV_USERD_MINI_BL_Y, MSG_NO_PUT, _COL_N_BLACK, NULL);

		MCVSys_UserDispNumDraw( wk, msg_wifilobby_mg05, 
				WifiList_GetFriendInfo( p_list, friendNo, WIFILIST_FRIEND_BALLOON_NUM ),
				MCV_USERD_MINI_BLNUM_X, MCV_USERD_MINI_BLNUM_Y );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�����e�B�A��\�����[�h�̃X�N���[���ɕύX
 *
 *	@param	wk		���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispDrawFrontierOffScrn( WIFIP2PMATCH_WORK *wk )
{
	int i;
	int roop;

	// �t�����e�B�A�{�ݐ��擾
	roop = WF_USERDISPTYPE_NUM - WF_USERDISPTYPE_BLTW; 

	for( i=0; i<roop; i++ ){
		GF_BGL_ScrWriteExpand( wk->bgl, GF_BGL_FRAME2_S,
				MCV_USERD_NOFR_SCRN_X+(MCV_USERD_NOFR_SCRN_SIZX*i), MCV_USERD_NOFR_SCRN_Y, 
				MCV_USERD_NOFR_SCRN_SIZX, MCV_USERD_NOFR_SCRN_SIZY, 
				wk->view.p_useretcscrn->rawData,
				0, 0, 
				wk->view.p_useretcscrn->screenWidth/8, wk->view.p_useretcscrn->screenHeight/8 );
	}

	// �p���b�g�ύX
	GF_BGL_ScrPalChange( wk->bgl, GF_BGL_FRAME2_S, 
			MCV_USERD_NOFR_SCRN_X, MCV_USERD_NOFR_SCRN_Y,
			(MCV_USERD_NOFR_SCRN_SIZX*roop), MCV_USERD_NOFR_SCRN_SIZY, 
			MCV_PAL_BTTN+MCV_PAL_BTTN_NONE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�@�N�g���[�f�[�^�\��
 *
 *	@param	wk			���[�N
 *	@param	strid		����ID
 *	@param	factoryid	�t�@�N�g���[�f�[�^ID
 *	@param	friendno	�F�B�ԍ�
 *	@param	x			�\��X
 *	@param	y			�\��Y
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispFrontiorNumDraw( WIFIP2PMATCH_WORK *wk, u32 strid, u32 factoryid, u32 friendno, u32 x, u32 y )
{
	int num;
	FRONTIER_SAVEWORK* p_fsave;

	p_fsave = SaveData_GetFrontier( wk->pSaveData );

	num = FrontierRecord_Get(p_fsave, factoryid, friendno);
	
	MCVSys_UserDispNumDraw( wk, strid, num, x, y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�����e�B�A�@�^�C�g�����b�Z�[�W�擾
 *
 *	@param	wk			���[�N
 *	@param	p_str		������i�[��
 *	@param	factoryid	�t�@�N�g���[�@�Z�[�u�f�[�^ID
 *	@param	friendno	�F�B�i���o�[
 *
 *	factoryid
 *		MCV_FRONTIOR_TOWOR,
 *		MCV_FRONTIOR_FACTORY,
 *		MCV_FRONTIOR_FACTORY100,
 *		MCV_FRONTIOR_CASTLE,
 *		MCV_FRONTIOR_STAGE,
 *		MCV_FRONTIOR_ROULETTE,
 *		
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispFrontiorTitleStrGet( WIFIP2PMATCH_WORK *wk, STRBUF* p_str, u32 factoryid, u32 friendno )
{
	FRONTIER_SAVEWORK* p_fsave;
	BOOL clear_flag;
	u32 stridx;
	static const u32 sc_MCVSYS_FRONTIOR_CLEAR_BIT[ MCV_FRONTIOR_NUM ] = {
		FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
		FRID_FACTORY_MULTI_WIFI_CLEAR_BIT,
		FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT,
		FRID_CASTLE_MULTI_WIFI_CLEAR_BIT,
		FRID_STAGE_MULTI_WIFI_CLEAR_BIT,
		FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
	};

	GF_ASSERT( factoryid < MCV_FRONTIOR_NUM );

	p_fsave		= SaveData_GetFrontier( wk->pSaveData );

	clear_flag = FrontierRecord_Get(p_fsave, sc_MCVSYS_FRONTIOR_CLEAR_BIT[factoryid], friendno);
	if( clear_flag == FALSE ){
		stridx = msg_wifilobby_bf06;
	}else{
		stridx = msg_wifilobby_bf18;
	}
	MSGMAN_GetString(  wk->MsgManager, stridx, p_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������\�����鏈��
 *
 *	@param	wk		���[�N
 *	@param	strid	���b�Z�[�WID
 *	@param	num		����
 *	@param	x		���h�b�g���W
 *	@param	y		���h�b�g���W
 */
//-----------------------------------------------------------------------------
static void MCVSys_UserDispNumDraw( WIFIP2PMATCH_WORK *wk, u32 strid, u32 num, u32 x, u32 y )
{
	WORDSET_RegisterNumber(wk->view.p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
	MSGMAN_GetString(  wk->MsgManager, strid, wk->pExpStrBuf );
	WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );

	GF_STR_PrintColor( &wk->view.userWin, FONT_SYSTEM, wk->TitleString, x, y, MSG_NO_PUT, _COL_N_BLACK, NULL);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���`��
 *
 *	@param	wk		�V�X�e�����[�N
 *	@param	cx		���L�������W
 *	@param	cy		���L�������W
 *	@param	type	�{�^���^�C�v
 *	@param	frame	�t���[��
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnWrite( WIFIP2PMATCH_WORK *wk, u8 cx, u8 cy, u8 type, u8 frame )
{
	u16 reed_x, reed_y;
	u16* p_buff;

	GF_ASSERT( frame < 4 );

	// frame 3�̂Ƃ���1��\������
	if( frame == 3 ){
		frame = 1;
	}

	if( (frame < 2) ){
		reed_x = MCV_BUTTON_SIZX * frame;
		reed_y = MCV_BUTTON_SIZY * type;
		
		GF_BGL_ScrWriteExpand( wk->bgl, GF_BGL_FRAME2_S,
				cx, cy, MCV_BUTTON_SIZX, MCV_BUTTON_SIZY, 
				wk->view.p_bttnscrn->rawData,
				reed_x, reed_y, 
				32, 18 );
	}else{
		reed_x = 0;
		reed_y = MCV_BUTTON_SIZY * type;
		p_buff = (u16*)wk->view.p_bttnscrn->rawData;
		
		GF_BGL_ScrWriteExpand( wk->bgl, GF_BGL_FRAME2_S,
				cx, cy, MCV_BUTTON_SIZX, MCV_BUTTON_SIZY, 
				&p_buff[ 32*18 ],
				reed_x, reed_y, 
				16, 18 );
	}

	// �p���b�g�J���[�����킹��
	GF_BGL_ScrPalChange( wk->bgl, GF_BGL_FRAME2_S, cx, cy,
			MCV_BUTTON_SIZX, MCV_BUTTON_SIZY, type+MCV_PAL_BTTN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ԂɑΉ��������b�Z�[�W�f�[�^�\��
 *
 *	@param	status	���
 *
 *	@return	���b�Z�[�W�f�[�^
 */
//-----------------------------------------------------------------------------
static u32 MCVSys_StatusMsgIdGet( u32 status, int* col )
{
	u32 msg_id;
	*col = _COL_N_BLACK;
    switch(status){
      case WIFI_STATUS_VCT:      // VCT��
		msg_id = msg_wifilobby_027;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_SBATTLE50:      // �ΐ풆
      case WIFI_STATUS_SBATTLE100:      // �ΐ풆
      case WIFI_STATUS_SBATTLE_FREE:      // �ΐ풆
      case WIFI_STATUS_DBATTLE50:      // �ΐ풆
      case WIFI_STATUS_DBATTLE100:      // �ΐ풆
      case WIFI_STATUS_DBATTLE_FREE:      // �ΐ풆
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE:      // �ΐ풆
#endif
		msg_id = msg_wifilobby_024;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_SBATTLE50_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_060;
        break;
      case WIFI_STATUS_SBATTLE100_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_061;
        break;
      case WIFI_STATUS_SBATTLE_FREE_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_059;
        break;
      case WIFI_STATUS_DBATTLE50_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_063;
        break;
      case WIFI_STATUS_DBATTLE100_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_064;
        break;
      case WIFI_STATUS_DBATTLE_FREE_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_062;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_MBATTLE_FREE_WAIT:   // �ΐ��W��
		msg_id = msg_wifilobby_062;
        break;
#endif
      case WIFI_STATUS_TRADE:          // ������
		msg_id = msg_wifilobby_026;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_TRADE_WAIT:    // ������W��
		msg_id = msg_wifilobby_025;
        break;
      case WIFI_STATUS_POFIN:          // �|�t�B����
		msg_id = msg_wifilobby_101;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_POFIN_WAIT:    // �|�t�B����W��
		msg_id = msg_wifilobby_100;
        break;
      case WIFI_STATUS_FRONTIER:          // �t�����e�B�A��
		msg_id = msg_wifilobby_139;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_FRONTIER_WAIT:    // �t�����e�B�A��W��
		msg_id = msg_wifilobby_138;
        break;
#ifdef WFP2P_DEBUG_EXON 
      case WIFI_STATUS_BATTLEROOM:          // �o�g�����[����
		msg_id = msg_wifilobby_debug_00;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_BATTLEROOM_WAIT:    // �o�g�����[����W��
		msg_id = msg_wifilobby_debug_00;
        break;
#endif
      case WIFI_STATUS_BUCKET:          // �o�P�b�g��
      case WIFI_STATUS_BALANCEBALL:          // �o�P�b�g��
      case WIFI_STATUS_BALLOON:          // �o�P�b�g��
		msg_id = msg_wifilobby_148;
		*col = _COL_N_GRAY;
        break;
      case WIFI_STATUS_BUCKET_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALANCEBALL_WAIT:    // �o�P�b�g��W��
      case WIFI_STATUS_BALLOON_WAIT:    // �o�P�b�g��W��
		msg_id = msg_wifilobby_147;
        break;
      case WIFI_STATUS_LOGIN_WAIT:    // �ҋ@���@���O�C������͂���
		msg_id = msg_wifilobby_046;
        break;
      default:
		msg_id = msg_wifilobby_056;
		break;
    }
	return msg_id;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^����̃g���[�i�[���̕\��
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	p_bmp		�r�b�g�}�b�v
 *	@param	friendNo	�F�B�i���o�[
 *	@param	frame		�{�^���t���[��
 *	@param	area_id		�z�uID
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnWinDraw( WIFIP2PMATCH_WORK *wk, GF_BGL_BMPWIN* p_bmp, u32 friendNo, u32 frame, u32 area_id )
{
	int sex;
	int col;
	int y;

	// ��ʃN���[��
	GF_BGL_BmpWinDataFill( p_bmp, 0 );

	//y���W���擾
	y = ViewButtonFrame_y[ frame ];

	sex = WifiList_GetFriendInfo( wk->pList, friendNo-1, WIFILIST_FRIEND_SEX );
	// �g���[�i�[��
	if( sex == PM_MALE ){
		col = _COL_N_BLUE;
	}else{
		col = _COL_N_RED;
	}
    MCVSys_FriendNameSet(wk, friendNo-1);
    MSGMAN_GetString(  wk->MsgManager, msg_wifilobby_033, wk->pExpStrBuf );
    WORDSET_ExpandStr( wk->view.p_wordset, wk->TitleString, wk->pExpStrBuf );
    GF_STR_PrintColor( p_bmp, FONT_TALK, wk->TitleString, 
			0, y, MSG_NO_PUT, col, NULL);

	GF_BGL_BmpWinOnVReq( p_bmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ԃE�B���h�E�̕`��
 *
 *	@param	wk			�����Ăރ��[�N
 *	@param	p_stbmp		��ԃr�b�g�}�b�v
 *	@param	friendNo	�F�B�ԍ�
 *	@param	frame		�{�^���t���[��
 *	@param	area_id		�z�uID
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnStatusWinDraw( WIFIP2PMATCH_WORK *wk, GF_BGL_BMPWIN* p_stbmp, u32 friendNo, u32 frame, u32 area_id )
{
	int y;
	int i;
	int vct_icon;
	_WIFI_MACH_STATUS* p_status;
	u32 status;

	p_status = WifiFriendMatchStatusGet( wk, friendNo - 1 );
	status = _WifiMyStatusGet( wk, p_status );

	// �A�C�R���\��
	y = ViewButtonFrame_y[ frame ];

	for( i=0; i<WF_VIEW_STATUS_NUM; i++ ){
		// ��ʃN���[��
		GF_BGL_BmpWinDataFill( &p_stbmp[i], 0 );
		if( i==0 ){
			WifiP2PMatchFriendListBmpStIconWrite( &p_stbmp[i], &wk->icon, 
					0, y,
					status );
		}else{

			if( p_status->vchat ){
				vct_icon = PLAYER_DISP_ICON_IDX_NONE;
			}else{
				vct_icon = PLAYER_DISP_ICON_IDX_VCTNOT;
			}
			WifiP2PMatchFriendListBmpIconWrite( &p_stbmp[i], &wk->icon, 
					0, y,
					vct_icon, 0 );
		}
		GF_BGL_BmpWinOnVReq( &p_stbmp[i] );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���A�j�����C��
 *
 *	@param	wk		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_BttnAnmMan( WIFIP2PMATCH_WORK *wk )
{
	static const u8 BttnAnmFrame[ MCV_BUTTON_FRAME_NUM ] = {
		0, 1, 2, 1
	};

	// ���삵�Ă��邩�`�F�b�N
	if( wk->view.touch_friendNo == 0 ){
		return ;
	}

	// ����
	wk->view.button_count ++;
	if( wk->view.button_count >= BttnAnmFrame[ wk->view.touch_frame ] ){
		wk->view.button_count = 0;
		wk->view.touch_frame ++;

		wk->view.button_on = TRUE;

		// �I���`�F�b�N
		if( wk->view.touch_frame >= MCV_BUTTON_FRAME_NUM ){
			wk->view.button_count = 0;
			wk->view.touch_frame = 0;
			wk->view.user_disp = MCV_USERDISP_INIT;
		}
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u��ʁ@OAM�{�^���@������
 *
 *	@param	wk			���[�N
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnInit( WIFIP2PMATCH_WORK *wk, ARCHANDLE* p_handle, u32 heapID )
{
	BOOL result;
	int i;
	CLACT_HEADER head;
	GF_BGL_BMPWIN bttn_font;
	STRBUF* p_str;
	FONTOAM_OAM_DATA_PTR p_fontoam_data;
	int fontoam_cgsize;
	FONTOAM_INIT fontoam_add;
	CLACT_ADD add[ MCV_USERD_BTTN_NUM ] = {
		{	// LEFT
			NULL, NULL,
			{ FX32_CONST(MCV_USERD_BTTN_LEFT_X), FX32_CONST(MCV_USERD_BTTN_Y)+NAMEIN_SUB_ACTOR_DISTANCE, 0 },
			{ FX32_ONE, FX32_ONE, FX32_ONE },
			0, MCV_USERD_BTTN_PRI, NNS_G2D_VRAM_TYPE_2DSUB,
			0
		},
		{	// BACK
			NULL, NULL,
			{ FX32_CONST(MCV_USERD_BTTN_BACK_X), FX32_CONST(MCV_USERD_BTTN_Y)+NAMEIN_SUB_ACTOR_DISTANCE, 0 },
			{ FX32_ONE, FX32_ONE, FX32_ONE },
			0, MCV_USERD_BTTN_PRI, NNS_G2D_VRAM_TYPE_2DSUB,
			0
		},
		{	// RIGHT
			NULL, NULL,
			{ FX32_CONST(MCV_USERD_BTTN_RIGHT_X), FX32_CONST(MCV_USERD_BTTN_Y)+NAMEIN_SUB_ACTOR_DISTANCE, 0 },
			{ FX32_ONE, FX32_ONE, FX32_ONE },
			0, MCV_USERD_BTTN_PRI, NNS_G2D_VRAM_TYPE_2DSUB,
			0
		},
	};

	
	// �ꉞ�\����OFF
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );
//	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
	
	// ���\�[�X�Ǎ���
	wk->view.button_res[0] = CLACT_U_ResManagerResAddArcChar_ArcHandle(
			wk->resMan[0], p_handle, NARC_wifip2pmatch_wf_match_btm_oam_NCGR,
			FALSE, MCV_USERD_BTTN_RESCONTID,
			NNS_G2D_VRAM_TYPE_2DSUB, heapID );
	wk->view.button_res[1] = CLACT_U_ResManagerResAddArcPltt_ArcHandle(
			wk->resMan[1], p_handle, NARC_wifip2pmatch_wf_match_btm_oam_NCLR,
			FALSE, MCV_USERD_BTTN_RESCONTID,
			NNS_G2D_VRAM_TYPE_2DSUB, 8, heapID );
	wk->view.button_res[2] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
			wk->resMan[2], p_handle, NARC_wifip2pmatch_wf_match_btm_oam_NCER,
			FALSE, MCV_USERD_BTTN_RESCONTID, CLACT_U_CELL_RES,
			heapID );
	wk->view.button_res[3] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle(
			wk->resMan[3], p_handle, NARC_wifip2pmatch_wf_match_btm_oam_NANR,
			FALSE, MCV_USERD_BTTN_RESCONTID, CLACT_U_CELLANM_RES,
			heapID );

	// VRAM�W�J
	result = CLACT_U_CharManagerSetCharModeAdjustAreaCont( wk->view.button_res[0] );
	GF_ASSERT( result );
	result = CLACT_U_PlttManagerSetCleanArea( wk->view.button_res[1] );
	GF_ASSERT( result );
	CLACT_U_ResManagerResOnlyDelete( wk->view.button_res[0] );
	CLACT_U_ResManagerResOnlyDelete( wk->view.button_res[1] );

	// �w�b�_�[�쐬
	CLACT_U_MakeHeader( &head, 
			MCV_USERD_BTTN_RESCONTID, MCV_USERD_BTTN_RESCONTID,
			MCV_USERD_BTTN_RESCONTID, MCV_USERD_BTTN_RESCONTID,
			CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
			0, MCV_USERD_BTTN_BGPRI,
			wk->resMan[0], wk->resMan[1], wk->resMan[2], wk->resMan[3],
			NULL, NULL );
		

	// �A�N�^�[�쐬
	for( i=0; i<MCV_USERD_BTTN_NUM; i++ ){
		add[i].ClActSet = wk->clactSet;
		add[i].ClActHeader = &head;
		add[i].heap = heapID;
		wk->view.button_act[i] = CLACT_Add( &add[i] );
		CLACT_AnmChg( wk->view.button_act[i], c_MCV_USER_BTTN_ANM_RELEASE[i] );
	}

	// �{�^���t�H���g�Ǎ���
	FontProc_LoadFont( FONT_BUTTON, heapID );

	// ���ǂ�@�`��p�̈�쐬
	GF_BGL_BmpWinInit( &bttn_font );
	GF_BGL_BmpWinObjAdd( wk->bgl, &bttn_font, 
			MCV_USERD_BTTN_FONT_SIZX, MCV_USERD_BTTN_FONT_SIZY,
			MCV_USERD_BTTN_FONT_CGXOFS, MCV_USERD_BTTN_FONT_COL );

	// ������擾
	p_str = STRBUF_Create( 64, heapID );
	MSGMAN_GetString( wk->MsgManager, msg_wifilobby_143, p_str );

	// ���������������
	GF_STR_PrintColor( &bttn_font, FONT_BUTTON, p_str, 0, 0, MSG_NO_PUT, GF_PRINTCOLOR_MAKE(1,2,0), NULL );

	STRBUF_Delete( p_str );

	// ������OAM�f�[�^�쐬
	p_fontoam_data = FONTOAM_OAMDATA_Make( &bttn_font, heapID );

	// �T�C�Y�擾
	fontoam_cgsize = FONTOAM_OAMDATA_NeedCharSize( p_fontoam_data, NNS_G2D_VRAM_TYPE_2DSUB );

	// CharAreaAlloc
	result = CharVramAreaAlloc( fontoam_cgsize, CHARM_CONT_AREACONT,
			NNS_G2D_VRAM_TYPE_2DSUB, &wk->view.back_fontoam_cg );
	GF_ASSERT(result);

	// fontoam�쐬
	fontoam_add.fontoam_sys = wk->fontoam;
	fontoam_add.bmp = &bttn_font;
	fontoam_add.clact_set = wk->clactSet;
	fontoam_add.pltt = CLACT_U_PlttManagerGetProxy( wk->view.button_res[1], NULL );
	fontoam_add.parent = wk->view.button_act[MCV_USERD_BTTN_BACK];
	fontoam_add.char_ofs = wk->view.back_fontoam_cg.alloc_ofs;
	fontoam_add.x = MCV_USERD_BTTN_FONT_X;
	fontoam_add.y = MCV_USERD_BTTN_FONT_Y;
	fontoam_add.bg_pri = MCV_USERD_BTTN_BGPRI;
	fontoam_add.soft_pri = MCV_USERD_BTTN_FONT_OAMPRI;
	fontoam_add.draw_area = NNS_G2D_VRAM_TYPE_2DSUB;
	fontoam_add.heap = heapID;
	wk->view.back_fontoam = FONTOAM_OAMDATA_Init( &fontoam_add, p_fontoam_data );

	// OAM�\���f�[�^�j��
	FONTOAM_OAMDATA_Free( p_fontoam_data );

	// BMP�j��
	GF_BGL_BmpWinDel( &bttn_font );
	
	// ������t�H���g�j��
	FontProc_UnloadFont( FONT_BUTTON );

	// �{�^�������蔻��f�[�^�쐬
	wk->view.p_oambttnman = BMN_Create( c_MCV_USER_BTTN_OAM_Hit, MCV_USERD_BTTN_NUM, 
			MCVSys_OamBttnCallBack, wk, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���f�[�^�̔j��
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnDelete( WIFIP2PMATCH_WORK *wk )
{
	int i;

	// �{�^���}�l�[�W���j��
	BMN_Delete( wk->view.p_oambttnman );

	// �t�H���gOAM�j��
	FONTOAM_OAMDATA_Delete( wk->view.back_fontoam );

	// �L�����̈�j��
	CharVramAreaFree( &wk->view.back_fontoam_cg );
	
	// ���[�N�̔j��
	for( i=0; i<MCV_USERD_BTTN_NUM; i++ ){
		CLACT_Delete( wk->view.button_act[i] );
	}

	// ���\�[�X�j��
	CLACT_U_CharManagerDelete( wk->view.button_res[0] );
	CLACT_U_PlttManagerDelete( wk->view.button_res[1] );
	for( i=0; i<CLACT_RESOURCE_NUM; i++ ){
		CLACT_U_ResManagerResDelete( wk->resMan[i], wk->view.button_res[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	OAM�{�^��ON
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnOn( WIFIP2PMATCH_WORK *wk )
{
	int i;
	
	// �\����Ԃɂ���
	wk->view.buttonact_on = MCV_USERD_BTTN_MODE_NML;

	// �\��ON
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );

	// �p�����[�^������
	wk->view.touch_button = 0;

	// �A�j��������
	for( i=0; i<MCV_USERD_BTTN_NUM; i++ ){
		MCVSys_OamBttnObjAnmStart( &wk->view, i );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���\���@BACK��\�����[�h	
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnOnNoBack( WIFIP2PMATCH_WORK *wk )
{
	MCVSys_OamBttnOn( wk );

	// ���ǂ邾����\��
	CLACT_SetDrawFlag( wk->view.button_act[MCV_USERD_BTTN_BACK], FALSE );
	FONTOAM_SetDrawFlag( wk->view.back_fontoam, FALSE );

	// �\����Ԃɂ���
	wk->view.buttonact_on = MCV_USERD_BTTN_MODE_NOBACK;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^��OFF
 *
 *	@param	wk 
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnOff( WIFIP2PMATCH_WORK *wk )
{
	// �߂邪�\��OFF�ɂȂ��Ă邩������Ȃ��̂�ON�ɂ��Ă���
	if( wk->view.buttonact_on == MCV_USERD_BTTN_MODE_NOBACK ){
		CLACT_SetDrawFlag( wk->view.button_act[MCV_USERD_BTTN_BACK], TRUE );
		FONTOAM_SetDrawFlag( wk->view.back_fontoam, TRUE );
	}
	
	// ��\����Ԃɂ���
	wk->view.buttonact_on = MCV_USERD_BTTN_MODE_WAIT;

	// �\����OFF
	GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^����\������
 *
 *	@param	wk 
 *
 *	@retval	MCV_USERD_BTTN_RET_NONE,	// ���̔������Ȃ�
 *	@retval	MCV_USERD_BTTN_RET_LEFT,	// ���������ꂽ
 *	@retval	MCV_USERD_BTTN_RET_BACK,	// ���ǂ邪�����ꂽ
 *	@retval	MCV_USERD_BTTN_RET_RIGHT,	// �E�������ꂽ
 */
//-----------------------------------------------------------------------------
static u32 MCVSys_OamBttnMain( WIFIP2PMATCH_WORK *wk )
{
	int i;
	BOOL result;
	
	// �\��ON���傤�������`�F�b�N
	if( wk->view.buttonact_on == MCV_USERD_BTTN_MODE_WAIT ){
		return MCV_USERD_BTTN_RET_NONE;
	}


	// �{�^���C�x���g�f�[�^����
	wk->view.touch_button = MCV_USERD_BTTN_NUM;	// ���ɂ��������Ȃ��悤�ɂ��肦�Ȃ��{�^���ԍ�
	wk->view.touch_button_event = 0xff;	// ���ɂ��������Ȃ��悤�ɂ��肦�Ȃ�����������Ă���

	// �{�^���}�l�[�W�����s
	BMN_Main( wk->view.p_oambttnman );

	// �{�^���A�j���R���g���[��
	for( i=0; i<MCV_USERD_BTTN_NUM; i++ ){

		// �o�b�N��\�����[�h�Ȃ�o�b�N�̏������s��Ȃ�
		if( (wk->view.buttonact_on == MCV_USERD_BTTN_MODE_NOBACK) &&
			(i == MCV_USERD_BTTN_BACK) ){
			continue;
		}
		
		result = MCVSys_OamBttnObjAnmMain( &wk->view, i, wk->view.touch_button, wk->view.touch_button_event );
		if( result == TRUE ){
			// �{�^����������
			return MCV_USERD_BTTN_RET_LEFT + i;
		}
	}

	return MCV_USERD_BTTN_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���R�[���o�b�N
 *
 *	@param	bttn_no		�{�^��No
 *	@param	event		�C�x���gNo
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnCallBack( u32 bttn_no, u32 event, void* p_work )
{
	WIFIP2PMATCH_WORK* p_wk = p_work;
	p_wk->view.touch_button = bttn_no;
	p_wk->view.touch_button_event = event;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���A�j���J�n
 *
 *	@param	p_view		���[�N
 *	@param	bttn_no		�{�^���i���o�[
 */
//-----------------------------------------------------------------------------
static void MCVSys_OamBttnObjAnmStart( WIFIP2PMATCH_VIEW*  p_view, u32 bttn_no )
{
	// �A�j��������	RELEASE�A�j���ŏI�t���[���ɐݒ�
	CLACT_AnmChg( p_view->button_act[ bttn_no ], c_MCV_USER_BTTN_ANM_RELEASE[bttn_no] );
	CLACT_AnmFrameSet( p_view->button_act[ bttn_no ], MCV_USERD_BTTN_ANMMAX_0ORG );

	// ������̍��W������
	if( bttn_no == MCV_USERD_BTTN_BACK ){
		FONTOAM_SetMat( p_view->back_fontoam, MCV_USERD_BTTN_FONT_X, c_MCV_USER_BTTN_FONT_YANM[0] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j�����C��
 *
 *	@param	p_view		���[�N
 *	@param	bttn_no		�{�^���i���o�[
 *	@param	push_bttn	������Ă���{�^��
 *	@param	event		�C�x���g
 *
 *	@retval	TRUE	�����I�����
 *	@retval	FALSE	������Ă��Ȃ����͉����I����ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL MCVSys_OamBttnObjAnmMain( WIFIP2PMATCH_VIEW*  p_view, u32 bttn_no, u32 push_bttn, u32 event )
{
	BOOL ret = FALSE;
	u32 anm_frame;
	u32 anm_seq;
	
	// �������ԉ����Ă���Ƃ���MCV_USERD_BTTN_ANMPUSHOK�܂ł�������TRUE��Ԃ�
	// �Z�����ԉ����Ă���Ƃ��������ꂽ�Ƃ���TRUE��Ԃ�
	
	
	// ������������Ă��邩�`�F�b�N
	if( (push_bttn == bttn_no)  ){

		anm_frame = CLACT_AnmFrameGet( p_view->button_act[bttn_no] );

		// ������Ă���Ƃ�
		if( (event == BMN_EVENT_TOUCH)  || (event == BMN_EVENT_HOLD) ){
	 
			// �C�x���g���ŏ��̃^�b�`��������A�j���ݒ�
			if( event == BMN_EVENT_TOUCH ){
				CLACT_AnmChg( p_view->button_act[ bttn_no ], c_MCV_USER_BTTN_ANM_PUSH[bttn_no] );
			}
			
			// �A�j����PUSHOK�ȉ��̎��̂ݓ�����
			if( anm_frame < MCV_USERD_BTTN_ANMPUSHOK ){
				
				CLACT_AnmFrameChg( p_view->button_act[bttn_no], FX32_CONST(2) );
				anm_frame = CLACT_AnmFrameGet( p_view->button_act[bttn_no] );

				// �u���ǂ�v�Ȃ�t�H���g��������
				if( bttn_no == MCV_USERD_BTTN_BACK ){
					FONTOAM_SetMat( p_view->back_fontoam, MCV_USERD_BTTN_FONT_X, 
							c_MCV_USER_BTTN_FONT_YANM[anm_frame] );
				}

				// �{�^�������܂ŃA�j�������牟�������Ƃɂ���
				// ���̎��̃t���[������͏��anm_frame�`�F�b�N��
				// �Ђ�������̂ł����͏��߂�OK�ɂ����Ƃ��ɂ̂�
				// ���s�����
				if( anm_frame >= MCV_USERD_BTTN_ANMPUSHOK ){
					ret = TRUE;
				}
			}
			
		}
		// �������Ƃ�
		else if( event == BMN_EVENT_RELEASE ){
			if( anm_frame < MCV_USERD_BTTN_ANMPUSHOK ){
				// �{�^�������������ƂɂȂ�t���[���܂ł����Ă��Ȃ����
				// �{�^�������������Ƃɂ���
				ret = TRUE;
			}
		}
	}else{
		
		anm_seq = CLACT_AnmGet( p_view->button_act[bttn_no] );
		anm_frame = CLACT_AnmFrameGet( p_view->button_act[bttn_no] );
	
		// �A�j����PUSH�A�j����������RELEASE�A�j���ɐ؂�ւ���
		if( anm_seq == c_MCV_USER_BTTN_ANM_PUSH[bttn_no] ){
			CLACT_AnmChg( p_view->button_act[bttn_no], c_MCV_USER_BTTN_ANM_RELEASE[bttn_no] );
			CLACT_AnmFrameSet( p_view->button_act[bttn_no], MCV_USERD_BTTN_ANMMAX_0ORG - anm_frame );
		}

		// �A�j�����I���܂ŃA�j��������
		CLACT_AnmFrameChg( p_view->button_act[bttn_no], FX32_CONST(2) );
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ŏ��ɐl������y�[�W��Ԃ�
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�y�[�W	�i�N�����Ȃ��Ȃ�O�y�[�W�ڂ�Ԃ��j
 */
//-----------------------------------------------------------------------------
static s32 MCVSys_FirstFriendPageGet( const WIFIP2PMATCH_WORK * cp_wk )
{
	int i;
	s32 friend_no;

	friend_no = 0xffff;	// ���肦�Ȃ��ő�l�ŏ�����

	for( i=0; i<WIFIP2PMATCH_MEMBER_MAX; i++ ){
		if( (cp_wk->index2NoBackUp[i] != 0) && (friend_no > cp_wk->index2NoBackUp[i]) ){
			friend_no = cp_wk->index2NoBackUp[i];
		}
	}

	if( friend_no == 0xffff ){
		return 0;
	}

	return ( (friend_no - 1) / WCR_MAPDATA_1BLOCKOBJNUM );
}

//----------------------------------------------------------------------------
/**
 *	@brief	Vier�̂ق��̃��[�h�Z�b�g�ɐl�̖��O��ݒ�
 *	
 *	@param	p_wk			���[�N
 *	@param	friendno		�F�B�i���o�[
 */
//-----------------------------------------------------------------------------
static void MCVSys_FriendNameSet( WIFIP2PMATCH_WORK* p_wk, int friendno )
{
    if(friendno != -1){
        MYSTATUS* pTarget = MyStatus_AllocWork(HEAPID_WIFIP2PMATCH);
        MyStatus_SetMyName(pTarget, WifiList_GetFriendNamePtr(p_wk->pList,friendno));
        WORDSET_RegisterPlayerName( p_wk->view.p_wordset, 0, pTarget);
        sys_FreeMemoryEz(pTarget);
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�}�b�`�f�[�^�̃X�e�[�^�X�擾
 *
 *	@param	wk			�V�X�e�����[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static _WIFI_MACH_STATUS* WifiFriendMatchStatusGet( WIFIP2PMATCH_WORK* wk, u32 idx )
{
	GF_ASSERT( idx < WIFIP2PMATCH_MEMBER_MAX );

#ifdef WFP2PM_MANY_OBJ
	idx = 0;
#endif
	return &wk->pMatch->friendMatchStatus[ idx ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�̏�Ԃ��擾����
 *
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static u8 WifiDwc_getFriendStatus( int idx )
{
#ifdef WFP2PM_MANY_OBJ
	idx = 0;
#endif
	return mydwc_getFriendStatus( idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	P2P�ʐM�Q�[�����J�n����B
 *
 *	@param	friendno	�^�[�Q�b�g�ԍ�	�i-1�Ȃ�e�j
 *	@param	status		�X�e�[�^�X
 *
 *	@return
 */
//-----------------------------------------------------------------------------
static BOOL WifiP2PMatch_CommWifiBattleStart( WIFIP2PMATCH_WORK* wk, int friendno, int status )
{
	// 080707	tomoya takahashi
	// �����ʐM���[�N��������
	CommToolInitialize( HEAPID_COMMUNICATION );// COMMUNICATON�ɂ��Ă���̂́Acomm_system�̒��ł�COMMUNICATION�ł���Ă��邽�߂ł��B
	
	// �{�C�X�`���b�g�ݒ�
	mydwc_setVChat(wk->pMatch->myMatchStatus.vchat);// �{�C�X�`���b�g��BGM���ʂ̊֌W�𐮗� tomoya takahashi

	// 4�l�ŗV�ԃQ�[���̂Ƃ��́A�ʐM�̈ꕔ��HEAP_WORLD����m�ۂ���
	// ���̑��͒ʏ�̒ʐM�o�b�t�@����Ƃ�
	if( _wait2to4Mode( status ) == TRUE ){
		mydwc_recvHeapChange( TRUE, HEAPID_WIFIP2PMATCHEX );
	}else{
		mydwc_recvHeapChange( FALSE, HEAPID_WIFIP2PMATCHEX );
	}

	// �ڑ�����O�ɂS�l��W�ő����Ă���\���̂���
	// �R�}���h��ݒ肷��
	CommCommandWFP2PMF_MatchStartInitialize();
	
	return CommWifiBattleStart( friendno );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B��W���@�ݒ�ON
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void FriendRequestWaitOn( WIFIP2PMATCH_WORK* wk, BOOL msg_on )
{
	if( wk->friend_request_wait == FALSE ){
		// ���b�Z�[�W�������āA�v���C���[������~����
		wk->friend_request_wait = TRUE;
		WIFI_MCR_PlayerMovePause( &wk->matchroom, TRUE );

		if( msg_on == TRUE ){
			WifiP2PMatchMessagePrint( wk, msg_wifilobby_142, FALSE );	
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B��W���@�ݒ�OFF
 *
 *	@param	wk	���[�N
 */
//-----------------------------------------------------------------------------
static void FriendRequestWaitOff( WIFIP2PMATCH_WORK* wk )
{
	if( wk->friend_request_wait == TRUE ){
		// ���b�Z�[�W�������āA�v���C���[������J�n����
		wk->friend_request_wait = FALSE;
		EndMessageWindowOff( wk );
		WIFI_MCR_PlayerMovePause( &wk->matchroom, FALSE );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B��W���t���O�擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	��W��
 *	@retval	FALSE	�ڂ��イ���Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL FriendRequestWaitFlagGet( const WIFIP2PMATCH_WORK* cp_wk )
{
	return cp_wk->friend_request_wait;
}

