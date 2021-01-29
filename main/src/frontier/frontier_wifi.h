//============================================================================================
/**
 * @file	frontier_wifi.h
 * @bfief	�t�����e�B�AWiFi��t�@�Ăяo��
 * @author	Satoshi Nohara
 * @date	07.07.27
 */
//============================================================================================
#ifndef	__FRONTIER_WIFI_H__
#define	__FRONTIER_WIFI_H__

#include "field/field.h"
#include "frontier/frontier_main.h"
#include "application/pokelist.h"
#include "application/p_status.h"

//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern FRONTIER_EX_PARAM* EvCmdFrontierSystemCall( FIELDSYS_WORK* fsys, void* work );

#define FRWIFI_COMM_BUF_LEN		(40)
#define FR_WIFI_COUNTER_ENTRY_POKE_MAX		(4)


//============================================================================================
//
//	�\����
//
//============================================================================================
typedef struct{

	SAVEDATA* sv;
	void* p_work;

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�ʐM�p�F�f�[�^�o�b�t�@
	u16	send_buf[FRWIFI_COMM_BUF_LEN];

	//�ʐM�p�F��{���
	u8	pair_sex;											//����
	u8	pair_retire_flag;									//���^�C����I�񂾂�=1
	STRCODE pair_name[PERSON_NAME_SIZE + EOM_SIZE];			//

	//�Q�����Ă���莝���|�P�����̈ʒu���
	u8	mine_poke_pos[FR_WIFI_COUNTER_ENTRY_POKE_MAX];

	u8	pair_bf_no;
	u8	recieve_count;

	u8	dummy36;
	u8	pair_game_continue;									//�p�[�g�i�[�������ėV�Ԃ�

	u16 pair_rensyou;										//�p�[�g�i�[�̘A�����`�F�b�N�p
	u16	pair_stage_del;										//�p�[�g�i�[���X�e�[�W�L�^�����Ă悢��

	u16 monsno[FR_WIFI_COUNTER_ENTRY_POKE_MAX];
	u16 itemno[FR_WIFI_COUNTER_ENTRY_POKE_MAX];
	u16 pair_monsno[FR_WIFI_COUNTER_ENTRY_POKE_MAX];
	u16 pair_itemno[FR_WIFI_COUNTER_ENTRY_POKE_MAX];

	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	//�|�P�������X�g�p
	FIELDSYS_WORK* fsys;									//�|�P�������X�g�Ăяo���ɕK�v
	u8	seq;
	u8  next_seq;
	u8	mode;												//�|�P�������X�g���[�h
	u8	pos;												//�|�P�������X�g�̌��݃J�[�\���ʒu
	u8	bf_no;												//�{��No
	u8	sel[FR_WIFI_COUNTER_ENTRY_POKE_MAX];
	PLIST_DATA* pld;
	PSTATUS_DATA* psd;
	u16* ret_wk;
}FRWIFI_SCRWORK;


#endif	//__FRONTIER_WIFI_H__


