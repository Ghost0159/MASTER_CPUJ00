//==============================================================================
/**
 * @file	poke_anm024.s
 * @brief	横とびはね
 * @author	saito
 * @date	
 *
 */
//==============================================================================

	.text
	
	.include	"past.h"
	.include	"past_def.h"
	
	.global		PAST_ANIME024
	
// =============================================================================
//
//
//	横とびはね
//
//
// =============================================================================
#define OFS_Y	( -6 )
#define ROT_1	( 0x2000 )
#define ROT_3	( 0x1000 )
#define ROT_2	( 0x0800 )
PAST_ANIME024:
	//>飛び跳ね1回目
	//4回の処理で0x8000（180°）を処理
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,4
	//4回の処理で8移動
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,8,4
	//4回の処理で0x1000(22.5°)傾く
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_ROT,ROT_2,4
	HOLD_CMD
	
	//>飛び跳ね2〜4回目
	START_LOOP	2
		
		//スプライト位置をx=8に設定 dxを0に設定
		SET_ADD_PARAM		PARAM_X,USE_VAL,8,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		APPLY_TRANS
		//スプライト回転を0x1000にセット
		SET_ADD_PARAM		PARAM_ROT,USE_VAL,ROT_2,PARAM_SET
		APPLY_AFFINE
		
		//6回の処理で0x8000（180°）を処理
		CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,6
		//6回の処理で-16移動
		CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,-16,6
		//6回の処理で0x1000〜-0x1000の-0x2000(45°)分傾く
		CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,-ROT_3,6
		HOLD_CMD

		//スプライト位置をx=-8に設定 dxを0に設定
		SET_ADD_PARAM		PARAM_X,USE_VAL,-8,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		APPLY_TRANS
		//スプライト回転を-0x1000にセット
		SET_ADD_PARAM		PARAM_ROT,USE_VAL,-ROT_2,PARAM_SET
		APPLY_AFFINE

		//6回の処理で0x8000（180°）を処理
		CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,6
		//6回の処理で16移動
		CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,16,6
		//6回の処理で-0x1000〜0x1000の0x2000(45°)分傾く
		CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,ROT_3,6
		HOLD_CMD

	END_LOOP

	//スプライト位置をx=8に設定 dxを0に設定
	SET_ADD_PARAM		PARAM_X,USE_VAL,8,PARAM_SET
	SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
	APPLY_TRANS
	//スプライト回転を0x1000にセット
	SET_ADD_PARAM		PARAM_ROT,USE_VAL,ROT_2,PARAM_SET
	APPLY_AFFINE

	//>飛び跳ね5回目
	//6回の処理で0x8000（180°）を処理
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,6
	//6回の処理で-16移動
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,-16,6
	//6回の処理で0x1000〜-0x1000の-0x2000(45°)分傾く
	CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,-ROT_3,6
	HOLD_CMD

	//スプライト位置をx=-8に設定 dxを0に設定
	SET_ADD_PARAM		PARAM_X,USE_VAL,-8,PARAM_SET
	SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
	APPLY_TRANS
	//スプライト回転を-0x1000にセット
	SET_ADD_PARAM		PARAM_ROT,USE_VAL,-ROT_2,PARAM_SET
	APPLY_AFFINE
	
	//>飛び跳ね6回目
	//6回の処理で0x8000（180°）を処理
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,6
	//6回の処理で8移動
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,8,6
	//6回の処理で0x1000(45°)傾く
	CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,ROT_2,6
	HOLD_CMD
	
	
	END_ANM
