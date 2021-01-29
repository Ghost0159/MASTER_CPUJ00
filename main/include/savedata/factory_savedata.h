//============================================================================================
/**
 * @file	factory_savedata.h
 * @brief	�t�@�N�g���[�Z�[�u�f�[�^�p�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.03.28
 */
//============================================================================================
#ifndef	__FACTORY_SAVEDATA_H__
#define	__FACTORY_SAVEDATA_H__


//============================================================================================
//
//	��`
//
//============================================================================================
enum{
	FACTORYDATA_ID_LEVEL = 0,
	FACTORYDATA_ID_TYPE,
	FACTORYDATA_ID_ROUND,
	//�g���[�i�[
	FACTORYDATA_ID_TR_INDEX,
	//�莝��
	FACTORYDATA_ID_TEMOTI_POKE_INDEX,
	FACTORYDATA_ID_TEMOTI_POW_RND,
	FACTORYDATA_ID_TEMOTI_PERSONAL_RND,
	//�G
	FACTORYDATA_ID_ENEMY_POKE_INDEX,
	FACTORYDATA_ID_ENEMY_POW_RND,
	FACTORYDATA_ID_ENEMY_PERSONAL_RND,

	/////////
	//����
	/////////
	FACTORYSCORE_ID_CLEAR_FLAG,
};

//---------------------------------------------------------------------------
/**
 * @brief	�t�@�N�g���[�Z�[�u�f�[�^�ւ̕s���S�^��`
 */
//---------------------------------------------------------------------------
typedef struct _FACTORYDATA FACTORYDATA;
typedef struct _FACTORYSCORE FACTORYSCORE;


//============================================================================================
//
//
//============================================================================================
extern FACTORYDATA * SaveData_GetFactoryData( SAVEDATA* sv );

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern void FACTORYDATA_Init( FACTORYDATA* wk );
extern int FACTORYDATA_GetWorkSize( void );
extern FACTORYDATA* FACTORYDATA_AllocWork( int heapID );

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern BOOL FACTORYDATA_GetSaveFlag( FACTORYDATA* wk );
extern void FACTORYDATA_SetSaveFlag( FACTORYDATA* wk, BOOL flag );
extern void FACTORYDATA_PutPlayData( FACTORYDATA* wk, u8 id, u8 param, const void* buf );
extern u32 FACTORYDATA_GetPlayData( FACTORYDATA* wk, u8 id, u8 param, void* buf );

//����
extern void FACTORYSCORE_Init( FACTORYSCORE* wk );
extern int FACTORYSCORE_GetWorkSize( void );
extern void FACTORYSCORE_PutScoreData( FACTORYSCORE* wk, u8 id, u8 param, const void* buf );
extern u32 FACTORYSCORE_GetScoreData( FACTORYSCORE* wk, u8 id, u8 param, void* buf );
extern FACTORYSCORE * SaveData_GetFactoryScore( SAVEDATA* sv );


#endif	__FACTORY_SAVEDATA_H__


