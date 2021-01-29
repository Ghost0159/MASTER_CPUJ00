//==============================================================================================
/**
 * @file	stage_savedata.c
 * @brief	�u�o�g���X�e�[�W�v�Z�[�u�f�[�^
 * @author	Satoshi Nohara
 * @date	2007.06.07
 */
//==============================================================================================
#include "common.h"
#include "savedata/savedata_def.h"
#include "savedata/savedata.h"
#include "savedata/frontier_savedata.h"
#include "savedata/stage_savedata.h"
#include "frontier_savedata_local.h"
#include "stage_savedata_local.h"
#include "gflib/assert.h"
#include "gflib/heapsys.h"
#include "../frontier/stage_def.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
int STAGEDATA_GetWorkSize( void );
void STAGEDATA_Init( STAGEDATA* wk );
STAGEDATA * SaveData_GetStageData( SAVEDATA * sv );

BOOL STAGEDATA_GetSaveFlag( STAGEDATA* wk );
void STAGEDATA_SetSaveFlag( STAGEDATA* wk, BOOL flag );
void STAGEDATA_PutPlayData( STAGEDATA* wk, u8 id, u8 param, u8 param2, const void* buf );
u32 STAGEDATA_GetPlayData( STAGEDATA* wk, u8 id, u8 param, u8 param2, void* buf );

u8 Stage_GetTypeLevel( u8 csr_pos, u8* p_rank );
void Stage_SetTypeLevel( u8 csr_pos, u8* p_rank, u8 num );
void Stage_ClearTypeLevel( u8* p_rank );

//����
int STAGESCORE_GetWorkSize( void );
void STAGESCORE_Init( STAGESCORE* wk );
STAGESCORE * SaveData_GetStageScore( SAVEDATA * sv );
void STAGESCORE_PutScoreData( STAGESCORE* wk, u8 id, u8 param, u8 param2, const void* buf );
u32 STAGESCORE_GetScoreData( STAGESCORE* wk, u8 id, u8 param, u8 param2, void* buf );


//============================================================================================
//
//	�Z�[�u�f�[�^�V�X�e�����ˑ�����֐�
//
//============================================================================================

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)�X�e�[�W�Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int STAGEDATA_GetWorkSize( void )
{
	return sizeof(STAGEDATA);
}

//----------------------------------------------------------
/**
 * @brief	(���f�f�[�^)�X�e�[�W���[�N�̏�����
 *
 * @param	wk		STAGEDATA�ւ̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void STAGEDATA_Init( STAGEDATA* wk )
{
	//MI_CpuClear32( wk, sizeof(STAGEDATA) );
	MI_CpuClear8( wk, sizeof(STAGEDATA) );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�X�e�[�W�Z�[�u�f�[�^�̎擾(�v���C�f�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	STAGEDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
STAGEDATA * SaveData_GetStageData( SAVEDATA * sv )
{
	//return (STAGEDATA*)SaveData_Get( sv, GMDATA_ID_STAGE );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->play_stage;
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
 * @param	wk		STAGEDATA�^�̃|�C���^
 *
 * @retval	TRUE	�������Z�[�u����Ă���
 * @retval	FALSE	�Z�[�u����Ă��Ȃ�
 */
//----------------------------------------------------------
BOOL STAGEDATA_GetSaveFlag( STAGEDATA* wk )
{
	return wk->save_flag;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�Z�[�u��ԃt���O���Z�b�g
 *
 * @param	wk		STAGEDATA�^�̃|�C���^
 * @param	flag	TRUE�AFALSE
 *
 * @retval	none
 */
//----------------------------------------------------------
void STAGEDATA_SetSaveFlag( STAGEDATA* wk, BOOL flag )
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
 * @param	wk		STAGEDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void STAGEDATA_PutPlayData( STAGEDATA* wk, u8 id, u8 param, u8 param2, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case STAGEDATA_ID_TYPE:
		wk->type = buf8[0];
		OS_Printf( "wk->type = %d\n", wk->type );
		break;

	//�����l�ځH
	case STAGEDATA_ID_ROUND:
		wk->round = buf8[0];
		OS_Printf( "wk->round = %d\n", wk->round );
		break;

	//�G�g���[�i�[�f�[�^�C���f�b�N�X
	case STAGEDATA_ID_TR_INDEX:
		wk->tr_index[param] = buf16[0];
		OS_Printf( "wk->tr_index[%d] = %d\n", param, wk->tr_index[param] );
		break;

	//�Q�����Ă���莝���|�P�����̈ʒu���
	case STAGEDATA_ID_MINE_POKE_POS:
		wk->mine_poke_pos[param] = buf8[0];
		OS_Printf( "wk->mine_poke_pos[%d] = %d\n", param, wk->mine_poke_pos[param] );
		break;

	//�G�|�P�����f�[�^�����Ȃ��悤�ɂ��邽�߂ɕۑ�
	case STAGEDATA_ID_ENEMY_POKE_INDEX:
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
 * @param	wk		STAGEDATA�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 STAGEDATA_GetPlayData( STAGEDATA* wk, u8 id, u8 param, u8 param2, void* buf )
{
	switch( id ){

	//�V���O���A�_�u���A�}���`�Awifi�}���`
	case STAGEDATA_ID_TYPE:
		return (u32)wk->type;

	//�����l�ځH
	case STAGEDATA_ID_ROUND:
		return (u32)wk->round;

	//�G�g���[�i�[�f�[�^�C���f�b�N�X
	case STAGEDATA_ID_TR_INDEX:
		return (u32)wk->tr_index[param];

	//�Q�����Ă���莝���|�P�����̈ʒu���
	case STAGEDATA_ID_MINE_POKE_POS:
		return wk->mine_poke_pos[param];

	//�G�|�P�����f�[�^�����Ȃ��悤�ɂ��邽�߂ɕۑ�
	case STAGEDATA_ID_ENEMY_POKE_INDEX:
		return wk->enemy_poke_index[param];

	default:
		GF_ASSERT(0);
		break;
	};

	return 0;
}


//============================================================================================
//
//	�^�C�v���Ƃ̃��x����4bit�Ŏ����ƂɂȂ����̂ŃA�N�Z�X�֐�
//
//	�^�C�v���x�������R�[�h�ɂȂ����̂ŁA
//	���̊֐����g���ăZ�[�u�f�[�^�ւ̓A�N�Z�X���Ă��Ȃ��B
//	STAGE_SCRWORK��type_level�̑���݂̂Ɏg�p���Ă���(�͂�)�B
//
//	�^�C�v���x���̃��R�[�h�ւ�fssc_stage_sub.c�ɂ���StageScr_TypeLevelRecordGet,Set���g�p�B
//
//============================================================================================

//--------------------------------------------------------------
/**
 * @brief   �^�C�v���Ƃ̃��x�����擾
 *
 * @param   csr_pos		�J�[�\���ʒu
 * @param   p_rank		�����N�f�[�^�̃|�C���^
 *
 * @retval  "���x��"
 */
//--------------------------------------------------------------
u8 Stage_GetTypeLevel( u8 csr_pos, u8* p_rank )
{
	u8 offset;
	u8 param;
	u8 ret;

	offset	= ( csr_pos / 2 );			//�t���O�̃I�t�Z�b�g[0,1][2,3],,,
	param	= ( csr_pos % 2 );			//�t���O�̂ǂ����4bit���擾

	ret = (p_rank[offset] >> (4 * param)) & 0x0f;

	OS_Printf( "p_rank[%d] = %d\n", offset, p_rank[offset] );
	OS_Printf( "ret = %d\n", ret );
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   �^�C�v���Ƃ̃��x�����Z�b�g
 *
 * @param   csr_pos		�J�[�\���ʒu
 * @param   p_rank		�����N�f�[�^�̃|�C���^
 * @param   num			�Z�b�g����l
 *
 * @retval  none
 */
//--------------------------------------------------------------
void Stage_SetTypeLevel( u8 csr_pos, u8* p_rank, u8 num )
{
	u8 offset;
	u8 param;
	u8 set_num;

	offset	= ( csr_pos / 2 );			//�t���O�̃I�t�Z�b�g[0,1][2,3],,,
	param	= ( csr_pos % 2 );			//�t���O�̂ǂ����4bit���擾

	//OS_Printf( "************************\n" );
	//OS_Printf( "before p_rank[%d] = %d\n", offset, p_rank[offset] );

	if( param == 0 ){
		p_rank[offset] = p_rank[offset] & 0xf0;
	}else{
		p_rank[offset] = p_rank[offset] & 0x0f;
	}

	set_num = (num << (4 * param));
	OS_Printf( "set_num = %d\n", set_num );

	p_rank[offset] |= set_num;
	OS_Printf( "after p_rank[%d] = %d\n", offset, p_rank[offset] );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�v���C�f�[�^�@�w�肵���o�g���^�C�v�̃��x�����N���A
 *
 * @param   p_rank		�����N�f�[�^�̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void Stage_ClearTypeLevel( u8* p_rank )
{
	int i;

	for( i=0; i < STAGE_TR_TYPE_MAX ;i++ ){
		Stage_SetTypeLevel( i, p_rank, 0 );		//0�I���W��
	}
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}


//============================================================================================
//
//	���ъ֘A
//
//============================================================================================

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)�X�e�[�W�Z�[�u���[�N�̃T�C�Y�擾
 *
 * @param	none
 *
 * @return	int		�T�C�Y(�o�C�g�P��)
 */
//----------------------------------------------------------
int STAGESCORE_GetWorkSize( void )
{
	return sizeof(STAGESCORE);
}

//----------------------------------------------------------
/**
 * @brief	(���уf�[�^)�X�e�[�W���[�N�̏�����
 *
 * @param	wk		STAGESCORE�^�̃|�C���^
 *
 * @return	none
 */
//----------------------------------------------------------
void STAGESCORE_Init( STAGESCORE* wk )
{
	//MI_CpuClear32( wk, sizeof(STAGESCORE) );
	MI_CpuClear8( wk, sizeof(STAGESCORE) );

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_SetCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return;
}

//----------------------------------------------------------
/**
 * @brief	�X�e�[�W�Z�[�u�f�[�^�̎擾(���уf�[�^)
 *
 * @param	sv		�Z�[�u�f�[�^�ւ̃|�C���^
 *
 * @return	STAGEDATA�ւ̃|�C���^
 */
//----------------------------------------------------------
STAGESCORE * SaveData_GetStageScore( SAVEDATA * sv )
{
	//return (STAGEDATA*)SaveData_Get( sv, GMDATA_ID_STAGE );
	FRONTIER_SAVEWORK* data = SaveData_Get( sv, GMDATA_ID_FRONTIER );
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FRONTIER)
	SVLD_CheckCrc(GMDATA_ID_FRONTIER);
#endif //CRC_LOADCHECK
	return &data->stage.score;
}

//----------------------------------------------------------
/**
 * @brief	���уf�[�^�@�Z�b�g
 *
 * @param	wk		STAGESCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^���i�[�����o�b�t�@�ւ̃|�C���^
 *
 * @retval	none
 */
//----------------------------------------------------------
void STAGESCORE_PutScoreData( STAGESCORE* wk, u8 id, u8 param, u8 param2, const void* buf )
{
	u32	*buf32	= (u32 *)buf;
	u16	*buf16	= (u16 *)buf;
	u8	*buf8	= (u8 *)buf;

	switch( id ){

	//10�A��(�N���A)�������t���O
	case STAGESCORE_ID_CLEAR_FLAG:
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
 * @brief	���уf�[�^�@�擾
 *
 * @param	wk		STAGESCORE�^�̃|�C���^
 * @param	id		�擾����f�[�^ID
 * @param	buf		void*:�f�[�^�擾�|�C���^
 *
 * @retval	none
 *
 * �߂�l���擾���鎞�Ɍ^�L���X�g���g�p���Ă���̂ŃR�s�y���ӁI
 */
//----------------------------------------------------------
u32 STAGESCORE_GetScoreData( STAGESCORE* wk, u8 id, u8 param, u8 param2, void* buf )
{
	//OS_Printf( "STAGEDATA = %d\n", sizeof(STAGEDATA) );
	//OS_Printf( "STAGESCORE = %d\n", sizeof(STAGESCORE) );

	switch( id ){

	//10�A��(�N���A)�������t���O
	case STAGESCORE_ID_CLEAR_FLAG:
		OS_Printf( "get clear_flag = %d\n", wk->clear_flag );
		return (u32)((wk->clear_flag >> param) & 0x01);

	default:
		GF_ASSERT(0);
		break;
	};

	return 0;
}


