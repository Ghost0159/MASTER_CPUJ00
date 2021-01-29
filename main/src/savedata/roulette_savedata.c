//==============================================================================================
/**
 * @file	roulette_savedata.c
 * @brief	�u�o�g�����[���b�g�v�Z�[�u�f�[�^
 * @author	Satoshi Nohara
 * @date	2007.09.05
 */
//==============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/frontier_savedata.h"
#include "savedata/roulette_savedata.h"
#include "frontier_savedata_local.h"
#include "roulette_savedata_local.h"
#include "gflib/assert.h"
#include "gflib/heapsys.h"
#include "../frontier/roulette_def.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
int ROULETTEDATA_GetWorkSize( void );
void ROULETTEDATA_Init( ROULETTEDATA* wk );
ROULETTEDATA * SaveData_GetRouletteData( SAVEDATA * sv );

BOOL ROULETTEDATA_GetSaveFlag( ROULETTEDATA* wk );
void ROULETTEDATA_SetSaveFlag( ROULETTEDATA* wk, BOOL flag );
void ROULETTEDATA_PutPlayData( ROULETTEDATA* wk, u8 id, u8 param, u8 param2, const void* buf );
u32 ROULETTEDATA_GetPlayData( ROULETTEDATA* wk, u8 id, u8 param, u8 param2, void* buf );

//����
int ROULETTESCORE_GetWorkSize( void );
void ROULETTESCORE_Init( ROULETTESCORE* wk );
ROULETTESCORE * SaveData_GetRouletteScore( SAVEDATA * sv );
void ROULETTESCORE_PutScoreData( ROULETTESCORE* wk, u8 id, u8 param, u8 param2, const void* buf );
u32 ROULETTESCORE_GetScoreData( ROULETTESCORE* wk, u8 id, u8 param, u8 param2, void* buf );


//============================================================================================
//
//	�Z�[�u�f�[�^�V�X�e�����ˑ�����֐�
//
//============================================================================================

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)���[���b�g�Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int ROULETTEDATA_GetWorkSize( void )
{
	return sizeof(ROULETTEDATA);
}

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)���[���b�g���[�N�̏�����
 *
 * @param	wk		ROULETTEDATA�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void ROULETTEDATA_Init( ROULETTEDATA* wk )
{
	//MI_CpuClear32( wk, sizeof(ROULETTEDATA) );
	MI_CpuClear8( wk, sizeof(ROULETTEDATA) );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	���[���b�g�Z�[�u�f�[�^�̎擾(�v���C�f�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	ROULETTEDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
ROULETTEDATA * SaveData_GetRouletteData( SAVEDATA * sv )
{
	//return (ROULETTEDATA*)SaveData_Get( sv, GMDATA_ID_ROULETTE );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->play_roulette;
}


//============================================================================================
//
//	�����o�A�N�Z�X
//
//============================================================================================

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�������Z�[�u�ς݂��ǂ����H
 *
 * @param	wk		ROULETTEDATA�^�̃|�C���^
 *
 * @retval	TRUE	�������Z�[�u����Ă���
 * @retval	FALSE	�Z�[�u����Ă��Ȃ�
 */
//----------------------------------------------------------
BOOL ROULETTEDATA_GetSaveFlag( ROULETTEDATA* wk )
{
	return wk->save_flag;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�Z�[�u��ԃt���O���Z�b�g
 *
 * @param	wk		ROULETTEDATA�^�̃|�C���^
 * @param	flag	TRUE�AFALSE
 *
 * @retval	none
 */
//----------------------------------------------------------
void ROULETTEDATA_SetSaveFlag( ROULETTEDATA* wk, BOOL flag )
{
	wk->save_flag = flag;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�Z�b�g
 *
 * @param	wk		ROULETTEDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void ROULETTEDATA_PutPlayData( ROULETTEDATA* wk, u8 id, u8 param, u8 param2, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case ROULETTEDATA_ID_TYPE:
		wk->type = buf8[0];
		OS_Printf( "wk->type = %d\n", wk->type );
		break;

	//�J�[�\�������_���ړ��t���O
	case ROULETTEDATA_ID_RANDOM_FLAG:
		wk->random_flag = buf8[0];
		OS_Printf( "wk->random_flag = %d\n", wk->random_flag );
		break;

	//�����l�ځH
	case ROULETTEDATA_ID_ROUND:
		wk->round = buf8[0];
		OS_Printf( "wk->round = %d\n", wk->round );
		break;

	//�J�[�\���̃X�s�[�h���x��
	case ROULETTEDATA_ID_CSR_SPEED_LEVEL:
		wk->csr_speed_level = buf8[0];
		OS_Printf( "wk->csr_speed_level = %d\n", wk->csr_speed_level );
		break;

	//�莝���|�P������ITEM
	case ROULETTEDATA_ID_TEMOTI_ITEM:
		wk->item[param] = buf16[0];
		OS_Printf( "wk->item[%d] = %d\n", param, wk->item[param] );
		break;

	//�G�g���[�i�[�f�[�^�C���f�b�N�X
	case ROULETTEDATA_ID_TR_INDEX:
		wk->tr_index[param] = buf16[0];
		OS_Printf( "wk->tr_index[%d] = %d\n", param, wk->tr_index[param] );
		break;

	//�Q�����Ă���莝���|�P�����̈ʒu���
	case ROULETTEDATA_ID_MINE_POKE_POS:
		wk->mine_poke_pos[param] = buf8[0];
		OS_Printf( "wk->mine_poke_pos[%d] = %d\n", param, wk->mine_poke_pos[param] );
		break;

	//�G�|�P�����f�[�^�����Ȃ��悤�ɂ��邽�߂ɕۑ�
	case ROULETTEDATA_ID_ENEMY_POKE_INDEX:
		wk->enemy_poke_index[param] = buf16[0];
		OS_Printf("wk->enemy_poke_index[%d] = %d\n", param, wk->enemy_poke_index[param] );
		break;

	default:
		GF_ASSERT(0);
		break;
	};

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�擾
 *
 * @param	wk		ROULETTEDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 ROULETTEDATA_GetPlayData( ROULETTEDATA* wk, u8 id, u8 param, u8 param2, void* buf )
{
	switch( id ){

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case ROULETTEDATA_ID_TYPE:
		return (u32)wk->type;

	//�J�[�\�������_���ړ��t���O
	case ROULETTEDATA_ID_RANDOM_FLAG:
		return (u32)wk->random_flag;

	//�����l�ځH
	case ROULETTEDATA_ID_ROUND:
		return (u32)wk->round;

	//�J�[�\���X�s�[�h���x��
	case ROULETTEDATA_ID_CSR_SPEED_LEVEL:
		return (u32)wk->csr_speed_level;

	//�莝���|�P������ITEM
	case ROULETTEDATA_ID_TEMOTI_ITEM:
		return (u32)wk->item[param];

	//�G�g���[�i�[�f�[�^�C���f�b�N�X
	case ROULETTEDATA_ID_TR_INDEX:
		return (u32)wk->tr_index[param];

	//�Q�����Ă���莝���|�P�����̈ʒu���
	case ROULETTEDATA_ID_MINE_POKE_POS:
		return wk->mine_poke_pos[param];

	//�G�|�P�����f�[�^�����Ȃ��悤�ɂ��邽�߂ɕۑ�
	case ROULETTEDATA_ID_ENEMY_POKE_INDEX:
		return wk->enemy_poke_index[param];

	default:
		GF_ASSERT(0);
		break;
	};

	return 0;
}


//============================================================================================
//
//	���ъ֘A
//
//============================================================================================

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)���[���b�g�Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int ROULETTESCORE_GetWorkSize( void )
{
	return sizeof(ROULETTESCORE);
}

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)���[���b�g���[�N�̏�����
 *
 * @param	wk		ROULETTEDATA�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void ROULETTESCORE_Init( ROULETTESCORE* wk )
{
	//MI_CpuClear32( wk, sizeof(ROULETTESCORE) );
	MI_CpuClear8( wk, sizeof(ROULETTESCORE) );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	���[���b�g�Z�[�u�f�[�^�̎擾(���уf�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	ROULETTEDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
ROULETTESCORE * SaveData_GetRouletteScore( SAVEDATA * sv )
{
	//return (ROULETTEDATA*)SaveData_Get( sv, GMDATA_ID_ROULETTE );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
	return &data->roulette.score;
}

//----------------------------------------------------------
/**
 * @brief	���уf�[�^�@�Z�b�g
 *
 * @param	wk		ROULETTESCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void ROULETTESCORE_PutScoreData( ROULETTESCORE* wk, u8 id, u8 param, u8 param2, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//7�A��(�N���A)�������t���O
	case ROULETTESCORE_ID_CLEAR_FLAG:
		OS_Printf( "before clear_flag = %d\n", wk->clear_flag );
		if( buf8[0] >= 1 ){
			wk->clear_flag |= (1 << param);					//�Z�b�g
		}else{
			wk->clear_flag &= (0xff ^ (1 << param));		//���Z�b�g
		}
		OS_Printf( "after clear_flag = %d\n", wk->clear_flag );
		break;

	default:
		GF_ASSERT(0);
		break;
	};

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�擾
 *
 * @param	wk		ROULETTESCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 ROULETTESCORE_GetScoreData( ROULETTESCORE* wk, u8 id, u8 param, u8 param2, void* buf )
{
	//OS_Printf( "ROULETTEDATA = %d\n", sizeof(ROULETTEDATA) );
	//OS_Printf( "ROULETTESCORE = %d\n", sizeof(ROULETTESCORE) );

	switch( id ){

	//7�A��(�N���A)�������t���O
	case ROULETTESCORE_ID_CLEAR_FLAG:
		OS_Printf( "get clear_flag = %d\n", wk->clear_flag );
		return (u32)((wk->clear_flag >> param) & 0x01);

	default:
		GF_ASSERT(0);
		break;
	};

	return 0;
}


