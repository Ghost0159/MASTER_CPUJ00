//==============================================================================
/**
 * @file	br_sys.h
 * @brief	�o�g�����R�[�_�[
 * @author	goto
 * @date	2007.07.26(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================

#ifndef	__BR_SYS_H__
#define __BR_SYS_H__

#include "include/application/br_sys_def.h"


// -------------------------------------
//
//	�v���Z�X�f�[�^
//
// -------------------------------------
extern const PROC_DATA	BattleRecorder_ManagerProcData;		///< �Ǘ�

extern const PROC_DATA	BattleRecorder_BrowseProcData;
extern const PROC_DATA	BattleRecorder_GDSProcData;

extern const PROC_DATA* BattleRecoder_ProcDataGet( int mode );



#endif	//__BR_SYS_H__