//==============================================================================================
/**
 * @file	roulette_sys.h
 * @brief	�u�o�g�����[���b�g�v�V�X�e���w�b�_�[
 * @author	Satoshi Nohara
 * @date	07.09.06
 */
//==============================================================================================
#ifndef _ROULETTE_SYS_H_
#define _ROULETTE_SYS_H_

#include "common.h"
#include "system/procsys.h"


//==============================================================================================
//
//	�^�N�g���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _ROULETTE_WORK ROULETTE_WORK;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�p���b�g��`
#define BR_TALKWIN_PAL			(10)				//��b�E�B���h�E�g�p�p���b�g�i���o�[
#define BR_MENU_PAL				(11)				//���j���[�E�B���h�E�g�p�p���b�g�i���o�[
#define BR_MSGFONT_PAL			(12)				//MSG�t�H���g�g�p�p���b�g�i���o�[
#define BR_FONT_PAL				(13)				//SYSTEM�t�H���g�g�p�p���b�g�i���o�[

#define ROULETTE_VANISH_ON		(0)					//��\��
#define ROULETTE_VANISH_OFF		(1)					//�\��

//�p�l���̐�
#define ROULETTE_PANEL_H_MAX	(4)
#define ROULETTE_PANEL_V_MAX	(4)
#define ROULETTE_PANEL_MAX		(ROULETTE_PANEL_H_MAX * ROULETTE_PANEL_V_MAX)

//���\�[�X�}�l�[�W���[�I�u�W�F�N�g�i���o�[
enum{
	ROULETTE_RES_OBJ_BUTTON = 0,					//�{�^��
	ROULETTE_RES_OBJ_CSR,							//�J�[�\��
	ROULETTE_RES_OBJ_ITEMKEEP,						//�A�C�e���������Ă���A�C�R��
	ROULETTE_RES_OBJ_ICON1,							//�A�C�R��1
	ROULETTE_RES_OBJ_ICON2,							//�A�C�R��2
	ROULETTE_RES_OBJ_ICON3,							//�A�C�R��3
	ROULETTE_RES_OBJ_ICON4,							//�A�C�R��4
	ROULETTE_RES_OBJ_ICON5,							//�A�C�R��5
	ROULETTE_RES_OBJ_ICON6,							//�A�C�R��6
	ROULETTE_RES_OBJ_ICON7,							//�A�C�R��7
	ROULETTE_RES_OBJ_ICON8,							//�A�C�R��8
	ROULETTE_RES_OBJ_MAX,							//�ő吔(���\�[�X�t�@�C����1�A���Ƃ̓A�C�R��)
};

//�Ǘ�CHAR_ID
enum{
	ROULETTE_ID_CHAR_BUTTON = 0,					//�{�^��
	ROULETTE_ID_CHAR_CSR,							//�J�[�\��
	ROULETTE_ID_CHAR_ITEMKEEP,						//�A�C�e���������Ă���A�C�R��
	ROULETTE_ID_CHAR_ICON1,							//�A�C�R��1
	ROULETTE_ID_CHAR_ICON2,							//�A�C�R��2
	ROULETTE_ID_CHAR_ICON3,							//�A�C�R��3
	ROULETTE_ID_CHAR_ICON4,							//�A�C�R��4
	ROULETTE_ID_CHAR_ICON5,							//�A�C�R��5
	ROULETTE_ID_CHAR_ICON6,							//�A�C�R��6
	ROULETTE_ID_CHAR_ICON7,							//�A�C�R��7
	ROULETTE_ID_CHAR_ICON8,							//�A�C�R��8
	ROULETTE_ID_CHAR_MAX,							//
};

//�Ǘ�PLTT_ID
enum{
	ROULETTE_ID_PLTT_BUTTON = 0,					//�{�^��
	ROULETTE_ID_PLTT_CSR,							//�J�[�\��
	ROULETTE_ID_PLTT_ITEMKEEP,						//�A�C�e���������Ă���A�C�R��
	ROULETTE_ID_PLTT_ICON,							//�A�C�R��
	ROULETTE_ID_PLTT_MAX,							//
};

//�Ǘ�CELL_ID
enum{
	ROULETTE_ID_CELL_BUTTON = 0,					//�{�^��
	ROULETTE_ID_CELL_CSR,							//�J�[�\��
	ROULETTE_ID_CELL_ITEMKEEP,						//�A�C�e���������Ă���A�C�R��
	ROULETTE_ID_CELL_ICON,							//�A�C�R��
	ROULETTE_ID_CELL_MAX,							//
};

//�Ǘ�CELLANM_ID
enum{
	ROULETTE_ID_CELLANM_BUTTON = 0,					//�{�^��
	ROULETTE_ID_CELLANM_CSR,						//�J�[�\��
	ROULETTE_ID_CELLANM_ITEMKEEP,					//�A�C�e���������Ă���A�C�R��
	ROULETTE_ID_CELLANM_ICON,						//�A�C�R��
	ROULETTE_ID_CELLANM_MAX,						//
};

//�Ǘ�ID
enum{
	ROULETTE_ID_OBJ_BUTTON = 0,						//�{�^��
	ROULETTE_ID_OBJ_CSR,							//�J�[�\��
	ROULETTE_ID_OBJ_ITEMKEEP,						//�A�C�e���������Ă���A�C�R��
	//ROULETTE_ID_OBJ_ITEMKEEP1,					//�A�C�e���������Ă���A�C�R��1
	//ROULETTE_ID_OBJ_ITEMKEEP2,					//�A�C�e���������Ă���A�C�R��2
	//ROULETTE_ID_OBJ_ITEMKEEP3,					//�A�C�e���������Ă���A�C�R��3
	//ROULETTE_ID_OBJ_ITEMKEEP4,					//�A�C�e���������Ă���A�C�R��4
	ROULETTE_ID_OBJ_ICON1,							//�A�C�R��1
	ROULETTE_ID_OBJ_ICON2,							//�A�C�R��2
	ROULETTE_ID_OBJ_ICON3,							//�A�C�R��3
	ROULETTE_ID_OBJ_ICON4,							//�A�C�R��4
	ROULETTE_ID_OBJ_ICON5,							//�A�C�R��5
	ROULETTE_ID_OBJ_ICON6,							//�A�C�R��6
	ROULETTE_ID_OBJ_ICON7,							//�A�C�R��7
	ROULETTE_ID_OBJ_ICON8,							//�A�C�R��8
	ROULETTE_ID_OBJ_MAX,
};

#define ROULETTE_POKE_ICON_MAX		(8)				//�|�P�����A�C�R���ő吔
#define ROULETTE_ITEM_KEEP_MAX		(8)				//�A�C�e�������Ă���A�C�R���ő吔

//clact�ɐݒ肷��ő�A�N�^�[��
//�p�l��16�A�J�[�\��1�A�|�P�����A�C�R��8�A�A�C�e�������Ă���A�C�R��8
//�p�l��16�A�J�[�\��1�A�|�P�����A�C�R��8�A�A�C�e�������Ă���A�C�R��8�A�{�^��1
//#define ROULETTE_CLACT_OBJ_MAX	(ROULETTE_PANEL_MAX + 1 + ROULETTE_POKE_ICON_MAX + ROULETTE_ITEM_KEEP_MAX)
#define ROULETTE_CLACT_OBJ_MAX	(ROULETTE_PANEL_MAX + 1 + ROULETTE_POKE_ICON_MAX + ROULETTE_ITEM_KEEP_MAX + 1)

//�A�j���i���o�[(�J�[�\���A�p�l�����\�[�X)
enum{
	ROULETTE_ANM_EV_MINE_HP_DOWN = 0,
	ROULETTE_ANM_EV_MINE_DOKU,
	ROULETTE_ANM_EV_MINE_MAHI,
	ROULETTE_ANM_EV_MINE_YAKEDO,
	ROULETTE_ANM_EV_MINE_NEMURI,
	ROULETTE_ANM_EV_MINE_KOORI,
	ROULETTE_ANM_EV_MINE_SEED_GET,
	ROULETTE_ANM_EV_MINE_ITEM_GET,
	ROULETTE_ANM_EV_MINE_LV_UP,
	////////////////////////////////////////////////
	ROULETTE_ANM_EV_ENEMY_HP_DOWN,
	ROULETTE_ANM_EV_ENEMY_DOKU,					//10
	ROULETTE_ANM_EV_ENEMY_MAHI,
	ROULETTE_ANM_EV_ENEMY_YAKEDO,
	ROULETTE_ANM_EV_ENEMY_NEMURI,
	ROULETTE_ANM_EV_ENEMY_KOORI,
	ROULETTE_ANM_EV_ENEMY_SEED_GET,
	ROULETTE_ANM_EV_ENEMY_ITEM_GET,
	ROULETTE_ANM_EV_ENEMY_LV_UP,
	////////////////////////////////////////////////
	ROULETTE_ANM_EV_PLACE_HARE,
	ROULETTE_ANM_EV_PLACE_AME,
	ROULETTE_ANM_EV_PLACE_SUNAARASI,			//20
	ROULETTE_ANM_EV_PLACE_ARARE,
	ROULETTE_ANM_EV_PLACE_KIRI,
	ROULETTE_ANM_EV_PLACE_TRICK,
	ROULETTE_ANM_EV_PLACE_SPEED_UP,
	ROULETTE_ANM_EV_PLACE_SPEED_DOWN,			//25
	ROULETTE_ANM_EV_PLACE_RANDOM,
	////////////////////////////////////////////////
	ROULETTE_ANM_EV_EX_POKE_CHANGE,				//27
	ROULETTE_ANM_EV_EX_BP_GET,					//28
	ROULETTE_ANM_EV_EX_BTL_WIN,					//29
	ROULETTE_ANM_EV_EX_SUKA,					//30
	ROULETTE_ANM_EV_EX_BP_GET_BIG,				//31
	ROULETTE_ANM_COUNT_3,
	ROULETTE_ANM_COUNT_2,
	ROULETTE_ANM_COUNT_1,
	ROULETTE_ANM_CSR,
};

//�A�j���i���o�[(�{�^�����\�[�X)
enum{
	ROULETTE_ANM_BUTTON_STOP = 0,
	ROULETTE_ANM_BUTTON_PUSH,
	ROULETTE_ANM_BUTTON_START,
};

#if 0
//�Z���i���o�[
enum{
	ROULETTE_CELL_CURSOR_L = 0,
	ROULETTE_CELL_CURSOR_R, 
	ROULETTE_CELL_CURSOR_ITEM, 
	ROULETTE_CELL_SCR_CURSOR_U, 
	ROULETTE_CELL_SCR_CURSOR_D, 
	ROULETTE_CELL_RANK_CURSOR_L,
	ROULETTE_CELL_RANK_CURSOR_R, 
};
#endif

#define BR_FRAME_WIN	(GF_BGL_FRAME1_M)
#define BR_FRAME_TYPE	(GF_BGL_FRAME0_M)
#define BR_FRAME_EFF	(GF_BGL_FRAME2_M)			//�p�l���G�t�F�N�g
#define BR_FRAME_BG		(GF_BGL_FRAME3_M)			//SINGLE,DOUBLE
#define BR_FRAME_SUB	(GF_BGL_FRAME0_S)			//�{�[��

typedef struct{
	s16  x;
	s16  y;
}ROULETTE_POS;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern const PROC_DATA RouletteProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT RouletteProc_Init( PROC * proc, int * seq );
extern PROC_RESULT RouletteProc_Main( PROC * proc, int * seq );
extern PROC_RESULT RouletteProc_End( PROC * proc, int * seq );

//�ʐM
extern void Roulette_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
extern void Roulette_CommRecvBufRankUpType(int id_no,int size,void *pData,void *work);
extern void Roulette_CommRecvBufCsrPos(int id_no,int size,void *pData,void *work);
extern void Roulette_CommRecvBufModoru(int id_no,int size,void *pData,void *work);


#endif //_ROULETTE_SYS_H_


