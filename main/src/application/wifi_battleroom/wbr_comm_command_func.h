//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_comm_command_func.h
 *	@brief		wifi �o�g�����[���@����M�R�[���o�b�N�֐�
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WBR_COMM_COMMAND_FUNC_H__
#define __WBR_COMM_COMMAND_FUNC_H__



#undef GLOBAL
#ifdef	__WBR_COMM_COMMAND_FUNC_H_GLOBAL
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
GLOBAL void WBR_CNM_GameSysStart( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_GameSysGameStart( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_GameSysEnd( int netID, int size, void* pBuff, void* p_work );

GLOBAL void WBR_CNM_KoCommonRecv( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_KoGameDataRecv( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_OyaGameDataRecv( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_KoOyaDataGetRecv( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_KoTalkDataRecv( int netID, int size, void* pBuff, void* p_work );
GLOBAL void WBR_CNM_KoTalkReqRecv( int netID, int size, void* pBuff, void* p_work );

// �o�b�t�@�擾�R�[���o�b�N�֐�
GLOBAL u8* WBR_CNM_KoCommRecvBuffGet( int netID, void* p_work, int size );

// �f�[�^�T�C�Y
GLOBAL int WBR_CNM_ZeroSizeGet( void );
GLOBAL int WBR_CNM_KoCommonSizeGet( void );
GLOBAL int WBR_CNM_OyaCommonSizeGet( void );
GLOBAL int WBR_CNM_KoGameDataSizeGet( void );
GLOBAL int WBR_CNM_OyaGameDataSizeGet( void );
GLOBAL int WBR_CNM_KoTalkDataSizeGet( void );
GLOBAL int WBR_CNM_KoTalkReqSizeGet( void );

#undef	GLOBAL
#endif		// __WBR_COMM_COMMAND_FUNC_H__

