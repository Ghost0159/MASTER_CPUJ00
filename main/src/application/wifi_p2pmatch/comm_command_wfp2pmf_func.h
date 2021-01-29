//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		comm_command_wfp2pmf_func.h
 *	@brief		�Q�`�S�l��p�ҍ���	�ʐM�R�}���h
 *	@author		tomoya takahashi
 *	@data		2007.05.24
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __COMM_COMMAND_WFP2PMF_FUNC_H__
#define __COMM_COMMAND_WFP2PMF_FUNC_H__


#include "wifi_p2pmatchfour_local.h"

#undef GLOBAL
#ifdef	__COMM_COMMAND_WFP2PMF_FUNC_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

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
extern void CommCommandWFP2PMFInitialize( WFP2P_WK* p_wk );
extern void CommCommandWFP2PMF_MatchStartInitialize( void );	// wifi_p2pmatch.c���ŃR�}���h��ݒ肷��Ƃ��̊֐�

// ���ʃT�C�Y�擾�֐�
extern int CommWFP2PMFGetZeroSize( void );
extern int CommWFP2PMFGetWFP2PMF_COMM_RESULTSize( void );
extern int CommWFP2PMFGetWFP2PMF_COMM_VCHATSize( void );

// �e����̃��N�G�X�g
extern void CommWFP2PMFGameResult( int netID, int size, void* pBuff, void* pWork );
extern void CommWFP2PMFGameStart( int netID, int size, void* pBuff, void* pWork );
extern void CommWFP2PMFGameVchat( int netID, int size, void* pBuff, void* pWork );



#undef	GLOBAL
#endif		// __COMM_COMMAND_WFP2PMF_FUNC_H__

