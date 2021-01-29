//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wbr_surver_cont.c
 *	@brief		�T�[�o�[�@�I�u�W�F�N�g�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.04.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "application/wifi_2dmap/wf2dmap_map.h"
#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_judge.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"

#define __WBR_SURVER_CONT_H_GLOBAL
#include  "wbr_surver_cont.h"

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
#define WBR_SURVERCONT_GRID_X		( 16 )		// �Ǘ��O���b�h��
#define WBR_SURVERCONT_GRID_Y		( 12 )		// �Ǘ��O���b�h��
#define WBR_SURVERCONT_GRID_BUFF	( WBR_SURVERCONT_GRID_X*WBR_SURVERCONT_GRID_Y )

//-------------------------------------
///	�}�b�v�p�����[�^
//=====================================
enum {
	WBR_MAPPM_NONE,
	WBR_MAPPM_EXIT,
	WBR_MAPPM_NUM
} ;
#define WBR_MAPDATA_NONE	(WF2DMAP_MAPDATA(0, WBR_MAPPM_NONE))
#define WBR_MAPDATA_EXIT	(WF2DMAP_MAPDATA(0, WBR_MAPPM_EXIT))

//-----------------------------------------------------------------------------
/**
 *					�}�b�v�f�[�^
 */
//-----------------------------------------------------------------------------
static const WF2DMAP_MAP	WBR_SurverContMapData[ WBR_SURVERCONT_GRID_BUFF ] = {
	// 0
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 1
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 2
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 3
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 4
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 5
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 6
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 7
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 8
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 9
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 10
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,

	// 11
	WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_EXIT, WBR_MAPDATA_EXIT,
	WBR_MAPDATA_EXIT, WBR_MAPDATA_EXIT, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE, WBR_MAPDATA_NONE,
};

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------


//-------------------------------------
///	�I�u�W�F�N�g�f�[�^
//=====================================
typedef struct {
	WF2DMAP_OBJWK* p_obj;
	WBR_TALK_DATA talk_data;
	WBR_TALK_REQ talk_req;
	BOOL talk_req_flag;
} WBR_SURVEROBJ;


//-------------------------------------
///	�T�[�o�[�I�u�W�F�N�g�Ǘ�
//=====================================
typedef struct _WBR_SURVERCONT {
	WF2DMAP_MAPSYS*		p_mapsys;
	WF2DMAP_OBJSYS*		p_objsys;
	WF2DMAP_REQCMDQ*	p_reqQ;
	WF2DMAP_ACTCMDQ*	p_actQ;

	WBR_SURVEROBJ* p_wk;
	u32 wknum;
} WBR_SURVERCONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WBR_SurverCont_ObjInit( WBR_SURVERCONT* p_sys, WBR_SURVEROBJ* p_wk, u32 aid );
static BOOL WBR_SurverCont_ExitCheck( const WBR_SURVERCONT* cp_sys );
static WF2DMAP_CMD WBR_SurverCont_OBJST2CMDGet( WF2DMAP_OBJST st );
static void WBR_SurverCont_TalkMain( WBR_SURVERCONT* p_sys );
static void WBR_SurverCont_ObjTalkStart( WBR_SURVERCONT* p_sys, WBR_SURVEROBJ* p_wk );
static void WBR_SurverCont_ObjTalkEnd( WBR_SURVERCONT* p_sys, WBR_SURVEROBJ* p_wk );

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[�I�u�W�F�N�g�Ǘ��V�X�e��
 *	
 *	@param	objnum		�I�u�W�F�N�g��
 *	@param	heapID		�q�[�vID
 *
 *	@return	�T�[�o�[�Ǘ����[�N
 */
//-----------------------------------------------------------------------------
WBR_SURVERCONT* WBR_SurverContInit( u32 objnum, u32 heapID )
{
	WBR_SURVERCONT* p_sys;
	int i;

	// �������m��
	p_sys = sys_AllocMemory( heapID, sizeof(WBR_SURVERCONT) );
	memset( p_sys, 0, sizeof(WBR_SURVERCONT) );

	{	// �I�u�W�F�N�g���[�N�쐬
		p_sys->wknum = objnum;
		p_sys->p_wk = sys_AllocMemory( heapID, sizeof(WBR_SURVEROBJ)*p_sys->wknum );
		memset( p_sys->p_wk, 0, sizeof(WBR_SURVEROBJ)*p_sys->wknum );
	}

	{	// �}�b�v�f�[�^�쐬
		p_sys->p_mapsys = WF2DMAP_MAPSysInit( WBR_SURVERCONT_GRID_X, WBR_SURVERCONT_GRID_Y, heapID );
		WF2DMAP_MAPSysDataSet( p_sys->p_mapsys, WBR_SurverContMapData );
	}

	{	// �I�u�W�F�N�g�f�[�^�쐬
		p_sys->p_objsys = WF2DMAP_OBJSysInit( p_sys->wknum, heapID );
	}

	{	// �R�}���hQ
		p_sys->p_reqQ = WF2DMAP_REQCMDQSysInit( WBR_CMDQ_BUFFNUM, heapID );
		p_sys->p_actQ = WF2DMAP_ACTCMDQSysInit( WBR_CMDQ_BUFFNUM, heapID );
	}

	{
		// �l�����̃I�u�W�F�N�g�쐬
		for( i=0; i<p_sys->wknum; i++ ){
			WBR_SurverCont_ObjInit( p_sys,  &p_sys->p_wk[i], i );
		}
	}

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ǘ��f�[�^�j��
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WBR_SurverContExit( WBR_SURVERCONT* p_sys )
{
	// �e�V�X�e�����[�N�j��
	WF2DMAP_MAPSysExit( p_sys->p_mapsys );
	WF2DMAP_OBJSysExit( p_sys->p_objsys );
	WF2DMAP_REQCMDQSysExit( p_sys->p_reqQ );
	WF2DMAP_ACTCMDQSysExit( p_sys->p_actQ );

	// �I�u�W�F�N�g���[�N�j��
	sys_FreeMemoryEz( p_sys->p_wk );

	// �V�X�e�����[�N�j��
	sys_FreeMemoryEz( p_sys );
}


//----------------------------------------------------------------------------
/**
 *	@brief	���쏈��
 *
 *	@param	p_sys		�V�X�e�����[�N
 *
 *	@retval	TRUE	�I��
 *	@retval	FALSE	�r��
 */
//-----------------------------------------------------------------------------
BOOL WBR_SurverContMain( WBR_SURVERCONT* p_sys )
{
	BOOL result;
	
	// �I�u�W�F�N�g����
	WF2DMAP_OBJSysMain( p_sys->p_objsys );

	// ��b�J�n�`�F�b�N
	WBR_SurverCont_TalkMain( p_sys );

	// �I���`�F�b�N
	result = WBR_SurverCont_ExitCheck( p_sys );
	if( result ){	// �I��
		return result;
	}
	
	// �W���b�W����
	{
		WF2DMAP_REQCMD	req;
		WF2DMAP_ACTCMD	act;
		while( WF2DMAP_REQCMDQSysCmdPop( p_sys->p_reqQ, &req ) == TRUE ){

			// �W���b�W
			result = WF2DMAP_JUDGESysCmdJudge( p_sys->p_mapsys, p_sys->p_objsys, &req, &act );
			if( result == TRUE ){
				// �I�u�W�F�ɃA�N�V�����R�}���h�𔽉f
				WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );

				// �A�N�V�����R�}���h�L���[�ɐݒ�
				WF2DMAP_ACTCMDQSysCmdPush( p_sys->p_actQ, &act );
			}
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏�Ԃ̃R�}���h���A�N�V�����R�}���hQ�ɐݒ肷��
 *
 *	@param	p_sys		�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
void WBR_SurverContNowStatusActCmdSet( WBR_SURVERCONT* p_sys )
{
	int i;
	WF2DMAP_ACTCMD	act;

	for( i=0; i<p_sys->wknum; i++ ){
		act.playid = WF2DMAP_OBJWkDataGet( p_sys->p_wk[i].p_obj, WF2DMAP_OBJPM_PLID );
		act.way = WF2DMAP_OBJWkDataGet( p_sys->p_wk[i].p_obj, WF2DMAP_OBJPM_WAY );
		act.cmd = WF2DMAP_OBJWkDataGet( p_sys->p_wk[i].p_obj, WF2DMAP_OBJPM_ST );
		act.cmd = WBR_SurverCont_OBJST2CMDGet( act.cmd );

		if( (act.cmd == WF2DMAP_CMD_WALK) || (act.cmd == WF2DMAP_CMD_RUN) ){
			act.pos = WF2DMAP_OBJWkLastMatrixGet( p_sys->p_wk[i].p_obj );
		}else{
			act.pos = WF2DMAP_OBJWkMatrixGet( p_sys->p_wk[i].p_obj );
		}
		
		// �A�N�V�����R�}���h�L���[�ɐݒ�
		WF2DMAP_ACTCMDQSysCmdPush( p_sys->p_actQ, &act );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�������N�G�X�g�R�}���h��ݒ�
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_req		���N�G�X�g�R�}���h���[�N
 */
//-----------------------------------------------------------------------------
void WBR_SurverContReqCmdSet( WBR_SURVERCONT* p_sys, const WF2DMAP_REQCMD* cp_req )
{
	WF2DMAP_REQCMDQSysCmdPush( p_sys->p_reqQ, cp_req );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�f�[�^��ݒ�
 *
 *	@param	p_sys		�V�X�e���f�[�^
 *	@param	aid			AID
 *	@param	cp_data		�b�f�[�^
 */
//-----------------------------------------------------------------------------
void WBR_SurverContTalkDataSet( WBR_SURVERCONT* p_sys, u32 aid, const WBR_TALK_DATA* cp_data )
{
	p_sys->p_wk[ aid ].talk_data = *cp_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b���������N�G�X�g�f�[�^���i�[
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	cp_data		�f�[�^
 */
//-----------------------------------------------------------------------------
void WBR_SurverContTalkReqDataSet( WBR_SURVERCONT* p_sys, const WBR_TALK_REQ* cp_data )
{
	p_sys->p_wk[ cp_data->netid ].talk_req = *cp_data;
	p_sys->p_wk[ cp_data->netid ].talk_req_flag = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���⑗�M�f�[�^���擾����
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_buff		���M�f�[�^�i�[��
 *
 *	@retval	TRUE	�f�[�^��������
 *	@retval	FALSE	�����f�[�^���Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WBR_SurverContSendDataPop( WBR_SURVERCONT* p_sys, WBR_OBJ_DATA* p_buff )
{
	BOOL result;

	// �A�N�V�����R�}���h���擾
	result = WF2DMAP_ACTCMDQSysCmdPop( p_sys->p_actQ, &p_buff->act );
	if( result ){
		// ��b�f�[�^���i�[	
		p_buff->talk_data = p_sys->p_wk[ p_buff->act.playid ].talk_data;

		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		�f�[�^�i�[�惏�[�N
 *	@param	aid			�ʐMID
 */
//-----------------------------------------------------------------------------
static void WBR_SurverCont_ObjInit( WBR_SURVERCONT* p_sys, WBR_SURVEROBJ* p_wk, u32 aid )
{
	static WF2DMAP_OBJDATA	data = {
		0, 0, 0,
		WF2DMAP_OBJST_NONE, WF2DMAP_WAY_UP,
		HERO,
	};
	static const u8 player_pos[2][2] = {
		{ WF2DMAP_GRID2POS(7), WF2DMAP_GRID2POS(10) },
		{ WF2DMAP_GRID2POS(8), WF2DMAP_GRID2POS(10) },
	};

	// �o�^���W��ݒ�
	data.x = player_pos[aid][0];
	data.y = player_pos[aid][1];
	data.playid = aid;

	// �I�u�W�F�N�g�f�[�^�o�^
	p_sys->p_wk[ aid ].p_obj = WF2DMAP_OBJWkNew( p_sys->p_objsys, &data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o���`�F�b�N
 *
 *	@param	cp_sys	�V�X�e�����[�N
 *
 *	@retval	TRUE	�o���ɗ����Ă���
 *	@retval	FALSE	�o���ɂ��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WBR_SurverCont_ExitCheck( const WBR_SURVERCONT* cp_sys )
{
	int i;
	u32 status;
	u32 way;
	WF2DMAP_POS pos;
	u32 param;
	u32 exit_count = 0;
	
	for( i=0; i<cp_sys->wknum; i++ ){
		status = WF2DMAP_OBJWkDataGet( cp_sys->p_wk[i].p_obj, WF2DMAP_OBJPM_ST );
		way = WF2DMAP_OBJWkDataGet( cp_sys->p_wk[i].p_obj, WF2DMAP_OBJPM_WAY );
		if( (status == WF2DMAP_OBJST_NONE) && (way == WF2DMAP_WAY_DOWN) ){
			pos = WF2DMAP_OBJWkMatrixGet( cp_sys->p_wk[i].p_obj );
			param = WF2DMAP_MAPSysParamGet( cp_sys->p_mapsys, 
					WF2DMAP_POS2GRID(pos.x), WF2DMAP_POS2GRID(pos.y) );

			if( param == WBR_MAPPM_EXIT ){
				exit_count ++;
			}
		}
	}
	// �݂�ȏo���ɂ��Ă�����I���
	if( exit_count >= cp_sys->wknum ){
		return TRUE;
	}
	return FALSE;
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g��Ԃ��R�}���h�ɕύX����
 *
 *	@param	st	�I�u�W�F�N�g���
 */
//-----------------------------------------------------------------------------
static WF2DMAP_CMD WBR_SurverCont_OBJST2CMDGet( WF2DMAP_OBJST st )
{
	static const u32 Cmd[ WF2DMAP_OBJST_NUM ] = {
		WF2DMAP_CMD_NONE,
		WF2DMAP_CMD_TURN,
		WF2DMAP_CMD_WALK,
		WF2DMAP_CMD_RUN,
		WF2DMAP_CMD_BUSY,
	};
	return Cmd[ st ];
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������`�F�b�N���C���֐�
 *
 *	@param	p_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
static void WBR_SurverCont_TalkMain( WBR_SURVERCONT* p_sys )
{
	int i;
	for( i=0; i<p_sys->wknum; i++ ){
	
		// ���N�G�X�g�����Ă��邩�`�F�b�N
		if( p_sys->p_wk[i].talk_req_flag == TRUE ){
			if( p_sys->p_wk[i].talk_req.flag == WBR_TALK_REQ_START ){
				WBR_SurverCont_ObjTalkStart( p_sys, &p_sys->p_wk[i] );
			}else{
				WBR_SurverCont_ObjTalkEnd( p_sys, &p_sys->p_wk[i] );
			}

			p_sys->p_wk[i].talk_req_flag = FALSE;
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������`�F�b�N����
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		�b�������`�F�b�N����I�u�W�F�N�g
 */
//-----------------------------------------------------------------------------
static void WBR_SurverCont_ObjTalkStart( WBR_SURVERCONT* p_sys, WBR_SURVEROBJ* p_wk )
{
	u32 status;
	u32 way;
	u32 playid;
	u32 to_status;
	u32 to_way;
	u32 to_playid;
	WF2DMAP_POS pos;
	WF2DMAP_POS to_pos;
	const WF2DMAP_OBJWK* cp_hitobj;
	WBR_SURVEROBJ* p_talkobj;
	WF2DMAP_ACTCMD	act;

	status = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_ST );
	way = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_WAY );
	playid = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_PLID );
	pos = WF2DMAP_OBJWkMatrixGet( p_wk->p_obj );

	// �����������~��Ԃ��`�F�b�N
	if( status != WF2DMAP_OBJST_NONE ){
		return ;
	}

	// �����̖ڂ̑O�ɐl�����邩�`�F�b�N
	cp_hitobj = WF2DMAP_OBJSysHitCheck( p_wk->p_obj, p_sys->p_objsys, way );	
	if( cp_hitobj == NULL ){
		return ;
	}

	// �b������f�[�^�擾
	to_status = WF2DMAP_OBJWkDataGet( cp_hitobj, WF2DMAP_OBJPM_ST );
	to_way = WF2DMAP_OBJWkDataGet( cp_hitobj, WF2DMAP_OBJPM_WAY );
	to_playid = WF2DMAP_OBJWkDataGet( cp_hitobj, WF2DMAP_OBJPM_PLID );
	to_pos = WF2DMAP_OBJWkMatrixGet( cp_hitobj );

	// �b�����肪�ҋ@��ԂŃi�C�g�b���Ȃ�
	if( to_status != WF2DMAP_OBJST_NONE ){
		return ;
	}

	// ���̐l�̃T�[�o�[���[�N���擾
	p_talkobj = &p_sys->p_wk[ to_playid ];

	// �Q�l�Ƃ��r�W�[��ԂɕύX
	// �b�f�[�^��ݒ肵�A�A�N�V�����R�}���h��ݒ�
	{
		// �b��������̐ݒ�
		act.cmd = WF2DMAP_CMD_BUSY;
		act.way = way;
		act.playid = playid;
		act.pos = pos;
		WF2DMAP_ACTCMDQSysCmdPush( p_sys->p_actQ, &act );
		WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );		
		p_wk->talk_data.netid = playid;
		p_wk->talk_data.talk_mode = WBR_TALK_OYA;
		p_wk->talk_data.talk_playid = to_playid;
		p_wk->talk_data.talk_seq = WBR_TALK_SEQ_START;
	}
	{
		// �b�����������ݒ�
		act.cmd = WF2DMAP_CMD_BUSY;
		act.way = WF2DMPA_OBJToolRetWayGet( way );
		act.playid = to_playid;
		act.pos = to_pos;
		WF2DMAP_ACTCMDQSysCmdPush( p_sys->p_actQ, &act );
		WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );		
		p_talkobj->talk_data.netid = to_playid;
		p_talkobj->talk_data.talk_mode = WBR_TALK_KO;
		p_talkobj->talk_data.talk_playid = playid;
		p_talkobj->talk_data.talk_seq = WBR_TALK_SEQ_START;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�b�������I������
 *
 *	@param	p_sys		�V�X�e�����[�N
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void WBR_SurverCont_ObjTalkEnd( WBR_SURVERCONT* p_sys, WBR_SURVEROBJ* p_wk )
{
	WBR_SURVEROBJ* p_talkobj;
	WF2DMAP_ACTCMD act;
	u32 way;
	u32 to_way;
	u32 playid;
	u32 to_playid;
	WF2DMAP_POS pos;
	WF2DMAP_POS to_pos;

	
	// �b�����������`�F�b�N
	if( p_wk->talk_data.talk_mode == WBR_TALK_NONE ){
		return ;
	}

	// �b�����������I�u�W�F�擾
	p_talkobj = &p_sys->p_wk[ p_wk->talk_data.talk_playid ];

	// �f�[�^�擾
	way = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_WAY );
	playid = WF2DMAP_OBJWkDataGet( p_wk->p_obj, WF2DMAP_OBJPM_PLID );
	pos = WF2DMAP_OBJWkMatrixGet( p_wk->p_obj );
	to_way = WF2DMAP_OBJWkDataGet( p_talkobj->p_obj, WF2DMAP_OBJPM_WAY );
	to_playid = WF2DMAP_OBJWkDataGet( p_talkobj->p_obj, WF2DMAP_OBJPM_PLID );
	to_pos = WF2DMAP_OBJWkMatrixGet( p_talkobj->p_obj );

	// �b�������I��
	{
		// �b�������e
		act.cmd = WF2DMAP_CMD_NONE;
		act.way = way;
		act.playid = playid;
		act.pos = pos;
		WF2DMAP_ACTCMDQSysCmdPush( p_sys->p_actQ, &act );
		WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );
		memset( &p_wk->talk_data, 0, sizeof(WBR_TALK_DATA) );
	}
	{
		// �b�������q
		act.cmd = WF2DMAP_CMD_NONE;
		act.way = to_way;
		act.playid = to_playid;
		act.pos = to_pos;
		WF2DMAP_ACTCMDQSysCmdPush( p_sys->p_actQ, &act );
		WF2DMAP_OBJSysCmdSet( p_sys->p_objsys, &act );
		memset( &p_talkobj->talk_data, 0, sizeof(WBR_TALK_DATA) );
	}
}
