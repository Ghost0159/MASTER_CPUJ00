//==============================================================================
/**
 * @file	footprint_id.h
 * @brief	�L����ID�A�Z��ID���̒�`�ꏊ
 * @author	matsuda
 * @date	2008.01.26(�y)
 */
//==============================================================================
#ifndef __FOOTPRINT_ID_H__
#define __FOOTPRINT_ID_H__


//==============================================================================
//	�L����ID
//==============================================================================
enum{
	CHARID_FOOTPRINT_START = 10000,
	
	CHARID_INK_FOOT_0,				//�C���N�̏�ɒu�����ՁF0�Ԗ�
	CHARID_INK_FOOT_1,				//�C���N�̏�ɒu�����ՁF1�Ԗ�
	CHARID_INK_FOOT_2,				//�C���N�̏�ɒu�����ՁF2�Ԗ�
	CHARID_INK_FOOT_3,				//�C���N�̏�ɒu�����ՁF3�Ԗ�
	CHARID_INK_FOOT_4,				//�C���N�̏�ɒu�����ՁF4�Ԗ�
	CHARID_INK_FOOT_5,				//�C���N�̏�ɒu�����ՁF5�Ԗ�
	CHARID_INK,						//�C���N
	CHARID_INK_FOUNDATION = CHARID_INK,			//�C���N�̉��ɕ~�����n
	CHARID_TOUCH_EFF,				//�C���N���^�b�`�������ɏo���G�t�F�N�g
	
	//-- �T�u��� --//
	CHARID_SUB_NAME_FRAME,			///<���O���͂ރt���[��
	CHARID_SUB_NAME_FOOT_0,			///<���O�̉��ɒu�����ՁF0�Ԗ�
	CHARID_SUB_NAME_FOOT_1,			///<���O�̉��ɒu�����ՁF1�Ԗ�
	CHARID_SUB_NAME_FOOT_2,			///<���O�̉��ɒu�����ՁF2�Ԗ�
	CHARID_SUB_NAME_FOOT_3,			///<���O�̉��ɒu�����ՁF3�Ԗ�
	CHARID_SUB_NAME_FOOT_4,			///<���O�̉��ɒu�����ՁF4�Ԗ�
	CHARID_SUB_NAME_FOOT_5,			///<���O�̉��ɒu�����ՁF5�Ԗ�
	CHARID_SUB_NAME_FOOT_6,			///<���O�̉��ɒu�����ՁF6�Ԗ�
	CHARID_SUB_NAME_FOOT_7,			///<���O�̉��ɒu�����ՁF7�Ԗ�
};

//==============================================================================
//	�p���b�gID
//==============================================================================
enum{
	PLTTID_FOOTPRINT_START = 10000,

	PLTTID_OBJ_COMMON,	///<�풓OBJ�p���b�g
	PLTTID_OBJ_INK_FOOT,	///<�C���N�p���b�g�ɒu����Ă��鎩���̑���
	PLTTID_OBJ_FONTOAM,		///<FONTOAM�p�p���b�g
	
	//-- �T�u��� --//
	PLTTID_SUB_OBJ_COMMON,	///<�T�u��ʂ̏풓OBJ�p���b�g
};

//--------------------------------------------------------------
//	�풓OBJ�p���b�g�I�t�Z�b�g
//--------------------------------------------------------------
enum{
	PALOFS_INK = 1,					///<�C���N
	PALOFS_INK_FOUNDATION = 2,		///<�C���N�̉��ɕ~�����n
	PALOFS_TOUCH_EFF = 0,			///<�C���N���^�b�`�������ɏo���G�t�F�N�g
	
	FOOTPRINT_COMMON_PAL_NUM = 3,	///<�풓OBJ�p���b�g�̖{��
};

//--------------------------------------------------------------
//	�T�u��ʂ̏풓OBJ�p���b�g�I�t�Z�b�g
//--------------------------------------------------------------
enum{
	PALOFS_SUB_NAME_FRAME = 0,		///<���O���͂ރt���[��
	PALOFS_SUB_NAME_FOOT = 0,		///<���O�̉��ɔz�u���鑫��

	FOOTPRINT_SUB_COMMON_PAL_NUM = 1,	///<�T�u��ʂ̏풓OBJ�p���b�g�̖{��
};

//--------------------------------------------------------------
//	�p���b�g�̃J���[�ʒu�w��
//--------------------------------------------------------------
enum{
	COLOR_NO_INK_START = 1,			///<�C���N�̃J���[�J�n�ʒu
	
	//-- �T�u��� --//
	COLOR_NO_SUB_NAME_FOOT = 7,		///<���O�̉��ɔz�u���鑫��
};

//==============================================================================
//	�Z��ID
//==============================================================================
enum{
	CELLID_FOOTPRINT_START = 10000,

	CELLID_INK_FOOT,				//�C���N�̏�ɒu������
	CELLID_INK,						//�C���N
	CELLID_INK_FOUNDATION = CELLID_INK,			//�C���N�̉��ɕ~�����n
	CELLID_TOUCH_EFF,				//�C���N���^�b�`�������ɏo���G�t�F�N�g

	//-- �T�u��� --//
	//-- �T�u��� --//
	CELLID_SUB_NAME_FRAME,			///<���O���͂ރt���[��
	CELLID_SUB_NAME_FOOT,			///<���O�̉��ɒu������
};

//==============================================================================
//	�Z���A�j��ID
//==============================================================================
enum{
	CELLANMID_FOOTPRINT_START = 10000,

	CELLANMID_INK_FOOT,					//�C���N�̏�ɒu������
	CELLANMID_INK,						//�C���N
	CELLANMID_INK_FOUNDATION = CELLANMID_INK,			//�C���N�̉��ɕ~�����n
	CELLANMID_TOUCH_EFF,				//�C���N���^�b�`�������ɏo���G�t�F�N�g

	//-- �T�u��� --//
	//-- �T�u��� --//
	CELLANMID_SUB_NAME_FRAME,			///<���O���͂ރt���[��
	CELLANMID_SUB_NAME_FOOT,			///<���O�̉��ɒu������
};

//==============================================================================
//	�}���`�Z��ID
//==============================================================================
enum{
	MCELLID_FOOTPRINT_START = 10000,
};

//==============================================================================
//	�}���`�Z���A�j��ID
//==============================================================================
enum{
	MCELLANMID_FOOTPRINT_START = 10000,
};



#endif	//__FOOTPRINT_ID_H__
