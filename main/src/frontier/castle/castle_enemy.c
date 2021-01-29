//==============================================================================================
/**
 * @file	castle_enemy.c
 * @brief	�u�o�g���L���b�X�� �G�g���[�i�[��ʁv���C���\�[�X
 * @author	Satoshi Nohara
 * @date	07.07.05
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
#include "system/bmp_list.h"
#include "system/window.h"
#include "system/fontproc.h"
#include "system/numfont.h"
#include "gflib/strbuf_family.h"
#include "savedata/config.h"
#include "application/p_status.h"
#include "application/app_tool.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_number.h"
#include "poketool/pokeicon.h"
#include "savedata/b_tower.h"
#include "savedata/frontier_savedata.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_info.h"
#include "communication/comm_def.h"
#include "communication/wm_icon.h"

#include "castle_sys.h"
#include "castle_common.h"
#include "castle_clact.h"
#include "castle_bmp.h"
#include "castle_obj.h"
#include "castle_enemy_def.h"
#include "application/castle.h"
#include "../frontier_tool.h"						//Frontier_PokeParaMake
#include "../castle_tool.h"							//
#include "../castle_def.h"
#include "../comm_command_frontier.h"

#include "../../field/comm_direct_counter_def.h"	//
#include "../../field/fieldobj.h"
#include "../../field/scr_tool.h"

#include "msgdata\msg.naix"							//NARC_msg_??_dat
#include "msgdata\msg_castle_trainer.h"				//msg_??

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


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�V�[�P���X��`
enum {
	SEQ_GAME_INIT,											//������

	SEQ_GAME_TYPE_SEL,										//�^�C�v�I��

	SEQ_GAME_SEND_RECV,										//����M

	SEQ_GAME_END_MULTI,										//(�ʐM)�I��
	SEQ_GAME_END,											//�I��
};

//�I����
enum{
	SEL_TEMOTI = 0,
	SEL_LEVEL,
	SEL_TUYOSA,
	SEL_WAZA,
	SEL_TOZIRU,
};

#define INFO_RANKUP_POINT			(50)					//�����N�A�b�v�ɕK�v�ȃ|�C���g

//�������肵������`
enum{
	FC_PARAM_TEMOTI = 0,				//�Ă���
	FC_PARAM_LEVEL,						//���x��
	FC_PARAM_INFO,						//���傤�ق�
	FC_PARAM_TUYOSA,					//�悳
	FC_PARAM_WAZA,						//�킴
	/////////////////////////////
	FC_PARAM_INFO_RANKUP,			//(���傤�ق�)�����N�A�b�v
	/////////////////////////////
	FC_PARAM_TOZIRU,					//�Ƃ���
};

//TYPE_SEL�ɖ߂������̕��A�����̕����`
#define RECOVER_NONE				(0)
#define RECOVER_NORMAL				(1)
#define RECOVER_RANKUP				(2)
#define RECOVER_STATUS				(3)

//----------------------------------------------------------------------------------------------
//���̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_HATA_START_X			= (96),
	CASTLE_MULTI_HATA_START_X	= (64),
	CASTLE_HATA_START_Y			= (60),
};
#define CASTLE_HATA_WIDTH_X		(64)				//�\�����镝
#define CASTLE_HATA_WIDTH_Y		(12)				//�\�����镝

//�����N�A�b�v�G�t�F�N�g�\���ʒu
#define CASTLE_ENEMY_RANKUP_X	(211)
#define CASTLE_ENEMY_RANKUP_Y	(105)


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
struct _CASTLE_ENEMY_WORK{

	PROC* proc;										//PROC�ւ̃|�C���^
	FRONTIER_SAVEWORK* fro_sv;						//

	u8	sub_seq;									//�V�[�P���X
	u8	type;										//�����Ƃ��ēn���ꂽ�o�g���^�C�v
	u8	msg_index;									//���b�Z�[�Windex
	u8	recover;									//���A�t���O

	u8	tmp_csr_pos;								//�ޔ����Ă���J�[�\���ʒu
	u8	csr_pos;									//���݂̃J�[�\���ʒu
	u8	level_sel;									//
	u8	eff_init_flag:1;							//����G�t�F�N�g�������t���O
	u8	list_flag:1;								//���X�g�\�������t���O
	u8	menu_flag:1;								//���j���[�\�������t���O
	u8	recover_flag:5;								//TYPE_SEL�ɖ߂������̕���t���O

	u8	send_req;
	u8	parent_decide_pos;							//���肵���J�[�\���ʒu(�ǂ̃|�P������)
	u8	parent_decide_updown;
	u8	parent_decide_type;							//���肵������(FC_PARAM_??)

	u8	h_max;
	u8	modoru_pos;
	u8	wait;
	u8	recieve_count;								//��M�J�E���g

	u16 list_csr_pos;								//���X�g�őI�������ʒu
	u16	dummy26;

	u16	basic_list_lp;
	u16	basic_list_cp;

	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g
	STRBUF* msg_buf;								//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;								//�e���|�����o�b�t�@�|�C���^

	STRBUF* menu_buf[CASTLE_MENU_BUF_MAX];			//���j���[�o�b�t�@�|�C���^
	STRCODE str[PERSON_NAME_SIZE + EOM_SIZE];		//���j���[�̃��b�Z�[�W

	GF_BGL_INI*	bgl;								//BGL�ւ̃|�C���^
	GF_BGL_BMPWIN bmpwin[CASTLE_ENEMY_BMPWIN_MAX];	//BMP�E�B���h�E�f�[�^

	//BMP���j���[(bmp_menu.h)
	BMPMENU_HEADER MenuH;							//BMP���j���[�w�b�_�[
	BMPMENU_WORK* mw;								//BMP���j���[���[�N
	BMPMENU_DATA Data[CASTLE_MENU_BUF_MAX];			//BMP���j���[�f�[�^

	//BMP���X�g
	BMPLIST_WORK* lw;								//BMP���X�g�f�[�^
	BMPLIST_DATA* menulist;							//

	PALETTE_FADE_PTR pfd;							//�p���b�g�t�F�[�h

	NUMFONT* num_font;								//8x8�t�H���g

	//const CONFIG* config;							//�R���t�B�O�|�C���^
	CONFIG* config;									//�R���t�B�O�|�C���^
	SAVEDATA* sv;									//�Z�[�u�f�[�^�|�C���^
	CASTLEDATA* castle_sv;							//�L���b�X���Z�[�u�f�[�^�|�C���^
	CASTLESCORE* score_sv;							//�L���b�X���Z�[�u�f�[�^�|�C���^

	CASTLE_CLACT castle_clact;						//�Z���A�N�^�f�[�^
	CASTLE_OBJ* p_icon[CASTLE_COMM_POKE_TOTAL_NUM];	//�A�C�R��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_ball[CASTLE_COMM_POKE_TOTAL_NUM];	//�{�[��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_eff;								//�G�t�F�N�gOBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_poke_sel;							//�|�P�����I���E�B��OBJ�̃|�C���^
	CASTLE_OBJ* p_pair_poke_sel;					//�y�A�̃|�P�����I���E�B��OBJ�̃|�C���^
	CASTLE_OBJ* p_hp[CASTLE_COMM_POKE_TOTAL_NUM];	//HP���OBJ�̃|�C���^
	CASTLE_OBJ* p_hata[CASTLE_COMM_POKE_TOTAL_NUM][2];	//��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_eff_rankup;						//�����N�A�b�vOBJ�̃|�C���^�i�[�e�[�u��

	u16* p_ret_work;								//CASTLE_CALL_WORK�̖߂�l���[�N�ւ̃|�C���^

	//�G�g���[�i�[��ʂ̏�񂪌��J���ꂽ���t���O
	u8*	p_temoti_flag;
	u8*	p_level_flag;
	u8*	p_tuyosa_flag;
	u8* p_waza_flag;

	//�o�g���L���b�X���p
	POKEPARTY* p_party;

	ARCHANDLE* hdl;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//CASTLE_COMM castle_comm;
#if 1
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[CASTLE_COMM_BUF_LEN];

	//�ʐM�p
	u8	pair_csr_pos;								//�p�[�g�i�[�̃J�[�\���ʒu
	u8	pair_rankup_type;							//�p�[�g�i�[�̃����N�A�b�v�������^�C�v
	u8	pair_modoru_flag;							//�p�[�g�i�[�́u�߂�v�I��������
	u8	pair_rank[CASTLE_RANK_TYPE_MAX];			//�p�[�g�i�[�̃����N
	u16 pair_cp;									//�p�[�g�i�[��CP

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
PROC_RESULT CastleEnemyProc_Init( PROC * proc, int * seq );
PROC_RESULT CastleEnemyProc_Main( PROC * proc, int * seq );
PROC_RESULT CastleEnemyProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameInit( CASTLE_ENEMY_WORK* wk );
static void TypeSelInit( CASTLE_ENEMY_WORK* wk );
static BOOL Seq_GameTypeSel( CASTLE_ENEMY_WORK* wk );
static BOOL Seq_GameSendRecv( CASTLE_ENEMY_WORK* wk );
static BOOL Seq_GameEndMulti( CASTLE_ENEMY_WORK* wk );
static BOOL Seq_GameEnd( CASTLE_ENEMY_WORK* wk );

//���ʏ���
static void CastleCommon_Delete( CASTLE_ENEMY_WORK* wk );
static void Castle_Recover( CASTLE_ENEMY_WORK* wk );
static void Castle_InitSub1( void );
static void Castle_InitSub2( CASTLE_ENEMY_WORK* wk );

//���ʏ������A�I��
static void Castle_ObjInit( CASTLE_ENEMY_WORK* wk );
static void Castle_BgInit( CASTLE_ENEMY_WORK* wk );
static void Castle_BgExit( GF_BGL_INI * ini );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//BG�O���t�B�b�N�f�[�^
static void Castle_SetMainBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  );
static void Castle_SetMainBgPalette( void );
static void Castle_SetStatusBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  );
static void Castle_SetWazaBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  );
static void Castle_SetStatusBgPalette( void );
static void Castle_SetSubBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  );

//���b�Z�[�W
static u8 CastleWriteMsg( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 CastleWriteMsgSimple( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 Castle_EasyMsg( CASTLE_ENEMY_WORK* wk, int msg_id, u8 font );
static void Castle_StatusMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke );
static void StMsgWriteSub( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, int msg, u16 x, u16 y );
static void Castle_WazaMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke );
static void WazaMsgWriteSub( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u32 msg_id, u32 msg_id2, POKEMON_PARAM* poke, u32 id, u32 id2, u32 id3 );
static void Castle_PokeHpMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win );
static void Castle_PokeLvMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win );

//���j���[
static void CastleInitMenu( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max );
static void CastleSetMenuData( CASTLE_ENEMY_WORK* wk, u8 no, u8 param, int msg_id );
static void Castle_SetMenu2( CASTLE_ENEMY_WORK* wk );
static void Castle_SetMenu3( CASTLE_ENEMY_WORK* wk );

//���X�g
static void Castle_BasicListMake( CASTLE_ENEMY_WORK* wk );
static void Castle_CsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode );
static void Castle_Basic2ListMake( CASTLE_ENEMY_WORK* wk );
static void Castle_Basic2CsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode );
static void Castle_Basic2LineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y );

//������
static void Castle_SetNumber( CASTLE_ENEMY_WORK* wk, u32 bufID, s32 number, u32 keta, NUMBER_DISPTYPE disp );
static void Castle_SetPokeName( CASTLE_ENEMY_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp );
static void Castle_SetPlayerName( CASTLE_ENEMY_WORK* wk, u32 bufID );
static void PlayerNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PairNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PokeNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font );
static void PokeNameWriteEx( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex );
static void PokeSexWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font, u8 sex );
static void CastleEnemy_Default_Write( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_Default_Del( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_Basic_Write( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_Basic_Del( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_Basic2_Write( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_Basic2_Del( CASTLE_ENEMY_WORK* wk );

//�c�[��
static BOOL Castle_CheckType( CASTLE_ENEMY_WORK* wk, u8 type );
static void NextSeq( CASTLE_ENEMY_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( CASTLE_ENEMY_WORK* wk );
static void CsrMove( CASTLE_ENEMY_WORK* wk, int key );
static void CsrMoveSub( CASTLE_ENEMY_WORK* wk );
static void PokeSelMoveSub( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 flag );
static void CastleEnemy_GetPokeSelXY( CASTLE_ENEMY_WORK* wk, u32* x, u32* y, u8 csr_pos );
static u16 GetCsrX( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 lr );
static u16 GetCsrY( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void Castle_GetOffset( CASTLE_ENEMY_WORK* wk, u16* offset_x, u16* offset_y, u16* pair_offset_x, u16* pair_offset_y );
static BOOL CastleEnemy_DecideEff( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 sel_type );
static BOOL CastleEnemy_DecideEff2( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 sel_type );
static u16 CastleEnemy_GetLvCP( u8 level_sel );
static void CastleEnemy_TuyosaWinChg( CASTLE_ENEMY_WORK* wk, s8 add_sub );
static void CastleEnemy_WazaWinChg( CASTLE_ENEMY_WORK* wk, s8 add_sub );
static void CastleEnemy_PairDecideDel( CASTLE_ENEMY_WORK* wk );
static void BmpTalkWinClearSub( GF_BGL_BMPWIN* win );

//�ʐM
BOOL CastleEnemy_CommSetSendBuf( CASTLE_ENEMY_WORK* wk, u16 type, u16 param );
void CastleEnemy_CommSendBufBasicData( CASTLE_ENEMY_WORK* wk, u16 type );
void CastleEnemy_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
void CastleEnemy_CommSendBufRankUpType( CASTLE_ENEMY_WORK* wk, u16 type, u16 param );
void CastleEnemy_CommRecvBufReqType(int id_no,int size,void *pData,void *work);
void CastleEnemy_CommSendBufCsrPos( CASTLE_ENEMY_WORK* wk, u16 type );
void CastleEnemy_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);
void CastleEnemy_CommSendBufModoru( CASTLE_ENEMY_WORK* wk );
void CastleEnemy_CommRecvBufModoru(int id_no,int size,void *pData,void *work);

//�T�u�V�[�P���X
static void CastleEnemy_SeqSubTuyosa( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubTuyosaFlagOn( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubTuyosaMsgSet( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubWaza( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubWazaFlagOn( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubWazaMsgSet( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubMenuWinClear( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_SeqSubTalkWinOn( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_SeqSubCPWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win );
static void CastleEnemy_SeqSubLevelMsg( CASTLE_ENEMY_WORK* wk, u8 level_sel );
static void CastleEnemy_SeqSubNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win );
static void CastleEnemy_SeqSubRankUp( CASTLE_ENEMY_WORK* wk, u8 decide_type, u8 parent_decide_type );
static void CastleEnemy_SeqSubTemoti( CASTLE_ENEMY_WORK* wk, u8 csr_pos );
static void CastleEnemy_SeqSubLevel( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 level_sel );
static void CastleEnemy_SeqSubStatusWinDel( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_SeqSubListEnd( CASTLE_ENEMY_WORK* wk );
static void CastleEnemy_IconSelAnm( CASTLE_ENEMY_WORK* wk );
static BOOL CastleEnemy_SeqSubTuyosaWazaYes( CASTLE_ENEMY_WORK* wk, u16 use_cp, u16 cp_not_msg );

//castle_rank.c
static void CastleRank_SeqSubRankUp(CASTLE_ENEMY_WORK* wk, u8 parent_decide_pos, u8 param);
static void CastleRank_SeqSubHataVanish( CASTLE_ENEMY_WORK* wk );
static void Castle_HataVanish( CASTLE_ENEMY_WORK* wk, u8 no );
static void Castle_GetHataXY( CASTLE_ENEMY_WORK* wk, u32* x, u32* y );


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
PROC_RESULT CastleEnemyProc_Init( PROC * proc, int * seq )
{
	int i;
	CASTLE_ENEMY_WORK* wk;
	CASTLE_CALL_WORK* castle_call;

	Overlay_Load( FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE );

	Castle_InitSub1();

	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_CASTLE, 0x20000 );

	wk = PROC_AllocWork( proc, sizeof(CASTLE_ENEMY_WORK), HEAPID_CASTLE );
	memset( wk, 0, sizeof(CASTLE_ENEMY_WORK) );

	wk->bgl				= GF_BGL_BglIniAlloc( HEAPID_CASTLE );
	wk->proc			= proc;
	castle_call			= (CASTLE_CALL_WORK*)PROC_GetParentWork( proc );
	wk->sv				= castle_call->sv;
	wk->castle_sv		= SaveData_GetCastleData( wk->sv );
	wk->score_sv		= SaveData_GetCastleScore( wk->sv );
	wk->type			= castle_call->type;
	wk->p_ret_work		= &castle_call->ret_work;
	wk->config			= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾
	wk->p_party			= castle_call->p_e_party;
	wk->p_temoti_flag	= &castle_call->enemy_temoti_flag[0];
	wk->p_level_flag	= &castle_call->enemy_level_flag[0];
	wk->p_tuyosa_flag	= &castle_call->enemy_tuyosa_flag[0];
	wk->p_waza_flag		= &castle_call->enemy_waza_flag[0];
	wk->parent_decide_pos = CASTLE_ENEMY_DECIDE_NONE;
	wk->pair_cp			= castle_call->pair_cp;
	wk->fro_sv			= SaveData_GetFrontier( wk->sv );

	for( i=0; i < CASTLE_RANK_TYPE_MAX; i++ ){
		wk->pair_rank[i] = 1;									//�p�[�g�i�[�̃����N1�I���W��
	}

	//�ʐM�^�C�v�łȂ���
	if( Castle_CommCheck(wk->type) == FALSE ){
		wk->h_max = 3;
	}else{
		wk->h_max = 4;
	}
	wk->modoru_pos = wk->h_max;

	//B�{�^�������āA��L�[�����������ɖ߂�ʒu
	wk->tmp_csr_pos = (wk->modoru_pos - 1);

	wk->pair_csr_pos	= 0;

	Castle_InitSub2( wk );

	if( Castle_CommCheck(wk->type) == TRUE ){
		CommCommandFrontierInitialize( wk );
	}

	//OS_Printf( "CheckCharRest() = %d\n", CheckCharRest() );

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
PROC_RESULT CastleEnemyProc_Main( PROC * proc, int * seq )
{
	CASTLE_ENEMY_WORK* wk  = PROC_GetWork( proc );
	//OS_Printf( "(*seq) = %d\n", (*seq) );

	//���肪�u�߂�v��I�񂾎�
	if( wk->pair_modoru_flag == 1 ){

		switch( *seq ){

		//�ʐM�I����
		case SEQ_GAME_TYPE_SEL:
		//case SEQ_GAME_SEND_RECV:						//08.06.17 ����M���͋����I�����Ȃ�
			wk->pair_modoru_flag = 0;
			CastleEnemy_PairDecideDel( wk );
			CastleEnemy_SeqSubStatusWinDel( wk );

			//�G�t�F�N�g�����I������悤�ɂ���
			if( wk->p_eff != NULL ){
				CastleObj_Delete( wk->p_eff );
				wk->eff_init_flag = 0;
			}

			//�u�����񂪖߂��I�����܂����I�v
			CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
			Frontier_PairNameWordSet( wk->wordset, 0 );
			wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_08, FONT_TALK );

			NextSeq( wk, seq, SEQ_GAME_END_MULTI );
			break;
		};

	//
	}else if( wk->parent_decide_pos != CASTLE_ENEMY_DECIDE_NONE ){
		switch( *seq ){

		//����M�֋����ړ�
		case SEQ_GAME_TYPE_SEL:
		case SEQ_GAME_END_MULTI:						//08.06.17 ��Ɂu�߂�v���̎��́A
			wk->pair_modoru_flag = 0;					//08.06.17 �u����M�v�ɖ߂�
			CastleEnemy_PairDecideDel( wk );
			CastleEnemy_SeqSubStatusWinDel( wk );
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

				//�ʐM�^�C�v�̎�
				if( Castle_CommCheck(wk->type) == TRUE ){
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
			NextSeq( wk, seq, SEQ_GAME_TYPE_SEL );						//�^�C�v�I�𒆂�
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

	//�A�C�R���㉺�A�j��
	CastleEnemy_IconSelAnm( wk );

	//CastleObj_IconAnm( wk->p_icon );				//�A�C�R���A�j��
	CLACT_Draw( wk->castle_clact.ClactSet );		//�Z���A�N�^�[�풓�֐�

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
PROC_RESULT CastleEnemyProc_End( PROC * proc, int * seq )
{
	int i;
	CASTLE_ENEMY_WORK* wk = PROC_GetWork( proc );

	*(wk->p_ret_work) = wk->csr_pos;					//�߂�l�i�[���[�N�֑��(�J�[�\���ʒu)

	OS_Printf( "*(wk->p_ret_work) = %d\n", *(wk->p_ret_work) );

	DellVramTransferManager();

	CastleCommon_Delete( wk );							//�폜����

	PROC_FreeWork( proc );								//���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_DeleteHeap( HEAPID_CASTLE );

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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( CASTLE_ENEMY_WORK* wk )
{
	switch( wk->sub_seq ){

	//�R�}���h�ݒ��̓���
	case 0:
		if( Castle_CommCheck(wk->type) == TRUE ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_CASTLE_ENEMY_INIT );
		}
		wk->sub_seq++;
		break;

	//�^�C�~���O�R�}���h���͂������m�F
	case 1:
		if( Castle_CommCheck(wk->type) == TRUE ){
			if( CommIsTimingSync(DBC_TIM_CASTLE_ENEMY_INIT) == TRUE ){
				CommToolTempDataReset();
				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;
		
	//��{��񑗐M
	case 2:
		if( Castle_CommCheck(wk->type) == TRUE ){
			//��{���𑗐M
			if( CastleEnemy_CommSetSendBuf(wk,CASTLE_COMM_ENEMY_PAIR,0) == TRUE ){
				wk->sub_seq++;
			}
		}else{
			TypeSelInit( wk );

			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
							WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_CASTLE );
			wk->sub_seq++;
		}
		break;

	//�u���b�N�C��
	case 3:
		if( Castle_CommCheck(wk->type) == TRUE ){
			if( wk->recieve_count >= CASTLE_COMM_PLAYER_NUM ){
				wk->recieve_count = 0;

				TypeSelInit( wk );

				WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
								WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_CASTLE );

				wk->sub_seq++;
			}
		}else{
			wk->sub_seq++;
		}
		break;
		
	//�t�F�[�h�I���҂�
	case 4:
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static void TypeSelInit( CASTLE_ENEMY_WORK* wk )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,icon_offset_x,ball_offset_x;
	GF_BGL_BMPWIN* win;

	//�ʐM���Ă��邩�ŕ\���̃I�t�Z�b�g���ς��
	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	///////////////////////////////////////////////////////////////////////////////
	win = &wk->bmpwin[ENEMY_BMPWIN_TR1];
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );					//�h��Ԃ�
	CastleEnemy_SeqSubNameWrite( wk, win );							//�v���C���[�A�p�[�g�i�[���\��
	CastleEnemy_SeqSubCPWrite( wk, win );							//CP�\��

	///////////////////////////////////////////////////////////////////////////////
	//HP��LV�\��
	Castle_PokeHpMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_HP] );
	Castle_PokeLvMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_LV] );

	///////////////////////////////////////////////////////////////////////////////
	CastleEnemy_Default_Write( wk );								//TALKMENU,���ǂ�

	GF_Disp_DispOn();
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���^�C�v�I��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
enum{
	SEQ_SUB_START = 0,
	SEQ_SUB_TYPE_SEL,
	SEQ_SUB_MENU_MAIN,
	//////////////////////////////////////////////////////////////////
	SEQ_SUB_TEMOTI_YES_NO,
	//////////////////////////////////////////////////////////////////
	SEQ_SUB_LEVEL_LIST,
	SEQ_SUB_LEVEL_YES_NO,
	//////////////////////////////////////////////////////////////////
	SEQ_SUB_INFO_LIST,
	SEQ_SUB_INFO_TUYOSA_YES_NO,
	SEQ_SUB_INFO_WAZA_YES_NO,
	SEQ_SUB_INFO_RANKUP_YES_NO,
	SEQ_SUB_INFO_RANKUP_DECIDE_EFF_WAIT,
	//////////////////////////////////////////////////////////////////
	SEQ_SUB_DECIDE_EFF_WAIT,				//����G�t�F�N�g�҂�
	SEQ_SUB_DECIDE_EFF_WAIT_NEXT_WIN_OFF,	//����G�t�F�N�g�҂����E�B���h�I�t��
	SEQ_SUB_DECIDE_EFF_WAIT_WIN_OFF,		//����G�t�F�N�g�҂����E�B���h�I�t�A�ŏ��̉�ʂɖ߂�
	//////////////////////////////////////////////////////////////////
	SEQ_SUB_INFO_KEY_WAIT_LIST_RETURN,		//�L�[�҂����āA���傤�ق����X�g�ɖ߂�
	SEQ_SUB_KEY_WAIT,						//�L�[�҂��A�ŏ��̉�ʂɖ߂�
	SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF,	//�L�[�҂��A�X�e�[�^�X�E�B���h�E�����ABASIC���X�g�֖߂�
	SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF,		//�L�[�҂��A�X�e�[�^�X�E�B���h�E�����ABASIC���X�g�֖߂�
};

static BOOL Seq_GameTypeSel( CASTLE_ENEMY_WORK* wk )
{
	u8 check_pos;
	u16 now_cp,check_cp;
	int i,ans;
	u32 color,now_rank,ret;
	POKEMON_PARAM* poke;

	switch( wk->sub_seq ){

	case SEQ_SUB_START:
		wk->recover = 0;
		wk->sub_seq = SEQ_SUB_TYPE_SEL;

		//�����N�A�b�v�������ɖ߂�悪���ꂩ�`�F�b�N
		if( wk->recover_flag == RECOVER_NORMAL ){
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Default_Write( wk );								//TALKMENU,���ǂ�

		//�����N�A�b�v�������ɖ߂�悪���ꂩ�`�F�b�N
		}else if( wk->recover_flag == RECOVER_RANKUP ){
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2

			//�����N�A�b�v�G�t�F�N�g
			CastleObj_RankUpEff( wk->p_eff_rankup, CASTLE_ENEMY_RANKUP_X, CASTLE_ENEMY_RANKUP_Y );

			wk->sub_seq = SEQ_SUB_INFO_LIST;

		//�ʐM�Ŏ������X�e�[�^�X���J���������t���O���`�F�b�N
		}else if( wk->recover_flag == RECOVER_STATUS ){
			wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT_WIN_OFF;
		}

		wk->recover_flag = RECOVER_NONE;			//TYPE_SEL�ɖ߂������̕���t���O�N���A
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�^�C�v��I��
	case SEQ_SUB_TYPE_SEL:
		CsrMove( wk, sys.trg );

		//����{�^������������
		if( sys.trg & PAD_BUTTON_A ){
			Snd_SePlay( SEQ_SE_DP_SELECT );

			//�u���ǂ�v
			if( wk->csr_pos >= wk->modoru_pos ){
				return TRUE;
			}else{
				CastleEnemy_Default_Del( wk );		//TALKMENU,���ǂ�
				CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
				wk->sub_seq = SEQ_SUB_MENU_MAIN;
				break;
			}

		}else if( sys.trg & PAD_BUTTON_B ){
			//���łɁu���ǂ�v�ʒu�̎��͉������Ȃ�
			if( wk->csr_pos != wk->modoru_pos ){
				Snd_SePlay( SEQ_SE_DP_SELECT );
				wk->csr_pos = wk->modoru_pos;
				CsrMoveSub( wk );
			}
			break;
		}

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//��邱�Ƃ�I��
	case SEQ_SUB_MENU_MAIN:
		ret = BmpListMain( wk->lw );
		Castle_ListSeCall( ret, SEQ_SE_DP_SELECT );
		BmpListDirectPosGet( wk->lw, &wk->list_csr_pos );					//�J�[�\���ʒu

		switch( ret ){

		case BMPLIST_NULL:
			break;

		case BMPLIST_CANCEL:
			CastleEnemy_Basic_Del( wk );			//TALKMENU2,BASIC
			CastleEnemy_Default_Write( wk );		//TALKMENU,���ǂ�
			wk->sub_seq = SEQ_SUB_START;
			break;

		//���傤����
		case FC_PARAM_TEMOTI:
			wk->parent_decide_type = ret;			//�p�����[�^

			//���̂����J����Ă��Ȃ�������
			if( wk->p_temoti_flag[GetCsrPokePos(wk->h_max,wk->csr_pos)] == 0 ){
				CastleEnemy_Basic_Del( wk );		//TALKMENU2,BASIC

				//�u�|�P�����́@���傤�������@�킩��܂��@���b�o���@�����@�܂����H�v
				CastleEnemy_SeqSubTalkWinOn( wk );
				Castle_SetNumber(	wk, 0, CP_USE_TEMOTI, CASTLE_KETA_CP, 
									NUMBER_DISPTYPE_LEFT );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_04, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_TEMOTI_YES_NO;
			}else{
				CastleEnemy_Basic_Del( wk );		//TALKMENU2,BASIC

				poke =  PokeParty_GetMemberPointer( wk->p_party, 
													GetCsrPokePos(wk->h_max,wk->csr_pos) );

				/*�u�|�P�����́@���傤�����́@���@�ł����I�v*/
				CastleEnemy_SeqSubTalkWinOn( wk );
				Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_04_03, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KEY_WAIT;
				//�������Ȃ�(���X�g�p��)
				//Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
				//Snd_SePlay( SEQ_SE_DP_BOX03 );
			}
			break;

		//���x��
		case FC_PARAM_LEVEL:
			wk->parent_decide_type = ret;		//�p�����[�^
			CastleEnemy_Basic_Del( wk );		//TALKMENU2,BASIC

			poke =  PokeParty_GetMemberPointer( wk->p_party, 
												GetCsrPokePos(wk->h_max,wk->csr_pos) );

			//�u���́@���x�����@�������܂��@�ǂ���Ɂ@���܂����H�v
			CastleEnemy_SeqSubTalkWinOn( wk );
			Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
			wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_05, FONT_TALK );
			Castle_SetMenu3( wk );				//�u���x���{�T�E���x���[�T�E��߂�v
			wk->sub_seq = SEQ_SUB_LEVEL_LIST;
			break;

		//���傤�ق�
		case FC_PARAM_INFO:
			CastleEnemy_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2
			wk->sub_seq = SEQ_SUB_INFO_LIST;
			break;

		//�Ƃ���
		case FC_PARAM_TOZIRU:
			CastleEnemy_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleEnemy_Default_Write( wk );	//TALKMENU,���ǂ�
			wk->sub_seq = SEQ_SUB_START;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�Ă����v�u�͂��E�������v���j���[����
	case SEQ_SUB_TEMOTI_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleEnemy_SeqSubMenuWinClear( wk );
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));

			//CP������Ȃ���
			if( now_cp < CP_USE_TEMOTI ){
				//���s�������Ă���̂ő��k
				CastleEnemy_SeqSubTalkWinOn( wk );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_03, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KEY_WAIT;
				break;
			}

			if( Castle_CommCheck(wk->type) == FALSE ){

				Castle_CPRecord_Sub( wk->fro_sv, wk->type, CP_USE_TEMOTI );
				CastleEnemy_SeqSubCPWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_TR1] );

				//�����X�^�[�{�[���̃A�C�R�����|�P�����A�C�R���ɂ���
				//���x���A���ʁA�̗͂�\��
				CastleEnemy_SeqSubTemoti( wk, wk->csr_pos );
			
				wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT;
			}else{
				wk->send_req = 1;
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleEnemy_SeqSubMenuWinClear( wk );
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u���x���v���j���[����
	case SEQ_SUB_LEVEL_LIST:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u���x���{�T�v
		case 0:
			CastleEnemy_SeqSubMenuWinClear( wk );
			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

			//���ł�+5���Ă�����o���Ȃ�
			if( wk->p_level_flag[GetCsrPokePos(wk->h_max,wk->csr_pos)] == 1 ){
				CastleEnemy_SeqSubTalkWinOn( wk );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_05_04, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KEY_WAIT;
				break;
			}else{
				CastleEnemy_SeqSubLevelMsg( wk, 1 );
				wk->sub_seq = SEQ_SUB_LEVEL_YES_NO;
			}
			break;
			
		//�u���x���|�T�v
		case 1:
			CastleEnemy_SeqSubMenuWinClear( wk );
			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

			//���ł�-5���Ă�����o���Ȃ�
			if( wk->p_level_flag[GetCsrPokePos(wk->h_max,wk->csr_pos)] == 2 ){
				CastleEnemy_SeqSubTalkWinOn( wk );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_05_05, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KEY_WAIT;
				break;
			}else{
				CastleEnemy_SeqSubLevelMsg( wk, 2 );
				wk->sub_seq = SEQ_SUB_LEVEL_YES_NO;
			}
			break;
			
		case 2:
		case BMPMENU_CANCEL:
			CastleEnemy_SeqSubMenuWinClear( wk );
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u���x���v�u�͂��E�������v���j���[����
	case SEQ_SUB_LEVEL_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleEnemy_SeqSubMenuWinClear( wk );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );
			check_cp = CastleEnemy_GetLvCP( wk->level_sel );			//�g�pCP

			//CP������Ȃ���
			if( now_cp < check_cp ){
				CastleEnemy_SeqSubTalkWinOn( wk );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_03, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KEY_WAIT;
				return FALSE;
			}

			//�������Ă����珈�����ς��Ƃ��낪�A�ʐM�A��ʐM�œ��ɂȂ��̂ŃZ�b�g���Ė��Ȃ�
			wk->parent_decide_updown = wk->level_sel;

			if( Castle_CommCheck(wk->type) == FALSE ){
				BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
				Castle_CPRecord_Sub(wk->fro_sv, wk->type,
									CastleEnemy_GetLvCP(wk->level_sel));
				CastleEnemy_SeqSubCPWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_TR1] );
				CastleEnemy_SeqSubLevel( wk, wk->csr_pos, wk->level_sel );
				wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT;
			}else{
				wk->send_req = 1;
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleEnemy_SeqSubMenuWinClear( wk );
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u���傤�ق��v���X�g����
	case SEQ_SUB_INFO_LIST:
		ret = BmpListMain( wk->lw );
		Castle_ListSeCall( ret, SEQ_SE_DP_SELECT );
		BmpListDirectPosGet( wk->lw, &wk->list_csr_pos );					//�J�[�\���ʒu

		switch( ret ){

		case BMPLIST_NULL:
			break;

		case BMPLIST_CANCEL:
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Basic2_Del( wk );		//TALK,BASIC2
			CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
			break;

		case FC_PARAM_TUYOSA:
			wk->parent_decide_type = ret;		//�p�����[�^
			CastleEnemy_Basic2_Del( wk );		//TALK,BASIC2

			//�悳�����J����Ă��Ȃ�������
			if( wk->p_tuyosa_flag[GetCsrPokePos(wk->h_max,wk->csr_pos)] == 0 ){
				//�u�|�P�����́@�悳���@�킩��܂��@���b�o���@�����@�܂����H�v
				CastleEnemy_SeqSubTalkWinOn( wk );
				Castle_SetNumber( wk, 0, CP_USE_TUYOSA, CASTLE_KETA_CP, NUMBER_DISPTYPE_LEFT);
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_06, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_INFO_TUYOSA_YES_NO;
			}else{
				CastleEnemy_SeqSubTuyosa( wk, wk->csr_pos );
				wk->sub_seq = SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF;
			}
			break;

		case FC_PARAM_WAZA:
			wk->parent_decide_type = ret;		//�p�����[�^
			CastleEnemy_Basic2_Del( wk );		//TALK,BASIC2

			//�����N������Ȃ���
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_INFO );
			if( now_rank == 1 ){
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_09_10, FONT_TALK );
				wk->sub_seq = SEQ_SUB_INFO_KEY_WAIT_LIST_RETURN;
				return FALSE;
			}

			//��b�E�B���h�E�\��
			//CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));

			//�Z�����J����Ă��Ȃ�������
			if( wk->p_waza_flag[GetCsrPokePos(wk->h_max,wk->csr_pos)] == 0 ){
				//�u�����Ă���@�킴���@�킩��܂��@���b�o���@�����@�܂����H�v
				CastleEnemy_SeqSubTalkWinOn( wk );
				Castle_SetNumber( wk, 0, CP_USE_WAZA, CASTLE_KETA_CP, NUMBER_DISPTYPE_LEFT);
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_07, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_INFO_WAZA_YES_NO;
			}else{
				CastleEnemy_SeqSubWaza( wk, wk->csr_pos );	//�Z���A�o�o����\��
				wk->sub_seq = SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF;
			}
			break;

		case FC_PARAM_INFO_RANKUP:
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_INFO );
				
			//�����N�A�b�v�l�`�w��I��
			//if( now_rank == CASTLE_RANK_MAX ){
			if( now_rank == (CASTLE_RANK_MAX-1) ){				//��������2���ō��Ȃ̂ŁI
				//�u����@�����傤�@�����N�́@������܂���I�v
				Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
				Snd_SePlay( SEQ_SE_DP_BOX03 );
				//wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_09_08, FONT_TALK );
				//wk->sub_seq = SEQ_SUB_INFO_KEY_WAIT_LIST_RETURN;
			}else{
				wk->parent_decide_type = ret;		//�p�����[�^
				CastleEnemy_Basic2_Del( wk );		//TALK,BASIC2

				//�K�v��CP�|�C���g�擾
				now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));

				/*�u���b�o���g���ă����N�������܂����H�v*/
				Castle_SetNumber(wk, 0, INFO_RANKUP_POINT, 
								 CASTLE_KETA_CP, NUMBER_DISPTYPE_LEFT );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_11_01, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_INFO_RANKUP_YES_NO;
			}
			break;
		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�悳�v�u�͂��E�������v���j���[����
	case SEQ_SUB_INFO_TUYOSA_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			ans = CastleEnemy_SeqSubTuyosaWazaYes( wk, CP_USE_TUYOSA, msg_castle_trainer_06_05 );
			if( ans == TRUE ){
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleEnemy_SeqSubMenuWinClear( wk );
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2
			wk->sub_seq = SEQ_SUB_INFO_LIST;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�킴�v�u�͂��E�������v���j���[����
	case SEQ_SUB_INFO_WAZA_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			ans = CastleEnemy_SeqSubTuyosaWazaYes( wk, CP_USE_WAZA, msg_castle_trainer_07_03 );
			if( ans == TRUE ){
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleEnemy_SeqSubMenuWinClear( wk );
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2
			wk->sub_seq = SEQ_SUB_INFO_LIST;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�����N�A�b�v�v�u�͂��E�������v���j���[����
	case SEQ_SUB_INFO_RANKUP_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleEnemy_SeqSubMenuWinClear( wk );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_INFO );

			//CP�̔�r
			if( now_cp < INFO_RANKUP_POINT ){
				//�u�b�o���@����܂���v
				CastleEnemy_SeqSubMenuWinClear( wk );
				CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_07_03, FONT_TALK );
				wk->sub_seq = SEQ_SUB_INFO_KEY_WAIT_LIST_RETURN;
				break;
			}
						
			//�����N�A�b�v
			if( Castle_CommCheck(wk->type) == FALSE ){
				CastleRank_SeqSubRankUp( wk, wk->csr_pos, FC_PARAM_INFO_RANKUP );
				wk->sub_seq = SEQ_SUB_INFO_RANKUP_DECIDE_EFF_WAIT;
				break;
			}else{
				wk->send_req		= 1;
				return TRUE;
			}

			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleEnemy_SeqSubMenuWinClear( wk );
			//BmpTalkWinClear( &wk->bmpwin[ENEMY_BMPWIN_TALK], WINDOW_TRANS_OFF );
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2
			wk->sub_seq = SEQ_SUB_INFO_LIST;
			break;
		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//��񃉃��N�A�b�v����G�t�F�N�g�҂�
	case SEQ_SUB_INFO_RANKUP_DECIDE_EFF_WAIT:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
		//if( CastleEnemy_DecideEff(wk,wk->csr_pos,wk->parent_decide_type) == TRUE ){
			CastleEnemy_SeqSubMenuWinClear( wk );
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2

			//�����N�A�b�v�G�t�F�N�g
			CastleObj_RankUpEff( wk->p_eff_rankup, CASTLE_ENEMY_RANKUP_X, CASTLE_ENEMY_RANKUP_Y );

			wk->sub_seq = SEQ_SUB_INFO_LIST;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//����G�t�F�N�g�҂�
	case SEQ_SUB_DECIDE_EFF_WAIT:
		if( CastleEnemy_DecideEff(wk,wk->csr_pos,wk->parent_decide_type) == TRUE ){
			wk->sub_seq = SEQ_SUB_KEY_WAIT;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//����G�t�F�N�g�҂����E�B���h�I�t��
	case SEQ_SUB_DECIDE_EFF_WAIT_NEXT_WIN_OFF:
		if( CastleEnemy_DecideEff(wk,wk->csr_pos,wk->parent_decide_type) == TRUE ){
			wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT_WIN_OFF;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//����G�t�F�N�g�҂����E�B���h�I�t���čŏ��̉�ʂɖ߂�
	case SEQ_SUB_DECIDE_EFF_WAIT_WIN_OFF:
		if( wk->parent_decide_type == FC_PARAM_TUYOSA ){
			wk->sub_seq = SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF;
		}else{
			wk->sub_seq = SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�L�[�҂����āA���傤�ق����X�g�ɖ߂�
	case SEQ_SUB_INFO_KEY_WAIT_LIST_RETURN:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleEnemy_Basic2_Write( wk );		//TALK,BASIC2
			wk->sub_seq = SEQ_SUB_INFO_LIST;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�L�[�҂�
	case SEQ_SUB_KEY_WAIT:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			CastleEnemy_Default_Write( wk );		//TALKMENU,���ǂ�
			wk->sub_seq = SEQ_SUB_START;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�悳�F�L�[�҂����ăX�e�[�^�X�Ȃǂ̃E�B���h�E�������āABASIC���X�g�֖߂�
	case SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF:
		if( sys.trg & PAD_KEY_LEFT ){
			CastleEnemy_TuyosaWinChg( wk, -1 );
		}else if( sys.trg & PAD_KEY_RIGHT ){
			CastleEnemy_TuyosaWinChg( wk, 1 );
		}else if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleEnemy_SeqSubStatusWinDel( wk );
			CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�킴�F�L�[�҂����ăX�e�[�^�X�Ȃǂ̃E�B���h�E�������āABASIC���X�g�֖߂�
	case SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF:
		if( sys.trg & PAD_KEY_LEFT ){
			CastleEnemy_WazaWinChg( wk, -1 );
		}else if( sys.trg & PAD_KEY_RIGHT ){
			CastleEnemy_WazaWinChg( wk, 1 );
		}else if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleEnemy_SeqSubStatusWinDel( wk );
			CastleEnemy_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	����M
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameSendRecv( CASTLE_ENEMY_WORK* wk )
{
	u8 sel_pos;

	switch( wk->sub_seq ){

	case 0:
		wk->recover_flag = RECOVER_NORMAL;	//�����N�A�b�v�������ɖ߂�悪����t���OOFF

		//�I�񂾃��N�G�X�g(��肽������)�𑗐M
		if( CastleEnemy_CommSetSendBuf(wk,CASTLE_COMM_ENEMY_UP_TYPE,wk->csr_pos) == TRUE ){
			wk->send_req = 0;
			wk->sub_seq++;
		}
		break;

	case 1:
		if( wk->parent_decide_pos == CASTLE_ENEMY_DECIDE_NONE ){
			break;
		}

		//�e�����߂������N��LR�������N�A�b�v������
		wk->recieve_count = 0;

		//�����N���グ�鏈���Ƃ���ȊO�ŕ����Ă���
		if( wk->parent_decide_type == FC_PARAM_INFO_RANKUP ){ 
			CastleRank_SeqSubRankUp( wk, wk->parent_decide_pos, FC_PARAM_INFO_RANKUP );
		}else{
			CastleEnemy_SeqSubRankUp( wk, wk->parent_decide_pos, wk->parent_decide_type );
		}

		wk->sub_seq++;
		break;

	//����G�t�F�N�g�҂�
	case 2:
		sel_pos = GetCommSelCsrPos( wk->modoru_pos, wk->parent_decide_pos );	//�J�[�\���ʒu�擾
		if( CastleEnemy_DecideEff(wk,sel_pos,wk->parent_decide_type) == TRUE ){
			wk->wait = CASTLE_COMM_WAIT;
			wk->sub_seq++;
		}
		break;

	case 3:
		//if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
		//Snd_SePlay( SEQ_SE_DP_SELECT );
		wk->wait--;
		if( wk->wait == 0 ){
			//�����J�n
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_CASTLE_ENEMY_UP );
			wk->sub_seq++;
			break;
		}
		break;

	//����
	case 4:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_CASTLE_ENEMY_UP) == TRUE ){
			wk->sub_seq++;
		}
		break;

	case 5:
		//�G�t�F�N�g�^�C�~���O��2�ӏ��ɂȂ����̂Œǉ�
		sel_pos = GetCommSelCsrPos( wk->modoru_pos, wk->parent_decide_pos );	//�J�[�\���ʒu�擾
		if( CastleEnemy_DecideEff2(wk,sel_pos,wk->parent_decide_type) == TRUE ){
			CommToolTempDataReset();
			CommToolInitialize( HEAPID_CASTLE );	//timingSyncEnd=0xff

			//08.03.28�폜
			//BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
			//CastleEnemy_SeqSubStatusWinDel( wk );

			//CastleEnemy_SeqSubAllInfoWrite( wk );								//���X�V
			wk->parent_decide_pos = CASTLE_ENEMY_DECIDE_NONE;

#if 0
			//08.03.28�폜
			//�����N�A�b�v�������ɖ߂�悪���ꂩ�`�F�b�N
			if( wk->recover_flag == RECOVER_NORMAL ){
				CastleEnemy_Default_Write( wk );								//TALKMENU,���ǂ�
			}
#endif

			//�u�߂飃��N�G�X�g����ɂ��Ă��Ă��A�Ȃ��������Ƃɂ���
			wk->pair_modoru_flag = 0;
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEndMulti( CASTLE_ENEMY_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//�u�߂�v���M
	case 0:
		if( CastleEnemy_CommSetSendBuf(wk,CASTLE_COMM_ENEMY_MODORU,0) == TRUE ){
			wk->wait = CASTLE_MODORU_WAIT;
			wk->sub_seq++;
		}
		break;

	//�����J�n
	case 1:
		if( wk->wait > 0 ){
			wk->wait--;
		}

		if( wk->wait == 0 ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_CASTLE_ENEMY_END );
			wk->sub_seq++;
		}
		break;

	//�����҂�
	case 2:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_CASTLE_ENEMY_END) == TRUE ){
			CommToolTempDataReset();
			BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( CASTLE_ENEMY_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//�t�F�[�h�A�E�g
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
					WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_CASTLE );

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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleCommon_Delete( CASTLE_ENEMY_WORK* wk )
{
	u8 e_max;
	int i,j;

	CastleObj_Delete( wk->p_poke_sel );
	CastleObj_Delete( wk->p_pair_poke_sel );
	//CastleObj_Delete( wk->p_eff );		//����Ȃ�
	CastleObj_Delete( wk->p_eff_rankup );
	
	//�悳�A�킴
	for( i=0; i < CASTLE_COMM_POKE_TOTAL_NUM ;i++ ){
		for( j=0; j < 2 ;j++ ){
			CastleObj_Delete( wk->p_hata[i][j] );
		}
	}

	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );
	for( i=0; i < e_max ;i++ ){
		CastleObj_Delete( wk->p_ball[i] );
		CastleObj_Delete( wk->p_icon[i] );
		CastleObj_Delete( wk->p_hp[i] );
	}

	//�ʐM�A�C�R���폜
	WirelessIconEasyEnd();

	//�p���b�g�t�F�[�h�J��
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );

	//�p���b�g�t�F�[�h�V�X�e���J��
	PaletteFadeFree( wk->pfd );
	wk->pfd = NULL;

	CastleClact_DeleteCellObject(&wk->castle_clact);		//2D�I�u�W�F�N�g�֘A�̈�J��

	MSGMAN_Delete( wk->msgman );						//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );						//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );						//���b�Z�[�W�o�b�t�@�J��
	NUMFONT_Delete( wk->num_font );

	for( i=0; i < CASTLE_MENU_BUF_MAX ;i++ ){
		STRBUF_Delete( wk->menu_buf[i] );				//���j���[�o�b�t�@�J��
	}

	CastleExitBmpWin( wk->bmpwin, CASTLE_BMP_ENEMY );	//BMP�E�B���h�E�J��
	Castle_BgExit( wk->bgl );							//BGL�폜
	ArchiveDataHandleClose( wk->hdl );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���A
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_Recover( CASTLE_ENEMY_WORK* wk )
{
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������1
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_InitSub1( void )
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_InitSub2( CASTLE_ENEMY_WORK* wk )
{
	u32 start_x,start_y;
	u32 hp,hp_max;
	u8 e_max;
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,icon_offset_x,ball_offset_x;
	int i,j,flip;
	GF_BGL_BMPWIN* win;

	wk->hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_CASTLE );
	Castle_BgInit( wk );								//BG������
	Castle_ObjInit( wk );								//OBJ������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_castle_trainer_dat, HEAPID_CASTLE );

	wk->wordset = WORDSET_Create( HEAPID_CASTLE );
	wk->msg_buf = STRBUF_Create( CASTLE_MSG_BUF_SIZE, HEAPID_CASTLE );
	wk->tmp_buf = STRBUF_Create( CASTLE_MSG_BUF_SIZE, HEAPID_CASTLE );

	//������o�b�t�@�쐬
	for( i=0; i < CASTLE_MENU_BUF_MAX ;i++ ){
		wk->menu_buf[i] = STRBUF_Create( CASTLE_MENU_BUF_SIZE, HEAPID_CASTLE );
	}

	//�t�H���g�p���b�g
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, BC_FONT_PAL * 32, HEAPID_CASTLE );
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, BC_MSGFONT_PAL * 32, HEAPID_CASTLE );

	//8x8�t�H���g�쐬(�g�pbmpwin[ENEMY_BMPWIN_TYPE]�̃p���b�g(BC_FONT_PAL)���g�p
	//wk->num_font = NUMFONT_Create( 15, 14, FBMP_COL_NULL, HEAPID_CASTLE );
	wk->num_font = NUMFONT_Create( FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, HEAPID_CASTLE );

	//�r�b�g�}�b�v�ǉ�
	CastleAddBmpWin( wk->bgl, wk->bmpwin, CASTLE_BMP_ENEMY );

	//�ʐM���Ă��邩�ŕ\���̃I�t�Z�b�g���ς��
	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	////////////////////////////////////////////////////////////////////////////////////////
	if( Castle_CommCheck(wk->type) == FALSE ){
		icon_offset_x = ICON_START_X;
		ball_offset_x = BALL_START_X;
	}else{
		icon_offset_x = ICON_MULTI_START_X;
		ball_offset_x = BALL_MULTI_START_X;
	}

	//�{�[��OBJ
	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );
	for( i=0; i < e_max ;i++ ){
		wk->p_ball[i] = CastleObj_Create(	&wk->castle_clact, 
											ID_CHAR_CSR, ID_PLTT_CSR,
											ID_CELL_CSR, CASTLE_ANM_BALL_MOVE, 
											BALL_WIDTH_X*i+ball_offset_x, 
											BALL_START_Y, CASTLE_BG_PRI_LOW, NULL );

		//HP���OBJ�ǉ�
		wk->p_hp[i] = CastleObj_Create(	&wk->castle_clact,
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, CASTLE_ANM_HP_MIDORI,
										ICON_WIDTH_X*i+icon_offset_x, 
										ICON_START_Y+20, CASTLE_BG_PRI_BACK, NULL );

		//�|�P�����A�C�R��OBJ�ǉ�
		wk->p_icon[i] = CastleObj_Create(	&wk->castle_clact, 
											ID_CHAR_ICON1+i, ID_PLTT_ICON,
											ID_CELL_ICON, POKEICON_ANM_HPMAX,
											ICON_WIDTH_X*i+icon_offset_x, 
											ICON_START_Y, CASTLE_BG_PRI_LOW, NULL );

		//�|�P�����A�C�R���̃p���b�g�؂�ւ�
		CastleObj_IconPalChg( wk->p_icon[i], PokeParty_GetMemberPointer(wk->p_party,i) );

		//�莝�������J����Ă��Ȃ�������
		if( wk->p_temoti_flag[i] == 0 ){
			CastleObj_Vanish( wk->p_ball[i], CASTLE_VANISH_OFF );	//�{�[���\��
			CastleObj_Vanish( wk->p_icon[i], CASTLE_VANISH_ON );	//�|�P�A�C�R����\��
			CastleObj_Vanish( wk->p_hp[i], CASTLE_VANISH_ON );		//HP��ԃA�C�R����\��
		}else{
			CastleObj_Vanish( wk->p_ball[i], CASTLE_VANISH_ON );	//�{�[����\��
			CastleObj_Vanish( wk->p_icon[i], CASTLE_VANISH_OFF );	//�|�P�A�C�R���\��
			CastleObj_Vanish( wk->p_hp[i], CASTLE_VANISH_OFF );		//HP��ԃA�C�R���\��
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//�|�P�����I���E�B��OBJ�ǉ�
	CastleEnemy_GetPokeSelXY( wk, &start_x, &start_y, 0 );
	wk->p_poke_sel = CastleObj_Create(	&wk->castle_clact, 
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, CASTLE_ANM_SELECT_ZIBUN, 
										start_x, start_y, 
										CASTLE_BG_PRI_LOW, NULL );

	wk->p_pair_poke_sel = CastleObj_Create(	&wk->castle_clact, 
											ID_CHAR_CSR, ID_PLTT_CSR, 
											ID_CELL_CSR, CASTLE_ANM_SELECT_AITE, 
											start_x, start_y, 
											CASTLE_BG_PRI_LOW, NULL );

	if( Castle_CommCheck(wk->type) == FALSE ){
		CastleObj_Vanish( wk->p_pair_poke_sel, CASTLE_VANISH_ON );			//��\��
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//��OBJ�ǉ�
	for( i=0; i < CASTLE_COMM_POKE_TOTAL_NUM ;i++ ){
		for( j=0; j < 2 ;j++ ){
			Castle_GetHataXY( wk, &start_x, &start_y );

			wk->p_hata[i][j] = CastleObj_Create(&wk->castle_clact, 
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, CASTLE_ANM_HATA, 
										start_x + (CASTLE_HATA_WIDTH_X * i), 
										start_y + (CASTLE_HATA_WIDTH_Y * j), 
										CASTLE_BG_PRI_LOW, NULL );

			CastleObj_Vanish( wk->p_hata[i][j], CASTLE_VANISH_ON );			//��\��
		}
	}

	//�悳�A�킴�����J����Ă��������\������
	CastleRank_SeqSubHataVanish( wk );

	////////////////////////////////////////////////////////////////////////////////////////
	//�����N�A�b�vOBJ�ǉ�
	wk->p_eff_rankup = CastleObj_Create(&wk->castle_clact, 
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, CASTLE_ANM_RANK_UP, 
										20, 20,			//�l�K��
										CASTLE_BG_PRI_HIGH, NULL );
	CastleObj_Vanish( wk->p_eff_rankup, CASTLE_VANISH_ON );					//��\��

	////////////////////////////////////////////////////////////////////////////////////////
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_BgInit( CASTLE_ENEMY_WORK* wk )
{
	SetVramBank();
	SetBgHeader( wk->bgl );

	//�p���b�g�t�F�[�h�V�X�e�����[�N�쐬
	wk->pfd = PaletteFadeInit( HEAPID_CASTLE );

	//���N�G�X�g�f�[�^��malloc���ăZ�b�g
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_OBJ, FADE_PAL_ALL_SIZE, HEAPID_CASTLE );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ALL_SIZE, HEAPID_CASTLE );

	//���ʔw�i
	Castle_SetMainBgGraphic( wk, BC_FRAME_BG );
	Castle_SetMainBgPalette();
	Castle_SetStatusBgGraphic( wk, BC_FRAME_SLIDE );
	//Castle_SetWazaBgGraphic( wk, BC_FRAME_SLIDE );
	Castle_SetStatusBgPalette();
	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );		//BG��\��
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��

	//����ʔw�i�A�p���b�g�Z�b�g
	Castle_SetSubBgGraphic( wk, BC_FRAME_SUB );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�֘A������
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_ObjInit( CASTLE_ENEMY_WORK* wk )
{
	//CastleClact_InitCellActor( &wk->castle_clact );
	CastleClact_InitCellActor(	&wk->castle_clact, 
								//PokeParty_GetMemberPointer(wk->p_party,0) );
								//wk->p_party, wk->type );
								wk->p_party, Castle_CommCheck(wk->type) );
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
static void Castle_BgExit( GF_BGL_INI * ini )
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
	GF_BGL_BGControlExit( ini, BC_FRAME_BG );			//3
	GF_BGL_BGControlExit( ini, BC_FRAME_SLIDE );
	GF_BGL_BGControlExit( ini, BC_FRAME_TYPE );
	GF_BGL_BGControlExit( ini, BC_FRAME_WIN );
	GF_BGL_BGControlExit( ini, BC_FRAME_SUB );

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
	CASTLE_ENEMY_WORK* wk = work;

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
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BC_FRAME_WIN, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BC_FRAME_WIN, 32, 0, HEAPID_CASTLE );
		GF_BGL_ScrClear( ini, BC_FRAME_WIN );
	}

	{	//BG(�t�H���g)(�^�C�v�\��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BC_FRAME_TYPE, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BC_FRAME_TYPE, 32, 0, HEAPID_CASTLE );
		GF_BGL_ScrClear( ini, BC_FRAME_TYPE );
	}

	{	//����(�X�e�[�^�X)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			//GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BC_FRAME_SLIDE, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BC_FRAME_SLIDE, 32, 0, HEAPID_CASTLE );
		GF_BGL_ScrClear( ini, BC_FRAME_SLIDE );
	}

	{	//����(SINGLE,DOUBLE)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x08000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BC_FRAME_BG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( BC_FRAME_BG, 32, 0, HEAPID_CASTLE );
		GF_BGL_ScrClear( ini, BC_FRAME_BG );
	}

	//--------------------------------------------------------------------------------
	{	//�����(�{�[��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x10000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, BC_FRAME_SUB, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, BC_FRAME_SUB );
	}

	G2_SetBG0Priority( 0 );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetMainBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TAISEN_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	if( Castle_CommCheck(wk->type) == FALSE ){
		ArcUtil_HDL_ScrnSet(wk->hdl, BC_TAISEN01_NSCR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	}else{
		ArcUtil_HDL_ScrnSet(wk->hdl, BC_TAISEN02_NSCR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	}
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
static void Castle_SetMainBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BC_TAISEN_NCLR, &dat, HEAPID_CASTLE );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*4) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*4) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʃX�e�[�^�X
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetStatusBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TAISEN_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BC_TAISEN_STATUS_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���ʃX�e�[�^�X�p���b�g�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetStatusBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BC_TAISEN_NCLR, &dat, HEAPID_CASTLE );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*4) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*4) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʂ킴
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetWazaBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TAISEN_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BC_TAISEN_WAZA_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i�A�p���b�g�Z�b�g
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetSubBgGraphic( CASTLE_ENEMY_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BATT_FRONTIER_NCGR_BIN, wk->bgl, frm,
							0, 0, TRUE, HEAPID_CASTLE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BATT_FRONTIER_NSCR_BIN, wk->bgl, frm,
						0, 0, TRUE, HEAPID_CASTLE );

	ArcUtil_HDL_PalSet(	wk->hdl, BATT_FRONTIER_NCLR, PALTYPE_SUB_BG,
						0, 0x20, HEAPID_CASTLE );
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
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
static u8 CastleWriteMsg( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	u8 msg_index;

	GF_BGL_BmpWinDataFill( win, b_col );			//�h��Ԃ�
	MSGMAN_GetString( wk->msgman, msg_id, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	msg_index = GF_STR_PrintColor( win, font, wk->msg_buf, x, y, wait, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	GF_BGL_BmpWinOnVReq( win );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�\��(�h��Ԃ��Ȃ�)
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
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
static u8 CastleWriteMsgSimple( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	u8 msg_index;

	MSGMAN_GetString( wk->msgman, msg_id, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	msg_index = GF_STR_PrintColor( win, font, wk->msg_buf, x, y, wait, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	GF_BGL_BmpWinOnVReq( win );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	�ȒP���b�Z�[�W�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Castle_EasyMsg( CASTLE_ENEMY_WORK* wk, int msg_id, u8 font )
{
	u8 msg_index;

	msg_index = CastleWriteMsg(wk, &wk->bmpwin[ENEMY_BMPWIN_TALK], msg_id, 
								1, 1, MSG_NO_PUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, font );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	�X�e�[�^�X���b�Z�[�W�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
enum{
	//��������
	STATUS_MOTIMONO_X =		(1*8),
	STATUS_MOTIMONO_Y =		(1*8),
	STATUS_MOTIMONO_NUM_X =	(9*8),

	//���������@��
	STATUS_SEIKAKU_X =		(1*8),
	STATUS_SEIKAKU_Y =		(3*8),
	STATUS_SEIKAKU_NUM_X =	(9*8),

	//�Ƃ�����
	STATUS_TOKUSEI_X =		(1*8),
	STATUS_TOKUSEI_Y =		(5*8),
	STATUS_TOKUSEI_NUM_X =	(9*8),

	//��������
	STATUS_KOUGEKI_X =		(1*8),
	STATUS_KOUGEKI_Y =		(7*8),
	STATUS_KOUGEKI_NUM_X =	(8*8),

	//�ڂ�����
	STATUS_BOUGYO_X =		(12*8),
	STATUS_BOUGYO_Y =		(STATUS_KOUGEKI_Y),
	STATUS_BOUGYO_NUM_X =	(19*8),

	//�Ƃ�����
	STATUS_TOKUKOU_X =		(1*8),
	STATUS_TOKUKOU_Y =		(9*8),
	STATUS_TOKUKOU_NUM_X =	(STATUS_KOUGEKI_NUM_X),

	//�Ƃ��ڂ�
	STATUS_TOKUBOU_X =		(12*8),
	STATUS_TOKUBOU_Y =		(STATUS_TOKUKOU_Y),
	STATUS_TOKUBOU_NUM_X =	(STATUS_BOUGYO_NUM_X),

	//���΂₳
	STATUS_SUBAYASA_X =		(1*8),
	STATUS_SUBAYASA_Y =		(11*8),
	STATUS_SUBAYASA_NUM_X =	(STATUS_KOUGEKI_NUM_X),
};

static void Castle_StatusMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke )
{
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );	//�h��Ԃ�

	//�u�������́v
	WORDSET_RegisterItemName( wk->wordset, 0, PokeParaGet(poke,ID_PARA_item,NULL) );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_01, STATUS_MOTIMONO_X, STATUS_MOTIMONO_Y );
	StMsgWriteSub( wk, win,msg_castle_trainer_status_01_01,STATUS_MOTIMONO_NUM_X,STATUS_MOTIMONO_Y);

	//�u���������v
	WORDSET_RegisterSeikaku( wk->wordset, 0, PokeSeikakuGet(poke) );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_02, STATUS_SEIKAKU_X, STATUS_SEIKAKU_Y );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_02_01, STATUS_SEIKAKU_NUM_X,STATUS_SEIKAKU_Y);

	//�u�Ƃ������v
	WORDSET_RegisterTokuseiName( wk->wordset, 0, PokeParaGet(poke,ID_PARA_speabino,NULL) );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_03, STATUS_TOKUSEI_X, STATUS_TOKUSEI_Y );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_03_01, STATUS_TOKUSEI_NUM_X,STATUS_TOKUSEI_Y);

	//�u���������v
	//OS_Printf( "pow = %d\n", PokeParaGet(poke,ID_PARA_pow,NULL) );
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_pow,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_04, STATUS_KOUGEKI_X, STATUS_KOUGEKI_Y );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_04_01, STATUS_KOUGEKI_NUM_X,STATUS_KOUGEKI_Y);

	//�u�ڂ�����v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_def,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_05, STATUS_BOUGYO_X, STATUS_BOUGYO_Y );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_05_01, STATUS_BOUGYO_NUM_X, STATUS_BOUGYO_Y );

	//�u�Ƃ������v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_spepow,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_06, STATUS_TOKUKOU_X, STATUS_TOKUKOU_Y );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_06_01, STATUS_TOKUKOU_NUM_X,STATUS_TOKUKOU_Y);

	//�u�Ƃ��ڂ��v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_spedef,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_07, STATUS_TOKUBOU_X, STATUS_TOKUBOU_Y );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_07_01, STATUS_TOKUBOU_NUM_X,STATUS_TOKUBOU_Y);

	//�u���΂₳�v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_agi,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_trainer_status_08, STATUS_SUBAYASA_X, STATUS_SUBAYASA_Y );
	StMsgWriteSub( wk, win,msg_castle_trainer_status_08_01,STATUS_SUBAYASA_NUM_X,STATUS_SUBAYASA_Y);

	GF_BGL_BmpWinOnVReq( win );
	return;
}

static void StMsgWriteSub( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, int msg, u16 x, u16 y )
{
	CastleWriteMsgSimple(	wk, win, msg, x, y, MSG_NO_PUT,
							FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z���b�Z�[�W�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
enum{
	//�Z��
	STATUS_WAZA_X =			(3*8),
	STATUS_WAZA_Y =			(2*8),

	//�Z�|�C���g
	STATUS_POINT_X =		(12*8),
	STATUS_POINT_Y =		(2*8),
};

static void Castle_WazaMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke )
{
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );	//�h��Ԃ�

	//id+i�͕ۏ؂���邩�킩��Ȃ��̂ŁA���Ȃ��B

	WazaMsgWriteSub(	wk, win, 0, msg_castle_trainer_waza_01_01, 
						msg_castle_trainer_waza_02, poke, ID_PARA_waza1, 
						ID_PARA_pp1, ID_PARA_pp_max1 );

	WazaMsgWriteSub(	wk, win, 1, msg_castle_trainer_waza_01_02, 
						msg_castle_trainer_waza_02, poke, ID_PARA_waza2, 
						ID_PARA_pp2, ID_PARA_pp_max2 );

	WazaMsgWriteSub(	wk, win, 2, msg_castle_trainer_waza_01_03, 
						msg_castle_trainer_waza_02, poke, ID_PARA_waza3, 
						ID_PARA_pp3, ID_PARA_pp_max3 );

	WazaMsgWriteSub(	wk, win, 3, msg_castle_trainer_waza_01_04, 
						msg_castle_trainer_waza_02, poke, ID_PARA_waza4, 
						ID_PARA_pp4, ID_PARA_pp_max4 );

	GF_BGL_BmpWinOnVReq( win );
	return;
}

static void WazaMsgWriteSub( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u32 msg_id, u32 msg_id2, POKEMON_PARAM* poke, u32 id, u32 id2, u32 id3 )
{
	u16 x,y,point_x,point_y;

	x = STATUS_WAZA_X;
	y = STATUS_WAZA_Y + (no * 24);
	point_x = STATUS_POINT_X;
	point_y = STATUS_POINT_Y + (no * 24);

	//�u�Z���v
	//WORDSET_RegisterWazaName( wk->wordset, 0, PokeParaGet(poke,id,NULL) );
	WORDSET_RegisterWazaName( wk->wordset, no, PokeParaGet(poke,id,NULL) );
	wk->msg_index = CastleWriteMsgSimple( wk, win,
									msg_id, x, y, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

	//�u�Z�|�C���g�v
	//Castle_SetNumber( wk, 0, PokeParaGet(poke,id2,NULL), CASTLE_KETA_STATUS );
	//Castle_SetNumber( wk, 1, PokeParaGet(poke,id3,NULL), CASTLE_KETA_STATUS );
	Castle_SetNumber(	wk, 4, PokeParaGet(poke,id2,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	Castle_SetNumber(	wk, 5, PokeParaGet(poke,id3,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_LEFT );
	wk->msg_index = CastleWriteMsgSimple( wk, win,
									msg_id2, point_x, point_y, MSG_NO_PUT,
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * HP��MHP�̃X���b�V���\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeHpMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 hp_offset_x;
	u8 e_max;
	int i;
	POKEMON_PARAM* poke;

	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );	//�h��Ԃ�

	if( Castle_CommCheck(wk->type) == FALSE ){
		hp_offset_x = CASTLE_HP_START_X;
	}else{
		hp_offset_x = CASTLE_MULTI_HP_START_X;
	}

	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=0;i < e_max ;i++ ){

		//���̂����J����Ă��Ȃ������牽���\�����Ȃ�
		if( wk->p_temoti_flag[i] == 0 ){
			GF_BGL_BmpWinFill(	win, FBMP_COL_NULL, 
								hp_offset_x + (CASTLE_HP_WIDTH_X * i), CASTLE_HP_START_Y,
								CASTLE_HP_WIDTH_X, 2*8 );

			continue;
		}

		poke =  PokeParty_GetMemberPointer( wk->p_party, i );

		//HP�͉E�Â�
		NUMFONT_WriteNumber(wk->num_font, PokeParaGet(poke,ID_PARA_hp,NULL), CASTLE_KETA_HP, 
							NUMFONT_MODE_SPACE, win, 
							hp_offset_x + (CASTLE_HP_WIDTH_X * i), CASTLE_HP_START_Y );

		//HP��MHP�̃X���b�V��(�L���`��)
		NUMFONT_WriteMark(	wk->num_font, NUMFONT_MARK_SLASH, win, 
							CASTLE_HPSLASH_START_X + hp_offset_x + (CASTLE_HP_WIDTH_X * i),
							CASTLE_HP_START_Y );

		//HPMAX�͍��Â�
		NUMFONT_WriteNumber(wk->num_font, PokeParaGet(poke,ID_PARA_hpmax,NULL), CASTLE_KETA_HP, 
							NUMFONT_MODE_LEFT, win, 
							CASTLE_HPMAX_START_X + hp_offset_x + (CASTLE_HP_WIDTH_X * i), 
							CASTLE_HP_START_Y );
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���x���\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeLvMsgWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win )
{
	u32 sex;
	u16 lv_offset_x,sex_offset_x;
	u8 e_max;
	int i;
	POKEMON_PARAM* poke;

	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );	//�h��Ԃ�

	if( Castle_CommCheck(wk->type) == FALSE ){
		lv_offset_x = CASTLE_LV_START_X;
		sex_offset_x = CASTLE_SEX_START_X;
	}else{
		lv_offset_x = CASTLE_MULTI_LV_START_X;
		sex_offset_x = CASTLE_MULTI_SEX_START_X;
	}

	e_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=0;i < e_max ;i++ ){
		poke =  PokeParty_GetMemberPointer( wk->p_party, i );

		//LV(�L���Ɛ������Z�b�g�ŕ`��)
		NUMFONT_WriteSet(	wk->num_font, NUMFONT_MARK_LV, PokeParaGet(poke,ID_PARA_level,NULL),
							CASTLE_KETA_LV, NUMFONT_MODE_LEFT, win,
							lv_offset_x + (CASTLE_LV_WIDTH_X * i), CASTLE_LV_START_Y );

		//���ʕ\��
		sex = PokeParaGet( poke, ID_PARA_sex, NULL );
		PokeSexWrite(	wk, win, sex_offset_x + (CASTLE_SEX_WIDTH_X * i), CASTLE_SEX_START_Y, 
						BC_FONT, sex );

	}

	GF_BGL_BmpWinOnVReq( win );
	return;
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	win		GF_BGL_BMPWIN�^�̃|�C���^
 * @param	y_max	���ڍő吔
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void CastleInitMenu( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max )
{
	int i;

	for( i=0; i < CASTLE_MENU_BUF_MAX ;i++ ){
		wk->Data[i].str	  = NULL;
		wk->Data[i].param = 0;
	}

	wk->MenuH.menu		= wk->Data;
	wk->MenuH.win		= win;
	wk->MenuH.font		= BC_FONT;
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	no		�Z�b�g����ꏊ(�߂�l)
 * @param	param	�߂�l
 * @param	msg_id	���b�Z�[�WID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void CastleSetMenuData( CASTLE_ENEMY_WORK* wk, u8 no, u8 param, int msg_id )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( no < CASTLE_MENU_BUF_MAX, "���j���[���ڐ��I�[�o�[�I" );

	MSGMAN_GetString( wk->msgman, msg_id, wk->menu_buf[no] );

	wk->Data[ no ].str = (const void *)wk->menu_buf[no];
	wk->Data[ no ].param = param;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g2�u�͂��A�������v
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetMenu2( CASTLE_ENEMY_WORK* wk )
{
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_YESNO] );
	CastleInitMenu( wk, &wk->bmpwin[ENEMY_BMPWIN_YESNO], 2 );
	CastleSetMenuData( wk, 0, 0, msg_castle_trainer_02_01 );				//�͂�
	CastleSetMenuData( wk, 1, 1, msg_castle_trainer_02_02 );				//������
	wk->mw = BmpMenuAddEx( &wk->MenuH, 8, 0, 0, HEAPID_CASTLE, PAD_BUTTON_CANCEL );
	wk->menu_flag = 1;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ���j���[�Z�b�g3�u���x���{�T�A���x���[�T�A��߂�v
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetMenu3( CASTLE_ENEMY_WORK* wk )
{
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_SEL] );
	CastleInitMenu( wk, &wk->bmpwin[ENEMY_BMPWIN_SEL], 3 );
	CastleSetMenuData( wk, 0, 0, msg_castle_trainer_05_01 );				//���x���{�T
	CastleSetMenuData( wk, 1, 1, msg_castle_trainer_05_02 );				//���x���[�T
	CastleSetMenuData( wk, 2, 2, msg_castle_trainer_05_03 );				//��߂�
	wk->mw = BmpMenuAddEx( &wk->MenuH, 8, 0, 0, HEAPID_CASTLE, PAD_BUTTON_CANCEL );
	wk->menu_flag = 1;
	return;
}


//==============================================================================================
//
//	�u���傤�����A���x���A���傤�ق��A�Ƃ���v
//
//==============================================================================================
static const BMPLIST_HEADER CastleListH = {
	NULL,						//�\�������f�[�^�|�C���^

	NULL,						//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,						//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,						//GF_BGL_BMPWIN�̃|�C���^

	0,							//���X�g���ڐ�
	0,							//�\���ő區�ڐ�

	0,							//���x���\���w���W
	8,							//���ڕ\���w���W
	0,							//�J�[�\���\���w���W
	0,							//�\���x���W
	FBMP_COL_BLACK,				//�����F
	FBMP_COL_NULL,				//�w�i�F
	FBMP_COL_BLK_SDW,			//�����e�F
	0,							//�����Ԋu�w
	16,							//�����Ԋu�x
	BMPLIST_NO_SKIP,			//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,				//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0,							//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
	NULL,
};

static const u32 basic_msg_tbl[][2] = {
	{ msg_castle_trainer_choice_01,	FC_PARAM_TEMOTI },	//����ׂ�
	{ msg_castle_trainer_choice_02,	FC_PARAM_LEVEL },	//���x��
	{ msg_castle_trainer_choice_03,	FC_PARAM_INFO },	//���傤�ق�
	{ msg_castle_trainer_choice_04,	BMPLIST_CANCEL },	//�Ƃ���
};
#define BASIC_MSG_TBL_MAX	( NELEMS(basic_msg_tbl) )

static const u32 basic_under_msg_tbl[] = {
	msg_castle_trainer_00_03,	//����ׂ�
	msg_castle_trainer_00_04,	//���x��
	msg_castle_trainer_00_05,	//���傤�ق�
	msg_castle_trainer_00_06,	//�Ƃ���
};

//--------------------------------------------------------------
/**
 * @brief	�u����ׂ�A���x���A���傤�ق��A�Ƃ���v���X�g�쐬
 *
 * @param	wk			CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_BasicListMake( CASTLE_ENEMY_WORK* wk )
{
	int i;
	BMPLIST_HEADER list_h;

	//���j���[�E�B���h�E��`��
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_BASIC_LIST] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[ENEMY_BMPWIN_BASIC_LIST], FBMP_COL_WHITE );	//�h��Ԃ�

	wk->menulist = BMP_MENULIST_Create( BASIC_MSG_TBL_MAX, HEAPID_CASTLE );

	for( i=0; i < BASIC_MSG_TBL_MAX; i++ ){
		BMP_MENULIST_AddArchiveString(	wk->menulist, wk->msgman, 
										basic_msg_tbl[i][0], basic_msg_tbl[i][1] );
	}

	list_h			= CastleListH;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[ENEMY_BMPWIN_BASIC_LIST];
	list_h.work		= wk;
	list_h.call_back= Castle_CsrMoveCallBack;				//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	list_h.icon		= NULL;									//���\�����Ƃ̃R�[���o�b�N�֐�
	list_h.count	= BASIC_MSG_TBL_MAX;					//���X�g���ڐ�
	list_h.b_col	= FBMP_COL_WHITE;						//�w�i�F
	list_h.line		= 4;									//�\���ő區�ڐ�
	wk->lw			= BmpListSet( &list_h, wk->basic_list_lp, wk->basic_list_cp, HEAPID_CASTLE );

	wk->list_flag	= 1;
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[ENEMY_BMPWIN_BASIC_LIST] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_CsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode )
{
	u32 count,line;
	u16 item,list_bak,pos_bak;
	CASTLE_ENEMY_WORK* wk = (CASTLE_ENEMY_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	//���������͖炳�Ȃ�
	if( mode == 0 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

	BmpListDirectPosGet( work, &pos_bak );				//�J�[�\�����W�擾

	CastleWriteMsg( wk, &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2], 
					basic_under_msg_tbl[pos_bak], 1, 1, MSG_NO_PUT, 
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );

	return;
}


//==============================================================================================
//
//	�u�悳�A�킴�A�����N�A�b�v�A��߂�v���X�g
//
//==============================================================================================
static const u32 basic2_msg_tbl[][2] = {
	{ msg_castle_trainer_09_01,	FC_PARAM_TUYOSA },		//�悳
	{ msg_castle_trainer_09_02,	FC_PARAM_WAZA },		//�킴
	{ msg_castle_trainer_09_03,	FC_PARAM_INFO_RANKUP },	//�����N�A�b�v
	{ msg_castle_trainer_09_04,	BMPLIST_CANCEL },		//��߂�
};
#define BASIC2_MSG_TBL_MAX			( NELEMS(basic2_msg_tbl) )

static const u16 basic2_under_msg_tbl[][2] = {
	{ msg_castle_trainer_09_05, msg_castle_trainer_09_05 },		//�悳
	{ msg_castle_trainer_09_06,	msg_castle_trainer_09_06 },		//�킴
	{ msg_castle_trainer_09_07, msg_castle_trainer_09_08 },		//�����N�A�b�v(�����������)
	{ msg_castle_trainer_09_09,	msg_castle_trainer_09_09 },		//��߂�
};

//--------------------------------------------------------------
/**
 * @brief	�u�悳�A�킴�A�����N�A�b�v�A��߂�v���X�g�쐬
 *
 * @param	wk			CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_Basic2ListMake( CASTLE_ENEMY_WORK* wk )
{
	int i;
	BMPLIST_HEADER list_h;

	//���j���[�E�B���h�E��`��
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_BASIC2_LIST] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[ENEMY_BMPWIN_BASIC2_LIST], FBMP_COL_WHITE );	//�h��Ԃ�

	wk->menulist = BMP_MENULIST_Create( BASIC2_MSG_TBL_MAX, HEAPID_CASTLE );

	for( i=0; i < BASIC2_MSG_TBL_MAX; i++ ){
		BMP_MENULIST_AddArchiveString(	wk->menulist, wk->msgman, 
										basic2_msg_tbl[i][0], basic2_msg_tbl[i][1] );
	}

	list_h			= CastleListH;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[ENEMY_BMPWIN_BASIC2_LIST];
	list_h.work		= wk;
	list_h.call_back= Castle_Basic2CsrMoveCallBack;			//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	list_h.icon		= Castle_Basic2LineWriteCallBack;		//���\�����Ƃ̃R�[���o�b�N�֐�
	list_h.count	= BASIC2_MSG_TBL_MAX;					//���X�g���ڐ�
	list_h.b_col	= FBMP_COL_WHITE;						//�w�i�F
	list_h.line		= 4;									//�\���ő區�ڐ�
	wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_CASTLE );

	wk->list_flag	= 1;
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[ENEMY_BMPWIN_BASIC2_LIST] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_Basic2CsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode )
{
	u8 now_rank,no;
	u16 pos_bak;
	CASTLE_ENEMY_WORK* wk = (CASTLE_ENEMY_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	//���������͖炳�Ȃ�
	if( mode == 0 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

	BmpListDirectPosGet( work, &pos_bak );				//�J�[�\�����W�擾

	//�����N�ɂ���ă��b�Z�[�W���ς��
	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_INFO );
	if( now_rank == 1 ){
		no= 0;
	}else{
		no = 1;
	}

	//���̃E�B���h�E�ɕ\�����郁�b�Z�[�W���ς��
	CastleWriteMsg( wk, &wk->bmpwin[ENEMY_BMPWIN_TALK], 
					basic2_under_msg_tbl[pos_bak][no], 1, 1, MSG_NO_PUT, 
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_Basic2LineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y )
{
	u8 now_rank,f_col;
	CASTLE_ENEMY_WORK* wk = (CASTLE_ENEMY_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_INFO );

	f_col = FBMP_COL_BLACK;

	switch( param ){
	case FC_PARAM_WAZA:
		//�����N�����ĕ\���F��ς���
		if( now_rank == 1 ){
			f_col = FBMP_COL_BLK_SDW;
		}else{
			f_col = FBMP_COL_BLACK;
		}
		break;

	case FC_PARAM_INFO_RANKUP:
		//�����N�����ĕ\���F��ς���
		if( now_rank == (CASTLE_RANK_MAX-1) ){
			f_col = FBMP_COL_BLK_SDW;
		}else{
			f_col = FBMP_COL_BLACK;
		}
		break;

	//case FC_PARAM_TUYOSA:
	default:
		f_col = FBMP_COL_BLACK;
		break;
	};

	BmpListColorControl( work, f_col, FBMP_COL_WHITE, FBMP_COL_BLK_SDW );
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * �����Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void Castle_SetNumber( CASTLE_ENEMY_WORK* wk, u32 bufID, s32 number, u32 keta, NUMBER_DISPTYPE disp )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, keta, disp, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P���������Z�b�g
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	ppp		POKEMON_PASO_PARAM
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Castle_SetPokeName( CASTLE_ENEMY_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp )
{
	WORDSET_RegisterPokeMonsName( wk->wordset, bufID, ppp );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�����Z�b�g
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Castle_SetPlayerName( CASTLE_ENEMY_WORK* wk, u32 bufID )
{
	WORDSET_RegisterPlayerName( wk->wordset, bufID, SaveData_GetMyStatus(wk->sv) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[����\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
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
static void PlayerNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	const MYSTATUS* my;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^

	my = SaveData_GetMyStatus( wk->sv );
	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_CASTLE );
	
	//GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	STRBUF_SetStringCode( player_buf, MyStatus_GetMyName(my) );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}
	//OS_Printf( "col = %d\n", col );

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_NO_PUT, col, NULL );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[����\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
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
static void PairNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	MYSTATUS* my;
	u32 col;

	//�p�[�g�i�[��MyStatus�擾
	my = CommInfoGetMyStatus( (CommGetCurrentID() ^ 1) );

	if( MyStatus_GetMySex(my) == PM_MALE ){
		col = COL_BLUE;
	}else{
		col = COL_RED;
	}

	WORDSET_RegisterPlayerName( wk->wordset, 0, my );

	CastleWriteMsgSimple(	wk, win, msg_castle_trainer_name_02, 
							x, y, MSG_NO_PUT, 
							GF_PRINTCOLOR_GET_LETTER(col),
							GF_PRINTCOLOR_GET_SHADOW(col),
							GF_PRINTCOLOR_GET_GROUND(col),
							//FBMP_COL_NULL,
							BC_FONT );
								
	//GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
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
static void PokeNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	u8 x_pos;
	u32 sex,msg_id,col;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	poke =  PokeParty_GetMemberPointer( wk->p_party, 0 );
	PokeParaGet( poke, ID_PARA_default_name, sel_poke_buf );	//�|�P�����̃f�t�H���g�����擾

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	buf = STRBUF_Create( POKE_NAME_BUF_SIZE, HEAPID_CASTLE );	//STRBUF����
	STRBUF_SetStringCode( buf, sel_poke_buf );					//STRCODE��STRBUF

	GF_STR_PrintColor( win, font, buf, x, y, MSG_NO_PUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );
	GF_BGL_BmpWinOnVReq( win );

	STRBUF_Delete( buf );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��(���ʎw��)
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
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
static void PokeNameWriteEx( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex )
{
	u8 x_pos;
	u32 msg_id,col;
	MSGDATA_MANAGER* man;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	//�|�P���������擾
	man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_monsname_dat, HEAPID_CASTLE );
	buf = MSGMAN_AllocString( man, monsno );
	MSGMAN_Delete( man );

	GF_STR_PrintColor( win, font, buf, x, y, MSG_NO_PUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

/*
	//�E�[�ɐ��ʃR�[�h��\��
	x_pos = GF_BGL_BmpWinGet_SizeX(win) - 1;

	msg_id = (sex == PARA_MALE) ? msg_castle_male : msg_castle_female;
	col = (sex == PARA_MALE) ? COL_BLUE : COL_RED;

	STRBUF_Clear( buf );
	MSGMAN_GetString( wk->msgman, msg_id, buf );
	GF_STR_PrintColor( win, font, buf, x_pos*8, y, MSG_NO_PUT, col, NULL );
*/

	STRBUF_Delete( buf );

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P�����̐��ʂ�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 * @param	x		X���W(�h�b�g�P��)
 * @param	y		Y���W(�h�b�g�P��)
 * @param	f_col	�����F�i���o�[(�h��Ԃ��J���[�R�[�h)
 * @param	s_col	�e�F�i���o�[
 * @param	b_col	�w�i�F�i���o�[
 * @param	font	�t�H���g���
 * @param	sex		����
 *
 * @return	none
 */
//--------------------------------------------------------------
static void PokeSexWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font, u8 sex )
{
	u32 msg_id,col;
	u8 f_col,s_col,b_col;

	if( sex == PARA_MALE ){
		msg_id = msg_castle_trainer_status_10_01;
		f_col = FBMP_COL_BLUE;
		s_col = FBMP_COL_BLU_SDW;
		b_col = FBMP_COL_NULL;
	}else if( sex == PARA_FEMALE ){
		msg_id = msg_castle_trainer_status_10_02;
		f_col = FBMP_COL_RED;
		s_col = FBMP_COL_RED_SDW;
		b_col = FBMP_COL_NULL;
	}else{
		return;		//�������Ȃ�
	}

	CastleWriteMsgSimple(	wk, win, msg_id, 
							x, y, MSG_NO_PUT, 
							f_col, s_col, b_col, font );

	//GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ������Ԃ�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleEnemy_Default_Write( CASTLE_ENEMY_WORK* wk )
{
	//�u���ǂ�v
	wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[ENEMY_BMPWIN_MODORU], 
									msg_castle_trainer_00_02, 1, 1+4 , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM );

	//���Ƀ��j���[�E�B���h�E�Ő�����\��
	//CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_TALKMENU] );
	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU], CONFIG_GetWindowType(wk->config));

	wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[ENEMY_BMPWIN_TALKMENU], 
									msg_castle_trainer_00_01, 1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ������Ԃ��폜
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleEnemy_Default_Del( CASTLE_ENEMY_WORK* wk )
{
	//BmpMenuWinClear( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU], WINDOW_TRANS_OFF );
	//GF_BGL_BmpWinOffVReq( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU] );
	BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU] );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ��{��Ԃ�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleEnemy_Basic_Write( CASTLE_ENEMY_WORK* wk )
{
	//���Ƀ��j���[�E�B���h�E2�Ő�����\��
	//CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2] );
	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2], CONFIG_GetWindowType(wk->config));

	/*�u�|�P�����̐��̂��킩��܂��v*/
	//wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2], 
	//								msg_castle_trainer_00_03, 1, 1, MSG_NO_PUT, 
	//								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BC_FONT );

	wk->list_csr_pos = 0;
	Castle_BasicListMake( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ��{��Ԃ��폜
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleEnemy_Basic_Del( CASTLE_ENEMY_WORK* wk )
{
	BmpListPosGet( wk->lw, &wk->basic_list_lp, &wk->basic_list_cp );
	//BmpMenuWinClear( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2], WINDOW_TRANS_OFF );
	//GF_BGL_BmpWinOffVReq( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2] );
	BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2] );
	CastleEnemy_SeqSubListEnd( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �u�悳�A�킴�A�����N�A�b�v�A��߂�v��Ԃ�\��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleEnemy_Basic2_Write( CASTLE_ENEMY_WORK* wk )
{
	//���Ƀ��j���[�E�B���h�E2�Ő�����\��
	//CastleWriteMenuWin( wk->bgl, &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2] );
	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));

	/*�u�|�P�����̂悳���킩��܂��v*/
	wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[ENEMY_BMPWIN_TALK], 
									msg_castle_trainer_09_05, 1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );

	wk->list_csr_pos = 0;
	Castle_Basic2ListMake( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �u�悳�A�킴�A�����N�A�b�v�A��߂�v��Ԃ��폜
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleEnemy_Basic2_Del( CASTLE_ENEMY_WORK* wk )
{
	//BmpMenuWinClear( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2], WINDOW_TRANS_OFF );
	//GF_BGL_BmpWinOffVReq( &wk->bmpwin[ENEMY_BMPWIN_TALKMENU2] );
	CastleEnemy_SeqSubListEnd( wk );
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	type	�`�F�b�N����^�C�v
 *
 * @return	"TRUE = type�������AFALSE = type���Ⴄ"
 */
//--------------------------------------------------------------
static BOOL Castle_CheckType( CASTLE_ENEMY_WORK* wk, u8 type )
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( CASTLE_ENEMY_WORK* wk, int* seq, int next )
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck( CASTLE_ENEMY_WORK* wk )
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
static void CsrMove( CASTLE_ENEMY_WORK* wk, int key )
{
	u8 poke_pos;
	int flag;

	flag = 0;
	poke_pos = GetCsrPokePos( wk->h_max, wk->csr_pos );

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_LEFT ){

		if( wk->csr_pos == wk->modoru_pos ){
			return;
		}

		if( poke_pos == 0 ){
			wk->csr_pos+=(wk->h_max-1);
		}else{
			wk->csr_pos--;
		}

		flag = 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_RIGHT ){

		if( wk->csr_pos == wk->modoru_pos ){
			return;
		}

		if( poke_pos == (wk->h_max - 1) ){
			wk->csr_pos-=(wk->h_max-1);
		}else{
			wk->csr_pos++;
		}

		flag = 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_UP ){

		//�u���ǂ�v�Əc�ɂ��ꂢ�ɕ���ł��Ȃ��̂ŕۗ�
		if( wk->csr_pos < wk->h_max ){
			return;
		}

		wk->csr_pos = wk->tmp_csr_pos;
		flag = 1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_DOWN ){

		//�u���ǂ�v�Əc�ɂ��ꂢ�ɕ���ł��Ȃ��̂ŕۗ�
		if( wk->csr_pos >= wk->modoru_pos ){
			return;
		}

		wk->tmp_csr_pos = wk->csr_pos;
		wk->csr_pos = wk->modoru_pos;
		flag = 1;
	}

	if( flag == 1 ){
		CsrMoveSub( wk );
	}

	return;
}

static void CsrMoveSub( CASTLE_ENEMY_WORK* wk )
{
	Snd_SePlay( SEQ_SE_DP_SELECT );

	//�ʐM�^�C�v�̎��́A�J�[�\���ʒu�𑗐M
	if( Castle_CommCheck(wk->type) == TRUE ){
		CastleEnemy_CommSetSendBuf( wk, CASTLE_COMM_ENEMY_CSR_POS, wk->csr_pos );
	}

	//�|�P�����I���E�B���h�E
	//PokeSelMoveSub( wk, wk->csr_pos, CommGetCurrentID() );
	PokeSelMoveSub( wk, wk->csr_pos, 0 );
	return;
}

//�|�P�����I���E�B���h�E
static void PokeSelMoveSub( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 flag )
{
	CASTLE_OBJ* obj;
	u32 start_x,start_y,anm_no,modoru_anm_no;

	if( flag == 0 ){
		obj = wk->p_poke_sel;
		anm_no = CASTLE_ANM_SELECT_ZIBUN;
		modoru_anm_no = CASTLE_ANM_MODORU_ZIBUN;
	}else{
		obj = wk->p_pair_poke_sel;
		anm_no = CASTLE_ANM_SELECT_AITE;
		modoru_anm_no = CASTLE_ANM_MODORU_AITE;
	}

	//�u���ǂ�v�|�P�����I���E�B���h�̂̕\���ؑ�
	if( csr_pos >= wk->modoru_pos ){
		CastleObj_AnmChg( obj, modoru_anm_no );
		CastleObj_SetObjPos( obj, ENEMY_CSR_MODORU_X, ENEMY_CSR_MODORU_Y );
	}else{
		CastleObj_AnmChg( obj, anm_no );
		CastleEnemy_GetPokeSelXY( wk, &start_x, &start_y, csr_pos );
		CastleObj_SetObjPos( obj, start_x, start_y );
	}

	return;
}

//�|�P�����I���E�B���h�E�I�t�Z�b�g�擾
static void CastleEnemy_GetPokeSelXY( CASTLE_ENEMY_WORK* wk, u32* x, u32* y, u8 csr_pos )
{
	if( Castle_CommCheck(wk->type) == TRUE ){
		if( csr_pos == 0 ){
			*x = CASTLE_MULTI_POKE_SEL_START_X;
		}else if( csr_pos == 1 ){
			*x = CASTLE_MULTI_POKE_SEL_START_X2;
		}else if( csr_pos == 2 ){
			*x = CASTLE_MULTI_POKE_SEL_START_X3;
		}else{
			*x = CASTLE_MULTI_POKE_SEL_START_X4;
		}
	}else{
		if( csr_pos == 0 ){
			*x = CASTLE_POKE_SEL_START_X;
		}else if( csr_pos == 1 ){
			*x = CASTLE_POKE_SEL_START_X2;
		}else{
			*x = CASTLE_POKE_SEL_START_X3;
		}
	}

	*y = CASTLE_POKE_SEL_START_Y;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���w�擾
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
static u16 GetCsrX( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 lr )
{
	u8 poke_pos;
	int ret;
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,lr_offset_x;

	poke_pos = GetCsrPokePos( wk->h_max, csr_pos );

	if( lr == 0 ){						//��
		lr_offset_x = SEL_CSR_L_OFFSET;
	}else{								//�E
		lr_offset_x = SEL_CSR_R_OFFSET;
	}

	if( csr_pos >= wk->modoru_pos ){
		ret = (ENEMY_CSR_MODORU_X + lr_offset_x);
		return ret;
	}

	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	if( Castle_CommCheck(wk->type) == TRUE ){

		if( poke_pos < wk->h_max ){
			ret = poke_pos * CSR_WIDTH_X + lr_offset_x + offset_x;
		}else{
			ret = poke_pos * CSR_WIDTH_X + lr_offset_x + pair_offset_x;
		}

		//OS_Printf( "GetCsrX = %d\n", ret );
		return ret;
	}

	ret = poke_pos * CSR_WIDTH_X + lr_offset_x + offset_x;
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���x�擾
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"Y"
 */
//--------------------------------------------------------------
static u16 GetCsrY( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	u16 ret;

	if( csr_pos >= wk->modoru_pos ){
		return ENEMY_CSR_MODORU_Y;
	}

	ret = CSR_START_Y;
	//OS_Printf( " GetCsrY = %d\n", ret );
	return ret;
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
static void Castle_GetOffset( CASTLE_ENEMY_WORK* wk, u16* offset_x, u16* offset_y, u16* pair_offset_x, u16* pair_offset_y )
{
	//�ʐM���Ă��邩�ŕ\���̃I�t�Z�b�g���ς��
	if( Castle_CommCheck(wk->type) == FALSE ){
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
 * @brief	�I�����ꂽ�J�[�\���ʒu��n���āA�K�v�ȉ��o����������
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE=�I���AFALSE=�p��"
 */
//--------------------------------------------------------------
static BOOL CastleEnemy_DecideEff( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 sel_type )
{
	u16 eff_offset_x;
	u32 anm_no;
	u8 poke_pos,type_offset;
	POKEMON_PARAM* poke;

	//�I�t�Z�b�g
	type_offset = wk->modoru_pos;

	poke_pos = GetCsrPokePos( wk->h_max, csr_pos );
	poke =  PokeParty_GetMemberPointer( wk->p_party, poke_pos );

	switch( sel_type ){

	//�u�Ă����v
	case FC_PARAM_TEMOTI:
		wk->eff_init_flag = 0;
		return TRUE;
		break;

	//�u���x���v
	case FC_PARAM_LEVEL:

		//�����ݒ�
		if( wk->eff_init_flag == 0 ){
			wk->eff_init_flag = 1;

			//�A�j���i���o�[���擾
			//if( wk->level_sel == 1 ){
			if( wk->parent_decide_updown == 1 ){
				anm_no = CASTLE_ANM_LV_UP;
			}else{
				anm_no = CASTLE_ANM_LV_DOWN;
			}

			if( Castle_CommCheck(wk->type) == FALSE ){
				eff_offset_x	= LV_UPDOWN_START_X;
			}else{
				eff_offset_x	= LV_UPDOWN_MULTI_START_X;
			}

			//�G�t�F�N�gOBJ�ǉ�
			wk->p_eff = CastleObj_Create(	&wk->castle_clact, 
											ID_CHAR_CSR, ID_PLTT_CSR, 
											ID_CELL_CSR, anm_no, 
											eff_offset_x + (LV_UPDOWN_WIDTH_X * poke_pos),
											LV_UPDOWN_START_Y,
											CASTLE_BG_PRI_HIGH, NULL );
		}

		//�A�j���[�V�����I���҂�
		if( CastleObj_AnmActiveCheck(wk->p_eff) == FALSE ){
			CastleObj_Delete( wk->p_eff );
			wk->p_eff = NULL;
			wk->eff_init_flag = 0;
			return TRUE;
		}
		break;

	//�u�悳�v
	case FC_PARAM_TUYOSA:
		if( Castle_CommCheck(wk->type) == FALSE ){
			if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
				Snd_SePlay( SEQ_SE_DP_SELECT );
				CastleEnemy_SeqSubTuyosa( wk, csr_pos );					//�悳
				wk->eff_init_flag = 0;
				return TRUE;
			}
		}else{
			wk->eff_init_flag = 0;
			return TRUE;
		}
		break;

	//�u�킴�v
	case FC_PARAM_WAZA:
		if( Castle_CommCheck(wk->type) == FALSE ){
			if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
				Snd_SePlay( SEQ_SE_DP_SELECT );
				CastleEnemy_SeqSubWaza( wk, csr_pos );								//�킴
				wk->eff_init_flag = 0;
				return TRUE;
			}
		}else{
			wk->eff_init_flag = 0;
			return TRUE;
		}
		break;

	//�u���傤�ق������N�A�b�v�v
	case FC_PARAM_INFO_RANKUP:
		wk->eff_init_flag = 0;
		return TRUE;
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�I�����ꂽ�J�[�\���ʒu��n���āA�K�v�ȉ��o����������
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	"TRUE=�I���AFALSE=�p��"
 */
//--------------------------------------------------------------
static BOOL CastleEnemy_DecideEff2( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 sel_type )
{
	u32 anm_no;
	u8 poke_pos,type_offset;
	POKEMON_PARAM* poke;

	//�I�t�Z�b�g
	type_offset = wk->modoru_pos;

	poke_pos = GetCsrPokePos( wk->h_max, csr_pos );
	poke =  PokeParty_GetMemberPointer( wk->p_party, poke_pos );

	switch( sel_type ){

	//�u�Ă����v
	//�u���x���v
	//�u���傤�ق������N�A�b�v�v
	case FC_PARAM_TEMOTI:
	case FC_PARAM_LEVEL:
	case FC_PARAM_INFO_RANKUP:
		wk->eff_init_flag = 0;
		return TRUE;
		break;

	//�u�悳�v
	case FC_PARAM_TUYOSA:
		if( Castle_CommCheck(wk->type) == TRUE ){
			if( CommGetCurrentID() == COMM_PARENT_ID ){						//�e
				if( wk->parent_decide_pos >= type_offset ){					//�e�F�q�̌��肪�L���Ȃ�
					wk->eff_init_flag = 0;
					return TRUE;
				}
			}else{															//�q
				if( wk->parent_decide_pos < type_offset ){					//�q�F�e�̌��肪�L���Ȃ�
					wk->eff_init_flag = 0;
					return TRUE;
				}
			}
		}

#if 0
	
		if( wk->eff_init_flag == 0 ){
			CastleEnemy_SeqSubTuyosa( wk, csr_pos );						//�悳
			wk->eff_init_flag = 1;
			return FALSE;
		}

		if( sys.trg & PAD_KEY_LEFT ){
			CastleEnemy_TuyosaWinChg( wk, -1 );
		}else if( sys.trg & PAD_KEY_RIGHT ){
			CastleEnemy_TuyosaWinChg( wk, 1 );
		}else if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			wk->eff_init_flag = 0;
			return TRUE;
		}
#else
		if( wk->eff_init_flag == 0 ){
			wk->recover_flag = RECOVER_STATUS;						//�ʐM�Ŏ������X�e�[�^�X�J������
			CastleEnemy_SeqSubTuyosa( wk, csr_pos );						//�悳
			wk->eff_init_flag = 0;
			return TRUE;
		}
#endif
		break;

	//�u�킴�v
	case FC_PARAM_WAZA:
		if( Castle_CommCheck(wk->type) == TRUE ){
			if( CommGetCurrentID() == COMM_PARENT_ID ){						//�e
				if( wk->parent_decide_pos >= type_offset ){					//�e�F�q�̌��肪�L���Ȃ�
					wk->eff_init_flag = 0;
					return TRUE;
				}
			}else{															//�q
				if( wk->parent_decide_pos < type_offset ){					//�q�F�e�̌��肪�L���Ȃ�
					wk->eff_init_flag = 0;
					return TRUE;
				}
			}
		}

#if 0
		if( wk->eff_init_flag == 0 ){
			CastleEnemy_SeqSubWaza( wk, csr_pos );							//�킴
			wk->eff_init_flag = 1;
			return FALSE;
		}

		if( sys.trg & PAD_KEY_LEFT ){
			CastleEnemy_WazaWinChg( wk, -1 );
		}else if( sys.trg & PAD_KEY_RIGHT ){
			CastleEnemy_WazaWinChg( wk, 1 );
		}else if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			wk->eff_init_flag = 0;
			return TRUE;
		}
#else
		if( wk->eff_init_flag == 0 ){
			wk->recover_flag = RECOVER_STATUS;						//�ʐM�Ŏ������X�e�[�^�X�J������
			CastleEnemy_SeqSubWaza( wk, csr_pos );							//�킴
			wk->eff_init_flag = 0;
			return TRUE;
		}
#endif
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	���x������̎g�pCP���擾
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static u16 CastleEnemy_GetLvCP( u8 level_sel )
{
	if( level_sel == 1 ){
		return CP_USE_LEVEL_PLUS;
	}

	return CP_USE_LEVEL_MINUS;
}

//--------------------------------------------------------------
/**
 * @brief	�悳�E�B���h�E��\�����Ă��鎞�ɍ��E�Ő؂�ւ�
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_TuyosaWinChg( CASTLE_ENEMY_WORK* wk, s8 add_sub )
{
	POKEMON_PARAM* poke;
	s8 pos;

	pos	= wk->csr_pos;

	while( 1 ){

		pos += add_sub;

		if( pos < 0 ){
			pos = (wk->h_max - 1);
		}else if( pos >= wk->h_max ){
			pos = 0;
		}

		//����`�F�b�N����
		if( pos == wk->csr_pos ){
			return;
		}

		//�E�B���h�E��؂�ւ�
		if( wk->p_tuyosa_flag[pos] == 1 ){			//�t���OON
			wk->csr_pos = pos;
			CsrMoveSub( wk );
			poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,wk->csr_pos) );
			Castle_StatusMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_STATUS], poke );
			return;
		}
	};

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�킴�E�B���h�E��\�����Ă��鎞�ɍ��E�Ő؂�ւ�
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_WazaWinChg( CASTLE_ENEMY_WORK* wk, s8 add_sub )
{
	POKEMON_PARAM* poke;
	s8 pos;

	pos	= wk->csr_pos;

	while( 1 ){

		pos += add_sub;

		if( pos < 0 ){
			pos = (wk->h_max - 1);
		}else if( pos >= wk->h_max ){
			pos = 0;
		}

		//����`�F�b�N����
		if( pos == wk->csr_pos ){
			return;
		}

		//�E�B���h�E��؂�ւ�
		if( wk->p_waza_flag[pos] == 1 ){			//�t���OON
			wk->csr_pos = pos;
			CsrMoveSub( wk );
			poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,wk->csr_pos) );
			Castle_WazaMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_STATUS], poke );
			return;
		}
	};

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�y�A���������肵�����̋����I������
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_PairDecideDel( CASTLE_ENEMY_WORK* wk )
{
	CastleEnemy_SeqSubMenuWinClear( wk );
	CastleEnemy_SeqSubListEnd( wk );
	BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
	CastleObj_Vanish( wk->p_eff_rankup, CASTLE_VANISH_ON );			//��\��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�E�B���h�E�폜
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BmpTalkWinClearSub( GF_BGL_BMPWIN* win )
{
	BmpTalkWinClear( win, WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( win );
	return;
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
 * @param	wk			CASTLE_RANK_WORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL CastleEnemy_CommSetSendBuf( CASTLE_ENEMY_WORK* wk, u16 type, u16 param )
{
	int ret,command;

	switch( type ){

	//���O
	case CASTLE_COMM_ENEMY_PAIR:
		command = FC_CASTLE_ENEMY_PAIR;
		CastleEnemy_CommSendBufBasicData( wk, type );
		break;

	//���N�G�X�g
	case CASTLE_COMM_ENEMY_UP_TYPE:
		command = FC_CASTLE_ENEMY_REQ_TYPE;
		CastleEnemy_CommSendBufRankUpType( wk, type, param );
		break;

	//�J�[�\���ʒu
	case CASTLE_COMM_ENEMY_CSR_POS:
		command = FC_CASTLE_ENEMY_CSR_POS;
		CastleEnemy_CommSendBufCsrPos( wk, type );
		break;

	//�u�߂�v
	case CASTLE_COMM_ENEMY_MODORU:
		command = FC_CASTLE_ENEMY_MODORU;
		CastleEnemy_CommSendBufModoru( wk );
		break;
	};

	if( CommSendData(command,wk->send_buf,CASTLE_COMM_BUF_LEN) == TRUE ){
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
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleEnemy_CommSendBufBasicData( CASTLE_ENEMY_WORK* wk, u16 type )
{
	int i,num;
	MYSTATUS* my;
	
	OS_Printf( "******�L���b�X��****** ��{��񑗐M\n" );

	num = 0;
	my	= SaveData_GetMyStatus( wk->sv );

	wk->send_buf[num] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );
	num+=1;														//1

	wk->send_buf[num]	= MyStatus_GetMySex(my);				//����
	OS_Printf( "���M�Fmy sex = %d\n", wk->send_buf[1] );
	num+=1;														//2

	//�����N
	for( i=0; i < CASTLE_RANK_TYPE_MAX ;i++ ){
		wk->send_buf[num+i] = Castle_GetRank( wk->sv, wk->type, i );
		OS_Printf( "���M�Frank = %d\n", wk->send_buf[num+i] );
	}
	num+=CASTLE_RANK_TYPE_MAX;									//5

	//wk->send_buf[num] = CASTLESCORE_GetCP( wk->score_sv );
	//OS_Printf( "���M�Fcp = %d\n", wk->send_buf[num] );
	//num+=1;														//6

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
void CastleEnemy_CommRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_ENEMY_WORK* wk = work;
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
	
	//wk-> = ;
	num+=1;													//2

	for( i=0; i < CASTLE_RANK_TYPE_MAX ;i++ ){
		wk->pair_rank[i] = (u8)recv_buf[num+i];				//�����N
		OS_Printf( "��M�Fwk->pair_rank[%d] = %d\n", i, wk->pair_rank[i] );
	}
	num+=CASTLE_RANK_TYPE_MAX;								//5

	//wk->pair_cp = (u16)recv_buf[num];						//CP
	//OS_Printf( "��M�Fwk->pair_cp = %d\n", wk->pair_cp );
	//num+=1;													//6

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ƀ��N�G�X�g(�ǂ�������N�A�b�v������)���Z�b�g
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleEnemy_CommSendBufRankUpType( CASTLE_ENEMY_WORK* wk, u16 type, u16 param )
{
	OS_Printf( "******�L���b�X��****** �����N�A�b�v���������N�G�X�g��񑗐M\n" );

	//�R�}���h
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	//�J�[�\���ʒu
	wk->send_buf[1] = param;
	OS_Printf( "���M�Fcsr_pos = %d\n", wk->send_buf[1] );

	//��Ɏq�̑I�������Ă��Ȃ��āA�܂��l�������Ă��Ȃ����́A�e�̌���̓Z�b�g���Ă��܂�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		if( wk->parent_decide_pos == CASTLE_ENEMY_DECIDE_NONE ){
			wk->parent_decide_pos = param;
			//wk->parent_decide_lr = (wk->csr_pos % wk->h_max);
		}
	}

	//�e�̌���^�C�v
	wk->send_buf[2] = wk->parent_decide_pos;
	OS_Printf( "���M�Fparent_decide_pos = %d\n", wk->send_buf[2] );

	//�p�[�g�i�[�����A�E�ŁA�����A����̂ǂ���̃����N���グ�悤�Ƃ��Ă��邩
	//wk->send_buf[3] = (wk->csr_pos % wk->h_max);
	//OS_Printf( "���M�Frankup_lr = %d\n", wk->send_buf[3] );

	//���x���グ����
	wk->send_buf[4] = wk->parent_decide_updown;
	OS_Printf( "���M�Flv_updown = %d\n", wk->send_buf[4] );

	//���肵������
	wk->send_buf[5] = wk->parent_decide_type;
	OS_Printf( "���M�Fparent_decide_type = %d\n", wk->send_buf[5] );

	return;
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
void CastleEnemy_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_ENEMY_WORK* wk = work;
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
	
	wk->pair_rankup_type = recv_buf[1];
	OS_Printf( "��M�Fwk->pair_rankup_type = %d\n", wk->pair_rankup_type );

	////////////////////////////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�̌��肪���łɌ��܂��Ă�����A�q�̑I���͖���
		if( wk->parent_decide_pos != CASTLE_ENEMY_DECIDE_NONE ){
			wk->pair_rankup_type= 0;
			//wk->parent_decide_lr= (wk->csr_pos % wk->h_max);
		}else{

			//�e�̌��肪���܂��Ă��Ȃ����́A
			//�e�����M���鎞�Ɂu�q�ɂ���ł�����v�Ƒ��M����
			//wk->parent_decide_pos	= wk->pair_rankup_type;
			
			//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g("modoru_pos")��������
			wk->parent_decide_pos	= wk->pair_rankup_type + wk->modoru_pos;
			wk->parent_decide_updown= recv_buf[4];
			wk->parent_decide_type  = recv_buf[5];
		}
	////////////////////////////////////////////////////////////////////////
	//�q
	}else{
		//�e�̌���^�C�v
		wk->parent_decide_pos = recv_buf[2];
		
		//�p�[�g�i�[�����A�E�ŁA�����A����̂ǂ���̃����N���グ�悤�Ƃ��Ă��邩
		//wk->parent_decide_lr	= recv_buf[3];
		wk->parent_decide_updown= recv_buf[4];
		wk->parent_decide_type  = recv_buf[5];
	}

	OS_Printf( "��M�Fwk->parent_decide_pos = %d\n", wk->parent_decide_pos );
	//OS_Printf( "��M�Fparent_decide_lr = %d\n", wk->parent_decide_lr );
	OS_Printf( "��M�Fparent_decide_updown = %d\n", wk->parent_decide_updown );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɃJ�[�\���ʒu���Z�b�g
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleEnemy_CommSendBufCsrPos( CASTLE_ENEMY_WORK* wk, u16 type )
{
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	wk->send_buf[1]	= wk->csr_pos;
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
void CastleEnemy_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work)
{
	CASTLE_ENEMY_WORK* wk = work;
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

	//�|�P�����I���E�B���h�E
	//PokeSelMoveSub( wk, wk->pair_csr_pos, (CommGetCurrentID() ^ 1) );
	PokeSelMoveSub( wk, wk->pair_csr_pos, 1 );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�Ɂu�߂�v���Z�b�g
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleEnemy_CommSendBufModoru( CASTLE_ENEMY_WORK* wk )
{
	OS_Printf( "******�L���b�X��****** �u�߂�v��񑗐M\n" );

	wk->send_buf[0] = 1;
	OS_Printf( "���M�F�߂� = %d\n", wk->send_buf[0] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �u�߂�v �ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		FRONTIER_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CastleEnemy_CommRecvBufModoru(int id_no,int size,void *pData,void *work)
{
	CASTLE_ENEMY_WORK* wk = work;
	const u16* recv_buf = pData;

	OS_Printf( "******�L���b�X��****** �u�߂�v����M\n" );

	//wk->recieve_count++;

	//�����̃f�[�^�͎󂯎��Ȃ�
	if( CommGetCurrentID() == id_no ){
		return;
	}

	wk->pair_modoru_flag = (u8)recv_buf[0];
	OS_Printf( "��M�Fwk->pair_modoru_flag = %d\n", wk->pair_modoru_flag );
	return;
}


//==============================================================================================
//
//	�T�u�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief		�����Ă��铹��A���i�A�����A�e�\�͒l��\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubTuyosa( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;

	//�悳�A�킴�����J����Ă��������\������
	//CastleEnemy_SeqSubTuyosaFlagOn( wk, csr_pos );
	//CastleRank_SeqSubHataVanish( wk );

	Castle_SetStatusBgGraphic( wk, BC_FRAME_SLIDE );
	BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_StatusMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_STATUS], poke );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );

	//Snd_SePlay( SEQ_SE_DP_DENSI16 );
	//Snd_SePlay( SEQ_SE_DP_UG_020 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief		�悳���J�t���O�I��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubTuyosaFlagOn( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	wk->p_tuyosa_flag[GetCsrPokePos(wk->h_max,csr_pos)] = 1;			//�t���OON
	return;
}

//�u�悳�v������悤�ɂȂ���MSG
static void CastleEnemy_SeqSubTuyosaMsgSet( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;

	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
	wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_06_04, FONT_TALK );

	//�悳�����J����Ă��������\������
	CastleEnemy_SeqSubTuyosaFlagOn( wk, csr_pos );
	CastleRank_SeqSubHataVanish( wk );
	Snd_SePlay( SEQ_SE_DP_UG_020 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief		�Z���A�o�o����\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubWaza( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;

	//�悳�A�킴�����J����Ă��������\������
	//CastleEnemy_SeqSubWazaFlagOn( wk, csr_pos );
	//CastleRank_SeqSubHataVanish( wk );

	Castle_SetWazaBgGraphic( wk, BC_FRAME_SLIDE );
	BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_WazaMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_STATUS], poke );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );

	//Snd_SePlay( SEQ_SE_DP_DENSI16 );
	//Snd_SePlay( SEQ_SE_DP_UG_020 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief		�킴���J�t���O�I��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubWazaFlagOn( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	wk->p_waza_flag[GetCsrPokePos(wk->h_max,csr_pos)] = 1;					//�t���OON
	return;
}

static void CastleEnemy_SeqSubWazaMsgSet( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;

	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
	wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_07_04, FONT_TALK );

	//�킴�����J����Ă��������\������
	CastleEnemy_SeqSubWazaFlagOn( wk, csr_pos );
	CastleRank_SeqSubHataVanish( wk );
	Snd_SePlay( SEQ_SE_DP_UG_020 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���j���[�E�B���h�E�N���A
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubMenuWinClear( CASTLE_ENEMY_WORK* wk )
{
	if( wk->menu_flag == 1 ){
		wk->menu_flag = 0;
		BmpMenuExit( wk->mw, NULL );
		BmpMenuWinClear( wk->MenuH.win, WINDOW_TRANS_OFF );
		GF_BGL_BmpWinOffVReq( wk->MenuH.win );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	��b�E�B���h�EON
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubTalkWinOn( CASTLE_ENEMY_WORK* wk )
{
	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
	return;
}

//--------------------------------------------------------------
/**
 * @brief	CP�X�V
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubCPWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,now_cp;

	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	if( Castle_CommCheck(wk->type) == FALSE ){
		x = offset_x + INFO_PLAYER_CP_X;
		y = offset_y + INFO_PLAYER_CP_Y;
		GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );

		now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

		Castle_SetNumber(	wk, 0, now_cp, 
							CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
		wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_trainer_cp_01, x, y , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
	}else{

		//�e�A�q�̉�ʂƂ��A�e�A�q�̏��Ԃ�CP���\�������悤�ɂ���

		//////////////////////////////////////////////////////////////////////////////
		//�e�Ȃ�
		if( CommGetCurrentID() == COMM_PARENT_ID ){
			x = offset_x + INFO_PLAYER_CP_X;
			y = offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

			Castle_SetNumber(	wk, 0, now_cp, 
								CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
										msg_castle_trainer_cp_01, x, y , MSG_NO_PUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

			//�p�[�g�i�[��CP��\��
			x = pair_offset_x + INFO_PLAYER_CP_X;
			y = pair_offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );
			Castle_SetNumber( wk, 0, wk->pair_cp, CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
										msg_castle_trainer_cp_01, x, y , MSG_NO_PUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

		//////////////////////////////////////////////////////////////////////////////
		//�q�Ȃ�
		}else{
			//�p�[�g�i�[��CP��\��
			x = offset_x + INFO_PLAYER_CP_X;
			y = offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );
			Castle_SetNumber( wk, 0, wk->pair_cp, CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
										msg_castle_trainer_cp_01, x, y , MSG_NO_PUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

			x = pair_offset_x + INFO_PLAYER_CP_X;
			y = pair_offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

			Castle_SetNumber(	wk, 0, now_cp, 
								CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
										msg_castle_trainer_cp_01, x, y , MSG_NO_PUT, 
										FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
		}
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	LEVEL����CP�m�F
 *
 * @param	level_sel	1=+5�A2=-5
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubLevelMsg( CASTLE_ENEMY_WORK* wk, u8 level_sel )
{
	Castle_SetNumber( wk, 0, CastleEnemy_GetLvCP(level_sel), CASTLE_KETA_CP, NUMBER_DISPTYPE_LEFT );
	wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_02, FONT_TALK );
	Castle_SetMenu2( wk );										//�u�͂��E�������v
	wk->level_sel = level_sel;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�A�p�[�g�i�[���\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubNameWrite( CASTLE_ENEMY_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y;

	//�I�t�Z�b�g�擾
	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	if( Castle_CommCheck(wk->type) == FALSE ){
		x = offset_x + INFO_PLAYER_X;
		y = offset_y + INFO_PLAYER_Y;
		PlayerNameWrite( wk, win, x, y, BC_FONT );			//�v���C���[����\��
		//GF_BGL_BmpWinOnVReq( win );
	}else{

		//�e�A�q�̉�ʂƂ��A�e�A�q�̏��Ԃɖ��O���\�������悤�ɂ���

		//�e�Ȃ�
		if( CommGetCurrentID() == COMM_PARENT_ID ){
			x = offset_x + INFO_PLAYER_X;
			y = offset_y + INFO_PLAYER_Y;
			PlayerNameWrite( wk, win, x, y, BC_FONT );		//�v���C���[����\��

			x = pair_offset_x + INFO_PLAYER_X;
			y = pair_offset_y + INFO_PLAYER_Y;
			PairNameWrite( wk, win, x, y, BC_FONT );		//�p�[�g�i�[����\��

		//�q�Ȃ�
		}else{
			x = offset_x + INFO_PLAYER_X;
			y = offset_y + INFO_PLAYER_Y;
			PairNameWrite( wk, win, x, y, BC_FONT );		//�p�[�g�i�[����\��

			x = pair_offset_x + INFO_PLAYER_X;
			y = pair_offset_y + INFO_PLAYER_Y;
			PlayerNameWrite( wk, win, x, y, BC_FONT );		//�v���C���[����\��
		}

		//GF_BGL_BmpWinOnVReq( win );
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����N�A�b�v
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 *
 * �e�A�q�Ƃ��ɉ�ʂ̓G�|�P�����̕��т͓���
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubRankUp( CASTLE_ENEMY_WORK* wk, u8 parent_decide_pos, u8 parent_decide_type )
{
	u8	lr,sel_pos,type_offset;
	u16 sub_cp;
	u16 buf16[4];
	u16 offset_x,offset_y,pair_offset_x,pair_offset_y;

	Snd_SePlay( SEQ_SE_DP_PIRORIRO2 );

	//�I�t�Z�b�g
	type_offset = wk->modoru_pos;

	//�I�񂾃J�[�\���ʒu���擾
	sel_pos = GetCommSelCsrPos( type_offset, parent_decide_pos );

	////////////////////////////////////////////////
	//�g�pCP
	switch( parent_decide_type ){

	case FC_PARAM_TEMOTI:
		sub_cp = CP_USE_TEMOTI;
		break;

	case FC_PARAM_LEVEL:
		sub_cp = CastleEnemy_GetLvCP( wk->parent_decide_updown );			//�g�pCP
		break;

	case FC_PARAM_TUYOSA:
		sub_cp = CP_USE_TUYOSA;
		break;

	case FC_PARAM_WAZA:
		sub_cp = CP_USE_WAZA;
		break;
	};

	OS_Printf( "parent_decide_type = %d\n", parent_decide_type );
	OS_Printf( "sub_cp = %d\n", sub_cp );
	OS_Printf( "wk->pair_cp = %d\n", wk->pair_cp );

	////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�F�e�̌��肪�L���Ȃ�
		if( parent_decide_pos < type_offset ){
			Castle_SetPlayerName( wk, 5 );
			Castle_CPRecord_Sub( wk->fro_sv, wk->type, sub_cp );//������CP���炷

		//�e�F�q�̌��肪�L���Ȃ�
		}else{
			Castle_SetPairName( wk->wordset, 5 );
			wk->pair_cp -= sub_cp;								//�����CP���炷
		}

	///////////////////////////////////////////////
	//�q
	}else{

		//�q�F�e�̌��肪�L���Ȃ�
		if( parent_decide_pos < type_offset ){
			Castle_SetPairName( wk->wordset, 5 );
			wk->pair_cp -= sub_cp;								//�����CP���炷

		//�q�F�q�̌��肪�L���Ȃ�
		}else{
			Castle_SetPlayerName( wk, 5 );
			Castle_CPRecord_Sub( wk->fro_sv, wk->type, sub_cp );//������CP���炷
		}
	}

	OS_Printf( "wk->pair_cp = %d\n", wk->pair_cp );

	CastleEnemy_SeqSubCPWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_TR1] );	//CP�\�����X�V
	CastleEnemy_SeqSubMenuWinClear( wk );							//���j���[��\�����Ă�����폜

	switch( parent_decide_type ){

	case FC_PARAM_TEMOTI:
		CastleEnemy_SeqSubTemoti( wk, sel_pos );							//�Ă���
		break;

	case FC_PARAM_LEVEL:
		CastleEnemy_SeqSubLevel( wk, sel_pos, wk->parent_decide_updown );	//���x��
		break;

	case FC_PARAM_TUYOSA:
		CastleEnemy_SeqSubTuyosaMsgSet( wk, sel_pos );						//�悳������MSG
		break;

	case FC_PARAM_WAZA:
		CastleEnemy_SeqSubWazaMsgSet( wk, sel_pos );						//�킴������MSG
		break;
	};

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�莝��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubTemoti( CASTLE_ENEMY_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;

	OS_Printf( "GetCsrPokePos(wk->h_max,csr_pos) = %d\n", GetCsrPokePos(wk->h_max,csr_pos) );

	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
	Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
	wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_04_03, FONT_TALK );

	//�t���OON
	wk->p_temoti_flag[GetCsrPokePos(wk->h_max,csr_pos)] = 1;

	CastleObj_Vanish( wk->p_ball[GetCsrPokePos(wk->h_max,csr_pos)], CASTLE_VANISH_ON );	//��\��
	CastleObj_Vanish( wk->p_icon[GetCsrPokePos(wk->h_max,csr_pos)], CASTLE_VANISH_OFF );//�\��

	//HP��LV�\��
	Castle_PokeHpMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_HP] );
	Castle_PokeLvMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_LV] );

	CastleObj_Vanish( wk->p_hp[GetCsrPokePos(wk->h_max,csr_pos)], CASTLE_VANISH_OFF );	//HP��ԕ\��

	//Snd_SePlay( SEQ_SE_DP_DENSI16 );
	Snd_SePlay( SEQ_SE_DP_UG_020 );
	//Snd_SePlay( SEQ_SE_DP_DENSI15 );
	//Snd_SePlay( SEQ_SE_DP_DENSI06 );		//��Ȃ�
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���x��
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubLevel( CASTLE_ENEMY_WORK* wk, u8 csr_pos, u8 level_sel )
{
	u32 exp;
	POKEMON_PARAM* temp_poke;

	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
	temp_poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_SetPokeName( wk, 0, PPPPointerGet(temp_poke) );
	if( level_sel == 1 ){
		wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_05_06, FONT_TALK );
		//Snd_SePlay( SEQ_SE_DP_DENSI16 );
		//Snd_SePlay( SEQ_SE_DP_ZUKAN02 );
		Snd_SePlay( SEQ_SE_DP_OPEN7 );
	}else{
		wk->msg_index = Castle_EasyMsg( wk, msg_castle_trainer_05_07, FONT_TALK );
		//Snd_SePlay( SEQ_SE_DP_DENSI16 );
		//Snd_SePlay( SEQ_SE_DP_ZUKAN02 );
		Snd_SePlay( SEQ_SE_DP_CLOSE7 );
	}

	//���x�����オ������A����������
	//�\������Ă�������X�V����
			
	//�t���OON
	if( wk->p_level_flag[GetCsrPokePos(wk->h_max,csr_pos)] == 0 ){
		wk->p_level_flag[GetCsrPokePos(wk->h_max,csr_pos)] = level_sel;
	}else{
		wk->p_level_flag[GetCsrPokePos(wk->h_max,csr_pos)] = 0;		//��x�㉺���������̂����ɖ߂���
	}

	//���x������
	if( wk->p_level_flag[GetCsrPokePos(wk->h_max,csr_pos)] == 0 ){
		exp = PokeLevelExpGet( PokeParaGet(temp_poke,ID_PARA_monsno,NULL), 50 );
		PokeParaPut( temp_poke, ID_PARA_exp, &exp );
		PokeParaCalc( temp_poke );
	}else if( wk->p_level_flag[GetCsrPokePos(wk->h_max,csr_pos)] == 1 ){
		exp = PokeLevelExpGet( PokeParaGet(temp_poke,ID_PARA_monsno,NULL), 55 );
		PokeParaPut( temp_poke, ID_PARA_exp, &exp );
		PokeParaCalc( temp_poke );
	}else{
		exp = PokeLevelExpGet( PokeParaGet(temp_poke,ID_PARA_monsno,NULL), 45 );
		PokeParaPut( temp_poke, ID_PARA_exp, &exp );
		PokeParaCalc( temp_poke );
	}

	//HP��LV�\��
	Castle_PokeHpMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_HP] );
	Castle_PokeLvMsgWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_LV] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�X�e�[�^�X�E�B���h�E������
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubStatusWinDel( CASTLE_ENEMY_WORK* wk )
{
	GF_BGL_BmpWinDataFill( &wk->bmpwin[ENEMY_BMPWIN_STATUS], FBMP_COL_NULL );	//�h��Ԃ�
	//GF_BGL_BmpWinOnVReq( &wk->bmpwin[ENEMY_BMPWIN_STATUS] );
	//GF_BGL_BmpWinOffVReq( &wk->bmpwin[ENEMY_BMPWIN_STATUS] );
	GF_BGL_BmpWinOff( &wk->bmpwin[ENEMY_BMPWIN_STATUS] );						//��VReq���Ȃ�
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�I��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_SeqSubListEnd( CASTLE_ENEMY_WORK* wk )
{
	GF_BGL_BMPWIN* p_list_win;

	if( wk->list_flag == 1 ){
		wk->list_flag = 0;
		//wk->list_csr_pos = 0;	//���ӁI
		p_list_win = (GF_BGL_BMPWIN*)BmpListParamGet( wk->lw, BMPLIST_ID_WIN );

#if 0
		CastleEnemy_SeqSubMenuWinClear( wk );
#else
		//BmpMenuExit( wk->mw, NULL );
		//BmpMenuWinClear( wk->MenuH.win, WINDOW_TRANS_OFF );
		BmpMenuWinClear( p_list_win, WINDOW_TRANS_OFF );
#endif

		GF_BGL_BmpWinDataFill( p_list_win, FBMP_COL_NULL );	//�h��Ԃ�
		GF_BGL_BmpWinOffVReq( p_list_win );

		//���X�g�폜
		BMP_MENULIST_Delete( wk->menulist );
		BmpListExit( wk->lw, NULL, NULL );
	}

	return;;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�R���㉺�A�j��
 *
 * @param	wk		CASTLE_RANK_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleEnemy_IconSelAnm( CASTLE_ENEMY_WORK* wk )
{
	int	i;
	u8 m_max,anm;
	POKEMON_PARAM* poke;

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=0; i < m_max ;i++ ){
		poke =  PokeParty_GetMemberPointer( wk->p_party, i );

		anm = Frontier_PokeIconAnmNoGet(PokeParaGet(poke,ID_PARA_hp,NULL),
										PokeParaGet(poke,ID_PARA_hpmax,NULL) );

		if( wk->p_icon[i] != NULL ){
			CastleObj_PokeIconAnmChg( wk->p_icon[i], anm );

			//�I�����Ă���|�P�����ł͂Ȃ������A�u���ǂ�v�I�𒆂̎�
			if((i != GetCsrPokePos(wk->h_max,wk->csr_pos)) || (wk->csr_pos >= wk->modoru_pos) ){
				CastleObj_PokeIconPosSet( wk->p_icon[i], 0 );
			}else{
				CastleObj_PokeIconPosSet( wk->p_icon[i], 1 );
			}
		}
	}
	return;
}


//--------------------------------------------------------------
/**
 * @brief	�悳�A�킴�u�͂��v�̋��ʏ���
 *
 * @param	
 *
 * @return	"FALSE = ���̌�break, TRUE = ���̌�return TRUE"
 */
//--------------------------------------------------------------
static BOOL CastleEnemy_SeqSubTuyosaWazaYes( CASTLE_ENEMY_WORK* wk, u16 use_cp, u16 cp_not_msg )
{
	u16 now_cp;

	CastleEnemy_SeqSubMenuWinClear( wk );

	now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

	//CP������Ȃ���
	if( now_cp < use_cp ){
		CastleEnemy_SeqSubTalkWinOn( wk );
		wk->msg_index = Castle_EasyMsg( wk, cp_not_msg, FONT_TALK );
		wk->sub_seq = SEQ_SUB_KEY_WAIT;
		return FALSE;
	}

	if( Castle_CommCheck(wk->type) == FALSE ){
		BmpTalkWinClearSub( &wk->bmpwin[ENEMY_BMPWIN_TALK] );
		Castle_CPRecord_Sub( wk->fro_sv, wk->type, use_cp );
		CastleEnemy_SeqSubCPWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_TR1] );

		if( use_cp == CP_USE_TUYOSA ){
			CastleEnemy_SeqSubTuyosaMsgSet( wk, wk->csr_pos );		//�悳������MSG
		}else{
			CastleEnemy_SeqSubWazaMsgSet( wk, wk->csr_pos );		//�킴������MSG
		}

		wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT_NEXT_WIN_OFF;
		return FALSE;

	}else{
		wk->send_req = 1;
		return TRUE;
	}

	return FALSE;
}


//==============================================================================================
//
//	castle_rank.c
//
//==============================================================================================

//�ʐM����ɕ\������郁�b�Z�[�W
static const u16 rankup_pair_msg_tbl[CASTLE_RANK_TYPE_MAX][CASTLE_RANK_MAX] = {
//���񕜂̃����N�A�b�v�Q
//���񕜂̃����N�A�b�v�R
	{	0, 0, 0 },							//���g�p

//������̃����N�A�b�v�Q
//������̃����N�A�b�v�R
	{	0, 0, 0 },							//���g�p

//�����̃����N�A�b�v�Q
//�����̃����N�A�b�v�R
//�u�킴�̂��傤�ق����������Ƃ��ł���悤�ɂȂ�܂��v
//�u�킴�̂��傤�ق����������Ƃ��ł���悤�ɂȂ�܂��v
	{ 0, msg_castle_trainer_11_05, msg_castle_trainer_11_05 },
};

//--------------------------------------------------------------
/**
 * @brief	�����N�A�b�v(castle_rank.c����ړ�)
 *
 * @param	wk		CASTLE_RANK_WORK�^�̃|�C���^
 * @param	param	FC_PARAM_INFO_RANKUP
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleRank_SeqSubRankUp(CASTLE_ENEMY_WORK* wk, u8 parent_decide_pos, u8 param)
{
	u8	type_offset,sel_pos,type,m_max;
	u32 now_rank;
	u16 buf16[4];

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );

	if( param == FC_PARAM_INFO_RANKUP ){
		type = CASTLE_RANK_TYPE_INFO;
	}else{
		OS_Printf( "CastleRank_SeqSubRankUp ����param���s���ł��I\n" );
		GF_ASSERT( 0 );
	}

	//�I�t�Z�b�g
	type_offset = wk->modoru_pos;	

	//�I�񂾃J�[�\���ʒu���擾
	sel_pos = GetCommSelCsrPos( type_offset, parent_decide_pos );

	////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�F�e�̌��肪�L���Ȃ�
		if( parent_decide_pos < type_offset ){
			Castle_SetPlayerName( wk, 5 );
			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			Castle_CPRecord_Sub( wk->fro_sv, wk->type, INFO_RANKUP_POINT );

			//�����N�A�b�v
			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			buf16[0] = (now_rank + 1);
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
				CastleScr_GetRankRecordID(wk->type,type),
				Frontier_GetFriendIndex(CastleScr_GetRankRecordID(wk->type,type)), (now_rank + 1) );
			OS_Printf( "�e�F�e�������N�A�b�v %d\n", (now_rank+1) );

			//�����N�A�b�v�������ɖ߂�悪����t���OON
			if( Castle_CommCheck(wk->type) == TRUE ){
				wk->recover_flag = RECOVER_RANKUP;
			}

		//�e�F�q�̌��肪�L���Ȃ�
		}else{
			Castle_SetPairName( wk->wordset, 5 );
			now_rank= wk->pair_rank[type];
			wk->pair_cp -= INFO_RANKUP_POINT;						//CP���炷
			wk->pair_rank[type]++;
		}

	///////////////////////////////////////////////
	//�q
	}else{

		//�q�F�e�̌��肪�L���Ȃ�
		if( parent_decide_pos < type_offset ){
			Castle_SetPairName( wk->wordset, 5 );
			now_rank= wk->pair_rank[type];
			wk->pair_cp -= INFO_RANKUP_POINT;						//CP���炷
			wk->pair_rank[type]++;

		//�q�F�q�̌��肪�L���Ȃ�
		}else{
			Castle_SetPlayerName( wk, 5 );
			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			Castle_CPRecord_Sub( wk->fro_sv, wk->type, INFO_RANKUP_POINT );

			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			buf16[0] = (now_rank + 1);
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
				CastleScr_GetRankRecordID(wk->type,type),
				Frontier_GetFriendIndex(CastleScr_GetRankRecordID(wk->type,type)), (now_rank + 1) );
			OS_Printf( "�q�F�q�������N�A�b�v %d\n", (now_rank+1) );

			//�����N�A�b�v�������ɖ߂�悪����t���OON
			if( Castle_CommCheck(wk->type) == TRUE ){
				wk->recover_flag = RECOVER_RANKUP;
			}
		}
	}

	//���j���[��\�����Ă�����폜
	CastleEnemy_SeqSubMenuWinClear( wk );

	//CP�\�����X�V
	CastleEnemy_SeqSubCPWrite( wk, &wk->bmpwin[ENEMY_BMPWIN_TR1] );

	//CastleRank_SeqSubAllInfoWrite( wk );							//���X�V

	//��b�E�B���h�E�\��
	CastleTalkWinPut( &wk->bmpwin[ENEMY_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );

	//�u�����N���A�b�v���܂����I�v
	wk->msg_index = Castle_EasyMsg( wk, rankup_pair_msg_tbl[type][now_rank], FONT_TALK );
	//Snd_SePlay( SEQ_SE_DP_PIRORIRO2 );
	//Snd_SePlay( SEQ_SE_DP_DANSA4 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����N�����Ċ���\������
 *
 * @param	wk		CASTLE_ENEMY_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleRank_SeqSubHataVanish( CASTLE_ENEMY_WORK* wk )
{
	u8 m_max;
	int i;

	m_max = Castle_GetEnemyPokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=0; i < m_max ;i++ ){
		Castle_HataVanish( wk, i );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����N�̃��x������A���̃o�j�b�V���𑀍�(�\���ɂ���)
 *
 * @param	wk			CASTLE_ENEMY_WORK�^�̃|�C���^
 * @param	no			�ǂꂩ
 * @param	level		���݂̃����N
 * @param	flag		�e���q��
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Castle_HataVanish( CASTLE_ENEMY_WORK* wk, u8 no )
{
	if( wk->p_tuyosa_flag[no] == 1 ){
		CastleObj_Vanish( wk->p_hata[no][0], CASTLE_VANISH_OFF );		//�\��
	}

	if( wk->p_waza_flag[no] == 1 ){
		CastleObj_Vanish( wk->p_hata[no][1], CASTLE_VANISH_OFF );		//�\��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	��OBJ�I�t�Z�b�g�擾
 *
 * @param	
 *
 * @return	"�I�t�Z�b�g"
 */
//--------------------------------------------------------------
static void Castle_GetHataXY( CASTLE_ENEMY_WORK* wk, u32* x, u32* y )
{
	if( Castle_CommCheck(wk->type) == TRUE ){
		*x = CASTLE_MULTI_HATA_START_X;
	}else{
		*x = CASTLE_HATA_START_X;
	}

	*y = CASTLE_HATA_START_Y;
	return;
}



