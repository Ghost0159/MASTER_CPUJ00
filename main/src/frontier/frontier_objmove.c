//==============================================================================
/**
 * @file	frontier_objmove.c
 * @brief	OBJ����R�[�h�̎�����
 * @author	matsuda
 * @date	2007.07.27(��)
 */
//==============================================================================
#include "common.h"
#include "system/pm_overlay.h"
#include "system/procsys.h"
#include "system/pmfprint.h"
#include "system/palanm.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "system/bmp_list.h"
//#include "script.h"
#include "..\fielddata\script\connect_def.h"
#include "system/pm_overlay.h"

#include "system/msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_debug_matsu.h"

#include "frontier_scr.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier_scrcmd_sub.h"
#include "frontier_objmove.h"


//==============================================================================
//	�^��`
//==============================================================================
///OBJ����R�[�h�̊֐��^
typedef BOOL (*FSS_OBJMOVE_FUNC)(FMAIN_PTR, FSS_ACTOR_WORK *);


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void FSS_ObjMoveFunc(FMAIN_PTR fmain, FSS_ACTOR_WORK *fss_actor);
void FSS_ObjMoveFuncIDSet(FMAIN_PTR fmain, u16 playid, u8 move_id, s16 work[], int work_num);
void FSS_ObjMoveFuncAll(FMAIN_PTR fmain);
static BOOL MoveFunc_Kyoro(FMAIN_PTR fmain, FSS_ACTOR_WORK *fss_actor);


//==============================================================================
//	�֐��e�[�u��
//==============================================================================
///OBJ����R�[�h�̎��s�֐��e�[�u��
///		��OBJMOVE_ID_???�ƕ��т𓯂��ɂ��Ă������ƁI
static const FSS_OBJMOVE_FUNC FssObjMoveFuncTbl[] = {
	NULL,
	MoveFunc_Kyoro,
};



//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   OBJ����R�[�h�ݒ�
 *
 * @param   fmain		
 * @param   playid		OBJ�F��ID
 * @param   move_id		OBJ����R�[�hID(OBJMOVE_ID_???)
 * @param   work[]		�����Ƃ��ēn���l�̓������z��
 * @param   work_num	work[]�̗v�f��
 */
//--------------------------------------------------------------
void FSS_ObjMoveFuncIDSet(FMAIN_PTR fmain, u16 playid, u8 move_id, s16 work[], int work_num)
{
	FSS_ACTOR_WORK *fss_actor;
	int i;
	
	fss_actor = Frontier_ActorWorkSearch(fmain, playid);
	MI_CpuClear8(&fss_actor->move, sizeof(FSS_OBJMOVE_WORK));
	
	fss_actor->move.move_id = move_id;
	for(i = 0; i < work_num; i++){
		fss_actor->move.work[i] = work[i];
	}
}

//--------------------------------------------------------------
/**
 * @brief   OBJ����R�[�h���s
 *
 * @param   fmain		
 * @param   fss_actor	�Ώ�OBJ�̃A�N�^�[�|�C���^
 */
//--------------------------------------------------------------
static void FSS_ObjMoveFunc(FMAIN_PTR fmain, FSS_ACTOR_WORK *fss_actor)
{
	int ret;
	
	if(FssObjMoveFuncTbl[fss_actor->move.move_id] == NULL){
		return;
	}
	
	ret = FssObjMoveFuncTbl[fss_actor->move.move_id](fmain, fss_actor);
	if(ret == TRUE){
		MI_CpuClear8(&fss_actor->move, sizeof(FSS_OBJMOVE_WORK));
	}
}

//--------------------------------------------------------------
/**
 * @brief   �S�Ă�OBJ��OBJ����R�[�h�����s
 *
 * @param   fmain		
 */
//--------------------------------------------------------------
void FSS_ObjMoveFuncAll(FMAIN_PTR fmain)
{
	FSS_ACTOR_WORK *fss_actor;
	int i;
	
	fss_actor = Frontier_ActorBufferGet(fmain);
	for(i = 0; i < FIELD_OBJ_MAX; i++){
		if(fss_actor->objwk != NULL && fss_actor->anime_tcb == NULL){
			FSS_ObjMoveFunc(fmain, fss_actor);
		}
		fss_actor++;
	}
}




//==============================================================================
//
//	����R�[�h���s�֐�
//
//==============================================================================
///����낫���̌�����ς���E�F�C�g�l
#define KYORO_WAIT		(30)
///���ʂ������Ă��鎞�̃E�F�C�g�l
#define KYORO_FRONT_WAIT	(45)
//--------------------------------------------------------------
/**
 * @brief   OBJ����F����낫��낷��
 *
 * @param   fmain		
 * @param   fss_actor		
 *
 * @retval  TRUE:����I��
 *
 * work[0] = ���ʂ̌���(FC_DIR_???)
 * work[1] = ����낫��듮��ōŏ��Ɍ���������������肷��t���O(0 or 1)
 * work[2] = �����E�F�C�g
 *
 * ���ʂ̌�����DOWN�̏ꍇ�Awork[1]��0���ƁA�����E�A1���ƁA�E�����A�ƃA�j�����܂�
 */
//--------------------------------------------------------------
static BOOL MoveFunc_Kyoro(FMAIN_PTR fmain, FSS_ACTOR_WORK *fss_actor)
{
	FSS_OBJMOVE_WORK *move = &fss_actor->move;
	enum{
		WORK_FRONT_DIR,
		WORK_KYORO_FLG,
		WORK_WAIT,
		WORK_ANMCODE_1,
		WORK_ANMCODE_2,
		WORK_ANMCODE_3,
	};
	WF2DMAP_ACTCMD act;
	FMAP_PTR fmap = Frontier_FMapAdrsGet(fmain);
	
	if(move->work[WORK_WAIT] > 0){
		move->work[WORK_WAIT]--;
		return FALSE;
	}

	switch(move->seq){
	case 0:
		switch(move->work[WORK_FRONT_DIR]){
		case FC_DIR_U:
		case FC_DIR_D:
			if(move->work[WORK_KYORO_FLG] == 0){
				move->work[WORK_ANMCODE_1] = FC_DIR_L;
				move->work[WORK_ANMCODE_2] = FC_DIR_R;
			}
			else{
				move->work[WORK_ANMCODE_1] = FC_DIR_R;
				move->work[WORK_ANMCODE_2] = FC_DIR_L;
			}
			break;
		case FC_DIR_L:
		case FC_DIR_R:
			if(move->work[WORK_KYORO_FLG] == 0){
				move->work[WORK_ANMCODE_1] = FC_DIR_U;
				move->work[WORK_ANMCODE_2] = FC_DIR_D;
			}
			else{
				move->work[WORK_ANMCODE_1] = FC_DIR_D;
				move->work[WORK_ANMCODE_2] = FC_DIR_U;
			}
			break;
		default:
			GF_ASSERT(0);
			return TRUE;
		}
		move->work[WORK_ANMCODE_3] = move->work[WORK_FRONT_DIR];
		move->seq++;
		//break;
		
	case 1:
	case 2:
	case 3:
		FSSC_Sub_ActCmdCreate(&act, fss_actor->objwk, 
			fss_actor->param.playid, move->work[WORK_ANMCODE_1 + move->seq - 1]);
		WF2DMAP_ACTCMDQSysCmdPush(fmap->actcmd_q, &act);
		if(move->work[WORK_FRONT_DIR] == move->work[WORK_ANMCODE_1 + move->seq - 1]){
			move->work[WORK_WAIT] = KYORO_FRONT_WAIT;
			move->seq = 1;
		}
		else{
			move->work[WORK_WAIT] = KYORO_WAIT;
			move->seq++;
		}
		break;
	
	default:
		return TRUE;
	}
	
	return FALSE;
}

