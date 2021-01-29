//============================================================================================
/**
 * @file	factory_def.h
 * @bfief	�u�o�g���t�@�N�g���[�v��`
 * @author	Satoshi Nohara
 * @date	07.03.29
 */
//============================================================================================
#ifndef	__FACTORY_DEF_H__
#define	__FACTORY_DEF_H__


//============================================================================================
//
//	��`(enum�֎~)
//
//============================================================================================
#define FACTORY_LAP_ENEMY_MAX	(7)							//1�T���̓G�g���[�i�[�̍ő吔
#define FACTORY_LAP_MAX			(8)							///�t�@�N�g���[�̍ő����
#define FACTORY_LAP_MULTI_ENEMY_MAX	(FACTORY_LAP_ENEMY_MAX*2)//�}���`1�T���̓G�g���[�i�[�̍ő吔

//���ӁI(frontier_tool.h�ɂ��郌�x����`�Ƃ���Ȃ��悤�ɒ��ӁI)
//�t�@�N�g���[�̃Q�[�����x��
#define FACTORY_LEVEL_50		(0)							//���x��50
#define FACTORY_LEVEL_OPEN		(1)							//�I�[�v��
#define FACTORY_LEVEL_MAX		(2)							//�ő吔

//�^�C�v
#define FACTORY_TYPE_SINGLE		(0)
#define FACTORY_TYPE_DOUBLE		(1)
#define FACTORY_TYPE_MULTI		(2)
#define FACTORY_TYPE_WIFI_MULTI	(3)
#define FACTORY_TYPE_MAX		(4)							//�ő吔

//���[�h
#define FACTORY_MODE_RENTAL		(0)							//�����^��
#define FACTORY_MODE_TRADE		(1)							//����

//�|�P������
#define FACTORY_RENTAL_POKE_MAX	(6)							//�����^���|�P�����f�[�^�̍ő吔
#define FACTORY_MINE_POKE_MAX	(4)							//�����|�P�����f�[�^�̍ő吔
#define FACTORY_ENEMY_POKE_MAX	(4)							//�G�|�P�����f�[�^�̍ő吔
#define FACTORY_TOTAL_POKE_MAX	(FACTORY_MINE_POKE_MAX+FACTORY_ENEMY_POKE_MAX)	//���`�F�b�N�Ɏg�p

//�߂�l�ɕK�v�ȃ��[�N�̐�
#define	FACTORY_PARTY_POKE_MAX	(6)							//POKEPARTY�̃|�P������(6���ő�)
#define FACTORY_RET_WORK_MAX	(FACTORY_PARTY_POKE_MAX)	//ret_work[]
#define FACTORY_SEL_WORK_MAX	(FACTORY_PARTY_POKE_MAX)	//sel_poke_no[]

//�߂�l
#define FACTORY_RET_CANCEL		(0xff)						//�������Ȃ��ŃL�����Z������

//���R�[�hID�擾���Ɏg�p
#define FACTORY_RECID_SIZE		(4)							//�A���A�ő�A���A�����A�ő������4��
//#define FACTORY_RECID_NUM		(FACTORY_RECID_SIZE * 3)	//�V���O���A�_�u���A�}���`��3��

//FACTORY_SCRWORK�̎擾�R�[�h(factory.s�Ŏg�p)
#define FA_ID_SET_MODE				(0)
#define FA_ID_SET_LEVEL				(1)
#define FA_ID_SET_TYPE				(2)
#define FA_ID_GET_RET_WORK			(3)
#define FA_ID_GET_RENSYOU			(4)
#define FA_ID_INC_RENSYOU			(5)
//#define FA_ID_INC_TRADE_COUNT		(6)
#define FA_ID_SYSTEM_RESET			(7)
//#define FA_ID_CLEAR_PLAY_DATA		(8)
#define FA_ID_IS_SAVE_DATA_ENABLE	(9)
#define FA_ID_SAVE_REST_PLAY_DATA	(10)
//#define FA_ID_PUSH_NOW_LOCATION		(11)
//#define FA_ID_POP_NOW_LOCATION		(12)
//#define FA_ID_SET_NG				(13)
#define FA_ID_INC_ROUND				(14)
#define FA_ID_GET_MONSNO			(15)
#define FA_ID_GET_WAZANO			(16)
#define FA_ID_GET_POKE_TYPE			(17)
#define FA_ID_GET_LARGE_TYPE		(18)
#define FA_ID_GET_LAP				(19)
#define FA_ID_GET_TR_OBJ_CODE		(20)
#define FA_ID_SET_LOSE				(21)
#define FA_ID_SET_CLEAR				(22)
#define FA_ID_GET_ROUND				(23)
#define FA_ID_GET_RETIRE_FLAG		(24)
//#define FA_ID_SET_PAIR_NAME			(25)
#define FA_ID_GET_TRADE_YESNO_FLAG	(26)
#define FA_ID_COMM_COMMAND_INITIALIZE	(27)
#define FA_ID_CHECK_COMM_TYPE		(28)
#define FA_ID_GET_TYPE				(29)
#define FA_ID_BTL_ROOM_PAL_CHG		(30)
#define	FA_ID_ADD_MAP_RAIL_TCB		(31)
#define	FA_ID_DEL_MAP_RAIL_TCB		(32)
#define	FA_ID_SET_B_TOWER_PARTNER_DATA	(33)
#define FA_ID_BRAIN_PLAYER_POKE_NAME	(34)
#define FA_ID_GET_BP_POINT			(35)
#define FA_ID_INC_TRADE_COUNT		(36)
#define FA_ID_CHECK_BRAIN			(37)
#define FA_ID_PAIR_POKE_CREATE		(38)
#define FA_ID_ENEMY_POKE_CREATE		(39)
#define FA_ID_RENTAL_POKE_CREATE	(40)
#define FA_ID_BRAIN_APPEAR_MSG_CHK	(41)

//�ʐM�o�b�t�@�̃T�C�Y(send_buf,recv_buf)
//#define FACTORY_COMM_BUF_LEN		(40)
#define FACTORY_COMM_BUF_LEN		(60)

//�ʐM�^�C�v
//fssc_factory_sub.c
#define FACTORY_COMM_BASIC			(0)
#define FACTORY_COMM_TR				(1)
#define FACTORY_COMM_RENTAL			(2)
#define FACTORY_COMM_ENEMY			(3)
#define FACTORY_COMM_RETIRE			(4)
#define FACTORY_COMM_TRADE_YESNO	(5)
#define FACTORY_COMM_TEMOTI			(6)
//factory.c
#define FACTORY_COMM_PAIR			(7)
#define FACTORY_COMM_PAIR_POKE		(8)
//#define FACTORY_COMM_PAIR_CANCEL	(9)
#define FACTORY_COMM_TRADE			(10)

//�ʐM�̎���1�l�̃|�P������
#define	FACTORY_COMM_POKE_NUM		(2)

//�ʐM�l��
#define	FACTORY_COMM_PLAYER_NUM		(2)

//����(�}���`�Ȃ�G�̐���4�ȂǁA1�l�������2�ɂȂ�)
#define FACTORY_FLAG_SOLO			(0)					//2
#define FACTORY_FLAG_TOTAL			(1)					//2+2=4

//���R�[�h�Z�b�g�ɓn���O�ɐ��������Ȃ��ƃ_���I
//�d�l���́u�Z�[�u�f�[�^�ɂ��āv�̗��ɍő吔�������Ă���
#define FACTORY_TRADE_MAX			(9999)				//������
#define FACTORY_RENSYOU_MAX			(9999)				//�A����


#endif	//__FACTORY_DEF_H__


