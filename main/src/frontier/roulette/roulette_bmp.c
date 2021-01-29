//==============================================================================================
/**
 * @file	roulette_bmp.c
 * @brief	�u�o�g�����[���b�g�vBMP
 * @author	Satoshi Nohara
 * @date	07.09.05
 */
//==============================================================================================
#include "common.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "savedata/config.h"

#include "roulette_sys.h"
#include "roulette_bmp.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//���j���[�E�B���h�E�L����
#define BR_MENU_CGX_NUM		(1024-MENU_WIN_CGX_SIZ)

//��b�E�B���h�E�L����
#define	BR_TALKWIN_CGX_SIZE	(TALK_WIN_CGX_SIZ)
#define	BR_TALKWIN_CGX_NUM	(BR_MENU_CGX_NUM - BR_TALKWIN_CGX_SIZE)


//==============================================================================================
//
//	�����N�A�b�v��ʃf�[�^
//
//==============================================================================================
//�u��b�v
#define RANK_WIN_TALK_PX			(2)
#define RANK_WIN_TALK_PY			(19)
#define RANK_WIN_TALK_SX			(27)
#define RANK_WIN_TALK_SY			(4)
#define RANK_WIN_TALK_CGX			(1)

//�u�͂��E�������v
#define RANK_WIN_YESNO_PX			(24)
#define RANK_WIN_YESNO_PY			(13)
#define RANK_WIN_YESNO_SX			(7)
#define RANK_WIN_YESNO_SY			(4)
#define RANK_WIN_YESNO_CGX			(RANK_WIN_TALK_CGX + RANK_WIN_TALK_SX * RANK_WIN_TALK_SY)

static const BMPWIN_DAT RouletteBmpData[] =
{
	{	// 0:�u��b�v
		BR_FRAME_WIN, RANK_WIN_TALK_PX, RANK_WIN_TALK_PY,
		RANK_WIN_TALK_SX, RANK_WIN_TALK_SY, BR_MSGFONT_PAL, RANK_WIN_TALK_CGX
	},
	{	// 1:�u�͂��E�������v
		BR_FRAME_WIN, RANK_WIN_YESNO_PX, RANK_WIN_YESNO_PY,
		RANK_WIN_YESNO_SX, RANK_WIN_YESNO_SY, BR_FONT_PAL, RANK_WIN_YESNO_CGX
	},
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
void RouletteAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );
void RouletteExitBmpWin( GF_BGL_BMPWIN* win );
void RouletteOffBmpWin( GF_BGL_BMPWIN* win );
void RouletteWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );
void RouletteTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype );

//�͂��A�������E�B���h�E��`
#define YESNO_PX		(23)			//�J�n�w
#define YESNO_PY		(19)			//�J�n�x
#define YESNO_SX		(8)				//���w
#define YESNO_SY		(4)				//���x

//��b�E�B���h�E��`
#define TALK_PX			(2)				//�J�n�w
#define TALK_PY			(19)			//�J�n�x
#define TALK_SX			(17)			//���w
#define TALK_SY			(4)				//���x


//==============================================================================================
//
//	�֐�
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
void RouletteAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win )
{
	u8 i;
	const BMPWIN_DAT* dat = RouletteBmpData;

	//�r�b�g�}�b�v�ǉ�
	for( i=0; i < ROULETTE_BMPWIN_MAX; i++ ){
		GF_BGL_BmpWinAddEx( bgl, &win[i], &dat[i] );
		GF_BGL_BmpWinDataFill( &win[i], FBMP_COL_NULL );		//�h��Ԃ�
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�j��	
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
void RouletteExitBmpWin( GF_BGL_BMPWIN* win )
{
	u16	i;

	for( i=0; i < ROULETTE_BMPWIN_MAX; i++ ){
		GF_BGL_BmpWinDel( &win[i] );
	}

	return;
}

//--------------------------------------------------------------
/**
 * @brief	�r�b�g�}�b�v�E�B���h�E�I�t
 *
 * @param	win		�r�b�g�}�b�v�E�B���h�E
 *
 * @return	none
 */
//--------------------------------------------------------------
void RouletteOffBmpWin( GF_BGL_BMPWIN* win )
{
	u16	i;

	for( i=0; i < ROULETTE_BMPWIN_MAX; i++ ){
		GF_BGL_BmpWinOff( &win[i] );
	}

	return;
}

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
void RouletteWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win )
{
	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
	//MenuWinGraphicSet( bgl, BR_FRAME_WIN, BR_MENU_CGX_NUM, BR_MENU_PAL, 0, HEAPID_ROULETTE );
	MenuWinGraphicSet(	bgl, GF_BGL_BmpWinGet_Frame(win), 
						BR_MENU_CGX_NUM, BR_MENU_PAL, 0, HEAPID_ROULETTE );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite( win, WINDOW_TRANS_OFF, BR_MENU_CGX_NUM, BR_MENU_PAL );
	return;
}

//--------------------------------------------------------------------------------------------
/**
 * @brief	��b�E�B���h�E�\��
 *
 * @param	win		BMP�f�[�^
 *
 * @return	none
 */
//--------------------------------------------------------------------------------------------
void RouletteTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype )
{
	TalkWinGraphicSet( win->ini, GF_BGL_BmpWinGet_Frame(win), BR_TALKWIN_CGX_NUM,
						BR_TALKWIN_PAL, wintype, HEAPID_ROULETTE );

	GF_BGL_BmpWinDataFill( win, 15 );
	BmpTalkWinWrite( win, WINDOW_TRANS_ON, BR_TALKWIN_CGX_NUM, BR_TALKWIN_PAL );
	return;
}


