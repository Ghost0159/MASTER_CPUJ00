//==============================================================================
/**
 * @file	br_weekly_rank.c
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2008.01.08(��)
 *
 * �ɐF�X�ȉ�����������Ă��悢
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

#include "br_private.h"

/*
	���ʂ����C��
	����ʂ��T�u
	
	�ɓ���ւ��Ďg�p���Ă��܂��B
*/

typedef struct {	
		
	int				eva;
	int				evb;
	int				color;
	
	int				ranking_no;				///< �ǂ̃����L���O���Ă邩
	int				ranking_no_max;			///< �����L���O�̍ő�l�@�N���A�󋵂ɂ���ĕϓ�����	max = 2 or 3
	int				group_no;				///< �ǂ̃O���[�v���Ă邩
	int				group_no_max;			///< �O���[�v�̍ő�l�@�Œ�H
	int				ranking_mode;			///< �����L���O�@���T�@��T
	
	int				ranking_num;			///< �������邩
	
	GF_BGL_BMPWIN	win_m[ 6 ];
	GF_BGL_BMPWIN	win_s[ 2 ];
	OAM_BUTTON		obtn[ 2 ];				///< ����@�߂�@����͏�Ƀr�W�u��
	BUTTON_MAN*		btn_man;

	GT_LAST_WEEK_RANKING_ALL_RECV		last_week_data_dmy;
	GT_THIS_WEEK_RANKING_DATA_ALL_RECV	this_week_data_dmy;
	GT_RANKING_TYPE_RECV				ranking_type_dmy;
	
	GT_LAST_WEEK_RANKING_ALL_RECV*		last_week_data;
	GT_THIS_WEEK_RANKING_DATA_ALL_RECV*	this_week_data;	
	GT_RANKING_TYPE_RECV*				ranking_type;

	GT_RANKING_MYDATA_SEND				send_data;
	
	MSGDATA_MANAGER*					man_trtype;
	STRBUF*								unit_str;

	///< ���X�g�p
	TL_INFO				tl_info[ 3 ][ 20 ];	///< �ǁ[���Ă��ςɂȂ�̂�
	TOUCH_LIST_HEADER	tl_head;			///< �ǁ[���Ă��ςɂȂ�̂�

} RANK_WORK;

#define RANKING_NUM_THIS_WEEK	( 4 )
#define RANKING_NUM_LAST_WEEK	( 2 )

//==============================================================
// Prototype
//==============================================================
static void TL_HeaderMake( BR_WORK* wk );
static void TL_InfoMake( BR_WORK* wk );
static void Group_WinAdd( BR_WORK* wk );
static void Group_WinDel( BR_WORK* wk );
static void Ranking_WinTitleAdd( BR_WORK* wk );
static void Ranking_WinTitleDel( BR_WORK* wk );
static void Ranking_WinTitlePut( BR_WORK* wk );
static void ButtonOAM_PoeSet( BR_WORK* wk, int mode );
static void FontButton_Create( BR_WORK* wk );
static void FontButton_Delete( BR_WORK* wk );
static void RankingPageLimit_Init( RANK_WORK* rwk );
static void Btn_CallBack_01( u32 button, u32 event, void* work );
extern int Number_to_Unit_Get( u64 num );
extern void BR_Response_RankingType(void *work, const GDS_RAP_ERROR_INFO *error_info);
extern void BR_Response_RankingUpdate(void *work, const GDS_RAP_ERROR_INFO *error_info);



// -----------------------------------------
//
//	�����L���O
//
// -----------------------------------------
static BOOL BR_WeeklyRanking_Init( BR_WORK* wk );
static BOOL BR_WeeklyRanking_Setting( BR_WORK* wk );
static BOOL BR_WeeklyRanking_Send_Recv( BR_WORK* wk );
static BOOL BR_WeeklyRanking_RankChange( BR_WORK* wk );
static BOOL BR_WeeklyRanking_GroupChange( BR_WORK* wk );
static BOOL BR_WeeklyRanking_MainIn( BR_WORK* wk );
static BOOL BR_WeeklyRanking_Main( BR_WORK* wk );
static BOOL BR_WeeklyRanking_FadeOut( BR_WORK* wk );
static BOOL BR_WeeklyRanking_Exit( BR_WORK* wk );

#define REC_NUM	( 87 )

static const int Score_Unit_IndexTable[ REC_NUM ];

BOOL (* const BR_WeeklyRanking_MainTable[])( BR_WORK* wk ) = {
	BR_WeeklyRanking_Init,
	BR_WeeklyRanking_Setting,
	BR_WeeklyRanking_Send_Recv,
	BR_WeeklyRanking_RankChange,
	BR_WeeklyRanking_GroupChange,
	BR_WeeklyRanking_MainIn,
	BR_WeeklyRanking_Main,
	BR_WeeklyRanking_FadeOut,
	BR_WeeklyRanking_Exit,
};

static const int ranking_win_process[][ 4 ] = {
	{ 4,  3, 24, 4 },
	{ 4,  9, 24, 4 },
};

static const int ranking_win_result[][ 4 ] = {
	{ 4, 3, 24, 4 },
	{ 4, 9, 24, 4 },
};

static const int ranking_win_group[][ 4 ] = {
	{  7,  4, 18, 2 },
	{  7, 11, 18, 2 },
};

#define PRINT_COL_RANK	( GF_PRINTCOLOR_MAKE( 15, 13, 0 ) )

// =============================================================================
//
//
//	���^�b�`���X�g
//
//
// =============================================================================
static const TOUCH_LIST_HEADER tl_head_this_week = {
	NULL,
	12,					///< ��
	1,					///< 1�s��

	 4,15,
	24, 8,
	256,
	GF_BGL_FRAME2_M,

	4,					///< �\������
	NULL,				///< �����蔻��
};

static const TOUCH_LIST_HEADER tl_head_last_week = {
	NULL,
	12,					///< ��
	2,					///< 1�s��

	 4,15,
	24, 8,
	256,
	GF_BGL_FRAME2_M,

	2,					///< �\������
	NULL,				///< �����蔻��
};

//--------------------------------------------------------------
/**
 * @brief	�w�b�_�[make
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void TL_HeaderMake( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
		rwk->tl_head = tl_head_this_week;
		rwk->unit_str= NULL;
	}
	else {
		rwk->tl_head = tl_head_last_week;
		rwk->unit_str= MSGMAN_AllocString( wk->sys.man_ranking, Score_Unit_IndexTable[ rwk->last_week_data->ranking_data[ rwk->ranking_no ].ranking_type - 1 ] );
	}
	rwk->tl_head.data_max = rwk->ranking_num;		
	rwk->tl_head.info = rwk->tl_info[ rwk->ranking_no ];
//	rwk->tl_head.info = rwk->tl_info[ rwk->group_no ];

	///< �^�b�`���X�g������
	CATS_SystemActiveSet( wk, TRUE );
	TouchList_WorkClear( &wk->sys.touch_list );
//	TouchList_InitEx( &wk->sys.touch_list, wk, rwk->man_trtype, &rwk->tl_head );
	TouchList_InitRanking( &wk->sys.touch_list, wk, rwk->man_trtype, &rwk->tl_head, rwk->ranking_no, rwk->group_no, rwk->unit_str );
	Slider_AnimeCheck( &wk->sys.slider, &wk->sys.touch_list );			///< �A�j���`�F�b�N
	TouchList_CursorAdd( &wk->sys.touch_list, wk, CATS_D_AREA_MAIN );	///< ���X�g�̃J�[�\���o�^
	TouchList_CursorPosSet( &wk->sys.touch_list, 112, 184 );				///< �J�[�\���̈ʒu�ύX
	CATS_SystemActiveSet( wk, FALSE );
}

static void TL_InfoMake( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	int i;
	u64 param;
	
	rwk->last_week_data = &rwk->last_week_data_dmy;
	rwk->this_week_data = &rwk->this_week_data_dmy;

	switch ( rwk->group_no ){
	case 0:
		{
			for ( i = 0; i < GT_TRAINER_VIEW_MAX; i++ ){
				if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
					rwk->tl_info[ rwk->ranking_no ][ i ].msg		= TL_THIS_WEEK_TR;
					rwk->tl_info[ rwk->ranking_no ][ i ].param		= WiFi_TR_DataGet( rwk->this_week_data->ranking_data[ rwk->ranking_no ].trainer_type_ranking[ i ], 4 );
					rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= i;
				}
				else {
					rwk->tl_info[ rwk->ranking_no ][ i ].msg		= TL_LAST_WEEK_TR;
					rwk->tl_info[ rwk->ranking_no ][ i ].param		= WiFi_TR_DataGet( rwk->last_week_data->ranking_data[ rwk->ranking_no ].trainer_type_ranking[ i ], 4 );
					rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= rwk->last_week_data->ranking_data[ rwk->ranking_no ].trainer_type_score[ i ];
					#ifdef PM_DEBUG
					{
						if ( sys.cont & PAD_BUTTON_L
						&&	 sys.cont & PAD_KEY_DOWN ){
							rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= WEEKLY_RANK_DATA_MAX + 1;
						}
					}	
					#endif
					if ( rwk->tl_info[ rwk->ranking_no ][ i ].param_ex >= WEEKLY_RANK_DATA_MAX ){
						rwk->tl_info[ rwk->ranking_no ][ i ].param_ex = WEEKLY_RANK_DATA_MAX;
					}
				}
			}
		}
		break;

	case 1:
		{
			for ( i = 0; i < GT_YEAR_MONTH_NUM; i++ ){
				if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
					rwk->tl_info[ rwk->ranking_no ][ i ].msg		= TL_THIS_WEEK_YM;
					rwk->tl_info[ rwk->ranking_no ][ i ].param		= rwk->this_week_data->ranking_data[ rwk->ranking_no ].birthday_month_ranking[ i ];
					rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= i;
				}
				else {
					rwk->tl_info[ rwk->ranking_no ][ i ].msg		= TL_LAST_WEEK_YM;
					rwk->tl_info[ rwk->ranking_no ][ i ].param		= rwk->last_week_data->ranking_data[ rwk->ranking_no ].birthday_month_ranking[ i ];
					rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= rwk->last_week_data->ranking_data[ rwk->ranking_no ].birthday_month_score[ i ];
					#ifdef PM_DEBUG
					{
						if ( sys.cont & PAD_BUTTON_L
						&&	 sys.cont & PAD_KEY_DOWN ){
							rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= WEEKLY_RANK_DATA_MAX + 1;
						}
					}	
					#endif
					if ( rwk->tl_info[ rwk->ranking_no ][ i ].param_ex >= WEEKLY_RANK_DATA_MAX ){
						rwk->tl_info[ rwk->ranking_no ][ i ].param_ex = WEEKLY_RANK_DATA_MAX;
					}
				}
			}
		}
		break;

	case 2:
		{
			for ( i = 0; i < GT_MONSNO_RANKING_MAX; i++ ){
				if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
					rwk->tl_info[ rwk->ranking_no ][ i ].msg		= TL_THIS_WEEK_MN;
					rwk->tl_info[ rwk->ranking_no ][ i ].param		= rwk->this_week_data->ranking_data[ rwk->ranking_no ].monsno_ranking[ i ];
					rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= i;
				}
				else {
					rwk->tl_info[ rwk->ranking_no ][ i ].msg		= TL_LAST_WEEK_MN;
					rwk->tl_info[ rwk->ranking_no ][ i ].param		= rwk->last_week_data->ranking_data[ rwk->ranking_no ].monsno_ranking[ i ];
					rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= rwk->last_week_data->ranking_data[ rwk->ranking_no ].monsno_score[ i ];	
					#ifdef PM_DEBUG
					{
						if ( sys.cont & PAD_BUTTON_L
						&&	 sys.cont & PAD_KEY_DOWN ){
							rwk->tl_info[ rwk->ranking_no ][ i ].param_ex	= WEEKLY_RANK_DATA_MAX + 1;
						}
					}	
					#endif
					if ( rwk->tl_info[ rwk->ranking_no ][ i ].param_ex >= WEEKLY_RANK_DATA_MAX ){
						rwk->tl_info[ rwk->ranking_no ][ i ].param_ex = WEEKLY_RANK_DATA_MAX;
					}
				}
				
			}
		}
		break;
	}
}


static void Group_WinAdd( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	GF_BGL_BMPWIN* win;
	int ofs = 1;
	int i;
	int x;
	STRBUF* str1;
	
	for ( i = 0; i < 2; i++ ){		
		win = &rwk->win_s[ i ];		
		GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_S, ranking_win_group[ i ][ 0 ], 
															 ranking_win_group[ i ][ 1 ],
															 ranking_win_group[ i ][ 2 ],
															 ranking_win_group[ i ][ 3 ], eBG_PAL_FONT, ofs );
		ofs += ( ranking_win_group[ i ][ 2 ] * ranking_win_group[ i ][ 3 ] );		
		GF_BGL_BmpWinDataFill( win, 0x00 );		
		str1 = MSGMAN_AllocString( wk->sys.man, msg_502 + i );		
		x = BR_print_x_Get( win, str1 );		
		GF_STR_PrintColor( win, FONT_SYSTEM, str1, x, 0, MSG_NO_PUT, PRINT_COL_RANK, NULL );		
		GF_BGL_BmpWinOnVReq( win );		
		STRBUF_Delete( str1 );
	}
}

static void Group_WinDel( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	GF_BGL_BMPWIN* win;
	int i;
	
	for ( i = 0; i < 2; i++ ){
		GF_BGL_BmpWinOff( &rwk->win_s[ i ] );
		GF_BGL_BmpWinDel( &rwk->win_s[ i ] );
	}	
}



static void Ranking_WinTitleAdd( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	GF_BGL_BMPWIN* win;
	int ofs = 1;
	int i;
	int x;
	STRBUF* str1;
	int win_max;
	
	if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
		win_max = NELEMS( ranking_win_process );
	}
	else {
		win_max = NELEMS( ranking_win_result );
	}
	
	for ( i = 0; i < win_max; i++ ){
		win = &rwk->win_m[ i ];
		
		GF_BGL_BmpWinInit( win );
		
		if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
			GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_M, ranking_win_process[ i ][ 0 ], 
																 ranking_win_process[ i ][ 1 ],
																 ranking_win_process[ i ][ 2 ],
																 ranking_win_process[ i ][ 3 ], eBG_PAL_FONT, ofs );
			ofs += ( ranking_win_process[ i ][ 2 ] * ranking_win_process[ i ][ 3 ] );
		}
		else {
			GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_M, ranking_win_result[ i ][ 0 ], 
																 ranking_win_result[ i ][ 1 ],
																 ranking_win_result[ i ][ 2 ],
																 ranking_win_result[ i ][ 3 ], eBG_PAL_FONT, ofs );
			ofs += ( ranking_win_result[ i ][ 2 ] * ranking_win_result[ i ][ 3 ] );
		}
		
		GF_BGL_BmpWinDataFill( win, 0x00 );
		
		str1 = MSGMAN_AllocString( wk->sys.man, msg_409 );
		
		x	 = 0;//BR_print_x_Get( win, str1 );
		
		GF_STR_PrintColor( win, FONT_SYSTEM, str1, x, 0, MSG_NO_PUT, PRINT_COL_RANK, NULL );
		
		GF_BGL_BmpWinOnVReq( win );
		
		STRBUF_Delete( str1 );
	}
}

static void Ranking_WinTitleDel( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	GF_BGL_BMPWIN* win;
	int ofs = 1;
	int i;
	int x;
	STRBUF* str1;
	int win_max;
	
	if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
		win_max = NELEMS( ranking_win_process );
	}
	else {
		win_max = NELEMS( ranking_win_result );
	}

	for ( i = 0; i < win_max; i++ ){
		GF_BGL_BmpWinOff( &rwk->win_m[ i ] );
		GF_BGL_BmpWinDel( &rwk->win_m[ i ] );		
	}
}

///< �X�R�A�̒P�ʃe�[�u��
static const int Score_Unit_IndexTable[ REC_NUM ] = {
	msg_br_rank_500,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_500,
	msg_br_rank_507,
	msg_br_rank_501,
	msg_br_rank_502,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_503,
	msg_br_rank_503,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_507,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_504,
	msg_br_rank_503,
	msg_br_rank_503,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_501,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_501,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_500,
	msg_br_rank_505,
	msg_br_rank_500,
	msg_br_rank_506,
	msg_br_rank_506,
	msg_br_rank_506,
	msg_br_rank_500,
	msg_br_rank_501,
	msg_br_rank_501,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
	msg_br_rank_507,
};

#define RECID_DUMYY_DATA	( 0xFFFF )
static u32 RecordID_Table[ REC_NUM ] = {
	RECID_DENDOU_CNT,
	RECID_WAZA_HANERU,	
	RECID_WAZA_ZIBAKU,
	RECID_WAZA_DAIBAKUHATU,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_CONTEST_ENTRY,
	RECID_CONTEST_COMM_ENTRY,
	RECID_CONTEST_GPX,
	RECID_CONTEST_COMM_GPX,
	RECID_CONTEST_RIBBON_GET,
	RECID_WAZA_MUKOU,
	RECID_TEMOTI_KIZETU,
	RECID_TEMOTI_MAKIZOE,
	RECID_NIGERU_SIPPAI,
	RECID_NIGERARETA,
	RECID_FISHING_FAILURE,
	RECID_NUTS_PLANT,
	RECID_WALK_COUNT,
	RECID_BTL_ENCOUNT,
	RECID_BTL_TRAINER,
	RECID_CAPTURE_POKE,
	RECID_FISHING_SUCCESS,
	RECID_TAMAGO_HATCHING,
	RECID_POKE_EVOLUTION,
	RECID_SLOT_FEAVER,
	RECID_BTOWER_CHALLENGE,
	RECID_COMM_TRADE,
	RECID_COMM_BTL_WIN,
	RECID_COMM_BTL_LOSE,
	RECID_COMM_BTL_DRAW,
	RECID_WIFI_TRADE,
	RECID_WIFI_BTL_WIN,
	RECID_WIFI_BTL_LOSE,
	RECID_WIFI_BTL_DRAW,
	RECID_BTOWER_WIN,
	RECID_SHOPPING_MONEY,
	RECID_SODATEYA_CNT,
	RECID_KILL_POKE,
	RECID_GTS_PUT,
	RECID_WIFICLUB_TRADE,
	RECID_MYSIGN_WRITE,
	RECID_FOSSIL_RESTORE,
	RECID_FOOTMARK_LOOK,
	RECID_MAIL_WRITE,
	RECID_MITSU_ENC,
	RECID_LOBBY_CHAT,
	RECID_UG_BALL,
	RECID_TV_COUNT,
	RECID_NICKNAME,
	RECID_PREMIUM_BALL,
	RECID_FUREAI_ITEM,
	RECID_COOKING_SINGLE,
	RECID_COOKING_FRIENDS,
	RECID_DRESS_COUNT,
	RECID_KAIRIKI_COUNT,
	RECID_MARSH_COUNT,
	RECID_LEADERHOUSE_BATTLE,
	RECID_FRONTIER_COUNT,
	RECID_FRONTIER_BRAIN,
	RECID_FACTORY_WIN,
	RECID_CASTLE_WIN,
	RECID_STAGE_WIN,
	RECID_ROULETTE,
	RECID_FACTORY_POKE_CHANGE,
	RECID_CASTLE_POINT,
	RECID_STAGE_RANK10_WIN,
	RECID_ROULETTE_BP,
	RECID_WIN_BP,
	RECID_USE_BP,
	RECID_LOBBY_GAME_COUNT,
	RECID_GURUGURU_COUNT,
	RECID_CHANGE_HOSHINOKAKERA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA,
	RECID_DUMYY_DATA
};


//--------------------------------------------------------------
/**
 * @brief	���R�[�h�̒l�擾
 *
 * @param	wk	
 * @param	no	
 *
 * @retval	u32	
 *
 */
//--------------------------------------------------------------
static u32 Send_RECORD_Get( BR_WORK* wk, int no )
{
	u32 data;
	int index;
	
	index = no;
	
	if ( index >= REC_NUM ){
		OS_Printf( "���R�[�h�̒l���ł��� = %d\n", index );
		index = 0;
		GF_ASSERT( 0 );
	}
#if 0	
	{
		int i;
		for ( i = 0; i < REC_NUM; i++ ){
			OS_Printf( " rec id [ %2d ] = %5d\n", i, RecordID_Table[ i ] );
		}
	}
#endif	
	GF_ASSERT_MSG( RecordID_Table[ index ] != RECID_DUMYY_DATA, " rec id [ %d ] = %d\n", index, RecordID_Table[ index ] );
		
	data = RECORD_Get( SaveData_GetRecord( wk->save ), RecordID_Table[ index ] );
	
//	OS_Printf( "rec id = %d �� �l�� %d�@�ł�\n", index, data );
	
	return data;
}

static void Ranking_WinTitlePut( BR_WORK* wk )
{
	STRBUF*		str1;
	STRBUF*		str2;
	STRBUF*		name;
	STRBUF*		temp;
	WORDSET*	wset;
	RANK_WORK* rwk = wk->sub_work;
		
	temp = STRBUF_Create( 255, HEAPID_BR );
	wset = BR_WORDSET_Create( HEAPID_BR );
	
	///< �����L���O�^�C�g��
	GF_BGL_BmpWinDataFill( &rwk->win_m[ 0 ], 0x00 );
	
	if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
	//	rwk->this_week_data->ranking_data[ rwk->ranking_no ].ranking_type = 26 + rwk->ranking_no;
		str1 = MSGMAN_AllocString( wk->sys.man_ranking, rwk->this_week_data->ranking_data[ rwk->ranking_no ].ranking_type - 1 );
	}
	else {
	//	rwk->last_week_data->ranking_data[ rwk->ranking_no ].ranking_type = 45 + rwk->ranking_no;
		str1 = MSGMAN_AllocString( wk->sys.man_ranking, rwk->last_week_data->ranking_data[ rwk->ranking_no ].ranking_type - 1 );
	}

	GF_STR_PrintColor( &rwk->win_m[ 0 ], FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_RANK, NULL );
	GF_BGL_BmpWinOnVReq( &rwk->win_m[ 0 ] );
	STRBUF_Delete( str1 );
	
	///< ���e
	GF_BGL_BmpWinDataFill( &rwk->win_m[ 1 ], 0x00 );
	
	if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){
		str1 = MSGMAN_AllocString( wk->sys.man, msg_504 + rwk->group_no );
	}
	else {
		str1 = MSGMAN_AllocString( wk->sys.man, msg_513 );
	}
	GF_STR_PrintColor( &rwk->win_m[ 1 ], FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_RANK, NULL );
	GF_BGL_BmpWinOnVReq( &rwk->win_m[ 1 ] );
	STRBUF_Delete( str1 );
	
	name = GDS_Profile_CreateNameString( wk->br_gpp[ 0 ], HEAPID_BR );
	BR_ErrorStrChange( wk, name );
	
	///< �Q���O���[�v
	switch ( rwk->group_no ){
	case 0:
		{
			int view = GDS_Profile_GetTrainerView( wk->br_gpp[ 0 ] );
			str1 = MSGMAN_AllocString( wk->sys.man, msg_507 );
			str2 = MSGMAN_AllocString( rwk->man_trtype, WiFi_TR_DataGet( view, 4 ) );
		}
		break;
	case 1:
		{
			int month = GDS_Profile_GetMonthBirthday( wk->br_gpp[ 0 ] );
			str1 = MSGMAN_AllocString( wk->sys.man, msg_508 );
			str2 = STRBUF_Create( 255, HEAPID_BR );
			STRBUF_SetNumber( str2, month, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
		}
		break;
	case 2:
		{
			STRCODE code[ 255 ];
			int monsno = GDS_Profile_GetMonsNo( wk->br_gpp[ 0 ] );
			int egg	= GDS_Profile_GetEggFlag( wk->br_gpp[ 0 ] );
			str1 = MSGMAN_AllocString( wk->sys.man, msg_509 );
			if ( egg ){
				monsno = MONSNO_TAMAGO;	///< �^�}�S�Ȃ�^�}�S
			}
			if ( monsno ){
				str2 = STRBUF_Create( 255, HEAPID_BR );
				MSGDAT_MonsNameGet( monsno, HEAPID_BR, code );
				STRBUF_SetStringCode( str2, code );
			}
			else {
				str2 = MSGMAN_AllocString( wk->sys.man, msg_21 );
			}
		}
		break;
	}
	WORDSET_RegisterWord( wset, 0, str2, 0, TRUE, PM_LANG );
	WORDSET_RegisterWord( wset, 1, name, 0, TRUE, PM_LANG );
	WORDSET_ExpandStr( wset, temp, str1 );
	
	GF_STR_PrintColor( &rwk->win_m[ 1 ], FONT_SYSTEM, temp, 0, 16, MSG_NO_PUT, PRINT_COL_RANK, NULL );
	GF_BGL_BmpWinOnVReq( &rwk->win_m[ 1 ] );
	STRBUF_Delete( str1 );
	STRBUF_Delete( str2 );
	STRBUF_Delete( temp );
	STRBUF_Delete( name );
	WORDSET_ClearAllBuffer( wset );
	
	WORDSET_Delete( wset );
}


static void ButtonOAM_PoeSet( BR_WORK* wk, int mode )
{
	RANK_WORK* rwk = wk->sub_work;
	
	if ( mode == 0 ){
		///< 2��
		CATS_ObjectEnableCap( rwk->obtn[ 0 ].cap, TRUE );
		FONTOAM_SetDrawFlag( rwk->obtn[ 0 ].font_obj, TRUE );
		CATS_ObjectPosSetCap( rwk->obtn[ 1 ].cap, 128, 232 );
	}
	else {
		///< 1��
		CATS_ObjectEnableCap( rwk->obtn[ 0 ].cap, FALSE );
		FONTOAM_SetDrawFlag( rwk->obtn[ 0 ].font_obj, FALSE );
		CATS_ObjectPosSetCap( rwk->obtn[ 1 ].cap, 80, 232 );
	}
	FONTOAM_SetMat( rwk->obtn[ 0 ].font_obj, FONT_OAM_TAG_OX, FONT_OAM_TAG_OY );
	FONTOAM_SetMat( rwk->obtn[ 1 ].font_obj, FONT_OAM_TAG_OX, FONT_OAM_TAG_OY );
}


static void FontButton_Create( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;

	ExTag_ResourceLoad_Box( wk, NNS_G2D_VRAM_TYPE_2DSUB );		
	rwk->obtn[ 0 ].cap = ExTag_Add( wk, NNS_G2D_VRAM_TYPE_2DSUB );
	rwk->obtn[ 1 ].cap = ExTag_Add( wk, NNS_G2D_VRAM_TYPE_2DSUB );
	FontOam_Create( &rwk->obtn[ 0 ], &wk->sys, NNS_G2D_VRAM_TYPE_2DSUB );
	FontOam_Create( &rwk->obtn[ 1 ], &wk->sys, NNS_G2D_VRAM_TYPE_2DSUB );
	FontOam_MsgSet( &rwk->obtn[ 0 ], &wk->sys, msg_302 );
	FontOam_MsgSet( &rwk->obtn[ 1 ], &wk->sys, msg_05 );

	CATS_ObjectPosSetCap( rwk->obtn[ 0 ].cap,  32, 232 );
	CATS_ObjectPosSetCap( rwk->obtn[ 1 ].cap, 128, 232 );
	FONTOAM_SetMat( rwk->obtn[ 0 ].font_obj, FONT_OAM_TAG_OX, FONT_OAM_TAG_OY );
	FONTOAM_SetMat( rwk->obtn[ 1 ].font_obj, FONT_OAM_TAG_OX, FONT_OAM_TAG_OY );
	FONTOAM_SetDrawFlag( rwk->obtn[ 0 ].font_obj, FALSE );
	FONTOAM_SetDrawFlag( rwk->obtn[ 1 ].font_obj, TRUE );
	
	ButtonOAM_PoeSet( wk, 1 );	
}

static void FontButton_Delete( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	FontOam_Delete( &rwk->obtn[ 0 ] );
	FontOam_Delete( &rwk->obtn[ 1 ] );
	CATS_ActorPointerDelete_S( rwk->obtn[ 0 ].cap );
	CATS_ActorPointerDelete_S( rwk->obtn[ 1 ].cap );
	
	ExTag_ResourceDelete( wk );			
}


//--------------------------------------------------------------
/**
 * @brief	�e�����L���O�̏�����쐬
 *
 * @param	rwk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void RankingPageLimit_Init( RANK_WORK* rwk )
{
	int num[] = { GT_TRAINER_VIEW_MAX, GT_YEAR_MONTH_NUM, GT_MONSNO_RANKING_MAX };
	
	rwk->ranking_num = num[ rwk->group_no ];
}


static const RECT_HIT_TBL hit_table_01[] = {
	{ DTC(  3 ), DTC(  7 ), DTC( 6 ), DTC( 26 ) },	///< ��������Q�O
	{ DTC( 10 ), DTC( 14 ), DTC( 6 ), DTC( 26 ) },	///< �v���t�B�[��	
	{ 152, 184, 80, 176 },	///< �߂�
};

static void Btn_CallBack_01( u32 button, u32 event, void* work )
{
	BR_WORK*  wk = work;
	RANK_WORK* rwk = wk->sub_work;
	
	if ( event != BMN_EVENT_TOUCH ){ return; }

	switch ( button ){
	case 0:
		TouchAction( wk );
		rwk->ranking_no++;
		rwk->ranking_no %= rwk->ranking_no_max;			
		BR_Main_SeqChange( wk, eRANK_GroupChange );
		break;
	
	case 1:
		TouchAction( wk );
		rwk->group_no++;
		rwk->group_no %= rwk->group_no_max;		
		BR_Main_SeqChange( wk, eRANK_GroupChange );
		break;

	default:
		TouchAction( wk );
		BR_Main_SeqChange( wk, eRANK_Fade );
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_Init( BR_WORK* wk )
{
	RANK_WORK* rwk = sys_AllocMemory( HEAPID_BR, sizeof( RANK_WORK ) );
	
	memset( rwk, 0, sizeof( RANK_WORK ) );
	
	wk->sub_work = rwk;
	
	rwk->ranking_mode	= wk->ex_param1;
	rwk->ranking_no_max = GT_RANKING_WEEK_NUM;
	
	if ( BR_IsGameClear( wk ) == FALSE ){		///< �N���A���Ė����ƂP���炷
		rwk->ranking_no_max--;
	}	
	rwk->group_no_max	= 3;					///< trtype birth poke

	RankingPageLimit_Init( rwk );
	
	rwk->man_trtype = wk->sys.man;//MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_trtype_dat, HEAPID_BR );
	
	GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_M );
	GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME3_M );
	GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_S );
	GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME3_S );

	Plate_AlphaInit_Default( &rwk->eva, &rwk->evb, ePLANE_ALL );
	
	rwk->btn_man = BMN_Create( hit_table_01, NELEMS( hit_table_01 ), Btn_CallBack_01, wk, HEAPID_BR );

	BR_Main_SeqChange( wk, eRANK_Setting );

	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�Z�b�e�B���O
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_Setting( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	switch ( wk->sub_seq ){
	case 0:
		if ( BR_PaletteFade( &rwk->color, eFADE_MODE_OUT ) ){ wk->sub_seq++; }	
		ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_OBJ, TAG_PALETTE, 			 rwk->color, wk->sys.logo_color );	///< main	oam
		ColorConceChangePfd( wk->sys.pfd, FADE_SUB_OBJ,	 LINE_OTHER_SUB_PALETTE, rwk->color, wk->sys.logo_color );	///< sub	oam
		ColorConceChangePfd( wk->sys.pfd, FADE_SUB_BG,	 LOGO_PALETTE,			 rwk->color, wk->sys.logo_color );	///< sub	bg
		break;
	
	case 1:
		CATS_SystemActiveSet( wk, TRUE );
		NormalTag_DeleteAllOp( wk );						///< tag delete		
		FontButton_Create( wk );
		CATS_SystemActiveSet( wk, FALSE );
		
		GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 1 );			///< priority
		GF_BGL_PrioritySet( GF_BGL_FRAME1_M, 3 );
		GF_BGL_PrioritySet( GF_BGL_FRAME2_M, 0 );
		GF_BGL_PrioritySet( GF_BGL_FRAME3_M, 1 );
		
		GF_BGL_PrioritySet( GF_BGL_FRAME0_S, 1 );
		GF_BGL_PrioritySet( GF_BGL_FRAME1_S, 3 );
		GF_BGL_PrioritySet( GF_BGL_FRAME2_S, 0 );
		GF_BGL_PrioritySet( GF_BGL_FRAME3_S, 2 );
		
		BR_ChangeDisplayVIntr( wk, DISP_3D_TO_MAIN );				///< main = top
		Cursor_Visible( wk->cur_wk, FALSE );
		wk->sub_seq++;
		break;

	default:
		if ( BR_IsChangeDisplay( wk ) == FALSE ){ break; }
		BR_Main_SeqChange( wk, eRANK_SendRecv );
		break;
	}
	
	return FALSE;
}

//#define SKIP_CHECK	( sys.trg & PAD_BUTTON_A )
#define SKIP_CHECK	( 1 )
static BOOL BR_WeeklyRanking_Send_Recv( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	GDS_RAP_ERROR_INFO *error_info;
		
	if ( BR_GDSRAP_MoveStatusAllCheck( wk ) == FALSE ){ return FALSE; }
	
	switch ( wk->sub_seq ){
	case 0:
		///< ������
		HormingCursor_Init( wk, wk->cur_wk, COMM_HORMING_CUR_CX, COMM_HORMING_CUR_CY );
		Cursor_Visible( wk->cur_wk, TRUE );
		Cursor_R_Set( wk->cur_wk, COMM_HORMING_R, COMM_HORMING_R );
		Tool_InfoMessageMain( wk, msg_info_010 );
		Snd_SePlay( eBR_SND_SEARCH );
		wk->sub_seq++;
		break;
		
	case 1:
		///< ���݊J�Ò��̃f�[�^�̎擾	
		if( GDSRAP_Tool_Send_RankingTypeDownload( BR_GDSRAP_WorkGet( wk ) ) == TRUE ){
			wk->sub_seq++;
		}
		break;
	
	case 2:
		if( GDSRAP_ErrorInfoGet( BR_GDSRAP_WorkGet( wk ), &error_info ) == TRUE ){
			//�G���[�������̏���
			Snd_SeStopBySeqNo( eBR_SND_SEARCH, 0 );
			Tool_InfoMessageMainDel( wk );
			BR_ErrorSet( wk, eERR_RANKING_TYPE_RECV, error_info->result, error_info->type );
			Cursor_Visible( wk->cur_wk, FALSE );
			wk->sub_seq = 5;
		}
		else{
			{
				///< �����̃f�[�^�̑��M
				rwk->ranking_type = &rwk->ranking_type_dmy;

			//	GDS_CONV_GDSProfile_to_GTRankingProfile( wk->save, wk->br_gpp[ 0 ], &rwk->send_data.profile );

				rwk->send_data.playtime.hour   = PLAYTIME_GetHour( SaveData_GetPlayTime( wk->save ) );
				rwk->send_data.playtime.minute = PLAYTIME_GetMinute( SaveData_GetPlayTime( wk->save ) );
				rwk->send_data.playtime.second = PLAYTIME_GetSecond( SaveData_GetPlayTime( wk->save ) );

				rwk->send_data.my_data[ 0 ].ranking_type = rwk->ranking_type->ranking_type[ 0 ];
				rwk->send_data.my_data[ 1 ].ranking_type = rwk->ranking_type->ranking_type[ 1 ];
				rwk->send_data.my_data[ 2 ].ranking_type = rwk->ranking_type->ranking_type[ 2 ];

				rwk->send_data.my_data[ 0 ].score = Send_RECORD_Get( wk, rwk->send_data.my_data[ 0 ].ranking_type - 1 );
				OS_Printf( "--- rank type = %d, score = %d\n", rwk->send_data.my_data[ 0 ].ranking_type - 1, rwk->send_data.my_data[ 0 ].score );
				rwk->send_data.my_data[ 1 ].score = Send_RECORD_Get( wk, rwk->send_data.my_data[ 1 ].ranking_type - 1 );
				OS_Printf( "--- rank type = %d, score = %d\n", rwk->send_data.my_data[ 1 ].ranking_type - 1, rwk->send_data.my_data[ 1 ].score );
				rwk->send_data.my_data[ 2 ].score = Send_RECORD_Get( wk, rwk->send_data.my_data[ 2 ].ranking_type - 1 );
				OS_Printf( "--- rank type = %d, score = %d\n", rwk->send_data.my_data[ 2 ].ranking_type - 1, rwk->send_data.my_data[ 2 ].score );
			}
			Snd_SeStopBySeqNo( eBR_SND_SEARCH, 0 );
			wk->sub_seq++;
		}
		break;
	
	case 3:
		if( GDSRAP_Tool_Send_RankingUpdate(	BR_GDSRAP_WorkGet( wk ), wk->br_gpp[ 0 ], &rwk->send_data.my_data[ 0 ] ) == TRUE ){
			wk->sub_seq++;
		}
		break;
	
	case 4:
		Tool_InfoMessageMainDel( wk );
		if( GDSRAP_ErrorInfoGet( BR_GDSRAP_WorkGet( wk ), &error_info ) == TRUE){
			//�G���[�������̏���
			Snd_SeStopBySeqNo( eBR_SND_SEARCH, 0 );
			BR_ErrorSet( wk, eERR_RANKING_SEND, error_info->result, error_info->type );
			Cursor_Visible( wk->cur_wk, FALSE );
			wk->sub_seq = 5;
		}
		else{
			BOOL err = FALSE;
			
			if ( rwk->ranking_mode == BR_EX_DATA_THIS_WEEK_RANK ){	
				if ( rwk->this_week_data->ranking_data[ 0 ].ranking_type == 0
				||	 rwk->this_week_data->ranking_data[ 1 ].ranking_type == 0
				||	 rwk->this_week_data->ranking_data[ 2 ].ranking_type == 0 ){
					err = TRUE;
				}
			}
			else {				
				if ( rwk->last_week_data->ranking_data[ 0 ].ranking_type == 0
				||	 rwk->last_week_data->ranking_data[ 1 ].ranking_type == 0
				||	 rwk->last_week_data->ranking_data[ 2 ].ranking_type == 0 ){
					err = TRUE;
				}
			}
						
			if ( err ){
				BR_ErrorSet( wk, eERR_RANKING_SEND, 0, 0 );
				wk->sys.comm_err_wk.err_msg_id = msg_info_031;
				Cursor_Visible( wk->cur_wk, FALSE );
				wk->sub_seq = 5;
			}
			else {
				Snd_SeStopBySeqNo( eBR_SND_SEARCH, 0 );
				Snd_SePlay( eBR_SND_RECV_OK );
				wk->sub_seq = 0xFF;
			}
		}
		break;
	
	case 5:
		///< �G���[���̏I������
		CATS_SystemActiveSet( wk, TRUE );
		FontButton_Delete( wk );
//		MSGMAN_Delete( rwk->man_trtype );
		BMN_Delete( rwk->btn_man );
		BR_PaletteFadeIn_Init( &rwk->color );
		NormalTag_RecoverAllOp( wk );
		CATS_SystemActiveSet( wk, FALSE );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_M );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_S );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME3_M );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME3_S );
		BR_ChangeDisplayVIntr( wk, DISP_3D_TO_SUB );				///< main = top
		wk->sub_seq++;
		break;
	
	case 6:
		if ( BR_IsChangeDisplay( wk ) == FALSE ){ break; }
		if ( BR_PaletteFade( &rwk->color, eFADE_MODE_IN ) ){					
			PaletteFadeClear( wk );
			BR_PaletteFadeIn_Init( &rwk->color );
			ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_OBJ, TAG_PALETTE, 16, wk->sys.logo_color );	///< main	oam
			
			BR_Main_ProcSeqChange( wk, eMAIN_Input, TRUE, wk->proc_seq );
			BR_Main_SeqChange( wk, eINPUT_MENU_IN );		
			sys_FreeMemoryEz( rwk );
		}
		else {
			ColorConceChangePfd( wk->sys.pfd, FADE_SUB_BG,	 LOGO_PALETTE, rwk->color, wk->sys.logo_color );	///< main	oam
			ColorConceChangePfd( wk->sys.pfd, FADE_SUB_OBJ, LINE_OTHER_SUB_PALETTE, rwk->color, wk->sys.logo_color );	///< main	oam
		}
		break;

	default:
//		if ( !GF_TP_GetTrg() ){ break; }		
		///< ���X�g�p�̃f�[�^���쐬
		TL_InfoMake( wk );	///< �����L���O��M�f�[�^�R�s�[
//		Tool_InfoMessageMainDel( wk );
		Cursor_Visible( wk->cur_wk, FALSE );
		Cursor_R_Set( wk->cur_wk, 0, 0 );
		BR_Main_SeqChange( wk, eRANK_MainIn );
		break;
	}
	
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	���C�� �C��
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_MainIn( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	switch ( wk->sub_seq ){
	case 0:
		ArcUtil_HDL_BgCharSet( wk->sys.p_handle, NARC_batt_rec_gra_batt_rec_data_NCGR,   wk->sys.bgl, GF_BGL_FRAME3_M, 0, 0, 0, HEAPID_BR );
		ArcUtil_HDL_BgCharSet( wk->sys.p_handle, NARC_batt_rec_gra_batt_rec_data_NCGR,   wk->sys.bgl, GF_BGL_FRAME3_S, 0, 0, 0, HEAPID_BR );
		ArcUtil_HDL_ScrnSet( wk->sys.p_handle,	 NARC_batt_rec_gra_batt_rec_rank_u_NSCR, wk->sys.bgl, GF_BGL_FRAME3_M, 0, 0, 0, HEAPID_BR );
		ArcUtil_HDL_ScrnSet( wk->sys.p_handle,	 NARC_batt_rec_gra_batt_rec_rank_d_NSCR, wk->sys.bgl, GF_BGL_FRAME3_S, 0, 0, 0, HEAPID_BR );
		
		Ranking_WinTitleAdd( wk );
		Ranking_WinTitlePut( wk );
		CATS_SystemActiveSet( wk, TRUE );
		Slider_InitEx( &wk->sys.slider, wk, eTAG_EX_BOX_SLIDER );			///< �g���܂킷
		TL_HeaderMake( wk );												///< �������A�ݒ�Ȃ�
		CATS_SystemActiveSet( wk, FALSE );
		Group_WinAdd( wk );
		
		GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG3, VISIBLE_ON );
		GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );
		wk->sub_seq++;
		break;
	
	case 1:
		///< alpha
		BR_PaletteFade( &rwk->color, eFADE_MODE_IN );
		if ( Plate_AlphaFade( &rwk->eva, &rwk ->evb, eFADE_MODE_IN, ePLANE_ALL ) ){							
			GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_ON );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
			wk->sub_seq++;
		}
		ColorConceChangePfd( wk->sys.pfd, FADE_SUB_OBJ,	LINE_OTHER_SUB_PALETTE, rwk->color, wk->sys.logo_color );
		ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_OBJ, TAG_PALETTE, rwk->color, wk->sys.logo_color );		///< �㉺�J�[�\�����^�O�p���b�g�Ȃ̂�
		break;
		
	default:
		BR_Main_SeqChange( wk, eRANK_Main );
		break;
	}
	
	return FALSE;
}

static BOOL BR_WeeklyRanking_Main( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	Slider_HitCheck_Main( &wk->sys.slider );
	TouchList_ValueSet( &wk->sys.touch_list, wk->sys.slider.value );
		
	TouchList_Ranking_Main( &wk->sys.touch_list, wk, rwk->ranking_no, rwk->group_no, rwk->unit_str );

	BMN_Main( rwk->btn_man );
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�t�F�[�h
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_FadeOut( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	switch ( wk->sub_seq ){
	case 0:	
		CATS_SystemActiveSet( wk, TRUE );
		Slider_Exit( &wk->sys.slider );					///< �X���C�_�[�̔j��
		TouchList_Exit( &wk->sys.touch_list );			///< ���X�g�̔j��
		if ( rwk->unit_str ){
			STRBUF_Delete( rwk->unit_str );
		}
		TouchList_CursorDel( &wk->sys.touch_list, wk );	///< ���X�g�̃J�[�\���j��	
		CATS_SystemActiveSet( wk, FALSE );
		Cursor_Visible( wk->cur_wk_top, FALSE );
		GF_Disp_GXS_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_OFF );
		GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_OFF );
		wk->sub_seq++;
		break;
	
	case 1:
		BR_PaletteFade( &rwk->color, eFADE_MODE_OUT );
		if ( Plate_AlphaFade( &rwk->eva, &rwk->evb, eFADE_MODE_OUT, ePLANE_ALL ) ){
			wk->sub_seq++;
		}
		ColorConceChangePfd( wk->sys.pfd, FADE_SUB_OBJ,	LINE_OTHER_SUB_PALETTE, rwk->color, wk->sys.logo_color );
		ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_OBJ, TAG_PALETTE, rwk->color, wk->sys.logo_color );		///< �㉺�J�[�\�����^�O�p���b�g�Ȃ̂�
		break;

	default:
		BR_Main_SeqChange( wk, eRANK_Exit );
		break;
	}
	
	return FALSE;	
}


//--------------------------------------------------------------
/**
 * @brief	�I��
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_Exit( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	switch ( wk->sub_seq ){
	case 0:		
		Group_WinDel( wk );
		Ranking_WinTitleDel( wk );
		CATS_SystemActiveSet( wk, TRUE );
		FontButton_Delete( wk );
		CATS_SystemActiveSet( wk, FALSE );
//		MSGMAN_Delete( rwk->man_trtype );
		BMN_Delete( rwk->btn_man );
		wk->sub_seq++;
		break;
		
	case 1:
		CATS_SystemActiveSet( wk, TRUE );
		BR_PaletteFadeIn_Init( &rwk->color );
		NormalTag_RecoverAllOp( wk );
		CATS_SystemActiveSet( wk, FALSE );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_M );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_S );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME3_M );
		GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME3_S );
		BR_ChangeDisplayVIntr( wk, DISP_3D_TO_SUB );				///< main = top
		wk->sub_seq++;
		break;
	
	default:
		if ( BR_IsChangeDisplay( wk ) == FALSE ){ break; }
		if ( BR_PaletteFade( &rwk->color, eFADE_MODE_IN ) ){					
			PaletteFadeClear( wk );
			BR_PaletteFadeIn_Init( &rwk->color );
			ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_OBJ, TAG_PALETTE, 16, wk->sys.logo_color );	///< main	oam
			
			BR_Main_ProcSeqChange( wk, eMAIN_Input, TRUE, wk->proc_seq );
			BR_Main_SeqChange( wk, eINPUT_MENU_IN );		
			sys_FreeMemoryEz( rwk );
		}
		else {
			ColorConceChangePfd( wk->sys.pfd, FADE_SUB_BG,	 LOGO_PALETTE, rwk->color, wk->sys.logo_color );	///< main	oam
			ColorConceChangePfd( wk->sys.pfd, FADE_SUB_OBJ, LINE_OTHER_SUB_PALETTE, rwk->color, wk->sys.logo_color );	///< main	oam
		}
		break;
	}
	
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�����L���O�����؂�ւ�
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_RankChange( BR_WORK* wk )
{
	GF_ASSERT( 0 );
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�O���[�v�����؂�ւ�
 *
 * @param	wk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL BR_WeeklyRanking_GroupChange( BR_WORK* wk )
{
	RANK_WORK* rwk = wk->sub_work;
	
	switch ( wk->sub_seq ){
	case 0:
		TL_InfoMake( wk );
		RankingPageLimit_Init( rwk );
		Ranking_WinTitleDel( wk );
		TouchList_Exit( &wk->sys.touch_list );			///< ���X�g�̔j��
		if ( rwk->unit_str ){
			STRBUF_Delete( rwk->unit_str );
		}
		TouchList_CursorDel( &wk->sys.touch_list, wk );	///< ���X�g�̃J�[�\���j��
		wk->sub_seq++;
		break;
	
	case 1:
		if ( Plate_AlphaFade( &rwk->eva, &rwk->evb, eFADE_MODE_OUT, ePLANE_MAIN ) ){			
			wk->sub_seq++;
		}
		break;
	
	case 2:
		if ( Plate_AlphaFade( &rwk->eva, &rwk->evb, eFADE_MODE_IN, ePLANE_MAIN ) ){
			Ranking_WinTitleAdd( wk );
			Ranking_WinTitlePut( wk );
			TL_HeaderMake( wk );
			wk->sub_seq++;
		}
		break;

	default:		
		BR_Main_SeqChange( wk, eRANK_Main );
		break;
	}
	
	return FALSE;
}



//--------------------------------------------------------------
/**
 * @brief   �J�Ò��̃����L���O�^�C�v�_�E�����[�h���̃��X�|���X�R�[���o�b�N
 *
 * @param   work			
 * @param   error_info		�G���[���
 */
//--------------------------------------------------------------
void BR_Response_RankingType(void *work, const GDS_RAP_ERROR_INFO *error_info)
{
	BR_WORK* wk = work;
	RANK_WORK* rwk = wk->sub_work;
	
	OS_TPrintf("�J�Ò��̃����L���O�^�C�v�̃_�E�����[�h���X�|���X�擾\n");
	if(error_info->occ == TRUE){
		//TRUE�Ȃ�΃G���[�������Ă���̂ŁA�����Ń��j���[��߂��Ƃ��A�v���I�����[�h�ֈڍs�Ƃ�����
		rwk->ranking_type = &rwk->ranking_type_dmy;
	}
	else{
		//���펞�Ȃ�Ύ�M�o�b�t�@����f�[�^�擾�Ȃǂ��s��
		GT_RANKING_TYPE_RECV *recv_type;
		int i;		
		GDS_RAP_RESPONSE_RankingType_Download_RecvPtr_Set( BR_GDSRAP_WorkGet( wk ), &rwk->ranking_type );
		MI_CpuCopy8( rwk->ranking_type, &rwk->ranking_type_dmy, sizeof( GT_RANKING_TYPE_RECV ) );
	}
}

//--------------------------------------------------------------
/**
 * @brief   �����L���O�X�V���̃��X�|���X�R�[���o�b�N
 *
 * @param   work			
 * @param   error_info		�G���[���
 */
//--------------------------------------------------------------
void BR_Response_RankingUpdate(void *work, const GDS_RAP_ERROR_INFO *error_info)
{
	BR_WORK* wk = work;
	RANK_WORK* rwk = wk->sub_work;
	
	OS_TPrintf("�����L���O�X�V�̃��X�|���X�擾\n");
	if(error_info->occ == TRUE){
		//TRUE�Ȃ�΃G���[�������Ă���̂ŁA�����Ń��j���[��߂��Ƃ��A�v���I�����[�h�ֈڍs�Ƃ�����
		rwk->last_week_data = &rwk->last_week_data_dmy;
		rwk->this_week_data = &rwk->this_week_data_dmy;
	}
	else{
		//���펞�Ȃ�Ύ�M�o�b�t�@����f�[�^�擾�Ȃǂ��s��
//		GT_LAST_WEEK_RANKING_ALL_RECV *last_week;
//		GT_THIS_WEEK_RANKING_DATA_ALL_RECV *this_week;
		GDS_RAP_RESPONSE_RankingUpdate_Download_RecvPtr_Set( BR_GDSRAP_WorkGet( wk ), &rwk->last_week_data, &rwk->this_week_data );
		MI_CpuCopy8( rwk->last_week_data, &rwk->last_week_data_dmy, sizeof( GT_LAST_WEEK_RANKING_ALL_RECV ) );
		MI_CpuCopy8( rwk->this_week_data, &rwk->this_week_data_dmy, sizeof( GT_THIS_WEEK_RANKING_DATA_ALL_RECV ) );
		
		
		{
			int i;
			for( i = 0; i < GT_RANKING_WEEK_NUM; i++ ){
				OS_TPrintf("���T�̃^�C�v ����%d = %d\n", i, rwk->this_week_data->ranking_data[ i ].ranking_type );
				OS_TPrintf("��T�̃^�C�v ����%d = %d\n", i, rwk->last_week_data->ranking_data[ i ].ranking_type );
			}
		}
//		{
//			int i;			
//			for ( i = 0; i < 10; i++ ){
//				OS_Printf( "��T = %d\n", rwk->last_week_data->ranking_data[ 0 ].monsno_ranking[ i ] );
//				OS_Printf( "��T = %d\n", rwk->last_week_data->ranking_data[ 0 ].birthday_month_ranking[ i ] );
//				OS_Printf( "��T = %d\n", rwk->last_week_data->ranking_data[ 0 ].trainer_type_ranking[ i ] );
//			}
//		}
	#if 0
		for(i = 0; i < GT_RANKING_WEEK_NUM; i++){
			OS_TPrintf("��T�̃����L���O�^�C�v ����%d=%d", i, last_week->ranking_data[i].ranking_type);
		}
		for(i = 0; i < GT_RANKING_WEEK_NUM; i++){
			OS_TPrintf("���T�̃����L���O�^�C�v ����%d=%d", i, this_week->ranking_data[i].ranking_type);
		}
	#endif
	}
}
