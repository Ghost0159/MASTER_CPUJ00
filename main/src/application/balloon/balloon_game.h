//==============================================================================
/**
 * @file	balloon_game.h
 * @brief	���D����F�Q�[����ʂ̃w�b�_
 * @author	matsuda
 * @date	2007.11.06(��)
 */
//==============================================================================
#ifndef __BALLOON_GAME_H__
#define __BALLOON_GAME_H__

#include "system/procsys.h"
#include "balloon_comm_types.h"


//==============================================================================
//	�^��`
//==============================================================================
///���D����Q�[�����䃁�C���\���̂̕s��`�|�C���^
typedef struct _BALLOON_GAME_WORK * BALLOON_GAME_PTR;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern PROC_RESULT BalloonGameProc_Init( PROC * proc, int * seq );
extern PROC_RESULT BalloonGameProc_Main( PROC * proc, int * seq );
extern PROC_RESULT BalloonGameProc_End(PROC *proc, int *seq);

extern int Balloon_NetID_to_PlayerPos(BALLOON_GAME_PTR game, int net_id);
extern int Balloon_NetID_to_EntryNo(BALLOON_GAME_PTR game, int net_id);
extern int Balloon_BalloonNoGet(BALLOON_GAME_PTR game);
extern BOOL Balloon_ServerCheck(BALLOON_GAME_PTR game);
extern void BalloonSio_RecvBufferSet(
	BALLOON_GAME_PTR game, int net_id, const BALLOON_SIO_PLAY_WORK *src);
extern BALLOON_SIO_PLAY_WORK * BalloonSio_RecvBufferReadPtrGet(BALLOON_GAME_PTR game, int net_id);
extern void BalloonSio_SendBufferSet(BALLOON_GAME_PTR game, const BALLOON_SIO_PLAY_WORK *src);
extern BALLOON_SIO_PLAY_WORK * BalloonSio_SendBufferReadPtrGet(BALLOON_GAME_PTR game);
extern void Timing_AnswerReqParamSet(BALLOON_GAME_PTR game, int timing_req, u8 timing_no);
extern BOOL Air3D_EntryAdd(BALLOON_GAME_PTR game, int air);
extern void BalloonParticle_EmitAdd(BALLOON_GAME_PTR game, int emit_no);

#endif	//__BALLOON_GAME_H__
