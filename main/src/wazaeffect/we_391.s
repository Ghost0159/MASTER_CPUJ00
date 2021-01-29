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

	.global		WEST_Hataku
	
// =============================================================================
//
//
//	���͂���
//
//
// =============================================================================
#define W391_LIFE	(12)
#define W391_WAIT	(W391_LIFE / 4)

WEST_Hataku:

	LOAD_PARTICLE_DROP	0,W_391_SPA
	LOAD_PARTICLE_DROP	1,W_391_SPA

	//SE_REPEAT		SEQ_SE_DP_W179, WAZA_SE_PAN_AT, 20, 6
	SE_WAITPLAY_C		SEQ_SE_DP_W179,1
	SE_WAITPLAY_C		SEQ_SE_DP_W179,15
	SE_WAITPLAY_C		SEQ_SE_DP_W179,30
	SE_WAITPLAY_C		SEQ_SE_DP_W179,45
	SE_WAITPLAY_C		SEQ_SE_DP_W179,60
	SE_WAITPLAY_C		SEQ_SE_DP_W179,75
	SE_WAITPLAY_C		SEQ_SE_DP_293,86

	LOOP_LABEL		3

		ADD_PARTICLE_EMIT_SET 	0, 0, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 0, 0, 0, 0, W391_LIFE, +32, 1,
		
		WAIT					W391_WAIT		
		ADD_PARTICLE_EMIT_SET 	0, 1, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 1, 0, 0, 0, W391_LIFE, +32, 1,
		
		WAIT					W391_WAIT		
		ADD_PARTICLE_EMIT_SET 	0, 2, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 2, 0, 0, 0, W391_LIFE, +32, 1,
		
		WAIT					W391_WAIT		
		ADD_PARTICLE_EMIT_SET 	0, 3, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 3, 0, 0, 0, W391_LIFE, +32, 1,

		ADD_PARTICLE_EMIT_SET 	0, 4, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 4, 0, 0, 0, W391_LIFE, -32, 0,
		
		WAIT					W391_WAIT		
		ADD_PARTICLE_EMIT_SET 	0, 5, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 5, 0, 0, 0, W391_LIFE, -32, 0,
		
		WAIT					W391_WAIT		
		ADD_PARTICLE_EMIT_SET 	0, 6, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 6, 0, 0, 0, W391_LIFE, -32, 0,
		
		WAIT					W391_WAIT		
		ADD_PARTICLE_EMIT_SET 	0, 7, W_391_391_HARTSWAP_RING1, EMTFUNC_ATTACK_POS
		FUNC_CALL				WEST_SP_EMIT_PARABOLIC, 7, 7, 0, 0, 0, W391_LIFE, -32, 0,
		
	LOOP

	ADD_PARTICLE 	0,W_391_391_HARTSWAP_RING2, EMTFUNC_ATTACK_POS
	ADD_PARTICLE 	0,W_391_391_HARTSWAP_RING3, EMTFUNC_ATTACK_POS	
	ADD_PARTICLE 	0,W_391_391_HARTSWAP_BALL,	EMTFUNC_ATTACK_POS

	ADD_PARTICLE 	0,W_391_391_HARTSWAP_RING2, EMTFUNC_DEFENCE_POS
	ADD_PARTICLE 	0,W_391_391_HARTSWAP_RING3, EMTFUNC_DEFENCE_POS	
	ADD_PARTICLE 	0,W_391_391_HARTSWAP_BALL,	EMTFUNC_DEFENCE_POS
	
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_M1, 0, 1, WE_PAL_PINK, WEDEF_FADE_PARAM, 15
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_E1, 0, 1, WE_PAL_PINK, WEDEF_FADE_PARAM, 15
	
	WAIT_PARTICLE
	EXIT_PARTICLE	0,
	EXIT_PARTICLE	1,
	
	SEQEND
