//==============================================================================
/**
 * @file	frontier_actor.c
 * @brief	�t�����e�B�A2D�}�b�v�Ŏg�p����A�N�^�[
 * @author	matsuda
 * @date	2007.05.30(��)
 */
//==============================================================================
#include "common.h"
#include "system/arc_tool.h"
#include "system/clact_tool.h"

#include "frontier_act_pri.h"
#include "graphic/frontier_obj_def.h"
#include "frontier_act_id.h"
#include "frontier_actor.h"


//==============================================================================
//	�A�N�^�[�w�b�_
//==============================================================================
//==============================================================================
//
//	�A�N�^�[�w�b�_
//			���\�[�XID�����̂܂܃}�l�[�W���̊Ǘ�ID�Ƃ��Ă��g�p���Ă��܂�
//
//	��frontier_act_id.h��FrontierArticleActorHeadTbl�̕��тŒ�`���������ƁI
//
//==============================================================================
static const TCATS_OBJECT_ADD_PARAM_S FrontierArticleActorHeadTbl[] = {
	//ACTID_TEST_BALL
	{
		0, 0, 0,						//x, y, z
		0, ACT_SOFTPRI_TEST, 0,			//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
#if 0
			TEST_BALL32K_NCGR_BIN,		//�L����
			TEST_BALL32K_NCLR,				//�p���b�g
			TEST_BALL32K_NCER_BIN,		//�Z��
			TEST_BALL32K_NANR_BIN,		//�Z���A�j��
#else
			BT_OBJ_DOOR_NCGR_BIN,		//�L����
			BT_OBJ_DOOR_NCLR,			//�p���b�g
			BT_OBJ_DOOR_NCER_BIN,		//�Z��
			BT_OBJ_DOOR_NANR_BIN,		//�Z���A�j��
#endif
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z���A�j��
		},
		ACT_BGPRI_TEST,					//BG�v���C�I���e�B
		0,								//VRAM�]���t���O
	},
/*	//ACTID_TEST_BALL2
	{
		0, 0, 0,						//x, y, z
		0, ACT_SOFTPRI_TEST, 0,			//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
			TEST_BALL32K_NCGR_BIN,		//�L����
			TEST_BALL32K_NCLR,				//�p���b�g
			TEST_BALL32K_NCER_BIN,		//�Z��
			TEST_BALL32K_NANR_BIN,		//�Z���A�j��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z���A�j��
		},
		ACT_BGPRI_TEST,					//BG�v���C�I���e�B
		0,								//VRAM�]���t���O
	},
*/
	//ACTID_TOWER_DOOR
	{
		0, 0, 0,						//x, y, z
		0, ACT_SOFTPRI_TEST, 0,			//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
			BT_OBJ_DOOR_NCGR_BIN,		//�L����
			BT_OBJ_DOOR_NCLR,			//�p���b�g
			BT_OBJ_DOOR_NCER_BIN,		//�Z��
			BT_OBJ_DOOR_NANR_BIN,		//�Z���A�j��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z���A�j��
		},
		ACT_BGPRI_TEST,					//BG�v���C�I���e�B
		0,								//VRAM�]���t���O
	},

	//ACTID_ROULETTE_RAMP
	{
		0, 0, 0,						//x, y, z
		0, ACT_SOFTPRI_TEST, 0,			//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
			BR_RAMP_NCGR_BIN,			//�L����
			BR_RAMP_NCLR,				//�p���b�g
			BR_RAMP_NCER_BIN,			//�Z��
			BR_RAMP_NANR_BIN,			//�Z���A�j��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z���A�j��
		},
		ACT_BGPRI_TEST,					//BG�v���C�I���e�B
		0,								//VRAM�]���t���O
	},

	//ACTID_ROULETTE_POKEPANEL
	{
		0, 0, 0,						//x, y, z
		0, ACT_SOFTPRI_TEST, 0,			//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
		NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
		{	//�g�p���\�[�XID�e�[�u��
			BR_POKEPANEL_NCGR_BIN,		//�L����
			BR_POKEPANEL_NCLR,			//�p���b�g
			BR_POKEPANEL_NCER_BIN,		//�Z��
			BR_POKEPANEL_NANR_BIN,		//�Z���A�j��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z��
			CLACT_U_HEADER_DATA_NONE,	//�}���`�Z���A�j��
		},
		ACT_BGPRI_TEST,					//BG�v���C�I���e�B
		0,								//VRAM�]���t���O
	},

};



//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   
 *
 * @param   csp		
 * @param   crp		
 * @param   hdl		
 * @param   pfd		
 * @param   act_id		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
void FAct_ResourceLoad(CATS_SYS_PTR csp, CATS_RES_PTR crp, ARCHANDLE* hdl, 
	PALETTE_FADE_PTR pfd, u16 act_id)
{
	const TCATS_OBJECT_ADD_PARAM_S *obj_head;
	
	GF_ASSERT(act_id < NELEMS(FrontierArticleActorHeadTbl));
	obj_head = &FrontierArticleActorHeadTbl[act_id];
	
	CATS_LoadResourceCharArcH(csp, crp, hdl, obj_head->id[CLACT_U_CHAR_RES], 1, 
		NNS_G2D_VRAM_TYPE_2DMAIN, obj_head->id[CLACT_U_CHAR_RES]);
	CATS_LoadResourcePlttWorkArcH(pfd, FADE_MAIN_OBJ, csp, crp, 
		hdl, obj_head->id[CLACT_U_PLTT_RES], 0, 
		1, NNS_G2D_VRAM_TYPE_2DMAIN, obj_head->id[CLACT_U_PLTT_RES]);
	CATS_LoadResourceCellArcH(csp, crp, hdl, obj_head->id[CLACT_U_CELL_RES], 1, 
		obj_head->id[CLACT_U_CELL_RES]);
	CATS_LoadResourceCellAnmArcH(csp, crp, hdl, 
		obj_head->id[CLACT_U_CELLANM_RES], 1, obj_head->id[CLACT_U_CELLANM_RES]);
}

//--------------------------------------------------------------
/**
 * @brief   �Q�[�W�̃��\�[�X���������
 * @param   gauge		�Q�[�W���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void FAct_ResourceFree(CATS_RES_PTR crp, u16 act_id)
{
	const TCATS_OBJECT_ADD_PARAM_S *obj_head;

	GF_ASSERT(act_id < NELEMS(FrontierArticleActorHeadTbl));
	obj_head = &FrontierArticleActorHeadTbl[act_id];

	//���\�[�X���
	CATS_FreeResourceChar(crp, obj_head->id[CLACT_U_CHAR_RES]);
	CATS_FreeResourcePltt(crp, obj_head->id[CLACT_U_PLTT_RES]);
	CATS_FreeResourceCell(crp, obj_head->id[CLACT_U_CELL_RES]);
	CATS_FreeResourceCellAnm(crp, obj_head->id[CLACT_U_CELLANM_RES]);
}

//--------------------------------------------------------------
/**
 * @brief   
 *
 * @param   csp		
 * @param   crp		
 * @param   act_id		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
CATS_ACT_PTR FAct_ActorSet(CATS_SYS_PTR csp, CATS_RES_PTR crp, u16 act_id)
{
	CATS_ACT_PTR cap;
	
	GF_ASSERT(act_id < NELEMS(FrontierArticleActorHeadTbl));
	
	cap = CATS_ObjectAdd_S(csp, crp, &FrontierArticleActorHeadTbl[act_id]);
	CATS_ObjectUpdate(cap->act);
	return cap;
}

//--------------------------------------------------------------
/**
 * @brief   
 *
 * @param   cap		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
void FAct_ActorDel(CATS_ACT_PTR cap)
{
	CATS_ActorPointerDelete_S(cap);
}

