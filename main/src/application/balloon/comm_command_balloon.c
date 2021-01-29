//=============================================================================
/**
 * @file	comm_command_balloon.c
 * @brief	�f�[�^�𑗂邽�߂̃R�}���h���e�[�u�������Ă��܂�
 *          ���D�~�j�Q�[���p�ł�
 *          comm_command_balloon.h �� enum �Ɠ������тł���K�v������܂�
 * @author	matsuda
 * @date    2007.11.26(��)
 */
//=============================================================================
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
#include "balloon_comm_types.h"
#include "balloon_send_recv.h"



//==============================================================================
//  �e�[�u���ɏ����֐��̒�`
//==============================================================================
static	void Recv_CommTiming(int id_no,int size,void *pData,void *work);
static	void Recv_CommBalloonPlayData(int id_no,int size,void *pData,void *work);
static	void Recv_CommGameEnd(int id_no,int size,void *pData,void *work);
static	void Recv_CommServerVersion(int id_no,int size,void *pData,void *work);

//==============================================================================
//  static��`
//==============================================================================
static int _getGamePlaySize(void);
static int _getServerVersionSize(void);


//==============================================================================
//	�e�[�u���錾
//  comm_shar.h �� enum �Ɠ����Ȃ�тɂ��Ă�������
//  CALLBACK���Ă΂ꂽ���Ȃ��ꍇ��NULL�������Ă�������
//  �R�}���h�̃T�C�Y��Ԃ��֐��������Ă��炦��ƒʐM���y���Ȃ�܂�
//  _getZero�̓T�C�Y�Ȃ���Ԃ��܂��B_getVariable�͉σf�[�^�g�p���Ɏg���܂�
//==============================================================================
static const CommPacketTbl _CommPacketTbl[] = {
    {NULL,                      _getZero, 			NULL},	// CB_EXIT_BALLOON
	{Recv_CommServerVersion,	_getServerVersionSize,	NULL},	// CB_SERVER_VERSION
	{Recv_CommBalloonPlayData, 	_getGamePlaySize,	NULL},	// CB_PLAY_PARAM
	{Recv_CommGameEnd,		 	_getZero,			NULL},	// CB_GAME_END
};

//--------------------------------------------------------------
/**
 * @brief   �R���e�X�g�p�̃R�}���h�̌n�ɏ��������܂�
 * @param   none
 * @retval  none
 */
//--------------------------------------------------------------
void CommCommandBalloonInitialize(void* pWork)
{
    int length = sizeof(_CommPacketTbl)/sizeof(CommPacketTbl);
    CommCommandInitialize(_CommPacketTbl, length, pWork);
    
    GF_ASSERT(sizeof(BALLOON_SIO_PLAY_WORK) < 256);	//���̃T�C�Y�𒴂�����HugeBuff�ɂ���K�v������
}

//--------------------------------------------------------------
/**
 * @brief   �R�Ƃ��T�C�Y��Ԃ��܂�
 * @param   command         �R�}���h
 * @retval  �T�C�Y   �ςȂ� COMM_VARIABLE_SIZE Zero�͂O��Ԃ�
 */
//--------------------------------------------------------------
static int _getGamePlaySize(void)
{
	return sizeof(BALLOON_SIO_PLAY_WORK);
}

//--------------------------------------------------------------
/**
 * @brief   �T�[�o�[�o�[�W�����T�C�Y��Ԃ��܂�
 * @param   command         �R�}���h
 * @retval  �T�C�Y   �ςȂ� COMM_VARIABLE_SIZE Zero�͂O��Ԃ�
 */
//--------------------------------------------------------------
static int _getServerVersionSize(void)
{
	return sizeof(u32);
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
static	void Recv_CommTiming(int id_no,int size,void *pData,void *work)
{
	;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �Q�[���v���C���f�[�^�ʐM��M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		
 */
//--------------------------------------------------------------
static	void Recv_CommBalloonPlayData(int id_no,int size,void *pData,void *work)
{
	BALLOON_GAME_PTR game = work;
	
	BalloonSio_RecvBufferSet(game, id_no, pData);
}

//--------------------------------------------------------------
/**
 * @brief   �Q�[���v���C���f�[�^���M����
 * @param   consys		�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL Send_CommBalloonPlayData(BALLOON_GAME_PTR game, BALLOON_SIO_PLAY_WORK *send_data)
{
	if(CommSendData(CB_PLAY_PARAM, send_data, sizeof(BALLOON_SIO_PLAY_WORK)) == TRUE){
		return TRUE;
	}
	return FALSE;
}


//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �Q�[���I����`����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		
 */
//--------------------------------------------------------------
static	void Recv_CommGameEnd(int id_no,int size,void *pData,void *work)
{
	BALLOON_GAME_PTR game = work;
	
	game->game_end = TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   �Q�[���I�����b�Z�[�W���M����
 * @param   consys		�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL Send_CommGameEnd(BALLOON_GAME_PTR game)
{
	if(CommSendData(CB_GAME_END, NULL, 0) == TRUE){
		return TRUE;
	}
	return FALSE;
}

//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �T�[�o�[�o�[�W��������M����
 *
 * @param   id_no		���M�҂̃l�b�gID
 * @param   size		��M�f�[�^�T�C�Y
 * @param   pData		��M�f�[�^
 * @param   work		
 */
//--------------------------------------------------------------
static	void Recv_CommServerVersion(int id_no,int size,void *pData,void *work)
{
	BALLOON_GAME_PTR game = work;
	u32 *recv_data = pData;
	u32 server_version;
	int i;
	
	server_version = recv_data[0];
	for(i = 0; i < game->bsw->player_max; i++){
		if(game->bsw->player_netid[i] == id_no){
			game->recv_server_version[i] = server_version;
			break;
		}
	}
	GF_ASSERT(i < game->bsw->player_max);
	OS_TPrintf("��M�F�T�[�o�[�o�[�W���� = %d(net_id=%d, ��M�T�C�Y=%d)\n", server_version, id_no, size);
	game->recv_server_version_cnt++;
}

//--------------------------------------------------------------
/**
 * @brief   �T�[�o�[�o�[�W�������M����
 * @param   consys		�R���e�X�g�V�X�e�����[�N�ւ̃|�C���^
 * @retval  TRUE:���M�����B�@FALSE:���M���s
 */
//--------------------------------------------------------------
BOOL Send_CommServerVersion(BALLOON_GAME_PTR game)
{
	u32 server_version = BALLOON_SERVER_VERSION;
	
	if(CommSendData(CB_SERVER_VERSION, &server_version, sizeof(u32)) == TRUE){
		return TRUE;
	}
	return FALSE;
}
