
//============================================================================
/**
 *
 *@file		sub_296.s
 *@brief	�퓬�V�[�P���X
 *			�퓬���ɃM���e�B�i�t�H�����`�F���W
 *@author	HisashiSogabe
 *@data		2008.05.16
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

SUB_296:
	//�|�P�����p�����[�^�̍Čv�Z���w��
	VALUE				VAL_BIT,BUF_PARA_SERVER_STATUS_FLAG2,SERVER_STATUS_FLAG2_POKEPARA_CALC
	PSP_VALUE			VAL_SET,SIDE_CLIENT_WORK,ID_PSP_form_no,FORMNO_GIRATINA_NORMAL
	VALUE				VAL_SET,BUF_PARA_TEMP_WORK,0
	IF_PSP				IF_FLAG_BIT,SIDE_CLIENT_WORK,ID_PSP_personal_rnd,1,GiratinaSpeabi2
GiratinaSpeabi1:
	PERSONAL_DATA_GET	MONSNO_KIMAIRAN,BUF_PARA_TEMP_WORK,ID_PER_speabi1
	BRANCH				GiratinaSpeabiSet
GiratinaSpeabi2:
	PERSONAL_DATA_GET	MONSNO_KIMAIRAN,BUF_PARA_TEMP_WORK,ID_PER_speabi2
GiratinaSpeabiSet:
	PSP_VALUE_WORK		VAL_SET,SIDE_CLIENT_WORK,ID_PSP_speabino,BUF_PARA_CALC_WORK
	GOSUB				SUB_SEQ_TENKIYA
	//�Čv�Z���ꂽ�p�����[�^�������[�h
	POKEPARA_RELOAD		SIDE_CLIENT_WORK
	SEQ_END
