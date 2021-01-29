//==============================================================================
/**
 * @file	gds_profile_types.h
 * @brief	GDS�v���t�B�[���̃��[�J���w�b�_
 * @author	matsuda
 * @date	2007.08.22(��)
 */
//==============================================================================
#ifndef __GDS_PROFILE_TYPES_H__
#define __GDS_PROFILE_TYPES_H__

#include "gds_local_common.h"
#include "system/pms_data.h"

//==============================================================================
//	�萔��`
//==============================================================================
///���O�̃T�C�Y(EOM����)
#define PLW_TRAINER_NAME_SIZE	(7+1)	//PERSON_NAME_SIZE + EOM_SIZE

///�C�x���g�^��̎��ȏЉ�b�Z�[�W������
#define EVENT_SELF_INTRO			(40)

///���ȏЉ�b�Z�[�W�̕\���`��
enum{
	MESSAGE_FLAG_NORMAL,		///<�ȈՉ�b
	MESSAGE_FLAG_EVENT,			///<�t���[���[�h
};


//==============================================================================
//	�\���̒�`
//==============================================================================
//--------------------------------------------------------------
/**
 *	GDS�v���t�B�[���\����
 *		88byte
 */
//--------------------------------------------------------------
typedef struct _GDS_PROFILE{
	u16 name[PLW_TRAINER_NAME_SIZE];		///< ���O(EOM����)		16
	u32 player_id;							///< �v���C���[ID		4
	u8 player_sex;							///< �v���C���[����
	u8 birthday_month;						///< �v���C���[�̒a����
	u8 trainer_view;						///< ���j�I�����[�����ł̌�����

	u8 country_code;						///< �Z��ł��鍑�R�[�h
	u8 local_code;							///< �Z��ł���n���R�[�h

	u8 version_code;						///< �o�[�W�����R�[�h
	u8 language;							///< ����R�[�h
	
	u8 egg_flag:1;							///< 1=�^�}�S
	u8 form_no:7;							///< �|�P�����t�H�����ԍ�
	u16 monsno;								///< �|�P�����ԍ�		2
	
	u8 message_flag;						///< �ȈՉ�b�A�t���[���[�h�ǂ�����g�p���邩
	u8 padding;
	//PMS_DATA self_introduction;			///< ���ȏЉ�(�ȈՉ�b)	8
	union{
		PMS_DATA self_introduction;			///< ���ȏЉ�(�ȈՉ�b)	8
		u16 event_self_introduction[EVENT_SELF_INTRO];	///< ���ȏЉ�(�C�x���g�p�t���[���[�h(EOM����))
	};
	
	u8 work[12];							///< �\��				14
	
	//CRC(�K���Ō���ɂ��Ă�������)
	GDS_CRC				crc;
}GDS_PROFILE;


#endif	//__GDS_PROFILE_TYPES_H__
