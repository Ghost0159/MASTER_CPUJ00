//==============================================================================
/**
 * @file	ending_scene3.c
 * @brief	�V�[���R�F���C��
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
#define MAP_SCENE3_BG_SPEED			(-0x80)

///EVY���Z�l(����8�r�b�g����)
#define MAP_SCENE3_ADD_EVY			(0x0400)

///�p���b�g�A�j���؂�ւ��E�F�C�g
#define ENDING_SCENE3_PALANM_WAIT	0	//((ENDING_COUNTER_SCENE3_END - ENDING_COUNTER_CAP_SCENE2_END) / ENDING_SCENE3_PALANM_NUM)

///�X�^�[�̔��������Z���x
#define STAR_EV_ADD			(3)
///�X�^�[�̑��xX
#define STAR_ADD_X			(-0x6000)	//fx32
///�X�^�[�̑��xY
#define STAR_ADD_Y			(0xd000)	//fx32

//==============================================================================
//	�\���̒�`
//==============================================================================
///�X�^�[�̓���ݒ�\����
typedef struct{
	s16 start_x;
	s16 start_y;
}STAR_MOVE_DATA;

///�W�o�R�C���ړ��p�����[�^
typedef struct{
	s16 x;				///<�����ʒuX
	s16 y;				///<�����ʒuY
	fx32 add_x;			///<�ړ����xX
	fx32 add_y;			///<�ړ����xY

	fx32 add_theta;		///<SIN�J�[�u�p�x���Z�l
	fx32 furihaba;		///<SIN�J�[�u�ӂ蕝
	s32 add_rotate;		///<��]���Z�l
	u16 add_rotate_num;	///<���t���[����]�����Z����������Ԃ���
}JIBA_MOVE_DATA;

//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void MapScene3_JibaInit(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3);
static void MapScene3_JibaUpdate(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3);
static void MapScene3_JibaMove(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3, CATS_ACT_PTR cap, int no);
static void MapScene3_BGScroll(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3);
static void MapScene3_BGPalAnm(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3);
static void MapScene3_StarInit(ENDING_MAIN_WORK *emw);
static void MapScene3_StarUpdate(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3);
static void MapScene3_StarSet(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3, int tblno);
static void MapScene3_StarMove(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3);

//==============================================================================
//	�f�[�^
//==============================================================================
static const JIBA_MOVE_DATA JibaMoveData[] = {
	{
		256 + 48, 			///<�����ʒuX
		-10, 			///<�����ʒuY
		-0xe00, 		///<�ړ����xX(fx32)
		0x600, 		///<�ړ����xY(fx32)
		0x8000,			///<SIN�J�[�u�p�x���Z�l
		0xc000,			///<SIN�J�[�u�ӂ蕝
		150,				///<��]���Z�l
		20,				///<���t���[����]�����Z����������Ԃ���
	},
};


///�X�^�[����f�[�^
static const STAR_MOVE_DATA StarMoveData[] = {
	{
		200,
		0,
	},
	{
		128+16,
		0,
	},
	{
		180,
		0,
	},
};

///�X���̔z�u�Ԋu
#define LIGHT_ARRANGEMENT_INTERVAL			(8)

///�O��̃��f���z�u�f�[�^
ALIGN4 const u16 Scene3MdlSort[] = {
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
	ENDING_SCENE3_MDL_TREE_A2,
};


//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �V�[���R�F���C��
 *
 * @param   emw		�G���f�B���O���C�����[�N�ւ̃|�C���^
 * @param   sw		�V�[�����[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�I���B�@FALSE:�p����
 */
//--------------------------------------------------------------
BOOL MapScene3_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw)
{
	enum{
		SEQ_INIT,
		SEQ_INIT_WAIT,
		SEQ_MAIN,
		SEQ_OUT,
	};
	SCENE_WORK_MAP3 *sw_map3 = &sw->sw_map3;
	s32 mdl_index_no;
	
	switch(sw->seq){
	case SEQ_INIT:
		{
			int i;
			for(i = 0; i < ENDING_SCENE_LINE_3DOBJ_MAX; i++){
				MI_CpuClear8(&emw->obj[MODEL_LINE_0][i].render, sizeof(NNSG3dRenderObj));
				if(i % LIGHT_ARRANGEMENT_INTERVAL == 0){
					mdl_index_no = ENDING_SCENE3_MDL_LIGHT;
				}
				else if(i >= NELEMS(Scene3MdlSort)){
					mdl_index_no = ENDING_SCENE3_MDL_TREE_A2;
				}
				else{
					mdl_index_no = Scene3MdlSort[i];
				}
				NNS_G3dRenderObjInit( 
					&emw->obj[MODEL_LINE_0][i].render, emw->mdl[mdl_index_no].pModel );
			}
			sw_map3->mdl_sort_tblno = ENDING_SCENE_LINE_3DOBJ_MAX;
		}
		MapScene3_JibaInit(emw, sw_map3);
		MapScene3_StarInit(emw);
		ChangeBrightnessRequest(ENDING_FADE_SYNC_SCENE, BRIGHTNESS_NORMAL, BRIGHTNESS_BLACK,
			ENDING_FADE_PLANE, MASK_DOUBLE_DISPLAY);
		sw->seq++;
		break;
	case SEQ_INIT_WAIT:
		if(IsFinishedBrightnessChg(MASK_DOUBLE_DISPLAY) == TRUE){
			//�X���̃��C�g��BG�Ƃ̔�������L���ɂ���ׁAPLANE2��ݒ�
			G2_SetBlendAlpha(0, 
				GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2
				| GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ, 
				31, 0);
			sw->seq++;
		}
		break;
	case SEQ_MAIN:
	#ifdef DEBUG_ENDING_SCENE_SKIP
		if(sys.trg & PAD_BUTTON_Y){
			emw->main_counter = ENDING_COUNTER_SCENE3_END;
		}
	#endif
		if(emw->main_counter >= ENDING_COUNTER_SCENE3_END){
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

	if(sw_map3->mdl_sort_tblno % LIGHT_ARRANGEMENT_INTERVAL == 0){
		mdl_index_no = ENDING_SCENE3_MDL_LIGHT;
	}
	else if(sw_map3->mdl_sort_tblno >= NELEMS(Scene3MdlSort)){
		mdl_index_no = ENDING_SCENE3_MDL_TREE_A2;
	}
	else{
		mdl_index_no = Scene3MdlSort[sw_map3->mdl_sort_tblno];
	}
	if(EndintTool_ModelLineScroll(emw, mdl_index_no) == TRUE){
		sw_map3->mdl_sort_tblno++;
	}

	EndingTool_HeroAnimeUpdate(emw, emw->cap[ACT_INDEX_SCENE3_HERO]);
	MapScene3_JibaUpdate(emw, sw_map3);
	MapScene3_StarUpdate(emw, sw_map3);
	//MapScene3_BGScroll(emw, sw_map3);
	MapScene3_BGPalAnm(emw, sw_map3);
	
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �W�o�R�C�������ݒ�
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void MapScene3_JibaInit(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3)
{
	int i, tblno = 0;
	
	GF_ASSERT(NELEMS(JibaMoveData) == ACT_INDEX_SCENE3_JIBA_0-ACT_INDEX_SCENE3_JIBA_0+1);
	
	for(i = ACT_INDEX_SCENE3_JIBA_0; i <= ACT_INDEX_SCENE3_JIBA_0; i++, tblno++){
		CATS_ObjectAffineSetCap(emw->cap[i], CLACT_AFFINE_DOUBLE);
		CATS_ObjectPosSetCap_SubSurface(emw->cap[i], 
			JibaMoveData[tblno].x, JibaMoveData[tblno].y, ENDING_SUB_ACTOR_DISTANCE);
		sw_map3->jiba_work[tblno].x = JibaMoveData[tblno].x * FX32_ONE;
		sw_map3->jiba_work[tblno].y = JibaMoveData[tblno].y * FX32_ONE;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �W�o�R�C���X�V����
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void MapScene3_JibaUpdate(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3)
{
	int i, tblno = 0;
	
	if(emw->main_counter < ENDING_COUNTER_SCENE3_JIBA){
		return;
	}
	
	for(i = ACT_INDEX_SCENE3_JIBA_0; i <= ACT_INDEX_SCENE3_JIBA_0; i++, tblno++){
		MapScene3_JibaMove(emw, sw_map3, emw->cap[i], tblno);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �W�o�R�C�����쏈��
 *
 * @param   emw		
 * @param   sw_map3		
 * @param   cap		
 * @param   no		
 */
//--------------------------------------------------------------
static void MapScene3_JibaMove(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3, CATS_ACT_PTR cap, int no)
{
	JIBA_WORK *jiba = &sw_map3->jiba_work[no];
	const JIBA_MOVE_DATA *movedata = &JibaMoveData[no];
	fx32 offset_y;
	
	if(jiba->x < -64*FX32_ONE){
		return;
	}
	
	//SIN�J�[�u
	jiba->theta += movedata->add_theta;
	if(jiba->theta >= (360 << FX32_SHIFT)){
		jiba->theta -= 360 << FX32_SHIFT;
	}
	offset_y = FX_Mul(Sin360FX(jiba->theta), movedata->furihaba);

	//���W�ړ�
	jiba->x += movedata->add_x;
	jiba->y += movedata->add_y;
	CATS_ObjectPosSetCapFx32_SubSurface(
		cap, jiba->x, jiba->y + offset_y, ENDING_SUB_ACTOR_DISTANCE);
	
	//�A�t�B���ݒ�
	if(jiba->rotate_dir == 0){
		CATS_ObjectRotationAddCap(cap, movedata->add_rotate);
	}
	else{
		CATS_ObjectRotationAddCap(cap, -movedata->add_rotate);
	}
	jiba->rotate_count++;
	if(jiba->rotate_count >= movedata->add_rotate_num){
		jiba->rotate_count = 0;
		jiba->rotate_dir ^= 1;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �X�^�[�����ݒ�
 *
 * @param   emw		
 */
//--------------------------------------------------------------
static void MapScene3_StarInit(ENDING_MAIN_WORK *emw)
{
	CATS_ObjectObjModeSetCap(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], GX_OAM_MODE_XLU);	//������ON
	CATS_ObjectObjModeSetCap(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], GX_OAM_MODE_XLU);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^�[�X�V����
 *
 * @param   emw		
 * @param   sw_map3		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void MapScene3_StarUpdate(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3)
{
	if(emw->main_counter == ENDING_COUNTER_SCENE3_STAR_0){
		MapScene3_StarSet(emw, sw_map3, 0);
	}
	if(emw->main_counter == ENDING_COUNTER_SCENE3_STAR_1){
		MapScene3_StarSet(emw, sw_map3, 1);
	}
	if(emw->main_counter == ENDING_COUNTER_SCENE3_STAR_2){
		MapScene3_StarSet(emw, sw_map3, 2);
	}
	
	MapScene3_StarMove(emw, sw_map3);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^�[�̓���J�n�ݒ�
 *
 * @param   emw			
 * @param   sw_map3		
 * @param   tblno		
 */
//--------------------------------------------------------------
static void MapScene3_StarSet(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3, int tblno)
{
	GF_ASSERT(sw_map3->star_move == FALSE);		//���̃X�^�[���쒆�͏o���Ȃ�(���������󂢂ĂȂ�)
	GF_ASSERT(NELEMS(StarMoveData) > tblno);
	
	sw_map3->star_move = TRUE;
	sw_map3->star_ud = 0;

	sw_map3->ev1 = 0;
	sw_map3->ev2 = 31;
	
	CATS_ObjectPosSetCap_SubSurface(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], 
		StarMoveData[tblno].start_x, StarMoveData[tblno].start_y, ENDING_SUB_ACTOR_DISTANCE);
	CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], CATS_ENABLE_TRUE);	//�\��
	CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], CATS_ENABLE_FALSE);	//��\��
	
	G2S_SetBlendAlpha(0, 
		GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2
		| GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ, 
		sw_map3->ev1, sw_map3->ev2);
	G2_SetBlendAlpha(0, 
		GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2
		| GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ, 
		sw_map3->ev2, sw_map3->ev1);
}

//--------------------------------------------------------------
/**
 * @brief   �X�^�[����
 *
 * @param   emw		
 * @param   sw_map3		
 */
//--------------------------------------------------------------
static void MapScene3_StarMove(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3)
{
	s16 x, y;
	
	if(sw_map3->star_move == FALSE){
		return;
	}
	
	if(sw_map3->star_ud == 0){	//����
		sw_map3->ev1 += STAR_EV_ADD;
		sw_map3->ev2 -= STAR_EV_ADD;
		if(sw_map3->ev1 > 31){
			sw_map3->ev1 = 31;
		}
		if(sw_map3->ev2 < 0){
			sw_map3->ev2 = 0;
		}
		G2S_ChangeBlendAlpha(sw_map3->ev1, sw_map3->ev2);
		
		CATS_ObjectPosMoveCapFx32(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], STAR_ADD_X, STAR_ADD_Y);
		CATS_ObjectPosGetCap_SubSurface(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], 
			&x, &y, ENDING_SUB_ACTOR_DISTANCE);
		if(y > 192+32){
			CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_SUB], CATS_ENABLE_FALSE);
			CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], CATS_ENABLE_TRUE);
			CATS_ObjectPosSetCap_SubSurface(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], 
				x, -32, ENDING_SUB_ACTOR_DISTANCE);
			sw_map3->star_ud = 1;
		}
	}
	else{	//�����
		sw_map3->ev2 += STAR_EV_ADD;
		sw_map3->ev1 -= STAR_EV_ADD;
		if(sw_map3->ev2 > 31){
			sw_map3->ev2 = 31;
		}
		if(sw_map3->ev1 < 0){
			sw_map3->ev1 = 0;
		}
		G2_ChangeBlendAlpha(sw_map3->ev1, sw_map3->ev2);
		
		CATS_ObjectPosMoveCapFx32(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], STAR_ADD_X, STAR_ADD_Y);
		CATS_ObjectPosGetCap_SubSurface(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], 
			&x, &y, ENDING_SUB_ACTOR_DISTANCE);
		if(y > 192+32){
			CATS_ObjectEnableCap(emw->cap[ACT_INDEX_SCENE3_STAR_MAIN], CATS_ENABLE_FALSE);
			sw_map3->star_ud = 0;
			sw_map3->star_move = FALSE;
		}
	}
}

//--------------------------------------------------------------
/**
 * @brief   �w�iBG�X�N���[��
 *
 * @param   emw			
 * @param   sw_map3		�}�b�v�V�[��0���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void MapScene3_BGScroll(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3)
{
	sw_map3->bg_scr_y += MAP_SCENE3_BG_SPEED;
	GF_BGL_ScrollSet(emw->bgl, FRAME_M_BG, GF_BGL_SCROLL_Y_SET, sw_map3->bg_scr_y / FX32_ONE);
	GF_BGL_ScrollSet(emw->bgl, FRAME_S_BG, GF_BGL_SCROLL_Y_SET, sw_map3->bg_scr_y / FX32_ONE);
}

//--------------------------------------------------------------
/**
 * @brief   �w�i�p���b�g�A�j��
 *
 * @param   emw		
 * @param   sw_map3		
 */
//--------------------------------------------------------------
static void MapScene3_BGPalAnm(ENDING_MAIN_WORK *emw, SCENE_WORK_MAP3 *sw_map3)
{
	int i, s;
	int anm_pos, evy, next_color_pos;
	u16 *trans_buf_m, *trans_buf_s;
	
	switch(sw_map3->anm_seq){
	case 0:
		if(emw->main_counter < ENDING_COUNTER_SCENE3_PALANM_START){
			return;
		}
		if(sw_map3->anm_pos >= ENDING_SCENE3_PALANM_NUM){
			return;
		}
		
		if(sw_map3->anm_frame > ENDING_SCENE3_PALANM_WAIT * (sw_map3->anm_pos + 1)){
			sw_map3->anm_seq++;
		}
		break;
	case 1:
		anm_pos = sw_map3->anm_pos;
		sw_map3->evy += MAP_SCENE3_ADD_EVY;
		
		if(sw_map3->evy >= (16 << 8)){
			evy = 16;
			sw_map3->evy = 0;
			sw_map3->anm_pos++;
			sw_map3->anm_seq = 0;
		}
		else{
			evy = sw_map3->evy >> 8;
		}

		next_color_pos = anm_pos + 1;
		if(next_color_pos >= ENDING_SCENE3_PALANM_NUM){
			next_color_pos = ENDING_SCENE3_PALANM_NUM - 1;//anm_pos - 1;
		}
		
		trans_buf_m = PaletteWorkTransWorkGet( emw->pfd, FADE_MAIN_BG );
		trans_buf_s = PaletteWorkTransWorkGet( emw->pfd, FADE_SUB_BG );
		for(s = 0; s < 16; s++){
			SoftFade(&sw_map3->palanm_buf[anm_pos][s], 
				&trans_buf_m[s + 16*ENDING_SCENE3_PALANM_PAL_POS],
				1, evy, sw_map3->palanm_buf[next_color_pos][s]);
			SoftFade(&sw_map3->palanm_buf[anm_pos][s], 
				&trans_buf_s[s + 16*ENDING_SCENE3_PALANM_PAL_POS],
				1, evy, sw_map3->palanm_buf[next_color_pos][s]);
		}
		break;
	}
	
	sw_map3->anm_frame++;
}

