//==============================================================================
/**
 * @file	bb_server.c
 * @brief	�ȒP�Ȑ���������
 * @author	goto
 * @date	2007.10.01(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#include "common.h"
#include "bb_server.h"

//--------------------------------------------------------------
/**
 * @brief	�������m��
 *
 * @param	comm_num	
 *
 * @retval	BB_SERVER*	
 *
 */
//--------------------------------------------------------------
BB_SERVER* BB_Server_AllocMemory( int comm_num, BB_SYS* sys )
{
	BB_SERVER* wk = sys_AllocMemory( HEAPID_BB, sizeof( BB_SERVER ) );
	
	memset( wk, 0, sizeof( BB_SERVER ) );
	
	wk->comm_num = comm_num;
	wk->timer	 = BB_GAME_TIMER;
	wk->sys		 = sys;
	
	return wk; 
}


//--------------------------------------------------------------
/**
 * @brief	���������
 *
 * @param	wk	
 *
 * @retval	none	
 *
 */
//--------------------------------------------------------------
void BB_Server_FreeMemory( BB_SERVER* wk )
{
	sys_FreeMemoryEz( wk );
}


//--------------------------------------------------------------
/**
 * @brief	���C��
 *
 * @param	wk	
 *
 * @retval	BOOL	
 *
 */
//--------------------------------------------------------------
BOOL BB_Server_Main( BB_SERVER* wk )
{	
	if ( ( --wk->timer ) <= 0 ){
		return FALSE;
	}

//	if ( ( wk->timer % 30 ) == 0 ){	OS_Printf( " �c�� %2d �b\n", wk->timer / 30 ); }

	return TRUE;
}

