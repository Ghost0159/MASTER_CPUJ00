//==============================================================================================
/**
 * @file	roulette.c
 * @brief	�u�o�g�����[���b�g�v���C���\�[�X
 * @author	Satoshi Nohara
 * @date	07.09.06
 */
//==============================================================================================
#include "system/main.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "system/brightness.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "system/bmp_menu.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/numfont.h"
#include "gflib/strbuf_family.h"
#include "gflib/touchpanel.h"
#include "savedata/config.h"
#include "application/p_status.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_number.h"
#include "poketool/pokeicon.h"
#include "savedata/b_tower.h"
#include "savedata/frontier_savedata.h"
#include "battle/tokusyu_def.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_info.h"
#include "communication/comm_def.h"
#include "communication/wm_icon.h"

#include "../roulette_def.h"
#include "roulette_sys.h"
#include "roulette_clact.h"
#include "roulette_bmp.h"
#include "roulette_obj.h"
#include "roulette.dat"
#include "application/roulette.h"
#include "../frontier_tool.h"					//Frontier_PokeParaMake
#include "../roulette_tool.h"						//ROULETTE_POKE_RANGE
#include "../comm_command_frontier.h"

#include "../../field/comm_direct_counter_def.h"//
#include "../../field/fieldobj.h"
#include "../../field/scr_tool.h"
#include "field/weather_no.h"

#include "msgdata\msg.naix"						//NARC_msg_??_dat
#include "msgdata\msg_roulette.h"				//msg_??

#include "../graphic/frontier_obj_def.h"
#include "../graphic/frontier_bg_def.h"


#include "system/pm_overlay.h"
FS_EXTERN_OVERLAY(frontier_common);


//==============================================================================================
//
//	�f�o�b�N�p
//
//==============================================================================================
//PROC_DATA* p_proc;

#ifdef PM_DEBUG
//#define DEBUG_DECIDE_EV_NO		//��`�L���ŁA���̃p�l���i���o�[�������Z�b�g
#define DEBUG_PANEL_NO			(ROULETTE_EV_MINE_DOKU)
//#define DEBUG_PANEL_NO			(ROULETTE_EV_ENEMY_ITEM_GET)
//#define DEBUG_PANEL_NO			(ROULETTE_EV_MINE_ITEM_GET)
//#define DEBUG_PANEL_NO			(ROULETTE_EV_EX_POKE_CHANGE)
//#define DEBUG_PANEL_NO			(ROULETTE_EV_ENEMY_NEMURI)
//#define DEBUG_PANEL_NO			(ROULETTE_EV_MINE_NEMURI)
#endif

static u8 debug_panel_no;
static u8 debug_panel_flag;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�V�[�P���X��`
enum {
	SEQ_GAME_INIT,											//������

	SEQ_GAME_CSR_MOVE,										//�J�[�\���ړ���

	SEQ_GAME_SEND_RECV,										//����M

	SEQ_GAME_END_MULTI,										//(�ʐM)�I��
	SEQ_GAME_END,											//�I��
};

//�t�H���g�J���[
#define	COL_BLUE	( GF_PRINTCOLOR_MAKE(FBMP_COL_BLUE,FBMP_COL_BLU_SDW,FBMP_COL_NULL) )//��
#define	COL_RED		( GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_NULL) )	//��

//��񃁃b�Z�[�W�̕\���ʒu
enum{
	INFO_SINGLE_OFFSET_X	= 8*8,
	INFO_MULTI_L_OFFSET_X	= 0,			//�y�A�̃I�t�Z�b�g(�k)
	INFO_MULTI_R_OFFSET_X	= 16*8,			//�y�A�̃I�t�Z�b�g(�q)

	INFO_PLAYER_X		= 1, 
	INFO_PLAYER_Y		= 1,
	INFO_PLAYER_CP_X	= 7*8, 
	INFO_PLAYER_CP_Y	= INFO_PLAYER_Y,

	INFO_KAIHUKU_X		= 1, 
	INFO_KAIHUKU_Y		= 1,
	INFO_KAIHUKU_RANK_X = 8*8, 
	INFO_KAIHUKU_RANK_Y = INFO_KAIHUKU_Y,

	INFO_ITEM_X			= 1,
	INFO_ITEM_Y			= 4*8,
	INFO_ITEM_RANK_X	= 8*8,
	INFO_ITEM_RANK_Y	= INFO_ITEM_Y,

	INFO_INFO_X			= 1,
	INFO_INFO_Y			= 8*8,
	INFO_INFO_RANK_X	= 8*8,
	INFO_INFO_RANK_Y	= INFO_INFO_Y,
};

//�E���J�[�\���̃I�t�Z�b�g
#define SEL_CSR_L_OFFSET	(8)
#define SEL_CSR_R_OFFSET	(120)

//�u���ǂ�v�̈ʒu
#define CSR_MODORU_X		(8*8)
#define CSR_MODORU_Y		(22*8+4)

#define ROULETTE_DECIDE_NONE	(0xff)				//���薳��

//�p�l���z�u
enum{
	PANEL_START_X	= 68,//64,						//�J�nX
	PANEL_START_Y	= 36,//32,						//�J�nY
	PANEL_WIDTH_X	= 40,//32,
	PANEL_WIDTH_Y	= 40,//32,
};

//�|�P�����A�C�R���z�u(�������؂��`�ōs��)
enum{
	//ICON_MINE_START_X	= 24,						//�J�nX
	ICON_MINE_START_X	= 16,						//�J�nX
	//ICON_ENEMY_START_X	= 232,						//�J�nX
	ICON_ENEMY_START_X	= 238,						//�J�nX
	ICON_START_Y		= 46,						//�J�nY
	ICON_START_MULTI_Y	= 22,						//�J�nY
	ICON_WIDTH_Y		= 40,//32,
};

//�A�C�e�������Ă��邩�A�C�R���z�u
enum{
	ITEMKEEP_MINE_START_X	= (ICON_MINE_START_X + 8),	//�J�nX
	ITEMKEEP_ENEMY_START_X	= (ICON_ENEMY_START_X + 8),	//�J�nX
	ITEMKEEP_START_Y		= (ICON_START_Y + 4),		//�J�nY
	ITEMKEEP_START_MULTI_Y	= (ICON_START_MULTI_Y + 4),	//�J�nY
	ITEMKEEP_WIDTH_Y		= ICON_WIDTH_Y,
};

//�{�^���z�u
enum{
	BUTTON_START_X	= 128,//132,//64,					//�J�nX
	BUTTON_START_Y	= 96,//32,							//�J�nY
	BUTTON_WIDTH_X	= 64,
	BUTTON_WIDTH_Y	= 64,
	TP_BUTTON_CX	= 128,								//�{�^���̒��SX
	TP_BUTTON_CY	= 96,								//�{�^���̒��SY
	TP_BUTTON_R		= 32,								//���a
};

//�{�^���̃^�b�`����e�[�u��
static const TP_HIT_TBL ButtonHitTbl[] =
{
	{ TP_USE_CIRCLE, TP_BUTTON_CX, TP_BUTTON_CY, 32 },
	{ TP_HIT_END, 0, 0, 0 }
};

#define ROULETTE_COUNT_321_WAIT			( 24 )			//�J�E���g�_�E���E�F�C�g
#define ROULETTE_PANEL_WAIT				( 30 )			//�p�l���\���E�F�C�g

#define ROULETTE_GAME_TIME				( 30 * 30 )		//�Q�[������(30�b)

#define ROULETTE_PANEL_EFF_WAIT			( 4 )			//�p�l���G�t�F�N�g�X�N���[���̐؂�ւ�

///��������1�Ώۖ�
#define ROULETTE_BLD_PLANE_1			(GX_BLEND_PLANEMASK_BG2)

///��������2�Ώۖ�
#define ROULETTE_BLD_PLANE_2			(GX_BLEND_BGALL | GX_BLEND_PLANEMASK_OBJ)

///��1�Ώۖʂɑ΂��郿�u�����f�B���O�W��
#define ROULETTE_BLD_ALPHA_1			(8)

///��2�Ώۖʂɑ΂��郿�u�����f�B���O�W��
#define ROULETTE_BLD_ALPHA_2			(14)

#define ROULETTE_START_WAIT				(10)			//3,2,1�J�n�܂ł̃E�F�C�g


//==============================================================================================
//
//	���b�Z�[�W�֘A�̒�`
//
//==============================================================================================
#define ROULETTE_MENU_BUF_MAX	(2)								//���j���[�o�b�t�@�̍ő吔
#define BR_FONT					(FONT_SYSTEM)					//�t�H���g���
#define ROULETTE_MSG_BUF_SIZE	(600)//(800)//(1024)			//��b�̃��b�Z�[�Wsize
#define ROULETTE_MENU_BUF_SIZE	(32)							//���j���[�̃��b�Z�[�Wsize
#define PLAYER_NAME_BUF_SIZE	(PERSON_NAME_SIZE + EOM_SIZE)	//�v���C���[���̃��b�Z�[�Wsize
#define POKE_NAME_BUF_SIZE		(MONS_NAME_SIZE + EOM_SIZE)		//�|�P�������̃��b�Z�[�Wsize


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
struct _ROULETTE_WORK{

	PROC* proc;										//PROC�ւ̃|�C���^
	FRONTIER_SAVEWORK* fro_sv;						//

	u8	sub_seq;									//�V�[�P���X
	u8	type;										//�����Ƃ��ēn���ꂽ�o�g���^�C�v
	u8	msg_index;									//���b�Z�[�Windex
	u8	wait;

	u8	random_flag;								//�J�[�\���ړ������_��on�t���O
	u8	csr_pos;									//���݂̃J�[�\���ʒu
	u8	parent_decide_pos;							//���肳�ꂽ�J�[�\���ʒu
	u8	recieve_count;								//��M�J�E���g

	u8	csr_wait;									//�J�[�\���������E�F�C�g
	u8	point_rank;
	u8	point;										//�퓬���ʃ|�C���g
	u8	panel_scrn:1;
	u8	panel_wait:7;

	u8	suberi_csr_pos;
	u8  dummy16;
	u16	start_csr_pos;								//�J�n�J�[�\���ʒu

	int game_timer;									//30�b�ŋ����I��

	u8*	p_csr_speed_level;							//�J�[�\���������X�s�[�h�̃��x��
	u8* p_decide_ev_no;								//���肵���C�x���g�i���o�[
	u16* p_rand_pos;								//�|�P�p�[�e�B�̂ǂ̈ʒu����`�F�b�N���邩

	u16	rensyou;									//����ڂ�
	u16	lap;										//����

	u8	h_max;										//���̍ő吔
	u8	v_max;										//�c�̍ő吔
	u8	hv_max;										//��*�c
	u8	menu_flag:1;								//���j���[�\�������t���O
	u8	rankup_req:7;								//�����N�A�b�v���������N�G�X�g�t���O

	u8	panel_color_num[ROULETTE_PANEL_COLOR_MAX];	//�p�l���̐F���Ƃ̐���ۑ�

	u8	panel_no[ROULETTE_PANEL_MAX];				//�p�l���i���o�[

	u8	ev_tbl[ROULETTE_EV_MAX];					//�I�o���X�g�쐬�p
	u8	ev_tbl_max;									//�I�o���X�g�ő吔

	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g
	STRBUF* msg_buf;								//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;								//�e���|�����o�b�t�@�|�C���^

	STRBUF* menu_buf[ROULETTE_MENU_BUF_MAX];		//���j���[�o�b�t�@�|�C���^
	STRCODE str[PERSON_NAME_SIZE + EOM_SIZE];		//���j���[�̃��b�Z�[�W

	GF_BGL_INI*	bgl;								//BGL�ւ̃|�C���^
	GF_BGL_BMPWIN bmpwin[ROULETTE_BMPWIN_MAX];		//BMP�E�B���h�E�f�[�^

	//BMP���j���[(bmp_menu.h)
	BMPMENU_HEADER MenuH;							//BMP���j���[�w�b�_�[
	BMPMENU_WORK* mw;								//BMP���j���[���[�N
	BMPMENU_DATA Data[ROULETTE_MENU_BUF_MAX];		//BMP���j���[�f�[�^

	PALETTE_FADE_PTR pfd;							//�p���b�g�t�F�[�h

	//const CONFIG* config;							//�R���t�B�O�|�C���^
	CONFIG* config;									//�R���t�B�O�|�C���^
	SAVEDATA* sv;									//�Z�[�u�f�[�^�|�C���^
	ROULETTEDATA* roulette_sv;						//�L���b�X���Z�[�u�f�[�^�|�C���^
	ROULETTESCORE* score_sv;						//�L���b�X���Z�[�u�f�[�^�|�C���^

	ROULETTE_CLACT roulette_clact;					//�Z���A�N�^�f�[�^
	ROULETTE_OBJ* p_csr;							//�J�[�\��OBJ�̃|�C���^�i�[�e�[�u��
	ROULETTE_OBJ* p_panel[ROULETTE_PANEL_MAX];		//�p�l��OBJ�̃|�C���^�i�[�e�[�u��
	ROULETTE_OBJ* p_m_icon[ROULETTE_COMM_POKE_TOTAL_NUM];//�|�P�����A�C�R��OBJ�̃|�C���^�i�[�e�[�u��
	ROULETTE_OBJ* p_e_icon[ROULETTE_COMM_POKE_TOTAL_NUM];//�|�P�����A�C�R��OBJ�̃|�C���^�i�[�e�[�u��
	ROULETTE_OBJ* p_m_itemkeep[ROULETTE_COMM_POKE_TOTAL_NUM];//�A�C�e�������Ă��邩�A�C�R��OBJ
	ROULETTE_OBJ* p_e_itemkeep[ROULETTE_COMM_POKE_TOTAL_NUM];//�A�C�e�������Ă��邩�A�C�R��OBJ
	ROULETTE_OBJ* p_button;							//�{�^��OBJ�̃|�C���^�i�[�e�[�u��

	int* p_weather;									//�V��
	u16* p_ret_work;								//ROULETTE_CALL_WORK�̖߂�l���[�N�ւ̃|�C���^
	POKEPARTY* p_m_party;
	POKEPARTY* p_e_party;

	ARCHANDLE* hdl;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//ROULETTE_COMM roulette_comm;
#if 1
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[ROULETTE_COMM_BUF_LEN];

	//�ʐM�p
	u8	pair_csr_pos;								//�p�[�g�i�[�̃J�[�\���ʒu
	u8	pair_sel_pos;								//�p�[�g�i�[�̃����N�A�b�v�������J�[�\���ʒu

	u16 pair_rensyou;
	u16 dummy26;
#endif
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	
	u32 dummy_work;
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//PROC
PROC_RESULT RouletteProc_Init( PROC * proc, int * seq );
PROC_RESULT RouletteProc_Main( PROC * proc, int * seq );
PROC_RESULT RouletteProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameInit( ROULETTE_WORK* wk );
static BOOL Seq_GameCsrMove( ROULETTE_WORK* wk );
static BOOL Seq_GameSendRecv( ROULETTE_WORK* wk );
static BOOL Seq_GameEndMulti( ROULETTE_WORK* wk );
static BOOL Seq_GameEnd( ROULETTE_WORK* wk );

//���ʏ���
static void RouletteCommon_Delete( ROULETTE_WORK* wk );
static void Roulette_InitSub1( void );
static void Roulette_InitSub2( ROULETTE_WORK* wk );

//���ʏ������A�I��
static void Roulette_ObjInit( ROULETTE_WORK* wk );
static void Roulette_BgInit( ROULETTE_WORK* wk );
static void Roulette_BgExit( GF_BGL_INI * ini );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//BG�O���t�B�b�N�f�[�^
static void Roulette_SetMainBgGraphic( ROULETTE_WORK * wk, u32 frm );
static void Roulette_SetMain2BgGraphic( ROULETTE_WORK * wk, u32 frm );
//static void Roulette_SetPanelEffBgGraphic( ROULETTE_WORK * wk, u32 frm, u8 flag );
static void Roulette_SetMainBgPalette( void );
static void Roulette_SetSubBgGraphic( ROULETTE_WORK * wk, u32 frm  );

//���b�Z�[�W
static u8 RouletteWriteMsg( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 RouletteWriteMsgSimple( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 Roulette_EasyMsg( ROULETTE_WORK* wk, int msg_id );

//���j���[
static void RouletteInitMenu( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max );
static void RouletteSetMenuData( ROULETTE_WORK* wk, u8 no, u8 param, int msg_id );
static void Roulette_SetMenu2( ROULETTE_WORK* wk );

//������
static void Roulette_SetNumber( ROULETTE_WORK* wk, u32 bufID, s32 number, u32 keta, NUMBER_DISPTYPE disp );
static void Roulette_SetPokeName( ROULETTE_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp );
static void Roulette_SetPlayerName( ROULETTE_WORK* wk, u32 bufID );
static void PlayerNameWrite( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PairNameWrite( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void TalkWinWrite( ROULETTE_WORK* wk );

//�c�[��
static BOOL Roulette_CheckType( ROULETTE_WORK* wk, u8 type );
static void NextSeq( ROULETTE_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( ROULETTE_WORK* wk );
static void CsrMove( ROULETTE_WORK* wk, int key );
static void CsrPosSet( ROULETTE_WORK* wk, u8 csr_pos );
static u16 GetCsrX( ROULETTE_WORK* wk, u8 csr_pos, u8 lr );
static u16 GetCsrY( ROULETTE_WORK* wk, u8 csr_pos );
static void Roulette_GetOffset( ROULETTE_WORK* wk, u16* offset_x, u16* offset_y, u16* pair_offset_x, u16* pair_offset_y );
static void Roulette_SetCsrPosSuberi( ROULETTE_WORK* wk );
static u8 GetCommSelCsrPos( u8 type_offset, u8 decide_type );
static u16 Roulette_CommGetRensyou( ROULETTE_WORK* wk );

//�ʐM
BOOL Roulette_CommSetSendBuf( ROULETTE_WORK* wk, u16 type, u16 param );
void Roulette_CommSendBufBasicData( ROULETTE_WORK* wk, u16 type );
void Roulette_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
void Roulette_CommSendBufRankUpType( ROULETTE_WORK* wk, u16 type, u16 param );
void Roulette_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work);
void Roulette_CommSendBufCsrPos( ROULETTE_WORK* wk, u16 type );
void Roulette_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);

//�T�u�V�[�P���X
static void Roulette_SeqSubPanelDecide( ROULETTE_WORK* wk, u8 decide_type );

//�C�x���g
//static void Roulette_EvSet( ROULETTE_WORK* wk, u8 no );
static u8 Roulette_GetDecideColor( ROULETTE_WORK* wk, u8 point_rank );
static void Roulette_EvPanelChoice( ROULETTE_WORK* wk );
static void Roulette_EvPanelChoice2( ROULETTE_WORK* wk );
static void Roulette_EvPanelChoice3( ROULETTE_WORK* wk );
static void Roulette_ItemKeepVanishSet( ROULETTE_WORK* wk );

//�^�b�`�p�l��
static BOOL Roulette_CheckButtonPush( ROULETTE_WORK* wk );
static void Roulette_SetButtonPush( ROULETTE_WORK* wk );

//���ʂ̃����_�������߂Ă���
static u16 Roulette_GetRandPos( ROULETTE_WORK* wk );


//==============================================================================================
//
//	PROC
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�v���Z�X�֐��F������
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	"������"
 */
//--------------------------------------------------------------
PROC_RESULT RouletteProc_Init( PROC * proc, int * seq )
{
	int i;
	ROULETTE_WORK* wk;
	ROULETTE_CALL_WORK* roulette_call;

	Overlay_Load( FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE );

	Roulette_InitSub1();

	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_ROULETTE, 0x20000 );

	wk = PROC_AllocWork( proc, sizeof(ROULETTE_WORK), HEAPID_ROULETTE );
	memset( wk, 0, sizeof(ROULETTE_WORK) );

	wk->bgl				= GF_BGL_BglIniAlloc( HEAPID_ROULETTE );
	wk->proc			= proc;
	roulette_call		= (ROULETTE_CALL_WORK*)PROC_GetParentWork( proc );
	wk->sv				= roulette_call->sv;
	wk->roulette_sv		= SaveData_GetRouletteData( wk->sv );
	wk->score_sv		= SaveData_GetRouletteScore( wk->sv );
	wk->type			= roulette_call->type;
	wk->lap				= roulette_call->lap;
	wk->rensyou			= roulette_call->rensyou;
	wk->pair_rensyou	= roulette_call->pair_rensyou;
	wk->point			= roulette_call->point;
	wk->p_ret_work		= &roulette_call->ret_work;
	wk->config			= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾
	wk->p_m_party		= roulette_call->p_m_party;
	wk->p_e_party		= roulette_call->p_e_party;
	wk->parent_decide_pos = ROULETTE_DECIDE_NONE;
	wk->fro_sv			= SaveData_GetFrontier( wk->sv );
	wk->p_weather		= roulette_call->p_weather;
	wk->p_csr_speed_level = roulette_call->p_csr_speed_level;
	wk->p_decide_ev_no	= roulette_call->p_decide_ev_no;
	wk->p_rand_pos		= roulette_call->p_rand_pos;
	OS_Printf( "p_csr_speed_level = %d\n", *wk->p_csr_speed_level );
	wk->game_timer		= ROULETTE_GAME_TIME;
	wk->random_flag		= roulette_call->random_flag;

	for( i=0; i < ROULETTE_EV_MAX ;i++ ){
		wk->ev_tbl[i] = ROULETTE_EV_NONE;
	}

	wk->h_max = ROULETTE_PANEL_H_MAX;
	wk->v_max = ROULETTE_PANEL_V_MAX;
	wk->hv_max= ROULETTE_PANEL_MAX;

	//wk->pair_csr_pos	= (wk->h_max / 2);
	wk->pair_csr_pos	= 0;

	//���ʂ̃����_�������߂Ă���
	(*wk->p_rand_pos) = Roulette_GetRandPos( wk );

	//�J�n�J�[�\���ʒu
	wk->start_csr_pos = ( gf_rand() % ROULETTE_PANEL_MAX );

	Roulette_InitSub2( wk );

	if( Roulette_CommCheck(wk->type) == TRUE ){
		CommCommandFrontierInitialize( wk );
	}

	(*seq) = SEQ_GAME_INIT;
	OS_Printf( "(*seq) = %d\n", (*seq) );

	debug_panel_no = ROULETTE_EV_ENEMY_HP_DOWN;			//�f�o�b�N�p�l���i���o�[
	debug_panel_flag = 0;

	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief	�v���Z�X�֐��F���C��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	"������"
 */
//--------------------------------------------------------------
PROC_RESULT RouletteProc_Main( PROC * proc, int * seq )
{
	ROULETTE_WORK* wk  = PROC_GetWork( proc );
	//OS_Printf( "(*seq) = %d\n", (*seq) );

	//BgCheck( wk );

	if( wk->parent_decide_pos != ROULETTE_DECIDE_NONE ){
		switch( *seq ){

		//����M�֋����ړ�
		case SEQ_GAME_CSR_MOVE:
			OS_Printf( "�{�^�������ꂽ�I\n" );
			Roulette_SetButtonPush( wk );

			//��M�����J�[�\���ʒu���Z�b�g
			wk->suberi_csr_pos = GetCommSelCsrPos( wk->hv_max, wk->parent_decide_pos );
			//RouletteObj_Vanish( wk->p_csr, ROULETTE_VANISH_ON );					//��\��

			NextSeq( wk, seq, SEQ_GAME_SEND_RECV );
			break;

		//�������Ȃ�
		//case SEQ_GAME_INIT:
		//case SEQ_GAME_SEND_RECV:
		//case SEQ_GAME_END_MULTI:
		//case SEQ_GAME_END:
		//	break;
		};
	}

	switch( *seq ){

	//-----------------------------------
	//������
	case SEQ_GAME_INIT:
		if( Seq_GameInit(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_CSR_MOVE );					//�J�[�\���ړ�����
		}
		break;

	//-----------------------------------
	//�J�[�\���ړ���
	case SEQ_GAME_CSR_MOVE:

		if( Seq_GameCsrMove(wk) == TRUE ){

			if( wk->rankup_req == 1 ){
				NextSeq( wk, seq, SEQ_GAME_SEND_RECV );					//����M��
			}else{

				//�ʐM�^�C�v�̎�
				if( Roulette_CommCheck(wk->type) == TRUE ){
					NextSeq( wk, seq, SEQ_GAME_END_MULTI );				//(�ʐM)�I����
				}else{
					NextSeq( wk, seq, SEQ_GAME_END );					//�I����
				}
			}

		}
		break;

	//-----------------------------------
	//����M
	case SEQ_GAME_SEND_RECV:
		if( Seq_GameSendRecv(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_END_MULTI );						//(�ʐM)�I����
		}
		break;

	//-----------------------------------
	//�ʐM�����I��
	case SEQ_GAME_END_MULTI:
		if( Seq_GameEndMulti(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_END );							//�I����
		}
		break;

	//-----------------------------------
	//�I��
	case SEQ_GAME_END:
		if( Seq_GameEnd(wk) == TRUE ){
			return PROC_RES_FINISH;
		}
		break;

	}

#if 1
	//�p�l�����oBG�̃X�N���[���؂�ւ�
	wk->panel_wait++;
	if( wk->panel_wait >= ROULETTE_PANEL_EFF_WAIT ){
		wk->panel_wait = 0;
		wk->panel_scrn ^= 1;
		//Roulette_SetPanelEffBgGraphic( wk, BR_FRAME_EFF, (wk->panel_scrn + 1) );	//1��2��n��
	}
#endif

	CLACT_Draw( wk->roulette_clact.ClactSet );		//�Z���A�N�^�[�풓�֐�

	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief	�v���Z�X�֐��F�I��
 *
 * @param	proc	�v���Z�X�f�[�^
 * @param	seq		�V�[�P���X
 *
 * @return	"������"
 */
//--------------------------------------------------------------
PROC_RESULT RouletteProc_End( PROC * proc, int * seq )
{
	int i;
	ROULETTE_WORK* wk = PROC_GetWork( proc );

	*(wk->p_ret_work) = wk->csr_pos;					//�߂�l�i�[���[�N�֑��(�J�[�\���ʒu)

	OS_Printf( "*(wk->p_ret_work) = %d\n", *(wk->p_ret_work) );

	DellVramTransferManager();

	RouletteCommon_Delete( wk );						//�폜����

	PROC_FreeWork( proc );								//���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_DeleteHeap( HEAPID_ROULETTE );

	Overlay_UnloadID( FS_OVERLAY_ID(frontier_common) );

	return PROC_RES_FINISH;
}


//==============================================================================================
//
//	�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�Q�[��������
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( ROULETTE_WORK* wk )
{
	switch( wk->sub_seq ){

	//�����J�n
	case 0:
		if( Roulette_CommCheck(wk->type) == TRUE ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_ROULETTE_INIT_5 );
			wk->sub_seq++;
		}else{
			wk->sub_seq++;
		}
		break;

	//�����҂�
	case 1:
		if( Roulette_CommCheck(wk->type) == TRUE ){
			//�^�C�~���O�R�}���h���͂������m�F
			if( CommIsTimingSync(DBC_TIM_ROULETTE_INIT_5) == TRUE ){
				CommToolTempDataReset();
				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;

	//�p�l���I�o��1
	case 2:
		Roulette_EvPanelChoice( wk );
		wk->sub_seq++;
		break;

	//�p�l���I�o��2
	case 3:
		Roulette_EvPanelChoice2( wk );
		wk->sub_seq++;
		break;

	//�p�l���I�o��3
	case 4:
		Roulette_EvPanelChoice3( wk );
		wk->sub_seq++;
		break;

	//�����J�n
	case 5:
		if( Roulette_CommCheck(wk->type) == TRUE ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_ROULETTE_PANEL );
			wk->sub_seq++;
		}else{
			wk->sub_seq++;
		}
		break;

	//�����҂�
	case 6:
		if( Roulette_CommCheck(wk->type) == TRUE ){
			//�^�C�~���O�R�}���h���͂������m�F
			if( CommIsTimingSync(DBC_TIM_ROULETTE_PANEL) == TRUE ){
				CommToolTempDataReset();
				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;

	//��{��񑗐M
	case 7:
		if( Roulette_CommCheck(wk->type) == TRUE ){
			if( Roulette_CommSetSendBuf(wk,ROULETTE_COMM_RANK_PAIR,0) == TRUE ){
				wk->sub_seq++;
			}
		}else{
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
							WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_ROULETTE );
			wk->sub_seq++;
		}
		break;

	//��M�҂��A�u���b�N�C��
	case 8:
		if( Roulette_CommCheck(wk->type) == TRUE ){
			if( wk->recieve_count >= ROULETTE_COMM_PLAYER_NUM ){
				wk->recieve_count = 0;

				WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
								WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_ROULETTE );
				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;
		
	//�t�F�[�h�I���҂�
	case 9:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ���
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCsrMove( ROULETTE_WORK* wk )
{
	u16 x,y;
	u16 now_cp;
	int i;
	u32 ret,color;
	u16 buf16[4];

	enum{
		SEQ_SUB_START = 0,
		SEQ_SUB_START2,
		SEQ_SUB_START_SUB,
		SEQ_SUB_COUNT_3,
		SEQ_SUB_COUNT_2,
		SEQ_SUB_COUNT_1,
		SEQ_SUB_RANKUP_SEL,
		SEQ_SUB_CSR_STOP,
		SEQ_SUB_PANEL_WAIT,
	};

	switch( wk->sub_seq ){

	case SEQ_SUB_START:

#if 0
		PaletteFadeReq( wk->pfd, PF_BIT_MAIN_BG, 0xffff, 1, 
						0,									//�J�n�Z�x
						16,									//�ŏI�Z�x 
						0x0000 );							//�ύX��̐F
		SoftFadePfd( wk->pfd, FADE_MAIN_BG, 0, 16*13, 16, 0x0000 );
#endif

		wk->wait = 0;
		wk->csr_pos = wk->start_csr_pos;
		CsrPosSet( wk, wk->csr_pos );
		wk->sub_seq = SEQ_SUB_START2;
		break;

	case SEQ_SUB_START2:
		wk->wait++;
		if( wk->wait >= ROULETTE_START_WAIT ){
			wk->wait = 0;
			wk->sub_seq = SEQ_SUB_START_SUB;
		}
		break;

	case SEQ_SUB_START_SUB:
		//�J�E���g3
		for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
			if( wk->p_panel[i] != NULL ){
				RouletteObj_AnmChg( wk->p_panel[i], ROULETTE_ANM_COUNT_3 );
				RouletteObj_Vanish( wk->p_panel[i], ROULETTE_VANISH_OFF );			//�\��
			}
		}

		Roulette_SetMain2BgGraphic( wk, BR_FRAME_BG );	//���ʃp�l����\�����鎞�̔w�i
		Snd_SePlay( SEQ_SE_DP_WIN_OPEN2 );
		wk->wait = ROULETTE_COUNT_321_WAIT;
		wk->sub_seq = SEQ_SUB_COUNT_3;
		break;

	case SEQ_SUB_COUNT_3:
		wk->wait--;
		if( wk->wait > 0 ){
			break;
		}

		//�J�E���g2
		for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
			if( wk->p_panel[i] != NULL ){
				RouletteObj_AnmChg( wk->p_panel[i], ROULETTE_ANM_COUNT_2 );
			}
		}

		Snd_SePlay( SEQ_SE_DP_WIN_OPEN2 );
		wk->wait = ROULETTE_COUNT_321_WAIT;
		wk->sub_seq = SEQ_SUB_COUNT_2;
		break;

	case SEQ_SUB_COUNT_2:
		wk->wait--;
		if( wk->wait > 0 ){
			break;
		}

		//�J�E���g1
		for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
			if( wk->p_panel[i] != NULL ){
				RouletteObj_AnmChg( wk->p_panel[i], ROULETTE_ANM_COUNT_1 );
			}
		}

		Snd_SePlay( SEQ_SE_DP_WIN_OPEN2 );
		wk->wait = ROULETTE_COUNT_321_WAIT;
		wk->sub_seq = SEQ_SUB_COUNT_1;
		break;

	case SEQ_SUB_COUNT_1:
		wk->wait--;
		if( wk->wait > 0 ){
			break;
		}

		//�p�l���Z�b�g
		for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
			if( wk->p_panel[i] != NULL ){
				RouletteObj_AnmChg( wk->p_panel[i], wk->panel_no[i] );
			}
		}

		RouletteObj_Vanish( wk->p_csr, ROULETTE_VANISH_OFF );					//�\��
		Snd_SePlay( SEQ_SE_DP_UG_020 );
		RouletteObj_AnmChg( wk->p_button, ROULETTE_ANM_BUTTON_STOP );
		wk->sub_seq = SEQ_SUB_RANKUP_SEL;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�����N�A�b�v��I��
	case SEQ_SUB_RANKUP_SEL:
#ifdef PM_DEBUG
		if( sys.trg & PAD_BUTTON_L ){
			debug_panel_no++;
			if( debug_panel_no >= ROULETTE_EV_MAX ){
				debug_panel_no = ROULETTE_EV_ENEMY_HP_DOWN;
			}
			debug_panel_flag = 1;
			OS_Printf( "DEBUG panel_no = %d\n", debug_panel_no );
		}else if( sys.trg & PAD_BUTTON_R ){
			debug_panel_no+=10;
			if( debug_panel_no >= ROULETTE_EV_MAX ){
				debug_panel_no = ROULETTE_EV_ENEMY_HP_DOWN;
			}
			debug_panel_flag = 1;
			OS_Printf( "DEBUG panel_no = %d\n", debug_panel_no );
		}
#endif

		CsrMove( wk, sys.trg );

		//�e�̂݃^�C�}�[���炷
		if( CommGetCurrentID() == COMM_PARENT_ID ){
			if( wk->game_timer > 0 ){
				wk->game_timer--;
			}

			if( wk->game_timer == 0 ){
				//Snd_SePlay( SEQ_SE_DP_PINPON );	//�p�l�����艹�ŏ㏑�������̂ł���Ȃ��I
				OS_Printf( "wk->game_timer = 0�ɂȂ�܂����I\n" );
			}
		}
		
		//����ʂ̃{�^�������������A�^�C�}�[��0�ɂȂ������AA�{�^������������
		//if( Roulette_CheckButtonPush(wk) == TRUE ){
		if( (Roulette_CheckButtonPush(wk) == TRUE) || (wk->game_timer == 0) ){

			//���ׂ�𑫂����l�����肷��
			Roulette_SetCsrPosSuberi( wk );
			//RouletteObj_Vanish( wk->p_csr, ROULETTE_VANISH_ON );					//��\��

			if( Roulette_CommCheck(wk->type) == FALSE ){
				wk->sub_seq = SEQ_SUB_CSR_STOP;
				break;
			}else{
				wk->rankup_req		= 1;
				return TRUE;
			}
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�\����~����
	case SEQ_SUB_CSR_STOP:
		Roulette_SeqSubPanelDecide( wk, wk->csr_pos );
		wk->wait = ROULETTE_PANEL_WAIT;
		wk->sub_seq = SEQ_SUB_PANEL_WAIT;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�p�l���E�F�C�g����
	case SEQ_SUB_PANEL_WAIT:
		wk->wait--;
		if( wk->wait == 0 ){
			return TRUE;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	����M
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameSendRecv( ROULETTE_WORK* wk )
{
	switch( wk->sub_seq ){

	case 0:
		//�I�񂾃��N�G�X�g(��肽������)�𑗐M
		if( Roulette_CommSetSendBuf(wk,ROULETTE_COMM_RANK_UP_TYPE, wk->suberi_csr_pos) == TRUE ){
			//Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
			//Snd_SePlay( SEQ_SE_DP_BUTTON9 );
			//RouletteObj_AnmChg( wk->p_button, ROULETTE_ANM_BUTTON_PUSH );
			wk->rankup_req = 0;
			wk->sub_seq++;
		}
		break;

	case 1:
		wk->sub_seq++;
		break;

	case 2:
		if( wk->parent_decide_pos == ROULETTE_DECIDE_NONE ){
			break;
		}

		//�e�����߂������N��LR�������N�A�b�v������
		wk->recieve_count = 0;
		Roulette_SeqSubPanelDecide( wk, wk->parent_decide_pos );
		wk->sub_seq++;
		break;

	case 3:
		//if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			//�����J�n
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_ROULETTE_UP );
			wk->sub_seq++;
			break;
		//}
		break;

	//����
	case 4:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_ROULETTE_UP) == TRUE ){
			CommToolTempDataReset();
			CommToolInitialize( HEAPID_ROULETTE );	//timingSyncEnd=0xff
			wk->parent_decide_pos = ROULETTE_DECIDE_NONE;
			return TRUE;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�}���`�I����
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEndMulti( ROULETTE_WORK* wk )
{
	switch( wk->sub_seq ){

	//�u�߂�v���M
	case 0:
		wk->wait = ROULETTE_MODORU_WAIT;
		wk->sub_seq++;
		break;

	//�����J�n
	case 1:
		if( wk->wait > 0 ){
			wk->wait--;
		}

		if( wk->wait == 0 ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_ROULETTE_END );
			wk->sub_seq++;
		}
		break;

	//�����҂�
	case 2:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_ROULETTE_END) == TRUE ){
			CommToolTempDataReset();
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�I����
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( ROULETTE_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//�t�F�[�h�A�E�g
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
					WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_ROULETTE );

		wk->sub_seq++;
		break;

	//�t�F�[�h�I���҂�
	case 1:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;
	};

	return FALSE;
}


//==============================================================================================
//
//	���ʏ���
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���� �폜
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void RouletteCommon_Delete( ROULETTE_WORK* wk )
{
	u8 m_max;
	int i;

	if( wk->p_csr != NULL ){
		RouletteObj_Delete( wk->p_csr );
	}

	if( wk->p_button != NULL ){
		RouletteObj_Delete( wk->p_button );
	}

	for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
		if( wk->p_panel[i] != NULL ){
			RouletteObj_Delete( wk->p_panel[i] );
		}
	}

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	for( i=0; i < m_max ;i++ ){	
		if( wk->p_m_icon[i] != NULL ){
			RouletteObj_Delete( wk->p_m_icon[i] );
		}
		if( wk->p_e_icon[i] != NULL ){
			RouletteObj_Delete( wk->p_e_icon[i] );
		}

		if( wk->p_m_itemkeep[i] != NULL ){
			RouletteObj_Delete( wk->p_m_itemkeep[i] );
		}
		if( wk->p_e_itemkeep[i] != NULL ){
			RouletteObj_Delete( wk->p_e_itemkeep[i] );
		}
	}

	//�ʐM�A�C�R���폜
	WirelessIconEasyEnd();

	//�p���b�g�t�F�[�h�J��
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );

	//�p���b�g�t�F�[�h�V�X�e���J��
	PaletteFadeFree( wk->pfd );
	wk->pfd = NULL;

	RouletteClact_DeleteCellObject(&wk->roulette_clact);		//2D�I�u�W�F�N�g�֘A�̈�J��

	MSGMAN_Delete( wk->msgman );						//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );						//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );						//���b�Z�[�W�o�b�t�@�J��

	for( i=0; i < ROULETTE_MENU_BUF_MAX ;i++ ){
		STRBUF_Delete( wk->menu_buf[i] );				//���j���[�o�b�t�@�J��
	}

	RouletteExitBmpWin( wk->bmpwin );					//BMP�E�B���h�E�J��
	Roulette_BgExit( wk->bgl );							//BGL�폜
	ArchiveDataHandleClose( wk->hdl );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������1
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_InitSub1( void )
{
	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_HBlankIntrSet( NULL,NULL );						//HBlank�Z�b�g
	GF_Disp_GX_VisibleControlInit();
	GF_Disp_GXS_VisibleControlInit();
	GX_SetVisiblePlane( 0 );
	GXS_SetVisiblePlane( 0 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������2
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_InitSub2( ROULETTE_WORK* wk )
{
	u8 m_max;
	u16 x,y,offset_y,offset_y2;
	int i,flip;
	GF_BGL_BMPWIN* win;
	POKEMON_PARAM* poke;

	wk->hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_ROULETTE );
	Roulette_BgInit( wk );								//BG������
	Roulette_ObjInit( wk );								//OBJ������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_roulette_dat, HEAPID_ROULETTE );

	wk->wordset = WORDSET_Create( HEAPID_ROULETTE );
	wk->msg_buf = STRBUF_Create( ROULETTE_MSG_BUF_SIZE, HEAPID_ROULETTE );
	wk->tmp_buf = STRBUF_Create( ROULETTE_MSG_BUF_SIZE, HEAPID_ROULETTE );

	//������o�b�t�@�쐬
	for( i=0; i < ROULETTE_MENU_BUF_MAX ;i++ ){
		wk->menu_buf[i] = STRBUF_Create( ROULETTE_MENU_BUF_SIZE, HEAPID_ROULETTE );
	}

	//�t�H���g�p���b�g
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, BR_FONT_PAL * 32, HEAPID_ROULETTE );
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, BR_MSGFONT_PAL * 32, HEAPID_ROULETTE );

	//�r�b�g�}�b�v�ǉ�
	RouletteAddBmpWin( wk->bgl, wk->bmpwin );

	//�J�[�\��OBJ�ǉ�
	wk->p_csr = RouletteObj_Create(	&wk->roulette_clact, 
									ROULETTE_ID_CHAR_CSR, ROULETTE_ID_PLTT_CSR,
									ROULETTE_ID_CELL_CSR, 
									ROULETTE_ANM_CSR, 
									PANEL_START_X,
									PANEL_START_Y,
									0, ROULETTE_BG_PRI_LOW, DISP_MAIN );
	RouletteObj_Vanish( wk->p_csr, ROULETTE_VANISH_ON );					//��\��

	//�p�l��OBJ�ǉ�
	for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){	
		wk->p_panel[i] = RouletteObj_Create(&wk->roulette_clact, 
										ROULETTE_ID_CHAR_CSR, ROULETTE_ID_PLTT_CSR,
										ROULETTE_ID_CELL_CSR, 
										ROULETTE_ANM_COUNT_3,
										PANEL_START_X + (PANEL_WIDTH_X*(i%ROULETTE_PANEL_H_MAX)),
										PANEL_START_Y + (PANEL_WIDTH_Y*(i/ROULETTE_PANEL_H_MAX)),
										1, ROULETTE_BG_PRI_LOW, DISP_MAIN );
		RouletteObj_Vanish( wk->p_panel[i], ROULETTE_VANISH_ON );			//��\��
	}

	//�A�C�R��OBJ�ǉ�
	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	if( Roulette_CommCheck(wk->type) == FALSE ){
		offset_y  = ICON_START_Y;
		offset_y2 = ITEMKEEP_START_Y;
	}else{
		offset_y  = ICON_START_MULTI_Y;
		offset_y2 = ITEMKEEP_START_MULTI_Y;
	}

	for( i=0; i < m_max ;i++ ){	

		//�A�C�e�������Ă��邩�A�C�R��
		wk->p_m_itemkeep[i] = RouletteObj_Create(	&wk->roulette_clact, 
												ROULETTE_ID_CHAR_ITEMKEEP, 
												ROULETTE_ID_PLTT_ITEMKEEP,
												ROULETTE_ID_CELL_ITEMKEEP, 
												0,
												ITEMKEEP_MINE_START_X,
												offset_y2 + (ITEMKEEP_WIDTH_Y*i),
												0, ROULETTE_BG_PRI_LOW, DISP_MAIN );
		//RouletteObj_SetScaleAffine( wk->p_m_itemkeep[i], 1 );	//1.5�{

		wk->p_e_itemkeep[i] = RouletteObj_Create(	&wk->roulette_clact, 
												ROULETTE_ID_CHAR_ITEMKEEP, 
												ROULETTE_ID_PLTT_ITEMKEEP,
												ROULETTE_ID_CELL_ITEMKEEP, 
												0,
												ITEMKEEP_ENEMY_START_X,
												offset_y2 + (ITEMKEEP_WIDTH_Y*i),
												0, ROULETTE_BG_PRI_LOW, DISP_MAIN );
		//RouletteObj_SetScaleAffine( wk->p_e_itemkeep[i], 1 );	//1.5�{

		//�|�P�����A�C�R��
		wk->p_m_icon[i] = RouletteObj_Create(	&wk->roulette_clact, 
												ROULETTE_ID_CHAR_ICON1+i, ROULETTE_ID_PLTT_ICON,
												ROULETTE_ID_CELL_ICON, 
												POKEICON_ANM_HPMAX,
												ICON_MINE_START_X,
												offset_y + (ICON_WIDTH_Y*i),
												1, ROULETTE_BG_PRI_LOW, DISP_MAIN );
		//RouletteObj_SetScaleAffine( wk->p_m_icon[i], 1 );	//1.5�{

		wk->p_e_icon[i] = RouletteObj_Create(	&wk->roulette_clact, 
												ROULETTE_ID_CHAR_ICON5+i, ROULETTE_ID_PLTT_ICON,
												ROULETTE_ID_CELL_ICON, 
												POKEICON_ANM_HPMAX,
												ICON_ENEMY_START_X,
												offset_y + (ICON_WIDTH_Y*i),
												1, ROULETTE_BG_PRI_LOW, DISP_MAIN );
		//RouletteObj_SetScaleAffine( wk->p_e_icon[i], 1 );	//1.5�{

		//�p���b�g�؂�ւ�
		RouletteObj_IconPalChg( wk->p_m_icon[i], PokeParty_GetMemberPointer(wk->p_m_party,i) );
		RouletteObj_IconPalChg( wk->p_e_icon[i], PokeParty_GetMemberPointer(wk->p_e_party,i) );

		//�I�[�g�A�j���Ȃ�
		RouletteObj_SetAnmFlag( wk->p_m_icon[i], 0 );
		RouletteObj_SetAnmFlag( wk->p_e_icon[i], 0 );
	}

	//�A�C�e�������Ă��邩�A�C�R���̃o�j�b�V���Z�b�g
	Roulette_ItemKeepVanishSet( wk );

	//�{�^��
	wk->p_button = RouletteObj_Create(	&wk->roulette_clact, 
										ROULETTE_ID_CHAR_BUTTON, ROULETTE_ID_PLTT_BUTTON,
										ROULETTE_ID_CELL_BUTTON, 
										ROULETTE_ANM_BUTTON_START, 
										BUTTON_START_X,
										BUTTON_START_Y,
										0, ROULETTE_BG_PRI_HIGH, DISP_SUB );

	//�ʐM�A�C�R���Z�b�g
	if( CommIsInitialize() ){
//CLACT_U_WmIcon_SetReserveAreaCharManager( NNS_G2D_VRAM_TYPE_2DMAIN,GX_OBJVRAMMODE_CHAR_1D_64K );
		CLACT_U_WmIcon_SetReserveAreaCharManager( NNS_G2D_VRAM_TYPE_2DMAIN,
												  GX_OBJVRAMMODE_CHAR_1D_32K );
		CLACT_U_WmIcon_SetReserveAreaPlttManager( NNS_G2D_VRAM_TYPE_2DMAIN );
		WirelessIconEasy();
	}

	sys_VBlankFuncChange( VBlankFunc, (void*)wk );		//VBlank�Z�b�g
	return;
}


//==============================================================================================
//
//	���ʏ������A�I��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	BG�֘A������
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_BgInit( ROULETTE_WORK* wk )
{
	SetVramBank();
	SetBgHeader( wk->bgl );

	//�p���b�g�t�F�[�h�V�X�e�����[�N�쐬
	wk->pfd = PaletteFadeInit( HEAPID_ROULETTE );

	//���N�G�X�g�f�[�^��malloc���ăZ�b�g
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_OBJ, FADE_PAL_ALL_SIZE, HEAPID_ROULETTE );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ALL_SIZE, HEAPID_ROULETTE );

	//����
	Roulette_SetMainBgGraphic( wk, BR_FRAME_BG );					//�w�i
	//Roulette_SetPanelEffBgGraphic( wk, BR_FRAME_EFF, 0 );			//�p�l���G�t�F�N�g
	Roulette_SetMainBgPalette();
	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��

	//����ʔw�i�A�p���b�g�Z�b�g
	Roulette_SetSubBgGraphic( wk, BR_FRAME_SUB );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�֘A������
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_ObjInit( ROULETTE_WORK* wk )
{
	//RouletteClact_InitCellActor( &wk->roulette_clact );
	RouletteClact_InitCellActor(&wk->roulette_clact, 
								wk->p_m_party, wk->p_e_party, 
								Roulette_CommCheck(wk->type) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BG���
 *
 * @param	ini		BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_BgExit( GF_BGL_INI * ini )
{
	//���C����ʂ̊e�ʂ̕\���R���g���[��(�\��OFF)
	GF_Disp_GX_VisibleControl(	GX_PLANEMASK_BG0 | 
								GX_PLANEMASK_BG1 | 
								GX_PLANEMASK_BG2 |
								GX_PLANEMASK_BG3 | 
								GX_PLANEMASK_OBJ, 
								VISIBLE_OFF );

	//�T�u��ʂ̊e�ʂ̕\���R���g���[��(�\��OFF)
	GF_Disp_GXS_VisibleControl(	GX_PLANEMASK_BG0 | 
								GX_PLANEMASK_BG1 | 
								GX_PLANEMASK_BG2 | 
								GX_PLANEMASK_BG3 | 
								GX_PLANEMASK_OBJ, 
								VISIBLE_OFF );

	//GF_BGL_BGControlSet�Ŏ擾�������������J��
	GF_BGL_BGControlExit( ini, BR_FRAME_BG );			//3
	//GF_BGL_BGControlExit( ini, BR_FRAME_EFF );
	//GF_BGL_BGControlExit( ini, BR_FRAME_TYPE );
	GF_BGL_BGControlExit( ini, BR_FRAME_WIN );
	GF_BGL_BGControlExit( ini, BR_FRAME_SUB );

	sys_FreeMemoryEz( ini );
	return;
}


//==============================================================================================
//
//	�ݒ�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	VBlank�֐�
 *
 * @param	work	���[�N
 *
 * @return	none
 */
//--------------------------------------------------------------
static void VBlankFunc( void * work )
{
	ROULETTE_WORK* wk = work;

	//�p���b�g�]��
	if( wk->pfd != NULL ){
		PaletteFadeTrans( wk->pfd );
	}

	GF_BGL_VBlankFunc( wk->bgl );

	//�Z���A�N�^�[
	//Vram�]���}�l�[�W���[���s
	DoVramTransferManager();

	//�����_�����LOAM�}�l�[�W��Vram�]��
	REND_OAMTrans();	

	OS_SetIrqCheckFlag( OS_IE_V_BLANK );
}

//--------------------------------------------------------------
/**
 * @brief	VRAM�ݒ�
 *
 * @param	none
 *
 * @return	none
 *
 * �ׂ����ݒ肵�Ă��Ȃ��̂Œ��ӁI
 */
//--------------------------------------------------------------
static void SetVramBank(void)
{
	GF_BGL_DISPVRAM tbl = {
		GX_VRAM_BG_128_C,				//���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			//���C��2D�G���W����BG�g���p���b�g

		GX_VRAM_SUB_BG_32_H,			//�T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		//�T�u2D�G���W����BG�g���p���b�g

		GX_VRAM_OBJ_64_E,				//���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		//���C��2D�G���W����OBJ�g���p���b�g

		GX_VRAM_SUB_OBJ_16_I,			//�T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	//�T�u2D�G���W����OBJ�g���p���b�g

		GX_VRAM_TEX_01_AB,				//�e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_01_FG			//�e�N�X�`���p���b�g�X���b�g
	};

	GF_Disp_SetBank( &tbl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BG�ݒ�
 *
 * @param	init	BGL�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetBgHeader( GF_BGL_INI * ini )
{
	{	//BG SYSTEM
		GF_BGL_SYS_HEADER BGsys_data = {
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	{	//BG(�t�H���g)(��b�A���j���[)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BR_FRAME_WIN, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BR_FRAME_WIN, 32, 0, HEAPID_ROULETTE );
		GF_BGL_ScrClear( ini, BR_FRAME_WIN );
	}

#if 0
	{	//����(�p�l���G�t�F�N�g)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			//GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			//GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BR_FRAME_EFF, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BR_FRAME_EFF );
	}
#endif

	{	//����(SINGLE,DOUBLE)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BR_FRAME_BG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BR_FRAME_BG );
	}

	//--------------------------------------------------------------------------------
	{	//�����(�{�[��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BR_FRAME_SUB, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BR_FRAME_SUB );
	}

	G2_SetBG0Priority( 0 );
	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );		//��\��
	return;
}


//==============================================================================================
//
//	BG�O���t�B�b�N�f�[�^
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʔw�i
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SetMainBgGraphic( ROULETTE_WORK * wk, u32 frm )
{
	u32 scrn;

	ArcUtil_HDL_BgCharSet(	wk->hdl, BR_PANEL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_ROULETTE );

	if( Roulette_CommCheck(wk->type) == FALSE ){
		scrn = BR_PANEL_NSCR_BIN;
	}else{
		scrn = BR_PANEL_MUL_NSCR_BIN;
	}
	ArcUtil_HDL_ScrnSet(wk->hdl, scrn, wk->bgl, frm, 0, 0, 1, HEAPID_ROULETTE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʃp�l��OBJ��\�����鎞�̔w�i
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SetMain2BgGraphic( ROULETTE_WORK * wk, u32 frm )
{
	u32 scrn;

	//ArcUtil_HDL_BgCharSet(	wk->hdl, BR_PANEL_NCGR_BIN, 
	//						wk->bgl, frm, 0, 0, 1, HEAPID_ROULETTE );

	if( Roulette_CommCheck(wk->type) == FALSE ){
		scrn = BR_PANEL2_NSCR_BIN;
	}else{
		scrn = BR_PANEL_MUL2_NSCR_BIN;
	}
	ArcUtil_HDL_ScrnSet(wk->hdl, scrn, wk->bgl, frm, 0, 0, 1, HEAPID_ROULETTE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʃp�l���G�t�F�N�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 * @param	flag	0=char,scrn�Z�b�g�A1=scrn�Z�b�g�A2=scrn2�Z�b�g
 *
 * @return	none
 */
//--------------------------------------------------------------
#if 0
static void Roulette_SetPanelEffBgGraphic( ROULETTE_WORK * wk, u32 frm, u8 flag )
{
	u32 scrn;

	if( Roulette_CommCheck(wk->type) == FALSE ){
		if( flag == 2 ){
			scrn = BR_PANEL_ALPHA2_NSCR_BIN;
		}else{
			scrn = BR_PANEL_ALPHA_NSCR_BIN;
		}
	}else{
		if( flag == 2 ){
			scrn = BR_PANEL_ALPHA2_MUL_NSCR_BIN;
		}else{
			scrn = BR_PANEL_ALPHA_MUL_NSCR_BIN;
		}
	}

	if( flag == 0 ){
		ArcUtil_HDL_BgCharSet( wk->hdl, BR_PANEL_NCGR_BIN, wk->bgl, frm, 0, 0, 1, HEAPID_ROULETTE );
	}

	ArcUtil_HDL_ScrnSet( wk->hdl, scrn, wk->bgl, frm, 0, 0, 1, HEAPID_ROULETTE );

#if 1
	//�u�����h�ݒ�
	G2_SetBlendAlpha(	ROULETTE_BLD_PLANE_1, ROULETTE_BLD_PLANE_2, 
						ROULETTE_BLD_ALPHA_1, ROULETTE_BLD_ALPHA_2 );
#endif

	return;
}
#endif

//--------------------------------------------------------------
/**
 * @brief	���ʔw�i�p���b�g�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SetMainBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	//buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BR_RANK_NCLR, &dat, HEAPID_ROULETTE );
	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BR_PANEL_NCLR, &dat, HEAPID_ROULETTE );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*7) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*7) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i�A�p���b�g�Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SetSubBgGraphic( ROULETTE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BR_BUTTON_BG_NCGR_BIN, wk->bgl, frm,
							0, 0, TRUE, HEAPID_ROULETTE );

	ArcUtil_HDL_PalSet(	wk->hdl, BR_BUTTON_BG_NCLR, PALTYPE_SUB_BG,
						0, (sizeof(u16)*16*2), HEAPID_ROULETTE );
	
	ArcUtil_HDL_ScrnSet(wk->hdl, BR_BUTTON_BG_NSCR_BIN, wk->bgl, frm,
						0, 0, TRUE, HEAPID_ROULETTE );
	return;
}


//==============================================================================================
//
//	���b�Z�[�W�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�\��
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	msg_id	���b�Z�[�WID
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	wait	�����\���E�F�C�g
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 *
 * @return	"�����`�惋�[�`���̃C���f�b�N�X"
 */
//--------------------------------------------------------------
static u8 RouletteWriteMsg( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	GF_BGL_BmpWinDataFill( win, b_col );			//�h��Ԃ�
	MSGMAN_GetString( wk->msgman, msg_id, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	return GF_STR_PrintColor( win, font, wk->msg_buf, x, y, wait, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );
}

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�\��(�h��Ԃ��Ȃ�)
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	msg_id	���b�Z�[�WID
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	wait	�����\���E�F�C�g
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 *
 * @return	"�����`�惋�[�`���̃C���f�b�N�X"
 *
 * �h��Ԃ��Ȃ�
 */
//--------------------------------------------------------------
static u8 RouletteWriteMsgSimple( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	MSGMAN_GetString( wk->msgman, msg_id, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	return GF_STR_PrintColor( win, font, wk->msg_buf, x, y, wait, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );
}

//--------------------------------------------------------------
/**
 * @brief	�ȒP���b�Z�[�W�\��
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Roulette_EasyMsg( ROULETTE_WORK* wk, int msg_id )
{
	u8 msg_index;

	msg_index = RouletteWriteMsg(	wk, &wk->bmpwin[RANK_BMPWIN_TALK], msg_id, 
								1, 1, MSG_ALLPUT, 
								//1, 1, CONFIG_GetMsgSpeed(wk->config), 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BR_FONT );

	GF_BGL_BmpWinOn( &wk->bmpwin[RANK_BMPWIN_TALK] );
	return msg_index;
}


//==============================================================================================
//
//	���j���[�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[	�w�b�_�[�����ݒ�
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	win		GF_BGL_BMPWIN�^�̃|�C���^
 * @param	y_max	���ڍő吔
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void RouletteInitMenu( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max )
{
	int i;

	for( i=0; i < ROULETTE_MENU_BUF_MAX ;i++ ){
		wk->Data[i].str	  = NULL;
		wk->Data[i].param = 0;
	}

	wk->MenuH.menu		= wk->Data;
	wk->MenuH.win		= win;
	wk->MenuH.font		= BR_FONT;
	wk->MenuH.x_max		= 1;
	wk->MenuH.y_max		= y_max;
	wk->MenuH.line_spc	= 0;
	wk->MenuH.c_disp_f	= 0;
	//wk->MenuH.c_disp_f	= 1;			//�J�[�\���Ȃ�
	//wk->MenuH.loop_f	= 0;			//���[�v����
	wk->MenuH.loop_f	= 1;			//���[�v�L��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[	�f�[�^�Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	no		�Z�b�g����ꏊ(�߂�l)
 * @param	param	�߂�l
 * @param	msg_id	���b�Z�[�WID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void RouletteSetMenuData( ROULETTE_WORK* wk, u8 no, u8 param, int msg_id )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( no < ROULETTE_MENU_BUF_MAX, "���j���[���ڐ��I�[�o�[�I" );

	MSGMAN_GetString( wk->msgman, msg_id, wk->menu_buf[no] );

	wk->Data[ no ].str = (const void *)wk->menu_buf[no];
	wk->Data[ no ].param = param;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g2�u�͂��A�������v
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SetMenu2( ROULETTE_WORK* wk )
{
	RouletteWriteMenuWin( wk->bgl, &wk->bmpwin[RANK_BMPWIN_YESNO] );
	RouletteInitMenu( wk, &wk->bmpwin[RANK_BMPWIN_YESNO], 2 );
	RouletteSetMenuData( wk, 0, 0, msg_roulette_02_01 );				//�͂�
	RouletteSetMenuData( wk, 1, 1, msg_roulette_02_02 );				//������
	wk->mw = BmpMenuAddEx( &wk->MenuH, 8, 0, 0, HEAPID_ROULETTE, PAD_BUTTON_CANCEL );
	return;
}


//==============================================================================================
//
//	������Z�b�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���l���Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * �����Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void Roulette_SetNumber( ROULETTE_WORK* wk, u32 bufID, s32 number, u32 keta, NUMBER_DISPTYPE disp )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, keta, disp, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P���������Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	ppp		POKEMON_PASO_PARAM
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Roulette_SetPokeName( ROULETTE_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp )
{
	WORDSET_RegisterPokeMonsName( wk->wordset, bufID, ppp );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�����Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Roulette_SetPlayerName( ROULETTE_WORK* wk, u32 bufID )
{
	WORDSET_RegisterPlayerName( wk->wordset, bufID, SaveData_GetMyStatus(wk->sv) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[����\��
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 *
 * @return	none
 */
//--------------------------------------------------------------
static void PlayerNameWrite( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	const MYSTATUS* my;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^

	my = SaveData_GetMyStatus( wk->sv );
	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_ROULETTE );
	
	//GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	STRBUF_SetStringCode( player_buf, MyStatus_GetMyName(my) );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_ALLPUT, col, NULL );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��

	//GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[����\��
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 *
 * @return	none
 */
//--------------------------------------------------------------
static void PairNameWrite( ROULETTE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	MYSTATUS* my;									//�p�[�g�i�[��MyStatus
	u32 col;

	//�p�[�g�i�[��MyStatus���擾
	my = CommInfoGetMyStatus( 1 - CommGetCurrentID() );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	WORDSET_RegisterPlayerName( wk->wordset, 0, my );

	RouletteWriteMsgSimple(	wk, win, msg_roulette_name_02, 
							x, y, MSG_ALLPUT, 
							GF_PRINTCOLOR_GET_LETTER(col),
							GF_PRINTCOLOR_GET_SHADOW(col),
							GF_PRINTCOLOR_GET_GROUND(col),
							//FBMP_COL_NULL,
							BR_FONT );
								
	//GF_BGL_BmpWinOn( win );
	return;
}


//==============================================================================================
//
//	�c�[��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���݂�type�`�F�b�N�֐�
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	type	�`�F�b�N����^�C�v
 *
 * @return	"TRUE = type�������AFALSE = type���Ⴄ"
 */
//--------------------------------------------------------------
static BOOL Roulette_CheckType( ROULETTE_WORK* wk, u8 type )
{
	if( wk->type == type ){
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�ύX
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( ROULETTE_WORK* wk, int* seq, int next )
{
	wk->sub_seq = 0;
	*seq		= next;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�L�[�`�F�b�N
 *
 * @param	key		�`�F�b�N����L�[
 *
 * @return	"����"
 */
//--------------------------------------------------------------
static int KeyCheck( int key )
{
	return (sys.cont & key);
}

//--------------------------------------------------------------
/**
 * @brief	BG�`�F�b�N
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck( ROULETTE_WORK* wk )
{
	if( sys.cont & PAD_KEY_UP ){
		if( sys.cont & PAD_BUTTON_SELECT ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_START ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_DOWN ){
		if( sys.cont & PAD_BUTTON_SELECT ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_START ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_LEFT ){
		if( sys.cont & PAD_BUTTON_SELECT ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_START ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_RIGHT ){
		if( sys.cont & PAD_BUTTON_SELECT ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
			//GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_START ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
			//GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�\���L�[����
 *
 * @param	key		�`�F�b�N����L�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CsrMove( ROULETTE_WORK* wk, int key )
{
	u8 level;

	level = *wk->p_csr_speed_level;

	//����Ȃ�
	wk->csr_wait++;

	if( wk->csr_wait >= csr_wait_tbl[level][0] ){
		wk->csr_wait = 0;

		if( wk->random_flag == 1 ){		//�J�[�\���������_���̎�
			wk->csr_pos = ( gf_rand() % ROULETTE_PANEL_MAX );
		}else{
			wk->csr_pos++;
		}

		//Snd_SePlay( SEQ_SE_DP_SELECT );
		Snd_SePlay( SEQ_SE_DP_BUTTON3 );

		if( wk->csr_pos >= wk->hv_max ){
			wk->csr_pos = 0;
		}

		CsrPosSet( wk, wk->csr_pos );
	}

	return;
}

static void CsrPosSet( ROULETTE_WORK* wk, u8 csr_pos )
{
	RouletteObj_SetObjPos(	wk->p_csr,
							PANEL_START_X + (PANEL_WIDTH_X*(csr_pos%ROULETTE_PANEL_H_MAX)),
							PANEL_START_Y + (PANEL_WIDTH_Y*(csr_pos/ROULETTE_PANEL_H_MAX)));
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���w�擾
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
static u16 GetCsrX( ROULETTE_WORK* wk, u8 csr_pos, u8 lr )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,lr_offset_x;

	if( lr == 0 ){						//��
		lr_offset_x = SEL_CSR_L_OFFSET;
	}else{								//�E
		lr_offset_x = SEL_CSR_R_OFFSET;
	}

	if( csr_pos >= (wk->hv_max-1) ){
		return (CSR_MODORU_X + lr_offset_x);
	}

	Roulette_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	if( Roulette_CommCheck(wk->type) == TRUE ){
		if( (csr_pos % wk->h_max) == 0 ){
			//OS_Printf( "GetCsrX = %d\n", (lr_offset_x + offset_x) );
			return ( lr_offset_x + offset_x );
		}else{
			//OS_Printf( "GetCsrX = %d\n", (lr_offset_x + pair_offset_x) );
			return ( lr_offset_x + pair_offset_x );
		}
	}

	//OS_Printf( "GetCsrX = %d\n", (lr_offset_x + offset_x) );
	return ( lr_offset_x + offset_x );
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���x�擾
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"Y"
 */
//--------------------------------------------------------------
//�J�[�\���x���W(�����N�A�b�v���ڂ���)
static const u16 csr_y_tbl[] = {
	72, 104, 136, 180,
};

static u16 GetCsrY( ROULETTE_WORK* wk, u8 csr_pos )
{
	if( csr_pos >= (wk->hv_max-1) ){
		return CSR_MODORU_Y;
	}

	//OS_Printf( "GetCsrY = %d\n", csr_y_tbl[csr_pos / wk->h_max] );
	return csr_y_tbl[csr_pos / wk->h_max];
}

//--------------------------------------------------------------
/**
 * @brief	�I�t�Z�b�g�擾
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_GetOffset( ROULETTE_WORK* wk, u16* offset_x, u16* offset_y, u16* pair_offset_x, u16* pair_offset_y )
{
	//�ʐM���Ă��邩�ŕ\���̃I�t�Z�b�g���ς��
	if( Roulette_CommCheck(wk->type) == FALSE ){
		*offset_x		= INFO_SINGLE_OFFSET_X;
		*offset_y		= 0;
		*pair_offset_x	= 0;
		*pair_offset_y	= 0;
	}else{
		*offset_x		= INFO_MULTI_L_OFFSET_X;
		*offset_y		= 0;
		*pair_offset_x	= INFO_MULTI_R_OFFSET_X;
		*pair_offset_y	= 0;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���ׂ�𑫂����l�����肷��(����)
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SetCsrPosSuberi( ROULETTE_WORK* wk )
{
	wk->suberi_csr_pos = wk->csr_pos;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�ʐM�̑I�񂾃J�[�\���ʒu���擾
 *
 * @param	hv_max	�c���̐�
 * @param	csr_pos	�J�[�\���ʒu
 *
 * @return	"�J�[�\���ʒu"
 */
//--------------------------------------------------------------
static u8 GetCommSelCsrPos( u8 type_offset, u8 decide_type )
{
	//�I�t�Z�b�g���O��
	if( decide_type < type_offset ){
		return decide_type;
	}

	return (decide_type - type_offset);
}

//--------------------------------------------------------------
/**
 * @brief	"�e�q"�̘A�������r���āA�g�p����A�������擾
 *
 * @param	wk		ROULETTE_SCRWORK�^�̃|�C���^
 *
 * @return	"�A����"
 */
//--------------------------------------------------------------
static u16 Roulette_CommGetRensyou( ROULETTE_WORK* wk )
{
	u16	rensyou;
	
	rensyou = wk->rensyou;

	if( Roulette_CommCheck(wk->type) == TRUE ){

		//�ʐM���ɂ͎��񐔂̑����ق��Œ��I
		if( wk->pair_rensyou > wk->rensyou ){
			rensyou = wk->pair_rensyou;
		}
	}

	OS_Printf( "wk->rensyou = %d\n", wk->rensyou );
	OS_Printf( "wk->pair_rensyou = %d\n", wk->pair_rensyou );
	OS_Printf( "rensyou = %d\n", rensyou );
	return rensyou;
}


//==============================================================================================
//
//	�ʐM(CommStart)
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���M�E�F�C�g�@
 *
 * @param	wk			ROULETTE_WORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL Roulette_CommSetSendBuf( ROULETTE_WORK* wk, u16 type, u16 param )
{
	int ret,command;

	switch( type ){

	//���O
	case ROULETTE_COMM_RANK_PAIR:
		command = FC_ROULETTE_PAIR;
		Roulette_CommSendBufBasicData( wk, type );
		break;

	//�����N�A�b�v���������N�G�X�g
	case ROULETTE_COMM_RANK_UP_TYPE:
		command = FC_ROULETTE_UP_TYPE;
		Roulette_CommSendBufRankUpType( wk, type, param );
		break;

	//�J�[�\���ʒu
	case ROULETTE_COMM_RANK_CSR_POS:
		command = FC_ROULETTE_CSR_POS;
		Roulette_CommSendBufCsrPos( wk, type );
		break;

	};

	if( CommSendData(command,wk->send_buf,ROULETTE_COMM_BUF_LEN) == TRUE ){
		ret = TRUE;
	}else{
		ret = FALSE;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɋ�{�����Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Roulette_CommSendBufBasicData( ROULETTE_WORK* wk, u16 type )
{
	int i,num;
	MYSTATUS* my;
	
	num = 0;
	my	= SaveData_GetMyStatus( wk->sv );

	wk->send_buf[num] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );
	num+=1;														//1

	//wk->send_buf[num] = wk->rensyou;
	//OS_Printf( "���M�Frensyou = %d\n", wk->send_buf[1] );
	num+=1;														//2

	//�p�l��
	for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
		wk->send_buf[num+i] = wk->panel_no[i];
		OS_Printf( "���M�Fpanel_no = %d\n", wk->send_buf[num+i] );
	}
	num+=ROULETTE_PANEL_MAX;									//18

	//�J�n�J�[�\���ʒu
	wk->send_buf[num] = wk->start_csr_pos;						//19
	num+=1;

	return;
}

//--------------------------------------------------------------
/**
 * @brief   ��{��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Roulette_CommRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	ROULETTE_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** ��{����M\n" );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//type = recv_buf[0];
	num+=1;													//1
	
	//wk->pair_rensyou = (u16)recv_buf[num];
	//OS_Printf( "��M�Fwk->pair_rensyou = %d\n", wk->pair_rensyou );
	num+=1;													//2

	//�q�̂ݎ󂯎��(���[���b�g��ʂɓn���ꂽlap�́A���łɎ��񐔑������̒l�ɂȂ��Ă���)
	if( CommGetCurrentID() != COMM_PARENT_ID ){

		//�p�l��
		for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
			wk->panel_no[i] = (u8)recv_buf[num+i];
			OS_Printf( "��M�Fwk->panel_no[%d] = %d\n", i, wk->panel_no[i] );
		}
		num+=ROULETTE_PANEL_MAX;							//18

		//�J�n�J�[�\���ʒu
		wk->start_csr_pos = (u16)recv_buf[num];				//19
		num+=1;
	}

	//�f�o�b�N****************************************************/
	OS_Printf( "***************\n�ŏI���肳�ꂽ�C�x���g\n" );
	for( i=0; i < ROULETTE_PANEL_MAX; i++ ){
		OS_Printf( "wk->panel_no[%d] = %d\n", i, wk->panel_no[i] );
	}
	/*************************************************************/

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ��N�G�X�g(�ǂ�������N�A�b�v������)���Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Roulette_CommSendBufRankUpType( ROULETTE_WORK* wk, u16 type, u16 param )
{
	u8 m_max;

	//�R�}���h
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	//�J�[�\���ʒu
	wk->send_buf[1] = param;
	OS_Printf( "���M�Fcsr_pos = %d\n", wk->send_buf[1] );

	//��Ɏq�̑I�������Ă��Ȃ��āA�܂��l�������Ă��Ȃ����́A�e�̌���̓Z�b�g���Ă��܂�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		if( wk->parent_decide_pos == ROULETTE_DECIDE_NONE ){
			wk->parent_decide_pos = param;
		}
	}

	//�e�̌���^�C�v
	wk->send_buf[2] = wk->parent_decide_pos;
	OS_Printf( "���M�Fparent_decide_pos = %d\n", wk->send_buf[2] );

#if 1
	//���ʂ̃����_��(�Ƃ������|�P�����̈ʒu���ǂ����猩�Ă�����)�����߂Ă���
	wk->send_buf[3] = *wk->p_rand_pos;
	OS_Printf( "���M�Frand_pos = %d\n", wk->send_buf[3] );
#endif

	return;
}

//���ʂ̃����_�������߂Ă���
static u16 Roulette_GetRandPos( ROULETTE_WORK* wk )
{
	return gf_rand();
}

//--------------------------------------------------------------
/**
 * @brief   �����N�A�b�v���������N�G�X�g �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Roulette_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work)
{
	int i,num;
	ROULETTE_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �����N�A�b�v���������N�G�X�g����M\n" );

	num = 0;
	wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//�R�}���h
	//recv_buf[0]
	
	wk->pair_sel_pos = recv_buf[1];
	OS_Printf( "��M�Fwk->pair_sel_pos = %d\n", wk->pair_sel_pos );

	////////////////////////////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�̌��肪���łɌ��܂��Ă�����A�q�̑I���͖���
		if( wk->parent_decide_pos != ROULETTE_DECIDE_NONE ){
			wk->pair_sel_pos = 0;
		}else{

			//�e�̌��肪���܂��Ă��Ȃ����́A
			//�e�����M���鎞�Ɂu�q�ɂ���ł�����v�Ƒ��M����
			//wk->parent_decide_pos	= wk->pair_sel_pos;
			
			//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g��������
			wk->parent_decide_pos	= wk->pair_sel_pos + wk->hv_max;
		}
	////////////////////////////////////////////////////////////////////////
	//�q
	}else{
		//�e�̌���^�C�v
		wk->parent_decide_pos = recv_buf[2];

#if 1
		//�e�ɍ��킹��
		//���ʂ̃����_��(�Ƃ������|�P�����̈ʒu���ǂ����猩�Ă�����)�����߂Ă���
		(*wk->p_rand_pos) = recv_buf[3];
		OS_Printf( "��M�Fwk->p_rand_pos = %d\n", *wk->p_rand_pos );
#endif

	}

	OS_Printf( "��M�Fwk->parent_decide_pos = %d\n", wk->parent_decide_pos );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɃJ�[�\���ʒu���Z�b�g
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Roulette_CommSendBufCsrPos( ROULETTE_WORK* wk, u16 type )
{
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	//wk->send_buf[1]	= wk->csr_pos;
	if( wk->csr_pos == (wk->hv_max - 1) ){		//�u���ǂ�v
		wk->send_buf[1]	= wk->csr_pos;
	}else{
		//wk->send_buf[1]	= (wk->csr_pos ^ 1);	//�ʒu�����E�t�Ȃ̂Ŕ��]
		wk->send_buf[1]	= wk->csr_pos;
	}
	OS_Printf( "���M�Fcsr_pos = %d\n", wk->send_buf[1] );

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �J�[�\���ʒu �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Roulette_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work)
{
	ROULETTE_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �J�[�\���ʒu����M\n" );

	//wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//type = recv_buf[0];
	
	wk->pair_csr_pos = (u8)recv_buf[1];						//�J�[�\���ʒu
	OS_Printf( "��M�Fwk->pair_csr_pos = %d\n", wk->pair_csr_pos );

	return;
}


//==============================================================================================
//
//	�T�u�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�p�l������
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_SeqSubPanelDecide( ROULETTE_WORK* wk, u8 decide_type )
{
	int i;
	u8	type,type_offset,pos,panel_no;
	u16 buf16[4];

	//�I�t�Z�b�g
	type_offset = wk->hv_max;	

	//�I�񂾃J�[�\���ʒu���擾
	pos = GetCommSelCsrPos( type_offset, decide_type );

	//���肵���C�x���g�i���o�[���Z�b�g
	panel_no = wk->panel_no[pos];

#ifdef PM_DEBUG
#ifdef DEBUG_DECIDE_EV_NO
	OS_Printf( "\n���f�o�b�N�Ńp�l���i���o�[�������Z�b�g���Ă��܂��I��\n" );
	(panel_no) = DEBUG_PANEL_NO;
#endif

	//�ʐM�ł́A�Z�b�g����p�l���ƁA�ǂ��Ŏ~�߂����̈ʒu���������Ă��Ȃ��̂ŁA
	//�z�u����Ă��Ȃ��p�l���́A���f���Ȃ��̂ŁA���̃f�o�b�N�͖����ƂȂ�͂��B
	if( debug_panel_flag == 1 ){
		(panel_no) = debug_panel_no;
	}
#endif

	(*wk->p_decide_ev_no) = panel_no;

	//�J�[�\�����ߐs����
	for( i=0; i < ROULETTE_PANEL_MAX ;i++ ){
		if( wk->p_panel[i] != NULL ){
			RouletteObj_AnmChg( wk->p_panel[i], panel_no );
		}
	}

	//���肵���J�[�\���ʒu�ɃJ�[�\��OBJ���ړ�
	CsrPosSet( wk, pos );

	Snd_SePlay( SEQ_SE_DP_PIRORIRO2 );
	OS_Printf( "�J�[�\���ʒu = %d\n", pos );
	OS_Printf( "�C�x���g�̎�� = %d\n", panel_no );
	//Roulette_EvSet( wk, panel_no );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�p�l���̌���
 *
 * @param	
 *
 * @return	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
static void Roulette_EvPanelChoice( ROULETTE_WORK* wk )
{
	int ev_i,i,j;
	u16 rand,num,rensyou;
	u8 challenge,flag;

	////////////////////////////////////////////////////////////////////////////
	//�C�x���g�o�����񐔃e�[�u���̂ǂ����Q�Ƃ��邩�����݂̎��񐔂Ō��߂�
	OS_Printf( "���݂̎��� = %d\n", wk->lap );

	//�o���\���e�[�u������t���O���擾
	for( ev_i=0; ev_i < ROULETTE_EV_APPEAR_MAX ;ev_i++ ){

		//���[���b�g��p��ʂɓn�����񐔂́A���������������łɓn����Ă���
		switch( wk->lap ){
		case 0:
			flag = roulette_ev_appear_tbl[ev_i].flag1;
			break;
		case 1:
			flag = roulette_ev_appear_tbl[ev_i].flag2;
			break;
		case 2:
			flag = roulette_ev_appear_tbl[ev_i].flag3;
			break;
		case 3:
			flag = roulette_ev_appear_tbl[ev_i].flag4;
			break;
		case 4:
			flag = roulette_ev_appear_tbl[ev_i].flag5;
			break;
		case 5:
			flag = roulette_ev_appear_tbl[ev_i].flag6;
			break;
		//case 6:
		//case 7:
		default:
			flag = roulette_ev_appear_tbl[ev_i].flag7;
			break;
		};
		OS_Printf( "roulette_ev_appear_tbl[%d] = %d\n", ev_i, flag );

		//�o���\��������
		if( flag == 1 ){

			challenge = 0xff;

			//����ڂ���o���\�C�x���g���`�F�b�N
			switch( ev_i ){
			case ROULETTE_EV_EX_POKE_CHANGE:
				challenge = ROULETTE_EV_CHALLENGE_POKE_CHANGE;
				break;
			case ROULETTE_EV_PLACE_SPEED_UP:
				challenge = ROULETTE_EV_CHALLENGE_SPEED_UP;
				break;
			case ROULETTE_EV_PLACE_SPEED_DOWN:
				challenge = ROULETTE_EV_CHALLENGE_SPEED_DOWN;
				break;
			case ROULETTE_EV_PLACE_RANDOM:
				challenge = ROULETTE_EV_CHALLENGE_RANDOM;
				break;
			case ROULETTE_EV_EX_BP_GET:
				challenge = ROULETTE_EV_CHALLENGE_BP_GET;
				break;
			case ROULETTE_EV_EX_BTL_WIN:
				challenge = ROULETTE_EV_CHALLENGE_BTL_WIN;
				break;
			case ROULETTE_EV_EX_BP_GET_BIG:
				challenge = ROULETTE_EV_CHALLENGE_BP_GET_BIG;
				break;
			};

			//�e�q���l�������A�������擾
			rensyou = Roulette_CommGetRensyou( wk );

			num = ( rensyou % ROULETTE_LAP_ENEMY_MAX );

			//�J���X�g���Ă�����K��7�l�ڂɂ���
			if( rensyou >= ROULETTE_RENSYOU_MAX ){
				num = 6;			//0-6
			}
			OS_Printf( "���݉��l�ڂ� = %d\n", num );

			if( challenge != 0xff ){
				switch( num ){
				case 0:
					flag = roulette_ev_challenge_tbl[challenge].flag1;
					break;
				case 1:
					flag = roulette_ev_challenge_tbl[challenge].flag2;
					break;
				case 2:
					flag = roulette_ev_challenge_tbl[challenge].flag3;
					break;
				case 3:
					flag = roulette_ev_challenge_tbl[challenge].flag4;
					break;
				case 4:
					flag = roulette_ev_challenge_tbl[challenge].flag5;
					break;
				case 5:
					flag = roulette_ev_challenge_tbl[challenge].flag6;
					break;
				case 6:
				case 7:
					flag = roulette_ev_challenge_tbl[challenge].flag7;
					break;
				default:
					OS_Printf( "�����ɂ͏��������Ȃ��͂��I\n" );
					GF_ASSERT( 0 );
					flag = 1;
					break;
				};
			}else{
				flag = 1;		//����ڂ��̃`�F�b�N���Ȃ��̂ŏo���\
			}

			if( flag == 1 ){
				wk->ev_tbl[wk->ev_tbl_max] = ev_i;		//�C�x���g�i���o�[���
				wk->ev_tbl_max++;
				OS_Printf( "�I�o�C�x���g = %d\n", ev_i );
				OS_Printf( "�I�o�C�x���g�� = %d\n", wk->ev_tbl_max );
			}
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�p�l���̌���2(�ʐM�Ȃ̂ŏ����𕪂��Ȃ��ƃ_���I)
 *
 * @param	
 *
 * @return	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
static void Roulette_EvPanelChoice2( ROULETTE_WORK* wk )
{
	int i,j;

	////////////////////////////////////////////////////////////////////////////
	//�|�C���g���ɂ���ă����N������
	for( i=0; i < ROULETTE_POINT_TBL_MAX ;i++ ){
		if( wk->point >= roulette_point_tbl[i] ){
			break;
		}
	}

	//roulette_category_tbl[����]���擾(0�ɋ߂��قǎ������L���Ȋm���ɂȂ�)
	wk->point_rank = i;		//0-3�Ńq�b�g���Ȃ�������4�ɂȂ�(���̒l��roulette_category_tbl�Q��)
	OS_Printf( "**********************\npoint_rank = %d\n", wk->point_rank );

	////////////////////////////////////////////////////////////////////////////

	//�f�o�b�N*********************************/
	OS_Printf( "***************\n�I�o���ꂽ�C�x���g\n" );
	for( j=0; j < wk->ev_tbl_max; j++ ){
		OS_Printf( "wk->ev_tbl[%d] = %d\n", j, wk->ev_tbl[j] );
	}
	OS_Printf( "***************\n\n" );
	/******************************************/

	//�F���Ƃ̖��������߂�
	for( i=0; i < wk->ev_tbl_max; i++ ){

		//��
		if( Roulette_GetEvPanelColor(wk->ev_tbl[i]) == ROULETTE_PANEL_COLOR_RED ){
			wk->panel_color_num[ROULETTE_PANEL_COLOR_RED]++;

		//��
		}else if( Roulette_GetEvPanelColor(wk->ev_tbl[i]) == ROULETTE_PANEL_COLOR_BLACK ){
			wk->panel_color_num[ROULETTE_PANEL_COLOR_BLACK]++;

		//��
		}else if( Roulette_GetEvPanelColor(wk->ev_tbl[i]) == ROULETTE_PANEL_COLOR_WHITE ){
			wk->panel_color_num[ROULETTE_PANEL_COLOR_WHITE]++;

		//���F
		}else if( Roulette_GetEvPanelColor(wk->ev_tbl[i]) == ROULETTE_PANEL_COLOR_YELLOW ){
			wk->panel_color_num[ROULETTE_PANEL_COLOR_YELLOW]++;
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�p�l���̌���3(�ʐM�Ȃ̂ŏ����𕪂��Ȃ��ƃ_���I)
 *
 * @param	
 *
 * @return	"�C�x���g�i���o�["
 */
//--------------------------------------------------------------
static void Roulette_EvPanelChoice3( ROULETTE_WORK* wk )
{
	int loop,j,offset,range;
	u16 rand,temp_rand;
	u8 num,color;

	//�p�l��16�Z�b�g����
	num = 0;
	for( j=0; j < ROULETTE_PANEL_MAX; j++ ){

		//�m���e�[�u��0-4��n��
		color = Roulette_GetDecideColor( wk, wk->point_rank );	//��0�A��1�A��2�A��3���擾

#if 1
		offset	= 0;
		range	= 0;

		//�����_�������߂镝���Z�b�g(�F���Ƃ̌������擾)
		range = wk->panel_color_num[color];

		//��
		if( color == ROULETTE_PANEL_COLOR_RED ){
			offset	= 0;

		//��
		}else if( color == ROULETTE_PANEL_COLOR_BLACK ){
			offset += wk->panel_color_num[ROULETTE_PANEL_COLOR_RED];

		//��
		}else if( color == ROULETTE_PANEL_COLOR_WHITE ){
			offset += wk->panel_color_num[ROULETTE_PANEL_COLOR_RED];
			offset += wk->panel_color_num[ROULETTE_PANEL_COLOR_BLACK];

		//���F
		}else if( color == ROULETTE_PANEL_COLOR_YELLOW ){
			offset += wk->panel_color_num[ROULETTE_PANEL_COLOR_RED];
			offset += wk->panel_color_num[ROULETTE_PANEL_COLOR_BLACK];
			offset += wk->panel_color_num[ROULETTE_PANEL_COLOR_WHITE];
		}

		rand = ( gf_rand() % range );

		//�o��OK�������e�F�̌��𑫂��āA���܂����F�̊J�n�I�t�Z�b�g�Ƃ���
		rand += offset;

		OS_Printf( "�C�x���g�T�[�`�J�n�ʒu = %d\n", rand );
#else
		rand = ( gf_rand() % wk->ev_tbl_max );	//�J�n�ʒu
		OS_Printf( "�C�x���g�T�[�`�J�n�ʒu = %d\n", rand );
#endif

		//1���`�F�b�N�p�ɃR�s�[
		num = rand;
		loop = 0;

		//�I�Ԃ��Ƃ̏o���郊�X�g����I�o
		while( 1 ){

			//�������[�v���
			if( loop >= 50 ){
				GF_ASSERT_MSG( 0, "�p�l�������܂炸���[�v���Ă��܂��I\n" );
				wk->panel_no[j] = ROULETTE_EV_ENEMY_HP_DOWN;
				loop = 0;
				break;
			}

			///////////////////////////////////////////////////////////////////////////////
			//�����́A�F�͈͓̔�����A�����_���őI�o�ɂ��Ă���̂ŁA�F�͕K�������ɂȂ�͂�
			//�Ȃ̂ŁA�����F�����`�F�b�N����K�v�͂Ȃ����A�ꉞ�B(X)
			//
			//�Ⴆ�΁A�����I�΂�邱�Ƃ����邪�A
			//��������I�o����Ă��Ȃ���������̂ŁA
			//�����F��������K�v�͂���I
			///////////////////////////////////////////////////////////////////////////////

			//�I�o���ꂽ���X�g����A�����F�̃C�x���g��T��
			if( Roulette_GetEvPanelColor(wk->ev_tbl[num]) == color ){
				OS_Printf( "�I�΂ꂽ�C�x���g = %d\n", wk->ev_tbl[num] );
				wk->panel_no[j] = wk->ev_tbl[num];
				loop = 0;
				break;
			}

			num++;
			loop++;
			if( num >= wk->ev_tbl_max ){
				num = 0;
			}

			//�J�n�ʒu�ɖ߂��Ă�����A���X�g����I�Ԃ��Ƃ��o���Ȃ������̂ŁA�F��ύX����I
			if( num == rand ){

#if 0
				//�Ⴄ�F���擾
				while( 1 ){
					temp_rand = ( gf_rand() % ROULETTE_PANEL_COLOR_MAX );
					if( temp_rand != color ){
						break;
					}
				}

				//���̃J���[���Ȃ��ƁA�܂��O�̃J���[�ɖ߂��ĂƂ������Ƃ����邩��
				//���Ȃ��A�ԂȂ��A���Ȃ��A�ԂȂ��A�A�A
#endif

				color++;
				if( color >= ROULETTE_PANEL_COLOR_MAX ){
					color = 0;
				}
				OS_Printf( "�J�n�ʒu�ɖ߂��Ă����̂ŃJ���[��ύX = %d\n", color );
			}
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�m������p�l���̐F�����߂�
 *
 * @param	point_rank(0-4)
 *
 * @return	"�ǂ̐F�̃C�x���g�ɂ��邩"
 */
//--------------------------------------------------------------
static u8 Roulette_GetDecideColor( ROULETTE_WORK* wk, u8 point_rank )
{
	u8 num,i;
	u16 rand;

	num = 0;
	rand = ( gf_rand() % 100 );
	OS_Printf( "�J���[�����肷��rand = %d\n", rand );

	//�ԁA���A���A����4��
	for( i=0; i < ROULETTE_PANEL_COLOR_MAX ;i++ ){
		num+=roulette_category_tbl[point_rank][i];
		if( rand < num ){
			break;
		}
	}

	//�G���[���
	if( i >= ROULETTE_PANEL_COLOR_MAX ){
		OS_Printf( "�J���[�擾�����������I\n" );
		GF_ASSERT( 0 );
		i = 0;
	}

	OS_Printf( "color = %d\n", i );
	return i;				//�ǂ̐F�̃C�x���g��
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e�������Ă��邩�A�C�R���\���A��\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Roulette_ItemKeepVanishSet( ROULETTE_WORK* wk )
{
	u8 m_max,e_max,i;
	POKEMON_PARAM* poke;

	m_max = Roulette_GetMinePokeNum( wk->type, ROULETTE_FLAG_TOTAL );
	e_max = Roulette_GetEnemyPokeNum( wk->type, ROULETTE_FLAG_TOTAL );

	//����
	for( i=0; i < m_max ;i++ ){	
		poke =  PokeParty_GetMemberPointer( wk->p_m_party, i );
		if( PokeParaGet(poke,ID_PARA_item,NULL) == 0 ){
			RouletteObj_Vanish( wk->p_m_itemkeep[i], ROULETTE_VANISH_ON );			//��\��
		}else{
			RouletteObj_Vanish( wk->p_m_itemkeep[i], ROULETTE_VANISH_OFF );			//�\��
		}
	}

	//�G
	for( i=0; i < e_max ;i++ ){	
		poke =  PokeParty_GetMemberPointer( wk->p_e_party, i );
		//OS_Printf( "enemy item = %d\n", PokeParaGet(poke,ID_PARA_item,NULL) );
		if( PokeParaGet(poke,ID_PARA_item,NULL) == 0 ){
			RouletteObj_Vanish( wk->p_e_itemkeep[i], ROULETTE_VANISH_ON );			//��\��
		}else{
			RouletteObj_Vanish( wk->p_e_itemkeep[i], ROULETTE_VANISH_OFF );			//�\��
		}
	}

	return;
}


//==============================================================================================
//
//	�^�b�`�p�l���֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�{�^��������������
 *
 * @param	wk		ROULETTE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = �{�^���������AFALSE = �{�^�������Ă��Ȃ�"
 */
//--------------------------------------------------------------
static BOOL Roulette_CheckButtonPush( ROULETTE_WORK* wk )
{
	int	ret = GF_TP_HitTrg( ButtonHitTbl );

	if( ret == 0 ){
	//if( (ret == 0) || (sys.trg & PAD_BUTTON_A) ){
		//Snd_SePlay( SEQ_SE_DP_BUTTON9 );
		OS_Printf( "�{�^���������I\n" );
		Roulette_SetButtonPush( wk );
		return TRUE;
	}

	return FALSE;
}

static void Roulette_SetButtonPush( ROULETTE_WORK* wk )
{
	Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
	Snd_SePlay( SEQ_SE_DP_BUTTON9 );
	RouletteObj_AnmChg( wk->p_button, ROULETTE_ANM_BUTTON_PUSH );
	return;
}


