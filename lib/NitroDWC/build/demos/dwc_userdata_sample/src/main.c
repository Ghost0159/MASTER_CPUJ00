#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "userdata.h"
#include "screen.h"
#include "mp.h"

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
static DWCFriendsMatchControl	s_FMCtrl;
static DWCInetControl			s_ConnCtrl;
static DWCFriendData stSampleFriendData ATTRIBUTE_ALIGN( 32 );// サンプルMP交換用友達情報
static u16* s_SendBuf ATTRIBUTE_ALIGN(32);                  // データ送信用バッファ
static char stFriendKey[sizeof(DWCFriendData)];             // 友達登録鍵
static int  s_state  = state_init;
static int  s_msgSeq;

static const char* s_messages[msg_seq_last][6] =
{
    {
        NULL, NULL, NULL, NULL, NULL, NULL
    },
    {
        "The Wi-Fi ID may have been\nerased.\nPlease check your access-point\nsettings with Nintendo WFC\nsetup.",
        "continue",
        NULL, NULL, NULL, NULL
    },
    {
        "Connect to Nintendo WFC?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "Nintendo WFC treats your\nGame Card and Nintendo DS as\na set.\nSave this Nintendo DS system's\nNintendo WFC configuration to\nthis Game Card and connect to\nNintendo WFC?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "The Wi-Fi ID from this \nNintendo DS has been saved to \nthe Game Card.\nFrom now on, please use this\nNintendo DS when attempting to\nconnect to Nintendo WFC.",
        "continue",
        NULL, NULL, NULL, NULL
    },
    {
        "Unable to connect to Nintendo \nWFC because the Wi-Fi ID from \nthe Nintendo DS and the Game \nCard do not match.",
        "continue",
        NULL, NULL, NULL, NULL
    },
    {
        "When you update your Wi-Fi ID,\nyou will lose any data used \nto connect to Nintendo WFC up \nto this point.\nUpdate the Wi-Fi ID saved to \nthis Game Card anyway?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "Do you still want to update?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "Connecting to Nintendo WFC...\nPlease wait a moment...",
        "success",
        "error", NULL, NULL, NULL
    },
};

/** --------------------------------------------------------------------
  static functions
  ----------------------------------------------------------------------*/
static int  update_init( void );
static int  update_initMPData( void );
static int  update_changeMPData( void );
static int  update_initFriendKey( void );
static int  update_changeFriendKey( void );
static int  update_readyToConnect( void );
static BOOL InputNumber( void );
static void DispMenuMsg( const u8 figure );
static int  do_msgSeqInit( void );
static int  do_msgSeq0003( void );
static int  do_msgSeq0004( void );
static int  do_msgSeq0005( void );
static int  do_msgSeq0006( void );
static int  do_msgSeq0007( void );
static int  do_msgSeq0008( void );
static int  do_msgSeq0009( void );

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
  1フレーム1回呼ばれる関数
  ----------------------------------------------------------------------*/
static void
update( void )
{
    switch ( s_state )
    {
    case state_init:			s_state = update_init();			  break;
    case state_initMPData:      s_state = update_initMPData();        break;
    case state_changeMPData:    s_state = update_changeMPData();      break;
    case state_initFriendKey:   s_state = update_initFriendKey();     break;
    case state_changeFriendKey: s_state = update_changeFriendKey();   break;
    case state_readyToConnect:	s_state = update_readyToConnect();	  break;
    }
}

/** --------------------------------------------------------------------
  初期状態
  ----------------------------------------------------------------------*/
static int
update_init( void )
{
    DispMessage( "INIT MODE", "save", "create userdata", "change MPdata", "change friendkey", "friend state" );

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // AボタンでWi-Fiコネクション接続準備に進む  
        s_msgSeq = msg_seq_init;
        return state_readyToConnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンでバックアップデータを消去する
        DTUD_DeleteUserData();
        DTUD_DeleteFriendList();
        DTUD_Init();
        
        OS_TPrintf( "Delete and Create UserData.\n" );
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_X )
    {
        // XボタンでMPで友達情報登録モード
        DispMessage( "CHANGE MP FRIENDDATA?", "yes", "no", NULL, NULL, NULL );
        return state_initMPData;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_Y )
    {
        // Yボタンで友達登録鍵で友達情報登録モード
        DispMessage( "CHANGE FRIEND KEY?", "yes", "no", NULL, NULL, NULL );
        return state_initFriendKey;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_L )
    {
        DTUD_DispFriendList();
    }

    return state_init;
}

/*---------------------------------------------------------------------------*
  Name:         update_initMPData

  Description:  MP通信用のデータの作成とデータのセットを行う。

  Arguments:    NONE.

  Returns:      交換を行う場合 state_changeMPData を返す。
                交換キャンセルの場合 state_init を返す。
 *---------------------------------------------------------------------------*/
static int
update_initMPData( void )
{
    DWCUserData*   s_UserData;
    DWCFriendData* s_FriendList;
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // MP通信用データ作成
        s_UserData = DTUD_GetUserData();
        DWC_CreateExchangeToken(s_UserData, &stSampleFriendData);
        s_SendBuf = (u16*)(&stSampleFriendData);
        
        // MPデータセット
        s_FriendList = DTUD_GetFriendList();
        MP_SetData(s_SendBuf, s_FriendList);
        return state_changeMPData;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        return state_init;
    }
    
    return state_initMPData;
}

/*---------------------------------------------------------------------------*
  Name:         update_changeMPData

  Description:  MPで友達情報を交換し、交換した情報を登録する。

  Arguments:    NONE.

  Returns:      交換が終了、またはキャンセルされた場合は state_init を返す。
                終了していない場合は state_changeMPData を返す。
 *---------------------------------------------------------------------------*/
static int
update_changeMPData( void )
{
    DWCFriendData* s_FriendList;
    int  friendIdx;
    int  i,j;

    // MP通信スタート
    if( MP_Match() )
    {
        // MP通信完了
        s_FriendList = DTUD_GetFriendList();
        friendIdx = DTUD_GetAvailableFriendListIndex();// 同一のデータを省く
        for(i = 0; i < friendIdx; i++){
            for(j = i+1; j < friendIdx; j++){
                if(DWC_IsEqualFriendData( (const DWCFriendData*)&s_FriendList[i], (const DWCFriendData*)&s_FriendList[j])){
                    MI_CpuClear8(&s_FriendList[j], sizeof(DWCFriendData));
                }
            }
        }
        MP_Finish();
        DTUD_DispFriendList();
        return state_init;
    }
    return state_changeMPData;
}

/*---------------------------------------------------------------------------*
  Name:         update_initFriendKey

  Description:  友達登録鍵の初期化を行う。

  Arguments:    NONE.

  Returns:      友達リストがいっぱいで登録不可能か、キャンセルした場合 state_init を返す。
                交登開始の場合 state_changeFriendKey を返す。
 *---------------------------------------------------------------------------*/
static int
update_initFriendKey( void )
{
    int  friendIdx = -1;
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // 友達登録鍵の文字列を'0'にクリアする
        MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));
        
        // 友達リストの空きをチェック
        friendIdx = DTUD_GetAvailableFriendListIndex();
        if ( friendIdx == -1 )
        {
            OS_TPrintf( "Failed to register new friend. Friend list is full.\n" );
            return state_init;
        }
        else
        {
            return state_changeFriendKey;
        }
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        return state_init;        
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

    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bを押すと入力キャンセル
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_init;
    }
    // 友達登録鍵数値入力
    else if( InputNumber() )
    {
        // 友達登録鍵の正当性チェック
        s_UserData = DTUD_GetUserData();
        if( DWC_CheckFriendKey( s_UserData, DWC_StringToFriendKey( stFriendKey )))
        {
            // 友達登録鍵による友達登録
            s_FriendList = DTUD_GetFriendList();
            friendIdx = DTUD_GetAvailableFriendListIndex();
            DWC_CreateFriendKeyToken( &s_FriendList[friendIdx], DWC_StringToFriendKey( stFriendKey ) );
        }
        else
        {
            OS_TPrintf( "Invalid Friend Code!\n" );
        }
        DTUD_DispFriendList();
        return state_init;
    }
    
    return state_changeFriendKey;
}

/** --------------------------------------------------------------------
  Wi-Fiコネクション接続準備
  ----------------------------------------------------------------------*/
static int
update_readyToConnect( void )
{
    // 各シーケンス用のメッセージを表示する
    DispMessage( s_messages[s_msgSeq][0], s_messages[s_msgSeq][1], s_messages[s_msgSeq][2], s_messages[s_msgSeq][3], s_messages[s_msgSeq][4], s_messages[s_msgSeq][5]);

    switch ( s_msgSeq )
    {
    case msg_seq_init:			s_msgSeq = do_msgSeqInit();			break;
    case msg_seq_0003:			s_msgSeq = do_msgSeq0003();			break;
    case msg_seq_0004:			s_msgSeq = do_msgSeq0004();			break;
    case msg_seq_0005:			s_msgSeq = do_msgSeq0005();			break;
    case msg_seq_0006:			s_msgSeq = do_msgSeq0006();			break;
    case msg_seq_0007:			s_msgSeq = do_msgSeq0007();			break;
    case msg_seq_0008:			s_msgSeq = do_msgSeq0008();			break;
    case msg_seq_0009:			s_msgSeq = do_msgSeq0009();			break;
    }

    if ( s_msgSeq == msg_seq_exit )
    {
        return state_init;
    }
    else
    {
        return state_readyToConnect;
    }
}

/** --------------------------------------------------------------------
  接続時フローに従ったシーケンス処理
  ----------------------------------------------------------------------*/
static int
do_msgSeqInit( void )
{
    if ( DWC_CheckHasProfile( DTUD_GetUserData() ) )
    {
        // 既にGameSpyのプロファイルを持っている場合
        if ( DWC_CheckValidConsole( DTUD_GetUserData() ) )
        {
            // DS本体とDSカードのユーザデータが一致した場合
            return msg_seq_0003;
        }
        else
        {
            // DS本体とDSカードのユーザデータが一致しなかった場合
            OS_TPrintf( "UserData is not Valid Console.\n" );

            return msg_seq_0006;
        }
    }
    else
    {
        // まだ一度もログインしていない（もしくはログイン後にセーブ
        // できなかった）場合は、そのまま接続シーケンスに進む。
        // このサンプルでは、Wi-Fiコネクションへの接続は実際には行なわないので、
        // 認証済みログインIDが入手できず、毎回ここに来ることになる。
        return msg_seq_0004;
    }

    return msg_seq_init;
}

static int
do_msgSeq0003( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // AボタンでWi-Fiコネクションへの接続を開始する
        return msg_seq_0009;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンで処理を中断して最初に戻る
        return msg_seq_exit;
    }

    return msg_seq_0003;
}

static int
do_msgSeq0004( void )
{
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // AボタンでWi-Fiコネクションへの接続を開始する
        return msg_seq_0009;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンで処理を中断して最初に戻る
        return msg_seq_exit;
    }

    return msg_seq_0004;
}

static int
do_msgSeq0005( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // Aボタンで接続完了したことにして、また最初に戻る
        OS_TPrintf( "Connected!\n" );
        return msg_seq_exit;
    }

    return msg_seq_0005;
}

static int
do_msgSeq0006( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // Aボタンで次に進む
        return msg_seq_0007;
    }

    return msg_seq_0006;
}

static int
do_msgSeq0007( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // Aボタンで次に進む
        return msg_seq_0008;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンで処理を中断して最初に戻る
        return msg_seq_exit;
    }

    return msg_seq_0007;
}

static int
do_msgSeq0008( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // AボタンでWi-Fiコネクションへの接続を開始する
        return msg_seq_0009;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンで処理を中断して最初の質問に戻る
        return msg_seq_0006;
    }

    return msg_seq_0008;
}

static int
do_msgSeq0009( void )
{
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // Aボタンでエラーなく成功したことにする
        DTUD_SaveFriendList();
        OS_TPrintf( "Saved FriendData.\n");
        if ( DTUD_CheckUpdateUserData() )
        {
            // ユーザデータが更新されている場合はセーブする
            DTUD_SaveUserData();

            OS_TPrintf( "Saved UserData.\n");
            return msg_seq_0005;
        }
        else
        {
            // 接続完了したことにして、また最初に戻る
            OS_TPrintf( "Connected!\n" );
            return msg_seq_exit;
        }
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // Bボタンでエラーが発生したことにして最初に戻る
        OS_TPrintf( "Error occured!\n" );
        return msg_seq_exit;
    }

    return msg_seq_0009;
}

/*---------------------------------------------------------------------------*
  Name:         InputNumber

  Description:  友達登録鍵の数値入力を行う。

  Arguments:    NONE.

  Returns:      友達登録鍵入力の状態を返す。
                key_change:入力終了
                key_set   :入力中
 *---------------------------------------------------------------------------*/
static BOOL InputNumber( void )
{
    static u8  figure = 0;
    u8 maxFigure = 12;

    DispMenuMsg(figure);
    
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
        DispMenuMsg(figure);
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
        DispMenuMsg(figure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_LEFT)
    {
        // 十字キー左右で選択桁数の移動
        if (figure > 0 && figure <= maxFigure-1 ) figure--;
        // 入力中数値表示
        DispMenuMsg(figure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_RIGHT)
    {
        // 十字キー左右で選択桁数の移動
        if (figure >= 0 && figure < maxFigure-1 ) figure++;
        // 入力中数値表示
        DispMenuMsg(figure);
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
static void DispMenuMsg( const u8 figure )
{
    int i;
    u8 maxFigure = 12;

    int x = 16;

    DispMessage( "NUMBER INPUT", "set data", "cancel", NULL, NULL, NULL );
    
    // 数値入力中
    // 友達登録。友達登録鍵入力中
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
void
DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l )
{
    s16 x = 1;
    s16 y = 7;

    ClearScreen();
    
    PrintString( 0, 0, 0xf, "================================" );
    PrintString( 2, 1, 0xf, "*** USER DATA SAMPLE ***" );
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
  ----------------------------------------------------------------------*/
/** --------------------------------------------------------------------
  毎フレーム呼び出す、状態表示関数
  ----------------------------------------------------------------------*/
void
DrawConsole( void )
{
    static int i = 0;
    char loop[] = "/|\\-";

    dbs_Print( 30,0, "%c", loop[((i)>>2)&3] );

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
    if ( DWC_Init( s_Work ) == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        // DS本体のNVRAMが破壊されている
        DispMessage( s_messages[s_msgSeq][0], s_messages[s_msgSeq][1], s_messages[s_msgSeq][2], s_messages[s_msgSeq][3], s_messages[s_msgSeq][4], s_messages[s_msgSeq][5]);
        
        while (1)
        {
            DrawConsole();
            ReadKeyData();

            // Aボタンで先に進む
            if ( g_KeyCtrl.trg & PAD_BUTTON_A ) break;

            dbs_DemoUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();
        }
    }

    // ユーザデータ読み込み
    DTUD_Init();
    // ここまで起動時の処理


    // ここからゲーム中のWi-Fi関連処理
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
