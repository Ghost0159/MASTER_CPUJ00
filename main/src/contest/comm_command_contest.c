//=============================================================================
/**
 * @file	comm_command_contest.c
 * @brief	�f�[�^�𑗂邽�߂̃R�}���h���e�[�u�������Ă��܂�
 *          �R���e�X�g�p�ł�
 *          comm_command_contest.h �� enum �Ɠ������тł���K�v������܂�
 * @author	matsuda
 * @date    2005.12.15(��)
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
#include "comm_command_contest_fld.h"


//==============================================================================
//  �e�[�u���ɏ����֐��̒�`
//==============================================================================
static	void CommContestRecvTiming(int id_no,int size,void *pData,void *work);
static u8* CommContestGetRecvDataBuff( int netID,void*pWork,int size);
static	void CommContestRecvClipData(int id_no,int size,void *pData,void *work);
static	void CommContestRecvClipDataAll(int id_no,int size,void *pData,void *work);
static	void CommContestRecvImcLocal(int id_no,int size,void *pData,void *work);
static	void CommContestRecvImcForceEnd(int id_no,int size,void *pData,void *work);

//==============================================================================
//  static��`
//==============================================================================
static int _getDancingSize(void);
static int _getImcLocalSize(void);

//==============================================================================
//	�e�[�u���錾
//  comm_shar.h �� enum �Ɠ����Ȃ�тɂ��Ă�������
//  CALLBACK���Ă΂ꂽ���Ȃ��ꍇ��NULL�������Ă�������
//  �R�}���h�̃T�C�Y��Ԃ��֐��������Ă��炦��ƒʐM���y���Ȃ�܂�
//  _getZero�̓T�C�Y�Ȃ���Ԃ��܂��B_getVariable�͉σf�[�^�g�p���Ɏg���܂�
//==============================================================================
static const CommPacketTbl _CommPacketTbl[] = {
    {NULL,                      _getZero, NULL},		// CC_EXIT_CONTEST
	{CommContestRecvTiming, _getZero, NULL},			// CC_TIMING_DATA
	{CommContestRecvNormal, _getVariable, CommContestGetRecvDataBuff},		// CC_NORMAL_DATA
	{CommContestRecvDance, _getDancingSize, NULL},		// CC_DANCE_DATA
	{CommContestRecvFastParam, _getVariable, NULL},	// CC_FAST_PARAM
	{CommContestRecvPokemonParam, _getVariable, NULL},	// CC_POKEPARA_DATA
	{CommContestRecvPokemonParamAll, _getVariable, CommContestGetRecvDataBuff},	// CC_POKEPARA_ALL_DATA
	{CommContestRecvBreederData, _getVariable, NULL},	// CC_BREEDER_DATA
	{CommContestRecvJudgeData, _getVariable, NULL},	// CC_JUDGE_DATA
	{CommContestRecvNameData, _getVariable, NULL},	// CC_NAME_DATA
	{CommContestRecvClipData, _getVariable, NULL},	// CC_IMC_DATA
	{CommContestRecvClipDataAll, _getVariable, CommContestGetRecvDataBuff},	// CC_IMC_ALL_DATA
	{CommContestRecvImcLocal, _getImcLocalSize, NULL},	// CC_IMC_LOCAL
	{CommContestRecvImcForceEnd, _getImcLocalSize, NULL},	// CC_IMC_FORCE_END
	{CommContestRecvPerap, _getVariable, CommContestGetRecvDataBuff},	// CC_PERAP_DATA
	{CommContestRecvEtcData, _getVariable, NULL},	// CC_ETC_DATA
};

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�p�̃R�}���h�̌n�ɏ��������܂�
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------
void CommCommandContestInitialize(void* pWork)
{
    int length = sizeof(_CommPacketTbl)/sizeof(CommPacketTbl);
    CommCommandInitialize(_CommPacketTbl, length, pWork);
}

//--------------------------------------------------------------
/**
 * @brief   ��M�̈�擾�֐�(����f�[�^�FCommSendHugeData�̎�M�p)
 *
 * @param   netID		
 * @param   pWork		
 * @param   size		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static u8* CommContestGetRecvDataBuff( int netID,void*pWork,int size)
{
	CONTEST_SYSTEM *consys = pWork;
	
	GF_ASSERT(size < CON_HUGEBUF_SIZE);
    return consys->recieve_huge_buf[netID];
}

//--------------------------------------------------------------
/**
 * @brief   �R�Ƃ��T�C�Y��Ԃ��܂�
 * @param   command         �R�}���h
 * @retval  �T�C�Y   �ςȂ� COMM_VARIABLE_SIZE Zero�͂O��Ԃ�
 */
//--------------------------------------------------------------
static int _getDancingSize(void)
{
	return sizeof(DANCING_PARAM);
}

//--------------------------------------------------------------
/**
 * @brief   �R�Ƃ��T�C�Y��Ԃ��܂�
 * @param   command         �R�}���h
 * @retval  �T�C�Y   �ςȂ� COMM_VARIABLE_SIZE Zero�͂O��Ԃ�
 */
//--------------------------------------------------------------
static int _getImcLocalSize(void)
{
	return sizeof(CON_IMC_LOCAL_SIO);
}





//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �������ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		
 */
//--------------------------------------------------------------
static	void CommContestRecvTiming(int id_no,int size,void *pData,void *work)
{
	;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �N���b�v�f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
static	void CommContestRecvClipData(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int clip_size;
	int breeder_no;
	u8 *clip_buf;
	
	clip_size = ImcSaveData_GetContestWorkSize();
//	breeder_no = ((u8*)pData)[clip_size];
	clip_buf = pData;
	breeder_no = clip_buf[clip_size];

	//OS_TPrintf("clip_size = %d, size = %d\n", clip_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("clip��breeder_no = %d\n", breeder_no);
	//OS_TPrintf("clip�̍Ō�f�[�^ = %d\n", clip_buf[clip_size - 1]);

	MI_CpuCopy8(pData, consys->c_game.imc_data[breeder_no], clip_size);
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �N���b�v�f�[�^���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗��|�P�������������Ă���u���[�_�[�̔ԍ�
 * @param   pp				�N���b�v�f�[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendClipData(CONTEST_SYSTEM *consys, int breeder_no, const IMC_CONTEST_SAVEDATA *imc_data)
{
	u8 *clip_buf;
	int clip_size;
	int ret;
	
	clip_size = ImcSaveData_GetContestWorkSize();
	clip_buf = sys_AllocMemory(HEAPID_CONTEST, clip_size + 1);
	MI_CpuCopy8(imc_data, clip_buf, clip_size);
	clip_buf[clip_size] = breeder_no;
	
	//OS_TPrintf("clip_size = %d\n", clip_size);
	//OS_TPrintf("breeder_no = %d\n", clip_buf[clip_size]);
	//OS_TPrintf("���M��clip_data�̍Ō�f�[�^ = %d\n", clip_buf[clip_size - 1]);
	if(CommSendData(CC_IMC_DATA, clip_buf, clip_size + 1) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
	sys_FreeMemoryEz(clip_buf);
	return ret;
}

//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �N���b�v�f�[�^(�S����)�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
static	void CommContestRecvClipDataAll(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int clip_size, all_size;
	u8 *clip_buf;
	int i;
	
	clip_size = ImcSaveData_GetContestWorkSize();
	all_size = clip_size * BREEDER_MAX;
//	breeder_no = ((u8*)pData)[clip_size];
	clip_buf = pData;

	//OS_TPrintf("all_size = %d, size = %d\n", all_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("clip�̍Ō�f�[�^ = %d\n", clip_buf[all_size - 1]);
	GF_ASSERT(all_size < CON_HUGEBUF_SIZE);

	for(i = 0; i < BREEDER_MAX; i++){
		MI_CpuCopy8(&clip_buf[clip_size * i], consys->c_game.imc_data[i], clip_size);
	}
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �N���b�v�f�[�^(�S����)���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   imc_data		�N���b�v�f�[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * ���e��p���߂ł��B
 */
//--------------------------------------------------------------
BOOL CommContestSendClipDataAll(CONTEST_SYSTEM *consys, IMC_CONTEST_SAVEDATA **imc_data)
{
	u8 *clip_buf;
	int clip_size, all_size;
	int ret;
	int i;
	
	clip_size = ImcSaveData_GetContestWorkSize();
	all_size = clip_size * BREEDER_MAX;
	GF_ASSERT(all_size < CON_HUGEBUF_SIZE);
//	clip_buf = sys_AllocMemory(HEAPID_CONTEST, all_size);
	clip_buf = consys->huge_buf;
	for(i = 0; i < BREEDER_MAX; i++){
		MI_CpuCopy8(imc_data[i], &clip_buf[clip_size * i], clip_size);
	}
	
	//OS_TPrintf("all_size = %d\n", all_size);
	//OS_TPrintf("breeder_no = %d\n", clip_buf[all_size]);
	//OS_TPrintf("���M��clip�̍Ō�f�[�^ = %d\n", clip_buf[all_size - 1]);
	if(CommSendHugeData(CC_IMC_ALL_DATA, clip_buf, all_size) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
//	sys_FreeMemoryEz(clip_buf);
	return ret;
}

//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �C���[�W�N���b�v��ʗp�f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
static	void CommContestRecvImcLocal(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;

	MI_CpuCopy8(pData, &consys->imc_local.recieve_sio, size);
}

//--------------------------------------------------------------
/**
 * @brief   �C���[�W�N���b�v��ʗp�F�J�E���^�[�f�[�^���M����
 *
 * @param   imc_local		�C���[�W�N���b�v��ʗp�ʐM����M���[�N�ւ̃|�C���^
 * @param   counter			�J�E���^�[
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendImcLocal(CON_IMC_LOCAL *imc_local, u32 counter)
{
	if(imc_local->sio_flag == FALSE){
		imc_local->recieve_sio.counter = counter;
		return TRUE;
	}

	if(imc_local->server_no != imc_local->my_net_id){
		return FALSE;
	}
	
	imc_local->trans_sio.counter = counter;
	if(CommSendData(CC_IMC_LOCAL, &imc_local->trans_sio, sizeof(CON_IMC_LOCAL_SIO)) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �C���[�W�N���b�v��ʗp�A�e�E�q �f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
static	void CommContestRecvImcForceEnd(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	CON_IMC_LOCAL_SIO *imc_local;
	
	imc_local = pData;
	consys->imc_local.recieve_force_end[id_no] = imc_local->force_end;
}

//--------------------------------------------------------------
/**
 * @brief   �C���[�W�N���b�v��ʗp�F�����I���t���O���M
 *
 * @param   imc_local		�C���[�W�N���b�v��ʗp�ʐM����M���[�N�ւ̃|�C���^
 * @param   counter			�J�E���^�[
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * �e�E�q�A���Ɏg�p�o���܂��B
 */
//--------------------------------------------------------------
BOOL CommContestSendImcForceEnd(CON_IMC_LOCAL *imc_local, int force_end)
{
	if(imc_local->sio_flag == FALSE){
		imc_local->recieve_force_end[0] = force_end;
		return TRUE;
	}

	imc_local->trans_sio.force_end = force_end;
	if(CommSendData(CC_IMC_FORCE_END, &imc_local->trans_sio, sizeof(CON_IMC_LOCAL_SIO)) == TRUE){
		return TRUE;
	}
	return FALSE;
}


