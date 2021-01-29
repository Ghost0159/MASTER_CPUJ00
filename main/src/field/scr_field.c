//============================================================================================
/**
 * @file	scr_field.c
 * @bfief	�X�N���v�g�Ŏg�p����v���O����(��풓�t�B�[���h�v���O����)
 * @author	Satoshi Nohara
 * @date	07.12.03
 *
 * ���X�g�����̎������̂�scr_wazaoshie.c�ɂ����݂��Ă���
 */
//============================================================================================
#include "common.h"
#include "gflib/msg_print.h"
#include "system/lib_pack.h"
#include "system/window.h"
#include "savedata/frontier_savedata.h"
#include "savedata/frontier_ex.h"
#include "savedata/savedata_def.h"
#include "savedata/misc.h"
#include "fieldsys.h"
#include "zonedata.h"
#include "mapdefine.h"
#include "poketool/monsno.h"
#include "field/eventflag.h"							//EventWork_CheckEventFlag
#include "field/evwkdef.h"								//FH_FLAG_START
#include "field/fieldobj.h"
#include "script.h"										//ID_HIDE_ITEM_SCR_OFFSET
#include "scr_tool.h"

#include "system/wipe.h"

#include "scrcmd_def.h"
#include "ev_win.h"
#include "scr_field.h"
#include "msgdata\msg.naix"								//NARC_msg_??_dat
#include "msgdata\msg_ev_win.h"	
#include "src/application/zukanlist/zkn_data/zukan_data.naix"

#include "system/msgdata.h"								//MSGMAN_TYPE_DIRECT
#include "system/pm_str.h"
#include "system/fontproc.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
#include "fieldmap.h"
#include "fld_bmp.h"

#include "poketool/boxdata.h"
#include "savedata/sodateyadata.h"
#include "battle/battle_server.h"

#include "../frontier/frontier_def.h"
#include "syswork.h"

#include	"itemtool/item.h"
#include	"itemtool/itemequip.h"
#include	"itemtool/itemsym.h"

#include "communication/comm_wifihistory.h"

#include "../fielddata/eventdata/zone_c04evc.h"
#include "../../include/pl_bugfix.h"

//==============================================================================================
//
//	��`
//
//==============================================================================================
#define ZKN_SORTDATA_ONESIZE		(sizeof(u16))


//============================================================================================
//
//	�v���g�^�C�v�錾	
//
//============================================================================================
static u16* ZKN_SORTDATA_Get( int heap, int idx, int* p_arry_num );
BOOL EvCmdSeisekiBmpListStart( VM_MACHINE * core );
static BOOL EvSeisekiSelWinWait(VM_MACHINE * core);
static void	GetMezameruPowerParam(POKEMON_PARAM *pp,int *power,int *type);
BOOL EvCmdSetFavoritePoke( VM_MACHINE * core );
BOOL EvCmdGetFavoritePoke( VM_MACHINE * core );
BOOL EvCmdVillaListCheck( VM_MACHINE * core );


//============================================================================================
//
//	�f�[�^
//
//============================================================================================
static u16 arc_tbl[] = {
	NARC_zukan_data_zkn_sort_a_dat,				//��
	NARC_zukan_data_zkn_sort_ka_dat,			//��
	NARC_zukan_data_zkn_sort_sa_dat,			//��
	NARC_zukan_data_zkn_sort_ta_dat,			//��
	NARC_zukan_data_zkn_sort_na_dat,			//��
	NARC_zukan_data_zkn_sort_ha_dat,			//��
	NARC_zukan_data_zkn_sort_ma_dat,			//��
	NARC_zukan_data_zkn_sort_ra_dat,			//��		//���ӁI�@�}�ӂ̕��тɂ��킹��
	NARC_zukan_data_zkn_sort_yawa_dat,			//���		//���ӁI�@�}�ӂ̕��тɂ��킹��
};
#define ARC_TBL_MAX		( NELEMS(arc_tbl) )


//==============================================================================================
//
//	ev_win�̃��X�g���������ɂ��Ă���
//
//==============================================================================================
//�L���ŁA�\������荀�ڐ����������X�g�����́A�㉺�ɃX�N���[���J�[�\����\��
//�O���t�B�b�N�f�[�^���Ȃ��̂ŁA���̕ӂ͉��ł��B
//#define EV_WIN_SCROLL_CURSOR

#ifdef EV_WIN_SCROLL_CURSOR	//070228
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/clact_tool.h"
#include "field_clact.h"
#include "field/ranking.naix"			//�O���t�B�b�N�f�[�^���Ȃ��̂ŉ�
#endif


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
static void EvWin_MsgManSet( SEISEKI_WORK* wk, MSGDATA_MANAGER* msgman );
static void EvWin_Init( FIELDSYS_WORK* fsys, SEISEKI_WORK* wk, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman, u16* lp_work, u16* cp_work );

//BMP���X�g
SEISEKI_WORK * CmdSeisekiBmpList_Init( FIELDSYS_WORK* fsys, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman, u16* lp_work, u16* cp_work);
void CmdSeisekiBmpList_MakeList( SEISEKI_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  );
static void CmdSeisekiBmpList_Start( SEISEKI_WORK* wk );

static void BmpList_list_make( SEISEKI_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  );
static u32 BmpList_length_get( SEISEKI_WORK* wk );
static void BmpList_h_default_set(SEISEKI_WORK* wk);
static void	BmpList_CallBack( BMPLIST_WORK* lw, u32 param, u8 y );
static void BmpList_CursorMoveCallBack( BMPLIST_WORK* wk, u32 param, u8 mode );
static void EvBmpList_MainTCB( TCB_PTR tcb, void* wk );
static void EvBmpList_Del( SEISEKI_WORK* wk );

#ifdef EV_WIN_SCROLL_CURSOR	//070228
static void EvWin_ActorInit( SEISEKI_WORK* wk );
static void EvWin_ActorRelease( SEISEKI_WORK* wk );
#endif


//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�������A���X�g�쐬�A�J�n
 *
 * @param	none
 *
 * @retval	1
 */
//--------------------------------------------------------------
BOOL EvCmdSeisekiBmpListStart( VM_MACHINE * core )
{
	LOAD_RESULT load_ret;
	FRONTIER_EX_SAVEDATA* fes;
	u16 renshou;
	u16* p_data;
	int i,sort_data_count;
	MSGDATA_MANAGER* man;
	MSGDATA_MANAGER* ev_win_man;
	FIELDSYS_WORK* fsys	= core->fsys;
	SEISEKI_WORK* seiseki_win;
	WORDSET** wordset			= GetEvScriptWorkMemberAdrs( fsys, ID_EVSCR_WORDSET );
	u16 type					= VMGetWorkValue( core );
	u16 param2					= VMGetWorkValue( core );
	u16 wk_id					= VMGetU16( core );
	u16 wk_id2					= VMGetU16( core );				//lp_work�p
	u16 wk_id3					= VMGetU16( core );				//cp_work�p

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	//�|�P������
	man = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_monsname_dat, HEAPID_EVENT );

	//evwin������
	seiseki_win	= CmdSeisekiBmpList_Init(	fsys, 20, 1, 0, 1, 
									GetEventWorkAdrs(fsys,wk_id), *wordset, 
									GetEvScriptWorkMemberAdrs(core->fsys,ID_EVSCR_MSGWINDAT), man, 
									GetEventWorkAdrs(fsys,wk_id2),
									GetEventWorkAdrs(fsys,wk_id3) );

	//�t�����e�B�A�O���f�[�^�̃��[�h
	fes = FrontierEx_Load( fsys->savedata, HEAPID_WORLD, &load_ret );

	//�f�[�^����ǂݍ���
	if( load_ret == LOAD_RESULT_OK ){

		//���X�g�쐬
		p_data = ZKN_SORTDATA_Get( HEAPID_EVENT, arc_tbl[param2], &sort_data_count );
		for( i=0; i < sort_data_count ;i++ ){

			OS_Printf( "p_data[%d] = %d\n", i, p_data[i] );
			OS_Printf( "renshou = %d\n", renshou );

			renshou = FrontierEx_StageRenshou_Get(	fsys->savedata, fes, 
													StageScr_GetExMaxWinRecordID(type), p_data[i] );
	
			//�A���L�^�����݂����烊�X�g�쐬
			if( renshou != 0 ){
				CmdSeisekiBmpList_MakeList(seiseki_win, p_data[i], EV_WIN_TALK_MSG_NONE, p_data[i]);
			}
		}

		sys_FreeMemoryEz( p_data );
	}

	if( fes != NULL ){
		sys_FreeMemoryEz( fes );
	}

#if 1
	//ev_win.gmm����u���ǂ�v���g�p���Ă���
	
	ev_win_man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_ev_win_dat, HEAPID_EVENT );
	EvWin_MsgManSet( seiseki_win, ev_win_man );
	CmdSeisekiBmpList_MakeList(	seiseki_win, msg_ev_win_013, 
								EV_WIN_TALK_MSG_NONE, EV_WIN_B_CANCEL );
	MSGMAN_Delete( ev_win_man );

	//�ꉞ�߂�
	EvWin_MsgManSet( seiseki_win, man );
#endif

	//���X�g�J�n
	CmdSeisekiBmpList_Start( seiseki_win );
	
	VM_SetWait( core, EvSeisekiSelWinWait );

	MSGMAN_Delete( man );
	return 1;
}

//�E�F�C�g�֐�
static BOOL EvSeisekiSelWinWait(VM_MACHINE * core)
{
	FIELDSYS_WORK* fsys = core->fsys;
	u16* ret_wk = GetEventWorkAdrs( fsys, core->reg[0] );	//���ӁI

	if( *ret_wk == EV_WIN_NOTHING ){
		return FALSE;	//�p��
	}

	return TRUE;		//�I��
}

//----------------------------------------------------------------------------
/**
 *	@brief	�\�[�g�ް��擾
 *
 *	@param	heap		�q�[�v
 *	@param	idx			�ް����ޯ��
 *	@param	p_arry_num	�z��v�f���擾��
 *
 *	@return	�ް��o�b�t�@		�v�f����p_arry_num�Ɋi�[
 */
//-----------------------------------------------------------------------------
static u16* ZKN_SORTDATA_Get( int heap, int idx, int* p_arry_num )
{
	u32 size;
	u16* p_buf;
	
	//�ǂݍ���
	p_buf = ArcUtil_LoadEx( ARC_ZUKAN_DATA, idx, FALSE, 
							heap, ALLOC_TOP, &size );

	*p_arry_num = size / ZKN_SORTDATA_ONESIZE;
	return p_buf;
}


//==============================================================================================
//
//	��`
//
//==============================================================================================
#define EV_WIN_FONT				(FONT_SYSTEM)	//�t�H���g�w��

#define EVWIN_MSG_BUF_SIZE		(40*2)			//���b�Z�[�W�o�b�t�@�T�C�Y

#define EV_WIN_LIST_MAX			(120)			//BMP���X�g���ڂ̍ő吔
#define	EV_WIN_MSG_MAX			(120)			//MSG�o�b�t�@�̍ő吔

//BMP���X�g�w�b�_�[��`
#define EV_LIST_LINE			(8)				//�\���ő區�ڐ�
#define EV_LIST_RABEL_X			(1)				//���x���\���w���W
#define EV_LIST_DATA_X			(12)			//���ڕ\���w���W
#define EV_LIST_CURSOR_X		(2)				//�J�[�\���\���w���W
#define EV_LIST_LINE_Y			(1)				//�\���x���W

//�J�[�\����
#define EV_WIN_CURSOR_WIDTH		(12)			//�J�[�\����

#define EV_WIN_DEFAULT_WAIT		(3)				//�L�[���삪�����ɓ���Ȃ��悤�Ɋ�{�E�F�C�g

// ����������g���ĉ������I�I
#define	EVWIN_FREE_CGX			( 1 )			//�t���[�̃L�����ʒu

#ifdef EV_WIN_SCROLL_CURSOR	//070228
#define CURSOR_ANMNO				(0)
#define CURSOR_SPRI					(0)
#define CURSOR_PALT					(1)

#define EVWIN_CHR_H_ID_CURSOR		(13528)
#define EVWIN_PAL_H_ID				(13528)
#define EVWIN_CEL_H_ID_CURSOR		(13528)
#define EVWIN_ANM_H_ID_CURSOR		(13528)

#define EVWIN_ACTMAX				(2)			//�A�N�^�[��(������Ɖ������̂Q��)

#define ACT_RES_PAL_NUM				(3)

//�O���t�B�b�N�f�[�^���Ȃ��̂Œu������
enum{
	ARC_EVWIN_GRA	= ARC_RANKING_GRA,
	NARC_evwin_nclr = NARC_ranking_ranking_nclr,
	NARC_evwin_ncer = NARC_ranking_ranking_ncer,
	NARC_evwin_nanr = NARC_ranking_ranking_nanr,
	NARC_evwin_ncgr = NARC_ranking_ranking_ncgr,
};
#endif


//==============================================================================================
//
//	�\����
//
//==============================================================================================
struct _SEISEKI_WORK{
	FIELDSYS_WORK * fsys;						//FIELDSYS_WORK�̃|�C���^
	TCB_PTR	tcb;

 	GF_BGL_BMPWIN bmpwin;						//BMP�E�B���h�E�f�[�^
 	GF_BGL_BMPWIN* talk_bmpwin;					//BMP��b�E�B���h�E�f�[�^
	
	STRBUF* msg_buf[EV_WIN_MSG_MAX];			//���b�Z�[�W�f�[�^�̃|�C���^
	MSGDATA_MANAGER* msgman;					//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;							//�P��Z�b�g

	u8  wait;									//�E�F�C�g
	u8  menu_id;								//BMP���j���[ID
	u8  cursor_pos;								//�J�[�\���ʒu
	u8  cancel:1;								//�L�����Z��
	u8  msgman_del_flag:1;						//���b�Z�[�W�}�l�[�W���[�폜�t���O
	u8  dmy:6;									//

	u8  x;										//�E�B���h�E�ʒuX
	u8  y;										//�E�B���h�E�ʒuY
	u8  dmyy;									//���j���[�i���o�[
	u8  list_no;								//���j���[���ڂ̉��Ԗڂ�

	u16* pMsg;									//���b�Z�[�W�f�[�^
	u16* work;									//���ʎ擾���[�N
	u16* lp_work;								//list_bak�擾���[�N
	u16* cp_work;								//cursor_bak�擾���[�N
 
	//BMP���X�g
	BMPLIST_HEADER ListH;						//BMP���X�g�w�b�_�[
	BMPLIST_WORK* lw;							//BMP���X�g�f�[�^
	u16 list_bak;								//���X�g�ʒu�o�b�N�A�b�v
	u16 cursor_bak;								//�J�[�\���ʒu�o�b�N�A�b�v
	BMPLIST_DATA list_Data[EV_WIN_LIST_MAX];	//���X�g�f�[�^
	u16 talk_msg_id[EV_WIN_LIST_MAX];			//���X�g�f�[�^�ɑ΂ɂȂ��b���b�Z�[�WID

	u16 pos_bak;

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	FIELD_CLACT		fcat;						//�t�B�[���h�p�Z���A�N�^�[�ݒ�
	CATS_ACT_PTR	act[EVWIN_ACTMAX];			//CATS���g�����̐�p�A�N�^�[�\����
#endif
};


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�E�B���h�E�@���b�Z�[�W�}�l�[�W���[�Z�b�g
 *
 * @param	wk			WAZA_OSHIE_WORK�^�̃|�C���^
 * @param	msgman		MSGDATA_MANAGER�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvWin_MsgManSet( SEISEKI_WORK* wk, MSGDATA_MANAGER* msgman )
{
	wk->msgman = msgman;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�C�x���g�E�B���h�E�@���[�N������
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvWin_Init( FIELDSYS_WORK* fsys, SEISEKI_WORK* wk, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman, u16* lp_work, u16* cp_work )
{
	int i;

	wk->msgman		= msgman;		//���p��
	wk->msgman_del_flag = 0;
	wk->wordset		= wordset;		//�X�N���v�g�������p��

	wk->fsys		= fsys;
	wk->work		= work;
#if 1
	*wk->work		= 0;			//������
#endif
	wk->lp_work		= lp_work;
	wk->cp_work		= cp_work;
	wk->cancel		= cancel;
	wk->cursor_pos	= cursor;
	wk->x			= x;
	wk->y			= y;
	wk->list_no		= 0;
	wk->talk_bmpwin	= talk_bmpwin;
	wk->wait		= EV_WIN_DEFAULT_WAIT;
	wk->pos_bak		= cursor;

	for( i=0; i < EV_WIN_LIST_MAX ;i++ ){
		wk->list_Data[i].str	= NULL;
		wk->list_Data[i].param	= 0;
		wk->talk_msg_id[i] = EV_WIN_TALK_MSG_NONE;
	}

	//MSGMAN_Create�̌�ɏ���
	for( i=0; i < EV_WIN_MSG_MAX ;i++ ){
		wk->msg_buf[i] = STRBUF_Create( EVWIN_MSG_BUF_SIZE, HEAPID_FIELD );
	}

	//�I�������l���擾���郏�[�N��������
	*wk->work = EV_WIN_NOTHING;

	return;
}


//==============================================================================================
//
//	BMP���X�g�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@������
 *
 * @param	x			�E�B���h�E�\��X���W
 * @param	y			�E�B���h�E�\��X���W
 * @param	cursor		�J�[�\���ʒu
 * @param	cancel		B�L�����Z���t���O(TRUE=�L���AFALSE=����)
 * @param	work		���ʂ������郏�[�N�̃|�C���^
 * @param	wordset		WORDSET�^�̃|�C���^
 *
 * @retval	"SEISEKI_WORK�^�̃A�h���X�ANULL�͎��s"
 */
//--------------------------------------------------------------
SEISEKI_WORK * CmdSeisekiBmpList_Init( FIELDSYS_WORK* fsys, u8 x, u8 y, u8 cursor, u8 cancel, u16* work, WORDSET* wordset, GF_BGL_BMPWIN* talk_bmpwin, MSGDATA_MANAGER* msgman, u16* lp_work, u16* cp_work )
{
	SEISEKI_WORK* wk;
	int i;

	wk = sys_AllocMemory( HEAPID_FIELD, sizeof(SEISEKI_WORK) );
	if( wk == NULL ){
		OS_Printf( "ev_win.c Alloc ERROR!" );
		return NULL;
	}
	memset( wk, 0, sizeof(SEISEKI_WORK) );

	//���[�N������	
	EvWin_Init( fsys, wk, x, y, cursor, cancel, work, wordset, talk_bmpwin, msgman, lp_work, cp_work );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g�쐬
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPLIST�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void CmdSeisekiBmpList_MakeList( SEISEKI_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	BmpList_list_make( wk, msg_id, talk_msg_id, param  );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@�J�n
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void CmdSeisekiBmpList_Start( SEISEKI_WORK* wk )
{
	//�\���ő區�ڐ��`�F�b�N
	if( wk->list_no > EV_LIST_LINE ){
		GF_BGL_BmpWinAdd( wk->fsys->bgl, &wk->bmpwin, FLD_MBGFRM_FONT, wk->x, wk->y, 
									11, EV_LIST_LINE*2, FLD_SYSFONT_PAL, EVWIN_FREE_CGX );
	}else{
		GF_BGL_BmpWinAdd( wk->fsys->bgl, &wk->bmpwin, FLD_MBGFRM_FONT, wk->x, wk->y, 
									11, wk->list_no*2, FLD_SYSFONT_PAL, EVWIN_FREE_CGX );
	}

	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
	MenuWinGraphicSet(
		wk->fsys->bgl, FLD_MBGFRM_FONT, MENU_WIN_CGX_NUM, MENU_WIN_PAL, 0, HEAPID_FIELD );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite(&wk->bmpwin, WINDOW_TRANS_OFF, MENU_WIN_CGX_NUM, MENU_WIN_PAL);

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	EvWin_ActorInit( wk );
#endif

	//���X�g�w�b�_�[�����ݒ�
	BmpList_h_default_set(wk);

	//OS_Printf( "*wk->lp_work = %d\n", *wk->lp_work );
	//OS_Printf( "*wk->cp_work = %d\n", *wk->cp_work );
	wk->lw = BmpListSet(	(const BMPLIST_HEADER*)&wk->ListH, *wk->lp_work, 
							*wk->cp_work, HEAPID_FIELD );

	//TCB�ǉ�
	wk->tcb	= TCB_Add( EvBmpList_MainTCB, wk, 0 );

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	���X�g�쐬
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 * @param	msg_id		���b�Z�[�WID
 * @param	talk_msg_id	��b���b�Z�[�WID
 * @param	param		BMPLIST�p�����[�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_list_make( SEISEKI_WORK* wk, u32 msg_id, u32 talk_msg_id, u32 param  )
{
	int i;
	void* msg;

	SDK_ASSERTMSG( wk->list_no < EV_WIN_LIST_MAX, "���X�g���ڐ��I�[�o�[�I" );

	{
		//�W�J����
		
		STRBUF* tmp_buf2= STRBUF_Create( EVWIN_MSG_BUF_SIZE, HEAPID_FIELD );	//�R�s�[�p�o�b�t�@

		MSGMAN_GetString( wk->msgman, msg_id, tmp_buf2 );
		WORDSET_ExpandStr( wk->wordset, wk->msg_buf[wk->list_no], tmp_buf2 );	//�W�J
		wk->list_Data[ wk->list_no ].str = (const void *)wk->msg_buf[wk->list_no];

		STRBUF_Delete( tmp_buf2 );
	}

	if( param == EV_WIN_LIST_LABEL ){
		wk->list_Data[ wk->list_no ].param = BMPLIST_RABEL;
		//*pa = BMPLIST_RABEL;
	}else{
		wk->list_Data[ wk->list_no ].param = param;
		//*pa = param;
	}

	wk->talk_msg_id[ wk->list_no ] = talk_msg_id;
	wk->list_no++;

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	���ڂ̒�����ő啶�������擾
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static u32 BmpList_length_get( SEISEKI_WORK* wk )
{
	int i;
	u32 ret,tmp_ret;

	ret		= 0;
	tmp_ret = 0;

	for( i=0; i < wk->list_no ;i++ ){
		if( wk->list_Data[i].str == NULL ){
			break;
		}

		ret = FontProc_GetPrintStrWidth( EV_WIN_FONT, (STRBUF*)wk->list_Data[i].str, 0 );
		
		if( tmp_ret < ret ){
			tmp_ret = ret;
		}
	}

	return tmp_ret + EV_WIN_CURSOR_WIDTH;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�w�b�_�[�����ݒ�
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_h_default_set(SEISEKI_WORK* wk)
{
	wk->ListH.list		= wk->list_Data;
	wk->ListH.call_back = BmpList_CursorMoveCallBack;
	wk->ListH.icon		= BmpList_CallBack;
	wk->ListH.win		= &wk->bmpwin;

	wk->ListH.count		= wk->list_no;
	wk->ListH.line		= EV_LIST_LINE;

	wk->ListH.rabel_x	= EV_LIST_RABEL_X;
	wk->ListH.data_x	= EV_LIST_DATA_X;
	wk->ListH.cursor_x	= EV_LIST_CURSOR_X;
	wk->ListH.line_y	= EV_LIST_LINE_Y;

	wk->ListH.f_col		= FBMP_COL_BLACK;
	wk->ListH.b_col		= FBMP_COL_WHITE;		
	wk->ListH.s_col		= FBMP_COL_BLK_SDW;

	wk->ListH.msg_spc	= 0;
	wk->ListH.line_spc	= 16;
	wk->ListH.page_skip	= BMPLIST_LRKEY_SKIP;
	wk->ListH.font		= EV_WIN_FONT;
	wk->ListH.c_disp_f	= 0;

	wk->ListH.work		= (void*)wk;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���X�g�V�X�e������̃R�[���o�b�N����s�\������
 *
 * @param	lw			BMPLIST_WORK�^�̃|�C���^
 * @param	param		�I�������^�[���p�����[�^
 * @param	y			
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void	BmpList_CallBack(BMPLIST_WORK* lw,u32 param,u8 y)
{
	if( param == BMPLIST_RABEL ){
		BmpListTmpColorChange( lw, FBMP_COL_RED, FBMP_COL_WHITE, FBMP_COL_RED_SDW );
	}else{
		BmpListTmpColorChange( lw, FBMP_COL_BLACK, FBMP_COL_WHITE, FBMP_COL_BLK_SDW );
	}
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g	�J�[�\���ړ����Ƃ̃R�[���o�b�N
 *
 * @param	
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpList_CursorMoveCallBack( BMPLIST_WORK* wk, u32 param, u8 mode )
{
	u32 count,line;
	u16 list_bak = 0;
	u16 cursor_bak = 0;
	SEISEKI_WORK* evwin_wk = (SEISEKI_WORK*)BmpListParamGet(wk,BMPLIST_ID_WORK);

	BmpListPosGet( wk, &list_bak, &cursor_bak );

	if( (evwin_wk->lp_work != NULL) && (evwin_wk->cp_work != NULL) ){
		*evwin_wk->lp_work = list_bak;
		*evwin_wk->cp_work = cursor_bak;
		OS_Printf( "*wk->lp_work = %d\n", *evwin_wk->lp_work );
		OS_Printf( "*wk->cp_work = %d\n", *evwin_wk->cp_work );
	}

#ifdef EV_WIN_SCROLL_CURSOR	//070228

	//��������
	if( mode == 1 ){
		//
	}

	count = BmpListParamGet( wk, BMPLIST_ID_COUNT );			//���X�g���ڐ�
	line  = BmpListParamGet( wk, BMPLIST_ID_LINE );				//�\���ő區�ڐ�

	//�S�Ă̍��ڂ�\�����Ă��Ȃ���
	if( count > line ){

		if( list_bak == 0 ){
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\����\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\���\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], FALSE );	//OBJ�̔�\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], TRUE );		//OBJ�̕\���ݒ�
		}else if( list_bak == (count-line) ){
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\���\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\����\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], TRUE );		//OBJ�̕\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], FALSE );	//OBJ�̔�\���ݒ�
		}else{
			//OS_Printf( "�E�B���h�E�̏�̃X�N���[���J�[�\���\��\n" );
			//OS_Printf( "�E�B���h�E�̉��̃X�N���[���J�[�\���\��\n" );
			CATS_ObjectEnableCap( evwin_wk->act[0], TRUE );		//OBJ�̕\���ݒ�
			CATS_ObjectEnableCap( evwin_wk->act[1], TRUE );		//OBJ�̕\���ݒ�
		}
	}

#endif

	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g���C��
 *
 * @param	tcb			TCB_PTR
 * @param	wk			���[�N�̃A�h���X
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void EvBmpList_MainTCB( TCB_PTR tcb, void* wk )
{
	u16 tmp_pos_bak;
	u32	ret;
	SEISEKI_WORK* swk;
	swk = (SEISEKI_WORK*)wk;

	//�E�F�C�g
	if( swk->wait != 0 ){
		swk->wait--;
		return;
	}

	//���C�v���͏������Ȃ�
	if( WIPE_SYS_EndCheck() == FALSE ){
		return;
	}

	ret = BmpListMain( swk->lw );

	//BMP���X�g�̑S�̈ʒu���擾
	tmp_pos_bak = swk->pos_bak;		//�ޔ�
	BmpListDirectPosGet( swk->lw, &swk->pos_bak );

	//�J�[�\�������������`�F�b�N
	if( tmp_pos_bak != swk->pos_bak ){
		Snd_SePlay( SEQ_SE_DP_SELECT );
	}

#ifdef EV_WIN_SCROLL_CURSOR	//070228
	CLACT_Draw( swk->fcat.cas );
#endif

	switch( ret ){
	case BMPLIST_NULL:
		break;
	case BMPLIST_CANCEL:
		if( swk->cancel == TRUE ){			//TRUE = B�L�����Z���L��
			Snd_SePlay( SEQ_SE_DP_SELECT );
			*swk->work = EV_WIN_B_CANCEL;	//�I�������l�����[�N�ɑ��
#ifdef EV_WIN_SCROLL_CURSOR	//070228
			EvWin_ActorRelease( wk );		//�폜
#endif
			EvBmpList_Del(wk);
		}
		break;
	default:
		Snd_SePlay( SEQ_SE_DP_SELECT );
		*swk->work = ret;					//�I�������l�����[�N�ɑ��
#ifdef EV_WIN_SCROLL_CURSOR	//070228
		EvWin_ActorRelease( wk );			//�폜
#endif
		EvBmpList_Del(wk);
		break;
	};

	return;
};

//--------------------------------------------------------------
/**
 * @brief	BMP���X�g�@���X�g�I��
 *
 * @param	wk			SEISEKI_WORK�^�̃|�C���^
 *
 * @retval	none
 *
 * SEQ_SE_DP_SELECT��炵�Ă���I
 */
//--------------------------------------------------------------
static void EvBmpList_Del( SEISEKI_WORK* wk )
{
	int i;

	Snd_SePlay(SEQ_SE_DP_SELECT);	//���ӁI

	BmpListExit( wk->lw, NULL, NULL );
	BmpMenuWinClear( wk->ListH.win, WINDOW_TRANS_ON );
	GF_BGL_BmpWinDel( &wk->bmpwin );

	for( i=0; i < EV_WIN_MSG_MAX ;i++ ){
		STRBUF_Delete( wk->msg_buf[i] );
	}

	if( wk->msgman_del_flag == 1 ){
		//WORDSET_Delete( wk->wordset );
		MSGMAN_Delete( wk->msgman );
	}

	TCB_Delete( wk->tcb );
	sys_FreeMemoryEz( wk );
	return;
}


//==============================================================================================
//
//	�X�N���[���J�[�\��
//
//==============================================================================================
#ifdef EV_WIN_SCROLL_CURSOR	//070228

//�A�N�^�[������
static void EvWin_ActorInit( SEISEKI_WORK* wk )
{
	int i;
	s16 x = (wk->x * 8) + (BmpList_length_get(wk) / 2);

	//���\�[�X�o�^�ő吔��`�\����
	TCATS_RESOURCE_NUM_LIST	crnl = { 1, 1, 1, 1 };

	//�o�^�p�\���� �P�̓o�^�p(���W�͌�Őݒ肵����)
	static const TCATS_OBJECT_ADD_PARAM_S ActAddParam_S[] =
	{
		//�����
		{
			0, 0, 0,
			CURSOR_ANMNO, CURSOR_SPRI, CURSOR_PALT, NNS_G2D_VRAM_TYPE_2DMAIN,
			{
				EVWIN_CHR_H_ID_CURSOR, EVWIN_PAL_H_ID,
				EVWIN_CEL_H_ID_CURSOR, EVWIN_ANM_H_ID_CURSOR, 0, 0,
			},
			0, 0
		},

		//������
		{
			0, 0, 0,
			CURSOR_ANMNO, CURSOR_SPRI, CURSOR_PALT, NNS_G2D_VRAM_TYPE_2DMAIN,
			{
				EVWIN_CHR_H_ID_CURSOR, EVWIN_PAL_H_ID,
				EVWIN_CEL_H_ID_CURSOR, EVWIN_ANM_H_ID_CURSOR, 0, 0,
			},
			0, 0
		},
	};
	
	//�t�B�[���h�Z���A�N�^�[������
	FieldCellActSet_S( &wk->fcat, &crnl, EVWIN_ACTMAX, HEAPID_FIELD );
	
	{
		ARCHANDLE* hdl;
		
		hdl = ArchiveDataHandleOpen( ARC_EVWIN_GRA, HEAPID_FIELD );

		//���\�[�X�ݒ�
		FldClact_LoadResPlttArcH(	&wk->fcat, hdl, NARC_evwin_nclr,
									0, ACT_RES_PAL_NUM, NNS_G2D_VRAM_TYPE_2DMAIN, 
									EVWIN_PAL_H_ID );

		FldClact_LoadResCellArcH(	&wk->fcat, hdl, NARC_evwin_ncer,
									0, EVWIN_CEL_H_ID_CURSOR );

		FldClact_LoadResCellAnmArcH(&wk->fcat, hdl, NARC_evwin_nanr,
									0, EVWIN_ANM_H_ID_CURSOR );

		FldClact_LoadResourceCharArcH(	&wk->fcat, hdl, NARC_evwin_ncgr,
										0, NNS_G2D_VRAM_TYPE_2DMAIN, EVWIN_CHR_H_ID_CURSOR );
		
		ArchiveDataHandleClose( hdl );
	}

	//�Z���A�N�^�[�ǉ�(�P���p)
	for( i=0; i < EVWIN_ACTMAX; i++ ){
		wk->act[i] = FieldCellActAdd_S( &wk->fcat, &ActAddParam_S[i] );
		CLACT_SetAnmFlag( wk->act[i]->act, 1 );				//�I�[�g�A�j��
		CATS_ObjectEnableCap( wk->act[i], FALSE );			//OBJ�̔�\���ݒ�
	}

	//���W�̐ݒ�
	//OS_Printf( "x = %d\n", x );
	//OS_Printf( "wk->y = %d\n", (wk->y*8) );
	//OS_Printf( "y = %d\n", (wk->list_no*8) );
	CATS_ObjectPosSetCap( wk->act[0], x, (wk->y*8) );
	CATS_ObjectPosSetCap( wk->act[1], x, (wk->list_no*8) );

	//�I�[�g�A�j���őΉ�����Ȃ��āA���W�𓮂����ăA�j���ł������̂����B�B�ۗ�
	
	//�I���J�[�\���̐F�ύX
	//CATS_ObjectPaletteSetCap(wk->act[ACT_CURSOR],CURSOR_PALF);
	
	return;
}

//�A�N�^�[�폜
static void EvWin_ActorRelease( SEISEKI_WORK* wk )
{
	int i;

	//�A�N�^�[�|�C���^�̍폜
	for( i=0; i < EVWIN_ACTMAX; i++ ){
		if( wk->act[i] != NULL ){
			CATS_ActorPointerDelete_S( wk->act[i] );
		}
	}

	//�Z���A�N�^�[�폜�i�P���p�j
	FieldCellActDelete_S( &wk->fcat );
	return;
}
#endif


//============================================================================================
//
//	�g�b�v�u���[�_�[
//
//============================================================================================
BOOL EvCmdTopBreederPowRndGet( VM_MACHINE * core );

//--------------------------------------------------------------
/**
 * @brief	�|�P�����̃p���[�����l�`�F�b�N
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
//status.gmm�ł́uHP�v�ł͂Ȃ��u������傭�v�ƂȂ��Ă���̂Ń_��(08.05.15)
#if 0

#include "msgdata/msg_status.h"
static const u16 msg_id_tbl[] = {
	STATUS_000,	//������傭
	STATUS_001,	//��������
	STATUS_002, //�ڂ�����
	STATUS_003,	//���΂₳
	STATUS_004,	//�Ƃ�����
	STATUS_005,	//�Ƃ��ڂ�
};

#else

#include "msgdata/msg_d31r0201.h"
static const u16 msg_id_tbl[] = {
	msg_tower_judge_08,		//HP
	msg_tower_judge_08_01,	//��������
	msg_tower_judge_08_02,	//�ڂ�����
	msg_tower_judge_08_05,	//���΂₳
	msg_tower_judge_08_03,	//�Ƃ�����
	msg_tower_judge_08_04,	//�Ƃ��ڂ�
};

#endif

BOOL EvCmdTopBreederPowRndGet( VM_MACHINE * core )
{
	u8 i;
	u32 temp[6];
	POKEMON_PARAM* poke;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 pos				= VMGetWorkValue( core );	//�莝���̉��Ԗڂ��������Ă���
	u16* ret_wk			= VMGetWork( core );		//�p���[�����̍��v
	u16* ret_wk2		= VMGetWork( core );		//�p���[�������ł������p�����[�^
	u16* ret_wk3		= VMGetWork( core );		//�p���[�������ł������p�����[�^�̒l

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(fsys->savedata), pos );

	//�p���[�����擾
	temp[0] = PokeParaGet( poke, ID_PARA_hp_rnd, NULL );
	temp[1] = PokeParaGet( poke, ID_PARA_pow_rnd, NULL );
	temp[2] = PokeParaGet( poke, ID_PARA_def_rnd, NULL );
	temp[3] = PokeParaGet( poke, ID_PARA_agi_rnd, NULL );
	temp[4] = PokeParaGet( poke, ID_PARA_spepow_rnd, NULL );
	temp[5] = PokeParaGet( poke, ID_PARA_spedef_rnd, NULL );

	//�p���[�����̍��v
	*ret_wk = 0;
	for( i=0; i < 6 ;i++ ){
		*ret_wk += temp[i];
	}

	//�p���[�������ł������p�����[�^
	*ret_wk2 = 0;
	*ret_wk3 = temp[0];

	for( i=1; i < 6 ;i++ ){

		if( temp[*ret_wk2] < temp[i] ){

			*ret_wk2 = i;
			*ret_wk3 = temp[i];

		}else if( temp[*ret_wk2] == temp[i] ){

			//�����l�����݂����烉���_���ɂ���
			if( (gf_rand() % 2) == 0 ){
				*ret_wk2 = i;
				*ret_wk3 = temp[i];
			}
		}
	}

	//���b�Z�[�W��ID��Ԃ��悤�ɂ���
	*ret_wk2 = msg_id_tbl[*ret_wk2];
	return 0;
}


//==============================================================================
//
//	���j�I�����[���ɓ���O�Ƀt�H������߂�
//
//==============================================================================
BOOL EvCmdUnionPokeFormReturn( VM_MACHINE * core )
{
	POKEMON_PARAM* poke;
	POKEPARTY *party;
	int party_max, pos, ret;
	int monsno, form_no;
	u32 set_item;
	int item_array[TEMOTI_POKEMAX];
	int hakkin_num		= 0;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16* ret_wk			= VMGetWork( core );
	
	party		= SaveData_GetTemotiPokemon(fsys->savedata);
	party_max	= PokeParty_GetPokeCount(party);
	*ret_wk		= 0;
	
	//�莝���̃n�b�L���_�}�̐����J�E���g
	for(pos = 0; pos < party_max; pos++){
		poke = PokeParty_GetMemberPointer(party, pos);
		item_array[pos] = PokeParaGet(poke, ID_PARA_item, NULL);
		if(item_array[pos] == ITEM_HAKKINDAMA){
			hakkin_num++;
		}
	}

	//�n�b�L���_�}���o�b�O�֖߂�
	if(hakkin_num > 0){

		ret = MyItem_AddItem(	SaveData_GetMyItem(fsys->savedata), 
								ITEM_HAKKINDAMA, hakkin_num, HEAPID_FIELD);

		if(ret == FALSE){
			*ret_wk = 0xff;	//�n�b�L���_�}���莝���ɖ߂��Ȃ�����
			return 0;
		}

		//�o�b�O�ɖ߂����̂Ńn�b�L���_�}���������Ă���|�P�����̃A�C�e���𖳂��ɂ���
		set_item = 0;
		for(pos = 0; pos < party_max; pos++){
			if(item_array[pos] == ITEM_HAKKINDAMA){
				poke = PokeParty_GetMemberPointer(party, pos);
				PokeParaPut(poke, ID_PARA_item, &set_item);
			}
		}
	}
	
	//�M���e�B�i�A�V�F�C�~�A���g���̃t�H������߂�
	for(pos = 0; pos < party_max; pos++){

		poke	= PokeParty_GetMemberPointer(party, pos);
		form_no = PokeParaGet(poke, ID_PARA_form_no, NULL);

		if(form_no > 0){

			monsno = PokeParaGet(poke, ID_PARA_monsno, NULL);

			switch(monsno){
			case MONSNO_KIMAIRAN:	//�M���e�B�i
				//��Ńn�b�L���_�}�͎��グ�Ă���̂ōČv�Z��������
				PokeParaGirathinaFormChange(poke);
				break;
			case MONSNO_PURAZUMA:	//���g��
				PokeParaRotomuFormChange(poke, FORMNO_ROTOMU_NORMAL, 0);
				break;
			case MONSNO_EURISU:		//�V�F�C�~
				PokeParaSheimiFormChange(poke, FORMNO_SHEIMI_NORMAL);
				break;
			}
		}
	}
	
	return 0;
}

//==============================================================================
//
//	��ĉ��ɗa����O�Ƀt�H������߂�(EvCmdUnionPokeFormReturn�̒P�i�o�[�W����)
//
//==============================================================================
BOOL EvCmdSodateyaPokeFormReturn( VM_MACHINE * core )
{
	POKEMON_PARAM* poke;
	POKEPARTY *party;
	int monsno, form_no;
	u32 set_item;
	int item;
	int ret;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 poke_pos		= VMGetWorkValue( core );				//�莝���ʒu
	u16* ret_wk			= VMGetWork( core );
	
	party	= SaveData_GetTemotiPokemon(fsys->savedata);
	poke	= PokeParty_GetMemberPointer(party, poke_pos);
	*ret_wk = 0;
	
	//���X�g���L�����Z�������l�������Ă��鎞�͉������Ȃ�
	if( poke_pos == 0xff ){
		return 0;
	}

	//�n�b�L���_�}�m�F
	item = PokeParaGet(poke, ID_PARA_item, NULL);

	if(item == ITEM_HAKKINDAMA){

		ret = MyItem_AddItem(	SaveData_GetMyItem(fsys->savedata), 
								ITEM_HAKKINDAMA, 1, HEAPID_FIELD);

		if(ret == FALSE){
			*ret_wk = 0xff;	//�n�b�L���_�}���莝���ɖ߂��Ȃ�����
			return 0;
		}

		//�o�b�O�ɖ߂����̂Ńn�b�L���_�}���������Ă���|�P�����̃A�C�e���𖳂��ɂ���
		set_item = 0;
		PokeParaPut(poke, ID_PARA_item, &set_item);
	}
	
	//�M���e�B�i�A�V�F�C�~�A���g���̃t�H������߂�
	form_no = PokeParaGet(poke, ID_PARA_form_no, NULL);

	if(form_no > 0){

		monsno = PokeParaGet(poke, ID_PARA_monsno, NULL);

		switch(monsno){
		case MONSNO_KIMAIRAN:	//�M���e�B�i
			//��Ńn�b�L���_�}�͎��グ�Ă���̂ōČv�Z��������
			PokeParaGirathinaFormChange(poke);
			break;
		case MONSNO_PURAZUMA:	//���g��
			PokeParaRotomuFormChange(poke, FORMNO_ROTOMU_NORMAL, 0);
			break;
		case MONSNO_EURISU:		//�V�F�C�~
			PokeParaSheimiFormChange(poke, FORMNO_SHEIMI_NORMAL);
			break;
		}
	}

	return 0;
}

//============================================================================================
//
//	�t�H�����`�F���W
//
//============================================================================================
BOOL EvCmdPokeFormChange( VM_MACHINE * core );

//�t�H��������������Ή����Ȃ��ƃ_���I
#include "battle/battle_server.h"
#include "battle/wazano_def.h"
#include "ev_pokemon.h"
static const u8 pura_form_tbl[FORMNO_ROTOMU_MAX] = {
	//FORMNO_ROTOMU_NORMAL,				//�m�[�}���t�H����
	FORMNO_ROTOMU_HOT,					//�d�q�����W�t�H����
	FORMNO_ROTOMU_WASH,					//����@�t�H����
	FORMNO_ROTOMU_COLD,					//�①�Ƀt�H����
	FORMNO_ROTOMU_FAN,					//��@�t�H����
	FORMNO_ROTOMU_CUT,					//������@�t�H����
};

static const u16 pura_waza_tbl[FORMNO_ROTOMU_MAX] = {
	//WAZANO_DENKISYOKKU,
	WAZANO_OOBAAHIITO,
	WAZANO_HAIDOROPONPU,
	WAZANO_HUBUKI,
	WAZANO_EASURASSYU,
	WAZANO_RIIHUSUTOOMU,
};

//--------------------------------------------------------------
/**
 * @brief	�|�P�����̃t�H�����`�F���W
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdPokeFormChange( VM_MACHINE * core )
{
	POKEMON_PARAM* poke;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 pos				= VMGetWorkValue( core );	//�莝���̉��Ԗڂ��������Ă���
	u16 no				= VMGetWorkValue( core );	//form�ǂꂩ

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(fsys->savedata), pos );
	PokeParaPut( poke, ID_PARA_form_no, &no );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	���g���̃t�H�����`�F���W���莝���ɉ��C���邩�擾�A�ŏ��Ɍ������莝���ʒu���擾
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdTemotiRotomuFormChangeGet( VM_MACHINE * core )
{
	u32 monsno,form_no,tamago_flag;
	int temoti_max,i,count;
	POKEMON_PARAM* pp;
	POKEPARTY* party;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16* ret_wk			= VMGetWork( core );
	u16* ret_wk2		= VMGetWork( core );

	count = 0;
	*ret_wk2 = 0xff;

	party = SaveData_GetTemotiPokemon( fsys->savedata );
	temoti_max = PokeParty_GetPokeCount( party );

	//�^�}�S�łȂ��A�m�[�}���t�H�����łȂ��A���g���̐����擾
	for( i=0; i < temoti_max; i++ ){

		pp = PokeParty_GetMemberPointer( party, i );

		monsno = PokeParaGet( pp, ID_PARA_monsno, NULL );
		form_no= PokeParaGet( pp, ID_PARA_form_no, NULL );
		tamago_flag = PokeParaGet( pp, ID_PARA_tamago_flag, NULL );
		OS_Printf( "monsno = %d\n", monsno );
		OS_Printf( "form_no = %d\n", form_no );
		OS_Printf( "tamago_flag = %d\n", tamago_flag );

		if( (monsno == MONSNO_PURAZUMA) && (form_no != FORMNO_ROTOMU_NORMAL) && (tamago_flag == 0)){

			//�ŏ��Ɍ������莝���ʒu��ۑ�
			if( *ret_wk2 == 0xff ){
				*ret_wk2 = i;
			}

			count++;
		}
	}

	*ret_wk = count;
	OS_Printf( "�� *ret_wk = %d\n", *ret_wk );
	OS_Printf( "�ʒu *ret_wk2 = %d\n", *ret_wk2 );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	���g���̃t�H������ς������̋Z��������
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdRotomuFormWazaChange( VM_MACHINE * core )
{
	u32 now_form_no,i;
	u16 del_waza_no,waza_count;
	POKEMON_PARAM* poke;
	POKEPARTY* party;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 poke_pos	= VMGetWorkValue( core );				//�莝���ʒu
	u16 waza_pos	= VMGetWorkValue( core );				//�Z�̈ʒu
	u16 waza		= VMGetWorkValue( core );				//�o����Z
	u16 next_form	= VMGetWorkValue( core );				//���̃t�H����

	party = SaveData_GetTemotiPokemon( fsys->savedata );
	poke = PokeParty_GetMemberPointer( party, poke_pos );

#if 1	//2008.03.27(��)�@matsuda
	PokeParaRotomuFormChange(poke, next_form, waza_pos);
	ZukanWork_SetPokeGet(SaveData_GetZukanWork(fsys->savedata), poke);//�}�ӓo�^
#else

	now_form_no = PokeParaGet( poke, ID_PARA_form_no, NULL );

	///////////////////////////////////////
	//�m�[�}������Ⴄ�t�H�����ɕύX���鎞
	///////////////////////////////////////
	if( now_form_no == FORMNO_ROTOMU_NORMAL ){

			EvPoke_ChangeWaza(	SaveData_GetTemotiPokemon(core->fsys->savedata), poke_pos, 
								waza_pos, waza );

	}else{
		for( i=0; i < FORMNO_ROTOMU_MAX ;i++ ){
			if( now_form_no == pura_form_tbl[i] ){
				del_waza_no = pura_waza_tbl[i];				//���̃t�H�����̐�p�Z���擾
				break;
			}
		}

		/////////////////////////////////
		//�m�[�}���ɖ߂���
		/////////////////////////////////
		if( next_form == FORMNO_ROTOMU_NORMAL ){

			//�w�肳�ꂽ�Z(��p�Z)�������Ă�����Y�ꂳ����
			for( i=0; i < 4 ;i++ ){
				if( PokeParaGet(poke,ID_PARA_waza1+i,NULL) == del_waza_no ){
					PokeParaWazaDelPos( poke, i );
					break;
				}
			}

			//�Z�̐���0�ɂȂ��Ă�����u�ł񂫃V���b�N�v���o�������Ă���
			waza_count = 0;
			for( i=0; i < 4 ;i++ ){
				if( PokeParaGet(poke,ID_PARA_waza1+i,NULL) != 0 ){
					waza_count++;
				}
			}
	
			if( waza_count == 0 ){
				EvPoke_ChangeWaza(	SaveData_GetTemotiPokemon(core->fsys->savedata), poke_pos, 
									0, WAZANO_DENKISYOKKU );
			}

		/////////////////////////////////
		//����ɈႤ�t�H�����ɕύX���鎞
		/////////////////////////////////
		}else{

			//�w�肳�ꂽ�Z(��p�Z)�������Ă�����A���̃t�H�����̋Z�ɒu��������
			for( i=0; i < 4 ;i++ ){
				if( PokeParaGet(poke,ID_PARA_waza1+i,NULL) == del_waza_no ){
					EvPoke_ChangeWaza(	SaveData_GetTemotiPokemon(core->fsys->savedata), poke_pos, 
										i, waza );
					break;
				}
			}

			//�w�肳�ꂽ�Z�������Ă��Ȃ�������A�v���C���[���I�������Z�ʒu������������
			if( i == 4 ){
				EvPoke_ChangeWaza(	SaveData_GetTemotiPokemon(core->fsys->savedata), poke_pos, 
									waza_pos, waza );
			}
		}
	}
#endif

	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�߂��߂�p���[�̃^�C�v���擾
 *
 * @param	none
 *
 * @retval	0
 *
 * �|�P�����i���o�[�����āA�o�����Ȃ��|�P�������ʂ����Ă���̂Œ��ӁI
 */
//--------------------------------------------------------------
BOOL EvCmdMezameruPawaaTypeGet( VM_MACHINE * core )
{
	u16 monsno;
	int power,type;
	POKEMON_PARAM* poke;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 pos				= VMGetWorkValue( core );	//�莝���̉��Ԗڂ��������Ă���
	u16*ret_wk			= VMGetWork( core );	//

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(fsys->savedata), pos );

	monsno = PokeParaGet( poke, ID_PARA_monsno, NULL );

	if( PokeParaGet(poke,ID_PARA_tamago_flag,NULL) == 0 ){
		switch( monsno ){
		case MONSNO_KYATAPII:
		case MONSNO_TORANSERU:
		case MONSNO_BIIDORU:
		case MONSNO_KOKUUN:
		case MONSNO_KOIKINGU:
		case MONSNO_METAMON:
		case MONSNO_SOONANSU:
		case MONSNO_DOOBURU:
		case MONSNO_KEMUSSO:
		case MONSNO_KARASARISU:
		case MONSNO_MAYURUDO:
		case MONSNO_SOONANO:
		case MONSNO_DANBARU:
		case MONSNO_MINOMUTTI:
		case MONSNO_HEKISAGON:		//�~�c�n�j�[
		case MONSNO_KOBOSI:			//�R���{�[�V
			*ret_wk = 0xffff;		//�Ȃ��R�[�h������Ă���
			return 0;
		};
	}

	//�߂��߂�p���[�̃^�C�v���擾
	GetMezameruPowerParam( poke, &power, &type );
	*ret_wk = type;
	//*ret_wk2 = power;
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�߂��߂�p���[�̈З͂ƃ^�C�v���擾
 *
 * @param[in]	pp		�擾����PokemonParam�\���̂ւ̃|�C���^
 * @param[out]	power	�З͂��i�[���郏�[�N�ւ̃|�C���^�iNULL�w��Ŏ擾�Ȃ��j
 * @param[out]	type	�З͂��i�[���郏�[�N�ւ̃|�C���^�iNULL�w��Ŏ擾�Ȃ��j
 */
//--------------------------------------------------------------
static void	GetMezameruPowerParam(POKEMON_PARAM *pp,int *power,int *type)
{
	int	hp_rnd;
	int	pow_rnd;
	int	def_rnd;
	int	agi_rnd;
	int	spepow_rnd;
	int	spedef_rnd;

	hp_rnd		=PokeParaGet(pp,ID_PARA_hp_rnd,		NULL);
	pow_rnd		=PokeParaGet(pp,ID_PARA_pow_rnd,	NULL);
	def_rnd		=PokeParaGet(pp,ID_PARA_def_rnd,	NULL);
	agi_rnd		=PokeParaGet(pp,ID_PARA_agi_rnd,	NULL);
	spepow_rnd	=PokeParaGet(pp,ID_PARA_spepow_rnd,	NULL);
	spedef_rnd	=PokeParaGet(pp,ID_PARA_spedef_rnd,	NULL);

	if(power!=NULL){
		*power=	((hp_rnd&2)		>>1)|
				((pow_rnd&2)	>>0)|
				((def_rnd&2)	<<1)|
				((agi_rnd&2)	<<2)|
				((spepow_rnd&2)	<<3)|
				((spedef_rnd&2)	<<4);
		*power=((*power)*40/63)+30;
	}
	if(type!=NULL){
		*type=	((hp_rnd&1)		>>0)|
				((pow_rnd&1)	<<1)|
				((def_rnd&1)	<<2)|
				((agi_rnd&1)	<<3)|
				((spepow_rnd&1)	<<4)|
				((spedef_rnd&1)	<<5);
		*type=((*type)*15/63)+1;

		if(*type>=HATE_TYPE){
			//*type++;
			*type=(*type)+1;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief	�t�H�����`�F���W�������g�������邩�`�F�b�N(�S�ă`�F�b�N)
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
#if 0
static u32 MyRotomuFormCheck(SAVEDATA *sv);

#include "battle/battle_server.h"
#if 0
// ���g���̃t�H������`(BIT�w��)
#define FORMNO_ROTOMU_BIT_NORMAL		(1 << FORMNO_ROTOMU_NORMAL)
#define FORMNO_ROTOMU_BIT_HOT			(1 << FORMNO_ROTOMU_HOT)
#define FORMNO_ROTOMU_BIT_WASH			(1 << FORMNO_ROTOMU_WASH)
#define FORMNO_ROTOMU_BIT_COLD			(1 << FORMNO_ROTOMU_COLD)
#define FORMNO_ROTOMU_BIT_FAN			(1 << FORMNO_ROTOMU_FAN)
#define FORMNO_ROTOMU_BIT_CUT			(1 << FORMNO_ROTOMU_CUT)
#endif
BOOL EvCmdRotomuFormCheck( VM_MACHINE * core )
{
	u32 rotom_bit;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16*hot_wk			= VMGetWork( core );	//
	u16*wash_wk			= VMGetWork( core );	//
	u16*cold_wk			= VMGetWork( core );	//
	u16*fan_wk			= VMGetWork( core );	//
	u16*cut_wk			= VMGetWork( core );	//

	*hot_wk	= 0;		//
	*wash_wk= 0;		//
	*cold_wk= 0;		//
	*fan_wk	= 0;		//
	*cut_wk	= 0;		//

	rotom_bit = MyRotomuFormCheck( fsys->savedata );

	if( ((rotom_bit >> FORMNO_ROTOMU_BIT_HOT) & 0x01) == 1 ){
		*hot_wk = 1;
	}

	if( ((rotom_bit >> FORMNO_ROTOMU_BIT_WASH) & 0x01) == 1 ){
		*wash_wk = 1;
	}

	if( ((rotom_bit >> FORMNO_ROTOMU_BIT_COLD) & 0x01) == 1 ){
		*cold_wk = 1;
	}

	if( ((rotom_bit >> FORMNO_ROTOMU_BIT_FAN) & 0x01) == 1 ){
		*fan_wk = 1;
	}

	if( ((rotom_bit >> FORMNO_ROTOMU_BIT_CUT) & 0x01) == 1 ){
		*cut_wk = 1;
	}

	OS_Printf( "*hot_wk = %d\n", *hot_wk );
	OS_Printf( "*wash_wk = %d\n", *wash_wk );
	OS_Printf( "*cold_wk = %d\n", *cold_wk );
	OS_Printf( "*fan_wk = %d\n", *fan_wk );
	OS_Printf( "*cut_wk = %d\n", *cut_wk );
	return 1;
}

//--------------------------------------------------------------
/**
 * @brief   �����̎莝���A�{�b�N�X�A��ĉ��A����t�H�����`�F���W�������g�������邩�擾
 *
 * @param   sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @retval  FORMNO_ROTOMU_BIT_??? �� OR�̑��a�ŕԂ��Ă��܂�
 * 			���m�[�}���t�H�����ł��r�b�g�������Ƃɒ���
 */
//--------------------------------------------------------------
static u32 MyRotomuFormCheck(SAVEDATA *sv)
{
	int i;
	POKEMON_PARAM *pp;
	POKEMON_PASO_PARAM *ppp;
	u32 form_bit;
	
	form_bit = 0;
	
	//�莝���`�F�b�N
	{
		POKEPARTY *party;
		int temoti_max;
		
		party = SaveData_GetTemotiPokemon(sv);
		temoti_max = PokeParty_GetPokeCount(party);
		for(i = 0; i < temoti_max; i++){
			pp = PokeParty_GetMemberPointer(party, i);
			if(PokeParaGet(pp, ID_PARA_monsno, NULL) == MONSNO_PURAZUMA
					&& PokeParaGet(pp, ID_PARA_tamago_flag, NULL) == 0){
				form_bit |= 1 << PokeParaGet(pp, ID_PARA_form_no, NULL);
			}
		}
	}
	
	//��ĉ��`�F�b�N
	{
		SODATEYA_WORK *sodateya;
		SODATEYA_POKE *s_poke;
		
		sodateya = SaveData_GetSodateyaWork(sv);
		for(i = 0; i < SODATEYA_POKE_MAX; i++){
			s_poke = SodateyaWork_GetSodateyaPokePointer(sodateya, i);
			ppp = SodateyaPoke_GetPokePasoPointer(s_poke);
			if(PokePasoParaGet(ppp,ID_PARA_monsno,NULL) == MONSNO_PURAZUMA
					&& PokePasoParaGet(ppp, ID_PARA_tamago_flag, NULL) == 0){
				form_bit |= 1 << PokePasoParaGet(ppp, ID_PARA_form_no, NULL);
			}
		}
	}
	
	//�{�b�N�X�`�F�b�N
	{
		BOX_DATA *boxdata;
		u32 tray;
		
		boxdata = SaveData_GetBoxData(sv);
		for(tray = 0; tray < BOX_MAX_TRAY; tray++){
			for(i = 0; i < BOX_MAX_POS; i++){
				ppp = BOXDAT_GetPokeDataAddress(boxdata, tray, i);
				if(PokePasoParaGet(ppp, ID_PARA_monsno, NULL) == MONSNO_PURAZUMA
						&& PokePasoParaGet(ppp, ID_PARA_tamago_flag, NULL) == 0){
					form_bit |= 1 << PokePasoParaGet(ppp, ID_PARA_form_no, NULL);
				}
			}
		}
	}
	
	return form_bit;
}
#endif

//============================================================================================
//
//	���C�ɓ���|�P����
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���C�ɓ���|�P�����̃Z�b�g
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdSetFavoritePoke( VM_MACHINE * core )
{
	MISC* misc;
	POKEMON_PARAM* poke;
	FIELDSYS_WORK* fsys	= core->fsys;

	//�擪�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(fsys->savedata), 0 );

	misc = SaveData_GetMisc( fsys->savedata );
	MISC_SetFavoriteMonsno( misc, 
							PokeParaGet(poke,ID_PARA_monsno,NULL),
							PokeParaGet(poke,ID_PARA_form_no,NULL),
							PokeParaGet(poke,ID_PARA_tamago_flag,NULL) );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	���C�ɓ���|�P�����̎擾
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdGetFavoritePoke( VM_MACHINE * core )
{
	int mons,form,egg;
	MISC* misc;
	POKEMON_PARAM* poke;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16* mons_wk		= VMGetWork( core );		//
	u16* form_wk		= VMGetWork( core );		//
	u16* egg_wk			= VMGetWork( core );		//

	misc = SaveData_GetMisc( fsys->savedata );
	MISC_GetFavoriteMonsno( misc, &mons, &form, &egg );

	*mons_wk = mons;
	*form_wk = form;
	*egg_wk	 = egg;
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�w�肵���ʒu�̃t�H�����i���o�[��������(���g���p)
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdTemotiRotomuFormNoGet( VM_MACHINE * core )
{
	POKEMON_PARAM* pp;
	POKEPARTY* party;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 pos				= VMGetWorkValue( core );
	u16* ret_wk			= VMGetWork( core );

	party = SaveData_GetTemotiPokemon( fsys->savedata );
	pp = PokeParty_GetMemberPointer( party, pos );

	*ret_wk = PokeParaGet( pp, ID_PARA_form_no, NULL );
	OS_Printf( "pos = %d\n", pos );
	OS_Printf( "*ret_wk = %d\n", *ret_wk );
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief	�ʑ��̃��X�g�\���\���`�F�b�N
 *
 * @param	none
 *
 * @retval	0
 */
//--------------------------------------------------------------
BOOL EvCmdVillaListCheck( VM_MACHINE * core )
{
	EVENTWORK* ev;
	RECORD* rec;
	FIELDSYS_WORK* fsys	= core->fsys;
	u16 no				= VMGetWorkValue( core );
	u16* ret_wk			= VMGetWork( core );

	ev		= SaveData_GetEventWork( fsys->savedata );
	rec		= SaveData_GetRecord( fsys->savedata );
	*ret_wk = 1;		//OK

	switch( no ){

	//�|�P��������1
	case 13:
		//�t�����e�B�A�S�{��1���ȏサ�Ă���H
		if( RECORD_Get(rec,RECID_BTOWER_WIN) < 1 ){
			*ret_wk = 0;
		}

		if( RECORD_Get(rec,RECID_FACTORY_WIN) < 1 ){
			*ret_wk = 0;
		}

		if( RECORD_Get(rec,RECID_CASTLE_WIN) < 1 ){
			*ret_wk = 0;
		}

		if( RECORD_Get(rec,RECID_STAGE_WIN) < 1 ){
			*ret_wk = 0;
		}

		if( RECORD_Get(rec,RECID_ROULETTE) < 1 ){
			*ret_wk = 0;
		}
		break;

	//�|�P��������2
	case 14:
		*ret_wk = 0;		//NG

		//��V���{�����ǂꂩ1�R�擾���Ă���H
		if( SysWork_MemoryPrintFactory(ev) >= MEMORY_PRINT_PUT_OK_1ST ){
			*ret_wk = 1;
		}

		if( SysWork_MemoryPrintStage(ev) >= MEMORY_PRINT_PUT_OK_1ST ){
			*ret_wk = 1;
		}

		if( SysWork_MemoryPrintCastle(ev) >= MEMORY_PRINT_PUT_OK_1ST ){
			*ret_wk = 1;
		}

		if( SysWork_MemoryPrintRoulette(ev) >= MEMORY_PRINT_PUT_OK_1ST ){
			*ret_wk = 1;
		}

		if( SysWork_MemoryPrintTower(ev) >= MEMORY_PRINT_PUT_OK_1ST ){
			*ret_wk = 1;
		}
		break;

	//�s�A�m
	case 15:
		//�a������J�E���g10�ȏ�H
		if( RECORD_Get(rec,RECID_DENDOU_CNT) < 10 ){
			*ret_wk = 0;
		}
		break;

	//�������Z�b�g
	case 16:
		//�Đ�{�݂Ő퓬������50�ȏ�H
		if( RECORD_Get(rec,RECID_LEADERHOUSE_BATTLE) < 50 ){
			*ret_wk = 0;
		}
		break;

	//���ׂǂ���
	case 17:
		//�؂̎���A������50�ȏ�H
		if( RECORD_Get(rec,RECID_NUTS_PLANT) < 50 ){
			*ret_wk = 0;
		}
		break;

	//���΂炵���߂���
	case 18:
		//�^�}�S�z��������30�ȏ�H
		if( RECORD_Get(rec,RECID_TAMAGO_HATCHING) < 30 ){
			*ret_wk = 0;
		}
		break;

	//�V�����f���A
	case 20:
		//�����J�E���^�[300000�ȏ�H
		if( RECORD_Get(rec,RECID_WALK_COUNT) < 300000 ){
			*ret_wk = 0;
		}
		break;

	};

	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�h���X�A�b�v���M�ς݃t���O�����Z�b�g����
 *
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 *	@return "1"
 */
//-----------------------------------------------------------------------------
BOOL EvCmdImageClipTvSendFlagReset( VM_MACHINE * core )
{
#if 0	//�t���O���̖����Ȃ���2008.06.21(�y) matsuda
	FIELDSYS_WORK* fsys	= core->fsys;
	MISC * misc;
	
	misc = SaveData_GetMisc(fsys->savedata);
	MISC_SetGdsSend_Dress(misc, 0);
#endif
	return 1;
}

typedef struct{
	s16 x;
	s16 z;
}POS;

//���ވʒu
static const POS rezi_d35r0102_pos[] = {
	{ 4,	7 },	//
	{ 5,	5 },	//
	{ 5,	9 },	//
	{ 7,	7 },	//
	{ 9,	5 },	//
	{ 9,	9 },	//
	{ 10,	7 },	//
};
#define REZI_D35R0102_POS_MAX	( NELEMS(rezi_d35r0102_pos) )

static const POS rezi_d35r0104_pos[] = {
	{ 3,	7 },
	{ 5,	7 },
	{ 7,	5 },
	{ 7,	7 },
	{ 7,	9 },
	{ 9,	7 },
	{ 11,	7 },
};
#define REZI_D35R0104_POS_MAX	( NELEMS(rezi_d35r0104_pos) )

static const POS rezi_d35r0106_pos[] = {
	{ 5,	5 },
	{ 5,	7 },
	{ 5,	9 },
	{ 7,	7 },
	{ 9,	5 },
	{ 9,	7 },
	{ 9,	9 },
};
#define REZI_D35R0106_POS_MAX	( NELEMS(rezi_d35r0106_pos) )

//----------------------------------------------------------------------------
/**
 *	@brief	���W�}�b�v�̏��`�F�b�N
 *
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 *	@return "0"
 */
//-----------------------------------------------------------------------------
BOOL EvCmdD35FloorSet( VM_MACHINE * core )
{
	int i,all_num;
	u16* wk		= VMGetWork( core );
	u16 zone_id = VMGetWorkValue( core );
	u16 x		= VMGetWorkValue( core );
	u16 z		= VMGetWorkValue( core );
	
	switch( zone_id ){

	case ZONE_ID_D35R0102:
		all_num = REZI_WORK_2_ALL_NUM;
		for( i=0; i < REZI_D35R0102_POS_MAX ;i++ ){
			if( (x == rezi_d35r0102_pos[i].x) && (z == rezi_d35r0102_pos[i].z) ){
				Snd_SePlay( SEQ_SE_PL_JUMP2 );
				*wk |= (1 << i);
				break;
			}
		}
		break;

	case ZONE_ID_D35R0104:
		all_num = REZI_WORK_4_ALL_NUM;
		for( i=0; i < REZI_D35R0104_POS_MAX ;i++ ){
			if( (x == rezi_d35r0104_pos[i].x) && (z == rezi_d35r0104_pos[i].z) ){
				Snd_SePlay( SEQ_SE_PL_JUMP2 );
				*wk |= (1 << i);
				break;
			}
		}
		break;

	case ZONE_ID_D35R0106:
		all_num = REZI_WORK_6_ALL_NUM;
		for( i=0; i < REZI_D35R0106_POS_MAX ;i++ ){
			if( (x == rezi_d35r0106_pos[i].x) && (z == rezi_d35r0106_pos[i].z) ){
				Snd_SePlay( SEQ_SE_PL_JUMP2 );
				*wk |= (1 << i);
				break;
			}
		}
		break;
	};

	//�S�Ă̏��𓥂�
	if( *wk == all_num ){
		*wk = REZI_WORK_POS_OK;
	}

	OS_Printf( "i = %d\n", i );
	OS_Printf( "*wk = %d\n", *wk );
	return 0;
}


//============================================================================================
//	�t�B�[���hOBJ
//============================================================================================
//--------------------------------------------------------------
/**
 *  @brief �t�B�[���hOBJ�@OBJ�ɍ����擾���s�ł��\������l�ɂ���B
 *  ���łɍ��������@�ɍ��킹��B���@�����͂��̂܂܁B
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *	@return "0"
 */
//--------------------------------------------------------------
BOOL EvCmdFldOBJAllHeightVanishOFF( VM_MACHINE * core )
{
	fx32 y;
	VecFx32 pos;
	BLACT_WORK_PTR blact;
	
	int objno = 0;
	FIELDSYS_WORK *fsys = core->fsys;
	FIELD_OBJ_SYS_PTR fos = fsys->fldobjsys;
	FIELD_OBJ_PTR jiki = Player_FieldOBJGet( fsys->player );
	FIELD_OBJ_PTR fldobj;
	
	FieldOBJ_VecPosGet( jiki, &pos );
	y = pos.y;
	
	while( FieldOBJSys_FieldOBJSearch(
		fos,&fldobj,&objno,FLDOBJ_STA_BIT_USE) == TRUE ){
		if( fldobj != jiki ){
			FieldOBJ_StatusBit_ON(
				fldobj, FLDOBJ_STA_BIT_HEIGHT_VANISH_OFF );
			
			if( FieldOBJ_StatusBit_CheckEasy(
				fldobj,FLDOBJ_STA_BIT_HEIGHT_GET_ERROR) == TRUE ){
				FieldOBJ_VecPosGet( fldobj, &pos );
				pos.y = y;
				FieldOBJ_VecPosSet( fldobj, &pos );
				FieldOBJ_NowPosGY_Set( fldobj, SIZE_H_GRID_FX32(y) );
			}
			
			blact = FieldOBJ_DrawBlAct00_BlActPtrGet( fldobj );
			
			if( blact == NULL &&
				FieldOBJ_OBJCodeSeedCheck(FieldOBJ_OBJCodeGet(fldobj)) ){
				if( FieldOBJ_StatusBitCheck_DrawProcInitComp(fldobj) ){
					FieldOBJ_DrawProcCall( fldobj );
					blact = FieldOBJ_SeedBlActPtrGet( fldobj );
				}
			}
			
			if( blact != NULL ){
				FieldOBJ_BlActPosSet( fldobj, blact );
				BLACT_ObjDrawFlagSet( blact, TRUE );
			}
		}
	}
	
	return 0;
}

//--------------------------------------------------------------
/**
 *  @brief �t�B�[���hOBJ
 *  EvCmdFldOBJAllHeightVanishOFF()�ŃZ�b�g�����t���O��߂��B
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *	@return "0"
 */
//--------------------------------------------------------------
BOOL EvCmdFldOBJAllHeightVanishON( VM_MACHINE * core )
{
	int objno = 0;
	FIELDSYS_WORK *fsys = core->fsys;
	FIELD_OBJ_SYS_PTR fos = fsys->fldobjsys;
	FIELD_OBJ_PTR jiki = Player_FieldOBJGet( fsys->player );
	FIELD_OBJ_PTR fldobj;
	
	while( FieldOBJSys_FieldOBJSearch(
		fos,&fldobj,&objno,FLDOBJ_STA_BIT_USE) == TRUE ){
		if( fldobj != jiki ){
			FieldOBJ_StatusBit_OFF(
				fldobj, FLDOBJ_STA_BIT_HEIGHT_VANISH_OFF );
		}
	}
	
	return 0;
}

//--------------------------------------------------------------
/**
 *  @brief EvCmdFldOBJForceDraw_C04EventOnly()�R�A
 */
//--------------------------------------------------------------
static void evcmd_FldOBJForceDrawC04OnlyCore( FIELD_OBJ_PTR fldobj, int grid )
{
	VecFx32 pos;
	BLACT_WORK_PTR blact;
	
	FieldOBJ_StatusBit_ON( fldobj, FLDOBJ_STA_BIT_HEIGHT_VANISH_OFF );
	
	FieldOBJ_VecPosGet( fldobj, &pos );
	pos.y = GRID_SIZE_FX32( grid );
	FieldOBJ_VecPosSet( fldobj, &pos );
	FieldOBJ_NowPosGY_Set( fldobj, G_GRID_H_GRID(grid) );
	
	blact = FieldOBJ_DrawBlAct00_BlActPtrGet( fldobj );
	
	if( blact != NULL ){
		FieldOBJ_BlActPosSet( fldobj, blact );
		BLACT_ObjDrawFlagSet( blact, TRUE );
	}
}

//--------------------------------------------------------------
/**
 *  @brief C04���� �t�B�[���hOBJ�AGINGABOSS,BABYBOY1�������\��
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *	@return "0"
 */
//--------------------------------------------------------------
BOOL EvCmdFldOBJForceDraw_C04EventOnly( VM_MACHINE * core )
{
	FIELD_OBJ_PTR fldobj;
	FIELDSYS_WORK *fsys = core->fsys;
	FIELD_OBJ_SYS_PTR fos = fsys->fldobjsys;
	
	//�A�J�M
	fldobj = FieldOBJSys_OBJIDSearch( fos, C04_AKAGI );
	
	if( fldobj != NULL ){
		evcmd_FldOBJForceDrawC04OnlyCore( fldobj, 9 );
	}
	
	//�q��
	fldobj = FieldOBJSys_OBJIDSearch( fos, C04_BABYBOY1 );
	
	if( fldobj != NULL ){
		evcmd_FldOBJForceDrawC04OnlyCore( fldobj, 9 );
	}
	
	return 0;
}

//--------------------------------------------------------------
/**
 *  @brief C04���� EvCmdFldOBJForceDraw_C04EventOnly()�̌�n��
 *	@param	core		���z�}�V������\���̂ւ̃|�C���^
 *	@return "0"
 */
//--------------------------------------------------------------
BOOL EvCmdFldOBJForceDraw_C04EventAfterOnly( VM_MACHINE * core )
{
	FIELD_OBJ_PTR fldobj;
	FIELD_OBJ_SYS_PTR fos = core->fsys->fldobjsys;
	
	//�A�J�M
	fldobj = FieldOBJSys_OBJIDSearch( fos, C04_AKAGI );
	
	if( fldobj != NULL ){
		FieldOBJ_StatusBit_OFF( fldobj, FLDOBJ_STA_BIT_HEIGHT_VANISH_OFF );
	}
	
	//�q��
	fldobj = FieldOBJSys_OBJIDSearch( fos, C04_BABYBOY1 );
	
	if( fldobj != NULL ){
		FieldOBJ_StatusBit_OFF( fldobj, FLDOBJ_STA_BIT_HEIGHT_VANISH_OFF );
	}
	
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief   �n���V�o�^
 *
 * @param   SAVEDATA	savedata
 *
 * @return "1"
 */
//--------------------------------------------------------------
BOOL EvCmdWifiHistory( VM_MACHINE * core )
{
	WIFI_HISTORY* pHistory = SaveData_GetWifiHistory( core->fsys->savedata );
	Comm_WifiHistoryCheck( pHistory );
	return 1;
}

//--------------------------------------------------------------
/**
 * @brief   �v���C���[�̉��ʂ𑀍�
 *
 * @param   SAVEDATA	savedata
 *
 * @return "0"
 */
//--------------------------------------------------------------
BOOL EvCmdSndPlayerVolume( VM_MACHINE * core )
{
	u16 volume = VMGetWorkValue( core );

	Snd_PlayerSetPlayerVolume( PLAYER_FIELD, volume );
	return 0;
}

#if PL_S0802_080716_FIX
//--------------------------------------------------------------------------------------------
/**
 * @brief		�X�N���v�g�R�}���h�F�}�ӌ������Ƃ��邩�擾
 * @param		core
 * @return		"0"
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdGetPokeSeeFlag( VM_MACHINE * core )
{
	u16 monsno	= VMGetWorkValue(core);
	u16* ret_wk	= VMGetWork( core );
	const ZUKAN_WORK* zw = SaveData_GetZukanWork( core->fsys->savedata );
	
	*ret_wk = ZukanWork_GetPokeSeeFlag( zw, monsno );
	return 0;
}
#endif

