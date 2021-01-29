//==============================================================================
/**
 * @file	balloon.c
 * @brief	���D�c��܂����C��
 * @author	matsuda
 * @date	2007.11.01(��)
 */
//==============================================================================
#include "common.h"
#include "system/clact_tool.h"
#include "system/palanm.h"
#include "system/pmfprint.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/particle.h"
#include "system/brightness.h"
#include "system/snd_tool.h"
#include "communication/communication.h"
#include "msgdata/msg.naix"
#include "system/wipe.h"
#include "communication/wm_icon.h"
#include "system/msgdata_util.h"
#include <dwc.h>
#include "libdpw/dpw_tr.h"
#include "system/procsys.h"

#include "balloon_common.h"
#include "application/balloon.h"
#include "balloon_game.h"
#include "balloon_entry.h"


//==============================================================================
//	�萔��`
//==============================================================================
///���C���V�[�P���X�ԍ�
enum{
	MAINSEQ_ENTRY_PROC,
	MAINSEQ_ENTRY_WAIT,
	MAINSEQ_GAME_PROC,
	MAINSEQ_GAME_WAIT,
	MAINSEQ_RESULT_PROC,
	MAINSEQ_RESULT_WAIT,
	MAINSEQ_END_BEFORE_TIMING,
	MAINSEQ_END_BEFORE_TIMING_WAIT,
	MAINSEQ_END,
};

///�Q�[���I�����̍Ō�̓������Ɏg�p����ԍ�
#define BALLOON_END_TIMING_NO		(222)

//==============================================================================
//	PROC�f�[�^
//==============================================================================
///���D����Q�[����ʃv���Z�X��`�f�[�^
static const PROC_DATA BalloonGameProcData = {
	BalloonGameProc_Init,
	BalloonGameProc_Main,
	BalloonGameProc_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(balloon),
};

///���D����G���g���[��ʁ����ʔ��\��ʃv���Z�X��`�f�[�^
static const PROC_DATA BalloonEntryProcData = {
	BalloonEntryProc_Init,
	BalloonEntryProc_Main,
	BalloonEntryProc_End,
	NO_OVERLAY_ID,//FS_OVERLAY_ID(balloon),
};


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void Ballon_ProcWorkInit(BALLOON_SYSTEM_WORK *bsw, BALLOON_PROC_WORK *parent);
static BOOL Ballon_DisconnectErrorCheck( BALLOON_SYSTEM_WORK *bsw );


//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���D����F������
 *
 * @param   proc		PROC�ւ̃|�C���^
 * @param   seq			�V�[�P���X���[�N
 *
 * @retval  
 */
//--------------------------------------------------------------
PROC_RESULT BalloonProc_Init( PROC * proc, int * seq )
{
	BALLOON_PROC_WORK *parent = PROC_GetParentWork(proc);
	BALLOON_SYSTEM_WORK *bsw;

	//E���[���Ǘ��p�q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_BALLOON, 0x60000 );

	bsw = PROC_AllocWork(proc, sizeof(BALLOON_SYSTEM_WORK), HEAPID_BALLOON );
	MI_CpuClear8(bsw, sizeof(BALLOON_SYSTEM_WORK));
#ifdef PM_DEBUG
	bsw->debug_offline = parent->debug_offline;
#endif
	Ballon_ProcWorkInit(bsw, parent);
	
	return PROC_RES_FINISH;
}

//--------------------------------------------------------------
/**
 * @brief   ���D����F���C��
 *
 * @param   proc		PROC�ւ̃|�C���^
 * @param   seq			�V�[�P���X���[�N
 *
 * @retval  
 */
//--------------------------------------------------------------
PROC_RESULT BalloonProc_Main( PROC * proc, int * seq )
{
	BALLOON_SYSTEM_WORK * bsw  = PROC_GetWork( proc );
	BALLOON_PROC_WORK *parent = PROC_GetParentWork(proc);

	// �ʐM�G���[�I���`�F�b�N
	if( Ballon_DisconnectErrorCheck( bsw ) == TRUE ){
		return PROC_RES_FINISH;
	}
	
	
	switch(*seq){
	case MAINSEQ_ENTRY_PROC:
	#ifdef PM_DEBUG
		if(parent->debug_offline == TRUE){
			*seq = MAINSEQ_GAME_PROC;
			break;
		}
	#endif
		bsw->mode = BALLOON_MODE_ENTRY;
		bsw->sub_proc = PROC_Create(&BalloonEntryProcData, bsw, HEAPID_BALLOON);
		(*seq)++;
		break;
	case MAINSEQ_ENTRY_WAIT:
		if(ProcMain(bsw->sub_proc) == TRUE){
			PROC_Delete(bsw->sub_proc);
			bsw->sub_proc = NULL;
			(*seq)++;	//�Q�[����ʂ�
		}
		break;

	case MAINSEQ_GAME_PROC:
		bsw->mode = BALLOON_MODE_GAME;
		bsw->sub_proc = PROC_Create(&BalloonGameProcData, bsw, HEAPID_BALLOON);
		(*seq)++;
		break;
	case MAINSEQ_GAME_WAIT:
		if(ProcMain(bsw->sub_proc) == TRUE){
			PROC_Delete(bsw->sub_proc);
			bsw->sub_proc = NULL;
			(*seq)++;
		}
		break;

	case MAINSEQ_RESULT_PROC:
	#ifdef PM_DEBUG
		if(parent->debug_offline == TRUE){
			*seq = MAINSEQ_END;
			break;
		}
	#endif
		if(parent->vchat){
			mydwc_stopvchat();
		}
		bsw->mode = BALLOON_MODE_RESULT;
		bsw->sub_proc = PROC_Create(&BalloonEntryProcData, bsw, HEAPID_BALLOON);
		(*seq)++;
		break;
	case MAINSEQ_RESULT_WAIT:
		if(ProcMain(bsw->sub_proc) == TRUE){
			PROC_Delete(bsw->sub_proc);
			bsw->sub_proc = NULL;
			if(bsw->replay == TRUE){
				(*seq) = MAINSEQ_ENTRY_PROC;	//MAINSEQ_GAME_PROC;
			}
			else{
				(*seq)++;
			}
		}
		break;
	
	case MAINSEQ_END_BEFORE_TIMING:	//�Q�[���I���O�̍Ō�̓��������s��
		// �ؒf�G���[�𖳎�����i�u���[�X�N���[���ɂ���΂Ȃ��Ȃ�j
		CommStateSetErrorCheck(FALSE,TRUE);
		//�������ߑ��M
		CommTimingSyncStart(BALLOON_END_TIMING_NO);
		(*seq)++;
		break;
	case MAINSEQ_END_BEFORE_TIMING_WAIT:
		if((CommIsTimingSync(BALLOON_END_TIMING_NO) == TRUE) ||
			(CommGetConnectNum() < CommInfoGetEntryNum()) ){	// �l�������Ȃ��Ȃ����炻�̂܂ܔ�����
			(*seq)++;
		}
		break;

	case MAINSEQ_END:
	default:
		return PROC_RES_FINISH;
	}
	
	return PROC_RES_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���D����F�I������
 *
 * @param   proc		PROC�ւ̃|�C���^
 * @param   seq			�V�[�P���X���[�N
 *
 * @retval  
 */
//--------------------------------------------------------------
PROC_RESULT BalloonProc_End(PROC *proc, int *seq)
{
	BALLOON_SYSTEM_WORK * bsw  = PROC_GetWork( proc );

	PROC_FreeWork( proc );				// PROC���[�N�J��

	sys_DeleteHeap( HEAPID_BALLOON );

	return PROC_RES_FINISH;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   PROC���[�N�̏����l�ݒ�
 * @param   bsw		PROC���[�N�ւ̃|�C���^
 * @param   parent	BALLOON_PROC_WORK�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Ballon_ProcWorkInit(BALLOON_SYSTEM_WORK *bsw, BALLOON_PROC_WORK *parent)
{
	int i, index, my_no, current_id;
	
//	bsw->result_param.p_gadget = &parent->gadget;	// GADGET�Ȃ����܂��� tomoya
	
	current_id = CommGetCurrentID();
	my_no = 0;
	
	//�Q�����Ă���netID�̃��X�g���쐬
	index = 0;
	for(i = 0; i < WFLBY_MINIGAME_MAX; i++){
		if(CommInfoGetMyStatus(i) != NULL){
			bsw->player_netid[index] = i;
			if(i == current_id){
				my_no = index;
			}
			index++;
		}
		else{
			bsw->player_netid[i] = 0xffff;
		}
	}
	bsw->player_max = index;
	
	bsw->vchat = parent->vchat;
	
#ifdef PM_DEBUG
	if(bsw->debug_offline == TRUE){
		if(bsw->debug_offline == TRUE){
			bsw->player_netid[0] = 0;
			bsw->player_max = 1;
		}
	}
	else{
		//�G���g���[��ʁ����ʔ��\��ʗp�̒l�Z�b�g
		MNGM_ENRES_PARAM_Init( &bsw->entry_param, parent->wifi_lobby, parent->p_save, parent->vchat, &parent->lobby_wk );
	}
#else
	MNGM_ENRES_PARAM_Init( &bsw->entry_param, parent->wifi_lobby, parent->p_save, parent->vchat, &parent->lobby_wk );
#endif
}



//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM�G���[�ؒf�`�F�b�N
 *
 *	@param	bsw		���[�N
 *
 *	@retval	TRUE	�I����
 *	@retval	FALSE	�G���[�Ȃ�
 */
//-----------------------------------------------------------------------------
static BOOL Ballon_DisconnectErrorCheck( BALLOON_SYSTEM_WORK *bsw )
{
	if( bsw->dis_error == FALSE ){
		if( MNGM_ERROR_CheckDisconnect( &bsw->entry_param ) == TRUE ){
			bsw->dis_error = TRUE;	// �ؒf�G���[����

			// 
		}
	}

	if( bsw->dis_error == TRUE ){
		// �ؒf�G���[���ŁA�T�u�v���b�N���Ȃ��ꍇ�����ɏI��
		if( bsw->sub_proc == NULL ){
			return TRUE;
		}
	}
	return FALSE;
}
