//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		comm_bct_command_func.c
 *	@brief		�o�P�b�g�~�j�Q�[���@�ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.06.20
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "communication/communication.h"
#include "comm_bct_command.h"

#include "comm_bct_command_func.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM�R�}���h�ݒ�
 *	
 *	@param	p_wk			���[�N
 */
//-----------------------------------------------------------------------------
void CommCommandBCTInitialize( BUCKET_WK* p_wk )
{
	CommCommandInitialize( BCT_CommCommandTclGet(), 
			BCT_CommCommandTblNumGet(), p_wk );
}

int CommBCTGetZeroSize( void )
{
	return 0;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���J�n	��M
 *
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTGameStart( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;
	
	Bucket_StartSet( p_wk );
	TOMOYA_PRINT( "recv start\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���I���@��M
 *
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTGameEnd( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;

	Bucket_EndSet( p_wk );
	TOMOYA_PRINT( "recv end\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��f�[�^�@�ʐM
 *
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTNuts( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;

	Bucket_ClientNutsSet( p_wk, pBuff, netID );

	if( CommGetCurrentID() == COMM_PARENT_ID ){
		Bucket_SurverNutsSet( p_wk, pBuff, netID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�R�A����M
 *
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTScore( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;
	Bucket_SurverScoreSet( p_wk, *((u32*)pBuff), netID );

	OS_Printf( "score get netid[%d]\n", netID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�݂�Ȃ̃X�R�A����M
 *	
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTAllScore( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;
	Bucket_ClientAllScoreSet( p_wk, pBuff, netID );

	OS_Printf( "all score get netid[%d]\n", netID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���̃X�R�A����M
 *	
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTMiddleScore( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;
	u32 score;

	score = *((u32*)pBuff);

	Bucket_ClientMiddleScoreSet( p_wk, score, netID );

//	OS_Printf( "middle score get netid[%d]\n", netID );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���̃X�R�A���݂�Ȃ���󂯎������	�i�e�[���q�j
 *	
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTMiddleScoreOk( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;

	Bucket_ClientMiddleScoreOkSet( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�̒i�K����M	�e�[���q
 *	
 *	@param	netID		NETID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	pWork		���[�N
 */
//-----------------------------------------------------------------------------
void CommBCTGameDataIdx( int netID, int size, void* pBuff, void* pWork )
{
	BUCKET_WK* p_wk = pWork;
	u32 idx;

	idx = *((u32*)pBuff);
	// �e���炩�`�F�b�N
	if( netID == COMM_PARENT_ID ){
		Bucket_ClientGameDataIdxSet( p_wk, idx );
		OS_Printf( "game level get[%d] level[%d]\n", netID, idx );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�؂̎��ʐM�f�[�^�T�C�Y�擾
 *	@return	�T�C�Y
 */
//-----------------------------------------------------------------------------
int CommBCTGetNutsSize( void )
{
	return sizeof(BCT_NUT_COMM);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�݂�Ȃ̓��_�ʐM�f�[�^�T�C�Y�擾
 *	@return	�T�C�Y
 */
//-----------------------------------------------------------------------------
int CommBCTGetAllScoreSize( void )
{
	return sizeof(BCT_SCORE_COMM);
}

//----------------------------------------------------------------------------
/**
 *	@brief	���_�ʐM�f�[�^�T�C�Y�擾
 *	@return	�T�C�Y
 */
//-----------------------------------------------------------------------------
int CommBCTGetScoreSize( void )
{
	return sizeof(u32);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�C���f�b�N�X�T�C�Y�擾
 *	@return	�T�C�Y
 */
//-----------------------------------------------------------------------------
int CommBCTGetGameDataIdxSize( void )
{
	return sizeof(u32);
}
