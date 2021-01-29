//============================================================================
/**
 *
 *@file		be_107.s
 *@brief	�퓬�V�[�P���X�iBattleEffect�j
 *			107�@�G���u�˂ނ�v��Ԃ̎��������ʂ̂���Z�i�u�˂ނ�v��ԂłȂ��ƋZ�͎��s����j�A
 *				 �G�|�P���������ޏ�Ԃɂ���	
 *
 *@author	HisashiSogabe
 *@data		2006.01.27
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

BE_107:
#if AFTER_MASTER_070405_BT_FIX
	//�݂���肪�łĂ�����A���܂����܂��
	MIGAWARI_CHECK	SIDE_DEFENCE,Umakukimaran
	//���łɂ����ނɂ������Ă�����A���܂����܂��
	IF_PSP			IF_FLAG_BIT,SIDE_DEFENCE,ID_PSP_condition2,CONDITION2_AKUMU,Umakukimaran
	//�˂ނ��Ă��Ȃ�������A���܂����܂��
	IF_PSP			IF_FLAG_NBIT,SIDE_DEFENCE,ID_PSP_condition,CONDITION_NEMURI,Umakukimaran
#endif //AFTER_MASTER_070405_BT_FIX
	VALUE		VAL_SET,BUF_PARA_ADD_STATUS_DIRECT,ADD_COND2_AKUMU|ADD_STATUS_WAZAKOUKA
	SEQ_END

#if AFTER_MASTER_070405_BT_FIX
Umakukimaran:
	VALUE		VAL_BIT,BUF_PARA_WAZA_STATUS_FLAG,WAZA_STATUS_FLAG_UMAKUKIMARAN
	SEQ_END
#endif //AFTER_MASTER_070405_BT_FIX
