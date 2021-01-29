//==============================================================================
/**
 * @file	frontier_savedata.h
 * @brief	�t�����e�B�A�Z�[�u
 * @author	matsuda
 * @date	2007.04.26(��)
 */
//==============================================================================
#ifndef __FRONTIER_SAVEDATA_H__
#define __FRONTIER_SAVEDATA_H__


//==============================================================================
//	�萔��`
//==============================================================================

///�F�B�蒠�f�[�^�łȂ����R�[�h���擾���鎞��friend_no�Ɏw�肷��l
#define FRONTIER_RECORD_NOT_FRIEND		(0xff)

///�A���L�^�̃��~�b�g�l
#define FRONTIER_RECORD_LIMIT		(9999)

//--------------------------------------------------------------
/**
 *	�t�����e�B�A�e�{�݂̋L�^�f�[�^��ID
 */
//--------------------------------------------------------------
enum{
	//�^���[�pINDEX�J�n
	FRID_TOWER_SINGLE_RENSHOU,
	FRID_TOWER_SINGLE_RENSHOU_CNT,
	FRID_TOWER_DOUBLE_RENSHOU,
	FRID_TOWER_DOUBLE_RENSHOU_CNT,
	FRID_TOWER_MULTI_NPC_RENSHOU,
	FRID_TOWER_MULTI_NPC_RENSHOU_CNT,
	FRID_TOWER_MULTI_COMM_RENSHOU,
	FRID_TOWER_MULTI_COMM_RENSHOU_CNT,
	FRID_TOWER_WIFI_DL_RENSHOU,
	FRID_TOWER_WIFI_DL_RENSHOU_CNT,
	
	//�t�@�N�g���[�pINDEX�J�n(���т�ύX���Ă̓_���I)
	FRID_FACTORY_SINGLE_RENSHOU,
	FRID_FACTORY_SINGLE_RENSHOU_CNT,
	FRID_FACTORY_SINGLE_TRADE,
	FRID_FACTORY_SINGLE_TRADE_CNT,
	FRID_FACTORY_SINGLE_RENSHOU100,
	FRID_FACTORY_SINGLE_RENSHOU100_CNT,
	FRID_FACTORY_SINGLE_TRADE100,
	FRID_FACTORY_SINGLE_TRADE100_CNT,
	FRID_FACTORY_DOUBLE_RENSHOU,
	FRID_FACTORY_DOUBLE_RENSHOU_CNT,
	FRID_FACTORY_DOUBLE_TRADE,
	FRID_FACTORY_DOUBLE_TRADE_CNT,
	FRID_FACTORY_DOUBLE_RENSHOU100,
	FRID_FACTORY_DOUBLE_RENSHOU100_CNT,
	FRID_FACTORY_DOUBLE_TRADE100,
	FRID_FACTORY_DOUBLE_TRADE100_CNT,
	FRID_FACTORY_MULTI_COMM_RENSHOU,
	FRID_FACTORY_MULTI_COMM_RENSHOU_CNT,
	FRID_FACTORY_MULTI_COMM_TRADE,
	FRID_FACTORY_MULTI_COMM_TRADE_CNT,
	FRID_FACTORY_MULTI_COMM_RENSHOU100,
	FRID_FACTORY_MULTI_COMM_RENSHOU100_CNT,
	FRID_FACTORY_MULTI_COMM_TRADE100,
	FRID_FACTORY_MULTI_COMM_TRADE100_CNT,

	//�X�e�[�W�pINDEX�J�n
	FRID_STAGE_SINGLE_RENSHOU,				///<�_�~�[(�|�P�������̍ő�A�����ɂȂ�\��)
	FRID_STAGE_SINGLE_RENSHOU_CNT,			///<�X�e�[�W�F�V���O�����݂̘A����
	FRID_STAGE_SINGLE_MONSNO,				///<�X�e�[�W�F�V���O�����킵�Ă���|�P�����ԍ�
	FRID_STAGE_SINGLE_TYPE_LEVEL_0,
	FRID_STAGE_SINGLE_TYPE_LEVEL_1,
	FRID_STAGE_SINGLE_TYPE_LEVEL_2,
	FRID_STAGE_SINGLE_TYPE_LEVEL_3,
	FRID_STAGE_SINGLE_TYPE_LEVEL_4,
	FRID_STAGE_SINGLE_TYPE_LEVEL_5,
	FRID_STAGE_SINGLE_TYPE_LEVEL_6,
	FRID_STAGE_SINGLE_TYPE_LEVEL_7,
	FRID_STAGE_SINGLE_TYPE_LEVEL_8,
	FRID_STAGE_DOUBLE_RENSHOU,				///<�_�~�[(�|�P�������̍ő�A�����ɂȂ�\��)
	FRID_STAGE_DOUBLE_RENSHOU_CNT,			///<�X�e�[�W�F�_�u�����݂̘A����
	FRID_STAGE_DOUBLE_MONSNO,				///<�X�e�[�W�F�_�u�����킵�Ă���|�P�����ԍ�
	FRID_STAGE_DOUBLE_TYPE_LEVEL_0,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_1,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_2,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_3,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_4,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_5,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_6,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_7,
	FRID_STAGE_DOUBLE_TYPE_LEVEL_8,
	FRID_STAGE_MULTI_COMM_RENSHOU,			///<�_�~�[(�|�P�������̍ő�A�����ɂȂ�\��)
	FRID_STAGE_MULTI_COMM_RENSHOU_CNT,		///<�X�e�[�W�F���C�����X���݂̘A����
	FRID_STAGE_MULTI_COMM_MONSNO,			///<�X�e�[�W�F���C�����X���킵�Ă���|�P�����ԍ�
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_0,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_1,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_2,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_3,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_4,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_5,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_6,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_7,
	FRID_STAGE_MULTI_COMM_TYPE_LEVEL_8,
	
	//�L���b�X���pINDEX�J�n
	FRID_CASTLE_SINGLE_RENSHOU,				///<�L���b�X���F�V���O���ő�A����
	FRID_CASTLE_SINGLE_RENSHOU_CNT,			///<�L���b�X���F�V���O�����݂̘A����
	FRID_CASTLE_SINGLE_CP,					///<�L���b�X���F�V���O�����݂̃L���b�X���|�C���g
	FRID_CASTLE_SINGLE_USED_CP,				///<�L���b�X���F�V���O���g�p�����L���b�X���|�C���g
	FRID_CASTLE_SINGLE_REMAINDER_CP,		///<�L���b�X���F�V���O���ő�A�������L�^�������̎c��CP
	FRID_CASTLE_SINGLE_RANK_RECOVER,	///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_SINGLE_RANK_RENTAL,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_SINGLE_RANK_INFO,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_DOUBLE_RENSHOU,				///<�L���b�X���F�_�u���ő�A����
	FRID_CASTLE_DOUBLE_RENSHOU_CNT,			///<�L���b�X���F�_�u�����݂̘A����
	FRID_CASTLE_DOUBLE_CP,					///<�L���b�X���F�_�u�����݂̃L���b�X���|�C���g
	FRID_CASTLE_DOUBLE_USED_CP,				///<�L���b�X���F�_�u���g�p�����L���b�X���|�C���g
	FRID_CASTLE_DOUBLE_REMAINDER_CP,		///<�L���b�X���F�_�u���ő�A�������L�^�������̎c��CP
	FRID_CASTLE_DOUBLE_RANK_RECOVER,	///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_DOUBLE_RANK_RENTAL,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_DOUBLE_RANK_INFO,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_MULTI_COMM_RENSHOU,			///<�L���b�X���F���C�����X�ő�A����
	FRID_CASTLE_MULTI_COMM_RENSHOU_CNT,		///<�L���b�X���F���C�����X���݂̘A����
	FRID_CASTLE_MULTI_COMM_CP,				///<�L���b�X���F���C�����X���݂̃L���b�X���|�C���g
	FRID_CASTLE_MULTI_COMM_USED_CP,			///<�L���b�X���F���C�����X�g�p�����L���b�X���|�C���g
	FRID_CASTLE_MULTI_COMM_REMAINDER_CP,///<�L���b�X���F���C�����X�ő�A�������L�^�������̎c��CP
	FRID_CASTLE_MULTI_COMM_RANK_RECOVER,	///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_MULTI_COMM_RANK_RENTAL,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_MULTI_COMM_RANK_INFO,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	
	//���[���b�g�pINDEX�J�n
	FRID_ROULETTE_SINGLE_RENSHOU,			///<���[���b�g�F�V���O���ő�A����
	FRID_ROULETTE_SINGLE_RENSHOU_CNT,		///<���[���b�g�F�V���O�����݂̘A����
	FRID_ROULETTE_DOUBLE_RENSHOU,			///<���[���b�g�F�_�u���ő�A����
	FRID_ROULETTE_DOUBLE_RENSHOU_CNT,		///<���[���b�g�F�_�u�����݂̘A����
	FRID_ROULETTE_MULTI_COMM_RENSHOU,		///<���[���b�g�F���C�����X�ő�A����
	FRID_ROULETTE_MULTI_COMM_RENSHOU_CNT,	///<���[���b�g�F���C�����X���݂̘A����

	FRID_SINGLE_END,	//��l�p�̃f�[�^�I�[
	
	//WIFI�̗F�B���̃N���A�t���O(�r�b�g�P�ʂŊǗ����Ă��܂�)
	FRID_TOWER_MULTI_WIFI_CLEAR_BIT = FRID_SINGLE_END,	//FrontierRecord_Set,Get�֐��̂ݑΉ�
	FRID_TOWER_MULTI_WIFI_CLEAR_BIT_SUB,		//���g�p�֎~�@�̈�m�ۂ̂�
	FRID_FACTORY_MULTI_WIFI_CLEAR_BIT,			//FrontierRecord_Set,Get�֐��̂ݑΉ�
	FRID_FACTORY_MULTI_WIFI_CLEAR_BIT_SUB,		//���g�p�֎~�@�̈�m�ۂ̂�
	FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT,		//FrontierRecord_Set,Get�֐��̂ݑΉ�
	FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT_SUB,	//���g�p�֎~�@�̈�m�ۂ̂�
	FRID_STAGE_MULTI_WIFI_CLEAR_BIT,			//FrontierRecord_Set,Get�֐��̂ݑΉ�
	FRID_STAGE_MULTI_WIFI_CLEAR_BIT_SUB,		//���g�p�֎~�@�̈�m�ۂ̂�
	FRID_CASTLE_MULTI_WIFI_CLEAR_BIT,			//FrontierRecord_Set,Get�֐��̂ݑΉ�
	FRID_CASTLE_MULTI_WIFI_CLEAR_BIT_SUB,		//���g�p�֎~�@�̈�m�ۂ̂�
	FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT,			//FrontierRecord_Set,Get�֐��̂ݑΉ�
	FRID_ROULETTE_MULTI_WIFI_CLEAR_BIT_SUB,		//���g�p�֎~�@�̈�m�ۂ̂�

	//�I�[
	FRID_MAX,

	//--------------------------------------------------------------
	//	�F�B�蒠���Ɏ����R�[�h
	//--------------------------------------------------------------
	//Wifi�^���[
	FRID_TOWER_MULTI_WIFI_RENSHOU = FRID_MAX,
	FRID_TOWER_MULTI_WIFI_RENSHOU_CNT,
	
	//Wifi�t�@�N�g���[(���т�ύX���Ă̓_���I)
	FRID_FACTORY_MULTI_WIFI_RENSHOU,
	FRID_FACTORY_MULTI_WIFI_RENSHOU_CNT,
	FRID_FACTORY_MULTI_WIFI_TRADE,
	FRID_FACTORY_MULTI_WIFI_TRADE_CNT,
	FRID_FACTORY_MULTI_WIFI_RENSHOU100,
	FRID_FACTORY_MULTI_WIFI_RENSHOU100_CNT,
	FRID_FACTORY_MULTI_WIFI_TRADE100,
	FRID_FACTORY_MULTI_WIFI_TRADE100_CNT,
	
	//Wifi�X�e�[�W							//���͕ʂɊO���Ń|�P�������Ƃ̍ő�A�����������Ă���
	FRID_STAGE_MULTI_WIFI_RENSHOU,			//������Ă���|�P�����̍ő�A����
	FRID_STAGE_MULTI_WIFI_RENSHOU_CNT,		//������Ă���|�P�����̘A����
	FRID_STAGE_MULTI_WIFI_MONSNO,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_0,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_1,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_2,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_3,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_4,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_5,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_6,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_7,
	FRID_STAGE_MULTI_WIFI_TYPE_LEVEL_8,
	
	//Wifi�L���b�X��
	FRID_CASTLE_MULTI_WIFI_RENSHOU,
	FRID_CASTLE_MULTI_WIFI_RENSHOU_CNT,
	FRID_CASTLE_MULTI_WIFI_CP,
	FRID_CASTLE_MULTI_WIFI_USED_CP,
	FRID_CASTLE_MULTI_WIFI_REMAINDER_CP,	///<�L���b�X���FWIFI�ő�A�������L�^�������̎c��CP
	FRID_CASTLE_MULTI_WIFI_RANK_RECOVER,	///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_MULTI_WIFI_RANK_RENTAL,		///<�L���b�X���F�ǂ��܂ŏグ�����L��
	FRID_CASTLE_MULTI_WIFI_RANK_INFO,		///<�L���b�X���F�ǂ��܂ŏグ�����L��

	//Wifi���[���b�g
	FRID_ROULETTE_MULTI_WIFI_RENSHOU,
	FRID_ROULETTE_MULTI_WIFI_RENSHOU_CNT,
	
	//Wifi�I�[
	FRID_WIFI_MAX,
};


//==============================================================================
//	�\���̒�`
//==============================================================================
/**
 *	@brief	�t�����e�B�A�@�u���b�N�f�[�^�\���̂ւ̕s���S�^�|�C���^
 *
 * ���g�͌����܂��񂪃|�C���^�o�R�ŎQ�Ƃł��܂�
 */
typedef struct _FRONTIER_SAVEWORK	FRONTIER_SAVEWORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern int FrontierData_GetWorkSize(void);
extern void FrontierData_Init(FRONTIER_SAVEWORK* save);
extern u16 FrontierRecord_Get(FRONTIER_SAVEWORK* save, int id, int friend_no);
extern u16 FrontierRecord_Set(FRONTIER_SAVEWORK* save, int id, int friend_no, u16 data);
extern u16 FrontierRecord_Add(FRONTIER_SAVEWORK* save, int id, int friend_no, int add_value);
extern u16 FrontierRecord_Sub(FRONTIER_SAVEWORK* save, int id, int friend_no, int sub_value);
extern u16 FrontierRecord_Inc(FRONTIER_SAVEWORK* save, int id, int friend_no);
extern FRONTIER_SAVEWORK * SaveData_GetFrontier(SAVEDATA * sv);
extern u16 FrontierRecord_SetIfLarge(FRONTIER_SAVEWORK *save, int id, int friend_no, u16 data);
extern void FrontierRecord_ResetData(FRONTIER_SAVEWORK* save, int friend_no);
extern void FrontierRecord_DataMarge(FRONTIER_SAVEWORK* save, int delNo, int friend_no);
extern void FrontierRecord_DataCrear(FRONTIER_SAVEWORK *save);


#endif	//__FRONTIER_SAVEDATA_H__
