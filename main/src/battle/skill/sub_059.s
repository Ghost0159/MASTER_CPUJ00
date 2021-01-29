
//============================================================================
/**
 *
 *@file		sub_059.s
 *@brief	�퓬�V�[�P���X
 *			���߂��p���V�[�P���X
 *@author	HisashiSogabe
 *@data		2006.01.19
 *
 */
//============================================================================
	.text

	.include	"waza_seq_def.h"

SUB_059:
	//�����}�W�b�N�K�[�h�́A�󂯂Ȃ�
	TOKUSEI_CHECK	TOKUSEI_HAVE,SIDE_WORK,TOKUSYU_MAZIKKUGAADO,SUB_059_END
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_WORK,WAZANO_MAKITUKU,MakiEffect
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_WORK,WAZANO_HONOONOUZU,HonouEffect
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_WORK,WAZANO_MAGUMASUTOOMU,MagumaEffect
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_WORK,WAZANO_KARADEHASAMU,KaraEffect
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_WORK,WAZANO_UZUSIO,UzuEffect
	IF				IF_FLAG_EQ,BUF_PARA_WAZA_WORK,WAZANO_SUNAZIGOKU,SunaEffect

ShimeEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_SIMETUKE
	BRANCH			SUB_059_NEXT

MakiEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_MAKITUKU
	BRANCH			SUB_059_NEXT

HonouEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_HONOO
	BRANCH			SUB_059_NEXT

MagumaEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_MAGUMA
	BRANCH			SUB_059_NEXT

KaraEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_KARA
	BRANCH			SUB_059_NEXT
	
UzuEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_UZUSIO
	BRANCH			SUB_059_NEXT

SunaEffect:
	STATUS_EFFECT	SIDE_WORK,STATUS_DAMAGE_SUNA
	
SUB_059_NEXT:
	SERVER_WAIT
	MESSAGE			ShimeKeizokuMineMsg,TAG_NICK_WAZA,SIDE_WORK,SIDE_WORK
	SERVER_WAIT
	WAIT			MSG_WAIT
	VALUE			VAL_BIT,BUF_PARA_SERVER_STATUS_FLAG,SERVER_STATUS_FLAG_NO_BLINK
	JUMP			SUB_SEQ_HP_CALC
SUB_059_END:
	SEQ_END