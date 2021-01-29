//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_map.h
 *	@brief		�}�b�v�f�[�^�Ǘ��V�X�e��
 *	@author		tomoya takahashi
 *	@data		2007.03.14
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_MAP_H__
#define __WF2DMAP_MAP_H__

#include "application/wifi_2dmap/wf2dmap_common.h"

#undef GLOBAL
#ifdef	__WF2DMAP_MAP_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define WF2DMAP_MAPDATA_NONE	(0xffffffff)

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�v�f�[�^�\����
//	�f�[�^�\��
//	31�`1 0		bit
//	param hit
//
//	1bit���q�b�g�t���O
//	�c��31bit���p�����[�^
//=====================================
typedef union {
	u32 data;
	struct {
		u32 hit:1;		// �����蔻��f�[�^
		u32	param:31;	// �p�����[�^
	};
} WF2DMAP_MAP;
#define WF2DMAP_MAPDATA(hit, param)		(((param)<<1) | (hit))	// �}�b�v�f�[�^�쐬

//-------------------------------------
///	�}�b�v�f�[�^�Ǘ��V�X�e�����[�N
//=====================================
typedef struct _WF2DMAP_MAPSYS WF2DMAP_MAPSYS;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
// �V�X�e���Ǘ�
GLOBAL WF2DMAP_MAPSYS* WF2DMAP_MAPSysInit( u16 xgrid, u16 ygrid, u32 heapID );
GLOBAL void WF2DMAP_MAPSysExit( WF2DMAP_MAPSYS* p_sys );

// �}�b�v�f�[�^�ݒ�
GLOBAL void WF2DMAP_MAPSysDataSet( WF2DMAP_MAPSYS* p_sys, const WF2DMAP_MAP* cp_buff );
// �}�b�v�̑傫���擾
GLOBAL u16 WF2DMAP_MAPSysGridXGet( const WF2DMAP_MAPSYS* cp_sys );
GLOBAL u16 WF2DMAP_MAPSysGridYGet( const WF2DMAP_MAPSYS* cp_sys );

// �}�b�v�f�[�^�擾
GLOBAL WF2DMAP_MAP WF2DMAP_MAPSysDataGet( const WF2DMAP_MAPSYS* cp_sys, u16 xgrid, u16 ygrid );
GLOBAL BOOL WF2DMAP_MAPSysHitGet( const WF2DMAP_MAPSYS* cp_sys, u16 xgrid, u16 ygrid );
GLOBAL u32 WF2DMAP_MAPSysParamGet( const WF2DMAP_MAPSYS* cp_sys, u16 xgrid, u16 ygrid );


#undef	GLOBAL
#endif		// __WF2DMAP_MAP_H__

