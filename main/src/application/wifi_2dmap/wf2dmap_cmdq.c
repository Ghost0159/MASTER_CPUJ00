//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_cmdq.c
 *	@brief		�R�}���h�L���[
 *	@author		tomoya takahashi
 *	@data		2007.03.28
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#define __WF2DMAP_CMDQ_H_GLOBAL
#include "application/wifi_2dmap/wf2dmap_cmdq.h"

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
 *		�A�N�V�����R�}���h�o�b�t�@
 *		�i��������֗��Ȃ̂ō쐬���܂����j
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�A�N�V�����R�}���h�o�b�t�@
//=====================================
typedef struct _WF2DMAP_ACTCMDQ {
	WF2DMAP_ACTCMD* p_buff;
	u32 num;
	u16 top;		// �擪
	u16	tail;		// ����
}WF2DMAP_ACTCMDQ;



//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�V�����R�}���h�o�b�t�@���[�N�쐬
 *
 *	@param	buffnum		�R�}���h�z��
 *	@param	heapID		�q�[�vID
 *
 *	@return	�A�N�V�����R�}���h�o�b�t�@���[�N
 */
//-----------------------------------------------------------------------------
WF2DMAP_ACTCMDQ* WF2DMAP_ACTCMDQSysInit( u32 buffnum, u32 heapID )
{
	WF2DMAP_ACTCMDQ* p_sys;

	p_sys = sys_AllocMemory( heapID, sizeof(WF2DMAP_ACTCMDQ) );
	GF_ASSERT( p_sys );

	p_sys->num = buffnum + 1;	// �L���[�������ς��ɂȂ������ƃ`�F�b�N�p
	
	p_sys->p_buff = sys_AllocMemory( heapID, sizeof(WF2DMAP_ACTCMD)*p_sys->num );
	memset( p_sys->p_buff, 0, sizeof(WF2DMAP_ACTCMD)*p_sys->num );

	p_sys->top = 0;
	p_sys->tail = 0;

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�b�t�@���[�N�̃N���A
 *
 *	@param	p_sys		�A�N�V�����R�}���h�L���[���[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_ACTCMDQSysExit( WF2DMAP_ACTCMDQ* p_sys )
{
	sys_FreeMemoryEz( p_sys->p_buff );
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�b�t�@���̎擾
 *
 *	@param	cp_sys		�A�N�V�����R�}���h�L���[���[�N
 *
 *	@return	�o�b�t�@��
 */
//-----------------------------------------------------------------------------
u32 WF2DMAP_ACTCMDQSysBuffNumGet( const WF2DMAP_ACTCMDQ* cp_sys )
{
	return cp_sys->num - 1;	// �L���[�������ς��ɂȂ������ƃ`�F�b�N���邽��num+1���Ă���̂�-1����
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�V�����R�}���h�L���[�@�R�}���h�ݒ�
 *
 *	@param	p_sys		�A�N�V�����R�}���h�L���[���[�N
 *	@param	cp_buff		�ݒ�R�}���h
 */
//-----------------------------------------------------------------------------
void WF2DMAP_ACTCMDQSysCmdPush( WF2DMAP_ACTCMDQ* p_sys, const WF2DMAP_ACTCMD* cp_buff )
{
	// ����+1���擪�̎����t
	if( ((p_sys->tail + 1)%p_sys->num) == p_sys->top ){
		WF2DMAP_ACTCMD tmp;	// �̂Ă�R�}���h
		// ���t
		// 1�f�[�^���|�b�v���ăf�[�^�i�[
		WF2DMAP_ACTCMDQSysCmdPop( p_sys, &tmp );
	}

	// �f�[�^�ݒ�
	p_sys->p_buff[ p_sys->tail ] = *cp_buff;

	// �����ʒu�𓮂���
	p_sys->tail = (p_sys->tail + 1)%p_sys->num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�V�����R�}���h�L���[�@�R�}���h�擾
 *
 *	@param	p_sys		�A�N�V�����R�}���h�L���[���[�N
 *	@param	p_cmd		�R�}���h�i�[��
 *
 *	@retval	TRUE	�擾����
 *	@retval	FALSE	�擾���s
 */
//-----------------------------------------------------------------------------
BOOL WF2DMAP_ACTCMDQSysCmdPop( WF2DMAP_ACTCMDQ* p_sys, WF2DMAP_ACTCMD* p_cmd )
{
	// �擪=����	�f�[�^���Ȃ�
	if( p_sys->tail == p_sys->top ){
		return FALSE;
	}

	*p_cmd = p_sys->p_buff[ p_sys->top ];

	// �擪�ʒu�𓮂���
	p_sys->top = (p_sys->top + 1)%p_sys->num;

	return TRUE;
}





//-----------------------------------------------------------------------------
/**
 *		���N�G�X�g�R�}���h�L���[
 *		�i��������֗��Ȃ̂ō쐬���܂����j
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
///	���N�G�X�g�R�}���h�L���[
//=====================================
typedef struct _WF2DMAP_REQCMDQ{
	WF2DMAP_REQCMD* p_buff;
	u32 num;
	u16 top;		// �擪
	u16	tail;		// ����
}WF2DMAP_REQCMDQ;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h�o�b�t�@���[�N�쐬
 *
 *	@param	buffnum		�R�}���h�z��
 *	@param	heapID		�q�[�vID
 *
 *	@return	���N�G�X�g�R�}���h�o�b�t�@���[�N
 */
//-----------------------------------------------------------------------------
WF2DMAP_REQCMDQ* WF2DMAP_REQCMDQSysInit( u32 buffnum, u32 heapID )
{
	WF2DMAP_REQCMDQ* p_sys;

	p_sys = sys_AllocMemory( heapID, sizeof(WF2DMAP_REQCMDQ) );
	GF_ASSERT( p_sys );

	p_sys->num = buffnum + 1;	// �L���[�������ς��ɂȂ������ƃ`�F�b�N�p
	
	p_sys->p_buff = sys_AllocMemory( heapID, sizeof(WF2DMAP_REQCMD)*p_sys->num );
	memset( p_sys->p_buff, 0, sizeof(WF2DMAP_REQCMD)*p_sys->num );

	p_sys->top = 0;
	p_sys->tail = 0;

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h�@�L���[�@�j��
 *
 *	@param	p_sys	���N�G�X�g�R�}���h�@�L���[���[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_REQCMDQSysExit( WF2DMAP_REQCMDQ* p_sys )
{
	sys_FreeMemoryEz( p_sys->p_buff );
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h�@�v�b�V��
 *
 *	@param	p_sys		���N�G�X�g�R�}���h�@�L���[���[�N
 *	@param	cp_cmd		�ݒ�R�}���h�f�[�^
 */
//-----------------------------------------------------------------------------
void WF2DMAP_REQCMDQSysCmdPush( WF2DMAP_REQCMDQ* p_sys, const WF2DMAP_REQCMD* cp_cmd )
{
	// ����+1���擪�̎����t
	if( ((p_sys->tail + 1)%p_sys->num) == p_sys->top ){
		WF2DMAP_REQCMD tmp;	// �̂Ă�R�}���h
		// ���t
		// 1�f�[�^���|�b�v���ăf�[�^�i�[
		WF2DMAP_REQCMDQSysCmdPop( p_sys, &tmp );
	}

	// �f�[�^�ݒ�
	p_sys->p_buff[ p_sys->tail ] = *cp_cmd;

	// �����ʒu�𓮂���
	p_sys->tail = (p_sys->tail + 1)%p_sys->num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���N�G�X�g�R�}���h�L���[�@�R�}���h�擾
 *
 *	@param	p_sys		���N�G�X�g�R�}���h�L���[���[�N
 *	@param	p_cmd		�R�}���h�i�[��
 *
 *	@retval	TRUE	�擾����
 *	@retval	FALSE	�擾���s
 */
//-----------------------------------------------------------------------------
BOOL WF2DMAP_REQCMDQSysCmdPop( WF2DMAP_REQCMDQ* p_sys, WF2DMAP_REQCMD* p_cmd )
{
	// �擪=����	�f�[�^���Ȃ�
	if( p_sys->tail == p_sys->top ){
		return FALSE;
	}

	*p_cmd = p_sys->p_buff[ p_sys->top ];

	// �擪�ʒu�𓮂���
	p_sys->top = (p_sys->top + 1)%p_sys->num;

	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�b�t�@���̎擾
 *
 *	@param	cp_sys		���N�G�X�g�R�}���h�L���[���[�N
 *
 *	@return	�o�b�t�@��
 */
//-----------------------------------------------------------------------------
u32 WF2DMAP_REQCMDQSysBuffNumGet( const WF2DMAP_REQCMDQ* cp_sys )
{
	return cp_sys->num - 1;	// �L���[�������ς��ɂȂ������ƃ`�F�b�N���邽��num+1���Ă���̂�-1����
}
