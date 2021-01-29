#ifdef PM_DEBUG
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		d_mori2.h
 *	@brief		�X����̃f�o�b�N���j���[�풓����
 *	@author		tomoya takahashi
 *	@data		2007.04.17
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __D_MORI2_H__
#define __D_MORI2_H__

#include "common.h"
#include "application/namein.h"
#include "fieldsys.h"

#undef GLOBAL
#ifdef	__D_MORI2_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *		���O���͌Ăяo���e�X�g
 *		�������X�g�Ăяo��
 */
//-----------------------------------------------------------------------------
typedef struct{
	int 			seq;
	int 			sub;
	int				flag;
	GF_BGL_INI    	*bgl;
	FIELDSYS_WORK 	*fsys;
	NAMEIN_PARAM   *NameInParam;
}TEST_PROC_WORK;
GLOBAL void TestNameIn_Call( TCB_PTR tcb, void* work );
GLOBAL void TestTradeList_Call( TCB_PTR tcb, void* work );
GLOBAL void TestWorldTrade_Call( TCB_PTR tcb, void* work );

#undef	GLOBAL
#endif		// __D_MORI2_H__

#endif	// PM_DEBUG
