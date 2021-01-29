//
//  libVCT �J���t�@�����X���[�h�̃f���A�v���P�[�V���� 
//

#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "dbs.h"
#include "utility.h"
#include "screen.h"

#include "sound.h"
#include "vct.h"

#include "vct_menu.h"
#include "demo_util.h"
#include "demo_menu.h"

#ifdef SDK_DEBUG
#include "debug_menu.h"
#endif

#include "conference.h"

//
//  �L��ϐ� 
//
extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;
extern KeyControl stKeyCnt;

//
//  �ÓI�ϐ� 
//
static VCTSession sSession[3];
static VCTSession *sCurrentSession = NULL;

extern KeyControl stKeyCnt;
extern GameControl stGameCnt;

static Menu *sMenu = NULL;

static char        sClientMsg[MAX_PLAYERS][32] = {
    "Add aid: 0",
    "Add aid: 1",
    "Add aid: 2",
    "Add aid: 3",
    "Add aid: 4",
    "Add aid: 5",
    "Add aid: 6",
    "Add aid: 7",
};

//
//  �֐��v���g�^�C�v 
//
static void         ClearSession(VCTSession *session);
static Menu*        InitMenu();
static void         DestroyMenu();


/////////////////////////////////////////////////////////////////////////////////////
//
//  VoiceChat�̃C�x���g�R�[���o�b�N�֐� 
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
//  VoiceChat�̃C�x���g�R�[���o�b�N�֐� 
//
static void VoiceChatEventCallback(u8 aid, VCTEvent event, VCTSession *session, void *data)
{
#pragma unused(aid, data)
    
    switch (event) {
    case VCT_EVENT_DISCONNECTED:
        VCT_StopStreaming(session);
        OS_TPrintf("Disconnected\n");
        VCT_DeleteSession(session);
        sMenu->ChangeMenu(MENU_AUDIO_INFO);
        BroadCastStatus(DEMO_STATUS_CONFERENCE);
        break;

    case VCT_EVENT_CONNECTED:
        VCT_StartStreaming(session);
        BroadCastStatus(DEMO_STATUS_TALKING);
        OS_TPrintf("Connected\n");
        break;

    case VCT_EVENT_ABORT:
        OS_TPrintf("Session abrot\n");
        VCT_StopStreaming(session);
        VCT_DeleteSession(session);
        break;

    default:
        OS_TPrintf(" not handler (%s)\n", EventName(event));
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  GAME_CONNECTED_MAIN ����Ăяo�����֐� 
//
GameMode ConferenceMain()
{
    GameSequence    *gameSeq = &gameSeqList[GAME_MODE_CONFER];
    GameMode        returnSeq = GAME_MODE_NUM;
    int             curIdx = 0;
    u32             aidBitmap = 0;

    for (int i = 0; i < MAX_PLAYERS; ++i) {
        OS_SNPrintf(sClientMsg[i], 32, "Add aid: %d", i);
    }

    InitMenu();

#ifdef SDK_PROFILE_FUNCTIONCOST
    OS_InitFunctionCost(&myCostBuffer, MYCOSTBUFFER_SIZE);
    OS_InitStatistics(&myStatBuffer, MYSTATBUFFER_SIZE);
#endif

    BroadCastStatus(DEMO_STATUS_CONFERENCE);
    //
    //  VoiceChat SDK �������� 
    //
    VCTConfig config;

    config.mode         = VCT_MODE_CONFERENCE;
    config.session      = sSession;
    config.numSession   = 3;
    config.aid          = DWC_GetMyAID();
    config.callback     = VoiceChatEventCallback;
    config.userData     = NULL;
    config.audioBuffer     = gAudioBuffer;
    config.audioBufferSize = VCT_AUDIO_BUFFER_SIZE * VCT_DEFAULT_AUDIO_BUFFER_COUNT * VCT_MAX_AUDIO_STREAM;

    SetupSound();

    if (!VCT_Init(&config)) {
        OS_TPrintf("ERROR: Can't initialize VoiceChat!!!\n");
        return GAME_MODE_CONNECTED;
    }

    VCT_SetReportLevel(VCT_REPORTLEVEL_NONE);

    //  �R�l�N�V�����N���[�Y�R�[���o�b�N���Đݒ�ireturnSeq�̍Đݒ�j 
    DWC_SetConnectionClosedCallback(ConnectionClosedCallback, &returnSeq);
    
    sCurrentSession = NULL;

    DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
    
    //  �T�E���h���[�v���X�^�[�g 
    StartSoundLoop();

    OSTick tick1, tick2;
    //  �J���t�@�����X���J�n 
    while (1) {
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print(14, 0, "a:%d", DWC_GetMyAID());
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
        dbs_Print( 2, 2, "== Conference mode demo ==");

        ReadKeyData();

        int key = sMenu->ShowMenu();

        tick1 = OS_GetTick();
        DWC_ProcessFriendsMatch();
        tick1 = OS_GetTick() - tick1;
        
        if (DWC_GetLastError(NULL)){
             //  �}�b�`���O���s���̏��� 
            ShutdownInet();
            DWC_ClearError();
            returnSeq = GAME_MODE_MAIN;
            break;
        }
        
        //  �s�N�`���[�t���[���Ɉ�x�Ăяo�����C���֐��B
        tick2 = OS_GetTick();
        VCT_Main();
        tick2 = OS_GetTick() - tick2;

        //  CloseCallback�ɂ��returnSeq�̕ύX 
        if (returnSeq != GAME_MODE_NUM) break;

        //  SELECT�{�^���Ń��j���[��؂�ւ� 
        if (stKeyCnt.trg & PAD_BUTTON_SELECT) {
            sMenu->GoNext();
        }

        if (stKeyCnt.cont & PAD_BUTTON_L) {
            OS_TPrintf("%lld, %lld\n", tick1, tick2);
        }
        
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
                if (aidBitmap & (1 << curIdx)) {
                    VCT_RemoveConferenceClient((u8)curIdx);
                    aidBitmap &= ~(1 << curIdx);
                    OS_SNPrintf(sClientMsg[curIdx], 32, "Add aid: %d", curIdx);
                    gameSeq->menuList[curIdx].msg = (const char*)sClientMsg[curIdx];
                }
                else {
                    VCT_AddConferenceClient((u8)curIdx);
                    aidBitmap |= (1 << curIdx);
                    OS_SNPrintf(sClientMsg[curIdx], 32, "Remove aid: %d", curIdx);
                    gameSeq->menuList[curIdx].msg = (const char*)sClientMsg[curIdx];
                }
                
                //  �Q���l���ɍ��킹�ăR�[�f�b�N��ύX 
                //
                int n = MATH_CountPopulation(aidBitmap);
                if (n < 3) {
                    VCT_SetCodec(VCT_CODEC_4BIT_ADPCM);
                }
                else {
                    VCT_SetCodec(VCT_CODEC_3BIT_ADPCM);
                }
                
                DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
                break;
                
            case 8:  // Return
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
        
        //  �T�E���h���C��
        NNS_SndMain();

        //  ��ʂ��A�b�v�f�[�g
        ShowTicks(sCurrentSession ? StateName(sCurrentSession->state) : "No session");
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

void ConferenceModeDispCallback(int curIdx, void *param)
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
