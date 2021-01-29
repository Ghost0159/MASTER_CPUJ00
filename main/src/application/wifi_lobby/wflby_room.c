//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_room.c
 *	@brief		WiFi���r�[	�����V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.11.01
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "gflib/button_man.h"

#include "system/wipe.h"
#include "system/pm_overlay.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/wordset.h"
#include "system/msgdata.h"
#include "system/arc_tool.dat"
#include "system/arc_util.h"
#include "system/pm_debug_wifi.h"
#include "system/pm_debug_wifi.h"
#include "system/pmfprint.h"


#include "savedata/config.h"

#include "communication/communication.h"

#include "battle/trtype_def.h"
#include "battle/wazatype_icon.h"

#include "field/shop_gra.naix"

#include "msgdata/msg_wifi_hiroba.h"
#include "msgdata/msg_wifi_system.h"
#include "msgdata/msg.naix"

#include "graphic/wifi_lobby_other.naix"
#include "graphic/unionobj2d_onlyfront.naix"

#include "wifi/dwc_lobbylib.h"

#include "wflby_room_def.h"
#include "wflby_event.h"
#include "wflby_ev_def.h"
#include "wflby_snd.h"
#include "wflby_gadget.h"
#include "wflby_timeevent.h"




//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------

#ifdef PM_DEBUG
//#define	WFLBY_DEBUG_ROOM_CAMERA		// �J������ς���
#define	WFLBY_DEBUG_ROOM_DPRINT		// �f�o�b�N�\��
#define WFLBY_DEBUG_ROOM_ITEMCHG	// �K�W�F�b�g�ύX
//#define	WFLBY_DEBUG_ROOM_LIGHT	// ���C�g��\��
//#define WFLBY_DEBUG_ROOM_TRTYPE_CHG	//  �g���[�i�^�C�v�ύX
//#define WFLBY_DEBUG_ROOM_PRINT_TIME	// �������x��\��

#endif


#ifdef WFLBY_DEBUG_ROOM_CAMERA
static	WFLBY_DEBUG_ROOM_CAMERA_FLAG = 0;
#endif

#ifdef WFLBY_DEBUG_ROOM_LIGHT
typedef struct{
	GXRgb lightcolor[2];
	GXRgb matcolor[4];
} WFLBY_DEBUG_ROOM_LIGHTDATA;
#endif




#ifdef WFLBY_DEBUG_ROOM_PRINT_TIME

static OSTick	WFLBY_DEBUG_ROOM_PRINT_TIME_Tick;
#define WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_INIT	WFLBY_DEBUG_ROOM_PRINT_TIME_Tick = OS_GetTick();
#define WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT(line)	OS_TPrintf( "line[%d] time %d ms\n", (line), OS_TicksToMilliSeconds(OS_GetTick() - WFLBY_DEBUG_ROOM_PRINT_TIME_Tick) );
#define WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT_DRAW(line)	OS_TPrintf( "line[%d] time %d ms\n", (line), OS_TicksToMilliSeconds(OS_GetTick() - WFLBY_DEBUG_ROOM_PRINT_TIME_Tick) );

#else		// WFLBY_DEBUG_ROOM_PRINT_TIME

#define		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_INIT			((void)0);
#define		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT(line)	((void)0);
#define		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT_DRAW(line)	((void)0);

#endif		// WFLBY_DEBUG_ROOM_PRINT_TIME


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	���C���V�[�P���X
//=====================================
enum{
	WFLBY_ROOM_MAINSEQ_FADEIN,
	WFLBY_ROOM_MAINSEQ_FADEIN_WAIT,
	WFLBY_ROOM_MAINSEQ_MAIN,
	WFLBY_ROOM_MAINSEQ_ERR_MSG,
	WFLBY_ROOM_MAINSEQ_ERR_MSGWAIT,
	WFLBY_ROOM_MAINSEQ_TIMEOUT_MSG,
	WFLBY_ROOM_MAINSEQ_TIMEOUT_MSGWAIT,
	WFLBY_ROOM_MAINSEQ_FADEOUT,
	WFLBY_ROOM_MAINSEQ_FADEOUT_WAIT,
};

//-------------------------------------
///	3DOBJCONT�֘A
//=====================================
#define WFLBY_ROOM_3DOBJCONT_WKNUM		( 24 )

//-------------------------------------
///	�\���V�X�e���֘A
//=====================================
#define WFLBY_ROOM_VTRTSK_NUM			( 32 )	// VRAM�]���^�X�N��

//-------------------------------------
// BG
//=====================================
enum{
	// ���C�����
	WFLBY_ROOM_BGCNT_MAIN_MSGWIN,
	
	// �T�u���
	WFLBY_ROOM_BGCNT_SUB_BACK,
	WFLBY_ROOM_BGCNT_SUB_BTTN_TR,
	WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG,

	WFLBY_ROOM_BGCNT_NUM,	// �a�f�R���g���[���e�[�u����
};
enum {	// ���C���ʃp���b�g
	WFLBY_ROOM_BGPL_DMY,				//�@�_�~�[
	WFLBY_ROOM_BGPL_TALKWIN,			// ��b�E�B���h�E
	WFLBY_ROOM_BGPL_BOARDWIN,			// �ŔE�B���h�E
	WFLBY_ROOM_BGPL_SYSWIN,				// �V�X�e���E�B���h�E
	WFLBY_ROOM_BGPL_TALKFONT_CL,		// ��b�t�H���g
	WFLBY_ROOM_BGPL_SYSFONT_CL,			// �V�X�e���t�H���g

	WFLBY_ROOM_BGPL_NUM,		// �������g�p����p���b�g
};
enum {	// �T�u�ʃp���b�g
	WFLBY_ROOM_BGSPL_BACK00,			// �w�i���Ȃ���
	WFLBY_ROOM_BGSPL_BACK01,			// �w�i���Ȃ���
	WFLBY_ROOM_BGSPL_BACK02,			// �w�i���Ȃ���
	WFLBY_ROOM_BGSPL_BACK03,			// �w�i���Ȃ���
	WFLBY_ROOM_BGSPL_BACK04,			// �w�i���Ȃ���
	WFLBY_ROOM_BGSPL_FONTCL,			// �t�H���g�J���[
	WFLBY_ROOM_BGSPL_NONE01,			// 
	WFLBY_ROOM_BGSPL_NONE02,			// 
	WFLBY_ROOM_BGSPL_NONE03,			// 
	WFLBY_ROOM_BGSPL_NONE04,			// 
	WFLBY_ROOM_BGSPL_BACK05,			// �w�i���Ȃ���
	WFLBY_ROOM_BGSPL_WAZATYPE0,			// �Z�^�C�v0
	WFLBY_ROOM_BGSPL_WAZATYPE1,			// �Z�^�C�v1
	WFLBY_ROOM_BGSPL_WAZATYPE2,			// �Z�^�C�v2 
	WFLBY_ROOM_BGSPL_NONE08,			// 
	WFLBY_ROOM_BGSPL_BACK06,			// �w�i���Ȃ���

	WFLBY_ROOM_BGSPL_NUM,		// �������g�p����p���b�g
};

//-------------------------------------
// �E�B���h�E�O���t�B�b�N
//=====================================
#define WFLBY_TALKWINGRA_CGX		( 1 )
#define WFLBY_BOARDWINGRA_CGX		( WFLBY_TALKWINGRA_CGX+TALK_WIN_CGX_SIZ )
#define WFLBY_SYSWINGRA_CGX			( WFLBY_BOARDWINGRA_CGX+(BOARD_WIN_CGX_SIZ+BOARD_MAP_CGX_SIZ) )
#define WFLBY_SYSWINGRA_CGXEND		( WFLBY_SYSWINGRA_CGX+MENU_WIN_CGX_SIZ )



//-------------------------------------
// OAM
//=====================================
#define WFLBY_ROOM_OAM_CONTNUM			( 24 )	// OAM�Ǘ���
#define WFLBY_ROOM_OAM_RESNUM			( 4 )	// ���\�[�X�Ǘ����
#define WFLBY_ROOM_CLACT_NUM			( 24 )
#define WFLBY_ROOM_CLACT_SUBSURFACE_Y	( 256*FX32_ONE )


//-------------------------------------
//�v���W�F�N�V�����}�g���N�X�𑀍삷��ۂ̂y�I�t�Z�b�g
//=====================================
#define	WFLBY_ROOM_PRO_MAT_Z_OFS	(1)



//-------------------------------------
///	�����I�����b�Z�[�W�E�G�C�g
//=====================================
#define WFLBY_MSG_WAIT	( 60 )


//-------------------------------------
// ��b�E�B���h�E
//=====================================
#define WFLBY_TALKWIN_X			( 2 )
#define WFLBY_TALKWIN_Y			( 19 )
#define WFLBY_TALKWIN_SIZX		( 27 )
#define WFLBY_TALKWIN_SIZY		( 4 )
#define WFLBY_TALKWIN_CGX		( WFLBY_SYSWINGRA_CGXEND )
#define WFLBY_TALKWIN_CGXEND	( WFLBY_TALKWIN_CGX+(WFLBY_TALKWIN_SIZX*WFLBY_TALKWIN_SIZY) )
#define WFLBY_TALKWIN_STRBUFNUM	( 384 )	// �����o�b�t�@��
#define WFLBY_TALKWIN_MSGCOL	( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )	

//-------------------------------------
// ���X�g�E�B���h�E
//=====================================
#define WFLBY_LISTWIN_X			( 16 )
#define WFLBY_LISTWIN_Y			( 3 )
#define WFLBY_LISTWIN_SIZX		( 15 )
#define WFLBY_LISTWIN_SIZY		( 18 )	// ���X�g�ő�T�C�Y
#define WFLBY_LISTWIN_CGX		( WFLBY_TALKWIN_CGXEND )
#define WFLBY_LISTWIN_CGXEND	( WFLBY_LISTWIN_CGX+(WFLBY_LISTWIN_SIZX*WFLBY_LISTWIN_SIZY) )
#define WFLBY_LISTWIN_MSGCOL	( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )	

#define WFLBY_LISTWIN_CLACT_RESNUM	(4)	// �Z�����\�[�X��
#define WFLBY_LISTWIN_CLACT_OBJNUM	(2)	// �A�N�^�[�I�u�W�F��
#define WFLBY_LISTWIN_CLACT_RESCONT_ID	( 5000 )
#define WFLBY_LISTWIN_CLACT_X		( 192 )
static const u8 sc_WFLBY_LISTWIN_CLACT_Y[ WFLBY_LISTWIN_CLACT_OBJNUM ] = {
	20, 136
};

//-------------------------------------
///	�G���[�\���E�B���h�E
//=====================================
#define WFLBY_ERRWIN_X			( 4 )
#define WFLBY_ERRWIN_Y			( 4 )
#define WFLBY_ERRWIN_SIZX		( 23 )
#define WFLBY_ERRWIN_SIZY		( 16 )
#define WFLBY_ERRWIN_CGX		( WFLBY_SYSWINGRA_CGXEND )
#define WFLBY_ERRWIN_CGXEND		( WFLBY_ERRWIN_CGX+(WFLBY_ERRWIN_SIZX*WFLBY_ERRWIN_SIZY) )
#define WFLBY_ERRWIN_MSGCOL		( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )	

//-------------------------------------
///	�T�u�E�B���h�E
//=====================================
#define WFLBY_SUBWIN_MSGCOL		( GF_PRINTCOLOR_MAKE( 1, 2, 15 ) )	



//-------------------------------------
///	YesNoWin
//=====================================
#define WFLBY_YESNOWIN_DATANUM	( 2 )		// ���ڐ�


//-------------------------------------
/// �L���p���b�Z�[�W
//=====================================
#define WFLBY_DEFMSG_STRNUM		( 480 )


//-------------------------------------
// �����
//=====================================
enum {	// �`����
	WFLBY_UNDERWIN_DRAW_NONE,		// �����Ȃ�
	WFLBY_UNDERWIN_DRAW_BTTN,		// �{�^���\��
	WFLBY_UNDERWIN_DRAW_PROFILE,	// �v���t�B�[���\��
	WFLBY_UNDERWIN_DRAW_FLOAT,		// �t���[�g�p�{�^���\��
	WFLBY_UNDERWIN_DRAW_BTTN_DUMMY,	// �{�^���\���@�_�~�[�i�ŏ��Ƀ^�b�`�g�C�����炤�Ƃ��j
} ;
enum{	// ����V�[�P���X
	WFLBY_UNDERWIN_SEQ_STARTWAIT,			// �J�n�҂�
	WFLBY_UNDERWIN_SEQ_STARTFADEOUT,		// �J�n�t�F�[�h�A�E�g
	WFLBY_UNDERWIN_SEQ_STARTFADEOUTWAIT,	// �J�n�t�F�[�h�A�E�g�҂�
	WFLBY_UNDERWIN_SEQ_STARTFADEIN,			// �J�n�t�F�[�h�C��
	WFLBY_UNDERWIN_SEQ_STARTFADEINWAIT,		// �J�n�t�F�[�h�C���҂�
	WFLBY_UNDERWIN_SEQ_NORMAL,
	WFLBY_UNDERWIN_SEQ_FADEOUT,
	WFLBY_UNDERWIN_SEQ_FADEOUTWAIT,
	WFLBY_UNDERWIN_SEQ_CHANGE,
	WFLBY_UNDERWIN_SEQ_FASEIN,
	WFLBY_UNDERWIN_SEQ_FASEINWAIT,
};
#define WFLBY_UNDERWIN_FASE_DIV		( 3 )
#define WFLBY_UNDERWIN_FASE_SYNC	( 1 )

#define WFLBY_UNDERWIN_BGCG_END		( 320 )	// �w�i�ƃt���[���pCGX�̃I����



//-------------------------------------
// trainer�J�[�h
//=====================================
#define WFLBY_TRCARD_STRBUF	( 64 )		// ������o�b�t�@�v��
enum{	// �E�B���h�E�萔
	WFLBY_TRCARD_WIN_NAME,
	WFLBY_TRCARD_WIN_COUNTRY,
	WFLBY_TRCARD_WIN_TRSTART,
	WFLBY_TRCARD_WIN_LASTACTION,
	WFLBY_TRCARD_WIN_VIPAIKOTOBA,
	WFLBY_TRCARD_WIN_NUM,
};
#define WFLBY_TRCARD_WIN_NAME_X		( 8 )
#define WFLBY_TRCARD_WIN_NAME_Y		( 2 )
#define WFLBY_TRCARD_WIN_NAME_SIZX	( 16 )
#define WFLBY_TRCARD_WIN_NAME_SIZY	( 4 )
#define WFLBY_TRCARD_WIN_NAME_CGX	( WFLBY_UNDERWIN_BGCG_END )

#define WFLBY_TRCARD_WIN_COUNTRY_X		( 1 )
#define WFLBY_TRCARD_WIN_COUNTRY_Y		( 7 )
#define WFLBY_TRCARD_WIN_COUNTRY_SIZX	( 30 )
#define WFLBY_TRCARD_WIN_COUNTRY_SIZY	( 6 )
#define WFLBY_TRCARD_WIN_COUNTRY_CGX	( WFLBY_TRCARD_WIN_NAME_CGX+(WFLBY_TRCARD_WIN_NAME_SIZX*WFLBY_TRCARD_WIN_NAME_SIZY) )

#define WFLBY_TRCARD_WIN_TRSTART_X		( 1 )
#define WFLBY_TRCARD_WIN_TRSTART_Y		( 14 )
#define WFLBY_TRCARD_WIN_TRSTART_SIZX	( 19 )
#define WFLBY_TRCARD_WIN_TRSTART_SIZY	( 4 )
#define WFLBY_TRCARD_WIN_TRSTART_CGX	( WFLBY_TRCARD_WIN_COUNTRY_CGX+(WFLBY_TRCARD_WIN_COUNTRY_SIZX*WFLBY_TRCARD_WIN_COUNTRY_SIZY) )

#define WFLBY_TRCARD_WIN_LASTACTION_X		( 1 )
#define WFLBY_TRCARD_WIN_LASTACTION_Y		( 19 )
#define WFLBY_TRCARD_WIN_LASTACTION_SIZX	( 19 )
#define WFLBY_TRCARD_WIN_LASTACTION_SIZY	( 4 )
#define WFLBY_TRCARD_WIN_LASTACTION_CGX	( WFLBY_TRCARD_WIN_TRSTART_CGX+(WFLBY_TRCARD_WIN_TRSTART_SIZX*WFLBY_TRCARD_WIN_TRSTART_SIZY) )

#define WFLBY_TRCARD_WIN_VIPAIKOTOBA_X		( 0 )
#define WFLBY_TRCARD_WIN_VIPAIKOTOBA_Y		( 14 )
#define WFLBY_TRCARD_WIN_VIPAIKOTOBA_SIZX	( 22 )
#define WFLBY_TRCARD_WIN_VIPAIKOTOBA_SIZY	( 8 )
#define WFLBY_TRCARD_WIN_VIPAIKOTOBA_CGX	( WFLBY_TRCARD_WIN_COUNTRY_CGX+(WFLBY_TRCARD_WIN_COUNTRY_SIZX*WFLBY_TRCARD_WIN_COUNTRY_SIZY) )

// �Z�^�C�v
#define WFLBY_TRCARD_WIN_WAZATYPE1_X		( 26 )
#define WFLBY_TRCARD_WIN_WAZATYPE1_Y		( 2 )
#define WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX	( 4 )
#define WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY	( 2 )
#define WFLBY_TRCARD_WIN_WAZATYPE1_CGX		( WFLBY_TRCARD_WIN_VIPAIKOTOBA_CGX+(WFLBY_TRCARD_WIN_VIPAIKOTOBA_SIZX*WFLBY_TRCARD_WIN_VIPAIKOTOBA_SIZY) )
#define WFLBY_TRCARD_WIN_WAZATYPE2_CGX		( WFLBY_TRCARD_WIN_WAZATYPE1_CGX+(WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX*WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY) )
static const u16 sc_WFLBY_TRCARD_WAZATYPE_CGX[ WFLBY_SELECT_TYPENUM ] = {
	WFLBY_TRCARD_WIN_WAZATYPE1_CGX,
	WFLBY_TRCARD_WIN_WAZATYPE2_CGX,
};

static const u16 sc_WFLBY_TRCARD_WAZATYPE_SCRN[ WFLBY_SELECT_TYPENUM ][WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY][WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX] ={
	{
		{ 
			WFLBY_TRCARD_WIN_WAZATYPE1_CGX+0, WFLBY_TRCARD_WIN_WAZATYPE1_CGX+1,
			WFLBY_TRCARD_WIN_WAZATYPE1_CGX+2, WFLBY_TRCARD_WIN_WAZATYPE1_CGX+3,
		},
		{ 
			WFLBY_TRCARD_WIN_WAZATYPE1_CGX+4, WFLBY_TRCARD_WIN_WAZATYPE1_CGX+5,
			WFLBY_TRCARD_WIN_WAZATYPE1_CGX+6, WFLBY_TRCARD_WIN_WAZATYPE1_CGX+7,
		},
	},
	{
		{ 
			WFLBY_TRCARD_WIN_WAZATYPE2_CGX+0, WFLBY_TRCARD_WIN_WAZATYPE2_CGX+1,
			WFLBY_TRCARD_WIN_WAZATYPE2_CGX+2, WFLBY_TRCARD_WIN_WAZATYPE2_CGX+3,
		},
		{ 
			WFLBY_TRCARD_WIN_WAZATYPE2_CGX+4, WFLBY_TRCARD_WIN_WAZATYPE2_CGX+5,
			WFLBY_TRCARD_WIN_WAZATYPE2_CGX+6, WFLBY_TRCARD_WIN_WAZATYPE2_CGX+7,
		},
	},
};


static const BMPWIN_DAT sc_WFLBY_TRCARD_WIN_DATA[ WFLBY_TRCARD_WIN_NUM ] = {
	{
		GF_BGL_FRAME2_S,
		WFLBY_TRCARD_WIN_NAME_X,
		WFLBY_TRCARD_WIN_NAME_Y,
		WFLBY_TRCARD_WIN_NAME_SIZX,
		WFLBY_TRCARD_WIN_NAME_SIZY,
		WFLBY_ROOM_BGSPL_FONTCL,
		WFLBY_TRCARD_WIN_NAME_CGX,
	},
	{
		GF_BGL_FRAME2_S,
		WFLBY_TRCARD_WIN_COUNTRY_X,
		WFLBY_TRCARD_WIN_COUNTRY_Y,
		WFLBY_TRCARD_WIN_COUNTRY_SIZX,
		WFLBY_TRCARD_WIN_COUNTRY_SIZY,
		WFLBY_ROOM_BGSPL_FONTCL,
		WFLBY_TRCARD_WIN_COUNTRY_CGX,
	},
	{
		GF_BGL_FRAME2_S,
		WFLBY_TRCARD_WIN_TRSTART_X,
		WFLBY_TRCARD_WIN_TRSTART_Y,
		WFLBY_TRCARD_WIN_TRSTART_SIZX,
		WFLBY_TRCARD_WIN_TRSTART_SIZY,
		WFLBY_ROOM_BGSPL_FONTCL,
		WFLBY_TRCARD_WIN_TRSTART_CGX,
	},
	{
		GF_BGL_FRAME2_S,
		WFLBY_TRCARD_WIN_LASTACTION_X,
		WFLBY_TRCARD_WIN_LASTACTION_Y,
		WFLBY_TRCARD_WIN_LASTACTION_SIZX,
		WFLBY_TRCARD_WIN_LASTACTION_SIZY,
		WFLBY_ROOM_BGSPL_FONTCL,
		WFLBY_TRCARD_WIN_LASTACTION_CGX,
	},
	{
		GF_BGL_FRAME2_S,
		WFLBY_TRCARD_WIN_VIPAIKOTOBA_X,
		WFLBY_TRCARD_WIN_VIPAIKOTOBA_Y,
		WFLBY_TRCARD_WIN_VIPAIKOTOBA_SIZX,
		WFLBY_TRCARD_WIN_VIPAIKOTOBA_SIZY,
		WFLBY_ROOM_BGSPL_FONTCL,
		WFLBY_TRCARD_WIN_VIPAIKOTOBA_CGX,
	},
};
// �����F
#define WFLBY_TRCARD_COL_RED	( GF_PRINTCOLOR_MAKE( 3, 4, 0 ) )
#define WFLBY_TRCARD_COL_BLUE	( GF_PRINTCOLOR_MAKE( 7, 8, 0 ) )
#define WFLBY_TRCARD_COL_BLACK	( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )
// �A�C�e���A�C�R��
#define WFLBY_TRCARD_ICON_RESID		( 100 )
#define WFLBY_TRCARD_ICON_PLTTNUM	( 3 )		// �p���b�g�{��
#define WFLBY_TRCARD_ICON_BGPRI		( 0 )		// BG�D�揇��
#define WFLBY_TRCARD_ICON_SFPRI		( 16 )		// �D�揇��
#define WFLBY_TRCARD_ICON_X			( FX32_CONST( 208 ) )
#define WFLBY_TRCARD_ICON_Y			( FX32_CONST( 152 )+WFLBY_ROOM_CLACT_SUBSURFACE_Y )
#define WFLBY_TRCARD_TRVIEW_RESID	( 101 )
#define WFLBY_TRCARD_TRVIEW_BGPRI	( 3 )
#define WFLBY_TRCARD_TRVIEW_SFPRI	( 32 )

// �t�B�[���h�g���[�i�h�c���o�g���g���[�i�^�C�v�ɕϊ�����e�[�u��
typedef struct {
	u16 fld_trno;
	u16  btl_trtype;
	u16 x;
	u16 y;
} WFLBY_TRCARD_TRTYPE;
#define WFLBY_TRCARD_TRTYPE_NUM	( 18 )
static const WFLBY_TRCARD_TRTYPE sc_WFLBY_TRCARD_TRTYPE[WFLBY_TRCARD_TRTYPE_NUM]={
	{  HERO,		TRTYPE_BOY,		33,	35 },
	{  HEROINE,		TRTYPE_GIRL,	38,	36 },

	{  BOY1,		TRTYPE_SCHOOLB,	38,	25 },
	{  BOY3,		TRTYPE_MUSHI,	44,	32 },
	{  MAN3,		TRTYPE_ELITEM,	38,	44 },
	{  BADMAN,		TRTYPE_HEADS,	40,	40 },
	{  EXPLORE,		TRTYPE_ISEKI,	44,	26 },
	{  FIGHTER,		TRTYPE_KARATE,	44,	37 },
	{  GORGGEOUSM,	TRTYPE_PRINCE,	29,	40 },
	{  MYSTERY,		TRTYPE_ESPM,	42,	44 },

	{  GIRL1,		TRTYPE_MINI,	39,	34 },
	{  GIRL2,		TRTYPE_BATTLEG,	40,	36 },
	{  WOMAN2,		TRTYPE_SISTER,	38,	41 },
	{  WOMAN3,		TRTYPE_ELITEW,	36,	42 },
	{  IDOL,		TRTYPE_IDOL,	38,	38 },
	{  LADY,		TRTYPE_MADAM,	38,	39 },
	{  COWGIRL,		TRTYPE_COWGIRL,	38,	35 },
	{  GORGGEOUSW,	TRTYPE_PRINCESS,40,	44 },
};
// �������\�[�X
enum{
	WFLBY_TRCARD_RIREKI_RES_UNIONOBJ,
	WFLBY_TRCARD_RIREKI_RES_ICON,
	WFLBY_TRCARD_RIREKI_RES_HEROBOY,
	WFLBY_TRCARD_RIREKI_RES_HEROGIRL,
	WFLBY_TRCARD_RIREKI_RES_NUM,
};
#define WFLBY_TRCARD_RIREKI_RESID		( 150 )
#define WFLBY_TRCARD_RIREKI_BGPRI		( 0 )		// BG�D�揇��
// ���������ɑΉ������L�����N�^���\�[�X�f�[�^
typedef struct{
	u8 arcid;
	u8 paloffs;
	u16 dataid;
	s16 ofs_x;
	s16 ofs_y;
} WFLBY_TRCARD_RIREKI_CHAR;
static const WFLBY_TRCARD_RIREKI_CHAR sc_WFLBY_TRCARD_RIREKI_CHAR[ WFLBY_LASTACTION_MAX ] = {
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 0, NARC_unionobj2d_onlyfront_only_front00_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 6, NARC_unionobj2d_onlyfront_only_front01_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 1, NARC_unionobj2d_onlyfront_only_front02_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 2, NARC_unionobj2d_onlyfront_only_front03_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 6, NARC_unionobj2d_onlyfront_only_front04_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 5, NARC_unionobj2d_onlyfront_only_front05_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 3, NARC_unionobj2d_onlyfront_only_front06_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 4, NARC_unionobj2d_onlyfront_only_front07_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 0, NARC_unionobj2d_onlyfront_only_front08_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 7, NARC_unionobj2d_onlyfront_only_front09_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 4, NARC_unionobj2d_onlyfront_only_front10_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 1, NARC_unionobj2d_onlyfront_only_front11_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 5, NARC_unionobj2d_onlyfront_only_front12_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 5, NARC_unionobj2d_onlyfront_only_front13_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 2, NARC_unionobj2d_onlyfront_only_front14_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_UNIONOBJ, 3, NARC_unionobj2d_onlyfront_only_front15_NCGR, -8, -6 },

	{ WFLBY_TRCARD_RIREKI_RES_ICON, 0, NARC_wifi_lobby_other_profile_icon1_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 1, NARC_wifi_lobby_other_profile_icon2_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 0, NARC_wifi_lobby_other_profile_icon3_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 0, NARC_wifi_lobby_other_profile_icon6_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 1, NARC_wifi_lobby_other_profile_icon5_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 0, NARC_wifi_lobby_other_profile_icon4_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 1, NARC_wifi_lobby_other_profile_icon7_NCGR, 0, 0 },
	{ WFLBY_TRCARD_RIREKI_RES_ICON, 0, NARC_wifi_lobby_other_profile_icon8_NCGR, 0, 0 },
};
enum{
	WFLBY_TRCARD_RIREKI_SEX_MAN,	
	WFLBY_TRCARD_RIREKI_SEX_GIRL,	
	WFLBY_TRCARD_RIREKI_SEX_NUM,	
};
static const WFLBY_TRCARD_RIREKI_CHAR sc_WFLBY_TRCARD_RIREKI_HERO_CHAR[ WFLBY_TRCARD_RIREKI_SEX_NUM ] = {
	{ WFLBY_TRCARD_RIREKI_RES_HEROBOY, 0, NARC_unionobj2d_onlyfront_pl_boy01_NCGR, -8, -6 },
	{ WFLBY_TRCARD_RIREKI_RES_HEROGIRL, 0, NARC_unionobj2d_onlyfront_pl_girl01_NCGR, -8, -6 },
};
static const VecFx32 sc_WFLBY_TRCARD_RIREKI_POS[ WFLBY_LASTACT_BUFFNUM ] = {
	{ FX32_CONST(32), FX32_CONST(144)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(56), FX32_CONST(144)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(80), FX32_CONST(144)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(104), FX32_CONST(144)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(128), FX32_CONST(144)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(152), FX32_CONST(144)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },

	{ FX32_CONST(32), FX32_CONST(176)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(56), FX32_CONST(176)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(80), FX32_CONST(176)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(104), FX32_CONST(176)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(128), FX32_CONST(176)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
	{ FX32_CONST(152), FX32_CONST(176)+WFLBY_ROOM_CLACT_SUBSURFACE_Y, 0 },
};


//-------------------------------------
///	�K�W�F�b�g�{�^��
//=====================================
enum{
	WFLBY_ROOM_UNDERWIN_BTTN_MODE_GADGET,		// �K�W�F�b�g
	WFLBY_ROOM_UNDERWIN_BTTN_MODE_FLOAT,		// �t���[�g
};
enum{	// �A�j���t���[��
	WFLBY_ROOM_UNDERWIN_BTTN_ANM_UP,
	WFLBY_ROOM_UNDERWIN_BTTN_ANM_DOWN,
	WFLBY_ROOM_UNDERWIN_BTTN_ANM_CENTER,
	WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM,
};
enum{	//  �{�^������X�e�[�^�X
	WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON,		// �����đ��v
	WFLBY_ROOM_UNDERWIN_BTTN_STATUS_WAIT,	// �������Ⴞ��
};
#define WFLBY_ROOM_UNDERWIN_BTTN_WAIT	( 90 )	// �{�^���������Ȃ��Ԋu
#define WFLBY_ROOM_UNDERWIN_BTTN_FLOAT_FRAME_NUM	(2)// �t���[�g�p�{�^���̃t���[����

#define WFLBY_UNDERWIN_BTTN_ICON_BGCG_END		( WFLBY_UNDERWIN_BGCG_END+144 )	// �w�i�ƃt���[���pCGX�̃I����


#define WFLBY_BTTN_WIN_TITLE_X		( 1 )
#define WFLBY_BTTN_WIN_TITLE_Y		( 0 )
#define WFLBY_BTTN_WIN_TITLE_SIZX	( 24 )
#define WFLBY_BTTN_WIN_TITLE_SIZY	( 3 )
#define WFLBY_BTTN_WIN_TITLE_CGX	( WFLBY_UNDERWIN_BTTN_ICON_BGCG_END )
static const BMPWIN_DAT sc_WFLBY_BTTN_WIN_DATA = {
	GF_BGL_FRAME1_S,
	WFLBY_BTTN_WIN_TITLE_X,
	WFLBY_BTTN_WIN_TITLE_Y,
	WFLBY_BTTN_WIN_TITLE_SIZX,
	WFLBY_BTTN_WIN_TITLE_SIZY,
	WFLBY_ROOM_BGSPL_FONTCL,
	WFLBY_BTTN_WIN_TITLE_CGX,
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�O���t�B�b�N�V�X�e��
//=====================================
typedef struct {
	// BG
	GF_BGL_INI*				p_bgl;

	// OAM
    CLACT_SET_PTR           p_clactset;		// �Z���A�N�^�[�Z�b�g
    CLACT_U_EASYRENDER_DATA renddata;       // �ȈՃ����_�[�f�[�^
    CLACT_U_RES_MANAGER_PTR p_resman[WFLBY_ROOM_OAM_RESNUM]; // �L�����E�p���b�g���\�[�X�}�l�[�W��
	NNSG2dCellTransferState*	p_celltransarray;		///< �Z��Vram�]���}�l�[�W���[�̈�

	// 3D
	GF_G3DMAN*		p_3dman;
} WFLBY_GRAPHICCONT;


//-------------------------------------
///	��b���b�Z�[�W�\��
//=====================================
typedef struct {
	GF_BGL_BMPWIN	win;
	u32				msgno;
	u32				msgwait;
	STRBUF*			p_str;
	void*			p_timewait;
} WFLBY_ROOM_TALKMSG;

//-------------------------------------
///	���X�g�\��
//=====================================
typedef struct {
	BMPLIST_HEADER	data;
	GF_BGL_BMPWIN	win;
	BMPLIST_WORK*	p_listwk;
	BMP_MENULIST_DATA*	p_bmplist;
	u16					bmplistnum;
	u16					list_num;

	// �㉺�@�Z���A�N�^�[
	BOOL				clact_draw;
	CLACT_U_RES_OBJ_PTR		resObj[WFLBY_LISTWIN_CLACT_RESNUM];	// ���ꂼ��̃��\�[�X�I�u�W�F��
	CLACT_WORK_PTR clact[ WFLBY_LISTWIN_CLACT_OBJNUM ];
} WFLBY_ROOM_LISTWIN;

//-------------------------------------
///	YESNOWIN
//=====================================
typedef struct {
	BMP_MENULIST_DATA*	p_bmplist;		// ���X�g
	BMPLIST_HEADER		bmplist;
} WFLBY_ROOM_YESNOWIN;


//-------------------------------------
///	�G���[���b�Z�[�W�\��
//=====================================
typedef struct {
	GF_BGL_BMPWIN	win;
} WFLBY_ROOM_ERRMSG;

//-------------------------------------
///	�T�u�E�B���h�E�\��
//=====================================
typedef struct {
	GF_BGL_BMPWIN	win;
} WFLBY_ROOM_SUBWIN;

//-------------------------------------
///	�L�ꃁ�b�Z�[�W�f�[�^
//=====================================
typedef struct {
	WORDSET*			p_wordset;
	MSGDATA_MANAGER*	p_msgman[WFLBY_DEFMSG_TYPE_NUM];
	STRBUF*				p_str;
	STRBUF*				p_tmp;
} WFLBY_ROOM_DEFMSG;

//-------------------------------------
///	�K�W�F�b�g�{�^��
//=====================================
typedef struct {
	s16	lock_wait;
	u8 seq;
	u8 bttnevent;
	u16 lastbttnevent;
	u16 bttnstop;		// �{�^����~��TRUE
	u16 mode;			// �{�^�����[�h	WFLBY_ROOM_UNDERWIN_BTTN_MODE_�`
	u16 bttn_itemno;	// �{�^�����Ή����Ă���A�C�e���̒l�imode�ɂ���Ē��̒l�̈Ӗ����ς��܂��j

	// �E�B���h�E
	GF_BGL_BMPWIN		win;

	// �{�^���}�l�[�W��
	BUTTON_MAN*	p_bttnman;

	// ���\�[�X
	void*				p_scrnbuff[ WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM ];
	NNSG2dScreenData*	p_scrn[ WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM ];
} WFLBY_GADGET_BTTN;

//-------------------------------------
///	�g���[�i�[�J�[�h
//=====================================
typedef struct {
	u32 aikotoba_draw;
	
	// �E�B���h�E
	GF_BGL_BMPWIN		win[WFLBY_TRCARD_WIN_NUM];


	// ���ȉ���ʂ�\�����邽�тɍX�V�����������
	// �A�C�e���A�C�R��
	CLACT_WORK_PTR		p_itemicon;
	CLACT_U_RES_OBJ_PTR p_itemiconres[WFLBY_ROOM_OAM_RESNUM];

	CLACT_WORK_PTR		p_tr;
	CLACT_U_RES_OBJ_PTR p_trres[WFLBY_ROOM_OAM_RESNUM];


	// �������\�[�X
	CLACT_WORK_PTR		p_rireki[WFLBY_LASTACT_BUFFNUM];
	CLACT_U_RES_OBJ_PTR p_rirekires_pal[WFLBY_TRCARD_RIREKI_RES_NUM];
	CLACT_U_RES_OBJ_PTR p_rirekires_cg[WFLBY_LASTACT_BUFFNUM];
	CLACT_U_RES_OBJ_PTR p_rirekires_cell[WFLBY_TRCARD_RIREKI_RES_NUM];
	CLACT_U_RES_OBJ_PTR p_rirekires_anm[WFLBY_TRCARD_RIREKI_RES_NUM];

} WFLBY_TR_CARD;

//-------------------------------------
///	����ʁ@���[�N
//=====================================
typedef struct {
	u8					seq;			// ����V�[�P���X
	u8					now;			// ���̕`����
	u8					req;			// �`��ύX���N�G�X�g
	u8					req_tridx;		// ���N�G�X�g���󂯂��g���[�iIDX
	u16					req_aikotoba;	// �����t���Ђ傤������̂�
	u16					bttn_start;		// ������ŃK�W�F�b�g�擾��TRUE�ɂȂ�
	WFLBY_TR_CARD		tr_card;
	WFLBY_GADGET_BTTN	bttn;
	ARCHANDLE*			p_handle;

	// �t���[�g�A�j���I���`�F�b�N�p
	u16		flt_float_idx;
	u8		flt_anm_idx;
	u8		flt_anm_flag;

	// VBlank�p���b�g�]���f�[�^
	// VBLANK�]���^�X�N���ł̂ݎg�p�\	���̑��̏ꏊ�ł͕s��l
	u16					dataidx;
	u16					heapID;
} WFLBY_UNDER_WIN;

//-------------------------------------
///	�eNPC���[�J����Ԃł̔����\�t���O
//=====================================
typedef struct {
	u8 talk_busy[ WFLBY_PLAYER_MAX ];			// ��b���BUSY
	u8 info_draw_busy[ WFLBY_PLAYER_MAX ];		// ���̕\��BUSY
} WFLBY_NPC_BUSY;

//-------------------------------------
///	�L�ꃏ�[�N
//=====================================
typedef struct _WFLBY_ROOMWK{
	// �e��t���O
	u8 end_flag;		// �I���t���O
	u8 end_ret;			// WFLBY_ROOM_RET����
	u8 plno;			// �v���C���[�i���o�[
	u8 fade_flag;		// �t�F�[�h��TRUE
	u8 err_check:4;		// �ʐM�G���[���`�F�b�N���邩
	u8 err_flag:4;		// �ʐM�G���[�t���O
	u8 evno;			// �ŋߋN�������C�x���g		�C�x���g�V�X�e�����ł̃`�F�b�N�p
	u8 timeout;			// �^�C���A�E�g
	u8 timeout_off;		// �^�C���A�E�g�`�F�b�N���s�����̃t���O
	s32 msg_wait;

#if PL_T0867_080716_FIX
	// ��l�����A�C�x���g��A�ړ�����\��������̂��H�Ƃ����t���O
	// ���l����b�W�����v���s�����Ƃ��ł���̂��Ɏg�p
	BOOL player_ev_after_move;
#endif

	// �eNPC�����\�t���O
	WFLBY_NPC_BUSY npc_busy;
 
	// �V�X�e�����
	WFLBY_SYSTEM* p_system;

	// �L��Z�[�u�f�[�^
	WFLBY_ROOM_SAVE* p_save;

	// �O���t�B�b�N�V�X�e��������
	WFLBY_GRAPHICCONT	graphic;

	// �������
	WFLBY_UNDER_WIN		under_win;

	// ���b�Z�[�W�V�X�e��
	WFLBY_ROOM_DEFMSG	def_msg;

	// �E�B���h�E
	WFLBY_ROOM_TALKMSG	talkwin;	// ��b�E�B���h�E
	WFLBY_ROOM_TALKMSG	boardwin;	// �ŔE�B���h�E
	WFLBY_ROOM_LISTWIN	listwin;	// ���X�g�E�B���h�E
	WFLBY_ROOM_ERRMSG	errwin;		// �G���[�E�B���h�E
	WFLBY_ROOM_YESNOWIN yesnowin;	// YESNO�E�B���h�E
	WFLBY_ROOM_SUBWIN	subwin;		// �T�u�E�B���h�E

	// �K�W�F�b�g
	WFLBY_GADGET*			p_gadget;
	BOOL					gadget_flag;	// �K�W�F�b�g����t���O

	// �^�C���C�x���g
	WFLBY_TIMEEVENT*		p_timeevent;

	// �e�Ǘ��V�X�e��
	WFLBY_3DOBJCONT*		p_objcont;
	WFLBY_3DMAPOBJ_CONT*	p_mapobjcont;
	WFLBY_MAPCONT*			p_mapcont;
	WFLBY_CAMERA*			p_camera;
	WFLBY_EVENT*			p_event;

	// �Z�[�u�f�[�^
	MYSTATUS*			p_mystatus;
} WFLBY_ROOMWK;



//-----------------------------------------------------------------------------
/**
 *				�f�[�^�e�[�u��
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�O���t�B�b�N�֘A
//=====================================
// �G�b�W�}�[�L���O�ݒ�
//�G�b�W�}�[�L���O�e�[�u��
static const GXRgb sc_WFLBY_ROOM_EDGECOLOR[8] = {
    GX_RGB(0, 0, 0),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4),
    GX_RGB(4, 4, 4)
};
// �o���N�ݒ�
static const GF_BGL_DISPVRAM sc_WFLBY_ROOM_BANK = {
//	GX_VRAM_BG_32_FG,				// ���C��2D�G���W����BG
	GX_VRAM_BG_128_B,				// ���C��2D�G���W����BG
	GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
	GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
	GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
//	GX_VRAM_OBJ_128_B,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJ_32_FG,				// ���C��2D�G���W����OBJ
	GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
	GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
	GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_VRAM_TEX_01_AC,				// �e�N�X�`���C���[�W�X���b�g
	GX_VRAM_TEXPLTT_0123_E			// �e�N�X�`���p���b�g�X���b�g
};

// BG�ݒ�
static const GF_BGL_SYS_HEADER sc_BGINIT = {
	GX_DISPMODE_GRAPHICS,
	GX_BGMODE_0,
	GX_BGMODE_0,
	GX_BG0_AS_3D
};

// BG�R���g���[��
static const u32 sc_WFLBY_ROOM_BGCNT_FRM[ WFLBY_ROOM_BGCNT_NUM ] = {
	GF_BGL_FRAME1_M,
	GF_BGL_FRAME0_S,
	GF_BGL_FRAME1_S,
	GF_BGL_FRAME2_S,
};
static const GF_BGL_BGCNT_HEADER sc_WFLBY_ROOM_BGCNT_DATA[ WFLBY_ROOM_BGCNT_NUM ] = {
	// ���C�����
	{	// GF_BGL_FRAME1_M
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},

	// �T�u���
	{	// GF_BGL_FRAME0_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		2, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME1_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		1, 0, 0, FALSE
	},
	{	// GF_BGL_FRAME2_S
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
};

// OAM�ݒ�
static const CHAR_MANAGER_MAKE sc_WFLBY_ROOM_CHARMAN_INIT = {
	WFLBY_ROOM_OAM_CONTNUM,
	32*1024,	// 128K->���X�g�̃J�[�\�����t�B�[���h�Ƌ��ʂŎg�p����̂�32K�ɂ���
	16*1024,	// 16K
	HEAPID_WFLBY_ROOM
};


// YESNOWIN�f�[�^
static const BMPLIST_HEADER sc_WFLBY_ROOM_YESNO_HEADER = {
	NULL,
	NULL,
	NULL,
	NULL,

	0,								//���X�g���ڐ�
	WFLBY_YESNOWIN_DATANUM,			//�\���ő區�ڐ�
	0,								//���x���\���w���W
	8,								//���ڕ\���w���W
	0,								//�J�[�\���\���w���W
	0,								//�\���x���W
	FBMP_COL_BLACK,					//�\�������F
	FBMP_COL_WHITE,					//�\���w�i�F
	FBMP_COL_BLK_SDW,				//�\�������e�F
	0,								//�����Ԋu�w
	16,								//�����Ԋu�x
	BMPLIST_NO_SKIP,				//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,					//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0,								//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)

	NULL
};



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_VBlank( void* p_work );

static void WFLBY_ROOM_GraphicInit( WFLBY_GRAPHICCONT* p_sys, SAVEDATA* p_save, u32 heapID );
static void WFLBY_ROOM_GraphicMain( WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_GraphicExit( WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_GraphicVblank( WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_DrawSys3DSetUp( void );

static void WFLBY_ROOM_RoomMain( WFLBY_ROOMWK* p_wk );
static void WFLBY_ROOM_RoomDraw( WFLBY_ROOMWK* p_wk );

static void WFLBY_ROOM_MapAnmCont( WFLBY_ROOMWK* p_wk );

static void WFLBY_ROOM_TalkWin_Init( WFLBY_ROOM_TALKMSG* p_wk, WFLBY_GRAPHICCONT* p_sys, SAVEDATA* p_save, u32 heapID );
static void WFLBY_ROOM_TalkWin_Exit( WFLBY_ROOM_TALKMSG* p_wk );
static void WFLBY_ROOM_TalkWin_Print( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str );
static void WFLBY_ROOM_TalkWin_PrintAll( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str );
static void WFLBY_ROOM_TalkWin_StartTimeWait( WFLBY_ROOM_TALKMSG* p_wk );
static void WFLBY_ROOM_TalkWin_StopTimeWait( WFLBY_ROOM_TALKMSG* p_wk );
static void WFLBY_ROOM_TalkWin_StopTimeWait_NoTrans( WFLBY_ROOM_TALKMSG* p_wk );
static BOOL WFLBY_ROOM_TalkWin_CheckTimeWait( const WFLBY_ROOM_TALKMSG* cp_wk );
static BOOL WFLBY_ROOM_TalkWin_EndWait( const WFLBY_ROOM_TALKMSG* cp_wk );
static void WFLBY_ROOM_TalkWin_Off( WFLBY_ROOM_TALKMSG* p_wk );

static void WFLBY_ROOM_TalkWin_Board_Init( WFLBY_ROOM_TALKMSG* p_wk, WFLBY_GRAPHICCONT* p_sys, SAVEDATA* p_save, u32 heapID );
static void WFLBY_ROOM_TalkWin_Board_Exit( WFLBY_ROOM_TALKMSG* p_wk );
static void WFLBY_ROOM_TalkWin_Board_Print( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str );
static void WFLBY_ROOM_TalkWin_Board_PrintAll( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str );
static void WFLBY_ROOM_TalkWin_Board_Off( WFLBY_ROOM_TALKMSG* p_wk );

static void WFLBY_ROOM_TalkWin_Board_ClearWin( WFLBY_ROOM_TALKMSG* p_wk );
static void WFLBY_ROOM_TalkWin_Board_PrintSimple( WFLBY_ROOM_TALKMSG* p_wk, u8 x, u8 y, const STRBUF* cp_str );
static void WFLBY_ROOM_TalkWin_Board_OnVReq( WFLBY_ROOM_TALKMSG* p_wk );
static void WFLBY_ROOM_TalkWin_Board_SetPalNo( WFLBY_ROOM_TALKMSG* p_wk, u16 pal );


static void WFLBY_ROOM_ListWin_Init( WFLBY_ROOM_LISTWIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID );
static void WFLBY_ROOM_ListWin_Exit( WFLBY_ROOM_LISTWIN* p_wk, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_ListWin_CreateBmpList( WFLBY_ROOM_LISTWIN* p_wk, u32 num, u32 heapID );
static void WFLBY_ROOM_ListWin_DeleteBmpList( WFLBY_ROOM_LISTWIN* p_wk );
static void WFLBY_ROOM_ListWin_SetBmpListStr( WFLBY_ROOM_LISTWIN* p_wk, const STRBUF* cp_str, u32 param );
static const  BMP_MENULIST_DATA* WFLBY_ROOM_ListWin_GetBmpList( const WFLBY_ROOM_LISTWIN* cp_wk );
static BOOL WFLBY_ROOM_ListWin_CheckBmpListParam( const WFLBY_ROOM_LISTWIN* cp_wk, u32 param );
static void WFLBY_ROOM_ListWin_Start( WFLBY_ROOM_LISTWIN* p_wk, const BMPLIST_HEADER* cp_data, WFLBY_GRAPHICCONT* p_sys, u16 list_p, u16 cursor_p, u32 heapID, u8 cx, u8 cy, u8 szcx );
static u32 WFLBY_ROOM_ListWin_Main( WFLBY_ROOM_LISTWIN* p_wk );
static void WFLBY_ROOM_ListWin_End( WFLBY_ROOM_LISTWIN* p_wk, u16* p_list_p, u16* p_cursor_p );
static void WFLBY_ROOM_ListWin_CurCallBack( BMPLIST_WORK* p_wk,u32 param,u8 mode );
static void WFLBY_ROOM_ListWin_SetClactDraw( WFLBY_ROOM_LISTWIN* p_wk, BOOL flag );
static void WFLBY_ROOM_ListWin_ContClactDraw( WFLBY_ROOM_LISTWIN* p_wk );

static void WFLBY_ROOM_SubWin_Init( WFLBY_ROOM_SUBWIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID );
static void WFLBY_ROOM_SubWin_Exit( WFLBY_ROOM_SUBWIN* p_wk );
static void WFLBY_ROOM_SubWin_Start( WFLBY_ROOM_SUBWIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID, u8 cx, u8 cy, u8 szcx, u8 szcy );
static void WFLBY_ROOM_SubWin_End( WFLBY_ROOM_SUBWIN* p_wk );
static void WFLBY_ROOM_SubWin_PrintAll( WFLBY_ROOM_SUBWIN* p_wk, const STRBUF* cp_str, u8 x, u8 y );
static void WFLBY_ROOM_SubWin_Clear( WFLBY_ROOM_SUBWIN* p_wk );
static void WFLBY_ROOM_SubWin_ClearRect( WFLBY_ROOM_SUBWIN* p_wk, u16 x, u16 y, u16 sx, u16 sy );



static void WFLBY_ROOM_YesNoWin_Init( WFLBY_ROOM_YESNOWIN* p_wk, WFLBY_ROOM_DEFMSG* p_msg, u32 heapID );
static void WFLBY_ROOM_YesNoWin_Exit( WFLBY_ROOM_YESNOWIN* p_wk );


static void WFLBY_ROOM_ErrWin_Init( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID );
static void WFLBY_ROOM_ErrWin_Exit( WFLBY_ROOM_ERRMSG* p_wk );
static void WFLBY_ROOM_ErrWin_DrawErr( WFLBY_ROOM_ERRMSG* p_wk, const STRBUF* cp_str );
static void WFLBY_ROOM_ErrWin_DrawDwcErr( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_ROOM_DEFMSG* p_msg );
static void WFLBY_ROOM_ErrWin_DrawLobbyErr( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_ROOM_DEFMSG* p_msg, int errno );
static void WFLBY_ROOM_ErrWin_DrawSystemErr( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_ROOM_DEFMSG* p_msg, WFLBY_SYSTEM_ERR_TYPE type );


static void WFLBY_ROOM_Msg_Init( WFLBY_ROOM_DEFMSG* p_wk, u32 heapID );
static void WFLBY_ROOM_Msg_Exit( WFLBY_ROOM_DEFMSG* p_wk );
static STRBUF* WFLBY_ROOM_Msg_Get( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_DEFMSG_TYPE type, u32 strid );
static void WFLBY_ROOM_Msg_SetNumber( WFLBY_ROOM_DEFMSG* p_wk, u32  num, u32 keta, u32 bufid, NUMBER_DISPTYPE disptype );
static void WFLBY_ROOM_Msg_SetPlayerName( WFLBY_ROOM_DEFMSG* p_wk, const MYSTATUS* cp_mystatus, u32 bufid );
static void WFLBY_ROOM_Msg_SetMinigame( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_GAMETYPE type, u32 bufid );
static void WFLBY_ROOM_Msg_SetTimeevent( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_EVENTGMM_TYPE type, u32 bufid );
static void WFLBY_ROOM_Msg_SetItem( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_ITEMTYPE item, u32 bufid );
static void WFLBY_ROOM_Msg_SetCountry( WFLBY_ROOM_DEFMSG* p_wk, u32 country, u32 bufid );
static void WFLBY_ROOM_Msg_SetArea( WFLBY_ROOM_DEFMSG* p_wk, u32 country, u32 area, u32 bufid );
static void WFLBY_ROOM_Msg_SetPMSWord( WFLBY_ROOM_DEFMSG* p_wk, u32 bufID, PMS_WORD word );
static void WFLBY_ROOM_Msg_ClearWordSet( WFLBY_ROOM_DEFMSG* p_wk );


static void WFLBY_ROOM_GadGet_SetFlag( WFLBY_ROOMWK* p_wk );

//-------------------------------------
///	�����
//=====================================
static void WFLBY_ROOM_UNDERWIN_Init( WFLBY_UNDER_WIN* p_wk, const WFLBY_ROOM_SAVE* cp_save, WFLBY_GRAPHICCONT* p_sys, const MYSTATUS* cp_mystatus, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Exit( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_Main( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOMWK* p_room, BOOL no_input, u32 heapID );
static BOOL WFLBY_ROOM_UNDERWIN_CheckFade( const WFLBY_UNDER_WIN* cp_wk );
static u32 WFLBY_ROOM_UNDERWIN_GetSeq( const WFLBY_UNDER_WIN* cp_wk );
static void WFLBY_ROOM_UNDERWIN_ReqTrCard( WFLBY_UNDER_WIN* p_wk, u32 idx, BOOL aikotoba );
static void WFLBY_ROOM_UNDERWIN_ChangeItemTrCard( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 item, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_ReqBttn( WFLBY_UNDER_WIN* p_wk );
static void WFLBY_ROOM_UNDERWIN_ReqBttnFloat( WFLBY_UNDER_WIN* p_wk );
static u32 WFLBY_ROOM_UNDERWIN_GetTrCardPlIdx( const WFLBY_UNDER_WIN* cp_wk );
static BOOL WFLBY_ROOM_UNDERWIN_CheckTrCardDraw( const WFLBY_UNDER_WIN* cp_wk );
static BOOL WFLBY_ROOM_UNDERWIN_CheckTrCardCanOff( const WFLBY_UNDER_WIN* cp_wk );
static BOOL WFLBY_ROOM_UNDERWIN_CheckTrCardAikotobaDraw( const WFLBY_UNDER_WIN* cp_wk );
#if PL_T0862_080712_FIX
static BOOL WFLBY_ROOM_UNDERWIN_CheckReqNone( const WFLBY_UNDER_WIN* cp_wk );
#endif
static void WFLBY_ROOM_UNDERWIN_StartTrCard( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, BOOL mydata, u32 heapID, const WFLBY_USER_PROFILE* cp_profile, const WFLBY_USER_PROFILE* cp_myprofile, BOOL vip, BOOL aikotoba, const WFLBY_AIKOTOBA_DATA* cp_aikotoba, BOOL draw_item );
static void WFLBY_ROOM_UNDERWIN_EndTrCard( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_StartBttn( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, u32 heapID, const WFLBY_USER_PROFILE* cp_profile );
static void WFLBY_ROOM_UNDERWIN_StartBttnFloat( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID, u32 idx );
static void WFLBY_ROOM_UNDERWIN_EndBttn( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_SetBttnStop( WFLBY_UNDER_WIN* p_wk, BOOL stop );
static void WFLBY_ROOM_UNDERWIN_PalTrans( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 dataidx, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_PalTransVTcb( TCB_PTR p_tcb, void* p_work );

// common
static void WFLBY_ROOM_UNDERWIN_Common_Init( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 sex, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Common_Exit( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_Common_LoadScrn( WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 scrnid, u32 bg_frame, u32 cgx, u32 heapID );

// tr_card
static void WFLBY_ROOM_UNDERWIN_TrCard_Init( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_TrCard_Exit( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_TrCard_Main( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin );
static void WFLBY_ROOM_UNDERWIN_TrCard_Start( WFLBY_UNDER_WIN* p_ugwk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, BOOL mydata, ARCHANDLE* p_handle, u32 heapID, const WFLBY_USER_PROFILE* cp_profile, const WFLBY_USER_PROFILE* cp_myprofile, BOOL vip, BOOL aikotoba, const WFLBY_AIKOTOBA_DATA* cp_aikotoba, BOOL item_draw );
static void WFLBY_ROOM_UNDERWIN_TrCard_WriteWazaType( WFLBY_UNDER_WIN* p_ugwk, WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID, const WFLBY_USER_PROFILE* cp_profile );
static void WFLBY_ROOM_UNDERWIN_TrCard_End( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_TrCard_ChangeItem( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, WFLBY_ITEMTYPE itemno );
static BOOL WFLBY_ROOM_UNDERWIN_TrCard_CheckAikotobaDraw( const WFLBY_TR_CARD* cp_wk );
static void WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Init( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, WFLBY_ITEMTYPE itemno, BOOL draw_flag );
static void WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Exit( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_TrCard_TrView_Init( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, u32 trtype );
static void WFLBY_ROOM_UNDERWIN_TrCard_TrView_Exit( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_TrCard_TrView_PallColorChange( NNSG2dPaletteData* p_pltt );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinClear( WFLBY_TR_CARD* p_wk );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg, u32 winno, u32 strid, u8 x, u8 y, GF_PRINTCOLOR col );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinPrintRightSide( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg, u32 winno, u32 strid, u8 x, u8 y, GF_PRINTCOLOR col );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinOn( WFLBY_TR_CARD* p_wk, u32 idx );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetName( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg,const MYSTATUS* cp_mystatus );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetCountry( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg,const WFLBY_USER_PROFILE* cp_profile );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetLastAction( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg,const WFLBY_USER_PROFILE* cp_profile );
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetAikotoba( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg, const WFLBY_AIKOTOBA_DATA* cp_aikotoba );
static void WFLBY_ROOM_UNDERWIN_TrCard_InitRireki( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, const WFLBY_USER_PROFILE* cp_profile, const WFLBY_USER_PROFILE* cp_myprofile );
static void WFLBY_ROOM_UNDERWIN_TrCard_ExitRireki( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys );
static const WFLBY_TRCARD_RIREKI_CHAR* WFLBY_ROOM_UNDERWIN_TrCard_GetRirekiData( WFLBY_LASTACTION_TYPE last_act, s32 userid, s32 myid, u32 sex );

// button
static void WFLBY_ROOM_UNDERWIN_Button_Init( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Button_Exit( WFLBY_GADGET_BTTN* p_wk );
static BOOL WFLBY_ROOM_UNDERWIN_Button_Main( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys,  ARCHANDLE* p_handle, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Button_Start( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, const WFLBY_USER_PROFILE* cp_profile, ARCHANDLE* p_handle, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Button_StartFloat( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 idx, ARCHANDLE* p_handle, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Button_End( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys );
static void WFLBY_ROOM_UNDERWIN_Button_DrawButton( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 idx, ARCHANDLE* p_handle, u32 heapID );
static void WFLBY_ROOM_UNDERWIN_Button_BttnCallBack( u32 bttnno, u32 event, void* p_work );
static void WFLBY_ROOM_UNDERWIN_Button_BttnLockClear( WFLBY_GADGET_BTTN* p_wk );


#ifdef PM_DEBUG
PROC_RESULT WFLBY_ROOM_InitDebug(PROC* p_proc, int* p_seq)
{
	FS_EXTERN_OVERLAY(wifilobby_common);
	FS_EXTERN_OVERLAY(wifi_2dmapsys);

	Overlay_Load( FS_OVERLAY_ID(wifilobby_common), OVERLAY_LOAD_NOT_SYNCHRONIZE);
	Overlay_Load( FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);

	return WFLBY_ROOM_Init( p_proc, p_seq );
}

PROC_RESULT WFLBY_ROOM_ExitDebug(PROC* p_proc, int* p_seq)
{
	BOOL result;
	
	FS_EXTERN_OVERLAY(wifilobby_common);
	FS_EXTERN_OVERLAY(wifi_2dmapsys);

	result  = WFLBY_ROOM_Exit( p_proc, p_seq );

	Overlay_UnloadID( FS_OVERLAY_ID(wifilobby_common) );
	Overlay_UnloadID( FS_OVERLAY_ID(wifi_2dmapsys) );

	return result;
}
#endif


//----------------------------------------------------------------------------
/**
 *	@brief	�����V�X�e��	������
 *
 *	@param	p_proc		�v���Z�X���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_ROOM_Init(PROC* p_proc, int* p_seq)
{
	WFLBY_ROOMWK* p_wk;
	WFLBY_ROOM_PARAM* p_param;

#ifdef WFLBY_DEBUG_ROOM_CAMERA
	OS_TPrintf( "���J��������\n" );
	OS_TPrintf( "	���ˉe�Ɠ����ˉe��؂�ւ���	SELECT\n" );
#endif

#ifdef WFLBY_DEBUG_ROOM_ITEMCHG
//	OS_TPrintf( "���^�b�`�g�C����" );
//	OS_TPrintf( " �^�b�`�g�C�����̂��̂ɕς���	L\n" );
#endif

	OS_TPrintf( "sub char max %d\n", WFLBY_TRCARD_WIN_VIPAIKOTOBA_CGX );

	// �p�����[�^�擾
	p_param = PROC_GetParentWork( p_proc );

	// �q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WFLBY_ROOM, 0x3a000 );
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WFLBY_ROOMGRA, 0x3d000 );

	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(WFLBY_ROOMWK), HEAPID_WFLBY_ROOM );
	memset( p_wk, 0, sizeof(WFLBY_ROOMWK) );

	// �����ۑ��f�[�^�ݒ���ۑ�
	p_wk->p_save = &p_param->save;

	// �V�X�e�������擾
	p_wk->p_system = p_param->p_system;

	// �v���C���[�i���o�[�ݒ�
	p_wk->plno = WFLBY_SYSTEM_GetMyIdx( p_wk->p_system );
	GF_ASSERT( WFLBY_SYSTEM_GetMyIdx( p_wk->p_system ) != DWC_LOBBY_USERIDTBL_IDX_NONE );

	// ���b�Z�[�W�\���֌W��ݒ�
	MsgPrintSkipFlagSet(MSG_SKIP_ON);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);


	// �Z�[�u�f�[�^�ݒ�
	p_wk->p_mystatus = SaveData_GetMyStatus( p_param->p_save );

	// �O���t�B�b�N�V�X�e��������
	WFLBY_ROOM_GraphicInit( &p_wk->graphic, p_param->p_save, HEAPID_WFLBY_ROOM );

	// �}�b�v�Ǘ��V�X�e���쐬
	p_wk->p_mapcont = WFLBY_MAPCONT_Init( HEAPID_WFLBY_ROOM );


	// �I�u�W�F�N�g�Ǘ��V�X�e���쐬
	{
		u32 sex;
		sex = MyStatus_GetMySex( p_wk->p_mystatus );
		p_wk->p_objcont = WFLBY_3DOBJCONT_Init( WFLBY_ROOM_3DOBJCONT_WKNUM, sex, 
				p_wk->p_mapcont, HEAPID_WFLBY_ROOM, HEAPID_WFLBY_ROOMGRA );	

	}

	// �}�b�v�I�u�W�F�Ǘ��V�X�e���쐬
	p_wk->p_mapobjcont = WFLBY_3DMAPOBJCONT_Init( p_param->season, p_param->room, p_wk->p_mapcont, HEAPID_WFLBY_ROOM, HEAPID_WFLBY_ROOMGRA );
	

	// �J����������
	p_wk->p_camera = WFLBY_CAMERA_Init( HEAPID_WFLBY_ROOM );

	// �E�B���h�E
	WFLBY_ROOM_TalkWin_Init( &p_wk->talkwin, &p_wk->graphic, p_param->p_save, HEAPID_WFLBY_ROOM );
	WFLBY_ROOM_TalkWin_Board_Init( &p_wk->boardwin, &p_wk->graphic, p_param->p_save, HEAPID_WFLBY_ROOM );
	WFLBY_ROOM_ListWin_Init( &p_wk->listwin, &p_wk->graphic, HEAPID_WFLBY_ROOM );
	WFLBY_ROOM_ErrWin_Init( &p_wk->errwin, &p_wk->graphic, HEAPID_WFLBY_ROOM );
	WFLBY_ROOM_SubWin_Init( &p_wk->subwin, &p_wk->graphic, HEAPID_WFLBY_ROOM );

	// �G���[���b�Z�[�W
	WFLBY_ROOM_Msg_Init( &p_wk->def_msg, HEAPID_WFLBY_ROOM );

	// YESNO�E�B���h�E
	WFLBY_ROOM_YesNoWin_Init( &p_wk->yesnowin, &p_wk->def_msg, HEAPID_WFLBY_ROOM );


	// ������ʏ�����
	WFLBY_ROOM_UNDERWIN_Init( &p_wk->under_win, p_wk->p_save, &p_wk->graphic, p_wk->p_mystatus, HEAPID_WFLBY_ROOM );

	// �^�b�`�g�C��\���ł����ԂȂ�\������
	{
		const WFLBY_USER_PROFILE* cp_profile;

		cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_wk->p_system );
		if( WFLBY_SYSTEM_CheckGetItem( cp_profile ) == TRUE ){
			WFLBY_ROOM_UNDERWIN_StartBttn( &p_wk->under_win, &p_wk->graphic, &p_wk->def_msg,
					HEAPID_WFLBY_ROOM, 
					cp_profile );
		}
	}

	// �C�x���g�V�X�e���쐬
	p_wk->p_event = WFLBY_EVENT_Init( p_wk, HEAPID_WFLBY_ROOM );

	// �Q�[���C�x���g�J�n
	WFLBY_EV_DEF_StartPlayer( p_wk );
	WFLBY_EV_DEF_StartNpc( p_wk );

	// �K�W�F�b�g������
	p_wk->p_gadget = WFLBY_GADGET_Init( p_wk->p_system, p_wk->p_mapcont, p_wk->p_camera, p_wk->p_objcont, HEAPID_WFLBY_ROOM, HEAPID_WFLBY_ROOMGRA );

	// �^�C���C�x���g������
	p_wk->p_timeevent = WFLBY_TIMEEVENT_Init( HEAPID_WFLBY_ROOM, p_wk );

	// VBlank�֐��ݒ�
	sys_VBlankFuncChange( WFLBY_ROOM_VBlank, p_wk );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~
	
	return	PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����V�X�e��	���C��
 */	
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_ROOM_Main(PROC* p_proc, int* p_seq)
{
	WFLBY_ROOMWK* p_wk;
	WFLBY_ROOM_PARAM* p_param;
	BOOL result;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

	switch( *p_seq ){
	case WFLBY_ROOM_MAINSEQ_FADEIN:
		WIPE_SYS_Start(WIPE_PATTERN_WMS, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );
		p_wk->fade_flag = TRUE;
		(*p_seq) ++;
		break;
		
	case WFLBY_ROOM_MAINSEQ_FADEIN_WAIT:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			p_wk->fade_flag = FALSE;
			(*p_seq) ++;
		}
		break;
		
	case WFLBY_ROOM_MAINSEQ_MAIN:

#ifdef WFLBY_DEBUG_ROOM_WLDTIMER_AUTO
		if( WFLBY_DEBUG_ROOM_WFLBY_TIMER_AUTO == TRUE ){
			sys.trg		|= PAD_KEY_RIGHT;
			sys.cont	|= PAD_KEY_RIGHT;

			if( (gf_mtRand() % 200) == 0 ){
				sys.trg		|= PAD_BUTTON_A;
				sys.cont	|= PAD_BUTTON_A;
			}
		}
#endif

#ifdef WFLBY_DEBUG_ROOM_MINIGAME_AUTO
		if( WFLBY_DEBUG_ROOM_MINIGAME_AUTO_FLAG == TRUE ){
			if( (gf_mtRand() % 10) == 0 ){
				sys.trg		|= (PAD_KEY_LEFT | PAD_BUTTON_A);
				sys.cont	|= PAD_KEY_LEFT;
			}
		}
#endif

#ifdef _WIFI_DEBUG_TUUSHIN
		{
			static int roop = 100;

			roop --;
			// �I����
			if( roop <= 0 ){
				(*p_seq) = WFLBY_ROOM_MAINSEQ_FADEOUT;
				break;
			}
		}
#endif
		

		// �G���[���������Ă�����I��
		if( p_wk->err_check == TRUE ){

			if( (WFLBY_ERR_CheckError() == TRUE) || 
				(WFLBY_SYSTEM_GetError( p_wk->p_system ) != WFLBY_SYSTEM_ERR_NONE) ){
				p_wk->err_flag = TRUE;
			}
		}

		// �^�C���A�E�g�I���`�F�b�N
		if( p_wk->timeout_off == FALSE ){
			if( WFLBY_SYSTEM_Event_GetEnd( p_wk->p_system ) == TRUE ){
				// �^�C���A�E�g��Ԃ̏I��
				p_wk->timeout	= TRUE;
				WFLBY_ROOM_SetEndRet( p_wk, WFLBY_ROOM_RET_ROOM_TIMEOUT );
			}
		}

		// �I���`�F�b�N
		if( (p_wk->end_flag == TRUE) || (p_wk->err_flag == TRUE) || (p_wk->timeout == TRUE) ){

			// �G���[���
			if( p_wk->err_flag ){
				(*p_seq) = WFLBY_ROOM_MAINSEQ_ERR_MSG;

			// �^�C���A�E�g���
			}else if( p_wk->timeout == TRUE ){
				(*p_seq) = WFLBY_ROOM_MAINSEQ_TIMEOUT_MSG;

			// �ʏ�I��
			}else{
				(*p_seq) = WFLBY_ROOM_MAINSEQ_FADEOUT;
			}
//			p_wk->fade_flag = TRUE;
		}
		break;

	// �G���[���b�Z�[�W
	case WFLBY_ROOM_MAINSEQ_ERR_MSG:
		{
			// �S���b�Z�[�W������
			WFLBY_ROOM_TalkWin_Off( &p_wk->talkwin );
			WFLBY_ROOM_ListWin_End( &p_wk->listwin, NULL, NULL );
			WFLBY_ROOM_SubWin_End( &p_wk->subwin );
			WFLBY_ROOM_YesNoWin_Exit( &p_wk->yesnowin );
			
			if( CommStateIsWifiError() ){
				// DWC�n�G���[
				WFLBY_ROOM_ErrWin_DrawDwcErr( &p_wk->errwin, &p_wk->def_msg);
			}
			else if( CommStateWifiLobbyError() ){
				// ���r�[�n�G���[
				WFLBY_ROOM_ErrWin_DrawLobbyErr( &p_wk->errwin, &p_wk->def_msg, 
						DWC_LOBBY_GetErr() );
			}
			else {
				// ���r�[�V�X�e���n�G���[
				WFLBY_ROOM_ErrWin_DrawSystemErr( &p_wk->errwin, &p_wk->def_msg, 
					WFLBY_SYSTEM_GetError( p_wk->p_system )	);
			}
			(*p_seq) = WFLBY_ROOM_MAINSEQ_ERR_MSGWAIT;
		}
		break;
		
	// ���b�Z�[�W�I���҂�
	case WFLBY_ROOM_MAINSEQ_ERR_MSGWAIT:
		if( sys.trg & PAD_BUTTON_DECIDE ){
			(*p_seq) = WFLBY_ROOM_MAINSEQ_FADEOUT;
		}
		break;

	// �^�C���A�E�g���b�Z�[�W
	case WFLBY_ROOM_MAINSEQ_TIMEOUT_MSG:
		{
			// �S���b�Z�[�W������
			WFLBY_ROOM_TalkWin_Off( &p_wk->talkwin );
			WFLBY_ROOM_ListWin_End( &p_wk->listwin, NULL, NULL );
			WFLBY_ROOM_SubWin_End( &p_wk->subwin );
			WFLBY_ROOM_YesNoWin_Exit( &p_wk->yesnowin );

			WFLBY_ROOM_TalkWin_Print( &p_wk->talkwin, 
					WFLBY_ROOM_Msg_Get( &p_wk->def_msg, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_end_01 ) );
			
			(*p_seq) = WFLBY_ROOM_MAINSEQ_TIMEOUT_MSGWAIT;

			p_wk->msg_wait = WFLBY_MSG_WAIT;
		}
		break;
		
	// �^�C���A�E�g���b�Z�[�W�҂�
	case WFLBY_ROOM_MAINSEQ_TIMEOUT_MSGWAIT:
		if( WFLBY_ROOM_TalkWin_EndWait( &p_wk->talkwin ) ){
			p_wk->msg_wait --;
			if( p_wk->msg_wait <= 0 ){
				(*p_seq) = WFLBY_ROOM_MAINSEQ_FADEOUT;
			}
		}
		break;
		
	case WFLBY_ROOM_MAINSEQ_FADEOUT:
		{
			u32 seq;

			// �T�u�ʏ�ԂŃt�F�[�h�̏����𕪊򂷂�
			seq = WFLBY_ROOM_UNDERWIN_GetSeq( &p_wk->under_win );

			// �m�[�}����ԂȂ烏�C�v�J�n
			if( seq == WFLBY_UNDERWIN_SEQ_NORMAL ){
				WIPE_SYS_Start(WIPE_PATTERN_WMS, 
						WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );

				p_wk->fade_flag = TRUE;
				(*p_seq) ++;
				break;
			}

			// �J�n�҂����Ȃ烁�C����ʂ������C�v
			if( seq == WFLBY_UNDERWIN_SEQ_STARTWAIT ){
				WIPE_SYS_Start(WIPE_PATTERN_WMS, 
						WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WFLBY_ROOM );

				p_wk->fade_flag = TRUE;
				(*p_seq) ++;
				break;
			}

			// ���̑��̏ꍇ�͂���NORMAL�ɂȂ�
			
		}
		break;
		
	case WFLBY_ROOM_MAINSEQ_FADEOUT_WAIT:
		result = WIPE_SYS_EndCheck();
		if( result == TRUE ){
			p_wk->fade_flag = FALSE;
			return	PROC_RES_FINISH;
		}
		break;
	}


	// �G���[���^�C���A�E�g���ɂ͍X�V���Ȃ�
	// �������C������
	WFLBY_ROOM_RoomMain( p_wk );

	// �����`�揈��
	WFLBY_ROOM_RoomDraw( p_wk );

	// ���C�g�����i�K���ŕύX
#ifdef WFLBY_DEBUG_ROOM_LIGHT
	{
		static const WFLBY_DEBUG_ROOM_LIGHTDATA sc_WFLBY_DEBUG_Light[ 2 ] = {
			{
				{
					GX_RGB( 23,23,25 ),
					GX_RGB( 31,31,31 ),
				},
				{
					GX_RGB( 16,16,16 ),
					GX_RGB( 14,14,14 ),
					GX_RGB( 20,20,20 ),
					GX_RGB( 16,16,16 ),
				},
			},
			{
				{
					GX_RGB( 11,11,16 ),
					GX_RGB( 31,31,31 ),
				},
				{
					GX_RGB( 14,14,16 ),
					GX_RGB( 10,10,10 ),
					GX_RGB( 14,14,16 ),
					GX_RGB( 8,8,11 ),
				},
			}
		};
		static u32 WFLBY_DEBUG_LightNum = 1;
		int i;
		
		if( sys.trg & PAD_BUTTON_R ){
			for( i=0; i<2;  i++ ){
				NNS_G3dGlbLightColor( i, sc_WFLBY_DEBUG_Light[WFLBY_DEBUG_LightNum].lightcolor[i] );
			}
			NNS_G3dGlbMaterialColorDiffAmb( sc_WFLBY_DEBUG_Light[WFLBY_DEBUG_LightNum].matcolor[0],
					sc_WFLBY_DEBUG_Light[WFLBY_DEBUG_LightNum].matcolor[1],  FALSE );
			NNS_G3dGlbMaterialColorSpecEmi( sc_WFLBY_DEBUG_Light[WFLBY_DEBUG_LightNum].matcolor[2], 
					sc_WFLBY_DEBUG_Light[WFLBY_DEBUG_LightNum].matcolor[3],  FALSE );

			WFLBY_DEBUG_LightNum = (WFLBY_DEBUG_LightNum+1) % 2;
		}
	}
#endif

	// �����\��
#ifdef	WFLBY_DEBUG_ROOM_DPRINT
	if( sys.trg & PAD_BUTTON_Y ){
		NNS_GfdDumpLnkTexVramManager();
		NNS_GfdDumpLnkPlttVramManager();

		OS_TPrintf( "HEAPID_WFLBY_GLB free size 0x%x\n", sys_GetHeapFreeSize( HEAPID_WFLOBBY ) );
		OS_TPrintf( "HEAPID_WFLBY_ROOM free size 0x%x\n", sys_GetHeapFreeSize( HEAPID_WFLBY_ROOM ) );
		OS_TPrintf( "HEAPID_WFLBY_ROOMGRA free size 0x%x\n", sys_GetHeapFreeSize( HEAPID_WFLBY_ROOMGRA ) );
		OS_TPrintf( "HEAPID_CONN free size 0x%x\n", sys_GetHeapFreeSize( HEAPID_COMMUNICATION ) );
		OS_TPrintf( "HEAPID_WIFIMENU free size size 0x%x\n", sys_GetHeapFreeSize( HEAPID_WIFIMENU ) );
		OS_TPrintf( "HEAPID_APP free size size 0x%x\n", sys_GetHeapFreeSize( HEAPID_BASE_APP ) );

	}
#endif

	
	return	PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����V�X�e��	�T�u
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBY_ROOM_Exit(PROC* p_proc, int* p_seq)
{
	WFLBY_ROOMWK* p_wk;
	WFLBY_ROOM_PARAM* p_param;
	
	p_wk	= PROC_GetWork( p_proc );
	p_param = PROC_GetParentWork( p_proc );

	// �I���f�[�^�̐ݒ�
	{
		p_param->ret = p_wk->end_ret;
	}

	// ���荞�ݐݒ�
	sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	// �^�C���C�x���g�j��
	WFLBY_TIMEEVENT_Exit( p_wk->p_timeevent );

	// �K�W�F�b�g�j��
	WFLBY_GADGET_Exit( p_wk->p_gadget );

	// �C�x���g�V�X�e���j��
	WFLBY_EVENT_Exit( p_wk->p_event );

	// ������ʔj��
	WFLBY_ROOM_UNDERWIN_Exit( &p_wk->under_win, &p_wk->boardwin, &p_wk->graphic );

	// �G���[���b�Z�[�W
	WFLBY_ROOM_Msg_Exit( &p_wk->def_msg );

	// �E�B���h�E�j��
	WFLBY_ROOM_TalkWin_Exit( &p_wk->talkwin );
	WFLBY_ROOM_TalkWin_Board_Exit( &p_wk->boardwin );
	WFLBY_ROOM_ListWin_Exit( &p_wk->listwin, &p_wk->graphic );
	WFLBY_ROOM_ErrWin_Exit( &p_wk->errwin );
	WFLBY_ROOM_YesNoWin_Exit( &p_wk->yesnowin );
	WFLBY_ROOM_SubWin_Exit( &p_wk->subwin );

	// �J�����j��
	WFLBY_CAMERA_Exit( p_wk->p_camera );

	// �}�b�v�I�u�W�F�Ǘ��V�X�e���j��
	WFLBY_3DMAPOBJCONT_Exit( p_wk->p_mapobjcont );
	
	// �I�u�W�F�N�g�Ǘ��V�X�e���j��
	WFLBY_3DOBJCONT_Exit( p_wk->p_objcont );	

	// �}�b�v�Ǘ��V�X�e���j��
	WFLBY_MAPCONT_Exit( p_wk->p_mapcont );

	// �O���t�B�b�N�V�X�e���j��
	WFLBY_ROOM_GraphicExit( &p_wk->graphic );

	// ���[�N�j��
	sys_FreeMemoryEz( p_wk );

	// �q�[�v�j��
	sys_DeleteHeap( HEAPID_WFLBY_ROOM );
	sys_DeleteHeap( HEAPID_WFLBY_ROOMGRA );


	// Se��S��~
	Snd_SeStopAll( 0 );
	Snd_PMVoiceStop( 0 );

	return	PROC_RES_FINISH;
}





//----------------------------------------------------------------------------
/**
 *	@brief	�V�X�e���f�[�^���擾����
 */
//-----------------------------------------------------------------------------
WFLBY_SYSTEM* WFLBY_ROOM_GetSystemData( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_system;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Z�[�u�f�[�^���擾����
 */
//-----------------------------------------------------------------------------
WFLBY_ROOM_SAVE* WFLBY_ROOM_GetSaveData( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_save;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�l���Ǘ��V�X�e�����擾����
 */
//-----------------------------------------------------------------------------
WFLBY_3DOBJCONT* WFLBY_ROOM_GetObjCont( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_objcont;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v���Ԃ����Ǘ��V�X�e�����擾����
 */
//-----------------------------------------------------------------------------
WFLBY_3DMAPOBJ_CONT* WFLBY_ROOM_GetMapObjCont( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_mapobjcont;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�Ǘ��V�X�e�����擾����
 */
//-----------------------------------------------------------------------------
WFLBY_MAPCONT* WFLBY_ROOM_GetMapCont( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_mapcont;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�����Ǘ��V�X�e�����擾����
 */
//-----------------------------------------------------------------------------
WFLBY_CAMERA* WFLBY_ROOM_GetCamera( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_camera;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�Ǘ��V�X�e�����擾����
 */
//-----------------------------------------------------------------------------
WFLBY_EVENT* WFLBY_ROOM_GetEvent( WFLBY_ROOMWK* p_wk )
{
	return p_wk->p_event;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�`�F�b�N�t���O��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	flag	�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetErrorCheckFlag( WFLBY_ROOMWK* p_wk, BOOL flag )
{
	p_wk->err_check = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�`�F�b�N�t���O���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�`�F�b�N���Ă�
 *	@retval	FALSE	�`�F�b�N���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_GetErrorCheckFlag( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->err_check;
}


//----------------------------------------------------------------------------
/**
 *	@brief	��l���v���C���[�i���o�[���擾����
 */
//-----------------------------------------------------------------------------
u32 WFLBY_ROOM_GetPlno( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->plno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t�F�[�h�����`�F�b�N
 *	@retval	TRUE	�t�F�[�h��
 *	@retval	FALSE	�t�F�[�h���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_GetFadeFlag( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->fade_flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���t���O�擾
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	���s
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_GetEndFlag( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->end_flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	ROOM���Ă񂾃V�X�e���ɕԂ��l
 *
 *	@return	�l
 */
//-----------------------------------------------------------------------------
u8 WFLBY_ROOM_GetEndRet( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->end_ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���t���O��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetEndFlag( WFLBY_ROOMWK* p_wk, BOOL flag )
{
	p_wk->end_flag = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I������ROOM���Ă񂾃V�X�e���ɕԂ��l
 *
 *	@param	p_wk		���[�N
 *	@param	retdata		�f�[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetEndRet( WFLBY_ROOMWK* p_wk,  u8 retdata )
{
	p_wk->end_ret = retdata;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�i���o�[��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	evno		�C�x���g�i���o�[
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetEvNo( WFLBY_ROOMWK* p_wk, u8 evno )
{
	p_wk->evno = evno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�C�x���g�i���o�[���擾����
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�C�x���g�i���o�[
 */
//-----------------------------------------------------------------------------
u8 WFLBY_ROOM_GetEvNo( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->evno;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���A�E�g�`�F�b�N���~���܂�
 *		
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_OffTimeOutCheck( WFLBY_ROOMWK* p_wk )
{
	p_wk->timeout_off = TRUE;
}

#if PL_T0867_080716_FIX
//----------------------------------------------------------------------------
/**
 *	@brief	��l�����C�x���g��A�I�[�g�ňړ�����\��������̂��H��ݒ�
 *
 *	@param	cp_wk		���[�N
 *	@param	flag		�t���O
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetPlayerEventAfterMove( WFLBY_ROOMWK* p_wk, BOOL flag )
{
	p_wk->player_ev_after_move = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l�����C�x���g��A�I�[�g�ňړ�����\��������̂��H��ݒ�
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�ړ�����		���l�̉�b�W�����v�s�\
 *	@retval	FALSE	�ړ����Ȃ�		���l�̉�b�W�����v�\
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_CheckPlayerEventAfterMove( const WFLBY_ROOMWK* cp_wk )
{
	return cp_wk->player_ev_after_move;
}
#endif

//----------------------------------------------------------------------------
/**
 *	@brief	NPC��b�r�W�[��Ԑݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	plno		�v���C���[�i���o�[
 *	@param	busy		BUSY���	TRUE�FBUSY	FALSE�FNORMAL
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetNpcTalkBusy( WFLBY_ROOMWK* p_wk, u8 plno, BOOL busy )
{
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );
	p_wk->npc_busy.talk_busy[ plno ] = busy;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC��b�\���`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	plno		�v���C���[�i���o�[
 *
 *	@retval	TRUE	��b�s�\
 *	@retval	FALSE	��b�\
 */
//-----------------------------------------------------------------------------
BOOL WFBLY_ROOM_CheckNpcTalkBusy( const WFLBY_ROOMWK* cp_wk, u8 plno )
{
	return cp_wk->npc_busy.talk_busy[ plno ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC��ԕ\���\�ݒ�		NPC�̓������l����������ł���K�v������܂��B
 *
 *	@param	p_wk		���[�N
 *	@param	plno		�v���C���[�i���o�[
 *	@param	busy		BUSY���	TRUE:BUSY		FALSE:NORMAL
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SetNpcInfoDrawBusy( WFLBY_ROOMWK* p_wk, u8 plno, BOOL busy )
{
	GF_ASSERT( plno < WFLBY_PLAYER_MAX );
	p_wk->npc_busy.info_draw_busy[ plno ] = busy;
}

//----------------------------------------------------------------------------
/**
 *	@brief	NPC��ԕ\���\���`�F�b�N	NPC�̓������l����������ł���K�v������܂��B
 *
 *	@param	cp_wk		���[�N
 *	@param	plno		�v���C���[�i���o�[
 *
 *	@retval	TRUE	��b�s�\
 *	@retval	FALSE	��b�\
 */
//-----------------------------------------------------------------------------
BOOL WFBLY_ROOM_CheckNpcInfoDrawBusy( const WFLBY_ROOMWK* cp_wk, u8 plno )
{
	return cp_wk->npc_busy.info_draw_busy[ plno ];
}





//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_TALKWIN_PrintStr( WFLBY_ROOMWK* p_wk, const STRBUF* cp_str )
{
	WFLBY_ROOM_TalkWin_Print( &p_wk->talkwin, cp_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\��	��x�ɑS���\��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_TALKWIN_PrintStrAllPut( WFLBY_ROOMWK* p_wk, const STRBUF* cp_str )
{
	WFLBY_ROOM_TalkWin_PrintAll( &p_wk->talkwin, cp_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�̕\���������������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	���b�Z�[�W�I��
 *	@retval	FALSE	���b�Z�[�W�\����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_TALKWIN_WaitEnd( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_TalkWin_EndWait( &cp_wk->talkwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃE�G�C�g�A�C�R����\������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_TALKWIN_StartTimeWait( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_TalkWin_StartTimeWait( &p_wk->talkwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ԃE�G�C�g�A�C�R��������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_TALKWIN_StopTimeWait( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_TalkWin_StopTimeWait( &p_wk->talkwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g�\�������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�^�C���E�G�C�g�\����
 *	@retval	FALSE	�^�C���E�G�C�g�o���Ă܂���
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_TALKWIN_CheckTimeWait( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_TalkWin_CheckTimeWait( &cp_wk->talkwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_TALKWIN_Off( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_TalkWin_Off( &p_wk->talkwin );
}




//----------------------------------------------------------------------------
/**
 *	@brief	�ŔE�B���h�E	���b�Z�[�W�\��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_BOARDWIN_PrintStr( WFLBY_ROOMWK* p_wk, const STRBUF* cp_str )
{
	WFLBY_ROOM_TalkWin_Board_Print( &p_wk->boardwin, cp_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ŔE�B���h�E	���b�Z�[�W��C�ɕ\��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_BOARDWIN_PrintStrAllPut( WFLBY_ROOMWK* p_wk, const STRBUF* cp_str )
{
	WFLBY_ROOM_TalkWin_Board_PrintAll( &p_wk->boardwin, cp_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ŔE�B���h�E	���b�Z�[�W�\�������҂�
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_BOARDWIN_WaitEnd( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_TalkWin_EndWait( &cp_wk->boardwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ŔE�B���h�E�̕\������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_BOARDWIN_Off( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_TalkWin_Board_Off( &p_wk->boardwin );
}



//----------------------------------------------------------------------------
/**
 *	@brief	���ڃ��X�g�쐬
 *
 *	@param	p_wk		���[�N
 *	@param	num			���X�g��
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_LISTWIN_CreateBmpList( WFLBY_ROOMWK* p_wk, u32 num )
{
	WFLBY_ROOM_ListWin_CreateBmpList( &p_wk->listwin, num, HEAPID_WFLBY_ROOMGRA );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڃ��X�g�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_LISTWIN_DeleteBmpList( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_ListWin_DeleteBmpList( &p_wk->listwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڂ̒ǉ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 *	@param	param		�p�����[�^
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_LISTWIN_SetBmpListStr( WFLBY_ROOMWK* p_wk, const STRBUF* cp_str, u32 param )
{
	WFLBY_ROOM_ListWin_SetBmpListStr( &p_wk->listwin, cp_str, param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڃ��X�g�|�C���^���擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�|�C���^
 */
//-----------------------------------------------------------------------------
const BMP_MENULIST_DATA* WFLBY_ROOM_LISTWIN_GetBmpList( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_ListWin_GetBmpList( &cp_wk->listwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v���X�g��param���o�^����Ă��邩�`�F�b�N����
 *
 *	@param	cp_wk		���[�N
 *	@param	param		�p�����[�^
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_LISTWIN_CheckBmpListParam( const WFLBY_ROOMWK* cp_wk, u32 param )
{
	return WFLBY_ROOM_ListWin_CheckBmpListParam( &cp_wk->listwin, param );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�E�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�\�����X�g�f�[�^
 *	@param	list_p		���X�g�����ʒu
 *	@param	cursor_p	���X�g�����J�[�\��
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_LISTWIN_Start( WFLBY_ROOMWK* p_wk, const BMPLIST_HEADER* cp_data, u16 list_p, u16 cursor_p )
{
	WFLBY_ROOM_ListWin_Start( &p_wk->listwin, cp_data, &p_wk->graphic, list_p, cursor_p, HEAPID_WFLBY_ROOM, WFLBY_LISTWIN_X, WFLBY_LISTWIN_Y, WFLBY_LISTWIN_SIZX );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�E�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�\�����X�g�f�[�^
 *	@param	list_p		���X�g�����ʒu
 *	@param	cursor_p	���X�g�����J�[�\��
 *	@param	cx			�r�b�g�}�b�v�L������
 *	@param	cy			�r�b�g�}�b�v�L������
 *	@param	szcx		�r�b�g�}�b�v�L�����T�C�Y��
 */
//-----------------------------------------------------------------------------
extern void WFLBY_ROOM_LISTWIN_Start_Ex( WFLBY_ROOMWK* p_wk, const BMPLIST_HEADER* cp_data, u16 list_p, u16 cursor_p, u8 cx, u8 cy, u8 szcx )
{
	WFLBY_ROOM_ListWin_Start( &p_wk->listwin, cp_data, &p_wk->graphic, list_p, cursor_p, HEAPID_WFLBY_ROOM, cx, cy, szcx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�E�B���h�E	���C��
 *
 *	@param	p_wk	���[�N
 *
 * @retval	"param = �I���p�����[�^"
 * @retval	"BMPLIST_NULL = �I��"
 * @retval	"BMPLIST_CANCEL	= �L�����Z��(�a�{�^��)"
 */
//-----------------------------------------------------------------------------
u32 WFLBY_ROOM_LISTWIN_Main( WFLBY_ROOMWK* p_wk )
{
	return WFLBY_ROOM_ListWin_Main( &p_wk->listwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�E�B���h�E	�I��
 *
 *	@param	p_wk		���[�N
 *	@param	p_list_p	���X�g�J�[�\���ʒu�擾
 *	@param	p_cursor_p	�J�[�\���ʒu�擾
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_LISTWIN_End( WFLBY_ROOMWK* p_wk, u16* p_list_p, u16* p_cursor_p )
{
	WFLBY_ROOM_ListWin_End( &p_wk->listwin, p_list_p, p_cursor_p );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�E�B���h�E	���@ONOFF
 *
 *	@param	p_wk		���[�N
 *	@param	flag		�\���t���O	TRUE�F�\��
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_LISTWIN_YAZIRUSHI_SetDraw( WFLBY_ROOMWK* p_wk, BOOL flag )
{
	WFLBY_ROOM_ListWin_SetClactDraw( &p_wk->listwin, flag );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E�\��
 *
 *	@param	p_wk		���[�N
 *	@param	cx			���L�����ʒu
 *	@param	cy			���L�����ʒu
 *	@param	szcx		���L�����T�C�Y
 *	@param	szcy		���L�����T�C�Y
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SUBWIN_Start( WFLBY_ROOMWK* p_wk, u8 cx, u8 cy, u8 szcx, u8 szcy )
{
	WFLBY_ROOM_SubWin_Start( &p_wk->subwin, &p_wk->graphic, HEAPID_WFLBY_ROOM, cx, cy, szcx, szcy );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E�\��OFF
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SUBWIN_End( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_SubWin_End( &p_wk->subwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E	���b�Z�[�W�\��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		���b�Z�[�W
 *	@param	x			���h�b�g���W
 *	@param	y			���h�b�g���W
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SUBWIN_Print( WFLBY_ROOMWK* p_wk, const STRBUF* cp_str, u8 x, u8 y )
{
	WFLBY_ROOM_SubWin_PrintAll( &p_wk->subwin, cp_str, x, y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E	�\�����N���A
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SUBWIN_Clear( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_SubWin_Clear( &p_wk->subwin );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��`�N���A����
 *
 *	@param	p_wk	���[�N
 *	@param	x		�����W
 *	@param	y		�����W
 *	@param	sx		���T�C�Y
 *	@param	sy		���T�C�Y
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_SUBWIN_ClearRect( WFLBY_ROOMWK* p_wk, u16 x, u16 y, u16 sx, u16 sy )
{
	WFLBY_ROOM_SubWin_ClearRect( &p_wk->subwin, x, y, sx, sy );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YESNO�E�B���h�E	�J�n
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_YESNOWIN_Start( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_ListWin_Start( &p_wk->listwin, &p_wk->yesnowin.bmplist, &p_wk->graphic, 
			0, 0, HEAPID_WFLBY_ROOM, 25, 13, 6 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YESNO�E�B���h�E	�J�n	����������J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_YESNOWIN_StartNo( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_ListWin_Start( &p_wk->listwin, &p_wk->yesnowin.bmplist, &p_wk->graphic, 
			0, WFLBY_ROOM_YESNO_NO, HEAPID_WFLBY_ROOM, 25, 13, 6 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YESNO�E�B���h�E�@���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	WFLBY_ROOM_YESNO_OK,	// �͂�
 *	@retval	WFLBY_ROOM_YESNO_NO,	// ������
 *	@retval	WFLBY_ROOM_YESNO_WAIT,	// �I�ђ�
 */
//-----------------------------------------------------------------------------
WFLBY_ROOM_YESNO_RET WFLBY_ROOM_YESNOWIN_Main( WFLBY_ROOMWK* p_wk )
{
	u32 result;

	result = WFLBY_ROOM_ListWin_Main( &p_wk->listwin );
	switch( result ){
	// ������
	case BMPLIST_CANCEL:
		Snd_SePlay( WFLBY_SND_CURSOR );
	case WFLBY_ROOM_YESNO_NO:
		return WFLBY_ROOM_YESNO_NO;

	//  �͂�
	case WFLBY_ROOM_YESNO_OK:
		return WFLBY_ROOM_YESNO_OK;

	// �I��
	default:
		break;
	}

	return WFLBY_ROOM_YESNO_WAIT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	YESNO�E�B���h�E�@�I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_YESNOWIN_End( WFLBY_ROOMWK* p_wk )
{
	// ���X�g����
	WFLBY_ROOM_ListWin_End( &p_wk->listwin, NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L���p���b�Z�[�W�f�[�^�̐ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		���b�Z�[�W�^�C�v
 *	@param	stridx		������IDX
 *
 *	@return	���b�Z�[�W�f�[�^
 */
//-----------------------------------------------------------------------------
STRBUF* WFLBY_ROOM_MSG_Get( WFLBY_ROOMWK* p_wk, WFLBY_DEFMSG_TYPE type, u32 stridx )
{
	return WFLBY_ROOM_Msg_Get( &p_wk->def_msg, type, stridx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	num			����
 *	@param	keta		��
 *	@param	bufid		�o�b�t�@�h�c
 *	@param	disptype	���߂Ƃ�
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetNumber( WFLBY_ROOMWK* p_wk, u32  num, u32 keta, u32 bufid, NUMBER_DISPTYPE disptype )
{
	WFLBY_ROOM_Msg_SetNumber( &p_wk->def_msg, num, keta, bufid, disptype );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀv���C���[����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_mystatus	MYSTATUS
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetPlayerName( WFLBY_ROOMWK* p_wk, const MYSTATUS* cp_mystatus, u32 bufid )
{
	WFLBY_ROOM_Msg_SetPlayerName( &p_wk->def_msg, cp_mystatus, bufid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀv���C���[����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetIdxPlayerName( WFLBY_ROOMWK* p_wk, u32 idx, u32 bufid )
{
	MYSTATUS* p_status;
	const WFLBY_USER_PROFILE* cp_profile;

	p_status	= MyStatus_AllocWork( HEAPID_WFLBY_ROOM );
	if( idx == WFLBY_SYSTEM_GetMyIdx( p_wk->p_system ) ){
		cp_profile	= WFLBY_SYSTEM_GetMyProfileLocal( p_wk->p_system );	
	}else{
		cp_profile	= WFLBY_SYSTEM_GetUserProfile( p_wk->p_system, idx );	
	}
	WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_status, HEAPID_WFLBY_ROOM );
	WFLBY_ROOM_Msg_SetPlayerName( &p_wk->def_msg, p_status, bufid );

	sys_FreeMemoryEz( p_status );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		�~�j�Q�[���^�C�v
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetMinigame( WFLBY_ROOMWK* p_wk, WFLBY_GAMETYPE type, u32 bufid )
{
	WFLBY_ROOM_Msg_SetMinigame( &p_wk->def_msg, type, bufid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���C�x���g����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		�^�C���C�x���gGMM�^�C�v
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetTimeEvent( WFLBY_ROOMWK* p_wk, WFLBY_EVENTGMM_TYPE type, u32 bufid )
{
	WFLBY_ROOM_Msg_SetTimeevent( &p_wk->def_msg, type, bufid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	item		�A�C�e��
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetItem( WFLBY_ROOMWK* p_wk, WFLBY_ITEMTYPE item, u32 bufid )
{
	WFLBY_ROOM_Msg_SetItem( &p_wk->def_msg, item, bufid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�̃N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_ClearWordSet( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_Msg_ClearWordSet( &p_wk->def_msg );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀ^�C���]�[���̃��b�Z�[�W��ǉ�����
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetAisatsuJapan( WFLBY_ROOMWK* p_wk, u32 bufID, u32 time_zone )
{
	WORDSET_RegisterWiFiLobbyAisatsuJapan( p_wk->def_msg.p_wordset, bufID, time_zone );
}
void WFLBY_ROOM_MSG_SetAisatsuEnglish( WFLBY_ROOMWK* p_wk, u32 bufID, u32 time_zone )
{
	WORDSET_RegisterWiFiLobbyAisatsuEnglish( p_wk->def_msg.p_wordset, bufID, time_zone );
}
void WFLBY_ROOM_MSG_SetAisatsuFrance( WFLBY_ROOMWK* p_wk, u32 bufID, u32 time_zone )
{
	WORDSET_RegisterWiFiLobbyAisatsuFrance( p_wk->def_msg.p_wordset, bufID, time_zone );
}
void WFLBY_ROOM_MSG_SetAisatsuItaly( WFLBY_ROOMWK* p_wk, u32 bufID, u32 time_zone )
{
	WORDSET_RegisterWiFiLobbyAisatsuItaly( p_wk->def_msg.p_wordset, bufID, time_zone );
}
void WFLBY_ROOM_MSG_SetAisatsuGerMany( WFLBY_ROOMWK* p_wk, u32 bufID, u32 time_zone )
{
	WORDSET_RegisterWiFiLobbyAisatsuGerMany( p_wk->def_msg.p_wordset, bufID, time_zone );
}
void WFLBY_ROOM_MSG_SetAisatsuSpain( WFLBY_ROOMWK* p_wk, u32 bufID, u32 time_zone )
{
	WORDSET_RegisterWiFiLobbyAisatsuSpain( p_wk->def_msg.p_wordset, bufID, time_zone );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�ɋZ�^�C�v����ݒ�
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_MSG_SetWazaType( WFLBY_ROOMWK* p_wk, u32 bufID, WFLBY_POKEWAZA_TYPE type )
{
	type = WFLBY_BattleWazaType_Get( type );
	WORDSET_RegisterPokeTypeName( p_wk->def_msg.p_wordset, bufID, type );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h�̕\��
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *	@param	aikotoba	�����t������ꍇ�\�����邩	TRUE:�\������
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_UNDERWIN_TrCardOn( WFLBY_ROOMWK* p_wk, u32 idx, BOOL aikotoba )
{
	// �\���ύX����K�v�����邩�`�F�b�N
	if( (WFLBY_ROOM_UNDERWIN_GetTrCardPlIdx( &p_wk->under_win ) == idx) &&
		(WFLBY_ROOM_UNDERWIN_CheckTrCardDraw( &p_wk->under_win ) == TRUE) &&
#if PL_T0862_080712_FIX
		(WFLBY_ROOM_UNDERWIN_CheckReqNone( &p_wk->under_win ) == FALSE) &&
#endif
		(WFLBY_ROOM_UNDERWIN_CheckTrCardAikotobaDraw( &p_wk->under_win ) == aikotoba) ){
		return;
	}
	WFLBY_ROOM_UNDERWIN_ReqTrCard( &p_wk->under_win, idx, aikotoba );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�K�W�F�b�g�ύX
 *
 *	@param	p_wk		���[�N
 *	@param	item		�ύX����A�C�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_UNDERWIN_TrCardGadGetChange( WFLBY_ROOMWK* p_wk, WFLBY_ITEMTYPE item )
{
	WFLBY_ROOM_UNDERWIN_ChangeItemTrCard( &p_wk->under_win, &p_wk->graphic, item,  HEAPID_WFLBY_ROOM );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h�@�Z�^�C�v�����ĕ`��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_UNDERWIN_TrCardWazaTypeWrite( WFLBY_ROOMWK* p_wk )
{
	const WFLBY_USER_PROFILE* cp_profile;
	u32 plidx;

	// �\���ύX����K�v�����邩�`�F�b�N
	if( (WFLBY_ROOM_UNDERWIN_CheckTrCardDraw( &p_wk->under_win ) == FALSE) ){
		return;
	}

	plidx		= WFLBY_ROOM_UNDERWIN_GetTrCardPlIdx( &p_wk->under_win );
	cp_profile	= WFLBY_SYSTEM_GetUserProfile( p_wk->p_system, plidx );

	WFLBY_ROOM_UNDERWIN_TrCard_WriteWazaType( &p_wk->under_win, &p_wk->under_win.tr_card, &p_wk->graphic, HEAPID_WFLBY_ROOM, cp_profile );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h�̔�\��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_UNDERWIN_TrCardOff( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_UNDERWIN_ReqBttn( &p_wk->under_win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�����̃v���C���[ID���擾����
 *
 *	@param	cp_wk		���[�N
 *
 *	@return �v���C���[�i���o�[
 */
//-----------------------------------------------------------------------------
u32 WFLBY_ROOM_UNDERWIN_TrCardGetPlIdx( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_UNDERWIN_GetTrCardPlIdx( &cp_wk->under_win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h��\�����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�g���[�i�J�[�h��\�����Ă���
 *	@retval	FALSE	�g���[�i�J�[�h��\�����Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_UNDERWIN_TrCardGetDraw( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_UNDERWIN_CheckTrCardDraw( &cp_wk->under_win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h��OFF�ɂ��邱�Ƃ����̂����H
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�\
 *	@retval	FALSE	�s�\
 *
 *	������Ƃ킩��ɂ����ł����A
 *	���N�G�X�g��NONE�Ńg���[�i�[�J�[�h�\�������H�Ƃ������Ƃł��B
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_UNDERWIN_TrCardCheckCanOff( const WFLBY_ROOMWK* cp_wk )
{
	return WFLBY_ROOM_UNDERWIN_CheckTrCardCanOff( &cp_wk->under_win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������ʂ��ʏ��Ԃ��`�F�b�N����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�ʏ���
 *	@retval	FALSE	���̑��̏��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_UNDERWIN_CheckSeqNormal( const WFLBY_ROOMWK* cp_wk )
{
	u32 seq;

	seq = WFLBY_ROOM_UNDERWIN_GetSeq( &cp_wk->under_win );
	if( seq == WFLBY_UNDERWIN_SEQ_NORMAL ){
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�{�^���\��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_UNDERWIN_FloatBttnOn( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_UNDERWIN_ReqBttnFloat( &p_wk->under_win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�{�^����\��
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_UNDERWIN_FloatBttnOff( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_UNDERWIN_ReqBttn( &p_wk->under_win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�A�j�������`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	flaotidx	�t���[�g�C���f�b�N�X
 *
 *	@retval	TRUE	�A�j����
 *	@retval	FALSE	�A�j�����ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_FLOAT_CheckShakeAnm( const WFLBY_ROOMWK* cp_wk, u32 floatidx )
{
	return WFLBY_TIMEEVENT_FLOAT_CheckAnm( cp_wk->p_timeevent, floatidx, WFLBY_FLOAT_ANM_SHAKE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g���擾������TRUE������
 *
 *	@param	p_wk		���[�N
 *	@param	get			�K�W�F�b�g
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_GadGet_Get( WFLBY_ROOMWK* p_wk )
{
	p_wk->under_win.bttn_start = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���̓����Ԃ�ݒ肷��	�{�^���������~�����邱�Ƃ��o���܂��B
 *
 *	@param	p_wk		���[�N
 *	@param	stop		��~������Ƃ�TRUE
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_GadGet_SetStop( WFLBY_ROOMWK* p_wk, BOOL stop )
{
	WFLBY_ROOM_UNDERWIN_SetBttnStop( &p_wk->under_win, stop );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g���b�N	�N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_GadGet_ClearLockWait( WFLBY_ROOMWK* p_wk )
{
	WFLBY_ROOM_UNDERWIN_Button_BttnLockClear( &p_wk->under_win.bttn );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������K�W�F�b�g���N�����邩�`�F�b�N
 *
 *	@param	p_wk		���[�N
 *
 *	@retval	TRUE	�N��
 *	@retval	FALSE	���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_GadGet_GetFlag( WFLBY_ROOMWK* p_wk )
{
	BOOL ret;
	
	ret = p_wk->gadget_flag;
	p_wk->gadget_flag = FALSE;
	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�𗬂�
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
void WFLBY_ROOM_GadGet_Play( WFLBY_ROOMWK* p_wk, u32 idx )
{
	const WFLBY_USER_PROFILE* cp_profile;
	u32 gadget;
	
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	cp_profile	= WFLBY_SYSTEM_GetUserProfile( p_wk->p_system, idx );
	gadget		= WFLBY_SYSTEM_GetProfileItem( cp_profile );
	WFLBY_GADGET_Start( p_wk->p_gadget, idx, gadget );

	// ������������݂�Ȃɑ���
	if( idx == WFLBY_SYSTEM_GetMyIdx( p_wk->p_system ) ){
		WFLBY_SYSTEM_SendGadGetData( p_wk->p_system, gadget );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g���I���̂��`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	TRUE		�I�����
 *	@retval	FALSE		�r��
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_ROOM_GadGet_PlayWait( const WFLBY_ROOMWK* cp_wk, u32 idx )
{
	GF_ASSERT( idx < WFLBY_PLAYER_MAX );

	return WFLBY_GADGET_EndWait( cp_wk->p_gadget, idx );
}







//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	VBLANK�֐�
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_VBlank( void* p_work )
{
	WFLBY_ROOMWK* p_wk = p_work;


	WFLBY_3DMAPOBJCONT_VBlank( p_wk->p_mapobjcont );
	WFLBY_3DOBJCONT_VBlank( p_wk->p_objcont );
	WFLBY_ROOM_GraphicVblank( &p_wk->graphic );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���V�X�e��������
 *
 *	@param	p_sys		�V�X�e��
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_GraphicInit( WFLBY_GRAPHICCONT* p_sys, SAVEDATA* p_save, u32 heapID )
{

	// ���������W�X�^OFF
	G2_BlendNone();
	G2S_BlendNone();

	// Vram�]���}�l�[�W���쐬
	initVramTransferManagerHeap( WFLBY_ROOM_VTRTSK_NUM, heapID );

	// �o���N�ݒ�
	GF_Disp_SetBank( &sc_WFLBY_ROOM_BANK );

	// �`��ʕύX
	// ���C���ƃT�u��؂�ւ���
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();

	// �o�b�N�O���E���h�����ɂ���
	{
		GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_M, 0 );
	}

	// BGL
	{
		int i;

		GF_BGL_InitBG(&sc_BGINIT);

		p_sys->p_bgl = GF_BGL_BglIniAlloc( heapID );

		for( i=0; i<WFLBY_ROOM_BGCNT_NUM; i++ ){
			GF_BGL_BGControlSet( p_sys->p_bgl, 
					sc_WFLBY_ROOM_BGCNT_FRM[i], &sc_WFLBY_ROOM_BGCNT_DATA[i],
					GF_BGL_MODE_TEXT );
			GF_BGL_ClearCharSet( sc_WFLBY_ROOM_BGCNT_FRM[i], 32, 0, heapID);
			GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[i] );
		}
	}


	// �E�B���h�E�ݒ�
	{
		CONFIG* p_config;
		u8 winnum;

		p_config = SaveData_GetConfig( p_save );
		winnum = CONFIG_GetWindowType( p_config );

		// �����p���b�g
		SystemFontPaletteLoad( PALTYPE_MAIN_BG, WFLBY_ROOM_BGPL_SYSFONT_CL*32, heapID );
		TalkFontPaletteLoad( PALTYPE_MAIN_BG, WFLBY_ROOM_BGPL_TALKFONT_CL*32, heapID );

		// �E�B���h�E�O���t�B�b�N
		MenuWinGraphicSet(
				p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN], WFLBY_SYSWINGRA_CGX,
				WFLBY_ROOM_BGPL_SYSWIN, 0, heapID );
		TalkWinGraphicSet(
				p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN], WFLBY_TALKWINGRA_CGX, 
				WFLBY_ROOM_BGPL_TALKWIN, winnum, heapID );
		BoardWinGraphicSet(
			p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN],
			WFLBY_BOARDWINGRA_CGX, WFLBY_ROOM_BGPL_BOARDWIN, 
			BOARD_TYPE_INFO, 0, heapID );

		// �p���b�g�����A���r�[�p�p���b�g���g�p����
		ArcUtil_PalSet( ARC_WIFILOBBY_OTHER_GRA, NARC_wifi_lobby_other_lobby_board_NCLR, 
				PALTYPE_MAIN_BG, WFLBY_ROOM_BGPL_BOARDWIN*32, 32, heapID );
	}


	// OAM
	{
		int i;

		// OAM�}�l�[�W���[�̏�����
		NNS_G2dInitOamManagerModule();

		// ���LOAM�}�l�[�W���쐬
		// �����_���pOAM�}�l�[�W���쐬
		// �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
		REND_OAMInit(
			0, 126,     // ���C�����OAM�Ǘ��̈�
			0, 31,      // ���C����ʃA�t�B���Ǘ��̈�
			0, 126,     // �T�u���OAM�Ǘ��̈�
			0, 31,      // �T�u��ʃA�t�B���Ǘ��̈�
			heapID);


		// �L�����N�^�}�l�[�W���[������
		InitCharManagerReg(&sc_WFLBY_ROOM_CHARMAN_INIT, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_64K ); //���X�g�̃J�[�\�����t�B�[���h�Ƌ��ʂŎg�p����̂�32K�ɂ���
	
		// �p���b�g�}�l�[�W���[������
		InitPlttManager(WFLBY_ROOM_OAM_CONTNUM, heapID);

		// �ǂݍ��݊J�n�ʒu��������
		CharLoadStartAll();
		PlttLoadStartAll();

		//�ʐM�A�C�R���p�ɃL�������p���b�g����
		CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);	//���X�g�̃J�[�\�����t�B�[���h�Ƌ��ʂŎg�p����̂�32K�ɂ���
		CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);


		// ����ʂɒʐM�A�C�R�����o��
		WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��


		// �Z���A�N�^�[�쐬
		p_sys->p_clactset = CLACT_U_SetEasyInit( WFLBY_ROOM_CLACT_NUM, &p_sys->renddata, heapID );
		CLACT_U_SetSubSurfaceMatrix( &p_sys->renddata, 0, WFLBY_ROOM_CLACT_SUBSURFACE_Y );

		// ���\�[�X�}�l�[�W���쐬
		for( i=0; i<WFLBY_ROOM_OAM_RESNUM; i++ ){
			p_sys->p_resman[i] = CLACT_U_ResManagerInit(WFLBY_ROOM_OAM_CONTNUM, i, heapID);
		}

		// �Z��Vram�]���}�l�[�W���쐬
		p_sys->p_celltransarray = InitCellTransfer(WFLBY_ROOM_OAM_CONTNUM, heapID );
		

		// �\���J�n
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
	}

	// 3D�ݒ�
	{
		p_sys->p_3dman = GF_G3DMAN_Init( heapID, GF_G3DMAN_LNK, GF_G3DTEX_256K, 
				GF_G3DMAN_LNK, GF_G3DPLT_64K, WFLBY_ROOM_DrawSys3DSetUp );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��V�X�e��	���C������
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_GraphicMain( WFLBY_GRAPHICCONT* p_sys )
{
	CLACT_Draw( p_sys->p_clactset );

	UpdateCellTransfer();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�V�X�e���j��
 *	
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_GraphicExit( WFLBY_GRAPHICCONT* p_sys )
{
	// VRAM�]���}�l�[�W���j��
	DellVramTransferManager();

	// BG�̔j��
	{
		int i;

		for( i=0; i<WFLBY_ROOM_BGCNT_NUM; i++ ){
			GF_BGL_BGControlExit( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[i] );
		}

		// BGL�j��
		sys_FreeMemoryEz( p_sys->p_bgl );
	}

	// OAM�̔j��
	{
		int i;

		DeleteCellTransfer( p_sys->p_celltransarray );
		p_sys->p_celltransarray = NULL;

		// ���\�[�X�}�l�[�W���쐬
		for( i=0; i<WFLBY_ROOM_OAM_RESNUM; i++ ){
			CLACT_U_ResManagerDelete( p_sys->p_resman[i] );
		}

		// �Z���A�N�^�[�j��
		CLACT_DestSet( p_sys->p_clactset );


		// ���\�[�X���
		DeleteCharManager();
		DeletePlttManager();

		//OAM�����_���[�j��
		REND_OAM_Delete();
	}

	// BG�̔j��
	{
		GF_G3D_Exit( p_sys->p_3dman );
	}
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�NVBlank����
 *
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_GraphicVblank( WFLBY_GRAPHICCONT* p_sys )
{
    // BG��������
    GF_BGL_VBlankFunc( p_sys->p_bgl );

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();

	// Vram�]��
	DoVramTransferManager();
}

//----------------------------------------------------------------------------
/**
 *	@brief	3D�Z�b�g�A�b�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_DrawSys3DSetUp( void )
{
	// �R�c�g�p�ʂ̐ݒ�(�\�����v���C�I���e�B�[)
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
    G2_SetBG0Priority(1);

	// �e��`�惂�[�h�̐ݒ�(�V�F�[�h���A���`�G�C���A�X��������)
    G3X_SetShading( GX_SHADING_TOON );
    G3X_AntiAlias( TRUE );
	G3X_AlphaTest( FALSE, 0 );	// �A���t�@�e�X�g�@�@�I�t
	G3X_AlphaBlend( TRUE );		// �A���t�@�u�����h�@�I��

	G3X_EdgeMarking( TRUE );
	G3X_SetEdgeColorTable(sc_WFLBY_ROOM_EDGECOLOR);
	G3X_SetFog( FALSE, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0 );

	// �N���A�J���[�̐ݒ�
    G3X_SetClearColor(GX_RGB(26,26,26),0,0x7fff,63,FALSE);	//color,alpha,depth,polygonID,fog
	// �r���[�|�[�g�̐ݒ�
    G3_ViewPort(0, 0, 255, 191);

	{
		VecFx16 vec;
		// ���C�g0
		{
			NNS_G3dGlbLightVector( 0, 0xfffff805, 0xfffff224, 0x6e );
		}
		// ���C�g1
		{
			VEC_Fx16Set( &vec, 0, 0, FX16_ONE );
			VEC_Fx16Normalize( &vec, &vec );
			NNS_G3dGlbLightVector( 1, vec.x, vec.y, vec.z );
		}
		// ���C�g2
		// ���E���v��j���[�X�ȂǃN���[�Y�㖾������������̗p
		{
			VEC_Fx16Set( &vec, 0, 0, FX16_ONE );
			VEC_Fx16Normalize( &vec, &vec );
			NNS_G3dGlbLightVector( 2, vec.x, vec.y, vec.z );
			NNS_G3dGlbLightColor( 2, GX_RGB( 31,31,31 ) );
		}
		// ���C�g3
		{
			VEC_Fx16Set( &vec, 0, 0, FX16_ONE );
			VEC_Fx16Normalize( &vec, &vec );
			NNS_G3dGlbLightVector( 3, vec.x, vec.y, vec.z );
			NNS_G3dGlbLightColor( 3, GX_RGB( 31,31,31 ) );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_RoomMain( WFLBY_ROOMWK* p_wk )
{
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_INIT;

	// �l������
	WFLBY_3DOBJCONT_Move( p_wk->p_objcont );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );


	// �G���[�ƃ^�C���A�E�g�œ��삵�Ȃ�
	if( (p_wk->err_flag == FALSE) && (p_wk->timeout == FALSE) ){	

		// �C�x���g���s	BEFORE
		{
			WFLBY_EVENT_MainBefore( p_wk->p_event );
		}
		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );

	}


	// �z�u�I�u�W�F����
	WFLBY_3DMAPOBJCONT_Main( p_wk->p_mapobjcont );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );


	// �G���[�ƃ^�C���A�E�g�œ��삵�Ȃ�
	if( (p_wk->err_flag == FALSE) && (p_wk->timeout == FALSE) ){	
		// �l�����샊�N�G�X�g���f
		WFLBY_3DOBJCONT_ReqMove( p_wk->p_objcont );
		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );

		// �C�x���g���s	AFTER
		{
			WFLBY_EVENT_MainAfter( p_wk->p_event );
		}
		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );

		// �^�C���C�x���g���C��
		WFLBY_TIMEEVENT_Main( p_wk->p_timeevent );
		WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );
	}


	// �z�u�I�u�W�F�A�j���Ǘ�
	WFLBY_ROOM_MapAnmCont( p_wk );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );

	
	// ������ʃ��C������
	if( p_wk->fade_flag == FALSE ){
		BOOL no_input;
		// �I�������ɂ����Ă�����A�^�b�`�Ȃǎ󂯕t���Ȃ�
		if( (p_wk->err_flag == TRUE) || (p_wk->timeout == TRUE) || (p_wk->end_flag == TRUE) ){	
			no_input = TRUE;
		}else{
			no_input = FALSE;
		}
		WFLBY_ROOM_UNDERWIN_Main( &p_wk->under_win, p_wk, no_input, HEAPID_WFLBY_ROOM );
	}
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );

	// �K�W�F�b�g�A�j��
	WFLBY_GADGET_Main( p_wk->p_gadget );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT( __LINE__ );

	// �~�j�Q�[���Q�����b�N�J�E���^


#ifdef WFLBY_DEBUG_ROOM_CAMERA

	// �J������ς���
	if( sys.trg & PAD_BUTTON_SELECT ){
		WFLBY_DEBUG_ROOM_CAMERA_FLAG ^= 1;
		WFLBY_CAMERA_Exit( p_wk->p_camera );
		if( WFLBY_DEBUG_ROOM_CAMERA_FLAG ){
			p_wk->p_camera = WFLBY_CAMERA_DEBUG_CameraInit( HEAPID_WFLBY_ROOM );
		}else{
			p_wk->p_camera = WFLBY_CAMERA_Init( HEAPID_WFLBY_ROOM );
		}
		// �^�[�Q�b�g��ݒ�
		WFLBY_CAMERA_SetTargetPerson( p_wk->p_camera,
				WFLBY_3DOBJCONT_GetPlayer( p_wk->p_objcont ) );
	}
#endif

#ifdef WFLBY_DEBUG_ROOM_ITEMCHG
	// �^�b�`�g�C��ς���
	if( sys.trg & PAD_BUTTON_L ){
		WFLBY_USER_PROFILE* p_profile;
		u32 item;

		p_profile	= WFLBY_SYSTEM_GetMyProfileLocal( p_wk->p_system );
		item		= WFLBY_SYSTEM_GetProfileItem( p_profile );
		item		= (item + 1) % WFLBY_ITEM_NUM;
		WFLBY_SYSTEM_SetMyItem( p_wk->p_system, item );

		OS_TPrintf( "=====  DEBUG Item Change %d  =======\n", item );
	}
#endif

}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�揈��
 *
 *	@param	p_wk	���[�N
 */	
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_RoomDraw( WFLBY_ROOMWK* p_wk )
{
	MtxFx44 org_pm;
		
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_INIT;
	
	GF_G3X_Reset();


	// �J�����ݒ�
	WFLBY_CAMERA_Draw( p_wk->p_camera );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT_DRAW( __LINE__ );

	// �}�b�v�̕\��
	WFLBY_3DMAPOBJCONT_Draw( p_wk->p_mapobjcont );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT_DRAW( __LINE__ );

	//�v���W�F�N�V�����}�g���N�X�̕ϊ�
#if 0
	{
		const MtxFx44 *m;
		MtxFx44 pm;

		m = NNS_G3dGlbGetProjectionMtx();

		org_pm = *m;
		pm = org_pm;
		pm._32 += FX_Mul(pm._22,WFLBY_ROOM_PRO_MAT_Z_OFS*FX32_ONE);
		NNS_G3dGlbSetProjectionMtx(&pm);
//		NNS_G3dGlbFlush();		//�@�W�I���g���R�}���h��]��
	}
#endif
	
	// �l���̕\��
	WFLBY_3DOBJCONT_Draw( p_wk->p_objcont );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT_DRAW( __LINE__ );

	//�v���W�F�N�V�����}�g���N�X�����ɖ߂�
#if 0
	{
		NNS_G3dGlbSetProjectionMtx(&org_pm);
		NNS_G3dGlbFlush();		//�@�W�I���g���R�}���h��]��
	}
#endif

	// �K�W�F�b�g
	WFLBY_GADGET_Draw( p_wk->p_gadget );
	WFLBY_DEBUG_ROOM_PRINT_TIME_TICK_PRINT_DRAW( __LINE__ );
	
	GF_G3_RequestSwapBuffers(GX_SORTMODE_AUTO,GX_BUFFERMODE_Z);

	// �`��V�X�e�����C��
	WFLBY_ROOM_GraphicMain( &p_wk->graphic );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�A�j���R���g���[��
 *
 *	@param	p_wk	���[�N
 *
 *	�N���A�j�����Ǘ����邩�킩�����̂͂����ŊǗ�����
 */
//-----------------------------------------------------------------------------
static  void WFLBY_ROOM_MapAnmCont( WFLBY_ROOMWK* p_wk )
{
	int i;
	BOOL play;
	BOOL recruit;
	u32 num;
	u32 gametype;
	BOOL minigame_end;

	// �~�j�Q�[���I�����Ă邩�`�F�b�N
	minigame_end = WFLBY_SYSTEM_Event_GetMiniGameStop( p_wk->p_system );
	
	// �~�j�Q�[��
	for( i=0; i<DWC_LOBBY_MG_NUM; i++ ){
		recruit = DWC_LOBBY_MG_CheckRecruit( i );				// ��W����
		if( recruit == TRUE ){
			if( DWC_LOBBY_MG_CheckEntryOk( i ) == FALSE ){		// ��W���ł܂�������łȂ���
				play = TRUE;
			}else{
				play = FALSE;
			}
		}else{
			play = FALSE;	// ��W���ĂȂ��̂ł�����ł�킯�Ȃ�
		}
		num = DWC_LOBBY_MG_GetEntryNum( i );
		WFLBY_3DMAPOBJCONT_MAP_SetMGAnm(  p_wk->p_mapobjcont, WFLBY_GAME_BALLSLOW+i, num, 
				recruit, play, minigame_end );
	}
}





//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�O���t�B�b�N���[�N
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Init( WFLBY_ROOM_TALKMSG* p_wk, WFLBY_GRAPHICCONT* p_sys, SAVEDATA* p_save, u32 heapID )
{

	//  �r�b�g�}�b�v�m��
	GF_BGL_BmpWinAdd(
				p_sys->p_bgl, &p_wk->win, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN],
				WFLBY_TALKWIN_X, WFLBY_TALKWIN_Y,
				WFLBY_TALKWIN_SIZX, WFLBY_TALKWIN_SIZY, WFLBY_ROOM_BGPL_TALKFONT_CL,
				WFLBY_TALKWIN_CGX );

	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// ������o�b�t�@�쐬
	p_wk->p_str = STRBUF_Create( WFLBY_TALKWIN_STRBUFNUM, heapID );

	// ���b�Z�[�W�\���E�G�C�g��ݒ�
	{
		CONFIG* p_config;
		p_config = SaveData_GetConfig( p_save );
		p_wk->msgwait = CONFIG_GetMsgPrintSpeed( p_config );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Exit( WFLBY_ROOM_TALKMSG* p_wk )
{
	//  ���ׂĒ�~
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}
	WFLBY_ROOM_TalkWin_StopTimeWait_NoTrans( p_wk );
	
	// ������o�b�t�@�j��
	STRBUF_Delete( p_wk->p_str );

	// �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�̕\������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Print( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	p_wk->msgno = GF_STR_PrintColor( &p_wk->win, FONT_TALK, p_wk->p_str, 0, 0,
			p_wk->msgwait, WFLBY_TALKWIN_MSGCOL, NULL );

	// �E�B���h�E����������
	BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_TALKWINGRA_CGX, WFLBY_ROOM_BGPL_TALKWIN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\��	��x�ɑS���\������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_PrintAll( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	GF_STR_PrintColor( &p_wk->win, FONT_TALK, p_wk->p_str, 0, 0,
			MSG_NO_PUT, WFLBY_TALKWIN_MSGCOL, NULL );

	// �E�B���h�E����������
	BmpTalkWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_TALKWINGRA_CGX, WFLBY_ROOM_BGPL_TALKWIN );
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g�A�C�R���J�n
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_StartTimeWait( WFLBY_ROOM_TALKMSG* p_wk )
{
	GF_ASSERT( p_wk->p_timewait == NULL );
	
	p_wk->p_timewait = TimeWaitIconAdd( &p_wk->win, WFLBY_TALKWINGRA_CGX );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g�A�C�R���I��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_StopTimeWait( WFLBY_ROOM_TALKMSG* p_wk )
{
	if( p_wk->p_timewait != NULL ){
		TimeWaitIconDel( p_wk->p_timewait );
		p_wk->p_timewait = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g�A�C�R���I��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_StopTimeWait_NoTrans( WFLBY_ROOM_TALKMSG* p_wk )
{
	if( p_wk->p_timewait != NULL ){
		TimeWaitIconTaskDel( p_wk->p_timewait );
		p_wk->p_timewait = NULL;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C���E�G�C�g��\�����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�^�C���E�G�C�g��
 *	@retval	FALSE	�^�C���E�G�C�g�\�����ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_TalkWin_CheckTimeWait( const WFLBY_ROOM_TALKMSG* cp_wk )
{
	if( cp_wk->p_timewait != NULL ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\���I��
 *
 *	@param	cp_wk		���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_TalkWin_EndWait( const WFLBY_ROOM_TALKMSG* cp_wk )
{
	if( GF_MSG_PrintEndCheck( cp_wk->msgno ) == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E��\��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Off( WFLBY_ROOM_TALKMSG* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �^�C���E�G�C�g������
	WFLBY_ROOM_TalkWin_StopTimeWait_NoTrans( p_wk );

	// �S�̂�����
	BmpTalkWinClear( &p_wk->win, WINDOW_TRANS_OFF );

	// V���N�G�X�g
	GF_BGL_BmpWinOffVReq( &p_wk->win );
}



//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E�@�h��	�ŔE�B���h�E	������
 *
 *	@param	p_wk			���[�N
 *	@param	p_sys			�V�X�e��
 *	@param	p_save			�Z�[�u�f�[�^
 *	@param	heapID			�q�[�v�h�c
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_Init( WFLBY_ROOM_TALKMSG* p_wk, WFLBY_GRAPHICCONT* p_sys, SAVEDATA* p_save, u32 heapID )
{
	WFLBY_ROOM_TalkWin_Init( p_wk, p_sys, p_save, heapID );
	// �p���b�g�i���o�[�����ύX
	GF_BGL_BmpWinSet_Pal( &p_wk->win, WFLBY_ROOM_BGPL_BOARDWIN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E�@�h��	�ŔE�B���h�E	�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_Exit( WFLBY_ROOM_TALKMSG* p_wk )
{
	WFLBY_ROOM_TalkWin_Exit( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E�@�h��	�ŔE�B���h�E�@���b�Z�[�W��������
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_Print( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	p_wk->msgno = GF_STR_PrintSimple( &p_wk->win, FONT_TALK, p_wk->p_str, 0, 0,
			p_wk->msgwait, NULL );

	// �E�B���h�E����������
	BmpBoardWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_BOARDWINGRA_CGX, WFLBY_ROOM_BGPL_BOARDWIN, BOARD_TYPE_INFO );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�B���h�E	�h��	�ŔE�B���h�E�@���b�Z�[�W�S���`��
 *
 *	@param	p_wk
 *	@param	cp_str 
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_PrintAll( WFLBY_ROOM_TALKMSG* p_wk, const STRBUF* cp_str )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	GF_STR_PrintSimple( &p_wk->win, FONT_TALK, p_wk->p_str, 0, 0,
			MSG_NO_PUT, NULL );

	// �E�B���h�E����������
	BmpBoardWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_BOARDWINGRA_CGX, WFLBY_ROOM_BGPL_BOARDWIN, BOARD_TYPE_INFO );
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ŔE�B���h�E��OFF������
 *
 *	@param	p_wk ���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_Off( WFLBY_ROOM_TALKMSG* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �^�C���E�G�C�g������
	WFLBY_ROOM_TalkWin_StopTimeWait_NoTrans( p_wk );

	// �S�̂�����
	BmpBoardWinClear( &p_wk->win, BOARD_TYPE_INFO, WINDOW_TRANS_OFF );

	// V���N�G�X�g
	GF_BGL_BmpWinOffVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ŕ{�[�h�@�N���A�E�B���h�E
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_ClearWin( WFLBY_ROOM_TALKMSG* p_wk )
{
	// ���b�Z�[�W�\�����Ȃ����
	if( GF_MSG_PrintEndCheck( p_wk->msgno ) ){
		GF_STR_PrintForceStop( p_wk->msgno );
	}

	// �r�b�g�}�b�v�̃N���A
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V���v���Ƀ��b�Z�[�W����������
 *
 *	@param	p_wk	���[�N
 *	@param	x		�����W
 *	@param	y		�����W
 *	@param	cp_str	������
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_PrintSimple( WFLBY_ROOM_TALKMSG* p_wk, u8 x, u8 y, const STRBUF* cp_str )
{
	// ������R�s�[
	STRBUF_Copy( p_wk->p_str, cp_str );
	GF_STR_PrintSimple( &p_wk->win, FONT_TALK, p_wk->p_str, x, y,
			MSG_NO_PUT, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�̕\��	VBLANK����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_OnVReq( WFLBY_ROOM_TALKMSG* p_wk )
{
	// �E�B���h�E����������
	BmpBoardWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_BOARDWINGRA_CGX, WFLBY_ROOM_BGPL_BOARDWIN, BOARD_TYPE_INFO );
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�E�B���h�E�̃p���b�g�ԍ���ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	pal		�p���b�g�ԍ�
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_TalkWin_Board_SetPalNo( WFLBY_ROOM_TALKMSG* p_wk, u16 pal )
{
	// �p���b�g�i���o�[�����ύX
	GF_BGL_BmpWinSet_Pal( &p_wk->win, pal );
}




//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�V�X�e��		������
 *
 *	@param	p_wk	���[�N
 *	@param	p_sys	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_Init( WFLBY_ROOM_LISTWIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID )
{
	CLACT_HEADER head;
	CLACT_ADD_SIMPLE add;
	int i;

	// �A�N�^�[���\�[�X�ǂݍ���
	ARCHANDLE* p_handle = ArchiveDataHandleOpen( ARC_SHOP_GRA, heapID );
	// ���\�[�X�ǂݍ���
	p_wk->resObj[ CLACT_U_CHAR_RES ] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
			p_sys->p_resman[ CLACT_U_CHAR_RES ],
			p_handle, NARC_shop_gra_shop_arw_NCGR, 
			FALSE, WFLBY_LISTWIN_CLACT_RESCONT_ID, 
			NNS_G2D_VRAM_TYPE_2DMAIN, heapID );	

	p_wk->resObj[ CLACT_U_PLTT_RES ] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
			p_sys->p_resman[ CLACT_U_PLTT_RES ],
			p_handle, NARC_shop_gra_shop_obj_NCLR, 
			FALSE, WFLBY_LISTWIN_CLACT_RESCONT_ID, 
			NNS_G2D_VRAM_TYPE_2DMAIN, 1,  heapID );	

	p_wk->resObj[ CLACT_U_CELL_RES ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
			p_sys->p_resman[ CLACT_U_CELL_RES ],
			p_handle, NARC_shop_gra_shop_arw_NCER, 
			FALSE, WFLBY_LISTWIN_CLACT_RESCONT_ID, 
			CLACT_U_CELL_RES, heapID );	

	p_wk->resObj[ CLACT_U_CELLANM_RES ] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
			p_sys->p_resman[ CLACT_U_CELLANM_RES ],
			p_handle, NARC_shop_gra_shop_arw_NANR, 
			FALSE, WFLBY_LISTWIN_CLACT_RESCONT_ID, 
			CLACT_U_CELLANM_RES, heapID );

	ArchiveDataHandleClose( p_handle );


	CLACT_U_CharManagerSetAreaCont( p_wk->resObj[ CLACT_U_CHAR_RES ] );
	CLACT_U_PlttManagerSetCleanArea( p_wk->resObj[ CLACT_U_PLTT_RES ] );

	// �w�b�_�[�쐬
	CLACT_U_MakeHeader(
			&head,
			WFLBY_LISTWIN_CLACT_RESCONT_ID,
			WFLBY_LISTWIN_CLACT_RESCONT_ID,
			WFLBY_LISTWIN_CLACT_RESCONT_ID,
			WFLBY_LISTWIN_CLACT_RESCONT_ID,
			CLACT_U_HEADER_DATA_NONE,
			CLACT_U_HEADER_DATA_NONE,
			0, 0,
			p_sys->p_resman[ 0 ],
			p_sys->p_resman[ 1 ],
			p_sys->p_resman[ 2 ],
			p_sys->p_resman[ 3 ],
			NULL, NULL );
	
	// �A�N�^�[�o�^
	add.ClActSet = p_sys->p_clactset;
	add.ClActHeader = &head;
	add.pri		= 0;
	add.DrawArea = NNS_G2D_VRAM_TYPE_2DMAIN;
	add.heap	= heapID;
	add.mat.x	= WFLBY_LISTWIN_CLACT_X * FX32_ONE;
	for( i=0; i<WFLBY_LISTWIN_CLACT_OBJNUM; i++ ){
		add.mat.y	= sc_WFLBY_LISTWIN_CLACT_Y[ i ] * FX32_ONE;
		p_wk->clact[i] = CLACT_AddSimple( &add );
		CLACT_AnmChg( p_wk->clact[ i ], i );
		CLACT_SetAnmFlag( p_wk->clact[ i ], TRUE );
		CLACT_SetDrawFlag( p_wk->clact[ i ], FALSE );
	}

	p_wk->clact_draw = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�V�X�e��		�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_Exit( WFLBY_ROOM_LISTWIN* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
	int i;
	
	// ���X�g�f�[�^���c���Ă�����͂�
	if( p_wk->p_bmplist != NULL ){
		WFLBY_ROOM_ListWin_DeleteBmpList( p_wk );
	}
	
	// ���삵�Ă����炷�ׂĂ͂�
	if( p_wk->p_listwk != NULL ){
		WFLBY_ROOM_ListWin_End( p_wk, NULL, NULL );
	}

	// �A�N�^�[�j��
	for( i=0; i<WFLBY_LISTWIN_CLACT_OBJNUM; i++ ){
		CLACT_Delete( p_wk->clact[i] );
		p_wk->clact[i] = NULL;
	}

	CLACT_U_CharManagerDelete( p_wk->resObj[ CLACT_U_CHAR_RES ] );
	CLACT_U_PlttManagerDelete( p_wk->resObj[ CLACT_U_PLTT_RES ] );

	for( i=0; i<WFLBY_LISTWIN_CLACT_RESNUM; i++ ){
		CLACT_U_ResManagerResDelete( p_sys->p_resman[i], p_wk->resObj[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڃ��X�g���쐬����
 *	
 *	@param	p_wk		���[�N
 *	@param	num			���X�g��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_CreateBmpList( WFLBY_ROOM_LISTWIN* p_wk, u32 num, u32 heapID )
{
	GF_ASSERT( p_wk->p_bmplist == NULL );
	p_wk->p_bmplist		= BMP_MENULIST_Create( num, heapID );
	p_wk->bmplistnum	= num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڃ��X�g��j������
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_DeleteBmpList( WFLBY_ROOM_LISTWIN* p_wk )
{
	if( p_wk->p_bmplist != NULL ){
		BMP_MENULIST_Delete( p_wk->p_bmplist );
		p_wk->p_bmplist		= NULL;
		p_wk->bmplistnum	= 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڂ̒ǉ�
 *	
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 *	@param	param		�p�����[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_SetBmpListStr( WFLBY_ROOM_LISTWIN* p_wk, const STRBUF* cp_str, u32 param )
{
	BMP_MENULIST_AddString( p_wk->p_bmplist, cp_str, param );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ڃ��X�g���擾����
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	���ڃ��X�g
 */	
//-----------------------------------------------------------------------------
static const BMP_MENULIST_DATA* WFLBY_ROOM_ListWin_GetBmpList( const WFLBY_ROOM_LISTWIN* cp_wk )
{
	return cp_wk->p_bmplist;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v���X�g	��param�����������Ă��邩�`�F�b�N����
 *
 *	@param	cp_wk		���[�N
 *	@param	param		�p�����[�^
 *
 *	@retval	TRUE	����
 *	@retval	FALSE	�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_ListWin_CheckBmpListParam( const WFLBY_ROOM_LISTWIN* cp_wk, u32 param )
{
	int i;

	for( i=0; i<cp_wk->bmplistnum; i++ ){
		if( cp_wk->p_bmplist[i].param == param ){
			return  TRUE;
		}
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�V�X�e��	�\���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	p_sys		�\���V�X�e��
 *	@param	list_p		���������X�g�ʒu
 *	@param	cursor_p	�J�[�\���ʒu
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_Start( WFLBY_ROOM_LISTWIN* p_wk, const BMPLIST_HEADER* cp_data, WFLBY_GRAPHICCONT* p_sys, u16 list_p, u16 cursor_p, u32 heapID, u8 cx, u8 cy, u8 szcx )
{
	GF_ASSERT( p_wk->p_listwk == NULL );
	// �T�C�Y���Ԃɍ����Ă��邩�`�F�b�N
	GF_ASSERT( (cp_data->line*2) < WFLBY_LISTWIN_SIZY );

	// �w�b�_�[�R�s�[
	p_wk->data		= *cp_data;
	p_wk->data.win	= &p_wk->win;

	// ���X�g����
	p_wk->list_num	= cp_data->count;

	// ���X�g�R�[���o�b�N�ݒ�
	p_wk->data.call_back = WFLBY_ROOM_ListWin_CurCallBack;

	// �r�b�g�}�b�v�����쐬
	GF_BGL_BmpWinAdd(
				p_sys->p_bgl, &p_wk->win, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN],
				cx, cy,
				szcx, cp_data->line*2, WFLBY_ROOM_BGPL_SYSFONT_CL,
				WFLBY_LISTWIN_CGX );

	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
	
	// ���X�g�쐬
    p_wk->p_listwk = BmpListSet(&p_wk->data, list_p, cursor_p, heapID );
    BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_SYSWINGRA_CGX, WFLBY_ROOM_BGPL_SYSWIN );

	// �\��
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�V�X�e��	���C��
 *
 *	@param	p_wk	���[�N
 *
 * @retval	"param = �I���p�����[�^"
 * @retval	"BMPLIST_NULL = �I��"
 * @retval	"BMPLIST_CANCEL	= �L�����Z��(�a�{�^��)"
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_ROOM_ListWin_Main( WFLBY_ROOM_LISTWIN* p_wk )
{
	u32 result;
	
	if( p_wk->p_listwk == NULL ){
		return BMPLIST_CANCEL;
	}
	result = BmpListMain( p_wk->p_listwk );


	switch( result ){
	case BMPLIST_NULL:
	case BMPLIST_CANCEL:
		//  �A�N�^�[����
		WFLBY_ROOM_ListWin_ContClactDraw( p_wk );
		break;
		
	default:
		Snd_SePlay(WFLBY_SND_CURSOR);
		break;
	}

	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�V�X�e��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_list_p	���X�g�ʒu�i�[��
 *	@param	p_cursor_p	�J�[�\���ʒu�i�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_End( WFLBY_ROOM_LISTWIN* p_wk, u16* p_list_p, u16* p_cursor_p )
{
	int i;
	
	if( p_wk->p_listwk == NULL ){
		return ;
	}
	BmpListExit( p_wk->p_listwk, p_list_p, p_cursor_p );
	p_wk->p_listwk = NULL;

	// �E�B���h�E��j��
    BmpMenuWinClear( &p_wk->win, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_wk->win );
	GF_BGL_BmpWinDel( &p_wk->win );

	// OAM��\��
	p_wk->clact_draw = FALSE;
	for( i=0; i<WFLBY_LISTWIN_CLACT_OBJNUM; i++ ){
		CLACT_SetDrawFlag( p_wk->clact[i], FALSE );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v���X�g�J�[�\���R�[���o�b�N
 *
 *	@param	p_wk		���X�g���[�N
 *	@param	param		�p�����[�^
 *	@param	mode		���[�h 
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_CurCallBack( BMPLIST_WORK* p_wk,u32 param,u8 mode )
{
    if(mode == 0){
        Snd_SePlay(WFLBY_SND_CURSOR);
    }
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�N�^�[�̕\���ݒ�
 *
 *	@param	p_wk	���[�N	
 *	@param	flag	�t���O
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_SetClactDraw( WFLBY_ROOM_LISTWIN* p_wk, BOOL flag )
{
	p_wk->clact_draw = flag;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�@�Z���A�N�^�[�\������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ListWin_ContClactDraw( WFLBY_ROOM_LISTWIN* p_wk )
{
	u16 list_pos;

	// ��\�����͂����Ɣ�\��
	if( p_wk->clact_draw == FALSE ){
		CLACT_SetDrawFlag( p_wk->clact[0], FALSE );
		CLACT_SetDrawFlag( p_wk->clact[1], FALSE );
		return ;
	}
	
	// ���X�g�ʒu�擾
	BmpListPosGet( p_wk->p_listwk, &list_pos, NULL );
	
	if( (list_pos <= 0) ){
		CLACT_SetDrawFlag( p_wk->clact[ 0 ], FALSE );
	}else{
		CLACT_SetDrawFlag( p_wk->clact[ 0 ], TRUE );
	}
	if( list_pos >= (p_wk->list_num - 7) ){
		CLACT_SetDrawFlag( p_wk->clact[1], FALSE );
	}else{
		CLACT_SetDrawFlag( p_wk->clact[1], TRUE );
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_Init( WFLBY_ROOM_SUBWIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID )
{
	// �Ƃ��ɂȂ�
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E	�j��
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_Exit( WFLBY_ROOM_SUBWIN* p_wk )
{
	WFLBY_ROOM_SubWin_End( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E	�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *	@param	cx			���L�����ʒu
 *	@param	cy			���L�����ʒu
 *	@param	szcx		���L�����T�C�Y
 *	@param	szcy		���L�����T�C�Y
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_Start( WFLBY_ROOM_SUBWIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID, u8 cx, u8 cy, u8 szcx, u8 szcy )
{
	// �r�b�g�}�b�v�����쐬
	GF_BGL_BmpWinAdd(
				p_sys->p_bgl, &p_wk->win, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN],
				cx, cy,
				szcx, szcy, WFLBY_ROOM_BGPL_SYSFONT_CL,
				WFLBY_LISTWIN_CGX );

    BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_SYSWINGRA_CGX, WFLBY_ROOM_BGPL_SYSWIN );

	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );

	// �\��
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E	�I��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_End( WFLBY_ROOM_SUBWIN* p_wk )
{
	// �E�B���h�E���������Ă��Ȃ���Δj������
	if( GF_BGL_BmpWinAddCheck( &p_wk->win ) == TRUE ){
		// �E�B���h�E��j��
		BmpMenuWinClear( &p_wk->win, WINDOW_TRANS_OFF );
		GF_BGL_BmpWinOffVReq( &p_wk->win );
		GF_BGL_BmpWinDel( &p_wk->win );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E�ɕ������`��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 *	@param	x			�����W�i�h�b�g�j
 *	@param	y			�����W�i�h�b�g�j
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_PrintAll( WFLBY_ROOM_SUBWIN* p_wk, const STRBUF* cp_str, u8 x, u8 y )
{
	// �\��
	GF_STR_PrintColor( &p_wk->win, FONT_SYSTEM, cp_str, x, y,
			MSG_NO_PUT, WFLBY_SUBWIN_MSGCOL, NULL );

	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E�\�����N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_Clear( WFLBY_ROOM_SUBWIN* p_wk )
{
	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�u�E�B���h�E�\���̃N���A
 *
 *	@param	p_wk		���[�N
 *	@param	x			�����W		�S���h�b�g�P��
 *	@param	y			�����W
 *	@param	sx			�T�C�Y��
 *	@param	sy			�T�C�Y��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_SubWin_ClearRect( WFLBY_ROOM_SUBWIN* p_wk, u16 x, u16 y, u16 sx, u16 sy )
{
	// �N���[��
	GF_BGL_BmpWinFill( &p_wk->win, 15, x, y, sx, sy );
}




//----------------------------------------------------------------------------
/**
 *	@brief	YesNo�E�B���h�E�f�[�^�쐬
 *
 *	@param	p_wk		���[�N	
 *	@param	p_msg		���b�Z�[�W�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_YesNoWin_Init( WFLBY_ROOM_YESNOWIN* p_wk, WFLBY_ROOM_DEFMSG* p_msg, u32 heapID )
{
	STRBUF* p_str;

	GF_ASSERT( p_wk->p_bmplist == NULL );
	
	p_wk->p_bmplist = BMP_MENULIST_Create( WFLBY_YESNOWIN_DATANUM, heapID );

	// �f�[�^�쐬
	p_str = WFLBY_ROOM_Msg_Get(p_msg, WFLBY_DEFMSG_TYPE_HIROBA, msg_wifi_hirobawin_02 );
	BMP_MENULIST_AddString( p_wk->p_bmplist, p_str, WFLBY_ROOM_YESNO_OK );
	p_str = WFLBY_ROOM_Msg_Get(p_msg, WFLBY_DEFMSG_TYPE_HIROBA, msg_wifi_hirobawin_01 );
	BMP_MENULIST_AddString( p_wk->p_bmplist, p_str, WFLBY_ROOM_YESNO_NO );


	// �r�b�g�}�b�v���X�g�w�b�_�[�ɐݒ�
	p_wk->bmplist			= sc_WFLBY_ROOM_YESNO_HEADER;
	p_wk->bmplist.count		= WFLBY_YESNOWIN_DATANUM;
	p_wk->bmplist.list		= p_wk->p_bmplist;
}

//----------------------------------------------------------------------------
/**
 *	@brief	YesNo�E�B���h�E�@���X�g�f�[�^�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_YesNoWin_Exit( WFLBY_ROOM_YESNOWIN* p_wk )
{
	if( p_wk->p_bmplist != NULL ){
		BMP_MENULIST_Delete( p_wk->p_bmplist );
		p_wk->p_bmplist = NULL;
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�E�B���h�E�V�X�e��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ErrWin_Init( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID )
{
	//  �r�b�g�}�b�v�m��
	GF_BGL_BmpWinAdd(
				p_sys->p_bgl, &p_wk->win, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_MAIN_MSGWIN],
				WFLBY_ERRWIN_X, WFLBY_ERRWIN_Y,
				WFLBY_ERRWIN_SIZX, WFLBY_ERRWIN_SIZY, WFLBY_ROOM_BGPL_SYSFONT_CL,
				WFLBY_ERRWIN_CGX );

	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->win, 15 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���[�\���V�X�e��	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ErrWin_Exit( WFLBY_ROOM_ERRMSG* p_wk )
{
	// �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�G���[���b�Z�[�W�̕\��
 *	
 *	@param	p_wk		���[�N
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ErrWin_DrawErr( WFLBY_ROOM_ERRMSG* p_wk, const STRBUF* cp_str )
{
	GF_STR_PrintColor( &p_wk->win, FONT_SYSTEM, cp_str, 0, 0,
			MSG_NO_PUT, WFLBY_ERRWIN_MSGCOL, NULL );

	// �E�B���h�E����������
	BmpMenuWinWrite( &p_wk->win, WINDOW_TRANS_OFF, WFLBY_SYSWINGRA_CGX, WFLBY_ROOM_BGPL_SYSWIN );
	GF_BGL_BmpWinOnVReq( &p_wk->win );
}

//----------------------------------------------------------------------------
/**
 *	@brief	DWC�G���[�̕\������
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	errno		�G���[�i���o�[
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ErrWin_DrawDwcErr( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_ROOM_DEFMSG* p_msg )
{
	u32 msgno;
	STRBUF*  p_str;
    COMMSTATE_DWCERROR* pErr;

    pErr = CommStateGetWifiError();
	// ���b�Z�[�W�擾
	msgno = WFLBY_ERR_GetStrID(  pErr->errorCode,  pErr->errorType );	// ���b�Z�[�W�m�n�擾
	WFLBY_ROOM_Msg_SetNumber( p_msg, pErr->errorCode, 5, 0, NUMBER_DISPTYPE_ZERO );	// �d�q�q�m�n�ݒ�
	p_str = WFLBY_ROOM_Msg_Get( p_msg, WFLBY_DEFMSG_TYPE_ERR, msgno );

	// �`��
	WFLBY_ROOM_ErrWin_DrawErr( p_wk, p_str );

}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[���G���[�̕\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W�V�X�e��
 *	@param	errno		�G���[�i���o�[
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ErrWin_DrawLobbyErr( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_ROOM_DEFMSG* p_msg, int errno )
{
	u32 msgno;
	s32 draw_no;
	STRBUF*  p_str;

	draw_no = DWC_LOBBY_GetErrNo( errno );

	// ���b�Z�[�W�擾
	WFLBY_ROOM_Msg_SetNumber( p_msg, draw_no, 5, 0, NUMBER_DISPTYPE_ZERO );	// �d�q�q�m�n�ݒ�
	p_str = WFLBY_ROOM_Msg_Get( p_msg, WFLBY_DEFMSG_TYPE_ERR, dwc_lobby_0001 );

	// �`��
	WFLBY_ROOM_ErrWin_DrawErr( p_wk, p_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���r�[�V�X�e�������@��	�G���[�\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W�V�X�e��
 *	@param	type		�G���[�^�C�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_ErrWin_DrawSystemErr( WFLBY_ROOM_ERRMSG* p_wk, WFLBY_ROOM_DEFMSG* p_msg, WFLBY_SYSTEM_ERR_TYPE type )
{
	STRBUF*  p_str;

	// ���b�Z�[�W�擾
	p_str = WFLBY_ROOM_Msg_Get( p_msg, WFLBY_DEFMSG_TYPE_ERR, dwc_error_0015 );
	// �`��
	WFLBY_ROOM_ErrWin_DrawErr( p_wk, p_str );
}




//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�V�X�e���쐬	������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_Init( WFLBY_ROOM_DEFMSG* p_wk, u32 heapID )
{
	int i;
	static const u32 sc_DEFMSG_INX[ WFLBY_DEFMSG_TYPE_NUM ] = {
		NARC_msg_wifi_h_info_dat,
		NARC_msg_wifi_hiroba_dat,
		NARC_msg_wifi_system_dat,
		NARC_msg_wifi_aisatu_dat
	};
	
	p_wk->p_wordset = WORDSET_CreateEx( WORDSET_DEFAULT_SETNUM, WORDSET_COUNTRY_BUFLEN, heapID );
	for( i=0; i<WFLBY_DEFMSG_TYPE_NUM; i++ ){
		p_wk->p_msgman[i]	= MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, sc_DEFMSG_INX[i], heapID );
	}
	p_wk->p_str		= STRBUF_Create( WFLBY_DEFMSG_STRNUM, heapID );
	p_wk->p_tmp		= STRBUF_Create( WFLBY_DEFMSG_STRNUM, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W	�j��
 *
 *	@param	p_wk		���[�N
 */	
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_Exit( WFLBY_ROOM_DEFMSG* p_wk )
{
	int i;
	WORDSET_Delete( p_wk->p_wordset );
	for( i=0; i<WFLBY_DEFMSG_TYPE_NUM; i++ ){
		MSGMAN_Delete( p_wk->p_msgman[i] );
	}
	STRBUF_Delete( p_wk->p_str );
	STRBUF_Delete( p_wk->p_tmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�擾
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static STRBUF* WFLBY_ROOM_Msg_Get( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_DEFMSG_TYPE type, u32 strid )
{
	MSGMAN_GetString( p_wk->p_msgman[type], strid, p_wk->p_tmp );
	WORDSET_ExpandStr( p_wk->p_wordset, p_wk->p_str, p_wk->p_tmp );
	return p_wk->p_str;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�ɐ�����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	num			����
 *	@param	keta		��
 *	@param	bufid		�o�b�t�@�h�c
 *	@param	disptype	���߂Ƃ�
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetNumber( WFLBY_ROOM_DEFMSG* p_wk, u32  num, u32 keta, u32 bufid, NUMBER_DISPTYPE disptype )
{
	WORDSET_RegisterNumber( p_wk->p_wordset, bufid, num, keta, disptype, NUMBER_CODETYPE_DEFAULT );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀv���C���[����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_mystatus	MYSTATUS
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetPlayerName( WFLBY_ROOM_DEFMSG* p_wk, const MYSTATUS* cp_mystatus, u32 bufid )
{
	WORDSET_RegisterPlayerName( p_wk->p_wordset, bufid, cp_mystatus );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		�~�j�Q�[���^�C�v
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetMinigame( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_GAMETYPE type, u32 bufid )
{
	WORDSET_RegisterWiFiLobbyGameName( p_wk->p_wordset, bufid, type );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�~�j�Q�[����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	type		�^�C���C�x���g�^�C�v
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetTimeevent( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_EVENTGMM_TYPE type, u32 bufid )
{
	WORDSET_RegisterWiFiLobbyEventName( p_wk->p_wordset, bufid, type );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	item		�K�W�F�b�g�^�C�v
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetItem( WFLBY_ROOM_DEFMSG* p_wk, WFLBY_ITEMTYPE item, u32 bufid )
{
	WORDSET_RegisterWiFiLobbyItemName( p_wk->p_wordset, bufid, item );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������ݒ�
 *
 *	@param	p_wk		���[�N	
 *	@param	country		��ID
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetCountry( WFLBY_ROOM_DEFMSG* p_wk, u32 country, u32 bufid )
{
	WORDSET_RegisterCountryName( p_wk->p_wordset, bufid, country );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�n�於��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	country		��ID
 *	@param	area		�n��ID
 *	@param	bufid		�o�b�t�@ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetArea( WFLBY_ROOM_DEFMSG* p_wk, u32 country, u32 area, u32 bufid )
{
	WORDSET_RegisterLocalPlaceName( p_wk->p_wordset, bufid, country, area );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ȈՉ�b��ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	bufID		�o�b�t�@ID
 *	@param	word		�P��ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_SetPMSWord( WFLBY_ROOM_DEFMSG* p_wk, u32 bufID, PMS_WORD word )
{
	WORDSET_RegisterPMSWord( p_wk->p_wordset, bufID, word );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�̃o�b�t�@�N���A
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_Msg_ClearWordSet( WFLBY_ROOM_DEFMSG* p_wk )
{
	WORDSET_ClearAllBuffer( p_wk->p_wordset ) ;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�N���t���O��ݒ肷��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_GadGet_SetFlag( WFLBY_ROOMWK* p_wk )
{
	p_wk->gadget_flag = TRUE;
}




//----------------------------------------------------------------------------
/**
 *	@brief	������ʏ���������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_mystatus	MYSTATUS
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Init( WFLBY_UNDER_WIN* p_wk, const WFLBY_ROOM_SAVE* cp_save, WFLBY_GRAPHICCONT* p_sys, const MYSTATUS* cp_mystatus, u32 heapID )
{
	u32 sex;

	// ���ʎ擾
	sex = MyStatus_GetMySex( cp_mystatus );
	
	p_wk->p_handle = ArchiveDataHandleOpen( ARC_WIFILOBBY_OTHER_GRA, heapID );
	WFLBY_ROOM_UNDERWIN_Common_Init( p_wk, p_sys, p_wk->p_handle, sex, heapID );

	// �g���[�i�J�[�h
	WFLBY_ROOM_UNDERWIN_TrCard_Init( &p_wk->tr_card, p_sys, p_wk->p_handle, heapID );

	// �{�^���̏�����
	WFLBY_ROOM_UNDERWIN_Button_Init( &p_wk->bttn, p_sys, p_wk->p_handle, heapID );
	
	// �V�[�P���X��Ԃ𒲐�
	p_wk->req = WFLBY_UNDERWIN_DRAW_NONE;
	p_wk->now = WFLBY_UNDERWIN_DRAW_NONE;
	if( cp_save->pl_inevno == WFLBY_EV_DEF_PLAYERIN_NORMAL ){
		p_wk->seq = WFLBY_UNDERWIN_SEQ_STARTWAIT;

		// �o�b�N�p���b�g�J���[�����ɂ��Ă���
		GF_BGL_BackGroundColorSet( GF_BGL_FRAME0_S, 0 );
		
		// �T�u�ʑS����\��
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
	}else{
		p_wk->seq = WFLBY_UNDERWIN_SEQ_NORMAL;
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	������ʔj������
 *
 *	@param	p_wk		���[�N
 *	@param	p_boardwin	�Ŕ��[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Exit( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys )
{
	// �{�^���j��
	WFLBY_ROOM_UNDERWIN_Button_Exit( &p_wk->bttn );
	
	// �g���[�i�J�[�h
	WFLBY_ROOM_UNDERWIN_TrCard_Exit( &p_wk->tr_card, p_boardwin, p_sys );

	WFLBY_ROOM_UNDERWIN_Common_Exit( p_wk, p_sys );
	ArchiveDataHandleClose( p_wk->p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C������
 *
 *	@param	p_wk		���[�N
 *	@param	p_room		�������[�N
 *	@param	no_input	�{�^�����͂Ȃǎ󂯕t���Ȃ��Ƃ��FTRUE
 *	@param	heapID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Main( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOMWK* p_room, BOOL no_input, u32 heapID )
{
	WFLBY_SYSTEM*		p_system;
	WFLBY_GRAPHICCONT*	p_sys;
	WFLBY_ROOM_DEFMSG*	p_msg;
	BOOL result;

	p_system	= p_room->p_system;
	p_sys		= &p_room->graphic;
	p_msg		= &p_room->def_msg;
	
	// ����V�[�P���X
	switch( p_wk->seq ){
	// �J�n�҂�
	case WFLBY_UNDERWIN_SEQ_STARTWAIT:			
		if( p_wk->bttn_start == TRUE ){
			p_wk->seq ++;
		}
		break;
	// �J�n�t�F�[�h�A�E�g
	case WFLBY_UNDERWIN_SEQ_STARTFADEOUT:		
		WIPE_SYS_Start(WIPE_PATTERN_S, 
				WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_WHITE, 4, 1, heapID );
		p_wk->seq ++;
		break;
	// �J�n�t�F�[�h�A�E�g�҂�
	case WFLBY_UNDERWIN_SEQ_STARTFADEOUTWAIT:	
		if( WIPE_SYS_EndCheck() == TRUE ){

			// �����̃g���[�i�J�[�h��\��
#if 0
			{
				const WFLBY_USER_PROFILE* cp_myprofile;
				BOOL vip;

				// �����̃v���t�B�[���擾
				cp_myprofile = WFLBY_SYSTEM_GetMyProfileLocal( p_system );

				// VIP�f�[�^���擾
				vip			= WFLBY_SYSTEM_GetUserVipFlag( p_system, WFLBY_SYSTEM_GetMyIdx( p_system ) );
				WFLBY_ROOM_UNDERWIN_StartTrCard( p_wk, &p_room->boardwin, p_sys, p_msg, TRUE, heapID, 
						cp_myprofile, cp_myprofile, vip, FALSE, NULL, FALSE );

				// ��Ԃ��X�^�[�g�v���t�B�[����Ԃɂ���
				p_wk->now = WFLBY_UNDERWIN_DRAW_PROFILE;
			}
#endif
			// �K�W�F�b�g�\��
			{
				const WFLBY_USER_PROFILE* cp_profile;
				cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_system );
				WFLBY_ROOM_UNDERWIN_StartBttn( p_wk, p_sys,  &p_room->def_msg, heapID, cp_profile );
				p_wk->now = WFLBY_UNDERWIN_DRAW_BTTN_DUMMY;
			}

			// �T�u�ʑS���\���J�n
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
			p_wk->seq ++;
		}
		break;
	// �J�n�t�F�[�h�C��
	case WFLBY_UNDERWIN_SEQ_STARTFADEIN:			
		WIPE_SYS_Start(WIPE_PATTERN_S, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_WHITE, 6, 1, heapID );
		p_wk->seq ++;
		break;
	// �J�n�t�F�[�h�C���҂�
	case WFLBY_UNDERWIN_SEQ_STARTFADEINWAIT:		
		if( WIPE_SYS_EndCheck() == TRUE ){
			p_wk->seq ++;
		}
		break;

	case WFLBY_UNDERWIN_SEQ_NORMAL:

		// ���͂��󂯕t���Ȃ��ꍇ
		// �Ȃɂ����Ȃ�
		if( no_input == TRUE ){
			break;
		}


		switch( p_wk->now ){
		// �K�W�F�b�g�{�^��
		case WFLBY_UNDERWIN_DRAW_BTTN:
			result = WFLBY_ROOM_UNDERWIN_Button_Main( &p_wk->bttn, p_sys, p_wk->p_handle, heapID );
			if( result == TRUE ){
				// �����̃K�W�F�b�g�N��
				WFLBY_ROOM_GadGet_SetFlag( p_room );
			}
			break;

		// �K�W�F�b�g�{�^���@�_�~�[
		case WFLBY_UNDERWIN_DRAW_BTTN_DUMMY:
			break;
			
		// �t���[�g
		case WFLBY_UNDERWIN_DRAW_FLOAT:
			// �A�j���Đ����Ȃ�Đ����̃A�j���I���҂�
			if( p_wk->flt_anm_flag ){
				if( WFLBY_TIMEEVENT_FLOAT_CheckAnm( p_room->p_timeevent, 
							p_wk->flt_float_idx, p_wk->flt_anm_idx ) == FALSE ){
					p_wk->flt_anm_flag = FALSE;

					// �{�^�����A
					WFLBY_ROOM_UNDERWIN_Button_BttnLockClear( &p_wk->bttn );
				}
			}

			
			result = WFLBY_ROOM_UNDERWIN_Button_Main( &p_wk->bttn, p_sys, p_wk->p_handle, heapID );
			if( result == TRUE ){
				// �t���[�g�A�j���N��
				WFLBY_SYSTEM_SetFloatAnm( p_system, WFLBY_SYSTEM_GetMyIdx( p_system ) );
				WFLBY_SYSTEM_SendFloatAnm( p_system );	// �A�j���f�[�^���M

				// �Đ������t���[�g�A�j�����擾
				{
					u32 reserve_idx, float_idx, float_ofs;

					reserve_idx = WFLBY_SYSTEM_GetFloatPlIdxReserve( p_system, WFLBY_SYSTEM_GetMyIdx( p_system ) );
					WFLBY_SYSTEM_GetFloatIdxOfs( reserve_idx, &float_idx, &float_ofs );

					p_wk->flt_anm_flag	= TRUE;
					p_wk->flt_float_idx	= float_idx;
					p_wk->flt_anm_idx	= float_ofs;
				}
			}
			break;

		// �v���t�B�[��
		case WFLBY_UNDERWIN_DRAW_PROFILE:
			WFLBY_ROOM_UNDERWIN_TrCard_Main( &p_wk->tr_card, &p_room->boardwin );
			break;

		default:
			break;
		}


		// �ύX���N�G�X�g�����Ă�����ύX����
		if( p_wk->req != WFLBY_UNDERWIN_DRAW_NONE ){
			p_wk->seq ++;
		}
		break;

	case WFLBY_UNDERWIN_SEQ_FADEOUT:

		WIPE_SYS_Start(WIPE_PATTERN_S, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_BLACK, WFLBY_UNDERWIN_FASE_DIV, WFLBY_UNDERWIN_FASE_SYNC, heapID );
		p_wk->seq ++;
		break;

	case WFLBY_UNDERWIN_SEQ_FADEOUTWAIT:
		if( WIPE_SYS_EndCheck() == TRUE ){

			// �j������
			switch( p_wk->now ){
			case WFLBY_UNDERWIN_DRAW_BTTN:
			case WFLBY_UNDERWIN_DRAW_BTTN_DUMMY:
			case WFLBY_UNDERWIN_DRAW_FLOAT:
				WFLBY_ROOM_UNDERWIN_EndBttn( p_wk, p_sys );
				break;

			case WFLBY_UNDERWIN_DRAW_PROFILE:
				WFLBY_ROOM_UNDERWIN_EndTrCard( p_wk, &p_room->boardwin, p_sys );
				break;
			}
			
			p_wk->seq ++;
		}
		break;

	case WFLBY_UNDERWIN_SEQ_CHANGE:
		{
			const WFLBY_USER_PROFILE* cp_profile;
			BOOL vip;
			BOOL aikotoba;
			const WFLBY_AIKOTOBA_DATA* cp_aikotoba;

#if PL_G0200_080716_FIX
			// �v���t�B�[���̎擾���s�\�Ȃ̂ł���΁A�{�^���\���ɂ���
			if( p_wk->req == WFLBY_UNDERWIN_DRAW_PROFILE ){
				// �������`�F�b�N
				if( p_wk->req_tridx != WFLBY_SYSTEM_GetMyIdx( p_system ) ){
					cp_profile = WFLBY_SYSTEM_GetUserProfile( p_system, p_wk->req_tridx );
					if( cp_profile == NULL ){
						p_wk->req = WFLBY_UNDERWIN_DRAW_BTTN;
					}
				}
			}
#endif
			

			switch( p_wk->req ){
			case WFLBY_UNDERWIN_DRAW_BTTN:
			case WFLBY_UNDERWIN_DRAW_BTTN_DUMMY:
				cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_system );
				WFLBY_ROOM_UNDERWIN_StartBttn( p_wk, p_sys,  &p_room->def_msg, heapID, cp_profile );
				break;

			case WFLBY_UNDERWIN_DRAW_PROFILE:
				{
					BOOL  mydata;
					const WFLBY_USER_PROFILE* cp_myprofile;

					// �����̃v���t�B�[���擾
					cp_myprofile = WFLBY_SYSTEM_GetMyProfileLocal( p_system );

					// �������`�F�b�N
					if( p_wk->req_tridx == WFLBY_SYSTEM_GetMyIdx( p_system ) ){
						cp_profile = WFLBY_SYSTEM_GetMyProfileLocal( p_system );
						mydata = TRUE;
					}else{
						cp_profile = WFLBY_SYSTEM_GetUserProfile( p_system, p_wk->req_tridx );
						mydata = FALSE;
					}

					// VIP�f�[�^���擾
					vip			= WFLBY_SYSTEM_GetUserVipFlag( p_system, p_wk->req_tridx );
					if( p_wk->req_aikotoba == TRUE ){	// �����t�\��������Ƃ����������t�̏����擾����
						aikotoba	= WFLBY_SYSTEM_GetUserAikotobaFlag( p_system, p_wk->req_tridx );
						cp_aikotoba	= WFLBY_SYSTEM_GetUserAikotobaStr( p_system, p_wk->req_tridx );
						if( cp_aikotoba == NULL ){	// �����t���Ȃ��������΂ɍ����t�\���Ȃ��ɂ���i�O�̂��߁j
							aikotoba = FALSE;
						}
					}else{
						aikotoba	= FALSE;
						cp_aikotoba	= NULL;
					}
					WFLBY_ROOM_UNDERWIN_StartTrCard( p_wk, &p_room->boardwin, p_sys, p_msg, mydata, heapID, cp_profile, cp_myprofile, vip, aikotoba, cp_aikotoba, TRUE );
				}
				break;

			case WFLBY_UNDERWIN_DRAW_FLOAT:
				{
					u32 idx;
					u32 floatidx;
					u32 floatoffs;
					idx = WFLBY_SYSTEM_GetFloatPlIdxReserve( p_system, WFLBY_SYSTEM_GetMyIdx( p_system ) );
					if( idx == DWC_LOBBY_USERIDTBL_IDX_NONE ){
						GF_ASSERT(0);
						// �\�񂵂ĂȂ�
						idx = 0;	// 0�ɂ��Ă��܂�
					}
					WFLBY_SYSTEM_GetFloatIdxOfs( idx, &floatidx, &floatoffs );
					WFLBY_ROOM_UNDERWIN_StartBttnFloat( p_wk, p_sys, heapID, floatoffs );
				}
				break;
			}

			
			p_wk->req = WFLBY_UNDERWIN_DRAW_NONE;
			p_wk->seq ++;
		}
		break;

	case WFLBY_UNDERWIN_SEQ_FASEIN:
		WIPE_SYS_Start(WIPE_PATTERN_S, 
				WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
				WIPE_FADE_BLACK, WFLBY_UNDERWIN_FASE_DIV, WFLBY_UNDERWIN_FASE_SYNC, heapID );

		// �����g���[�i�[�J�[�h�Ȃ��x�g���[�i�[�J�[�h�̃��C�������s
		if( p_wk->now == WFLBY_UNDERWIN_DRAW_PROFILE ){
			WFLBY_ROOM_UNDERWIN_TrCard_Main( &p_wk->tr_card, &p_room->boardwin );
		}
		
		p_wk->seq ++;
		break;

	case WFLBY_UNDERWIN_SEQ_FASEINWAIT:

		if( WIPE_SYS_EndCheck() == TRUE ){
			p_wk->seq = WFLBY_UNDERWIN_SEQ_NORMAL;
		}
		break;

	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	������ʂ��t�F�[�h���łȂ����`�F�b�N
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�t�F�[�h��
 *	@retval	FALSE	�t�F�[�h���ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_CheckFade( const WFLBY_UNDER_WIN* cp_wk )
{
	if( cp_wk->seq == WFLBY_UNDERWIN_SEQ_NORMAL ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	������ʂ̃V�[�P���X���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�V�[�P���X
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_ROOM_UNDERWIN_GetSeq( const WFLBY_UNDER_WIN* cp_wk )
{
	return cp_wk->seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���ؑփ��N�G�X�g
 *
 *	@param	p_wk		���[�N
 *	@param	idx			�C���f�b�N�X
 *	@param	aikotoba	�����t���Ђ傤������̂�
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_ReqTrCard( WFLBY_UNDER_WIN* p_wk, u32 idx, BOOL aikotoba )
{
	p_wk->req			= WFLBY_UNDERWIN_DRAW_PROFILE;
	p_wk->req_tridx		= idx;
	p_wk->req_aikotoba	= aikotoba;
}

//----------------------------------------------------------------------------
/**
 *	@brief	������ʃg���[�i�J�[�h	�A�C�e���A�C�R���ύX
 *	
 *	@param	p_wk	���[�N
 *	@param	p_sys	�O���t�B�b�N���[�N
 *	@param	item	�A�C�e��
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_ChangeItemTrCard( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 item, u32 heapID )
{
	GF_ASSERT( item < WFLBY_ITEM_NUM );
	GF_ASSERT( p_wk->now == WFLBY_UNDERWIN_DRAW_PROFILE );

	// �ւ񂱂��ł���̂��H
	if( ( p_wk->now == WFLBY_UNDERWIN_DRAW_PROFILE ) ){
		// �ύX
		WFLBY_ROOM_UNDERWIN_TrCard_ChangeItem( &p_wk->tr_card, p_sys, p_wk->p_handle, heapID, item );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���؂�ւ����N�G�X�g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_ReqBttn( WFLBY_UNDER_WIN* p_wk )
{
	if( p_wk->now != WFLBY_UNDERWIN_DRAW_BTTN ){
		p_wk->req = WFLBY_UNDERWIN_DRAW_BTTN;
	} 
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�{�^���؂�ւ����N�G�X�g
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_ReqBttnFloat( WFLBY_UNDER_WIN* p_wk )
{
	if( p_wk->now != WFLBY_UNDERWIN_DRAW_FLOAT ){
		p_wk->req = WFLBY_UNDERWIN_DRAW_FLOAT;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h�̃v���C���[�C���f�b�N�X���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�v���C���[�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static u32 WFLBY_ROOM_UNDERWIN_GetTrCardPlIdx( const WFLBY_UNDER_WIN* cp_wk )
{
	return cp_wk->req_tridx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	trainer�J�[�h��\�����Ă��邩���擾����
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	trainer�J�[�h�\����
 *	@retval	FALSE	trainer�J�[�h�\�����ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_CheckTrCardDraw( const WFLBY_UNDER_WIN* cp_wk )
{
	if( ( cp_wk->req == WFLBY_UNDERWIN_DRAW_PROFILE ) ){
		return TRUE;
	}
	if( ( cp_wk->now == WFLBY_UNDERWIN_DRAW_PROFILE ) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h��OFF�ɂ��邱�Ƃ����̂����H
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�\
 *	@retval	FALSE	�s�\
 *
 *	������Ƃ킩��ɂ����ł����A
 *	���N�G�X�g��NONE�Ńg���[�i�[�J�[�h�\�������H�Ƃ������Ƃł��B
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_CheckTrCardCanOff( const WFLBY_UNDER_WIN* cp_wk )
{
	if( ( cp_wk->req == WFLBY_UNDERWIN_DRAW_NONE ) && ( cp_wk->now == WFLBY_UNDERWIN_DRAW_PROFILE ) ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�����t��\�����Ă��邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�\�����Ă�
 *	@retval	FALSE	�\�����ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_CheckTrCardAikotobaDraw( const WFLBY_UNDER_WIN* cp_wk )
{
	return WFLBY_ROOM_UNDERWIN_TrCard_CheckAikotobaDraw( &cp_wk->tr_card );
}

#if PL_T0862_080712_FIX
//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g���������Ă��邩�`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�������Ă���
 *	@retval	FALSE	�������ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_CheckReqNone( const WFLBY_UNDER_WIN* cp_wk )
{
	if( cp_wk->req != WFLBY_UNDERWIN_DRAW_NONE ){
		return  TRUE;
	}
	return FALSE;
}
#endif

//----------------------------------------------------------------------------
/**
 *	@brief	�X�e�[�^�X�\���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_boardwin	�ŔE�B���h�E�V�X�e��
 *	@param	p_sys		�O���t�B�b�N�V�X�e��
 *	@param	p_msg		���b�Z�[�W
 *	@param	mydata		�����̃f�[�^��
 *	@param	heapID		�q�[�v�h�c	
 *	@param	cp_profile	���[�U�v���t�B�[��
 *	@param	cp_myprofile���[�U�v���t�B�[��
 *	@param	vip			VIP�t���O
 *	@param	aikotoba	�����t�`��t���O
 *	@param	cp_aikotoba	�����t������
 *	@param	draw_item	�A�C�e����\�����邩
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_StartTrCard( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, BOOL mydata, u32 heapID, const WFLBY_USER_PROFILE* cp_profile, const WFLBY_USER_PROFILE* cp_myprofile, BOOL vip, BOOL aikotoba, const WFLBY_AIKOTOBA_DATA* cp_aikotoba, BOOL draw_item )
{

	// �v���t�B�[����\�����Ȃ炢���������
	if( p_wk->now == WFLBY_UNDERWIN_DRAW_PROFILE ){
		WFLBY_ROOM_UNDERWIN_EndTrCard( p_wk, p_boardwin, p_sys );
	}
	
	WFLBY_ROOM_UNDERWIN_TrCard_Start( p_wk, p_boardwin, &p_wk->tr_card, p_sys, p_msg, mydata, p_wk->p_handle, heapID, cp_profile, cp_myprofile, vip, aikotoba, cp_aikotoba, draw_item );
	p_wk->now = WFLBY_UNDERWIN_DRAW_PROFILE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h�\���n�e�e
 *
 *	@param	p_wk		���[�N
 *	@param	p_boardwin	�ŔE�B���h�E
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_EndTrCard( WFLBY_UNDER_WIN* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys )
{
	WFLBY_ROOM_UNDERWIN_TrCard_End( &p_wk->tr_card, p_boardwin, p_sys );
	p_wk->now = WFLBY_UNDERWIN_DRAW_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���̋N��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e��
 *	@param	p_msg		���b�Z�[�W
 *	@param	heapID		�q�[�vID
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_StartBttn( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, u32 heapID, const WFLBY_USER_PROFILE* cp_profile )
{
	// �{�^����\�����Ȃ炢���������
	if( (p_wk->now == WFLBY_UNDERWIN_DRAW_BTTN) ||
		(p_wk->now == WFLBY_UNDERWIN_DRAW_BTTN_DUMMY) ||
		(p_wk->now == WFLBY_UNDERWIN_DRAW_FLOAT) ){
		WFLBY_ROOM_UNDERWIN_EndBttn( p_wk, p_sys );
	}
	
	WFLBY_ROOM_UNDERWIN_Button_Start( &p_wk->bttn, p_sys, p_msg, cp_profile, 
			p_wk->p_handle, heapID );
	p_wk->now = WFLBY_UNDERWIN_DRAW_BTTN;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[�g�{�^���̋N��
 *
 *	@param	p_wk			���[�N
 *	@param	p_sys			�V�X�e��
 *	@param	heapID			�q�[�v�h�c
 *	@param	idx				�t���[�g�����Ă���Ƃ���C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_StartBttnFloat( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID, u32 idx )
{
	// �{�^����\�����Ȃ炢���������
	if( p_wk->now == WFLBY_UNDERWIN_DRAW_FLOAT ){
		WFLBY_ROOM_UNDERWIN_EndBttn( p_wk, p_sys );
	}
	
	WFLBY_ROOM_UNDERWIN_Button_StartFloat( &p_wk->bttn, p_sys, idx, 
			p_wk->p_handle, heapID );
	p_wk->now = WFLBY_UNDERWIN_DRAW_FLOAT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���̏I��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_EndBttn( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
	WFLBY_ROOM_UNDERWIN_Button_End( &p_wk->bttn, p_sys );

	p_wk->now = WFLBY_UNDERWIN_DRAW_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^�������~�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	stop		��~������Ƃ�TRUE
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_SetBttnStop( WFLBY_UNDER_WIN* p_wk, BOOL stop )
{
	p_wk->bttn.bttnstop = stop;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WIFI���r�[����ʗp�p���b�gVBlank�]���V�X�e��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e��
 *	@param	dataidx		�f�[�^�C���f�b�N�X
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_PalTrans( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 dataidx, u32 heapID )
{
	p_wk->dataidx	= dataidx;
	p_wk->heapID	= heapID;
	// �^�X�N����
	VWaitTCB_Add( WFLBY_ROOM_UNDERWIN_PalTransVTcb, p_wk, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�]��TCB
 *
 *	@param	p_tcb		TCB
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_PalTransVTcb( TCB_PTR p_tcb, void* p_work )
{
	WFLBY_UNDER_WIN* p_wk = p_work;
	
	// �o�b�N�O���E���h
	{
		// �J���[�p���b�g
		ArcUtil_HDL_PalSet( p_wk->p_handle, 
				p_wk->dataidx, 
				PALTYPE_SUB_BG, 0, 
				0, p_wk->heapID );
	}

	//  �����������ݗp
	{
		SystemFontPaletteLoad( PALTYPE_SUB_BG, WFLBY_ROOM_BGSPL_FONTCL*32, p_wk->heapID );
	}

	// �Z�^�C�v�������ݗp�p���b�g
	{
		ArcUtil_PalSet( WazaTypeIcon_ArcIDGet(), 
				WazaTypeIcon_PlttIDGet(), PALTYPE_SUB_BG, WFLBY_ROOM_BGSPL_WAZATYPE0*32, 3*32, p_wk->heapID );
	}

	// �^�X�N�j��
	TCB_Delete( p_tcb );
}



//----------------------------------------------------------------------------
/**
 *	@brief	���ʃO���t�B�b�N������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_handle	�n���h��
 *	@param	sex			�����̐���
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Common_Init( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 sex, u32 heapID )
{
	// �o�b�N�O���E���h
	{
		// �J���[�p���b�g
		if( sex == PM_MALE ){
			ArcUtil_HDL_PalSet( p_handle, 
					NARC_wifi_lobby_other_gadget_sub_NCLR, 
					PALTYPE_SUB_BG, 0, 
					0, heapID );
		}else{
			ArcUtil_HDL_PalSet( p_handle, 
					NARC_wifi_lobby_other_gadget_sub_2_NCLR, 
					PALTYPE_SUB_BG, 0, 
					0, heapID );
		}

		// �L�����N�^
		ArcUtil_HDL_BgCharSet( p_handle, 
				NARC_wifi_lobby_other_gadget_bg_NCGR, p_sys->p_bgl, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK], 0, 0, FALSE, heapID );
	}

	//  �����������ݗp
	{
		SystemFontPaletteLoad( PALTYPE_SUB_BG, WFLBY_ROOM_BGSPL_FONTCL*32, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʃO���t�B�b�N�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Common_Exit( WFLBY_UNDER_WIN* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���������݋��ʏ���
 *
 *	@param	p_sys		�`��V�X�e��
 *	@param	p_handle	�n���h��
 *	@param	scrnid		�X�N���[���h�c
 *	@param	bg_frame	BG�t���[��
 *	@param	cgofs		�b�fofs
 *	@param	heapID		�q�[�v�h�c
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Common_LoadScrn( WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 scrnid, u32 bg_frame, u32 cgofs, u32 heapID )
{
	void* p_buff;
	NNSG2dScreenData* p_scrn;
	p_buff = ArcUtil_HDL_ScrnDataGet( p_handle, scrnid,  FALSE,
			 &p_scrn, heapID );

	// �X�N���[���L�����N�^�I�t�Z�b�g�l����������
	{
		int i;
		u16* p_scrndata;
		int size;

		size = p_scrn->szByte / 2;

		// �X�N���[���f�[�^���
		p_scrndata = (u16*)p_scrn->rawData;

		for(i=0; i<size; i++){
			p_scrndata[ i ] += cgofs;
		}
	}

	// �a�f�k�ɏ�������
	GF_BGL_ScrWrite( p_sys->p_bgl, bg_frame, p_scrn->rawData, 
			0, 0, p_scrn->screenWidth/8, p_scrn->screenHeight/8 );

	// �]���t���O�𗧂Ă�
	GF_BGL_LoadScreenV_Req( p_sys->p_bgl, bg_frame );

	sys_FreeMemoryEz( p_buff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	trainer�J�[�h	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�O���t�B�b�N���[�N
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_Init( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID )
{
	// �E�B���h�E������
	{
		int i;

		for( i=0; i<WFLBY_TRCARD_WIN_NUM; i++ ){
			GF_BGL_BmpWinAddEx( p_sys->p_bgl, &p_wk->win[i], &sc_WFLBY_TRCARD_WIN_DATA[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�ȁ[�J�[�h�j������
 *
 *	@param	p_wk		���[�N
 *	@param	p_boardwin	�ŔE�B���h�E�V�X�e��
 *	@param	p_sys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_Exit( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys )
{
	// End���Ă΂�Ă��Ȃ���ԂȂ�j��
	if( p_wk->p_itemicon != NULL ){
		WFLBY_ROOM_UNDERWIN_TrCard_End( p_wk, p_boardwin, p_sys );
	}

	// �E�B���h�E�j��
	{
		int i;

		for( i=0; i<WFLBY_TRCARD_WIN_NUM; i++ ){
			GF_BGL_BmpWinDel( &p_wk->win[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	trainer�J�[�h���C������
 *
 *	@param	p_wk		���[�N
 *	@param	p_boardwin	�Ŕ��[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_Main( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	trainer�J�[�h	�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_boardwin	�ŔE�B���h�E�V�X�e��
 *	@param	p_sys		�O���t�B�b�N���[�N
 *	@param	p_msg		���b�Z�[�W
 *	@param	mydata		�����̃f�[�^��
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 *	@param	cp_profile	�v���t�B�[��
 *	@param	cp_myprofile�����̃v���t�B�[��
 *	@param	vip			VIP�t���O
 *	@param	aikotoba	�����t�`��t���O
 *	@param	cp_aikotoba	�����t������
 *	@param	item_draw	�^�b�`�g�C��\�����邩
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_Start( WFLBY_UNDER_WIN* p_ugwk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, BOOL mydata, ARCHANDLE* p_handle, u32 heapID, const WFLBY_USER_PROFILE* cp_profile, const WFLBY_USER_PROFILE* cp_myprofile, BOOL vip, BOOL aikotoba, const WFLBY_AIKOTOBA_DATA* cp_aikotoba, BOOL item_draw )
{
	GF_PRINTCOLOR col;
	MYSTATUS* p_mystatus;

	// ���[�h�Z�b�g�̒��g�N���A
	WFLBY_ROOM_Msg_ClearWordSet( p_msg );
	
	// ���ʂ���J���[���擾
	if( WFLBY_SYSTEM_GetProfileSex( cp_profile ) == PM_MALE ){
		col = WFLBY_TRCARD_COL_BLUE;

		//  VBlank���ԃJ���[�p���b�g�]��
		if( vip == FALSE ){
			WFLBY_ROOM_UNDERWIN_PalTrans( p_ugwk, p_sys, NARC_wifi_lobby_other_gadget_sub_NCLR, heapID );
		}else{
			// VIP�Ȃ�VIP�J���[
			WFLBY_ROOM_UNDERWIN_PalTrans( p_ugwk, p_sys, NARC_wifi_lobby_other_gadget_sub_3_NCLR, heapID );
		}
	}else{
		col = WFLBY_TRCARD_COL_RED;

		//  VBlank���ԃJ���[�p���b�g�]��
		if( vip == FALSE ){
			WFLBY_ROOM_UNDERWIN_PalTrans( p_ugwk, p_sys, NARC_wifi_lobby_other_gadget_sub_2_NCLR, heapID );
		}else{
			// VIP�Ȃ�VIP�J���[
			WFLBY_ROOM_UNDERWIN_PalTrans( p_ugwk, p_sys, NARC_wifi_lobby_other_gadget_sub_3_NCLR, heapID );
		}
	}

	// �`��X�N���[���N���A
	GF_BGL_ScrClearCodeVReq( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK], 0 );
	GF_BGL_ScrClearCodeVReq( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN_TR], 0 );
	GF_BGL_ScrClearCodeVReq( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG], 0 );
	WFLBY_ROOM_UNDERWIN_TrCard_WinClear( p_wk );
	
	// �X�N���[���]��
	WFLBY_ROOM_UNDERWIN_Common_LoadScrn( p_sys, p_handle, 
			NARC_wifi_lobby_other_profile2_NSCR, 
			sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK], 0, heapID );

	// �v���t�B�[������MyStatus���쐬����
	p_mystatus = MyStatus_AllocWork( heapID );
	WFLBY_SYSTEM_GetProfileMyStatus( cp_profile, p_mystatus, heapID );
	
	// �E�B���h�E�ւ̏�������
	// ���O
	{
		u32 namesize;
		
		// ID No
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_NAME, 
				msg_hiroba_profile_01, 0, 0, col );
		// ID
		WFLBY_ROOM_Msg_SetNumber( p_msg, 
				MyStatus_GetID_Low( p_mystatus ), 5, 0, NUMBER_DISPTYPE_ZERO );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrintRightSide( p_wk, p_msg, WFLBY_TRCARD_WIN_NAME, 
				msg_hiroba_profile_05_01, 122, 0, WFLBY_TRCARD_COL_BLACK );

		// ���O
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_NAME, 
				msg_hiroba_profile_02, 0, 16, col );
		// �Ȃ܂�
		WFLBY_ROOM_UNDERWIN_TrCard_WinSetName( p_wk, p_msg, p_mystatus );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrintRightSide( p_wk, p_msg, WFLBY_TRCARD_WIN_NAME, 
				msg_hiroba_profile_05_02, 122, 16, WFLBY_TRCARD_COL_BLACK );

		// �E�B���h�E�̓]��
		WFLBY_ROOM_UNDERWIN_TrCard_WinOn( p_wk, WFLBY_TRCARD_WIN_NAME );
	}

	// �n�於
	{
		// ����ł�΂���
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_COUNTRY, 
				msg_hiroba_profile_03, 0, 0, col );

		// �n�悪���邩
		if( WFLBY_SYSTEM_GetProfileNation( cp_profile ) == WFLBY_WLDDATA_NATION_NONE ) {
			// �_�~�[���b�Z�[�W
			WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_COUNTRY, 
					msg_hiroba_profile_06, 32, 16, WFLBY_TRCARD_COL_BLACK );
		}else{

			// ����
			WFLBY_ROOM_UNDERWIN_TrCard_WinSetCountry( p_wk, p_msg, cp_profile );
			WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_COUNTRY, 
					msg_hiroba_profile_05_03, 32, 16, WFLBY_TRCARD_COL_BLACK );
		 }


		// �E�B���h�E�̓]��
		WFLBY_ROOM_UNDERWIN_TrCard_WinOn( p_wk, WFLBY_TRCARD_WIN_COUNTRY );
		
	}

	// �����t������[�������t�\��
	//		�@ �Ȃ��[���`�����n�߂��Ƃ��ƁA�Ō�ɂ������ƕ\��
	if( aikotoba ){
		// �����t
		p_wk->aikotoba_draw = TRUE;

		// �^�C�g��
		WFLBY_ROOM_UNDERWIN_TrCard_WinSetName( p_wk, p_msg, p_mystatus );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_VIPAIKOTOBA, 
				msg_hiroba_profile_07, 8, 0, col );

		// �����t�ݒ�
		WFLBY_ROOM_UNDERWIN_TrCard_WinSetAikotoba( p_wk, p_msg, cp_aikotoba );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_VIPAIKOTOBA, 
				msg_hiroba_profile_10, 6, 24, WFLBY_TRCARD_COL_BLACK );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_VIPAIKOTOBA, 
				msg_hiroba_profile_11, 90, 24, WFLBY_TRCARD_COL_BLACK );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_VIPAIKOTOBA, 
				msg_hiroba_profile_12, 6, 44, WFLBY_TRCARD_COL_BLACK );
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_VIPAIKOTOBA, 
				msg_hiroba_profile_13, 90, 44, WFLBY_TRCARD_COL_BLACK );

		// �E�B���h�E�̓]��
		WFLBY_ROOM_UNDERWIN_TrCard_WinOn( p_wk, WFLBY_TRCARD_WIN_VIPAIKOTOBA );
		
	}else{
		p_wk->aikotoba_draw = FALSE;

		// �ڂ�������͂��߂��Ƃ�
		WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_VIPAIKOTOBA, 
				msg_hiroba_profile_05, 8, 0, col );

		// �E�B���h�E�̓]��
		WFLBY_ROOM_UNDERWIN_TrCard_WinOn( p_wk, WFLBY_TRCARD_WIN_VIPAIKOTOBA );

		// �s���������P�Q���o��
		WFLBY_ROOM_UNDERWIN_TrCard_InitRireki( p_wk, p_sys, p_handle, heapID, cp_profile, cp_myprofile );
		
#if 0
		//  �`�����͂��߂��Ƃ�
		{
			RTCDate date;
			RTCTime time;
			s64	start_sec;
			BOOL sec_check;

			// �ڂ�������͂��߂��Ƃ�
			WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
					msg_hiroba_profile_04, 0, 0, col );

			//  ���t�擾
			sec_check	= WFLBY_SYSTEM_CheckProfileGameStartTime( cp_profile );

			if( sec_check == TRUE ){
				start_sec	= WFLBY_SYSTEM_GetProfileGameStartTime( cp_profile );
				RTC_ConvertSecondToDateTime( &date, &time, 
						start_sec );
				
				// �N
				// year�̒��g��2007�Ȃ�07 2008�N�Ȃ�08�������Ă���̂�2000�𑫂�
				WFLBY_ROOM_Msg_SetNumber( p_msg, 2000+date.year, 4, 0, NUMBER_DISPTYPE_ZERO );
				WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
						msg_hiroba_profile_05_04, 58, 16, WFLBY_TRCARD_COL_BLACK );
				// ��
				WFLBY_ROOM_Msg_SetNumber( p_msg, date.month, 2, 0, NUMBER_DISPTYPE_ZERO );
				WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
						msg_hiroba_profile_05_05, 102, 16, WFLBY_TRCARD_COL_BLACK );
				// ��
				WFLBY_ROOM_Msg_SetNumber( p_msg, date.day, 2, 0, NUMBER_DISPTYPE_ZERO );
				WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
						msg_hiroba_profile_05_05, 128, 16, WFLBY_TRCARD_COL_BLACK );
			}else{

				// �S���H�H�H�H

				// �N
				WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
						msg_hiroba_profile_08, 58, 16, WFLBY_TRCARD_COL_BLACK );
				// ��
				WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
						msg_hiroba_profile_09, 102, 16, WFLBY_TRCARD_COL_BLACK );
				// ��
				WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_TRSTART, 
						msg_hiroba_profile_09, 128, 16, WFLBY_TRCARD_COL_BLACK );

			}

			// �E�B���h�E�̓]��
			WFLBY_ROOM_UNDERWIN_TrCard_WinOn( p_wk, WFLBY_TRCARD_WIN_TRSTART );

		}


		//  �Ō�ɂ�������
		{
			// �ڂ�������͂��߂��Ƃ�
			WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_LASTACTION, 
					msg_hiroba_profile_05, 0, 0, col );

			// ��������
			WFLBY_ROOM_UNDERWIN_TrCard_WinSetLastAction( p_wk, p_msg, cp_profile );
			WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( p_wk, p_msg, WFLBY_TRCARD_WIN_LASTACTION, 
					msg_hiroba_profile_05_06, 32, 16, WFLBY_TRCARD_COL_BLACK );

			// �E�B���h�E�̓]��
			WFLBY_ROOM_UNDERWIN_TrCard_WinOn( p_wk, WFLBY_TRCARD_WIN_LASTACTION );
		}
#endif
	}

	// �A�C�e���A�C�R���ݒ�
	WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Init( p_wk, p_sys, p_handle, heapID, 
			WFLBY_SYSTEM_GetProfileItem( cp_profile ), item_draw );



	// �g���[�i�O���t�B�b�N�\��
#ifndef WFLBY_DEBUG_ROOM_TRTYPE_CHG
	{
		u32 tr_type;
		if( mydata == TRUE ){
			// �����̃f�[�^�Ȃ�A
			// ��l���̃L�����N�^
			if( WFLBY_SYSTEM_GetProfileSex( cp_profile ) == PM_FEMALE ){
				tr_type = HEROINE;
			}else{
				tr_type = HERO;
			}
		}else{

			// ���l�̃f�[�^�Ȃ�A
			// ���̂܂܂�
			tr_type = WFLBY_SYSTEM_GetProfileTrType( cp_profile );
		}
		WFLBY_ROOM_UNDERWIN_TrCard_TrView_Init( p_wk, p_sys, p_handle, heapID,
				tr_type );
	}
#else
	{
		static u32 trtype = 0;

		WFLBY_ROOM_UNDERWIN_TrCard_TrView_Init( p_wk, p_sys, p_handle, heapID,
				sc_WFLBY_TRCARD_TRTYPE[trtype].fld_trno );
		trtype = (trtype+1) % WFLBY_TRCARD_TRTYPE_NUM;
	}
#endif

	// �Z�^�C�v�\��
	WFLBY_ROOM_UNDERWIN_TrCard_WriteWazaType( p_ugwk, p_wk, p_sys, heapID, cp_profile );

	// MyStatus�j��
	sys_FreeMemoryEz( p_mystatus );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�v��`�悷��
 *
 *	@param	p_ugwk		������ʃ��[�N
 *	@param	p_wk		�v���t�B�[�����[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	heapID		�q�[�vID	�i�O���t�B�b�N��ǂݍ��ށ@�e���|������RAM�̊m�ۂ̂݁j
 *	@param	cp_profile	�v���t�B�[�����[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WriteWazaType( WFLBY_UNDER_WIN* p_ugwk, WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 heapID, const WFLBY_USER_PROFILE* cp_profile )
{
	u16 type;
	int i;
	u16 bt_type;
	void* p_char;
	NNSG2dCharacterData* p_chardata;


	for( i=0; i<WFLBY_SELECT_TYPENUM; i++ ){
		type = WFLBY_SYSTEM_GetProfileWazaType( cp_profile, i );
		
		if( type != WFLBY_POKEWAZA_TYPE_NONE ){

			// �L����Z�^�C�v����o�g���p�Z�^�C�v�ɕύX
			bt_type = WFLBY_BattleWazaType_Get( type );

			// �L�����N�^�W�J
			p_char = ArcUtil_Load( WazaTypeIcon_ArcIDGet(),
					WazaTypeIcon_CgrIDGet( bt_type ),
					TRUE, heapID, ALLOC_BOTTOM );
			NNS_G2dGetUnpackedCharacterData( p_char, &p_chardata );
			GF_BGL_LoadCharacter(p_sys->p_bgl, 
					sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
					p_chardata->pRawData, 
					WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX*WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY*32,
					sc_WFLBY_TRCARD_WAZATYPE_CGX[i] );
			sys_FreeMemoryEz( p_char );

			// �X�N���[����������
			GF_BGL_ScrWriteExpand( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
					WFLBY_TRCARD_WIN_WAZATYPE1_X,
					WFLBY_TRCARD_WIN_WAZATYPE1_Y+(WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY*i), 
					WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX, 
					WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY,
					sc_WFLBY_TRCARD_WAZATYPE_SCRN[i],					
					0, 0, 
					WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX, WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY );

			// �p���b�g�ݒ�
			GF_BGL_ScrPalChange( p_sys->p_bgl, 
					sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
					WFLBY_TRCARD_WIN_WAZATYPE1_X,
					WFLBY_TRCARD_WIN_WAZATYPE1_Y+(WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY*i), 
					WFLBY_TRCARD_WIN_WAZATYPE1_SIZEX, WFLBY_TRCARD_WIN_WAZATYPE1_SIZEY,
					WFLBY_ROOM_BGSPL_WAZATYPE0 + WazaTypeIcon_PlttOffsetGet( bt_type ) );

			GF_BGL_LoadScreenV_Req( p_sys->p_bgl, 
					sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�h�`��I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_End( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_TALKMSG* p_boardwin, WFLBY_GRAPHICCONT* p_sys )
{
	// �����t�E�B���h�E�j��
	if( p_wk->aikotoba_draw == TRUE ){
		p_wk->aikotoba_draw = FALSE;
	}else{

		// �������[�N�j��
		WFLBY_ROOM_UNDERWIN_TrCard_ExitRireki( p_wk, p_sys );
	}
	
	
	// �A�C�R���j��
	WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Exit( p_wk, p_sys );

	// �g���[�i�O���t�B�b�N�j��
	WFLBY_ROOM_UNDERWIN_TrCard_TrView_Exit( p_wk, p_sys );
	
	// �`��X�N���[���N���A
	GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK] );
	GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN_TR] );
	GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�A�C�e����ύX
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�O���t�B�b�N���[�N
 *	@param	p_handle	�A�[�J�C�u�n���h��
 *	@param	heapID		�q�[�vID
 *	@param	itemno		�A�C�e��ID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_ChangeItem( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, WFLBY_ITEMTYPE itemno )
{
	
	// �A�C�R���j��
	WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Exit( p_wk, p_sys );

	// �A�C�e���A�C�R���ݒ�
	WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Init( p_wk, p_sys, p_handle, heapID, 
			itemno, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h	�����t�\�������`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�����t�\����
 *	@retval	FALSE	�����t��\����
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_TrCard_CheckAikotobaDraw( const WFLBY_TR_CARD* cp_wk )
{
	return cp_wk->aikotoba_draw;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e���A�C�R��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�v�h�c
 *	@param	itemno		�A�C�e���m�n
 *	@param	draw_flag	�\�����邩
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Init( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, WFLBY_ITEMTYPE itemno, BOOL draw_flag )
{
	u32 itemtype;
	u32 itemanm;
	GF_ASSERT( itemno < WFLBY_ITEM_NUM );
	GF_ASSERT( p_wk->p_itemicon == NULL );

	// �A�C�e���̃^�C�v
	itemtype	= itemno / WFLBY_ITEM_DANKAI;
	itemanm		= itemno % WFLBY_ITEM_DANKAI;
	
	// itemno�̃A�C�R�����\�[�X��ǂݍ���
	{
		BOOL result;
		// �L�����N�^
		p_wk->p_itemiconres[0] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
				p_sys->p_resman[0], p_handle,
				NARC_wifi_lobby_other_gadget_icon1_NCGR + (itemtype * 3),
				FALSE, WFLBY_TRCARD_ICON_RESID, NNS_G2D_VRAM_TYPE_2DSUB, heapID );
		// VRAM�]��
		result =CLACT_U_CharManagerSetAreaCont( p_wk->p_itemiconres[0] );
		GF_ASSERT( result );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_itemiconres[0] );
		// �p���b�g
		p_wk->p_itemiconres[1] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
				p_sys->p_resman[1], p_handle,
				NARC_wifi_lobby_other_gadget_oam_NCLR, 
				FALSE, WFLBY_TRCARD_ICON_RESID, 
				NNS_G2D_VRAM_TYPE_2DSUB, WFLBY_TRCARD_ICON_PLTTNUM, heapID );
		result =CLACT_U_PlttManagerSetCleanArea( p_wk->p_itemiconres[1] );
		GF_ASSERT( result );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_itemiconres[1] );
		// �Z��	
		p_wk->p_itemiconres[2] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_sys->p_resman[2], p_handle,
				NARC_wifi_lobby_other_gadget_icon1_NCER + (itemtype * 3),
				FALSE, WFLBY_TRCARD_ICON_RESID, CLACT_U_CELL_RES, heapID );
		// �Z���A�j��	
		p_wk->p_itemiconres[3] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_sys->p_resman[3], p_handle,
				NARC_wifi_lobby_other_gadget_icon1_NANR + (itemtype * 3),
				FALSE, WFLBY_TRCARD_ICON_RESID, CLACT_U_CELLANM_RES, heapID );

	}

	// �ǉ�
	{
		CLACT_HEADER header;
		CLACT_ADD_SIMPLE add = {NULL};
		// �w�b�_�[�쐬
		CLACT_U_MakeHeader( &header, WFLBY_TRCARD_ICON_RESID, WFLBY_TRCARD_ICON_RESID,
				WFLBY_TRCARD_ICON_RESID, WFLBY_TRCARD_ICON_RESID,
				CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
				0, WFLBY_TRCARD_ICON_BGPRI,
				p_sys->p_resman[0],
				p_sys->p_resman[1],
				p_sys->p_resman[2],
				p_sys->p_resman[3],
				NULL, NULL );

		add.ClActSet	= p_sys->p_clactset;
		add.ClActHeader = &header;
		add.pri			= WFLBY_TRCARD_ICON_SFPRI;
		add.DrawArea	= NNS_G2D_VRAM_TYPE_2DSUB;
		add.heap		= heapID;
		add.mat.x		= WFLBY_TRCARD_ICON_X;
		add.mat.y		= WFLBY_TRCARD_ICON_Y;
		p_wk->p_itemicon = CLACT_AddSimple( &add );

		// �A�j���ݒ�
		CLACT_AnmChg( p_wk->p_itemicon, itemanm );

		CLACT_SetDrawFlag( p_wk->p_itemicon, draw_flag );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�C�e���A�C�R��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_ItemIcon_Exit( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
	// Icon�j��
	CLACT_Delete( p_wk->p_itemicon );
	p_wk->p_itemicon = NULL;

	// ���\�[�X�j��
	{
		int i;
		
		// VRAM
		CLACT_U_CharManagerDelete( p_wk->p_itemiconres[0] );
		CLACT_U_PlttManagerDelete( p_wk->p_itemiconres[1] );

		// ���\�[�X
		for( i=0; i<WFLBY_ROOM_OAM_RESNUM; i++ ){
			CLACT_U_ResManagerResDelete( p_sys->p_resman[i], p_wk->p_itemiconres[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�����ڕ\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�v
 *	@param	trtype		�g���[�i�^�C�v
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_TrView_Init( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, u32 trtype )
{
	u32 btl_trtype;
	u16 x, y;
	u32 i;
	TR_CLACT_GRA tr_gradata;
	
	GF_ASSERT( p_wk->p_tr == NULL );

	// �g���[�i�^�C�v�O���t�B�b�N�擾
	for( i=0; i<WFLBY_TRCARD_TRTYPE_NUM; i++ ){
		if( sc_WFLBY_TRCARD_TRTYPE[i].fld_trno == trtype ){
			btl_trtype	= sc_WFLBY_TRCARD_TRTYPE[i].btl_trtype;
			x			= sc_WFLBY_TRCARD_TRTYPE[i].x;
			y			= sc_WFLBY_TRCARD_TRTYPE[i].y;
		}
	}
	TrCLACTGraDataGet( btl_trtype, PARA_FRONT, &tr_gradata );
	
	// �g���[�i���\�[�X��ǂݍ���
	{
		BOOL result;
		// �L�����N�^
		p_wk->p_trres[0] = CLACT_U_ResManagerResAddArcChar( 
				p_sys->p_resman[0], tr_gradata.arcID,
				tr_gradata.ncgrID,
				FALSE, WFLBY_TRCARD_TRVIEW_RESID, NNS_G2D_VRAM_TYPE_2DSUB, heapID );
		result = CLACT_U_CharManagerSetAreaCont( p_wk->p_trres[0] );
		GF_ASSERT( result );
		// �p���b�g
		p_wk->p_trres[1] = CLACT_U_ResManagerResAddArcPltt( 
				p_sys->p_resman[1], tr_gradata.arcID,
				tr_gradata.nclrID, 
				FALSE, WFLBY_TRCARD_TRVIEW_RESID, 
				NNS_G2D_VRAM_TYPE_2DSUB, 1, heapID );
		WFLBY_ROOM_UNDERWIN_TrCard_TrView_PallColorChange( CLACT_U_ResManagerGetResObjResPltt( p_wk->p_trres[1] ) );
		result =CLACT_U_PlttManagerSetCleanArea( p_wk->p_trres[1] );
		GF_ASSERT( result );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_trres[1] );
		// �Z��	
		p_wk->p_trres[2] = CLACT_U_ResManagerResAddArcKindCell( 
				p_sys->p_resman[2], tr_gradata.arcID,
				tr_gradata.ncerID,
				FALSE, WFLBY_TRCARD_TRVIEW_RESID, CLACT_U_CELL_RES, heapID );
		// �Z���A�j��	
		p_wk->p_trres[3] = CLACT_U_ResManagerResAddArcKindCell( 
				p_sys->p_resman[3], tr_gradata.arcID,
				tr_gradata.nanrID,
				FALSE, WFLBY_TRCARD_TRVIEW_RESID, CLACT_U_CELLANM_RES, heapID );

	}

	// �ǉ�
	{
		CLACT_HEADER header;
		CLACT_ADD_SIMPLE add = {NULL};
		// �w�b�_�[�쐬
		CLACT_U_MakeHeader( &header, WFLBY_TRCARD_TRVIEW_RESID, WFLBY_TRCARD_TRVIEW_RESID,
				WFLBY_TRCARD_TRVIEW_RESID, WFLBY_TRCARD_TRVIEW_RESID,
				CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
				TRUE, WFLBY_TRCARD_TRVIEW_BGPRI,
				p_sys->p_resman[0],
				p_sys->p_resman[1],
				p_sys->p_resman[2],
				p_sys->p_resman[3],
				NULL, NULL );

		add.ClActSet	= p_sys->p_clactset;
		add.ClActHeader = &header;
		add.pri			= WFLBY_TRCARD_TRVIEW_SFPRI;
		add.DrawArea	= NNS_G2D_VRAM_TYPE_2DSUB;
		add.heap		= heapID;
		add.mat.x		= FX32_CONST( x );
		add.mat.y		= FX32_CONST( y ) + WFLBY_ROOM_CLACT_SUBSURFACE_Y;
		p_wk->p_tr = CLACT_AddSimple( &add );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�����ڔj��
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_TrView_Exit( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
	DelVramTransData( CLACT_ImageProxyGet( p_wk->p_tr ) );
	CLACT_Delete( p_wk->p_tr );
	p_wk->p_tr = NULL;

	// ���\�[�X�j��
	{
		int i;
		
		// VRAM
		CLACT_U_CharManagerDelete( p_wk->p_trres[0] );
		CLACT_U_PlttManagerDelete( p_wk->p_trres[1] );

		// ���\�[�X
		for( i=0; i<WFLBY_ROOM_OAM_RESNUM; i++ ){
			CLACT_U_ResManagerResDelete( p_sys->p_resman[i], p_wk->p_trres[i] );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�̐F��ύX
 *
 *	@param	p_pltt	�p���b�g
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_TrView_PallColorChange( NNSG2dPaletteData* p_pltt )
{
	int i, r, g, b, y;
	int pal_size;
	u16* p_pal;

	// �Z�s�A�J���[�ɂ���
	
	pal_size	= p_pltt->szByte / 2;	// 1�p���b�g�P�ʂ̃T�C�Y�ɂ���
	p_pal		= p_pltt->pRawData;

	for(i = 0; i < pal_size; i++){
		r = ((*p_pal) & 0x1f);
		g = (((*p_pal) >> 5) & 0x1f);
		b = (((*p_pal) >> 10) & 0x1f);

		y = ((r)*76 + (g)*151 + (b)*29) >> 8;

		r = ((y)*256)>>8;
		g = ((y)*200)>>8;
		b = ((y)*128)>>8;

		if(r > 31){ r = 31; }

		*p_pal = (u16)((b<<10)|(g<<5)|r);
		p_pal++;
	}


}


//----------------------------------------------------------------------------
/**
 *	@brief	trainer�J�[�h�E�B���h�E�N���A
 *	
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinClear( WFLBY_TR_CARD* p_wk )
{
	int i;
	for( i=0; i<WFLBY_TRCARD_WIN_NUM; i++ ){
		GF_BGL_BmpWinDataFill( &p_wk->win[i], 0 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�E�B���h�E�̕\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W
 *	@param	winno		�E�B���h�E�i���o�[
 *	@param	strid		���b�Z�[�Wid
 *	@param	x			�����W
 *	@param	y			�����W
 *	@param	col			�J���[
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinPrint( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg, u32 winno, u32 strid, u8 x, u8 y, GF_PRINTCOLOR col )
{
	STRBUF* p_str;
	
	// �\��
	p_str = WFLBY_ROOM_Msg_Get( p_msg, WFLBY_DEFMSG_TYPE_HIROBA, strid );
	GF_STR_PrintColor( &p_wk->win[ winno ], FONT_SYSTEM, p_str, 
			x, y, MSG_NO_PUT, col, NULL );
}
// �E�[�\���o�[�W����
static void WFLBY_ROOM_UNDERWIN_TrCard_WinPrintRightSide( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg, u32 winno, u32 strid, u8 x, u8 y, GF_PRINTCOLOR col )
{
	STRBUF* p_str;
	u32 strsize;
	s32 draw_x;
	
	// �\��
	p_str = WFLBY_ROOM_Msg_Get( p_msg, WFLBY_DEFMSG_TYPE_HIROBA, strid );

	strsize = FontProc_GetPrintStrWidth( FONT_SYSTEM, p_str, 0 );

	draw_x = x - strsize;
	if( draw_x < 0 ){
		draw_x = 0;
	}

	GF_STR_PrintColor( &p_wk->win[ winno ], FONT_SYSTEM, p_str, 
			draw_x, y, MSG_NO_PUT, col, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�J�[�hon
 *	
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 *
 * idx
	WFLBY_TRCARD_WIN_NAME,
	WFLBY_TRCARD_WIN_COUNTRY,
	WFLBY_TRCARD_WIN_TRSTART,
	WFLBY_TRCARD_WIN_LASTACTION,
	WFLBY_TRCARD_WIN_VIPAIKOTOBA,
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinOn( WFLBY_TR_CARD* p_wk, u32 idx )
{
	GF_BGL_BmpWinOnVReq( &p_wk->win[idx] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O�̐ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_mystatus	My�X�e�[�^�X
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetName( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg,const MYSTATUS* cp_mystatus )
{
	WFLBY_ROOM_Msg_SetPlayerName( p_msg, cp_mystatus, 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ƒn����ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetCountry( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg,const WFLBY_USER_PROFILE* cp_profile )
{
	u32 nation;
	u32 area;

	nation = WFLBY_SYSTEM_GetProfileNation( cp_profile );
	area = WFLBY_SYSTEM_GetProfileArea( cp_profile );
	
	WFLBY_ROOM_Msg_SetCountry( p_msg, nation, 0 );
	WFLBY_ROOM_Msg_SetArea( p_msg, nation, area, 1 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ō�ɂ������Ƃ�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	cp_profile	�v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetLastAction( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg,const WFLBY_USER_PROFILE* cp_profile )
{
	u32 action;
//	action = WFLBY_SYSTEM_GetProfileLastAction( cp_profile, 0 );
	action = 0;
	WFLBY_ROOM_Msg_SetMinigame( p_msg, action, 0  );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����t�ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	p_msg		���b�Z�[�W���[�N
 *	@param	cp_aikotoba	�����t�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_WinSetAikotoba( WFLBY_TR_CARD* p_wk, WFLBY_ROOM_DEFMSG* p_msg, const WFLBY_AIKOTOBA_DATA* cp_aikotoba )
{
	int i;
	for( i=0; i<WFLBY_AIKOTOBA_WORD_NUM; i++ ){
		WFLBY_ROOM_Msg_SetPMSWord( p_msg, i, cp_aikotoba->word[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	������\������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�O���t�B�b�N�V�X�e��	
 *	@param	p_handle	�A�[�J�C�u�n���h��
 *	@param	heapID		�q�[�vID
 *	@param	cp_profile	�v���t�B�[��
 *	@param	cp_myprofile�����̃v���t�B�[��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_InitRireki( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID, const WFLBY_USER_PROFILE* cp_profile, const WFLBY_USER_PROFILE* cp_myprofile )
{
	int i;
	ARCHANDLE* p_union_handle;
	ARCHANDLE* p_work_handle;
	BOOL result;
	u32 last_action;
	u32 last_action_userid;
	u32 my_userid;
	u32 my_sex;
	CLACT_ADD_SIMPLE add;
	CLACT_HEADER	 header;
	const WFLBY_TRCARD_RIREKI_CHAR* cp_chardata;
	static const u16 sc_PAL[ WFLBY_TRCARD_RIREKI_RES_NUM ][3] = {
		{ ARC_2DUNIONOBJ_FRONT, NARC_unionobj2d_onlyfront_wf_match_top_trainer_NCLR, 8 },
		{ ARC_WIFILOBBY_OTHER_GRA, NARC_wifi_lobby_other_profile_icon_NCLR, 2 },
		{ ARC_2DUNIONOBJ_FRONT, NARC_unionobj2d_onlyfront_pl_boy01_NCLR, 1 },
		{ ARC_2DUNIONOBJ_FRONT, NARC_unionobj2d_onlyfront_pl_girl01_NCLR, 1 },
	};
	static const u16 sc_CELL[ WFLBY_TRCARD_RIREKI_RES_NUM ] = {
		NARC_unionobj2d_onlyfront_only_front00_NCER,
		NARC_wifi_lobby_other_profile_icon1_NCER,
		NARC_unionobj2d_onlyfront_only_front_boy_NCER,
		NARC_unionobj2d_onlyfront_only_front_girl_NCER,
	};
	static const u16 sc_ANM[ WFLBY_TRCARD_RIREKI_RES_NUM ] = {
		NARC_unionobj2d_onlyfront_only_front00_NANR,
		NARC_wifi_lobby_other_profile_icon1_NANR,
		NARC_unionobj2d_onlyfront_only_front_boy_NANR,
		NARC_unionobj2d_onlyfront_only_front_girl_NANR,
	};


	// ���j�I���I�u�W�F�@�\�����̃A�[�J�C�u�n���h���I�[�v��
	p_union_handle = ArchiveDataHandleOpen( ARC_2DUNIONOBJ_FRONT, heapID );


	// �����̃��[�UIDX�擾
	my_userid	= WFLBY_SYSTEM_GetProfileUserID( cp_myprofile );
	my_sex		= WFLBY_SYSTEM_GetProfileSex( cp_myprofile );
 

	// �p���b�g�ƃZ���f�[�^�͑S���ǂݍ���
	for( i=0; i<WFLBY_TRCARD_RIREKI_RES_NUM; i++ ){

		// WFLBY_TRCARD_RIREKI_RES_HEROBOY��WFLBY_TRCARD_RIREKI_RES_HEROGIRL��
		// �����̐��ʂœǂݍ��ނ����߂�
		if( i == WFLBY_TRCARD_RIREKI_RES_HEROBOY ){
			if( my_sex == PM_FEMALE ){
				p_wk->p_rirekires_pal[i] = NULL;
				p_wk->p_rirekires_cell[i] = NULL;
				p_wk->p_rirekires_anm[i] = NULL;
				continue;
			}
		}
		else if( i == WFLBY_TRCARD_RIREKI_RES_HEROGIRL ){
			if( my_sex == PM_MALE ){
				p_wk->p_rirekires_pal[i] = NULL;
				p_wk->p_rirekires_cell[i] = NULL;
				p_wk->p_rirekires_anm[i] = NULL;
				continue;
			}
		}

		if( sc_PAL[i][0] == ARC_2DUNIONOBJ_FRONT ){
			p_work_handle = p_union_handle;
		}else{
			p_work_handle = p_handle;
		}

		p_wk->p_rirekires_pal[i] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
				p_sys->p_resman[1], p_work_handle,
				sc_PAL[i][1], 
				FALSE, WFLBY_TRCARD_RIREKI_RESID+i, 
				NNS_G2D_VRAM_TYPE_2DSUB, sc_PAL[i][2], heapID );
		result =CLACT_U_PlttManagerSetCleanArea( p_wk->p_rirekires_pal[i] );
		GF_ASSERT( result );
		CLACT_U_ResManagerResOnlyDelete( p_wk->p_rirekires_pal[i] );


		// �Z��	
		p_wk->p_rirekires_cell[i] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_sys->p_resman[2], p_work_handle,
				sc_CELL[i],
				FALSE, WFLBY_TRCARD_RIREKI_RESID+i, CLACT_U_CELL_RES, heapID );
		// �Z���A�j��	
		p_wk->p_rirekires_anm[i] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
				p_sys->p_resman[3], p_work_handle,
				sc_ANM[i],
				FALSE, WFLBY_TRCARD_RIREKI_RESID+i, CLACT_U_CELLANM_RES, heapID );
	}

	// �L�����N�^�ǂݍ���
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){
		
		last_action = WFLBY_SYSTEM_GetProfileLastAction( cp_profile, i );
		last_action_userid = WFLBY_SYSTEM_GetProfileLastActionUserID( cp_profile, i );

		if( last_action == WFLBY_LASTACTION_MAX ){

			p_wk->p_rirekires_cg[i] = NULL;
			p_wk->p_rireki[i] = NULL;
		}else{

			// �����L�����N�^�f�[�^���擾
			cp_chardata = WFLBY_ROOM_UNDERWIN_TrCard_GetRirekiData( last_action, last_action_userid, my_userid, my_sex );

			if( cp_chardata->arcid == WFLBY_TRCARD_RIREKI_RES_ICON ){
				p_work_handle = p_handle;
			}else{
				p_work_handle = p_union_handle;
			}

			// �L�����N�^
			p_wk->p_rirekires_cg[i] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
					p_sys->p_resman[0], p_work_handle,
					cp_chardata->dataid,
					FALSE, WFLBY_TRCARD_RIREKI_RESID+i, NNS_G2D_VRAM_TYPE_2DSUB, heapID );
			// VRAM�]��
			result =CLACT_U_CharManagerSetAreaCont( p_wk->p_rirekires_cg[i] );
			GF_ASSERT( result );
			CLACT_U_ResManagerResOnlyDelete( p_wk->p_rirekires_cg[i] );

			// �A�N�^�[�쐬
			CLACT_U_MakeHeader( &header, WFLBY_TRCARD_RIREKI_RESID+i, 
					WFLBY_TRCARD_RIREKI_RESID + cp_chardata->arcid,
					WFLBY_TRCARD_RIREKI_RESID + cp_chardata->arcid,
					WFLBY_TRCARD_RIREKI_RESID + cp_chardata->arcid,
					CLACT_U_HEADER_DATA_NONE, CLACT_U_HEADER_DATA_NONE,
					0, WFLBY_TRCARD_RIREKI_BGPRI,
					p_sys->p_resman[0],
					p_sys->p_resman[1],
					p_sys->p_resman[2],
					p_sys->p_resman[3],
					NULL, NULL );

			add.ClActSet	= p_sys->p_clactset;
			add.ClActHeader = &header;
			add.pri			= 0;
			add.DrawArea	= NNS_G2D_VRAM_TYPE_2DSUB;
			add.heap		= heapID;
			add.mat			= sc_WFLBY_TRCARD_RIREKI_POS[i];
			add.mat.x		+= FX32_CONST(cp_chardata->ofs_x);
			add.mat.y		+= FX32_CONST(cp_chardata->ofs_y);

			p_wk->p_rireki[i] = CLACT_AddSimple( &add );

			// �p���b�g�I�t�Z�b�g�ݒ�
			CLACT_PaletteOffsetChgAddTransPlttNo( p_wk->p_rireki[i], cp_chardata->paloffs );
		}
	}

	ArchiveDataHandleClose( p_union_handle );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	����\��OFF
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_TrCard_ExitRireki( WFLBY_TR_CARD* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
	int i;
	
	// �A�N�^�[�j���{�L�����N�^VRAM	
	for( i=0; i<WFLBY_LASTACT_BUFFNUM; i++ ){
		if( p_wk->p_rirekires_cg[i] != NULL ){
			CLACT_Delete( p_wk->p_rireki[i] );
			p_wk->p_rireki[i] = NULL;
			
			CLACT_U_CharManagerDelete( p_wk->p_rirekires_cg[i] );
			CLACT_U_ResManagerResDelete( p_sys->p_resman[0], p_wk->p_rirekires_cg[i] );
			p_wk->p_rirekires_cg[i] = NULL;
		}
	}
	// �p���b�g �Z���@�A�j��
	for( i=0; i<WFLBY_TRCARD_RIREKI_RES_NUM; i++ ){
		if( p_wk->p_rirekires_pal[i] != NULL ){
			CLACT_U_PlttManagerDelete( p_wk->p_rirekires_pal[i] );
			CLACT_U_ResManagerResDelete( p_sys->p_resman[1], p_wk->p_rirekires_pal[i] );
			CLACT_U_ResManagerResDelete( p_sys->p_resman[2], p_wk->p_rirekires_cell[i] );
			CLACT_U_ResManagerResDelete( p_sys->p_resman[3], p_wk->p_rirekires_anm[i] );
			p_wk->p_rirekires_pal[i] = NULL;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����œǂݍ��ރL�����N�^�f�[�^���擾����
 *
 *	@param	last_act	�A�N�V�������e
 *	@param	userid		���̃��[�UID
 *	@param	myid		������ID
 */
//-----------------------------------------------------------------------------
static const WFLBY_TRCARD_RIREKI_CHAR* WFLBY_ROOM_UNDERWIN_TrCard_GetRirekiData( WFLBY_LASTACTION_TYPE last_act, s32 userid, s32 myid, u32 sex )
{
	// �����Ɠ����l�Ȃ玩���̊G���o��
	if( userid == myid ){
		return &sc_WFLBY_TRCARD_RIREKI_HERO_CHAR[ sex ];
	}
	return &sc_WFLBY_TRCARD_RIREKI_CHAR[ last_act ];
}



//----------------------------------------------------------------------------
/**
 *	@brief	�������	�K�W�F�b�g�{�^��	������
 *
 *	@param	p_wk			���[�N
 *	@param	p_sys			�`��V�X�e��
 *	@param	p_handle		�n���h��
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_Init( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, ARCHANDLE* p_handle, u32 heapID )
{
	int i;

	// �r�b�g�}�b�v�E�B���h�E������
	GF_BGL_BmpWinAddEx( p_sys->p_bgl, &p_wk->win, &sc_WFLBY_BTTN_WIN_DATA );
	
	// �e�X�N���[���f�[�^��ǂݍ���
	for( i=0; i<WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM; i++ ){
		p_wk->p_scrnbuff[i] = ArcUtil_HDL_ScrnDataGet( p_handle, 
				NARC_wifi_lobby_other_gadget_but0_n_NSCR+i, FALSE,
				&p_wk->p_scrn[i], heapID );
	}

	// �{�^���}�l�[�W���쐬
	{
		static const TP_HIT_TBL sc_HitTbl = { 32, 160, 40, 216 };

		p_wk->p_bttnman = BMN_Create( &sc_HitTbl, 1, 
				WFLBY_ROOM_UNDERWIN_Button_BttnCallBack, p_wk, heapID );
	}

	p_wk->bttnstop = TRUE;	// �ŏ��͔����Ȃ�
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���j������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_Exit( WFLBY_GADGET_BTTN* p_wk )
{
	int i;

	// �{�^���}�l�[�W���j��
	BMN_Delete( p_wk->p_bttnman );	

	//  �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->win );
	
	// �S���\�[�X�j��
	for( i=0; i<WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM; i++ ){
		sys_FreeMemoryEz( p_wk->p_scrnbuff[i] );
	}
	p_wk->seq = WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�{�^��	���C������
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e��
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�K�W�F�b�g�N��
 *	@retval	FALSE	�K�W�F�b�g�N���Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFLBY_ROOM_UNDERWIN_Button_Main( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys,  ARCHANDLE* p_handle, u32 heapID )
{
	BOOL ret = FALSE;

	// �K�W�F�b�g��������̂Ƃ������{�^���}�l�[�W��
	if( p_wk->bttnstop == FALSE ){
		BMN_Main( p_wk->p_bttnman );
	}else{
		p_wk->bttnevent = BMN_EVENT_RELEASE;
	}
	

	// �V�[�P���X�Ǘ�
	if( p_wk->seq == WFLBY_ROOM_UNDERWIN_BTTN_STATUS_WAIT ){
		if( p_wk->lock_wait == FALSE ){
			p_wk->seq = WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON;
			WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_UP, p_handle, heapID );
		}
	}

	// �C�x���g�Ƃ��ς�����I
	if( p_wk->bttnevent != p_wk->lastbttnevent ){
		p_wk->lastbttnevent = p_wk->bttnevent;

		// �G��ύX
		if( p_wk->bttnevent == BMN_EVENT_HOLD ){
			WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_DOWN, p_handle, heapID );
		}else{
			// �����Ă����Ƃ��Ƃ��߂ȂƂ��ŏ����O���t�B�b�N��ς���
			if( p_wk->seq == WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON ){
				WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_UP, p_handle, heapID );
			}else{
				WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_CENTER, p_handle, heapID );
			}
		}

		// �{�^���N���`�F�b�N
		if( (p_wk->seq == WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON) &&
			(p_wk->bttnevent == BMN_EVENT_HOLD) ){
			ret = TRUE;
			p_wk->seq		= WFLBY_ROOM_UNDERWIN_BTTN_STATUS_WAIT;
			p_wk->lock_wait	= TRUE;
		}
	}

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�{�^��	�J�n
 *
 *	@param	p_wk			���[�N
 *	@param	p_sys			�`��V�X�e��
 *	@param	p_msg			���b�Z�[�W
 *	@param	cp_profile		�v���t�B�[��
 *	@param	p_handle		�n���h��
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_Start( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, WFLBY_ROOM_DEFMSG* p_msg, const WFLBY_USER_PROFILE* cp_profile, ARCHANDLE* p_handle, u32 heapID )
{
	// �o�b�N�O���E���h�ݒ�
	{
		ArcUtil_HDL_ScrnSet( p_handle, NARC_wifi_lobby_other_gadget_bg_NSCR,
				p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK],
				0, 0, FALSE, heapID );
	}

	// ���샂�[�h�ݒ�
	p_wk->mode = WFLBY_ROOM_UNDERWIN_BTTN_MODE_GADGET;

	// ��ɏo�����W�F�b�g�̊G���o��
	{

		p_wk->bttn_itemno = WFLBY_SYSTEM_GetProfileItem( cp_profile );
		ArcUtil_HDL_BgCharSet( p_handle, NARC_wifi_lobby_other_gadget_1a_NCGR+p_wk->bttn_itemno,
				p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
				WFLBY_UNDERWIN_BGCG_END, 0, FALSE, heapID );

		WFLBY_ROOM_UNDERWIN_Common_LoadScrn( p_sys, p_handle, 
				NARC_wifi_lobby_other_gadget_1a_NSCR+p_wk->bttn_itemno, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
				WFLBY_UNDERWIN_BGCG_END, heapID );
	}

	// �^�C�g���\��
	{	
		STRBUF* p_str;
		
		// �\��
		p_str = WFLBY_ROOM_Msg_Get( p_msg, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_profile_14 );
		GF_BGL_BmpWinDataFill( &p_wk->win, 0 );
		GF_STR_PrintColor( &p_wk->win, FONT_SYSTEM, p_str, 
				0, 4, MSG_ALLPUT, GF_PRINTCOLOR_MAKE( 15, 14, 0 ), NULL );
	}


	// �����Ă����Ƃ��Ƃ��߂ȂƂ��ŏ����O���t�B�b�N��ς���
	if( p_wk->seq == WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON ){
		WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_UP, p_handle, heapID );
	}else{
		WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_CENTER, p_handle, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���J�n	�t���[�g�p
 *
 *	@param	p_wk			���[�N
 *	@param	p_sys			�`��V�X�e��
 *	@param	idx				�t���[�g��荞�݈ʒu�C���f�b�N�X�i�O�`�Q�j
 *	@param	p_handle		�A�[�J�C�u�n���h��
 *	@param	heapID			�q�[�v�h�c
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_StartFloat( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 idx, ARCHANDLE* p_handle, u32 heapID )
{
	GF_ASSERT( idx < WFLBY_FLOAT_ON_NUM );
	
	// �o�b�N�O���E���h�ݒ�
	{
		ArcUtil_HDL_ScrnSet( p_handle, NARC_wifi_lobby_other_gadget_bg_NSCR,
				p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK],
				0, 0, FALSE, heapID );
	}

	// ���샂�[�h�ݒ�
	p_wk->mode			= WFLBY_ROOM_UNDERWIN_BTTN_MODE_FLOAT;
	p_wk->bttn_itemno	= idx;

	// ��ɏo���G���o��
	ArcUtil_HDL_BgCharSet( p_handle, NARC_wifi_lobby_other_float_a_NCGR+p_wk->bttn_itemno,
			p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
			WFLBY_UNDERWIN_BGCG_END, 0, FALSE, heapID );


	// �����Ă����Ƃ��Ƃ��߂ȂƂ��ŏ����O���t�B�b�N��ς���
	if( p_wk->seq == WFLBY_ROOM_UNDERWIN_BTTN_STATUS_ON ){
		WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_UP, p_handle, heapID );
	}else{
		WFLBY_ROOM_UNDERWIN_Button_DrawButton( p_wk, p_sys, WFLBY_ROOM_UNDERWIN_BTTN_ANM_CENTER, p_handle, heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�K�W�F�b�g�{�^��	�J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_End( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys )
{
	// �X�N���[���N���A
	GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BACK] );
	GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN_TR] );
	GF_BGL_ScrClear( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG] );

	// �X�N���[�����W�ݒ�
	GF_BGL_ScrollSet( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
			GF_BGL_SCROLL_Y_SET, 0 );
}



//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���`��	
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�`��V�X�e��
 *	@param	idx			�C���f�b�N�X
 *	@param	p_handle	�n���h��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_DrawButton( WFLBY_GADGET_BTTN* p_wk, WFLBY_GRAPHICCONT* p_sys, u32 idx, ARCHANDLE* p_handle, u32 heapID )
{
	static const s8 sc_BUTTON_Y[ WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM ] = {
		0, 9, 6
	};
/*
	GF_BGL_ScrWrite( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN_TR],
			(const void*)p_wk->p_scrn[idx]->rawData, 0, 0, 32, 24 );//*/

	GF_BGL_ScrWriteExpand(
				p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN_TR],
				0, 3, 32, 21,
				(const void*)p_wk->p_scrn[idx]->rawData, 0, 3, 32, 32 );
	

	GF_BGL_LoadScreenV_Req( p_sys->p_bgl , sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN_TR] );


	// �t���[�g���[�h�Ȃ�t���[���O���t�B�b�N���ύX
	if( p_wk->mode == WFLBY_ROOM_UNDERWIN_BTTN_MODE_FLOAT ){
		static const u8 sc_FLOAT_BTTN_IDX[ WFLBY_ROOM_UNDERWIN_BTTN_ANM_NUM ] = {
			0, 1, 0
		};
		u32 scrn_idx;
		
		// �t���[�g�{�^���̂Ƃ��̓X�N���[�����]��
		scrn_idx = (p_wk->bttn_itemno*WFLBY_ROOM_UNDERWIN_BTTN_FLOAT_FRAME_NUM)+sc_FLOAT_BTTN_IDX[ idx ];
		WFLBY_ROOM_UNDERWIN_Common_LoadScrn( p_sys, p_handle, 
				NARC_wifi_lobby_other_float_a1_NSCR+scrn_idx, 
				sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
				WFLBY_UNDERWIN_BGCG_END, heapID );
	}

	// idx�ɂ���āBsc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG]�ʂ�
	// Y���W�𒲐�
	GF_BGL_ScrollReq( p_sys->p_bgl, sc_WFLBY_ROOM_BGCNT_FRM[WFLBY_ROOM_BGCNT_SUB_BTTN2_MSG],
			GF_BGL_SCROLL_Y_SET, sc_BUTTON_Y[idx] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���R�[���o�b�N
 *
 *	@param	bttnno		�{�^���i���o�[
 *	@param	event		�C�x���g
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_BttnCallBack( u32 bttnno, u32 event, void* p_work )
{
	WFLBY_GADGET_BTTN* p_wk = p_work;

	p_wk->bttnevent = event;

	
	// �u�Ԃ̃f�[�^�͂���Ȃ�
	if( p_wk->bttnevent == BMN_EVENT_TOUCH ){
		p_wk->bttnevent = BMN_EVENT_HOLD;
	}else{
		// ��������ނ�����Ȃ�
		if( p_wk->bttnevent == BMN_EVENT_SLIDEOUT ){
			p_wk->bttnevent = BMN_EVENT_RELEASE;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�^���@���b�N�E�G�C�g���N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_ROOM_UNDERWIN_Button_BttnLockClear( WFLBY_GADGET_BTTN* p_wk )
{
	p_wk->lock_wait = FALSE;
}

