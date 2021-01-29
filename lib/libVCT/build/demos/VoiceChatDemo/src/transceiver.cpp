//
//  libVCT トランシーバーモードのデモアプリケーション 
//

#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "dbs.h"
#include "utility.h"
#include "screen.h"

#include "transceiver.h"
#include "sound.h"
#include "vct.h"

#include "vct_menu.h"
#include "demo_util.h"
#include "demo_menu.h"

#ifdef SDK_DEBUG
#include "debug_menu.h"
#endif

//
//  広域変数 
//
extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;
extern KeyControl stKeyCnt;

//
//  静的変数 
//
static VCTSession sSession[1];
static VCTSession *sCurrentSession = NULL;

extern KeyControl stKeyCnt;
extern GameControl stGameCnt;

static Menu *sMenu = NULL;

//
//  関数プロトタイプ 
//
static void         ClearSession(VCTSession *session);
static Menu*        InitMenu();
static void         DestroyMenu();

/////////////////////////////////////////////////////////////////////////////////////
//
//  関数コスト計測用 
//
#ifdef SDK_PROFILE_FUNCTIONCOST

#include "cost.h"

#define MYCOSTBUFFER_SIZE 0x30000
#define MYSTATBUFFER_SIZE 0x3000

static u32 myCostBuffer[MYCOSTBUFFER_SIZE / sizeof(u32)];
static u32 myStatBuffer[MYSTATBUFFER_SIZE / sizeof(u32)];

#endif

/////////////////////////////////////////////////////////////////////////////////////
//
//  Ｌボタンが押されたときの処理 
//
static void LButtonDown()
{
    VCTSession *session;
	int ret;

    ret = VCT_Contact(&session);
    switch (ret) {
    case VCT_ERROR_TRANSCEIVER_BUSY:
        //  トランシーバーモードで、既に誰かが会話中の場合 VCT_ERROR_TRANSCEIVER_BUSYが帰ります 
        //
        OS_TPrintf("Transceiver is busy\n");
        PlaySE(SE_BUSY);
        break;

    case VCT_ERROR_SEND_FAIL:
        //  送信失敗の際は、セッションを削除してください 
        //
        VCT_DeleteSession(session);
        break;

    case VCT_ERROR_NONE:
        //  エラーなし 
        sCurrentSession = session;
        break;

    default:
        break;
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Ｌボタンが離されたときの処理 
//
static void LButtonUp()
{
    int ret;

    if (sCurrentSession) {
        //
        // まずストリーミングを停止し、その後Releaseを発行します。
        // （サーバーの応答が遅れたりした場合に、音声をストリーミングし続ける
        // のを防ぐため）。
        //
        VCT_StopStreaming(sCurrentSession);
        ret = VCT_Release(sCurrentSession);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Can't send Release request [%d]\n", ret);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  VoiceChatのイベントコールバック関数
//
static void VoiceChatEventCallback(u8 aid, VCTEvent event, VCTSession *session, void *data)
{
#pragma unused(aid, data)
    
    switch (event) {
    //
    //  サーバーのイベント
    //
    case VCT_EVENT_CONTACT:
        //
        //  Contactに対する応答はlibVCT内部で既に行われています。
        //
        OS_TPrintf("Contact from aid:%d\n", session->aid);
        sCurrentSession = session;
        VCT_StartStreaming(session);
        BroadCastStatus(DEMO_STATUS_TALKING);
        PlaySE(SE_NOISE);
        break;

    case VCT_EVENT_RESPONDBYE:
        OS_TPrintf("Release from aid:%d\n", session->aid);
        PlaySE(SE_NOISE);
        VCT_StopStreaming(session);
        ClearSession(session);
        break;

    //
    //  サーバー・クライアント共通のイベント
    //
    case VCT_EVENT_DISCONNECTED:
        VCT_StopStreaming(session);
        OS_TPrintf("Disconnected\n");
        ClearSession(session);
        PlaySE(SE_NOISE);
        break;

    //
    //  クライアントのイベント
    //
    case VCT_EVENT_BUSY:
        OS_TPrintf("Return busy here from aid:%d\n", session->aid);
        ClearSession(session);
        PlaySE(SE_BUSY);
        break;
        
    case VCT_EVENT_CONNECTED:
        OS_TPrintf("Connected by aid:%d (talking %d)\n", session->aid, session->talking);
        PlaySE(SE_BEEP);
        VCT_StartStreaming(session);
        BroadCastStatus(DEMO_STATUS_TALKING);
        break;

    case VCT_EVENT_NOTIFY_FREE:
        OS_TPrintf("Notify Free\n");
        PlaySE(SE_NOISE);
        VCT_StopStreaming(session);
        ClearSession(session);
        break;
        
    case VCT_EVENT_NOTIFY_BUSY:
        OS_TPrintf("Notify Busy\n");
        PlaySE(SE_NOISE);
        VCT_StartStreaming(session);
        BroadCastStatus(DEMO_STATUS_TALKING);
        sCurrentSession = session;
        break;

    case VCT_EVENT_TIMEOUT:
        if (session == sCurrentSession) {
            OS_TPrintf("Session timeout\n");
            VCT_StopStreaming(session);
            ClearSession(session);
        }
        break;

    case VCT_EVENT_ABORT:
        OS_TPrintf("Session abrot\n");
        VCT_StopStreaming(session);
        ClearSession(session);
        break;

    default:
        OS_TPrintf(" not handler (%s)\n", EventName(event));
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  セッションのクリア
//
static void ClearSession(VCTSession *session)
{
    if (session == sCurrentSession) {
        VCT_StopStreaming(session);
        VCT_DeleteSession(session);
        sCurrentSession = NULL;
    }
    else {
        SDK_WARNING(FALSE, "What is this session??\n");
        VCT_DeleteSession(session);
    }
    sMenu->ChangeMenu(MENU_AUDIO_INFO);
    BroadCastStatus(DEMO_STATUS_TRANSCEIVER);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  GAME_CONNECTED_MAIN から呼び出される関数
//
GameMode TransceiverMain()
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_TRANSCEIVER];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

    InitMenu();
    DWC_SetReportLevel(0);

#ifdef SDK_PROFILE_FUNCTIONCOST
    OS_InitFunctionCost(&myCostBuffer, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer, MYSTATBUFFER_SIZE);
#endif

    BroadCastStatus(DEMO_STATUS_TRANSCEIVER);
    //
    //  VoiceChat SDK を初期化
    //
    VCTConfig config;

    config.mode         = VCT_MODE_TRANSCEIVER;
    config.session      = sSession;
    config.numSession   = 1;
    config.aid          = DWC_GetMyAID();
    config.callback     = VoiceChatEventCallback;
    config.userData     = NULL;
    config.audioBuffer     = gAudioBuffer;
    config.audioBufferSize = VCT_AUDIO_BUFFER_SIZE * VCT_DEFAULT_AUDIO_BUFFER_COUNT;

    SetupSound();

    if (!VCT_Init(&config)) {
        OS_TPrintf("ERROR: Can't initialize VoiceChat!!!\n");
        return GAME_MODE_CONNECTED;
    }

    //  デバッグモードでは短めに設定 ＆ APIの動作テスト
    //
#ifdef SDK_DEBUG
    VCT_SetTransceiverTimeout(15);
#endif

    //  コネクションクローズコールバックを再設定（returnSeqの再設定）
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);
    
    sCurrentSession = NULL;

    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
    
    //  サウンドループをスタート
    StartSoundLoop();

    while (1) {
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print(14, 0, "a:%d", DWC_GetMyAID());
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
        dbs_Print( 2, 2, "== Transceiver mode demo ==");

        ReadKeyData();
        DWC_ProcessFriendsMatch();
        
        if (DWC_GetLastError(NULL)){
            //  マッチング失敗時の処理
            ShutdownInet();
            DWC_ClearError();
            returnSeq = GAME_MODE_MAIN;
            break;
        }
        
        //  ピクチャーフレームに一度呼び出すメイン関数。
        VCT_Main();

        //  CloseCallbackによるreturnSeqの変更
        if (returnSeq != GAME_MODE_NUM) break;

        //  SELECTボタンでメニューを切り替え
        if (stKeyCnt.trg & PAD_BUTTON_SELECT) {
            sMenu->GoNext();
        }
        if (stKeyCnt.trg & PAD_BUTTON_L) {
            LButtonDown();
        }
        else if (stKeyCnt.release & PAD_BUTTON_L) {
            LButtonUp();
        }
        
        int key = sMenu->ShowMenu();
        
        //
        // メニューハンドラがキーを処理しない場合だけ、
        // ここにキーコードが帰ってきます
        //

        if (key & PAD_BUTTON_A) {
            switch (curIdx){
            case 0:  // Client
            case 1:  // Client
            case 2:  // Client
            case 3:  // Client
            case 4:  // Client
            case 5:  // Client
            case 6:  // Client
            case 7:  // Client
		        VCT_SetTransceiverMode(FALSE);
		        VCT_SetTransceiverServer((u8)curIdx);
                break;
            case 8:  // Server
		        u8* aidList;
		        int n = DWC_GetAIDList(&aidList);
		        VCT_SetTransceiverMode(TRUE);
		        VCT_SetTransceiverClients(aidList, n);
                break;
            case 9:  // Return
                returnSeq = GAME_MODE_CONNECTED;
                break;
            }
        }
        else if (key & PAD_KEY_UP) {
	        // 十字キー上でカーソル移動 
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
	        // メニューリスト再表示 
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (key & PAD_KEY_DOWN) {
	        // 十字キー下でカーソル移動 
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
	        // メニューリスト再表示 
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_BUTTON_DEBUG) {
            //  バックライトオフ（エミュレータノイズ対策用）
            (void)PM_SetBackLight( PM_LCD_ALL, PM_BACKLIGHT_OFF );
        }
        
        //  ビープ音を発生
        SendBeep(stKeyCnt.cont & PAD_BUTTON_R);
        
        ShowTicks(sCurrentSession ? StateName(sCurrentSession->state) : "No session");

        //  サウンドメイン
        NNS_SndMain();

        //  画面をアップデート
        dbs_DemoUpdate();

        //  IRQ待ち
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        dbs_DemoLoad();
        
        OS_CheckStack(OS_GetCurrentThread());

#ifdef SDK_PROFILE_FUNCTIONCOST
        __vct_cost_count();
        OS_CalcStatistics(&myStatBuffer);
#endif
    }
    //  セッションを削除 
    ClearScreen();

    //  サウンドループを停止 
    StopSoundLoop();
    TerminateSound();
    
    //  libVCT 終了処理 
    VCT_Cleanup();
#ifdef SDK_PROFILE_FUNCTIONCOST
    OS_DumpStatistics(&myStatBuffer);
#endif    

    BroadCastStatus(DEMO_STATUS_EXIT);
    
    DestroyMenu();
    DWC_SetReportLevel(DWC_REPORTFLAG_ALL);

    return returnSeq;
}

void TransceiverModeDispCallback(int curIdx, void *param)
{
#pragma unused(curIdx, param)
    dbs_CPrintf(NULL, "\n my AID=%d\n", DWC_GetMyAID());
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  メニュー関連 
//

static Menu* InitMenu()
{
    if (sMenu == NULL) {
        sMenu = new Menu(NULL);

        sMenu->AddItem(new AudioInfo);
        sMenu->AddItem(new VADInfo);

#ifdef SDK_DEBUG
        //
        //  デバッグ用メニューを追加 
        //
        sMenu->AddItem(new AudioDebug);
#endif
    }

    return sMenu;
}

static void DestroyMenu()
{
    delete sMenu;
    sMenu = NULL;
}
