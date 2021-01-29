//============================================================================================
/**
 * @file	roulette_savedata.h
 * @brief	���[���b�g�Z�[�u�f�[�^�p�w�b�_�[
 * @author	Satoshi Nohara
 * @date	2007.09.05
 */
//============================================================================================
#ifndef	__ROULETTE_SAVEDATA_H__
#define	__ROULETTE_SAVEDATA_H__


//============================================================================================
//
//	��`
//
//============================================================================================
enum{
	ROULETTEDATA_ID_TYPE = 0,
	ROULETTEDATA_ID_RANDOM_FLAG,
	ROULETTEDATA_ID_ROUND,
	ROULETTEDATA_ID_CSR_SPEED_LEVEL,
	//�莝���|�P����
	ROULETTEDATA_ID_TEMOTI_ITEM,
	//�g���[�i�[
	ROULETTEDATA_ID_TR_INDEX,
	//�Q�����Ă���莝���|�P�����ʒu���
	ROULETTEDATA_ID_MINE_POKE_POS,
	//�G�|�P�����f�[�^�����Ȃ��悤�ɕۑ�
	ROULETTEDATA_ID_ENEMY_POKE_INDEX,

	//////////
	//����
	//////////
	ROULETTESCORE_ID_CLEAR_FLAG,
};


//---------------------------------------------------------------------------
/**
 * @brief	�X�e�[�W�Z�[�u�f�[�^�ւ̕s���S�^��`
 */
//---------------------------------------------------------------------------
typedef struct _ROULETTEDATA ROULETTEDATA;
typedef struct _ROULETTESCORE ROULETTESCORE;


//============================================================================================
//
//
//============================================================================================
extern ROULETTEDATA * SaveData_GetRouletteData( SAVEDATA* sv );

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern void ROULETTEDATA_Init( ROULETTEDATA* wk );
extern int ROULETTEDATA_GetWorkSize( void );
extern ROULETTEDATA* ROULETTEDATA_AllocWork( int heapID );

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern void ROULETTEDATA_ClearTypeLevel( ROULETTEDATA* wk, u8 type );
extern BOOL ROULETTEDATA_GetSaveFlag( ROULETTEDATA* wk );
extern void ROULETTEDATA_SetSaveFlag( ROULETTEDATA* wk, BOOL flag );
extern void ROULETTEDATA_PutPlayData( ROULETTEDATA* wk, u8 id, u8 param, u8 param2, const void* buf );
extern u32 ROULETTEDATA_GetPlayData( ROULETTEDATA* wk, u8 id, u8 param, u8 param2, void* buf );

//����
extern void ROULETTESCORE_Init( ROULETTESCORE* wk );
extern int ROULETTESCORE_GetWorkSize( void );
extern ROULETTESCORE * SaveData_GetRouletteScore( SAVEDATA* sv );
extern void ROULETTESCORE_PutScoreData( ROULETTESCORE* wk, u8 id, u8 param, u8 param2, const void* buf );
extern u32 ROULETTESCORE_GetScoreData( ROULETTESCORE* wk, u8 id, u8 param, u8 param2, void* buf );


#endif	__ROULETTE_SAVEDATA_H__


