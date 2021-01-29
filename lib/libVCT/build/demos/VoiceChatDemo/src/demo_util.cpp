#include <nitro.h>

#include "vct.h"

#include "vct_menu.h"
#include "demo_util.h"
#include "main.h"
#include "thread_statistics.h"

extern GameControl stGameCnt;
//  CPU���׌v���p 
static u64 sAudioTick = 0;
static int sAudioCount = 0;

#pragma profile off

//
//  SDK�̃X�e�[�g�}�V���̖��O 
//
static char * state_name[] = {
    "Init         ",
    "OutGoing     ",
    "Talking      ",
    "Connected    ",
    "Incoming     ",
    "Disconnecting",
};

//
//  �C�x���g�̖��O�i�\���p�j 
//
static char *event_name[] = {
    "None",
    "Incoming",
    "Reject",
    "Busy",
    "Cancel",
    "Notify Free",
    "Notify Busy",
    "Connected",
    "Respondbye",
    "Disconnected",
    "Contact",
    "Timeout",
    "Abort",
};

const char * _demo_status_str[NUM_OF_DEMO_STATUS] = {
    "x",
    "ph",
    "tr",
    "conf",
    "talk",
    "alive",
    "exit"
};

static char        sClientMsg[MAX_PLAYERS][32] = {
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

void AddAudioTick(u64 tick)
{
    sAudioTick += tick;
    ++sAudioCount;
}

const char* EventName(VCTEvent event)
{
    return event_name[event];
}

const char* StateName(VCTState state)
{
    return state_name[state];
    
}

void ShowTicks(const char *state_name)
{
    static OSTick prev = 0;
    static int flag = 0;
    static int count  = 0;
    OSTick now = OS_GetTick();

    u64 diff = now - prev;

    if (diff > OS_SecondsToTicks(1)) {
        if (flag) {
            PutMainScreen(31, 0, 4, "*");
        }
        else {
            PutMainScreen(31, 0, 4, " ");
        }
        flag = !flag;
        prev = now;

        //  ���C���֐���SNDCallback���ŏ����ticks��CPU���ׂ�\�� 
        u64 result[3];
        GetThreadTicks(result);
        for (int i = 0; i < 3; ++i) {
            result[i] = (result[i] * 1000) / diff;
            if (result[i] > 1000) {
                result[i] = 1000;
            }
        }

        sAudioTick = (sAudioCount) ? sAudioTick / sAudioCount : 0;

        PutMainScreen(8, 22, 18, "%4d / %2lld.%1lld%% %2lld.%1lld%% %2lld.%1lld%%",
                      (u32)sAudioTick,
                      result[0] / 10, result[0] % 10,
                      result[1] / 10, result[1] % 10,
                      result[2] / 10, result[2] % 10);
        sAudioTick = 0;
        sAudioCount = 0;
    }

    if (state_name) {
        PutMainScreen(1, 23, 15, (char*)state_name);
    }
    else {
        PutMainScreen(1, 23, 15, "-No session- ");
    }

}
static u8 sSendBuffer[512];

void ClearMenuList()
{
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        sClientMsg[i][0] = '\0';
    }
}

void UpdateClientInfo(u8 aid, client_info *info)
{
    OS_SNPrintf(sClientMsg[aid], 32, "aid: %d / pid: %d (%s)", aid, info->profileID,
             _demo_status_str[info->status]);
}

void UpdateMenuList(GameSequence *seq)
{
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        seq->menuList[i].msg = (const char*)sClientMsg[i];
    }
}

//
//  �����̏���S���ɑ��M 
//
void BroadCastStatus(demo_status status)
{
    u8* pAidList;
    client_info info;
    int numHost;
    int i;

    info.magic     = DEMO_EXCHANGE_MAGIC_TOKEN;
    info.profileID = stGameCnt.userData.profileID;
    info.status    = status;

    numHost = DWC_GetAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == DWC_GetMyAID())
        {
            continue;
        }

        DWC_SendReliable(pAidList[i], &info, sizeof(info));
    }
}

/*---------------------------------------------------------------------------*
   DWC�R�l�N�V�����N���[�Y�R�[���o�b�N�֐� 
 *---------------------------------------------------------------------------*/
void ConnectionClosedCallback(DWCError error,
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
            // �T�[�o�N���C�A���g�^�}�b�`���O�ŁA�������N���C�A���g�̎��ɁA
            // �T�[�o���R�l�N�V�������N���[�Y�����B
            // �T�[�o���N���[�Y�����̂Ŏ������N���[�Y����B
            DWC_CloseAllConnectionsHard();
        }
        
        if (DWC_GetNumConnectionHost() == 1){
            //  �S�ẴR�l�N�V�������N���[�Y���ꂽ�ꍇ 
            if (isLocal){
                // ��������N���[�Y�����ꍇ�͑S���Ƃ̃R�l�N�V������
                // �N���[�Y�ł����玟�ɐi��
                stGameCnt.blocking = FALSE;  // �u���b�L���O���� 
            }
            if ((DWC_GetLastMatchingType() != DWC_MATCH_TYPE_SC_SV) || isServer){
                // isServer==TRUE�Ŏ����ŃN���[�Y�����ꍇ�ƁA
                // ���肩��̃N���[�Y�̏ꍇ�́A�����Ŏ��̃��[�h���Z�b�g����
                *(GameMode *)param = GAME_MODE_LOGIN;
            }
            // �������T�[�o�N���C�A���g�^�}�b�`���O�̃T�[�o�ŁA
            // �N���C�A���g���S���N���[�Y�����ꍇ�͉������Ȃ�
        }
        
        // ���������[�U�[���͋󔒂ɂ���
	    OS_SNPrintf(sClientMsg[aid], 32, "");		

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

 // �u���b�L���O����
        stGameCnt.blocking = FALSE; 
    }
}

