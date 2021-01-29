
//============================================================================
/**
 *
 *@file		sub_281.s
 *@brief	�퓬�V�[�P���X�iBattleEffect�j
 *			�U���Ώۂ����Ȃ��ē�����Ȃ�
 *
 *@author	HisashiSogabe
 *@data		2006.07.04
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

SUB_281:
	ATTACK_MESSAGE
	SERVER_WAIT
	WAIT			MSG_WAIT
	MESSAGE			NoPokemonMsg,TAG_NONE
	SERVER_WAIT
	WAIT			MSG_WAIT
	//���ߌn�t���O�𗎂Ƃ�
	KEEP_OFF		SIDE_ATTACK
	//�������Ԃł�������o���Ă��Ȃ��Ƃ��͂�����t���O�𗎂Ƃ�
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_NO_NOW,WAZANO_IKARI,SUB_281_END
	IF_PSP			IF_FLAG_NBIT,SIDE_ATTACK,ID_PSP_condition2,CONDITION2_IKARI,SUB_281_END
	PSP_VALUE		VAL_NBIT,SIDE_ATTACK,ID_PSP_condition2,CONDITION2_IKARI
SUB_281_END:
	SEQ_END
