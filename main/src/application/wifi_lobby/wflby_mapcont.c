//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_mapcont.c
 *	@brief		�}�b�v�Ǘ��V�X�e��
 *	@author		tomoya	takahashi
 *	@data		2007.11.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#include "system/arc_util.h"

#include "wflby_mapcont.h"
#include "map_conv/wflby_mapdata.h"
#include "map_conv/wflby_maparc.h"

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
#define WFLBY_MAPCONT_PARAM_SHIFT	( 15 )
#define WFLBY_MAPCONT_EVENT_MSK		( 0x00007fff )

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�v�V�X�e��������
//=====================================
typedef struct _WFLBY_MAPCONT{
	WF2DMAP_MAPSYS* p_map;	
}WFLBY_MAPCONT;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �V�X�e���쐬
//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�Ǘ�	������
 *
 *	@param	heapID	�q�[�vID
 *
 *	@return	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
WFLBY_MAPCONT* WFLBY_MAPCONT_Init( u32 heapID )
{
	WFLBY_MAPCONT* p_sys;
	void* p_mapdata;

	p_sys = sys_AllocMemory( heapID, sizeof(WFLBY_MAPCONT) );
	
	// �V�X�e���쐬
	p_sys->p_map = WF2DMAP_MAPSysInit( WFLBY_MAPSIZE_X,  WFLBY_MAPSIZE_Y, heapID );
	
	// �}�b�v�f�[�^�̓ǂݍ���
	p_mapdata = ArcUtil_Load( ARC_WFLBY_MAP, NARC_wflby_map_wflby_map_dat, FALSE, heapID, ALLOC_BOTTOM);
	WF2DMAP_MAPSysDataSet( p_sys->p_map, p_mapdata );

	sys_FreeMemoryEz( p_mapdata );

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�V�X�e���̔j��
 *
 *	@param	p_sys	�V�X�e��
 */
//-----------------------------------------------------------------------------
void WFLBY_MAPCONT_Exit( WFLBY_MAPCONT* p_sys )
{
	WF2DMAP_MAPSysExit( p_sys->p_map );
	sys_FreeMemoryEz( p_sys );
}

// �}�b�v�̑傫���擾
//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�̑傫�����擾
 *	
 *	@param	cp_sys	�V�X�e�����[�N
 */
//-----------------------------------------------------------------------------
u16 WFLBY_MAPCONT_GridXGet( const WFLBY_MAPCONT* cp_sys )
{
	return WFLBY_MAPSIZE_X;
}
u16 WFLBY_MAPCONT_GridYGet( const WFLBY_MAPCONT* cp_sys )
{
	return WFLBY_MAPSIZE_Y;
}

// �}�b�v�f�[�^�擾
//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�f�[�^�̎擾
 *
 *	@param	cp_sys	�V�X�e�����[�N
 *	@param	xgrid	���O���b�h
 *	@param	ygrid	���O���b�h
 *
 *	@return	�}�b�v�f�[�^
 */
//-----------------------------------------------------------------------------
WF2DMAP_MAP WFLBY_MAPCONT_DataGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid )
{
	return WF2DMAP_MAPSysDataGet( cp_sys->p_map, xgrid, ygrid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����蔻��̗L���`�F�b�N
 *
 *	@param	cp_sys	�V�X�e�����[�N
 *	@param	xgrid	���O���b�h
 *	@param	ygrid	���O���b�h
 *
 *	@return	�}�b�v�f�[�^
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_MAPCONT_HitGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid )
{
	return WF2DMAP_MAPSysHitGet( cp_sys->p_map, xgrid, ygrid );
}

//----------------------------------------------------------------------------
/**
 *	@brief	MAPOBJID�擾	
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	xgrid		���O���b�h
 *	@param	ygrid		���O���b�h
 *
 *	@return	WFLBY_MAPOBJID_�`
 */
//-----------------------------------------------------------------------------
u32 WFLBY_MAPCONT_ObjIDGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid )
{
	u32 param;
	param = WF2DMAP_MAPSysParamGet( cp_sys->p_map, xgrid, ygrid );
	return (param >> WFLBY_MAPCONT_PARAM_SHIFT);
}

//----------------------------------------------------------------------------
/**
 *	@brief	MAPEVID�擾
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	xgrid		���O���b�h
 *	@param	ygrid		���O���b�h
 *
 *	@return	WFLBY_MAPEVID_�`
 */
//-----------------------------------------------------------------------------
u32 WFLBY_MAPCONT_EventGet( const WFLBY_MAPCONT* cp_sys, u16 xgrid, u16 ygrid )
{
	u32 param;
	param = WF2DMAP_MAPSysParamGet( cp_sys->p_map, xgrid, ygrid );
	param &= WFLBY_MAPCONT_EVENT_MSK;
	return param;
}

//----------------------------------------------------------------------------
/**
 *	@brief	OBJID�̃O���b�h����������
 *
 *	@param	cp_sys		�V�X�e�����[�N
 *	@param	objid		����OBJID
 *	@param	p_gridx		�O���b�hX�i�[��
 *	@param	p_gridy		�O���b�hY�i�[��
 *	@param	no			���ڂ̃f�[�^���w��	�iOBJID�̃f�[�^�̎w����ʒu�j
 *
 *	@retval	TRUE	������
 *	@retval	FALSE	�Ȃ�����
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_MAPCONT_SarchObjID( const WFLBY_MAPCONT* cp_sys, u32 objid, u16* p_gridx, u16* p_gridy, u32 no )
{
	int i, j, count;
	u32 get_objid;
	
	count = 0;
	
	for( i=0; i<WFLBY_MAPSIZE_Y; i++ ){
		for( j=0; j<WFLBY_MAPSIZE_X; j++ ){
			get_objid = WFLBY_MAPCONT_ObjIDGet( cp_sys, j, i );
			if( get_objid == objid ){

				// �w����ʒu���`�F�b�N
				if( count >= no ){
					*p_gridx = j;
					*p_gridy = i;
					return TRUE;
				}
				count ++;
			}
		}
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�I�u�W�F�N�gID�̐��𐔂���
 *
 *	@param	cp_sys		�V�X�e��
 *	@param	objid		�I�u�W�F�N�gID
 *
 *	@return	��
 */
//-----------------------------------------------------------------------------
u32 WFLBY_MAPCONT_CountObjID( const WFLBY_MAPCONT* cp_sys, u32 objid )
{
	int i, j,  count;
	u32 get_objid;
	
	count =0;
	for( i=0; i<WFLBY_MAPSIZE_Y; i++ ){
		for( j=0; j<WFLBY_MAPSIZE_X; j++ ){
			get_objid = WFLBY_MAPCONT_ObjIDGet( cp_sys, j, i );
			if( get_objid == objid ){
				count ++;
			}
		}
	}
	return count;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��ɏ������N������̂��`�F�b�N
 *
 *	@param	event	�C�x���gNO
 *
 *	@retval	TRUE	�N������
 *	@retval	FALSE	�N�����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_MAPCONT_Event_CheckMount( u32 event )
{
	static const u8 sc_WFLBY_MAPCONT_EVENT_MOUNT[] = {
		WFLBY_MAPEVID_EV_PLAYER_OUT,
		WFLBY_MAPEVID_EV_FOOT_00_00,
		WFLBY_MAPEVID_EV_FOOT_00_01,
		WFLBY_MAPEVID_EV_FOOT_00_02,
		WFLBY_MAPEVID_EV_FOOT_00_03,
		WFLBY_MAPEVID_EV_FOOT_00_04,
		WFLBY_MAPEVID_EV_FOOT_00_05,
		WFLBY_MAPEVID_EV_FOOT_00_06,
		WFLBY_MAPEVID_EV_FOOT_00_07,
		WFLBY_MAPEVID_EV_FOOT_01_00,
		WFLBY_MAPEVID_EV_FOOT_01_01,
		WFLBY_MAPEVID_EV_FOOT_01_02,
		WFLBY_MAPEVID_EV_FOOT_01_03,
		WFLBY_MAPEVID_EV_FOOT_01_04,
		WFLBY_MAPEVID_EV_FOOT_01_05,
		WFLBY_MAPEVID_EV_FOOT_01_06,
		WFLBY_MAPEVID_EV_FOOT_01_07,
		WFLBY_MAPEVID_EV_WLDTIMER_00,
		WFLBY_MAPEVID_EV_WLDTIMER_01,
		WFLBY_MAPEVID_EV_WLDTIMER_02,
		WFLBY_MAPEVID_EV_WLDTIMER_03,
		WFLBY_MAPEVID_EV_TOPIC_00,
		WFLBY_MAPEVID_EV_TOPIC_01,
		WFLBY_MAPEVID_EV_TOPIC_02,
		WFLBY_MAPEVID_EV_TOPIC_03,
		WFLBY_MAPEVID_EV_BALLSLOW,
		WFLBY_MAPEVID_EV_BALANCE,
		WFLBY_MAPEVID_EV_BALLOON,
		WFLBY_MAPEVID_EV_FLOAT00,
		WFLBY_MAPEVID_EV_FLOAT01,
		WFLBY_MAPEVID_EV_FLOAT02,
		WFLBY_MAPEVID_EV_ANKETO_BOARD,
	};
	int i;

	for( i=0; i<NELEMS(sc_WFLBY_MAPCONT_EVENT_MOUNT); i++ ){
		if( event == sc_WFLBY_MAPCONT_EVENT_MOUNT[i] ){
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ڂ̑O�ł��̕����ɃL�[�������Ă�����N������̂��`�F�b�N
 *
 *	@param	event	�C�x���gNO
 *
 *	@retval	TRUE	�N������
 *	@retval	FALSE	�N�����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_MAPCONT_Event_CheckFrontKey( u32 event )
{
	static const u8 sc_WFLBY_MAPCONT_EVENT_FRONTKEY[] = {
		WFLBY_MAPEVID_EV_KANBAN00,
		WFLBY_MAPEVID_EV_KANBAN01,
		WFLBY_MAPEVID_EV_KANBAN02,
	};
	int i;

	for( i=0; i<NELEMS(sc_WFLBY_MAPCONT_EVENT_FRONTKEY); i++ ){
		if( event == sc_WFLBY_MAPCONT_EVENT_FRONTKEY[i] ){
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ڂ̑O�ł��̕�����������A�����Ă�����N������̂��`�F�b�N
 *
 *	@param	event	�C�x���gNO
 *
 *	@retval	TRUE	�N������
 *	@retval	FALSE	�N�����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_MAPCONT_Event_CheckFrontDecide( u32 event )
{
	static const u8 sc_WFLBY_MAPCONT_EVENT_FRONTA[] = {
		WFLBY_MAPEVID_EV_SW_IN,
		WFLBY_MAPEVID_EV_SW_TOPIC,
		WFLBY_MAPEVID_EV_SW_TOPIC2,
		WFLBY_MAPEVID_EV_KANBAN00,
		WFLBY_MAPEVID_EV_KANBAN01,
		WFLBY_MAPEVID_EV_KANBAN02,
		WFLBY_MAPEVID_EV_ANKETO_MAN,
	};
	int i;

	for( i=0; i<NELEMS(sc_WFLBY_MAPCONT_EVENT_FRONTA); i++ ){
		if( event == sc_WFLBY_MAPCONT_EVENT_FRONTA[i] ){
			return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����C�x���g���N�����邩�`�F�b�N
 *
 *	@param	event		�C�x���g
 *
 *	@retval	TRUE	�N������
 *	@retval	FALSE	�N�����Ȃ�
 */
//-----------------------------------------------------------------------------
BOOL WFLBY_MAPCONT_Event_CheckFrontDouzou( u32 event )
{

	static const u8 sc_WFLBY_MAPCONT_EVENT_FRONTDZ[] = {
		WFLBY_MAPEVID_EV_DZ_00,
		WFLBY_MAPEVID_EV_DZ_01,
		WFLBY_MAPEVID_EV_DZ_02,
		WFLBY_MAPEVID_EV_DZ_03,
	};
	int i;

	for( i=0; i<NELEMS(sc_WFLBY_MAPCONT_EVENT_FRONTDZ); i++ ){
		if( event == sc_WFLBY_MAPCONT_EVENT_FRONTDZ[i] ){
			return TRUE;
		}
	}

	return FALSE;
}


//  �o���邾������Ȃ��ق��������֐�
//----------------------------------------------------------------------------
/**
 *	@brief	�}�b�v�R�A�V�X�e���擾�iwf2dmap_map�j
 *
 *	@param	cp_sys	�V�X�e�����[�N
 *	
 *	@return	�}�b�v�R�A�V�X�e��
 */
//-----------------------------------------------------------------------------
const WF2DMAP_MAPSYS* WFLBY_MAPCONT_GetMapCoreSys( const WFLBY_MAPCONT* cp_sys )
{
	return cp_sys->p_map;
}
