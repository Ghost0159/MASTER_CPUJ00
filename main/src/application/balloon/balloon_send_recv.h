//==============================================================================
/**
 * @file	balloon_send_recv.h
 * @brief	���D�~�j�Q�[���F����M�f�[�^�쐬�����߂̃w�b�_
 * @author	matsuda
 * @date	2007.11.27(��)
 */
//==============================================================================
#ifndef __BALLOON_SEND_RECV_H__
#define __BALLOON_SEND_RECV_H__


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void SendBalloon_Air(BALLOON_GAME_PTR game, const BALLOON_AIR_DATA *air_data);
extern void SendBalloon_Exploded(BALLOON_GAME_PTR game, int balloon_no, u8 timing_no);
extern void SendBalloon_Appear(BALLOON_GAME_PTR game, int balloon_no, int level, u8 timing_no);
extern void SendBalloon_Timing(BALLOON_GAME_PTR game, int timing_req, u8 timing_no);
extern void SendBalloon_Start(BALLOON_GAME_PTR game, u8 timing_no);
extern void SendBalloon_Finish(BALLOON_GAME_PTR game, u8 timing_no);
extern void SendBalloon_TouchPenDemo(BALLOON_GAME_PTR game, u8 timing_no);

extern BOOL RecvBalloon_FuncCall(BALLOON_GAME_PTR game, BALLOON_SIO_PLAY_WORK *recv);


#endif	//__BALLOON_SEND_RECV_H__
