//==============================================================================
/**
 *
 * �퓬�^��
 *
 */
//==============================================================================
#ifndef __GT_GDS_BATTLE_REC_H__
#define __GT_GDS_BATTLE_REC_H__


#include "gds_define.h"
#include "gds_profile_local.h"
#include "gds_battle_rec_sub.h"


//----------------------------------------------------------
/**
 *	�^��Z�[�u�f�[�^�{�́i7272byte�j
 */
//----------------------------------------------------------
typedef struct {
	GT_REC_BATTLE_PARAM	bp;						//�퓬�J�n�p�����[�^	336byte
	GT_RECORD_PARAM		rp;						//�^��o�b�t�@			4096byte
	GT_REC_POKEPARTY	rec_party[GT_CLIENT_MAX];	//�|�P�����p�����[�^	628 * 4
	GT_MYSTATUS			my_status[GT_CLIENT_MAX];	//�v���C���[���		32byte * 4
	GT_CONFIG			config;					//�R���t�B�O			2byte
	u16 magic_key;
	
	//u32					checksum;	//�`�F�b�N�T��	4byte
	GT_GDS_CRC crc;							///< CRC						4
}GT_BATTLE_REC_WORK;

//--------------------------------------------------------------
/**
 *	�퓬�^��̃w�b�_
 *		100byte
 */
//--------------------------------------------------------------
typedef struct {
	u16 monsno[GT_HEADER_MONSNO_MAX];	///<�|�P�����ԍ�(�\������K�v���Ȃ��̂Ń^�}�S�̏ꍇ��0)	24
	u8 form_no[GT_HEADER_MONSNO_MAX];	///<�|�P�����̃t�H�����ԍ�								12

	u16 battle_counter;		///<�A����												2
	u8 mode;				///<�퓬���[�h(�t�@�N�g���[50�A�t�@�N�g���[100�A�ʐM�ΐ�...)
	
	u8 secure;				///<TRUE:���S���ۏႳ��Ă���B�@FALSE�F�Đ����������Ȃ�
	
	GT_REGULATION regulation;			///<���M�����[�V�����f�[�^		32

	u16 magic_key;			///<�}�W�b�N�L�[
	
	u8 work[14];			///< �\��										16
	
	GT_DATA_NUMBER data_number;		///<�f�[�^�i���o�[(�T�[�o�[���ŃZ�b�g�����)	8
	GT_GDS_CRC crc;							///< CRC						4
}GT_BATTLE_REC_HEADER;

//--------------------------------------------------------------
/**
 *	���M�f�[�^�F�퓬�^��
 *	7500byte
 */
//--------------------------------------------------------------
typedef struct {
	GT_GDS_PROFILE profile;		///<GDS�v���t�B�[��		88
	GT_BATTLE_REC_HEADER head;		///<�퓬�^��̃w�b�_	100
	GT_BATTLE_REC_WORK rec;		///<�^��f�[�^�{��			7272
}GT_BATTLE_REC_SEND;

//--------------------------------------------------------------
/**
 *	���M�f�[�^�F�������N�G�X�g
 *	16byte
 */
//--------------------------------------------------------------
typedef struct{
	u16 monsno;					///< �|�P�����ԍ�
	u8 battle_mode;				///< �����{��
	u8 country_code;			///< �Z��ł��鍑�R�[�h
	u8 local_code;				///< �Z��ł���n���R�[�h
	
	u8 padding[3];				//�p�f�B���O
	
	u32 server_version;			///< ROM�̃T�[�o�[�o�[�W����
	
	u8 work[4];					///<�\��
}GT_BATTLE_REC_SEARCH_SEND;

//--------------------------------------------------------------
/**
 *	���M�f�[�^�F�����L���O�擾
 */
//--------------------------------------------------------------
typedef struct{
	u32 server_version;			///< ROM�̃T�[�o�[�o�[�W����
}GT_BATTLE_REC_RANKING_SEARCH_SEND;


//--------------------------------------------------------------
/**
 *	��M�f�[�^�F�T�v�ꗗ�擾��
 *	228byte
 */
//--------------------------------------------------------------
typedef struct {
	GT_GDS_PROFILE profile;		///<GDS�v���t�B�[��
	GT_BATTLE_REC_HEADER head;		///<�퓬�^��̃w�b�_
}GT_BATTLE_REC_OUTLINE_RECV;

//--------------------------------------------------------------
/**
 *	��M�f�[�^�F�ꌏ�̌������ʎ擾��
 *	7500byte
 */
//--------------------------------------------------------------
typedef struct{
	GT_GDS_PROFILE profile;		///<GDS�v���t�B�[��
	GT_BATTLE_REC_HEADER head;		///<�퓬�^��̃w�b�_
	GT_BATTLE_REC_WORK rec;		///<�퓬�^��{��
}GT_BATTLE_REC_RECV;


#endif	//__GT_GDS_BATTLE_REC_H__

