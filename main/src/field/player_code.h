//******************************************************************************
/**
 *
 * @file	player_code.h
 * @brief	���@�V���{���Q�Ɓ@���X�N���v�g�Q�ƃA���@�̂�enum�֎~
 * @author	kagaya
 * @data	05.08.03
 *
 */
//******************************************************************************
#ifndef PLAYER_CODE_H_FILE
#define PLAYER_CODE_H_FILE

//==============================================================================
//	define
//==============================================================================
//--------------------------------------------------------------
//	DEBUG
//--------------------------------------------------------------
#define DEBUG_PLAYER_B_BTN_FLIP

//--------------------------------------------------------------
///	���@�`��
//--------------------------------------------------------------
#define HERO_FORM_NORMAL 		(0x00)		///<2��
#define HERO_FORM_CYCLE			(0x01)		///<���]��
#define HERO_FORM_SWIM			(0x02)		///<�g���
#define HERO_FORM_MAX			(0x06)		///<�`�ԍő�

//�ȉ��`�掯�� ���ۂɂ͐ݒ肳��Ȃ�
#define HERO_DRAWFORM_SP		(0x10)		///<��`�Z
#define HERO_DRAWFORM_WATER		(0x11)		///<������
#define HERO_DRAWFORM_CONTEST	(0x12)		///<�R���e�X�g
#define HERO_DRAWFORM_FISHING	(0x13)		///<�ނ�
#define HERO_DRAWFORM_POKETCH	(0x14)		///<�|�P�b�`
#define HERO_DRAWFORM_SAVE		(0x15)		///<�Z�[�u
#define HERO_DRAWFORM_BANZAI	(0x16)		///<����
#define HERO_DRAWFORM_BS		(0x17)		///<�o�g���T�[�`���[
#define HERO_DRAWFORM_TW		(0x18)		///<�j�ꂽ���E
#define HERO_DRAWFORM_TWSWIM	(0x19)		///<�j�ꂽ���E�g���
#define HERO_DRAWFORM_TWSAVE	(0x1a)		///<�j�ꂽ���E�Z�[�u
#define HERO_DRAWFORM_TWPOKETCH	(0x1b)		///<�j�ꂽ���E�|�P�b�`
#define HERO_DRAWFORM_GSTGYM			(0x1c)	///<�S�[�X�g�W��
#define HERO_DRAWFORM_GSTGYM_SAVE		(0x1d)	///<�S�[�X�g�W���Z�[�u
#define HERO_DRAWFORM_GSTGYM_POKETCH	(0x1e)	///<�S�[�X�g�W���|�P�b�`

//���]�Ԃ͈��ނɂȂ�܂���
#define HERO_FORM_CYCLE_R (HERO_FORM_CYCLE)
#define HERO_FORM_CYCLE_D (HERO_FORM_CYCLE)

//--------------------------------------------------------------
///	���N�G�X�g
//--------------------------------------------------------------
#define HERO_REQBIT_NON			(0)							///<���N�G�X�g�Ȃ�
#define HERO_REQBIT_NORMAL		(1<<0)						///<�Q���ړ��ɕύX
#define HERO_REQBIT_CYCLE		(1<<1)						///<���]�Ԉړ��ɕύX
#define HERO_REQBIT_SWIM		(1<<2)						///<�g���ړ��ɕύX
#define HERO_REQBIT_UNDER_OFF	(1<<3)						///<��������𖳌��ɂ���
#define HERO_REQBIT_WATER		(1<<4)						///<�����莩�@�ɕύX����
#define HERO_REQBIT_FISHING		(1<<5)						///<�ނ莩�@�֕\���ύX
#define HERO_REQBIT_POKETCH		(1<<6)						///<�|�P�b�`���@�֕\���ύX
#define HERO_REQBIT_SAVE		(1<<7)						///<�Z�[�u���@�֕\���ύX
#define HERO_REQBIT_BANZAI		(1<<8)						///<���Ύ��@�֕\���ύX
#define HERO_REQBIT_BS			(1<<9)						///<BS���@�֕\���ύX
#define HERO_REQBIT_MAX			(10)						///<���N�G�X�g����

#endif //PLAYER_CODE_H_FILE
