//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_2dobj.h
 *	@brief		wifi 2d�V�X�e���@����Ǘ�
 *	@author		tomoya takahashi
 *	@data		2007.03.06
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_2DOBJ_H__
#define __WIFI_2DOBJ_H__

#include "application/wifi_2dmap/wifi_2dcommon.h"
#include "application/wifi_2dmap/wifi_2dmap.h"

#undef GLOBAL
#ifdef	__WIFI_2DOBJ_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-------------------------------------
///	�}�b�v�t���O
//	�����蔻��Ɏg�p����萔�ł��B
//=====================================
typedef enum {
	WF_MAP_NONE = 0,
	WF_MAP_PLAYER,			// �l�̂���Ƃ���
	WF_MAP_PLAYER_NEXT,		// ���l������Ƃ���
	WF_MAP_BLOCK,			// �l�ȊO�̂����Ȃ��Ƃ���
	WF_MAP_FLAGNUM,
} WF_MAP_FLAG;


//-------------------------------------
///	�����Ԓ萔
//=====================================
typedef enum {
	WF_OBJ_STATUS_NONE,	// �ҋ@���
	WF_OBJ_STATUS_TURN,	// �U��������	
	WF_OBJ_STATUS_WALK,	// �������	
	WF_OBJ_STATUS_RUN,	// ������	
	WF_OBJ_STATUS_NUM,	// 
} WF_OBJ_STATUS;

//-------------------------------------
///	�p�����[�^�w��萔
//=====================================
typedef enum {
	WF_OBJ_PARAM_X,			// X���W
	WF_OBJ_PARAM_Y,			// Y���W
	WF_OBJ_PARAM_PLAYID,	// �v���C���[����ID	�iAID�Ȃǂ��w��j
	WF_OBJ_PARAM_STATUS,	// ���̏��
	WF_OBJ_PARAM_WAY,	// �����Ă������
	WF_OBJ_PARAM_NUM
} WF_OBJ_PARAM;


//-------------------------------------
///	���W�֘A
//=====================================
#define WF_OBJ_GRID_SIZ	( 16 )	// 1�O���b�h�T�C�Y
#define WF_OBJ_GRID_TO_POS(x)	( (x)*WF_OBJ_GRID_SIZ )	// �O���b�h�����W�ɕϊ�
#define WF_OBJ_POS_TO_GRID(x)	( (x)/WF_OBJ_GRID_SIZ )	// ���W���O���b�h�ɕϊ�

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�Ǘ��V�X�e��
//=====================================
typedef struct _WF_OBJ_SYS WF_OBJ_SYS;

//-------------------------------------
///	�I�u�W�F�N�g���[�N
//=====================================
typedef struct _WF_OBJ_WK WF_OBJ_WK;

//-------------------------------------
///	�I�u�W�F�N�g���[�N�������f�[�^
//=====================================
typedef struct {
	s16 x;		// x���W
	s16 y;		// y���W
	u16 playid;	// �v���C���[����ID	�iAID�Ȃǂ��w��j
	u16 way;	// �����iWF_COMMON_WAY�j
} WF_OBJ_WKDATA;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e���쐬�@�͂�
GLOBAL WF_OBJ_SYS* WF_OBJ_SysInit( WF_MAP_SYS* p_mapdata, u32 objnum, u32 heapID );
GLOBAL void WF_OBJ_SysExit( WF_OBJ_SYS* p_sys );

// �I�u�W�F�N�g����
GLOBAL WF_OBJ_WK* WF_OBJ_WkNew( WF_OBJ_SYS* p_sys, const WF_OBJ_WKDATA* cp_data );
GLOBAL void WF_OBJ_WkDel( WF_OBJ_SYS* p_sys, WF_OBJ_WK* p_wk );


// �I�u�W�F�N�g����
GLOBAL BOOL WF_OBJ_WkMoveReq( WF_OBJ_SYS* p_sys, WF_OBJ_WK* p_wk, WF_OBJ_STATUS req, WF_COMMON_WAY way );
GLOBAL BOOL WF_OBJ_WkMain( WF_OBJ_SYS* p_sys, WF_OBJ_WK* p_wk );
GLOBAL s32 WF_OBJ_WkParamGet( const WF_OBJ_WK* cp_wk, WF_OBJ_PARAM param );
GLOBAL void WF_OBJ_WkParamSet( WF_OBJ_WK* p_wk, WF_OBJ_PARAM param, s32 num );
GLOBAL WF_MAP WF_OBJ_WkPosMapGet( const WF_OBJ_SYS* cp_sys, const WF_OBJ_WK* cp_wk );
GLOBAL WF_MAP WF_OBJ_WkWayPosMapGet( const WF_OBJ_SYS* cp_sys, const WF_OBJ_WK* cp_wk, WF_COMMON_WAY way );


#undef	GLOBAL
#endif		// __WIFI_2DOBJ_H__

