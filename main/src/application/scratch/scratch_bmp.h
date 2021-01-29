//==============================================================================================
/**
 * @file	scratch_bmp.h
 * @brief	�u�X�N���b�`�vBMP
 * @author	Satoshi Nohara
 * @date	08.12.11
 */
//==============================================================================================
#ifndef _SCRATCH_BMP_H_
#define _SCRATCH_BMP_H_

#include "scratch_sys.h"
#include "system/window.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�r�b�g�}�b�vID
enum{
	BMPWIN_TALK = 0,
	BMPWIN_YESNO,
	BMPWIN_ITEM1,
	BMPWIN_ITEM2,
	BMPWIN_ITEM3,
	BMPWIN_ITEM4,
	BMPWIN_NUM1,
	BMPWIN_NUM2,
	BMPWIN_NUM3,
	BMPWIN_NUM4,

	BMPWIN_START,
	BMPWIN_ATARI,
	BMPWIN_HAZURE,
	BMPWIN_NEXT,
	BMPWIN_YAMERU,
	BMPWIN_SELECT,

	SCRATCH_BMPWIN_MAX,				//BMP�E�B���h�E�̐�
};


//==============================================================================================
//
//	extern�錾
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�ǉ�
 *
 * @param	ini		BGL�f�[�^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void ScratchAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�j��	
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void ScratchExitBmpWin( GF_BGL_BMPWIN* win );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�I�t
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void ScratchOffBmpWin( GF_BGL_BMPWIN* win );

//--------------------------------------------------------------
/**
 * @brief	���j���[�E�B���h�E�\��
 *
 * @param	ini		BGL�f�[�^
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void ScratchWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );

//--------------------------------------------------------------------------------------------
/**
 * ��b�E�B���h�E�\��
 *
 * @param	win		BMP�f�[�^
 * @param	wintype	�E�B���h�E�^�C�v
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
extern void ScratchTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype );


#endif //_SCRATCH_BMP_H_


