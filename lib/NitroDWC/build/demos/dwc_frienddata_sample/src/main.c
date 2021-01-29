#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "userdata.h"
#include "screen.h"

#include "dbs.h"

/** --------------------------------------------------------------------
  defines
  ----------------------------------------------------------------------*/
#define GAME_NAME        "dwctest" // このサンプルが使用するゲーム名
#define GAME_SECRET_KEY  "d4q9GZ"  // このサンプルが使用するシークレットキー
#define GAME_PRODUCTID   10824      // このサンプルが使用するプロダクトID

/** --------------------------------------------------------------------
  globals
  ----------------------------------------------------------------------*/
KeyControl		g_KeyCtrl;

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
static DWCFriendsMatchControl s_FMCtrl;
static DWCInetControl		  s_ConnCtrl;
static char stFriendKey[sizeof(DWCFriendData)]; // 友達登録鍵
static int  s_state  = state_init;
static BOOL s_logined;
static BOOL s_canceled;

static int  s_profileID = 0;
static u64  s_friendkey = 0;

/** --------------------------------------------------------------------
  static functions
  ----------------------------------------------------------------------*/
static int update_friend( void );
static int update_init( void );
static int update_connect( void );
static int update_login( void );
static int update_online( void );
static int update_initFriendKey( void );
static int update_changeFriendKey( void );
static int update_initFriendData( void );
static int update_inputFriendIndex( void );
static int update_deleteFriendData( void );
static int update_disconnect( void );
static int update_save( void );
static int update_error( void );

static void cb_login( DWCError error, int profileID, void* param );
static void cb_updateServers(DWCError error, BOOL isChanged, void* param);
static void cb_close( DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param );
static int  InputNumber( u8 maxFigure );
static void DispMenuMsg( const u8 figure, const u8 maxFigure );
static void DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l );

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


/** --------------------------------------------------------------------
  初期化処理
  ----------------------------------------------------------------------*/
static void
initFunctions( void )
{
	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    CARD_Init();
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    PXI_Init();
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

    Heap_Init();
    Heap_SetDebug( TRUE );

    // 出力初期化
    InitScreen();
    dbs_DemoInit();
    DTDB_SetPrintOverride( TRUE );

    // キー入力初期化
    InitKeyData( &g_KeyCtrl );

    // 表示開始
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();
}

/** --------------------------------------------------------------------
  
  ----------------------------------------------------------------------*/

/** --------------------------------------------------------------------
  1フレーム1回呼ばれる関数
  ----------------------------------------------------------------------*/
static void
update( void )
{
    int errcode;
    DWCError err;
    DWCErrorType errtype;

    switch ( s_state )
    {
    case state_init:			 s_state = update_init();			 break;
    case state_connect:			 s_state = update_connect();		 break;
    case state_login:			 s_state = update_login();			 break;
    case state_online:			 s_state = update_online();			 break;
    case state_initFriendKey:    s_state = update_initFriendKey();   break;
    case state_changeFriendKey:  s_state = update_changeFriendKey(); break;
    case state_initFriendData:   s_state = update_initFriendData();  break;
    case state_inputFriendIndex: s_state = update_inputFriendIndex();break;
    case state_deleteFriendData: s_state = update_deleteFriendData();break;
    case state_disconnect:		 s_state = update_disconnect();		 break;
    case state_save:             s_state = update_save();            break;
    case state_error:			 s_state = update_error();			 break;
    }

    err = DWC_GetLastErrorEx(&errcode, &errtype);

    if ( err != DWC_ERROR_NONE && s_state != state_error )
    {
        // エラーが発生した場合、ここでは状態をエラー解除待ちにするだけ
        OS_TPrintf( "\n*******************************\n" );
        OS_TPrintf( "ERROR : %d : %d\n", err, errcode );
        OS_TPrintf( "Push START BUTTON to continue\n" );
        OS_TPrintf( "*******************************\n\n" );
        s_state = state_error;
    }
}

/** --------------------------------------------------------------------
  初期状態
  ----------------------------------------------------------------------*/
static int
update_init( void )
{
    // DS本体とDSカードのユーザデータが一致するか
    if ( DWC_CheckValidConsole( DTUD_GetUserData() ) )
    {
        DispMessage( "STATE : CONNECTING", NULL, NULL, NULL, NULL, NULL );

        DWC_InitInet( &s_ConnCtrl );
        
        DWC_SetAuthServer( DWC_CONNECTINET_AUTH_RELEASE );
        
        DWC_ConnectInetAsync();

        // 友達登録鍵を取得
        s_friendkey = DWC_CreateFriendKey( DTUD_GetUserData() );
        
        return state_connect;
    }
    else
    {
        // 一致していない場合はインターネットに接続できない
        OS_TPrintf( "UserData is not Valid Console.\n" );
        return state_init;
    }

    return state_init;
}

/** --------------------------------------------------------------------
  接続中
  ----------------------------------------------------------------------*/
static int
update_connect( void )
{
    DWC_ProcessInet();

    if ( DWC_CheckInet() )
    {
        int status;

        status = DWC_GetLastError(NULL);

        if ( status == DWC_ERROR_NONE )
        {
            OS_TPrintf("Connected!..start FriendsMatch.\n");

            s_logined = FALSE;

            DWC_InitFriendsMatch(&s_FMCtrl, DTUD_GetUserData(),
                                 GAME_PRODUCTID, GAME_NAME, GAME_SECRET_KEY,
                                 0, 0,
                                 DTUD_GetFriendList(), FRIEND_LIST_LEN);

            // 認証用関数を使ってログイン
            DWC_LoginAsync(L"なまえ", NULL, cb_login, NULL);

            return state_login;
        }
        else
        {
            OS_TPrintf("Error\n");
            return state_disconnect;
        }
    }

    return state_connect;
}

/** --------------------------------------------------------------------
  ログイン中
  ----------------------------------------------------------------------*/
int
update_login( void )
{
    DWC_ProcessFriendsMatch();  // DWC通信処理更新

    if ( s_logined )
    {
        // ログイン成功
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );

        // ingamesnチェックの結果を取得する
	    if( DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID )
        {
            // 不適切な名前と判断された場合は特別な処理が必要
		    OS_TPrintf("BAD ingamesn is detected by NAS.\n");
        }

        // 友達登録鍵を取得
        s_friendkey = DWC_CreateFriendKey( DTUD_GetUserData() );

        s_canceled = FALSE;

        return state_online;
    }

    return state_login;
}

/** --------------------------------------------------------------------
  オンライン中
  ----------------------------------------------------------------------*/
int
update_online( void )
{
    DWC_ProcessFriendsMatch();  // DWC通信処理更新

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        DispMessage( "SAVE DATA?", "save", "cancel", NULL, NULL, NULL );
        return state_save;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DWC_ShutdownFriendsMatch();
        return state_disconnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_X )
    {
        DispMessage( "CHANGE FRIEND KEY?", "yes", "no", NULL, NULL, NULL );
        return state_initFriendKey;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_Y )
    { 
        DispMessage( "DELETE FRIEND DATA?", "yes", "no", NULL, NULL, NULL );
        return state_initFriendData;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_L )
    {
        DTUD_DispFriendList();
        return state_online;
    }

    return state_online;
}

/*---------------------------------------------------------------------------*
  Name:         update_initFriendKey

  Description:  友達登録鍵の初期化を行う。

  Arguments:    NONE.

  Returns:      友達リストがいっぱいで登録不可能か、キャンセルした場合 state_online を返す。
                交登開始の場合 state_changeFriendKey を返す。
 *---------------------------------------------------------------------------*/
static int
update_initFriendKey( void )
{
    int  friendIdx = -1;

    DWC_ProcessFriendsMatch();  // DWC通信処理更新

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // 友達登録鍵の文字列を'0'にクリアする
        MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));

        //友達リストの空きをチェック
        friendIdx = DTUD_GetAvailableFriendListIndex();
        if ( friendIdx == -1 )
        {
            OS_TPrintf( "Failed to register new friend. Friend list is full.\n" );
            DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
            return state_online;
        }
        else
        {
            return state_changeFriendKey;
        }
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // キャンセルされたらオンライン状態に戻る
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    return state_initFriendKey;
}

/*---------------------------------------------------------------------------*
  Name:         update_changeFriendKey

  Description:  友達登録鍵の入力と、友達登録を行う。

  Arguments:    NONE.

  Returns:      登録が終了した場合は state_init を返す。
                終了していない場合は state_changeFriendKey を返す。
 *---------------------------------------------------------------------------*/
static int
update_changeFriendKey( void )
{
    DWCUserData*   s_UserData;
    DWCFriendData* s_FriendList;
    int  friendIdx = -1;
    u8   maxFigure = 12;// 友達登録鍵は最高１２桁

    DWC_ProcessFriendsMatch();  // DWC通信処理更新
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンを押すと入力キャンセル
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;        
    }
    // 友達登録鍵数値入力
    else if( InputNumber( maxFigure ) )
    {
        // 友達登録鍵の正当性チェック
        s_UserData = DTUD_GetUserData();
        if( DWC_CheckFriendKey( s_UserData, DWC_StringToFriendKey( stFriendKey )))
        {
            // 友達登録鍵による友達登録
            friendIdx = DTUD_GetAvailableFriendListIndex();
            s_FriendList = DTUD_GetFriendList();
            DWC_CreateFriendKeyToken( &s_FriendList[friendIdx], DWC_StringToFriendKey( stFriendKey ) );
        }
        else
        {
            OS_TPrintf( "Invalid Friend Code!\n" );
        }
        DTUD_DispFriendList();
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    
    return state_changeFriendKey;
}

/*---------------------------------------------------------------------------*
  Name:         update_initFriendData

  Description:  友達削除のための初期化を行う。

  Arguments:    NONE.

  Returns:      削除終了か、キャンセルした場合 state_online を返す。
                まだ交登録中の場合 state_deleteFriendData を返す。
 *---------------------------------------------------------------------------*/
static int
update_initFriendData( void )
{
    DWC_ProcessFriendsMatch();  // DWC通信処理更新

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        DTUD_DispFriendList();
        
        // 友達登録鍵の文字列を'0'にクリアする
        MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));

        return state_inputFriendIndex;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }

    return state_initFriendData;
    
}

/*---------------------------------------------------------------------------*
  Name:         update_inputFriendIndex

  Description:  友達リストインデックスの数値入力を行う。

  Arguments:    NONE.

  Returns:      BOOL - 削除が終了、またはキャンセルした場合TRUE
                       終了していない場合はFALSEを返す。
 *---------------------------------------------------------------------------*/
static int
update_inputFriendIndex( void )
{
    u8 maxFigure = 2;
    u8 i;

    DWC_ProcessFriendsMatch();  // DWC通信処理更新

    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンを押すと入力キャンセル
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;        
    }
    else if( InputNumber( maxFigure ) )
    {
        // 友達リストが編集可能か確認
        if( DWC_CanChangeFriendList() )
        {
            DispMessage( "Is Data deleted, Really?", "yes", "no", NULL, NULL, NULL );
            PrintString( 1, 15, 0xf, "Set friend index:");
            for(i = 0; i < maxFigure; i++){
                PrintString( (s16)(18+i), 15, 0xf, "%c", stFriendKey[i] );
            }
            return state_deleteFriendData;
        }
        else
        {
            OS_TPrintf( "Invalid Friend Index!\n" );
            DTUD_DispFriendList();
            DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
            return state_online;
        }
    }
    return state_inputFriendIndex;
}

/*---------------------------------------------------------------------------*
  Name:         update_deleteFriendData

  Description:  友達リストインデックスにより友達削除を行う。

  Arguments:    NONE.

  Returns:      削除が終了、またはキャンセルした場合はstate_onlineを返す。
                終了していない場合はstate_deleteFriendDataを返す。
 *---------------------------------------------------------------------------*/
static int
update_deleteFriendData( void )
{
    DWCFriendData* s_FriendList;
    int friendIdx;
    u8 maxFigure = 2;

    DWC_ProcessFriendsMatch();  // DWC通信処理更新
        
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // 入力データをint型に変換
        friendIdx = (int)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));

        // 友達リストから友達を削除
        s_FriendList = DTUD_GetFriendList();
        DWC_DeleteBuddyFriendData(&s_FriendList[friendIdx]);

        DTUD_DispFriendList();
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    return state_deleteFriendData;
}

/** --------------------------------------------------------------------
  接続切断
  ----------------------------------------------------------------------*/
static int
update_disconnect( void )
{
    if ( DWC_CleanupInetAsync() )
    {
        return state_init;
    }

    return state_disconnect;
}

/** --------------------------------------------------------------------
  データをセーブする。
  ----------------------------------------------------------------------*/
static int
update_save( void )
{
    DWC_ProcessFriendsMatch();  // DWC通信処理更新
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        if( DWC_CheckDirtyFlag( DTUD_GetUserData() ) ){
            DTUD_SaveUserData();
            OS_TPrintf( "Saved User Data.\n" );
        }
        
        DTUD_SaveFriendList();
        OS_TPrintf( "Saved Friend Data.\n" );

        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    return state_save;
}

/** --------------------------------------------------------------------
  エラー解除待ち
  ----------------------------------------------------------------------*/
static int update_error( void )
{
    int errcode;
    DWCError err;
    DWCErrorType errtype;
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_START )
    {
        // スタートボタンでエラー解除
        err = DWC_GetLastErrorEx(&errcode, &errtype);

        DWC_ClearError();
        
        if ( errtype == DWC_ETYPE_DISCONNECT )
        {
            // 通信切断時はFriendsMatch処理の終了とインターネット接続の
            // Clearupを行なう
            DWC_ShutdownFriendsMatch();
            return state_disconnect;
        }
        else if ( errtype == DWC_ETYPE_FATAL )
        {
            // Fatal Errorなので終了
            OS_Panic("++FATAL ERROR\n");
            return state_init;  // not reached;
        }
        else if ( errtype == DWC_ETYPE_LIGHT )
        {
            // 軽いエラーは何もせずにオンライン状態に戻る
            return state_online;
        }
        else {
            // FriendsMatch処理のみ終了したら、再びFrientsMatch初期化を行なう
            DWC_ShutdownFriendsMatch();
            return state_connect;
        }
    }

    return state_error;
}

/*---------------------------------------------------------------------------*
  Name:         InputNumber

  Description:  友達登録鍵の数値入力を行う。

  Arguments:    maxFigure - 入力する最大の桁数

  Returns:      入力終了ならTRUE
                入力中ならFALSEを返す。
 *---------------------------------------------------------------------------*/
static BOOL InputNumber( u8 maxFigure )
{
    static u8 figure = 0;

    if(figure > maxFigure){
        figure = 0;
    }
    
    DispMenuMsg(figure, maxFigure);

    if (g_KeyCtrl.trg & PAD_BUTTON_A)
    {
        figure = 0;
        return TRUE;
    }
    else if (g_KeyCtrl.trg & PAD_KEY_UP)
    {
        // 十字キー上下で選択中の数値の増減
        stFriendKey[figure]++;
        if(stFriendKey[figure] > '9')
        {
            stFriendKey[figure] = '0';
        }
        // 入力中数値表示
        DispMenuMsg(figure ,maxFigure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_DOWN)
    {
        // 十字キー上下で選択中の数値の増減
        stFriendKey[figure]--;
        if(stFriendKey[figure] < '0')
        {
            stFriendKey[figure] = '9';
        }
        // 入力中数値表示
        DispMenuMsg(figure, maxFigure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_LEFT)
    {
        // 十字キー左右で選択桁数の移動
        if (figure > 0 && figure <= maxFigure-1 ) figure--;
        // 入力中数値表示
        DispMenuMsg(figure, maxFigure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_RIGHT)
    {
        // 十字キー左右で選択桁数の移動
        if (figure >= 0 && figure < maxFigure-1 ) figure++;
        // 入力中数値表示
        DispMenuMsg(figure, maxFigure);
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DispMenuMsg

  Description:  友達登録中の数値を表示する。

  Arguments:    figure    - 入力中の桁数
                maxFigure - 最大桁数

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
static void DispMenuMsg( const u8 figure, const u8 maxFigure )
{
    int i;

    int x = 16;

    DispMessage( "NUMBER INPUT", "set data", "cancel", NULL, NULL, NULL );
    
    // 数値入力中
    // 友達登録。友達登録鍵入力中
    if( maxFigure == 12 )
    {
        PrintString( 1, 15, 0xf, "Set friend key:");
        for(i = 0; i < maxFigure; i++){
            PrintString( (s16)(x+i), 15, 0xf, "%c", stFriendKey[i] );
            if(i%4 == 3 && i != maxFigure - 1)
            {
                x++;
                PrintString( (s16)(x+i), 15, 0xf, "-" );
            }
        }
        // 選択中の数字を示す為の下線を表示する
        PrintString( (s16)(16+figure+figure/4), 16, 0xf, "-" );
    }
    else if( maxFigure == 2 )
    {
        x += 2;
        PrintString( 1, 15, 0xf, "Set friend index:");
        for(i = 0; i < maxFigure; i++){
            PrintString( (s16)(x+i), 15, 0xf, "%c", stFriendKey[i] );
        }
        // 選択中の数字を示す為の下線を表示する
        PrintString( (s16)(18+figure), 16, 0xf, "-" );
    }
}

/** --------------------------------------------------------------------
  ----------------------------------------------------------------------*/
/** --------------------------------------------------------------------
  コールバック関数
  ----------------------------------------------------------------------*/
static void
cb_login(DWCError error, int profileID, void* param)
{
    (void)param;
    if (error == DWC_ERROR_NONE)
    {
        // 認証成功、プロファイルID取得
        OS_TPrintf("Login succeeded. profileID = %u\n\n", profileID);

        s_profileID = profileID;  // プロファイルIDを記録

        // stUserDataが更新されているかどうかを確認。
        if ( DTUD_CheckUpdateUserData() )
        {
            DTUD_SaveUserData();
        }

        // 友達リストとGameSpyサーバ上のバディリストの同期処理
        DWC_UpdateServersAsync(NULL,
                               cb_updateServers, NULL,
                               NULL, NULL,
                               NULL, NULL);
        
        // コネクションクローズコールバックを設定
        DWC_SetConnectionClosedCallback(cb_close, NULL);
    }
    else
    {
        OS_TPrintf("Login Error\n");
    }
}

static void
cb_updateServers(DWCError error, BOOL isChanged, void* param)
{
    (void)param;
    (void)isChanged;

    if ( error == DWC_ERROR_NONE )
    {
        // 友達リスト同期処理成功
        OS_TPrintf( "Update Servers succeeded.\n" );
        
        s_logined = TRUE;
    }
    else
    {
        OS_TPrintf("Error\n");
    }
}

static void
cb_close(DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param)
{
    (void)isLocal;
    (void)isServer;
    (void)param;

    OS_TPrintf( "Closed Callback err:%d, aid:%d, idx:%d\n",
               error, aid, index );
}

/** -------------------------------------------------------------------------
  ---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         DispMessage

  Description:  汎用のメッセージを表示する。

  Arguments:    main_msg - 現在のモード
                msg_a    - Aボタンが押された時の処理
                msg_b    - Bボタンが押された時の処理
                msg_x    - Xボタンが押された時の処理
                msg_y    - Yボタンが押された時の処理
                msg_l    - Lボタンが押された時の処理

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
static void
DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l )
{
    s16 x = 1;
    s16 y = 5;

    ClearScreen();
    
    PrintString( 0, 0, 0xf, "================================" );
    PrintString( 2, 1, 0xf, "*** FRIEND DATA SAMPLE ***" );
    PrintString( 0, 2, 0xf, "================================" );
    
    if ( main_msg == NULL ) return;

    PrintString( 0, y, 0xf, "--------------------------------" );
    y++;
    // メインメッセージ表示
    while ( *main_msg != '\0' )
    {
        if ( *main_msg == '\n' )
        {
            x = 1;  y++;
        }
        else
        {
            PrintString( x, y, 0xf, "%c", *main_msg );
            x++;
        }
        main_msg++;
    }
    y++;
    PrintString( 0, y, 0xf, "--------------------------------" );

    y += 2;

    // Aボタン説明、Bボタン、Xボタン、Yボタン説明表示
    if ( msg_a != NULL )
    {
        PrintString( 1, y, 0xf, "A BUTTON:%s", msg_a );
    }
    y++;
    if ( msg_b != NULL )
    {
        PrintString( 1, y, 0xf, "B BUTTON:%s", msg_b );
    }
    y += 2;
    if ( msg_x != NULL )
    {
        PrintString( 1, y, 0xf, "X BUTTON:%s", msg_x );
    }
    y++;
    if ( msg_y != NULL )
    {
        PrintString( 1, y, 0xf, "Y BUTTON:%s", msg_y );
    }
    y += 2;
    if ( msg_l != NULL )
    {
        PrintString( 1, y, 0xf, "L BUTTON:%s", msg_l );
    }
}

/** --------------------------------------------------------------------
  毎フレーム呼び出す、状態表示関数
  ----------------------------------------------------------------------*/
void
DrawConsole( void )
{
    static int i = 0;
    char loop[] = "/|\\-";

    dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
    dbs_Print( 13, 0, "n:%d", DWC_GetNumConnectionHost() );
    dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
    dbs_Print( 13, 1, "p:%d", s_profileID );
    dbs_Print( 30, 0, "%c", loop[((i)>>2)&3] );
    dbs_Print( 0, 2, "friendkey:%012lld", s_friendkey );

    i++;
}

/** --------------------------------------------------------------------
  エントリポイント
  ----------------------------------------------------------------------*/
void NitroMain ()
{
    initFunctions();

    // デバッグ表示レベル指定
    DWC_SetReportLevel((u32)( DWC_REPORTFLAG_ALL &
                              ~DWC_REPORTFLAG_QR2_REQ ));
    
    // DWCライブラリ初期化
    DWC_Init( s_Work );

    // ユーザデータ読み込み
    DTUD_Init();
    // メモリ確保関数設定
    DWC_SetMemFunc( DTDB_GetAllocFunc(), DTDB_GetFreeFunc() );

    while (1)
    {
        DrawConsole();

        ReadKeyData();

        update();

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
        dbs_DemoLoad();
    }

    OS_Terminate();
}
