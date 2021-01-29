//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		scr_wifilobby.c
 *	@brief		WiFi���r�[�C�x���g
 *	@author		tomoya takahashi
 *	@data		2008.01.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include  "common.h"

#include "application/wifi_lobby.h"

#include "field/fieldsys.h"
#include "scr_wifilobby.h"

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
//-------------------------------------
///	�C�x���g���[�N
//=====================================
typedef struct {
	u16 seq;
	u16	check_skip;
	WFLBY_PROC_PARAM param;
} EV_WIFILOBBY_WK;


// �I�[�o�[���CID�錾
FS_EXTERN_OVERLAY(wifilobby_common);

// �}�Ӄv���b�N�f�[�^
static const PROC_DATA sc_PROC = {	
	WFLBYProc_Init,
	WFLBYProc_Main,
	WFLBYProc_Exit,
	FS_OVERLAY_ID(wifilobby_common)
};



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static BOOL GMEVENT_Sub_WiFiLobby(GMEVENT_CONTROL * event);

//----------------------------------------------------------------------------
/**
 *	@brief	WiFi�L��v���b�N�@�C�x���g�J�n
 *
 *	@param	event		�C�x���g���[�N
 *	@param	check_skip	�ŏ��̐ڑ��`�F�b�N���X�L�b�v���邩
 */
//-----------------------------------------------------------------------------
void EventCmd_WiFiLobbyProc( GMEVENT_CONTROL * event, BOOL check_skip )
{
	EV_WIFILOBBY_WK* p_evwk;

    p_evwk = sys_AllocMemoryLo( HEAPID_WORLD, sizeof(EV_WIFILOBBY_WK) );
	memset( p_evwk, 0, sizeof(EV_WIFILOBBY_WK) );
	p_evwk->check_skip = check_skip;
	FieldEvent_Call(event, GMEVENT_Sub_WiFiLobby, p_evwk);
}



//----------------------------------------------------------------------------
/**
 *	@brief	WiFi�L��	�v���b�N
 *
 *	@param	event		�C�x���g���[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
static BOOL GMEVENT_Sub_WiFiLobby( GMEVENT_CONTROL * event )
{
	FIELDSYS_WORK *		p_fsys = FieldEvent_GetFieldSysWork(event);
	EV_WIFILOBBY_WK*	p_evwk = FieldEvent_GetSpecialWork(event);

	// ����
	switch( p_evwk->seq ){
	case 0:
		{
			p_evwk->param.p_save			= p_fsys->savedata;
			p_evwk->param.check_skip		= p_evwk->check_skip;
			p_evwk->param.p_wflby_counter	= &p_fsys->wflby_counter;
			EventCmd_CallSubProc(event, &sc_PROC, &p_evwk->param);
			p_evwk->seq ++;
		}
		break;

	case 1:
		sys_FreeMemoryEz( p_evwk );
		return TRUE;
	}
	return FALSE;
}
