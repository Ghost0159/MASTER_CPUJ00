//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_2dcommon.h
 *	@brief		wifi_2d�V�X�e�����ʃw�b�_
 *	@author		tomoya takahashi
 *	@data		2007.03.06
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_2DCOMMON_H__
#define __WIFI_2DCOMMON_H__

#undef GLOBAL
#ifdef	__WIFI_2DCOMMON_H_GLOBAL
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
///	�����Ă������
typedef enum {
	WF_COMMON_TOP,
	WF_COMMON_BOTTOM,
	WF_COMMON_LEFT,
	WF_COMMON_RIGHT,
	WF_COMMON_WAYNUM
} WF_COMMON_WAY;

#define WF_COMMON_WALK_FRAME	(8)		// �����t���[����
#define WF_COMMON_TURN_FRAME	(2)		// �U������t���[����
#define WF_COMMON_RUN_FRAME		(4)		// ����t���[����
#define WF_COMMON_WALLWALK_FRAME (16)	// �Ǖ����t���[����
#define WF_COMMON_SLOWWALK_FRAME (16)	// ������肠�邫�t���[����
#define WF_COMMON_HIGHWALK2_FRAME (2)	// �������邫�t���[����
#define WF_COMMON_HIGHWALK4_FRAME (4)	// �������邫�t���[����

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
#undef	GLOBAL
#endif		// __WIFI_2DCOMMON_H__

