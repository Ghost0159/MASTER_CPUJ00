
//============================================================================
/**
 *
 *@file		sub_011.s
 *@brief	�퓬�V�[�P���X
 *			�|�P�����ߊl�V�[�P���X
 *@author	HisashiSogabe
 *@data		2005.07.20
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

SUB_011:
	MESSAGE				ItemUseMsg,TAG_TRNAME_ITEM,SIDE_ATTACK,SIDE_WORK
	SERVER_WAIT
	POKEMON_GET_INIT	GET_NORMAL
	POKEMON_GET
#if AFTER_MASTER_070409_60_EUR_FIX
//�ԈႦ��4�o�C�g�̃T�C�Y�ł͂Ȃ��o�b�t�@��ǂ߂�悤�ɂ��Ă���̂ŁA32bit�̃��[�N�ɃR�s�[������A0xff�Ń}�X�N
	VALUE_WORK			VAL_GET,BUF_PARA_WIN_LOSE_FLAG,BUF_PARA_TEMP_WORK
	VALUE				VAL_AND,BUF_PARA_TEMP_WORK,0x000000ff
	IF					IF_FLAG_NE,BUF_PARA_TEMP_WORK,FIGHT_POKE_GET,SUB_011_END
	SIZENKAIHUKU_CHECK	SIDE_MINE,SUB_011_END
	PSP_VALUE			VAL_SET,SIDE_MINE,ID_PSP_condition,0
#endif AFTER_MASTER_070409_60_EUR_FIX
SUB_011_END:
	SEQ_END
