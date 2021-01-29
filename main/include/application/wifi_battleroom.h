//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_battleroom.h
 *	@brief		wifi	�o�g�����[��
 *	@author		tomoya takahashi
 *	@data		2007.02.15
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_BATTLEROOM_H__
#define __WIFI_BATTLEROOM_H__

#include "include/field/field.h"

#undef GLOBAL
#ifdef	__WIFI_BATTLEROOM_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

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
///		wifi �o�g�����[���V�X�e��
//=====================================
typedef struct _WIFI_BATTLEROOM WIFI_BATTLEROOM;

//-------------------------------------
///	�������f�[�^
//=====================================
typedef struct {
	FIELDSYS_WORK* p_fsys;
} WBR_INIT;



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
GLOBAL WIFI_BATTLEROOM* WBRSys_Init( const WBR_INIT* cp_init, u32 heapID );
GLOBAL void WBRSys_Exit( WIFI_BATTLEROOM* p_sys );
GLOBAL BOOL WBRSys_EndCheck( const WIFI_BATTLEROOM* cp_sys );


#undef	GLOBAL
#endif		// __WIFI_BATTLEROOM_H__

