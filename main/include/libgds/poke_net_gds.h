/*===========================================================================*
  Project:  Pokemon Global Data Station Network Library
  File:     poke_net_gds.h

  $NoKeywords: $
 *===========================================================================*/
/*! @file
	@brief	Pokemon GDS WiFi ���C�u����
	@date	2007/12/18
*/
//===========================================================================
/**
		@mainpage	Pokemon GDS WiFi ���C�u����
			<b>�E�T�v</b><br>
			�@�{���C�u�����́ANintendoDS��Ń|�P�����f�c�r�֘A�̒ʐM���s���ׂ̂��̂ł��B<br>
			<b>�E����</b><br>
			�@�{�w���v�t�@�C���̓��t�@�����X�}�j���A���ł��B<br>
			�@�����͕ʓr poke_net_inst.pdf ���Q�Ƃ��Ă��������B<br>
			�@<br>
		@version	0.04	���M�����[�V�����A�T�[�o�[�o�[�W�����Ή�
		@version	0.03	�ő�ŏ���`�ȂǑΉ��A�o�g���f�[�^�����Ή�
		@version	0.02	�o�g���f�[�^/�����L���O�@�\�ǉ�<br>������������
		@version	0.01	���񃊃��[�X �h���X�A�b�v/�{�b�N�X�V���b�g

*/

#ifndef ___POKE_NET_GDS___
#define ___POKE_NET_GDS___

#include "application/battle_recorder/gds_min_max.h"				// �ő�ŏ���`
#include "poke_net_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------*
					�^�E�萔�錾
 *-----------------------------------------------------------------------*/

//! �|�P���� WiFi GDS ���C�u�����̌��݂̏��
typedef enum {
	POKE_NET_GDS_STATUS_INACTIVE ,					//!< ��ғ���
	POKE_NET_GDS_STATUS_INITIALIZED ,				//!< ��������
	POKE_NET_GDS_STATUS_REQUEST ,					//!< ���N�G�X�g���s��
	POKE_NET_GDS_STATUS_NETSETTING ,				//!< �l�b�g���[�N�ݒ蒆
	POKE_NET_GDS_STATUS_CONNECTING ,				//!< �ڑ���
	POKE_NET_GDS_STATUS_SENDING ,					//!< ���M��
	POKE_NET_GDS_STATUS_RECEIVING ,					//!< ��M��
	POKE_NET_GDS_STATUS_ABORTED ,					//!< ���f�I��
	POKE_NET_GDS_STATUS_FINISHED ,					//!< ����I��
	POKE_NET_GDS_STATUS_ERROR ,						//!< �G���[�I��
	POKE_NET_GDS_STATUS_COUNT
} POKE_NET_GDS_STATUS;

//! �|�P���� WiFi GDS ���C�u�������G���[�I�������ۂ̏ڍ�
typedef enum {
	POKE_NET_GDS_LASTERROR_NONE ,					//!< ���ɂȂ�
	POKE_NET_GDS_LASTERROR_NOTINITIALIZED ,			//!< ����������Ă��Ȃ�
	POKE_NET_GDS_LASTERROR_ILLEGALREQUEST ,			//!< ���N�G�X�g���ُ�
	POKE_NET_GDS_LASTERROR_CREATESOCKET ,			//!< �\�P�b�g�����Ɏ��s
	POKE_NET_GDS_LASTERROR_IOCTRLSOCKET ,			//!< �\�P�b�g�̃m���u���b�L���O���Ɏ��s
	POKE_NET_GDS_LASTERROR_NETWORKSETTING ,			//!< �l�b�g���[�N�ݒ莸�s
	POKE_NET_GDS_LASTERROR_CREATETHREAD ,			//!< �X���b�h�������s
	POKE_NET_GDS_LASTERROR_CONNECT ,				//!< �ڑ����s
	POKE_NET_GDS_LASTERROR_SENDREQUEST ,			//!< ���N�G�X�g�G���[
	POKE_NET_GDS_LASTERROR_RECVRESPONSE ,			//!< ���X�|���X�G���[
	POKE_NET_GDS_LASTERROR_CONNECTTIMEOUT ,			//!< �ڑ��^�C���A�E�g
	POKE_NET_GDS_LASTERROR_SENDTIMEOUT ,			//!< ���M�^�C���A�E�g
	POKE_NET_GDS_LASTERROR_RECVTIMEOUT ,			//!< ��M�^�C���A�E�g
	POKE_NET_GDS_LASTERROR_ABORTED ,				//!< ���f�I��
	POKE_NET_GDS_LASTERROR_GETSVL ,					//!< SVL�擾�G���[
	POKE_NET_GDS_LASTERROR_COUNT
} POKE_NET_GDS_LASTERROR;

/*
// = �Г��P�̌��J�T�[�o�[ =
#define POKE_NET_GDS_URL		"192.168.1.219"				// �T�[�o�[URL
#define POKE_NET_GDS_PORTNO		23457						// �T�[�o�[�|�[�g�ԍ�
*/
/*
// = �Г������T�[�o�[�F�Ǘ� =
#define POKE_NET_GDS_URL		"192.168.1.105"				// �T�[�o�[URL
#define POKE_NET_GDS_PORTNO		13570						// �T�[�o�[�|�[�g�ԍ�
*/
/*
// = �Г������T�[�o�[:�X���[�u =
#define POKE_NET_GDS_URL		"192.168.1.105"				// �T�[�o�[URL
#define POKE_NET_GDS_PORTNO		13571						// �T�[�o�[�|�[�g�ԍ�
*/
/*
#define POKE_NET_GDS_URL		"pmsv.denyu-sha.to"
#define POKE_NET_GDS_PORTNO		12400
*/

#define POKE_NET_GDS_URL		"pkgdsprod.nintendo.co.jp"	// �T�[�o�[URL
#define POKE_NET_GDS_PORTNO		12400						// �T�[�o�[�|�[�g�ԍ�


/*-----------------------------------------------------------------------*
					�O���[�o���ϐ���`
 *-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*
					�֐��O���錾
 *-----------------------------------------------------------------------*/
extern BOOL POKE_NET_GDS_Initialize(POKE_NET_REQUESTCOMMON_AUTH *_auth);
extern void POKE_NET_GDS_Release();
extern void POKE_NET_GDS_SetThreadLevel(unsigned long _level);
extern POKE_NET_GDS_STATUS POKE_NET_GDS_GetStatus();
extern POKE_NET_GDS_LASTERROR POKE_NET_GDS_GetLastErrorCode();
extern BOOL POKE_NET_GDS_Abort();

extern BOOL POKE_NET_GDS_DebugMessageRequest(char *_message ,void *_response);

extern BOOL POKE_NET_GDS_DressupShotRegist(GT_GDS_DRESS_SEND *_data ,void *_response);
extern BOOL POKE_NET_GDS_DressupShotGet(long _pokemonno ,void *_response);

extern BOOL POKE_NET_GDS_BoxShotRegist(long _groupno ,GT_BOX_SHOT_SEND *_data ,void *_response);
extern BOOL POKE_NET_GDS_BoxShotGet(long _groupno ,void *_response);

extern BOOL POKE_NET_GDS_RankingGetType(void *_response);
extern BOOL POKE_NET_GDS_RankingUpdate(GT_RANKING_MYDATA_SEND *_data ,void *_response);

extern void *POKE_NET_GDS_GetResponse();
extern long POKE_NET_GDS_GetResponseSize();
extern long POKE_NET_GDS_GetResponseMaxSize(long _reqno);

extern BOOL POKE_NET_GDS_BattleDataRegist(GT_BATTLE_REC_SEND *_data ,void *_response);
extern BOOL POKE_NET_GDS_BattleDataSearchCondition(GT_BATTLE_REC_SEARCH_SEND *_condition ,void *_response);
extern BOOL POKE_NET_GDS_BattleDataSearchRanking(GT_BATTLE_REC_RANKING_SEARCH_SEND *_condition ,void *_response);
extern BOOL POKE_NET_GDS_BattleDataSearchExRanking(GT_BATTLE_REC_RANKING_SEARCH_SEND *_condition ,void *_response);
extern BOOL POKE_NET_GDS_BattleDataGet(u64 _code ,u32 _sver ,void *_response);
extern BOOL POKE_NET_GDS_BattleDataFavorite(u64 _code ,void *_response);

extern void POKE_NET_GDS_DEBUG_PrintResponse(POKE_NET_RESPONSE *_response ,long _size);

#ifndef SDK_FINALROM

#else

#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // ___POKE_NET_GDS___
