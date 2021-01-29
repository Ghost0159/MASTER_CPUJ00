//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_judge.c
 *	@brief		�R�}���h���f���W���[�������N�G�X�g�R�}���h�o�b�t�@
 *	@author		tomoya takahashi
 *	@data		2007.03.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#define __WF2DMAP_JUDGE_H_GLOBAL
#include "application/wifi_2dmap/wf2dmap_judge.h"

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
 *			�R�}���h�W���b�W���W���[��
 */
//-----------------------------------------------------------------------------
#define WF2DMAP_JUDGEWALLWALK_WAIT_FRAME	(8)


//-----------------------------------------------------------------------------
/**
 *			�v���g�^�C�v�錾
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_JUDGESysNoneJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysTurnJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysRunJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysBusyJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysWallWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysSlowWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );
static BOOL WF2DMAP_JUDGESysStayWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act );

static BOOL WF2DMAP_JUDGESysMapHitCheck( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_objsys, WF2DMAP_WAY way );
static BOOL WF2DMAP_JUDGESysObjHitCheck( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_OBJSYS* cp_objsys, WF2DMAP_WAY way );

static void WF2DMAP_JUDGESysACTCMDSet( WF2DMAP_ACTCMD* p_act, const WF2DMAP_OBJWK* cp_wk, WF2DMAP_CMD cmd, u32 playid, WF2DMAP_WAY way );

//----------------------------------------------------------------------------
/**
 *	@brief	�R�}���h���s���f����
 *
 *	@param	cp_mapsys		�}�b�v�f�[�^�Ǘ��V�X�e��
 *	@param	cp_objsys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	cp_req			���N�G�X�g�R�}���h�f�[�^
 *	@param	p_act			���s�R�}���h�i�[��
 *
 *	@retval	TRUE	�R�}���h���s�\
 *	@retval	FALSE	�R�}���h���s�s�\
 *
 *	�R�}���h���s�\����p_act�Ɏ��s�R�}���h���i�[����܂�
 */
//-----------------------------------------------------------------------------
BOOL WF2DMAP_JUDGESysCmdJudge( const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	const WF2DMAP_OBJWK* cp_wk;
	static BOOL (* const pJudge[ WF2DMAP_CMD_NUM ])( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act ) = {
		WF2DMAP_JUDGESysNoneJudge,
		WF2DMAP_JUDGESysTurnJudge,
		WF2DMAP_JUDGESysWalkJudge,
		WF2DMAP_JUDGESysRunJudge,
		WF2DMAP_JUDGESysBusyJudge,
		WF2DMAP_JUDGESysWallWalkJudge,
		WF2DMAP_JUDGESysSlowWalkJudge,
		WF2DMAP_JUDGESysWalkJudge,
		WF2DMAP_JUDGESysStayWalkJudge,
		WF2DMAP_JUDGESysStayWalkJudge,
		WF2DMAP_JUDGESysStayWalkJudge,
		WF2DMAP_JUDGESysStayWalkJudge,
	};
	BOOL result;

	GF_ASSERT( cp_mapsys );
	GF_ASSERT( cp_objsys );
	GF_ASSERT( cp_req );
	GF_ASSERT( p_act );

	// playID�̃��[�N���擾
	cp_wk = WF2DMAP_OBJWkConstGet( cp_objsys, cp_req->playid );
	GF_ASSERT(cp_wk);

	// ����J�n���Ă悢���`�F�b�N
	result = pJudge[ cp_req->cmd ]( cp_wk, cp_mapsys, cp_objsys, cp_req, p_act );
	return result;
}  


//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�R�}���h�����s���Ă悢���`�F�b�N����֐��Q
 *
 *	@param	cp_wk			���[�N
 *	@param	cp_mapsys		�}�b�v�f�[�^�Ǘ��V�X�e��
 *	@param	cp_objsys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	cp_req			���N�G�X�g�R�}���h
 *
 *	@retval	TRUE	���s���Ă悢
 *	@retval	FALSE	���s���Ă͂����Ȃ�
 */
//-----------------------------------------------------------------------------
// WF2DMAP_CMD_NONE
static BOOL WF2DMAP_JUDGESysNoneJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	s32 status;

	status = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_ST );
	
	if( (status == WF2DMAP_OBJST_NONE) || (status == WF2DMAP_OBJST_BUSY) ){

		// ���삵�đ��v�Ȃ�A�N�V�����R�}���h�쐬
		WF2DMAP_JUDGESysACTCMDSet( p_act, cp_wk, cp_req->cmd, cp_req->playid, cp_req->way );
		return TRUE;
	}
	return FALSE;
}

// WF2DMAP_CMD_TURN
static BOOL WF2DMAP_JUDGESysTurnJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	s32 status;
	s32 way;

	status = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_ST );
	way = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_WAY );
	
	if( (status == WF2DMAP_OBJST_NONE) ){
		if( way != cp_req->way ){
			// ���삵�đ��v�Ȃ�A�N�V�����R�}���h�쐬
			WF2DMAP_JUDGESysACTCMDSet( p_act, cp_wk, cp_req->cmd, cp_req->playid, cp_req->way );
			return TRUE;
		}
	}
	return FALSE;
	
}

// WF2DMAP_CMD_WALK
static BOOL WF2DMAP_JUDGESysWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	s32 status;
	BOOL hit;
	s32 st_frame;

	do{
		status = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_ST );
	
		// �ҋ@��ԂłȂ��Ă͂Ȃ�Ȃ�
		if( (status != WF2DMAP_OBJST_NONE) ){
			break;
		}

		// �}�b�v�Ƃ̓����蔻��
		hit = WF2DMAP_JUDGESysMapHitCheck( cp_wk, cp_mapsys, cp_req->way );

		// �����蔻��̂Ȃ��}�b�v�łȂ��Ƃ����Ȃ�
		if( hit == TRUE ){
			// �����蔻�肠��̂Ői�߂Ȃ�
			break;
		}

		// ���̐l���I�u�W�F�N�g�Ƃ��Ԃ��Ă͂����Ȃ�
		hit = WF2DMAP_JUDGESysObjHitCheck( cp_wk, cp_objsys, cp_req->way );
		// �����蔻��̂Ȃ��}�b�v�łȂ��Ƃ����Ȃ�
		if( hit == TRUE ){
			// �����蔻�肠��̂Ői�߂Ȃ�
			break;
		}
		
		// �����܂ŉ������Ȃ���΂��邢�Ăn�j
		WF2DMAP_JUDGESysACTCMDSet( p_act, cp_wk, cp_req->cmd, cp_req->playid, cp_req->way );
		return TRUE;

	}while(0);
	
	
	// �ҋ@��Ԃ�WF2DMAP_JUDGEWALLWALK_WAIT_FRAME�t���[���ȏソ������Ǖ���
	if( (status == WF2DMAP_OBJST_NONE) ){
		st_frame = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_STFRAME );
//		TOMOYA_PRINT( "st_frame %d\n", st_frame );
		if( st_frame >= WF2DMAP_JUDGEWALLWALK_WAIT_FRAME ){
			// �_�~�[���[�v���甲���Ă����ꍇ�͕������Ƃ��ł��Ȃ�
			WF2DMAP_JUDGESysACTCMDSet( p_act, cp_wk, WF2DMAP_CMD_WALLWALK, cp_req->playid, cp_req->way );
			return TRUE;
		}
	}
	return FALSE;
}

// WF2DMAP_CMD_RUN
static BOOL WF2DMAP_JUDGESysRunJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	// �����ƈꏏ
	return WF2DMAP_JUDGESysWalkJudge( cp_wk, cp_mapsys, cp_objsys, cp_req, p_act );
	
}

// WF2DMAP_CMD_BUSY
static BOOL WF2DMAP_JUDGESysBusyJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	s32 status;

	status = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_ST );
	
	if( (status == WF2DMAP_OBJST_NONE) ){
		WF2DMAP_JUDGESysACTCMDSet( p_act, cp_wk, cp_req->cmd, cp_req->playid, cp_req->way );
		return TRUE;
	}
	return FALSE;
}

// WF2DMAP_CMD_WALLWALK
static BOOL WF2DMAP_JUDGESysWallWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	GF_ASSERT( 0 );	// WALLWALK�̓W���b�W���܂���
	return FALSE;
}

// WF2DMAP_CMD_SLOWWALK
static BOOL WF2DMAP_JUDGESysSlowWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	// �����ƈꏏ
	return WF2DMAP_JUDGESysWalkJudge( cp_wk, cp_mapsys, cp_objsys, cp_req, p_act );
}

// WF2DMAP_CMD_STAYWALK2
static BOOL WF2DMAP_JUDGESysStayWalkJudge( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, const WF2DMAP_OBJSYS* cp_objsys, const WF2DMAP_REQCMD* cp_req, WF2DMAP_ACTCMD* p_act )
{
	s32 status;

	status = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_ST );
	
	// �����Ȃ���ԂȂ狖��
	if( (status == WF2DMAP_OBJST_NONE) ){
		WF2DMAP_JUDGESysACTCMDSet( p_act, cp_wk, cp_req->cmd, cp_req->playid, cp_req->way );
		return TRUE;
	}
	return FALSE;
}







//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�Ƃ̓����蔻��
 *
 *	@param	cp_wk			�I�u�W�F�N�g�f�[�^
 *	@param	cp_mapsys		�}�b�v�f�[�^�Ǘ��V�X�e��
 *	@param	way				�i�ޕ���
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	������Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_JUDGESysMapHitCheck( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_MAPSYS* cp_mapsys, WF2DMAP_WAY way )
{
	WF2DMAP_POS pos;

	// �i�񂾐�̍��W���擾
	pos = WF2DMAP_OBJWkMatrixGet( cp_wk );
	pos = WF2DMAP_OBJToolWayPosGet( pos, way );

	// �i�񂾐�̃}�b�v�f�[�^�擾
	return WF2DMAP_MAPSysHitGet( cp_mapsys,
			WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�Ƃ̓����蔻��
 *
 *	@param	cp_wk			�I�u�W�F�N�g�f�[�^
 *	@param	cp_objsys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	way				�i�ޕ���
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	������Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_JUDGESysObjHitCheck( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_OBJSYS* cp_objsys, WF2DMAP_WAY way )
{
	const WF2DMAP_OBJWK* cp_retwk;
	
	cp_retwk = WF2DMAP_OBJSysHitCheck( cp_wk, cp_objsys, way );
	if( cp_retwk == NULL ){
		return FALSE;
	}
	return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�V�����R�}���h�ݒ�
 *
 *	@param	p_act		�ݒ��A�N�V�����R�}���h
 *	@param	cp_wk		�I�u�W�F�N�g���[�N
 *	@param	cmd			�R�}���h
 *	@param	playid		�v���CID
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_JUDGESysACTCMDSet( WF2DMAP_ACTCMD* p_act, const WF2DMAP_OBJWK* cp_wk, WF2DMAP_CMD cmd, u32 playid, WF2DMAP_WAY way )
{
	p_act->pos = WF2DMAP_OBJWkMatrixGet( cp_wk );
	p_act->cmd = cmd;
	p_act->playid = playid;
	p_act->way = way;
}
