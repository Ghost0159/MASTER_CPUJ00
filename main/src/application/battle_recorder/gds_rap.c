//==============================================================================
/**
 * @file	gds_rap.h
 * @brief	GDS���C�u���������b�p�[��������
 * @author	matsuda
 * @date	2008.01.09(��)
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
#include "savedata/gds_profile_types.h"

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
#include "communication\communication.h"

#include "gds_battle_rec.h"
#include "gds_ranking.h"
#include "gds_boxshot.h"
#include "gds_dressup.h"

#include "gds_rap.h"
#include "gds_rap_response.h"
#include "gds_data_conv.h"

#include "msgdata/msg_wifi_system.h"
#include "savedata/playtime.h"
#include "savedata/battle_rec.h"

#include "battle/battle_common.h"

#include "gflib/strcode.h"
#include "gflib\msg_print.h"
#include "system\fontproc.h"
#include "system\msgdata.h"
#include "msgdata/msg.naix"
#include "msgdata/msg_battle_rec.h"


//==============================================================================
//	�萔��`
//==============================================================================
///DWC�ɓn�����[�N�̃T�C�Y
#define MYDWC_HEAPSIZE		0x20000

///�G���[���b�Z�[�W�W�J�p�o�b�t�@�̃T�C�Y
#define DWC_ERROR_BUF_NUM		(16*8*2)

///���M�O�̃E�F�C�g(���M���Ă��郁�b�Z�[�W��������ׂɃE�F�C�g�����Ă���)
#define GDS_SEND_BEFORE_WAIT	(60)

//--------------------------------------------------------------
//	�T�u�V�[�P���X�̖߂�l
//--------------------------------------------------------------
enum{
	SUBSEQ_CONTINUE,		///<�T�u�V�[�P���X�F�ێ�
	SUBSEQ_NEXT,			///<�T�u�V�[�P���X�F���̃T�u�V�[�P���X��
	SUBSEQ_PROCCESS_CHANGE,	///<�T�u�V�[�P���X�F�T�u�V�[�P���X�e�[�u���ύX
};

//==============================================================================
//	�\���̒�`
//==============================================================================
///�T�u�v���Z�X�V�[�P���X�̊֐��^��`
typedef int (*SUBPROCESS_FUNC)(GDS_RAP_WORK *, GDS_RAP_SUB_PROCCESS_WORK *sub_work);


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static int GDSRAP_SEQ_GDS_Release(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work);
static int GDSRAP_SEQ_Error_Cleanup(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work);
static int GDSRAP_SEQ_Error_DWC_Message(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work);
static int GDSRAP_SEQ_Error_DWC_MessageWait(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work);

static int GDSRAP_MAIN_Send(GDS_RAP_WORK *gdsrap);
static int GDSRAP_MAIN_Recv(GDS_RAP_WORK *gdsrap);
static int GDSRAP_ProccessCheck(GDS_RAP_WORK *gdsrap);

static void _systemMessagePrint(GDS_RAP_WORK *gdsrap, STRBUF *str_buf);
static void errorDisp(GDS_RAP_WORK *gdsrap, int type, int code);

static int Local_GetResponse(GDS_RAP_WORK *gdsrap);

static void * LIB_Heap_Init(int heap_id);
static void LIB_Heap_Exit(void);

static BOOL RecvSubProccess_Normal(void *work_gdsrap, void *work_recv_sub_work);
static BOOL RecvSubProccess_VideoSendFlagResetSave(void *work_gdsrap, void *work_recv_sub_work);
static BOOL RecvSubProccess_DataNumberSetSave(void *work_gdsrap, void *work_recv_sub_work);
static BOOL RecvSubProccess_DressSendFlagResetSave(void *work_gdsrap, void *work_recv_sub_work);
static BOOL RecvSubProccess_BoxSendFlagResetSave(void *work_gdsrap, void *work_recv_sub_work);
static BOOL RecvSubProccess_SystemError(void *work_gdsrap, void *work_recv_sub_work);

static SAVE_RESULT GdsDivSave(GDS_RAP_WORK *gdsrap);

//==============================================================================
//	�V�[�P���X�e�[�u��
//==============================================================================
///Wifi�ڑ��G���[�������̋����ؒf����
static const SUBPROCESS_FUNC SubSeq_WifiError[] = {
	GDSRAP_SEQ_Error_Cleanup,
	GDSRAP_SEQ_GDS_Release,
	GDSRAP_SEQ_Error_DWC_Message,
	GDSRAP_SEQ_Error_DWC_MessageWait,
	NULL,	//�I�[
};

///�T�[�o�[�T�[�r�X�G���[�������̋����ؒf����(�l�b�g�͐ڑ��o���Ă��邪�A�T�[�o�[���~�܂��Ă���ꍇ)
static const SUBPROCESS_FUNC SubSeq_ServerServiceError[] = {
//	GDSRAP_SEQ_Error_ServerService_Message,
//	GDSRAP_SEQ_Error_ServerService_MessageWait,
	NULL,	//�I�[
};

//--------------------------------------------------------------
//	�T�u�v���Z�X�e�[�u��
//--------------------------------------------------------------
///�T�u�v���Z�X�e�[�u��
///		��GDSRAP_PROCESS_REQ�ƕ��т𓯂��ɂ��Ă������ƁI�I
static const SUBPROCESS_FUNC * const GDSRapSeqTbl[] = {
	NULL,
	
	//�ȉ��A�V�X�e�������ł̂݌Ăяo��
	SubSeq_WifiError,			//GDSRAP_PROCESS_REQ_WIFI_ERROR
	SubSeq_ServerServiceError,	//GDSRAP_PROCESS_REQ_SERVER_ERROR
};



//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   GDS���C�u��������������
 *
 * @param   gdsrap				GDS���b�p�[���[�N�ւ̃|�C���^
 * @param   init_data			�������f�[�^
 *
 * @retval  TRUE
 */
//--------------------------------------------------------------
int GDSRAP_Init(GDS_RAP_WORK *gdsrap, const GDSRAP_INIT_DATA *init_data)
{
	int ret;
	
	MI_CpuClear8(gdsrap, sizeof(GDS_RAP_WORK));
	gdsrap->heap_id = init_data->heap_id;
	gdsrap->savedata = init_data->savedata;
	gdsrap->response_callback = init_data->response_callback;
	gdsrap->callback_work = init_data->callback_work;
	gdsrap->callback_error_msg_wide = init_data->callback_error_msg_wide;
	gdsrap->laststat = -1;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_LAST;
	gdsrap->recv_wait_req = POKE_NET_GDS_REQCODE_LAST;
	
	gdsrap->response = sys_AllocMemory(init_data->heap_id, POKE_NET_GDS_GetResponseMaxSize(-1));
	MI_CpuClear8(gdsrap->response, POKE_NET_GDS_GetResponseMaxSize(-1));
	OS_TPrintf("���X�|���X�o�b�t�@�̃T�C�Y��%x\n", POKE_NET_GDS_GetResponseMaxSize(-1));

	//���b�Z�[�W�}�l�[�W���쐬
	gdsrap->msgman_wifisys = MSGMAN_Create(MSGMAN_TYPE_NORMAL, 
		ARC_MSG, NARC_msg_wifi_system_dat, init_data->heap_id);
	gdsrap->wordset = WORDSET_Create(init_data->heap_id);	//�P��o�b�t�@�쐬
	gdsrap->ErrorString = STRBUF_Create(DWC_ERROR_BUF_NUM, init_data->heap_id);
	
	//��check�@�b��q�[�v�쐬
	gdsrap->areanaLo = LIB_Heap_Init(init_data->heap_id);

	gdsrap->pokenet_auth.PID = init_data->gs_profile_id;
	gdsrap->pokenet_auth.ROMCode = PM_VERSION;
	gdsrap->pokenet_auth.LangCode = PM_LANG;
	ret = POKE_NET_GDS_Initialize(&gdsrap->pokenet_auth);
	GF_ASSERT(ret == TRUE);		//�������Ɏ��s���鎖�͂��肦�Ȃ��͂�
	gdsrap->gdslib_initialize = TRUE;

	gdsrap->comm_initialize_ok = TRUE;
	
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   GDS���C�u�����j��
 *
 * @param   gdsrap		
 */
//--------------------------------------------------------------
void GDSRAP_Exit(GDS_RAP_WORK *gdsrap)
{
	//GDS���C�u�������
	POKE_NET_GDS_Release();
	gdsrap->gdslib_initialize = FALSE;

	//���b�Z�[�W�}�l�[�W���폜
	STRBUF_Delete(gdsrap->ErrorString);
	WORDSET_Delete(gdsrap->wordset);
	MSGMAN_Delete(gdsrap->msgman_wifisys);

	sys_FreeMemoryEz(gdsrap->response);

	//��check�@�b��q�[�v���
	LIB_Heap_Exit();
	sys_FreeMemoryEz(gdsrap->areanaLo);
}

//--------------------------------------------------------------
/**
 * @brief   �|�P����WifiGDS���C�u�����̉�����s���܂�
 *
 * @param   gdsrap		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
static int GDSRAP_SEQ_GDS_Release(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work)
{
	POKE_NET_GDS_Release();
	
	gdsrap->gdslib_initialize = FALSE;
	
	return SUBSEQ_NEXT;
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ��G���[�������������̐ؒf�������s���܂�
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int GDSRAP_SEQ_Error_Cleanup(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work)
{
	OS_TPrintf("----�G���[�ׁ̈AWIFI�����ؒf\n");
	
	DWC_ClearError();
	DWC_CleanupInet();
	CommStateWifiDPWEnd();
	
#if 0	//GDS�ł͂�������Dpw_Tr_Init���g���Ă��Ȃ��̂Ŗ���
	//��ʂ𔲂�����2�x�A���ŁuE���[���ݒ�v������Wifi�Ɍq���悤�Ƃ����
	//"dpw_tr.c:150 Panic:dpw tr is already initialized."
	//�̃G���[���o��̂ł�����Ƃ��̏I���֐����ĂԂ悤�ɂ��� 2007.10.26(��) matsuda
	Dpw_Tr_End();
#endif

	gdsrap->connect_success = FALSE;

	return SUBSEQ_NEXT;
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ��G���[���b�Z�[�W�\������
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int GDSRAP_SEQ_Error_DWC_Message(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work)
{
	int type, msgno;
	
    type =  mydwc_errorType(-gdsrap->ErrorCode, gdsrap->ErrorType);
	OS_Printf("error code = %d, type = %d\n", gdsrap->ErrorCode, type);

    errorDisp(gdsrap, type, -gdsrap->ErrorCode);
	
	return SUBSEQ_NEXT;
}

//--------------------------------------------------------------
/**
 * @brief   Wifi�ڑ��G���[���b�Z�[�W�\���F����L�[�ł̏I���҂�
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int GDSRAP_SEQ_Error_DWC_MessageWait(GDS_RAP_WORK *gdsrap, GDS_RAP_SUB_PROCCESS_WORK *sub_work)
{
	if((sys.trg & PAD_BUTTON_DECIDE) || (sys.trg & PAD_BUTTON_CANCEL)){
		_systemMessagePrint(gdsrap, NULL);
		return SUBSEQ_NEXT;
	}
	return SUBSEQ_CONTINUE;
}



//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�h���X�A�b�v�V���b�g�o�^
 *
 * @param   gdsrap			
 * @param   gt_profile		
 * @param   gt_dress		
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_DressupUpload(GDS_RAP_WORK *gdsrap, GDS_PROFILE_PTR gpp, IMC_TELEVISION_SAVEDATA * dress)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	//GDS�v���t�B�[����GT_GDS�v���t�B�[���֕ϊ�
	GDS_CONV_GDSProfile_to_GTGDSProfile(
		gdsrap->savedata, gpp, &gdsrap->send_buf.gt_dress_send.profile);
	//GDS�h���X��GT_GDS�h���X�֕ϊ�
	GDS_CONV_Dress_to_GTDress(gdsrap->savedata, dress, &gdsrap->send_buf.gt_dress_send.dressup);
	
	gdsrap->send_before_wait = GDS_SEND_BEFORE_WAIT;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_DRESSUPSHOT_REGIST;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�h���X�A�b�v�V���b�g �_�E�����[�h
 *
 * @param   gdsrap			
 * @param   monsno			��M����|�P�����ԍ�
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_DressupDownload(GDS_RAP_WORK *gdsrap, int monsno)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	gdsrap->send_buf.sub_para.dressup.recv_monsno = monsno;
	
	gdsrap->send_req = POKE_NET_GDS_REQCODE_DRESSUPSHOT_GET;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�{�b�N�X�V���b�g �A�b�v���[�h
 *
 * @param   gdsrap		
 * @param   category_no		�o�^����J�e�S���[�ԍ�
 * @param   gpp				GDS�v���t�B�[���ւ̃|�C���^
 * @param   boxdata			�{�b�N�X�f�[�^�ւ̃|�C���^
 * @param   tray_number		�{�b�N�X�̃g���C�ԍ�
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BoxshotUpload(GDS_RAP_WORK *gdsrap, int category_no, GDS_PROFILE_PTR gpp, const BOX_DATA *boxdata, int tray_number)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	gdsrap->send_buf.sub_para.box.category_no = category_no;
	gdsrap->send_buf.sub_para.box.tray_number = tray_number;	//���M�ς݃t���O��߂��וK�v

	//GDS�v���t�B�[����GT_GDS�v���t�B�[���֕ϊ�
	GDS_CONV_GDSProfile_to_GTGDSProfile(
		gdsrap->savedata, gpp, &gdsrap->send_buf.gt_box_send.profile);
	//�{�b�N�X�f�[�^��GT�{�b�N�X�f�[�^�֕ϊ�
	GDS_CONV_Box_to_GTBox(gdsrap->savedata, boxdata, tray_number, 
		&gdsrap->send_buf.gt_box_send.box_shot, gdsrap->heap_id);
	
	//GT�{�b�N�X�f�[�^�ɃJ�e�S���[�ԍ����Z�b�g
	GDS_GTBoxShot_SetCategoryNo(gdsrap->savedata, 
		&gdsrap->send_buf.gt_box_send.box_shot, category_no);
	
	gdsrap->send_before_wait = GDS_SEND_BEFORE_WAIT;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BOXSHOT_REGIST;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�{�b�N�X�V���b�g �_�E�����[�h
 *
 * @param   gdsrap			
 * @param   category_no		��M����J�e�S���[�ԍ�
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BoxshotDownload(GDS_RAP_WORK *gdsrap, int category_no)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	gdsrap->send_buf.sub_para.box.category_no = category_no;
	
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BOXSHOT_GET;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�J�Ò��̃����L���O�^�C�v�@�_�E�����[�h
 *
 * @param   gdsrap		
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_RankingTypeDownload(GDS_RAP_WORK *gdsrap)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}
	
	gdsrap->send_req = POKE_NET_GDS_REQCODE_RANKING_GETTYPE;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�����L���O�X�V(�����̃f�[�^���M�����ʎ�M)
 *
 * @param   gdsrap		
 * @param   gpp						GDS�v���t�B�[���ւ̃|�C���^
 * @param   ranking_mydata[]		�����̃����L���O�f�[�^(�z��ւ̃|�C���^)
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_RankingUpdate(GDS_RAP_WORK *gdsrap, GDS_PROFILE_PTR gpp, GT_RANKING_MYDATA ranking_mydata[])
{
	PLAYTIME *playtime;
	int i;
	
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	//�v���C���ԃZ�b�g
	playtime = SaveData_GetPlayTime(gdsrap->savedata);
	MI_CpuCopy8(playtime, &gdsrap->send_buf.gt_ranking_mydata_send.playtime, sizeof(GT_PLAYTIME));

	//GDS�v���t�B�[���������L���O�v���t�B�[���֕ϊ�
	GDS_CONV_GDSProfile_to_GTRankingProfile(gdsrap->savedata, gpp, 
		&gdsrap->send_buf.gt_ranking_mydata_send.profile);
	
	//�����L���O�f�[�^
	for(i = 0; i < GT_RANKING_WEEK_NUM; i++){
		gdsrap->send_buf.gt_ranking_mydata_send.my_data[i] = ranking_mydata[i];
	}

	gdsrap->send_req = POKE_NET_GDS_REQCODE_RANKING_UPDATE;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���f�[�^�o�^
 *
 * @param   gdsrap		
 * @param   gpp						GDS�v���t�B�[���ւ̃|�C���^
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 * 
 * �^��f�[�^�̓T�C�Y������ȈׁA�R�s�[�����ɁAbrs�œǂݍ���ł���f�[�^�����̂܂ܑ��M���܂��B
 * brs�Ƀf�[�^��W�J���Ă��炱�̊֐����g�p����悤�ɂ��Ă��������B
 *
 * �I�I���̊֐��g�p��Abrs�ɓǂݍ���ł���v���t�B�[���f�[�^�̏��������A
 * 	   �^��f�[�^�{�̂̈Í������s���̂ł��̂܂܃Z�[�u������Đ����Ȃ��悤�ɒ��ӁI�I
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideoUpload(GDS_RAP_WORK *gdsrap, GDS_PROFILE_PTR gpp)
{
	GDS_PROFILE_PTR profile_ptr;
	
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	//�^��f�[�^��GDS�v���t�B�[����GT�n�ƑS�������^�̂͂�
	GF_ASSERT((BattleRec_GetWorkSize()-sizeof(EX_CERTIFY_SAVE_KEY)) == sizeof(GT_BATTLE_REC_SEND));
	GF_ASSERT(sizeof(GT_GDS_PROFILE) == sizeof(GDS_PROFILE));

	//�^��f�[�^�͋���ȈׁA�R�s�[�����ɁA���̂܂�brs�̃f�[�^�𑗐M����
	gdsrap->send_buf.gt_battle_rec_send_ptr = (GT_BATTLE_REC_SEND *)BattleRec_RecWorkAdrsGet();
	//brs�ɓW�J����Ă���^��f�[�^�{�͕̂���������Ă���̂ŁA���M����O�ɍēx�Í�������
	BattleRec_GDS_SendData_Conv(gdsrap->savedata);
	
	//GDS�v���t�B�[���̂݁A�ŐV�̂�K�p����ׁA�㏑������
	profile_ptr = BattleRec_GDSProfilePtrGet();
	MI_CpuCopy8(gpp, profile_ptr, sizeof(GT_GDS_PROFILE));
	
	gdsrap->send_before_wait = GDS_SEND_BEFORE_WAIT;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST;
	return TRUE;
}

///�����I�Ƀt���[���[�h�̎��ȏЉ�b�Z�[�W���Z�b�g����
void DEBUG_GDSRAP_SendVideoProfileFreeWordSet(GDS_RAP_WORK *gdsrap, u16 *set_code)
{
#ifdef PM_DEBUG
	GDS_PROFILE_PTR profile_ptr;

	profile_ptr = BattleRec_GDSProfilePtrGet();
	MI_CpuCopy16(set_code, profile_ptr->event_self_introduction, GT_EVENT_SELF_INTRO*2);
	profile_ptr->message_flag = 1;
	DEBUG_BattleRec_SecureFlagSet(gdsrap->savedata);
#endif
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I�_�E�����[�h(�ڍ׌���)
 *
 * @param   gdsrap		
 * @param   monsno			�|�P�����ԍ�(�w��Ȃ��FGT_BATTLE_REC_SEARCH_MONSNO_NONE)
 * @param   battle_mode		�����{��(�w��Ȃ��F�N���A�O=GT_BATTLE_MODE_EXCLUSION_FRONTIER
 * 											   �N���A��=GT_BATTLE_REC_SEARCH_BATTLE_MODE_NONE)
 * @param   country_code	���R�[�h(�w��Ȃ��FGT_BATTLE_REC_SEARCH_COUNTRY_CODE_NONE)
 * @param   local_code		�n���R�[�h(�w��Ȃ��FGT_BATTLE_REC_SEARCH_LOCAL_CODE_NONE)
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideoSearchDownload(GDS_RAP_WORK *gdsrap, u16 monsno, u8 battle_mode, u8 country_code, u8 local_code)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	MI_CpuClear8(&gdsrap->send_buf.gt_battle_rec_search, sizeof(GT_BATTLE_REC_SEARCH_SEND));
	gdsrap->send_buf.gt_battle_rec_search.monsno = monsno;
	gdsrap->send_buf.gt_battle_rec_search.battle_mode = battle_mode;
	gdsrap->send_buf.gt_battle_rec_search.country_code = country_code;
	gdsrap->send_buf.gt_battle_rec_search.local_code = local_code;
	gdsrap->send_buf.gt_battle_rec_search.server_version = BATTLE_SERVER_VERSION;

	gdsrap->send_req_option = POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_CONDITION;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I�_�E�����[�h(�ŐV30��:�R���V�A���̂�)
 *
 * @param   gdsrap		
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideoNewDownload_ColosseumOnly(GDS_RAP_WORK *gdsrap)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	MI_CpuClear8(&gdsrap->send_buf.gt_battle_rec_search, sizeof(GT_BATTLE_REC_SEARCH_SEND));
	gdsrap->send_buf.gt_battle_rec_search.monsno = GT_BATTLE_REC_SEARCH_MONSNO_NONE;
	gdsrap->send_buf.gt_battle_rec_search.battle_mode = GT_BATTLE_MODE_EXCLUSION_FRONTIER;
	gdsrap->send_buf.gt_battle_rec_search.country_code = GT_BATTLE_REC_SEARCH_COUNTRY_CODE_NONE;
	gdsrap->send_buf.gt_battle_rec_search.local_code = GT_BATTLE_REC_SEARCH_LOCAL_CODE_NONE;
	gdsrap->send_buf.gt_battle_rec_search.server_version = BATTLE_SERVER_VERSION;

	gdsrap->send_req_option = POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_CONDITION;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I�_�E�����[�h(�ŐV30��)
 *
 * @param   gdsrap		
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideoNewDownload(GDS_RAP_WORK *gdsrap)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	MI_CpuClear8(&gdsrap->send_buf.gt_battle_rec_search, sizeof(GT_BATTLE_REC_SEARCH_SEND));
	gdsrap->send_buf.gt_battle_rec_search.monsno = GT_BATTLE_REC_SEARCH_MONSNO_NONE;
	gdsrap->send_buf.gt_battle_rec_search.battle_mode = GT_BATTLE_REC_SEARCH_BATTLE_MODE_NONE;
	gdsrap->send_buf.gt_battle_rec_search.country_code = GT_BATTLE_REC_SEARCH_COUNTRY_CODE_NONE;
	gdsrap->send_buf.gt_battle_rec_search.local_code = GT_BATTLE_REC_SEARCH_LOCAL_CODE_NONE;
	gdsrap->send_buf.gt_battle_rec_search.server_version = BATTLE_SERVER_VERSION;

	gdsrap->send_req_option = POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_CONDITION;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I�_�E�����[�h(�R���V�A�����C�ɓ��胉���L���O���30��)
 *
 * @param   gdsrap		
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideoFavoriteDownload_Colosseum(GDS_RAP_WORK *gdsrap)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	MI_CpuClear8(&gdsrap->send_buf.gt_battle_rec_ranking_search, 
		sizeof(GT_BATTLE_REC_RANKING_SEARCH_SEND));
	gdsrap->send_buf.gt_battle_rec_ranking_search.server_version = BATTLE_SERVER_VERSION;

	gdsrap->send_req_option = POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_RANKING;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I�_�E�����[�h(�t�����e�B�A���C�ɓ��胉���L���O���30��)
 *
 * @param   gdsrap		
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideoFavoriteDownload_Frontier(GDS_RAP_WORK *gdsrap)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	MI_CpuClear8(&gdsrap->send_buf.gt_battle_rec_ranking_search, 
		sizeof(GT_BATTLE_REC_RANKING_SEARCH_SEND));
	gdsrap->send_buf.gt_battle_rec_ranking_search.server_version = BATTLE_SERVER_VERSION;

	gdsrap->send_req_option = POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_EXRANKING;
	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I�擾
 *
 * @param   gdsrap		
 * @param   data_number	�f�[�^�i���o�[
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideo_DataDownload(GDS_RAP_WORK *gdsrap, u64 data_number)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	gdsrap->send_buf.sub_para.rec.data_number = data_number;

	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_GET;
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M���N�G�X�g�F�o�g���r�f�I���C�ɓ���o�^
 *
 * @param   gdsrap		
 * @param   data_number	�f�[�^�i���o�[
 *
 * @retval  TRUE�F���N�G�X�g���󂯕t����
 * @retval  FALSE�F�󂯕t�����Ȃ�����
 */
//--------------------------------------------------------------
int GDSRAP_Tool_Send_BattleVideo_FavoriteUpload(GDS_RAP_WORK *gdsrap, u64 data_number)
{
	if(GDSRAP_MoveStatusAllCheck(gdsrap) == FALSE){
		return FALSE;
	}

	gdsrap->send_buf.sub_para.rec.data_number = data_number;

	gdsrap->send_req = POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE;
	return TRUE;
}




//--------------------------------------------------------------
/**
 * @brief   ���C������
 *
 * @param   gdsrap		
 *
 * @retval  
 *
 *
 */
//--------------------------------------------------------------
int GDSRAP_Main(GDS_RAP_WORK *gdsrap)
{
	int ret;

#if 0	//gds_main.c �ŌĂ΂��悤�ɂȂ���
	if(gdsrap->comm_initialize_ok){
		// ��M���x�����N�𔽉f������
		DWC_UpdateConnection();

		// Dpw_Tr_Main() �����͗�O�I�ɂ��ł��Ăׂ�
	//	Dpw_Tr_Main();

		// �ʐM��Ԃ��m�F���ăA�C�R���̕\����ς���
		WirelessIconEasy_SetLevel( WM_LINK_LEVEL_3 - DWC_GetLinkLevel() );
	}
#endif

	//�l�b�g�ڑ����݂̂̏���
	if(gdsrap->gdslib_initialize == TRUE){
//		DWC_ProcessInet();
		
		//���M�f�[�^�m�F
		if(gdsrap->send_req != POKE_NET_GDS_REQCODE_LAST){
			if(gdsrap->send_before_wait > 0){
				gdsrap->send_before_wait--;
			}
			else{
				switch(POKE_NET_GDS_GetStatus()){
				case POKE_NET_GDS_STATUS_INITIALIZED:	//�������ς�
				case POKE_NET_GDS_STATUS_ABORTED:		//!< ���f�I��
				case POKE_NET_GDS_STATUS_FINISHED:		//!< ����I��
				case POKE_NET_GDS_STATUS_ERROR:			//!< �G���[�I��
					ret = GDSRAP_MAIN_Send(gdsrap);
					if(ret == TRUE){
						OS_TPrintf("data send! req_code = %d\n", gdsrap->send_req);
						gdsrap->send_req = POKE_NET_GDS_REQCODE_LAST;
					}
					break;
				default:	//���͑��M�o���Ȃ�
					OS_TPrintf("send wait ... status = %d\n", POKE_NET_GDS_GetStatus());
					break;
				}
			}
		}
		
		//��M�f�[�^�m�F
		GDSRAP_MAIN_Recv(gdsrap);
		
		//��M�T�u�v���Z�X���s(�f�[�^��M��A������t���[�ɖ߂��O�Ɏ��s���鏈��)
		if(gdsrap->recv_sub_work.recv_sub_proccess != NULL){
			if(gdsrap->recv_sub_work.recv_sub_proccess(gdsrap, &gdsrap->recv_sub_work) == TRUE){
				gdsrap->recv_sub_work.recv_sub_proccess = NULL;
				//��M�f�[�^�̃R�[���o�b�N�Ăяo��
				if(gdsrap->recv_sub_work.user_callback_func != NULL){
					gdsrap->recv_sub_work.user_callback_func(
						gdsrap->response_callback.callback_work, &gdsrap->error_info);
					gdsrap->recv_sub_work.user_callback_func = NULL;
				}
				MI_CpuClear8(&gdsrap->recv_sub_work, sizeof(GDS_RECV_SUB_PROCCESS_WORK));
				gdsrap->recv_wait_req = POKE_NET_GDS_REQCODE_LAST;
			}
		}
	}
	
	//�T�u�V�[�P���X���s
	if(GDSRapSeqTbl[gdsrap->proccess_tblno] != NULL){
		int sub_ret;
		
		sub_ret = GDSRapSeqTbl[gdsrap->proccess_tblno][gdsrap->proccess_seqno](gdsrap, &gdsrap->sub_work);
		switch(sub_ret){
		case SUBSEQ_CONTINUE:
			break;
		case SUBSEQ_NEXT:
			MI_CpuClear8(&gdsrap->sub_work, sizeof(GDS_RAP_SUB_PROCCESS_WORK));
			gdsrap->proccess_seqno++;
			MI_CpuClear8(&gdsrap->sub_work, sizeof(GDS_RAP_SUB_PROCCESS_WORK));
			if(GDSRapSeqTbl[gdsrap->proccess_tblno][gdsrap->proccess_seqno] == NULL){
				gdsrap->proccess_seqno = 0;
				gdsrap->proccess_tblno = GDSRAP_PROCESS_REQ_NULL;
			}
			break;
		case SUBSEQ_PROCCESS_CHANGE:
			MI_CpuClear8(&gdsrap->sub_work, sizeof(GDS_RAP_SUB_PROCCESS_WORK));
			gdsrap->proccess_seqno = 0;
			break;
		}
	}
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M����
 *
 * @param   gdsrap		
 *
 * @retval  TRUE:���M����
 * @retval  FALSE:���M���s
 */
//--------------------------------------------------------------
static int GDSRAP_MAIN_Send(GDS_RAP_WORK *gdsrap)
{
	int ret = FALSE;
	
	//���N�G�X�g�Ƀf�[�^�����܂��Ă���Α��M
	switch(gdsrap->send_req){
	case POKE_NET_GDS_REQCODE_DRESSUPSHOT_REGIST:	// �h���X�A�b�v�V���b�g�o�^
		ret = POKE_NET_GDS_DressupShotRegist(&gdsrap->send_buf.gt_dress_send, gdsrap->response);
		break;
	case POKE_NET_GDS_REQCODE_DRESSUPSHOT_GET:		// �h���X�A�b�v�V���b�g�擾
		ret = POKE_NET_GDS_DressupShotGet(gdsrap->send_buf.sub_para.dressup.recv_monsno, 
			gdsrap->response);
		break;
	case POKE_NET_GDS_REQCODE_BOXSHOT_REGIST:		// �{�b�N�X�V���b�g�o�^
		ret = POKE_NET_GDS_BoxShotRegist(gdsrap->send_buf.sub_para.box.category_no, 
			&gdsrap->send_buf.gt_box_send ,gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("box shot�o�^���N�G�X�g���� category = %d\n", gdsrap->send_buf.sub_para.box.category_no);
		}
		break;
	case POKE_NET_GDS_REQCODE_BOXSHOT_GET:			// �{�b�N�X�V���b�g�擾
		ret = POKE_NET_GDS_BoxShotGet(gdsrap->send_buf.sub_para.box.category_no ,gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("box shot�擾���N�G�X�g���� category = %d\n", gdsrap->send_buf.sub_para.box.category_no);
		}
		break;
	case POKE_NET_GDS_REQCODE_RANKING_GETTYPE:		// �J�Â��Ă��郉���L���O�̃^�C�v���擾
		ret = POKE_NET_GDS_RankingGetType(gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("ranking type �擾���N�G�X�g����\n");
		}
		break;
	case POKE_NET_GDS_REQCODE_RANKING_UPDATE:		// �����L���O�̍X�V
		ret = POKE_NET_GDS_RankingUpdate(&gdsrap->send_buf.gt_ranking_mydata_send, 
			gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("ranking�X�V���N�G�X�g����\n");
		}
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST:	// �o�g���r�f�I�o�^
		ret = POKE_NET_GDS_BattleDataRegist(gdsrap->send_buf.gt_battle_rec_send_ptr, 
			gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("�o�g���r�f�I�o�^���N�G�X�g����\n");
		}
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH:	// �o�g���r�f�I����
		switch(gdsrap->send_req_option){
		case POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_CONDITION:	//�ڍ׌���
			ret = POKE_NET_GDS_BattleDataSearchCondition(&gdsrap->send_buf.gt_battle_rec_search,
				gdsrap->response);
			break;
		case POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_RANKING:	//�ŐV30��
			ret = POKE_NET_GDS_BattleDataSearchRanking(
				&gdsrap->send_buf.gt_battle_rec_ranking_search, gdsrap->response);
			break;
		case POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCHTYPE_EXRANKING:	//���C�ɓ�����30��
			ret = POKE_NET_GDS_BattleDataSearchExRanking(
				&gdsrap->send_buf.gt_battle_rec_ranking_search, gdsrap->response);
			break;
		}
		if(ret == TRUE){
			OS_TPrintf("�o�g���r�f�I�������N�G�X�g����\n");
		}
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_GET:	// �o�g���r�f�I�擾
		ret = POKE_NET_GDS_BattleDataGet(gdsrap->send_buf.sub_para.rec.data_number, 
			BATTLE_SERVER_VERSION, gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("�o�g���r�f�I�擾���N�G�X�g����\n");
		}
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE:	// �o�g���r�f�I���C�ɓ���o�^
		ret = POKE_NET_GDS_BattleDataFavorite(gdsrap->send_buf.sub_para.rec.data_number, 
			gdsrap->response);
		if(ret == TRUE){
			OS_TPrintf("�o�g���r�f�I���C�ɓ���o�^���N�G�X�g����\n");
		}
		break;
	default:
		OS_Printf("-not supported\n");
		GF_ASSERT(0);
		break;
	}
	
	if(ret == TRUE){
		OS_TPrintf("���M���� send_req = %d\n", gdsrap->send_req);
		gdsrap->recv_wait_req = gdsrap->send_req;
	}
	else{
		//��check ���M���s���A����͐�������܂Ŗ��t���[�����킵�����Ă��邪�A
		//        ����ł����̂��A�G���[���b�Z�[�W���o���K�v������̂��m�F
		OS_TPrintf("���M���s�Bsend_req = %d\n", gdsrap->send_req);
	}
	return ret;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�X�e�[�^�X�m�F
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static int GDSRAP_MAIN_Recv(GDS_RAP_WORK *gdsrap)
{
	if(gdsrap->recv_wait_req == POKE_NET_GDS_REQCODE_LAST){
		return TRUE;
	}
	
	// �X�e�[�^�X�擾
	gdsrap->stat = POKE_NET_GDS_GetStatus();

	if(	gdsrap->stat != gdsrap->laststat ){
		// �X�e�[�^�X�ɕω���������
		switch(gdsrap->stat){
		case POKE_NET_GDS_STATUS_INACTIVE:					// ��ғ���
			OS_Printf("-inactive\n");
			break;
		case POKE_NET_GDS_STATUS_INITIALIZED:				// �������ς�
			OS_Printf("-initialized\n");
			break;
		case POKE_NET_GDS_STATUS_REQUEST:					// ���N�G�X�g���s
			OS_Printf("-request\n");
			break;
		case POKE_NET_GDS_STATUS_NETSETTING:				// �l�b�g���[�N�ݒ蒆
			OS_Printf("-netsetting\n");
			break;
		case POKE_NET_GDS_STATUS_CONNECTING:				// �ڑ���
			OS_Printf("-connecting\n");
			break;
		case POKE_NET_GDS_STATUS_SENDING:					// ���M��
			OS_Printf("-sending\n");
			break;
		case POKE_NET_GDS_STATUS_RECEIVING:					// ��M��
			OS_Printf("-receiving\n");
			break;
		case POKE_NET_GDS_STATUS_ABORTED:					// ���f
			OS_Printf("-aborted\n");
			gdsrap->error_info.type = GDS_ERROR_TYPE_STATUS;
			gdsrap->error_info.req_code = 0;
			gdsrap->error_info.result = POKE_NET_GDS_STATUS_ABORTED;
			gdsrap->error_info.occ = TRUE;
		//	gdsrap->recv_wait_req = POKE_NET_GDS_REQCODE_LAST;
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_SystemError;
			break;
		case POKE_NET_GDS_STATUS_FINISHED:					// ����I��
			OS_Printf("-finished\n");
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_Normal;
			// ���X�|���X����
			Local_GetResponse(gdsrap);
			break;
		case POKE_NET_GDS_STATUS_ERROR:						// �G���[�I��
			OS_Printf("-error lasterrorcode:%d\n" ,POKE_NET_GDS_GetLastErrorCode());
			gdsrap->error_info.type = GDS_ERROR_TYPE_LIB;
			gdsrap->error_info.req_code = 0;
			gdsrap->error_info.result = POKE_NET_GDS_GetLastErrorCode();
			gdsrap->error_info.occ = TRUE;
		//	gdsrap->recv_wait_req = POKE_NET_GDS_REQCODE_LAST;
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_SystemError;
			break;
		}
		gdsrap->laststat = gdsrap->stat;
	}
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �����Z�[�u
 *
 * @param   gdsrap		
 *
 * @retval  SAVE_RESULT_CONTINUE	�Z�[�u�����p����
 * @retval  SAVE_RESULT_LAST		�Z�[�u�����p�����A�Ō�̈�O
 * @retval  SAVE_RESULT_OK			�Z�[�u����I��
 * @retval  SAVE_RESULT_NG			�Z�[�u���s�I��
 */
//--------------------------------------------------------------
static SAVE_RESULT GdsDivSave(GDS_RAP_WORK *gdsrap)
{
	SAVE_RESULT result;
	
	switch(gdsrap->div_save_seq){
	case 0:
		//result = SaveData_Save(sv);
		OS_TPrintf("�����Z�[�u�J�n\n");
		SaveData_DivSave_Init(gdsrap->savedata, SVBLK_ID_MAX);
		gdsrap->div_save_seq++;
		return SAVE_RESULT_CONTINUE;
	case 1:
		result = SaveData_DivSave_Main(gdsrap->savedata);
		if(result == SAVE_RESULT_OK || result == SAVE_RESULT_NG){
			gdsrap->div_save_seq = 0;
			OS_TPrintf("�����Z�[�u����\n");
		}
		return result;
	}
	return SAVE_RESULT_CONTINUE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�[�^��M��̃T�u�v���Z�X�F���ɉ����������Ȃ��ꍇ
 *
 * @param   gdsrap		
 *
 * @retval  TRUE:���������B�@FALSE:�����p����
 */
//--------------------------------------------------------------
static BOOL RecvSubProccess_Normal(void *work_gdsrap, void *work_recv_sub_work)
{
	GDS_RAP_WORK *gdsrap = work_gdsrap;
	GDS_RECV_SUB_PROCCESS_WORK *recv_sub_work = work_recv_sub_work;

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�[�^��M��̃T�u�v���Z�X�F�r�f�I���M�ς݃t���O�����Z�b�g���ăZ�[�u����
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static BOOL RecvSubProccess_VideoSendFlagResetSave(void *work_gdsrap, void *work_recv_sub_work)
{
	//���M�O�̃Z�[�u�͖����Ȃ������A���[�J���ł̑��M�ς݃`�F�b�N�������Ȃ����̂ŉ�������K�v����
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�[�^��M��̃T�u�v���Z�X�F�^��f�[�^�Ƀf�[�^�i���o�[���Z�b�g���ăZ�[�u����
 *
 * @param   gdsrap		
 * @param   recv_sub_work		
 *
 * @retval  
 */
//--------------------------------------------------------------
static BOOL RecvSubProccess_DataNumberSetSave(void *work_gdsrap, void *work_recv_sub_work)
{
	GDS_RAP_WORK *gdsrap = work_gdsrap;
	GDS_RECV_SUB_PROCCESS_WORK *recv_sub_work = work_recv_sub_work;
	POKE_NET_RESPONSE *res;
	POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST *param;
	SAVE_RESULT ret;
	
	res = POKE_NET_GDS_GetResponse();
	param = (POKE_NET_GDS_RESPONSE_BATTLEDATA_REGIST *)(res->Param);
	ret = BattleRec_GDS_MySendData_DataNumberSetSave(
		gdsrap->savedata, param->Code, 
		&recv_sub_work->recv_save_seq0, &recv_sub_work->recv_save_seq1);
	if(ret == SAVE_RESULT_OK || ret == SAVE_RESULT_NG){
		OS_TPrintf("�O���Z�[�u����\n");
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�[�^��M��̃T�u�v���Z�X�F�h���X���M�ς݃t���O�����Z�b�g���ăZ�[�u����
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static BOOL RecvSubProccess_DressSendFlagResetSave(void *work_gdsrap, void *work_recv_sub_work)
{
	//���M�O�̃Z�[�u�͖����Ȃ������A���[�J���ł̑��M�ς݃`�F�b�N�������Ȃ����̂ŉ�������K�v����
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�[�^��M��̃T�u�v���Z�X�F�{�b�N�X���M�ς݃t���O�����Z�b�g���ăZ�[�u����
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static BOOL RecvSubProccess_BoxSendFlagResetSave(void *work_gdsrap, void *work_recv_sub_work)
{
	//���M�O�̃Z�[�u�͖����Ȃ������A���[�J���ł̑��M�ς݃`�F�b�N�������Ȃ����̂ŉ�������K�v����
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �f�[�^��M��̃T�u�v���Z�X�F�V�X�e���G���[�����������ꍇ�̃P�A����
 *
 * @param   gdsrap		
 *
 * @retval  
 */
//--------------------------------------------------------------
static BOOL RecvSubProccess_SystemError(void *work_gdsrap, void *work_recv_sub_work)
{
	GDS_RAP_WORK *gdsrap = work_gdsrap;
	GDS_RECV_SUB_PROCCESS_WORK *recv_sub_work = work_recv_sub_work;
	
	OS_TPrintf("�V�X�e���G���[�����������ׁA�P�A������ laststat=%d, recv_wait_req=%d\n", gdsrap->laststat, gdsrap->recv_wait_req);
	
	//���M�O�̃Z�[�u�͖����Ȃ������A���[�J���ł̑��M�ς݃`�F�b�N�������Ȃ����̂�
	//�P�A�����͖����Ȃ���
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ��M�f�[�^����
 *
 * @param   gdsrap		
 *
 * @retval  TRUE:����I���B�@FALSE:�G���[����(�G���[����error_info�Q��)
 */
//--------------------------------------------------------------
static int Local_GetResponse(GDS_RAP_WORK *gdsrap)
{
	POKE_NET_RESPONSE *res;
	int size;
	int result = TRUE;
	
	res = POKE_NET_GDS_GetResponse();
	size = POKE_NET_GDS_GetResponseSize();
	
	GF_ASSERT(gdsrap->recv_wait_req == res->ReqCode);
	
	gdsrap->recv_sub_work.user_callback_func = NULL;
	
	// == �����ł��ꂼ��̃��X�|���X�ɑ΂��鏈�����s�� ==
	switch(res->ReqCode){
	case POKE_NET_GDS_REQCODE_DRESSUPSHOT_REGIST:		// �h���X�A�b�v�V���b�g�o�^
		OS_TPrintf("��M�F�h���X�A�b�v�V���b�g�A�b�v���[�h\n");
		result = GDS_RAP_RESPONSE_DressupShot_Upload(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_dressupshot_regist;
		if(result == FALSE){	//���M�ς݃t���O��߂��čăZ�[�u
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_DressSendFlagResetSave;
		}
		break;
	case POKE_NET_GDS_REQCODE_DRESSUPSHOT_GET:			// �h���X�A�b�v�V���b�g�擾
		OS_TPrintf("��M�F�h���X�A�b�v�V���b�g�擾\n");
		result = GDS_RAP_RESPONSE_DressupShot_Download(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_dressupshot_get;
		break;
	case POKE_NET_GDS_REQCODE_BOXSHOT_REGIST:			// �{�b�N�X�V���b�g�o�^
		OS_TPrintf("��M�F�{�b�N�X�V���b�g�o�^\n");
		result = GDS_RAP_RESPONSE_Boxshot_Upload(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_boxshot_regist;
		if(result == FALSE){	//���M�ς݃t���O��߂��čăZ�[�u
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_BoxSendFlagResetSave;
		}
		break;
	case POKE_NET_GDS_REQCODE_BOXSHOT_GET:				// �{�b�N�X�V���b�g�擾
		OS_TPrintf("��M�F�{�b�N�X�V���b�g�擾\n");
		result = GDS_RAP_RESPONSE_Boxshot_Download(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_boxshot_get;
		break;
	case POKE_NET_GDS_REQCODE_RANKING_GETTYPE:			// �����L���O�^�C�v�擾
		OS_TPrintf("��M�F�����L���O�^�C�v�擾\n");
		result = GDS_RAP_RESPONSE_RankingType_Download(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_ranking_type_get;
		break;
	case POKE_NET_GDS_REQCODE_RANKING_UPDATE:			// �����L���O�X�V
		OS_TPrintf("��M�F�����L���O�X�V�擾\n");
		result = GDS_RAP_RESPONSE_RankingUpdate_Download(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_ranking_update_get;
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST:		// �o�g���r�f�I�o�^
		OS_TPrintf("��M�F�o�g���r�f�I�o�^\n");
		result = GDS_RAP_RESPONSE_BattleVideo_Upload(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_battle_video_regist;
		if(result == FALSE){	//���M�ς݃t���O��߂��čăZ�[�u
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_VideoSendFlagResetSave;
		}
		else{
			//�o�^�R�[�h�������̃f�[�^�ɓ���āA�O���Z�[�u���s
			gdsrap->recv_sub_work.recv_sub_proccess = RecvSubProccess_DataNumberSetSave;
		}
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH:		// �o�g���r�f�I����
		OS_TPrintf("��M�F�o�g���r�f�I����\n");
		result = GDS_RAP_RESPONSE_BattleVideo_Search_Download(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_battle_video_search_get;
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_GET:		// �o�g���r�f�I�擾
		OS_TPrintf("��M�F�o�g���r�f�I�擾\n");
		result = GDS_RAP_RESPONSE_BattleVideo_Data_Download(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_battle_video_data_get;
		break;
	case POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE:		// �o�g���r�f�I���C�ɓ���o�^
		OS_TPrintf("��M�F�o�g���r�f�I���C�ɓ���o�^\n");
		result = GDS_RAP_RESPONSE_BattleVideo_Favorite_Upload(gdsrap, res);
		gdsrap->recv_sub_work.user_callback_func = gdsrap->response_callback.func_battle_video_favorite_regist;
		break;
		
	case POKE_NET_GDS_REQCODE_DEBUG_MESSAGE:			// �f�o�b�O�p���N�G�X�g
	default:
		OS_TPrintf("��M�F����`�Ȃ���\n");
		break;
	}
	
	//�G���[�����Z�b�g
	if(result == FALSE){
		gdsrap->error_info.type = GDS_ERROR_TYPE_APP;
		gdsrap->error_info.req_code = res->ReqCode;
		gdsrap->error_info.result = res->Result;
		gdsrap->error_info.occ = TRUE;
	}
	else{
		gdsrap->error_info.occ = FALSE;
	}
	
	return result;
}

//--------------------------------------------------------------
/**
 * @brief   �G���[�����擾����
 *
 * @param   gdsrap			
 * @param   error_info		�G���[�������
 *
 * @retval  TRUE:�G���[���������Ă���
 * @retval  FALSE:�G���[����
 */
//--------------------------------------------------------------
BOOL GDSRAP_ErrorInfoGet(GDS_RAP_WORK *gdsrap, GDS_RAP_ERROR_INFO **error_info)
{
	*error_info = &gdsrap->error_info;
	return gdsrap->error_info.occ;
}

//--------------------------------------------------------------
/**
 * @brief   �G���[�����N���A����
 *
 * @param   gdsrap		
 */
//--------------------------------------------------------------
void GDSRAP_ErrorInfoClear(GDS_RAP_WORK *gdsrap)
{
	MI_CpuClear8(&gdsrap->error_info, sizeof(GDS_RAP_ERROR_INFO));
}

//------------------------------------------------------------------
/**
 * @brief   WIFI�G���[���b�Z�[�W�F���C�h�\���R�[���o�b�N�Ăяo��
 *
 * @param   wk		
 * @param   msgno		
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void _systemMessagePrint(GDS_RAP_WORK *gdsrap, STRBUF *str_buf)
{
	gdsrap->callback_error_msg_wide(gdsrap->callback_work, str_buf);
}

//------------------------------------------------------------------
/**
 * @brief   Wifi�R�l�N�V�����G���[�̕\��
 *
 * @param   gdsrap		
 * @param   type	
 *
 * @retval  none		
 */
//------------------------------------------------------------------
static void errorDisp(GDS_RAP_WORK *gdsrap, int type, int code)
{
    int msgno;
	STRBUF *tempstr;

    if(type != -1){
        msgno = dwc_error_0001 + type;
    }
    else{
        msgno = dwc_error_0012;
    }
    
    WORDSET_RegisterNumber(gdsrap->wordset, 0, code,
                           5, NUMBER_DISPTYPE_ZERO, NUMBER_CODETYPE_DEFAULT);

	tempstr = MSGMAN_AllocString( gdsrap->msgman_wifisys, msgno );
	WORDSET_ExpandStr( gdsrap->wordset, gdsrap->ErrorString, tempstr );
	STRBUF_Delete(tempstr);

    _systemMessagePrint(gdsrap, gdsrap->ErrorString);
}

//--------------------------------------------------------------
/**
 * @brief   GDSRAP�����炩�̓�������s�A���͎�t�A�҂����A�Ȃǂ����Ă��邩�m�F����
 *
 * @param   gdsrap		
 *
 * @retval  TRUE:���̏��������s�A��t���Ă��Ȃ�
 * @retval  FALSE:���炩�̏��������s�A��t�A�҂��Ă���
 */
//--------------------------------------------------------------
BOOL GDSRAP_MoveStatusAllCheck(GDS_RAP_WORK *gdsrap)
{
	if(GDSRAP_ProccessCheck(gdsrap) == GDSRAP_PROCESS_REQ_NULL
			&& gdsrap->send_req == POKE_NET_GDS_REQCODE_LAST
			&& gdsrap->recv_wait_req == POKE_NET_GDS_REQCODE_LAST){
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
/**
 * @brief   GDSRAP�����s���̃v���Z�X�ԍ����擾����
 *
 * @param   gdsrap		
 *
 * @retval  GDSRAP_PROCESS_REQ_???(���s���̏������Ȃ��ꍇ��GDSRAP_PROCESS_REQ_NULL)
 */
//--------------------------------------------------------------
static int GDSRAP_ProccessCheck(GDS_RAP_WORK *gdsrap)
{
	return gdsrap->proccess_tblno;
}

//--------------------------------------------------------------
/**
 * @brief   GDSRAP�ɑ΂��ď����̃��N�G�X�g���s��
 *
 * @param   gdsrap				
 * @param   proccess_req		GDSRAP_PROCESS_REQ_???
 *
 * @retval  TRUE:���N�G�X�g��t����
 * @retval  FALSE:���N�G�X�g��t���s
 */
//--------------------------------------------------------------
BOOL GDSRAP_ProccessReq(GDS_RAP_WORK *gdsrap, GDSRAP_PROCESS_REQ proccess_req)
{
	gdsrap->proccess_tblno = proccess_req;
	OS_TPrintf("�T�u�v���Z�X���N�G�X�g�Ftblno = %d\n", proccess_req);
	
	return TRUE;
}


//==============================================================================
//	�f�o�b�O�p�c�[��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �f�o�b�O�c�[���F�Z�[�u���s�t���O��OFF����
 *
 * @param   gdsrap		
 */
//--------------------------------------------------------------
void DEBUG_GDSRAP_SaveFlagReset(GDS_RAP_WORK *gdsrap)
{
#ifdef PM_DEBUG
//	gdsrap->send_before_div_save = 0;
#endif
}

//==============================================================================
//
//	��check�@�b��@GDS���C�u������OS_AllocFromMain�֐����g�p���Ă���̂�
//				�ꎞ�I�Ƀq�[�v���쐬����
//
//==============================================================================
#define ROUND(n, a)		(((u32)(n)+(a)-1) & ~((a)-1))
static OSHeapHandle sHandle;

static void * LIB_Heap_Init(int heap_id)
{
	void*    arenaLo;
	void*    arenaHi;
	void*	alloc_ptr;
	
	int heap_size = 0x2000;
	
	arenaLo = sys_AllocMemory(heap_id, heap_size);
	alloc_ptr = arenaLo;
	arenaHi = (void*)((u32)arenaLo + heap_size);
	
    arenaLo = OS_InitAlloc(OS_ARENA_MAIN, arenaLo, arenaHi, 1);
    OS_SetArenaLo(OS_ARENA_MAIN, arenaLo);

    // [nakata] �|�C���^��32bit���E�ɃA���C������
    arenaLo = (void*)ROUND(arenaLo, 32);
    arenaHi = (void*)ROUND(arenaHi, 32);

    // [nakata] �q�[�v�̈�̍쐬
    sHandle = OS_CreateHeap(OS_ARENA_MAIN, arenaLo, arenaHi);
    OS_SetCurrentHeap(OS_ARENA_MAIN, sHandle );

    // From here on out, OS_Alloc and OS_Free behave like malloc and free respectively
//    OS_SetArenaLo(OS_ARENA_MAIN, arenaHi);

	return alloc_ptr;
}

static void LIB_Heap_Exit(void)
{
	OS_ClearAlloc(OS_ARENA_MAIN);
}
