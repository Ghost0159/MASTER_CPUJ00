//==============================================================================
/**
 *
 * GDS�����L���O
 *
 */
//==============================================================================
#ifndef __GT_GDS_RANKING_H__
#define __GT_GDS_RANKING_H__

#include "gds_define.h"
#include "gds_profile_local.h"
#include "gds_ranking_sub.h"


//--------------------------------------------------------------
/**
 *	�����̃����L���O�o�^�p�f�[�^	8byte
 */
//--------------------------------------------------------------
typedef struct{
	u8 ranking_type;		///<�����L���O�w��i���o�[
	u8 padding[3];			//�p�f�B���O
	u32 score;				///<����
}GT_RANKING_MYDATA;

//--------------------------------------------------------------
/**
 *	��T�̃����L���O�f�[�^		456byte
 */
//--------------------------------------------------------------
typedef struct{
	u8 ranking_type;						///<�����L���O�w��i���o�[
	u8 padding[3];
	
	///�g���[�i�[�^�C�v�̃����L���O����(1�ʂ��珇�ʏ��Ƀg���[�i�[�^�C�v�������Ă��܂�)
	u8 trainer_type_ranking[GT_TRAINER_VIEW_MAX];
	///�g���[�i�[�^�C�v�����L���O�̃X�R�A(trainer_type_ranking�Ɠ������Ԃœ��_�������Ă��܂�)
	u64 trainer_type_score[GT_TRAINER_VIEW_MAX];
	
	///�a�����̃����L���O����(1�ʂ��珇�ʏ��ɒa�����������Ă��܂�)
	u8 birthday_month_ranking[GT_YEAR_MONTH_NUM];
	///�a���������L���O�̃X�R�A(birthday_month_ranking�Ɠ������Ԃœ��_�������Ă��܂�)
	u64 birthday_month_score[GT_YEAR_MONTH_NUM];
	
	///�D���ȃ|�P�����̃����L���O����(1�ʂ��珇�ʏ��Ƀ|�P�����ԍ��������Ă��܂�)
	u16 monsno_ranking[GT_MONSNO_RANKING_MAX];
	///�D���ȃ|�P���������L���O�̃X�R�A(monsno_ranking�Ɠ������Ԃœ��_�������Ă��܂�)
	u64 monsno_score[GT_MONSNO_RANKING_MAX];
}GT_LAST_WEEK_RANKING_DATA;

//--------------------------------------------------------------
/**
 *	���T�̃����L���O�f�[�^		72byte
 */
//--------------------------------------------------------------
typedef struct{
	u8 ranking_type;						///<�����L���O�w��i���o�[
	u8 padding[3];
	
	///�g���[�i�[�^�C�v�̃����L���O����(1�ʂ��珇�ʏ��Ƀg���[�i�[�^�C�v�������Ă��܂�)
	u8 trainer_type_ranking[GT_TRAINER_VIEW_MAX];
	
	///�a�����̃����L���O����(1�ʂ��珇�ʏ��ɒa�����������Ă��܂�)
	u8 birthday_month_ranking[GT_YEAR_MONTH_NUM];
	
	///�D���ȃ|�P�����̃����L���O����(1�ʂ��珇�ʏ��Ƀ|�P�����ԍ��������Ă��܂�)
	u16 monsno_ranking[GT_MONSNO_RANKING_MAX];
}GT_THIS_WEEK_RANKING_DATA;



//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 *	��M�f�[�^�F���ݍs���Ă��郉���L���O���e		4byte
 */
//--------------------------------------------------------------
typedef struct{
	u8 ranking_type[GT_RANKING_WEEK_NUM];		///<���݊J�Â���Ă��郉���L���O�̎��
	u8 padding;		//�p�f�B���O
}GT_RANKING_TYPE_RECV;

//--------------------------------------------------------------
/**
 *	��M�f�[�^�F��T�̃����L���O�f�[�^		1368byte
 */
//--------------------------------------------------------------
typedef struct{
	GT_LAST_WEEK_RANKING_DATA ranking_data[GT_RANKING_WEEK_NUM];
}GT_LAST_WEEK_RANKING_ALL_RECV;

//--------------------------------------------------------------
/**
 *	��M�f�[�^�F���T�̃����L���O�f�[�^		216byte
 */
//--------------------------------------------------------------
typedef struct{
	GT_THIS_WEEK_RANKING_DATA ranking_data[GT_RANKING_WEEK_NUM];
}GT_THIS_WEEK_RANKING_DATA_ALL_RECV;


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 *	���M�f�[�^�F�����̃����L���O�f�[�^		40byte
 */
//--------------------------------------------------------------
typedef struct{
	GT_RANKING_PROFILE profile;				///<�����L���O�p�v���t�B�[��
	GT_PLAYTIME playtime;					///<�v���C����
	GT_RANKING_MYDATA my_data[GT_RANKING_WEEK_NUM];	///<�����̃����L���O�o�^�p�f�[�^
}GT_RANKING_MYDATA_SEND;


#endif	//__GT_GDS_RANKING_H__
