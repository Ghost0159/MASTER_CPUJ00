//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		minigame_commcomand_func.c
 *	@brief		�~�j�Q�[���c�[���ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.11.08
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "minigame_commcomand.h"
#include "minigame_commcomand_func.h"

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


// �ʐM�������֐�
void CommCommandMNGMInitialize( MNGM_RESULTWK* p_wk )
{
	CommCommandInitialize( MNGM_CommCommandTclGet(), 
			MNGM_CommCommandTblNumGet(), p_wk );
}
void CommCommandMNGMEntryInitialize( MNGM_ENTRYWK* p_wk )
{
	CommCommandInitialize( MNGM_CommCommandTclGet(), 
			MNGM_CommCommandTblNumGet(), p_wk );
}

// ���ʃT�C�Y�擾�֐�
int CommMNGMGetZeroSize( void )
{
	return 0;
}
int CommMNGMGetu32Size( void )
{
	return sizeof(u32);
}

// �e�[���q�@Rare�Q�[���^�C�v
void CommMNGMRareGame( int netID, int size, void* pBuff, void* pWork )
{
	MNGM_ENTRY_SetRareGame( pWork, *((u32*)pBuff) );
}


// �q�[>�e	����������
void CommMNGMRetryYes( int netID, int size, void* pBuff, void* pWork )
{
	MNGM_RESULT_SetKoRetry( pWork, netID, TRUE );
}

// �q�[>�e	�������Ȃ�
void CommMNGMRetryNo( int netID, int size, void* pBuff, void* pWork )
{
	MNGM_RESULT_SetKoRetry( pWork, netID, FALSE );
}


// �e�[>�q	����������
void CommMNGMRetryOk( int netID, int size, void* pBuff, void* pWork )
{
	MNGM_RESULT_SetOyaRetry( pWork, TRUE );
}

// �e�[>�q	�������Ȃ�
void CommMNGMRetryNg( int netID, int size, void* pBuff, void* pWork )
{
	MNGM_RESULT_SetOyaRetry( pWork, FALSE );
}

