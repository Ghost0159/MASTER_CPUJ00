//==============================================================================================
/**
 * @file	scratch_sys.h
 * @brief	�u�X�N���b�`�v�V�X�e���w�b�_�[
 * @author	Satoshi Nohara
 * @date	07.12.11
 */
//==============================================================================================
#ifndef _SCRATCH_SYS_H_
#define _SCRATCH_SYS_H_

#include "common.h"
#include "system/procsys.h"


//==============================================================================================
//
//	�X�N���b�`���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _SCRATCH_WORK SCRATCH_WORK;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�p���b�g��`
#define BS_TALKWIN_PAL			(10)				//��b�E�B���h�E�g�p�p���b�g�i���o�[
#define BS_MENU_PAL				(11)				//���j���[�E�B���h�E�g�p�p���b�g�i���o�[
#define BS_MSGFONT_PAL			(12)				//MSG�t�H���g�g�p�p���b�g�i���o�[
#define BS_FONT_PAL				(13)				//SYSTEM�t�H���g�g�p�p���b�g�i���o�[
#define SCRATCH_TOUCH_SUB_PAL	(10)				//����ʁu�͂��A�������v�g�p�p���b�g�i���o�[

#define SCRATCH_VANISH_ON		(0)					//��\��
#define SCRATCH_VANISH_OFF		(1)					//�\��

//���\�[�X�}�l�[�W���[�I�u�W�F�N�g�i���o�[
enum{
	SCRATCH_RES_OBJ_U_POKE = 0,						//�|�P����
	SCRATCH_RES_OBJ_D_POKE,							//�|�P����
#if 1
	SCRATCH_RES_OBJ_D_CARD,							//�J�[�h
	SCRATCH_RES_OBJ_D_BUTTON,						//�{�^��
	SCRATCH_RES_OBJ_D_ATARI,						//������E�B���h�E
#else
	//�J�[�h�Ɛ؂�ւ�
	SCRATCH_RES_OBJ_D_CARD,							//�J�[�h
	SCRATCH_RES_OBJ_D_BUTTON=SCRATCH_RES_OBJ_D_CARD,//�{�^��(�J�[�h�Ɛ؂�ւ�)
#endif
	SCRATCH_RES_OBJ_MAX,							//�ő吔
};

//�Ǘ�ID
enum{
	SCRATCH_ID_OBJ_U_POKE = 0,						//�|�P����
	SCRATCH_ID_OBJ_D_POKE,							//�|�P����
#if 1
	SCRATCH_ID_OBJ_D_CARD,							//�J�[�h
	SCRATCH_ID_OBJ_D_BUTTON,						//�{�^��
	SCRATCH_ID_OBJ_D_ATARI,							//������E�B���h�E
#else
	//�J�[�h�Ɛ؂�ւ�
	SCRATCH_ID_OBJ_D_CARD,							//�J�[�h
	SCRATCH_ID_OBJ_D_BUTTON=SCRATCH_ID_OBJ_D_CARD,	//�{�^��(�J�[�h�Ɛ؂�ւ�)
#endif
	SCRATCH_ID_OBJ_MAX,
};

#define SCRATCH_U_POKE_MAX		(4)					//��\���|�P������
#define SCRATCH_ATARI_MAX		(4)					//������̐�
#define SCRATCH_D_POKE_MAX		(9)					//���\���|�P������
#define SCRATCH_D_WAKU_MAX		(9)					//���\�����N��
#define SCRATCH_D_CARD_MAX		(4)					//���\���J�[�h��

//���ʁF�|�P����12
//����ʁF�|�P����9+�J�[�h4+�{�^��1+�E�B��+1+�┓���N9+������|�P����4+������E�B��1
#define SCRATCH_CLACT_OBJ_MAX	(40)				//�Z���A�N�^�[�ɐݒ肷��ő�A�N�^�[��

//�A�j���i���o�[
enum{
	ANM_CARD_BLUE = 0,								//��
	ANM_CARD_RED,									//��
	ANM_CARD_YELLOW,								//���F
	ANM_CARD_GREEN,									//��
};

//�A�j���i���o�[
enum{
	ANM_POKE_REE = 0,								//���[�V����
	ANM_POKE_TYE,									//�`�F�����{
	ANM_POKE_TAMA,									//�^�}�U���V
	ANM_POKE_GOKU,									//�S�N����
	ANM_POKE_META,									//���^����
};

//�A�j���i���o�[
enum{
	ANM_BUTTON_YAMERU = 0,							//��߂�{�^��
	ANM_BUTTON_NEXT,								//���̃J�[�h�փ{�^��
	ANM_GIN_WAKU,									//�┓�̃��N
	ANM_CENTER_WIN,									//�^�񒆂ɕ\�����郁�b�Z�[�W�̃E�B���h�E
};

#define SCRATCH_FRAME_U_MSG		(GF_BGL_FRAME0_S)	//���b�Z�[�W
#define SCRATCH_FRAME_TYPE		(GF_BGL_FRAME1_S)	//���g�p
#define SCRATCH_FRAME_U_YAKU	(GF_BGL_FRAME2_S)	//��w�i
#define SCRATCH_FRAME_U_BG		(GF_BGL_FRAME3_S)	//4���̃J�[�h��I�Ԏ��̔w�i
#define SCRATCH_FRAME_D_TOUCH	(GF_BGL_FRAME0_M)	//����
#define SCRATCH_FRAME_D_SUB		(GF_BGL_FRAME1_M)	//�J�[�h�w�i
#define SCRATCH_FRAME_D_MSG		(GF_BGL_FRAME2_M)	//���b�Z�[�W
//#define SCRATCH_FRAME_D_ATARI	(GF_BGL_FRAME3_M)	//������E�B���h�E

#define SCRATCH_U_MASK_MSG		(GX_PLANEMASK_BG0)	//���b�Z�[�W
#define SCRATCH_U_MASK_TYPE		(GX_PLANEMASK_BG1)	//���g�p
#define SCRATCH_U_MASK_YAKU		(GX_PLANEMASK_BG2)	//��w�i
#define SCRATCH_U_MASK_BG		(GX_PLANEMASK_BG3)	//4���̃J�[�h��I�Ԏ��̔w�i
#define SCRATCH_D_MASK_TOUCH	(GX_PLANEMASK_BG0)	//����
#define SCRATCH_D_MASK_SUB		(GX_PLANEMASK_BG1)	//�J�[�h�w�i
#define SCRATCH_D_MASK_MSG		(GX_PLANEMASK_BG2)	//���b�Z�[�W
//#define SCRATCH_D_MASK_ATARI	(GX_PLANEMASK_BG3)	//������E�B���h�E

typedef struct{
	s16  x;
	s16  y;
}SCRATCH_POS;

//CLACT�Œ�`���Ă���Z�����傫�����ăT�u��ʂɉe�����łĂ��܂��̂ŗ����Ă݂�
//oekaki.c,guru2_receipt.c�Q�l
//#define SCRATCH_SUB_ACTOR_DISTANCE 	( 256 * FX32_ONE )
#define SCRATCH_SUB_ACTOR_DISTANCE 	( 512 * FX32_ONE )		//�g�傳����̂ł���ɗ���

#define SCRATCH_CARD_NUM		(3)						//�X�N���b�`�̃J�[�h�̐�

//OBJ�Ƃ̃v���C�I���e�B
enum{
	SCRATCH_OBJ_PRI_H = 0,
	SCRATCH_OBJ_PRI_N = 10,
	SCRATCH_OBJ_PRI_L = 20,
};

//BG�Ƃ̃v���C�I���e�B
enum{
	SCRATCH_OBJ_BG_PRI_H = 0,
	SCRATCH_OBJ_BG_PRI_N = 1,
	SCRATCH_OBJ_BG_PRI_L = 2,
};

//���N�̃p���b�g���\�[�X��
#define WAKU_PAL_NUM			(8)


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern const PROC_DATA ScratchProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT ScratchProc_Init( PROC * proc, int * seq );
extern PROC_RESULT ScratchProc_Main( PROC * proc, int * seq );
extern PROC_RESULT ScratchProc_End( PROC * proc, int * seq );


#endif //_SCRATCH_SYS_H_


