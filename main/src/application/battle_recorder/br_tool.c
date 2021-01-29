//==============================================================================
/**
 * @file	br_tool.c
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2008.03.04(��)
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
#include "msgdata/msg.naix"
#include "system/brightness.h"
#include "system/fontproc.h"
#include "system/lib_pack.h"
#include "gflib/msg_print.h"
#include "gflib/touchpanel.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeicon.h"
#include "poketool/boxdata.h"
#include "system/snd_tool.h"
#include "gflib/strbuf_family.h"
#include "system/pm_overlay.h"

#include "br_private.h"

#include "field/situation_local.h"
#include "savedata/misc.h"

#include "gflib/strcode.h"
#include "gflib\msg_print.h"
#include "system\fontproc.h"
#include "system\msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_battle_rec.h"


// =============================================================================
//
//
//	���f�o�b�O�p
//
//
// =============================================================================
#ifdef PM_DEBUG
#ifdef BR_AUTO_SERCH
	int g_debug_auto = 0;
	int	g_debug_wait = 0;
	u64 g_debug_count = 0;
#endif
#endif


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void ErrorNameSet(STRBUF *dest_str, int heap_id);


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	heapID	
 *
 * @retval	WORDSET*	
 *
 */
//--------------------------------------------------------------
WORDSET* BR_WORDSET_Create( u32 heapID )
{
	//WORDSET_DEFAULT_SETNUM = 8
	WORDSET* wset = WORDSET_CreateEx( 4, WORDSET_COUNTRY_BUFLEN, heapID );	///< �ő��3���[�h����������4�ŁB
	
	return wset;
}

// =============================================================================
//
//
//	���J���[�ύX
//
//
// =============================================================================
int BR_FadeColorCode_Get( BR_WORK* wk )
{
	u32 bg_color_table[] = {
		LOGO_COLOR_B,
		LOGO_COLOR_B2,
		LOGO_COLOR_B3,
		LOGO_COLOR_B4,
		LOGO_COLOR_B5,
		LOGO_COLOR_B6,
		LOGO_COLOR_B7,
//		LOGO_COLOR_B8,
//		LOGO_COLOR_B9,
	};
	
	return 	bg_color_table[ wk->sys.color_type ];
}

int BR_FadeColorCode_Get_BootLine( BR_WORK* wk )
{
	u32 bg_color_table[] = {
		LINE_COLOR_B,
		LINE_COLOR_B2,
		LINE_COLOR_B3,
		LINE_COLOR_B4,
		LINE_COLOR_B5,
		LINE_COLOR_B6,
		LINE_COLOR_B7,
//		LINE_COLOR_B8,
//		LINE_COLOR_B9,
	};
	
	if ( wk->mode != BR_MODE_BROWSE ){
		return LINE_COLOR_G;
	}
	return 	bg_color_table[ wk->sys.color_type ];
}

//Recoder_ColorChange
/*	
	GET_PAL_FONT = 0,
	GET_PAL_PHOTO,
	GET_PAL_OBJ,
	GET_PAL_BG,	
*/
int BR_ColorPaletteID_Get( BR_WORK* wk, int type )
{
	int font[] = {
		NARC_batt_rec_gra_batt_rec_font_NCLR,
		NARC_batt_rec_gra_batt_rec_font2_NCLR,
		NARC_batt_rec_gra_batt_rec_font3_NCLR,
		NARC_batt_rec_gra_batt_rec_font4_NCLR,
		NARC_batt_rec_gra_batt_rec_font5_NCLR,
		NARC_batt_rec_gra_batt_rec_font6_NCLR,
		NARC_batt_rec_gra_batt_rec_font7_NCLR,
//		NARC_batt_rec_gra_batt_rec_font8_NCLR,
//		NARC_batt_rec_gra_batt_rec_font9_NCLR,
	};
	int photo[] = {
		NARC_batt_rec_gra_batt_rec_photo_NCLR,
		NARC_batt_rec_gra_batt_rec_photo2_NCLR,
		NARC_batt_rec_gra_batt_rec_photo3_NCLR,
		NARC_batt_rec_gra_batt_rec_photo4_NCLR,
		NARC_batt_rec_gra_batt_rec_photo5_NCLR,
		NARC_batt_rec_gra_batt_rec_photo6_NCLR,
		NARC_batt_rec_gra_batt_rec_photo7_NCLR,
//		NARC_batt_rec_gra_batt_rec_photo8_NCLR,
//		NARC_batt_rec_gra_batt_rec_photo9_NCLR,
	};
	int obj[] = {
		NARC_batt_rec_gra_batt_rec_browse_obj_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_obj2_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_obj3_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_obj4_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_obj5_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_obj6_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_obj7_NCLR,
//		NARC_batt_rec_gra_batt_rec_browse_obj8_NCLR,
//		NARC_batt_rec_gra_batt_rec_browse_obj9_NCLR,
	};
	int bg[] = {
		NARC_batt_rec_gra_batt_rec_browse_bg_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_bg2_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_bg3_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_bg4_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_bg5_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_bg6_NCLR,
		NARC_batt_rec_gra_batt_rec_browse_bg7_NCLR,
//		NARC_batt_rec_gra_batt_rec_browse_bg8_NCLR,
//		NARC_batt_rec_gra_batt_rec_browse_bg9_NCLR,
	};
	int no;
	
	no = wk->sys.color_type;
	
	switch ( type ){
	case 0: return font[ no ];
	case 1: return photo[ no ];
	case 2: return obj[ no ];
	case 3:	return bg[ no ];
	}
	
	GF_ASSERT( 0 );
	
	return font[ 0 ];
}

//--------------------------------------------------------------
/**
 * @brief	�J���[�^�C�v�ݒ�
 *
 * @param	wk	
 * @param	type	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_ColorPaletteChange( BR_WORK* wk, int type )
{
	PALETTE_FADE_PTR pfd = wk->sys.pfd;
	CATS_SYS_PTR csp = wk->sys.csp;
	CATS_RES_PTR crp = wk->sys.crp;
	ARCHANDLE*	 hdl = wk->sys.p_handle;
	
	PaletteWorkSet_Arc( pfd, ARC_BR_GRA, BR_ColorPaletteID_Get( wk, GET_PAL_BG ),	HEAPID_BR, FADE_MAIN_BG, 0x20 * BG_PAL_BASE_END, eBG_PAL_BASE_0 );	
	PaletteWorkSet_Arc( pfd, ARC_BR_GRA, BR_ColorPaletteID_Get( wk, GET_PAL_BG ),	HEAPID_BR, FADE_SUB_BG,	 0x20 * BG_PAL_BASE_END, eBG_PAL_BASE_0 );	
	PaletteWorkSet_Arc( pfd, ARC_BR_GRA, BR_ColorPaletteID_Get( wk, GET_PAL_FONT ), HEAPID_BR, FADE_MAIN_BG, 0x20, eBG_PAL_FONT*16 );	
	PaletteWorkSet_Arc( pfd, ARC_BR_GRA, BR_ColorPaletteID_Get( wk, GET_PAL_FONT ), HEAPID_BR, FADE_SUB_BG,	 0x20, eBG_PAL_FONT*16 );
	
	Cursor_ColorTypeChange( wk->cur_wk, wk->sys.color_type );
	Cursor_ColorTypeChange( wk->cur_wk, wk->sys.color_type );

	CATS_FreeResourcePltt( crp, eID_OAM_SIDE_BAR_M );
	CATS_FreeResourcePltt( crp, eID_OAM_SIDE_BAR_S );
	CATS_LoadResourcePlttWorkArcH( pfd, FADE_MAIN_OBJ, csp, crp, hdl, BR_ColorPaletteID_Get( wk, GET_PAL_OBJ ), FALSE, OAM_TAG_PAL_MAX, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_SIDE_BAR_M );
	CATS_LoadResourcePlttWorkArcH( pfd, FADE_SUB_OBJ,  csp, crp, hdl, BR_ColorPaletteID_Get( wk, GET_PAL_OBJ ), FALSE, OAM_TAG_PAL_MAX, NNS_G2D_VRAM_TYPE_2DSUB,  eID_OAM_SIDE_BAR_S );
}

// ----------------------------------------------------------------------------
//
//
//	���\�t�g�t�F�[�h�֐��S
//
//
// ----------------------------------------------------------------------------
// -----------------------------------------
//
//	�� �\�t�g�t�F�[�h����\����
//
// -----------------------------------------
typedef struct _FADE_SYS {
	
	BOOL	active;				///< TRUE = ���쒆
	TCB_PTR	tcb;
	
	FADEREQ req;
	u16		start_pos;
	u16		col_num;
	u16		next_rgb;
	u8		start_evy;
	u8		end_evy;
	u8		evy;
	s8		param;
	s8		wait;
	s8		timer;
	
	PALETTE_FADE_PTR	pfd;	///< pfd
	
} FADE_SYS;

static void PalGrayScale(u16 *pal, u16 pal_size);

static void PalGrayScale(u16 *pal, u16 pal_size)
{
	int i, r, g, b;
	u32 c;

	for(i = 0; i < pal_size; i++){
		r = ((*pal) & 0x1f);
		g = (((*pal) >> 5) & 0x1f);
		b = (((*pal) >> 10) & 0x1f);

		c = RGBtoY(r,g,b);

		*pal = (u16)((c<<10)|(c<<5)|c);
		pal++;
	}
}

//--------------------------------------------------------------
/**
 * @brief	�t�F�[�h�̂������Ă��Ȃ���Ԃ�
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void PaletteFadeClear( BR_WORK* wk )
{
	ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_OBJ, 0xFFFF, 0, wk->sys.logo_color );	///< main	oam
	ColorConceChangePfd( wk->sys.pfd, FADE_MAIN_BG,  0xFFFF, 0, wk->sys.logo_color );	///< main	bg
	ColorConceChangePfd( wk->sys.pfd, FADE_SUB_OBJ,  0xFFFF, 0, wk->sys.logo_color );	///< sub	oam
	ColorConceChangePfd( wk->sys.pfd, FADE_SUB_BG, 	 0xFFFF, 0, wk->sys.logo_color );	///< sub	bg
}


//--------------------------------------------------------------
/**
 * @brief	50���Ƀ\�[�g���ꂽ�f�[�^�擾
 *
 * @param	heap	
 * @param	idx	
 * @param	p_arry_num	
 *
 * @retval	u16*	
 *
 */
//--------------------------------------------------------------
u16* ZukanSortDataGet_50on( int heap, int idx, int* p_arry_num )
{
	u32 size;
	u16* p_buf;
	
	// �ǂݍ���
	p_buf = ArcUtil_LoadEx( ARC_ZUKAN_DATA, NARC_zukan_data_zkn_sort_aiueo_dat, FALSE, heap, ALLOC_TOP, &size );

	*p_arry_num = size / sizeof( u16 );

	return p_buf;
}


// =============================================================================
//
//
//	���g���p�����[�^����
//
//
// =============================================================================
//--------------------------------------------------------------
/**
 * @brief	�t���O�𗧂Ă�
 *
 * @param	wk	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void ExParam2_Set( BR_WORK* wk, BOOL flag )
{
	wk->ex_param2 = flag;
}


//--------------------------------------------------------------
/**
 * @brief	�t���O���擾
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL ExParam2_Get( BR_WORK* wk )
{
	return wk->ex_param2;
}


// =============================================================================
//
//
//	�����荞�݂悤���b�Z�[�W�@���M�ł��Ȃ��Ƃ�
//
//
// =============================================================================
void Tool_InfoMessage( BR_WORK* wk, int no )
{
	STRBUF* str1;
	GF_BGL_BMPWIN* win = &wk->info_win;
	
	if ( wk->bInfo == TRUE ){ return; }

	wk->bInfo = TRUE;

	BR_disp_BG_InfoWindowLoad( wk, GF_BGL_FRAME2_S, FALSE );
	GF_Disp_GXS_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_ON );
	G2S_BlendNone();
	GF_BGL_PrioritySet( GF_BGL_FRAME2_S, 0 );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_S, 1, 19, 30, 4, eBG_PAL_FONT, 32 );

	str1 = MSGMAN_AllocString( wk->sys.man, no );
	
	GF_BGL_BmpWinDataFill( win, BR_MSG_CLEAR_CODE );
	
	GF_STR_PrintColor( win, FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_INFO, NULL );
	
	GF_BGL_BmpWinOnVReq( win );
	
	STRBUF_Delete( str1 );
}

void Tool_InfoMessageSet( BR_WORK* wk, int no )
{
	STRBUF* str1;
	GF_BGL_BMPWIN* win = &wk->info_win;
	
	if ( wk->bInfo == FALSE ){ return; }

	str1 = MSGMAN_AllocString( wk->sys.man, no );
	
	GF_BGL_BmpWinDataFill( win, BR_MSG_CLEAR_CODE );
	
	GF_STR_PrintColor( win, FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_INFO, NULL );
	
	GF_BGL_BmpWinOnVReq( win );
	
	STRBUF_Delete( str1 );	
}

void Tool_InfoMessageDel( BR_WORK* wk )
{
	if ( wk->bInfo == FALSE ){ return; }
	wk->bInfo = FALSE;
	
	GF_BGL_BmpWinOff( &wk->info_win );
	GF_BGL_BmpWinDel( &wk->info_win );
	GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_S );
	GF_Disp_GXS_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_OFF );
	
	
	GF_BGL_PrioritySet( GF_BGL_FRAME2_S, eBR_BG2_PRI );
	
	G2_SetBlendAlpha( GX_BLEND_PLANEMASK_BG2,
					  GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ,	7, 8 );
		
	G2S_SetBlendAlpha( GX_BLEND_PLANEMASK_BG2,
					   GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ,  7, 8 );
}

void InfoMessageDel( BR_WORK* wk )
{	
	GF_BGL_BmpWinOff( &wk->info_win );
	GF_BGL_BmpWinDel( &wk->info_win );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_OFF );
}

void Tool_InfoMessageMain( BR_WORK* wk, int no )
{
	STRBUF* str1;
	GF_BGL_BMPWIN* win = &wk->info_win;
	
	if ( wk->bInfo == TRUE ){ return; }

	wk->bInfo = TRUE;

	BR_disp_BG_InfoWindowLoad( wk, GF_BGL_FRAME2_M, FALSE );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_ON );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_M, 1, 19, 30, 4, eBG_PAL_FONT, 32 );

	str1 = MSGMAN_AllocString( wk->sys.man, no );
	
	GF_BGL_BmpWinDataFill( win, BR_MSG_CLEAR_CODE );
	
	GF_STR_PrintColor( win, FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_INFO, NULL );
	
	GF_BGL_BmpWinOnVReq( win );
	
	STRBUF_Delete( str1 );
}

void Tool_InfoMessageMainSet( BR_WORK* wk, int no )
{
	STRBUF* str1;
	GF_BGL_BMPWIN* win = &wk->info_win;
	
	if ( wk->bInfo == FALSE ){ return; }

	str1 = MSGMAN_AllocString( wk->sys.man, no );
	
	GF_BGL_BmpWinDataFill( win, BR_MSG_CLEAR_CODE );
	
	GF_STR_PrintColor( win, FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_INFO, NULL );
	
	GF_BGL_BmpWinOnVReq( win );
	
	STRBUF_Delete( str1 );	
}

void Tool_InfoMessageMainDel( BR_WORK* wk )
{
	if ( wk->bInfo == FALSE ){ return; }
	
	wk->bInfo = FALSE;
	
	GF_BGL_BmpWinOff( &wk->info_win );
	GF_BGL_BmpWinDel( &wk->info_win );
	GF_BGL_ScrClear( wk->sys.bgl, GF_BGL_FRAME2_M );
	GF_Disp_GX_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_OFF );
}

void Tool_WiFiEnd_Message( BR_WORK* wk, int no )
{
	STRBUF* str1;
	GF_BGL_BMPWIN* win = &wk->info_win;
	
	if ( wk->bInfo == TRUE ){ return; }
	if ( wk->bWiFiEnd == TRUE ){ return; }

	wk->bInfo = TRUE;
	wk->bWiFiEnd = TRUE;

	G2S_BlendNone();
	GF_BGL_PrioritySet( GF_BGL_FRAME2_S, 0 );

	BR_disp_BG_InfoWindowLoad( wk, GF_BGL_FRAME2_S, FALSE );
	GF_Disp_GXS_VisibleControl(  GX_PLANEMASK_BG2, VISIBLE_ON );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_S, 1, 19, 30, 4, eBG_PAL_FONT, 32 );

	str1 = MSGMAN_AllocString( wk->sys.man, no );
	
	GF_BGL_BmpWinDataFill( win, BR_MSG_CLEAR_CODE );
	
	GF_STR_PrintColor( win, FONT_SYSTEM, str1, 0, 0, MSG_NO_PUT, PRINT_COL_INFO, NULL );
	
	GF_BGL_BmpWinOnVReq( win );
	
	STRBUF_Delete( str1 );
}

// =============================================================================
//
//
//	���@GPP_WORK ����v���t�B�[����ʂ��ꊇ���č�� (�X�N���[���͎��O��)
//
//
// =============================================================================
static void BR_prof_WinAdd_Top( GPP_WORK* gwk, BR_WORK* wk );
static void BR_prof_WinDel_Top( GPP_WORK* gwk, BR_WORK* wk );
static void PokeIcon_Visible( GPP_WORK* gwk, int flag );
static void PokeIcon_Del( GPP_WORK* gwk, BR_WORK* wk );
static void PokeIcon_Add( GPP_WORK* gwk, BR_WORK* wk );
static void HeroIcon_Visible( GPP_WORK* gwk, int flag );
static void HeroIcon_Del( GPP_WORK* gwk, BR_WORK* wk );
static void HeroIcon_Add( GPP_WORK* gwk, BR_WORK* wk );

static void BR_prof_WinAdd_Top( GPP_WORK* gwk, BR_WORK* wk )
{
	int i;
	int ofs;
	GF_BGL_BMPWIN* win;
	STRBUF*	str;

	{
		s16 win_p[][4] = {
			{  4, 4, 24, 2 },	///< ���o��
			{ 15, 8,  6, 2 },	///< �˂�ꂢ
			{ 16, 8, 12, 2 },	///< ��������
			{  4,11, 11, 2 },	///< ����ł���`
			{  4,13, 24, 2 },	///< ���E�n��
			{  4,15, 24, 2 },	///< �n��
			{  4,17, 11, 2 },	///< ���ȏЉ�
			{  4,19, 24, 4 },	///< �ȈՕ�
			{ 0xFF,0,0,0 },
			{  4,21, 24, 2 },	///< �f�[�^�i���o�[			
		};
		int mes_id[] = {
			msg_20, msg_20, msg_20,	msg_20,
			msg_20,	msg_20,	msg_20, msg_20,
			msg_20,	msg_20,	msg_20, msg_20,
		};
		
		ofs = 1;
		for ( i = 0; i < 8; i++ ){
						
			win = &gwk->win[ i ];				
			str = MSGMAN_AllocString( wk->sys.man, mes_id[ i ] );				
			GF_BGL_BmpWinInit( win );
			GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_M,
							  win_p[i][0], win_p[i][1], win_p[i][2], win_p[i][3], eBG_PAL_FONT, ofs );				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, str, BR_print_x_Get( win, str ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
			GF_BGL_BmpWinOnVReq( win );
			ofs += ( win_p[i][2] * win_p[i][3] );
			STRBUF_Delete( str );
		}
//		OS_Printf( "win add char end %d\n", ofs );
	}
	
	///< �����`��
	{
		STRBUF* 	str1;
		STRBUF* 	str2;
		STRBUF*		tmp;
		WORDSET*	wset;
		GF_BGL_BMPWIN* win;
		GDS_PROFILE_PTR	gpp;
		
		gpp = gwk->gpp;
		wset = BR_WORDSET_Create( HEAPID_BR );
		
		///< ���o��
		win = &gwk->win[ 0 ];
		str1 = GDS_Profile_CreateNameString( gpp, HEAPID_BR );			///< ���O
		BR_ErrorStrChange( wk, str1 );
		str2 = MSGMAN_AllocString( wk->sys.man, msg_13 );			///< �` �̃v���t�B�[��
		tmp  = STRBUF_Create( 255, HEAPID_BR );						///< �e���|����

		WORDSET_RegisterWord( wset, 0, str1, 0, TRUE, PM_LANG );	
		WORDSET_ExpandStr( wset, tmp, str2 );
	
		GF_BGL_BmpWinDataFill( win, 0x00 );
		GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
		GF_BGL_BmpWinOnVReq( win );

		STRBUF_Delete( str1 );
		STRBUF_Delete( str2 );
		STRBUF_Delete( tmp );
		WORDSET_ClearAllBuffer( wset );
		
		///< �N��
		{
			int age = GDS_Profile_GetMonthBirthday( gpp );
		#if 0	
			win = &gwk->win[ 1 ];
			tmp = MSGMAN_AllocString( wk->sys.man, msg_14 );				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
			GF_BGL_BmpWinOnVReq( win );
			STRBUF_Delete( tmp );
		#endif	
			win  = &gwk->win[ 2 ];			
			str1 = MSGMAN_AllocString( wk->sys.man, msg_15 );
			str2 = GDS_Profile_CreateNameString( gpp, HEAPID_BR );
			BR_ErrorStrChange( wk, str2 );
			tmp  = STRBUF_Create( 255, HEAPID_BR );	
			
			STRBUF_SetNumber( str2, age, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			
			WORDSET_RegisterWord( wset, 0, str2, 0, TRUE, PM_LANG );	
			WORDSET_ExpandStr( wset, tmp, str1 );
				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
			GF_BGL_BmpWinOnVReq( win );
			
			STRBUF_Delete( str1 );
			STRBUF_Delete( str2 );
			STRBUF_Delete( tmp );
			WORDSET_ClearAllBuffer( wset );
		}
		
		///< ����ł���΂���
		{
			win = &gwk->win[ 3 ];
			tmp = MSGMAN_AllocString( wk->sys.man, msg_16 );				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
			GF_BGL_BmpWinOnVReq( win );
			STRBUF_Delete( tmp );
			
			{		
				int country = GDS_Profile_GetNation( gpp );
				int area	= GDS_Profile_GetArea( gpp );
				
//				OS_Printf( " country = %d area = %d\n", country, area );
				
				win = &gwk->win[ 4 ];							
				GF_BGL_BmpWinDataFill( win, 0x00 );
				
				if ( country == 0 ){
					///< ���R�[�h���ݒ�
					tmp = MSGMAN_AllocString( wk->sys.man, msg_21 );
				//	GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
					GF_STR_PrintColor( win, FONT_SYSTEM, tmp, 0, 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
					GF_BGL_BmpWinOnVReq( win );				
					STRBUF_Delete( tmp );
				}
				else {
					///< �����\��
					tmp  = STRBUF_Create( 255, HEAPID_BR );
					str1 = MSGMAN_AllocString( wk->sys.man, msg_22 );
					WORDSET_RegisterCountryName( wset, 0, country);
					WORDSET_ExpandStr( wset, tmp, str1 );
				//	GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
					GF_STR_PrintColor( win, FONT_SYSTEM, tmp, 0, 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
					GF_BGL_BmpWinOnVReq( win );
					STRBUF_Delete( tmp );
					STRBUF_Delete( str1 );
					
					if ( area != 0 ){
						///< �n�於�\��
						win = &gwk->win[ 5 ];							
						GF_BGL_BmpWinDataFill( win, 0x00 );
						tmp	 = STRBUF_Create( 255, HEAPID_BR );
						str1 = MSGMAN_AllocString( wk->sys.man, msg_23 );
						WORDSET_RegisterLocalPlaceName( wset, 0, country, area );
						WORDSET_ExpandStr( wset, tmp, str1 );
				//		GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
						GF_STR_PrintColor( win, FONT_SYSTEM, tmp, 4, 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
						GF_BGL_BmpWinOnVReq( win );
						STRBUF_Delete( tmp );
						STRBUF_Delete( str1 );
					}
				}
				
				WORDSET_ClearAllBuffer( wset );
			}
		}
		
		///< �������傤����
		{
			win = &gwk->win[ 6 ];
			tmp = MSGMAN_AllocString( wk->sys.man, msg_17 );				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
			GF_BGL_BmpWinOnVReq( win );
			STRBUF_Delete( tmp );
			
			{
				PMS_DATA	dest;
								
				win = &gwk->win[ 7 ];
				
				tmp = GDS_Profile_GetSelfIntroduction( gpp, &dest, HEAPID_BR );
				
				if ( tmp == NULL ){
					tmp = PMSDAT_ToString( &dest, HEAPID_BR );
				}
				
				GF_BGL_BmpWinDataFill( win, 0x00 );
				GF_STR_PrintColor( win, FONT_SYSTEM, tmp, 0, 0, MSG_NO_PUT, PRINT_COL_PHOTO, NULL );
				GF_BGL_BmpWinOnVReq( win );
				
				STRBUF_Delete( tmp );				
			}
		}
		
		WORDSET_Delete( wset );
	}
}


static void BR_prof_WinDel_Top( GPP_WORK* gwk, BR_WORK* wk )
{
	int i;
	
	for ( i = 0; i < 8; i++ ){
		GF_BGL_BmpWinOff( &gwk->win[ i ] );
		GF_BGL_BmpWinDel( &gwk->win[ i ] );		
	}	
}

static void PokeIcon_Visible( GPP_WORK* gwk, int flag )
{
	if ( gwk->cap_poke ){
		CATS_ObjectEnableCap( gwk->cap_poke, flag );
	}
}

static void PokeIcon_Del( GPP_WORK* gwk, BR_WORK* wk )
{
	CATS_FreeResourceChar( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourcePltt( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourceCell( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourceCellAnm( wk->sys.crp, eID_OAM_POKE_ICON );	
	
	if ( gwk->cap_poke ){
		CATS_ActorPointerDelete_S( gwk->cap_poke );
		gwk->cap_poke = NULL;
	}
}

static void PokeIcon_Add( GPP_WORK* gwk, BR_WORK* wk )
{	
	int egg;
	int monsno;
	int palno;
	int form;
	u8	trans_pal;
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	TCATS_OBJECT_ADD_PARAM_S coap;
	
	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;
	
	//�p���b�g
	trans_pal = CATS_LoadResourcePlttWorkArc( pfd, FADE_MAIN_OBJ, csp, crp,
		ARC_POKEICON, PokeIconPalArcIndexGet(), 0, POKEICON_PAL_MAX, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON );
	//�Z��
	{
		int arc = PokeIcon64kCellArcIndexGet();
		CATS_LoadResourceCellArc( csp, crp, ARC_POKEICON, arc, 0, eID_OAM_POKE_ICON );
	}
	//�Z���A�j��
	{
		int arc = PokeIcon64kCellAnmArcIndexGet();
		CATS_LoadResourceCellAnmArc( csp, crp, ARC_POKEICON, arc, 0, eID_OAM_POKE_ICON );
	}

	{		
		int gra_id;
		
		monsno = GDS_Profile_GetMonsNo( gwk->gpp );
		form   = GDS_Profile_GetFormNo( gwk->gpp );
		egg	   = GDS_Profile_GetEggFlag( gwk->gpp );
		
		if ( monsno == 0 ){ return; }
		
		gra_id = PokeIconCgxArcIndexGetByMonsNumber( monsno, egg, form );
		
		CATS_LoadResourceCharArcModeAdjustAreaCont(	csp, crp, ARC_POKEICON, gra_id, 0,
						 							NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON );
	
		coap.x		= 72 + 4;
		coap.y		= 64 - 4;
		coap.z		= 0;		
		coap.anm	= 0;
		coap.pri	= 0;
		coap.pal	= 0;
		coap.d_area = CATS_D_AREA_MAIN;
		coap.bg_pri = 0;
		coap.vram_trans = 0;

		coap.id[0] = eID_OAM_POKE_ICON;
		coap.id[1] = eID_OAM_POKE_ICON;
		coap.id[2] = eID_OAM_POKE_ICON;
		coap.id[3] = eID_OAM_POKE_ICON;
		coap.id[4] = CLACT_U_HEADER_DATA_NONE;
		coap.id[5] = CLACT_U_HEADER_DATA_NONE;
		
		gwk->cap_poke = CATS_ObjectAdd_S( wk->sys.csp, wk->sys.crp, &coap );

		palno = PokeIconPalNumGet( monsno, form, egg );
		CATS_ObjectPaletteOffsetSetCap( gwk->cap_poke, trans_pal + palno );
		CATS_ObjectAnimeSeqSetCap( gwk->cap_poke, POKEICON_ANM_HPMAX );
	}
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void HeroIcon_Visible( GPP_WORK* gwk, int flag )
{
	CATS_ObjectEnableCap( gwk->cap_hero, flag );
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void HeroIcon_Del( GPP_WORK* gwk, BR_WORK* wk )
{	
	CATS_FreeResourceChar( wk->sys.crp, eID_OAM_HERO_ICON );
	CATS_FreeResourcePltt( wk->sys.crp, eID_OAM_HERO_ICON );
	CATS_FreeResourceCell( wk->sys.crp, eID_OAM_HERO_ICON );
	CATS_FreeResourceCellAnm( wk->sys.crp, eID_OAM_HERO_ICON );
	
	CATS_ActorPointerDelete_S( gwk->cap_hero );
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static const int wifi_tr_type_tbl[ BR_UNION_CHAR_MAX ][ 4 ] = {
	{
		BOY1,
		TRTYPE_SCHOOLB,
		NARC_wifi2dchar_boy1_NCLR,
		NARC_wifi2dchar_boy1_NCGR, 
	},
	{
		BOY3,
		TRTYPE_MUSHI,
		NARC_wifi2dchar_boy3_NCLR,
		NARC_wifi2dchar_boy3_NCGR, 
	},
	{
		MAN3,
		TRTYPE_ELITEM,
		NARC_wifi2dchar_man3_NCLR,
		NARC_wifi2dchar_man3_NCGR, 
	},
	{
		BADMAN,
		TRTYPE_HEADS,
		NARC_wifi2dchar_badman_NCLR,
		NARC_wifi2dchar_badman_NCGR, 
	},
	{
		EXPLORE,
		TRTYPE_ISEKI,
		NARC_wifi2dchar_explore_NCLR,
		NARC_wifi2dchar_explore_NCGR, 
	},
	{
		FIGHTER,
		TRTYPE_KARATE,
		NARC_wifi2dchar_fighter_NCLR,
		NARC_wifi2dchar_fighter_NCGR, 
	},
	{
		GORGGEOUSM,
		TRTYPE_PRINCE,
		NARC_wifi2dchar_gorggeousm_NCLR,
		NARC_wifi2dchar_gorggeousm_NCGR, 
	},
	{
		MYSTERY,
		TRTYPE_ESPM,
		NARC_wifi2dchar_mystery_NCLR,
		NARC_wifi2dchar_mystery_NCGR, 
	},
	{
		GIRL1,
		TRTYPE_MINI,
		NARC_wifi2dchar_girl1_NCLR,
		NARC_wifi2dchar_girl1_NCGR, 
	},
	{
		GIRL2,
		TRTYPE_BATTLEG,
		NARC_wifi2dchar_girl2_NCLR,
		NARC_wifi2dchar_girl2_NCGR, 
	},
	{
		WOMAN2,
		TRTYPE_SISTER,
		NARC_wifi2dchar_woman2_NCLR,
		NARC_wifi2dchar_woman2_NCGR, 
	},
	{
		WOMAN3,
		TRTYPE_ELITEW,
		NARC_wifi2dchar_woman3_NCLR,
		NARC_wifi2dchar_woman3_NCGR, 
	},
	{
		IDOL,
		TRTYPE_IDOL,
		NARC_wifi2dchar_idol_NCLR,
		NARC_wifi2dchar_idol_NCGR, 
	},
	{
		LADY,
		TRTYPE_MADAM,
		NARC_wifi2dchar_lady_NCLR,
		NARC_wifi2dchar_lady_NCGR, 
	},
	{
		COWGIRL,
		TRTYPE_COWGIRL,
		NARC_wifi2dchar_cowgirl_NCLR,
		NARC_wifi2dchar_cowgirl_NCGR, 
	},
	{
		GORGGEOUSW,
		TRTYPE_PRINCESS,
		NARC_wifi2dchar_gorggeousw_NCLR,
		NARC_wifi2dchar_gorggeousw_NCGR, 
	},	
};


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	trtype	
 * @param	data	1 = TR_TYPE, 2 = ncl, 3 = ncg 4 = trtype_name(�I���W�i���T�C�L�b�J�[���Ƃ��Ƃ�)
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int WiFi_TR_DataGet( int trtype, int data )
{
	int index = trtype;
	
	if ( trtype > BR_UNION_CHAR_MAX || trtype < 0 ){
		GF_ASSERT_MSG( 0, "trtype �ُ� = %d\n", trtype );		
		index = 0;
	}
	
	if ( data == 4 ){
	//	OS_Printf( " trtype = %d\n", index );
		return msg_trtype_000 + index;
	}

	if ( data >= 5 ){
		return 0;
	}

	return wifi_tr_type_tbl[ index ][ data ];
}


static void HeroIcon_Add( GPP_WORK* gwk, BR_WORK* wk )
{
	int sex;
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	TCATS_OBJECT_ADD_PARAM_S coap;

	int	ncl = NARC_wifi2dchar_pl_boy01_NCLR;
	int ncg = NARC_wifi2dchar_pl_boy01_NCGR;
	int	nce = NARC_wifi2dchar_npc_NCER;
	int nca = NARC_wifi2dchar_npc_NANR;	

	sex = GDS_Profile_GetSex( gwk->gpp );

	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;
	
	if ( wk->ex_param1 == LOADDATA_MYREC ){
		
		if ( sex == PM_FEMALE ){
			ncl = NARC_wifi2dchar_pl_girl01_NCLR;
			ncg = NARC_wifi2dchar_pl_girl01_NCGR;
		}
	}
	else {
		int view = GDS_Profile_GetTrainerView( gwk->gpp );
		
		if ( view >= BR_UNION_CHAR_MAX ){ view = 0; }
		
		ncl = WiFi_TR_DataGet( view, 2 );
		ncg = WiFi_TR_DataGet( view, 3 );
	}
	
	CATS_LoadResourcePlttWorkArc( pfd, FADE_MAIN_OBJ, csp, crp, ARC_WIFI2DCHAR, ncl, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_HERO_ICON );
	CATS_LoadResourceCellArc( csp, crp, ARC_WIFI2DCHAR, nce, 0, eID_OAM_HERO_ICON );
	CATS_LoadResourceCellAnmArc( csp, crp, ARC_WIFI2DCHAR, nca, 0, eID_OAM_HERO_ICON );
	CATS_LoadResourceCharArcModeAdjustAreaCont(	csp, crp, ARC_WIFI2DCHAR, ncg, 0, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_HERO_ICON );

	coap.x		= 40;
	coap.y		= 56+4;
	coap.z		= 0;		
	coap.anm	= 0;
	coap.pri	= 0;
	coap.pal	= 0;
	coap.d_area = CATS_D_AREA_MAIN;
	coap.bg_pri = 0;
	coap.vram_trans = 0;
			
	coap.id[0] = eID_OAM_HERO_ICON;
	coap.id[1] = eID_OAM_HERO_ICON;
	coap.id[2] = eID_OAM_HERO_ICON;
	coap.id[3] = eID_OAM_HERO_ICON;
	coap.id[4] = CLACT_U_HEADER_DATA_NONE;
	coap.id[5] = CLACT_U_HEADER_DATA_NONE;
	
	gwk->cap_hero = CATS_ObjectAdd_S( wk->sys.csp, wk->sys.crp, &coap );
	CATS_ObjectAnimeSeqSetCap( gwk->cap_hero, eHERO_BOTTOM );
	CATS_ObjectUpdateCap( gwk->cap_hero );
}


//--------------------------------------------------------------
/**
 * @brief	GPP�f�[�^�쐬
 *
 * @param	gwk	
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void GppDataMake( GPP_WORK* gwk, BR_WORK* wk )
{
	BR_prof_WinAdd_Top( gwk, wk );
	HeroIcon_Add( gwk, wk );
	PokeIcon_Add( gwk, wk );
}


//--------------------------------------------------------------
/**
 * @brief	GPP�f�[�^���
 *
 * @param	gwk	
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void GppDataFree( GPP_WORK* gwk, BR_WORK* wk )
{
	BR_prof_WinDel_Top( gwk, wk );
	GppData_IconEnable( gwk, FALSE );
	HeroIcon_Del( gwk, wk );
	PokeIcon_Del( gwk, wk );
}


//--------------------------------------------------------------
/**
 * @brief	GPP�f�[�^��\��
 *
 * @param	gwk	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void GppData_IconEnable( GPP_WORK* gwk, int flag )
{
	PokeIcon_Visible( gwk, flag );
	HeroIcon_Visible( gwk, flag );
}

// =============================================================================
//
//
//	�����X�g����
//
//
// =============================================================================
static void CountryPrint( TOUCH_LIST* tl, BR_WORK* wk )
{
	STRBUF*		str1;
	STRBUF*		tmp;
	WORDSET*	wset;
	GF_BGL_BMPWIN* win;
	
	int country = GDS_Profile_GetNation( wk->br_gpp[ 0 ] );
	int area	= GDS_Profile_GetArea( wk->br_gpp[ 0 ] );
	
	win = &tl->win;
	if ( country == 0 && area == 0 ){
		str1 = MSGMAN_AllocString( wk->sys.man, msg_722 );	
	}
	else {
		
		wset = BR_WORDSET_Create( HEAPID_BR );
		
		if ( area != 0 ){
			str1  = STRBUF_Create( 255, HEAPID_BR );
			tmp = MSGMAN_AllocString( wk->sys.man, msg_23 );
			WORDSET_RegisterLocalPlaceName( wset, 0, country, area );
			WORDSET_ExpandStr( wset, str1, tmp );
		}
		else {
			str1  = STRBUF_Create( 255, HEAPID_BR );
			tmp = MSGMAN_AllocString( wk->sys.man, msg_22 );
			WORDSET_RegisterCountryName( wset, 0, country );
			WORDSET_ExpandStr( wset, str1, tmp );			
		}
		STRBUF_Delete( tmp );
		WORDSET_Delete( wset );				
	}
	GF_STR_PrintColor( win, FONT_SYSTEM, str1, 0, 16, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
	GF_BGL_BmpWinOnVReq( win );
	STRBUF_Delete( str1 );
}
//--------------------------------------------------------------
/**
 * @brief	���X�g�̍X�V
 *
 * @param	tl	
 * @param	wk	
 * @param	head	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchList_Init( TOUCH_LIST* tl, BR_WORK* wk, const TOUCH_LIST_HEADER* head )
{
	int i;
	
	GF_BGL_BMPWIN* win;
	STRBUF* 	str;
	
//	memset( tl, 0, sizeof( TOUCH_LIST ) );

	win = &tl->win;

	tl->max = head->data_max;
	tl->min = 0;
//	tl->now = 0;
	tl->old = 0;
	tl->lim = head->lim;
	tl->head = head;
	tl->man	 = wk->sys.man;
	tl->page = ( tl->max / tl->lim ) + 1;
	
	if ( tl->max < tl->lim ){
		 tl->lim = tl->max;
		 tl->view.t_stoper = tl->lim - 1;
		 tl->view.b_stoper = tl->lim - 1;
	}
//	tl->bView = TRUE;	
//	OS_Printf( "list max = %d\n", tl->max );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, head->frame, head->px, head->py, head->sx, head->sy, eBG_PAL_FONT, head->ofs );
	GF_BGL_BmpWinDataFill( win, 0x00 );	
	for ( i = 0; i < tl->lim; i++ ){
		
		str = MSGMAN_AllocString( tl->man, head->info[ i ].msg );
		GF_STR_PrintColor( win, FONT_SYSTEM, str, 0, ( head->list_height * 16 ) * i, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );		
		STRBUF_Delete( str );
		if ( head->info[ i ].msg == msg_16 ){
			CountryPrint( tl, wk );
		}
	}
	GF_BGL_BmpWinOnVReq( win );
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	tl	
 * @param	wk	
 * @param	man	
 * @param	head	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchList_InitEx( TOUCH_LIST* tl, BR_WORK* wk, MSGDATA_MANAGER* man, const TOUCH_LIST_HEADER* head )
{
	int i;
	
	GF_BGL_BMPWIN* win;
	STRBUF* 	str;
	
//	memset( tl, 0, sizeof( TOUCH_LIST ) );

	win = &tl->win;

	tl->max = head->data_max;
	tl->min = 0;
//	tl->now = 0;				///< �����p��
	tl->old = 0;				///< ���X�g�̍ĕ`��𑣂����߂ɏ����� ����� now != old = �`��
	tl->lim = head->lim;
	tl->head = head;
	if ( man ){
		tl->man	 = man;
	}
	else {
		tl->man	 = wk->sys.man;
	}
	tl->page = ( tl->max / tl->lim ) + 1;
	
//	tl->bView = TRUE;	
	tl->view.t_stoper = tl->lim / 2;
	tl->view.b_stoper = tl->lim / 2;
	if ( tl->max < tl->lim ){
		 tl->lim = tl->max;
		 tl->view.t_stoper = tl->lim - 1;
		 tl->view.b_stoper = tl->lim - 1;
	}
//	OS_Printf( "stoper = %d %d\n", tl->view.t_stoper, tl->view.b_stoper );
//	OS_Printf( "list max = %d\n", tl->max );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, head->frame, head->px, head->py, head->sx, head->sy, eBG_PAL_FONT, head->ofs );
	GF_BGL_BmpWinDataFill( win, 0x00 );	
	
	if ( head->info ){
		for ( i = 0; i < tl->lim; i++ ){		
			str = MSGMAN_AllocString( tl->man, head->info[ i ].msg );
			GF_STR_PrintColor( win, FONT_SYSTEM, str, 0, ( head->list_height * 16 ) * i, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );		
			STRBUF_Delete( str );
		}
	}
	else {
		int win_no;
		STRBUF* str1;
		STRBUF*	str_num;
		STRBUF* str_colon;
		int width;	
		for ( i = 0; i < tl->lim; i++ ){
			str1 	= STRBUF_Create( 255, HEAPID_BR );
			str_num = STRBUF_Create( 255, HEAPID_BR );
			str_colon = MSGMAN_AllocString( wk->sys.man, msg_607 );
			
			STRBUF_SetNumber( str_num, ( i + 1 ), 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			STRBUF_SetStringCode( str1, wk->recv_data.outline_data_tmp[ i ]->profile.name );
			BR_ErrorStrChange( wk, str1 );
			width = FontProc_GetPrintStrWidth( FONT_SYSTEM, str_num, 0 );

			GF_STR_PrintColor( win, FONT_SYSTEM, str_num, 	16 - width, 4 + ( i * 24 ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_STR_PrintColor( win, FONT_SYSTEM, str_colon, 16, 4 + ( i * 24 ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_STR_PrintColor( win, FONT_SYSTEM, str1, 		22, 4 + ( i * 24 ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );

			STRBUF_Delete( str1 );			
			STRBUF_Delete( str_num );		
			STRBUF_Delete( str_colon );
		}
	}
	
	
	GF_BGL_BmpWinOnVReq( win );
}

void TouchList_InitRanking( TOUCH_LIST* tl, BR_WORK* wk, MSGDATA_MANAGER* man, const TOUCH_LIST_HEADER* head, int ranking_no, int group_no, STRBUF* unit )
{
	int i;
	
	GF_BGL_BMPWIN* win;
	STRBUF* 	str;
	STRBUF*		str1;
	STRBUF*		str2;
	STRBUF*		str3;
	STRBUF*		temp;
	WORDSET*	wset;
	int now_rank  = 0;
	u64 now_param = 0;
	int rank_table[ 30 ] = { 0 };
	STRBUF*		str_num;

	win = &tl->win;

	tl->max = head->data_max;
	tl->min = 0;
//	tl->now = 0;				///< �����p��
	tl->old = 0;				///< ���X�g�̍ĕ`��𑣂����߂ɏ����� ����� now != old = �`��
	tl->lim = head->lim;
	tl->head = head;
	if ( man ){
		tl->man	 = man;
	}
	else {
		tl->man	 = wk->sys.man;
	}
	tl->page = ( tl->max / tl->lim ) + 1;
	
	if ( tl->max < tl->lim ){
		 tl->lim = tl->max;
		 tl->view.t_stoper = tl->lim - 1;
		 tl->view.b_stoper = tl->lim - 1;
	}	
//	tl->bView = TRUE;
	tl->view.t_stoper = tl->lim / 2;
	tl->view.b_stoper = tl->lim / 2;
//	OS_Printf( "stoper = %d %d\n", tl->view.t_stoper, tl->view.b_stoper );
//	OS_Printf( "list max = %d\n", tl->max );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, head->frame, head->px, head->py, head->sx, head->sy, eBG_PAL_FONT, head->ofs );
	GF_BGL_BmpWinDataFill( win, 0x00 );	
	
	wset = BR_WORDSET_Create( HEAPID_BR );
	
	
	{
		u64 score = tl->head->info[ 0 ].param_ex;
		int num[] = { GT_TRAINER_VIEW_MAX, GT_YEAR_MONTH_NUM, GT_MONSNO_RANKING_MAX };
		now_param = score;
		rank_table[ 0 ] = now_rank;		

		for ( i = 1; i < num[ group_no ]; i++ ){				
			u64 score = tl->head->info[ i ].param_ex;
			
			if ( score == now_param ){
				rank_table[ i ] = now_rank;
			}
			else {
				now_param = score;
				rank_table[ i ] = i;
				now_rank = i;
			}
		//	OS_Printf( "rank = %2d\n", rank_table[ i ] );
		}
	}
	
	for ( i = 0; i < tl->lim; i++ ){		
		str1 = MSGMAN_AllocString( wk->sys.man, msg_510 + group_no );		///< ���ʁF�g���[�i�[�^�C�v or ���܂ꌎ or �����ȃ|�P����
//		str1 = MSGMAN_AllocString( wk->sys.man, msg_510 + ranking_no );		///< ���ʁF�g���[�i�[�^�C�v or ���܂ꌎ or �����ȃ|�P����
		str2 = STRBUF_Create( 255, HEAPID_BR );
		temp = STRBUF_Create( 255, HEAPID_BR );		
		str_num  = STRBUF_Create( 255, HEAPID_BR );
		
	//	OS_Printf( "ranking data = %d\n", tl->head->info[ i ].param );
		
		switch ( group_no ){
		case 0:
			{
				int view  = tl->head->info[ i ].param;
				str3 = MSGMAN_AllocString( tl->man, view );
			}
			break;
		case 1:
			{
				int month = tl->head->info[ i ].param;
				str3 = STRBUF_Create( 255, HEAPID_BR );
				STRBUF_SetNumber( str3, month, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			}
			break;
		case 2:
			{
				STRCODE code[ 255 ];
				int monsno = tl->head->info[ i ].param;
				str3 = STRBUF_Create( 255, HEAPID_BR );
				MSGDAT_MonsNameGet( monsno, HEAPID_BR, code );
				STRBUF_SetStringCode( str3, code );
			}
			break;
		}
		{
			u64 score = tl->head->info[ i ].param_ex;
			STRBUF_SetNumber64( str_num, score, Number_to_Unit_Get( score ), NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT);
		}
		STRBUF_SetNumber( str2, rank_table[ i ] + 1, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
		WORDSET_RegisterWord( wset, 0, str2, 0, TRUE, PM_LANG );	///< ����
		WORDSET_RegisterWord( wset, 1, str3, 0, TRUE, PM_LANG );	///< ����
		WORDSET_ExpandStr( wset, temp, str1 );
		
		GF_STR_PrintColor( win, FONT_SYSTEM, temp, 0, i * ( 16 * tl->head->list_height ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
		if ( tl->head->list_height == 2 ){
			int width = FontProc_GetPrintStrWidth( FONT_SYSTEM, str_num, 0 );
			GF_STR_PrintColor( win, FONT_SYSTEM, str_num, 16, ( i * ( 16 * tl->head->list_height ) ) + 16, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			if ( unit ){
				GF_STR_PrintColor( win, FONT_SYSTEM, unit, 16 + width, ( i * ( 16 * tl->head->list_height ) ) + 16, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			}
		}
		STRBUF_Delete( str1 );
		STRBUF_Delete( str2 );
		STRBUF_Delete( str3 );
		STRBUF_Delete( temp );
		
		STRBUF_Delete( str_num );
		
		WORDSET_ClearAllBuffer( wset );	
	}	

	GF_BGL_BmpWinOnVReq( win );	
	WORDSET_Delete( wset );
}

//--------------------------------------------------------------
/**
 * @brief	�{�b�N�X�p�̏�����
 *
 * @param	tl	
 * @param	wk	
 * @param	head	
 * @param	str[20][2]	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchList_InitBoxShot( TOUCH_LIST* tl, BR_WORK* wk, const TOUCH_LIST_HEADER* head, STRBUF* str[20][2] )
{
	int i;

	GF_BGL_BMPWIN* win;
	win = &tl->win;

	tl->max = head->data_max;
	tl->min = 0;
//	tl->now = 0;
	tl->old = 0;
	tl->lim = head->lim;
	tl->head = head;
	tl->man	 = wk->sys.man;
	tl->page = ( tl->max / tl->lim ) + 1;
		
	tl->view.t_stoper = tl->lim / 2;
	tl->view.b_stoper = tl->lim / 2;
	
	if ( tl->max < tl->lim ){
		 tl->lim = tl->max;
		 tl->view.t_stoper = tl->lim - 1;
		 tl->view.b_stoper = tl->lim - 1;
	}
	tl->bView = TRUE;
//	OS_Printf( "list max = %d\n", tl->max );
	
	GF_BGL_BmpWinInit( win );
	GF_BGL_BmpWinAdd( wk->sys.bgl, win, head->frame, head->px, head->py, head->sx, head->sy, eBG_PAL_FONT, head->ofs );
	GF_BGL_BmpWinDataFill( win, 0x00 );	

	for ( i = 0; i < tl->lim; i++ ){
		GF_STR_PrintColor( win, FONT_SYSTEM, str[ i ][ 0 ],   0, ( ( head->list_height * 8 ) * i ) + 4, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
		GF_STR_PrintColor( win, FONT_SYSTEM, str[ i ][ 1 ], 136, ( ( head->list_height * 8 ) * i ) + 4, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
	}
	GF_BGL_BmpWinOnVReq( win );
}

//--------------------------------------------------------------
/**
 * @brief	�{�b�N�X�p�̃��C��
 *
 * @param	tl	
 * @param	wk	
 * @param	str[20][2]	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int TouchList_MainBoxShot( TOUCH_LIST* tl, BR_WORK* wk, STRBUF* str[20][2] )
{
	int i;
	int param = 0;
	GF_BGL_BMPWIN* win;
	
	if ( tl->view.this == tl->old ){ return param; }	
	if ( tl->lim == tl->max ){ return param; }
	
	win = &tl->win;
	
	GF_BGL_BmpWinDataFill( win, 0x00 );	
	{
		int i;
		int s = 0;
		int e;
				
		if ( tl->view.l_pos >= tl->view.t_stoper ){
			s = tl->view.this - tl->view.t_stoper;
			if ( tl->view.this >= ( tl->max - ( tl->view.t_stoper + 1 ) ) ){
				s = tl->max - tl->lim;
			}
		}

		for ( i = 0; i < tl->lim; i++ ){
			if ( str[ s + i ][ 0 ] == NULL ){ continue; }
			GF_STR_PrintColor( win, FONT_SYSTEM, str[ s + i ][ 0 ],   0, ( ( tl->head->list_height * 8 ) * i ) + 4, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_STR_PrintColor( win, FONT_SYSTEM, str[ s + i ][ 1 ], 136, ( ( tl->head->list_height * 8 ) * i ) + 4, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
		}
	}
	GF_BGL_BmpWinOn( win );	

	tl->old = tl->view.this;

	return param;
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�̍X�V
 *
 * @param	tl	
 * @param	wk	
 * @param	head	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
int TouchList_Ranking_Main( TOUCH_LIST* tl, BR_WORK* wk, int ranking_no, int group_no, STRBUF* unit )
{
	int i;
	int param = 0;	
	GF_BGL_BMPWIN* win;
	STRBUF*		str1;
	STRBUF*		str2;
	STRBUF*		str3;
	STRBUF*		temp;
	STRBUF*		str_num;
	WORDSET*	wset;
	
	if ( tl->now == tl->old ){ return param; }	
	if ( tl->lim == tl->max ){ return param; }
	
	win = &tl->win;
	
	wset = BR_WORDSET_Create( HEAPID_BR );
	
	GF_BGL_BmpWinDataFill( win, 0x00 );	
	{
		int no = 0;
		int now_rank  = 0;
		u64 now_param = 0;
		int s = tl->now;
		int e = s + tl->lim;
		int rank_table[ 30 ] = { 0 };
				
		if ( e >= tl->max ){
			 s = tl->max - tl->lim;
			 e = tl->max;
		}
		
		{
			u64 score = tl->head->info[ 0 ].param_ex;
			int num[] = { GT_TRAINER_VIEW_MAX, GT_YEAR_MONTH_NUM, GT_MONSNO_RANKING_MAX };
			now_param = score;
			rank_table[ 0 ] = now_rank;
			

			for ( i = 1; i < num[ group_no ]; i++ ){				
				u64 score = tl->head->info[ i ].param_ex;
				
				if ( score == now_param ){
					rank_table[ i ] = now_rank;
				}
				else {
					now_param = score;
					rank_table[ i ] = i;
					now_rank = i;
				}
			//	OS_Printf( "rank = %2d\n", rank_table[ i ] );
			}
		}
		
		for ( i = s; i < e; i++ ){		
			str1 = MSGMAN_AllocString( wk->sys.man, msg_510 + group_no );		///< ���ʁF�g���[�i�[�^�C�v or ���܂ꌎ or �����ȃ|�P����
//			str1 = MSGMAN_AllocString( wk->sys.man, msg_510 + ranking_no );		///< ���ʁF�g���[�i�[�^�C�v or ���܂ꌎ or �����ȃ|�P����			
			str2 = STRBUF_Create( 255, HEAPID_BR );
			temp = STRBUF_Create( 255, HEAPID_BR );		
			str_num  = STRBUF_Create( 255, HEAPID_BR );
			
		//	OS_Printf( "ranking data = %d\n", tl->head->info[ i ].param );
			
			switch ( group_no ){
			case 0:
				{
					int view  = tl->head->info[ i ].param;
					str3 = MSGMAN_AllocString( tl->man, view );
				}
				break;
			case 1:
				{
					int month = tl->head->info[ i ].param;
					str3 = STRBUF_Create( 255, HEAPID_BR );
					STRBUF_SetNumber( str3, month, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
				}
				break;
			case 2:
				{
					STRCODE code[ 255 ];
					int monsno = tl->head->info[ i ].param;
					str3 = STRBUF_Create( 255, HEAPID_BR );
					MSGDAT_MonsNameGet( monsno, HEAPID_BR, code );
					STRBUF_SetStringCode( str3, code );
				}
				break;
			}
			{
				u64 score = tl->head->info[ i ].param_ex;
				score = tl->head->info[ i ].param_ex;					
				STRBUF_SetNumber64( str_num, score, Number_to_Unit_Get( score ), NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT);		
			}
			STRBUF_SetNumber( str2, rank_table[ i ] + 1, 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);			
			WORDSET_RegisterWord( wset, 0, str2, 0, TRUE, PM_LANG );	///< ����
			WORDSET_RegisterWord( wset, 1, str3, 0, TRUE, PM_LANG );	///< ����
			WORDSET_ExpandStr( wset, temp, str1 );
			
			GF_STR_PrintColor( win, FONT_SYSTEM, temp, 0, no * ( 16 * tl->head->list_height ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			if ( tl->head->list_height == 2 ){
				int width = FontProc_GetPrintStrWidth( FONT_SYSTEM, str_num, 0 );
				GF_STR_PrintColor( win, FONT_SYSTEM, str_num, 16, ( no * ( 16 * tl->head->list_height ) ) + 16, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
				if ( unit ){
					GF_STR_PrintColor( win, FONT_SYSTEM, unit, 16 + width, ( no * ( 16 * tl->head->list_height ) ) + 16, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
				}
			}
			no++;
			STRBUF_Delete( str1 );
			STRBUF_Delete( str2 );
			STRBUF_Delete( str3 );
			STRBUF_Delete( temp );
			
			STRBUF_Delete( str_num );
			
			WORDSET_ClearAllBuffer( wset );	
		}
	}	

	GF_BGL_BmpWinOnVReq( win );	
	WORDSET_Delete( wset );
	
	tl->old = tl->now;
	
	return param;	
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
int TouchList_Main( TOUCH_LIST* tl, BR_WORK* wk )
{
	int i;
	int param = 0;
	GF_BGL_BMPWIN* win;
	STRBUF* 	str;
	
	///< �^�b�`����
	if ( tl->head->hit != NULL ){	
		for ( i = 0; i < tl->lim; i++ ){			
			if ( GF_TP_SingleHitTrg( &tl->head->hit[ i ] ) ){
				TouchAction( wk );
				param = tl->head->info[ tl->now + i ].param;
			}
		}
	}
	
//	if ( param )OS_Printf( "param = %d\n", param );
	
	if ( tl->now == tl->old ){ return param; }	
	if ( tl->lim == tl->max ){ return param; }
	
	win = &tl->win;
	
	GF_BGL_BmpWinDataFill( win, 0x00 );	
	{
		int no = 0;
		int s = tl->now;
		int e = s + tl->lim;
				
		if ( e >= tl->max ){
			 s = tl->max - tl->lim;
			 e = tl->max;
		}

		for ( i = s; i < e; i++ ){
			str = MSGMAN_AllocString( tl->man, tl->head->info[ i ].msg );
			GF_STR_PrintColor( win, FONT_SYSTEM, str, 0, ( tl->head->list_height * 16 ) * no, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			STRBUF_Delete( str );			
			no++;
		}		
	}
	GF_BGL_BmpWinOn( win );	

	tl->old = tl->now;

	return param;
}


//--------------------------------------------------------------
/**
 * @brief	���͈̓��[�h
 *
 * @param	tl	
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
int TouchList_ViewMode_Main( TOUCH_LIST* tl, BR_WORK* wk )
{
	int i;
	int param = 0;
	GF_BGL_BMPWIN* win;
	STRBUF* 	str;
	
	if ( tl->view.this == tl->old ){ return param; }	
	if ( tl->lim == tl->max ){ return param; }
	
	win = &tl->win;

	GF_BGL_BmpWinDataFill( win, 0x00 );	
	{
		int i;
		int s = 0;
		int win_no;
		STRBUF* str1;
		STRBUF*	str_num;
		STRBUF* str_colon;
		int width;
		int e;
				
		if ( tl->view.l_pos >= tl->view.t_stoper ){
			s = tl->view.this - tl->view.t_stoper;
			if ( tl->view.this >= ( tl->max - ( tl->view.t_stoper + 1 ) ) ){
				s = tl->max - tl->lim;
			}
		}
		
	//	OS_Printf( "s = %d\n", s );
				
		for ( i = 0; i < tl->lim; i++ ){
			str1 	= STRBUF_Create( 255, HEAPID_BR );
			str_num = STRBUF_Create( 255, HEAPID_BR );
			str_colon = MSGMAN_AllocString( wk->sys.man, msg_607 );
			
			STRBUF_SetNumber( str_num, ( s + i + 1 ), 2, NUMBER_DISPTYPE_SPACE, NUMBER_CODETYPE_DEFAULT);
			STRBUF_SetStringCode( str1, wk->recv_data.outline_data_tmp[ s + i ]->profile.name );
			BR_ErrorStrChange( wk, str1 );
			width = FontProc_GetPrintStrWidth( FONT_SYSTEM, str_num, 0 );

			GF_STR_PrintColor( win, FONT_SYSTEM, str_num, 	16 - width, 4 + ( i * 24 ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_STR_PrintColor( win, FONT_SYSTEM, str_colon, 16, 4 + ( i * 24 ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_STR_PrintColor( win, FONT_SYSTEM, str1, 		22, 4 + ( i * 24 ), MSG_NO_PUT, PRINT_COL_NORMAL, NULL );

			STRBUF_Delete( str1 );			
			STRBUF_Delete( str_num );		
			STRBUF_Delete( str_colon );
		}
	}
	GF_BGL_BmpWinOn( win );

	tl->old = tl->view.this;

	return param;	
}


//--------------------------------------------------------------
/**
 * @brief	���͈̓��[�h�̒l�ݒ�
 *
 * @param	tl	
 * @param	val	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchList_ViewMode_ValueSet( TOUCH_LIST* tl, s16 val )
{
	s16 pos  = 0;
	int move = 0xFF;
	
	if ( val < 0 ){
		///< ��
		if ( tl->view.this > 0 ){
			 tl->view.this--;
			move = 0;
			Snd_SePlay( eBR_SND_SCROLL );
		}
	}
	else if ( val > 0 ){
		///< ��
		if ( tl->view.this < tl->max - 1 ){
			 tl->view.this++;
			move = 1;
			Snd_SePlay( eBR_SND_SCROLL );
		}
	}
	
	switch ( move ){
	case 0:
		if ( tl->view.this < tl->view.b_stoper ){
			pos = tl->view.this;
		}
		else if ( tl->view.this >= ( tl->max - tl->view.b_stoper ) ){
			pos = tl->view.b_stoper + 1;
			if ( tl->view.this >= ( tl->max - tl->view.b_stoper ) ){
				pos += ( tl->view.this - ( tl->max - tl->view.b_stoper ) );
			}
		}
		else {
			pos = tl->view.b_stoper;
			if ( tl->view.this <= tl->view.b_stoper ){
				pos += ( tl->view.this - tl->view.b_stoper );
			}
		}
		break;

	case 1:
		if ( tl->view.this <= tl->view.t_stoper ){
			pos = tl->view.this;
		}
		else {
			pos = tl->view.t_stoper;			 
			if ( tl->view.this >= ( tl->max - tl->view.t_stoper ) ){
				pos += ( tl->view.this - ( tl->max - tl->view.t_stoper - 1 ) );
			}
		}
		break;
	}

	if ( pos < 0 ){ GF_ASSERT( 0 ); }
		 
	if ( move != 0xFF ){
		tl->view.l_pos = pos;
//		OS_Printf( "pos   = %d\n", pos );
//		OS_Printf( "l_pos = %d\n", tl->view.l_pos );
//		OS_Printf( "this  = %d\n", tl->view.this );
//		OS_Printf( "max   = %d\n", tl->max );
	}
	{
		s16 s = 0;
		if ( tl->view.l_pos >= tl->view.t_stoper ){
			s = tl->view.this - tl->view.t_stoper;
			if ( tl->view.this >= ( tl->max - ( tl->view.t_stoper + 1 ) ) ){
				s = tl->max - tl->lim;
			}
		}
		tl->now = s;
//		OS_Printf( "now   = %d\n", pos );
//		OS_Printf( "l_pos = %d\n", tl->view.l_pos );
		TouchList_ViewModeCursorEnableCheck( tl );
	}
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	tl	
 * @param	val	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchList_ValueSet( TOUCH_LIST* tl, s16 val )
{
	s16 old = tl->now;
	
	tl->now += val;
	
	if ( tl->now < 0 ){
		 tl->now = 0;
	}	
	if ( tl->lim == tl->max ){
		 tl->now = 0;
	}	
	if ( tl->now + tl->lim > tl->max ){
		 tl->now = tl->max - tl->lim;
	}
	
	if ( old != tl->now ){
		 Snd_SePlay( eBR_SND_SCROLL );
	}
	
	TouchList_CursorEnableCheck( tl );
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�̏I��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchList_Exit( TOUCH_LIST* wk )
{
	GF_BGL_BmpWinOff( &wk->win );
	GF_BGL_BmpWinDel( &wk->win );
	
//	memset( wk, 0, sizeof( TOUCH_LIST ) );
}

void TouchList_WorkClear( TOUCH_LIST* wk )
{
	memset( wk, 0, sizeof( TOUCH_LIST ) );	
}

// =============================================================================
//
//
//	�����X�g�p�J�[�\��
//
//
// =============================================================================
void TouchList_CursorAdd( TOUCH_LIST* tl, BR_WORK* br_wk, int d_area )
{
	CATS_ACT_PTR cap;
	TCATS_OBJECT_ADD_PARAM_S coap;
	
	ARCHANDLE*		 hdl = br_wk->sys.p_handle;
	GF_BGL_INI*		 bgl = br_wk->sys.bgl;
	CATS_SYS_PTR	 csp = br_wk->sys.csp;
	CATS_RES_PTR	 crp = br_wk->sys.crp;
	PALETTE_FADE_PTR pfd = br_wk->sys.pfd;
	
	CATS_LoadResourceCharArcH( csp, crp, hdl, NARC_batt_rec_gra_batt_rec_cursor_NCGR, FALSE, d_area,  eID_LIST_CUR );
	CATS_LoadResourceCellArcH( csp, crp, hdl, NARC_batt_rec_gra_batt_rec_cursor_NCER, FALSE, eID_LIST_CUR );
	CATS_LoadResourceCellAnmArcH( csp, crp, hdl, NARC_batt_rec_gra_batt_rec_cursor_NANR, FALSE, eID_LIST_CUR );
	
	coap.x		= 128;
	coap.y		=  96;
	coap.z		= 0;		
	coap.anm	= 0;
	coap.pri	= 1;
	coap.pal	= 0;
	coap.d_area = d_area;
	coap.bg_pri = eBR_BG0_PRI;
	coap.vram_trans = 0;
	
	coap.id[4] = CLACT_U_HEADER_DATA_NONE;
	coap.id[5] = CLACT_U_HEADER_DATA_NONE;
	
	coap.id[0]		= eID_LIST_CUR;
	coap.id[2]		= eID_LIST_CUR;
	coap.id[3]		= eID_LIST_CUR;
	if ( d_area == NNS_G2D_VRAM_TYPE_2DMAIN ){
		coap.id[1]		= eID_OAM_SIDE_BAR_M;
	}
	else {
		coap.id[1]		= eID_OAM_SIDE_BAR_S;
	}
	
	tl->cap[ 0 ] = CATS_ObjectAdd_S( br_wk->sys.csp, br_wk->sys.crp, &coap );
	tl->cap[ 1 ] = CATS_ObjectAdd_S( br_wk->sys.csp, br_wk->sys.crp, &coap );
	CATS_ObjectPaletteSetCap( tl->cap[ 0 ], 2 );
	CATS_ObjectPaletteSetCap( tl->cap[ 1 ], 2 );
	CATS_ObjectAnimeSeqSetCap( tl->cap[ 0 ], 0 );
	CATS_ObjectAnimeSeqSetCap( tl->cap[ 1 ], 0 );
	CATS_ObjectUpdateCap( tl->cap[ 0 ] );
	CATS_ObjectUpdateCap( tl->cap[ 1 ] );
	CATS_ObjectFlipSetCap( tl->cap[ 1 ], CLACT_FLIP_V );
	
	if ( d_area == CATS_D_AREA_MAIN ){
		CATS_ObjectPosSetCap( tl->cap[ 0 ], 256 / 2,  24 );
		CATS_ObjectPosSetCap( tl->cap[ 1 ], 256 / 2, 120 );
	}
	else {
		CATS_ObjectPosSetCap( tl->cap[ 0 ], 256 / 2,  24 + 64 );
		CATS_ObjectPosSetCap( tl->cap[ 1 ], 256 / 2, 120 + 64 );
	}
	TouchList_CursorEnableCheck( tl );
}

void TouchList_CursorPosSet( TOUCH_LIST* tl, s16 ty, s16 by )
{
	CATS_ObjectPosSetCap( tl->cap[ 0 ], 256 / 2, ty );
	CATS_ObjectPosSetCap( tl->cap[ 1 ], 256 / 2, by );	
}

void TouchList_CursorEnableCheck( TOUCH_LIST* tl )
{
	CATS_ObjectEnableCap( tl->cap[ 0 ], TRUE );
	CATS_ObjectEnableCap( tl->cap[ 1 ], TRUE );
	if ( tl->now == 0 ){
		CATS_ObjectEnableCap( tl->cap[ 0 ], FALSE );
	}
	if ( tl->now + tl->lim >= tl->max ){
		CATS_ObjectEnableCap( tl->cap[ 1 ], FALSE );
	}
	CATS_ObjectUpdateCapEx( tl->cap[ 0 ] );
	CATS_ObjectUpdateCapEx( tl->cap[ 1 ] );
}

void TouchList_ViewModeCursorEnableCheck( TOUCH_LIST* tl )
{
	CATS_ObjectEnableCap( tl->cap[ 0 ], TRUE );
	CATS_ObjectEnableCap( tl->cap[ 1 ], TRUE );
	if ( tl->now == 0 ){
		CATS_ObjectEnableCap( tl->cap[ 0 ], FALSE );
	}
//	if ( tl->now >= tl->lim ){
	if ( tl->now + tl->lim >= tl->max ){
		CATS_ObjectEnableCap( tl->cap[ 1 ], FALSE );
	}
	if ( tl->lim >= tl->max ){
		CATS_ObjectEnableCap( tl->cap[ 0 ], FALSE );
		CATS_ObjectEnableCap( tl->cap[ 1 ], FALSE );
	}
	CATS_ObjectUpdateCapEx( tl->cap[ 0 ] );
	CATS_ObjectUpdateCapEx( tl->cap[ 1 ] );
}

void TouchList_CursorDel( TOUCH_LIST* tl, BR_WORK* wk )
{
	CATS_FreeResourceChar( wk->sys.crp, eID_LIST_CUR );
	CATS_FreeResourceCell( wk->sys.crp, eID_LIST_CUR );
	CATS_FreeResourceCellAnm( wk->sys.crp, eID_LIST_CUR );
	CATS_ActorPointerDelete_S( tl->cap[ 0 ] );
	CATS_ActorPointerDelete_S( tl->cap[ 1 ] );
}

// =============================================================================
//
//
//	���X���C�_�[����
//
//
// =============================================================================
//--------------------------------------------------------------
/**
 * @brief	�X���C�_�[�̓o�^
 *
 * @param	wk	
 * @param	br_wk	
 * @param	d_area	
 *
 * @retval	static CATS_ACT_PTR	
 *
 */
//--------------------------------------------------------------
static CATS_ACT_PTR Slider_Add( BR_SLIDER* wk, BR_WORK* br_wk, int d_area, int anmno )
{
	CATS_ACT_PTR cap;
	TCATS_OBJECT_ADD_PARAM_S coap;
	
	coap.x		= 128;
	coap.y		=  96;
	coap.z		= 0;		
	coap.anm	= 0;
	coap.pri	= 1;
	coap.pal	= 0;
	coap.d_area = d_area;
	coap.bg_pri = eBR_BG0_PRI;
	coap.vram_trans = 0;
	
	coap.id[4] = CLACT_U_HEADER_DATA_NONE;
	coap.id[5] = CLACT_U_HEADER_DATA_NONE;
	
	coap.id[0]		= eID_OAM_EX_TAG;
	coap.id[2]		= eID_OAM_EX_TAG;
	coap.id[3]		= eID_OAM_EX_TAG;
	if ( d_area == NNS_G2D_VRAM_TYPE_2DMAIN ){
		coap.id[1]		= eID_OAM_SIDE_BAR_M;
	}
	else {
		coap.id[1]		= eID_OAM_SIDE_BAR_S;
	}	
	cap = CATS_ObjectAdd_S( br_wk->sys.csp, br_wk->sys.crp, &coap );	
	CATS_ObjectPaletteSetCap( cap, 2 );
	CATS_ObjectAnimeSeqSetCap( cap, anmno );
	CATS_ObjectUpdateCap( cap );
	
	return cap;	
}

//--------------------------------------------------------------
/**
 * @brief	�X���C�_�[�̍폜
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void Slider_Del( BR_SLIDER* wk )
{
	CATS_ActorPointerDelete_S( wk->cap[ 0 ] );
	CATS_ActorPointerDelete_S( wk->cap[ 1 ] );
}

//--------------------------------------------------------------
/**
 * @brief	�X���C�_�[�̔�\��
 *
 * @param	wk	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void Slider_Enable( BR_SLIDER* wk, int flag )
{
	CATS_ObjectEnableCap( wk->cap[ 0 ], flag );
	CATS_ObjectEnableCap( wk->cap[ 1 ], flag );
}

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Slider_Init( BR_SLIDER* wk, BR_WORK* br_wk )
{	
	Slider_InitEx( wk, br_wk, eTAG_SLIDER );
}

void Slider_InitEx( BR_SLIDER* wk, BR_WORK* br_wk, int anmno )
{
	wk->cap[ 0 ] = Slider_Add( wk, br_wk, CATS_D_AREA_SUB, anmno );
	wk->cap[ 1 ] = Slider_Add( wk, br_wk, CATS_D_AREA_SUB, anmno );

	CATS_ObjectPosSetCap( wk->cap[ 0 ],   0 + 24, 72 + 64 );
	CATS_ObjectPosSetCap( wk->cap[ 1 ], 256 - 24, 72 + 64 );
}

void Slider_WorkClear( BR_SLIDER* wk )
{
	memset( wk, 0, sizeof( BR_SLIDER ) );	
}

//--------------------------------------------------------------
/**
 * @brief	�X���C�_�[�I������
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Slider_Exit( BR_SLIDER* wk )
{
	Slider_Del( wk );
//	memset( wk, 0, sizeof( BR_SLIDER ) );
}

//--------------------------------------------------------------
/**
 * @brief	�f�[�^�̎擾
 *
 * @param	wk	
 * @param	check	
 *
 * @retval	s16	
 *
 */
//--------------------------------------------------------------
void Slider_Value_CheckSet( BR_SLIDER* wk, u32 y )
{
	s32 check = y - wk->old_y;
	
//	if ( wk->value < 0 && check > 0 ){ wk->value = 0; }
//	if ( wk->value > 0 && check < 0 ){ wk->value = 0; }	
	if ( ( check * check ) >= ( BB_SLIDER_MOVE_VALUE * BB_SLIDER_MOVE_VALUE ) ) {		///< ���ʂ̓��삪�������ꍇ
		
		if ( check < 0 ){
			wk->value = -1;						///< �ړ��l�Ƃ��Đݒ肷��
		}
		else {
			wk->value = +1;
		}
		wk->old_y = y;							///< ���ݒn����_�ɒu���Ȃ���
	}
	else {
		wk->value = 0;
	}	
//	if ( wk->value != 0 ){ OS_Printf( "%d\n",wk->value ); }
}

///< �X���C�_�[�̓����蔻��
static const TP_HIT_TBL slider_hit_tbl[] = {
	{ 8, 128,   0 +  8,   0 + 32 },
	{ 8, 128, 256 - 32, 256 -  8 },
};


//--------------------------------------------------------------
/**
 * @brief	�A�j��
 *
 * @param	wk	
 * @param	lim	
 * @param	max	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Slider_AnimeCheck( BR_SLIDER* wk, TOUCH_LIST* tl )
{
	if ( tl->bView ){
	//	OS_Printf( "�r���[���[�h\n" );
		if ( tl->max < 2 ){
			wk->bAnime = FALSE;
		}
		else {
			wk->bAnime = TRUE;
		}		
	}
	else {
	//	OS_Printf( "�ʏ탊�X�g���[�h\n" );
		if ( tl->lim >= tl->max ){
			wk->bAnime = FALSE;
		}
		else {
			wk->bAnime = TRUE;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief	�X���C�_�[�̓����蔻��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void Slider_HitCheck_Main( BR_SLIDER* wk )
{
	u32  x, y;
	s16  check;
	BOOL bCont;
	BOOL bSliderHit = FALSE;
	
	bCont = GF_TP_GetPointCont( &x, &y );

	if ( wk->bAnime ){
		CATS_ObjectUpdateCapEx( wk->cap[ 0 ] );
		CATS_ObjectUpdateCapEx( wk->cap[ 1 ] );
	}

	if ( wk->bHit == FALSE && bCont == FALSE ){			///< �G�������Ȃ� �� �G���ĂȂ�
	
		wk->bHit  = FALSE;
		wk->frame = 0;
		wk->old_x = 0;
		wk->old_y = 0;			
		wk->value = 0;
		return;
	}
	
	if ( GF_TP_SingleHitCont( &slider_hit_tbl[ 0 ] )		///< �X���C�_�[�Ƃ̓����蔻��
	||	 GF_TP_SingleHitCont( &slider_hit_tbl[ 1 ] ) ){
		bSliderHit = TRUE;
	}
	
	if ( bSliderHit ){
		if ( !wk->bHit ){									///< �t���[��0�Ȃ珉����
			wk->bHit  = TRUE;
			wk->old_x = x;
			wk->old_y = y;
			wk->frame = BB_SLIDER_HIT_FRAME;
//			OS_Printf( "���炢���[�G����\n");
		}
		else {
			wk->frame--;
			Slider_Value_CheckSet( wk, y );			///< �ړ��ʂ̎擾
//			OS_Printf( "���炢���[�G���Ă�\n");
		}
	}
	else {
		if ( wk->bHit ){
		//	Slider_Value_CheckSet( wk, y );			///< �ړ��ʂ̎擾			
			wk->bHit  = FALSE;
			wk->frame = 0;
			wk->old_x = 0;
			wk->old_y = 0;
//			OS_Printf( "���炢���[�͂Ȃ���\n");
		}
	}
}



//--------------------------------------------------------------
/**
 * @brief	�X�R�A���Z
 *
 * @param	wk	
 * @param	id	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_AddScore( BR_WORK* wk, u32 id )
{
	RECORD* rec;	
	
	rec = SaveData_GetRecord( wk->save );
	
	RECORD_Score_Add( rec, id );
}

//--------------------------------------------------------------
/**
 * @brief	brs �𐶐�
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void RecData_To_brs( BR_WORK* wk )
{
//	VIDEO_SEARCH_WORK* vwk = wk->sub_work;
	LOAD_RESULT result;
	
	BattleRec_Init( wk->save, HEAPID_BR_MANAGER, &result );
	{
//		GDS_PROFILE_PTR			profile	= ( GDS_PROFILE_PTR )( &vwk->rec_data->profile );
//		BATTLE_REC_HEADER_PTR	head	= ( BATTLE_REC_HEADER_PTR )( &vwk->rec_data->head );
//		BATTLE_REC_WORK_PTR		rec		= ( BATTLE_REC_WORK_PTR )( &vwk->rec_data->rec );
		GDS_PROFILE_PTR			profile	= ( GDS_PROFILE_PTR )( &wk->recv_data.rec_data_tmp->profile );
		BATTLE_REC_HEADER_PTR	head	= ( BATTLE_REC_HEADER_PTR )( &wk->recv_data.rec_data_tmp->head );
		BATTLE_REC_WORK_PTR		rec		= ( BATTLE_REC_WORK_PTR )( &wk->recv_data.rec_data_tmp->rec );
		
		BattleRec_DataSet( profile, head, rec, NULL, wk->save );
	}
//	BattleRec_Load( wk->save, HEAPID_BASE_APP, &result, NULL, 0 );	
//	OS_Printf( "brs_load result = %d\n", result );
}


//--------------------------------------------------------------
/**
 * @brief	DISP�؂�ւ�TCB
 *
 * @param	tcb	
 * @param	work
 * @retval	none
 *
 */
//--------------------------------------------------------------
static void DispChange_TCB( TCB_PTR tcb, void* work )
{
	BR_DISP_SW* wk = work;
	
	BR_ChangeDisplay( wk->disp_sw );
	WirelessIconOAM_Trans();
	wk->bChange = TRUE;
	
	TCB_Delete( tcb );
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 * @param	mode	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_ChangeDisplayVIntr( BR_WORK* wk, int mode )
{
	wk->disp.disp_sw = mode;
	wk->disp.bChange = FALSE;
	
	VIntrTCB_Add( DispChange_TCB, &wk->disp, eVINTR_TCB_PRI_CHANGE );
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL BR_IsChangeDisplay( BR_WORK* wk )
{
	return wk->disp.bChange;
}


//--------------------------------------------------------------
/**
 * @brief	�`��
 *
 * @param	tcb	
 * @param	work	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void Draw_60fps_TCB( TCB_PTR tcb, void* work )
{
	BR_WORK* wk = work;
	
	/// �`��t���O �`�F�b�N
	/// ���d�N���t���O �`�F�b�N
	if ( wk->sys.crp == NULL ){ return; }
	
	if ( IsCATS_SystemActive( wk ) == TRUE ){
	//	OS_Printf( " CATS���쒆�ɗ��� \n" );
		return;
	}
	
	if ( wk->sys.bDraw == TRUE ){ 
	//	OS_Printf( " ���Ԓ��ɗ��� \n" );
		return;
	}
	wk->sys.bDraw = TRUE;

	SoftSpriteTextureTrans( wk->sys.ssm_p );

	CATS_Draw( wk->sys.crp );
//	OS_Printf( " V�J�E���^�@%d\n ", GX_GetVCount() );
	CATS_RenderOamTrans();
//	PaletteFadeTrans( wk->sys.pfd );

	wk->sys.bDraw = FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�`��TCB
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_DrawTCB_Add( BR_WORK* wk )
{
	wk->view_tcb = VIntrTCB_Add( Draw_60fps_TCB, wk, eVINTR_TCB_PRI_BAR );
}

//--------------------------------------------------------------
/**
 * @brief	�Z�[�u���� brs ����
 *
 * @param	wk	
 * @param	no	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
BOOL RecData_Save( BR_WORK* wk, int no, u8 secure )
{
	SAVE_RESULT result;
	
//	brs->head.secure
#if 0
	if ( wk->ex_param3 == TRUE ){
		///< �Đ����Ă��Ȃ�
		result = BattleRec_GDS_RecvData_Save( wk->save, no, FALSE, &wk->save_work1, &wk->save_work2 );
	}
	else {
		///< �Đ��������Ƃ�����
		result = BattleRec_GDS_RecvData_Save( wk->save, no, TRUE,  &wk->save_work1, &wk->save_work2 );
	}
#endif
	result = BattleRec_GDS_RecvData_Save( wk->save, no, secure, &wk->save_work1, &wk->save_work2 );
		
	if ( result == SAVE_RESULT_CONTINUE || result == SAVE_RESULT_LAST ){
		return FALSE;
	}
	
	return TRUE;
}


//--------------------------------------------------------------
/**
 * @brief	�t���O�ݒ�
 *
 * @param	wk	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void RecData_RecStopFlagSet( BR_WORK* wk, BOOL flag )
{
	OS_Printf( " �r�f�I���Ƃ��Ă��āA�܂����Ăˁ[�t���O = ON\n" );
	/// TRUE = ON = �݂Ăˁ[
	wk->ex_param3 = flag;
}

//--------------------------------------------------------------
/**
 * @brief	30���̃f�[�^�擾���ʃ��X�|���X�R�[���o�b�N
 *
 * @param	work	
 * @param	error_info	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Response_BattleVideoSearch(void *work, const GDS_RAP_ERROR_INFO *error_info)
{
	BR_WORK* wk = work;	
	BR_RECV_DATA* recv_data = &wk->recv_data;
	
	OS_Printf( "�^��f�[�^�R�O����M\n");
	if( error_info->occ == TRUE ){
		OS_Printf( "�G���[�I\n");
	}
	else {
		OS_Printf( "����Ɏ擾�I\n");
		recv_data->outline_data_num = GDS_RAP_RESPONSE_BattleVideoSearch_Download_RecvPtr_Set( BR_GDSRAP_WorkGet( wk ), recv_data->outline_data_tmp, 30 );
		
		{
			int i;
			
			for ( i = 0; i < recv_data->outline_data_num; i++ ){
				GF_ASSERT_MSG( wk->recv_data.outline_data_tmp[ i ] != NULL, "%d�Ԗڂɋ���ۂ̃f�[�^����M������\n",i );
				MI_CpuCopy8( wk->recv_data.outline_data_tmp[ i ], &wk->recv_data.outline_data[ i ], sizeof( GT_BATTLE_REC_OUTLINE_RECV ) );
				wk->recv_data.outline_secure_flag[ i ] = wk->recv_data.outline_data_tmp[ i ]->head.secure;
			//	OS_Printf( " secure flag = %d\n", wk->recv_data.outline_secure_flag[ i ] );
			}
			
		}
	}
	
	OS_Printf( "���� = %d\n",recv_data->outline_data_num);
}

//--------------------------------------------------------------
/**
 * @brief	�f�[�^�̓ǂݍ���
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_SaveData_GppHead_Load( BR_WORK* wk )
{
	int i;
	LOAD_RESULT result;	
	for ( i = 0; i < 4; i++ ){		
		BattleRec_Load( wk->save, HEAPID_BR, &result, NULL, i );		
		switch ( result ){
		case RECLOAD_RESULT_NULL:
			 wk->br_head[ i ] = NULL;
			 wk->br_gpp[ i ]  = NULL;
			 if ( i == 0 ){
				wk->br_gpp[ i ] = GDS_Profile_AllocMemory( HEAPID_BR );
				GDS_Profile_MyDataSet( wk->br_gpp[ i ], wk->save );
			 }
			break;
		case RECLOAD_RESULT_NG:
			 wk->br_head[ i ] = NULL;
			 wk->br_gpp[ i ]  = NULL;
			 if ( i == 0 ){
				wk->br_gpp[ i ] = GDS_Profile_AllocMemory( HEAPID_BR );
				GDS_Profile_MyDataSet( wk->br_gpp[ i ], wk->save );
			 }
			break;
		case RECLOAD_RESULT_ERROR:
			 wk->br_head[ i ] = NULL;
			 wk->br_gpp[ i ]  = NULL;
			 if ( i == 0 ){
				wk->br_gpp[ i ] = GDS_Profile_AllocMemory( HEAPID_BR );
				GDS_Profile_MyDataSet( wk->br_gpp[ i ], wk->save );
			 }
			break;
		case RECLOAD_RESULT_OK:
			 wk->br_head[ i ] = BattleRec_HeaderAllocCopy( HEAPID_BR );			
			 if ( i == 0 ){
				wk->br_gpp[ i ] = GDS_Profile_AllocMemory( HEAPID_BR );
				GDS_Profile_MyDataSet( wk->br_gpp[ i ], wk->save );
			 }
			 else {				
				wk->br_gpp[ i ] = BattleRec_GDSProfileAllocCopy( HEAPID_BR );
			 }
			break;			
		}				
		if ( BattleRec_DataExistCheck() == TRUE ){
			BattleRec_Exit();	
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief	�폜
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_SaveData_GppHead_Delete( BR_WORK* wk )
{
	int i;

	for ( i = 0; i < 4; i++ ){
		
		if ( wk->br_head[ i ] != NULL ){
			sys_FreeMemoryEz( wk->br_head[ i ] );
			wk->br_head[ i ] = NULL;
		}
		
		if ( wk->br_gpp[ i ] != NULL ){
			sys_FreeMemoryEz( wk->br_gpp[ i ] );
			wk->br_gpp[ i ] = NULL;
		}
	}	
}


// =============================================================================
//
//
//	���|�P�����A�C�R��
//
//
// =============================================================================
///< �L�����ȊO
void PokemonIcon_ResLoad( BR_WORK* wk )
{
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	
	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;
	
	CATS_LoadResourcePlttWorkArc( pfd, FADE_MAIN_OBJ, csp, crp,	ARC_POKEICON, PokeIconPalArcIndexGet(), 0, POKEICON_PAL_MAX, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON );
	CATS_LoadResourceCellArc( csp, crp, ARC_POKEICON, PokeIcon64kCellArcIndexGet(), 0, eID_OAM_POKE_ICON );
	CATS_LoadResourceCellAnmArc( csp, crp, ARC_POKEICON, PokeIcon64kCellAnmArcIndexGet(), 0, eID_OAM_POKE_ICON );
}

void PokemonIcon_ResFree( BR_WORK* wk )
{
	CATS_FreeResourcePltt( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourceCell( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourceCellAnm( wk->sys.crp, eID_OAM_POKE_ICON );
}

CATS_ACT_PTR PokemonIcon_Add( BR_WORK* wk, int id, int monsno, int form, int egg )
{
	CATS_ACT_PTR cap;
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	TCATS_OBJECT_ADD_PARAM_S coap;
	int palno;
	
	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;
		
	if ( monsno == 0 ){ return NULL; }
	
	CATS_LoadResourceCharArcModeAdjustAreaCont(	csp, crp, ARC_POKEICON, PokeIconCgxArcIndexGetByMonsNumber( monsno, egg, form ), 0, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON + id );
		
	coap.x		= 0;
	coap.y		= 0;
	coap.z		= 0;
	coap.anm	= 0;
	coap.pri	= 0;
	coap.pal	= 0;
	coap.d_area = CATS_D_AREA_MAIN;
	coap.bg_pri = 0;
	coap.vram_trans = 0;

	coap.id[ 0 ] = eID_OAM_POKE_ICON + id;
	coap.id[ 1 ] = eID_OAM_POKE_ICON;
	coap.id[ 2 ] = eID_OAM_POKE_ICON;
	coap.id[ 3 ] = eID_OAM_POKE_ICON;
	coap.id[ 4 ] = CLACT_U_HEADER_DATA_NONE;
	coap.id[ 5 ] = CLACT_U_HEADER_DATA_NONE;
	
	cap = CATS_ObjectAdd_S( csp, crp, &coap );
		
	palno  = PokeIconPalNumGet( monsno, form, egg );
	
	CATS_ObjectPaletteOffsetSetCap( cap, eOAM_PAL_POKE_ICON + palno );
	
	CATS_ObjectAnimeSeqSetCap( cap, POKEICON_ANM_HPMAX );
	
	CATS_ObjectUpdateCap( cap );

	return cap;	
}


//--------------------------------------------------------------
/**
 * @brief	�|�P�����̃A�[�J�C�u���J��
 *
 * @param	heap	
 *
 * @retval	ARCHANDLE*	
 *
 */
//--------------------------------------------------------------
ARCHANDLE* PokemonGraphic_ArcOpen( int heap )
{
	ARCHANDLE* hdl;
	
	hdl = ArchiveDataHandleOpen( ARC_POKEICON, HEAPID_BR );
	
	return hdl;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����̃A�[�J�C�u����
 *
 * @param	hdl	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void PokemonGraphic_ArcClose( ARCHANDLE* hdl )
{
	ArchiveDataHandleClose( hdl );	
}

CATS_ACT_PTR PokemonIcon_AddHDL( BR_WORK* wk, ARCHANDLE* hdl, int id, int monsno, int form, int egg )
{
	CATS_ACT_PTR cap;
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	TCATS_OBJECT_ADD_PARAM_S coap;
	int palno;
	
	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;
		
	if ( monsno == 0 ){ return NULL; }
	
	CATS_LoadResourceCharArcModeAdjustAreaCont_Handle( csp, crp, hdl, PokeIconCgxArcIndexGetByMonsNumber( monsno, egg, form ), 0, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON + id );
		
	coap.x		= 0;
	coap.y		= -48;
	coap.z		= 0;
	coap.anm	= 0;
	coap.pri	= 0;
	coap.pal	= 0;
	coap.d_area = CATS_D_AREA_MAIN;
	coap.bg_pri = 0;
	coap.vram_trans = 0;

	coap.id[ 0 ] = eID_OAM_POKE_ICON + id;
	coap.id[ 1 ] = eID_OAM_POKE_ICON;
	coap.id[ 2 ] = eID_OAM_POKE_ICON;
	coap.id[ 3 ] = eID_OAM_POKE_ICON;
	coap.id[ 4 ] = CLACT_U_HEADER_DATA_NONE;
	coap.id[ 5 ] = CLACT_U_HEADER_DATA_NONE;
	
	cap = CATS_ObjectAdd_S( csp, crp, &coap );
		
	palno  = PokeIconPalNumGet( monsno, form, egg );
	
	CATS_ObjectPaletteOffsetSetCap( cap, eOAM_PAL_POKE_ICON + palno );
	
	CATS_ObjectAnimeSeqSetCap( cap, POKEICON_ANM_HPMAX );
	
	CATS_ObjectUpdateCap( cap );

	return cap;	
}

void PokemonIcon_Del( BR_WORK* wk, int id, CATS_ACT_PTR cap )
{	
	int i;

	if ( cap == NULL){ return; }
		
	CATS_FreeResourceChar( wk->sys.crp, eID_OAM_POKE_ICON + id );
	CATS_ActorPointerDelete_S( cap );
}


void PokemonIcon_DelActor( BR_WORK* wk, int id, CATS_ACT_PTR cap )
{	
	int i;

	if ( cap == NULL){ return; }
		
	CATS_ActorPointerDelete_S( cap );
}

// =============================================================================
//
//
//	���r�f�I���`��V�X�e��
//
//
// =============================================================================
static void video_hero_add( VIDEO_WORK* vwk, BR_WORK* wk )
{
	int sex;
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	TCATS_OBJECT_ADD_PARAM_S coap;
	
	int	ncl = NARC_wifi2dchar_hero_NCLR;
	int ncg = NARC_wifi2dchar_hero_NCGR;
	int	nce = NARC_wifi2dchar_npc_NCER;
	int nca = NARC_wifi2dchar_npc_NANR;
	
	sex = GDS_Profile_GetSex( vwk->gpp );

	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;
	
	if ( wk->ex_param1 == LOADDATA_MYREC ){
		
		if ( sex == PM_FEMALE ){
			ncl = NARC_wifi2dchar_heroine_NCLR;
			ncg = NARC_wifi2dchar_heroine_NCGR;
		}
	}
	else {
		int view = GDS_Profile_GetTrainerView( vwk->gpp );
		
	//	OS_Printf( " view = %d\n", view );
		
		if ( view >= BR_UNION_CHAR_MAX ){ view = 0; }
		
		ncl = WiFi_TR_DataGet( view, 2 );
		ncg = WiFi_TR_DataGet( view, 3 );
	}
	
	CATS_LoadResourcePlttWorkArc( pfd, FADE_MAIN_OBJ, csp, crp, ARC_WIFI2DCHAR, ncl, 0, 1, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_HERO_ICON );
	CATS_LoadResourceCellArc( csp, crp, ARC_WIFI2DCHAR, nce, 0, eID_OAM_HERO_ICON );
	CATS_LoadResourceCellAnmArc( csp, crp, ARC_WIFI2DCHAR, nca, 0, eID_OAM_HERO_ICON );
	CATS_LoadResourceCharArcModeAdjustAreaCont(	csp, crp, ARC_WIFI2DCHAR, ncg, 0, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_HERO_ICON );

	coap.x		= 40;
	coap.y		= 56+4;
	coap.z		= 0;		
	coap.anm	= 0;
	coap.pri	= 0;
	coap.pal	= 0;
	coap.d_area = CATS_D_AREA_MAIN;
	coap.bg_pri = 0;
	coap.vram_trans = 0;
			
	coap.id[0] = eID_OAM_HERO_ICON;
	coap.id[1] = eID_OAM_HERO_ICON;
	coap.id[2] = eID_OAM_HERO_ICON;
	coap.id[3] = eID_OAM_HERO_ICON;
	coap.id[4] = CLACT_U_HEADER_DATA_NONE;
	coap.id[5] = CLACT_U_HEADER_DATA_NONE;
	
	vwk->cap_hero = CATS_ObjectAdd_S( wk->sys.csp, wk->sys.crp, &coap );
	CATS_ObjectAnimeSeqSetCap( vwk->cap_hero, eHERO_BOTTOM );
	CATS_ObjectUpdateCap( vwk->cap_hero );
}

static void video_win_add( VIDEO_WORK* vwk, BR_WORK* wk )
{
	int i;
	int ofs;
	GF_BGL_BMPWIN* win;
	STRBUF*	str;

	{
		s16 win_p[][4] = {
			{  4, 4, 24, 2 },
			{  4, 6, 24, 4 },
			{  4,11,  5, 2 },
			{  4,13,  5, 2 },
			{  4,21, 24, 2 },
		};
		int mes_id[] = {
			msg_20, msg_20, 
			msg_20,	msg_11_2,
			msg_20,	msg_20,	
		};
		
		ofs = 1;
		for ( i = 0; i < 5; i++ ){
			
			if ( win_p[i][0] == 0xFF ){ break; }
			
			win = &vwk->win[ i ];				
			str = MSGMAN_AllocString( wk->sys.man, mes_id[ i ] );				
			GF_BGL_BmpWinInit( win );
			GF_BGL_BmpWinAdd( wk->sys.bgl, win, GF_BGL_FRAME2_M,
							  win_p[i][0], win_p[i][1], win_p[i][2], win_p[i][3], eBG_PAL_FONT, ofs );				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, str, BR_print_x_Get( win, str ), 0, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_BGL_BmpWinOnVReq( win );
			ofs += ( win_p[i][2] * win_p[i][3] );
			STRBUF_Delete( str );
		}
	//	OS_Printf( "win add char end %d\n", ofs );
	}
	
	///< �����`��
	{
		STRBUF* 	str1;
		STRBUF* 	str2;
		STRBUF*		tmp;
		WORDSET*	wset;
		GF_BGL_BMPWIN* win;
		GDS_PROFILE_PTR	gpp;
		
		gpp = vwk->gpp;
		wset = BR_WORDSET_Create( HEAPID_BR );
		
		///< ���o��
		win = &vwk->win[ 0 ];
		str1 = GDS_Profile_CreateNameString( gpp, HEAPID_BR );			///< ���O
		BR_ErrorStrChange( wk, str1 );
		str2 = MSGMAN_AllocString( wk->sys.man, msg_09 );			///< �` �̋L�^
		tmp  = STRBUF_Create( 255, HEAPID_BR );						///< �e���|����

		WORDSET_RegisterWord( wset, 0, str1, 0, TRUE, PM_LANG );	
		WORDSET_ExpandStr( wset, tmp, str2 );
	
		GF_BGL_BmpWinDataFill( win, 0x00 );
		GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
		GF_BGL_BmpWinOnVReq( win );

		STRBUF_Delete( str1 );
		STRBUF_Delete( str2 );
		STRBUF_Delete( tmp );
		WORDSET_ClearAllBuffer( wset );
		
		///< �{�ݖ�
		{
			int no = RecHeader_ParamGet( vwk->br_head, RECHEAD_IDX_MODE, 0 );
			win = &vwk->win[ 1 ];
			tmp = MSGMAN_AllocString( wk->sys.man, msg_rule_000 + no );	///< �����������
				
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, tmp, 0, 0, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_BGL_BmpWinOnVReq( win );

			STRBUF_Delete( tmp );
			WORDSET_ClearAllBuffer( wset );
		}
		
		
		///< �A��
		{
			int num = RecHeader_ParamGet( vwk->br_head, RECHEAD_IDX_COUNTER, 0);
			
			if ( num != 0 ){
				win = &vwk->win[ 2 ];
				str1 = STRBUF_Create( 255, HEAPID_BR );						///< ����
				str2 = MSGMAN_AllocString( wk->sys.man, msg_11 );			///< �����������
				tmp  = STRBUF_Create( 255, HEAPID_BR );						///< �e���|����
				
				STRBUF_SetNumber( str1, num, 4, NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT);
					
				WORDSET_RegisterWord( wset, 0, str1, 0, TRUE, PM_LANG );	
				WORDSET_ExpandStr( wset, tmp, str2 );
			
				GF_BGL_BmpWinDataFill( win, 0x00 );
				GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
				GF_BGL_BmpWinOnVReq( win );

				STRBUF_Delete( str1 );
				STRBUF_Delete( str2 );
				STRBUF_Delete( tmp );
				WORDSET_ClearAllBuffer( wset );
			}
			else {
				win = &vwk->win[ 3 ];
				GF_BGL_BmpWinDataFill( win, 0x00 );
				GF_BGL_BmpWinOnVReq( win );
			}
		}
		
		
		///< �f�[�^�i���o�[	
		{
			u64 data = RecHeader_ParamGet( vwk->br_head, RECHEAD_IDX_DATA_NUMBER, 0 );
			u64	dtmp1 = data;
			u32	dtmp2[ 3 ];
			STRBUF* num1 = STRBUF_Create( 255, HEAPID_BR );
			STRBUF* num2 = STRBUF_Create( 255, HEAPID_BR );
			STRBUF* num3 = STRBUF_Create( 255, HEAPID_BR );

			win = &vwk->win[ 4 ];

			dtmp2[ 0 ] = dtmp1 % 100000;	///< 3block
			dtmp1 /= 100000;
			dtmp2[ 1 ] = dtmp1 % 100000;	///< 2block
			dtmp1 /= 100000;
			dtmp2[ 2 ] = dtmp1;				///< 1block

			{
				int check1 = ( dtmp2[ 2 ] / 10 ) % 10;

				if ( ( check1 == 0 ) && ( data != 0 ) ){
					str2 = MSGMAN_AllocString( wk->sys.man, msg_12_2 );		///< �����������
					OS_Printf( "����ȃf�[�^�i���o�[\n");
				}
				else {
					str2 = MSGMAN_AllocString( wk->sys.man, msg_12 );		///< �����������
				}
			}
			tmp  = STRBUF_Create( 255, HEAPID_BR );						///< �e���|����
			
			STRBUF_SetNumber( num1, dtmp2[ 0 ], 5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);
			STRBUF_SetNumber( num2, dtmp2[ 1 ], 5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);
			STRBUF_SetNumber( num3, dtmp2[ 2 ], 2, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);
						
			WORDSET_RegisterWord( wset, 2, num1, 0, TRUE, PM_LANG );
			WORDSET_RegisterWord( wset, 1, num2, 0, TRUE, PM_LANG );
			WORDSET_RegisterWord( wset, 0, num3, 0, TRUE, PM_LANG );	
			WORDSET_ExpandStr( wset, tmp, str2 );
		
			OS_Printf( " %2d - %5d - %5d \n", dtmp2[ 2 ], dtmp2[ 1 ], dtmp2[ 0 ] );
					
			GF_BGL_BmpWinDataFill( win, 0x00 );
			GF_STR_PrintColor( win, FONT_SYSTEM, tmp, BR_print_x_Get( win, tmp ), 0, MSG_NO_PUT, PRINT_COL_NORMAL, NULL );
			GF_BGL_BmpWinOnVReq( win );

			STRBUF_Delete( num1 );
			STRBUF_Delete( num2 );
			STRBUF_Delete( num3 );
			STRBUF_Delete( str2 );
			STRBUF_Delete( tmp );
			WORDSET_ClearAllBuffer( wset );
		}
		
		WORDSET_Delete( wset );
	}	
}

static void video_win_del( VIDEO_WORK* vwk, BR_WORK* wk )
{
	int i;
	
	for ( i = 0; i < 5; i++ ){
		GF_BGL_BmpWinOff( &vwk->win[ i ] );
		GF_BGL_BmpWinDel( &vwk->win[ i ] );		
	}			
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	vwk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
BOOL VideoCheck2vs2_Head( BATTLE_REC_HEADER_PTR head )
{
	int	type;
	BOOL b2vs2 = FALSE; 
	
	type = RecHeader_ParamGet( head, RECHEAD_IDX_MODE, 0 );
	
	switch ( type ){
	case GT_BATTLE_MODE_TOWER_MULTI:
	case GT_BATTLE_MODE_FACTORY50_MULTI:
	case GT_BATTLE_MODE_FACTORY100_MULTI:
	case GT_BATTLE_MODE_STAGE_MULTI:
	case GT_BATTLE_MODE_CASTLE_MULTI:
	case GT_BATTLE_MODE_ROULETTE_MULTI:
	case GT_BATTLE_MODE_COLOSSEUM_MULTI:
		b2vs2 = TRUE;
		break;
	default:
		b2vs2 = FALSE;
		break;
	};
	
	return b2vs2;
}

//--------------------------------------------------------------
/**
 * @brief	2vs2�Ő�������̃`�F�b�N
 *
 * @param	vwk	
 *
 * @retval	static BOOL	
 *
 */
//--------------------------------------------------------------
static BOOL VideoCheck2vs2( VIDEO_WORK* vwk )
{	
	return VideoCheck2vs2_Head( vwk->br_head );
}

//--------------------------------------------------------------
/**
 * @brief	2vs2 BG �擾
 *
 * @param	vwk	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int ProfileBGGet( VIDEO_WORK* vwk )
{
	if ( VideoCheck2vs2( vwk ) == TRUE ){
		return NARC_batt_rec_gra_batt_rec_bg1ub_data_NSCR;
	}
	
	return NARC_batt_rec_gra_batt_rec_bg1ua_data_NSCR;
}

static void video_poke_add( VIDEO_WORK* vwk, BR_WORK* wk )
{	
	int i;
	int monsno;
	int form;
	int no = 0;
	
	TCATS_OBJECT_ADD_PARAM_S coap;
	int palno;
	int gra_id;
	int monsno_tbl[ 12 ];
	int form_tbl[ 12 ];
	int type;
	
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	PALETTE_FADE_PTR pfd;
	
	s16 pos_tbl[][ 12 ][ 2 ] = {
		{
			{ 11*8, 13*8 },{ 14*8, 13*8 },{ 17*8, 13*8 },{ 20*8, 13*8 },{ 23*8, 13*8 },{ 26*8, 13*8 },
			{  3*8, 19*8 },{  6*8, 19*8 },{  9*8, 19*8 },{ 12*8, 19*8 },{ 15*8, 19*8 },{ 18*8, 19*8 },
		},
		{
			{  3*8, 17*8 },{  6*8, 17*8 },{  9*8, 17*8 },	///< ����
			{  5*8, 20*8 },{  8*8, 20*8 },{ 11*8, 20*8 },	///< ����
			{ 18*8, 17*8 },{ 21*8, 17*8 },{ 24*8, 17*8 },	///< ����
			{ 20*8, 20*8 },{ 23*8, 20*8 },{ 26*8, 20*8 },	///< ����
		},
	};

	csp = wk->sys.csp;
	crp = wk->sys.crp;
	pfd = wk->sys.pfd;

	//�p���b�g
	CATS_LoadResourcePlttWorkArc( pfd, FADE_MAIN_OBJ, csp, crp,
								  ARC_POKEICON, PokeIconPalArcIndexGet(), 0, POKEICON_PAL_MAX, NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON );
	//�Z��
	{
		int arc = PokeIcon64kCellArcIndexGet();
		CATS_LoadResourceCellArc( csp, crp, ARC_POKEICON, arc, 0, eID_OAM_POKE_ICON );
	}
	//�Z���A�j��
	{
		int arc = PokeIcon64kCellAnmArcIndexGet();
		CATS_LoadResourceCellAnmArc( csp, crp, ARC_POKEICON, arc, 0, eID_OAM_POKE_ICON );
	}
	
	///< �e�[�u���̍쐬
	{	
		int temp_no = 0;
		int temp_monsno = 0;
		int temp_form = 0;
		int add = 6;
		int si = 0;
		int ei = add;
		
		type = 0;

		///< 2vs2�̏ꍇ
		if ( VideoCheck2vs2( vwk ) == TRUE ){
			add = 3;
			ei = 3;
			type = 1;
		}
		do {
			for ( i = si; i < ei; i++ ){
				monsno_tbl[ temp_no ] = 0xFF;
				
				temp_monsno = RecHeader_ParamGet( vwk->br_head, RECHEAD_IDX_MONSNO, i );
				temp_form	= RecHeader_ParamGet( vwk->br_head, RECHEAD_IDX_FORM_NO, i );

				if ( temp_monsno == 0 ){ continue; }
				
				monsno_tbl[ temp_no ] = temp_monsno;
				form_tbl[ temp_no]	  = temp_form;
				
				temp_no++;
			}
			for ( i = temp_no; i < ei; i++ ){	
				monsno_tbl[ temp_no ] = 0;
				form_tbl[ temp_no ]	  = 0;
				temp_no++;
			}		
			si += add;
			ei += add;
			
		} while ( ei <= 12 );
	}
	
	for ( i = 0; i < 12; i++ ){
				
		vwk->cap_poke[ i ] = NULL;
		
		monsno = monsno_tbl[ i ];
		
		if ( monsno == 0 ){ continue; }		///< ���܂�����
		
		form = form_tbl[ i ];
		
		gra_id = PokeIconCgxArcIndexGetByMonsNumber( monsno, 0, form );
		
		CATS_LoadResourceCharArcModeAdjustAreaCont(	csp, crp, ARC_POKEICON, gra_id, 0,
						 							NNS_G2D_VRAM_TYPE_2DMAIN, eID_OAM_POKE_ICON + no );

		coap.x		= pos_tbl[ type ][ i ][ 0 ] + 12 - 4;
		coap.y		= pos_tbl[ type ][ i ][ 1 ] - 12;
		coap.z		= 0;		
		coap.anm	= 0;
		coap.pri	= 0;
		coap.pal	= 0;
		coap.d_area = CATS_D_AREA_MAIN;
		coap.bg_pri = 0;
		coap.vram_trans = 0;
				
		coap.id[0] = eID_OAM_POKE_ICON + no;
		coap.id[1] = eID_OAM_POKE_ICON;
		coap.id[2] = eID_OAM_POKE_ICON;
		coap.id[3] = eID_OAM_POKE_ICON;
		coap.id[4] = CLACT_U_HEADER_DATA_NONE;
		coap.id[5] = CLACT_U_HEADER_DATA_NONE;
		
		vwk->cap_poke[ no ] = CATS_ObjectAdd_S( wk->sys.csp, wk->sys.crp, &coap );
		
		palno  = PokeIconPalNumGet( monsno, form, 0 );
	//	CLACT_PaletteOffsetChgAddTransPlttNo( vwk->cap_poke[ no ]->act, palno );
		CATS_ObjectPaletteOffsetSetCap( vwk->cap_poke[ no ], eOAM_PAL_POKE_ICON + palno );
		CATS_ObjectAnimeSeqSetCap( vwk->cap_poke[ no ], POKEICON_ANM_HPMAX );
		CATS_ObjectUpdateCap( vwk->cap_poke[ no ] );
		CATS_ObjectPriSetCap( vwk->cap_poke[ no ], 12 - i );
		
	//	OS_Printf( "no = %2d\n", no );
		no++;	
	}
}

void VideoDataMake( VIDEO_WORK* vwk, BR_WORK* wk )
{
	video_win_add( vwk, wk );
	video_poke_add( vwk, wk );
//	video_hero_add( vwk, wk );
}

void VideoDataFree( VIDEO_WORK* vwk, BR_WORK* wk )
{
	int i;
	
	CATS_FreeResourcePltt( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourceCell( wk->sys.crp, eID_OAM_POKE_ICON );
	CATS_FreeResourceCellAnm( wk->sys.crp, eID_OAM_POKE_ICON );	
	
	for ( i = 0; i < 12; i++ ){
		if ( vwk->cap_poke[ i ] ){
			CATS_FreeResourceChar( wk->sys.crp, eID_OAM_POKE_ICON + i );
			CATS_ActorPointerDelete_S( vwk->cap_poke[ i ] );	
			
			vwk->cap_poke[ i ] = NULL;
		}
	}
	
#if 0	
	CATS_FreeResourceChar( wk->sys.crp, eID_OAM_HERO_ICON );
	CATS_FreeResourcePltt( wk->sys.crp, eID_OAM_HERO_ICON );
	CATS_FreeResourceCell( wk->sys.crp, eID_OAM_HERO_ICON );
	CATS_FreeResourceCellAnm( wk->sys.crp, eID_OAM_HERO_ICON );
	
	CATS_ActorPointerDelete_S( vwk->cap_hero );
	vwk->cap_hero = NULL;
#endif	
	video_win_del( vwk, wk );
}

void VideoData_IconEnable( VIDEO_WORK* vwk, int flag )
{
	int i;
	
	for ( i = 0; i < 12; i++ ){
		if ( vwk->cap_poke[ i ] ){
			CATS_ObjectEnableCap( vwk->cap_poke[ i ], flag );
		}
	}
//	CATS_ObjectEnableCap( vwk->cap_hero, flag );
}


//--------------------------------------------------------------
/**
 * @brief	�E�B���h�E�ɑ΂��āA��������Z���^�����O����
 *
 * @param	win	
 * @param	str	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int BR_print_x_Get( GF_BGL_BMPWIN* win, STRBUF* str )
{
	int len = FontProc_GetPrintStrWidth( FONT_SYSTEM, str, 0 );
	int x	= ( GF_BGL_BmpWinGet_SizeX( win ) * 8 - len ) / 2;
	
	return x;
}


//--------------------------------------------------------------
/**
 * @brief	�Z�[�u�p�̃��[�N������������
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_SaveWork_Clear( BR_WORK* wk )
{
	wk->save_work1 = 0;
	wk->save_work2 = 0;
}


//--------------------------------------------------------------
/**
 * @brief	�Z�[�u�f�[�^���폜����
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL BR_Executed_RecDelete( BR_WORK* wk )
{
#if 0
	BattleRec_SaveDataErase(wk->save, HEAPID_BR, wk->ex_param1);
#else
	MISC *misc = SaveData_GetMisc(wk->save);
	EX_CERTIFY_SAVE_KEY key, old_key;
	u8 flag;
	
	MISC_ExtraSaveKeyGet(misc, EXDATA_ID_BATTLE_REC_MINE + wk->ex_param1, &key, &old_key, &flag);
	MISC_ExtraSaveKeySet(misc, EXDATA_ID_BATTLE_REC_MINE + wk->ex_param1, 
		EX_CERTIFY_SAVE_KEY_NO_DATA, EX_CERTIFY_SAVE_KEY_NO_DATA,  flag);
#endif
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief	�قڋ��ʂ̃v���C�I���e�B
 *
 * @param	none	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void PrioritySet_Common( void )
{
	GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 1 );			///< priority
	GF_BGL_PrioritySet( GF_BGL_FRAME1_M, 3 );
	GF_BGL_PrioritySet( GF_BGL_FRAME2_M, 0 );
	GF_BGL_PrioritySet( GF_BGL_FRAME3_M, 1 );		
	
	GF_BGL_PrioritySet( GF_BGL_FRAME0_S, 1 );
	GF_BGL_PrioritySet( GF_BGL_FRAME1_S, 3 );
	GF_BGL_PrioritySet( GF_BGL_FRAME2_S, 0 );
	GF_BGL_PrioritySet( GF_BGL_FRAME3_S, 1 );
}



//--------------------------------------------------------------
/**
 * @brief	�J�[�\�����AON OFF �܂߂� �g�p�\�ɂ���
 *
 * @param	wk	
 * @param	cur	
 * @param	x	
 * @param	y	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void HormingCursor_Init( BR_WORK* wk, CURSOR_WORK* cur, s16 x, s16 y )
{
	if ( wk->sys.touch == INPUT_MODE_KEY ){	
		Cursor_Visible( cur, TRUE );
	}
	Cursor_Active( cur, TRUE );
	{
		Cursor_PosSetFix( cur, x, y );
		Cursor_PosSet( cur, x, y );
	}
}


//--------------------------------------------------------------
/**
 * @brief	�J�[�\���̈ʒu�ݒ�
 *
 * @param	cur	
 * @param	x	
 * @param	y	
 * @param	event	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CurPosSet( CURSOR_WORK* cur, s16 x, s16 y, u32 event )
{
	if ( event == 0xFF ){ return; }
	
	Cursor_PosSet_Check( cur, x, y );
}

//--------------------------------------------------------------
/**
 * @brief	���j�b�g�����擾
 *
 * @param	num	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int Number_to_Unit_Get( u64 num )
{
	int unit = 1;
	u64 dat  = num;
	
	while ( dat > 1 ){
		
		dat /= 10;
		unit++;
	}
	
	OS_Printf( "�� = %d\n", unit );
	
	return unit;
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 * @param	id	
 * @param	frame	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_ScrSet( BR_WORK* wk, int id, int frame )
{
	ArcUtil_HDL_ScrnSet( wk->sys.p_handle, id, wk->sys.bgl, frame, 0, 0, 0, HEAPID_BR );
}

// =============================================================================
//
//
//	���G���[����
//
//
// =============================================================================
//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Error_Init( BR_WORK* wk )
{
	int frame = GF_BGL_FRAME2_M;
	
	if ( wk->sys.err_wk.bErrorInit ){ return; }	
	wk->sys.err_wk.bErrorInit = TRUE;

	if ( sys.disp3DSW == DISP_3D_TO_SUB ){	///< ����main�Ȃ��ɏo��
		frame = GF_BGL_FRAME2_S;
	}

	GF_BGL_BmpWinAdd( wk->sys.bgl, &wk->sys.err_wk.err_win, frame, 
					  ERR_WIN_X, ERR_WIN_Y, ERR_WIN_SX, ERR_WIN_SY, ERR_WIN_PAL, ERR_WIN_OFS );
	GF_BGL_BmpWinOff( &wk->sys.err_wk.err_win );
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Error_Exit( BR_WORK* wk )
{
	if ( wk->sys.err_wk.bErrorInit == FALSE ){ return; }
	wk->sys.err_wk.bErrorInit = FALSE;

	GF_BGL_BmpWinOff( &wk->sys.err_wk.err_win );
	GF_BGL_BmpWinDel( &wk->sys.err_wk.err_win );
}

//--------------------------------------------------------------
/**
 * @brief	�G���[���b�Z�[�W
 *
 * @param	work	
 * @param	error_msg	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Error_CallBack( void* work, STRBUF* error_msg )
{
	BR_WORK* wk = work;
	
	if( error_msg != NULL ){
	//	GF_BGL_BmpWinDataFill(&testsys->win[TEST_BMPWIN_LOG], 15 );
	//	testsys->msg_index = GF_STR_PrintSimple( &testsys->win[TEST_BMPWIN_LOG], FONT_TALK,	error_msg, 0, 0, MSG_ALLPUT, NULL);		
	}
	else {
	//	GF_BGL_BmpWinDataFill(&testsys->win[TEST_BMPWIN_LOG], 15 );	
	}
}


// =============================================================================
//
//
//	���^�b�`�A�N�V����
//
//
// =============================================================================
//--------------------------------------------------------------
/**
 * @brief	�^�b�`�A�N�V����TCB
 *
 * @param	tcb	
 * @param	work	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void TouchAction_TCB( TCB_PTR tcb, void* work )
{
	BR_WORK* wk = work;
	s16 ofs_r;
	
	if ( (++wk->touch_work.cnt) >= 7 ){
		TCB_Delete( tcb );
		wk->touch_work.cnt = 0;
		wk->touch_work.bAction = FALSE;
		Cursor_SizeS( wk->touch_work.cur );
		Cursor_R_Set( wk->touch_work.cur, 0, 0 );
		Cursor_Active( wk->touch_work.cur, FALSE );
		Cursor_Visible( wk->touch_work.cur, FALSE );
		return;
	}
	ofs_r = TOUCH_ACTION_CUR_R( wk->touch_work.cnt );
	Cursor_R_Set( wk->touch_work.cur, ofs_r, ofs_r );
}

//--------------------------------------------------------------
/**
 * @brief	�^�b�`�A�N�V����������
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
static void TouchActionInit( BR_WORK* wk )
{
	u32 x, y;
	s16 ofs = 0;

	if ( wk->touch_work.bAction == TRUE ){
		if ( wk->touch_work.tcb ){
			TCB_Delete( wk->touch_work.tcb );
		}
		wk->touch_work.tcb = NULL;
	}	
	if ( sys.disp3DSW == DISP_3D_TO_MAIN ){
		wk->touch_work.cur = wk->cur_wk_top;
	}
	else {
		wk->touch_work.cur = wk->cur_wk;
	}	
	wk->touch_work.bAction = TRUE;
	wk->touch_work.cnt = 0;
	
	GF_TP_GetPointTrg( &x, &y );
	
#ifdef BR_AUTO_SERCH
	x = 128;
	y =  96;
#endif

	Cursor_SizeL( wk->touch_work.cur );
	Cursor_Active( wk->touch_work.cur, TRUE );
	Cursor_Visible( wk->touch_work.cur, TRUE );
	Cursor_PosSet( wk->touch_work.cur, x, y + ofs );
	Cursor_PosSetFix( wk->touch_work.cur, x, y + ofs );
	wk->touch_work.tcb = TCB_Add( TouchAction_TCB, wk, eTCB_PRI_TOUCH_ACTION );

#ifdef PM_DEBUG
	{
		BOOL bCheck = DebugTCBSys_FuncEntryCheck( sys.mainTCBSys, TouchAction_TCB );
		GF_ASSERT_MSG( bCheck, "TouchAction TCB Add Error = %d", bCheck );
	}
#endif

	if ( wk->touch_work.tcb != NULL ){
		TouchAction_TCB( wk->touch_work.tcb, wk );
	}
	else {
		wk->touch_work.bAction = FALSE;
		Cursor_Visible( wk->touch_work.cur, FALSE );
		GF_ASSERT_MSG( 0, "TouchAction Error\n" );
	}
}

//--------------------------------------------------------------
/**
 * @brief	�^�b�`�A�N�V�����Ăяo��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void TouchAction( BR_WORK* wk )
{
	TouchActionInit( wk );
	Snd_SePlay( eBR_SND_TOUCH );
}


//--------------------------------------------------------------
/**
 * @brief	CATS �������Ă��邩
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL IsCATS_SystemActive( BR_WORK* wk )
{
	return wk->sys.bCATS;
}

//--------------------------------------------------------------
/**
 * @brief	CATS �̓�����w��
 *
 * @param	wk	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void CATS_SystemActiveSet( BR_WORK* wk, BOOL flag )
{
	wk->sys.bCATS = flag;
}

//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	wk	
 * @param	obtn	
 * @param	draw_area	
 * @param	msg_id	
 * @param	x	
 * @param	y	
 * @param	ox	
 * @param	oy	
 * @param	anime	
 * @param	flag	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Tool_FontButton_Create( BR_WORK* wk, OAM_BUTTON* obtn, int draw_area, int msg_id, s16 x, s16 y, s16 ox, s16 oy, int anime, BOOL flag )
{
	obtn->cap = ExTag_Add( wk, draw_area );	
	FontOam_Create( obtn, &wk->sys, draw_area );	
	FontOam_MsgSet( obtn, &wk->sys, msg_id );
	
	CATS_ObjectPosSetCap( obtn->cap, x, y );
	FONTOAM_SetMat( obtn->font_obj, ox, oy );
	FONTOAM_SetDrawFlag( obtn->font_obj, flag );
	CATS_ObjectAnimeSeqSetCap( obtn->cap, anime );
}


//--------------------------------------------------------------
/**
 * @brief	
 *
 * @param	obtn	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Tool_FontButton_Delete( OAM_BUTTON* obtn )
{
	FontOam_Delete( obtn );
	CATS_ActorPointerDelete_S( obtn->cap );	
}

// =============================================================================
//
//
//	���G���[����
//
//
// =============================================================================
//--------------------------------------------------------------
/**
 * @brief	�G���[���b�Z�[�W�������炤
 *
 * @param	wk	
 * @param	mode	
 * @param	result	
 *
 * @retval	int	
 *
 */
//--------------------------------------------------------------
int BR_ErrorMsgGet( BR_WORK* wk, int mode, int result, int type )
{
	wk->sys.comm_err_wk.main_seq	= wk->main_seq;
	wk->sys.comm_err_wk.sub_seq		= wk->sub_seq;
	wk->sys.comm_err_wk.proc_seq	= *wk->proc_seq;
	wk->sys.comm_err_wk.bError		= TRUE;
	
	OS_Printf( "err result = %d\n", result );
	OS_Printf( "err type   = %d\n", type );

//	GF_ASSERT_MSG( 0, "mode = %d, result = %d, type = %d\n", mode, result, type );

	{
		int msg = msg_err_000;
		
		switch ( type ){
		case GDS_ERROR_TYPE_LIB:
			///< GDS���C�u�����̃G���[
			switch ( result ){
			case POKE_NET_GDS_LASTERROR_NONE:
				msg = msg_lib_err_000;
				break;
			case POKE_NET_GDS_LASTERROR_NOTINITIALIZED:
				msg = msg_lib_err_001;
				break;
			case POKE_NET_GDS_LASTERROR_ILLEGALREQUEST:
				msg = msg_lib_err_002;
				break;
			case POKE_NET_GDS_LASTERROR_CREATESOCKET:
				msg = msg_lib_err_003;
				break;
			case POKE_NET_GDS_LASTERROR_IOCTRLSOCKET:
				msg = msg_lib_err_004;
				break;
			case POKE_NET_GDS_LASTERROR_NETWORKSETTING:
				msg = msg_lib_err_005;
				break;
			case POKE_NET_GDS_LASTERROR_CREATETHREAD:
				msg = msg_lib_err_006;
				break;
			case POKE_NET_GDS_LASTERROR_CONNECT:
				msg = msg_lib_err_007;
				break;
			case POKE_NET_GDS_LASTERROR_SENDREQUEST:
				msg = msg_lib_err_008;
				break;
			case POKE_NET_GDS_LASTERROR_RECVRESPONSE:
				msg = msg_lib_err_009;
				break;
			case POKE_NET_GDS_LASTERROR_CONNECTTIMEOUT:
				msg = msg_lib_err_010;
				break;
			case POKE_NET_GDS_LASTERROR_SENDTIMEOUT:
				msg = msg_lib_err_011;
				break;
			case POKE_NET_GDS_LASTERROR_RECVTIMEOUT:
				msg = msg_lib_err_012;
				break;
			case POKE_NET_GDS_LASTERROR_ABORTED:
				msg = msg_lib_err_013;
				break;
			case POKE_NET_GDS_LASTERROR_GETSVL:
				msg = msg_lib_err_014;
				break;
			default:						
				GF_ASSERT_MSG( 0, "result = %d\n", result );
				break;
			}
			break;
		
		case GDS_ERROR_TYPE_STATUS:
			///< GDS�̏�ԃG���[
			switch ( result ){
			case POKE_NET_GDS_STATUS_INACTIVE:
				msg = msg_st_err_000;
				break;
			case POKE_NET_GDS_STATUS_INITIALIZED:
				msg = msg_st_err_001;
				break;
			case POKE_NET_GDS_STATUS_REQUEST:
				msg = msg_st_err_002;
				break;
			case POKE_NET_GDS_STATUS_NETSETTING:
				msg = msg_st_err_003;
				break;
			case POKE_NET_GDS_STATUS_CONNECTING:
				msg = msg_st_err_004;
				break;
			case POKE_NET_GDS_STATUS_SENDING:
				msg = msg_st_err_005;
				break;
			case POKE_NET_GDS_STATUS_RECEIVING:
				msg = msg_st_err_006;
				break;
			case POKE_NET_GDS_STATUS_ABORTED:
				msg = msg_st_err_007;
				break;
			case POKE_NET_GDS_STATUS_FINISHED:
				msg = msg_st_err_008;
				break;
			case POKE_NET_GDS_STATUS_ERROR:
				msg = msg_st_err_009;
				break;
			default:						
				GF_ASSERT_MSG( 0, "result = %d\n", result );
				break;
			}
			break;
		
		case GDS_ERROR_TYPE_APP:
			///< �e�A�v���P�[�V�����̃G���[
			{		
				switch ( mode ){
				case eERR_DRESS_SEND:
					///< �h���X���M��
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_SUCCESS:
						msg = msg_err_000;
						break;				
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_AUTH:
						msg = msg_err_001;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ALREADY:
						msg = msg_err_002;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ILLEGAL:
						msg = msg_err_003;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ILLEGALPROFILE:
						msg = msg_err_004;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_UNKNOWN:
						msg = msg_err_005;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}
					break;
				
				case eERR_DRESS_RECV:
					///< �h���X��M��
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_SUCCESS:
						msg = msg_err_006;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_AUTH:
						msg = msg_err_007;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_ILLEGALPOKEMON:
						msg = msg_err_008;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_UNKNOWN:
						msg = msg_err_009;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}
					break;
				
				case eERR_BOX_SEND:
					///< �{�b�N�X�V���b�g���M��
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_SUCCESS:
						msg = msg_err_010;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_AUTH:
						msg = msg_err_011;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ALREADY:
						msg = msg_err_012;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGAL:
						msg = msg_err_013;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGALPROFILE:
						msg = msg_err_014;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGALGROUP:
						msg = msg_err_015;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_UNKNOWN:
						msg = msg_err_016;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}			
					break;
					
				case eERR_BOX_RECV:
					///< �{�b�N�X�V���b�g��M��
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_SUCCESS:
						msg = msg_err_017;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_AUTH:
						msg = msg_err_018;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_ILLEGALGROUP:
						msg = msg_err_019;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_UNKNOWN:
						msg = msg_err_020;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}			
					break;
				
				case eERR_RANKING_TYPE_RECV:
					///< �����L���O�f�[�^�̎�M
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_SUCCESS:
						msg = msg_err_021;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_ERROR_AUTH:
						msg = msg_err_022;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_ERROR_UNKNOWN:
						msg = msg_err_023;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}						
					break;
					
				case eERR_RANKING_SEND:
					///< �����L���O�f�[�^�̑��M
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_SUCCESS:
						msg = msg_err_024;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_AUTH:
						msg = msg_err_025;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALTYPE:
						msg = msg_err_026;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALDATA:
						msg = msg_err_027;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALPROFILE:
						msg = msg_err_028;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_UNKNOWN:
						msg = msg_err_029;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}						
					break;
				
				case eERR_VIDEO_SEND:
					///< �r�f�I�̑��M
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_SUCCESS:
						msg = msg_err_030;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_AUTH:
						msg = msg_err_031;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ALREADY:
						msg = msg_err_032;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGAL:
						msg = msg_err_033;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPROFILE:
						msg = msg_err_034;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_UNKNOWN:
						msg = msg_err_035;
						break;					
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPOKEMON_MINE:
						msg = msg_err_036_0;
						break;					
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPOKEMON_OTHER:	
						msg = msg_err_036_1;
						break;						
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}						
					break;

				case eERR_VIDEO_SEARCH:
					///< �r�f�I�̌���
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_SUCCESS:
						msg = msg_err_036;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_AUTH:
						msg = msg_err_037;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_ILLEGALPARAM:
						msg = msg_err_038;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_UNKNOWN:
						msg = msg_err_039;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}
					break;

				case eERR_VIDEO_DATA_GET:
					///< �r�f�I�̎�M
					switch ( result ){
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_SUCCESS:
						msg = msg_err_040;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_AUTH:
						msg = msg_err_041;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_ILLEGALCODE:
						msg = msg_err_042;
						break;
					case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_UNKNOWN:
						msg = msg_err_043;
						break;
					default:						
						GF_ASSERT_MSG( 0, "result = %d\n", result );
						break;
					}
					break;
					
				default:
					GF_ASSERT_MSG( 0, "result = %d\n", result );
					break;
				}
			}
			break;
		}
		wk->sys.comm_err_wk.err_msg_id	= msg;
	}
	
	OS_Printf( "msg id = %d\n", wk->sys.comm_err_wk.err_msg_id );
	
	return wk->sys.comm_err_wk.err_msg_id;	
}

//--------------------------------------------------------------
/**
 * @brief	�G���[���󂯎����
 *
 * @param	mode	�N�����[�h
 * @param	result	�󂯎��������
 * @param	type	�G���[�^�C�v GDS_ERROR_TYPE_LIB�@GDS_ERROR_TYPE_STATUS GDS_ERROR_TYPE_APP
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_ErrorSet( BR_WORK* wk, int mode, int result, int type )
{
	wk->sys.comm_err_wk.main_seq	= wk->main_seq;
	wk->sys.comm_err_wk.sub_seq		= wk->sub_seq;
	wk->sys.comm_err_wk.proc_seq	= *wk->proc_seq;
	wk->sys.comm_err_wk.err_seq		= 0;
	wk->sys.comm_err_wk.err_count	= 0;
	wk->sys.comm_err_wk.err_msg_id	= BR_ErrorMsgGet( wk, mode, result, type );
	
	wk->sys.comm_err_wk.bError		= TRUE;	

	*wk->proc_seq = eMAIN_Error;
}

//--------------------------------------------------------------
/**
 * @brief	�G���[�����̃��C��
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_Error_Main( BR_WORK* wk )
{
	switch ( wk->sys.comm_err_wk.err_seq ){
	case 0:
		if ( wk->sys.comm_err_wk.err_count < 8 ){
			wk->sys.comm_err_wk.err_count++;
		//	GX_SetMasterBrightness( -wk->sys.comm_err_wk.err_count );
		//	GXS_SetMasterBrightness( -wk->sys.comm_err_wk.err_count );
		//	G2_SetBlendBrightness( GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 |GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, -wk->sys.comm_err_wk.err_count );
		//	G2S_SetBlendBrightness( GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 |GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, -wk->sys.comm_err_wk.err_count );
		}
		else {
			Tool_InfoMessageMain( wk, wk->sys.comm_err_wk.err_msg_id );	
			wk->sys.comm_err_wk.err_seq++;
		}
		break;
	
	case 1:
		if ( GF_TP_GetTrg() ){
			Tool_InfoMessageMainDel( wk );	
			wk->sys.comm_err_wk.err_seq++;
		}
	#ifdef PM_DEBUG
		#ifdef BR_AUTO_SERCH
			Tool_InfoMessageMainDel( wk );	
			wk->sys.comm_err_wk.err_seq++;
			break;
		#endif
	#endif
		break;
	
	default:
		if ( wk->sys.comm_err_wk.err_count > 0 ){
			wk->sys.comm_err_wk.err_count--;
		//	GX_SetMasterBrightness( -wk->sys.comm_err_wk.err_count );
		//	GXS_SetMasterBrightness( -wk->sys.comm_err_wk.err_count );
		//	G2_SetBlendBrightness( GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 |GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, -wk->sys.comm_err_wk.err_count );
		//	G2S_SetBlendBrightness( GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 |GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, -wk->sys.comm_err_wk.err_count );
		}
		else {
			*wk->proc_seq = wk->sys.comm_err_wk.proc_seq;
		}		
		break;
	}
}


//--------------------------------------------------------------
/**
 * @brief	�ۑ�����Ă�H
 *
 * @param	wk	
 * @param	video_no	
 *
 * @retval	BOOL	YES
 *
 */
//--------------------------------------------------------------
BOOL BattleVideo_Preserved( BR_WORK* wk, u64 video_no )
{
	u64 no;
	int i;
	
	///< �����̃Z�[�u�͔�r���Ȃ�
	for ( i = 1; i < 4; i++ ){		
		if ( wk->br_head[ i ] == NULL ){ continue; }
		
		no = RecHeader_ParamGet( wk->br_head[ i ], RECHEAD_IDX_DATA_NUMBER, 0 );
		
		if ( no == video_no ){ return TRUE; }	///< �����Ă���u�b�u�[
	}
	
	return FALSE;
}


//--------------------------------------------------------------
/**
 * @brief	�G���[��������K��̕�����ɒu��������
 *
 * @param	wk	
 * @param	strbuf	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BR_ErrorStrChange( BR_WORK* wk, STRBUF *strbuf )
{
	GDS_RAP_WORK* gdsrap = BR_GDSRAP_WorkGet( wk );
	STRBUF *temp_str;
	int strcode_size = 64;	//�K���ɑ傫��
	
	temp_str = STRBUF_Create(strcode_size, gdsrap->heap_id);

	//�s�������R�[�h�`�F�b�N
	if(FontProc_ErrorStrCheck(FONT_SYSTEM, strbuf, temp_str) == FALSE){
		OS_TPrintf("�s���ȕ����R�[�h�������Ă���\n");
		ErrorNameSet(strbuf, gdsrap->heap_id);
	}

	STRBUF_Delete(temp_str);
}

//--------------------------------------------------------------
/**
 * @brief   �G���[�����������ꍇ�ɕ\������㗝�����o�b�t�@�ɃZ�b�g
 *
 * @param   dest_str		�����
 * @param   heap_id			�e���|�����Ŏg�p����q�[�v
 */
//--------------------------------------------------------------
static void ErrorNameSet(STRBUF *dest_str, int heap_id)
{
	MSGDATA_MANAGER * msgman;

	STRBUF_Clear(dest_str);
	
	msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_battle_rec_dat, heap_id );
	MSGMAN_GetString( msgman, msg_error_trainer_name, dest_str );
	MSGMAN_Delete( msgman );
}
