//============================================================================================
/**
 * @file	calendar_enc.c
 * @brief	�J�����_�[����݂̃G���J�E���g
 * @author	Nozomu Saito
 * @date	2006.05.16
 */
//============================================================================================
#include "common.h"
#include "system/pm_rtc.h"

#include "calendar_enc.h"

#define DAY_DATA(day,prob_no)		( (day<<8) | (prob_no&0xff) )
#define GET_DAY(data)				( (data>>8)&0xff )
#define GET_PROB_NO(data)			( data&0xff )

enum {
	JAN_DAT_MAX = 4,
	FEB_DAT_MAX = 4,
	MAR_DAT_MAX = 3,
	APR_DAT_MAX = 4,
	MAY_DAT_MAX = 4,
	JUN_DAT_MAX = 1,
	JUL_DAT_MAX = 3,
	AUG_DAT_MAX = 4,
	SEP_DAT_MAX = 6,
	OCT_DAT_MAX = 3,
	NOV_DAT_MAX = 4,
	DEC_DAT_MAX = 3,
};

#define MONTH_NUM	(12)


typedef u16 CAL_ENC_DAY_DATA;

//1��
static const CAL_ENC_DAY_DATA JanData[JAN_DAT_MAX] = {
	DAY_DATA( 1,3),			//���U		New Year's Day�i�k�āE�C�M�E�C�^�E�X�E�t�E�h�j
	DAY_DATA(11,2),			//���J��
	DAY_DATA(12,4),			//B
	DAY_DATA(29,2)			//FRLG����
};

//2��
static const CAL_ENC_DAY_DATA FebData[FEB_DAT_MAX] = {
	DAY_DATA( 3,2),			//�ߕ�
	DAY_DATA(11,2),			//�����L�O��
	DAY_DATA(14,0),			//�o�����^�C���f�[(�k��)
	DAY_DATA(27,4)			//�ԗΔ���
};

//3��
static const CAL_ENC_DAY_DATA MarData[MAR_DAT_MAX] = {
	DAY_DATA( 3,2),			//�ЂȍՂ�
	DAY_DATA(18,2),			//�|�P�p�[�N�J��
	DAY_DATA(21,3),			//�t���̓�
};

//4��
static const CAL_ENC_DAY_DATA AprData[APR_DAT_MAX] = {
	DAY_DATA( 1,2),			//�A�j���J�n
	DAY_DATA(25,2),			//�|�P�Z���I�[�v��
	DAY_DATA(26,1),			//GF�ݗ�
	DAY_DATA(29,2),
};

//5��
static const CAL_ENC_DAY_DATA MayData[MAY_DAT_MAX] = {
	DAY_DATA( 1,0),			//�l�x		���[�f�[(�܌���)�i�k�āE�C�M�E�C�^�E�X�E�t�E�h�j
	DAY_DATA( 3,2),			//���@�L�O��
	DAY_DATA( 4,0),			//�����̋x��
	DAY_DATA( 5,2),			//���ǂ��̓�
};

//6��
static const CAL_ENC_DAY_DATA JunData[JUN_DAT_MAX] = {
	DAY_DATA(21,2),			//DUMMY
};

//7��
static const CAL_ENC_DAY_DATA JulData[JUL_DAT_MAX] = {
	DAY_DATA( 7,4),			//���[
	DAY_DATA(18,2),			//����ő��e
	DAY_DATA(24,2)			//DUMMY
};

//8��
static const CAL_ENC_DAY_DATA AugData[AUG_DAT_MAX] = {
	DAY_DATA(13,1),			//���~
	DAY_DATA(14,1),			//���~
	DAY_DATA(15,2),			//���~
	DAY_DATA(16,1),			//���~
};

//9��
static const CAL_ENC_DAY_DATA SepData[SEP_DAT_MAX] = {
	DAY_DATA(7,2),			//�\�ܖ�
	DAY_DATA(12,2),			//�\�ܖ�
	DAY_DATA(15,2),			//�\�ܖ�
	DAY_DATA(20,2),			//
	DAY_DATA(23,3),			//�H���̓�
	DAY_DATA(28,2)
};

//10��
static const CAL_ENC_DAY_DATA OctData[OCT_DAT_MAX] = {
	DAY_DATA( 5,2),			//�Ԑ���(���B)
	DAY_DATA(15,2),			//����
	DAY_DATA(30,2),			//
};

//11��
static const CAL_ENC_DAY_DATA NovData[NOV_DAT_MAX] = {
	DAY_DATA( 3,1),			//�����̓�
	DAY_DATA(12,2),			//����ő��e���J(�k��)
	DAY_DATA(21,2),			//����RS����
	DAY_DATA(23,0)			//�ΘJ���ӂ̓�
};

//12��
static const CAL_ENC_DAY_DATA DecData[DEC_DAT_MAX] = {
	DAY_DATA(14,2),			//�N���X�^������
	DAY_DATA(23,2),			//�V�c�a����
	DAY_DATA(31,1),			//��A��
};

typedef struct CAL_ENC_MONTH_DATA_tag{
	int Num;
	const CAL_ENC_DAY_DATA *DayData;
}CAL_ENC_MONTH_DATA;


static const CAL_ENC_MONTH_DATA CalEncMonthData[MONTH_NUM] = {
	{JAN_DAT_MAX,JanData},{FEB_DAT_MAX,FebData},{MAR_DAT_MAX,MarData},
	{APR_DAT_MAX,AprData},{MAY_DAT_MAX,MayData},{JUN_DAT_MAX,JunData},
	{JUL_DAT_MAX,JulData},{AUG_DAT_MAX,AugData},{SEP_DAT_MAX,SepData},
	{OCT_DAT_MAX,OctData},{NOV_DAT_MAX,NovData},{DEC_DAT_MAX,DecData}
};

static const int ProbTbl[] = 
{
	0,-5,5,-10,10
};
/**
static void DebugPrint(void);
*/
//-----------------------------------------------------------------------------
/**
 * ���ɂ��ɂ��m���v�Z
 *
 * @param	inProb			���݃G���J�E���g�m��
 * @param	inPenalty		�y�i���e�B�[
 *
 * @return	int				�G���J�E���g�m��
 */
//-----------------------------------------------------------------------------
int CalEnc_GetProb(const int inProb, const BOOL inPenalty)
{
	u8 i;
	int prob;
	RTCDate date;
	const CAL_ENC_MONTH_DATA * data;

	//���ԕs���ύX���ꂽ�ꍇ�́A�������Ȃ�
	if (inPenalty){
		return inProb;
	}
	
	if (inProb == 0){
		return 0;
	}

//	DebugPrint();
	
	GF_RTC_GetDate(&date);

	GF_ASSERT(date.month>0);
	GF_ASSERT(date.month<=12);

	data = &(CalEncMonthData[date.month-1]);

	for(i=0;i<data->Num;i++){
		if ( date.day == GET_DAY(data->DayData[i]) ){
			prob = inProb + ProbTbl[ GET_PROB_NO(data->DayData[i]) ];
			if (prob < 0){
				prob = 1;
			}
			return prob;
		}
	}
	return inProb;
}

/**
static void DebugPrint(void)
{
	int i,j;
	const CAL_ENC_MONTH_DATA * data;
	for(j=0;j<MONTH_NUM;j++){
		data = &(CalEncMonthData[j]);
		OS_Printf("Month:%d DayNum:%d\n",j+1,data->Num);
		for(i=0;i<data->Num;i++){
			OS_Printf(" Day:%d Prob:%d\n",GET_DAY(data->DayData[i]),GET_PROB_NO(data->DayData[i]));
		}
	}
}
*/
