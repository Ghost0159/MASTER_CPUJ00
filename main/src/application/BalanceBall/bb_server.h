//==============================================================================
/**
 * @file	bb_server.h
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.09.25(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef __BB_SERVER_H__
#define __BB_SERVER_H__

#include "bb_common.h"

extern BB_SERVER* BB_Server_AllocMemory( int comm_num, BB_SYS* sys );

extern void		  BB_Server_FreeMemory( BB_SERVER* wk );
extern BOOL		  BB_Server_Main( BB_SERVER* wk );


#endif

