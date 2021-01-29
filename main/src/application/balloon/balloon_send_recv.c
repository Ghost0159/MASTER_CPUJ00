//==============================================================================
/**
 * @file	balloon_send_recv.c
 * @brief	���D�~�j�Q�[���F����M�f�[�^�쐬������
 * @author	matsuda
 * @date	2007.11.27(��)
 */
//==============================================================================
#include "common.h"
#include "system/clact_tool.h"
#include "system/palanm.h"
#include "system/pmfprint.h"
#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/fontproc.h"
#include "system/msgdata.h"
#include "system/particle.h"
#include "system/brightness.h"
#include "system/snd_tool.h"
#include "communication/communication.h"
#include "communication/wm_icon.h"
#include "msgdata/msg.naix"
#include "system/wipe.h"
#include "communication/wm_icon.h"
#include "system/msgdata_util.h"
#include "system/procsys.h"
#include "system/d3dobj.h"
#include "system/fontoam.h"

#include "balloon_common.h"
#include "balloon_comm_types.h"
#include "balloon_game_types.h"
#include "application/balloon.h"
#include "balloon_game.h"
#include "balloon_tcb_pri.h"
#include "balloon_sonans.h"
#include "balloon_tool.h"
#include "comm_command_balloon.h"
#include "balloon_send_recv.h"


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void RecvBalloon_Timing(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);
static void RecvBalloon_Air(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);
static void RecvBalloon_Exploded(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);
static void RecvBalloon_Appear(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);
static void RecvBalloon_Start(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);
static void RecvBalloon_Finish(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);
static void RecvBalloon_TouchPenDemo(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv);

//==============================================================================
//	�f�[�^
//==============================================================================
//typedef void* ( *pDemoInit_func )( DEMO_TENGAN_WORK* wk );
	
///���D�~�j�Q�[���̎�M�f�[�^���ߗp�̊֐��e�[�u��
static void (* const BalloonRecvTbl[])(BALLOON_GAME_PTR, const BALLOON_SIO_PLAY_WORK *) = {
	NULL,					//ORDER_CODE_NULL
	
	RecvBalloon_Air,		//ORDER_CODE_AIR
	RecvBalloon_Exploded,	//ORDER_CODE_EXPLODED
	RecvBalloon_Appear,		//ORDER_CODE_APPEAR

	RecvBalloon_Start,		//ORDER_CODE_START
	RecvBalloon_Finish,		//ORDER_CODE_FINISH
	RecvBalloon_Timing,		//ORDER_CODE_TIMING

	RecvBalloon_TouchPenDemo,	//ORDER_CODE_TOUCHPEN_DEMO
};

#ifdef _NITRO
//��M�֐��e�[�u�������߃R�[�h�Ɠ����������p�ӂ���Ă��邩
SDK_COMPILER_ASSERT(NELEMS(BalloonRecvTbl) == ORDER_CODE_MAX);
#endif


//==============================================================================
//
//	
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ��M�f�[�^�󂯎��֐��Ăяo��
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 *
 * @retval	TRUE:��M���ߎ��s�����B�@FALSE:�����������Ȃ�����
 */
//--------------------------------------------------------------
BOOL RecvBalloon_FuncCall(BALLOON_GAME_PTR game, BALLOON_SIO_PLAY_WORK *recv)
{
	if(BalloonRecvTbl[recv->order_code] != NULL){
		BalloonRecvTbl[recv->order_code](game, recv);
		MI_CpuClear8(recv, sizeof(BALLOON_SIO_PLAY_WORK));
		return TRUE;
	}
	return FALSE;
}


//==============================================================================
//	AIR
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���D�j�􃁃b�Z�[�W���M
 *
 * @param   game		
 * @param   balloon_no		���D�ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_Air(BALLOON_GAME_PTR game, const BALLOON_AIR_DATA *air_data)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_AIR;
	data.air_data = *air_data;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   ��C�f�[�^��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_Air(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	if(recv->air_data.net_id == CommGetCurrentID()){
		return;	//�����̃f�[�^�͎󂯎��Ȃ�
	}
	BalloonTool_PlayerAirParamAdd(game, &recv->air_data);
}

//==============================================================================
//	EXPLODED
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���D�j�􃁃b�Z�[�W���M
 *
 * @param   game		
 * @param   balloon_no		���D�ԍ�
 * @param   timing_no		�����ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_Exploded(BALLOON_GAME_PTR game, int balloon_no, u8 timing_no)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_EXPLODED;
	data.exploded.no = balloon_no;
	data.exploded.timing_req = TIMING_REQ_EXPLODED_AFTER;
	data.exploded.timing_no = timing_no;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   ���D�j�􃁃b�Z�[�W��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_Exploded(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	game->exploded_count++;
	
	//�j��A�N�^�[�\��
	BalloonTool_ExplodedParamAdd(game);
	
	Timing_AnswerReqParamSet(game, recv->exploded.timing_req, recv->exploded.timing_no);
}

//==============================================================================
//	APPEAR
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���D�o�ꃁ�b�Z�[�W���M
 *
 * @param   game			
 * @param   balloon_no		���D�ԍ�
 * @param   level			���D���x��(BALLOON_LEVEL_???)
 * @param   timing_no		�����ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_Appear(BALLOON_GAME_PTR game, int balloon_no, int level, u8 timing_no)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_APPEAR;
	data.appear.no = balloon_no;
	data.appear.level = level;
	data.appear.timing_req = TIMING_REQ_APPEAR_AFTER;
	data.appear.timing_no = timing_no;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   ���D�o�ꃁ�b�Z�[�W��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_Appear(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	game->balloon_no = recv->appear.no;
	BalloonTool_BalloonBGSet(game->bgl, game->bsw->player_max, recv->appear.level, &game->bst);
	game->balloon_occ = TRUE;

	Timing_AnswerReqParamSet(game, recv->appear.timing_req, recv->appear.timing_no);
}

//==============================================================================
//	START
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �Q�[���J�n���b�Z�[�W���M
 *
 * @param   game			
 * @param   timing_req		�������̎��
 * @param   timing_no		�����ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_Start(BALLOON_GAME_PTR game, u8 timing_no)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_START;
	data.timing.timing_req = TIMING_REQ_START_AFTER;
	data.timing.timing_no = timing_no;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   �~�j�Q�[���J�n���b�Z�[�W��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_Start(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	game->countdown_eff = BALLOON_COUNTDOWN_START;
	Timing_AnswerReqParamSet(game, recv->start.timing_req, recv->start.timing_no);
}

//==============================================================================
//	FINISH
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ������胁�b�Z�[�W���M
 *
 * @param   game			
 * @param   timing_req		�������̎��
 * @param   timing_no		�����ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_Finish(BALLOON_GAME_PTR game, u8 timing_no)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_FINISH;
	data.finish.timing_req = TIMING_REQ_FINISH_AFTER;
	data.finish.timing_no = timing_no;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   �~�j�Q�[���I�����b�Z�[�W��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_Finish(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	game->game_finish = TRUE;
	game->countdown_eff = BALLOON_COUNTDOWN_TIMEUP;
	Timing_AnswerReqParamSet(game, recv->finish.timing_req, recv->finish.timing_no);
}

//==============================================================================
//	TIMING
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ������胁�b�Z�[�W���M
 *
 * @param   game			
 * @param   timing_req		�������̎��
 * @param   timing_no		�����ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_Timing(BALLOON_GAME_PTR game, int timing_req, u8 timing_no)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_TIMING;
	data.timing.timing_req = timing_req;
	data.timing.timing_no = timing_no;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   ������胁�b�Z�[�W��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_Timing(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	Timing_AnswerReqParamSet(game, recv->timing.timing_req, recv->timing.timing_no);
}

//==============================================================================
//	�^�b�`�y���f��
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �Q�[���J�n���b�Z�[�W���M
 *
 * @param   game			
 * @param   timing_req		�������̎��
 * @param   timing_no		�����ԍ�
 */
//--------------------------------------------------------------
void SendBalloon_TouchPenDemo(BALLOON_GAME_PTR game, u8 timing_no)
{
	BALLOON_SIO_PLAY_WORK data;
	
	MI_CpuClear8(&data, sizeof(BALLOON_SIO_PLAY_WORK));
	
	data.order_code = ORDER_CODE_TOUCHPEN_DEMO;
	data.timing.timing_req = TIMING_REQ_TOUCHPEN_DEMO_AFTER;
	data.timing.timing_no = timing_no;
	
	BalloonSio_SendBufferSet(game, &data);
}

//--------------------------------------------------------------
/**
 * @brief   �~�j�Q�[���J�n���b�Z�[�W��M����
 *
 * @param   game		
 * @param   recv		��M�f�[�^
 */
//--------------------------------------------------------------
static void RecvBalloon_TouchPenDemo(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *recv)
{
	game->countdown_eff = BALLOON_COUNTDOWN_TOUCHPEN_DEMO_INIT;
	Timing_AnswerReqParamSet(game, recv->start.timing_req, recv->start.timing_no);
}
