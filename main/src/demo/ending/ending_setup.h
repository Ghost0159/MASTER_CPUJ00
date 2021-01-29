//==============================================================================
/**
 * @file	ending_setup.h
 * @brief	�G���f�B���O�̊e�V�[���̏����ݒ�A��������̃w�b�_
 * @author	matsuda
 * @date	2008.04.10(��)
 */
//==============================================================================
#ifndef __ENDING_SETUP_H__
#define __ENDING_SETUP_H__


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void EndingSceneSetup_Load(ENDING_MAIN_WORK *emw, int scene_no);
extern void EndingSceneSetup_Unload(ENDING_MAIN_WORK *emw, int scene_no);
extern BOOL EndingSceneSetup_MainFunc(ENDING_MAIN_WORK *emw, int scene_no);

//--------------------------------------------------------------
//	�e�V�[���̃��C���֐�
//--------------------------------------------------------------
extern BOOL MapScene1_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);
extern BOOL MapScene2_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);
extern BOOL MapScene3_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);
extern BOOL MapCapScene1_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);
extern BOOL MapCapScene2_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);
extern BOOL MapFieldScene1_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);
extern BOOL MapCopyrightScene_Main(ENDING_MAIN_WORK *emw, ENDING_SCENE_WORK *sw);


//==============================================================================
//	�O���f�[�^�錾
//==============================================================================
extern const u16 Scene3MdlSort[];

#endif	//__ENDING_SETUP_H__
