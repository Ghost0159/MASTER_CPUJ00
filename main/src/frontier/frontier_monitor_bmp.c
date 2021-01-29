//==============================================================================================
/**
 * @file	frontier_bmp.c
 * @brief	�u�o�g���t�����e�B�A���щ�ʁvBMP
 * @author	Satoshi Nohara
 * @date	2007.03.15
 */
//==============================================================================================
#include "common.h"
#include "system/fontproc.h"
#include "system/pm_str.h"
#include "system/buflen.h"
#include "system/wordset.h"
#include "savedata/config.h"
#include "frontier_monitor_bmp.h"


//==============================================================================================
//
//	�t�@�N�g���[�f�[�^
//
//==============================================================================================
//�u�o�g���g���[�h�́@�V���O���@���������v
#define BF_WIN_TITLE_PX				(2)
#define BF_WIN_TITLE_PY				(2)
#define BF_WIN_TITLE_SX				(30)
#define BF_WIN_TITLE_SY				(2)
#define BF_WIN_TITLE_CGX			(1)

//�u���x���T�O�E�E�E�����^���^��������v
#define BF_WIN_ENTRY1_PX			(2)
#define BF_WIN_ENTRY1_PY			(7)
#define BF_WIN_ENTRY1_SX			(28)
#define BF_WIN_ENTRY1_SY			(2)
#define BF_WIN_ENTRY1_CGX			(BF_WIN_TITLE_CGX + BF_WIN_TITLE_SX * BF_WIN_TITLE_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
#define BF_WIN_BEFORE_PX			(2)
#define BF_WIN_BEFORE_PY			(10)
#define BF_WIN_BEFORE_SX			(28)
#define BF_WIN_BEFORE_SY			(2)
#define BF_WIN_BEFORE_CGX			(BF_WIN_ENTRY1_CGX + BF_WIN_ENTRY1_SX * BF_WIN_ENTRY1_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v
#define BF_WIN_MAX_PX				(2)
#define BF_WIN_MAX_PY				(12)
#define BF_WIN_MAX_SX				(28)
#define BF_WIN_MAX_SY				(2)
#define BF_WIN_MAX_CGX				(BF_WIN_BEFORE_CGX + BF_WIN_BEFORE_SX * BF_WIN_BEFORE_SY)

//�u�I�[�v�����x���v
#define BF_WIN_ENTRY2_PX			(2)
#define BF_WIN_ENTRY2_PY			(15)
#define BF_WIN_ENTRY2_SX			(16)
#define BF_WIN_ENTRY2_SY			(2)
#define BF_WIN_ENTRY2_CGX			(BF_WIN_MAX_CGX + BF_WIN_MAX_SX * BF_WIN_MAX_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
#define BF_WIN_BEFORE2_PX			(2)
#define BF_WIN_BEFORE2_PY			(18)
#define BF_WIN_BEFORE2_SX			(28)
#define BF_WIN_BEFORE2_SY			(2)
#define BF_WIN_BEFORE2_CGX			(BF_WIN_ENTRY2_CGX + BF_WIN_ENTRY2_SX * BF_WIN_ENTRY2_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v
#define BF_WIN_MAX2_PX				(2)
#define BF_WIN_MAX2_PY				(20)
#define BF_WIN_MAX2_SX				(28)
#define BF_WIN_MAX2_SY				(2)
#define BF_WIN_MAX2_CGX				(BF_WIN_BEFORE2_CGX + BF_WIN_BEFORE2_SX * BF_WIN_BEFORE2_SY)

static const BMPWIN_DAT FactoryMonitorBmpData[] =
{
	{	// 0:�u�o�g���g���[�h�́@�V���O���@���������v
		BFM_FRAME_WIN, BF_WIN_TITLE_PX, BF_WIN_TITLE_PY,
		BF_WIN_TITLE_SX, BF_WIN_TITLE_SY, BFM_FONT_PAL, BF_WIN_TITLE_CGX
	},	
	{	// 1:�u���x���T�O�E�E�E�����^���^��������v
		BFM_FRAME_WIN, BF_WIN_ENTRY1_PX, BF_WIN_ENTRY1_PY,
		BF_WIN_ENTRY1_SX, BF_WIN_ENTRY1_SY, BFM_FONT_PAL, BF_WIN_ENTRY1_CGX
	},	
	{	// 2:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BF_WIN_BEFORE_PX, BF_WIN_BEFORE_PY,
		BF_WIN_BEFORE_SX, BF_WIN_BEFORE_SY, BFM_FONT_PAL, BF_WIN_BEFORE_CGX
	},	
	{	// 3:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BF_WIN_MAX_PX, BF_WIN_MAX_PY,
		BF_WIN_MAX_SX, BF_WIN_MAX_SY, BFM_FONT_PAL, BF_WIN_MAX_CGX
	},	
	{	// 4:�u�I�[�v�����x���v
		BFM_FRAME_WIN, BF_WIN_ENTRY2_PX, BF_WIN_ENTRY2_PY,
		BF_WIN_ENTRY2_SX, BF_WIN_ENTRY2_SY, BFM_FONT_PAL, BF_WIN_ENTRY2_CGX
	},	
	{	// 5:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BF_WIN_BEFORE2_PX, BF_WIN_BEFORE2_PY,
		BF_WIN_BEFORE2_SX, BF_WIN_BEFORE2_SY, BFM_FONT_PAL, BF_WIN_BEFORE2_CGX
	},
	{	// 6:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BF_WIN_MAX2_PX, BF_WIN_MAX2_PY,
		BF_WIN_MAX2_SX, BF_WIN_MAX2_SY, BFM_FONT_PAL, BF_WIN_MAX2_CGX
	},
};


//==============================================================================================
//
//	�X�e�[�W�f�[�^
//
//==============================================================================================
//�u�o�g���g���[�h�́@�V���O���@���������v
#define BS_WIN_TITLE_PX				(2)
#define BS_WIN_TITLE_PY				(2)
#define BS_WIN_TITLE_SX				(28)
#define BS_WIN_TITLE_SY				(2)
#define BS_WIN_TITLE_CGX			(1)

//�u�s�J�`���E�́@���낭�v
#define BS_WIN_ENTRY1_PX			(2)
#define BS_WIN_ENTRY1_PY			(7)
#define BS_WIN_ENTRY1_SX			(28)
#define BS_WIN_ENTRY1_SY			(2)
#define BS_WIN_ENTRY1_CGX			(BS_WIN_TITLE_CGX + BS_WIN_TITLE_SX * BS_WIN_TITLE_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
#define BS_WIN_BEFORE_PX			(2)
#define BS_WIN_BEFORE_PY			(10)
#define BS_WIN_BEFORE_SX			(28)
#define BS_WIN_BEFORE_SY			(2)
#define BS_WIN_BEFORE_CGX			(BS_WIN_ENTRY1_CGX + BS_WIN_ENTRY1_SX * BS_WIN_ENTRY1_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v
#define BS_WIN_MAX_PX				(2)
#define BS_WIN_MAX_PY				(13)
#define BS_WIN_MAX_SX				(28)
#define BS_WIN_MAX_SY				(2)
#define BS_WIN_MAX_CGX				(BS_WIN_BEFORE_CGX + BS_WIN_BEFORE_SX * BS_WIN_BEFORE_SY)

static const BMPWIN_DAT StageMonitorBmpData[] =
{
	{	// 0:�u�o�g���g���[�h�́@�V���O���@���������v
		BFM_FRAME_WIN, BS_WIN_TITLE_PX, BS_WIN_TITLE_PY,
		BS_WIN_TITLE_SX, BS_WIN_TITLE_SY, BFM_FONT_PAL, BS_WIN_TITLE_CGX
	},	
	{	// 1:�u���x���T�O�E�E�E�����^���^��������v
		BFM_FRAME_WIN, BS_WIN_ENTRY1_PX, BS_WIN_ENTRY1_PY,
		BS_WIN_ENTRY1_SX, BS_WIN_ENTRY1_SY, BFM_FONT_PAL, BS_WIN_ENTRY1_CGX
	},	
	{	// 2:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BS_WIN_BEFORE_PX, BS_WIN_BEFORE_PY,
		BS_WIN_BEFORE_SX, BS_WIN_BEFORE_SY, BFM_FONT_PAL, BS_WIN_BEFORE_CGX
	},	
	{	// 3:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BS_WIN_MAX_PX, BS_WIN_MAX_PY,
		BS_WIN_MAX_SX, BS_WIN_MAX_SY, BFM_FONT_PAL, BS_WIN_MAX_CGX
	},	
};


//==============================================================================================
//
//	�L���b�X���f�[�^
//
//==============================================================================================
//�u�o�g���g���[�h�́@�V���O���@���������v
#define BC_WIN_TITLE_PX				(2)
#define BC_WIN_TITLE_PY				(2)
#define BC_WIN_TITLE_SX				(28)
#define BC_WIN_TITLE_SY				(2)
#define BC_WIN_TITLE_CGX			(1)

//�u�L���b�X���|�C���g�v
#define BC_WIN_ENTRY1_PX			(18)
#define BC_WIN_ENTRY1_PY			(7)
#define BC_WIN_ENTRY1_SX			(12)
#define BC_WIN_ENTRY1_SY			(2)
#define BC_WIN_ENTRY1_CGX			(BC_WIN_TITLE_CGX + BC_WIN_TITLE_SX * BC_WIN_TITLE_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
#define BC_WIN_BEFORE_PX			(2)
#define BC_WIN_BEFORE_PY			(10)
#define BC_WIN_BEFORE_SX			(28)
#define BC_WIN_BEFORE_SY			(2)
#define BC_WIN_BEFORE_CGX			(BC_WIN_ENTRY1_CGX + BC_WIN_ENTRY1_SX * BC_WIN_ENTRY1_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v
#define BC_WIN_MAX_PX				(2)
#define BC_WIN_MAX_PY				(13)
#define BC_WIN_MAX_SX				(28)
#define BC_WIN_MAX_SY				(2)
#define BC_WIN_MAX_CGX				(BC_WIN_BEFORE_CGX + BC_WIN_BEFORE_SX * BC_WIN_BEFORE_SY)

static const BMPWIN_DAT CastleMonitorBmpData[] =
{
	{	// 0:�u�o�g���g���[�h�́@�V���O���@���������v
		BFM_FRAME_WIN, BC_WIN_TITLE_PX, BC_WIN_TITLE_PY,
		BC_WIN_TITLE_SX, BC_WIN_TITLE_SY, BFM_FONT_PAL, BC_WIN_TITLE_CGX
	},	
	{	// 1:�u���x���T�O�E�E�E�����^���^��������v
		BFM_FRAME_WIN, BC_WIN_ENTRY1_PX, BC_WIN_ENTRY1_PY,
		BC_WIN_ENTRY1_SX, BC_WIN_ENTRY1_SY, BFM_FONT_PAL, BC_WIN_ENTRY1_CGX
	},	
	{	// 2:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BC_WIN_BEFORE_PX, BC_WIN_BEFORE_PY,
		BC_WIN_BEFORE_SX, BC_WIN_BEFORE_SY, BFM_FONT_PAL, BC_WIN_BEFORE_CGX
	},	
	{	// 3:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BC_WIN_MAX_PX, BC_WIN_MAX_PY,
		BC_WIN_MAX_SX, BC_WIN_MAX_SY, BFM_FONT_PAL, BC_WIN_MAX_CGX
	},	
};


//==============================================================================================
//
//	���[���b�g�f�[�^
//
//==============================================================================================
//�u�o�g���g���[�h�́@�V���O���@���������v
#define BR_WIN_TITLE_PX				(2)
#define BR_WIN_TITLE_PY				(2)
#define BR_WIN_TITLE_SX				(28)
#define BR_WIN_TITLE_SY				(2)
#define BR_WIN_TITLE_CGX			(1)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
#define BR_WIN_BEFORE_PX			(2)
#define BR_WIN_BEFORE_PY			(8)
#define BR_WIN_BEFORE_SX			(28)
#define BR_WIN_BEFORE_SY			(2)
#define BR_WIN_BEFORE_CGX			(BR_WIN_TITLE_CGX + BR_WIN_TITLE_SX * BR_WIN_TITLE_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v
#define BR_WIN_MAX_PX				(2)
#define BR_WIN_MAX_PY				(11)
#define BR_WIN_MAX_SX				(28)
#define BR_WIN_MAX_SY				(2)
#define BR_WIN_MAX_CGX				(BR_WIN_BEFORE_CGX + BR_WIN_BEFORE_SX * BR_WIN_BEFORE_SY)

static const BMPWIN_DAT RouletteMonitorBmpData[] =
{
	{	// 0:�u�o�g���g���[�h�́@�V���O���@���������v
		BFM_FRAME_WIN, BR_WIN_TITLE_PX, BR_WIN_TITLE_PY,
		BR_WIN_TITLE_SX, BR_WIN_TITLE_SY, BFM_FONT_PAL, BR_WIN_TITLE_CGX
	},	
	{	// 1:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BR_WIN_BEFORE_PX, BR_WIN_BEFORE_PY,
		BR_WIN_BEFORE_SX, BR_WIN_BEFORE_SY, BFM_FONT_PAL, BR_WIN_BEFORE_CGX
	},	
	{	// 2:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BR_WIN_MAX_PX, BR_WIN_MAX_PY,
		BR_WIN_MAX_SX, BR_WIN_MAX_SY, BFM_FONT_PAL, BR_WIN_MAX_CGX
	},	
};


//==============================================================================================
//
//	�^���[�f�[�^
//
//==============================================================================================
//�u�o�g���^���[�́@�V���O���@���������v
#define BT_WIN_TITLE_PX				(2)
#define BT_WIN_TITLE_PY				(2)
#define BT_WIN_TITLE_SX				(30)
#define BT_WIN_TITLE_SY				(2)
#define BT_WIN_TITLE_CGX			(1)

//�u�g���[�i�[�ƃ}���`�v
#define BT_WIN_ENTRY1_PX			(2)
#define BT_WIN_ENTRY1_PY			(7)
#define BT_WIN_ENTRY1_SX			(28)
#define BT_WIN_ENTRY1_SY			(2)
#define BT_WIN_ENTRY1_CGX			(BT_WIN_TITLE_CGX + BT_WIN_TITLE_SX * BT_WIN_TITLE_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v"�V���O���A�_�u��"
#define BT_WIN_BEFORE0_PX			(2)
#define BT_WIN_BEFORE0_PY			(7)
#define BT_WIN_BEFORE0_SX			(28)
#define BT_WIN_BEFORE0_SY			(2)
#define BT_WIN_BEFORE0_CGX			(BT_WIN_ENTRY1_CGX + BT_WIN_ENTRY1_SX * BT_WIN_ENTRY1_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v"�V���O���A�_�u��"
#define BT_WIN_MAX0_PX				(2)
#define BT_WIN_MAX0_PY				(10)
#define BT_WIN_MAX0_SX				(28)
#define BT_WIN_MAX0_SY				(2)
#define BT_WIN_MAX0_CGX				(BT_WIN_BEFORE0_CGX + BT_WIN_BEFORE0_SX * BT_WIN_BEFORE0_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v"�}���`"
#define BT_WIN_BEFORE_PX			(2)
#define BT_WIN_BEFORE_PY			(10)
#define BT_WIN_BEFORE_SX			(28)
#define BT_WIN_BEFORE_SY			(2)
#define BT_WIN_BEFORE_CGX			(BT_WIN_ENTRY1_CGX + BT_WIN_ENTRY1_SX * BT_WIN_ENTRY1_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v"�}���`"
#define BT_WIN_MAX_PX				(2)
#define BT_WIN_MAX_PY				(12)
#define BT_WIN_MAX_SX				(28)
#define BT_WIN_MAX_SY				(2)
#define BT_WIN_MAX_CGX				(BT_WIN_BEFORE_CGX + BT_WIN_BEFORE_SX * BT_WIN_BEFORE_SY)

//�u�Ƃ������ƃ}���`�v
#define BT_WIN_ENTRY2_PX			(2)
#define BT_WIN_ENTRY2_PY			(15)
#define BT_WIN_ENTRY2_SX			(16)
#define BT_WIN_ENTRY2_SY			(2)
#define BT_WIN_ENTRY2_CGX			(BT_WIN_MAX_CGX + BT_WIN_MAX_SX * BT_WIN_MAX_SY)

//�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
#define BT_WIN_BEFORE2_PX			(2)
#define BT_WIN_BEFORE2_PY			(18)
#define BT_WIN_BEFORE2_SX			(28)
#define BT_WIN_BEFORE2_SY			(2)
#define BT_WIN_BEFORE2_CGX			(BT_WIN_ENTRY2_CGX + BT_WIN_ENTRY2_SX * BT_WIN_ENTRY2_SY)

//�u���������@9999��񂵂傤�E�E�E9999�����v
#define BT_WIN_MAX2_PX				(2)
#define BT_WIN_MAX2_PY				(20)
#define BT_WIN_MAX2_SX				(28)
#define BT_WIN_MAX2_SY				(2)
#define BT_WIN_MAX2_CGX				(BT_WIN_BEFORE2_CGX + BT_WIN_BEFORE2_SX * BT_WIN_BEFORE2_SY)

static const BMPWIN_DAT TowerMonitorBmpData[] =
{
	{	// 0:�u�o�g���^���[�́@�V���O���@���������v
		BFM_FRAME_WIN, BT_WIN_TITLE_PX, BT_WIN_TITLE_PY,
		BT_WIN_TITLE_SX, BT_WIN_TITLE_SY, BFM_FONT_PAL, BT_WIN_TITLE_CGX
	},	
	{	// 1:�u�g���[�i�[�ƃ}���`�v
		BFM_FRAME_WIN, BT_WIN_ENTRY1_PX, BT_WIN_ENTRY1_PY,
		BT_WIN_ENTRY1_SX, BT_WIN_ENTRY1_SY, BFM_FONT_PAL, BT_WIN_ENTRY1_CGX
	},	
	{	// 2:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BT_WIN_BEFORE0_PX, BT_WIN_BEFORE0_PY,
		BT_WIN_BEFORE0_SX, BT_WIN_BEFORE0_SY, BFM_FONT_PAL, BT_WIN_BEFORE0_CGX
	},	
	{	// 3:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BT_WIN_MAX0_PX, BT_WIN_MAX0_PY,
		BT_WIN_MAX0_SX, BT_WIN_MAX0_SY, BFM_FONT_PAL, BT_WIN_MAX0_CGX
	},	
	{	// 4:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BT_WIN_BEFORE_PX, BT_WIN_BEFORE_PY,
		BT_WIN_BEFORE_SX, BT_WIN_BEFORE_SY, BFM_FONT_PAL, BT_WIN_BEFORE_CGX
	},	
	{	// 5:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BT_WIN_MAX_PX, BT_WIN_MAX_PY,
		BT_WIN_MAX_SX, BT_WIN_MAX_SY, BFM_FONT_PAL, BT_WIN_MAX_CGX
	},	
	{	// 6:�u�Ƃ������ƃ}���`�v
		BFM_FRAME_WIN, BT_WIN_ENTRY2_PX, BT_WIN_ENTRY2_PY,
		BT_WIN_ENTRY2_SX, BT_WIN_ENTRY2_SY, BFM_FONT_PAL, BT_WIN_ENTRY2_CGX
	},	
	{	// 7:�u���񂩂��@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BT_WIN_BEFORE2_PX, BT_WIN_BEFORE2_PY,
		BT_WIN_BEFORE2_SX, BT_WIN_BEFORE2_SY, BFM_FONT_PAL, BT_WIN_BEFORE2_CGX
	},	
	{	// 8:�u���������@9999��񂵂傤�E�E�E9999�����v
		BFM_FRAME_WIN, BT_WIN_MAX2_PX, BT_WIN_MAX2_PY,
		BT_WIN_MAX2_SX, BT_WIN_MAX2_SY, BFM_FONT_PAL, BT_WIN_MAX2_CGX
	},	
};


//==============================================================================================
//
//	�v���g�^�C�v�錾
//
//==============================================================================================
void FrontierMonitorAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, u8 bmp_no );
void FrontierMonitorExitBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );
void FrontierMonitorOffBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no );

//���j���[�E�B���h�E�L����
#define BF_MENU_CGX_NUM			(1024-MENU_WIN_CGX_SIZ)

//��b�E�B���h�E�L����
#define	BF_TALKWIN_CGX_SIZE	(TALK_WIN_CGX_SIZ)
#define	BF_TALKWIN_CGX_NUM	(BF_MENU_CGX_NUM - BF_TALKWIN_CGX_SIZE)

typedef struct{
	const BMPWIN_DAT* dat;				//BMP�f�[�^
	u32	max;							//�o�^�ő吔
}FRONTIER_MONITOR_BMP;

//GetBmpNo�ƕ��т���v�����Ă���̂ł��炳�Ȃ��I
static const FRONTIER_MONITOR_BMP frontier_monitor_bmp[FRONTIER_MONITOR_BMP_MAX] = {
	{ FactoryMonitorBmpData,	FACTORY_MONITOR_BMPWIN_MAX	},
	{ StageMonitorBmpData,		STAGE_MONITOR_BMPWIN_MAX	},
	{ CastleMonitorBmpData,		CASTLE_MONITOR_BMPWIN_MAX	},
	{ RouletteMonitorBmpData,	ROULETTE_MONITOR_BMPWIN_MAX	},
	{ TowerMonitorBmpData,		TOWER_MONITOR_BMPWIN_MAX	},
};


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
void FrontierMonitorAddBmpWin( GF_BGL_INI* bgl, GF_BGL_BMPWIN* win, u8 bmp_no )
{
	u8 i;
	const BMPWIN_DAT* dat = frontier_monitor_bmp[bmp_no].dat;
	u32 max = frontier_monitor_bmp[bmp_no].max;

	//�r�b�g�}�b�v�ǉ�
	for( i=0; i < max; i++ ){
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
void FrontierMonitorExitBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no )
{
	u16	i;
	u32 max = frontier_monitor_bmp[bmp_no].max;

	for( i=0; i < max; i++ ){
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
void FrontierMonitorOffBmpWin( GF_BGL_BMPWIN* win, u8 bmp_no )
{
	u16	i;
	u32 max = frontier_monitor_bmp[bmp_no].max;

	for( i=0; i < max; i++ ){
		GF_BGL_BmpWinOff( &win[i] );
	}


	return;
}


