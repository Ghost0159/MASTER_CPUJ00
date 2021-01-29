//==============================================================================
/**
 * @file	balloon_id.h
 * @brief	�L����ID�A�Z��ID���̒�`�ꏊ
 * @author	matsuda
 * @date	2007.11.27(��)
 */
//==============================================================================
#ifndef __BALLOON_ID_H__
#define __BALLOON_ID_H__


//==============================================================================
//	�L����ID
//==============================================================================
enum{
	CHARID_BALLOON_START = 10000,
	
	CHARID_TOUCH_PEN,		///<�^�b�`�y��
	CHARID_COUNTER_WIN,		///<�J�E���^�[�̃E�B���h�E
	CHARID_BALLOON_AIM,		///<�Ə�
	
	//-- �T�u��� --//
	CHARID_SUB_BALLOON_MIX,			///<�F�X�ȃL���������፬��
};

//==============================================================================
//	�p���b�gID
//==============================================================================
enum{
	PLTTID_BALLOON_START = 10000,

	PLTTID_OBJ_COMMON,	///<�풓OBJ�p���b�g
	
	PLTTID_TOUCH_PEN,	///<�^�b�`�y��
	PLTTID_COUNTER,		///<�J�E���^�[
	PLTTID_COUNTER_WIN,	///<�J�E���^�[�̃E�B���h�E
	PLTTID_OBJ_AIM,		///<�Ə�
	
	//-- �T�u��� --//
	PLTTID_SUB_OBJ_COMMON,	///<�T�u��ʂ̏풓OBJ�p���b�g
};

//--------------------------------------------------------------
//	�풓OBJ�p���b�g�I�t�Z�b�g
//--------------------------------------------------------------
enum{
	PALOFS_NONE = 0,

	
	BALLOON_COMMON_PAL_NUM = 3,	///<�풓OBJ�p���b�g�̖{��
};

//--------------------------------------------------------------
//	�T�u��ʂ̏풓OBJ�p���b�g�I�t�Z�b�g
//--------------------------------------------------------------
enum{
	PALOFS_SUB_AIR_BLUE = 1,			///<��C�F��
	PALOFS_SUB_AIR_RED = 2,				///<��C�F��
	PALOFS_SUB_AIR_GREEN = 4,			///<��C�F��
	PALOFS_SUB_AIR_YELLOW = 3,			///<��C�F��

	PALOFS_SUB_EXPLODED = 5,				///<�������̎��ӂԂ�
	PALOFS_SUB_EXPLODED_SMOKE = 0,			///<�������̉�
	PALOFS_SUB_EXPLODED_CHIP_BLUE =6,		///<�j��:��
	PALOFS_SUB_EXPLODED_CHIP_RED =7,		///<�j��:��
	PALOFS_SUB_EXPLODED_CHIP_YELLOW =8,		///<�j��:��

	PALOFS_SUB_ICON_BALLOON = 0,	///<���D�A�C�R��
	
	PALOFS_SUB_JOINT = 0,			///<�W���C���g
	PALOFS_SUB_BOOSTER_BLUE = 1,	///<�u�[�X�^�[(��)
	PALOFS_SUB_BOOSTER_RED = 2,		///<�u�[�X�^�[(��)
	PALOFS_SUB_BOOSTER_YELLOW = 3,	///<�u�[�X�^�[(��)
	PALOFS_SUB_BOOSTER_GREEN = 4,	///<�u�[�X�^�[(��)
	PALOFS_SUB_BOOSTER_HIT = 0,		///<�u�[�X�^�[:�q�b�g�G�t�F�N�g
	PALOFS_SUB_BOOSTER_LAND_SMOKE = 0,	///<�u�[�X�^�[:���n�̉�
	PALOFS_SUB_BOOSTER_SHADOW = 1,	///<�u�[�X�^�[:�e
	
	BALLOON_SUB_COMMON_PAL_NUM = 9,	///<�T�u��ʂ̏풓OBJ�p���b�g�̖{��
};

//==============================================================================
//	�Z��ID
//==============================================================================
enum{
	CELLID_BALLOON_START = 10000,

	CELLID_TOUCH_PEN,		///<�^�b�`�y��
	CELLID_COUNTER_WIN,		///<�J�E���^�[�̃E�B���h�E
	CELLID_BALLOON_AIM,		///<�Ə�

	//-- �T�u��� --//
	CELLID_SUB_BALLOON_MIX,			///<�F�X�ȃZ�������፬��
};

//==============================================================================
//	�Z���A�j��ID
//==============================================================================
enum{
	CELLANMID_BALLOON_START = 10000,

	CELLANMID_TOUCH_PEN,		///<�^�b�`�y��
	CELLANMID_COUNTER_WIN,		///<�J�E���^�[�̃E�B���h�E
	CELLANMID_BALLOON_AIM,		///<�Ə�

	//-- �T�u��� --//
	CELLANMID_SUB_BALLOON_MIX,		///<�F�X�ȃZ���A�j�������፬��
};

//==============================================================================
//	�}���`�Z��ID
//==============================================================================
enum{
	MCELLID_BALLOON_START = 10000,
};

//==============================================================================
//	�}���`�Z���A�j��ID
//==============================================================================
enum{
	MCELLANMID_BALLOON_START = 10000,
};



#endif	//__BALLOON_ID_H__
