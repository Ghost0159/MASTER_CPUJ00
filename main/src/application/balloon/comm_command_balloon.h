//=============================================================================
/**
 * @file	comm_command_balloon.h
 * @brief	�ʐM�̃R�}���h�ꗗ  ���D�~�j�Q�[���p
 * @author	matsuda
 * @date    2007.11.26(��)
 */
//=============================================================================
#ifndef __COMM_COMMAND_BALLOON_H__
#define __COMM_COMMAND_BALLOON_H__

#include "communication/comm_command.h"
#include "balloon_game.h"
#include "balloon_comm_types.h"


/// ���D�~�j�Q�[����p�ʐM�R�}���h�̒�`�B
enum CommCommandBalloon_e {
  CB_EXIT_BALLOON = CS_COMMAND_MAX, ///<���D�~�j�Q�[�����I�����t�B�[���h�ɖ߂鎖�𑗐M

	CB_SERVER_VERSION,			///<�T�[�o�[�o�[�W������`����
	CB_PLAY_PARAM,				///<�Q�[���v���C���f�[�^
	CB_GAME_END,				///<�Q�[���I����`����
	
  //------------------------------------------------�����܂�
  CB_COMMAND_MAX   // �I�[--------------����͈ړ������Ȃ��ł�������
};

extern void CommCommandBalloonInitialize(void* pWork);


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern BOOL Send_CommBalloonPlayData(BALLOON_GAME_PTR game, BALLOON_SIO_PLAY_WORK *send_data);
extern BOOL Send_CommGameEnd(BALLOON_GAME_PTR game);
extern BOOL Send_CommServerVersion(BALLOON_GAME_PTR game);


#endif// __COMM_COMMAND_BALLOON_H__

