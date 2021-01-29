//
//  libVCT 電話モードのデモアプリケーション
//

#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "dbs.h"
#include "utility.h"
#include "screen.h"

#include "phone.h"
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
static VCTSession sSession[2];

static app_info sAppInfo;

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
//  Ａボタンが押されたときの処理
//
static void AButtonDown(u8 aid)
{
	int ret;

    if (sAppInfo.session == NULL) {
        VCTSession *session = VCT_CreateSession(aid);
        if (session == NULL) {
            // セッションを使い切っている、またはaidが自分自身の場合
            // CraeteSession が失敗します
            OS_TPrintf("Can't create session!\n");
            return;
        }
        
        ret = VCT_Request(session, VCT_REQUEST_INVITE);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Can't request Invite [%d]\n", ret);
            VCT_DeleteSession(session);
        }
        else {
            sAppInfo.session = session;
            PlaySE(SE_RINGOUT);
        }
    }
    else {
        switch (sAppInfo.session->state) {
        case VCT_STATE_INCOMING:
            //  他の端末から会話要求があった場合、ここでOKを返してストリーミングを開始
            //
            ret = VCT_Response(sAppInfo.session, VCT_RESPONSE_OK);
            if (ret != VCT_ERROR_NONE) {
                OS_TPrintf("Can't send response OK [%d]\n", ret);
            }
            VCT_StartStreaming(sAppInfo.session);
            BroadCastStatus(DEMO_STATUS_TALKING);
            StopSE();
            break;

        default:
            //  それ以外は何もしない
            //
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  Ｂボタンが押されたときの処理
//

static void BButtonDown()
{
    int ret;

    if (sAppInfo.session == NULL) {
        //  現在のセッションがなければ何もしない
        //
        return;
    }
    
    switch (sAppInfo.session->state) {
    case VCT_STATE_INCOMING:
        //
        //  会話要求があった場合、BボタンでDecline
        //
        ret = VCT_Response(sAppInfo.session, VCT_RESPONSE_DECLINE);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Can't send response Decline [%d]\n", ret);
        }
        StopSE();
        ClearSession(sAppInfo.session);
        break;


    case VCT_STATE_OUTGOING:
        //
        //  こちらから会話要求中の場合はBボタンでキャンセル
        //
        ret = VCT_Request(sAppInfo.session, VCT_REQUEST_CANCEL);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Can't send request CANCEL [%d]\n", ret);
        }
        StopSE();
        ClearSession(sAppInfo.session);
        break;

    case VCT_STATE_TALKING:
        //
        //  接続中のときは会話終了(Bye)
        //
        ret = VCT_Request(sAppInfo.session, VCT_REQUEST_BYE);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Cna't send request BYE [%d]\n", ret);
        }
        //
        //  音声の送信は早めに止める
        //
        VCT_StopStreaming(sAppInfo.session);
        BroadCastStatus(DEMO_STATUS_PHONE);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  VoiceChatのイベントコールバック関数
//

static void VoiceChatEventCallback(u8 aid, VCTEvent event, VCTSession *session, void *data)
{
#pragma unused(aid, data)
    
    int ret;

    switch (event) {
    case VCT_EVENT_INCOMING:
        if (sAppInfo.session) {
            OS_TPrintf("Return busy to %d\n", session->aid);
            ret = VCT_Response(session, VCT_RESPONSE_BUSY_HERE);
            VCT_DeleteSession(session);
            break;
        }
            
        OS_TPrintf("Invite From %d\n", session->aid);
        PlaySE(SE_RINGIN);
        sAppInfo.session = session;
        break;

    case VCT_EVENT_RESPONDBYE:
        OS_TPrintf("Bye From %d\n", session->aid);
        ret = VCT_Response(session, VCT_RESPONSE_OK);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Can't send response Ok [%d]\n", ret);
        }
        ClearSession(session);
        break;

    case VCT_EVENT_DISCONNECTED:
        OS_TPrintf("Disconnected\n");
        ClearSession(session);
        break;

    case VCT_EVENT_CANCEL:
        OS_TPrintf("Cancel From %d\n", session->aid);
        ret = VCT_Response(session, VCT_RESPONSE_TERMINATED);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Can't send response RequestTerminated [%d]\n", ret);
        }
        StopSE();
        ClearSession(session);
        break;

    case VCT_EVENT_CONNECTED:
        OS_TPrintf("200 OK From %d\n", session->aid);
        VCT_StartStreaming(session);
        BroadCastStatus(DEMO_STATUS_TALKING);
        StopSE();
        break;

    case VCT_EVENT_REJECT:
        OS_TPrintf("Reject From %d\n", session->aid);
        ClearSession(session);
        StopSE();
        break;

    case VCT_EVENT_BUSY:
    case VCT_EVENT_TIMEOUT:
    case VCT_EVENT_ABORT:
        OS_TPrintf("Clear session by '%s'\n", EventName(event));
        StopSE();
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
    if (session == sAppInfo.session) {
        VCT_StopStreaming(session);
        VCT_DeleteSession(session);
        sAppInfo.session = NULL;
    }
    else {
        SDK_WARNING(FALSE, "What is this session??\n");
        VCT_DeleteSession(session);
    }
    sMenu->ChangeMenu(MENU_AUDIO_INFO);
    BroadCastStatus(DEMO_STATUS_PHONE);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  GAME_CONNECTED_MAIN から呼び出される関数
//
GameMode PhoneMain()
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_PHONE];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

    InitMenu();
    DWC_SetReportLevel(0);

#ifdef SDK_PROFILE_FUNCTIONCOST
    OS_InitFunctionCost(&myCostBuffer, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer, MYSTATBUFFER_SIZE);
#endif

	BroadCastStatus(DEMO_STATUS_PHONE);
    //
    //  libVCT を初期化
    //
    VCTConfig config;
    
    config.mode         = VCT_MODE_PHONE;
    config.session      = sSession;
    config.numSession   = 2;
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

    //  コネクションクローズコールバックを再設定（returnSeqの再設定） 
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);
    
    sAppInfo.session = NULL;

    UpdateMenuList(gameSeq);
    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
    
    //  サウンドループをスタート 
    StartSoundLoop();

    while (1) {
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print(14, 0, "a:%d", DWC_GetMyAID());
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
        dbs_Print( 5, 2, "== Phone mode demo ==");

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
        int key = sMenu->ShowMenu();
        
        //
        // メニューハンドラがキーを処理しない場合だけ、
        // ここにキーコードが帰ってきます
        //
        if (key & PAD_BUTTON_A) {
            if (curIdx == gameSeq->numMenu - 1) {
                returnSeq = GAME_MODE_CONNECTED;
            }
            else {
                AButtonDown((u8)curIdx);
            }
        }
        else if (key & PAD_BUTTON_B) {
            BButtonDown();
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
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
        
        ShowTicks(sAppInfo.session ? StateName(sAppInfo.session->state) : "No session");

        //  サウンドメイン 
        NNS_SndMain();

        //  画面をアップデート 
        dbs_DemoUpdate();

        //  IRQ待ち 
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);

        dbs_DemoLoad();
        //  スタック溢れチェック 
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

/////////////////////////////////////////////////////////////////////////////////////
//
//  メニュー関連 
//

static Menu* InitMenu()
{
    if (sMenu == NULL) {
        sMenu = new Menu(&sAppInfo);

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
