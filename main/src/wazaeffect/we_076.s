//==============================================================================
/**
 * @file	we_076.s
 * @brief	�\�[���[�r�[��			76
 * @author	goto
 * @date	2005.07.13(��)
 *
 * �����ɐF�X�ȉ�����������Ă��悢
 *
 */
//==============================================================================
	
	.text
	
	.include	"west.h"

	.global		WEST_SOORAABIIMU
	
// =============================================================================
//
//
//	���\�[���[�r�[��			76
//
//
// =============================================================================
WEST_SOORAABIIMU:
	
	LOAD_PARTICLE_DROP	0,W_076_SPA

	TURN_CHK	WEST_076_TURN_1, WEST_076_TURN_2

	SEQEND

WEST_076_TURN_1:
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG

	ADD_PARTICLE 	0,W_076_076_SOLAR_CHARGE, EMTFUNC_ATTACK_POS
	
	WAIT			10
	SE_L		SEQ_SE_DP_SHUSHU
	WAIT			20
	
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_M1, 0, 2, WE_PAL_YELLOW, 10,0

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	WAIT_FLAG

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG
		
	SEQEND

WEST_076_TURN_2:
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG
	
	ADD_PARTICLE 	0,W_076_076_BALL_BEAM01,	EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_076_076_SOLAR_BEAM14A,	EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_076_076_SOLAR_BEAM14B,	EMTFUNC_ATTACK_POS
	
	WAIT			5
	SE_FLOW_LR		SEQ_SE_DP_W076B
	
	WAIT			15	
	
	ADD_PARTICLE 	0,W_076_076_SOLAR_BEAM25A,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_076_076_SOLAR_BEAM25B,	EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_076_076_BALL_BEAM02,	EMTFUNC_DEFENCE_POS
	
	WAIT			5	

	POKE_FADE_EZ	WE_PAL_YELLOW, WE_TOOL_E1, 2
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_B_X, 0, SHAKE_B_WAIT, SHAKE_NUM_LONG*3,  WE_TOOL_E1 | WE_TOOL_SSP,
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG
	SEQEND

/*
WEST_076_TURN_2:

	PTAT_JP			PTAT

	CONTEST_JP		WEST_CONTEST

	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG

//	WORK_CLEAR
//	HAIKEI_CHG		BG_ID_076, WEDEF_HAIKEI_MODE_FADE | WEDEF_HAIKEI_SBIT_NONE
//	HAIKEI_CHG_WAIT

	ADD_PARTICLE_SEP	0, W_076_076_SOLAR_BEAM14A, W_076_076_SOLAR_BEAM25A, W_076_076_SOLAR_BEAM36A, W_076_076_SOLAR_BEAM14A, W_076_076_SOLAR_BEAM25A, W_076_076_SOLAR_BEAM36A, EMTFUNC_SEP_POS
	
	ADD_PARTICLE_SEP	0, W_076_076_SOLAR_BEAM14B, W_076_076_SOLAR_BEAM25B, W_076_076_SOLAR_BEAM36B, W_076_076_SOLAR_BEAM14B, W_076_076_SOLAR_BEAM25B, W_076_076_SOLAR_BEAM36B, EMTFUNC_SEP_POS
		
	ADD_PARTICLE_SEP	0, W_076_076_BALL_BEAM01, W_076_076_BALL_BEAM02, W_076_076_BALL_BEAM03, W_076_076_BALL_BEAM04, W_076_076_BALL_BEAM05, W_076_076_BALL_BEAM06, EMTFUNC_SEP_POS
	
	SE_FLOW_LR		SEQ_SE_DP_W076B
	WAIT			10
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_B_X, 0, SHAKE_B_WAIT, SHAKE_NUM_LONG*3,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	WAIT_FLAG
	
//	WORK_CLEAR
//	HAIKEI_RECOVER	BG_ID_076, WEDEF_HAIKEI_RC_MODE_FADE | WEDEF_HAIKEI_SBIT_NONE
//	HAIKEI_CHG_WAIT
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG
	
	SEQEND

///< 2 2
PTAT:
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 0, 12, WE_PAL_BLACK,
	WAIT_FLAG

//	WORK_CLEAR
//	HAIKEI_CHG		BG_ID_076, WEDEF_HAIKEI_MODE_FADE | WEDEF_HAIKEI_SBIT_NONE
//	HAIKEI_CHG_WAIT

	ADD_PARTICLE_PTAT	0, W_076_076_SOLAR_BEAM_A_PARTY, W_076_076_SOLAR_BEAM_A_PARTY, W_076_076_SOLAR_BEAM_A_PARTY, W_076_076_SOLAR_BEAM_A_PARTY, EMTFUNC_ATTACK_POS
	ADD_PARTICLE_PTAT	0, W_076_076_SOLAR_BEAM_B_PARTY, W_076_076_SOLAR_BEAM_B_PARTY, W_076_076_SOLAR_BEAM_B_PARTY, W_076_076_SOLAR_BEAM_B_PARTY, EMTFUNC_ATTACK_POS
	ADD_PARTICLE_PTAT	0, W_076_076_BALL_BEAM_PARTY1,	 W_076_076_BALL_BEAM_PARTY2,   W_076_076_BALL_BEAM_PARTY1,   W_076_076_BALL_BEAM_PARTY2,   EMTFUNC_ATTACK_POS
	
	WAIT			10
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_B_X, 0, SHAKE_B_WAIT, SHAKE_NUM_LONG*3,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	WAIT_FLAG
	
//	WORK_CLEAR
//	HAIKEI_RECOVER	BG_ID_076, WEDEF_HAIKEI_RC_MODE_FADE | WEDEF_HAIKEI_SBIT_NONE
//	HAIKEI_CHG_WAIT
	FUNC_CALL		WEST_SP_WE_HAIKEI_PAL_FADE, 5, 0, 1, 12, 0, WE_PAL_BLACK,
	WAIT_FLAG
	
	SEQEND

///< �R���e�X�g
WEST_CONTEST:

	WORK_CLEAR
	HAIKEI_CHG		BG_ID_076, WEDEF_HAIKEI_MODE_FADE | WEDEF_HAIKEI_SBIT_NONE
	HAIKEI_CHG_WAIT
	
	ADD_PARTICLE	0, W_076_076_SOLAR_BEAM_CNT,	EMTFUNC_ATTACK_POS
	ADD_PARTICLE	0, W_076_076_SOLAR_BEAM_A_CNT,	EMTFUNC_ATTACK_POS	
	ADD_PARTICLE	0, W_076_076_SOLAR_BEAM_B_CNT,	EMTFUNC_ATTACK_POS		
	
	SE_FLOW_LR		SEQ_SE_DP_W076B
	WAIT			10
	
	FUNC_CALL		WEST_SP_WT_SHAKE, 5, SHAKE_B_X, 0, SHAKE_B_WAIT, SHAKE_NUM_LONG*3,  WE_TOOL_E1 | WE_TOOL_SSP,

	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	WAIT_FLAG
	
	WORK_CLEAR
	HAIKEI_RECOVER	BG_ID_076, WEDEF_HAIKEI_RC_MODE_FADE | WEDEF_HAIKEI_SBIT_NONE
	HAIKEI_CHG_WAIT
	
	SEQEND
*/