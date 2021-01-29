//==============================================================================
/**
 * @file	ending_scene1.c
 * @brief	�V�[���P�F���C��
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

#include "msgdata\msg.naix"

#include "demo\ending.h"
#include "graphic/ending.naix"
#include "ending_common.h"
#include "ending_setup.h"
#include "ending_tool.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�w�i�X�N���[�����xX(fx32)
#define MAP_SCENE1_BG_SPEED			(-0x40)

//==============================================================================
//	�\���̒�`
//==============================================================================
///�t�����e���쐧��\����
typedef struct{
	s32 start_wait;		///<����J�n���܂ł̃E�F�C�g
	fx32 start_x;		///<�o����WX
	fx32 start_y;		///<�o����WY
	fx32 add_x;			///<X���Z�l
	fx32 add_y;			///<Y���Z�l
	fx32 add_theta;		///<SIN�J�[�u�p�x���Z�l
	fx32 furihaba;		///<SIN�J�[�u�ӂ蕝
	f32 add_scale;		///<�X�P�[�����Z�l(float)
	u16 add_scale_num;	///<���t���[���X�P�[�������Z����������Ԃ���
	u16 padding;
}FUWANTE_MOVE_DATA;

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void MapScene1_FuwanteInit(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1);
static void MapScene1_FuwanteUpdate(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1);
static void MapScene1_BGScroll(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1);
static void FuwanteMove(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1, CATS_ACT_PTR cap, int no);

//==============================================================================
//	�f�[�^
//==============================================================================
///�t�����e���쐧��f�[�^
static const FUWANTE_MOVE_DATA FuwanteMoveDataTbl[] = {
	{
		ENDING_COUNTER_SCENE1_FUWANTE_0,						//����J�n���܂ł̃E�F�C�g
		-64 * FX32_ONE, 			//�o����WX
		80 * FX32_ONE, 			//�o����WY
		0x600,					//���Z�lX
		-0x80,					//���Z�lY
		0xe00,					//SIN�J�[�u���Z�l(fx32)
		0x8000,					//SIN�J�[�u�ӂ蕝(fx32)
		0.0030,					//�A�t�B�����Z�l(float)
		60,						//���t���[���A�t�B�������Z������܂�Ԃ���
	},
	{
		ENDING_COUNTER_SCENE1_FUWANTE_1,						//����J�n���܂ł̃E�F�C�g
		-64 * FX32_ONE, 			//�o����WX
		190 * FX32_ONE, 			//�o����WY
		0x780,					//���Z�lX
		-0xa0,					//���Z�lY
		0x1000,					//SIN�J�[�u���Z�l(fx32)
		0xc000,					//SIN�J�[�u�ӂ蕝(fx32)
		-0.0040,					//�A�t�B�����Z�l(float)
		50,						//���t���[���A�t�B�������Z������܂�Ԃ���
	},
	{
		ENDING_COUNTER_SCENE1_FUWANTE_2,						//����J�n���܂ł̃E�F�C�g
		-64 * FX32_ONE, 		//�o����WX
		120 * FX32_ONE, 		//�o����WY
		0x640,					//���Z�lX
		-0x90,					//���Z�lY
		0xc00,					//SIN�J�[�u���Z�l(fx32)
		0x8000,					//SIN�J�[�u�ӂ蕝(fx32)
		-0.0045,					//�A�t�B�����Z�l(float)
		60,						//���t���[���A�t�B�������Z������܂�Ԃ���
	},
};



//--------------------------------------------------------------
/**
 * @brief   �V�[���P�F���C��
 *
 * @param   emw		�G���f�B���O���C�����[�N�ւ̃|�C���^
 * @param   sw		�V�[�����[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�p����
 */
//--------------------------------------------------------------
BOOL MapScene1_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw)
{
	enum{
		SEQ_INIT,
		SEQ_MAIN,
		SEQ_OUT,
	};
	SCENE_WORK_MAP1 *sw_map1 = &sw->sw_map1;
	
	switch(sw->seq){
	case SEQ_INIT:
		MapScene1_FuwanteInit(emw, sw_map1);
		ChangeBrightnessRequest(ENDING_FADE_SYNC_SCENE, BRIGHTNESS_NORMAL, BRIGHTNESS_BLACK,
			ENDING_FADE_PLANE, MASK_DOUBLE_DISPLAY);
		sw->seq++;
		break;
	case SEQ_MAIN:
	#ifdef DEBUG_ENDING_SCENE_SKIP
		if(sys.trg & PAD_BUTTON_Y){
			emw->main_counter = ENDING_COUNTER_SCENE1_END;
		}
	#endif
		if(emw->main_counter >= ENDING_COUNTER_SCENE1_END){
			ChangeBrightnessRequest(ENDING_FADE_SYNC_SCENE, BRIGHTNESS_BLACK, BRIGHTNESS_NORMAL,
				ENDING_FADE_PLANE, MASK_DOUBLE_DISPLAY);
			sw->seq++;
		}
		break;
	case SEQ_OUT:
		if(IsFinishedBrightnessChg(MASK_DOUBLE_DISPLAY) == TRUE){
			return TRUE;
		}
		break;
	}

	EndingTool_HeroAnimeUpdate(emw, emw->cap[ACT_INDEX_SCENE1_HERO]);
	MapScene1_FuwanteUpdate(emw, sw_map1);
	EndintTool_ModelLineScroll(emw, -1);
	MapScene1_BGScroll(emw, sw_map1);
	
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�����ݒ�
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void MapScene1_FuwanteInit(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1)
{
	int i, tblno = 0;
	
	GF_ASSERT(NELEMS(FuwanteMoveDataTbl) == ACT_INDEX_SCENE1_FUWANTE_2-ACT_INDEX_SCENE1_FUWANTE_0+1);
	
	for(i = ACT_INDEX_SCENE1_FUWANTE_0; i <= ACT_INDEX_SCENE1_FUWANTE_2; i++, tblno++){
		CATS_ObjectPosSetCapFx32_SubSurface(emw->cap[i], 
			FuwanteMoveDataTbl[tblno].start_x, 
			FuwanteMoveDataTbl[tblno].start_y, 
			ENDING_SUB_ACTOR_DISTANCE);
		sw_map1->fuwante_work[tblno].x = FuwanteMoveDataTbl[tblno].start_x;
		sw_map1->fuwante_work[tblno].y = FuwanteMoveDataTbl[tblno].start_y;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�X�V����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void MapScene1_FuwanteUpdate(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1)
{
	int i, tblno = 0;
	
	for(i = ACT_INDEX_SCENE1_FUWANTE_0; i <= ACT_INDEX_SCENE1_FUWANTE_2; i++, tblno++){
		FuwanteMove(emw, sw_map1, emw->cap[i], tblno);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e���쏈��
 *
 * @param   emw			
 * @param   sw_map1		
 * @param   cap			�Ώۂ̃t�����e�A�N�^�[�ւ̃|�C���^
 * @param   no			�t�����e�ԍ�
 */
//--------------------------------------------------------------
static void FuwanteMove(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1, CATS_ACT_PTR cap, int no)
{
	FUWANTE_WORK *fuwante = &sw_map1->fuwante_work[no];
	const FUWANTE_MOVE_DATA *movedata = &FuwanteMoveDataTbl[no];
	fx32 offset_y;
	
	if(fuwante->start_wait <  movedata->start_wait){
		fuwante->start_wait++;
		return;
	}
	
	//SIN�J�[�u
	fuwante->theta += movedata->add_theta;
	if(fuwante->theta >= (360 << FX32_SHIFT)){
		fuwante->theta -= 360 << FX32_SHIFT;
	}
	offset_y = FX_Mul(Sin360FX(fuwante->theta), movedata->furihaba);

	//���W�ړ�
	fuwante->x += movedata->add_x;
	fuwante->y += movedata->add_y;
	CATS_ObjectPosSetCapFx32_SubSurface(
		cap, fuwante->x, fuwante->y + offset_y, ENDING_SUB_ACTOR_DISTANCE);
	
	//�A�t�B���ݒ�
	if(fuwante->scale_dir == 0){
		CATS_ObjectScaleAddCap(cap, movedata->add_scale, movedata->add_scale);
	}
	else{
		CATS_ObjectScaleAddCap(cap, -movedata->add_scale, -movedata->add_scale);
	}
	fuwante->scale_count++;
	if(fuwante->scale_count >= movedata->add_scale_num){
		fuwante->scale_count = 0;
		fuwante->scale_dir ^= 1;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �w�iBG�X�N���[��
 *
 * @param   emw			
 * @param   sw_map1		�}�b�v�V�[��0���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void MapScene1_BGScroll(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP1 *sw_map1)
{
	sw_map1->bg_scr_x += MAP_SCENE1_BG_SPEED;
	GF_BGL_ScrollSet(emw->bgl, FRAME_M_BG, GF_BGL_SCROLL_X_SET, sw_map1->bg_scr_x / FX32_ONE);
	GF_BGL_ScrollSet(emw->bgl, FRAME_S_BG, GF_BGL_SCROLL_X_SET, sw_map1->bg_scr_x / FX32_ONE);
}

