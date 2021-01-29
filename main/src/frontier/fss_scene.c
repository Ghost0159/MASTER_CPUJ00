//==============================================================================
/**
 * @file	fss_scene.c
 * @brief	�t�����e�B�A�p�X�N���v�g�F�V�[���f�[�^
 * @author	matsuda
 * @date	2007.04.03(��)
 */
//==============================================================================
#include "common.h"
#include "system/snd_tool.h"
#include "frontier_types.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"

#include "msgdata/msg.naix"
#include "script/fr_script.naix"

#include "graphic/frontier_bg_def.h"
#include "system/arc_tool.h"

#include "stage_bganm.h"

#include "frontier_tool.h"
#include "castle_tool.h"
#include "factory_tool.h"
#include "wifi_counter_tool.h"


//==============================================================================
//	�^��`
//==============================================================================
///�V�[���f�[�^�̏�������֐��E�I������֐��̌^
typedef void (*FSS_SCENE_FUNC)(FMAP_PTR, void **);


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void TestInitFunc(FMAP_PTR fmap, void **ptr_work);
static void TestEndFunc(FMAP_PTR fmap, void **ptr_work);
static void InitFunc_Stage(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_Stage(FMAP_PTR fmap, void **ptr_work);
static void InitFunc_RouletteWay(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_RouletteWay(FMAP_PTR fmap, void **ptr_work);
static void InitFunc_Roulette(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_Roulette(FMAP_PTR fmap, void **ptr_work);
static void InitFunc_Factory(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_Factory(FMAP_PTR fmap, void **ptr_work);
static void InitFunc_FactoryBtl(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_FactoryBtl(FMAP_PTR fmap, void **ptr_work);
static void InitFunc_WifiCounter(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_WifiCounter( FMAP_PTR fmap, void** ptr_work );
static void InitFunc_CastleBtl(FMAP_PTR fmap, void **ptr_work);
static void EndFunc_CastleBtl(FMAP_PTR fmap, void **ptr_work);


//==============================================================================
//	�\���̒�`
//==============================================================================
///�V�[���f�[�^�\����
typedef struct{
	int bg_mode;		///<BG���[�h(�e�L�X�gBG�A�g��BG�̐ݒ�Ȃ�)
	
	FSS_SCENE_FUNC init_func;	///<��������֐�
	FSS_SCENE_FUNC end_func;	///<�I������֐�
	
	u16 scr_id;			///<�X�N���v�gID
	u16 msg_id;			///<���b�Z�[�WID
	u16 bgm_id;			///<BGM ID(�ǂݍ��މ��F��"SND_SCENE_FIELD"�Œ�ɂȂ��Ă��܂�)
	u16 screen_size;	///<�X�N���[���T�C�Y(GF_BGL_SCRSIZ_???)
	
	u16 map_arc_id;		///<�}�b�v�̃A�[�J�C�uID
	u16 arc_screen_id;	///<�}�b�v�X�N���[����ID
	u16 arc_char_id;	///<�}�b�v�L�����N�^��ID
	u16 arc_pltt_id;	///<�}�b�v�p���b�g��ID

	u16 arc_multi_screen_id;	///<���d�ʃ}�b�v�X�N���[����ID(�p���b�g��arc_pltt_id�Ƌ���)
	u16 arc_multi_char_id;		///<���d�ʃ}�b�v�L�����N�^��ID
	u16 arc_multi_pltt_id;		///<���d�ʃ}�b�v�p���b�g��ID

	u8 scroll_mode;		///<SCROLL_MODE_???
	u8 multiple_scroll_mode;	///<���d�ʂ̃X�N���[����ʏ�ʂƓ��������邩(TRUE:����, FALSE:�V�X�e�����ł͑��d�ʃX�N���[���͂�����Ȃ�)
	
	u8 dummy[2];		///<4�o�C�g���E�_�~�[
}FSS_SCENE_DATA;


//==============================================================================
//	�f�[�^(GX_BGMODE_5,SCROLL_MODE_EASY)
//==============================================================================
//�V�[���f�[�^�e�[�u��
static const FSS_SCENE_DATA SceneDataTbl[] = {
	{//FSS_SCENEID_TESTSCR
		GX_BGMODE_0,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		TestInitFunc,							//��������֐�
		TestEndFunc,							//�I������֐�
		NARC_fr_script_testscr_bin,		//�X�N���v�gID
		NARC_msg_cmsg_acting_dat,		//���b�Z�[�WID
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_512x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BF_ROOM_BATTLE_NSCR_BIN,		//�}�b�v�X�N���[����ID
		BF_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BF_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		BF_ROOM_RAIL_NSCR_BIN,			//���d�ʂ̃X�N���[��ID
		BF_ROOM_NCGR_BIN,				//���d�ʂ̃L�����N�^ID
		BF_ROOM_NCLR,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_NORMAL,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_TESTSCR2		TESTSCR����}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_0,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		0,		//�X�N���v�gID	TESTSCR�Ƌ���
		NARC_msg_cmsg_acting_dat,		//���b�Z�[�WID
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_512x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BF_ROOM_EXCHANGE_NSCR_BIN,		//�}�b�v�X�N���[����ID
		BF_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BF_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_NORMAL,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_TESTSCR3		TESTSCR����MAP_CHANGE_EX�Ő؂�ւ��Ă���}�b�v
		GX_BGMODE_0,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_testscr3_bin,	//�X�N���v�gID(EX�Ȃ̂ł��ꂪ�ǂݍ��܂��)
		NARC_msg_cmsg_dance_dat,		//���b�Z�[�WID
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_512x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BF_ROOM_BATTLE_NSCR_BIN,		//�}�b�v�X�N���[����ID
		BF_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BF_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_NORMAL,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	{//FSS_SCENEID_FACTORY
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_Factory,				//��������֐�
		EndFunc_Factory,				//�I������֐�
		NARC_fr_script_factory_bin,		//�X�N���v�gID
		NARC_msg_factory_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_FACTORY,				//BGM ID
		GF_BGL_SCRSIZ_512x512,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BF_ROOM_EXCHANGE_NSCR_BIN,		//�}�b�v�X�N���[����ID
		BF_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BF_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		BF_ROOM_RAIL_NSCR_BIN,			//���d�ʂ̃X�N���[��ID
		BF_ROOM_NCGR_BIN,				//���d�ʂ̃L�����N�^ID
		BF_ROOM_NCLR,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		//TRUE,							//���d�ʂ̃X�N���[������
		FALSE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_FACTORY_BTL	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_FactoryBtl,			//��������֐�
		EndFunc_FactoryBtl,				//�I������֐�
		NARC_fr_script_factory_bin,		//�X�N���v�gID
		NARC_msg_factory_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_FACTORY,				//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BF_ROOM_BATTLE_NSCR_BIN,		//�}�b�v�X�N���[����ID
		BF_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BF_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	{//FSS_SCENEID_TOWER_SINGLE_WAY	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_tower3_bin,		//�X�N���v�gID
		NARC_msg_d31r0201_dat,			//���b�Z�[�WID(r0203�͑��݂��Ȃ�)
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_512x512,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BT_WAY_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BT_NCGR_BIN,					//�}�b�v�L�����N�^��ID
		BT_NCLR,						//�}�b�v�p���b�g��ID
		BT_WAY_A_NSCR_BIN,				//���d�ʂ̃X�N���[��ID
		BT_NCGR_BIN,					//���d�ʂ̃L�����N�^ID
		BT_NCLR,						//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_TOWER_MULTI_WAY	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_tower4_bin,		//�X�N���v�gID
		NARC_msg_d31r0201_dat,			//���b�Z�[�WID(r0204�͑��݂��Ȃ�)
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_512x512,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BT_MULWAY_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BT_NCGR_BIN,					//�}�b�v�L�����N�^��ID
		BT_NCLR,						//�}�b�v�p���b�g��ID
		BT_MULWAY_A_NSCR_BIN,			//���d�ʂ̃X�N���[��ID
		BT_NCGR_BIN,					//���d�ʂ̃L�����N�^ID
		BT_NCLR,						//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_TOWER_SINGLE_BTL	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_tower5_bin,		//�X�N���v�gID
		NARC_msg_d31r0205_dat,			//���b�Z�[�WID
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BT_SBATTLE_NSCR_BIN,			//�}�b�v�X�N���[����ID
		BT_NCGR_BIN,					//�}�b�v�L�����N�^��ID
		BT_NCLR,						//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_TOWER_MULTI_BTL	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_tower6_bin,		//�X�N���v�gID
		NARC_msg_d31r0206_dat,			//���b�Z�[�WID
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BT_MULBATTLE_NSCR_BIN,			//�}�b�v�X�N���[����ID
		BT_NCGR_BIN,					//�}�b�v�L�����N�^��ID
		BT_NCLR,						//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	{//FSS_SCENEID_STAGE_WAY
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_stage_bin,		//�X�N���v�gID
		NARC_msg_stage_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_STAGE,				//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BS_STAGE_ROUKA_NSCR_BIN,		//�}�b�v�X�N���[����ID
		BS_STAGE_ROUKA_NCGR_BIN,		//�}�b�v�L�����N�^��ID
		BS_STAGE_ROUKA_NCLR,			//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_STAGE_BTL
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_Stage,							//��������֐�
		EndFunc_Stage,							//�I������֐�
		NARC_fr_script_stage_bin,		//�X�N���v�gID
		NARC_msg_stage_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_STAGE,				//BGM ID
		GF_BGL_SCRSIZ_512x512,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BS_STAGE_01_A_NSCR_BIN,			//�}�b�v�X�N���[����ID
		BS_STAGE_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BS_STAGE_NCLR,					//�}�b�v�p���b�g��ID
		BS_STAGE_ALPHA_A_NSCR_BIN,			//���d�ʂ̃X�N���[��ID
		BS_STAGE_ALPHA_NCGR_BIN,				//���d�ʂ̃L�����N�^ID
		BS_STAGE_ALPHA_NCLR,			//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	{//FSS_SCENEID_CASTLE
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_castle_bin,		//�X�N���v�gID
		NARC_msg_castle_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_CASTLE,				//BGM ID
		GF_BGL_SCRSIZ_512x512,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BC_OHMA_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BC_OHMA_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BC_OHMA_NCLR,					//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_CASTLE_BTL	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_CastleBtl,				//��������֐�
		EndFunc_CastleBtl,				//�I������֐�
		NARC_fr_script_castle_bin,		//�X�N���v�gID(����)
		NARC_msg_castle_room_dat,		//���b�Z�[�WID(����)
		SEQ_PL_BF_CASTLE,				//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BC_ROOM_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BC_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BC_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		BC_ROOM_A_NSCR_BIN,				//���d�ʂ̃X�N���[��ID
		BC_ROOM_NCGR_BIN,				//���d�ʂ̃L�����N�^ID
		BC_ROOM_NCLR,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_CASTLE_BYPATH	�}�b�v�؂�ւ��ŗ���}�b�v
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		NULL,							//��������֐�
		NULL,							//�I������֐�
		NARC_fr_script_castle_bin,		//�X�N���v�gID(����)
		NARC_msg_castle_room_dat,		//���b�Z�[�WID(����)
		SEQ_PL_BF_CASTLE,				//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BC_ROUKA_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BC_ROUKA_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BC_ROUKA_NCLR,					//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	{//FSS_SCENEID_WIFI_COUNTER
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_WifiCounter,			//��������֐�
		EndFunc_WifiCounter,			//�I������֐�
		NARC_fr_script_wifi_counter_bin,//�X�N���v�gID
		NARC_msg_bf_info_dat,			//���b�Z�[�WID
		SEQ_BF_TOWWER,					//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BF_CONNECT_NSCR_BIN,			//�}�b�v�X�N���[����ID
		BF_CONNECT_NCGR_BIN,			//�}�b�v�L�����N�^��ID
		BF_CONNECT_NCLR,				//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
	{//FSS_SCENEID_ROULETTE_WAY
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_RouletteWay,			//��������֐�
		EndFunc_RouletteWay,			//�I������֐�
		NARC_fr_script_roulette_bin,	//�X�N���v�gID
		NARC_msg_roulette_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_ROULETTE,				//BGM ID
		GF_BGL_SCRSIZ_256x256,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BR_ROUKA_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BR_ROUKA_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BR_ROUKA_NCLR,					//�}�b�v�p���b�g��ID
		MULTI_BG_NULL,					//���d�ʂ̃X�N���[��ID
		MULTI_BG_NULL,					//���d�ʂ̃L�����N�^ID
		MULTI_BG_NULL,					//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
	{//FSS_SCENEID_ROULETTE_BTL
		GX_BGMODE_5,	//BG���[�h(�ꎞ�I�Ȃ��́B�S�ẴO���t�B�b�N���ڍs�o���������)
		InitFunc_Roulette,				//��������֐�
		EndFunc_Roulette,				//�I������֐�
		NARC_fr_script_roulette_bin,	//�X�N���v�gID
		NARC_msg_roulette_room_dat,		//���b�Z�[�WID
		SEQ_PL_BF_ROULETTE,				//BGM ID
		GF_BGL_SCRSIZ_512x512,			//�X�N���[���T�C�Y
		ARC_FRONTIER_BG,				//�}�b�v�̃A�[�J�C�uID
		BR_ROOM_NSCR_BIN,				//�}�b�v�X�N���[����ID
		BR_ROOM_NCGR_BIN,				//�}�b�v�L�����N�^��ID
		BR_ROOM_NCLR,					//�}�b�v�p���b�g��ID
		BR_KEKKA01_HP_A_NSCR_BIN,		//���d�ʂ̃X�N���[��ID
		BR_KEKKA01_HP_NCGR_BIN,			//���d�ʂ̃L�����N�^ID
		BR_ROOM_KEKKA_NCLR,				//���d�ʂ̃p���b�gID
		SCROLL_MODE_EASY,				//�X�N���[�����[�h
		TRUE,							//���d�ʂ̃X�N���[������
	},
};


//==============================================================================
//
//	
//
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �V�[��ID����V�[���f�[�^�̊e��p�����[�^���擾����
 *
 * @param   scene_id		�V�[��ID
 * @param   param_code		�擾�������f�[�^(FSS_SCENE_DATA_???)
 *
 * @retval  �擾�f�[�^
 */
//--------------------------------------------------------------
int FSS_SceneParamGet(int scene_id, int param_code)
{
	switch(param_code){
	case FSS_SCENE_DATA_BG_MODE:
		return SceneDataTbl[scene_id].bg_mode;
	case FSS_SCENE_DATA_SCRID:
		return SceneDataTbl[scene_id].scr_id;
	case FSS_SCENE_DATA_MSGID:
		return SceneDataTbl[scene_id].msg_id;
	case FSS_SCENE_DATA_BGMID:
		return SceneDataTbl[scene_id].bgm_id;
	case FSS_SCENE_DATA_SCREEN_SIZE:
		return SceneDataTbl[scene_id].screen_size;
	case FSS_SCENE_DATA_MAP_ARCID:
		return SceneDataTbl[scene_id].map_arc_id;
	case FSS_SCENE_DATA_MAP_SCREENID:
		return SceneDataTbl[scene_id].arc_screen_id;
	case FSS_SCENE_DATA_MAP_CHARID:
		return SceneDataTbl[scene_id].arc_char_id;
	case FSS_SCENE_DATA_MAP_PLTTID:
		return SceneDataTbl[scene_id].arc_pltt_id;
	case FSS_SCENE_DATA_MULTI_MAP_SCREENID:
		return SceneDataTbl[scene_id].arc_multi_screen_id;
	case FSS_SCENE_DATA_MULTI_MAP_CHARID:
		return SceneDataTbl[scene_id].arc_multi_char_id;
	case FSS_SCENE_DATA_MULTI_MAP_PLTTID:
		return SceneDataTbl[scene_id].arc_multi_pltt_id;
	case FSS_SCENE_DATA_SCROLL_MODE:
		return SceneDataTbl[scene_id].scroll_mode;
	case FSS_SCENE_DATA_MULTI_SCROLL_MODE:
		return SceneDataTbl[scene_id].multiple_scroll_mode;
	}
	
	GF_ASSERT(0);
	return 0;
}

//--------------------------------------------------------------
/**
 * @brief   �V�[���f�[�^�ɐݒ肳��Ă��鏉������֐��Ăяo��
 *
 * @param   fmap			
 * @param   ptr_work		��������֐�����Alloc�����ꍇ�̃|�C���^�ۑ��ꏊ
 * @param   scene_id		�V�[��ID
 */
//--------------------------------------------------------------
void FSS_SceneInitFuncCall(FMAP_PTR fmap, void **ptr_work, int scene_id)
{
	if(SceneDataTbl[scene_id].init_func != NULL){
		SceneDataTbl[scene_id].init_func(fmap, ptr_work);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �V�[���f�[�^�ɐݒ肳��Ă���I������֐��Ăяo��
 *
 * @param   fmap			
 * @param   ptr_work		��������֐����Ń|�C���^�Ȃǂ��Z�b�g�����ꍇ�A���̃��[�N�ɓ����Ă���
 * @param   scene_id		�V�[��ID
 */
//--------------------------------------------------------------
void FSS_SceneEndFuncCall(FMAP_PTR fmap, void **ptr_work, int scene_id)
{
	if(SceneDataTbl[scene_id].end_func != NULL){
		SceneDataTbl[scene_id].end_func(fmap, ptr_work);
	}
}



//==============================================================================
//	��������֐��E�I������֐�
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ��������֐��̎��s�e�X�g
 *
 * @param   fmap			
 * @param   ptr_work		��������֐����Ń������m�ۂ����ꍇ�̃|�C���^�ێ��̈�
 */
//--------------------------------------------------------------
static void TestInitFunc(FMAP_PTR fmap, void **ptr_work)
{
	OS_TPrintf("��������֐����Ă΂ꂽ\n");
	
	//�������m�ۂ��K�v�ȏꍇ�͂����Ŋm�ۂ��A�|�C���^���V�X�e�������p�ӂ��Ă���̈�ɕۑ������܂�
	//�q�[�vID�͊�{�I�ɂ�HEAPID_FRONTIERMAP���g�p�B
	*ptr_work = sys_AllocMemory(HEAPID_FRONTIERMAP, 32);
}

//--------------------------------------------------------------
/**
 * @brief   �I������֐��̎��s�e�X�g
 *
 * @param   fmap			
 * @param   ptr_work		��������֐����Ń������m�ۂ����ꍇ�̃|�C���^�ێ��̈�
 */
//--------------------------------------------------------------
static void TestEndFunc(FMAP_PTR fmap, void **ptr_work)
{
	OS_TPrintf("�I������֐����Ă΂ꂽ\n");

	//InitFunc�Ŋm�ۂ��Ďg�p���Ă��������������̃^�C�~���O�ŉ�������邱�Ƃ��o���܂�
	sys_FreeMemoryEz(*ptr_work);
}

//==============================================================================
//	�X�e�[�W�F��������֐�
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 */
//--------------------------------------------------------------
static void InitFunc_Stage(FMAP_PTR fmap, void **ptr_work)
{
	//BG�A�j������V�X�e���쐬
	*ptr_work = StageAnimation_Init(fmap->bgl, fmap->pfd);

	GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 1 );
	GF_BGL_PrioritySet( FRMAP_FRAME_EFF, 2 );
	GF_BGL_PrioritySet( FRMAP_FRAME_MAP, 3 );

	//GF_Disp_GX_VisibleControl( FRMAP_FRAME_EFF, VISIBLE_OFF );	//BG��\��
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���X�e�[�W�F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 */
//--------------------------------------------------------------
static void EndFunc_Stage(FMAP_PTR fmap, void **ptr_work)
{
	//BG�A�j������V�X�e�����
	StageAnimation_End(*ptr_work);
	return;
}

//==============================================================================
//	���[���b�g�ʘH�F��������֐�
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[���b�g�ʘH�F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * ROULETTE_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void InitFunc_RouletteWay(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n���[���b�g�ʘH�@��������@�J�n\n" );

#if 0
	PaletteFadeReq( fmap->pfd, 0xffff, 0xffff, 0, 
	//PaletteFadeReq( fmap->pfd, PF_BIT_MAIN_BG, 0xffff, 0, 
					0,									//�J�n�Z�x
					8,									//�ŏI�Z�x 
					0x0000 );							//�ύX��̐F
#endif
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[���b�g�ʘH�F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * ROULETTE_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void EndFunc_RouletteWay(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n���[���b�g�ʘH�@��������@�I��\n" );
	return;
}

//==============================================================================
//	���[���b�g�F��������֐�
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[���b�g�F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * ROULETTE_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void InitFunc_Roulette(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n���[���b�g�@��������@�J�n\n" );

#if 0
	PaletteFadeReq( fmap->pfd, 0xffff, 0xffff, 0, 
	//PaletteFadeReq( fmap->pfd, PF_BIT_MAIN_BG, 0xffff, 0, 
					0,									//�J�n�Z�x
					8,									//�ŏI�Z�x 
					0x0000 );							//�ύX��̐F
#endif

	GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 1 );
	//GF_BGL_PrioritySet( FRMAP_FRAME_WIN, 0 );				//���b�Z�[�W���őO��
	GF_BGL_PrioritySet( FRMAP_FRAME_EFF, 2 );
	GF_BGL_PrioritySet( FRMAP_FRAME_MAP, 3 );

#if 1
	G2_SetBlendAlpha(	GX_BLEND_PLANEMASK_BG0, 
						GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | 
						GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 
						20, 20 );	//�����̒l�̓p�[�e�B�N���f�[�^���̒l�����f����̂ňӖ��Ȃ�
#else
	G2_SetBlendAlpha(	GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_OBJ,
						GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | 
						GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 
						20, 20 );	//�����̒l�̓p�[�e�B�N���f�[�^���̒l�����f����̂ňӖ��Ȃ�
#endif

	//GF_Disp_GX_VisibleControl( FRMAP_FRAME_EFF, VISIBLE_OFF );	//BG��\��
	GF_Disp_GX_VisibleControl( GX_PLANEMASK_BG2, VISIBLE_OFF );		//BG��\��
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g�����[���b�g�F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * ROULETTE_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void EndFunc_Roulette(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n���[���b�g�@��������@�I��\n" );
	return;
}


//==============================================================================
//	�t�@�N�g���[�F��������֐�
//==============================================================================

//--------------------------------------------------------------
/**
 * @brief   �o�g���t�@�N�g���[�F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * FACTORY_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void InitFunc_Factory(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n�t�@�N�g���[�@��������@�J�n\n" );
	GF_BGL_PrioritySet( FRMAP_FRAME_EFF, 3 );
	GF_BGL_PrioritySet( FRMAP_FRAME_MAP, 2 );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���t�@�N�g���[�F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * FACTORY_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void EndFunc_Factory(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n�t�@�N�g���[�@��������@�I��\n" );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���t�@�N�g���[�ΐ핔���F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * FACTORY_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void InitFunc_FactoryBtl(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n�t�@�N�g���[�@��������@�J�n\n" );
#if 0
	PaletteFadeWorkAllocSet( fmap->pfd, FADE_MAIN_BG_EX3, 2*6, HEAPID_FRONTIERMAP );
	SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 0, 1, 16, 0x0000 );
	SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 1, 1, 16, 0x3d6b );
	SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 2, 1, 16, 0x3d8c );
	SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 3, 1, 16, 0x418c );
	SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 4, 1, 16, 0x3d6b );
	SoftFadePfd( fmap->pfd, FADE_MAIN_BG_EX3, 5, 1, 16, 0x6f37 );	//�����܂ŕK�v
#endif

	G2_SetBlendAlpha(	GX_BLEND_PLANEMASK_BG0, 
						GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | 
						GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 
						20, 20 );	//�����̒l�̓p�[�e�B�N���f�[�^���̒l�����f����̂ňӖ��Ȃ�

	return;
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���t�@�N�g���[�ΐ핔���F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 *
 * FACTORY_SCRWORK�͂܂��m�ۂ���Ă��Ȃ��̂ŃA�N�Z�X���Ă̓_���I
 */
//--------------------------------------------------------------
static void EndFunc_FactoryBtl(FMAP_PTR fmap, void **ptr_work)
{
	OS_Printf( "\n***********************\n�t�@�N�g���[�@��������@�I��\n" );
#if 0
	PaletteFadeWorkAllocFree( fmap->pfd, FADE_MAIN_BG_EX3 );
#endif
	return;
}


//==============================================================================
//	wifi�J�E���^�[�F��������֐�
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   wifi�J�E���^�[�F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 */
//--------------------------------------------------------------
static void InitFunc_WifiCounter(FMAP_PTR fmap, void **ptr_work)
{
	*ptr_work = WifiCounterMonitor_Init( fmap->bgl );
	return;
}

//--------------------------------------------------------------
/**
 * @brief   wifi�J�E���^�[�F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 */
//--------------------------------------------------------------
static void EndFunc_WifiCounter( FMAP_PTR fmap, void** ptr_work )
{
	WifiCounterMonitor_End( *ptr_work );
	return;
}


//==============================================================================
//	�L���b�X��BTL�F��������֐�
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �L���b�X��BTL�F��������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 */
//--------------------------------------------------------------
static void InitFunc_CastleBtl(FMAP_PTR fmap, void **ptr_work)
{
	//GF_BGL_PrioritySet( FRMAP_FRAME_EFF, 0 );
	
	GF_BGL_PrioritySet( FRMAP_FRAME_WIN, 0 );				//���b�Z�[�W���őO��
#if 0
	GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 1 );				//�p�[�e�B�N��
	GF_BGL_PrioritySet( FRMAP_FRAME_EFF, 2 );
	GF_BGL_PrioritySet( FRMAP_FRAME_MAP, 3 );
#else
	GF_BGL_PrioritySet( GF_BGL_FRAME0_M, 2 );				//�p�[�e�B�N��
	GF_BGL_PrioritySet( FRMAP_FRAME_EFF, 2 );
	GF_BGL_PrioritySet( FRMAP_FRAME_MAP, 3 );
#endif

	G2_SetBlendAlpha(	GX_BLEND_PLANEMASK_BG0, 
						GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | 
						GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 
						20, 20 );	//�����̒l�̓p�[�e�B�N���f�[�^���̒l�����f����̂ňӖ��Ȃ�
	return;
}

//--------------------------------------------------------------
/**
 * @brief   �L���b�X��BTL�F�I������֐�
 *
 * @param   fmap		
 * @param   ptr_work		
 */
//--------------------------------------------------------------
static void EndFunc_CastleBtl( FMAP_PTR fmap, void** ptr_work )
{
	return;
}


