//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_surver.c
 *	@brief		�T�[�o�[�V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.02.21
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]


#include "wbr_surver_cont.h"
#define __WBR_SURVER_H_GLOBAL
#include "wbr_surver.h"

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
#define WBR_SURVER_KO_COMMON_RECV_OK_MSK	(0x3)	// �ʐM�����}�X�N

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�T�[�o�[���[�N
//=====================================
typedef struct _WBR_SURVER{
	u32 seq;	// �V�[�P���X
	
	// ��M�p���[�N
	WBR_KO_COMM_GAME	game_ko[ WBR_COMM_NUM ];

	// ���ʏ�����M�}�X�N
	u32	common_data_msk;

	// �I�u�W�F�N�g�Ǘ��V�X�e��
	WBR_SURVERCONT*	p_survercont;
}WBR_SURVER;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WBR_SurverDataSendExe( WBR_SURVER* p_sys );

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[�V�X�e��������
 *
 *	@param	heap	�q�[�v
 *
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WBR_SURVER* WBR_SurverInit( u32 heap )
{
	WBR_SURVER* p_sys;

	// �V�X�e�����[�N�쐬
	p_sys = sys_AllocMemory( heap, sizeof(WBR_SURVER) );
	memset( p_sys, 0, sizeof(WBR_SURVER) );
	
	// �I�u�W�F�N�g�Ǘ��V�X�e���쐬
	{
		p_sys->p_survercont = WBR_SurverContInit( WBR_COMM_NUM, heap );
	}

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[�V�X�e���j��
 *
 *	@param	p_sys	�T�[�o�[�V�X�e��
 */
//-----------------------------------------------------------------------------
void WBR_SurverExit( WBR_SURVER* p_sys )
{
	WBR_SurverContExit( p_sys->p_survercont );
	sys_FreeMemoryEz( p_sys );
} 

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[���C��
 *
 *	@param	p_sys	�T�[�o�[�V�X�e��
 */
//-----------------------------------------------------------------------------
void WBR_SurverMain( WBR_SURVER* p_sys )
{
	BOOL result;
	
	switch( p_sys->seq ){
	case WBR_SURVER_SEQ_START_SEND:
		// �ʐM�J�n�𑗐M
		CommSendData( CNM_WBR_GAME_SYS_START, NULL, 0 );
		
		p_sys->seq ++;
		break;
		
	case WBR_SURVER_SEQ_GETCOMMON_DATA:
		// �݂�Ȃɋ��ʃf�[�^�������킽��̂�҂�
		break;

	case WBR_SURVER_SEQ_MAIN_START:	// �J�n����
		CommSendData( CNM_WBR_GAME_SYS_GAMESTART, NULL, 0 );
		p_sys->seq++;
		break;
		
	case WBR_SURVER_SEQ_MAIN:
		// �I�u�W�F�N�g�Ǘ�
		result = WBR_SurverContMain( p_sys->p_survercont );
		WBR_SurverDataSendExe( p_sys );	// �f�[�^���M
		if( result == TRUE ){
			p_sys->seq ++;
		}
		break;

	case WBR_SURVER_SEQ_END_SEND:
		// �ʐM�J�n�𑗐M
		CommSendData( CNM_WBR_GAME_SYS_END, NULL, 0 );
		p_sys->seq ++;
		break;

	case WBR_SURVER_SEQ_END_WAIT:
		break;

	default:
		GF_ASSERT(0);
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�P���X�ݒ�
 *	
 *	@param	p_sys	���[�N
 *	@param	seq		�V�[�P���X
 */
//-----------------------------------------------------------------------------
void WBR_SurverSeqSet( WBR_SURVER* p_sys, u32 seq )
{
	p_sys->seq = seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�V�[�P���X�擾
 *
 *	@param	cp_sys	���[�N
 *
 *	@return
 */
//-----------------------------------------------------------------------------
u32 WBR_SurverSeqGet( const WBR_SURVER* cp_sys )
{
	return cp_sys->seq;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[�����̎q�f�[�^��ݒ�
 *
 *	@param	p_sys		���[�N
 *	@param	cp_ko		�q�f�[�^
 *	@param	id			ID
 */
//-----------------------------------------------------------------------------
void WBR_SurverKoGameDataSet( WBR_SURVER* p_sys, const WBR_KO_COMM_GAME* cp_ko, u32 id )
{
	WBR_SurverContReqCmdSet( p_sys->p_survercont, &cp_ko->req );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������f�[�^���擾
 *
 *	@param	p_sys		���[�N
 *	@param	cp_ko		�q�f�[�^
 *	@param	id			ID
 */
//-----------------------------------------------------------------------------
void WBR_SurverKoTalkDataSet( WBR_SURVER* p_sys, const WBR_KO_COMM_TALK* cp_ko, u32 id )
{
	WBR_SurverContTalkDataSet( p_sys->p_survercont, id, &cp_ko->talk_data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b���������N�G�X�g���擾
 *
 *	@param	p_sys		���[�N
 *	@param	cp_ko		�q�f�[�^
 *	@param	id			ID
 */
//-----------------------------------------------------------------------------
void WBR_SurverKoTalkReqSet( WBR_SURVER* p_sys, const WBR_TALK_REQ* cp_ko, u32 id )
{
	WBR_SurverContTalkReqDataSet( p_sys->p_survercont, cp_ko );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�q����̋��ʃf�[�^��M����
 *
 *	@param	p_sys		���[�N
 *	@param	id			ID
 *
 *	@retval	TRUE	�S�������M����
 *	@retval	FALSE	�S������܂���M��
 */
//-----------------------------------------------------------------------------
BOOL WBR_SurverKoCommDataRecv( WBR_SURVER* p_sys, u32 id )
{
	p_sys->common_data_msk |= 1<<id;

	if( p_sys->common_data_msk == WBR_SURVER_KO_COMMON_RECV_OK_MSK ){
		return TRUE;
	}
	return FALSE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[�̏�Ԃ�]��
 *
 *	@param	cp_sys	���[�N
 */
//-----------------------------------------------------------------------------
void WBR_SurverDataSend( WBR_SURVER* p_sys )
{
	WBR_SurverContNowStatusActCmdSet( p_sys->p_survercont );
}


//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
/**
 *	@brief	����f�[�^���M
 *
 *	@param	p_sys		���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_SurverDataSendExe( WBR_SURVER* p_sys )
{
	WBR_OBJ_DATA oya_data;

	while( WBR_SurverContSendDataPop( p_sys->p_survercont, &oya_data ) == TRUE ){
		CommSendData( CNM_WBR_OYA_GAMEDATA, &oya_data, sizeof(WBR_OBJ_DATA) );
	}
}
