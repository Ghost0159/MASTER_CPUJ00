//==============================================================================================
/**
 * @file	scratch.c
 * @brief	�u�X�N���b�`�v���C���\�[�X
 * @author	Satoshi Nohara
 * @date	07.12.11
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
#include "system/touch_subwindow.h"
#include "system/msg_ds_icon.h"
#include "gflib/strbuf_family.h"
#include "savedata/config.h"
#include "application/p_status.h"
#include "poketool/pokeparty.h"
#include "poketool/poke_number.h"
#include "poketool/pokeicon.h"
#include "itemtool/itemsym.h"

#include "scratch_snd_def.h"
#include "scratch_sys.h"
#include "scratch_clact.h"
#include "scratch_bmp.h"
#include "scratch_obj.h"
#include "scratch.naix"
#include "application/scratch.h"

#include "msgdata\msg.naix"						//NARC_msg_??_dat
#include "msgdata\msg_scratch_contents.h"		//msg_??

#include "gflib/touchpanel.h"
#include "../../system/touchpanel_draw.h"


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

	SEQ_GAME_CARD_IN,										//�J�[�h�C��
	SEQ_GAME_CARD_SEL,										//�J�[�h�I��
	SEQ_GAME_CARD_OUT,										//�J�[�h�A�E�g

	SEQ_GAME_CARD_TOUCH_IN,									//�J�[�h���C��
	SEQ_GAME_CARD_TOUCH,									//�J�[�h��蒆
	SEQ_GAME_CARD_TOUCH_OUT,								//�J�[�h���A�E�g

	SEQ_GAME_END,											//�I��
};

//�t�H���g�J���[
#define	COL_BLUE	( GF_PRINTCOLOR_MAKE(FBMP_COL_BLUE,FBMP_COL_BLU_SDW,FBMP_COL_NULL) )//��
#define	COL_RED		( GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_NULL) )	//��

#define SCRATCH_END_WAIT		(10)				//�I������܂ł̕\���E�F�C�g

#define SCRATCH_TP_NOT			(0xffff)			//�^�b�`���Ă��Ȃ����̒�`

//���N�̃p���b�g�؂�ւ�
//OS_Printf( "pal_no = %d\n", ScratchObj_PaletteNoGet( wk->p_d_waku[0] ) );
#define WAKU_PAL_END			(10)
#define WAKU_PAL_WAIT			(1)

//#define CENTER_ATARI_NO			(4)					//�����肪�K���z�u�����ʒu

#define SCRATCH_CARD_MODE_EASY	(0)		//�C�[�W�J�[�h

#define SCRATCH_CARD_POKE_MAX	(5)		//�J�[�h�̒��g�̃|�P�����̎�ނ̍ő吔

#define SCRATCH_EASY_ATARI_NUM	(4)		//�J�[�h�ɔz�u����铖����|�P�����̐���
//#define SCRATCH_EASY_ATARI_NUM	(5)		//�J�[�h�ɔz�u����铖����|�P�����̐���

#define SCRATCH_CARD_KEZURI_NUM	(3)		//�J�[�h�����鐔��
#define SCRATCH_CARD_ATARI_NUM	(3)		//������ɕK�v�ȑ�����|�P�����̐�

#define SCRATCH_MOSAIC_MAX		(4)		//���U�C�N�̍ő啝


//==============================================================================================
//
//	OBJ�z�u�f�[�^
//
//	�|�P�G��(5x5)
//
//==============================================================================================
//��F�|�P�����z�u�ʒu
#define SCRATCH_U_POKE_X		(60)
#define SCRATCH_U_POKE_Y		(26)
#define SCRATCH_U_POKE_WIDTH_X	(41)
#define SCRATCH_U_POKE_WIDTH_Y	(42)

//���F������|�P�����z�u�ʒu
static const SCRATCH_POS d_atari_pos[SCRATCH_CARD_ATARI_NUM] = {
	{ 52, 68 },	{ 108, 68 }, { 164, 68 }, 
};

//���F������E�B���h�E�z�u�ʒu
static const SCRATCH_POS d_atari_win_pos = { 38, 56 };

//���F�|�P�����z�u�ʒu
static const SCRATCH_POS d_poke_pos[SCRATCH_D_POKE_MAX] = {
#if 0
	{ 52, 18 },	{ 124, 18 },	{ 196, 18 }, 
	{ 52, 66 },	{ 124, 66 },	{ 196, 66 }, 
	{ 52, 114 },{ 124, 114 },	{ 196, 114 }, 
#else
	//+5						//-5
	{ 57, 18 },	{ 124, 18 },	{ 191, 18 }, 
	{ 57, 66 },	{ 124, 66 },	{ 191, 66 }, 
	{ 57, 114 },{ 124, 114 },	{ 191, 114 }, 
#endif
};

//�┓�̃��N�̕\���I�t�Z�b�g(���F�|�P�����z�u�ɑ�������)
//#define SCRATCH_GIN_WAKU_OFFSET_X	(-20)
#define SCRATCH_GIN_WAKU_OFFSET_X	(-36)
#define SCRATCH_GIN_WAKU_OFFSET_Y	(-10)

//���F�J�[�h�ʒu
static const SCRATCH_POS d_card_pos[SCRATCH_D_CARD_MAX] = {
	{ 0, 36 },	{ 56, 36 }, { 112, 36 },	{ 168, 36 },
};

//���F�{�^���z�u�ʒu
static const SCRATCH_POS d_next_button_pos = { 68, 160 };
static const SCRATCH_POS d_yameru_button_pos = { 88, 160 };

//���F�^�񒆃E�B���h�E�z�u�ʒu
//static const SCRATCH_POS d_win_pos = { 68, 64 };
static const SCRATCH_POS d_win_pos = { 68, 68 };

//�g�傷�鎞�̖ڕW�ʒu
//static const SCRATCH_POS card_center_pos = { 4, 8 };
//static const SCRATCH_POS card_center_pos = { 16, 4 };
//static const SCRATCH_POS card_center_pos = { 48, 4 };
static const SCRATCH_POS card_center_pos = { 64, 4 };

//#define SCRATCH_CARD_IN_OFFSET_X	(-132)			//�J�[�h���C�����鎞�̕\���I�t�Z�b�g
#define SCRATCH_CARD_IN_OFFSET_X	(-256)			//�J�[�h���C�����鎞�̕\���I�t�Z�b�g
//#define SCRATCH_CARD_IN_SPD		(4)				//�J�[�h���C������X�s�[�h
//#define SCRATCH_CARD_IN_SPD			(8)				//�J�[�h���C������X�s�[�h
#define SCRATCH_CARD_IN_SPD			(32)				//�J�[�h���C������X�s�[�h
//#define SCRATCH_CARD_IN_SPD			(32)				//�J�[�h���C������X�s�[�h
#define SCRATCH_CARD_IN_SPD2		(16)				//�J�[�h���C������X�s�[�h

//#define SCRATCH_CARD_OUT_SPD		(4)				//�J�[�h���A�E�g����X�s�[�h
//#define SCRATCH_CARD_OUT_SPD		(8)				//�J�[�h���A�E�g����X�s�[�h
#define SCRATCH_CARD_OUT_SPD		(16)				//�J�[�h���A�E�g����X�s�[�h
//#define SCRATCH_CARD_OUT_OFFSET_X	(200)			//�J�[�h���A�E�g���鎞�̕\���I�t�Z�b�g
#define SCRATCH_CARD_OUT_OFFSET_X	(256)			//�J�[�h���A�E�g���鎞�̕\���I�t�Z�b�g

#define SCRATCH_CARD_AFF_WAIT		(1)				//�J�[�h���g�傷��E�F�C�g
//#define SCRATCH_CARD_AFF_MAX		(0x2000)		//�J�[�h�̍ő�g��
//#define SCRATCH_CARD_AFF_MAX		(0x1e00)		//�J�[�h�̍ő�g��
//#define SCRATCH_CARD_AFF_MAX		(0x1b00)		//�J�[�h�̍ő�g��
//#define SCRATCH_CARD_AFF_MAX		(0x1a00)		//�J�[�h�̍ő�g��
//#define SCRATCH_CARD_AFF_MAX		(0x1900)		//�J�[�h�̍ő�g��
#define SCRATCH_CARD_AFF_MAX		(0x1800)		//�J�[�h�̍ő�g��
#define SCRATCH_CARD_AFF_ADD		(0x0100)		//�J�[�h�̊g��
#define SCRATCH_CARD_AFF_DEFAULT	(0x1000)		//�J�[�h�̏����{��(���{)

//���
//#define SCRATCH_CARD_TOUCH_IN_OFFSET_X	(-256)	//���J�[�h���C�����鎞�̕\���I�t�Z�b�g
#define SCRATCH_CARD_TOUCH_IN_OFFSET_X	(256)		//���J�[�h���C�����鎞�̕\���I�t�Z�b�g
//#define SCRATCH_CARD_TOUCH_IN_SPD		(-8)		//���J�[�h���C������X�s�[�h
#define SCRATCH_CARD_TOUCH_IN_SPD		(-16)		//���J�[�h���C������X�s�[�h
//#define SCRATCH_CARD_TOUCH_OUT_SPD		(-8)		//���J�[�h���A�E�g����X�s�[�h
#define SCRATCH_CARD_TOUCH_OUT_SPD		(-16)		//���J�[�h���A�E�g����X�s�[�h
//#define SCRATCH_CARD_TOUCH_OUT_SPD		(8)		//���J�[�h���A�E�g����X�s�[�h
#define SCRATCH_CARD_TOUCH_OUT_OFFSET_X	(-256)		//�J�[�h���A�E�g���鎞�̕\���I�t�Z�b�g

//�J�[�h�̐F�i���o�[
enum{
	CARD_BLUE = 0,		//����
	CARD_RED,			//�E��
	CARD_YELLOW,		//����
	CARD_GREEN,			//�E��
};

//#define CARD_CENTER_SPD_X		(8)		//�J�[�h�𒆐S�ɂ��鑬�x
//#define CARD_CENTER_SPD_Y		(8)
//#define CARD_CENTER_SPD_X		(16)	//�J�[�h�𒆐S�ɂ��鑬�x
//#define CARD_CENTER_SPD_Y		(16)
//#define CARD_CENTER_SPD_X		(16)	//�J�[�h�𒆐S�ɂ��鑬�x
//#define CARD_CENTER_SPD_Y		(12)
#define CARD_CENTER_SPD_X		(24)	//�J�[�h�𒆐S�ɂ��鑬�x
#define CARD_CENTER_SPD_Y		(16)

//�_�~�[�i���o�[
enum{
	DUMMY_CARD_POKE = 0xb0,
	DUMMY_KEZURI_CARD1,
	DUMMY_KEZURI_CARD2,
	DUMMY_KEZURI_CARD3,
};


//==============================================================================================
//
//	�Ⴆ��؂̎�
//
//==============================================================================================
//�C�[�W�[�J�[�h
static const u16 scratch_easy_item[] = {
	ITEM_ZAROKUNOMI,
	ITEM_NEKOBUNOMI,
	ITEM_TAPORUNOMI,
	ITEM_ROMENOMI,
	ITEM_UBUNOMI,
	ITEM_MATOMANOMI,
	////////////////
	ITEM_OKKANOMI,
	ITEM_ITOKENOMI,
	ITEM_SOKUNONOMI,
	ITEM_RINDONOMI,
	ITEM_YATHENOMI,
	ITEM_YOPUNOMI,
	ITEM_BIAANOMI,
	ITEM_SYUKANOMI,
	ITEM_BAKOUNOMI,
	ITEM_UTANNOMI,
	ITEM_TANGANOMI,
	ITEM_YOROGINOMI,
	ITEM_KASIBUNOMI,
	ITEM_HABANNOMI,
	ITEM_NAMONOMI,
	ITEM_RIRIBANOMI,
	ITEM_HOZUNOMI,
};
#define SCRATCH_EASY_ITEM	( NELEMS(scratch_easy_item) )


//==============================================================================================
//
//	�J�[�h�̃^�b�`����
//
//==============================================================================================
static const RECT_HIT_TBL card_hit_tbl[SCRATCH_D_CARD_MAX+1] = {
	//top,bottom,left,right
	{ 42,	154,	0,		80 },			//��ԍ�
	{ 42,	154,	88,		138 },			//
	{ 42,	154,	144,	195 },			//
	{ 42,	154,	204,	254 },			//��ԉE
	{ TP_HIT_END,0,0,0 },					//�I���f�[�^
};

#define SCRATCH_START_MSG_DEL_WAIT		( 30 )			//�X�N���b�`�X�^�[�g�������܂ł̃E�F�C�g
#define SCRATCH_RESULT_MSG_DEL_WAIT		( 60 )			//�X�N���b�`���ʂ������܂ł̃E�F�C�g
#define SCRATCH_GIN_ALL_CLEAR_WAIT		( 30 )			//�┓��S�ď�������Ԃ̃E�F�C�g
#define SCRATCH_RESULT_MOSAIC_WAIT		( 30 )			//���^�����̃��U�C�N���n�܂�܂ł̃E�F�C�g


//==============================================================================================
//
//	���̃J�[�h�ցE��߂�{�^���̃^�b�`����
//
//==============================================================================================
static const RECT_HIT_TBL next_hit_tbl[] = {
	{ 164,	191,	76,		180 },			//
	{ TP_HIT_END,0,0,0 },					//�I���f�[�^
};

static const RECT_HIT_TBL yameru_hit_tbl[] = {
	{ 164,	191,	94,		164 },			//
	{ TP_HIT_END,0,0,0 },					//�I���f�[�^
};


//==============================================================================================
//
//	�┓�u���b�N���Ƃ̃^�b�`����(�┓�̃T�C�Y��)
//
//==============================================================================================
static const RECT_HIT_TBL block_hit_tbl[] = {	//�����ӁIblock_hit_tbl2�Ɠ����l�ɂ���
	//top,bottom,left,right
#if 0
	{ 21,	54,		27,		87 },			//1�i��
	{ 21,	54,		102,	162 },			//
	{ 21,	54,		174,	232 },			//

	{ 68,	100,	27,		87 },			//2�i��
	{ 68,	100,	102,	162 },			//
	{ 68,	100,	174,	232 },			//

	{ 114,	148,	27,		87 },			//3�i��
	{ 114,	148,	102,	162 },			//
	{ 114,	148,	174,	232 },			//
#else
	{ 21,	54,		32,		89 },			//1�i��			//+5
	{ 21,	54,		102,	157 },			//
	{ 21,	54,		170,	227 },			//				//-5

	{ 68,	100,	32,		89 },			//2�i��
	{ 68,	100,	102,	157 },			//
	{ 68,	100,	170,	227 },			//

	{ 114,	148,	32,		89 },			//3�i��
	{ 114,	148,	102,	157 },			//
	{ 114,	148,	170,	227 },			//
#endif
	{ TP_HIT_END,0,0,0 },					//�I���f�[�^
};

#define SCRATCH_TOUCH_SIZE_X	(240)
#define SCRATCH_TOUCH_SIZE_Y	(160)
#define SCRATCH_TOUCH_SIZE_MAX	(SCRATCH_TOUCH_SIZE_X * SCRATCH_TOUCH_SIZE_Y)

//RECT�\����(RECT_HIT_TBL�̒l�𒼐ڎQ�Əo���Ȃ��̂œ����f�[�^�𕡐��p��)
typedef struct{
	u8 top;
	u8 bottom;
	u8 left;
	u8 right;
}RECT;
static const RECT block_hit_tbl2[] = {		//�����ӁIblock_hit_tbl�Ɠ����l�ɂ���
	//top,bottom,left,right
#if 0
	{ 21,	54,		27,		87 },			//1�i��
	{ 21,	54,		102,	162 },			//
	{ 21,	54,		174,	232 },			//

	{ 68,	100,	27,		87 },			//2�i��
	{ 68,	100,	102,	162 },			//
	{ 68,	100,	174,	232 },			//

	{ 114,	148,	27,		87 },			//3�i��
	{ 114,	148,	102,	162 },			//
	{ 114,	148,	174,	232 },			//
#else
	{ 21,	54,		32,		89 },			//1�i��			//+5
	{ 21,	54,		102,	157 },			//
	{ 21,	54,		170,	227 },			//				//-5

	{ 68,	100,	32,		89 },			//2�i��
	{ 68,	100,	102,	157 },			//
	{ 68,	100,	170,	227 },			//

	{ 114,	148,	32,		89 },			//3�i��
	{ 114,	148,	102,	157 },			//
	{ 114,	148,	170,	227 },			//
#endif
};


//==============================================================================================
//
//	�┓�̍�藦���`�F�b�N������W�e�[�u��(���̍��W����T�C�Y�����`�F�b�N����)
//
//==============================================================================================
static const SCRATCH_POS block_check_pos[SCRATCH_D_POKE_MAX] = {
#if 0
	{ 46,	27	 },			//1�i��
	{ 121,	27	 },			//
	{ 193,	27	 },			//

	{ 46,	74	 },			//2�i��
	{ 121,	74	 },			//
	{ 193,	74	 },			//

	{ 46,	120  },			//3�i��
	{ 121,	120  },			//
	{ 193,	120  },			//
#else
	{ 42,	27	 },			//1�i��
	{ 117,	27	 },			//
	{ 189,	27	 },			//

	{ 42,	74	 },			//2�i��
	{ 117,	74	 },			//
	{ 189,	74	 },			//

	{ 42,	120  },			//3�i��
	{ 117,	120  },			//
	{ 189,	120  },			//
#endif
};
#define BLOCK_SIZE_X	(25)
#define BLOCK_SIZE_Y	(20)				//X * Y = 500
//#define BLOCK_COUNT	(35)				//���̒l�܂ŃJ�E���g���ꂽ��S�Č��J
//#define BLOCK_COUNT	(70)				//���̒l�܂ŃJ�E���g���ꂽ��S�Č��J
#define BLOCK_COUNT		(380)				//���̒l�܂ŃJ�E���g���ꂽ��S�Č��J


//==============================================================================================
//
//	���b�Z�[�W�֘A�̒�`
//
//==============================================================================================
#define SCRATCH_MENU_BUF_MAX		(2)						//���j���[�o�b�t�@�̍ő吔
#define BS_FONT					(FONT_SYSTEM)			//�t�H���g���
#define SCRATCH_MSG_BUF_SIZE		(600)//(800)//(1024)			//��b�̃��b�Z�[�Wsize
#define SCRATCH_MENU_BUF_SIZE		(32)					//���j���[�̃��b�Z�[�Wsize
#define PLAYER_NAME_BUF_SIZE	(PERSON_NAME_SIZE + EOM_SIZE)	//�v���C���[���̃��b�Z�[�Wsize
#define POKE_NAME_BUF_SIZE		(MONS_NAME_SIZE + EOM_SIZE)		//�|�P�������̃��b�Z�[�Wsize


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
struct _SCRATCH_WORK{

	PROC* proc;										//PROC�ւ̃|�C���^
	PROC* child_proc;								//PROC�ւ̃|�C���^

	u8	sub_seq;									//�V�[�P���X
	u8	type;										//�����Ƃ��ēn���ꂽ�o�g���^�C�v
	u8	msg_index;									//���b�Z�[�Windex
	u8	tmp_csr_pos;								//�ޔ����Ă���J�[�\���ʒu

	u8	csr_pos;									//���݂̃J�[�\���ʒu
	u8	wait;
	u8	card_num;									//�I�������J�[�h����
	u8	time_wait;

	s16	counter;
	s16	counter2;

	u8	mosaic_size;								//���U�C�N�T�C�Y(0-15)

	u8	card_col[SCRATCH_D_CARD_MAX];				//�I�񂾃J�[�h�̏ꏊ(�F�i���o�[���i�[)
	u8	mode[SCRATCH_CARD_NUM];

	VecFx32 scale_fx;

	VecFx32 scale_tbl;

	void*	msgicon;
	MSGDATA_MANAGER* msgman;						//���b�Z�[�W�}�l�[�W���[
	WORDSET* wordset;								//�P��Z�b�g
	STRBUF* msg_buf;								//���b�Z�[�W�o�b�t�@�|�C���^
	STRBUF* tmp_buf;								//�e���|�����o�b�t�@�|�C���^

	STRCODE str[PERSON_NAME_SIZE + EOM_SIZE];		//���j���[�̃��b�Z�[�W

	GF_BGL_INI*	bgl;								//BGL�ւ̃|�C���^
	GF_BGL_BMPWIN bmpwin[SCRATCH_BMPWIN_MAX];			//BMP�E�B���h�E�f�[�^

	PALETTE_FADE_PTR pfd;							//�p���b�g�t�F�[�h

	NUMFONT* num_font;								//8x8�t�H���g

	//const CONFIG* config;							//�R���t�B�O�|�C���^
	CONFIG* config;									//�R���t�B�O�|�C���^
	SAVEDATA* sv;									//�Z�[�u�f�[�^�|�C���^

	SCRATCH_CLACT scratch_clact;					//�Z���A�N�^�f�[�^
	SCRATCH_OBJ* p_u_poke[SCRATCH_U_POKE_MAX];		//�|�P����OBJ�̃|�C���^�i�[�e�[�u��
	SCRATCH_OBJ* p_d_poke[SCRATCH_D_POKE_MAX];		//�|�P����OBJ�̃|�C���^�i�[�e�[�u��
	SCRATCH_OBJ* p_d_card[SCRATCH_D_CARD_MAX];		//�J�[�hOBJ�̃|�C���^�i�[�e�[�u��

	SCRATCH_OBJ* p_d_button;						//����ʃ{�^��
	SCRATCH_OBJ* p_d_win;							//����ʂ̐^�񒆂̃E�B���h�E
	SCRATCH_OBJ* p_d_waku[SCRATCH_CARD_ATARI_NUM];	//����ʂ̋┓�̃��NOBJ�̃|�C���^�i�[�e�[�u��
	SCRATCH_OBJ* p_d_atari[SCRATCH_CARD_ATARI_NUM];	//������|�P����OBJ�̃|�C���^�i�[�e�[�u��
	SCRATCH_OBJ* p_d_atari_win;						//������E�B���h�E�̐^�񒆂̃E�B���h�E

	u8 card_poke[SCRATCH_D_POKE_MAX];				//�J�[�h�̒��ɂ���|�P����

	u16 atari_item[SCRATCH_ATARI_MAX];				//������̏��i

	u16* p_ret_work;								//SCRATCH_CALL_WORK�̖߂�l���[�N�ւ̃|�C���^
	u16* p_item_no;									//SCRATCH_CALL_WORK��item�i���o�|�ւ̃|�C���^
	u16* p_item_num;								//SCRATCH_CALL_WORK��item���ւ̃|�C���^

	ARCHANDLE* hdl;

	TP_BRUSH_DATA brush;							//�u���V�f�[�^

	//�L�����N�^�f�[�^
	void* p_char;
	NNSG2dCharacterData* p_chardata;
	u8* buf;

	TOUCH_SW_SYS* touch_subwin;

	u8 block_flag[SCRATCH_D_POKE_MAX];				//�ǂ̃u���b�N�̋┓�ɐG�ꂽ���t���O

	u8 pltt_res_no;
	u8 pltt_wait;
	u8 atari_poke;
	u8 waku_flag:1;									//���[�`�t���O=1
	u8 move_num:7;									//�J�[�h���S�ւ̈ړ���

	u8 kezuri_card_num;								//��藦OK�ȃJ�[�h�̐�
	u8 kezuri_card[SCRATCH_CARD_KEZURI_NUM];		//������J�[�h

	u8 block_count[SCRATCH_D_POKE_MAX];

	u32 tp_x;
	u32 tp_y;
	TP_ONE_DATA	tpData;

	//u8 touch_flag[256*192/8];
	//u8 touch_flag[256*192];
	u8 touch_flag[SCRATCH_TOUCH_SIZE_MAX];
};


//==============================================================================================
//
//	�^�b�`�p�l��
//
//==============================================================================================
//�L������
#define NUM_X_MAX			(32)
#define NUM_Y_MAX			(24)
#define NUM_MAX				(NUM_X_MAX * NUM_Y_MAX)
//�h�b�g��
#define DOT_X_MAX			(NUM_X_MAX * 8)
#define DOT_Y_MAX			(NUM_Y_MAX * 8)
#define DOT_MAX				(DOT_X_MAX * DOT_Y_MAX)

//16�F��1�L�����̃T�C�Y
#define ONE_CHAR			(0x20)

//�G�ꂽ�ʒu����N���A����dot(�㉺�A���E�Ŕ{�ɂȂ�)
//#define DOT_WRITE_WIDTH		(2)
#define DOT_WRITE_WIDTH		(3)


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
//PROC
PROC_RESULT ScratchProc_Init( PROC * proc, int * seq );
PROC_RESULT ScratchProc_Main( PROC * proc, int * seq );
PROC_RESULT ScratchProc_End( PROC * proc, int * seq );

//�V�[�P���X
static BOOL Seq_GameInit( SCRATCH_WORK* wk );
static BOOL Seq_GameCardIn( SCRATCH_WORK* wk );
static BOOL Seq_GameCardSel( SCRATCH_WORK* wk );
static BOOL Seq_GameCardOut( SCRATCH_WORK* wk );
static BOOL Seq_GameCardTouchIn( SCRATCH_WORK* wk );
static BOOL Seq_GameCardTouch( SCRATCH_WORK* wk );
static BOOL Seq_GameCardTouchOut( SCRATCH_WORK* wk );
static BOOL Seq_GameEnd( SCRATCH_WORK* wk );

//���ʏ���
static void Scratch_WakuAdd( SCRATCH_WORK* wk );
static void Scratch_AtariAdd( SCRATCH_WORK* wk );
static void Scratch_ButtonAdd( SCRATCH_WORK* wk );
static void Scratch_CardDelete( SCRATCH_WORK* wk );
static void ScratchCommon_Delete( SCRATCH_WORK* wk );
static void Scratch_Recover( SCRATCH_WORK* wk );
static void Scratch_InitSub1( void );
static void Scratch_InitSub2( SCRATCH_WORK* wk );
static void Scratch_AllocTouchSub( SCRATCH_WORK* wk );
static void Scratch_InitTouchSub( SCRATCH_WORK* wk );
static void Scratch_FreeTouchSub( SCRATCH_WORK* wk );

//���ʏ������A�I��
static void Scratch_ObjInit( SCRATCH_WORK* wk );
static void Scratch_BgInit( SCRATCH_WORK* wk );
static void Scratch_BgExit( GF_BGL_INI * ini );

//�ݒ�
static void VBlankFunc( void * work );
static void SetVramBank(void);
static void SetBgHeader( GF_BGL_INI * ini );

//BG�O���t�B�b�N�f�[�^
static void Scratch_SetMainBgGraphic( SCRATCH_WORK * wk, u32 frm  );
static void Scratch_SetMainBgPalette( void );
static void Scratch_SetYakuBgGraphic( SCRATCH_WORK * wk, u32 frm  );
static void Scratch_SetBackBgGraphic( SCRATCH_WORK * wk, u32 frm  );
static void Scratch_SetSubBgGraphic( SCRATCH_WORK * wk, u32 frm  );
static void Scratch_SetSubBgPalette( void );
static void Scratch_SetTouchBgGraphic( SCRATCH_WORK * wk, u32 frm  );
//static void Scratch_SetAtariBgGraphic( SCRATCH_WORK * wk, u32 frm  );

//���b�Z�[�W
static u8 ScratchWriteMsg( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 ScratchWriteMsgSimple( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font );
static u8 Scratch_KakuninMsg( SCRATCH_WORK* wk );
//static u8 Scratch_CardNumMsg( SCRATCH_WORK* wk );
static u8 Scratch_CardSelectMsg( SCRATCH_WORK* wk );
static u8 Scratch_StartMsg( SCRATCH_WORK* wk );
static u8 Scratch_AtariMsg( SCRATCH_WORK* wk );
static u8 Scratch_HazureMsg( SCRATCH_WORK* wk );
static u8 Scratch_NextMsg( SCRATCH_WORK* wk );
static u8 Scratch_YameruMsg( SCRATCH_WORK* wk );
static u8 Scratch_YakuMsg( SCRATCH_WORK* wk, u8 no );

//������
static void Scratch_SetNumber( SCRATCH_WORK* wk, u32 bufID, s32 number );
static void Scratch_SetPokeName( SCRATCH_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp );
static void Scratch_SetPlayerName( SCRATCH_WORK* wk, u32 bufID );
static void PlayerNameWrite( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font );
static void PokeNameWriteEx( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex );

//�c�[��
static void NextSeq( SCRATCH_WORK* wk, int* seq, int next );
static int KeyCheck( int key );
static void BgCheck( SCRATCH_WORK* wk );
static u16 GetCsrX( SCRATCH_WORK* wk );
static void Scratch_MetamonPokeSet( SCRATCH_WORK* wk );
static void Scratch_OthersPokeSet( SCRATCH_WORK* wk );
static void Scratch_PokeSet( SCRATCH_WORK* wk );
static void Scratch_CardPokeSet( SCRATCH_WORK* wk, u8 mode );
static void Scratch_CardPokeClear( SCRATCH_WORK* wk );
static void Scratch_YakuSet( SCRATCH_WORK* wk, u8 mode );
static BOOL Scratch_CardCenterMove( SCRATCH_WORK* wk, u8 pos );
static void ScrPalChg( SCRATCH_WORK* wk, u32 frm, u8 pltt_no, u8 sx, u8 sy );
static u8 Scratch_BlockFlagOnNumGet( SCRATCH_WORK* wk );
static void Scratch_BlockFlagClear( SCRATCH_WORK* wk );
static void Scratch_BlockCountClear( SCRATCH_WORK* wk );
static void Scratch_HoseiTouch2( SCRATCH_WORK* wk, int index );
static void Scratch_TouchFlagSet( SCRATCH_WORK* wk );
static void Scratch_TouchFlagSetSub( SCRATCH_WORK* wk, int tp_x, int tp_y );
static BOOL Scratch_TouchFlagCheck( SCRATCH_WORK* wk, u8 index );
static void Scratch_PlttNoResChg( SCRATCH_WORK* wk );
static BOOL Scratch_AtariCheck( SCRATCH_WORK* wk );
static BOOL Scratch_MosaicEffInit( SCRATCH_WORK* wk );
static BOOL Scratch_MosaicEffMain( SCRATCH_WORK* wk, u8 flag );

//�T�u�V�[�P���X
static void Scratch_SeqSubYameruCard( SCRATCH_WORK* wk );
static void Scratch_SeqSubNextCard( SCRATCH_WORK* wk );
static void Scratch_SeqSubGinOk( SCRATCH_WORK* wk );

//�L�����N�^�[�f�[�^���o�b�t�@�ɃR�s�[
static void Scratch_Touch_0( SCRATCH_WORK* wk );
//�o�b�t�@�𐶐�
static void Scratch_Touch_1( SCRATCH_WORK* wk );
//�L�����N�^�[�f�[�^���R�s�[
static void Scratch_Touch_1_5( SCRATCH_WORK* wk );
//�^�b�`�p�l���ɐG��Ă�����W���擾���ăL�����N�^�[�f�[�^������������
static void Scratch_Touch_2( SCRATCH_WORK* wk, int tp_x, int tp_y );
//�n���ꂽ�ʒu�̃L�����N�^�[�f�[�^��0�N���A
static void Scratch_Touch_3( SCRATCH_WORK* wk, u32 tp_x, u32 tp_y );


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
PROC_RESULT ScratchProc_Init( PROC * proc, int * seq )
{
	int i;
	SCRATCH_WORK* wk;
	SCRATCH_CALL_WORK* scratch_call;

	Scratch_InitSub1();

	//sys_CreateHeap( HEAPID_BASE_APP, HEAPID_SCRATCH, 0x20000 );
	//sys_CreateHeap( HEAPID_BASE_APP, HEAPID_SCRATCH, 0x30000 );
	//sys_CreateHeap( HEAPID_BASE_APP, HEAPID_SCRATCH, 0x38000 );
	//sys_CreateHeap( HEAPID_BASE_APP, HEAPID_SCRATCH, 0x40000 );
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_SCRATCH, 0x48000 );

	wk = PROC_AllocWork( proc, sizeof(SCRATCH_WORK), HEAPID_SCRATCH );
	memset( wk, 0, sizeof(SCRATCH_WORK) );

	wk->bgl				= GF_BGL_BglIniAlloc( HEAPID_SCRATCH );
	wk->proc			= proc;
	scratch_call			= (SCRATCH_CALL_WORK*)PROC_GetParentWork( proc );
	wk->sv				= scratch_call->sv;
	wk->type			= scratch_call->type;
	wk->p_ret_work		= &scratch_call->ret_work;
	wk->config			= SaveData_GetConfig( wk->sv );			//�R���t�B�O�|�C���^���擾
	wk->p_item_no		= &scratch_call->item_no[0];
	wk->p_item_num		= &scratch_call->item_num[0];

	Scratch_CardPokeClear( wk );

	//���i�N���A
	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
		wk->p_item_no[i]	= 0;
		wk->p_item_num[i]	= 0;
	}

	//�p���b�g���\�[�X�؂�ւ������l�Z�b�g
	wk->pltt_res_no = 0;

	Scratch_InitSub2( wk );

	(*seq) = SEQ_GAME_INIT;
	OS_Printf( "(*seq) = %d\n", (*seq) );

	Snd_DataSetByScene( SND_SCENE_SUB_SCRATCH, 0, 0 );

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
PROC_RESULT ScratchProc_Main( PROC * proc, int * seq )
{
	SCRATCH_WORK* wk  = PROC_GetWork( proc );
	//OS_Printf( "(*seq) = %d\n", (*seq) );

	switch( *seq ){

	//-----------------------------------
	//������
	case SEQ_GAME_INIT:
		if( Seq_GameInit(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_CARD_IN );
		}
		break;

	//-----------------------------------
	//�J�[�h�C��
	case SEQ_GAME_CARD_IN:
		if( Seq_GameCardIn(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_CARD_SEL );			//�J�[�h�I����
		}
		break;

	//-----------------------------------
	//�J�[�h�I��
	case SEQ_GAME_CARD_SEL:
		if( Seq_GameCardSel(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_CARD_OUT );			//�J�[�h�A�E�g��
		}
		break;

	//-----------------------------------
	//�J�[�h�A�E�g
	case SEQ_GAME_CARD_OUT:
		if( Seq_GameCardOut(wk) == TRUE ){
			if( wk->card_num >= SCRATCH_CARD_NUM ){
				wk->card_num = 0;

				Scratch_CardDelete( wk );					//���F�J�[�hOBJ�폜
				//ScratchClact_ResButtonChg( &wk->scratch_clact );	//���\�[�X�؂�ւ�(card��button)
				Scratch_WakuAdd( wk );						//���F���NOBJ�ǉ�
				//Scratch_ButtonAdd( wk );					//���F�{�^��OBJ�ǉ�
				Scratch_AtariAdd( wk );						//���F������|�P����OBJ�ǉ�

				NextSeq( wk, seq, SEQ_GAME_CARD_TOUCH_IN );	//�J�[�h���C����
			}else{
				NextSeq( wk, seq, SEQ_GAME_CARD_IN );		//�J�[�h�C����
			}
		}
		break;

	//-----------------------------------
	//�J�[�h���C��
	case SEQ_GAME_CARD_TOUCH_IN:
		if( Seq_GameCardTouchIn(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_CARD_TOUCH );		//�J�[�h����
		}
		break;

	//-----------------------------------
	//�J�[�h��蒆
	case SEQ_GAME_CARD_TOUCH:
		if( Seq_GameCardTouch(wk) == TRUE ){
			if( wk->card_num >= SCRATCH_CARD_NUM ){
				NextSeq( wk, seq, SEQ_GAME_END );			//�I����
			}else{
				NextSeq( wk, seq, SEQ_GAME_CARD_TOUCH_OUT );//�J�[�h���A�E�g��
			}
		}
		break;

	//-----------------------------------
	//�J�[�h���A�E�g
	case SEQ_GAME_CARD_TOUCH_OUT:
		if( Seq_GameCardTouchOut(wk) == TRUE ){
			NextSeq( wk, seq, SEQ_GAME_CARD_TOUCH_IN );		//�J�[�h���C����
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

	CLACT_Draw( wk->scratch_clact.ClactSet );		//�Z���A�N�^�[�풓�֐�

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
PROC_RESULT ScratchProc_End( PROC * proc, int * seq )
{
	int i;
	SCRATCH_WORK* wk = PROC_GetWork( proc );

	StopTP();

	*(wk->p_ret_work) = wk->csr_pos;					//�߂�l�i�[���[�N�֑��(�J�[�\���ʒu)

	OS_Printf( "*(wk->p_ret_work) = %d\n", *(wk->p_ret_work) );

	DellVramTransferManager();

	ScratchCommon_Delete( wk );							//�폜����

	PROC_FreeWork( proc );								//���[�N�J��

	sys_VBlankFuncChange( NULL, NULL );					//VBlank�Z�b�g
	sys_DeleteHeap( HEAPID_SCRATCH );

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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameInit( SCRATCH_WORK* wk )
{
	switch( wk->sub_seq ){

	//�u���b�N�C��
	case 0:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEIN, WIPE_TYPE_FADEIN, 
						WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC*3, HEAPID_SCRATCH );

		wk->sub_seq++;
		break;
		
	//�t�F�[�h�I���҂�
	case 1:
		if( WIPE_SYS_EndCheck() == TRUE ){
			wk->card_num = 0;
			return TRUE;
		}
		break;
	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�[�h�C��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCardIn( SCRATCH_WORK* wk )
{
	int i;

	enum{
		SEQ_SUB_START = 0,
		SEQ_SUB_CARD_IN,
		SEQ_SUB_CARD_IN_END,
	};

	switch( wk->sub_seq ){

	//�J�[�h���C������ʒu�ɔz�u
	case SEQ_SUB_START:
		wk->counter = SCRATCH_CARD_IN_OFFSET_X;

		for( i=0; i < SCRATCH_D_CARD_MAX ;i++ ){
			ScratchObj_SetObjPos(	wk->p_d_card[i], (d_card_pos[i].x + wk->counter), 
									d_card_pos[i].y );
			ScratchObj_Priority( wk->p_d_card[i], (SCRATCH_OBJ_PRI_L+i) );
		}

		//�u�����ڂ̃J�[�h��I��ł��������v
		ScratchTalkWinPut( &wk->bmpwin[BMPWIN_SELECT], CONFIG_GetWindowType(wk->config) );

		Scratch_CardSelectMsg( wk );
		GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_MSG, VISIBLE_ON );		//�チ�b�Z�[�W�I��

		wk->counter2 = (SCRATCH_D_CARD_MAX - 1);
		Snd_SePlay( SE_D_CARD_IN );
		wk->sub_seq = SEQ_SUB_CARD_IN;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�C��
	case SEQ_SUB_CARD_IN:
		wk->counter += SCRATCH_CARD_IN_SPD;
		ScratchObj_SetObjPos(	wk->p_d_card[wk->counter2], 
								(d_card_pos[wk->counter2].x + wk->counter), 
								d_card_pos[wk->counter2].y );

		if( wk->counter >= 0 ){
			//�S�ẴJ�[�h������������
			if( wk->counter2 == 0 ){
				wk->sub_seq = SEQ_SUB_CARD_IN_END;
			}else{
				Snd_SePlay( SE_D_CARD_IN );
				wk->counter2--;												//���̃J�[�h��
				wk->counter = SCRATCH_CARD_IN_OFFSET_X;
			}
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�C���I��
	case SEQ_SUB_CARD_IN_END:
		wk->counter = 0;
		return TRUE;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�[�h�I��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCardSel( SCRATCH_WORK* wk )
{
	u32 tp_x,tp_y;
	int i,index;

	enum{
		SEQ_SUB_START = 0,
		SEQ_SUB_CARD_SEL,
		SEQ_SUB_CARD_SEL_END,
	};

	switch( wk->sub_seq ){

	//
	case SEQ_SUB_START:
		//ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_OFF );					//�I��
		//Scratch_CardNumMsg( wk );
		GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_MSG, VISIBLE_ON );			//�����b�Z�[�W�I��
		wk->time_wait = SCRATCH_START_MSG_DEL_WAIT;
		wk->sub_seq = SEQ_SUB_CARD_SEL;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�I��
	case SEQ_SUB_CARD_SEL:

		//���ԂŃ��b�Z�[�W�����Ă���
		if( wk->time_wait == 0 ){
			GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_MSG, VISIBLE_OFF );		//�����b�Z�[�W�I�t
			ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_ON );				//�I�t
		}else{
			wk->time_wait--;
		}

		index = GF_TP_RectHitCont( (const RECT_HIT_TBL*)card_hit_tbl );

		//if( GF_TP_SingleHitCont((const RECT_HIT_TBL*)card_hit_tbl) == TRUE ){
		if( index != RECT_HIT_NONE ){
			GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_MSG, VISIBLE_OFF );		//�����b�Z�[�W�I�t
			ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_ON );				//�I�t
			Snd_SePlay( SE_D_BUTTON_TOUCH );

			wk->card_col[wk->card_num] = index;		//�I�񂾃J�[�h�̏ꏊ(�F�i���o�[���i�[)
			wk->card_num++;
			OS_Printf( "�J�[�h�Ƀ^�b�`���܂��� = %d\n", index );
			OS_Printf( "0=����ԁA1=�E��A2=�������F�A3=�E����\n" );

			//�^�b�`�p�l���ɐG��Ă���Ȃ炻�̍��W�擾(�x�^����)
			GF_TP_GetPointCont( &tp_x, &tp_y );
			OS_Printf( "tp_x = %d\ttp_y = %d\n", tp_x, tp_y );

			BmpTalkWinClear( &wk->bmpwin[BMPWIN_SELECT], WINDOW_TRANS_ON );
			GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_MSG, VISIBLE_OFF );		//�チ�b�Z�[�W�I�t
			wk->sub_seq = SEQ_SUB_CARD_SEL_END;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�I���I��
	case SEQ_SUB_CARD_SEL_END:
		return TRUE;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�[�h�A�E�g
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCardOut( SCRATCH_WORK* wk )
{
	int i,pos,now_x,now_y;

	enum{
		SEQ_SUB_START = 0,
		SEQ_SUB_CARD_AFF,
		SEQ_SUB_CARD_OUT,
		SEQ_SUB_CARD_OUT_END,
	};

	//�ǂ̃J�[�h��I�񂾂�
	pos = wk->card_col[wk->card_num-1];
					
	switch( wk->sub_seq ){

	//
	case SEQ_SUB_START:
		wk->scale_fx.x = SCRATCH_CARD_AFF_DEFAULT;		//���{
		wk->scale_fx.y = SCRATCH_CARD_AFF_DEFAULT;
		wk->scale_fx.z = SCRATCH_CARD_AFF_DEFAULT;
		wk->counter = 0;
		wk->move_num= 0;
		OS_Printf( "�ǂ̃J�[�h�� pos = %d\n", pos );
		ScratchObj_Priority( wk->p_d_card[pos], SCRATCH_OBJ_PRI_H );

		wk->sub_seq = SEQ_SUB_CARD_AFF;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�g��
	case SEQ_SUB_CARD_AFF:

		//�J�[�h�𒆐S�Ɉړ�
		if( Scratch_CardCenterMove(wk,pos) == TRUE ){
			//
		}

		wk->scale_fx.x += SCRATCH_CARD_AFF_ADD;
		wk->scale_fx.y += SCRATCH_CARD_AFF_ADD;
		wk->scale_fx.z += SCRATCH_CARD_AFF_ADD;

		wk->counter++;
		if( (wk->counter % SCRATCH_CARD_AFF_WAIT) == 0 ){

			//�I�񂾃J�[�h�ɂ���
			ScratchObj_SetScaleAffineTbl( wk->p_d_card[pos], &wk->scale_fx );
		}

		//�J�[�h�̍ő�g��
		if( wk->scale_fx.x == SCRATCH_CARD_AFF_MAX ){
			wk->counter = 0;

			wk->scale_fx.x = 0x1000;
			wk->scale_fx.y = 0x1000;
			wk->scale_fx.z = 0x1000;

			Snd_SePlay( SE_D_CARD_OUT );
			wk->sub_seq = SEQ_SUB_CARD_OUT;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�A�E�g
	case SEQ_SUB_CARD_OUT:

		//�J�[�h�𒆐S�Ɉړ�
		//if( Scratch_CardCenterMove(wk,pos) == TRUE ){
			//
		//}

		wk->counter += SCRATCH_CARD_OUT_SPD;
		for( i=0; i < SCRATCH_D_CARD_MAX ;i++ ){
			ScratchObj_GetObjPos( wk->p_d_card[i], &now_x, &now_y );	//���݂̍��W�擾
			OS_Printf( "i = %d, now_x = %d\n", i, now_x );
			ScratchObj_SetObjPos(	wk->p_d_card[i], (now_x + SCRATCH_CARD_OUT_SPD), 
									now_y );
			//ScratchObj_SetObjPos(	wk->p_d_card[i], (d_card_pos[i].x + wk->counter), 
			//						d_card_pos[i].y );
			
			//����������
			//if( now_x >= 368 ){
			//	ScratchObj_SetScaleAffineTbl( wk->p_d_card[pos], &wk->scale_fx );
			//}
		}

		if( wk->counter >= SCRATCH_CARD_OUT_OFFSET_X ){
		//if( wk->counter >= SCRATCH_CARD_OUT_OFFSET_X + 128 ){		//�g�啪(128)
			//ScratchObj_SetScaleAffine2( wk->p_d_card[0], 0 );
			//ScratchObj_SetScaleAffine( wk->p_d_card[0], 5 );
			

			//ScratchObj_SetScaleAffineTbl( wk->p_d_card[pos], &wk->scale_fx );
			ScratchObj_SetScaleAffine( wk->p_d_card[pos], 0 );
			wk->sub_seq = SEQ_SUB_CARD_OUT_END;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�A�E�g�I��
	case SEQ_SUB_CARD_OUT_END:
		wk->counter = 0;
		return TRUE;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�[�h���C��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCardTouchIn( SCRATCH_WORK* wk )
{
	int i,pos;

	enum{
		SEQ_SUB_CARD_TOUCH_IN_START = 0,
		SEQ_SUB_CARD_TOUCH_IN_START2,
		SEQ_SUB_CARD_TOUCH_IN,
		SEQ_SUB_CARD_TOUCH_IN_END,
	};

	switch( wk->sub_seq ){

	//�J�[�h���C������ʒu�ɔz�u
	case SEQ_SUB_CARD_TOUCH_IN_START:
		//�L�����N�^�[�f�[�^�����ɖ߂�
		Scratch_Touch_1_5( wk );
		GF_BGL_LoadCharacter( wk->bgl, SCRATCH_FRAME_D_TOUCH, wk->buf, wk->p_chardata->szByte, 0 );
		GF_BGL_LoadScreenReq( wk->bgl, SCRATCH_FRAME_D_TOUCH );

		wk->counter2 = SCRATCH_CARD_TOUCH_IN_OFFSET_X;
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_U_YAKU, GF_BGL_SCROLL_X_SET, wk->counter2 );
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_D_TOUCH, GF_BGL_SCROLL_X_SET, wk->counter2 );
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_D_SUB, GF_BGL_SCROLL_X_SET, wk->counter2 );

		GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_TOUCH, VISIBLE_ON );			//���ʃI��
		GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_SUB, VISIBLE_ON );			//�J�[�h�ʃI��
		GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_YAKU, VISIBLE_ON );			//���I��

		wk->counter = SCRATCH_CARD_IN_OFFSET_X;

		//�C�[�W�[����
		wk->mode[ wk->card_num ] = SCRATCH_CARD_MODE_EASY;						//�C�[�W�J�[�h

		//�N���A
		for( i=0; i < SCRATCH_TOUCH_SIZE_MAX ;i++ ){
			wk->touch_flag[i] = 0;
		}

		//������J�[�h�̐����N���A
		wk->kezuri_card_num = 0;

		//�J�[�h�̃|�P��������
		Scratch_CardPokeClear( wk );
		Scratch_CardPokeSet( wk, wk->mode[ wk->card_num ] );

		//��������
		Scratch_YakuSet( wk, wk->mode[ wk->card_num ] );

		//�┓�ɐG�ꂽ�t���O�N���A
		Scratch_BlockFlagClear( wk );

		//�G�ꂽ�ʒu�J�E���g���N���A
		Scratch_BlockCountClear( wk );

		wk->sub_seq = SEQ_SUB_CARD_TOUCH_IN_START2;
		break;

	case SEQ_SUB_CARD_TOUCH_IN_START2:
		//��F�|�P����OBJ�\��
		for( i=0; i < SCRATCH_U_POKE_MAX ;i++ ){
			ScratchObj_SetObjPos(	wk->p_u_poke[i], 
							(SCRATCH_U_POKE_X + wk->counter),
							(SCRATCH_U_POKE_Y + (i * SCRATCH_U_POKE_WIDTH_Y)) );
			ScratchObj_Vanish( wk->p_u_poke[i], SCRATCH_VANISH_OFF );		//�\��
		}

		//���F���NOBJ�\��
		for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
			//�g�p�p���b�g�����ɖ߂�
			ScratchObj_PaletteNoChg( wk->p_d_waku[i], 1 );
		}

		//�N���A
		wk->pltt_res_no		= 0;
		wk->waku_flag		= 0;
		wk->kezuri_card[0]	= DUMMY_KEZURI_CARD1;
		wk->kezuri_card[1]	= DUMMY_KEZURI_CARD2;
		wk->kezuri_card[2]	= DUMMY_KEZURI_CARD3;

		//���F�|�P����OBJ�\��
		for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
			ScratchObj_SetObjPos(	wk->p_d_poke[i], (d_poke_pos[i].x + wk->counter), 
									d_poke_pos[i].y );
			ScratchObj_Vanish( wk->p_d_poke[i], SCRATCH_VANISH_OFF );		//�\��
			ScratchObj_AnmChg( wk->p_d_poke[i], wk->card_poke[i] );			//�z�u�|�P�����ɕύX
		}

		//�J�[�h�̃X�N���[���̃p���b�g�ύX(u=4,5,6,7�p���b�g�Ad=1,2,3,4�p���b�g)
		ScrPalChg( wk, SCRATCH_FRAME_D_SUB, (wk->card_col[ wk->card_num ] + 4), 32, 24 );
		ScrPalChg( wk, SCRATCH_FRAME_U_YAKU, (wk->card_col[ wk->card_num ] + 1), 32, 24 );

		Snd_SePlay( SE_D_CARD_IN );
		wk->sub_seq = SEQ_SUB_CARD_TOUCH_IN;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�C��
	case SEQ_SUB_CARD_TOUCH_IN:
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_U_YAKU, GF_BGL_SCROLL_X_SET, wk->counter2 );
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_D_TOUCH, GF_BGL_SCROLL_X_SET, wk->counter2 );
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_D_SUB, GF_BGL_SCROLL_X_SET, wk->counter2 );
		wk->counter2 += SCRATCH_CARD_TOUCH_IN_SPD;

		if( wk->counter >= 0 ){
			wk->sub_seq = SEQ_SUB_CARD_TOUCH_IN_END;
		}else{
			//��F�|�P����OBJ�\��
			for( i=0; i < SCRATCH_U_POKE_MAX ;i++ ){
				ScratchObj_SetObjPos(	wk->p_u_poke[i], 
							(SCRATCH_U_POKE_X + wk->counter),
							(SCRATCH_U_POKE_Y + (i * SCRATCH_U_POKE_WIDTH_Y)) );
			}

			//���F�|�P����OBJ�\��
			for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
				ScratchObj_SetObjPos(	wk->p_d_poke[i], (d_poke_pos[i].x + wk->counter), 
										d_poke_pos[i].y );
			}
		}

		wk->counter += SCRATCH_CARD_IN_SPD2;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�C���I��
	case SEQ_SUB_CARD_TOUCH_IN_END:
		wk->counter	= 0;
		wk->counter2= 0;
		return TRUE;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�[�h��蒆
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCardTouch( SCRATCH_WORK* wk )
{
	int i,index;
	u32 ret,color;

	enum{
		SEQ_SUB_START = 0,
		SEQ_SUB_START_SUB,
		SEQ_SUB_MSG_WAIT,
		SEQ_SUB_TYPE_SEL,

		SEQ_SUB_GIN_OK_ATARI_MOSAIC_WAIT,
		SEQ_SUB_GIN_OK_ATARI_MOSAIC_WAIT2,

		SEQ_SUB_GIN_OK_ATARI_MSG_WAIT,
		SEQ_SUB_GIN_OK_HAZURE_MSG_WAIT,
		SEQ_SUB_GIN_OK_GIN_ALL_CLEAR_WAIT,
		SEQ_SUB_NEXT_WAIT,
		SEQ_SUB_YAMERU_WAIT,
		SEQ_SUB_YAMERU_WAIT_NO,
	};

	switch( wk->sub_seq ){

	case SEQ_SUB_START:
		if( wk->card_num == (SCRATCH_CARD_NUM - 1) ){
			GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_NEXT] );
			Scratch_YameruMsg( wk );
			ScratchObj_SetObjPos( wk->p_d_button, d_yameru_button_pos.x, d_yameru_button_pos.y );
			ScratchObj_AnmChg( wk->p_d_button, ANM_BUTTON_YAMERU );				//��߂�ɕύX
		}else{
			GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_YAMERU] );
			Scratch_NextMsg( wk );
		}

		wk->time_wait = SCRATCH_START_MSG_DEL_WAIT;
		ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_OFF );					//�I��
		Scratch_StartMsg( wk );

		//����\��
		for( i=0; i < SCRATCH_ATARI_MAX ;i++ ){
			Scratch_YakuMsg( wk, i );
		}

		//���̃J�[�h��(��߂�)�{�^��
		ScratchObj_Vanish( wk->p_d_button, SCRATCH_VANISH_OFF );				//�I��
		ScratchObj_SetAnmFlag( wk->p_d_button, 0 );								//�{�^���A�j��OFF

		Snd_SePlay( SE_D_GAME_START );
		wk->sub_seq = SEQ_SUB_START_SUB;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	case SEQ_SUB_START_SUB:
		GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_MSG, VISIBLE_ON );			//�����b�Z�[�W�I��
		GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_MSG, VISIBLE_ON );			//�チ�b�Z�[�W�I��
		wk->sub_seq = SEQ_SUB_MSG_WAIT;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���b�Z�[�W�\���҂�
	case SEQ_SUB_MSG_WAIT:

		//���ԂŃ��b�Z�[�W�����Ă���
		if( wk->time_wait == 0 ){
			ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_ON );				//�I�t
			GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_START], FBMP_COL_NULL );
			//GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_ATARI], FBMP_COL_NULL );
			//GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_HAZURE], FBMP_COL_NULL );
			GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_START] );
			//GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_ATARI] );
			//GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_HAZURE] );
			
			wk->sub_seq = SEQ_SUB_TYPE_SEL;
		}else{
			wk->time_wait--;
		}

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�^�C�v��I��
	case SEQ_SUB_TYPE_SEL:
#if 0
	1:	�L�����f�[�^���o�b�t�@�ɃR�s�[
	2:	�^�b�`�p�l���ŐG��Ă�����W�擾
	3:	2)�̈ʒu�̃L�����f�[�^�̃h�b�g�����N���A
	4:	2),3)�̌J��Ԃ�

	2�̈ʒu����c�A�����h�b�g��Ή�������

	�G���Ă���h�b�g�̈ʒu�̂݃o�b�t�@�����������ē]���ɂ���
#endif

#ifdef PM_DEBUG

		//BgCheck( wk );
		
		//���N�������f�o�b�N�L�[
		if( sys.cont & PAD_BUTTON_L ){
			GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_TOUCH, VISIBLE_OFF );		//���ʃI�t
		}else if( sys.cont & PAD_BUTTON_R ){
			GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_TOUCH, VISIBLE_ON );		//���ʃI��
		}
#endif

		//�T���v�����O���擾
		//MainTP( &wk->tpData, TP_BUFFERING_JUST, 1 );
		MainTP( &wk->tpData, TP_BUFFERING_JUST, 2 );

		index = GF_TP_RectHitCont( (const RECT_HIT_TBL*)block_hit_tbl );
		//if( (index != RECT_HIT_NONE) && (index != 4) ){
		if( index != RECT_HIT_NONE ){

			//�^�b�`�p�l���ɐG��Ă���Ȃ炻�̍��W�擾(�x�^����)
			//GF_TP_GetPointCont( &wk->tp_x, &wk->tp_y );			//���ݍ��W

			//�܂��┓�u���b�N�ɐG��鎞
			if( Scratch_BlockFlagOnNumGet(wk) < SCRATCH_CARD_KEZURI_NUM ){
				wk->block_flag[index] = 1;
			}

			//�G�ꂽ�u���b�N�̋┓��������
			if( wk->block_flag[index] == 1 ){

				//�G�ꂽ�t���O�𗧂Ă�
				Scratch_TouchFlagSet( wk );

				//�G�ꂽ�t���O�`�F�b�N
				Scratch_TouchFlagCheck( wk, index );
			
				//tpData�Ɋi�[���ꂽ���W�̃L�����N�^�f�[�^������������
				Scratch_HoseiTouch2( wk, index );

				//��艹���Đ����Ă��Ȃ�������Đ�
				if( Snd_SePlayCheck(SE_D_CARD_KEZURI) == FALSE ){
					Snd_SePlay( SE_D_CARD_KEZURI );
				}

				//�^�b�`�p�l���ɐG��Ă�����W���擾���ăL�����N�^�[�f�[�^������������
				//Scratch_Touch_2( wk, wk->tp_x, wk->tp_y );
			}

			//�G�ꂽ�t���O�`�F�b�N
			//Scratch_TouchFlagCheck( wk, index );

			//�����肪���������
			if( Scratch_AtariCheck(wk) == TRUE ){

				Snd_SeStopBySeqNo( SE_D_CARD_KEZURI, 0 );
				ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_OFF );	//�\��

				//���i���Z�b�g
				wk->p_item_no[wk->card_num]	= wk->atari_item[wk->atari_poke];

				//�u����̂��܁v�̎��̓A�C�e������1�ɂ���
				if( wk->atari_item[wk->atari_poke] == ITEM_KINNOTAMA ){
					wk->p_item_num[wk->card_num]= 1;
				}else{
					wk->p_item_num[wk->card_num]= 3;
				}

				//���^�����̃��U�C�N�G�t�F�N�g������
				if( Scratch_MosaicEffInit(wk) == TRUE ){
					wk->time_wait = SCRATCH_RESULT_MOSAIC_WAIT;
				}else{
					wk->time_wait = 0;
				}

				Scratch_AtariMsg( wk );									//�����胁�b�Z�[�W
				wk->sub_seq = SEQ_SUB_GIN_OK_ATARI_MOSAIC_WAIT;
				return FALSE;
			}

			//�����肪������Ă��Ȃ��āA�S�č������
			if( wk->kezuri_card_num >= SCRATCH_CARD_KEZURI_NUM	){

				Snd_SeStopBySeqNo( SE_D_CARD_KEZURI, 0 );
				wk->time_wait = SCRATCH_RESULT_MSG_DEL_WAIT;
				ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_OFF );	//�\��

				Scratch_HazureMsg( wk );								//�O�ꃁ�b�Z�[�W
				wk->sub_seq = SEQ_SUB_GIN_OK_HAZURE_MSG_WAIT;
				return FALSE;
			}

		//�^�b�`���Ă��Ȃ���
		}else{
			Snd_SeStopBySeqNo( SE_D_CARD_KEZURI, 0 );					//��胋�[�v�����~�߂�
		}
	
		//���[�`�̎�(��ʂɃ^�b�`���Ă��Ȃ�������������)
		if( wk->waku_flag == 1 ){
			Scratch_PlttNoResChg( wk );		//�E�F�C�g�v�Z���ăp���b�g�i���o�[�A���\�[�X�ύX
		}

		//3���ڂ̃J�[�h��V��ł��鎞
		if( wk->card_num == (SCRATCH_CARD_NUM - 1) ){
			//��߂���^�b�`������
			index = GF_TP_RectHitTrg( (const RECT_HIT_TBL*)yameru_hit_tbl );
			if( index != RECT_HIT_NONE ){
				ScratchObj_SetAnmFlag( wk->p_d_button, 1 );		//�{�^���A�j��ON
				Scratch_KakuninMsg( wk );						//�X�N���b�`����߂܂����H
				Snd_SePlay( SE_D_BUTTON_TOUCH );
				ScratchObj_AnmChg( wk->p_d_button, ANM_BUTTON_YAMERU );		//��߂�ɕύX
				Scratch_AllocTouchSub( wk );
				Scratch_InitTouchSub( wk );
				wk->sub_seq = SEQ_SUB_YAMERU_WAIT;
				break;
			}
		}else{
			//���̃J�[�h�ւ��^�b�`������
			index = GF_TP_RectHitTrg( (const RECT_HIT_TBL*)next_hit_tbl );

			if( index != RECT_HIT_NONE ){
				ScratchObj_SetAnmFlag( wk->p_d_button, 1 );		//�{�^���A�j��ON
				Scratch_KakuninMsg( wk );						//�X�N���b�`����߂܂����H
				Snd_SePlay( SE_D_BUTTON_TOUCH );
				ScratchObj_AnmChg( wk->p_d_button, ANM_BUTTON_NEXT );		//���̃J�[�h�ւɕύX
				Scratch_AllocTouchSub( wk );
				Scratch_InitTouchSub( wk );
				wk->sub_seq = SEQ_SUB_NEXT_WAIT;
				break;
			}
		}

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���U�C�N�E�F�C�g
	case SEQ_SUB_GIN_OK_ATARI_MOSAIC_WAIT:

		if( wk->time_wait == 0 ){
			//���^�����̃��U�C�N�G�t�F�N�g�I���҂�
			if( Scratch_MosaicEffMain(wk,0) == FALSE ){
				wk->sub_seq = SEQ_SUB_GIN_OK_ATARI_MOSAIC_WAIT2;
			}
		}else{
			wk->time_wait--;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���U�C�N�E�F�C�g2
	case SEQ_SUB_GIN_OK_ATARI_MOSAIC_WAIT2:

		//���^�����̃��U�C�N�G�t�F�N�g�I���҂�
		if( Scratch_MosaicEffMain(wk,1) == FALSE ){
			//wk->time_wait = SCRATCH_RESULT_MSG_DEL_WAIT;
			wk->time_wait = 0;
			wk->sub_seq = SEQ_SUB_GIN_OK_ATARI_MSG_WAIT;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�����胁�b�Z�[�W�\��
	case SEQ_SUB_GIN_OK_ATARI_MSG_WAIT:

		//������E�B���h�E���A�j���[�V�������Ă����甲����
		if( ScratchObj_AnmActiveCheck(wk->p_d_atari_win) == TRUE ){
			break;
		}

		//���ԂŃ��b�Z�[�W�����Ă���
		if( wk->time_wait == 0 ){

			for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
				ScratchObj_Vanish( wk->p_d_atari[i], SCRATCH_VANISH_ON );		//��\��
			}
			
			ScratchObj_Vanish( wk->p_d_atari_win, SCRATCH_VANISH_ON );			//��\��
			//GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_ATARI, VISIBLE_OFF );	//��������win off
			Scratch_SeqSubNextCard( wk );
			Scratch_SeqSubGinOk( wk );
			return TRUE;
		}else{
			wk->time_wait--;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�͂��ꃁ�b�Z�[�W�\��
	case SEQ_SUB_GIN_OK_HAZURE_MSG_WAIT:

		//���ԂŃ��b�Z�[�W�����Ă���
		if( wk->time_wait == 0 ){
			GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_TOUCH, VISIBLE_OFF );	//���ʃI�t
			Scratch_SeqSubNextCard( wk );
			Scratch_SeqSubGinOk( wk );
			wk->time_wait = SCRATCH_GIN_ALL_CLEAR_WAIT;
			wk->sub_seq = SEQ_SUB_GIN_OK_GIN_ALL_CLEAR_WAIT;
		}else{
			wk->time_wait--;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�͂���ŋ┓��S�ăN���A�����E�F�C�g
	case SEQ_SUB_GIN_OK_GIN_ALL_CLEAR_WAIT:
		if( wk->time_wait == 0 ){
			return TRUE;
		}else{
			wk->time_wait--;
		}
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//���̃J�[�h�փE�F�C�g
	case SEQ_SUB_NEXT_WAIT:
		ret = TOUCH_SW_MainMC( wk->touch_subwin );

		//�����Ȃ�
		//if( ret == TOUCH_SW_RET_NORMAL ){
			//
		//}

		//�u�͂��v�������u��
		//if( ret == TOUCH_SW_RET_YES_TOUCH ){
		if( ret == TOUCH_SW_RET_YES ){
			ScratchObj_SetAnmFlag( wk->p_d_button, 0 );		//�{�^���A�j��OFF
			BmpMenuWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );
			GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TALK] );
			Scratch_SeqSubNextCard( wk );
			Scratch_FreeTouchSub( wk );
			return TRUE;

		//�u�������v�������u��
		//}else if( ret == TOUCH_SW_RET_NO_TOUCH ){
		}else if( ret == TOUCH_SW_RET_NO ){
			ScratchObj_SetAnmFlag( wk->p_d_button, 0 );		//�{�^���A�j��OFF
			BmpMenuWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );
			GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TALK] );
			Scratch_FreeTouchSub( wk );
			wk->sub_seq = SEQ_SUB_YAMERU_WAIT_NO;
		}

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//��߂�E�F�C�g
	case SEQ_SUB_YAMERU_WAIT:
		ret = TOUCH_SW_MainMC( wk->touch_subwin );

		//�����Ȃ�
		//if( ret == TOUCH_SW_RET_NORMAL ){
			//
		//}

		//�u�͂��v�������u��
		//if( ret == TOUCH_SW_RET_YES_TOUCH ){
		if( ret == TOUCH_SW_RET_YES ){
			ScratchObj_SetAnmFlag( wk->p_d_button, 0 );		//�{�^���A�j��OFF
			BmpMenuWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );
			GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TALK] );
			Scratch_SeqSubYameruCard( wk );
			Scratch_FreeTouchSub( wk );
			return TRUE;

		//�u�������v�������u��
		//}else if( ret == TOUCH_SW_RET_NO_TOUCH ){
		}else if( ret == TOUCH_SW_RET_NO ){
			ScratchObj_SetAnmFlag( wk->p_d_button, 0 );		//�{�^���A�j��OFF
			BmpMenuWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );
			GF_BGL_BmpWinOff( &wk->bmpwin[BMPWIN_TALK] );
			Scratch_FreeTouchSub( wk );
			wk->sub_seq = SEQ_SUB_YAMERU_WAIT_NO;
		}

		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//��߂�E�F�C�g�u�������v
	case SEQ_SUB_YAMERU_WAIT_NO:
		if( GF_TP_GetCont() == FALSE ){			//��x��ʂ��痣���Ă��玟�֐i��
			wk->sub_seq = SEQ_SUB_TYPE_SEL;
		}
		break;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�Q�[���J�[�h���A�E�g
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameCardTouchOut( SCRATCH_WORK* wk )
{
	int i,pos;

	enum{
		SEQ_SUB_CARD_TOUCH_OUT_START = 0,
		SEQ_SUB_CARD_TOUCH_OUT,
		SEQ_SUB_CARD_TOUCH_OUT_END,
	};

	//�ǂ̃J�[�h��I�񂾂�
	pos = wk->card_col[wk->card_num-1];
	OS_Printf( "pos = %d\n", pos );
					
	switch( wk->sub_seq ){

	//
	case SEQ_SUB_CARD_TOUCH_OUT_START:
		GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_MSG, VISIBLE_OFF );		//�����b�Z�[�W�I�t
		GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_MSG, VISIBLE_OFF );		//�チ�b�Z�[�W�I�t

		//���̃J�[�h��(��߂�)�{�^��
		ScratchObj_Vanish( wk->p_d_button, SCRATCH_VANISH_ON );				//�I�t

		wk->counter	= 0;
		wk->counter2= 0;

		Snd_SePlay( SE_D_CARD_OUT );
		wk->sub_seq = SEQ_SUB_CARD_TOUCH_OUT;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�A�E�g
	case SEQ_SUB_CARD_TOUCH_OUT:
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_U_YAKU, GF_BGL_SCROLL_X_SET, wk->counter2 );
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_D_TOUCH, GF_BGL_SCROLL_X_SET, wk->counter2 );
		GF_BGL_ScrollSet( wk->bgl, SCRATCH_FRAME_D_SUB, GF_BGL_SCROLL_X_SET, wk->counter2 );
		wk->counter2 += SCRATCH_CARD_TOUCH_OUT_SPD;

		//if( wk->counter2 <= SCRATCH_CARD_TOUCH_OUT_OFFSET_X ){
		if( wk->counter >= SCRATCH_CARD_OUT_OFFSET_X ){
			wk->sub_seq = SEQ_SUB_CARD_TOUCH_OUT_END;
		}else{
			//��F�|�P����OBJ�\��
			for( i=0; i < SCRATCH_U_POKE_MAX ;i++ ){
				ScratchObj_SetObjPos(	wk->p_u_poke[i], 
						(SCRATCH_U_POKE_X + wk->counter),
						(SCRATCH_U_POKE_Y + (i * SCRATCH_U_POKE_WIDTH_Y)) );
			}

#if 0
			//���F���NOBJ�\��
			for( i=0; i < wk->kezuri_card_num ;i++ ){
				ScratchObj_SetObjPos(wk->p_d_waku[i], 
					(d_poke_pos[ wk->kezuri_card[i] ].x + wk->counter + SCRATCH_GIN_WAKU_OFFSET_X), 
					d_poke_pos[ wk->kezuri_card[i] ].y + SCRATCH_GIN_WAKU_OFFSET_Y );

				ScratchObj_Vanish( wk->p_d_waku[i], SCRATCH_VANISH_ON );		//��\��
			}
#else
			for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
				ScratchObj_Vanish( wk->p_d_waku[i], SCRATCH_VANISH_ON );		//��\��
			}
#endif

			//���F�|�P����OBJ�\��
			for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
				ScratchObj_SetObjPos(	wk->p_d_poke[i], (d_poke_pos[i].x + wk->counter), 
										d_poke_pos[i].y );
			}
		}

		wk->counter += SCRATCH_CARD_OUT_SPD;
		break;

	//////////////////////////////////////////////////////////////////////////////////////
	//�J�[�h�A�E�g�I��
	case SEQ_SUB_CARD_TOUCH_OUT_END:
		wk->counter	= 0;
		wk->counter2= 0;
		return TRUE;

	};

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�V�[�P���X�F�I����
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���ցAFALSE = �p��"
 */
//--------------------------------------------------------------
static BOOL Seq_GameEnd( SCRATCH_WORK* wk )
{
	int i;

	switch( wk->sub_seq ){

	case 0:
		wk->wait = SCRATCH_END_WAIT;
		wk->sub_seq++;
		break;

	//�t�F�[�h�A�E�g
	case 1:
		wk->wait--;
		if( wk->wait == 0 ){
			BmpTalkWinClear( &wk->bmpwin[BMPWIN_TALK], WINDOW_TRANS_ON );
			WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_FADEOUT, WIPE_TYPE_FADEOUT, 
					WIPE_FADE_BLACK, WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_SCRATCH );
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


//==============================================================================================
//
//	���ʏ���
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	����ʃ��N�ǉ�
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_WakuAdd( SCRATCH_WORK* wk )
{
	int i;

	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){

		wk->p_d_waku[i] = ScratchObj_Create(&wk->scratch_clact, SCRATCH_ID_OBJ_D_BUTTON, 
											ANM_GIN_WAKU, d_poke_pos[i].x, d_poke_pos[i].y, 
											DISP_MAIN,
											SCRATCH_OBJ_BG_PRI_L, 
											(SCRATCH_OBJ_PRI_N + 1) );
											//SCRATCH_OBJ_PRI_H );
		ScratchObj_Vanish( wk->p_d_waku[i], SCRATCH_VANISH_ON );		//��\��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʓ�����|�P�����ǉ�
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_AtariAdd( SCRATCH_WORK* wk )
{
	int i;

	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){

		wk->p_d_atari[i] = ScratchObj_Create(&wk->scratch_clact, SCRATCH_ID_OBJ_D_POKE, 
											ANM_POKE_REE, d_atari_pos[i].x, d_atari_pos[i].y, 
											DISP_MAIN,
											SCRATCH_OBJ_BG_PRI_H, 
											SCRATCH_OBJ_PRI_H );
		ScratchObj_Vanish( wk->p_d_atari[i], SCRATCH_VANISH_ON );		//��\��
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʃ{�^���ǉ�
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_ButtonAdd( SCRATCH_WORK* wk )
{
	//���F�{�^��OBJ�ǉ�
	wk->p_d_button = ScratchObj_Create( &wk->scratch_clact, SCRATCH_ID_OBJ_D_BUTTON, 
										ANM_BUTTON_NEXT, d_next_button_pos.x, d_next_button_pos.y, 
										DISP_MAIN,
										SCRATCH_OBJ_BG_PRI_N, 
										SCRATCH_OBJ_PRI_H );
	ScratchObj_Vanish( wk->p_d_button, SCRATCH_VANISH_ON );		//��\��

	//���F�^�񒆃E�B���h�EOBJ�ǉ�
	wk->p_d_win = ScratchObj_Create(&wk->scratch_clact, SCRATCH_ID_OBJ_D_BUTTON, 
									ANM_CENTER_WIN, d_win_pos.x, d_win_pos.y, 
									DISP_MAIN,
									SCRATCH_OBJ_BG_PRI_N, 
									SCRATCH_OBJ_PRI_H );
	ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_ON );		//��\��

	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʃJ�[�h�폜
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_CardDelete( SCRATCH_WORK* wk )
{
	int i;

	for( i=0; i < SCRATCH_D_CARD_MAX ;i++ ){
		if( wk->p_d_card[i] != NULL ){
			ScratchObj_Delete( wk->p_d_card[i] );
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� �폜
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScratchCommon_Delete( SCRATCH_WORK* wk )
{
	int i;

	for( i=0; i < SCRATCH_U_POKE_MAX ;i++ ){
		if( wk->p_u_poke[i] != NULL ){
			ScratchObj_Delete( wk->p_u_poke[i] );
		}
	}

	for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
		if( wk->p_d_poke[i] != NULL ){
			ScratchObj_Delete( wk->p_d_poke[i] );
		}
	}

	if( wk->p_d_button != NULL ){
		ScratchObj_Delete( wk->p_d_button );
	}

	if( wk->p_d_win != NULL ){
		ScratchObj_Delete( wk->p_d_win );
	}

	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
		if( wk->p_d_waku[i] != NULL ){
			ScratchObj_Delete( wk->p_d_waku[i] );
		}
	}

	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
		if( wk->p_d_atari[i] != NULL ){
			ScratchObj_Delete( wk->p_d_atari[i] );
		}
	}

	if( wk->p_d_atari_win != NULL ){
		ScratchObj_Delete( wk->p_d_atari_win );
	}

	//�{�^���t�H���g�폜
	FontProc_UnloadFont( FONT_BUTTON );
	
	//�p���b�g�t�F�[�h�J��
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_OBJ );
	PaletteFadeWorkAllocFree( wk->pfd, FADE_MAIN_BG );

	//�p���b�g�t�F�[�h�V�X�e���J��
	PaletteFadeFree( wk->pfd );
	wk->pfd = NULL;

	ScratchClact_DeleteCellObject(&wk->scratch_clact);		//2D�I�u�W�F�N�g�֘A�̈�J��

	MSG_DsIconFlashDelete( wk->msgicon );
	MSGMAN_Delete( wk->msgman );							//���b�Z�[�W�}�l�[�W���J��
	WORDSET_Delete( wk->wordset );
	STRBUF_Delete( wk->msg_buf );							//���b�Z�[�W�o�b�t�@�J��
	STRBUF_Delete( wk->tmp_buf );							//���b�Z�[�W�o�b�t�@�J��
	NUMFONT_Delete( wk->num_font );

	sys_FreeMemoryEz( wk->p_char );							//�L�����N�^�[�f�[�^
	sys_FreeMemoryEz( wk->buf );							//�L�����N�^�[�f�[�^���R�s�[�����o�b�t�@

	ScratchExitBmpWin( wk->bmpwin );						//BMP�E�B���h�E�J��
	Scratch_BgExit( wk->bgl );								//BGL�폜
	ArchiveDataHandleClose( wk->hdl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���A
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_Recover( SCRATCH_WORK* wk )
{
	int i;

	Scratch_InitSub1();

	wk->bgl	= GF_BGL_BglIniAlloc( HEAPID_SCRATCH );

	Scratch_InitSub2( wk );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���� ������1
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_InitSub1( void )
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_InitSub2( SCRATCH_WORK* wk )
{
	int i,flip;

	wk->hdl= ArchiveDataHandleOpen( ARC_SCRATCH, HEAPID_SCRATCH );
	Scratch_BgInit( wk );								//BG������
	Scratch_ObjInit( wk );								//OBJ������

	//���b�Z�[�W�f�[�^�}�l�[�W���[�쐬
	wk->msgman = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, 
								NARC_msg_scratch_contents_dat, HEAPID_SCRATCH );

	wk->wordset = WORDSET_Create( HEAPID_SCRATCH );
	wk->msg_buf = STRBUF_Create( SCRATCH_MSG_BUF_SIZE, HEAPID_SCRATCH );
	wk->tmp_buf = STRBUF_Create( SCRATCH_MSG_BUF_SIZE, HEAPID_SCRATCH );

	//�t�H���g�p���b�g
	SystemFontPaletteLoad( PALTYPE_MAIN_BG, BS_FONT_PAL * 32, HEAPID_SCRATCH );
	SystemFontPaletteLoad( PALTYPE_SUB_BG, BS_FONT_PAL * 32, HEAPID_SCRATCH );
	TalkFontPaletteLoad( PALTYPE_MAIN_BG, BS_MSGFONT_PAL * 32, HEAPID_SCRATCH );
	TalkFontPaletteLoad( PALTYPE_SUB_BG, BS_MSGFONT_PAL * 32, HEAPID_SCRATCH );

	//�{�^���t�H���g���[�h
	FontProc_LoadFont( FONT_BUTTON, HEAPID_SCRATCH );

	//8x8�t�H���g�쐬(�g�pbmpwin[BMPWIN_TYPE]�̃p���b�g(BS_FONT_PAL)���g�p
	wk->num_font = NUMFONT_Create( 15, 14, FBMP_COL_NULL, HEAPID_SCRATCH );

	//�r�b�g�}�b�v�ǉ�
	ScratchAddBmpWin( wk->bgl, wk->bmpwin );

	//�㉺�t�Ȃ̂Œ���
	wk->msgicon = MSG_DsIconFlashAdd( NULL, MSG_DSI_SUBDISP, BS_MSGFONT_PAL, HEAPID_SCRATCH );

	//�L�����N�^�[�f�[�^���o�b�t�@�ɓǍ��A�]��(���ƂŊJ������)
	Scratch_Touch_0( wk );
			
	//�o�b�t�@�𐶐�(���ƂŊJ������)
	Scratch_Touch_1( wk );

	//�L�����N�^�[�f�[�^���R�s�[
	Scratch_Touch_1_5( wk );

	GF_Disp_DispOn();

	//��F�|�P����OBJ�ǉ�
	for( i=0; i < SCRATCH_U_POKE_MAX ;i++ ){
		wk->p_u_poke[i] = ScratchObj_Create(&wk->scratch_clact, SCRATCH_ID_OBJ_U_POKE, 
											ANM_POKE_REE + i, 
											SCRATCH_U_POKE_X,
											SCRATCH_U_POKE_Y + (i * SCRATCH_U_POKE_WIDTH_Y),
											DISP_SUB,
											SCRATCH_OBJ_BG_PRI_H, 
											SCRATCH_OBJ_PRI_H );
		ScratchObj_Vanish( wk->p_u_poke[i], SCRATCH_VANISH_ON );		//��\��
	}

	//���F�|�P����OBJ�ǉ�
	for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
		wk->p_d_poke[i] = ScratchObj_Create(&wk->scratch_clact, SCRATCH_ID_OBJ_D_POKE, 
											ANM_POKE_REE, d_poke_pos[i].x, d_poke_pos[i].y, 
											DISP_MAIN,
											SCRATCH_OBJ_BG_PRI_L, 
											SCRATCH_OBJ_PRI_N );
		ScratchObj_Vanish( wk->p_d_poke[i], SCRATCH_VANISH_ON );		//��\��
	}

	Scratch_ButtonAdd( wk );					//���F�{�^��OBJ�ǉ�

	//���F�J�[�hOBJ�ǉ�
	for( i=0; i < SCRATCH_D_CARD_MAX ;i++ ){
		wk->p_d_card[i] = ScratchObj_Create(&wk->scratch_clact, SCRATCH_ID_OBJ_D_CARD, 
											(ANM_CARD_BLUE + i), d_card_pos[i].x, d_card_pos[i].y, 
											DISP_MAIN,
											//SCRATCH_OBJ_BG_PRI_H, 
											SCRATCH_OBJ_BG_PRI_N, 
											SCRATCH_OBJ_PRI_H );
		//ScratchObj_Vanish( wk->p_d_card[i], SCRATCH_VANISH_ON );		//��\��
		
		//��ʊO�ɂ��Ă���
		ScratchObj_SetObjPos(	wk->p_d_card[i], (d_card_pos[i].x + SCRATCH_CARD_IN_OFFSET_X), 
								d_card_pos[i].y );

		ScratchObj_Priority( wk->p_d_card[i], (SCRATCH_OBJ_PRI_L+i) );
	}

	//������E�B���h�E
	wk->p_d_atari_win = ScratchObj_Create(	&wk->scratch_clact, SCRATCH_ID_OBJ_D_ATARI, 
											0, 
											d_atari_win_pos.x,
											d_atari_win_pos.y,
											DISP_MAIN,
											SCRATCH_OBJ_BG_PRI_H, 
											SCRATCH_OBJ_PRI_N );
	ScratchObj_Vanish( wk->p_d_atari_win, SCRATCH_VANISH_ON );		//��\��

#if 1
	//VRAM���蓖�Ă�C=128,I=16�����Ȃ��āAC�͒ʐM�p�Ŏg�p�s�Ȃ̂ŁB

	//��ʏo�͂��㉺����ւ���
	GX_SetDispSelect( GX_DISP_SELECT_SUB_MAIN );
#endif

	InitTPSystem();										// �^�b�`�p�l���V�X�e��������
	InitTPNoBuff(1);
	//InitTPNoBuff(2);
	//InitTPNoBuff(3);
	//InitTPNoBuff(4);

	sys_VBlankFuncChange( VBlankFunc, (void*)wk );		//VBlank�Z�b�g
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʁu�͂��A�������v�^�b�`�V�X�e���m��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_AllocTouchSub( SCRATCH_WORK* wk )
{
	wk->touch_subwin = TOUCH_SW_AllocWork( HEAPID_SCRATCH );	//�m��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʁu�͂��A�������v�^�b�`�V�X�e��������
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_InitTouchSub( SCRATCH_WORK* wk )
{
	TOUCH_SW_PARAM cp_param;

	cp_param.p_bgl		= wk->bgl;				//BGL�V�X�e�����[�N
	cp_param.bg_frame	= SCRATCH_FRAME_D_MSG;	//BG�i���o�[
	//cp_param.char_offs	= (1024 - 64);			//�L�����N�^�]���I�t�Z�b�g((��׸��P�� 1/32byte)
	cp_param.char_offs	= (1024 - 128);			//�L�����N�^�]���I�t�Z�b�g((��׸��P�� 1/32byte)
	cp_param.pltt_offs	= SCRATCH_TOUCH_SUB_PAL;//�p���b�g�]���I�t�Z�b�g(��گ�1�{���P�� 1/32byte)
	cp_param.x			= 24;					//x���W(��׸��P��)
	cp_param.y			= 8;					//y���W(��׸��P��)

	TOUCH_SW_Init( wk->touch_subwin, &cp_param );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʁu�͂��A�������v�^�b�`�V�X�e���폜
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_FreeTouchSub( SCRATCH_WORK* wk )
{
	TOUCH_SW_FreeWork( wk->touch_subwin );
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_BgInit( SCRATCH_WORK* wk )
{
	SetVramBank();
	SetBgHeader( wk->bgl );

	//�p���b�g�t�F�[�h�V�X�e�����[�N�쐬
	wk->pfd = PaletteFadeInit( HEAPID_SCRATCH );

	//���N�G�X�g�f�[�^��malloc���ăZ�b�g
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_OBJ, FADE_PAL_ALL_SIZE, HEAPID_SCRATCH );
	PaletteFadeWorkAllocSet( wk->pfd, FADE_MAIN_BG, FADE_PAL_ALL_SIZE, HEAPID_SCRATCH );

	//���ʔw�i
	Scratch_SetMainBgGraphic( wk, SCRATCH_FRAME_U_BG );
	Scratch_SetMainBgPalette();

	//���ʖ�
	Scratch_SetYakuBgGraphic( wk, SCRATCH_FRAME_U_YAKU );

	//����ʃo�b�N
	//Scratch_SetBackBgGraphic( wk, SCRATCH_FRAME_D_MSG );

	//����ʔw�i�A�p���b�g�Z�b�g
	Scratch_SetSubBgGraphic( wk, SCRATCH_FRAME_D_SUB );
	Scratch_SetSubBgPalette();

	//����ʃ^�b�`�A�p���b�g�Z�b�g
	Scratch_SetTouchBgGraphic( wk, SCRATCH_FRAME_D_TOUCH );

	//����ʓ�����E�B���h�E
	//Scratch_SetAtariBgGraphic( wk, SCRATCH_FRAME_D_ATARI );

	GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_MSG, VISIBLE_OFF );		//�����b�Z�[�W�I�t
	GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_MSG, VISIBLE_OFF );		//�チ�b�Z�[�W�I�t
	return;
}

//--------------------------------------------------------------
/**
 * @brief	OBJ�֘A������
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_ObjInit( SCRATCH_WORK* wk )
{
	ScratchClact_InitCellActor(	&wk->scratch_clact );
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
static void Scratch_BgExit( GF_BGL_INI * ini )
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
	GF_BGL_BGControlExit( ini, SCRATCH_FRAME_U_BG );
	GF_BGL_BGControlExit( ini, SCRATCH_FRAME_U_YAKU );
	GF_BGL_BGControlExit( ini, SCRATCH_FRAME_U_MSG );
	GF_BGL_BGControlExit( ini, SCRATCH_FRAME_D_SUB );
	GF_BGL_BGControlExit( ini, SCRATCH_FRAME_D_TOUCH );
	GF_BGL_BGControlExit( ini, SCRATCH_FRAME_D_MSG );
	//GF_BGL_BGControlExit( ini, SCRATCH_FRAME_D_ATARI );

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
	SCRATCH_WORK* wk = work;

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
		GX_VRAM_BG_128_A,				//���C��2D�G���W����BG
		GX_VRAM_BGEXTPLTT_NONE,			//���C��2D�G���W����BG�g���p���b�g

		GX_VRAM_SUB_BG_128_C,			//�T�u2D�G���W����BG
		GX_VRAM_SUB_BGEXTPLTT_NONE,		//�T�u2D�G���W����BG�g���p���b�g

		GX_VRAM_OBJ_128_B,				//���C��2D�G���W����OBJ
		GX_VRAM_OBJEXTPLTT_NONE,		//���C��2D�G���W����OBJ�g���p���b�g

		GX_VRAM_SUB_OBJ_16_I,			//�T�u2D�G���W����OBJ
		GX_VRAM_SUB_OBJEXTPLTT_NONE,	//�T�u2D�G���W����OBJ�g���p���b�g

		GX_VRAM_TEX_NONE,				//�e�N�X�`���C���[�W�X���b�g
		GX_VRAM_TEXPLTT_NONE			//�e�N�X�`���p���b�g�X���b�g
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

	//����128k=0x20000���Ɏ��߂�
	//--------------------------------------------------------------------------------
	{	//BG(�t�H���g)(��b�A���j���[)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_U_MSG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( SCRATCH_FRAME_U_MSG, 32, 0, HEAPID_SCRATCH );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_U_MSG );
	}

	//--------------------------------------------------------------------------------
	{	//����(�w�i)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			3, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_U_BG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_U_BG );
	}

	//--------------------------------------------------------------------------------
	{	//����(��)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_U_YAKU, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_U_YAKU );
	}

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�����128k=0x20000���Ɏ��߂�
	//--------------------------------------------------------------------------------
	{	//�����(�w�i)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			//GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
			GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01,
			2, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_D_SUB, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_D_SUB );
	}

	//--------------------------------------------------------------------------------
	{	//�����(�^�b�`)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x0c000, GX_BG_EXTPLTT_01,
			1, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_D_TOUCH, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_D_TOUCH );
	}

	//--------------------------------------------------------------------------------
	{	//�����(���b�Z�[�W)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x2000, GX_BG_CHARBASE_0x14000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_D_MSG, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( SCRATCH_FRAME_D_MSG, 32, 0, HEAPID_SCRATCH );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_D_MSG );
	}

	//--------------------------------------------------------------------------------
#if 0
	{	//�����(������E�B���h�E)
		GF_BGL_BGCNT_HEADER TextBgCntDat = {
			0, 0, 0x800, 0, GF_BGL_SCRSIZ_256x256, GX_BG_COLORMODE_16,
			GX_BG_SCRBASE_0x3000, GX_BG_CHARBASE_0x18000, GX_BG_EXTPLTT_01,
			0, 0, 0, FALSE
		};
		GF_BGL_BGControlSet( ini, SCRATCH_FRAME_D_ATARI, &TextBgCntDat, GF_BGL_MODE_TEXT );
		GF_BGL_ClearCharSet( SCRATCH_FRAME_D_ATARI, 32, 0, HEAPID_SCRATCH );
		GF_BGL_ScrClear( ini, SCRATCH_FRAME_D_ATARI );
	}
#endif

	//G2_SetBG0Priority( 0 );

	GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_TOUCH, VISIBLE_OFF );			//���ʃI�t
	GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_SUB, VISIBLE_OFF );			//�J�[�h�ʃI�t
	//GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_ATARI, VISIBLE_OFF );			//������E�B���I�t
	GF_Disp_GXS_VisibleControl( SCRATCH_U_MASK_YAKU, VISIBLE_OFF );			//���I�t
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SetMainBgGraphic( SCRATCH_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_u_bg00_NCGR, 
							wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );

	ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_u_bg00_NSCR, 
						wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F���ʔw�i(��)
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SetYakuBgGraphic( SCRATCH_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_u_bg01_NCGR, 
							wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );

	ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_u_bg01_NSCR, 
						wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );
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
static void Scratch_SetMainBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet( ARC_SCRATCH, NARC_scratch_u_bg_NCLR, &dat, HEAPID_SCRATCH );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*5) );
	//GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*5) );		//���C��
	GXS_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*5) );		//�T�u

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����ʔw�i�p���b�g�ݒ�
 *
 * @param	none
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SetSubBgPalette( void )
{
	void *buf;
	NNSG2dPaletteData *dat;

	buf = ArcUtil_PalDataGet( ARC_SCRATCH, NARC_scratch_l_bg_NCLR, &dat, HEAPID_SCRATCH );

	DC_FlushRange( dat->pRawData, (sizeof(u16)*16*9) );
	//GXS_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*9) );		//�T�u
	//GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*9) );		//���C��
	GX_LoadBGPltt( dat->pRawData, 0, (sizeof(u16)*16*10) );		//���C��

	sys_FreeMemoryEz(buf);
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʃo�b�N
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SetBackBgGraphic( SCRATCH_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_l_bg00_NCGR, 
							wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );

	ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_l_bg00_NSCR, 
						wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʔw�i
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SetSubBgGraphic( SCRATCH_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_l_bg01_NCGR, 
							wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );

	ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_l_bg01_NSCR, 
						wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʍ���
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SetTouchBgGraphic( SCRATCH_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_l_bg01gin_NCGR, 
	//ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_l_bg00_NCGR, 
	//ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_l_bg01_NCGR, 
							wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );

	ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_l_bg01gin_NSCR, 
	//ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_l_bg00_NSCR, 
	//ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_l_bg01_NSCR, 
						wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�O���t�B�b�N�f�[�^�Z�b�g�F����ʓ�����E�B���h�E
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	frm		�t���[���i���o�[
 *
 * @return	none
 */
//--------------------------------------------------------------
#if 0
static void Scratch_SetAtariBgGraphic( SCRATCH_WORK * wk, u32 frm  )
{
	ArcUtil_HDL_BgCharSet(	wk->hdl, NARC_scratch_l_bg01win_NCGR, 
							wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );

	ArcUtil_HDL_ScrnSet(wk->hdl, NARC_scratch_l_bg01win_NSCR, 
						wk->bgl, frm, 0, 0, 0, HEAPID_SCRATCH );
	return;
}
#endif


//==============================================================================================
//
//	���b�Z�[�W�֘A
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���b�Z�[�W�\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
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
static u8 ScratchWriteMsg( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
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
static u8 ScratchWriteMsgSimple( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, int msg_id, u32 x, u32 y, u32 wait, u8 f_col, u8 s_col, u8 b_col, u8 font )
{
	MSGMAN_GetString( wk->msgman, msg_id, wk->tmp_buf );

	//�o�^���ꂽ�P����g���ĕ�����W�J����
	WORDSET_ExpandStr( wk->wordset, wk->msg_buf, wk->tmp_buf );

	return GF_STR_PrintColor( win, font, wk->msg_buf, x, y, wait, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );
}

//--------------------------------------------------------------
/**
 * @brief	�m�F���b�Z�[�W�\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_KakuninMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	ScratchWriteMenuWin( wk->bgl, &wk->bmpwin[BMPWIN_TALK] );
	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_TALK], msg_scratch_contents_07, 
								1, 1, MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, BS_FONT );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_TALK] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	���܂��ߕ\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
#if 0
static u8 Scratch_CardNumMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	//���܂��߂��Z�b�g
	Scratch_SetNumber( wk, 0, (wk->card_num+1) );

	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_START], msg_scratch_contents_01, 
								8*4, 1+4, MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BS_FONT );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_START] );
	return msg_index;
}
#endif

//--------------------------------------------------------------
/**
 * @brief	���܂��߂́@�J�[�h���@�����ł�������
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_CardSelectMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	//���܂��߂��Z�b�g
	Scratch_SetNumber( wk, 0, (wk->card_num+1) );

	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_SELECT], msg_scratch_contents_13, 
								1, 1, MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_WHITE, FONT_TALK );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_SELECT] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	�X�N���b�`�X�^�[�g�\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_StartMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	//���܂��߂��Z�b�g
	Scratch_SetNumber( wk, 0, (wk->card_num+1) );

	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_START], msg_scratch_contents_10, 
								8*1-1, 1+4, MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BS_FONT );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_START] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	�R��������I�\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_AtariMsg( SCRATCH_WORK* wk )
{
	u8 msg_index,i;

	msg_index = 0;

	//������|�P�����ɕύX���ĕ\��
	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
		ScratchObj_AnmChg( wk->p_d_atari[i], wk->card_poke[ wk->kezuri_card[i] ] );
		ScratchObj_Vanish( wk->p_d_atari[i], SCRATCH_VANISH_OFF );		//�\��
	}

	ScratchObj_Vanish( wk->p_d_atari_win, SCRATCH_VANISH_OFF );			//�\��
	ScratchObj_SetAnmFlag( wk->p_d_atari_win, 1 );						//�I�[�g�A�j��
	//GF_Disp_GX_VisibleControl( SCRATCH_D_MASK_ATARI, VISIBLE_ON );		//��������E�B���h�E�I��

	Snd_SePlay( SE_D_CARD_ATARI );

	/*�����胁�b�Z�[�W�͂���̂ł́H*/
	//msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_ATARI], msg_scratch_contents_11, 
	//							8*2, 1+4, MSG_ALLPUT, 
	//							FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BS_FONT );

	//GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_ATARI] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	����˂�\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_HazureMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	Snd_SePlay( SE_D_CARD_HAZURE );
	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_HAZURE], msg_scratch_contents_12, 
								8*3-1, 1+4, MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BS_FONT );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_HAZURE] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	���̃J�[�h�֕\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_NextMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_NEXT], msg_scratch_contents_05, 
								4, 1, MSG_ALLPUT, 
								//FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_BUTTON );
								FBMP_COL_BLACK,FBMP_COL_BLACK,FBMP_COL_NULL, FONT_BUTTON );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_NEXT] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	��߂�\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
static u8 Scratch_YameruMsg( SCRATCH_WORK* wk )
{
	u8 msg_index;

	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_YAMERU], msg_scratch_contents_06, 
								4, 1, MSG_ALLPUT, 
								//FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, FONT_BUTTON );
								FBMP_COL_BLACK,FBMP_COL_BLACK,FBMP_COL_NULL, FONT_BUTTON );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_YAMERU] );
	return msg_index;
}

//--------------------------------------------------------------
/**
 * @brief	��\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	no		���Ԗڂ̖���
 * @param	item	�A�C�e���i���o�[
 *
 * @return	"msg_index"
 */
//--------------------------------------------------------------
//static const u8 yaku_offset_y[SCRATCH_ATARI_MAX] = { 0, 4, 0, 4 };		//�\���I�t�Z�b�g
static const u8 yaku_offset_y[SCRATCH_ATARI_MAX] = { 10, 12, 6, 8 };		//�\���I�t�Z�b�g
#define SCRATCH_ITEM_NUM_X	(8*3)

static u8 Scratch_YakuMsg( SCRATCH_WORK* wk, u8 no )
{
	u8 msg_index;

	//�A�C�e����
	WORDSET_RegisterItemName( wk->wordset, 0, wk->atari_item[no] );
	msg_index = ScratchWriteMsg(wk, &wk->bmpwin[BMPWIN_ITEM1+no], msg_scratch_contents_02, 
								4, yaku_offset_y[no], MSG_ALLPUT, 
								FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BS_FONT );

	//�A�C�e����
	//msg_index = ScratchWriteMsgSimple(wk, &wk->bmpwin[BMPWIN_ITEM1+no], msg_scratch_contents_03, 
	//							SCRATCH_ITEM_NUM_X, 16+yaku_offset_y[no], MSG_ALLPUT, 
	//							FBMP_COL_BLACK,FBMP_COL_BLK_SDW,FBMP_COL_NULL, BS_FONT );

	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_ITEM1+no] );
	return msg_index;
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	number	�Z�b�g���鐔�l
 *
 * @retval	none
 *
 * �����Œ�ɂ��Ă���
 */
//--------------------------------------------------------------
static void Scratch_SetNumber( SCRATCH_WORK* wk, u32 bufID, s32 number )
{
	WORDSET_RegisterNumber( wk->wordset, bufID, number, 1, 
							NUMBER_DISPTYPE_LEFT, NUMBER_CODETYPE_DEFAULT );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P���������Z�b�g
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 * @param	ppp		POKEMON_PASO_PARAM
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Scratch_SetPokeName( SCRATCH_WORK* wk, u32 bufID, POKEMON_PASO_PARAM* ppp )
{
	WORDSET_RegisterPokeMonsName( wk->wordset, bufID, ppp );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[�����Z�b�g
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	bufID	�o�b�t�@ID
 *
 * @retval	none
 */
//--------------------------------------------------------------
static void Scratch_SetPlayerName( SCRATCH_WORK* wk, u32 bufID )
{
	WORDSET_RegisterPlayerName( wk->wordset, bufID, SaveData_GetMyStatus(wk->sv) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�v���C���[����\��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
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
static void PlayerNameWrite( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 font )
{
	u32 col;
	const MYSTATUS* my;
	STRBUF* player_buf;								//�v���C���[���o�b�t�@�|�C���^

	my = SaveData_GetMyStatus( wk->sv );
	player_buf = STRBUF_Create( PLAYER_NAME_BUF_SIZE, HEAPID_SCRATCH );
	
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
 * @brief	�|�P��������\��(���ʎw��)
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
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
static void PokeNameWriteEx( SCRATCH_WORK* wk, GF_BGL_BMPWIN* win, u32 x, u32 y, u8 f_col, u8 s_col, u8 b_col, u8 font, u16 monsno, u8 sex )
{
	u8 x_pos;
	u32 msg_id,col;
	MSGDATA_MANAGER* man;
	STRBUF* buf;
	POKEMON_PARAM* poke;
	STRCODE sel_poke_buf[POKE_NAME_BUF_SIZE];					//�|�P�������o�b�t�@�|�C���^

	GF_BGL_BmpWinDataFill( win, b_col );						//�h��Ԃ�

	//�|�P���������擾
	man = MSGMAN_Create( MSGMAN_TYPE_DIRECT, ARC_MSG, NARC_msg_itemname_dat, HEAPID_SCRATCH );
	buf = MSGMAN_AllocString( man, monsno );
	MSGMAN_Delete( man );

	GF_STR_PrintColor( win, font, buf, x, y, MSG_ALLPUT, 
								GF_PRINTCOLOR_MAKE(f_col,s_col,b_col), NULL );

	STRBUF_Delete( buf );

	GF_BGL_BmpWinOn( win );
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	seq		�V�[�P���X�̃|�C���^
 * @param	next	���̃V�[�P���X��`
 *
 * @return	none
 */
//--------------------------------------------------------------
static void NextSeq( SCRATCH_WORK* wk, int* seq, int next )
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
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void BgCheck( SCRATCH_WORK* wk )
{
	if( sys.cont & PAD_KEY_UP ){
		if( sys.cont & PAD_BUTTON_L ){
			//GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_ON );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_R ){
			//GF_Disp_GX_VisibleControl( GX_PLANEMASK_OBJ, VISIBLE_OFF );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_OFF );
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
		if( sys.cont & PAD_BUTTON_L ){
			//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_ON );
		}else if( sys.cont & PAD_BUTTON_R ){
			//GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
			GF_Disp_GXS_VisibleControl( GX_PLANEMASK_BG1, VISIBLE_OFF );
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
 * @brief	�J�[�\���w�擾
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
static u16 GetCsrX( SCRATCH_WORK* wk )
{
	return wk->csr_pos;
}

//�����_�������܂������Ȃ��������̔z�u��
#if 0
static const u8 atari_poke_tbl[SCRATCH_D_POKE_MAX][SCRATCH_EASY_ATARI_NUM] = {
#if 0
	//4�̓Z���^�[�Ȃ̂ŕK�������
	{ 1,2,3,4,7 },
	{ 2,3,4,5,6 },
	{ 1,2,4,5,7 },
	{ 1,2,4,7,6 },
	{ 1,3,4,5,8 },
	{ 0,1,2,4,7 },
	{ 2,4,5,6,8 },
	{ 0,1,2,4,7 },
	{ 3,4,5,6,8 },
#else
	//4�̓Z���^�[�Ȃ̂ŕK�������
	{ 1,2,3,4 },
	{ 2,3,4,5 },
	{ 1,2,4,5 },
	{ 1,2,4,7 },
	{ 1,3,4,5 },
	{ 0,1,2,4 },
	{ 2,4,5,6 },
	{ 0,1,2,4 },
	{ 3,4,5,6 },
#endif
};
#endif

//--------------------------------------------------------------
/**
 * @brief	���^������z�u
 *
 * @param	wk			SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_MetamonPokeSet( SCRATCH_WORK* wk )
{
	int i;
	u8 no;

	//������ɕK�v�ȑ�����|�P�����̐���z�u
	for( i=0; i < 2 ;i++ ){

		while( 1 ){
			//0-8���擾
			no = ( gf_rand() % SCRATCH_D_POKE_MAX );

			//�󂢂Ă�J�[�h�̏ꏊ�Ƀ��^����������
			if( wk->card_poke[no] == DUMMY_CARD_POKE ){
				wk->card_poke[no] = ANM_POKE_META;
				break;
			}
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�͂���|�P������z�u
 *
 * @param	wk			SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_OthersPokeSet( SCRATCH_WORK* wk )
{
	int i,j,set_pos,loop_count,no;
	u8 set_poke;

	set_poke	= ( gf_rand() % 4 );	//�Z�b�g����|�P�����i���o�[(0-3�����߂�B���^�����ȊO)
	loop_count	= 0;

	for( i=0; i < SCRATCH_D_POKE_MAX; i++ ){

		while( 1 ){
			no = ( gf_rand() % SCRATCH_D_POKE_MAX );

			//�󂢂Ă�ꏊ��������
			if( wk->card_poke[no] == DUMMY_CARD_POKE ){
				loop_count = 0;
				wk->card_poke[no] = set_poke;

				//3,2,2�C���z�u����
				if( (i == 2) || (i == 4) || (i == 6) ){
					set_poke++;										//���̃|�P�����i���o�[�ɂ���
					if( set_poke == (SCRATCH_CARD_POKE_MAX-1) ){	//���^����������
						set_poke = 0;
					}
				}
				break;
			}

			/////////////////////////////////////////////////////////////////////////////////////
			//�����_�������܂������Ȃ�������
			loop_count++;
			if( loop_count >= 30 ){
				loop_count = 0;

				//�擪����󂢂Ă��鏊�ɔz�u
				for( set_pos=0; set_pos < SCRATCH_D_POKE_MAX; set_pos++ ){

					//�󂢂Ă�ꏊ���T�[�`
					if( wk->card_poke[set_pos] == DUMMY_CARD_POKE ){
						wk->card_poke[set_pos] = set_poke;

						//3,2,2�C���z�u����
						if( (i == 2) || (i == 4) || (i == 6) ){
							set_poke++;								//���̃|�P�����i���o�[�ɂ���
							if( set_poke == (SCRATCH_CARD_POKE_MAX-1) ){	//���^����������
								set_poke = 0;
							}
						}
						break;		//for����]]]]]]]]]]]]]]]]]]]]
					}
				}
				break;				//while����]]]]]]]]]]]]]]]]]]
			}
		}
	}

	OS_Printf( "�z�u�����|�P�����i���o�[\n" );
	for( i=0; i < SCRATCH_D_POKE_MAX; i++ ){
		OS_Printf( "wk->card_poke[%d] = %d\n", i, wk->card_poke[i] );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�|�P������z�u(�S��ރZ�b�g)
 *
 * @param	wk			SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_PokeSet( SCRATCH_WORK* wk )
{
	int i,j,set_pos,loop_count,no;
	u8 set_poke,set_poke_num;

	set_poke	= (SCRATCH_CARD_POKE_MAX-1);	//�Z�b�g����|�P�����i���o�[(���^��������X�^�[�g)
	set_poke_num= 0;							//�Z�b�g�����|�P�����̐�
	loop_count	= 0;

	for( i=0; i < SCRATCH_D_POKE_MAX; i++ ){

		while( 1 ){
			no = ( gf_rand() % SCRATCH_D_POKE_MAX );

			//�󂢂Ă�ꏊ��������
			if( wk->card_poke[no] == DUMMY_CARD_POKE ){
				loop_count = 0;

				wk->card_poke[no] = set_poke;
				set_poke_num++;

				//�����肪�������ɂȂ�O�ɃZ�b�g����|�P������ύX���遚
				if( set_poke_num == (SCRATCH_CARD_ATARI_NUM-1) ){
					set_poke_num = 0;
					set_poke++;										//���̃|�P�����i���o�[�ɂ���
					if( set_poke == SCRATCH_CARD_POKE_MAX ){
						set_poke = 0;
					}
				}
				break;
			}

			/////////////////////////////////////////////////////////////////////////////////////
			//�����_�������܂������Ȃ�������
			loop_count++;
			if( loop_count >= 30 ){
				loop_count = 0;

				//�擪����󂢂Ă��鏊�ɔz�u
				for( set_pos=0; set_pos < SCRATCH_D_POKE_MAX; set_pos++ ){

					//�󂢂Ă�ꏊ���T�[�`
					if( wk->card_poke[set_pos] == DUMMY_CARD_POKE ){

						wk->card_poke[set_pos] = set_poke;
						set_poke_num++;

						//�����肪�������ɂȂ�O�ɃZ�b�g����|�P������ύX����
						if( set_poke_num == (SCRATCH_CARD_ATARI_NUM-1) ){
							set_poke_num = 0;
							set_poke++;								//���̃|�P�����i���o�[�ɂ���
							if( set_poke == SCRATCH_CARD_POKE_MAX ){
								set_poke = 0;
							}
						}
						break;		//for����]]]]]]]]]]]]]]]]]]]]
					}
				}
				break;				//while����]]]]]]]]]]]]]]]]]]
			}
		}
	}

	OS_Printf( "�z�u�����|�P�����i���o�[\n" );
	for( i=0; i < SCRATCH_D_POKE_MAX; i++ ){
		OS_Printf( "wk->card_poke[%d] = %d\n", i, wk->card_poke[i] );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�z�u�|�P����������
 *
 * @param	wk			SCRATCH_WORK�^�̃|�C���^
 * @param	mode		�C�[�W�E�n�[�h�ǂ��炩
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_CardPokeSet( SCRATCH_WORK* wk, u8 mode )
{
#if 1
	//���^�����|�P�������Z�b�g
	Scratch_MetamonPokeSet( wk );

	//���̑��|�P�������Z�b�g
	Scratch_OthersPokeSet( wk );
#else
	//�|�P�������Z�b�g
	Scratch_PokeSet( wk );
#endif
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�h�̒��̃|�P�����i���o�[���N���A
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_CardPokeClear( SCRATCH_WORK* wk )
{
	int i;

	for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
		wk->card_poke[i] = DUMMY_CARD_POKE;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	����������
 *
 * @param	wk			SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_YakuSet( SCRATCH_WORK* wk, u8 mode )
{
	int i,j;
	u16 item_pos[SCRATCH_ATARI_MAX];
	u16 kin_pos;

	//�N���A
	for( i=0; i < SCRATCH_ATARI_MAX ;i++ ){
		item_pos[i] = 0xff;
	}

	//�u����̂��܁v�̔z�u�ꏊ
	kin_pos = ( gf_rand() % SCRATCH_ATARI_MAX );

	for( i=0; i < SCRATCH_ATARI_MAX ;i++ ){

		if( i == kin_pos ){
			wk->atari_item[i] = ITEM_KINNOTAMA;			//1�͕K���z�u����
		}else{

			while( 1 ){
				item_pos[i] = ( gf_rand() % SCRATCH_EASY_ITEM );

				//�����A�C�e����z�u���Ȃ��悤�ɂ���
				for( j=0; j < i ;j++ ){

					//���łɓ����A�C�e������������
					if( item_pos[j] == item_pos[i] ){
						break;
					}
				}

				//�����A�C�e�����Ȃ�������
				if( j == i ){
					wk->atari_item[i] = scratch_easy_item[ item_pos[i] ];
					break;		//while����
				}
			}
		}
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�h�𒆐S�Ɉړ�
 *
 * @param	none
 *
 * @return	"FALSE = �p�����ATRUE = �ڕW���B"
 */
//--------------------------------------------------------------
//static const int card_x_spd_tbl[SCRATCH_D_CARD_MAX] = { 4, 1, -1, -4 };
static const int card_x_spd_tbl[SCRATCH_D_CARD_MAX] = { 8, 2, -2, -8 };
//static const int card_y_spd_tbl[SCRATCH_D_CARD_MAX] = { -2, -2, -2, -2 };
static const int card_y_spd_tbl[SCRATCH_D_CARD_MAX] = { -4, -4, -4, -4 };

#define CARD_MOVE_NUM	(8)	//���̉񐔂Œ��S�ɂ����Ă���
static const s8 card_move_x_tbl[SCRATCH_D_CARD_MAX][CARD_MOVE_NUM] = {
	{ 9,	9,	9,	8,	7,	6,	5,	4, },	//

	//{ 4,	4,	3,	3,	2,	2,	1,	1, },	//
	{ 2,	2,	2,	1,	1,	1,	1,	1, },	//

	//{ -4,	-4,	-3, -3, -2, -2,	-1,	-1, },	//3�ڐ��ʂɏo��
	{ -8,	-8,-8,-7, -7, -6, -5, -4, },

	//{ -9,	-9,	-9,	-8,	-7,	-6,	-5,	-4, },	//
	{ -18,	-16,-15,-14,-13,-12,-10,-8, },	//
};
static const s8 card_move_y_tbl[CARD_MOVE_NUM] = { -4,	-4,	-4,	-4,	-4,	-4,	-4,	-4, };	//38

static BOOL Scratch_CardCenterMove( SCRATCH_WORK* wk, u8 pos )
{
	int now_x,now_y,set_x,set_y;

	//�ڕW�񐔈ړ�����
	if( wk->move_num >= CARD_MOVE_NUM ){
		return TRUE;
	}

	//���݂̈ʒu���擾
	ScratchObj_GetObjPos( wk->p_d_card[pos], &now_x, &now_y );
	OS_Printf( "now_x = %d\n", now_x );
	OS_Printf( "now_y = %d\n", now_y );

	set_x = ( now_x + card_move_x_tbl[pos][wk->move_num] );
	set_y = ( now_y + card_move_y_tbl[wk->move_num] );
	wk->move_num++;

	OS_Printf( "set_x = %d\n", set_x );
	OS_Printf( "set_y = %d\n", set_y );
	ScratchObj_SetObjPos( wk->p_d_card[pos], set_x, set_y );

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�p���b�g�؂�ւ�
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
static void ScrPalChg( SCRATCH_WORK* wk, u32 frm, u8 pltt_no, u8 sx, u8 sy )
{
	OS_Printf( "�؂�ւ���p���b�g�i���o�[ = %d\n", pltt_no );
	GF_BGL_ScrPalChange( wk->bgl, frm, 0, 0, sx, sy, pltt_no );
	GF_BGL_LoadScreenV_Req( wk->bgl, frm );							//�X�N���[���f�[�^�]��
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�┓�ɐG�ꂽ�����擾
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"��"
 */
//--------------------------------------------------------------
static u8 Scratch_BlockFlagOnNumGet( SCRATCH_WORK* wk )
{
	u8 i,count;

	count = 0;

	for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
		if( wk->block_flag[i] == 1 ){
			count++;
		}
	}

	return count;
}

//--------------------------------------------------------------
/**
 * @brief	�┓�ɐG�ꂽ�����N���A
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	void
 */
//--------------------------------------------------------------
static void Scratch_BlockFlagClear( SCRATCH_WORK* wk )
{
	u8 i;

	for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
		wk->block_flag[i] = 0;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�G�ꂽ�ʒu�J�E���g���N���A
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	void
 */
//--------------------------------------------------------------
static void Scratch_BlockCountClear( SCRATCH_WORK* wk )
{
	u8 i;

	for( i=0; i < SCRATCH_D_POKE_MAX ;i++ ){
		wk->block_count[i] = 0;
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�T���v�����O�����擾���Ċi�[
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	void
 */
//--------------------------------------------------------------
static void Scratch_HoseiTouch2( SCRATCH_WORK* wk, int index )
{
	int i,j,x,y;

	for( i=0; i < wk->tpData.Size ;i++ ){
		x = wk->tpData.TPDataTbl[i].x;
		y = wk->tpData.TPDataTbl[i].y;

		OS_Printf( "tpData.TPDataTbl[%d].x = %d\n", i, x );
		OS_Printf( "tpData.TPDataTbl[%d].y = %d\n", i, y );

		//block_hit_tbl�̒����Q�Əo���Ȃ��̂ŁA�����f�[�^�e�[�u����p��
		if( (block_hit_tbl2[index].left <= x) && (x <= block_hit_tbl2[index].right) &&
			(block_hit_tbl2[index].top <= y) && (y <= block_hit_tbl2[index].bottom) ){
			Scratch_Touch_2( wk, x, y );
		}
	}

	return;
}

//�G�ꂽ�t���O�𗧂Ă�
static void Scratch_TouchFlagSet( SCRATCH_WORK* wk )
{
	int i;
	u8 x,y;

	for( i=0; i < wk->tpData.Size ;i++ ){

		x = wk->tpData.TPDataTbl[i].x;
		y = wk->tpData.TPDataTbl[i].y;

		//wk->touch_flag[ (x / 8) + ((y / 8)*32) ] |= (1 << (x % 8));
		
#if 0
		if( (x < SCRATCH_TOUCH_SIZE_X) && (y < SCRATCH_TOUCH_SIZE_Y) ){
			wk->touch_flag[ x + (y*SCRATCH_TOUCH_SIZE_X) ] = 1;
		}
#else
		Scratch_TouchFlagSetSub( wk, x, y );
#endif
	}

	return;
}

//�G�ꂽ���W����4dot�̕�����������
static void Scratch_TouchFlagSetSub( SCRATCH_WORK* wk, int tp_x, int tp_y )
{
	int i,j;

	for( j=-(DOT_WRITE_WIDTH); j < DOT_WRITE_WIDTH ;j++ ){			//Y
		for( i=-(DOT_WRITE_WIDTH); i < DOT_WRITE_WIDTH ;i++ ){		//X
			if( ((tp_x + i ) > 0) && ((tp_x + i) < SCRATCH_TOUCH_SIZE_X) &&
				((tp_y + j) > 0) && ((tp_y + j) < SCRATCH_TOUCH_SIZE_Y) ){
				//OS_Printf( "tp_x + i = %d\n", tp_x + i );
				//OS_Printf( "tp_y + j = %d\n", tp_y + j );
				wk->touch_flag[ (tp_x+i) + ((tp_y+j)*SCRATCH_TOUCH_SIZE_X) ] = 1;
			}
		}
	}
	return;
}

//�G�ꂽ�t���O�`�F�b�N
static BOOL Scratch_TouchFlagCheck( SCRATCH_WORK* wk, u8 index )
{
	int i,j,start_x,start_y,count;

	count	= 0;
	start_x = block_check_pos[index].x;
	start_y = block_check_pos[index].y;

	//�u���b�N���̃t���O���`�F�b�N
	for( i=start_y; i <  (start_y+BLOCK_SIZE_Y) ;i++ ){
		for( j=start_x; j <  (start_x+BLOCK_SIZE_X) ;j++ ){

			if( wk->touch_flag[i*SCRATCH_TOUCH_SIZE_X+j] == 1 ){
				count++;
			}
		}
	}

	OS_Printf( "count = %d\n", count );
	if( count >= BLOCK_COUNT ){

		//����̂�SE��炷
		if( wk->block_count[index] == 0 ){
			Snd_SePlay( SE_D_KEZURI_OK );
			wk->kezuri_card[wk->kezuri_card_num] = index;	//�ǂ̃J�[�h���ۑ�
			wk->kezuri_card_num++;

			//���[�`�ɂ���
			//if( wk->card_poke[index] == wk->atari_poke ){
			//	wk->waku_flag = 1;
			//}
		}

		wk->block_count[index] = 1;
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	�E�F�C�g�v�Z���ăp���b�g�i���o�[�A���\�[�X�ύX
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	void
 */
//--------------------------------------------------------------
static void Scratch_PlttNoResChg( SCRATCH_WORK* wk )
{
	int i;

	wk->pltt_wait++;

	if( wk->pltt_wait >= WAKU_PAL_WAIT ){

		wk->pltt_wait = 0;

		wk->pltt_res_no++;
		if( wk->pltt_res_no >= WAKU_PAL_NUM ){
			wk->pltt_res_no = 0;
		}

		if( wk->kezuri_card_num == 2 ){

			//�P���ځA�Q����
			for( i=0; i < 2; i++ ){
				ScratchObj_PaletteNoChg( wk->p_d_waku[i], 3 );
				ScratchObj_Vanish( wk->p_d_waku[i], SCRATCH_VANISH_OFF );	//�\��
				ScratchObj_SetObjPos( wk->p_d_waku[i], 
						(d_poke_pos[ wk->kezuri_card[i] ].x + SCRATCH_GIN_WAKU_OFFSET_X), 
						d_poke_pos[ wk->kezuri_card[i] ].y + SCRATCH_GIN_WAKU_OFFSET_Y );
			}
		}

		//�p���b�g3�Ԏg�p�ύX
		//�p���b�g���\�[�X��؂�ւ���
		ScratchClact_ButtonResourcePalChg( &wk->scratch_clact, wk->pltt_res_no );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����肪����������擾
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ������AFALSE = �͂���"
 */
//--------------------------------------------------------------
static BOOL Scratch_AtariCheck( SCRATCH_WORK* wk )
{
	int i,atari_flag;

	atari_flag = 0;

	//2���͍��Ȃ��ƁA���[�`�ɂ͂Ȃ�Ȃ�
	if( wk->kezuri_card_num <= 1 ){
		return FALSE;
	}

	if( wk->card_poke[ wk->kezuri_card[0] ] == wk->card_poke[ wk->kezuri_card[1] ] ){
		wk->waku_flag = 1;
	}else if( wk->card_poke[ wk->kezuri_card[0] ] == ANM_POKE_META ){	//���^�����͕K�����[�`
		wk->waku_flag = 1;
	}else if( wk->card_poke[ wk->kezuri_card[1] ] == ANM_POKE_META ){	//���^�����͕K�����[�`
		wk->waku_flag = 1;
	}
			
	//3���͍��Ȃ��ƁA������ɂ͂Ȃ�Ȃ�
	if( wk->kezuri_card_num <= 2 ){
		return FALSE;
	}

	//�P�A�Q�A�R����
	if( (wk->card_poke[ wk->kezuri_card[0] ] == wk->card_poke[ wk->kezuri_card[1] ]) &&
		(wk->card_poke[ wk->kezuri_card[0] ] == wk->card_poke[ wk->kezuri_card[2] ]) ){
		atari_flag = 1;
	}

	//�P�A�Q����(���^����)
	if( (wk->card_poke[ wk->kezuri_card[0] ] == ANM_POKE_META) &&
		(wk->card_poke[ wk->kezuri_card[1] ] == ANM_POKE_META) ){
		atari_flag = 1;
	}

	//�P�A�R����(���^����)
	if( (wk->card_poke[ wk->kezuri_card[0] ] == ANM_POKE_META) &&
		(wk->card_poke[ wk->kezuri_card[2] ] == ANM_POKE_META) ){
		atari_flag = 1;
	}

	//�Q�A�R����(���^����)
	if( (wk->card_poke[ wk->kezuri_card[1] ] == ANM_POKE_META) &&
		(wk->card_poke[ wk->kezuri_card[2] ] == ANM_POKE_META) ){
		atari_flag = 1;
	}

	//�P(���^����)�A�Q�A�R����
	if( (wk->card_poke[ wk->kezuri_card[0] ] == ANM_POKE_META) &&
		(wk->card_poke[ wk->kezuri_card[1] ] == wk->card_poke[ wk->kezuri_card[2] ]) ){
		atari_flag = 1;
	}

	//�P�A�Q(���^����)�A�R����
	if( (wk->card_poke[ wk->kezuri_card[1] ] == ANM_POKE_META) &&
		(wk->card_poke[ wk->kezuri_card[0] ] == wk->card_poke[ wk->kezuri_card[2] ]) ){
		atari_flag = 1;
	}

	//�P�A�Q�A�R(���^����)����
	if( (wk->card_poke[ wk->kezuri_card[2] ] == ANM_POKE_META) &&
		(wk->card_poke[ wk->kezuri_card[0] ] == wk->card_poke[ wk->kezuri_card[1] ]) ){
		atari_flag = 1;
	}

	if( atari_flag == 1 ){
		//������|�P�������Z�b�g����
		for( i=0; i < SCRATCH_CARD_KEZURI_NUM; i++ ){
			if( wk->card_poke[ wk->kezuri_card[i] ] != ANM_POKE_META ){
				wk->atari_poke = wk->card_poke[ wk->kezuri_card[i] ];
				break;
			}
		}
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief	������̃��^�����̃��U�C�N�G�t�F�N�g�F������
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 *
 * @return	"TRUE = ���^��������AFALSE = ���^�������Ȃ�"
 */
//--------------------------------------------------------------
static BOOL Scratch_MosaicEffInit( SCRATCH_WORK* wk )
{
	int i;
	BOOL flag;

	flag = FALSE;

	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){

		//���^�����������烂�U�C�N�I��
		if( wk->card_poke[ wk->kezuri_card[i] ] == ANM_POKE_META ){
			ScratchObj_MosaicSet( wk->p_d_atari[i], TRUE );
			flag = TRUE;
		}
	}

	wk->mosaic_size = 0;					//���U�C�N�T�C�Y(0-15)

	//���U�C�N�̕���ݒ�
	G2_SetOBJMosaicSize( wk->mosaic_size, wk->mosaic_size );	//��0-15(0�͒ʏ�\��)

	return flag;
}

//--------------------------------------------------------------
/**
 * @brief	������̃��^�����̃��U�C�N�G�t�F�N�g�F���C��
 *
 * @param	wk		SCRATCH_WORK�^�̃|�C���^
 * @param	flag	0=mosaic++�A1=mosaic--
 *
 * @return	"TRUE = �p�����AFALSE = �I��"
 */
//--------------------------------------------------------------
static BOOL Scratch_MosaicEffMain( SCRATCH_WORK* wk, u8 flag )
{
	int i;

	//���^���������邩
	for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
		if( wk->card_poke[ wk->kezuri_card[i] ] == ANM_POKE_META ){
			break;
		}
	}

	//���^�������Ȃ��̂ŏI��
	if( i == SCRATCH_CARD_ATARI_NUM ){
		return FALSE;
	}

	if( flag == 0 ){

		//����̂ݖ炷
		if( wk->mosaic_size == 0 ){
			Snd_SePlay( SE_D_METAMON_CHG );
		}

		if( wk->mosaic_size < SCRATCH_MOSAIC_MAX ){
			wk->mosaic_size++;					//���U�C�N�T�C�Y(0-15)
		}else{
			//���^�����𓖂���|�P�����ɕύX
			for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
				if( wk->card_poke[ wk->kezuri_card[i] ] == ANM_POKE_META ){
					ScratchObj_AnmChg(	wk->p_d_atari[i], wk->atari_poke );
				}
			}
			return FALSE;
		}
	}else{
		if( wk->mosaic_size > 0 ){
			wk->mosaic_size--;					//���U�C�N�T�C�Y(0-15)
		}else{
			//���U�C�N���I�t
			for( i=0; i < SCRATCH_CARD_ATARI_NUM ;i++ ){
				ScratchObj_MosaicSet( wk->p_d_atari[i], FALSE );
			}
			return FALSE;
		}
	}

	//���U�C�N�̕���ݒ�
	G2_SetOBJMosaicSize( wk->mosaic_size, wk->mosaic_size );	//��0-15(0�͒ʏ�\��)

	return TRUE;
}


//==============================================================================================
//
//	�T�u�V�[�P���X
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	��߂�ւ������߂̏���
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SeqSubYameruCard( SCRATCH_WORK* wk )
{
	wk->card_num++;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	���̃J�[�h�ւ������߂̏���
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SeqSubNextCard( SCRATCH_WORK* wk )
{
	wk->card_num++;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�┓OK
 *
 * @param	
 *
 * @return	none
 */
//--------------------------------------------------------------
static void Scratch_SeqSubGinOk( SCRATCH_WORK* wk )
{
	ScratchObj_Vanish( wk->p_d_win, SCRATCH_VANISH_ON );				//�I�t
	GF_BGL_BmpWinDataFill( &wk->bmpwin[BMPWIN_START], FBMP_COL_NULL );
	GF_BGL_BmpWinOnVReq( &wk->bmpwin[BMPWIN_START] );
	return;
}


//==============================================================================================
//
//	�q�b�g�`�F�b�N(�^�b�`�p�l��)
//
//==============================================================================================
#if 0
//--------------------------------------------------------------
/**
 * @brief	�q�b�g�`�F�b�N
 *
 * @param	none
 *
 * @return	"TRUE=�G��Ă���AFALSE=�G��Ă��Ȃ�"
 */
//--------------------------------------------------------------
static BOOL Scratch_TPSystemHitCheck( ANIMAL_OBJ* animal_obj )
{
	int i;
	RECT_HIT_TBL hit_tbl[2];
	const VecFx32* mat_p;
	VecFx32 mat;

	for( i=0; i < 2 ;i++ ){
		hit_tbl[i].rect.top		= TP_HIT_END;			//������
		hit_tbl[i].rect.bottom	= 0;
		hit_tbl[i].rect.left	= 0;
		hit_tbl[i].rect.right	= 0;
	}

	mat_p = AnimalObj_GetPos( animal_obj );				//�����̈ʒu�擾
	mat = *mat_p;

#if SUB_DISP_CHG										//1=�T�u��ʂɕύX
	//mat.y -= SUB_SURFACE_Y;
	mat.y -= SCRATCH_SUB_ACTOR_DISTANCE;
#endif

	//�����́u32x32�v
	hit_tbl[0].rect.top		= mat.y / FX32_ONE - 16;
	hit_tbl[0].rect.bottom	= mat.y / FX32_ONE + 16;
	hit_tbl[0].rect.left	= mat.x / FX32_ONE - 16;
	hit_tbl[0].rect.right	= mat.x / FX32_ONE + 16;

	if( GF_TP_SingleHitCont((const RECT_HIT_TBL*)hit_tbl) == TRUE ){
		Snd_SePlay( SE_D_BUTTON_TOUCH );
		//OS_Printf( "�e�[�u����HIT���܂���\n" );
		return TRUE;
	}

	return FALSE;
}
#endif


//==============================================================================================
//
//	�L�����N�^�[�f�[�^���R�s�[���āA�L�����N�^�f�[�^������������
//
//==============================================================================================

//------------------------------------------------------------------
/**
 * �L�����N�^�[�f�[�^���o�b�t�@�ɃR�s�[
 * "�I�����ɊJ�����K�v"
 *
 * @param   wk		SCRATCH_WORK�^�̃|�C���^
 *
 * �L��������Ȃ��悤�ɉ�ʃT�C�Y���p�ӂ��Ă����A
 * ��������̊֐��Ńo�b�t�@�ɃR�s�[���Ă����āA
 * ��ʂ��^�b�`������A�o�b�t�@�̂��̏ꏊ��0�N���A����(�r�b�g�P��)
 */
//------------------------------------------------------------------
static void Scratch_Touch_0( SCRATCH_WORK* wk )
{
	//�L�����N�^�擾
	wk->p_char = ArcUtil_CharDataGet(	ARC_SCRATCH, NARC_scratch_l_bg01gin_NCGR, 
	//wk->p_char = ArcUtil_CharDataGet(	ARC_SCRATCH, NARC_scratch_l_bg00_NCGR, 
										0, &wk->p_chardata, HEAPID_SCRATCH );

	//�L�����N�^�]��
	GF_BGL_LoadCharacter(	wk->bgl, SCRATCH_FRAME_D_TOUCH,
							wk->p_chardata->pRawData,
							wk->p_chardata->szByte,
							0 );

	GF_BGL_LoadScreenReq( wk->bgl, SCRATCH_FRAME_D_TOUCH );
	return;
}

//------------------------------------------------------------------
/**
 * �o�b�t�@�𐶐�	"�I�����ɊJ�����K�v"
 *
 * @param   wk		SCRATCH_WORK�^�̃|�C���^
 */
//------------------------------------------------------------------
static void Scratch_Touch_1( SCRATCH_WORK* wk )
{
	wk->buf = sys_AllocMemory( HEAPID_SCRATCH, wk->p_chardata->szByte );
	if( wk->buf == NULL ){
		OS_Printf( "�������m�ێ��s�I\n" );
		GF_ASSERT( wk->buf != NULL );
	}
	return;
}

//------------------------------------------------------------------
/**
 * �L�����N�^�[�f�[�^���R�s�[
 *
 * @param   wk		SCRATCH_WORK�^�̃|�C���^
 */
//------------------------------------------------------------------
static void Scratch_Touch_1_5( SCRATCH_WORK* wk )
{
	//OS_Printf( "�L�����N�^�f�[�^�T�C�Y = %d\n", wk->p_chardata->szByte );
	memcpy( wk->buf, wk->p_chardata->pRawData, wk->p_chardata->szByte );
	return;
}

//------------------------------------------------------------------
/**
 * �^�b�`�p�l���ɐG��Ă�����W���擾���ăL�����N�^�[�f�[�^������������
 *
 * @param   wk		SCRATCH_WORK�^�̃|�C���^
 */
//------------------------------------------------------------------
static void Scratch_Touch_2( SCRATCH_WORK* wk, int tp_x, int tp_y )
{
	int i,j;

	//����Ő��m���m���߂�
	//OS_Printf( "tp_x = %d\n", tp_x );
	//OS_Printf( "tp_y = %d\n", tp_y );
	//Scratch_Touch_3( wk, tp_x, tp_y );

	for( j=-(DOT_WRITE_WIDTH); j < DOT_WRITE_WIDTH ;j++ ){			//Y
		for( i=-(DOT_WRITE_WIDTH); i < DOT_WRITE_WIDTH ;i++ ){		//X
			if( ((tp_x + i ) > 0) && ((tp_x + i) < 256) &&
				((tp_y + j) > 0) && ((tp_y + j) < 192) ){
				//OS_Printf( "tp_x + i = %d\n", tp_x + i );
				//OS_Printf( "tp_y + j = %d\n", tp_y + j );
				Scratch_Touch_3( wk, (tp_x+i), (tp_y+j) );
			}
		}
	}

	GF_BGL_LoadCharacter( wk->bgl, SCRATCH_FRAME_D_TOUCH, wk->buf, wk->p_chardata->szByte, 0 );
	GF_BGL_LoadScreenReq( wk->bgl, SCRATCH_FRAME_D_TOUCH );
	return;
}

//------------------------------------------------------------------
/**
 * �n���ꂽ�ʒu�̃L�����N�^�[�f�[�^��0�N���A
 *
 * @param   wk		SCRATCH_WORK�^�̃|�C���^
 * @param   tp_x	�h�b�g�P�ʂ�X
 * @param   tp_y	�h�b�g�P�ʂ�Y
 */
//------------------------------------------------------------------
static void Scratch_Touch_3( SCRATCH_WORK* wk, u32 tp_x, u32 tp_y )
{
	u8 set_bit;
	u32 pos_x,pos_y,offset_x,offset_y,decide_pos;

	set_bit = 0xff;
		
	if( (tp_x % 2) == 0 ){					//�V�t�g����l(�O��4bit���A�㔼4bit��)
		set_bit ^= 0x0f;					//2�i(00001111)
	}else{
		set_bit ^= 0xf0;					//2�i(11110000)
	}
	//OS_Printf( "set_bit = %d\n", set_bit );

	//�L�����P�ʂ��Ƃ̏ꏊ���擾
	pos_x = ( tp_x / 8 * ONE_CHAR );		//�h�b�g�ʒu���L�����ʒu�ɂ��āA1�L����
	pos_y = ( tp_y / 8 * ONE_CHAR * 32 );	//�h�b�g�ʒu���L�����ʒu�ɂ��āA1�L����x����
	//OS_Printf( "pos_x = %d\n", pos_x );
	//OS_Printf( "pos_y = %d\n", pos_y );

	offset_x = ( tp_x % 8 / 2 );			//��0-3�̂ǂꂩ
	//OS_Printf( "offset_x = %d\n", offset_x );
	
	if( tp_y < 8 ){
		offset_y = ( tp_y * 4 );			//�c0-7�̂ǂꂩ(4=���̗v�f��)
	}else{
		offset_y = ( tp_y % 8 );
		offset_y = ( offset_y * 4 );		//�c0-7�̂ǂꂩ(4=���̗v�f��)
	}
	//OS_Printf( "offset_y = %d\n", offset_y );

	decide_pos = ( pos_y + pos_x + offset_x + offset_y );
	//OS_Printf( "�Z�b�g���Ă���v�f���Fdecide_pos = %d\n", decide_pos );

	//OS_Printf( "�O wk->buf = %d\n", wk->buf[decide_pos] );
	wk->buf[ decide_pos ] &= set_bit;
	//OS_Printf( "�� wk->buf = %d\n\n******************\n", wk->buf[decide_pos] );
	return;
}


//==============================================================================================
//
//	���ォ��E��16�L������(COL_NUM��)�p���b�g�̃J���[�Ɠ���8x8�̃L�����ɏ�������
//
//==============================================================================================
#if 0
#define COL_NUM	(2)

void CgxDataSetup( GF_BGL_INI* bgl )
{
	int i;
	u32 fillDat,offset;
	u8* buf = sys_AllocMemory( HEAPID_SCRATCH, (0x20 * COL_NUM) );

	offset = 0;

	if( buf ){

		for( i=0; i < COL_NUM; i++ ){
			fillDat = (i<<28) | (i<<24) | (i<<20) | (i<<16) | (i<<12) | (i<<8) | (i<<4) | i;
			MI_CpuFillFast( buf+i*0x20, fillDat, 0x20 );
		}

		GF_BGL_LoadCharacter( bgl, SCRATCH_FRAME_D_TOUCH, buf, (0x20 * COL_NUM), offset );
		sys_FreeMemoryEz( buf );
	}

	return;
}
#endif


