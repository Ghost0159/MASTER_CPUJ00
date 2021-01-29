//==============================================================================
/**
 * @file	fss_task.h
 * @brief	�t�����e�B�A�p�X�N���v�g���z�}�V���̃w�b�_
 * @author	matsuda
 * @date	2007.03.29(��)
 */
//==============================================================================
#ifndef __FSS_TASK_H__
#define __FSS_TASK_H__

#include "gflib/msg_print.h"
#include "system/msgdata.h"

#include "fs_usescript.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�X�N���v�g����^�X�N�ő吔
#define FSS_TASK_MAX		(8)

#define FSST_STACK_MAX	( 20 )	// ���z�}�V���̃X�^�b�N�̐[��


//==============================================================================
//	�^��`
//==============================================================================
typedef struct _FS_SYSTEM FS_SYSTEM;

typedef	u8	FSS_CODE;				// �X�N���v�g�A�C�x���g�f�[�^�̌^�錾

typedef struct _FSS_TASK FSS_TASK;
typedef BOOL ( * FRSCR_CMD)(FSS_TASK *);

typedef BOOL ( * FSST_WAIT_FUNC)(FSS_TASK *);

//==============================================================================
//	�\���̒�`
//==============================================================================
///�X�N���v�g����^�X�N�\����
typedef struct _FSS_TASK{
	FS_SYSTEM *fss;	///<�X�N���v�g���䃏�[�N�ւ̃|�C���^
	
	u32 seqno;
	u16 local_work[FSW_LOCAL_MAX - FSW_LOCAL_START];

	u8	SP;									// �X�^�b�N�|�C���^
	u8	status;								// ������
	const FSS_CODE * PC;						// ���݂̃X�N���v�g�A�h���X
	const FSS_CODE * array[FSST_STACK_MAX];	// �X�^�b�N
	const FRSCR_CMD * command_table;			// �X�N���v�g�R�}���h�e�[�u���J�n�ʒu
	u32 cmd_max;							// �R�}���h����l
	u16 reg[FSW_REG_MAX - FSW_REG_START];	// �ėp���W�X�^

	MSGDATA_MANAGER* msgman;				// ���b�Z�[�W�}�l�[�W���[
	FSS_CODE * pScript;						// ���[�h�����X�N���v�g
	FSST_WAIT_FUNC routine;					// �E�F�C�g�֐�

	u8	cmp_flag;							// �v�Z���ʃt���O

//	void * event_work;						// �R�}���h�ȂǂŎQ�Ƃ��郏�[�N�̃|�C���^

}FSS_TASK;


// u8�T�C�Y�̃f�[�^���擾���邽�߂̃}�N��
#define FSSTGetU8(core)	(*((core)->PC++))


//==============================================================================
//	�O���f�[�^�錾
//==============================================================================
extern const FRSCR_CMD FSSCmdTable[];
extern const u32 FSSCmdTableMax;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void FSST_Init( FSS_TASK * core, const FRSCR_CMD * cmd_tbl, u32 cmd_max);
extern u8 FSST_Start( FSS_TASK * core, const FSS_CODE * start );
extern void FSST_SetWait( FSS_TASK * core, FSST_WAIT_FUNC func );
extern void FSST_End( FSS_TASK * core );
extern void FSST_SetWork( FSS_TASK * core, void * work );
extern u8 FSST_Control( FSS_TASK * core );
extern u8 FSSTStackPush( FSS_TASK * core, const FSS_CODE * val );
extern const FSS_CODE * FSSTStackPop( FSS_TASK * core );
extern void FSSTJump( FSS_TASK * core, FSS_CODE * adrs );
extern void FSSTCall( FSS_TASK * core, FSS_CODE * adrs );
extern void FSSTRet( FSS_TASK * core );
extern u16 FSSTGetU16( FSS_TASK * core );
extern u32 FSSTGetU32( FSS_TASK * core );


#endif	//__FSS_TASK_H__

