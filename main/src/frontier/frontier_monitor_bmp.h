//==============================================================================================
/**
 * @file	frontier_bmp.h
 * @brief	�u�t�����e�B�A�vBMP
 * @author	Satoshi Nohara
 * @date	2007.12.6
 */
//==============================================================================================
#ifndef _FRONTIER_MONITOR_BMP_H_
#define _FRONTIER_MONITOR_BMP_H_

#include "system/window.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�t�@�N�g���[�F�r�b�g�}�b�vID
enum{
	BF_BMPWIN_TITLE = 0,
	BF_BMPWIN_ENTRY1,
	BF_BMPWIN_BEFORE,
	BF_BMPWIN_MAX,
	BF_BMPWIN_ENTRY2,
	BF_BMPWIN_BEFORE2,
	BF_BMPWIN_MAX2,
	FACTORY_MONITOR_BMPWIN_MAX,				//BMP�E�B���h�E�̐�
};

//�X�e�[�W�F�r�b�g�}�b�vID
enum{
	BS_BMPWIN_TITLE = 0,
	BS_BMPWIN_ENTRY1,
	BS_BMPWIN_BEFORE,
	BS_BMPWIN_MAX,
	STAGE_MONITOR_BMPWIN_MAX,				//BMP�E�B���h�E�̐�
};

//�L���b�X���F�r�b�g�}�b�vID
enum{
	BC_BMPWIN_TITLE = 0,
	BC_BMPWIN_ENTRY1,
	BC_BMPWIN_BEFORE,
	BC_BMPWIN_MAX,
	CASTLE_MONITOR_BMPWIN_MAX,				//BMP�E�B���h�E�̐�
};

//���[���b�g�F�r�b�g�}�b�vID
enum{
	BR_BMPWIN_TITLE = 0,
	BR_BMPWIN_BEFORE,
	BR_BMPWIN_MAX,
	ROULETTE_MONITOR_BMPWIN_MAX,				//BMP�E�B���h�E�̐�
};

//�^���[�F�r�b�g�}�b�vID
enum{
	BT_BMPWIN_TITLE = 0,
	BT_BMPWIN_ENTRY1,
	BT_BMPWIN_BEFORE0,
	BT_BMPWIN_MAX0,
	BT_BMPWIN_BEFORE,
	BT_BMPWIN_MAX,
	BT_BMPWIN_ENTRY2,
	BT_BMPWIN_BEFORE2,
	BT_BMPWIN_MAX2,
	TOWER_MONITOR_BMPWIN_MAX,					//BMP�E�B���h�E�̐�
};

#define FRONTIER_MONITOR_BMP_MAX		(5)		//

//�p���b�g��`
#define BFM_MSGFONT_PAL			(12)				//MSG�t�H���g�g�p�p���b�g�i���o�[
#define BFM_FONT_PAL			(13)				//SYSTEM�t�H���g�g�p�p���b�g�i���o�[

#define BFM_FRAME_WIN	(GF_BGL_FRAME0_M)
#define BFM_FRAME_BG	(GF_BGL_FRAME2_M)			//MULTI�ƃ����^�����莞
#define BFM_FRAME_SUB	(GF_BGL_FRAME2_S)			//�{�[��


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
extern void FrontierMonitorAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, u8 bmp_no );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�j��	
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void FrontierMonitorExitBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�I�t
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
extern void FrontierMonitorOffBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );


#endif //_FRONTIER_MONITOR_BMP_H_


