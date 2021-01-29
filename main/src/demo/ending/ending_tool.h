//==============================================================================
/**
 * @file	ending_tool.h
 * @brief	�G���f�B���O�F�c�[����
 * @author	matsuda
 * @date	2008.04.10(��)
 */
//==============================================================================
#ifndef __ENDING_TOOL_H__
#define __ENDING_TOOL_H__


//==============================================================================
//	�萔��`
//==============================================================================
///�L���v�`���[�摜�\���^�C�v
enum{
	CAPTURE_TBL_TYPE_PICTURE,		///<�v���o�摜
	CAPTURE_TBL_TYPE_FIELD,			///<����O
};

///��l���̖ڂ̃L�����N�^�f�[�^�T�C�Y
#define HERO_EYE_CHARBUF_SIZE	(0x20 * 8)	//�ڂ̃L��������2�L����(128k�}�b�s���O�̈�8�L������)


//==============================================================================
//	�O���֐��錾
//==============================================================================
extern void EndingTool_ModelLineInit(ENDING_MAIN_WORK *emw);
extern void EndingTool_ModelLineArrangeDataInit(ENDING_MAIN_WORK *emw, int map_no);
extern BOOL EndintTool_ModelLineScroll(ENDING_MAIN_WORK *emw, s32 mdl_index_no);
extern void EndingTool_CaptureGraphicTrans(ENDING_MAIN_WORK *emw, int cap_no, 
	int frame_no, int tbl_type);
extern void EndingTool_CaptureGraphicPlttTransUpdate(ENDING_MAIN_WORK *emw);
extern void EndingTool_CaptureGraphicPlttTransFree(ENDING_MAIN_WORK *emw);
extern void EndingTool_HeroAnimeUpdate(ENDING_MAIN_WORK *emw, CATS_ACT_PTR cap);

//--------------------------------------------------------------
//	ending.c
//--------------------------------------------------------------
extern void EndingSceneVramExit(ENDING_MAIN_WORK *emw);
extern void FinalSceneVramSetup(ENDING_MAIN_WORK *emw);

#ifdef PM_DEBUG
extern void EndingTool_Debug_ModelLineArrangeSetting(ENDING_MAIN_WORK *emw, int line);
#endif	//PM_DEBUG

#endif	//__ENDING_TOOL_H__
