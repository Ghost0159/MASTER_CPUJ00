//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		minigame_commcomand_func.h
 *	@brief		�~�j�Q�[���c�[���ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.11.08
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __MINIGAME_COMMCOMAND_FUNC_H__
#define __MINIGAME_COMMCOMAND_FUNC_H__

#include "minigame_tool_local.h"

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
extern void CommCommandMNGMInitialize( MNGM_RESULTWK* p_wk );
extern void CommCommandMNGMEntryInitialize( MNGM_ENTRYWK* p_wk );

// ���ʃT�C�Y�擾�֐�
extern int CommMNGMGetZeroSize( void );
extern int CommMNGMGetu32Size( void );

// �e�[���q�@Rare�Q�[���^�C�v
extern void CommMNGMRareGame( int netID, int size, void* pBuff, void* pWork );


// �q�[>�e	����������
extern void CommMNGMRetryYes( int netID, int size, void* pBuff, void* pWork );
// �q�[>�e	�������Ȃ�
extern void CommMNGMRetryNo( int netID, int size, void* pBuff, void* pWork );

// �e�[>�q	����������
extern void CommMNGMRetryOk( int netID, int size, void* pBuff, void* pWork );
// �e�[>�q	�������Ȃ�
extern void CommMNGMRetryNg( int netID, int size, void* pBuff, void* pWork );

#endif		// __MINIGAME_COMMCOMAND_FUNC_H__

