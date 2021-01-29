#ifndef __COMMON_H__
#define __COMMON_H__

//=========================================================================
/**
 * @file	common.h
 * @brief	���ʒ�`�t�@�C��
 * @author	GAME FREAK inc.
 *
 */
//=========================================================================

#include "gflib/system.h"					//�O���[�o���ϐ���`
#include "gflib/standard.h"
#include "gflib/tcb.h"					//Task Control Block
#include "gflib/mmap_get.h"
// �������d�v�����ȃt�@�C��������
// �������A���t�@�x�b�g���@������
#include "gflib/bg_system.h"				//�a�f�R���g���[���V�X�e��
#include "gflib/blact.h"
#include "gflib/calc2d.h"
#include "gflib/calc3d.h"
#include "gflib/calctool.h"
#include "gflib/camera.h"					//��{�J�����V�X�e��
#include "gflib/clact.h"
#include "gflib/char_manager.h"
#include "gflib/display.h"				//�\����ʐݒ�
#include "gflib/bounding_box.h"
#include "gflib/fntequ_agb_jp.h"
#include "gflib/fntsys.h"
#include "gflib/msg_print.h"
#include "gflib/pltt_manager.h"
#include "gflib/point_sel.h"
#include "gflib/simple_3dgraphics.h"		//��{�R�c�O���t�B�b�N�V�X�e��
#include "gflib/str_tool.h"
#include "gflib/touchpanel_system.h"
#include "gflib/vram_transfer_anm.h"
#include "gflib/vram_transfer_manager.h"
#include "gflib/sleep.h"
#include "gflib/res_manager.h"
#include "gflib/texanm_sys.h"
#include "gflib/gf_gx.h"
#include "gflib/g3d_system.h"

#include "src_os_print.h"			// OS_Printf()�̗L��/������`


#include "gflib\assert.h"
#include "gflib\sdkdef.h"




#define	GLOBAL_MSGLEN	( 2 )		///<������f�[�^�␳��`


//#define	PAD_BUTTON_DECIDE	( PAD_BUTTON_A|PAD_BUTTON_X )	// ����{�^����`
//#define	PAD_BUTTON_CANCEL	( PAD_BUTTON_B|PAD_BUTTON_Y )	// �L�����Z���{�^����`
#define	PAD_BUTTON_DECIDE	( PAD_BUTTON_A )	// ����{�^����`
#define	PAD_BUTTON_CANCEL	( PAD_BUTTON_B )	// �L�����Z���{�^����`


//==============================================================================
//	�T�[�o�[�̐��i�ŁA�J���łւ̐ڑ��؂�ւ�
//==============================================================================
///�F�؃T�[�o�[�̊J���p�T�[�o�[�A���i�ŃT�[�o�[�̐؂�ւ�
// ----------------------------------------------------------------------------
// localize_spec_mark(LANG_ALL) imatake 2008/07/29
// PG5_WIFIRELEASE = yes �Ȃ�AWi-Fi�֌W�������[�X�T�[�o�ɐڑ�

#ifdef PG5_WIFIRELEASE
//���i�ŗp�T�[�o�[
#define GF_DWC_CONNECTINET_AUTH_TYPE	(DWC_CONNECTINET_AUTH_RELEASE)
//Wi-Fi���r�[�T�[�o
#define GF_DWC_LOBBY_CHANNEL_PREFIX		(DWC_LOBBY_CHANNEL_PREFIX_RELEASE)
#else
//�J���p�T�[�o�[
#define GF_DWC_CONNECTINET_AUTH_TYPE	(DWC_CONNECTINET_AUTH_TEST)
//Wi-Fi���r�[�T�[�o
#define GF_DWC_LOBBY_CHANNEL_PREFIX		(PPW_LOBBY_CHANNEL_PREFIX_DEBUG)
#endif

// ----------------------------------------------------------------------------

#endif //__COMMON_H__
