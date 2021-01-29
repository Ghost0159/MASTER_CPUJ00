//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		comm_command_wfp2pmf.h
 *	@brief		�Q�`�S�l��p�ҍ���	�ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.05.24
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __COMM_COMMAND_WFP2PMF_H__
#define __COMM_COMMAND_WFP2PMF_H__



#undef GLOBAL
#ifdef	__COMM_COMMAND_WFP2PMF_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

enum CommCommandTradeList_e {

	//-------------------------------------
	//�@�Q�[���p
	//=====================================
	// �����p	
	CNM_WFP2PMF_RESULT = CS_COMMAND_MAX,	///< �ʐMKONG		�e�[���q
	CNM_WFP2PMF_START,						///< �Q�[���J�n		�e�[���q
	CNM_WFP2PMF_VCHAT,						///< VCHAT�f�[�^	�e�[���q

	//-------------------------------------
	//	
	//=====================================
	
	//------------------------------------------------�����܂�
	CNM_COMMAND_MAX   // �I�[--------------����͈ړ������Ȃ��ł�������
};

GLOBAL const CommPacketTbl* WFP2PMF_CommCommandTclGet( void );
GLOBAL int WFP2PMF_CommCommandTblNumGet( void );

#undef	GLOBAL
#endif		// __COMM_COMMAND_WFP2PMF_H__

