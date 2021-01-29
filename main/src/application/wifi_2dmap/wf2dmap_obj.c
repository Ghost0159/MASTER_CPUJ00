//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_obj.c
 *	@brief		�I�u�W�F�N�g�f�[�^�Ǘ�
 *	@author		tomoya takahashi
 *	@data		2007.03.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#define __WF2DMAP_OBJ_H_GLOBAL
#include "application/wifi_2dmap/wf2dmap_obj.h"

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
//#define DEBUG_STCHECK	// ����r������R�}���h���N�G�X�g����Ă��Ȃ����`�F�b�N����

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
///	�I�u�W�F�N�g���[�N
//=====================================
typedef struct _WF2DMAP_OBJWK {
	BOOL move;				// ����t���O
	WF2DMAP_POS pos;		// ���ݍ��W
	WF2DMAP_POS last_pos;	// �O���W
	u16 playid;				// �v���C���[ID
	u16 charaid;			// �L�����N�^ID
	u16 frame;				// ����t���[��
	u16 endframe;			// �I���t���[��
	u8	way;				// ����
	u8	status;				// ���
	u16 st_frame;			// ���̏�ԂɂȂ��Ă���t���[����
}WF2DMAP_OBJWK;

//-------------------------------------
///	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
//=====================================
typedef struct _WF2DMAP_OBJSYS {
	WF2DMAP_OBJWK* p_buff;
	u32 num;
}WF2DMAP_OBJSYS;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static WF2DMAP_OBJWK* WF2DMAP_OBJSysCleanWkGet( WF2DMAP_OBJSYS* p_sys );




static void WF2DMAP_OBJSysWkMoveInit( WF2DMAP_OBJWK* p_wk, const WF2DMAP_ACTCMD* cp_cmd );
static void WF2DMAP_OBJSysWkMoveMain( WF2DMAP_OBJWK* p_wk );


static void WF2DMAP_OBJSysWkNoneInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkTurnInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkWalkInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkRunInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkBusyInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkWallWalkInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkSlowWalkInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkWalk4Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkStayWalk2Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkStayWalk4Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkStayWalk8Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );
static void WF2DMAP_OBJSysWkStayWalk16Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way );

static BOOL WF2DMAP_OBJSysWkNoneMain( WF2DMAP_OBJWK* p_wk );
static BOOL WF2DMAP_OBJSysWkTurnMain( WF2DMAP_OBJWK* p_wk );
static BOOL WF2DMAP_OBJSysWkWalkMain( WF2DMAP_OBJWK* p_wk );
static BOOL WF2DMAP_OBJSysWkRunMain( WF2DMAP_OBJWK* p_wk );
static BOOL WF2DMAP_OBJSysWkBusyMain( WF2DMAP_OBJWK* p_wk );
static BOOL WF2DMAP_OBJSysWkWallWalkMain( WF2DMAP_OBJWK* p_wk );
static BOOL WF2DMAP_OBJSysWkStayWalk2Main( WF2DMAP_OBJWK* p_wk );

static void WF2DMAP_OBJSysWkMoveEnd( WF2DMAP_OBJWK* p_wk );

static WF2DMAP_OBJST WF2DMAP_OBJSysActionCmdLinkOBJStGet( WF2DMAP_CMD cmd );
static WF2DMAP_POS WF2DMAP_OBJSysWayPosGet( WF2DMAP_POS pos, WF2DMAP_WAY way );
static BOOL WF2DMAP_OBJSysFrameCont( WF2DMAP_OBJWK* p_wk );


//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��������
 *
 *	@param	objnum	�I�u�W�F�N�g��
 *	@param	heapID	�q�[�vID
 *
 *	@return	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJSYS* WF2DMAP_OBJSysInit( u32 objnum, u32 heapID )
{
	WF2DMAP_OBJSYS* p_sys;

	p_sys = sys_AllocMemory( heapID, sizeof(WF2DMAP_OBJSYS) );
	GF_ASSERT( p_sys );

	p_sys->num = objnum;

	p_sys->p_buff = sys_AllocMemory( heapID, sizeof(WF2DMAP_OBJWK)*p_sys->num );
	GF_ASSERT( p_sys->p_buff );
	memset( p_sys->p_buff, 0, sizeof(WF2DMAP_OBJWK)*p_sys->num );

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e���j��
 *
 *	@param	p_obj	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJSysExit( WF2DMAP_OBJSYS* p_sys )
{
	GF_ASSERT( p_sys );
	sys_FreeMemoryEz( p_sys->p_buff );
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�A�N�V�����R�}���h�̐ݒ�
 *
 *	@param	p_objsys	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	cp_cmd		�ݒ�A�N�V�����R�}���h
 * 
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJSysCmdSet( WF2DMAP_OBJSYS* p_objsys, const WF2DMAP_ACTCMD* cp_cmd )
{
	WF2DMAP_OBJWK* p_wk;
	
	// �R�}���h���s
	p_wk = WF2DMAP_OBJWkGet( p_objsys, cp_cmd->playid );
	WF2DMAP_OBJSysWkMoveInit( p_wk, cp_cmd );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�Ǘ�	�R�}���h���s
 *
 *	@param	p_objsys		�I�u�W�F�N�g�Ǘ����[�N
 *	
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJSysMain( WF2DMAP_OBJSYS* p_objsys )
{
	int i;
	WF2DMAP_OBJWK* p_wk;
	u32 objnum;

	objnum = WF2DMAP_OBJSysObjNumGet( p_objsys );
	
	// ������s
	for( i=0; i<objnum; i++ ){
		p_wk = WF2DMAP_OBJWkIdxGet( p_objsys, i );	//buff�C���f�b�N�X�Ń��[�N�擾
		if( p_wk ){
			WF2DMAP_OBJSysWkMoveMain( p_wk );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e���@�Ǘ��I�u�W�F�N�g���擾
 *
 *	@param	cp_sys	�I�u�W�F�N�g�Ǘ��V�X�e��
 *
 *	@return	�I�u�W�F�N�g�̐�
 */
//-----------------------------------------------------------------------------
u32 WF2DMAP_OBJSysObjNumGet( const WF2DMAP_OBJSYS* cp_sys )
{
	GF_ASSERT( cp_sys );
	return cp_sys->num;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g�f�[�^����
 *
 *	@param	p_obj		�I�u�W�F�N�g�Ǘ�
 *	@param	cp_data 
 *
 *	@return	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJWK* WF2DMAP_OBJWkNew( WF2DMAP_OBJSYS* p_sys, const WF2DMAP_OBJDATA* cp_data )
{
	WF2DMAP_OBJWK* p_wk;
	GF_ASSERT( p_sys );
	GF_ASSERT( cp_data );
	
	// �󂫂�T��
	p_wk = WF2DMAP_OBJSysCleanWkGet( p_sys );
	
	// �f�[�^�ݒ�
	p_wk->pos.x = cp_data->x;		// x���W
	p_wk->pos.y = cp_data->y;		// y���W
	p_wk->last_pos.x = cp_data->x;		// �O��x���W
	p_wk->last_pos.y = cp_data->y;		// �O��y���W
	p_wk->playid = cp_data->playid;	// �v���C���[ID
	p_wk->status = cp_data->status;	// ���
	p_wk->way = cp_data->way;		// ����
	p_wk->charaid = cp_data->charaid;// �L�����N�^�[

	// ����J�n
	p_wk->move = TRUE;

	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���[�N�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJWkDel( WF2DMAP_OBJWK* p_wk )
{
	memset( p_wk, 0, sizeof(WF2DMAP_OBJWK) );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���[�N�擾
 *
 *	@param	p_sys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	playid		�v���C���[ID
 *	
 *	@retval	�I�u�W�F�N�g���[�N
 *	@retval	NULL	���̃I�u�W�F�N�g�͂��Ȃ�
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJWK* WF2DMAP_OBJWkGet( WF2DMAP_OBJSYS* p_sys, u16 playid )
{
	int i;
	
	for( i=0; i<p_sys->num; i++ ){
		if( p_sys->p_buff[i].move == TRUE ){

			if( p_sys->p_buff[i].playid == playid ){
				return &p_sys->p_buff[i];
			}
		}
	}
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���[�N���C���f�b�N�X�Ŏ擾����
 *
 *	@param	p_sys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	�I�u�W�F�N�g���[�N
 *	pretval	NULL	���삿�イ�łȂ�
 */
//-----------------------------------------------------------------------------
WF2DMAP_OBJWK* WF2DMAP_OBJWkIdxGet( WF2DMAP_OBJSYS* p_sys, u16 idx )
{
	GF_ASSERT( p_sys );
	GF_ASSERT( idx < p_sys->num );

	if( p_sys->p_buff[ idx ].move == TRUE ){
		return &p_sys->p_buff[ idx ];
	}
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�R���X�g���[�N�̎擾
 *
 *	@param	cp_sys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	playid		�v���C���[ID
 *
 *	@return	�R���X�g���[�N
 */
//-----------------------------------------------------------------------------
const WF2DMAP_OBJWK* WF2DMAP_OBJWkConstGet( const WF2DMAP_OBJSYS* cp_sys, u16 playid )
{
	// ����Ă邱�Ƃ͈ꏏ�Ȃ̂ōė��p����
	return WF2DMAP_OBJWkGet( (WF2DMAP_OBJSYS*)cp_sys, playid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���[�N���C���f�b�N�X�Ŏ擾����
 *
 *	@param	p_sys		�I�u�W�F�N�g�f�[�^�Ǘ��V�X�e��
 *	@param	idx			�C���f�b�N�X
 *
 *	@retval	�I�u�W�F�N�g���[�N
 *	pretval	NULL	���삿�イ�łȂ�
 */
//-----------------------------------------------------------------------------
const WF2DMAP_OBJWK* WF2DMAP_OBJWkConstIdxGet( const WF2DMAP_OBJSYS* cp_sys, u16 idx )
{
	// ����Ă邱�Ƃ͈ꏏ�Ȃ̂ōė��p����
	return WF2DMAP_OBJWkIdxGet( (WF2DMAP_OBJSYS*)cp_sys, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�̃f�[�^�擾
 *
 *	@param	cp_wk		���[�N
 *	@param	id			�擾�����f�[�^�萔
 *
 *	@return	�w�肵���f�[�^
 */
//-----------------------------------------------------------------------------
s32 WF2DMAP_OBJWkDataGet( const WF2DMAP_OBJWK* cp_wk, WF2DMAP_OBJPARAM id )
{
	s32 data;

	GF_ASSERT( cp_wk );
	
	switch( id ){
	case WF2DMAP_OBJPM_X:		// x���W
		data = cp_wk->pos.x;
		break;
		
	case WF2DMAP_OBJPM_Y:		// y���W
		data = cp_wk->pos.y;
		break;
		
	case WF2DMAP_OBJPM_LX:		// �O��x���W
		data = cp_wk->last_pos.x;
		break;
		
	case WF2DMAP_OBJPM_LY:		// �O��y���W
		data = cp_wk->last_pos.y;
		break;
		
	case WF2DMAP_OBJPM_PLID:		// �v���C���[ID
		data = cp_wk->playid;
		break;
		
	case WF2DMAP_OBJPM_ST:		// ���
		data = cp_wk->status;
		break;
		
	case WF2DMAP_OBJPM_WAY:		// ����
		data = cp_wk->way;
		break;
		
	case WF2DMAP_OBJPM_CHARA:	// �L�����N�^�[
		data = cp_wk->charaid;
		break;
		
	case WF2DMAP_OBJPM_FRAME:	// ����t���[��
		data = cp_wk->frame;
		break;
		
	case WF2DMAP_OBJPM_ENDFRAME:	// �I������t���[��
		data = cp_wk->endframe;
		break;

	case WF2DMAP_OBJPM_STFRAME:
		data = cp_wk->st_frame;	// ���̏�ԂɂȂ��ĉ��t���[�������Ă��邩
		break;
		
	default:
		// ����Ȓ萔�Ȃ�
		GF_ASSERT(0);
		break;
	}
	return data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�̃f�[�^�擾
 *
 *	@param	p_wk	���[�N
 *	@param	id		�ݒ肷��f�[�^�萔
 *	@param	data	�f�[�^
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJWkDataSet( WF2DMAP_OBJWK* p_wk, WF2DMAP_OBJPARAM id, s32 data )
{
	GF_ASSERT( p_wk );
	switch( id ){
	case WF2DMAP_OBJPM_X:		// x���W
		p_wk->pos.x = data;
		break;
		
	case WF2DMAP_OBJPM_Y:		// y���W
		p_wk->pos.y = data;
		break;
		
	case WF2DMAP_OBJPM_LX:		// �O��x���W
		p_wk->last_pos.x = data;
		break;
		
	case WF2DMAP_OBJPM_LY:		// �O��y���W
		p_wk->last_pos.y = data;
		break;
		
	case WF2DMAP_OBJPM_PLID:		// �v���C���[ID
		p_wk->playid = data;
		break;
		
	case WF2DMAP_OBJPM_ST:		// ���
		GF_ASSERT( data < WF2DMAP_OBJST_NUM );
		p_wk->status = data;
		break;
		
	case WF2DMAP_OBJPM_WAY:		// ����
		GF_ASSERT( data < WF2DMAP_WAY_NUM );
		p_wk->way = data;
		break;
		
	case WF2DMAP_OBJPM_CHARA:	// �L�����N�^�[
		p_wk->charaid = data;
		break;
		
	case WF2DMAP_OBJPM_FRAME:	// ����t���[��
		p_wk->frame = data;
		break;
		
	case WF2DMAP_OBJPM_ENDFRAME:	// �I������t���[��
		p_wk->endframe = data;
		break;

	case WF2DMAP_OBJPM_STFRAME:	// ���̏�ԂɂȂ��ĉ��t���[�������Ă��邩
		p_wk->st_frame = data;
		break;
		
	default:
		// ����Ȓ萔�Ȃ�
		GF_ASSERT(0);
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���[�N�̃f�[�^���Z
 *
 *	@param	p_wk	���[�N
 *	@param	id		���Z����f�[�^�萔
 *	@param	data	���Z�f�[�^
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJWkDataAdd( WF2DMAP_OBJWK* p_wk, WF2DMAP_OBJPARAM id, s32 data )
{
	GF_ASSERT( p_wk );
	switch( id ){
	case WF2DMAP_OBJPM_X:		// x���W
		p_wk->pos.x += data;
		break;
		
	case WF2DMAP_OBJPM_Y:		// y���W
		p_wk->pos.y += data;
		break;
		
	case WF2DMAP_OBJPM_LX:		// �O��x���W
		p_wk->last_pos.x += data;
		break;
		
	case WF2DMAP_OBJPM_LY:		// �O��y���W
		p_wk->last_pos.y += data;
		break;
		
	case WF2DMAP_OBJPM_PLID:		// �v���C���[ID
		p_wk->playid += data;
		break;
		
	case WF2DMAP_OBJPM_ST:		// ���
		GF_ASSERT( data < WF2DMAP_OBJST_NUM );
		p_wk->status += data;
		break;
		
	case WF2DMAP_OBJPM_WAY:		// ����
		GF_ASSERT( data < WF2DMAP_WAY_NUM );
		p_wk->way += data;
		break;
		
	case WF2DMAP_OBJPM_CHARA:	// �L�����N�^�[
		p_wk->charaid += data;
		break;
		
	case WF2DMAP_OBJPM_FRAME:	// ����t���[��
		p_wk->frame += data;
		break;
		
	case WF2DMAP_OBJPM_ENDFRAME:	// �I������t���[��
		p_wk->endframe += data;
		break;
		
	default:
		// ����Ȓ萔�Ȃ�
		GF_ASSERT(0);
		break;
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	way�̕����ɂP�i�񂾍��W���擾����
 *
 *	@param	pos		���W
 *	@param	way		����
 *
 *	@return	way�̕����ɂP�i�񂾍��W
 */
//-----------------------------------------------------------------------------
WF2DMAP_POS WF2DMAP_OBJToolWayPosGet( WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	static const s8 add[WF2DMAP_WAY_NUM][2] = {
		{ 0, -WF2DMAP_GRID_SIZ },
		{ 0, WF2DMAP_GRID_SIZ },
		{ -WF2DMAP_GRID_SIZ, 0 },
		{ WF2DMAP_GRID_SIZ, 0 },
	};

	pos.x += add[way][0];
	pos.y += add[way][1];

	return pos;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����̋t���擾
 *
 *	@param	way		����
 *
 *	@return	way�̋t����
 */
//-----------------------------------------------------------------------------
WF2DMAP_WAY WF2DMPA_OBJToolRetWayGet( WF2DMAP_WAY way )
{
	static const u8 retway[ WF2DMAP_WAY_NUM ] = {
		WF2DMAP_WAY_DOWN,
		WF2DMAP_WAY_UP,
		WF2DMAP_WAY_RIGHT,
		WF2DMAP_WAY_LEFT,
	};
	return retway[ way ];
} 




//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/**
 *	@brief	��̃��[�N���擾����
 *
 *	@param	p_sys	�I�u�W�F�N�g�f�[�^�Ǘ����[�N
 *
 *	@return	��̃��[�N
 */
//-----------------------------------------------------------------------------
static WF2DMAP_OBJWK* WF2DMAP_OBJSysCleanWkGet( WF2DMAP_OBJSYS* p_sys )
{
	int i;

	for( i=0; i<p_sys->num; i++ ){
		if( p_sys->p_buff[i].move == FALSE ){
			return &p_sys->p_buff[i];
		}
	}
	// �󂫂Ȃ�
	GF_ASSERT(0);
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	���W
 */
//-----------------------------------------------------------------------------
WF2DMAP_POS WF2DMAP_OBJWkMatrixGet( const WF2DMAP_OBJWK* cp_wk )
{
	return cp_wk->pos;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���W�̎擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�O���W
 */
//-----------------------------------------------------------------------------
WF2DMAP_POS WF2DMAP_OBJWkLastMatrixGet( const WF2DMAP_OBJWK* cp_wk )
{
	return cp_wk->last_pos;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[���ɑΉ��������W���擾
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�t���[�����ɑΉ��������W
 */
//-----------------------------------------------------------------------------
WF2DMAP_POS WF2DMAP_OBJWkFrameMatrixGet( const WF2DMAP_OBJWK* cp_wk )
{
	s32 frame;
	s32 framemax;
	WF2DMAP_POS  pos, last_pos;
	
	frame = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_FRAME );
	framemax = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_ENDFRAME );

	pos = WF2DMAP_OBJWkMatrixGet( cp_wk );
	last_pos = WF2DMAP_OBJWkLastMatrixGet( cp_wk );

	// �����o��
	pos.x -= last_pos.x;
	pos.y -= last_pos.y;

	// �t���[�������獡�̍��W���o��
	if( frame > 0 ){
		pos.x = (frame * pos.x) / framemax;
		pos.y = (frame * pos.y) / framemax;
	}else{
		// 0�̂Ƃ��͌v�Z���s����Ȃ̂Ŏ�v�Z
		pos.x = 0;
		pos.y = 0;
	}

	// �O�̍��W�𑫂�
	pos.x += last_pos.x;
	pos.y += last_pos.y;

	return pos;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���W�̐ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	pos		���W
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJWkMatrixSet( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos )
{
	p_wk->pos = pos;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�O���W�̐ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	pos		�O���W
 */
//-----------------------------------------------------------------------------
void WF2DMAP_OBJWkLastMatrixSet( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos )
{
	p_wk->last_pos = pos;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�����蔻����s��
 *
 *	@param	cp_wk		���ׂ郏�[�N
 *	@param	cp_objsys	�I�u�W�F�N�g�V�X�e��
 *	@param	way			����
 *
 *	@retval	���������I�u�W�F�N�g
 *	@retval	NULL	������Ȃ�����
 */
//-----------------------------------------------------------------------------
const WF2DMAP_OBJWK* WF2DMAP_OBJSysHitCheck( const WF2DMAP_OBJWK* cp_wk, const WF2DMAP_OBJSYS* cp_objsys, WF2DMAP_WAY way )
{
	int i;
	u32 objnum;
	WF2DMAP_POS pos, ck_pos;
	u32 my_playid, ck_playid;
	const WF2DMAP_OBJWK* cp_ckwk;

	// �i�񂾐�̍��W���擾
	pos = WF2DMAP_OBJWkMatrixGet( cp_wk );
	pos = WF2DMAP_OBJToolWayPosGet( pos, way );

	// �����̃v���C���[�h�c�擾
	my_playid = WF2DMAP_OBJWkDataGet( cp_wk, WF2DMAP_OBJPM_PLID );

	// �Ǘ��I�u�W�F�N�g���擾
	objnum = WF2DMAP_OBJSysObjNumGet( cp_objsys );
	for(i=0; i<objnum; i++){
		// �����蔻��`�F�b�N���郏�[�N�擾
		cp_ckwk = WF2DMAP_OBJWkConstIdxGet( cp_objsys, i );

		// �f�[�^�������Ă��邩�`�F�b�N
		if( cp_ckwk == NULL ){
			continue;
		}
		
		// �v���C���[�h�c�擾
		ck_playid = WF2DMAP_OBJWkDataGet( cp_ckwk, WF2DMAP_OBJPM_PLID );

		// �v���C���[�h�c�����Ԃ�Ȃ��I�u�W�F�N�g�Ƃ����`�F�b�N����
		if( ck_playid != my_playid ){

			// ���ݍ��W
			ck_pos = WF2DMAP_OBJWkMatrixGet( cp_ckwk );
			if( (ck_pos.x == pos.x) && (ck_pos.y == pos.y) ){
				return cp_ckwk;	// ���������I
			}

			// �O���W
			ck_pos = WF2DMAP_OBJWkLastMatrixGet( cp_ckwk );
			if( (ck_pos.x == pos.x) && (ck_pos.y == pos.y) ){
				return cp_ckwk;	// ���������I
			}
		}
	}

	// ������Ȃ��E�E�E
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̈ʒu�ɃI�u�W�F�N�g�����邩�`�F�b�N
 *
 *	@param	cp_objsys		�I�u�W�F�N�g�V�X�e��
 *	@param	pos				���W
 */
//-----------------------------------------------------------------------------
const WF2DMAP_OBJWK* WF2DMAP_OBJSysPosHitCheck( const WF2DMAP_OBJSYS* cp_objsys, WF2DMAP_POS pos )
{
	int i;
	u32 objnum;
	WF2DMAP_POS ck_pos;
	const WF2DMAP_OBJWK* cp_ckwk;

	// �Ǘ��I�u�W�F�N�g���擾
	objnum = WF2DMAP_OBJSysObjNumGet( cp_objsys );
	for(i=0; i<objnum; i++){
		// �����蔻��`�F�b�N���郏�[�N�擾
		cp_ckwk = WF2DMAP_OBJWkConstIdxGet( cp_objsys, i );

		if( cp_ckwk == NULL ){
			continue;
		}

		// ���ݍ��W
		ck_pos = WF2DMAP_OBJWkMatrixGet( cp_ckwk );
		if( (ck_pos.x == pos.x) && (ck_pos.y == pos.y) ){
			return cp_ckwk;	// ���������I
		}

		// �O���W
		ck_pos = WF2DMAP_OBJWkLastMatrixGet( cp_ckwk );
		if( (ck_pos.x == pos.x) && (ck_pos.y == pos.y) ){
			return cp_ckwk;	// ���������I
		}
	}

	// ������Ȃ��E�E�E
	return NULL;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���쏉����
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	cp_cmd		�A�N�V�����R�}���h
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkMoveInit( WF2DMAP_OBJWK* p_wk, const WF2DMAP_ACTCMD* cp_cmd )
{
	int st;
	static void (* const pInit[WF2DMAP_CMD_NUM])( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way ) = {
		WF2DMAP_OBJSysWkNoneInit,
		WF2DMAP_OBJSysWkTurnInit,
		WF2DMAP_OBJSysWkWalkInit,
		WF2DMAP_OBJSysWkRunInit,
		WF2DMAP_OBJSysWkBusyInit,
		WF2DMAP_OBJSysWkWallWalkInit,
		WF2DMAP_OBJSysWkSlowWalkInit,
		WF2DMAP_OBJSysWkWalk4Init,
		WF2DMAP_OBJSysWkStayWalk2Init,
		WF2DMAP_OBJSysWkStayWalk4Init,
		WF2DMAP_OBJSysWkStayWalk8Init,
		WF2DMAP_OBJSysWkStayWalk16Init,
	};
	
	pInit[cp_cmd->cmd]( p_wk, cp_cmd->pos, cp_cmd->way );
	p_wk->st_frame = 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�g���상�C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkMoveMain( WF2DMAP_OBJWK* p_wk )
{
	static BOOL (* const pMain[WF2DMAP_OBJST_NUM])( WF2DMAP_OBJWK* p_wk ) = {
		WF2DMAP_OBJSysWkNoneMain,
		WF2DMAP_OBJSysWkTurnMain,
		WF2DMAP_OBJSysWkWalkMain,
		WF2DMAP_OBJSysWkRunMain,
		WF2DMAP_OBJSysWkBusyMain,
		WF2DMAP_OBJSysWkWallWalkMain,
		WF2DMAP_OBJSysWkWalkMain,
		WF2DMAP_OBJSysWkWalkMain,
		WF2DMAP_OBJSysWkStayWalk2Main,
		WF2DMAP_OBJSysWkStayWalk2Main,
		WF2DMAP_OBJSysWkStayWalk2Main,
		WF2DMAP_OBJSysWkStayWalk2Main,
	};
	u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
	BOOL result;
	
	result = pMain[st]( p_wk );
	p_wk->st_frame ++;

	if( result == TRUE ){
		// ���ł͑ҋ@��ԂɑJ�ڂ����Ă���
		WF2DMAP_OBJSysWkMoveEnd( p_wk );
		p_wk->st_frame = 0;
	}
}



//----------------------------------------------------------------------------
/**
 *	@brief	�ҋ@��Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkNoneInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

	// ���W��ݒ�
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_NONE );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�U�������Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkTurnInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );

	// ������ݒ�	�f�[�^�I�ɂ͐�ɐU��������Ă��܂�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_TURN );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_TURN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkWalkInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	WF2DMAP_POS next_pos;
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	next_pos = WF2DMAP_OBJSysWayPosGet( pos, way );	// ���̍��W�擾
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, next_pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_WALK );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_WALK );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkRunInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	WF2DMAP_POS next_pos;
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	next_pos = WF2DMAP_OBJSysWayPosGet( pos, way );	// ���̍��W�擾
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, next_pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_RUN );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_RUN );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z������Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkBusyInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_BUSY );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ǖ���������Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkWallWalkInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_WALLWALK );

	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_WALLWALK );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������������Ԑݒ�
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkSlowWalkInit( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	WF2DMAP_POS next_pos;
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	next_pos = WF2DMAP_OBJSysWayPosGet( pos, way );	// ���̍��W�擾
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, next_pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_SLOWWALK );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_SLOWWALK );
}

//----------------------------------------------------------------------------
/**
 *	@brief	4�V���N	����
 *
 *	@param	p_wk		���[�N
 *	@param	pos			�ʒu
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkWalk4Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	WF2DMAP_POS next_pos;
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	next_pos = WF2DMAP_OBJSysWayPosGet( pos, way );	// ���̍��W�擾
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, next_pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_WALK4 );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_WALK4 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏�����Q�t���[��	������
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkStayWalk2Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK2 );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_WALK2 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏�����S�t���[��	������
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkStayWalk4Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK4 );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_WALK4 );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏�����W�t���[��	������
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkStayWalk8Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK8 );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_WALK );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏�����P�U�t���[��	������
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *	@param	pos			���W
 *	@param	way			����
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkStayWalk16Init( WF2DMAP_OBJWK* p_wk, WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	GF_ASSERT( p_wk );

#ifdef DEBUG_STCHECK	// �ꉞ���쒆�ɋ�������ɐݒ肵�Ă��Ȃ����`�F�b�N
	{
		u32 st = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ST );
		if( st != WF2DMAP_OBJST_NONE ){
			// NG
			GF_ASSERT(0);
		}
	}
#endif

	// ���W��ݒ�
	WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	WF2DMAP_OBJWkMatrixSet( p_wk, pos );

	// ������ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_WAY, way );

	// ��Ԃ�ݒ�
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ST, WF2DMAP_OBJST_STAYWALK16 );
	
	// �t���[��������
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, 0 );
	WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_ENDFRAME, WF2DMAP_FRAME_SLOWWALK );
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ҋ@��ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkNoneMain( WF2DMAP_OBJWK* p_wk )
{
	// �ҋ@�Ȃ̂ŉ������Ȃ�
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�U�������ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkTurnMain( WF2DMAP_OBJWK* p_wk )
{
	return WF2DMAP_OBJSysFrameCont( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	������ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkWalkMain( WF2DMAP_OBJWK* p_wk )
{
	BOOL result;
	WF2DMAP_POS pos;
	result = WF2DMAP_OBJSysFrameCont( p_wk );
	if( result == TRUE ){
		pos = WF2DMAP_OBJWkMatrixGet( p_wk );
		WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	}
	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkRunMain( WF2DMAP_OBJWK* p_wk )
{
	BOOL result;
	WF2DMAP_POS pos;
	result = WF2DMAP_OBJSysFrameCont( p_wk );
	if( result == TRUE ){
		pos = WF2DMAP_OBJWkMatrixGet( p_wk );
		WF2DMAP_OBJWkLastMatrixSet( p_wk, pos );
	}
	return result;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Z������ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkBusyMain( WF2DMAP_OBJWK* p_wk )
{
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Ǖ�����ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkWallWalkMain( WF2DMAP_OBJWK* p_wk )
{
	return WF2DMAP_OBJSysFrameCont( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���̏������ԃ��C��
 *
 *	@param	p_wk		�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	���슮��
 *	@retval	TRUE	���쒆
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysWkStayWalk2Main( WF2DMAP_OBJWK* p_wk )
{
	return WF2DMAP_OBJSysFrameCont( p_wk );
}



//----------------------------------------------------------------------------
/**
 *	@brief	way�̕����ɂP�i�񂾍��W���擾����
 *
 *	@param	pos		���W
 *	@param	way		����
 *
 *	@return	way�̕����ɂP�i�񂾍��W
 */
//-----------------------------------------------------------------------------
static WF2DMAP_POS WF2DMAP_OBJSysWayPosGet( WF2DMAP_POS pos, WF2DMAP_WAY way )
{
	return WF2DMAP_OBJToolWayPosGet( pos, way );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�t���[���Ǘ�
 *	
 *	@param	p_wk	�I�u�W�F�N�g���[�N
 *
 *	@retval	TRUE	�I���t���[���ɂȂ���
 *	@retval	FALSE	�܂��I��肶��Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL WF2DMAP_OBJSysFrameCont( WF2DMAP_OBJWK* p_wk )
{	
	s32 frame;
	s32 endframe;

	frame = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_FRAME );
	endframe = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_ENDFRAME );
	
	if( frame < endframe ){
		frame ++;
		WF2DMAP_OBJWkDataSet( p_wk, WF2DMAP_OBJPM_FRAME, frame );

		// frame��i�߂�endframe�ɂȂ����ꍇ��TRUE��Ԃ�
		if( frame < endframe ){
			return FALSE;
		}
	}
	return TRUE;
}


//----------------------------------------------------------------------------
/**
 *	@brief	�ҋ@��Ԃɏ���������
 *
 *	@param	p_wk	�I�u�W�F�N�g���[�N
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJSysWkMoveEnd( WF2DMAP_OBJWK* p_wk )
{
	WF2DMAP_POS pos;
	WF2DMAP_WAY way;

	pos = WF2DMAP_OBJWkMatrixGet( p_wk );
	way = WF2DMAP_OBJWkDataGet( p_wk, WF2DMAP_OBJPM_WAY );
	WF2DMAP_OBJSysWkNoneInit( p_wk, pos, way );
}




