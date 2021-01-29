//==============================================================================
/**
 * @file	codein_pv.c
 * @brief	�������̓C���^�[�t�F�[�X
 * @author	goto
 * @date	2007.07.11(��)
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

#include "codein_pv.h"
#include "codein_snd.h"



//--------------------------------------------------------------
/**
 * @brief	�t�H�[�J�X�Z�b�g���Ċg��k���Ώۂ�ݒ肷��
 *
 * @param	next_focus	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_FocusSet( CODEIN_WORK* wk, int next_focus )
{
	int index;
	wk->focus_old = wk->focus_now;
	wk->focus_now = next_focus;
	
	wk->ls = 0;
	wk->le = 0;
	wk->ss = 0;
	wk->se = 0;
#if 0	
	if ( wk->focus_now != 0 ){		
		wk->ls = ( wk->focus_now - 1 ) * wk->param.block[ wk->focus_now - 0 ];
		wk->le = ( wk->focus_now - 0 ) * wk->param.block[ wk->focus_now - 1 ];
	}

	if ( wk->focus_old != 0 ){		
		wk->ss = ( wk->focus_old - 1 ) * wk->param.block[ wk->focus_old - 0 ];
		wk->se = ( wk->focus_old - 0 ) * wk->param.block[ wk->focus_old - 1 ];
	}
#endif

	if ( wk->focus_now != 0 ){
		wk->ls = wk->b_tbl[ wk->focus_now - 1 ][ 0 ];
		wk->le = wk->b_tbl[ wk->focus_now - 1 ][ 1 ];
	}
	if ( wk->focus_old != 0 ){
		wk->ss = wk->b_tbl[ wk->focus_old - 1 ][ 0 ];
		wk->se = wk->b_tbl[ wk->focus_old - 1 ][ 1 ];
	}

	OS_Printf( "fn = %2d | fo =  %2d\n", wk->focus_now, wk->focus_old );
	OS_Printf( "L = %2d ~ %2d\n", wk->ls, wk->le );
	OS_Printf( "S = %2d ~ %2d\n", wk->ss, wk->se );
}

//--------------------------------------------------------------
/**
 * @brief	�e��f�[�^������������
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_ParamInit( CODEIN_WORK* wk )
{
	int i;
	
	///< Touch �p�l�����[�h
	wk->sys.touch = TRUE;
	
	///< block table �̍쐬
	{
		wk->b_tbl[ 0 ][ 0 ] = 0;
		wk->b_tbl[ 0 ][ 1 ] = wk->param.block[ 0 ];
		wk->b_tbl[ 1 ][ 0 ] = wk->param.block[ 0 ];
		wk->b_tbl[ 1 ][ 1 ] = wk->param.block[ 0 ] + wk->param.block[ 1 ];
		wk->b_tbl[ 2 ][ 0 ] = wk->param.block[ 0 ] + wk->param.block[ 1 ];
		wk->b_tbl[ 2 ][ 1 ] = wk->param.block[ 0 ] + wk->param.block[ 1 ] + wk->param.block[ 2 ];
	}		
	///< focus�̐ݒ�
	CI_pv_FocusSet( wk, 1 );

	///< block ��������code�̍ő吔�����߂�
	for ( i = 0; i < CODE_BLOCK_MAX; i++ ){
		
		wk->code_max += wk->param.block[ i ];
	}
	
	///< table �쐬
	{
		int sw = 0;
		const x_tbl[][ CODE_BLOCK_MAX ] = {
			{ POS_4_4_4_p1, POS_4_4_4_p2, POS_4_4_4_p3 },
			{ POS_2_5_5_p1, POS_2_5_5_p2, POS_2_5_5_p3 },
		};
		if ( wk->param.block[ 0 ] != 4 ){
			sw = 1;
		}
		for ( i = 0; i < CODE_BLOCK_MAX; i++ ){
			
			wk->x_tbl[ i ] = x_tbl[ sw ][ i ];
		}
		wk->x_tbl[ i ] = x_tbl[ sw ][ i - 1 ];
	}
	
	///< code����ł�bar�̈ʒu
	{
		int pos = 0;			
		for ( i = 0; i < BAR_OAM_MAX; i++ ){
			
			pos += wk->param.block[ i ];
			wk->bar[ i ].state = pos - 1;
			OS_Printf( "bar pos = %2d\n", wk->bar[ i ].state );
		}
	}
	
	///< �O���[�v�̐ݒ�
	{
		int j;
		int bno = 0;
		i = 0;
		do {
			
			for ( j = 0; j < wk->param.block[ bno ]; j++ ){
				
				wk->code[ i ].group = bno + 1;
				OS_Printf( "group = %2d\n", wk->code[ i ].group );
				i++;
			}
			bno++;			
		} while ( i < wk->code_max );
	}
}


//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�؂�ւ�
 *
 * @param	wk	
 * @param	seq	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_SeqChange( CODEIN_WORK* wk, int seq )
{
	wk->seq			= seq;
	wk->wait		= 0;
	wk->gene_seq	= 0;
	wk->gene_cnt	= 0;
}

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL CI_pv_MainInit( CODEIN_WORK* wk )
{
	CI_pv_disp_CodeRes_Load( wk );	
	CI_pv_disp_SizeFlagSet( wk );	

	CI_pv_disp_CodeOAM_Create( wk );
	CI_pv_disp_MovePosSet( wk, eMPS_SET );
	
	CI_pv_disp_CurOAM_Create( wk );
	CI_pv_disp_BtnOAM_Create( wk );
	
	CI_pv_disp_HitTableSet( wk );
	
	
	CI_pv_FontOam_SysInit( wk );
	CI_pv_FontOam_Add( wk );
	
	CI_pv_disp_BMP_WindowAdd( wk->sys.bgl, &wk->sys.win, GF_BGL_FRAME0_S, 2, 21, 27, 2, 100 );
	
	
	CI_pv_SeqChange( wk, eSEQ_INPUT );
	
	
	WIPE_SYS_Start( WIPE_PATTERN_WMS,
				    WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN,
				    WIPE_FADE_BLACK,  WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_CODEIN );
				   
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�I��
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL CI_pv_MainEnd( CODEIN_WORK* wk )
{
	switch ( wk->gene_seq ){
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS,
			    WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT,
			    WIPE_FADE_BLACK,  WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_CODEIN );
		wk->gene_seq++;
		break;
		
	case 1:
		if ( WIPE_SYS_EndCheck() == TRUE ){
			
			wk->gene_seq++;
		}
		break;
		
	default:
	
		return TRUE;
	}
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	����
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL CI_pv_MainInput( CODEIN_WORK* wk )
{
	switch ( wk->gene_seq ){
	case 0:
		if ( WIPE_SYS_EndCheck() == TRUE ){
			
			wk->gene_seq++;
		}
		break;

	default:	
		CI_pv_StateUpdate( wk );	
		
		BMN_Main( wk->sys.btn );
		CI_KEY_Main( wk );
		break;
	}
	
	return FALSE;	
}

//--------------------------------------------------------------
/**
 * @brief	�t�H�[�J�X�̈ړ�
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL CI_pv_MainFocusMove( CODEIN_WORK* wk )
{
	int i;
	static f32 scale_l[] = {
		0.5f, 0.2f, 0.5f, 1.0f, 1.2f, 1.0f, 1.0f
	};
	static f32 scale_s[] = {
		0.8f, 0.6f, 0.4f, 0.2f, 0.8f, 1.0f, 1.0f
	};

	switch( wk->gene_seq ){
	///< �g��k���O��
	case 0:
		CI_pv_disp_CurOAM_Visible( wk, 0, FALSE );
		{
			///< �R�[�h�����̏���
			for ( i = 0; i < wk->code_max; i++ ){
				
				if ( wk->code[ i ].move_wk.wait == 0 ){ continue; }
				
				CATS_ObjectPosMoveCap( wk->code[ i ].cap, wk->code[ i ].move_wk.x, wk->code[ i ].move_wk.y );
				wk->code[ i ].move_wk.wait--;				
				
				
				if ( i >= wk->ls && i < wk->le ){
					CATS_ObjectScaleSetCap( wk->code[ i ].cap, scale_l[ wk->code[ i ].move_wk.scale ], scale_l[ wk->code[ i ].move_wk.scale ] );
					wk->code[ i ].move_wk.scale++;
				}
				
				if ( i >= wk->ss && i < wk->se ){
					CATS_ObjectScaleSetCap( wk->code[ i ].cap, scale_s[ wk->code[ i ].move_wk.scale ], scale_s[ wk->code[ i ].move_wk.scale ] );
					wk->code[ i ].move_wk.scale++;
				}
			}
			
			///< �o�[�����̏���
			for ( i = 0; i < BAR_OAM_MAX; i++ ){
				if ( wk->bar[ i ].move_wk.wait == 0 ){ continue; }
				CATS_ObjectPosMoveCap( wk->bar[ i ].cap, wk->bar[ i ].move_wk.x, wk->bar[ i ].move_wk.y );
				wk->bar[ i ].move_wk.wait--;
			}
			
			///< ����
			if ( wk->code[ 0 ].move_wk.wait == 0 ){
				for ( i = wk->ls; i < wk->le; i++ ){
					CATS_ObjectAnimeSeqSetCap( wk->code[ i ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ i ].state, wk->code[ i ].size ) );///< anime pat �� 3 �ɁB
					CATS_ObjectUpdateCap( wk->code[ i ].cap );
				}
				for ( i = wk->ss; i < wk->se; i++ ){
					CATS_ObjectAnimeSeqSetCap( wk->code[ i ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ i ].state, wk->code[ i ].size ) );///< anime pat �� 3 �ɁB
					CATS_ObjectUpdateCap( wk->code[ i ].cap );
				}
				wk->gene_seq++;
			}
			wk->gene_cnt++;
		}
		break;
		
	///< �g��k���㔼
	case 1:
		for ( i = wk->ls; i < wk->le; i++ ){
			if ( wk->code[ i ].move_wk.scale == SCALE_CNT_F ){ continue; }
			CATS_ObjectScaleSetCap( wk->code[ i ].cap, scale_l[ wk->code[ i ].move_wk.scale ], scale_l[ wk->code[ i ].move_wk.scale ] );
			wk->code[ i ].move_wk.scale++;
		}
		for ( i = wk->ss; i < wk->se; i++ ){
			if ( wk->code[ i ].move_wk.scale == SCALE_CNT_F ){ continue; }
			CATS_ObjectScaleSetCap( wk->code[ i ].cap, scale_s[ wk->code[ i ].move_wk.scale ], scale_s[ wk->code[ i ].move_wk.scale ] );
			wk->code[ i ].move_wk.scale++;
		}
		wk->gene_cnt++;
		if ( wk->gene_cnt == SCALE_CNT_F ){
			
			wk->gene_seq++;		
		}
		break;
	
	///< �I��
	default:
		CI_pv_disp_HitTableSet( wk );				///< �����蔻��̃e�[�u���X�V
		
		if ( wk->state.work == 0 ){
			CI_pv_disp_CurBar_PosSet( wk, CI_pv_FocusTopSerach( wk, wk->state.target ) );
		}
		else {
			CI_pv_disp_CurBar_PosSet( wk, CI_pv_FocusBottomSerach( wk, wk->state.target ) );
		}
		
		if ( wk->focus_now != 0 ){
			CI_pv_disp_CurOAM_Visible( wk, 0, TRUE );
		}
		
		CI_pv_StateInit( wk );		
		CI_pv_SeqChange( wk, eSEQ_INPUT );
		break;
	}
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�֐��e�[�u��
 *
 * @param	CI_pv_MainTable[]	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL (* const CI_pv_MainTable[])( CODEIN_WORK* wk ) = {
	CI_pv_MainInit,
	CI_pv_MainInput,
	CI_pv_MainFocusMove,
	CI_pv_MainEnd,
};


//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	wk	
 *
 * @retval	BOOL	TRUE = �I��
 *
 */
//--------------------------------------------------------------
BOOL CI_pv_MainUpdate( CODEIN_WORK* wk )
{
	BOOL bActive;
	
	bActive = CI_pv_MainTable[ wk->seq ]( wk );	
	
//	BMN_Main( wk->sys.btn );
//	CI_KEY_Main( wk );
	
	CI_pv_disp_CurUpdate( wk );
	CI_pv_disp_BtnUpdate( wk );
	CATS_Draw( wk->sys.crp );
	
	return bActive;
}


//--------------------------------------------------------------
/**
 * @brief	�L�[���̓��C��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_KEY_Main( CODEIN_WORK* wk )
{
	const int key_tbl[][ 5 ] = {
		{ 0, 1, 2, 3, 4 },
		{ 5, 6, 7, 8, 9 },
		{ 10,10,10,11,11 },
	};
	BOOL bMove = FALSE;
	
	int dat = key_tbl[ wk->cur[ 1 ].move_wk.y ][ wk->cur[ 1 ].move_wk.x ];
	
	if ( wk->sys.touch == TRUE ){
		if ( sys.trg ){			
			wk->sys.touch = FALSE;
			CI_pv_disp_CurSQ_PosSet( wk, dat );
			if ( dat == 10 || dat == 11 ){
				if ( wk->cur[ 1 ].state != eANM_CUR_SQ_L ){
					wk->cur[ 1 ].state = eANM_CUR_SQ_L;
				}
			}
			else {
				if ( wk->cur[ 1 ].state != eANM_CUR_SQ ){
					wk->cur[ 1 ].state = eANM_CUR_SQ;
				}
			}
			return;
		}
	}
	
	if ( sys.repeat & PAD_KEY_UP ){
		if ( wk->cur[ 1 ].move_wk.y > 0 ){
			wk->cur[ 1 ].move_wk.y--;
		}
		else {
			wk->cur[ 1 ].move_wk.y = 2;
		}
		bMove = TRUE;
		
//		Snd_SePlay( CI_SE_MOVE );
	}
	else if ( sys.repeat & PAD_KEY_DOWN ){
		wk->cur[ 1 ].move_wk.y++;
		wk->cur[ 1 ].move_wk.y %= 3;
		bMove = TRUE;

//		Snd_SePlay( CI_SE_MOVE );
	}
	else if ( sys.repeat & PAD_KEY_RIGHT ){
		
		if ( dat == 10 ){
			wk->cur[ 1 ].move_wk.x = 3;
		}
		else if ( dat == 11 ){
			wk->cur[ 1 ].move_wk.x = 0;			
		}
		else {				
			wk->cur[ 1 ].move_wk.x++;
			wk->cur[ 1 ].move_wk.x %= 5;
		}
		bMove = TRUE;

//		Snd_SePlay( CI_SE_MOVE );
	}
	else if ( sys.repeat & PAD_KEY_LEFT ){
		
		if ( dat == 10 ){
			wk->cur[ 1 ].move_wk.x = 3;
		}
		else if ( dat == 11 ){
			wk->cur[ 1 ].move_wk.x = 0;			
		}
		else {				
			if ( wk->cur[ 1 ].move_wk.x > 0 ){
				wk->cur[ 1 ].move_wk.x--;
			}
			else {
				wk->cur[ 1 ].move_wk.x = 4;
			}
		}
		bMove = TRUE;

//		Snd_SePlay( CI_SE_MOVE );
	}
	else if ( sys.trg & PAD_BUTTON_A ){
		///< A �{�^��������
		
		int cur_p;
		int now_g;
		int next_g;
		
		if ( dat == 10 ){
			///< Back �����ꂽ�ꍇ	
			CI_pv_Input_back( wk );

			Snd_SePlay( CI_SE_BACK );
		}		
		else if ( dat == 11 ){
			///< End �����ꂽ�ꍇ
			CI_pv_Input_End( wk );	

			Snd_SePlay( CI_SE_END );				
		}
		else {
			
			///< ���������ŉ����ꂽ�ꍇ
			
			if ( wk->focus_now == 0 ){ return; }
						
			cur_p = wk->cur[ 0 ].state;				
			wk->code[ cur_p ].state = dat + 1;
			CI_pv_disp_CurOAM_Visible( wk, 1, FALSE );
			CI_pv_disp_CurOAM_Visible( wk, 2, TRUE );
			CI_pv_disp_CurSQ_PosSetEx( wk, dat, 2 );
			CATS_ObjectAnimeSeqSetCap( wk->code[ cur_p ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ cur_p ].state, wk->code[ cur_p ].size ) );
			CATS_ObjectAnimeSeqSetCap( wk->cur[ 2 ].cap, eANM_CUR_TOUCH );
			
			now_g = wk->code[ cur_p ].group;
			cur_p++;
			if ( cur_p == wk->code_max ){
									
				wk->state.param	 = eSTATE_FOCUS_MOVE;
				wk->state.target = 0;				
				wk->cur[ 1 ].move_wk.x = 3;
				wk->cur[ 1 ].move_wk.y = 2;
				bMove = TRUE;
			}
			else {					
				next_g = wk->code[ cur_p ].group;
				
				if ( now_g != next_g ){
					
					wk->state.param	 = eSTATE_FOCUS_MOVE;
					wk->state.target = next_g;
				}
				else {
					
					wk->state.param	 = eSTATE_CUR_MOVE;
					wk->state.target = cur_p;
				}
				Snd_SePlay( CI_SE_TOUCH );
			}
		}
	}
	else if ( sys.trg & PAD_BUTTON_B ){
		///< �߂�ƈꏏ
		CI_pv_Input_back( wk );

		Snd_SePlay( CI_SE_TOUCH );
	}
	else if ( sys.repeat & PAD_BUTTON_L ){
		
		int button = wk->cur[ 0 ].state;
		
		if ( button == 0 ){
			wk->cur[ 0 ].state = wk->code_max - 1;
		}
		else {
			wk->cur[ 0 ].state--;
		}
		button = wk->cur[ 0 ].state;
		
		if ( wk->code[ button ].size == TRUE ){
			///< ���͕����̈ړ�
			
			wk->state.param		= eSTATE_CUR_MOVE;
			wk->state.target	= button;
		}
		else {
			///< �g�k�J�����g�ړ�����
			
			wk->state.param		= eSTATE_FOCUS_MOVE;
			wk->state.target	= wk->code[ button ].group;
			wk->state.work	 = 1;
		}
		
		Snd_SePlay( CI_SE_MOVE );
	}
	else if ( sys.repeat & PAD_BUTTON_R ){
		
		int button = wk->cur[ 0 ].state;
		
		if ( button == wk->code_max - 1 ){
			wk->cur[ 0 ].state = 0;
		}
		else {
			wk->cur[ 0 ].state++;
		}
		button = wk->cur[ 0 ].state;
		
		if ( wk->code[ button ].size == TRUE ){
			///< ���͕����̈ړ�
			
			wk->state.param		= eSTATE_CUR_MOVE;
			wk->state.target	= button;
		}
		else {
			///< �g�k�J�����g�ړ�����
			
			wk->state.param		= eSTATE_FOCUS_MOVE;
			wk->state.target	= wk->code[ button ].group;
		}
		
		Snd_SePlay( CI_SE_MOVE );
	}
	
	
	if ( bMove == TRUE ){
		Snd_SePlay( CI_SE_MOVE );

		dat = key_tbl[ wk->cur[ 1 ].move_wk.y ][ wk->cur[ 1 ].move_wk.x ];
		CI_pv_disp_CurSQ_PosSet( wk, dat );
		
		if ( dat == 10 || dat == 11 ){
			if ( wk->cur[ 1 ].state != eANM_CUR_SQ_L ){
				wk->cur[ 1 ].state = eANM_CUR_SQ_L;
			}
		}
		else {
			if ( wk->cur[ 1 ].state != eANM_CUR_SQ ){
				wk->cur[ 1 ].state = eANM_CUR_SQ;
			}
		}
	}
}


//--------------------------------------------------------------
/**
 * @brief	�I��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_Input_End( CODEIN_WORK* wk )
{
	int i;
	u32 num = 0;
	
	STRBUF* str_dest = STRBUF_Create( 100, HEAPID_CODEIN );
	
	wk->btn[ 1 ].state = 1;
	wk->btn[ 1 ].move_wk.wait = 0;
	
	for ( i = 0; i < wk->code_max; i++ ){
		
		if ( wk->code[ i ].state == 0 ){
			 wk->code[ i ].state = 1;
			 CATS_ObjectAnimeSeqSetCap( wk->code[ i ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ i ].state, wk->code[ i ].size ) );
		}
		
		num = wk->code[ i ].state - 1;		
		STRBUF_SetNumber( str_dest, num, 1, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT );
		STRBUF_AddStr( wk->param.strbuf, str_dest );
	}
	
	STRBUF_Delete( str_dest );
	
	CI_pv_SeqChange( wk, eSEQ_END );
}

//--------------------------------------------------------------
/**
 * @brief	�߂�
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_Input_back( CODEIN_WORK* wk )
{	///< �߂�
					
	int cur_p;
	int now_g;
	int next_g;
	
	wk->btn[ 0 ].state = 1;
	wk->btn[ 0 ].move_wk.wait = 0;
	
	if ( wk->focus_now == 0 ){
		cur_p = wk->cur[ 0 ].state = wk->code_max - 1;				
		next_g = wk->code[ cur_p ].group;
		wk->state.param	 = eSTATE_FOCUS_MOVE;
		wk->state.target = next_g;
		wk->state.work	 = 1;
		return;
	}
	
	cur_p = wk->cur[ 0 ].state;				
	wk->code[ cur_p ].state = eANM_CODE_LN;
	
	CATS_ObjectAnimeSeqSetCap( wk->code[ cur_p ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ cur_p ].state, wk->code[ cur_p ].size ) );
									
	now_g = wk->code[ cur_p ].group;
	if ( cur_p > 0 ){						 
		cur_p--;
		CATS_ObjectAnimeSeqSetCap( wk->code[ cur_p ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ cur_p ].state, wk->code[ cur_p ].size ) );
		
		next_g = wk->code[ cur_p ].group;
		
		if ( now_g != next_g ){
			
			wk->state.param	 = eSTATE_FOCUS_MOVE;
			wk->state.target = next_g;
			wk->state.work	 = 1;
		}
		else {
			
			wk->state.param	 = eSTATE_CUR_MOVE;
			wk->state.target = cur_p;
		}
	}
}



//--------------------------------------------------------------
/**
 * @brief	�{�^���p�}�l�[�W���[�ݒ�
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_ButtonManagerInit( CODEIN_WORK* wk )
{
	int i;
	
	///< �R�[�h�̓����蔻��
	for ( i = 0; i < eHRT_CODE_11 + 1; i ++ ){
				
		wk->code[ i ].hit = &wk->sys.rht[ i ];
	}
	
	///< �{�^�������̓����蔻��
	{
		///< x, y, sx, sy,
		const s16 tbl[][ 4 ] = {
			// 0 - 4
			{  32,  80, 20, 20 },{  80,  80, 20, 20 },{ 128,  80, 20, 20 },{ 176,  80, 20, 20 },{ 224,  80, 20, 20 },
			
			// 5 - 9
			{  32, 128, 20, 20 },{  80, 128, 20, 20 },{ 128, 128, 20, 20 },{ 176, 128, 20, 20 },{ 224, 128, 20, 20 },
			
			// back - end
			{  64, 176, 60, 12 },{ 192, 176, 60, 12 },
		};
		for ( ; i < eHRT_MAX; i++ ){
			
			wk->sys.rht[ i ].rect.top		= tbl[ i - eHRT_NUM_0 ][ 1 ] - tbl[ i - eHRT_NUM_0 ][ 3 ];
			wk->sys.rht[ i ].rect.left		= tbl[ i - eHRT_NUM_0 ][ 0 ] - tbl[ i - eHRT_NUM_0 ][ 2 ];
			wk->sys.rht[ i ].rect.bottom	= tbl[ i - eHRT_NUM_0 ][ 1 ] + tbl[ i - eHRT_NUM_0 ][ 3 ];
			wk->sys.rht[ i ].rect.right		= tbl[ i - eHRT_NUM_0 ][ 0 ] + tbl[ i - eHRT_NUM_0 ][ 2 ];
		}	
	}

	wk->sys.btn = BMN_Create( wk->sys.rht, eHRT_MAX, 
							  CI_pv_ButtonManagerCallBack, wk, HEAPID_CODEIN );
}


//--------------------------------------------------------------
/**
 * @brief	�{�^���p�}�l�[�W���[�R�[���o�b�N
 *
 * @param	button	
 * @param	event	
 * @param	work	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_ButtonManagerCallBack( u32 button, u32 event, void* work )
{
	CODEIN_WORK* wk = work;
	
	///< ���͂���Ȃ���Δ�΂�
	if ( wk->seq != eSEQ_INPUT ){ return; }
	
	///< Touch ���ᖳ����� Touch �ɂ��Ĕ�΂�
	if ( wk->sys.touch != TRUE ){
		 wk->sys.touch = TRUE;
	}
	
	if ( event == BMN_EVENT_TOUCH ){
		
		///< �������͕����������ꂽ
		if ( button >= eHRT_CODE_0 && button <= eHRT_CODE_11 ){
			
			OS_Printf( "button = %3d\n", button );
			
			if ( wk->code[ button ].size == TRUE ){
				///< ���͕����̈ړ�
				
				wk->state.param		= eSTATE_CUR_MOVE;
				wk->state.target	= button;
			}
			else {
				///< �g�k�J�����g�ړ�����
				
				wk->state.param		= eSTATE_FOCUS_MOVE;
				wk->state.target	= wk->code[ button ].group;
			}
			Snd_SePlay( CI_SE_BLOCK );
		}
		///< ���̑��̃{�^��
		else {
			
			///< �L�[�ʒu�ɂ��ϊ�
			if ( button == eHRT_BACK ){
				wk->cur[ 1 ].move_wk.x = 0;
				wk->cur[ 1 ].move_wk.y = 2;
				Snd_SePlay( CI_SE_BACK );
			}
			else if ( button == eHRT_END ){
				wk->cur[ 1 ].move_wk.x = 3;
				wk->cur[ 1 ].move_wk.y = 2;
				Snd_SePlay( CI_SE_END );
			}
			else {
				wk->cur[ 1 ].move_wk.x = ( button - eHRT_NUM_0 ) % 5;
				wk->cur[ 1 ].move_wk.y = ( button - eHRT_NUM_0 ) / 5;
				Snd_SePlay( CI_SE_TOUCH );
			}
			
			///< �����p�l��
			if ( button >= eHRT_NUM_0 && button <= eHRT_NUM_9 ){
				
				int cur_p;
				int now_g;
				int next_g;
				
				if ( wk->focus_now == 0 ){ return; }
				
				cur_p = wk->cur[ 0 ].state;				
				wk->code[ cur_p ].state = button - eHRT_NUM_0 + 1;
				
				CATS_ObjectAnimeSeqSetCap( wk->code[ cur_p ].cap, CI_pv_disp_CodeAnimeGet( wk->code[ cur_p ].state, wk->code[ cur_p ].size ) );
				
				///< �J�[�\��
				CI_pv_disp_CurOAM_Visible( wk, 1, TRUE );
				CI_pv_disp_CurSQ_PosSet( wk, button - eHRT_NUM_0 );
				CI_pv_disp_CurOAM_Visible( wk, 1, FALSE );
				CI_pv_disp_CurOAM_Visible( wk, 2, TRUE );
				CI_pv_disp_CurSQ_PosSetEx( wk, button - eHRT_NUM_0, 2 );
				CATS_ObjectAnimeSeqSetCap( wk->cur[ 2 ].cap, eANM_CUR_TOUCH );
							
				now_g = wk->code[ cur_p ].group;
				cur_p++;
				if ( cur_p == wk->code_max ){
										
					wk->state.param	 = eSTATE_FOCUS_MOVE;
					wk->state.target = 0;
					wk->state.work	 = 0;
				}
				else {					
					next_g = wk->code[ cur_p ].group;
					
					if ( now_g != next_g ){
						
						wk->state.param	 = eSTATE_FOCUS_MOVE;
						wk->state.target = next_g;
						wk->state.work	 = 0;
					}
					else {
						
						wk->state.param	 = eSTATE_CUR_MOVE;
						wk->state.target = cur_p;
					}
				}
			}
			else {
				///< back End
				
				if ( button == eHRT_BACK ){
					///< �߂�
					CI_pv_Input_back( wk );
				}
				else {
					///< ����
					CI_pv_Input_End( wk );					
				}
			}
		}
	}
}


//--------------------------------------------------------------
/**
 * @brief	��Ԃ̍X�V����
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_StateUpdate( CODEIN_WORK* wk )
{
	switch ( wk->state.param ){
	
	case eSTATE_INPUT:
		break;
	
	///< �t�H�[�J�X���ړ�����
	case eSTATE_FOCUS_MOVE:
		CI_pv_FocusSet( wk, wk->state.target );				///< ���̃t�H�[�J�X�ݒ�
		CI_pv_disp_SizeFlagSet( wk );						///< �T�C�Y�ݒ�
		CI_pv_disp_MovePosSet( wk, eMPS_MOVE_SET );			///< �ړ��n�ݒ�
		
		CI_pv_SeqChange( wk, eSEQ_FOCUS_MOVE );		
		wk->state.param	= eSTATE_BLANK;
		break;
	
	///< ���͈ʒu���ړ�����
	case eSTATE_CUR_MOVE:
		
		CI_pv_disp_CurBar_PosSet( wk, wk->state.target );	///< �J�[�\���ʒu��ύX
		CI_pv_StateInit( wk );
		break;
	
	case eSTATE_BLANK:
		break;
	}
}


//--------------------------------------------------------------
/**
 * @brief	��Ԃ̏�����
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CI_pv_StateInit( CODEIN_WORK* wk )
{	
	wk->state.param		= eSTATE_INPUT;
	wk->state.target	= 0;	
	wk->state.work	 	= 0;
}


//--------------------------------------------------------------
/**
 * @brief	�Ώۃt�H�[�J�X�̐擪�u���b�N���擾
 *
 * @param	wk	
 * @param	next	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int CI_pv_FocusTopSerach( CODEIN_WORK* wk, int next )
{
	int i;
	
	for ( i = 0; i < wk->code_max; i++ ){
		
		if ( wk->code[ i ].group == next ){
			
			return i;
		}
	}
	
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�Ώۃt�H�[�J�X�̍Ō���u���b�N���擾
 *
 * @param	wk	
 * @param	next	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int CI_pv_FocusBottomSerach( CODEIN_WORK* wk, int next )
{
	int i;
	int block = 0;
	int search = 0;
	
	for ( i = 0; i < wk->code_max; i++ ){
		
		if ( wk->code[ i ].group == next ){
			
			search = 1;
		}
		else {
			
			if ( search == 1 ){
				
				return i - 1;
			}
		}
	}
	
	return wk->code_max - 1;
}

