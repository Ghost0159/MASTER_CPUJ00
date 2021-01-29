//============================================================================================
/**
 * @file	fssc_wifi_counter_sub.h
 * @bfief	�t�����e�B�A�V�X�e���X�N���v�g�R�}���h�T�u�FWiFi��t
 * @author	Satoshi Nohara
 * @date	07.07.31
 */
//============================================================================================
#ifndef	__FSSC_WIFI_COUNTER_SUB_H__
#define	__FSSC_WIFI_COUNTER_SUB_H__

#include "gflib/tcb.h"
#include "field/field.h"
#include "frontier/frontier_wifi.h"


//============================================================================================
//
//	extern�錾
//
//============================================================================================
extern FRWIFI_SCRWORK* FrWiFiCounterScr_WorkAlloc( SAVEDATA* savedata );
extern void FrWiFiCounterScr_WorkFree( FRWIFI_SCRWORK* wk );
extern BOOL FrWiFiScr_CommSetSendBuf( FRWIFI_SCRWORK* wk, u16 type, u16 param, u16 param2 );
extern void FrWiFiCounter_PokeSelect( FMAIN_PTR fmain, FRWIFI_SCRWORK* wk, u16 seq );


#endif	//__FSSC_WIFI_COUNTER_SUB_H__


