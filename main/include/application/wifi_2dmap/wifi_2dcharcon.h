//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_2dcharcon.h
 *	@brief		�萔��define�o�[�W����
 *	@author		tomoya takahashi
 *	@data		2007.04.12
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_2DCHARCON_H__
#define __WIFI_2DCHARCON_H__

//-------------------------------------
///	����^�C�v
// �����I�ɂ́A�Z���A�j�����\�[�X�̎��
//=====================================
#define	WF2DC_C_MOVERUN	(0)		// �����{�U������{����A�j���i��l���̂݁j
#define	WF2DC_C_MOVENORMAL	(1)	// �����{�U������̂݃A�j��
#define	WF2DC_C_MOVETURN	(2)	// �U������̂݃A�j��
		// �g���A�j��
		// �A�j���[�V�����^�C�v��
#define	WF2DC_C_MOVENUM	(3)

//-------------------------------------
///	�A�j���[�V�����^�C�v
//�@����^�C�v���I�[�\�h�b�N�X�A�j���̏ꍇ�ɂ̂�
//�@�g�p�ł���t���O�ł��B
//	
//=====================================
#define	WF2DC_C_ANMWAY	(0)	// �����ς��A�j��	1�t���[���Ő؂�ւ��܂�
#define	WF2DC_C_ANMROTA	(1)	// ��]�A�j��		���[�v

		// WF2DC_C_MOVETURN���[�h�łȂ��Ǝw��ł��܂���
#define	WF2DC_C_ANMWALK	(2)	// �����A�j��		1��8�t���[��
#define	WF2DC_C_ANMTURN	(3)	// �U������A�j��	2�t���[��

		// WF2DC_C_MOVERUN���[�h�łȂ��Ǝw��ł��܂���
#define	WF2DC_C_ANMRUN	(4)	// ����A�j��		1��4�t���[��

		// �Ǖ����A�j��
#define	WF2DC_C_ANMWALLWALK	(5)	// �Ǖ����A�j��		1��16�t���[��
		
		// ����������
#define	WF2DC_C_ANMSLOWWALK	(6)	// �������A�j��		1��16�t���[��

		// ��������
#define	WF2DC_C_ANMHIGHWALK2 (7)	// �������A�j��		1��2�t���[��
#define	WF2DC_C_ANMHIGHWALK4 (8)	// �������A�j��		1��4�t���[��
	
		// ���������Ƃ��͂���
#define	WF2DC_C_ANMNONE	(9)
#define	WF2DC_C_ANMNUM	(10)



#endif		// __WIFI_2DCHARCON_H__

