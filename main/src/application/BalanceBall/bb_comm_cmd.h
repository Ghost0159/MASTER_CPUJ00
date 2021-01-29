//==============================================================================
/**
 * @file	bb_comm_cmd.h
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.09.25(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef __BB_COMM_CMD_H__
#define __BB_COMM_CMD_H__

#include "communication/communication.h"

#include "bb_common.h"

enum CommCommandBB {
	
	CCMD_BB_START	= CS_COMMAND_MAX,	///< �ʐM�J�n
	CCMD_BB_END,						///< �e > �q
	CCMD_BB_STATE,						///< �q > �S
	CCMD_BB_RESULT,						///< �q > �S
	CCMD_BB_SCORE,						///< �e > �S
	
	CCMD_BB_CONNECT_END,	
};

extern const CommPacketTbl* BB_CommCommandTclGet( void );
extern int BB_CommCommandTblNumGet( void );
extern void  BB_CommCommandInit( BB_WORK* wk );

#endif	//