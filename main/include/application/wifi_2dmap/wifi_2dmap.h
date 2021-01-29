//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_2dmap.h
 *	@brief		�}�b�v�����蔻��Ǘ��f�[�^
 *	@author		tomoya takahashi
 *	@data		2007.03.05
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_2DMAP_H__
#define __WIFI_2DMAP_H__

#include "application/wifi_2dmap/wifi_2dcommon.h"

#undef GLOBAL
#ifdef	__WIFI_2DMAP_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define		WF_MAP_AREAOVER_MAP		(0xffffffff)	// �͈͊O�̃}�b�v

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�}�b�v�Ǘ��V�X�e��
//=====================================
typedef struct _WF_MAP_SYS WF_MAP_SYS;


//-------------------------------------
///	�Ǘ��}�b�v�f�[�^
//=====================================
typedef union {
	u32	data;
	struct {
		u16 flag;	// �}�b�v�t���O
		u8 param;	// �p�����[�^
		u8 event;	// �C�x���g�f�[�^
	};
} WF_MAP;
#define WF_MAP_PARAM_SHIFT	(16)	// �}�b�v�p�����[�^�V�t�g
#define WF_MAP_EVENT_SHIFT	(24)	// �}�b�v�C�x���g�V�t�g

/*
 *	�Ǘ��}�b�v�f�[�^�̐���
 *		flag	�����蔻��Ɏg�p����ϐ��ł��B
 *				�������ɐl������Ƃ��A�����ɂ͕ǂ�����Ȃǂ��`�F�b�N����Ƃ��Ɏg�p���܂��B
 *		param	�Ԃ��������̂𔻕ʂ��邽�߂̃p�����[�^�ł��B
 *				�l�Ƃ̓����蔻��̂Ƃ��́Aparam�ɂ��̐l�����ʂ��鐔�������Ă����āA
 *				�Ԃ������l����肵�܂��B
 *		event	��l�������̏�ɏ�����Ƃ��ɉ������N�����������Ɏg�p���܂��B
 *				�o����A������ȂȂǂ����ʂ��鐔�������Ă����A������������`�F�b�N����
 *				���ʂɂ�艽���C�x���g�𔭍s���Ă��������B
 *
 *		data	���������邽�߂̃}�b�v�f�[�^���쐬����Ƃ���R�s�[����Ƃ��Ɏg�p���܂��B
 *				flag/param/event���܂Ƃ߂ăR�s�[���邱�Ƃ��ł��܂��B
 */

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �V�X�e���쐬�j��
GLOBAL WF_MAP_SYS* WF_MAP_SysInit( u16 xgrid, u16 ygrid, u32 heapID );
GLOBAL void WF_MAP_SysExit( WF_MAP_SYS* p_sys );

// �������}�b�v�f�[�^�œ����o�b�t�@������������
GLOBAL void WF_MAP_SysDataSet( WF_MAP_SYS* p_sys, const WF_MAP* cp_def );

// �f�[�^�ݒ�擾
GLOBAL WF_MAP WF_MAP_DataGet( const WF_MAP_SYS* cp_sys, u16 x, u16 y );
GLOBAL void WF_MAP_DataSet( WF_MAP_SYS* p_sys, u16 x, u16 y, u16 mapflag, u8 param );
GLOBAL void WF_MAP_EventSet( WF_MAP_SYS* p_sys, u16 x, u16 y, u8 event );


#undef	GLOBAL
#endif		// __WIFI_2DMAP_H__

