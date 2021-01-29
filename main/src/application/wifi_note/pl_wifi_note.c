//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		pl_wifi_note.c
 *	@brief		�F�B�蒠	�v���`�i�o�[�W����
 *	@author		tomoya takahshi
 *	@data		2007.07.23
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "dwc.h"

#include "gflib/strbuf.h"
#include "gflib/strbuf_family.h"

#include "system/wordset.h"
#include "system/msgdata.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/wipe.h"
#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "system/pm_overlay.h"
#include "system/gra_tool.h"
#include "system/msgdata_util.h"

#if PL_T0861_080712_FIX
#include "system/pmfprint.h"
#endif

#include "savedata/config.h"
#include "savedata/wifilist.h"
#include "savedata/frontier_savedata.h"
#include "savedata/factory_savedata.h"
#include "savedata/stage_savedata.h"
#include "savedata/castle_savedata.h"
#include "savedata/roulette_savedata.h"
#include "savedata/b_tower.h"

#include "wifi/dwc_rapfriend.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_note.h"

#include "application/pl_wifi_note.h"
#include "application/codein/codein.h"
#include "application/namein.h"
#include "application/wifi_2dmap/wifi_2dchar.h"
#include "application/wifi_lobby/wflby_def.h"

#include "itemtool/itemsym.h"
#include "itemtool/myitem.h"

#include "frontier/factory_def.h"

#include "field/union_beacon_tool.h"
#include "field/sysflag.h"

#include "pl_wifinote.naix"
#include "wifi_note_snd.h"
#include "savedata/frontier_savedata.h"

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
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------


// WIFI2DMAP�V�X�e���I�[�o�[���C
FS_EXTERN_OVERLAY(wifi_2dmapsys);

//-------------------------------------
///	������o�b�t�@�T�C�Y
//=====================================
#define WFNOTE_STRBUF_SIZE	(128)

//-------------------------------------
///	CLACT
//=====================================
#define CLACT_RESOURCE_NUM		( 4 )	// ���\�[�X�}�l�[�W����
#define CLACT_WKNUM				( 64 )	// ���[�N��
#define CLACT_RESNUM			( 64 )	// �ǂݍ��ރ��\�[�X��
#define CLACT_MAIN2_REND_Y_S32	( 256 )	// �X�N���[�������Ȃ��T�[�t�F�[�X�ʂ�Y���W�ʒu
#define CLACT_MAIN2_REND_Y		( FX32_CONST(256) )	// �X�N���[�������Ȃ��T�[�t�F�[�X�ʂ�Y���W�ʒu
#define CLACT_MAIN_VRAM_SIZ		( 0x14000 )
#define CLACT_SUB_VRAM_SIZ		( 0 )

//-------------------------------------
///	VRAMTRANFER_MAN
//=====================================
#define VRAMTR_MAN_NUM		(16)

//-------------------------------------
///	BG
//=====================================
enum{	// BG���ʒ萔
	WFNOTE_BG_MAIN_BACK,
	WFNOTE_BG_MAIN_MSG,
	WFNOTE_BG_MAIN_SCROLL,
	WFNOTE_BG_MAIN_SCRMSG,
	WFNOTE_BG_SUB_BACK,
	WFNOTE_BG_NUM,
};
enum{	// �p���b�g�e�[�u��
	// ���C��
	WFNOTE_BGPL_MAIN_BACK_0 = 0,
	WFNOTE_BGPL_MAIN_BACK_1,
	WFNOTE_BGPL_MAIN_BACK_2,
	WFNOTE_BGPL_MAIN_BACK_3,
	WFNOTE_BGPL_MAIN_BACK_4,
	WFNOTE_BGPL_MAIN_BACK_5,
	WFNOTE_BGPL_MAIN_BACK_6,
	WFNOTE_BGPL_MAIN_MSGFONT,
	WFNOTE_BGPL_MAIN_TALKWIN,
	WFNOTE_BGPL_MAIN_MENUWIN,
	WFNOTE_BGPL_MAIN_TRGRA,

	// �T�u
	WFNOTE_BGPL_SUB_BACK_0 = 0,
	WFNOTE_BGPL_SUB_BACK_1,
	WFNOTE_BGPL_SUB_BACK_2,
	WFNOTE_BGPL_SUB_BACK_3,
	WFNOTE_BGPL_SUB_BACK_4,
};
#define WFNOTE_BGPL_MAIN_BACK_NUM	((WFNOTE_BGPL_MAIN_BACK_6+1) - WFNOTE_BGPL_MAIN_BACK_0)
#define WFNOTE_BGPL_SUB_BACK_NUM	((WFNOTE_BGPL_SUB_BACK_4+1) - WFNOTE_BGPL_SUB_BACK_0)
// ��{CGX	MAIN_MSG�ʂɓǂݍ��܂�܂�
#define WFNOTE_BGCGX_MAIN_MSG_MENU	(1)
#define WFNOTE_BGCGX_MAIN_MSG_TALK	(WFNOTE_BGCGX_MAIN_MSG_MENU+MENU_WIN_CGX_SIZ)
#define WFNOTE_BGCGX_MAIN_MSG_TALKEND	(WFNOTE_BGCGX_MAIN_MSG_TALK+TALK_WIN_CGX_SIZ)

// ��{�^�C�g���r�b�g�}�b�v
#define WFNOTE_BG_TITLEBMP_X	( 1 )
#define WFNOTE_BG_TITLEBMP_Y	( 0 )
#define WFNOTE_BG_TITLEBMP_SIZX	( 24 )
#define WFNOTE_BG_TITLEBMP_SIZY	( 3 )
#define WFNOTE_BG_TITLEBMP_CGX	( WFNOTE_BGCGX_MAIN_MSG_TALKEND )
#define WFNOTE_BG_TITLEBMP_CGX_END	( WFNOTE_BG_TITLEBMP_CGX + (WFNOTE_BG_TITLEBMP_SIZX*WFNOTE_BG_TITLEBMP_SIZY) )
#define WFNOTE_BG_TITLEBMP_PL_Y	( 4 )

// ��{OAM�̃f�[�^
#define WFNOTE_OAM_COMM_CONTID	( 100 )	// ��{�IOAM�̊Ǘ�ID

// FONT�J���[
#define	WFNOTE_COL_BLACK	( GF_PRINTCOLOR_MAKE( 1, 2, 0 ) )		// �t�H���g�J���[�F��
#define	WFNOTE_COL_WHITE	( GF_PRINTCOLOR_MAKE( 15, 2, 0 ) )		// �t�H���g�J���[�F��
#define	WFNOTE_COL_RED	    ( GF_PRINTCOLOR_MAKE( 3, 4, 0 ) )		// �t�H���g�J���[�F��
#define	WFNOTE_COL_BLUE	    ( GF_PRINTCOLOR_MAKE( 5, 6, 0 ) )		// �t�H���g�J���[�F��

// �`��G���A��
enum{	
	WFNOTE_DRAWAREA_MAIN,
	WFNOTE_DRAWAREA_RIGHT,
	WFNOTE_DRAWAREA_LEFT,
	WFNOTE_DRAWAREA_NUM,
};

//  ��{�X�N���[���f�[�^
enum{
	WFNOTE_SCRNDATA_BACK,	// �w�i�p
	WFNOTE_SCRNDATA_KIT,		// �������ݗp
	WFNOTE_SCRNDATA_NUM,		// 
};	

//-------------------------------------
///	STATUS
//=====================================
enum {
	WFNOTE_STATUS_MODE_SELECT,	// �ŏ��̑I�����
	WFNOTE_STATUS_FRIENDLIST,	// �F�B�f�[�^�\��
	WFNOTE_STATUS_CODEIN,		// �F�B�R�[�h����
	WFNOTE_STATUS_MYCODE,		// �����̗F�B�R�[�h�m�F
	WFNOTE_STATUS_FRIENDINFO,	// �F�B�ڍ׃f�[�^
	WFNOTE_STATUS_END,			// �I��
	WFNOTE_STATUS_NUM
} ;
typedef enum{
	WFNOTE_STRET_CONTINUE,	// �Â�
	WFNOTE_STRET_FINISH,	// ��Ƃ͏I�����
} WFNOTE_STRET;
#define WFNOTE_STATUS_CHANGE_WAIT	(4)	// �ύX���E�G�C�g

//-------------------------------------
///	PROC SEQ
//=====================================
enum {
	WFNOTE_PROCSEQ_MAIN,
	WFNOTE_PROCSEQ_WAIT,
} ;

//-------------------------------------
///	�F�B�f�[�^�V�K�ݒ�֐��@�߂�l
//=====================================
enum{
	WFNOTE_DATA_NEWFRIENDSET_OK,	// �ݒ�ł���
	WFNOTE_DATA_NEWFRIENDSET_FRIENDKEYNG,	// �F�B�R�[�h���s��
	WFNOTE_DATA_NEWFRIENDSET_SETING,	// �����ݒ肵�Ă�����
	WFNOTE_DATA_NEWFRIENDSET_NUM

};

// ���f�[�^
#define WFNOTE_YAZIRUSHINUM		(2)	// ���̐�
#define WFNOTE_YAZIRUSHI_XLEF	(FX32_CONST(6))
#define WFNOTE_YAZIRUSHI_XRIGHT	(FX32_CONST(249))
#define WFNOTE_YAZIRUSHI_Y		(FX32_CONST(98))
#define WFNOTE_YAZIRUSHI_PRI		( 8 )

// �^�C�g�����b�Z�[�W�X�s�[�h
#define WFNOTE_TITLE_MSG_SPEED	( 2 ) 



//-------------------------------------
///	MODESELECT
//=====================================
// BMP
#define WFNOTE_MODESELECT_MSG_X		( 4 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MODESELECT_MSG_Y		( 4 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MODESELECT_MSG_SIZX	( 26 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MODESELECT_MSG_SIZY	( 20 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MODESELECT_CGX		( 1 )	// �L�����N�^�I�t�Z�b�g

#define WFNOTE_MODESELECT_TALK_X		( 2 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MODESELECT_TALK_Y		( 19 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MODESELECT_TALK_SIZX		( 27 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MODESELECT_TALK_SIZY		( 4 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MODESELECT_TALKCGX		( WFNOTE_BG_TITLEBMP_CGX_END )	// �L�����N�^�I�t�Z�b�g

#define WFNOTE_MODESELECT_YESNO_FRM		( GF_BGL_FRAME1_M )
#define WFNOTE_MODESELECT_YESNO_X		( 25 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MODESELECT_YESNO_Y		( 13 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MODESELECT_YESNO_SIZX		( 6 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MODESELECT_YESNO_SIZY		( 4 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MODESELECT_YESNOCGX		( WFNOTE_MODESELECT_TALKCGX+(WFNOTE_MODESELECT_TALK_SIZX*WFNOTE_MODESELECT_TALK_SIZY) )	// �L�����N�^�I�t�Z�b�g

// ����V�[�P���X
enum{
	WFNOTE_MODESELECT_SEQ_INIT,
	WFNOTE_MODESELECT_SEQ_FADEINWAIT,
	WFNOTE_MODESELECT_SEQ_INIT_NOFADE,
	WFNOTE_MODESELECT_SEQ_MAIN,
	WFNOTE_MODESELECT_SEQ_FADEOUT,
	WFNOTE_MODESELECT_SEQ_FADEOUTWAIT,

	// �R�[�h���͂���߂��Ă����Ƃ��p
	WFNOTE_MODESELECT_SEQ_CODECHECK_INIT,
	WFNOTE_MODESELECT_SEQ_CODECHECK_FADEINWAIT,
	WFNOTE_MODESELECT_SEQ_CODECHECK_MSGWAIT,		//�o�^�m�F���b�Z�[�WON
	WFNOTE_MODESELECT_SEQ_CODECHECK_MSGYESNOWAIT,	//�͂��������I��҂�
	WFNOTE_MODESELECT_SEQ_CODECHECK_RESLUTMSGWAIT,	//�I�����ʕ\��
};
// �J�[�\��
enum{
	WFNOTE_MODESELECT_CURSOR_FRIENDLIST,
	WFNOTE_MODESELECT_CURSOR_CODEIN,
	WFNOTE_MODESELECT_CURSOR_MYCODE,
	WFNOTE_MODESELECT_CURSOR_END,
	WFNOTE_MODESELECT_CURSOR_NUM,
};
#define WFNOTE_MODESELECT_CURSOR_CSIZX	( 28 )	// �J�[�\�������̉��T�C�Y�i�L�����j
#define WFNOTE_MODESELECT_CURSOR_CSIZY	( 4 )	// �J�[�\�������̏c�T�C�Y�i�L�����j
#define WFNOTE_MODESELECT_CURSOR_PAL_ON	( WFNOTE_BGPL_SUB_BACK_1 )	// �I����
#define WFNOTE_MODESELECT_CURSOR_PAL_OFF ( WFNOTE_BGPL_SUB_BACK_0 )	// ��I����


//-------------------------------------
///	FRIENDLIST
//=====================================
#define WFNOTE_FRIENDLIST_PAGEFRIEND_NUM	(8)	// 1�y�[�W�ɂ���l�̐�
#define WFNOTE_FRIENDLIST_PAGEFRIEND_HALF	(WFNOTE_FRIENDLIST_PAGEFRIEND_NUM/2)	// 1�y�[�W�ɂ���l�̐�
#define WFNOTE_FRIENDLIST_PAGE_NUM			(4)	// �y�[�W����
#define WFNOTE_FRIENDLIST_FRIENDMAX			( WFNOTE_FRIENDLIST_PAGEFRIEND_NUM*WFNOTE_FRIENDLIST_PAGE_NUM )	// �\���ł���F�B�̍ő�l
#define WFNOTE_FRIENDLIST_2DCHAR_NUM		(16)// 2dchar���[�N��
enum{	// �J�[�\���f�[�^����
	WFNOTE_FRIENDLIST_CURSORDATA_0,
	WFNOTE_FRIENDLIST_CURSORDATA_1,
	WFNOTE_FRIENDLIST_CURSORDATA_2,
	WFNOTE_FRIENDLIST_CURSORDATA_3,
	WFNOTE_FRIENDLIST_CURSORDATA_4,
	WFNOTE_FRIENDLIST_CURSORDATA_5,
	WFNOTE_FRIENDLIST_CURSORDATA_6,
	WFNOTE_FRIENDLIST_CURSORDATA_7,
	WFNOTE_FRIENDLIST_CURSORDATA_BACK,
	WFNOTE_FRIENDLIST_CURSORDATA_NUM,
	WFNOTE_FRIENDLIST_CURSORDATA_OFF,	// �J�[�\���������Ƃ���POS
};

// �r�b�g�}�b�v
#define WFNOTE_FRIENDLIST_TEXT_OFSX	( 4 )	// �J�n�ʒu��
#define WFNOTE_FRIENDLIST_TEXT_OFSY	( 4 )	// �J�n�ʒu��
#define WFNOTE_FRIENDLIST_TEXT_SIZX	( 26 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_TEXT_SIZY	( 17 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_TEXT_CGX0	( 1 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_TEXT_CGX1	( WFNOTE_FRIENDLIST_TEXT_CGX0+(WFNOTE_FRIENDLIST_TEXT_SIZX*WFNOTE_FRIENDLIST_TEXT_SIZY) )	// �T�C�Y��

#define WFNOTE_FRIENDLIST_BACKMSG_X	( 15 )	// �J�n�ʒu��
#define WFNOTE_FRIENDLIST_BACKMSG_Y	( 21 )	// �J�n�ʒu��
#define WFNOTE_FRIENDLIST_BACKMSG_SIZX	( 8 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_BACKMSG_SIZY	( 3 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_BACKMSG_CGX	( WFNOTE_BG_TITLEBMP_CGX_END )

#define WFNOTE_FRIENDLIST_MENU_X	( 16 )	// �J�n�ʒu��
#define WFNOTE_FRIENDLIST_MENU_Y	( 9 )	// �J�n�ʒu��
#define WFNOTE_FRIENDLIST_MENU_SIZX	( 15 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_MENU_SIZY	( 8 )	// �T�C�Y��
#define WFNOTE_FRIENDLIST_MENU_CGX	( WFNOTE_FRIENDLIST_BACKMSG_CGX+(WFNOTE_FRIENDLIST_BACKMSG_SIZX*WFNOTE_FRIENDLIST_BACKMSG_SIZY) )

#define WFNOTE_FRIENDLIST_TALK_X		( 2 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_TALK_Y		( 19 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_TALK_SIZX		( 27 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_TALK_SIZY		( 4 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_TALK_CGX		( WFNOTE_FRIENDLIST_MENU_CGX+(WFNOTE_FRIENDLIST_MENU_SIZX*WFNOTE_FRIENDLIST_MENU_SIZY) )	// �L�����N�^�I�t�Z�b�g

#define WFNOTE_FRIENDLIST_YESNO_FRM		( GF_BGL_FRAME1_M )
#define WFNOTE_FRIENDLIST_YESNO_X		( 25 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_YESNO_Y		( 13 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_YESNO_SIZX		( 6 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_YESNO_SIZY		( 4 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_FRIENDLIST_YESNOCGX		( WFNOTE_FRIENDLIST_TALK_CGX+(WFNOTE_FRIENDLIST_TALK_SIZX*WFNOTE_FRIENDLIST_TALK_SIZY) )	// �L�����N�^�I�t�Z�b�g
	

// ����V�[�P���X
enum{
	WFNOTE_FRIENDLIST_SEQ_INIT_PAGEINIT,
	WFNOTE_FRIENDLIST_SEQ_INIT,
	WFNOTE_FRIENDLIST_SEQ_MAIN,
	WFNOTE_FRIENDLIST_SEQ_SCRLLINIT,
	WFNOTE_FRIENDLIST_SEQ_SCRLL,

	WFNOTE_FRIENDLIST_SEQ_MENUINIT,	//�@���j���[�I��
	WFNOTE_FRIENDLIST_SEQ_MENUWAIT,	// ���j���[�I��҂�
	WFNOTE_FRIENDLIST_SEQ_INFOINIT,		// �ڂ��������
	WFNOTE_FRIENDLIST_SEQ_INFO,		// �ڂ��������
	WFNOTE_FRIENDLIST_SEQ_INFORET,	// �ڂ��������
	WFNOTE_FRINEDLIST_SEQ_NAMECHG_INIT,	// ���O�ύX
	WFNOTE_FRINEDLIST_SEQ_NAMECHG_WIPE,	// ���O�ύX
	WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAIT,	// ���O�ύX
	WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAITWIPE,	// ���O�ύX
	WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAITWIPEWAIT,	// ���O�ύX
	WFNOTE_FRIENDLIST_SEQ_DELETE_INIT,	// �j��
	WFNOTE_FRIENDLIST_SEQ_DELETE_YESNODRAW,	// �j��
	WFNOTE_FRIENDLIST_SEQ_DELETE_WAIT,	// �j��YESNO���܂��Ă��珈��
	WFNOTE_FRIENDLIST_SEQ_DELETE_ANMSTART,	// �j��YESNO���܂��Ă��珈��
	WFNOTE_FRIENDLIST_SEQ_DELETE_ANMWAIT,	// �j��YESNO���܂��Ă��珈��
	WFNOTE_FRIENDLIST_SEQ_DELETE_END,		//  �j��YESNO���܂��Ă��珈��
	WFNOTE_FRIENDLIST_SEQ_CODE_INIT,	// �R�[�h�؂�ւ�
	WFNOTE_FRIENDLIST_SEQ_CODE_WAIT,	// �����L�[�������Ă��炤
	WFNOTE_FRIENDLIST_SEQ_END,
};

// ������
#define WFNOTE_FRIENDLIST_SHIORI_X		( 0x16 )	// �ʒu�i�L�����T�C�Y�j
#define WFNOTE_FRIENDLIST_SHIORI_Y		( 0 )		// �ʒu(�L�����T�C�Y)
#define WFNOTE_FRIENDLIST_SHIORI_SIZX	( 9 )		// �T�C�Y�i�L�����N�^�T�C�Y�j
#define WFNOTE_FRIENDLIST_SHIORI_SIZY	( 3 )		// �T�C�Y�i�L�����N�^�T�C�Y�j
#define WFNOTE_FRIENDLIST_SHIORI_ONEX	( 2 )		// �P�y�[�W���̃T�C�Y
#define WFNOTE_FRIENDLIST_SHIORI_OFSX	( 1 )		// ONEX�̒��ŕ\������L�����N�^�܂ł̐�
#define WFNOTE_FRIENDLIST_SHIORI_OFSY	( 1 )		// ONEX�̒��ŕ\������L�����N�^�܂ł̐�
#define WFNOTE_FRIENDLIST_SHIORISCRN_X	( 0 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDLIST_SHIORISCRN_Y	( 4 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDLIST_CL_SHIORISCRN_X	( 0x10 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDLIST_CL_SHIORISCRN_Y	( 0x0 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDLIST_SHIORISCRNPOS_X	( 3 )	// �|�W�V�����X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDLIST_EFFECTSHIORISCRNPOS_X	( 4 )	// �G�t�F�N�g�p�|�W�V�����X�N���[���ǂݍ��݂�
enum{												// �|�W�V�����X�N���[���ǂݍ��݂�
	WFNOTE_FRIENDLIST_SHIORISCRNPOS0_Y = 0,
	WFNOTE_FRIENDLIST_SHIORISCRNPOS1_Y,
	WFNOTE_FRIENDLIST_SHIORISCRNPOS2_Y,
	WFNOTE_FRIENDLIST_SHIORISCRNPOS3_Y,
};
#define WFNOTE_FRIENDLIST_SHIORIPOS_SIZ	(1)			// �|�W�V�����X�N���[���̑傫��

// ���ǂ�\���ʒu
#define WFNOTE_FRIENDLIST_BACK_X		( 11 )	// �߂�\���ʒu
#define WFNOTE_FRIENDLIST_BACK_Y		( 20 )	// �߂�\���ʒu
#define WFNOTE_FRIENDLIST_BACK_SIZX		( 10 )	// �߂�\���T�C�Y
#define WFNOTE_FRIENDLIST_BACK_SIZY		( 4 )	// �߂�\���T�C�Y
#define WFNOTE_FRIENDLIST_SCRBACK_X		( 6 )	// �߂�ǂݍ��ݐ�ʒu
#define WFNOTE_FRIENDLIST_SCRBACK_Y		( 0 )	// �߂�ǂݍ��ݐ�ʒu
#define WFNOTE_FRIENDLIST_CL_SCRBACK_X		( 0 )	// �߂�ǂݍ��ݐ�ʒu
#define WFNOTE_FRIENDLIST_CL_SCRBACK_Y		( 8 )	// �߂�ǂݍ��ݐ�ʒu

// �v���C���[�\���ʒu�isc_WFNOTE_FRIENDLIST_CURSORDATA�Ȃ��̈ʒu�f�[�^����̃I�t�Z�b�g�j
#define WFNOTE_FRIENDLIST_PLAYER_X		( 8 )
#define WFNOTE_FRIENDLIST_PLAYER_Y		( 6+CLACT_MAIN2_REND_Y_S32 )
#define WFNOTE_FRIENDLIST_PRI			( 8 )
#define WFNOTE_FRIENDLIST_BGPRI			( 1 )
// �͂ĂȂ̏ꍇ
#define WFNOTE_FRIENDLIST_NOTPlAYER_SIZX	(3)
#define WFNOTE_FRIENDLIST_NOTPlAYER_SIZY	(4)
#define WFNOTE_FRIENDLIST_NOTPlAYERSCRN_X	(0)
#define WFNOTE_FRIENDLIST_NOTPlAYERSCRN_Y	(0)

// �v���C���[���\���ʒu�isc_WFNOTE_FRIENDLIST_CURSORDATA�Ȃ��̈ʒu�f�[�^����̃I�t�Z�b�g�j
#define WFNOTE_FRIENDLIST_PLAYERNAME_X	( 0 )
#define WFNOTE_FRIENDLIST_PLAYERNAME_Y	( -24 )

// SEQ_MAIN�֐��߂�l
enum{
	WFNOTE_FRIENDLIST_SEQMAINRET_NONE,
	WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGELEFT,// �y�[�W�ύX
	WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGERIGHT,// �y�[�W�ύX
	WFNOTE_FRIENDLIST_SEQMAINRET_CANCEL,	// �߂�
	WFNOTE_FRIENDLIST_SEQMAINRET_SELECTPL,	// �l��I������
};


// �X�N���[��
#define WFNOTE_FIRENDLIST_SCRLL_COUNT	(12)	// �X�N���[���J�E���g�i4�̔{���j
#define WFNOTE_FRIENDLIST_SCRLL_SIZX	(256)	// �X�N���[���T�C�Y


// ���j���[�f�[�^
enum{
	WFNOTE_FRIENDLIST_MENU_NML,		// �ʏ탁�j���[
	WFNOTE_FRIENDLIST_MENU_CODE,	// �F�B�ԍ��̂ݓo�^���̃��j���[
	WFNOTE_FRIENDLIST_MENU_NUM,
};
#define WFNOTE_FRIENDLIST_MENU_LISTNUM	(4)	// ���j���[���X�g�̍��ڐ�

// �ڍ׉�ʂփt�F�[�h�̃E�G�C�g
#define WFNOTE_FRIENDLIST_INFO_WAIT	(4)

//  �����S���A�N�^�[
#define WFNOTE_FRIENDLIST_CLEARACT_Y	( -4 + CLACT_MAIN2_REND_Y_S32 )

//-------------------------------------
///	CODEIN
//=====================================
enum{	// �V�[�P���X
	WFNOTE_CODEIN_SEQ_NAMEIN,
	WFNOTE_CODEIN_SEQ_NAMEIN_WAIT,
	WFNOTE_CODEIN_SEQ_CODEIN_WAIT,
	WFNOTE_CODEIN_SEQ_END,

	WFNOTE_CODEIN_SEQ_NAMEINONLY,	// ���O���͂̂�
	WFNOTE_CODEIN_SEQ_NAMEINONLY_WAIT,	// ���O���͂̂�
	WFNOTE_CODEIN_SEQ_NAMEINONLY_END,	// ���O���͂̂�
	
};
#define FRIENDCODE_MAXLEN	(12)	// �F�B�R�[�h������

//-------------------------------------
///	MYCODE
//=====================================
// BMP	���b�Z�[�W
#define WFNOTE_MYCODE_MSG_X		( 5 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MYCODE_MSG_Y		( 13 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MYCODE_MSG_SIZX	( 23 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MYCODE_MSG_SIZY	( 8 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MYCODE_CGX		( 1 )	// �L�����N�^�I�t�Z�b�g
// BMP	�R�[�h
#define WFNOTE_MYCODE_CODE_X		( 10 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MYCODE_CODE_Y		( 8 )	// �J�n�ʒu�i�L�����P�ʁj
#define WFNOTE_MYCODE_CODE_SIZX		( 15 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MYCODE_CODE_SIZY		( 2 )	// �T�C�Y�i�L�����P�ʁj
#define WFNOTE_MYCODE_CODE_CGX		( WFNOTE_MYCODE_CGX+(WFNOTE_MYCODE_MSG_SIZX*WFNOTE_MYCODE_MSG_SIZY) )	// �L�����N�^�I�t�Z�b�g
#define WFNOTE_MYCODE_CODE_PR_X		( 4 )	// �\�����W

// �V�[�P���X
enum{
	WFNOTE_MYCODE_SEQ_INIT,
	WFNOTE_MYCODE_SEQ_MAIN,
};

//-------------------------------------
///	FRIENDINFO
//=====================================
#define WFNOTE_FRIENDINFO_PAGE_NUM					( 7 )	// �y�[�W��
#define WFNOTE_FRIENDINFO_FRONTIEROFFPAGE_NUM		( 2 )	// �t�����e�B�A�����y�[�W��

// �V�[�P���X
enum{
	WFNOTE_FRIENDINFO_SEQ_INIT,
	WFNOTE_FRIENDINFO_SEQ_INITRP,
	WFNOTE_FRIENDINFO_SEQ_MAIN,
	WFNOTE_FRIENDINFO_SEQ_SCRINIT,
	WFNOTE_FRIENDINFO_SEQ_SCR,
	WFNOTE_FRIENDINFO_SEQ_FRIENDCHG,
	WFNOTE_FRIENDINFO_SEQ_END,
};

// �r�b�g�}�b�v
#define WFNOTE_FRIENDINFO_TEXT_CGX0	( 1 )	// �T�C�Y��
#define WFNOTE_FRIENDINFO_TEXT_CGX1	( WFNOTE_FRIENDINFO_TEXT_CGX0+(500) )	// �T�C�Y��


// ������
#define WFNOTE_FRIENDINFO_SHIORI_X		( 22 )	// �ʒu�i�L�����T�C�Y�j
#define WFNOTE_FRIENDINFO_SHIORI_Y		( 0 )		// �ʒu(�L�����T�C�Y)
#define WFNOTE_FRIENDINFO_SHIORI_SIZX	( 9 )		// �T�C�Y�i�L�����N�^�T�C�Y�j
#define WFNOTE_FRIENDINFO_SHIORI_SIZY	( 3 )		// �T�C�Y�i�L�����N�^�T�C�Y�j
#define WFNOTE_FRIENDINFO_SHIORI_ONEX	( 1 )		// �P�y�[�W���̃T�C�Y
#define WFNOTE_FRIENDINFO_SHIORI_OFSX	( 1 )		// ONEX�̒��ŕ\������L�����N�^�܂ł̐�
#define WFNOTE_FRIENDINFO_SHIORI_OFSY	( 1 )		// ONEX�̒��ŕ\������L�����N�^�܂ł̐�
#define WFNOTE_FRIENDINFO_SHIORISCRN_X	( 16 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_SHIORISCRN_Y	( 4 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_CL_SHIORISCRN_X	( 0x10 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_CL_SHIORISCRN_Y	( 0x0 )		// �X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_SHIORISCRNPOS_X	( 5 )	// �|�W�V�����X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_SHIORISCRNPOS_Y	( 0 )	// �|�W�V�����X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_EFFECTSHIORISCRNPOS_X	( 5 )	// �G�t�F�N�g�p�|�W�V�����X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_EFFECTSHIORISCRNPOS_Y	( 1 )	// �G�t�F�N�g�p�|�W�V�����X�N���[���ǂݍ��݂�
#define WFNOTE_FRIENDINFO_SHIORIPOS_SIZ	(1)			// �|�W�V�����X�N���[���̑傫��

// �X�N���[��
#define WFNOTE_FRIENDINFO_SCRLL_COUNT	(4)	// �X�N���[���J�E���g
#define WFNOTE_FRIENDINFO_SCRLL_SIZX	(256)	// �X�N���[���T�C�Y



// �y�[�W�O�O
enum{
	WFNOTE_FRIENDINFO_PAGE00_BA_GR,
	WFNOTE_FRIENDINFO_PAGE00_BA_BT,
	WFNOTE_FRIENDINFO_PAGE00_BA_TR,
	WFNOTE_FRIENDINFO_PAGE00_BA_DAY,
	WFNOTE_FRIENDINFO_PAGE00_BA_TRG,
	WFNOTE_FRIENDINFO_PAGE00_BA_NUM,
};
#define WFNOTE_FRIENDINFO_PAGE00_BA_GR_X	( 14 )	// �r�b�g�}�b�v�G���A	�O���[�v�Ƒΐ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_GR_Y	( 4 )	// �r�b�g�}�b�v�G���A	�O���[�v�Ƒΐ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_GR_SIZX	( 16 )	// �r�b�g�}�b�v�G���A	�O���[�v�Ƒΐ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_GR_SIZY ( 6 )	// �r�b�g�}�b�v�G���A	�O���[�v�Ƒΐ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_BT_X	( 22 )	// �r�b�g�}�b�v�G���A	�ΐ퐬��
#define WFNOTE_FRIENDINFO_PAGE00_BA_BT_Y	( 10 )	// �r�b�g�}�b�v�G���A	�ΐ퐬��
#define WFNOTE_FRIENDINFO_PAGE00_BA_BT_SIZX	( 8 )	// �r�b�g�}�b�v�G���A	�ΐ퐬��
#define WFNOTE_FRIENDINFO_PAGE00_BA_BT_SIZY ( 5 )	// �r�b�g�}�b�v�G���A	�ΐ퐬��
#define WFNOTE_FRIENDINFO_PAGE00_BA_TR_X	( 14 )	// �r�b�g�}�b�v�G���A	�����ƃ|�t�B��
#define WFNOTE_FRIENDINFO_PAGE00_BA_TR_Y	( 15 )	// �r�b�g�}�b�v�G���A	�����ƃ|�t�B��
#define WFNOTE_FRIENDINFO_PAGE00_BA_TR_SIZX	( 17 )	// �r�b�g�}�b�v�G���A	�����ƃ|�t�B��
#define WFNOTE_FRIENDINFO_PAGE00_BA_TR_SIZY ( 6 )	// �r�b�g�}�b�v�G���A	�����ƃ|�t�B��
#define WFNOTE_FRIENDINFO_PAGE00_BA_DAY_X	( 2 )	// �r�b�g�}�b�v�G���A	���ɂ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_DAY_Y	( 21 )	// �r�b�g�}�b�v�G���A	���ɂ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_DAY_SIZX	( 28 )	// �r�b�g�}�b�v�G���A	���ɂ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_DAY_SIZY ( 3 )	// �r�b�g�}�b�v�G���A	���ɂ�
#define WFNOTE_FRIENDINFO_PAGE00_BA_TRG_X	( 1 )	// �r�b�g�}�b�v�G���A	�g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_BA_TRG_Y	( 5 )	// �r�b�g�}�b�v�G���A	�g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_BA_TRG_SIZX	( 11 )	// �r�b�g�}�b�v�G���A	�g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_BA_TRG_SIZY ( 13 )	// �r�b�g�}�b�v�G���A	�g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_GRT_X	( 0 )	// �O���[�v�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_GRT_Y	( 4 )	// �O���[�v�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_BTT_X	( 0 )	// �ΐ퐬�у^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_BTT_Y	( 28 )	// �ΐ퐬�у^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_BTW_X	( 0 )	// �ΐ퐬�т���
#define WFNOTE_FRIENDINFO_PAGE00_BTW_Y	( 4 )	// �ΐ퐬�т���
#define WFNOTE_FRIENDINFO_PAGE00_BTL_X	( 0 )	// �ΐ퐬�т܂�
#define WFNOTE_FRIENDINFO_PAGE00_BTL_Y	( 20 )	// �ΐ퐬�т܂�
#define WFNOTE_FRIENDINFO_PAGE00_BTWLN_X ( 32 )// �ΐ퐬�т����܂��̐���
#define WFNOTE_FRIENDINFO_PAGE00_TRT_X	( 0 )	// �|�P���������^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_TRT_Y	( 4 )	// �|�P���������^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_TRN_X	( 104 )	// �|�P���������l
#define WFNOTE_FRIENDINFO_PAGE00_TRN_Y	( 4 )	// �|�P���������l
#define WFNOTE_FRIENDINFO_PAGE00_POT_X	( 0 )	// �|�t�B���^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_POT_Y	( 20 )	// �|�t�B���^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_PON_X	( 104 )	// �|�t�B���l
#define WFNOTE_FRIENDINFO_PAGE00_PON_Y	( 20 )	// �|�t�B���l
#define WFNOTE_FRIENDINFO_PAGE00_DAYT_X	( 0 )	// �Ō�ɗV�񂾓��t�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_DAYT_Y	( 6 )	// �Ō�ɗV�񂾓��t�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE00_DAYN_X	( 144 )	// �Ō�ɗV�񂾓��t�l
#define WFNOTE_FRIENDINFO_PAGE00_DAYN_Y	( 6 )	// �Ō�ɗV�񂾓��t�l
#define WFNOTE_FRIENDINFO_PAGE00_TRNNA_X	( 24 )	// �g���[�i�[���O
#define WFNOTE_FRIENDINFO_PAGE00_TRNNA_Y	( 88 )	// �g���[�i�[���O
#define WFNOTE_FRIENDINFO_PAGE00_TRNGR_X	( 4 )	// �g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_TRNGR_Y	( 4 )	// �g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZX	( 10 )	// �g���[�i�[�O���t�B�b�N�T�C�Y��
#define WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZY	( 10 )	// �g���[�i�[�O���t�B�b�N�T�C�Y��
#define WFNOTE_FRIENDINFO_PAGE00_TRGRA_CUT_X	(0)	// �؂����
#define WFNOTE_FRIENDINFO_PAGE00_TRGRA_CUT_Y	(0)	// �؂����
#define WFNOTE_FRIENDINFO_PAGE00_TRNGR_CSIZX ( 11 )	// �g���[�i�[�O���t�B�b�N
#define WFNOTE_FRIENDINFO_PAGE00_TRNGR_CSIZY ( 11 )	// �g���[�i�[�O���t�B�b�N

// �y�[�W�O�P
enum{
	WFNOTE_FRIENDINFO_PAGE01_BA,
	WFNOTE_FRIENDINFO_PAGE01_BA_NUM,
};
#define WFNOTE_FRIENDINFO_PAGE01_BA_X	( 2 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE01_BA_Y	( 4 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE01_BA_SIZX ( 20 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE01_BA_SIZY ( 9 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE01_BTT_X		( 0 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTT_Y		( 8 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTL_X		( 0 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTLN_X		( 56 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTL_Y		( 32 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTM_X		( 0 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTMN_X		( 56 )	// �o�g���^���[�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE01_BTM_Y		( 56 )	// �o�g���^���[�^�C�g��

// �y�[�W02
enum{
	WFNOTE_FRIENDINFO_PAGE02_BA_TT,		// �^�C�g��
	WFNOTE_FRIENDINFO_PAGE02_LV50TT,	// LV50�^�C�g��
	WFNOTE_FRIENDINFO_PAGE02_LV50NO,	// LV50�l
	WFNOTE_FRIENDINFO_PAGE02_LV100TT,	// LV100�^�C�g��
	WFNOTE_FRIENDINFO_PAGE02_LV100NO,	// LV100�l
	WFNOTE_FRIENDINFO_PAGE02_BA_NUM,
};

#define WFNOTE_FRIENDINFO_PAGE02_BA_TT_X	( 2 )	// �r�b�g�}�b�v�G���A	�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_TT_Y	( 5 )	// �r�b�g�}�b�v�G���A	�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_TT_SIZX ( 14 )	// �r�b�g�}�b�v�G���A	�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_TT_SIZY ( 2 )	// �r�b�g�}�b�v�G���A	�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_X	( 2 )	// �r�b�g�}�b�v�G���A	LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_Y	( 8 )	// �r�b�g�}�b�v�G���A	LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_SIZX ( 28 )	// �r�b�g�}�b�v�G���A	LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_SIZY ( 2 )	// �r�b�g�}�b�v�G���A	LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_X	( 2 )	// �r�b�g�}�b�v�G���A	LV50�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_Y	( 11 )	// �r�b�g�}�b�v�G���A	LV50�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_SIZX ( 28 )	// �r�b�g�}�b�v�G���A	LV50�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_SIZY ( 4 )	// �r�b�g�}�b�v�G���A	LV50�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_X	( 2 )	// �r�b�g�}�b�v�G���A	LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_Y	( 16 )	// �r�b�g�}�b�v�G���A	LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_SIZX ( 28 )	// �r�b�g�}�b�v�G���A	LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_SIZY ( 2 )	// �r�b�g�}�b�v�G���A	LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_X	( 2 )	// �r�b�g�}�b�v�G���A	LV100�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_Y	( 19 )	// �r�b�g�}�b�v�G���A	LV100�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_SIZX ( 28 )	// �r�b�g�}�b�v�G���A	LV100�l
#define WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_SIZY ( 4 )	// �r�b�g�}�b�v�G���A	LV100�l
#define WFNOTE_FRIENDINFO_PAGE02_TT_X		( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_TT_Y		( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV50TT_X	( 0 )	// LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV50TT2_X	( 120 )	// LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV50TT_Y	( 0 )	// LV50�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV50LT_X	( 0 )	// LV50�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV50LN0_X	( 56 )	// LV50�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV50LN1_X	( 168 )	// LV50�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV50L_Y	( 0 )	// LV50�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV50MT_X	( 0 )	// LV50�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV50MN0_X	( 56 )	// LV50�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV50MN1_X	( 168 )	// LV50�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV50M_Y	( 16 )	// LV50�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV100TT_X	( 0 )	// LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV100TT2_X	( 120 )	// LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV100TT_Y	( 0 )	// LV100�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE02_LV100LT_X	( 0 )	// LV100�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV100LN0_X	( 56 )	// LV100�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV100LN1_X	( 168 )	// LV100�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV100L_Y	( 0 )	// LV100�O��
#define WFNOTE_FRIENDINFO_PAGE02_LV100MT_X	( 0 )	// LV100�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV100MN0_X	( 56 )	// LV100�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV100MN1_X	( 168 )	// LV100�ō�
#define WFNOTE_FRIENDINFO_PAGE02_LV100M_Y	( 16 )	// LV100�ō�


// �y�[�W03
enum{
	WFNOTE_FRIENDINFO_PAGE03_BA,
	WFNOTE_FRIENDINFO_PAGE03_BA_NUM,
};
#define WFNOTE_FRIENDINFO_PAGE03_BA_X	( 2 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE03_BA_Y	( 5 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE03_BA_SIZX ( 28 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE03_BA_SIZY ( 11 )	// �r�b�g�}�b�v�G���A

#define WFNOTE_FRIENDINFO_PAGE03_TT_X	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE03_TT_Y	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE03_CPT_X	( 104 )	// �L���b�X���|�C���g�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE03_CPT_Y	( 20 )	// �L���b�X���|�C���g�^�C�g��
#define WFNOTE_FRIENDINFO_PAGE03_CPLT_X	( 0 )	// �L���b�X���|�C���g�O��
#define WFNOTE_FRIENDINFO_PAGE03_CPLN0_X	( 56 )	// �L���b�X���|�C���g�O��
#define WFNOTE_FRIENDINFO_PAGE03_CPLN1_X	( 168 )	// �L���b�X���|�C���g�O��
#define WFNOTE_FRIENDINFO_PAGE03_CPL_Y	( 48 )	// �L���b�X���|�C���g�O��
#define WFNOTE_FRIENDINFO_PAGE03_CPMT_X	( 0 )	// �L���b�X���|�C���g�ő�
#define WFNOTE_FRIENDINFO_PAGE03_CPMN0_X	( 56 )	// �L���b�X���|�C���g�ő�
#define WFNOTE_FRIENDINFO_PAGE03_CPMN1_X	( 168 )	// �L���b�X���|�C���g�ő�
#define WFNOTE_FRIENDINFO_PAGE03_CPM_Y	( 72 )	// �L���b�X���|�C���g�ő�


// �y�[�W04
enum{
	WFNOTE_FRIENDINFO_PAGE04_BA,
	WFNOTE_FRIENDINFO_PAGE04_BA_NUM,
};
#define WFNOTE_FRIENDINFO_PAGE04_BA_X	( 2 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE04_BA_Y	( 5 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE04_BA_SIZX ( 20 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE04_BA_SIZY ( 11 )	// �r�b�g�}�b�v�G���A

#define WFNOTE_FRIENDINFO_PAGE04_TT_X	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE04_TT_Y	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE04_TT2_Y	( 24 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE04_LT_X	( 0 )	// �O��
#define WFNOTE_FRIENDINFO_PAGE04_LN_X	( 56 )	// �O��
#define WFNOTE_FRIENDINFO_PAGE04_L_Y	( 48 )	// �O��
#define WFNOTE_FRIENDINFO_PAGE04_MT_X	( 0 )	// �ő�
#define WFNOTE_FRIENDINFO_PAGE04_MN_X	( 56 )	// �ő�
#define WFNOTE_FRIENDINFO_PAGE04_M_Y	( 72 )	// �ő�


// �y�[�W05
enum{
	WFNOTE_FRIENDINFO_PAGE05_BA,
	WFNOTE_FRIENDINFO_PAGE05_BA_NUM,
};
#define WFNOTE_FRIENDINFO_PAGE05_BA_X	( 2 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE05_BA_Y	( 5 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE05_BA_SIZX ( 20 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE05_BA_SIZY ( 11 )	// �r�b�g�}�b�v�G���A

#define WFNOTE_FRIENDINFO_PAGE05_TT_X	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE05_TT_Y	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE05_LT_X	( 0 )	// �O��
#define WFNOTE_FRIENDINFO_PAGE05_LN_X	( 56 )	// �O��
#define WFNOTE_FRIENDINFO_PAGE05_L_Y	( 24 )	// �O��
#define WFNOTE_FRIENDINFO_PAGE05_MT_X	( 0 )	// �ő�
#define WFNOTE_FRIENDINFO_PAGE05_MN_X	( 56 )	// �ő�
#define WFNOTE_FRIENDINFO_PAGE05_M_Y	( 48 )	// �ő�

// �y�[�W06
enum{
	WFNOTE_FRIENDINFO_PAGE06_BA,
	WFNOTE_FRIENDINFO_PAGE06_BA_NUM,
};
#define WFNOTE_FRIENDINFO_PAGE06_BA_X	( 2 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE06_BA_Y	( 5 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE06_BA_SIZX ( 22 )	// �r�b�g�}�b�v�G���A
#define WFNOTE_FRIENDINFO_PAGE06_BA_SIZY ( 11 )	// �r�b�g�}�b�v�G���A

#define WFNOTE_FRIENDINFO_PAGE06_TT_X	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE06_TT_Y	( 0 )	// �^�C�g��
#define WFNOTE_FRIENDINFO_PAGE06_BC_X	( 0 )	// ���܂Ȃ�
#define WFNOTE_FRIENDINFO_PAGE06_BC_N_X	( 120 )	// �l
#define WFNOTE_FRIENDINFO_PAGE06_BC_Y	( 24 )	// Y
#define WFNOTE_FRIENDINFO_PAGE06_BB_X	( 0 )	// ���܂̂�
#define WFNOTE_FRIENDINFO_PAGE06_BB_N_X	( 120 )	// �l
#define WFNOTE_FRIENDINFO_PAGE06_BB_Y	( 48 )	// Y
#define WFNOTE_FRIENDINFO_PAGE06_BL_X	( 0 )	// ���܂̂�
#define WFNOTE_FRIENDINFO_PAGE06_BL_N_X	( 120 )	// �l
#define WFNOTE_FRIENDINFO_PAGE06_BL_Y	( 72 )	// Y


// �y�[�W�ύX�E�G�C�g
#define WFNOTE_FRIENDINFO_PAGECHG_WAIT	( 4 )



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	���j���[�f�[�^�\����
//=====================================
typedef struct {
	u32 strid;
	u32 param;
} WFNOTE_MENU_DATA;


//-------------------------------------
//	�X�N���[���G���A�f�[�^
//=====================================
typedef struct {
	s16 scrn_x;		// �`��G���A�J�n�X�N���[����
	s16 scrn_y;		// �`��G���A�J�n�X�N���[��y
	u8 scrn_sizx;	// �`��G���A�T�C�Y�X�N���[����
	u8 scrn_sizy;	// �`��G���A�T�C�Y�X�N���[����
} WFNOTE_SCRNAREA;


//-------------------------------------
//	�X�N���[���f�[�^
//=====================================
typedef struct {
	void* p_scrnbuff[WFNOTE_SCRNDATA_NUM];	// �X�N���[���f�[�^
	NNSG2dScreenData* p_scrn[WFNOTE_SCRNDATA_NUM];	// �X�N���[���f�[�^
} WFNOTE_SCRNDATA;


//-------------------------------------
///	�R�[�h���͌��ʃf�[�^
//=====================================
typedef struct {
	STRBUF* p_name;
	STRBUF* p_code;
} WFNOTE_CODEIN_DATA;

//-------------------------------------
///	���O���͏��������
//=====================================
typedef struct {
	const STRCODE* cp_str;
	BOOL init;
} WFNOTE_NAMEIN_INITDATA;


//-------------------------------------
///	�F�B�C���f�b�N�X�f�[�^
//=====================================
typedef struct {
	u32 friendnum;	// �F�B�̑���
	u8 fridx[WFNOTE_FRIENDLIST_FRIENDMAX];	// �F�B�C���f�b�N�X
} WFNOTE_IDXDATA;



//-------------------------------------
///	�O���[�o���f�[�^���[�N
//=====================================
typedef struct {
	SAVEDATA* p_save;	// �Z�[�u�f�[�^
	u8 subseq;			// �T�u�V�[�P���X
	u8 reqstatus;		// ���ɐi�݂����X�e�[�^�X
	u8 reqsubseq;		// ���̃X�e�[�^�X�̊J�n�T�u�V�[�P���X
	s8 select_listidx;	// �I���������X�g�C���f�b�N�X

	WFNOTE_IDXDATA idx;	// �F�B�f�[�^�C���f�b�N�X

	WFNOTE_CODEIN_DATA codein;	// �R�[�h���͉�ʂœ��͂����f�[�^
	WFNOTE_NAMEIN_INITDATA namein_init;	// ���O���͉�ʏ������f�[�^
} WFNOTE_DATA;

//-------------------------------------
///	�\�����[�N
//=====================================
typedef struct {
	GF_BGL_INI* p_bgl;		// bgl
	CLACT_SET_PTR p_clact;	// �A�N�^�[
	NNSG2dRenderSurface scrll_surface;	// �X�N���[���p�T�[�t�F�[�X
	CLACT_U_EASYRENDER_DATA	renddata;	// �����_�[�f�[�^
	CLACT_U_RES_MANAGER_PTR	p_resman[CLACT_RESOURCE_NUM];	// ���\�[�X�}�l�[�W��
	WORDSET* p_wordset;			// ���b�Z�[�W�W�J�p���[�N�}�l�[�W���[
	MSGDATA_MANAGER* p_msgman;	// ���O���̓��b�Z�[�W�f�[�^�}�l�[�W���[
	ARCHANDLE* p_handle;		// �O���t�B�b�N�A�[�J�C�u�n���h��
	GF_BGL_BMPWIN title;		// �^�C�g���p�r�b�g�}�b�v
	STRBUF* p_titlestr;			// �^�C�g��������
	STRBUF* p_titletmp;			// �^�C�g��������
	u32 title_stridx;			// �^�C�g���`��C���f�b�N�X
	CLACT_HEADER clheader;		// ��{�I��OAM�̃w�b�_�[
	CLACT_U_RES_OBJ_PTR p_resobj[CLACT_RESOURCE_NUM];// ���\�[�X�I�u�W�F

	CLACT_WORK_PTR p_yazirushi[WFNOTE_YAZIRUSHINUM];	// ���A�N�^�[

	WFNOTE_SCRNDATA scrn;	// ��{�X�N���[���f�[�^

} WFNOTE_DRAW;

//-------------------------------------
///	���[�h�I����ʃ��[�N
//=====================================
typedef struct {
	// �f�[�^
	u32 cursor;	// �J�[�\��
	
	// �O���t�B�b�N
	GF_BGL_BMPWIN msg;	// ���b�Z�[�W��	
	void* p_scrnbuff;	// �X�N���[���f�[�^
	NNSG2dScreenData* p_scrn;	// �X�N���[���f�[�^
	GF_BGL_BMPWIN talk;	// ��b�E�B���h�E
	BMPMENU_WORK* p_yesno;// YESNO�E�B���h�E
	u32 talkmsg_idx;	// ��b���b�Z�[�Widx
	u32 msg_speed;	// ��b���b�Z�[�W�X�s�[�h
	STRBUF* p_str;
} WFNOTE_MODESELECT;

//-------------------------------------
///	�F�B�f�[�^�\����ʃ��[�N
//	�`��G���A�f�[�^
//=====================================
typedef struct {
	WFNOTE_SCRNAREA scrn_area;	// �`��X�N���[���G���A
	GF_BGL_BMPWIN text;	// ���b�Z�[�W��
	WF_2DCWK* p_clwk[WFNOTE_FRIENDLIST_PAGEFRIEND_NUM];
} WFNOTE_FRIENDLIST_DRAWAREA;

//-------------------------------------
///	�F�B�f�[�^�J�[�\���f�[�^
//=====================================
typedef struct {
	WFNOTE_SCRNAREA scrn_data;
	u32 bg_frame;
} WFNOTE_FRIELDLIST_CURSORDATA;


//-------------------------------------
///	�F�B�f�[�^�\����ʃ��[�N
//=====================================
typedef struct {
	s16 page;	// �y�[�W
	s16 lastpage;
	s16 pos;	// �y�[�W���̈ʒu
	s16 lastpos;// 1�O�̈ʒu

	WFNOTE_FRIENDLIST_DRAWAREA drawdata[WFNOTE_DRAWAREA_NUM];
	s16 count;	// �X�N���[���J�E���^
	u16 way;	// �X�N���[������
	WF_2DCSYS* p_charsys;	// �L�����N�^�\���V�X�e��

	GF_BGL_BMPWIN backmsg;	// �߂郁�b�Z�[�W�p

	GF_BGL_BMPWIN talk;	// �g�[�N���b�Z�[�W�p 
	STRBUF* p_talkstr;
	u32 msgidx;			
	u32 msgspeed;
	GF_BGL_BMPWIN menu;	// ���j���[���X�g�p
	BMPLIST_DATA* p_menulist[WFNOTE_FRIENDLIST_MENU_NUM];	// ���j���[���X�g
    BMPLIST_WORK* p_listwk;	// BMP���j���[���[�N
    
	BMPMENU_WORK* p_yesno;	// YESNO�E�B���h�E

	s32 wait;

	CLACT_WORK_PTR p_clearact;	// �����S���A�N�^�[
	u32 last_frame;
	
} WFNOTE_FRIENDLIST;

//-------------------------------------
///	�F�B�R�[�h���͉�ʃ��[�N
//=====================================
typedef struct {
	PROC* p_subproc;	// �T�u�v���Z�X
	NAMEIN_PARAM* p_namein;	// ���O���̓p�����[�^
	CODEIN_PARAM* p_codein;	// �R�[�h���̓p�����[�^
} WFNOTE_CODEIN;

//-------------------------------------
///	�����̗F�B�R�[�h�m�F��ʃ��[�N
//=====================================
typedef struct {
	// �O���t�B�b�N�f�[�^
	GF_BGL_BMPWIN msg;	// ���b�Z�[�W��	
	GF_BGL_BMPWIN code;	// �R�[�h��	
	void* p_scrnbuff;	// �X�N���[���f�[�^
	NNSG2dScreenData* p_scrn;	// �X�N���[���f�[�^
} WFNOTE_MYCODE;



//-------------------------------------
///	�F�B�ڍ׏��`����
///	��ʍ\���X�N���[���f�[�^
//=====================================
typedef struct {
	void* p_scrnbuff[ WFNOTE_FRIENDINFO_PAGE_NUM ];	// �X�N���[���f�[�^
	NNSG2dScreenData* p_scrn[ WFNOTE_FRIENDINFO_PAGE_NUM ];	// �X�N���[���f�[�^
} WFNOTE_FRIENDINFO_SCRNDATA;

//-------------------------------------
///	�F�B�ڍ׏��`����
//	�\���G���A���
//=====================================
typedef struct {
	WFNOTE_SCRNAREA scrnarea;
	u32 cgx;
	GF_BGL_BMPWIN*	p_msg[ WFNOTE_FRIENDINFO_PAGE_NUM ];	//�\����
	u32 msgnum[ WFNOTE_FRIENDINFO_PAGE_NUM ];		// ����msg�r�b�g�}�b�v��
} WFNOTE_FRIENDINFO_DRAWAREA;

//-------------------------------------
///	�F�B�ڍ׏��\����ʃ��[�N
//=====================================
typedef struct {
	s16 page;	// ���݂̃y�[�W
	s16 lastpage;	// �P�O�̃y�[�W
	s32 count;	// �X�N���[���J�E���g
	WF_COMMON_WAY way;	// ����
	u16 wait;	// �E�G�C�g
	u16 rep;	// �㉺���s�[�g����
	BOOL frontier_draw;	// �t�����e�B�A�`��L��
	
	
	WFNOTE_FRIENDINFO_DRAWAREA drawarea[WFNOTE_DRAWAREA_NUM];// �\���G���A
	WFNOTE_FRIENDINFO_SCRNDATA scrn;	// ��ʍ\���X�N���[���f�[�^
} WFNOTE_FRIENDINFO;

//-------------------------------------
///	�r�b�g�}�b�v�o�^�f�[�^
//=====================================
typedef struct {
	u8 pos_x;
	u8 pos_y;
	u8 siz_x;
	u8 siz_y;
} WFNOTE_FRIENDINFO_BMPDATA;



//-------------------------------------
///	�F�B�蒠���[�N
//=====================================
typedef struct {
	u32 status;			// ���̏��
	WFNOTE_DRAW draw;	// �\���V�X�e�����[�N
	WFNOTE_DATA data;	// �f�[�^���[�N
	
	// status���[�N
	WFNOTE_MODESELECT	mode_select;	// ���[�h�Z���N�g
	WFNOTE_FRIENDLIST	friendlist;		// �F�B�f�[�^�\��
	WFNOTE_CODEIN		codein;			// �F�B�R�[�h����
	WFNOTE_MYCODE		mycode;			// �����̗F�B�R�[�h�m�F
	WFNOTE_FRIENDINFO	friendinfo;		// �F�B�ڍו\�����
	
	s32 wait;
} WFNOTE_WK;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�S�̊֐�
//=====================================
static void WFNOTE_VBlank( void* p_work );
static void WFNOTE_DrawInit( WFNOTE_WK* p_wk, u32 heapID );
static void WFNOTE_DrawExit( WFNOTE_WK* p_wk );


//-------------------------------------
///	�f�[�^���[�N�֐�
//=====================================
static void WFNOTE_DATA_Init( WFNOTE_DATA* p_data, void* p_save, u32 heapID );
static void WFNOTE_DATA_Exit( WFNOTE_DATA* p_data );
static void WFNOTE_DATA_SetReqStatus( WFNOTE_DATA* p_data, u8 reqstatus, u8 reqsubseq );
static void WFNOTE_DATA_CODEIN_Set( WFNOTE_DATA* p_data, const STRBUF* cp_name, const STRBUF* cp_code );
static u32 WFNOTE_DATA_NewFriendDataSet( WFNOTE_DATA* p_data, STRBUF* p_code, STRBUF* p_name );
static void WFNOTE_DATA_NAMEIN_INIT_Set( WFNOTE_DATA* p_data, const STRCODE* cp_str );
static void WFNOTE_DATA_SELECT_ListIdxSet( WFNOTE_DATA* p_data, u32 idx );
static void WFNOTE_DATA_FrIdxMake( WFNOTE_DATA* p_data );
static u32 WFNOTE_DATA_FrIdxGetIdx( const WFNOTE_DATA* cp_data, u32 idx );

//-------------------------------------
///	�`�惏�[�N�֐�
//=====================================
static void WFNOTE_DRAW_Init( WFNOTE_DRAW* p_draw, const WFNOTE_DATA* cp_data, u32 heapID );
static void WFNOTE_DRAW_Main( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_VBlank( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_Exit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_BankInit( void );
static void WFNOTE_DRAW_BgInit( WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_DRAW_BgExit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_ClactInit( WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_DRAW_ClactExit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_MsgInit( WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_DRAW_MsgExit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_BmpInit( WFNOTE_DRAW* p_draw, const WFNOTE_DATA* cp_data, u32 heapID );
static void WFNOTE_DRAW_BmpExit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_SCRNDATA_Init( WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_DRAW_SCRNDATA_Exit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_BmpTitleWrite( WFNOTE_DRAW* p_draw, u32 msg_idx );
static void WFNOTE_DRAW_BmpTitleOff( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_FriendCodeSetWordset( WFNOTE_DRAW* p_draw, u64 code );
static void WFNOTE_DRAW_FriendNameSetWordset( WFNOTE_DRAW* p_draw, SAVEDATA* p_save, u32 idx, u32 heapID );
static void WFNOTE_DRAW_FriendGroupSetWordset( WFNOTE_DRAW* p_draw, SAVEDATA* p_save, u32 idx, u32 heapID );
static BOOL WFNOTE_DRAW_FriendDaySetWordset( WFNOTE_DRAW* p_draw, SAVEDATA* p_save, u32 idx );
static void WFNOTE_DRAW_NumberSetWordset( WFNOTE_DRAW* p_draw, u32 num );
static void WFNOTE_DRAW_WflbyGameSetWordSet( WFNOTE_DRAW* p_draw, u32 num );
static void WFNOTE_DRAW_YazirushiInit( WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_DRAW_YazirushiExit( WFNOTE_DRAW* p_draw );
static void WFNOTE_DRAW_YazirushiSetDrawFlag( WFNOTE_DRAW* p_draw, BOOL flag );
static void WFNOTE_DRAW_YazirushiSetAnmFlag( WFNOTE_DRAW* p_draw, BOOL flag );



//-------------------------------------
///	�I�����[�h�֐�
//=====================================
static void WFNOTE_MODESELECT_Init( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static WFNOTE_STRET WFNOTE_MODESELECT_Main( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_MODESELECT_Exit( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MODESELECT_DrawInit( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_MODESELECT_DrawExit( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MODESELECT_SEQ_Init( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static BOOL WFNOTE_MODESELECT_SEQ_Main( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MODESELECT_CursorScrnPalChange( WFNOTE_MODESELECT* p_wk, WFNOTE_DRAW* p_draw );
static void WFNOTE_MODESELECT_EndDataSet( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data );
static void WFNOTE_MODESELECT_DrawOff( WFNOTE_MODESELECT* p_wk, WFNOTE_DRAW* p_draw );
static void WFNOTE_MODESELECT_TalkMsgPrint( WFNOTE_MODESELECT* p_wk, WFNOTE_DRAW* p_draw, u32 msgidx, u32 heapID );
static BOOL WFNOTE_MODESELECT_TalkMsgEndCheck( const WFNOTE_MODESELECT* cp_wk );
static void WFNOTE_MODESELECT_TalkMsgOff( WFNOTE_MODESELECT* p_wk );
static BOOL WFNOTE_MODESELECT_StatusChengeCheck( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data );

//-------------------------------------
///	�F�B�f�[�^�\���֐�
//=====================================
static void WFNOTE_FRIENDLIST_Init( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static WFNOTE_STRET WFNOTE_FRIENDLIST_Main( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDLIST_Exit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDLIST_DrawInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDLIST_DrawExit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDLIST_SEQ_DrawOn( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDLIST_SEQ_DrawOff( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static u32 WFNOTE_FRIENDLIST_SEQ_Main( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDLIST_SEQ_ScrllInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static BOOL WFNOTE_FRIENDLIST_SEQ_ScrllMain( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDLIST_SEQ_MenuInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static u32 WFNOTE_FRIENDLIST_SEQ_MenuWait( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static BOOL WFNOTE_FRIENDLIST_SEQ_CodeInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDLIST_SEQ_DeleteInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static BOOL WFNOTE_FRIENDLIST_SEQ_DeleteYesNoDraw( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static u32 WFNOTE_FRIENDLIST_SEQ_DeleteWait( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDLIST_DrawPage( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 page, u32 pos, u32 draw_area, u32 heapID );
static void WFNOTE_FRIENDLIST_DrawShiori( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page );
static void WFNOTE_FRIENDLIST_DrawShioriEff00( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page0, u32 page1 );
static void WFNOTE_FRIENDLIST_DrawShioriEff01( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page0, u32 page1 );
static void WFNOTE_FRIENDLIST_ScrSeqChange( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WF_COMMON_WAY way );
static void WFNOTE_FRIENDLIST_TalkMsgWrite( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 msgid, u32 heapID );
static BOOL WFNOTE_FRIENDLIST_TalkMsgEndCheck( const WFNOTE_FRIENDLIST* cp_wk );
static void WFNOTE_FRIENDLIST_TalkMsgOff( WFNOTE_FRIENDLIST* p_wk );
static u32 WFNOTE_FRIENDLIST_FRIDX_IdxGet( const WFNOTE_FRIENDLIST* cp_wk );
static void WFNOTE_FRIENDLIST_MENULISTCallBack( BMPLIST_WORK * p_wk, u32 param, u8 mode );
static void WFNOTE_FRIENDLIST_DeleteAnmStart( WFNOTE_FRIENDLIST* p_wk );
static BOOL WFNOTE_FRIENDLIST_DeleteAnmMain( WFNOTE_FRIENDLIST* p_wk );
static void WFNOTE_FRIENDLIST_DeleteAnmOff( WFNOTE_FRIENDLIST* p_wk );
static void WFNOTE_FRIENDLIST_DRAWAREA_Init( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNAREA* cp_area, u32 text_cgx, u32 heapID );
static void WFNOTE_FRIENDLIST_DRAWAREA_Exit( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDLIST_DRAWAREA_BackWrite( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn );
static void WFNOTE_FRIENDLIST_DRAWAREA_CursorWrite( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, u32 pos );
static void WFNOTE_FRIENDLIST_DRAWAREA_WritePlayer( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, WF_2DCSYS* p_charsys, const WFNOTE_SCRNDATA* cp_scrn, const WFNOTE_IDXDATA* cp_idx, u32 page, u32 heapID );
static void WFNOTE_FRIENDLIST_DRAWAREA_DrawOff( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn );
static void WFNOTE_FRIENDLIST_DRAWAREA_CharWkDel( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDLIST_DRAWAREA_SetPlayer( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WF_2DCSYS* p_charsys, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn, u32 pos, WIFI_LIST* p_list, u32 idx, u32 heapID );
static void WFNOTE_FRIENDLIST_DRAWAREA_SetNoPlayer( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn, u32 pos );


//-------------------------------------
///	�F�B�R�[�h���͉�ʃ��[�N
//=====================================
static void WFNOTE_CODEIN_Init( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static WFNOTE_STRET WFNOTE_CODEIN_Main( WFNOTE_CODEIN* p_wk, WFNOTE_WK* p_sys, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_CODEIN_Exit( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_CODEIN_DrawInit( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_CODEIN_DrawExit( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static NAMEIN_PARAM* WFNOTE_CODEIN_NameInParamMake( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, u32 heapID );

//-------------------------------------
///	�����̗F�B�R�[�h�m�F��ʃ��[�N
//=====================================
static void WFNOTE_MYCODE_Init( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static WFNOTE_STRET WFNOTE_MYCODE_Main( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MYCODE_Exit( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MYCODE_DrawInit( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_MYCODE_DrawExit( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MYCODE_SEQ_Init( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_MYCODE_DrawOff( WFNOTE_MYCODE* p_wk, WFNOTE_DRAW* p_draw );

//-------------------------------------
///	�F�B�ڍ׏��
//=====================================
static void WFNOTE_FRIENDINFO_Init( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static WFNOTE_STRET WFNOTE_FRIENDINFO_Main( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_Exit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDINFO_ScrollInit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static BOOL WFNOTE_FRIENDINFO_ScrollMain( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DrawInit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DrawExit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDINFO_DrawOn( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DrawOff( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRINEDINFO_DrawPage( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 drawarea, u32 page, u32 heapID );
static void WFNOTE_FRIENDINFO_ShioriDraw( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw, u32 page, u32 heapID );
static void WFNOTE_FRIENDINFO_ShioriOff( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDINFO_ShioriAnm00( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw, u32 page00, u32 page01 );
static void WFNOTE_FRIENDINFO_ShioriAnm01( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw, u32 page00, u32 page01 );
static BOOL WFNOTE_FRIENDINFO_SelectListIdxAdd( WFNOTE_DATA* p_data, WF_COMMON_WAY way );
static void WFNOTE_FRIENDINFO_SCRNDATA_Init( WFNOTE_FRIENDINFO_SCRNDATA* p_scrn, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_SCRNDATA_Exit( WFNOTE_FRIENDINFO_SCRNDATA* p_scrn );
static void WFNOTE_FRIENDINFO_DRAWAREA_Init( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNAREA* cp_scrnarea, u32 cgx, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAWAREA_Exit( WFNOTE_FRIENDINFO_DRAWAREA* p_wk );
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpInit( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, u32 idx, u32 num, const WFNOTE_FRIENDINFO_BMPDATA* cp_data, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpExit( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 idx );
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 idx );
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOffVReq( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 idx );
static void WFNOTE_FRIENDINFO_DRAWAREA_Page( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 page, const WFNOTE_FRIENDINFO_SCRNDATA* cp_scrn, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAWAREA_PageOff( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw );
static void WFNOTE_FRIENDINFO_DRAW_Page00( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Page01( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Page02( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Page03( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Page04( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Page05( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Page06( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID );
static void WFNOTE_FRIENDINFO_DRAW_Clean( WFNOTE_DRAW* p_draw, const WFNOTE_SCRNAREA* cp_scrnarea );
static void WFNOTE_FRIENDINFO_DRAW_Bmp( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 page, u32 bmp, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 msg_idx, u32 x, u32 y, u32 col, STRBUF* p_str, STRBUF* p_tmp );
static void WFNOTE_FRIENDINFO_PageChange( WFNOTE_FRIENDINFO* p_wk, s32 add );
static BOOL WFNOTE_FRIENDINFO_PofinCaseCheck( WFNOTE_DATA* p_data, u32 heapID );




//-----------------------------------------------------------------------------
/**
 *		�f�[�^
 */
//-----------------------------------------------------------------------------
//�@�X�N���[���G���A
static const WFNOTE_SCRNAREA sc_SCRNAREA[ WFNOTE_DRAWAREA_NUM ] = {
	{ 0, 0, 32, 24 },
	{ 32, 0, 32, 24 },
	{ -32, 0, 32, 24 }
};

// BG�R���g���[���f�[�^
static const GF_BGL_BGCNT_HEADER sc_WFNOTE_BGCNT[ WFNOTE_BG_NUM ] = {
	{	// WFNOTE_BG_MAIN_BACK
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		3, 0, 0, FALSE
	},
	{	// WFNOTE_BG_MAIN_MSG
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xc800, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
	{	// WFNOTE_BG_MAIN_SCROLL
		0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
		2, 0, 0, FALSE
	},
	{	// WFNOTE_BG_MAIN_SCRMSG
		0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x20000, GX_BG_EXTPLTT_01,
		1, 0, 0, FALSE
	},
	{	// WFNOTE_BG_SUB_BACK
		0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
		GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
		0, 0, 0, FALSE
	},
};

// BG�t���[���e�[�u��
static const u32 sc_WFNOTE_BGFRM[ WFNOTE_BG_NUM ] = {
	GF_BGL_FRAME0_M,
	GF_BGL_FRAME1_M,
	GF_BGL_FRAME2_M,
	GF_BGL_FRAME3_M,
	GF_BGL_FRAME0_S,
};

// �X�N���[���p�T�[�t�F�[�X
static NNSG2dViewRect s_SCRLLSURFACE_RECT = {
	{ 0, CLACT_MAIN2_REND_Y },
	{ SURFACE_WIDTH, SURFACE_HEIGHT }
};

//-------------------------------------
///	MODESELECT
//=====================================
static const BMPWIN_DAT WFNOTE_MODESELECT_YESNOWIN = {
    WFNOTE_MODESELECT_YESNO_FRM, 
	WFNOTE_MODESELECT_YESNO_X, WFNOTE_MODESELECT_YESNO_Y,
    WFNOTE_MODESELECT_YESNO_SIZX, WFNOTE_MODESELECT_YESNO_SIZY,
	WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_MODESELECT_YESNOCGX
};

//-------------------------------------
///	FRIENDLIST
//=====================================
static const WFNOTE_FRIELDLIST_CURSORDATA sc_WFNOTE_FRIENDLIST_CURSORDATA[ WFNOTE_FRIENDLIST_CURSORDATA_NUM ] = {
	// �v���C���[0�`8
	{	
		{ 1, 4, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{
		{ 1, 8, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{
		{ 1, 12, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{
		{ 1, 16, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{	
		{ 16, 4, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{
		{ 16, 8, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{
		{ 16, 12, 15, 4 },
		GF_BGL_FRAME2_M
	},
	{
		{ 16, 16, 15, 4 },
		GF_BGL_FRAME2_M
	},
	// �߂�
	{
		{ WFNOTE_FRIENDLIST_BACK_X, WFNOTE_FRIENDLIST_BACK_Y, 
			WFNOTE_FRIENDLIST_BACK_SIZX, WFNOTE_FRIENDLIST_BACK_SIZY },
		GF_BGL_FRAME0_M
	},
};

static const u8 sc_POSSCRNY[ WFNOTE_FRIENDLIST_PAGE_NUM ] = {
	WFNOTE_FRIENDLIST_SHIORISCRNPOS0_Y,
	WFNOTE_FRIENDLIST_SHIORISCRNPOS1_Y,
	WFNOTE_FRIENDLIST_SHIORISCRNPOS2_Y,
	WFNOTE_FRIENDLIST_SHIORISCRNPOS3_Y,
};

// ���j���[�f�[�^
static const WFNOTE_MENU_DATA sc_WFNOTE_FRIENDLIST_MENU[WFNOTE_FRIENDLIST_MENU_NUM][WFNOTE_FRIENDLIST_MENU_LISTNUM] = {
	{
		{ msg_wifi_note_09, (u32)WFNOTE_FRIENDLIST_SEQ_INFOINIT },
		{ msg_wifi_note_10, (u32)WFNOTE_FRINEDLIST_SEQ_NAMECHG_INIT },
		{ msg_wifi_note_11, (u32)WFNOTE_FRIENDLIST_SEQ_DELETE_INIT },
		{ msg_wifi_note_12, (u32)WFNOTE_FRIENDLIST_SEQ_MAIN },
	},
	{
		{ msg_wifi_note_33, (u32)WFNOTE_FRIENDLIST_SEQ_CODE_INIT },
		{ msg_wifi_note_10, (u32)WFNOTE_FRINEDLIST_SEQ_NAMECHG_INIT },
		{ msg_wifi_note_11, (u32)WFNOTE_FRIENDLIST_SEQ_DELETE_INIT },
		{ msg_wifi_note_12, (u32)WFNOTE_FRIENDLIST_SEQ_MAIN },
	}
};
static const BMPLIST_HEADER WFNOTE_FRINEDLIST_MENUHEADER = {
    NULL,			// �\�������f�[�^�|�C���^
    WFNOTE_FRIENDLIST_MENULISTCallBack,		// �J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
    NULL,					// ���\�����Ƃ̃R�[���o�b�N�֐�
    NULL,                   // 
    WFNOTE_FRIENDLIST_MENU_LISTNUM,	// ���X�g���ڐ�
    WFNOTE_FRIENDLIST_MENU_LISTNUM,	// �\���ő區�ڐ�
    0,						// ���x���\���w���W
    8,						// ���ڕ\���w���W
    0,						// �J�[�\���\���w���W
    0,						// �\���x���W
    FBMP_COL_BLACK,			// �����F
    FBMP_COL_WHITE,			// �w�i�F
    FBMP_COL_BLK_SDW,		// �����e�F
    0,						// �����Ԋu�w
    16,						// �����Ԋu�x
    BMPLIST_LRKEY_SKIP,		// �y�[�W�X�L�b�v�^�C�v
    FONT_SYSTEM,				// �����w��
    0,						// �a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
    NULL                    // work
};

// YESNO�E�B���h�E
static const BMPWIN_DAT WFNOTE_FRIENDLIST_YESNOWIN = {
    WFNOTE_FRIENDLIST_YESNO_FRM, 
	WFNOTE_FRIENDLIST_YESNO_X, WFNOTE_FRIENDLIST_YESNO_Y,
    WFNOTE_FRIENDLIST_YESNO_SIZX, WFNOTE_FRIENDLIST_YESNO_SIZY,
	WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_FRIENDLIST_YESNOCGX
};



//-------------------------------------
///	FRIENDINFO
//=====================================
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE00MSG[ WFNOTE_FRIENDINFO_PAGE00_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE00_BA_GR_X, WFNOTE_FRIENDINFO_PAGE00_BA_GR_Y,
		WFNOTE_FRIENDINFO_PAGE00_BA_GR_SIZX, WFNOTE_FRIENDINFO_PAGE00_BA_GR_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE00_BA_BT_X, WFNOTE_FRIENDINFO_PAGE00_BA_BT_Y,
		WFNOTE_FRIENDINFO_PAGE00_BA_BT_SIZX, WFNOTE_FRIENDINFO_PAGE00_BA_BT_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE00_BA_TR_X, WFNOTE_FRIENDINFO_PAGE00_BA_TR_Y,
		WFNOTE_FRIENDINFO_PAGE00_BA_TR_SIZX, WFNOTE_FRIENDINFO_PAGE00_BA_TR_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE00_BA_DAY_X, WFNOTE_FRIENDINFO_PAGE00_BA_DAY_Y,
		WFNOTE_FRIENDINFO_PAGE00_BA_DAY_SIZX, WFNOTE_FRIENDINFO_PAGE00_BA_DAY_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE00_BA_TRG_X, WFNOTE_FRIENDINFO_PAGE00_BA_TRG_Y,
		WFNOTE_FRIENDINFO_PAGE00_BA_TRG_SIZX, WFNOTE_FRIENDINFO_PAGE00_BA_TRG_SIZY,
	},
};
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE01MSG[ WFNOTE_FRIENDINFO_PAGE01_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE01_BA_X, WFNOTE_FRIENDINFO_PAGE01_BA_Y,
		WFNOTE_FRIENDINFO_PAGE01_BA_SIZX, WFNOTE_FRIENDINFO_PAGE01_BA_SIZY,
	},
};
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE02MSG[ WFNOTE_FRIENDINFO_PAGE02_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE02_BA_TT_X, WFNOTE_FRIENDINFO_PAGE02_BA_TT_Y,
		WFNOTE_FRIENDINFO_PAGE02_BA_TT_SIZX, WFNOTE_FRIENDINFO_PAGE02_BA_TT_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_X, WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_Y,
		WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_SIZX, WFNOTE_FRIENDINFO_PAGE02_BA_LV50TT_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_X, WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_Y,
		WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_SIZX, WFNOTE_FRIENDINFO_PAGE02_BA_LV50NO_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_X, WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_Y,
		WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_SIZX, WFNOTE_FRIENDINFO_PAGE02_BA_LV100TT_SIZY,
	},
	{ 
		WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_X, WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_Y,
		WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_SIZX, WFNOTE_FRIENDINFO_PAGE02_BA_LV100NO_SIZY,
	},
};
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE03MSG[ WFNOTE_FRIENDINFO_PAGE03_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE03_BA_X, WFNOTE_FRIENDINFO_PAGE03_BA_Y,
		WFNOTE_FRIENDINFO_PAGE03_BA_SIZX, WFNOTE_FRIENDINFO_PAGE03_BA_SIZY,
	},
};
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE04MSG[ WFNOTE_FRIENDINFO_PAGE04_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE04_BA_X, WFNOTE_FRIENDINFO_PAGE04_BA_Y,
		WFNOTE_FRIENDINFO_PAGE04_BA_SIZX, WFNOTE_FRIENDINFO_PAGE04_BA_SIZY,
	},
};
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE05MSG[ WFNOTE_FRIENDINFO_PAGE05_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE05_BA_X, WFNOTE_FRIENDINFO_PAGE05_BA_Y,
		WFNOTE_FRIENDINFO_PAGE05_BA_SIZX, WFNOTE_FRIENDINFO_PAGE05_BA_SIZY,
	},
};
static const WFNOTE_FRIENDINFO_BMPDATA	sc_PAGE06MSG[ WFNOTE_FRIENDINFO_PAGE06_BA_NUM ] = {
	{ 
		WFNOTE_FRIENDINFO_PAGE06_BA_X, WFNOTE_FRIENDINFO_PAGE06_BA_Y,
		WFNOTE_FRIENDINFO_PAGE06_BA_SIZX, WFNOTE_FRIENDINFO_PAGE06_BA_SIZY,
	},
};

// ���ׂĂ܂Ƃ߂��e�[�u��
static const WFNOTE_FRIENDINFO_BMPDATA* scp_WFNOTE_FRIENDINFO_PAGEBMP[ WFNOTE_FRIENDINFO_PAGE_NUM ] = {
	sc_PAGE00MSG, sc_PAGE06MSG, sc_PAGE01MSG, sc_PAGE02MSG,
	sc_PAGE03MSG, sc_PAGE04MSG, sc_PAGE05MSG,
	
};
static const u8 sc_WFNOTE_FRIENDINFO_PAGEBMPNUM[ WFNOTE_FRIENDINFO_PAGE_NUM ] = {
	WFNOTE_FRIENDINFO_PAGE00_BA_NUM,
	WFNOTE_FRIENDINFO_PAGE06_BA_NUM,
	WFNOTE_FRIENDINFO_PAGE01_BA_NUM,
	WFNOTE_FRIENDINFO_PAGE02_BA_NUM,
	WFNOTE_FRIENDINFO_PAGE03_BA_NUM,
	WFNOTE_FRIENDINFO_PAGE04_BA_NUM,
	WFNOTE_FRIENDINFO_PAGE05_BA_NUM,
};
 






//----------------------------------------------------------------------------
/**
 *	@brief	�v���Z�X�@������
 *
 *	@param	proc		�v���Z�X���[�N
 *	@param	seq			�V�[�P���X
 *
 *	@retval PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WifiNoteProc_Init( PROC * proc, int * seq )
{
	WFNOTE_WK* p_wk;

	// wifi_2dchar���g�p���邽�߂�OVERLAY��LOAD
	Overlay_Load( FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);
	
	// ���[�N�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFINOTE, 0x70000 );
	p_wk = PROC_AllocWork( proc, sizeof(WFNOTE_WK), HEAPID_WIFINOTE );
	memset( p_wk, 0, sizeof(WFNOTE_WK) );

	// �f�[�^���[�N����
	WFNOTE_DATA_Init( &p_wk->data, PROC_GetParentWork(proc), HEAPID_WIFINOTE );

	// �`��V�X�e�����[�N������
	WFNOTE_DRAW_Init( &p_wk->draw, &p_wk->data, HEAPID_WIFINOTE );

	// �e��ʂ̃��[�N������
	WFNOTE_MODESELECT_Init( &p_wk->mode_select, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
	WFNOTE_FRIENDLIST_Init( &p_wk->friendlist, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
	WFNOTE_CODEIN_Init( &p_wk->codein, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
	WFNOTE_MYCODE_Init( &p_wk->mycode, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
	WFNOTE_FRIENDINFO_Init( &p_wk->friendinfo, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );

	// ���荞��
    sys_VBlankFuncChange( WFNOTE_VBlank, p_wk );
	sys_HBlankIntrStop();

	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���Z�X�@���C������
 *
 *	@param	proc		�v���Z�X���[�N
 *	@param	seq			�V�[�P���X
 *
 *	@retval PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WifiNoteProc_Main( PROC * proc, int * seq )
{
	WFNOTE_WK* p_wk  = PROC_GetWork( proc );
	WFNOTE_STRET stret;

	switch( *seq ){
	case WFNOTE_PROCSEQ_MAIN:
		
		switch( p_wk->status ){
		case WFNOTE_STATUS_MODE_SELECT:	// �ŏ��̑I�����
			stret = WFNOTE_MODESELECT_Main( &p_wk->mode_select, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
			break;
			
		case WFNOTE_STATUS_FRIENDLIST:	// �F�B�f�[�^�\��
			stret = WFNOTE_FRIENDLIST_Main( &p_wk->friendlist, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
			break;
			
		case WFNOTE_STATUS_CODEIN:		// �F�B�R�[�h����
			stret = WFNOTE_CODEIN_Main( &p_wk->codein, p_wk, &p_wk->data, &p_wk->draw, HEAPID_WIFINOTE );
			break;
			
		case WFNOTE_STATUS_MYCODE:		// �����̗F�B�R�[�h�m�F
			stret = WFNOTE_MYCODE_Main( &p_wk->mycode, &p_wk->data, &p_wk->draw );
			break;

		case WFNOTE_STATUS_FRIENDINFO:	// �F�B�ڍ׃f�[�^
			stret = WFNOTE_FRIENDINFO_Main( &p_wk->friendinfo, &p_wk->data, &p_wk->draw,  HEAPID_WIFINOTE );
			break;
			
		case WFNOTE_STATUS_END:			// �I��
			return PROC_RES_FINISH;
		}

		// ��Ԑ؂�ւ�����
		if( stret == WFNOTE_STRET_FINISH ){
			p_wk->status = p_wk->data.reqstatus;
			p_wk->data.subseq = p_wk->data.reqsubseq;
			p_wk->wait = WFNOTE_STATUS_CHANGE_WAIT;
			(*seq) = WFNOTE_PROCSEQ_WAIT;
		}
		break;

	case WFNOTE_PROCSEQ_WAIT:
		p_wk->wait --;
		if( p_wk->wait <= 0 ){
			(*seq) = WFNOTE_PROCSEQ_MAIN;
		}
		
		break;
	}


	// �`�惁�C��
	WFNOTE_DRAW_Main( &p_wk->draw );

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���Z�X�@�j��
 *
 *	@param	proc		�v���Z�X���[�N
 *	@param	seq			�V�[�P���X
 *
 *	@retval PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,	GF_BGL_BmpWinInit		///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WifiNoteProc_End( PROC * proc, int * seq )
{
	WFNOTE_WK* p_wk  = PROC_GetWork( proc );

	// ���荞��
    sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();

	// �e��ʂ̃��[�N�j��
	WFNOTE_MODESELECT_Exit( &p_wk->mode_select, &p_wk->data, &p_wk->draw );
	WFNOTE_FRIENDLIST_Exit( &p_wk->friendlist, &p_wk->data, &p_wk->draw );
	WFNOTE_CODEIN_Exit( &p_wk->codein, &p_wk->data, &p_wk->draw );
	WFNOTE_MYCODE_Exit( &p_wk->mycode, &p_wk->data, &p_wk->draw );
	WFNOTE_FRIENDINFO_Exit( &p_wk->friendinfo, &p_wk->data, &p_wk->draw );

	// �`��V�X�e�����[�N�j��
	WFNOTE_DRAW_Exit( &p_wk->draw );

	// �f�[�^���[�N�j��
	WFNOTE_DATA_Exit( &p_wk->data );

	PROC_FreeWork( proc );				// PROC���[�N�J��
	sys_DeleteHeap( HEAPID_WIFINOTE );

	// �I�[�o�[���C�j��
	Overlay_UnloadID( FS_OVERLAY_ID(wifi_2dmapsys) );

	return PROC_RES_FINISH;
}




//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�S�̊֐�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_work	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_VBlank( void* p_work )
{
	WFNOTE_WK* p_wk = p_work;

	// �`��VBLANK
	WFNOTE_DRAW_VBlank( &p_wk->draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	WFNOTE�@������
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DrawInit( WFNOTE_WK* p_wk, u32 heapID )
{
	// �`��V�X�e�����[�N������
	WFNOTE_DRAW_Init( &p_wk->draw, &p_wk->data, heapID );

	// �e��ʂ̃��[�N������
	WFNOTE_MODESELECT_DrawInit( &p_wk->mode_select, &p_wk->data, &p_wk->draw, heapID );
	WFNOTE_FRIENDLIST_DrawInit( &p_wk->friendlist, &p_wk->data, &p_wk->draw, heapID );
	WFNOTE_CODEIN_DrawInit( &p_wk->codein, &p_wk->data, &p_wk->draw, heapID );
	WFNOTE_MYCODE_DrawInit( &p_wk->mycode, &p_wk->data, &p_wk->draw, heapID );
	WFNOTE_FRIENDINFO_DrawInit( &p_wk->friendinfo, &p_wk->data, &p_wk->draw, heapID );

	// ���荞��
    sys_VBlankFuncChange( WFNOTE_VBlank, p_wk );
	sys_HBlankIntrStop();

#if PL_T0861_080712_FIX
	MsgPrintSkipFlagSet(MSG_SKIP_ON);
	MsgPrintAutoFlagSet(MSG_AUTO_OFF);
	MsgPrintTouchPanelFlagSet(MSG_TP_OFF);
#endif

}

//----------------------------------------------------------------------------
/**
 *	@brief	WFNOTE	�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DrawExit( WFNOTE_WK* p_wk )
{
	// ���荞��
    sys_VBlankFuncChange( NULL, NULL );
	sys_HBlankIntrStop();

	// �e��ʂ̃��[�N�j��
	WFNOTE_MODESELECT_DrawExit( &p_wk->mode_select, &p_wk->data, &p_wk->draw );
	WFNOTE_FRIENDLIST_DrawExit( &p_wk->friendlist, &p_wk->data, &p_wk->draw );
	WFNOTE_CODEIN_DrawExit( &p_wk->codein, &p_wk->data, &p_wk->draw );
	WFNOTE_MYCODE_DrawExit( &p_wk->mycode, &p_wk->data, &p_wk->draw );
	WFNOTE_FRIENDINFO_DrawExit( &p_wk->friendinfo, &p_wk->data, &p_wk->draw );

	// �`��V�X�e�����[�N�j��
	WFNOTE_DRAW_Exit( &p_wk->draw );
}


//-------------------------------------
///	�f�[�^���[�N�֐�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^���[�N������
 *
 *	@param	p_data		�f�[�^���[�N
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_Init( WFNOTE_DATA* p_data, void* p_save, u32 heapID )
{
	p_data->p_save = p_save;
	p_data->codein.p_name = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_data->codein.p_code = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^���[�N�j��
 *
 *	@param	p_data	�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_Exit( WFNOTE_DATA* p_data )
{
	STRBUF_Delete( p_data->codein.p_name );
	STRBUF_Delete( p_data->codein.p_code );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�X�e�[�^�X�ݒ�
 *
 *	@param	p_data		���[�N
 *	@param	reqstatus	���N�G�X�g�X�e�[�^�X
 *	@param	reqsubseq	���N�G�X�g�T�u�V�[�P���X
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_SetReqStatus( WFNOTE_DATA* p_data, u8 reqstatus, u8 reqsubseq )
{
	p_data->reqstatus = reqstatus;
	p_data->reqsubseq = reqsubseq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�f�[�^�ɃR�[�h���͉�ʂ̌��ʂ�ݒ肷��
 *
 *	@param	p_data		���[�N
 *	@param	cp_name		���O
 *	@param	cp_code		�R�[�h
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_CODEIN_Set( WFNOTE_DATA* p_data, const STRBUF* cp_name, const STRBUF* cp_code )
{
	if( cp_name ){
		STRBUF_Copy( p_data->codein.p_name, cp_name );
	}
	if( cp_code ){
		STRBUF_Copy( p_data->codein.p_code, cp_code );
	}
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�ŐV�̗F�B�f�[�^�ݒ�
 *
 *	@param	p_data		���[�N
 *	@param	p_code		�R�[�h������
 *	@param	p_name		���O������
 */
//-----------------------------------------------------------------------------
static u32 WFNOTE_DATA_NewFriendDataSet( WFNOTE_DATA* p_data, STRBUF* p_code, STRBUF* p_name )
{
	int i;
	WIFI_LIST* p_list;
	BOOL result;
	BOOL set_check;
	u64 friendcode;
	u64 mycode;
    DWCFriendData *p_dwc_list = NULL;
    DWCUserData *p_dwc_userdata = NULL;
    int ret;
	int pos;

	p_list = SaveData_GetWifiListData( p_data->p_save );

	mycode = DWC_CreateFriendKey( WifiList_GetMyUserInfo(p_list) ); 

	for( i=0; i<WIFILIST_FRIEND_MAX; i++ ){
		result = WifiList_IsFriendData( p_list, i );
		if( result == FALSE ){
			friendcode = STRBUF_GetNumber(p_code, &set_check);	

			if( (set_check == TRUE) && (friendcode !=mycode) ){
				
				p_dwc_userdata = WifiList_GetMyUserInfo( p_list );
				if(!DWC_CheckFriendKey(p_dwc_userdata, friendcode)){
					// �F�B�R�[�h���Ⴄ
					return WFNOTE_DATA_NEWFRIENDSET_FRIENDKEYNG;
				}
				ret = dwc_checkFriendCodeByToken(p_data->p_save, friendcode, &pos);
				if(ret == DWCFRIEND_INLIST){	
					// �����ݒ肸��
					return WFNOTE_DATA_NEWFRIENDSET_SETING;
				}
				// ����
				p_dwc_list = WifiList_GetDwcDataPtr(p_list, i);
				DWC_CreateFriendKeyToken(p_dwc_list, friendcode);
				// ���O�Ɛ��ʐݒ�	�����Őݒ�
				WifiList_SetFriendName(p_list, i, p_name);
				WifiList_SetFriendInfo(p_list, i, WIFILIST_FRIEND_SEX, PM_NEUTRAL);
				return WFNOTE_DATA_NEWFRIENDSET_OK;
			}else{
				// �F�B�R�[�h���Ⴄ
				return WFNOTE_DATA_NEWFRIENDSET_FRIENDKEYNG;
			}
		}
	}

	GF_ASSERT(0);
	// �����͂Ƃ���Ȃ��͂�
	return WFNOTE_DATA_NEWFRIENDSET_OK;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O����	�������f�[�^�ݒ�
 *
 *	@param	p_data		�f�[�^
 *	@param	cp_str		������
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_NAMEIN_INIT_Set( WFNOTE_DATA* p_data, const STRCODE* cp_str )
{
	p_data->namein_init.cp_str = cp_str;
	p_data->namein_init.init = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I���������X�g�C���f�b�N�X��ݒ肷��
 *
 *	@param	p_data		�f�[�^
 *	@param	idx			�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_SELECT_ListIdxSet( WFNOTE_DATA* p_data, u32 idx )
{
	p_data->select_listidx = idx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B���X�g�쐬
 *		
 *	@param	p_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DATA_FrIdxMake( WFNOTE_DATA* p_data )
{
	int i;
	WIFI_LIST* p_list;

	memset( &p_data->idx, 0, sizeof(WFNOTE_IDXDATA) );

	p_list = SaveData_GetWifiListData( p_data->p_save );

	p_data->idx.friendnum = 0;
	for( i=0; i<WFNOTE_FRIENDLIST_FRIENDMAX; i++ ){
		if( WifiList_IsFriendData( p_list, i ) == TRUE ){
			p_data->idx.fridx[ p_data->idx.friendnum ] = i;
			p_data->idx.friendnum ++;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	WIFI���X�g�e�[�u������F�B�ԍ����擾����
 *
 *	@param	cp_data		�f�[�^
 *	@param	idx			���X�g�C���f�b�N�X
 *
 *	@return	�F�B�ԍ�
 */
//-----------------------------------------------------------------------------
static u32 WFNOTE_DATA_FrIdxGetIdx( const WFNOTE_DATA* cp_data, u32 idx )
{
	return cp_data->idx.fridx[ idx ];
}



//-------------------------------------
///	�`�惏�[�N�֐�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�`�惏�[�N������
 *
 *	@param	p_draw		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_Init( WFNOTE_DRAW* p_draw, const WFNOTE_DATA* cp_data, u32 heapID )
{
	// �܂��s������
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();
	p_draw->p_handle = ArchiveDataHandleOpen( ARC_WIFINOTE_GRA, heapID );

	WFNOTE_DRAW_BankInit();
	WFNOTE_DRAW_BgInit( p_draw, heapID );
	WFNOTE_DRAW_ClactInit( p_draw, heapID );
	WFNOTE_DRAW_MsgInit( p_draw, heapID );
	WFNOTE_DRAW_BmpInit( p_draw, cp_data, heapID );
	WFNOTE_DRAW_YazirushiInit( p_draw, heapID );
	WFNOTE_DRAW_SCRNDATA_Init( p_draw, heapID );

	// OAM�ʕ\��
    GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�惏�[�N���C��
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_Main( WFNOTE_DRAW* p_draw )
{
	CLACT_Draw( p_draw->p_clact );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�惏�[�NV�u�����N
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_VBlank( WFNOTE_DRAW* p_draw )
{
	GF_BGL_VBlankFunc( p_draw->p_bgl );
	DoVramTransferManager();
	REND_OAMTrans();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�惏�[�N�@�j��
 *
 *	@param	p_draw	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_Exit( WFNOTE_DRAW* p_draw )
{
	WFNOTE_DRAW_YazirushiExit( p_draw );
	WFNOTE_DRAW_SCRNDATA_Exit( p_draw );
	WFNOTE_DRAW_BmpExit( p_draw );
	WFNOTE_DRAW_MsgExit( p_draw );
	WFNOTE_DRAW_ClactExit( p_draw );
	WFNOTE_DRAW_BgExit( p_draw );

	ArchiveDataHandleClose( p_draw->p_handle );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���N�ݒ�
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BankInit( void )
{
    GF_BGL_DISPVRAM tbl = {
		GX_VRAM_BG_256_AB,				// ���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g

		GX_VRAM_SUB_BG_128_C,			// �T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g

		GX_VRAM_OBJ_80_EF,				// ���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g

		GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g

		GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
	};
	GF_Disp_SetBank( &tbl );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG������
 *
 *	@param	p_draw		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BgInit( WFNOTE_DRAW* p_draw, u32 heapID )
{
	int i;
	
	p_draw->p_bgl = GF_BGL_BglIniAlloc( heapID );

    // BG SYSTEM
	{	
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	// BG�R���g���[���ݒ�
	for( i=0; i<WFNOTE_BG_NUM; i++ ){
		GF_BGL_BGControlSet( p_draw->p_bgl, sc_WFNOTE_BGFRM[i], &sc_WFNOTE_BGCNT[i], GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( sc_WFNOTE_BGFRM[i], 32, 0, heapID );
		GF_BGL_ScrClear( p_draw->p_bgl, sc_WFNOTE_BGFRM[i] );
	}

	// ��{�I�ȃo�b�N�O���E���h�Ǎ��݁��]��
	// �L�����N�^�f�[�^�́A�X�N���[���ʂɂ��]��
	ArcUtil_HDL_PalSet( p_draw->p_handle, NARC_pl_wifinote_techo_NCLR, PALTYPE_MAIN_BG,
			WFNOTE_BGPL_MAIN_BACK_0, WFNOTE_BGPL_MAIN_BACK_NUM*32, heapID );
	ArcUtil_HDL_PalSet( p_draw->p_handle, NARC_pl_wifinote_techo_NCLR, PALTYPE_SUB_BG,
			WFNOTE_BGPL_SUB_BACK_0, WFNOTE_BGPL_SUB_BACK_NUM*32, heapID );
	
	ArcUtil_HDL_BgCharSet( p_draw->p_handle, NARC_pl_wifinote_techo_NCGR, 
			p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ], 0, 0, FALSE, heapID );
	ArcUtil_HDL_BgCharSet( p_draw->p_handle, NARC_pl_wifinote_techo_NCGR, 
			p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ], 0, 0, FALSE, heapID );
	ArcUtil_HDL_BgCharSet( p_draw->p_handle, NARC_pl_wifinote_techo_sub_NCGR, 
			p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_SUB_BACK ], 0, 0, FALSE, heapID );
	
	ArcUtil_HDL_ScrnSet( p_draw->p_handle, NARC_pl_wifinote_techo_main_NSCR,
			p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ], 0, 0, FALSE, heapID );
	ArcUtil_HDL_ScrnSet( p_draw->p_handle, NARC_pl_wifinote_techo_sub_NSCR,
			p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_SUB_BACK ], 0, 0, FALSE, heapID );


}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�j��
 *
 *	@param	p_draw		���[�N 
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BgExit( WFNOTE_DRAW* p_draw )
{
	int i;
	
	// BG�R���g���[���j��
	for( i=0; i<WFNOTE_BG_NUM; i++ ){
		GF_BGL_BGControlExit( p_draw->p_bgl, sc_WFNOTE_BGFRM[i] );
	}

	sys_FreeMemoryEz( p_draw->p_bgl );
	p_draw->p_bgl = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[������
 *
 *	@param	p_draw		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_ClactInit( WFNOTE_DRAW* p_draw, u32 heapID )
{
	int i;
	BOOL result;
	
	// OAM�}�l�[�W���[�̏�����
	NNS_G2dInitOamManagerModule();

	// Vram�]���}�l�[�W��������
	initVramTransferManagerHeap( VRAMTR_MAN_NUM, heapID );

	// ���LOAM�}�l�[�W���쐬
	// �����_���pOAM�}�l�[�W���쐬
	// �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
	REND_OAMInit( 
			0, 126,		// ���C�����OAM�Ǘ��̈�
			0, 30,		// ���C����ʃA�t�B���Ǘ��̈�
			0, 126,		// �T�u���OAM�Ǘ��̈�
			0, 30,		// �T�u��ʃA�t�B���Ǘ��̈�
			heapID );
	
	// �L�����N�^�}�l�[�W���[������
	{
		CHAR_MANAGER_MAKE cm = {
			CLACT_RESNUM,
			CLACT_MAIN_VRAM_SIZ,
			CLACT_SUB_VRAM_SIZ,
			0
		};
		cm.heap = heapID;
		InitCharManagerReg(&cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K );
	}
	// �p���b�g�}�l�[�W���[������
	InitPlttManager(CLACT_RESNUM, heapID);

	// �ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();
	
	
	// �Z���A�N�^�[������
	p_draw->p_clact = CLACT_U_SetEasyInit( CLACT_WKNUM, &p_draw->renddata, heapID );
	
	// �X�N���[���p�T�[�t�F�[�X�̍쐬
	REND_OAM_SetSurface( &p_draw->scrll_surface, &s_SCRLLSURFACE_RECT,
			NNS_G2D_SURFACETYPE_MAIN2D, &p_draw->renddata.Rend );
	
	//���\�[�X�}�l�[�W���[������
	for(i=0;i<CLACT_RESOURCE_NUM;i++){		//���\�[�X�}�l�[�W���[�쐬
		p_draw->p_resman[i] = CLACT_U_ResManagerInit(CLACT_RESNUM, i, heapID);	
	}


	// ��{�I�ȃ��\�[�X��ǂݍ���
	p_draw->p_resobj[0] = CLACT_U_ResManagerResAddArcChar_ArcHandle( 
			p_draw->p_resman[0], p_draw->p_handle, NARC_pl_wifinote_techo_obj_NCGR,
			FALSE, WFNOTE_OAM_COMM_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );
	p_draw->p_resobj[1] = CLACT_U_ResManagerResAddArcPltt_ArcHandle( 
			p_draw->p_resman[1], p_draw->p_handle, NARC_pl_wifinote_techo_NCLR,
			FALSE, WFNOTE_OAM_COMM_CONTID, NNS_G2D_VRAM_TYPE_2DMAIN, 5, heapID );
	p_draw->p_resobj[2] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
			p_draw->p_resman[2], p_draw->p_handle, NARC_pl_wifinote_techo_NCER,
			FALSE, WFNOTE_OAM_COMM_CONTID, CLACT_U_CELL_RES, heapID );
	p_draw->p_resobj[3] = CLACT_U_ResManagerResAddArcKindCell_ArcHandle( 
			p_draw->p_resman[3], p_draw->p_handle, NARC_pl_wifinote_techo_NANR,
			FALSE, WFNOTE_OAM_COMM_CONTID, CLACT_U_CELLANM_RES, heapID );

	// VRAM�]��
	result = CLACT_U_CharManagerSetAreaCont( p_draw->p_resobj[0] );	
	GF_ASSERT( result );
	result = CLACT_U_PlttManagerSetCleanArea( p_draw->p_resobj[1] );	
	GF_ASSERT( result );
	CLACT_U_ResManagerResOnlyDelete( p_draw->p_resobj[0] );
	CLACT_U_ResManagerResOnlyDelete( p_draw->p_resobj[1] );

	// �w�b�_�[�쐬
	CLACT_U_MakeHeader( &p_draw->clheader, WFNOTE_OAM_COMM_CONTID, WFNOTE_OAM_COMM_CONTID, 
			WFNOTE_OAM_COMM_CONTID, WFNOTE_OAM_COMM_CONTID, CLACT_U_HEADER_DATA_NONE, 
			CLACT_U_HEADER_DATA_NONE, FALSE, 0,
			p_draw->p_resman[0], p_draw->p_resman[1], p_draw->p_resman[2], p_draw->p_resman[3],
			NULL, NULL );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�j��
 *
 *	@param	p_draw	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_ClactExit( WFNOTE_DRAW* p_draw )
{
	int i;

	// ��{OAM���\�[�X�j��
	CLACT_U_CharManagerDelete( p_draw->p_resobj[0] );
	CLACT_U_PlttManagerDelete( p_draw->p_resobj[1] );
	for( i=0; i<CLACT_RESOURCE_NUM; i++ ){
		CLACT_U_ResManagerResDelete( p_draw->p_resman[i], p_draw->p_resobj[i] );
	}
	
	// ���\�[�X�}�l�[�W���j��
	for(i=0;i<CLACT_RESOURCE_NUM;i++){		//���\�[�X�}�l�[�W���[�쐬
		CLACT_U_ResManagerDelete( p_draw->p_resman[i] );	
	}
	
	// �Z���A�N�^�[�Z�b�g�j��
	CLACT_DestSet( p_draw->p_clact );

	// �L�����N�^�}�l�[�W���j��
	DeleteCharManager();
	
	// �p���b�g�}�l�[�W���j��
	DeletePlttManager();
	
	// �����_���[�j��
	REND_OAM_Delete();
	
	// Vram�]���}�l�[�W���j��
	DellVramTransferManager();
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�@������
 *
 *	@param	p_draw		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_MsgInit( WFNOTE_DRAW* p_draw, u32 heapID )
{
    p_draw->p_wordset = WORDSET_Create( heapID );
    p_draw->p_msgman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_note_dat, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�@�j��
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_MsgExit( WFNOTE_DRAW* p_draw )
{
	MSGMAN_Delete( p_draw->p_msgman );
	WORDSET_Delete( p_draw->p_wordset );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�֘A������
 *
 *	@param	p_draw
 *	@param	cp_data
 *	@param	heapID 
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BmpInit( WFNOTE_DRAW* p_draw, const WFNOTE_DATA* cp_data, u32 heapID )
{
	int type;

	// ��{�E�B���h�E�f�[�^�Ǎ���
	type = CONFIG_GetWindowType(SaveData_GetConfig(cp_data->p_save));
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, WFNOTE_BGPL_MAIN_MSGFONT*0x20, heapID );
	TalkWinGraphicSet(
		p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_MSG], WFNOTE_BGCGX_MAIN_MSG_TALK,
		WFNOTE_BGPL_MAIN_TALKWIN,  type, heapID );
	MenuWinGraphicSet(
		p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_MSG], WFNOTE_BGCGX_MAIN_MSG_MENU,
		WFNOTE_BGPL_MAIN_MENUWIN, 0, heapID );
	
	// ��{�r�b�g�}�b�v
	GF_BGL_BmpWinInit( &p_draw->title );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_draw->title, 
			sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_MSG], 
			WFNOTE_BG_TITLEBMP_X, WFNOTE_BG_TITLEBMP_Y, 
			WFNOTE_BG_TITLEBMP_SIZX, WFNOTE_BG_TITLEBMP_SIZY,
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_BG_TITLEBMP_CGX );
	
	// �^�C�g���p������o�b�t�@�쐬
	p_draw->p_titlestr = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_draw->p_titletmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
}

//----------------------------------------------------------------------------
/**	
 *	@brief	�r�b�g�}�b�v�j��
 *
 *	@param	p_draw	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BmpExit( WFNOTE_DRAW* p_draw )
{
	STRBUF_Delete( p_draw->p_titlestr );
	STRBUF_Delete( p_draw->p_titletmp );
	GF_BGL_BmpWinDel( &p_draw->title );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��{�X�N���[���f�[�^	������
 *
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_SCRNDATA_Init( WFNOTE_DRAW* p_draw, u32 heapID )
{
	static const u8 sc_SCRNDATA[ WFNOTE_SCRNDATA_NUM ] = {
		NARC_pl_wifinote_techo_02_NSCR,
		NARC_pl_wifinote_techo_02a_NSCR,
	};
	int i;
	// �X�N���[���f�[�^������
	for( i=0; i<WFNOTE_SCRNDATA_NUM; i++ ){	
		p_draw->scrn.p_scrnbuff[i] = ArcUtil_HDL_ScrnDataGet( p_draw->p_handle, 
				sc_SCRNDATA[i], FALSE, &p_draw->scrn.p_scrn[i], heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^�j��
 *
 *	@param	p_draw	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_SCRNDATA_Exit( WFNOTE_DRAW* p_draw )
{
	int i;

	// �X�N���[���f�[�^������
	for( i=0; i<WFNOTE_SCRNDATA_NUM; i++ ){	
		sys_FreeMemoryEz( p_draw->scrn.p_scrnbuff[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���̕`��
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	msg_idx	���b�Z�[�WID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BmpTitleWrite( WFNOTE_DRAW* p_draw, u32 msg_idx )
{
	// �^�X�N�����~
	if( GF_MSG_PrintEndCheck( p_draw->title_stridx ) == 1 ){
		GF_STR_PrintForceStop( p_draw->title_stridx );
	}
	
	// �N���A
	GF_BGL_BmpWinDataFill( &p_draw->title, 0 );

	MSGMAN_GetString( p_draw->p_msgman, msg_idx, p_draw->p_titletmp );
	WORDSET_ExpandStr( p_draw->p_wordset, p_draw->p_titlestr, p_draw->p_titletmp );
    p_draw->title_stridx = GF_STR_PrintColor(&p_draw->title, FONT_TALK, 
			p_draw->p_titlestr, 0, WFNOTE_BG_TITLEBMP_PL_Y, WFNOTE_TITLE_MSG_SPEED, 
			WFNOTE_COL_WHITE, NULL);

    GF_BGL_BmpWinOnVReq(&p_draw->title);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�^�C�g���\��Off
 *
 *	@param	p_draw		���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_BmpTitleOff( WFNOTE_DRAW* p_draw )
{
	// �^�X�N�����~
	if( GF_MSG_PrintEndCheck( p_draw->title_stridx ) == 1 ){
		GF_STR_PrintForceStop( p_draw->title_stridx );
	}

	GF_BGL_BmpWinOffVReq(&p_draw->title);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�R�[�h�����[�h�Z�b�g�ɐݒ�
 *
 *	@param	p_draw	�`��V�X�e��
 *	@param	code	�R�[�h
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_FriendCodeSetWordset( WFNOTE_DRAW* p_draw, u64 code )
{
    u64 num;

    num = code / 100000000;
    WORDSET_RegisterNumber(p_draw->p_wordset, 1, num, 4,NUMBER_DISPTYPE_ZERO,NUMBER_CODETYPE_DEFAULT);
    num = (code/10000) % 10000;
    WORDSET_RegisterNumber(p_draw->p_wordset, 2, num, 4,NUMBER_DISPTYPE_ZERO,NUMBER_CODETYPE_DEFAULT);
    num = code % 10000;
    WORDSET_RegisterNumber(p_draw->p_wordset, 3, num, 4,NUMBER_DISPTYPE_ZERO,NUMBER_CODETYPE_DEFAULT);

}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�̖��O��ݒ肷��
 *
 *	@param	p_draw		�`��V�X�e��
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	idx			�F�B�̃C���f�b�N�X
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_FriendNameSetWordset( WFNOTE_DRAW* p_draw, SAVEDATA* p_save, u32 idx, u32 heapID )
{
	WIFI_LIST* p_wifilist;
	MYSTATUS* p_mystatus;

	p_wifilist = SaveData_GetWifiListData( p_save );
	p_mystatus = MyStatus_AllocWork( heapID );	
	MyStatus_SetMyName( p_mystatus, WifiList_GetFriendNamePtr(p_wifilist, idx) );
	WORDSET_RegisterPlayerName( p_draw->p_wordset, 0, p_mystatus );
	sys_FreeMemoryEz( p_mystatus );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�̃O���[�v�����[�h�Z�b�g�ɐݒ肷��
 *
 *	@param	p_draw		�`��V�X�e��
 *	@param	p_save		�f�[�^
 *	@param	idx			�C���f�b�N�X
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_FriendGroupSetWordset( WFNOTE_DRAW* p_draw, SAVEDATA* p_save, u32 idx, u32 heapID )
{
	MYSTATUS* p_mystatus = MyStatus_AllocWork( heapID );
	WIFI_LIST* p_wifilist = SaveData_GetWifiListData( p_save );
	MyStatus_SetMyName(p_mystatus, WifiList_GetFriendGroupNamePtr( p_wifilist, idx ));
	WORDSET_RegisterPlayerName( p_draw->p_wordset, 0, p_mystatus );
	sys_FreeMemoryEz( p_mystatus );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�ƍŌ�ɗV�񂾓��ɂ���ݒ�
 *
 *	@param	p_draw		�`��V�X�e��
 *	@param	p_save		�Z�[�u�f�[�^
 *	@param	idx			�F�B���X�g�C���f�b�N�X
 *
 *	@retval	TRUE	���ɂ���������
 *	@retval	FALSE	���ɂ����Ȃ�����
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_DRAW_FriendDaySetWordset( WFNOTE_DRAW* p_draw, SAVEDATA* p_save, u32 idx )
{
	WIFI_LIST* p_wifilist = SaveData_GetWifiListData( p_save );
	u32 num;
	BOOL ret = TRUE;

	num = WifiList_GetFriendInfo( p_wifilist, idx, WIFILIST_FRIEND_LASTBT_DAY);
	if( num == 0 ){
		ret = FALSE;	// �O�Ƃ������Ƃ͂P����V��ł��Ȃ�
	}
	WORDSET_RegisterNumber( p_draw->p_wordset, 2, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
	num = WifiList_GetFriendInfo( p_wifilist, idx, WIFILIST_FRIEND_LASTBT_YEAR);
	WORDSET_RegisterNumber( p_draw->p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
	num = WifiList_GetFriendInfo( p_wifilist, idx, WIFILIST_FRIEND_LASTBT_MONTH);
	WORDSET_RegisterNumber( p_draw->p_wordset, 1, num, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);

	return ret;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�ɐ�����ݒ肷��
 *
 *	@param	p_draw		�`��V�X�e��
 *	@param	num			����
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_NumberSetWordset( WFNOTE_DRAW* p_draw, u32 num )
{
	WORDSET_RegisterNumber( p_draw->p_wordset, 0, num, 4, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�Z�b�g�Ƀ��r�[�~�j�Q�[������ݒ�
 *
 *	@param	p_draw
 *	@param	num 
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_WflbyGameSetWordSet( WFNOTE_DRAW* p_draw, u32 num )
{
	WORDSET_RegisterWiFiLobbyGameName( p_draw->p_wordset, 0, num );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��󏉊���
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_YazirushiInit( WFNOTE_DRAW* p_draw, u32 heapID )
{
	static CLACT_ADD yazirushiadd[WFNOTE_YAZIRUSHINUM] = {
		{	// ��
			NULL, NULL,
			{ WFNOTE_YAZIRUSHI_XLEF, WFNOTE_YAZIRUSHI_Y, 0 },
			{ FX32_ONE, FX32_ONE, FX32_ONE },
			0, WFNOTE_YAZIRUSHI_PRI,
			NNS_G2D_VRAM_TYPE_2DMAIN, 0
		},
		{	// ��
			NULL, NULL,
			{ WFNOTE_YAZIRUSHI_XRIGHT, WFNOTE_YAZIRUSHI_Y, 0 },
			{ FX32_ONE, FX32_ONE, FX32_ONE },
			0, WFNOTE_YAZIRUSHI_PRI,
			NNS_G2D_VRAM_TYPE_2DMAIN, 0
		},
	};
	int i;

	// ���A�N�^�[�쐬
	for( i=0; i<WFNOTE_YAZIRUSHINUM; i++ ){
		yazirushiadd[i].ClActSet = p_draw->p_clact;
		yazirushiadd[i].ClActHeader = &p_draw->clheader;
		yazirushiadd[i].heap	= heapID;
		p_draw->p_yazirushi[i] = CLACT_Add( &yazirushiadd[i] );
		CLACT_SetAnmFlag( p_draw->p_yazirushi[i], TRUE );
		CLACT_SetDrawFlag( p_draw->p_yazirushi[i], FALSE );
		CLACT_AnmChg( p_draw->p_yazirushi[i],  i );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j��
 *
 *	@param	p_draw	�`�惏�[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_YazirushiExit( WFNOTE_DRAW* p_draw )
{
	int i;
	// ���A�N�^�[�j��
	for( i=0; i<WFNOTE_YAZIRUSHINUM; i++ ){
		CLACT_Delete( p_draw->p_yazirushi[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���\���ݒ�
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	flag	�t���O
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_YazirushiSetDrawFlag( WFNOTE_DRAW* p_draw, BOOL flag )
{
	int i;
	for( i=0; i<WFNOTE_YAZIRUSHINUM; i++ ){
		CLACT_SetDrawFlag( p_draw->p_yazirushi[i], flag );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���A�j���ݒ�
 *
 *	@param	p_draw	�`�惏�[�N
 *	@param	flag	�t���O
 */
//-----------------------------------------------------------------------------
static void WFNOTE_DRAW_YazirushiSetAnmFlag( WFNOTE_DRAW* p_draw, BOOL flag )
{
	int i;
	for( i=0; i<WFNOTE_YAZIRUSHINUM; i++ ){
		CLACT_SetAnmFlag( p_draw->p_yazirushi[i], flag );
	}
}



//-------------------------------------
///	�I�����[�h�֐�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�I����ʁ@���[�N������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`�惏�[�N	
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_Init( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	WFNOTE_MODESELECT_DrawInit( p_wk, p_data, p_draw, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�I�����	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	WFNOTE_STRET_CONTINUE,	// �Â�
 *	@retval	WFNOTE_STRET_FINISH,	// ��Ƃ͏I�����
 */
//-----------------------------------------------------------------------------
static WFNOTE_STRET WFNOTE_MODESELECT_Main( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	BOOL result;
	
	switch( p_data->subseq ){
	case WFNOTE_MODESELECT_SEQ_INIT:	// ������
		WFNOTE_MODESELECT_SEQ_Init( p_wk, p_data, p_draw );
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_HOLEIN, WIPE_TYPE_HOLEIN, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID);
		p_data->subseq = WFNOTE_MODESELECT_SEQ_FADEINWAIT;
		break;
		
	case WFNOTE_MODESELECT_SEQ_FADEINWAIT:
		if( WIPE_SYS_EndCheck() ){
			p_data->subseq = WFNOTE_MODESELECT_SEQ_MAIN;
		}
		break;

	case WFNOTE_MODESELECT_SEQ_INIT_NOFADE:
		WFNOTE_MODESELECT_SEQ_Init( p_wk, p_data, p_draw );
		p_data->subseq = WFNOTE_MODESELECT_SEQ_MAIN;
		break;

	case WFNOTE_MODESELECT_SEQ_MAIN:
		result = WFNOTE_MODESELECT_SEQ_Main( p_wk, p_data, p_draw );
		if( result == TRUE ){

			// ���̏�ԂɑJ�ڂ��đ��v���`�F�b�N
			if( WFNOTE_MODESELECT_StatusChengeCheck( p_wk, p_data ) == TRUE ){

				// �J�[�\���ʒu�ɑΉ������I���ݒ���s��
				WFNOTE_MODESELECT_EndDataSet( p_wk, p_data );
				
				// �I���`�F�b�N
				if( (p_wk->cursor == WFNOTE_MODESELECT_CURSOR_END) ||
					(p_wk->cursor == WFNOTE_MODESELECT_CURSOR_CODEIN) ){
					p_data->subseq = WFNOTE_MODESELECT_SEQ_FADEOUT;
				}else{
					WFNOTE_MODESELECT_DrawOff( p_wk, p_draw );
					return WFNOTE_STRET_FINISH;
				}
			}else{

				if( p_wk->cursor == WFNOTE_MODESELECT_CURSOR_CODEIN ){
					// ���b�Z�[�W��������
					// ���b�Z�[�W�I���҂�����
					WFNOTE_MODESELECT_TalkMsgPrint( p_wk, p_draw, msg_wifi_note_30, heapID );
					p_data->subseq = WFNOTE_MODESELECT_SEQ_CODECHECK_RESLUTMSGWAIT;
				}
			}
		}
		break;


		// ���̂܂܏I������Ƃ�
	case WFNOTE_MODESELECT_SEQ_FADEOUT:
        WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_HOLEOUT, WIPE_TYPE_HOLEOUT, WIPE_FADE_BLACK, 
                        WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID);

		p_data->subseq ++;
		break;
		
	case WFNOTE_MODESELECT_SEQ_FADEOUTWAIT:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return WFNOTE_STRET_FINISH;
		}
		break;

		// �R�[�h���͉�ʂ���̕��A
	case WFNOTE_MODESELECT_SEQ_CODECHECK_INIT:
		WFNOTE_MODESELECT_SEQ_Init( p_wk, p_data, p_draw );
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_HOLEIN, WIPE_TYPE_HOLEIN, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID);
		p_data->subseq = WFNOTE_MODESELECT_SEQ_CODECHECK_FADEINWAIT;
		break;
		
	case WFNOTE_MODESELECT_SEQ_CODECHECK_FADEINWAIT:
		if( WIPE_SYS_EndCheck() ){
			u64 code;
			MYSTATUS* p_friend;
			// ���b�Z�[�W�̕\��
			// ���O��ݒ�
			p_friend = MyStatus_AllocWork( heapID );
			MyStatus_SetMyName(p_friend, STRBUF_GetStringCodePointer(p_data->codein.p_name));
			WORDSET_RegisterPlayerName( p_draw->p_wordset, 0, p_friend );
			sys_FreeMemoryEz( p_friend );

			// �F�B�R�[�h�ݒ�
			code = STRBUF_GetNumber(p_data->codein.p_code, &result);
			if( result ){	// �����ɕϊ��ł�����
				WFNOTE_DRAW_FriendCodeSetWordset( p_draw, code );

				WFNOTE_MODESELECT_TalkMsgPrint( p_wk, p_draw, msg_wifi_note_26, heapID );
				p_data->subseq = WFNOTE_MODESELECT_SEQ_CODECHECK_MSGWAIT;
			}else{

				// �R�[�h���Ԉ���Ă���ƕ\��
				WFNOTE_MODESELECT_TalkMsgPrint( p_wk, p_draw, msg_wifi_note_27, heapID );
				p_data->subseq = WFNOTE_MODESELECT_SEQ_CODECHECK_RESLUTMSGWAIT;  // �Ƃ������R�[�h�Ⴄ�\��
			}
		}
		break;
		
	case WFNOTE_MODESELECT_SEQ_CODECHECK_MSGWAIT:		//�o�^�m�F���b�Z�[�WON
		if( WFNOTE_MODESELECT_TalkMsgEndCheck( p_wk ) == FALSE ){
			break;
		}

		// �͂��������E�B���h�E�\��
        p_wk->p_yesno =
            BmpYesNoSelectInit( p_draw->p_bgl,
                                &WFNOTE_MODESELECT_YESNOWIN,
                                WFNOTE_BGCGX_MAIN_MSG_MENU, WFNOTE_BGPL_MAIN_MENUWIN,
                                heapID );
        p_data->subseq = WFNOTE_MODESELECT_SEQ_CODECHECK_MSGYESNOWAIT;
		break;
		
	case WFNOTE_MODESELECT_SEQ_CODECHECK_MSGYESNOWAIT:	//�͂��������I��҂�
		{
			int ret;
			u32 set_ret;
			static const u8 msgdata[ WFNOTE_DATA_NEWFRIENDSET_NUM ] = {
				0,
				msg_wifi_note_27,
				msg_wifi_note_31,
			};
			

			ret = BmpYesNoSelectMain( p_wk->p_yesno, heapID );

			switch( ret ){
			case BMPMENU_NULL:	// ���������ĂȂ�
				break;
				
			case BMPMENU_CANCEL:	// ������
				// ���b�Z�[�W�������ďI���
				WFNOTE_MODESELECT_TalkMsgOff( p_wk );
				p_data->subseq = WFNOTE_MODESELECT_SEQ_MAIN;
				break;
				
			default:		// �͂�
				// �o�^��Ƃ����Č��ʂ�\��
				set_ret = WFNOTE_DATA_NewFriendDataSet( p_data, p_data->codein.p_code, p_data->codein.p_name );
				if( set_ret == WFNOTE_DATA_NEWFRIENDSET_OK ){
					// ���������̂ł��̂܂܏I���
					WFNOTE_MODESELECT_TalkMsgOff( p_wk );
					p_data->subseq = WFNOTE_MODESELECT_SEQ_MAIN;
				}else{
					WFNOTE_MODESELECT_TalkMsgPrint( p_wk, p_draw, msgdata[set_ret], heapID );
					p_data->subseq = WFNOTE_MODESELECT_SEQ_CODECHECK_RESLUTMSGWAIT;  // �Ƃ������R�[�h�Ⴄ�\��
				}
				break;
			}
		}
		break;
		
	case WFNOTE_MODESELECT_SEQ_CODECHECK_RESLUTMSGWAIT:	//�I�����ʕ\��
		if( WFNOTE_MODESELECT_TalkMsgEndCheck( p_wk ) == FALSE ){
			break;
		}
		if( sys.trg & PAD_BUTTON_DECIDE ){
			WFNOTE_MODESELECT_TalkMsgOff( p_wk );
			p_data->subseq = WFNOTE_MODESELECT_SEQ_MAIN;
		}
		break;
	}
	return WFNOTE_STRET_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�I�����	���[�N�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_Exit( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	WFNOTE_MODESELECT_DrawExit( p_wk, p_data, p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֌W�̏�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_DrawInit( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	int i;
	static const u8 sc_msgidx[ 4 ] = {	// msg_idx
		msg_wifi_note_02,
		msg_wifi_note_03,
		msg_wifi_note_04,
		msg_wifi_note_05,
	};
	static const u8 sc_pos[4][2] = {	// ���W
		{ 40, 8 },
		{ 32, 48 },
		{ 28, 88 },
		{ 80, 128 },
	};

	
	// �r�b�g�}�b�v�E�B���h�E�쐬
	GF_BGL_BmpWinInit( &p_wk->msg );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->msg, 
			sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG], 
			WFNOTE_MODESELECT_MSG_X, WFNOTE_MODESELECT_MSG_Y, 
			WFNOTE_MODESELECT_MSG_SIZX, WFNOTE_MODESELECT_MSG_SIZY, 
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_MODESELECT_CGX );

	GF_BGL_BmpWinInit( &p_wk->talk );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->talk, 
			sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_MSG], 
			WFNOTE_MODESELECT_TALK_X, WFNOTE_MODESELECT_TALK_Y, 
			WFNOTE_MODESELECT_TALK_SIZX, WFNOTE_MODESELECT_TALK_SIZY, 
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_MODESELECT_TALKCGX );

	// �N���[��
	GF_BGL_BmpWinDataFill( &p_wk->msg, 0 );
	GF_BGL_BmpWinDataFill( &p_wk->talk, 0 );

	// ��b�E�B���h�E�p���b�Z�[�W�o�b�t�@�m��
	p_wk->p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// ���b�Z�[�W�X�s�[�h�擾
	p_wk->msg_speed = CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(p_data->p_save)); 

	// ���b�Z�[�W��������
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �����̖��O�����[�h�Z�b�g�ɐݒ�
	WORDSET_RegisterPlayerName( p_draw->p_wordset, 0, SaveData_GetMyStatus( p_data->p_save ) );
	
	for( i=0; i<4; i++ ){
		MSGMAN_GetString( p_draw->p_msgman, sc_msgidx[i], p_tmp );
		WORDSET_ExpandStr( p_draw->p_wordset, p_str, p_tmp );
		GF_STR_PrintColor( &p_wk->msg, FONT_SYSTEM, 
				p_str, sc_pos[i][0], sc_pos[i][1], MSG_NO_PUT, WFNOTE_COL_BLACK, NULL);
	}
	
	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );

	// �J�[�\���X�N���[���ǂݍ���
	p_wk->p_scrnbuff = ArcUtil_HDL_ScrnDataGet( p_draw->p_handle,
			NARC_pl_wifinote_techo_01_NSCR, FALSE, &p_wk->p_scrn, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֌W�̔j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_DrawExit( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	// ��b������o�b�t�@�j��
	STRBUF_Delete( p_wk->p_str );
	
	// �r�b�g�}�b�v�j��
	GF_BGL_BmpWinDel( &p_wk->msg );
	GF_BGL_BmpWinDel( &p_wk->talk );

	// �X�N���[���f�[�^�j��
	sys_FreeMemoryEz( p_wk->p_scrnbuff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�P���X������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^���[�N
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_SEQ_Init( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	// �^�C�g���\��
	WFNOTE_DRAW_BmpTitleWrite( p_draw, msg_wifi_note_01 );
	// ������`��
	GF_BGL_BmpWinOnVReq( &p_wk->msg );
	// �{�^���\��
	GF_BGL_ScrWrite( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			p_wk->p_scrn->rawData, 0, 0, 
			p_wk->p_scrn->screenWidth/8, p_wk->p_scrn->screenHeight/8 );
	// �X�N���[�����W�����ɖ߂�
	GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL], GF_BGL_SCROLL_X_SET, 0 );
	GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL], GF_BGL_SCROLL_Y_SET, 0 );
	// �J�[�\���ʒu�̃p���b�g��ݒ�
	WFNOTE_MODESELECT_CursorScrnPalChange( p_wk, p_draw );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�h�I����ʃ��C��
 *
 *	@param	p_wk		���[�h�I����ʃ��[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`�惏�[�N
 *
 *	@retval	TRUE	������I��
 *	@retval	FALSE	���������Ă��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_MODESELECT_SEQ_Main( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	BOOL move = FALSE;
	
	// �I���`�F�b�N
	if( sys.trg & PAD_BUTTON_DECIDE ){
        Snd_SePlay(WIFINOTE_DECIDE_SE);
		return TRUE;
	}

	// �L�����Z���`�F�b�N
	if( sys.trg & PAD_BUTTON_CANCEL ){
        Snd_SePlay(WIFINOTE_BS_SE);
		p_wk->cursor = WFNOTE_MODESELECT_CURSOR_END;	// �I���ɂ���
		return TRUE;
	}

	// �ړ��`�F�b�N
	if( sys.repeat & PAD_KEY_UP ){
		if( p_wk->cursor > 0 ){
            p_wk->cursor--;
			move = TRUE;
		}
	}else if( sys.repeat & PAD_KEY_DOWN ){
		if( p_wk->cursor < WFNOTE_MODESELECT_CURSOR_END ){
            p_wk->cursor++;
			move = TRUE;
		}
	}

	// �ړ������特�ƕ`��X�V
	if( move == TRUE ){
		Snd_SePlay( WIFINOTE_MOVE_SE );
		WFNOTE_MODESELECT_CursorScrnPalChange( p_wk, p_draw );
		GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\���̈ʒu�ɍ��킹�ăX�N���[���̃p���b�g
 *
 *	@param	p_wk	���[�N
 *	@param	p_draw	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_CursorScrnPalChange( WFNOTE_MODESELECT* p_wk, WFNOTE_DRAW* p_draw )
{
	int i;
	static const u8 sc_pos[WFNOTE_MODESELECT_CURSOR_NUM][2] = {
		// x , y
		{ 2, 4 },
		{ 2, 9 },
		{ 2, 14 },
		{ 2, 19 },
	};
	u8 pal;

	for( i=0; i<WFNOTE_MODESELECT_CURSOR_NUM; i++ ){
		if( p_wk->cursor == i ){
			pal = WFNOTE_MODESELECT_CURSOR_PAL_ON;
		}else{
			pal = WFNOTE_MODESELECT_CURSOR_PAL_OFF;
		}
		GF_BGL_ScrPalChange( p_draw->p_bgl, 
				sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL],
				sc_pos[i][0], sc_pos[i][1],
				WFNOTE_MODESELECT_CURSOR_CSIZX, WFNOTE_MODESELECT_CURSOR_CSIZY,
				pal );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\���ʒu�ɑΉ������I���f�[�^��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_EndDataSet( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data )
{
	static const u8 reqstatus[ WFNOTE_MODESELECT_CURSOR_NUM ] = {
		WFNOTE_STATUS_FRIENDLIST,
		WFNOTE_STATUS_CODEIN,
		WFNOTE_STATUS_MYCODE,
		WFNOTE_STATUS_END
	};
	WFNOTE_DATA_SetReqStatus( p_data, reqstatus[p_wk->cursor], 0 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\������S������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`�惏�[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_DrawOff( WFNOTE_MODESELECT* p_wk, WFNOTE_DRAW* p_draw )
{
	// �^�C�g��
	WFNOTE_DRAW_BmpTitleOff( p_draw );

	// SCROLL��
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL], 0, 0, 0, 32, 24, 0 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );

	// SCRMSG��
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG], 0, 0, 0, 32, 24, 0 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG] );

	// msg
	GF_BGL_BmpWinOffVReq(&p_wk->msg);
}	

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�v�����g
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`�惏�[�N
 *	@param	msgidx		���b�Z�[�WIDX
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_TalkMsgPrint( WFNOTE_MODESELECT* p_wk, WFNOTE_DRAW* p_draw, u32 msgidx, u32 heapID )
{
	STRBUF* p_tmp;

	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	
	// �N���A
	GF_BGL_BmpWinDataFill( &p_wk->talk, 15 );

	MSGMAN_GetString( p_draw->p_msgman, msgidx, p_tmp );
	WORDSET_ExpandStr( p_draw->p_wordset, p_wk->p_str, p_tmp );
    p_wk->talkmsg_idx = GF_STR_PrintColor(&p_wk->talk, FONT_TALK, 
			p_wk->p_str, 0, 0, p_wk->msg_speed, WFNOTE_COL_BLACK, NULL);
    BmpTalkWinWrite( &p_wk->talk, WINDOW_TRANS_OFF, 
			WFNOTE_BGCGX_MAIN_MSG_TALK, WFNOTE_BGPL_MAIN_TALKWIN );

    GF_BGL_BmpWinOnVReq(&p_wk->talk);

	STRBUF_Delete( p_tmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�̏I���҂�
 *	
 *	@param	cp_wk	���[�N
 *	
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�܂�
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_MODESELECT_TalkMsgEndCheck( const WFNOTE_MODESELECT* cp_wk )
{
    if( GF_MSG_PrintEndCheck( cp_wk->talkmsg_idx ) == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�E�C���h�E������
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MODESELECT_TalkMsgOff( WFNOTE_MODESELECT* p_wk )
{
	BmpTalkWinClear( &p_wk->talk, WINDOW_TRANS_OFF );
    GF_BGL_BmpWinOffVReq(&p_wk->talk);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\���ʒu�̃X�e�[�^�X�ɑJ�ڂ��Ă����v���`�F�b�N����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *
 *	@retval	TRUE	���v
 *	@retval	FALSE	����
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_MODESELECT_StatusChengeCheck( WFNOTE_MODESELECT* p_wk, WFNOTE_DATA* p_data )
{
	int  i;
	WIFI_LIST* p_list;
	
	if( p_wk->cursor == WFNOTE_MODESELECT_CURSOR_CODEIN ){
		// ���������邩�`�F�b�N
		p_list = SaveData_GetWifiListData( p_data->p_save );
		for(i=0;i<WIFILIST_FRIEND_MAX;i++){
			if( !WifiList_IsFriendData( p_list, i ) ){  // 
				return TRUE;
			}
		}
	}else{

		// �F�B�o�^�ȊO�͖�������OK
		return TRUE;
	}

	return FALSE;
}



//-------------------------------------
///	�F�B�f�[�^�\���֐�
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�\�����[�N	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_Init( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	p_wk->msgspeed = CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(p_data->p_save));
	
	WFNOTE_FRIENDLIST_DrawInit( p_wk, p_data, p_draw,  heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�\�����[�N	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	WFNOTE_STRET_CONTINUE,	// �Â�
 *	@retval	WFNOTE_STRET_FINISH,	// ��Ƃ͏I�����
 */
//-----------------------------------------------------------------------------
static WFNOTE_STRET WFNOTE_FRIENDLIST_Main( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u32 result;
	WIFI_LIST* p_wifilist;
	u32 listidx;

	switch( p_data->subseq ){
	case WFNOTE_FRIENDLIST_SEQ_INIT_PAGEINIT:	// �\������y�[�W������������Ƃ�
		p_wk->page = 0;			
		p_wk->lastpage = 0;			
		p_wk->pos = 0;
		p_wk->lastpos = 0;
	case WFNOTE_FRIENDLIST_SEQ_INIT:	// �ȑO�̏�Ԃŕ\��
		// ���X�g�f�[�^�쐬
		WFNOTE_DATA_FrIdxMake( p_data );

		// �`�揉����
		WFNOTE_FRIENDLIST_SEQ_DrawOn( p_wk, p_data, p_draw, heapID );	

		p_data->subseq  = WFNOTE_FRIENDLIST_SEQ_MAIN;
		break;

		// ���X�g����
	case WFNOTE_FRIENDLIST_SEQ_MAIN:
		result = WFNOTE_FRIENDLIST_SEQ_Main( p_wk, p_data,  p_draw );
		switch( result ){
		case WFNOTE_FRIENDLIST_SEQMAINRET_NONE:
			break;
			
		case WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGELEFT:// �y�[�W�ύX
			p_wk->lastpage = p_wk->page;
			p_wk->page --;
			if( p_wk->page < 0 ){
				p_wk->page += WFNOTE_FRIENDLIST_PAGE_NUM;
			}
			WFNOTE_FRIENDLIST_ScrSeqChange( p_wk, p_data, WF_COMMON_LEFT );
			break;
			
		case WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGERIGHT:// �y�[�W�ύX
			p_wk->lastpage = p_wk->page;
			p_wk->page = (p_wk->page + 1) % WFNOTE_FRIENDLIST_PAGE_NUM;
			WFNOTE_FRIENDLIST_ScrSeqChange( p_wk, p_data, WF_COMMON_RIGHT );
			break;
			
		case WFNOTE_FRIENDLIST_SEQMAINRET_CANCEL:	// �߂�
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_END;
			break;
			
		case WFNOTE_FRIENDLIST_SEQMAINRET_SELECTPL:	// �l��I������
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MENUINIT;
			break;
		}
		break;
		
		// �X�N���[������
	case WFNOTE_FRIENDLIST_SEQ_SCRLLINIT:
		WFNOTE_FRIENDLIST_SEQ_ScrllInit( p_wk, p_data, p_draw, heapID );
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_SCRLL;
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_SCRLL:

		result = WFNOTE_FRIENDLIST_SEQ_ScrllMain( p_wk, p_data, p_draw, heapID );
		if( result == TRUE ){
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MAIN;
		}
		break;

	case WFNOTE_FRIENDLIST_SEQ_MENUINIT:	//�@���j���[�I��

		// �I���������X�g�C���f�b�N�X���f�[�^�\���̂ɐݒ�
		WFNOTE_DATA_SELECT_ListIdxSet( p_data, 
				WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk ) );
		
		WFNOTE_FRIENDLIST_SEQ_MenuInit( p_wk, p_data, p_draw, heapID );
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MENUWAIT;
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_MENUWAIT:	// ���j���[�I��҂�
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MenuWait( p_wk, p_data, p_draw, heapID );
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_INFOINIT:		// �ڂ��������
		p_wk->wait = WFNOTE_FRIENDLIST_INFO_WAIT;
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_INFO;
		break;

	case WFNOTE_FRIENDLIST_SEQ_INFO:		// �ڂ��������
		// ��ʐ؂�ւ������ꂢ�Ɍ�����悤�ɂ������wait
		p_wk->wait --;
		if( p_wk->wait <= 0 ){
			WFNOTE_FRIENDLIST_SEQ_DrawOff( p_wk, p_data, p_draw );
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_FRIENDINFO, 0 );
			return WFNOTE_STRET_FINISH; 
		}
		break;

	case WFNOTE_FRIENDLIST_SEQ_INFORET: // �ڂ������邩��߂��Ă����Ƃ�
		// �y�[�W��ݒ�
		p_wk->page = p_data->select_listidx / WFNOTE_FRIENDLIST_PAGEFRIEND_NUM;
		p_wk->pos = p_data->select_listidx % WFNOTE_FRIENDLIST_PAGEFRIEND_NUM;
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_INIT;	// ��������
		break;
		
	case WFNOTE_FRINEDLIST_SEQ_NAMECHG_INIT:	// ���O�ύX
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_HOLEOUT, WIPE_TYPE_HOLEOUT, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID);

		// �����񏉊����f�[�^��ݒ�
		listidx = WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk );
		p_wifilist = SaveData_GetWifiListData( p_data->p_save );
		WFNOTE_DATA_NAMEIN_INIT_Set( p_data, WifiList_GetFriendNamePtr( p_wifilist, p_data->idx.fridx[listidx] ) );	
		p_data->subseq = WFNOTE_FRINEDLIST_SEQ_NAMECHG_WIPE;
		break;

	case WFNOTE_FRINEDLIST_SEQ_NAMECHG_WIPE:	// ���O�ύX
		if( WIPE_SYS_EndCheck() ){
			// ���O���͉�ʂ�
			WFNOTE_FRIENDLIST_SEQ_DrawOff( p_wk, p_data, p_draw );
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_CODEIN, WFNOTE_CODEIN_SEQ_NAMEINONLY );
			return WFNOTE_STRET_FINISH;
		}
		break;
		
	case WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAIT:	// ���O�ύX
		// ���O��ύX
		listidx = WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk );
		p_wifilist = SaveData_GetWifiListData( p_data->p_save );
		WifiList_SetFriendName( p_wifilist, p_data->idx.fridx[listidx], p_data->codein.p_name );
	case WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAITWIPE:	// ���O�ύX	�������Ȃ��ŕ��A
		// �`�揉����
		WFNOTE_FRIENDLIST_SEQ_DrawOn( p_wk, p_data, p_draw, heapID );
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_HOLEIN, WIPE_TYPE_HOLEIN, WIPE_FADE_BLACK, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, heapID);
		p_data->subseq = WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAITWIPEWAIT;
		break;
		
	case WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAITWIPEWAIT:	// ���O�ύX
		if( WIPE_SYS_EndCheck() ){
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MAIN;
		}
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_DELETE_INIT:	// �j��
		WFNOTE_FRIENDLIST_SEQ_DeleteInit( p_wk, p_data, p_draw, heapID );
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_DELETE_YESNODRAW;
		break;

	case WFNOTE_FRIENDLIST_SEQ_DELETE_YESNODRAW:	// �j��	YESNO�E�B���h�E�\��
		result = WFNOTE_FRIENDLIST_SEQ_DeleteYesNoDraw( p_wk, p_data, p_draw, heapID );
		if( result == TRUE ){
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_DELETE_WAIT;
		}
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_DELETE_WAIT:	// �j��YESNO���܂��Ă��珈��
		result = BmpYesNoSelectMain( p_wk->p_yesno, heapID );

		switch( result ){
		case BMPMENU_NULL:	// ���������ĂȂ�
			break;	
			
		case BMPMENU_CANCEL:	// ������
			// ���b�Z�[�W�������Ė����o��
			WFNOTE_FRIENDLIST_TalkMsgOff( p_wk );
			WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );
			p_data->subseq =  WFNOTE_FRIENDLIST_SEQ_MAIN;
			break;
			
		default:		// �͂�
			WFNOTE_FRIENDLIST_TalkMsgOff( p_wk );
			p_data->subseq =  WFNOTE_FRIENDLIST_SEQ_DELETE_ANMSTART;
			break;
		}

		break;

	case WFNOTE_FRIENDLIST_SEQ_DELETE_ANMSTART:	// �j��YESNO���܂��Ă��珈��
		WFNOTE_FRIENDLIST_DeleteAnmStart( p_wk );
		p_data->subseq = WFNOTE_FRIENDLIST_SEQ_DELETE_ANMWAIT;
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_DELETE_ANMWAIT:	// �j��YESNO���܂��Ă��珈��
		if( WFNOTE_FRIENDLIST_DeleteAnmMain( p_wk ) == TRUE ){
			// �j������
			p_wifilist = SaveData_GetWifiListData( p_data->p_save );
			listidx = WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk );
			WifiList_ResetData( p_wifilist, p_data->idx.fridx[ listidx ] );
			//�t�����h���Ɏ��t�����e�B�A�f�[�^���폜 2008.05.24(�y) matsuda
			FrontierRecord_ResetData(
				SaveData_GetFrontier(p_data->p_save), p_data->idx.fridx[ listidx ]);

			// �F�B�C���f�b�N�X����蒼��
			// �F�B�̕\������������
			// ���X�g�f�[�^�쐬
			WFNOTE_DATA_FrIdxMake( p_data );

			// �`�揉����
			WFNOTE_FRIENDLIST_DrawPage( p_wk, p_data, p_draw, p_wk->page, p_wk->pos, WFNOTE_DRAWAREA_MAIN, heapID );

			// ���b�Z�[�W�������Ė����o��
			WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );
			p_data->subseq =  WFNOTE_FRIENDLIST_SEQ_DELETE_END;
		}
		break;

	case WFNOTE_FRIENDLIST_SEQ_DELETE_END:
		WFNOTE_FRIENDLIST_DeleteAnmOff( p_wk );
		p_data->subseq =  WFNOTE_FRIENDLIST_SEQ_MAIN;
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_CODE_INIT:	// �R�[�h�؂�ւ�
		result = WFNOTE_FRIENDLIST_SEQ_CodeInit( p_wk, p_data, p_draw, heapID );
		if( result == TRUE ){
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_CODE_WAIT;
		}else{
			WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );	// ���\��
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MAIN;
		}
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_CODE_WAIT:	// �����L�[�������Ă��炤
		if( WFNOTE_FRIENDLIST_TalkMsgEndCheck( p_wk ) == FALSE ){
			break;
		}
		
		if( sys.trg ){	// �Ȃɂ��L�[�������ꂽ�猳�ɖ߂�
			WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );	// ���\��
			p_data->subseq = WFNOTE_FRIENDLIST_SEQ_MAIN;
			WFNOTE_FRIENDLIST_TalkMsgOff( p_wk );
		}
		break;
		
	case WFNOTE_FRIENDLIST_SEQ_END:
		WFNOTE_FRIENDLIST_SEQ_DrawOff( p_wk, p_data, p_draw );
		WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_MODE_SELECT, WFNOTE_MODESELECT_SEQ_INIT_NOFADE );
		return WFNOTE_STRET_FINISH; 
	}
	return WFNOTE_STRET_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�f�[�^�\�����[�N	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_Exit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	WFNOTE_FRIENDLIST_DrawExit( p_wk, p_data, p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�揉����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DrawInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	static const u16 sc_TEXTCGX[ WFNOTE_DRAWAREA_NUM ] = {
		WFNOTE_FRIENDLIST_TEXT_CGX0,
		WFNOTE_FRIENDLIST_TEXT_CGX1,
		WFNOTE_FRIENDLIST_TEXT_CGX1,
	};
	int i, j;
	STRBUF* p_str;
    BMPLIST_HEADER list_h;
	CLACT_ADD clearadd = {
		NULL,
		NULL,
		{ 0,0,0 },	
		{ FX32_ONE,FX32_ONE,FX32_ONE },	
		0,
		0,
		NNS_G2D_VRAM_TYPE_2DMAIN,
		0
	};

	//  �`��G���A�쐬
	for( i=0; i<WFNOTE_DRAWAREA_NUM; i++ )
	{
		WFNOTE_FRIENDLIST_DRAWAREA_Init( &p_wk->drawdata[ i ], p_draw, 
				&sc_SCRNAREA[i], sc_TEXTCGX[i], heapID );
	}

	// �L�����N�^�f�[�^������
	p_wk->p_charsys = WF_2DC_SysInit( p_draw->p_clact, NULL, WFNOTE_FRIENDLIST_2DCHAR_NUM, heapID );

	// ���j�I���L�����N�^��ǂݍ���
	WF_2DC_UnionResSet( p_wk->p_charsys, NNS_G2D_VRAM_TYPE_2DMAIN, 
			WF_2DC_MOVETURN, heapID );

		
	// �߂郁�b�Z�[�W�쐬
	GF_BGL_BmpWinInit( &p_wk->backmsg );	
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->backmsg, 
			sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_MSG ], 
			WFNOTE_FRIENDLIST_BACKMSG_X, WFNOTE_FRIENDLIST_BACKMSG_Y,
			WFNOTE_FRIENDLIST_BACKMSG_SIZX, WFNOTE_FRIENDLIST_BACKMSG_SIZY,
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_FRIENDLIST_BACKMSG_CGX );
	GF_BGL_BmpWinDataFill( &p_wk->backmsg, 0 );

	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	MSGMAN_GetString( p_draw->p_msgman, msg_wifi_note_08, p_str );
    GF_STR_PrintColor( &p_wk->backmsg, FONT_SYSTEM, 
			p_str,
			0, 0, MSG_NO_PUT, WFNOTE_COL_BLACK, NULL);
	STRBUF_Delete( p_str );

	// ���j���[�쐬
	GF_BGL_BmpWinInit( &p_wk->menu );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->menu, 
			sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_MSG ], 
			WFNOTE_FRIENDLIST_MENU_X, WFNOTE_FRIENDLIST_MENU_Y,
			WFNOTE_FRIENDLIST_MENU_SIZX, WFNOTE_FRIENDLIST_MENU_SIZY,
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_FRIENDLIST_MENU_CGX );
	GF_BGL_BmpWinDataFill( &p_wk->menu, 0 );

	// ���j���[�f�[�^�쐬
	for( i=0; i<WFNOTE_FRIENDLIST_MENU_NUM; i++ ){
		p_wk->p_menulist[i] = BMP_MENULIST_Create( WFNOTE_FRIENDLIST_MENU_LISTNUM, heapID );
		for( j=0; j<WFNOTE_FRIENDLIST_MENU_LISTNUM; j++ ){
			BMP_MENULIST_AddArchiveString( p_wk->p_menulist[i], p_draw->p_msgman,
					sc_WFNOTE_FRIENDLIST_MENU[i][j].strid, sc_WFNOTE_FRIENDLIST_MENU[i][j].param );
		}
	}

	// ��b�E�B���h�E�쐬
	GF_BGL_BmpWinInit( &p_wk->talk );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->talk, 
			sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_MSG ], 
			WFNOTE_FRIENDLIST_TALK_X, WFNOTE_FRIENDLIST_TALK_Y,
			WFNOTE_FRIENDLIST_TALK_SIZX, WFNOTE_FRIENDLIST_TALK_SIZY,
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_FRIENDLIST_TALK_CGX );
	GF_BGL_BmpWinDataFill( &p_wk->talk, 15 );
	p_wk->p_talkstr = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �����S���A�N�^�[�̍쐬
	clearadd.ClActSet = p_draw->p_clact;
	clearadd.ClActHeader = &p_draw->clheader;
	clearadd.heap = heapID;
	p_wk->p_clearact = CLACT_Add( &clearadd );
	// �Ƃ肠������\��
	CLACT_SetDrawFlag( p_wk->p_clearact, FALSE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DrawExit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	int i;

	// �����S���A�N�^�[
	CLACT_Delete( p_wk->p_clearact );

	// ��b���E�B���h�E
	GF_BGL_BmpWinDel( &p_wk->talk );
	STRBUF_Delete( p_wk->p_talkstr );

	// ���j���[�f�[�^�j��
	for( i=0; i<WFNOTE_FRIENDLIST_MENU_NUM; i++ ){
	    BMP_MENULIST_Delete( p_wk->p_menulist[i] );
	}
	GF_BGL_BmpWinDel( &p_wk->menu );

	// ���b�Z�[�W�ʔj��
	GF_BGL_BmpWinDel( &p_wk->backmsg );

	// ���j�I���L�����N�^��ǂݍ���
	WF_2DC_UnionResDel( p_wk->p_charsys );

	// �L�����N�^�f�[�^������
	WF_2DC_SysExit( p_wk->p_charsys );


	//  �`��G���A�쐬
	for( i=0; i<WFNOTE_DRAWAREA_NUM; i++ ){
		WFNOTE_FRIENDLIST_DRAWAREA_Exit( &p_wk->drawdata[ i ], p_draw );
	}
}
//----------------------------------------------------------------------------
/**
 *	@brief	�`��J�n�֐�
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_SEQ_DrawOn( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	int i;
	
	// �^�C�g���`��
	WFNOTE_DRAW_BmpTitleWrite( p_draw, msg_wifi_note_06 );

	// �߂�`��
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			WFNOTE_FRIENDLIST_BACK_X,
			WFNOTE_FRIENDLIST_BACK_Y,
			WFNOTE_FRIENDLIST_BACK_SIZX, WFNOTE_FRIENDLIST_BACK_SIZY,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_SCRBACK_X, WFNOTE_FRIENDLIST_SCRBACK_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// �߂�̃��b�Z�[�W�`��
	GF_BGL_BmpWinOnVReq( &p_wk->backmsg );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );


	// ���`��
	WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );

	//  �ŏ��̃y�[�W�`��
	WFNOTE_FRIENDLIST_DrawPage( p_wk, p_data, p_draw, p_wk->page, p_wk->pos, WFNOTE_DRAWAREA_MAIN, heapID );

}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��OFF�@
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_SEQ_DrawOff( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	int i;
	
	// �^�C�g��OFF
	WFNOTE_DRAW_BmpTitleOff( p_draw );

	// �߂��\��
	GF_BGL_BmpWinOffVReq( &p_wk->backmsg );

	// �`��G���A�̕\��OFF
	for( i=0; i<WFNOTE_DRAWAREA_NUM; i++ ){
		WFNOTE_FRIENDLIST_DRAWAREA_DrawOff( &p_wk->drawdata[i], 
				p_draw, &p_draw->scrn );
	}

	// ���`��OFF
	WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, FALSE );

	// �w�i�����ɖ߂�
	// ���ǂ�
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			WFNOTE_FRIENDLIST_BACK_X,
			WFNOTE_FRIENDLIST_BACK_Y,
			WFNOTE_FRIENDLIST_BACK_SIZX, WFNOTE_FRIENDLIST_BACK_SIZY,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_CL_SCRBACK_X, WFNOTE_FRIENDLIST_CL_SCRBACK_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	// ������ԂԂ�
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			WFNOTE_FRIENDLIST_SHIORI_X, WFNOTE_FRIENDLIST_SHIORI_Y,
			WFNOTE_FRIENDLIST_SHIORI_SIZX, WFNOTE_FRIENDLIST_SHIORI_SIZY,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_CL_SHIORISCRN_X, WFNOTE_FRIENDLIST_CL_SHIORISCRN_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B���X�g�\��	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *
 *	@retval	WFNOTE_FRIENDLIST_SEQMAINRET_NONE,
 *	@retval	WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGE,// �y�[�W�ύX
 *	@retval	WFNOTE_FRIENDLIST_SEQMAINRET_CANCEL,	// �߂�
 *	@retval	WFNOTE_FRIENDLIST_SEQMAINRET_SELECTPL,	// �l��I������
 *
 */
//-----------------------------------------------------------------------------
static u32 WFNOTE_FRIENDLIST_SEQ_Main( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	u16 lastpos;
	BOOL move;
	BOOL pagechang;
	WF_COMMON_WAY way;
	
	if( sys.trg & PAD_BUTTON_DECIDE ){
	
		// �L�����Z���`�F�b�N
		if( p_wk->pos == WFNOTE_FRIENDLIST_CURSORDATA_BACK ){
			Snd_SePlay(WIFINOTE_DECIDE_SE);
			return WFNOTE_FRIENDLIST_SEQMAINRET_CANCEL;
		}
	
		// ���̈ʒu�ɗF�B�����邩�`�F�b�N
		if( p_data->idx.friendnum <= p_wk->pos + (p_wk->page*WFNOTE_FRIENDLIST_PAGEFRIEND_NUM) ){
			return WFNOTE_FRIENDLIST_SEQMAINRET_NONE;	// ���Ȃ�
		}
		
		// �F�B�̏���\������
		Snd_SePlay(WIFINOTE_DECIDE_SE);
		return WFNOTE_FRIENDLIST_SEQMAINRET_SELECTPL;
	}

	if( sys.trg & PAD_BUTTON_CANCEL ){
		// ���[�h�I���ɖ߂�
        Snd_SePlay(WIFINOTE_BS_SE);
		return WFNOTE_FRIENDLIST_SEQMAINRET_CANCEL;
	}

	lastpos = p_wk->pos;	// �O�̍��W��ۑ����Ă���	���ǂ�@���烊�X�g�ɃJ�[�\�����ړ�����Ƃ��Ɏg�p����
	move = FALSE;
	pagechang = FALSE;
	
	if( sys.repeat & PAD_KEY_UP ){
		if( p_wk->pos == WFNOTE_FRIENDLIST_CURSORDATA_BACK ){
			//  �߂�ʒu�����ɍs���Ƃ��͂P�O�ɂ����Ƃ���ɖ߂�
			p_wk->pos  = p_wk->lastpos;
			move = TRUE;
			WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );
		}else{
			if( p_wk->pos < WFNOTE_FRIENDLIST_PAGEFRIEND_HALF ){
				if( p_wk->pos > 0 ){
					p_wk->pos --;
					move = TRUE;
				}
			}else{
				if( p_wk->pos > WFNOTE_FRIENDLIST_PAGEFRIEND_HALF ){
					p_wk->pos --;
					move =  TRUE;
				}
			}
		}
	}else if( sys.repeat & PAD_KEY_DOWN ){
		if( (p_wk->pos == 3) || (p_wk->pos == 7) ){
			// ��Ԃ����̍��ڂɂ��ĉ��������ꂽ��BACK�Ɉړ�
			p_wk->pos = WFNOTE_FRIENDLIST_CURSORDATA_BACK;
			move = TRUE;
			// ��������
			WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, FALSE );
		}else{
			
			if( p_wk->pos < WFNOTE_FRIENDLIST_PAGEFRIEND_HALF ){
				if( p_wk->pos < 3 ){
					p_wk->pos ++;
					move = TRUE;
				}
			}else{
				if( p_wk->pos < 7 ){
					p_wk->pos ++;
					move =  TRUE;
				}
			}
		}
	}else if( sys.repeat & PAD_KEY_LEFT ){
		// BACK�̎��͍��͖���
		if( p_wk->pos != WFNOTE_FRIENDLIST_CURSORDATA_BACK ){
			if( p_wk->pos < WFNOTE_FRIENDLIST_PAGEFRIEND_HALF ){
				p_wk->pos += WFNOTE_FRIENDLIST_PAGEFRIEND_HALF;	
				move = TRUE;
				pagechang = TRUE;
				way = WF_COMMON_LEFT;
			}else{
				p_wk->pos -= WFNOTE_FRIENDLIST_PAGEFRIEND_HALF;	
				move = TRUE;
			}
		}
	}else if( sys.repeat & PAD_KEY_RIGHT ){
		// BACK�̎��́��͖���
		if( p_wk->pos != WFNOTE_FRIENDLIST_CURSORDATA_BACK ){
			if( p_wk->pos < WFNOTE_FRIENDLIST_PAGEFRIEND_HALF ){
				p_wk->pos += WFNOTE_FRIENDLIST_PAGEFRIEND_HALF;	
				move = TRUE;
			}else{
				p_wk->pos -= WFNOTE_FRIENDLIST_PAGEFRIEND_HALF;	
				move = TRUE;
				pagechang = TRUE;
				way = WF_COMMON_RIGHT;
			}
		}
	}

	if( pagechang == TRUE ){
		// �X�N���[�������ֈڍs����
		if(way == WF_COMMON_LEFT){
			return WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGELEFT;
		}
		return WFNOTE_FRIENDLIST_SEQMAINRET_PAGECHANGERIGHT;
	}else{
		if( move == TRUE ){

			Snd_SePlay( WIFINOTE_MOVE_SE );
			
			// �J�[�\����������
			WFNOTE_FRIENDLIST_DRAWAREA_CursorWrite( &p_wk->drawdata[WFNOTE_DRAWAREA_MAIN], p_draw, p_wk->pos );
			p_wk->lastpos = lastpos;
		}
	}
	return WFNOTE_FRIENDLIST_SEQMAINRET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���V�[�P���X������
 *	
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_SEQ_ScrllInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u16 draw_area;

	if( p_wk->way == WF_COMMON_LEFT ){
		draw_area = WFNOTE_DRAWAREA_LEFT;
	}else{
		draw_area = WFNOTE_DRAWAREA_RIGHT;
	}
	
	// �X�N���[�������ɏ����L��
	WFNOTE_FRIENDLIST_DrawPage( p_wk, p_data, p_draw, p_wk->page, p_wk->pos, draw_area, heapID );

	// �J�[�\��������
	WFNOTE_FRIENDLIST_DRAWAREA_CursorWrite( &p_wk->drawdata[ WFNOTE_DRAWAREA_MAIN ], p_draw, WFNOTE_FRIENDLIST_CURSORDATA_OFF );

	// �����Ƃ߂�
	WFNOTE_DRAW_YazirushiSetAnmFlag( p_draw, FALSE );

	// �X�N���[����
	Snd_SePlay( WIFINOTE_SCRLL_SE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���V�[�P���X	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDLIST_SEQ_ScrllMain( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u32 draw_area;
	s32 scrll_x;
	NNSG2dViewRect rect;
	u32 idx;
	static void (*pSHIORIFUNC[ 4 ])( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page0, u32 page1 ) = {
		WFNOTE_FRIENDLIST_DrawShioriEff00,
		NULL,
		WFNOTE_FRIENDLIST_DrawShioriEff01,
		NULL
	};

	// �J�E���^���I������炷�ׂĂ�`�悵�ďI���
	if( p_wk->count >= WFNOTE_FIRENDLIST_SCRLL_COUNT ){

		// �X�N���[�����W�����ɖ߂�
		GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ], GF_BGL_SCROLL_X_SET, 0 );
		GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ], GF_BGL_SCROLL_X_SET, 0 );
		// OAM�ɂ����f
		rect = s_SCRLLSURFACE_RECT;
		RNDP_SetSurfaceRect( &p_draw->scrll_surface, &rect );

		// ��Ɨp�O���t�B�b�N������
		if( p_wk->way == WF_COMMON_LEFT ){
			draw_area = WFNOTE_DRAWAREA_LEFT;
		}else{
			draw_area = WFNOTE_DRAWAREA_RIGHT;
		}
		WFNOTE_FRIENDLIST_DRAWAREA_DrawOff( &p_wk->drawdata[ draw_area ], p_draw, &p_draw->scrn );

		// ���C���ʂɕ\��
		WFNOTE_FRIENDLIST_DrawPage( p_wk, p_data, p_draw, p_wk->page, p_wk->pos, WFNOTE_DRAWAREA_MAIN, heapID );

		// ��󓮍�J�n
		WFNOTE_DRAW_YazirushiSetAnmFlag( p_draw, TRUE );
		return TRUE;
	}
	
	scrll_x = (p_wk->count * WFNOTE_FRIENDLIST_SCRLL_SIZX) / WFNOTE_FIRENDLIST_SCRLL_COUNT;

	if( p_wk->way == WF_COMMON_LEFT ){
		scrll_x = -scrll_x;
	}

	//  �X�N���[�����W�ݒ�
	GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ], GF_BGL_SCROLL_X_SET, scrll_x );
	GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ], GF_BGL_SCROLL_X_SET, scrll_x );

	// OAM�ɂ����f
	rect = s_SCRLLSURFACE_RECT;
	rect.posTopLeft.x += scrll_x << FX32_SHIFT;
	RNDP_SetSurfaceRect( &p_draw->scrll_surface, &rect );

	// �A�j���[�V����
	if( p_wk->count % (WFNOTE_FIRENDLIST_SCRLL_COUNT/4) ){
		idx = p_wk->count / (WFNOTE_FIRENDLIST_SCRLL_COUNT/4);
		if( pSHIORIFUNC[idx] ){
			pSHIORIFUNC[idx]( p_wk, p_draw, p_wk->page, p_wk->lastpage );
		}
	}

	// �J�E���g
	p_wk->count ++;
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j���[�\��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_SEQ_MenuInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	BMPLIST_HEADER list_h;
	WIFI_LIST* p_wifilist;
	u32 sex;
	u32 listidx;

	// wifi���X�g���琫�ʂ��擾
	p_wifilist = SaveData_GetWifiListData( p_data->p_save );
	listidx = WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk );
	sex = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[ listidx ], WIFILIST_FRIEND_SEX );
	

	list_h = WFNOTE_FRINEDLIST_MENUHEADER;

	if( sex == PM_NEUTRAL ){	
		list_h.list = p_wk->p_menulist[ WFNOTE_FRIENDLIST_MENU_CODE ];
	}else{
		list_h.list = p_wk->p_menulist[ WFNOTE_FRIENDLIST_MENU_NML ];
	}
	list_h.win = &p_wk->menu;
	
	// ���j���[�̕\��
	p_wk->p_listwk = BmpListSet( &list_h, 0, 0, heapID );
    BmpMenuWinWrite( &p_wk->menu, WINDOW_TRANS_OFF, WFNOTE_BGCGX_MAIN_MSG_MENU, WFNOTE_BGPL_MAIN_MENUWIN );
	GF_BGL_BmpWinOnVReq( &p_wk->menu );

	// ���b�Z�[�W�\��
	WFNOTE_DRAW_FriendNameSetWordset( p_draw, p_data->p_save, p_data->idx.fridx[ listidx ], heapID );
	WFNOTE_FRIENDLIST_TalkMsgWrite( p_wk, p_draw, msg_wifi_note_13, heapID );
	// �₶�邵��\��
	WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, FALSE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j���[�\��	�I��҂�
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@return	���ɐi��łق����V�[�P���X
 */
//-----------------------------------------------------------------------------
static u32 WFNOTE_FRIENDLIST_SEQ_MenuWait( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u32 ret;
	u32 ret_seq = WFNOTE_FRIENDLIST_SEQ_MENUWAIT;
	
	
	// ���b�Z�[�W�I���҂�
	if( WFNOTE_FRIENDLIST_TalkMsgEndCheck( p_wk ) == FALSE ){
		return ret_seq;
	}

	// ���X�g�I��
	ret = BmpListMain( p_wk->p_listwk ); 

    switch(ret){
    case BMPLIST_NULL:
		return ret_seq;
    case BMPLIST_CANCEL:
        Snd_SePlay(WIFINOTE_MENU_BS_SE);
		WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );
		
		ret_seq = WFNOTE_FRIENDLIST_SEQ_MAIN;
        break;
    default:
        Snd_SePlay(WIFINOTE_MENU_DECIDE_SE);
		ret_seq = ret;
        break;
    }
    WFNOTE_FRIENDLIST_TalkMsgOff( p_wk );
    BmpMenuWinClear( &p_wk->menu, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &p_wk->menu );
    BmpListExit( p_wk->p_listwk, NULL, NULL);

	// MAIN�ɂ��ǂ�̂Ŗ����o��
	if( ret_seq == WFNOTE_FRIENDLIST_SEQ_MAIN ){
		WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );
	}
	
	return ret_seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R�[�h�\��	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�\���ł���
 *	@retval	FALSE	�\���ł��Ȃ�
 */	
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDLIST_SEQ_CodeInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	WIFI_LIST* p_wifilist;
	DWCFriendData* p_frienddata;
	u64 code;
	u32 listidx;

	p_wifilist = SaveData_GetWifiListData( p_data->p_save );
	listidx = WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk );
	p_frienddata = WifiList_GetDwcDataPtr( p_wifilist,
			p_data->idx.fridx[ listidx ] ); 
	code = DWC_GetFriendKey( p_frienddata );
	if(code!=0){
		// �R�[�h�\��
		Snd_SePlay(WIFINOTE_DECIDE_SE);
		WFNOTE_DRAW_FriendCodeSetWordset( p_draw, code );
		WFNOTE_DRAW_FriendNameSetWordset( p_draw, p_data->p_save, p_data->idx.fridx[ listidx ], heapID );
		WFNOTE_FRIENDLIST_TalkMsgWrite( p_wk, p_draw, msg_wifi_note_32, heapID );
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�j������	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_SEQ_DeleteInit( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u32 listidx;

	listidx = WFNOTE_FRIENDLIST_FRIDX_IdxGet( p_wk );
	WFNOTE_DRAW_FriendNameSetWordset( p_draw, p_data->p_save, p_data->idx.fridx[ listidx ], heapID );
	WFNOTE_FRIENDLIST_TalkMsgWrite( p_wk, p_draw, msg_wifi_note_29, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	YesNo�E�B���h�E�̕\��
 *
 *	@param	p_wk	���[�N
 *	@param	p_data	�f�[�^
 *	@param	p_draw	�`��V�X�e��
 *	@param	heapID	�q�[�vID
 *
 *	@retval	TRUE	�I��
 *	@retval	TRUE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDLIST_SEQ_DeleteYesNoDraw( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	if( WFNOTE_FRIENDLIST_TalkMsgEndCheck( p_wk ) == FALSE ){
		return FALSE;
	}

	// YesNo�E�B���h�E�ݒ�
	p_wk->p_yesno =
		BmpYesNoSelectInit( p_draw->p_bgl,
							&WFNOTE_MODESELECT_YESNOWIN,
							WFNOTE_BGCGX_MAIN_MSG_MENU, WFNOTE_BGPL_MAIN_MENUWIN,
							heapID );
	return TRUE;	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�j�������@YesNo�I����j������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	YesNo�I���̖߂�l
 */
//-----------------------------------------------------------------------------
static u32 WFNOTE_FRIENDLIST_SEQ_DeleteWait( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	return  TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�y�[�W�`��֐�
 *	
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	page		�y�[�W
 *	@param	pos			�J�[�\���ʒu
 *	@param	draw_area	�`��G���A
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DrawPage( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 page, u32 pos, u32 draw_area, u32 heapID )
{
	// ���C���ʂւ̕`�悩�`�F�b�N
	if( WFNOTE_DRAWAREA_MAIN == draw_area ){
		// �y�[�W�ƃJ�[�\���ύX
		p_wk->page = page;
		p_wk->pos = pos;

		// �y�[�W�ɑΉ������������\��
		WFNOTE_FRIENDLIST_DrawShiori( p_wk, p_draw, page );
	}

	// �w�i��`�悵�Ȃ���
	WFNOTE_FRIENDLIST_DRAWAREA_BackWrite( &p_wk->drawdata[draw_area], 
			p_draw, &p_draw->scrn );

	// �y�[�W�f�[�^�`��
	WFNOTE_FRIENDLIST_DRAWAREA_WritePlayer( &p_wk->drawdata[draw_area], p_data, 
			p_draw, p_wk->p_charsys, &p_draw->scrn, &p_data->idx, page, heapID );

	// �J�[�\���`��
	WFNOTE_FRIENDLIST_DRAWAREA_CursorWrite( &p_wk->drawdata[draw_area], p_draw, pos );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������̕\��
 *	
 *	@param	p_wk	���[�N
 *	@param	p_draw	�`��V�X�e��
 *	@param	page	�y�[�W��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DrawShiori( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page )
{
	u32 x;
	
	// ��{���܂��`��
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			WFNOTE_FRIENDLIST_SHIORI_X, WFNOTE_FRIENDLIST_SHIORI_Y,
			WFNOTE_FRIENDLIST_SHIORI_SIZX, WFNOTE_FRIENDLIST_SHIORI_SIZY,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_SHIORISCRN_X, WFNOTE_FRIENDLIST_SHIORISCRN_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// ���̃y�[�W�̂������`��
	x = WFNOTE_FRIENDLIST_SHIORI_X + (WFNOTE_FRIENDLIST_SHIORI_ONEX*page) + WFNOTE_FRIENDLIST_SHIORI_OFSX;
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			x, WFNOTE_FRIENDLIST_SHIORI_Y + WFNOTE_FRIENDLIST_SHIORI_OFSY,
			WFNOTE_FRIENDLIST_SHIORIPOS_SIZ, WFNOTE_FRIENDLIST_SHIORIPOS_SIZ,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_SHIORISCRNPOS_X, sc_POSSCRNY[page], 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������A�j���@�V�[���O
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�f�[�^
 *	@param	page0		�y�[�W�P
 *	@param	page1		�y�[�W�Q
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DrawShioriEff00( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page0, u32 page1 )
{
	u32 x;

	x = WFNOTE_FRIENDLIST_SHIORI_X + (WFNOTE_FRIENDLIST_SHIORI_ONEX*page1) + WFNOTE_FRIENDLIST_SHIORI_OFSX;
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			x, WFNOTE_FRIENDLIST_SHIORI_Y + WFNOTE_FRIENDLIST_SHIORI_OFSY,
			WFNOTE_FRIENDLIST_SHIORIPOS_SIZ, WFNOTE_FRIENDLIST_SHIORIPOS_SIZ,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_EFFECTSHIORISCRNPOS_X, sc_POSSCRNY[page1], 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������A�j���@�V�[���P
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�f�[�^
 *	@param	page0		�y�[�W�P
 *	@param	page1		�y�[�W�Q
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DrawShioriEff01( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 page0, u32 page1 )
{
	u32 x;
	
	// ��{���܂��`��
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			WFNOTE_FRIENDLIST_SHIORI_X, WFNOTE_FRIENDLIST_SHIORI_Y,
			WFNOTE_FRIENDLIST_SHIORI_SIZX, WFNOTE_FRIENDLIST_SHIORI_SIZY,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_SHIORISCRN_X, WFNOTE_FRIENDLIST_SHIORISCRN_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// ���̃y�[�W�̂������`��
	x = WFNOTE_FRIENDLIST_SHIORI_X + (WFNOTE_FRIENDLIST_SHIORI_ONEX*page0) + WFNOTE_FRIENDLIST_SHIORI_OFSX;
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			x, WFNOTE_FRIENDLIST_SHIORI_Y + WFNOTE_FRIENDLIST_SHIORI_OFSY,
			WFNOTE_FRIENDLIST_SHIORIPOS_SIZ, WFNOTE_FRIENDLIST_SHIORIPOS_SIZ,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_SHIORISCRNPOS_X, sc_POSSCRNY[page0], 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���V�[�P���X�ɕύX����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_ScrSeqChange( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DATA* p_data, WF_COMMON_WAY way )
{
	p_data->subseq = WFNOTE_FRIENDLIST_SEQ_SCRLLINIT;
	p_wk->way = way;
	p_wk->count = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	msgid		���b�Z�[�WID
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_TalkMsgWrite( WFNOTE_FRIENDLIST* p_wk, WFNOTE_DRAW* p_draw, u32 msgid, u32 heapID )
{
	STRBUF* p_tmp;

	// �N���A
	GF_BGL_BmpWinDataFill( &p_wk->talk, 15 );

	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	MSGMAN_GetString( p_draw->p_msgman, msgid, p_tmp );
	WORDSET_ExpandStr( p_draw->p_wordset, p_wk->p_talkstr, p_tmp );
    p_wk->msgidx = GF_STR_PrintColor(&p_wk->talk, FONT_TALK, 
			p_wk->p_talkstr, 0, 0, p_wk->msgspeed, WFNOTE_COL_BLACK, NULL);
    BmpTalkWinWrite( &p_wk->talk, WINDOW_TRANS_OFF, 
			WFNOTE_BGCGX_MAIN_MSG_TALK, WFNOTE_BGPL_MAIN_TALKWIN );

    GF_BGL_BmpWinOnVReq(&p_wk->talk);

	STRBUF_Delete( p_tmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�\���I���҂�
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDLIST_TalkMsgEndCheck( const WFNOTE_FRIENDLIST* cp_wk )
{
    if( GF_MSG_PrintEndCheck( cp_wk->msgidx ) == 0 ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W���\���ɂ���
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_TalkMsgOff( WFNOTE_FRIENDLIST* p_wk )
{
	BmpTalkWinClear( &p_wk->talk, WINDOW_TRANS_OFF );
    GF_BGL_BmpWinOffVReq(&p_wk->talk);

	// �߂�̕�����������̂ŕ\��
	GF_BGL_BmpWinOnVReq( &p_wk->backmsg );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ݑI�𒆂̗F�B���X�g�C���f�b�N�X���擾
 *
 *	@param	cp_wk		���[�N
 *
 *	@return	�F�B���X�g�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static u32 WFNOTE_FRIENDLIST_FRIDX_IdxGet( const WFNOTE_FRIENDLIST* cp_wk )
{
	u32 num;
	num = cp_wk->page * WFNOTE_FRIENDLIST_PAGEFRIEND_NUM;
	num += cp_wk->pos;
	return num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j���[�̂P�s�ړ����Ƃ̃R�[���o�b�N
 *
 *	@param	p_wk		���[�N
 *	@param	param		�p�����[�^
 *	@param	mode		���[�h
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_MENULISTCallBack( BMPLIST_WORK * p_wk, u32 param, u8 mode )
{
	if( mode == 0 ){
		Snd_SePlay(WIFINOTE_MENU_MOVE_SE);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�j���A�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DeleteAnmStart( WFNOTE_FRIENDLIST* p_wk )
{
	VecFx32 vec;
	u32 pos;

	
	// �\��ON
	CLACT_SetDrawFlag( p_wk->p_clearact, TRUE );

	// �A�j������
	CLACT_AnmChg( p_wk->p_clearact, 2 );
	p_wk->last_frame = 0;

	// �I�[�g�A�j���ݒ�
	CLACT_SetAnmFlag( p_wk->p_clearact, TRUE );

	pos = p_wk->pos;

	// �|�W�V�����ɂ��킹�č��W��ݒ�
	vec.x = ((MATH_ABS(sc_SCRNAREA[ WFNOTE_DRAWAREA_MAIN ].scrn_x)+sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_x) * 8);
	vec.y = ((MATH_ABS(sc_SCRNAREA[ WFNOTE_DRAWAREA_MAIN ].scrn_y)+sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_y) * 8) + WFNOTE_FRIENDLIST_CLEARACT_Y;
	vec.x <<= FX32_SHIFT;
	vec.y <<= FX32_SHIFT;
	CLACT_SetMatrix( p_wk->p_clearact, &vec );

	// �E�B���h�E�}�X�N�ݒ�ON
	G2_SetWndOBJInsidePlane( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2, FALSE );
	G2_SetWndOutsidePlane( GX_WND_PLANEMASK_BG0|GX_WND_PLANEMASK_BG1|GX_WND_PLANEMASK_BG2|GX_WND_PLANEMASK_BG3|GX_WND_PLANEMASK_OBJ, TRUE );
	GX_SetVisibleWnd( GX_WNDMASK_OW );//*/

	// �����S����
	Snd_SePlay( WIFINOTE_CLEAN_SE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���X�g�j���A�j��	���C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDLIST_DeleteAnmMain( WFNOTE_FRIENDLIST* p_wk )
{
	u32 frame;
	
	if( CLACT_AnmActiveCheck( p_wk->p_clearact ) == FALSE ){
		return TRUE;
	}

	// �A�j�����؂�ւ����������S����
	frame = CLACT_AnmFrameGet( p_wk->p_clearact );
	if( (frame - p_wk->last_frame) >= 2 ){
		p_wk->last_frame = frame;
		Snd_SePlay( WIFINOTE_CLEAN_SE );
	}
	
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�j���\����Ԃ�����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DeleteAnmOff( WFNOTE_FRIENDLIST* p_wk )
{
	GX_SetVisibleWnd( GX_WNDMASK_NONE );
	CLACT_SetDrawFlag( p_wk->p_clearact, FALSE );
	CLACT_SetAnmFlag( p_wk->p_clearact, FALSE );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�`��G���A���[�N	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_area		�X�N���[���G���A���
 *	@param	text_cgx	�e�L�X�g�ʂ̂�����
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_Init( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNAREA* cp_area, u32 text_cgx, u32 heapID )
{
	// �X�N���[���f�[�^�ۑ�
	p_wk->scrn_area = *cp_area;

	// �r�b�g�}�b�v
	GF_BGL_BmpWinInit( &p_wk->text );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->text, 
			sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ], 
			WFNOTE_FRIENDLIST_TEXT_OFSX+MATH_ABS(cp_area->scrn_x), 
			WFNOTE_FRIENDLIST_TEXT_OFSY+MATH_ABS(cp_area->scrn_y),
			WFNOTE_FRIENDLIST_TEXT_SIZX, WFNOTE_FRIENDLIST_TEXT_SIZY,
			WFNOTE_BGPL_MAIN_MSGFONT, text_cgx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��G���A���[�N	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_Exit( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw )
{
	GF_BGL_BmpWinDel( &p_wk->text );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��G���A�ɔw�i�`��
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_scrn		�X�N���[���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_BackWrite( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn )
{
	// �w�i�`��
	GF_BGL_ScrWrite( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			cp_scrn->p_scrn[WFNOTE_SCRNDATA_BACK]->rawData, 
			MATH_ABS(p_wk->scrn_area.scrn_x), MATH_ABS(p_wk->scrn_area.scrn_y), 
			cp_scrn->p_scrn[WFNOTE_SCRNDATA_BACK]->screenWidth/8,
			cp_scrn->p_scrn[WFNOTE_SCRNDATA_BACK]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J�[�\���`��
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	pos			�J�[�\���ʒu
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_CursorWrite( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, u32 pos )
{
	int i;
	int pal;

	for( i=0; i<WFNOTE_FRIENDLIST_CURSORDATA_NUM; i++ ){
		if( pos == i ){
			pal = WFNOTE_BGPL_MAIN_BACK_6;
		}else{
			pal = WFNOTE_BGPL_MAIN_BACK_0;
		}

		// �p���b�g�J���[�����킹��
		GF_BGL_ScrPalChange( p_draw->p_bgl, 
				sc_WFNOTE_FRIENDLIST_CURSORDATA[i].bg_frame,
				MATH_ABS(p_wk->scrn_area.scrn_x) + sc_WFNOTE_FRIENDLIST_CURSORDATA[i].scrn_data.scrn_x,
				MATH_ABS(p_wk->scrn_area.scrn_y) + sc_WFNOTE_FRIENDLIST_CURSORDATA[i].scrn_data.scrn_y,
				sc_WFNOTE_FRIENDLIST_CURSORDATA[i].scrn_data.scrn_sizx,
				sc_WFNOTE_FRIENDLIST_CURSORDATA[i].scrn_data.scrn_sizy, pal );
	}

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
	
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[���\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	p_charsys	�L�����N�^�V�X�e��
 *	@param	cp_scrn		�X�N���[���V�X�e��
 *	@param	cp_idx		�C���f�b�N�X�f�[�^
 *	@param	page		�y�[�W
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_WritePlayer( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, WF_2DCSYS* p_charsys, const WFNOTE_SCRNDATA* cp_scrn, const WFNOTE_IDXDATA* cp_idx, u32 page, u32 heapID )
{
	int i;
	int idx;
	WIFI_LIST* p_list;

	p_list = SaveData_GetWifiListData( p_data->p_save );
	
	// �N���A
	GF_BGL_BmpWinDataFill( &p_wk->text, 0 );

	// �A�N�^�[���c���Ă�����͂�
	WFNOTE_FRIENDLIST_DRAWAREA_CharWkDel( p_wk, p_draw );
	
	// ���O�ƃA�N�^�[�̐���
	idx = page * WFNOTE_FRIENDLIST_PAGEFRIEND_NUM;
	for( i=0; i<WFNOTE_FRIENDLIST_PAGEFRIEND_NUM; i++ ){
		if( cp_idx->friendnum > (idx+i) ){
			// �\������
			WFNOTE_FRIENDLIST_DRAWAREA_SetPlayer( p_wk, p_charsys, p_draw, cp_scrn, i, p_list, cp_idx->fridx[idx+i],  heapID );
		}else{
			// �v���C���[�͕\�����Ȃ�
			WFNOTE_FRIENDLIST_DRAWAREA_SetNoPlayer( p_wk, p_draw, cp_scrn, i );
		}
	}

	// ��������BG���X�V
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ] );

	// �e�L�X�g�ʕ\��
	GF_BGL_BmpWinOnVReq( &p_wk->text );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��OFF
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_scrn		�X�N���[���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_DrawOff( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn )
{
	// �A�N�^�[�j��
	WFNOTE_FRIENDLIST_DRAWAREA_CharWkDel( p_wk, p_draw );

	// �\��OFF
	GF_BGL_BmpWinOffVReq( &p_wk->text );

	// �w�i�N���A
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			0, 
			MATH_ABS(p_wk->scrn_area.scrn_x), MATH_ABS(p_wk->scrn_area.scrn_y), 
			p_wk->scrn_area.scrn_sizx,
			p_wk->scrn_area.scrn_sizy, 0 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );

	// �X�N���[���ʂ��N���A
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ],
			0, 
			MATH_ABS(p_wk->scrn_area.scrn_x), MATH_ABS(p_wk->scrn_area.scrn_y), 
			p_wk->scrn_area.scrn_sizx,
			p_wk->scrn_area.scrn_sizy, 0 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��C�ɃL�����N�^���[�N��j������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_CharWkDel( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw )
{
	int i;

	for( i=0; i<WFNOTE_FRIENDLIST_PAGEFRIEND_NUM; i++ ){
		if( p_wk->p_clwk[ i ] ){
			WF_2DC_WkDel( p_wk->p_clwk[ i ] );
			p_wk->p_clwk[ i ] = NULL;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�f�[�^�\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_charsys	�L�����N�^�V�X�e��
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_scrn		�X�N���[���f�[�^
 *	@param	pos			�ʒu
 *	@param	p_list		WiFi���X�g
 *	@param	idx			WiFi���X�g���̃C���f�b�N�X
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_SetPlayer( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WF_2DCSYS* p_charsys, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn, u32 pos, WIFI_LIST* p_list, u32 idx, u32 heapID )
{
	int sex;
	int view;
	WF_2DC_WKDATA adddata;
	STRBUF* p_str;
	int x, y;
    static const GF_PRINTCOLOR sc_SEXCOL[] = {WFNOTE_COL_BLUE,WFNOTE_COL_RED,WFNOTE_COL_BLACK}; 

	view = WifiList_GetFriendInfo( p_list, idx, WIFILIST_FRIEND_UNION_GRA );
	sex = WifiList_GetFriendInfo( p_list, idx, WIFILIST_FRIEND_SEX );

	if( sex == PM_NEUTRAL ){
		// ���Ȃ��G��\��
		WFNOTE_FRIENDLIST_DRAWAREA_SetNoPlayer( p_wk, p_draw, cp_scrn, pos );
	}else{
		// �L�����N�^�\��
		adddata.x = ((p_wk->scrn_area.scrn_x+sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_x) * 8) + WFNOTE_FRIENDLIST_PLAYER_X;
		adddata.y = ((p_wk->scrn_area.scrn_y+sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_y) * 8) + WFNOTE_FRIENDLIST_PLAYER_Y;
		adddata.pri = WFNOTE_FRIENDLIST_PRI;
		adddata.bgpri = WFNOTE_FRIENDLIST_BGPRI;
		p_wk->p_clwk[ pos ] = WF_2DC_WkAdd( p_charsys, &adddata, view, heapID );
	}

	//  ���O�\��
	x = ((sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_x) * 8) + WFNOTE_FRIENDLIST_PLAYERNAME_X;
	y = ((sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_y) * 8) + WFNOTE_FRIENDLIST_PLAYERNAME_Y;
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
    STRBUF_SetStringCode( p_str, WifiList_GetFriendNamePtr(p_list,idx) );
    GF_STR_PrintColor( &p_wk->text, FONT_SYSTEM, 
			p_str,
			x, y, MSG_NO_PUT, sc_SEXCOL[sex], NULL);

	STRBUF_Delete( p_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�v���C���[�̂��Ȃ���Ԃ�\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_scrn		�X�N���[���f�[�^
 *	@param	pos			�ʒu
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDLIST_DRAWAREA_SetNoPlayer( WFNOTE_FRIENDLIST_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNDATA* cp_scrn, u32 pos )
{
	//  ���̈ʒu�̃X�N���[���ɂ͂Ăȃ}�[�N���o��
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			MATH_ABS(p_wk->scrn_area.scrn_x) + sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_x,
			MATH_ABS(p_wk->scrn_area.scrn_y) + sc_WFNOTE_FRIENDLIST_CURSORDATA[pos].scrn_data.scrn_y,
			WFNOTE_FRIENDLIST_NOTPlAYER_SIZX, WFNOTE_FRIENDLIST_NOTPlAYER_SIZY,
			cp_scrn->p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDLIST_NOTPlAYERSCRN_X, WFNOTE_FRIENDLIST_NOTPlAYERSCRN_Y, 
			cp_scrn->p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			cp_scrn->p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );

}


//-------------------------------------
///	�F�B�R�[�h���͉�ʃ��[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�R�[�h���͉��	������
 *
 *	@param	p_wk		���[�N	
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_CODEIN_Init( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	WFNOTE_CODEIN_DrawInit( p_wk, p_data, p_draw, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�R�[�h���͉��	���C���֐�
 *
 *	@param	p_wk		���[�N
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static WFNOTE_STRET WFNOTE_CODEIN_Main( WFNOTE_CODEIN* p_wk, WFNOTE_WK* p_sys, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	int block[ CODE_BLOCK_MAX ];


	switch( p_data->subseq ){
	case WFNOTE_CODEIN_SEQ_NAMEIN:

		// ���O���́A�R�[�h���̓p�����[�^�쐬
		p_wk->p_namein = WFNOTE_CODEIN_NameInParamMake( p_wk, p_data, heapID );
		CodeIn_BlockDataMake_4_4_4( block );
		p_wk->p_codein = CodeInput_ParamCreate( heapID, FRIENDCODE_MAXLEN, 
				block, SaveData_GetConfig(p_data->p_save) ); 

		WFNOTE_DrawExit( p_sys );	// ��ʃ��[�N�S�j��
        p_wk->p_subproc = PROC_Create( &NameInProcData, p_wk->p_namein, heapID );
		p_data->subseq = WFNOTE_CODEIN_SEQ_NAMEIN_WAIT;
		break;
		
	case WFNOTE_CODEIN_SEQ_NAMEIN_WAIT:
		// �I���҂�
		if( ProcMain( p_wk->p_subproc ) == FALSE ){
			break;
		}
		PROC_Delete( p_wk->p_subproc );

		// �L�����Z���`�F�b�N
		if( p_wk->p_namein->cancel==0 ){
			// ����

			p_wk->p_subproc = PROC_Create( &CodeInput_ProcData, p_wk->p_codein, heapID ); 
			p_data->subseq = WFNOTE_CODEIN_SEQ_CODEIN_WAIT;
		}else{
			// �I����
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_MODE_SELECT, WFNOTE_MODESELECT_SEQ_INIT );
			p_data->subseq = WFNOTE_CODEIN_SEQ_END;
		}
		break;
		
	case WFNOTE_CODEIN_SEQ_CODEIN_WAIT:
		// �I���҂�
		if( ProcMain( p_wk->p_subproc ) == FALSE ){
			break;
		}
		PROC_Delete( p_wk->p_subproc );

		// �f�[�^��ݒ�
		WFNOTE_DATA_CODEIN_Set( p_data, p_wk->p_namein->strbuf, p_wk->p_codein->strbuf );
		WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_MODE_SELECT, WFNOTE_MODESELECT_SEQ_CODECHECK_INIT );
		p_data->subseq = WFNOTE_CODEIN_SEQ_END;
		break;
		
	case WFNOTE_CODEIN_SEQ_END:
		// ���O���́A�R�[�h���͉�ʂ̔j��
		NameIn_ParamDelete( p_wk->p_namein );
		CodeInput_ParamDelete( p_wk->p_codein );

		WFNOTE_DrawInit( p_sys, heapID );	// ��ʕ��A
		return WFNOTE_STRET_FINISH; 



	case WFNOTE_CODEIN_SEQ_NAMEINONLY:	// ���O���͂̂�
		// ���O���́A�R�[�h���̓p�����[�^�쐬
		p_wk->p_namein = WFNOTE_CODEIN_NameInParamMake( p_wk, p_data, heapID );
		WFNOTE_DrawExit( p_sys );	// ��ʃ��[�N�S�j��
        p_wk->p_subproc = PROC_Create( &NameInProcData, p_wk->p_namein, heapID );
		p_data->subseq = WFNOTE_CODEIN_SEQ_NAMEINONLY_WAIT;
		break;
		
	case WFNOTE_CODEIN_SEQ_NAMEINONLY_WAIT:	// ���O���͂̂�
		// �I���҂�
		if( ProcMain( p_wk->p_subproc ) == FALSE ){
			break;
		}
		PROC_Delete( p_wk->p_subproc );

		// �L�����Z���`�F�b�N
		if( p_wk->p_namein->cancel==0 ){
			// �����Ɠ��͂��ꂽ�̂ŁA���̏ꍇ��FRIENDLIST�ɖ߂�
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_FRIENDLIST, WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAIT );
			// �f�[�^��ݒ�
			WFNOTE_DATA_CODEIN_Set( p_data, p_wk->p_namein->strbuf, NULL );
			p_data->subseq = WFNOTE_CODEIN_SEQ_NAMEINONLY_END;
		}else{
			// �I����
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_FRIENDLIST, WFNOTE_FRINEDLIST_SEQ_NAMECHG_WAITWIPE );
			p_data->subseq = WFNOTE_CODEIN_SEQ_NAMEINONLY_END;
		}
		break;
		
	case WFNOTE_CODEIN_SEQ_NAMEINONLY_END:	// ���O���͂̂�
		// ���O���́A�R�[�h���͉�ʂ̔j��
		NameIn_ParamDelete( p_wk->p_namein );

		WFNOTE_DrawInit( p_sys, heapID );	// ��ʕ��A
		return WFNOTE_STRET_FINISH; 
	}
	return WFNOTE_STRET_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�R�[�h���͉��	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_CODEIN_Exit( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{

	WFNOTE_CODEIN_DrawExit( p_wk, p_data, p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֌W�̏�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFNOTE_CODEIN_DrawInit( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֌W�̔j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e�� 
 */
//-----------------------------------------------------------------------------
static void WFNOTE_CODEIN_DrawExit( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	���O���́@���[�N�쐬
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static NAMEIN_PARAM* WFNOTE_CODEIN_NameInParamMake( WFNOTE_CODEIN* p_wk, WFNOTE_DATA* p_data, u32 heapID )
{
	NAMEIN_PARAM* p_param;

	p_param = NameIn_ParamAllocMake( heapID, NAMEIN_FRIENDNAME, 0, 
							NAMEIN_PERSON_LENGTH, 
							SaveData_GetConfig(p_data->p_save) );

	// ���O���͉�ʏ�����data�`�F�b�N
	if( p_data->namein_init.init == TRUE ){
		#if( PM_LANG == LANG_JAPAN )
		{
			if( IsJapaneseCode( *p_data->namein_init.cp_str ) ){
				STRBUF_SetStringCode( p_param->strbuf, p_data->namein_init.cp_str );
			}else{
				STRBUF_Clear( p_param->strbuf );
			}
		}
		#else
		{
			if( IsJapaneseCode( *p_data->namein_init.cp_str ) == FALSE ){
				STRBUF_SetStringCode( p_param->strbuf, p_data->namein_init.cp_str );
			}else{
				STRBUF_Clear( p_param->strbuf );
			}
		}
		#endif
		p_data->namein_init.init = FALSE;
	}
	return p_param;
}

//-------------------------------------
///	�����̗F�B�R�[�h�m�F��ʃ��[�N
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�����̗F�B�R�[�h�m�F��ʃ��[�N������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MYCODE_Init( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	WFNOTE_MYCODE_DrawInit( p_wk, p_data, p_draw, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̗F�B�R�[�h�m�F��� ���C������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *
 *	@retval	WFNOTE_STRET_CONTINUE,	// �Â�
 *	@retval	WFNOTE_STRET_FINISH,	// ��Ƃ͏I�����
 */
//-----------------------------------------------------------------------------
static WFNOTE_STRET WFNOTE_MYCODE_Main( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	switch( p_data->subseq ){
	case WFNOTE_MYCODE_SEQ_INIT:
		WFNOTE_MYCODE_SEQ_Init( p_wk, p_data, p_draw );
		p_data->subseq = WFNOTE_MYCODE_SEQ_MAIN;
		break;
		
	case WFNOTE_MYCODE_SEQ_MAIN:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay(WIFINOTE_DECIDE_SE);
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_MODE_SELECT, WFNOTE_MODESELECT_SEQ_INIT_NOFADE );
			WFNOTE_MYCODE_DrawOff( p_wk, p_draw );	// �\��OFF
			return WFNOTE_STRET_FINISH; 
		}
		break;
	}
	return WFNOTE_STRET_CONTINUE; 
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̗F�B�R�[�h�m�F��ʃ��[�N�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MYCODE_Exit( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	WFNOTE_MYCODE_DrawExit( p_wk, p_data, p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֌W�̏�����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MYCODE_DrawInit( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u64 code;
	DWCUserData* p_userdata;
	WIFI_LIST* p_list;
	STRBUF* p_str;
	STRBUF* p_tmp;

	// �r�b�g�}�b�v�쐬
	GF_BGL_BmpWinInit( &p_wk->msg );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->msg, 
			sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG], 
			WFNOTE_MYCODE_MSG_X, WFNOTE_MYCODE_MSG_Y, 
			WFNOTE_MYCODE_MSG_SIZX, WFNOTE_MYCODE_MSG_SIZY, 
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_MYCODE_CGX );
	GF_BGL_BmpWinInit( &p_wk->code );
	GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->code, 
			sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG], 
			WFNOTE_MYCODE_CODE_X, WFNOTE_MYCODE_CODE_Y, 
			WFNOTE_MYCODE_CODE_SIZX, WFNOTE_MYCODE_CODE_SIZY, 
			WFNOTE_BGPL_MAIN_MSGFONT, WFNOTE_MYCODE_CODE_CGX );

	GF_BGL_BmpWinDataFill( &p_wk->msg, 0 );
	GF_BGL_BmpWinDataFill( &p_wk->code, 0 );

	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �����̃R�[�h�擾
	p_list = SaveData_GetWifiListData(p_data->p_save);
	p_userdata = WifiList_GetMyUserInfo( p_list );
	code = DWC_CreateFriendKey( p_userdata );

	if( code != 0 ){
		// �R�[�h����
		// ���b�Z�[�W
        MSGMAN_GetString( p_draw->p_msgman, msg_wifi_note_24, p_str );
		GF_STR_PrintColor( &p_wk->msg, FONT_SYSTEM,
						   p_str, 0, 0, MSG_NO_PUT, WFNOTE_COL_BLACK, NULL);

		// ����
		WFNOTE_DRAW_FriendCodeSetWordset( p_draw, code );
        MSGMAN_GetString( p_draw->p_msgman, msg_wifi_note_28, p_tmp );
		WORDSET_ExpandStr( p_draw->p_wordset, p_str, p_tmp );
		GF_STR_PrintColor( &p_wk->code, FONT_SYSTEM,
						   p_str, WFNOTE_MYCODE_CODE_PR_X, 0, 
						   MSG_NO_PUT, WFNOTE_COL_BLACK, NULL);
	}else{
		// �R�[�h�Ȃ�
		// ���b�Z�[�W
        MSGMAN_GetString( p_draw->p_msgman, msg_wifi_note_25, p_str );
		GF_STR_PrintColor( &p_wk->msg, FONT_SYSTEM,
						   p_str, 0, 0, MSG_NO_PUT, WFNOTE_COL_BLACK, NULL);

		// ����
	}

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );

	// �X�N���[���f�[�^�ǂݍ���
	p_wk->p_scrnbuff = ArcUtil_HDL_ScrnDataGet( p_draw->p_handle,
			NARC_pl_wifinote_techo_04_NSCR, FALSE, &p_wk->p_scrn, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��֌W�̔j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MYCODE_DrawExit( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	GF_BGL_BmpWinDel( &p_wk->msg );
	GF_BGL_BmpWinDel( &p_wk->code );
	sys_FreeMemoryEz( p_wk->p_scrnbuff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�������@�V�[�P���X
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MYCODE_SEQ_Init( WFNOTE_MYCODE* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	
	// �^�C�g���ݒ�
	WORDSET_RegisterPlayerName( p_draw->p_wordset, 0, SaveData_GetMyStatus( p_data->p_save ) );
	WFNOTE_DRAW_BmpTitleWrite( p_draw, msg_wifi_note_23 );

	// �w�i�ݒ�
	GF_BGL_ScrWrite( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			p_wk->p_scrn->rawData, 0, 0, 
			p_wk->p_scrn->screenWidth/8, p_wk->p_scrn->screenHeight/8 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );

	// ������ݒ�
    GF_BGL_BmpWinOnVReq(&p_wk->msg);
    GF_BGL_BmpWinOnVReq(&p_wk->code);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��OFF
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_MYCODE_DrawOff( WFNOTE_MYCODE* p_wk, WFNOTE_DRAW* p_draw )
{
	// �^�C�g��
	WFNOTE_DRAW_BmpTitleOff( p_draw );

	// SCROLL��
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL], 0, 0, 0, 32, 24, 0 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );

	// msg
	GF_BGL_BmpWinOffVReq(&p_wk->msg);
	GF_BGL_BmpWinOffVReq(&p_wk->code);

	
	// SCRMSG��
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG], 0, 0, 0, 32, 24, 0 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG] );
}



//-------------------------------------
///	�F�B�ڍ׏��
//=====================================
//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�ڍו\�����	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_Init( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	// SaveData����t�����e�B�A��\�����邩�`�F�b�N����
	// ���͏�ɕ\��
	p_wk->frontier_draw = SysFlag_ArriveGet(SaveData_GetEventWork(p_data->p_save),FLAG_ARRIVE_D32R0101);
	
	WFNOTE_FRIENDINFO_DrawInit( p_wk, p_data, p_draw, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�ڍו\�����	���C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static WFNOTE_STRET WFNOTE_FRIENDINFO_Main( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	BOOL result;

	switch( p_data->subseq ){
	case WFNOTE_FRIENDINFO_SEQ_INIT:
		p_wk->rep = FALSE;
		p_wk->page = 0;
	case WFNOTE_FRIENDINFO_SEQ_INITRP:
		// �`��J�n
		WFNOTE_FRIENDINFO_DrawOn( p_wk, p_data, p_draw, heapID );
		p_data->subseq = WFNOTE_FRIENDINFO_SEQ_MAIN;
		break;
		
	case WFNOTE_FRIENDINFO_SEQ_MAIN:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( WIFINOTE_DECIDE_SE );
			p_data->subseq = WFNOTE_FRIENDINFO_SEQ_END;
		}else{
			if( sys.repeat & PAD_KEY_LEFT ){
				WFNOTE_FRIENDINFO_PageChange( p_wk, -1 );
				p_data->subseq = WFNOTE_FRIENDINFO_SEQ_SCRINIT;
				p_wk->way = WF_COMMON_LEFT;
			}else if( sys.repeat & PAD_KEY_RIGHT ){
				WFNOTE_FRIENDINFO_PageChange( p_wk, 1 );
				p_data->subseq = WFNOTE_FRIENDINFO_SEQ_SCRINIT;
				p_wk->way = WF_COMMON_RIGHT;
			}else if( sys.cont & PAD_KEY_UP ){
				p_data->subseq = WFNOTE_FRIENDINFO_SEQ_FRIENDCHG;
				p_wk->way = WF_COMMON_TOP;
				p_wk->wait = WFNOTE_FRIENDINFO_PAGECHG_WAIT;
			}else if( sys.cont & PAD_KEY_DOWN ){
				p_data->subseq = WFNOTE_FRIENDINFO_SEQ_FRIENDCHG;
				p_wk->way = WF_COMMON_BOTTOM;
				p_wk->wait = WFNOTE_FRIENDINFO_PAGECHG_WAIT;
			}
		}
		if( (sys.cont & (PAD_KEY_UP|PAD_KEY_DOWN)) == 0 ){
			p_wk->rep = FALSE;
		}
		break;
		
	case WFNOTE_FRIENDINFO_SEQ_SCRINIT:
		WFNOTE_FRIENDINFO_ScrollInit( p_wk, p_data, p_draw, heapID );
		WFNOTE_FRIENDINFO_ScrollMain( p_wk, p_data, p_draw, heapID );
		p_data->subseq = WFNOTE_FRIENDINFO_SEQ_SCR;
		break;
		
	case WFNOTE_FRIENDINFO_SEQ_SCR:
		result = WFNOTE_FRIENDINFO_ScrollMain( p_wk, p_data, p_draw, heapID );
		if( result == TRUE ){
			p_data->subseq = WFNOTE_FRIENDINFO_SEQ_MAIN;
		}
		break;

		// �\������l��������
	case WFNOTE_FRIENDINFO_SEQ_FRIENDCHG:
		if( p_wk->rep ){	// ���s�[�g���Ȃ�E�G�C�g
			p_wk->wait --;
			if( p_wk->wait > 0 ){
				break;
			}
		}
		
		result = WFNOTE_FRIENDINFO_SelectListIdxAdd( p_data, p_wk->way );
		if( result ){

			Snd_SePlay( WIFINOTE_MOVE_SE );

			WFNOTE_FRIENDINFO_DrawOff( p_wk, p_draw );
			WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_FRIENDINFO, WFNOTE_FRIENDINFO_SEQ_INITRP );
			// �������㉺��������Ă����烊�s�[�g
			p_wk->rep = TRUE;
			return WFNOTE_STRET_FINISH;
		}

		// �ύX�ł��Ȃ������̂Ō��ɖ߂�
		p_data->subseq = WFNOTE_FRIENDINFO_SEQ_MAIN;
		break;
		
	case WFNOTE_FRIENDINFO_SEQ_END:
		WFNOTE_FRIENDINFO_DrawOff( p_wk, p_draw );
		WFNOTE_DATA_SetReqStatus( p_data, WFNOTE_STATUS_FRIENDLIST, WFNOTE_FRIENDLIST_SEQ_INFORET );
		return WFNOTE_STRET_FINISH;
	}
	return WFNOTE_STRET_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�F�B�ڍו\�����	�j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_Exit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	WFNOTE_FRIENDINFO_DrawExit( p_wk, p_data, p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[��������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_ScrollInit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u32 drawarea;
	
	p_wk->count = 0;

	// ���̃A�j�����Ƃ߂�
	WFNOTE_DRAW_YazirushiSetAnmFlag( p_draw, FALSE );	

	// �X�N���[�������Ƀy�[�W����������
	if( p_wk->way == WF_COMMON_LEFT ){
		drawarea = WFNOTE_DRAWAREA_LEFT;
	}else{
		drawarea = WFNOTE_DRAWAREA_RIGHT;
	}
	WFNOTE_FRINEDINFO_DrawPage( p_wk, p_data, p_draw, drawarea, p_wk->page, heapID );

	// �X�N���[����
	Snd_SePlay( WIFINOTE_SCRLL_SE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[�����C��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDINFO_ScrollMain( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	u32 draw_area;
	s32 scrll_x;
	NNSG2dViewRect rect;
	u32 idx;
	static void (*pSHIORIFUNC[ 4 ])( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DRAW* p_draw, u32 page0, u32 page1 ) = {
		WFNOTE_FRIENDINFO_ShioriAnm00,
		NULL,
		WFNOTE_FRIENDINFO_ShioriAnm01,
		NULL
	};

	// �J�E���^���I������炷�ׂĂ�`�悵�ďI���
	if( p_wk->count > WFNOTE_FRIENDINFO_SCRLL_COUNT ){
		

		// �X�N���[�����W�����ɖ߂�
		GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ], GF_BGL_SCROLL_X_SET, 0 );
		GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ], GF_BGL_SCROLL_X_SET, 0 );

		// ��Ɨp�O���t�B�b�N������
		if( p_wk->way == WF_COMMON_LEFT ){
			draw_area = WFNOTE_DRAWAREA_LEFT;
		}else{
			draw_area = WFNOTE_DRAWAREA_RIGHT;
		}
		// ���C���ʂɕ\��
		WFNOTE_FRINEDINFO_DrawPage( p_wk, p_data, p_draw, WFNOTE_DRAWAREA_MAIN, 
				p_wk->page, heapID );

		// ��󓮍�J�n
		WFNOTE_DRAW_YazirushiSetAnmFlag( p_draw, TRUE );
		return TRUE;
	}

	
	scrll_x = (p_wk->count * WFNOTE_FRIENDINFO_SCRLL_SIZX) / WFNOTE_FRIENDINFO_SCRLL_COUNT;

	if( p_wk->way == WF_COMMON_LEFT ){
		scrll_x = -scrll_x;
	}

	//  �X�N���[�����W�ݒ�
	GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ], GF_BGL_SCROLL_X_SET, scrll_x );
	GF_BGL_ScrollReq( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ], GF_BGL_SCROLL_X_SET, scrll_x );

	// �A�j���[�V����
	if( p_wk->count % (WFNOTE_FRIENDINFO_SCRLL_COUNT/4) ){
		idx = p_wk->count / (WFNOTE_FRIENDINFO_SCRLL_COUNT/4);
		if( pSHIORIFUNC[idx] ){
			pSHIORIFUNC[idx]( p_wk, p_draw, p_wk->page, p_wk->lastpage );
		}
	}

	// �J�E���g
	p_wk->count ++;

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`�揉����
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�v
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DrawInit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	int i, j;
	static const u32 sc_DRAWAREA_CGX[ WFNOTE_DRAWAREA_NUM ] = {
		WFNOTE_FRIENDINFO_TEXT_CGX0,
		WFNOTE_FRIENDINFO_TEXT_CGX1,
		WFNOTE_FRIENDINFO_TEXT_CGX1,
	};
	
	// �e�y�[�W�̃X�N���[���f�[�^�ǂݍ���
	WFNOTE_FRIENDINFO_SCRNDATA_Init( &p_wk->scrn, p_draw, heapID );

	// �`��G���A������
	for( i=0; i<WFNOTE_DRAWAREA_NUM; i++ ){	
		WFNOTE_FRIENDINFO_DRAWAREA_Init( &p_wk->drawarea[i],  p_draw, &sc_SCRNAREA[i],
				sc_DRAWAREA_CGX[i], heapID );
		// �`��G���A���̃r�b�g�}�b�v�쐬
		for( j=0; j<WFNOTE_FRIENDINFO_PAGE_NUM; j++ ){
			WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpInit( &p_wk->drawarea[i], p_draw, j,
					sc_WFNOTE_FRIENDINFO_PAGEBMPNUM[j], 
					scp_WFNOTE_FRIENDINFO_PAGEBMP[j], heapID );
		}
	}


}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��j��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 */	
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DrawExit( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw )
{
	int i, j;

	// �`��G���A�j��
	for( i=0; i<WFNOTE_DRAWAREA_NUM; i++ ){	
		for( j=0; j<WFNOTE_FRIENDINFO_PAGE_NUM; j++ ){
			WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpExit( &p_wk->drawarea[i], j );
		}
		WFNOTE_FRIENDINFO_DRAWAREA_Exit( &p_wk->drawarea[i] );
	}
	
	// �e�y�[�W�̃X�N���[���f�[�^�j��
	WFNOTE_FRIENDINFO_SCRNDATA_Exit( &p_wk->scrn );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���J�n
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DrawOn( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	// �^�C�g���\��
	WFNOTE_DRAW_FriendNameSetWordset( p_draw, p_data->p_save, 
			p_data->idx.fridx[p_data->select_listidx], heapID );
	WFNOTE_DRAW_BmpTitleWrite( p_draw, msg_wifi_note_14 );

	// �y�[�W�̕\��
	WFNOTE_FRINEDINFO_DrawPage( p_wk, p_data, p_draw, WFNOTE_DRAWAREA_MAIN, 
			p_wk->page, heapID );	

	// ���\��
	WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, TRUE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��OFF
 *
 *	@param	p_wk	���[�N
 *	@param	p_draw	�`��V�X�e��
 *
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DrawOff( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DRAW* p_draw )
{
	int i;
	
	WFNOTE_DRAW_YazirushiSetDrawFlag( p_draw, FALSE );

	for( i=0; i<WFNOTE_DRAWAREA_NUM; i++ ){
		WFNOTE_FRIENDINFO_DRAWAREA_PageOff( &p_wk->drawarea[ i ], p_draw );
	}
	WFNOTE_FRIENDINFO_ShioriOff( p_wk, p_draw );

	WFNOTE_DRAW_BmpTitleOff( p_draw );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�y�[�W�̕\��
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	drawarea	�`��G���A
 *	@param	page		�y�[�W
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRINEDINFO_DrawPage( WFNOTE_FRIENDINFO* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 drawarea, u32 page, u32 heapID )
{
	if( drawarea == WFNOTE_DRAWAREA_MAIN ){
		p_wk->page = page;	// �\���y�[�W�X�V
		// ������X�V
		WFNOTE_FRIENDINFO_ShioriDraw( p_wk, p_draw, page, heapID );
	}
	// �y�[�W�`��
	WFNOTE_FRIENDINFO_DRAWAREA_Page( &p_wk->drawarea[ drawarea ], p_data, 
			p_draw, page, &p_wk->scrn, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������`��	ON
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	page		�y�[�W
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_ShioriDraw( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw, u32 page, u32 heapID )
{
	u32 x;
	
	// ��{���܂��`��
	// �t�����e�B�AOFF�Ȃ��{�������ύX
	if( p_wk->frontier_draw == TRUE ){
		GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
				WFNOTE_FRIENDINFO_SHIORI_X, WFNOTE_FRIENDINFO_SHIORI_Y,
				WFNOTE_FRIENDINFO_SHIORI_SIZX, WFNOTE_FRIENDINFO_SHIORI_SIZY,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
				WFNOTE_FRIENDINFO_SHIORISCRN_X, WFNOTE_FRIENDINFO_SHIORISCRN_Y, 
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	}else{
		GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
				WFNOTE_FRIENDINFO_SHIORI_X, WFNOTE_FRIENDINFO_SHIORI_Y,
				WFNOTE_FRIENDINFO_SHIORI_SIZX, WFNOTE_FRIENDINFO_SHIORI_SIZY,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
				WFNOTE_FRIENDINFO_SHIORISCRN_X, WFNOTE_FRIENDINFO_SHIORISCRN_Y+3, 
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	}

	// ���̃y�[�W�̂������`��
	x = WFNOTE_FRIENDINFO_SHIORI_X + (WFNOTE_FRIENDINFO_SHIORI_ONEX*page) + WFNOTE_FRIENDINFO_SHIORI_OFSX;
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			x, WFNOTE_FRIENDINFO_SHIORI_Y + WFNOTE_FRIENDINFO_SHIORI_OFSY,
			WFNOTE_FRIENDINFO_SHIORIPOS_SIZ, WFNOTE_FRIENDINFO_SHIORIPOS_SIZ,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDINFO_SHIORISCRNPOS_X, WFNOTE_FRIENDINFO_SHIORISCRNPOS_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );


	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������OFF
 *	
 *	@param	p_wk		���[�N	
 *	@param	p_draw		�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_ShioriOff( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw )
{
	// ������ԂԂ�
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			WFNOTE_FRIENDINFO_SHIORI_X, WFNOTE_FRIENDINFO_SHIORI_Y,
			WFNOTE_FRIENDINFO_SHIORI_SIZX, WFNOTE_FRIENDINFO_SHIORI_SIZY,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDINFO_CL_SHIORISCRN_X, WFNOTE_FRIENDINFO_CL_SHIORISCRN_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������A�j���O�O
 *	
 *	@param	p_wk	���[�N
 *	@param	p_draw	�`��V�X�e��
 *	@param	page00	���̃y�[�W
 *	@param	page01	�O�̃y�[�W
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_ShioriAnm00( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw, u32 page00, u32 page01 )
{
	int x; 

	// ���̃y�[�W�̂������`��
	x = WFNOTE_FRIENDINFO_SHIORI_X + (WFNOTE_FRIENDINFO_SHIORI_ONEX*page01) + WFNOTE_FRIENDINFO_SHIORI_OFSX;
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			x, WFNOTE_FRIENDINFO_SHIORI_Y + WFNOTE_FRIENDINFO_SHIORI_OFSY,
			WFNOTE_FRIENDINFO_SHIORIPOS_SIZ, WFNOTE_FRIENDINFO_SHIORIPOS_SIZ,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDINFO_EFFECTSHIORISCRNPOS_X, 
			WFNOTE_FRIENDINFO_EFFECTSHIORISCRNPOS_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������A�j���O�P
 *	
 *	@param	p_wk	���[�N
 *	@param	p_draw	�`��V�X�e��
 *	@param	page00	���̃y�[�W
 *	@param	page01	�O�̃y�[�W
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_ShioriAnm01( WFNOTE_FRIENDINFO* p_wk,  WFNOTE_DRAW* p_draw, u32 page00, u32 page01 )
{
	int x; 

	// ��{���܂��`��
	// �t�����e�B�AOFF�Ȃ��{�������ύX
	if( p_wk->frontier_draw == TRUE ){
		GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
				WFNOTE_FRIENDINFO_SHIORI_X, WFNOTE_FRIENDINFO_SHIORI_Y,
				WFNOTE_FRIENDINFO_SHIORI_SIZX, WFNOTE_FRIENDINFO_SHIORI_SIZY,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
				WFNOTE_FRIENDINFO_SHIORISCRN_X, WFNOTE_FRIENDINFO_SHIORISCRN_Y, 
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	}else{
		GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
				WFNOTE_FRIENDINFO_SHIORI_X, WFNOTE_FRIENDINFO_SHIORI_Y,
				WFNOTE_FRIENDINFO_SHIORI_SIZX, WFNOTE_FRIENDINFO_SHIORI_SIZY,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
				WFNOTE_FRIENDINFO_SHIORISCRN_X, WFNOTE_FRIENDINFO_SHIORISCRN_Y+3, 
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
				p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );
	}

	// ���̃y�[�W�̂������`��
	x = WFNOTE_FRIENDINFO_SHIORI_X + (WFNOTE_FRIENDINFO_SHIORI_ONEX*page00) + WFNOTE_FRIENDINFO_SHIORI_OFSX;
	GF_BGL_ScrWriteExpand( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_BACK ],
			x, WFNOTE_FRIENDINFO_SHIORI_Y + WFNOTE_FRIENDINFO_SHIORI_OFSY,
			WFNOTE_FRIENDINFO_SHIORIPOS_SIZ, WFNOTE_FRIENDINFO_SHIORIPOS_SIZ,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->rawData, 
			WFNOTE_FRIENDINFO_SHIORISCRNPOS_X, 
			WFNOTE_FRIENDINFO_SHIORISCRNPOS_Y, 
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenWidth/8,
			p_draw->scrn.p_scrn[WFNOTE_SCRNDATA_KIT]->screenHeight/8 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_BACK] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�����X�g�C���f�b�N�X�l�@�ύX
 *
 *	@param	p_data	�f�[�^
 *	@param	way		����		�ォ����
 *
 *	@retval	TRUE	�ύX����
 *	@retval	FALSE	�ύX�ł��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDINFO_SelectListIdxAdd( WFNOTE_DATA* p_data, WF_COMMON_WAY way )
{
	int i;
	u32 sex;
	s32 select_listidx;
	WIFI_LIST* p_wifilist;

	p_wifilist = SaveData_GetWifiListData( p_data->p_save );
	select_listidx = p_data->select_listidx;
	
	if( way == WF_COMMON_TOP ){
		for( i=0; i<p_data->idx.friendnum-1; i++ ){
			select_listidx --;
			if( select_listidx < 0 ){
				select_listidx += p_data->idx.friendnum;
			}
			
			// ���̐l�̐��ʂ��`�F�b�N
			sex = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[select_listidx], WIFILIST_FRIEND_SEX );
			if( sex != PM_NEUTRAL ){
				p_data->select_listidx = select_listidx;
				return TRUE;
			}
		}
	}else{
		for( i=0; i<p_data->idx.friendnum-1; i++ ){
			select_listidx = (select_listidx + 1) % p_data->idx.friendnum;
			
			// ���̐l�̐��ʂ��`�F�b�N
			sex = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[select_listidx], WIFILIST_FRIEND_SEX );
			if( sex != PM_NEUTRAL ){
				p_data->select_listidx = select_listidx;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^������
 *
 *	@param	p_scrn		�X�N���[���f�[�^�i�[��
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_SCRNDATA_Init( WFNOTE_FRIENDINFO_SCRNDATA* p_scrn, WFNOTE_DRAW* p_draw, u32 heapID )
{
	static const u8 sc_SCRNIDX[WFNOTE_FRIENDINFO_PAGE_NUM] = {
		NARC_pl_wifinote_techo_03_00_NSCR,
		NARC_pl_wifinote_techo_03_06_NSCR,
		NARC_pl_wifinote_techo_03_01_NSCR,
		NARC_pl_wifinote_techo_03_02_NSCR,
		NARC_pl_wifinote_techo_03_03_NSCR,
		NARC_pl_wifinote_techo_03_04_NSCR,
		NARC_pl_wifinote_techo_03_05_NSCR,
	};
	int i;

	for( i=0; i<WFNOTE_FRIENDINFO_PAGE_NUM; i++ ){
		p_scrn->p_scrnbuff[i] = ArcUtil_HDL_ScrnDataGet(  
				p_draw->p_handle, sc_SCRNIDX[i], FALSE, 
				&p_scrn->p_scrn[i], heapID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^�j��
 *
 *	@param	p_scrn		�X�N���[���f�[�^
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_SCRNDATA_Exit( WFNOTE_FRIENDINFO_SCRNDATA* p_scrn )
{
	int i;

	for( i=0; i<WFNOTE_FRIENDINFO_PAGE_NUM; i++ ){
		sys_FreeMemoryEz( p_scrn->p_scrnbuff[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���G���A���[�N	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	cp_scrnarea	�\���G���A�f�[�^
 *	@param	cgx			���b�Z�[�W�ʂ�CGX
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_Init( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, const WFNOTE_SCRNAREA* cp_scrnarea, u32 cgx, u32 heapID )
{
	int i;
	p_wk->scrnarea = *cp_scrnarea;
	p_wk->cgx = cgx;
	
	for( i=0; i<WFNOTE_FRIENDINFO_PAGE_NUM; i++ ){
		p_wk->p_msg[i] = NULL;
		p_wk->msgnum[i] = 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�`��G���A�@�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_Exit( WFNOTE_FRIENDINFO_DRAWAREA* p_wk )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�r�b�g�}�b�v	������
 *
 *	@param	p_wk		���[�N
 *	@param	p_draw		�`��V�X�e��
 *	@param	idx			�C���f�b�N�X
 *	@param	num			��
 *	@param	cp_data		�\���f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpInit( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw, u32 idx, u32 num, const WFNOTE_FRIENDINFO_BMPDATA* cp_data, u32 heapID )
{
	int i;
	u16 cgx;
	
	p_wk->p_msg[idx] = GF_BGL_BmpWinAllocGet( heapID, num );
	p_wk->msgnum[idx] = num;

	// �쐬
	cgx = p_wk->cgx;
	for( i=0; i<num; i++ ){
		GF_BGL_BmpWinInit( &p_wk->p_msg[idx][i] );
		GF_BGL_BmpWinAdd( p_draw->p_bgl, &p_wk->p_msg[idx][i], 
				sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ],
				MATH_ABS(p_wk->scrnarea.scrn_x) + cp_data[i].pos_x,
				MATH_ABS(p_wk->scrnarea.scrn_y) + cp_data[i].pos_y,
				cp_data[i].siz_x, cp_data[i].siz_y,
				WFNOTE_BGPL_MAIN_MSGFONT, cgx );
		GF_BGL_BmpWinDataFill( &p_wk->p_msg[idx][i], 0 );
		cgx += ( cp_data[i].siz_x * cp_data[i].siz_y );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���b�Z�[�W�r�b�g�}�b�v�j��
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpExit( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 idx )
{
	if( p_wk->p_msg[idx] ){
		GF_BGL_BmpWinFree( p_wk->p_msg[idx], p_wk->msgnum[idx] );
		p_wk->p_msg[idx] = NULL;
		p_wk->msgnum[idx] = 0;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�V
 *
 *	@param	p_wk	���[�N 
 *	@param	idx		�C���f�b�N�X
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 idx )
{
	int i;
	for( i=0; i<p_wk->msgnum[idx]; i++ ){
		GF_BGL_BmpWinOnVReq( &p_wk->p_msg[idx][i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\��OFF
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOffVReq( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 idx )
{
	int i;

	for( i=0; i<p_wk->msgnum[idx]; i++ ){
		GF_BGL_BmpWinOffVReq( &p_wk->p_msg[idx][i] );
		GF_BGL_BmpWinDataFill( &p_wk->p_msg[idx][i], 0 );
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�`��A���A�Ƀy�[�W�̏�������
 *
 *	@param	p_wk		���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`�惏�[�N
 *	@param	page		�y�[�W
 *	@param	cp_scrn		�X�N���[���`��f�[�^
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_Page( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 page, const WFNOTE_FRIENDINFO_SCRNDATA* cp_scrn, u32 heapID )
{
	static void (*pWrite[ WFNOTE_FRIENDINFO_PAGE_NUM ])( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID ) = {
		WFNOTE_FRIENDINFO_DRAW_Page00,
		WFNOTE_FRIENDINFO_DRAW_Page06,
		WFNOTE_FRIENDINFO_DRAW_Page01,
		WFNOTE_FRIENDINFO_DRAW_Page02,
		WFNOTE_FRIENDINFO_DRAW_Page03,
		WFNOTE_FRIENDINFO_DRAW_Page04,
		WFNOTE_FRIENDINFO_DRAW_Page05,
	};

	// ���܂ł̕\���N���A
	WFNOTE_FRIENDINFO_DRAWAREA_PageOff( p_wk, p_draw );

	// �w�i�`��
	GF_BGL_ScrWrite( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			cp_scrn->p_scrn[page]->rawData, 
			MATH_ABS(p_wk->scrnarea.scrn_x), MATH_ABS(p_wk->scrnarea.scrn_y), 
			cp_scrn->p_scrn[page]->screenWidth/8,
			cp_scrn->p_scrn[page]->screenHeight/8 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );
	
	// �y�[�W��������
	pWrite[page]( p_wk, p_data, p_draw, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؁[��OFF 
 *
 *	@param	p_wk	���[�N 
 *	@param	p_draw	�`��V�X�e��
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAWAREA_PageOff( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DRAW* p_draw )
{
	int i;
	for( i=0; i<WFNOTE_FRIENDINFO_PAGE_NUM; i++ ){
		WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOffVReq( p_wk, i );
	}
	WFNOTE_FRIENDINFO_DRAW_Clean( p_draw, &p_wk->scrnarea );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�y�[�W�̏�������
 *
 *	@param	p_wk		�`��G���A���[�N
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAW_Page00( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	WIFI_LIST* p_wifilist;
	u32 num;
	BOOL result;
	u32 sex;
	u32 col;
	u32 no;
    TR_CLACT_GRA arcdata;
	void* p_trgrabuff;
	u32 x, y;


	p_wifilist = SaveData_GetWifiListData( p_data->p_save );

	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �O���[�v
	WFNOTE_DRAW_FriendGroupSetWordset( p_draw, p_data->p_save, 
			p_data->idx.fridx[p_data->select_listidx], heapID );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_GR,
			p_data, p_draw, msg_wifi_note_15, 
			WFNOTE_FRIENDINFO_PAGE00_GRT_X, WFNOTE_FRIENDINFO_PAGE00_GRT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �ΐ퐬��
	{
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_GR,
				p_data, p_draw, msg_wifi_note_16, 
				WFNOTE_FRIENDINFO_PAGE00_BTT_X, WFNOTE_FRIENDINFO_PAGE00_BTT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		// ����
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_BT,
				p_data, p_draw, msg_wifi_note_17, 
				WFNOTE_FRIENDINFO_PAGE00_BTW_X, WFNOTE_FRIENDINFO_PAGE00_BTW_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_BATTLE_WIN );
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_BT,
				p_data, p_draw, msg_wifi_note_59, 
				WFNOTE_FRIENDINFO_PAGE00_BTWLN_X, WFNOTE_FRIENDINFO_PAGE00_BTW_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );


		// �܂�
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_BT,
				p_data, p_draw, msg_wifi_note_18, 
				WFNOTE_FRIENDINFO_PAGE00_BTL_X, WFNOTE_FRIENDINFO_PAGE00_BTL_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_BATTLE_LOSE );
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_BT,
				p_data, p_draw, msg_wifi_note_59, 
				WFNOTE_FRIENDINFO_PAGE00_BTWLN_X, WFNOTE_FRIENDINFO_PAGE00_BTL_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
	}

	// �|�P��������
	{
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_TR,
				p_data, p_draw, msg_wifi_note_19, 
				WFNOTE_FRIENDINFO_PAGE00_TRT_X, WFNOTE_FRIENDINFO_PAGE00_TRT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_TRADE_NUM );
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_TR,
				p_data, p_draw, msg_wifi_note_20, 
				WFNOTE_FRIENDINFO_PAGE00_TRN_X, WFNOTE_FRIENDINFO_PAGE00_TRN_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
	}

	// ������
	if( WFNOTE_FRIENDINFO_PofinCaseCheck( p_data, heapID ) ){
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_TR,
				p_data, p_draw, msg_wifi_note_34, 
				WFNOTE_FRIENDINFO_PAGE00_POT_X, WFNOTE_FRIENDINFO_PAGE00_POT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_POFIN_NUM );
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_TR,
				p_data, p_draw, msg_wifi_note_35, 
				WFNOTE_FRIENDINFO_PAGE00_PON_X, WFNOTE_FRIENDINFO_PAGE00_PON_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
	}

	// �Ō�ɗV�񂾓��t
	{
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_DAY,
				p_data, p_draw, msg_wifi_note_21, 
				WFNOTE_FRIENDINFO_PAGE00_DAYT_X, WFNOTE_FRIENDINFO_PAGE00_DAYT_Y,
				WFNOTE_COL_WHITE, p_str, p_tmp );
		result = WFNOTE_DRAW_FriendDaySetWordset( p_draw, p_data->p_save, 
				p_data->idx.fridx[p_data->select_listidx] );
		if( result ){
			WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_DAY,
					p_data, p_draw, msg_wifi_note_22, 
					WFNOTE_FRIENDINFO_PAGE00_DAYN_X, WFNOTE_FRIENDINFO_PAGE00_DAYN_Y,
					WFNOTE_COL_WHITE, p_str, p_tmp );
		}
	}

	sex = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_SEX );

	// �g���[�i�[�̊G�\��
	{
		num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_UNION_GRA );
        no = UnionView_GetTrainerInfo(sex, num, UNIONVIEW_TRTYPE);
        TrCLACTGraDataGet( no, PARA_FRONT, &arcdata);
		p_trgrabuff = sys_AllocMemory( heapID, (WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZX*WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZY)*32 );
		ChangesInto_1D_from_2D( arcdata.arcID, arcdata.ncbrID, heapID, 
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_CUT_X, 
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_CUT_Y,
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZX, 
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZY, p_trgrabuff);
		// �r�b�g�}�b�v�ɏ�������
		GF_BGL_BmpWinPrint(
				&p_wk->p_msg[0][WFNOTE_FRIENDINFO_PAGE00_BA_TRG],
				p_trgrabuff,
				0, 0,
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZX*8, 
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZY*8,
				WFNOTE_FRIENDINFO_PAGE00_TRNGR_X, 
				WFNOTE_FRIENDINFO_PAGE00_TRNGR_Y,
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZX*8, 
				WFNOTE_FRIENDINFO_PAGE00_TRGRA_SIZY*8 );
		sys_FreeMemoryEz( p_trgrabuff );

		// �g���[�i�[�p���b�g�]��
		ArcUtil_PalSet( arcdata.arcID, arcdata.nclrID, PALTYPE_MAIN_BG,
				WFNOTE_BGPL_MAIN_TRGRA*32, 32, heapID );
	}

	// �g���[�i�[�̖��O
	{
		if( sex == PM_MALE ){
			col = WFNOTE_COL_BLUE;
		}else if( sex == PM_FEMALE ){
			col = WFNOTE_COL_RED;
		}else{
			col = WFNOTE_COL_BLACK;
		}
		WFNOTE_DRAW_FriendNameSetWordset( p_draw, p_data->p_save, p_data->idx.fridx[p_data->select_listidx],
				heapID );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 0, WFNOTE_FRIENDINFO_PAGE00_BA_TRG,
				p_data, p_draw, msg_wifi_note_07, 
				WFNOTE_FRIENDINFO_PAGE00_TRNNA_X, WFNOTE_FRIENDINFO_PAGE00_TRNNA_Y,
				col, p_str, p_tmp );
	}
	
	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 0 );

	// VReq�ŃX�N���[���ɃL�����N�^�f�[�^���W�J�����̂�
	// �g���[�i�[�̊G�̃p���b�g�ݒ�
	{
		x = GF_BGL_BmpWinGet_PosX( &p_wk->p_msg[0][ WFNOTE_FRIENDINFO_PAGE00_BA_TRG ] );
		y = GF_BGL_BmpWinGet_PosY( &p_wk->p_msg[0][ WFNOTE_FRIENDINFO_PAGE00_BA_TRG ] );
		GF_BGL_ScrPalChange(
					p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ],
					x, 
					y,
					WFNOTE_FRIENDINFO_PAGE00_TRNGR_CSIZX, 
					WFNOTE_FRIENDINFO_PAGE00_TRNGR_CSIZY, 
					WFNOTE_BGPL_MAIN_TRGRA );
	}

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}
static void WFNOTE_FRIENDINFO_DRAW_Page01( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	FRONTIER_SAVEWORK*  p_fsave;
	u32 num;
	BOOL clear_flag;

	p_fsave		= SaveData_GetFrontier( p_data->p_save );
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �^�C�g��
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 2, WFNOTE_FRIENDINFO_PAGE01_BA,
			p_data, p_draw, msg_wifi_note_36, 
			WFNOTE_FRIENDINFO_PAGE01_BTT_X, WFNOTE_FRIENDINFO_PAGE01_BTT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �O��
	{
		u32 title_stridx;
		clear_flag = FrontierRecord_Get(p_fsave, FRID_TOWER_MULTI_WIFI_CLEAR_BIT, p_data->idx.fridx[p_data->select_listidx]);
		if( clear_flag == FALSE ){
			title_stridx = msg_wifi_note_41;
		}else{
			title_stridx = msg_wifi_note_58;
		}
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 2, WFNOTE_FRIENDINFO_PAGE01_BA,
				p_data, p_draw, title_stridx, 
				WFNOTE_FRIENDINFO_PAGE01_BTL_X, WFNOTE_FRIENDINFO_PAGE01_BTL_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		num = FrontierRecord_Get(p_fsave, FRID_TOWER_MULTI_WIFI_RENSHOU_CNT, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 2, WFNOTE_FRIENDINFO_PAGE01_BA,
				p_data, p_draw, msg_wifi_note_48, 
				WFNOTE_FRIENDINFO_PAGE01_BTLN_X, WFNOTE_FRIENDINFO_PAGE01_BTL_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
	}

	//  �ō�
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 2, WFNOTE_FRIENDINFO_PAGE01_BA,
			p_data, p_draw, msg_wifi_note_42, 
			WFNOTE_FRIENDINFO_PAGE01_BTM_X, WFNOTE_FRIENDINFO_PAGE01_BTM_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	num = FrontierRecord_Get(p_fsave, FRID_TOWER_MULTI_WIFI_RENSHOU, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 2, WFNOTE_FRIENDINFO_PAGE01_BA,
			p_data, p_draw, msg_wifi_note_48, 
			WFNOTE_FRIENDINFO_PAGE01_BTMN_X, WFNOTE_FRIENDINFO_PAGE01_BTM_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 2 );

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}
static void WFNOTE_FRIENDINFO_DRAW_Page02( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	FRONTIER_SAVEWORK* p_fsave;
	u32 num;
	BOOL clear_flag;
	u32 stridx;

	p_fsave = SaveData_GetFrontier( p_data->p_save );
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �^�C�g��
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_BA_TT,
			p_data, p_draw, msg_wifi_note_37, 
			WFNOTE_FRIENDINFO_PAGE02_TT_X, WFNOTE_FRIENDINFO_PAGE02_TT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// LV50
	{
		// �^�C�g��
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50TT,
				p_data, p_draw, msg_wifi_note_43, 
				WFNOTE_FRIENDINFO_PAGE02_LV50TT_X, WFNOTE_FRIENDINFO_PAGE02_LV50TT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50TT,
				p_data, p_draw, msg_wifi_note_45, 
				WFNOTE_FRIENDINFO_PAGE02_LV50TT2_X, WFNOTE_FRIENDINFO_PAGE02_LV50TT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		//�O��
		clear_flag = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_CLEAR_BIT, p_data->idx.fridx[p_data->select_listidx]);
		if( clear_flag == FALSE ){
			stridx = msg_wifi_note_41;
		}else{
			stridx = msg_wifi_note_58;
		}
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50NO,
				p_data, p_draw, stridx, 
				WFNOTE_FRIENDINFO_PAGE02_LV50LT_X, WFNOTE_FRIENDINFO_PAGE02_LV50L_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		// �A��
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_RENSHOU_CNT, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50NO,
				p_data, p_draw, msg_wifi_note_48, 
				WFNOTE_FRIENDINFO_PAGE02_LV50LN0_X, WFNOTE_FRIENDINFO_PAGE02_LV50L_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		// ����
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_TRADE_CNT, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50NO,
				p_data, p_draw, msg_wifi_note_49, 
				WFNOTE_FRIENDINFO_PAGE02_LV50LN1_X, WFNOTE_FRIENDINFO_PAGE02_LV50L_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );


		//�ō�
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50NO,
				p_data, p_draw, msg_wifi_note_42, 
				WFNOTE_FRIENDINFO_PAGE02_LV50MT_X, WFNOTE_FRIENDINFO_PAGE02_LV50M_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		// �A��
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_RENSHOU, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50NO,
				p_data, p_draw, msg_wifi_note_48, 
				WFNOTE_FRIENDINFO_PAGE02_LV50MN0_X, WFNOTE_FRIENDINFO_PAGE02_LV50M_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		// ����
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_TRADE, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV50NO,
				p_data, p_draw, msg_wifi_note_49, 
				WFNOTE_FRIENDINFO_PAGE02_LV50MN1_X, WFNOTE_FRIENDINFO_PAGE02_LV50M_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
	}

	// LV100
	{
		// �^�C�g��
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100TT,
				p_data, p_draw, msg_wifi_note_44, 
				WFNOTE_FRIENDINFO_PAGE02_LV100TT_X, WFNOTE_FRIENDINFO_PAGE02_LV100TT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100TT,
				p_data, p_draw, msg_wifi_note_45, 
				WFNOTE_FRIENDINFO_PAGE02_LV100TT2_X, WFNOTE_FRIENDINFO_PAGE02_LV100TT_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		//�O��
		clear_flag = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT, p_data->idx.fridx[p_data->select_listidx]);
		if( clear_flag == FALSE ){
			stridx = msg_wifi_note_41;
		}else{
			stridx = msg_wifi_note_58;
		}
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100NO,
				p_data, p_draw, stridx, 
				WFNOTE_FRIENDINFO_PAGE02_LV100LT_X, WFNOTE_FRIENDINFO_PAGE02_LV100L_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		// �A��
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_RENSHOU100_CNT, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100NO,
				p_data, p_draw, msg_wifi_note_48, 
				WFNOTE_FRIENDINFO_PAGE02_LV100LN0_X, WFNOTE_FRIENDINFO_PAGE02_LV100L_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		// ����
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_TRADE100_CNT, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100NO,
				p_data, p_draw, msg_wifi_note_49, 
				WFNOTE_FRIENDINFO_PAGE02_LV100LN1_X, WFNOTE_FRIENDINFO_PAGE02_LV100L_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );


		//�ō�
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100NO,
				p_data, p_draw, msg_wifi_note_42, 
				WFNOTE_FRIENDINFO_PAGE02_LV100MT_X, WFNOTE_FRIENDINFO_PAGE02_LV100M_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );

		// �A��
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_RENSHOU100, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100NO,
				p_data, p_draw, msg_wifi_note_48, 
				WFNOTE_FRIENDINFO_PAGE02_LV100MN0_X, WFNOTE_FRIENDINFO_PAGE02_LV100M_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
		// ����
		num = FrontierRecord_Get(p_fsave, FRID_FACTORY_MULTI_WIFI_TRADE100, p_data->idx.fridx[p_data->select_listidx]);
		WFNOTE_DRAW_NumberSetWordset( p_draw, num );
		WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 3, WFNOTE_FRIENDINFO_PAGE02_LV100NO,
				p_data, p_draw, msg_wifi_note_49, 
				WFNOTE_FRIENDINFO_PAGE02_LV100MN1_X, WFNOTE_FRIENDINFO_PAGE02_LV100M_Y,
				WFNOTE_COL_BLACK, p_str, p_tmp );
	}
	

	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 3 );

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}
static void WFNOTE_FRIENDINFO_DRAW_Page03( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	FRONTIER_SAVEWORK* p_fsave;
	u32 num;
	BOOL clear_flag;
	u32 stridx;

	p_fsave = SaveData_GetFrontier( p_data->p_save );
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );


	// �^�C�g��
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_38, 
			WFNOTE_FRIENDINFO_PAGE03_TT_X, WFNOTE_FRIENDINFO_PAGE03_TT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_46, 
			WFNOTE_FRIENDINFO_PAGE03_CPT_X, WFNOTE_FRIENDINFO_PAGE03_CPT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �O��
	clear_flag = FrontierRecord_Get(p_fsave, FRID_CASTLE_MULTI_WIFI_CLEAR_BIT, p_data->idx.fridx[p_data->select_listidx]);
	if( clear_flag == FALSE ){	
		stridx = msg_wifi_note_41;
	}else{
		stridx = msg_wifi_note_58;
	}
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, stridx, 
			WFNOTE_FRIENDINFO_PAGE03_CPLT_X, WFNOTE_FRIENDINFO_PAGE03_CPL_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// ���肠
	num = FrontierRecord_Get(p_fsave, FRID_CASTLE_MULTI_WIFI_RENSHOU_CNT, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_51, 
			WFNOTE_FRIENDINFO_PAGE03_CPLN0_X, WFNOTE_FRIENDINFO_PAGE03_CPL_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// CP
	num = FrontierRecord_Get(p_fsave, FRID_CASTLE_MULTI_WIFI_CP, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_50, 
			WFNOTE_FRIENDINFO_PAGE03_CPLN1_X, WFNOTE_FRIENDINFO_PAGE03_CPL_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );


	// �ō�
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_42, 
			WFNOTE_FRIENDINFO_PAGE03_CPMT_X, WFNOTE_FRIENDINFO_PAGE03_CPM_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// ���肠
	num = FrontierRecord_Get(p_fsave, FRID_CASTLE_MULTI_WIFI_RENSHOU, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_51, 
			WFNOTE_FRIENDINFO_PAGE03_CPMN0_X, WFNOTE_FRIENDINFO_PAGE03_CPM_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// CP
	num = FrontierRecord_Get(p_fsave, FRID_CASTLE_MULTI_WIFI_REMAINDER_CP, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 4, WFNOTE_FRIENDINFO_PAGE03_BA,
			p_data, p_draw, msg_wifi_note_50, 
			WFNOTE_FRIENDINFO_PAGE03_CPMN1_X, WFNOTE_FRIENDINFO_PAGE03_CPM_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );


	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 4 );


	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}
static void WFNOTE_FRIENDINFO_DRAW_Page04( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	STRBUF* p_monsstr;
	FRONTIER_SAVEWORK* p_fsave;
	u32 num;
	BOOL clear_flag;
	u32 stridx;

	p_fsave = SaveData_GetFrontier( p_data->p_save );
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );


	// �^�C�g��
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 5, WFNOTE_FRIENDINFO_PAGE04_BA,
			p_data, p_draw, msg_wifi_note_39, 
			WFNOTE_FRIENDINFO_PAGE04_TT_X, WFNOTE_FRIENDINFO_PAGE04_TT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	num = FrontierRecord_Get(p_fsave, FRID_STAGE_MULTI_WIFI_MONSNO, p_data->idx.fridx[p_data->select_listidx]);
	p_monsstr = MSGDAT_UTIL_GetMonsName( num, heapID );
	GF_STR_PrintColor( &p_wk->p_msg[5][WFNOTE_FRIENDINFO_PAGE04_BA],
			FONT_SYSTEM, p_monsstr,
			WFNOTE_FRIENDINFO_PAGE04_TT_X, WFNOTE_FRIENDINFO_PAGE04_TT2_Y,
			MSG_NO_PUT, WFNOTE_COL_BLACK, NULL);
	STRBUF_Delete( p_monsstr );

	// �O��
	clear_flag = FrontierRecord_Get(p_fsave, FRID_STAGE_MULTI_WIFI_CLEAR_BIT, p_data->idx.fridx[p_data->select_listidx]);
	if( clear_flag == FALSE ){	
		stridx = msg_wifi_note_41;
	}else{
		stridx = msg_wifi_note_58;
	}
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 5, WFNOTE_FRIENDINFO_PAGE04_BA,
			p_data, p_draw, stridx, 
			WFNOTE_FRIENDINFO_PAGE04_LT_X, WFNOTE_FRIENDINFO_PAGE04_L_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = FrontierRecord_Get(p_fsave, FRID_STAGE_MULTI_WIFI_RENSHOU_CNT, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 5, WFNOTE_FRIENDINFO_PAGE04_BA,
			p_data, p_draw, msg_wifi_note_48, 
			WFNOTE_FRIENDINFO_PAGE04_LN_X, WFNOTE_FRIENDINFO_PAGE04_L_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �ō�
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 5, WFNOTE_FRIENDINFO_PAGE04_BA,
			p_data, p_draw, msg_wifi_note_42, 
			WFNOTE_FRIENDINFO_PAGE04_MT_X, WFNOTE_FRIENDINFO_PAGE04_M_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = FrontierRecord_Get(p_fsave, FRID_STAGE_MULTI_WIFI_RENSHOU, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 5, WFNOTE_FRIENDINFO_PAGE04_BA,
			p_data, p_draw, msg_wifi_note_48, 
			WFNOTE_FRIENDINFO_PAGE04_MN_X, WFNOTE_FRIENDINFO_PAGE04_M_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 5 );

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}
static void WFNOTE_FRIENDINFO_DRAW_Page05( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	FRONTIER_SAVEWORK* p_fsave;
	u32 num;
	BOOL clear_flag;
	u32 stridx;

	p_fsave = SaveData_GetFrontier( p_data->p_save );
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �^�C�g��
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 6, WFNOTE_FRIENDINFO_PAGE05_BA,
			p_data, p_draw, msg_wifi_note_40, 
			WFNOTE_FRIENDINFO_PAGE05_TT_X, WFNOTE_FRIENDINFO_PAGE05_TT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �O��
	clear_flag = FrontierRecord_Get(p_fsave, FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT, p_data->idx.fridx[p_data->select_listidx]);
	if( clear_flag == FALSE ){	
		stridx = msg_wifi_note_41;
	}else{
		stridx = msg_wifi_note_58;
	}
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 6, WFNOTE_FRIENDINFO_PAGE05_BA,
			p_data, p_draw, stridx, 
			WFNOTE_FRIENDINFO_PAGE05_LT_X, WFNOTE_FRIENDINFO_PAGE05_L_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = FrontierRecord_Get(p_fsave, FRID_ROULETTE_MULTI_WIFI_RENSHOU_CNT, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 6, WFNOTE_FRIENDINFO_PAGE05_BA,
			p_data, p_draw, msg_wifi_note_57, 
			WFNOTE_FRIENDINFO_PAGE05_LN_X, WFNOTE_FRIENDINFO_PAGE05_L_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �ō�
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 6, WFNOTE_FRIENDINFO_PAGE05_BA,
			p_data, p_draw, msg_wifi_note_42, 
			WFNOTE_FRIENDINFO_PAGE05_MT_X, WFNOTE_FRIENDINFO_PAGE05_M_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = FrontierRecord_Get(p_fsave, FRID_ROULETTE_MULTI_WIFI_RENSHOU, p_data->idx.fridx[p_data->select_listidx]);
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 6, WFNOTE_FRIENDINFO_PAGE05_BA,
			p_data, p_draw, msg_wifi_note_57, 
			WFNOTE_FRIENDINFO_PAGE05_MN_X, WFNOTE_FRIENDINFO_PAGE05_M_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );



	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 6 );

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}
// �~�j�Q�[��
static void WFNOTE_FRIENDINFO_DRAW_Page06( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 heapID )
{
	STRBUF* p_str;
	STRBUF* p_tmp;
	FRONTIER_SAVEWORK* p_fsave;
	WIFI_LIST* p_wifilist;
	u32 num;

	p_fsave		= SaveData_GetFrontier( p_data->p_save );
	p_wifilist	= SaveData_GetWifiListData( p_data->p_save );
	p_str = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );
	p_tmp = STRBUF_Create( WFNOTE_STRBUF_SIZE, heapID );

	// �^�C�g��
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_52, 
			WFNOTE_FRIENDINFO_PAGE06_TT_X, WFNOTE_FRIENDINFO_PAGE06_TT_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// ���܂���
	WFNOTE_DRAW_WflbyGameSetWordSet( p_draw, WFLBY_GAME_BALLSLOW );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_53, 
			WFNOTE_FRIENDINFO_PAGE06_BC_X, WFNOTE_FRIENDINFO_PAGE06_BC_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_BALLSLOW_NUM );
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_56, 
			WFNOTE_FRIENDINFO_PAGE06_BC_N_X, WFNOTE_FRIENDINFO_PAGE06_BC_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// ���܂̂�
	WFNOTE_DRAW_WflbyGameSetWordSet( p_draw, WFLBY_GAME_BALANCEBALL );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_53, 
			WFNOTE_FRIENDINFO_PAGE06_BB_X, WFNOTE_FRIENDINFO_PAGE06_BB_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_BALANCEBALL_NUM );
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_56, 
			WFNOTE_FRIENDINFO_PAGE06_BB_N_X, WFNOTE_FRIENDINFO_PAGE06_BB_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );

	// �ӂ�����
	WFNOTE_DRAW_WflbyGameSetWordSet( p_draw, WFLBY_GAME_BALLOON );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_53, 
			WFNOTE_FRIENDINFO_PAGE06_BL_X, WFNOTE_FRIENDINFO_PAGE06_BL_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );
	// �l
	num = WifiList_GetFriendInfo( p_wifilist, p_data->idx.fridx[p_data->select_listidx], WIFILIST_FRIEND_BALLOON_NUM );
	WFNOTE_DRAW_NumberSetWordset( p_draw, num );
	WFNOTE_FRIENDINFO_DRAW_Bmp( p_wk, 1, WFNOTE_FRIENDINFO_PAGE06_BA,
			p_data, p_draw, msg_wifi_note_56, 
			WFNOTE_FRIENDINFO_PAGE06_BL_N_X, WFNOTE_FRIENDINFO_PAGE06_BL_Y,
			WFNOTE_COL_BLACK, p_str, p_tmp );


	WFNOTE_FRIENDINFO_DRAWAREA_MSGBmpOnVReq( p_wk, 1 );

	STRBUF_Delete( p_str );
	STRBUF_Delete( p_tmp );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\���ʂ̃N���A
 *
 *	@param	p_draw	�`��V�X�e��
 *	@param	cp_scrnarea	�X�N���[���G���A
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAW_Clean( WFNOTE_DRAW* p_draw, const WFNOTE_SCRNAREA* cp_scrnarea )
{
	// �w�i�N���A
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCROLL ],
			0, 
			MATH_ABS(cp_scrnarea->scrn_x), MATH_ABS(cp_scrnarea->scrn_y), 
			32,
			24, 0 );

	// BG�ʓ]��
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCROLL] );

	// �X�N���[���ʂ��N���A
	GF_BGL_ScrFill( p_draw->p_bgl, sc_WFNOTE_BGFRM[ WFNOTE_BG_MAIN_SCRMSG ],
			0, 
			MATH_ABS(cp_scrnarea->scrn_x), MATH_ABS(cp_scrnarea->scrn_y), 
			32,
			24, 0 );
	GF_BGL_LoadScreenV_Req( p_draw->p_bgl, sc_WFNOTE_BGFRM[WFNOTE_BG_MAIN_SCRMSG] );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r�b�g�}�b�v�ւ̕����̕`��
 *	
 *	@param	p_wk		�`��G���A���[�N
 *	@param	page		�y�[�W
 *	@param	bmp			�r�b�g�}�b�v�C���f�b�N�X
 *	@param	p_data		�f�[�^
 *	@param	p_draw		�`��V�X�e��
 *	@param	msg_idx		���b�Z�[�W�C���f�b�N�X
 *	@param	x			��
 *	@param	y			��
 *	@param	col			�J���[
 *	@param	p_str		�g�p����STRBUF
 *	@param	p_tmp		�g�p����STRBUF
 */
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_DRAW_Bmp( WFNOTE_FRIENDINFO_DRAWAREA* p_wk, u32 page, u32 bmp, WFNOTE_DATA* p_data, WFNOTE_DRAW* p_draw, u32 msg_idx, u32 x, u32 y, u32 col, STRBUF* p_str, STRBUF* p_tmp )
{
	MSGMAN_GetString( p_draw->p_msgman, msg_idx, p_tmp );
	WORDSET_ExpandStr( p_draw->p_wordset, p_str, p_tmp );
    GF_STR_PrintColor( &p_wk->p_msg[page][bmp], FONT_SYSTEM, 
			p_str, x, y, MSG_NO_PUT, col, NULL);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�y�[�W�ύX
 *
 *	@param	p_wk		���[�N
 *	@param	page		�y�[�W
 */	
//-----------------------------------------------------------------------------
static void WFNOTE_FRIENDINFO_PageChange( WFNOTE_FRIENDINFO* p_wk, s32 add )
{
	int page_num;

	// �t�����e�B�A��`�悷�邩�`�F�b�N
	if( p_wk->frontier_draw == TRUE ){
		page_num = WFNOTE_FRIENDINFO_PAGE_NUM;
	}else{
		page_num = WFNOTE_FRIENDINFO_FRONTIEROFFPAGE_NUM;
	}
	
	if( add > 0 ){
		p_wk->lastpage = p_wk->page;
		p_wk->page = (p_wk->page + add) % page_num;
	}else if( add < 0 ){
		p_wk->lastpage = p_wk->page;
		p_wk->page = p_wk->page + add;
		if( p_wk->page < 0 ){
			p_wk->page += page_num;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�|�t�B���P�[�X�����Ă��邩�`�F�b�N
 *	
 *	@param	p_data	���[�N
 */
//-----------------------------------------------------------------------------
static BOOL WFNOTE_FRIENDINFO_PofinCaseCheck( WFNOTE_DATA* p_data, u32 heapID )
{
	// �|���g�P�[�X�����邩�`�F�b�N
	if( !MyItem_GetItemNum( SaveData_GetMyItem(p_data->p_save),ITEM_POFINKEESU, heapID ) ){
		return FALSE;
	}
	return TRUE;
}
