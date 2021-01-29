//==============================================================================
/**
 * @file	frontier_main.h
 * @brief	�t�����e�B�A���C�������̃w�b�_
 * @author	matsuda
 * @date	2007.04.05(��)
 */
//==============================================================================
#ifndef __FRONTIER_MAIN_H__
#define __FRONTIER_MAIN_H__

#include "savedata/config.h"
#include "system/procsys.h"
#include "system/mystatus.h"
#include "savedata/savedata_def.h"
#include "savedata/fnote_mem.h"
#include "itemtool/myitem.h"
#include "battle/battle_cursor_disp.h"

#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_judge.h"
#include "application/wifi_2dmap/wf2dmap_scrdraw.h"
#include "application/wifi_2dmap/wf2dmap_objdraw.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"

//���I�I�I
#include "field/field.h"

//==============================================================================
//	�^��`
//==============================================================================
typedef void (*FS_SUBPROC_END_FUNC)(void *);


//==============================================================================
//	�萔��`
//==============================================================================
///�}�b�v�؂�ւ����A�C�x���gID�w��Ȃ�
#define FS_MAPCHANGE_EVENTID_NULL		(0xffff)


//==============================================================================
//	�\���̒�`
//==============================================================================
///�t�����e�B�A�Ăяo�����ɊO������n�����V�X�e���֘A�f�[�^
typedef struct{
	void *syswork;				///<�A�v���P�[�V�������ŕێ����ė~�������[�N�̃|�C���^��n��
	
	const CONFIG *config;		///<�R���t�B�O�f�[�^�ւ̃|�C���^
	SAVEDATA *savedata;			///<�Z�[�u�f�[�^�ւ̃|�C���^
	
	FNOTE_DATA	*fnote_data;	///<�`���m�[�g���[�N
	BAG_CURSOR	*bag_cursor;	///<�o�b�O�̃J�[�\���f�[�^
	int	bg_id;					///<�w�iID
	int	ground_id;				///<�n�`ID
	int	zone_id;				///<�]�[��ID
	BATTLE_CURSOR_DISP	*battle_cursor;	///<�퓬�J�[�\�������\�����
	
	u8 scene_id;				///<�V�[��ID

	//���I�I�I
	FIELDSYS_WORK* fsys;		///<WiFi��t����Ă΂��@�|�P�������X�g�ɕK�v
	//POKE_PARTY 
	//SAVE
	//
}FRONTIER_EX_PARAM;


//--------------------------------------------------------------
//	
//--------------------------------------------------------------
///�L�����N�^���\�[�X�o�^�p�f�[�^�\����
typedef struct{
	u16 charid;
	u8 movetype;
}FSS_CHAR_RESOURCE_DATA;

///OBJ����p���[�N�����ėp���[�N�̍ő吔
#define FSS_OBJMOVE_WORK_MAX		(7)

///OBJ����p���[�N�\����
typedef struct{
	u8 move_id;		///<OBJ����R�[�h
	u8 seq;			///<�ėp�V�[�P���X�ԍ�
	s16 work[FSS_OBJMOVE_WORK_MAX];	///<�ėp���[�N
}FSS_OBJMOVE_WORK;

///�L�����N�^�A�N�^�[�o�^�p�f�[�^�\����
typedef struct{
	u16 charid;		///<�L�����N�^�[ID
	u16 status;		///<���������(WF2DMAP_OBJST)
	u16 playid;		///<�v���C���[����ID
	s16 x;			///<X���W
	s16 y;			///<Y���W
	u8 way;			///<����(WF2DMAP_WAY)

	u8 visible;		///<TRUE:�\���AFALSE:��\��
	u8 event_id;	///<�Ή�����C�x���gID
	
	//OBJ����R�[�h�p���[�N
	FSS_OBJMOVE_WORK objmove;	///<OBJ����R�[�h�p���[�N

}FSS_CHAR_ACTOR_DATA;

///�A�N�^�[��OBJ�p�����[�^
///(����FSS_CHAR_ACTOR_DATA�Ɠ����B�ǂ��炩��ύX����ꍇ�́A
/// Frontier_ActWork_to_ActData�֐��AFrontier_ActWorkDataSet�֐���
/// �ύX�Ȃǂɂ����ӂ��邱��)
typedef FSS_CHAR_ACTOR_DATA FSS_OBJPARAM;

///�A�N�^�[����p���[�N(PUSH���̏��ޔ��Ƃ��Ă��g�p�����
typedef struct{
	//param.playid������������̂Ŏ��K�v�͂Ȃ�����ǁA�T�[�`���Ȃ�����̂Ŏ������Ă܂�
	//objwk��NULL���ǂ����Ŏg�p�E���g�p����ɂ��g�p���Ă��܂�
	WF2DMAP_OBJWK *objwk;	///<�I�u�W�F�N�g���[�N
	WF2DMAP_OBJDRAWWK *drawwk;	///<�`��I�u�W�F�N�g���[�N

	FSS_OBJPARAM param;			///<OBJ�p�����[�^
	FSS_OBJMOVE_WORK move;		///<OBJ���샏�[�N
	TCB_PTR anime_tcb;	///<�X�N���v�g�A�j�����s���̏ꍇ�A����TCB�̃|�C���^������
}FSS_ACTOR_WORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern FRONTIER_EX_PARAM * Frontier_ExParamGet(FMAIN_PTR fmain);
extern FMAP_PTR Frontier_FMapAdrsGet(FMAIN_PTR fmain);
extern FSS_PTR Frontier_FSSAdrsGet(FMAIN_PTR fmain);
extern void Frontier_SubProcSet(FMAIN_PTR fmain, const PROC_DATA * proc_data, void * parent_work, int auto_free, FS_SUBPROC_END_FUNC end_func);
extern void Frontier_FinishReq(FMAIN_PTR fmain);
extern void * Frontier_SysWorkGet(FMAIN_PTR fmain);
extern void Frontier_SysWorkSet(FMAIN_PTR fmain, void *syswork);
extern FSS_CHAR_RESOURCE_DATA * Frontier_CharResourceBufferGet(FMAIN_PTR fmain);
extern FSS_ACTOR_WORK * Frontier_ActorBufferGet(FMAIN_PTR fmain);
extern FSS_ACTOR_WORK * Frontier_ActorWorkPtrGet(FMAIN_PTR fmain, int actor_no);
extern void Frontier_MapChangeReq(FMAIN_PTR fmain, u16 scene_id, u16 event_id);
extern PUSH_ARTICLE_WORK * Frontier_ArticlePushBufferGet(FMAIN_PTR fmain);
extern void Frontier_ArticlePushBufferInit(FMAIN_PTR fmain);

//==============================================================================
//	�f�[�^
//==============================================================================
extern const PROC_DATA FrontierMainProcData;


#endif	//__FRONTIER_MAIN_H__

