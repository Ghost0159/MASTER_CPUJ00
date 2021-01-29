//==============================================================================
/**
 * @file	we_err.h
 * @brief	�Z�G�t�F�N�g�֘A�̃f�o�b�O�֌W�̃w�b�_
 * @author	matsuda
 * @date	2005.07.29(��)
 */
//==============================================================================
#ifndef __WE_ERR_H__
#define __WE_ERR_H__

#ifdef PM_DEBUG

#include "wazaeffect/we_mana.h"


//==============================================================================
//	�O���[�o���ϐ�
//==============================================================================
extern WE_DEBUG_WORK WeDebugWork;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void DebugParticle_EmitMove(PTC_PTR ptc, EMIT_PTR emit, const VecFx32 *vec);
extern void DebugParticle_EmitMove2(PTC_PTR ptc, EMIT_PTR emit, WE_SYS_PTR we_sys);
extern void DebugParticle_CameraMove(PTC_PTR ptc);
extern void DebugParticle_CameraMove2(PTC_PTR ptc, EMIT_PTR emit, WE_SYS_PTR we_sys);
extern void DebugParticle_Axis(PTC_PTR ptc, EMIT_PTR emit);
extern void DebugParticle_AxisLoop(PTC_PTR ptc, int index, int cb_no, WE_SYS_PTR we_sys);

extern void DebugWazaEffect_LoopEmitter(PTC_PTR ptc, EMIT_PTR emit, WE_SYS_PTR we_sys);

#endif	//PM_DEBUG
#endif	//__WE_ERR_H__

