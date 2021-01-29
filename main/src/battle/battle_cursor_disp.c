//==============================================================================
/**
 * @file	battle_cursor_disp.c
 * @brief	�퓬�̏����J�[�\���\����ԋL���p�\����
 * @author	matsuda
 * @date	2007.08.09(��)
 */
//==============================================================================
#include "common.h"
#include "battle/battle_cursor_disp.h"


//--------------------------------------------------------------
/**
 * @brief   �퓬�J�[�\�������\����ԃ��[�N�̊m��
 *
 * @param   heap_id		�q�[�vID
 *
 * @retval  �퓬�J�[�\�������\����ԃ��[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
BATTLE_CURSOR_DISP * BattleCursorDisp_AllocWork(int heap_id)
{
	BATTLE_CURSOR_DISP *bcd;
	
	bcd = sys_AllocMemory(heap_id, sizeof(BATTLE_CURSOR_DISP));
	MI_CpuClear8(bcd, sizeof(BATTLE_CURSOR_DISP));
	return bcd;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�J�[�\�������\����ԃ��[�N�̉��
 *
 * @param   bcd		�퓬�J�[�\�������\����ԃ��[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void BattleCursorDisp_Free(BATTLE_CURSOR_DISP *bcd)
{
	sys_FreeMemoryEz(bcd);
}

