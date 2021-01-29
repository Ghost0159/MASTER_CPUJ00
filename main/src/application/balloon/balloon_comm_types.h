//==============================================================================
/**
 * @file	balloon_comm_types.h
 * @brief	���D�~�j�Q�[���ŒʐM�������f�[�^�^�̒�`
 * @author	matsuda
 * @date	2007.11.26(��)
 */
//==============================================================================
#ifndef __BALLOON_COMM_TYPES_H__
#define __BALLOON_COMM_TYPES_H__


//==============================================================================
//	�萔��`
//==============================================================================
///���߃R�[�h
enum{
	ORDER_CODE_NULL,		///<��
	
	ORDER_CODE_AIR,
	ORDER_CODE_EXPLODED,
	ORDER_CODE_APPEAR,

	ORDER_CODE_START,		///<�~�j�Q�[���J�n
	ORDER_CODE_FINISH,		///<�~�j�Q�[���I��
	ORDER_CODE_TIMING,		///<�����҂�

	ORDER_CODE_TOUCHPEN_DEMO,	///<�^�b�`�y���f��
	
	ORDER_CODE_MAX,			///<���߃R�[�h�I�[
};

///����M�o�b�t�@�̃`�F�[����(���߂Ă����鐔)
#define SIO_BUFFER_CHAIN_MAX		(60)


///������胊�N�G�X�g�R�}���h
enum{
	TIMING_REQ_NULL,				///<���N�G�X�g�Ȃ�
	//----------------------------
	
	TIMING_REQ_NORMAL,				///<���������

	TIMING_REQ_START_AFTER,			///<�Q�[���J�n���o���I�������瓯�����
	TIMING_REQ_FINISH_AFTER,		///<�Q�[���I�����o���I�������瓯�����

	TIMING_REQ_APPEAR_AFTER,		///<���D���o�ꂵ���瓯�����
	TIMING_REQ_EXPLODED_AFTER,		///<�j��G�t�F�N�g���I�������瓯�����
	
	TIMING_REQ_TOUCHPEN_DEMO_AFTER,	///<�^�b�`�y���f�����I�������瓯�����
	
	//----------------------------
	TIMING_REQ_WAIT,				///<�����M���͑����āA���̐l�̐M���҂�
};


//==============================================================================
//	�\���̒�`
//==============================================================================

///�������
typedef struct{
	u8 timing_req;		///<�������̎��
	u8 timing_no;		///<�������ԍ�
	
	u8 dummy[2];
}BALLOON_TIMING;

///�Q�[���J�n
typedef struct{
	u8 timing_req;		///<�������̎��
	u8 timing_no;		///<�������ԍ�
	
	u8 dummy[2];
}BALLOON_START;

///�Q�[���I��
typedef struct{
	u8 timing_req;		///<�������̎��
	u8 timing_no;		///<�������ԍ�
	
	u8 dummy[2];
}BALLOON_FINISH;

///����������C��
typedef struct{
	u8 net_id;			///<�ʐMID
	u8 booster_type;	///<BOOSTER_TYPE_???
	u16 no;				///<���D�ԍ�(�Q�[�����n�܂��ĉ��ڂ̕��D��)
	s32 air;			///<��C(�u�[�X�^�[�A���A���Ȃ������ȋ�C��)
	s32 last_air;		///<�u�[�X�^�[�A���A�␳�����݂̍ŏI�I�ȋ�C��
	u8 booster_occ;		///<�u�[�X�^�[�̐ݒ�L��(TRUE:�ݒ�ς�)
	u8 padding[3];
}BALLOON_AIR_DATA;

///�j�􂵂����D�ԍ����
typedef struct{
	u16 no;				///<���D�ԍ�(�Q�[�����n�܂��ĉ��ڂ̕��D��)

	u8 timing_req;
	u8 timing_no;
}BALLOON_EXPLODED;

///�V�����o�ꂷ�镗�D���
typedef struct{
	u16 no;				///<���D�ԍ�(�Q�[�����n�܂��ĉ��ڂ̕��D��)
	u8 level;			///<���D���x��(BALLOON_LEVEL_???)
	
	u8 timing_req;
	u8 timing_no;
	
	u8 dummy[2];
}BALLOON_APPEAR;


//--------------------------------------------------------------
/**
 *	�Q�[�����̑���M�f�[�^�\����
 */
//--------------------------------------------------------------
typedef struct{
	int order_code;		///<���߃R�[�h
	
	union{
		BALLOON_TIMING timing;
		BALLOON_TIMING start;
		BALLOON_TIMING finish;
		BALLOON_AIR_DATA air_data;
		BALLOON_EXPLODED exploded;
		BALLOON_APPEAR appear;
	};
}BALLOON_SIO_PLAY_WORK;


//--------------------------------------------------------------
/**
 *	����M�o�b�t�@(�`�F�[�����X�g�Ƃ��Ĉ���)
 */
//--------------------------------------------------------------
typedef struct{
	BALLOON_SIO_PLAY_WORK	play_work[SIO_BUFFER_CHAIN_MAX];
}BALLOON_SIO_BUFFER;


#endif	//__BALLOON_COMM_TYPES_H__

