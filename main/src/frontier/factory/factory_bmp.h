//==============================================================================================
/**
 * @file	factory_bmp.h
 * @brief	�u�o�g���t�@�N�g���[�vBMP
 * @author	Satoshi Nohara
 * @date	2007.03.15
 */
//==============================================================================================
#ifndef _FACTORY_BMP_H_
#define _FACTORY_BMP_H_

#include "factory_sys.h"
#include "system/window.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�r�b�g�}�b�vID
enum{
	BMPWIN_TR1 = 0,
	BMPWIN_TR2,
	BMPWIN_POKE1,
	BMPWIN_POKE2,
	BMPWIN_POKE3,
	BMPWIN_TALK,
	BMPWIN_YESNO,
	BMPWIN_SEL,
	BMPWIN_POKE4,
	BMPWIN_POKE5,
	FACTORY_BMPWIN_MAX,				//BMP�E�B���h�E�̐�
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
extern void FactoryAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�j��	
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void FactoryExitBmpWin( GF_BGL_BMPWIN* win );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�I�t
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void FactoryOffBmpWin( GF_BGL_BMPWIN* win );

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
extern void FactoryWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );

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
extern void FactoryTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype );


#endif //_FACTORY_BMP_H_


