//==============================================================================================
/**
 * @file	factory.c
 * @brief	�u�o�g���t�@�N�g���[�v���C���\�[�X
 * @author	Satoshi Nohara
 * @date	2007.03.14
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
#include "system/softsprite.h"
#include "gflib/strbuf_family.h"
#include "savedata/config.h"
#include "application/p_status.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_number.h"
#include "savedata/b_tower.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"
#include "communication/comm_info.h"
#include "communication/wm_icon.h"

#include "factory_sys.h"
#include "factory_clact.h"
#include "factory_bmp.h"
#include "factory_obj.h"
#include "factory_ball.h"
#include "factory_csr.h"
#include "factory_3d.h"
#include "application/factory.h"
#include "../frontier_tool.h"					//Frontier_PokeParaMake
#include "../factory_tool.h"					//FACTORY_POKE_RANGE
#include "../factory_def.h"
#include "../comm_command_frontier.h"

#include "../../field/comm_direct_counter_def.h"//

#include "msgdata\msg.naix"						//NARC_msg_??_dat
#include "msgdata\msg_factory_rental.h"			//msg_??

#include "../graphic/frontier_obj_def.h"
#include "../graphic/frontier_bg_def.h"


#include "system/pm_overlay.h"
FS_EXTERN_OVERLAY(frontier_common);


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�V�[�P���X��`
enum {
	SEQ_GAME_START_EFF,										//�Q�[���J�n���o

	SEQ_GAME_INIT,											//������
	SEQ_GAME_PSD,											//�|�P�����X�e�[�^�X����

	SEQ_GAME_RENTAL,										//�����^���|�P�����I��
	SEQ_GAME_FINAL_ANSWER,									//�ȏ��3�C�ł�낵���H
	SEQ_GAME_FINAL_ANSWER_CANCEL,							//�ȏ��3�C�ł�낵���H(�L�����Z��)

	SEQ_GAME_TRADE_TEBANASU,								//�����|�P�����I��(�Ă΂Ȃ�)
	SEQ_GAME_TRADE_TEBANASU_TYUUSI,							//�����|�P�����I��(�Ă΂Ȃ������イ��)
	SEQ_GAME_TRADE_UKETORU,									//�����|�P�����I��(�����Ƃ�)
	SEQ_GAME_TRADE_UKETORU_TYUUSI,							//�����|�P�����I��(�����Ƃ遨���イ��)

	SEQ_GAME_LEAVING_ENTRANCE,								//����A�ޏ�

	SEQ_GAME_SEND_RECV,

	SEQ_GAME_TRADE_END_MULTI,								//�ʐM�����I��
	SEQ_GAME_END,											//�I��

	SEQ_GAME_PAIR_TRADE_END,								//�p�[�g�i�[�����������̂ŋ����I��
};

//�|�P�����\��
#define TEST_X			(120)
#define TEST_Y			(43)//(48)
#define TEST_Z			(0)
#define TEST_POLY_ID	(0)

//3�C�����낤��ʂ̕\���ʒu
#define TEST_X1			(44)//(48)
#define TEST_X2			(128)//(48)
#define TEST_X3			(212)//(208)

#define FACTORY_SOFT_SPRITE_MAX		(3)						//�\�t�g�X�v���C�g�̐�

//H�t���b�v
#define FLIP_RENTAL			(0)
#define FLIP_TEBANASU		(1)
#define FLIP_UKETORU		(0)

//�t�H���g�J���[
#define	COL_BLUE	( GF_PRINTCOLOR_MAKE(FBMP_COL_BLUE,FBMP_COL_BLU_SDW,FBMP_COL_NULL) )//��
#define	COL_RED		( GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_NULL) )	//��

//�p�[�g�i�[�������������Ƃ�m�点�郁�b�Z�[�W�E�F�C�g
#define FACTORY_PAIR_TRADE_END_WAIT	(80)

//����̃|�P��������`�惊�N�G�X�g
enum{
	REQ_PAIR_MONSNO_NONE = 0,
	REQ_PAIR_MONSNO_DECIDE,
};

#define FACTORY_MARU_WAIT				(4)					//�p���b�g�؂�ւ��E�F�C�g
#define FACTORY_MARU_BASE_PAL			(6)					//��{�p���b�g�i���o�[
#define FACTORY_MARU_PAL_NUM			(4)					//�p���b�g��

//�w�i�̃E�B���h�E�̃p���b�g����ID
enum{
	ID_PAL_SINGLE_OPEN = 0,									//�V���O���A�|�P3�C
	ID_PAL_SINGLE_FLASH,
	ID_PAL_SINGLE_NORMAL,
};

//�w�i�̃E�B���h�E�̕ύX����p���b�gNO
#define FACTORY_SINGLE_WIN_OPEN_PAL		(2)					//�J���Ă��鎞(�V���O���A�|�P3�C)
#define FACTORY_SINGLE_WIN_FLASH_PAL	(1)					//���鎞
#define FACTORY_SINGLE_WIN_NORMAL_PAL	(2)					//�ʏ�

//����̃|�P�����\���E�B���h�E�̕\���ʒu
enum{
	FACTORY_OBJ_SINGLE_WIN_X = 80,
	FACTORY_OBJ_SINGLE_WIN_Y = 40,
	FACTORY_OBJ_MULTI_WIN_X = 216,
	FACTORY_OBJ_MULTI_WIN_Y = 40,
	FACTORY_OBJ_3POKE_WIN_X = 128,//80,
	FACTORY_OBJ_3POKE_WIN_Y = 40,
	FACTORY_OBJ_2POKE_WIN_X_OPEN = 80,//64,
	FACTORY_OBJ_2POKE_WIN_X_CLOSE = 80,//64,
	FACTORY_OBJ_2POKE_WIN_Y = 40,
	FACTORY_OBJ_MULTI_2POKE_WIN_X = 128,//80,
	FACTORY_OBJ_MULTI_2POKE_WIN_Y = 40,
};

//�A�j���E�F�C�g���ύX���ꂽ��Ή����Ȃ��ƃ_���I
#define POKE_APPEAR_WAIT				(21)				//�|�P�����\������^�C�~���O
#define POKE_FLASH_WAIT					(2)					//�|�P��������E�F�C�g

//���[���̑��x
#define RAIL_SPD	(8)
#define FACTORY_SCROLL_OFFSET_X			(33*RAIL_SPD)		//���[�v��


//==============================================================================================
//
//	���b�Z�[�W�֘A�̒�`
//
//==============================================================================================
#define FACTORY_MENU_BUF_MAX		(4)						//���j���[�o�b�t�@�̍ő吔
#define BF_FONT						(FONT_SYSTEM)			//�t�H���g���
#define FACTORY_MSG_BUF_SIZE		(800)//(1024)			//��b�̃��b�Z�[�Wsize
#define FACTORY_MENU_BUF_SIZE		(64)					//���j���[�̃��b�Z�[�Wsize
#define PLAYER_NAME_BUF_SIZE		(PERSON_NAME_SIZE + EOM_SIZE)	//�v���C���[���̃��b�Z�[�Wsize
#define POKE_NAME_BUF_SIZE			(MONS_NAME_SIZE + EOM_SIZE)		//�|�P�������̃��b�Z�[�Wsize

//�z�u�f�[�^�Ȃ�
#include "factory.dat"


//==============================================================================================
//
//	(�t�@�N�g���[)�ʏ�̃X�e�[�^�X��ʃy�[�W
//
//==============================================================================================
static const u8 PST_PageTbl_Normal[] = {
	PST_PAGE_INFO,			// �u�|�P�������傤�ق��v
	PST_PAGE_MEMO,			// �u�g���[�i�[�����v
	PST_PAGE_PARAM,			// �u�|�P�����̂���傭�v
	//PST_PAGE_CONDITION,		// �u�R���f�B�V�����v
	PST_PAGE_B_SKILL,		// �u���������킴�v
	//PST_PAGE_C_SKILL,		// �u�R���e�X�g�킴�v
	//PST_PAGE_RIBBON,		// �u���˂񃊃{���v
	PST_PAGE_RET,			// �u���ǂ�v
	PST_PAGE_MAX
};


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
struct _FACTORY_WORK{
	PROC* proc;										//PROC�ւ̃|�C���^
	PROC* child_proc;								//PROC�ւ̃|�C���^

	u8	sub_seq;									//�V�[�P���X
	u8	type;										//�����Ƃ��ēn���ꂽ�o�g���^�C�v
	u8	level;										//LV50�A�I�[�v��
	u8	mode;										//�����Ƃ��ēn���ꂽ��ʃ��[�h

	int scr_x;										//�X�N���[��X

	u8	msg_index;									//���b�Z�[�Windex
	u8	sel_count;									//�I�����Ă��鐔
	u8	ball_max;									//�\������{�[���̍ő吔���i�[
	u8	cancel:1;									//�L�����Z�������t���O
	u8	recover:1;									//���A�t���O
	u8	tebanasu_uketoru:1;							//�����=0,�󂯎��=1
	u8	trade_flag:1;								//�����������t���O
	u8	start_eff_flag:1;							//�J�n���o���I��������
	u8	pair_data_off_flag:1;						//�y�A�̃f�[�^��\�����Ȃ��t���O
	u8	psd_flag:1;									//�|�P�����X�e�[�^�X���t���O
	u8	dummy67:1;									//

	u32	maru_wait;									//�~�̃p���b�g�؂�ւ��E�F�C�g

	u8	pair_sel_count;								//
	u8	wait_count;									//
	u8	tebanasu_max;								//
	u8	uketoru_max;								//

	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g
	STRBUF* msg_buf;								//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;								//�e���|�����o�b�t�@�|�C���^

	STRBUF* menu_buf[FACTORY_MENU_BUF_MAX];			//���j���[�o�b�t�@�|�C���^
	STRCODE str[PERSON_NAME_SIZE + EOM_SIZE];		//���j���[�̃��b�Z�[�W

	GF_BGL_INI*	bgl;								//BGL�ւ̃|�C���^
	GF_BGL_BMPWIN bmpwin[FACTORY_BMPWIN_MAX];		//BMP�E�B���h�E�f�[�^

	//BMP���j���[(bmp_menu.h)
	BMPMENU_HEADER MenuH;							//BMP���j���[�w�b�_�[
	BMPMENU_WORK* mw;								//BMP���j���[���[�N
	BMPMENU_DATA Data[FACTORY_MENU_BUF_MAX];		//BMP���j���[�f�[�^

	PALETTE_FADE_PTR pfd;							//�p���b�g�t�F�[�h

	GF_G3DMAN* g3Dman;
	SOFT_SPRITE_MANAGER	*soft_sprite;				//�\���p�}�l�[�W���[�ւ̃|�C���^
	SOFT_SPRITE *ss[FACTORY_SOFT_SPRITE_MAX];		//�|�P�����\���p

	//const CONFIG* config;							//�R���t�B�O�|�C���^
	CONFIG* config;									//�R���t�B�O�|�C���^
	SAVEDATA* sv;									//�Z�[�u�f�[�^�|�C���^

	PSTATUS_DATA* psd;								//�|�P�����X�e�[�^�X

	FACTORY_CLACT factory_clact;					//�Z���A�N�^�f�[�^
	FACTORY_BALL* p_ball[BF_ENTRY_POKE_MAX];		//�{�[��OBJ�̃|�C���^�i�[�e�[�u��
	FACTORY_CSR* p_csr;								//�J�[�\��OBJ�̃|�C���^�i�[�e�[�u��
	FACTORY_CSR* p_csr_menu;						//���j���[�J�[�\��OBJ�̃|�C���^�i�[�e�[�u��
	FACTORY_OBJ* p_obj_win;							//�E�B���h�EOBJ�̃|�C���^�i�[�e�[�u��
	FACTORY_OBJ* p_pair_obj_win;					//�y�A�̃E�B���h�EOBJ�̃|�C���^�i�[�e�[�u��

	POKEPARTY* p_m_party;							//�n���ꂽ�����|�P�����f�[�^(6�C)
	POKEPARTY* p_e_party;							//�n���ꂽ�G�|�P�����f�[�^(6�C)
	u16 sel_work[ FACTORY_SEL_WORK_MAX ];			//�I�������{�[���ʒu(0-5)�L�����Z��(0xff)
	u16* p_ret_work;								//FACTORY_CALL_WORK�̖߂�l���[�N�ւ̃|�C���^

	int psd_pos;									//�|�P�����X�e�[�^�X�̑I�����ʎ擾

	ARCHANDLE* hdl;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//FACTORY_COMM factory_comm;
#if 1
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[FACTORY_COMM_BUF_LEN];

	//�ʐM�p
	u16 pair_trade_flag;							//����������

	//�ʐM�p�F�����^���|�P�����f�[�^
	u16	pair_poke_monsno[FACTORY_COMM_POKE_NUM];	//�|�P�����i���o�[
	u16 pair_poke_sex[FACTORY_COMM_POKE_NUM];		//�|�P��������
#endif
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	
	u8	trade_recieve_count;
	u8	pair_monsno_req;

	u32 dummy_work;
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//PROC
PROC_RESULT FactoryProc_Init( PROC * proc, int * seq );
PROC_RESULT FactoryProc_Main( PROC * proc, int * seq );
PROC_RESULT FactoryProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameStartEff( FACTORY_WORK* wk );
static BOOL Seq_GameInit( FACTORY_WORK* wk );
static BOOL Seq_GamePsd( FACTORY_WORK* wk );
static void RentalInit( FACTORY_WORK* wk );
static void RentalBallInit( FACTORY_WORK* wk );
static void RentalPokeInit( FACTORY_WORK* wk );
static void TradeInit( FACTORY_WORK* wk );
static void TradeBallInit( FACTORY_WORK* wk );

static BOOL Seq_GameRental( FACTORY_WORK* wk );
static void SetBCancel( FACTORY_WORK* wk );
static void SetRentarusuru( FACTORY_WORK* wk );
static void SetHazusu( FACTORY_WORK* wk );

static BOOL Seq_GameFinalAnswer( FACTORY_WORK* wk );
static BOOL Seq_GameFinalAnswerCancel( FACTORY_WORK* wk );
static void SetFinalAnswerNo( FACTORY_WORK* wk );

static BOOL Seq_GameTradeTebanasu( FACTORY_WORK* wk );
static void SetTebanasu( FACTORY_WORK* wk );

static BOOL Seq_GameTradeTebanasuTyuusi( FACTORY_WORK* wk );
static void SetTebanasuTyuusiNo( FACTORY_WORK* wk );

static BOOL Seq_GameTradeUketoru( FACTORY_WORK* wk );
static void SetModoru( FACTORY_WORK* wk );
static void SetUketoru( FACTORY_WORK* wk );
static void SetUketoruNo( FACTORY_WORK* wk );

static BOOL Seq_GameTradeUketoruTyuusi( FACTORY_WORK* wk );
static void SetUketoruTyuusiNo( FACTORY_WORK* wk );

static BOOL Seq_GameLeavingEntrance( FACTORY_WORK* wk );

static BOOL Seq_GameSendRecv( FACTORY_WORK* wk );

static BOOL Seq_GameTradeEndMulti( FACTORY_WORK* wk );
static BOOL Seq_GameEnd( FACTORY_WORK* wk );

static BOOL Seq_GamePairTradeEnd( FACTORY_WORK* wk );

static void FactoryCommon_Delete( FACTORY_WORK* wk );
static void Factory_Recover( FACTORY_WORK* wk );
static void Factory_InitSub1( void );
static void Factory_InitSub2( FACTORY_WORK* wk );
static void FactoryCommon_ChangePoke( FACTORY_WORK* wk, u8 index, u8 poke_pos, int flip, const POKEPARTY* party );

//���ʏ������A�I��
static void Factory_ObjInit( FACTORY_WORK* wk );
static void Factory_BgInit( FACTORY_WORK* wk );
static void Factory_BgExit( GF_BGL_INI * ini );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//�c�[��
static void NextSeq( FACTORY_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( FACTORY_WORK* wk );
static BOOL RailMoveIn( FACTORY_WORK* wk );
static BOOL RailMoveOut( FACTORY_WORK* wk );
static void MaruMove( FACTORY_WORK* wk );
static void ScrPalChg( FACTORY_WORK* wk, u32 frm, u8 pltt_no, u8 sx, u8 sy );
static void Factory_ObjWinSet( FACTORY_WORK* wk, u32 anm_no );

//BG�O���t�B�b�N�f�[�^
static void Factory_SetMainBgGraphic( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgGraphic2( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgGraphic3( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgGraphic_Rail( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgGraphic_Back( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgGraphic_Maru( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgPalette( void );
static void Factory_SetSubBgGraphic( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetMainBgScrn( FACTORY_WORK * wk, u32 frm  );
static void Factory_SetRentalBackBgScrn( FACTORY_WORK * wk, u32 frm );
static void Factory_Set3PokeBgScrn( FACTORY_WORK * wk, u32 frm );

//���b�Z�[�W
static u8 FactoryWriteMsg( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 FactoryWriteMsgSimple( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 Factory_EasyMsg( FACTORY_WORK* wk, int msg_id );

//���j���[
static void FactoryInitMenu( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max );
static void FactorySetMenuData( FACTORY_WORK* wk, u8 no, u8 param, int msg_id );
static void Factory_SetMenu( FACTORY_WORK* wk );
static void Factory_SetMenu2( FACTORY_WORK* wk );
static void Factory_SetMenu3( FACTORY_WORK* wk );
static void Factory_SetMenu4( FACTORY_WORK* wk );

//������
static void Factory_SetNumber( FACTORY_WORK* wk, u32 bufID, s32 number );
static void Factory_SetPokeName( FACTORY_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp );
static void Factory_SetPlayerName( FACTORY_WORK* wk, u32 bufID );
static void PlayerNameWrite( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PairNameWrite( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PokeNameWrite( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u8 csr_pos, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, const POKEPARTY* party );
static void PokeNameWriteEx( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex );

//�|�P�����X�e�[�^�X
static void Factory_SetPSD( FACTORY_WORK* wk );

//�c�[��
static BOOL Factory_CheckType( FACTORY_WORK* wk, u8 type );
static BOOL Factory_CheckMode( FACTORY_WORK* wk, u8 mode );
static u8 Factory_GetRentalSelPokeNum( u8 type );
static void Factory_SelWorkClear( FACTORY_WORK* wk );
static BOOL Factory_CheckCancel( FACTORY_WORK* wk );
static void Factory_SetCancel( FACTORY_WORK* wk, u8 flag );
static u8 Factory_GetScrPalNo( FACTORY_WORK* wk, u8 id, u8 final_answer_flag  );
static u32 Factory_GetWinAnmNo( FACTORY_WORK* wk, u32 anm_no );

//�|�P�����\��
static void Factory_PokeAdd(FACTORY_WORK* wk, u8 index, POKEMON_PARAM* poke, int x,int y, int flip);
static BOOL PokeFlipCheck(u32 monsno);

//�ʐM
BOOL Factory_CommSetSendBuf( FACTORY_WORK* wk, u16 type, u16 param );
void Factory_CommSendBufBasicData( FACTORY_WORK* wk, u16 type );
void Factory_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
void Factory_CommSendBufRentalData( FACTORY_WORK* wk, u16 type, u16 param );
void Factory_CommRecvBufRentalData(int id_no,int size,void *pData,void *work);
void Factory_CommRecvBufCancelData(int id_no,int size,void *pData,void *work);
void Factory_CommSendBufTradeFlag( FACTORY_WORK* wk, u16 type, u16 sel );
void Factory_CommRecvBufTradeFlag(int id_no,int size,void *pData,void *work);
static void Factory_PairMonsNoRecvReq( FACTORY_WORK* wk );

//���ʏ���
static FACTORY_BALL* Factory_BallAdd( FACTORY_WORK* wk, int no );
static FACTORY_OBJ* Factory_ObjWinAdd( FACTORY_WORK* wk, u32 anm_no );
static FACTORY_OBJ* Factory_PairObjWinAdd( FACTORY_WORK* wk, u32 anm_no );
static void Factory_ObjWinDel( FACTORY_WORK* wk );
static void Factory_FrameWinBmpSet( FACTORY_WORK* wk );
static void BmpMenuWinClearSub( GF_BGL_BMPWIN* win );


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
PROC_RESULT FactoryProc_Init( PROC * proc, int * seq )
{
	int i;
	FACTORY_WORK* wk;
	FACTORY_CALL_WORK* factory_call;

	Overlay_Load( FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE );

	Factory_InitSub1();

	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_FACTORY, 0x20000 );

	wk = PROC_AllocWork( proc, sizeof(FACTORY_WORK), HEAPID_FACTORY );
	memset( wk, 0, sizeof(FACTORY_WORK) );

	//3D�V�X�e���ݒ�
	wk->g3Dman = GF_G3DMAN_Init( HEAPID_FACTORY, GF_G3DMAN_LNK, GF_G3DTEX_256K, 
									GF_G3DMAN_LNK, GF_G3DPLT_32K, Factory_SimpleSetUp );

	wk->bgl				= GF_BGL_BglIniAlloc( HEAPID_FACTORY );
	wk->proc			= proc;
	factory_call		= (FACTORY_CALL_WORK*)PROC_GetParentWork( proc );
	wk->sv				= factory_call->sv;
	wk->type			= factory_call->type;
	wk->level			= factory_call->level;
	wk->mode			= factory_call->mode;
	wk->p_m_party		= factory_call->p_m_party;
	wk->p_e_party		= factory_call->p_e_party;
	wk->p_ret_work		= &factory_call->ret_work[0];
	wk->config			= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾

	//�l����ԏ������p���b�g��6�ŁA�����p���b�g��8�ɂȂ��Ă���
	wk->maru_wait		= ( FACTORY_MARU_WAIT * 2 );

	//�����^�����[�h
	if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){
		wk->ball_max = BF_ENTRY_POKE_MAX;

	//�������[�h(�{�[���̐��Ɣz�u�ꏊ�Ȃǂ��Ⴄ)
	}else{

		//�ʐM�^�C�v�̎�
		if( Factory_CommCheck(wk->type) == TRUE ){
			wk->ball_max = BF_T_MULTI_BALL_TEBANASU_POS_MAX;
			wk->tebanasu_max = BF_HV_TYUUSI_MULTI_MAX;
			wk->uketoru_max = BF_HV_MODORU_MULTI_MAX;
		}else{
			wk->ball_max = BF_T_BALL_POS_MAX;
			wk->tebanasu_max = BF_HV_TYUUSI_MAX;
			wk->uketoru_max = BF_HV_MODORU_MAX;
		}
	}

	Factory_InitSub2( wk );

	if( Factory_CommCheck(wk->type) == TRUE ){
		CommCommandFrontierInitialize( wk );
	}

	(*seq) = SEQ_GAME_START_EFF;
	OS_Printf( "(*seq) = %d\n", (*seq) );

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
PROC_RESULT FactoryProc_Main( PROC * proc, int * seq )
{
	FACTORY_WORK* wk  = PROC_GetWork( proc );
	//OS_Printf( "(*seq) = %d\n", (*seq) );

	//�y�A������������
	if( wk->pair_trade_flag == 1 ){

		switch( *seq ){
		//���̃V�[�P���X���ł͋����I��������I
		case SEQ_GAME_TRADE_TEBANASU:					//�����|�P�����I��(�Ă΂Ȃ�)
		case SEQ_GAME_TRADE_TEBANASU_TYUUSI:			//�����|�P�����I��(�Ă΂Ȃ������イ��)
		case SEQ_GAME_TRADE_UKETORU:					//�����|�P�����I��(�����Ƃ�)
		case SEQ_GAME_TRADE_UKETORU_TYUUSI:				//�����|�P�����I��(�����Ƃ遨���イ��)

			//�X�e�[�^�X�ɐ؂�ւ��鎞�ȊO
			if( wk->psd_flag == 0 ){
				NextSeq( wk, seq, SEQ_GAME_SEND_RECV );				//
			}
			break;

		//���̃V�[�P���X���ł͋����I�������Ȃ��I
		//case SEQ_GAME_START_EFF:						//�J�n���o
		//case SEQ_GAME_INIT:							//������
		//case SEQ_GAME_PSD:							//�|�P�����X�e�[�^�X����
		//case SEQ_GAME_LEAVING_ENTRANCE:				//����A�ޏ�
			//break;
		//���ɉ������Ȃ��I
		//case SEQ_GAME_TRADE_END_MULTI:				//�ʐM�����I��
		//case SEQ_GAME_END:							//�I��
		};
	}

	//]]]]]]]]]]]]]
	
	switch( *seq ){

	//-----------------------------------
	//�J�n���o
	case SEQ_GAME_START_EFF:
		if( Seq_GameStartEff(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_INIT );
		}
		break;

#if 1
	//-----------------------------------
	//�|�P�����X�e�[�^�X����
	case SEQ_GAME_PSD:
		if( Seq_GamePsd(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_INIT );
		}else{
			return PROC_RES_CONTINUE;	//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		}
		//break;	//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#endif

	//-----------------------------------
	//������
	case SEQ_GAME_INIT:
		if( Seq_GameInit(wk) == TRUE ){
			if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){	//�����^��
				NextSeq( wk, seq, SEQ_GAME_RENTAL );
			}else{														//����
				NextSeq( wk, seq, SEQ_GAME_TRADE_TEBANASU );
			}
		}
		break;

	//-----------------------------------
	//�����^���|�P�����I��
	case SEQ_GAME_RENTAL:
		//�p�[�g�i�[�̃|�P������
		Factory_PairMonsNoRecvReq( wk );

		if( Seq_GameRental(wk) == TRUE ){
			if( wk->recover == 1 ){
				NextSeq( wk, seq, SEQ_GAME_PSD );						//�|�P�����X�e�[�^�X
			}else{
				if( wk->sel_count == Factory_GetRentalSelPokeNum(wk->type) ){
					NextSeq( wk, seq, SEQ_GAME_FINAL_ANSWER );			//3�C�I�������̂ŏI��
				}else{
					//B�L�����Z����������
					if( Factory_CheckCancel(wk) == TRUE ){
						//�����̓L�����Z���t���O���̂܂�
						NextSeq( wk, seq, SEQ_GAME_END );				//�I����
					}else{
						NextSeq( wk, seq, SEQ_GAME_RENTAL );			//3�C�I������܂ŌJ��Ԃ�
					}
				}
			}
		}
		break;

	//-----------------------------------
	//�ȏ��3�C�ł�낵���ł����H
	case SEQ_GAME_FINAL_ANSWER:

		//BgCheck( wk );

		//�p�[�g�i�[�̃|�P������
		Factory_PairMonsNoRecvReq( wk );

		if( Seq_GameFinalAnswer(wk) == TRUE ){
			if( wk->sel_count == Factory_GetRentalSelPokeNum(wk->type) ){
				NextSeq( wk, seq, SEQ_GAME_END );						//�I����
			}else{
				NextSeq( wk, seq, SEQ_GAME_FINAL_ANSWER_CANCEL );		//�L�����Z���������ēx�I����
			}
		}
		break;

	//-----------------------------------
	//�ȏ��3�C�ł�낵���ł����H(�L�����Z��)
	case SEQ_GAME_FINAL_ANSWER_CANCEL:
		if( Seq_GameFinalAnswerCancel(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_RENTAL );						//�ēx�I����
		}
		break;

	//-----------------------------------
	//������|�P�����I��
	case SEQ_GAME_TRADE_TEBANASU:
		if( Seq_GameTradeTebanasu(wk) == TRUE ){
			if( wk->recover == 1 ){
				NextSeq( wk, seq, SEQ_GAME_PSD );						//�|�P�����X�e�[�^�X
			}else{
				//B�L�����Z����������
				if( Factory_CheckCancel(wk) == TRUE ){
					Factory_SetCancel( wk, 0 );
					NextSeq( wk, seq, SEQ_GAME_TRADE_TEBANASU_TYUUSI );	//���~����H
				}else{
					NextSeq( wk, seq, SEQ_GAME_LEAVING_ENTRANCE );		//����A�ޏ�
				}
			}
		}
		break;

	//-----------------------------------
	//������|�P�������~����H
	case SEQ_GAME_TRADE_TEBANASU_TYUUSI:
		if( Seq_GameTradeTebanasuTyuusi(wk) == TRUE ){

			//B�L�����Z����������
			if( Factory_CheckCancel(wk) == TRUE ){
				Factory_SetCancel( wk, 0 );
				NextSeq( wk, seq, SEQ_GAME_TRADE_TEBANASU );			//������|�P������I��
			}else{
				//�����̓L�����Z���t���O���̂܂�

				//�ʐM�^�C�v�̎�
				if( Factory_CommCheck(wk->type) == TRUE ){
					wk->trade_flag = 0;									//���������t���OOFF
					NextSeq( wk, seq, SEQ_GAME_SEND_RECV );				//
				}else{
					NextSeq( wk, seq, SEQ_GAME_END );					//�I����
				}
			}
		}
		break;

	//-----------------------------------
	//�󂯎��|�P�����I��
	case SEQ_GAME_TRADE_UKETORU:
		if( Seq_GameTradeUketoru(wk) == TRUE ){

			//B�L�����Z����������
			if( Factory_CheckCancel(wk) == TRUE ){
				Factory_SetCancel( wk, 0 );
				NextSeq( wk, seq, SEQ_GAME_TRADE_UKETORU_TYUUSI );		//���~����H
			}else{
				if( wk->sel_count == 0 ){
					NextSeq( wk, seq, SEQ_GAME_LEAVING_ENTRANCE );		//����A�ޏ�
				}else{
					//�ʐM�^�C�v�̎�
					if( Factory_CommCheck(wk->type) == TRUE ){
						NextSeq( wk, seq, SEQ_GAME_SEND_RECV );			//
					}else{
						NextSeq( wk, seq, SEQ_GAME_END );				//�I����
					}
				}
			}
		}
		break;

	//-----------------------------------
	//�󂯎��|�P�������~����H
	case SEQ_GAME_TRADE_UKETORU_TYUUSI:
		if( Seq_GameTradeUketoruTyuusi(wk) == TRUE ){

			//B�L�����Z����������
			if( Factory_CheckCancel(wk) == TRUE ){
				Factory_SetCancel( wk, 0 );
				NextSeq( wk, seq, SEQ_GAME_TRADE_UKETORU );				//�󂯎��|�P������I��
			}else{

				//�ʐM�^�C�v�̎�
				if( Factory_CommCheck(wk->type) == TRUE ){
					wk->trade_flag = 0;									//���������t���OOFF
					NextSeq( wk, seq, SEQ_GAME_SEND_RECV );				//
				}else{
					NextSeq( wk, seq, SEQ_GAME_END );					//�I����
				}
			}
		}
		break;

	//-----------------------------------
	//����A�ޏ�
	case SEQ_GAME_LEAVING_ENTRANCE:
		if( Seq_GameLeavingEntrance(wk) == TRUE ){
			if( wk->sel_count == 0 ){
				TradeInit( wk );
				NextSeq( wk, seq, SEQ_GAME_TRADE_TEBANASU );			//������|�P������I��
			}else{
				NextSeq( wk, seq, SEQ_GAME_TRADE_UKETORU );				//�󂯎��|�P������I��
			}
		}
		break;

	//-----------------------------------
	//����M
	case SEQ_GAME_SEND_RECV:
		if( Seq_GameSendRecv(wk) == TRUE ){
			//����̌����̕����L�����`�F�b�N
			if( wk->pair_trade_flag == 1 ){
				NextSeq( wk, seq, SEQ_GAME_PAIR_TRADE_END );
			}else{
				NextSeq( wk, seq, SEQ_GAME_TRADE_END_MULTI );			//�ʐM�����I����
			}
		}
		break;

	//-----------------------------------
	//�ʐM�����I��
	case SEQ_GAME_TRADE_END_MULTI:
		if( Seq_GameTradeEndMulti(wk) == TRUE ){
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

	//-----------------------------------
	//�p�[�g�i�[�����������̂ŋ����I��
	case SEQ_GAME_PAIR_TRADE_END:
		if( Seq_GamePairTradeEnd(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_TRADE_END_MULTI );				//�ʐM�����I����
		}
		break;

	}

	CLACT_Draw( wk->factory_clact.ClactSet );		//�Z���A�N�^�[�풓�֐�
	Factory_3DMain( wk->soft_sprite );				//3D���C������

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
PROC_RESULT FactoryProc_End( PROC * proc, int * seq )
{
	int i;
	FACTORY_WORK* wk = PROC_GetWork( proc );

	//�����^�����[�h
	if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){
		for( i=0; i < Factory_GetRentalSelPokeNum(wk->type); i++ ){
			wk->p_ret_work[i] = wk->sel_work[i];		//�߂�l�i�[���[�N�֑��
		}

	//�������[�h(�{�[���̐��Ɣz�u�ꏊ���Ⴄ)
	}else{
		for( i=0; i < 2; i++ ){							//[0]�莝���̂ǂ�ƁA[1]�G�̂ǂ������
			wk->p_ret_work[i] = wk->sel_work[i];		//�߂�l�i�[���[�N�֑��
			//OS_Printf( "wk->sel_work[0] = %d\n", wk->sel_work[0] );
			//OS_Printf( "wk->sel_work[1] = %d\n", wk->sel_work[1] );
		}

		//�������Ă��Ȃ���
		if( wk->trade_flag == 0 ){
			wk->p_ret_work[0] = FACTORY_RET_CANCEL;		//�߂�l�i�[���[�N�֑��
			wk->p_ret_work[1] = FACTORY_RET_CANCEL;		//�߂�l�i�[���[�N�֑��
		}
	}


	OS_Printf( "wk->p_ret_work[0] = %d\n", wk->p_ret_work[0] );
	OS_Printf( "wk->p_ret_work[1] = %d\n", wk->p_ret_work[1] );
	OS_Printf( "wk->p_ret_work[2] = %d\n", wk->p_ret_work[2] );

	FactoryCommon_Delete( wk );							//�폜����

	PROC_FreeWork( proc );								//���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_DeleteHeap( HEAPID_FACTORY );

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
 * @brief	�V�[�P���X�F�Q�[���J�n���o
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameStartEff( FACTORY_WORK* wk )
{
	int i;
	const VecFx32* p_vec;

	enum{
		SEQ_START_EFF_TIMING = 0,
		SEQ_START_EFF_TIMING_SYNC,
		SEQ_START_EFF_START,
		SEQ_START_EFF_WIPE_END_CHECK,
		SEQ_START_EFF_RAIL_MOVE,
		SEQ_START_EFF_POKE_APPEAR_TIMING,
		SEQ_START_EFF_POKE_FADEIN,
	};

	switch( wk->sub_seq ){

	//�R�}���h�ݒ��̓���
	case SEQ_START_EFF_TIMING:
		if( Factory_CommCheck(wk->type) == TRUE ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_FACTORY_START_INIT );
		}
		wk->sub_seq++;
		break;

	//�^�C�~���O�R�}���h���͂������m�F
	case SEQ_START_EFF_TIMING_SYNC:
		if( Factory_CommCheck(wk->type) == TRUE ){
			if( CommIsTimingSync(DBC_TIM_FACTORY_START_INIT) == TRUE ){
				CommToolTempDataReset();
				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;

	//�Q�[���J�n���o�X�^�[�g
	case SEQ_START_EFF_START:
		OS_Printf( "*********\n�Q�[���J�n���o\n************\n" );

		//�{�[���̏����ʒu����ʊO�ɃZ�b�g
		for( i=0; i < wk->ball_max ;i++ ){
			p_vec = FactoryBall_GetPos( wk->p_ball[i] );		//���݂̍��W�擾
			FactoryBall_Entrance( wk->p_ball[i] );				//���W�Z�b�g
		}

		//�~�܂������ɃX�N���[���ʒu�������悤�ɂ���
		GF_BGL_ScrollSet( wk->bgl, BF_FRAME_SLIDE, GF_BGL_SCROLL_X_SET, FACTORY_SCROLL_OFFSET_X );

		SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 1 );		//��\��

		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_FACTORY );

		wk->sub_seq++;
		break;

	//���C�v�I���҂�
	case SEQ_START_EFF_WIPE_END_CHECK:
		if( WIPE_SYS_EndCheck() == FALSE ){
			break;
		}
		Snd_SePlay( SEQ_SE_DP_ELEBETA2 );
		wk->wait_count = 0;
		wk->sub_seq++;
		break;

	//���[���ړ�
	case SEQ_START_EFF_RAIL_MOVE:
		if( RailMoveIn(wk) == TRUE ){
			Snd_SeStopBySeqNo( SEQ_SE_DP_ELEBETA2, 0 );
			Snd_SePlay( SEQ_SE_DP_KASYA );

			//�{�[���h��A�j��
			for( i=0; i < wk->ball_max; i++ ){
				FactoryBall_AnmChg( wk->p_ball[i], ANM_BALL_YURE );
			}

			//�E�B���h�E�����J�n
			Factory_ObjWinSet( wk, ANM_SINGLE_OPEN );	//�J��

			//�l����ԏ������p���b�g��6�ŁA�����p���b�g��8�ɂȂ��Ă���
			wk->maru_wait		= ( FACTORY_MARU_WAIT * 2 );

			wk->wait_count = 0;
			wk->sub_seq++;
		}
		break;

	//�|�P�����\���^�C�~���O�����킹��
	case SEQ_START_EFF_POKE_APPEAR_TIMING:
		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜
		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�|�P�����t�F�[�h�C��
	case SEQ_START_EFF_POKE_FADEIN:
		if( wk->wait_count == 0 ){
			Factory_SetMainBgScrn( wk, BF_FRAME_BG );
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );		//BG��\��
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 0), 21, 11 );
			SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 0 );				//�\��
			SoftSpritePalFadeSetAll( wk->soft_sprite, 16, 0, 1, 0xffff );	//������t�F�[�h�C��
		}

		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		wk->wait_count++;
		if( wk->wait_count < POKE_FLASH_WAIT ){
			break;
		}
		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_NORMAL, 0), 21, 11 );
		wk->wait_count = 0;

		//BMP�Ƃ��Ďg�p����BG�ʂ��N���A���Ă���
		Factory_FrameWinBmpSet( wk );

		return TRUE;
		//break;
		
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�Q�[��������
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( FACTORY_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	case 0:
		OS_Printf( "*********\n�Q�[��������\n************\n" );
		if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){		//�����^��
			RentalInit( wk );
		}else{
			TradeInit( wk );
		}

		//�ʐM�^�C�v�̎�
		if( Factory_CommCheck(wk->type) == TRUE ){
			PairNameWrite(wk, &wk->bmpwin[BMPWIN_TR2], 0, 0, BF_FONT );	//�p�[�g�i�[����\��
		}

		//�J�n���o���I�����Ă��鎞(���������邩���ʕ��A���Ȃ�)�̓��C�v������
		if( wk->start_eff_flag == 1 ){
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
							WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_FACTORY );
		}

		wk->start_eff_flag = 1;											//�J�n���o�I��
		wk->sub_seq++;
		break;
		
	//�t�F�[�h�I���҂�
	case 1:
		//OS_Printf( "*********\n�Q�[�������� ���C�v�҂�\n************\n" );
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�����^��������
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static void RentalInit( FACTORY_WORK* wk )
{
	int i;
	u8 m_max;

	m_max = Factory_GetMinePokeNum( wk->type );

	Factory_SetCancel( wk, 0 );

	//�J�[�\������
	wk->p_csr = FactoryCsr_Create(	&wk->factory_clact, 
									wk->ball_max, wk->ball_max,
									CSR_H_MODE, wk->psd_pos, bf_h_ball_csr_pos, NULL );

	//�v���C���[����\��
	PlayerNameWrite(wk, &wk->bmpwin[BMPWIN_TR1], 0, 0, BF_FONT );

	//�I��������Ԃɂ���
	for( i=0; i < wk->sel_count ;i++ ){
		FactoryBall_Decide( wk->p_ball[ wk->sel_work[i] ] );
		FactoryBall_PalChg( wk->p_ball[ wk->sel_work[i] ], PAL_BALL_MOVE );
		FactoryBall_AnmChg( wk->p_ball[ wk->sel_work[i] ], ANM_BALL_MOVE );

		PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+i], 
						wk->sel_work[i], 0, 0, 
						//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
						FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
	}

	//���ݑI�����Ă���|�P�������\��
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
					//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

	//��b�E�B���h�E�\��
	FactoryTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );

	//�u���΂�߂́@�|�P�������@�����Ł@���������v
	Factory_SetNumber( wk, 0, (wk->sel_count+1) );
	wk->msg_index = Factory_EasyMsg( wk, msg_rental_01 );

	if( wk->recover == 1 ){

		FactoryCsr_Pause( wk->p_csr, 1 );	//�J�[�\���|�[�Y

		//���j���[����
		Factory_SetMenu( wk );

		//���j���[�J�[�\������
		wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
											POKE_SEL_MENU_MAX, POKE_SEL_MENU_MAX, 
											CSR_V_MODE, 0, bf_v_menu_csr_pos, NULL );

		//�u�悳�������I�񂾎��ɕ\����������̂őΏ�����v
		if( (wk->pair_poke_monsno[0] != 0) && (wk->pair_poke_monsno[1] != 0) ){

			//�p�[�g�i�[�̑I�񂾃|�P������\��
			for( i=0; i < m_max ;i++ ){
#if 0
				PokeNameWriteEx(wk, &wk->bmpwin[BMPWIN_POKE4+i],				//���ʎw��L��
								0, 0, 
								//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, 
								FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, 
								wk->pair_poke_monsno[i], wk->pair_poke_sex[i] );

				GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE4+i] );
#else
				GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE4+i], FBMP_COL_NULL );	//�h��Ԃ�

				//�p�[�g�i�[�̑I�񂾃|�P������\��
				if( i < wk->pair_sel_count ){
					PokeNameWriteEx(wk, &wk->bmpwin[BMPWIN_POKE4+i],				//���ʎw��L��
									0, 0, 
									//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, 
									FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, 
									wk->pair_poke_monsno[i], wk->pair_poke_sex[i] );
				}

				GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE4+i] );
#endif
			}

		}

	}

	return;
}

static void RentalBallInit( FACTORY_WORK* wk )
{
	int i;

	//�{�[������
	for( i=0; i < wk->ball_max ;i++ ){
		wk->p_ball[i] = Factory_BallAdd( wk, i );
		FactoryBall_Vanish( wk->p_ball[i], FACTORY_VANISH_OFF );	//�\��
	}

	return;
}

static void RentalPokeInit( FACTORY_WORK* wk )
{
	Factory_PokeAdd(wk, 0, PokeParty_GetMemberPointer(wk->p_m_party,wk->psd_pos),
					TEST_X, TEST_Y, FLIP_RENTAL );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����������
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static void TradeInit( FACTORY_WORK* wk )
{
	int i;

	Factory_SetCancel( wk, 0 );

	//��b�E�B���h�E�\��
	FactoryTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );

	//�J�[�\������
	if( Factory_CommCheck(wk->type) == FALSE ){
		wk->p_csr = FactoryCsr_Create(	&wk->factory_clact, 
										wk->tebanasu_max, BF_HV_TYUUSI_H_MAX, 
										CSR_HV_MODE, wk->psd_pos, 
										bf_hv_tyuusi_csr_pos, bf_hv_tyuusi_anm_tbl );
	}else{
		wk->p_csr = FactoryCsr_Create(	&wk->factory_clact, 
										wk->tebanasu_max, BF_HV_TYUUSI_H_MULTI_MAX, 
										CSR_HV_MODE, wk->psd_pos, 
										bf_hv_tyuusi_multi_csr_pos, bf_hv_tyuusi_multi_anm_tbl );
	}

	//�v���C���[����\��
	PlayerNameWrite(wk, &wk->bmpwin[BMPWIN_TR1], 0, 0, BF_FONT );

	//�K��1�C�̓|�P�������\��
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
					//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

	//�E�����j���[�\���u���イ���v
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	wk->msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_SEL], msg_f_change_choice_01, 
									1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_SEL] );

	//�u�Ă΂Ȃ��@�|�P�������@�����Ł@���������v
	wk->msg_index = Factory_EasyMsg( wk, msg_f_change_01 );

	if( wk->recover == 1 ){
		//���j���[����
		Factory_SetMenu( wk );

		//���j���[�J�[�\������
		wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
											POKE_SEL_MENU_MAX, POKE_SEL_MENU_MAX, 
											CSR_V_MODE, 0, bf_v_menu_csr_pos, NULL );
	}
	return;
}

static void TradeBallInit( FACTORY_WORK* wk )
{
	int i;

	SDK_ASSERTMSG( wk->p_ball[0] == NULL, "wk->p_ball[0]��NULL�ł͂���܂���I" );

	//�{�[������
	for( i=0; i < wk->ball_max ;i++ ){
		wk->p_ball[i] = Factory_BallAdd( wk, i );
		FactoryBall_Vanish( wk->p_ball[i], FACTORY_VANISH_OFF );

		//������̎�
		if( wk->tebanasu_uketoru == 0 ){
			FactoryBall_Decide( wk->p_ball[i] );						//���肵����Ԃ�
			FactoryBall_PalChg( wk->p_ball[i], PAL_BALL_MOVE );
			FactoryBall_AnmChg( wk->p_ball[i], ANM_BALL_STOP );
		}
	}

	return;
}

static void TradePokeInit( FACTORY_WORK* wk )
{
	Factory_PokeAdd(wk, 0, PokeParty_GetMemberPointer(wk->p_m_party,wk->psd_pos),
					TEST_X, TEST_Y, FLIP_TEBANASU );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�|�P�����X�e�[�^�X����
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GamePsd( FACTORY_WORK* wk )
{
	switch( wk->sub_seq ){

	case 0:
		SDK_ASSERTMSG( wk->child_proc != NULL, "child_proc��NULL�ł��I" );

		if( ProcMain(wk->child_proc) == TRUE ){
			OS_Printf( "*********\n�X�e�[�^�X�I��\n************\n" );
			wk->psd_pos = wk->psd->pos;									//���ʎ擾
			OS_Printf( "wk->psd_pos = %d\n", wk->psd_pos );
			sys_FreeMemoryEz( wk->psd );
			sys_FreeMemoryEz( wk->child_proc );
			wk->child_proc = NULL;
			Factory_Recover( wk );
			wk->psd_flag = 0;
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[�������^���|�P�����I��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameRental( FACTORY_WORK* wk )
{
	int i;
	u32 ret;

	switch( wk->sub_seq ){

	case 0:
		OS_Printf( "*********\n�Q�[�������^��\n************\n" );

		//�|�P�����X�e�[�^�X�̕��A���ɂ���Ĕ�ѐ��ύX���Ă���
		if( wk->recover == 0 ){
			wk->sub_seq = 1;
		}else{
			wk->sub_seq = 2;
		}
		wk->recover = 0;
		break;

	//�����^���|�P������I��
	case 1:
		FactoryCsr_Move( wk->p_csr );			//�J�[�\���ړ�����

		//���E�L�[�������ꂽ��\�����Ă���|�P������ύX����
		if( sys.trg & (PAD_KEY_LEFT | PAD_KEY_RIGHT) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			FactoryCommon_ChangePoke(	wk, wk->sel_count, 
										FactoryCsr_GetCsrPos(wk->p_csr), FLIP_RENTAL, 
										wk->p_m_party );
		}

		//����{�^�����������烁�j���[�쐬
		if( sys.trg & PAD_BUTTON_A ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			FactoryCsr_Pause( wk->p_csr, 1 );	//�J�[�\���|�[�Y
			Factory_SetMenu( wk );

			//�J�[�\������
			wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
												POKE_SEL_MENU_MAX, POKE_SEL_MENU_MAX,
												CSR_V_MODE, 0, bf_v_menu_csr_pos, NULL );
			wk->sub_seq++;

		}else if( sys.trg & PAD_BUTTON_B ){

			//�L�����Z���{�^������������1�O�ɖ߂�
			if( wk->sel_count > 0 ){
				Snd_SePlay( SEQ_SE_DP_SELECT );
				SetBCancel( wk );

				//�I�񂾃|�P�����i���o�[�𑗐M
				if( Factory_CommCheck(wk->type) == TRUE ){
					Factory_CommSetSendBuf( wk, FACTORY_COMM_PAIR_POKE, 0 );
				}

				return TRUE;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			}
		}

		break;

	//���j���[����
	case 2:
		ret = BmpMenuMain( wk->mw );
		FactoryCsr_Move( wk->p_csr_menu );		//�J�[�\���ړ�����

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�悳���݂�v
		case RET_PARAM_TUYOSA:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );	//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			wk->psd_flag = 1;

			//�t�F�[�h�A�E�g
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
							WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FACTORY );
			wk->sub_seq++;
			break;

		//�u�����^������v
		case RET_PARAM_RENTAL:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			SetRentarusuru( wk );

			//�I�񂾃|�P�����i���o�[�𑗐M
			if( Factory_CommCheck(wk->type) == TRUE ){
				Factory_CommSetSendBuf( wk, FACTORY_COMM_PAIR_POKE, 0 );
			}

			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			
		//�u�͂����v
		case RET_PARAM_HAZUSU:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			SetHazusu( wk );

			//�I�񂾃|�P�����i���o�[�𑗐M
			if( Factory_CommCheck(wk->type) == TRUE ){
				Factory_CommSetSendBuf( wk, FACTORY_COMM_PAIR_POKE, 0 );
			}

			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

		//�u�ׂ̂ɂ���v�EB�L�����Z��
		case BMPMENU_CANCEL:
		case RET_PARAM_BETUNO:
		default:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Pause( wk->p_csr, 0 );	//�J�[�\���|�[�Y����
			FactoryCsr_Delete( wk->p_csr_menu );//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		};

		break;

	case 3:
		//�t�F�[�h�I���҂�
		if( WIPE_SYS_EndCheck() == TRUE ){
			Factory_SetPSD( wk );
			FactoryCommon_Delete( wk );			//�폜����
			wk->child_proc = PROC_Create( &PokeStatusProcData, wk->psd, HEAPID_FACTORY );
			wk->recover = 1;
			//wk->sub_seq++;
			return TRUE;
		}
		break;

	case 4:
		if( wk->child_proc == NULL ){
			return TRUE;
		}
		break;

	case 5:
		//�t�F�[�h�I���҂�
		if( WIPE_SYS_EndCheck() == TRUE ){
			wk->sub_seq = 2;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			//return TRUE;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�FB�L�����Z�����������̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetBCancel( FACTORY_WORK* wk )
{
	//���ݑI�𒆂̃|�P������������
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1+wk->sel_count] );

	//1�O�ɑI�������|�P�������������āA���ݑI�𒆂̃|�P�������ɏ���������
	wk->sel_count--;
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], FBMP_COL_NULL );	//�h��Ԃ�
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
					//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

	//�u���΂�߂́@�|�P�������@�����Ł@���������v
	Factory_SetNumber( wk, 0, (wk->sel_count+1) );
	wk->msg_index = Factory_EasyMsg( wk, msg_rental_01 );

	FactoryBall_Cancel( wk->p_ball[ wk->sel_work[wk->sel_count] ] );	//�L�����Z�����
	FactoryBall_PalChg( wk->p_ball[ wk->sel_work[wk->sel_count] ], PAL_BALL_STOP );
	FactoryBall_AnmChg( wk->p_ball[ wk->sel_work[wk->sel_count] ], ANM_BALL_STOP );

	wk->sel_work[wk->sel_count] = 0;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�u�����^������v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetRentarusuru( FACTORY_WORK* wk )
{
	FactoryBall_Decide( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ] );	//���肵����Ԃ�
	FactoryBall_PalChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], PAL_BALL_MOVE );
	FactoryBall_AnmChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], ANM_BALL_MOVE );

	wk->sel_work[wk->sel_count] = FactoryCsr_GetCsrPos( wk->p_csr );

	wk->sel_count++;

	if( wk->sel_count == Factory_GetRentalSelPokeNum(wk->type) ){
		FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
		wk->p_csr_menu = NULL;
	}else{
		FactoryCsr_Pause( wk->p_csr, 0 );			//�J�[�\���|�[�Y����
		FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
		wk->p_csr_menu = NULL;

		PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], 
						FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
						//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
						FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

		//�u���΂�߂́@�|�P�������@�����Ł@���������v
		Factory_SetNumber( wk, 0, (wk->sel_count+1) );
		wk->msg_index = Factory_EasyMsg( wk, msg_rental_01 );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�u�͂����v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetHazusu( FACTORY_WORK* wk )
{
	u8 count;
	int i;

	FactoryBall_Cancel( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ] );	//�L�����Z�����
	FactoryBall_PalChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], PAL_BALL_STOP );
	FactoryBall_AnmChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], ANM_BALL_STOP );

#if 1
	//-----------------------------------------------------
	//�|�P�����I����4�C�ȏ�ɂȂ������͏C�����Ȃ��ƃ_���I)
	//-----------------------------------------------------
	
	//���ł�2�C�I��ł��鎞
	if( wk->sel_count >= 2 ){

		//1�C�ڂ��O�����Ƃ��Ă�����
		if( wk->sel_work[0] == FactoryCsr_GetCsrPos(wk->p_csr) ){

			//2�C�ڂ�1�C�ڂɑ������I
			wk->sel_work[0] = wk->sel_work[1];

			//�|�P����������
			SoftSpriteDel( wk->ss[0] );

			//�|�P�����\��
			Factory_PokeAdd(wk, 0, 
						PokeParty_GetMemberPointer(wk->p_m_party,FactoryCsr_GetCsrPos(wk->p_csr)),
						TEST_X, TEST_Y, FLIP_RENTAL );
		}
	}
#endif

	wk->sel_count--;
	wk->sel_work[wk->sel_count] = 0;

	FactoryCsr_Pause( wk->p_csr, 0 );			//�J�[�\���|�[�Y����
	FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
	wk->p_csr_menu = NULL;

	//�����^�������擾���āA�|�P�������E�B���h�E��h��Ԃ�
	count = Factory_GetRentalSelPokeNum( wk->type );
	for( i=0; i < count ;i++ ){
		GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1+i], FBMP_COL_NULL );
	}

	//�I�����Ă��鐔�A�|�P��������\��
	for( i=0; i <  wk->sel_count ;i++ ){
		PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+i], 
						wk->sel_work[i], 0, 0, 
						//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
						FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
	}

	//���ݑI�𒆂̃|�P��������\��
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0,
					//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

	//���f
	for( i=0; i < count ;i++ ){
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1+i] );
	}

	//�u���΂�߂́@�|�P�������@�����Ł@���������v
	Factory_SetNumber( wk, 0, (wk->sel_count+1) );
	wk->msg_index = Factory_EasyMsg( wk, msg_rental_01 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�ȏ��3�C�ł�낵���ł����H
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameFinalAnswer( FACTORY_WORK* wk )
{
	u8 m_max;
	u32 ret;
	int i;
	int x_tbl[3];

	enum{
		SEQ_FINAL_ANSWER_START = 0,
		SEQ_FINAL_ANSWER_VANISH_TIMING,
		SEQ_FINAL_ANSWER_POKE_FADEOUT,
		SEQ_FINAL_ANSWER_POKE_APPEAR_TIMING,
		SEQ_FINAL_ANSWER_POKE_FADEIN,
		SEQ_FINAL_ANSWER_MENU,
		SEQ_FINAL_ANSWER_SEND,
		SEQ_FINAL_ANSWER_TIMING_SYNC,
	};

	m_max = Factory_GetMinePokeNum( wk->type );

	switch( wk->sub_seq ){

	//�ŏI�m�F�X�^�[�g
	case SEQ_FINAL_ANSWER_START:

		FactoryCsr_Vanish( wk->p_csr, FACTORY_VANISH_ON );		//�o�j�b�V��

		//�y�A�̃f�[�^��\�����Ȃ��t���OON
		wk->pair_data_off_flag = 1;
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TR2] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE4] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE5] );

#if 1
		//�|�P������������
		for( i=0; i < wk->sel_count; i++ ){
			GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1+i], FBMP_COL_NULL );	//�����F
			GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1+i] );
		}

		//�v���C���[��������
		GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_TR1], FBMP_COL_NULL );			//�����F
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_TR1] );
#endif

		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 0), 21, 11 );
		SoftSpritePalFadeSetAll( wk->soft_sprite, 0, 16, 0, 0xffff );		//�t�F�[�h�A�E�g���Ĕ�

		wk->wait_count = 0;
		wk->sub_seq++;
		break;

	//BG����A�|�P�����z���C�g�A�E�g�I�������A�w�i��؂�ւ���
	case SEQ_FINAL_ANSWER_VANISH_TIMING:
		wk->wait_count++;
		if( wk->wait_count < POKE_FLASH_WAIT ){
			break;
		}

		SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 1 );					//��\��
		Factory_ObjWinSet( wk, ANM_SINGLE_CLOSE );	//����
		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_OPEN, 0), 21, 11 );

		//�X�N���[���؂�ւ�
		Factory_SetRentalBackBgScrn( wk, BF_FRAME_BG );

		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�E�B���h�E�A�j���I��������A3�C�̕\������
	case SEQ_FINAL_ANSWER_POKE_FADEOUT:
		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜

		//������I�񂾏��ԂɃ|�P������\������
		SoftSpriteDel( wk->ss[0] );
		x_tbl[0] = TEST_X1;
		x_tbl[1] = TEST_X2;
		x_tbl[2] = TEST_X3;

		//�S�Ă̑I�������|�P������\��
		for( i=0; i < m_max ;i++ ){
			Factory_PokeAdd(wk, i, PokeParty_GetMemberPointer(wk->p_m_party,wk->sel_work[i]),
							x_tbl[i], TEST_Y, FLIP_RENTAL );
			SoftSpriteParaSet( wk->ss[i], SS_PARA_VANISH, 1 );				//��\��
		}

		Factory_ObjWinSet( wk, ANM_3POKE_OPEN );	//�J��
		wk->sub_seq++;
		break;

	//�|�P�����\��
	case SEQ_FINAL_ANSWER_POKE_APPEAR_TIMING:
		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜
		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�|�P�����t�F�[�h�C����
	case SEQ_FINAL_ANSWER_POKE_FADEIN:
		if( wk->wait_count == 0 ){

			//�X�N���[���؂�ւ�
			Factory_Set3PokeBgScrn( wk, BF_FRAME_BG );

			//�S�Ă̑I�������|�P������\��
			for( i=0; i < m_max ;i++ ){
				SoftSpriteParaSet( wk->ss[i], SS_PARA_VANISH, 0 );			//�\��
			}

			if( Factory_CommCheck(wk->type) == FALSE ){
				ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk,ID_PAL_SINGLE_FLASH,1), 32, 11 );
			}else{
				ScrPalChg(wk,BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 1), 21, 11 );
			}
			SoftSpritePalFadeSetAll( wk->soft_sprite, 16, 0, 1, 0xffff );	//������t�F�[�h�C��
		}

		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		wk->wait_count++;
		if( wk->wait_count < POKE_APPEAR_WAIT ){
			break;
		}

		if( Factory_CommCheck(wk->type) == FALSE ){
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk,ID_PAL_SINGLE_NORMAL,1), 32, 11 );
		}else{
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_NORMAL, 1), 21, 11);
		}

		//�y�A�̃f�[�^��\�����Ȃ��t���OOFF
		wk->pair_data_off_flag = 0;

		//�p�[�g�i�[�̑I�񂾃|�P������\�����N�G�X�g
		if( Factory_CommCheck(wk->type) == TRUE ){
			wk->pair_monsno_req = REQ_PAIR_MONSNO_DECIDE;
			Factory_PairMonsNoRecvReq( wk );
		}

		//�u�����傤�́@�R�Ђ��Ł@��낵���ł����H�v
		Factory_SetNumber( wk, 0, m_max );
		wk->msg_index = Factory_EasyMsg( wk, msg_rental_02 );
		Factory_SetMenu2( wk );							//�u�͂��A�������v
		wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
											YES_NO_MENU_MAX, YES_NO_MENU_MAX,
											CSR_V_MODE, 0, bf_v_yesno_csr_pos, NULL );
		wk->wait_count = 0;
		wk->sub_seq++;
		break;

	//���j���[����
	case SEQ_FINAL_ANSWER_MENU:
		ret = BmpMenuMain( wk->mw );
		FactoryCsr_Move( wk->p_csr_menu );				//�J�[�\���ړ�����

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case RET_PARAM_YES:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			wk->sub_seq++;
			break;

		//�u�������v�EB�L�����Z��
		case BMPMENU_CANCEL:
		case RET_PARAM_NO:
		default:
			FactoryCsr_Vanish( wk->p_csr, FACTORY_VANISH_OFF );	
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			wk->sel_count--;
			wk->sel_work[wk->sel_count] = 0;

			//�I�񂾃|�P�����i���o�[�𑗐M
			if( Factory_CommCheck(wk->type) == TRUE ){
				Factory_CommSetSendBuf( wk, FACTORY_COMM_PAIR_POKE, 0 );
			}

			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]
		};

		break;

	//
	case SEQ_FINAL_ANSWER_SEND:
		//�ʐM�^�C�v�łȂ����͏I��
		if( Factory_CommCheck(wk->type) == FALSE ){
			return TRUE;
		}

		//���M����������
		if( Factory_CommSetSendBuf(wk,FACTORY_COMM_PAIR_POKE,0) == TRUE ){

			//���b�Z�[�W�\��
			wk->msg_index = Factory_EasyMsg( wk, msg_rental_03 );

			//�����J�n
			//CommToolInitialize( HEAPID_FACTORY );	//timingSyncEnd=0xff
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_FACTORY_RENTAL_END );
			wk->sub_seq++;
		}
		break;

	//�����҂�
	case SEQ_FINAL_ANSWER_TIMING_SYNC:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_FACTORY_RENTAL_END) == TRUE ){
			CommToolTempDataReset();
			return TRUE;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�ȏ��3�C�ł�낵���ł����H(�L�����Z��)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameFinalAnswerCancel( FACTORY_WORK* wk )
{
	u8 m_max;
	int i;

	enum{
		SEQ_FINAL_ANSWER_CANCEL_START = 0,
		SEQ_FINAL_ANSWER_CANCEL_VANISH_TIMING,
		SEQ_FINAL_ANSWER_CANCEL_POKE_FADEOUT,
		SEQ_FINAL_ANSWER_CANCEL_POKE_APPEAR_TIMING,
		SEQ_FINAL_ANSWER_CANCEL_POKE_FADEIN,
	};

	m_max = Factory_GetMinePokeNum( wk->type );

	switch( wk->sub_seq ){

	//�ŏI�m�F�X�^�[�g
	case SEQ_FINAL_ANSWER_CANCEL_START:
		//�y�A�̃f�[�^��\�����Ȃ��t���OON
		wk->pair_data_off_flag = 1;
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TR2] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE4] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE5] );

		if( Factory_CommCheck(wk->type) == FALSE ){
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 1), 32, 11 );
		}else{
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 1), 21, 11 );
		}
		SoftSpritePalFadeSetAll( wk->soft_sprite, 0, 16, 0, 0xffff );		//�t�F�[�h�A�E�g���Ĕ�
		wk->wait_count = 0;
		wk->sub_seq++;
		break;

	//BG����A�|�P�����z���C�g�A�E�g�I�������A�w�i��؂�ւ���
	case SEQ_FINAL_ANSWER_CANCEL_VANISH_TIMING:
		wk->wait_count++;
		if( wk->wait_count < POKE_FLASH_WAIT ){
			break;
		}

		//�S�Ă̑I�������|�P�������폜
		for( i=0; i < m_max ;i++ ){
			SoftSpriteDel( wk->ss[i] );
		}

		Factory_ObjWinSet( wk, ANM_3POKE_CLOSE );	//����
		if( Factory_CommCheck(wk->type) == FALSE ){
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_OPEN, 0), 32, 11 );
		}else{
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_OPEN, 0), 21, 11 );
		}

		//�X�N���[���؂�ւ�
		Factory_SetRentalBackBgScrn( wk, BF_FRAME_BG );

		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�E�B���h�E�A�j���I��������A3�C�̕\������
	case SEQ_FINAL_ANSWER_CANCEL_POKE_FADEOUT:
		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜

		//���ݑI�����Ă���|�P������\��
		Factory_PokeAdd(wk, 0, 
						PokeParty_GetMemberPointer(wk->p_m_party,FactoryCsr_GetCsrPos(wk->p_csr)),
						TEST_X, TEST_Y, FLIP_RENTAL );
		SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 1 );					//��\��

		Factory_ObjWinSet( wk, ANM_SINGLE_OPEN );	//�J��
		wk->sub_seq++;
		break;

	//�|�P�����\��
	case SEQ_FINAL_ANSWER_CANCEL_POKE_APPEAR_TIMING:
		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜
		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�|�P�����t�F�[�h�C����
	case SEQ_FINAL_ANSWER_CANCEL_POKE_FADEIN:
		if( wk->wait_count == 0 ){
			Factory_SetMainBgScrn( wk, BF_FRAME_BG );
			SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 0 );				//�\��
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 0), 21, 11 );
			SoftSpritePalFadeSetAll( wk->soft_sprite, 16, 0, 1, 0xffff );	//������t�F�[�h�C��
		}

		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		wk->wait_count++;
		if( wk->wait_count < POKE_APPEAR_WAIT ){
			break;
		}
		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_NORMAL, 0), 21, 11 );

		//�y�A�̃f�[�^��\�����Ȃ��t���OOFF
		wk->pair_data_off_flag = 0;

		//�p�[�g�i�[�̑I�񂾃|�P������\�����N�G�X�g
		if( Factory_CommCheck(wk->type) == TRUE ){
			wk->pair_monsno_req = REQ_PAIR_MONSNO_DECIDE;
			Factory_PairMonsNoRecvReq( wk );
		}

		SetFinalAnswerNo( wk );
		return TRUE;
		//break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�u�ȏ��3�C�ł�낵���ł����H�v�u�������v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetFinalAnswerNo( FACTORY_WORK* wk )
{
	int i;
	u8 m_max;

	m_max = Factory_GetMinePokeNum( wk->type );

	//�|�P��������\��
	for( i=0; i < wk->sel_count; i++ ){
		PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+i], 
						wk->sel_work[i], 0, 0, 
						//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
						FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
	}

	//�I�𒆂̃|�P��������\��
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

	//�v���C���[����\��
	PlayerNameWrite(wk, &wk->bmpwin[BMPWIN_TR1], 0, 0, BF_FONT );

	FactoryCsr_Pause( wk->p_csr, 0 );										//�J�[�\���|�[�Y����
	FactoryBall_Cancel( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ] );	//�L�����Z����Ԃ�
	FactoryBall_PalChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], PAL_BALL_STOP );
	FactoryBall_AnmChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], ANM_BALL_STOP );

	//�u���΂�߂́@�|�P�������@�����Ł@���������v
	Factory_SetNumber( wk, 0, (wk->sel_count+1) );
	wk->msg_index = Factory_EasyMsg( wk, msg_rental_01 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[������������|�P�����I��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameTradeTebanasu( FACTORY_WORK* wk )
{
	int i;
	u32 ret;

	switch( wk->sub_seq ){

	case 0:
		Factory_SetCancel( wk, 0 );

		//�|�P�����X�e�[�^�X�̕��A���ɂ���Ĕ�ѐ��ύX���Ă���
		if( wk->recover == 0 ){
			wk->sub_seq = 1;
		}else{
			wk->sub_seq = 2;
		}
		wk->recover = 0;
		break;

	//�����|�P������I��
	case 1:
		FactoryCsr_Move( wk->p_csr );			//�J�[�\���ړ�����

		//�㉺���E�L�[�������ꂽ��\�����Ă���|�P������ύX����
		if( sys.trg & (PAD_KEY_LEFT | PAD_KEY_RIGHT | PAD_KEY_UP | PAD_KEY_DOWN) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			FactoryCommon_ChangePoke(	wk, wk->sel_count, 
										FactoryCsr_GetCsrPos(wk->p_csr), FLIP_TEBANASU,
										wk->p_m_party );
		}

		//����{�^�����������烁�j���[�쐬
		if( sys.trg & PAD_BUTTON_A ){

			Snd_SePlay( SEQ_SE_DP_SELECT );

			//�u���イ���v��I��ł�����
			if( FactoryCsr_GetCsrPos(wk->p_csr) == (wk->tebanasu_max-1) ){
				Factory_SetCancel( wk, 1 );
				return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]
			}else{
				FactoryCsr_Pause( wk->p_csr, 1 );	//�J�[�\���|�[�Y
				Factory_SetMenu( wk );

				//�J�[�\������
				wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
													POKE_SEL_MENU_MAX, POKE_SEL_MENU_MAX, 
													CSR_V_MODE, 0, bf_v_menu_csr_pos, NULL );
				wk->sub_seq++;
			}

		}else if( sys.trg & PAD_BUTTON_B ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			Factory_SetCancel( wk, 1 );
			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		}

		break;

	//���j���[����
	case 2:
		ret = BmpMenuMain( wk->mw );
		FactoryCsr_Move( wk->p_csr_menu );		//�J�[�\���ړ�����

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�悳���݂�v
		case RET_PARAM_TUYOSA:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );	//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			wk->psd_flag = 1;

			//�t�F�[�h�A�E�g
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
							WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FACTORY );
			wk->sub_seq++;
			break;

		//�u�Ă΂Ȃ��v
		case RET_PARAM_TEBANASU:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			SetTebanasu( wk );
			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			
		//�u�ׂ̂ɂ���v�EB�L�����Z��
		case BMPMENU_CANCEL:
		case RET_PARAM_BETUNO:
		default:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Pause( wk->p_csr, 0 );	//�J�[�\���|�[�Y����
			FactoryCsr_Delete( wk->p_csr_menu );//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;

			//�E�����j���[�\���u���イ���v
			FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
			wk->msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_SEL], msg_f_change_choice_01, 
										1, 1, MSG_NO_PUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
			GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_SEL] );

			wk->sub_seq = 1;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			break;
			//return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		};

		break;

	case 3:
		//�t�F�[�h�I���҂�
		if( WIPE_SYS_EndCheck() == TRUE ){
			Factory_SetPSD( wk );
			FactoryCommon_Delete( wk );			//�폜����
			wk->child_proc = PROC_Create( &PokeStatusProcData, wk->psd, HEAPID_FACTORY );
			wk->recover = 1;
			//wk->sub_seq++;
			return TRUE;
		}
		break;

	case 4:
		if( wk->child_proc == NULL ){
			return TRUE;
		}
		break;

	case 5:
		//�t�F�[�h�I���҂�
		if( WIPE_SYS_EndCheck() == TRUE ){
			wk->sub_seq = 2;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			//return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�u�Ă΂Ȃ��v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetTebanasu( FACTORY_WORK* wk )
{
	FactoryBall_Decide( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ] );	//���肵����Ԃ�
	FactoryBall_PalChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], PAL_BALL_MOVE );
	FactoryBall_AnmChg( wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ], ANM_BALL_STOP );

	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+wk->sel_count], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
					//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
	wk->sel_work[wk->sel_count] = FactoryCsr_GetCsrPos( wk->p_csr );
	wk->sel_count++;

	FactoryCsr_Delete( wk->p_csr_menu );								//���j���[�J�[�\��OBJ�폜
	wk->p_csr_menu = NULL;
	FactoryCsr_Delete( wk->p_csr );										//�J�[�\��OBJ�폜
	wk->p_csr = NULL;

	//�󂯎��ɐ؂�ւ�
	wk->tebanasu_uketoru = 1;

	//�|�P��������
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1] );
	//SoftSpriteDel( wk->ss[0] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�������ʂŌ������~����H
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameTradeTebanasuTyuusi( FACTORY_WORK* wk )
{
	int i,flag;
	u32 ret;

	switch( wk->sub_seq ){

	//����
	case 0:
		//���ݑI�𒆂̃|�P������������
		GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1], FBMP_COL_NULL );	//�h��Ԃ�
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1+wk->sel_count] );
		SoftSpriteDel( wk->ss[0] );

		//�u�|�P�����́@����������@���イ���@���܂����H�v
		wk->msg_index = Factory_EasyMsg( wk, msg_f_change_02 );

		FactoryCsr_Vanish( wk->p_csr, FACTORY_VANISH_ON );		//�o�j�b�V��
		FactoryCsr_Pause( wk->p_csr, 1 );						//�J�[�\���|�[�Y
		Factory_SetMenu2( wk );									//�u�͂��A�������v
		wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
											YES_NO_MENU_MAX, YES_NO_MENU_MAX,
											CSR_V_MODE, 0, bf_v_yesno_csr_pos, NULL );
		wk->sub_seq++;
		break;

	//���j���[����
	case 1:
		ret = BmpMenuMain( wk->mw );
		FactoryCsr_Move( wk->p_csr_menu );				//�J�[�\���ړ�����

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case RET_PARAM_YES:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			wk->trade_flag = 0;							//���������t���OOFF

#if 1
			/*�u���΂炭���܂����������v*/
			if( Factory_CommCheck(wk->type) == TRUE ){
				wk->msg_index = Factory_EasyMsg( wk, msg_rental_03 );
			}
#endif

			wk->sub_seq++;
			break;

		//�u�������v�EB�L�����Z��
		case BMPMENU_CANCEL:
		case RET_PARAM_NO:
		default:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );		//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			SetTebanasuTyuusiNo( wk );
			wk->sub_seq++;
			break;
		};

		break;

	case 2:
		return TRUE;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�������ʂŁu���イ���v�u�������v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetTebanasuTyuusiNo( FACTORY_WORK* wk )
{
	FactoryCsr_SetCsrPos( wk->p_csr, 0 );

	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1], 
					FactoryCsr_GetCsrPos(wk->p_csr), 0, 0, 
					//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );
					FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_m_party );

	Factory_PokeAdd(wk, 0, 
					PokeParty_GetMemberPointer(wk->p_m_party,FactoryCsr_GetCsrPos(wk->p_csr)),
					TEST_X, TEST_Y, FLIP_TEBANASU );

	FactoryCsr_Vanish( wk->p_csr, FACTORY_VANISH_OFF );	
	FactoryCsr_Pause( wk->p_csr, 0 );					//�J�[�\���|�[�Y����
	Factory_SetCancel( wk, 1 );

	//�E�����j���[�\���u���イ���v
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	wk->msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_SEL], msg_f_change_choice_01, 
									1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_SEL] );

	//�u�Ă΂Ȃ��@�|�P�������@�����Ł@���������v
	wk->msg_index = Factory_EasyMsg( wk, msg_f_change_01 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�󂯎���ʂŃ|�P�����I��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameTradeUketoru( FACTORY_WORK* wk )
{
	int i,flag;
	u32 ret;
	u8 poke_pos;

	switch( wk->sub_seq ){

	case 0:
		//�E�����j���[�\���u���ǂ�v�u���イ���v(�h��Ԃ��Ȃ����g�p)
		FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
		wk->msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_SEL], msg_f_change_choice_05, 
									1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
		wk->msg_index = FactoryWriteMsgSimple(wk, &wk->bmpwin[BMPWIN_SEL], msg_f_change_choice_06, 
									1, 1+16, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_SEL] );

		//�J�[�\������
		if( Factory_CommCheck(wk->type) == FALSE ){
			wk->p_csr = FactoryCsr_Create(	&wk->factory_clact, 
											wk->uketoru_max, BF_HV_MODORU_H_MAX, 
											CSR_HV_MODE, 0, 
											bf_hv_modoru_csr_pos, bf_hv_modoru_anm_tbl );
		}else{
			wk->p_csr = FactoryCsr_Create(	&wk->factory_clact, 
											wk->uketoru_max, BF_HV_MODORU_H_MULTI_MAX, 
											CSR_HV_MODE, 0, 
											bf_hv_modoru_multi_csr_pos, bf_hv_modoru_multi_anm_tbl);
		}

		//�|�P�����\��
		poke_pos = FactoryCsr_GetCsrPos(wk->p_csr);
		PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1], 
						poke_pos, 0, 0, 
						//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_e_party );
						FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, wk->p_e_party );

		//�u�����Ƃ�@�|�P�������@�����Ł@���������v
		FactoryTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
		wk->msg_index = Factory_EasyMsg( wk, msg_f_change_05 );
		wk->sub_seq++;
		break;

	//�󂯎��|�P������I��
	case 1:
		FactoryCsr_Move( wk->p_csr );			//�J�[�\���ړ�����

		//�㉺�L�[
		if( sys.trg & (PAD_KEY_UP | PAD_KEY_DOWN) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
		}

		//���E�L�[�������ꂽ��\�����Ă���|�P������ύX����
		if( sys.trg & (PAD_KEY_LEFT | PAD_KEY_RIGHT) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			FactoryCommon_ChangePoke(	wk, 0, FactoryCsr_GetCsrPos(wk->p_csr), 
										FLIP_UKETORU, wk->p_e_party );
		}

		//����{�^�����������烁�j���[�쐬
		if( sys.trg & PAD_BUTTON_A ){

			Snd_SePlay( SEQ_SE_DP_SELECT );

			//�u���イ���v��I��ł�����(�������́AB�L�����Z��)
			if( FactoryCsr_GetCsrPos(wk->p_csr) == (wk->uketoru_max-1) ){
				Factory_SetCancel( wk, 1 );
				return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

			//�u���ǂ�v��I��ł�����
			}else if( FactoryCsr_GetCsrPos(wk->p_csr) == (wk->uketoru_max-2) ){
				SetModoru( wk );
				return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			}else{
				FactoryCsr_Pause( wk->p_csr, 1 );	//�J�[�\���|�[�Y
				Factory_SetMenu2( wk );				//�u�͂��A�������v
				wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
													YES_NO_MENU_MAX, YES_NO_MENU_MAX, 
													CSR_V_MODE, 0, bf_v_yesno_csr_pos, NULL );

				//�u���́@�|�P�������@�����Ƃ�܂����H�v
				wk->msg_index = Factory_EasyMsg( wk, msg_f_change_06 );
				wk->sub_seq++;
			}

		}else if( sys.trg & PAD_BUTTON_B ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			Factory_SetCancel( wk, 1 );
			return TRUE;							//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		}

		break;

	//���j���[����
	case 2:
		ret = BmpMenuMain( wk->mw );
		FactoryCsr_Move( wk->p_csr_menu );			//�J�[�\���ړ�����

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case RET_PARAM_YES:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			SetUketoru( wk );

#if 1
			/*�u���΂炭���܂����������v*/
			if( Factory_CommCheck(wk->type) == TRUE ){
				wk->msg_index = Factory_EasyMsg( wk, msg_rental_03 );
			}
#endif

			wk->sub_seq++;
			break;

		//�u�������v�EB�L�����Z��
		case BMPMENU_CANCEL:
		case RET_PARAM_NO:
		default:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			SetUketoruNo( wk );
			wk->sub_seq = 1;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			break;
		};

		break;

	case 3:
		return TRUE;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�󂯎���ʂŁu���ǂ�v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetModoru( FACTORY_WORK* wk )
{
	FactoryCsr_Delete( wk->p_csr );								//�J�[�\��OBJ�폜
	wk->p_csr = NULL;

	//�Ă΂Ȃ��ɐ؂�ւ�
	wk->tebanasu_uketoru = 0;

	//�|�P��������
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1] );
	SoftSpriteDel( wk->ss[0] );

	//�E�����j���[������
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_SEL], FBMP_COL_WHITE );	//�h��Ԃ�
	BmpTalkWinClear( &wk->bmpwin[BMPWIN_SEL], WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[BMPWIN_SEL] );

	wk->sel_count--;
	wk->sel_work[0] = 0;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�󂯎���ʂŁu�͂��v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetUketoru( FACTORY_WORK* wk )
{
	wk->trade_flag = 1;													//���������t���OON

	wk->sel_work[wk->sel_count] = FactoryCsr_GetCsrPos( wk->p_csr );
	wk->sel_count++;

	FactoryCsr_Delete( wk->p_csr_menu );								//���j���[�J�[�\��OBJ�폜
	wk->p_csr_menu = NULL;
	FactoryCsr_Vanish( wk->p_csr, FACTORY_VANISH_ON );					//�o�j�b�V��

	//�|�P��������
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1] );
	SoftSpriteDel( wk->ss[0] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�󂯎���ʂŁu�������v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetUketoruNo( FACTORY_WORK* wk )
{
	FactoryCsr_Delete( wk->p_csr_menu );	//���j���[�J�[�\��OBJ�폜
	wk->p_csr_menu = NULL;
	FactoryCsr_Pause( wk->p_csr, 0 );		//�J�[�\������

	//�E�����j���[�\���u���ǂ�v�u���イ���v(�h��Ԃ��Ȃ����g�p)
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	wk->msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_SEL], msg_f_change_choice_05, 
									1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
	wk->msg_index = FactoryWriteMsgSimple(wk,&wk->bmpwin[BMPWIN_SEL],msg_f_change_choice_06,
										1, 1+16, MSG_NO_PUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BF_FONT );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_SEL] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�󂯎���ʂŌ������~����H
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameTradeUketoruTyuusi( FACTORY_WORK* wk )
{
	int i,flag;
	u32 ret;

	switch( wk->sub_seq ){

	//����
	case 0:
		//���ݑI�𒆂̃|�P����������
		GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE1], FBMP_COL_NULL );	//�h��Ԃ�
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE1] );
		SoftSpriteDel( wk->ss[0] );

		//�u�|�P�����́@����������@���イ���@���܂����H�v
		FactoryTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
		wk->msg_index = Factory_EasyMsg( wk, msg_f_change_02 );

		//���̑I�����u�͂��A�������v�ŏ������A�����Ă邩�A���܂�̂ł����̓o�j�b�V��
		FactoryCsr_Vanish( wk->p_csr, FACTORY_VANISH_ON );		//�o�j�b�V��

		Factory_SetMenu2( wk );									//�u�͂��A�������v
		wk->p_csr_menu = FactoryCsr_Create( &wk->factory_clact, 
											YES_NO_MENU_MAX, YES_NO_MENU_MAX,
											CSR_V_MODE, 0, bf_v_yesno_csr_pos, NULL );
		wk->sub_seq++;
		break;

	//���j���[�����u�͂��A�������v
	case 1:
		ret = BmpMenuMain( wk->mw );
		FactoryCsr_Move( wk->p_csr_menu );		//�J�[�\���ړ�����

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case RET_PARAM_YES:
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );				//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			wk->trade_flag = 0;									//���������t���OOFF

#if 1
			/*�u���΂炭���܂����������v*/
			if( Factory_CommCheck(wk->type) == TRUE ){
				wk->msg_index = Factory_EasyMsg( wk, msg_rental_03 );
			}
#endif

			wk->sub_seq++;
			break;

		//�u�������v�EB�L�����Z��
		case BMPMENU_CANCEL:
		case RET_PARAM_NO:
		default:
			BmpMenuExit( wk->mw, NULL );
			//BmpMenuWinClearSub( wk->MenuH.win );				//����`�悷��܂łɊԂ�����̂ŁB
			FactoryCsr_Delete( wk->p_csr );						//�J�[�\��OBJ�폜
			wk->p_csr = NULL;
			FactoryCsr_Delete( wk->p_csr_menu );				//���j���[�J�[�\��OBJ�폜
			wk->p_csr_menu = NULL;
			SetUketoruTyuusiNo( wk );
			wk->sub_seq++;
			break;
		};

		break;

	case 2:
		return TRUE;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Z�b�g�F�󂯎���ʂŁu���イ���v���u�������v��I�񂾎��̏���
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void SetUketoruTyuusiNo( FACTORY_WORK* wk )
{
	Factory_PokeAdd(wk, 0, 
					PokeParty_GetMemberPointer(wk->p_e_party,0),
					TEST_X, TEST_Y, FLIP_UKETORU );

	Factory_SetCancel( wk, 1 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F����A�ޏ�
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameLeavingEntrance( FACTORY_WORK* wk )
{
	u8 m_max;
	int i,flag,x,y;
	VecFx32 vec;
	const VecFx32* p_vec;

	enum{
		SEQ_LEAVING_ENTRANCE_START = 0,
		SEQ_LEAVING_ENTRANCE_VANISH_TIMING,
		SEQ_LEAVING_ENTRANCE_POKE_FADEOUT,

		SEQ_LEAVING_ENTRANCE_RAIL_MOVE_OUT,
		SEQ_LEAVING_ENTRANCE_RAIL_MOVE_IN,

		SEQ_LEAVING_ENTRANCE_POKE_APPEAR_TIMING,
		SEQ_LEAVING_ENTRANCE_POKE_FADEIN,
	};

	m_max = Factory_GetMinePokeNum( wk->type );

	switch( wk->sub_seq ){

	//�X�^�[�g
	case SEQ_LEAVING_ENTRANCE_START:
		//�y�A�̃f�[�^��\�����Ȃ��t���OON
		wk->pair_data_off_flag = 1;
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TR2] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE4] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE5] );

		//�v���C���[��������
		GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_TR1], FBMP_COL_NULL );	//�����F
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_TR1] );

		//��b�E�B���h�E�폜
		BmpTalkWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_OFF );
		GF_BGL_BmpWinOffVReq( &wk->bmpwin[BMPWIN_TALK] );

		FactoryExitBmpWin( wk->bmpwin );									//BMP�E�B���h�E�J��

		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 0), 21, 11 );
		SoftSpritePalFadeSetAll( wk->soft_sprite, 0, 16, 0, 0xffff );		//�t�F�[�h�A�E�g���Ĕ�

		wk->wait_count = 0;
		wk->sub_seq++;
		break;

	//BG����A�|�P�����z���C�g�A�E�g�I�������A�w�i��؂�ւ���
	case SEQ_LEAVING_ENTRANCE_VANISH_TIMING:
		wk->wait_count++;
		if( wk->wait_count < POKE_FLASH_WAIT ){
			break;
		}

		SoftSpriteDel( wk->ss[0] );

		Factory_ObjWinSet( wk, ANM_SINGLE_CLOSE );	//����
		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_OPEN, 0), 21, 11 );

		//�X�N���[���؂�ւ�
		Factory_SetRentalBackBgScrn( wk, BF_FRAME_BG );

		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�E�B���h�E�A�j���I���҂�
	case SEQ_LEAVING_ENTRANCE_POKE_FADEOUT:
		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜

		Factory_SetMainBgGraphic_Maru( wk, BF_FRAME_WIN );					//�E��̉~
		Factory_SetMainBgGraphic_Rail( wk, BF_FRAME_SLIDE );				//���[��
		Factory_SetMainBgGraphic_Back( wk, BF_FRAME_BG );					//�w�i

		//�X�N���[���ʒu�̒���
		GF_BGL_ScrollSet( wk->bgl, BF_FRAME_SLIDE, GF_BGL_SCROLL_X_SET, wk->scr_x );

		Snd_SePlay( SEQ_SE_DP_ELEBETA2 );
		wk->wait_count = 0;
		wk->sub_seq++;
		break;

	//�ޏ�
	case SEQ_LEAVING_ENTRANCE_RAIL_MOVE_OUT:
		if( RailMoveOut(wk) == TRUE ){
			Snd_SeStopBySeqNo( SEQ_SE_DP_ELEBETA2, 0 );
			Snd_SePlay( SEQ_SE_DP_KASYA );

			//�l����ԏ������p���b�g��6�ŁA�����p���b�g��8�ɂȂ��Ă���
			wk->maru_wait		= ( FACTORY_MARU_WAIT * 2 );

			//�S�Ẵ{�[�����ޏꂵ����
			for( i=0; i < wk->ball_max; i++ ){
				wk->p_ball[i] = FactoryBall_Delete( wk->p_ball[i] );		//�폜
				wk->p_ball[i] = NULL;
			}

			//�ʐM�̎��́A�Ă΂Ȃ��A�����Ƃ�ŕ\������{�[���̐����ς��̂őΉ�
			if( Factory_CommCheck(wk->type) == TRUE ){
				if( wk->ball_max == BF_T_MULTI_BALL_TEBANASU_POS_MAX ){
					wk->ball_max = BF_T_MULTI_BALL_UKETORU_POS_MAX;
				}else{
					wk->ball_max = BF_T_MULTI_BALL_TEBANASU_POS_MAX;
				}
			}

			TradeBallInit( wk );											//�{�[���ǉ�
			for( i=0; i < wk->ball_max; i++ ){
				FactoryBall_Entrance( wk->p_ball[i] );						//���W�Z�b�g
			}

			Snd_SePlay( SEQ_SE_DP_ELEBETA2 );
			wk->wait_count = 0;
			wk->sub_seq++;
		}
		break;

	//����
	case SEQ_LEAVING_ENTRANCE_RAIL_MOVE_IN:
		if( RailMoveIn(wk) == TRUE ){
			Snd_SeStopBySeqNo( SEQ_SE_DP_ELEBETA2, 0 );
			Snd_SePlay( SEQ_SE_DP_KASYA );

			for( i=0; i < wk->ball_max; i++ ){
				FactoryBall_AnmChg( wk->p_ball[i], ANM_BALL_YURE );
			}

			//�l����ԏ������p���b�g��6�ŁA�����p���b�g��8�ɂȂ��Ă���
			wk->maru_wait		= ( FACTORY_MARU_WAIT * 2 );

			Factory_ObjWinSet( wk, ANM_SINGLE_OPEN );	//�J��

			wk->wait_count = 0;
			wk->sub_seq++;
		}
		break;

	//�|�P�����\��
	case SEQ_LEAVING_ENTRANCE_POKE_APPEAR_TIMING:
		//�����OBJ�E�B���h�E�̃A�j���V���[���I���܂ő҂�
		if( FactoryObj_AnmActiveCheck(wk->p_obj_win) == TRUE ){
			break;
		}
		Factory_ObjWinDel( wk );											//OBJ�E�B���h�E�폜
		wk->wait_count = 0;													//�N���A
		wk->sub_seq++;
		break;

	//�|�P�����t�F�[�h�C����
	case SEQ_LEAVING_ENTRANCE_POKE_FADEIN:
		if( wk->wait_count == 0 ){
			Factory_SetMainBgScrn( wk, BF_FRAME_BG );
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );		//BG��\��

			//�莝���A�G�ǂ���̃|�P����������
			if( wk->tebanasu_uketoru == 0 ){
				TradePokeInit( wk );
			}else{
				Factory_PokeAdd(wk, 0, 
								PokeParty_GetMemberPointer(wk->p_e_party,0),
								TEST_X, TEST_Y, FLIP_UKETORU );
			}

			SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 0 );					//�\��
			SoftSpritePalFadeSetAll( wk->soft_sprite, 16, 0, 1, 0xffff );		//������t�F�[�h�C��
			ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_FLASH, 1), 21, 11 );
		}

		if( SoftSpritePalFadeExist(wk->ss[0]) ){							//TRUE = �t�F�[�h��
			//������ɂȂ�
		}

		wk->wait_count++;
		if( wk->wait_count < POKE_APPEAR_WAIT ){
			break;
		}
		ScrPalChg( wk, BF_FRAME_BG, Factory_GetScrPalNo(wk, ID_PAL_SINGLE_NORMAL, 1), 21, 11 );

		//BMP�Ƃ��Ďg�p����BG�ʂ��N���A���Ă���
		Factory_FrameWinBmpSet( wk );

		//�y�A�̃f�[�^��\�����Ȃ��t���OOFF
		wk->pair_data_off_flag = 0;
		//�p�[�g�i�[�̑I�񂾃|�P������\�����N�G�X�g
		if( Factory_CommCheck(wk->type) == TRUE ){
			wk->pair_monsno_req = REQ_PAIR_MONSNO_DECIDE;
			Factory_PairMonsNoRecvReq( wk );
		}

		wk->wait_count = 0;
		return TRUE;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	����M
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameSendRecv( FACTORY_WORK* wk )
{
	switch( wk->sub_seq ){

	case 0:
		//�����������t���O�𑗐M
		if( Factory_CommSetSendBuf(wk,FACTORY_COMM_TRADE,wk->trade_flag) == TRUE ){
			wk->sub_seq++;
		}
		break;

	case 1:
		if( wk->trade_recieve_count < FACTORY_COMM_PLAYER_NUM ){
			break;
		}

		//���̌㔲���āApair_trade_flag�����āA����̌������L�����������ŕ���
		
		wk->trade_recieve_count = 0;
		//break;
		return TRUE;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�����}���`�I����
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameTradeEndMulti( FACTORY_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	case 0:
		//�����������t���O�𑗐M
		//if( Factory_CommSetSendBuf(wk,FACTORY_COMM_TRADE,wk->trade_flag) == TRUE ){

			//���b�Z�[�W�\��
			wk->msg_index = Factory_EasyMsg( wk, msg_f_change_07 );
	
			//�����J�n
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_FACTORY_TRADE_END );
		
			wk->sub_seq++;
		//}
		break;

	//�����҂�
	case 1:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_FACTORY_TRADE_END) == TRUE ){
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( FACTORY_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//�t�F�[�h�A�E�g
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
					WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_FACTORY );

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

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�p�[�g�i�[�����������̂ŋ����I����
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GamePairTradeEnd( FACTORY_WORK* wk )
{
	int i;
	POKEMON_PARAM* temp_poke;
	POKEMON_PASO_PARAM* paso_poke;

	switch( wk->sub_seq ){

	case 0:
		//���j���[�J�[�\��OBJ���\������Ă�����
		if( wk->p_csr_menu != NULL ){
			BmpMenuExit( wk->mw, NULL );
			BmpMenuWinClearSub( wk->MenuH.win );
			FactoryCsr_Delete( wk->p_csr_menu );
			wk->p_csr_menu = NULL;
		}

		//�p�[�g�i�[�����Z�b�g
		Frontier_PairNameWordSet( wk->wordset, 0 );

		//��������|�P����(�p�[�g�i�[�̃|�P�����͌㔼�Ɋi�[����Ă���)
		temp_poke = PokeParty_GetMemberPointer( wk->p_m_party, 
												(FACTORY_COMM_POKE_NUM + wk->sel_work[0]) );
		paso_poke = PPPPointerGet( temp_poke );
		Factory_SetPokeName( wk, 1, paso_poke );

		//�󂯎�����|�P����
		temp_poke = PokeParty_GetMemberPointer( wk->p_e_party, wk->sel_work[1] );
		paso_poke = PPPPointerGet( temp_poke );
		Factory_SetPokeName( wk, 2, paso_poke );

#if 0
		wk->msg_index = Factory_EasyMsg( wk, msg_f_change_08 );
#else
		FactoryTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
		wk->msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_TALK], msg_f_change_08, 
										1, 1, 
										CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(wk->sv)),
										//MSG_ALLPUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_TALK] );
#endif

		wk->wait_count = FACTORY_PAIR_TRADE_END_WAIT;
		wk->sub_seq++;
		break;

	case 1:
#if 1
		//3�s���b�Z�[�W�Ȃ̂ŉ�b�I���҂�
		if( GF_MSG_PrintEndCheck(wk->msg_index) == 0 ){
			wk->wait_count = 0;
			return TRUE;
		}
#else
		//�����E�F�C�g������
		wk->wait_count--;
		if( wk->wait_count == 0 ){
			return TRUE;
		}
#endif
		break;
	};

	return FALSE;
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
	FACTORY_WORK* wk = work;

	//�|�P�����X�e�[�^�X�\����
	if( wk->child_proc != NULL ){
		return;
	}
		
	//VBlank�֐��ŌĂяo��
	SoftSpriteTextureTrans( wk->soft_sprite );

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
		//GX_VRAM_OBJ_128_B,				//���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		//���C��2D�G���W����OBJ�g���p���b�g

		GX_VRAM_SUB_OBJ_16_I,			//�T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	//�T�u2D�G���W����OBJ�g���p���b�g

		GX_VRAM_TEX_01_AB,				//�e�N�X�`���C���[�W�X���b�g
		//GX_VRAM_TEX_0_A,				//�e�N�X�`���C���[�W�X���b�g
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
			GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D,
		};
		GF_BGL_InitBG( &BGsys_data );
	}

	{	//BG(�t�H���g)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BF_FRAME_WIN, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BF_FRAME_WIN, 32, 0, HEAPID_FACTORY );
		GF_BGL_ScrClear( ini, BF_FRAME_WIN );
	}

	{	//����(MULTI)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x1800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
			//0, 0, 0, FALSE					//�J�n���o�̂݃t�H���g��(��)����ɂ���悤�ɂ���
		};
		GF_BGL_BGControlSet( ini, BF_FRAME_SLIDE, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BF_FRAME_SLIDE );
	}

	{	//����(SINGLE,DOUBLE)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x2800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BF_FRAME_BG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BF_FRAME_BG );
	}

	//--------------------------------------------------------------------------------
	{	//�����(�{�[��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x3800, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BF_FRAME_SUB, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BF_FRAME_SUB );
	}

	G2_SetBG0Priority( 0 );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_BgInit( FACTORY_WORK* wk )
{
	SetVramBank();
	SetBgHeader( wk->bgl );

	//�p���b�g�t�F�[�h�V�X�e�����[�N�쐬
	wk->pfd = PaletteFadeInit( HEAPID_FACTORY );

	//���N�G�X�g�f�[�^��malloc���ăZ�b�g
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_OBJ, FADE_PAL_ALL_SIZE, HEAPID_FACTORY );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ALL_SIZE, HEAPID_FACTORY );

	Factory_SetMainBgPalette();

	//�J�n���o���I�����Ă��Ȃ���
	if( wk->start_eff_flag == 0 ){
		//���ʔw�i
//TEST
		Factory_SetMainBgGraphic_Maru( wk, BF_FRAME_WIN );				//�E��̉~
		Factory_SetMainBgGraphic_Rail( wk, BF_FRAME_SLIDE );			//���[��
		Factory_SetMainBgGraphic_Back( wk, BF_FRAME_BG );				//�w�i

		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );		//BG�\��
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );		//BG�\��
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );		//BG�\��
	}else{
		//���ʔw�i
		Factory_SetMainBgGraphic( wk, BF_FRAME_BG );					//�S�Ă�������w�i
		Factory_SetMainBgGraphic_Rail( wk, BF_FRAME_SLIDE );			//���[��
		
		//�X�N���[���ʒu�̒���
		GF_BGL_ScrollSet( wk->bgl, BF_FRAME_SLIDE, GF_BGL_SCROLL_X_SET, wk->scr_x );

		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );		//BG�\��
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );		//BG�\��
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );		//BG�\��
	}

	//����ʔw�i�A�p���b�g�Z�b�g
	Factory_SetSubBgGraphic( wk, BF_FRAME_SUB );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�֘A������
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_ObjInit( FACTORY_WORK* wk )
{
	FactoryClact_InitCellActor( &wk->factory_clact );
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
static void Factory_BgExit( GF_BGL_INI * ini )
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
	GF_BGL_BGControlExit( ini, BF_FRAME_BG );
	GF_BGL_BGControlExit( ini, BF_FRAME_SLIDE );
	GF_BGL_BGControlExit( ini, BF_FRAME_WIN );
	GF_BGL_BGControlExit( ini, BF_FRAME_SUB );

	sys_FreeMemoryEz( ini );
	return;
}


//==============================================================================================
//
//	�c�[��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�ύX
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( FACTORY_WORK* wk, int* seq, int next )
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck( FACTORY_WORK* wk )
{
	if( sys.cont & PAD_KEY_UP ){
		if( sys.cont & PAD_BUTTON_SELECT ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_START ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_DOWN ){
		if( sys.cont & PAD_BUTTON_A ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		}else if( sys.cont & PAD_BUTTON_L ){
			SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 0 );		//�\��
		}else if( sys.cont & PAD_BUTTON_R ){
			SoftSpriteParaSet( wk->ss[0], SS_PARA_VANISH, 1 );		//��\��
		}
	}else if( sys.cont & PAD_KEY_LEFT ){
		if( sys.cont & PAD_BUTTON_A ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_RIGHT ){
		if( sys.cont & PAD_BUTTON_A ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_BUTTON_SELECT ){
		if( sys.cont & PAD_BUTTON_A ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG3, VISIBLE_OFF );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���[���ړ�(����)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static BOOL RailMoveIn( FACTORY_WORK* wk )
{
	int i,x,y,end_flag;
	const VecFx32* p_vec;

	end_flag = 0;

	//���[���X�N���[��
	GF_BGL_ScrollSet( wk->bgl, BF_FRAME_SLIDE, GF_BGL_SCROLL_X_INC, RAIL_SPD );
	wk->scr_x = GF_BGL_ScrollGetX( wk->bgl, BF_FRAME_SLIDE );
	//OS_Printf( "scroll_x = %d\n", GF_BGL_ScrollGetX(wk->bgl,BF_FRAME_SLIDE) );

	for( i=0; i < wk->ball_max; i++ ){
		p_vec = FactoryBall_GetPos( wk->p_ball[i] );					//���݂̍��W�擾

		//���̈ʒu���i�݂����Ȃ��悤�ɂ���
		if( ((p_vec->x / FX32_ONE)-RAIL_SPD) <= FactoryBall_GetInitX(wk->p_ball[i]) ){

			x = FactoryBall_GetInitX( wk->p_ball[i] );
			y = FactoryBall_GetInitY( wk->p_ball[i] );
			FactoryBall_SetPos( wk->p_ball[i], x, y );
			end_flag = 1;

		}else{
			FactoryBall_Move( wk->p_ball[i], -RAIL_SPD, 0 );		//��ʉE����C�����Ă���
		}
	}

	MaruMove( wk );													//�~�̈ړ�

	if( end_flag == 1 ){
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	���[���ړ�(�ޏ�)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static BOOL RailMoveOut( FACTORY_WORK* wk )
{
	int i,x,y,end_flag;
	const VecFx32* p_vec;

	end_flag = 0;

	//���[���X�N���[��
	GF_BGL_ScrollSet( wk->bgl, BF_FRAME_SLIDE, GF_BGL_SCROLL_X_INC, RAIL_SPD );
	wk->scr_x = GF_BGL_ScrollGetX( wk->bgl, BF_FRAME_SLIDE );
	//OS_Printf( "scroll_x = %d\n", GF_BGL_ScrollGetX(wk->bgl,BF_FRAME_SLIDE) );

	for( i=0; i < wk->ball_max; i++ ){
		p_vec = FactoryBall_GetPos( wk->p_ball[i] );					//���݂̍��W�擾

		//��ʊO�ɏo�Ă��邩�`�F�b�N
		if( ((p_vec->x / FX32_ONE)-RAIL_SPD) < -24 ){
			FactoryBall_Vanish( wk->p_ball[i], FACTORY_VANISH_ON );		//��\��
			end_flag++;
		}else{
			FactoryBall_Move( wk->p_ball[i], -RAIL_SPD, 0 );			//��ʍ��փA�E�g���Ă���
		}
	}

	MaruMove( wk );														//�~�̈ړ�

	//�S�Ẵ{�[�����ޏꂵ�����`�F�b�N
	if( end_flag == wk->ball_max ){
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�~�̈ړ�
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void MaruMove( FACTORY_WORK* wk )
{
	if( (wk->maru_wait % FACTORY_MARU_WAIT) == 0 ){
		//�p���b�g�{�����`�F�b�N���āA�J�E���^�[���N���A����
		if( (wk->maru_wait / FACTORY_MARU_WAIT) >= FACTORY_MARU_PAL_NUM ){
			wk->maru_wait = 0;
		}

		ScrPalChg(	wk, BF_FRAME_WIN, 
					(wk->maru_wait / FACTORY_MARU_WAIT) + FACTORY_MARU_BASE_PAL, 32, 32 );
	}

	wk->maru_wait++;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p���b�g�؂�ւ�
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScrPalChg( FACTORY_WORK* wk, u32 frm, u8 pltt_no, u8 sx, u8 sy )
{
	OS_Printf( "�؂�ւ���p���b�g�i���o�[ = %d\n", pltt_no );
	GF_BGL_ScrPalChange( wk->bgl, frm, 0, 0, sx, sy, pltt_no );
	GF_BGL_LoadScreenV_Req( wk->bgl, frm );							//�X�N���[���f�[�^�]��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�E�B���h�E�̃Z�b�g
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 * @param	anm_no		�A�j���i���o�[
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_ObjWinSet( FACTORY_WORK* wk, u32 anm_no )
{
	u32 comm_anm_no;

	switch( anm_no ){

	case ANM_SINGLE_OPEN:	//�J��
		Snd_SePlay( SEQ_SE_DP_OPEN2 );
		comm_anm_no = ANM_MULTI_OPEN;
		break;

	case ANM_SINGLE_CLOSE:	//����
		Snd_SePlay( SEQ_SE_DP_CLOSE2 );
		comm_anm_no = ANM_MULTI_CLOSE;
		break;

	case ANM_3POKE_OPEN:	//�J��
		Snd_SePlay( SEQ_SE_DP_OPEN2 );
		comm_anm_no = ANM_MULTI_OPEN;
		break;

	case ANM_3POKE_CLOSE:	//����
		Snd_SePlay( SEQ_SE_DP_CLOSE2 );
		comm_anm_no = ANM_MULTI_CLOSE;
		break;
	};

	wk->p_obj_win = Factory_ObjWinAdd( wk, Factory_GetWinAnmNo(wk,anm_no) );
	if( Factory_CommCheck(wk->type) == TRUE ){
		wk->p_pair_obj_win = Factory_PairObjWinAdd( wk, comm_anm_no );
	}
	return;
}


//==============================================================================================
//
//	BG�O���t�B�b�N�f�[�^
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʔw�i(�V���O���A�_�u��)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgGraphic( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BF_RENTAL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	if( Factory_CommCheck(wk->type) == FALSE ){
		ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_NSCR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );
	}else{
		ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_MULTI_NSCR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʔw�i(�J�n���o��)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgGraphic_Back( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BF_RENTAL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_BACK_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����(�}���`)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgGraphic2( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BF_RENTAL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_WIFIMODE_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����(�����^���|�P�������莞)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgGraphic3( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BF_RENTAL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL2_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����(���[��)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgGraphic_Rail( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BF_RENTAL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_RAIL_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����(�E��̉~)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgGraphic_Maru( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BF_RENTAL_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_MARU_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_FACTORY );

	GF_BGL_PrioritySet( BF_FRAME_WIN, 2 );	//�E��̉~�̃v���C�I���e�B��������
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���ʔw�i�p���b�g�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BF_RENTAL_NCLR, &dat, HEAPID_FACTORY );
		
	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*11) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*11) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i�A�p���b�g�Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetSubBgGraphic( FACTORY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BATT_FRONTIER_NCGR_BIN, wk->bgl, frm,
							0, 0, TRUE, HEAPID_FACTORY );

	ArcUtil_HDL_ScrnSet(wk->hdl, BATT_FRONTIER_NSCR_BIN, wk->bgl, frm,
						0, 0, TRUE, HEAPID_FACTORY );

	ArcUtil_HDL_PalSet(	wk->hdl, BATT_FRONTIER_NCLR, PALTYPE_SUB_BG,
						0, 0x20, HEAPID_FACTORY );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���[���f�[�^�Z�b�g�F���ʔw�i
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMainBgScrn( FACTORY_WORK * wk, u32 frm  )
{
	if( Factory_CommCheck(wk->type) == FALSE ){
		ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_NSCR_BIN, 
							wk->bgl, BF_FRAME_BG, 0, 0, 1, HEAPID_FACTORY );
	}else{
		ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_MULTI_NSCR_BIN, 
							wk->bgl, BF_FRAME_BG, 0, 0, 1, HEAPID_FACTORY );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���[���f�[�^�Z�b�g�F�J�n���o��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetRentalBackBgScrn( FACTORY_WORK * wk, u32 frm )
{
	ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_BACK_NSCR_BIN, 
						wk->bgl, BF_FRAME_BG, 0, 0, 1, HEAPID_FACTORY );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���[���f�[�^�Z�b�g�F2,3�C�|�P��������X�N���[��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_Set3PokeBgScrn( FACTORY_WORK * wk, u32 frm )
{
	if( Factory_CommCheck(wk->type) == FALSE ){
		ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_3POKE_NSCR_BIN, 
							wk->bgl, BF_FRAME_BG, 0, 0, 1, HEAPID_FACTORY );
	}else{
		ArcUtil_HDL_ScrnSet(wk->hdl, BF_RENTAL_MULTI_2POKE_NSCR_BIN, 
							wk->bgl, BF_FRAME_BG, 0, 0, 1, HEAPID_FACTORY );
	}
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
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
static u8 FactoryWriteMsg( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
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
static u8 FactoryWriteMsgSimple( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Factory_EasyMsg( FACTORY_WORK* wk, int msg_id )
{
	u8 msg_index;

	msg_index = FactoryWriteMsg(wk, &wk->bmpwin[BMPWIN_TALK], msg_id, 
								1, 1, MSG_NO_PUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );


	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_TALK] );
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	win		GF_BGL_BMPWIN�^�̃|�C���^
 * @param	y_max	���ڍő吔
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void FactoryInitMenu( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max )
{
	int i;

	for( i=0; i < FACTORY_MENU_BUF_MAX ;i++ ){
		wk->Data[i].str	  = NULL;
		wk->Data[i].param = 0;
	}

	wk->MenuH.menu		= wk->Data;
	wk->MenuH.win		= win;
	wk->MenuH.font		= BF_FONT;
	wk->MenuH.x_max		= 1;
	wk->MenuH.y_max		= y_max;
	wk->MenuH.line_spc	= 0;
	wk->MenuH.c_disp_f	= 1;			//�J�[�\���Ȃ�
	wk->MenuH.loop_f	= 1;			//���[�v�L��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���j���[	�f�[�^�Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	no		�Z�b�g����ꏊ(�߂�l)
 * @param	param	�߂�l
 * @param	msg_id	���b�Z�[�WID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void FactorySetMenuData( FACTORY_WORK* wk, u8 no, u8 param, int msg_id )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( no < FACTORY_MENU_BUF_MAX, "���j���[���ڐ��I�[�o�[�I" );

	MSGMAN_GetString( wk->msgman, msg_id, wk->menu_buf[no] );

	wk->Data[ no ].str = (const void *)wk->menu_buf[no];
	wk->Data[ no ].param = param;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMenu( FACTORY_WORK* wk )
{
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	FactoryInitMenu( wk, &wk->bmpwin[BMPWIN_SEL], POKE_SEL_MENU_MAX );
	FactorySetMenuData( wk, 0, RET_PARAM_TUYOSA, msg_rental_choice_01 );			//�悤�����݂�

	//�����^�����[�h
	if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){

		if( FactoryBall_GetDecideFlag(wk->p_ball[ FactoryCsr_GetCsrPos(wk->p_csr) ]) == 0 ){
			FactorySetMenuData( wk, 1, RET_PARAM_RENTAL, msg_rental_choice_02 );	//�����^������
		}else{
			FactorySetMenuData( wk, 1, RET_PARAM_HAZUSU, msg_rental_choice_04 );	//�͂���
		}

	//�������[�h(�{�[���̐��Ɣz�u�ꏊ�Ȃǂ��Ⴄ)
	}else{
		FactorySetMenuData( wk, 1, RET_PARAM_TEBANASU, msg_f_change_choice_03 );	//�Ă΂Ȃ�
	}

	FactorySetMenuData( wk, 2, RET_PARAM_BETUNO, msg_rental_choice_03 );			//�ׂ̂ɂ���
	wk->mw = BmpMenuAddEx( &wk->MenuH, 0, 0, 0, HEAPID_FACTORY, PAD_BUTTON_B );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g2�u�͂��A�������v
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMenu2( FACTORY_WORK* wk )
{
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	FactoryInitMenu( wk, &wk->bmpwin[BMPWIN_SEL], YES_NO_MENU_MAX );
	FactorySetMenuData( wk, 0, RET_PARAM_YES, msg_rental_04 );						//�͂�
	FactorySetMenuData( wk, 1, RET_PARAM_NO, msg_rental_05 );						//������
	wk->mw = BmpMenuAddEx( &wk->MenuH, 0, 0, 0, HEAPID_FACTORY, PAD_BUTTON_B );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g3�u���ǂ�A���イ���v
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMenu3( FACTORY_WORK* wk )
{
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	FactoryInitMenu( wk, &wk->bmpwin[BMPWIN_SEL], TRADE_FINAL_MENU_MAX );
	FactorySetMenuData( wk, 0, RET_PARAM_MODORU, msg_rental_04 );					//���ǂ�
	FactorySetMenuData( wk, 1, RET_PARAM_TYUUSI, msg_rental_05 );					//���イ��
	wk->mw = BmpMenuAddEx( &wk->MenuH, 0, 0, 0, HEAPID_FACTORY, PAD_BUTTON_B );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g4�u���イ���v
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetMenu4( FACTORY_WORK* wk )
{
	FactoryWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_SEL] );
	FactoryInitMenu( wk, &wk->bmpwin[BMPWIN_SEL], TRADE_FINAL_MENU_MAX );
	FactorySetMenuData( wk, 0, RET_PARAM_MODORU, msg_f_change_choice_01 );			//���イ��
	wk->mw = BmpMenuAddEx( &wk->MenuH, 0, 0, 0, HEAPID_FACTORY, PAD_BUTTON_B );
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * �����Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void Factory_SetNumber( FACTORY_WORK* wk, u32 bufID, s32 number )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, 1, 
							NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P���������Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	ppp		POKEMON_PASO_PARAM
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_SetPokeName( FACTORY_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp )
{
	WORDSET_RegisterPokeMonsName( wk->wordset, bufID, ppp );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�����Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_SetPlayerName( FACTORY_WORK* wk, u32 bufID )
{
	WORDSET_RegisterPlayerName( wk->wordset, bufID, SaveData_GetMyStatus(wk->sv) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[����\��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
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
static void PlayerNameWrite( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	const MYSTATUS* my;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^

	my = SaveData_GetMyStatus( wk->sv );
	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_FACTORY );
	
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	STRBUF_SetStringCode( player_buf, MyStatus_GetMyName(my) );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_NO_PUT, col, NULL );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[����\��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
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
static void PairNameWrite( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^
	MYSTATUS* my;									//�p�[�g�i�[��MyStatus

	//�p�[�g�i�[��MyStatus���擾
	my = CommInfoGetMyStatus( 1 - CommGetCurrentID() );

	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_FACTORY );
	
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	//���O�擾(STRBUF�ɃR�s�[)
	MyStatus_CopyNameString( my, player_buf );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_NO_PUT, col, NULL );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	csr_pos	POKEPARTY�̈ʒu
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
static void PokeNameWrite( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u8 csr_pos, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, const POKEPARTY* party )
{
	u8 x_pos;
	u32 sex,msg_id,col;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	poke = PokeParty_GetMemberPointer( party, csr_pos );
	PokeParaGet( poke, ID_PARA_default_name, sel_poke_buf );	//�|�P�����̃f�t�H���g�����擾

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	buf = STRBUF_Create( POKE_NAME_BUF_SIZE, HEAPID_FACTORY );	//STRBUF����
	STRBUF_SetStringCode( buf, sel_poke_buf );					//STRCODE��STRBUF

	GF_STR_PrintColor( win, font, buf, x, y, MSG_NO_PUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	//�E�[�ɐ��ʃR�[�h��\��
	x_pos = GF_BGL_BmpWinGet_SizeX(win) - 1;
	sex = PokeParaGet( poke, ID_PARA_sex, NULL );

	msg_id = (sex == PARA_MALE) ? msg_rental_male : msg_rental_female;
	col = (sex == PARA_MALE) ? COL_BLUE : COL_RED;

	STRBUF_Clear( buf );

	//���ʂ����鎞
	if( sex != PARA_UNK ){
		MSGMAN_GetString( wk->msgman, msg_id, buf );
		GF_STR_PrintColor( win, font, buf, x_pos*8, y, MSG_NO_PUT, col, NULL );
	}

	STRBUF_Delete( buf );
	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��(���ʎw��)
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 * @param	monsno	�����X�^�[�i���o�[
 * @param	sex		����
 *
 * @return	none
 */
//--------------------------------------------------------------
static void PokeNameWriteEx( FACTORY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex )
{
	u8 x_pos;
	u32 msg_id,col;
	MSGDATA_MANAGER* man;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	//�|�P���������擾
	man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_monsname_dat, HEAPID_FACTORY );
	buf = MSGMAN_AllocString( man, monsno );
	MSGMAN_Delete( man );

	GF_STR_PrintColor( win, font, buf, x, y, MSG_NO_PUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	//�E�[�ɐ��ʃR�[�h��\��
	x_pos = GF_BGL_BmpWinGet_SizeX(win) - 1;

	msg_id = (sex == PARA_MALE) ? msg_rental_male : msg_rental_female;
	col = (sex == PARA_MALE) ? COL_BLUE : COL_RED;

	STRBUF_Clear( buf );

	//���ʂ����鎞
	if( sex != PARA_UNK ){
		MSGMAN_GetString( wk->msgman, msg_id, buf );
		GF_STR_PrintColor( win, font, buf, x_pos*8, y, MSG_NO_PUT, col, NULL );
	}

	STRBUF_Delete( buf );

	//GF_BGL_BmpWinOnVReq( win );
	return;
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void FactoryCommon_Delete( FACTORY_WORK* wk )
{
	int i;

	//�{�[��OBJ�폜
	for( i=0; i < wk->ball_max ;i++ ){
		if( wk->p_ball[i] != NULL ){
			wk->p_ball[i] = FactoryBall_Delete( wk->p_ball[i] );
		}
	}

	//�J�[�\��OBJ�폜
	if( wk->p_csr != NULL ){
		FactoryCsr_Delete( wk->p_csr );
		wk->p_csr = NULL;
	}

	//�ʐM�A�C�R���폜
	WirelessIconEasyEnd();

	//�p���b�g�t�F�[�h�J��
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );

	//�p���b�g�t�F�[�h�V�X�e���J��
	PaletteFadeFree( wk->pfd );
	wk->pfd = NULL;

	FactoryClact_DeleteCellObject(&wk->factory_clact);	//2D�I�u�W�F�N�g�֘A�̈�J��

	//�\�t�g�E�F�A�X�v���C�g�폜
	for( i=0; i < FACTORY_SOFT_SPRITE_MAX; i++ ){
		if( wk->ss[i] != NULL ){
			SoftSpriteDel( wk->ss[i] );
		}
	}

	//�V�X�e���I��
	SoftSpriteEnd( wk->soft_sprite );

	MSGMAN_Delete( wk->msgman );						//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );						//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );						//���b�Z�[�W�o�b�t�@�J��

	for( i=0; i < FACTORY_MENU_BUF_MAX ;i++ ){
		STRBUF_Delete( wk->menu_buf[i] );				//���j���[�o�b�t�@�J��
	}

	FactoryExitBmpWin( wk->bmpwin );					//BMP�E�B���h�E�J��
	Factory_BgExit( wk->bgl );							//BGL�폜
	ArchiveDataHandleClose( wk->hdl );

	GF_G3D_Exit( wk->g3Dman );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���A
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_Recover( FACTORY_WORK* wk )
{
	int i;

	Factory_InitSub1();

	//3D�V�X�e���ݒ�
	wk->g3Dman = GF_G3DMAN_Init(HEAPID_FACTORY, GF_G3DMAN_LNK, GF_G3DTEX_256K, 
								GF_G3DMAN_LNK, GF_G3DPLT_32K, Factory_SimpleSetUp );

	wk->bgl	= GF_BGL_BglIniAlloc( HEAPID_FACTORY );

	Factory_InitSub2( wk );
	FactoryAddBmpWin( wk->bgl, wk->bmpwin );		//�r�b�g�}�b�v�ǉ�
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������1
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_InitSub1( void )
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_InitSub2( FACTORY_WORK* wk )
{
	int i,flip;

	wk->hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_FACTORY );
	Factory_BgInit( wk );								//BG������
	Factory_ObjInit( wk );								//OBJ������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_factory_rental_dat, HEAPID_FACTORY );

	wk->wordset = WORDSET_Create( HEAPID_FACTORY );
	wk->msg_buf = STRBUF_Create( FACTORY_MSG_BUF_SIZE, HEAPID_FACTORY );
	wk->tmp_buf = STRBUF_Create( FACTORY_MSG_BUF_SIZE, HEAPID_FACTORY );

	//������o�b�t�@�쐬
	for( i=0; i < FACTORY_MENU_BUF_MAX ;i++ ){
		wk->menu_buf[i] = STRBUF_Create( FACTORY_MENU_BUF_SIZE, HEAPID_FACTORY );
	}

	//�t�H���g�p���b�g
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, BF_FONT_PAL * 32, HEAPID_FACTORY );
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, BF_MSGFONT_PAL * 32, HEAPID_FACTORY );

	//�\�t�g�E�F�A�X�v���C�g
	wk->soft_sprite = SoftSpriteInit( HEAPID_FACTORY );

	//�ʐM�A�C�R���Z�b�g
	if( CommIsInitialize() ){
//CLACT_U_WmIcon_SetReserveAreaCharManager( NNS_G2D_VRAM_TYPE_2DMAIN,GX_OBJVRAMMODE_CHAR_1D_64K );
		CLACT_U_WmIcon_SetReserveAreaCharManager( NNS_G2D_VRAM_TYPE_2DMAIN,
												  GX_OBJVRAMMODE_CHAR_1D_32K );
		CLACT_U_WmIcon_SetReserveAreaPlttManager( NNS_G2D_VRAM_TYPE_2DMAIN );
		WirelessIconEasy();
	}

	//�{�[���A�|�P�����ǉ�
	if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){	//�����^��
		RentalBallInit( wk );
		RentalPokeInit( wk );
	}else{
		TradeBallInit( wk );
		TradePokeInit( wk );
	}

	GF_Disp_DispOn();

	sys_VBlankFuncChange( VBlankFunc, (void*)wk );		//VBlank�Z�b�g
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� �|�P�����؂�ւ�
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void FactoryCommon_ChangePoke( FACTORY_WORK* wk, u8 index, u8 poke_pos, int flip, const POKEPARTY* party )
{
	int pos = poke_pos;

	//�{�[���̕\������菬�������̏���
	if( FactoryCsr_GetCsrPos(wk->p_csr) < wk->ball_max ){
		SoftSpriteDel( wk->ss[0] );
		Factory_PokeAdd(wk, 0, 
						PokeParty_GetMemberPointer(party,pos),
						TEST_X, TEST_Y, flip );
		PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1+index], 
						pos, 0, 0, 
						//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, party );
						FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, party );
	}

	return;
}


//==============================================================================================
//
//	�|�P�����X�e�[�^�X�Ăяo���֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	psd�̃Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SetPSD( FACTORY_WORK* wk )
{
	wk->psd = sys_AllocMemory( HEAPID_FACTORY, sizeof(PSTATUS_DATA) );
	memset( wk->psd, 0, sizeof(PSTATUS_DATA) );

	wk->psd->ppd	= wk->p_m_party;
	wk->psd->cfg	= wk->config;
	wk->psd->ppt	= PST_PP_TYPE_POKEPARTY;		//�p�����[�^�^�C�v

	//���ƂŊm�F
	//wk->psd->mode = PST_MODE_NORMAL;
	wk->psd->mode = PST_MODE_NO_WAZACHG;

	wk->psd->max	= wk->ball_max;
	wk->psd->pos	= FactoryCsr_GetCsrPos( wk->p_csr );

	//���ƂŊm�F
	wk->psd->waza	= 0;
	wk->psd->zukan_mode = PMNumber_GetMode( wk->sv );
	//wk->psd->ev_contest = PokeStatus_ContestFlagGet( wk->sv );
	wk->psd->ev_contest = 0;	//����ʂ̃^�b�`�{�^���ŃR���e�X�g���ڂ��\������Ȃ��悤�ɂ���

	//[[[[[[�X�e�[�^�X�̃y�[�W�͂���ł悢�̂��H]]]]]]]
	PokeStatus_PageSet( wk->psd, PST_PageTbl_Normal );
	PokeStatus_PlayerSet( wk->psd, SaveData_GetMyStatus(wk->sv) );
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	type	�`�F�b�N����^�C�v
 *
 * @return	"TRUE = type�������AFALSE = type���Ⴄ"
 */
//--------------------------------------------------------------
static BOOL Factory_CheckType( FACTORY_WORK* wk, u8 type )
{
	if( wk->type == type ){
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	���݂�mode�`�F�b�N�֐�
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	mode	�`�F�b�N����MODE
 *
 * @return	"TRUE = type�������AFALSE = type���Ⴄ"
 */
//--------------------------------------------------------------
static BOOL Factory_CheckMode( FACTORY_WORK* wk, u8 mode )
{
	if( wk->mode == mode ){
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	type���Ƃ̃|�P������I�Ԑl�����擾
 *
 * @param	type	�^�C�v
 *
 * @return	"�V���O���A�_�u��		3"
 * @return	"�}���`�Awifi�}���`		2"
 */
//--------------------------------------------------------------
static u8 Factory_GetRentalSelPokeNum( u8 type )
{
	switch( type ){
	case FACTORY_TYPE_SINGLE:
	case FACTORY_TYPE_DOUBLE:
		return 3;
	};

	//case FACTORY_TYPE_MULTI:
	//case FACTORY_TYPE_WIFI_MULTI:
	return FACTORY_COMM_POKE_NUM;	//2
}

//--------------------------------------------------------------
/**
 * @brief	sel_work�̃N���A
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_SelWorkClear( FACTORY_WORK* wk )
{
	int i;

	for( i=0; i < FACTORY_SEL_WORK_MAX ;i++ ){
		wk->sel_work[i] = 0;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�L�����Z���������`�F�b�N
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = �L�����Z�������AFALSE = �L�����Z�����Ă��Ȃ�"
 */
//--------------------------------------------------------------
static BOOL Factory_CheckCancel( FACTORY_WORK* wk )
{
	if( wk->cancel == 0 ){
		return 0;
	}

	return 1;
}

//--------------------------------------------------------------
/**
 * @brief	�L�����Z���̃Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = �L�����Z�������AFALSE = �L�����Z�����Ă��Ȃ�"
 */
//--------------------------------------------------------------
static void Factory_SetCancel( FACTORY_WORK* wk, u8 flag )
{
	wk->cancel = flag;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	ID����A�ʐM�^�C�v�����`�F�b�N���āA�w�i�̕ύX����p���b�gNO�擾
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"�p���b�g�i���o�["
 */
//--------------------------------------------------------------
static u8 Factory_GetScrPalNo( FACTORY_WORK* wk, u8 id, u8 final_answer_flag  )
{
	u8 pltt_no;
	pltt_no = 0;

	switch( id ){

	//�J���Ă鎞
	case ID_PAL_SINGLE_OPEN:
		if( final_answer_flag == 1 ){
			pltt_no = FACTORY_SINGLE_WIN_OPEN_PAL;				//�|�P3�C
		}else if( Factory_CommCheck(wk->type) == FALSE ){
			pltt_no = FACTORY_SINGLE_WIN_OPEN_PAL;				//�V���O��
		}else{
			pltt_no = FACTORY_SINGLE_WIN_OPEN_PAL;				//�V���O��
		}
		break;

	//���鎞
	case ID_PAL_SINGLE_FLASH:
		if( final_answer_flag == 1 ){
			pltt_no = FACTORY_SINGLE_WIN_FLASH_PAL;				//�|�P3�C
		}else if( Factory_CommCheck(wk->type) == FALSE ){
			pltt_no = FACTORY_SINGLE_WIN_FLASH_PAL;				//�V���O��
		}else{
			pltt_no = FACTORY_SINGLE_WIN_FLASH_PAL;				//�V���O��
		}
		break;

	//�ʏ�
	case ID_PAL_SINGLE_NORMAL:
		if( final_answer_flag == 1 ){
			pltt_no = FACTORY_SINGLE_WIN_NORMAL_PAL;			//�|�P3�C
		}else if( Factory_CommCheck(wk->type) == FALSE ){
			pltt_no = FACTORY_SINGLE_WIN_NORMAL_PAL;			//�V���O��
		}else{
			pltt_no = FACTORY_SINGLE_WIN_NORMAL_PAL;			//�V���O��
		}
		break;

	};

	return pltt_no;
};

//--------------------------------------------------------------
/**
 * @brief	�ʐM�^�C�v�����`�F�b�N���āA�A�j���i���o�[���擾
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	"�A�j���i���o�["
 */
//--------------------------------------------------------------
static u32 Factory_GetWinAnmNo( FACTORY_WORK* wk, u32 anm_no )
{
	switch( anm_no ){

	case ANM_SINGLE_OPEN:
		break;

	case ANM_SINGLE_CLOSE:
		break;

	case ANM_3POKE_OPEN:
		if( Factory_CommCheck(wk->type) == TRUE ){
			return ANM_MULTI_2POKE_OPEN;
		}
		break;

	case ANM_3POKE_CLOSE:
		if( Factory_CommCheck(wk->type) == TRUE ){
			return ANM_MULTI_2POKE_CLOSE;
		}
		break;

	};

	return anm_no;
};


//==============================================================================================
//
//	�\�t�g�E�F�A�X�v���C�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�|�P�����\��
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	index	ss[index]	
 * @param	poke	POKEMON_PARAM�|�C���^
 * @param	x		X���W
 * @param	y		Y���W
 * @param	flip	���E���]���邩
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Factory_PokeAdd(FACTORY_WORK* wk, u8 index, POKEMON_PARAM* poke, int x, int y, int flip)
{
	u32 monsno,formno;

	wk->ss[index] = Factory_SoftSpritePokeAdd(	wk->soft_sprite, TEST_POLY_ID, 
												poke,
												x, y, TEST_Z );

	SoftSpriteParaSet( wk->ss[index], SS_PARA_VANISH, 0 );	//�\��

	//�R���e�X�g�Ɠ������]�ł��邩�`�F�b�N���g�p����
	monsno = PokeParaGet( poke, ID_PARA_monsno, NULL );
	formno = PokeParaGet( poke, ID_PARA_form_no, NULL );

	if( PokeFormNoPersonalParaGet(monsno,formno,ID_PER_reverse) == 0 ){		//���]OK
	//if( PokeFlipCheck(monsno) == TRUE ){
		Factory_PokeHFlip( wk->ss[index], flip );			//���E���]
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �t�@�N�g���[�Ŕ��]�����Ȃ��|�P�������`�F�b�N
 *
 * @param   monsno		�|�P�����ԍ�
 *
 * @retval  TRUE�F���]����
 * @retval  FALSE�F���]���Ȃ�
 *
 * src/contest/con_battle.c�̒��g���R�s�[���Ă��܂�
 */
//--------------------------------------------------------------
static BOOL PokeFlipCheck(u32 monsno)
{
	switch(monsno){
	case MONSNO_HITODEMAN:
	case MONSNO_EREBUU:
	case MONSNO_ARIGEITU:
	case MONSNO_ANNOON:
	case MONSNO_NYUURA:
	case MONSNO_ROZERIA:
	case MONSNO_ZANGUUSU:
	case MONSNO_ABUSORU:
	case MONSNO_SUBOMII:
	case MONSNO_ROZUREIDO:
	case MONSNO_BUUBAAN:
	case MONSNO_MORIGAME:
	case MONSNO_TOGEON:
	case MONSNO_KINGURAA:
		return FALSE;
	}
	return TRUE;
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
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL Factory_CommSetSendBuf( FACTORY_WORK* wk, u16 type, u16 param )
{
	int ret,command;

	//�ʐM�^�C�v�łȂ���
	if( Factory_CommCheck(wk->type) == FALSE ){
		return FALSE;
	}

	switch( type ){

	//���O
	case FACTORY_COMM_PAIR:
		command = FC_FACTORY_PAIR;
		Factory_CommSendBufBasicData( wk, type );
		break;

	//�����^����ʂŌ��肵���|�P�����i���o�[�Ɛ���(2�C���肵���瑗��)
	case FACTORY_COMM_PAIR_POKE:
		command = FC_FACTORY_PAIR_POKE;
		Factory_CommSendBufRentalData( wk, type, param );
		break;

	//������ʂŌ�������(���肵���瑗��)
	case FACTORY_COMM_TRADE:
		command = FC_FACTORY_TRADE;
		Factory_CommSendBufTradeFlag( wk, type, param );
		break;
	};

	if( CommSendData(command,wk->send_buf,FACTORY_COMM_BUF_LEN) == TRUE ){
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
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 *
 * �t�@�N�g���[���[�N���m�ی�A�ŏ��ɑ���f�[�^
 */
//--------------------------------------------------------------
void Factory_CommSendBufBasicData( FACTORY_WORK* wk, u16 type )
{
	MYSTATUS* my;
	
	my	= SaveData_GetMyStatus( wk->sv );

	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );
	//wk->send_buf[1]	= ;
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
 *
 * �t�@�N�g���[���[�N���m�ی�A�ŏ��Ɏ󂯎��f�[�^
 */
//--------------------------------------------------------------
void Factory_CommRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FACTORY_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** ��{����M\n" );

	num = 0;
	//wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "id_no = %d\n", id_no );
	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	//type = recv_buf[0];
	
	//wk-> = ;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ����^���őI�񂾃|�P�����i���o�[�Ɛ��ʂ��Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Factory_CommSendBufRentalData( FACTORY_WORK* wk, u16 type, u16 param )
{
	int i,num;
	POKEMON_PARAM* poke;
	
	num = 0;

	//�N���A���Ă���
	for( i=0; i < FACTORY_COMM_BUF_LEN ;i++ ){
		wk->send_buf[i] = 0;
	}

	wk->send_buf[0] = wk->sel_count;
	OS_Printf( "���M�Fsel_count = %d\n", wk->send_buf[0] );
	num += 1;															//1

	//�|�P�����i���o�[
	for( i=0; i < wk->sel_count ;i++ ){
		poke = PokeParty_GetMemberPointer( wk->p_m_party, wk->sel_work[i] );
		wk->send_buf[i+num] = PokeParaGet( poke, ID_PARA_monsno, NULL );
		OS_Printf( "���M�Fmonsno[%d] = %d\n", i, wk->send_buf[i+num] );
	}
	num += wk->sel_count;												//3

	//�|�P��������
	for( i=0; i < wk->sel_count ;i++ ){
		poke = PokeParty_GetMemberPointer( wk->p_m_party, wk->sel_work[i] );
		wk->send_buf[i+num] = PokeParaGet( poke, ID_PARA_sex, NULL );
		OS_Printf( "���M�Fsex[%d] = %d\n", i, wk->send_buf[i+num] );
	}
	num += wk->sel_count;												//5
	
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �����^���őI�񂾃|�P�����i���o�[�Ɛ��� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * �q�@�̃����^���|�P����
 * �g���[�i�[�f�[�^(7�l��)
 *
 * �����^����ʌĂяo���O�ɁA�󂯎��f�[�^
 */
//--------------------------------------------------------------
void Factory_CommRecvBufRentalData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	u8 m_max;
	FACTORY_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** �����^������M\n" );

	num = 0;
	m_max = Factory_GetMinePokeNum( wk->type );
	//wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "id_no = %d\n", id_no );
	OS_Printf( "size = %d\n", size );
	//OS_Printf( "pData[0] = %d\n", ((u8*)pData)[0] );
	OS_Printf( "recv_buf[0] = %d\n", recv_buf[0] );

	wk->pair_sel_count = recv_buf[0];
	OS_Printf( "��M�Fwk->pair_sel_count = %d\n", wk->pair_sel_count );
	num += 1;															//1
	
	//�|�P�����i���o�[
	for( i=0; i < wk->pair_sel_count ;i++ ){
		wk->pair_poke_monsno[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->pair_poke_monsno[%d] = %d\n", i, wk->pair_poke_monsno[i] );
	}
	num += wk->pair_sel_count;											//3

	//�|�P�����̃p���[����
	for( i=0; i < wk->pair_sel_count ;i++ ){
		wk->pair_poke_sex[i] = recv_buf[i+num];
		OS_Printf( "��M�Fwk->pair_poke_sex[%d] = %d\n", i, wk->pair_poke_sex[i] );
	}
	num += wk->pair_sel_count;											//5
	
	//�p�[�g�i�[�̑I�񂾃|�P������\�����N�G�X�g
	wk->pair_monsno_req = REQ_PAIR_MONSNO_DECIDE;

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �����^���L�����Z�� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Factory_CommRecvBufCancelData(int id_no,int size,void *pData,void *work)
{
	OS_Printf( "���g�p\n" );
	GF_ASSERT(0);
	return;
}

//��M�����^�C�~���O���A
//�|�P�����X�e�[�^�X�����ʕ��A���Ă��鎞���ƁA
//BMP���܂���������Ă��Ȃ����߂܂����I
static void Factory_PairMonsNoRecvReq( FACTORY_WORK* wk )
{
	int i;
	u8 m_max;

	m_max = Factory_GetMinePokeNum( wk->type );

	//�y�A�̃f�[�^��\�����Ȃ��t���O
	if( wk->pair_data_off_flag == 1 ){
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TR2] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE4] );
		GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_POKE5] );
		return;
	}

	if( wk->pair_monsno_req == REQ_PAIR_MONSNO_DECIDE ){

		//�p�[�g�i�[�̑I�񂾃|�P����������
		for( i=0; i < m_max ;i++ ){
			GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_POKE4+i], FBMP_COL_NULL );	//�h��Ԃ�

			//�u�悳�������I�񂾎��ɕ\����������̂őΏ�����v
			//�p�[�g�i�[�̑I�񂾃|�P������\��
			if( i < wk->pair_sel_count ){
				PokeNameWriteEx(wk, &wk->bmpwin[BMPWIN_POKE4+i],				//���ʎw��L��
								0, 0, 
								//FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, 
								FBMP_COL_WHITE, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BF_FONT, 
								wk->pair_poke_monsno[i], wk->pair_poke_sex[i] );
			}

			GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_POKE4+i] );
		}

		PairNameWrite(wk, &wk->bmpwin[BMPWIN_TR2], 0, 0, BF_FONT );	//�p�[�g�i�[����\��
	}

	wk->pair_monsno_req = REQ_PAIR_MONSNO_NONE;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɍ������������Z�b�g
 *
 * @param	wk		FACTORY_WORK�^�̃|�C���^
 * @param	sel		0=�������Ă��Ȃ�(�������Ȃ�����)�A1=��������
 *
 * @return	none
 */
//--------------------------------------------------------------
void Factory_CommSendBufTradeFlag( FACTORY_WORK* wk, u16 type, u16 trade_flag )
{
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	wk->send_buf[1] = trade_flag;
	OS_Printf( "���M�Ftrade_flag = %d\n", wk->send_buf[1] );

	wk->send_buf[2] = wk->sel_work[0];
	OS_Printf( "���M�Fsel_work[0] = %d\n", wk->send_buf[2] );

	wk->send_buf[3] = wk->sel_work[1];
	OS_Printf( "���M�Fsel_work[1] = %d\n", wk->send_buf[3] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   ���������� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 *
 * wk->pair_trade_flag
 * 0=�������Ă��Ȃ�
 * 1=��������
 */
//--------------------------------------------------------------
void Factory_CommRecvBufTradeFlag(int id_no,int size,void *pData,void *work)
{
	int i,num;
	FACTORY_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** ��������������M\n" );

	num = 0;
	wk->trade_recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	OS_Printf( "id_no = %d\n", id_no );

	//type = recv_buf[0];
	
	wk->pair_trade_flag = (u8)recv_buf[1];
	OS_Printf( "��M�Fwk->pair_trade_flag = %d\n", wk->pair_trade_flag );

	////////////////////////////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�̌��肪���łɌ��܂��Ă�����A�q�̑I���͖���
		if( wk->trade_flag == 1 ){
			wk->pair_trade_flag = 0;
		}else{

#if PL_T0868_080717_FIX
			//�q�@���I�����Ă�����(�������~�̎��͏㏑�����Ȃ�)
			if( wk->pair_trade_flag == 1 ){
#endif
			wk->sel_work[0] = (u8)recv_buf[2];
			OS_Printf( "��M�Fwk->sel_work[0] = %d\n", wk->sel_work[0] );

			wk->sel_work[1] = (u8)recv_buf[3];
			OS_Printf( "��M�Fwk->sel_work[1] = %d\n", wk->sel_work[1] );
#if PL_T0868_080717_FIX
			}
#endif

			//�e�̌��肪���܂��Ă��Ȃ����́A
			//�e�����M���鎞�Ɂu�q�ɂ���ł�����v�Ƒ��M����
			
			//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g��������
			//wk->parent_decide_pos	= wk->pair_sel_pos + wk->hv_max;
		}
	////////////////////////////////////////////////////////////////////////
	//�q
	}else{
		//�e�̌���^�C�v
		//wk->parent_decide_pos = recv_buf[2];
		
		//�e��������������D�悵�āA�q�̌����������̓N���A����
		if( wk->pair_trade_flag == 1 ){
			wk->trade_flag = 0;

			wk->sel_work[0] = (u8)recv_buf[2];
			OS_Printf( "��M�Fwk->sel_work[0] = %d\n", wk->sel_work[0] );

			wk->sel_work[1] = (u8)recv_buf[3];
			OS_Printf( "��M�Fwk->sel_work[1] = %d\n", wk->sel_work[1] );
		}
	}

	return;
}


//==============================================================================================
//
//	���ʏ���
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�{�[���ǉ�
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 * @param	no			�e�[�u���̉��Ԗڂ�
 *
 * @retval	none
 */
//--------------------------------------------------------------
static FACTORY_BALL* Factory_BallAdd( FACTORY_WORK* wk, int no )
{
	int x,y;

	//�����^��(�ʐM�͊֌W�Ȃ�)
	if( Factory_CheckMode(wk,FACTORY_MODE_RENTAL) == TRUE ){
		x = bf_ball_pos[no][0];										//�����^���̃{�[��
		y = bf_ball_pos[no][1]; 
	}else{

		//����

		//�ʐM�łȂ���
		if( Factory_CommCheck(wk->type) == FALSE ){
			x = bf_t_ball_pos[no][0];								//�Ă΂Ȃ��A�����Ƃ�̃{�[��
			y = bf_t_ball_pos[no][1]; 
		}else{

			//�ʐM�̎�
			if( wk->ball_max == BF_T_MULTI_BALL_TEBANASU_POS_MAX ){
				x = bf_t_multi_ball_tebanasu_pos[no][0];			//�Ă΂Ȃ����̃{�[��
				y = bf_t_multi_ball_tebanasu_pos[no][1];
			}else{
				x = bf_t_multi_ball_uketoru_pos[no][0];				//�����Ƃ鎞�̃{�[��
				y = bf_t_multi_ball_uketoru_pos[no][1];
			}
		}
	}

	return FactoryBall_Create( &wk->factory_clact, x, y, HEAPID_FACTORY );
}

//--------------------------------------------------------------
/**
 * @brief	����̃E�B���h�E�\��
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 * @param	no			�e�[�u���̉��Ԗڂ�
 *
 * @retval	none
 */
//--------------------------------------------------------------
static FACTORY_OBJ* Factory_ObjWinAdd( FACTORY_WORK* wk, u32 anm_no )
{
	int x, y;

	switch( anm_no ){

	//�V���O���F�J���A����
	case ANM_SINGLE_OPEN:
	case ANM_SINGLE_CLOSE:
		x = FACTORY_OBJ_SINGLE_WIN_X;
		y = FACTORY_OBJ_SINGLE_WIN_Y;
		break;

	//�V���O���F(����|�P����3�C)�J���A����
	case ANM_3POKE_OPEN:
	case ANM_3POKE_CLOSE:
		x = FACTORY_OBJ_3POKE_WIN_X;
		y = FACTORY_OBJ_3POKE_WIN_Y;
		break;

	//�}���`�F(����|�P����3�C)�J���A����
	case ANM_MULTI_2POKE_OPEN:
		x = FACTORY_OBJ_2POKE_WIN_X_OPEN;
		y = FACTORY_OBJ_2POKE_WIN_Y;
		break;

	case ANM_MULTI_2POKE_CLOSE:
		x = FACTORY_OBJ_2POKE_WIN_X_CLOSE;
		y = FACTORY_OBJ_2POKE_WIN_Y;
		break;
	};

	return FactoryObj_Create( &wk->factory_clact, anm_no, x, y, HEAPID_FACTORY );
}

//--------------------------------------------------------------
/**
 * @brief	�ʐM��p�F�E��̃E�B���h�E�\��
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 * @param	no			�e�[�u���̉��Ԗڂ�
 *
 * @retval	none
 */
//--------------------------------------------------------------
static FACTORY_OBJ* Factory_PairObjWinAdd( FACTORY_WORK* wk, u32 anm_no )
{
	int x, y;

	switch( anm_no ){

	//�}���`�F�J���A����
	case ANM_MULTI_OPEN:
	case ANM_MULTI_CLOSE:
		x = FACTORY_OBJ_MULTI_WIN_X;
		y = FACTORY_OBJ_MULTI_WIN_Y;
		break;

	default:
		OS_Printf( "�s���ȃA�j���i���o�[���n����܂����I\n" );
		GF_ASSERT(0);
		x = 0;
		y = 0;
		break;
	};

	return FactoryObj_Create( &wk->factory_clact, anm_no, x, y, HEAPID_FACTORY );
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�E�B���h�E�̍폜
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_ObjWinDel( FACTORY_WORK* wk )
{
	if( wk->p_obj_win != NULL ){
		FactoryObj_Delete( wk->p_obj_win );								//OBJ�E�B���h�E�폜
		wk->p_obj_win = NULL;
	}

	if( wk->p_pair_obj_win != NULL ){
		FactoryObj_Delete( wk->p_pair_obj_win );						//OBJ�E�B���h�E�폜
		wk->p_pair_obj_win = NULL;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BF_FRAME_WIN��BMP�ɐݒ�
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Factory_FrameWinBmpSet( FACTORY_WORK* wk )
{
	GF_BGL_PrioritySet( BF_FRAME_WIN, 1 );	//�E��̉~�̃v���C�I���e�B���グ��
	GF_BGL_ClearCharSet( BF_FRAME_WIN, 32, 0, HEAPID_FACTORY );
	GF_BGL_ScrClear( wk->bgl, BF_FRAME_WIN );
	FactoryAddBmpWin( wk->bgl, wk->bmpwin );							//�r�b�g�}�b�v�ǉ�
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );			//BG�\��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BF_FRAME_WIN��BMP�ɐݒ�
 *
 * @param	wk			FACTORY_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpMenuWinClearSub( GF_BGL_BMPWIN* win )
{
	BmpMenuWinClear( win, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( win );
	return;
}


