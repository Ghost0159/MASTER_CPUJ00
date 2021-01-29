//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		comm_bct_command_func.h
 *	@brief		�o�P�b�g�~�j�Q�[���@�ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.06.20
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __COMM_BCT_COMMAND_FUNC_H__
#define __COMM_BCT_COMMAND_FUNC_H__

#include "bct_local.h"

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
extern void CommCommandBCTInitialize( BUCKET_WK* p_wk );

// ���ʃT�C�Y�擾�֐�
extern int CommBCTGetZeroSize( void );
extern int CommBCTGetNutsSize( void );
extern int CommBCTGetAllScoreSize( void );
extern int CommBCTGetScoreSize( void );
extern int CommBCTGetGameDataIdxSize( void );

// �e����̃��N�G�X�g
extern void CommBCTGameStart( int netID, int size, void* pBuff, void* pWork );
extern void CommBCTGameEnd( int netID, int size, void* pBuff, void* pWork );

// �؂̎��f�[�^�ʐM
extern void CommBCTNuts( int netID, int size, void* pBuff, void* pWork );

// �X�R�A�ʐM
extern void CommBCTScore( int netID, int size, void* pBuff, void* pWork );
extern void CommBCTAllScore( int netID, int size, void* pBuff, void* pWork );
extern void CommBCTMiddleScore( int netID, int size, void* pBuff, void* pWork );
extern void CommBCTMiddleScoreOk( int netID, int size, void* pBuff, void* pWork );

// �Q�[�����x��
extern void CommBCTGameDataIdx( int netID, int size, void* pBuff, void* pWork );



#endif		// __COMM_BCT_COMMAND_FUNC_H__

