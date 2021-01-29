//==============================================================================
/**
 * @file	battle_cursor_disp.h
 * @brief	�퓬�J�[�\�������\����ԋL���̃w�b�_
 * @author	matsuda
 * @date	2007.08.09(��)
 */
//==============================================================================
#ifndef __BATTLE_CURSOR_DISP_H__
#define __BATTLE_CURSOR_DISP_H__

#ifndef	__ASM_NO_DEF_	// ������ȍ~�́A�A�Z���u���ł͖���

//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct{
	u8 on_off;		///<TRUE:�\���AFALSE:��\��
	
	u8 dummy[3];	///<�p�f�B���O
}BATTLE_CURSOR_DISP;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern BATTLE_CURSOR_DISP * BattleCursorDisp_AllocWork(int heap_id);
extern void BattleCursorDisp_Free(BATTLE_CURSOR_DISP *bcd);

#endif	//__ASM_NO_DEF_

#endif	//__BATTLE_CURSOR_DISP_H__
