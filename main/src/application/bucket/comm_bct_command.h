//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		comm_bct_command.h
 *	@brief		�o�P�b�g�~�j�Q�[���@�ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.06.20
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __COMM_BCT_COMMAND_H__
#define __COMM_BCT_COMMAND_H__
#include "communication/communication.h"

enum CommCommandBCT {

	//-------------------------------------
	//�@�Q�[���p
	//=====================================
	CNM_BCT_START = CS_COMMAND_MAX,	//< �ʐM�J�n
	CNM_BCT_END,					///< �Q�[���I��				�e�[���q
	CNM_BCT_NUTS,					///< �؂̎��f�[�^			�q�[�����̑��݂��
	CNM_BCT_SCORE,					///< �����̓��_				�q�[���e
	CNM_BCT_ALLSCORE,				///< �S���̓��_				�e�[���q�@���ʔ��\�̃^�C�~���O�ɂ��g�p
	CNM_BCT_GAME_LEVEL,				///< �Q�[�����x���̑��M		�e�[���q�@
	CNM_BCT_MIDDLESCORE,			///< �r���̎����̓��_		�q�[���q
	CNM_BCT_MIDDLESCORE_OK,			///< �S���̓��_������		�e�[���q
	

	//-------------------------------------
	//	
	//=====================================
	
	//------------------------------------------------�����܂�
	CNM_COMMAND_MAX   // �I�[--------------����͈ړ������Ȃ��ł�������
};

extern const CommPacketTbl* BCT_CommCommandTclGet( void );
extern int BCT_CommCommandTblNumGet( void );

#endif		// __COMM_BCT_COMMAND_H__

