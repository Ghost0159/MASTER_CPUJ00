//==============================================================================
/**
 * @file	battle_playbackstop.c
 * @brief	�퓬�^��Đ����̒�~�{�^������
 * @author	matsuda
 * @date	2007.07.25(��)
 */
//==============================================================================
#include "common.h"
#include "battle/battle_common.h"
#include "battle_input.h"
#include "battle/battle_tcb_pri.h"
#include "battle/fight_tool.h"
#include "battle_playbackstop.h"
#include "system/snd_tool.h"
#include "battle_snd_def.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�T�[�o�[�ɒ�~�t���O���Z�b�g����O�ɏ����E�F�C�g������(����������SE�𕷂����邽��)
#define RPS_END_WAIT		(8)


//==============================================================================
//	�\���̒�`
//==============================================================================
///�^��Đ��̒�~�{�^������V�X�e���\����
typedef struct{
	BATTLE_WORK *bw;	///<�o�g�����[�N�ւ̃|�C���^
	s16 seq;			///<���݂̃V�[�P���X�ԍ�
	s16 wait;			///<�E�F�C�g�J�E���^
}PLAYBACK_STOP_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void PlayBackStop_Main(TCB_PTR tcb, void *work);



//--------------------------------------------------------------
/**
 * @brief   �퓬�^��Đ��̒�~�{�^������V�X�e���쐬
 * @param   bw		
 * @retval  ����V�X�e����TCB�|�C���^
 */
//--------------------------------------------------------------
TCB_PTR PlayBackStopButton_Create(BATTLE_WORK *bw)
{
	PLAYBACK_STOP_WORK *psw;
	TCB_PTR tcb;
	
	psw = sys_AllocMemory(HEAPID_BATTLE, sizeof(PLAYBACK_STOP_WORK));
	MI_CpuClear8(psw, sizeof(PLAYBACK_STOP_WORK));
	
	psw->bw = bw;
	tcb = TCB_Add(PlayBackStop_Main, psw, TCBPRI_PLAYBACK_STOP);
	
	return tcb;
}

//--------------------------------------------------------------
/**
 * @brief   �퓬�^��Đ��̒�~�{�^������V�X�e�����
 * @param   tcb		����V�X�e����TCB�|�C���^
 */
//--------------------------------------------------------------
void PlayBackStopButton_Free(TCB_PTR tcb)
{
	PLAYBACK_STOP_WORK *psw;
	
	psw = TCB_GetWork(tcb);
	sys_FreeMemoryEz(psw);
	TCB_Delete(tcb);
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �퓬�^��Đ��̒�~�{�^������V�X�e���F���C������
 *
 * @param   tcb			TCB�ւ̃|�C���^
 * @param   work		PLAYBACK_STOP_WORK�\����
 */
//--------------------------------------------------------------
static void PlayBackStop_Main(TCB_PTR tcb, void *work)
{
	PLAYBACK_STOP_WORK *psw = work;
	enum{
		SEQ_INIT,
		SEQ_IN_EFF_WAIT,
		SEQ_MAIN,
		SEQ_WAIT,
		SEQ_END,
	};
	BI_PARAM_PTR bip;
	
	bip = BattleWorkGF_BGL_BIPGet(psw->bw);
	
	switch(psw->seq){
	case SEQ_INIT:
		GF_ASSERT(bip != NULL);
		
		{
			ARCHANDLE* hdl_bg, *hdl_obj;

			hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
			hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
			BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_PLAYBACK_STOP, FALSE, NULL);
			ArchiveDataHandleClose( hdl_bg );
			ArchiveDataHandleClose( hdl_obj );
		}
		psw->seq++;
		break;
	case SEQ_IN_EFF_WAIT:
		if(BINPUT_EffectEndCheck(bip) == TRUE){
			psw->seq++;
		}
		break;
	case SEQ_MAIN:
		if(BattleWorkBattleRecStopFlagCheck(psw->bw) == TRUE){
			if(BINPUT_TouchCheck(bip) == TRUE){
				Snd_SePlay(BSE_TP_DECIDE);
				psw->seq++;
			}
		}
		break;
	case SEQ_WAIT:
		psw->wait++;
		if(psw->wait > RPS_END_WAIT){
			BattleWorkBattleRecStopFlagSet(psw->bw,BI_SYSMSG_STOP);
			psw->seq++;
		}
		break;

	case SEQ_END:	//�S�Ă̏����I���BFree���Ă΂��̂�҂�
		break;
	}
}

