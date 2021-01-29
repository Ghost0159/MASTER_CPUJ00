//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_battleroom.c
 *	@brief		wifi	�o�g�����[��
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "wifi_battleroom_local.h"
#include "system/pm_overlay.h"

#define __WIFI_BATTLEROOM_H_GLOBAL
#include "application/wifi_battleroom.h"

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
enum{
	WBRSYS_STATUS_START_SYNC,
	WBRSYS_STATUS_START_SYNCWAIT,
	WBRSYS_STATUS_MAIN,
	WBRSYS_STATUS_END_SYNC,
	WBRSYS_STATUS_END_SYNCWAIT,
	WBRSYS_STATUS_END,
	WBRSYS_STATUS_NUM,	// ��Ԃ̐�
};

#define WBRSYS_TCB_PRI	(32)	// TCB�D�揇��


// WIFI2DMAP�V�X�e���I�[�o�[���C
FS_EXTERN_OVERLAY(wifi_2dmapsys);

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
static void WBRSys_ContTcb( TCB_PTR tcb, void* p_work );	// �Ǘ�TCB


//----------------------------------------------------------------------------
/**
 *	@brief	wifi�o�g�����[��������
 *
 *	@param	cp_init		�������f�[�^
 *	@param	heapID		�q�[�vID
 *
 *	@return �Ǘ��V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WIFI_BATTLEROOM* WBRSys_Init( const WBR_INIT* cp_init, u32 heapID )
{
	WIFI_BATTLEROOM* p_wk;
	int i;
	WBR_CLIENT_INIT client_init;


	// wifi_2dmap�I�[�o�[���C�Ǎ���
	Overlay_Load( FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);
	
	// �q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WIFI_BATTLEROOM, 0x40000 );
	
	// ���[�N�쐬
	p_wk = sys_AllocMemory( HEAPID_WIFI_BATTLEROOM, sizeof(WIFI_BATTLEROOM) );
	memset( p_wk, 0, sizeof(WIFI_BATTLEROOM) );

	// FIELDSYS_WORK��ݒ�
	p_wk->p_fsys = cp_init->p_fsys;

	// �ʐM������
	CommCommandInitialize( Wbr_CommCommandTclGet(), CNM_COMMAND_MAX, p_wk );

	// �S������SAVEDATA���擾
	for( i=0; i<WBR_COMM_NUM; i++ ){
		p_wk->mystatus.cp_mystatus[ i ] = CommInfoGetMyStatus( i );
	}
	
	// �e�Ȃ�e�f�[�^������
	p_wk->netid = CommGetCurrentID();
	if( p_wk->netid == COMM_PARENT_ID ){
		p_wk->p_surver = WBR_SurverInit( heapID );
	}
	
	// �q�f�[�^������
	client_init.netid = p_wk->netid;
	client_init.cp_mystatus = &p_wk->mystatus;
	client_init.p_fsys = cp_init->p_fsys;
	p_wk->p_client = WBR_ClientInit( &client_init, HEAPID_WIFI_BATTLEROOM );
	
	// �Ǘ�TCB�o�^
	p_wk->tcb = TCB_Add( WBRSys_ContTcb, p_wk, WBRSYS_TCB_PRI );

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	wifi�o�g�����[���j��
 *		
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WBRSys_Exit( WIFI_BATTLEROOM* p_sys )
{
	GF_ASSERT( p_sys->status == WBRSYS_STATUS_END );

	// �^�X�N�j��
	TCB_Delete( p_sys->tcb );

	// �q�f�[�^�j��
	WBR_ClientExit( p_sys->p_client );
	
	// �e�f�[�^�j��
	if( p_sys->p_surver ){
		WBR_SurverExit( p_sys->p_surver );
		p_sys->p_surver = NULL;
	}

	// ���[�N�j��
	sys_FreeMemoryEz( p_sys );

	// �q�[�v�j��
	sys_DeleteHeap( HEAPID_WIFI_BATTLEROOM );


	// �I�[�o�[���C�j��
	Overlay_UnloadID( FS_OVERLAY_ID(wifi_2dmapsys) );
	
}

//----------------------------------------------------------------------------
/**
 *	@brief	wifi�o�g�����[���I���`�F�b�N
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WBRSys_EndCheck( const WIFI_BATTLEROOM* cp_sys )
{
	if( cp_sys->status == WBRSYS_STATUS_END ){
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�o�g�����[���Ǘ�TCB
 *
 *	@param	tcb			TCB�|�C���^
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WBRSys_ContTcb( TCB_PTR tcb, void* p_work )
{
	WIFI_BATTLEROOM* p_wk = p_work;
	BOOL result;

	switch( p_wk->status ){
	case WBRSYS_STATUS_START_SYNC:
		CommTimingSyncStart(WBR_COMM_SYNC_START);
		p_wk->status = WBRSYS_STATUS_START_SYNCWAIT;
		break;
		
	case WBRSYS_STATUS_START_SYNCWAIT:
		if(!CommIsTimingSync(WBR_COMM_SYNC_START)){
			break;
		}
		p_wk->status = WBRSYS_STATUS_MAIN;
		break;
		
	case WBRSYS_STATUS_MAIN:
		if(p_wk->p_surver){
			// �T�[�o���C��
			WBR_SurverMain( p_wk->p_surver );
		}

		// �N���C�A���g���C��
		result = WBR_ClientMain( p_wk->p_client );

		if( result == TRUE ){
			p_wk->status = WBRSYS_STATUS_END_SYNC;
		}
		break;
		
	case WBRSYS_STATUS_END_SYNC:
		CommTimingSyncStart(WBR_COMM_SYNC_END);
		p_wk->status = WBRSYS_STATUS_END_SYNCWAIT;
		break;
		
	case WBRSYS_STATUS_END_SYNCWAIT:
		if(!CommIsTimingSync(WBR_COMM_SYNC_END)){
			break;
		}
		p_wk->status = WBRSYS_STATUS_END;
		break;
		
	case WBRSYS_STATUS_END:
		break;
	}
}
