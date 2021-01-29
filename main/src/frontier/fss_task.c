//==============================================================================
/**
 * @file	fss_task.c
 * @brief	�t�����e�B�A�p�X�N���v�g���z�}�V��
 * @author	matsuda
 * @date	2007.03.29(��)
 */
//==============================================================================
#include "common.h"
#include "frontier_types.h"
#include "fss_task.h"

//==============================================================================
//	�萔��`
//==============================================================================
// ���z�}�V���̓����Ԓ�`
enum{
	FSSTSTAT_READY,		// ��~(����I���j
	FSSTSTAT_RUN,		// ���쒆
	FSSTSTAT_WAIT,		// �҂���ԁi�`�F�b�N���[�`���Ăяo���j
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
void FSST_Init( FSS_TASK * core, const FRSCR_CMD * cmd_tbl, u32 cmd_max);
u8 FSST_Start( FSS_TASK * core, const FSS_CODE * start );
void FSST_SetWait( FSS_TASK * core, FSST_WAIT_FUNC func );
void FSST_End( FSS_TASK * core );
void FSST_SetWork( FSS_TASK * core, void * work );
u8 FSST_Control( FSS_TASK * core );
u8 FSSTStackPush( FSS_TASK * core, const FSS_CODE * val );
const FSS_CODE * FSSTStackPop( FSS_TASK * core );
void FSSTJump( FSS_TASK * core, FSS_CODE * adrs );
void FSSTCall( FSS_TASK * core, FSS_CODE * adrs );
void FSSTRet( FSS_TASK * core );
u16 FSSTGetU16( FSS_TASK * core );
u32 FSSTGetU32( FSS_TASK * core );



//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V��������
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	cmd_tbl		���߃e�[�u���J�n�A�h���X
 * @param	cmd_max		���߃e�[�u���̑傫��
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSST_Init( FSS_TASK * core, const FRSCR_CMD * cmd_tbl, u32 cmd_max)
{
	u32	i;

	core->status = FSSTSTAT_READY;
	core->PC = NULL;
	core->SP = 0;
	core->routine = NULL;
	core->command_table = cmd_tbl;
	core->cmd_max = cmd_max;
	for( i=0; i<FSW_REG_WORK_MAX; i++ ){
		core->reg[i] = 0;
	}
	for( i=0; i<FSST_STACK_MAX; i++ ){
		core->array[i] = NULL;
	}

//	core->event_work = NULL;		//(06.07.20)
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V���ɃR�[�h��ݒ�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	start		���s�R�[�h�̊J�n�A�h���X
 *
 * @return	���TRUE
 */
//--------------------------------------------------------------------------------------------
u8 FSST_Start( FSS_TASK * core, const FSS_CODE * start )
{
	core->PC = start;
	core->status = FSSTSTAT_RUN;

	return TRUE;
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V�����E�F�C�g��Ԃɐݒ�
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	func		�E�F�C�g�֐�
 *
 * @return	none
 *
 * @li	TRUE���Ԃ��Ă���܂ŃE�F�C�g�֐��𖈉�Ăт���
 */
//--------------------------------------------------------------------------------------------
void FSST_SetWait( FSS_TASK * core, FSST_WAIT_FUNC func )
{
	core->status = FSSTSTAT_WAIT;
	core->routine = func;
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V�����s�I��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSST_End( FSS_TASK * core )
{
	core->status = FSSTSTAT_READY;
	core->PC = NULL;
}

//--------------------------------------------------------------------------------------------
/**
 * �R�}���h�ȂǂŎQ�Ƃ��郏�[�N���Z�b�g
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	work		���[�N�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSST_SetWork( FSS_TASK * core, void * work )
{
//	core->event_work = work;
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V�����䃁�C��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @retval	"TRUE = ���s��"
 * @retval	"FALSE = ��~���E���s�I��"
 */
//--------------------------------------------------------------------------------------------
u8 FSST_Control( FSS_TASK * core )
{
	u16 code;

	switch( core->status ){
	case FSSTSTAT_READY:
		return FALSE;

	case FSSTSTAT_WAIT:
		if( core->routine != NULL ){
			if( core->routine(core) == TRUE ){
				core->status = FSSTSTAT_RUN;
				core->routine = NULL;
			}
			return TRUE;
		}
		core->status = FSSTSTAT_RUN;
		/* FALL THROUGH */
		//break;
		
	case FSSTSTAT_RUN:
		while( TRUE ){
			if( core->PC == NULL ){
				core->status = FSSTSTAT_READY;
				return FALSE;
			}

			code = FSSTGetU16( core );

			//OS_Printf( "*********code = %d\n", code );
			if( code >= core->cmd_max ) {
				GF_ASSERT_MSG(0, "command error %04x:%08x\n",code, core->PC - 2);
				core->status = FSSTSTAT_READY;
				return FALSE;
			}
			if( core->command_table[code]( core ) == 1 ){
				break;
			}
		}
		break;
	}
	return TRUE;
}


//============================================================================================
// ���z�}�V������p�T�u���[�`��
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V���X�^�b�N�v�b�V��
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	val
 *
 * @retval	"0 = ����"
 * @retval	"1 = �G���["
 */
//--------------------------------------------------------------------------------------------
u8 FSSTStackPush( FSS_TASK * core, const FSS_CODE * val )
{
	if( core->SP + 1 >= FSST_STACK_MAX ){ return 1; }
	core->array[core->SP] = val;
	core->SP ++;

	return 0;
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V���X�^�b�N�|�b�v
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return
 */
//--------------------------------------------------------------------------------------------
const FSS_CODE * FSSTStackPop( FSS_TASK * core )
{
	if( core->SP == 0 ){ return NULL; }
	core->SP --;

	return core->array[core->SP];
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V���W�����v����
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	adrs		�W�����v��A�h���X
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSSTJump( FSS_TASK * core, FSS_CODE * adrs )
{
	core->PC = adrs;
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V���R�[������
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 * @param	adrs		�Ăяo���A�h���X
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSSTCall( FSS_TASK * core, FSS_CODE * adrs )
{
	FSSTStackPush( core, core->PC );
	core->PC = adrs;
}

//--------------------------------------------------------------------------------------------
/**
 * ���z�}�V�����^�[������
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void FSSTRet( FSS_TASK * core )
{
	core->PC = FSSTStackPop( core );
}


//--------------------------------------------------------------------------------------------
/**
 * PC�̃A�h���X����16bit(2byte)�f�[�^�擾
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	�擾�f�[�^
 */
//--------------------------------------------------------------------------------------------
u16 FSSTGetU16( FSS_TASK * core )
{
	u16	val;

	val = (u16)FSSTGetU8( core );
	val += (u16)FSSTGetU8( core ) << 8;
	return val;
}

//--------------------------------------------------------------------------------------------
/**
 * PC�̃A�h���X����32bit(4byte)�f�[�^�擾
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	�擾�f�[�^
 */
//--------------------------------------------------------------------------------------------
u32 FSSTGetU32( FSS_TASK * core )
{
	u32	val;
	u8	a,b,c,d;

	a = FSSTGetU8( core );
	b = FSSTGetU8( core );
	c = FSSTGetU8( core );
	d = FSSTGetU8( core );

	val = 0;
	val += (u32)d;
	val <<= 8;
	val += (u32)c;
	val <<= 8;
	val += (u32)b;
	val <<= 8;
	val += (u32)a;

	return val;
}

