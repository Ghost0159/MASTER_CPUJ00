//==============================================================================
/**
 * @file	gds_rap.h
 * @brief	GDS���C�u���������b�p�[��������
 * @author	matsuda
 * @date	2008.01.09(��)
 */
//==============================================================================
#ifndef __GDS_RAP_H__
#define __GDS_RAP_H__

#include <dwc.h>
#include "libgds/poke_net_gds.h"
#include "libgds/poke_net_common.h"


//==============================================================================
//	�萔��`
//==============================================================================
///GDS�G���[�^�C�v
enum{
	GDS_ERROR_TYPE_LIB,			///<���C�u�����G���[(POKE_NET_GDS_LASTERROR)
	GDS_ERROR_TYPE_STATUS,		///<�X�e�[�^�X�G���[(POKE_NET_GDS_STATUS)
	GDS_ERROR_TYPE_APP,			///<�e�A�v�����̃G���[
};

//==============================================================================
//	�\���̒�`
//==============================================================================

///���M�p�����[�^�\����
typedef struct{
	//���M�f�[�^
	union{
		GT_GDS_DRESS_SEND gt_dress_send;	///<�h���X�A�b�v���M�f�[�^
		GT_BOX_SHOT_SEND gt_box_send;		///<�{�b�N�X�V���b�g���M�f�[�^
		GT_RANKING_MYDATA_SEND gt_ranking_mydata_send;	///<�����̃����L���O���M�f�[�^
		GT_BATTLE_REC_SEND *gt_battle_rec_send_ptr;	///<���M�f�[�^�ւ̃|�C���^
		GT_BATTLE_REC_SEARCH_SEND gt_battle_rec_search;	///<�r�f�I�������M�f�[�^
		GT_BATTLE_REC_RANKING_SEARCH_SEND gt_battle_rec_ranking_search;	///<�r�f�I�����L���O�������M�f�[�^
	};

	//���M�f�[�^�ɕt������I�v�V�����p�����[�^
	union{
		//�{�b�N�X�̃T�u�p�����[�^
		struct{
			u8 category_no;		///<�o�^�J�e�S���[�ԍ�
			u8 tray_number;		///<�{�b�N�X�ԍ�
			u8 box_padding[2];
		}box;
		//�h���X�A�b�v�̃T�u�p�����[�^
		struct{
			u16 recv_monsno;	///<�擾����J�e�S���[�ԍ�(�|�P�����ԍ�)
			u16 dressup_padding;
		}dressup;
		//�o�g���r�f�I�̃T�u�p�����[�^
		struct{
			u64 data_number;	///<�f�[�^�i���o�[
		}rec;
	}sub_para;
}GDS_RAP_SEND_WORK;

///GDS���b�p�[���[�N�̃T�u���[�N
typedef struct _GDS_RAP_SUB_PROCCESS_WORK{
	int local_seq;
	int wait;
	int work;
}GDS_RAP_SUB_PROCCESS_WORK;

///GDS���C�u�����̃G���[���Ǘ��\����
typedef struct _GDS_RAP_ERROR_INFO{
	BOOL occ;				///<TRUE:�G���[��񂠂�B�@FALSE:�G���[��񖳂�
	int type;				///<�G���[�^�C�v(GDS_ERROR_TYPE_???)
	int req_code;			///<���N�G�X�g�R�[�h(POKE_NET_RESPONSE.ReqCode)
	int result;				///<����(POKE_NET_RESPONSE.Result)
}GDS_RAP_ERROR_INFO;

///�f�[�^��M���̃R�[���o�b�N�֐��̌^
typedef void (*GDSRAP_RESPONSE_FUNC)(void *, const GDS_RAP_ERROR_INFO *);
///�ʐM�G���[���b�Z�[�W�\���̃R�[���o�b�N�֐��̌^
typedef void (*GDSRAP_ERROR_WIDE_MSG_FUNC)(void *, STRBUF *);

///�f�[�^��M���̃R�[���o�b�N�֐��Ȃǂ��܂Ƃ߂��\����
typedef struct{
	void *callback_work;	///<�f�[�^��M���̃R�[���o�b�N�֐��Ɉ����Ƃ��ēn���|�C���^

	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�h���X�A�b�v�V���b�g�o�^(POKE_NET_GDS_REQCODE_DRESSUPSHOT_REGIST)
	GDSRAP_RESPONSE_FUNC func_dressupshot_regist;
	
	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�h���X�A�b�v�V���b�g�擾(POKE_NET_GDS_REQCODE_DRESSUPSHOT_GET)
	GDSRAP_RESPONSE_FUNC func_dressupshot_get;
	
	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�{�b�N�X�V���b�g�o�^(POKE_NET_GDS_REQCODE_BOXSHOT_REGIST)
	GDSRAP_RESPONSE_FUNC func_boxshot_regist;
	
	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�{�b�N�X�V���b�g�擾(POKE_NET_GDS_REQCODE_BOXSHOT_GET)
	GDSRAP_RESPONSE_FUNC func_boxshot_get;

	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�J�Ò��̃����L���O�^�C�v�擾(POKE_NET_GDS_REQCODE_RANKING_GETTYPE)
	GDSRAP_RESPONSE_FUNC func_ranking_type_get;
	
	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�����L���O�X�V(�����̃f�[�^�擾�����ʎ擾)(POKE_NET_GDS_REQCODE_RANKING_UPDATE)
	GDSRAP_RESPONSE_FUNC func_ranking_update_get;

	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�o�g���r�f�I�o�^(POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST)
	GDSRAP_RESPONSE_FUNC func_battle_video_regist;

	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�o�g���r�f�I����(�ڍ׌����A�ŐV�A���C�ɓ��胉���L���O)(POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH)
	GDSRAP_RESPONSE_FUNC func_battle_video_search_get;

	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�o�g���r�f�I�f�[�^�擾(POKE_NET_GDS_REQCODE_BATTLEDATA_GET)
	GDSRAP_RESPONSE_FUNC func_battle_video_data_get;

	///�f�[�^��M���̃R�[���o�b�N�֐��ւ̃|�C���^
	///�o�g���r�f�I���C�ɓ���o�^(POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE)
	GDSRAP_RESPONSE_FUNC func_battle_video_favorite_regist;
}GDS_RAP_RESPONSE_CALLBACK;

///�f�[�^��M��ɍs���T�u�v���Z�X�p�̃R�[���o�b�N�֐��̌^
typedef BOOL (*GDSRAP_RECV_SUB_PROCCESS_FUNC)(void *, void *);

///�f�[�^��M��ɍs���T�u�v���Z�X�p���[�N
typedef struct _GDS_RECV_SUB_PROCCESS_WORK{
	GDSRAP_RESPONSE_FUNC user_callback_func;	///<�f�[�^��M��ɌĂяo���̃R�[���o�b�N�֐�
	u16 recv_save_seq0;
	u16 recv_save_seq1;
	u8 recv_sub_seq;
	GDSRAP_RECV_SUB_PROCCESS_FUNC recv_sub_proccess;
}GDS_RECV_SUB_PROCCESS_WORK;

///GDS���C�u�����ANitroDWC�ɋ߂��֌W�̃��[�N�̍\����
typedef struct _GDS_RAP_WORK{
	SAVEDATA *savedata;
	
	//�A�v������n����郏�[�N�|�C���^
	///�G���[���b�Z�[�W�`��p�̃R�[���o�b�N�֐�
	GDSRAP_ERROR_WIDE_MSG_FUNC callback_error_msg_wide;
	void *callback_work;	///<�R�[���o�b�N�֐��Ăяo�����A�����Ƃ��ēn��
	
	//�A�v������n�����d�v�f�[�^
	POKE_NET_REQUESTCOMMON_AUTH pokenet_auth;
	int heap_id;
	
	POKE_NET_GDS_STATUS stat;
	POKE_NET_GDS_STATUS laststat;
	
	GDS_RAP_RESPONSE_CALLBACK response_callback;	///�f�[�^��M���̃R�[���o�b�N�֐�
	GDS_RAP_ERROR_INFO error_info;			///<�G���[���i�[�p���[�N
	
	//��check�@�b��
	void *areanaLo;
	
	//���M�f�[�^
	GDS_RAP_SEND_WORK send_buf;				///<���M�f�[�^
	//��M�f�[�^
	void *response;		///<POKE_NET_GDS_RESPONSE_RESULT_DRESSUPSHOT_REGIST_xxxx����M����܂�
	
	//�����g�p���[�N
	int comm_initialize_ok;		///<TRUE:�ʐM���C�u�����������ς�
	int proccess_tblno;		///<���s����T�u�v���Z�X�̃e�[�u���ԍ�
	int proccess_seqno;		///<���s���̃T�u�v���Z�X�e�[�u�����̃V�[�P���X�ԍ�
	BOOL gdslib_initialize;			///<TRUE:GDS���C�u�����������ς�
	BOOL connect_success;					///<TRUE:�l�b�g�ڑ���
	GDS_RAP_SUB_PROCCESS_WORK sub_work;		///<�T�u�v���Z�X����p���[�N

	int ConnectErrorNo;						///< DWC�E�܂��̓T�[�o�[����̃G���[
	int ErrorRet;
	int ErrorCode;
	int ErrorType;
	
	int send_req;	///<POKE_NET_GDS_REQCODE_???(���N�G�X�g�����̏ꍇ��POKE_NET_GDS_REQCODE_LAST)
	int recv_wait_req;///<send_req�̌��ʎ�M�f�[�^�҂�(POKE_NET_GDS_REQCODE_???)
	int send_req_option;	///<POKE_NET_GDS_REQCODE_???�ł���Ɍ����𕪂���ꍇ�̃I�v�V����

	MSGDATA_MANAGER *msgman_wifisys;		///<���b�Z�[�W�f�[�^�}�l�[�W���̃|�C���^
	WORDSET *wordset;						///<Alloc�������b�Z�[�W�p�P��o�b�t�@�ւ̃|�C���^
	STRBUF *ErrorString;					///<������W�J�o�b�t�@

	GDS_RECV_SUB_PROCCESS_WORK recv_sub_work;	///<�f�[�^��M��̃T�u�v���Z�X�p���[�N
	
	u8 div_save_seq;			///<�����Z�[�u���s�V�[�P���X
	u8 send_before_wait;		///<TRUE:���M�O�Ƀ��b�Z�[�W��������ׂ̈��E�F�C�g
}GDS_RAP_WORK;


///GDSRAP���������Ɉ����n������
typedef struct{
	int gs_profile_id;		///<GS�v���t�@�C��ID
	
	int heap_id;
	SAVEDATA *savedata;
	
	GDS_RAP_RESPONSE_CALLBACK response_callback;	///�f�[�^��M���̃R�[���o�b�N�֐�
	
	///�G���[���b�Z�[�W�`��p�̃R�[���o�b�N�֐�
	GDSRAP_ERROR_WIDE_MSG_FUNC callback_error_msg_wide;
	void *callback_work;	///<�R�[���o�b�N�֐��Ăяo�����A�����Ƃ��ēn��
}GDSRAP_INIT_DATA;

//==============================================================================
//	�萔��`
//==============================================================================
//--------------------------------------------------------------
//	�������s���N�G�X�g
//--------------------------------------------------------------
typedef enum{
	GDSRAP_PROCESS_REQ_NULL,				///<���N�G�X�g����
	
//	GDSRAP_PROCESS_REQ_INTERNET_CONNECT,	///<�C���^�[�l�b�g�ڑ�
//	GDSRAP_PROCESS_REQ_INTERNET_CLEANUP,	///<�C���^�[�l�b�g�ؒf
	
	//�ȉ��A�V�X�e�������ł̂݌Ăяo��
	GDSRAP_PROCESS_REQ_WIFI_ERROR,			///<WIFI�ڑ��G���[�������������̋����ؒf����
	GDSRAP_PROCESS_REQ_SERVER_ERROR,		///<�T�[�o�[�T�[�r�X�G���[�������������̋����ؒf����
}GDSRAP_PROCESS_REQ;


//==============================================================================
//	�O���֐��錾
//==============================================================================
//--------------------------------------------------------------
//	���C��
//--------------------------------------------------------------
extern int GDSRAP_Init(GDS_RAP_WORK *gdsrap, const GDSRAP_INIT_DATA *init_data);
extern int GDSRAP_Main(GDS_RAP_WORK *gdsrap);
extern void GDSRAP_Exit(GDS_RAP_WORK *gdsrap);

//--------------------------------------------------------------
//	���M�n
//--------------------------------------------------------------
extern int GDSRAP_Tool_Send_DressupUpload(GDS_RAP_WORK *gdsrap, 
	GDS_PROFILE_PTR gpp, IMC_TELEVISION_SAVEDATA * dress);
extern int GDSRAP_Tool_Send_DressupDownload(GDS_RAP_WORK *gdsrap, int monsno);
extern int GDSRAP_Tool_Send_BoxshotUpload(GDS_RAP_WORK *gdsrap, int category_no, 
	GDS_PROFILE_PTR gpp, const BOX_DATA *boxdata, int tray_number);
extern int GDSRAP_Tool_Send_BoxshotDownload(GDS_RAP_WORK *gdsrap, int category_no);
extern int GDSRAP_Tool_Send_RankingTypeDownload(GDS_RAP_WORK *gdsrap);
extern int GDSRAP_Tool_Send_RankingUpdate(GDS_RAP_WORK *gdsrap, GDS_PROFILE_PTR gpp, 
	GT_RANKING_MYDATA ranking_mydata[]);
extern int GDSRAP_Tool_Send_BattleVideoUpload(GDS_RAP_WORK *gdsrap, GDS_PROFILE_PTR gpp);
extern int GDSRAP_Tool_Send_BattleVideoSearchDownload(GDS_RAP_WORK *gdsrap, 
	u16 monsno, u8 battle_mode, u8 country_code, u8 local_code);
extern int GDSRAP_Tool_Send_BattleVideoNewDownload_ColosseumOnly(GDS_RAP_WORK *gdsrap);
extern int GDSRAP_Tool_Send_BattleVideoNewDownload(GDS_RAP_WORK *gdsrap);
extern int GDSRAP_Tool_Send_BattleVideoFavoriteDownload_Colosseum(GDS_RAP_WORK *gdsrap);
extern int GDSRAP_Tool_Send_BattleVideoFavoriteDownload_Frontier(GDS_RAP_WORK *gdsrap);
extern int GDSRAP_Tool_Send_BattleVideo_DataDownload(GDS_RAP_WORK *gdsrap, u64 data_number);
extern int GDSRAP_Tool_Send_BattleVideo_FavoriteUpload(GDS_RAP_WORK *gdsrap, u64 data_number);

//--------------------------------------------------------------
//	�����n
//--------------------------------------------------------------
extern BOOL GDSRAP_MoveStatusAllCheck(GDS_RAP_WORK *gdsrap);
extern BOOL GDSRAP_ProccessReq(GDS_RAP_WORK *gdsrap, GDSRAP_PROCESS_REQ proccess_req);

//--------------------------------------------------------------
//	�G���[
//--------------------------------------------------------------
extern BOOL GDSRAP_ErrorInfoGet(GDS_RAP_WORK *gdsrap, GDS_RAP_ERROR_INFO **error_info);
extern void GDSRAP_ErrorInfoClear(GDS_RAP_WORK *gdsrap);

//--------------------------------------------------------------
//	�f�o�b�O�p
//--------------------------------------------------------------
extern void DEBUG_GDSRAP_SaveFlagReset(GDS_RAP_WORK *gdsrap);
extern void DEBUG_GDSRAP_SendVideoProfileFreeWordSet(GDS_RAP_WORK *gdsrap, u16 *set_code);

#endif	//__GDS_RAP_H__
