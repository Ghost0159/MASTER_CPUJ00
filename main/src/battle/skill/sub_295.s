
//============================================================================
/**
 *
 *@file		sub_295.s
 *@brief	�퓬�V�[�P���X
 *			�t�B�[���h�V��Ńg���b�N���[��
 *@author	HisashiSogabe
 *@data		2007.09.11
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

SUB_295:
	VALUE			VAL_SET,BUF_PARA_WAZA_WORK,WAZANO_TORIKKURUUMU
	WAZA_EFFECT		SIDE_WORK
	SERVER_WAIT
	//�G�t�F�N�g�J�E���g�����ɖ߂�
	VALUE			VAL_SET,BUF_PARA_WAZA_EFF_CNT,0
	//�Z�G�t�F�N�g���N������悤�Ƀt���O�𗎂Ƃ�
	VALUE			VAL_NBIT,BUF_PARA_SERVER_STATUS_FLAG,SERVER_STATUS_FLAG_NO_WAZA_EFFECT
	MESSAGE			FieldTrickRoomMsg,TAG_NONE
	SERVER_WAIT
	WAIT			MSG_WAIT
	VALUE			VAL_BIT,BUF_PARA_FIELD_CONDITION,TRICKROOM_COUNT<<FIELD_TRICKROOM_SHIFT
	SEQ_END
