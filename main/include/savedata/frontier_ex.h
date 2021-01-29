//==============================================================================
/**
 * @file	frontier_ex.h
 * @brief	�o�g���t�����e�B�A�O���f�[�^(�i�s�Ɋ֌W�Ȃ��L�^�̂�)�̃w�b�_
 * @author	matsuda
 * @date	2007.07.18(��)
 */
//==============================================================================
#ifndef	__FRONTIER_EX_H__
#define	__FRONTIER_EX_H__

#include "common.h"
#include "savedata/savedata_def.h"


//==============================================================================
//	�萔��`
//==============================================================================

//�o�g���X�e�[�W�̃f�[�^�擾ID
enum{
	FREXID_STAGE_RENSHOU_SINGLE,		///<�X�e�[�W�F�V���O���ő�A����
	FREXID_STAGE_RENSHOU_DOUBLE,		///<�X�e�[�W�F�_�u���ő�A����
	FREXID_STAGE_RENSHOU_MULTI,			///<�X�e�[�W�F���C�����X�ő�A����
//	FREXID_STAGE_RENSHOU_WIFI,			///<�X�e�[�W�FWIFI�ő�A����
};

//--------------------------------------------------------------
/**
 *	�퓬�^��Z�[�u�f�[�^�̕s���S�^�\���̐錾
 */
//--------------------------------------------------------------
typedef struct _FRONTIER_EX_SAVEDATA		FRONTIER_EX_SAVEDATA;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern int FrontierEx_GetWorkSize( void );
extern void FrontierEx_Init(FRONTIER_EX_SAVEDATA *work);
extern FRONTIER_EX_SAVEDATA * FrontierEx_Load(SAVEDATA *sv,int heapID,LOAD_RESULT *result);
extern SAVE_RESULT FrontierEx_Save(SAVEDATA *sv, FRONTIER_EX_SAVEDATA *fes);
extern u16 FrontierEx_StageRenshou_Get(SAVEDATA *sv, FRONTIER_EX_SAVEDATA *fes, int id, int monsno);

extern BOOL FrontierEx_StageRenshou_RenshouCopyExtra(SAVEDATA *sv, 
	int record_id, int record_monsno_id, 
	int friend_no, int save_id, int heap_id, LOAD_RESULT *load_result, SAVE_RESULT *save_result);


extern FRONTIER_EX_SAVEDATA * SaveData_Extra_LoadFrontierEx(SAVEDATA * sv, int heap_id, LOAD_RESULT * result);
extern SAVE_RESULT SaveData_Extra_SaveFrontierEx(SAVEDATA * sv, FRONTIER_EX_SAVEDATA * data);

//�f�o�b�N�p
extern u16 Debug_FrontierEx_StageRenshou_Set(FRONTIER_EX_SAVEDATA *fes, int id, int monsno, u16 data);


#endif	/* __FRONTIER_EX_H__ */
