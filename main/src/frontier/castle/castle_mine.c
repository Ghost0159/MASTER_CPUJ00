//==============================================================================================
/**
 * @file	castle_mine.c
 * @brief	�u�o�g���L���b�X�� �莝���|�P������ʁv���C���\�[�X
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
#include "poketool/status_rcv.h"
#include "itemtool/item.h"
#include "savedata/b_tower.h"
#include "savedata/frontier_savedata.h"

#include "communication/comm_system.h"
#include "communication/comm_tool.h"
#include "communication/comm_def.h"
#include "communication/comm_info.h"
#include "communication/wm_icon.h"

#include "castle_sys.h"
#include "castle_common.h"
#include "castle_clact.h"
#include "castle_bmp.h"
#include "castle_obj.h"
#include "application/castle.h"
#include "../frontier_tool.h"						//Frontier_PokeParaMake
#include "../castle_tool.h"							//
#include "../castle_def.h"
#include "../comm_command_frontier.h"

#include "../../field/comm_direct_counter_def.h"	//
#include "../../field/fieldobj.h"
#include "../../field/scr_tool.h"

#include "msgdata\msg.naix"							//NARC_msg_??_dat
#include "msgdata\msg_castle_poke.h"				//msg_??

#include "../graphic/frontier_obj_def.h"
#include "../graphic/frontier_bg_def.h"

#include "castle_item.dat"							//����A�؂̎��f�[�^
#include "castle_mine_def.h"						//

#include "system/pm_overlay.h"
FS_EXTERN_OVERLAY(frontier_common);


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

//��{���X�g�̑I����
enum{
	SEL_KAIHUKU = 0,
	SEL_RENTAL,
	SEL_TUYOSA,
	SEL_WAZA,
	SEL_TOZIRU,
	CASTLE_MINE_SEL_MAX,
};

//�������肵������`
enum{
	FC_PARAM_KAIHUKU = 0,								//�����ӂ�
	FC_PARAM_LIST_START_KAIHUKU,						//"�����ӂ����X�g�I�t�Z�b�g"
	FC_PARAM_HP_KAIHUKU = FC_PARAM_LIST_START_KAIHUKU,	//�g�o�����ӂ�
	FC_PARAM_PP_KAIHUKU,								//�o�o�����ӂ�
	FC_PARAM_HPPP_KAIHUKU,								//���ׂĂ����ӂ�
	FC_PARAM_KAIHUKU_RANKUP,							//(��)�����N�A�b�v
	/////////////////////////////
	FC_PARAM_RENTAL,									//�����^��
	FC_PARAM_LIST_START_RENTAL,							//"�����^�����X�g�I�t�Z�b�g"
	FC_PARAM_KINOMI = FC_PARAM_LIST_START_RENTAL,		//���̂�
	FC_PARAM_ITEM,										//�ǂ���
	FC_PARAM_RENTAL_RANKUP,								//(�����^��)�����N�A�b�v
	/////////////////////////////
	FC_PARAM_TUYOSA,									//�悳
	FC_PARAM_WAZA,										//�킴
	FC_PARAM_TOZIRU,									//�Ƃ���
};

//�����N�A�b�v�G�t�F�N�g�\���ʒu
#define CASTLE_MINE_RANKUP_X	(204)
#define CASTLE_MINE_RANKUP_Y	(100)
#define CASTLE_MINE_RANKUP_X2	(211)
#define CASTLE_MINE_RANKUP_Y2	(106)


//==============================================================================================
//
//	�񕜊֘A�̒�`
//
//==============================================================================================
//�񕜎g�p�������̃��b�Z�[�W�e�[�u��
static const u16 kaihuku_msg[] = {
	msg_castle_poke_04,			//�g�o�����ӂ�
	msg_castle_poke_05,			//�o�o�����ӂ�
	msg_castle_poke_06,			//���ׂĂ����ӂ�
};

//�񕜂̎g�pCP
static const u16 kaihuku_cp_tbl[] = {
	//HP�����ӂ��APP�����ӂ��A���ׂĂ����ӂ�
	10,8,12
};

//�񕜂�I�񂾎��ɕ\�����郊�X�g
static const u16 kaihuku_item_tbl[][3] = {
	//�����N�A�A�C�e���A���X�g�p�����[�^
	{ 1, msg_castle_kaifuku_01_01,	FC_PARAM_HP_KAIHUKU },			//�g�o�����ӂ�
	{ 2, msg_castle_kaifuku_01_02,	FC_PARAM_PP_KAIHUKU },			//�o�o�����ӂ�
	{ 3, msg_castle_kaifuku_01_03,	FC_PARAM_HPPP_KAIHUKU },		//���ׂĂ����ӂ�
	//�����N�A�b�v
	//��߂�
};

//�񕜃��X�g���b�Z�[�W�e�[�u��
static const u32 kaihuku_msg_tbl[][3] = {
	//�����N�Amsg_id�A���X�g�p�����[�^
	{ 1, msg_castle_kaifuku_01_01,	FC_PARAM_HP_KAIHUKU },			//�g�o�����ӂ�
	{ 2, msg_castle_kaifuku_01_02,	FC_PARAM_PP_KAIHUKU },			//�o�o�����ӂ�
	{ 3, msg_castle_kaifuku_01_03,	FC_PARAM_HPPP_KAIHUKU }, 		//���ׂĂ����ӂ�
	{ 1, msg_castle_kaifuku_02,		FC_PARAM_KAIHUKU_RANKUP }, 		//�����N�A�b�v
	{ 1, msg_castle_kaifuku_03,		BMPLIST_CANCEL },				//��߂�
};
#define KAIHUKU_MSG_TBL_MAX		( NELEMS(kaihuku_msg_tbl) )


//==============================================================================================
//
//	�����N�A�b�v�֘A�̒�`
//
//==============================================================================================
//�����N�A�b�v�ɕK�v�ȃ|�C���g
static const u16 rankup_point_tbl[CASTLE_RANK_TYPE_MAX][CASTLE_RANK_MAX] = {
	{ 0, 100, 100 },		//��
	{ 0, 100, 150 },		//�����^��
	{ 0, 50, 50 },		//���(castle_enemy.c��INFO_RANKUP_POINT���g�p���Ă���)
};

//�ʐM����ɕ\������郁�b�Z�[�W
static const u16 rankup_pair_msg_tbl[CASTLE_RANK_TYPE_MAX][CASTLE_RANK_MAX] = {
//���񕜂̃����N�A�b�v�Q
//�u�����L�Y������Ł@�����ӂ��@�ł���悤�Ɂ@�Ȃ�܂����I�v
//���񕜂̃����N�A�b�v�R
//�u�������L�Y������Ɓ@�s�[�s�[�}�b�N�X�Ł@�����ӂ��@�ł���悤�Ɂ@�Ȃ�܂����I�v
	{	0, msg_castle_poke_27,	msg_castle_poke_28 },

//������̃����N�A�b�v�Q
//������̃����N�A�b�v�R
//�u�ǂ������@�����^���@�ł���悤�Ɂ@�Ȃ�܂����I�v
//�u�����^���ł���@���݂̂Ɓ@�ǂ����� ����邢���@�ӂ��܂����I�v
	{	0, msg_castle_poke_38,	msg_castle_poke_39 },

//�����̃����N�A�b�v�Q
//�����̃����N�A�b�v�R
	{	0, 0, 0 },							//���g�p
};


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
struct _CASTLE_MINE_WORK{

	PROC* proc;										//PROC�ւ̃|�C���^
	FRONTIER_SAVEWORK* fro_sv;						//

	u8	sub_seq;									//�V�[�P���X
	u8	type;										//�����Ƃ��ēn���ꂽ�o�g���^�C�v
	u8	msg_index;									//���b�Z�[�Windex
	u8	wait;

	u8	tmp_csr_pos;								//�ޔ����Ă���J�[�\���ʒu
	u8	csr_pos;									//���݂̃J�[�\���ʒu
	u8	list_flag:1;								//���X�g�\�������t���O
	u8	send_req:1;									//���M���N�G�X�g�t���O
	u8	eff_init_flag:1;							//����G�t�F�N�g�������t���O
	u8	item_list_flag:1;							//�A�C�e�����X�g�\�������t���O
	u8	menu_flag:1;								//���j���[�\�������t���O
	u8	rankup_recover_flag:2;						//�����N�A�b�v�������ɖ߂�悪����t���O
	u8	dummy25_flag:1;								//�����N�A�b�v�������ɖ߂�悪����t���O
	u8	recieve_count;								//��M�J�E���g

	u16 parent_decide_item;							//���肵���A�C�e���i���o�[
	u8	parent_decide_pos;							//���肵���J�[�\���ʒu(�ǂ̃|�P������)
	u8	parent_decide_type;							//���肵������(FC_PARAM_??)

	u8	h_max;
	u8	modoru_pos;
	u16 list_csr_pos;								//���X�g�őI�������ʒu

	//u32 before_hp;

	u16	basic_list_lp;
	u16	basic_list_cp;

	MSGDATA_MANAGER* msgman_iteminfo;				//���b�Z�[�W�}�l�[�W���[
	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g
	STRBUF* msg_buf;								//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;								//�e���|�����o�b�t�@�|�C���^

	STRBUF* menu_buf[CASTLE_MENU_BUF_MAX];			//���j���[�o�b�t�@�|�C���^
	STRCODE str[PERSON_NAME_SIZE + EOM_SIZE];		//���j���[�̃��b�Z�[�W

	GF_BGL_INI*	bgl;								//BGL�ւ̃|�C���^
	GF_BGL_BMPWIN bmpwin[CASTLE_MINE_BMPWIN_MAX];	//BMP�E�B���h�E�f�[�^

	//BMP���j���[(bmp_menu.h)
	BMPMENU_HEADER MenuH;							//BMP���j���[�w�b�_�[
	BMPMENU_WORK* mw;								//BMP���j���[���[�N
	BMPMENU_DATA Data[CASTLE_MENU_BUF_MAX];			//BMP���j���[�f�[�^

	//BMP���X�g
	BMPLIST_WORK* lw;								//BMP���X�g�f�[�^
	BMPLIST_DATA* menulist;							//
	BMPLIST_HEADER list_h;

	PALETTE_FADE_PTR pfd;							//�p���b�g�t�F�[�h

	NUMFONT* num_font;								//8x8�t�H���g

	//const CONFIG* config;							//�R���t�B�O�|�C���^
	CONFIG* config;									//�R���t�B�O�|�C���^
	SAVEDATA* sv;									//�Z�[�u�f�[�^�|�C���^
	CASTLEDATA* castle_sv;							//�L���b�X���Z�[�u�f�[�^�|�C���^
	CASTLESCORE* score_sv;							//�L���b�X���Z�[�u�f�[�^�|�C���^

	CASTLE_CLACT castle_clact;						//�Z���A�N�^�f�[�^
	CASTLE_OBJ* p_scr_u;							//�X�N���[���J�[�\��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_scr_d;							//�X�N���[���J�[�\��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_itemicon;							//�A�C�e���A�C�R��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_icon[CASTLE_COMM_POKE_TOTAL_NUM];	//�A�C�R��OBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_itemkeep[CASTLE_COMM_POKE_TOTAL_NUM];	//�A�C�e�������Ă���A�C�R��OBJ�̃|�C���^
	CASTLE_OBJ* p_eff;								//�G�t�F�N�gOBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_poke_sel;							//�|�P�����I���E�B��OBJ�̃|�C���^
	CASTLE_OBJ* p_pair_poke_sel;					//�y�A�̃|�P�����I���E�B��OBJ�̃|�C���^
	CASTLE_OBJ* p_hp[CASTLE_COMM_POKE_TOTAL_NUM];	//HP���OBJ�̃|�C���^
	CASTLE_OBJ* p_eff_rankup;						//�����N�A�b�vOBJ�̃|�C���^�i�[�e�[�u��
	CASTLE_OBJ* p_item_csr;							//�A�C�e���J�[�\��OBJ�̃|�C���^�i�[�e�[�u��

	u16* p_ret_work;								//CASTLE_CALL_WORK�̖߂�l���[�N�ւ̃|�C���^
	POKEPARTY* p_party;

	ARCHANDLE* hdl;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//CASTLE_COMM castle_comm;
#if 1
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[CASTLE_COMM_BUF_LEN];

	//�ʐM�p
	u8	pair_csr_pos;								//�p�[�g�i�[�̃J�[�\���ʒu
	u8	pair_sel_pos;								//�p�[�g�i�[�̑I�������ʒu
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
PROC_RESULT CastleMineProc_Init( PROC * proc, int * seq );
PROC_RESULT CastleMineProc_Main( PROC * proc, int * seq );
PROC_RESULT CastleMineProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameInit( CASTLE_MINE_WORK* wk );
static void TypeSelInit( CASTLE_MINE_WORK* wk );
static BOOL Seq_GameTypeSel( CASTLE_MINE_WORK* wk );
static BOOL Seq_GameSendRecv( CASTLE_MINE_WORK* wk );
static BOOL Seq_GameEndMulti( CASTLE_MINE_WORK* wk );
static BOOL Seq_GameEnd( CASTLE_MINE_WORK* wk );

//���ʏ���
static void CastleCommon_Delete( CASTLE_MINE_WORK* wk );
static void Castle_InitSub1( void );
static void Castle_InitSub2( CASTLE_MINE_WORK* wk );

//���ʏ������A�I��
static void Castle_ObjInit( CASTLE_MINE_WORK* wk );
static void Castle_BgInit( CASTLE_MINE_WORK* wk );
static void Castle_BgExit( GF_BGL_INI * ini );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//BG�O���t�B�b�N�f�[�^
static void Castle_SetMainBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  );
static void Castle_SetMainBgPalette( void );
static void Castle_SetStatusBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  );
static void Castle_SetWazaBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  );
static void Castle_SetItemBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  );
static void Castle_SetStatusBgPalette( void );
static void Castle_SetSubBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  );

//���b�Z�[�W
static u8 CastleWriteMsg( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 CastleWriteMsgSimple( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 Castle_EasyMsg( CASTLE_MINE_WORK* wk, int msg_id, u8 font );
static void Castle_StatusMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke );
static void StMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, int msg, u16 x, u16 y );
static void Castle_WazaMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke );
static void WazaMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u32 msg_id, u32 msg_id2, POKEMON_PARAM* poke, u32 id, u32 id2, u32 id3 );
static void Castle_PokeHpMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void Castle_PokeHpMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u8 flag );
static void Castle_PokeLvMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void Castle_PokeLvMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u8 flag );
static void Castle_NameCPWinWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void Castle_NameCPWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void Castle_PokeHpEff( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u32 hp );
static void CastleMine_Default_Write( CASTLE_MINE_WORK* wk );
static void CastleMine_Default_Del( CASTLE_MINE_WORK* wk );
static void CastleMine_Basic_Write( CASTLE_MINE_WORK* wk );
static void CastleMine_Basic_Del( CASTLE_MINE_WORK* wk );
static void CastleMine_Kaihuku_Write( CASTLE_MINE_WORK* wk );
static void CastleMine_Kaihuku_Del( CASTLE_MINE_WORK* wk );
static void CastleMine_Rental_Write( CASTLE_MINE_WORK* wk );
static void CastleMine_Rental_Del( CASTLE_MINE_WORK* wk );
static void CastleMine_ItemSeed_Write( CASTLE_MINE_WORK* wk, u8 type );
static void CastleMine_ItemSeed_Del( CASTLE_MINE_WORK* wk );

//���j���[
static void CastleInitMenu( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max );
static void CastleSetMenuData( CASTLE_MINE_WORK* wk, u8 no, u8 param, int msg_id );
static void Castle_SetMenu2( CASTLE_MINE_WORK* wk );

//���X�g
static void Castle_ItemListMake( CASTLE_MINE_WORK* wk, u8 decide_type );
static void Castle_CsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode );
static void Castle_LineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y );
static void Castle_KaihukuListMake( CASTLE_MINE_WORK* wk );
static void Castle_KaihukuCsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode );
static void Castle_KaihukuLineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y );
static void Castle_RentalListMake( CASTLE_MINE_WORK* wk );
static void Castle_RentalCsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode );
static void Castle_RentalLineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y );
static void Castle_BasicListMake( CASTLE_MINE_WORK* wk );
static void Castle_BasicCsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode );

//������
static void Castle_SetNumber( CASTLE_MINE_WORK* wk, u32 bufID, s32 number, u32 keta, NUMBER_DISPTYPE disp );
static void Castle_SetPokeName( CASTLE_MINE_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp );
static void Castle_SetPlayerName( CASTLE_MINE_WORK* wk, u32 bufID );
static void PlayerNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PairNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PokeNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font );
static void PokeNameWriteEx( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex );
static void PokeSexWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font, u8 sex );
static void TalkWinWrite( CASTLE_MINE_WORK* wk );
static u8 CastleItemInfoWriteMsg( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u16 item );

//�c�[��
static BOOL Castle_CheckType( CASTLE_MINE_WORK* wk, u8 type );
static void NextSeq( CASTLE_MINE_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( CASTLE_MINE_WORK* wk );
static void CsrMove( CASTLE_MINE_WORK* wk, int key );
static void CsrMoveSub( CASTLE_MINE_WORK* wk );
static void PokeSelMoveSub( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 flag );
static void CastleMine_GetPokeSelXY( CASTLE_MINE_WORK* wk, u32* x, u32* y, u8 csr_pos );
static u16 GetCsrX( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 lr );
static u16 GetCsrY( CASTLE_MINE_WORK* wk, u8 csr_pos );
static u16 GetItemCP( CASTLE_MINE_WORK* wk, u16 csr_pos, u8 type );
static u16 GetItemCPByItemNo( u16 item );
static u16 GetItemNo( CASTLE_MINE_WORK*wk, u16 csr_pos, u8 type );
static void Castle_GetOffset( CASTLE_MINE_WORK* wk, u16* offset_x, u16* offset_y, u16* pair_offset_x, u16* pair_offset_y );
static BOOL CastleMine_DecideEff( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 sel_type );
static void CastleMine_ItemWinPokeIconMove( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 flag );
static u32 CastleMine_GetPokeAnmNo( CASTLE_MINE_WORK* wk, u8 gauge_color );
static u32 CastleMine_GetHpAnmNo( CASTLE_MINE_WORK* wk, u8 gauge_color );
static void CastleMine_StatusWinChg( CASTLE_MINE_WORK* wk, s8 add_sub );
static void CastleMine_WazaWinChg( CASTLE_MINE_WORK* wk, s8 add_sub );
static void CastleMine_PairDecideDel( CASTLE_MINE_WORK* wk );
static BOOL CastleMine_PPRecoverCheck( POKEMON_PARAM* poke );
static void BmpTalkWinClearSub( GF_BGL_BMPWIN* win );
static void BmpTalkWinPutSub( CASTLE_MINE_WORK* wk );

//�ʐM
BOOL CastleMine_CommSetSendBuf( CASTLE_MINE_WORK* wk, u16 type, u16 param );
void CastleMine_CommSendBufBasicData( CASTLE_MINE_WORK* wk, u16 type );
void CastleMine_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
void CastleMine_CommSendBufRankUpType( CASTLE_MINE_WORK* wk, u16 type, u16 param );
void CastleMine_CommRecvBufReqType(int id_no,int size,void *pData,void *work);
void CastleMine_CommSendBufCsrPos( CASTLE_MINE_WORK* wk, u16 type );
void CastleMine_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);
void CastleMine_CommSendBufModoru( CASTLE_MINE_WORK* wk );
void CastleMine_CommRecvBufModoru(int id_no,int size,void *pData,void *work);

//�񕜏���
static void CastleMine_ItemUse( POKEMON_PARAM* pp, u16 item );

//�T�u�V�[�P���X
static void CastleMine_SeqSubNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void CastleMine_SeqSubKaihuku( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 type );
static void CastleMine_SeqSubItem( CASTLE_MINE_WORK* wk, u8 csr_pos, u16 item );
static void CastleMine_SeqSubMenuWinClear( CASTLE_MINE_WORK* wk );
static void CastleMine_SeqSubListStart( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void CastleMine_SeqSubItemListEnd( CASTLE_MINE_WORK* wk );
static void CastleMine_SeqSubItemListReturn( CASTLE_MINE_WORK* wk );
static void CastleMine_SeqSubListEnd( CASTLE_MINE_WORK* wk );
static void CastleMine_SeqSubCPWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win );
static void CastleMine_SeqSubRankUp( CASTLE_MINE_WORK* wk, u8 parent_decide_pos, u8 parent_decide_type );
static void CastleMine_SeqSubStatusWin( CASTLE_MINE_WORK* wk, u8 csr_pos );
static void CastleMine_SeqSubWazaWin( CASTLE_MINE_WORK* wk, u8 csr_pos );
static void CastleMine_SeqSubStatusWazaDel( CASTLE_MINE_WORK* wk );
static void CastleMine_IconSelAnm( CASTLE_MINE_WORK* wk );

//�f�o�b�N
static void Debug_HpDown( CASTLE_MINE_WORK* wk, u8 no );

//�����N�A�b�v����(castle_rank.c���ړ�)
static void CastleRank_SeqSubRankUp( CASTLE_MINE_WORK* wk, u8 parent_decide_pos, u8 param );


//==============================================================================================
//
//	���X�g�f�[�^
//
//==============================================================================================
static const BMPLIST_HEADER CastleListH = {
	NULL,						//�\�������f�[�^�|�C���^

	Castle_CsrMoveCallBack,		//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	Castle_LineWriteCallBack,	//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,						//GF_BGL_BMPWIN�̃|�C���^

	(CASTLE_ITEM_ALL_MAX+1),	//���X�g���ڐ�
	7,							//�\���ő區�ڐ�

	0,							//���x���\���w���W
	8,							//���ڕ\���w���W
	0,							//�J�[�\���\���w���W
	0,							//�\���x���W
	FBMP_COL_BLACK,				//�����F
	//FBMP_COL_WHITE,			//�w�i�F
	FBMP_COL_NULL,				//�w�i�F
	FBMP_COL_BLK_SDW,			//�����e�F
	0,							//�����Ԋu�w
	16,							//�����Ԋu�x
	BMPLIST_NO_SKIP,			//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,				//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0,							//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)

	NULL,
};


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
PROC_RESULT CastleMineProc_Init( PROC * proc, int * seq )
{
	int i;
	CASTLE_MINE_WORK* wk;
	CASTLE_CALL_WORK* castle_call;

	Overlay_Load( FS_OVERLAY_ID(frontier_common), OVERLAY_LOAD_NOT_SYNCHRONIZE );

	Castle_InitSub1();

	//sys_CreateHeap( HEAPID_BASE_APP, HEAPID_CASTLE, 0x20000 );
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_CASTLE, 0x25000 );

	wk = PROC_AllocWork( proc, sizeof(CASTLE_MINE_WORK), HEAPID_CASTLE );
	memset( wk, 0, sizeof(CASTLE_MINE_WORK) );

	wk->bgl				= GF_BGL_BglIniAlloc( HEAPID_CASTLE );
	wk->proc			= proc;
	castle_call			= (CASTLE_CALL_WORK*)PROC_GetParentWork( proc );
	wk->sv				= castle_call->sv;
	wk->castle_sv		= SaveData_GetCastleData( wk->sv );
	wk->score_sv		= SaveData_GetCastleScore( wk->sv );
	wk->type			= castle_call->type;
	wk->p_ret_work		= &castle_call->ret_work;
	wk->config			= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾
	wk->p_party			= castle_call->p_m_party;
	wk->parent_decide_pos = CASTLE_MINE_DECIDE_NONE;
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

	Castle_InitSub2( wk );

	if( Castle_CommCheck(wk->type) == TRUE ){
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
PROC_RESULT CastleMineProc_Main( PROC * proc, int * seq )
{
	CASTLE_MINE_WORK* wk  = PROC_GetWork( proc );

	//OS_Printf( " *seq = %d\n", *seq );

	//���肪�u�߂�v��I�񂾎�
	if( wk->pair_modoru_flag == 1 ){

		switch( *seq ){

		//�ʐM�I����
		case SEQ_GAME_TYPE_SEL:
		//case SEQ_GAME_SEND_RECV:					//08.06.17 ����M���͋����I�����Ȃ�
			wk->pair_modoru_flag = 0;
			CastleMine_PairDecideDel( wk );

			//�G�t�F�N�g�����I������悤�ɂ���
			if( wk->p_eff != NULL ){
				CastleObj_Delete( wk->p_eff );
				wk->eff_init_flag = 0;
			}

			//�u�����񂪖߂��I�����܂����I�v
			CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
			Frontier_PairNameWordSet( wk->wordset, 0 );

			wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_01_01, FONT_TALK );

			NextSeq( wk, seq, SEQ_GAME_END_MULTI );
			break;
		};

	//���肪�������肵����
	}else if( wk->parent_decide_pos != CASTLE_MINE_DECIDE_NONE ){
		switch( *seq ){

		//����M�֋����ړ�
		case SEQ_GAME_TYPE_SEL:
		case SEQ_GAME_END_MULTI:					//08.06.17 ��Ɂu�߂�v���̎��́A
			wk->pair_modoru_flag = 0;				//08.06.17 �u����M�v�ɖ߂�
			CastleMine_PairDecideDel( wk );
			NextSeq( wk, seq, SEQ_GAME_SEND_RECV );
			break;

		//�������Ȃ�
		//case SEQ_GAME_INIT:
		//case SEQ_GAME_SEND_RECV:
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

		//BgCheck( wk );
#if 0
		if( sys.cont & PAD_BUTTON_L ){
			//CastleObj_SetObjPos(wk->p_icon[0], 
			//					CASTLE_ITEM_POKE_ICON_X, CASTLE_ITEM_POKE_ICON_Y);
			Debug_HpDown( wk, 0 );
			Debug_HpDown( wk, 1 );
			Debug_HpDown( wk, 2 );
			//Debug_HpDown( wk, 3 );
			Castle_PokeHpMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_HP] );
			break;
		}
#endif

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

	CastleMine_IconSelAnm( wk );					//�A�C�R���㉺�A�j��
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
PROC_RESULT CastleMineProc_End( PROC * proc, int * seq )
{
	int i;
	CASTLE_MINE_WORK* wk = PROC_GetWork( proc );

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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( CASTLE_MINE_WORK* wk )
{
	switch( wk->sub_seq ){

	//�R�}���h�ݒ��̓���
	case 0:
		if( Castle_CommCheck(wk->type) == TRUE ){
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_CASTLE_MINE_INIT );
		}
		wk->sub_seq++;
		break;

	//�^�C�~���O�R�}���h���͂������m�F
	case 1:
		if( Castle_CommCheck(wk->type) == TRUE ){
			if( CommIsTimingSync(DBC_TIM_CASTLE_MINE_INIT) == TRUE ){
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
			if( CastleMine_CommSetSendBuf(wk,CASTLE_COMM_MINE_PAIR,0) == TRUE ){
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static void TypeSelInit( CASTLE_MINE_WORK* wk )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,icon_offset_x;
	GF_BGL_BMPWIN* win;

	//�ʐM���Ă��邩�ŕ\���̃I�t�Z�b�g���ς��
	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	///////////////////////////////////////////////////////////////////////////////
	win = &wk->bmpwin[MINE_BMPWIN_TR1];
	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );					//�h��Ԃ�
	CastleMine_SeqSubNameWrite( wk, win );							//�v���C���[�A�p�[�g�i�[���\��
	CastleMine_SeqSubCPWrite( wk, win );							//CP�\��

	///////////////////////////////////////////////////////////////////////////////
	//HP��LV�\��
	Castle_PokeHpMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_HP] );
	Castle_PokeLvMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_LV] );

	///////////////////////////////////////////////////////////////////////////////
	CastleMine_Default_Write( wk );									//TALKMENU,���ǂ�

	GF_Disp_DispOn();
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���^�C�v�I��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
enum{
	SEQ_SUB_START = 0,
	SEQ_SUB_MENU_MAIN,
	/////////////////////////////////////////////////////////////////////
	SEQ_SUB_KAIHUKU_ITEM_LIST,					//�񕜂���A�C�e����I��
	SEQ_SUB_KAIHUKU_YES_NO,						//����ł悢�u�͂��E�������v
	SEQ_SUB_KAIHUKU_RANKUP_YES_NO,				//�񕜃����N�A�b�v
	SEQ_SUB_KAIHUKU_RANKUP_DECIDE_EFF_WAIT,		//�񕜃����N�A�b�v����G�t�F�N�g
	SEQ_SUB_KAIHUKU_RANKUP_DECIDE_EFF_KEY_WAIT,	//�񕜃����N�A�b�v����G�t�F�N�g�L�[�҂�
	SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN,		//�L�[�҂����āA���X�g�֖߂�
	/////////////////////////////////////////////////////////////////////
	SEQ_SUB_RENTAL_LIST,						//�����^�����邱�Ƃ�I��
	SEQ_SUB_RENTAL_ITEM_SEED_LIST,				//���̂݁A�A�C�e���̃��X�g
	SEQ_SUB_RENTAL_YES_NO,						//����ł悢�u�͂��E�������v
	SEQ_SUB_RENTAL_RANKUP_YES_NO,				//�����^�������N�A�b�v
	SEQ_SUB_RENTAL_RANKUP_DECIDE_EFF_WAIT,		//�����^�������N�A�b�v����G�t�F�N�g
	SEQ_SUB_RENTAL_RANKUP_DECIDE_EFF_KEY_WAIT,	//�����^�������N�A�b�v����G�t�F�N�g�L�[�҂�
	SEQ_SUB_RENTAL_KEY_WAIT_LIST_RETURN,		//�L�[�҂����āA�����^�����X�g�֖߂�
	SEQ_SUB_RENTAL_KEY_WAIT_ITEM_SEED_RETURN,	//�L�[�҂����āA���̂݁A����X�g�֖߂�
	/////////////////////////////////////////////////////////////////////
	SEQ_SUB_RENTAL_ITEM_TRADE,					//���łɃA�C�e�������Ă�
	SEQ_SUB_RENTAL_ITEM_TRADE_YES_NO,			//����ł悢�u�͂��E�������v
	/////////////////////////////////////////////////////////////////////
	SEQ_SUB_DECIDE_EFF_WAIT,					//����G�t�F�N�g
	SEQ_SUB_DECIDE_KEY_WAIT,					//����G�t�F�N�g��̃L�[�҂�
	/////////////////////////////////////////////////////////////////////
	SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF,		//�L�[�҂����ăE�B���h�E�I�t����BASIC���X�g��
	SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF,			//�L�[�҂����ăE�B���h�E�I�t����BASIC���X�g��
};

static BOOL Seq_GameTypeSel( CASTLE_MINE_WORK* wk )
{
	u8 sel_pos,sel_type;
	u8 now_rank;
	u16 now_cp;
	int i;
	u32 color,ret;
	POKEMON_PARAM* poke;

	switch( wk->sub_seq ){

	//////////////////////////////////////////////////////////////////////////////////////
	//�^�C�v��I��
	case SEQ_SUB_START:

		//�����N�A�b�v�������ɖ߂�悪���ꂩ�`�F�b�N
		if( wk->rankup_recover_flag == 1 ){
			//BmpTalkWinClear( &wk->bmpwin[MINE_BMPWIN_TALK], WINDOW_TRANS_OFF );
			CastleMine_Kaihuku_Write( wk );

			//�����N�A�b�v�G�t�F�N�g
			CastleObj_RankUpEff( wk->p_eff_rankup, CASTLE_MINE_RANKUP_X, CASTLE_MINE_RANKUP_Y );

			wk->sub_seq = SEQ_SUB_KAIHUKU_ITEM_LIST;
			wk->rankup_recover_flag = 0;	//�����N�A�b�v�������ɖ߂�悪����t���OOFF
			return FALSE;
		}else if( wk->rankup_recover_flag == 2 ){
			//BmpTalkWinClear( &wk->bmpwin[MINE_BMPWIN_TALK], WINDOW_TRANS_OFF );
			CastleMine_Rental_Write( wk );								//TALK,RENTAL

			//�����N�A�b�v�G�t�F�N�g
			CastleObj_RankUpEff( wk->p_eff_rankup, CASTLE_MINE_RANKUP_X2, CASTLE_MINE_RANKUP_Y2 );

			wk->sub_seq = SEQ_SUB_RENTAL_LIST;
			wk->rankup_recover_flag = 0;	//�����N�A�b�v�������ɖ߂�悪����t���OOFF
			return FALSE;
		}

		CsrMove( wk, sys.trg );

		//����{�^������������
		if( sys.trg & PAD_BUTTON_A ){
			Snd_SePlay( SEQ_SE_DP_SELECT );

			//�u���ǂ�v
			if( wk->csr_pos >= wk->modoru_pos ){
				return TRUE;
			}else{
				CastleMine_Default_Del( wk );		//TALKMENU,���ǂ�
				CastleMine_Basic_Write( wk );		//TALKMENU2,BASIC
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

		switch( ret ){

		case BMPLIST_NULL:
			break;

		case BMPLIST_CANCEL:
			CastleMine_Basic_Del( wk );			//TALKMENU2,BASIC
			CastleMine_Default_Write( wk );		//TALKMENU,���ǂ�
			wk->sub_seq = SEQ_SUB_START;
			break;

		//��
		case FC_PARAM_KAIHUKU:
			CastleMine_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleMine_Kaihuku_Write( wk );
			wk->sub_seq = SEQ_SUB_KAIHUKU_ITEM_LIST;
			break;

		//�����^��
		case FC_PARAM_RENTAL:
			CastleMine_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleMine_Rental_Write( wk );	//TALK,RENTAL
			wk->sub_seq = SEQ_SUB_RENTAL_LIST;
			break;

		//�����Ă��铹��A���i�A�����A�e�\�͒l��\��
		case FC_PARAM_TUYOSA:
			CastleMine_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleMine_SeqSubStatusWin( wk, wk->csr_pos );
			wk->sub_seq = SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF;
			break;

		//�Z���A�o�o����\��
		case FC_PARAM_WAZA:
			CastleMine_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleMine_SeqSubWazaWin( wk, wk->csr_pos );
			wk->sub_seq = SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF;
			break;

		//�Ƃ���
		case FC_PARAM_TOZIRU:
			CastleMine_Basic_Del( wk );		//TALKMENU2,BASIC
			CastleMine_Default_Write( wk );	//TALKMENU,���ǂ�
			wk->sub_seq = SEQ_SUB_START;
			break;
		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�����ӂ��v�g���A�C�e�����X�g����
	case SEQ_SUB_KAIHUKU_ITEM_LIST:

#if 1
		//���X�g�����������Ƀ��[�v������
		BmpListDirectPosGet( wk->lw, &wk->list_csr_pos );					//�J�[�\���ʒu

		if( sys.trg & PAD_KEY_UP ){
			if( wk->list_csr_pos == 0 ){
				BmpListMainOutControlEx( wk->lw, (BMPLIST_HEADER*)&wk->list_h, 
					0, (KAIHUKU_MSG_TBL_MAX-1), 1, PAD_KEY_DOWN, NULL, NULL );
				BmpListRewrite( wk->lw );
				Snd_SePlay( SEQ_SE_DP_SELECT );

				CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALK], 
								msg_castle_poke_20, 1, 1, MSG_NO_PUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
				return FALSE;
			}
		}else if( sys.trg & PAD_KEY_DOWN ){
			if( wk->list_csr_pos == (KAIHUKU_MSG_TBL_MAX-1) ){
				BmpListMainOutControlEx( wk->lw, (BMPLIST_HEADER*)&wk->list_h, 
					0, 0, 1, PAD_KEY_UP, NULL, NULL );
				BmpListRewrite( wk->lw );
				Snd_SePlay( SEQ_SE_DP_SELECT );

				CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALK], 
								msg_castle_poke_13, 1, 1, MSG_NO_PUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
				return FALSE;
			}
		}
#endif

		ret = BmpListMain( wk->lw );
		Castle_ListSeCall( ret, SEQ_SE_DP_SELECT );
		BmpListDirectPosGet( wk->lw, &wk->list_csr_pos );					//�J�[�\���ʒu

		switch( ret ){

		case BMPLIST_NULL:
			break;

		case BMPLIST_CANCEL:
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Kaihuku_Del( wk );
			CastleMine_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
			break;

		case FC_PARAM_HP_KAIHUKU:
		case FC_PARAM_PP_KAIHUKU:
		case FC_PARAM_HPPP_KAIHUKU:
			wk->parent_decide_type = ret;									//�p�����[�^
			CastleMine_Kaihuku_Del( wk );

			//��b�E�B���h�E�\��
			CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));

			//�����N������Ȃ���
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );
			if( now_rank < kaihuku_msg_tbl[wk->list_csr_pos][0] ){
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_14, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
			}else{
				//�u���b�o�@������܂����@��낵���@�ł����H�v
				Castle_SetNumber(	wk, 0, kaihuku_cp_tbl[wk->list_csr_pos], 
									CASTLE_KETA_STATUS, NUMBER_DISPTYPE_LEFT );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_02, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_KAIHUKU_YES_NO;
			}
			break;

		case FC_PARAM_KAIHUKU_RANKUP:
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );
			
			//�����N�A�b�v�l�`�w��I��
			if( now_rank == CASTLE_RANK_MAX ){
				//�u����@�����傤�@�����N�́@������܂���I�v
				//wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_23, FONT_TALK );
				//wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
				Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
				Snd_SePlay( SEQ_SE_DP_BOX03 );
			}else{
				wk->parent_decide_type = ret;									//�p�����[�^
				CastleMine_Kaihuku_Del( wk );

				//�K�v��CP�|�C���g�擾
				now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));

				/*�u���b�o���g���ă����N�������܂����H�v*/
				Castle_SetNumber(wk, 0, rankup_point_tbl[CASTLE_RANK_TYPE_KAIHUKU][now_rank], 
								 CASTLE_KETA_CP, NUMBER_DISPTYPE_LEFT );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_24, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_KAIHUKU_RANKUP_YES_NO;
			}
			break;

		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�񕜁v���̃A�C�e���ł悢���u�͂��E�������v���j���[����
	case SEQ_SUB_KAIHUKU_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,wk->csr_pos) );
			CastleMine_SeqSubMenuWinClear( wk );					//���j���[��\�����Ă�����폜

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));

			//�����N������Ȃ���
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );
			if( now_rank < kaihuku_msg_tbl[wk->list_csr_pos][0] ){
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_14, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
				break;
			}
			
			//CP������Ȃ���
			if( now_cp < kaihuku_cp_tbl[wk->list_csr_pos] ){
				//���s�������Ă���̂ő��k
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_03, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
				break;
			}

			if( wk->list_csr_pos == 0 ){

				//HP�񕜂̕K�v���Ȃ���
				if( PokeParaGet(poke,ID_PARA_hp,NULL) == PokeParaGet(poke,ID_PARA_hpmax,NULL) ){
					wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_10, FONT_TALK );
					wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
					break;
				}
			}else if( wk->list_csr_pos == 1 ){

				//PP�񕜂̕K�v���Ȃ���
				if( CastleMine_PPRecoverCheck(poke) == FALSE ){
					wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_10, FONT_TALK );
					wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
					break;
				}

			}else{

				//HP,PP�񕜂̕K�v���Ȃ���
				if( (PokeParaGet(poke,ID_PARA_hp,NULL) == PokeParaGet(poke,ID_PARA_hpmax,NULL)) &&
					(CastleMine_PPRecoverCheck(poke) == FALSE) ){
					wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_10, FONT_TALK );
					wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
					break;
				}
			}

			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );

			if( Castle_CommCheck(wk->type) == FALSE ){

#if 1
				//CP���炷
				Castle_CPRecord_Sub(wk->fro_sv, wk->type,
							kaihuku_cp_tbl[wk->parent_decide_type-FC_PARAM_LIST_START_KAIHUKU] );

				CastleMine_SeqSubCPWrite( wk, &wk->bmpwin[MINE_BMPWIN_TR1] );			//CP�\��
#endif

				//�I�񂾃|�P�����̈ʒu�A�J�[�\���̈ʒu��n��
				CastleMine_SeqSubKaihuku( wk, wk->csr_pos, wk->parent_decide_type );	//��
				wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT;
			}else{
				wk->send_req = 1;
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleMine_SeqSubMenuWinClear( wk );					//���j���[��\�����Ă�����폜
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Kaihuku_Write( wk );
			wk->sub_seq = SEQ_SUB_KAIHUKU_ITEM_LIST;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�񕜃����N�A�b�v���܂����u�͂��E�������v�I��
	case SEQ_SUB_KAIHUKU_RANKUP_YES_NO:

		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleMine_SeqSubMenuWinClear( wk );					//���j���[��\�����Ă�����폜

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );

			//CP�̔�r
			if( now_cp < rankup_point_tbl[CASTLE_RANK_TYPE_KAIHUKU][now_rank] ){
				//�u�b�o���@����܂���v
				CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_40, FONT_TALK );
				wk->sub_seq = SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN;
				break;
			}
				
			//�����N�A�b�v
			if( Castle_CommCheck(wk->type) == FALSE ){
				CastleRank_SeqSubRankUp( wk, wk->csr_pos, FC_PARAM_KAIHUKU_RANKUP );
				wk->sub_seq = SEQ_SUB_KAIHUKU_RANKUP_DECIDE_EFF_WAIT;
				break;
			}else{
				wk->send_req		= 1;
				return TRUE;
			}

			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Kaihuku_Write( wk );
			wk->sub_seq = SEQ_SUB_KAIHUKU_ITEM_LIST;
			break;
		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�񕜃����N�A�b�v����G�t�F�N�g�҂�
	case SEQ_SUB_KAIHUKU_RANKUP_DECIDE_EFF_WAIT:
		if( CastleMine_DecideEff(wk,wk->csr_pos,wk->parent_decide_type) == TRUE ){
			wk->sub_seq = SEQ_SUB_KAIHUKU_RANKUP_DECIDE_EFF_KEY_WAIT;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�񕜃����N�A�b�v����G�t�F�N�g�҂��L�[�҂�
	case SEQ_SUB_KAIHUKU_RANKUP_DECIDE_EFF_KEY_WAIT:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
			//BmpTalkWinClear( &wk->bmpwin[MINE_BMPWIN_TALK], WINDOW_TRANS_OFF );
			CastleMine_Kaihuku_Write( wk );

			//�����N�A�b�v�G�t�F�N�g
			CastleObj_RankUpEff( wk->p_eff_rankup, CASTLE_MINE_RANKUP_X, CASTLE_MINE_RANKUP_Y );

			wk->sub_seq = SEQ_SUB_KAIHUKU_ITEM_LIST;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�񕜁v�L�[�҂����āA���X�g�֖߂�
	case SEQ_SUB_KAIHUKU_KEY_WAIT_LIST_RETURN:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Kaihuku_Write( wk );
			wk->sub_seq = SEQ_SUB_KAIHUKU_ITEM_LIST;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u�����^���v���X�g����
	case SEQ_SUB_RENTAL_LIST:
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );			//TEST

		ret = BmpListMain( wk->lw );
		Castle_ListSeCall( ret, SEQ_SE_DP_SELECT );
		BmpListDirectPosGet( wk->lw, &wk->list_csr_pos );					//�J�[�\���ʒu

		switch( ret ){

		case BMPLIST_NULL:
			break;

		case BMPLIST_CANCEL:
		//case (RENTAL_MSG_TBL_MAX - 1):			//��߂�
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Rental_Del( wk );		//TALKMENU2,RENTAL
			CastleMine_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
			break;

		case FC_PARAM_KINOMI:
			wk->parent_decide_type = ret;									//�p�����[�^
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Rental_Del( wk );		//TALKMENU2,RENTAL
			CastleMine_ItemSeed_Write( wk, FC_PARAM_KINOMI );
			wk->sub_seq = SEQ_SUB_RENTAL_ITEM_SEED_LIST;
			break;

		case FC_PARAM_ITEM:
			wk->parent_decide_type = ret;		//�p�����[�^
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Rental_Del( wk );		//TALKMENU2,RENTAL

			//�u�ǂ����v��I�����Ă��鎞(�����N������Ȃ���)
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );
			if( now_rank == 1 ){
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_29, FONT_TALK );
				wk->sub_seq = SEQ_SUB_RENTAL_KEY_WAIT_LIST_RETURN;
				return FALSE;	//���ӁI
			}

			CastleMine_ItemSeed_Write( wk, FC_PARAM_ITEM );
			wk->sub_seq = SEQ_SUB_RENTAL_ITEM_SEED_LIST;
			break;

		case FC_PARAM_RENTAL_RANKUP:
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );

			//�����N�A�b�v�l�`�w��I��
			if( now_rank == CASTLE_RANK_MAX ){
				//�u����@�����傤�@�����N�́@������܂���I�v
				//wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_23, FONT_TALK );
				//wk->sub_seq = SEQ_SUB_RENTAL_KEY_WAIT_LIST_RETURN;
				Snd_SeStopBySeqNo( SEQ_SE_DP_SELECT, 0 );
				Snd_SePlay( SEQ_SE_DP_BOX03 );
			}else{
				wk->parent_decide_type = ret;		//�p�����[�^
				CastleMine_Rental_Del( wk );		//TALKMENU2,RENTAL

				/*�u���b�o���g���ă����N�������܂����H�v*/
				Castle_SetNumber(wk, 0, rankup_point_tbl[CASTLE_RANK_TYPE_RENTAL][now_rank], 
								 CASTLE_KETA_CP, NUMBER_DISPTYPE_LEFT );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_24, FONT_TALK );
				Castle_SetMenu2( wk );						//�u�͂��E�������v
				wk->sub_seq = SEQ_SUB_RENTAL_RANKUP_YES_NO;
			}
			break;

		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u���̂݁A�ǂ����v���X�g����
	case SEQ_SUB_RENTAL_ITEM_SEED_LIST:
		ret = BmpListMain( wk->lw );
		Castle_ListSeCall( ret, SEQ_SE_DP_SELECT );
		BmpListDirectPosGet( wk->lw, &wk->list_csr_pos );					//�J�[�\���ʒu

		switch( ret ){

		case BMPLIST_NULL:
			break;

		case BMPLIST_CANCEL:
			CastleMine_ItemSeed_Del( wk );
			CastleMine_Rental_Write( wk );
			wk->sub_seq = SEQ_SUB_RENTAL_LIST;
			break;

		default:
			GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_ITEMINFO] );

			//��b�E�B���h�E�\��
			CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));

			//�u���b�o�@������܂����@��낵���@�ł����H�v
			Castle_SetNumber(	wk, 0, GetItemCP(wk,wk->list_csr_pos,wk->parent_decide_type), 
								CASTLE_KETA_STATUS, NUMBER_DISPTYPE_LEFT );
			wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_02, FONT_TALK );
			Castle_SetMenu2( wk );						//�u�͂��E�������v
			CastleObj_Vanish( wk->p_itemicon, CASTLE_VANISH_ON );					//��\��
			wk->sub_seq = SEQ_SUB_RENTAL_YES_NO;
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u����v���̃A�C�e���ł悢�u�͂��E�������v���j���[����
	case SEQ_SUB_RENTAL_YES_NO:
		poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,wk->csr_pos) );
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));

			//CP������Ȃ���
			if( now_cp < GetItemCP(wk,wk->list_csr_pos,wk->parent_decide_type) ){
				//���s�������Ă���̂ő��k
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_03, FONT_TALK );
				CastleObj_Vanish( wk->p_itemicon, CASTLE_VANISH_ON );	//��\��
				wk->sub_seq = SEQ_SUB_RENTAL_KEY_WAIT_ITEM_SEED_RETURN;
				break;
			}

			//�A�C�e���������Ă��Ȃ��|�P�����̎�
			if( PokeParaGet(poke,ID_PARA_item,NULL) == 0 ){

				if( Castle_CommCheck(wk->type) == FALSE ){
					CastleMine_ItemSeed_Del( wk );
					CastleTalkWinPut(	&wk->bmpwin[MINE_BMPWIN_TALK], 
										CONFIG_GetWindowType(wk->config));

					//�u���Ɂ����������܂����I�v
					Castle_CPRecord_Sub(wk->fro_sv, wk->type, 
										GetItemCP(wk,wk->list_csr_pos,wk->parent_decide_type));

					CastleMine_SeqSubCPWrite( wk, &wk->bmpwin[MINE_BMPWIN_TR1] );	//CP�\��
					CastleMine_SeqSubItem(	wk, wk->csr_pos, 
											GetItemNo(wk,wk->list_csr_pos,wk->parent_decide_type) );
					wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT;
				}else{
					wk->parent_decide_item = GetItemNo(wk,wk->list_csr_pos,wk->parent_decide_type);
					CastleMine_ItemSeed_Del( wk );
					BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
					wk->send_req = 1;
					return TRUE;
				}

			}else{

				//�A�C�e���������Ă���|�P�����̎�

				//�u���͂��łɁ��������Ă��܂��I�v
				//�u�����Ă��铹������ւ��܂����H�v
				Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
				WORDSET_RegisterItemName( wk->wordset, 1, PokeParaGet(poke,ID_PARA_item,NULL) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_08, FONT_TALK );
				wk->sub_seq = SEQ_SUB_RENTAL_ITEM_TRADE;					//���łɃA�C�e�������Ă�
			}
			
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleMine_SeqSubMenuWinClear( wk );					//���j���[��\�����Ă�����폜
			CastleMine_SeqSubItemListReturn( wk );
			wk->sub_seq = SEQ_SUB_RENTAL_ITEM_SEED_LIST;			//���X�g�����֖߂�
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�����^�������N�A�b�v���܂����u�͂��E�������v�I��
	case SEQ_SUB_RENTAL_RANKUP_YES_NO:

		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleMine_SeqSubMenuWinClear( wk );					//���j���[��\�����Ă�����폜

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
										//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
										Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)));
			now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );

			//CP�̔�r
			if( now_cp < rankup_point_tbl[CASTLE_RANK_TYPE_RENTAL][now_rank] ){
				//�u�b�o���@����܂���v
				CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
				wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_40, FONT_TALK );
				wk->sub_seq = SEQ_SUB_RENTAL_KEY_WAIT_LIST_RETURN;
				break;
			}
				
			//�����N�A�b�v
			if( Castle_CommCheck(wk->type) == FALSE ){
				CastleRank_SeqSubRankUp( wk, wk->csr_pos, FC_PARAM_RENTAL_RANKUP );
				wk->sub_seq = SEQ_SUB_RENTAL_RANKUP_DECIDE_EFF_WAIT;
				break;
			}else{
				wk->send_req		= 1;
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Rental_Write( wk );
			wk->sub_seq = SEQ_SUB_RENTAL_LIST;
			break;
		};
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�����^�������N�A�b�v����G�t�F�N�g�҂�
	case SEQ_SUB_RENTAL_RANKUP_DECIDE_EFF_WAIT:
		if( CastleMine_DecideEff(wk,wk->csr_pos,wk->parent_decide_type) == TRUE ){
			wk->sub_seq = SEQ_SUB_RENTAL_RANKUP_DECIDE_EFF_KEY_WAIT;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�����^�������N�A�b�v����G�t�F�N�g�҂��L�[�҂�
	case SEQ_SUB_RENTAL_RANKUP_DECIDE_EFF_KEY_WAIT:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
			//BmpTalkWinClear( &wk->bmpwin[MINE_BMPWIN_TALK], WINDOW_TRANS_OFF );
			CastleMine_Rental_Write( wk );

			//�����N�A�b�v�G�t�F�N�g
			CastleObj_RankUpEff( wk->p_eff_rankup, CASTLE_MINE_RANKUP_X2, CASTLE_MINE_RANKUP_Y2 );

			wk->sub_seq = SEQ_SUB_RENTAL_LIST;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u����v�L�[�҂����āA�����^�����X�g�֖߂�
	case SEQ_SUB_RENTAL_KEY_WAIT_LIST_RETURN:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Rental_Write( wk );
			wk->sub_seq = SEQ_SUB_RENTAL_LIST;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�u����v�L�[�҂����āA���̂݁A����X�g�֖߂�
	case SEQ_SUB_RENTAL_KEY_WAIT_ITEM_SEED_RETURN:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleMine_SeqSubItemListReturn( wk );
			wk->sub_seq = SEQ_SUB_RENTAL_ITEM_SEED_LIST;				//���X�g�����֖߂�
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���łɓ�������Ă��܂�
	case SEQ_SUB_RENTAL_ITEM_TRADE:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_09, FONT_TALK );
			Castle_SetMenu2( wk );										//�u�͂��E�������v
			//BmpTalkWinClear( &wk->bmpwin[MINE_BMPWIN_TALK], WINDOW_TRANS_OFF );
			wk->sub_seq = SEQ_SUB_RENTAL_ITEM_TRADE_YES_NO;				//��������H
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�����������H�u�͂��E�������v
	case SEQ_SUB_RENTAL_ITEM_TRADE_YES_NO:
		ret = BmpMenuMain( wk->mw );

		switch( ret ){

		case BMPMENU_NULL:
			break;

		//�u�͂��v
		case 0:
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜

			if( Castle_CommCheck(wk->type) == FALSE ){
				CastleMine_ItemSeed_Del( wk );
				CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));

				//�u���Ɂ����������܂����I�v
				Castle_CPRecord_Sub(wk->fro_sv, wk->type,
									GetItemCP(wk,wk->list_csr_pos,wk->parent_decide_type) );
				CastleMine_SeqSubCPWrite( wk, &wk->bmpwin[MINE_BMPWIN_TR1] );	//CP�\��
				CastleMine_SeqSubItem(	wk, wk->csr_pos, 
										GetItemNo(wk,wk->list_csr_pos,wk->parent_decide_type) );
				wk->sub_seq = SEQ_SUB_DECIDE_EFF_WAIT;
			}else{
				wk->parent_decide_item = GetItemNo( wk, wk->list_csr_pos, wk->parent_decide_type );
				CastleMine_ItemSeed_Del( wk );
				BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
				wk->send_req = 1;
				return TRUE;
			}
			break;
			
		//�u�������v
		case 1:
		case BMPMENU_CANCEL:
			CastleMine_SeqSubMenuWinClear( wk );				//���j���[��\�����Ă�����폜
			CastleMine_SeqSubItemListReturn( wk );
			wk->sub_seq = SEQ_SUB_RENTAL_ITEM_SEED_LIST;						//���X�g�����֖߂�
			break;
		};

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//����G�t�F�N�g�҂�
	case SEQ_SUB_DECIDE_EFF_WAIT:
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );				//TEST
		if( CastleMine_DecideEff(wk,wk->csr_pos,wk->parent_decide_type) == TRUE ){
			wk->sub_seq = SEQ_SUB_DECIDE_KEY_WAIT;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//����G�t�F�N�g��̃L�[�҂�
	case SEQ_SUB_DECIDE_KEY_WAIT:
		if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
			CastleMine_Default_Write( wk );		//TALKMENU,���ǂ�
			wk->sub_seq = SEQ_SUB_START;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�悳�F�L�[�҂����ăX�e�[�^�X�Ȃǂ̃E�B���h�E��������BASIC���X�g��
	case SEQ_SUB_INFO_TUYOSA_KEY_WAIT_WIN_OFF:
		if( sys.trg & PAD_KEY_LEFT ){
			CastleMine_StatusWinChg( wk, -1 );
		}else if( sys.trg & PAD_KEY_RIGHT ){
			CastleMine_StatusWinChg( wk, 1 );
		}else if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleMine_SeqSubStatusWazaDel( wk );							//�X�e�[�^�X�E�Z���폜
			CastleMine_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�킴�F�L�[�҂����ăX�e�[�^�X�Ȃǂ̃E�B���h�E��������BASIC���X�g��
	case SEQ_SUB_INFO_WAZA_KEY_WAIT_WIN_OFF:
		if( sys.trg & PAD_KEY_LEFT ){
			CastleMine_WazaWinChg( wk, -1 );
		}else if( sys.trg & PAD_KEY_RIGHT ){
			CastleMine_WazaWinChg( wk, 1 );
		}else if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
			Snd_SePlay( SEQ_SE_DP_SELECT );
			CastleMine_SeqSubStatusWazaDel( wk );							//�X�e�[�^�X�E�Z���폜
			CastleMine_Basic_Write( wk );		//TALKMENU2,BASIC
			wk->sub_seq = SEQ_SUB_MENU_MAIN;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	����M
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameSendRecv( CASTLE_MINE_WORK* wk )
{
	u8 sel_pos;
	POKEMON_PARAM* poke;

	switch( wk->sub_seq ){

	case 0:
		wk->rankup_recover_flag = 0;	//�����N�A�b�v�������ɖ߂�悪����t���OOFF

		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//TEST

		//�I�񂾃��N�G�X�g(��肽������)�𑗐M
		if( CastleMine_CommSetSendBuf(wk,CASTLE_COMM_MINE_UP_TYPE,wk->csr_pos) == TRUE ){
			wk->send_req = 0;
			wk->sub_seq++;
		}
		break;

	case 1:
		if( wk->parent_decide_pos == CASTLE_MINE_DECIDE_NONE ){
			break;
		}

		//�e�����߂������N��LR�������N�A�b�v������
		wk->recieve_count = 0;

		//�����N���グ�鏈���Ƃ���ȊO�ŕ����Ă���
		if( (wk->parent_decide_type == FC_PARAM_KAIHUKU_RANKUP) ||
			(wk->parent_decide_type == FC_PARAM_RENTAL_RANKUP) ){
			CastleRank_SeqSubRankUp( wk, wk->parent_decide_pos, wk->parent_decide_type );
		}else{
			CastleMine_SeqSubRankUp( wk, wk->parent_decide_pos, wk->parent_decide_type );
		}

		wk->sub_seq++;
		break;

	//����G�t�F�N�g�҂�
	case 2:
		sel_pos = GetCommSelCsrPos( wk->modoru_pos, wk->parent_decide_pos );	//�J�[�\���ʒu�擾
		if( CastleMine_DecideEff(wk,sel_pos,wk->parent_decide_type) == TRUE ){
			wk->wait = CASTLE_COMM_WAIT;
			wk->sub_seq++;
		}
		break;

	//�L�[�҂�
	case 3:
		//if( sys.trg & (PAD_BUTTON_DECIDE | PAD_BUTTON_CANCEL) ){
		//Snd_SePlay( SEQ_SE_DP_SELECT );
		wk->wait--;
		if( wk->wait == 0 ){
			//�����J�n
			CommToolTempDataReset();
			CommTimingSyncStart( DBC_TIM_CASTLE_MINE_UP );
			wk->sub_seq++;
			break;
		}
		break;

	//����
	case 4:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_CASTLE_MINE_UP) == TRUE ){
			CommToolTempDataReset();
			CommToolInitialize( HEAPID_CASTLE );	//timingSyncEnd=0xff
			wk->parent_decide_pos = CASTLE_MINE_DECIDE_NONE;

			//�����N�A�b�v�������ɖ߂�悪���ꂩ�`�F�b�N
			if( wk->rankup_recover_flag == 0 ){
				BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
				CastleMine_Default_Write( wk );								//TALKMENU,���ǂ�
			}

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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEndMulti( CASTLE_MINE_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	//�u�߂�v���M
	case 0:
		if( CastleMine_CommSetSendBuf(wk,CASTLE_COMM_MINE_MODORU,0) == TRUE ){
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
			CommTimingSyncStart( DBC_TIM_CASTLE_MINE_END );
			wk->sub_seq++;
		}
		break;

	//�����҂�
	case 2:
		//�^�C�~���O�R�}���h���͂������m�F
		if( CommIsTimingSync(DBC_TIM_CASTLE_MINE_END) == TRUE ){
			CommToolTempDataReset();
			BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( CASTLE_MINE_WORK* wk )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleCommon_Delete( CASTLE_MINE_WORK* wk )
{
	u8 m_max;
	int i,j;

	CastleObj_Delete( wk->p_scr_u );
	CastleObj_Delete( wk->p_scr_d );
	CastleObj_Delete( wk->p_itemicon );
	//CastleObj_Delete( wk->p_eff );		//����Ȃ�
	CastleObj_Delete( wk->p_eff_rankup );
	CastleObj_Delete( wk->p_poke_sel );
	CastleObj_Delete( wk->p_pair_poke_sel );
	CastleObj_Delete( wk->p_item_csr );

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_TOTAL );
	for( i=0; i < m_max ;i++ ){
		CastleObj_Delete( wk->p_icon[i] );
		CastleObj_Delete( wk->p_itemkeep[i] );
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

	CastleClact_DeleteCellObject(&wk->castle_clact);	//2D�I�u�W�F�N�g�֘A�̈�J��

	MSGMAN_Delete( wk->msgman );						//���b�Z�[�W�}�l�[�W���J��
	MSGMAN_Delete( wk->msgman_iteminfo );				//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );						//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );						//���b�Z�[�W�o�b�t�@�J��
	NUMFONT_Delete( wk->num_font );

	for( i=0; i < CASTLE_MENU_BUF_MAX ;i++ ){
		STRBUF_Delete( wk->menu_buf[i] );				//���j���[�o�b�t�@�J��
	}

	CastleExitBmpWin( wk->bmpwin, CASTLE_BMP_MINE );	//BMP�E�B���h�E�J��
	Castle_BgExit( wk->bgl );							//BGL�폜
	ArchiveDataHandleClose( wk->hdl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������1
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_InitSub2( CASTLE_MINE_WORK* wk )
{
	u32 start_x,start_y;
	u32 hp,hp_max,anm_no,hp_anm_no;
	u8 m_max;
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,icon_offset_x,itemkeep_offset_x;
	int i,j,flip;
	GF_BGL_BMPWIN* win;
	POKEMON_PARAM* poke;

	wk->hdl = ArchiveDataHandleOpen( ARC_FRONTIER_BG, HEAPID_CASTLE );
	Castle_BgInit( wk );								//BG������
	Castle_ObjInit( wk );								//OBJ������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_castle_poke_dat, HEAPID_CASTLE );

	//�A�C�e�����
	//wk->msgman_iteminfo = MSGMAN_Create(MSGMAN_TYPE_NORMAL, ARC_MSG, 
	wk->msgman_iteminfo = MSGMAN_Create(MSGMAN_TYPE_DIRECT, ARC_MSG, 
										NARC_msg_iteminfo_dat, HEAPID_CASTLE );
										
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

	//8x8�t�H���g�쐬(�g�pbmpwin[MINE_BMPWIN_TYPE]�̃p���b�g(BC_FONT_PAL)���g�p
	//wk->num_font = NUMFONT_Create( 15, 14, FBMP_COL_NULL, HEAPID_CASTLE );
	wk->num_font = NUMFONT_Create( FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, HEAPID_CASTLE );

	//�r�b�g�}�b�v�ǉ�
	CastleAddBmpWin( wk->bgl, wk->bmpwin, CASTLE_BMP_MINE );

	//�ʐM���Ă��邩�ŕ\���̃I�t�Z�b�g���ς��
	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	////////////////////////////////////////////////////////////////////////////////////////
	//���X�g�X�N���[���J�[�\��OBJ�ǉ�
	wk->p_scr_u = CastleObj_Create(	&wk->castle_clact, 
									ID_CHAR_CSR, ID_PLTT_CSR, 
									ID_CELL_CSR, CASTLE_ANM_SCR_CURSOR_U, 
									CASTLE_SCR_U_X, CASTLE_SCR_U_Y, CASTLE_BG_PRI_HIGH, NULL );
	wk->p_scr_d = CastleObj_Create(	&wk->castle_clact,
									ID_CHAR_CSR, ID_PLTT_CSR, 
									ID_CELL_CSR, CASTLE_ANM_SCR_CURSOR_D, 
									CASTLE_SCR_D_X, CASTLE_SCR_D_Y, CASTLE_BG_PRI_HIGH, NULL );
	CastleObj_Vanish( wk->p_scr_u, CASTLE_VANISH_ON );					//��\��
	CastleObj_Vanish( wk->p_scr_d, CASTLE_VANISH_ON );					//��\��

	////////////////////////////////////////////////////////////////////////////////////////
	if( Castle_CommCheck(wk->type) == FALSE ){
		icon_offset_x		= ICON_START_X;
		itemkeep_offset_x	= ICON_START_X + CASTLE_ITEMKEEP_OFFSET_X;
	}else{
		icon_offset_x		= ICON_MULTI_START_X;
		itemkeep_offset_x	= ICON_MULTI_START_X + CASTLE_ITEMKEEP_OFFSET_X;
	}

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_TOTAL );
	for( i=0; i < m_max ;i++ ){

		//�A�C�e�������Ă���A�C�R��OBJ�ǉ�
		wk->p_itemkeep[i] = CastleObj_Create(	&wk->castle_clact,
												ID_CHAR_ITEMKEEP, ID_PLTT_ITEMKEEP, 
												ID_CELL_ITEMKEEP, 0,
												ICON_WIDTH_X*i+itemkeep_offset_x, 
												ICON_START_Y + CASTLE_ITEMKEEP_OFFSET_Y, 
												CASTLE_BG_PRI_LOW, NULL );

		//�A�C�e���������Ă��Ȃ��|�P�����̎��̓A�C�e�������Ă���A�C�R�����\��
		poke =  PokeParty_GetMemberPointer( wk->p_party, i );
		if( PokeParaGet(poke,ID_PARA_item,NULL) == 0 ){
			CastleObj_Vanish( wk->p_itemkeep[i], CASTLE_VANISH_ON );			//��\��
		}

		//�|�P�����A�C�R���̃A�j���i���o�[���擾
		hp = PokeParaGet( poke, ID_PARA_hp, NULL );
		hp_max = PokeParaGet( poke, ID_PARA_hpmax, NULL );
		anm_no = CastleMine_GetPokeAnmNo( wk, GetHPGaugeDottoColor(hp,hp_max,48) );		//POKE�A�j��
		hp_anm_no = CastleMine_GetHpAnmNo( wk, GetHPGaugeDottoColor(hp,hp_max,48) );	//HP�A�j��

		//HP���OBJ�ǉ�
		wk->p_hp[i] = CastleObj_Create(	&wk->castle_clact,
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, hp_anm_no,
										ICON_WIDTH_X*i+icon_offset_x, 
										ICON_START_Y+20, CASTLE_BG_PRI_BACK, NULL );

		//�|�P�����A�C�R��OBJ�ǉ�
		wk->p_icon[i] = CastleObj_Create(	&wk->castle_clact,
											ID_CHAR_ICON1+i, ID_PLTT_ICON, 
											ID_CELL_ICON, anm_no,
											ICON_WIDTH_X*i+icon_offset_x, 
											ICON_START_Y, CASTLE_BG_PRI_LOW, NULL );

		//�p���b�g�؂�ւ�
		CastleObj_IconPalChg( wk->p_icon[i], PokeParty_GetMemberPointer(wk->p_party,i) );
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//�A�C�e���A�C�R��OBJ
	wk->p_itemicon = CastleObj_Create(	&wk->castle_clact, 
										ID_CHAR_ITEMICON, ID_PLTT_ITEMICON,
										ID_CELL_ITEMICON, 0, 
										CASTLE_ITEMICON_X, CASTLE_ITEMICON_Y, 
										CASTLE_BG_PRI_HIGH, NULL );

	//�A�C�e���A�C�R��(CHAR�APLTT�ύX)
	CastleClact_ItemIconCharChange( &wk->castle_clact, ITEM_KIZUGUSURI );
	CastleClact_ItemIconPlttChange( &wk->castle_clact, ITEM_KIZUGUSURI );

	CastleObj_Vanish( wk->p_itemicon, CASTLE_VANISH_ON );					//��\��

	////////////////////////////////////////////////////////////////////////////////////////
	//�|�P�����I���E�B��OBJ�ǉ�
	CastleMine_GetPokeSelXY( wk, &start_x, &start_y, 0 );
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
	//�����N�A�b�vOBJ�ǉ�
	wk->p_eff_rankup = CastleObj_Create(&wk->castle_clact, 
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, CASTLE_ANM_RANK_UP, 
										20, 20,			//�l�K��
										CASTLE_BG_PRI_HIGH, NULL );
	CastleObj_Vanish( wk->p_eff_rankup, CASTLE_VANISH_ON );					//��\��

	////////////////////////////////////////////////////////////////////////////////////////
	//�A�C�e���J�[�\��OBJ�ǉ�
	wk->p_item_csr = CastleObj_Create(	&wk->castle_clact, 
										ID_CHAR_CSR, ID_PLTT_CSR, 
										ID_CELL_CSR, CASTLE_ANM_CURSOR_ITEM, 
										20, 20,			//�l�K��
										CASTLE_BG_PRI_NORMAL, NULL );
	CastleObj_Vanish( wk->p_item_csr, CASTLE_VANISH_ON );					//��\��

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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_BgInit( CASTLE_MINE_WORK* wk )
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
	//Castle_SetItemBgGraphic( wk, BC_FRAME_SLIDE );
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_ObjInit( CASTLE_MINE_WORK* wk )
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
	CASTLE_MINE_WORK* wk = work;

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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetMainBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TEMOCHI_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	if( Castle_CommCheck(wk->type) == FALSE ){
		ArcUtil_HDL_ScrnSet(wk->hdl, BC_TEMOCHI01_NSCR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	}else{
		ArcUtil_HDL_ScrnSet(wk->hdl, BC_TEMOCHI02_NSCR_BIN, 
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

	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BC_TEMOCHI_NCLR, &dat, HEAPID_CASTLE );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*4) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*4) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʃX�e�[�^�X
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetStatusBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TEMOCHI_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BC_TEMOCHI_STATUS_NSCR_BIN, 
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

	buf = ArcUtil_PalDataGet( ARC_FRONTIER_BG, BC_TEMOCHI_NCLR, &dat, HEAPID_CASTLE );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*4) );
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*4) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʂ킴
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetWazaBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TEMOCHI_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BC_TEMOCHI_WAZA_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʓ���
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetItemBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, BC_TEMOCHI_NCGR_BIN, 
							wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );

	ArcUtil_HDL_ScrnSet(wk->hdl, BC_TEMOCHI_ITEM_NSCR_BIN, 
						wk->bgl, frm, 0, 0, 1, HEAPID_CASTLE );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i�A�p���b�g�Z�b�g
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetSubBgGraphic( CASTLE_MINE_WORK * wk, u32 frm  )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static u8 CastleWriteMsg( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	u8 msg_index;

	GF_BGL_BmpWinDataFill( win, b_col );			//"********�h��Ԃ�********"
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static u8 CastleWriteMsgSimple( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Castle_EasyMsg( CASTLE_MINE_WORK* wk, int msg_id, u8 font )
{
	u8 msg_index;

	msg_index = CastleWriteMsg(	wk, &wk->bmpwin[MINE_BMPWIN_TALK], msg_id, 
								1, 1, MSG_NO_PUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, font );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_TALK] );
	return msg_index;
}
//--------------------------------------------------------------
/**
 * @brief	�X�e�[�^�X���b�Z�[�W�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
enum{
	//��������
	STATUS_MOTIMONO_X =		(1),
	STATUS_MOTIMONO_Y =		(1*8),
	STATUS_MOTIMONO_NUM_X =	(8*8),

	//���������@��
	STATUS_SEIKAKU_X =		(1),
	STATUS_SEIKAKU_Y =		(3*8),
	STATUS_SEIKAKU_NUM_X =	(8*8),

	//�Ƃ�����
	STATUS_TOKUSEI_X =		(1),
	STATUS_TOKUSEI_Y =		(5*8),
	STATUS_TOKUSEI_NUM_X =	(8*8),

	//��������
	STATUS_KOUGEKI_X =		(1),
	STATUS_KOUGEKI_Y =		(7*8),
	STATUS_KOUGEKI_NUM_X =	(7*8),

	//�ڂ�����
	STATUS_BOUGYO_X =		(11*8),
	STATUS_BOUGYO_Y =		(STATUS_KOUGEKI_Y),
	STATUS_BOUGYO_NUM_X =	(18*8),

	//�Ƃ�����
	STATUS_TOKUKOU_X =		(1),
	STATUS_TOKUKOU_Y =		(9*8),
	STATUS_TOKUKOU_NUM_X =	(STATUS_KOUGEKI_NUM_X),

	//�Ƃ��ڂ�
	STATUS_TOKUBOU_X =		(11*8),
	STATUS_TOKUBOU_Y =		(STATUS_TOKUKOU_Y),
	STATUS_TOKUBOU_NUM_X =	(STATUS_BOUGYO_NUM_X),

	//���΂₳
	STATUS_SUBAYASA_X =		(1),
	STATUS_SUBAYASA_Y =		(11*8),
	STATUS_SUBAYASA_NUM_X =	(STATUS_KOUGEKI_NUM_X),
};

static void Castle_StatusMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke )
{
	GF_BGL_BmpWinDataFill( win,FBMP_COL_NULL );			//"********�h��Ԃ�********"

	//�u�������́v
	WORDSET_RegisterItemName( wk->wordset, 0, PokeParaGet(poke,ID_PARA_item,NULL) );
	StMsgWriteSub( wk, win, msg_castle_poke_status_01, STATUS_MOTIMONO_X, STATUS_MOTIMONO_Y );
	StMsgWriteSub( wk, win,msg_castle_poke_status_01_01,STATUS_MOTIMONO_NUM_X,STATUS_MOTIMONO_Y);

	//�u���������v
	WORDSET_RegisterSeikaku( wk->wordset, 0, PokeSeikakuGet(poke) );
	StMsgWriteSub( wk, win, msg_castle_poke_status_02, STATUS_SEIKAKU_X, STATUS_SEIKAKU_Y );
	StMsgWriteSub( wk, win, msg_castle_poke_status_02_01, STATUS_SEIKAKU_NUM_X, STATUS_SEIKAKU_Y );

	//�u�Ƃ������v
	WORDSET_RegisterTokuseiName( wk->wordset, 0, PokeParaGet(poke,ID_PARA_speabino,NULL) );
	StMsgWriteSub( wk, win, msg_castle_poke_status_03, STATUS_TOKUSEI_X, STATUS_TOKUSEI_Y );
	StMsgWriteSub( wk, win, msg_castle_poke_status_03_01, STATUS_TOKUSEI_NUM_X,STATUS_TOKUSEI_Y);

	//�u���������v
	//OS_Printf( "pow = %d\n", PokeParaGet(poke,ID_PARA_pow,NULL) );
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_pow,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_poke_status_04, STATUS_KOUGEKI_X, STATUS_KOUGEKI_Y );
	StMsgWriteSub( wk, win, msg_castle_poke_status_04_01, STATUS_KOUGEKI_NUM_X,STATUS_KOUGEKI_Y);

	//�u�ڂ�����v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_def,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_poke_status_05, STATUS_BOUGYO_X, STATUS_BOUGYO_Y );
	StMsgWriteSub( wk, win, msg_castle_poke_status_05_01, STATUS_BOUGYO_NUM_X, STATUS_BOUGYO_Y );

	//�u�Ƃ������v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_spepow,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_poke_status_06, STATUS_TOKUKOU_X, STATUS_TOKUKOU_Y );
	StMsgWriteSub( wk, win, msg_castle_poke_status_06_01, STATUS_TOKUKOU_NUM_X,STATUS_TOKUKOU_Y);

	//�u�Ƃ��ڂ��v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_spedef,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_poke_status_07, STATUS_TOKUBOU_X, STATUS_TOKUBOU_Y );
	StMsgWriteSub( wk, win, msg_castle_poke_status_07_01, STATUS_TOKUBOU_NUM_X,STATUS_TOKUBOU_Y);

	//�u���΂₳�v
	Castle_SetNumber(	wk, 0, PokeParaGet(poke,ID_PARA_agi,NULL), CASTLE_KETA_STATUS,
						NUMBER_DISPTYPE_SPACE );
	StMsgWriteSub( wk, win, msg_castle_poke_status_08, STATUS_SUBAYASA_X, STATUS_SUBAYASA_Y );
	StMsgWriteSub( wk, win,msg_castle_poke_status_08_01,STATUS_SUBAYASA_NUM_X,STATUS_SUBAYASA_Y);

	GF_BGL_BmpWinOnVReq( win );
	return;
}

static void StMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, int msg, u16 x, u16 y )
{
	CastleWriteMsgSimple(	wk, win, msg, x, y, MSG_NO_PUT,
							FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Z���b�Z�[�W�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
enum{
	//�Z��
	STATUS_WAZA_X =			(3*8),
	STATUS_WAZA_Y =			(1*8+4),

	//�Z�|�C���g
	STATUS_POINT_X =		(12*8),
	STATUS_POINT_Y =		(1*8+4),
};

static void Castle_WazaMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, POKEMON_PARAM* poke )
{
	GF_BGL_BmpWinDataFill( win,FBMP_COL_NULL );			//"********�h��Ԃ�********"

	//id+i�͕ۏ؂���邩�킩��Ȃ��̂ŁA���Ȃ��B

	WazaMsgWriteSub(	wk, win, 0, msg_castle_poke_waza_01_01, 
						msg_castle_poke_waza_02, poke, ID_PARA_waza1, 
						ID_PARA_pp1, ID_PARA_pp_max1 );

	WazaMsgWriteSub(	wk, win, 1, msg_castle_poke_waza_01_02, 
						msg_castle_poke_waza_02, poke, ID_PARA_waza2, 
						ID_PARA_pp2, ID_PARA_pp_max2 );

	WazaMsgWriteSub(	wk, win, 2, msg_castle_poke_waza_01_03, 
						msg_castle_poke_waza_02, poke, ID_PARA_waza3, 
						ID_PARA_pp3, ID_PARA_pp_max3 );

	WazaMsgWriteSub(	wk, win, 3, msg_castle_poke_waza_01_04, 
						msg_castle_poke_waza_02, poke, ID_PARA_waza4, 
						ID_PARA_pp4, ID_PARA_pp_max4 );

	GF_BGL_BmpWinOnVReq( win );
	return;
}

static void WazaMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u32 msg_id, u32 msg_id2, POKEMON_PARAM* poke, u32 id, u32 id2, u32 id3 )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeHpMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	int i;
	u8 m_max;

	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );					//�h��Ԃ�

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=0;i < m_max ;i++ ){
		Castle_PokeHpMsgWriteSub( wk, win, i, 0 );
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * HP��MHP�̃X���b�V���\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	flag	0=�ʏ�A1=�A�C�e���E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeHpMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u8 flag )
{
	u16 hp_offset_x,hp_x,hp_y,hpmax_x,hpmax_y,slash_x,slash_y;
	POKEMON_PARAM* poke;

	poke =  PokeParty_GetMemberPointer( wk->p_party, no );
	OS_Printf( "*********poke = %d\n", poke );

	if( Castle_CommCheck(wk->type) == FALSE ){
		hp_offset_x = CASTLE_HP_START_X;
	}else{
		hp_offset_x = CASTLE_MULTI_HP_START_X;
	}

	//0=�ʏ�A1=�A�C�e���E�B���h�E
	if( flag == 0 ){
		hp_x = hp_offset_x + (CASTLE_HP_WIDTH_X * no);
		hp_y = CASTLE_HP_START_Y;
		slash_x = CASTLE_HPSLASH_START_X + hp_offset_x + (CASTLE_HP_WIDTH_X * no);
		slash_y = CASTLE_HP_START_Y;
		hpmax_x = CASTLE_HPMAX_START_X + hp_offset_x + (CASTLE_HP_WIDTH_X * no);
		hpmax_y = CASTLE_HP_START_Y;
	}else{
		hp_x = 4;
		hp_y = 0;
		slash_x = CASTLE_HPSLASH_START_X + 4;
		slash_y = 0;
		hpmax_x = CASTLE_HPMAX_START_X + 4;
		hpmax_y = 0;
	}

	//HP�͉E�Â�
	NUMFONT_WriteNumber(wk->num_font, PokeParaGet(poke,ID_PARA_hp,NULL), CASTLE_KETA_HP, 
						NUMFONT_MODE_SPACE, win, hp_x, hp_y );

	//HP��MHP�̃X���b�V��(�L���`��)
	NUMFONT_WriteMark(	wk->num_font, NUMFONT_MARK_SLASH, win, slash_x, slash_y );

	//HPMAX�͍��Â�
	NUMFONT_WriteNumber(wk->num_font, PokeParaGet(poke,ID_PARA_hpmax,NULL), CASTLE_KETA_HP, 
						NUMFONT_MODE_LEFT, win, hpmax_x, hpmax_y );

	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���x���\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeLvMsgWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	int i;
	u8 m_max;

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_TOTAL );

	for( i=0;i < m_max ;i++ ){
		Castle_PokeLvMsgWriteSub( wk, win, i, 0 );
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���x���\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	flag	0=�ʏ�A1=�A�C�e���E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeLvMsgWriteSub( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u8 flag )
{
	u32 sex,lv_x,lv_y,sex_x,sex_y;
	u16 lv_offset_x,sex_offset_x;
	POKEMON_PARAM* poke;

	if( Castle_CommCheck(wk->type) == FALSE ){
		lv_offset_x = CASTLE_LV_START_X;
		sex_offset_x = CASTLE_SEX_START_X;
	}else{
		lv_offset_x = CASTLE_MULTI_LV_START_X;
		sex_offset_x = CASTLE_MULTI_SEX_START_X;
	}

	poke =  PokeParty_GetMemberPointer( wk->p_party, no );

	//0=�ʏ�A1=�A�C�e���E�B���h�E
	if( flag == 0 ){
		lv_x = lv_offset_x + (CASTLE_LV_WIDTH_X * no);
		lv_y = CASTLE_LV_START_Y;
		sex_x = sex_offset_x + (CASTLE_SEX_WIDTH_X * no);
		sex_y = CASTLE_SEX_START_Y;
	}else{
		lv_x = 4;
		lv_y = 0;
		sex_x = (8*6);
		sex_y = 0;
	}

	//LV(�L���Ɛ������Z�b�g�ŕ`��)
	NUMFONT_WriteSet(	wk->num_font, NUMFONT_MARK_LV, PokeParaGet(poke,ID_PARA_level,NULL),
						CASTLE_KETA_LV, NUMFONT_MODE_LEFT, win, lv_x, lv_y );

	//���ʕ\��
	sex = PokeParaGet( poke, ID_PARA_sex, NULL );
	PokeSexWrite( wk, win, sex_x, sex_y, BC_FONT, sex );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ���O�{CP�E�B���h�E�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_NameCPWinWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 now_cp;

	now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

	//���j���[�E�B���h�E��`��
	CastleWriteMenuWin( wk->bgl, win );

	GF_BGL_BmpWinDataFill( win, FBMP_COL_WHITE );			//�h��Ԃ�
	PlayerNameWrite( wk, win, 1, 1, BC_FONT );				//�v���C���[����\��

	Castle_SetNumber( wk, 0, now_cp, CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
	wk->msg_index = CastleWriteMsgSimple( wk, win, msg_castle_poke_cp_03,
									CASTLE_NAME_CP_START_X, 1*16 , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * CP�E�B���h�E�X�V
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_NameCPWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 now_cp;

	now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
								Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

	GF_BGL_BmpWinFill( win, FBMP_COL_NULL, 0, 7*8, 16, 16 );
	//GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );				//�h��Ԃ�

	//PlayerNameWrite( wk, win, 1, 1, BC_FONT );				//�v���C���[����\��

	Castle_SetNumber( wk, 0, now_cp, CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
	wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_poke_cp_03, 1, 1*16 , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����񃁃b�Z�[�W�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	msg_id	���b�Z�[�WID
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 CastleItemInfoWriteMsg( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u16 item )
{
	u8 msg_index;

	GF_BGL_BmpWinDataFill( win, FBMP_COL_NULL );			//�h��Ԃ�

	MSGMAN_GetString( wk->msgman_iteminfo, item, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	msg_index = GF_STR_PrintColor(win,BC_FONT, wk->msg_buf, ITEM_INFO_PX, ITEM_INFO_PY, MSG_NO_PUT, 
								GF_PRINTCOLOR_MAKE(FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL), 
								NULL );

	GF_BGL_BmpWinOnVReq( win );
	return msg_index;
}

//--------------------------------------------------------------------------------------------
/**
 * HP�\�����X�V
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void Castle_PokeHpEff( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 no, u32 hp )
{
	u16 hp_offset_x;

	if( Castle_CommCheck(wk->type) == FALSE ){
		hp_offset_x = CASTLE_HP_START_X;
	}else{
		hp_offset_x = CASTLE_MULTI_HP_START_X;
	}

	GF_BGL_BmpWinFill(	win, FBMP_COL_NULL, hp_offset_x + (CASTLE_HP_WIDTH_X * no), 0, 
						//CASTLE_HP_WIDTH_X, 8 );
						//8*3, 8 );
						8*3, 9 );

	//HP�͉E�Â�
	NUMFONT_WriteNumber(wk->num_font, hp, CASTLE_KETA_HP, 
						NUMFONT_MODE_SPACE, win, 
						hp_offset_x + (CASTLE_HP_WIDTH_X * no), CASTLE_HP_START_Y );

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ������Ԃ�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Default_Write( CASTLE_MINE_WORK* wk )
{
	//�u���ǂ�v
	wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_MODORU], 
									msg_castle_poke_00_02, 1, 1+4 , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_SYSTEM );

	//���Ƀ��j���[�E�B���h�E�Ő�����\��
	//CastleWriteMenuWin( wk->bgl, &wk->bmpwin[MINE_BMPWIN_TALKMENU] );
	CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALKMENU], CONFIG_GetWindowType(wk->config) );

	wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALKMENU], 
									msg_castle_poke_00_01, 1, 1, MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ������Ԃ��폜
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Default_Del( CASTLE_MINE_WORK* wk )
{
	//BmpMenuWinClear( &wk->bmpwin[MINE_BMPWIN_TALKMENU], WINDOW_TRANS_OFF );
	//GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_TALKMENU] );
	BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALKMENU] );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ��{��Ԃ�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Basic_Write( CASTLE_MINE_WORK* wk )
{
	//���Ƀ��j���[�E�B���h�E2�Ő�����\��
	//CastleWriteMenuWin( wk->bgl, &wk->bmpwin[MINE_BMPWIN_TALKMENU2] );
	CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALKMENU2], CONFIG_GetWindowType(wk->config) );

	/*�u�g�o��o�o�������ӂ��ł��܂��v*/
	//wk->msg_index = CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALKMENU2], 
	//								msg_castle_poke_choice_01, 1, 1, MSG_NO_PUT, 
	//								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BC_FONT );

	wk->list_csr_pos = 0;
	Castle_BasicListMake( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * ��{��Ԃ��폜
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Basic_Del( CASTLE_MINE_WORK* wk )
{
	BmpListPosGet( wk->lw, &wk->basic_list_lp, &wk->basic_list_cp );
	//BmpMenuWinClear( &wk->bmpwin[MINE_BMPWIN_TALKMENU2], WINDOW_TRANS_OFF );
	//GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_TALKMENU2] );
	BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALKMENU2] );
	CastleMine_SeqSubListEnd( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �񕜍��ڏ�Ԃ�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Kaihuku_Write( CASTLE_MINE_WORK* wk )
{
	//��b�E�B���h�E�\��
	CastleTalkWinPut(	&wk->bmpwin[MINE_BMPWIN_TALK], 
						CONFIG_GetWindowType(wk->config));
	//BmpTalkWinPutSub( wk );

	//�u�ǂ���g���܂����H�v
	wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_13, FONT_TALK );

	wk->list_csr_pos = 0;
	Castle_KaihukuListMake( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �񕜍��ڏ�Ԃ��폜
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Kaihuku_Del( CASTLE_MINE_WORK* wk )
{
	CastleMine_SeqSubListEnd( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �����^�����ڏ�Ԃ�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Rental_Write( CASTLE_MINE_WORK* wk )
{
	//��b�E�B���h�E�\��
	CastleTalkWinPut(	&wk->bmpwin[MINE_BMPWIN_TALK], 
						CONFIG_GetWindowType(wk->config));

	//wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_41, FONT_TALK );
	
	wk->list_csr_pos = 0;
	Castle_RentalListMake( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �����^�����ڏ�Ԃ��폜
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_Rental_Del( CASTLE_MINE_WORK* wk )
{
	CastleMine_SeqSubListEnd( wk );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e�����̂ݍ��ڏ�Ԃ�\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_ItemSeed_Write( CASTLE_MINE_WORK* wk, u8 type )
{
	wk->item_list_flag = 1;

	BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );

	//�����͂����ɏ���
	GF_BGL_BmpWinOff( &wk->bmpwin[MINE_BMPWIN_TALK] );
	GF_BGL_BmpWinOff( &wk->bmpwin[MINE_BMPWIN_BASIC_LIST] );

	Castle_SetItemBgGraphic( wk, BC_FRAME_SLIDE );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_STATUS] );
	
	wk->list_csr_pos = 0;
	Castle_ItemListMake( wk, type );
	Castle_NameCPWinWrite( wk, &wk->bmpwin[MINE_BMPWIN_CP] );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * �A�C�e�����̂ݍ��ڏ�Ԃ��폜
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void CastleMine_ItemSeed_Del( CASTLE_MINE_WORK* wk )
{
	if( wk->item_list_flag == 1 ){
		wk->item_list_flag = 0;
		GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_ITEMINFO] );
		CastleMine_SeqSubListEnd( wk );
		CastleMine_SeqSubItemListEnd( wk );				//���X�g�I��
	}
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	win		GF_BGL_BMPWIN�^�̃|�C���^
 * @param	y_max	���ڍő吔
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void CastleInitMenu( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u8 y_max )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	no		�Z�b�g����ꏊ(�߂�l)
 * @param	param	�߂�l
 * @param	msg_id	���b�Z�[�WID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void CastleSetMenuData( CASTLE_MINE_WORK* wk, u8 no, u8 param, int msg_id )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_SetMenu2( CASTLE_MINE_WORK* wk )
{
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[MINE_BMPWIN_YESNO] );
	CastleInitMenu( wk, &wk->bmpwin[MINE_BMPWIN_YESNO], 2 );
	CastleSetMenuData( wk, 0, 0, msg_castle_poke_02_01 );				//�͂�
	CastleSetMenuData( wk, 1, 1, msg_castle_poke_02_02 );				//������
	wk->mw = BmpMenuAddEx( &wk->MenuH, 8, 0, 0, HEAPID_CASTLE, PAD_BUTTON_CANCEL );
	wk->menu_flag = 1;
	return;
}


//==============================================================================================
//
//	���X�g�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�A�C�e�����X�g�쐬
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_ItemListMake( CASTLE_MINE_WORK* wk, u8 decide_type )
{
	u8 now_rank,list_max,poke_pos;
	u16 item;
	int i;
	MSGDATA_MANAGER* msgman_itemname;			//���b�Z�[�W�}�l�[�W���[

	//�|�P�����A�C�R���̈ʒu��ύX
	CastleMine_ItemWinPokeIconMove( wk, wk->csr_pos, 1 );

	//HP�\���ALV�\��
	poke_pos = GetCsrPokePos( wk->h_max, wk->csr_pos );
	Castle_PokeHpMsgWriteSub( wk, &wk->bmpwin[MINE_BMPWIN_ITEM_HP], poke_pos, 1 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_ITEM_HP] );
	Castle_PokeLvMsgWriteSub( wk, &wk->bmpwin[MINE_BMPWIN_ITEM_LV], poke_pos, 1 );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_ITEM_LV] );

	CastleObj_Vanish( wk->p_itemicon, CASTLE_VANISH_OFF );				//�\��
	CastleObj_Vanish( wk->p_scr_u, CASTLE_VANISH_OFF );					//�\��
	CastleObj_Vanish( wk->p_scr_d, CASTLE_VANISH_OFF );					//�\��
	CastleObj_Vanish( wk->p_item_csr, CASTLE_VANISH_OFF );				//�\��

	msgman_itemname = MSGMAN_Create(MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_itemname_dat, HEAPID_CASTLE );

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );

	if( decide_type == FC_PARAM_KINOMI ){
		list_max = castle_seed_rank[now_rank-1];
	}else{
		list_max = castle_item_rank[now_rank-1];
	}
	wk->menulist = BMP_MENULIST_Create( (list_max+1), HEAPID_CASTLE );

	for( i=0; i < list_max; i++ ){

		if( decide_type == FC_PARAM_KINOMI ){
			item = castle_seed[i];
		}else{
			item = castle_item[i];
		}
		BMP_MENULIST_AddArchiveString(	wk->menulist, msgman_itemname, item, i );
	}

	//�u��߂�v�����b�Z�[�W���Ȃ�
	BMP_MENULIST_AddArchiveString(	wk->menulist, wk->msgman, 
									msg_castle_poke_choice_05, BMPLIST_CANCEL );


	wk->list_h			= CastleListH;
	wk->list_h.list		= wk->menulist;
	wk->list_h.win		= &wk->bmpwin[MINE_BMPWIN_LIST];
	wk->list_h.work		= wk;
	wk->list_h.call_back= Castle_CsrMoveCallBack;				//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.icon		= Castle_LineWriteCallBack;				//���\�����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.count	= (list_max + 1);						//���X�g���ڐ�
	wk->list_h.b_col	= FBMP_COL_NULL;						//�w�i�F
	wk->list_h.line		= ( 6 );								//�\���ő區�ڐ�
	wk->list_h.data_x	= ( 0 );								//���ڕ\���w���W
	wk->list_h.c_disp_f = 1;									//�J�[�\��(allow)(0:ON,1:OFF)
	//wk->list_page_skip	= BMPLIST_LRKEY_SKIP;				//�y�[�W�X�L�b�v�^�C�v(�A�C�e���̂�)

	//�����̒���GF_BGL_BmpWinOn���Ă΂�Ă���
	wk->lw			= BmpListSet( &wk->list_h, 0, 0, HEAPID_CASTLE );

	CastleMine_SeqSubListStart( wk, &wk->bmpwin[MINE_BMPWIN_LIST] );
	//GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_LIST] );

	MSGMAN_Delete( msgman_itemname );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_CsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode )
{
	u32 count,line;
	u16 item,list_bak,cursor_bak,pos_bak;
	CASTLE_MINE_WORK* wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	//���������͖炳�Ȃ�
	if( mode == 0 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
		BmpListDirectPosGet( wk->lw, &pos_bak );
	}else{
		pos_bak = 0;	//���������͒l����������(�Ƃ������ǂ����ŏ����l�Z�b�g���Ă���H)
	}

	count = BmpListParamGet( work, BMPLIST_ID_COUNT );							//���X�g���ڐ�
	line  = BmpListParamGet( work, BMPLIST_ID_LINE );							//�\���ő區�ڐ�
	BmpListPosGet( work, &list_bak, &cursor_bak );
	if( list_bak == 0 ){
		CastleObj_Vanish( wk->p_scr_u, CASTLE_VANISH_ON );						//��\��
		CastleObj_Vanish( wk->p_scr_d, CASTLE_VANISH_OFF );						//�\��
	}else if( list_bak == (count-line) ){
		CastleObj_Vanish( wk->p_scr_u, CASTLE_VANISH_OFF );						//�\��
		CastleObj_Vanish( wk->p_scr_d, CASTLE_VANISH_ON );						//��\��
	}else{
		CastleObj_Vanish( wk->p_scr_u, CASTLE_VANISH_OFF );						//�\��
		CastleObj_Vanish( wk->p_scr_d, CASTLE_VANISH_OFF );						//�\��
	}

	//�J�[�\���ʒu�𒲐�
	//CastleObj_SetObjPos( wk->p_item_csr, 160, (24 + cursor_bak * 16) );
	CastleObj_SetObjPos( wk->p_item_csr, 158, (24 + cursor_bak * 16) );
	//OS_Printf( "list_bak = %d\n", list_bak );
	//OS_Printf( "cursor_bak = %d\n", cursor_bak );

	//OS_Printf( "pos_bak = %d\n", pos_bak );
	if( param != BMPLIST_CANCEL ){

		//������X�V
		CastleItemInfoWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_ITEMINFO], 
								GetItemNo(wk,pos_bak,wk->parent_decide_type) );
		//GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_ITEMINFO] );

		//�A�C�e���A�C�R��(CHAR�APLTT�ύX)
		CastleClact_ItemIconCharChange( &wk->castle_clact, 
										GetItemNo(wk,pos_bak,wk->parent_decide_type) );
		CastleClact_ItemIconPlttChange( &wk->castle_clact, 
										GetItemNo(wk,pos_bak,wk->parent_decide_type) );
	}else{
		//���ǂ�
		CastleClact_ItemIconCharChange( &wk->castle_clact, ITEM_RETURN_ID );
		CastleClact_ItemIconPlttChange( &wk->castle_clact, ITEM_RETURN_ID );
		GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_ITEMINFO], FBMP_COL_NULL );		//�h��Ԃ�
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_ITEMINFO] );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_LineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y )
{
	CASTLE_MINE_WORK* wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	if( param != BMPLIST_CANCEL ){

		///< �l�i�Ȃǂ̕\������
		//price = 100;
		//expb  = STRBUF_Create( 16, HEAPID_CASTLE );
		//strb  = MSGMAN_AllocString( wk->msgman, msg_castle_poke_cp_03 );
		//WORDSET_ExpandStr( wk->wset, expb, strb );
		//siz = FontProc_GetPrintStrWidth( FONT_SYSTEM, expb, 0 );
		//GF_STR_PrintColor(
		//	&wk->win[WIN_IDX_LIST], FONT_SYSTEM, expb,
		//	LIST_PRICE_EX-siz, y, MSG_NO_PUT, SHOPCOL_N_BLACK, NULL );

		//�����́Afor����PARAM���Z�b�g���Ă���̂ł���ł悢
		Castle_SetNumber(	wk, 0, GetItemCP(wk,param,wk->parent_decide_type), 
							CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
		wk->msg_index = CastleWriteMsgSimple(	wk, &wk->bmpwin[MINE_BMPWIN_LIST], 
												msg_castle_poke_cp_03, 
												CASTLE_LIST_ITEM_CP_X, y , MSG_NO_PUT, 
												FBMP_COL_BLACK, FBMP_COL_BLK_SDW, FBMP_COL_NULL, 
												BC_FONT );
		GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_LIST] );

		//STRBUF_Delete( strb );
		//STRBUF_Delete( expb );
	}

	return;
}

//==============================================================================================
//
//	�u�����ӂ��v��I�񂾎��ɕ\�����郊�X�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�񕜂�I�񂾎��ɕ\�����郊�X�g�쐬
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_KaihukuListMake( CASTLE_MINE_WORK* wk )
{
	u8 now_rank,list_max;
	u16 item;
	int i;

	//���j���[�E�B���h�E��`��
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[MINE_BMPWIN_KAIHUKU_LIST] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_KAIHUKU_LIST], FBMP_COL_WHITE );	//�h��Ԃ�

	wk->menulist = BMP_MENULIST_Create( KAIHUKU_MSG_TBL_MAX, HEAPID_CASTLE );

	for( i=0; i < KAIHUKU_MSG_TBL_MAX; i++ ){
		BMP_MENULIST_AddArchiveString(	wk->menulist, wk->msgman, 
										kaihuku_msg_tbl[i][1], kaihuku_msg_tbl[i][2] );
	}

	wk->list_h			= CastleListH;
	wk->list_h.list		= wk->menulist;
	wk->list_h.win		= &wk->bmpwin[MINE_BMPWIN_KAIHUKU_LIST];
	wk->list_h.work		= wk;
	wk->list_h.call_back= Castle_KaihukuCsrMoveCallBack;		//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.icon		= Castle_KaihukuLineWriteCallBack;		//���\�����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.count	= KAIHUKU_MSG_TBL_MAX;					//���X�g���ڐ�
	wk->list_h.line		= KAIHUKU_MSG_TBL_MAX;					//�\���ő區�ڐ�
	wk->list_h.b_col	= FBMP_COL_WHITE;						//�w�i�F

	//�����̒���GF_BGL_BmpWinOn���Ă΂�Ă���
	wk->lw			= BmpListSet( &wk->list_h, 0, 0, HEAPID_CASTLE );
	//GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_KAIHUKU_LIST] );

	CastleMine_SeqSubListStart( wk, &wk->bmpwin[MINE_BMPWIN_KAIHUKU_LIST] );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_KaihukuCsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode )
{
	u8 now_rank;
	u16 msg_id;
	CASTLE_MINE_WORK* wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	//���������͖炳�Ȃ�
	if( mode == 0 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );

	switch( param ){
	case FC_PARAM_KAIHUKU_RANKUP:
		//���̃E�B���h�E�ɕ\�����郁�b�Z�[�W�������N�ɂ���ĕς��
		if( now_rank == CASTLE_RANK_MAX ){
			msg_id = msg_castle_poke_23;
		}else if( now_rank == 1 ){
			msg_id = msg_castle_poke_21;
		}else{
			msg_id = msg_castle_poke_22;
		}
		break;
	case BMPLIST_CANCEL:
		msg_id = msg_castle_poke_20;
		break;
	default:
		msg_id = msg_castle_poke_13;
		break;
	};

	CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALK], 
					msg_id, 1, 1, MSG_NO_PUT, 
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_KaihukuLineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y )
{
	u16 pos_bak;
	u8 now_rank,f_col;
	CASTLE_MINE_WORK* wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	BmpListDirectPosGet( work, &pos_bak );				//�J�[�\�����W�擾
	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );

	switch( param ){
	case FC_PARAM_HP_KAIHUKU:			//�g�o�����ӂ�
	case FC_PARAM_PP_KAIHUKU:			//�o�o�����ӂ�
	case FC_PARAM_HPPP_KAIHUKU:			//���ׂĂ����ӂ�
		//�����N�����ĕ\���F��ς���
		if( now_rank >= kaihuku_msg_tbl[param-FC_PARAM_LIST_START_KAIHUKU][0] ){
			f_col = FBMP_COL_BLACK;
		}else{
			f_col = FBMP_COL_BLK_SDW;
		}
		break;

	case FC_PARAM_KAIHUKU_RANKUP:		//�����N�A�b�v
		//�����N�����ĕ\���F��ς���
		if( now_rank == CASTLE_RANK_MAX ){
			f_col = FBMP_COL_BLK_SDW;
		}else{
			f_col = FBMP_COL_BLACK;
		}
		break;

	default:
		f_col = FBMP_COL_BLACK;
		break;
	};

	BmpListColorControl( work, f_col, FBMP_COL_WHITE, FBMP_COL_BLK_SDW );
	return;
}


//==============================================================================================
//
//	�u�����^���v��I�񂾎��ɕ\�����郊�X�g
//
//==============================================================================================
static const u32 rental_msg_tbl[][3] = {
	//�����N�Amsg_id�A���X�g�p�����[�^
	{ 1, msg_castle_rental_01,	FC_PARAM_KINOMI },			//���̂�
	{ 2, msg_castle_rental_02,	FC_PARAM_ITEM },			//�ǂ���
	{ 1, msg_castle_rental_03,	FC_PARAM_RENTAL_RANKUP },	//�����N�A�b�v
	{ 1, msg_castle_rental_04,	BMPLIST_CANCEL },			//��߂�
};
#define RENTAL_MSG_TBL_MAX		( NELEMS(rental_msg_tbl) )

//--------------------------------------------------------------
/**
 * @brief	�����^����I�񂾎��ɕ\�����郊�X�g�쐬
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_RentalListMake( CASTLE_MINE_WORK* wk )
{
	u8 now_rank,list_max;
	u16 item;
	int i;

	//�w��l�ŃN���A���ꂽ�L�������Z�b�g
	//GF_BGL_CharFill(	wk->bgl, BC_FRAME_TYPE, 0, 
	//					(MINE_WIN_RENTAL_LIST_SX * MINE_WIN_RENTAL_LIST_SY),
	//					MINE_WIN_RENTAL_LIST_CGX );

	GF_BGL_VisibleSet( BC_FRAME_TYPE, VISIBLE_OFF );

	//���j���[�E�B���h�E��`��
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[MINE_BMPWIN_RENTAL_LIST] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_RENTAL_LIST], FBMP_COL_WHITE );	//�h��Ԃ�

	wk->menulist = BMP_MENULIST_Create( RENTAL_MSG_TBL_MAX, HEAPID_CASTLE );

	for( i=0; i < RENTAL_MSG_TBL_MAX; i++ ){
		BMP_MENULIST_AddArchiveString(	wk->menulist, wk->msgman, 
										rental_msg_tbl[i][1], rental_msg_tbl[i][2] );
	}

	wk->list_h			= CastleListH;
	wk->list_h.list		= wk->menulist;
	wk->list_h.win		= &wk->bmpwin[MINE_BMPWIN_RENTAL_LIST];
	wk->list_h.work		= wk;
	wk->list_h.call_back= Castle_RentalCsrMoveCallBack;			//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.icon		= Castle_RentalLineWriteCallBack;		//���\�����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.count	= RENTAL_MSG_TBL_MAX;					//���X�g���ڐ�
	wk->list_h.line		= RENTAL_MSG_TBL_MAX;					//�\���ő區�ڐ�
	wk->list_h.b_col	= FBMP_COL_WHITE;						//�w�i�F

	//�����̒���GF_BGL_BmpWinOn���Ă΂�Ă���
	wk->lw			= BmpListSet( &wk->list_h, 0, 0, HEAPID_CASTLE );

	CastleMine_SeqSubListStart( wk, &wk->bmpwin[MINE_BMPWIN_RENTAL_LIST] );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_RENTAL_LIST] );

	GF_BGL_VisibleSet( BC_FRAME_TYPE, VISIBLE_ON );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_RentalCsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode )
{
	u16 msg_id;
	u8 now_rank;
	CASTLE_MINE_WORK* wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	//���������͖炳�Ȃ�
	if( mode == 0 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );

	switch( param ){
	case FC_PARAM_KINOMI:
		msg_id = msg_castle_poke_41;
		break;
	case FC_PARAM_ITEM:
		msg_id = msg_castle_poke_42;
		break;
	case FC_PARAM_RENTAL_RANKUP:
		//���̃E�B���h�E�ɕ\�����郁�b�Z�[�W�������N�ɂ���ĕς��
		if( now_rank == CASTLE_RANK_MAX ){
			msg_id = msg_castle_poke_32;
		}else if( now_rank == 1 ){
			msg_id = msg_castle_poke_30;
		}else{
			msg_id = msg_castle_poke_31;
		}
		break;
	default:
		msg_id = msg_castle_poke_33;
		break;
	};

	CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALK], 
					msg_id, 1, 1, MSG_NO_PUT, 
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���\�����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_RentalLineWriteCallBack( BMPLIST_WORK* work, u32 param, u8 y )
{
	u8 now_rank,f_col;
	CASTLE_MINE_WORK* wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );

	switch( param ){

	case FC_PARAM_ITEM:
		//�����N�����ĕ\���F��ς���
		//if( now_rank >= rental_msg_tbl[param][0] ){
		if( now_rank >= rental_msg_tbl[param-FC_PARAM_LIST_START_RENTAL][0] ){
			f_col = FBMP_COL_BLACK;
		}else{
			f_col = FBMP_COL_BLK_SDW;
		}
		break;

	case FC_PARAM_RENTAL_RANKUP:
		//�����N�����ĕ\���F��ς���
		if( now_rank == CASTLE_RANK_MAX ){
			f_col = FBMP_COL_BLK_SDW;
		}else{
			f_col = FBMP_COL_BLACK;
		}
		break;
	
	//case FC_PARAM_KINOMI:
	default:
		f_col = FBMP_COL_BLACK;
		break;
	};

	BmpListColorControl( work, f_col, FBMP_COL_WHITE, FBMP_COL_BLK_SDW );
	return;
}


//==============================================================================================
//
//	��{���X�g�֘A(�����ӂ��A�����^���A�悳�A�킴�A�Ƃ���)
//
//==============================================================================================
#define CASTLE_MINE_BASIC_LIST_MAX	(5)

static const u32 basic_msg_tbl[][2] = {
	{ msg_castle_poke_choice_01,	FC_PARAM_KAIHUKU},
	{ msg_castle_poke_choice_02,	FC_PARAM_RENTAL	},
	{ msg_castle_poke_choice_03,	FC_PARAM_TUYOSA	},
	{ msg_castle_poke_choice_04,	FC_PARAM_WAZA	},
	{ msg_castle_poke_choice_05,	BMPLIST_CANCEL	},
};

static const u16 basic_under_msg_tbl[] = {
	msg_castle_poke_00_03,
	msg_castle_poke_00_04,
	msg_castle_poke_00_05,
	msg_castle_poke_00_06,
	msg_castle_poke_00_07,
};

//--------------------------------------------------------------
/**
 * @brief	��{���j���[��\�����郊�X�g�쐬
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_BasicListMake( CASTLE_MINE_WORK* wk )
{
	int i;

	//���j���[�E�B���h�E��`��
	CastleWriteMenuWin( wk->bgl, &wk->bmpwin[MINE_BMPWIN_BASIC_LIST] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_BASIC_LIST], FBMP_COL_WHITE );	//�h��Ԃ�

	wk->menulist = BMP_MENULIST_Create( CASTLE_MINE_BASIC_LIST_MAX, HEAPID_CASTLE );

	for( i=0; i < CASTLE_MINE_BASIC_LIST_MAX; i++ ){
		BMP_MENULIST_AddArchiveString(	wk->menulist, wk->msgman, 
										basic_msg_tbl[i][0], basic_msg_tbl[i][1] );
	}

	wk->list_h			= CastleListH;
	wk->list_h.list		= wk->menulist;
	wk->list_h.win		= &wk->bmpwin[MINE_BMPWIN_BASIC_LIST];
	wk->list_h.work		= wk;
	wk->list_h.call_back= Castle_BasicCsrMoveCallBack;			//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.icon		= NULL;									//���\�����Ƃ̃R�[���o�b�N�֐�
	wk->list_h.count	= CASTLE_MINE_BASIC_LIST_MAX;			//���X�g���ڐ�
	wk->list_h.line		= CASTLE_MINE_BASIC_LIST_MAX;			//�\���ő區�ڐ�
	wk->list_h.b_col	= FBMP_COL_WHITE;						//�w�i�F
	wk->list_h.work		= wk;

	//�����̒���GF_BGL_BmpWinOn���Ă΂�Ă���
	wk->lw			= BmpListSet( &wk->list_h, wk->basic_list_lp, wk->basic_list_cp, HEAPID_CASTLE);

	CastleMine_SeqSubListStart( wk, &wk->bmpwin[MINE_BMPWIN_BASIC_LIST] );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[MINE_BMPWIN_BASIC_LIST] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Castle_BasicCsrMoveCallBack( BMPLIST_WORK* work, u32 param, u8 mode )
{
	CASTLE_MINE_WORK* wk;
	u16 pos_bak;

	wk = (CASTLE_MINE_WORK*)BmpListParamGet( work, BMPLIST_ID_WORK );

	//���������͖炳�Ȃ�
	if( mode == 0 ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

	//BmpListDirectPosGet( work, &wk->cursor_bak );		//�J�[�\�����W�擾
	BmpListDirectPosGet( work, &pos_bak );				//�J�[�\�����W�擾

	CastleWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_TALKMENU2], 
					basic_under_msg_tbl[pos_bak], 1, 1, MSG_NO_PUT, 
					FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * �����Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void Castle_SetNumber( CASTLE_MINE_WORK* wk, u32 bufID, s32 number, u32 keta, NUMBER_DISPTYPE disp )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, keta, disp, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P���������Z�b�g
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	ppp		POKEMON_PASO_PARAM
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Castle_SetPokeName( CASTLE_MINE_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp )
{
	WORDSET_RegisterPokeMonsName( wk->wordset, bufID, ppp );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�����Z�b�g
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Castle_SetPlayerName( CASTLE_MINE_WORK* wk, u32 bufID )
{
	WORDSET_RegisterPlayerName( wk->wordset, bufID, SaveData_GetMyStatus(wk->sv) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[����\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static void PlayerNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
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

	GF_STR_PrintColor( win, font, player_buf, x, y, MSG_NO_PUT, col, NULL );
	GF_BGL_BmpWinOnVReq( win );

	STRBUF_Delete( player_buf );					//���b�Z�[�W�o�b�t�@�J��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�p�[�g�i�[����\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static void PairNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
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

	CastleWriteMsgSimple(	wk, win, msg_castle_poke_name_02, 
							x, y, MSG_NO_PUT, 
							GF_PRINTCOLOR_GET_LETTER(col),
							GF_PRINTCOLOR_GET_SHADOW(col),
							GF_PRINTCOLOR_GET_GROUND(col),
							//FBMP_COL_NULL,
							BC_FONT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P��������\��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static void PokeNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font )
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

/*
	//�E�[�ɐ��ʃR�[�h��\��
	x_pos = GF_BGL_BmpWinGet_SizeX(win) - 1;
	sex = PokeParaGet( poke, ID_PARA_sex, NULL );

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
 * @brief	�|�P��������\��(���ʎw��)
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static void PokeNameWriteEx( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
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
static void PokeSexWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font, u8 sex )
{
	u32 msg_id,col;
	u8 f_col,s_col,b_col;

	if( sex == PARA_MALE ){
		msg_id = msg_castle_poke_status_10_01;
		f_col = FBMP_COL_BLUE;
		s_col = FBMP_COL_BLU_SDW;
		b_col = FBMP_COL_NULL;
	}else if( sex == PARA_FEMALE ){
		msg_id = msg_castle_poke_status_10_02;
		f_col = FBMP_COL_RED;
		s_col = FBMP_COL_RED_SDW;
		b_col = FBMP_COL_NULL;
	}else{
		return;	//���ʂȂ�
	}

	CastleWriteMsgSimple(	wk, win, msg_id, 
							x, y, MSG_NO_PUT, 
							f_col, s_col, b_col, font );

	//GF_BGL_BmpWinOnVReq( win );
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	type	�`�F�b�N����^�C�v
 *
 * @return	"TRUE = type�������AFALSE = type���Ⴄ"
 */
//--------------------------------------------------------------
static BOOL Castle_CheckType( CASTLE_MINE_WORK* wk, u8 type )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( CASTLE_MINE_WORK* wk, int* seq, int next )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck( CASTLE_MINE_WORK* wk )
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
static void CsrMove( CASTLE_MINE_WORK* wk, int key )
{
	u8 poke_pos;
	int flag;

	flag = 0;
	poke_pos = GetCsrPokePos( wk->h_max, wk->csr_pos );

	///////////////////////////////////////////////////////////////////////////////////////////
	if( sys.trg & PAD_KEY_LEFT ){

		//�u���ǂ�v�I��
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

		//�u���ǂ�v�I��
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

static void CsrMoveSub( CASTLE_MINE_WORK* wk )
{
	Snd_SePlay( SEQ_SE_DP_SELECT );

	//�ʐM�^�C�v�̎��́A�J�[�\���ʒu�𑗐M
	if( Castle_CommCheck(wk->type) == TRUE ){
		CastleMine_CommSetSendBuf( wk, CASTLE_COMM_MINE_CSR_POS, wk->csr_pos );
	}

	//�|�P�����I���E�B���h�E
	//PokeSelMoveSub( wk, wk->csr_pos, CommGetCurrentID() );
	PokeSelMoveSub( wk, wk->csr_pos, 0 );
	return;
}

//�|�P�����I���E�B���h�E
static void PokeSelMoveSub( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 flag )
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
		CastleObj_SetObjPos( obj, MINE_CSR_MODORU_X, MINE_CSR_MODORU_Y );
	}else{
		CastleObj_AnmChg( obj, anm_no );
		CastleMine_GetPokeSelXY( wk, &start_x, &start_y, csr_pos );
		CastleObj_SetObjPos( obj, start_x, start_y );
	}

	return;
}

//�|�P�����I���E�B���h�E�I�t�Z�b�g�擾
static void CastleMine_GetPokeSelXY( CASTLE_MINE_WORK* wk, u32* x, u32* y, u8 csr_pos )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
static u16 GetCsrX( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 lr )
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
		ret = (MINE_CSR_MODORU_X + lr_offset_x);
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"Y"
 */
//--------------------------------------------------------------
static u16 GetCsrY( CASTLE_MINE_WORK* wk, u8 csr_pos )
{
	u16 ret;

	if( csr_pos >= wk->modoru_pos ){
		return MINE_CSR_MODORU_Y;
	}

	ret = CSR_START_Y;
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e���b�o�擾
 *
 * @param	csr_pos		�J�[�\���ʒu
 *
 * @return	"�b�o"
 */
//--------------------------------------------------------------
static u16 GetItemCP( CASTLE_MINE_WORK* wk, u16 csr_pos, u8 type )
{
	u8 now_rank,list_max,item_max,seed_max;

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );
	item_max = castle_item_rank[now_rank-1];
	seed_max = castle_seed_rank[now_rank-1];

	if( type == FC_PARAM_KINOMI ){
		return castle_seed_cp[csr_pos];
	}

	return castle_item_cp[csr_pos];
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e���b�o�擾(�A�C�e���i���o�[����擾)
 *
 * @param	item		�A�C�e���i���o�[
 *
 * @return	"�b�o"
 */
//--------------------------------------------------------------
static u16 GetItemCPByItemNo( u16 item )
{
	int i;

	for( i=0; i < CASTLE_ITEM_MAX ;i++ ){
		if( castle_item[i] == item ){
			return castle_item_cp[i];
		}
	}

	for( i=0; i < CASTLE_SEED_MAX ;i++ ){
		if( castle_seed[i] == item ){
			return castle_seed_cp[i];
		}
	}

	GF_ASSERT( 0 );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e���i���o�[�擾
 *
 * @param	csr_pos		�J�[�\���ʒu
 *
 * @return	"�A�C�e���i���o�["
 */
//--------------------------------------------------------------
static u16 GetItemNo( CASTLE_MINE_WORK* wk, u16 csr_pos, u8 type )
{
	u8 now_rank,list_max,item_max,seed_max;

	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_RENTAL );
	item_max = castle_item_rank[now_rank-1];
	seed_max = castle_seed_rank[now_rank-1];

	if( type == FC_PARAM_KINOMI ){
		return castle_seed[csr_pos];
	}

	return castle_item[csr_pos];
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
static void Castle_GetOffset( CASTLE_MINE_WORK* wk, u16* offset_x, u16* offset_y, u16* pair_offset_x, u16* pair_offset_y )
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"TRUE=�I���AFALSE=�p��"
 */
//--------------------------------------------------------------
static BOOL CastleMine_DecideEff( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 sel_type )
{
	u32 hp,hp_max,anm_no,hp_anm_no;
	u16 eff_offset_x;
	u8 poke_pos;
	POKEMON_PARAM* poke;

	poke_pos	= GetCsrPokePos( wk->h_max, csr_pos );
	poke		= PokeParty_GetMemberPointer( wk->p_party, poke_pos );
	hp			= PokeParaGet( poke, ID_PARA_hp, NULL );
	hp_max		= PokeParaGet( poke, ID_PARA_hpmax, NULL );
	anm_no		= CastleMine_GetPokeAnmNo( wk, GetHPGaugeDottoColor(hp,hp_max,48) );	//POKE�A�j��
	hp_anm_no	= CastleMine_GetHpAnmNo( wk, GetHPGaugeDottoColor(hp,hp_max,48) );		//HP�A�j��

	switch( sel_type ){

	//�u�����N�A�b�v�v
	case FC_PARAM_KAIHUKU_RANKUP:		//(��)�����N�A�b�v
	case FC_PARAM_RENTAL_RANKUP:		//(�����^��)�����N�A�b�v
		wk->eff_init_flag = 0;
		return TRUE;
		break;

	//�u�����ӂ��v
	//case SEL_KAIHUKU:
	case FC_PARAM_HP_KAIHUKU:			//�g�o�����ӂ�
	case FC_PARAM_PP_KAIHUKU:			//�o�o�����ӂ�
	case FC_PARAM_HPPP_KAIHUKU:			//���ׂĂ����ӂ�

		//�����ݒ�
		if( wk->eff_init_flag == 0 ){
			wk->eff_init_flag = 1;

			if( Castle_CommCheck(wk->type) == FALSE ){
				eff_offset_x		= KAIHUKU_EFF_START_X;
			}else{
				eff_offset_x		= KAIHUKU_EFF_MULTI_START_X;
			}

			//�G�t�F�N�gOBJ�ǉ�
			wk->p_eff = CastleObj_Create(	&wk->castle_clact, 
											ID_CHAR_CSR, ID_PLTT_CSR, 
											ID_CELL_CSR, CASTLE_ANM_KAIHUKU_EFF, 
											eff_offset_x + (KAIHUKU_EFF_WIDTH_X * poke_pos),
											KAIHUKU_EFF_START_Y,
											CASTLE_BG_PRI_HIGH, NULL );
		}

		//�A�j���[�V�����I�����I�����Ă�����폜
		//(HP�̐��l�������I���̂�������������̂ŕ����Ă���)
		if( wk->p_eff != NULL ){
			if( CastleObj_AnmActiveCheck(wk->p_eff) == FALSE ){
				CastleObj_Delete( wk->p_eff );
				wk->p_eff = NULL;
			}
		}

#if 0
		//HP�̐��l�����������Ă���
		if( wk->before_hp != PokeParaGet(poke,ID_PARA_hp,NULL) ){
			//OS_Printf( "before_hp = %d\n", wk->before_hp );
			//OS_Printf( "para_hp = %d\n", PokeParaGet(poke,ID_PARA_hp,NULL) );
			wk->before_hp++;
			Castle_PokeHpEff( wk, &wk->bmpwin[MINE_BMPWIN_HP], poke_pos, wk->before_hp );
		}else{
			//HP�񕜂��I�����Ă��āA�񕜉��o�A�j�����I�����Ă�����
			if( wk->p_eff == NULL ){
				CastleObj_AnmChg( wk->p_hp[poke_pos], hp_anm_no );			//HP�A�j���؂�ւ�
				CastleObj_PokeIconAnmChg( wk->p_icon[poke_pos], anm_no );	//POKE�A�j���؂�ւ�
				wk->eff_init_flag = 0;
				return TRUE;
			}
		}
#else
		//�񕜉��o�A�j�����I�����Ă�����
		if( wk->p_eff == NULL ){
			Castle_PokeHpEff(	wk, &wk->bmpwin[MINE_BMPWIN_HP], poke_pos, 
								PokeParaGet(poke,ID_PARA_hp,NULL) );

			CastleObj_AnmChg( wk->p_hp[poke_pos], hp_anm_no );			//HP�A�j���؂�ւ�
			CastleObj_PokeIconAnmChg( wk->p_icon[poke_pos], anm_no );	//POKE�A�j���؂�ւ�
			wk->eff_init_flag = 0;
			return TRUE;
		}
#endif
		break;

	//�u�ǂ����v
	//case SEL_RENTAL:
	case FC_PARAM_KINOMI:				//���̂�
	case FC_PARAM_ITEM:					//�ǂ���

		//�����ݒ�
		if( wk->eff_init_flag == 0 ){
			wk->eff_init_flag = 1;

			if( Castle_CommCheck(wk->type) == FALSE ){
				eff_offset_x		= KAIHUKU_EFF_START_X;
			}else{
				eff_offset_x		= KAIHUKU_EFF_MULTI_START_X;
			}

			//�G�t�F�N�gOBJ�ǉ�
			wk->p_eff = CastleObj_Create(	&wk->castle_clact, 
											ID_CHAR_CSR, ID_PLTT_CSR, 
											ID_CELL_CSR, CASTLE_ANM_ITEMGET_EFF, 
											eff_offset_x + (KAIHUKU_EFF_WIDTH_X * poke_pos),
											KAIHUKU_EFF_START_Y,
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
	//case SEL_TUYOSA:
	case FC_PARAM_TUYOSA:				//�悳
		wk->eff_init_flag = 0;
		return TRUE;
		break;

	//�u�킴�v
	//case SEL_WAZA:
	case FC_PARAM_WAZA:					//�킴
		wk->eff_init_flag = 0;
		return TRUE;
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e���E�B���h�E�̍��ɑI�����Ă���|�P�����A�C�R�����ړ�
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 * @param	flag	1=�A�C�e���E�B���h�E�ʒu�Ɉړ��A0=���̈ʒu�ɖ߂�
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_ItemWinPokeIconMove( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 flag )
{
	u8 poke_pos,pri;
	u16 icon_offset_x;
	s16 x,y,itemkeep_x,itemkeep_y;

	poke_pos = GetCsrPokePos( wk->h_max, csr_pos );

	if( Castle_CommCheck(wk->type) == FALSE ){
		icon_offset_x = ICON_START_X;
	}else{
		icon_offset_x = ICON_MULTI_START_X;
	}

	//�|�P�����A�C�R���̈ʒu��ύX
	if( flag == 1 ){
		x = CASTLE_ITEM_POKE_ICON_X;
		y = CASTLE_ITEM_POKE_ICON_Y;
		pri = CASTLE_BG_PRI_HIGH;

		//�|�P�����A�C�R���ƁA�A�C�e�������Ă���A�C�R�����\��
		//CastleObj_Vanish( wk->p_icon[poke_pos], CASTLE_VANISH_OFF );	//�\��
		//CastleObj_Vanish( wk->p_itemkeep[poke_pos], CASTLE_VANISH_OFF );//�\��
	}else{
		x = (ICON_WIDTH_X * poke_pos + icon_offset_x);
		y = ICON_START_Y;
		pri = CASTLE_BG_PRI_LOW;

		//�|�P�����A�C�R���ƁA�A�C�e�������Ă���A�C�R�����\��
		//CastleObj_Vanish( wk->p_icon[poke_pos], CASTLE_VANISH_ON );		//��\��
		//CastleObj_Vanish( wk->p_itemkeep[poke_pos], CASTLE_VANISH_ON );	//��\��
	}

	//�A�C�e�������Ă���A�C�R��
	itemkeep_x = ( x + CASTLE_ITEMKEEP_OFFSET_X );
	itemkeep_y = ( y + CASTLE_ITEMKEEP_OFFSET_Y );
	CastleObj_SetInitXY( wk->p_itemkeep[poke_pos], itemkeep_x, itemkeep_y );
	CastleObj_SetObjPos(wk->p_itemkeep[poke_pos], itemkeep_x, itemkeep_y );
	CastleObj_PriorityChg( wk->p_itemkeep[poke_pos], pri );

	//////////////////////////////////////////////////////////////////////////////////

	//�|�P�����A�j���ŏ������W���g�p���Ă���̂ŏ���������
	CastleObj_SetInitXY( wk->p_icon[poke_pos], x, y );
	CastleObj_SetObjPos(wk->p_icon[poke_pos], x, y );
	CastleObj_PriorityChg( wk->p_icon[poke_pos], pri );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[�W�J���[����|�P�����A�j���i���o�[�擾
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"�A�j���i���o�["
 */
//--------------------------------------------------------------
static u32 CastleMine_GetPokeAnmNo( CASTLE_MINE_WORK* wk, u8 gauge_color )
{
	u32 anm_no;

	switch( gauge_color ){

	case HP_DOTTO_MAX:
		anm_no = POKEICON_ANM_HPMAX;
		break;

	case HP_DOTTO_GREEN:	// ��
		anm_no = POKEICON_ANM_HPGREEN;
		break;

	case HP_DOTTO_YELLOW:	// ��
		anm_no = POKEICON_ANM_HPYERROW;
		break;

	case HP_DOTTO_RED:		// ��
		anm_no = POKEICON_ANM_HPRED;
		break;
	};

	return anm_no;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[�W�J���[����HP�A�j���i���o�[�擾
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	"�A�j���i���o�["
 */
//--------------------------------------------------------------
static u32 CastleMine_GetHpAnmNo( CASTLE_MINE_WORK* wk, u8 gauge_color )
{
	u32 hp_anm_no;

	switch( gauge_color ){

	case HP_DOTTO_MAX:
		hp_anm_no = CASTLE_ANM_HP_MIDORI;
		break;

	case HP_DOTTO_GREEN:	// ��
		hp_anm_no = CASTLE_ANM_HP_MIDORI;
		break;

	case HP_DOTTO_YELLOW:	// ��
		hp_anm_no = CASTLE_ANM_HP_KIIRO;
		break;

	case HP_DOTTO_RED:		// ��
		hp_anm_no = CASTLE_ANM_HP_AKA;
		break;
	};

	return hp_anm_no;
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
static void CastleMine_StatusWinChg( CASTLE_MINE_WORK* wk, s8 add_sub )
{
	POKEMON_PARAM* poke;
	s8 pos;

	pos	= wk->csr_pos;

	pos += add_sub;

	if( pos < 0 ){
		pos = (wk->h_max - 1);
	}else if( pos >= wk->h_max ){
		pos = 0;
	}

	//�E�B���h�E��؂�ւ�
	wk->csr_pos = pos;
	CsrMoveSub( wk );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,wk->csr_pos) );
	Castle_StatusMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_STATUS], poke );
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
static void CastleMine_WazaWinChg( CASTLE_MINE_WORK* wk, s8 add_sub )
{
	POKEMON_PARAM* poke;
	s8 pos;

	pos	= wk->csr_pos;

	pos += add_sub;

	if( pos < 0 ){
		pos = (wk->h_max - 1);
	}else if( pos >= wk->h_max ){
		pos = 0;
	}

	//�E�B���h�E��؂�ւ�
	wk->csr_pos = pos;
	CsrMoveSub( wk );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,wk->csr_pos) );
	Castle_WazaMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_STATUS], poke );
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
static void CastleMine_PairDecideDel( CASTLE_MINE_WORK* wk )
{
	CastleMine_SeqSubMenuWinClear( wk );							//���j���[��\�����Ă�����폜
	CastleMine_SeqSubListEnd( wk );
	CastleMine_ItemSeed_Del( wk );
	BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
	CastleMine_SeqSubStatusWazaDel( wk );							//�X�e�[�^�X�E�Z���폜
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//TEST
	CastleObj_Vanish( wk->p_eff_rankup, CASTLE_VANISH_ON );			//��\��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	HP,PP�̉񕜂̕K�v�����邩�`�F�b�N
 *
 * @param	
 *
 * @return	"TRUE = �񕜂̕K�v����AFALSE = �񕜂̕K�v�Ȃ�"
 */
//--------------------------------------------------------------
static BOOL CastleMine_PPRecoverCheck( POKEMON_PARAM* poke )
{
	u8 flag;

	flag = FALSE;

	//�񕜂̕K�v�����邩�`�F�b�N
	if( PokeParaGet(poke,ID_PARA_pp1,NULL) != PokeParaGet(poke,ID_PARA_pp_max1,NULL) ){
		flag = TRUE;
	}

	if( PokeParaGet(poke,ID_PARA_pp2,NULL) != PokeParaGet(poke,ID_PARA_pp_max2,NULL) ){
		flag = TRUE;
	}

	if( PokeParaGet(poke,ID_PARA_pp3,NULL) != PokeParaGet(poke,ID_PARA_pp_max3,NULL) ){
		flag = TRUE;
	}

	if( PokeParaGet(poke,ID_PARA_pp4,NULL) != PokeParaGet(poke,ID_PARA_pp_max4,NULL) ){
		flag = TRUE;
	}

	return flag;
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

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�E�B���h�E�\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BmpTalkWinPutSub( CASTLE_MINE_WORK* wk )
{
	GF_BGL_BmpWinOff( &wk->bmpwin[MINE_BMPWIN_TALKMENU] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_TALKMENU], FBMP_COL_WHITE );

	GF_BGL_BmpWinOff( &wk->bmpwin[MINE_BMPWIN_TALKMENU2] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_TALKMENU2], FBMP_COL_WHITE );

	CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );
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
 * @param	wk			CASTLE_MINE_WORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
BOOL CastleMine_CommSetSendBuf( CASTLE_MINE_WORK* wk, u16 type, u16 param )
{
	int ret,command;

	switch( type ){

	//���O
	case CASTLE_COMM_MINE_PAIR:
		command = FC_CASTLE_MINE_PAIR;
		CastleMine_CommSendBufBasicData( wk, type );
		break;

	//���N�G�X�g
	case CASTLE_COMM_MINE_UP_TYPE:
		command = FC_CASTLE_MINE_REQ_TYPE;
		CastleMine_CommSendBufRankUpType( wk, type, param );
		break;

	//�J�[�\���ʒu
	case CASTLE_COMM_MINE_CSR_POS:
		command = FC_CASTLE_MINE_CSR_POS;
		CastleMine_CommSendBufCsrPos( wk, type );
		break;

	//�u�߂�v
	case CASTLE_COMM_MINE_MODORU:
		command = FC_CASTLE_MINE_MODORU;
		CastleMine_CommSendBufModoru( wk );
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleMine_CommSendBufBasicData( CASTLE_MINE_WORK* wk, u16 type )
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
void CastleMine_CommRecvBufBasicData(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_MINE_WORK* wk = work;
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleMine_CommSendBufRankUpType( CASTLE_MINE_WORK* wk, u16 type, u16 param )
{
	OS_Printf( "******�L���b�X��****** �����N�A�b�v���������N�G�X�g��񑗐M\n" );

	//�R�}���h
	wk->send_buf[0] = type;
	OS_Printf( "���M�Ftype = %d\n", wk->send_buf[0] );

	//�I�����Ă���ʒu
	wk->send_buf[1] = param;
	OS_Printf( "���M�Fcsr_pos = %d\n", wk->send_buf[1] );

	//��Ɏq�̑I�������Ă��Ȃ��āA�܂��l�������Ă��Ȃ����́A�e�̌���̓Z�b�g���Ă��܂�
	if( CommGetCurrentID() == COMM_PARENT_ID ){
		if( wk->parent_decide_pos == CASTLE_MINE_DECIDE_NONE ){
			wk->parent_decide_pos	= param;
			//wk->parent_decide_lr	= (wk->csr_pos % wk->h_max);
		}
	}

	//�e�̌���^�C�v
	wk->send_buf[2] = wk->parent_decide_pos;
	OS_Printf( "���M�Fparent_decide_pos = %d\n", wk->send_buf[2] );

	//�p�[�g�i�[�����A�E�ŁA�����A����̂ǂ���̃����N���グ�悤�Ƃ��Ă��邩
	//wk->send_buf[3] = (wk->csr_pos % wk->h_max);
	//OS_Printf( "���M�Frankup_lr = %d\n", wk->send_buf[3] );

	//����̃A�C�e���i���o�[
	wk->send_buf[4] = wk->parent_decide_item;
	OS_Printf( "���M�Fitem = %d\n", wk->send_buf[4] );

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
void CastleMine_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work)
{
	int i,num;
	CASTLE_MINE_WORK* wk = work;
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
		if( wk->parent_decide_pos != CASTLE_MINE_DECIDE_NONE ){
			wk->pair_sel_pos = 0;
			//wk->parent_decide_lr= (wk->csr_pos % wk->h_max);
		}else{

			//�e�̌��肪���܂��Ă��Ȃ����́A
			//�e�����M���鎞�Ɂu�q�ɂ���ł�����v�Ƒ��M����
			//wk->parent_decide_pos	= wk->pair_sel_pos;
			
			//�q�̑I�����̗p���ꂽ���Ƃ��킩��悤�ɃI�t�Z�b�g("modoru_pos")��������
			wk->parent_decide_pos	= wk->pair_sel_pos + wk->modoru_pos;
			wk->parent_decide_item	= recv_buf[4];
			wk->parent_decide_type	= recv_buf[5];
		}
	////////////////////////////////////////////////////////////////////////
	//�q
	}else{
		//�e�̌���^�C�v
		wk->parent_decide_pos = recv_buf[2];
		
		//�p�[�g�i�[�����A�E�ŁA�����A����̂ǂ���̃����N���グ�悤�Ƃ��Ă��邩
		//wk->parent_decide_lr	= recv_buf[3];
		wk->parent_decide_item	= recv_buf[4];
		wk->parent_decide_type	= recv_buf[5];
	}

	OS_Printf( "��M�Fwk->parent_decide_pos = %d\n", wk->parent_decide_pos );
	//OS_Printf( "��M�Fparent_decide_lr = %d\n", wk->parent_decide_lr );
	OS_Printf( "��M�Fparent_decide_item = %d\n", wk->parent_decide_item );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	send_buf�ɃJ�[�\���ʒu���Z�b�g
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleMine_CommSendBufCsrPos( CASTLE_MINE_WORK* wk, u16 type )
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
void CastleMine_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work)
{
	CASTLE_MINE_WORK* wk = work;
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
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleMine_CommSendBufModoru( CASTLE_MINE_WORK* wk )
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
void CastleMine_CommRecvBufModoru(int id_no,int size,void *pData,void *work)
{
	CASTLE_MINE_WORK* wk = work;
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
//	�񕜏���
//
//==============================================================================================
#define PLACE_CASTLE	(0)		//�ꏊ(�ߊl�����ꏊ�Ɠ����������`�F�b�N���ĂȂ��x�𑀍�)

//--------------------------------------------------------------
/**
 * @brief	�A�C�e���g�p
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_ItemUse( POKEMON_PARAM* pp, u16 item )
{
	StatusRecover( pp, item, 0, PLACE_CASTLE, HEAPID_CASTLE );
	return;
}


//==============================================================================================
//
//	�T�u�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�A�p�[�g�i�[���\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubNameWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y;

	//�I�t�Z�b�g�擾
	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	if( Castle_CommCheck(wk->type) == FALSE ){
		x = offset_x + INFO_PLAYER_X;
		y = offset_y + INFO_PLAYER_Y;
		PlayerNameWrite( wk, win, x, y, BC_FONT );			//�v���C���[����\��
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
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	��
 *
 * @param	type	FC_PARAM_??
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubKaihuku( CASTLE_MINE_WORK* wk, u8 csr_pos, u8 type )
{
	u32 now_rank;
	POKEMON_PARAM* poke;

	OS_Printf( "**********csr_pos = %d\n", csr_pos );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	OS_Printf( "**********poke = %d\n", poke );

#if 0
	//CP���炷
	Castle_CPRecord_Sub(wk->fro_sv, wk->type,
						kaihuku_cp_tbl[type-FC_PARAM_LIST_START_KAIHUKU] );

	CastleMine_SeqSubCPWrite( wk, &wk->bmpwin[MINE_BMPWIN_TR1] );		//CP�\��
#endif

	//���݂̉񕜃����N�ɉ��������b�Z�[�W
	now_rank = Castle_GetRank( wk->sv, wk->type, CASTLE_RANK_TYPE_KAIHUKU );
	Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
	CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
	wk->msg_index = Castle_EasyMsg( wk, kaihuku_msg[type-FC_PARAM_LIST_START_KAIHUKU], FONT_TALK );

	//�񕜑O��HP��ۑ����Ă���
	//wk->before_hp = PokeParaGet( poke, ID_PARA_hp, NULL );

	//���ۂ̉񕜏���
	switch( type ){

	case FC_PARAM_HP_KAIHUKU:
		CastleMine_ItemUse( poke, ITEM_MANTANNOKUSURI );
		break;

	case FC_PARAM_PP_KAIHUKU:
		CastleMine_ItemUse( poke, ITEM_PIIPIIMAKKUSU );
		break;

	case FC_PARAM_HPPP_KAIHUKU:
		CastleMine_ItemUse( poke, ITEM_MANTANNOKUSURI );
		CastleMine_ItemUse( poke, ITEM_PIIPIIMAKKUSU );
		break;

	default:
		GF_ASSERT( 0 );
	};

	Snd_SePlay( SEQ_SE_DP_KAIFUKU );
	//Castle_PokeHpMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_HP] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���肵���A�C�e���̂b�o���炷�A��������A���b�Z�[�W�\��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubItem( CASTLE_MINE_WORK* wk, u8 csr_pos, u16 item )
{
	POKEMON_PARAM* poke;

	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );

	//�A�C�e����������
	PokeParaPut( poke, ID_PARA_item, &item );

	//�A�C�e�������Ă���A�C�R����\��
	CastleObj_Vanish( wk->p_itemkeep[GetCsrPokePos(wk->h_max,csr_pos)], CASTLE_VANISH_OFF );//�\��

	//�u���Ɂ����������܂����I�v
	Castle_SetPokeName( wk, 0, PPPPointerGet(poke) );
	WORDSET_RegisterItemName( wk->wordset, 1, item );
	wk->msg_index = Castle_EasyMsg( wk, msg_castle_poke_07, FONT_TALK );

	//Snd_SePlay( SEQ_SE_DP_DENSI16 );
	//Snd_SePlay( SEQ_SE_DP_SUTYA );
	//Snd_SePlay( SEQ_SE_PL_BAG_030 );
	Snd_SePlay( SEQ_SE_DP_UG_020 );

	//Castle_NameCPWinWrite( wk, &wk->bmpwin[MINE_BMPWIN_CP] );	//���X�g�I������̂ōX�V�Ȃ�
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
static void CastleMine_SeqSubMenuWinClear( CASTLE_MINE_WORK* wk )
{
	if( wk->menu_flag == 1 ){
		wk->menu_flag = 0;
		BmpMenuExit( wk->mw, NULL );
		BmpMenuWinClear( wk->MenuH.win, WINDOW_TRANS_OFF );
		//BmpMenuWinClear( wk->MenuH.win, WINDOW_TRANS_OFF );
		//GF_BGL_BmpWinOnVReq( wk->MenuH.win );
		GF_BGL_BmpWinOffVReq( wk->MenuH.win );
	}
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�J�n�t���O�Z�b�g
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubListStart( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	SDK_ASSERTMSG(wk->list_flag == 0,"���X�g�������Ă���̂ɁA�ēx���X�g���J�n���悤�Ƃ��Ă��܂�");
	wk->list_flag	= 1;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�e�����X�g���I���������ɂ��鏈��
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubItemListEnd( CASTLE_MINE_WORK* wk )
{
	CastleObj_Vanish( wk->p_itemicon, CASTLE_VANISH_ON );					//��\��
	CastleObj_Vanish( wk->p_scr_u, CASTLE_VANISH_ON );						//��\��
	CastleObj_Vanish( wk->p_scr_d, CASTLE_VANISH_ON );						//��\��
	CastleObj_Vanish( wk->p_item_csr, CASTLE_VANISH_ON );					//��\��

	//���O�{CP�E�B���h�E������
	BmpMenuWinClear( &wk->bmpwin[MINE_BMPWIN_CP], WINDOW_TRANS_OFF );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_CP] );

	//�|�P�����A�C�R���̈ʒu��߂�
	CastleMine_ItemWinPokeIconMove( wk, wk->csr_pos, 0 );

	//HP��\���ALV��\��
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_ITEM_HP], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_ITEM_HP] );
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_ITEM_LV], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_ITEM_LV] );

	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );		//OBJ��\��
	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//TEST
	return;;
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�֖߂�
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubItemListReturn( CASTLE_MINE_WORK* wk )
{
	u16 pos_bak;

	BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );
	CastleObj_Vanish( wk->p_itemicon, CASTLE_VANISH_OFF );		//�\��
	BmpListRewrite( wk->lw );
	BmpListDirectPosGet( wk->lw, &pos_bak );
	CastleItemInfoWriteMsg( wk, &wk->bmpwin[MINE_BMPWIN_ITEMINFO],
							GetItemNo(wk,pos_bak,wk->parent_decide_type) );
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
static void CastleMine_SeqSubListEnd( CASTLE_MINE_WORK* wk )
{
	GF_BGL_BMPWIN* p_list_win;

	if( wk->list_flag == 1 ){
		wk->list_flag = 0;
		p_list_win = (GF_BGL_BMPWIN*)BmpListParamGet( wk->lw, BMPLIST_ID_WIN );

		BmpMenuWinClear( p_list_win, WINDOW_TRANS_OFF );
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
 * @brief	CP�X�V
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubCPWrite( CASTLE_MINE_WORK* wk, GF_BGL_BMPWIN* win )
{
	u16 x,y,offset_x,offset_y,pair_offset_x,pair_offset_y,now_cp;

	Castle_GetOffset( wk, &offset_x, &offset_y, &pair_offset_x, &pair_offset_y );

	if( Castle_CommCheck(wk->type) == FALSE ){
		x = offset_x + INFO_PLAYER_CP_X;
		y = offset_y + INFO_PLAYER_CP_Y;
		GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );

		now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

		Castle_SetNumber(	wk, 0, now_cp, CASTLE_KETA_CP,
							NUMBER_DISPTYPE_SPACE);
		wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_poke_cp_01, x, y , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
	}else{

		//�e�A�q�̉�ʂƂ��A�e�A�q�̏��Ԃ�CP���\�������悤�ɂ���

		////////////////////////////////////////////////////////////////
		//�e�Ȃ�
		if( CommGetCurrentID() == COMM_PARENT_ID ){
			x = offset_x + INFO_PLAYER_CP_X;
			y = offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

			Castle_SetNumber(	wk, 0, now_cp, CASTLE_KETA_CP,
								NUMBER_DISPTYPE_SPACE);
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_poke_cp_01, x, y , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

			//�p�[�g�i�[��CP��\��
			x = pair_offset_x + INFO_PLAYER_CP_X;
			y = pair_offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );
			Castle_SetNumber( wk, 0, wk->pair_cp, CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_poke_cp_02, x, y , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

		////////////////////////////////////////////////////////////////
		//�q�Ȃ�
		}else{
			//�p�[�g�i�[��CP��\��
			x = offset_x + INFO_PLAYER_CP_X;
			y = offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );
			Castle_SetNumber( wk, 0, wk->pair_cp, CASTLE_KETA_CP, NUMBER_DISPTYPE_SPACE );
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_poke_cp_02, x, y , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );

			x = pair_offset_x + INFO_PLAYER_CP_X;
			y = pair_offset_y + INFO_PLAYER_CP_Y;
			GF_BGL_BmpWinFill( win, FBMP_COL_NULL, x, y, 6*8, 2*8 );

			now_cp = FrontierRecord_Get(wk->fro_sv, CastleScr_GetCPRecordID(wk->type),
									//FRONTIER_RECORD_NOT_FRIEND );		//���݂�CP
									Frontier_GetFriendIndex(CastleScr_GetCPRecordID(wk->type)) );

			Castle_SetNumber(	wk, 0, now_cp, CASTLE_KETA_CP,
								NUMBER_DISPTYPE_SPACE);
			wk->msg_index = CastleWriteMsgSimple( wk, win, 
									msg_castle_poke_cp_01, x, y , MSG_NO_PUT, 
									FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BC_FONT );
		}
	}

	GF_BGL_BmpWinOnVReq( win );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����N�A�b�v
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubRankUp( CASTLE_MINE_WORK* wk, u8 parent_decide_pos, u8 parent_decide_type )
{
	u8	sel_pos,type_offset;
	u16 sub_cp;
	u16 buf16[4];
	u16 offset_x,offset_y,pair_offset_y;

	Snd_SePlay( SEQ_SE_DP_PIRORIRO2 );

	//�I�t�Z�b�g
	type_offset = wk->modoru_pos;

	//�I�񂾃J�[�\���ʒu���擾
	sel_pos = GetCommSelCsrPos( type_offset, parent_decide_pos );

	////////////////////////////////////////////////
	//����̎�ނɂ���Ĉ������CP�������
	//switch( GetCsrSelType(wk->h_max,sel_pos) ){
	switch( parent_decide_type ){

	//case SEL_KAIHUKU:
	case FC_PARAM_HP_KAIHUKU:			//�g�o�����ӂ�
	case FC_PARAM_PP_KAIHUKU:			//�o�o�����ӂ�
	case FC_PARAM_HPPP_KAIHUKU:			//���ׂĂ����ӂ�
		sub_cp = kaihuku_cp_tbl[parent_decide_type-FC_PARAM_LIST_START_KAIHUKU];;
		break;

	//case SEL_RENTAL:
	case FC_PARAM_KINOMI:				//���̂�
	case FC_PARAM_ITEM:					//�ǂ���
		sub_cp = GetItemCPByItemNo( wk->parent_decide_item );
		break;

	//case SEL_TUYOSA:
	//case SEL_WAZA:
	case FC_PARAM_TUYOSA:				//�悳
	case FC_PARAM_WAZA:					//�킴
		sub_cp = 0;
		break;
	};

	////////////////////////////////////////////////
	//�e
	if( CommGetCurrentID() == COMM_PARENT_ID ){

		//�e�F�e�̌��肪�L���Ȃ�ALR�͂��̂܂�
		if( parent_decide_pos < type_offset ){
			Castle_SetPlayerName( wk, 5 );
			Castle_CPRecord_Sub( wk->fro_sv, wk->type,sub_cp );

		//�e�F�q�̌��肪�L���Ȃ�ALR�͋t�ɂȂ�
		}else{
			Castle_SetPairName( wk->wordset, 5 );
			wk->pair_cp -= sub_cp;								//�����CP���炷
		}

	///////////////////////////////////////////////
	//�q
	}else{

		//�q�F�e�̌��肪�L���Ȃ�ALR�͋t�ɂȂ�
		if( parent_decide_pos < type_offset ){
			Castle_SetPairName( wk->wordset, 5 );
			wk->pair_cp -= sub_cp;								//�����CP���炷

		//�q�F�q�̌��肪�L���Ȃ�ALR�͂��̂܂�
		}else{
			Castle_SetPlayerName( wk, 5 );
			Castle_CPRecord_Sub(wk->fro_sv, wk->type, sub_cp );	//������CP���炷
		}
	}

	CastleMine_SeqSubCPWrite( wk, &wk->bmpwin[MINE_BMPWIN_TR1] );	//CP�\�����X�V
	CastleMine_SeqSubMenuWinClear( wk );							//���j���[��\�����Ă�����폜
	CastleMine_SeqSubStatusWazaDel( wk );							//�X�e�[�^�X�E�Z���폜

	CastleMine_ItemSeed_Del( wk );									//�A�C�e�����X�g�\�����Ă���폜
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//TEST

	BmpTalkWinClearSub( &wk->bmpwin[MINE_BMPWIN_TALK] );

	//switch( GetCsrSelType(wk->h_max,sel_pos) ){
	switch( parent_decide_type ){

	//case SEL_KAIHUKU:
	case FC_PARAM_HP_KAIHUKU:											//�g�o�����ӂ�
	case FC_PARAM_PP_KAIHUKU:											//�o�o�����ӂ�
	case FC_PARAM_HPPP_KAIHUKU:											//���ׂĂ����ӂ�
		CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
		CastleMine_SeqSubKaihuku( wk, sel_pos, parent_decide_type );
		break;

	//case SEL_RENTAL:
	case FC_PARAM_KINOMI:												//���̂�
	case FC_PARAM_ITEM:													//�ǂ���
		CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config));
		CastleMine_SeqSubItem( wk, sel_pos, wk->parent_decide_item );	//�ǂ���
		break;

	//case SEL_TUYOSA:
	case FC_PARAM_TUYOSA:												//�悳
		CastleMine_SeqSubStatusWin( wk, sel_pos );						//�悳
		break;

	//case SEL_WAZA:
	case FC_PARAM_WAZA:													//�킴
		CastleMine_SeqSubWazaWin( wk, sel_pos );						//�킴
		break;
	};

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�悳
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubStatusWin( CASTLE_MINE_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;
	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
	Castle_SetStatusBgGraphic( wk, BC_FRAME_SLIDE );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_TALK] );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_LIST] );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_CP] );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_StatusMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_STATUS], poke );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�킴
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubWazaWin( CASTLE_MINE_WORK* wk, u8 csr_pos )
{
	POKEMON_PARAM* poke;
	//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
	Castle_SetWazaBgGraphic( wk, BC_FRAME_SLIDE );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_TALK] );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_LIST] );
	GF_BGL_BmpWinOffVReq( &wk->bmpwin[MINE_BMPWIN_CP] );
	poke =  PokeParty_GetMemberPointer( wk->p_party, GetCsrPokePos(wk->h_max,csr_pos) );
	Castle_WazaMsgWrite( wk, &wk->bmpwin[MINE_BMPWIN_STATUS], poke );
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_ON );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�悳�A�킴�E�B���h�E������
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_SeqSubStatusWazaDel( CASTLE_MINE_WORK* wk )
{
	GF_BGL_BmpWinDataFill( &wk->bmpwin[MINE_BMPWIN_STATUS], FBMP_COL_NULL );	//�h��Ԃ�
	GF_BGL_BmpWinOff( &wk->bmpwin[MINE_BMPWIN_STATUS] );						//��VReq���Ȃ�
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�R���㉺�A�j��
 *
 * @param	wk		CASTLE_MINE_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleMine_IconSelAnm( CASTLE_MINE_WORK* wk )
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


//==============================================================================================
//
//	�f�o�b�N
//
//==============================================================================================
static void Debug_HpDown( CASTLE_MINE_WORK* wk, u8 no )
{
	u32 hp;
	POKEMON_PARAM* poke;

	//�|�P�����ւ̃|�C���^�擾
	poke =  PokeParty_GetMemberPointer( wk->p_party, no );

	hp = 5;
	PokeParaPut( poke, ID_PARA_hp, &hp );
	//PokeParaCalc( poke );
	return;
}


//==============================================================================================
//
//	castle_rank.c
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�����N�A�b�v(castle_rank.c����ړ�)
 *
 * @param	wk		CASTLE_RANK_WORK�^�̃|�C���^
 * @param	param	FC_PARAM_KAIHUKU_RANKUP,FC_PARAM_RENTAL_RANKUP
 *
 * @return	none
 */
//--------------------------------------------------------------
static void CastleRank_SeqSubRankUp( CASTLE_MINE_WORK* wk, u8 parent_decide_pos, u8 param )
{
	u8	type_offset,sel_pos,type,m_max;
	u32 now_rank;
	u16 buf16[4];

	m_max = Castle_GetMinePokeNum( wk->type, CASTLE_FLAG_SOLO );

	//�擾���郉���N�^�C�v
	if( param == FC_PARAM_KAIHUKU_RANKUP ){
		type = CASTLE_RANK_TYPE_KAIHUKU;
	}else{
		type = CASTLE_RANK_TYPE_RENTAL;
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

			Castle_CPRecord_Sub( wk->fro_sv, wk->type, rankup_point_tbl[type][now_rank] );

			//�����N�A�b�v
			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			buf16[0] = (now_rank + 1);
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
				CastleScr_GetRankRecordID(wk->type,type),
				Frontier_GetFriendIndex(CastleScr_GetRankRecordID(wk->type,type)), (now_rank + 1) );

			OS_Printf( "�e�F�e�������N�A�b�v %d\n", (now_rank+1) );

			//�����N�A�b�v�������ɖ߂�悪����t���OON
			if( Castle_CommCheck(wk->type) == TRUE ){
				if( param == FC_PARAM_KAIHUKU_RANKUP ){
					wk->rankup_recover_flag = 1;
				}else{
					wk->rankup_recover_flag = 2;
				}
			}

		//�e�F�q�̌��肪�L���Ȃ�
		}else{
			Castle_SetPairName( wk->wordset, 5 );
			now_rank= wk->pair_rank[type];
			wk->pair_cp -= rankup_point_tbl[type][now_rank];						//CP���炷
			wk->pair_rank[type]++;
		}

	///////////////////////////////////////////////
	//�q
	}else{

		//�q�F�e�̌��肪�L���Ȃ�
		if( parent_decide_pos < type_offset ){
			Castle_SetPairName( wk->wordset, 5 );
			now_rank= wk->pair_rank[type];
			wk->pair_cp -= rankup_point_tbl[type][now_rank];						//CP���炷
			wk->pair_rank[type]++;

		//�q�F�q�̌��肪�L���Ȃ�
		}else{
			Castle_SetPlayerName( wk, 5 );
			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			Castle_CPRecord_Sub( wk->fro_sv, wk->type, rankup_point_tbl[type][now_rank] );

			now_rank = Castle_GetRank( wk->sv, wk->type, type );

			buf16[0] = (now_rank + 1);
			FrontierRecord_Set(	SaveData_GetFrontier(wk->sv), 
				CastleScr_GetRankRecordID(wk->type,type),
				Frontier_GetFriendIndex(CastleScr_GetRankRecordID(wk->type,type)), (now_rank + 1) );
			OS_Printf( "�q�F�q�������N�A�b�v %d\n", (now_rank+1) );

			//�����N�A�b�v�������ɖ߂�悪����t���OON
			if( Castle_CommCheck(wk->type) == TRUE ){
				if( param == FC_PARAM_KAIHUKU_RANKUP ){
					wk->rankup_recover_flag = 1;
				}else{
					wk->rankup_recover_flag = 2;
				}
			}
		}
	}

	CastleMine_SeqSubMenuWinClear( wk );							//���j���[��\�����Ă�����폜

	//CP�\�����X�V
	CastleMine_SeqSubCPWrite( wk, &wk->bmpwin[MINE_BMPWIN_TR1] );

	//CastleRank_SeqSubAllInfoWrite( wk );							//���X�V

	//��b�E�B���h�E�\��
	CastleTalkWinPut( &wk->bmpwin[MINE_BMPWIN_TALK], CONFIG_GetWindowType(wk->config) );

	//�u�����N���A�b�v���܂����I�v
	wk->msg_index = Castle_EasyMsg( wk, rankup_pair_msg_tbl[type][now_rank], FONT_TALK );
	//Snd_SePlay( SEQ_SE_DP_PIRORIRO2 );
	//Snd_SePlay( SEQ_SE_DP_DANSA4 );
	return;
}


