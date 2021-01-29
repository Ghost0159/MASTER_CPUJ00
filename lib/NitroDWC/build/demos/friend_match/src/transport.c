#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"

#include "dbs.h"

#include "transport.h"

extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // ゲーム制御情報構造体
extern KeyControl stKeyCnt;    // キー入力制御構造体

//callbackプロトタイプ
static void ConnectionClosedCallback(DWCError error,
                                     BOOL isLocal,
                                     BOOL isServer,
                                     u8 aid,
                                     int index,
                                     void* param);

/** -------------------------------------------------------------------------
  DWC Transport
  ---------------------------------------------------------------------------*/
#define			MAX_PLAYERS				3
const int		SIZE_SEND_BUFFER = 512;
const int		SIZE_RECV_BUFFER = 4 * 1024;

static u8		sSendBuffer[ SIZE_SEND_BUFFER ];
static u8		sRecvBuffer[ MAX_PLAYERS ][ SIZE_RECV_BUFFER ];

/** -------------------------------------------------------------------------
  送信完了コールバック  
  ---------------------------------------------------------------------------*/
static void
SendDoneCallback( int size, u8 aid )
{
#ifdef SDK_FINALROM
#pragma unused(size, aid)
#endif
	OS_TPrintf( "to aid = %d  size = %d\n", aid, size );
}

/** -------------------------------------------------------------------------
  受信完了コールバック  
  ---------------------------------------------------------------------------*/
static void
UserRecvCallback( u8 aid, u8* buffer, int size )
{
#ifdef SDK_FINALROM
#pragma unused(aid, buffer, size)
#endif
	OS_TPrintf( "from aid = %d  size = %d  buffer[0] = %X\n", aid, size, buffer[0] );
}

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

/** -------------------------------------------------------------------------
  Reliabe送信
  ---------------------------------------------------------------------------*/
static void
BroadCastReliable( void )
{
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID())
        {
            continue;
        }

        DWC_SendReliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
    }
}

/** -------------------------------------------------------------------------
  Unreliable送信
  ---------------------------------------------------------------------------*/
static void
BroadCastUnreliable( void )
{
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID()) continue;

        DWC_SendUnreliable( pAidList[i], sSendBuffer, SIZE_SEND_BUFFER );
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
    
    // 送信コールバックの設定	
    DWC_SetUserSendCallback( SendDoneCallback ); 

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

    OS_TPrintf( "my aid = %d\n", DWC_GetMyAID() );
    
    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );

        ReadKeyData();  // キーデータ取得

        DWC_ProcessFriendsMatch();  // DWC通信処理更新

        if (DWC_GetLastError(NULL)){
            // マッチメイク失敗時の処理
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

            switch (curIdx){
            case 0:             // Reliable送信
                BroadCastReliable();
				break;
            case 1:             // Unreliable送信
				BroadCastUnreliable();
                break;
            case 2:             // コネクションクローズ
                DWC_CloseConnectionsAsync();

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
#ifdef SDK_FINALROM
#pragma unused(aid, index)
#endif

    if (error == DWC_ERROR_NONE){
        if (!isLocal && isServer){
            // サーバクライアントマッチメイクで、自分がクライアントの時に、
            // サーバDSがコネクションをクローズした。
            // サーバDSがクローズしたので自分もクローズする。
            DWC_CloseConnectionsAsync();
        }
        
        if (DWC_GetNumConnectionHost() == 1){
            // 全てのコネクションがクローズされた場合
            if (isLocal){
                // 自分からクローズした場合は全員とのコネクションが
                // クローズできたら次に進む
                stGameCnt.blocking = FALSE;  // ブロッキング解除
            }
            if ((DWC_GetLastMatchingType() != DWC_MATCH_TYPE_SC_SV) || isServer){
                // isServer==TRUEで自分でクローズした場合と、
                // 相手からのクローズの場合は、ここで次のモードをセットする
                *(GameMode *)param = GAME_MODE_LOGIN;
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
        DispErrorMessage();
        OS_TPrintf("Failed to close connections. %d\n\n", error);

        *(GameMode *)param = GAME_MODE_MAIN;

        stGameCnt.blocking = FALSE;  // ブロッキング解除
    }
}


