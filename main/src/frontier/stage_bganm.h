//==============================================================================
/**
 * @file	stage_bganm.h
 * @brief	�o�g���X�e�[�W�FBG�A�j���̃w�b�_
 * @author	matsuda
 * @date	2007.08.07(��)
 */
//==============================================================================
#ifndef __STAGE_BGANM_H__
#define __STAGE_BGANM_H__


//==============================================================================
//	�^��`
//==============================================================================
///STAGE_BGANM_WORK�̕s��`�|�C���^
typedef struct _STAGE_ANIMATION_WORK * STAGE_ANIMATION_WORK_PTR;


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern STAGE_ANIMATION_WORK_PTR StageAnimation_Init(GF_BGL_INI *bgl, PALETTE_FADE_PTR pfd);
extern void StageAnimation_End(STAGE_ANIMATION_WORK_PTR anmsys);


#endif	//__STAGE_BGANM_H__

