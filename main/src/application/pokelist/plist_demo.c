//==============================================================================
/**
 * @file	plist_demo.c
 * @brief	�|�P�������X�g��ł̃V�F�C�~�A�M���e�B�i�̃t�H�����`�F���W�f��
 * @author	matsuda
 * @date	2007.12.15(�y)
 */
//==============================================================================
#include "common.h"

#include "system/procsys.h"
#include "system/clact_tool.h"
#include "system/bmp_menu.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/wordset.h"
#include "system/numfont.h"
#include "system/window.h"
#include "system/snd_tool.h"
#include "battle/battle_common.h"
#include "poketool/pokeparty.h"
#include "poketool/status_rcv.h"
#include "poketool/waza_tool.h"
#include "poketool/poke_regulation.h"
#include "itemtool/item.h"
#include "itemtool/myitem.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_pokelist.h"
#include "application/app_tool.h"
#include "application/p_status.h"
#include "../../field/zonedata.h"
#include "../../field/fieldsys.h"

#define	PLIST_ITEM_H_GLOBAL
#include "application/pokelist.h"
#include "plist_sys.h"
#include "plist_bmp.h"
#include "plist_obj.h"
#include "plist_item.h"
#include "plist_snd_def.h"

#include "system/particle.h"
#include "particledata/pl_pokelist/pokelist_particle_lst.h"
#include "particledata/pl_pokelist/pokelist_particle_def.h"
#include "battle/battle_server.h"

#include "plist_demo_snd_def.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�퓬�p�[�e�B�N���̃J�����j�A�ݒ�
#define BP_NEAR			(FX32_ONE)
///�퓬�p�[�e�B�N���̃J�����t�@�[�ݒ�
#define BP_FAR			(FX32_ONE * 900)

///�p�[�e�B�N�����o�āA���̎��Ԃ����o������|�P�����A�C�R���ύX
#define ICON_CHANGE_WAIT_GIRATHINA		(65)
///�p�[�e�B�N�����o�āA���̎��Ԃ����o������|�P�����A�C�R���ύX
#define ICON_CHANGE_WAIT_SHEIMI			(35)

//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct _LIST_FORMDEMO_WORK{
	int seq;
	int wait;
	int wait_max;
	int monsno;
	u32 spa_id;				///<SPA��ID
	int pos;				///<�J�[�\���ʒu
	PTC_PTR ptc;			///<�p�[�e�B�N���V�X�e���ւ̃|�C���^
}LIST_FORMDEMO_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void PokeList_FormDemoEnd(PLIST_WORK *wk);
static void ListDemo_EffectInit(PLIST_WORK *wk);
static void ListDemo_EffectExit(PLIST_WORK *wk);
static void ListDemo_ParticleSystemCreate(LIST_FORMDEMO_PTR demo);
static void ListDemo_ParticleAdd(LIST_FORMDEMO_PTR demo);
static int ListDemo_ParticleMain(void);
static void ListDemo_ParticleExit(LIST_FORMDEMO_PTR demo);
static u32 sAllocTex(u32 size, BOOL is4x4comp);
static u32 sAllocTexPalette(u32 size, BOOL is4pltt);
static void EmitCall_PosSet(EMIT_PTR emit);

//==============================================================================
//	�f�[�^
//==============================================================================
///�|�P�������X�g�̃J�[�\���ʒu�ɑΉ������G�~�b�^�ʒu(X,Y)
static const fx32 EmitPos[][2] = {
	{-16500, 12000},
	{5000, 11500},
	{-16500, 5000},
	{5000, 3500},
	{-16500, -3000},
	{5000, -4500},
};


//--------------------------------------------------------------------------------------------
/**
 * �t�H�����`�F���W�f�������ݒ�
 *
 * @param	wk		�|�P�������X�g�̃��[�N
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void PokeList_FormDemoInit( PLIST_WORK * wk )
{
	GF_ASSERT(wk->demo == NULL);
	
	wk->demo = sys_AllocMemory(HEAPID_POKELIST, sizeof(LIST_FORMDEMO_WORK));
	MI_CpuClear8(wk->demo, sizeof(LIST_FORMDEMO_WORK));
	wk->demo->pos = wk->pos;
}

//--------------------------------------------------------------
/**
 * @brief   �t�H�����`�F���W�f�����C������
 *
 * @param   wk		�|�P�������X�g�̃��[�N
 *
 * @retval  TRUE:�G�t�F�N�g�I���B�@�@FALSE:�G�t�F�N�g��
 */
//--------------------------------------------------------------
int PokeList_FormDemoMain(PLIST_WORK *wk)
{
	LIST_FORMDEMO_PTR demo = wk->demo;
	POKEMON_PARAM * pp = PokeParty_GetMemberPointer( wk->dat->pp, wk->pos );
	enum{
		DEMOSEQ_INIT,
		
		DEMOSEQ_USE_MSG,
		DEMOSEQ_USE_MSG_WAIT,
		
		DEMOSEQ_EFF_INIT,
		DEMOSEQ_EFF_ADD,
		DEMOSEQ_EFF_MAIN,
		DEMOSEQ_EFF_END,

		DEMOSEQ_VOICE_PLAY,
		DEMOSEQ_VOICE_WAIT,
		
		DEMOSEQ_END_MSG,
		DEMOSEQ_END_MSG_WAIT,
		
		DEMOSEQ_FINISH,
	};
	
	switch(demo->seq){
	case DEMOSEQ_INIT:
		{//�|�P�����̃p�����[�^�ύX
			demo->monsno = PokeParaGet(pp, ID_PARA_monsno, NULL);
			switch(demo->monsno){
			case MONSNO_KIMAIRAN:
				PokeParaGirathinaFormChange(pp);
				demo->wait_max = ICON_CHANGE_WAIT_GIRATHINA;
				demo->spa_id = DEMO_GIRATHINA_SPA;
				break;
			case MONSNO_EURISU:
				PokeParaSheimiFormChange(pp, FORMNO_SHEIMI_FLOWER);
				demo->wait_max = ICON_CHANGE_WAIT_SHEIMI;
				demo->spa_id = DEMO_SHEIMI_SPA;
				break;
			case MONSNO_PURAZUMA:	//���󃍃g���̓f���̗\�薳�� 2007.10.12(��)
			default:
				GF_ASSERT(0);
				break;
			}
			ZukanWork_SetPokeGet(
				SaveData_GetZukanWork(GameSystem_GetSaveData(wk->dat->fsys)), pp);//�}�ӓo�^
		}
		demo->seq++;
		break;
		
	case DEMOSEQ_USE_MSG:
	case DEMOSEQ_USE_MSG_WAIT:
		demo->seq++;
		break;
	
	case DEMOSEQ_EFF_INIT:
		ListDemo_EffectInit(wk);
		demo->seq++;
		break;
	case DEMOSEQ_EFF_ADD:
		GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG0, VISIBLE_ON );
		ListDemo_ParticleAdd(demo);
		demo->seq++;
		break;
	case DEMOSEQ_EFF_MAIN:
		//�p�[�e�B�N���ŃA�C�R�����B�ꂽ�^�C�~���O��_���ă|�P�����A�C�R���ύX
		demo->wait++;
		if(demo->wait == demo->wait_max){
			PokeListIconChange( wk, wk->pos );
		}
		
		ListDemo_ParticleMain();
		if(demo->wait > demo->wait_max && Particle_GetEmitterNum(demo->ptc) == 0){
			demo->seq++;
		}
		break;
	case DEMOSEQ_EFF_END:
		ListDemo_EffectExit(wk);
		demo->seq++;
		break;
	
	case DEMOSEQ_VOICE_PLAY:
		PokeVoicePlay_PP(pp);
		demo->seq++;
		break;
	case DEMOSEQ_VOICE_WAIT:
		if(Snd_PMVoicePlayCheck() == 0){
			demo->seq++;
		}
		break;
		
	case DEMOSEQ_END_MSG:
		{
			STRBUF * str;

			str = MSGMAN_AllocString( wk->msg_man, mes_pokelist_12_01 );
			WORDSET_RegisterPokeNickName( wk->wset, 0, PPPPointerGet(pp));
			WORDSET_ExpandStr( wk->wset, wk->msg_buf, str );
			STRBUF_Delete( str );

			PokeListLargeMsgPut( wk, PL_MSG_COMP_EXPAND, 1 );
		}
		demo->seq++;
		break;
	case DEMOSEQ_END_MSG_WAIT:
		if( GF_MSG_PrintEndCheck( wk->msg_index ) == 0 ){
			PokeList_FormDemoEnd(wk);
			wk->dat->ret_mode = PL_RET_NORMAL;
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �t�H�����`�F���W�f���I��
 *
 * @param   wk		�|�P�������X�g�̃��[�N
 */
//--------------------------------------------------------------
static void PokeList_FormDemoEnd(PLIST_WORK *wk)
{
	sys_FreeMemoryEz(wk->demo);
	wk->demo = NULL;
}

//--------------------------------------------------------------
/**
 * @brief   �f�����o�p�ɉ�ʍ\���ύX
 *
 * @param   wk		�|�P�������X�g�̃��[�N
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void ListDemo_EffectInit(PLIST_WORK *wk)
{
	//BG0�ʂ�3D�ʂ֕ύX
	PokeListBG0_Change(wk, LIST_BG0_MODE_3D);
	
	//�p�[�e�B�N���V�X�e���쐬
	ListDemo_ParticleSystemCreate(wk->demo);
	
	//Blend�ݒ�
	G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, GX_BLEND_ALL, 31, 0);
}

//--------------------------------------------------------------
/**
 * @brief   �f�����o�p�ɕύX���Ă�����ʍ\�������ɖ߂�
 *
 * @param   wk		�|�P�������X�g�̃��[�N
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static void ListDemo_EffectExit(PLIST_WORK *wk)
{
	//�p�[�e�B�N���V�X�e���j��
	ListDemo_ParticleExit(wk->demo);

	//BG0�ʂ�3D�ʂ֕ύX
	PokeListBG0_Change(wk, LIST_BG0_MODE_2D);
	
	//Blend�ݒ�
	G2_BlendNone();
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�������X�g�p�p�[�e�B�N���V�X�e���쐬
 *
 * @param   demo		�f�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void ListDemo_ParticleSystemCreate(LIST_FORMDEMO_PTR demo)
{
	void *heap;
	GF_CAMERA_PTR camera_ptr;

	Particle_SystemWorkInit();	//�p�[�e�B�N���V�X�e��������
	
	heap = sys_AllocMemory(HEAPID_POKELIST, PARTICLE_LIB_HEAP_SIZE);
	demo->ptc = Particle_SystemCreate(sAllocTex, sAllocTexPalette, heap, 
		PARTICLE_LIB_HEAP_SIZE, TRUE, HEAPID_POKELIST);
	camera_ptr = Particle_GetCameraPtr(demo->ptc);
	GFC_SetCameraClip(BP_NEAR, BP_FAR, camera_ptr);
}

//--------------------------------------------------------------
/**
 * @brief   �G�~�b�^����
 *
 * @param   demo		�f���p���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void ListDemo_ParticleAdd(LIST_FORMDEMO_PTR demo)
{
	void *resource;

	//���\�[�X�ǂݍ��݁��o�^
	resource = Particle_ArcResourceLoad(
		ARC_POKELIST_PARTICLE, demo->spa_id, HEAPID_POKELIST);
	Particle_ResourceSet(demo->ptc, resource, PTC_AUTOTEX_LNK | PTC_AUTOPLTT_LNK, TRUE);
	
	//�G�~�b�^����
	switch(demo->monsno){
	case MONSNO_KIMAIRAN:
		Particle_CreateEmitterCallback(demo->ptc, 
			DEMO_GIRATHINA_FORM_EFF01A, EmitCall_PosSet, demo);
		Particle_CreateEmitterCallback(demo->ptc, 
			DEMO_GIRATHINA_FORM_EFF01B, EmitCall_PosSet, demo);
		Particle_CreateEmitterCallback(demo->ptc, 
			DEMO_GIRATHINA_FORM_EFF01C, EmitCall_PosSet, demo);
		
		Snd_SePlay(SE_FORM_CHANGE_GIRATHINA);
		//Snd_SePlay(SE_FORM_CHANGE_GIRATHINA2);
		break;
	case MONSNO_EURISU:
		Particle_CreateEmitterCallback(demo->ptc, DEMO_SHEIMI_FORM_EFF02A, EmitCall_PosSet, demo);
		Particle_CreateEmitterCallback(demo->ptc, DEMO_SHEIMI_FORM_EFF02B, EmitCall_PosSet, demo);

		Snd_SePlay(SE_FORM_CHANGE_SHEIMI);
		break;
	}
}

//--------------------------------------------------------------
/**
 * @brief   �G�~�b�^�̍��W��ݒ肷��
 * @param   emit		���������G�~�b�^�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void EmitCall_PosSet(EMIT_PTR emit)
{
	LIST_FORMDEMO_PTR demo;
	
	demo = Particle_GetTempPtr();
	
	SPL_SetEmitterPositionX(emit, EmitPos[demo->pos][0]);
	SPL_SetEmitterPositionY(emit, EmitPos[demo->pos][1]);
//	SPL_SetEmitterPositionZ(emit, ptw->z);
}

//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e���E���C���֐�(�v�Z�E�`�揈���Ȃǂ����s)
 *
 * @retval  �`�悪�s��ꂽ��
 */
//--------------------------------------------------------------
static int ListDemo_ParticleMain(void)
{
	int draw_num;
	
	GF_G3X_Reset();

	draw_num = Particle_DrawAll();	//�p�[�e�B�N���`��

	if(draw_num > 0){
		//�p�[�e�B�N���̕`�悪�I�������̂ŁA�Ăу\�t�g�E�F�A�X�v���C�g�p�J�����ɐݒ�
		GF_G3X_Reset();
	}

	Particle_CalcAll();	//�p�[�e�B�N���v�Z

	
	GF_G3_RequestSwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
	return draw_num;
}

//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e���j��
 *
 * @param   demo		�f���p���[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void ListDemo_ParticleExit(LIST_FORMDEMO_PTR demo)
{
	void *heap;
	
	heap = Particle_HeapPtrGet(demo->ptc);
	Particle_SystemExit(demo->ptc);
	sys_FreeMemoryEz(heap);
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`��VRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param   size		�e�N�X�`���T�C�Y
 * @param   is4x4comp	4x4���k�e�N�X�`���ł��邩�ǂ����̃t���O(TRUE=���k�e�N�X�`��)
 *
 * @retval  �ǂݍ��݂��J�n����VRAM�̃A�h���X
 */
//--------------------------------------------------------------
static u32 sAllocTex(u32 size, BOOL is4x4comp)
{
	NNSGfdTexKey key;
	
	key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
	GF_ASSERT(key != NNS_GFD_ALLOC_ERROR_TEXKEY);
	Particle_LnkTexKeySet(key);		//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	
	return NNS_GfdGetTexKeyAddr(key);
}

//--------------------------------------------------------------
/**
 * @brief	�e�N�X�`���p���b�gVRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param	size		�e�N�X�`���T�C�Y
 * @param	is4pltt		4�F�p���b�g�ł��邩�ǂ����̃t���O
 *
 * @retval	�ǂݍ��݂��J�n����VRAM�̃A�h���X
 *
 * direct�`���̃e�N�X�`���̏ꍇ�ASPL_LoadTexPlttByCallbackFunction��
 * �R�[���o�b�N�֐����Ăяo���܂���B
 */
//--------------------------------------------------------------
static u32 sAllocTexPalette(u32 size, BOOL is4pltt)
{
	NNSGfdPlttKey key;
	
	key = NNS_GfdAllocPlttVram(size, is4pltt, NNS_GFD_ALLOC_FROM_LOW);
	GF_ASSERT(key != NNS_GFD_ALLOC_ERROR_PLTTKEY);
	
	Particle_PlttLnkTexKeySet(key);	//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	
	return NNS_GfdGetPlttKeyAddr(key);
}
