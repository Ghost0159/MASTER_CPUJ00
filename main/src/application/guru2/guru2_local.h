//==============================================================================
/**
 * @file	guru2_local.h
 * @brief	���邮������@���[�J���p�w�b�_�[�t�@�C��
 * @author	kagaya
 * @data	05.07.13
 */
//==============================================================================
#ifndef GURU2_LH_FILE
#define GURU2_LH_FILE

#include "common.h"
#include "application\guru2.h"

#include "guru2_comm.h"
#include "guru2.naix"

//==============================================================================
//	define
//==============================================================================
//--------------------------------------------------------------
//	debug
//--------------------------------------------------------------
//----
#ifdef PM_DEBUG

#ifdef DEBUG_ONLY_FOR_kagaya
//#define GURU2_DEBUG_ON					//��`�Ńf�o�b�O�@�\ON
#endif

#ifdef GURU2_DEBUG_ON
//#define DEBUG_GAME_SEC (10)			//��`�ŃQ�[�����ԕύX
//#define DEBUG_DISP_CHECK				//��`�ŃJ�����A�M�̉�ʃ`�F�b�N
#endif

#endif
//----

#define GURU2_RC_MCCELL_OFF

//--------------------------------------------------------------
//	
//--------------------------------------------------------------
#define NUM_FX32(a)	(FX32_ONE*(a))			///<����->FX32�^��
#define FX32_NUM(a)	((a)/FX32_ONE)			///<FX32->������

//--------------------------------------------------------------
//	�q�[�v
//--------------------------------------------------------------
#define GURU2_HEAPSIZE	(0x80000)			//<�q�[�v�T�C�Y

//--------------------------------------------------------------
///	GURU2RET
//--------------------------------------------------------------
typedef enum
{
	GURU2RET_NON,
	GURU2RET_NEXT,
	GURU2RET_END,
}GURU2RET;

//--------------------------------------------------------------
///	���邮��������[�h
//--------------------------------------------------------------
enum
{
	GURU2MODE_RECEIPT,
	GURU2MODE_POKESEL,
	GURU2MODE_POKESEL_CANCEL,
	GURU2MODE_GAME_MAIN,
};

//--------------------------------------------------------------
///	���邮��ʐM�ڑ����
//--------------------------------------------------------------
enum
{
	GURU2CONNECT_ON = 0,		//�ڑ����
	GURU2CONNECT_CUT_EXE,		//�ؒf��
	GURU2CONNECT_CUT,			//�ؒf����
	GURU2CONNECT_TIMING,		//������
	GURU2CONNECT_MAX,			//�ő�
};

//--------------------------------------------------------------
///	���邮��ʐM�V�[�P���X
//--------------------------------------------------------------
enum
{
	GURU2CONNECTPROC_NON = 0,	//���ɖ���
	GURU2CONNECTPROC_TIMING,	//��������
	GURU2CONNECTPROC_CUT,		//�ؒf
	GURU2CONNECTPROC_MAX,		//�ő�
};

//==============================================================================
//	typedef
//==============================================================================
///�Q�[���v���Z�X���[�N
typedef struct _TAG_GURU2PROC_WORK GURU2PROC_WORK;
///�Q�[�����C���p���[�N
typedef struct _TAG_GURU2MAIN_WORK GURU2MAIN_WORK;
///��t�p���[�N
typedef struct _TAG_GURU2RC_WORK GURU2RC_WORK;

//--------------------------------------------------------------
///	GURU2PROC_WORK
//--------------------------------------------------------------
struct _TAG_GURU2PROC_WORK
{
	int guru2_mode;
	int trade_no;
	int receipt_num;		//��t�l��
	u32 receipt_bit;		//��t�r�b�g
	BOOL receipt_ret;
	
	GURU2_PARAM param;
	GURU2COMM_WORK *g2c;
	GURU2MAIN_WORK *g2m;
	GURU2RC_WORK *g2r;
};

//==============================================================================
//	extern
//==============================================================================
/* guru2_main.c */
extern void Guru2Main_CommButtonSet( GURU2MAIN_WORK *g2m, u32 btn );
extern void Guru2Main_CommGameDataSet(
		GURU2MAIN_WORK *g2m, const GURU2COMM_GAMEDATA *data );
extern void Guru2Main_CommPlayNoDataSet(
		GURU2MAIN_WORK *g2m, const GURU2COMM_PLAYNO *no );
extern void Guru2Main_CommPlayMaxSet( GURU2MAIN_WORK *g2m, int max );
extern void Guru2Main_TradeNoSet( GURU2MAIN_WORK *g2m, int id, int no );
extern void Guru2Main_GameResultSet(
	GURU2MAIN_WORK *g2m, const GURU2COMM_GAMERESULT *result );

#endif //GURU2_LH_FILE
