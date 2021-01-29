//==============================================================================
/**
 * @file	battle_rec_local.h
 * @brief	�퓬�^�惍�[�J���w�b�_
 * @author	matsuda
 * @date	2008.05.15(��)
 */
//==============================================================================
#ifndef __BATTLE_REC_LOCAL_H__
#define __BATTLE_REC_LOCAL_H__

//==============================================================================
//	
//==============================================================================
///�f�[�^�i���o�[�̌^
typedef u64 DATA_NUMBER;
///�f�[�^�i���o�[�̃o�C�g�T�C�Y
#define DATANUMBER_SIZE		(sizeof(DATA_NUMBER))

///�^��w�b�_�Ɋi�[����|�P�����ԍ��̍ő�v�f��
#define HEADER_MONSNO_MAX		(12)

///��b�E�B���h�E�̖���
#define TALK_WINDOW_MAX			(20)

///�^��f�[�^�̑��ݗL��
#define REC_OCC_MAGIC_KEY		(0xe281)

//--------------------------------------------------------------
/**
 *	�퓬�^��p�ɃJ�X�^�}�C�Y���ꂽPOKEPARTY
 */
//--------------------------------------------------------------
typedef struct{
	///	�ێ��ł���|�P�������̍ő�
	u16 PokeCountMax;
	///	���ݕێ����Ă���|�P������
	u16 PokeCount;
	///	�|�P�����f�[�^
	REC_POKEPARA member[TEMOTI_POKEMAX];
}REC_POKEPARTY;

///<�ΐ�^��p���[�N�\���̐錾
struct record_param
{
	REC_DATA	rec_buffer[CLIENT_MAX][REC_BUFFER_SIZE];
};

//----------------------------------------------------------
/**
 *	�^��Z�[�u�f�[�^�{�́i??? bytes�j
 */
//----------------------------------------------------------
typedef struct _BATTLE_REC_WORK {
	REC_BATTLE_PARAM	rbp;
	RECORD_PARAM		rp;
	REC_POKEPARTY		rec_party[CLIENT_MAX];
	MYSTATUS			my_status[CLIENT_MAX];
	CONFIG				config;
//	u16 padding;
	u16 magic_key;
	
	//CRC(�K���Ō���ɂ��Ă�������)
	GDS_CRC				crc;
}BATTLE_REC_WORK;

//--------------------------------------------------------------
/**
 *	�퓬�^��̃w�b�_
 */
//--------------------------------------------------------------
typedef struct _BATTLE_REC_HEADER{
	u16 monsno[HEADER_MONSNO_MAX];	///<�|�P�����ԍ�(�\������K�v���Ȃ��̂Ń^�}�S�̏ꍇ��0)
	u8 form_no[HEADER_MONSNO_MAX];	///<�|�P�����̃t�H�����ԍ�

	u16 battle_counter;		///<�A����
	u8 mode;				///<�퓬���[�h(�t�@�N�g���[50�A�t�@�N�g���[100�A�ʐM�ΐ�...)
	
	u8 secure;				///<TRUE:���S���ۏႳ��Ă���B�@FALSE�F�Đ����������Ȃ�

	REGULATION regulation;			///<���M�����[�V�����f�[�^		32

	u16 magic_key;			///<�}�W�b�N�L�[
	u8 work[14];			///< �\��										16

	//CRC�ƃf�[�^�i���o�[(�K���Ō���ɂ��Ă�������)
	DATA_NUMBER data_number;///<�f�[�^�i���o�[(�T�[�o�[���ŃZ�b�g�����)�B�`�F�b�N�T���Ώۂ���O��
	GDS_CRC				crc;
}BATTLE_REC_HEADER;

//--------------------------------------------------------------
/**
 *	�^��Z�[�u�f�[�^�FGDS�v���t�B�[���{�w�b�_�{�{��
 */
//--------------------------------------------------------------
struct _BATTLE_REC_SAVEDATA{
	//�a������ȊO�̊O���Z�[�u�͕K����ԍŏ���EX_SAVE_KEY�\���̂�z�u���Ă�������
	EX_CERTIFY_SAVE_KEY save_key;			///<�F�؃L�[

	GDS_PROFILE profile;			///<GDS�v���t�B�[��
	BATTLE_REC_HEADER head;			///<�퓬�^��w�b�_
	BATTLE_REC_WORK rec;			///<�퓬�^��{��
};


#endif	//__BATTLE_REC_LOCAL_H__
