//==============================================================================================
/**
 * @file	d_nohara.c
 * @brief	�f�o�b�N�\�[�X
 * @author	Satoshi Nohara
 * @date	2005.07.26
 */
//==============================================================================================
#include "common.h"
#include "system/lib_pack.h"
#include "system/bmp_list.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/get_text.h"

#include "system/snd_tool.h"
#include "system/snd_perap.h"
#include "system/wipe.h"

#include "system/savedata.h"
#include "savedata/mystatus.h"
#include "savedata/zukanwork.h"
#include "savedata\battle_rec.h"
#include "savedata\frontier_savedata.h"
#include "savedata\frontier_ex.h"
#include "savedata\factory_savedata.h"
#include "savedata\stage_savedata.h"
#include "savedata\castle_savedata.h"
#include "savedata\roulette_savedata.h"

#include "fld_bgm.h"
#include "poketool/monsno.h"
#include "fieldmap.h"
#include "fieldsys.h"
#include "field_subproc.h"
#include "field_encount.h"
#include "field_battle.h"
#include "fld_bmp.h"
#include "mapdefine.h"						//ZONE_ID_C01
#include "script.h"							//EventSet_Script
#include "scr_tool.h"
#include "ev_mapchange.h"					//

#include "sysflag.h"
#include "syswork.h"

#include "btl_searcher.h"

#include "msgdata/msg.naix"
#include "msgdata/msg_debug_nohara.h"

#include "..\fielddata\script\t01r0202_def.h"		//SCRID_TRAINER_MOVE_SCRIPT
#include "..\fielddata\script\common_scr_def.h"		//SCRID_TRAINER_MOVE_SCRIPT
#include "..\fielddata\script\connect_def.h"		//SCRID_CONNECT_COLOSSEUM
#include "..\fielddata\script\trainer_def.h"		//SCRID_TANPAN_01
#include "..\fielddata\script\r201_def.h"			//SCRID_R201_FLAG_CHANGE
#include "..\fielddata\script\hiden_def.h"			//SCRID_R201_FLAG_CHANGE
#include "..\fielddata\script\con_reception_def.h"	//
#include "..\fielddata\script\pc_ug_def.h"	//
#include "..\fielddata\script\perap_def.h"	//
#include "..\fielddata\script\saisen_def.h"	//
#include "..\fielddata\script\debug_scr_def.h"	//
#include "..\fielddata\script\factory_lobby_def.h"	//
#include "..\fielddata\script\seiseki_def.h"	//

#include "itemtool/myitem.h"
#include "itemtool/itemsym.h"

//#include "application/factory.h"
#include "../frontier/factory_def.h"
#include "../frontier/castle_def.h"
#include "../frontier/stage_def.h"
#include "../frontier/roulette_def.h"

#include "field/situation.h"
#include "situation_local.h"
#include "field_poketch.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
#define D_NOHARA_BMPWIN_MAX	(1)				//BMP�E�B���h�E�f�[�^�ő吔

typedef void (*voidFunc)(void* func);		//�֐��|�C���^�^


//==============================================================================================
//
//	�ϐ�
//
//==============================================================================================
static NNSSndCaptureOutputEffectType stereo_mono = NNS_SND_CAPTURE_OUTPUT_EFFECT_NORMAL;

//static s16 d_buf[ 32*100 ] ATTRIBUTE_ALIGN(32);	//�g�`�i�[�o�b�t�@(�����̊��H)
static u8 debug_t07r0201_no;

//==============================================================================================
//
//	�f�o�b�N�\����
//
//==============================================================================================
typedef struct{
	u8	seq;										//�����i���o�[
	u8	wave_buf_flag:1;							//�g�`�i�[�o�b�t�@���m�ۂ����t���O
	u8	waveout_flag:1;								//�����Đ����t���O
	u8	play_flag:6;								//�Đ����t���O
	s16 work;										//�ėp���[�N

	int friend_work;								//���Ԗڂ̗F�B��

	u8	fro_type;
	u8	btl_type;
	u8	fro_seq;
	u8	fro_add;

	u16 list_bak;									//���X�g�ʒu�o�b�N�A�b�v
	u16 cursor_bak;									//�J�[�\���ʒu�o�b�N�A�b�v

	u32 sel;
	u32 count;										//�J�E���^�[
	u32 count2;										//�J�E���^�[

	FIELDSYS_WORK* fsys;							//

	GF_BGL_BMPWIN bmpwin[D_NOHARA_BMPWIN_MAX];		//BMP�E�B���h�E�f�[�^
	BMPLIST_WORK* lw;								//BMP���X�g�f�[�^

	BMPLIST_DATA* menulist;							//

//	s16* wave_buf;									//�g�`�i�[�o�b�t�@�̃|�C���^

	//STRBUF* msg_buf[EV_WIN_MENU_MAX];				//���b�Z�[�W�f�[�^�̃|�C���^
	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g

	void* factory_call;								//�t�@�N�g���[�Ăяo��
}D_NOHARA_WORK;


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
void DebugNoharaMenuInit( FIELDSYS_WORK* fsys );

static void DebugNoharaMenuMain( TCB_PTR tcb, void * work );
static void TcbBmpDel( TCB_PTR tcb, void * work );
static void BmpDel( TCB_PTR tcb );
static void NumMsgSet( D_NOHARA_WORK* wk, GF_BGL_BMPWIN* win, int num, u8 x, u8 y );

void DebugBgmMenuInit( FIELDSYS_WORK* fsys );
void DebugScriptMenuInit( FIELDSYS_WORK* fsys );

static void Debug_StageScr_TypeLevelRecordSet( SAVEDATA* sv, u8 type, u8 csr_pos );

//static void D_Nohara_01( TCB_PTR tcb, void * work );
//static void D_Nohara_02( TCB_PTR tcb, void * work );
static void D_Nohara_03( TCB_PTR tcb, void * work );
//static void D_Nohara_04( TCB_PTR tcb, void * work );
//static void D_Nohara_05( TCB_PTR tcb, void * work );
//static void D_Nohara_06( TCB_PTR tcb, void * work );
//static void D_Nohara_07( TCB_PTR tcb, void * work );
//static void D_Nohara_08( TCB_PTR tcb, void * work );
static void D_Nohara_09( TCB_PTR tcb, void * work );
//static void D_Nohara_10( TCB_PTR tcb, void * work );
static void D_Nohara_11( TCB_PTR tcb, void * work );
//static void D_Nohara_12( TCB_PTR tcb, void * work );
//static void D_Nohara_13( TCB_PTR tcb, void * work );
//static void D_Nohara_14( TCB_PTR tcb, void * work );
//static void D_Nohara_15( TCB_PTR tcb, void * work );
static void D_Nohara_16( TCB_PTR tcb, void * work );
static void D_Nohara_17( TCB_PTR tcb, void * work );
//static void D_Nohara_18( TCB_PTR tcb, void * work );
//static void D_Nohara_19( TCB_PTR tcb, void * work );
//static void D_Nohara_20( TCB_PTR tcb, void * work );
static void D_Nohara_21( TCB_PTR tcb, void * work );
//static void D_Nohara_22( TCB_PTR tcb, void * work );
static void D_Nohara_23( TCB_PTR tcb, void * work );
static void D_Nohara_24( TCB_PTR tcb, void * work );
static void D_Nohara_25( TCB_PTR tcb, void * work );
static void D_Nohara_26( TCB_PTR tcb, void * work );
static void D_Nohara_27( TCB_PTR tcb, void * work );
static void D_Nohara_28( TCB_PTR tcb, void * work );
static void D_Nohara_29( TCB_PTR tcb, void * work );
//static void D_Nohara_30( TCB_PTR tcb, void * work );
//static void D_Nohara_31( TCB_PTR tcb, void * work );
//static void D_Nohara_32( TCB_PTR tcb, void * work );
static void D_Nohara_33( TCB_PTR tcb, void * work );
static void D_Nohara_34( TCB_PTR tcb, void * work );
static void D_Nohara_35( TCB_PTR tcb, void * work );
static void D_Nohara_40( TCB_PTR tcb, void * work );
static void D_Nohara_41( TCB_PTR tcb, void * work );
static void D_Nohara_42( TCB_PTR tcb, void * work );


//==============================================================================================
//
//	���X�g�f�[�^
//
//==============================================================================================
static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList[] = {
	{ msg_debug_nohara_35, (u32)D_Nohara_35 },		//�ʑ��`�F�b�N
	{ msg_debug_nohara_34, (u32)D_Nohara_34 },		//�t�����e�B�A�A��������
	//{ msg_debug_nohara_32, (u32)D_Nohara_32 },		//�b�o�Z�b�g
	{ msg_debug_nohara_21, (u32)D_Nohara_21 },		//�f�o�b�N�W�����v
	//{ msg_debug_nohara_20, (u32)D_Nohara_31 },		//�^��Đ�
	//{ msg_debug_nohara_30, (u32)D_Nohara_30 },		//�o�g���t�@�N�g���[���j�^�[
	{ msg_debug_nohara_11, (u32)D_Nohara_11 },
	{ msg_debug_nohara_40, (u32)D_Nohara_40 },		//�t�����e�B�A�����t���OON
	{ msg_debug_nohara_33, (u32)D_Nohara_33 },		//HP5
	{ msg_debug_nohara_42, (u32)D_Nohara_42 },		//hp_rnd,spedef_rnd��K����������

	{ msg_debug_nohara_24, (u32)D_Nohara_24 },		//�o�g���T�[�`���[
	//{ msg_debug_nohara_01, (u32)D_Nohara_01 },
	//{ msg_debug_nohara_02, (u32)D_Nohara_02 },
	//{ msg_debug_nohara_03, (u32)D_Nohara_03 },
	//{ msg_debug_nohara_04, (u32)D_Nohara_04 },
	//{ msg_debug_nohara_05, (u32)D_Nohara_05 },
	//{ msg_debug_nohara_06, (u32)D_Nohara_06 },
	//{ msg_debug_nohara_07, (u32)D_Nohara_07 },
	//{ msg_debug_nohara_08, (u32)D_Nohara_08 },
	//{ msg_debug_nohara_09, (u32)D_Nohara_09 },
	//{ msg_debug_nohara_10, (u32)D_Nohara_10 },
	//{ msg_debug_nohara_12, (u32)D_Nohara_12 },
	//{ msg_debug_nohara_13, (u32)D_Nohara_13 },
	//{ msg_debug_nohara_14, (u32)D_Nohara_14 },
	//{ msg_debug_nohara_15, (u32)D_Nohara_15 },		//�g�`�e�X�g
	//{ msg_debug_nohara_16, (u32)D_Nohara_16 },	//���C���V�i���I�i�s
	{ msg_debug_nohara_17, (u32)D_Nohara_17 },		//�o�b�W�t���O�Z�b�g
	//{ msg_debug_nohara_18, (u32)D_Nohara_18 },		//�B���A�C�e�����X�g
	//{ msg_debug_nohara_19, (u32)D_Nohara_19 },		//�L���v�`���`�F�b�N
	//{ msg_debug_nohara_22, (u32)D_Nohara_22 },		//�|�P�b�`�g�`�e�X�g
	//{ msg_debug_nohara_23, (u32)D_Nohara_23 },		//�}�ӁA�o�b�O�V�X�e���t���O�Z�b�g
	{ msg_debug_nohara_27, (u32)D_Nohara_27 },		//�|�P����������2�Đ��o����t���O�I��
	{ msg_debug_nohara_28, (u32)D_Nohara_28 },		//�|�P����������2�Đ��o����t���O�I�t
	{ msg_debug_nohara_29, (u32)D_Nohara_29 },		//�E�F�C�g�w�肠��̖����Đ��̃e�X�g
	//{ msg_debug_nohara_20, (u32)D_Nohara_20 },		//�M�l�X�E�B���h�E
	{ msg_debug_nohara_41, (u32)D_Nohara_41 },		//�X�e�[�W�A��������(�O���Z�[�u���K�v)
};

static const BMPLIST_HEADER DebugListH = {
	NULL,					//�\�������f�[�^�|�C���^

	NULL,					//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,					//GF_BGL_BMPWIN�̃|�C���^

	NELEMS(DebugMenuList),	//���X�g���ڐ�
	9,						//�\���ő區�ڐ�

	0,						//���x���\���w���W
	8,						//���ڕ\���w���W
	0,						//�J�[�\���\���w���W
	0,						//�\���x���W
/*
	FBMP_COL_BLACK,			//�����F
	FBMP_COL_WHITE,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
*/
	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						//�����Ԋu�w
	16,						//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,		//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};


//==============================================================================================
//
//	BMP�E�B���h�E
//
//==============================================================================================
enum{
	D_NOHARA_BMPWIN_FRAME	= GF_BGL_FRAME3_M,
	D_NOHARA_BMPWIN_PX1		= 1,
	D_NOHARA_BMPWIN_PY1		= 1,
	D_NOHARA_BMPWIN_SX		= 16,
	D_NOHARA_BMPWIN_SY		= 20,
	D_NOHARA_BMPWIN_PL		= FLD_SYSFONT_PAL,
	D_NOHARA_BMPWIN_CH		= 1,
};

static const BMPWIN_DAT	DebugNoharaWinData = {
	D_NOHARA_BMPWIN_FRAME,					//�E�C���h�E�g�p�t���[��
	D_NOHARA_BMPWIN_PX1,D_NOHARA_BMPWIN_PY1,//�E�C���h�E�̈�̍����X,Y���W�i�L�����P�ʂŎw��j
	D_NOHARA_BMPWIN_SX,	D_NOHARA_BMPWIN_SY,	//�E�C���h�E�̈��X,Y�T�C�Y�i�L�����P�ʂŎw��j
	D_NOHARA_BMPWIN_PL,						//�E�C���h�E�̈�̃p���b�g�i���o�[	
	D_NOHARA_BMPWIN_CH						//�E�C���h�E�L�����̈�̊J�n�L�����N�^�i���o�[
};


//==============================================================================================
//
//	�v���O����
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�f�o�b�N���j���[�Ăяo��(fld_debug.c)
 *
 * @param	fsys	FIELDSYS_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void DebugNoharaMenuInit( FIELDSYS_WORK* fsys )
{
	int i;
	BMPLIST_HEADER list_h;
	D_NOHARA_WORK* wk;

	wk = (D_NOHARA_WORK*)TCB_GetWork( PMDS_taskAdd(DebugNoharaMenuMain, sizeof(D_NOHARA_WORK), 0, HEAPID_BASE_DEBUG) );

	wk->seq				= 0;
	wk->sel				= 0;
	wk->work			= 0;
	wk->wave_buf_flag	= 0;								//�g�`�i�[�o�b�t�@���m�ۂ����t���OOFF
	wk->waveout_flag	= 0;								//�����Đ����t���OOFF
	wk->fsys			= fsys;

	GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//�r�b�g�}�b�v�ǉ�

	wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList), HEAPID_BASE_DEBUG );


	wk->wordset = WORDSET_Create( HEAPID_BASE_DEBUG );

	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, 
								NARC_msg_debug_nohara_dat, HEAPID_BASE_DEBUG);

	for( i=0; i < NELEMS(DebugMenuList); i++ ){
		BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
										DebugMenuList[i].str_id, DebugMenuList[i].param );
	}

	list_h			= DebugListH;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[0];
	wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );

	GF_BGL_BmpWinOn( &wk->bmpwin[0] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�f�o�b�N���j���[���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void DebugNoharaMenuMain( TCB_PTR tcb, void * work )
{
	u32	ret;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	switch( wk->seq ){
	case 0:
		ret = BmpListMain( wk->lw );

		switch( ret ){
		case BMPLIST_NULL:
			break;
		case BMPLIST_CANCEL:
			TcbBmpDel( tcb, work );				//TCBBMP�J��

			//�g�`�Đ��p�`�����l�����J������
			Snd_DebugNormalChannelFree();

			break;
		default:
			wk->sel = ret;
			wk->seq++;
			break;
		};

		//BMP���X�g�̃��X�g�ʒu�A�J�[�\���ʒu���擾
		BmpListPosGet( wk->lw, &wk->list_bak, &wk->cursor_bak );
		break;

	case 1:
		{
			voidFunc func = (voidFunc)wk->sel;
			TCB_ChangeFunc( tcb, (void*)func );	//TCB�̓���֐��؂�ւ�
		}
		break;

	};

	return;
}

//--------------------------------------------------------------
/**
 * @brief	TCB,BMP�J��
 *
 * @param	tcb		TCB_PTR�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void TcbBmpDel( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	MSGMAN_Delete( wk->msgman );
	WORDSET_Delete( wk->wordset );

#if 0
	//�g�`�i�[�o�b�t�@���m�ۂ����t���OON��������
	if( wk->wave_buf_flag == 1 ){
		sys_FreeMemoryEz( wk->wave_buf );	//�o�b�t�@�J��
	}
#endif

	BmpDel( tcb );						//BMP�J��
	PMDS_taskDel( tcb );				//TCB�J��
	FieldSystemProc_SeqHoldEnd();
	return;
}

//--------------------------------------------------------------
/**
 * @brief	BMP�J��
 *
 * @param	tcb		TCB_PTR�^
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void BmpDel( TCB_PTR tcb )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)TCB_GetWork(tcb);

	//BMP
	BMP_MENULIST_Delete( wk->menulist );
	BmpListExit( wk->lw, &wk->list_bak, &wk->cursor_bak );
	GF_BGL_BmpWinOff( &wk->bmpwin[0] );
	GF_BGL_BmpWinDel( &wk->bmpwin[0] );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�������b�Z�[�W�\��
 *
 * @param	win_index	�r�b�g�}�b�vINDEX
 * @param	num			���l
 * @param	x			�\���ʒuX
 * @param	y			�\���ʒuY
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void NumMsgSet( D_NOHARA_WORK* wk, GF_BGL_BMPWIN* win, int num, u8 x, u8 y )
{
	STRBUF* tmp_buf	= STRBUF_Create( 12, HEAPID_BASE_DEBUG );
	STRBUF* tmp_buf2= STRBUF_Create( 12, HEAPID_BASE_DEBUG );

	MSGMAN_GetString( wk->msgman, msg_debug_nohara_num, tmp_buf );
	WORDSET_RegisterNumber(wk->wordset, 1, num, 4, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, tmp_buf2, tmp_buf );

	GF_STR_PrintSimple( win, FONT_SYSTEM, tmp_buf2, x, y, MSG_NO_PUT, NULL );

	STRBUF_Delete( tmp_buf );
	STRBUF_Delete( tmp_buf2 );
	return;
}


//==============================================================================================
//
//	01	�t�Đ��֘A
//
//==============================================================================================
//static void D_Nohara_01_Main( TCB_PTR tcb, void * work );

//extern BOOL Snd_ArcStrmStart( u16 no );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_01( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_01_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	02	���o�[�u�֘A
//
//==============================================================================================
//static void D_Nohara_02_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_02( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_02_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	03	�g���b�N�t�F�[�h�֘A
//
//==============================================================================================
//static void D_Nohara_03_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_03( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_03_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	04	FIELD BGM ���ʊ֘A
//
//==============================================================================================
//static void D_Nohara_04_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_04( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_04_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	05	�����p�^�[���֘A
//
//==============================================================================================
//static void D_Nohara_05_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_05( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_05_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	06	�o�̓G�t�F�N�g�֘A
//
//==============================================================================================
//static void D_Nohara_06_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_06( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_06_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	07	�}�C�N�֘A
//
//==============================================================================================
//static void D_Nohara_07_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_07( TCB_PTR tcb, void * work )
 
//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_07_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	08	FIELD TEMPO ���ʊ֘A
//
//==============================================================================================
//static void D_Nohara_08_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_08( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_08_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	09	�X�N���v�g�֘A
//
//==============================================================================================
static void D_Nohara_09_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_09( TCB_PTR tcb, void * work )
{
	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	10	�g���b�N�t�F�[�h�t���O����
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_10( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	11	�X�N���v�g���s�֘A
//
//==============================================================================================
static void D_Nohara_11_Main( TCB_PTR tcb, void * work );

static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList11[] = {
	{ msg_debug_nohara_11_01, (u32)D_Nohara_11_Main },
	{ msg_debug_nohara_11_02, (u32)D_Nohara_11_Main },
	{ msg_debug_nohara_11_03, (u32)D_Nohara_11_Main },
};

static const BMPLIST_HEADER ListH11 = {
	NULL,					//�\�������f�[�^�|�C���^
	NULL,					//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,					//GF_BGL_BMPWIN�̃|�C���^

	NELEMS(DebugMenuList11),//���X�g���ڐ�
	10,						//�\���ő區�ڐ�

	0,						//���x���\���w���W
	8,						//���ڕ\���w���W
	0,						//�J�[�\���\���w���W
	0,						//�\���x���W
/*
	FBMP_COL_BLACK,			//�����F
	FBMP_COL_WHITE,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
*/
	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						//�����Ԋu�w
	16,						//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,		//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_11( TCB_PTR tcb, void * work )
{
	int i;
	BMPLIST_HEADER list_h;
	GF_BGL_BMPWIN* win;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	BmpDel( tcb );								//BMP�J��

	wk->seq				= 0;
	wk->sel				= 0;
	wk->count			= 0;

	GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//�r�b�g�}�b�v�ǉ�

	wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList11), HEAPID_BASE_DEBUG );

	for( i=0; i < NELEMS(DebugMenuList11); i++ ){
		BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
										DebugMenuList11[i].str_id, DebugMenuList11[i].param );
	}

	list_h			= ListH11;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[0];
	wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );

	GF_BGL_BmpWinOn( &wk->bmpwin[0] );

	//TCB_ChangeFunc( tcb, D_Nohara_11_Main );	//TCB�̓���֐��؂�ւ�
	TCB_ChangeFunc( tcb, DebugNoharaMenuMain );	//TCB�̓���֐��؂�ւ�
	return;
}
 
//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_11_Main( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	switch( wk->cursor_bak ){

	case 0:
		//�V�F�C�~�C�x���g�Ń}�C���o��������
		SpScriptStart( wk->fsys, SCRID_DEBUG_R224_SYEIMI );
		break;

	case 1:
		//�R�̌΃C�x���g�J�n
		SpScriptStart( wk->fsys, SCRID_DEBUG_L01_L02_L03_LAKE );
		break;

	case 2:
		//�|�P�Z���n���X�g�b�p�[�폜�A�Ƃ������蒠�C�x���g����
		SpScriptStart( wk->fsys, SCRID_DEBUG_PC_UG );
		break;

	default:
		//EventSet_Script( wk->fsys, SCRID_TANPAN_01, NULL );
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	};

	wk->seq = 0;
	TCB_ChangeFunc( tcb, DebugNoharaMenuMain );		//TCB�̓���֐��؂�ւ�
	return;
}


//==============================================================================================
//
//	12	��������
//
//==============================================================================================
//static void D_Nohara_12_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_12( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_12_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	13	��������2
//
//==============================================================================================
//static void D_Nohara_13_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_13( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_13_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	14	�T�E���h�G�t�F�N�g
//
//==============================================================================================
//static void D_Nohara_14_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_14( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_14_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	15	�����g�`�֘A
//
//==============================================================================================
//static void D_Nohara_15_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_15( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_15_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	16	���C���V�i���I�֘A
//
//	���݂̃]�[���̃X�N���v�g�ƃ��b�Z�[�W��ǂݍ��ނ悤�ɂ��Ă���̂ŁA
//	�f�o�b�N�ŃX�N���v�g��ID���w�肷�邾���ł͏o���Ȃ��I
//
//	�܂��X�N���v�g�����쐬���Ă��Ȃ����̂́Acommon_scr.ev�ɒǉ����Ď����B�B�B
//	�Ƃ��v�������A���b�Z�[�W���Ȃ��̂ŁA���������B�B�B
//
//==============================================================================================
static void D_Nohara_16_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_16( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;
	wk->seq		= 0;
	wk->count	= 0;
	wk->work	= 0;
	TCB_ChangeFunc( tcb, D_Nohara_16_Main );	//TCB�̓���֐��؂�ւ�
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_16_Main( TCB_PTR tcb, void * work )
{
	//�I��
	if( sys.cont == PAD_BUTTON_B ){
		TcbBmpDel( tcb, work );		//TCBBMP�J��
	}

	return;
}


//==============================================================================================
//
//	17	�o�b�W�t���O�Z�b�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_17( TCB_PTR tcb, void * work )
{
	int i;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;
	MYSTATUS* my = SaveData_GetMyStatus( wk->fsys->savedata );

	for( i=0; i < 8; i++ ){
		MyStatus_SetBadgeFlag( my, i );
	}

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	18	�B���A�C�e������
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_18( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	19	�L���v�`���`�F�b�N
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_19( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	20	�M�l�X�E�B���h�E
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_20( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	21	�f�o�b�N�W�����v
//
//==============================================================================================
static void D_Nohara_21_Main( TCB_PTR tcb, void * work );

typedef struct{
	u32 zone_id;
	u32 num;
	u32 x;
	u32 z;
}DEBUG_JUMP_WORK;

static const DEBUG_JUMP_WORK debug_jump_work[] = {

	{ ZONE_ID_D24R0106,	2406,	10,		10 },			//
	{ ZONE_ID_D05R0111,	5111,	10,		10 },			//
	{ ZONE_ID_R228,		228,	783,	339 },			//

	{ ZONE_ID_D35R0101,	3501,	10,		10 },			//
	{ ZONE_ID_D35R0102,	3502,	10,		10 },			//
	{ ZONE_ID_D24R0106,	2406,	10,		10 },			//

	{ ZONE_ID_T06R0101,	601,	7,		11 },			//�Đ�{��
	{ ZONE_ID_D05R0114,	514,	31,		33 },			//���̂͂���
	{ ZONE_ID_D05R0115,	515,	31,		33 },			//���̂͂���
	{ ZONE_ID_D17R0104,	174,	10,		10 },			//���ǂ�̂ǂ�����
	//{ ZONE_ID_D32R0401,	3241,	16,		16 },			//�X�e�[�W��t
	//{ ZONE_ID_D32R0301,	3231,	16,		16 },			//�t�@�N�g���[��t
	{ ZONE_ID_T02,		2,		160,	846 },			//�}�T�S�^�E��
	{ ZONE_ID_T03,		3,		176,	624 },			//�\�m�I�^�E��
	{ ZONE_ID_R201,		201,	110,	856 },			//
	{ ZONE_ID_R205A,	2051,	208,	592 },			//
	{ ZONE_ID_R205B,	2052,	272,	528 },			//
	{ ZONE_ID_C05R1101,	511,	5,		5 },			//�R���e�X�g��t
	{ ZONE_ID_D27R0101, 2701,	46,		53 },			//
};
#define DEBUG_JUMP_WORK_MAX	( NELEMS(debug_jump_work) )

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_21( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;
	wk->seq		= 0;
	wk->count	= 0;
	wk->work	= 0;

	NumMsgSet( wk, &wk->bmpwin[0], debug_jump_work[wk->work].num, 8*7, 8*2 );
	GF_BGL_BmpWinOn( &wk->bmpwin[0] );

	TCB_ChangeFunc( tcb, D_Nohara_21_Main );	//TCB�̓���֐��؂�ւ�
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_21_Main( TCB_PTR tcb, void * work )
{
	int flag;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	flag = 0;	//���������t���O

	if( (sys.repeat == PAD_KEY_UP) || (sys.cont == PAD_KEY_RIGHT) ){
		flag = 1;
		wk->work++;
		if( wk->work >= DEBUG_JUMP_WORK_MAX ){
			wk->work = 0;
		}
	}

	if( (sys.repeat == PAD_KEY_DOWN) || (sys.cont == PAD_KEY_LEFT) ){
		flag = 1;
		wk->work--;
		if( wk->work < 0 ){
			wk->work = (DEBUG_JUMP_WORK_MAX - 1);
		}
	}

	//���������t���O�������Ă�����
	if( flag == 1 ){
		//�w��͈͂�h��Ԃ�
		GF_BGL_BmpWinFill( &wk->bmpwin[0], FBMP_COL_BLACK, 8*7, 8*2, 
								8*8, 8*2 );
	
		//�l�\��
		NumMsgSet( wk, &wk->bmpwin[0], debug_jump_work[wk->work].num, 8*7, 8*2 );
		GF_BGL_BmpWinOn( &wk->bmpwin[0] );
	}

	//�W�����v
	if( sys.trg == PAD_BUTTON_A ){

		EventSet_EasyMapChange( wk->fsys, debug_jump_work[wk->work].zone_id, DOOR_ID_JUMP_CODE, 
							debug_jump_work[wk->work].x , debug_jump_work[wk->work].z, DIR_DOWN );

		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	}

	//�I��
	if( sys.cont == PAD_BUTTON_B ){
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	}

	return;
}


//==============================================================================================
//
//	22	�|�P�b�`�g�`�e�X�g
//
//==============================================================================================
//static void D_Nohara_22_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_22( TCB_PTR tcb, void * work )

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
//static void D_Nohara_22_Main( TCB_PTR tcb, void * work )


//==============================================================================================
//
//	23	�}�ӁA�o�b�O�V�X�e���t���O�Z�b�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_23( TCB_PTR tcb, void * work )
{
	int type,ret;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	{
		//���j���[�u�}�Ӂv
		ZUKAN_WORK* zw = SaveData_GetZukanWork( wk->fsys->savedata );
		ZukanWork_SetZukanGetFlag( zw );

		//���j���[�u�o�b�O�v
		SysFlag_BagSet( SaveData_GetEventWork(wk->fsys->savedata) );

		//���j���[�u�|�P�����v
		SysWork_FirstPokeNoSet( SaveData_GetEventWork(wk->fsys->savedata), MONSNO_HUSIGIDANE );
	}

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	24	�o�g���T�[�`���[�֘A
//
//==============================================================================================
static void D_Nohara_24_Main( TCB_PTR tcb, void * work );

static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList24[] = {
	{ msg_debug_nohara_24_01, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_02, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_03, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_04, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_05, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_06, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_07, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_08, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_09, (u32)D_Nohara_24_Main },
	{ msg_debug_nohara_24_10, (u32)D_Nohara_24_Main },
};

static const BMPLIST_HEADER ListH24 = {
	NULL,					//�\�������f�[�^�|�C���^
	NULL,					//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,					//GF_BGL_BMPWIN�̃|�C���^

	NELEMS(DebugMenuList24),//���X�g���ڐ�
	10,						//�\���ő區�ڐ�

	0,						//���x���\���w���W
	8,						//���ڕ\���w���W
	0,						//�J�[�\���\���w���W
	0,						//�\���x���W
/*
	FBMP_COL_BLACK,			//�����F
	FBMP_COL_WHITE,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
*/
	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						//�����Ԋu�w
	16,						//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,		//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_24( TCB_PTR tcb, void * work )
{
	int i;
	BMPLIST_HEADER list_h;
	GF_BGL_BMPWIN* win;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	BmpDel( tcb );								//BMP�J��

	wk->seq				= 0;
	wk->sel				= 0;
	wk->count			= 0;

	GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//�r�b�g�}�b�v�ǉ�

	wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList24), HEAPID_BASE_DEBUG );

	for( i=0; i < NELEMS(DebugMenuList24); i++ ){
		BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
										DebugMenuList24[i].str_id, DebugMenuList24[i].param );
	}

	list_h			= ListH24;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[0];
	wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );

	GF_BGL_BmpWinOn( &wk->bmpwin[0] );

	//TCB_ChangeFunc( tcb, D_Nohara_24_Main );	//TCB�̓���֐��؂�ւ�
	TCB_ChangeFunc( tcb, DebugNoharaMenuMain );	//TCB�̓���֐��؂�ւ�
	return;
}
 
//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_24_Main( TCB_PTR tcb, void * work )
{
	EVENTWORK* ev;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	ev = SaveData_GetEventWork( wk->fsys->savedata );

	switch( wk->cursor_bak ){
	case 0:
		//�o�b�e���[MAX
		SysWork_BtlSearcherBatterySet( ev, BS_BATTERY_MAX );
		break;

	case 1:
		//�o�b�e���[CLR
		SysWork_BtlSearcherBatterySet( ev, 0 );
		break;

	case 2:
		//�N���A�[MAX
		SysWork_BtlSearcherClearSet( ev, BS_CLEAR_MAX );
		break;

	case 3:
		//�N���A�[CLR
		SysWork_BtlSearcherClearSet( ev, 0 );
		break;

	case 4:
		//�N��
		EventSet_Script( wk->fsys, SCRID_BTL_SEARCHER, NULL );
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;						//���ӁI

	case 5:
		//���x���P�Z�b�g
		SysFlag_BsLvSet( ev, 1 );

		MyItem_AddItem( SaveData_GetMyItem(wk->fsys->savedata),
						ITEM_BATORUSAATYAA, 1, HEAPID_BASE_DEBUG);
		break;

	case 6:
		//���x���Q�Z�b�g
		SysFlag_BsLvSet( ev, 2 );
		break;

	case 7:
		//���x���R�Z�b�g
		SysFlag_BsLvSet( ev, 3 );
		break;

	case 8:
		//���x���S�Z�b�g
		SysFlag_BsLvSet( ev, 4 );
		break;

	case 9:
		//���x���T�Z�b�g
		SysFlag_BsLvSet( ev, 5 );
		break;

	default:
		return;
	};

	wk->seq = 0;
	TCB_ChangeFunc( tcb, DebugNoharaMenuMain );		//TCB�̓���֐��؂�ւ�
	return;
}


//==============================================================================================
//
//	25	BGM�I�t
//
//==============================================================================================
extern void Snd_DebugBgmFlagSet( u8 sw );

//--------------------------------------------------------------
/**
 * @brief	BGM�I�t
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_25( TCB_PTR tcb, void * work )
{
#ifdef PM_DEBUG
	Snd_DebugBgmFlagSet( 1 );
	Snd_Stop();
#endif

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	26	BGM�I��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	BGM�I��
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_26( TCB_PTR tcb, void * work )
{
#ifdef PM_DEBUG
	u16 bgm_no;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	Snd_DebugBgmFlagSet( 0 );

	//�T�E���h�f�[�^�Z�b�g(�V�[�����ύX����Ȃ����͉������Ȃ�)
	Snd_SceneSet( SND_SCENE_DUMMY );
	bgm_no = Snd_FieldBgmNoGet( wk->fsys, wk->fsys->location->zone_id );
	Snd_ZoneBgmSet(Snd_FieldBgmNoGetNonBasicBank(wk->fsys,wk->fsys->location->zone_id));//zone set

	Snd_DataSetByScene( SND_SCENE_FIELD, bgm_no, 1 );
#endif

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	27	�|�P����������2�Đ��o����t���O"�I��"
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�|�P����������2�Đ��o����t���O"�I��"
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_27( TCB_PTR tcb, void * work )
{
#ifdef SND_PV_070213
	Snd_PMVoiceDoubleFlagSet( 1 );
#endif
	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	28	�|�P����������2�Đ��o����t���O"�I�t"
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�|�P����������2�Đ��o����t���O"�I�t"
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_28( TCB_PTR tcb, void * work )
{
#ifdef SND_PV_070213
	Snd_PMVoiceDoubleFlagSet( 0 );
#endif
	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	29	�E�F�C�g�w�肠��̖����Đ��̃e�X�g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�E�F�C�g�w�肠��̖����Đ��̃e�X�g
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_29( TCB_PTR tcb, void * work )
{
#ifdef SND_PV_070213

#if 0
	Snd_PMVoiceWaitPlayEx( PV_NORMAL, MONSNO_HITOKAGE, 127, 127, HEAPID_WORLD, 60, 0 );

	Snd_PMVoiceWaitPlayEx( PV_NORMAL, MONSNO_WANINOKO, -128, 127, HEAPID_WORLD, 100, 0 );
#endif

#if 0
	Snd_PMVoicePlay( MONSNO_KAIOOGA, 0 );
	Snd_PMVoiceWaitPlayEx( PV_NORMAL, MONSNO_WANINOKO, -128, 127, HEAPID_WORLD, 10, 0 );
#endif

#if 0
	Snd_PMVoiceWaitPlayEx( PV_NORMAL, MONSNO_WANINOKO, -128, 127, HEAPID_WORLD, 10, 0 );
	Snd_PMVoicePlay( MONSNO_KAIOOGA, 0 );
#endif

	Snd_PMVoiceWaitPlayEx( PV_NORMAL, MONSNO_WANINOKO, -128, 127, HEAPID_WORLD, 100, 0 );

#endif

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	30	�o�g���t�@�N�g���[
//
//==============================================================================================
#if 0
static void D_Nohara_30_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	�o�g���t�@�N�g���[���j�^�[�Ăяo��
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_30( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	EventSet_Script( wk->fsys, SCRID_SEISEKI_STAGE, NULL );

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_30_Main( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	switch( wk->seq ){

	case 0:
		//FieldSystemProc_SeqHoldEnd();		

		//wk->factory_call = Factory_SetProc( wk->fsys, HEAPID_BASE_DEBUG, 
		//									BF_TYPE_SINGLE, BF_MODE_RENTARU );

		//GameSystem_FinishFieldProc( wk->fsys );

		wk->seq++;
		break;

	case 1:
		//�v���Z�X�I���҂�
		if( FieldEvent_Cmd_WaitSubProcEnd(wk->fsys) == FALSE ){
			sys_FreeMemoryEz( wk->factory_call );
			wk->seq++;
		}
		break;

	case 2:
		if(GameSystem_CheckSubProcExists(wk->fsys) == FALSE){
			//�t�B�[���h�}�b�v�v���Z�X���A
			FieldEvent_Cmd_SetMapProc( wk->fsys );
		}
		break;

	case 3:
		//�t�B�[���h�v���Z�X�J�n�I���҂�
		if( !FieldEvent_Cmd_WaitMapProcStart(wk->fsys) ){
			wk->seq = 0;
			TcbBmpDel( tcb, work );				//TCBBMP�J��
		}
		break;
	};

	return;
}
#endif


//==============================================================================================
//
//	31	�^��Đ�
//
//==============================================================================================
#if 0
static void D_Nohara_31_Main( TCB_PTR tcb, void * work );

//--------------------------------------------------------------
/**
 * @brief	�^��Đ�
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_31( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;
	wk->seq		= 0;
	TCB_ChangeFunc( tcb, D_Nohara_31_Main );	//TCB�̓���֐��؂�ւ�
	return;
}

BATTLE_PARAM* bp;
//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_31_Main( TCB_PTR tcb, void * work )
{
	LOAD_RESULT load_ret;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	switch( wk->seq ){

	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_WORLD );
		wk->seq++;
		break;

	case 1:
		//�C�x���g�R�}���h�F�t�B�[���h�}�b�v�v���Z�X�I��
		//EventCmd_FinishFieldMap( wk->fsys->event );
		
		if( WIPE_SYS_EndCheck() == TRUE ){
			MSGMAN_Delete( wk->msgman );
			WORDSET_Delete( wk->wordset );
			BmpDel( tcb );						//BMP�J��
			wk->seq++;
		}
		break;

	case 2:
		//BATTLE_PARAM�ݒ�
		bp = BattleParam_Create( HEAPID_WORLD, FIGHT_TYPE_1vs1 );
		BattleParam_SetParamByGameDataCore( bp, wk->fsys, wk->fsys->savedata, 
											wk->fsys->location->zone_id, 
											wk->fsys->fnote, wk->fsys->bag_cursor, 
											wk->fsys->battle_cursor );

		//bp	���[�h�����f�[�^���琶������BATTLE_PARAM�\���̂ւ̃|�C���^
		//num	���[�h����f�[�^�i���o�[�iLOADDATA_MYREC�ALOADDATA_DOWNLOAD1�ALOADDATA_DOWNLOAD2�c�j

		//�ΐ�^��f�[�^�̃��[�h
		BattleRec_Load( wk->fsys->savedata, HEAPID_WORLD, &load_ret, bp, LOADDATA_MYREC );
		OS_Printf( "battle_load ret = %d\n", load_ret );
#if 0
	LOAD_RESULT_NULL = 0,		///<�f�[�^�Ȃ�
	LOAD_RESULT_OK,				///<�f�[�^����ǂݍ���
	LOAD_RESULT_NG,				///<�f�[�^�ُ�
	LOAD_RESULT_BREAK,			///<�j��A�����s�\
#endif
		wk->seq++;
		break;

	case 3:
		GameSystem_FinishFieldProc( wk->fsys );
		Snd_DataSetByScene( SND_SCENE_BATTLE, SEQ_BA_TRAIN, 1 );	//�o�g���ȍĐ�
		FieldBattle_SetProc( wk->fsys, bp );
		OS_Printf( "�^��퓬�Ăяo���e�X�g\n" );
		wk->seq++;
		break;

	case 4:
		if( FieldEvent_Cmd_WaitSubProcEnd(wk->fsys) ){		//�T�u�v���Z�X�I���҂�
			break;
		}
		wk->seq++;
		break;

	case 5:
		if( GameSystem_CheckSubProcExists( wk->fsys ) == FALSE ){
			BattleParam_Delete( bp );						//BATTLE_PARAM�̊J��
			BattleRec_Exit();
			//FieldEvent_Cmd_SetMapProc( wk->fsys );
			GameSystem_CreateFieldProc( wk->fsys );			//�t�B�[���h���A
			wk->seq++;
		}
		break;

	case 6:
		if( GameSystem_CheckFieldProcExists(wk->fsys) ){
		//if( FieldEvent_Cmd_WaitMapProcStart(wk->fsys) ){
			wk->seq++;
		}
		break;

	case 7:
		FieldFadeWipeSet( FLD_DISP_BRIGHT_BLACKIN );
#if 0

		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_WORLD );
		wk->seq++;
		break;

	case 8:
		if( WIPE_SYS_EndCheck() == TRUE ){
#endif
			wk->seq = 0;
			PMDS_taskDel( tcb );				//TCB�J��
			FieldSystemProc_SeqHoldEnd();
//		}
		return;
	};

	return;
}
#endif


//==============================================================================================
//
//	32	�b�o�Z�b�g
//
//==============================================================================================
#if 0
//--------------------------------------------------------------
/**
 * @brief	�b�o�Z�b�g
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_32( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	//�V���O�����݂�CP���R�[�h��9999
	FrontierRecord_Set( SaveData_GetFrontier(wk->fsys->savedata), 
						CastleScr_GetCPRecordID(CASTLE_TYPE_SINGLE),
						FRONTIER_RECORD_NOT_FRIEND, 9999 );

	//�}���`���݂�CP���R�[�h��9999
	FrontierRecord_Set( SaveData_GetFrontier(wk->fsys->savedata), 
						CastleScr_GetCPRecordID(CASTLE_TYPE_MULTI),
						FRONTIER_RECORD_NOT_FRIEND, 9999 );

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}
#endif


//==============================================================================================
//
//	33	HP����
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	HP����
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_33( TCB_PTR tcb, void * work )
{
	u32 hp;
	POKEMON_PARAM* poke;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(wk->fsys->savedata), 0 );

	hp = 5;
	PokeParaPut( poke, ID_PARA_hp, &hp );
	//PokeParaCalc( poke );

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	42	hp_rnd,spedef_rnd����
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	hp_rnd,spedef_rnd����
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_42( TCB_PTR tcb, void * work )
{
	u32 hp;
	POKEMON_PARAM* poke;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(wk->fsys->savedata), 0 );

	//�w�͒l�e�X�g
#if 0
	hp = 200;
	PokeParaPut( poke, ID_PARA_hp_rnd, &hp );
	hp = 200;
	PokeParaPut( poke, ID_PARA_spedef_rnd, &hp );
#else
	hp = 200;
	PokeParaPut( poke, ID_PARA_hp_exp, &hp );
	hp = 200;
	PokeParaPut( poke, ID_PARA_spedef_exp, &hp );
#endif

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	35	�ʑ��C�x���g
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�ʑ��C�x���g
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_35( TCB_PTR tcb, void * work )
{
	u16 waza;
	POKEMON_PARAM* poke;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	//���R�[�h�\��
	//OS_Printf( "dendou = %d\n", RECORD_Get(SaveData_GetRecord(wk->fsys->savedata),
	//								RECID_DENDOU_CNT) );

#if 0
	//�擪�̃|�P������waza3�Awaza4��Y�ꂳ����

	//�|�P�����ւ̃|�C���^�擾
	poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(wk->fsys->savedata), 0 );

	waza = 0;
	PokeParaPut( poke, ID_PARA_waza3, &waza );
	PokeParaPut( poke, ID_PARA_waza4, &waza );
	//PokeParaCalc( poke );
#endif

#if 0
	debug_t07r0201_no++;
	if( debug_t07r0201_no >= T07R0201_EVENT_EVENT_MAX ){
		debug_t07r0201_no = 0;
	}

	OS_Printf( "�ʑ��̃C�x���g�i���o�[��%d�ɕύX���܂���\n", debug_t07r0201_no );
	SysFlag_T07ObjInReset( SaveData_GetEventWork(wk->fsys->savedata) );//�ʑ��O��OBJ�����ɓ�����off
	SysWork_T07R0201Set( SaveData_GetEventWork(wk->fsys->savedata), debug_t07r0201_no );
#else
	SysWorkUpdateEventT07R0201( wk->fsys->savedata );
#endif

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	40	�t�����e�B�A����
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�t�����e�B�A����
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_40( TCB_PTR tcb, void * work )
{
	EVENTWORK* ev;
	D_NOHARA_WORK* wk = (D_NOHARA_WORK*)work;

	ev = SaveData_GetEventWork( wk->fsys->savedata );

	if( sys.cont & PAD_BUTTON_L ){
		SysFlag_Arrive( ev, SYSFLAG_MODE_RESET, FLAG_ARRIVE_D32R0101 );
	}else{
		SysFlag_Arrive( ev, SYSFLAG_MODE_SET, FLAG_ARRIVE_D32R0101 );
	}

	TcbBmpDel( tcb, work );		//TCBBMP�J��
	return;
}


//==============================================================================================
//
//	41	�X�e�[�W�A��������֘A
//
//==============================================================================================
static void D_Nohara_41_Main( TCB_PTR tcb, void * work );
static void StageRenshouSet( D_NOHARA_WORK* wk, u16 start_no, u16 end_no );

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_41( TCB_PTR tcb, void * work )
{
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;
	wk->seq		= 0;
	wk->count	= 0;
	wk->work	= 0;
	TCB_ChangeFunc( tcb, D_Nohara_41_Main );	//TCB�̓���֐��؂�ւ�
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_41_Main( TCB_PTR tcb, void * work )
{
	LOAD_RESULT load_result;
	SAVE_RESULT save_result;
	FRONTIER_EX_SAVEDATA* fes;
	int flag;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	flag = 0;	//���������t���O

	if( (sys.repeat == PAD_KEY_UP) || (sys.cont == PAD_KEY_RIGHT) ){
		flag = 1;
		wk->work++;
		if( wk->work > MONSNO_END ){
			wk->work = 1;
		}
	}

	if( (sys.repeat == PAD_KEY_DOWN) || (sys.cont == PAD_KEY_LEFT) ){
		flag = 1;
		wk->work--;
		if( wk->work < 1 ){
			wk->work = MONSNO_END;
		}
	}

	//���������t���O�������Ă�����
	if( flag == 1 ){
		//�w��͈͂�h��Ԃ�
		GF_BGL_BmpWinFill( &wk->bmpwin[0], FBMP_COL_BLACK, 8*7, 8*2, 
								8*8, 8*2 );
	
		//�����X�^�[�i���o�[�\��
		NumMsgSet( wk, &wk->bmpwin[0], wk->work, 8*7, 8*2 );
		GF_BGL_BmpWinOn( &wk->bmpwin[0] );
	}

	//�I��(1�C�Ώ�)
	if( sys.trg == PAD_BUTTON_A ){
		StageRenshouSet( wk, wk->work, (wk->work+1) );
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	}

	//�I��(10�C�Ώ�)
	if( sys.trg == PAD_BUTTON_X ){
		//StageRenshouSet( wk, wk->work, (wk->work+10) );			//MONSNO_END�`�F�b�N���K�v
		StageRenshouSet( wk, 1, (10+1) );
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	}

	//�I��(100�C�Ώ�)
	if( sys.trg == PAD_BUTTON_Y ){
		//StageRenshouSet( wk, wk->work, (wk->work+100) );			//MONSNO_END�`�F�b�N���K�v
		StageRenshouSet( wk, 1, (100+1) );
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	}

	//�I��(�S�đΏ�)
	if( sys.trg == PAD_BUTTON_START ){
		//�A���Z�E�X�͒���o���Ȃ����A�I�[���������͕|���̂ŁA�قڑS�Ẵ|�P�����ɂ��Ă���
		//StageRenshouSet( wk, 1, (MONSNO_END+1) );
		StageRenshouSet( wk, 1, MONSNO_END );
		TcbBmpDel( tcb, work );		//TCBBMP�J��
		return;
	}

	//�I��
	if( sys.cont == PAD_BUTTON_B ){
		TcbBmpDel( tcb, work );		//TCBBMP�J��
	}

	return;
}




static void StageRenshouSet( D_NOHARA_WORK* wk, u16 start_no, u16 end_no )
{
	LOAD_RESULT load_result;
	SAVE_RESULT save_result;
	FRONTIER_EX_SAVEDATA* fes;
	int flag,i;

	fes = FrontierEx_Load( wk->fsys->savedata, HEAPID_WORLD, &load_result );
	if( load_result != LOAD_RESULT_OK ){
		//
	}else{

		for( i=start_no; i < end_no ;i++ ){

			//Debug_FrontierEx_StageRenshou_Set( fes, FREXID_STAGE_RENSHOU_SINGLE, wk->work, 9999 );
			Debug_FrontierEx_StageRenshou_Set( fes, FREXID_STAGE_RENSHOU_SINGLE, i, 500 );

			//OS_Printf( "num = %d\n", FrontierEx_StageRenshou_Get(	wk->fsys->savedata,fes, 
			//														FREXID_STAGE_RENSHOU_SINGLE, 
			//														wk->work) );
		}	

#if 1
		//�t�B�[���h���샂�f���̃Z�[�u
		Field_SaveFieldObj( wk->fsys );

		// �|�P�b�`�֘A�f�[�^�̃Z�[�u
		Field_SendPoketchInfo( wk->fsys, POKETCH_SEND_SAVE, 0 );

		//�ʒu����������
		wk->fsys->location->grid_x = Player_NowGPosXGet( wk->fsys->player );
		wk->fsys->location->grid_z = Player_NowGPosZGet( wk->fsys->player );
		wk->fsys->location->door_id = DOOR_ID_JUMP_CODE;
		wk->fsys->location->dir = Player_DirGet( wk->fsys->player );
#endif

		//�O���Z�[�u���Ă΂�Ȃ��Ɣ��f���Ȃ��̂ŁA���̃f�o�b�N���������B�B�B
		save_result = FrontierEx_Save(wk->fsys->savedata, fes);
	}

	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}

	return;
}


//--------------------------------------------------------------
/**
 * @brief	�o�C�i�����b�Z�[�W�t�@�C���ǂݍ��݁A�\���e�X�g
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_89(void)
{
#if 1
	return;
#else
	static u16 * pMsg = NULL;

	//debug_msg.dat��cvs�ɓo�^���Ă��Ȃ��̂Œ��ӁI
	pMsg = (u16 *)sys_LoadFile( HEAPID_BASE_DEBUG, "/data/script/debug_msg.dat" );
	FieldTalkWinPut();
	msg_no_print( pMsg );
	FieldTalkMsgStart( pMsg, 1 );					//��2����=skip
	sys_FreeMemoryEz( pMsg );

	return;
#endif
}

//--------------------------------------------------------------
/**
 * @brief	�g���b�N�~���[�g�e�X�g
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_72(void)
{
	Snd_PlayerSetTrackMute( SND_HANDLE_FIELD, ( (1 << 0) | (2 << 0) ), TRUE );
	return;
}


//==============================================================================================
//
//	34	�t�����e�B�A�֘A(�A��������)
//
//==============================================================================================
static void D_Nohara_34_Main( TCB_PTR tcb, void * work );
static void D_Nohara_34_2_Main( TCB_PTR tcb, void * work );

enum{
	D34_PARAM_TOWER = 100,
	D34_PARAM_FACTORY,
	D34_PARAM_FACTORY_OPEN,
	D34_PARAM_STAGE,
	D34_PARAM_CASTLE,
	D34_PARAM_ROULETTE,
};

static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList34[] = {
	//�^���[�̃Z�b�g�͐F�X�Ⴄ�̂ŕۗ��B�B�B
	{ msg_debug_nohara_34_15, D34_PARAM_TOWER },		//�^���[
	{ msg_debug_nohara_34_02, D34_PARAM_FACTORY },		//�t�@�N�g���[
	{ msg_debug_nohara_36,	  D34_PARAM_FACTORY_OPEN },	//�t�@�N�g���[�I�[�v��
	{ msg_debug_nohara_34_03, D34_PARAM_STAGE },		//�X�e�[�W
	{ msg_debug_nohara_34_04, D34_PARAM_CASTLE },		//�L���b�X��
	{ msg_debug_nohara_34_05, D34_PARAM_ROULETTE },		//���[���b�g
};

//�}�C�i�X�ɂ������̍ő�A�����Z�b�g�l
static const u16 frontier_num_max[] = {
	9996,			//�^���[
	9996,			//�t�@�N�g���[
	9996,			//�t�@�N�g���[�I�[�v��
	9990,			//�X�e�[�W
	9996,			//�L���b�X��
	9996,			//���[���b�g
};

enum{
	D34_PARAM_SINGLE = 0,
	D34_PARAM_DOUBLE,
	D34_PARAM_MULTI,
	D34_PARAM_WIFI_MULTI,
};

static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList34_type[] = {
	{ msg_debug_nohara_34_11, D34_PARAM_SINGLE },		//�V���O��
	{ msg_debug_nohara_34_12, D34_PARAM_DOUBLE },		//�_�u��
	{ msg_debug_nohara_34_13, D34_PARAM_MULTI },		//�}���`
	{ msg_debug_nohara_34_14, D34_PARAM_WIFI_MULTI },	//WIFI�}���`
};

static const BMPLIST_HEADER ListH34 = {
	NULL,					//�\�������f�[�^�|�C���^
	NULL,					//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,					//GF_BGL_BMPWIN�̃|�C���^

	//NELEMS(DebugMenuList34_type),//���X�g���ڐ�
	1,						//���X�g���ڐ�
	10,						//�\���ő區�ڐ�

	0,						//���x���\���w���W
	8,						//���ڕ\���w���W
	0,						//�J�[�\���\���w���W
	0,						//�\���x���W

	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						//�����Ԋu�w
	16,						//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,		//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

//--------------------------------------------------------------
/**
 * @brief	������
 *
 * @param	none
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_34( TCB_PTR tcb, void * work )
{
	int i;
	BMPLIST_HEADER list_h;
	GF_BGL_BMPWIN* win;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	switch( wk->fro_seq ){

	//�{�ݑI��
	case 0:
		BmpDel( tcb );								//BMP�J��
		GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//BMP�ǉ�
		wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList34), HEAPID_BASE_DEBUG );
		for( i=0; i < NELEMS(DebugMenuList34); i++ ){
			BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
											DebugMenuList34[i].str_id, DebugMenuList34[i].param );
		}
		list_h			= ListH34;
		list_h.list		= wk->menulist;
		list_h.win		= &wk->bmpwin[0];
		list_h.count	= NELEMS(DebugMenuList34);
		wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );
		GF_BGL_BmpWinOn( &wk->bmpwin[0] );
		break;

	//�o�g���^�C�v�I��
	case 1:
		GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//BMP�ǉ�
		wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList34_type), HEAPID_BASE_DEBUG );
		for( i=0; i < NELEMS(DebugMenuList34_type); i++ ){
			BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
									DebugMenuList34_type[i].str_id, DebugMenuList34_type[i].param );
		}
		list_h			= ListH34;
		list_h.list		= wk->menulist;
		list_h.win		= &wk->bmpwin[0];
		list_h.count	= NELEMS(DebugMenuList34_type);
		wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );
		GF_BGL_BmpWinOn( &wk->bmpwin[0] );
		break;

	};

	wk->fro_seq++;
	TCB_ChangeFunc( tcb, D_Nohara_34_Main );	//TCB�̓���֐��؂�ւ�
	return;
}
 
//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_34_Main( TCB_PTR tcb, void * work )
{
	u32	ret;
	D_NOHARA_WORK* wk;
	wk = (D_NOHARA_WORK*)work;

	ret = BmpListMain( wk->lw );

	switch( ret ){

	case BMPLIST_NULL:
		break;

	case BMPLIST_CANCEL:
		TcbBmpDel( tcb, work );				//TCBBMP�J��
		break;

	////////////////////////////////////////////////////////////////////
	case D34_PARAM_TOWER:
		wk->fro_type = D34_PARAM_TOWER;
		wk->fro_add  = 7;
		BmpDel( tcb );						//BMP�J��
		TCB_ChangeFunc( tcb, D_Nohara_34 );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_FACTORY:
		wk->fro_type = D34_PARAM_FACTORY;
		wk->fro_add  = 7;
		BmpDel( tcb );						//BMP�J��
		TCB_ChangeFunc( tcb, D_Nohara_34 );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_FACTORY_OPEN:
		wk->fro_type = D34_PARAM_FACTORY_OPEN;
		wk->fro_add  = 7;
		BmpDel( tcb );						//BMP�J��
		TCB_ChangeFunc( tcb, D_Nohara_34 );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_STAGE:
		wk->fro_type = D34_PARAM_STAGE;
		wk->fro_add  = 10;
		BmpDel( tcb );						//BMP�J��
		TCB_ChangeFunc( tcb, D_Nohara_34 );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_CASTLE:
		wk->fro_type = D34_PARAM_CASTLE;
		wk->fro_add  = 7;
		BmpDel( tcb );						//BMP�J��
		TCB_ChangeFunc( tcb, D_Nohara_34 );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_ROULETTE:
		wk->fro_type = D34_PARAM_ROULETTE;
		wk->fro_add  = 7;
		BmpDel( tcb );						//BMP�J��
		TCB_ChangeFunc( tcb, D_Nohara_34 );	//TCB�̓���֐��؂�ւ�
		break;

	////////////////////////////////////////////////////////////////////
	case D34_PARAM_SINGLE:
		wk->btl_type = D34_PARAM_SINGLE;
		TCB_ChangeFunc( tcb, D_Nohara_34_2_Main );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_DOUBLE:
		wk->btl_type = D34_PARAM_DOUBLE;
		TCB_ChangeFunc( tcb, D_Nohara_34_2_Main );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_MULTI:
		wk->btl_type = D34_PARAM_MULTI;
		TCB_ChangeFunc( tcb, D_Nohara_34_2_Main );	//TCB�̓���֐��؂�ւ�
		break;

	case D34_PARAM_WIFI_MULTI:
		wk->btl_type = D34_PARAM_WIFI_MULTI;
		TCB_ChangeFunc( tcb, D_Nohara_34_2_Main );	//TCB�̓���֐��؂�ւ�
		break;

	};

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	tcb		TCB_PTR�^
 * @param	work	���[�N
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void D_Nohara_34_2_Main( TCB_PTR tcb, void * work )
{
	int i;
	u8 buf8[4];
	u16 num,type,l_num,h_num;
	u32	ret,flag;
	D_NOHARA_WORK* wk;
	POKEMON_PARAM* poke;
	FACTORYSCORE* fa_score_sv;
	STAGESCORE* s_score_sv;
	CASTLESCORE* ca_score_sv;
	ROULETTESCORE* r_score_sv;
	FRONTIER_SAVEWORK * fr_sv;

	wk = (D_NOHARA_WORK*)work;
	fr_sv = SaveData_GetFrontier( wk->fsys->savedata );

	flag = 0;		//���������t���O

	if( sys.trg == PAD_BUTTON_A ){

		switch( wk->fro_type ){
		////////////////////////////////////////////////////////////////////
		case D34_PARAM_TOWER:

			////////////////////
			//��WIFI�}���`����I
			////////////////////

			//"7�A��(�N���A)�������t���O"�����o��
			FrontierRecord_Set(	fr_sv, 
				FRID_TOWER_MULTI_WIFI_CLEAR_BIT,
				wk->friend_work, 1 );

			//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
			FrontierRecord_Set(	fr_sv, 
					FRID_TOWER_MULTI_WIFI_RENSHOU_CNT,
					wk->friend_work, wk->work );
			break;

		case D34_PARAM_FACTORY:
			fa_score_sv = SaveData_GetFactoryScore( wk->fsys->savedata );

			if( wk->btl_type == FACTORY_TYPE_WIFI_MULTI ){

				//"7�A��(�N���A)�������t���O"�����o��
				FrontierRecord_Set(	fr_sv, 
					FRID_FACTORY_MULTI_WIFI_CLEAR_BIT,
					wk->friend_work, 1 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
						FactoryScr_GetWinRecordID(0,wk->btl_type),
						wk->friend_work, wk->work );
			}else{

				//"7�A��(�N���A)�������t���O"�����o��
				buf8[0] = 1;
				FACTORYSCORE_PutScoreData(	fa_score_sv, FACTORYSCORE_ID_CLEAR_FLAG, 
											wk->btl_type, buf8 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
					FactoryScr_GetWinRecordID(0,wk->btl_type),	//LV50
					Frontier_GetFriendIndex(FactoryScr_GetWinRecordID(0,wk->btl_type)), wk->work );
			}
			break;

		case D34_PARAM_FACTORY_OPEN:
			fa_score_sv = SaveData_GetFactoryScore( wk->fsys->savedata );

			if( wk->btl_type == FACTORY_TYPE_WIFI_MULTI ){

				//"7�A��(�N���A)�������t���O"�����o��
				FrontierRecord_Set(	fr_sv, 
					FRID_FACTORY_MULTI_WIFI_CLEAR_BIT,
					wk->friend_work, 1 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
						FactoryScr_GetWinRecordID(1,wk->btl_type),
						wk->friend_work, wk->work );
			}else{

				//"7�A��(�N���A)�������t���O"�����o��
				buf8[0] = 1;
				FACTORYSCORE_PutScoreData(	fa_score_sv, FACTORYSCORE_ID_CLEAR_FLAG, 
											//wk->btl_type, buf8 );
											(1*FACTORY_TYPE_MAX)+wk->btl_type, buf8 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
					FactoryScr_GetWinRecordID(1,wk->btl_type),	//�I�[�v��
					Frontier_GetFriendIndex(FactoryScr_GetWinRecordID(1,wk->btl_type)), wk->work );
			}
			break;

		case D34_PARAM_STAGE:
			s_score_sv = SaveData_GetStageScore( wk->fsys->savedata );

			//�擪�̃|�P�����ɂ���
			poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(wk->fsys->savedata), 0 );

			if( wk->btl_type == STAGE_TYPE_WIFI_MULTI ){

				//"7�A��(�N���A)�������t���O"�����o��
				FrontierRecord_Set(	fr_sv, 
					FRID_STAGE_MULTI_WIFI_CLEAR_BIT,
					wk->friend_work, 1 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
					StageScr_GetWinRecordID(wk->btl_type),
					wk->friend_work, wk->work );

				//"�A�����̃|�P�����i���o�["�����o��
				FrontierRecord_Set(	fr_sv, 
						StageScr_GetMonsNoRecordID(wk->btl_type),
						wk->friend_work, 
						PokeParaGet(poke,ID_PARA_monsno,NULL) );
			}else{
				//"7�A��(�N���A)�������t���O"�����o��
				buf8[0] = 1;
				STAGESCORE_PutScoreData( s_score_sv, STAGESCORE_ID_CLEAR_FLAG, wk->btl_type,0,buf8);

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
					StageScr_GetWinRecordID(wk->btl_type),
					Frontier_GetFriendIndex(StageScr_GetWinRecordID(wk->btl_type)), wk->work );

				//"�A�����̃|�P�����i���o�["�����o��
				FrontierRecord_Set(	fr_sv, 
						StageScr_GetMonsNoRecordID(wk->btl_type),
						Frontier_GetFriendIndex(StageScr_GetMonsNoRecordID(wk->btl_type)), 
						PokeParaGet(poke,ID_PARA_monsno,NULL) );

#if 1
				//�u???�v�͔�����
				//"�^�C�v���Ƃ̃��x��"����������
				for( i=0; i < (STAGE_TR_TYPE_MAX-1) ;i++ ){
					//StageScr_TypeLevelRecordSet(wk->fsys->savedata, wk->btl_type, 
					Debug_StageScr_TypeLevelRecordSet(	wk->fsys->savedata, wk->btl_type, i );
				}
#endif
			}

#if 0 
			//"�A�����̃|�P�����i���o�["�����o��
			FrontierRecord_Set(	fr_sv, 
						StageScr_GetMonsNoRecordID(wk->btl_type),
						Frontier_GetFriendIndex(StageScr_GetMonsNoRecordID(wk->btl_type)), 
						0 );

			//������0���Z�b�g���Ă��Ad32r0401.ev�̗���ŁA
			//�N���A���̎��́A�����|�P�����Œ��킵�Ă��邩�`�F�b�N�������Ă��܂��A
			//��񂪃N���A����Ă��܂��B
#endif

			break;

		case D34_PARAM_CASTLE:
			ca_score_sv = SaveData_GetCastleScore( wk->fsys->savedata );
		
			if( wk->btl_type == CASTLE_TYPE_WIFI_MULTI ){

				//"7�A��(�N���A)�������t���O"�����o��
				FrontierRecord_Set(	fr_sv, 
					FRID_CASTLE_MULTI_WIFI_CLEAR_BIT,
					wk->friend_work, 1 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
						CastleScr_GetWinRecordID(wk->btl_type),
						wk->friend_work, wk->work );
			}else{

				//"7�A��(�N���A)�������t���O"�����o��
				buf8[0] = 1;
				CASTLESCORE_PutScoreData(	ca_score_sv, CASTLESCORE_ID_CLEAR_FLAG, 
											wk->btl_type, 0, buf8 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
						CastleScr_GetWinRecordID(wk->btl_type),
						Frontier_GetFriendIndex(CastleScr_GetWinRecordID(wk->btl_type)), wk->work );
			}
			break;

		case D34_PARAM_ROULETTE:
			r_score_sv = SaveData_GetRouletteScore( wk->fsys->savedata );

			if( wk->btl_type == ROULETTE_TYPE_WIFI_MULTI ){

				//"7�A��(�N���A)�������t���O"�����o��
				FrontierRecord_Set(	fr_sv, 
					FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,
					wk->friend_work, 1 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
					RouletteScr_GetWinRecordID(wk->btl_type),
					wk->friend_work, wk->work );
			}else{

				//"7�A��(�N���A)�������t���O"�����o��
				buf8[0] = 1;
				ROULETTESCORE_PutScoreData( r_score_sv, ROULETTESCORE_ID_CLEAR_FLAG, 
											wk->btl_type, 0, buf8 );

				//"�A����"�����o��(�u����27�l�ڂł��v�Ƃ����悤�Ɏg��)
				FrontierRecord_Set(	fr_sv, 
					RouletteScr_GetWinRecordID(wk->btl_type),
					Frontier_GetFriendIndex(RouletteScr_GetWinRecordID(wk->btl_type)), wk->work );
			}
			break;
		}

		return;
	};

	//R�{�^���������Ă���ƗF�B��I��
	if( sys.cont & PAD_BUTTON_R ){

		if( (sys.repeat & PAD_KEY_UP) || (sys.cont & PAD_KEY_RIGHT) ){
			flag = 1;

			wk->friend_work+=1;

			if( wk->friend_work >= 32 ){
				wk->friend_work = 0;
			}
		}

		if( (sys.repeat & PAD_KEY_DOWN) || (sys.cont & PAD_KEY_LEFT) ){
			flag = 1;
	
			wk->friend_work-=1;

			//�}�C�i�X�ɂ������̍ő�A�����Z�b�g�l
			if( wk->friend_work < 0 ){
				wk->friend_work = 31;
			}
		}

	}else{

		if( (sys.repeat & PAD_KEY_UP) || (sys.cont & PAD_KEY_RIGHT) ){
			flag = 1;

			if( sys.cont & PAD_BUTTON_L ){
				wk->work+=1;
			}else{
				wk->work+=wk->fro_add;
			}

			if( wk->work >= 9999 ){
				wk->work = 0;
			}
		}

		if( (sys.repeat & PAD_KEY_DOWN) || (sys.cont & PAD_KEY_LEFT) ){
			flag = 1;
	
			if( sys.cont & PAD_BUTTON_L ){
				wk->work-=1;
			}else{
				wk->work-=wk->fro_add;
			}

			//�}�C�i�X�ɂ������̍ő�A�����Z�b�g�l
			if( wk->work < 0 ){
				//wk->work = (9999 - 1);
				wk->work = frontier_num_max[ (wk->fro_type - D34_PARAM_TOWER) ];
			}
		}
	}

	//���������t���O�������Ă�����
	if( flag == 1 ){
		//�w��͈͂�h��Ԃ�
		//GF_BGL_BmpWinFill( &wk->bmpwin[0], FBMP_COL_BLACK, 8*11, 0, 8*6, 8*2 );
		GF_BGL_BmpWinFill( &wk->bmpwin[0], FBMP_COL_BLACK, 8*11, 0, 8*6, 8*4 );

		//���l�\��
		NumMsgSet( wk, &wk->bmpwin[0], wk->work, 8*11, 2 );

		//���l�\��(�F�B)
		NumMsgSet( wk, &wk->bmpwin[0], wk->friend_work, 8*11, 8*2+2 );

		GF_BGL_BmpWinOn( &wk->bmpwin[0] );
	}

	//�I��
	if( sys.cont == PAD_BUTTON_B ){
		TcbBmpDel( tcb, work );				//TCBBMP�J��
	}

	return;
}








//--------------------------------------------------------------------------------------------
/**
 * �^�C�v���x���̃��R�[�h���Z�b�g(fssc_stage_sub.c�ɂ���֐��ŌĂׂȂ��̂ŃR�s�y�ł����Ă���)
 *
 * @param	wk
 *
 * @return	"���R�[�h"
 *
 */
//--------------------------------------------------------------------------------------------
static void Debug_StageScr_TypeLevelRecordSet( SAVEDATA* sv, u8 type, u8 csr_pos )
{
	u8 offset;
	u8 param;
	u8 set_num,total;
	u16 l_num,h_num;

	//total = 0xaa;		//10101010(10����ς�)
	//total = 0x99;		//10011001(10����O)
	//total = 0x88;		//10001000(9)
	total = 0x00;		//00000000(0)

#if 0
	switch( csr_pos ){
	case 0:
		total = 0x01;
		break;

	case 1:
		total = 0x01;
		break;

	case 2:
		total = 0x23;
		break;

	case 3:
		total = 0x23;
		break;

	case 4:
		total = 0x45;
		break;

	case 5:
		total = 0x45;
		break;

	case 6:
		total = 0x67;
		break;

	case 7:
		total = 0x67;
		break;

	case 8:
		total = 0x89;
		break;

	case 9:
		total = 0x89;
		break;

	};
#endif

	FrontierRecord_Set(	SaveData_GetFrontier(sv), 
					StageScr_GetTypeLevelRecordID(type,csr_pos),
					Frontier_GetFriendIndex(StageScr_GetTypeLevelRecordID(type,csr_pos)), total );
	return;
}










//==============================================================================================
//
//
//
//==============================================================================================

static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList2[] = {
	{ msg_debug_nohara_25, (u32)D_Nohara_25 },
	{ msg_debug_nohara_26, (u32)D_Nohara_26 },
};

static const BMPLIST_HEADER DebugListH2 = {
	NULL,					//�\�������f�[�^�|�C���^

	NULL,					//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,					//GF_BGL_BMPWIN�̃|�C���^

	NELEMS(DebugMenuList2),	//���X�g���ڐ�
	9,						//�\���ő區�ڐ�

	0,						//���x���\���w���W
	8,						//���ڕ\���w���W
	0,						//�J�[�\���\���w���W
	0,						//�\���x���W
/*
	FBMP_COL_BLACK,			//�����F
	FBMP_COL_WHITE,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
*/
	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						//�����Ԋu�w
	16,						//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,		//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

//--------------------------------------------------------------
/**
 * @brief	BGM�f�o�b�N���j���[�Ăяo��(fld_debug.c)
 *
 * @param	fsys	FIELDSYS_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void DebugBgmMenuInit( FIELDSYS_WORK* fsys )
{
	int i;
	BMPLIST_HEADER list_h;
	D_NOHARA_WORK* wk;

	wk = (D_NOHARA_WORK*)TCB_GetWork( PMDS_taskAdd(DebugNoharaMenuMain, sizeof(D_NOHARA_WORK), 0, HEAPID_BASE_DEBUG) );

	wk->seq				= 0;
	wk->sel				= 0;
	wk->work			= 0;
	wk->wave_buf_flag	= 0;								//�g�`�i�[�o�b�t�@���m�ۂ����t���OOFF
	wk->waveout_flag	= 0;								//�����Đ����t���OOFF
	wk->fsys			= fsys;

	GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//�r�b�g�}�b�v�ǉ�

	wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList2), HEAPID_BASE_DEBUG );

	wk->wordset = WORDSET_Create( HEAPID_BASE_DEBUG );

	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, 
								NARC_msg_debug_nohara_dat, HEAPID_BASE_DEBUG);

	for( i=0; i < NELEMS(DebugMenuList2); i++ ){
		BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
										DebugMenuList2[i].str_id, DebugMenuList2[i].param );
	}

	list_h			= DebugListH2;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[0];
	wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );

	GF_BGL_BmpWinOn( &wk->bmpwin[0] );
	return;
}


//==============================================================================================
//
//	�f�o�b�N�u�̂͂�v�ł͂Ȃ�������́u�f�o�b�N�X�N���v�g�v�Ăяo��
//
//==============================================================================================

static const struct{
	u32  str_id;
	u32  param;
}DebugMenuList3[] = {
	{ msg_debug_nohara_11, (u32)D_Nohara_11 },
};

static const BMPLIST_HEADER DebugListH3 = {
	NULL,					//�\�������f�[�^�|�C���^

	NULL,					//�J�[�\���ړ����Ƃ̃R�[���o�b�N�֐�
	NULL,					//���\�����Ƃ̃R�[���o�b�N�֐�

	NULL,					//GF_BGL_BMPWIN�̃|�C���^

	NELEMS(DebugMenuList3),	//���X�g���ڐ�
	9,						//�\���ő區�ڐ�

	0,						//���x���\���w���W
	8,						//���ڕ\���w���W
	0,						//�J�[�\���\���w���W
	0,						//�\���x���W
/*
	FBMP_COL_BLACK,			//�����F
	FBMP_COL_WHITE,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
*/
	FBMP_COL_WHITE,			//�����F
	FBMP_COL_BLACK,			//�w�i�F
	FBMP_COL_BLK_SDW,		//�����e�F
	0,						//�����Ԋu�w
	16,						//�����Ԋu�x
	BMPLIST_LRKEY_SKIP,		//�y�[�W�X�L�b�v�^�C�v
	FONT_SYSTEM,			//�����w��(�{���� u8 �����ǁA����Ȃɍ��Ȃ��Ǝv���̂�)
	0						//�a�f�J�[�\��(allow)�\���t���O(0:ON,1:OFF)
};

//--------------------------------------------------------------
/**
 * @brief	BGM�f�o�b�N���j���[�Ăяo��(fld_debug.c)
 *
 * @param	fsys	FIELDSYS_WORK�^�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void DebugScriptMenuInit( FIELDSYS_WORK* fsys )
{
	int i;
	BMPLIST_HEADER list_h;
	D_NOHARA_WORK* wk;

	wk = (D_NOHARA_WORK*)TCB_GetWork( PMDS_taskAdd(DebugNoharaMenuMain, sizeof(D_NOHARA_WORK), 0, HEAPID_BASE_DEBUG) );

	wk->seq				= 0;
	wk->sel				= 0;
	wk->work			= 0;
	wk->wave_buf_flag	= 0;								//�g�`�i�[�o�b�t�@���m�ۂ����t���OOFF
	wk->waveout_flag	= 0;								//�����Đ����t���OOFF
	wk->fsys			= fsys;

	GF_BGL_BmpWinAddEx( wk->fsys->bgl, &wk->bmpwin[0], &DebugNoharaWinData );	//�r�b�g�}�b�v�ǉ�

	wk->menulist = BMP_MENULIST_Create( NELEMS(DebugMenuList3), HEAPID_BASE_DEBUG );

	wk->wordset = WORDSET_Create( HEAPID_BASE_DEBUG );

	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_NORMAL, ARC_MSG, 
								NARC_msg_debug_nohara_dat, HEAPID_BASE_DEBUG);

	for( i=0; i < NELEMS(DebugMenuList3); i++ ){
		BMP_MENULIST_AddArchiveString( wk->menulist, wk->msgman, 
										DebugMenuList3[i].str_id, DebugMenuList3[i].param );
	}

	list_h			= DebugListH3;
	list_h.list		= wk->menulist;
	list_h.win		= &wk->bmpwin[0];
	wk->lw			= BmpListSet( &list_h, 0, 0, HEAPID_BASE_DEBUG );

	GF_BGL_BmpWinOn( &wk->bmpwin[0] );
	return;
}

#include "savedata/wifilist.h"
//--------------------------------------------------------------
/**
 * @brief	�F�B�蒠�̃J���X�g�`�F�b�N�p����
 *
 * @param	savedata	SAVEDATA�^�̃|�C���^
 *
 * @retval	none
 *
 * �����F���̊֐����Ă΂�鏈���ŁA
 *       �F�B��MAX�ɂ��鏈���́A1�l�F�B�����Ȃ��Ƌ@�\���Ȃ�
 */
//--------------------------------------------------------------
void Debug_FrontierWifiMultiRecordMaxSet( FIELDSYS_WORK* fsys );
void Debug_FrontierWifiMultiRecordMaxSet( FIELDSYS_WORK* fsys )
{
#ifdef PM_DEBUG
	int i;
	//LOAD_RESULT load_result;
	//SAVE_RESULT save_result;
	POKEMON_PARAM* poke;
	FACTORYSCORE* fa_score_sv;
	STAGESCORE* s_score_sv;
	CASTLESCORE* ca_score_sv;
	ROULETTESCORE* r_score_sv;
	FRONTIER_SAVEWORK * fr_sv;
	SAVEDATA* savedata;
	//FRONTIER_EX_SAVEDATA* fes;

	savedata    = fsys->savedata;
	fr_sv		= SaveData_GetFrontier( savedata );
	fa_score_sv = SaveData_GetFactoryScore( savedata );
	fa_score_sv = SaveData_GetFactoryScore( savedata );
	s_score_sv	= SaveData_GetStageScore( savedata );
	ca_score_sv = SaveData_GetCastleScore( savedata );
	r_score_sv	= SaveData_GetRouletteScore( savedata );

	for( i=0; i < WIFILIST_FRIEND_MAX; i++ ){

		////////////////////
		//�^���[
		////////////////////

		//"7�A��(�N���A)�������t���O"�����o��
		FrontierRecord_Set(	fr_sv, FRID_TOWER_MULTI_WIFI_CLEAR_BIT, i, 1 );

		//"�A����"�����o��
		FrontierRecord_Set(	fr_sv, FRID_TOWER_MULTI_WIFI_RENSHOU_CNT, i, 9999 );

		//"�ō��A����"�����o��
		FrontierRecord_Set(	fr_sv, FRID_TOWER_MULTI_WIFI_RENSHOU, i, 9999 );

		////////////////////
		//�t�@�N�g���[(LV50)
		////////////////////

		//"7�A��(�N���A)�������t���O"�����o��
		FrontierRecord_Set(	fr_sv, FRID_FACTORY_MULTI_WIFI_CLEAR_BIT, i, 1 );

		//"�A����"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetWinRecordID(0,FACTORY_TYPE_WIFI_MULTI), 
							i, FACTORY_RENSYOU_MAX );

		//"�ō��A����"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetMaxWinRecordID(0,FACTORY_TYPE_WIFI_MULTI), 
							i, FACTORY_RENSYOU_MAX );

		//"������"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetTradeRecordID(0,FACTORY_TYPE_WIFI_MULTI),
							i, FACTORY_TRADE_MAX );

		//"�ő������"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetMaxTradeRecordID(0,FACTORY_TYPE_WIFI_MULTI),
							i, FACTORY_TRADE_MAX );

		////////////////////
		//�t�@�N�g���[(LV100)
		////////////////////

		//"7�A��(�N���A)�������t���O"�����o��
		FrontierRecord_Set(	fr_sv, FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT, i, 1 );

		//"�A����"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetWinRecordID(1,FACTORY_TYPE_WIFI_MULTI),
							i, FACTORY_RENSYOU_MAX );

		//"�ō��A����"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetMaxWinRecordID(1,FACTORY_TYPE_WIFI_MULTI),
							i, FACTORY_RENSYOU_MAX );

		//"������"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetTradeRecordID(1,FACTORY_TYPE_WIFI_MULTI),
							i, FACTORY_TRADE_MAX );

		//"�ő������"�����o��
		FrontierRecord_Set(	fr_sv, FactoryScr_GetMaxTradeRecordID(1,FACTORY_TYPE_WIFI_MULTI),
							i, FACTORY_TRADE_MAX );

		////////////////////
		//�X�e�[�W
		////////////////////

		//�擪�̃|�P�����ɂ���
		poke = PokeParty_GetMemberPointer( SaveData_GetTemotiPokemon(savedata), 0 );

		//"7�A��(�N���A)�������t���O"�����o��
		FrontierRecord_Set(	fr_sv, FRID_STAGE_MULTI_WIFI_CLEAR_BIT, i, 1 );

		//"�A����"�����o��
		FrontierRecord_Set(	fr_sv, StageScr_GetWinRecordID(STAGE_TYPE_WIFI_MULTI),
							i, STAGE_RENSYOU_MAX );

		//"�ō��A����"�����o��
		FrontierRecord_Set(	fr_sv, StageScr_GetMaxWinRecordID(STAGE_TYPE_WIFI_MULTI),
							i, STAGE_RENSYOU_MAX );

		//"�A�����̃|�P�����i���o�["�����o��
		FrontierRecord_Set(	fr_sv, StageScr_GetMonsNoRecordID(STAGE_TYPE_WIFI_MULTI),
							i, PokeParaGet(poke,ID_PARA_monsno,NULL) );

		////////////////////
		//�L���b�X��
		////////////////////
		
		//"7�A��(�N���A)�������t���O"�����o��
		FrontierRecord_Set(	fr_sv, FRID_CASTLE_MULTI_WIFI_CLEAR_BIT, i, 1 );

		//"�A����"�����o��
		FrontierRecord_Set(	fr_sv, CastleScr_GetWinRecordID(CASTLE_TYPE_WIFI_MULTI),
							i, CASTLE_RENSYOU_MAX );

		//"�ō��A����"�����o��
		FrontierRecord_Set(	fr_sv, CastleScr_GetMaxWinRecordID(CASTLE_TYPE_WIFI_MULTI),
							i, CASTLE_RENSYOU_MAX );

		//"CP"�����o��
		FrontierRecord_Set(	fr_sv, CastleScr_GetCPRecordID(CASTLE_TYPE_WIFI_MULTI),
							i, CASTLE_CP_MAX );

		//"�ō�CP"�����o��
		FrontierRecord_Set(	fr_sv, CastleScr_GetRemainderCPRecordID(CASTLE_TYPE_WIFI_MULTI),
							i, CASTLE_CP_MAX );

		////////////////////
		//���[���b�g
		////////////////////

		//"7�A��(�N���A)�������t���O"�����o��
		FrontierRecord_Set(	fr_sv, FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT, i, 1 );

		//"�A����"�����o��
		FrontierRecord_Set(	fr_sv, RouletteScr_GetWinRecordID(ROULETTE_TYPE_WIFI_MULTI),
							i, ROULETTE_RENSYOU_MAX );

		//"�ō��A����"�����o��
		FrontierRecord_Set(	fr_sv, RouletteScr_GetMaxWinRecordID(ROULETTE_TYPE_WIFI_MULTI),
							i, ROULETTE_RENSYOU_MAX );
	}
#endif

#if 0
	//�X�e�[�W"�ō��A�����̃|�P�����i���o�["�����o��

	fes = FrontierEx_Load( savedata, HEAPID_WORLD, &load_result );
	if( load_result != LOAD_RESULT_OK ){
		//
	}else{

		Debug_FrontierEx_StageRenshou_Set( fes, FREXID_STAGE_RENSHOU_SINGLE, i, 500 );

#if 1
		//�t�B�[���h���샂�f���̃Z�[�u
		Field_SaveFieldObj( fsys );

		// �|�P�b�`�֘A�f�[�^�̃Z�[�u
		Field_SendPoketchInfo( fsys, POKETCH_SEND_SAVE, 0 );

		//�ʒu����������
		fsys->location->grid_x	= Player_NowGPosXGet( fsys->player );
		fsys->location->grid_z	= Player_NowGPosZGet( fsys->player );
		fsys->location->door_id = DOOR_ID_JUMP_CODE;
		fsys->location->dir		= Player_DirGet( fsys->player );
#endif

		//�O���Z�[�u���Ă΂�Ȃ��Ɣ��f���Ȃ�
		save_result = FrontierEx_Save( savedata, fes );
	}

	if( fes != NULL ){
		sys_FreeMemoryEz(fes);
	}
#endif

	return;
}


