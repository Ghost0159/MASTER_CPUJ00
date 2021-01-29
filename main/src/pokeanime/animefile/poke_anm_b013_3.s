//==============================================================================
/**
 * @file	poke_anm_b013_3.s
 * @brief	沈む
 * @author	
 * @date	
 *
 */
//==============================================================================

	.text
	
	.include	"past.h"
	.include	"past_def.h"
	
	.global		PAST_ANIME_B013_3
	
// =============================================================================
//
//
//	背面	沈むスロー
//
//
// =============================================================================
#define TILT	(8*0x100)
#define CALC_NUM	(45)	

PAST_ANIME_B013_3:
	//回転中心を左にずらす
	SET_ADD_VAL		 PARA_ROT_CX,USE_VAL,-32,PARAM_SET

	//CALC_NUM回の処理で180°を処理
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN,TARGET_ROT,TILT,0x8000,0,CALC_NUM
	HOLD_CMD
	
	END_ANM
