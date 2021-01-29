#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"

#include "dbs.h"



//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_NAME        "dwctest" // このサンプルが使用するゲーム名
#define GAME_SECRET_KEY  "d4q9GZ"  // このサンプルが使用するシークレットキー
#define GAME_PRODUCTID   10824      // このサンプルが使用するプロダクトID

#define GAME_FRAME       1         // 想定するゲームフレーム（1/60を1とする）

#define GAME_NUM_MATCH_KEYS 3      // マッチメイク用追加キー個数

#define NETCONFIG_USE_HEAP 1

// DWC_MATCH_OPTION_SC_CONNECT_BLOCKマッチメイクオプションを試すスイッチ
//#define GAME_MATCH_OP_SC_CONNECT_BLOCK

// ストレージサーバへのセーブ・ロードを試すスイッチ
//#define GAME_USE_STORAGE_SERVER

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
// マッチメイク１件完了後、次のマッチメイクを受け付けるまでの時間（単位：秒）
#define GAME_MATCH_OP_SC_BLOCK_TIME 10
#endif

// 認証サーバをデフォルトの開発向け認証サーバから製品向け認証サーバに変更する
// 場合は、下記の定義を有効にしてください。
//#define USE_AUTHSERVER_RELEASE 1

// メモリ確保/開放時にメモリを埋めるデバッグオプション
#undef DEBUG_OPTION_FILL_HEAP

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
typedef struct tagRegFriendParam
{
    int step;    // 0:コマンド選択中、1:登録・削除友達情報入力中
    u32 figure;  // 入力中のプロファイルIDの桁
    u32 value;   // 入力中の値
} RegFriendParam;


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static GameControl stGameCnt;  // ゲーム制御情報構造体
static KeyControl stKeyCnt;    // キー入力制御構造体
static DWCFriendsMatchControl stDwcCnt;    // FriendsMatch制御構造体

static int stNumEntry  = 2;    // ネットワーク構成人数指定
static int stServerIdx = 0;    // 接続したいサーバDSの友達リストインデックス

static BOOL stCloseFlag = FALSE;    // 自分でクローズした場合はTRUE

// 友達無指定ピアマッチメイク用追加キーデータ構造体
static GameMatchExtKeys stMatchKeys[GAME_NUM_MATCH_KEYS] = { 0, };

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
static OSTick stMatchFinishTime = 0;  // サーバクライアントマッチメイクでマッチメイクが終わった時間
#endif

// ユーザデータを格納する構造体。
static DWCUserData stUserData;
static DWCInetControl stConnCtrl;

// デバッグ出力のオーバーライド
static BOOL sPrintOverride;

static u8 stNumBackup;
static u16 card_lock_id;

static u32 loopcnt = 0;
static char loopstr[] = "/|\\-";

// Wi-Fiコネクション設定の使用言語
static int utilityLangage = DWC_LANGUAGE_JAPANESE;

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void InitGameData(void);
static void SetGameMatchKeys(void);

static GameMode GameMain(void);
static GameMode GameRegisterFriendMain(void);
static GameMode GameLogonMain(void);
static GameMode GameMatchMain(void);
static GameMode GameConnectedMain(void);
static GameMode NetConfigLanguageMain(void);
static GameMode NetConfigMain(void);
static GameMode StartIPMain(void);
static GameMode StopIPMain(void);
static GameMode BackupUserdataMain(void);

static int GetAvailableFriendListIndex(void);

static void LoginCallback(DWCError error, int profileID, void* param);

static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param);
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param);
static void DeleteFriendListCallback(int deletedIndex, int srcIndex, void* param);
static void BuddyFriendCallback(int index, void* param);

static void LoginToStorageCallback(DWCError error, void* param);
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param);
static void LoadFromServerCallback(BOOL success, int index, char* data, int len, void* param);

static void ConnectToAnybodyCallback(DWCError error, BOOL cancel, void* param);
static void ConnectToFriendsCallback(DWCError error, BOOL cancel, void* param);
static void SetupGameServerCallback(DWCError error,
                                    BOOL cancel,
                                    BOOL self,
                                    BOOL isServer,
                                    int index,
                                    void* param);
static void ConnectToGameServerCallback(DWCError error,
                                        BOOL cancel,
                                        BOOL self,
                                        BOOL isServer,
                                        int index,
                                        void* param);
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8 aid,
                                     int index,
                                     void* param);
static void NewClientCallback(int index, void* param);
static int  EvaluateAnybodyCallback(int index, void* param);
static int  EvaluateFriendCallback(int index, void* param);
#ifdef DWC_STOP_SC_SERVER
static void StopSCMatchingCallback(void* param);
#endif

static DWCError HandleDWCError(GameMode* gameMode);

static void DispErrorMessage(DWCError error, int errorCode, DWCErrorType errorType);
static void DispMenuMsgWithCursor(const GameSequence* gameSeq, int index, void* param);
static void DispFriendList(BOOL overLoad);
static void DispFriendListData(void);

static void RegFriendModeDispCallback(int curIdx, void* param);
static void LogonModeDispCallback(int curIdx, void* param);
static void MatchModeDispCallback(int curIdx, void* param);
static void BackupModeDispCallback(int curIdx, void* param);

static void GameWaitVBlankIntr(void);
static void VBlankIntr(void);


//----------------------------------------------------------------------------
// initialized variable
//----------------------------------------------------------------------------
// ゲームシーケンスリスト
static GameSequence gameSeqList[GAME_MODE_NUM] =
{
    {
        "GAME MAIN MODE",
        GameMain,
        NULL,
        6,
        {
            "Login", GAME_MODE_LOGIN, TRUE,
            "Register friend", GAME_MODE_REG_FRIEND, FALSE,
            "NetConfig", GAME_MODE_NETCONFIG_LANGUAGE, FALSE,
            "StartIP", GAME_MODE_STARTIP, FALSE,
            "StopIP", GAME_MODE_STOPIP, FALSE,
            "UserData Backup", GAME_MODE_BACKUP, FALSE,
        },
    },
    {
        "REGISTER FRIEND MODE",
        GameRegisterFriendMain,
        RegFriendModeDispCallback,
        3,
        {
            "Set", GAME_MODE_NUM, FALSE,
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
            "Close connections", GAME_MODE_LOGIN, FALSE
        },
    },
    {
        "LANGUAGE",
        NetConfigLanguageMain,
        NULL,
        8,
        {
            "Japanese", GAME_MODE_NETCONFIG, FALSE,
            "English", GAME_MODE_NETCONFIG, FALSE,
            "French", GAME_MODE_NETCONFIG, FALSE,
            "German", GAME_MODE_NETCONFIG, FALSE,
            "Italian", GAME_MODE_NETCONFIG, FALSE,
            "Spanish", GAME_MODE_NETCONFIG, FALSE,
            "Hangul", GAME_MODE_NETCONFIG, FALSE,
            "Return", GAME_MODE_MAIN, FALSE
        },
    },
    {
        "NET CONFIG GUI",
        NetConfigMain,        NULL,        0,
    },
    {
        "START TO CONNECT INTERNET",
        StartIPMain,        NULL,        0,
    },
    {
        "STOP TO CONNECT INTERNET",
        StopIPMain,        NULL,        0,
    },
    {
        "BACKUP USERDATA",
        BackupUserdataMain,
        BackupModeDispCallback,
        4,
        {
            "Load UserData", GAME_MODE_NUM, FALSE,
            "Save UserData", GAME_MODE_NUM, FALSE,
            "Report UserData", GAME_MODE_NUM, FALSE,
            "return", GAME_MODE_MAIN, FALSE,
        }
    },
};


// サンプルではセーブできないのでこれをセーブデータの代わりとみなす
static GameSaveData saveData =
{
    {
        "Player1", "Player2", "Player3", "Player4"
    },
    0, FALSE,
    {
        0,
    }
};

// GameSpy Statusデータとしてセットするサンプルデータ
static const char gsStatusSample[16] = { 10, 9, 8, 0, 0, 1, 2, 3, };

static const struct
{
    CARDBackupType type;
    const char *comment;
} stBackupTypeTable[] = {
    { CARD_BACKUP_TYPE_EEPROM_4KBITS,   "EEPROM    4 kb" },
    { CARD_BACKUP_TYPE_EEPROM_64KBITS,  "EEPROM   64 kb" },
    { CARD_BACKUP_TYPE_EEPROM_512KBITS, "EEPROM  512 kb" },
    { CARD_BACKUP_TYPE_FLASH_2MBITS,    "FLASH     2 Mb" },
    { CARD_BACKUP_TYPE_FRAM_256KBITS,   "FRAM    256 kb" },
} ;
enum { BACKUP_TABLE_MAX = sizeof(stBackupTypeTable) / sizeof(*stBackupTypeTable) } ;


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

#ifdef DEBUG_OPTION_FILL_HEAP
    MI_CpuFill8( ptr, 0xac, size );
#endif

    //OS_TPrintf( "alloc: ptr=0x%08X size=%d\n", ptr, size );

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

#ifdef DEBUG_OPTION_FILL_HEAP
    MI_CpuFill8( ptr, 0xfe, size );
#endif

    OS_FreeToMain( ptr );

    //OS_TPrintf( "free : ptr=0x%08X size=%d\n", ptr, size );

    OS_RestoreInterrupts( old );
}

/** --------------------------------------------------------------------
  OS_TPrintf()のコンソール出力対応
  ----------------------------------------------------------------------*/
#ifdef SDK_FINALROM
#undef OS_TVPrintf
#undef OS_TPrintf
#endif
extern void OS_TVPrintf(const char *fmt, va_list vlist );
static char common_buffer[256];        // thread unsafe, but less use of stack

void OS_TVPrintf(const char *fmt, va_list vlist )
{
    (void)OS_VSNPrintf(common_buffer, sizeof(common_buffer), fmt, vlist);
#ifndef SDK_FINALROM
    OS_PutString(common_buffer);
#endif

    if ( sPrintOverride )
    {
        dbs_CVPrintf( NULL, fmt, vlist );
    }
}
#ifdef SDK_FINALROM
extern void OS_TPrintf(const char *fmt, ...);
void OS_TPrintf(const char *fmt, ...)
{
    va_list vlist;

    va_start(vlist, fmt);
    OS_TVPrintf(fmt, vlist);
    va_end(vlist);
}
#endif


/*---------------------------------------------------------------------------*
  メインルーチン
 *---------------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
void NitroMain ()
{
    int friendIdx = 0;
    int ret;

	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    CARD_Init();
    ret = OS_GetLockID();
    if (ret == OS_LOCK_ID_ERROR)
    {
        OS_Panic("demo fatal error! OS_GetLockID() failed");
    }
    card_lock_id = (u16)ret;

    // スタック溢れチェック初期設定
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    
    PXI_Init();
    
    Heap_Init();
	Alarm_Init();
//    Heap_SetDebug( TRUE );

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
    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.

    // 表示開始
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // キー入力初期化
    InitKeyData(&stKeyCnt);

    InitGameData();

    // デバッグ用にコンソールの情報を削除する。
    //DWC_Debug_DWCInitError( s_Work, DWC_INIT_RESULT_DESTROY_OTHER_SETTING );

    // デバッグ表示レベル指定
    DWC_SetReportLevel((unsigned long)(DWC_REPORTFLAG_ALL & ~DWC_REPORTFLAG_QR2_REQ));
	gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_Verbose);

    // DWCライブラリ初期化
    ret = DWC_Init( s_Work );
    OS_TPrintf( "DWC_Init() result = %d\n", ret );

    if ( ret == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        OS_TPrintf( "Wi-Fi setting might be broken.\n" );
    }
    
    // ユーザデータ作成
    DWC_CreateUserData( &stUserData, 'ABCD' );

#if 0
    OS_TPrintf("+++VALID CONSOLE %d +++", DWC_CheckValidConsole( &stUserData ) );
    {
        char test[16];
        sprintf(test,"%u",0xa1546344);

        OS_PutString(test);
    }
#endif

    // ユーザデータ表示
    DWC_ReportUserData( &stUserData );

    // ヒープ使用量表示ON
    //Heap_SetDebug(TRUE);

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
  ゲーム定義のマッチメイク用追加キーセット関数
 *---------------------------------------------------------------------------*/
static void SetGameMatchKeys(void)
{
    int ranking = (int)((OS_GetTick()*1592653589UL+453816691UL) & 0xff);

    // ランキングを整数キーとして設定
    stMatchKeys[0].isStr  = 0;
    strcpy(stMatchKeys[0].keyStr, "game_rank");
    stMatchKeys[0].ivalue = ranking;
    stMatchKeys[0].keyID  =
        DWC_AddMatchKeyInt(stMatchKeys[0].keyID,
                           stMatchKeys[0].keyStr,
                           &stMatchKeys[0].ivalue);
    if (stMatchKeys[0].keyID == 0) OS_TPrintf("AddMatchKey failed 0.\n");

    // マッチメイクに用いる評価タイプを文字列キーとして設定
    stMatchKeys[1].isStr  = 1;
    strcpy(stMatchKeys[1].keyStr, "game_mtype");
    strcpy(stMatchKeys[1].svalue, "eval_rank");
    stMatchKeys[1].keyID  =
        DWC_AddMatchKeyString(stMatchKeys[1].keyID,
                              stMatchKeys[1].keyStr,
                              stMatchKeys[1].svalue);
    if (stMatchKeys[1].keyID == 0) OS_TPrintf("AddMatchKey failed 1.\n");

    // とりあえずもう一つぐらい整数キーを設定
    stMatchKeys[2].isStr  = 0;
    strcpy(stMatchKeys[2].keyStr, "game_manner");
    stMatchKeys[2].ivalue = ranking%11;
    stMatchKeys[2].keyID  =
        DWC_AddMatchKeyInt(stMatchKeys[2].keyID,
                           stMatchKeys[2].keyStr,
                           &stMatchKeys[2].ivalue);
    if (stMatchKeys[2].keyID == 0) OS_TPrintf("AddMatchKey failed 2.\n");
}


/*---------------------------------------------------------------------------*
  オフライン時メイン関数
 *---------------------------------------------------------------------------*/
static GameMode GameMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_MAIN];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

#if 0
// サンプルのMP交換用友達情報表示テスト
{
    DWCFriendData friendData;
    int i;
    
    DWC_CreateExchangeToken(&stUserData, &friendData);
    for (i = 0; i < 12; i++){
        OS_TPrintf("0x%02x, ", ((u8 *)&friendData)[i]);
    }
    OS_TPrintf("\n");
}
#endif


    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;
		ReadKeyData();  // キーデータ取得

        DWC_ProcessFriendsMatch();  // FriendsMatch通信処理更新

        HandleDWCError(&returnSeq);  // エラー処理

        if (stGameCnt.blocking){
            // DWC処理中はキー操作を禁止する
            // Vブランク待ち処理
            GameWaitVBlankIntr();
            continue;
        }

        // 次に進むべきモードがセットされていたら認証完了
        if (returnSeq == GAME_MODE_LOGIN){
            // ログインが選択されていた場合
            if (stGameCnt.userData.isValidProfile){
                // 認証成功でプロファイルIDを取得できた場合はループを抜ける
                break;
            }
            else {
                // 認証失敗の場合はモードを進めないようにする
                returnSeq = GAME_MODE_NUM;
            }
        }
        else if (returnSeq != GAME_MODE_NUM){
            // それ以外の場合は必ず抜ける
            break;
        }
            
        ////////// 以下キー操作処理
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // Aボタンでメニュー決定
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (returnSeq){
            case GAME_MODE_LOGIN:  // ログイン

                // FriendsMatchライブラリ初期化
                // ライブラリが使うソケットの送受信バッファサイズは
                // デフォルトのまま（8KByte）にしておく
                DWC_InitFriendsMatch(&stDwcCnt, &stUserData, GAME_PRODUCTID, GAME_NAME,
                                     GAME_SECRET_KEY, 0, 0,
                                     stGameCnt.friendList.keyList, GAME_MAX_FRIEND_LIST);

                // 認証用関数を使ってログイン
                if (!DWC_LoginAsync(L"_INGAMESN_", NULL, LoginCallback, &returnSeq)){
                    // ログインを開始できなかった場合はブロッキング状態にしない。
                    // エラー発生中か二重ログインでなければFALSEにはならない。
                    OS_TPrintf("Can't call DWC_LoginAsync().\n");
                    returnSeq          = GAME_MODE_NUM;
                    stGameCnt.blocking = 0;
                }
                break;
            default:
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // 十字キー上でカーソル移動
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // 十字キー下でカーソル移動
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if ( stKeyCnt.trg & PAD_BUTTON_Y){
            Heap_Dump();
        }
        ////////// キー操作処理ここまで

        // Vブランク待ち処理
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  友達登録メイン関数
 *---------------------------------------------------------------------------*/
static GameMode GameRegisterFriendMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_REG_FRIEND];
    GameMode returnSeq = GAME_MODE_NUM;
    RegFriendParam cntParam;
    int curIdx = 0;
    int friendIdx;
    int maxFigure;
    int validFlg;

    cntParam.step   = 0;
    cntParam.figure = 0;

    while (1){
        ReadKeyData();  // キーデータ取得

        // 次に進むべきモードがセットされていたら抜ける
        if (returnSeq != GAME_MODE_NUM){
            break;
        }
            
        ////////// 以下キー操作処理
        if (cntParam.step == 0){
            // コマンド選択中
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // Aボタンでメニュー決定
                returnSeq = gameSeq->menuList[curIdx].mode;
                stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

                if ((curIdx == 0) || (curIdx == 1)){
                    // 友達のプロファイルID登録（今は生のプロファイルID）、
                    // もしくはインデックスを指定して友達情報を削除
                    cntParam.step   = 1;
                    cntParam.figure = 0;
                    cntParam.value  = 0;
                    
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);

                    if (curIdx == 0){
                        maxFigure = 10;  // プロファイルIDは最高１０桁
                    }
                    else {
                        maxFigure = 2;   // 友達リストインデックスは最高２桁
                    }
                }
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // 十字キー上でカーソル移動
                curIdx--;
                if (curIdx < 0) curIdx = gameSeq->numMenu-1;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // 十字キー下でカーソル移動
                curIdx++;
                if (curIdx >= gameSeq->numMenu) curIdx = 0;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        else {
            // 数値入力中
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // Aボタンで友達の登録・削除を行う
                if (curIdx == 0){
                    // 友達登録
                    if (cntParam.value && DWC_CanChangeFriendList()){
                        friendIdx = GetAvailableFriendListIndex();
                        if (friendIdx == -1){
                            OS_TPrintf("Failed to register new friend. Friend list is full.\n");
                        }
                        else {
                            DWC_SetGsProfileId(&stGameCnt.friendList.keyList[friendIdx],
                                               (int)cntParam.value);
                        }
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // プロファイルID=0は無効
                    }
                }
                else {
                    // 友達削除
                    if ((cntParam.value < GAME_MAX_FRIEND_LIST) &&
                        DWC_CanChangeFriendList()){
                        // 友達リストから友達を削除
                        DWC_DeleteBuddyFriendData(&stGameCnt.friendList.keyList[cntParam.value]);
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // 友達リスト範囲外
                    }
                }

                if (validFlg){
                    // 有効な入力であればコマンド選択に戻る
                    cntParam.step = 0;
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
            }
            else if (stKeyCnt.trg & PAD_BUTTON_B){
                // Bボタンでコマンド選択に戻る
                cntParam.step = 0;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // 十字キー上下で選択中の数値の増減（2^32越えの判定はなし）
                if ((cntParam.value/UtilPow(10, cntParam.figure))%10 == 9){
                    cntParam.value -= UtilPow(10,  cntParam.figure)*9;
                }
                else {
                    cntParam.value += UtilPow(10, cntParam.figure);
                }
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // 十字キー上下で選択中の数値の増減（2^32越えの判定はなし）
                if ((cntParam.value/UtilPow(10, cntParam.figure))%10 == 0){
                    cntParam.value += UtilPow(10, cntParam.figure)*9;
                }
                else {
                    cntParam.value -= UtilPow(10, cntParam.figure);
                }
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_LEFT){
                // 十字キー左右で選択桁数の移動
                if (cntParam.figure < maxFigure-1) cntParam.figure++;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_RIGHT){
                // 十字キー左右で選択桁数の移動
                if (cntParam.figure > 0) cntParam.figure--;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        ////////// キー操作処理ここまで

        // Vブランク待ち処理
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  ログイン後メイン関数
 *---------------------------------------------------------------------------*/
static GameMode GameLogonMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_LOGIN];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;
    BOOL boolRet;

    stNumEntry  = 2;
    stServerIdx = 0;

#if 0
// サンプルの友達登録鍵表示テスト
{
    char tmpstr[DWC_FRIENDKEY_STRING_BUFSIZE];
    
    DWC_FriendKeyToString(tmpstr, DWC_CreateFriendKey(&stUserData));
    OS_TPrintf("friendKeyToken '%s'\n", tmpstr);
}
#endif

#ifdef GAME_USE_STORAGE_SERVER
// GameSpyストレージサーバへのセーブ・ロードを行なうテストのためのログイン処理
{
    if (!DWC_LoginToStorageServerAsync(LoginToStorageCallback, NULL)){
        OS_TPrintf("DWC_LoginToStorageServerAsync() failed.\n");
    }
}
#endif

#if 0
// 指定人数以下でのマッチメイク完了オプション設定テスト
{
    DWCMatchOptMinComplete optval;

    optval.valid    = 1;
    optval.minEntry = 2;
    optval.timeout  = 30000;

    if (DWC_SetMatchingOption(DWC_MATCH_OPTION_MIN_COMPLETE, &optval, sizeof(optval))){
        OS_TPrintf("Failed to set matching option\n");
    }
}
#endif

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
// 接続完了後、新規接続をブロックするオプション設定テスト
{
    BOOL boolval = TRUE;

    DWC_SetMatchingOption(DWC_MATCH_OPTION_SC_CONNECT_BLOCK, &boolval, sizeof(boolval));

    stMatchFinishTime = 0;  // マッチメイク終了後時間初期化
}
#endif

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;

        ReadKeyData();  // キーデータ取得

        DWC_ProcessFriendsMatch();  // DWC通信処理更新

        HandleDWCError(&returnSeq);  // エラー処理

        if (stGameCnt.blocking){
            // DWC処理中はキー操作を禁止する（キャンセルのみ可）
            if (stKeyCnt.trg & PAD_BUTTON_B){
                // Bボタンでマッチメイクをキャンセルする
                boolRet = DWC_CancelMatching();
                if (!boolRet){
                    OS_TPrintf("Now unable to cancel matching.\n");
                }
            }

            // Vブランク待ち処理
            GameWaitVBlankIntr();
            continue;
        }

        // 次に進むべきモードがセットされていたらループを抜ける
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// 以下キー操作処理
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // Aボタンでメニュー決定
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;
            boolRet   = TRUE;

            switch (curIdx){
            case 0:  // 友達無指定ピアマッチメイク呼び出し
                if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                    // Xボタンが押されていなければ、マッチメイク用追加キーデータ設定
                    SetGameMatchKeys();
                }
                
                boolRet =
                    DWC_ConnectToAnybodyAsync((u8)stNumEntry,
                                              "game_mtype = 'eval_rank'",
                                              ConnectToAnybodyCallback,
                                              &returnSeq,
                                              EvaluateAnybodyCallback,
                                              NULL);
                break;
            case 1:  // 友達指定ピアマッチメイク呼び出し
                if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                    // Xボタンが押されていなければ、マッチメイク用追加キーデータ設定
                    SetGameMatchKeys();
                }

                // 引数friendIdxListにNULLを指定すると
                // 友達リスト内の全員から検索する
                boolRet = 
                    DWC_ConnectToFriendsAsync(NULL,
                                              0,
                                              (u8)stNumEntry,
                                              TRUE,//FALSE,
                                              ConnectToFriendsCallback,
                                              &returnSeq,
                                              EvaluateFriendCallback,
                                              NULL);
                break;
            case 2:  // サーバDSとしてサーバクライアントマッチメイク開始
                boolRet =
                    DWC_SetupGameServer((u8)stNumEntry,
                                        SetupGameServerCallback,
                                        &returnSeq,
                                        NewClientCallback,
                                        NULL);
                break;
            case 3:  // クライアントDSとしてサーバクライアントマッチメイク開始
                boolRet =
                    DWC_ConnectToGameServerAsync(stServerIdx,
                                                 ConnectToGameServerCallback,
                                                 &returnSeq,
                                                 NewClientCallback,
                                                 NULL);
                break;
            case 4:  // ログアウト
                DWC_ShutdownFriendsMatch();   // DWC FriendsMatchライブラリ終了
                break;
            default:
                break;
            }

            if (!boolRet){
                // マッチメイクを開始できなかった場合はブロッキング状態にしない。
                // エラー発生中かログイン前、接続後、二重呼び出し時しか
                // FALSEにはならない
                OS_TPrintf("Can't call matching function.\n");
                returnSeq          = GAME_MODE_NUM;
                stGameCnt.blocking = 0;
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_Y){
            if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                // Xボタンが押されていなければ、
                // Yボタンで現在の友達の状態を取得表示（statusString）
                DispFriendList(TRUE);
                OS_TPrintf("\n");
            }
            else {
                // Xボタンが押されていれば、
                // Yボタンで現在の友達の状態を取得表示（statusData）
                DispFriendListData();
                OS_TPrintf("\n");
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_SELECT){
#ifndef GAME_USE_STORAGE_SERVER  // GameSpyのStatusセットを試す場合
            if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                // Xボタンが押されていなければ、
                // セレクトボタンでGameSpy Status文字列をセット
                if (DWC_SetGsStatusString("SampleString-00")){
                    OS_TPrintf("Set GsStatusString.\n");
                }
                else {
                    OS_TPrintf("Failed to set GsStatusString.\n");
                }
            }
            else {
                // Xボタンが押されていれば、
                // セレクトボタンでGameSpy Statusデータをセット
                if (DWC_SetGsStatusData(gsStatusSample, sizeof(gsStatusSample))){
                    OS_TPrintf("Set GsStatusData.\n");
                }
                else {
                    OS_TPrintf("Failed to set GsStatusData.\n");
                }
            }

#else  // ストレージサーバへのセーブを試す場合
            // セレクトボタンでストレージサーバにテストキーをセーブ
            char keyvalues[12];
            BOOL boolResult;

            if (stKeyCnt.cont & PAD_BUTTON_X){
                // Xボタンが押されていればPrivateデータをセーブ
                sprintf(keyvalues, "\\test_key_prv\\%lld", OS_GetTick());
                boolResult = DWC_SavePrivateDataAsync(keyvalues, NULL);
            }
            else {
                // Xボタンが押されていなければPublicデータをセーブ
                sprintf(keyvalues, "\\test_key_pub\\%lld", OS_GetTick());
                boolResult = DWC_SavePublicDataAsync(keyvalues, NULL);
            }

            if (boolResult){
                OS_TPrintf("Saved '%s'.\n", keyvalues);
            }
            else {
                OS_TPrintf("Can't save to storage server.\n");
            }
#endif
        }
        else if (stKeyCnt.trg & PAD_BUTTON_START){
#ifndef GAME_USE_STORAGE_SERVER  // GameSpyのStatusセットを試す場合
            char statusData[DWC_GS_STATUS_STRING_LEN];
            
            if (!(stKeyCnt.cont & PAD_BUTTON_X)){
                // Xボタンが押されていなければ、
                // スタートボタンでGameSpy Status文字列を取得
                if (DWC_GetGsStatusString(statusData)){
                    OS_TPrintf("Got my GsStatusString : %s\n", statusData);
                }
                else {
                    OS_TPrintf("Failed to get GsStatusString.\n");
                }
            }
            else {
                // Xボタンが押されていれば、
                // スタートボタンでGameSpy Statusデータを取得
                int size, i;

                size = DWC_GetGsStatusData(statusData);
                if (size != -1){
                    OS_TPrintf("Got my GsStatusData\n");
                    for (i = 0; i < size; i++){
                        OS_TPrintf("%02d, ", statusData[i]);
                    }
                    OS_TPrintf("\n");
                }
                else {
                    OS_TPrintf("Failed to get GsStatusData.\n");
                }
            }

#else  // ストレージサーバからのロードを試す場合
            // スタートボタンでストレージサーバからテストデータをロード
            BOOL boolResult;

#if 1
            if (stKeyCnt.cont & PAD_BUTTON_X){
                // Xボタンが押されていれば自分のPrivateデータをロード
                boolResult = DWC_LoadOwnPrivateDataAsync("\\test_key_prv\\test_key_pub", NULL);
            }
            else {
                // Xボタンが押されていなければ自分のPublicデータをロード
                boolResult = DWC_LoadOwnPublicDataAsync("\\test_key_prv\\test_key_pub", NULL);
            }
#else
            // 友達リスト中のホストのデータをロード
            boolResult = DWC_LoadOthersDataAsync("\\test_key", stServerIdx, NULL);
#endif
            if (!boolResult) OS_TPrintf("Can't load from storage server.\n");
#endif
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // 十字キー上でカーソル移動
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // 十字キー下でカーソル移動
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_LEFT){
            // 十字キー左でエントリー人数減少
            if (stNumEntry > 2){
                stNumEntry--;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_RIGHT){
            // 十字キー右でエントリー人数増加
            if (stNumEntry < DWC_MAX_CONNECTIONS){
                stNumEntry++;
                // メニューリスト再表示
                DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_L){
            // LRボタンでサーバクライアントマッチメイクのサーバDSの
            // 友達リストインデックスを選択
            if (curIdx == 3){
                if (stServerIdx > 0){
                    stServerIdx--;
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
                }
            }
        }
        else if (stKeyCnt.trg & PAD_BUTTON_R){
            // LRボタンでサーバクライアントマッチメイクのサーバDSの
            // 友達リストインデックスを選択
            if (curIdx == 3){
                if (stServerIdx < GAME_MAX_FRIEND_LIST-1){
                    stServerIdx++;
                    // メニューリスト再表示
                    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
                }
            }
        }
        ////////// キー操作処理ここまで

        // Vブランク待ち処理
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/** -------------------------------------------------------------------------
  DWC Transport
  ---------------------------------------------------------------------------*/
const int		SIZE_SEND_BUFFER = 512;
const int		SIZE_RECV_BUFFER = 4 * 1024;

static u8		sSendBuffer[ SIZE_SEND_BUFFER ];
static u8		sRecvBuffer[ DWC_MAX_CONNECTIONS-1 ][ SIZE_RECV_BUFFER ];

/** -------------------------------------------------------------------------
  送信完了コールバック  
  ---------------------------------------------------------------------------*/
static void
SendDoneCallback( int size, u8 aid )
{
	OS_TPrintf( "to aid = %d  size = %d\n", aid, size );
}

/** -------------------------------------------------------------------------
  受信完了コールバック  
  ---------------------------------------------------------------------------*/
static void
UserRecvCallback( u8 aid, u8* buffer, int size )
{
	OS_TPrintf( "from aid = %d  size = %d  buffer[0] = %X\n", aid, size, buffer[0] );
}

/** -------------------------------------------------------------------------
  DWC Transport用　受信バッファを設定  
  ---------------------------------------------------------------------------*/
static void
SetRecvBuffer( void )
{
    u8* pAidList;
	int i;
    int j = 0;
	int connectingNum = DWC_GetAIDList(&pAidList);

    OS_TPrintf( "connection host = %d\n", connectingNum );
    
    for ( i = 0; i < connectingNum; ++i )
    {
		if ( pAidList[i] == DWC_GetMyAID() )
        {
            j++;
			continue;
        }

        DWC_SetRecvBuffer( pAidList[i], &sRecvBuffer[i-j], SIZE_RECV_BUFFER );
    }
}

/** -------------------------------------------------------------------------
  Reliabe送信
  ---------------------------------------------------------------------------*/
static void
BroadCastReliable( void )
{
#if 1
    // AIDリストを使って全員送信を行う場合
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID()) continue;

        DWC_SendReliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }

#else
    // AIDビットマップを使って全員送信を行う場合
    u32 bitmap = DWC_SendReliableBitmap( DWC_GetAIDBitmap(), sSendBuffer, SIZE_SEND_BUFFER );
    OS_TPrintf("Sent to %x.\n", bitmap);
#endif
}

/** -------------------------------------------------------------------------
  Unreliable送信
  ---------------------------------------------------------------------------*/
static void
BroadCastUnreliable( void )
{
#if 1
    // AIDリストを使って全員送信を行う場合
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID()) continue;

        DWC_SendUnreliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }

#else
    // AIDビットマップを使って全員送信を行う場合
    u32 bitmap = DWC_SendUnreliableBitmap( DWC_GetAIDBitmap(), sSendBuffer, SIZE_SEND_BUFFER );
    OS_TPrintf("Sent to %x.\n", bitmap);
#endif
}

/*---------------------------------------------------------------------------*
  メッシュ接続完了後メイン関数
 *---------------------------------------------------------------------------*/
static GameMode GameConnectedMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_CONNECTED];
    GameMode returnSeq = GAME_MODE_NUM;
    int closeResult;
    int curIdx = 0;
    
    // 送信コールバックの設定	
    DWC_SetUserSendCallback( SendDoneCallback ); 

    // 受信コールバックの設定	
    DWC_SetUserRecvCallback( UserRecvCallback ); 
    
    // 受信バッファの設定
    // ここでセットするとサーバクライアントマッチメイクの場合に
    // 後から入ってくるクライアントの受信バッファがセットされない！
    //SetRecvBuffer();

    // コネクションクローズコールバックを設定
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);

#if 0
	// パケットロス率を設定
	if ( DWC_GetMyAID() != 0 )
    {
		DWC_SetSendDrop( 30, 0 );
		DWC_SetRecvDrop( 30, 0 );        
    }
#endif
#if 0
    // 遅延を設定（単位: ms）
	if ( DWC_GetMyAID() != 0 )
    {
		DWC_SetSendDelay( 500, 0 );
		DWC_SetRecvDelay( 500, 0 );        
    }
#endif    

    OS_TPrintf("AID bitmap = %x\n", DWC_GetAIDBitmap());
    OS_TPrintf( "my aid = %d\n", DWC_GetMyAID() );

    while (1){        
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;

        ReadKeyData();  // キーデータ取得

        DWC_ProcessFriendsMatch();  // DWC通信処理更新

        HandleDWCError(&returnSeq);  // エラー処理

        if (stGameCnt.blocking){
            // DWC処理中はキー操作を禁止する
            // Vブランク待ち処理
            GameWaitVBlankIntr();
            continue;
	    }

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
        if ((stMatchFinishTime != 0) &&
            (OS_TicksToSeconds(OS_GetTick()-stMatchFinishTime) > GAME_MATCH_OP_SC_BLOCK_TIME)){
            // マッチメイク１件完了後、一定時間経ったら新規接続のブロックを解除する
            DWC_ClearMOSCConnectBlock();

            stMatchFinishTime = 0;
            OS_TPrintf("Clear matching block!.\n");
        }
#endif

        // 次に進むべきモードがセットされていたらループを抜ける
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// 以下キー操作処理
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // Aボタンでメニュー決定
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (curIdx){
            case 0:             // Reliable送信
                BroadCastReliable();
				break;
            case 1:             // Unreliable送信
				BroadCastUnreliable();
                break;
            case 2:             // コネクションクローズ
                stCloseFlag = TRUE;
                closeResult = DWC_CloseAllConnectionsHard();
                break;
            default:
                break;
            }
        }
#ifdef DWC_STOP_SC_SERVER
        else if (stKeyCnt.trg & PAD_BUTTON_START){
            // スタートボタンでサーバクライアントマッチメイクを締め切る
            BOOL boolResult = DWC_StopSCMatchingAsync(StopSCMatchingCallback, NULL);
            OS_TPrintf("DWC_StopSCMatchingAsync : result %d\n", boolResult);
        }
#endif
        else if (stKeyCnt.trg & PAD_BUTTON_SELECT){
            // セレクトボタンでaid = 1のホストを強制切断する。
            // 通信中に電源を切ったホストを切断する時のテスト
            u32 bitmap = 0x02;
#if 1
            // AID指定版
            closeResult = DWC_CloseConnectionHard(1);
#else
            // AIDビットマップ指定版
            closeResult = DWC_CloseConnectionHardBitmap(&bitmap);
#endif

            OS_TPrintf("Closed connection to aid = 1, result = %d, bitmap = %x\n",
                       closeResult, bitmap);

            // 結果に関わらず処理を続けるのでブロッキングしない。
            // また、クローズコールバックはCloseConnectionHard関数内で呼ばれる。
            stGameCnt.blocking = FALSE;
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // 十字キー上でカーソル移動
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // 十字キー下でカーソル移動
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        ////////// キー操作処理ここまで

        // Vブランク待ち処理
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  友達リストの空きインデックス取得関数
 *---------------------------------------------------------------------------*/
static int GetAvailableFriendListIndex(void)
{
    int i;

    for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
        if (!DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i]))
            return i;
    }

    return -1;
}


/*---------------------------------------------------------------------------*
  ログインコールバック関数
 *---------------------------------------------------------------------------*/
static void LoginCallback(DWCError error, int profileID, void *param)
{
    BOOL result;
	RTCTime time;
	RTCDate date;

	if (error == DWC_ERROR_NONE){
        // 認証成功、プロファイルID取得
        OS_TPrintf("Login succeeded. profileID = %u\n\n", profileID);

        // ingamesnチェックの結果を取得する
		if(DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID)
			OS_TPrintf("BAD ingamesn is detected by NAS.\n");
		else if(DWC_GetIngamesnCheckResult() == DWC_INGAMESN_VALID)
			OS_TPrintf("GOOD ingamesn is detected by NAS.\n");
		else
			OS_TPrintf("ingamesn is not checked yet.\n");
		
		// 認証サーバの時刻を表示する
		if(DWC_GetDateTime(&date, &time) == TRUE)
			OS_TPrintf("NasTime = %02d/%02d/%02d %02d:%02d:%02d\n", date.year+2000, date.month, date.day, time.hour, time.minute, time.second);
		else
			DWC_Printf(DWC_REPORTFLAG_AUTH, "DWC_GetNasTime failed\n");


        // stUserDataが更新されているかどうかを確認。
        if ( DWC_CheckDirtyFlag( &stUserData ) )
        {
            DWC_ClearDirtyFlag( &stUserData );

            OS_TPrintf("*******************************\n");
            OS_TPrintf("You must save the DWCUserData!!\n");
            OS_TPrintf("*******************************\n");

            // 必ずこのタイミングでチェックして、dirty flagが有効になっていたら、
            // DWCUserDataをDSカードのバックアップに保存するようにしてください。
            // saveUserDataToDSCard( &stUserData );
        }

        stGameCnt.userData.profileID = profileID;
        stGameCnt.userData.isValidProfile = TRUE;

        // GameSpy Status文字列セットテスト
        //DWC_SetGsStatusData(gsStatusSample, sizeof(gsStatusSample));
        

        // 友達リスト同期処理開始。
        result = DWC_UpdateServersAsync(NULL,//stGameCnt.userData.playerName,
                                        UpdateServersCallback, param,
                                        FriendStatusCallback, param,
                                        DeleteFriendListCallback, param);
        if (result == FALSE){
            // アップデートを開始できなかった場合はブロッキング状態を解除する。
            // エラー発生中かログイン前、二重呼び出し時しかFALSEにはならない。
            OS_TPrintf("Can't call DWC_UpdateServersAsync().\n");
             *(GameMode *)param = GAME_MODE_NUM;
            stGameCnt.blocking  = 0;
        }
        else {
            // GameSpyサーバ上バディ成立コールバックを登録する
            DWC_SetBuddyFriendCallback(BuddyFriendCallback, NULL);

            // ストレージサーバセーブ・ロード完了通知コールバックを登録する
            DWC_SetStorageServerCallback(SaveToServerCallback,
                                         LoadFromServerCallback);
        }
    }
    else {
        // 認証失敗
        OS_TPrintf("Login failed. %d\n\n", error);

        stGameCnt.blocking = FALSE;  // ブロッキング解除
    }
}


/*---------------------------------------------------------------------------*
  友達リスト同期処理完了コールバック関数
 *---------------------------------------------------------------------------*/
static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param)
{
#pragma unused(param)

    if (error == DWC_ERROR_NONE){
        // 友達リスト同期処理完了
        OS_TPrintf("Updating servers succeeded.\n");

        if (isChanged){
            // 友達リストが変更されていたらセーブする
            // （セーブのつもり）
            MI_CpuCopy32(&stGameCnt.friendList, &saveData.friendList, sizeof(GameFriendList));
        }

        // 友達リストを表示する
        DispFriendList(TRUE);
    }
    else {
        // 失敗しても特に何もしない
        OS_TPrintf("Failed to update GameSpy servers. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // ブロッキング解除
}


/*---------------------------------------------------------------------------*
  友達状態変化通知コールバック関数
 *---------------------------------------------------------------------------*/
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param)
{
#pragma unused(param)

    OS_TPrintf("Friend[%d] changed status -> %d (statusString : %s).\n",
               index, status, statusString);
}


/*---------------------------------------------------------------------------*
  友達リスト削除コールバック関数
 *---------------------------------------------------------------------------*/
static void DeleteFriendListCallback(int deletedIndex, int srcIndex, void* param)
{
#pragma unused(param)

    OS_TPrintf("friend[%d] was deleted (equal friend[%d]).\n",
               deletedIndex, srcIndex);
}


/*---------------------------------------------------------------------------*
  GameSpyバディ成立コールバック関数
 *---------------------------------------------------------------------------*/
static void BuddyFriendCallback(int index, void* param)
{
#pragma unused(param)

    OS_TPrintf("I was authorized by friend[%d].\n", index);
}


/*---------------------------------------------------------------------------*
  ストレージサーバへのログイン完了コールバック関数
 *---------------------------------------------------------------------------*/
static void LoginToStorageCallback(DWCError error, void* param)
{
#pragma unused(param)

    OS_TPrintf("Login to storage server: result %d\n", error);
}


/*---------------------------------------------------------------------------*
  ストレージサーバへのデータセーブ完了コールバック関数
 *---------------------------------------------------------------------------*/
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param)
{

    OS_TPrintf("Saved data to server. %08x\n", (u32)param);
    OS_TPrintf("result %d, isPublic %d.\n", success, isPublic);
}


/*---------------------------------------------------------------------------*
  ストレージサーバからのデータロード完了コールバック関数
 *---------------------------------------------------------------------------*/
static void LoadFromServerCallback(BOOL success, int index, char* data, int len, void* param)
{

    OS_TPrintf("Loaded data from server. %08x\n", (u32)(param));
    OS_TPrintf("result %d, index %d, data '%s', len %d\n",
               success, index, data, len);
}


/*---------------------------------------------------------------------------*
  友達無指定接続コールバック関数
 *---------------------------------------------------------------------------*/
static void ConnectToAnybodyCallback(DWCError error, BOOL cancel, void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            // 見知らぬ人たちとのコネクション設立完了
            OS_TPrintf("Succeeded to connect to anybody\n\n");

            // 受信バッファセット
            SetRecvBuffer();
        }
        else {
            OS_TPrintf("Canceled matching.\n");
            // ログイン後状態に戻る
            *(GameMode *)param = GAME_MODE_LOGIN;
        }
    }
    else {
        OS_TPrintf("Failed to connect to anybody. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // ブロッキング解除
}


/*---------------------------------------------------------------------------*
  友達指定接続コールバック関数
 *---------------------------------------------------------------------------*/
static void ConnectToFriendsCallback(DWCError error, BOOL cancel, void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            // 友達とのコネクション設立完了
            OS_TPrintf("Succeeded to connect to friends\n\n");

            // 受信バッファセット
            SetRecvBuffer();
        }
        else {
            OS_TPrintf("Canceled matching.\n");
            // ログイン後状態に戻る
            *(GameMode *)param = GAME_MODE_LOGIN;
        }
    }
    else {
        OS_TPrintf("Failed to connect to friends. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // ブロッキング解除
}


/*---------------------------------------------------------------------------*
  サーバクライアントマッチメイク・サーバ起動コールバック関数
 *---------------------------------------------------------------------------*/
static void SetupGameServerCallback(DWCError error,
                                    BOOL cancel,
                                    BOOL self,
                                    BOOL isServer,
                                    int  index,
                                    void* param)
{
#pragma unused(isServer)

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            // ネットワークに新規クライアントが加わった
            OS_TPrintf("A friend of mine joined the game.\n");
            OS_TPrintf("friendListIndex = %d.\n\n", index);

            // 受信バッファセット
            SetRecvBuffer();

#ifdef GAME_MATCH_OP_SC_CONNECT_BLOCK
            // マッチメイク１件完了時の時間を記録
            stMatchFinishTime = OS_GetTick();
#endif
        }
        else {
            if (self){
                // 自分がマッチメイクをキャンセルした
                OS_TPrintf("Canceled matching.\n\n");
                // ログイン後状態に戻る
                // paramの参照先はauto変数returnSeqだが、自分がマッチメイクを
                // キャンセルできる時はスコープ内
                *(GameMode *)param = GAME_MODE_LOGIN;
            }
            else {
                // クライアントDSがマッチメイクをキャンセルした
                OS_TPrintf("Client (friendListIndex = %d) canceled matching.\n\n", index);

                if (DWC_GetNumConnectionHost() == 1){
                    // まだ誰とも接続していなければブロッキングを解除しない。
                    // 一度でもマッチメイクが完了した後に接続数が1になった場合は
                    // 元々ブロッキング解除状態なので問題ない。
                    return;
                }
            }
        }
    }
    else {
        // エラーが発生した
        OS_TPrintf("Game server error occured. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // ブロッキング解除
}


/*---------------------------------------------------------------------------*
  サーバクライアントマッチメイク・サーバ接続コールバック関数
 *---------------------------------------------------------------------------*/
static void ConnectToGameServerCallback(DWCError error,
                                        BOOL cancel,
                                        BOOL self,
                                        BOOL isServer,
                                        int  index,
                                        void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!cancel){
            if (self){
                // 自分がサーバDSとそこにできているネットワークへの接続に
                // 成功した場合
                OS_TPrintf("Succeeded to join the game.\n\n");
            }
            else {
                // ネットワークに新規クライアントが加わった場合
                OS_TPrintf("New client joined the game.\n");
                if (index != -1){
                    OS_TPrintf("he is my friend [%d].\n\n", index);
                }
                else {
                    OS_TPrintf("he is not my friend.\n\n");
                }
            }

            // 受信バッファセット
            SetRecvBuffer();
        }
        else {
            if (self){
                // 自分がマッチメイクをキャンセルした
                OS_TPrintf("Canceled matching.\n\n");
                // ログイン後状態に戻る
                *(GameMode *)param = GAME_MODE_LOGIN;
            }
            else {
                if (isServer){
                    // サーバDSがマッチメイクをキャンセルした
                    OS_TPrintf("GameServer canceled matching.\n\n");
                    // マッチメイクを終了してログイン後状態に戻る
                    *(GameMode *)param = GAME_MODE_LOGIN;
                }
                else {
                    // 他のクライアントDSがマッチメイクをキャンセルした
                    OS_TPrintf("Host (friendListIndex = %d) canceled matching.\n\n", index);
                }
            }
        }
    }
    else {
        // エラーが発生した
        OS_TPrintf("Failed to join the game. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // ブロッキング解除
}


/*---------------------------------------------------------------------------*
  コネクションクローズコールバック関数
 *---------------------------------------------------------------------------*/
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8  aid,
                                     int index,
                                     void* param)
{

    if (error == DWC_ERROR_NONE){
        if (!isLocal && isServer){
            // サーバクライアントマッチメイクで、自分がクライアントの時に、
            // サーバDSがコネクションをクローズした。
            // サーバDSがクローズしたので自分もクローズする。
            stCloseFlag = TRUE;
            DWC_CloseAllConnectionsHard();
        }
        
        if (DWC_GetNumConnectionHost() == 1){
            // 全てのコネクションがクローズされた場合
            if ((DWC_GetLastMatchingType() != DWC_MATCH_TYPE_SC_SV) || stCloseFlag){
                // サーバクライアントマッチメイクのサーバDSが自分でクローズ関数を
                // 呼んでクローズした場合か、その他のマッチメイクタイプの場合は、
                // ここで次のモードをセットする
                *(GameMode *)param = GAME_MODE_LOGIN;
                stCloseFlag = FALSE;

                // 次のマッチメイクに備えて自ホストデータをGameSpyサーバに登録する
                // サーバクライアントマッチメイクではこの関数を呼んでも何も起らない
                DWC_RegisterMatchingStatus();
            }
            // 自分がサーバクライアントマッチメイクのサーバDSで、
            // クライアントが全員クローズした場合は何もしない
        }

        if (isLocal){
            OS_TPrintf("Closed connection to aid %d (friendListIndex = %d) Rest %d.\n",
                       aid, index, DWC_GetNumConnectionHost());
        }
        else {
            OS_TPrintf("Connection to aid %d (friendListIndex = %d) was closed. Rest %d.\n",
                       aid, index, DWC_GetNumConnectionHost());
        }
    }
    else {
        OS_TPrintf("Failed to close connections. %d\n\n", error);
    }
}


/*---------------------------------------------------------------------------*
  サーバクライアントマッチメイク時の新規接続クライアント通知コールバック関数
 *---------------------------------------------------------------------------*/
static void NewClientCallback(int index, void* param)
{
#pragma unused(param)

    // 新規接続クライアントの接続処理が終わるまでは、
    // コントローラ操作を禁止する
    // →実際のゲームの処理に即してブロッキングしないようにする
    //stGameCnt.blocking = TRUE;
    
    OS_TPrintf("New client started to connect.\n");
    if (index != -1){
        OS_TPrintf("He is my friend [%d].\n", index);
    }
    else {
        OS_TPrintf("He is not my friend.\n");
    }
}


/*---------------------------------------------------------------------------*
  友達無指定ピアマッチメイク時プレイヤー評価コールバック関数
 *---------------------------------------------------------------------------*/
static int  EvaluateAnybodyCallback(int index, void* param)
{
#pragma unused(param)
    const char* matchType = DWC_GetMatchStringValue(index, stMatchKeys[1].keyStr, "NONE");
    int rank;
    int diff;
#if 0  // 評価のテストを行いたい場合はここを1にする
    int full = 200;
#else  // 普段はマッチメイクしないことがあると面倒なのでこちらにする
    int full = 300;
#endif
    
    if (!strncmp(matchType, "eval_rank", strlen("eval_rank"))){
        // 相手もマッチメイクタイプとして"eval_rank"をセットしている場合
        rank = DWC_GetMatchIntValue(index, stMatchKeys[0].keyStr, -1);
        if (rank == -1){
            return 0;  // "game_rank"キーがなければマッチメイクしない
        }

        diff = (stMatchKeys[0].ivalue > rank) ? stMatchKeys[0].ivalue-rank : rank-stMatchKeys[0].ivalue;

        OS_TPrintf("Player[%d]'s rank is %d (mine %d). %d point.\n",
                   index, rank, stMatchKeys[0].ivalue, full-diff);

        // 自分との順位差が近いほど高得点になるように、200点満点で点を付ける。
        // 200位差以上ならマッチメイクしない（0以下の値を返す）
        return full-diff;
    }
    else if (!strncmp(matchType, "NONE", strlen("NONE"))){
        return 1;  // "game_mtype"キーがなければとりあえず平等にマッチメイクする
    }

    return 1;
}


/*---------------------------------------------------------------------------*
  友達指定ピアマッチメイク時プレイヤー評価コールバック関数
 *---------------------------------------------------------------------------*/
static int  EvaluateFriendCallback(int index, void* param)
{
#pragma unused(param)
    const char* matchType = DWC_GetMatchStringValue(index, stMatchKeys[1].keyStr, "NONE");
    
    if (!strncmp(matchType, "eval_rank", strlen("eval_rank"))){
        // 相手もマッチメイクタイプとして"eval_rank"をセットしている場合
        return 1;  // マッチメイクOK
    }
    else {
        return 0;  // "game_mtype"キーがなければマッチメイクしない
    }
}


#ifdef DWC_STOP_SC_SERVER
/*---------------------------------------------------------------------------*
  サーバクライアントマッチメイク締め切り完了コールバック関数
 *---------------------------------------------------------------------------*/
static void StopSCMatchingCallback(void* param)
{
#pragma unused(param)

    OS_TPrintf("StopSCMatchingCallback was called.\n");
}
#endif


/*---------------------------------------------------------------------------*
  DWCライブラリエラー処理関数
 *---------------------------------------------------------------------------*/
static DWCError HandleDWCError(GameMode* gameMode)
{
    int errorCode;
    DWCError error;
    DWCErrorType errorType;

    // DWCエラー取得
    error = DWC_GetLastErrorEx(&errorCode, &errorType);

    // エラーがなければすぐに戻る
    if (error == DWC_ERROR_NONE) return DWC_ERROR_NONE;

    // エラーに対応したメッセージを表示
    DispErrorMessage(error, errorCode, errorType);

    DWC_ClearError();        // エラーをクリア
    stGameCnt.blocking = 0;  // ブロッキング解除

    switch (errorType){
    case DWC_ETYPE_LIGHT:
        // 今のところマッチメイク中にしか出ないエラータイプなので、
        // 必ずログイン後状態に戻る
        *gameMode = GAME_MODE_LOGIN;
        break;

    case DWC_ETYPE_SHUTDOWN_FM:
        // FriendsMatch処理を終了し、GameSpyのヒープを解放する
        DWC_ShutdownFriendsMatch();

        // ログイン前の状態に戻る
        *gameMode = GAME_MODE_MAIN;
        break;

    case DWC_ETYPE_DISCONNECT:
        // FriendsMatch処理を終了し、GameSpyのヒープを解放する
        DWC_ShutdownFriendsMatch();

        // インターネット接続を完全に初期化する
        DWC_CleanupInet();

        // ログイン前の状態に戻る
        *gameMode = GAME_MODE_MAIN;
        break;

    case DWC_ETYPE_FATAL:
        // Fatal Error なので停止する
        OS_Panic("========== Stop process ==========\n");
        break;
    }

    return error;
}

/*---------------------------------------------------------------------------*
  通信エラー表示関数
 *---------------------------------------------------------------------------*/
static void DispErrorMessage(DWCError error, int errorCode, DWCErrorType errorType)
{

    if (error == DWC_ERROR_NONE) return;

    OS_TPrintf("ERROR: %d, code:%d, type:%d\n", error, -1*errorCode, errorType);
    
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
    case DWC_ERROR_DISCONNECTED:
        OS_TPrintf("ERROR: Connection to AP was lost.\n");
        break;
    case DWC_ERROR_FATAL:
        OS_TPrintf("FATAL ERROR: Please turn the power off.\n");
        break;
    case DWC_ERROR_FRIENDS_SHORTAGE:
        OS_TPrintf("ERROR: Your friends seem not to login yet.\n");
        break;
    case DWC_ERROR_NOT_FRIEND_SERVER:
        OS_TPrintf("ERROR: He is not buddy or not game server.\n");
        break;
    case DWC_ERROR_MO_SC_CONNECT_BLOCK:
        OS_TPrintf("ERROR: Game server is busy now.\n");
        break;
    case DWC_ERROR_SERVER_FULL:
        OS_TPrintf("ERROR: Game server is fully occupied.\n");
        break;
    default:
        OS_TPrintf("ERROR: Undefined yet.\n");
        break;
    }
}


/*---------------------------------------------------------------------------*
  メッセージリスト表示（カーソル付き）関数
 *---------------------------------------------------------------------------*/
static void DispMenuMsgWithCursor(const GameSequence* gameSeq, int index, void* param)
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
    OS_TPrintf("+++VALID CONSOLE %d +++\n", DWC_CheckValidConsole( &stUserData ) );
    Heap_Print();
}


/*---------------------------------------------------------------------------*
  友達リスト表示関数（GameSpy Status文字列表示版）
 *---------------------------------------------------------------------------*/
static void DispFriendList(BOOL overLoad)
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
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, server (%d/%d), statusString : %s\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i],
                       numEntry, maxEntry, statusString);
        }
        else {
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, statusString : %s\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i], statusString);
        }
    }
}


/*---------------------------------------------------------------------------*
  友達リスト表示関数（GameSpy Statusデータ表示版）
 *---------------------------------------------------------------------------*/
static void DispFriendListData(void)
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
            DWC_GetFriendStatusDataSC(&stGameCnt.friendList.keyList[i],
                                      &maxEntry, &numEntry, statusData, &size);
        
        if (maxEntry){
            // 友達がサーバクライアントマッチメイクのサーバDSの場合は、
            // （現在接続人数／最大接続人数）を表示する
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d, server (%d/%d)\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i],
                       numEntry, maxEntry);
        }
        else {
            OS_TPrintf("Friend[%d] '%s' : profileID %u, status %d\n",
                       i, stGameCnt.friendList.playerName[i],
                       DWC_GetGsProfileId(&stUserData, &stGameCnt.friendList.keyList[i]),
                       stGameCnt.friendStatus[i]);
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
  友達登録・消去モードの追加表示用コールバック関数
 *---------------------------------------------------------------------------*/
static void RegFriendModeDispCallback(int curIdx, void* param)
{
    RegFriendParam cntParam;
    int i;

    if (!param){
        // 初回はparam = NULLで呼ばれる
        cntParam.step = 0;
    }
    else {
        cntParam = *(RegFriendParam *)param;
    }

    OS_TPrintf("\n");

    if (cntParam.step == 0){
        // コマンド選択中は友達リストを表示
        DispFriendList(FALSE);
    }
    else {
        // 数値入力中
        if (curIdx == 0){
            // 友達登録。プロファイルID入力中
            OS_TPrintf("Set profileID to friend list : %010u\n", cntParam.value);

            // 選択中の数字を示す為の下線を表示する
            for (i = 0; i < 9-cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("                               -\n");
        }
        else {
            // 友達削除。友達リストインデックス入力中
            DispFriendList(FALSE);  // 友達リストを表示
            
            OS_TPrintf("Delete index : %02u\n", cntParam.value);

            for (i = 0; i < 1-cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("               -\n");
        }
    }
}


/*---------------------------------------------------------------------------*
  ログイン後モードの追加表示用コールバック関数
 *---------------------------------------------------------------------------*/
static void LogonModeDispCallback(int curIdx, void* param)
{
#pragma unused(param)

    // 自分以外のエントリー人数へのポインタを表示する
    OS_TPrintf("\nnumber of entry = ");
    if (stNumEntry == 2){
        OS_TPrintf("   %d >>\n", stNumEntry);
    }
    else if (stNumEntry == DWC_MAX_CONNECTIONS){
        OS_TPrintf("<< %d\n", stNumEntry);
    }
    else {
        OS_TPrintf("<< %d >>\n", stNumEntry);
    }

    if (curIdx == 3){
        // サーバクライアントマッチメイクのサーバDSへの接続時は、
        // 友達リストインデックスも表示する
        OS_TPrintf("GameServerIndex = ");
        if (DWC_GetNumFriend(stGameCnt.friendList.keyList, GAME_MAX_FRIEND_LIST) == 1){
            OS_TPrintf("   %d\n", stServerIdx);
        }
        else if (stServerIdx == 0){
            OS_TPrintf("   %d >>\n", stServerIdx);
        }
        else if (stServerIdx == GAME_MAX_FRIEND_LIST-1){
            OS_TPrintf("<< %d\n", stServerIdx);
        }
        else {
            OS_TPrintf("<< %d >>\n", stServerIdx);
        }
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
  Wi-Fiコネクション設定の使用言語選択
 *---------------------------------------------------------------------------*/
static GameMode NetConfigLanguageMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_NETCONFIG_LANGUAGE];
    int curIdx = 0;
    const int languageTable[] = {
        DWC_LANGUAGE_JAPANESE,
        DWC_LANGUAGE_ENGLISH,
        DWC_LANGUAGE_FRENCH,
        DWC_LANGUAGE_GERMAN,
        DWC_LANGUAGE_ITALIAN,
        DWC_LANGUAGE_SPANISH,
        DWC_LANGUAGE_HANGUL
    };

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState());
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost());
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel());
        dbs_Print(10, 1, "p:%d", stGameCnt.userData.profileID);
        dbs_Print(30, 0, "%c",  loopstr[((loopcnt++) >> 2) & 3]);
        ReadKeyData();
        if (stKeyCnt.trg & PAD_BUTTON_A){
            utilityLangage = languageTable[curIdx];
            break;
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu - 1;
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        GameWaitVBlankIntr();
    }

    return gameSeq->menuList[curIdx].mode;
}


/*---------------------------------------------------------------------------*
  Wi-Fiコネクション設定
 *---------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static GameMode NetConfigMain(void)
{
    GameMode returnSeq = GAME_MODE_NETCONFIG_LANGUAGE;

    (void)FS_LoadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    DWC_CleanupInet(); // ネットワークから切断する。
    sPrintOverride = FALSE; // OS_TPrintf()の出力を一時的に元に戻す。
    dbs_DemoFinalize();

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

#if defined( NETCONFIG_USE_HEAP )
    {
        void* work = OS_Alloc(DWC_UTILITY_WORK_SIZE);

        (void)DWC_StartUtility(
            work,
            utilityLangage,
            utilityLangage == DWC_LANGUAGE_JAPANESE ? DWC_UTILITY_TOP_MENU_FOR_JPN : DWC_UTILITY_TOP_MENU_COMMON
        );

        OS_Free(work);
    }
#else
    (void)DWC_StartUtilityEx(
        utilityLangage,
        utilityLangage == DWC_LANGUAGE_JAPANESE ? DWC_UTILITY_TOP_MENU_FOR_JPN : DWC_UTILITY_TOP_MENU_COMMON
    );
#endif

    (void)FS_UnloadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr( TRUE );

    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    return returnSeq;
}

static GameMode StartIPMain(void)
{
	DWCApInfo apinfo;
	
    DWC_InitInet( &stConnCtrl );

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

    
    DWC_ConnectInetAsync();
    // 安定なステートまで待つ。
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        // Vブランク待ち処理
        GameWaitVBlankIntr();
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
		if(DWC_GetApInfo(&apinfo) == TRUE) {
			OS_TPrintf("DWCApInfo.aptype = %d\n", apinfo.aptype);
			OS_TPrintf("DWCApInfo.area   = %d\n", apinfo.area);
			OS_TPrintf("DWCApInfo.spotinfo = %s\n", apinfo.spotinfo);
			OS_TPrintf("DWCApInfo.essid = %s\n", apinfo.essid);
			OS_TPrintf("DWCApInfo.bssid = %02x:%02x:%02x:%02x:%02x:%02x\n", apinfo.bssid[0], apinfo.bssid[1], apinfo.bssid[2], apinfo.bssid[3], apinfo.bssid[4], apinfo.bssid[5]);
		}
		else {
			OS_TPrintf("Failed to get DWCApInfo\n");
		}
			
        break;
    }

    return GAME_MODE_MAIN;
}

static GameMode StopIPMain(void)
{
    DWC_CleanupInet();

    return GAME_MODE_MAIN;
}

static GameMode BackupUserdataMain( void )
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_BACKUP];
    int curIdx = 0;
    GameMode returnSeq = GAME_MODE_NUM;

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
	    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
		loopcnt++;

        ReadKeyData();  // キーデータ取得

        HandleDWCError(&returnSeq);  // エラー処理

        // 次に進むべきモードがセットされていたらループを抜ける
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// 以下キー操作処理
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // Aボタンでメニュー決定
            BOOL end = FALSE;
            BOOL error = FALSE;
            int last_result;
            CARD_LockBackup((u16)card_lock_id);
            end = CARD_IdentifyBackup(stBackupTypeTable[stNumBackup].type);
            if(!end)
            {
                error = TRUE;
                last_result = CARD_GetResultCode();
            }
            CARD_UnlockBackup(card_lock_id);


            switch (curIdx){
            case 0:  // Load
                if ( error ) {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                CARD_LockBackup((u16)card_lock_id);
                if (CARD_IsBackupEeprom())
                {
                    CARD_ReadEeprom(0, &stUserData, sizeof(stUserData) );
					CARD_ReadEeprom(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFlash())
                {
                    CARD_ReadFlash(0, &stUserData, sizeof(stUserData) );
					CARD_ReadFlash(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFram())
                {
                    CARD_ReadFram(0, &stUserData, sizeof(stUserData) );
					CARD_ReadFram(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                CARD_UnlockBackup(card_lock_id);
                last_result = CARD_GetResultCode();

                if (last_result != CARD_RESULT_SUCCESS)
                {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                else
                {
                    OS_TPrintf("Load OK!\n");
                    returnSeq = GAME_MODE_MAIN;
                }
                break;
            case 1:  // Save
                if ( error ) {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                CARD_LockBackup((u16)card_lock_id);
                if (CARD_IsBackupEeprom())
                {
                    CARD_WriteAndVerifyEeprom(0, &stUserData, sizeof(stUserData) );
					CARD_WriteAndVerifyEeprom(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFlash())
                {
                    CARD_WriteAndVerifyFlash(0, &stUserData, sizeof(stUserData) );
					CARD_WriteAndVerifyFlash(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                else if (CARD_IsBackupFram())
                {
                    CARD_WriteAndVerifyFram(0, &stUserData, sizeof(stUserData) );
					CARD_WriteAndVerifyFram(sizeof(stUserData), &stGameCnt.friendList, sizeof(GameFriendList));
                }
                CARD_UnlockBackup(card_lock_id);
                last_result = CARD_GetResultCode();

                if (last_result != CARD_RESULT_SUCCESS)
                {
                    OS_TPrintf("Error occur with %d\n", last_result);
                    continue;
                }
                else
                {
                    OS_TPrintf("Save OK!\n");
                    returnSeq = GAME_MODE_MAIN;
                }
                break;
            case 2: // report
                DWC_ReportUserData( &stUserData );
                break;
            case 3:
                returnSeq = GAME_MODE_MAIN;
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // 十字キー上でカーソル移動
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
            // 十字キー下でカーソル移動
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // メニューリスト再表示
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_LEFT){
            // 十字キー左でエントリー人数減少
            stNumBackup--;
            stNumBackup %= BACKUP_TABLE_MAX;

            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_RIGHT){
            // 十字キー右でエントリー人数増加
            stNumBackup++;
            stNumBackup %= BACKUP_TABLE_MAX;

            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        ////////// キー操作処理ここまで

        // Vブランク待ち処理
        GameWaitVBlankIntr();
    }

    return returnSeq;
}


/*---------------------------------------------------------------------------*
  ログイン後モードの追加表示用コールバック関数
 *---------------------------------------------------------------------------*/
static void BackupModeDispCallback(int curIdx, void* param)
{
#pragma unused(param)
#pragma unused(curIdx)
    OS_TPrintf("\n BACKUP: %s\n", stBackupTypeTable[stNumBackup].comment);
}


/*---------------------------------------------------------------------------*
  Vブランク待ち関数
 *---------------------------------------------------------------------------*/
static void GameWaitVBlankIntr(void)
{
    int i;

    dbs_DemoUpdate();

    // 想定するゲームフレームに合わせてVブランク待ちの回数を変える
    for (i = 0; i < GAME_FRAME; i++){
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    }

    Heap_Debug();
    dbs_DemoLoad();

    // スタック溢れチェック
    OS_CheckStack(OS_GetCurrentThread());
}


/*---------------------------------------------------------------------------*
  Vブランク割り込み関数
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
