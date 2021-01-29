//==============================================================================
/**
 * @file	gds_main.h
 * @brief	GDS�v���b�N���䃁�C���̃w�b�_
 * @author	matsuda
 * @date	2008.01.17(��)
 */
//==============================================================================
#ifndef __GDS_MAIN_H__
#define __GDS_MAIN_H__


//==============================================================================
//	�\���̒�`
//==============================================================================
///GDS�v���b�N������Ăяo���Ƃ��Ɉ����n���p�����[�^�\����
typedef struct{
	FIELDSYS_WORK *fsys;	///<�t�B�[���h�V�X�e���ւ̃|�C���^
	SAVEDATA *savedata;		///<�Z�[�u�f�[�^�ւ̃|�C���^
	int connect;			///<0�ȊO:����ڑ�(�ڑ����́u�͂�/�������v�I�𖳂�)�B0�ȊO:2�x�ڈȍ~
	u8 gds_mode;			///<BR_MODE_GDS_???
}GDSPROC_PARAM;

///GDS�v���b�N����̃��C���\����
typedef struct{
	GDSPROC_PARAM *proc_param;		///<�Ăяo���p�����[�^�ւ̃|�C���^
	BOOL connect_success;			///<TRUE:�l�b�g�ڑ����B�@FALSE:�ڑ����Ă��Ȃ�
	PROC *sub_proc;					///<���s���̃T�u�v���Z�X

	void 			*heapPtr;		///<NitroDWC�ɓn���q�[�v���[�N�̉���p�|�C���^
	NNSFndHeapHandle heapHandle;	///<heapPtr��32�o�C�g�A���C�����g�ɍ��킹���|�C���^

	DWCInetControl   stConnCtrl;	// DWC�ڑ����[�N
	
	int comm_initialize_ok;			///<TRUE:�ʐM�������ς�

	
	//Wifi�ڑ���ʂ̌��ʂ��󂯎��
	BOOL ret_connect;				///<TRUE:Wifi�ڑ��s�����B�@FALSE:�ڑ����Ȃ�����
}GDSPROC_MAIN_WORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================

extern PROC_RESULT GdsMainProc_Init( PROC * proc, int * seq );
extern PROC_RESULT GdsMainProc_Main( PROC * proc, int * seq );
extern PROC_RESULT GdsMainProc_End( PROC * proc, int * seq );

//-- gds_connect.h --//
extern PROC_RESULT GdsConnectProc_Init( PROC * proc, int * seq );
extern PROC_RESULT GdsConnectProc_Main( PROC * proc, int * seq );
extern PROC_RESULT GdsConnectProc_End( PROC * proc, int * seq );


#endif	//__GDS_MAIN_H__
