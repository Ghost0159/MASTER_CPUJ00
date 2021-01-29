//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		minigame_commcomand.h
 *	@brief		�~�j�Q�[���c�[���ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.11.08
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __MINIGAME_COMMCOMAND_H__
#define __MINIGAME_COMMCOMAND_H__

#include "communication/communication.h"

enum CommCommandMNGM {

	//-------------------------------------
	//�@�Q�[���p
	//=====================================
	CNM_MNGM_RETRY_YES = CS_COMMAND_MAX,	//< �q�[���e	RETRY YES
	CNM_MNGM_RETRY_NO,						//<	�q�[���e	RETRY�@NO
	CNM_MNGM_RETRY_OK,						//< �e�[���q	RETRY
	CNM_MNGM_RETRY_NG,						//< 
	CNM_MNGM_ENTRY_RAREGAME,				//<	�e�[���q	Rare�Q�[���^�C�v
	

	//-------------------------------------
	//	
	//=====================================
	
	//------------------------------------------------�����܂�
	CNM_COMMAND_MAX   // �I�[--------------����͈ړ������Ȃ��ł�������
};

extern const CommPacketTbl* MNGM_CommCommandTclGet( void );
extern int MNGM_CommCommandTblNumGet( void );

#endif		// __MINIGAME_COMMCOMAND_H__

