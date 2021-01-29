//==============================================================================================
/**
 * @file	stage_sys.h
 * @brief	�u�o�g���X�e�[�W�v�V�X�e���w�b�_�[
 * @author	Satoshi Nohara
 * @date	07.06.08
 */
//==============================================================================================
#ifndef _STAGE_SYS_H_
#define _STAGE_SYS_H_

#include "common.h"
#include "system/procsys.h"


//==============================================================================================
//
//	�^�N�g���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _STAGE_WORK STAGE_WORK;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�p���b�g��`
#define BS_TALKWIN_PAL			(10)				//��b�E�B���h�E�g�p�p���b�g�i���o�[
#define BS_MENU_PAL				(11)				//���j���[�E�B���h�E�g�p�p���b�g�i���o�[
#define BS_MSGFONT_PAL			(12)				//MSG�t�H���g�g�p�p���b�g�i���o�[
#define BS_FONT_PAL				(13)				//SYSTEM�t�H���g�g�p�p���b�g�i���o�[

#define STAGE_VANISH_ON			(0)					//��\��
#define STAGE_VANISH_OFF		(1)					//�\��

//���\�[�X�}�l�[�W���[�I�u�W�F�N�g�i���o�[
enum{
	STAGE_RES_OBJ_CSR = 0,								//�J�[�\��
	STAGE_RES_OBJ_ICON,									//�A�C�R��
	STAGE_RES_OBJ_MAX,									//�ő吔(���\�[�X�t�@�C����1�A���Ƃ̓A�C�R��)
};

//�Ǘ�ID
enum{
	STAGE_ID_OBJ_CSR = 0,								//�J�[�\��
	STAGE_ID_OBJ_ICON,									//�A�C�R��
	STAGE_ID_OBJ_MAX,
};

#define STAGE_CLACT_OBJ_MAX	(2)						//�Z���A�N�^�[�ɐݒ肷��ő�A�N�^�[��

//�A�j���i���o�[
enum{
	ANM_CSR_STOP = 0,								//�J�[�\����~��
	ANM_CSR_MOVE,									//�J�[�\���ړ���
	ANM_CSR_POKE,									//�J�[�\���|�P����
};

#define BS_FRAME_WIN	(GF_BGL_FRAME0_M)
#define BS_FRAME_TYPE	(GF_BGL_FRAME1_M)
#define BS_FRAME_SLIDE	(GF_BGL_FRAME2_M)			//MULTI�ƃ����^�����莞
#define BS_FRAME_BG		(GF_BGL_FRAME3_M)			//SINGLE,DOUBLE
#define BS_FRAME_SUB	(GF_BGL_FRAME0_S)			//�{�[��

typedef struct{
	s16  x;
	s16  y;
}STAGE_POS;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern const PROC_DATA StageProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT StageProc_Init( PROC * proc, int * seq );
extern PROC_RESULT StageProc_Main( PROC * proc, int * seq );
extern PROC_RESULT StageProc_End( PROC * proc, int * seq );

//�ʐM
extern void Stage_CommSendBufBasicData( STAGE_WORK* wk, u16 type );
extern void Stage_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
extern void Stage_CommSendBufPokeTypeData( STAGE_WORK* wk, u16 type, u16 param );
extern void Stage_CommRecvBufPokeTypeData(int id_no,int size,void *pData,void *work);
extern void Stage_CommSendBufFinalAnswerData( STAGE_WORK* wk, u16 type, u16 param );
extern void Stage_CommRecvBufFinalAnswerData(int id_no,int size,void *pData,void *work);


#endif //_STAGE_SYS_H_


