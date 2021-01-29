//============================================================================================
/**
 * @file	frontier_def.h
 * @bfief	�u�t�����e�B�A�v���ʒ�`
 * @author	Satoshi Nohara
 * @date	07.07.31
 */
//============================================================================================
#ifndef	__FRONTIER_DEF_H__
#define	__FRONTIER_DEF_H__


//==============================================================================
//	�f�o�b�N��`
//==============================================================================
//�X�^�[�g�{�^���������ƁA�`�{�^���������ςȂ���ԂɂȂ�܂�
//�|�P�������X�g�I�����I��������A�X�^�[�g�{�^���������Ă�������
//#define DEBUG_FRONTIER_LOOP					//��`�L���œ������E���h�����[�v


//============================================================================================
//
//	�S�{�݁@���ʒ�`(enum�֎~)
//
//============================================================================================
#define FRONTIER_COMM_PLAYER_NUM		(2)			//�ʐM�l��
#define FRONTIER_COMM_POKE_NUM			(2)			//�ʐM�|�P������

#define WIFI_COUNTER_ENTRY_POKE_MAX		(2)			//�Q��

//�ʐM�^�C�v
#define FRONTIER_COMM_SEL				(0)
#define FRONTIER_COMM_YAMERU			(1)
#define FRONTIER_COMM_MONSNO_ITEMNO		(2)
#define FRONTIER_COMM_STAGE_RECORD_DEL	(4)
#define FRONTIER_COMM_GAME_CONTINUE		(5)

//�{�݃^�C�v
#define FRONTIER_NO_NONE				(0)
#define FRONTIER_NO_TOWER				(1)
#define FRONTIER_NO_FACTORY_LV50		(2)
#define FRONTIER_NO_FACTORY_LV100		(3)
#define FRONTIER_NO_CASTLE				(4)			//���Ӂ@�L���b�X���A�X�e�[�W�̏���
#define FRONTIER_NO_STAGE				(5)
#define FRONTIER_NO_ROULETTE			(6)
#define FRONTIER_NO_YAMERU				(7)

//�|�P�������X�g�A�X�e�[�^�X
#define FR_WIFI_POKESEL_PLIST_CALL		(0)				//�|�P�������X�g�Ăяo��
#define FR_WIFI_POKESEL_PLIST_WAIT		(1)				//�|�P�������X�g�I���҂�
#define FR_WIFI_POKESEL_PST_CALL		(2)				//�|�P�����X�e�[�^�X�Ăяo��
#define FR_WIFI_POKESEL_PST_WAIT		(3)				//�|�P�����X�e�[�^�X�I���҂�
#define FR_WIFI_POKESEL_EXIT			(4)				//�I��

//�G���J�E���g�G�t�F�N�g
#define FR_ENCOUNT_EFF_WIPE				(0)
#define FR_ENCOUNT_EFF_BOX_D			(1)
#define FR_ENCOUNT_EFF_BOX_LR			(2)
#define FR_ENCOUNT_EFF_CLOSE			(3)
#define FR_ENCOUNT_EFF_ZIGZAG			(4)

//FRWIFI_SCRWORK�̎擾�R�[�h(wifi_counter.s�Ŏg�p)
#define FWIFI_ID_COMM_COMMAND_INITIALIZE	(0)
#define FWIFI_ID_GET_PAIR_POKELIST_CANCEL	(1)
#define FWIFI_ID_CHECK_ENTRY				(2)
#define FWIFI_ID_STAGE_GET_CLEAR_FLAG		(3)
#define FWIFI_ID_STAGE_GET_CLEAR_MONSNO		(4)
#define FWIFI_ID_GET_ENTRY_MONSNO			(5)
#define FWIFI_ID_STAGE_SET_NEW_CHALLENGE	(6)
#define FWIFI_ID_SET_BF_NO					(7)
#define FWIFI_ID_GET_PAIR_STAGE_RECORD_DEL	(8)
#define FWIFI_ID_GET_BF_NO					(9)
#define FWIFI_ID_GET_PAIR_GAME_CONTINUE		(10)
#define FWIFI_ID_CHECK_PAIR_RENSYOU			(11)
#define FWIFI_ID_GET_CLEAR_FLAG				(12)
#define FWIFI_ID_POKELIST_SEL_CLEAR			(13)

//�S�{�݂̋��ʃE�F�C�g
#define FRONTIER_COMMON_WAIT				(30)

//�g���[�i�[AI(src\battle\tr_ai_def.h�Q��)
#define FR_AI_YASEI		(0)														//�쐶��(��{AI)
#define FR_AI_BASIC		(AI_THINK_BIT_BASIC)									//��{AI(�U���^AI)
#define FR_AI_EXPERT	(AI_THINK_BIT_BASIC | AI_THINK_BIT_ATTACK | AI_THINK_BIT_EXPERT)//expertAI

//�t�����e�B�A�̃Z�[�u���[�h
#define FR_MODE_CLEAR	(0)					//�N���A
#define FR_MODE_LOSE	(1)					//����
#define FR_MODE_REST	(2)					//�x��


//--------------------------------------------------------------------
//					     �u���[���o��A����
//
//�^���[�́Asrc/field/b_tower_fld.c�ɂ���
//--------------------------------------------------------------------
#define FACTORY_LEADER_SET_1ST		(21)
#define FACTORY_LEADER_SET_2ND		(49)

#define STAGE_LEADER_SET_1ST		(50)
#define STAGE_LEADER_SET_2ND		(170)

#define ROULETTE_LEADER_SET_1ST		(21)
#define ROULETTE_LEADER_SET_2ND		(49)

#define CASTLE_LEADER_SET_1ST		(21)
#define CASTLE_LEADER_SET_2ND		(49)


//--------------------------------------------------------------------
//			     �u���[���̃g���[�i�[�C���f�b�N�X
//--------------------------------------------------------------------
#define STAGE_LEADER_TR_INDEX_1ST	(307)	//100���(0�I���W��)
#define STAGE_LEADER_TR_INDEX_2ND	(308)	//170���


//--------------------------------------------------------------------
//			     �t�����e�B�A�̋L�O�v�����g�̒�`
//--------------------------------------------------------------------
#define MEMORY_PRINT_NONE			(0)		//�Ȃɂ��Ȃ�
#define MEMORY_PRINT_PUT_1ST		(1)		//1��ڂ̃u���[���ɏ������ċL�O�v�����g�����������
#define MEMORY_PRINT_PUT_OK_1ST		(2)		//1��ڂ̃u���[���ɏ��������L�O�v�����g��������Ă���
#define MEMORY_PRINT_PUT_2ND		(3)		//2��ڂ̃u���[���ɏ������ċL�O�v�����g�����������
#define MEMORY_PRINT_PUT_OK_2ND		(4)		//2��ڂ̃u���[���ɏ��������L�O�v�����g��������Ă���


#endif	//__FRONTIER_DEF_H__


