//==============================================================================
/**
 * @file	battle_rec.h
 * @brief	�퓬�^��f�[�^�p�w�b�_
 * @author	matsuda
 * @date	2007.03.06(��)
 */
//==============================================================================
#ifndef	__BATTLE_REC_H__
#define	__BATTLE_REC_H__


#ifndef	__ASM_NO_DEF_	// ������ȍ~�́A�A�Z���u���ł͖��� +++++++++++++++

#include "common.h"
#include "savedata/savedata_def.h"
#include "poketool/poke_tool.h"

#include "battle\battle_common.h"
#include "savedata/gds_profile.h"	//GDS_PROFILE_PTR��`

#include "application/battle_recorder/gds_battle_rec.h"

#endif	//__ASM_NO_DEF_		+++++++++++++++++++++++++++++++++++++++++++++++



#define	REC_BUFFER_SIZE	(0x0400)	///<�ΐ�^��o�b�t�@�T�C�Y
#define	REC_BUFFER_END	(0xff)		///<�ΐ�^��o�b�t�@�̏I�[��`

//���[�h�f�[�^�p�C���f�b�N�X
#define LOADDATA_MYREC		(0)
#define LOADDATA_DOWNLOAD0	(1)
#define LOADDATA_DOWNLOAD1	(2)
#define LOADDATA_DOWNLOAD2	(3)

///�^��f�[�^���[�h���̌��ʒl
#define RECLOAD_RESULT_NULL		(0)	///<�f�[�^�Ȃ�
#define RECLOAD_RESULT_OK		(1)	///<����ǂݍ���
#define RECLOAD_RESULT_NG		(2)	///<�f�[�^�s��
#define RECLOAD_RESULT_ERROR	(3)	///<���[�h�œǂݏo�����s(�Z�[�u�f�[�^���j�󂳂�Ă���ꍇ�Ȃ�)

#define REC_COUNTER_MAX			( 9999 )	///< �A���L�^�ő�l


//--------------------------------------------------------------
//	�퓬���[�h(�{�݈ꗗ)
//--------------------------------------------------------------
///�o�g���^���[ / �V���O��
#define RECMODE_TOWER_SINGLE		(GT_BATTLE_MODE_TOWER_SINGLE)
///�o�g���^���[ / �_�u��
#define RECMODE_TOWER_DOUBLE		(GT_BATTLE_MODE_TOWER_DOUBLE)
///�o�g���^���[ / WIFI�_�E�����[�h
#define RECMODE_TOWER_WIFI_DL		(GT_BATTLE_MODE_TOWER_SINGLE)

///�o�g���t�@�N�g���[ / �V���O��
#define RECMODE_FACTORY_SINGLE		(GT_BATTLE_MODE_FACTORY50_SINGLE)
///�o�g���t�@�N�g���[ / �_�u��
#define RECMODE_FACTORY_DOUBLE		(GT_BATTLE_MODE_FACTORY50_DOUBLE)

///�o�g���t�@�N�g���[ / �V���O��100
#define RECMODE_FACTORY_SINGLE100	(GT_BATTLE_MODE_FACTORY100_SINGLE)
///�o�g���t�@�N�g���[ / �_�u��100
#define RECMODE_FACTORY_DOUBLE100	(GT_BATTLE_MODE_FACTORY100_DOUBLE)

///�o�g���X�e�[�W / �V���O��
#define RECMODE_STAGE_SINGLE		(GT_BATTLE_MODE_STAGE_SINGLE)
///�o�g���X�e�[�W / �_�u��
#define RECMODE_STAGE_DOUBLE		(GT_BATTLE_MODE_STAGE_DOUBLE)

///�o�g���L���b�X�� / �V���O��
#define RECMODE_CASTLE_SINGLE		(GT_BATTLE_MODE_CASTLE_SINGLE)
///�o�g���L���b�X�� / �_�u��
#define RECMODE_CASTLE_DOUBLE		(GT_BATTLE_MODE_CASTLE_DOUBLE)

///�o�g�����[���b�g / �V���O��
#define RECMODE_ROULETTE_SINGLE		(GT_BATTLE_MODE_ROULETTE_SINGLE)
///�o�g�����[���b�g / �_�u��
#define RECMODE_ROULETTE_DOUBLE		(GT_BATTLE_MODE_ROULETTE_DOUBLE)

///�R���V�A��(�ʐM�ΐ�) / �V���O��(��������)
#define RECMODE_COLOSSEUM_SINGLE			(GT_BATTLE_MODE_COLOSSEUM_SINGLE)
///�R���V�A��(�ʐM�ΐ�) / �V���O��(�X�^���_�[�h�J�b�v)
#define RECMODE_COLOSSEUM_SINGLE_STANDARD	(GT_BATTLE_MODE_COLOSSEUM_SINGLE_STANDARD)
///�R���V�A��(�ʐM�ΐ�) / �V���O��(�t�@���V�[�J�b�v)
#define RECMODE_COLOSSEUM_SINGLE_FANCY		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_FANCY)
///�R���V�A��(�ʐM�ΐ�) / �V���O��(���g���J�b�v)
#define RECMODE_COLOSSEUM_SINGLE_LITTLE		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_LITTLE)
///�R���V�A��(�ʐM�ΐ�) / �V���O��(���C�g�J�b�v)
#define RECMODE_COLOSSEUM_SINGLE_LIGHT		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_LIGHT)
///�R���V�A��(�ʐM�ΐ�) / �V���O��(�_�u���J�b�v)
#define RECMODE_COLOSSEUM_SINGLE_DOUBLE		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_DOUBLE)
///�R���V�A��(�ʐM�ΐ�) / �V���O��(���̑��̃J�b�v)
#define RECMODE_COLOSSEUM_SINGLE_ETC		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_ETC)

///�R���V�A��(�ʐM�ΐ�) / �_�u��
#define RECMODE_COLOSSEUM_DOUBLE			(GT_BATTLE_MODE_COLOSSEUM_DOUBLE)
///�R���V�A��(�ʐM�ΐ�) / �_�u��(�X�^���_�[�h�J�b�v)
#define RECMODE_COLOSSEUM_DOUBLE_STANDARD	(GT_BATTLE_MODE_COLOSSEUM_DOUBLE_STANDARD)
///�R���V�A��(�ʐM�ΐ�) / �_�u��(�t�@���V�[�J�b�v)
#define RECMODE_COLOSSEUM_DOUBLE_FANCY		(GT_BATTLE_MODE_COLOSSEUM_DOUBLE_FANCY)
///�R���V�A��(�ʐM�ΐ�) / �_�u��(���g���J�b�v)
#define RECMODE_COLOSSEUM_DOUBLE_LITTLE		(GT_BATTLE_MODE_COLOSSEUM_DOUBLE_LITTLE)
///�R���V�A��(�ʐM�ΐ�) / �_�u��(���C�g�J�b�v)
#define RECMODE_COLOSSEUM_DOUBLE_LIGHT		(GT_BATTLE_MODE_COLOSSEUM_DOUBLE_LIGHT)
///�R���V�A��(�ʐM�ΐ�) / �_�u��(�_�u���J�b�v)
#define RECMODE_COLOSSEUM_DOUBLE_DOUBLE		(GT_BATTLE_MODE_COLOSSEUM_DOUBLE_DOUBLE)
///�R���V�A��(�ʐM�ΐ�) / �_�u��(���̑��̃J�b�v)
#define RECMODE_COLOSSEUM_DOUBLE_ETC		(GT_BATTLE_MODE_COLOSSEUM_DOUBLE_ETC)

///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(��������)
#define RECMODE_COLOSSEUM_MIX				(GT_BATTLE_MODE_COLOSSEUM_SINGLE)
///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(�X�^���_�[�h�J�b�v)
#define RECMODE_COLOSSEUM_MIX_STANDARD		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_STANDARD)
///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(�t�@���V�[�J�b�v)
#define RECMODE_COLOSSEUM_MIX_FANCY			(GT_BATTLE_MODE_COLOSSEUM_SINGLE_FANCY)
///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(���g���J�b�v)
#define RECMODE_COLOSSEUM_MIX_LITTLE		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_LITTLE)
///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(���C�g�J�b�v)
#define RECMODE_COLOSSEUM_MIX_LIGHT			(GT_BATTLE_MODE_COLOSSEUM_SINGLE_LIGHT)
///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(�_�u���J�b�v)
#define RECMODE_COLOSSEUM_MIX_DOUBLE		(GT_BATTLE_MODE_COLOSSEUM_SINGLE_DOUBLE)
///�R���V�A��(�ʐM�ΐ�) / �~�b�N�X(���̑��̃J�b�v)
#define RECMODE_COLOSSEUM_MIX_ETC			(GT_BATTLE_MODE_COLOSSEUM_SINGLE_ETC)

//�ȉ��A�}���`���[�h
#define RECMODE_TOWER_MULTI 		(GT_BATTLE_MODE_TOWER_MULTI)	///<�o�g���^���[ / �}���`
#define RECMODE_FACTORY_MULTI		(GT_BATTLE_MODE_FACTORY50_MULTI)	///<�o�g���t�@�N�g���[ / �}���`
#define RECMODE_FACTORY_MULTI100	(GT_BATTLE_MODE_FACTORY100_MULTI)///<�o�g���t�@�N�g���[ / �}���`100
#define RECMODE_STAGE_MULTI			(GT_BATTLE_MODE_STAGE_MULTI)	///<�o�g���X�e�[�W / �}���`
#define RECMODE_CASTLE_MULTI		(GT_BATTLE_MODE_CASTLE_MULTI)	///<�o�g���L���b�X�� / �}���`
#define RECMODE_ROULETTE_MULTI		(GT_BATTLE_MODE_ROULETTE_MULTI)	///<�o�g�����[���b�g / �}���`
#define RECMODE_COLOSSEUM_MULTI		(GT_BATTLE_MODE_COLOSSEUM_MULTI)///<�R���V�A��(�ʐM�ΐ�) / �}���`

//�I�[
#define RECMODE_MAX						(GT_BATTLE_MODE_ROULETTE_MULTI + 1)


//--------------------------------------------------------------
//	�^��w�b�_�̃p�����[�^�擾�vINDEX
//--------------------------------------------------------------
#define RECHEAD_IDX_MONSNO			(0)		///<�|�P�����ԍ�
#define RECHEAD_IDX_FORM_NO			(1)		///<�t�H�����ԍ�
#define RECHEAD_IDX_COUNTER			(2)		///<����ڂ�
#define RECHEAD_IDX_MODE			(3)		///<�^��{��
#define RECHEAD_IDX_DATA_NUMBER		(4)		///<�f�[�^�i���o�[
#define RECHEAD_IDX_SECURE			(5)		///<���S�����ς�



#ifndef	__ASM_NO_DEF_	// ������ȍ~�́A�A�Z���u���ł͖��� +++++++++++++++

//--------------------------------------------------------------
/**
 *	�퓬�^��Z�[�u�f�[�^�̕s���S�^�\���̐錾
 */
//--------------------------------------------------------------
typedef struct _BATTLE_REC_SAVEDATA		BATTLE_REC_SAVEDATA;

//--------------------------------------------------------------
/**
 *	�퓬�^��{�̂̕s���S�^�\���̐錾
 */
//--------------------------------------------------------------
typedef struct _BATTLE_REC_WORK * 	BATTLE_REC_WORK_PTR;

//--------------------------------------------------------------
/**
 *	�퓬�^��w�b�_�̕s���S�^�\���̐錾
 */
//--------------------------------------------------------------
typedef struct _BATTLE_REC_HEADER * 	BATTLE_REC_HEADER_PTR;

//--------------------------------------------------------------
/**
 *	�퓬�^��̃~���[�����O�Q�Ɛ搧��\����
 */
//--------------------------------------------------------------
typedef struct{
	u32 rec_key;				///<�F�؃L�[
	u8 rec_flag:2;				///<�Q�Ɛ�t���O
	u8 			:6;		//�]��
	
	u8 padding[3];		//�p�f�B���O
}BATTLE_REC_SAVE_KEY;

//==============================================================================
//	
//==============================================================================
extern int BattleRec_GetWorkSize( void );
extern void BattleRec_WorkInit(BATTLE_REC_SAVEDATA *rec);

//------------------------------------------------------------------
/**
 * �f�[�^������
 *
 * @param   heapID	�������m�ۂ��邽�߂̃q�[�vID		
 *
 */
//------------------------------------------------------------------
extern	void BattleRec_Init(SAVEDATA *sv,int heapID,LOAD_RESULT *result);

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̔j��
 */
//------------------------------------------------------------------
extern	void BattleRec_Exit(void);

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̃��������m�ۂ��Ă��邩�`�F�b�N
 *
 * @retval	TRUE:���݂���@FALSE:���݂��Ȃ�
 */
//------------------------------------------------------------------
extern	BOOL BattleRec_DataExistCheck(void);

//--------------------------------------------------------------
/**
 * @brief   ���[�N�̃A�h���X���擾
 *
 * @retval	brs�Ɋi�[����Ă��郏�[�N�A�h���X		
 */
//--------------------------------------------------------------
extern	BATTLE_REC_SAVEDATA *BattleRec_WorkAdrsGet( void );
extern void * BattleRec_RecWorkAdrsGet( void );

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̃��[�h
 *
 * @param	sv		�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 * @param	heapID	�f�[�^�����[�h���郁�������m�ۂ��邽�߂̃q�[�vID
 * @param	result	���[�h���ʂ��i�[���郏�[�N
 * @param	bp		���[�h�����f�[�^���琶������BATTLE_PARAM�\���̂ւ̃|�C���^
 * @param	num		���[�h����f�[�^�i���o�[�iLOADDATA_MYREC�ALOADDATA_DOWNLOAD0�ALOADDATA_DOWNLOAD1�c�j
 *
 * @retval	TRUE:����ǂݍ��݁B�@FALSE:�f�[�^�s��
 */
//------------------------------------------------------------------
BOOL BattleRec_Load(SAVEDATA *sv,int heapID,LOAD_RESULT *result,BATTLE_PARAM *bp,int num);

//------------------------------------------------------------------
/**
 * �ΐ�^��f�[�^�̃Z�[�u
 *
 * @param	sv		�Z�[�u�f�[�^�\���̂ւ̃|�C���^
 *
 * @retval	�Z�[�u����
 */
//------------------------------------------------------------------
extern SAVE_RESULT BattleRec_Save(SAVEDATA *sv, int rec_mode, int fight_count, int num, u16 *work0, u16 *work1);

//--------------------------------------------------------------
/**
 * @brief   �w��ʒu�̘^��f�[�^���폜(������)���ăZ�[�u���s
 *
 * @param   sv			
 * @param   heap_id		
 * @param   num			
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
extern SAVE_RESULT BattleRec_SaveDataErase(SAVEDATA *sv, int heap_id, int num);

//============================================================================================
/**
 *	�퓬�^�揈��
 *
 * @param[in]	client_no	�^����s��ClientNo
 * @param[in]	pos			�^��f�[�^�̊i�[�ꏊ
 * @param[in]	data		�^��f�[�^
 */
//============================================================================================
extern	void BattleRec_DataRec(int client_no,int pos,REC_DATA data);

//============================================================================================
/**
 *	�퓬�Đ��f�[�^�擾����
 *
 * @param[in]	rp			�퓬�V�X�e�����[�N�̍\���̃|�C���^
 * @param[in]	client_no	�^����s��ClientNo
 * @param[in]	pos			�^��f�[�^�̊i�[�ꏊ
 */
//============================================================================================
extern	REC_DATA BattleRec_DataPlay(int client_no,int pos);

//============================================================================================
/**
 *	BATTLE_PARAM�\���̕ۑ�����
 *
 * @param[in]	rec			�^��Đ��f�[�^�ۑ����[�N�̍\���̃|�C���^
 * @param[in]	bp			BATTLE_PARAM�\���̂ւ̃|�C���^
 */
//============================================================================================
extern	void BattleRec_BattleParamRec(BATTLE_PARAM *bp);

//============================================================================================
/**
 *	BATTLE_PARAM�\���̐�������
 *
 * @param[in]	rec			�^��Đ��f�[�^�ۑ����[�N�̍\���̃|�C���^
 * @param[in]	bp			BATTLE_PARAM�\���̂ւ̃|�C���^
 */
//============================================================================================
extern	void BattleRec_BattleParamCreate(BATTLE_PARAM *bp,SAVEDATA *sv);
extern void BattleRec_ServerVersionUpdate(int id_no, u32 server_version);
extern BOOL BattleRec_ServerVersionCheck(void);

//extern DENDOU_SAVEDATA* SaveData_GetDendouData( SAVEDATA* sv );
//�^��f�[�^�͒ʏ�Z�[�u�f�[�^�łȂ����߁A�A�N�Z�X���ɂ͉��L�֐����g�p���ăt���b�V������ǂݏ�������
extern BATTLE_REC_SAVEDATA * SaveData_Extra_LoadBattleRecData(SAVEDATA * sv, int heap_id, 
	LOAD_RESULT * result, int num);
extern SAVE_RESULT SaveData_Extra_SaveBattleRecData(
	SAVEDATA * sv, BATTLE_REC_SAVEDATA * data, int num);

extern BATTLE_REC_HEADER_PTR BattleRec_Header_AllocMemory(int heap_id);
extern void BattleRec_Header_FreeMemory(BATTLE_REC_HEADER_PTR brhp);

extern SAVE_RESULT BattleRec_GDS_RecvData_Save(SAVEDATA *sv, int num, u8 secure, 
	u16 *work0, u16 *work1);
extern void BattleRec_GDS_SendData_Conv(SAVEDATA *sv);
extern SAVE_RESULT BattleRec_GDS_MySendData_DataNumberSetSave(SAVEDATA *sv, u64 data_number, u16 *work0, u16 *work1);

extern BOOL BattleRec_DataOccCheck(SAVEDATA *sv,int heapID,LOAD_RESULT *result,int num);
extern SAVE_RESULT BattleRec_SecureSetSave(SAVEDATA *sv, int num);
extern void BattleRec_ClientTemotiGet(int rec_mode, int *client_max, int *temoti_max);

//==============================================================================
//	�f�[�^�擾
//==============================================================================
extern BATTLE_REC_HEADER_PTR BattleRec_HeaderAllocCopy(int heap_id);
extern GDS_PROFILE_PTR BattleRec_GDSProfileAllocCopy(int heap_id);
extern GDS_PROFILE_PTR BattleRec_GDSProfilePtrGet(void);
extern BATTLE_REC_WORK_PTR BattleRec_WorkPtrGet(void);
extern BATTLE_REC_HEADER_PTR BattleRec_HeaderPtrGet(void);
extern u64 RecHeader_ParamGet(BATTLE_REC_HEADER_PTR head, int index, int param);
extern void BattleRec_DataSet(GDS_PROFILE_PTR gpp, BATTLE_REC_HEADER_PTR head, 
	BATTLE_REC_WORK_PTR rec, BATTLE_PARAM *bp, SAVEDATA *sv);

//==============================================================================
//	�ꕔ�֐���battle_rec_ov.c�Ɉړ������ׁA�O�����J����K�v�ɂȂ�������
//==============================================================================
extern SAVE_RESULT Local_BattleRecSave(SAVEDATA *sv, BATTLE_REC_SAVEDATA *work, int num, u16 *seq);
extern void	BattleRec_Coded(void *data,u32 size,u32 code);

//==============================================================================
//	battl_rec_ov.c
//			��battle_recorder�I�[�o�[���C�̈�ɔz�u����Ă���̂Œ���!
//==============================================================================
extern void BattleRecTool_ErrorStrCheck(BATTLE_REC_SAVEDATA *src, BATTLE_PARAM *bp, int heap_id);
#ifdef PM_DEBUG
extern void DEBUG_BattleRec_SecureFlagSet(SAVEDATA *sv);
#endif

#endif	//__ASM_NO_DEF_

#endif	/* __BATTLE_REC_H__ */
