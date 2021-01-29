//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_comm_command_func.c
 *	@brief		wifi �o�g�����[���@��M�R�[���o�b�N�֐�
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "communication/communication.h"

#include "wifi_battleroom_local.h"
#include "wbr_surver.h"
#include "wbr_client.h"

#define	__WBR_COMM_COMMAND_FUNC_H_GLOBAL
#include "wbr_comm_command_func.h"

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
 *	@brief	wifi_battleroom�@�^�X�N���s�J�n���q��M
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_GameSysStart( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	WBR_ClientSeqSet( p_sys->p_client, WBR_CLIENT_SEQ_COMMDATA_SEND );

	OS_Printf( "���ʃf�[�^�̑��M�J�n\n\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�[���q�@�Q�[�������̊J�n
 *
 *	@param	netID	
 *	@param	size
 *	@param	pBuff
 *	@param	p_work 
 */
//-----------------------------------------------------------------------------
void WBR_CNM_GameSysGameStart( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;

	// �N���C�A���g�V�X�e���͎��ۂ̒ʐM������
	WBR_ClientSeqSet( p_sys->p_client, WBR_CLIENT_SEQ_WALK_START );

	OS_Printf( "�N���C�A���g�@�����J�n\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���I�����q��M
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_GameSysEnd( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	WBR_ClientGameEndSet( p_sys->p_client );

	OS_Printf( "�Q�[���I��\n" );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�q�@����̋��ʃf�[�^��M
 *
 *	@param	netID	�l�b�gID
 *	@param	size	�T�C�Y
 *	@param	pBuff	�o�b�t�@
 *	@param	p_work	���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_KoCommonRecv( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	BOOL result;

	if( p_sys->p_surver ){
		result = WBR_SurverKoCommDataRecv( p_sys->p_surver, netID );
		if( result == TRUE ){
			WBR_SurverSeqSet( p_sys->p_surver, WBR_SURVER_SEQ_MAIN_START );
			OS_Printf( "�q�@���ʃf�[�^��M����\n" );
		}
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�q����Q�[���R�}���h�f�[�^���擾����
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_KoGameDataRecv( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;

	if( p_sys->netid == COMM_PARENT_ID ){
		WBR_SurverKoGameDataSet( p_sys->p_surver, pBuff, netID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e����̃Q�[�����f�[�^���擾����
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_OyaGameDataRecv( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	WBR_ClientGameDataSet( p_sys->p_client, pBuff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q����̐e�Q�[���f�[�^������M�v��
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_KoOyaDataGetRecv( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	
	if( p_sys->netid == COMM_PARENT_ID ){
		WBR_SurverDataSend( p_sys->p_surver );
	}

}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������f�[�^��M
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_KoTalkDataRecv( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	
	if( p_sys->netid == COMM_PARENT_ID ){
		WBR_SurverKoTalkDataSet( p_sys->p_surver, pBuff, netID );
	}

	// �N���C�A���g�ɂ��o�^
	WBR_ClientTalkDataSet( p_sys->p_client, pBuff );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������J�n�f�[�^��M
 *
 *	@param	netID		�l�b�gID
 *	@param	size		�T�C�Y
 *	@param	pBuff		�o�b�t�@
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
void WBR_CNM_KoTalkReqRecv( int netID, int size, void* pBuff, void* p_work )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	
	if( p_sys->netid == COMM_PARENT_ID ){
		WBR_SurverKoTalkReqSet( p_sys->p_surver, pBuff, netID );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�̋��ʃf�[�^�擾��o�b�t�@�|�C���^��Ԃ�
 *
 *	@param	netID		�l�b�gID
 *	@param	p_work		���[�N
 *	@param	size		�T�C�Y
 *
 *	@return	�o�b�t�@�|�C���^
 */
//-----------------------------------------------------------------------------
u8* WBR_CNM_KoCommRecvBuffGet( int netID, void* p_work, int size )
{
	WIFI_BATTLEROOM* p_sys = p_work;
	return WBR_ClientCommDataBuffPtrGet( p_sys->p_client, netID );
}
 

//----------------------------------------------------------------------------
/**
 *	@brief	�T�C�Y0��Ԃ�
 */
//-----------------------------------------------------------------------------
int WBR_CNM_ZeroSizeGet( void )
{
	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@����̋��ʃf�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
int WBR_CNM_KoCommonSizeGet( void )
{
	return sizeof(WBR_COMM_COMMON);
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@����̋��ʃf�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
int WBR_CNM_OyaCommonSizeGet( void )
{
	return sizeof(WBR_COMM_COMMON) * WBR_COMM_NUM;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@����̃Q�[���f�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
int WBR_CNM_KoGameDataSizeGet( void )
{
	return sizeof( WBR_KO_COMM_GAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�e�@����̃Q�[���f�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
int WBR_CNM_OyaGameDataSizeGet( void )
{
	return sizeof( WBR_OYA_COMM_GAME );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q�@����̘b�������f�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
int WBR_CNM_KoTalkDataSizeGet( void )
{
	return sizeof( WBR_KO_COMM_TALK );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b���������N�G�X�g�f�[�^�T�C�Y�擾
 */
//-----------------------------------------------------------------------------
int WBR_CNM_KoTalkReqSizeGet( void )
{
	return sizeof( WBR_TALK_REQ );
}
