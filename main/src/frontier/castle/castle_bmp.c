//==============================================================================================
/**
 * @file	castle_bmp.c
 * @brief	�u�o�g���L���b�X���vBMP
 * @author	Satoshi Nohara
 * @date	07.07.05
 */
//==============================================================================================
#include "common.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "savedata/config.h"

#include "castle_sys.h"
#include "castle_bmp.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//���j���[�E�B���h�E�L����
#define BC_MENU_CGX_NUM		(1024-MENU_WIN_CGX_SIZ)

//��b�E�B���h�E�L����
#define	BC_TALKWIN_CGX_SIZE	(TALK_WIN_CGX_SIZ)
#define	BC_TALKWIN_CGX_NUM	(BC_MENU_CGX_NUM - BC_TALKWIN_CGX_SIZE)


//==============================================================================================
//
//	�莝���|�P������ʃf�[�^
//
//==============================================================================================
//�u�g���[�i�[��1,2�v
#define MINE_WIN_TR1_PX			(2)
#define MINE_WIN_TR1_PY			(1)
#define MINE_WIN_TR1_SX			(29)
#define MINE_WIN_TR1_SY			(2)
#define MINE_WIN_TR1_CGX		(1)

//�u���ǂ�v
#define MINE_WIN_MODORU_PX		(26)
#define MINE_WIN_MODORU_PY		(19)
#define MINE_WIN_MODORU_SX		(4)
#define MINE_WIN_MODORU_SY		(3)
#define MINE_WIN_MODORU_CGX		(MINE_WIN_TR1_CGX + MINE_WIN_TR1_SX * MINE_WIN_TR1_SY)

//�uLV99�v
#define MINE_WIN_LV_PX			(0)
#define MINE_WIN_LV_PY			(4)
#define MINE_WIN_LV_SX			(32)
#define MINE_WIN_LV_SY			(2)
#define MINE_WIN_LV_CGX			(MINE_WIN_MODORU_CGX + MINE_WIN_MODORU_SX * MINE_WIN_MODORU_SY)

//�uHP�v
#define MINE_WIN_HP_PX			(0)
#define MINE_WIN_HP_PY			(9)
#define MINE_WIN_HP_SX			(32)
#define MINE_WIN_HP_SY			(2)
#define MINE_WIN_HP_CGX			(MINE_WIN_LV_CGX + MINE_WIN_LV_SX * MINE_WIN_LV_SY)

//�uLV99�v(�t���[����ς��Ă���)
#define MINE_WIN_ITEM_LV_PX		(1)
#define MINE_WIN_ITEM_LV_PY		(7)
#define MINE_WIN_ITEM_LV_SX		(10)
#define MINE_WIN_ITEM_LV_SY		(2)
#define MINE_WIN_ITEM_LV_CGX	(MINE_WIN_CP_CGX + MINE_WIN_CP_SX * MINE_WIN_CP_SY)

//�uHP�v(�t���[����ς��Ă���)
#define MINE_WIN_ITEM_HP_PX		(1)
#define MINE_WIN_ITEM_HP_PY		(13)
#define MINE_WIN_ITEM_HP_SX		(11)
#define MINE_WIN_ITEM_HP_SY		(2)
#define MINE_WIN_ITEM_HP_CGX	(MINE_WIN_ITEM_LV_CGX + MINE_WIN_ITEM_LV_SX * MINE_WIN_ITEM_LV_SY)

/////////////////////////////////////
//�u�悳�E�킴�v(���t���[����ς��Ă���)
#define MINE_WIN_STATUS_PX		(5)
#define MINE_WIN_STATUS_PY		(10)
#define MINE_WIN_STATUS_SX		(24)
#define MINE_WIN_STATUS_SY		(14)
#define MINE_WIN_STATUS_CGX		(1)			//����

//�u�A�C�e�����X�g�v(���t���[����ς��Ă���)
#define MINE_WIN_LIST_PX		(12)
#define MINE_WIN_LIST_PY		(2)
#define MINE_WIN_LIST_SX		(19)
//#define MINE_WIN_LIST_SY		(14)
#define MINE_WIN_LIST_SY		(12)
#define MINE_WIN_LIST_CGX		(1)			//����

//�u��b�v(�t���[����ς��Ă���)
#define MINE_WIN_TALK_PX		(2)
#define MINE_WIN_TALK_PY		(19)
#define MINE_WIN_TALK_SX		(27)
#define MINE_WIN_TALK_SY		(4)
#define MINE_WIN_TALK_CGX		(MINE_WIN_STATUS_CGX + MINE_WIN_STATUS_SX * MINE_WIN_STATUS_SY)

//�u��b(���j���[�E�B���h�E)�v(�t���[����ς��Ă���)(���u��b�v���Ɏ��܂�悤�ɂ���)
#define MINE_WIN_TALKMENU_PX	(2)
#define MINE_WIN_TALKMENU_PY	(19)
#define MINE_WIN_TALKMENU_SX	(20)
#define MINE_WIN_TALKMENU_SY	(4)
#define MINE_WIN_TALKMENU_CGX	(MINE_WIN_TALK_CGX + MINE_WIN_TALK_SX *MINE_WIN_TALK_SY)

//�u��b(���j���[�E�B���h�E2)�v(�t���[����ς��Ă���)(���u��b�v���Ɏ��܂�悤�ɂ���)
#define MINE_WIN_TALKMENU2_PX	(2)
#define MINE_WIN_TALKMENU2_PY	(19)
#define MINE_WIN_TALKMENU2_SX	(17)
#define MINE_WIN_TALKMENU2_SY	(4)
#define MINE_WIN_TALKMENU2_CGX	(MINE_WIN_TALKMENU_CGX + MINE_WIN_TALKMENU_SX *MINE_WIN_TALKMENU_SY)

//�u��{���X�g1�v(�t���[����ς��Ă���)(���u���X�g�v���Ɏ��܂�悤�ɂ���v)
//�����ӂ��A�����^���A�悳�A�킴�A�Ƃ���
#define MINE_WIN_BASIC_LIST_PX	(23)
#define MINE_WIN_BASIC_LIST_PY	(13)
#define MINE_WIN_BASIC_LIST_SX	(8)
#define MINE_WIN_BASIC_LIST_SY	(10)
#define MINE_WIN_BASIC_LIST_CGX	(MINE_WIN_TALKMENU2_CGX + MINE_WIN_TALKMENU2_SX * MINE_WIN_TALKMENU2_SY)

//�u�񕜃��X�g�v(���t���[����ς��Ă���)
//�g�o�����ӂ��A�o�o�����ӂ��A���ׂĂ����ӂ��A�����N�A�b�v�A��߂�
#define MINE_WIN_KAIHUKU_LIST_PX	(20)
#define MINE_WIN_KAIHUKU_LIST_PY	(7)
#define MINE_WIN_KAIHUKU_LIST_SX	(11)
#define MINE_WIN_KAIHUKU_LIST_SY	(10)
#define MINE_WIN_KAIHUKU_LIST_CGX	(MINE_WIN_BASIC_LIST_CGX + MINE_WIN_BASIC_LIST_SX * MINE_WIN_BASIC_LIST_SY)

//�u�����^�����X�g�v(�t���[����ς��Ă���)(���u���X�g�v���Ɏ��܂�悤�ɂ���v)
//���̂݁A�ǂ����A�����N�A�b�v�A��߂�
#define MINE_WIN_RENTAL_LIST_PX		(22)
#define MINE_WIN_RENTAL_LIST_PY		(9)
#define MINE_WIN_RENTAL_LIST_SX		(9)
#define MINE_WIN_RENTAL_LIST_SY		(8)
#define MINE_WIN_RENTAL_LIST_CGX	(MINE_WIN_BASIC_LIST_CGX + MINE_WIN_BASIC_LIST_SX * MINE_WIN_BASIC_LIST_SY)

//�u������v(�t���[����ς��Ă���)
#define MINE_WIN_ITEMINFO_PX	(7)
#define MINE_WIN_ITEMINFO_PY	(17)
#define MINE_WIN_ITEMINFO_SX	(23)
#define MINE_WIN_ITEMINFO_SY	(6)
#define MINE_WIN_ITEMINFO_CGX	(MINE_WIN_TALK_CGX + MINE_WIN_TALK_SX *MINE_WIN_TALK_SY)

//�u�͂��E�������v(�t���[����ς��Ă���)
#define MINE_WIN_YESNO_PX		(24)
#define MINE_WIN_YESNO_PY		(13)
#define MINE_WIN_YESNO_SX		(7)
#define MINE_WIN_YESNO_SY		(4)
#define MINE_WIN_YESNO_CGX		(MINE_WIN_ITEMINFO_CGX + MINE_WIN_ITEMINFO_SX *MINE_WIN_ITEMINFO_SY)

//�u���x���{�T�E���x���[�T�E��߂�v(�t���[����ς��Ă���)
#define MINE_WIN_SEL_PX			(24)
#define MINE_WIN_SEL_PY			(11)
#define MINE_WIN_SEL_SX			(7)
#define MINE_WIN_SEL_SY			(6)
#define MINE_WIN_SEL_CGX		(MINE_WIN_YESNO_CGX + MINE_WIN_YESNO_SX *MINE_WIN_YESNO_SY)

//�u���O�{�b�o�E�B���h�E�v(�t���[����ς��Ă���)
#define MINE_WIN_CP_PX			(1)
#define MINE_WIN_CP_PY			(1)
#define MINE_WIN_CP_SX			(8)//(10)
#define MINE_WIN_CP_SY			(4)
#define MINE_WIN_CP_CGX			(MINE_WIN_SEL_CGX + MINE_WIN_SEL_SX *MINE_WIN_SEL_SY)

static const BMPWIN_DAT CastleMineBmpData[] =
{
	{	// 0:�u�g���[�i�[��1�A2�v
		BC_FRAME_WIN, MINE_WIN_TR1_PX, MINE_WIN_TR1_PY,
		MINE_WIN_TR1_SX, MINE_WIN_TR1_SY, BC_FONT_PAL, MINE_WIN_TR1_CGX
	},	
	{	// 1:�u���ǂ�v
		BC_FRAME_WIN, MINE_WIN_MODORU_PX, MINE_WIN_MODORU_PY,
		MINE_WIN_MODORU_SX, MINE_WIN_MODORU_SY, BC_FONT_PAL, MINE_WIN_MODORU_CGX
	},	
	{	// 3:�uLV99�v
		BC_FRAME_WIN, MINE_WIN_LV_PX, MINE_WIN_LV_PY,
		MINE_WIN_LV_SX, MINE_WIN_LV_SY, BC_FONT_PAL, MINE_WIN_LV_CGX
	},	
	{	// 4:�u999/999�v
		BC_FRAME_WIN, MINE_WIN_HP_PX, MINE_WIN_HP_PY,
		MINE_WIN_HP_SX, MINE_WIN_HP_SY, BC_FONT_PAL, MINE_WIN_HP_CGX
	},	
	//////////////////////
	{	// 5:�u�悳�E�킴�v
		BC_FRAME_TYPE, MINE_WIN_STATUS_PX, MINE_WIN_STATUS_PY,			//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_STATUS_SX, MINE_WIN_STATUS_SY, BC_FONT_PAL, MINE_WIN_STATUS_CGX
	},	
	{	// 6:�u���X�g�v
		BC_FRAME_TYPE, MINE_WIN_LIST_PX, MINE_WIN_LIST_PY,				//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_LIST_SX, MINE_WIN_LIST_SY, BC_FONT_PAL, MINE_WIN_LIST_CGX
	},
	{	// 10:�u��b�v
		BC_FRAME_TYPE, MINE_WIN_TALK_PX, MINE_WIN_TALK_PY,				//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_TALK_SX, MINE_WIN_TALK_SY, BC_MSGFONT_PAL, MINE_WIN_TALK_CGX
	},
	{	// 11:�u��b(���j���[�E�B���h�E)�v
		BC_FRAME_TYPE, MINE_WIN_TALKMENU_PX, MINE_WIN_TALKMENU_PY,		//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_TALKMENU_SX, MINE_WIN_TALKMENU_SY, BC_MSGFONT_PAL, MINE_WIN_TALKMENU_CGX
	},
	{	// 12:�u��b(���j���[�E�B���h�E2)�v
		BC_FRAME_TYPE, MINE_WIN_TALKMENU2_PX, MINE_WIN_TALKMENU2_PY,	//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_TALKMENU2_SX, MINE_WIN_TALKMENU2_SY, BC_MSGFONT_PAL, MINE_WIN_TALKMENU2_CGX
	},
	{	// 9:�u��{���X�g1�v
		BC_FRAME_TYPE, MINE_WIN_BASIC_LIST_PX, MINE_WIN_BASIC_LIST_PY,//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_BASIC_LIST_SX, MINE_WIN_BASIC_LIST_SY, BC_FONT_PAL, MINE_WIN_BASIC_LIST_CGX
	},
	{	// 7:�u�񕜃��X�g�v
		BC_FRAME_TYPE, MINE_WIN_KAIHUKU_LIST_PX, MINE_WIN_KAIHUKU_LIST_PY,//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_KAIHUKU_LIST_SX, MINE_WIN_KAIHUKU_LIST_SY, BC_FONT_PAL, MINE_WIN_KAIHUKU_LIST_CGX
	},
	{	// 8:�u�����^�����X�g�v
		BC_FRAME_TYPE, MINE_WIN_RENTAL_LIST_PX, MINE_WIN_RENTAL_LIST_PY,//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_RENTAL_LIST_SX, MINE_WIN_RENTAL_LIST_SY, BC_FONT_PAL, MINE_WIN_RENTAL_LIST_CGX
	},
	{	// 13:�u������v
		BC_FRAME_TYPE, MINE_WIN_ITEMINFO_PX, MINE_WIN_ITEMINFO_PY,		//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_ITEMINFO_SX, MINE_WIN_ITEMINFO_SY, BC_MSGFONT_PAL, MINE_WIN_ITEMINFO_CGX
	},
	{	// 14:�u�͂��E�������v
		BC_FRAME_TYPE, MINE_WIN_YESNO_PX, MINE_WIN_YESNO_PY,			//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_YESNO_SX, MINE_WIN_YESNO_SY, BC_FONT_PAL, MINE_WIN_YESNO_CGX
	},
	{	// 15:�u���x���{�T�E���x���[�T�E��߂�v
		BC_FRAME_TYPE, MINE_WIN_SEL_PX, MINE_WIN_SEL_PY,				//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_SEL_SX, MINE_WIN_SEL_SY, BC_FONT_PAL, MINE_WIN_SEL_CGX
	},
	{	// 16:�u���O�{CP�E�B���h�E�v
		BC_FRAME_TYPE, MINE_WIN_CP_PX, MINE_WIN_CP_PY,					//�t���[�����Ⴄ�̂Œ��ӁI
		MINE_WIN_CP_SX, MINE_WIN_CP_SY, BC_FONT_PAL, MINE_WIN_CP_CGX
	},
	{	// 17:�uLV99�v
		BC_FRAME_TYPE, MINE_WIN_ITEM_LV_PX, MINE_WIN_ITEM_LV_PY,
		MINE_WIN_ITEM_LV_SX, MINE_WIN_ITEM_LV_SY, BC_FONT_PAL, MINE_WIN_ITEM_LV_CGX
	},	
	{	// 18:�u999/999�v
		BC_FRAME_TYPE, MINE_WIN_ITEM_HP_PX, MINE_WIN_ITEM_HP_PY,
		MINE_WIN_ITEM_HP_SX, MINE_WIN_ITEM_HP_SY, BC_FONT_PAL, MINE_WIN_ITEM_HP_CGX
	},	

};


//==============================================================================================
//
//	�G�g���[�i�[��ʃf�[�^
//
//==============================================================================================
//�u�g���[�i�[��1,2�v
#define ENEMY_WIN_TR1_PX		(2)
#define ENEMY_WIN_TR1_PY		(1)
#define ENEMY_WIN_TR1_SX		(29)
#define ENEMY_WIN_TR1_SY		(2)
#define ENEMY_WIN_TR1_CGX		(1)

//�u���ǂ�v
#define ENEMY_WIN_MODORU_PX		(26)
#define ENEMY_WIN_MODORU_PY		(19)
#define ENEMY_WIN_MODORU_SX		(4)
#define ENEMY_WIN_MODORU_SY		(3)
#define ENEMY_WIN_MODORU_CGX	(ENEMY_WIN_TR1_CGX + ENEMY_WIN_TR1_SX * ENEMY_WIN_TR1_SY)

//�uLV99�v
#define ENEMY_WIN_LV_PX			(0)
#define ENEMY_WIN_LV_PY			(4)
#define ENEMY_WIN_LV_SX			(32)
#define ENEMY_WIN_LV_SY			(2)
#define ENEMY_WIN_LV_CGX		(ENEMY_WIN_MODORU_CGX + ENEMY_WIN_MODORU_SX * ENEMY_WIN_MODORU_SY)

//�uHP�v
#define ENEMY_WIN_HP_PX			(0)
#define ENEMY_WIN_HP_PY			(9)
#define ENEMY_WIN_HP_SX			(32)
#define ENEMY_WIN_HP_SY			(2)
#define ENEMY_WIN_HP_CGX		(ENEMY_WIN_LV_CGX + ENEMY_WIN_LV_SX * ENEMY_WIN_LV_SY)

/////////////////////////////////////////////////////////////////////////////////////////////
//�u�悳�E�킴�v(�t���[����ς��Ă���)
#define ENEMY_WIN_STATUS_PX		(4)
#define ENEMY_WIN_STATUS_PY		(10)
#define ENEMY_WIN_STATUS_SX		(26)
#define ENEMY_WIN_STATUS_SY		(14)
#define ENEMY_WIN_STATUS_CGX	(1)

//�u��{���X�g1�v(�t���[����ς��Ă���)(���u�悳�E�킴�v���Ɏ��܂�悤�ɂ���v)
//����ׂ�A���x���A���傤�ق��A�Ƃ���
#define ENEMY_WIN_BASIC_LIST_PX		(23)
#define ENEMY_WIN_BASIC_LIST_PY		(15)
#define ENEMY_WIN_BASIC_LIST_SX		(8)
#define ENEMY_WIN_BASIC_LIST_SY		(8)
#define ENEMY_WIN_BASIC_LIST_CGX	(1)		//����

//�u��{���X�g2�v(�t���[����ς��Ă���)(���u�悳�E�킴�v���Ɏ��܂�悤�ɂ���)
//�悳�A�킴�A�����N�A�b�v�A��߂�
#define ENEMY_WIN_BASIC2_LIST_PX	(22)
#define ENEMY_WIN_BASIC2_LIST_PY	(9)
#define ENEMY_WIN_BASIC2_LIST_SX	(9)
#define ENEMY_WIN_BASIC2_LIST_SY	(8)
#define ENEMY_WIN_BASIC2_LIST_CGX	(ENEMY_WIN_STATUS_CGX+ENEMY_WIN_STATUS_SX*ENEMY_WIN_STATUS_SY)

//�u��b�v(�t���[����ς��Ă���)
#define ENEMY_WIN_TALK_PX		(2)
#define ENEMY_WIN_TALK_PY		(19)
#define ENEMY_WIN_TALK_SX		(27)
#define ENEMY_WIN_TALK_SY		(4)
#define ENEMY_WIN_TALK_CGX	(ENEMY_WIN_BASIC2_LIST_CGX + ENEMY_WIN_BASIC2_LIST_SX * ENEMY_WIN_BASIC2_LIST_SY)

//�u��b(���j���[�E�B���h�E)�v(�t���[����ς��Ă���)(���u��b�v���Ɏ��܂�悤�ɂ���)
#define ENEMY_WIN_TALKMENU_PX	(2)
#define ENEMY_WIN_TALKMENU_PY	(19)
#define ENEMY_WIN_TALKMENU_SX	(20)
#define ENEMY_WIN_TALKMENU_SY	(4)
#define ENEMY_WIN_TALKMENU_CGX	(ENEMY_WIN_TALK_CGX + ENEMY_WIN_TALK_SX *ENEMY_WIN_TALK_SY)

//�u��b(���j���[�E�B���h�E2)�v(�t���[����ς��Ă���)(���u��b�v���Ɏ��܂�悤�ɂ���)
#define ENEMY_WIN_TALKMENU2_PX	(2)
#define ENEMY_WIN_TALKMENU2_PY	(19)
#define ENEMY_WIN_TALKMENU2_SX	(17)
#define ENEMY_WIN_TALKMENU2_SY	(4)
#define ENEMY_WIN_TALKMENU2_CGX	(ENEMY_WIN_TALKMENU_CGX+ENEMY_WIN_TALKMENU_SX*ENEMY_WIN_TALKMENU_SY)

//�u�͂��E�������v(�t���[����ς��Ă���)
#define ENEMY_WIN_YESNO_PX		(24)
#define ENEMY_WIN_YESNO_PY		(13)
#define ENEMY_WIN_YESNO_SX		(7)
#define ENEMY_WIN_YESNO_SY		(4)
#define ENEMY_WIN_YESNO_CGX	(ENEMY_WIN_TALKMENU2_CGX+ENEMY_WIN_TALKMENU2_SX*ENEMY_WIN_TALKMENU2_SY)

//�u���x���{�T�E���x���[�T�E��߂�v(�t���[����ς��Ă���)
#define ENEMY_WIN_SEL_PX		(24)
#define ENEMY_WIN_SEL_PY		(11)
#define ENEMY_WIN_SEL_SX		(7)
#define ENEMY_WIN_SEL_SY		(6)
#define ENEMY_WIN_SEL_CGX		(ENEMY_WIN_YESNO_CGX + ENEMY_WIN_YESNO_SX * ENEMY_WIN_YESNO_SY)

static const BMPWIN_DAT CastleEnemyBmpData[] =
{
	{	// 0:�u�g���[�i�[��1�A2�v
		BC_FRAME_WIN, ENEMY_WIN_TR1_PX, ENEMY_WIN_TR1_PY,
		ENEMY_WIN_TR1_SX, ENEMY_WIN_TR1_SY, BC_FONT_PAL, ENEMY_WIN_TR1_CGX
	},	
	{	// 1:�u���ǂ�v
		BC_FRAME_WIN, ENEMY_WIN_MODORU_PX, ENEMY_WIN_MODORU_PY,
		ENEMY_WIN_MODORU_SX, ENEMY_WIN_MODORU_SY, BC_FONT_PAL, ENEMY_WIN_MODORU_CGX
	},	
	{	// 2:�uLV99�v
		BC_FRAME_WIN, ENEMY_WIN_LV_PX, ENEMY_WIN_LV_PY,
		ENEMY_WIN_LV_SX, ENEMY_WIN_LV_SY, BC_FONT_PAL, ENEMY_WIN_LV_CGX
	},	
	{	// 3:�u999/999�v
		BC_FRAME_WIN, ENEMY_WIN_HP_PX, ENEMY_WIN_HP_PY,
		ENEMY_WIN_HP_SX, ENEMY_WIN_HP_SY, BC_FONT_PAL, ENEMY_WIN_HP_CGX
	},	
	{	// 4:�u�悳�E�킴�v
		BC_FRAME_TYPE, ENEMY_WIN_STATUS_PX, ENEMY_WIN_STATUS_PY,		//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_STATUS_SX, ENEMY_WIN_STATUS_SY, BC_FONT_PAL, ENEMY_WIN_STATUS_CGX
	},	
	{	// 5:�u��{���X�g1�v
		BC_FRAME_TYPE, ENEMY_WIN_BASIC_LIST_PX, ENEMY_WIN_BASIC_LIST_PY,//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_BASIC_LIST_SX, ENEMY_WIN_BASIC_LIST_SY, BC_FONT_PAL, ENEMY_WIN_BASIC_LIST_CGX
	},
	{	// 6:�u��{���X�g2�v
		BC_FRAME_TYPE, ENEMY_WIN_BASIC2_LIST_PX, ENEMY_WIN_BASIC2_LIST_PY,//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_BASIC2_LIST_SX, ENEMY_WIN_BASIC2_LIST_SY, BC_FONT_PAL, ENEMY_WIN_BASIC2_LIST_CGX
	},
	{	// 7:�u��b�v
		BC_FRAME_TYPE, ENEMY_WIN_TALK_PX, ENEMY_WIN_TALK_PY,			//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_TALK_SX, ENEMY_WIN_TALK_SY, BC_MSGFONT_PAL, ENEMY_WIN_TALK_CGX
	},
	{	// 8:�u��b(���j���[�E�B���h�E)�v
		BC_FRAME_TYPE, ENEMY_WIN_TALKMENU_PX, ENEMY_WIN_TALKMENU_PY,	//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_TALKMENU_SX, ENEMY_WIN_TALKMENU_SY, BC_MSGFONT_PAL, ENEMY_WIN_TALKMENU_CGX
	},
	{	// 9:�u��b(���j���[�E�B���h�E2)�v
		BC_FRAME_TYPE, ENEMY_WIN_TALKMENU2_PX, ENEMY_WIN_TALKMENU2_PY,	//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_TALKMENU2_SX, ENEMY_WIN_TALKMENU2_SY, BC_MSGFONT_PAL, ENEMY_WIN_TALKMENU2_CGX
	},
	{	// 10:�u�͂��E�������v
		BC_FRAME_TYPE, ENEMY_WIN_YESNO_PX, ENEMY_WIN_YESNO_PY,			//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_YESNO_SX, ENEMY_WIN_YESNO_SY, BC_FONT_PAL, ENEMY_WIN_YESNO_CGX
	},
	{	// 11:�u���x���{�T�E���x���[�T�E��߂�v
		BC_FRAME_TYPE, ENEMY_WIN_SEL_PX, ENEMY_WIN_SEL_PY,				//�t���[�����Ⴄ�̂Œ��ӁI
		ENEMY_WIN_SEL_SX, ENEMY_WIN_SEL_SY, BC_FONT_PAL, ENEMY_WIN_SEL_CGX
	},
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
void CastleAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, u8 bmp_no );
void CastleExitBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );
void CastleOffBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );
void CastleWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );
void CastleTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype );

//�͂��A�������E�B���h�E��`
#define YESNO_PX		(23)			//�J�n�w
#define YESNO_PY		(19)			//�J�n�x
#define YESNO_SX		(8)				//���w
#define YESNO_SY		(4)				//���x

//��b�E�B���h�E��`
#define TALK_PX			(2)				//�J�n�w
#define TALK_PY			(19)			//�J�n�x
#define TALK_SX			(17)			//���w
#define TALK_SY			(4)				//���x


typedef struct{
	const BMPWIN_DAT* dat;				//BMP�f�[�^
	u32	max;							//�o�^�ő吔
}CASTLE_BMP;

//
static const CASTLE_BMP castle_bmp[CASTLE_BMP_MAX] = {
	{ CastleMineBmpData,	CASTLE_MINE_BMPWIN_MAX	},
	{ CastleEnemyBmpData,	CASTLE_ENEMY_BMPWIN_MAX	},
};


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�ǉ�
 *
 * @param	ini		BGL�f�[�^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, u8 bmp_no )
{
	u8 i;
	const BMPWIN_DAT* dat = castle_bmp[bmp_no].dat;
	u32 max = castle_bmp[bmp_no].max;

	//�r�b�g�}�b�v�ǉ�
	for( i=0; i < max; i++ ){
		GF_BGL_BmpWinAddEx( bgl, &win[i], &dat[i] );
		GF_BGL_BmpWinDataFill( &win[i], FBMP_COL_NULL );		//�h��Ԃ�
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�j��	
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleExitBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no )
{
	u16	i;
	u32 max = castle_bmp[bmp_no].max;

	for( i=0; i < max; i++ ){
		GF_BGL_BmpWinDel( &win[i] );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�I�t
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleOffBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no )
{
	u16	i;
	u32 max = castle_bmp[bmp_no].max;

	for( i=0; i < max; i++ ){
		GF_BGL_BmpWinOff( &win[i] );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	���j���[�E�B���h�E�\��
 *
 * @param	ini		BGL�f�[�^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win )
{
	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
	//MenuWinGraphicSet( bgl, BC_FRAME_WIN, BC_MENU_CGX_NUM, BC_MENU_PAL, 0, HEAPID_CASTLE );
	MenuWinGraphicSet(	bgl, GF_BGL_BmpWinGet_Frame(win), 
						BC_MENU_CGX_NUM, BC_MENU_PAL, 0, HEAPID_CASTLE );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite( win, WINDOW_TRANS_OFF, BC_MENU_CGX_NUM, BC_MENU_PAL );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	��b�E�B���h�E�\��
 *
 * @param	win		BMP�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void CastleTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype )
{
	TalkWinGraphicSet( win->ini, GF_BGL_BmpWinGet_Frame(win), BC_TALKWIN_CGX_NUM,
						BC_TALKWIN_PAL, wintype, HEAPID_CASTLE );

	GF_BGL_BmpWinDataFill( win, 15 );
	BmpTalkWinWrite( win, WINDOW_TRANS_OFF, BC_TALKWIN_CGX_NUM, BC_TALKWIN_PAL );
	GF_BGL_BmpWinOnVReq( win );
	return;
}


