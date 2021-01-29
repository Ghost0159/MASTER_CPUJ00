//=============================================================================
/**
 * @file	comm_command_contest_fld.c
 * @brief	comm_command_contest.c����t�B�[���h�}�b�v�̈�ɃI�[�o�[���C���Ă����Ȃ����̂�
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

#include "comm_command_contest_fld.h"


//==============================================================================
//	�\���̒�`
//==============================================================================
typedef struct{
	u8 my_version;					///<�����̃R���e�X�g�o�[�W����
	u8 my_breeder_no;				///<�����̃u���[�_�[�ԍ�
	u8 theme;						///<�C���[�W�N���b�v�̃e�[�}
	
	u8 dummy[1];
}CCSEND_FASTPARAM;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static int ServerNoCheck(CCSEND_FASTPARAM *ccfp[CC_NETID_MAX], int player_num, u8 *version);


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �ŏ��Ɍ�������f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvFastParam(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	
	MI_CpuCopy8(pData, consys->recieve_buf[id_no], size);
	consys->recieve_count++;
	//OS_TPrintf("net_id = %d, server_versin = %d\n", id_no, ((CCSEND_FASTPARAM*)pData)->my_version);
	//OS_TPrintf("my_breeder_no = %d, theme = %d\n", ((CCSEND_FASTPARAM*)pData)->my_breeder_no, ((CCSEND_FASTPARAM*)pData)->theme);
	
	if(consys->recieve_count >= consys->c_game.player_num){
		CCSEND_FASTPARAM *ccfp[CC_NETID_MAX];
		int i;
		u8 version;
		
		for(i = 0; i < consys->c_game.player_num; i++){
			ccfp[i] = (void*)consys->recieve_buf[i];
		}
		consys->c_game.server_no = ServerNoCheck(ccfp, consys->c_game.player_num, &version);
		consys->c_game.server_version = version;
		consys->c_game.theme = ccfp[consys->c_game.server_no]->theme;
		
		//OS_TPrintf("�e��netID��%d\n", consys->c_game.server_no);
		//OS_TPrintf("�e�̃C���[�W�N���b�v�e�[�}��%d\n", consys->c_game.theme);
	}
}

//--------------------------------------------------------------
/**
 * @brief   �ŏ��Ɍ�������f�[�^���M����
 * @param   consys		�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendFastParam(CONTEST_SYSTEM *consys)
{
	CCSEND_FASTPARAM ccfp;
	
	ccfp.my_version = consys->c_game.my_version;
	ccfp.my_breeder_no = consys->c_game.my_breeder_no;
	ccfp.theme = consys->c_game.theme;
	
	if(CommSendData(CC_FAST_PARAM, &ccfp, sizeof(CCSEND_FASTPARAM)) == TRUE){
//		consys->recieve_count = 0;
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �T�[�o�[�ƂȂ�NetID���擾����
 *
 * @param   ccfp[CC_NETID_MAX]		
 * @param   player_num				�v���C���[�l��
 *
 * @retval  �T�[�o�[�ƂȂ��̃l�b�gID
 */
//--------------------------------------------------------------
static int ServerNoCheck(CCSEND_FASTPARAM *ccfp[CC_NETID_MAX], int player_num, u8 *version)
{
	int i, server_no;
	
	server_no = 0;
	for(i = 0; i < player_num; i++){
		if(ccfp[server_no]->my_version < ccfp[i]->my_version){
			server_no = i;
		}
	}
	*version = ccfp[server_no]->my_version;
	return server_no;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �|�P�����p�����[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvPokemonParam(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int pp_size;
	int breeder_no;
	u8 *pp_buf;
	
	pp_size = PokemonParam_GetWorkSize();
//	breeder_no = ((u8*)pData)[pp_size];
	pp_buf = pData;
	breeder_no = pp_buf[pp_size];

	//OS_TPrintf("pp_size = %d, size = %d\n", pp_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("pp��breeder_no = %d\n", breeder_no);
	//OS_TPrintf("pp�̍Ō�f�[�^ = %d\n", pp_buf[pp_size - 1]);

	MI_CpuCopy8(pData, consys->c_game.pp[breeder_no], pp_size);
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����p�����[�^���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗��|�P�������������Ă���u���[�_�[�̔ԍ�
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendPokePara(CONTEST_SYSTEM *consys, int breeder_no, const POKEMON_PARAM *pp)
{
	u8 *pp_buf;
	int pp_size;
	int ret;
	
	pp_size = PokemonParam_GetWorkSize();
	pp_buf = sys_AllocMemory(HEAPID_CONTEST, pp_size + 1);
	MI_CpuCopy8(pp, pp_buf, pp_size);
	pp_buf[pp_size] = breeder_no;
	
	//OS_TPrintf("pp_size = %d\n", pp_size);
	//OS_TPrintf("breeder_no = %d\n", pp_buf[pp_size]);
	//OS_TPrintf("���M��pp�̍Ō�f�[�^ = %d\n", pp_buf[pp_size - 1]);
	if(CommSendData(CC_POKEPARA_DATA, pp_buf, pp_size + 1) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
	sys_FreeMemoryEz(pp_buf);
	return ret;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �|�P�����p�����[�^(�S����)�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvPokemonParamAll(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int pp_size, all_size;
	u8 *pp_buf;
	int i;
	
	pp_size = PokemonParam_GetWorkSize();
	all_size = pp_size * BREEDER_MAX;
//	breeder_no = ((u8*)pData)[pp_size];
	pp_buf = pData;

	//OS_TPrintf("all_size = %d, size = %d\n", all_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("pp�̍Ō�f�[�^ = %d\n", pp_buf[all_size - 1]);

	for(i = 0; i < BREEDER_MAX; i++){
		MI_CpuCopy8(&pp_buf[pp_size * i], consys->c_game.pp[i], pp_size);
	}
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �|�P�����p�����[�^(�S����)���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   pp				�|�P�����p�����[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 *
 * ���e��p���߂ł��B
 */
//--------------------------------------------------------------
BOOL CommContestSendPokeParaAll(CONTEST_SYSTEM *consys, POKEMON_PARAM **pp)
{
	u8 *pp_buf;
	int pp_size, all_size;
	int ret;
	int i;
	
	pp_size = PokemonParam_GetWorkSize();
	all_size = pp_size * BREEDER_MAX;
//	pp_buf = sys_AllocMemory(HEAPID_CONTEST, all_size);
	pp_buf = consys->huge_buf;
	for(i = 0; i < BREEDER_MAX; i++){
		MI_CpuCopy8(pp[i], &pp_buf[pp_size * i], pp_size);
	}
	
	//OS_TPrintf("all_size = %d\n", all_size);
	//OS_TPrintf("breeder_no = %d\n", pp_buf[all_size]);
	//OS_TPrintf("���M��pp�̍Ō�f�[�^ = %d\n", pp_buf[all_size - 1]);
	if(CommSendHugeData(CC_POKEPARA_ALL_DATA, pp_buf, all_size) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
//	sys_FreeMemoryEz(pp_buf);
	return ret;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �u���[�_�[�f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvBreederData(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int bd_size;
	int breeder_no;
	u8 *bd_buf;
	
	bd_size = sizeof(BREEDER_DATA);
//	breeder_no = ((u8*)pData)[bd_size];
	bd_buf = pData;
	breeder_no = bd_buf[bd_size];

	//OS_TPrintf("bd_size = %d, size = %d\n", bd_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("pp��breeder_no = %d\n", breeder_no);
	//OS_TPrintf("pp�̍Ō�f�[�^ = %d\n", bd_buf[bd_size - 1]);

	MI_CpuCopy8(pData, &consys->c_game.bd[breeder_no], bd_size);
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �u���[�_�[�f�[�^���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗��|�P�������������Ă���u���[�_�[�̔ԍ�
 * @param   bd				���M����u���[�_�[�f�[�^�ւ̃|�C���^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendBreederData(CONTEST_SYSTEM *consys, int breeder_no, const BREEDER_DATA *bd)
{
	u8 *bd_buf;
	int bd_size;
	int ret;
	
	bd_size = sizeof(BREEDER_DATA);
	bd_buf = sys_AllocMemory(HEAPID_CONTEST, bd_size + 1);
	MI_CpuCopy8(bd, bd_buf, bd_size);
	bd_buf[bd_size] = breeder_no;
	
	//OS_TPrintf("bd_size = %d\n", bd_size);
	//OS_TPrintf("breeder_no = %d\n", bd_buf[bd_size]);
	//OS_TPrintf("���M��bd�̍Ō�f�[�^ = %d\n", bd_buf[bd_size - 1]);
	if(CommSendData(CC_BREEDER_DATA, bd_buf, bd_size + 1) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
	sys_FreeMemoryEz(bd_buf);
	return ret;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �R���f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvJudgeData(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int jd_all_size;
	u8 *jd_buf;
	int i;
	
	jd_all_size = sizeof(JUDGE_DATA) * JUDGE_MAX + 1;
	jd_buf = pData;

	//OS_TPrintf("jd_all_size = %d, size = %d\n", jd_all_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("jd�̍Ō�f�[�^ = %d\n", jd_buf[jd_all_size - 1]);

	for(i = 0; i < JUDGE_MAX; i++){
		MI_CpuCopy8(&jd_buf[sizeof(JUDGE_DATA) * i], &consys->c_game.jd[i], sizeof(JUDGE_DATA));
	}
	consys->c_game.special_judge_no = jd_buf[jd_all_size - 1];
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �R���f�[�^���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗��|�P�������������Ă���u���[�_�[�̔ԍ�
 * @param   jd				�R���f�[�^
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendJudgeData(CONTEST_SYSTEM *consys, int breeder_no, const JUDGE_DATA *jd)
{
	u8 *jd_buf;
	int jd_all_size;
	int ret;
	int i;
	const JUDGE_DATA *jd_src;
	
	jd_src = jd;
	jd_all_size = sizeof(JUDGE_DATA) * JUDGE_MAX + 1;
	jd_buf = sys_AllocMemory(HEAPID_CONTEST, jd_all_size);
	for(i = 0; i < JUDGE_MAX; i++){
		MI_CpuCopy8(jd_src, &jd_buf[sizeof(JUDGE_DATA) * i], sizeof(JUDGE_DATA));
		jd_src++;
	}
	jd_buf[jd_all_size - 1] = consys->c_game.special_judge_no;
	
	//OS_TPrintf("jd_all_size = %d\n", jd_all_size);
	//OS_TPrintf("���M��jd�̍Ō�f�[�^ = %d\n", jd_buf[jd_all_size - 1]);
	if(CommSendData(CC_JUDGE_DATA, jd_buf, jd_all_size) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
	sys_FreeMemoryEz(jd_buf);
	return ret;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �u���[�_�[���ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvNameData(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int breeder_no, name_size, head_size;
	u8 *recieve_buf;
	STRCODE *name_code;
	
	head_size = 4;
	recieve_buf = pData;
	breeder_no = recieve_buf[0];
	name_size = recieve_buf[1];
	name_code = (STRCODE *)(&recieve_buf[head_size]);
	
	//OS_TPrintf("���O��M�F�u���[�_�[No=%d�A���O�T�C�Y=%d\n", breeder_no, name_size);
	STRBUF_Clear(consys->c_game.breeder_name_str[breeder_no]);
	STRBUF_SetStringCode(consys->c_game.breeder_name_str[breeder_no], name_code);
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �u���[�_�[�����M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗�閼�O���������Ă���u���[�_�[��NO
 * @param   name			���M����u���[�_�[��
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendNameData(CONTEST_SYSTEM *consys, int breeder_no, const STRBUF *name)
{
	int name_size, head_size;
	u8 *trans_buf;
	BOOL ret;
	STRCODE name_code[BUFLEN_PERSON_NAME];
	
	name_size = BUFLEN_PERSON_NAME * sizeof(STRCODE);
	head_size = 4;
	
	STRBUF_GetStringCode(name, name_code, BUFLEN_PERSON_NAME);
	
	trans_buf = sys_AllocMemory(HEAPID_CONTEST, name_size + head_size);
	MI_CpuCopy8(name_code, &trans_buf[head_size], name_size);
	trans_buf[0] = breeder_no;
	trans_buf[1] = name_size;
	trans_buf[2] = 0;	//�ꉞ4�o�C�g���E�_�~�[
	trans_buf[3] = 0;	//�ꉞ4�o�C�g���E�_�~�[
	
	//OS_TPrintf("���O���M�F�u���[�_�[No=%d, ���O�T�C�Y=%d\n", breeder_no, name_size);
	if(CommSendData(CC_NAME_DATA, trans_buf, name_size + head_size) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
	
	sys_FreeMemoryEz(trans_buf);
	return ret;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �؃��b�v�p�����[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvPerap(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int perap_size;
	int breeder_no;
	u8 *perap_buf;
	
	perap_size = PERAPVOICE_GetWorkSize();
//	breeder_no = ((u8*)pData)[perap_size];
	perap_buf = pData;
	breeder_no = perap_buf[perap_size];

	//OS_TPrintf("�؃��b�v�f�[�^��M\n");
	//OS_TPrintf("perap_size = %d, size = %d\n", perap_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("perap��breeder_no = %d\n", breeder_no);
	//OS_TPrintf("perap�̍Ō�f�[�^ = %d\n", perap_buf[perap_size - 1]);
	
	MI_CpuCopy8(pData, consys->perap_voice[breeder_no], perap_size);
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �؃��b�v�p�����[�^���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗��|�P�������������Ă���u���[�_�[�̔ԍ�
 * @param   perap_voice		�؃��b�v�f�[�^(NULL�̏ꍇ�͂��̊֐����ĂԑO�Ɏ�����huge_buf�ɂ؃��b�v�f�[�^�����[�h���Ă�������)
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendPerap(CONTEST_SYSTEM *consys, int breeder_no, void *perap_voice)
{
	u8 *perap_buf;
	int perap_size;
	int ret;
	
	perap_size = PERAPVOICE_GetWorkSize();
//	perap_buf = sys_AllocMemory(HEAPID_CONTEST, perap_size + 1);
	perap_buf = consys->huge_buf;
	if(perap_voice != NULL){
		MI_CpuCopy8(perap_voice, perap_buf, perap_size);
	}
	perap_buf[perap_size] = breeder_no;
	
	//OS_TPrintf("perap_size = %d\n", perap_size);
	//OS_TPrintf("breeder_no = %d\n", perap_buf[perap_size]);
	//OS_TPrintf("���M��perap�̍Ō�f�[�^ = %d\n", perap_buf[perap_size - 1]);
	if(CommSendHugeData(CC_PERAP_DATA, perap_buf, perap_size + 1) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
//	sys_FreeMemoryEz(perap_buf);
	return ret;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �Œ蒷�ׂ̍����f�[�^�Q��đ��M�p�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		CONTEST_SYSTEM�ւ̃|�C���^
 */
//--------------------------------------------------------------
void CommContestRecvEtcData(int id_no,int size,void *pData,void *work)
{
	CONTEST_SYSTEM *consys = work;
	int etc_size;
	int breeder_no;
	u8 *data;
	CON_ETC_SIO_DATA *etc_buf;
	
	etc_size = sizeof(CON_ETC_SIO_DATA);
	etc_buf = pData;
	data = pData;
	breeder_no = data[etc_size];

	//OS_TPrintf("etc_size = %d, size = %d\n", etc_size, size);
	//OS_TPrintf("pData[0] = %d\n", ((u8*)pData)[0]);
	//OS_TPrintf("etc_data��breeder_no = %d\n", breeder_no);
	//OS_TPrintf("etc_data�̍Ō�f�[�^ = %d\n", etc_buf[etc_size - 1]);

	consys->c_game.player_sex[breeder_no] = etc_buf->player_sex;
	consys->c_game.character[breeder_no] = etc_buf->character;
	consys->c_game.popularity[breeder_no] = etc_buf->popularity;
	consys->c_game.obj_code[breeder_no] = etc_buf->obj_code;
	
	consys->recieve_count++;
}

//--------------------------------------------------------------
/**
 * @brief   �Œ蒷�ׂ̍����f�[�^�Q��đ��M�p���M����
 *
 * @param   consys			�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @param   breeder_no		�����瑗��|�P�������������Ă���u���[�_�[�̔ԍ�
 * @param   etc_data		���M����ׂ����f�[�^�Q
 *
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL CommContestSendEtcData(CONTEST_SYSTEM *consys, int breeder_no, const CON_ETC_SIO_DATA *etc_data)
{
	u8 *etc_buf;
	int etc_size;
	int ret;
	
	etc_size = sizeof(CON_ETC_SIO_DATA);
	etc_buf = sys_AllocMemory(HEAPID_CONTEST, etc_size + 1);
	MI_CpuCopy8(etc_data, etc_buf, etc_size);
	etc_buf[etc_size] = breeder_no;
	
	//OS_TPrintf("etc_size = %d\n", etc_size);
	//OS_TPrintf("breeder_no = %d\n", etc_buf[etc_size]);
	//OS_TPrintf("���M��etc_data�̍Ō�f�[�^ = %d\n", etc_buf[etc_size - 1]);
	if(CommSendData(CC_ETC_DATA, etc_buf, etc_size + 1) == TRUE){
//		consys->recieve_count = 0;
		ret = TRUE;
	}
	else{
		ret = FALSE;
	}
	sys_FreeMemoryEz(etc_buf);
	return ret;
}
