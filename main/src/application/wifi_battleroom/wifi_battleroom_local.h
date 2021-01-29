//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wifi_battleroom_local.h
 *	@brief		wifi �o�g�����[�����[�J���w�b�_
 *	@author		tomoya takahashi
 *	@data		2007.02.21
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_BATTLEROOM_LOCAL_H__
#define __WIFI_BATTLEROOM_LOCAL_H__

#include "include/field/field.h"
#include "wbr_common.h"
#include "wbr_surver.h"
#include "wbr_client.h"


#undef GLOBAL
#ifdef	__WIFI_BATTLEROOM_LOCAL_H_GLOBAL
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
typedef struct _WIFI_BATTLEROOM {

	FIELDSYS_WORK* p_fsys;
	
	// ���݂̏��
	u32 status;
	
	u32	netid;	// ������NETID
	
	// �e�f�[�^
	WBR_SURVER* p_surver;

	// �q�f�[�^
	WBR_CLIENT* p_client;

	// �}�l�[�W���̃^�X�N
	TCB_PTR tcb;

	// �X�e�[�^�X
	WBR_MYSTATUS	mystatus;
}WIFI_BATTLEROOM;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
#undef	GLOBAL
#endif		// __WIFI_BATTLEROOM_LOCAL_H__

