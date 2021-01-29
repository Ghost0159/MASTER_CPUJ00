//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_client.c
 *	@brief		�N���C�A���g�V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.02.22
 *
 *	�������`��PROC��
 *	���̑�PROC�ɕ��򂷂鏈�����Ǘ�
 * 
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "wbr_client_local.h"
#include "system/procsys.h"
#include "wbr_client_cont.h"
#include "field/fieldsys.h"
#include "field/tr_card_setup.h"
#include "field/field_subproc.h"

#define __WBR_CLIENT_H_GLOBAL
#include "wbr_client.h"

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
///	�N���C�A���g���[�N
//=====================================
typedef struct _WBR_CLIENT{
	u32 seq;
	WBR_CLIENT_COMMON common_data;

	// �N���C�A���g���ʃf�[�^
	WBR_COMM_COMMON comm_data;
}WBR_CLIENT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WBR_ClientCommDataInit( WBR_CLIENT* p_sys, WBR_COMM_COMMON* p_common_data );


//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g�V�X�e��������
 *	
 *	@param	p_init		�������f�[�^
 *	@param	heapID		�q�[�v
 *
 *	@return	�N���C�A���g���[�N
 */
//-----------------------------------------------------------------------------
WBR_CLIENT* WBR_ClientInit( const WBR_CLIENT_INIT* p_init, u32 heapID )
{
	WBR_CLIENT* p_sys;

	// ���[�N�쐬
	p_sys = sys_AllocMemory( heapID, sizeof( WBR_CLIENT ) );
	memset( p_sys, 0, sizeof(WBR_CLIENT) );
	
	// �������f�[�^�ݒ�
	p_sys->common_data.netid = p_init->netid;
	p_sys->common_data.cp_mystatus = p_init->cp_mystatus;
	p_sys->common_data.p_fsys = p_init->p_fsys;
	p_sys->common_data.heapID = heapID;

	return p_sys;
} 

//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g�V�X�e���j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WBR_ClientExit( WBR_CLIENT* p_sys )
{
	GF_ASSERT( p_sys->seq == WBR_CLIENT_SEQ_END );	// �I����ԂɂȂ��Ă��邩�`�F�b�N
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C���֐�
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�ʏ�
 */
//-----------------------------------------------------------------------------
BOOL WBR_ClientMain( WBR_CLIENT* p_sys )
{
	switch( p_sys->seq ){
	case WBR_CLIENT_SEQ_START_WAIT:		// �e����̊J�n���ߑ҂�
		break;

	case WBR_CLIENT_SEQ_COMMDATA_SEND:	// ���ʃf�[�^���M

		// ���ʃf�[�^�쐬
		WBR_ClientCommDataInit( p_sys, &p_sys->comm_data );
		CommSendHugeData( CNM_WBR_KO_COMMON, &p_sys->comm_data, sizeof(WBR_COMM_COMMON) );
		p_sys->seq ++;
		break;

	case WBR_CLIENT_SEQ_COMMDATA_WAIT:	// ���ʃf�[�^��M�҂�
		break;
			
	case WBR_CLIENT_SEQ_WALK_START:
		{
			static const PROC_DATA proc_data = {
				WBR_ClientCont_Init,
				WBR_ClientCont_Main,
				WBR_ClientCont_End,
				NO_OVERLAY_ID
			};
			// �v���Z�X�J�n
			GameSystem_StartSubProc( p_sys->common_data.p_fsys, &proc_data, &p_sys->common_data );

			p_sys->seq ++;
		}
		break;
		
	case WBR_CLIENT_SEQ_WALK_ENDWAIT:
		if( GameSystem_CheckSubProcExists( p_sys->common_data.p_fsys ) == FALSE ){

			// ����������̂��`�F�b�N
			switch( p_sys->common_data.next_seq ){
			case WBR_CLIENTCONT_RET_END:
				// �I����
				p_sys->seq = WBR_CLIENT_SEQ_END;	
				break;

			case WBR_CLIENTCONT_RET_TRCARD:
				// �g���[�i�[�J�[�h��
				p_sys->seq = WBR_CLIENT_SEQ_TRCARD_START;
				break;
			default:
				GF_ASSERT( 0 );
				break;
			}
		}
		break;

	case WBR_CLIENT_SEQ_TRCARD_START:
		FieldTrCard_SetProc( p_sys->common_data.p_fsys, 
				&p_sys->common_data.comm_data[ p_sys->common_data.link_netid ].tr_card );
		p_sys->seq++;
		break;
		
	case WBR_CLIENT_SEQ_TRCARD_ENDWAIT:
		if( GameSystem_CheckSubProcExists( p_sys->common_data.p_fsys ) == FALSE ){
			// �������\���ɖ߂�
			p_sys->seq = WBR_CLIENT_SEQ_WALK_START;
		}
		break;
		
	case WBR_CLIENT_SEQ_END:
		return TRUE;
	
	default:
		GF_ASSERT(0);
		break;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�P���X��ݒ肷��
 *
 *	@param	p_sys	�V�X�e�����[�N
 *	@param	seq		�V�[�P���X
 */
//-----------------------------------------------------------------------------
void WBR_ClientSeqSet( WBR_CLIENT* p_sys, u32 seq )
{
	p_sys->seq = seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���ʃf�[�^�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_data		���ʃf�[�^
 */
//-----------------------------------------------------------------------------
void WBR_ClientCommDataSet( WBR_CLIENT* p_sys, const WBR_COMM_COMMON* cp_data )
{
	memcpy( p_sys->common_data.comm_data, cp_data, sizeof(WBR_COMM_COMMON)*WBR_COMM_NUM );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���f�[�^�ݒ�
 *		
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_data		���ʃf�[�^
 */
//-----------------------------------------------------------------------------
void WBR_ClientGameDataSet( WBR_CLIENT* p_sys, const WBR_OYA_COMM_GAME* cp_data )
{
	WBR_ClientCont_SetOyaGameData( p_sys->common_data.p_procw, cp_data );
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������f�[�^�擾
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_data		�b�������f�[�^
 */
//-----------------------------------------------------------------------------
void WBR_ClientTalkDataSet( WBR_CLIENT* p_sys, const WBR_KO_COMM_TALK* cp_data )
{
	if( p_sys->common_data.p_procw != NULL ){
		WBR_ClientCont_SetTalkData( p_sys->common_data.p_procw, cp_data );
	}
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���I���f�[�^�ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WBR_ClientGameEndSet( WBR_CLIENT* p_sys )
{
	WBR_ClientCont_SetEndFlag( p_sys->common_data.p_procw );
}	

//----------------------------------------------------------------------------
/**
 *	@brief	���ʃf�[�^�i�[��o�b�t�@��Ԃ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	netid		�l�b�gID
 *
 *	@return	�o�b�t�@�|�C���^
 */
//-----------------------------------------------------------------------------
u8* WBR_ClientCommDataBuffPtrGet( WBR_CLIENT* p_sys, u32 netid )
{
	return (u8*)&p_sys->common_data.comm_data[ netid ];
}



//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g�f�[�^�쐬
 *
 *	@param	p_sys			�V�X�e�����[�N
 *	@param	p_common_data	���ʃf�[�^�i�[��
 */
//-----------------------------------------------------------------------------
static void WBR_ClientCommDataInit( WBR_CLIENT* p_sys, WBR_COMM_COMMON* p_common_data )
{
	TRCSET_MakeTrainerInfo( BADGE_BRUSH_OFF, TIME_UPDATE_OFF, GYM_READER_MASK_NONE, UNION_TR_NONE,
							p_sys->common_data.p_fsys,
							&p_common_data->tr_card );

}
