//==============================================================================================
/**
 * @file	castle_mine_def.h
 * @brief	�u�o�g���L���b�X���v�莝����ʂ̒�`
 * @author	Satoshi Nohara
 * @date	07.07.17
 *
 * "castle_mine.c"�̂�include��
 */
//==============================================================================================
#ifndef _CASTLE_MINE_DEF_H_
#define _CASTLE_MINE_DEF_H_


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
#define CSR_POS_NONE			(0xff)							//�J�[�\�������I��ł��Ȃ�
#define CASTLE_MINE_DECIDE_NONE	(0xff)							//���薳��

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
//�A�C�e�����̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	 ITEM_INFO_PX			= (1),
	 ITEM_INFO_PY			= (0),
	 ITEM_INFO_SX			= (23*8),
	 ITEM_INFO_SY			= (4*8),
};

//----------------------------------------------------------------------------------------------
//�|�P�����A�C�R���̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	ICON_START_X			= (64),
	ICON_MULTI_START_X		= (32),				//�ʐM
	ICON_START_Y			= (58),
	ICON_WIDTH_X			= (64),
};

//----------------------------------------------------------------------------------------------
//�A�C�e���A�C�R���\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_ITEMICON_X		= (28),
	CASTLE_ITEMICON_Y		= (164),
};

//----------------------------------------------------------------------------------------------
//�񕜁A�����N�A�b�v�G�t�F�N�g�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	KAIHUKU_EFF_START_X			= (ICON_START_X),
	KAIHUKU_EFF_MULTI_START_X	= (ICON_MULTI_START_X),		//�ʐM
	KAIHUKU_EFF_START_Y			= (ICON_START_Y + 4),
	KAIHUKU_EFF_WIDTH_X			= (ICON_WIDTH_X),
};

//----------------------------------------------------------------------------------------------
//�����N�A�b�v���G�t�F�N�g�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	RANKUP_HATA_EFF_START_X			= (ICON_START_X),
	RANKUP_HATA_EFF_MULTI_START_X	= (ICON_MULTI_START_X),		//�ʐM
	RANKUP_HATA_EFF_START_Y			= (ICON_START_Y + 64),
	RANKUP_HATA_EFF_WIDTH_X			= (16),
};


//----------------------------------------------------------------------------------------------
//��񃁃b�Z�[�W�̕\���ʒu(�Ă����A���x���A�A�A)
//----------------------------------------------------------------------------------------------
enum{
	INFO_SINGLE_OFFSET_X	= (3*8+4),
	INFO_MULTI_L_OFFSET_X	= 0,				//�y�A�̃I�t�Z�b�g(�k)
	INFO_MULTI_R_OFFSET_X	= (16*8-4),			//�y�A�̃I�t�Z�b�g(�q)

	INFO_PLAYER_X			= 4, 
	INFO_PLAYER_Y			= 1,
	INFO_PLAYER_CP_X		= (7*8), 
	INFO_PLAYER_CP_Y		= INFO_PLAYER_Y,
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
//�J�[�\���̕��сA�ʒu�Ȃ�
//----------------------------------------------------------------------------------------------
enum{
	//�J�n�ʒu
	CSR_MULTI_START_X		= (1*8),
	CSR_START_Y				= (12*8),

	//��
	CSR_WIDTH_X				= (8*8),
	CSR_WIDTH_Y				= (2*8),

	//���ѐ�
	CSR_H_MAX				= 3,
	CSR_V_MAX				= 4,

	//�u���ǂ�v
	MINE_CSR_MODORU_X		= (28*8),
	MINE_CSR_MODORU_Y		= (20*8),

	SEL_CSR_L_OFFSET		= (14),						//�����J�[�\���̃I�t�Z�b�g
	SEL_CSR_R_OFFSET		= (58),						//�E���J�[�\���̃I�t�Z�b�g
};

//----------------------------------------------------------------------------------------------
//���X�g�̃X�N���[���J�[�\���\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_SCR_U_X			= (160),
	CASTLE_SCR_U_Y			= (10),
	CASTLE_SCR_D_X			= (CASTLE_SCR_U_X),
	CASTLE_SCR_D_Y			= (124),
};

//----------------------------------------------------------------------------------------------
//�A�C�e���E�B���h�E�̃|�P�����A�C�R���̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_ITEM_POKE_ICON_X	= (40),
	CASTLE_ITEM_POKE_ICON_Y	= (80),
	CASTLE_ITEMKEEP_OFFSET_X	= (8),
	CASTLE_ITEMKEEP_OFFSET_Y	= (4),
};

//----------------------------------------------------------------------------------------------
//���X�g�̃A�C�e���b�o�l�̕\���ʒu
//----------------------------------------------------------------------------------------------
enum{
	CASTLE_LIST_ITEM_CP_X	= (11*8),//(10*8),
};


#endif //_CASTLE_MINE_DEF_H_

