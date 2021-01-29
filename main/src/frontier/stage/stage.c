//==============================================================================================
/**
 * @file	stage.c
 * @brief	�u�o�g���X�e�[�W�v���C���\�[�X
 * @author	Satoshi Nohara
 * @date	07.06.08
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
#include "savedata/config.h"
#include "application/p_status.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_number.h"
#include "poketool/pokeicon.h"
#include "savedata/b_tower.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"
#include "communication/comm_info.h"
#include "communication/wm_icon.h"

#include "stage_sys.h"
#include "stage_clact.h"
#include "stage_bmp.h"
#include "stage_obj.h"
#include "application/stage.h"
#include "../frontier_tool.h"					//Frontier_PokeParaMake
#include "../frontier_def.h"
#include "../stage_tool.h"						//STAGE_POKE_RANGE
#include "../stage_def.h"
#include "../comm_command_frontier.h"

#include "../../field/comm_direct_counter_def.h"//

#include "msgdata\msg.naix"						//NARC_msg_??_dat
#include "msgdata\msg_stage_room.h"				//msg_??

#include "../graphic/frontier_obj_def.h"
#include "../graphic/frontier_bg_def.h"


#include "system/pm_overlay.h"
FS_EXTERN_OVERLAY(frontier_common);


//==============================================================================================
//
//	�f�o�b�N�p
//
//==============================================================================================
#ifdef PM_DEBUG
//PROC_DATA* p_proc;
//#define DEBUG_BRAIN_SET							//�L���ŁA�u���[���o��ɂ���
#endif


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�V�[�P���X��`
enum {
	SEQ_GAME_INIT,											//������
	SEQ_GAME_PSD,											//�|�P�����X�e�[�^�X����

	SEQ_GAME_TYPE_SEL,										//�^�C�v�I��

	SEQ_GAME_SEND_RECV,										//����M

	SEQ_GAME_END_MULTI,										//(�ʐM)�I��
	SEQ_GAME_END,											//�I��

	SEQ_GAME_PAIR_END,										//�p�[�g�i�[�����肵���̂ŋ����I��
};

//�t�H���g�J���[
#define	COL_BLUE	( GF_PRINTCOLOR_MAKE(FBMP_COL_BLUE,FBMP_COL_BLU_SDW,FBMP_COL_NULL) )//��
#define	COL_RED		( GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_NULL) )	//��

//�^�C�v�\���ʒu
#define TYPE_START_X	(1)				//�J�n�w
#define TYPE_START_Y	(16)			//�J�n�x
#define TYPE_X			(64)			//�Ԋu�w
#define TYPE_Y			(36)			//�Ԋu�x

//�����N�\���ʒu
//#define RANK_START_X	(12)			//�J�n�w
//#define RANK_START_X	(14)			//�J�n�w
#define RANK_START_X	(18)			//�J�n�w
#define RANK_START_Y	(4)				//�J�n�x
#define RANK_X			(64)			//�Ԋu�w
#define RANK_Y			(36)			//�Ԋu�x

//�J�[�\���\���ʒu
#define CSR_START_X		(32)//(16)		//�J�n�w
#define CSR_START_Y		(16)			//�J�n�x
#define CSR_X			(64)			//�Ԋu�w
#define CSR_Y			(36)			//�Ԋu�x

//�|�P�������̕\���ʒu
#define POKE_NAME_PX	(10*8)
#define POKE_NAME_PY	(21*8)
#define POKE_NAME_SX	(9*8)
#define POKE_NAME_SY	(2*8)

//�I���������̃J���[
#define DECIDE_BG_PL_DAT	( 0x3f00 )

enum{
	NORMAL_PAL_CHG = 0,
	DECIDE_PAL_CHG,
	PAIR_PAL_CHG,
	MAX_PAL_CHG,
};

#define NORMAL_PAL		(0)				//�ʏ�p���b�g�ԍ�
#define NORMAL_PAL2		(1)				//�ʏ�p���b�g�ԍ�(2�{�g�p���Ă���)(�O���̃^�C�v)
#define NORMAL_PAL3		(2)				//�ʏ�p���b�g�ԍ�(2�{�g�p���Ă���)(�㔼�̃^�C�v)
#define DECIDE_PAL		(5)				//�v���C���[����p���b�g�ԍ�
#define PAIR_PAL		(4)				//�p�[�g�i�[����p���b�g�ԍ�
#define MAX_PAL			(3)				//���x���ō��p���b�g�ԍ�

//�I�񂾃|�P�����^�C�v�ɑ΂��Ẳ�
enum{
	FINAL_ANSWER_NONE = 0,				//�����Ȃ�
	FINAL_ANSWER_YES,					//�I�񂾃|�P�����^�C�v�Ői��
	FINAL_ANSWER_NO,					//����ς�I�ђ���
};

#define CSR_POS_NONE	(0xff)			//�J�[�\�������I��ł��Ȃ�

//�p�[�g�i�[���I���������Ƃ�m�点�郁�b�Z�[�W�E�F�C�g
//#define STAGE_PAIR_END_WAIT		(30)

//�A�C�R���̈ʒu
#define ICON_X			(160)
#define ICON_Y			(160)//(168)

//�J�[�\�����|�P������I�����Ă��鎞�̈ʒu
#define CSR_POKE_X		(128)
#define CSR_POKE_Y		(168)//(169)

//�I�����ă��j���[���o��܂ł̃E�F�C�g
#define STAGE_MENU_WAIT	(8)//(15)

#define STAGE_DECIDE_NONE	(0xff)

#define STAGE_END_WAIT		(10)				//�I������܂ł̕\���E�F�C�g
#define STAGE_MSG_WAIT		(10)				//�F�B���I��ł���̕\���E�F�C�g

#define STAGE_BRAIN_CSR_POS_NONE	(0x75)		//�u���[����I�ԑO�̃J�[�\���ʒu�̏����l


//==============================================================================================
//
//	���b�Z�[�W�֘A�̒�`
//
//==============================================================================================
#define STAGE_MENU_BUF_MAX		(2)						//���j���[�o�b�t�@�̍ő吔
#define BS_FONT					(FONT_SYSTEM)			//�t�H���g���
#define STAGE_MSG_BUF_SIZE		(600)//(800)//(1024)			//��b�̃��b�Z�[�Wsize
#define STAGE_MENU_BUF_SIZE		(32)					//���j���[�̃��b�Z�[�Wsize
#define PLAYER_NAME_BUF_SIZE	(PERSON_NAME_SIZE + EOM_SIZE)	//�v���C���[���̃��b�Z�[�Wsize
#define POKE_NAME_BUF_SIZE		(MONS_NAME_SIZE + EOM_SIZE)		//�|�P�������̃��b�Z�[�Wsize


//==============================================================================================
//
//	(�t�@�N�g���[)�ʏ�̃X�e�[�^�X��ʃy�[�W
//
//==============================================================================================
static const u8 PST_PageTbl_Normal[] = {
	PST_PAGE_INFO,			// �u�|�P�������傤�ق��v
	PST_PAGE_MEMO,			// �u�g���[�i�[�����v
	PST_PAGE_PARAM,			// �u�|�P�����̂���傭�v
	PST_PAGE_CONDITION,		// �u�R���f�B�V�����v
	PST_PAGE_B_SKILL,		// �u���������킴�v
	PST_PAGE_C_SKILL,		// �u�R���e�X�g�킴�v
	PST_PAGE_RIBBON,		// �u���˂񃊃{���v
	PST_PAGE_RET,			// �u���ǂ�v
	PST_PAGE_MAX
};


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
struct _STAGE_WORK{

	PROC* proc;										//PROC�ւ̃|�C���^
	PROC* child_proc;								//PROC�ւ̃|�C���^

	u8	sub_seq;									//�V�[�P���X
	u8	type;										//�����Ƃ��ēn���ꂽ�o�g���^�C�v
	u8	msg_index;									//���b�Z�[�Windex
	u8	recover;									//���A�t���O

	u8	tmp_csr_pos;								//�ޔ����Ă���J�[�\���ʒu
	u8	csr_pos;									//���݂̃J�[�\���ʒu
	u8	master_flag;								//�S�ă��x��10
	u8	menu_flag;									//���j���[�\�������t���O

	u16	psd_flag;									//�|�P�����X�e�[�^�X���t���O
	u16 init_flag;									//����t���O

	u16	counter;
	u8	typesel_recieve_count;						//��M�J�E���g
	u8	send_req;									//�^�C�v����̑��M���N�G�X�g

	u8	parent_decide_pos;
	u8	type_sel_return_flag;						//TYPE_SEL�V�[�P���X�ɖ߂�t���O
	u8	decide_csr_pos;								//���肵���J�[�\���ʒu
	u8	wait;

	u16 btl_count;									//������ڂ�
	u8	brain_appear;								//�u���[���o�ꂩ�t���O
	u8	brain_before_csr_pos;						//�u���[����I�ԑO�̃J�[�\���ʒu

	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g
	STRBUF* msg_buf;								//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;								//�e���|�����o�b�t�@�|�C���^

	STRBUF* menu_buf[STAGE_MENU_BUF_MAX];			//���j���[�o�b�t�@�|�C���^
	STRCODE str[PERSON_NAME_SIZE + EOM_SIZE];		//���j���[�̃��b�Z�[�W

	GF_BGL_INI*	bgl;								//BGL�ւ̃|�C���^
	GF_BGL_BMPWIN bmpwin[STAGE_BMPWIN_MAX];			//BMP�E�B���h�E�f�[�^

	//BMP���j���[(bmp_menu.h)
	BMPMENU_HEADER MenuH;							//BMP���j���[�w�b�_�[
	BMPMENU_WORK* mw;								//BMP���j���[���[�N
	BMPMENU_DATA Data[STAGE_MENU_BUF_MAX];			//BMP���j���[�f�[�^

	PALETTE_FADE_PTR pfd;							//�p���b�g�t�F�[�h

	NUMFONT* num_font;								//8x8�t�H���g

	//const CONFIG* config;							//�R���t�B�O�|�C���^
	CONFIG* config;									//�R���t�B�O�|�C���^
	SAVEDATA* sv;									//�Z�[�u�f�[�^�|�C���^

	PSTATUS_DATA* psd;								//�|�P�����X�e�[�^�X

	STAGE_CLACT stage_clact;						//�Z���A�N�^�f�[�^
	STAGE_OBJ* p_csr;								//�J�[�\��OBJ�̃|�C���^�i�[�e�[�u��
	STAGE_OBJ* p_icon;								//�A�C�R��OBJ�̃|�C���^�i�[�e�[�u��

	int psd_pos;									//�|�P�����X�e�[�^�X�̑I�����ʎ擾

	u16* p_ret_work;								//STAGE_CALL_WORK�̖߂�l���[�N�ւ̃|�C���^

	//�o�g���X�e�[�W�p
	POKEPARTY* p_party;
	u8* p_rank;										//�����N�f�[�^�̃|�C���^(0-9,10�œn�����)
	u16* p_pair_poke_level;							//����̃|�P�����̃��x��

	ARCHANDLE* hdl;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//STAGE_COMM stage_comm;
#if 1
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[STAGE_COMM_BUF_LEN];

	//�ʐM�p
	u8	pair_csr_pos;								//�p�[�g�i�[���I�񂾃|�P�����^�C�v(�J�[�\���ʒu)
	u8	pair_final_answer;							//0=�����Ȃ��A1=�ŏI����A2=�L�����Z��
	u8	pair_dummy;
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
PROC_RESULT StageProc_Init( PROC * proc, int * seq );
PROC_RESULT StageProc_Main( PROC * proc, int * seq );
PROC_RESULT StageProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameInit( STAGE_WORK* wk );
static BOOL Seq_GamePsd( STAGE_WORK* wk );
static void TypeSelInit( STAGE_WORK* wk );
static BOOL Seq_GameTypeSel( STAGE_WORK* wk );
static BOOL Seq_GameSendRecv( STAGE_WORK* wk );
static BOOL Seq_GameEndMulti( STAGE_WORK* wk );
static BOOL Seq_GameEnd( STAGE_WORK* wk );
static BOOL Seq_GamePairEnd( STAGE_WORK* wk );

//���ʏ���
static void StageCommon_Delete( STAGE_WORK* wk );
static void Stage_Recover( STAGE_WORK* wk );
static void Stage_InitSub1( void );
static void Stage_InitSub2( STAGE_WORK* wk );

//���ʏ������A�I��
static void Stage_ObjInit( STAGE_WORK* wk );
static void Stage_BgInit( STAGE_WORK* wk );
static void Stage_BgExit( GF_BGL_INI * ini );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//BG�O���t�B�b�N�f�[�^
static void Stage_SetMainBgGraphic( STAGE_WORK * wk, u32 frm  );
static void Stage_SetMainBgPalette( void );
static void Stage_SetSubBgGraphic( STAGE_WORK * wk, u32 frm  );

//���b�Z�[�W
static u8 StageWriteMsg( STAGE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 StageWriteMsgSimple( STAGE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 Stage_EasyMsg( STAGE_WORK* wk, int msg_id, u8 font );

//���j���[
static void StageInitMenu( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max );
static void StageSetMenuData( STAGE_WORK* wk, u8 no, u8 param, int msg_id );
static void Stage_SetMenu2( STAGE_WORK* wk );

//������
static void Stage_SetNumber( STAGE_WORK* wk, u32 bufID, s32 number );
static void Stage_SetPokeName( STAGE_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp );
static void Stage_SetPlayerName( STAGE_WORK* wk, u32 bufID );
static void PlayerNameWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PairNameWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PokeNameWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font );
static void PokeNameWriteEx( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex );
static void TypeWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u8 f_col, u8 s_col, u8 b_col, u8 font );
static void RankWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u8 num, u32 x, u32 y );
static void RankWriteAll( STAGE_WORK* wk, GF_BGL_BMPWIN* win );

//�|�P�����X�e�[�^�X
static void Stage_SetPSD( STAGE_WORK* wk );

//�c�[��
static BOOL Stage_CheckType( STAGE_WORK* wk, u8 type );
static void NextSeq( STAGE_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( STAGE_WORK* wk );
static void CsrMove( STAGE_WORK* wk, int key );
static u16 GetCsrX( STAGE_WORK* wk );
static u16 GetCsrY( STAGE_WORK* wk );
static void ScrnPalChg( GF_BGL_INI* ini, u8 csr_pos, u8 flag );
static void GameStartTypeScrnPalChg( STAGE_WORK* wk, GF_BGL_INI* ini );
static void StageMasterFlagSet( STAGE_WORK* wk );
static u8 Stage_GetCsrPos( u8 csr_pos );
static BOOL Stage_BtlCountLeaderCheck( STAGE_WORK* wk );
static BOOL Stage_BrainAppearCheck( STAGE_WORK* wk );

//�ʐM
BOOL Stage_CommSetSendBuf( STAGE_WORK* wk, u16 type, u16 param );
//BOOL Stage_CommWaitRecvBuf( STAGE_WORK* wk, u16 type );
void Stage_CommSendBufBasicData( STAGE_WORK* wk, u16 type );
void Stage_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
void Stage_CommSendBufPokeTypeData( STAGE_WORK* wk, u16 type, u16 param );
void Stage_CommRecvBufPokeTypeData(int id_no,int size,void *pData,void *work);
void Stage_CommSendBufFinalAnswerData( STAGE_WORK* wk, u16 type, u16 param );
void Stage_CommRecvBufFinalAnswerData(int id_no,int size,void *pData,void *work);

//�T�u�V�[�P���X
static void Stage_SeqSubMenuWinClear( STAGE_WORK* wk );
static void Stage_SeqSubTypeSelYesNo( STAGE_WORK* wk );
static void Stage_SeqSubTypeSelNo( STAGE_WORK* wk );
static void Stage_SeqSubWinRet( STAGE_WORK* wk );


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
PROC_RESULT StageProc_Init( PROC * proc, int * seq )
{
	int i;
	STAGE_WORK* wk;
	STAGE_CALL_WORK* stage_call;

	Overlay_Load( FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE );

	Stage_InitSub1();

	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_STAGE, 0x20000 );

	wk = PROC_AllocWork( proc, sizeof(STAGE_WORK), HEAPID_STAGE );
	memset( wk, 0, sizeof(STAGE_WORK) );

	wk->bgl				= GF_BGL_BglIniAlloc( HEAPID_STAGE );
	wk->proc			= proc;
	stage_call			= (STAGE_CALL_WORK*)PROC_GetParentWork( proc );
	wk->sv				= stage_call->sv;
	wk->type			= stage_call->type;
	wk->p_ret_work		= &stage_call->ret_work;
	wk->config			= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾
	wk->p_party			= stage_call->p_party;
	wk->p_rank			= stage_call->p_rank;					//0�I���W��
	wk->p_pair_poke_level = stage_call->p_pair_poke_level;		//
	wk->btl_count		= (stage_call->btl_count + 1);			//������ڂ�
	wk->csr_pos			= stage_call->csr_pos;					//�����J�[�\���ʒu

	wk->pair_csr_pos	= CSR_POS_NONE;

	wk->parent_decide_pos = STAGE_DECIDE_NONE;

	wk->brain_before_csr_pos = STAGE_BRAIN_CSR_POS_NONE;		//�u���[����I�ԑO�̃J�[�\���ʒu

	Stage_InitSub2( wk );

	if( Stage_CommCheck(wk->type) == TRUE ){
		CommCommandFrontierInitialize( wk );
	}

	(*seq) = SEQ_GAME_INIT;
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
PROC_RESULT StageProc_Main( PROC * proc, int * seq )
{
	STAGE_WORK* wk  = PROC_GetWork( proc );
	//OS_Printf( "(*seq) = %d\n", (*seq) );

#ifdef DEBUG_FRONTIER_LOOP
	sys.cont |= PAD_BUTTON_A;
	sys.trg |= PAD_BUTTON_A;
#endif	//DEBUG_FRONTIER_LOOP

	if( wk->parent_decide_pos != STAGE_DECIDE_NONE ){
		switch( *seq ){

		//����M�֋����ړ�
		case SEQ_GAME_TYPE_SEL:

			//�X�e�[�^�X�ɐ؂�ւ��鎞�ȊO
			if( wk->psd_flag == 0 ){
				Stage_SeqSubMenuWinClear( wk );
				//BmpTalkWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );
				NextSeq( wk, seq, SEQ_GAME_SEND_RECV );
			}
			break;

		//�������Ȃ�
		//case SEQ_GAME_INIT:
		//case SEQ_GAME_PSD:					//�|�P�����X�e�[�^�X����
		//case SEQ_GAME_SEND_RECV:
		//case SEQ_GAME_END_MULTI:
		//case SEQ_GAME_END:
		//case SEQ_GAME_PAIR_END:
		//	break;
		};
	}

	switch( *seq ){

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
			NextSeq( wk, seq, SEQ_GAME_TYPE_SEL );
		}
		break;

	//-----------------------------------
	//�^�C�v�I��
	case SEQ_GAME_TYPE_SEL:
		if( Seq_GameTypeSel(wk) == TRUE ){

			if( wk->send_req == 1 ){
				NextSeq( wk, seq, SEQ_GAME_SEND_RECV );					//����M��
			}else{

				if( wk->recover == 1 ){
					NextSeq( wk, seq, SEQ_GAME_PSD );					//�|�P�����X�e�[�^�X
					return PROC_RES_CONTINUE;	//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
				}else{

					//�ʐM�^�C�v�̎�
					if( Stage_CommCheck(wk->type) == TRUE ){
						NextSeq( wk, seq, SEQ_GAME_END_MULTI );			//(�ʐM)�I����
					}else{
						NextSeq( wk, seq, SEQ_GAME_END );				//�I����
					}
				}
			}
		}
		break;

	//-----------------------------------
	//����M
	case SEQ_GAME_SEND_RECV:
		if( Seq_GameSendRecv(wk) == TRUE ){

			if( wk->type_sel_return_flag == 1 ){
				wk->type_sel_return_flag = 0;
				NextSeq( wk, seq, SEQ_GAME_TYPE_SEL );						//�^�C�v�I�𒆂�
			}else{

				//�ʐM�^�C�v�̎�
				if( Stage_CommCheck(wk->type) == TRUE ){
					NextSeq( wk, seq, SEQ_GAME_END_MULTI );					//(�ʐM)�I����
				}else{
					NextSeq( wk, seq, SEQ_GAME_END );						//�I����
				}
			}
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

	//-----------------------------------
	//�p�[�g�i�[���|�P�����^�C�v�����肵���̂ŋ����I����
	case SEQ_GAME_PAIR_END:
		if( Seq_GamePairEnd(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_END_MULTI );						//(�ʐM)�I����
		}
		break;
	}

	CLACT_Draw( wk->stage_clact.ClactSet );		//�Z���A�N�^�[�풓�֐�

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
PROC_RESULT StageProc_End( PROC * proc, int * seq )
{
	int i;
	STAGE_WORK* wk = PROC_GetWork( proc );

	*(wk->p_ret_work) = wk->csr_pos;					//�߂�l�i�[���[�N�֑��(�J�[�\���ʒu)

	OS_Printf( "*(wk->p_ret_work) = %d\n", *(wk->p_ret_work) );

	StageCommon_Delete( wk );							//�폜����

	PROC_FreeWork( proc );								//���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_DeleteHeap( HEAPID_STAGE );

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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( STAGE_WORK* wk )
{
	switch( wk->sub_seq ){

	//�R�}���h�ݒ��̓���
	case 0:
		if( (wk->init_flag == 0) && (Stage_CommCheck(wk->type) == TRUE) ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_STAGE_TYPE_INIT );
		}
		wk->sub_seq++;
		break;

	//�^�C�~���O�R�}���h���͂������m�F
	case 1:
		if( (wk->init_flag == 0) && (Stage_CommCheck(wk->type) == TRUE) ){
			if( CommIsTimingSync(DBC_TIM_STAGE_TYPE_INIT) == TRUE ){
				CommToolTempDataReset();
				wk->init_flag = 1;
				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;

	//�u���b�N�C��
	case 2:
		TypeSelInit( wk );

		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_STAGE );

		wk->sub_seq++;
		break;
		
	//�t�F�[�h�I���҂�
	case 3:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static void TypeSelInit( STAGE_WORK* wk )
{
	int i;

	//�^�C�v�\��
	TypeWrite(	wk, &wk->bmpwin[BMPWIN_TYPE], FBMP_COL_BLACK, FBMP_COL_BLK_SDW, 
				FBMP_COL_NULL, BS_FONT );

	//�����N�\��
	RankWriteAll( wk, &wk->bmpwin[BMPWIN_TYPE] );

	//�|�P��������\��
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1], 0, 0,
					FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BS_FONT );

	//��ʕ��A��
	if( wk->recover == 1 ){
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�|�P�����X�e�[�^�X����
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GamePsd( STAGE_WORK* wk )
{
	switch( wk->sub_seq ){

	case 0:
		SDK_ASSERTMSG( wk->child_proc != NULL, "child_proc��NULL�ł��I" );

		if( ProcMain(wk->child_proc) == TRUE ){
			wk->psd_pos = wk->psd->pos;									//���ʎ擾
			OS_Printf( "wk->psd_pos = %d\n", wk->psd_pos );
			sys_FreeMemoryEz( wk->psd );
			sys_FreeMemoryEz( wk->child_proc );
			wk->child_proc = NULL;
			Stage_Recover( wk );
			wk->psd_flag = 0;
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���^�C�v�I��
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
enum{
	SEQ_SUB_START = 0,
	SEQ_SUB_TYPE_SEL,
	SEQ_SUB_MENU_WAIT,
	SEQ_SUB_MENU_MAIN,
	SEQ_SUB_BRAIN_MSG,
	SEQ_SUB_BRAIN_MSG_WAIT,
	SEQ_SUB_PSD_CALL,
};

static BOOL Seq_GameTypeSel( STAGE_WORK* wk )
{
	int i;
	u32 ret,color;

	switch( wk->sub_seq ){

	case SEQ_SUB_START:
		wk->recover = 0;
		wk->sub_seq = SEQ_SUB_TYPE_SEL;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�^�C�v��I��
	case SEQ_SUB_TYPE_SEL:
		CsrMove( wk, sys.trg );

		//����{�^������������
		if( sys.trg & PAD_BUTTON_A ){

#if 0
			//�S�Ẵ����N��10�ł͂Ȃ���
			if( wk->master_flag == 0 ){
				//�����N���}�b�N�X��������I���o���Ȃ�(0-9,10)
				if( Stage_GetTypeLevel(Stage_GetCsrPos(wk->csr_pos),wk->p_rank) >= 
																		STAGE_TYPE_LEVEL_MAX ){

					Snd_SePlay( SEQ_SE_DP_BOX03 );
					return FALSE;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
				}
			}
#endif

			//�_�~�[�^�C�v�̎�
			if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
				Snd_SePlay( SEQ_SE_DP_DECIDE );
				WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
								WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_STAGE );

				wk->psd_flag = 1;
				wk->sub_seq = SEQ_SUB_PSD_CALL;					//�|�P�����X�e�[�^�X��
			}else{

				//�u���[�����o�ꂷ��퓬�񐔂�������
				//if( Stage_BtlCountLeaderCheck(wk->btl_count) == TRUE ){
				if( Stage_BrainAppearCheck(wk) == TRUE ){

					//�u???�v�ȊO�͑I�ׂȂ�
					if( Stage_GetPokeType(wk->csr_pos) != HATE_TYPE ){
						Snd_SePlay( SEQ_SE_DP_BOX03 );
						return FALSE;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
					}
				}else{

					//�����N���}�b�N�X��������I���o���Ȃ�(0-9,10)
					if( Stage_GetTypeLevel(Stage_GetCsrPos(wk->csr_pos),wk->p_rank) >= 
																		STAGE_TYPE_LEVEL_MAX ){
						Snd_SePlay( SEQ_SE_DP_BOX03 );

#ifdef DEBUG_FRONTIER_LOOP
						wk->csr_pos++;
						StageObj_SetObjPos( wk->p_csr, GetCsrX(wk), GetCsrY(wk) );
#endif	//DEBUG_FRONTIER_LOOP

						return FALSE;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
					}

					//�ʏ�́u???�v�͑I�ׂȂ�
					if( Stage_GetPokeType(wk->csr_pos) == HATE_TYPE ){
						Snd_SePlay( SEQ_SE_DP_BOX03 );
						return FALSE;					//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
					}
				}

				Snd_SePlay( SEQ_SE_DP_DECIDE );

				//�^�C�v����b�E�B���h�E�ŉB����Ă��܂����Ƃ�����̂ŁA
				//��ɑI�������^�C�v��F�ւ����ăE�F�C�g������
				if( Stage_CommCheck(wk->type) == FALSE ){
					//�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX����
					ScrnPalChg( wk->bgl, wk->csr_pos, DECIDE_PAL_CHG );
					GF_BGL_LoadScreenV_Req( wk->bgl, BS_FRAME_BG );	//�X�N���[���f�[�^�]��
				}

				wk->counter = STAGE_MENU_WAIT;
				wk->sub_seq = SEQ_SUB_MENU_WAIT;				//���j���[�E�F�C�g��
			}
		}

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���j���[�E�F�C�g
	case SEQ_SUB_MENU_WAIT:
		wk->counter--;
		if( wk->counter == 0 ){

			if( Stage_CommCheck(wk->type) == FALSE ){
				Stage_SeqSubTypeSelYesNo( wk );
				wk->sub_seq = SEQ_SUB_MENU_MAIN;				//���j���[��
				break;
			}else{

#if 1
				StageTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Stage_EasyMsg( wk, msg_stage_room_wait, FONT_TALK );
#endif

				wk->decide_csr_pos = wk->csr_pos;
				wk->send_req = 1;
				return TRUE;
			}
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���j���[����
	case SEQ_SUB_MENU_MAIN:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			Stage_SeqSubMenuWinClear( wk );

			//�u���[�����o�ꂷ��퓬�񐔂ŁA�܂��o�ꃁ�b�Z�[�W��\�����Ă��Ȃ�������
			if( (Stage_BtlCountLeaderCheck(wk) == TRUE) &&
				(Stage_BrainAppearCheck(wk) == FALSE) ){

				//�S�Ẵ^�C�v��LV10���`�F�b�N���ăt���O���Z�b�g
				//StageMasterFlagSet( wk );

				wk->brain_before_csr_pos = wk->csr_pos;		//�u���[����I�ԑO�̃J�[�\���ʒu
				wk->sub_seq = SEQ_SUB_BRAIN_MSG;
			}else{
				Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
				Snd_SePlay( SEQ_SE_DP_UG_020 );		//����SE

				if( wk->brain_before_csr_pos != STAGE_BRAIN_CSR_POS_NONE ){
					wk->csr_pos = wk->brain_before_csr_pos;	//�u���[����I�ԑO�̃J�[�\���ʒu�ɖ߂�
				}
				return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			Stage_SeqSubMenuWinClear( wk );
			Stage_SeqSubTypeSelNo( wk );
			wk->sub_seq = SEQ_SUB_TYPE_SEL;
			break;
			//return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u���[���o�ꃁ�b�Z�[�W
	case SEQ_SUB_BRAIN_MSG:
		//��b�E�B���h�E�\��
		StageTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
		//wk->msg_index = Stage_EasyMsg( wk, msg_stage_room_boss_01, FONT_SYSTEM );
		wk->msg_index = StageWriteMsg(	wk, &wk->bmpwin[BMPWIN_TALK], msg_stage_room_boss_01, 
										1, 1, 
										CONFIG_GetMsgPrintSpeed(SaveData_GetConfig(wk->sv)),
										//MSG_ALLPUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
		GF_BGL_BmpWinOn( &wk->bmpwin[BMPWIN_TALK] );
		wk->sub_seq = SEQ_SUB_BRAIN_MSG_WAIT;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u���[���o�ꃁ�b�Z�[�W�҂�
	case SEQ_SUB_BRAIN_MSG_WAIT:
		if( GF_MSG_PrintEndCheck(wk->msg_index) == 0 ){
			wk->brain_appear = 1;
			Stage_SeqSubWinRet( wk );

			//�A�j���ύX
			StageObj_AnmChg( wk->p_csr, ANM_CSR_MOVE );

			//???�X�N���[���̃p���b�g�����ɖ߂�
			ScrnPalChg( wk->bgl, (TYPE_XY_MAX-1), NORMAL_PAL_CHG );

			//�^�C�v�̃p���b�g�ԍ���ς���
			GameStartTypeScrnPalChg( wk, wk->bgl );

			//�J�[�\���ʒu��???�ɍ��킹��
			wk->csr_pos = (TYPE_XY_MAX - 1);
			StageObj_SetObjPos( wk->p_csr, GetCsrX(wk), GetCsrY(wk) );

			wk->sub_seq = SEQ_SUB_START;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	case SEQ_SUB_PSD_CALL:
		//�t�F�[�h�I���҂�
		if( WIPE_SYS_EndCheck() == TRUE ){
			Stage_SetPSD( wk );
			StageCommon_Delete( wk );			//�폜����
			wk->child_proc = PROC_Create( &PokeStatusProcData, wk->psd, HEAPID_STAGE );
			//p_proc = (PROC_DATA*)wk->child_proc;
			wk->recover = 1;
			//wk->sub_seq++;
			return TRUE;						//���ӁI]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	����M
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
enum{
	SEQ_SEND_RECV_START = 0,
	SEQ_SEND_RECV_WAIT,
	SEQ_SEND_RECV_DECIDE_MINE,
	SEQ_SEND_RECV_DECIDE_PAIR,
	SEQ_SEND_RECV_DECIDE_PAIR2,
	SEQ_SEND_RECV_MINE_MENU_WAIT,
	SEQ_SEND_RECV_MINE_MENU_YES,
	SEQ_SEND_RECV_MINE_MENU_NO,
	SEQ_SEND_RECV_MINE_MENU_DECIDE,
	SEQ_SEND_RECV_MINE_MENU_DECIDE_WAIT,
};

static BOOL Seq_GameSendRecv( STAGE_WORK* wk )
{
	u8 get_csr_pos;
	u32 ret;

	switch( wk->sub_seq ){

	///////////////////////////////////////////
	//�I�񂾃|�P�����^�C�v�𑗐M
	///////////////////////////////////////////
	case SEQ_SEND_RECV_START:
		get_csr_pos = Stage_GetCsrPos( wk->decide_csr_pos );
		if(Stage_CommSetSendBuf(wk,STAGE_COMM_PAIR_POKETYPE,get_csr_pos) ==	TRUE ){

#if 0		//08.03.26�@�ړ�
			//��b�E�B���h�E�\��
			StageObj_Vanish( wk->p_icon, STAGE_VANISH_ON );	//��\��
			StageTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
			Frontier_PairNameWordSet( wk->wordset, 0 );
			wk->msg_index = Stage_EasyMsg( wk, msg_stage_room_10, FONT_TALK );
			wk->wait = STAGE_MSG_WAIT;
#else
			wk->wait = 0;
#endif

			wk->decide_csr_pos = 0;
			wk->send_req = 0;
			wk->sub_seq = SEQ_SEND_RECV_WAIT;
		}
		break;

	///////////////////////////////////////////
	//��M�҂�
	///////////////////////////////////////////
	case SEQ_SEND_RECV_WAIT:
		if( wk->wait > 0 ){
			wk->wait--;
			break;
		}

		//�e�̌��肪��̎��̂݁Aparent_decide_pos�ɒl����ɓ����Ă��܂��̂ŁA
		//��������M�������ɐi�ނ悤�ɂ���
		if( wk->typesel_recieve_count < STAGE_COMM_PLAYER_NUM ){
		//if( wk->parent_decide_pos == STAGE_DECIDE_NONE ){
			break;
		}

		wk->typesel_recieve_count = 0;

		//�e�̌��肪�L��
		if( wk->parent_decide_pos < TYPE_XY_MAX ){
			if( CommGetCurrentID() == COMM_PARENT_ID ){
				wk->sub_seq = SEQ_SEND_RECV_DECIDE_MINE;
			}else{
				wk->sub_seq = SEQ_SEND_RECV_DECIDE_PAIR;
			}

		//�q�̌��肪�L��
		}else{
			if( CommGetCurrentID() == COMM_PARENT_ID ){
				wk->sub_seq = SEQ_SEND_RECV_DECIDE_PAIR;
			}else{
				wk->sub_seq = SEQ_SEND_RECV_DECIDE_MINE;
			}
		}
		break;

	///////////////////////////////////////////
	//���肪����(����)
	///////////////////////////////////////////
	case SEQ_SEND_RECV_DECIDE_PAIR:

#if 1	//08.03.26�@�ړ�
		//��b�E�B���h�E�\��
		StageObj_Vanish( wk->p_icon, STAGE_VANISH_ON );	//��\��
		StageTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
		Frontier_PairNameWordSet( wk->wordset, 0 );
		wk->msg_index = Stage_EasyMsg( wk, msg_stage_room_10, FONT_TALK );
		wk->wait = STAGE_MSG_WAIT;
#endif

#if 1
		//�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX����
		ScrnPalChg( wk->bgl, wk->pair_csr_pos, PAIR_PAL_CHG );
		GF_BGL_LoadScreenV_Req( wk->bgl, BS_FRAME_BG );	//�X�N���[���f�[�^�]��

		//��b�E�B���h�E�\��
		//StageObj_Vanish( wk->p_icon, STAGE_VANISH_ON );	//��\��
		//StageTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );

		//�u�����񂪑I��ł��܂��@���΂炭���҂����������v
		//Frontier_PairNameWordSet( wk->wordset, 0 );
		//wk->msg_index = Stage_EasyMsg( wk, msg_stage_room_10, FONT_TALK );
#endif
		wk->sub_seq = SEQ_SEND_RECV_DECIDE_PAIR2;
		break;

	///////////////////////////////////////////
	//���肪����(�҂�)
	///////////////////////////////////////////
	case SEQ_SEND_RECV_DECIDE_PAIR2:

		//����̍ŏI�m�F����M
		if( wk->pair_final_answer == FINAL_ANSWER_NONE ){
			break;
		}

		//�^�C�v���ŏI����
		if( wk->pair_final_answer == FINAL_ANSWER_YES ){

			//�����I����
			wk->csr_pos = wk->pair_csr_pos;
			return TRUE;

		//�^�C�v���L�����Z��
		}else{
			Stage_SeqSubWinRet( wk );

			//�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX����
			ScrnPalChg( wk->bgl, wk->pair_csr_pos, NORMAL_PAL_CHG );
			GF_BGL_LoadScreenV_Req( wk->bgl, BS_FRAME_BG );	//�X�N���[���f�[�^�]��

			//�N���A���Čp��
			wk->pair_csr_pos		= CSR_POS_NONE;
			wk->pair_final_answer	= FINAL_ANSWER_NONE;
			wk->parent_decide_pos	= STAGE_DECIDE_NONE;

			wk->type_sel_return_flag = 1;
			return TRUE;
		}
		break;

	///////////////////////////////////////////
	//����������
	///////////////////////////////////////////
	case SEQ_SEND_RECV_DECIDE_MINE:
		//�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX����
		ScrnPalChg( wk->bgl, wk->csr_pos, DECIDE_PAL_CHG );
		GF_BGL_LoadScreenV_Req( wk->bgl, BS_FRAME_BG );	//�X�N���[���f�[�^�]��
		Stage_SeqSubTypeSelYesNo( wk );
		wk->sub_seq = SEQ_SEND_RECV_MINE_MENU_WAIT;
		break;

	///////////////////////////////////////////
	//���j���[�҂�
	///////////////////////////////////////////
	case SEQ_SEND_RECV_MINE_MENU_WAIT:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
			Snd_SePlay( SEQ_SE_DP_UG_020 );		//����SE
			Stage_SeqSubMenuWinClear( wk );
			wk->sub_seq = SEQ_SEND_RECV_MINE_MENU_YES;
			break;

		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			Stage_SeqSubMenuWinClear( wk );
			wk->sub_seq = SEQ_SEND_RECV_MINE_MENU_NO;
			break;
		};

		break;
		
	///////////////////////////////////////////
	//�u�͂��v
	///////////////////////////////////////////
	case SEQ_SEND_RECV_MINE_MENU_YES:
		//�|�P�����^�C�v���ŏI����ő��M
		if( Stage_CommSetSendBuf(wk,STAGE_COMM_PAIR_FINAL_ANSWER,FINAL_ANSWER_YES) == TRUE ){
			return TRUE;
		}
		break;

	///////////////////////////////////////////
	//�u�������v
	///////////////////////////////////////////
	case SEQ_SEND_RECV_MINE_MENU_NO:
		//�|�P�����^�C�v���L�����Z���ő��M
		if( Stage_CommSetSendBuf(wk,STAGE_COMM_PAIR_FINAL_ANSWER,FINAL_ANSWER_NO) == TRUE ){
			Stage_SeqSubTypeSelNo( wk );
			wk->pair_csr_pos = CSR_POS_NONE;
			wk->parent_decide_pos = STAGE_DECIDE_NONE;
			wk->type_sel_return_flag = 1;
			return TRUE;
		};
		break;

	///////////////////////////////////////////
	//�ŏI���肵��
	///////////////////////////////////////////
	case SEQ_SEND_RECV_MINE_MENU_DECIDE:
		//�����J�n
		CommToolTempDataReset();
		CommTimingSyncStart( DBC_TIM_STAGE_END );
		wk->sub_seq = SEQ_SEND_RECV_MINE_MENU_DECIDE_WAIT;
		break;

	///////////////////////////////////////////
	//����
	///////////////////////////////////////////
	case SEQ_SEND_RECV_MINE_MENU_DECIDE_WAIT:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_STAGE_END) == TRUE ){
			CommToolTempDataReset();
			CommToolInitialize( HEAPID_STAGE );	//timingSyncEnd=0xff
			wk->parent_decide_pos = STAGE_DECIDE_NONE;
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEndMulti( STAGE_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	case 0:
		//�����J�n
		CommToolTempDataReset();
		CommTimingSyncStart( DBC_TIM_STAGE_END );
		
		wk->sub_seq++;
		break;

	//�����҂�
	case 1:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_STAGE_END) == TRUE ){
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( STAGE_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	case 0:
		wk->wait = STAGE_END_WAIT;
		wk->sub_seq++;
		break;

	//�t�F�[�h�A�E�g
	case 1:
		wk->wait--;
		if( wk->wait == 0 ){
			Stage_SeqSubWinRet( wk );
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
					WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_STAGE );
			wk->sub_seq++;
		}
		break;

	//�t�F�[�h�I���҂�
	case 2:
		if( WIPE_SYS_EndCheck() == TRUE ){
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�p�[�g�i�[���|�P�����^�C�v�����肵���̂ŋ����I����
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GamePairEnd( STAGE_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	case 0:
		Stage_SeqSubMenuWinClear( wk );
		StageObj_Vanish( wk->p_icon, STAGE_VANISH_OFF );	//�\��
		BmpTalkWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );

		//wk->wait_count = STAGE_PAIR_END_WAIT;
		wk->sub_seq++;
		break;

	case 1:
		//�����E�F�C�g������
		//wk->wait_count--;
		//if( wk->wait_count == 0 ){
			return TRUE;
		//}
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void StageCommon_Delete( STAGE_WORK* wk )
{
	int i;

	if( wk->p_csr != NULL ){
		StageObj_Delete( wk->p_csr );
	}

	if( wk->p_icon != NULL ){
		StageObj_Delete( wk->p_icon );
	}

	//�ʐM�A�C�R���폜
	WirelessIconEasyEnd();

	//�p���b�g�t�F�[�h�J��
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );

	//�p���b�g�t�F�[�h�V�X�e���J��
	PaletteFadeFree( wk->pfd );
	wk->pfd = NULL;

	StageClact_DeleteCellObject(&wk->stage_clact);		//2D�I�u�W�F�N�g�֘A�̈�J��

	MSGMAN_Delete( wk->msgman );						//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );						//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );						//���b�Z�[�W�o�b�t�@�J��
	NUMFONT_Delete( wk->num_font );

	for( i=0; i < STAGE_MENU_BUF_MAX ;i++ ){
		STRBUF_Delete( wk->menu_buf[i] );				//���j���[�o�b�t�@�J��
	}

	StageExitBmpWin( wk->bmpwin );						//BMP�E�B���h�E�J��
	Stage_BgExit( wk->bgl );							//BGL�폜
	ArchiveDataHandleClose( wk->hdl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���A
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_Recover( STAGE_WORK* wk )
{
	int i;

	Stage_InitSub1();

	wk->bgl	= GF_BGL_BglIniAlloc( HEAPID_STAGE );

	Stage_InitSub2( wk );

	//���݂̃J�[�\���ʒu���_�~�[�^�C�v���ŃA�j����ύX����
	if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
		StageObj_AnmChg( wk->p_csr, ANM_CSR_POKE );			//�A�j���ύX
		StageObj_SetObjPos( wk->p_csr, CSR_POKE_X, CSR_POKE_Y );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������1
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_InitSub1( void )
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_InitSub2( STAGE_WORK* wk )
{
	int i,flip;

	wk->hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_STAGE );
	Stage_BgInit( wk );								//BG������
	Stage_ObjInit( wk );								//OBJ������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_stage_room_dat, HEAPID_STAGE );

	wk->wordset = WORDSET_Create( HEAPID_STAGE );
	wk->msg_buf = STRBUF_Create( STAGE_MSG_BUF_SIZE, HEAPID_STAGE );
	wk->tmp_buf = STRBUF_Create( STAGE_MSG_BUF_SIZE, HEAPID_STAGE );

	//������o�b�t�@�쐬
	for( i=0; i < STAGE_MENU_BUF_MAX ;i++ ){
		wk->menu_buf[i] = STRBUF_Create( STAGE_MENU_BUF_SIZE, HEAPID_STAGE );
	}

	//�t�H���g�p���b�g
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, BS_FONT_PAL * 32, HEAPID_STAGE );
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, BS_MSGFONT_PAL * 32, HEAPID_STAGE );

	//8x8�t�H���g�쐬(�g�pbmpwin[BMPWIN_TYPE]�̃p���b�g(BS_FONT_PAL)���g�p
	wk->num_font = NUMFONT_Create( 15, 14, FBMP_COL_NULL, HEAPID_STAGE );

	//�r�b�g�}�b�v�ǉ�
	StageAddBmpWin( wk->bgl, wk->bmpwin );

	//�^�C�v�̃p���b�g�ԍ���ς���
	GameStartTypeScrnPalChg( wk, wk->bgl );

	GF_Disp_DispOn();

	//�J�[�\��OBJ�ǉ�
	wk->p_csr = StageObj_Create(	&wk->stage_clact, STAGE_ID_OBJ_CSR, ANM_CSR_MOVE, 
									GetCsrX(wk), GetCsrY(wk), NULL );

	//�A�C�R��OBJ�ǉ�
	wk->p_icon = StageObj_Create(	&wk->stage_clact, STAGE_ID_OBJ_ICON, POKEICON_ANM_HPMAX,
									ICON_X, ICON_Y, NULL );

	//�p���b�g�؂�ւ�
	StageObj_IconPalChg( wk->p_icon, PokeParty_GetMemberPointer(wk->p_party,0) );

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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_BgInit( STAGE_WORK* wk )
{
	SetVramBank();
	SetBgHeader( wk->bgl );

	//�p���b�g�t�F�[�h�V�X�e�����[�N�쐬
	wk->pfd = PaletteFadeInit( HEAPID_STAGE );

	//���N�G�X�g�f�[�^��malloc���ăZ�b�g
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_OBJ, FADE_PAL_ALL_SIZE, HEAPID_STAGE );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ALL_SIZE, HEAPID_STAGE );

	//���ʔw�i
	Stage_SetMainBgGraphic( wk, BS_FRAME_BG );
	Stage_SetMainBgPalette();
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��

	//����ʔw�i�A�p���b�g�Z�b�g
	Stage_SetSubBgGraphic( wk, BS_FRAME_SUB );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�֘A������
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_ObjInit( STAGE_WORK* wk )
{
	StageClact_InitCellActor(	&wk->stage_clact, 
								PokeParty_GetMemberPointer(wk->p_party,0) );
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
static void Stage_BgExit( GF_BGL_INI * ini )
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
	GF_BGL_BGControlExit( ini, BS_FRAME_BG );			//3
	GF_BGL_BGControlExit( ini, BS_FRAME_TYPE );
	GF_BGL_BGControlExit( ini, BS_FRAME_WIN );
	GF_BGL_BGControlExit( ini, BS_FRAME_SUB );

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
	STAGE_WORK* wk = work;

	//�|�P�����X�e�[�^�X�\����
	if( wk->child_proc != NULL ){
		return;
	}

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
		GF_BGL_BGControlSet( ini, BS_FRAME_WIN, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BS_FRAME_WIN, 32, 0, HEAPID_STAGE );
		GF_BGL_ScrClear( ini, BS_FRAME_WIN );
	}

	{	//BG(�t�H���g)(�^�C�v�\��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BS_FRAME_TYPE, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BS_FRAME_TYPE, 32, 0, HEAPID_STAGE );
		GF_BGL_ScrClear( ini, BS_FRAME_TYPE );
	}

	{	//����(�w�i)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BS_FRAME_BG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BS_FRAME_BG );
	}

	//--------------------------------------------------------------------------------
	{	//�����(�{�[��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BS_FRAME_SUB, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BS_FRAME_SUB );
	}

	G2_SetBG0Priority( 0 );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SetMainBgGraphic( STAGE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BS_SELECT_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_STAGE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BS_SELECT_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_STAGE );
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
static void Stage_SetMainBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BS_SELECT_NCLR, &dat, HEAPID_STAGE );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*6) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*6) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i�A�p���b�g�Z�b�g
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SetSubBgGraphic( STAGE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BATT_FRONTIER_NCGR_BIN, wk->bgl, frm,
							0, 0, TRUE, HEAPID_STAGE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BATT_FRONTIER_NSCR_BIN, wk->bgl, frm,
						0, 0, TRUE, HEAPID_STAGE );

	ArcUtil_HDL_PalSet(	wk->hdl, BATT_FRONTIER_NCLR, PALTYPE_SUB_BG,
						0, 0x20, HEAPID_STAGE );
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
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
static u8 StageWriteMsg( STAGE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
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
static u8 StageWriteMsgSimple( STAGE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Stage_EasyMsg( STAGE_WORK* wk, int msg_id, u8 font )
{
	u8 msg_index;

	msg_index = StageWriteMsg(wk, &wk->bmpwin[BMPWIN_TALK], msg_id, 
								1, 1, MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, font );

	GF_BGL_BmpWinOn( &wk->bmpwin[BMPWIN_TALK] );

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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	win		GF_BGL_BMPWIN�^�̃|�C���^
 * @param	y_max	���ڍő吔
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void StageInitMenu( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max )
{
	int i;

	for( i=0; i < STAGE_MENU_BUF_MAX ;i++ ){
		wk->Data[i].str	  = NULL;
		wk->Data[i].param = 0;
	}

	wk->MenuH.menu		= wk->Data;
	wk->MenuH.win		= win;
	wk->MenuH.font		= BS_FONT;
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	no		�Z�b�g����ꏊ(�߂�l)
 * @param	param	�߂�l
 * @param	msg_id	���b�Z�[�WID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void StageSetMenuData( STAGE_WORK* wk, u8 no, u8 param, int msg_id )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( no < STAGE_MENU_BUF_MAX, "���j���[���ڐ��I�[�o�[�I" );

	MSGMAN_GetString( wk->msgman, msg_id, wk->menu_buf[no] );

	wk->Data[ no ].str = (const void *)wk->menu_buf[no];
	wk->Data[ no ].param = param;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g2�u�͂��A�������v
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SetMenu2( STAGE_WORK* wk )
{
	wk->menu_flag = 1;
	StageWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_YESNO] );
	StageInitMenu( wk, &wk->bmpwin[BMPWIN_YESNO], 2 );
	StageSetMenuData( wk, 0, 0, msg_stage_room_choice_05 );				//�͂�
	StageSetMenuData( wk, 1, 1, msg_stage_room_choice_06 );				//������
	wk->mw = BmpMenuAddEx( &wk->MenuH, 8, 0, 0, HEAPID_STAGE, PAD_BUTTON_CANCEL );
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * �����Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void Stage_SetNumber( STAGE_WORK* wk, u32 bufID, s32 number )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, 2, 
							NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P���������Z�b�g
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	ppp		POKEMON_PASO_PARAM
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Stage_SetPokeName( STAGE_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp )
{
	WORDSET_RegisterPokeMonsName( wk->wordset, bufID, ppp );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�����Z�b�g
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Stage_SetPlayerName( STAGE_WORK* wk, u32 bufID )
{
	WORDSET_RegisterPlayerName( wk->wordset, bufID, SaveData_GetMyStatus(wk->sv) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[����\��
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
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
static void PlayerNameWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	const MYSTATUS* my;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^

	my = SaveData_GetMyStatus( wk->sv );
	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_STAGE );
	
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	STRBUF_SetStringCode( player_buf, MyStatus_GetMyName(my) );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_ALLPUT, col, NULL );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��

	GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[����\��
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
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
static void PairNameWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^
	MYSTATUS* my;									//�p�[�g�i�[��MyStatus

	//�p�[�g�i�[��MyStatus���擾
	my = CommInfoGetMyStatus( 1 - CommGetCurrentID() );

	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_STAGE );
	
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	//���O�擾(STRBUF�ɃR�s�[)
	MyStatus_CopyNameString( my, player_buf );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_ALLPUT, col, NULL );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��

	GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
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
static void PokeNameWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	u8 x_pos;
	u32 sex,msg_id,col;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	poke =  PokeParty_GetMemberPointer( wk->p_party, 0 );
	PokeParaGet( poke, ID_PARA_default_name, sel_poke_buf );	//�|�P�����̃f�t�H���g�����擾

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	buf = STRBUF_Create( POKE_NAME_BUF_SIZE, HEAPID_STAGE );	//STRBUF����
	STRBUF_SetStringCode( buf, sel_poke_buf );					//STRCODE��STRBUF

	GF_STR_PrintColor( win, font, buf, x, y, MSG_ALLPUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	STRBUF_Delete( buf );

	GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��(���ʎw��)
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
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
static void PokeNameWriteEx( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex )
{
	u8 x_pos;
	u32 msg_id,col;
	MSGDATA_MANAGER* man;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	//�|�P���������擾
	man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_monsname_dat, HEAPID_STAGE );
	buf = MSGMAN_AllocString( man, monsno );
	MSGMAN_Delete( man );

	GF_STR_PrintColor( win, font, buf, x, y, MSG_ALLPUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	//�E�[�ɐ��ʃR�[�h��\��
	x_pos = GF_BGL_BmpWinGet_SizeX(win) - 1;

	msg_id = (sex == PARA_MALE) ? msg_stage_male : msg_stage_female;
	col = (sex == PARA_MALE) ? COL_BLUE : COL_RED;

	STRBUF_Clear( buf );
	MSGMAN_GetString( wk->msgman, msg_id, buf );
	GF_STR_PrintColor( win, font, buf, x_pos*8, y, MSG_ALLPUT, col, NULL );

	STRBUF_Delete( buf );

	GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�^�C�v��\��
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 *
 * @return	none
 */
//--------------------------------------------------------------
static void TypeWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	int i,j;
	STRBUF* buf;
	MSGDATA_MANAGER* man;

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_typename_dat, HEAPID_STAGE );
	buf = STRBUF_Create( POKE_NAME_BUF_SIZE, HEAPID_STAGE );	//STRBUF����

	for( i=0; i < TYPE_Y_MAX ;i++ ){
		for( j=0; j < TYPE_X_MAX ;j++ ){

			//�_�~�[�^�C�v�͖�������
			if( Stage_GetPokeType(i*TYPE_X_MAX+j) != DUMMY_TYPE ){

				STRBUF_Clear( buf );
				MSGMAN_GetString( man, Stage_GetPokeType(i*TYPE_X_MAX+j), buf );
				GF_STR_PrintColor(	win, font, buf, 
									TYPE_START_X + (TYPE_X * j), 
									TYPE_START_Y + (TYPE_Y * i), 
									MSG_ALLPUT, 
									GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );
			}
		}
	}

	STRBUF_Delete( buf );
	MSGMAN_Delete(man);
	GF_BGL_BmpWinOn( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����N�\��
 *
 * @param	wk			STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void RankWrite( STAGE_WORK* wk, GF_BGL_BMPWIN* win, u8 num, u32 x, u32 y )
{
	NUMFONT_WriteNumber( wk->num_font, num, 2, NUMFONT_MODE_LEFT, win, x, y );
	GF_BGL_BmpWinOnVReq( win );
	return;
}

static void RankWriteAll( STAGE_WORK* wk, GF_BGL_BMPWIN* win )
{
	int i,j;
	u8 rank,pos,type;

	for( i=0; i < TYPE_Y_MAX ;i++ ){
		for( j=0; j < TYPE_X_MAX ;j++ ){

			type = Stage_GetPokeType( i * TYPE_X_MAX + j );

			//�_�~�[�^�C�v�͖�������(???�����x������Ȃ��ŊO��)
			if( (type != DUMMY_TYPE) && (type != HATE_TYPE ) ){

				pos = ( i * TYPE_X_MAX + j );
				rank = Stage_GetTypeLevel( Stage_GetCsrPos(pos), wk->p_rank );

				//0�I���W���Ȃ̂�
				rank++; 
				if( rank > STAGE_TYPE_LEVEL_MAX ){
					rank = STAGE_TYPE_LEVEL_MAX;		//�\����10���ő�
				}
				
				RankWrite(	wk, win, rank, 
							RANK_START_X + (RANK_X * j), 
							RANK_START_Y + (RANK_Y * i) );
			}
		}
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SetPSD( STAGE_WORK* wk )
{
	wk->psd = sys_AllocMemory( HEAPID_STAGE, sizeof(PSTATUS_DATA) );
	memset( wk->psd, 0, sizeof(PSTATUS_DATA) );

	wk->psd->ppd	= wk->p_party;
	wk->psd->ppt	= PST_PP_TYPE_POKEPARTY;		//�p�����[�^�^�C�v

	wk->psd->cfg	= wk->config;

	//���ƂŊm�F
	//wk->psd->mode = PST_MODE_NORMAL;
	wk->psd->mode = PST_MODE_NO_WAZACHG;

	wk->psd->max	= PokeParty_GetPokeCount( wk->p_party );
	wk->psd->pos	= 0;

	//���ƂŊm�F
	wk->psd->waza	= 0;
	wk->psd->zukan_mode = PMNumber_GetMode( wk->sv );
	wk->psd->ev_contest = PokeStatus_ContestFlagGet( wk->sv );

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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	type	�`�F�b�N����^�C�v
 *
 * @return	"TRUE = type�������AFALSE = type���Ⴄ"
 */
//--------------------------------------------------------------
static BOOL Stage_CheckType( STAGE_WORK* wk, u8 type )
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( STAGE_WORK* wk, int* seq, int next )
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck( STAGE_WORK* wk )
{
	if( sys.cont & PAD_KEY_UP ){
		if( sys.cont & PAD_BUTTON_A ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );
		}
	}else if( sys.cont & PAD_KEY_DOWN ){
		if( sys.cont & PAD_BUTTON_A ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_B ){
			GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
		}else if( sys.cont & PAD_BUTTON_L ){
		}else if( sys.cont & PAD_BUTTON_R ){
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
 * @brief	�\���L�[����
 *
 * @param	key		�`�F�b�N����L�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CsrMove( STAGE_WORK* wk, int key )
{
	int flag;
	flag = 0;

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_LEFT ){

		//���݂̃J�[�\���ʒu���_�~�[�^�C�v�łȂ����́A
		//�J�[�\���ʒu��ۑ����Ă���
		if( Stage_GetPokeType(wk->csr_pos) != DUMMY_TYPE ){
			wk->tmp_csr_pos = wk->csr_pos;
		}else{
		}

		if( (wk->csr_pos % TYPE_X_MAX) == 0 ){					//0,4,8,12,16
			wk->csr_pos+=(TYPE_X_MAX-1);						//0��3
		}else if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
			wk->csr_pos = 16;									//16(���l�Ȃ̂Œ��ӁI)
		}else{
			wk->csr_pos--;
		}

		flag = 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_RIGHT ){

		//���݂̃J�[�\���ʒu���_�~�[�^�C�v�łȂ����́A
		//�J�[�\���ʒu��ۑ����Ă���
		if( Stage_GetPokeType(wk->csr_pos) != DUMMY_TYPE ){
			wk->tmp_csr_pos = wk->csr_pos;
		}else{
		}

		if( (wk->csr_pos % TYPE_X_MAX) == (TYPE_X_MAX-1) ){		//3,7,11,15,19
			wk->csr_pos-=(TYPE_X_MAX-1);						//0��3
		}else if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
			wk->csr_pos = 19;									//19(���l�Ȃ̂Œ��ӁI)
		}else{
			wk->csr_pos++;
		}

		flag = 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_UP ){

		//���݂̃J�[�\���ʒu���_�~�[�^�C�v�łȂ����́A
		//�J�[�\���ʒu��ۑ����Ă���
		if( Stage_GetPokeType(wk->csr_pos) != DUMMY_TYPE ){
			wk->tmp_csr_pos = wk->csr_pos;
		}else{
		}

		if( wk->csr_pos < TYPE_X_MAX ){							//0,1,2,3
			wk->csr_pos+=(TYPE_X_MAX*(TYPE_Y_MAX-1));			//0��16
		}else if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
			if( wk->tmp_csr_pos == 16 ){										//���ɂ���
				wk->csr_pos = 13;
			}else if( wk->tmp_csr_pos == 19 ){									//�E�ɂ���
				wk->csr_pos = 14;
			}else if( (wk->tmp_csr_pos == 13) || (wk->tmp_csr_pos == 1 ) ){		//����ɂ���
				wk->csr_pos = 13;
			}else if( (wk->tmp_csr_pos == 14) || (wk->tmp_csr_pos == 2 ) ){		//�E��ɂ���
				wk->csr_pos = 14;
			}
		}else{
			wk->csr_pos-=TYPE_X_MAX;
		}

		flag = 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_DOWN ){

		//���݂̃J�[�\���ʒu���_�~�[�^�C�v�łȂ����́A
		//�J�[�\���ʒu��ۑ����Ă���
		if( Stage_GetPokeType(wk->csr_pos) != DUMMY_TYPE ){
			wk->tmp_csr_pos = wk->csr_pos;
		}else{
		}

		if( wk->csr_pos >= TYPE_X_MAX*(TYPE_Y_MAX-1) ){							//16,17,18,19
			wk->csr_pos-=(TYPE_X_MAX*(TYPE_Y_MAX-1));							//16��0
		}else if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
			if( wk->tmp_csr_pos == 16 ){										//���ɂ���
				wk->csr_pos = 1;
			}else if( wk->tmp_csr_pos == 19 ){									//�E�ɂ���
				wk->csr_pos = 2;
			}else if( (wk->tmp_csr_pos == 13) || (wk->tmp_csr_pos == 1 ) ){		//����ɂ���
				wk->csr_pos = 1;
			}else if( (wk->tmp_csr_pos == 14) || (wk->tmp_csr_pos == 2 ) ){		//�E��ɂ���
				wk->csr_pos = 2;
			}
		}else{
			wk->csr_pos+=TYPE_X_MAX;
		}

		flag = 1;
	}

	if( flag == 1 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
		StageObj_SetObjPos( wk->p_csr, GetCsrX(wk), GetCsrY(wk) );
	}

	//���݂̃J�[�\���ʒu���_�~�[�^�C�v���ŃA�j����ύX����
	if( Stage_GetPokeType(wk->csr_pos) == DUMMY_TYPE ){
		StageObj_AnmChg( wk->p_csr, ANM_CSR_POKE );			//�A�j���ύX
		StageObj_SetObjPos( wk->p_csr, CSR_POKE_X, CSR_POKE_Y );
	}else{
		StageObj_AnmChg( wk->p_csr, ANM_CSR_MOVE );			//�A�j���ύX
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���w�擾
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
static u16 GetCsrX( STAGE_WORK* wk )
{
	u8 pos;
	pos = wk->csr_pos;

	return (pos % TYPE_X_MAX) * TYPE_X + CSR_START_X;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���x�擾
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"Y"
 */
//--------------------------------------------------------------
static u16 GetCsrY( STAGE_WORK* wk )
{
	return (wk->csr_pos / TYPE_X_MAX) * TYPE_Y + CSR_START_Y;
}

//--------------------------------------------------------------
/**
 * @brief	�^�C�v�̃p���b�g�ԍ���ύX
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void GameStartTypeScrnPalChg( STAGE_WORK* wk, GF_BGL_INI* ini )
{
	int i;

	//�u���[�����o�ꂷ��퓬�񐔂�������A�u???�v�ȊO�̓p���b�g�ύX
	//if( Stage_BtlCountLeaderCheck(wk->btl_count) == TRUE ){
	if( Stage_BrainAppearCheck(wk) == TRUE ){

		//�u???�v�͊܂܂�
		//for( i=0; i < (STAGE_TR_TYPE_MAX-1) ;i++ ){
		for( i=0; i < (TYPE_XY_MAX-3) ;i++ ){			//status���Astatus�E�A???��3�𔲂���
			ScrnPalChg( ini, i, MAX_PAL_CHG );
		}

	}else{

		//�J�n���Ƀ��x�����}�b�N�X�ɂȂ��Ă�����̂̓p���b�g�ԍ���ύX
		//for( i=0; i < STAGE_TR_TYPE_MAX ;i++ ){
		for( i=0; i < (STAGE_TR_TYPE_MAX-1) ;i++ ){
			if( Stage_GetTypeLevel(i,wk->p_rank) >= STAGE_TYPE_LEVEL_MAX ){		//0-9,10
				//�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX����
				ScrnPalChg( ini, i, MAX_PAL_CHG );
			}
		}

		//�u???�v���p���b�g�ύX
		ScrnPalChg( ini, (TYPE_XY_MAX - 1), MAX_PAL_CHG );
	}

	//�X�N���[���f�[�^�]��
	GF_BGL_LoadScreenV_Req( ini, BS_FRAME_BG );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScrnPalChg( GF_BGL_INI* ini, u8 csr_pos, u8 flag )
{
	u8 px,py,sx,sy,pal;

	if( flag == NORMAL_PAL_CHG ){
		pal = NORMAL_PAL;					//�ʏ�p���b�g
	}else if( flag == DECIDE_PAL_CHG ){
		pal = DECIDE_PAL;					//�v���C���[����p���b�g
	}else if( flag == PAIR_PAL_CHG ){
		pal = PAIR_PAL;						//�p�[�g�i�[����p���b�g
	}else{
		pal = MAX_PAL;						//���x���ō��p���b�g
	}

	sx = 8;
	px = (csr_pos % TYPE_X_MAX) * sx;

	//�X�N���[���̂x�T�C�Y���ꏊ�ɂ���ĈႤ�̂Œ���
	if( (csr_pos % 8) < 4 ){
		sy = 5;
	}else{
		sy = 4;
	}

	//
	if( csr_pos < TYPE_X_MAX ){
		py = 0;
	}else if( csr_pos < (TYPE_X_MAX * 2) ){
		py = 5;
	}else if( csr_pos < (TYPE_X_MAX * 3) ){
		py = 9;
		}else if( csr_pos < (TYPE_X_MAX * 4) ){
		py = 14;
	}else{
		py = 18;
	}

	//OS_Printf( "px = %d\n", px );
	//OS_Printf( "py = %d\n", py );
	//OS_Printf( "sx = %d\n", sx );
	//OS_Printf( "sy = %d\n", sy );
	GF_BGL_ScrPalChange( ini, BS_FRAME_BG, px, py, sx, sy, pal );

	/////////////////////////////////////////////////////////////////////////
	//�ʏ�p���b�g��2�{�g���Ă���̂ł���ɏ㏑��
	if( flag == NORMAL_PAL_CHG ){
		pal = NORMAL_PAL;

		sx = 1;
		px = (csr_pos % TYPE_X_MAX) * 8;

		//�X�N���[���̂x�T�C�Y���ꏊ�ɂ���ĈႤ�̂Œ���
		if( (csr_pos % 8) < 4 ){
			sy = 2;
		}else{
			sy = 3;
		}

		if( csr_pos < TYPE_X_MAX ){
			py = 2;
		}else if( csr_pos < (TYPE_X_MAX * 2) ){
			py = 6;
		}else if( csr_pos < (TYPE_X_MAX * 3) ){
			py = 11;
			}else if( csr_pos < (TYPE_X_MAX * 4) ){
			py = 15;
		}else{
			py = 20;
		}

		//OS_Printf( "px = %d\n", px );
		//OS_Printf( "py = %d\n", py );
		//OS_Printf( "sx = %d\n", sx );
		//OS_Printf( "sy = %d\n", sy );
		
		//����������p���b�g���Ⴄ
		if( csr_pos < 9 ){
			GF_BGL_ScrPalChange( ini, BS_FRAME_BG, px, py, sx, sy, NORMAL_PAL2 );	//�O��
		}else{
			GF_BGL_ScrPalChange( ini, BS_FRAME_BG, px, py, sx, sy, NORMAL_PAL3 );	//�㔼
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�S�Ẵ^�C�v��LV10���`�F�b�N���ăt���O���Z�b�g
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void StageMasterFlagSet( STAGE_WORK* wk )
{
	int i;

	//�u???�v�͊܂܂�
	//for( i=0; i < STAGE_TR_TYPE_MAX ;i++ ){
	for( i=0; i < (STAGE_TR_TYPE_MAX-1) ;i++ ){
		if( Stage_GetTypeLevel(i,wk->p_rank) < STAGE_TYPE_LEVEL_MAX ){
			wk->master_flag = 0;
			return;
		}
	}

	wk->master_flag = 1;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�f�[�^���Q�Ƃ��鎞�̃J�[�\���ʒu�𒲐������l���擾
 *
 * @param	csr_pos		�J�[�\���ʒu
 *
 * @return	"���������J�[�\���ʒu"
 */
//--------------------------------------------------------------
static u8 Stage_GetCsrPos( u8 csr_pos )
{
	if( csr_pos >= 17 ){
		return STAGE_TR_TYPE_MAX - 1;
	}

	return csr_pos;
}

//--------------------------------------------------------------
/**
 * @brief	�u���[�����o�ꂷ��퓬�񐔂����擾
 *
 * @param	btl_count	�퓬��
 *
 * @return	"TRUE = �u���[���o��AFALSE = ���Ȃ�"
 */
//--------------------------------------------------------------
static BOOL Stage_BtlCountLeaderCheck( STAGE_WORK* wk )
{
#ifdef DEBUG_BRAIN_SET
	return TRUE;
#endif

	//�V���O���̂�
	if( wk->type == STAGE_TYPE_SINGLE ){

		//�u���[�����o�ꂷ��퓬�񐔂�������A�u???�v�ȊO�̓p���b�g�ύX
		if( (wk->btl_count == STAGE_LEADER_SET_1ST) || (wk->btl_count == STAGE_LEADER_SET_2ND) ){
			return TRUE;
		}
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�u���[�����o�ꂷ��t���O���`�F�b�N���擾
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = �u���[���o��AFALSE = ���Ȃ�"
 */
//--------------------------------------------------------------
static BOOL Stage_BrainAppearCheck( STAGE_WORK* wk )
{
	return wk->brain_appear;
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
 * @param	wk			STAGE_WORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL Stage_CommSetSendBuf( STAGE_WORK* wk, u16 type, u16 param )
{
	int ret,command;

	//�ʐM�^�C�v�łȂ���
	if( Stage_CommCheck(wk->type) == FALSE ){
		return FALSE;
	}

	switch( type ){

	//���O
	case STAGE_COMM_PAIR:
		command = FC_STAGE_PAIR;
		Stage_CommSendBufBasicData( wk, type );
		break;

	//�^�C�v�I����ʂŌ��肵���|�P�����^�C�v
	case STAGE_COMM_PAIR_POKETYPE:
		command = FC_STAGE_PAIR_POKETYPE;
		Stage_CommSendBufPokeTypeData( wk, type, param );
		break;

	//�|�P�����^�C�v���ŏI���肩�L�����Z����
	case STAGE_COMM_PAIR_FINAL_ANSWER:
		command = FC_STAGE_PAIR_FINAL_ANSWER;
		Stage_CommSendBufFinalAnswerData( wk, type, param );
		break;
	};

	if( CommSendData(command,wk->send_buf,STAGE_COMM_BUF_LEN) == TRUE ){
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
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Stage_CommSendBufBasicData( STAGE_WORK* wk, u16 type )
{
	MYSTATUS* my;
	
	my	= SaveData_GetMyStatus( wk->sv );

	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );
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
void Stage_CommRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	STAGE_WORK* wk = work;
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
	
	//wk-> = 
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ^�C�v�I����ʂŌ��肵���|�P�����^�C�v���Z�b�g
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Stage_CommSendBufPokeTypeData( STAGE_WORK* wk, u16 type, u16 param )
{
	int i,num;
	POKEMON_PARAM* poke;
	POKEPARTY* party;
	
	num = 0;

	//�R�}���h
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	//�I�����Ă���ʒu
	wk->send_buf[1] = param;
	OS_Printf( "���M�Fcsr_pos = %d\n", wk->send_buf[1] );

	//��Ɏq�̑I�������Ă��Ȃ��āA�܂��l�������Ă��Ȃ����́A�e�̌���̓Z�b�g���Ă��܂�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		if( wk->parent_decide_pos == STAGE_DECIDE_NONE ){
			wk->parent_decide_pos = param;
		}
	}

	//�e�̌���^�C�v
	wk->send_buf[2] = wk->parent_decide_pos;
	OS_Printf( "���M�Fparent_decide_pos = %d\n", wk->send_buf[2] );

	poke =  PokeParty_GetMemberPointer( wk->p_party, 0 );
	wk->send_buf[3] = PokeParaGet( poke, ID_PARA_level, NULL );
	OS_Printf( "���M�Fpoke level = %d\n", wk->send_buf[3] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   recv_buf�̃^�C�v�I����ʂŌ��肵���|�P�����^�C�v �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Stage_CommRecvBufPokeTypeData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	STAGE_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�X�e�[�W****** ��{����M\n" );

	num = 0;
	wk->typesel_recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	//type = recv_buf[0];
	
	wk->pair_csr_pos = recv_buf[1];	//�p�[�g�i�[���I�񂾃|�P�����^�C�v(�J�[�\���ʒu)
	OS_Printf( "��M�Fwk->pair_csr_pos = %d\n", wk->pair_csr_pos );

	////////////////////////////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�̌��肪���łɌ��܂��Ă�����A�q�̑I���͖���
		if( wk->parent_decide_pos != STAGE_DECIDE_NONE ){
			wk->pair_csr_pos = 0;
		}else{

			//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g(TYPE_XY_MAX)��������
			wk->parent_decide_pos	= wk->pair_csr_pos + TYPE_XY_MAX;
		}
	////////////////////////////////////////////////////////////////////////
	//�q
	}else{
		//�e�̌���^�C�v
		wk->parent_decide_pos = recv_buf[2];
	}

	OS_Printf( "��M�Fwk->parent_decide_pos = %d\n", wk->parent_decide_pos );

	//////////////////////////////////////////////////////////////////////////////////////
	
	*(wk->p_pair_poke_level) = recv_buf[3];
	OS_Printf( "��M�Fwk->p_pair_poke_level = %d\n", *(wk->p_pair_poke_level) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ|�P�����^�C�v���ŏI���肩�L�����Z�������Z�b�g
 *
 * @param	wk		STAGE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void Stage_CommSendBufFinalAnswerData( STAGE_WORK* wk, u16 type, u16 param )
{
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	wk->send_buf[1] = param;
	OS_Printf( "���M�Fpair_final_answer = %d\n", wk->send_buf[1] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   recv_buf�̃|�P�����^�C�v���ŏI���肩�L�����Z���� �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Stage_CommRecvBufFinalAnswerData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	STAGE_WORK* wk = work;
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

	//�����Ȃ��͂��̂܂�
	if( recv_buf[1] == FINAL_ANSWER_NONE ){
		return;
	}
	
	wk->pair_final_answer = recv_buf[1];		//0=�����Ȃ��A1=�ŏI����A2=�L�����Z��
	OS_Printf( "��M�Fwk->pair_final_answer = %d\n", wk->pair_final_answer );
	return;
}


//==============================================================================================
//
//	�T�u�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���j���[�E�B���h�E�N���A
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SeqSubMenuWinClear( STAGE_WORK* wk )
{
	if( wk->menu_flag == 1 ){
		wk->menu_flag = 0;
		BmpMenuExit( wk->mw, NULL );
		BmpMenuWinClear( wk->MenuH.win, WINDOW_TRANS_ON );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�^�C�v��I��Łu�͂��A�������v��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SeqSubTypeSelYesNo( STAGE_WORK* wk )
{
	u8 num;
	int msg_id;

	//��b�E�B���h�E�\��
	StageObj_Vanish( wk->p_icon, STAGE_VANISH_ON );	//��\��
	StageTalkWinPut( &wk->bmpwin[BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );

	//�u���^�C�v�́������N�ƃo�g�����܂����H�v
	WORDSET_RegisterPokeTypeName( wk->wordset, 0, Stage_GetPokeType(wk->csr_pos) );

	num = Stage_GetTypeLevel( Stage_GetCsrPos(wk->csr_pos),wk->p_rank );	//0-9,10
	num++;
	if( num > STAGE_TYPE_LEVEL_MAX ){
		num = STAGE_TYPE_LEVEL_MAX;		//�\����10���ő�
	}

	Stage_SetNumber(wk, 1, num );

	if( Stage_BrainAppearCheck(wk) == TRUE ){
		msg_id = msg_stage_room_44;		//�u���[��
	}else{
		msg_id = msg_stage_room_41;		//�ʏ�
	}

	wk->msg_index = Stage_EasyMsg( wk, msg_id, FONT_TALK );

	//�A�j���ύX
	StageObj_AnmChg( wk->p_csr, ANM_CSR_STOP );

	//�u�͂��E�������v
	Stage_SetMenu2( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�^�C�v��I��Łu�������v
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SeqSubTypeSelNo( STAGE_WORK* wk )
{
	Stage_SeqSubWinRet( wk );

	//�w��t���[���̃X�N���[���̎w��ʒu�̃p���b�g��ύX����
	ScrnPalChg( wk->bgl, wk->csr_pos, NORMAL_PAL_CHG );
	GF_BGL_LoadScreenV_Req( wk->bgl, BS_FRAME_BG );	//�X�N���[���f�[�^�]��

	//�A�j���ύX
	StageObj_AnmChg( wk->p_csr, ANM_CSR_MOVE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�E�B���h�E�����ɖ߂�
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Stage_SeqSubWinRet( STAGE_WORK* wk )
{
	StageObj_Vanish( wk->p_icon, STAGE_VANISH_OFF );					//�|�P�����\��
	BmpTalkWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );		//��b�E�B���h�E�폜
	PokeNameWrite(	wk, &wk->bmpwin[BMPWIN_POKE1], 0, 0,				//�|�P��������\��
					FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, BS_FONT );
	return;
}


