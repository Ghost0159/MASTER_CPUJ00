//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_3dmapobj_data.h
 *	@brief		�z�u�I�u�W�F�O���t�B�b�N�ǂݍ��݃f�[�^
 *	@author		tomoya takahashi
 *	@data		2007.11.20
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WFLBY_3DMAPOBJ_DATA_H__
#define __WFLBY_3DMAPOBJ_DATA_H__

#include "../wflby_3dmapobj.h"


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�n�ʒ萔
//=====================================
enum {	// �A�j���萔
	WFLBY_3DMAPOBJ_MAP_ANM_LOBBY,
	WFLBY_3DMAPOBJ_MAP_ANM_WLDTIMER,
	WFLBY_3DMAPOBJ_MAP_ANM_FLAG,
	WFLBY_3DMAPOBJ_MAP_ANM_MAT,		// �}�b�g�A�j��
	WFLBY_3DMAPOBJ_MAP_ANM_POLL,
	WFLBY_3DMAPOBJ_MAP_ANM_NUM,
} ;
enum {	// ���f����
	WFLBY_3DMAPOBJ_MAPOBJ_MAP,	// �n�ʃI�u�W�F
	WFLBY_3DMAPOBJ_MAPOBJ_MAT,	// �}�b�g�I�u�W�F
	WFLBY_3DMAPOBJ_MAPOBJ_NUM,	// �n�ʃI�u�W�F��
} ;


//-------------------------------------
///	�t���[�g�萔
//=====================================
enum {	// �A�j���萔
	WFLBY_3DMAPOBJ_FLOAT_ANM_00,
	WFLBY_3DMAPOBJ_FLOAT_ANM_01,		// �������p
	WFLBY_3DMAPOBJ_FLOAT_ANM_02,		// ��
	WFLBY_3DMAPOBJ_FLOAT_ANM_03,		// �K��
	WFLBY_3DMAPOBJ_FLOAT_ANM_NUM,

	WFLBY_3DMAOOBJ_FLOAT_NODE_NUM = 3,	// �A�j���ɂ�������Ă���m�[�h�̐�(���ƐK���Ƌ������p)
} ;


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�z�u�I�u�W�F�N�g�f�[�^
// 5/28���ݗv�f��109
//=====================================
typedef struct {
	// �z�u�I�u�W�F�֌W
	u32		obj_idx[WFLBY_3DMAPOBJ_WK_NUM];											// 18
	u32		obj_anm[WFLBY_3DMAPOBJ_WK_NUM][WFLBY_3DMAPOBJ_ALL_ANM_MAX];				// ���ꂼ��̃I�u�W�F���̃A�j��	�g�p���Ȃ��ꍇ�́Amdl��DATAID�������Ă܂�									   // 18*3 = 54

	// FLOAT�֌W
	u32		float_idx[WFLBY_3DMAPOBJ_FLOAT_NUM];									// 2
	u32		float_texidx[WFLBY_3DMAPOBJ_FLOAT_NUM][WFLBY_3DMAPOBJ_FLOAT_COL_NUM];	// 2*3	6
	u32		float_anmidx[WFLBY_3DMAPOBJ_FLOAT_NUM][WFLBY_3DMAPOBJ_FLOAT_ANM_NUM];	// 2*4	8
	u32		float_nodenum[WFLBY_3DMAPOBJ_FLOAT_NUM];								// 2
	u32		float_anm_node[WFLBY_3DMAPOBJ_FLOAT_NUM][ WFLBY_3DMAOOBJ_FLOAT_NODE_NUM ];// 2*3 6

	// �}�b�v�֌W
	u32		map_idx[ WFLBY_3DMAPOBJ_MAPOBJ_NUM ];									// 2
	u32		map_nodenum;															// �}�b�v�̃m�[�h��	1
	u32		map_anmidx[ WFLBY_3DMAPOBJ_MAP_ANM_NUM ];								// 5
	u32		map_anmnodeidx[ WFLBY_3DMAPOBJ_MAP_ANM_NUM ];							// 5
} WFLBY_3DMAPOBJ_MDL_DATA;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

#endif		// __WFLBY_3DMAPOBJ_DATA_H__

