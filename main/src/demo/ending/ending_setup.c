//==============================================================================
/**
 * @file	ending_setup.c
 * @brief	�G���f�B���O�̊e�V�[���̏����ݒ�A�������
 * @author	matsuda
 * @date	2008.04.10(��)
 */
//==============================================================================
#include "common.h"
#include "gflib\camera.h"
#include "system\procsys.h"
#include "system\msgdata.h"
#include "system\font_arc.h"
#include "system\brightness.h"
#include "system\clact_util.h"
#include "system\render_oam.h"
#include "system\snd_tool.h"
#include "savedata\zukanwork.h"
#include "system/brightness.h"
#include "system/d3dobj.h"
#include "system/wipe.h"
#include "system/wipe_def.h"

#include "msgdata\msg.naix"

#include "demo\ending.h"
#include "graphic/ending.naix"
#include "ending_common.h"
#include "ending_setup.h"
#include "ending_id.h"
#include "ending_tool.h"
#include "ending_list.h"

#include "system/fontproc.h"


//==============================================================================
//	�\���̒�`
//==============================================================================
///�e�V�[�����ɓW�J�E�������֐��̌^
typedef void (*ENDING_SCENE_FUNC)(ENDING_MAIN_WORK *emw);

///�e�V�[�����̃��C�����[�v�֐��̌^
typedef BOOL (*ENDING_SCENE_MAIN_FUNC)(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);

///�e�V�[�����ɓW�J�E������鏈���֐��\����
typedef struct{
	ENDING_SCENE_FUNC init_bg;					///<BG�W�J
	ENDING_SCENE_FUNC init_3d;					///<3D�W�J
	ENDING_SCENE_FUNC init_mainact_resource;	///<�A�N�^�[���\�[�X�W�J(���C��)
	ENDING_SCENE_FUNC init_subact_resource;		///<�A�N�^�[���\�[�X�W�J(�T�u)
	ENDING_SCENE_FUNC init_main_act;			///<�A�N�^�[����(���C��)
	ENDING_SCENE_FUNC init_sub_act;				///<�A�N�^�[���(�T�u)

	ENDING_SCENE_FUNC free_bg;					///<BG���
	ENDING_SCENE_FUNC free_3d;					///<3D���
	ENDING_SCENE_FUNC free_mainact_resource;	///<�A�N�^�[���\�[�X���(���C��)
	ENDING_SCENE_FUNC free_subact_resource;		///<�A�N�^�[���\�[�X���(�T�u)
	ENDING_SCENE_FUNC free_main_act;			///<�A�N�^�[�폜(���C��)
	ENDING_SCENE_FUNC free_sub_act;				///<�A�N�^�[�폜(�T�u)
	
	ENDING_SCENE_MAIN_FUNC main_func;			///<�V�[�����䃁�C���֐��ւ̃|�C���^
}ENDING_SCENE_LOAD_FUNC;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void HeroResourceSet(ENDING_MAIN_WORK *emw);
static void HeroResourceFree(ENDING_MAIN_WORK *emw);
static void HeroActorSet(ENDING_MAIN_WORK *emw, CATS_ACT_PTR *cap_hero, CATS_ACT_PTR *cap_scarf);
static void HeroActorDel(ENDING_MAIN_WORK *emw, CATS_ACT_PTR cap, CATS_ACT_PTR cap_scraft);
static void BgGraphicSet_MapScene1( ENDING_MAIN_WORK * emw);
static void BgGraphicFree_MapScene1( ENDING_MAIN_WORK * emw);
static void DefaultResourceSet_MapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_MapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_MapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_MapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceSet_MapScene1_Sub(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_MapScene1_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_MapScene1_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_MapScene1_Sub(ENDING_MAIN_WORK *emw);
static void Model3DSet_MapScene1( ENDING_MAIN_WORK * emw);
static void Model3DFree_MapScene1(ENDING_MAIN_WORK *emw);
static void BgGraphicSet_CapScene1( ENDING_MAIN_WORK * emw);
static void BgGraphicFree_CapScene1( ENDING_MAIN_WORK * emw);
static void DefaultResourceSet_CapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_CapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_CapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_CapScene1_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceSet_CapScene1_Sub(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_CapScene1_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_CapScene1_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_CapScene1_Sub(ENDING_MAIN_WORK *emw);
static void Model3DSet_CapScene1( ENDING_MAIN_WORK * emw);
static void Model3DFree_CapScene1(ENDING_MAIN_WORK *emw);
static void BgGraphicSet_MapScene2( ENDING_MAIN_WORK * emw);
static void BgGraphicFree_MapScene2( ENDING_MAIN_WORK * emw);
static void DefaultResourceSet_MapScene2_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_MapScene2_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_MapScene2_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_MapScene2_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceSet_MapScene2_Sub(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_MapScene2_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_MapScene2_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_MapScene2_Sub(ENDING_MAIN_WORK *emw);
static void Model3DSet_MapScene2( ENDING_MAIN_WORK * emw);
static void Model3DFree_MapScene2(ENDING_MAIN_WORK *emw);
static void BgGraphicSet_MapScene3( ENDING_MAIN_WORK * emw);
static void BgGraphicFree_MapScene3( ENDING_MAIN_WORK * emw);
static void DefaultResourceSet_MapScene3_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_MapScene3_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_MapScene3_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_MapScene3_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceSet_MapScene3_Sub(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_MapScene3_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_MapScene3_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_MapScene3_Sub(ENDING_MAIN_WORK *emw);
static void Model3DSet_MapScene3( ENDING_MAIN_WORK * emw);
static void Model3DFree_MapScene3(ENDING_MAIN_WORK *emw);
static void BgGraphicSet_Field( ENDING_MAIN_WORK * emw);
static void BgGraphicFree_Field( ENDING_MAIN_WORK * emw);
static void DefaultResourceSet_Field_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_Field_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_Field_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_Field_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceSet_Field_Sub(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_Field_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_Field_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_Field_Sub(ENDING_MAIN_WORK *emw);
static void Model3DSet_Field( ENDING_MAIN_WORK * emw);
static void Model3DFree_Field(ENDING_MAIN_WORK *emw);
static void BgGraphicSet_Copyright( ENDING_MAIN_WORK * emw);
static void BgGraphicFree_Copyright( ENDING_MAIN_WORK * emw);
static void DefaultResourceSet_Copyright_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_Copyright_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_Copyright_Main(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_Copyright_Main(ENDING_MAIN_WORK *emw);
static void DefaultResourceSet_Copyright_Sub(ENDING_MAIN_WORK *emw);
static void DefaultResourceFree_Copyright_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorSet_Copyright_Sub(ENDING_MAIN_WORK *emw);
static void DefaultActorFree_Copyright_Sub(ENDING_MAIN_WORK *emw);
static void Model3DSet_Copyright( ENDING_MAIN_WORK * emw);
static void Model3DFree_Copyright(ENDING_MAIN_WORK *emw);

//==============================================================================
//	�f�[�^
//==============================================================================
///�e�V�[�����̃��[�h�֐��e�[�u��
static const ENDING_SCENE_LOAD_FUNC EndingSceneLoadFunc[] = {
	{
		BgGraphicSet_MapScene1,
		Model3DSet_MapScene1,
		DefaultResourceSet_MapScene1_Main,
		DefaultResourceSet_MapScene1_Sub,
		DefaultActorSet_MapScene1_Main,
		DefaultActorSet_MapScene1_Sub,
		BgGraphicFree_MapScene1,
		Model3DFree_MapScene1,
		DefaultResourceFree_MapScene1_Main,
		DefaultResourceFree_MapScene1_Sub,
		DefaultActorFree_MapScene1_Main,
		DefaultActorFree_MapScene1_Sub,
		MapScene1_Main,
	},
	{
		BgGraphicSet_CapScene1,
		Model3DSet_CapScene1,
		DefaultResourceSet_CapScene1_Main,
		DefaultResourceSet_CapScene1_Sub,
		DefaultActorSet_CapScene1_Main,
		DefaultActorSet_CapScene1_Sub,
		BgGraphicFree_CapScene1,
		Model3DFree_CapScene1,
		DefaultResourceFree_CapScene1_Main,
		DefaultResourceFree_CapScene1_Sub,
		DefaultActorFree_CapScene1_Main,
		DefaultActorFree_CapScene1_Sub,
		MapCapScene1_Main,
	},
	{
		BgGraphicSet_MapScene2,
		Model3DSet_MapScene2,
		DefaultResourceSet_MapScene2_Main,
		DefaultResourceSet_MapScene2_Sub,
		DefaultActorSet_MapScene2_Main,
		DefaultActorSet_MapScene2_Sub,
		BgGraphicFree_MapScene2,
		Model3DFree_MapScene2,
		DefaultResourceFree_MapScene2_Main,
		DefaultResourceFree_MapScene2_Sub,
		DefaultActorFree_MapScene2_Main,
		DefaultActorFree_MapScene2_Sub,
		MapScene2_Main,
	},
	{//���C���֐��ȊO�̓L���v�`���V�[��1�ƑS������
		BgGraphicSet_CapScene1,
		Model3DSet_CapScene1,
		DefaultResourceSet_CapScene1_Main,
		DefaultResourceSet_CapScene1_Sub,
		DefaultActorSet_CapScene1_Main,
		DefaultActorSet_CapScene1_Sub,
		BgGraphicFree_CapScene1,
		Model3DFree_CapScene1,
		DefaultResourceFree_CapScene1_Main,
		DefaultResourceFree_CapScene1_Sub,
		DefaultActorFree_CapScene1_Main,
		DefaultActorFree_CapScene1_Sub,
		MapCapScene2_Main,
	},
	{
		BgGraphicSet_MapScene3,
		Model3DSet_MapScene3,
		DefaultResourceSet_MapScene3_Main,
		DefaultResourceSet_MapScene3_Sub,
		DefaultActorSet_MapScene3_Main,
		DefaultActorSet_MapScene3_Sub,
		BgGraphicFree_MapScene3,
		Model3DFree_MapScene3,
		DefaultResourceFree_MapScene3_Main,
		DefaultResourceFree_MapScene3_Sub,
		DefaultActorFree_MapScene3_Main,
		DefaultActorFree_MapScene3_Sub,
		MapScene3_Main,
	},
	{//���C���֐��ȊO�̓L���v�`���V�[��1�ƑS������
		BgGraphicSet_Field,
		Model3DSet_Field,
		DefaultResourceSet_Field_Main,
		DefaultResourceSet_Field_Sub,
		DefaultActorSet_Field_Main,
		DefaultActorSet_Field_Sub,
		BgGraphicFree_Field,
		Model3DFree_Field,
		DefaultResourceFree_Field_Main,
		DefaultResourceFree_Field_Sub,
		DefaultActorFree_Field_Main,
		DefaultActorFree_Field_Sub,
		MapFieldScene1_Main,
	},
	{//�R�s�[���C�g
		BgGraphicSet_Copyright,
		Model3DSet_Copyright,
		DefaultResourceSet_Copyright_Main,
		DefaultResourceSet_Copyright_Sub,
		DefaultActorSet_Copyright_Main,
		DefaultActorSet_Copyright_Sub,
		BgGraphicFree_Copyright,
		Model3DFree_Copyright,
		DefaultResourceFree_Copyright_Main,
		DefaultResourceFree_Copyright_Sub,
		DefaultActorFree_Copyright_Main,
		DefaultActorFree_Copyright_Sub,
		MapCopyrightScene_Main,
	},
};


//--------------------------------------------------------------
//	�A�N�^�[�w�b�_
//--------------------------------------------------------------
///�t�����e �A�N�^�[�w�b�_(�T�u���)
static const TCATS_OBJECT_ADD_PARAM_S FuwanteObjParam = {
	0,0, 0,		//x, y, z
	0, SOFTPRI_SUB_FUWANTE, PALOFS_SUB_FUWANTE,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DSUB,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_SUB_FUWANTE,			//�L����
		PLTTID_SUB_FUWANTE,			//�p���b�g
		CELLID_SUB_FUWANTE,			//�Z��
		CELLANMID_SUB_FUWANTE,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_SUB_FUWANTE,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�L������ �A�N�^�[�w�b�_(�T�u���)
static const TCATS_OBJECT_ADD_PARAM_S KyamomeObjParam = {
	0,0, 0,		//x, y, z
	1, SOFTPRI_SUB_KYAMOME, PALOFS_SUB_KYAMOME,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DSUB,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_SUB_KYAMOME,			//�L����
		PLTTID_SUB_KYAMOME,			//�p���b�g
		CELLID_SUB_KYAMOME,			//�Z��
		CELLANMID_SUB_KYAMOME,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_SUB_KYAMOME,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�W�o�R�C�� �A�N�^�[�w�b�_(�T�u���)
static const TCATS_OBJECT_ADD_PARAM_S JibaObjParam = {
	0,0, 0,		//x, y, z
	2, SOFTPRI_SUB_JIBA, PALOFS_SUB_JIBA,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DSUB,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_SUB_JIBA,			//�L����
		PLTTID_SUB_JIBA,			//�p���b�g
		CELLID_SUB_JIBA,			//�Z��
		CELLANMID_SUB_JIBA,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_SUB_JIBA,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///���C����ʗ��ꐯ �A�N�^�[�w�b�_(���C�����)
static const TCATS_OBJECT_ADD_PARAM_S StarObjParam = {
	0,0, 0,		//x, y, z
	3, SOFTPRI_STAR, PALOFS_STAR,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_STAR,			//�L����
		PLTTID_STAR,			//�p���b�g
		CELLID_STAR,			//�Z��
		CELLANMID_STAR,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_STAR,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///�T�u��ʗ��ꐯ �A�N�^�[�w�b�_(�T�u���)
static const TCATS_OBJECT_ADD_PARAM_S StarSubObjParam = {
	0,0, 0,		//x, y, z
	3, SOFTPRI_SUB_STAR, PALOFS_SUB_STAR,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DSUB,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_SUB_JIBA,			//�L����
		PLTTID_SUB_JIBA,			//�p���b�g
		CELLID_SUB_JIBA,			//�Z��
		CELLANMID_SUB_JIBA,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_SUB_STAR,			//BG�v���C�I���e�B
	0,			//Vram�]���t���O
};

///��l�� �A�N�^�[�w�b�_(���C�����)
static const TCATS_OBJECT_ADD_PARAM_S HeroObjParam = {
	128+48+16,192-8*4, 0,		//x, y, z
	0, SOFTPRI_HERO, 0,	//�A�j���ԍ��A�D�揇�ʁA�p���b�g�ԍ�
	NNS_G2D_VRAM_TYPE_2DMAIN,		//�`��G���A
	{	//�g�p���\�[�XID�e�[�u��
		CHARID_HERO,			//�L����
		PLTTID_HERO,			//�p���b�g
		CELLID_HERO,			//�Z��
		CELLANMID_HERO,		//�Z���A�j��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z��
		CLACT_U_HEADER_DATA_NONE,		//�}���`�Z���A�j��
	},
	ACTBGPRI_HERO,			//BG�v���C�I���e�B
	1,			//Vram�]���t���O
};



//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ��l�����\�[�X�o�^�@�@����l���͕����̃V�[���Ŏg�p����̂ŒP�ƂŊ֐���
 *
 * @param   emw		�G���f�B���O���C�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void HeroResourceSet(ENDING_MAIN_WORK *emw)
{
	u8 palno;
	
	if(emw->parent_work->playerSex == PM_MALE){
		palno = CATS_LoadResourcePlttWorkArcH(emw->pfd, FADE_MAIN_OBJ, emw->csp, emw->crp, 
			emw->ending_hdl, NARC_ending_ed_hero1_NCLR, 0, 1, 
			NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_HERO);
		CATS_LoadResourceCharArcH(emw->csp, emw->crp, emw->ending_hdl, 
			NARC_ending_ed_hero1_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 
			CHARID_HERO);
		CATS_LoadResourceCellArcH(emw->csp, emw->crp, emw->ending_hdl, 
			NARC_ending_ed_hero1_NCER, 0, CELLID_HERO);
		CATS_LoadResourceCellAnmArcH(emw->csp, emw->crp, emw->ending_hdl, 
			NARC_ending_ed_hero1_NANR, 0, CELLANMID_HERO);
	}
	else{
		palno = CATS_LoadResourcePlttWorkArcH(emw->pfd, FADE_MAIN_OBJ, emw->csp, emw->crp, 
			emw->ending_hdl, NARC_ending_ed_heroine1_NCLR, 0, 1, 
			NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_HERO);
		CATS_LoadResourceCharArcH(emw->csp, emw->crp, emw->ending_hdl, 
			NARC_ending_ed_heroine1_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 
			CHARID_HERO);
		CATS_LoadResourceCellArcH(emw->csp, emw->crp, emw->ending_hdl, 
			NARC_ending_ed_heroine1_NCER, 0, CELLID_HERO);
		CATS_LoadResourceCellAnmArcH(emw->csp, emw->crp, emw->ending_hdl, 
			NARC_ending_ed_heroine1_NANR, 0, CELLANMID_HERO);
	}
	
	switch(emw->scene_no){
	case ENDING_SCENE_MAP1:
		SoftFadePfd(emw->pfd, FADE_MAIN_OBJ, palno * 16, 16, 4, GX_RGB( 28, 12, 6 ));
		break;
	case ENDING_SCENE_MAP2:
//		SoftFadePfd(emw->pfd, FADE_MAIN_OBJ, palno * 16, 16, 8, GX_RGB( 22,22,20 ));
		break;
	case ENDING_SCENE_MAP3:
		SoftFadePfd(emw->pfd, FADE_MAIN_OBJ, palno * 16, 16, 4, GX_RGB( 11,11,16 ));
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   ��l�����\�[�X����@�@����l���͕����̃V�[���Ŏg�p����̂ŒP�ƂŊ֐���
 *
 * @param   emw		�G���f�B���O���C�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void HeroResourceFree(ENDING_MAIN_WORK *emw)
{
	CATS_FreeResourceChar(emw->crp, CHARID_HERO);
	CATS_FreeResourcePltt(emw->crp, PLTTID_HERO);
	CATS_FreeResourceCell(emw->crp, CELLID_HERO);
	CATS_FreeResourceCellAnm(emw->crp, CELLANMID_HERO);
}

//--------------------------------------------------------------
/**
 * @brief   ��l���A�N�^�[�����@�@����l���͕����̃V�[���Ŏg�p����̂ŒP�ƂŊ֐���
 *
 * @param   emw		�G���f�B���O���C�����[�N�ւ̃|�C���^
 *
 * @retval  �������ꂽ��l���A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void HeroActorSet(ENDING_MAIN_WORK *emw, CATS_ACT_PTR *cap_hero, CATS_ACT_PTR *cap_scarf)
{
	CATS_ACT_PTR cap, cap_s;
	TCATS_OBJECT_ADD_PARAM_S act_head = HeroObjParam;
	
	MI_CpuClear8(&emw->hero_anm_work, sizeof(ENDING_HERO_ANM_WORK));

	cap = CATS_ObjectAdd_S(emw->csp, emw->crp, &act_head);
	CATS_ObjectAutoAnimeSetCap(cap, CATS_ANM_AUTO_ON);
	CATS_ObjectUpdate(cap->act);
	*cap_hero = cap;
	
	//�}�t���[
	act_head.pri = SOFTPRI_HERO_SCARF;	//�{�̂����v���C�I���e�B��������
	cap_s = CATS_ObjectAdd_S(emw->csp, emw->crp, &act_head);
	CATS_ObjectAnimeSeqSetCap(cap_s, 2);
	CATS_ObjectAutoAnimeSetCap(cap_s, CATS_ANM_AUTO_ON);
	CATS_ObjectUpdate(cap_s->act);
	*cap_scarf = cap_s;
	
	//�ڂ̃L�����f�[�^��ǂݍ���
	{
		void *arc_data;
		NNSG2dCharacterData *data;
		u32 eye_data_no;
		
		GF_ASSERT(emw->hero_eye_charbuf == NULL);
		if(emw->parent_work->playerSex == PM_MALE){
			eye_data_no = NARC_ending_ed_hero_eye_NCGR;
		}
		else{
			eye_data_no = NARC_ending_ed_heroine_eye_NCGR;
		}
		emw->hero_eye_charbuf = sys_AllocMemory(HEAPID_ENDING_DEMO, HERO_EYE_CHARBUF_SIZE);
		arc_data = ArcUtil_HDL_CharDataGet(emw->ending_hdl, eye_data_no, 
			0, &data, HEAPID_ENDING_DEMO);
		MI_CpuCopy32(data->pRawData, emw->hero_eye_charbuf, HERO_EYE_CHARBUF_SIZE);
		sys_FreeMemoryEz(arc_data);
		
		DC_FlushRange(emw->hero_eye_charbuf, HERO_EYE_CHARBUF_SIZE);
	}
}

//--------------------------------------------------------------
/**
 * @brief   ��l���A�N�^�[�폜�@�@����l���͕����̃V�[���Ŏg�p����̂ŒP�ƂŊ֐���
 *
 * @param   emw		�G���f�B���O���C�����[�N�ւ̃|�C���^
 * @param   cap		��l���A�N�^�[�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void HeroActorDel(ENDING_MAIN_WORK *emw, CATS_ACT_PTR cap, CATS_ACT_PTR cap_scraft)
{
	CATS_ActorPointerDelete_S(cap);
	CATS_ActorPointerDelete_S(cap_scraft);
	if(emw->hero_eye_charbuf != NULL){
		sys_FreeMemoryEz(emw->hero_eye_charbuf);
		emw->hero_eye_charbuf = NULL;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �V�[�����\�[�X�o�^
 *
 * @param   emw				�G���f�B���O���C�����[�N�ւ̃|�C���^
 * @param   scene_no		�V�[���ԍ�
 */
//--------------------------------------------------------------
void EndingSceneSetup_Load(ENDING_MAIN_WORK *emw, int scene_no)
{
	const ENDING_SCENE_LOAD_FUNC *scene_func;
	
	scene_func = &EndingSceneLoadFunc[scene_no];
	
	scene_func->init_bg(emw);
	scene_func->init_3d(emw);
	scene_func->init_mainact_resource(emw);
	scene_func->init_subact_resource(emw);
	scene_func->init_main_act(emw);
	scene_func->init_sub_act(emw);
}

//--------------------------------------------------------------
/**
 * @brief   �V�[�����\�[�X���
 *
 * @param   emw				�G���f�B���O���C�����[�N�ւ̃|�C���^
 * @param   scene_no		�V�[���ԍ�
 */
//--------------------------------------------------------------
void EndingSceneSetup_Unload(ENDING_MAIN_WORK *emw, int scene_no)
{
	const ENDING_SCENE_LOAD_FUNC *scene_func;
	
	scene_func = &EndingSceneLoadFunc[scene_no];
	
	scene_func->free_bg(emw);
	scene_func->free_3d(emw);
	scene_func->free_mainact_resource(emw);
	scene_func->free_subact_resource(emw);
	scene_func->free_main_act(emw);
	scene_func->free_sub_act(emw);
}

//--------------------------------------------------------------
/**
 * @brief   �V�[�����̃��C���֐��Ăяo��
 *
 * @param   emw				�G���f�B���O���C�����[�N�ւ̃|�C���^
 * @param   scene_no		�V�[���ԍ�
 *
 * @retval  TRUE:�I���B�@FALSE:�p����
 */
//--------------------------------------------------------------
BOOL EndingSceneSetup_MainFunc(ENDING_MAIN_WORK *emw, int scene_no)
{
	BOOL ret;
	
	ret = EndingSceneLoadFunc[scene_no].main_func(emw, &emw->scene_work);
	if(ret == TRUE){
		MI_CpuClear8(&emw->scene_work, sizeof(ENDING_SCENE_WORK));
	}
	return ret;
}


//==============================================================================
//
//	�}�b�v�V�[���P
//
//==============================================================================
//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g�F�}�b�v�V�[���P
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet_MapScene1( ENDING_MAIN_WORK * emw)
{
	GF_BGL_INI *bgl = emw->bgl;
	
	//-- ���C����� --//
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_ed_sora_m01_NCLR, 
		HEAPID_ENDING_DEMO, FADE_MAIN_BG, 0, 0);
	ArcUtil_HDL_BgCharSet(emw->ending_hdl, NARC_ending_ed_sora_m01_NCGR, emw->bgl, 
		FRAME_M_BG, 0, 0, 0, HEAPID_ENDING_DEMO);
	ArcUtil_HDL_ScrnSet(emw->ending_hdl, NARC_ending_ed_sora_m01_NSCR, emw->bgl, 
		FRAME_M_BG, 0, 0, 0, HEAPID_ENDING_DEMO);

	//-- �T�u��� --//
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_ed_sora_s01_NCLR, 
		HEAPID_ENDING_DEMO, FADE_SUB_BG, 0, 0);
	ArcUtil_HDL_BgCharSet(emw->ending_hdl, NARC_ending_ed_sora_s01_NCGR, emw->bgl, 
		FRAME_S_BG, 0, 0, 0, HEAPID_ENDING_DEMO);
	ArcUtil_HDL_ScrnSet(emw->ending_hdl, NARC_ending_ed_sora_s01_NSCR, emw->bgl, 
		FRAME_S_BG, 0, 0, 0, HEAPID_ENDING_DEMO);


	GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_Y_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_Y_SET, 0);
	
	GF_BGL_VisibleSet( FRAME_M_BG, VISIBLE_ON );
	GF_BGL_VisibleSet( FRAME_S_BG, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_BG0, VISIBLE_ON);
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_M_BG);
	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_S_BG);
}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^����F�}�b�v�V�[���P
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicFree_MapScene1( ENDING_MAIN_WORK * emw)
{
	GF_BGL_VisibleSet( FRAME_M_BG, VISIBLE_OFF );
	GF_BGL_VisibleSet( FRAME_S_BG, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_BG0, VISIBLE_OFF);
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_OFF);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g�F�V�[���P
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_MapScene1_Main(ENDING_MAIN_WORK *emw)
{
	HeroResourceSet(emw);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_MapScene1_Main(ENDING_MAIN_WORK *emw)
{
	HeroResourceFree(emw);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_MapScene1_Main(ENDING_MAIN_WORK *emw)
{
	HeroActorSet(emw, &emw->cap[ACT_INDEX_SCENE1_HERO], &emw->cap[ACT_INDEX_SCENE1_HERO_SCARF]);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_MapScene1_Main(ENDING_MAIN_WORK *emw)
{
	HeroActorDel(emw, emw->cap[ACT_INDEX_SCENE1_HERO], emw->cap[ACT_INDEX_SCENE1_HERO_SCARF]);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_MapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	//�t�����e
	CATS_LoadResourcePlttWorkArcH(emw->pfd, FADE_SUB_OBJ, emw->csp, emw->crp, 
		emw->ending_hdl, NARC_ending_ed_oam_NCLR, 0, 10, 
		NNS_G2D_VRAM_TYPE_2DSUB, PLTTID_SUB_FUWANTE);
	CATS_LoadResourceCharArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCGR, 0, NNS_G2D_VRAM_TYPE_2DSUB, 
		CHARID_SUB_FUWANTE);
	CATS_LoadResourceCellArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCER, 0, CELLID_SUB_FUWANTE);
	CATS_LoadResourceCellAnmArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NANR, 0, CELLANMID_SUB_FUWANTE);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_MapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	CATS_FreeResourceChar(emw->crp, CHARID_SUB_FUWANTE);
	CATS_FreeResourcePltt(emw->crp, PLTTID_SUB_FUWANTE);
	CATS_FreeResourceCell(emw->crp, CELLID_SUB_FUWANTE);
	CATS_FreeResourceCellAnm(emw->crp, CELLANMID_SUB_FUWANTE);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_MapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	int i;
	
	//�t�����e
	for(i = ACT_INDEX_SCENE1_FUWANTE_0; i <= ACT_INDEX_SCENE1_FUWANTE_2; i++){
		emw->cap[i] = CATS_ObjectAdd_S(emw->csp, emw->crp, &FuwanteObjParam);
		CATS_ObjectAffineSetCap(emw->cap[i], CLACT_AFFINE_DOUBLE);
		CATS_ObjectAutoAnimeSetCap(emw->cap[i], CATS_ANM_AUTO_ON);
		CATS_ObjectUpdate(emw->cap[i]->act);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_MapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	int i;
	
	for(i = ACT_INDEX_SCENE1_FUWANTE_0; i <= ACT_INDEX_SCENE1_FUWANTE_2; i++){
		CATS_ActorPointerDelete_S(emw->cap[i]);
	}
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g�F�}�b�v�V�[���P
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DSet_MapScene1( ENDING_MAIN_WORK * emw)
{
	int i;
	
	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE1_MDL_TREE_A], emw->ending_hdl, 
		NARC_ending_ed_tree01a_nsbmd, HEAPID_ENDING_DEMO);
	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE1_MDL_TREE_A].pModel);
	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE1_MDL_TREE_A].pModel);
	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE1_MDL_TREE_A].pModel);
	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE1_MDL_TREE_A].pModel);

	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE1_MDL_TREE_B], emw->ending_hdl, 
		NARC_ending_ed_tree01b_nsbmd, HEAPID_ENDING_DEMO);
	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE1_MDL_TREE_B].pModel);
	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE1_MDL_TREE_B].pModel);
	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE1_MDL_TREE_B].pModel);
	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE1_MDL_TREE_B].pModel);

	for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
		//�����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &emw->obj[MODEL_LINE_0][i], &emw->mdl[ENDING_SCENE1_MDL_TREE_A] );
		//���W�ݒ�
		D3DOBJ_SetMatrix( &emw->obj[MODEL_LINE_0][i], 0, 0, 0);
		D3DOBJ_SetScale(&emw->obj[MODEL_LINE_0][i], FX32_ONE, FX32_ONE, FX32_ONE);
		D3DOBJ_SetDraw(&emw->obj[MODEL_LINE_0][i], TRUE );
	}
	for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
		//�����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &emw->obj[MODEL_LINE_1][i], &emw->mdl[ENDING_SCENE1_MDL_TREE_B] );
		//���W�ݒ�
		D3DOBJ_SetMatrix( &emw->obj[MODEL_LINE_1][i], 0, 0, 0);
		D3DOBJ_SetScale(&emw->obj[MODEL_LINE_1][i], FX32_ONE, FX32_ONE, FX32_ONE);
		D3DOBJ_SetDraw(&emw->obj[MODEL_LINE_1][i], TRUE );
	}
	
	emw->mdl_line_num = 2;		//���C����2
	EndingTool_ModelLineArrangeDataInit(emw, 0);
	EndingTool_ModelLineInit(emw);
	emw->draw_3d = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DFree_MapScene1(ENDING_MAIN_WORK *emw)
{
	int i;
	
	for(i = 0; i < ENDING_SCENE_MDL_MAX; i++){
		// �S���\�[�X�j��
		D3DOBJ_MdlDelete( &emw->mdl[i] ); 
	}

	emw->draw_3d = FALSE;
}



//==============================================================================
//
//	�L���v�`���V�[���P
//
//==============================================================================
//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g�F�L���v�`���V�[���P
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet_CapScene1( ENDING_MAIN_WORK * emw)
{
	GF_BGL_INI *bgl = emw->bgl;

	EndingTool_CaptureGraphicTrans(emw, emw->cap_no, FRAME_M_CAPTURE, CAPTURE_TBL_TYPE_PICTURE);
	emw->cap_no++;

	GF_BGL_VisibleSet( FRAME_M_CAPTURE, VISIBLE_ON );
	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_M_BG);
}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^����F�L���v�`���V�[���P
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicFree_CapScene1( ENDING_MAIN_WORK * emw)
{
	GF_BGL_VisibleSet( FRAME_M_CAPTURE, VISIBLE_OFF );
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g�F�V�[���P
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_CapScene1_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_CapScene1_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_CapScene1_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_CapScene1_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_CapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_CapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_CapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_CapScene1_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g�F�L���v�`���V�[���P
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DSet_CapScene1( ENDING_MAIN_WORK * emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DFree_CapScene1(ENDING_MAIN_WORK *emw)
{
	;
}


//==============================================================================
//
//	�}�b�v�V�[���Q
//
//==============================================================================
//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g�F�}�b�v�V�[���Q
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet_MapScene2( ENDING_MAIN_WORK * emw)
{
	GF_BGL_INI *bgl = emw->bgl;
	
	//-- ���C����� --//
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_ed_sora_m02_NCLR, 
		HEAPID_ENDING_DEMO, FADE_MAIN_BG, 0, 0);
	ArcUtil_HDL_BgCharSet(emw->ending_hdl, NARC_ending_ed_sora_m02_NCGR, emw->bgl, 
		FRAME_M_BG, 0, 0, 0, HEAPID_ENDING_DEMO);
	ArcUtil_HDL_ScrnSet(emw->ending_hdl, NARC_ending_ed_sora_m02_NSCR, emw->bgl, 
		FRAME_M_BG, 0, 0, 0, HEAPID_ENDING_DEMO);

	//-- �T�u��� --//
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_ed_sora_s02_NCLR, 
		HEAPID_ENDING_DEMO, FADE_SUB_BG, 0, 0);
	ArcUtil_HDL_BgCharSet(emw->ending_hdl, NARC_ending_ed_sora_s02_NCGR, emw->bgl, 
		FRAME_S_BG, 0, 0, 0, HEAPID_ENDING_DEMO);
	ArcUtil_HDL_ScrnSet(emw->ending_hdl, NARC_ending_ed_sora_s02_NSCR, emw->bgl, 
		FRAME_S_BG, 0, 0, 0, HEAPID_ENDING_DEMO);

	//-- �p���b�g�A�j���p�Ƀo�b�t�@�R�s�[ --//
	{
		NNSG2dPaletteData *palData;
		void *p_work;

		p_work = ArcUtil_HDL_PalDataGet(emw->ending_hdl, 
			NARC_ending_ed_sora_02anm_NCLR, &palData, HEAPID_ENDING_DEMO);
		MI_CpuCopy16(palData->pRawData, 
			emw->scene_work.sw_map2.palanm_buf, 0x20*ENDING_SCENE2_PALANM_NUM);
		sys_FreeMemoryEz(p_work);
	}

	GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_Y_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_Y_SET, 0);

	GF_BGL_VisibleSet( FRAME_M_BG, VISIBLE_ON );
	GF_BGL_VisibleSet( FRAME_S_BG, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_BG0, VISIBLE_ON);
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_M_BG);
	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_S_BG);
}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^����F�}�b�v�V�[���Q
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicFree_MapScene2( ENDING_MAIN_WORK * emw)
{
	GF_BGL_VisibleSet( FRAME_M_BG, VISIBLE_OFF );
	GF_BGL_VisibleSet( FRAME_S_BG, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_BG0, VISIBLE_OFF);
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_OFF);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g�F�V�[���Q
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_MapScene2_Main(ENDING_MAIN_WORK *emw)
{
	HeroResourceSet(emw);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_MapScene2_Main(ENDING_MAIN_WORK *emw)
{
	HeroResourceFree(emw);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_MapScene2_Main(ENDING_MAIN_WORK *emw)
{
	HeroActorSet(emw, &emw->cap[ACT_INDEX_SCENE2_HERO], &emw->cap[ACT_INDEX_SCENE2_HERO_SCARF]);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_MapScene2_Main(ENDING_MAIN_WORK *emw)
{
	HeroActorDel(emw, emw->cap[ACT_INDEX_SCENE2_HERO], emw->cap[ACT_INDEX_SCENE2_HERO_SCARF]);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_MapScene2_Sub(ENDING_MAIN_WORK *emw)
{
	//�L������
	CATS_LoadResourcePlttWorkArcH(emw->pfd, FADE_SUB_OBJ, emw->csp, emw->crp, 
		emw->ending_hdl, NARC_ending_ed_oam_NCLR, 0, 10, 
		NNS_G2D_VRAM_TYPE_2DSUB, PLTTID_SUB_KYAMOME);
	CATS_LoadResourceCharArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCGR, 0, NNS_G2D_VRAM_TYPE_2DSUB, 
		CHARID_SUB_KYAMOME);
	CATS_LoadResourceCellArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCER, 0, CELLID_SUB_KYAMOME);
	CATS_LoadResourceCellAnmArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NANR, 0, CELLANMID_SUB_KYAMOME);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_MapScene2_Sub(ENDING_MAIN_WORK *emw)
{
	CATS_FreeResourceChar(emw->crp, CHARID_SUB_KYAMOME);
	CATS_FreeResourcePltt(emw->crp, PLTTID_SUB_KYAMOME);
	CATS_FreeResourceCell(emw->crp, CELLID_SUB_KYAMOME);
	CATS_FreeResourceCellAnm(emw->crp, CELLANMID_SUB_KYAMOME);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_MapScene2_Sub(ENDING_MAIN_WORK *emw)
{
	int i;
	
	//�L������
	for(i = ACT_INDEX_SCENE2_KYAMOME_0; i <= ACT_INDEX_SCENE2_KYAMOME_1; i++){
		emw->cap[i] = CATS_ObjectAdd_S(emw->csp, emw->crp, &KyamomeObjParam);
		CATS_ObjectAutoAnimeSetCap(emw->cap[i], CATS_ANM_AUTO_ON);
		CATS_ObjectUpdate(emw->cap[i]->act);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_MapScene2_Sub(ENDING_MAIN_WORK *emw)
{
	int i;
	
	for(i = ACT_INDEX_SCENE2_KYAMOME_0; i <= ACT_INDEX_SCENE2_KYAMOME_1; i++){
		CATS_ActorPointerDelete_S(emw->cap[i]);
	}
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g�F�}�b�v�V�[���Q
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DSet_MapScene2( ENDING_MAIN_WORK * emw)
{
	int i;
	
	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE2_MDL_LIGHT], emw->ending_hdl, 
		NARC_ending_ed_light01_nsbmd, HEAPID_ENDING_DEMO);
	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE2_MDL_LIGHT].pModel);
	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE2_MDL_LIGHT].pModel);
	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE2_MDL_LIGHT].pModel);
	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE2_MDL_LIGHT].pModel);

	for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
		//�����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &emw->obj[MODEL_LINE_0][i], &emw->mdl[ENDING_SCENE2_MDL_LIGHT] );
		//���W�ݒ�
		D3DOBJ_SetMatrix( &emw->obj[MODEL_LINE_0][i], 0, 0, 0);
		D3DOBJ_SetScale(&emw->obj[MODEL_LINE_0][i], FX32_ONE, FX32_ONE, FX32_ONE);
		D3DOBJ_SetDraw(&emw->obj[MODEL_LINE_0][i], TRUE );
	}
	for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
		D3DOBJ_SetDraw(&emw->obj[MODEL_LINE_1][i], FALSE );
	}
	
	emw->mdl_line_num = 1;		//���C����1
	EndingTool_ModelLineArrangeDataInit(emw, 1);
	EndingTool_ModelLineInit(emw);
	emw->draw_3d = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DFree_MapScene2(ENDING_MAIN_WORK *emw)
{
	int i;
	
	for(i = 0; i < ENDING_SCENE_MDL_MAX; i++){
		// �S���\�[�X�j��
		D3DOBJ_MdlDelete( &emw->mdl[i] ); 
	}

	emw->draw_3d = FALSE;
}



//==============================================================================
//
//	�}�b�v�V�[���R
//
//==============================================================================
//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g�F�}�b�v�V�[���R
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet_MapScene3( ENDING_MAIN_WORK * emw)
{
	GF_BGL_INI *bgl = emw->bgl;
	
	//-- ���C����� --//
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_ed_sora_m03_NCLR, 
		HEAPID_ENDING_DEMO, FADE_MAIN_BG, 0, 0);
	ArcUtil_HDL_BgCharSet(emw->ending_hdl, NARC_ending_ed_sora_m03_NCGR, emw->bgl, 
		FRAME_M_BG, 0, 0, 0, HEAPID_ENDING_DEMO);
	ArcUtil_HDL_ScrnSet(emw->ending_hdl, NARC_ending_ed_sora_m03_NSCR, emw->bgl, 
		FRAME_M_BG, 0, 0, 0, HEAPID_ENDING_DEMO);

	//-- �T�u��� --//
	PaletteWorkSet_Arc(emw->pfd, ARC_ENDING, NARC_ending_ed_sora_s03_NCLR, 
		HEAPID_ENDING_DEMO, FADE_SUB_BG, 0, 0);
	ArcUtil_HDL_BgCharSet(emw->ending_hdl, NARC_ending_ed_sora_s03_NCGR, emw->bgl, 
		FRAME_S_BG, 0, 0, 0, HEAPID_ENDING_DEMO);
	ArcUtil_HDL_ScrnSet(emw->ending_hdl, NARC_ending_ed_sora_s03_NSCR, emw->bgl, 
		FRAME_S_BG, 0, 0, 0, HEAPID_ENDING_DEMO);

	//-- �p���b�g�A�j���p�Ƀo�b�t�@�R�s�[ --//
	{
		NNSG2dPaletteData *palData;
		void *p_work;

		p_work = ArcUtil_HDL_PalDataGet(emw->ending_hdl, 
			NARC_ending_ed_sora_03anm_NCLR, &palData, HEAPID_ENDING_DEMO);
		MI_CpuCopy16(palData->pRawData, 
			emw->scene_work.sw_map3.palanm_buf, 0x20*ENDING_SCENE3_PALANM_NUM);
		sys_FreeMemoryEz(p_work);
		
		//�ŏ��̃p���b�g�󋵂̓p���b�g�A�j��0�Ԃ̂�]��
		PaletteWorkSet(emw->pfd, emw->scene_work.sw_map3.palanm_buf, FADE_MAIN_BG, 
			16*ENDING_SCENE3_PALANM_PAL_POS, 0x20);
		PaletteWorkSet(emw->pfd, emw->scene_work.sw_map3.palanm_buf, FADE_SUB_BG, 
			16*ENDING_SCENE3_PALANM_PAL_POS, 0x20);
	}


	GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_M_BG, GF_BGL_SCROLL_Y_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_S_BG, GF_BGL_SCROLL_Y_SET, 0);

	GF_BGL_VisibleSet( FRAME_M_BG, VISIBLE_ON );
	GF_BGL_VisibleSet( FRAME_S_BG, VISIBLE_ON );
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_BG0, VISIBLE_ON);
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_ON);

	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_M_BG);
	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_S_BG);
}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^����F�}�b�v�V�[���R
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicFree_MapScene3( ENDING_MAIN_WORK * emw)
{
	GF_BGL_VisibleSet( FRAME_M_BG, VISIBLE_OFF );
	GF_BGL_VisibleSet( FRAME_S_BG, VISIBLE_OFF );
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_BG0, VISIBLE_OFF);
	GF_Disp_GX_VisibleControl(GX_PLANEMASK_OBJ, VISIBLE_OFF);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g�F�V�[���R
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_MapScene3_Main(ENDING_MAIN_WORK *emw)
{
	HeroResourceSet(emw);

	//���ꐯ
	CATS_LoadResourcePlttWorkArcH(emw->pfd, FADE_MAIN_OBJ, emw->csp, emw->crp, 
		emw->ending_hdl, NARC_ending_ed_oam_NCLR, 0, 4, 
		NNS_G2D_VRAM_TYPE_2DMAIN, PLTTID_STAR);
	CATS_LoadResourceCharArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCGR, 0, NNS_G2D_VRAM_TYPE_2DMAIN, 
		CHARID_STAR);
	CATS_LoadResourceCellArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCER, 0, CELLID_STAR);
	CATS_LoadResourceCellAnmArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NANR, 0, CELLANMID_STAR);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_MapScene3_Main(ENDING_MAIN_WORK *emw)
{
	CATS_FreeResourceChar(emw->crp, CHARID_STAR);
	CATS_FreeResourcePltt(emw->crp, PLTTID_STAR);
	CATS_FreeResourceCell(emw->crp, CELLID_STAR);
	CATS_FreeResourceCellAnm(emw->crp, CELLANMID_STAR);

	HeroResourceFree(emw);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_MapScene3_Main(ENDING_MAIN_WORK *emw)
{
	HeroActorSet(emw, &emw->cap[ACT_INDEX_SCENE3_HERO], &emw->cap[ACT_INDEX_SCENE3_HERO_SCARF]);

	//���ꐯ
	emw->cap[ACT_INDEX_SCENE3_STAR_MAIN] = CATS_ObjectAdd_S(emw->csp, emw->crp, &StarObjParam);
	CATS_ObjectAutoAnimeSetCap(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], CATS_ANM_AUTO_ON);
	CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], CATS_ENABLE_FALSE);	//��\��
	CATS_ObjectUpdate(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN]->act);
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_MapScene3_Main(ENDING_MAIN_WORK *emw)
{
	HeroActorDel(emw, emw->cap[ACT_INDEX_SCENE3_HERO], emw->cap[ACT_INDEX_SCENE3_HERO_SCARF]);

	CATS_ActorPointerDelete_S(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN]);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_MapScene3_Sub(ENDING_MAIN_WORK *emw)
{
	//�W�o�R�C��
	CATS_LoadResourcePlttWorkArcH(emw->pfd, FADE_SUB_OBJ, emw->csp, emw->crp, 
		emw->ending_hdl, NARC_ending_ed_oam_NCLR, 0, 10, 
		NNS_G2D_VRAM_TYPE_2DSUB, PLTTID_SUB_JIBA);
	CATS_LoadResourceCharArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCGR, 0, NNS_G2D_VRAM_TYPE_2DSUB, 
		CHARID_SUB_JIBA);
	CATS_LoadResourceCellArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NCER, 0, CELLID_SUB_JIBA);
	CATS_LoadResourceCellAnmArcH(emw->csp, emw->crp, emw->ending_hdl, 
		NARC_ending_ed_oam_NANR, 0, CELLANMID_SUB_JIBA);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_MapScene3_Sub(ENDING_MAIN_WORK *emw)
{
	CATS_FreeResourceChar(emw->crp, CHARID_SUB_JIBA);
	CATS_FreeResourcePltt(emw->crp, PLTTID_SUB_JIBA);
	CATS_FreeResourceCell(emw->crp, CELLID_SUB_JIBA);
	CATS_FreeResourceCellAnm(emw->crp, CELLANMID_SUB_JIBA);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_MapScene3_Sub(ENDING_MAIN_WORK *emw)
{
	int i;
	
	//�W�o�R�C��
	for(i = ACT_INDEX_SCENE3_JIBA_0; i <= ACT_INDEX_SCENE3_JIBA_0; i++){
		emw->cap[i] = CATS_ObjectAdd_S(emw->csp, emw->crp, &JibaObjParam);
		CATS_ObjectAutoAnimeSetCap(emw->cap[i], CATS_ANM_AUTO_ON);
		CATS_ObjectUpdate(emw->cap[i]->act);
	}

	//���ꐯ
	emw->cap[ACT_INDEX_SCENE3_STAR_SUB] = CATS_ObjectAdd_S(emw->csp, emw->crp, &StarSubObjParam);
	CATS_ObjectAutoAnimeSetCap(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], CATS_ANM_AUTO_ON);
	CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], CATS_ENABLE_FALSE);	//��\��
	CATS_ObjectUpdate(emw->cap[ACT_INDEX_SCENE3_STAR_SUB]->act);
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_MapScene3_Sub(ENDING_MAIN_WORK *emw)
{
	int i;
	
	for(i = ACT_INDEX_SCENE3_JIBA_0; i <= ACT_INDEX_SCENE3_JIBA_0; i++){
		CATS_ActorPointerDelete_S(emw->cap[i]);
	}
	CATS_ActorPointerDelete_S(emw->cap[ACT_INDEX_SCENE3_STAR_SUB]);
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g�F�}�b�v�V�[���R
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DSet_MapScene3( ENDING_MAIN_WORK * emw)
{
	int i;
	
	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE3_MDL_TREE_A], emw->ending_hdl, 
		NARC_ending_ed_tree02a_nsbmd, HEAPID_ENDING_DEMO);
	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE3_MDL_TREE_A].pModel);
	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE3_MDL_TREE_A].pModel);
	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE3_MDL_TREE_A].pModel);
	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE3_MDL_TREE_A].pModel);

	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE3_MDL_TREE_A2], emw->ending_hdl, 
		NARC_ending_ed_tree02a2_nsbmd, HEAPID_ENDING_DEMO);
	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE3_MDL_TREE_A2].pModel);
	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE3_MDL_TREE_A2].pModel);
	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE3_MDL_TREE_A2].pModel);
	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE3_MDL_TREE_A2].pModel);

	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE3_MDL_TREE_B], emw->ending_hdl, 
		NARC_ending_ed_tree02b_nsbmd, HEAPID_ENDING_DEMO);
	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE3_MDL_TREE_B].pModel);
	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE3_MDL_TREE_B].pModel);
	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE3_MDL_TREE_B].pModel);
	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE3_MDL_TREE_B].pModel);

	D3DOBJ_MdlLoadH(&emw->mdl[ENDING_SCENE3_MDL_LIGHT], emw->ending_hdl, 
		NARC_ending_ed_light02_nsbmd, HEAPID_ENDING_DEMO);
//	NNS_G3dMdlUseGlbDiff(emw->mdl[ENDING_SCENE3_MDL_LIGHT].pModel);
//	NNS_G3dMdlUseGlbAmb(emw->mdl[ENDING_SCENE3_MDL_LIGHT].pModel);
//	NNS_G3dMdlUseGlbSpec(emw->mdl[ENDING_SCENE3_MDL_LIGHT].pModel);
//	NNS_G3dMdlUseGlbEmi(emw->mdl[ENDING_SCENE3_MDL_LIGHT].pModel);

	for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
		//�����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &emw->obj[MODEL_LINE_0][i], &emw->mdl[ENDING_SCENE3_MDL_TREE_A] );
		//���W�ݒ�
		D3DOBJ_SetMatrix( &emw->obj[MODEL_LINE_0][i], 0, 0, 0);
		D3DOBJ_SetScale(&emw->obj[MODEL_LINE_0][i], FX32_ONE, FX32_ONE, FX32_ONE);
		D3DOBJ_SetDraw(&emw->obj[MODEL_LINE_0][i], TRUE );
	}
	for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
		//�����_�[�I�u�W�F�N�g�ɓo�^
		D3DOBJ_Init( &emw->obj[MODEL_LINE_1][i], &emw->mdl[ENDING_SCENE3_MDL_TREE_B] );
		//���W�ݒ�
		D3DOBJ_SetMatrix( &emw->obj[MODEL_LINE_1][i], 0, 0, 0);
		D3DOBJ_SetScale(&emw->obj[MODEL_LINE_1][i], FX32_ONE, FX32_ONE, FX32_ONE);
		D3DOBJ_SetDraw(&emw->obj[MODEL_LINE_1][i], TRUE );
	}
	
	emw->mdl_line_num = 2;		//���C����2
	EndingTool_ModelLineArrangeDataInit(emw, 2);
	EndingTool_ModelLineInit(emw);
	emw->draw_3d = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DFree_MapScene3(ENDING_MAIN_WORK *emw)
{
	int i;
	
	for(i = 0; i < ENDING_SCENE_MDL_MAX; i++){
		// �S���\�[�X�j��
		D3DOBJ_MdlDelete( &emw->mdl[i] ); 
	}

	emw->draw_3d = FALSE;
}


//==============================================================================
//
//	�t�B�[���h
//
//==============================================================================
//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g�F�t�B�[���h
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet_Field( ENDING_MAIN_WORK * emw)
{
	GF_BGL_INI *bgl = emw->bgl;

	FinalSceneVramSetup(emw);	//VRAM�o���N�؂�ւ�
	
	emw->cap_no = 0;
	EndingTool_CaptureGraphicTrans(emw, emw->cap_no, FRAME_FIN_M_CAP_A, CAPTURE_TBL_TYPE_FIELD);
	emw->cap_no++;
	
	GF_BGL_VisibleSet( FRAME_FIN_M_CAP_A, VISIBLE_ON );
	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_FIN_M_CAP_A);
}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^����F�t�B�[���h
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicFree_Field( ENDING_MAIN_WORK * emw)
{
	GF_BGL_VisibleSet( FRAME_FIN_M_CAP_A, VISIBLE_OFF );
	GF_BGL_VisibleSet( FRAME_FIN_M_CAP_B, VISIBLE_OFF );
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g�F�t�B�[���h
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_Field_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_Field_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_Field_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_Field_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_Field_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_Field_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_Field_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_Field_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g�F�L���v�`���V�[���P
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DSet_Field( ENDING_MAIN_WORK * emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DFree_Field(ENDING_MAIN_WORK *emw)
{
	;
}



//==============================================================================
//
//	�R�s�[���C�g
//
//==============================================================================
//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^�Z�b�g�F�R�s�[���C�g
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicSet_Copyright( ENDING_MAIN_WORK * emw)
{
	GF_BGL_INI *bgl = emw->bgl;
	
	sys.disp3DSW = DISP_3D_TO_MAIN;
	GF_Disp_DispSelect();

	//-- ���C����� --//
	PaletteWork_Clear(emw->pfd, FADE_MAIN_BG, FADEBUF_ALL, 0x0000, 0, 1);

	//-- �T�u��ʁF�u�����v --//
//	PlEnding_ListWork_WriteEndStr( emw->list_work_sub, TRUE );
	{
		STRBUF *tempbuf;
		int xpos;
		
		emw->win_fin = GF_BGL_BmpWinAllocGet( HEAPID_ENDING_DEMO, 1 );
		GF_BGL_BmpWinAdd( bgl, emw->win_fin, FRAME_FIN_S_BG, 0, 0, 32, 32, SYSTEM_FONT_PAL, 0 );
		GF_BGL_BmpWinDataFill( emw->win_fin, 15 );
		GF_BGL_BmpWinMakeScrn( emw->win_fin );
		tempbuf = MSGMAN_AllocString( emw->msgman, PlEnding_FinStrIDGet());
		xpos = (256 - FontProc_GetPrintStrWidth( FONT_SYSTEM, tempbuf, 0 )) / 2;

		GF_STR_PrintExpand(emw->win_fin, FONT_SYSTEM, tempbuf, xpos, 80,
			MSG_NO_PUT, GF_PRINTCOLOR_MAKE(1,2,15), 3, 0, NULL );

		GF_BGL_BmpWinCgxOn( emw->win_fin );
		STRBUF_Delete(tempbuf);
	}

	GF_BGL_ScrollSet(bgl, FRAME_FIN_S_BG, GF_BGL_SCROLL_X_SET, 0);
	GF_BGL_ScrollSet(bgl, FRAME_FIN_S_BG, GF_BGL_SCROLL_Y_SET, 0);

	GF_BGL_VisibleSet( FRAME_FIN_S_BG, VISIBLE_ON );	//�u�����v�̕\��

	GX_SetVisibleWnd(GX_WNDMASK_NONE);
	GXS_SetVisibleWnd(GX_WNDMASK_NONE);

	WIPE_SetMstBrightness( WIPE_DISP_MAIN, -16 );
	WIPE_SetMstBrightness( WIPE_DISP_SUB, -16 );
	SetBrightness(0, ENDING_FADE_PLANE, MASK_DOUBLE_DISPLAY);
	
	GF_BGL_LoadScreenV_Req(emw->bgl, FRAME_FIN_S_BG);
}

//--------------------------------------------------------------------------------------------
/**
 * �O���t�B�b�N�f�[�^����F�R�s�[���C�g
 *
 * @param	emw		�|�P�������X�g��ʂ̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
static void BgGraphicFree_Copyright( ENDING_MAIN_WORK * emw)
{
	GF_BGL_VisibleSet( FRAME_FIN_S_BG, VISIBLE_OFF );

	if(emw->win_fin != NULL){
		GF_BGL_BmpWinDel(emw->win_fin);
		sys_FreeMemoryEz(emw->win_fin);
		emw->win_fin = NULL;
	}
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓OBJ�p���\�[�X�̃Z�b�g�F�V�[��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_Copyright_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_Copyright_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_Copyright_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   ���C����ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_Copyright_Main(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̃Z�b�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceSet_Copyright_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓OBJ�p���\�[�X�̉��
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultResourceFree_Copyright_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[�o�^
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorSet_Copyright_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   �T�u��ʂ̏풓�A�N�^�[���폜����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void DefaultActorFree_Copyright_Sub(ENDING_MAIN_WORK *emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���Z�b�g�F�R�s�[���C�g
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DSet_Copyright( ENDING_MAIN_WORK * emw)
{
	;
}

//--------------------------------------------------------------
/**
 * @brief   3D���f���폜
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void Model3DFree_Copyright(ENDING_MAIN_WORK *emw)
{
	;
}
