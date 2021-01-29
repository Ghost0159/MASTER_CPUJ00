//==============================================================================
/**
 * @file	poke_anm011.s
 * @brief	���Ƃт͂ˁ@�X���[
 * @author	saito
 * @date	
 *
 */
//==============================================================================

	.text
	
	.include	"past.h"
	.include	"past_def.h"
	
	.global		PAST_ANIME_PLB011
	
// =============================================================================
//
//
//	���Ƃт͂ˁ@�X���[
//
//
// =============================================================================
#define OFS_Y	( 2 )
#define ROT_1	( 0x2000 )
#define ROT_2	( 0x2000 )
#define ROT_3	( 0x1000 )
PAST_ANIME_PLB011:
	SET_ADD_PARAM		PARAM_Y,USE_VAL, 2,PARAM_SET
	SET_ADD_PARAM		PARAM_DY,USE_VAL,0,PARAM_SET
		
	//>��ђ���1���
	//8��̏�����ROT_1�i180���j������
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,8
	//8��̏�����8�ړ�
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,8,8
	//8��̏�����0x1000(22.5��)�X��
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_ROT,ROT_3,8
	HOLD_CMD
	
	//>��ђ���2�`4���
	START_LOOP	2

		//�X�v���C�g�ʒu��x=8�ɐݒ� dx��0�ɐݒ�
		SET_ADD_PARAM		PARAM_X,USE_VAL,8,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		APPLY_TRANS
		//�X�v���C�g��]��0x1000�ɃZ�b�g
		SET_ADD_PARAM		PARAM_ROT,USE_VAL,ROT_3,PARAM_SET
		APPLY_AFFINE
	
		//12��̏�����ROT_1�i180���j������
		CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,12
		//12��̏�����-16�ړ�
		CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,-16,12
		//12��̏�����0x1000�`-0x1000��-0x2000(45��)���X��
		CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,-ROT_2,12
		HOLD_CMD

		//�X�v���C�g�ʒu��x=-8�ɐݒ� dx��0�ɐݒ�
		SET_ADD_PARAM		PARAM_X,USE_VAL,-8,PARAM_SET
		SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
		APPLY_TRANS
		//�X�v���C�g��]��-0x1000�ɃZ�b�g
		SET_ADD_PARAM		PARAM_ROT,USE_VAL,-ROT_3,PARAM_SET
		APPLY_AFFINE

		//12��̏�����ROT_1�i180���j������
		CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,12
		//12��̏�����16�ړ�
		CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,16,12
		//12��̏�����-0x1000�`0x1000��0x2000(45��)���X��
		CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,ROT_2,12
		HOLD_CMD

	END_LOOP

	//�X�v���C�g�ʒu��x=8�ɐݒ� dx��0�ɐݒ�
	SET_ADD_PARAM		PARAM_X,USE_VAL,8,PARAM_SET
	SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
	APPLY_TRANS
	//�X�v���C�g��]��0x1000�ɃZ�b�g
	SET_ADD_PARAM		PARAM_ROT,USE_VAL,ROT_3,PARAM_SET
	APPLY_AFFINE

	//>��ђ���5���
	//12��̏�����ROT_1�i180���j������
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,12
	//12��̏�����-16�ړ�
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,-16,12
	//12��̏�����0x1000�`-0x1000��-0x2000(45��)���X��
	CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,-ROT_2,12
	HOLD_CMD

	//�X�v���C�g�ʒu��x=-8�ɐݒ� dx��0�ɐݒ�
	SET_ADD_PARAM		PARAM_X,USE_VAL,-8,PARAM_SET
	SET_ADD_PARAM		PARAM_DX,USE_VAL,0,PARAM_SET
	APPLY_TRANS
	//�X�v���C�g��]��-0x1000�ɃZ�b�g
	SET_ADD_PARAM		PARAM_ROT,USE_VAL,-ROT_3,PARAM_SET
	APPLY_AFFINE
	
	//>��ђ���6���
	//12��̏�����ROT_1�i180���j������
	CALL_MF_CURVE_DIVTIME	APPLY_SET,0,CURVE_SIN_MINUS,TARGET_DY,OFS_Y,ROT_1,0,12
	//12��̏�����8�ړ�
	CALL_MF_LINE_DIVTIME	APPLY_SET,0,TARGET_DX,8,12
	//12��̏�����0x1000(22.5��)�X��
	CALL_MF_LINE_DIVTIME	APPLY_ADD,0,TARGET_ROT,ROT_3,12
	HOLD_CMD
	
	
	END_ANM
