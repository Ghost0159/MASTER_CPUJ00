//==============================================================================
/**
 * @file	balloon_entry.h
 * @brief	���D����F�G���g���[��ʂ̃w�b�_
 * @author	matsuda
 * @date	2007.11.06(��)
 */
//==============================================================================
#ifndef __BALLOON_ENTRY_H__
#define __BALLOON_ENTRY_H__


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern PROC_RESULT BalloonEntryProc_Init( PROC * proc, int * seq );
extern PROC_RESULT BalloonEntryProc_Main( PROC * proc, int * seq );
extern PROC_RESULT BalloonEntryProc_End(PROC *proc, int *seq);


#endif	//__BALLOON_ENTRY_H__

