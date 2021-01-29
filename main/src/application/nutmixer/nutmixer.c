/**
 *	@file	nutmixer.c
 *	@brief	�؂̎��܂��܂�����
 *	@author	Miyuki Iwasawa
 *	@date	06.04.06
 */

#include "common.h"
#include "system/procsys.h"
#include "system/snd_tool.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/palanm.h"
#include "system/savedata.h"
#include "itemtool/itemsym.h"
#include "itemtool/item.h"
#include "gflib/strbuf_family.h"

#include "application/nutmixer.h"
#include "nutmixer_sys.h"

#include "nmix_mseq.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_porutogame1.h"

#include "wifi/dwc_rap.h"
#include "wifi/vchat.h"

#include "communication/communication.h"
#include "communication/comm_state.h"

//�v���g�^�C�v�錾�@���[�J��
//================================================================

//#define VCHATPAUSE_ON
#define VCHATPAUSE_WAIT		(3)
#define VCHATPAUSE_TCBPRI	(64)
static void VChatPauseStart( NUTMIXER_WORK* p_wk );
static void VChatPauseOffTcb( TCB_PTR tcb, void* pwork );


//------------------------------------------------------------------
/**
 * @brief	�T�u�v���Z�X�Ăяo������
 * @param	proc	�T�u�v���Z�X�|�C���^��ێ����郏�[�N�ւ̃|�C���^
 */
//------------------------------------------------------------------
BOOL NutMixerProcCall(PROC ** proc)
{
	if (*proc) {
		if (ProcMain(*proc)) {
			PROC_Delete(*proc);
			*proc = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

/**
 *	@brief	�i�b�c�~�L�T�[�@�v���Z�X������
 */
PROC_RESULT	NutMixerProc_Init(PROC* proc,int *seq)
{
	NUTMIXER_WORK* wk = NULL;
	NUTMIXER_PARAM* pp = (NUTMIXER_PARAM*)PROC_GetParentWork(proc);
	
	HeapStatePush();

	//�q�[�v�쐬
	sys_CreateHeap(HEAPID_BASE_APP,HEAPID_NUT_MIXER,0x20000);
	wk = PROC_AllocWork(proc,sizeof(NUTMIXER_WORK),HEAPID_NUT_MIXER);
	MI_CpuClear8(wk,sizeof(NUTMIXER_WORK));

	wk->heapID = HEAPID_NUT_MIXER;

	//���p���p�����[�^
	wk->param = pp;

	if( (pp->wifi_pofin) && (pp->wifi_vchat) ){
		// �{�C�X�`���b�g�J�n
		mydwc_startvchat( HEAPID_NUT_MIXER );
		OS_Printf( "vct on\n" );
	}

	// wifi���ǂ�����ݒ�
	wk->wifi = pp->wifi_pofin;
	wk->wifi_vchat = pp->wifi_vchat;

	// �o�b�O�J�[�\���ʒu�L���̈揉����
	wk->bag_cursor = MyItem_BagCursorAlloc( wk->heapID );

	//���샂�[�h�擾
	if(pp->sys_mode == 1){
		wk->comm_f = TRUE;
	}

	return PROC_RES_FINISH;
}

/**
 *	@brief	�i�b�c�~�L�T�[�@�v���Z�X���C��
 */
PROC_RESULT	NutMixerProc_Main(PROC* proc,int *seq)
{
	int	old_seq;
	NUTMIXER_WORK* wk = PROC_GetWork(proc);
	NUTMIXER_PARAM* pp = (NUTMIXER_PARAM*)PROC_GetParentWork(proc);

	old_seq = *seq;
	switch(*seq){
	case NMSEQ_BAG_MAIN:
#ifdef VCHATPAUSE_ON
		VChatPauseStart( wk );	// vchat pause
#endif
		*seq =  NMixMainSeq_Bag(wk,&(wk->m_seq));
		break;
	case NMSEQ_GAME_CALL:
#ifdef VCHATPAUSE_ON
		VChatPauseStart( wk );	// vchat pause
#endif
		*seq = NMixMainSeq_GameCall(wk,&(wk->m_seq));
		break;
	case NMSEQ_GAME_WAIT:
		*seq = NMixMainSeq_GameWait(wk,&(wk->m_seq));
		break;
	case NMSEQ_GAME_END:
		*seq = NMixMainSeq_GameEnd(wk,&(wk->m_seq));
		break;
	case NMSEQ_MAIN_END:
		return PROC_RES_FINISH;
	}

	if(old_seq != *seq){
		wk->m_seq = 0;
	}

	return PROC_RES_CONTINUE;
}

/**
 *	@brief	�i�b�c�~�L�T�[�@�v���Z�X�I��
 */
PROC_RESULT NutMixerProc_End(PROC* proc,int *seq)
{
	int	heapID = 0;
	NUTMIXER_WORK* wk = PROC_GetWork(proc);
	NUTMIXER_PARAM* pp = (NUTMIXER_PARAM*)PROC_GetParentWork(proc);


	switch( *seq ){
	case 0:
		
		//���[�N�G���A���
		heapID = wk->heapID;

		// �o�b�O�J�[�\���ʒu�L���̈�j��
		sys_FreeMemoryEz( wk->bag_cursor );

		MI_CpuClear8(wk,sizeof(NUTMIXER_WORK));
		PROC_FreeWork(proc);

		if( (pp->wifi_pofin) && (pp->wifi_vchat) ){
			// �{�C�X�`���b�g�I��
			mydwc_stopvchat();
		}
		
		HeapStatePop();
		HeapStateCheck(heapID);

		sys_DeleteHeap(heapID);

		// Wi-Fi�łȂ���΂��̂܂ܔ�����
		if( pp->wifi_pofin == FALSE ){
			return PROC_RES_FINISH;
		}

		// �ȉ�Wi-Fi�̂Ƃ��̂�
		CommStateSetErrorCheck(FALSE,TRUE);
		CommTimingSyncStart(SYNCHROID_PROC_END);

		(*seq) ++;
		break;

	case 1:
		if( (CommIsTimingSync(SYNCHROID_PROC_END)) ||
			(CommGetConnectNum() < CommInfoGetEntryNum()) ){	// �l�������Ȃ��Ȃ����炻�̂܂ܔ�����
			return PROC_RES_FINISH;
		}
		break;

	default:
		GF_ASSERT(0);
		break;
	}
	return PROC_RES_CONTINUE;
}

//====================================================================
//���[�J���T�u�֐�
//====================================================================


//----------------------------------------------------------------------------
/**
 *	@brief	Pause�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void VChatPauseStart( NUTMIXER_WORK* p_wk )
{
	if( (p_wk->wifi_vchat == TRUE) && (p_wk->wifi == TRUE) ){
		mydwc_pausevchat( TRUE );
		p_wk->vchat_wait = VCHATPAUSE_WAIT;
		TCB_Add( VChatPauseOffTcb, p_wk, VCHATPAUSE_TCBPRI );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�{�C�X�`���b�gPauseOFFTCB
 */
//-----------------------------------------------------------------------------
static void VChatPauseOffTcb( TCB_PTR tcb, void* pwork )
{
	NUTMIXER_WORK* wk = pwork;
	if( --wk->vchat_wait <= 0 ){
		mydwc_pausevchat( FALSE );
		TCB_Delete( tcb );
	}
}
