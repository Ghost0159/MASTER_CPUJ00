//==============================================================================================
/**
 * @file	factory_sys.h
 * @brief	�u�o�g���t�@�N�g���[�v�V�X�e���w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.03.15
 */
//==============================================================================================
#ifndef _FACTORY_SYS_H_
#define _FACTORY_SYS_H_

#include "common.h"
#include "system/procsys.h"


//#define DEBUG_ON									//�f�o�b�N�����L��


//==============================================================================================
//
//	�^�N�g���[�N�ւ̕s���S�^�|�C���^
//
//==============================================================================================
typedef	struct _FACTORY_WORK FACTORY_WORK;


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�p���b�g��`
#define BF_TALKWIN_PAL			(10)				//��b�E�B���h�E�g�p�p���b�g�i���o�[
#define BF_MENU_PAL				(11)				//���j���[�E�B���h�E�g�p�p���b�g�i���o�[
#define BF_MSGFONT_PAL			(12)				//MSG�t�H���g�g�p�p���b�g�i���o�[
#define BF_FONT_PAL				(13)				//SYSTEM�t�H���g�g�p�p���b�g�i���o�[

#define FACTORY_VANISH_ON		(0)					//��\��
#define FACTORY_VANISH_OFF		(1)					//�\��

//���\�[�X�}�l�[�W���[�I�u�W�F�N�g�i���o�[
enum{
	FACTORY_RES_OBJ_BALL = 0,						//�{�[���A�J�[�\���Ȃ�
	FACTORY_RES_OBJ_MAX,							//�ő吔(���\�[�X�t�@�C����1�̂�)
};

//�Ǘ�ID
enum{
	FACTORY_ID_OBJ_BALL	= 0,						//�{�[��
};

//�Ǘ��o���铮��OBJ�̍ő吔
#define BALL_OBJ_MAX			(6)					//�Z���A�N�^�[�ɐݒ肷��A�N�^�[�ő吔
#define CSR_OBJ_MAX				(2)					//�Z���A�N�^�[�ɐݒ肷��A�N�^�[�ő吔
#define WIN_OBJ_MAX				(1)
#define FACTORY_CLACT_OBJ_MAX	(BALL_OBJ_MAX+CSR_OBJ_MAX+WIN_OBJ_MAX)		//���͓K��

//�A�j���i���o�[

enum{
	ANM_SINGLE_OPEN	= 0,
	ANM_SINGLE_CLOSE,
	ANM_MULTI_OPEN,
	ANM_MULTI_CLOSE,
	ANM_3POKE_OPEN,
	ANM_3POKE_CLOSE,
	ANM_BALL_STOP,						//�{�[����~
	ANM_BALL_MOVE,						//�{�[������
	ANM_BALL_CSR,						//�{�[���J�[�\��
	ANM_MENU_CSR,						//���j���[�J�[�\��
	ANM_BALL_YURE,						//�{�[�����
	ANM_MULTI_2POKE_OPEN,
	ANM_MULTI_2POKE_CLOSE,
	ANM_BALL_CSR_STOP,					//�{�[���J�[�\����~
};

#define BF_FRAME_WIN	(GF_BGL_FRAME1_M)
#define BF_FRAME_SLIDE	(GF_BGL_FRAME2_M)			//MULTI�ƃ����^�����莞
#define BF_FRAME_BG		(GF_BGL_FRAME3_M)			//SINGLE,DOUBLE
#define BF_FRAME_SUB	(GF_BGL_FRAME0_S)			//�{�[��

typedef struct{
	s16  x;
	s16  y;
}FACTORY_POS;


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================
extern const PROC_DATA FactoryProcData;				//�v���Z�X��`�f�[�^
extern PROC_RESULT FactoryProc_Init( PROC * proc, int * seq );
extern PROC_RESULT FactoryProc_Main( PROC * proc, int * seq );
extern PROC_RESULT FactoryProc_End( PROC * proc, int * seq );

//�ʐM
extern void Factory_CommRecvBufBasicData(int id_no,int size,void *pData,void *work);
extern void Factory_CommRecvBufRentalData(int id_no,int size,void *pData,void *work);
extern void Factory_CommRecvBufCancelData(int id_no,int size,void *pData,void *work);
extern void Factory_CommRecvBufTradeFlag(int id_no,int size,void *pData,void *work);


#endif //_FACTORY_SYS_H_


