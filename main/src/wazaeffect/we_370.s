//==============================================================================
/**
 * @file	we_001.s
 * @brief	はたく
 * @author	goto
 * @date	2005.07.13(水)
 *
 * ここに色々な解説等を書いてもよい
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_370
	
// =============================================================================
//
//
//	■はたく
//
//
// =============================================================================
WEST_370:

	LOAD_PARTICLE_DROP	0,W_370_SPA

	WORK_SET		WEDEF_GP_INDEX_SPEED_X,  WEDEF_HAIKEI_SPEED_X
	WORK_SET		WEDEF_GP_INDEX_SPEED_Y,  WEDEF_HAIKEI_SPEED_Y
	HAIKEI_CHG		BG_ID_370, WEDEF_HAIKEI_MODE_FADE | WEDEF_HAIKEI_SBIT_MOVE
	HAIKEI_CHG_WAIT
		
	ADD_PARTICLE 	0,W_370_370_INFIGHT_HAND, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE
	
	ADD_PARTICLE 	0,W_370_370_INFIGHT_HIT, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE

	ADD_PARTICLE 	0,W_370_370_INFIGHT_LINE, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE
	
	SE_REPEAT_R		SEQ_SE_DP_003,4,5
	SE_REPEAT_R		SEQ_SE_DP_030,5,5

	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_M_X, 0, SHAKE_M_WAIT, SHAKE_M_NUM*5,  WE_TOOL_E1 | WE_TOOL_SSP,
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,

	WORK_SET		WEDEF_GP_INDEX_SPEED_X,  WEDEF_HAIKEI_HI_SPEED_X
	WORK_SET		WEDEF_GP_INDEX_SPEED_Y,  WEDEF_HAIKEI_SPEED_Y
	HAIKEI_RECOVER	BG_ID_370, WEDEF_HAIKEI_RC_MODE_FADE | WEDEF_HAIKEI_SBIT_STOP
	HAIKEI_CHG_WAIT
	SEQEND

