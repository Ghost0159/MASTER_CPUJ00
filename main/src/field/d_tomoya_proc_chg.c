//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d_tomoya_proc_chg.c
 *	@brief		�v���b�N�ύX����f�o�b�N�f�[�^
 *	@author		tomoya takahashi
 *	@data		2006.03.27
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifdef PM_DEBUG

#include "common.h"

#include "system/lib_pack.h"
#include "system/bmp_list.h"
#include "system/bmp_menu.h"
#include "fld_bmp.h"
#include "fieldmap.h"
#include "fieldsys.h"
#include "fieldmap_work.h"
#include "system/fontproc.h"

#include "d_tomoya.h"
#include "system/touchpanel_draw.h"
#include "field_light.h"
#include "fog_sys.h"
#include "weather_sys.h"
#include "field_glb_state.h"

#include "encount_effect.h"
#include "effect_uground.h"

#include "system/clact_util.h"
#include "system/render_oam.h"
#include "field_event.h"
#include "system/wipe.h"


#include "msgdata/msg.naix"
#include "msgdata/msg_debug_tomoya.h"

#include "include/application/imageClip/imc_playsys.h"
#include "include/application/imageClip/imc_sys.h"
#include "include/savedata/imageclip_data.h"
#include "include/application/imageClip/imc_preview.h"

#include "poketool/poke_tool.h"
#include "include/application/zukanlist/zkn_sys.h"

#include "src/field/ev_time.h"


#define	__D_TOMOYA_PROC_CHG_H_GLOBAL
#include "d_tomoya_proc_chg.h"

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

// ���������悤�@�O���[�o���ϐ�
u32 D_Tomoya_MazeMaze_TeamWork;

// ���r�[��p
BOOL D_Tomoya_WiFiLobby_DebugStart = FALSE;
u32 D_Tomoya_WiFiLobby_ChannelPrefix;
BOOL D_Tomoya_WiFiLobby_ChannelPrefixFlag = FALSE;
BOOL D_Tomoya_WiFiLobby_ALLVip = FALSE;
BOOL D_Tomoya_WiFiLobby_LockTime_Long = FALSE;

// �v���Z�X�ύX����
void DTomoya_ProcChenge( TCB_PTR tcb, void* work )
{
	DT_SPRITE_CHECK* data = work;
	
	switch( data->seq ){
	case 0:		// �t�F�[�h�A�E�g
		WIPE_SYS_Start( 
				WIPE_PATTERN_FMAS,
				WIPE_TYPE_FADEOUT,
				WIPE_TYPE_FADEOUT,
				0,
				6,1,
				HEAPID_FIELD );
		data->tmp_seq = 2;
		data->seq++;
		break;

	case 1:	// �t�F�[�h�҂�	
		if( WIPE_SYS_EndCheck() ){

			data->seq = data->tmp_seq;
		}
		break;

	case 2:		// �I��������
		data->pStartFunc( data );
		data->seq++;	
		break;

	case 3:		// �I���I���҂�
		if( GameSystem_CheckSubProcExists( data->p_fsys ) == FALSE ){

			// �t�B�[���h���A
			GameSystem_CreateFieldProc( data->p_fsys );
			data->seq++;
		}
		break;

	case 4:
		// �t�B�[���h���A�������������`�F�b�N
		if( GameSystem_CheckFieldProcExists(data->p_fsys) ){
/*		�t�B�[���h���͎����Ńt�F�[�h�C�����邽��
 *		�R�����g�A�E�g */
			WIPE_SYS_Start( 
					WIPE_PATTERN_FSAM,
					WIPE_TYPE_FADEIN,
					WIPE_TYPE_FADEIN,
					WIPE_FADE_OUTCOLOR,
					6,1,
					HEAPID_FIELD );
			// �t�F�[�h�C��
			data->seq = 1;
			data->tmp_seq = 5;//*/
/*			WIPE_SYS_Start( 
					WIPE_PATTERN_S,
					WIPE_TYPE_FADEIN,
					WIPE_TYPE_FADEIN,
					0,
					8,2,
					HEAPID_FIELD );
			// �t�F�[�h�C��
			data->seq = 1;
			data->tmp_seq = 5;//*/
		}
		break;

	case 5:		// �I��

		// ���[�N�J��
		data->pEndFunc( data );

		// �^�X�N�I��
		TCB_Delete( tcb );
		sys_FreeMemoryEz( work );
		break;
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����ł��\�[�X
 */	
//-----------------------------------------------------------------------------
BOOL EventScr_D_FldTrade(GMEVENT_CONTROL * event)
{
	FIELDSYS_WORK * p_fsys = FieldEvent_GetFieldSysWork(event);
	D_TOMOYA_EVENT_WORK* p_work = FieldEvent_GetSpecialWork(event);
	u16 monsno, monsno_chg;
	POKEPARTY* party = SaveData_GetTemotiPokemon( p_fsys->savedata );
	POKEMON_PARAM* pp = PokeParty_GetMemberPointer( party, 0 );

	switch( p_work->seq ){
	case 0:	// �����ł��邩�`�F�b�N
		monsno = FLD_TradeChangeMonsno( p_work->p_work );
		monsno_chg = PokeParaGet( pp, ID_PARA_monsno, NULL );
		if( monsno == monsno_chg ){
			p_work->seq++;
		}else{
			return TRUE;
		}
		break;

	case 1:	//����
		EventCmd_FldTrade( event, p_work->p_work, 0, HEAPID_BASE_DEBUG );
		p_work->seq++;
		break;

	case 2:	// �I��
		FLD_TradeDelete( p_work->p_work );
		sys_FreeMemoryEz( p_work );
		return TRUE;
	}
	return FALSE;
}

#endif // PM_DEBUG
