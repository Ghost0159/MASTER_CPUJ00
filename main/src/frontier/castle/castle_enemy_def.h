//==============================================================================================
/**
 * @file	castle_enemy_def.h
 * @brief	�u�o�g���L���b�X���v�G�g���[�i�[��ʂ̒�`
 * @author	Satoshi Nohara
 * @date	07.07.17
 *
 * "castle_enemy.c"�̂�include��
 */
//==============================================================================================
#ifndef _CASTLE_ENEMY_DEF_H_
#define _CASTLE_ENEMY_DEF_H_


//==============================================================================================
//
//	��`
//
//==============================================================================================

//----------------------------------------------------------------------------------------------
//���b�Z�[�W�֘A�̒�`
//----------------------------------------------------------------------------------------------
#define CASTLE_MENU_BUF_MAX		(3)								//���j���[�o�b�t�@�̍ő吔
#define BC_FONT					(FONT_SYSTEM)					//�t�H���g���
#define CASTLE_MSG_BUF_SIZE		(600)//(800)//(1024)			//��b�̃��b�Z�[�Wsize
#define CASTLE_MENU_BUF_SIZE	(32)							//���j���[�̃��b�Z�[�Wsize
#define PLAYER_NAME_BUF_SIZE	(PERSON_NAME_SIZE + EOM_SIZE)	//�v���C���[���̃��b�Z�[�Wsize
#define POKE_NAME_BUF_SIZE		(MONS_NAME_SIZE + EOM_SIZE)		//�|�P�������̃��b�Z�[�Wsize

//�t�H���g�J���[
#define	COL_BLUE	( GF_PRINTCOLOR_MAKE(FBMP_COL_BLUE,FBMP_COL_BLU_SDW,FBMP_COL_NULL) )//��
#define	COL_RED		( GF_PRINTCOLOR_MAKE(FBMP_COL_RED,FBMP_COL_RED_SDW,FBMP_COL_NULL) )	//��

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
#define CSR_POS_NONE				(0xff)						//�J�[�\�������I��ł��Ȃ�
#define CASTLE_ENEMY_DECIDE_NONE	(0xff)						//���薳��

//�g�pCP
#define CP_USE_TEMOTI		(1)			//����ׂ�
#define CP_USE_LEVEL_PLUS	(1)			//���x���{�T
#define CP_USE_LEVEL_MINUS	(15)		//���x���[�T
#define CP_USE_TUYOSA		(2)			//�悳
#define CP_USE_WAZA			(5)		//�킴

//----------------------------------------------------------------------------------------------
//�|�P�������̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	 POKE_NAME_PX			= (10*8),
	 POKE_NAME_PY			= (21*8),
	 POKE_NAME_SX			= (9*8),
	 POKE_NAME_SY			= (2*8),
};

//----------------------------------------------------------------------------------------------
//�A�C�R���̈ʒu
//----------------------------------------------------------------------------------------------
enum{
	ICON_START_X			= (64),
	ICON_MULTI_START_X		= (32),
	ICON_START_Y			= (58),
	ICON_WIDTH_X			= (64),
};

//----------------------------------------------------------------------------------------------
//�����N�A�b�v�G�t�F�N�g�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	RANKUP_EFF_START_X			= (ICON_START_X),
	RANKUP_EFF_MULTI_START_X	= (ICON_MULTI_START_X),		//�ʐM
	RANKUP_EFF_START_Y			= (ICON_START_Y + 4),
	RANKUP_EFF_WIDTH_X			= (ICON_WIDTH_X),
};

//----------------------------------------------------------------------------------------------
//�����X�^�[�{�[���̈ʒu
//----------------------------------------------------------------------------------------------
enum{
	BALL_START_X			= (60),
	BALL_MULTI_START_X		= (28),
	BALL_START_Y			= (62),
	BALL_WIDTH_X			= (64),
};

//----------------------------------------------------------------------------------------------
//���x���A�b�v�G�t�F�N�g�̈ʒu
//----------------------------------------------------------------------------------------------
enum{
	LV_UPDOWN_START_X			= (ICON_START_X + 16),
	LV_UPDOWN_MULTI_START_X		= (ICON_MULTI_START_X + 16),
	LV_UPDOWN_START_Y			= (ICON_START_Y - 8),
	LV_UPDOWN_WIDTH_X			= (ICON_WIDTH_X),
};

//----------------------------------------------------------------------------------------------
//��񃁃b�Z�[�W�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	INFO_SINGLE_OFFSET_X	= (4*8),
	INFO_MULTI_L_OFFSET_X	= (0),				//�y�A�̃I�t�Z�b�g(�k)
	INFO_MULTI_R_OFFSET_X	= (16*8-4),			//�y�A�̃I�t�Z�b�g(�q)

	INFO_PLAYER_X			= (1), 
	INFO_PLAYER_Y			= (1),
	INFO_PLAYER_CP_X		= (7*8), 
	INFO_PLAYER_CP_Y		= (INFO_PLAYER_Y),
};

//----------------------------------------------------------------------------------------------
//HP�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_HP_START_X		= (4*8+4),
	CASTLE_HPSLASH_START_X	= (3*8),
	CASTLE_HPMAX_START_X	= (1*8 + CASTLE_HPSLASH_START_X),
	CASTLE_MULTI_HP_START_X	= (4),
	CASTLE_HP_START_Y		= (1),
	CASTLE_HP_WIDTH_X		= (8*8),					//��
};

//----------------------------------------------------------------------------------------------
//LV�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_LV_START_X		= (CASTLE_HP_START_X+4),
	CASTLE_MULTI_LV_START_X	= (8),
	CASTLE_LV_START_Y		= (1),
	CASTLE_LV_WIDTH_X		= (CASTLE_HP_WIDTH_X),		//��
};

//----------------------------------------------------------------------------------------------
//���ʂ̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_SEX_START_X		= (CASTLE_LV_START_X + (5*8)),
	CASTLE_MULTI_SEX_START_X= (CASTLE_MULTI_LV_START_X + (5*8)),
	CASTLE_SEX_START_Y		= (1),
	CASTLE_SEX_WIDTH_X		= (CASTLE_HP_WIDTH_X),		//��
};

//----------------------------------------------------------------------------------------------
//�I�����Ă���|�P�����E�B���h�E�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_POKE_SEL_START_X			= (72),
	CASTLE_POKE_SEL_START_X2		= (CASTLE_HP_WIDTH_X+72),
	CASTLE_POKE_SEL_START_X3		= (CASTLE_HP_WIDTH_X*2+72),
	CASTLE_MULTI_POKE_SEL_START_X	= (40),
	CASTLE_MULTI_POKE_SEL_START_X2	= (CASTLE_MULTI_POKE_SEL_START_X+CASTLE_HP_WIDTH_X),
	CASTLE_MULTI_POKE_SEL_START_X3	= (168),
	CASTLE_MULTI_POKE_SEL_START_X4	= (CASTLE_MULTI_POKE_SEL_START_X3+CASTLE_HP_WIDTH_X),
	CASTLE_POKE_SEL_START_Y			= (88),
};

//----------------------------------------------------------------------------------------------
//�J�[�\���֘A
//----------------------------------------------------------------------------------------------
enum{
	//�J�n�ʒu
	CSR_MULTI_START_X		= (1*8),
	CSR_START_Y				= (12*8),

	//��
	CSR_WIDTH_X				= (8*8),
	CSR_WIDTH_Y				= (2*8),

	//���ѐ�
	CSR_H_MAX				= (3),
	CSR_V_MAX				= (4),

	//���ǂ�
	ENEMY_CSR_MODORU_X		= (28*8),
	ENEMY_CSR_MODORU_Y		= (20*8),

	SEL_CSR_L_OFFSET		= (10),			//�����J�[�\���̃I�t�Z�b�g
	SEL_CSR_R_OFFSET		= (55),			//�E���J�[�\���̃I�t�Z�b�g
};


#endif //_CASTLE_ENEMY_DEF_H_


