//==============================================================================
/**
 * @file	gds_debug.c
 * @brief	GDS�ʐM�̃f�o�b�O�֘A
 * @author	matsuda
 * @date	2007.12.11(��)
 */
//==============================================================================
#include "common.h"
#include "gds_types.h"
#include "gds_battle_rec.h"
#include "gds_ranking.h"
#include "gds_boxshot.h"
#include "gds_dressup.h"

#include "savedata/imageclip_data.h"
#include "poketool/boxdata.h"

#include "savedata/gds_profile.h"

#include "gds_data_conv.h"
#include "gds_debug.h"



//==============================================================================
//	�f�[�^
//==============================================================================
static const GT_BOX_SHOT DebugDummyBoxData[] = {
	{
		0,
	},
};



//==============================================================================
//
//	�{�b�N�X�V���b�g
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   GT�{�b�N�X�V���b�g�̃_�~�[�f�[�^���擾����
 *
 * @param   box_no		�Q�Ƃ���{�b�N�X�ԍ�
 *
 * @retval  
 *
 */
//--------------------------------------------------------------
void GDS_DEBUG_BOX_GET_DummyBoxData(GT_BOX_SHOT *dest)
{
	MI_CpuCopy8(DebugDummyBoxData, dest, sizeof(DebugDummyBoxData));
}


//==============================================================================
//
//	�h���X�A�b�v
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   GT�h���X�A�b�v�V���b�g�̃_�~�[�f�[�^�Ƃ��Ď����̃e���r�f�[�^���擾����
 *
 * @param   sv			�Z�[�u�f�[�^�ւ̃|�C���^
 * 
 * @retval	�e���r�f�[�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
IMC_TELEVISION_SAVEDATA * GDS_DEBUG_DRESSUP_GET_DummyDressData(SAVEDATA *sv)
{
	IMC_SAVEDATA *imc;
	
	imc =  SaveData_GetImcSaveData(sv);
	return ImcSaveData_GetTelevisionSaveData(imc, IMC_SAVEDATA_TELEVISION_MYDATA);
}

