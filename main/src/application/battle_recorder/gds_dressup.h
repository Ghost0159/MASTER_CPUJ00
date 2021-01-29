//==============================================================================
/**
 *
 * GDS�h���X�A�b�v
 *
 */
//==============================================================================
#ifndef __GT_GDS_DRESSUP_H__
#define __GT_GDS_DRESSUP_H__


#include "gds_define.h"
#include "gds_profile_local.h"
#include "gds_dressup_sub.h"


//--------------------------------------------------------------
/**
 *	�h���X�A�b�v		92byte
 */
//--------------------------------------------------------------
typedef struct {
	u32 init_flag;					///<�������f�[�^���̃t���O
	GT_IMC_SAVEDATA_POKEMON	pokemon;///<�|�P�����f�[�^
	u32 accessory_set_msk;			///<�i�[�A�N�Z�T���[�}�X�N
	GT_PMS_DATA title;					///<�N���b�v�^�C�g��
	GT_IMC_SAVEDATA_ACCESSORIE acce[ GT_IMC_SAVEDATA_TELEVISION_ACCE_NUM ];	///<�A�N�Z�T��
	u8 bg_id;								///<bg�i���o�[	
	u8 country;	// ���R�[�h
	
	u8 padding[2];					// �p�f�B���O
	
	GT_GDS_CRC crc;							///< CRC						4
}GT_GDS_DRESS;

//--------------------------------------------------------------
/**
 *	�h���X�A�b�v���M�f�[�^�F128 + 92 = 220byte
 */
//--------------------------------------------------------------
typedef struct{
	GT_GDS_PROFILE profile;
	GT_GDS_DRESS dressup;
}GT_GDS_DRESS_SEND;

//--------------------------------------------------------------
/**
 *	�h���X�A�b�v��M�f�[�^�F128 + 92 = 220byte
 */
//--------------------------------------------------------------
typedef struct{
	GT_GDS_PROFILE profile;
	GT_GDS_DRESS dressup;
}GT_GDS_DRESS_RECV;


#endif	//__GT_GDS_DRESSUP_H__

