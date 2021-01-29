//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wifi_p2pmatch_tool.h
 *	@brief		WiFi�N���u�ȊO�ŔC�V���F�؃T�[�o�ɃA�N�Z�X����Ƃ��ɁAWiFi�N���u��ŕ\������Ȃ��X�e�[�^�X��Ԃɂ���V�X�e��
 *	@author		tomoya takahashi
 *	@data		2008.03.28
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WIFI_P2PMATCH_TOOL_H__
#define __WIFI_P2PMATCH_TOOL_H__


#include "savedata/mystatus.h"

// WiFi�N���u�ȊO�ŔC�V���F�؃T�[�o�ɃA�N�Z�X����Ƃ��ɁA
// WiFi�N���u��ŕ\������Ȃ��X�e�[�^�X��Ԃɂ���V�X�e��
//
// Set�ŃN���[�����܂��B�N���[���Ɏg�p�������[�N�͔C�V���T�[�o�Ƃ̒ʐM��
// �ؒf����܂ŕێ����Ă��������B
typedef struct _WIFI_MACH_STATUS_tag _WIFI_MACH_STATUS;
extern _WIFI_MACH_STATUS* WifiP2PMatch_CleanStatus_Set( const MYSTATUS* cp_mystatus, u32 heapID );
extern void WifiP2PMatch_CleanStatus_Delete( _WIFI_MACH_STATUS* p_buff );
#endif		// __WIFI_P2PMATCH_TOOL_H__

