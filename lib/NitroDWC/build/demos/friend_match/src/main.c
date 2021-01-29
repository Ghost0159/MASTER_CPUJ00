#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "font.h"
#include "screen.h"

#include "dbs.h"

#include "gamemain.h"
#include "registerfriend.h"
#include "logon.h"
#include "transport.h"

//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
GameControl stGameCnt;  // ゲーム制御情報構造体
KeyControl stKeyCnt;    // キー入力制御構造体
DWCFriendsMatchControl stDwcCnt;    // DWC制御構造体

// ユーザデータを格納する構造体。
DWCUserData stUserData;
DWCInetControl stConnCtrl;

// デバッグ出力のオーバーライド
static BOOL sPrintOverride;


//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void InitGameData(void);

static GameMode StartIPMain(void);
static GameMode StopIPMain(void);

static void MatchModeDispCallback(int curIdx, void* param);

static void VBlankIntr(void);

//----------------------------------------------------------------------------
// initialized variable
//----------------------------------------------------------------------------
// ゲームシーケンスリスト
GameSequence gameSeqList[GAME_MODE_NUM] =
{
    {
        "GAME MAIN MODE",
        GameMain,
        NULL,
        4,
        {
            "Login", GAME_MODE_LOGIN, TRUE,
            "Register friend", GAME_MODE_REG_FRIEND, FALSE,
            "StartIP", GAME_MODE_STARTIP, FALSE,
            "StopIP", GAME_MODE_STOPIP, FALSE,
        },
    },
    {
        "REGISTER FRIEND MODE",
        GameRegisterFriendMain,
        RegFriendModeDispCallback,
        4,
        {
            "MP START:A STOP:B", GAME_MODE_NUM, FALSE,
            "FriendKey Set", GAME_MODE_NUM, FALSE,
            "Delete", GAME_MODE_NUM, FALSE,
            "Return", GAME_MODE_MAIN, FALSE,
        },
    },
    {
        "GAME LOGON MODE",
        GameLogonMain,
        LogonModeDispCallback,
        5,
        {
            "Connect to anybody", GAME_MODE_CONNECTED, TRUE,
            "Connect to friends", GAME_MODE_CONNECTED, TRUE,
            "Setup game server", GAME_MODE_CONNECTED, TRUE,
            "Connect to game server", GAME_MODE_CONNECTED, TRUE,
            "Logout", GAME_MODE_MAIN, FALSE
        },
    },
    {
        "GAME CONNECTED MODE",
        GameConnectedMain,
        NULL,
        3,
        {
            "DWC_SendReliable", GAME_MODE_NUM, FALSE,
            "DWC_SendUnreliable", GAME_MODE_NUM, FALSE,            
            "Close connections", GAME_MODE_LOGIN, TRUE
        },
    },
    {
        "START TO CONNECT INTERNET",
        StartIPMain,        NULL,        0,
    },
    {
        "STOP TO CONNECT INTERNET",
        StopIPMain,        NULL,        0,
    },
};


// サンプルではセーブできないのでこれをセーブデータの代わりとみなす
GameSaveData saveData =
{
    {
        "Player1", "Player2", "Player3", "Player4"
    },
    0, FALSE,
    {
        0,
    }
};

/*---------------------------------------------------------------------------*
  メモリ確保関数
 *---------------------------------------------------------------------------*/
static void*
AllocFunc( DWCAllocType name, u32   size, int align )
{
    void * ptr;
    OSIntrMode old;
    (void)name;
    (void)align;

    old = OS_DisableInterrupts();

    ptr = OS_AllocFromMain( size );

    OS_RestoreInterrupts( old );

    return ptr;
}

/*---------------------------------------------------------------------------*
  メモリ開放関数
 *---------------------------------------------------------------------------*/
static void
FreeFunc( DWCAllocType name, void* ptr,  u32 size  )
{
    OSIntrMode old;
    (void)name;
    (void)size;

    if ( !ptr ) return;

    old = OS_DisableInterrupts();

    OS_FreeToMain( ptr );

    OS_RestoreInterrupts( old );
}

/** --------------------------------------------------------------------
  OS_TPrintf()のコンソール出力対応
  ----------------------------------------------------------------------*/
#ifndef SDK_FINALROM
extern void OS_TVPrintf(const char *fmt, va_list vlist );
static char common_buffer[256];        // thread unsafe, but less use of stack

void OS_TVPrintf(const char *fmt, va_list vlist )
{
    (void)OS_VSNPrintf(common_buffer, sizeof(common_buffer), fmt, vlist);
    OS_PutString(common_buffer);

    if ( sPrintOverride )
    {
        dbs_CVPrintf( NULL, fmt, vlist );
    }
}
#endif


/*---------------------------------------------------------------------------*
  メインルーチン
 *---------------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
void NitroMain ()
{
    int friendIdx = 0;
	
	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();

    // スタック溢れチェック初期設定
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    
    PXI_Init();
    
    Heap_Init();
	Alarm_Init();

    GX_Init();
    FS_Init( MI_DMA_MAX_NUM ); 

    GX_DispOff();
    GXS_DispOff();

    
    // Vブランク割り込み許可
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);
	GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    // 出力初期化
    InitScreen();
    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.
    
    // 表示開始
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // キー入力初期化
    InitKeyData(&stKeyCnt);

    InitGameData();

    // DWCライブラリ初期化
    DWC_Init( s_Work );

    // ユーザデータ作成
    DWC_CreateUserData( &stUserData, 'ABCD' );

    // ユーザデータ表示
    DWC_ReportUserData( &stUserData );

    // デバッグ表示レベル指定
    DWC_SetReportLevel(DWC_REPORTFLAG_ALL);

    // メモリ確保関数設定
    DWC_SetMemFunc( AllocFunc, FreeFunc );
        
    // メインループ
    while (1){
        // メニュー表示
        DispMenuMsgWithCursor(&gameSeqList[stGameCnt.mode], 0, NULL);

        // シーケンスに対応するループ関数を呼び出す
        stGameCnt.mode = gameSeqList[stGameCnt.mode].seqFunc();
    }

	OS_Terminate();
}


/*---------------------------------------------------------------------------*
  ゲームデータとホスト情報初期化関数
 *---------------------------------------------------------------------------*/
static void InitGameData(void)
{

    // ゲーム制御構造体初期化
    stGameCnt.mode     = GAME_MODE_MAIN;
    stGameCnt.blocking = FALSE;
    MI_CpuClear8(stGameCnt.friendStatus, sizeof(stGameCnt.friendStatus));

    // セーブデータからプレイヤーデータをロードしたと想定。
    // とりあえずどの本体でもインデックス0のデータを使う。
    stGameCnt.userData.playerIdx = 0;
    MI_CpuCopy32(saveData.playerData[stGameCnt.userData.playerIdx].playerName,
                 stGameCnt.userData.playerName, sizeof(stGameCnt.userData.playerName));
    stGameCnt.userData.profileID = saveData.profileID;
    stGameCnt.userData.isValidProfile = saveData.isValidProfile;
    MI_CpuCopy32(&saveData.friendList, &stGameCnt.friendList, sizeof(GameFriendList));
}


/*---------------------------------------------------------------------------*
  ネットワーク切断関数
 *---------------------------------------------------------------------------*/
void ShutdownInet(void)
{
    DWC_ShutdownFriendsMatch();   // DWCライブラリ終了。GameSpyのヒープ開放
}

/*---------------------------------------------------------------------------*
  通信エラー表示関数
 *---------------------------------------------------------------------------*/
void DispErrorMessage(void)
{
    int errorCode;
    DWCError error;

    error = DWC_GetLastError(&errorCode);

    if (error == DWC_ERROR_NONE) return;

    switch (error){
    case DWC_ERROR_DS_MEMORY_ANY:
        OS_TPrintf("ERROR: DS_MEMORY_ANY.\n");
        break;
    case DWC_ERROR_AUTH_ANY:
        OS_TPrintf("ERROR: AUTH_ANY.\n");
        break;
    case DWC_ERROR_AUTH_OUT_OF_SERVICE:
        OS_TPrintf("ERROR: This game's network service has ended.\n");
        break;
    case DWC_ERROR_AUTH_STOP_SERVICE:
        OS_TPrintf("ERROR: Now server is temporarily unavailable.\nPlease re-login later.\n");
        break;
    case DWC_ERROR_AC_ANY:
        OS_TPrintf("ERROR: AC ANY.\n");
        break;
    case DWC_ERROR_NETWORK:
        OS_TPrintf("ERROR: Network error.\n");
        break;
    case DWC_ERROR_FRIENDS_SHORTAGE:
        OS_TPrintf("ERROR: Your friends seem not to login yet.\n");
        break;
    case DWC_ERROR_NOT_FRIEND_SERVER:
        OS_TPrintf("ERROR: Game server is fully occupied or canceled matching, or he is not mutually registered friend.\n");
        break;
    case DWC_ERROR_FATAL:
        OS_TPrintf("FATAL ERROR: Please cut the power off.\n");
        break;
    default:
        OS_TPrintf("ERROR: Undefined yet.\n");
        break;
    }

    OS_TPrintf("error code = %d\n", -1*errorCode);
}


/*---------------------------------------------------------------------------*
  メッセージリスト表示（カーソル付き）関数
 *---------------------------------------------------------------------------*/
void DispMenuMsgWithCursor(const GameSequence* gameSeq, int index, void* param)
{
    int i;

    OS_TPrintf("===============================\n");
    OS_TPrintf("%s\n", gameSeq->modeName);

    for (i = 0; i < gameSeq->numMenu; i++){
        if (i == index) OS_TPrintf("> ");
        else OS_TPrintf("  ");

        OS_TPrintf("%s\n", gameSeq->menuList[i].msg);
    }

    // モードごとの追加表示用コールバック関数を呼び出す
    if (gameSeq->dispCallback) gameSeq->dispCallback(index, param);

    OS_TPrintf("===============================\n\n");
}

/*---------------------------------------------------------------------------*
  友達リスト表示関数（GameSpy Status文字列表示版）
 *---------------------------------------------------------------------------*/
void DispFriendList(BOOL overLoad)
{
    u8   maxEntry = 0;
    u8   numEntry;
    char statusString[DWC_GS_STATUS_STRING_LEN];
    int  i;

    statusString[0] = '\0';

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i]))
            continue;
        
        if (overLoad){
            // 上書き指定の場合は友達情報の更新も行う
            stGameCnt.friendStatus[i] =
                DWC_GetFriendStatusSC(&stGameCnt.friendList.keyList[i],
                                      &maxEntry, &numEntry, statusString);
        }

        if (maxEntry){
            // 友達がサーバクライアントマッチメイクのサーバDSの場合は、
            // （現在接続人数／最大接続人数）を表示する
            if(DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]) != 4294967295){
                OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, server (%d/%d), statusString : %s\n",
                           i, stGameCnt.friendList.playerName[i],
                           DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                           stGameCnt.friendStatus[i],
                           numEntry, maxEntry, statusString);
            }
            else{// MP通信直後でまだ友達のprofileIDが貰えていない場合
                OS_TPrintf("Friend[%d] '%s' : profileID **********, status %d, server (%d/%d), statusString : %s\n",
                           i, stGameCnt.friendList.playerName[i],
                           stGameCnt.friendStatus[i],
                           numEntry, maxEntry, statusString);
            }
        }
        else {
            if(DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]) != 4294967295){
                OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, statusString : %s\n",
                           i, stGameCnt.friendList.playerName[i],
                           DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                           stGameCnt.friendStatus[i], statusString);
            }
            else{// MP通信直後でまだ友達のprofileIDが貰えていない場合
                OS_TPrintf("Friend[%d] '%s' : profileID **********, status %d, statusString : %s\n",
                           i, stGameCnt.friendList.playerName[i],
                           stGameCnt.friendStatus[i], statusString);
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  友達リスト表示関数（GameSpy Statusデータ表示版）
 *---------------------------------------------------------------------------*/
void DispFriendListData(void)
{
    u8   maxEntry = 0;
    u8   numEntry;
    char statusData[DWC_GS_STATUS_STRING_LEN];
    int  size;
    int  i, j;

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i]))
            continue;
        
        // 友達情報の更新
        stGameCnt.friendStatus[i] =
            stGameCnt.friendStatus[i] =
                DWC_GetFriendStatusDataSC(&stGameCnt.friendList.keyList[i],
                                          &maxEntry, &numEntry, statusData, &size);
        
        if (maxEntry){
            // 友達がサーバクライアントマッチメイクのサーバDSの場合は、
            // （現在接続人数／最大接続人数）を表示する
            if(DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]) != 4294967295){
                OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, server (%d/%d)\n",
                           i, stGameCnt.friendList.playerName[i],
                           DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                           stGameCnt.friendStatus[i],
                           numEntry, maxEntry);
            }
            else{// MP通信直後でまだ友達のprofileIDが貰えていない場合
                OS_TPrintf("Friend[%d] '%s' : profileID **********, status %d, server (%d/%d)\n",
                           i, stGameCnt.friendList.playerName[i],
                           stGameCnt.friendStatus[i],
                           numEntry, maxEntry);
            }
        }
        else {
            if(DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]) != 4294967295){
                OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d\n",
                           i, stGameCnt.friendList.playerName[i],
                           DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                           stGameCnt.friendStatus[i]);
            }
            else{// MP通信直後でまだ友達のprofileIDが貰えていない場合
                OS_TPrintf("Friend[%d] '%s' : profileID **********, status %d\n",
                           i, stGameCnt.friendList.playerName[i],
                           stGameCnt.friendStatus[i]);
            }
        }

        // GameSpy Status文字列を表示する
        OS_TPrintf("StatusData : ");
        for (j = 0; j < size; j++){
            OS_TPrintf("%02d, ", statusData[j]);
        }
        OS_TPrintf("\n");
    }
}

/*---------------------------------------------------------------------------*
  マッチメイクモードの追加表示用コールバック関数
 *---------------------------------------------------------------------------*/
static void MatchModeDispCallback(int curIdx, void* param)
{
#pragma unused(curIdx)
#pragma unused(param)
    
}

/*---------------------------------------------------------------------------*
  IPの取得
 *---------------------------------------------------------------------------*/
static GameMode StartIPMain(void)
{
    DWC_InitInet( &stConnCtrl );
    DWC_SetAuthServer( DWC_CONNECTINET_AUTH_RELEASE );

    DWC_ConnectInetAsync();
    // 安定なステートまで待つ。
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
    }

    switch ( DWC_GetInetStatus() )
    {
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // エラー表示
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_TPrintf("   Error occurred %d %d.\n", err, errcode );
        }
        DWC_ClearError();

        DWC_CleanupInet();
        break;
    case DWC_CONNECTINET_STATE_FATAL_ERROR:
        {
            // エラー表示
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_Panic("   Error occurred %d %d.\n", err, errcode );
        }
        break;

    case DWC_CONNECTINET_STATE_CONNECTED:
        OS_TPrintf("   CONNECTED!!!.\n");
        break;
    }

    return GAME_MODE_MAIN;
}

static GameMode StopIPMain(void)
{
    DWC_CleanupInet();

    return GAME_MODE_MAIN;
}

/*---------------------------------------------------------------------------*
  友達リストの空きインデックス取得関数
 *---------------------------------------------------------------------------*/
int GetAvailableFriendListIndex(void)
{
    int i;

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i])){
            return i;
        }
    }

    return -1;
}

/*---------------------------------------------------------------------------*
  Vブランク割り込み関数
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    //---- upload pseudo screen to VRAM
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));
    
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
