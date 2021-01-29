//==============================================================================
/**
 * @file	gds_rap_response.c
 * @brief	gds_rap.c�����M�f�[�^�̔����擾������Ɨ�����������
 * @author	matsuda
 * @date	2008.01.13(��)
 */
//==============================================================================
#include "common.h"
#include <dwc.h>
#include "wifi/dwc_rap.h"
#include "savedata\savedata.h"
#include "gds_battle_rec.h"
#include "gds_ranking.h"
#include "gds_boxshot.h"
#include "gds_dressup.h"

#include "savedata/imageclip_data.h"
#include "poketool/boxdata.h"
#include "poketool/monsno.h"
#include "gflib/strbuf_family.h"
#include "savedata/gds_profile.h"

#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/msgdata_util.h"
#include "system/wordset.h"
#include "msgdata/msg.naix"
#include "system/fontproc.h"
#include "gflib/strbuf_family.h"

#include "communication\comm_system.h"
#include "communication\comm_state.h"
#include "communication\comm_def.h"
#include "communication/wm_icon.h"

#include "gds_battle_rec.h"
#include "gds_ranking.h"
#include "gds_boxshot.h"
#include "gds_dressup.h"

#include "gds_rap.h"
#include "gds_rap_response.h"
#include "gds_data_conv.h"

#include "msgdata/msg_wifi_system.h"
#include "savedata/battle_rec.h"



//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�h���X�A�b�v�V���b�g �A�b�v���[�h
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_DressupShot_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_DRESSUPSHOT_REGIST *param;
	int ret = FALSE;
	
	switch(res->Result){
	//!< �o�^����
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_SUCCESS:
		param = (POKE_NET_GDS_RESPONSE_DRESSUPSHOT_REGIST *)(res->Param);
		OS_TPrintf("�h���X�A�b�v�V���b�g�A�b�v���[�h�����B�o�^�R�[�h = %d\n", param->Code);
		ret = TRUE;
		break;
	//!< ���[�U�[�F�؃G���[
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_AUTH:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g�o�^�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	//!< ���łɓo�^����Ă���
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ALREADY:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g�o�^�G���[�I:���ɓo�^����Ă���\n");
		break;
	//!< �s���ȃf�[�^
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ILLEGAL:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g�o�^�G���[�I:�s���f�[�^\n");
		break;
	//!< �s���ȃ��[�U�[�v���t�B�[��
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_ILLEGALPROFILE:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g�o�^�G���[�I:�s���v���t�B�[��\n");
		break;
	//!< ���̑��G���[
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_ERROR_UNKNOWN:
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g�o�^�G���[�I:���̑��̃G���[\n");
		break;
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�h���X�A�b�v�V���b�g �_�E�����[�h
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_DressupShot_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_DRESSUPSHOT_GET *param;
	int ret = FALSE;
	
	switch(res->Result){
	//!< �擾����
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_SUCCESS:
		param = (POKE_NET_GDS_RESPONSE_DRESSUPSHOT_GET *)(res->Param);
		OS_TPrintf("�h���X�A�b�v�V���b�g��M�����B�q�b�g���� = %d\n", param->HitCount);
		ret = TRUE;
		break;
	//!< ���[�U�[�F�؃G���[
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_AUTH:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	//!< �|�P�����ԍ��G���[
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_ILLEGALPOKEMON:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g��M�G���[�I:�|�P�����ԍ��G���[\n");
		break;
	//!< ���̑��G���[
	case POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_GET_ERROR_UNKNOWN:
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�h���X�A�b�v�V���b�g��M�G���[�I:���̑��̃G���[\n");
		break;
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�h���X�A�b�v�V���b�g
 *
 * @param   gdsrap		
 * @param   dress_array		�|�C���^���Z�b�g����z��f�[�^
 * @param   array_max		dress_array�̔z��v�f��
 *
 * @retval  ��M����
 *
 * dress_array[0] �` dress_array[x] �܂Ŏ�M�f�[�^�ւ̃|�C���^���Z�b�g����܂�
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_DressupShot_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, GT_GDS_DRESS_RECV **dress_array, int array_max)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_DRESSUPSHOT_GET *param;
	POKE_NET_GDS_RESPONSE_DRESSUPSHOT_RECVDATA *dress_recv;
	int i, hit_count;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_DRESSUPSHOT_GET *)(res->Param);
	
	hit_count = param->HitCount;
	if(hit_count > array_max){
		hit_count = array_max;
	//	GF_ASSERT(0);	//�p�ӂ��Ă���o�b�t�@������M����������
	}

	dress_recv = (POKE_NET_GDS_RESPONSE_DRESSUPSHOT_RECVDATA *)(param->Data);
	for(i = 0; i < hit_count; i++){
		dress_array[i] = &dress_recv[i].Data;
	}
	for( ; i < array_max; i++){
		dress_array[i] = NULL;
	}

	return hit_count;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�{�b�N�X�V���b�g �A�b�v���[�h
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_Boxshot_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_BOXSHOT_REGIST *param;
	int ret = FALSE;
	
	switch(res->Result){
	//!< �o�^����
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_SUCCESS:
		param = (POKE_NET_GDS_RESPONSE_BOXSHOT_REGIST *)(res->Param);
		OS_TPrintf("�{�b�N�X�V���b�g�A�b�v���[�h�����B�o�^�R�[�h = %d\n", param->Code);
		ret = TRUE;
		break;
	//!< ���[�U�[�F�؃G���[
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_AUTH:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g�o�^�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	//!< ���łɓo�^����Ă���
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ALREADY:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g�o�^�G���[�I:���ɓo�^����Ă���\n");
		break;
	//!< �s���ȃf�[�^
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGAL:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g�o�^�G���[�I:�s���ȃf�[�^\n");
		break;
	//!< �s���ȃ��[�U�[�v���t�B�[��
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGALPROFILE:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g�o�^�G���[�I:�s���v���t�B�[��\n");
		break;
	//!< �s���ȃO���[�v�ԍ�
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_ILLEGALGROUP:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g�o�^�G���[�I:�s���O���[�v�ԍ�\n");
		break;
	//!< ���̑��G���[
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_REGIST_ERROR_UNKNOWN:
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g�o�^�G���[�I:���̑��G���[\n");
		break;
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�{�b�N�X�V���b�g �_�E�����[�h
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_Boxshot_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_BOXSHOT_GET *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_SUCCESS:				//!< �擾����
		param = (POKE_NET_GDS_RESPONSE_BOXSHOT_GET *)(res->Param);
		OS_TPrintf("�{�b�N�X�V���b�g��M�����B�q�b�g���� = %d\n", param->HitCount);
		ret = TRUE;
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_AUTH:			//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_ILLEGALGROUP:	//!< �O���[�v�ԍ��G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g��M�G���[�I�O���[�v�ԍ��G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BOXSHOT_GET_ERROR_UNKNOWN:		//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�{�b�N�X�V���b�g��M�G���[�I:���̑��G���[\n");
		break;
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�{�b�N�X�V���b�g
 *
 * @param   gdsrap		
 * @param   box_array		�|�C���^���Z�b�g����z��f�[�^
 * @param   array_max		dress_array�̔z��v�f��
 *
 * @retval  ��M����
 *
 * box_array[0] �` box_array[x] �܂Ŏ�M�f�[�^�ւ̃|�C���^���Z�b�g����܂�
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_Boxshot_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, GT_BOX_SHOT_RECV **box_array, int array_max)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_BOXSHOT_GET *param;
	POKE_NET_GDS_RESPONSE_BOXSHOT_RECVDATA *box_recv;
	int i, hit_count;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_BOXSHOT_GET *)(res->Param);
	
	hit_count = param->HitCount;
	if(hit_count > array_max){
		hit_count = array_max;
		GF_ASSERT(0);	//�p�ӂ��Ă���o�b�t�@������M����������
	}

	box_recv = (POKE_NET_GDS_RESPONSE_BOXSHOT_RECVDATA *)(param->Data);
	for(i = 0; i < hit_count; i++){
		box_array[i] = &box_recv[i].Data;
	}
	for( ; i < array_max; i++){
		box_array[i] = NULL;
	}
	
	return hit_count;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�����L���O�^�C�v �_�E�����[�h
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_RankingType_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_RANKING_GETTYPE *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_SUCCESS:				//!< �擾����
		param = (POKE_NET_GDS_RESPONSE_RANKING_GETTYPE *)(res->Param);
		OS_TPrintf("�����L���O�^�C�v��M�����B\n");
		ret = TRUE;
	#if 0
		{
			int i;
			GT_RANKING_TYPE_RECV *recv = &param->Data;
			for(i = 0; i < GT_RANKING_WEEK_NUM; i++){
				OS_TPrintf("�J�Ò��̃^�C�v ����%d = %d\n", i, recv->ranking_type[i]);
			}
		}
	#endif
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_ERROR_AUTH:			//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�^�C�v��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_GETTYPE_ERROR_UNKNOWN:		//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�^�C�v��M�G���[�I:���̑��̃G���[\n");
		break;
	}
	
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�J�Ò��̃����L���O�^�C�v
 *
 * @param   gdsrap		
 * @param   ranking_type_recv		��M�f�[�^�ւ̃|�C���^�����
 */
//--------------------------------------------------------------
void GDS_RAP_RESPONSE_RankingType_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, GT_RANKING_TYPE_RECV **ranking_type_recv)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_RANKING_GETTYPE *param;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_RANKING_GETTYPE *)(res->Param);
	
	*ranking_type_recv = &param->Data;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�����L���O�X�V �_�E�����[�h
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_RankingUpdate_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_RANKING_UPDATE *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_SUCCESS:			//!< �X�V����
		param = (POKE_NET_GDS_RESPONSE_RANKING_UPDATE *)(res->Param);
		OS_TPrintf("�����L���O�X�V��M����\n");
		ret = TRUE;
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_AUTH:		//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�X�V��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALTYPE:	//!< �����L���O�^�C�v�G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�X�V��M�G���[�I:�����L���O�^�C�v�G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALDATA:	//!< �s���ȃf�[�^�G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�X�V��M�G���[�I:�s���ȃf�[�^\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_ILLEGALPROFILE:	//!< �s���ȃv���t�B�[��
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�X�V��M�G���[�I:�s���ȃv���t�B�[��\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_RANKING_UPDATE_ERROR_UNKNOWN:			//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�����L���O�X�V��M�G���[�I:���̑��̃G���[\n");
		break;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�����L���O�X�V(��T�ƍ��T�̌���)
 *
 * @param   gdsrap		
 * @param   last_week		��T�̌��ʂւ̃|�C���^�����
 * @param   this_week		���T�̌��ʂւ̃|�C���^�����
 */
//--------------------------------------------------------------
void GDS_RAP_RESPONSE_RankingUpdate_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, GT_LAST_WEEK_RANKING_ALL_RECV **last_week, GT_THIS_WEEK_RANKING_DATA_ALL_RECV **this_week)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_RANKING_UPDATE *param;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_RANKING_UPDATE *)(res->Param);
	
	*last_week = &param->LastWeek;
	*this_week = &param->ThisWeek;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�o�g���r�f�I�o�^
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideo_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_SUCCESS:		//!< �o�^����
		param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST *)(res->Param);
		OS_TPrintf("�o�g���r�f�I�o�^��M�����F�o�^�R�[�h%d\n", param->Code);
		ret = TRUE;
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_AUTH:		//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ALREADY:	//!< ���łɓo�^����Ă���
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:���ɓo�^����Ă���\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGAL:	//!< �s���ȃf�[�^
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:�s���f�[�^\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPROFILE:	//!< �s���ȃ��[�U�[�v���t�B�[��
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:�s���ȃ��[�U�[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_UNKNOWN:	//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:���̑��̃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPOKEMON_MINE: //�����̃|�P�����ŃG���[
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:�����̃|�P�������s��\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_REGIST_ERROR_ILLEGALPOKEMON_OTHER:	//����̃|�P�����ŃG��
		OS_TPrintf("�o�g���r�f�I�o�^��M�G���[�I:���̑��̃G���[\n");
		break;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�擾�F�o�g���r�f�I�o�^
 *
 * @param   gdsrap		
 * 
 * @retval	�o�^�R�[�h
 */
//--------------------------------------------------------------
u64 GDS_RAP_RESPONSE_BattleVideo_Upload_DataGet(GDS_RAP_WORK *gdsrap)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST *param;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST *)(res->Param);
	return param->Code;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�o�g���r�f�I����
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideo_Search_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_SUCCESS:		//!< �擾����
		param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH *)(res->Param);
		OS_TPrintf("�o�g���r�f�I������M�����F�q�b�g����=%d\n", param->HitCount);
		ret = TRUE;
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_AUTH:		//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I������M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_ILLEGALPARAM:	//!< �����p�����[�^�G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I������M�G���[�I:�����p�����[�^�G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_SEARCH_ERROR_UNKNOWN:	//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I������M�G���[�I:���̑��̃G���[\n");
		break;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�o�g���r�f�I����
 *
 * @param   gdsrap		
 * @param   outline_array	�|�C���^���Z�b�g����z��f�[�^
 * @param   array_max		outline_array�̔z��v�f��
 *
 * @retval  ��M����
 *
 * outline_array[0] �` outline_array[x] �܂Ŏ�M�f�[�^�ւ̃|�C���^���Z�b�g����܂�
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideoSearch_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, GT_BATTLE_REC_OUTLINE_RECV **outline_array, int array_max)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH *param;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_RECVDATA *outline_recv;
	int i, hit_count;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH *)(res->Param);
	
	hit_count = param->HitCount;
	if(hit_count > array_max){
		hit_count = array_max;
		GF_ASSERT(0);	//�p�ӂ��Ă���o�b�t�@������M����������
	}

	outline_recv = (POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_RECVDATA *)(param->Data);
	for(i = 0; i < hit_count; i++){
		outline_array[i] = &outline_recv[i].Data;
		if(outline_recv[i].Code != outline_array[i]->head.data_number){
			//�w�b�_�ƃf�[�^�i���o�[���Ⴄ�ꍇ�̓w�b�_�Ƀi���o�[�������ĂȂ��̂ł������œ����
			outline_array[i]->head.data_number = outline_recv[i].Code;
			//CRC�쐬
			outline_array[i]->head.crc.crc16ccitt_hash = SaveData_CalcCRC(gdsrap->savedata, 
				&outline_array[i]->head, 
				sizeof(GT_BATTLE_REC_HEADER) - GT_GDS_CRC_SIZE - sizeof(outline_recv[i].Code));
		}
	}
	for( ; i < array_max; i++){
		outline_array[i] = NULL;
	}
	
	return hit_count;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^���R�s�[���܂��F�o�g���r�f�I����
 *
 * @param   gdsrap		
 * @param   outline_array	�|�C���^���Z�b�g����z��f�[�^
 * @param   array_max		outline_array�̔z��v�f��
 *
 * @retval  ��M����
 *
 * outline_array[0] �` outline_array[x] �܂Ŏ�M�f�[�^�ւ̃|�C���^���Z�b�g����܂�
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideoSearch_Download_DataCopy(GDS_RAP_WORK *gdsrap, GT_BATTLE_REC_OUTLINE_RECV *outline_array, int array_max)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH *param;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_RECVDATA *outline_recv;
	int i, hit_count;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH *)(res->Param);
	
	hit_count = param->HitCount;
	if(hit_count > array_max){
		hit_count = array_max;
		GF_ASSERT(0);	//�p�ӂ��Ă���o�b�t�@������M����������
	}

	MI_CpuClear8(outline_array, sizeof(GT_BATTLE_REC_OUTLINE_RECV) * array_max);
	outline_recv = (POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH_RECVDATA *)(param->Data);
	for(i = 0; i < hit_count; i++){
		if(outline_recv[i].Code != outline_recv[i].Data.head.data_number){
			//�w�b�_�ƃf�[�^�i���o�[���Ⴄ�ꍇ�̓w�b�_�Ƀi���o�[�������ĂȂ��̂ł������œ����
			outline_recv[i].Data.head.data_number = outline_recv[i].Code;
			//CRC�쐬
			outline_recv[i].Data.head.crc.crc16ccitt_hash = SaveData_CalcCRC(gdsrap->savedata, 
				&outline_recv[i].Data.head, 
				sizeof(GT_BATTLE_REC_HEADER) - GT_GDS_CRC_SIZE - sizeof(outline_recv[i].Code));
		}
		outline_array[i] = outline_recv[i].Data;
	}
	
	return hit_count;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�o�g���r�f�I�f�[�^�擾
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideo_Data_Download(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
	POKE_NET_GDS_RESPONSE_BATTLEDATA_GET *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_SUCCESS:			//!< �擾����
		param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_GET *)(res->Param);
		OS_TPrintf("�o�g���r�f�I�f�[�^�擾��M����\n");
		ret = TRUE;
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_AUTH:		//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�f�[�^�擾��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_ILLEGALCODE:	//!< �R�[�h�G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�f�[�^�擾��M�G���[�I:�R�[�h�G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_GET_ERROR_UNKNOWN:		//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I�f�[�^�擾��M�G���[�I:���̑��̃G���[\n");
		break;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�o�g���r�f�I�f�[�^�擾
 *
 * @param   gdsrap		
 * @param   rec			�^��f�[�^�������Ă���f�[�^�ւ̃|�C���^�����
 *
 * @retval  �o�^�R�[�h
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideoData_Download_RecvPtr_Set(GDS_RAP_WORK *gdsrap, GT_BATTLE_REC_RECV **rec)
{
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_GET *param;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_GET *)(res->Param);
	
	*rec = &param->Data;
	if(param->Code != param->Data.head.data_number){
		//�w�b�_�ƃf�[�^�i���o�[���Ⴄ�ꍇ�̓w�b�_�Ƀi���o�[�������ĂȂ��̂ł������œ����
		param->Data.head.data_number = param->Code;
		//CRC�쐬
		(*rec)->head.crc.crc16ccitt_hash = SaveData_CalcCRC(gdsrap->savedata, &(*rec)->head, 
			sizeof(GT_BATTLE_REC_HEADER) - GT_GDS_CRC_SIZE - sizeof(param->Code));
	}
	
	return param->Code;
}

//--------------------------------------------------------------
/**
 * @brief   ���X�|���X�f�[�^���߁F�o�g���r�f�I���C�ɓ���o�^
 *
 * @param   gdsrap		
 * @param   res		
 *
 * @retval  TRUE:����
 * @retval  FALSE:���炩�̃G���[������(���e��res->Result�Q��)
 */
//--------------------------------------------------------------
int GDS_RAP_RESPONSE_BattleVideo_Favorite_Upload(GDS_RAP_WORK *gdsrap, POKE_NET_RESPONSE *res)
{
//	POKE_NET_GDS_RESPONSE_BATTLEDATA_FAVORITE *param;
	int ret = FALSE;
	
	switch(res->Result){
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_SUCCESS:			//!< ���C�ɓ���o�^����
//		param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_FAVORITE *)(res->Param);
		OS_TPrintf("�o�g���r�f�I�f�[�^���C�ɓ���o�^��M����\n");
		ret = TRUE;
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_ERROR_AUTH:		//!< ���[�U�[�F�؃G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I���C�ɓ���o�^��M�G���[�I:���[�U�[�F�؃G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_ERROR_ILLEGALCODE:	//!< �R�[�h�G���[
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I���C�ɓ���o�^��M�G���[�I:�R�[�h�G���[\n");
		break;
	case POKE_NET_GDS_RESPONSE_RESULT_BATTLEDATA_FAVORITE_ERROR_UNKNOWN:	//!< ���̑��G���[
	default:
		//��check�@�G���[���Ƃɐ�p�̃P�A�ƃ��b�Z�[�W������
		OS_TPrintf("�o�g���r�f�I���C�ɓ���o�^��M�G���[�I:���̑��̃G���[\n");
		break;
	}

	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�o�b�t�@����f�[�^�������Ă��镔���̃|�C���^���擾�F�o�g���r�f�I���C�ɓ���o�^
 *
 * @param   gdsrap		
 *
 * @retval  �o�^�R�[�h
 */
//--------------------------------------------------------------
void GDS_RAP_RESPONSE_BattleVideoFavorite_Upload_RecvPtr_Set(GDS_RAP_WORK *gdsrap)
{
	//����A�擾������͉̂�������
	return;
}
