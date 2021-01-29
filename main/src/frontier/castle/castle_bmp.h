//==============================================================================================
/**
 * @file	castle_bmp.h
 * @brief	�u�o�g���L���b�X���vBMP
 * @author	Satoshi Nohara
 * @date	08.07.05
 */
//==============================================================================================
#ifndef _CASTLE_BMP_H_
#define _CASTLE_BMP_H_

#include "castle_sys.h"
#include "system/window.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//BMP�f�[�^�i���o�[
enum{
	CASTLE_BMP_MINE = 0,		//�莝���|�P�������
	CASTLE_BMP_ENEMY,			//�G�g���[�i�[���
	CASTLE_BMP_MAX,
};


//==============================================================================================
//
//	�莝���|�P�����@�f�[�^
//
//==============================================================================================
//�r�b�g�}�b�vID
enum{
	MINE_BMPWIN_TR1 = 0,
	MINE_BMPWIN_MODORU,
	MINE_BMPWIN_LV,
	MINE_BMPWIN_HP,
	MINE_BMPWIN_STATUS,
	MINE_BMPWIN_LIST,
	MINE_BMPWIN_TALK,
	MINE_BMPWIN_TALKMENU,
	MINE_BMPWIN_TALKMENU2,
	MINE_BMPWIN_BASIC_LIST,
	MINE_BMPWIN_KAIHUKU_LIST,
	MINE_BMPWIN_RENTAL_LIST,
	MINE_BMPWIN_ITEMINFO,
	MINE_BMPWIN_YESNO,
	MINE_BMPWIN_SEL,
	MINE_BMPWIN_CP,
	MINE_BMPWIN_ITEM_LV,
	MINE_BMPWIN_ITEM_HP,
	CASTLE_MINE_BMPWIN_MAX,			//BMP�E�B���h�E�̐�
};


//==============================================================================================
//
//	�G�g���[�i�[�@�f�[�^
//
//==============================================================================================
//�r�b�g�}�b�vID
enum{
	ENEMY_BMPWIN_TR1 = 0,
	ENEMY_BMPWIN_MODORU,
	ENEMY_BMPWIN_LV,
	ENEMY_BMPWIN_HP,
	ENEMY_BMPWIN_STATUS,
	ENEMY_BMPWIN_BASIC_LIST,
	ENEMY_BMPWIN_BASIC2_LIST,
	ENEMY_BMPWIN_TALK,
	ENEMY_BMPWIN_TALKMENU,
	ENEMY_BMPWIN_TALKMENU2,
	ENEMY_BMPWIN_YESNO,
	ENEMY_BMPWIN_SEL,
	CASTLE_ENEMY_BMPWIN_MAX,		//BMP�E�B���h�E�̐�
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
extern void CastleAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, u8 bmp_no );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�j��	
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void CastleExitBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�I�t
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void CastleOffBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );

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
extern void CastleWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );

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
extern void CastleTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype );


#endif //_CASTLE_BMP_H_


