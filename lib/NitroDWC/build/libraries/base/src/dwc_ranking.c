/*---------------------------------------------------------------------------*
  Project:  NitroDWC -  - 
  File:     dwc_ranking.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_ranking.c,v $
  Revision 1.17  2007/08/23 23:32:35  takayama
  DWC_2_2_RC1 �̎��_�Ƀ��[���o�b�N�����B

  Revision 1.14  2007/05/31 07:05:32  nakata
  DWC_RNK_GET_MODE_NEAR_HI��DWC_RNK_GET_MODE_NEAR_LOW��ǉ�

  Revision 1.13  2006/06/21 01:04:22  hayashi
  changed to accept DWC_RnkPutScoreAsync with no user defined data.

  Revision 1.12  2006/06/16 04:51:58  hayashi
  fixed the wrong comment

  Revision 1.11  2006/05/30 14:08:50  hayashi
  added DWC_RnkResGetTotal

  Revision 1.10  2006/05/26 02:07:44  hayashi
  change the limit of number at getting frined ranking

  Revision 1.9  2006/05/23 12:49:31  hayashi
  exclude dwc_serverurl_inetrnal.h including

  Revision 1.8  2006/05/22 07:29:58  hayashi
  changed to be set the server by DWC_SetAuthServer()

  Revision 1.7  2006/05/12 08:28:54  hayashi
  NOTINITIALIZED -> NOTREADY
  RIVAL -> NEAR
  DWCAccUserData -> DWCUserData
  BOOL	DWC_RnkShutdown() -> DWCRnkError

  Revision 1.6  2006/04/26 09:25:40  hayashi
  specification is changed related to PID handling

  Revision 1.5  2006/04/24 10:12:12  hayashi
  change the limit value of parameter checking  ==0 to <=1

  Revision 1.4  2006/04/13 11:35:19  hayashi
  Change prefix Ranking->Rnk

  Revision 1.3  2006/04/13 09:01:38  hayashi
  character code converting

  Revision 1.2  2006/04/11 09:01:33  hayashi
  DWC_RnkInitialize�ŁA�ڑ���̃T�[�o�[��I���ł���悤�ɂ���

  Revision 1.1  2006/04/10 10:16:58  hayashi
  added the ranking module


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

#include <string.h>
#include <stdlib.h>

#include <base/dwc_account.h>
#include <base/dwc_ranking.h>
#include <base/dwc_error.h>
#include <base/dwc_memfunc.h>

#include <auth/dwc_http.h>

#include <ranking/dwc_ranksession.h>

/* -------------------------------------------------------------------------
        pre-processing switch
   ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
        define
   ------------------------------------------------------------------------- */

//
// �����dwc_serverurl_inetrnal.h�ł���`����Ă��邪�\����ȒP�̂��ߒ�����
//
#define DWC_AUTH_NAS_URL		"https://nas.nintendowifi.net/ac"

/* -------------------------------------------------------------------------
        macro
   ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
        struct
   ------------------------------------------------------------------------- */

// DWC_RnkInitialize�ɓn����鏉������������p�[�X���邽�߂̍\����
typedef union{

	char	data[84];

	struct{
		char	secretkey[20];	// �閧��
		char	randkey_1[8];	// �����Í����P
		char	randkey_2[8];	// �����Í����Q
		char	randkey_3[8];	// �����Í����R
		char	randkey_4[8];	// �����Í����S
		char	gamename[32];	// �Q�[����
	}info;

}	DWCiRankingInitData;

// ��M�f�[�^���󂯎�邽�߂̍\����
//
typedef struct{

	u32					size;	// �T�C�Y(bytes)
	DWCRnkGetMode		mode;	// ���[�h

	union{

		void*				header;

		struct{								// �� �����L���O���X�g�擾���̃w�b�_
											//
			u32	count;						//   �擾�����s��
											//
			u32	total;						//   ����
											//
			DWCRnkData	rankingdata;		//   �����L���O�f�[�^�̐擪�|�C���^

		}				*listheader;

		struct{								// �� ���ʎ擾���̃w�b�_
											//
			u32	order;						//   ����
											//
			u32	total;						//   ����

		}				*orderheader;

	};

} DWCiRankingResponse;


// �ڑ���̃T�[�o�[���擾���邽�߂ɔF�؃T�[�o�[�̃A�h���X���Q��
//
extern DWCHttpParam DWCauthhttpparam;

/* -------------------------------------------------------------------------
        variable
   ------------------------------------------------------------------------- */

// �����L���O���W���[���̏�ԊǗ��\����
struct {

	DWCRnkState	state;		// �X�e�[�^�X

	s32			pid;		// GameSpy Profile ID

}	g_rankinginfo	=		// �����f�[�^
						{
							DWC_RNK_STATE_NOTREADY,	// �X�e�[�^�X
						};


/* -------------------------------------------------------------------------
        prototype
   ------------------------------------------------------------------------- */

DWCRnkError	DWCi_RankingGetResponse( DWCiRankingResponse* out );


/* -------------------------------------------------------------------------
        function - internal
   ------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*
  Name:		DWCi_RankingGetResponse

  Description:	�ʐM�Ŏ�M�����f�[�^���擾���܂��B
				�擾�����o�b�t�@�́ADWC_RnkShutdown()�̌Ăяo���ŉ������
				��܂ŗL���ł��B

  Arguments:	out	���ʂ��i�[����DWCiRankingResponse�\���̂̃|�C���^

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
  				DWC_RNK_ERROR_NOTCOMPLETED		�ʐM������
				DWC_RNK_ERROR_EMPTY_RESPONSE	��̃��X�|���X
 *---------------------------------------------------------------------------*/

DWCRnkError	DWCi_RankingGetResponse( DWCiRankingResponse* out ){

	void* buf;
	u32 size;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// �X�e�[�^�X�`�F�b�N
	//
	if( g_rankinginfo.state != DWC_RNK_STATE_COMPLETED ){
	
		// �ʐM���������Ă��Ȃ�
		return DWC_RNK_ERROR_NOTCOMPLETED;
	
	}

	// ���X�|���X�f�[�^�擾
	// 
	buf = DWCi_RankingSessionGetResponse( &size );

	if( size == 0 ){
	
		// ���X�|���X�f�[�^����
		return DWC_RNK_ERROR_EMPTY_RESPONSE;
	
	}

	// �Ԃ�l�ɃZ�b�g����
	//
	out->size = size;
	out->mode = (DWCRnkGetMode)((u32*)buf)[0];
	out->header = &((u32*)buf)[1];

	return DWC_RNK_SUCCESS;

}


/* -------------------------------------------------------------------------
        function - external
   ------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------*
  Name:		DWC_RnkInitialize

  Description:	�����L���O���W���[�������������܂�
  				���ɏ������ς݂̏ꍇ�⏉�����f�[�^���s���ȏꍇ�͎��s���܂�
  				�ʐM�p�閧���������SDK�^�p�҂ɂ�蔭�s�������̂ŁA�T�[�o�[
  				�ɑ΂��ă��j�[�N�ł��B

  Arguments:	initdata		�ʐM�p�閧��������
  				userdata		���[�U�[�f�[�^

  Returns:
	DWC_RNK_SUCCESS							����
	DWC_RNK_ERROR_INIT_ALREADYINITIALIZED	��������
	DWC_RNK_ERROR_INIT_INVALID_INITDATASIZE	�s���ȏ������f�[�^�T�C�Y
	DWC_RNK_ERROR_INIT_INVALID_INITDATA		�s���ȏ������f�[�^
	DWC_RNK_ERROR_INIT_INVALID_USERDATA		�s���ȃ��[�U�[�f�[�^

 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkInitialize(	const char* initdata,
								const DWCUserData* userdata ){

	u32 randkey_1,randkey_2,randkey_3,randkey_4;
	DWCiRankingInitData* ptr;
	char secretkey[21];
	char buf[9] = "";

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// �X�e�[�^�X�`�F�b�N
	if( g_rankinginfo.state != DWC_RNK_STATE_NOTREADY ){
	
		// �������ς�
		return DWC_RNK_ERROR_INIT_ALREADYINITIALIZED;
	
	}

	// ���[�U�[�f�[�^�̐�����������
	if( !DWC_CheckUserData( userdata )
		|| !DWC_CheckHasProfile( userdata )){

		// �s���ȃ��[�U�[�f�[�^
		return DWC_RNK_ERROR_INIT_INVALID_USERDATA;

	}

	// �������f�[�^�`�F�b�N
	if( strlen( initdata ) >= sizeof( DWCiRankingInitData ) ){

		// �T�C�Y���s��
		return DWC_RNK_ERROR_INIT_INVALID_INITDATASIZE;

	}

	// �|�C���^�ɃL���X�g
	ptr = (DWCiRankingInitData*)initdata;

	// �閧���̎擾
	strncpy( secretkey, ptr->info.secretkey, 20 );
	secretkey[20] = '\0';

	// �����Í����̕���
	randkey_1 = strtoul( strncpy( buf, ptr->info.randkey_1, 8 ), NULL, 16 );
	randkey_2 = strtoul( strncpy( buf, ptr->info.randkey_2, 8 ), NULL, 16 );
	randkey_3 = strtoul( strncpy( buf, ptr->info.randkey_3, 8 ), NULL, 16 );
	randkey_4 = strtoul( strncpy( buf, ptr->info.randkey_4, 8 ), NULL, 16 );

	// �����Í����̐�����������
	if( !DWCi_RankingValidateKey(	ptr->info.gamename,
									secretkey,
									randkey_1,
									randkey_2,
									randkey_3,
									randkey_4) ){
	
		// �s���ȃL�[
		return DWC_RNK_ERROR_INIT_INVALID_INITDATA;
	
	}

	// GameSpyProfileID��ێ�����
	g_rankinginfo.pid = userdata->gs_profile_id;

	// �ڑ���̃T�[�o�[��I������
	if( strcmp( DWCauthhttpparam.url, DWC_AUTH_NAS_URL ) == 0 ){
	
		// HTTP���W���[����������(�{�ԃT�[�o�[)
		DWCi_RankingSessionInitialize( TRUE );

	}else{
	
		// HTTP���W���[����������(�J���T�[�o�[)
		DWCi_RankingSessionInitialize( FALSE );

	}

	// �X�e�[�^�X���Z�b�g
	g_rankinginfo.state = DWC_RNK_STATE_INITIALIZED;


	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkShutdown

  Description:	�����L���O���W���[���Ŏg�p�������\�[�X���J�����܂��B
				��M�o�b�t�@���J������邽�߁ADWCi_RankingGetResponse()�Ŏ擾
				�����o�b�t�@�͖����ɂȂ�܂��B

  Arguments:	�Ȃ�

  Returns:		DWC_RNK_SUCCESS	����
 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkShutdown(){

	// HTTP���W���[���̊J��
	DWCi_RankingSessionShutdown();

	// �X�e�[�^�X���Z�b�g
	g_rankinginfo.state = DWC_RNK_STATE_NOTREADY;

	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkPutScoreAsync

  Description:	�X�R�A��o�^����񓯊��������J�n���܂��B
  				

  Arguments:	category	�J�e�S���[
				region		���[�W�����R�[�h
  				score		�X�R�A
				data		���[�U�[��`�f�[�^�̃o�b�t�@�ւ̃|�C���^
				size		data�̃T�C�Y(DWC_RNK_DATA_MAX�ȉ�)

  Returns:		DWC_RNK_SUCCESS						����
  				DWC_RNK_IN_ERROR					�G���[������
  				DWC_RNK_ERROR_PUT_NOTREADY			��������
				DWC_RNK_ERROR_INVALID_PARAMETER		�s���ȃp�����[�^�[
				DWC_RNK_ERROR_PUT_INVALID_KEY		�Í����L�[���s��
				DWC_RNK_ERROR_PUT_NOMEMORY			�������[�s��
 *---------------------------------------------------------------------------*/
DWCRnkError	DWC_RnkPutScoreAsync(	u32 category,
									DWCRnkRegion region,
									s32 score,
									void* data,
									u32 size ){

	DWCiRankingSessionResult res;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// �X�e�[�^�X�`�F�b�N
	//
	if( ( g_rankinginfo.state != DWC_RNK_STATE_INITIALIZED ) 
		&& (g_rankinginfo.state != DWC_RNK_STATE_COMPLETED ) ){
	
		// ����������Ă��Ȃ�
		return DWC_RNK_ERROR_PUT_NOTREADY;
	
	}

	// �p�����[�^�[�`�F�b�N
	//
	if( (category > DWC_RNK_CATEGORY_MAX)
		|| (size > DWC_RNK_DATA_MAX)){
	
		// �s���ȃp�����[�^�[
		return DWC_RNK_ERROR_INVALID_PARAMETER;
	
	}

	if( (data == NULL) && (size != 0) ){
	
		// �s���ȃp�����[�^�[
		//
		// ���[�U�[��`�f�[�^�̃|�C���^��NULL�̏ꍇ�̓T�C�Y��0�ł���K�v������
		//
		return DWC_RNK_ERROR_INVALID_PARAMETER;

	}

	// ���N�G�X�g���s
	//
	res = DWCi_RankingSessionPutAsync(	category,
										g_rankinginfo.pid,
										region,
										score,
										data,
										size );

	// �G���[����
	switch( res ){

	case DWCi_RANKING_SESSION_ERROR_INVALID_KEY:
		return DWC_RNK_ERROR_PUT_INVALID_KEY;

	case DWCi_RANKING_SESSION_ERROR_NOMEMORY:
		return DWC_RNK_ERROR_PUT_NOMEMORY;

	}

	// �X�e�[�^�X���Z�b�g
	g_rankinginfo.state = DWC_RNK_STATE_PUT_ASYNC;

	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkGetScoreAsync

  Description:	�����L���O�f�[�^���擾����񓯊��������J�n���܂��B

  Arguments:	mode		�擾���[�h
  				category	�J�e�S���[
				region		���[�W�����R�[�h�iOR�ŕ����w��\�j
				param		�p�����[�^�[


  Returns:		DWC_RNK_SUCCESS						����
  				DWC_RNK_IN_ERROR					�G���[������
  				DWC_RNK_ERROR_GET_NOTREADY			��������
				DWC_RNK_ERROR_INVALID_PARAMETER		�s���ȃp�����[�^�[
				DWC_RNK_ERROR_GET_INVALID_KEY		�Í����L�[���s��
				DWC_RNK_ERROR_GET_NOMEMORY			�������[�s��
  *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkGetScoreAsync(	DWCRnkGetMode mode,
									u32 category,
									DWCRnkRegion region,
									DWCRnkGetParam* param ){

	DWCiRankingSessionResult res;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// �X�e�[�^�X�`�F�b�N
	//
	if( ( g_rankinginfo.state != DWC_RNK_STATE_INITIALIZED ) 
		&& (g_rankinginfo.state != DWC_RNK_STATE_COMPLETED ) ){
	
		// ����������Ă��Ȃ�
		return DWC_RNK_ERROR_GET_NOTREADY;
	
	}

	// �p�����[�^�[�`�F�b�N
	//
	if( (category > DWC_RNK_CATEGORY_MAX)
		|| (param == NULL) ){
	
		// �s���ȃp�����[�^�[
		return DWC_RNK_ERROR_INVALID_PARAMETER;
	
	}

	switch( mode ){

	// ����
	//
	case DWC_RNK_GET_MODE_ORDER:

		// �T�C�Y�`�F�b�N
		if( param->size != sizeof(param->order) ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		// �p�����[�^�[�`�F�b�N
		switch( param->order.sort ){

		case DWC_RNK_ORDER_ASC:	// ����
		case DWC_RNK_ORDER_DES:	// �~��
			// ����
			break;

		default:
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		break;

	// �n�C�X�R�A
	//
	case DWC_RNK_GET_MODE_TOPLIST:

		// �T�C�Y�`�F�b�N
		if( param->size != sizeof(param->toplist) ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		// �p�����[�^�[�`�F�b�N
		switch( param->toplist.sort ){

		case DWC_RNK_ORDER_ASC:	// ����
		case DWC_RNK_ORDER_DES:	// �~��
			// ����
			break;

		default:
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		if( param->toplist.limit > DWC_RNK_GET_MAX ||
			param->toplist.limit == 0 ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		break;

	case DWC_RNK_GET_MODE_NEAR:	// ���ӃX�R�A
	case DWC_RNK_GET_MODE_NEAR_HI:
	case DWC_RNK_GET_MODE_NEAR_LOW:

		// �T�C�Y�`�F�b�N
		if( param->size != sizeof(param->near) ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		// �p�����[�^�[�`�F�b�N
		switch( param->near.sort ){

		case DWC_RNK_ORDER_ASC:	// ����
		case DWC_RNK_ORDER_DES:	// �~��
			// ����
			break;

		default:
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		if( param->near.limit > DWC_RNK_GET_MAX ||
			param->near.limit <= 1 ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		break;

	case DWC_RNK_GET_MODE_FRIENDS:	// �F�B�w�胉���L���O


		// �T�C�Y�`�F�b�N
		if( param->size != sizeof(param->friends) ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		// �p�����[�^�[�`�F�b�N
		switch( param->friends.sort ){

		case DWC_RNK_ORDER_ASC:	// ����
		case DWC_RNK_ORDER_DES:	// �~��
			// ����
			break;

		default:
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		if( param->friends.limit > (DWC_RNK_FRIENDS_MAX+1) ||
			param->friends.limit <= 1 ){
			// �s���ȃp�����[�^�[
			return DWC_RNK_ERROR_INVALID_PARAMETER;
		}

		break;

	}

	// ���N�G�X�g���s
	//
	res = DWCi_RankingSessionGetAsync(	mode,
										category,
										g_rankinginfo.pid,
										region,
										param );

	// �G���[����
	switch( res ){

	case DWCi_RANKING_SESSION_ERROR_INVALID_KEY:
		return DWC_RNK_ERROR_GET_INVALID_KEY;

	case DWCi_RANKING_SESSION_ERROR_NOMEMORY:
		return DWC_RNK_ERROR_GET_NOMEMORY;

	}

	// �X�e�[�^�X���Z�b�g
	g_rankinginfo.state = DWC_RNK_STATE_GET_ASYNC;

	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkCancelProcess

  Description:	�񓯊������𒆒f���܂��B
  				���f�����DWC_RNK_STATE_ERROR�ɑJ�ڂ��邽�߁A�ĒʐM���s��
  				�ꍇ�ɂ͏I���A�������������o��K�v������܂��B

  Arguments:	�Ȃ�

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
  				DWC_RNK_ERROR_CANCEL_NOTASK		�񓯊��^�X�N���Ȃ�
 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkCancelProcess(){

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError())return DWC_RNK_IN_ERROR;

	// �X�e�[�^�X�`�F�b�N
	//
	if( (g_rankinginfo.state != DWC_RNK_STATE_PUT_ASYNC) &&
		(g_rankinginfo.state != DWC_RNK_STATE_GET_ASYNC) ){
	
		// �񓯊��������̃^�X�N���Ȃ�
		return DWC_RNK_ERROR_CANCEL_NOTASK;
	
	}

	// �L�����Z������
	//
	DWCi_RankingSessionCancel();

	// �X�e�[�^�X���Z�b�g
	//
	g_rankinginfo.state = DWC_RNK_STATE_ERROR;


	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkProcess

  Description:	�񓯊��������X�V���܂�

  Arguments:	�Ȃ�

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
				DWC_RNK_PROCESS_NOTASK	�񓯊��^�X�N���Ȃ�
 *---------------------------------------------------------------------------*/

DWCRnkError DWC_RnkProcess(){

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// �X�e�[�^�X�`�F�b�N
	//
	if( (g_rankinginfo.state != DWC_RNK_STATE_PUT_ASYNC) &&
		(g_rankinginfo.state != DWC_RNK_STATE_GET_ASYNC) ){
	
		// �񓯊��������̃^�X�N���Ȃ�
		return DWC_RNK_PROCESS_NOTASK;
	
	}

	// ������i�߂�
	//
	switch( DWCi_RankingSessionProcess() ){

	case DWCi_RANKING_SESSION_STATE_ERROR:

		// �G���[����
		g_rankinginfo.state = DWC_RNK_STATE_ERROR;

		break;
	
	case DWCi_RANKING_SESSION_STATE_CANCELED:
		
		//!< �L�����Z��
		g_rankinginfo.state = DWC_RNK_STATE_ERROR;

		break;
	
	case DWCi_RANKING_SESSION_STATE_INITIAL:
		
		//!< �������
		break;

	case DWCi_RANKING_SESSION_STATE_REQUEST:		//!< ���N�G�X�g���s
	case DWCi_RANKING_SESSION_STATE_GETTING_TOKEN:	//!< �g�[�N���擾��
	case DWCi_RANKING_SESSION_STATE_GOT_TOKEN:		//!< �g�[�N���擾����
	case DWCi_RANKING_SESSION_STATE_SENDING_DATA:	//!< �f�[�^���M��

		// ������
		break;

	case DWCi_RANKING_SESSION_STATE_COMPLETED:

		// ����
		g_rankinginfo.state = DWC_RNK_STATE_COMPLETED;

		break;
	
	}


	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkGetState

  Description:	���݂̏�����Ԃ��擾���܂�

  Arguments:	�Ȃ�

  Returns:		DWCRnkState
 *---------------------------------------------------------------------------*/

DWCRnkState	DWC_RnkGetState(){

	return g_rankinginfo.state;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkResGetRow

  Description:	�ʐM�Ŏ�M���������L���O���X�g����C�ӂ̍s�𒊏o���Ď擾���܂�

  Arguments:	out		���ʂ��i�[����DWCRnkData�\���̂̃|�C���^
				index	�擾���郊�X�g�̃C���f�b�N�X

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
				DWC_RNK_ERROR_INVALID_MODE		�s���ȃ��[�h
  				DWC_RNK_ERROR_INVALID_PARAMETER	�s���ȃp�����[�^�[
  				DWC_RNK_ERROR_NOTCOMPLETED		�ʐM������
				DWC_RNK_ERROR_EMPTY_RESPONSE	��̃��X�|���X
 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkResGetRow(	DWCRnkData* out,
										u32 index ){

	DWCRnkData* ptr;
	DWCiRankingResponse res;
	DWCRnkError err;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// ���X�|���X�̃|�C���^�𓾂�
	// 
	err = DWCi_RankingGetResponse( &res );
	if( err != DWC_RNK_SUCCESS )
		return err;

	// ���X�|���X�����v���邩���`�F�b�N
	// 
	switch( res.mode ){

	case DWC_RNK_GET_MODE_ORDER:

		// ���ʎ擾���N�G�X�g�ł̓����L���O���X�g�͂Ȃ��̂ŃG���[
		return DWC_RNK_ERROR_INVALID_MODE;

	case DWC_RNK_GET_MODE_TOPLIST:
	case DWC_RNK_GET_MODE_NEAR:
	case DWC_RNK_GET_MODE_FRIENDS:
	case DWC_RNK_GET_MODE_NEAR_HI:
	case DWC_RNK_GET_MODE_NEAR_LOW:

		break;

	}

	// �s���p�����[�^�[�`�F�b�N
	//
	if( out == NULL )
		return DWC_RNK_ERROR_INVALID_PARAMETER;

	if( index >= res.listheader->count )
		return DWC_RNK_ERROR_INVALID_PARAMETER;

	// �V�[�N����
	//
	ptr = &res.listheader->rankingdata;

	while( index-- > 0 ){

		ptr = (DWCRnkData*)( ((u8*)&ptr->userdata) + ptr->size );

	}

	// �������[�A�N�Z�X�I�[�o�[�̌��m
	// 
	if( (u32)&ptr->userdata + ptr->size > (u32)res.header + res.size ){

		return DWC_RNK_ERROR_INVALID_PARAMETER;

	}

	// �Ԃ�l�̃Z�b�g
	*out = *ptr;
	out->userdata = &ptr->userdata;


	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkResGetRowCount

  Description:	�ʐM�Ŏ�M���������L���O���X�g����C�ӂ̍s�𒊏o���Ď擾���܂�

  Arguments:	count	�s�����擾����ϐ��̃|�C���^

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
				DWC_RNK_ERROR_INVALID_MODE		�s���ȃ��[�h
  				DWC_RNK_ERROR_INVALID_PARAMETER	�s���ȃp�����[�^�[
  				DWC_RNK_ERROR_NOTCOMPLETED		�ʐM������
				DWC_RNK_ERROR_EMPTY_RESPONSE	��̃��X�|���X
 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkResGetRowCount( u32* count ){

	DWCiRankingResponse res;
	DWCRnkError err;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// ���X�|���X�̃|�C���^�𓾂�
	// 
	err = DWCi_RankingGetResponse( &res );
	if( err != DWC_RNK_SUCCESS )
		return err;

	// �s���p�����[�^�[�`�F�b�N
	//
	if( count == NULL )
		return DWC_RNK_ERROR_INVALID_PARAMETER;

	// ���X�|���X�����v���邩���`�F�b�N
	// 
	switch( res.mode ){

	case DWC_RNK_GET_MODE_ORDER:

		// ���ʎ擾���N�G�X�g�ł̓����L���O���X�g�͂Ȃ��̂ŃG���[
		return DWC_RNK_ERROR_INVALID_MODE;

	case DWC_RNK_GET_MODE_TOPLIST:
	case DWC_RNK_GET_MODE_NEAR:
	case DWC_RNK_GET_MODE_FRIENDS:
	case DWC_RNK_GET_MODE_NEAR_HI:
	case DWC_RNK_GET_MODE_NEAR_LOW:
	
		break;

	}

	// �Ԃ�l���Z�b�g
	//
	*count = res.listheader->count;


	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkResGetOrder

  Description:	���ʂ��擾���܂�

  Arguments:	order*	���ʂ��擾����ϐ��̃|�C���^

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
				DWC_RNK_ERROR_INVALID_MODE		�s���ȃ��[�h
  				DWC_RNK_ERROR_INVALID_PARAMETER	�s���ȃp�����[�^�[
  				DWC_RNK_ERROR_NOTCOMPLETED		�ʐM������
				DWC_RNK_ERROR_EMPTY_RESPONSE	��̃��X�|���X
 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkResGetOrder( u32* order ){

	DWCiRankingResponse res;
	DWCRnkError err;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// ���X�|���X�̃|�C���^�𓾂�
	// 
	err = DWCi_RankingGetResponse( &res );
	if( err != DWC_RNK_SUCCESS )
		return err;

	// �s���p�����[�^�[�`�F�b�N
	//
	if( order == NULL )
		return DWC_RNK_ERROR_INVALID_PARAMETER;

	// ���X�|���X�����v���邩���`�F�b�N
	// 
	if( res.mode != DWC_RNK_GET_MODE_ORDER )
		return DWC_RNK_ERROR_INVALID_MODE;

	// �Ԃ�l���Z�b�g
	//
	*order = res.orderheader->order;


	return DWC_RNK_SUCCESS;

}


/*---------------------------------------------------------------------------*
  Name:		DWC_RnkResGetTotal

  Description:	�������擾���܂�

  Arguments:	total*	���ʂ��擾����ϐ��̃|�C���^

  Returns:		DWC_RNK_SUCCESS					����
  				DWC_RNK_IN_ERROR				�G���[������
  				DWC_RNK_ERROR_INVALID_PARAMETER	�s���ȃp�����[�^�[
  				DWC_RNK_ERROR_NOTCOMPLETED		�ʐM������
				DWC_RNK_ERROR_EMPTY_RESPONSE	��̃��X�|���X
 *---------------------------------------------------------------------------*/

DWCRnkError	DWC_RnkResGetTotal( u32* total ){

	DWCiRankingResponse res;
	DWCRnkError err;

	// DWC�G���[���������Ă���ꍇ�͂͂���
	if (DWCi_IsError()) return DWC_RNK_IN_ERROR;

	// ���X�|���X�̃|�C���^�𓾂�
	// 
	err = DWCi_RankingGetResponse( &res );
	if( err != DWC_RNK_SUCCESS )
		return err;

	// �s���p�����[�^�[�`�F�b�N
	//
	if( total == NULL )
		return DWC_RNK_ERROR_INVALID_PARAMETER;

	// �Ԃ�l���Z�b�g
	//
	*total = res.orderheader->total;


	return DWC_RNK_SUCCESS;

}

