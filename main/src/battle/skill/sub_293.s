
//============================================================================
/**
 *
 *@file		sub_293.s
 *@brief	�퓬�V�[�P���X
 *			��������ɂ���ăt�H�����`�F���W������|�P�����̃`�F�b�N
 *@author	HisashiSogabe
 *@data		2007.04.17
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

SUB_293:
	//�Z���󂯂��N���C�A���g�����݂��Ȃ��Ȃ疳��
	IF					IF_FLAG_EQ,BUF_PARA_DEFENCE_CLIENT,NONE_CLIENT_NO,SUB_293_END
	//�Z���󂯂��|�P�������V�F�C�~����Ȃ��Ȃ疳��
	IF_PSP				IF_FLAG_NE,SIDE_DEFENCE,ID_PSP_monsno,MONSNO_EURISU,SUB_293_END
	//���łɃt�H�����i���o�[���O�������疳��
	IF_PSP				IF_FLAG_EQ,SIDE_DEFENCE,ID_PSP_form_no,0,SUB_293_END
	//�g���肪�o�Ă���Ƃ��͖���
	IF_PSP				IF_FLAG_BIT,SIDE_DEFENCE,ID_PSP_condition2,CONDITION2_MIGAWARI,SUB_293_END
	//�X�^�C�v�̋Z���󂯂ē�������A�t�H�����`�F���W�i�P���O�j
	IF_PSP				IF_FLAG_NE,SIDE_DEFENCE,ID_PSP_condition,CONDITION_KOORI,SUB_293_END
SheimiFormChg:
	PSP_VALUE			VAL_SET,SIDE_DEFENCE,ID_PSP_form_no,0
	VALUE				VAL_SET,BUF_PARA_TEMP_WORK,0
	IF_PSP				IF_FLAG_BIT,SIDE_DEFENCE,ID_PSP_personal_rnd,1,SheimiSpeabi2
SheimiSpeabi1:
	PERSONAL_DATA_GET	MONSNO_EURISU,BUF_PARA_TEMP_WORK,ID_PER_speabi1
	BRANCH				SheimiSpeabiSet
SheimiSpeabi2:
	PERSONAL_DATA_GET	MONSNO_EURISU,BUF_PARA_TEMP_WORK,ID_PER_speabi2
	IF					IF_FLAG_NE,BUF_PARA_TEMP_WORK,0,SheimiSpeabiSet
	PERSONAL_DATA_GET	MONSNO_EURISU,BUF_PARA_TEMP_WORK,ID_PER_speabi1
SheimiSpeabiSet:
	//�|�P�����p�����[�^�̍Čv�Z���w��
	VALUE				VAL_BIT,BUF_PARA_SERVER_STATUS_FLAG2,SERVER_STATUS_FLAG2_POKEPARA_CALC
	PSP_VALUE_WORK		VAL_SET,SIDE_DEFENCE,ID_PSP_speabino,BUF_PARA_CALC_WORK
	PERSONAL_DATA_GET	MONSNO_EURISU,BUF_PARA_TEMP_WORK,ID_PER_type1
	PSP_VALUE_WORK		VAL_SET,SIDE_DEFENCE,ID_PSP_type1,BUF_PARA_CALC_WORK
	PERSONAL_DATA_GET	MONSNO_EURISU,BUF_PARA_TEMP_WORK,ID_PER_type2
	PSP_VALUE_WORK		VAL_SET,SIDE_DEFENCE,ID_PSP_type2,BUF_PARA_CALC_WORK

	VALUE_WORK			VAL_SET,BUF_PARA_CLIENT_WORK,BUF_PARA_DEFENCE_CLIENT
	GOSUB				SUB_SEQ_TENKIYA

	//�Čv�Z���ꂽ�p�����[�^�������[�h
	POKEPARA_RELOAD		SIDE_DEFENCE
SUB_293_END:
	SEQ_END
