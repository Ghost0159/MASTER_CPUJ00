//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dmapobj.h
 *	@brief		�}�b�v�\�����Ǘ�
 *	@author		tomoya takahashi
 *	@data		2007.11.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_3DMAPOBJ_H__
#define __WFLBY_3DMAPOBJ_H__

#include "system/arc_tool.h"

#include "application/wifi_2dmap/wf2dmap_common.h"
#include "wflby_def.h"
#include "system/d3dobj.h"

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�t���[�g�^�C�v�萔
//=====================================
typedef enum {
	WFLBY_3DMAPOBJ_FLOAT_00,
	WFLBY_3DMAPOBJ_FLOAT_01,
	WFLBY_3DMAPOBJ_FLOAT_NUM
} WFLBY_3DMAPOBJ_FLOAT_TYPE;

//-------------------------------------
///	�z�u�I�u�W�F�^�C�v�萔
//=====================================
typedef enum {
	WFLBY_3DMAPOBJ_BIGBRONZE,
	WFLBY_3DMAPOBJ_SMALLBRONZE00,	// ���ꂼ��S�p�^�[��
	WFLBY_3DMAPOBJ_SMALLBRONZE01,	// 
	WFLBY_3DMAPOBJ_SMALLBRONZE02,	// 
	WFLBY_3DMAPOBJ_SMALLBRONZE03,	// 
	WFLBY_3DMAPOBJ_TREE,
	WFLBY_3DMAPOBJ_MG_00,
	WFLBY_3DMAPOBJ_MG_01,
	WFLBY_3DMAPOBJ_MG_02,
	WFLBY_3DMAPOBJ_TABLE,
	WFLBY_3DMAPOBJ_KANBAN,
	WFLBY_3DMAPOBJ_LAMP00,
	WFLBY_3DMAPOBJ_LAMP01,
	WFLBY_3DMAPOBJ_HANABI,
	WFLBY_3DMAPOBJ_HANABIBIG,
	WFLBY_3DMAPOBJ_BRHANABI,
	WFLBY_3DMAPOBJ_BRHANABIBIG,
	WFLBY_3DMAPOBJ_ANKETO,
	WFLBY_3DMAPOBJ_WK_NUM,
} WFLBY_3DMAPOBJ_WK_TYPE;

//-------------------------------------
///	�z�u�I�u�W�F	�A�j���^�C�v�萔
//=====================================
typedef enum {

	// �傫�ȃI�u�W�F
	WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_ALL=0,	// �풓�A�j��
	WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_ALL01,	// �풓�A�j��
	WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_ALL02,	// �풓�A�j��
	WFLBY_3DMAPOBJ_BIGBRONZEZ_ANM_NUM,		// ��

	// �������I�u�W�F
	WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH = 0,	// �G�ꂽ�Ƃ��A�j��
	WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH00,	// �G�ꂽ�Ƃ��A�j��
	WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_TOUCH01,	// �G�ꂽ�Ƃ��A�j��
	WFLBY_3DMAPOBJ_SMALLBRONZE_ANM_NUM,		// ��

	// �Ŕ�
	WFLBY_3DMAPOBJ_KANBAN_ANM_LAMP=0,	// �_���A�j��
	WFLBY_3DMAPOBJ_KANBAN_ANM_NUM,		// ��

	// �����v
	WFLBY_3DMAPOBJ_LAMP_ANM_ON=0,		// �_���A�j��
	WFLBY_3DMAPOBJ_LAMP_ANM_YURE,		// ���A�j��
	WFLBY_3DMAPOBJ_LAMP_ANM_NUM,

	// �ԉ�
	WFLBY_3DMAPOBJ_HANABI_ANM_NICA=0,	// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_HANABI_ANM_NITA,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_HANABI_ANM_NITP,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_HANABI_ANM_NUM,		// ��

	// �ł����ԉ�
	WFLBY_3DMAPOBJ_HANABIBIG_ANM_NICA=0,	// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_HANABIBIG_ANM_NITP,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_HANABIBIG_ANM_NUM,		// ��

	// �����ԉ�
	WFLBY_3DMAPOBJ_BRHANABI_ANM_NICA=0,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_BRHANABI_ANM_NITA,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_BRHANABI_ANM_NITP,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_BRHANABI_ANM_NUM,		// �ʏ�ԉ�

	// �����ł����ԉ�
	WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NICA=0,	// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NITA,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NITP,		// �ʏ�ԉ�
	WFLBY_3DMAPOBJ_BRHANABIBIG_ANM_NUM,		// �ʏ�ԉ�
	
	// �~�j�Q�[��
	WFLBY_3DMAPOBJ_MG_ANM_ALL=0,	// ��ɃA�j��
	WFLBY_3DMAPOBJ_MG_ANM_MOVE,		// ���쒆�A�j��
	WFLBY_3DMAPOBJ_MG_ANM_NUM,		// ��
	

	// �S�̂ň�ԑ����A�j����
	WFLBY_3DMAPOBJ_ALL_ANM_MAX	= 3,
} WFLBY_3DMAPOBJ_WK_ANM_TYPE;
typedef enum {	// �z�u�I�u�W�F�A�j���@�Đ��^�C�v
	WFLBY_3DMAPOBJ_WK_ANM_LOOP,				// ���[�v
	WFLBY_3DMAPOBJ_WK_ANM_NOLOOP,			// ���[�v�Ȃ�
	WFLBY_3DMAPOBJ_WK_ANM_NOAUTO,			// �t���[�����[�U�w��
	WFLBY_3DMAPOBJ_WK_ANM_BACKLOOP,			// �t�Đ����[�v
	WFLBY_3DMAPOBJ_WK_ANM_BACKNOLOOP,		// �t�Đ����[�v�Ȃ�
	WFLBY_3DMAPOBJ_WK_ANM_RANDWAIT,			// �����_���E�G�C�g�Đ�	���[�v
	WFLBY_3DMAPOBJ_WK_ANM_RANDSTART,		// �����_���X�^�[�g�Đ�	���[�v
	WFLBY_3DMAPOBJ_WK_ANM_PLAYNUM,
} WFLBY_3DMAPOBJ_WK_ANM_PLAY;
#define WFLBY_3DMAPOBJ_WK_ANM_RANDWAIT_WAITDEF	( 20 )	// ���������_���E�G�C�g�萔



//-------------------------------------
///	�t���[�g�J���[�萔
//=====================================
typedef enum {
	WFLBY_3DMAPOBJ_FLOAT_COL_00,
	WFLBY_3DMAPOBJ_FLOAT_COL_01,
	WFLBY_3DMAPOBJ_FLOAT_COL_02,
	WFLBY_3DMAPOBJ_FLOAT_COL_NUM,
} WFLBY_3DMAPOBJ_FLOAT_COL;



//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�v�\�����Ǘ��V�X�e��
//=====================================
typedef struct _WFLBY_3DMAPOBJ WFLBY_3DMAPOBJ;

//-------------------------------------
///	�t���[�g���[�N
//=====================================
typedef struct _WFLBY_3DMAPOBJ_FLOAT	WFLBY_3DMAPOBJ_FLOAT;

//-------------------------------------
///	���̑��̕����[�N
//	(�\����\�����炢�����o���Ȃ�)
//=====================================
typedef struct _WFLBY_3DMAPOBJ_WK	WFLBY_3DMAPOBJ_WK;

//-------------------------------------
///	�A�j���I���R�[���o�b�N
//=====================================
typedef void (*pWFLBY_3DMAPOBJ_WK_AnmCallBack)( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e���Ǘ�
extern WFLBY_3DMAPOBJ* WFLBY_3DMAPOBJ_Init( u32 float_num, u32 objwk_num, u32 heapID, u32 gheapID );
extern void WFLBY_3DMAPOBJ_Exit( WFLBY_3DMAPOBJ* p_sys );
extern void WFLBY_3DMAPOBJ_Main( WFLBY_3DMAPOBJ* p_sys );
extern void WFLBY_3DMAPOBJ_Draw( WFLBY_3DMAPOBJ* p_sys );
extern void WFLBY_3DMAPOBJ_VBlank( WFLBY_3DMAPOBJ* p_sys );

// �����̃��\�[�X
extern void WFLBY_3DMAPOBJ_ResLoad( WFLBY_3DMAPOBJ* p_sys, WFLBY_ROOM_TYPE room, WFLBY_SEASON_TYPE season, u32 heapID, u32 gheapID );
extern void WFLBY_3DMAPOBJ_ResRelease( WFLBY_3DMAPOBJ* p_sys );

// �}�b�v����
extern void WFLBY_3DMAPOBJ_MAP_OnPoll( WFLBY_3DMAPOBJ* p_sys );
extern void WFLBY_3DMAPOBJ_MAP_OffPoll( WFLBY_3DMAPOBJ* p_sys );

// �t���[�g���[�N����
extern WFLBY_3DMAPOBJ_FLOAT* WFLBY_3DMAPOBJ_FLOAT_Add( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_FLOAT_TYPE floattype, WFLBY_3DMAPOBJ_FLOAT_COL floatcol, const VecFx32* cp_mat );
extern void WFLBY_3DMAPOBJ_FLOAT_Del( WFLBY_3DMAPOBJ_FLOAT* p_wk );
extern void WFLBY_3DMAPOBJ_FLOAT_SetPos( WFLBY_3DMAPOBJ_FLOAT* p_wk, const VecFx32* cp_mat );
extern void WFLBY_3DMAPOBJ_FLOAT_SetOfsPos( WFLBY_3DMAPOBJ_FLOAT* p_wk, const VecFx32* cp_mat );
extern void WFLBY_3DMAPOBJ_FLOAT_GetPos( const WFLBY_3DMAPOBJ_FLOAT* cp_wk, VecFx32* p_mat );
extern void WFLBY_3DMAPOBJ_FLOAT_SetDraw( WFLBY_3DMAPOBJ_FLOAT* p_wk, BOOL flag );
extern BOOL WFLBY_3DMAPOBJ_FLOAT_GetDraw( const WFLBY_3DMAPOBJ_FLOAT* cp_wk );
extern BOOL WFLBY_3DMAPOBJ_FLOAT_SetAnmSound( WFLBY_3DMAPOBJ_FLOAT* p_wk );
extern BOOL WFLBY_3DMAPOBJ_FLOAT_SetAnmBody( WFLBY_3DMAPOBJ_FLOAT* p_wk );
extern void WFLBY_3DMAPOBJ_FLOAT_SetRotaX( WFLBY_3DMAPOBJ_FLOAT* p_wk, u16 x );
extern BOOL WFLBY_3DMAPOBJ_FLOAT_CheckAnmSound( const WFLBY_3DMAPOBJ_FLOAT* cp_wk );
extern BOOL WFLBY_3DMAPOBJ_FLOAT_CheckAnmBody( const WFLBY_3DMAPOBJ_FLOAT* cp_wk );

// �z�u�I�u�W�F����
extern WFLBY_3DMAPOBJ_WK* WFLBY_3DMAPOBJ_WK_Add( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK_TYPE objtype, u32 gridx, u32 gridy );
extern void WFLBY_3DMAPOBJ_WK_Del( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );
extern WFLBY_3DMAPOBJ_WK_TYPE WFLBY_3DMAPOBJ_WK_GetType( const WFLBY_3DMAPOBJ_WK* cp_wk );
extern void WFLBY_3DMAPOBJ_WK_SetPos( WFLBY_3DMAPOBJ_WK* p_wk, WF2DMAP_POS pos );
extern WF2DMAP_POS WFLBY_3DMAPOBJ_WK_GetPos( const WFLBY_3DMAPOBJ_WK* cp_wk );
extern void WFLBY_3DMAPOBJ_WK_AddAnm( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm, WFLBY_3DMAPOBJ_WK_ANM_PLAY play );
extern void WFLBY_3DMAPOBJ_WK_AddAnmAnmCallBack( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm, WFLBY_3DMAPOBJ_WK_ANM_PLAY play, pWFLBY_3DMAPOBJ_WK_AnmCallBack p_callback );
extern void WFLBY_3DMAPOBJ_WK_DelAnm( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm );
extern void WFLBY_3DMAPOBJ_WK_DelAnmAll( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk );
extern BOOL WFLBY_3DMAPOBJ_WK_CheckAnm( const WFLBY_3DMAPOBJ* cp_sys, const WFLBY_3DMAPOBJ_WK* cp_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm );
extern void WFLBY_3DMAPOBJ_WK_SetAnmFrame( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm, fx32 frame );	// �A�j���^�C�v��WFLBY_3DMAPOBJ_WK_ANM_NOAUTO�̂Ƃ���p
extern BOOL WFLBY_3DMAPOBJ_WK_GetAnmFlag( const WFLBY_3DMAPOBJ_WK* cp_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm );
extern fx32 WFLBY_3DMAPOBJ_WK_GetAnmFrame( const WFLBY_3DMAPOBJ_WK* cp_wk, WFLBY_3DMAPOBJ_WK_ANM_TYPE anm );
extern void WFLBY_3DMAPOBJ_WK_SetRandWait( WFLBY_3DMAPOBJ_WK* p_wk, u32 wait );
extern void WFLBY_3DMAPOBJ_WK_SetDraw( WFLBY_3DMAPOBJ_WK* p_wk, BOOL flag );
extern void WFLBY_3DMAPOBJ_WK_SetLightFlag( WFLBY_3DMAPOBJ* p_sys, WFLBY_3DMAPOBJ_WK* p_wk, u32 light_flag );
extern void WFLBY_3DMAPOBJ_WK_SetAnmSpeed( WFLBY_3DMAPOBJ_WK* p_wk, fx32 speed );
extern fx32 WFLBY_3DMAPOBJ_WK_GetAnmSpeed( const WFLBY_3DMAPOBJ_WK* cp_wk );
extern void WFLBY_3DMAPOBJ_WK_SetAlpha( WFLBY_3DMAPOBJ_WK* p_wk, u8 alpha, u8 def_alpha );
extern void WFLBY_3DMAPOBJ_WK_ResetAlpha( WFLBY_3DMAPOBJ_WK* p_wk );
extern void WFLBY_3DMAPOBJ_WK_SetScale( WFLBY_3DMAPOBJ_WK* p_wk, fx32 x, fx32 y, fx32 z );


#endif		// __WFLBY_3DMAPOBJ_H__

