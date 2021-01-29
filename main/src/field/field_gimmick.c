//============================================================================================
/**
 * @file	field_gimmick.c
 * @brief	�}�b�v�ŗL�̎d�|���𔭓����邩�ǂ����𔻒肷��\�[�X
 * @author	saito
 * @date	2006.02.09
 *
 */
//============================================================================================

#include "include/savedata/gimmickwork.h"
#include "field_gimmick_def.h"
#include "field_gimmick.h"
#include "gym.h"
#include "fieldsys.h"		//for	GameSystem_GetSaveData
#include "safari_train.h"
#include "fld_lift.h"
#include "field_tornworld.h"
#include "field_villa.h"

#include "mapdefine.h"

const static FLD_GMK_SETUP_FUNC FldGimmickSetupFunc[FLD_GIMMICK_MAX] = {
	NULL,					//0:����
	GYM_SetupWaterGym,		//1:���W��
//	GYM_SetupGhostGym,		//2:�S�[�X�g�W��
	GYM_SetupPLGhostGym,		//2:�S�[�X�g�W��
	GYM_SetupSteelGym,		//3:�|�W��
	GYM_SetupPLFightGym,	//4:�i���W��
	GYM_SetupElecGym,		//5:�d�C�W��
	SafariTrain_Setup,		//6:�T�t�@���d��
	FLIFT_Setup,			//7:�t�B�[���h���t�g
	GYM_SetupPLGrassGym,	//8:PL ���W��
	FldTornWorld_Setup,		//9:��Ԃꂽ������
	FldVilla_Setup,			//10:�ʑ�
};

const static FLD_GMK_END_FUNC FldGimmickEndFunc[FLD_GIMMICK_MAX] = {
	NULL,					//0:����
	NULL,					//1:���W��
//	NULL,					//2:�S�[�X�g�W��
	GYM_EndPLGhostGym,		//2:�S�[�X�g�W��
	GYM_EndSteelGym,		//3:�|�W��
	GYM_EndPLFightGym,		//4:�i���W��
	GYM_EndElecGym,			//5:�d�C�W��
	NULL,					//6:�T�t�@���d��
	NULL,					//7:�t�B�[���h���t�g
	GYM_EndPLGrassGym,		//8:PL ���W��
	FldTornWorld_End,		//9:��Ԃꂽ������
	FldVilla_End,			//10:�ʑ�
};

const static FLD_GMK_HIT_CHECK FldGimmickHitCheck[FLD_GIMMICK_MAX] = {
	NULL,					//0:����
	GYM_HitCheckWaterGym,	//1:���W��
	NULL,					//2:�S�[�X�g�W��
	GYM_HitCheckSteelGym,	//3:�|�W��
	GYM_HitCheckPLFightGym,	//4:�i���W��
	GYM_HitCheckElecGym,	//5:�d�C�W��
	NULL,					//6:�T�t�@���d��
	NULL,					//7:�t�B�[���h���t�g
	GYM_HitCheckPLGrassGym,	//8:PL ���W��
	FldTornWorld_HitCheck,	//9:��Ԃꂽ������
	FldVilla_HitCheck,		//10:�ʑ�
};

//---------------------------------------------------------------------------
/**
 * @brief	�M�~�b�N�̃Z�b�g�A�b�v�֐�
 * 
 * @param	fsys	�t�B�[���h�V�X�e���|�C���^
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
void FLDGMK_SetUpFieldGimmick(FIELDSYS_WORK *fsys)
{
	int id;
	GIMMICKWORK *work;
	
	//�M�~�b�N���[�N�擾
	work = SaveData_GetGimmickWork(GameSystem_GetSaveData(fsys));
	//�M�~�b�N�R�[�h���擾
	id = GIMMICKWORK_GetAssignID(work);
	
	if (id == FLD_GIMMICK_NONE){
		return;					//�M�~�b�N����
	}
	//�M�~�b�N�Z�b�g�A�b�v
	FldGimmickSetupFunc[id](fsys);
}

//---------------------------------------------------------------------------
/**
 * @brief	�M�~�b�N�̏I���֐�
 * 
 * @param	fsys	�t�B�[���h�V�X�e���|�C���^
 * 
 * @return	none
 */
//---------------------------------------------------------------------------
void FLDGMK_EndFieldGimmick(FIELDSYS_WORK *fsys)
{
	int id;
	GIMMICKWORK *work;
	//�M�~�b�N���[�N�擾
	work = SaveData_GetGimmickWork(GameSystem_GetSaveData(fsys));
	//�M�~�b�N�R�[�h���擾
	id = GIMMICKWORK_GetAssignID(work);
	
	if (id == FLD_GIMMICK_NONE){
		return;					//�M�~�b�N����
	}
	//�M�~�b�N�I��
	if (FldGimmickEndFunc[id] != NULL){
		FldGimmickEndFunc[id](fsys);
	}
}

//---------------------------------------------------------------------------
/**
 * @brief	�M�~�b�N�p���ꓖ���蔻�菈��
 * 
 * @param	fsys	�t�B�[���h�V�X�e���|�C���^
 * @param	inGridX		�O���b�h�w���W
 * @param	inGirdZ		�O���b�h�y���W
 * @param	inHeight	���ݍ���
 * @param	outHit		TRUE:�ʏ퓖���蔻����s�Ȃ�Ȃ�	FALSE�F�ʏ퓖���蔻����s��
 * 
 * @return	BOOL		TRUE:HIT		FALSE:NOHIT
 */
//---------------------------------------------------------------------------
BOOL FLDGMK_FieldGimmickHitCheck(	FIELDSYS_WORK *fsys,
									const int inGridX, const int inGridZ,
									const fx32 inHeight,
									BOOL *outHit)
{
	int id;
	GIMMICKWORK *work;
	//�M�~�b�N���[�N�擾
	work = SaveData_GetGimmickWork(GameSystem_GetSaveData(fsys));
	//�M�~�b�N�R�[�h���擾
	id = GIMMICKWORK_GetAssignID(work);
	
	if (id == FLD_GIMMICK_NONE){
		return FALSE;					//�M�~�b�N����
	}
	//�M�~�b�N�I��
	if (FldGimmickHitCheck[id] != NULL){
		BOOL rc;
		rc = FldGimmickHitCheck[id](fsys, inGridX, inGridZ, inHeight, outHit);
		return rc;
	}
	
	return FALSE;						//��������
}

//---------------------------------------------------------------------------
/**
 * @brief	�M�~�b�N�p����W�����v�A�g���r���[�g�`�F�b�N
 * 
 * @param	fsys	�t�B�[���h�V�X�e���|�C���^
 * @param	inGridX		�O���b�h�w���W
 * @param	inGirdZ		�O���b�h�y���W
 * @param	inHeight	���ݍ���
 * @param	dir			�i�s���� DIR_UP��
 * @return	BOOL		TRUE:HIT		FALSE:NOHIT
 */
//---------------------------------------------------------------------------
BOOL FLDGMK_FieldGimmickExAttrCheck(
		FIELDSYS_WORK *fsys,
		const int inGridX, const int inGridZ,
		const fx32 inHeight, int dir )
{
	//���W��
	if( fsys->location->zone_id == ZONE_ID_C04GYM0101 ){
		if( GYM_PLGrass_ExJumpPosCheck(fsys,inGridX,inGridZ,dir) ){
			return( TRUE );
		}
	}
	
	return( FALSE );
}
