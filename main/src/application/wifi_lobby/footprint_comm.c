//==============================================================================
/**
 * @file	footprint_comm.c
 * @brief	���Ճ{�[�h�̒ʐM����M�n
 * @author	matsuda
 * @date	2008.01.21(��)
 */
//==============================================================================
#include "common.h"
#include "system/arc_tool.h"
#include "application/wifi_lobby/wflby_system_def.h"
#include "footprint_common.h"
#include "footprint_stamp.h"
#include "include/wifi/dwc_lobbylib.h"
#include "footprint_comm.h"


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static void COMMCMD_PlayerIn(s32 userid, const void *cp_data, u32 size, void *p_work);
static void COMMCMD_PlayerOut(s32 userid, const void *cp_data, u32 size, void *p_work);
static void COMMCMD_Stamp(s32 userid, const void *cp_data, u32 size, void *p_work);

static BOOL RecvDataCheck(s32 userid, FOOTPRINT_MY_COMM_STATUS *my_comm);



//==============================================================================
//	�ʐM�f�[�^�e�[�u��
//==============================================================================

//--------------------------------------------------------------
/**
 *	�ʐM��M�e�[�u��		��COMM_FOOTPRINT�ƕ��т𓯂��ɂ��Ă������ƁI�I
 */
//--------------------------------------------------------------
static const DWC_LOBBY_MSGCOMMAND MsgCommandTbl_Footprint[] = {
	{ COMMCMD_PlayerIn,						sizeof(FOOTPRINT_IN_PARAM) },
	{ COMMCMD_PlayerOut,					0 },
	{ COMMCMD_Stamp,						sizeof(STAMP_PARAM) },
	
	// ��b
//	{ WFLBY_SYSTEM_COMMCMD_TALK_Req,		sizeof(WFLBY_SYSTEM_TALK_DATA) },
//	{ WFLBY_SYSTEM_COMMCMD_TALK_Ans,		sizeof(WFLBY_SYSTEM_TALK_DATA) },
//	{ WFLBY_SYSTEM_COMMCMD_TALK_Data,		sizeof(WFLBY_SYSTEM_TALK_DATA) },
//	{ WFLBY_SYSTEM_COMMCMD_TALK_End,		sizeof(WFLBY_SYSTEM_TALK_DATA) },

	// �g�s�b�N
//	{ WFLBY_SYSTEM_COMMCMD_TOPIC_Data,		sizeof(WFLBY_TOPIC) },
};

///���Ճ{�[�h�̒ʐM�R�}���h�ő吔
#define FOOTPRINT_COMM_MSGTBL_MAX		(NELEMS(MsgCommandTbl_Footprint))

///�R�}���h���s�ԍ�
typedef enum{
	COMM_FOOTPRINT_PLAYER_IN,	///<�������܂���
	COMM_FOOTPRINT_PLAYER_OUT,	///<�ގ����܂���
	COMM_FOOTPRINT_STAMP,		///<�X�^���v���s
}COMM_FOOTPRINT;



//==============================================================================
//
//	�V�X�e��
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �ʐM�V�X�e�������ݒ�
 *
 * @param   fps		
 */
//--------------------------------------------------------------
void Footprint_Comm_Init(FOOTPRINT_SYS_PTR fps)
{
	DWC_LOBBY_SUBCHAN_SetMsgCmd(MsgCommandTbl_Footprint, FOOTPRINT_COMM_MSGTBL_MAX, fps);
}

//--------------------------------------------------------------
/**
 * @brief   ��M�����f�[�^���󂯎����Ԃ��`�F�b�N
 *
 * @param   userid		���[�U�[ID
 * @param   my_comm		�����̒ʐM�X�e�[�^�X
 *
 * @retval  TRUE:��MOK
 * @retval  FALSE:��M����
 */
//--------------------------------------------------------------
static BOOL RecvDataCheck(s32 userid, FOOTPRINT_MY_COMM_STATUS *my_comm)
{
	if(my_comm->user_id == userid){
		OS_TPrintf("�����̃f�[�^��M\n");
		return FALSE;	//�����̃f�[�^�Ȃ̂Ŗ���
	}
	if(my_comm->ready == FALSE){
		OS_TPrintf("ready��ԂɂȂ��Ă��Ȃ��̂Ŏ󂯎�苑��\n");
		return FALSE;
	}
	if(DWC_LOBBY_SUBCHAN_GetUserIDIdx(userid) == DWC_LOBBY_USERIDTBL_IDX_NONE){
		OS_TPrintf("�܂��T�[�o�[�̃V�X�e���ɔ��f����Ă��Ȃ����[�U�[�Ȃ̂ŋ���\n");
		return FALSE;
	}
	
	return TRUE;
}

//==============================================================================
//
//	���M����
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   ���M�F�������܂���
 *
 * @param   in_para		�����p�����[�^
 *
 * @retval  TRUE:���M�����A�@FALSE:���s
 */
//--------------------------------------------------------------
BOOL Footprint_Send_PlayerIn(const FOOTPRINT_IN_PARAM *in_para)
{
	DWC_LOBBY_SUBCHAN_SendMsg(COMM_FOOTPRINT_PLAYER_IN, in_para, sizeof(FOOTPRINT_IN_PARAM));
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M�F�ގ����܂���
 *
 * @retval  TRUE:���M�����A�@FALSE:���s
 */
//--------------------------------------------------------------
BOOL Footprint_Send_PlayerOut(void)
{
	DWC_LOBBY_SUBCHAN_SendMsg(COMM_FOOTPRINT_PLAYER_OUT, NULL, 0);
	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   ���M�F�X�^���v���s
 *
 * @param   stamp		�X�^���v�p�����[�^
 *
 * @retval  TRUE:���M�����A�@FALSE:���s
 */
//--------------------------------------------------------------
BOOL Footprint_Send_Stamp(const STAMP_PARAM *stamp)
{
	if(WFLBY_ERR_CheckError() == TRUE){
		return FALSE;
	}
	DWC_LOBBY_SUBCHAN_SendMsg(COMM_FOOTPRINT_STAMP, stamp, sizeof(STAMP_PARAM));
	return TRUE;
}


//==============================================================================
//
//	��M�R�[���o�b�N
//
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �������܂�����M
 *
 * @param   userid		
 * @param   cp_data		
 * @param   size		
 * @param   p_work		
 */
//--------------------------------------------------------------
static void COMMCMD_PlayerIn(s32 userid, const void *cp_data, u32 size, void *p_work)
{
	FOOTPRINT_SYS_PTR fps = p_work;
	FOOTPRINT_MY_COMM_STATUS *my_comm = Footprint_MyCommStatusGet(fps);
	const FOOTPRINT_IN_PARAM *in_para = cp_data;
	
	OS_TPrintf("������M\n");
	if(RecvDataCheck(userid, my_comm) == FALSE){
		return;
	}
	
	Footprint_NameWrite(fps, userid);
}

//--------------------------------------------------------------
/**
 * @brief   �ގ����܂�����M
 *
 * @param   userid		
 * @param   cp_data		
 * @param   size		
 * @param   p_work		
 */
//--------------------------------------------------------------
static void COMMCMD_PlayerOut(s32 userid, const void *cp_data, u32 size, void *p_work)
{
	FOOTPRINT_SYS_PTR fps = p_work;
	FOOTPRINT_MY_COMM_STATUS *my_comm = Footprint_MyCommStatusGet(fps);
	
	OS_TPrintf("�ގ���M\n");
	if(RecvDataCheck(userid, my_comm) == FALSE){
		return;
	}
	
	Footprint_NameErase(fps, DWC_LOBBY_SUBCHAN_GetUserIDIdx(userid));
}

//--------------------------------------------------------------
/**
 * @brief   �������X�^���v��M
 *
 * @param   userid		
 * @param   cp_data		
 * @param   size		
 * @param   p_work		
 */
//--------------------------------------------------------------
static void COMMCMD_Stamp(s32 userid, const void *cp_data, u32 size, void *p_work)
{
	FOOTPRINT_SYS_PTR fps = p_work;
	FOOTPRINT_MY_COMM_STATUS *my_comm = Footprint_MyCommStatusGet(fps);
	const STAMP_PARAM *stamp = cp_data;
	
	OS_TPrintf("�X�^���v��M\n");
	if(RecvDataCheck(userid, my_comm) == FALSE){
		return;
	}
	
	Footprint_StampAdd(fps, stamp, userid);
}

#if 0
// �ʐM�R�[���o�b�N
//----------------------------------------------------------------------------
/**
 *	@brief	��b���N�G�X�g
 */
//-----------------------------------------------------------------------------
static void WFLBY_SYSTEM_COMMCMD_TALK_Req( s32 userid, const void* cp_data, u32 size, void* p_work )
{
	WFLBY_SYSTEM* p_wk = p_work;
	u32 idx;
	const WFLBY_SYSTEM_TALK_DATA* cp_talkdata;
	u32 status;

	// �f�[�^�擾
	cp_talkdata = cp_data;

	// ���̐l��IDX���擾
	idx = DWC_LOBBY_GetUserIDIdx( userid );

	
	status = WFLBY_SYSTEM_GetProfileStatus( &p_wk->myprofile );
	
	// ����b�ł����Ԃ��`�F�b�N
	// ��Ԃ��Z�������A�m��Ȃ��l�Ȃ炲�߂�Ȃ���
	// ���̐l���牽�x���b���������Ă���ꍇ
	if( (status != WFLBY_STATUS_LOGIN) ||
		(idx == DWC_LOBBY_USERIDTBL_IDX_NONE) ||
		(cp_talkdata->seq != WFLBY_TALK_SEQ_B_ANS) ||
		(WFLBY_SYSTEM_CheckTalkCount( p_wk, idx ) == FALSE)){

		// ��b�ł��Ȃ����Ƃ𑗐M
		OS_TPrintf( "talk don't  user=%d\n", userid );
		WFLBY_SYSTEM_TALK_SendTalkAns( p_wk, userid, FALSE );
		return ;
	}

	// ��b�ł���悤�Ȃ̂ŁA��������
	WFLBY_SYSTEM_TALK_SendTalkAns( p_wk, userid, TRUE );

	// ���߂Ă��`�F�b�N
	if( WFLBY_SYSTEM_TALK_CheckMsk( &p_wk->talk, idx ) == FALSE ){
		p_wk->talk.talk_first = TRUE;
	}else{
		p_wk->talk.talk_first = FALSE;
	}

	// ��Ԃ���b�ɂ���
	WFLBY_SYSTEM_TALK_SetMsk( &p_wk->talk, idx );
	p_wk->talk.talk_idx		= idx;
	p_wk->talk.talk_seq		= WFLBY_TALK_SEQ_A_SEL;
	p_wk->talk.talk_type	= WFLBY_TALK_TYPE_B;
	p_wk->talk.talk_b_start	= FALSE;

	// ��b�����҂��J�n
	WFLBY_SYSTEM_TALK_StartRecvWait( &p_wk->talk );

	OS_TPrintf( "talk match  user=%d\n", userid );
}

#endif
