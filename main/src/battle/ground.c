//==============================================================================
/**
 * @file	ground.c
 * @brief	�퓬��ʁF�n��(�|�P��������ɏ�邨�~)
 * @author	matsuda
 * @date	2005.10.06(��)
 */
//==============================================================================
#include "common.h"
#include "graphic\pl_batt_obj_def.h"
#include "battle/battle_common.h"
#include "battle/fight_tool.h"
#include "ground.h"
#include "system/arc_tool.h"
#include "battle/battle_id.h"
#include "battle/attr_def.h"



//==============================================================================
//	�萔��`
//==============================================================================
///�n�ʂ�BG�v���C�I���e�B
#define GROUND_BGPRI		(3)
///���@���n�ʂ̃\�t�g�v���C�I���e�B
#define GROUND_SOFTPRI_MINE		(1000)
///�G���n�ʂ̃\�t�g�v���C�I���e�B
#define GROUND_SOFTPRI_ENEMY	(1000)


//==============================================================================
//	�f�[�^
//==============================================================================
///�n�ʃA�N�^�[�w�b�_
static const TCATS_OBJECT_ADD_PARAM_S GroundObjParam[] = {
	{//GROUND_TYPE_NORMAL_MINE
		ENCOUNT_X_TYPE_AA, GROUND_MINE_Y, 0,		//x, y, z
		0, GROUND_SOFTPRI_MINE, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
			CHARID_GROUND_MINE,	//�L����
			PLTTID_GROUND,	//�p���b�g
			CELLID_GROUND_MINE,	//�Z��
			CELLANMID_GROUND_MINE,	//�Z���A�j��
			CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
		},
		GROUND_BGPRI,			//BG�v���C�I���e�B
		0,			//Vram�]���t���O
	},
	{//GROUND_TYPE_NORMAL_ENEMY
		ENCOUNT_X_TYPE_BB, GROUND_ENEMY_Y, 0,		//x, y, z
		0, GROUND_SOFTPRI_ENEMY, 0,		//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
			CHARID_GROUND_ENEMY,	//�L����
			PLTTID_GROUND,	//�p���b�g
			CELLID_GROUND_ENEMY,	//�Z��
			CELLANMID_GROUND_ENEMY,	//�Z���A�j��
			CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
		},
		GROUND_BGPRI,			//BG�v���C�I���e�B
		0,			//Vram�]���t���O
	},
};

///�n�ʃL����ID���\�[�X�e�[�u���F������
ALIGN4 static const u16 GroundResourceID_Mine[] = {
	GROUND02_M_NCGR_BIN,
	GROUND07_M_NCGR_BIN,
	GROUND00_M_NCGR_BIN,
	GROUND10_M_NCGR_BIN,
	GROUND04_M_NCGR_BIN,
	GROUND09_M_NCGR_BIN,
	GROUND05_M_NCGR_BIN,
	GROUND01_M_NCGR_BIN,
	GROUND03_M_NCGR_BIN,
	GROUND06_M_NCGR_BIN,
	GROUND08_M_NCGR_BIN,
	GROUND10_M_NCGR_BIN,	//�݂苴�@�_�~�[
	GROUND12_M_NCGR_BIN,	//�l�V��A
	GROUND13_M_NCGR_BIN,	//�l�V��B
	GROUND14_M_NCGR_BIN,	//�l�V��C
	GROUND15_M_NCGR_BIN,	//�l�V��D
	GROUND16_M_NCGR_BIN,	//�`�����s�I��
	GROUND17_M_NCGR_BIN,	//�j�ꂽ���E(����n�ʗL��)
	GROUND18_M_NCGR_BIN,	//�o�g���^���[
	GROUND19_M_NCGR_BIN,	//�o�g���t�@�N�g���[
	GROUND20_M_NCGR_BIN,	//�o�g�����[���b�g
	GROUND21_M_NCGR_BIN,	//�o�g���L���b�X��
	GROUND22_M_NCGR_BIN,	//�o�g���X�e�[�W
	GROUND23_M_NCGR_BIN,	//�j�ꂽ���E(�M���e�B�i��p�B����n�ʖ���)
};

///�n�ʃL����ID���\�[�X�e�[�u���F�G��
ALIGN4 static const u16 GroundResourceID_Enemy[] = {
	GROUND02_E_NCGR_BIN,
	GROUND07_E_NCGR_BIN,
	GROUND00_E_NCGR_BIN,
	GROUND10_E_NCGR_BIN,
	GROUND04_E_NCGR_BIN,
	GROUND09_E_NCGR_BIN,
	GROUND05_E_NCGR_BIN,
	GROUND01_E_NCGR_BIN,
	GROUND03_E_NCGR_BIN,
	GROUND06_E_NCGR_BIN,
	GROUND08_E_NCGR_BIN,
	GROUND08_E_NCGR_BIN,	//�݂苴�@�_�~�[
	GROUND12_E_NCGR_BIN,	//�l�V��A
	GROUND13_E_NCGR_BIN,	//�l�V��B
	GROUND14_E_NCGR_BIN,	//�l�V��C
	GROUND15_E_NCGR_BIN,	//�l�V��D
	GROUND16_E_NCGR_BIN,	//�`�����s�I��
	GROUND17_E_NCGR_BIN,	//�j�ꂽ���E(����n�ʗL��)
	GROUND18_E_NCGR_BIN,	//�o�g���^���[
	GROUND19_E_NCGR_BIN,	//�o�g���t�@�N�g���[
	GROUND20_E_NCGR_BIN,	//�o�g�����[���b�g
	GROUND21_E_NCGR_BIN,	//�o�g���L���b�X��
	GROUND22_E_NCGR_BIN,	//�o�g���X�e�[�W
	GROUND23_E_NCGR_BIN,	//�j�ꂽ���E(�M���e�B�i��p�B����n�ʖ���)
};

///�n�ʃp���b�gID���\�[�X�e�[�u��
ALIGN4 static const u16 GroundResourceID_Palette[][3] = {
	BATT_GROUND02_D_NCLR,	BATT_GROUND02_E_NCLR,	BATT_GROUND02_N_NCLR,
	BATT_GROUND07_D_NCLR,	BATT_GROUND07_E_NCLR,	BATT_GROUND07_N_NCLR,
	BATT_GROUND00_D_NCLR,	BATT_GROUND00_E_NCLR,	BATT_GROUND00_N_NCLR,
	BATT_GROUND10_D_NCLR,	BATT_GROUND10_E_NCLR,	BATT_GROUND10_N_NCLR,
	BATT_GROUND04_D_NCLR,	BATT_GROUND04_E_NCLR,	BATT_GROUND04_N_NCLR,
	BATT_GROUND09_D_NCLR,	BATT_GROUND09_E_NCLR,	BATT_GROUND09_N_NCLR,
	BATT_GROUND05_D_NCLR,	BATT_GROUND05_E_NCLR,	BATT_GROUND05_N_NCLR,
	BATT_GROUND01_D_NCLR,	BATT_GROUND01_E_NCLR,	BATT_GROUND01_N_NCLR,
	BATT_GROUND03_D_NCLR,	BATT_GROUND03_E_NCLR,	BATT_GROUND03_N_NCLR,
	BATT_GROUND06_D_NCLR,	BATT_GROUND06_E_NCLR,	BATT_GROUND06_N_NCLR,
	BATT_GROUND08_D_NCLR,	BATT_GROUND08_E_NCLR,	BATT_GROUND08_N_NCLR,
	BATT_GROUND08_D_NCLR,	BATT_GROUND08_E_NCLR,	BATT_GROUND08_N_NCLR,	//�݂苴�@�_�~�[
	BATT_GROUND12_D_NCLR,	BATT_GROUND12_E_NCLR,	BATT_GROUND12_N_NCLR,	//�l�V��A
	BATT_GROUND13_D_NCLR,	BATT_GROUND13_E_NCLR,	BATT_GROUND13_N_NCLR,	//�l�V��B
	BATT_GROUND14_D_NCLR,	BATT_GROUND14_E_NCLR,	BATT_GROUND14_N_NCLR,	//�l�V��C
	BATT_GROUND15_D_NCLR,	BATT_GROUND15_E_NCLR,	BATT_GROUND15_N_NCLR,	//�l�V��D
	BATT_GROUND16_D_NCLR,	BATT_GROUND16_E_NCLR,	BATT_GROUND16_N_NCLR,	//�`�����s�I��
	BATT_GROUND17_D_NCLR,	BATT_GROUND17_E_NCLR,	BATT_GROUND17_N_NCLR,	//�j�ꂽ���E
	BATT_GROUND18_D_NCLR,	BATT_GROUND18_E_NCLR,	BATT_GROUND18_N_NCLR,	//�o�g���^���[
	BATT_GROUND19_D_NCLR,	BATT_GROUND19_E_NCLR,	BATT_GROUND19_N_NCLR,	//�o�g���t�@�N�g���[
	BATT_GROUND20_D_NCLR,	BATT_GROUND20_E_NCLR,	BATT_GROUND20_N_NCLR,	//�o�g�����[���b�g
	BATT_GROUND21_D_NCLR,	BATT_GROUND21_E_NCLR,	BATT_GROUND21_N_NCLR,	//�o�g���L���b�X��
	BATT_GROUND22_D_NCLR,	BATT_GROUND22_E_NCLR,	BATT_GROUND22_N_NCLR,	//�o�g���X�e�[�W
	BATT_GROUND23_D_NCLR,	BATT_GROUND23_E_NCLR,	BATT_GROUND23_N_NCLR,	//�j�ꂽ���E(����n�ʖ���)
};


//--------------------------------------------------------------
/**
 * @brief   �n�ʂ̃��\�[�X�ǂݍ���
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Ground_ResourceLoad(GROUND_WORK *ground)
{
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	const TCATS_OBJECT_ADD_PARAM_S *obj_param;
	int char_resource_id, char_id, cell_resource_id, cell_id, anm_resource_id, anm_id;
	int time_zone_offset;
	ARCHANDLE* hdl;
	
	hdl = ArchiveDataHandleOpen( ARC_BATT_OBJ,  HEAPID_BATTLE ); 
	
//	GF_ASSERT(ground->type < NELEMS(GroundObjParam));
	
	csp = BattleWorkCATS_SYS_PTRGet(ground->bw);
	crp = BattleWorkCATS_RES_PTRGet(ground->bw);
	time_zone_offset = BattleWorkTimeZoneOffsetGet(ground->bw);
	
	obj_param = &GroundObjParam[ground->mine_enemy];
	if(ground->mine_enemy == GROUND_TYPE_NORMAL_MINE){
		char_resource_id = GroundResourceID_Mine[ground->ground_id];
		char_id = CHARID_GROUND_MINE;
		cell_resource_id = GROUND00_M_NCER_BIN;
		cell_id = CELLID_GROUND_MINE;
		anm_resource_id = GROUND00_M_NANR_BIN;
		anm_id = CELLANMID_GROUND_MINE;
	}
	else{
		char_resource_id = GroundResourceID_Enemy[ground->ground_id];
		char_id = CHARID_GROUND_ENEMY;
		cell_resource_id = GROUND00_E_NCER_BIN;
		cell_id = CELLID_GROUND_ENEMY;
		anm_resource_id = GROUND00_E_NANR_BIN;
		anm_id = CELLANMID_GROUND_ENEMY;
	}
	
	CATS_LoadResourceCharArcH(csp, crp, hdl, char_resource_id, 1, 
		NNS_G2D_VRAM_TYPE_2DMAIN, char_id);
	CATS_LoadResourcePlttWorkArcH(BattleWorkPfdGet(ground->bw), FADE_MAIN_OBJ, csp, crp, 
		hdl, GroundResourceID_Palette[ground->ground_id][time_zone_offset], 0, 
		1, NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_GROUND);
	//�w�i�`�F���W�̂��߂ɁABG�p���b�g�ɂ��W�J
	PaletteWorkSet_Arc(BattleWorkPfdGet(ground->bw), ARC_BATT_OBJ,
		GroundResourceID_Palette[ground->ground_id][time_zone_offset], HEAPID_BATTLE, 
		FADE_MAIN_BG, 0x20, 0x07*0x10);
	
	CATS_LoadResourceCellArcH(csp, crp, hdl, cell_resource_id, 1, cell_id);
	CATS_LoadResourceCellAnmArcH(csp, crp, hdl, anm_resource_id, 1, anm_id);
	
	ArchiveDataHandleClose( hdl );
}

//--------------------------------------------------------------
/**
 * @brief   �n�ʃA�N�^�[�𐶐�����
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 *
 * ��Ƀ��\�[�X���ǂݍ��܂�Ă���K�v������܂��B
 */
//--------------------------------------------------------------
void Ground_ActorSet(GROUND_WORK *ground)
{
	CATS_SYS_PTR csp;
	CATS_RES_PTR crp;
	const TCATS_OBJECT_ADD_PARAM_S *obj_param;
	
	csp = BattleWorkCATS_SYS_PTRGet(ground->bw);
	crp = BattleWorkCATS_RES_PTRGet(ground->bw);

	obj_param = &GroundObjParam[ground->mine_enemy];

	ground->cap = CATS_ObjectAdd_S(csp, crp, obj_param);
	CATS_ObjectUpdate(ground->cap->act);
}

//--------------------------------------------------------------
/**
 * @brief   �n�ʂ̃A�N�^�[���폜����
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Ground_ActorDel(GROUND_WORK *ground)
{
	if(ground->cap == NULL){
		return;
	}
	CATS_ActorPointerDelete_S(ground->cap);
	ground->cap = NULL;
}

//--------------------------------------------------------------
/**
 * @brief   �n�ʂ̃��\�[�X���������
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Ground_ResourceFree(GROUND_WORK *ground)
{
	CATS_RES_PTR crp;
	int char_id, cell_id, anm_id;
	
	crp = BattleWorkCATS_RES_PTRGet(ground->bw);

	if(ground->mine_enemy == GROUND_TYPE_NORMAL_MINE){
		char_id = CHARID_GROUND_MINE;
		cell_id = CELLID_GROUND_MINE;
		anm_id = CELLANMID_GROUND_MINE;
	}
	else{
		char_id = CHARID_GROUND_ENEMY;
		cell_id = CELLID_GROUND_ENEMY;
		anm_id = CELLANMID_GROUND_ENEMY;
	}
	
	CATS_FreeResourceChar(crp, char_id);
	CATS_FreeResourcePltt(crp, PLTTID_GROUND);
	CATS_FreeResourceCell(crp, cell_id);
	CATS_FreeResourceCellAnm(crp, anm_id);
}

//--------------------------------------------------------------
/**
 * @brief   �n�ʂ̃A�N�^�[�̕\���ݒ�
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 * @param   enable		CATS_ENABLE_TRUE(�\��) or CATS_ENABLE_FALSE(��\��)
 */
//--------------------------------------------------------------
void Ground_EnableSet(GROUND_WORK *ground, int enable)
{
	if(ground->cap == NULL){
		return;
	}
	
	CATS_ObjectEnableCap(ground->cap, enable);
}


//==============================================================================
//
//	�����̊֐����Z�b�g�ɂ�������
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �n�ʂ̃��\�[�X�Z�b�g�ƃA�N�^�[�o�^���Z�b�g�ɂ�������
 *
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 * @param   bw			�퓬�V�X�e�����[�N�ւ̃|�C���^
 * @param   type		�n�ʂ̃O���t�B�b�N�^�C�v(GROUND_TYPE_???)
 * @param   ground_id	�n�ʂ̎��(GROUND_ID_???)
 */
//--------------------------------------------------------------
void Ground_ActorResourceSet(GROUND_WORK *ground, BATTLE_WORK *bw, u16 mine_enemy, int ground_id)
{
	MI_CpuClearFast(ground, sizeof(GROUND_WORK));
	ground->bw = bw;
	ground->mine_enemy = mine_enemy;
	ground->ground_id = ground_id;
	if(ground_id >= GROUND_ID_ALL){
		GF_ASSERT(0);// && "�O���E���hID�����݂��܂���");
		ground->ground_id = 0;
	}
	Ground_ResourceLoad(ground);
	Ground_ActorSet(ground);
}

//--------------------------------------------------------------
/**
 * @brief   �n�ʂ̃A�N�^�[�폜�ƃ��\�[�X������Z�b�g�ɂ�������
 * @param   ground		�n�ʃ��[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void Ground_ActorResourceDel(GROUND_WORK *ground)
{
	Ground_ActorDel(ground);
	Ground_ResourceFree(ground);
	MI_CpuClearFast(ground, sizeof(GROUND_WORK));
}

