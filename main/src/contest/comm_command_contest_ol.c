//=============================================================================
/**
 * @file	comm_command_contest_ol.c
 * @brief	comm_command_contest.c����R���e�X�g�̈�ɃI�[�o�[���C���Ă����Ȃ����̂�
 *			�����o�����t�@�C���ł��B
 * @author	matsuda
 * @date    2007.12.04(��)
 */
//=============================================================================
#include "common.h"
#include "communication/communication.h"

#include "contest/contest.h"
#include "comm_command_contest.h"
#include "actin.h"
#include "visual.h"
#include "dance.h"
#include "con_result.h"
#include "contest_order.h"
#include "dance_tool.h"
#include "gflib/strbuf_family.h"
#include "savedata/perapvoice.h"

#include "comm_command_contest_ol.h"


//--------------------------------------------------------------
/**
 * @brief   �m�[�}���f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		
 */
//--------------------------------------------------------------
void CommContestRecvNormal(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	CONTEST_ORDER_WORK *cow;
	
	if(consys->class_flag == CON_CLASS_VISUAL){
		VISUAL_PROC_WORK *vpw;
		vpw = consys->class_proc;
		cow = &vpw->cow;
		OS_TPrintf("�r�W���A������f�[�^��M�Bid_no=%d, size=%d\n", id_no, size);
	}
	else if(consys->class_flag == CON_CLASS_DANCE){
		DANCE_PROC_WORK *dpw;
		dpw = consys->class_proc;
		cow = &dpw->cow;
		OS_TPrintf("�_���X����f�[�^��M�Bid_no=%d, size=%d\n", id_no, size);
	}
	else if(consys->class_flag == CON_CLASS_ACTIN){
		ACTIN_PROC_WORK *apw;
		apw = consys->class_proc;
		cow = &apw->cow;
		OS_TPrintf("���Z�͕���f�[�^��M�Bid_no=%d, size=%d\n", id_no, size);
	}
	else if(consys->class_flag == CON_CLASS_RESULT){
		CONRES_PROC_WORK *rpw;
		rpw = consys->class_proc;
		cow = &rpw->cow;
		OS_TPrintf("���ʔ��\�f�[�^��M�Bid_no=%d, size=%d\n", id_no, size);
	}
	else{
		GF_ASSERT(0 && "�s���ȃN���X�ł�");
		return;
	}

	GF_ASSERT(size <= CO_TRANSMIT_BUF_SIZE);
	GF_ASSERT(cow->receive_flag[id_no] == FALSE);
	MI_CpuCopy8(pData, cow->receive_buf[id_no], size);
	cow->receive_flag[id_no] = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �_���X�f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		
 */
//--------------------------------------------------------------
void CommContestRecvDance(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	DANCE_PROC_WORK *dpw;
	DANCING_PARAM *dancing_param;
	
	dpw = consys->class_proc;
	dancing_param = pData;
	
	DT_DancingParamRecieve(dpw, dancing_param);
}
