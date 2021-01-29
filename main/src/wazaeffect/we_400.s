//==============================================================================
/**
 * @file	we_001.s
 * @brief	�͂���
 * @author	goto
 * @date	2005.07.13(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_400
	
// =============================================================================
//
//
//	���͂���
//
//
// =============================================================================
WEST_400:
	
	LOAD_PARTICLE_DROP	0,W_400_SPA

	ADD_PARTICLE 	0,W_400_400_LINE_SCROLL, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_NONE, OPERATOR_POS_NONE, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE

	///< 1���
	SE_C			SEQ_SE_DP_BRADE
	SE_WAITPLAY_C	SEQ_SE_DP_W233,10
	ADD_PARTICLE 	0,W_400_400_LINE_SLASH1, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE
	WAIT			11
	ADD_PARTICLE 	0,W_400_B_BURN_RED2,  EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_400_400_HIT_MARK, EMTFUNC_DEFENCE_POS
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_S_X, 0, SHAKE_S_WAIT, SHAKE_S_NUM*3,  WE_TOOL_E1 | WE_TOOL_SSP,

	///< 2���
	WAIT			10
	SE_C			SEQ_SE_DP_BRADE
	SE_WAITPLAY_C	SEQ_SE_DP_W233,10
	ADD_PARTICLE 	0,W_400_400_LINE_SLASH2, EMTFUNC_FIELD_OPERATOR
	EX_DATA			OPERATOR_DEF_DATA_NUM, OPERATOR_PRI_NONE, OPERATOR_TARGET_DF, OPERATOR_POS_EP, OPERATOR_AXIS_NONE, OPERATOR_FLD_NONE, OPERATOR_CAMERA_NONE
	WAIT			8
	ADD_PARTICLE 	0,W_400_400_HIT_MARK, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_400_B_BURN_RED2,  EMTFUNC_DEFENCE_POS
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	SEQEND
