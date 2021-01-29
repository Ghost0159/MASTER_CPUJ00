//==============================================================================
/**
 * @file	frontier_actor.h
 * @brief	�t�����e�B�A2D�}�b�v�Ŏg�p����A�N�^�[�̃w�b�_
 * @author	matsuda
 * @date	2007.05.30(��)
 */
//==============================================================================
#ifndef __FRONTIER_ACTOR_H__
#define __FRONTIER_ACTOR_H__


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void FAct_ResourceLoad(CATS_SYS_PTR csp, CATS_RES_PTR crp, ARCHANDLE* hdl, 
	PALETTE_FADE_PTR pfd, u16 act_id);
extern void FAct_ResourceFree(CATS_RES_PTR crp, u16 act_id);
extern CATS_ACT_PTR FAct_ActorSet(CATS_SYS_PTR csp, CATS_RES_PTR crp, u16 act_id);
extern void FAct_ActorDel(CATS_ACT_PTR cap);


#endif	//__FRONTIER_ACTOR_H__
