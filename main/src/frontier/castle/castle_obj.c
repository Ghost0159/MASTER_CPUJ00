//==============================================================================================
/**
 * @file	castle_obj.c
 * @brief	�u�o�g���L���b�X���vOBJ(�J�[�\���A�A�C�R��)
 * @author	Satoshi Nohara
 * @date	2007.07.05
 */
//==============================================================================================
#include "common.h"
#include "src_os_print.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/snd_tool.h"
#include "poketool/pokeicon.h"

#include "castle_sys.h"
#include "castle_clact.h"
#include "castle_obj.h"
#include "frontier/frontier_tool.h"


//==============================================================================================
//
//	�\���̐錾
//
//==============================================================================================
//OBJ(�J�[�\���A�A�C�R��)
struct _CASTLE_OBJ{
	//u16 init_x;
	//u16 init_y;
	s16 init_x;
	s16 init_y;
	CLACT_WORK_PTR p_clact;			//�Z���A�N�^�[���[�N�|�C���^
};

//�J�[�\���̃p���b�g
enum{
	PAL_OBJ_MOVE = 0,
	PAL_OBJ_STOP,
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
CASTLE_OBJ* CastleObj_Create( CASTLE_CLACT* castle_clact, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, s16 x, s16 y, int bg_pri, const u8* anm_tbl );
void* CastleObj_Delete( CASTLE_OBJ* wk );
void CastleObj_Vanish( CASTLE_OBJ* wk, int flag );
u16 CastleObj_GetObjX( CASTLE_OBJ* wk );
u16 CastleObj_GetObjY( CASTLE_OBJ* wk );
void CastleObj_SetObjPos( CASTLE_OBJ* wk, u16 x, u16 y );
void CastleObj_AnmChg( CASTLE_OBJ* wk, u32 num );
void CastleObj_IconPalChg( CASTLE_OBJ* wk, POKEMON_PARAM* pp );
u16 CastleObj_GetAnmFrame( CASTLE_OBJ* wk );
void CastleObj_SetInitPosOffset( CASTLE_OBJ* wk, int offset_x, int offset_y );
void CastleObj_PokeIconAnmChg( CASTLE_OBJ* wk, u8 anm );
void CastleObj_PokeIconPosSet( CASTLE_OBJ* wk, u8 flag );
BOOL CastleObj_AnmActiveCheck( CASTLE_OBJ* wk );
void CastleObj_PriorityChg( CASTLE_OBJ* wk, u8 pri );
void CastleObj_SetInitXY( CASTLE_OBJ* wk, s16 x, s16 y );
void CastleObj_RankUpEff( CASTLE_OBJ* wk, u16 x, u16 y );


//==============================================================================================
//
//	�֐�
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ�쐬
 *
 * @param	castle_clact	CASTLE_CLACT�^�̃|�C���^
 * @param	data_index		�f�[�^index
 * @param	heapID			�q�[�vID
 *
 * @retval	"CASTLE_OBJ���[�N�ւ̃|�C���^"
 */
//--------------------------------------------------------------
CASTLE_OBJ* CastleObj_Create( CASTLE_CLACT* castle_clact, u32 char_no, u32 pltt_no, u32 cell_no, u32 anm_no, s16 x, s16 y, int bg_pri, const u8* anm_tbl )
{
	CASTLE_OBJ* wk;
	VecFx32	vec;

	wk = sys_AllocMemory( HEAPID_CASTLE, sizeof(CASTLE_OBJ) );		//�������m��
	memset( wk, 0, sizeof(CASTLE_OBJ) );

	wk->init_x		= x;
	wk->init_y		= y;

	wk->p_clact = CastleClact_SetActor( castle_clact, char_no, pltt_no, cell_no, anm_no, 
										0, bg_pri, DISP_MAIN );

	//�����J�[�\�����W���Z�b�g
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);
	CLACT_SetMatrix( wk->p_clact, &vec );

	return wk;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��OBJ���[�N�폜
 *
 * @param	wk		CASTLE_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void* CastleObj_Delete( CASTLE_OBJ* wk )
{
	if( wk == NULL ){
		OS_Printf( "CastleObj_Delete ������NULL\n" );
		GF_ASSERT( 0 );
		return NULL;
	}

	CLACT_Delete( wk->p_clact );

	//CASTLE_OBJ�̃����o�Ń������m�ۂ������̂���������J������

	sys_FreeMemoryEz( wk );
	return NULL;
}

//--------------------------------------------------------------
/**
 * @brief	�o�j�b�V������
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 * @param	flag	0:��`�� 1:�����_���`��	
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleObj_Vanish( CASTLE_OBJ* wk, int flag )
{
	CLACT_SetDrawFlag( wk->p_clact, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��X�ʒu���擾
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
u16 CastleObj_GetObjX( CASTLE_OBJ* wk )
{
	VecFx32	vec;
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	return (vec.x / FX32_ONE);
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\��Y�ʒu���擾
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 *
 * @return	"X"
 */
//--------------------------------------------------------------
u16 CastleObj_GetObjY( CASTLE_OBJ* wk )
{
	VecFx32	vec;
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	return (vec.y / FX32_ONE);
}

//--------------------------------------------------------------
/**
 * @brief	�J�[�\���ʒu���Z�b�g
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleObj_SetObjPos( CASTLE_OBJ* wk, u16 x, u16 y )
{
	VecFx32	vec;

	//���W���擾
	vec = *( CLACT_GetMatrix(wk->p_clact) );
	vec.x = (x * FX32_ONE);
	vec.y = (y * FX32_ONE);

	//���W��ݒ�
	CLACT_SetMatrix( wk->p_clact, &vec );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���؂�ւ�
 *
 * @param	wk		CASTLE_OBJ���[�N�̃|�C���^
 *
 * @retval	"NULL"
 */
//--------------------------------------------------------------
void CastleObj_AnmChg( CASTLE_OBJ* wk, u32 num )
{
	CLACT_SetAnmFrame( wk->p_clact, FX32_ONE );
	//CLACT_AnmFrameSet( wk->p_clact, 0 );		//�A�j���[�V�����t���[�����Z�b�g
	//CLACT_AnmChgCheck( wk->p_clact, num );		//�A�j���[�V�����̃V�[�P���X���`�F���W����
	//�K���ύX(07.11.28)
	CLACT_AnmChg( wk->p_clact, num );			//�A�j���[�V�����̃V�[�P���X���`�F���W����
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�C�R���A�j��
 *
 * @param	wk		CASTLE_OBJ���[�N�̃|�C���^
 * @param	pp		POKEMON_PARAM���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void CastleObj_IconPalChg( CASTLE_OBJ* wk, POKEMON_PARAM* pp )
{
	//�p���b�g�؂�ւ�
	//CLACT_PaletteOffsetChg�֐��̌��ʂɃp���b�g�̓]����擪�p���b�g�i���o�[�����Z
	CLACT_PaletteOffsetChgAddTransPlttNo( wk->p_clact, PokeIconPalNumGetByPP(pp) );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���t���[���擾
 *
 * @param	wk		CASTLE_OBJ���[�N�̃|�C���^
 *
 * @retval	"�A�j���t���[��"
 */
//--------------------------------------------------------------
u16 CastleObj_GetAnmFrame( CASTLE_OBJ* wk )
{
	return CLACT_AnmFrameGet( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	�������W�Ɏw�肵���I�t�Z�b�g�𑫂��č��W�Z�b�g
 *
 * @param	wk		CASTLE_OBJ���[�N�̃|�C���^
 *
 * @retval	none
 */
//--------------------------------------------------------------
void CastleObj_SetInitPosOffset( CASTLE_OBJ* wk, int offset_x, int offset_y )
{
	CastleObj_SetObjPos( wk, wk->init_x+offset_x, wk->init_y+offset_y );
	return;
}

//--------------------------------------------------------------
/**
 * �|�P�����A�C�R���A�j���؂�ւ�
 *
 * @param	awk		�A�N�^�[���[�N
 * @param	anm		�A�j���ԍ�
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleObj_PokeIconAnmChg( CASTLE_OBJ* wk, u8 anm )
{
	Frontier_PokeIconAnmChg( wk->p_clact, anm );
	return;
}

//--------------------------------------------------------------
/**
 * �|�P�����A�C�R���A�j���؂�ւ�
 *
 * @param	awk		�A�N�^�[���[�N
 * @param	flag	1=�I�����Ă���|�P����
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleObj_PokeIconPosSet( CASTLE_OBJ* wk, u8 flag )
{
	Frontier_PokeIconPosSet( wk->p_clact, wk->init_x, wk->init_y, flag );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�A�j���[�V���������`�F�b�N
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 *
 * @retval	TRUE	�A�j���[�V������
 * @retval	FALSE	�X�g�b�v
 */
//--------------------------------------------------------------
BOOL CastleObj_AnmActiveCheck( CASTLE_OBJ* wk )
{
	return CLACT_AnmActiveCheck( wk->p_clact );
}

//--------------------------------------------------------------
/**
 * @brief	BG�ʂƂ̗D��x��ύX
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 * @param	pri		�D��x
 *
 * @retval	none
 */
//--------------------------------------------------------------
void CastleObj_PriorityChg( CASTLE_OBJ* wk, u8 pri )
{
	CLACT_BGPriorityChg( wk->p_clact, pri );
	return;
}

//--------------------------------------------------------------
/**
 * @brief	����XY���Z�b�g
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 * @param	pri		�D��x
 *
 * @retval	none
 */
//--------------------------------------------------------------
void CastleObj_SetInitXY( CASTLE_OBJ* wk, s16 x, s16 y )
{
	wk->init_x = x;
	wk->init_y = y;
	return;
}

//--------------------------------------------------------------
/**
 * @brief	�����N�A�b�v�G�t�F�N�g�\��
 *
 * @param	wk		CASTLE_OBJ�^�̃|�C���^
 *
 * @return	none
 */
//--------------------------------------------------------------
void CastleObj_RankUpEff( CASTLE_OBJ* wk, u16 x, u16 y )
{
	CastleObj_AnmChg( wk, CASTLE_ANM_RANK_UP );
	CastleObj_SetObjPos( wk, x, y );
	CastleObj_Vanish( wk, CASTLE_VANISH_OFF );				//�\��
	Snd_SePlay( SEQ_SE_DP_PIRORIRO2 );
	Snd_SePlay( SEQ_SE_DP_DANSA4 );
	return;
}



