//============================================================================================
/**
 * @file	castle_savedata.h
 * @brief	�L���b�X���Z�[�u�f�[�^�p�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.06.27
 */
//============================================================================================
#ifndef	__CASTLE_SAVEDATA_H__
#define	__CASTLE_SAVEDATA_H__


//============================================================================================
//
//	��`
//
//============================================================================================
enum{
	CASTLEDATA_ID_TYPE = 0,
	CASTLEDATA_ID_ROUND,
	//�莝���|�P����
	CASTLEDATA_ID_TEMOTI_HP,
	CASTLEDATA_ID_TEMOTI_PP,
	CASTLEDATA_ID_TEMOTI_CONDITION,
	CASTLEDATA_ID_TEMOTI_ITEM,
	//�g���[�i�[
	CASTLEDATA_ID_TR_INDEX,
	//�Q�����Ă���莝���|�P�����ʒu���
	CASTLEDATA_ID_MINE_POKE_POS,
	//�G�|�P�����f�[�^�����Ȃ��悤�ɕۑ�
	CASTLEDATA_ID_ENEMY_POKE_INDEX,

	//////////
	//����
	//////////
	CASTLESCORE_ID_CLEAR_FLAG,
	CASTLESCORE_ID_GUIDE_FLAG,
};


//---------------------------------------------------------------------------
/**
 * @brief	�X�e�[�W�Z�[�u�f�[�^�ւ̕s���S�^��`
 */
//---------------------------------------------------------------------------
typedef struct _CASTLEDATA CASTLEDATA;
typedef struct _CASTLESCORE CASTLESCORE;


//============================================================================================
//
//
//============================================================================================
extern CASTLEDATA * SaveData_GetCastleData( SAVEDATA* sv );

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern void CASTLEDATA_Init( CASTLEDATA* wk );
extern int CASTLEDATA_GetWorkSize( void );
extern CASTLEDATA* CASTLEDATA_AllocWork( int heapID );

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern void CASTLEDATA_ClearTypeLevel( CASTLEDATA* wk, u8 type );
extern BOOL CASTLEDATA_GetSaveFlag( CASTLEDATA* wk );
extern void CASTLEDATA_SetSaveFlag( CASTLEDATA* wk, BOOL flag );
extern void CASTLEDATA_PutPlayData( CASTLEDATA* wk, u8 id, u8 param, u8 param2, const void* buf );
extern u32 CASTLEDATA_GetPlayData( CASTLEDATA* wk, u8 id, u8 param, u8 param2, void* buf );
extern u8 Castle_GetTypeLevel( u8 csr_pos, u8* p_rank );
extern void Castle_SetTypeLevel( u8 csr_pos, u8* p_rank, u8 num );
extern void Castle_ClearTypeLevel( u8* p_rank );

//����
extern void CASTLESCORE_Init( CASTLESCORE* wk );
extern int CASTLESCORE_GetWorkSize( void );
extern CASTLESCORE * SaveData_GetCastleScore( SAVEDATA* sv );
extern void CASTLESCORE_PutScoreData( CASTLESCORE* wk, u8 id, u8 param, u8 param2, const void* buf );
extern u32 CASTLESCORE_GetScoreData( CASTLESCORE* wk, u8 id, u8 param, u8 param2, void* buf );


#endif	__CASTLE_SAVEDATA_H__


