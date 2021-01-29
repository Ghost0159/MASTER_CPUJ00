//============================================================================================
/**
 * @file	stage_def.h
 * @bfief	�u�o�g���X�e�[�W�v��`
 * @author	Satoshi Nohara
 * @date	07.03.29
 */
//============================================================================================
#ifndef	__STAGE_DEF_H__
#define	__STAGE_DEF_H__


//============================================================================================
//
//	��`(enum�֎~)
//
//============================================================================================
#define STAGE_LAP_ENEMY_MAX			(10)						//1�T���̓G�g���[�i�[�̍ő吔
//#define STAGE_LAP_MAX				(10)						///�X�e�[�W�̍ő����
#define STAGE_LAP_MAX				(18)						///�X�e�[�W�̍ő����
#define STAGE_LAP_MULTI_ENEMY_MAX	(STAGE_LAP_ENEMY_MAX*2)		//1�T����2�l�̓G�g���[�i�[�̍ő吔
#define STAGE_TR_TYPE_MAX			(18)						//????���܂�
#define STAGE_TYPE_LEVEL_MAX		(10)						//�^�C�v���x���̍ő�

#define STAGE_ENTRY_POKE_MAX		(2)							//�Q���|�P�������V���O��=1 �_�u��=2

//�^�C�v�̕���
#define TYPE_X_MAX					(4)							//���ɕ��Ԑ�
#define TYPE_Y_MAX					(5)							//�c�ɕ��Ԑ�
#define DUMMY_TYPE					(0xfe)						//��������|�P�����^�C�v
#define TYPE_XY_MAX					(TYPE_X_MAX * TYPE_Y_MAX)

//�^�C�v
#define STAGE_TYPE_SINGLE		(0)
#define STAGE_TYPE_DOUBLE		(1)
#define STAGE_TYPE_MULTI		(2)
#define STAGE_TYPE_WIFI_MULTI	(3)
#define STAGE_TYPE_MAX			(4)								//�ő吔

//�g���[�i�[�^�C�v�̃f�[�^�e�[�u����`
#define STAGE_RANK_TR1_NUM		(8)								//
#define STAGE_RANK_TR2_NUM		(4)								//��p
#define STAGE_RANK_TR_MAX		(STAGE_RANK_TR1_NUM+STAGE_RANK_TR2_NUM)

/////////////////////////////////////////////////////////////////////////////////////////
//STAGE_SCRWORK�̎擾�R�[�h(stage.s�Ŏg�p)
#define FS_ID_SET_MODE				(0)
#define FS_ID_SET_LEVEL				(1)
#define FS_ID_SET_TYPE				(2)
#define FS_ID_GET_RET_WORK			(3)
#define FS_ID_GET_RENSYOU			(4)
#define FS_ID_INC_RENSYOU			(5)
#define FS_ID_INC_TRADE_COUNT		(6)
#define FS_ID_SYSTEM_RESET			(7)
//#define FS_ID_CLEAR_PLAY_DATA		(8)
#define FS_ID_IS_SAVE_DATA_ENABLE	(9)
#define FS_ID_SAVE_REST_PLAY_DATA	(10)
#define FS_ID_PUSH_NOW_LOCATION		(11)
#define FS_ID_POP_NOW_LOCATION		(12)
//#define FS_ID_SET_NG				(13)
#define FS_ID_INC_ROUND				(14)
#define FS_ID_GET_MONSNO			(15)
#define FS_ID_GET_WAZANO			(16)
#define FS_ID_GET_TYPE				(17)
#define FS_ID_GET_LARGE_TYPE		(18)
#define FS_ID_GET_LAP				(19)
#define FS_ID_GET_TR_OBJ_CODE		(20)
#define FS_ID_SET_LOSE				(21)
#define FS_ID_SET_CLEAR				(22)
#define FS_ID_GET_ROUND				(23)
#define FS_ID_GET_RETIRE_FLAG		(24)
//#define FS_ID_SET_PAIR_NAME			(25)
#define FS_ID_GET_TRADE_YESNO_FLAG	(26)
#define FS_ID_SET_TEMP_ITEM			(27)
#define FS_ID_INC_TYPE_LEVEL		(28)
#define FS_ID_COMM_COMMAND_INITIALIZE	(29)
#define FS_ID_CHECK_COMM_TYPE		(30)
#define FS_ID_EFF_FRAME_ON			(31)
#define	FS_ID_SET_B_TOWER_PARTNER_DATA	(32)
#define FS_ID_GET_TYPE_LEVEL		(33)
#define	FS_ID_GET_BP_POINT			(34)
#define	FS_ID_TYPE_LEVEL_MAX		(35)
#define FS_ID_SQRT					(36)
#define FS_ID_CHECK_BRAIN			(37)
#define FS_ID_WND_SET				(38)

//d32r0401.ev�Ŏg�p
#define STAGE_TOOL_CHK_ENTRY_POKE_NUM	(0)
#define STAGE_TOOL_GET_CLEAR_FLAG		(1)
#define STAGE_TOOL_GET_CLEAR_MONSNO		(2)
#define STAGE_TOOL_SET_NEW_CHALLENGE	(3)
#define STAGE_TOOL_SELECT_POKE			(4)

//�ʐM�o�b�t�@�̃T�C�Y(send_buf,recv_buf)
//#define STAGE_COMM_BUF_LEN			(40)
#define STAGE_COMM_BUF_LEN			(44)
#define STAGE_HUGE_BUF_LEN			(512)//(1024)

//�ʐM�^�C�v
//fssc_stage_sub.c
#define STAGE_COMM_BASIC				(0)
#define STAGE_COMM_TR					(1)
#define STAGE_COMM_ENEMY				(2)
#define STAGE_COMM_RETIRE				(3)
#define STAGE_COMM_TEMOTI				(7)
//stage.c
#define STAGE_COMM_PAIR					(4)
#define STAGE_COMM_PAIR_POKETYPE		(5)
#define STAGE_COMM_PAIR_FINAL_ANSWER	(6)

//�ʐM�̎���1�l�̃|�P������
#define	STAGE_COMM_POKE_NUM			(2)

//����(�}���`�Ȃ�G�̐���4�ȂǁA1�l�������2�ɂȂ�)
#define STAGE_FLAG_SOLO				(0)					//2
#define STAGE_FLAG_TOTAL			(1)					//2+2=4

//�ʐM�l��
#define STAGE_COMM_PLAYER_NUM		(2)

//�|�P�����A�N�^�[�\���ʒu
#define STAGE_POKE_ACT_X			(128)
#define STAGE_POKE_ACT_Y			(56)

//���R�[�h�Z�b�g�ɓn���O�ɐ��������Ȃ��ƃ_���I
//�d�l���́u�Z�[�u�f�[�^�ɂ��āv�̗��ɍő吔�������Ă���
#define STAGE_RENSYOU_MAX			(9999)				//�A����


#endif	//__STAGE_DEF_H__


