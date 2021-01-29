//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_ev_anketo.c
 *	@brief		�A���P�[�g�֌W�̃C�x���g
 *	@author		tomoya takahashi
 *	@data		2008.05.30
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"


#include "msgdata/msg_wifi_hiroba.h"

#include "wflby_def.h"
#include "wflby_system_def.h"
#include "wflby_room_def.h"
#include "wflby_ev_anketo.h"

#include "wflby_snd.h"

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
//-------------------------------------
///	�V�[�P���X
//=====================================
enum {
	WFLBY_EV_ANKETO_CHECK,			// ���͂Ȃ������߂�
	WFLBY_EV_ANKETO_BEFORE_YESNO,	// �����𕷂����H
	WFLBY_EV_ANKETO_BEFORE_YESNOWAIT,// �����𕷂����H
	WFLBY_EV_ANKETO_BEFORE_MSG00,	// �A���P�[�g������O

	WFLBY_EV_ANKETO_END,			// �I��
	
	WFLBY_EV_ANKETO_MSGWAIT,		// �A���P�[�g������O
} ;


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A���P�[�g�s�G�����[�N
//=====================================
typedef struct {
	u32 ret_seq;
} WFLBY_EV_ANKETO_PIERO;


//----------------------------------------------------------------------------
/**
 *	@brief	�s�G���͂Ȃ�����		���荞�݃C�x���g
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_EV_TALK_Piero( WFLBY_EVENTWK* p_wk, WFLBY_ROOMWK* p_rmwk, u32 plno )
{
	WFLBY_EV_ANKETO_PIERO* p_evwk;
	WFLBY_SYSTEM* p_system;
	WFLBY_3DOBJCONT* p_objcont;
	WFLBY_3DPERSON* p_pierrot;
	WFLBY_3DPERSON* p_player;
	STRBUF* p_str;

	p_evwk		= WFLBY_EVENTWK_GetWk( p_wk );
	p_system	= WFLBY_ROOM_GetSystemData( p_rmwk );
	p_objcont	= WFLBY_ROOM_GetObjCont( p_rmwk );
	p_pierrot	= WFLBY_3DOBJCONT_GetPierrot( p_objcont );
	p_player	= WFLBY_3DOBJCONT_GetPlayer( p_objcont );

	switch( WFLBY_EVENTWK_GetSeq( p_wk ) ){
	case WFLBY_EV_ANKETO_CHECK:			// ���͂Ȃ������߂�

		p_evwk = WFLBY_EVENTWK_AllocWk( p_wk, sizeof(WFLBY_EV_ANKETO_PIERO) );


		Snd_SePlay( WFLBY_SND_STAFF );
		
		
		{
			u32 way;

			way			= WFLBY_3DOBJCONT_GetWkObjData( p_player,  WF2DMAP_OBJPM_WAY );
			way			= WF2DMPA_OBJToolRetWayGet( way );

			// �s�G���������̂ق��Ɍ������āA����ɓ������~�߂�
			WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_pierrot, WFLBY_3DOBJCONT_MOVENONE );
			WFLBY_3DOBJCONT_DRAW_SetUpdata( p_pierrot, FALSE );
			WFLBY_3DOBJCONT_DRAW_SetWay( p_pierrot, way );
		}


		// �L��CM�I����
		if( WFLBY_SYSTEM_Event_GetEndCM( p_system ) ){
			// ���b�Z�[�W�\�����ďI���
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_hiroba_infoa_04 );
			p_evwk->ret_seq = WFLBY_EV_ANKETO_END;
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_MSGWAIT );
			break;
		}

		// ���ʔ��\�����`�F�b�N
		if( (WFLBY_SYSTEM_Event_GetNeon( p_system ) == TRUE) ){
			// ���b�Z�[�W�\�����ďI���
			p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_survey_guide_a_02 );
			p_evwk->ret_seq = WFLBY_EV_ANKETO_END;
			WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
			WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_MSGWAIT );
			break;
		}else{
			// ���[���݁H
			if( WFLBY_SYSTEM_FLAG_GetAnketoInput( p_system ) == TRUE ){
				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_survey_guide_a_01 );
				p_evwk->ret_seq = WFLBY_EV_ANKETO_END;
				WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_MSGWAIT );
				break;
			}else{

				p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_survey_guide_b_01 );
				p_evwk->ret_seq = WFLBY_EV_ANKETO_BEFORE_YESNO;
				WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_MSGWAIT );
			}
		}
		
		break;

	case WFLBY_EV_ANKETO_BEFORE_YESNO:	// �����𕷂����H
		WFLBY_ROOM_YESNOWIN_Start( p_rmwk );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_BEFORE_YESNOWAIT );
		break;

	case WFLBY_EV_ANKETO_BEFORE_YESNOWAIT:// �����𕷂����H
		{
			WFLBY_ROOM_YESNO_RET result;
			
			result = WFLBY_ROOM_YESNOWIN_Main( p_rmwk );
			switch( result ){
			case WFLBY_ROOM_YESNO_OK:	// �͂�
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_BEFORE_MSG00 );
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				break;
				
			case WFLBY_ROOM_YESNO_NO:	// ������
				WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_END );
				WFLBY_ROOM_YESNOWIN_End( p_rmwk );
				break;
				
			default:
			case WFLBY_ROOM_YESNO_WAIT:	// �I�ђ�
				break;
			}
		}
		break;

	case WFLBY_EV_ANKETO_BEFORE_MSG00:	// �A���P�[�g������O
		p_str = WFLBY_ROOM_MSG_Get( p_rmwk, WFLBY_DEFMSG_TYPE_HIROBA, msg_survey_guide_b_05 );
		p_evwk->ret_seq = WFLBY_EV_ANKETO_END;
		WFLBY_ROOM_TALKWIN_PrintStr( p_rmwk, p_str );
		WFLBY_EVENTWK_SetSeq( p_wk, WFLBY_EV_ANKETO_MSGWAIT );
		break;

	case WFLBY_EV_ANKETO_END:			// �I��
		WFLBY_ROOM_TALKWIN_Off( p_rmwk );
		WFLBY_ROOM_YESNOWIN_End( p_rmwk );
		WFLBY_3DOBJCONT_SetWkMove( p_objcont, p_player, WFLBY_3DOBJCONT_MOVEPLAYER );
		WFLBY_EVENTWK_DeleteWk( p_wk );

		// �s�G�����앜�A
		WFLBY_3DOBJCONT_DRAW_SetUpdata( p_pierrot, TRUE );
		// ��������
		WFLBY_3DOBJCONT_SetWkActCmd( p_objcont, p_pierrot, WF2DMAP_CMD_NONE, WF2DMAP_WAY_DOWN );
		return TRUE;

	case WFLBY_EV_ANKETO_MSGWAIT:		// �A���P�[�g������O
		// ���b�Z�[�W�I���҂�
		if( WFLBY_ROOM_TALKWIN_WaitEnd( p_rmwk ) == TRUE ){

			WFLBY_EVENTWK_SetSeq( p_wk, p_evwk->ret_seq );
		}
		break;

	default:
		GF_ASSERT(0);
		break;
	}

	return FALSE;
}
