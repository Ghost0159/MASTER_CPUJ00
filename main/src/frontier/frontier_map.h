//==============================================================================
/**
 * @file	frontier_map.h
 * @brief	�t�����e�B�A�}�b�v�̃w�b�_
 * @author	matsuda
 * @date	2007.04.04(��)
 */
//==============================================================================
#ifndef __FRONTIER_MAP_H__
#define __FRONTIER_MAP_H__

#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_judge.h"
#include "application/wifi_2dmap/wf2dmap_scrdraw.h"
#include "application/wifi_2dmap/wf2dmap_objdraw.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"

#include "system/clact_tool.h"
#include "frontier_particle.h"		//FRP_PTR��`�̂���
#include "frontier_main.h"			//FSS_CHAR_RESOURCE_DATA��`�̂���

#include "fs_usescript.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�t�����e�B�A�}�b�v�Ŋm�ۂ���q�[�v�T�C�Y
#define FRONTIERMAP_ALLOC_SIZE		(0x90000)

///�t�B�[���h��ŏo����OBJ�ő吔
#define FIELD_OBJ_MAX				(32)
///�L�����N�^���\�[�X�A�ő�Ǘ���
#define FIELD_OBJ_RESOURCE_MAX		(24)

///�}�b�vX�����O���b�h��
#define MAP_GRID_MAX_X				(256 / 16)
///�}�b�vY�����O���b�h��
#define MAP_GRID_MAX_Y				(256 / 16)

///�����ȃv���CID
#define FIELD_PLAYID_NULL			(0xffff)

///article_resource_id�Ŏg�p���Ă��Ȃ��ꍇ�̒l
#define SCR_ACTWORK_RESOURCE_ID_NULL	(0xffff)

//--------------------------------------------------------------
//	OBJWK�̃V�X�e�����ł��炩���ߊm�ۂ���ʒu
//--------------------------------------------------------------
///�v���C���[(���@)��objwk�g�p�ʒu
#define FIELD_OBJ_PLAYER		(FIELD_OBJ_MAX - 1)
///�ʐM�����objwk�g�p�ʒu
#define FIELD_OBJ_SIO_PLAYER	(FIELD_OBJ_MAX - 2)

//--------------------------------------------------------------
//	�t���[���ԍ�
//--------------------------------------------------------------
///�E�B���h�E�E���b�Z�[�W�ʂ̃t���[��
#define FRMAP_FRAME_WIN					(GF_BGL_FRAME1_M)
///�G�t�F�N�g��(or ���d��)�̃t���[��
#define FRMAP_FRAME_EFF					(GF_BGL_FRAME2_M)
///�w�i�ʂ̃t���[��
#define FRMAP_FRAME_MAP					(GF_BGL_FRAME3_M)

///�T�u��ʁF�ϋq�ʂ̃t���[��
#define FRMAP_FRAME_SUB_AUDIENCE		(GF_BGL_FRAME0_S)

///BG�ԍ��F�E�B���h�E
#define FRMAP_BGNO_WIN			(1)
///BG�ԍ��F�G�t�F�N�g
#define FRMAP_BGNO_EFF			(2)
///BG�ԍ��F�w�i
#define FRMAP_BGNO_BACKGROUND	(3)

///GX_WND�ԍ��F�G�t�F�N�g
#define FRMAP_GX_WND_EFF			(GX_WND_PLANEMASK_BG2)

///<3D�ʂ�BG�v���C�I���e�B
#define FRMAP_3DBG_PRIORITY		(0)
///�E�B���h�E�ʂ�BG�v���C�I���e�B
#define FRMAP_BGPRI_WIN			(0)
///�G�t�F�N�g�ʂ�BG�v���C�I���e�B
#define FRMAP_BGPRI_EFF			(1)
///�w�i�ʂ�BG�v���C�I���e�B
#define FRMAP_BGPRI_MAP			(3)

///OBJ�L�����N�^�[��BG�v���C�I���e�B
#define FRMAP_BGPRI_CHARACTER	(2)

///<�T�u��ʁF�w�i�ʂ�BG�v���C�I���e�B
///�ϋq�ʂ�BG�v���C�I���e�B
#define FRMAP_BGPRI_SUB_AUDIENCE	(3)


//--------------------------------------------------------------
//	�A�N�^�[
//--------------------------------------------------------------
///�T�u��ʂ̃T�[�t�F�[�X�ʒu
#define FMAP_SUB_ACTOR_DISTANCE		(512 * FX32_ONE)

///���C��	OAM�Ǘ��̈�E�J�n
#define FMAP_OAM_START_MAIN			(0)
///���C��	OAM�Ǘ��̈�E�I��
#define FMAP_OAM_END_MAIN				(128)
///���C��	�A�t�B���Ǘ��̈�E�J�n
#define FMAP_OAM_AFFINE_START_MAIN		(0)
///���C��	�A�t�B���Ǘ��̈�E�I��
#define FMAP_OAM_AFFINE_END_MAIN		(32)
///�T�u	OAM�Ǘ��̈�E�J�n
#define FMAP_OAM_START_SUB				(0)
///�T�u	OAM�Ǘ��̈�E�I��
#define FMAP_OAM_END_SUB				(128)
///�T�u �A�t�B���Ǘ��̈�E�J�n
#define FMAP_OAM_AFFINE_START_SUB		(0)
///�T�u	�A�t�B���Ǘ��̈�E�I��
#define FMAP_OAM_AFFINE_END_SUB		(32)

///�L�����}�l�[�W���F�L�����N�^ID�Ǘ���(���ʁ{�����)
#define FMAP_CHAR_MAX					(96)
///�L�����}�l�[�W���F���C����ʃT�C�Y(byte�P��)
#define FMAP_CHAR_VRAMSIZE_MAIN			(0x10000)	//(1024 * 0x40)	//64K
///�L�����}�l�[�W���F�T�u��ʃT�C�Y(byte�P��)
#define FMAP_CHAR_VRAMSIZE_SUB			(512 * 0x20)	//32K

///���C����ʁ{�T�u��ʂŎg�p����A�N�^�[����
#define FMAP_ACTOR_MAX					(64 + 64)	//���C����� + �T�u���
///OBJ�Ŏg�p����p���b�g�{��(���ʁ{�����)
#define FMAP_OAM_PLTT_MAX				(16 + 16)

///OAM���\�[�X�F�L�����o�^�ő吔(���C����� + �T�u���)
#define FMAP_OAMRESOURCE_CHAR_MAX		(FMAP_CHAR_MAX)
///OAM���\�[�X�F�p���b�g�o�^�ő吔(���C����� + �T�u���)
#define FMAP_OAMRESOURCE_PLTT_MAX		(FMAP_OAM_PLTT_MAX)
///OAM���\�[�X�F�Z���o�^�ő吔
#define FMAP_OAMRESOURCE_CELL_MAX		(64)
///OAM���\�[�X�F�Z���A�j���o�^�ő吔
#define FMAP_OAMRESOURCE_CELLANM_MAX	(64)
///OAM���\�[�X�F�}���`�Z���o�^�ő吔
#define FMAP_OAMRESOURCE_MCELL_MAX		(8)
///OAM���\�[�X�F�}���`�Z���A�j���o�^�ő吔
#define FMAP_OAMRESOURCE_MCELLANM_MAX	(8)


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///FMap_ActorSet�֐��̈����F�󂫗̈���������ēo�^
#define FMAP_ACT_POS_AUTO		(-1)


//==============================================================================
//	�\���̒�`
//==============================================================================
///�z�u���A�N�^�[�\����
typedef struct{
	CATS_ACT_PTR act[ACTWORK_MAX];///<�X�N���v�g��Ő������ĊǗ�����ꍇ�̃|�C���^�ۑ��ꏊ
	u16 act_id[ACTWORK_MAX];	///<�X�N���v�g��Ő������ĊǗ�����ꍇ��act_id�ۑ��ꏊ
	u32 anmbit;				///<�A�j������̗L��(1�r�b�g�P�ʂŊǗ�)
	u16 resource_id[ACTWORK_RESOURCE_MAX];	///<�X�N���v�g��Ő��������A�N�^�[���\�[�X��ID�ۑ��ꏊ
}FMAP_ARTICLE_ACT;

///�n�k����\����
typedef struct{
	TCB_PTR tcb;				///<�n�k������s�^�X�N
	s16 shake_x;				///<�h��X
	s16 shake_y;				///<�h��Y
	s8 wait;					///<�n�k�҂�
	s8 wait_max;				///<�E�F�C�g�ő�l
	s8 loop;					///<�n�k��
	
	u8 padding;
}FMAP_SHAKE_WORK;

///WINDOW����\����
typedef struct{
	TCB_PTR tcb;
	u8 x1;
	u8 y1;
	u8 x2;
	u8 y2;
	s16 wait;
	s8 on_off;
	u8 padding;
}FMAP_WND_WORK;

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�}�b�v�V�X�e�����[�N
 */
//--------------------------------------------------------------
typedef struct _FRONTIERMAP_PROC_WORK{
	//-- �V�X�e���n --//
	GF_BGL_INI *bgl;
	PALETTE_FADE_PTR pfd;			///<�p���b�g�t�F�[�h�V�X�e���ւ̃|�C���^
	FMAIN_PTR fmain;
	GF_G3DMAN *g3Dman;
	FRP_PTR frp;					///<�t�����e�B�A�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
	
	//2D�}�b�v�V�X�e�����W���[��
	WF2DMAP_OBJSYS *objsys;		///<�I�u�W�F�N�g�Ǘ����W���[��
//	WF2DMAP_OBJWK *objwk[FIELD_OBJ_MAX];	///<�I�u�W�F�N�g���[�N
	//�}�b�v�f�[�^�Ǘ����W���[��
	WF2DMAP_MAPSYS *mapsys;		///<�}�b�v�f�[�^�Ǘ����W���[��
	//�X�N���[���f�[�^�Ǘ����W���[��
	WF2DMAP_SCROLL scrollsys;	///<�X�N���[���f�[�^�Ǘ����W���[��(�����ꂾ������)
	//�I�u�W�F�N�g�f�[�^�`�惂�W���[��
	WF2DMAP_OBJDRAWSYS *drawsys;	///<�I�u�W�F�N�g�f�[�^�`�惂�W���[��
//	WF2DMAP_OBJDRAWWK *drawwk[FIELD_OBJ_MAX];	///<�`��I�u�W�F�N�g���[�N
	//�X�N���[���f�[�^�`�惂�W���[��
	WF2DMAP_SCRDRAW *scrdrawsys;		///<�X�N���[���f�[�^�`�惂�W���[��
	WF2DMAP_SCRDRAW *multi_scrdrawsys;	///<�X�N���[���f�[�^�`�惂�W���[��(���d�ʗp)
	//�R�}���h�L���[
	WF2DMAP_REQCMDQ *reqcmd_q;		///<���N�G�X�g�R�}���h
	WF2DMAP_ACTCMDQ *actcmd_q;		///<�A�N�V�����R�}���h
	
	//�Z���A�N�^�[
	struct{
		CATS_SYS_PTR		csp;
		CATS_RES_PTR		crp;
	}clactsys;

	//�z�u���A�N�^�[
	FMAP_ARTICLE_ACT article;		///<�z�u���A�N�^�[
	
	//�|�P�����A�N�^�[
	CATS_ACT_PTR poke_cap[POKE_DISP_MANAGER_ID_NUM];
	
	//�V�[���f�[�^�̏�������֐�����Alloc�����ꍇ�̃|�C���^�ێ��̈�
	void *scene_func_work_ptr;
	
	//-- ���[�J�� --//
	TCB_PTR objupdate_tcb;			///<�I�u�W�F�N�g�V�X�e���X�V����
	TCB_PTR cmdjudgeupdate_tcb;		///<�R�}���h���f���s����
	TCB_PTR update_tcb;				///<�V�X�e���X�V����
	TCB_PTR vintr_tcb;				///<V�u�����N���荞�݃^�X�N
	
	FMAP_SHAKE_WORK shake_work;		///<�n�k����\����
	FMAP_WND_WORK wnd_work;			///<Window����\����
	
	u16 seq;
	u16 tbl_seq_no;					///<���C���V�[�P���X�e�[�u���̔ԍ�
	u8 proc_mode;					///<���݂̓����
	u8 func_scene_id;				///<�V�[�����Ɏ��s����鏉��(�I��)����֐��̃V�[��ID

	u8 main_end;					///<TRUE:���C�������I��
}FRONTIERMAP_WORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern FMAP_PTR FrontierMap_Init(FMAIN_PTR fmain);
extern BOOL FrontierMap_Main(FMAP_PTR fmap);
extern void FrontierMap_End(FMAP_PTR fmap);
extern void FrontierMap_Push(FMAP_PTR fmap);
extern void FrontierMap_Pop(FMAP_PTR fmap);

extern void FMap_CharResourceSet(FMAP_PTR fmap, const FSS_CHAR_RESOURCE_DATA *res);
extern void FMap_CharResourceFree(FMAP_PTR fmap, int charid);
extern WF2DMAP_OBJWK * FMap_ActorSet(FMAP_PTR fmap, const FSS_CHAR_ACTOR_DATA *res, int auto_pos);
extern void FMap_ActorFree(FMAP_PTR fmap, WF2DMAP_OBJWK *objwk);
extern void FMap_OBJWkGet(FMAP_PTR fmap, u16 playid, 
	WF2DMAP_OBJWK **objwk, WF2DMAP_OBJDRAWWK **drawwk);

extern void FMap_ArticleActResourceIDSet(FMAP_PTR fmap, u16 act_id);
extern void FMap_ArticleActResourceIDDel(FMAP_PTR fmap, u16 act_id);
extern CATS_ACT_PTR FMap_ArticleActCreate(FMAP_PTR fmap, u16 work_no, u16 act_id);
extern void FMap_ArticleActDel(FMAP_PTR fmap, u16 work_no);
extern CATS_ACT_PTR FMap_ArticleActGet(FMAP_PTR fmap, u16 work_no);
extern void FMap_ArticleActAnmBitSet(FMAP_PTR fmap, u16 work_no, int anm_start);
extern u32 FMap_ArticleActAnmBitGet(FMAP_PTR fmap, u16 work_no);
extern void FMap_ScrollPosGet(FMAP_PTR fmap, s16 *ret_x, s16 *ret_y);

extern FSS_ACTOR_WORK * Frontier_ActorWorkSearch(FMAIN_PTR fmain, int playid);


#endif	//__FRONTIER_MAP_H__
