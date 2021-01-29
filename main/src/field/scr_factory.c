//============================================================================================
/**
 * @file	scr_factory.c
 * @bfief	�X�N���v�g�R�}���h�F�o�g���t�@�N�g���[(��t�܂��)
 * @author	Satoshi Nohara
 * @date	07.09.18
 */
//============================================================================================
#include "common.h"

#include <nitro/code16.h> 
#include "system/pm_str.h"
#include "system\msgdata.h"			//MSGMAN_GetString
#include "system\wordset.h"			//WordSet_RegistPlayerName
#include "system/bmp_menu.h"
#include "system/bmp_list.h"
#include "system/get_text.h"
#include "system/lib_pack.h"
#include "gflib/strbuf_family.h"	//����
//�t�B�[���h
#include "fieldsys.h"
#include "field_subproc.h"
//�X�N���v�g
#include "script.h"
#include "scrcmd.h"
#include "scrcmd_def.h"
#include "field/eventflag.h"
#include "field/evwkdef.h"
#include "sysflag.h"
#include "syswork.h"
#include "scr_tool.h"
//�o�g���t�@�N�g���[
#include "savedata/frontier_savedata.h"
#include "scr_factory.h"
//#include "scr_factory_sub.h"
#include "savedata/factory_savedata.h"
#include "../frontier/factory_def.h"
#include "application/factory.h"
#include "../frontier/comm_command_frontier.h"
//�ʐM
#include "communication/comm_def.h"
#include "communication/comm_tool.h"
#include "communication/comm_system.h"


//============================================================================================
//
//	�v���g�^�C�v�錾
//
//============================================================================================
BOOL EvCmdFactorySetContinueNG( VM_MACHINE * core );


//============================================================================================
//
//	�R�}���h
//
//============================================================================================

//--------------------------------------------------------------------------------------------
/**
 * @brief		�X�N���v�g�R�}���h�F�t�@�N�g���[�p��NG�Z�b�g
 * @param		core
 */
//--------------------------------------------------------------------------------------------
BOOL EvCmdFactorySetContinueNG( VM_MACHINE * core )
{
	u32 clear_id;
	u8 buf8[4];
	FACTORYSCORE* score_wk;
	u16 type	= VMGetWorkValue(core);
	u16 level	= VMGetWorkValue(core);

	score_wk = SaveData_GetFactoryScore( core->fsys->savedata );

	//"7�A��(�N���A)�������t���O"�̃N���A�������o��
	buf8[0] = 0;
	FACTORYSCORE_PutScoreData( score_wk, FACTORYSCORE_ID_CLEAR_FLAG, 
							(level*FACTORY_TYPE_MAX)+type, buf8 );

	//WIFI�̂ݓ���
	if( type == FACTORY_TYPE_WIFI_MULTI ){

		if( level == FACTORY_LEVEL_50 ){
			clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR_BIT;
		}else{
			clear_id = FRID_FACTORY_MULTI_WIFI_CLEAR100_BIT;
		}

		FrontierRecord_Set(	SaveData_GetFrontier(core->fsys->savedata), 
							clear_id,
							Frontier_GetFriendIndex(clear_id), 0 );
	}

	//"���݂̘A����"��0�ɂ���
	FrontierRecord_Set(	SaveData_GetFrontier(core->fsys->savedata), 
						FactoryScr_GetWinRecordID(level,type),
						Frontier_GetFriendIndex(FactoryScr_GetWinRecordID(level,type)), 0 );

	//"���݂̌�����"��0�ɂ���
	FrontierRecord_Set(	SaveData_GetFrontier(core->fsys->savedata), 
						FactoryScr_GetTradeRecordID(level,type),
						Frontier_GetFriendIndex(FactoryScr_GetTradeRecordID(level,type)), 0 );
	return 0;
}


