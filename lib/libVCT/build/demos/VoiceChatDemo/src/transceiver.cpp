//
//  libVCT �g�����V�[�o�[���[�h�̃f���A�v���P�[�V���� 
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
//  �L��ϐ� 
//
extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;
extern KeyControl stKeyCnt;

//
//  �ÓI�ϐ� 
//
static VCTSession sSession[1];
static VCTSession *sCurrentSession = NULL;

extern KeyControl stKeyCnt;
extern GameControl stGameCnt;

static Menu *sMenu = NULL;

//
//  �֐��v���g�^�C�v 
//
static void         ClearSession(VCTSession *session);
static Menu*        InitMenu();
static void         DestroyMenu();

/////////////////////////////////////////////////////////////////////////////////////
//
//  �֐��R�X�g�v���p 
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
//  �k�{�^���������ꂽ�Ƃ��̏��� 
//
static void LButtonDown()
{
    VCTSession *session;
	int ret;

    ret = VCT_Contact(&session);
    switch (ret) {
    case VCT_ERROR_TRANSCEIVER_BUSY:
        //  �g�����V�[�o�[���[�h�ŁA���ɒN������b���̏ꍇ VCT_ERROR_TRANSCEIVER_BUSY���A��܂� 
        //
        OS_TPrintf("Transceiver is busy\n");
        PlaySE(SE_BUSY);
        break;

    case VCT_ERROR_SEND_FAIL:
        //  ���M���s�̍ۂ́A�Z�b�V�������폜���Ă������� 
        //
        VCT_DeleteSession(session);
        break;

    case VCT_ERROR_NONE:
        //  �G���[�Ȃ� 
        sCurrentSession = session;
        break;

    default:
        break;
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  �k�{�^���������ꂽ�Ƃ��̏��� 
//
static void LButtonUp()
{
    int ret;

    if (sCurrentSession) {
        //
        // �܂��X�g���[�~���O���~���A���̌�Release�𔭍s���܂��B
        // �i�T�[�o�[�̉������x�ꂽ�肵���ꍇ�ɁA�������X�g���[�~���O��������
        // �̂�h�����߁j�B
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
//  VoiceChat�̃C�x���g�R�[���o�b�N�֐�
//
static void VoiceChatEventCallback(u8 aid, VCTEvent event, VCTSession *session, void *data)
{
#pragma unused(aid, data)
    
    switch (event) {
    //
    //  �T�[�o�[�̃C�x���g
    //
    case VCT_EVENT_CONTACT:
        //
        //  Contact�ɑ΂��鉞����libVCT�����Ŋ��ɍs���Ă��܂��B
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
    //  �T�[�o�[�E�N���C�A���g���ʂ̃C�x���g
    //
    case VCT_EVENT_DISCONNECTED:
        VCT_StopStreaming(session);
        OS_TPrintf("Disconnected\n");
        ClearSession(session);
        PlaySE(SE_NOISE);
        break;

    //
    //  �N���C�A���g�̃C�x���g
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
//  �Z�b�V�����̃N���A
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
//  GAME_CONNECTED_MAIN ����Ăяo�����֐�
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
    //  VoiceChat SDK ��������
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

    //  �f�o�b�O���[�h�ł͒Z�߂ɐݒ� �� API�̓���e�X�g
    //
#ifdef SDK_DEBUG
    VCT_SetTransceiverTimeout(15);
#endif

    //  �R�l�N�V�����N���[�Y�R�[���o�b�N���Đݒ�ireturnSeq�̍Đݒ�j
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);
    
    sCurrentSession = NULL;

    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
    
    //  �T�E���h���[�v���X�^�[�g
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
            //  �}�b�`���O���s���̏���
            ShutdownInet();
            DWC_ClearError();
            returnSeq = GAME_MODE_MAIN;
            break;
        }
        
        //  �s�N�`���[�t���[���Ɉ�x�Ăяo�����C���֐��B
        VCT_Main();

        //  CloseCallback�ɂ��returnSeq�̕ύX
        if (returnSeq != GAME_MODE_NUM) break;

        //  SELECT�{�^���Ń��j���[��؂�ւ�
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
        // ���j���[�n���h�����L�[���������Ȃ��ꍇ�����A
        // �����ɃL�[�R�[�h���A���Ă��܂�
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
	        // �\���L�[��ŃJ�[�\���ړ� 
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
	        // ���j���[���X�g�ĕ\�� 
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (key & PAD_KEY_DOWN) {
	        // �\���L�[���ŃJ�[�\���ړ� 
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
	        // ���j���[���X�g�ĕ\�� 
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_BUTTON_DEBUG) {
            //  �o�b�N���C�g�I�t�i�G�~�����[�^�m�C�Y�΍��p�j
            (void)PM_SetBackLight( PM_LCD_ALL, PM_BACKLIGHT_OFF );
        }
        
        //  �r�[�v���𔭐�
        SendBeep(stKeyCnt.cont & PAD_BUTTON_R);
        
        ShowTicks(sCurrentSession ? StateName(sCurrentSession->state) : "No session");

        //  �T�E���h���C��
        NNS_SndMain();

        //  ��ʂ��A�b�v�f�[�g
        dbs_DemoUpdate();

        //  IRQ�҂�
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        dbs_DemoLoad();
        
        OS_CheckStack(OS_GetCurrentThread());

#ifdef SDK_PROFILE_FUNCTIONCOST
        __vct_cost_count();
        OS_CalcStatistics(&myStatBuffer);
#endif
    }
    //  �Z�b�V�������폜 
    ClearScreen();

    //  �T�E���h���[�v���~ 
    StopSoundLoop();
    TerminateSound();
    
    //  libVCT �I������ 
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
//  ���j���[�֘A 
//

static Menu* InitMenu()
{
    if (sMenu == NULL) {
        sMenu = new Menu(NULL);

        sMenu->AddItem(new AudioInfo);
        sMenu->AddItem(new VADInfo);

#ifdef SDK_DEBUG
        //
        //  �f�o�b�O�p���j���[��ǉ� 
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
