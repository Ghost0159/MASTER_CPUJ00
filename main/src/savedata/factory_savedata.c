//==============================================================================================
/**
 * @file	factory_savedata.c
 * @brief	�u�o�g���t�@�N�g���[�v�Z�[�u�f�[�^
 * @author	Satoshi Nohara
 * @date	2007.03.28
 */
//==============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/frontier_savedata.h"
#include "savedata/factory_savedata.h"
#include "frontier_savedata_local.h"
#include "factory_savedata_local.h"
#include "gflib/assert.h"
#include "gflib/heapsys.h"
#include "../frontier/factory_def.h"


//============================================================================================
//
//	�Z�[�u�f�[�^�V�X�e�����ˑ�����֐�
//
//============================================================================================
int FACTORYDATA_GetWorkSize( void );
void FACTORYDATA_Init( FACTORYDATA* wk );
//FACTORYDATA* FACTORYDATA_AllocWork( int heapID );
FACTORYDATA * SaveData_GetFactoryData( SAVEDATA * sv );

//����
int FACTORYSCORE_GetWorkSize( void );
void FACTORYSCORE_Init( FACTORYSCORE* wk );
void FACTORYSCORE_PutScoreData( FACTORYSCORE* wk, u8 id, u8 param, const void* buf );
u32 FACTORYSCORE_GetScoreData( FACTORYSCORE* wk, u8 id, u8 param, void* buf );
FACTORYSCORE * SaveData_GetFactoryScore( SAVEDATA * sv );

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)�t�@�N�g���[�Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int FACTORYDATA_GetWorkSize( void )
{
	return sizeof(FACTORYDATA);
}

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)�t�@�N�g���[���[�N�̏�����
 *
 * @param	wk		FACTORYDATA�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void FACTORYDATA_Init( FACTORYDATA* wk )
{
	//MI_CpuClear32( wk, sizeof(FACTORYDATA) );
	MI_CpuClear8( wk, sizeof(FACTORYDATA) );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�t�@�N�g���[���[�N�̏�����
 *
 * @param	heapID		�������m�ۂ������Ȃ��q�[�v�w��
 *
 * @return	wk			�擾�������[�N�ւ̃|�C���^
 */
//----------------------------------------------------------
#if 0
FACTORYDATA* FACTORYDATA_AllocWork( int heapID )
{
	FACTORYDATA* wk = sys_AllocMemory( heapID, sizeof(FACTORYDATA) );
	FACTORYDATA_Init( wk );
	FACTORYSCORE_Init( wk );
	return wk;
}
#endif

//----------------------------------------------------------
/**
 * @brief	�t�@�N�g���[�Z�[�u�f�[�^�̎擾(�v���C�f�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	FACTORYDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
FACTORYDATA * SaveData_GetFactoryData( SAVEDATA * sv )
{
	//return (FACTORYDATA*)SaveData_Get( sv, GMDATA_ID_FACTORY );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->play_factory;
}


//============================================================================================
//
//	�����o�A�N�Z�X
//
//============================================================================================
BOOL FACTORYDATA_GetSaveFlag( FACTORYDATA* wk );
void FACTORYDATA_SetSaveFlag( FACTORYDATA* wk, BOOL flag );
void FACTORYDATA_PutPlayData( FACTORYDATA* wk, u8 id, u8 param, const void* buf );
u32 FACTORYDATA_GetPlayData( FACTORYDATA* wk, u8 id, u8 param, void* buf );

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�������Z�[�u�ς݂��ǂ����H
 *
 * @param	wk		FACTORYDATA�^�̃|�C���^
 *
 * @retval	TRUE	�������Z�[�u����Ă���
 * @retval	FALSE	�Z�[�u����Ă��Ȃ�
 */
//----------------------------------------------------------
BOOL FACTORYDATA_GetSaveFlag( FACTORYDATA* wk )
{
	return wk->save_flag;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�Z�[�u��ԃt���O���Z�b�g
 *
 * @param	wk		FACTORYDATA�^�̃|�C���^
 * @param	flag	TRUE�AFALSE
 *
 * @retval	none
 */
//----------------------------------------------------------
void FACTORYDATA_SetSaveFlag( FACTORYDATA* wk, BOOL flag )
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
 * @param	wk		FACTORYDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void FACTORYDATA_PutPlayData( FACTORYDATA* wk, u8 id, u8 param, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//LV50�A�I�[�v��
	case FACTORYDATA_ID_LEVEL:
		wk->level = buf8[0];
		break;

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case FACTORYDATA_ID_TYPE:
		wk->type = buf8[0];
		break;

	//�����l�ځH
	case FACTORYDATA_ID_ROUND:
		wk->round = buf8[0];
		//OS_Printf( "buf8[0] = %d\n", buf8[0] );
		//OS_Printf( "wk->round = %d\n", wk->round );
		break;

	//
	case FACTORYDATA_ID_TR_INDEX:
		wk->tr_index[param] = buf16[0];
		break;

	//�莝��
	case FACTORYDATA_ID_TEMOTI_POKE_INDEX:
		wk->temoti_poke_index[param] = buf16[0];
		break;

	case FACTORYDATA_ID_TEMOTI_POW_RND:
		wk->temoti_pow_rnd[param] = buf8[0];
		break;

	case FACTORYDATA_ID_TEMOTI_PERSONAL_RND:
		wk->temoti_personal_rnd[param] = buf32[0];
		break;

	//�G
	case FACTORYDATA_ID_ENEMY_POKE_INDEX:
		wk->enemy_poke_index[param] = buf16[0];
		break;

	case FACTORYDATA_ID_ENEMY_POW_RND:
		wk->enemy_pow_rnd[param] = buf8[0];
		break;

	case FACTORYDATA_ID_ENEMY_PERSONAL_RND:
		wk->enemy_personal_rnd[param] = buf32[0];
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
 * @param	wk		FACTORYDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 FACTORYDATA_GetPlayData( FACTORYDATA* wk, u8 id, u8 param, void* buf )
{
	switch( id ){

	case FACTORYDATA_ID_TYPE:
		return (u32)wk->type;

	case FACTORYDATA_ID_LEVEL:
		return (u32)wk->level;

	case FACTORYDATA_ID_ROUND:
		return (u32)wk->round;

	case FACTORYDATA_ID_TR_INDEX:
		return (u32)wk->tr_index[param];

	//�莝��
	case FACTORYDATA_ID_TEMOTI_POKE_INDEX:
		return (u32)wk->temoti_poke_index[param];

	case FACTORYDATA_ID_TEMOTI_POW_RND:
		return (u32)wk->temoti_pow_rnd[param];

	case FACTORYDATA_ID_TEMOTI_PERSONAL_RND:
		return (u32)wk->temoti_personal_rnd[param];

	//�G
	case FACTORYDATA_ID_ENEMY_POKE_INDEX:
		return (u32)wk->enemy_poke_index[param];

	case FACTORYDATA_ID_ENEMY_POW_RND:
		return (u32)wk->enemy_pow_rnd[param];

	case FACTORYDATA_ID_ENEMY_PERSONAL_RND:
		return (u32)wk->enemy_personal_rnd[param];

	};

	return 0;
}


//==============================================================================================
//
//	���ъ֘A
//
//==============================================================================================

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)�t�@�N�g���[�Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int FACTORYSCORE_GetWorkSize( void )
{
	return sizeof(FACTORYSCORE);
}

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)�t�@�N�g���[���[�N�̏�����
 *
 * @param	wk		FACTORYDATA�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void FACTORYSCORE_Init( FACTORYSCORE* wk )
{
	//MI_CpuClear32( wk, sizeof(FACTORYSCORE) );
	MI_CpuClear8( wk, sizeof(FACTORYSCORE) );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	���уf�[�^�@�Z�b�g
 *
 * @param	wk		FACTORYSCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void FACTORYSCORE_PutScoreData( FACTORYSCORE* wk, u8 id, u8 param, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//7�A��(�N���A)�������t���O
	case FACTORYSCORE_ID_CLEAR_FLAG:
		//wk->clear_flag = buf8[0];
		OS_Printf( "before clear_flag = %d\n", wk->clear_flag );
		if( buf8[0] >= 1 ){
			wk->clear_flag |= (1 << param);					//�Z�b�g
		}else{
			wk->clear_flag &= (0xff ^ (1 << param));		//���Z�b�g
		}
		OS_Printf( "after clear_flag = %d\n", wk->clear_flag );
		break;

	};

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	���уf�[�^�@�擾
 *
 * @param	wk		FACTORYSCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 FACTORYSCORE_GetScoreData( FACTORYSCORE* wk, u8 id, u8 param, void* buf )
{
	//OS_Printf( "FACTORYDATA = %d\n", sizeof(FACTORYDATA) );
	//OS_Printf( "FACTORYSCORE = %d\n", sizeof(FACTORYSCORE) );

	switch( id ){

	case FACTORYSCORE_ID_CLEAR_FLAG:
		//return (u32)wk->clear_flag;
		OS_Printf( "get clear_flag = %d\n", wk->clear_flag );
		return (u32)((wk->clear_flag >> param) & 0x01);

	};

	return 0;
}

//----------------------------------------------------------
/**
 * @brief	�t�@�N�g���[�Z�[�u�f�[�^�̎擾(���уf�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	FACTORYSCORE�ւ̃|�C���^
 */
//----------------------------------------------------------
FACTORYSCORE * SaveData_GetFactoryScore( SAVEDATA * sv )
{
	//return (FACTORYDATA*)SaveData_Get( sv, GMDATA_ID_FACTORY );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->factory.score;
}


