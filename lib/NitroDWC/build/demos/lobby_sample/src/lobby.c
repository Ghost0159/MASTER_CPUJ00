#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "main.h"
#include "../build/libraries/ppwlobby/include/WFLBY_USER_PROFILE.h"

#ifdef _NITRO
#define DWCDemoPrintf OS_Printf
#endif

#define SCHEDULE_NUM 5

static WFLBY_USER_PROFILE userProfile = {
    100000, // ユーザID
    1234,   // トレーナーID
    { 0x01, 0x02, 0xFF },   // ユーザ名
    0,      // 入室時間
    0,      // 自分の国のGMT時間
    { 1, 2, 3, 4, 5, 6},    // 手持ちのポケモン
    {0},    // ポケモンのフォルムデータ
    {0},    // 卵フラグ
    0,      // 性別
    1,      // 言語コード
    0,      // トレーナータイプ
    103,    // 国コード
    1,      // 地域コード
    0,      // 全国図鑑保持フラグ
    0,      // ゲームクリアフラグ
    0,      // タッチトイ
    12,     // ROMバージョン
    0,      // プレイヤーステータス
    0,      // 冒険を始めた日時
    {0},    // 最後に行ったこと
    {0},    // 最後に行ったことに関連する人
	{0}		// 選択したユーザの属性タイプ
};

static void lobby_dummy_func_call();


static void LobbyJoinChannelCallback(BOOL success, PPW_LOBBY_CHANNEL_KIND channelKind)
{
    DWCDemoPrintf("LobbyJoinChannelCallback is called. success: %d, channelKind: %d\n", success, channelKind);
}
static void LobbyPlayerConnectedCallback(s32 userId, const PPW_LobbySystemProfile* systemProfile, const u8* userProfile, u32 userProfileSize)
{
    DWCDemoPrintf("LobbyPlayerConnectedCallback is called. userId: %d, subChannel: %d, enterTime: %lld, userProfileSize: %u\n",
                  userId, systemProfile->subChannelKind, systemProfile->enterTime, userProfileSize);
    (void)userProfile;
}
static void LobbyPlayerDisconnectedCallback(s32 userId)
{
    DWCDemoPrintf("LobbyPlayerDisconnectedCallback is called. userId: %d\n", userId);
}
static void LobbyStringMassageReceivedCallback(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const char* message)
{
    DWCDemoPrintf("LobbyStringMassageReceivedCallback is called. userId: %d, channelKind: %d, type: %u, message: %s\n",
               userId, channelKind, type, message);
}
static void LobbyBinaryMassageReceivedCallback(s32 userId, PPW_LOBBY_CHANNEL_KIND channelKind, s32 type, const u8* data, u32 dataSize)
{
    DWCDemoPrintf("LobbyBinaryMassageReceivedCallback is called. userId: %d, channelKind: %d, type: %u, size: %u\n",
               userId, channelKind, type, dataSize);
    (void)data;
}

static void LobbyGetChannelDataCallback(BOOL success, BOOL broadcast, PPW_LOBBY_CHANNEL_KIND channelKind, s32 userId,
                                        const char* key, const u8* data, u32 dataSize, void* param)
{
    DWCDemoPrintf("LobbyGetChannelDataCallback is called. success: %d, broadcast: %d, channelKind: %d, userId: %d, key: %s, dataSize: %u, param: %u\n",
               success, broadcast, channelKind, userId, key, dataSize, param);
    (void)data;
}

static void LobbyUserProfileUpdatedCallback(s32 userId, const u8* userProfile, u32 userProfileSize)
{
    DWCDemoPrintf("LobbyUserProfileUpdatedCallback is called. userId: %d, userProfileSize: %u\n",
               userId, userProfileSize);
    (void)userProfile;
}

static void LobbySystemProfileUpdatedCallback(s32 userId, const PPW_LobbySystemProfile* systemProfile)
{
    DWCDemoPrintf("LobbySystemProfileUpdatedCallback is called. userId: %d, subChannelKind: %d\n",
               userId, systemProfile->subChannelKind);
}

static void LobbyRecruitCallback(s32 userId, const PPW_LobbyRecruitInfo* recruitInfo)
{
    DWCDemoPrintf("LobbyRecruitCallback is called. userId: %d, gameKind: %d, maxNum: %u, currentNum: %u, paramVal: %u, matchMakingString: %s\n",
                  userId, recruitInfo->gameKind, recruitInfo->maxNum, recruitInfo->currentNum, recruitInfo->paramVal, recruitInfo->matchMakingString);
    receivedRecruit(userId, *recruitInfo);
}

static void LobbyRecruitStoppedCallback(s32 userId, const PPW_LobbyRecruitInfo* recruitInfo)
{
    DWCDemoPrintf("LobbyRecruitStoppedCallback is called. userId: %d, gameKind: %d, maxNum: %u, currentNum: %u, paramVal: %u, matchMakingString: %s\n",
                  userId, recruitInfo->gameKind, recruitInfo->maxNum, recruitInfo->currentNum, recruitInfo->paramVal, recruitInfo->matchMakingString);
    receivedRecruitStopped(userId, *recruitInfo);
}

static void LobbyScheduleProgressCallback(PPW_LOBBY_TIME_EVENT event)
{
    DWCDemoPrintf("LobbyScheduleProgressCallback is called. event: %d.\n", event);
}

static void ShowSchedule(const PPW_LobbySchedule* schedule)
{
    int i;
    DWCDemoPrintf("lockTime: %u.\n", schedule->lockTime);
    DWCDemoPrintf("random: %u.\n", schedule->random);
    DWCDemoPrintf("roomFlag: %x.\n", schedule->roomFlag);
    DWCDemoPrintf("roomType: %u.\n", schedule->roomType);
    DWCDemoPrintf("season: %u.\n", schedule->season);
    DWCDemoPrintf("scheduleRecordNum: %u.\n", schedule->scheduleRecordNum);
    for(i=0; i<schedule->scheduleRecordNum; i++)
    {
        DWCDemoPrintf("Schedule %d:\n", i);
        DWCDemoPrintf("   time: %d.\n", schedule->scheduleRecords[i].time);
        DWCDemoPrintf("   event: %d.\n", schedule->scheduleRecords[i].event);
    }
}

static BOOL LobbyNotifyScheduleCallback(PPW_LOBBY_STATS_RESULT result, const PPW_LobbySchedule* schedule)
{
    DWCDemoPrintf("LobbyNotifyScheduleCallback is called. result: %d.\n", result);
    if(result == PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        ShowSchedule(schedule);
    }
    return TRUE;
}

static void LobbyCheckProfileCallback(PPW_LOBBY_STATS_RESULT result, const u8* userProfile, u32 userProfileSize)
{
    DWCDemoPrintf("LobbyCheckProfileCallback is called. result: %d, size: %u.\n", result, userProfileSize);
    (void)userProfile;
}

static void ShowVip(const PPW_LobbyVipRecord* vipRecords, u32 num)
{
    u32 i;
    for(i=0; i<num; i++)
    {
        DWCDemoPrintf("Vip %d:\n", i);
        DWCDemoPrintf("   profileId: %d.\n", vipRecords[i].profileId);
        DWCDemoPrintf("   key: %d.\n", vipRecords[i].key);
    }
}

static void LobbyGetVipCallback(PPW_LOBBY_STATS_RESULT result, const PPW_LobbyVipRecord* vipRecords, u32 num)
{
    DWCDemoPrintf("LobbyGetVipCallback is called. result: %d.\n", result);
    if(result == PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        ShowVip(vipRecords, num);
    }
}

static void LobbyGetQuestionnaireCallback(PPW_LOBBY_STATS_RESULT result, const PPW_LobbyQuestionnaire* questionnaire)
{
    DWCDemoPrintf("LobbyGetQuestionnaireCallback is called. result: %d.\n", result);
    if(result == PPW_LOBBY_STATS_RESULT_SUCCESS)
    {
        DWCDemoPrintf("  questionSerialNo:%d\n"
                      "  questionNo:%d\n",
            questionnaire->currentQuestionnaireRecord.questionSerialNo, questionnaire->currentQuestionnaireRecord.questionNo);
    }
}

static void LobbySubmitQuestionnaireCallback(PPW_LOBBY_STATS_RESULT result)
{
    DWCDemoPrintf("LobbySubmitQuestionnaireCallback is called. result: %d.\n", result);
}

static void LobbyExcessFloodCallback(u32 floodWeight)
{
    DWCDemoPrintf("LobbyExcessFloodCallback is called. floodWeight: %u\n", floodWeight);
}

BOOL lobby_initialize(const DWCUserData* userdata)
{
    PPW_LOBBY_RESULT result;
    PPW_LobbyCallbacks lobbyCallbacks;
    lobbyCallbacks.lobbyJoinChannelCallback = LobbyJoinChannelCallback;
    lobbyCallbacks.lobbyPlayerConnectedCallback = LobbyPlayerConnectedCallback;
    lobbyCallbacks.lobbyPlayerDisconnectedCallback = LobbyPlayerDisconnectedCallback;
    lobbyCallbacks.lobbyStringMassageReceivedCallback = LobbyStringMassageReceivedCallback;
    lobbyCallbacks.lobbyBinaryMassageReceivedCallback = LobbyBinaryMassageReceivedCallback;
    lobbyCallbacks.lobbyGetChannelDataCallback = LobbyGetChannelDataCallback;
    lobbyCallbacks.lobbySystemProfileUpdatedCallback = LobbySystemProfileUpdatedCallback;
    lobbyCallbacks.lobbyUserProfileUpdatedCallback = LobbyUserProfileUpdatedCallback;
    lobbyCallbacks.lobbyRecruitCallback = LobbyRecruitCallback;
    lobbyCallbacks.lobbyRecruitStoppedCallback = LobbyRecruitStoppedCallback;
    lobbyCallbacks.lobbyCheckProfileCallback = LobbyCheckProfileCallback;
    lobbyCallbacks.lobbyScheduleProgressCallback = LobbyScheduleProgressCallback;
    lobbyCallbacks.lobbyNotifyScheduleCallback = LobbyNotifyScheduleCallback;
    lobbyCallbacks.lobbyGetVipCallback = LobbyGetVipCallback;
    lobbyCallbacks.lobbyGetQuestionnaireCallback = LobbyGetQuestionnaireCallback;
    lobbyCallbacks.lobbySubmitQuestionnaireCallback = LobbySubmitQuestionnaireCallback;
    lobbyCallbacks.lobbyExcessFloodCallback = LobbyExcessFloodCallback;

    result = PPW_LobbyInitializeAsync(GAME_NAME, GAME_SECRET_KEY, DWC_LOBBY_CHANNEL_PREFIX_DEBUG,
                                      &lobbyCallbacks, userdata, (u8*)&userProfile, sizeof(userProfile));
    //result = PPW_LobbyInitializePidAsync(GAME_NAME, GAME_SECRET_KEY, DWC_LOBBY_CHANNEL_PREFIX_DEBUG,
    //                                  &lobbyCallbacks, 100000, (u8*)&userProfile, sizeof(userProfile));
    if(result != PPW_LOBBY_RESULT_SUCCESS)
    {
        DWCDemoPrintf("PPW_LobbyInitializeAsync failed.\n");
        return FALSE;
    }
    
    // デバッグ用スケジュールをセット
    // LobbyNotifyScheduleCallbackでTRUEを返すとここでセットしたものは無視されます。
    {
        u8 buf[sizeof(PPW_LobbySchedule) + sizeof(PPW_LobbyScheduleRecord) * (SCHEDULE_NUM - 1)];
        PPW_LobbySchedule* schedule = (PPW_LobbySchedule*)buf;
        schedule->lockTime = 1*60; // 1分でロック
        schedule->random = 0;
        schedule->roomFlag = PPW_LOBBY_ROOM_FLAG_ARCEUS;
        schedule->roomType = 1;
        schedule->season = 2;
        schedule->scheduleRecordNum = SCHEDULE_NUM;
        schedule->scheduleRecords[0].time = 10;    // ロックから10秒でイベント開始
        schedule->scheduleRecords[0].event = PPW_LOBBY_TIME_EVENT_NEON_A0;
        schedule->scheduleRecords[1].time = 10;    // 同時にイベントを発生させてみる
        schedule->scheduleRecords[1].event = PPW_LOBBY_TIME_EVENT_NEON_B0;
        schedule->scheduleRecords[2].time = 20;
        schedule->scheduleRecords[2].event = PPW_LOBBY_TIME_EVENT_FIRE_WORKS_START;
        schedule->scheduleRecords[3].time = 30;
        schedule->scheduleRecords[3].event = PPW_LOBBY_TIME_EVENT_PARADE;
        schedule->scheduleRecords[4].time = 40;
        schedule->scheduleRecords[4].event = PPW_LOBBY_TIME_EVENT_CLOSE;
        PPW_LobbySetSchedule(schedule, sizeof(buf));
    }
    
    if(!userdata)
    {
        lobby_dummy_func_call();
    }
    
    return TRUE;
}

BOOL lobby_send_message()
{
    return PPW_LobbySendChannelStringMessage(PPW_LOBBY_CHANNEL_KIND_MAIN, 0, "HelloMessage") == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_send_user_message()
{
    return PPW_LobbySendPlayerStringMessage(PPW_LobbyGetMyUserId(), 0, "HelloPlayerMessage") == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_set_channel_data()
{
    return PPW_LobbySetChannelData(PPW_LobbyGetMyUserId(), PPW_LOBBY_CHANNEL_KIND_MAIN,
                                   "ChannelKey", (u8*)"ChannelValue", strlen("ChannelValue") + 1) == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_get_channel_data()
{
    return PPW_LobbyGetChannelDataAsync(PPW_LobbyGetMyUserId(), PPW_LOBBY_CHANNEL_KIND_MAIN, "ChannelKey", (void*)123) == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_set_broadcast_channel_data()
{
    return PPW_LobbySetChannelData(PPW_LobbyGetMyUserId(), PPW_LOBBY_CHANNEL_KIND_MAIN,
                                   "b_BroadcastKey", (u8*)"BroadcastValue", strlen("BroadcastValue") + 1) == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_join_subchannel()
{
    return PPW_LobbyJoinSubChannelAsync(PPW_LOBBY_CHANNEL_KIND_FOOT1) == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_leave_subchannel()
{
    return PPW_LobbyLeaveSubChannel() == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_update_user_profile()
{
    static int user_data_counter = 0;
    user_data_counter++;
    return PPW_LobbyUpdateMyProfile((u8*)&user_data_counter, sizeof(user_data_counter), FALSE) == PPW_LOBBY_RESULT_SUCCESS;
}

BOOL lobby_get_user_profile()
{
    s32 buf = 0;
    u32 size = sizeof(buf);
    if(PPW_LobbyGetProfile(PPW_LobbyGetMyUserId(), (u8*)&buf, &size) != PPW_LOBBY_RESULT_SUCCESS)
    {
        return FALSE;
    }
    DWCDemoPrintf("My profile is %d.\n", buf);
    return TRUE;
}

BOOL lobby_enum_user()
{
    int i;
    s32 users[PPW_LOBBY_MAX_PLAYER_NUM_MAIN];
    u32 num = PPW_LOBBY_MAX_PLAYER_NUM_MAIN;
    if(PPW_LobbyEnumUserId(users, &num) != PPW_LOBBY_RESULT_SUCCESS)
    {
        return FALSE;
    }
    
    DWCDemoPrintf("There is %u users.\n", num + 1);
    DWCDemoPrintf("   MyID: %d.\n", PPW_LobbyGetMyUserId());
    for(i=0; i<num; i++)
    {
        DWCDemoPrintf("   ID: %d.\n", users[i]);
    }
    return TRUE;
}

BOOL lobby_get_time_info()
{
    PPW_LobbyTimeInfo info;
    if(PPW_LobbyGetTimeInfo(&info) != PPW_LOBBY_RESULT_SUCCESS)
    {
        return FALSE;
    }
    
    DWCDemoPrintf("currentTime: %lld.\n", info.currentTime);
    DWCDemoPrintf("openedTime: %lld.\n", info.openedTime);
    DWCDemoPrintf("lockedTime: %lld.\n", info.lockedTime);
    return TRUE;
}

BOOL lobby_get_schedule()
{
    u32 size = 0;
    PPW_LobbySchedule* schedule = NULL;
    
    while(PPW_LobbyGetSchedule(schedule, &size) != PPW_LOBBY_RESULT_SUCCESS)
    {
        if(schedule)
        {
            OS_Free(schedule);
        }
        schedule = OS_Alloc(size);
    }
    
    ShowSchedule(schedule);
    OS_Free(schedule);
    return TRUE;
}

BOOL lobby_get_vip()
{
    u32 num = 0;
    PPW_LobbyVipRecord* vipRecords = NULL;
    
    while(PPW_LobbyGetVip(vipRecords, &num) != PPW_LOBBY_RESULT_SUCCESS)
    {
        if(vipRecords)
        {
            OS_Free(vipRecords);
        }
        
        // VIPがいなかった場合、簡単にするためにnum = 1としてメモリを確保する
        if(num == 0)
        {
            num = 1;
        }
        vipRecords = (PPW_LobbyVipRecord*)OS_Alloc(sizeof(PPW_LobbyVipRecord) * num);
    }
    
    ShowVip(vipRecords, num);
    OS_Free(vipRecords);
    return TRUE;
}

BOOL lobby_submit_questionnaire()
{
    DWCDemoPrintf("I'll submit an answer of No.0.\n");
    return PPW_LobbySubmitQuestionnaire(0) == PPW_LOBBY_RESULT_SUCCESS;
}

const char* lobby_start_recruit()
{
    static PPW_LobbyRecruitInfo info;
    info.gameKind = 0;
    info.maxNum = MAX_PLAYERS;
    info.currentNum = 1;
    info.paramVal = 123;
    strncpy((char*)info.paramBuf, "Recruit", PPW_LOBBY_MAX_MATCHMAKING_BUF_SIZE);
    
    if(PPW_LobbyStartRecruit(&info) != PPW_LOBBY_RESULT_SUCCESS)
    {
        return NULL;
    }
    
    // この文字列をマッチング指標文字列として使ってください。
    DWCDemoPrintf("matchMakingString is %s\n", info.matchMakingString);
    
    return info.matchMakingString;
}

BOOL lobby_stop_recruit()
{
    return PPW_LobbyStopRecruit() == PPW_LOBBY_RESULT_SUCCESS;
}


void lobby_dummy_func_call()
{
    DWCDemoPrintf("Don't execute this. This code is debug purpose.\n");
    
    // メモリ使用量を調べるため残り全ての関数を呼ぶ
    PPW_LobbyInitializePidAsync(NULL, NULL, DWC_LOBBY_CHANNEL_PREFIX_DEBUG, NULL, 0, NULL, 0);
    PPW_LobbyGetSubChannelState();
    PPW_LobbySendPlayerStringMessage(0, 0, NULL);
    PPW_LobbySendChannelBinaryMessage(PPW_LOBBY_CHANNEL_KIND_MAIN, 0, NULL, 0);
    PPW_LobbySendPlayerBinaryMessage(0, 0, NULL, 0);
    PPW_LobbyUpdateRecruitInfo(NULL);
}