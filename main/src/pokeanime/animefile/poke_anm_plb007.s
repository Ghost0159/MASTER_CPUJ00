//==============================================================================
/**
 * @file	poke_anm007.s
 * @brief	
 * @author	
 * @date	
 *
 */
//==============================================================================

	.text
	
	.include	"past.h"
	.include	"past_def.h"
	
	.global		PAST_ANIME_PLB007
	
// =============================================================================
//
//
//	�s�����s����	
//
//
// =============================================================================
#define OFS_Y1	( 6 - 1 )	///< �ӂ蕝
#define OFS_Y2	( 9 - 2 )	///< �ӂ蕝 �w�ʗp�Ȃ̂ŁA8�ȉ��Ɏw��

PAST_ANIME_PLB007:
	START_LOOP	2
		CALL_MF_CURVE	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y1,0x2000,0,4
		HOLD_CMD
	END_LOOP
	
	CALL_MF_CURVE	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y2,0x2000,0,4
	HOLD_CMD
	END_ANM
