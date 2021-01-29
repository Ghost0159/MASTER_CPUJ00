//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		bucket.c
 *	@brief		�o�P�b�g�Q�[��
 *	@author		tomoya takahashi
 *	@data		2007.06.19
 *
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "system/wipe.h"
#include "system/snd_tool.h"
#include "system/arc_util.h"

#include "ballslow_data.naix"

#include "application/wifi_lobby/minigame_tool.h"

#include "bct_common.h"
#include "application/bucket/comm_bct_command.h"
#include "application/bucket/comm_bct_command_func.h"
#include "application/bucket/bct_surver.h"
#include "application/bucket/bct_client.h"
#include "application/bucket/bct_local.h"
#include "application/bucket.h"


//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define BCT_VRAMTR_MAN_NUM	( 32 )

// ���C���V�[�P���X
enum{
	BCT_MAINSEQ_ENTRY_INIT,
	BCT_MAINSEQ_ENTRY_WAIT,
	BCT_MAINSEQ_INIT,
	BCT_MAINSEQ_WIPE_IN,
	BCT_MAINSEQ_WIPE_INWAIT,
	BCT_MAINSEQ_STARTWAIT,
	BCT_MAINSEQ_COUNTDOWN,
	BCT_MAINSEQ_MAIN,
	BCT_MAINSEQ_SCORECOMM,
	BCT_MAINSEQ_SCORECOMMWAIT,
	BCT_MAINSEQ_RESULT,
	BCT_MAINSEQ_WIPE_OUT,
	BCT_MAINSEQ_WIPE_OUTWAIT,
	BCT_MAINSEQ_DELETE,
	BCT_MAINSEQ_RESULT_INIT,
	BCT_MAINSEQ_RESULT_WAIT,
};

// PLNETID�̃e�[�u���ɏ����l�Ƃ��ē����Ă���l
#define BCT_PLNETID_DEF		( 0xffff )

// �Ƃ肠����
#ifdef BCT_DEBUG_TIMESELECT
static u32 BCT_TIME_LIMIT = 1500;	// 50
#else
#define BCT_TIME_LIMIT	(30*40)
#endif


#ifdef PM_DEBUG
//#define BCT_DEBUG_ENTRY_CHG	// �G���g���[��ʂ����낢��ς���
#endif
#ifdef BCT_DEBUG_ENTRY_CHG
static int DEBUG_ENTRY_COUNT = 0;
#endif

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------



//-------------------------------------
///	�o�P�b�g�Q�[�����[�N
//=====================================
typedef struct _BUCKET_WK{
	MNGM_ENTRYWK*		p_entry;
	MNGM_RESULTWK*		p_result;
	MNGM_ENRES_PARAM	enres_param;
	MNGM_RESULT_PARAM	result_param;
	BCT_SURVER*			p_surver;
	BCT_CLIENT*			p_client;
	BOOL start;
	BOOL end;
	BOOL score_get;	// �݂�Ȃ̓��_����M������
	u16 netid;
	u16 plno;
	BCT_GAMEDATA		gamedata;	// �Q�[���f�[�^
	u32 raregame;		// rare�Q�[��
	u32	comm_num;

	u32 err_seq;	// �r�����f�V�[�P���X
	 


	u32 check_middle_score_count;
	BOOL all_middle_count_ok;		// �e����q�ɒʐM����������

	u8 middle_score_get_count[ BCT_PLAYER_NUM ];	// �݂�Ȃ̓r�����_����M������
} ;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void BCT_VBlank( void* p_work );
static void BCT_ClientNutsSend( BUCKET_WK* p_wk );
static void BCT_ClientMiddleScoreSend( BUCKET_WK* p_wk );
static void BCT_GAMEDATA_Load( BUCKET_WK* p_wk, u32 heapID );
static void BCT_GAMEDATA_Release( BUCKET_WK* p_wk );

static void BCT_ErrAllSysEnd( BUCKET_WK* p_wk, BUCKET_PROC_WORK* pp );

static BOOL BCT_GAMESendData( BUCKET_WK* p_wk, int command, const void* data, int size );


//----------------------------------------------------------------------------
/**
 *	@brief	�o�P�b�g�Q�[���v���Z�X�@�쐬
 *
 *	@param	p_proc		���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT BucketProc_Init( PROC * p_proc, int * p_seq )
{
	BUCKET_WK* p_wk;
	BUCKET_PROC_WORK* pp = PROC_GetParentWork(p_proc);
	BOOL result;

#ifdef BCT_DEBUG_TIMESELECT
	OS_Printf( "���[�h�������ł������� x[40] a[45] b[50] y[55]\n" );
	result = FALSE;

	if( sys.cont & PAD_BUTTON_X ){
		BCT_TIME_LIMIT = 30*40;
		result = TRUE;
		OS_Printf( "40�b���[�h\n" );
	}else if( sys.cont & PAD_BUTTON_A ){
		BCT_TIME_LIMIT = 30*45;
		result = TRUE;
		OS_Printf( "45�b���[�h\n" );
	}else if( sys.cont & PAD_BUTTON_B ){
		BCT_TIME_LIMIT = 30*50;
		result = TRUE;
		OS_Printf( "50�b���[�h\n" );
	}else if( sys.cont & PAD_BUTTON_Y ){
		BCT_TIME_LIMIT = 30*55;
		result = TRUE;
		OS_Printf( "55�b���[�h\n" );
	}
	if( result == FALSE ){
		return PROC_RES_CONTINUE;
	}
#endif

	// �q�[�v�쐬
	sys_CreateHeap( HEAPID_BASE_APP, HEAPID_BUCKET, 0x60000 );

	// ���[�N�쐬
	p_wk = PROC_AllocWork( p_proc, sizeof(BUCKET_WK), HEAPID_BUCKET );
	memset( p_wk, 0, sizeof(BUCKET_WK) );


	// �G���g���[�E���ʉ�ʃp�����[�^�쐬
	MNGM_ENRES_PARAM_Init( &p_wk->enres_param, pp->wifi_lobby, pp->p_save, pp->vchat, &pp->lobby_wk );

	// �Q�[���\���f�[�^�ǂݍ���
	BCT_GAMEDATA_Load( p_wk, HEAPID_BUCKET );


	return PROC_RES_FINISH;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�P�b�g�Q�[���v���Z�X�@���s
 *
 *	@param	p_proc		���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT BucketProc_Main( PROC* p_proc, int* p_seq )
{
	BUCKET_WK* p_wk = PROC_GetWork( p_proc );
	BUCKET_PROC_WORK* pp = PROC_GetParentWork(p_proc);
	BOOL result;
	u32 check;

	// �ؒf�G���[����
	if( (MNGM_ERROR_CheckDisconnect( &p_wk->enres_param ) == TRUE) ){

		switch( p_wk->err_seq ){
		case 0:
			// �^�����ɂ���
			if( WIPE_SYS_EndCheck() == TRUE ){
				WIPE_SYS_ExeEnd();
			}
			WIPE_SetBrightnessFadeOut(WIPE_FADE_BLACK);
			p_wk->err_seq ++;
			break;

		case 1:
			// �ʐM�ؒf
			if( MNGM_ERROR_DisconnectWait( &p_wk->enres_param ) == TRUE ){
				p_wk->err_seq ++;
			}
			break;

		case 2:
			// ���[�N�j��
			// �S�V�X�e����~���j��
			BCT_ErrAllSysEnd( p_wk, pp );
			return PROC_RES_FINISH;

		defalut:
			GF_ASSERT(0);
			return PROC_RES_FINISH;
		}
		return PROC_RES_CONTINUE;
	}

	
	// ���C�����[�v����
	switch( (*p_seq) ){
	case BCT_MAINSEQ_ENTRY_INIT:
#ifdef BCT_DEBUG_ENTRY_CHG
		switch(DEBUG_ENTRY_COUNT){
		case 0:
			TOMOYA_PRINT( "slow\n" );
			p_wk->p_entry = MNGM_ENTRY_InitBallSlow( &p_wk->enres_param, HEAPID_BUCKET );
			break;

		case 1:
			TOMOYA_PRINT( "balance\n" );
			p_wk->p_entry = MNGM_ENTRY_InitBalanceBall( &p_wk->enres_param, HEAPID_BUCKET );
			break;

		case 2:
			TOMOYA_PRINT( "balloon\n" );
			p_wk->p_entry = MNGM_ENTRY_InitBalloon( &p_wk->enres_param, HEAPID_BUCKET );
			break;
		}
#else
		p_wk->p_entry = MNGM_ENTRY_InitBallSlow( &p_wk->enres_param, HEAPID_BUCKET );
#endif
		(*p_seq) ++;
		break;

	case BCT_MAINSEQ_ENTRY_WAIT:
		if( MNGM_ENTRY_Wait( p_wk->p_entry ) == TRUE ){
			p_wk->raregame	= MNGM_ENTRY_GetRareGame( p_wk->p_entry );
			MNGM_ENTRY_Exit( p_wk->p_entry );
			p_wk->p_entry = NULL;
			(*p_seq) ++;
		}
		break;
		
	case BCT_MAINSEQ_INIT:
		if( sys.cont & PAD_BUTTON_A ){
			break;
		}

		// V�u�����NH�u�����N�֐��ݒ�
		sys_VBlankFuncChange( BCT_VBlank, p_wk );	// VBlank�Z�b�g
		sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

		// �e��t���O�̏�����
		p_wk->start		= FALSE;
		p_wk->end		= FALSE;
		p_wk->score_get = FALSE;
		memset( p_wk->middle_score_get_count, 0, sizeof(u8)*BCT_PLAYER_NUM );

		// VramTransferManager������
		initVramTransferManagerHeap( BCT_VRAMTR_MAN_NUM, HEAPID_BUCKET );

		// �ʐM�J�n
		CommCommandBCTInitialize( p_wk );

		// �ŏ��͒ʐM����
		p_wk->all_middle_count_ok = TRUE;

		// �ʐM�l�����擾
		p_wk->comm_num = CommInfoGetEntryNum();
		GF_ASSERT( p_wk->comm_num > 1 );

		// �ʐMID�擾
		p_wk->netid = CommGetCurrentID();
		p_wk->plno = MNGM_ENRES_PARAM_GetNetIDtoPlNO( &p_wk->enres_param, p_wk->netid );

		// �T�[�o�[������
		if( p_wk->netid == COMM_PARENT_ID ){
			p_wk->p_surver = BCT_SURVER_Init( HEAPID_BUCKET, BCT_TIME_LIMIT, p_wk->comm_num, &p_wk->gamedata );

			// �r���o�߃X�R�A�@�J�E���^�l��ݒ�
			p_wk->check_middle_score_count = 1;
		}
		p_wk->p_client = BCT_CLIENT_Init( HEAPID_BUCKET, BCT_TIME_LIMIT, p_wk->comm_num, p_wk->plno, &p_wk->gamedata );

		WirelessIconEasy();  // �ڑ����Ȃ̂ŃA�C�R���\��

		// VChatOn
		if( pp->vchat ){
			// �{�C�X�`���b�g�J�n
			mydwc_startvchat( HEAPID_BUCKET );
			TOMOYA_PRINT( "vct on\n" );
		}

		// ���A�Q�[���`�F�b�N���s��
		{
			BCT_GAME_TYPE_WK gametype;

			TOMOYA_PRINT( "p_wk->raregame = %d\n", p_wk->raregame );

			switch( p_wk->raregame ){
			case MNGM_RAREGAME_BUCKET_NORMAL:
				// ���̂܂�
				gametype.scale_rev	= FALSE;
				gametype.rota_rev	= FALSE;
				break;
			case MNGM_RAREGAME_BUCKET_REVERSE:
				// �t��]
				gametype.scale_rev	= FALSE;
				gametype.rota_rev	= TRUE;
				break;
			case MNGM_RAREGAME_BUCKET_BIG:
				// �X�P�[�����]
				gametype.scale_rev	= TRUE;
				gametype.rota_rev	= FALSE;
				break;

			default:
				GF_ASSERT(0);
				break;
			}

			BCT_CLIENT_GameTypeSet( p_wk->p_client, &gametype );
		}
		

//		Snd_DataSetByScene( SND_SCENE_BCT, SEQ_KINOMI1, 1 );


		CommTimingSyncStart(BCT_SYNCID_GAMESTART);

		TOMOYA_PRINT( "init\n" );
		(*p_seq) ++;
		break;

	case BCT_MAINSEQ_WIPE_IN:

		// ��������������܂ő҂�
		if(!CommIsTimingSync(BCT_SYNCID_GAMESTART)){
			break;
		}

		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_DOORIN, 
				WIPE_TYPE_DOORIN, WIPE_FADE_OUTCOLOR, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_BUCKET );
		(*p_seq) ++;
		break;

	case BCT_MAINSEQ_WIPE_INWAIT:
		BCT_CLIENT_StartMain( p_wk->p_client, BCT_CLIENT_STARTEVENT_NONE );	// �}���m�[�����܂킵�Ă���
		if( WIPE_SYS_EndCheck() ){
			// �e�Ȃ�Q�[���X�^�[�g�𑗐M
			if( p_wk->netid == COMM_PARENT_ID ){
				result = CommSendData( CNM_BCT_START, NULL,  0 );
				if( result ){
					(*p_seq) ++;
				}
			}else{
				(*p_seq) ++;
			}
		}
		break;

	case BCT_MAINSEQ_STARTWAIT:
		BCT_CLIENT_StartMain( p_wk->p_client, BCT_CLIENT_STARTEVENT_NONE );	// �}���m�[�����܂킵�Ă���
		if( p_wk->start ){
			(*p_seq) ++;
		}
		break;

	case BCT_MAINSEQ_COUNTDOWN:	// �J�E���g�_�E������
		result = BCT_CLIENT_StartMain( p_wk->p_client, BCT_CLIENT_STARTEVENT_COUNT_START );	// �J�E���g�_�E��
		if( result == FALSE ){
			(*p_seq) ++;
		}
		break;

	case BCT_MAINSEQ_MAIN:
		if( (p_wk->end) ){
			u32 score;
			score = BCT_CLIENT_ScoreGet( p_wk->p_client );
			// �����̃X�R�A�𑗐M
			result = CommSendData( CNM_BCT_SCORE, &score, sizeof(u32) );

			if( result ){
				(*p_seq) ++;
			}
			break;
		}

		
		if( p_wk->netid == COMM_PARENT_ID ){
			result = BCT_SURVER_Main( p_wk->p_surver );

			// �Q�[�����x���̕ύX�𑗐M
			if( BCT_SURVER_CheckGameLevelChange( p_wk->p_surver ) ){
				u32 level;
				BOOL result;
				level = BCT_SURVER_GetGameLevel( p_wk->p_surver );
				result = BCT_GAMESendData( p_wk, CNM_BCT_GAME_LEVEL, &level, sizeof(u32) );

				// ���M�ł������`�F�b�N
				if( result == TRUE ){
					// ���M�����̂Ńt���OOFF
					BCT_SURVER_ClearGameLevelChange( p_wk->p_surver );
				}
			}
			
			// �I���̑��M
			if( result == FALSE ){
				BCT_GAMESendData( p_wk, CNM_BCT_END, NULL,  0 );
			}
#ifdef BCT_DEBUG_NOT_TIMECOUNT
			if( sys.trg & PAD_BUTTON_CANCEL ){	
				BCT_GAMESendData( p_wk, CNM_BCT_END, NULL,  0 );
			}
#endif	// BCT_DEBUG_NOT_TIMECOUNT
		}
		BCT_CLIENT_Main( p_wk->p_client );

		// �؂̎��f�[�^���M
		BCT_ClientNutsSend( p_wk );

		// �r���o�߂̑��M�`�F�b�N
		BCT_ClientMiddleScoreSend( p_wk );
		break;

	case BCT_MAINSEQ_SCORECOMM:
		BCT_CLIENT_EndMain( p_wk->p_client, BCT_CLIENT_STARTEVENT_NONE );	
		if( p_wk->netid == COMM_PARENT_ID ){
			// �e�݂͂�Ȃ��瓾�_�����W���Ă݂�Ȃɑ���
			result = BCT_SURVER_ScoreAllUserGetCheck( p_wk->p_surver );
			if( result == TRUE ){
				BCT_SCORE_COMM score;
				BCT_SURVER_ScoreGet( p_wk->p_surver, &score );
				result = CommSendData( CNM_BCT_ALLSCORE, &score, sizeof(BCT_SCORE_COMM) );
				if( result ){
					(*p_seq) ++;
				}
			}
		}else{
			// �N���C�A���g�͂����Ɍ��ʎ�M����
			(*p_seq) ++;
		}
		break;

	case BCT_MAINSEQ_SCORECOMMWAIT:
		BCT_CLIENT_EndMain( p_wk->p_client, BCT_CLIENT_STARTEVENT_NONE );	
		if( p_wk->score_get == TRUE ){
			(*p_seq) ++;
		}
		break;
		
	case BCT_MAINSEQ_RESULT:
		// ���ʔ��\
		result = BCT_CLIENT_EndMain( p_wk->p_client, BCT_CLIENT_ENDEVENT_RESULTON );	
		if( result == FALSE ){
			(*p_seq)++;
		}
		break;

	case BCT_MAINSEQ_WIPE_OUT:
		WIPE_SYS_Start( WIPE_PATTERN_WMS, WIPE_TYPE_DOOROUT, 
				WIPE_TYPE_DOOROUT, WIPE_FADE_OUTCOLOR, 
				WIPE_DEF_DIV, WIPE_DEF_SYNC, HEAPID_BUCKET );
		BCT_CLIENT_EndMain( p_wk->p_client, BCT_CLIENT_ENDEVENT_RESULTON );	
		(*p_seq) ++;
		break;

	case BCT_MAINSEQ_WIPE_OUTWAIT:
		BCT_CLIENT_EndMain( p_wk->p_client, BCT_CLIENT_ENDEVENT_RESULTON );	
		if( WIPE_SYS_EndCheck() ){

			// �I�������J�n
			CommTimingSyncStart(BCT_SYNCID_END);
			(*p_seq) ++;
		}
		break;

	case BCT_MAINSEQ_DELETE:
		// ��������������܂ő҂�
		if(!CommIsTimingSync(BCT_SYNCID_END)){
			TOMOYA_PRINT( "sync_wait\n" );
			return PROC_RES_CONTINUE;
		}

		// VChatOff
		if( pp->vchat ){
			// �{�C�X�`���b�g�I��
			mydwc_stopvchat();
			TOMOYA_PRINT( "vct off\n" );
		}

		if( p_wk->netid == COMM_PARENT_ID ){
			BCT_SURVER_Delete( p_wk->p_surver );
			p_wk->p_surver = NULL;
		}
		BCT_CLIENT_Delete( p_wk->p_client );
		p_wk->p_client = NULL;

		sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
		sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

		//
		DellVramTransferManager();

		(*p_seq)++;
		break;


	case BCT_MAINSEQ_RESULT_INIT:
		{
			u32 rank;

			// ���ʌv�Z
			MNGM_RESULT_CalcRank( &p_wk->result_param, p_wk->enres_param.num );

			// ���ʂ���K�W�F�b�g�A�b�v�`�F�b�N
			rank = p_wk->result_param.result[ p_wk->plno ];
			
		}
#ifdef BCT_DEBUG_ENTRY_CHG
		switch(DEBUG_ENTRY_COUNT){
		case 0:
			TOMOYA_PRINT( "slow\n" );
			p_wk->p_result = MNGM_RESULT_InitBallSlow( &p_wk->enres_param, &p_wk->result_param, HEAPID_BUCKET );
			break;

		case 1:
			TOMOYA_PRINT( "balance\n" );
			p_wk->p_result = MNGM_RESULT_InitBalanceBall( &p_wk->enres_param, &p_wk->result_param, HEAPID_BUCKET );
			break;

		case 2:
			TOMOYA_PRINT( "balloon\n" );
			p_wk->result_param.balloon = 14;
			p_wk->p_result = MNGM_RESULT_InitBalloon( &p_wk->enres_param, &p_wk->result_param, HEAPID_BUCKET );
			break;
		}

		DEBUG_ENTRY_COUNT  = (DEBUG_ENTRY_COUNT + 1) % 3;
#else
		p_wk->p_result = MNGM_RESULT_InitBallSlow( &p_wk->enres_param, &p_wk->result_param, HEAPID_BUCKET );
#endif
		(*p_seq)++;
		break;
		
	case BCT_MAINSEQ_RESULT_WAIT:
		{
			BOOL replay;
			
			if( MNGM_RESULT_Wait( p_wk->p_result ) == TRUE ){
				
				// replay�`�F�b�N
				replay = MNGM_RESULT_GetReplay( p_wk->p_result );
				
				MNGM_RESULT_Exit( p_wk->p_result );
				p_wk->p_result = NULL;
				if( replay == FALSE ){
					return PROC_RES_FINISH;
				}else{
					(*p_seq) = BCT_MAINSEQ_ENTRY_INIT;
				}
			}
		}
		break;
	}

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�P�b�g�Q�[���v���Z�X�@�j��
 *
 *	@param	p_proc		���[�N
 *	@param	p_seq		�V�[�P���X
 *
 *	@retval	PROC_RES_CONTINUE = 0,		///<����p����
 *	@retval	PROC_RES_FINISH,			///<����I��
 */
//-----------------------------------------------------------------------------
PROC_RESULT BucketProc_End( PROC* p_proc, int* p_seq )
{
	BUCKET_WK* p_wk = PROC_GetWork( p_proc );
	BUCKET_PROC_WORK* pp = PROC_GetParentWork(p_proc);
	BOOL dis_error;

	switch( *p_seq ){
	case 0:


		dis_error = MNGM_ERROR_CheckDisconnect( &p_wk->enres_param );
		
		// �Q�[���\���f�[�^�j��
		BCT_GAMEDATA_Release( p_wk );

		// ���[�N�j��
		PROC_FreeWork( p_proc );

		// �q�[�v�j��
		sys_DeleteHeap( HEAPID_BUCKET );

		CommStateSetErrorCheck(FALSE,TRUE);

		// �ؒf�G���[���������Ă���Ȃ炻�̂܂܏I���
		if( dis_error == TRUE ){
			return PROC_RES_FINISH;
		}
		

		// �ʐM����
		CommTimingSyncStart(BCT_SYNCID_ERR_END);
		(*p_seq)++;
		break;

	case 1:
		if(	CommIsTimingSync(BCT_SYNCID_ERR_END) || 
			(CommGetConnectNum() < CommInfoGetEntryNum()) ){	// �l�������Ȃ��Ȃ����炻�̂܂ܔ�����
			return PROC_RES_FINISH;
		}
		break;

	default:
		break;
	}
	

	return PROC_RES_CONTINUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�P�b�g�J�n
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void Bucket_StartSet( BUCKET_WK* p_wk )
{
	p_wk->start = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�o�P�b�g�I��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void Bucket_EndSet( BUCKET_WK* p_wk )
{
	p_wk->end = TRUE;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[�ɖ؂̎��f�[�^��n��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�؂̎��f�[�^
 *	@param	netid		�l�b�gID
 */
//-----------------------------------------------------------------------------
void Bucket_SurverNutsSet( BUCKET_WK* p_wk, const BCT_NUT_COMM* cp_data, u32 netid )
{
	u32 pl_no;
	pl_no = MNGM_ENRES_PARAM_GetNetIDtoPlNO( &p_wk->enres_param, netid );	// �v���C���[NO�ɂ��ēn��
	BCT_SURVER_SetNutData( p_wk->p_surver, cp_data, pl_no );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g�ɖ؂̎��f�[�^��n��
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�؂̎��f�[�^
 *	@param	netid		�l�b�gID
 */
//-----------------------------------------------------------------------------
void Bucket_ClientNutsSet( BUCKET_WK* p_wk, const BCT_NUT_COMM* cp_data, u32 netid )
{
	BCT_CLIENT_NutsDataSet( p_wk->p_client, cp_data );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�T�[�o�[���[�N�ɃX�R�A��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	score		�X�R�A
 *	@param	netid		�l�b�gID
 */
//-----------------------------------------------------------------------------
void Bucket_SurverScoreSet( BUCKET_WK* p_wk, u32 score, u32 netid )
{
	u32 pl_no;
	pl_no = MNGM_ENRES_PARAM_GetNetIDtoPlNO( &p_wk->enres_param, netid );	// �v���C���[NO�ɂ��ēn��
	if( p_wk->p_surver != NULL ){
		BCT_SURVER_ScoreSet( p_wk->p_surver, score, pl_no );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�S�����̃X�R�A����M
 *
 *	@param	p_wk		���[�N
 *	@param	cp_data		�f�[�^
 *	@param	netid		�l�b�gID
 */
//-----------------------------------------------------------------------------
void Bucket_ClientAllScoreSet( BUCKET_WK* p_wk, const BCT_SCORE_COMM* cp_data, u32 netid )
{
	BCT_CLIENT_AllScoreSet( p_wk->p_client, cp_data );
	p_wk->score_get = TRUE;


	// �X�R�A���猋�ʔ��\�p�����[�^���쐬����
	{
		int i;
		for( i=0; i<BCT_PLAYER_NUM; i++ ){
			p_wk->result_param.score[i]		= cp_data->score[i];
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g���[�N�ɃQ�[���f�[�^�̒i�K��ݒ�
 *
 *	@param	p_wk	���[�N
 *	@param	idx		�Q�[���f�[�^�̒i�K
 */
//-----------------------------------------------------------------------------
void Bucket_ClientGameDataIdxSet( BUCKET_WK* p_wk, u32 idx )
{
	BCT_CLIENT_GameDataIdxSet( p_wk->p_client, idx );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g�ɓr���̓��_��ݒ肷��
 *
 *	@param	p_wk		���[�N
 *	@param	score		���_
 *	@param	netid		�l�b�gID
 */
//-----------------------------------------------------------------------------
void Bucket_ClientMiddleScoreSet( BUCKET_WK* p_wk, u32 score, u32 netid )
{
	u32 pl_no;
	int i;


	pl_no = MNGM_ENRES_PARAM_GetNetIDtoPlNO( &p_wk->enres_param, netid );	// �v���C���[NO�ɂ��ēn��
	BCT_CLIENT_MiddleScoreSet( p_wk->p_client, score, pl_no );

	// �r���̓��_��M
	p_wk->middle_score_get_count[ pl_no ] ++;

	// �e�Ȃ�S������r���o�߂�������q�@�ɒʐM�����o��
	if( p_wk->netid == COMM_PARENT_ID ){
		BOOL send_ok = TRUE;

		for( i=0; i<p_wk->comm_num; i++ ){
			if( p_wk->check_middle_score_count > p_wk->middle_score_get_count[i] ){
				send_ok = FALSE;
			}
		}

		if( send_ok == TRUE ){
			OS_TPrintf( "�e middle score ok\n" );
			p_wk->check_middle_score_count ++;	// ���̃J�E���g�l��
			CommSendData( CNM_BCT_MIDDLESCORE_OK, NULL, 0 );
		}
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���o�߃X�R�A�擾	�i�ʐM���j
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void Bucket_ClientMiddleScoreOkSet( BUCKET_WK* p_wk )
{
	OS_TPrintf( "middle score ok\n" );
	
	p_wk->all_middle_count_ok = TRUE;

	// �N���C�A���g�V�X�e���̃^�C���J�E���g��i�߂�
	// OFF
/*	BCT_CLIENT_TimeCountFlagSet( p_wk->p_client, TRUE );
	if( p_wk->netid == COMM_PARENT_ID ){
		BCT_SURVER_SetCountDown( p_wk->p_surver, TRUE );
	}//*/
}


//-----------------------------------------------------------------------------
/**
 *				�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	VBlank�֐�
 */
//-----------------------------------------------------------------------------
static void BCT_VBlank( void* p_work )
{
	BUCKET_WK* p_wk = p_work;

	BCT_CLIENT_VBlank( p_wk->p_client );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�N���C�A���g�̖؂̎��f�[�^�𑗐M����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_ClientNutsSend( BUCKET_WK* p_wk )
{
	BCT_NUT_COMM comm;
	
#ifndef BCT_DEBUG_AUTOSLOW
	while( BCT_CLIENT_NutsDataGet( p_wk->p_client, &comm ) == TRUE ){
		// ���M�ł��Ȃ��Ƃ������邯��ǁA����͂���ł��傤���Ȃ��Ƃ���
		BCT_GAMESendData( p_wk, CNM_BCT_NUTS, &comm, sizeof(BCT_NUT_COMM) );
	}
#else

	static u8 timing = 2;
	static u8 count = 0;

	while( BCT_CLIENT_NutsDataGet( p_wk->p_client, &comm ) == TRUE ){
	}

	// ����^�C�~���O�ő��M����
	count ++;
	if( count >= timing ){
		comm.pl_no = p_wk->plno;
		comm.in_flag = TRUE;
		comm.way.x = 2251;
		comm.way.y = 3422;
		comm.way.z = 3;
		comm.power = 18002;
		comm.mat.x = -266;
		comm.mat.y = 0;
		comm.mat.z = 2;
			
		count = 0;
		BCT_GAMESendData( p_wk, CNM_BCT_NUTS, &comm, sizeof(BCT_NUT_COMM) );
	}

	if( sys.trg & PAD_KEY_UP ){
		timing ++;
		OS_Printf( "send timing %d\n", timing );
	}else if( sys.trg & PAD_KEY_DOWN ){
		timing --;
		OS_Printf( "send timing %d\n", timing );
	}
#endif
}

//----------------------------------------------------------------------------
/**
 *	@brief	�r���o�ߓ��_�𑗐M����
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_ClientMiddleScoreSend( BUCKET_WK* p_wk )
{
	s32 time;
	u32 score;
	BOOL result;

	time = BCT_CLIENT_GetTime( p_wk->p_client );

	// �ŏ��͑���Ȃ�
	if( time == 0 ){
		return;
	}

	if( (time % BCT_MIDDLE_SCORE_SEND_TIMING) == 0 ){

		if( p_wk->all_middle_count_ok == TRUE ){
			// ���M
			score = BCT_CLIENT_ScoreGet( p_wk->p_client );
			result = BCT_GAMESendData( p_wk, CNM_BCT_MIDDLESCORE, &score, sizeof(u32) );
			GF_ASSERT( result );

			// OFF
/*			// �N���C�A���g�V�X�e���̃^�C���J�E���g���~
			BCT_CLIENT_TimeCountFlagSet( p_wk->p_client, FALSE );
			if( p_wk->netid == COMM_PARENT_ID ){
				BCT_SURVER_SetCountDown( p_wk->p_surver, FALSE );
			}
//*/
			// �e����ʐM����҂�
			p_wk->all_middle_count_ok = FALSE;

			OS_TPrintf( "middle score ok wait\n" );
		}
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���\���f�[�^�ǂݍ���
 *
 *	@param	p_wk		���[�N
 *	@param	heapID		�q�[�vID
 */
//-----------------------------------------------------------------------------
static void BCT_GAMEDATA_Load( BUCKET_WK* p_wk, u32 heapID )
{
	u32 size;
	int i;
	
	p_wk->gamedata.p_tbl	= ArcUtil_LoadEx( ARC_BUCKET_DATA, NARC_ballslow_data_bucket_data_bin, FALSE, heapID, ALLOC_TOP, &size );
	p_wk->gamedata.tblnum	= size / sizeof(BCT_GAMEDATA_ONE);

	// MYSTATUSTBL
	for( i=0; i<p_wk->enres_param.num; i++ ){
		p_wk->gamedata.cp_status[i]	= MNGM_ENRES_PARAM_GetMystatus( &p_wk->enres_param, i );
		p_wk->gamedata.vip[i]		= MNGM_ENRES_PARAM_GetVipFlag( &p_wk->enres_param, i );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[���\���f�[�^�j��
 *
 *	@param	p_wk		���[�N
 */
//-----------------------------------------------------------------------------
static void BCT_GAMEDATA_Release( BUCKET_WK* p_wk )
{
	sys_FreeMemoryEz( p_wk->gamedata.p_tbl );
	p_wk->gamedata.p_tbl	= NULL;
	p_wk->gamedata.tblnum	= 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�ʐM�G���[	�I������
 *
 *	@param	p_wk	���[�N
 *	@param	pp		�p�����[�^
 */
//-----------------------------------------------------------------------------
static void BCT_ErrAllSysEnd( BUCKET_WK* p_wk, BUCKET_PROC_WORK* pp )
{
	sys_VBlankFuncChange( NULL, NULL );	// VBlank�Z�b�g
	sys_HBlankIntrStop();	//HBlank���荞�ݒ�~

	// �S�������j��
	if( p_wk->p_entry != NULL ){
		MNGM_ENTRY_Exit( p_wk->p_entry );
		p_wk->p_entry = NULL;
	}
	if( p_wk->p_client ){
		if( p_wk->netid == COMM_PARENT_ID ){
			BCT_SURVER_Delete( p_wk->p_surver );
		}
		BCT_CLIENT_Delete( p_wk->p_client );

		DellVramTransferManager();
	}
	if( pp->vchat ){
		// �{�C�X�`���b�g�I��
		mydwc_stopvchat();
	}
	if( p_wk->p_result != NULL ){
		MNGM_RESULT_Exit( p_wk->p_result );
		p_wk->p_result = NULL;
	}
}


//----------------------------------------------------------------------------
/**
 *	@brief	�Q�[�����i�؂̎���r���o�߃X�R�A�f�[�^�j�̑��M�p�֐�
 *
 *	@param	p_wk		���[�N
 *	@param	command		�R�}���h
 *	@param	data		�f�[�^
 *	@param	size		�T�C�Y
 *
 *	@retval	TRUE	���M�ł���
 *	@retval	FALSE	���M���s
 */
//-----------------------------------------------------------------------------
static BOOL BCT_GAMESendData( BUCKET_WK* p_wk, int command, const void* data, int size )
{
	if( p_wk->all_middle_count_ok == FALSE ){
		OS_TPrintf( "!!!!!!not send data!!!!\n" );
		return FALSE;	// �ꏏ�̂Ƃ���܂ŗ��ĂȂ��̂ő��M�ł��Ȃ�
	}
	
	// �Q���v���C���[�S���̓r���o�ߎ�M�J�E���^�������łȂ��Ƒ��M���Ă͂����Ȃ�
	return CommSendData( command, data, size );
}
