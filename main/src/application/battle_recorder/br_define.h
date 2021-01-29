//==============================================================================
/**
 * @file	br_define.h
 * @brief	���l�܂Ƃ߂Ă�Ƃ�
 * @author	goto
 * @date	2008.03.12(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
#ifndef __BR_DEFINE_H__
#define __BR_DEFINE_H__

//	----------------------------------------
//
//		�f�o�b�O
//
//	----------------------------------------
#ifdef PM_DEBUG

//#define BR_AUTO_SERCH
#ifdef BR_AUTO_SERCH
	extern int g_debug_auto;
	extern int g_debug_wait;
	extern u64 g_debug_count;	
#endif

#endif


// -----------------------------------------
//
//	�^�X�N�̃v���C�I���e�B
//
// -----------------------------------------
enum {
	eVINTR_TCB_PRI_VIEW		= 3,
	eVINTR_TCB_PRI_CHANGE,
	eVINTR_TCB_PRI_BAR,
	
	eTCB_PRI_TOUCH_ACTION	= 0x1000,	
	eTCB_PRI_TAG_MOVE		= 0x2000,	
};


#define BB_INFO_WAIT			( 30 * 2 )					///< �����ŏ����鎞��


#define HEAPID_BR				( HEAPID_BATTLE_RECORDER )	///< HEAPID
#define HEAP_SIZE_BR			( 0x55000 )					///< 60000�ȓ��ɂ��Ƃ������B

#define BR_MSG_CLEAR_CODE		( 0xCC )

#define SUB_SURFACE_FX_Y_BR		( 256 * FX32_ONE )			///< �T�[�t�F�C�X
#define SUB_SURFACE_Y_BR		( 256 )
#define SUB_SURFACE_FX_Y_N		( 192 * FX32_ONE )
#define SUB_SURFACE_Y_N			( 192 )

///< �^�O�̍��W�Ǘ�
#define TAG_INIT_POS( n,m )		( 25 + ( ( 36 ) * ( n ) ) + ( 16 * ( 5 - ( m ) ) ) )
#define TAG_INIT_POS_2( n,m )	( 25 + ( ( 36 ) * ( n ) ) + ( 16 * ( 4 - ( m ) ) ) )
#define TAG_INIT_POS_U( n )		( 25 + ( ( 36 ) * ( n ) ) )
#define TAG_MAIN_POS			( 42 )
#define TAG_HIT_X				( TAG_MAIN_POS + 8 )
#define TAG_HIT_Y( n )			( TAG_INIT_POS( n, 4 ) + 0 )


#define BR_TAG_BMP_WIX_SX	( 20 )
#define BR_TAG_BMP_WIX_SY	( 2 )
#define BMPWIN_CHAR_OFS		( 256 )
#define BR_DRESS_VIEW_MAX	( 5 )		///< �h���X�A�b�v�̍ő吔
#define BR_UNION_CHAR_MAX	( 16 )		///< ���j�I���L�����̍ő吔

#define PRINT_COL_BOOT		( GF_PRINTCOLOR_MAKE(  1, 14,  0 ) )	// �N�����̕����F
#define PRINT_COL_NORMAL	( GF_PRINTCOLOR_MAKE( 15, 13,  0 ) )	// �悭�g�������F
#define	PRINT_COL_BLACK		( GF_PRINTCOLOR_MAKE( 14, 13,  0 ) )	// �t�H���g�J���[
#define	PRINT_COL_INFO		( GF_PRINTCOLOR_MAKE( 15, 13, 12 ) )	// �t�H���g�J���[
#define	PRINT_COL_PHOTO		( PRINT_COL_NORMAL )
#define	PRINT_COL_BOX_SHOT	( PRINT_COL_NORMAL )
#define	PRINT_COL_BOX_NAME	( GF_PRINTCOLOR_MAKE( 15,  1,  0 ) )
#define	PRINT_COL_FREC		( PRINT_COL_NORMAL )
#define	PRINT_COL_PROF		( PRINT_COL_NORMAL )

#define INPUT_MODE_TOUCH	( TRUE )
#define INPUT_MODE_KEY		( FALSE )
#define INPUT_MODE_DEF		( INPUT_MODE_TOUCH )

#define SIDE_BAR_WAIT		( 3 )
#define SIDE_BAR_SIN_MOVE_F	( 4 )
#define SIDE_BAR_OAM		( 2 )
#define SIDE_BAR_L			( 3 )
#define SIDE_BAR_R			( SIDE_BAR_L * 2 )
#define SIDE_BAR_TOTAL		( SIDE_BAR_R )

#define WEEKLY_RANK_DATA_MAX	( 9999999999999999999 )

///< �t�F�[�h�Ώۂ̃J���[�r�b�g
#define LOGO_PALETTE			( 0x0002 )
#define TAG_SUB_PALETTE			( 0x000C )		///< 
#define LINE_OTHER_SUB_PALETTE	( 0x000C )		///< 
#define TAG_PALETTE				( 0x000C )		///< 001-1100
#define POKE_ICON_PALETTE		( 0x00F0 )		///< 111-1100

///< ���S�̐F �p���b�g�P�̂P�F��
#define LOGO_COLOR_B			( 0x73fa )		///< �u���E�Y���[�h �ʏ�
#define LOGO_COLOR_B2			( 0x771f )
#define LOGO_COLOR_B3			( 0x6f7b )
#define LOGO_COLOR_B4			( 0x6fff )
#define LOGO_COLOR_B5			( 0x4ebf )
#define LOGO_COLOR_B6			( 0x575e )
#define LOGO_COLOR_B7			( 0x5b9f )
#define LOGO_COLOR_B8			( 0xFFFF )
#define LOGO_COLOR_B9			( 0xFFFF )
#define LOGO_COLOR_G			( 0x7fdd )		///< �O���[�o�����[�h

///< ���C���̐F �p���b�g�O�̂P�F��
#define LINE_COLOR_B			( 0x1642 )		///< �u���E�Y���[�h �ʏ�
#define LINE_COLOR_B2			( 0x357f )
#define LINE_COLOR_B3			( 0x3def )
#define LINE_COLOR_B4			( 0x031f )
#define LINE_COLOR_B5			( 0x00bc )
#define LINE_COLOR_B6			( 0x0131 )
#define LINE_COLOR_B7			( 0x023f )
#define LINE_COLOR_B8			( 0xFFFF )
#define LINE_COLOR_B9			( 0xFFFF )
#define LINE_COLOR_G			( 0x7e05 )		///< �O���[�o�����[�h

#define CHANGE_COLOR_MAX		( 7 )			///< ���낪���ł��鐔

#define SIDE_BAR_MOVE_F			( 10 )			///< �T�C�h�o�[�̈ړ��t���[��

#define	TAG_MAX					( 5 )			///< �^�O�̌�
#define TAG_CIRCLE_CX			( 15 + 1 )		///< ���邭��J�[�\���̃I�t�Z�b�g
#define TAG_INIT_RAD			( 90 )			///< �^�O�̊p�x
#define TAG_VANISH_TOP			( -48 )			///< �����Ȃ��ʒu
#define TAG_VANISH_BOTTOM		( 48 )			///< ���̌����Ȃ��ʒu
#define TAG_VANISH_BOTTOM_INIT	( 32 )			///< �������Ȃ��Ƃ��̏����l
#define TAG_UP_MOVE_F			( 8 )			///< ��Ƀ^�O������X�s�[�h
#define TAG_UP_SUB_MOVE_F		( 4 )			///< �T�u��ʂ̃^�O�̃X�s�[�h
#define TAG_DOWN_MOVE_F			( 8 )			///< ���ɉ�����Ƃ��̃^�O�̃X�s�[�h

#define MENU_IN_SPEED			( 1 )					///< ���j���[���ł�X�s�[�h ( ���� = x1 )
#define TAG_FADE_SPPED			( 1 * MENU_IN_SPEED )	///< �^�O�̃t�F�[�h�X�s�[�h
#define TAG_IN_WAIT				( 4 / MENU_IN_SPEED )	///< �^�O�̃C���T�[�g�҂�
#define TAG_MOVE_VALUE_U		( 8 * MENU_IN_SPEED )	///< �^�O�̈ړ���
#define TAG_RAD_VALUE_U			( -4 * MENU_IN_SPEED )	///< �^�O�̉�]�p�x

#define MENU_OUT_SPEED			( 1 )					///< ���j���[���߂�X�s�[�h ( ���� = x1 )
#define TAG_MOVE_VALUE_D		( 8 * MENU_OUT_SPEED )	///< �^�O�̖߂�X�s�[�h
#define TAG_RAD_VALUE_D			( 4 * MENU_OUT_SPEED )	///< �^�O�̉�]�p�x

#define FONT_OAM_TAG_OX			( 36 )			///< ����OAM�̃I�t�Z�b�g
#define FONT_OAM_TAG_OY			( -8 )			///< ����OAM�̃I�t�Z�b�g

#define TAG_HIT_HEIGHT			( 16 )
#define TAG_HIT_RIGHT			( 160 )

#define COMM_HORMING_CUR_CX		( 128 )			///< �z�[�~���O�J�[�\��
#define COMM_HORMING_CUR_CY		(  96 )
#define COMM_HORMING_R			(  24 )			///< ����M���̔��a
#define LIST_HORMING_R			(  12 )			///< ���X�g�J�[�\���̔��a

#define GLOBAL_POKE_ICON_ID		( 500 )			///< �O���[�o���ȃO���t�B�b�N�h�c


///< �G���[����݂̂܂Ƃ�
#define ERR_WIN_X				( 1 )
#define ERR_WIN_Y				( 1 )
#define ERR_WIN_SX				( 20 )
#define ERR_WIN_SY				( 12 )
#define ERR_WIN_PAL				( 14 )
#define ERR_WIN_OFS				(  1 )


#define RGBtoY(r,g,b)	(((r)*76 + (g)*151 + (b)*29) >> 8)


#define DTC( n ) ( n * 8 )
#define DEF_MP1		( GX_BLEND_PLANEMASK_BG3 )
#define DEF_MP2		( GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ )
#define DEF_SP1		( GX_BLEND_PLANEMASK_BG3 )
#define DEF_SP2		( GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ )

///< �X���C�_�[����
#define BB_SLIDER_HIT_FRAME		( 2 )
#define BB_SLIDER_MOVE_VALUE	( 16 )

///< �^�b�`���X�g�@���T�Ȃ�param_ex�̃X�R�A�͎Q�Ƃ��Ȃ�
#define TL_THIS_WEEK_TR	( 10000 )	///< ���T�g���[�i�[
#define TL_LAST_WEEK_TR	( 20000 )
#define TL_THIS_WEEK_YM	( 30000 )	///< ���T���܂��
#define TL_LAST_WEEK_YM	( 40000 )
#define TL_THIS_WEEK_MN	( 50000 )	///< ���T�|�P����
#define TL_LAST_WEEK_MN	( 60000 )

#define NOTFOUND_MONSNO	( 0xFFFF )	///< �|�P����

///< �^�b�`�A�N�V����
#define TOUCH_ACTION_DEF_R		( 6 )
#define TOUCH_ACTION_ADD_R		( 2 )
#define TOUCH_ACTION_CUR_R( n )	( TOUCH_ACTION_DEF_R + ( n * TOUCH_ACTION_ADD_R ) )

enum {	
	eBR_BG0_PRI	= 1,
	eBR_BG1_PRI	= 3,
	eBR_BG2_PRI	= 2,
	eBR_BG3_PRI	= 1,
};

enum {	
	eHERO_TOP	= 0,
	eHERO_BOTTOM,
	eHERO_LEFT,
	eHERO_RIGHT,	
};

///< OAM
enum {	
	eOAM_PTI_FONT		= 3,
	eOAM_PRI_TAG_OFS	= 10,
};

///< �V�[�P���X	�����j���[�f�[�^�����₹
enum {
	
	eMAIN_Start			= 0,
	eMAIN_Input,
	eMAIN_Profile,
	eMAIN_Frontier,
	eMAIN_PhotoSend,
	eMAIN_PhotoRecv,
	eMAIN_BoxSend,
	eMAIN_BoxRecv,
	
	eMAIN_ThisWeek,
	eMAIN_LastWeek,
	
	eMAIN_CommBattleRanking,
	eMAIN_BattleFrontierRanking,
	
	eMAIN_VideoNew30,
	eMAIN_VideoKuwasiku,
	eMAIN_VideoNoSearch,
	eMAIN_VideoSend,
	
	eMAIN_Blank,	
	eMAIN_Delete,
	
	eMAIN_Error,		///< ����ȏ���
	eMAIN_End			= 0xFF,	
};

///< ���j���[�f�[�^
#define SEAM_LESS_ON	( TRUE )
#define SEAM_LESS_OFF	( TRUE )
enum {
	
	eMENU_DATA_SEQ_NONE	= 0,
	eMENU_DATA_SEQ_SELECT,
	
	eMENU_DATA_SEQ_END,
	eMENU_DATA_SEQ_BACK,
	
	eMENU_DATA_PROF_INIT,	
	eMENU_DATA_FREC_INIT,
	
	eMENU_DATA_SEQ_BLANK,	
	eMENU_DATA_REC_DELETE,
	
	eMENU_DATA_PHOTO_SEND,
	eMENU_DATA_PHOTO_RECV,
	
	eMENU_DATA_BOX_SEND,
	eMENU_DATA_BOX_RECV,
	
	eMENU_DATA_THIS_WEEK,
	eMENU_DATA_LAST_WEEK,
	
	eMENU_DATA_COMM_BATTLE_RANKING,
	eMENU_DATA_BATTLE_FRONTIER_RANKING,
	
	eMENU_DATA_VIDE_NEW30,
	eMENU_DATA_VIDE_KUWASIKU,
	eMENU_DATA_VIDE_NO_SEARCH,
	eMENU_DATA_VIDE_SEND,
	
	eMUENU_DATA_DUMMY,
};

///< �G���[�R�[�h
enum {
	
	eERR_DRESS_SEND = 0,
	eERR_DRESS_RECV,
	
	eERR_BOX_SEND,
	eERR_BOX_RECV,
	
	eERR_RANKING_TYPE_RECV,
	eERR_RANKING_SEND,
	
	eERR_VIDEO_SEND,
	eERR_VIDEO_SEARCH,		///< �ŐV�Ƃ������L���O��
	eERR_VIDEO_DATA_GET,	///< �R�[�h����
};

///< �L�����f�[�^
enum {
	
	eID_OAM_SIDE_BAR_M	= 9999,
	eID_OAM_SIDE_BAR_S,
	eID_OAM_TAG_M,
	eID_OAM_TAG_S,
	
	eID_OAM_POKE_ICON	= 100000,
	eID_OAM_HERO_ICON	= 100000 + 65535,
	
	eID_OAM_HUM_ICON	= 18181,	///< �t�����e�B�A�u���C��
	eID_OAM_BG_PHOTO	= 28282,	///< BG
	eID_OAM_BP_NUM		= 29292,	///< BP
	
	eID_OAM_EX_TAG		= 11924,	///< �g���^�O
	
	eID_LIST_CUR		= 200000,	///< ���X�g�p�̃J�[�\��
};

///< Side Bar �V�[�P���X
enum {
	eSB_TCB_STATE_WAIT		= 0,
	eSB_TCB_STATE_MOVE_1,
	eSB_TCB_STATE_MOVE_2,
	eSB_TCB_STATE_STAY,
};

///< OAM�p���b�g
enum {
	
	eOAM_PAL_CUR			= 0,	
	eOAM_PAL_LINE			= 1,
	eOAM_PAL_TAG_1,
	eOAM_PAL_TAG_2,	
//	eOAM_PAL_FONT,
	
	///< �v���t�B�[��
	eOAM_PAL_POKE_ICON		= 4,			///<
	eOAM_PAL_POKE_ICON_2,
	eOAM_PAL_POKE_ICON_3,
	eOAM_PAL_PLAYER,
	
	///< BP���
	eOMA_PAL_BP_NUM			= 4,			///< ��
	eOMA_PAL_BG_OAM			= 4,			///< �� BGOAMx6
	eOMA_PAL_HUM			= 10,			///< �� �u���[��x5
	eOAM_OAL_PLAYER			= 15,			///< �� �v���C���[ �����p���b�g�˂���I
	
	eOAM_PAL_MAX,
};

#define OAM_TAG_PAL_MAX	( 3 )
#define dOAM_PAL_TAG_1	( eOAM_PAL_TAG_1 - eOAM_PAL_LINE )
#define dOAM_PAL_TAG_2	( eOAM_PAL_TAG_2 - eOAM_PAL_LINE )

///< BG �p���b�g
enum {
	
	eBG_PAL_BASE_0		= 0,
	eBG_PAL_BASE_END	= 8,
	
	eBG_PAL_BOX		= 12,///13��
	eBG_PAL_DRESS	= 13,
	eBG_PAL_FONT	= 14,	
};
#define BG_PAL_BASE_END		( eBG_PAL_BASE_END + 1 )

///< �^�O�R�[�h
enum {
	
	eTAG_FIGHT		= 0,
	eTAG_DELETE,
	eTAG_NONE,
	eTAG_FRONTIER,
	eTAG_END		= 4,
	eTAG_BACK,
	eTAG_MYDATA,
	eTAG_ATHORDATA,	
};

enum {
	
	eTAG_GDS_DETAIL_S		 = 0,		///< ���킵��������
	eTAG_GDS_FACILITIES_S,				///< �����ł�����
	eTAG_GDS_LOOK,						///< �݂�
	eTAG_GDS_VS,						///< VS
	eTAG_GDS_ANOTHER_REC,				///< �N���̋L�^
	eTAG_GDS_BACK,						///< ���ǂ�
	eTAG_GDS_OK,						///< �n�j
	eTAG_GDS_FACTORY_OPEN,				///< �t�@�N�g���[�I�[�v��
	eTAG_GDS_SAVE,						///< �ۑ�
	eTAG_GDS_SEND,						///< ���M
	eTAG_GDS_NEW30,						///< ��������R�O
	eTAG_GDS_DATANO_S,					///< �f�[�^�i���o�[�ł�����
	eTAG_GDS_PREV,						///< ��
	eTAG_GDS_NEXT,						///< �O
	eTAG_GDS_END,						///< �����
};

enum {	
	eTAG_GDS_PHOTO = 0,
	eTAG_GDS_BOX_SHOT,
	eTAG_GDS_BOX_DRESS_END,
	eTAG_GDS_BOX_DRESS_BACK,
	eTAG_GDS_BOX_DRESS_OK,
};

///< �Z�[�u
enum {
	eTAG_GDS_SAVE_DATA = 0,
	eTAG_GDS_SAVE_BACK
};

enum {	
	eTAG_GDS_THIS_WEEK = 0,
	eTAG_GDS_LAST_WEEK,
	eTAG_GDS_WEEKLY_RANK_END,
	eTAG_GDS_RANK_SAVE_DATA,
	eTAG_GDS_RANK_BACK,
	eTAG_GDS_RANK_SAVE_OK,
};

///< �r�f�I
enum {	
	eTAG_GDS_VIDEO_KUWASIKU = 0,
	eTAG_GDS_VIDEO_SISETU,
	eTAG_GDS_VIDEO_PLAY,
	eTAG_GDS_VIDEO_BACK,
	eTAG_GDS_VIDEO_OK,
	eTAG_GDS_VIDEO_SAVE,
	eTAG_GDS_VIDEO_SEND,
	eTAG_GDS_VIDEO_NEW30,
	eTAG_GDS_VIDEO_NO_SEARCH,
	eTAG_GDS_VIDEO_END,	
};

///< �r�f�I�����L���O
enum {	
	eTAG_GDS_VR_TUUSHIN = 0,
	eTAG_GDS_VR_FRONTIER,
	eTAG_GDS_VR_END,
	eTAG_GDS_VR_1,
	eTAG_GDS_VR_2,	///< ���ǂ�
	eTAG_GDS_VR_3,	///< �͂�
};

///< �g���^�O
enum {	
	eTAG_EX_BACK = 0,
	eTAG_EX_OK,
	eTAG_EX_END,
	eTAG_EX_SAVE,
	eTAG_EX_SEND,
	
	eTAG_SLIDER,
};

///< �{�b�N�X�̏ꍇ
enum {	
	eTAG_EX_BOX_BACK = 0,
	eTAG_EX_BOX_OK,	
	eTAG_EX_BOX_SLIDER,
};


///< �����蔻��
enum {	
	eHRT_TAG_0	= 0,		///< �^�O�̓����蔻��
	eHRT_TAG_1,
	eHRT_TAG_2,
	eHRT_TAG_3,
	eHRT_TAG_4,	
	
	eHRT_MAX,				///< �����蔻��e�[�u���̍ő�l
};


// -----------------------------------------
//
//	���j���[�f�[�^�\��
//
// -----------------------------------------
#define BR_EX_DATA_NONE				( 0xFBFBFBFB )
#define BR_EX_DATA_BOX_SHOT_SEND	( 100 )
#define BR_EX_DATA_VIDEO_SEND		( 100 )
#define BR_EX_DATA_THIS_WEEK_RANK	( 0 )
#define BR_EX_DATA_LAST_WEEK_RANK	( 1 )
#define BR_EX_DATA_BATTLE_RANK		( 300 )
#define BR_EX_DATA_FRONTIER_RANK	( 301 )

#define BR_EX_DATA_NEW30			( 200 )
#define BR_EX_DATA_KUWASIKU			( 210 )
#define BR_EX_DATA_NO_SEARCH		( 220 )

#define BR_EX_WIFI_END				( 999 )
#define BR_EX_SEND_CHECK			( 9999 )	///< ���M�m�F



#define BR_RECOVER_WORK_MAX			( 8 )		///< ���A�p�̃��[�N


// =============================================================================
//
//
//	���V�[�P���X��`
//
//
// =============================================================================

// br_start.c
enum {
	eSTART_SEQ_BOOT = 0,
	eSTART_SEQ_FADE,
	eSTART_SEQ_ON,
	eSTART_SEQ_MENU_IN,
};


// br_input.c
enum {
	
	eINPUT_TOUCH	= 0,
	eINPUT_KEY,
	eINPUT_END,
	eINPUT_TAG_SELECT,
	eINPUT_TAG_BACK,
	eINPUT_MENU_IN,
	eINPUT_EXECUTED,		///< �_�C���N�g�Ɏ��s����
	eINPUT_COLOR_CHANGE_L,
	eINPUT_COLOR_CHANGE_R,
	
	eINPUT_NONE,
};


// br_profile.c
enum {
	
	ePROF_INIT		= 0,
	ePROF_MAIN,
	ePROF_END,
};


// br_frontier.c
enum {
	
	eFREC_INIT		= 0,
	eFREC_MAIN,
	eFREC_END,
};


// br_photo

///< ��{�V�[�P���X
enum {
	eDRESS_Init = 0,
	eDRESS_Setup,	
	eDRESS_Sep,
};

#define DRESS_SepSeq	( eDRESS_Sep )

///< ���M�V�[�P���X
enum {
	eDRESS_SendIn	= DRESS_SepSeq,
	eDRESS_SendMain,
	eDRESS_SendOut,
	eDRESS_SendMainOut,
};

///< �|�P�����w���M�V�[�P���X
enum {
	eDRESS_PokemonInit	= DRESS_SepSeq,
	eDRESS_PokemonAIUEO,
	eDRESS_PokemonSelect,
	eDRESS_PokemonOut,

	eDRESS_RecvMain,		///< ��M
	
	eDRESS_MainIn,			///< �h���XIN
	eDRESS_Main,			///< ���C��
	eDRESS_MainOut,			///< �h���X�@���@�|�P�����I��
	eDRESS_ProfChange,		///< �v���t�@�h���X�؂�ւ�
	eDRESS_DataChange,		///< ���̃f�[�^
};

enum {
	ePHOTO_INIT = 0,
	ePHOTO_SETUP,
	ePHOTO_MAIN,
	ePHOTO_FADE_R,
	ePHOTO_END,
	ePHOTO_SEND_FADE_R,
};
enum {
	eWORD_NONE	= 0,	
	eWORD_A		= 1,
	eWORD_KA,
	eWORD_SA,
	eWORD_TA,
	eWORD_NA,
	eWORD_HA,
	eWORD_MA,
	eWORD_YA,
	eWORD_RA,
	eWORD_WA,	
};

enum {
	ePHOTO_SEND_DATA = 0,
	ePHOTO_SEND_BACK,
};

enum {
	ePHOTO_PREVIEW_50ON_SELECT = 0,
	ePHOTO_PREVIEW_50ON_FADE,
	ePHOTO_PREVIEW_50ON_END,
	
	ePHOTO_PREVIEW_POKE_INIT,
	ePHOTO_PREVIEW_POKE_FADE,
	
	ePHOTO_PREVIEW_POKE_SELECT,
	ePHOTO_PREVIEW_POKE_END,
	
	ePHOTO_PREVIEW_RECV_INIT,
	ePHOTO_PREVIEW_RECV,
	
	ePHOTO_PREVIEW_RECV_PHOTO_INIT,
	ePHOTO_PREVIEW_RECV_MAIN,
	ePHOTO_PREVIEW_RECV_END,
	ePHOTO_PREVIEW_RECV_PROF_INIT,
	ePHOTO_PREVIEW_RECV_PROF_RESET,
	
	ePHOTO_PREVIEW_BACK,
};

// br_box.c
enum {	
	eBOX_COMMON_INIT = 0,
	eBOX_SETUP,
	eBOX_INIT,
	eBOX_MAIN,
	eBOX_TRAY_CHANGE,
	eBOX_CATEGORY_IN,
	eBOX_CATEGORY_MAIN,
	eBOX_CATEGORY_OUT,
	eBOX_SEND_OUT,
	eBOX_END_FADE,
	eBOX_EXIT,	
};

enum {	
	eBOX2_COMMON_INIT = 0,
	eBOX2_SETUP,
	eBOX2_INIT,
	eBOX2_MAIN,		
	eBOX2_SELECT_IN,
	eBOX2_SELECT,
	eBOX2_SELECT_OUT,	
	eBOX2_PROF_IN,
	eBOX2_PROF_OUT,	
	eBOX2_BOX_IN,
	eBOX2_BOX,	
	eBOX2_BOX_OUT,
	eBOX2_END_FADE,
	eBOX2_EXIT,	
};

// br_weekly_ranking.c
enum {	
	eRANK_Init = 0,
	eRANK_Setting,
	eRANK_SendRecv,
	eRANK_RankChange,
	eRANK_GroupChange,
	eRANK_MainIn,
	eRANK_Main,
	eRANK_Fade,
	eRANK_Exit,
};

// br_video_ranking.c
enum {
	eVRANK_Init = 0,
	eVRANK_Setting,
	eVRANK_SendRecv,
	eVRANK_MainIn,
	eVRANK_Main,
	eVRANK_Exit,

	eVRANK_ProfIn,
	eVRANK_ProfMain,
	eVRANK_ProfChange,
	eVRANK_Play,
	eVRANK_ProfExit,
	eVRANK_SaveIn,
	eVRANK_SaveMain,
	eVRANK_SaveExit,

	eVRANK_CheckIn,				///< �m�F��ʂ���
	eVRANK_CheckMain,			///< �m�F
	eVRANK_CheckExit,			///< �I���`�Z�[�u��ʁH
	eVRANK_CheckFinish,			///< �I���`�Z�[�u��ʁH
	
	eVRANK_BrsDataGet,			///< �f�[�^�擾
	eVRANK_RecoverIn,
	eVRANK_PlayRecover,
};

// br_video.c
enum {
	///< ���M�p	
	eVIDEO_Init = 0,
	eVIDEO_Fade_Out,
	eVIDEO_Send_Init,
	eVIDEO_Send_Main,
	eVIDEO_Send_Exit,
};

enum {
	///< �f�[�^�i���o�[�w��
	eVIDEO_CodeIn_Init = 2,
	eVIDEO_CodeIn_Main,
	eVIDEO_CodeIn_Exit,
	eVIDEO_CodeIn_End,
	eVIDEO_CodeIn_DataSearch,
	
	eVIDEO_CodeIn_ProfIn,
	eVIDEO_CodeIn_ProfMain,
	eVIDEO_CodeIn_ProfChange,	///< �T�v�E�v���t�̐؂�ւ�
	eVIDEO_CodeIn_Play,
	eVIDEO_CodeIn_ProfExit,		///< ���ǂ�

	eVIDEO_CodeIn_SaveIn,		///< �Z�[�u��ʍs��
	eVIDEO_CodeIn_SaveMain,		///< �Z�[�u���
	eVIDEO_CodeIn_SaveExit,		///< �Z�[�u��ʁ`�v���t�B�[��

	eVIDEO_CodeIn_CheckIn,		///< �m�F��ʂ���
	eVIDEO_CodeIn_CheckMain,	///< �m�F
	eVIDEO_CodeIn_CheckExit,	///< �I���`�Z�[�u��ʁH
	eVIDEO_CodeIn_CheckFinish,	///< �I���`�Z�[�u��ʁH
	
	eVIDEO_CodeIn_RecoveIn,
	eVIDEO_PlayRecover,
};

enum {
	///< �ŐV30��
	eN30_DownLoad = 2,			///< �ꗗ�擾
	eN30_ListInit,				///< ���X�g������
	eN30_ListMain,				///< ���X�g���C��
	eN30_ListExit,				///< ���X�g������	
	eN30_RecoverIn,
};

enum {
	///< ����	Conditional Search
	eCS_TopViewInit = 2,		///< ����TOP
	eCS_TopViewMain,
	eCS_TopViewExit,
	
	///< �e�ݒ�
	eCS_SearchCommonInit,		///< ���ʏI���e��������
	eCS_Shisetu,
	eCS_Pokemon,
	eCS_PokemonAIUEO,
	eCS_PokemonSelect,
	eCS_Profile,
	eCS_SearchCommonExit,	
	
	eCS_SearchMain,				///< main
	eCS_SearchExit,				///< exit
	eCS_ListInit,				///< ���X�g�̏�����
	eCS_ListMain,				///< ���X�g�̏�����	
	eCS_TopViewRecover,			///< ������ʂɖ߂�

	eCS_ProfIn,					///< �v���t�B�[���C��
	eCS_ProfMain,				///< �v���t�B�[�����C��
	eCS_ProfChange,				///< �T�v�E�v���t�̐؂�ւ�
	eCS_Play,					///< �Đ�
	eCS_ProfExit,				///< ���ǂ�
	
	eCS_SaveIn,					///< �ۑ�
	eCS_SaveMain,				///< �ۑ����C��
	eCS_SaveExit,				///< �I��
	
	eCS_CheckIn,				///< �m�F��ʂ���
	eCS_CheckMain,				///< �m�F
	eCS_CheckExit,				///< �I���`�Z�[�u��ʁH
	eCS_CheckFinish,			///< �I���`�Z�[�u��ʁH
	
	eCS_BrsDataGet,				///< �f�[�^�擾
	
	eCS_RecoverIn,				///< �퓬����̕��A
	
	eCS_PlayRecover,
};


// ���낪��
enum {	
	GET_PAL_FONT = 0,
	GET_PAL_PHOTO,
	GET_PAL_OBJ,
	GET_PAL_BG,	
};

extern const RECT_HIT_TBL hit_rect_AIUEO[ 10 ];

#endif
