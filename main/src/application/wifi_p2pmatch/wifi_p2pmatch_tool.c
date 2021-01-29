//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_p2pmatch_tool.c
 *	@brief		WiFi�N���u�ȊO�ŔC�V���F�؃T�[�o�ɃA�N�Z�X����Ƃ��ɁAWiFi�N���u��ŕ\������Ȃ��X�e�[�^�X��Ԃɂ���V�X�e��
 *	@author		tomoya takahashi
 *	@data		2008.03.28
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


#include "common.h"
#include "application/wifi_p2pmatch_tool.h"

#include "application/wifi_p2pmatch/wifi_p2pmatch_local.h"

#include  "wifi/dwc_rap.h"

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
 *	@brief	WiFi�N���u��ŕ\������Ȃ��悤�ɏ�Ԗ������T�[�o�ɑ��M
 *
 *	@param	p_mystatus		�����̏��
 *	@param	heapID			�q�[�vID
 */
//-----------------------------------------------------------------------------
_WIFI_MACH_STATUS* WifiP2PMatch_CleanStatus_Set( const MYSTATUS* cp_mystatus, u32 heapID )
{
	_WIFI_MACH_STATUS* p_status;
	BOOL result;

	p_status = sys_AllocMemory( heapID, _WIFI_STATUS_MAX_SIZE );
	memset( p_status, 0, _WIFI_STATUS_MAX_SIZE );
	p_status->status = WIFI_STATUS_PLAY_OTHER;	// �������Ă����Α��v

	p_status->sex			= MyStatus_GetMySex( cp_mystatus );
	p_status->trainer_view	= MyStatus_GetTrainerView( cp_mystatus );
    p_status->version		= MyStatus_GetRomCode( cp_mystatus );
    p_status->regionCode	= MyStatus_GetRegionCode( cp_mystatus );

	result = mydwc_setMyInfo( p_status, _WIFI_STATUS_MAX_SIZE );
	GF_ASSERT( result == TRUE );

	return p_status;
}

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi�N���u��ŕ\������Ȃ��悤�ɏ�ԂȂ��𑗐M�����o�b�t�@��j������
 *
 *	@param	p_buff	���[�N
 */
//-----------------------------------------------------------------------------
void WifiP2PMatch_CleanStatus_Delete( _WIFI_MACH_STATUS* p_buff )
{
	sys_FreeMemoryEz( p_buff );
}


