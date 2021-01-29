#include <nitro.h>
#include <ninet/ip.h>
#include <nitro/os.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"

#include "dbs.h"
#include "screen.h"
#include "sound.h"

#include "transport.h"
#include "demo_util.h"

extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // ゲーム制御情報構造体 
extern KeyControl stKeyCnt;    // キー入力制御構造体 

/** -------------------------------------------------------------------------
  DWC Transport
  ---------------------------------------------------------------------------*/
const int		SIZE_SEND_BUFFER = 512;
const int		SIZE_RECV_BUFFER = 4 * 1024;

static u8		sSendBuffer[ SIZE_SEND_BUFFER ];
static u8		sRecvBuffer[ MAX_PLAYERS ][ SIZE_RECV_BUFFER ];

static int      stExchangeProfile = 0;

#define DEMO_EXCHANGE_MAGIC_TOKEN '_Vdm'

/** -------------------------------------------------------------------------
  DWC Transport用　受信バッファを設定  
  ---------------------------------------------------------------------------*/
void
SetRecvBuffer( void )
{
	int i;
    int j = 0;
	int connectingNum = DWC_GetNumConnectionHost()	;

    OS_TPrintf( "connection host = %d\n", connectingNum );
    
    for ( i = 0; i < connectingNum; ++i )
    {
		if ( i == DWC_GetMyAID() )
        {
            j++;
			continue;
        }

        DWC_SetRecvBuffer( (u8)i, &sRecvBuffer[i-j], SIZE_RECV_BUFFER );
    }
}

/*---------------------------------------------------------------------------*
  DWC 受信処理
  ---------------------------------------------------------------------------*/

static void UserRecvCallback( u8 aid, u8* buffer, int size )
{
    BOOL flag;
    
    flag = VCT_HandleData(aid, buffer, size);

    if (flag == FALSE) {
        //
        // 
        // VCT_HandleDataの戻り値がFALSEの場合、VoiceChat以外のデータ、または
        // VoiceChatが初期化されていない状態です。
        //
        // デモではクライアントの状態をやりとりしています
        // 
        //
        client_info *info = (client_info*)buffer;
        if (info->magic != DEMO_EXCHANGE_MAGIC_TOKEN) {
            return;
        }

        UpdateClientInfo(aid, info);
    }
}

/*---------------------------------------------------------------------------*
  メッシュ接続完了後メイン関数
 *---------------------------------------------------------------------------*/
GameMode GameConnectedMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_CONNECTED];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

    // レポートレベルを下げる
    DWC_SetReportLevel(0);
    
    // 受信コールバックの設定	
    DWC_SetUserRecvCallback( UserRecvCallback ); 
    
    // 受信バッファの設定	
    SetRecvBuffer();

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

    (void)PM_SetBackLight( PM_LCD_ALL, PM_BACKLIGHT_ON );
    OS_TPrintf( "my aid = %d\n", DWC_GetMyAID() );

    // プロフィール情報を交換する
    //
    BroadCastStatus(DEMO_STATUS_JOIN);
    
    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
    
    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print(14, 0, "a:%d", DWC_GetMyAID());
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );

        ReadKeyData();  // キーデータ取得 

        DWC_ProcessFriendsMatch();  // DWC通信処理更新 

        if (DWC_GetLastError(NULL)){
            // マッチング失敗時の処理
            ShutdownInet();  // ネットワーク切断 
            DWC_ClearError();
            returnSeq = GAME_MODE_MAIN;
            break;
        }

        if (stGameCnt.blocking){
            // DWC処理中はキー操作を禁止する
            dbs_DemoUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();
            // スタック溢れチェック
            OS_CheckStack(OS_GetCurrentThread());
            continue;
	    }

        // 次に進むべきモードがセットされていたらループを抜ける
        if (returnSeq != GAME_MODE_NUM) break;

        ////////// 以下キー操作処理
        if (stKeyCnt.trg & PAD_BUTTON_A){
            // Aボタンでメニュー決定
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (curIdx) {
            case 3:
                DWC_CloseAllConnectionsHard();
                
                if (DWC_GetNumConnectionHost() <= 1){
                    // 既に接続ホスト数が0ならすぐに終了し、コールバックは呼ばれない
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
        ////////// キー操作処理ここまで

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
        // スタック溢れチェック
        OS_CheckStack(OS_GetCurrentThread());
    }

    return returnSeq;
}

