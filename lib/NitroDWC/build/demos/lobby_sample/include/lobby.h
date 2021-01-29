/*---------------------------------------------------------------------------*
 * Project : NitroDWC - dwc_lobby_sample
 * File    : main.h
 *---------------------------------------------------------------------------*/

#ifndef __LOBBY_H__
#define __LOBBY_H__

#include "ppwlobby/ppw_lobby.h"
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
 *	defineíËã`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *	typedefíËã`
 *---------------------------------------------------------------------------*/
typedef BOOL (*lobby_func)();

/*---------------------------------------------------------------------------*
 *	prototypeêÈåæ
 *---------------------------------------------------------------------------*/
BOOL lobby_initialize(const DWCUserData* userdata);

BOOL lobby_send_message();
BOOL lobby_send_user_message();
BOOL lobby_set_channel_data();
BOOL lobby_get_channel_data();
BOOL lobby_set_broadcast_channel_data();
BOOL lobby_join_subchannel();
BOOL lobby_leave_subchannel();
BOOL lobby_update_user_profile();
BOOL lobby_get_user_profile();
BOOL lobby_enum_user();
BOOL lobby_get_time_info();
BOOL lobby_get_schedule();
BOOL lobby_get_vip();
BOOL lobby_submit_questionnaire();

const char* lobby_start_recruit();
BOOL lobby_stop_recruit();

/*---------------------------------------------------------------------------*
 *	inlineä÷êî
 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  // __LOBBY_H__
