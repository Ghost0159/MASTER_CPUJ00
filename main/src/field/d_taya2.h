//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d_taya2.h
 *	@brief		�c�J����@�f�o�b�N���j���[	�풓����
 *	@author		tomoya takahashi
 *	@data		2007.04.18
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __D_TAYA2_H__
#define __D_TAYA2_H__


#include "common.h"
#include "fieldsys.h"
#include "field_event.h"

#undef GLOBAL
#ifdef	__D_TAYA2_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------
/**
 * ����Ƀt�F�[�h�A�E�g���ĉ����ʉ�ʂ̏��������ĂԂ���
 */
//-----------------------------------------------------------------
typedef void (*ScheneChangeCallback)(FIELDSYS_WORK*, void* wk);

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------

typedef struct {
	u32					seq;
	ScheneChangeCallback	start_func;
	ScheneChangeCallback	end_func;
	BOOL				fadeFlag;
	void*				gen_wk;
}DEBUG_CHANGESCHENE_WORK;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

GLOBAL BOOL GMEVENT_ChangeScheneDebug(GMEVENT_CONTROL * event);

#undef	GLOBAL
#endif		// __D_TAYA2_H__

