//==============================================================================================
/**
 * @file	stage_bmp.c
 * @brief	�u�o�g���X�e�[�W�vBMP
 * @author	Satoshi Nohara
 * @date	07.06.08
 */
//==============================================================================================
#include "common.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "savedata/config.h"

#include "stage_sys.h"
#include "stage_bmp.h"


//==============================================================================================
//
//	��`
//
//==============================================================================================
//�u��b�v
#define WIN_TALK_PX			(2)
#define WIN_TALK_PY			(19)
#define WIN_TALK_SX			(27)
#define WIN_TALK_SY			(4)
#define WIN_TALK_CGX		(1)

//�u�͂��E�������v
#define WIN_YESNO_PX		(24)
#define WIN_YESNO_PY		(13)
#define WIN_YESNO_SX		(7)
#define WIN_YESNO_SY		(4)
#define WIN_YESNO_CGX		(WIN_TALK_CGX + WIN_TALK_SX * WIN_TALK_SY)

//�u�I�������|�P������1�v
#define WIN_POKE1_PX		(10)
#define WIN_POKE1_PY		(20)//(21)
#define WIN_POKE1_SX		(9)
#define WIN_POKE1_SY		(2)
#define WIN_POKE1_CGX		(WIN_YESNO_CGX + WIN_YESNO_SX * WIN_YESNO_SY)

//�u�^�C�v��(�Ƃ肠�������ꂢ�Ɏ��܂�Ȃ��̂ŉ�ʑS�̂��m��)�v(�t���[����ς��Ă���)
#define WIN_TYPE_PX			(1)
#define WIN_TYPE_PY			(0)
#define WIN_TYPE_SX			(30)
#define WIN_TYPE_SY			(22)
#if 0
#define WIN_TYPE_CGX		(WIN_POKE1_CGX + WIN_POKE1_SX * WIN_POKE1_SY)
#else
#define WIN_TYPE_CGX		(1)
#endif

//���j���[�E�B���h�E�L����
#define BS_MENU_CGX_NUM		(1024-MENU_WIN_CGX_SIZ)

//��b�E�B���h�E�L����
#define	BS_TALKWIN_CGX_SIZE	(TALK_WIN_CGX_SIZ)
#define	BS_TALKWIN_CGX_NUM	(BS_MENU_CGX_NUM - BS_TALKWIN_CGX_SIZE)


//==============================================================================================
//
//	�f�[�^
//
//==============================================================================================
static const BMPWIN_DAT StageBmpData[] =
{
	{	// 0:�u��b�v
		BS_FRAME_WIN, WIN_TALK_PX, WIN_TALK_PY,
		WIN_TALK_SX, WIN_TALK_SY, BS_MSGFONT_PAL, WIN_TALK_CGX
	},
	{	// 1:�u�͂��E�������v
		BS_FRAME_WIN, WIN_YESNO_PX, WIN_YESNO_PY,
		WIN_YESNO_SX, WIN_YESNO_SY, BS_FONT_PAL, WIN_YESNO_CGX
	},
	{	// 2:�u�|�P������1�v
		BS_FRAME_WIN, WIN_POKE1_PX, WIN_POKE1_PY,
		WIN_POKE1_SX, WIN_POKE1_SY, BS_FONT_PAL, WIN_POKE1_CGX
	},	
	{	// 3:�u�^�C�v���v
		BS_FRAME_TYPE, WIN_TYPE_PX, WIN_TYPE_PY,					//�t���[�����Ⴄ�̂Œ��ӁI
		WIN_TYPE_SX, WIN_TYPE_SY, BS_FONT_PAL, WIN_TYPE_CGX
	},	
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
void StageAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );
void StageExitBmpWin( GF_BGL_BMPWIN* win );
void StageOffBmpWin( GF_BGL_BMPWIN* win );
void StageWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win );
void StageTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype );

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
void StageAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win )
{
	u8 i;
	const BMPWIN_DAT* dat = StageBmpData;

	//�r�b�g�}�b�v�ǉ�
	for( i=0; i < STAGE_BMPWIN_MAX; i++ ){
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
void StageExitBmpWin( GF_BGL_BMPWIN* win )
{
	u16	i;

	for( i=0; i < STAGE_BMPWIN_MAX; i++ ){
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
void StageOffBmpWin( GF_BGL_BMPWIN* win )
{
	u16	i;

	for( i=0; i < STAGE_BMPWIN_MAX; i++ ){
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
void StageWriteMenuWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win )
{
	//���j���[�E�B���h�E�̃O���t�B�b�N���Z�b�g
	MenuWinGraphicSet( bgl, BS_FRAME_WIN, BS_MENU_CGX_NUM, BS_MENU_PAL, 0, HEAPID_STAGE );

	//���j���[�E�B���h�E��`��
	BmpMenuWinWrite( win, WINDOW_TRANS_OFF, BS_MENU_CGX_NUM, BS_MENU_PAL );
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
void StageTalkWinPut( GF_BGL_BMPWIN * win, WINTYPE wintype )
{
	TalkWinGraphicSet( win->ini, GF_BGL_BmpWinGet_Frame(win), BS_TALKWIN_CGX_NUM,
						BS_TALKWIN_PAL, wintype, HEAPID_STAGE );

	GF_BGL_BmpWinDataFill( win, 15 );
	BmpTalkWinWrite( win, WINDOW_TRANS_ON, BS_TALKWIN_CGX_NUM, BS_TALKWIN_PAL );
	return;
}


