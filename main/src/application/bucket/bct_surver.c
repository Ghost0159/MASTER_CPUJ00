//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		bct_surver.c
 *	@brief		�T�[�o�[
 *	@author		tomoya takahashi
 *	@data		2007.06.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "bct_surver.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�T�[�o�[���[�N
//=====================================
typedef struct _BCT_SURVER{
	s32 time;
	BOOL countdown;	// �J�E���g�_�E�����邩
	u32 comm_num;
	BCT_SCORE_COMM score;
	u32 score_msk;						// �X�R�A���i�[����NETID�}�X�N
	u32 ranktbl[ BCT_PLAYER_NUM ];		// ���ʌv�Z�p�e�[�u��
	const BCT_GAMEDATA* cp_gamedata;	// �Q�[���f�[�^
	u16 nutsin_count;					// �؂̎�����������
	u8	now_tbl_idx;					// ���̃Q�[���i�K 
	u8	tbl_idx_change;					// �e�[�u���̃C���ŃN�X���ς����
} ;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static u32 BCT_SURVER_ScoreSetNumGet( const BCT_SURVER* cp_wk );


//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[���[�N�̍쐬
 *
 *	@param	heapID		�g�p����q�[�v
 *	@param	timeover	���Ԑ���
 *	@param	comm_num	�ʐM�l��
 *	@param	cp_gamedata	�Q�[���f�[�^
 *
 *	@return	�쐬�������[�N
 */
//-----------------------------------------------------------------------------
BCT_SURVER* BCT_SURVER_Init( u32 heapID, u32 timeover, u32 comm_num, const BCT_GAMEDATA* cp_gamedata )
{
	BCT_SURVER* p_wk;

	p_wk = sys_AllocMemory( heapID, sizeof(BCT_SURVER) );
	memset( p_wk, 0, sizeof(BCT_SURVER) );

	p_wk->time			= timeover;
	p_wk->comm_num		= comm_num;
	p_wk->cp_gamedata	= cp_gamedata;
	p_wk->countdown		= TRUE;
	
	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[���[�N�̔j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void BCT_SURVER_Delete( BCT_SURVER* p_wk )
{
	sys_FreeMemoryEz( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[���[�N���C��
 *
 *	@param	p_wk	���[�N
 *
 *	@retval	TRUE	�r��
 *	@retval	FALSE	�I��
 *
// FALSE���A���Ă�����I��
// �I�����߂ƃX�R�A���݂�Ȃɑ���
 */
//-----------------------------------------------------------------------------
BOOL BCT_SURVER_Main( BCT_SURVER* p_wk )
{
	int i;
	
#ifndef BCT_DEBUG_NOT_TIMECOUNT
	if( (p_wk->time > 0) && (p_wk->countdown == TRUE) ){
		p_wk->time --;
	}
#endif
	if( p_wk->time <= 0 ){
		return FALSE;
	}

	// ���̒i�K�`�F�b�N
	for( i=p_wk->now_tbl_idx; i<p_wk->cp_gamedata->tblnum; i++ ){
		if( p_wk->cp_gamedata->p_tbl[i].nuts_num >= p_wk->nutsin_count ){
			break;
		}
	}

	// �C���f�b�N�X��p_wk->cp_gamedata.tblnum�ɂȂ��Ă��܂�����
	// �P�߂�
	if( i == p_wk->cp_gamedata->tblnum ){
		i--;
	}
	// �C���f�b�N�X���ς������ݒ�
	if( i != p_wk->now_tbl_idx ){
		p_wk->now_tbl_idx = i;

		// ����ɒi�K���ς�����t���O��ݒ�
		p_wk->tbl_idx_change = TRUE;
//		OS_TPrintf( "game data idx chenge %d\n", p_wk->now_tbl_idx );
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[�����x���̕ύX���`�F�b�N
 *
 *	@param	cp_wk	���[�N
 *
 *	@retval	TRUE	���x�����ς���Ă�
 *	@retval	FALSE	���x�����ς���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL BCT_SURVER_CheckGameLevelChange( const BCT_SURVER* cp_wk )
{
	return cp_wk->tbl_idx_change;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[�����x���̕ύX�t���O���N���A
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void BCT_SURVER_ClearGameLevelChange( BCT_SURVER* p_wk )
{
	p_wk->tbl_idx_change = FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[�����x���̕ύX�t���O���N���A
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	���x��
 */
//-----------------------------------------------------------------------------
u32	BCT_SURVER_GetGameLevel( const BCT_SURVER* cp_wk )
{
	return cp_wk->now_tbl_idx;
}

//----------------------------------------------------------------------------
/**
 *	@brief	��M�؂̎��f�[�^�ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	cp_data	�؂̎��f�[�^
 *	@param	plno	�l�b�gID
 */
//-----------------------------------------------------------------------------
void BCT_SURVER_SetNutData( BCT_SURVER* p_wk, const BCT_NUT_COMM* cp_data, u32 plno )
{
	if( cp_data->in_flag ){
		// ���J�E���g
		p_wk->nutsin_count ++;
//		OS_TPrintf( "nutsin count %d\n", p_wk->nutsin_count );
	} 
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�R�A��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	score		�X�R�A
 *	@param	plno		�l�b�gID
 */
//-----------------------------------------------------------------------------
void BCT_SURVER_ScoreSet( BCT_SURVER* p_wk, u32 score, u32 plno )
{
	int innum;
	int i;

	// �ő�l�I�[�o�[�`�F�b�N
	if( score > BCT_SCORE_MAX ){
		score = BCT_SCORE_MAX;
	}
	
	// �܂��o�^���Ă��Ȃ����`�F�b�N
	if( (p_wk->score_msk & (1 << plno)) == 0 ){

		p_wk->score.score[ plno ] = score;
//		OS_Printf( "p_wk->score_msk %x  score = %d\n", p_wk->score_msk, score );
		// �擾����
		p_wk->score_msk |= (1 << plno);
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�݂�Ȃ���X�R�A����M���������`�F�b�N����
 *
 *	@param	 cp_wk		���[�N
 *
 *	@retval	TRUE	�݂�Ȃ����M����
 *	@retval	FALSE	�݂�Ȃ����M���ĂȂ�
 */
//-----------------------------------------------------------------------------
BOOL BCT_SURVER_ScoreAllUserGetCheck( const BCT_SURVER* cp_wk )
{
	int num;
	num = BCT_SURVER_ScoreSetNumGet( cp_wk );
	if( num >= cp_wk->comm_num ){ 
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�݂�Ȃ̃X�R�A���擾����
 *
 *	@param	cp_wk		���[�N
 *	@param	p_data		�݂�Ȃ̃X�R�A�i�[��
 */
//-----------------------------------------------------------------------------
void BCT_SURVER_ScoreGet( BCT_SURVER* p_wk, BCT_SCORE_COMM* p_data )
{
	*p_data = p_wk->score;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�J�E���g�_�E���ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	flag	�t���O
 */
//-----------------------------------------------------------------------------
void BCT_SURVER_SetCountDown( BCT_SURVER* p_wk, BOOL flag )
{
	p_wk->countdown = flag;
}




//----------------------------------------------------------------------------
/**
 *	@brief	�X�R�A��ݒ肵���������߂�
 *
 *	@param	cp_wk	���[�N
 *
 *	@return	�X�R�A��ݒ肵����
 */
//-----------------------------------------------------------------------------
static u32 BCT_SURVER_ScoreSetNumGet( const BCT_SURVER* cp_wk )
{
	int i;
	int num;
	
	num = 0;
	for( i=0; i<BCT_PLAYER_NUM; i++ ){
		if( (cp_wk->score_msk & (1<<i)) != 0 ){
			num ++;
		}
	}
	return num;
}
