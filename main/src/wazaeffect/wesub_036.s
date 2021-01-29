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

	.global		WEST_Hataku
	
// =============================================================================
//
//
//	■はたく
//
//
// =============================================================================
#define WE463_RX	(64)
#define WE463_RY	(48)
#define WE463_WAIT1	(40)
#define WE463_WAIT2	(40)
#define WE463_RAD_B	(360 / 5)
#define WE463_RAD_S(n)	(  0 + (n * WE463_RAD_B))
#define WE463_RAD_E(n)	(360 + (n * WE463_RAD_B))
#define WE463_PARA_NUM	(10)
#define WE463_TARGET	(0)	///< 0 = 自分

WEST_Hataku:

	LOAD_PARTICLE_DROP		0,W_463_SPA
	LOAD_PARTICLE_DROP		1,W_463_SPA
	LOAD_PARTICLE_DROP		2,W_463_SPA

	WORK_CLEAR
	WORK_SET		WEDEF_GP_INDEX_FADE_TYPE,  1
	WORK_SET		WEDEF_GP_INDEX_SPEED_X,  0
	WORK_SET		WEDEF_GP_INDEX_SPEED_Y,  32
	HAIKEI_CHG		BG_ID_463, WEDEF_HAIKEI_MODE_FADE | WEDEF_HAIKEI_SBIT_MOVE
	
	HAIKEI_CHG_WAIT
	
	SE_L			SEQ_SE_DP_W463
	
	///< １
	ADD_PARTICLE_EMIT_SET		0, 0, W_463_463_FIRE_SCREW01,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 0, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 0
	ADD_PARTICLE_EMIT_SET		0, 1, W_463_463_FIRE_SCREW02,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 1, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT1, WE463_TARGET, 0
	ADD_PARTICLE_EMIT_SET		0, 2, W_463_463_RING_UP,			EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 2, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 0
	WAIT						5	
	
	FUNC_CALL		WEST_SP_WE_SSP_POKE_PAL_FADE, 6,  WE_TOOL_M1, 0, 3, WE_PAL_RED, WEDEF_FADE_PARAM, 10

	///< ２
	ADD_PARTICLE_EMIT_SET		0, 3, W_463_463_FIRE_SCREW01,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 3, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 0
	ADD_PARTICLE_EMIT_SET		0, 4, W_463_463_FIRE_SCREW02,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 4, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT1, WE463_TARGET, 0
	ADD_PARTICLE_EMIT_SET		0, 5, W_463_463_RING_UP,			EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 5, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 0
	WAIT						5	

	///< ３
	ADD_PARTICLE_EMIT_SET		1, 0, W_463_463_FIRE_SCREW01,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 0, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 1
	ADD_PARTICLE_EMIT_SET		1, 1, W_463_463_FIRE_SCREW02,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 1, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT1, WE463_TARGET, 1
	ADD_PARTICLE_EMIT_SET		1, 2, W_463_463_RING_UP,			EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 2, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 1
	WAIT						5	

	///< ４
	ADD_PARTICLE_EMIT_SET		1, 3, W_463_463_FIRE_SCREW01,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 3, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 1
	ADD_PARTICLE_EMIT_SET		1, 4, W_463_463_FIRE_SCREW02,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 4, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT1, WE463_TARGET, 1
	ADD_PARTICLE_EMIT_SET		1, 5, W_463_463_RING_UP,			EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 5, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 1
	WAIT						5	

	///< ５
	ADD_PARTICLE_EMIT_SET		2, 0, W_463_463_FIRE_SCREW01,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 0, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 2
	ADD_PARTICLE_EMIT_SET		2, 1, W_463_463_FIRE_SCREW02,		EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 1, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT1, WE463_TARGET, 2
	ADD_PARTICLE_EMIT_SET		2, 2, W_463_463_RING_UP,			EMTFUNC_ATTACK_POS
	FUNC_CALL					WEST_SP_EMIT_ROTATION, WE463_PARA_NUM, 2, WE463_RAD_S(0), WE463_RAD_E(0), WE463_RAD_S(0), WE463_RAD_E(0), WE463_RX, WE463_RY, WE463_WAIT2, WE463_TARGET, 2
	WAIT						5	
	
	WAIT_PARTICLE
	EXIT_PARTICLE		0,
	EXIT_PARTICLE		1,
	EXIT_PARTICLE		2,
	
	WORK_CLEAR
	WORK_SET		WEDEF_GP_INDEX_FADE_TYPE,  1
	WORK_SET		WEDEF_GP_INDEX_SPEED_X,  0
	WORK_SET		WEDEF_GP_INDEX_SPEED_Y,  32
	HAIKEI_RECOVER	BG_ID_463, WEDEF_HAIKEI_RC_MODE_FADE | WEDEF_HAIKEI_SBIT_STOP
	HAIKEI_CHG_WAIT
	SEQEND

