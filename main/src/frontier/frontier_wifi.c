//==============================================================================
/**
 * @file	castle_tool.c
 * @brief	�t�����e�B�AWiFi��t
 * @author	nohara
 * @date	2007.07.27
 */
//==============================================================================
#include "common.h"
#include "../field/fieldsys.h"
#include "../field/situation_local.h"

#include "frontier_scr.h"
#include "fss_scene.h"
#include "frontier_main.h"
#include "frontier_map.h"
#include "frontier/frontier_types.h"
//#include "frontier/frontier_main.h"
#include "frontier/fss_scene.h"
#include "frontier_wifi.h"
#include "frontier_def.h"


//==============================================================================
//	
//==============================================================================
FRONTIER_EX_PARAM* EvCmdFrontierSystemCall( FIELDSYS_WORK* fsys, void* work );
BOOL FSSC_WiFiCounterSendBuf( FSS_TASK * core );
BOOL FSSC_WiFiCounterRecvBuf( FSS_TASK * core );
static BOOL WaitWiFiCounterRecvBuf( FSS_TASK * core );
static BOOL FrWiFiScr_CommSetSendBuf( FRWIFI_SCRWORK* wk, u16 type, u16 param );

//--------------------------------------------------------------------------------------------
/**
 * @brief	�t�����e�B�AWiFi��t �Ăяo��
 */
//--------------------------------------------------------------------------------------------
FRONTIER_EX_PARAM* EvCmdFrontierSystemCall( FIELDSYS_WORK* fsys, void* work )
{
	FRONTIER_EX_PARAM* ex_param = sys_AllocMemoryLo( HEAPID_WORLD, sizeof(FRONTIER_EX_PARAM) );
	MI_CpuClear8( ex_param, sizeof(FRONTIER_EX_PARAM) );
	
	//PROC�R�[��
	
	//ex_param�쐬
	ex_param->syswork	= work;
	ex_param->config	= SaveData_GetConfig( fsys->savedata );
	ex_param->savedata	= fsys->savedata;
	ex_param->fnote_data= fsys->fnote;
	ex_param->bag_cursor= fsys->bag_cursor;
	ex_param->battle_cursor = fsys->battle_cursor;
	ex_param->bg_id		= 0;
	ex_param->ground_id	= 0;
	ex_param->zone_id	= fsys->location->zone_id;
	ex_param->scene_id	= FSS_SCENEID_WIFI_COUNTER;			//���ƂŕύX����
	ex_param->fsys		= fsys;

	GameSystem_StartSubProc( fsys,  &FrontierMainProcData, ex_param );	//PROC
	//EventCmd_CallSubProc( event, &FrontierMainProcData, ex_param );			//EVENT
	return ex_param;
}


#if 0
//==============================================================================================
//
//	���M�A��M�̌Ăяo��
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * ���M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterSendBuf( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	u16 type	= FSSTGetWorkValue( core );
	u16 param	= FSSTGetWorkValue( core );
	u16* ret_wk	= FSSTGetWork( core );

	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	*ret_wk = FrWiFiScr_CommSetSendBuf( wifi_scr_wk, type, param );
	return 1;
}

//--------------------------------------------------------------
/**
 * ��M
 *
 * @param	core		���z�}�V������\���̂ւ̃|�C���^
 *
 * @return	"0"
 */
//--------------------------------------------------------------
BOOL FSSC_WiFiCounterRecvBuf( FSS_TASK * core )
{
	u16 wk_id = FSSTGetU16( core );

	//���z�}�V���̔ėp���W�X�^�Ƀ��[�N��ID���i�[
	core->reg[0] = wk_id;

	FSST_SetWait( core, WaitWiFiCounterRecvBuf );
	return 1;
}

//return 1 = �I��
static BOOL WaitWiFiCounterRecvBuf( FSS_TASK * core )
{
	FRWIFI_SCRWORK* wifi_scr_wk;
	u16 type = FSS_GetEventWorkValue( core, core->reg[0] );		//���ӁI

	wifi_scr_wk =  Frontier_SysWorkGet( core->fss->fmain );

	if( wifi_scr_wk->recieve_count >= FRONTIER_COMM_PLAYER_NUM ){
		wifi_scr_wk->recieve_count = 0;
		return 1;
	}

	return 0;
}


//==============================================================================================
//
//	�ʐM(CommStart)
//
//==============================================================================================

//--------------------------------------------------------------
/**
 * @brief	���M�E�F�C�g�@
 *
 * @param	wk			CASTLE_SCRWORK�^�̃|�C���^
 * @param	type		���M�^�C�v
 *
 * @retval	none
 */
//--------------------------------------------------------------
static BOOL FrWiFiScr_CommSetSendBuf( FRWIFI_SCRWORK* wk, u16 type, u16 param )
{
	int ret;

	switch( type ){

	//
	case CASTLE_COMM_BASIC:
		ret = CommCastleSendBufBFNo( wk, param );
		break;

	case CASTLE_COMM_RETIRE:
		ret = CommCastleSendBufRetireFlag( wk, param );
		break;

	};

	return ret;
}
#endif


