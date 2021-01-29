//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_lobby.c
 *	@brief		WiFi���r�[���C���V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.09.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/pm_overlay.h"

#include "savedata/savedata_def.h"

#include "application/wifi_lobby.h"

#include "communication/communication.h"

#include "wifi/dwc_overlay.h"

#include "wflby_system.h"
#include "wflby_apl.h"

#include <ppwlobby/ppw_lobby.h>

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
///	WiFi���r�[���C�����[�N
//=====================================
typedef struct {
	SAVEDATA*		p_save;		// �Z�[�u�f�[�^
	WFLBY_SYSTEM*	p_commsys;	// WiFi���r�[���ʏ����V�X�e��
	WFLBY_APL*		p_apl;		// WiFi���r�[�A�v���Ǘ��V�X�e��
	TCB_PTR p_vtcb;				// VBLANKTCB
} WFLBY_WK;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_VBlankFunc( TCB_PTR p_tcb, void* p_work );


//----------------------------------------------------------------------------
/**
 *	@brief	����������
 *
 *	@param	p_proc		���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBYProc_Init( PROC* p_proc, int* p_seq )
{
	WFLBY_WK* p_wk;
	WFLBY_PROC_PARAM* p_param;

	{
		// �I�[�o�[���C
		FS_EXTERN_OVERLAY(wifi_2dmapsys);
		Overlay_Load( FS_OVERLAY_ID(wifi_2dmapsys), OVERLAY_LOAD_NOT_SYNCHRONIZE);

		// WiFi�I�[�o�[���C�J�n
		DwcOverlayStart();

		// ���E�����ighttp���C�u�����̂��߁j
		DpwCommonOverlayStart();
	}
	
	// �q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_WFLOBBY, 0x5000 );

	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(WFLBY_WK), HEAPID_WFLOBBY );
	memset( p_wk, 0, sizeof(WFLBY_WK) );

	// �p�����[�^���[�N�擾
	p_param = PROC_GetParentWork( p_proc );

	// �Z�[�u�f�[�^�擾
	p_wk->p_save = p_param->p_save;

	// ���ʏ����V�X�e���쐬
	p_wk->p_commsys = WFLBY_SYSTEM_Init( p_wk->p_save, HEAPID_WFLOBBY );
	p_wk->p_apl = WFLBY_APL_Init( p_param->check_skip, p_param->p_save, 
			p_param->p_wflby_counter, p_wk->p_commsys, HEAPID_WFLOBBY );

	// V�u�����NH�u�����N�֐��ݒ�
	sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~


	// �펞����VBLANK�^�X�N����
	p_wk->p_vtcb = VWaitTCB_Add( WFLBY_VBlankFunc, p_wk, 0 );

	// �v���b�N�J�n
	WFLBY_APL_Start( p_wk->p_apl );
	
	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C������
 *
 *	@param	p_proc		���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBYProc_Main( PROC* p_proc, int* p_seq )
{
	WFLBY_WK* p_wk;
	WFLBY_APL_RET apl_ret;

	p_wk = PROC_GetWork( p_proc );

	// �A�v�����C��
	apl_ret = WFLBY_APL_Main( p_wk->p_apl );

	// ���ʏ������C��
	// �ʒu�̓A�v���̂��ƌŒ�I�I�I�I
	WFLBY_SYSTEM_Main( p_wk->p_commsys );

	// �I���`�F�b�N
	if( apl_ret == WFLBY_APL_RET_END ){
		return PROC_RES_FINISH;
	}
	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�j������
 *
 *	@param	p_proc		���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT WFLBYProc_Exit( PROC* p_proc, int* p_seq )
{
	WFLBY_WK* p_wk;

	p_wk = PROC_GetWork( p_proc );

	// �펞����V�u�����NTCB�j��
	TCB_Delete( p_wk->p_vtcb );
	
	// V�u�����NH�u�����N�֐��ݒ�
	sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	// �A�v���V�X�e���j��
	WFLBY_APL_Exit( p_wk->p_apl );

	// ���ʏ����V�X�e���j��
	WFLBY_SYSTEM_Exit( p_wk->p_commsys );

	// ���[�N�j��
	PROC_FreeWork( p_proc );

	// �q�[�v�j��
	sys_DeleteHeap( HEAPID_WFLOBBY );

	// �QD�}�b�v�V�X�e�����I�[�o�[���C��j��
	{
		FS_EXTERN_OVERLAY(wifi_2dmapsys);
		Overlay_UnloadID( FS_OVERLAY_ID(wifi_2dmapsys) );

		// ���E�����ighttp���C�u�����̂��߁j
		DpwCommonOverlayEnd();

		// WiFi�I�[�o�[���C�I��
		DwcOverlayEnd();
	}

	return PROC_RES_FINISH;
}





//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 *
 *	@param	p_tcb		TCB�|�C���^
 *	@param	p_work		���[�N
 */
//-----------------------------------------------------------------------------
static void WFLBY_VBlankFunc( TCB_PTR p_tcb, void* p_work )
{
	WFLBY_WK* p_wk;
	p_wk = p_work;

	WFLBY_APL_VBlank( p_wk->p_apl );
	WFLBY_SYSTEM_VBlank( p_wk->p_commsys );
}

