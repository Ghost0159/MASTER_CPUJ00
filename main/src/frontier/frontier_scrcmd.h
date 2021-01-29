//============================================================================================
/**
 * @file	frontier_scrcmd.h
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h
 * @author	Satoshi Nohara
 * @date	07.04.10
 */
//============================================================================================
#ifndef	__FRONTIER_SCRCMD_H__
#define	__FRONTIER_SCRCMD_H__

#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_cmdq.h"


//==============================================================================
//	�\���̒�`
//==============================================================================
///�A�j���[�V�������X�g�f�[�^�\����
typedef struct{
	u16 code;		///<���s����A�j���[�V�����R�}���h�R�[�h
	u16 num;		///<code���s��
}FSSC_ANIME_DATA;

///�A�j���[�V����TCB����p���[�N�\����
typedef struct{
	u8 seq;
	u8 num;
	s16 wait;
	u16 playid;
	u8 *anm_count_ptr;
	const FSSC_ANIME_DATA *anm_list;
	WF2DMAP_ACTCMDQ *actcmd_q;
	FSS_ACTOR_WORK *fss_actor;
}FSSC_ANIME_MOVE_WORK;


//==============================================================================
//	�O���֐��錾
//==============================================================================
//extern inline u16 * FSSTGetWork(FSS_TASK *core);
//extern inline u16 FSSTGetWorkValue(FSS_TASK * core);
extern u16 * FSSTGetWork(FSS_TASK *core);
extern u16 FSSTGetWorkValue(FSS_TASK * core);
extern u16 FSS_GetEventWorkValue(FSS_TASK *core, u16 work_id );
extern u16 * FSS_GetEventWorkAdrs(FSS_TASK *core, u16 wk_id);


#endif	//__FRONTIER_SCRCMD_H__


