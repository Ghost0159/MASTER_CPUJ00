//==============================================================================
/**
 * @file	frontier_particle.h
 * @brief	�t�����e�B�A 2D�}�b�v�p�p�[�e�B�N���̃w�b�_
 * @author	matsuda
 * @date	2007.06.07(��)
 */
//==============================================================================
#ifndef __FRONTIER_PARTICLE_H__
#define __FRONTIER_PARTICLE_H__

#include "system/particle.h"


//==============================================================================
//	�^��`
//==============================================================================
///�t�����e�B�A�p�[�e�B�N���V�X�e���\���̂ւ̕s���S�^�|�C���^
typedef	struct _FRP_WORK * FRP_PTR;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern FRP_PTR FRParticle_Init(int heap_id);
extern void FRParticle_Exit(FRP_PTR frp);
extern PTC_PTR FRParticle_SystemCreate(FRP_PTR frp, int work_id, int spa_no, int camera_type);
extern void FRParticle_SystemExit(FRP_PTR frp, int work_id);
extern int FRParticle_Main(void);
extern PTC_PTR FRParticle_PTCPtrGet(FRP_PTR frp, int work_id);
extern BOOL FRParticle_EmitNumGet(FRP_PTR frp);


#endif	//__FRONTIER_PARTICLE_H__

