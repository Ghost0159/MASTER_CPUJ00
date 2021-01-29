//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_p2pmatchroom_map.h
 *	@brief		wifi	�}�b�`���O���[��	�}�b�v�f�[�^
 *	@author		tomoya takahashi
 *	@data		2007.03.30
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_P2PMATCHROOM_MAP_H__
#define __WIFI_P2PMATCHROOM_MAP_H__

#include "application/wifi_2dmap/wf2dmap_common.h"
#include "application/wifi_2dmap/wf2dmap_map.h"

#undef GLOBAL
#ifdef	__WIFI_P2PMATCHROOM_MAP_H_GLOBAL
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
///	�}�b�v�p�����[�^
//=====================================
enum {
	MCR_MAPPM_NONE,
	MCR_MAPPM_BLOCK,	// �����Ȃ��Ƃ���
	MCR_MAPPM_PC,		// Personal Computer
	MCR_MAPPM_MAP00,	// �΃}�b�g
	MCR_MAPPM_MAP01,	// ���F�}�b�g
	MCR_MAPPM_MAP02,	// �}�b�g
	MCR_MAPPM_MAP03,	// �ԃ}�b�g
	MCR_MAPPM_OBJ00,	// �I�u�W�F�z�u�ʒu00
	MCR_MAPPM_OBJ01,	// �I�u�W�F�z�u�ʒu01
	MCR_MAPPM_OBJ02,	// �I�u�W�F�z�u�ʒu02
	MCR_MAPPM_OBJ03,	// �I�u�W�F�z�u�ʒu03
	MCR_MAPPM_OBJ04,	// �I�u�W�F�z�u�ʒu04
	MCR_MAPPM_OBJ05,	// �I�u�W�F�z�u�ʒu05
	MCR_MAPPM_OBJ06,	// �I�u�W�F�z�u�ʒu06
	MCR_MAPPM_OBJ07,	// �I�u�W�F�z�u�ʒu07
	MCR_MAPPM_EXIT,
};
#define WCR_MAPDATA_BLOCK	(WF2DMAP_MAPDATA(1,MCR_MAPPM_BLOCK))
#define WCR_MAPDATA_PC		(WF2DMAP_MAPDATA(1,MCR_MAPPM_PC))
#define WCR_MAPDATA_EXIT	(WF2DMAP_MAPDATA(0,MCR_MAPPM_EXIT))
#define WCR_MAPDATA_MAP00	(WF2DMAP_MAPDATA(0,MCR_MAPPM_MAP00))
#define WCR_MAPDATA_MAP01	(WF2DMAP_MAPDATA(0,MCR_MAPPM_MAP01))
#define WCR_MAPDATA_MAP02	(WF2DMAP_MAPDATA(0,MCR_MAPPM_MAP02))
#define WCR_MAPDATA_MAP03	(WF2DMAP_MAPDATA(0,MCR_MAPPM_MAP03))
#define WCR_MAPDATA_OBJ00	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ00))
#define WCR_MAPDATA_OBJ01	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ01))
#define WCR_MAPDATA_OBJ02	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ02))
#define WCR_MAPDATA_OBJ03	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ03))
#define WCR_MAPDATA_OBJ04	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ04))
#define WCR_MAPDATA_OBJ05	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ05))
#define WCR_MAPDATA_OBJ06	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ06))
#define WCR_MAPDATA_OBJ07	(WF2DMAP_MAPDATA(0,MCR_MAPPM_OBJ07))

#define WCR_MAPDATA_1BLOCKOBJNUM	( 8 )	// 1BLOCK�ɓ���l���̐�


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL WF2DMAP_POS WcrMapGridSizGet( u32 no );
GLOBAL const WF2DMAP_MAP* WcrMapDataGet( u32 no );


#undef	GLOBAL
#endif		// __WIFI_P2PMATCHROOM_MAP_H__

