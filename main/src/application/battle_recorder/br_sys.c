//==============================================================================
/**
 * @file	br_sys.c
 * @brief	�o�g�����R�[�_�[
 * @author	goto
 * @date	2007.07.26(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#include "common.h"
#include "system/procsys.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/pm_str.h"
#include "system/clact_tool.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "system/window.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_fightmsg_dp.h"
#include "system/wipe.h"
#include "system/brightness.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "system/fontoam.h"
#include "system/msg_ds_icon.h"
#include "gflib/msg_print.h"
#include "gflib/touchpanel.h"
#include "poketool/poke_tool.h"
#include "poketool/monsno.h"
#include "poketool/pokeicon.h"
#include "poketool/boxdata.h"
#include "system/bmp_menu.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "system/pm_overlay.h"

#include "include/savedata/misc.h"

#include "br_private.h"

// =============================================================================
//
//
//	�����������@�o�g�����R�[�_�[�� 60fps �ŕ`��X�V��������܂� ����������
//
//
// =============================================================================

// ----------------------------------------
//
//	�v���Z�X��`
//
// ----------------------------------------
static PROC_RESULT BR_Proc_InitBrowse( PROC * proc, int * seq );
static PROC_RESULT BR_Proc_InitGDS( PROC * proc, int * seq );
static PROC_RESULT BR_Proc_Init( PROC * proc, int * seq, int mode );
static PROC_RESULT BR_Proc_Main( PROC * proc, int * seq );
static PROC_RESULT BR_Proc_Exit( PROC * proc, int * seq );

FS_EXTERN_OVERLAY( battle_recorder );

///< ���R�[�_�[���[�h
const PROC_DATA BattleRecorder_BrowseProcData = {
	BR_Proc_InitBrowse,
	BR_Proc_Main,
	BR_Proc_Exit,
	
	FS_OVERLAY_ID( battle_recorder ),	
};

///< �f�c�r���[�h
const PROC_DATA BattleRecorder_GDSProcData = {
	BR_Proc_InitGDS,
	BR_Proc_Main,
	BR_Proc_Exit,
	
	FS_OVERLAY_ID( battle_recorder ),	
};


//--------------------------------------------------------------
/**
 * @brief	�v���b�N�̏�����
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Proc_Init( PROC * proc, int * seq, int mode )
{
	BR_WORK* wk;
	
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_BR, HEAP_SIZE_BR );

	wk = BR_PROC_GetWork( proc );
	
//	wk->mode	  = mode;	�����ŗv��Ȃ�

	BR_Stack_Menu_Back( wk );	
	Snd_PlayerSetPlayerVolume( PLAYER_FIELD, BGM_VOL_BR );
	if(mode != 0){
		Snd_DataSetByScene( SND_SCENE_FIELD, SEQ_PL_WIFITOWER, 1 );	//GDS�̋Ȃ�炷
	}
	
	if ( mode == BR_MODE_BROWSE ){
		///< ���R�[�_�[�J���[�̎擾
		{
			MISC* misc = SaveData_GetMisc( wk->save );
			MISC_GetBattleRecoderColor( misc, &wk->sys.color_type );
			
			if ( wk->sys.color_type >= CHANGE_COLOR_MAX ){
				wk->sys.color_type = 0;
			}
			wk->sys.logo_color = BR_FadeColorCode_Get( wk );
		}
	}
	else {
		wk->sys.logo_color = LOGO_COLOR_G;
	}
		
	BR_SystemInit( wk );
	
// 	OS_Printf( "������\n");	
	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief	�u���E�U�[������
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Proc_InitBrowse( PROC * proc, int * seq )
{
// 	OS_Printf( "Browse mode \n");
	
	return BR_Proc_Init( proc, seq, BR_MODE_BROWSE );
}


//--------------------------------------------------------------
/**
 * @brief	GDS������
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Proc_InitGDS( PROC * proc, int * seq )
{
// 	OS_Printf( "GDS mode \n");
	
	return BR_Proc_Init( proc, seq, BR_MODE_GDS );
}


//--------------------------------------------------------------
/**
 * @brief	�v���b�N
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Proc_Main( PROC * proc, int * seq )
{
	BOOL bState = FALSE;
	BR_WORK* wk = BR_PROC_GetWork( proc );
	
	wk->proc_seq = seq;
	
	bState = BR_Main_Update( wk, seq );
	
	return ( bState ) ? PROC_RES_FINISH : PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief	�v���b�N�̏I��
 *
 * @param	proc	
 * @param	seq	
 *
 * @retval	static PROC_RESULT	
 *
 */
//--------------------------------------------------------------
static PROC_RESULT BR_Proc_Exit( PROC * proc, int * seq )
{
	BR_WORK* wk = BR_PROC_GetWork( proc );
	
	switch ( *seq ){
	case 0:
		///< TCB �n�̒�~
		BR_SideBar_TCB_Delete( wk );
		( *seq )++;
		break;
	
	case 1:
		///< �O���t�B�b�N�j��
		BR_SideBar_Delete( wk );
		BR_Tag_Delete( wk );
		BR_disp_FontOam_Delete( wk );
		( *seq )++;
		break;
	
	case 2:
		///< cursor del
		{
			if ( Cursor_Delete( wk->cur_wk ) == FALSE ){				
				( *seq )++;
			}
		}
		break;
	
	case 3:
		///< cursor del
		{
			if ( Cursor_Delete( wk->cur_wk_top ) == FALSE ){				
				( *seq )++;
			}
		}
		break;
		
	
	default:
		///< system �J��
		BR_SystemExit( wk );
		sys_DeleteHeap( HEAPID_BR );		
		
		Overlay_UnloadID( FS_OVERLAY_ID( battle_recorder ) );
		
		sys.disp3DSW = DISP_3D_TO_MAIN;
		GF_Disp_DispSelect();
		
	//	Snd_PlayerSetPlayerVolume( PLAYER_FIELD, BGM_VOL_MAX );
		
	//	OS_Printf( "�I��\n");
		return PROC_RES_FINISH;
	}
	
	return PROC_RES_CONTINUE;
}
