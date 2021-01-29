//
//  libVCT �d�b���[�h�̃f���A�v���P�[�V����
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
//  �L��ϐ�
//
extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;
extern KeyControl stKeyCnt;

//
//  �ÓI�ϐ�
//
static VCTSession sSession[2];

static app_info sAppInfo;

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
//  �`�{�^���������ꂽ�Ƃ��̏���
//
static void AButtonDown(u8 aid)
{
	int ret;

    if (sAppInfo.session == NULL) {
        VCTSession *session = VCT_CreateSession(aid);
        if (session == NULL) {
            // �Z�b�V�������g���؂��Ă���A�܂���aid���������g�̏ꍇ
            // CraeteSession �����s���܂�
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
            //  ���̒[�������b�v�����������ꍇ�A������OK��Ԃ��ăX�g���[�~���O���J�n
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
            //  ����ȊO�͉������Ȃ�
            //
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  �a�{�^���������ꂽ�Ƃ��̏���
//

static void BButtonDown()
{
    int ret;

    if (sAppInfo.session == NULL) {
        //  ���݂̃Z�b�V�������Ȃ���Ή������Ȃ�
        //
        return;
    }
    
    switch (sAppInfo.session->state) {
    case VCT_STATE_INCOMING:
        //
        //  ��b�v�����������ꍇ�AB�{�^����Decline
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
        //  �����炩���b�v�����̏ꍇ��B�{�^���ŃL�����Z��
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
        //  �ڑ����̂Ƃ��͉�b�I��(Bye)
        //
        ret = VCT_Request(sAppInfo.session, VCT_REQUEST_BYE);
        if (ret != VCT_ERROR_NONE) {
            OS_TPrintf("Cna't send request BYE [%d]\n", ret);
        }
        //
        //  �����̑��M�͑��߂Ɏ~�߂�
        //
        VCT_StopStreaming(sAppInfo.session);
        BroadCastStatus(DEMO_STATUS_PHONE);
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  VoiceChat�̃C�x���g�R�[���o�b�N�֐�
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
//  �Z�b�V�����̃N���A
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
//  GAME_CONNECTED_MAIN ����Ăяo�����֐�
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
    //  libVCT ��������
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

    //  �R�l�N�V�����N���[�Y�R�[���o�b�N���Đݒ�ireturnSeq�̍Đݒ�j 
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);
    
    sAppInfo.session = NULL;

    UpdateMenuList(gameSeq);
    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
    
    //  �T�E���h���[�v���X�^�[�g 
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
        int key = sMenu->ShowMenu();
        
        //
        // ���j���[�n���h�����L�[���������Ȃ��ꍇ�����A
        // �����ɃL�[�R�[�h���A���Ă��܂�
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
        
        ShowTicks(sAppInfo.session ? StateName(sAppInfo.session->state) : "No session");

        //  �T�E���h���C�� 
        NNS_SndMain();

        //  ��ʂ��A�b�v�f�[�g 
        dbs_DemoUpdate();

        //  IRQ�҂� 
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);

        dbs_DemoLoad();
        //  �X�^�b�N���`�F�b�N 
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

/////////////////////////////////////////////////////////////////////////////////////
//
//  ���j���[�֘A 
//

static Menu* InitMenu()
{
    if (sMenu == NULL) {
        sMenu = new Menu(&sAppInfo);

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
