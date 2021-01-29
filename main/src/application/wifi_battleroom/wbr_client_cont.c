//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_client_cont.c
 *	@brief		�N���C�A���g�I�u�W�F�N�g�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.04.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "application/wifi_2dmap/wf2dmap_common.h"
#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_objdraw.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"

#include "field/fieldobj_code.h"
#include "system/clact_util.h"
#include "system/render_oam.h"
#include "system/wipe.h"

#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_wifi_br.h"
#include "system/wordset.h"
#include "system/fontproc.h"
#include "system/window.h"

#define __WBR_CLIENT_CONT_H_GLOBAL
#include "wbr_client_cont.h"

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
#define WBR_CHARMAN_CONTNUM	( 32 )	// �L�����N�^�}�l�[�W���Ǘ���
#define WBR_PLTTMAN_CONTNUM	( 32 )	// �L�����N�^�}�l�[�W���Ǘ���
#define WBR_CLACT_CONTNUM	( 64 )	// �Z���A�N�^�[�Ǘ���
#define WBR_CHARVRAM_MSIZE	( 128*1024 )	// OAM�L�����N�^�T�C�Y
#define WBR_CHARVRAM_SSIZE	( 16*1024 )	// OAM�L�����N�^�T�C�Y
#define WBR_BGFRAME_BACK	( GF_BGL_FRAME0_M )	// �w�i��
#define WBR_BGFRAME_MSG		( GF_BGL_FRAME1_M )	// ���b�Z�[�W��
#define WBR_BGMSK_BACK		( GX_PLANEMASK_BG0 )	// �w�i��
#define WBR_BGMSK_MSG		( GX_PLANEMASK_BG1 )	// ���b�Z�[�W��

// message�E�B���h�E
#define WBR_MESSAGE_FONTBMP_X	( 4 )			// �����ʃr�b�g�}�b�vsize
#define WBR_MESSAGE_FONTBMP_Y	( 19 )			// �����ʃr�b�g�}�b�vsize
#define WBR_MESSAGE_FONTBMP_CX	( 23 )			// �����ʃr�b�g�}�b�vsize
#define WBR_MESSAGE_FONTBMP_CY	( 4 )			// �����ʃr�b�g�}�b�vsize
#define WBR_MESSAGE_FONT_COLOR		( 6 )		// �g�p�J���[�p���b�g
#define WBR_MESSAGE_FONT_CG_OFS		( 1 )		// �g�pCGX
#define WBR_MESSAGE_BMP_CG_OFS		( TALK_WIN_CGX_SIZ + 1 )
#define WBR_MESSAGE_FONT_STR_COLOR	( 5 )		// �t�H���g�p�J���[�p���b�g
#define WBR_MESSAGE_FONTBMP_PRICOLOR	( GF_PRINTCOLOR_MAKE(1,2,15) )

#define WBR_MESSAGE_STRBUFF_NUM	( 64 )


// �I�u�W�F�N�g �f�[�^
#define WBR_OBJ_BG_PRI	(1)
#define WBR_OBJ_CMDSEND_TIMING	(4)	// ����I�����t���[���O����R�}���h��]�����邩


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�������I�u�W�F�N�g�f�[�^
//=====================================
typedef struct {
	BOOL recv_wait;
	WF2DMAP_OBJWK*		p_obj;
	WF2DMAP_OBJDRAWWK*	p_draw;
	WBR_TALK_DATA talk_data;
	BOOL talk_recv;			// �b�f�[�^�X�V
	BOOL talk_recv_wait;	// �b�f�[�^�X�V�҂�
} WBR_CLIENTCONTOBJ;

//-------------------------------------
///	message�f�[�^
//=====================================
typedef struct {
	MSGDATA_MANAGER *p_man;	///<�}�l�[�W��
	WORDSET* p_wordset;		///<���[�N
	STRBUF* p_tmpstr;
	u32 okuri;

	// ���b�Z�[�W�E�B���h�E
	GF_BGL_BMPWIN* p_message;
} WBR_CLIENTCONTOBJ_MSG;


//-------------------------------------
///	�O���t�B�b�N�֌W���[�N
//=====================================
typedef struct {
	GF_BGL_INI*	p_bgl;
	CLACT_SET_PTR	p_clset;
	CLACT_U_EASYRENDER_DATA	renddata;

	WF2DMAP_OBJDRAWSYS* p_objdraw;

	// message�f�[�^
	WBR_CLIENTCONTOBJ_MSG	msg;

} WBR_CLIENTCONT_GRAPHIC;


//-------------------------------------
///	�������V�X�e��
//=====================================
typedef struct _WBR_CLIENTCONT{
	WBR_CLIENT_COMMON* p_prt;		// �N���C�A���g���[�N�Ƃ̋��ʃf�[�^
	WBR_CLIENTCONTOBJ	obj[WBR_COMM_NUM];
	WBR_CLIENTCONTOBJ* p_player;	// ��l��
	WBR_CLIENTCONT_GRAPHIC graphic;	// �O���t�B�b�N�f�[�^
	WF2DMAP_OBJSYS*		p_objsys;	// �I�u�W�F�N�g�Ǘ��V�X�e��
	WF2DMAP_ACTCMDQ*	p_actQ;		// �A�N�V�����R�}���h�L���[
	BOOL end_flag;					// �e����I���̒ʒB
} WBR_CLIENTCONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Vblank( void* p_wk );
static BOOL WBR_ClientCont_ObjDataRecvCheck( const WBR_CLIENTCONT* p_sys );
static WF2DMAP_WAY WBR_ClientCont_KayWayGet( int cont );


// �I�u�W�F�N�g�f�[�^�\�z
static void WBR_ClientCont_ObjInit( WBR_CLIENTCONT* p_sys, WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT_GRAPHIC* p_graphic, WBR_CLIENT_COMMON* p_client, int id, BOOL hero );
static void WBR_ClientCont_ObjDel( WBR_CLIENTCONTOBJ* p_obj );
static void WBR_ClientCont_PlayerMove( WBR_CLIENTCONT* p_sys, WBR_CLIENTCONTOBJ* p_player );
static BOOL WBR_ClientCont_PlayerReqCmdSendOkCheck( const WBR_CLIENTCONTOBJ* cp_obj );
static BOOL WBR_ClientCont_PlayerTalkCheck( const WBR_CLIENTCONTOBJ* cp_obj );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_PlayerTalkMove( WBR_CLIENTCONT* p_sys, WBR_CLIENTCONTOBJ* p_player );


// �O���t�B�b�N�֌W
static void WBR_ClientCont_GraphicInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID );
static void WBR_ClientCont_GraphicExit( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_GraphicVblank( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_BankInit( void );
static void WBR_ClientCont_BankExit( void );
static void WBR_ClientCont_BgInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID );
static void WBR_ClientCont_BgExit( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_BgLoad( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID );
static void WBR_ClientCont_BgRelease( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_ClactInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID );
static void WBR_ClientCont_ClactExit( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_ClactLoad( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID );
static void WBR_ClientCont_ClactRelease( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_MessageInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID );
static void WBR_ClientCont_MessageExit( WBR_CLIENTCONT_GRAPHIC* p_gra );

// message����
static void WBR_ClientCont_MessageOn( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_MessageOff( WBR_CLIENTCONT_GRAPHIC* p_gra );
static void WBR_ClientCont_MessageTrNameSet( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 buffid, const MYSTATUS* p_status );
static void WBR_ClientCont_MessagePrint( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 strid );
static BOOL WBR_ClientCont_MessageEndCheck( const WBR_CLIENTCONT_GRAPHIC* cp_gra );


// �b�f�[�^������
static void WBR_ClientCont_Talk_StartInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_NextInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_EndInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_ExitInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_TrCardReqInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_TrCardStartInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_TrCardEndWaitInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static void WBR_ClientCont_Talk_TrCardEndInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );

// �b�f�[�^���C��
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_StartMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_NextMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_EndMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_ExitMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardReqMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardStartMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardEndWaitMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardEndMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys );

//----------------------------------------------------------------------------
/**
 *	@brief	������
 *
 *	@param	p_proc		�v���Z�X���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval		PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval		PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WBR_ClientCont_Init( PROC* p_proc, int* p_seq )
{
	WBR_CLIENTCONT* p_sys = PROC_GetWork( p_proc );
	WBR_CLIENT_COMMON* p_prt = PROC_GetParentWork( p_proc );
	int i;
	BOOL hero;

	switch( *p_seq ){
	case 0:
		
		// �V�X�e�����[�N�쐬
		p_sys = PROC_AllocWork( p_proc, sizeof(WBR_CLIENTCONT), p_prt->heapID );	
		memset( p_sys, 0, sizeof(WBR_CLIENTCONT) );

		// �ォ����������f�[�^��ۑ�
		p_sys->p_prt = p_prt;

		// ���荞�ݐݒ�
		sys_VBlankFuncChange( WBR_ClientCont_Vblank, p_sys );	// VBlank�Z�b�g
		sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

		// �I�u�W�F�N�g�Ǘ����[�N����
		p_sys->p_objsys = WF2DMAP_OBJSysInit( WBR_COMM_NUM, p_prt->heapID );

		// �A�N�V�����R�}���h�L���[����
		p_sys->p_actQ = WF2DMAP_ACTCMDQSysInit( WBR_CMDQ_BUFFNUM, p_prt->heapID );

		// �O���t�B�b�N������
		WBR_ClientCont_GraphicInit( &p_sys->graphic, p_prt->heapID );

		// �I�u�W�F�N�g�f�[�^�\�z
		for( i=0; i<WBR_COMM_NUM; i++ ){
			if( i==p_prt->netid ){
				hero = TRUE;
			}else{
				hero = FALSE;
			}
			WBR_ClientCont_ObjInit( p_sys, &p_sys->obj[i], &p_sys->graphic, p_prt, i, hero );
			if( hero == TRUE ){	// ��l�����[�N������Ă���
				p_sys->p_player = &p_sys->obj[i];
			}
		}

		// �ォ�������Ă��郏�[�N�Ɏ����̃|�C���^��ݒ�
		// ��M�����f�[�^�Ȃǂ����炤�Ƃ��Ɏg�p����
		p_prt->p_procw = p_sys;

		// ���̏�Ԃ���M�v��
		CommSendData( CNM_WBR_KO_OYADATA_GET, NULL, 0 );

		(*p_seq) ++;
		break;
	
	case 1:
		// �ŐV��Ԃ��擾����܂ł܂�
		if( WBR_ClientCont_ObjDataRecvCheck( p_sys ) == TRUE ){
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
					WIPE_FADE_OUTCOLOR, WIPE_DEF_DIV, WIPE_DEF_SYNC, p_prt->heapID );
			
			(*p_seq) ++;
		}
		break;

	case 2:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return PROC_RES_FINISH;
		}
		break;
	}

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C��
 *
 *	@param	p_proc		�v���Z�X���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval		PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval		PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WBR_ClientCont_Main( PROC* p_proc, int* p_seq )
{
	WBR_CLIENTCONT* p_sys = PROC_GetWork( p_proc );
	WBR_CLIENT_COMMON* p_prt = PROC_GetParentWork( p_proc );
	int i;
	WBR_CLIENTCONT_RETTYPE ret_type;
	BOOL end_flag = FALSE;
	BOOL talk_flag;

	// �I�����N�G�X�g����������
	if( p_sys->end_flag == TRUE ){
		p_prt->next_seq = WBR_CLIENTCONT_RET_END;
		end_flag = TRUE;
	}

	// �I�u�W�F�N�g����
	WF2DMAP_OBJSysMain( p_sys->p_objsys );

	//  �A�N�V�����R�}���h�̐ݒ�
	{
		WF2DMAP_ACTCMD act;
		while( WF2DMAP_ACTCMDQSysCmdPop( p_sys->p_actQ, &act ) == TRUE ){
			WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );
		}
	}
	
	talk_flag = WBR_ClientCont_PlayerTalkCheck( p_sys->p_player );
	if( talk_flag == FALSE ){
		// �I�u�W�F�N�g����
		WBR_ClientCont_PlayerMove( p_sys, p_sys->p_player );
	}else{
		// ��b����
		ret_type = WBR_ClientCont_PlayerTalkMove( p_sys, p_sys->p_player );
		if( ret_type != WBR_CLIENTCONT_RET_NONE ){
			p_prt->next_seq = ret_type;
			end_flag = TRUE;
		}
	}

	// �I�u�W�F�N�g�`�揈��
	WF2DMAP_OBJDrawSysUpdata( p_sys->graphic.p_objdraw );
 
	// �A�N�^�[�̕\��
	CLACT_Draw( p_sys->graphic.p_clset );
	
	if( end_flag == TRUE ){
		return PROC_RES_FINISH;
	}
	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I��
 *
 *	@param	p_proc		�v���Z�X���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval		PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval		PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WBR_ClientCont_End( PROC* p_proc, int* p_seq )
{
	WBR_CLIENTCONT* p_sys = PROC_GetWork( p_proc );
	WBR_CLIENT_COMMON* p_prt = PROC_GetParentWork( p_proc );
	int i; 

	switch( *p_seq ){
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
				WIPE_FADE_OUTCOLOR, WIPE_DEF_DIV, WIPE_DEF_SYNC, p_prt->heapID );
		
		(*p_seq) ++;
		break;

	case 1:
		if( WIPE_SYS_EndCheck() == TRUE ){
			(*p_seq)++;
		}
		break;

	case 2:

		// ���荞�ݏI��
		sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
		sys_HBlankIntrStop();	//HBlank���荞�ݒ�~
		
		// �I�u�W�F�N�g�j��
		for( i=0; i<WBR_COMM_NUM; i++ ){
			WBR_ClientCont_ObjDel( &p_sys->obj[i] );
		}

		WF2DMAP_OBJSysExit( p_sys->p_objsys );
		WF2DMAP_ACTCMDQSysExit( p_sys->p_actQ );
		
		// �O���t�B�b�N�j��
		WBR_ClientCont_GraphicExit( &p_sys->graphic );	

		// ��ɓ`���Ă������[�N�ɂ�NULL�����Ă���
		p_prt->p_procw = NULL;

		// �v���Z�X���[�N�j��
		PROC_FreeWork(p_proc);
		return PROC_RES_FINISH;
	}

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���₩���M�����Q�[���f�[�^�ݒ�
 *
 *	@param	p_procw		���[�N
 *	@param	cp_oyadata	�e�f�[�^
 */
//-----------------------------------------------------------------------------
void WBR_ClientCont_SetOyaGameData( void* p_procw, const WBR_OYA_COMM_GAME* cp_oyadata )
{
	WBR_CLIENTCONT* p_wk = p_procw;
	u32 playid;

	if( p_wk ){
		playid = cp_oyadata->obj_data.act.playid;

		//  ��M����
		p_wk->obj[ playid ].recv_wait = TRUE;

		// �b�f�[�^�ݒ�
		p_wk->obj[ playid ].talk_data = cp_oyadata->obj_data.talk_data;
		p_wk->obj[ playid ].talk_recv = TRUE;

		// �A�N�V�����R�}���h���L���[�ɐݒ�
		WF2DMAP_ACTCMDQSysCmdPush( p_wk->p_actQ, &cp_oyadata->obj_data.act );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b�f�[�^���擾
 *
 *	@param	p_procw			���[�N
 *	@param	cp_talkdata		��b�f�[�^
 */
//-----------------------------------------------------------------------------
void WBR_ClientCont_SetTalkData( void* p_procw, const WBR_KO_COMM_TALK* cp_talkdata )
{
	WBR_CLIENTCONT* p_wk = p_procw;
	u32 netid;

	if( p_wk ){

		OS_TPrintf( "netid %d talk_seq %d talk_netid %d\n", cp_talkdata->talk_data.netid, cp_talkdata->talk_data.talk_seq, cp_talkdata->talk_data.talk_playid );

		// ���������`�F�b�N
		if( (p_wk->p_prt->netid != cp_talkdata->talk_data.talk_playid) &&
			(p_wk->p_prt->netid != cp_talkdata->talk_data.netid) ){
			return ;
		}

		// �V�[�P���X�̃��[�h�ɂ���M�����Ƃ��̓���𕪂���
		// �g���[�i�[�J�[�h�������[�h�I��
		if( cp_talkdata->talk_data.talk_seq == WBR_TALK_SEQ_TRCARD_END ){
			// �e�@���炫���̂ł���΁A���̃V�[�P���X��
			if( cp_talkdata->talk_data.talk_mode == WBR_TALK_OYA ){
				p_wk->p_player->talk_data.talk_seq = cp_talkdata->talk_data.talk_seq;
				p_wk->p_player->talk_recv = TRUE;
			}else{

				// �q�@�̏ꍇ�́A���̐l�̃V�[�P���X�f�[�^�𓮂���
				p_wk->obj[ cp_talkdata->talk_data.netid ].talk_data.talk_seq = cp_talkdata->talk_data.talk_seq;
			}
		}
		// �g���[�i�[�J�[�h�I���҂�
		else if( cp_talkdata->talk_data.talk_seq == WBR_TALK_SEQ_TRCARD_ENDWAIT ){
			// �������Ȃ�
			p_wk->p_player->talk_data.talk_seq = cp_talkdata->talk_data.talk_seq;
//			p_wk->p_player->talk_recv = TRUE;
		}
		// ����ȊO
		else {
			// �V�[�P���X���ς�邩�`�F�b�N
			if( p_wk->p_player->talk_data.talk_seq == cp_talkdata->talk_data.talk_seq ){
				GF_ASSERT(0);
				return ;
			}

			p_wk->p_player->talk_data.talk_seq = cp_talkdata->talk_data.talk_seq;
			p_wk->p_player->talk_recv = TRUE;
		}

	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�����N�G�X�g
 *	
 *	@param	p_procw		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WBR_ClientCont_SetEndFlag( void* p_procw )
{
	WBR_CLIENTCONT* p_wk = p_procw;

	if( p_wk ){
		p_wk->end_flag = TRUE;
	}
}



//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_wk 
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Vblank( void* p_wk )
{
	WBR_CLIENTCONT* p_wbr = p_wk;

	// �O���t�B�b�NV�u�����N�֐�
	WBR_ClientCont_GraphicVblank( &p_wbr->graphic );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e����̃f�[�^��M�`�F�b�N
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static BOOL WBR_ClientCont_ObjDataRecvCheck( const WBR_CLIENTCONT* p_sys )
{
	int i;
	for( i=0; i<WBR_COMM_NUM; i++ ){
		if( p_sys->obj[i].recv_wait == FALSE ){
			return FALSE;
		}
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�[��񂩂牟���ꂽ������Ԃ�
 *
 *	@param	cont	�L�[���
 *
 *	@return	����
 */
//-----------------------------------------------------------------------------
static WF2DMAP_WAY WBR_ClientCont_KayWayGet( int cont )
{
	WF2DMAP_WAY move_way = WF2DMAP_WAY_NUM;
	
	if( sys.cont & PAD_KEY_UP ){
		move_way = WF2DMAP_WAY_UP;
	}else if( sys.cont & PAD_KEY_DOWN ){
		move_way = WF2DMAP_WAY_DOWN;
	}else if( sys.cont & PAD_KEY_LEFT ){
		move_way = WF2DMAP_WAY_LEFT;
	}else if( sys.cont & PAD_KEY_RIGHT ){
		move_way = WF2DMAP_WAY_RIGHT;
	}
	return move_way;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N������
 *
 *	@param	p_gra		�O���t�B�b�N���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_GraphicInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID )
{
	// �o���N�ݒ�
	WBR_ClientCont_BankInit();

	// BGL������
	WBR_ClientCont_BgInit( p_gra, heapID );

	// �Z���A�N�^�[������
	WBR_ClientCont_ClactInit( p_gra, heapID );

	// BG�ǂݍ���
	WBR_ClientCont_BgLoad( p_gra, heapID );

	// �Z���A�N�^�[�ǂݍ���
	WBR_ClientCont_ClactLoad( p_gra, heapID );

	// message�쐬
	WBR_ClientCont_MessageInit( p_gra, heapID );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�O���t�B�b�N�j��
 *
 *	@param	p_gra		�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_GraphicExit( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
	// message�j��
	WBR_ClientCont_MessageExit( p_gra );
	
	// BG�ǂݍ���
	WBR_ClientCont_BgRelease( p_gra );

	// �Z���A�N�^�[�ǂݍ���
	WBR_ClientCont_ClactRelease( p_gra );

	// BGL������
	WBR_ClientCont_BgExit( p_gra );

	// �Z���A�N�^�[������
	WBR_ClientCont_ClactExit( p_gra );

	// �o���N�ݒ�
	WBR_ClientCont_BankExit();
}

//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_GraphicVblank( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
    // BG��������
	GF_BGL_VBlankFunc( p_gra->p_bgl );

    // �����_�����LOAM�}�l�[�W��Vram�]��
    REND_OAMTrans();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���N�ݒ�
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_BankInit( void )
{
    GF_BGL_DISPVRAM tbl = {
        GX_VRAM_BG_256_AC,				// ���C��2D�G���W����BG
        GX_VRAM_BGEXTPLTT_NONE,			// ���C��2D�G���W����BG�g���p���b�g
        GX_VRAM_SUB_BG_32_H,			// �T�u2D�G���W����BG
        GX_VRAM_SUB_BGEXTPLTT_NONE,		// �T�u2D�G���W����BG�g���p���b�g
		GX_VRAM_OBJ_128_B,				// ���C��2D�G���W����OBJ
        GX_VRAM_OBJEXTPLTT_NONE,		// ���C��2D�G���W����OBJ�g���p���b�g
        GX_VRAM_SUB_OBJ_16_I,			// �T�u2D�G���W����OBJ
        GX_VRAM_SUB_OBJEXTPLTT_NONE,	// �T�u2D�G���W����OBJ�g���p���b�g
        GX_VRAM_TEX_NONE,				// �e�N�X�`���C���[�W�X���b�g
        GX_VRAM_TEXPLTT_NONE			// �e�N�X�`���p���b�g�X���b�g
	};
    GF_Disp_SetBank( &tbl );

	// �\���ʐݒ�
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���N�ݒ�j��
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_BankExit( void )
{
	// VRAM�ݒ胊�Z�b�g
	GX_ResetBankForBG();			// ���C��2D�G���W����BG
	GX_ResetBankForBGExtPltt();		// ���C��2D�G���W����BG�g���p���b�g
	GX_ResetBankForSubBG();			// �T�u2D�G���W����BG
	GX_ResetBankForSubBGExtPltt();	// �T�u2D�G���W����BG�g���p���b�g
	GX_ResetBankForOBJ();			// ���C��2D�G���W����OBJ
	GX_ResetBankForOBJExtPltt();	// ���C��2D�G���W����OBJ�g���p���b�g
	GX_ResetBankForSubOBJ();		// �T�u2D�G���W����OBJ
	GX_ResetBankForSubOBJExtPltt();	// �T�u2D�G���W����OBJ�g���p���b�g
	GX_ResetBankForTex();			// �e�N�X�`���C���[�W
	GX_ResetBankForTexPltt();		// �e�N�X�`���p���b�g
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�ݒ�
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_BgInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID )
{
	GF_ASSERT( p_gra->p_bgl == NULL );
	
	// BGL�쐬
	p_gra->p_bgl = GF_BGL_BglIniAlloc( heapID );

	
	{
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS,GX_BGMODE_0,GX_BGMODE_0, GX_BG0_AS_3D
		};
		GF_BGL_InitBG(&BGsys_data);
	}

	{	// �w�i
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_256,
			GX_BG_SCRBASE_0xc000, GX_BG_CHARBASE_0x20000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( p_gra->p_bgl, WBR_BGFRAME_BACK, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( WBR_BGFRAME_BACK, 32, 0, heapID );
		GF_BGL_ScrClear( p_gra->p_bgl, WBR_BGFRAME_BACK );
	}

	{	// ���b�Z�[�W
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0xd000, GX_BG_CHARBASE_0x00000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( p_gra->p_bgl, WBR_BGFRAME_MSG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( WBR_BGFRAME_MSG, 32, 0, heapID );
		GF_BGL_ScrClear( p_gra->p_bgl, WBR_BGFRAME_MSG );
	}

/*	// �Ƃ肠������\��
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
//*/
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�ݒ�j��
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_BgExit( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
    GF_BGL_BGControlExit( p_gra->p_bgl, WBR_BGFRAME_BACK );
    GF_BGL_BGControlExit( p_gra->p_bgl, WBR_BGFRAME_MSG	 );
	sys_FreeMemoryEz( p_gra->p_bgl );
	p_gra->p_bgl = NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�f�[�^�ǂݍ���
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_BgLoad( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID )
{
	// ��b�t�H���g�p���b�g�ǂݍ���
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, WBR_MESSAGE_FONT_STR_COLOR*32, heapID );
	
	// �g�p�O���t�B�b�N�ǂݍ���
	TalkWinGraphicSet( p_gra->p_bgl, WBR_BGFRAME_MSG, 
			WBR_MESSAGE_FONT_CG_OFS, WBR_MESSAGE_FONT_COLOR, 0,
			heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	BG�f�[�^�j��
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_BgRelease( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�V�X�e��������
 *
 *	@param	p_gra	�O���t�B�b�N���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_ClactInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID )
{
	// OAM�}�l�[�W��������
    NNS_G2dInitOamManagerModule();

    // ���LOAM�}�l�[�W���쐬
    // �����_���pOAM�}�l�[�W���쐬
    // �����ō쐬����OAM�}�l�[�W�����݂�Ȃŋ��L����
    REND_OAMInit(
        0, 124,		// ���C�����OAM�Ǘ��̈�
        0, 31,		// ���C����ʃA�t�B���Ǘ��̈�
        0, 124,		// �T�u���OAM�Ǘ��̈�
        0, 31,		// �T�u��ʃA�t�B���Ǘ��̈�
		heapID );
	
	// �L�����N�^�}�l�[�W���[������
	{
		CHAR_MANAGER_MAKE cm = {
			WBR_CHARMAN_CONTNUM,
			WBR_CHARVRAM_MSIZE,
			WBR_CHARVRAM_SSIZE,
			0
		};
		cm.heap = heapID;
		InitCharManagerReg(&cm, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K );
	}
	// �p���b�g�}�l�[�W���[������
	InitPlttManager(WBR_PLTTMAN_CONTNUM, heapID);

	// �ǂݍ��݊J�n�ʒu��������
	CharLoadStartAll();
	PlttLoadStartAll();

	// �ʐM�A�C�R���p�ɃL�������p���b�g����
	CLACT_U_WmIcon_SetReserveAreaCharManager(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
	CLACT_U_WmIcon_SetReserveAreaPlttManager(NNS_G2D_VRAM_TYPE_2DMAIN);

	// �Z���A�N�^�[�Z�b�g�쐬
    p_gra->p_clset = CLACT_U_SetEasyInit( WBR_CLACT_CONTNUM, &p_gra->renddata, heapID );

	// �I�u�W�F�N�g�`��V�X�e���쐬
	p_gra->p_objdraw = WF2DMAP_OBJDrawSysInit( p_gra->p_clset, NULL, WBR_CLACT_CONTNUM, NNS_G2D_VRAM_TYPE_2DMAIN, heapID );


	// �\��
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�V�X�e���j��
 *
 *	@param	p_gra	�O���t�B�b�N�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_ClactExit( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
	// 2Dchar�V�X�e���j��
	WF2DMAP_OBJDrawSysExit( p_gra->p_objdraw );
	
    //OAM�����_���[�j��
    REND_OAM_Delete();

    // ���\�[�X���
    DeleteCharManager();
    DeletePlttManager();

	// �Z���A�N�^�[�Z�b�g�j��
    CLACT_DestSet(p_gra->p_clset);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�f�[�^�ǂݍ���
 *
 *	@param	p_gra		�O���t�B�b�N�f�[�^���[�N
 *	@param	heapID		�q�[�vID
 */	
//-----------------------------------------------------------------------------
static void WBR_ClientCont_ClactLoad( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID )
{
	// ��l���ƃq���C����ǂݍ���
	WF2DMAP_OBJDrawSysResSet( p_gra->p_objdraw, HERO, WF_2DC_MOVERUN, heapID );	
	WF2DMAP_OBJDrawSysResSet( p_gra->p_objdraw, HEROINE, WF_2DC_MOVERUN, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�N�^�[�f�[�^�j��
 *
 *	@param	p_gra		�O���t�B�b�N�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_ClactRelease( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
	// ��l���ƃq���C����j��
	WF2DMAP_OBJDrawSysResDel( p_gra->p_objdraw, HERO );	
	WF2DMAP_OBJDrawSysResDel( p_gra->p_objdraw, HEROINE );
}


//----------------------------------------------------------------------------
/**
 *	@brief	message������
 *
 *	@param	p_gra	���[�N
 *	@param	heapID	�q�[�vID
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_MessageInit( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 heapID )
{
	// ���b�Z�[�W�r�b�g�}�b�v�쐬
	p_gra->msg.p_message = GF_BGL_BmpWinAllocGet( heapID, 1 );

	// �r�b�g�}�b�v�E�B���h�E�o�^
	GF_BGL_BmpWinAdd( p_gra->p_bgl, p_gra->msg.p_message,
			WBR_BGFRAME_MSG, 
			WBR_MESSAGE_FONTBMP_X,
			WBR_MESSAGE_FONTBMP_Y,
			WBR_MESSAGE_FONTBMP_CX, 
			WBR_MESSAGE_FONTBMP_CY, 
			WBR_MESSAGE_FONT_STR_COLOR,
			WBR_MESSAGE_BMP_CG_OFS );

	// �N���[��
	GF_BGL_BmpWinDataFill( p_gra->msg.p_message, 15 );

	// �g�̏�������
	BmpTalkWinWrite( p_gra->msg.p_message, WINDOW_TRANS_OFF,
			WBR_MESSAGE_FONT_CG_OFS,
			WBR_MESSAGE_FONT_COLOR );

	// message�}�l�[�W��
	p_gra->msg.p_man = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_wifi_br_dat, heapID );	
	p_gra->msg.p_wordset = WORDSET_Create( heapID );	

	// STR�o�b�t�@�쐬
	p_gra->msg.p_tmpstr = STRBUF_Create( WBR_MESSAGE_STRBUFF_NUM, heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	message�j��
 *
 *	@param	p_gra	���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_MessageExit( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
	GF_BGL_BmpWinDel( p_gra->msg.p_message );
	GF_BGL_BmpWinFree( p_gra->msg.p_message, 1 );

	STRBUF_Delete( p_gra->msg.p_tmpstr );
	WORDSET_Delete( p_gra->msg.p_wordset );
	MSGMAN_Delete( p_gra->msg.p_man );
}

//----------------------------------------------------------------------------
/**
 *	@brief	messageON
 *
 *	@param	p_gra	���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_MessageOn( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
	GF_BGL_BmpWinOn( p_gra->msg.p_message );
	GF_Disp_GX_VisibleControl( WBR_BGMSK_MSG, VISIBLE_ON );
}

//----------------------------------------------------------------------------
/**
 *	@brief	messageOFF
 *
 *	@param	p_gra	���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_MessageOff( WBR_CLIENTCONT_GRAPHIC* p_gra )
{
	GF_Disp_GX_VisibleControl( WBR_BGMSK_MSG, VISIBLE_OFF );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[���ݒ�
 *
 *	@param	p_gra		���[�N
 *	@param	buffid		�o�b�t�@ID
 *	@param	p_status	�X�e�[�^�X�f�[�^
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_MessageTrNameSet( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 buffid, const MYSTATUS* p_status )
{
	WORDSET_RegisterPlayerName( p_gra->msg.p_wordset, buffid, p_status );
}

//----------------------------------------------------------------------------
/**
 *	@brief	message�v�����g
 *
 *	@param	p_gra		���[�N
 *	@param	strid		STRID
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_MessagePrint( WBR_CLIENTCONT_GRAPHIC* p_gra, u32 strid )
{
	STRBUF* p_str;

	// �N���[��
	GF_BGL_BmpWinDataFill( p_gra->msg.p_message, 15 );

	p_str = MSGMAN_AllocString( p_gra->msg.p_man, strid );
	WORDSET_ExpandStr( p_gra->msg.p_wordset, p_gra->msg.p_tmpstr, p_str );

	p_gra->msg.okuri = GF_STR_PrintColor( p_gra->msg.p_message, FONT_TALK, p_gra->msg.p_tmpstr, 0, 0, 1, WBR_MESSAGE_FONTBMP_PRICOLOR, NULL );

	STRBUF_Delete( p_str );
}

//----------------------------------------------------------------------------
/**
 *	@brief	message�v�����g�I���҂�
 *
 *	@param	cp_gra	���[�N
 *
 *	@retval	TRUE	�I��
 */
//-----------------------------------------------------------------------------
static BOOL WBR_ClientCont_MessageEndCheck( const WBR_CLIENTCONT_GRAPHIC* cp_gra )
{
	if( GF_MSG_PrintEndCheck( cp_gra->msg.okuri ) == 0 ){
		return TRUE;
	}
	return FALSE;
}	


//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^����
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_graphic	�O���t�B�b�N�f�[�^
 *	@param	p_client	�N���C�A���g�f�[�^
 *	@param	id			AID
 *	@param	hero		��l���t���O
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_ObjInit( WBR_CLIENTCONT* p_sys, WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT_GRAPHIC* p_graphic, WBR_CLIENT_COMMON* p_client, int id, BOOL hero )
{
	WF2DMAP_OBJDATA add = {
		0, 0, 0,
		WF2DMAP_OBJST_NONE,
		WF2DMAP_WAY_UP,
		0
	};
	u32 sex;
	
	// �Ղꂢ��[ID��ݒ�
	add.playid = id;
	
	// mystatus���琫�ʂ��擾
	sex = MyStatus_GetMySex( p_client->cp_mystatus->cp_mystatus[ id ] );
	if( sex == PM_MALE ){
		add.charaid = HERO;
	}else{
		add.charaid = HEROINE;
	}
	
	// �I�u�W�F�N�g�f�[�^����
	p_obj->p_obj = WF2DMAP_OBJWkNew( p_sys->p_objsys, &add );
	p_obj->p_draw = WF2DMAP_OBJDrawWkNew( p_graphic->p_objdraw, 
			p_obj->p_obj, hero, p_client->heapID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�j��
 *
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_ObjDel( WBR_CLIENTCONTOBJ* p_obj )
{
	WF2DMAP_OBJDrawWkDel( p_obj->p_draw );
	WF2DMAP_OBJWkDel( p_obj->p_obj );
}

//----------------------------------------------------------------------------
/**
 *	@brief	��l������
 *	
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_player	��l�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_PlayerMove( WBR_CLIENTCONT* p_sys, WBR_CLIENTCONTOBJ* p_player )
{
	u32 way;
	WBR_KO_COMM_GAME comm_data;
	u32 move_way;
	BOOL send_check;
	WBR_TALK_REQ talk_req;

	// ���̃t���[���R�}���h��]�����邩�`�F�b�N
	send_check = WBR_ClientCont_PlayerReqCmdSendOkCheck( p_player );
	if( send_check == FALSE ){
		return ;
	}
	
	// �b�������`�F�b�N
	if( sys.trg & PAD_BUTTON_A ){
		talk_req.netid = WF2DMAP_OBJWkDataGet( p_player->p_obj, WF2DMAP_OBJPM_PLID );
		talk_req.flag = WBR_TALK_REQ_START;
		// ���M
		CommSendData( CNM_WBR_KO_TALKREQ, &talk_req, sizeof(WBR_TALK_REQ) );
	}else{
	
		// �L�[��񂩂�������擾
		move_way = WBR_ClientCont_KayWayGet( sys.cont );

		if( move_way != WF2DMAP_WAY_NUM ){

			// �������Ă�������Ɣ�ׂăR�}���h���s
			way = WF2DMAP_OBJWkDataGet( p_player->p_obj, WF2DMAP_OBJPM_WAY );
			comm_data.req.way = move_way;
			comm_data.req.playid = WF2DMAP_OBJWkDataGet( p_player->p_obj, WF2DMAP_OBJPM_PLID );
			if( move_way == way ){
				if( sys.cont & PAD_BUTTON_B ){
					comm_data.req.cmd = WF2DMAP_CMD_RUN;
				}else{
					comm_data.req.cmd = WF2DMAP_CMD_WALK;
				}
			}else{
				comm_data.req.cmd = WF2DMAP_CMD_TURN;
			}

			// ���M
			CommSendData( CNM_WBR_KO_GAMEDATA, &comm_data, sizeof(WBR_KO_COMM_GAME) );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h��]�����邷��K�v�����邩�`�F�b�N
 *
 *	@param	cp_obj	�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���N�G�X�g�R�}���h��]�����đ��v
 *	@retval	FALSE	���N�G�X�g�R�}���h�͓]�����Ȃ��Ă悢
 */
//-----------------------------------------------------------------------------
static BOOL WBR_ClientCont_PlayerReqCmdSendOkCheck( const WBR_CLIENTCONTOBJ* cp_obj )
{
	s32 frame;
	s32 end_frame;
	u32 status;
	
	// �ҋ@��ԂȂ疳�����œ]��
	status = WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_ST );
	if( status == WF2DMAP_OBJST_NONE ){
		return TRUE;
	}
	// �r�W�[��ԂȂ疳�����œ]�����Ȃ�
	if( status == WF2DMAP_OBJST_BUSY ){
		return FALSE;
	}

	frame = WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_FRAME );
	end_frame = WF2DMAP_OBJWkDataGet( cp_obj->p_obj, WF2DMAP_OBJPM_ENDFRAME );

	if( frame >= end_frame - WBR_OBJ_CMDSEND_TIMING ){
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��b���`�F�b�N
 *
 *	@param	cp_obj	�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	��b��
 *	@retval	FALSE	��b���ĂȂ�
 */
//-----------------------------------------------------------------------------
static BOOL WBR_ClientCont_PlayerTalkCheck( const WBR_CLIENTCONTOBJ* cp_obj )
{
	if( cp_obj->talk_data.talk_mode == WBR_TALK_NONE ){
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b����������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_player	���샏�[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_PlayerTalkMove( WBR_CLIENTCONT* p_sys, WBR_CLIENTCONTOBJ* p_player )
{
	static void (* const pTalkInit[])( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys ) = {
		WBR_ClientCont_Talk_StartInit,
		WBR_ClientCont_Talk_NextInit,
		WBR_ClientCont_Talk_EndInit,
		WBR_ClientCont_Talk_ExitInit,
		WBR_ClientCont_Talk_TrCardReqInit,
		WBR_ClientCont_Talk_TrCardStartInit,
		WBR_ClientCont_Talk_TrCardEndWaitInit,
		WBR_ClientCont_Talk_TrCardEndInit,
	};
	static WBR_CLIENTCONT_RETTYPE (* const pTalkMain[])( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys ) = {
		WBR_ClientCont_Talk_StartMain,
		WBR_ClientCont_Talk_NextMain,
		WBR_ClientCont_Talk_EndMain,
		WBR_ClientCont_Talk_ExitMain,
		WBR_ClientCont_Talk_TrCardReqMain,
		WBR_ClientCont_Talk_TrCardStartMain,
		WBR_ClientCont_Talk_TrCardEndWaitMain,
		WBR_ClientCont_Talk_TrCardEndMain,
	};

	// ����������
	if( p_player->talk_recv ){
		p_player->talk_recv = FALSE;
		pTalkInit[ p_player->talk_data.talk_seq ]( p_player, p_sys );
		p_player->talk_recv_wait = FALSE;
	}

	// ���C��
	if( p_player->talk_recv_wait == FALSE ){
		return pTalkMain[ p_player->talk_data.talk_seq ]( p_player, p_sys );
	}

	return WBR_CLIENTCONT_RET_NONE;
}



//----------------------------------------------------------------------------
/**
 *	@brief	�b�������J�n
 *
 *	@param	p_obj		�I�u�W�F�N�g
 *	@param	p_sys		�V�X�e��
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_StartInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	const MYSTATUS* cp_mystatus;
	
	// ����g���[�i�[����ݒ�
	cp_mystatus = p_sys->p_prt->cp_mystatus->cp_mystatus[ p_obj->talk_data.talk_playid ];
	WBR_ClientCont_MessageTrNameSet( &p_sys->graphic, 0, cp_mystatus );

	// �e���q�ɂ����message��ύX
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_000 );
	}else{
		WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_001 );
	}
	WBR_ClientCont_MessageOn( &p_sys->graphic );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Ă̘b�������@������
 *
 *	@param	p_obj	�I�u�W�F�N�g���[�N
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_NextInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	const MYSTATUS* cp_mystatus;
	
	// ����g���[�i�[����ݒ�
	cp_mystatus = p_sys->p_prt->cp_mystatus->cp_mystatus[ p_obj->talk_data.talk_playid ];
	WBR_ClientCont_MessageTrNameSet( &p_sys->graphic, 0, cp_mystatus );

/*	�ʏ�
	// �e���q�ɂ����message��ύX
	if( p_obj->obj_data.talk_mode == WBR_TALK_OYA ){
		WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_000 );
	}else{
		WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_001 );
	}
//*/
	WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_005 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������I��	������
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_EndInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_ClientCont_MessageOff( &p_sys->graphic );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b������������	������
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_ExitInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h���N�G�X�g
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_TrCardReqInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_002 );
	}else{
		WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_003 );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h������
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_TrCardStartInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�I���҂�	������
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_TrCardEndWaitInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	// �ق��̐l�̏I�����܂��Ă�����
	WBR_ClientCont_MessagePrint( &p_sys->graphic, WBR_004 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�I��	������
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCont_Talk_TrCardEndInit( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
}


//----------------------------------------------------------------------------
/**
 *	@brief	�b�������J�n	���C��
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_StartMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;
	
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		if( WBR_ClientCont_MessageEndCheck( &p_sys->graphic ) == TRUE ){
			// message���I�������g���[�i�[�J�[�h���N�G�X�g��
			comm_data.talk_data = p_obj->talk_data;
			comm_data.talk_data.talk_seq = WBR_TALK_SEQ_TRCARD_REQ;
			CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
			p_obj->talk_recv_wait = 1;	// ��M�҂�
		}
	}
	return WBR_CLIENTCONT_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b����������
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_NextMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;
/*	�ʏ�
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		if( WBR_ClientCont_MessageEndCheck( &p_sys->graphic ) == TRUE ){
			// message���I�������g���[�i�[�J�[�h���N�G�X�g��
			comm_data.talk_data = p_obj->talk_data;
			comm_data.talk_data.talk_seq = WBR_TALK_SEQ_TRCARD_REQ;
			CommSendData( CNM_WBR_KO_TALKDATA, &p_obj->ko_comm_talk, sizeof(WBR_KO_COMM_TALK) );
			p_obj->talk_recv_wait = 1;	// ��M�҂�
		}
	}
//*/
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		if( WBR_ClientCont_MessageEndCheck( &p_sys->graphic ) == TRUE ){
			// message���I�������g���[�i�[�J�[�h���N�G�X�g��
			comm_data.talk_data = p_obj->talk_data;
			comm_data.talk_data.talk_seq = WBR_TALK_SEQ_END;
			CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
			p_obj->talk_recv_wait = 1;	// ��M�҂�
		}
	}
	return WBR_CLIENTCONT_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������I��	���C��
 *	
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_EndMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		if( WBR_ClientCont_MessageEndCheck( &p_sys->graphic ) == TRUE ){
			// message���I�������I����
			comm_data.talk_data = p_obj->talk_data;
			comm_data.talk_data.talk_seq = WBR_TALK_SEQ_EXIT;
			CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
			p_obj->talk_recv_wait = 1;	// ��M�҂�
		}
	}

	return WBR_CLIENTCONT_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b������������	���C��
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_ExitMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_TALK_REQ talk_req;

	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		talk_req.flag = WBR_TALK_REQ_END;
		talk_req.netid = WF2DMAP_OBJWkDataGet( p_obj->p_obj, WF2DMAP_OBJPM_PLID );
		CommSendData( CNM_WBR_KO_TALKREQ, &talk_req, sizeof(WBR_TALK_REQ) );
		p_obj->talk_recv_wait = 1;	// ��M�҂�
	}
	return WBR_CLIENTCONT_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h���N�G�X�g���C��
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardReqMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;
	
	// �q�Ȃ�g���[�i�[�J�[�h���s��
	if( p_obj->talk_data.talk_mode == WBR_TALK_KO ){
		if( WBR_ClientCont_MessageEndCheck( &p_sys->graphic ) == TRUE ){
			// message���I�������g���[�i�[�J�[�h��
			comm_data.talk_data = p_obj->talk_data;
			comm_data.talk_data.talk_seq = WBR_TALK_SEQ_TRCARD_START;
			CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
			p_obj->talk_recv_wait = 1;	// ��M�҂�
		}
	}
	return WBR_CLIENTCONT_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�@���C��
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardStartMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;
	// �V�[�P���X��ҋ@�܂��ɂ��Ă���
	comm_data.talk_data = p_obj->talk_data;
	comm_data.talk_data.talk_seq = WBR_TALK_SEQ_TRCARD_ENDWAIT;
	CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
	p_obj->talk_recv_wait = 1;	// ��M�҂�
	
	// �J�n�Ȃ̂ŁA�J�n������
	return WBR_CLIENTCONT_RET_TRCARD;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�I���҂�
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardEndWaitMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;
	
	// �e�@�͎q�@���g���[�i�[�J�[�h�I���ɂȂ�̂�҂�
	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		// �q�@�̘b�f�[�^�����̃V�[�P���X�Ɍ����Ă�����OK
		if( p_sys->obj[ p_obj->talk_data.talk_playid ].talk_data.talk_seq == WBR_TALK_SEQ_TRCARD_END ){
			comm_data.talk_data = p_obj->talk_data;
			comm_data.talk_data.talk_seq = WBR_TALK_SEQ_TRCARD_END;
			CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
			p_obj->talk_recv_wait = 1;	// ��M�҂�
		}
	}else{
		// �q�@�͂��₫�ɏI����]��
		comm_data.talk_data = p_obj->talk_data;
		comm_data.talk_data.talk_seq = WBR_TALK_SEQ_TRCARD_END;
		CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
		p_obj->talk_recv_wait = 1;	// ��M�҂�
	}
	return WBR_CLIENTCONT_RET_NONE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�g���[�i�[�J�[�h�I��	���C��
 *
 *	@param	p_obj		�I�u�W�F�N�g���[�N
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static WBR_CLIENTCONT_RETTYPE WBR_ClientCont_Talk_TrCardEndMain( WBR_CLIENTCONTOBJ* p_obj, WBR_CLIENTCONT* p_sys )
{
	WBR_KO_COMM_TALK comm_data;

	if( p_obj->talk_data.talk_mode == WBR_TALK_OYA ){
		comm_data.talk_data = p_obj->talk_data;
		comm_data.talk_data.talk_seq = WBR_TALK_SEQ_NEXT;
		CommSendData( CNM_WBR_KO_TALKDATA, &comm_data, sizeof(WBR_KO_COMM_TALK) );
		p_obj->talk_recv_wait = 1;	// ��M�҂�
	}
	return WBR_CLIENTCONT_RET_NONE;
}

