//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_comm_command.h
 *	@brief
 *	@author	
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_COMM_COMMAND_H__
#define __WBR_COMM_COMMAND_H__

#include "communication/comm_command.h"

#undef GLOBAL
#ifdef	__WBR_COMM_COMMAND_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
enum WbrCommCommand {

	//-------------------------------------
	//�@�Q�[���p
	//=====================================
	// �����p	
	CNM_WBR_GAME_SYS_START = CS_COMMAND_MAX,	///< OYA->KO ��������
	CNM_WBR_GAME_SYS_GAMESTART,					///< OYA->KO �ʐM�����X�^�[�g
	CNM_WBR_GAME_SYS_END,						///< OYA->KO �I�� 
	
	// �f�[�^��M�p
	CNM_WBR_KO_COMMON,		///< KO->OYA ���ʃf�[�^
	CNM_WBR_KO_GAMEDATA,	///< KO->OYA �Q�[���R�}���h
	CNM_WBR_OYA_GAMEDATA,	///< OYA->KO �Q�[����ԃf�[�^
	CNM_WBR_KO_OYADATA_GET,	///< KO->OYA ���݂̃Q�[����ԋ����擾
	CNM_WBR_KO_TALKDATA,	///< KO->OYA �b�������f�[�^
	CNM_WBR_KO_TALKREQ,		///< KO->OYA �b���������N�G�X�g

	//-------------------------------------
	//	
	//=====================================
	
	//------------------------------------------------�����܂�
	CNM_COMMAND_MAX   // �I�[--------------����͈ړ������Ȃ��ł�������
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL const CommPacketTbl* Wbr_CommCommandTclGet( void );

GLOBAL int Wbr_CommCommandTblNumGet( void );

#undef	GLOBAL
#endif		// __WBR_COMM_COMMAND_H__

