//==============================================================================================
/**
 * @file	castle_savedata.c
 * @brief	�u�o�g���L���b�X���v�Z�[�u�f�[�^
 * @author	Satoshi Nohara
 * @date	2007.06.27
 */
//==============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/frontier_savedata.h"
#include "savedata/castle_savedata.h"
#include "frontier_savedata_local.h"
#include "castle_savedata_local.h"
#include "gflib/assert.h"
#include "gflib/heapsys.h"
#include "../frontier/castle_def.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
int CASTLEDATA_GetWorkSize( void );
void CASTLEDATA_Init( CASTLEDATA* wk );
CASTLEDATA * SaveData_GetCastleData( SAVEDATA * sv );

BOOL CASTLEDATA_GetSaveFlag( CASTLEDATA* wk );
void CASTLEDATA_SetSaveFlag( CASTLEDATA* wk, BOOL flag );
void CASTLEDATA_PutPlayData( CASTLEDATA* wk, u8 id, u8 param, u8 param2, const void* buf );
u32 CASTLEDATA_GetPlayData( CASTLEDATA* wk, u8 id, u8 param, u8 param2, void* buf );

//����
int CASTLESCORE_GetWorkSize( void );
void CASTLESCORE_Init( CASTLESCORE* wk );
CASTLESCORE * SaveData_GetCastleScore( SAVEDATA * sv );
void CASTLESCORE_PutScoreData( CASTLESCORE* wk, u8 id, u8 param, u8 param2, const void* buf );
u32 CASTLESCORE_GetScoreData( CASTLESCORE* wk, u8 id, u8 param, u8 param2, void* buf );


//============================================================================================
//
//	�Z�[�u�f�[�^�V�X�e�����ˑ�����֐�
//
//============================================================================================

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)�L���b�X���Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int CASTLEDATA_GetWorkSize( void )
{
	return sizeof(CASTLEDATA);
}

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)�L���b�X�����[�N�̏�����
 *
 * @param	wk		CASTLEDATA�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void CASTLEDATA_Init( CASTLEDATA* wk )
{
	//MI_CpuClear32( wk, sizeof(CASTLEDATA) );
	MI_CpuClear8( wk, sizeof(CASTLEDATA) );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�L���b�X���Z�[�u�f�[�^�̎擾(�v���C�f�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	CASTLEDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
CASTLEDATA * SaveData_GetCastleData( SAVEDATA * sv )
{
	//return (CASTLEDATA*)SaveData_Get( sv, GMDATA_ID_CASTLE );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->play_castle;
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
 * @param	wk		CASTLEDATA�^�̃|�C���^
 *
 * @retval	TRUE	�������Z�[�u����Ă���
 * @retval	FALSE	�Z�[�u����Ă��Ȃ�
 */
//----------------------------------------------------------
BOOL CASTLEDATA_GetSaveFlag( CASTLEDATA* wk )
{
	return wk->save_flag;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�Z�[�u��ԃt���O���Z�b�g
 *
 * @param	wk		CASTLEDATA�^�̃|�C���^
 * @param	flag	TRUE�AFALSE
 *
 * @retval	none
 */
//----------------------------------------------------------
void CASTLEDATA_SetSaveFlag( CASTLEDATA* wk, BOOL flag )
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
 * @param	wk		CASTLEDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void CASTLEDATA_PutPlayData( CASTLEDATA* wk, u8 id, u8 param, u8 param2, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case CASTLEDATA_ID_TYPE:
		wk->type = buf8[0];
		OS_Printf( "wk->type = %d\n", wk->type );
		break;

	//�����l�ځH
	case CASTLEDATA_ID_ROUND:
		wk->round = buf8[0];
		OS_Printf( "wk->round = %d\n", wk->round );
		break;

	//�莝���|�P������HP
	case CASTLEDATA_ID_TEMOTI_HP:
		wk->hp[param] = buf16[0];
		OS_Printf( "wk->hp[%d] = %d\n", param, wk->hp[param] );
		break;

	//�莝���|�P������PP
	case CASTLEDATA_ID_TEMOTI_PP:
		wk->pp[param][param2] = buf8[0];
		OS_Printf( "wk->pp[%d][%d] = %d\n", param, param2, wk->pp[param][param2] );
		break;

	//�莝���|�P������CONDITION
	case CASTLEDATA_ID_TEMOTI_CONDITION:
		wk->condition[param] = buf32[0];
		OS_Printf( "wk->condition[%d] = %d\n", param, wk->condition[param] );
		break;

	//�莝���|�P������ITEM
	case CASTLEDATA_ID_TEMOTI_ITEM:
		wk->item[param] = buf16[0];
		OS_Printf( "wk->item[%d] = %d\n", param, wk->item[param] );
		break;

	//�G�g���[�i�[�f�[�^�C���f�b�N�X
	case CASTLEDATA_ID_TR_INDEX:
		wk->tr_index[param] = buf16[0];
		OS_Printf( "wk->tr_index[%d] = %d\n", param, wk->tr_index[param] );
		break;

	//�Q�����Ă���莝���|�P�����̈ʒu���
	case CASTLEDATA_ID_MINE_POKE_POS:
		wk->mine_poke_pos[param] = buf8[0];
		OS_Printf( "wk->mine_poke_pos[%d] = %d\n", param, wk->mine_poke_pos[param] );
		break;

	//�G�|�P�����f�[�^�����Ȃ��悤�ɂ��邽�߂ɕۑ�
	case CASTLEDATA_ID_ENEMY_POKE_INDEX:
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
 * @param	wk		CASTLEDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 CASTLEDATA_GetPlayData( CASTLEDATA* wk, u8 id, u8 param, u8 param2, void* buf )
{
	switch( id ){

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case CASTLEDATA_ID_TYPE:
		return (u32)wk->type;

	//�����l�ځH
	case CASTLEDATA_ID_ROUND:
		return (u32)wk->round;

	//�莝���|�P������HP
	case CASTLEDATA_ID_TEMOTI_HP:
		return (u32)wk->hp[param];

	//�莝���|�P������PP
	case CASTLEDATA_ID_TEMOTI_PP:
		return (u32)wk->pp[param][param2];

	//�莝���|�P������CONDITION
	case CASTLEDATA_ID_TEMOTI_CONDITION:
		return (u32)wk->condition[param];

	//�莝���|�P������ITEM
	case CASTLEDATA_ID_TEMOTI_ITEM:
		return (u32)wk->item[param];

	//�G�g���[�i�[�f�[�^�C���f�b�N�X
	case CASTLEDATA_ID_TR_INDEX:
		return (u32)wk->tr_index[param];

	//�Q�����Ă���莝���|�P�����̈ʒu���
	case CASTLEDATA_ID_MINE_POKE_POS:
		return wk->mine_poke_pos[param];

	//�G�|�P�����f�[�^�����Ȃ��悤�ɂ��邽�߂ɕۑ�
	case CASTLEDATA_ID_ENEMY_POKE_INDEX:
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
 * @brief	(���уf�[�^)�L���b�X���Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int CASTLESCORE_GetWorkSize( void )
{
	return sizeof(CASTLESCORE);
}

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)�L���b�X�����[�N�̏�����
 *
 * @param	wk		CASTLESCORE�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void CASTLESCORE_Init( CASTLESCORE* wk )
{
	int i,j;

	//MI_CpuClear32( wk, sizeof(CASTLESCORE) );
	MI_CpuClear8( wk, sizeof(CASTLESCORE) );

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�L���b�X���Z�[�u�f�[�^�̎擾(���уf�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	CASTLEDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
CASTLESCORE * SaveData_GetCastleScore( SAVEDATA * sv )
{
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->castle.score;
}

//----------------------------------------------------------
/**
 * @brief	���уf�[�^�@�Z�b�g
 *
 * @param	wk		CASTLESCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void CASTLESCORE_PutScoreData( CASTLESCORE* wk, u8 id, u8 param, u8 param2, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//7�A��(�N���A)�������t���O
	case CASTLESCORE_ID_CLEAR_FLAG:
		OS_Printf( "before clear_flag = %d\n", wk->clear_flag );
		if( buf8[0] >= 1 ){
			wk->clear_flag |= (1 << param);					//�Z�b�g
		}else{
			wk->clear_flag &= (0xff ^ (1 << param));		//���Z�b�g
		}
		OS_Printf( "after clear_flag = %d\n", wk->clear_flag );
		break;

	//�����󂯂����t���O
	case CASTLESCORE_ID_GUIDE_FLAG:
		wk->guide_flag = 1;
		OS_Printf( "guide_flag = %d\n", wk->guide_flag );
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
 * @param	wk		CASTLESCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 CASTLESCORE_GetScoreData( CASTLESCORE* wk, u8 id, u8 param, u8 param2, void* buf )
{
	OS_Printf( "CASTLEDATA = %d\n", sizeof(CASTLEDATA) );
	OS_Printf( "CASTLESCORE = %d\n", sizeof(CASTLESCORE) );

	switch( id ){

	//7�A��(�N���A)�������t���O
	case CASTLESCORE_ID_CLEAR_FLAG:
		OS_Printf( "get clear_flag = %d\n", wk->clear_flag );
		return (u32)((wk->clear_flag >> param) & 0x01);

	//�����󂯂����t���O
	case CASTLESCORE_ID_GUIDE_FLAG:
		OS_Printf( "guide_flag = %d\n", wk->guide_flag );
		return (u32)wk->guide_flag;

	default:
		GF_ASSERT(0);
		break;
	};

	return 0;
}


