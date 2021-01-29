#include <nitro.h>
#include <base/dwc_base_gamespy.h>
#include <natneg/natneg.h>
#include <serverbrowsing/sb_internal.h>
#include <base/dwc_report.h>
#include <base/dwc_account.h>
#include <base/dwc_error.h>
#include <base/dwc_memfunc.h>
#include <base/dwc_login.h>
#include <base/dwc_friend.h>
#include <base/dwc_match.h>
#include <base/dwc_transport.h>
#include <base/dwc_main.h>
#include <base/dwc_common.h>

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
// �}�b�`���C�N�R�}���h�ǉ����M�f�[�^�ő�T�C�Y�ƍő�v�f��
#define DWC_MATCH_COMMAND_ADD_MESSAGE_MAX 512
#define DWC_MATCH_COMMAND_ADD_MESSAGE_NUM (DWC_MATCH_COMMAND_ADD_MESSAGE_MAX >> 2)

// [arakit] main 051024
// �}�b�`���C�N�\�񋑔ۗ��R
#define DWC_MATCH_RESV_DENY_REASON_SV_FULL 0x10  // �T�[�o������I�[�o�[


//----------------------------------------------------------------------------
// enum
//----------------------------------------------------------------------------
// �}�b�`���C�N�L�����Z����ԗ񋓎q
enum {
    DWC_MATCH_CANCEL_STATE_INIT = 0,  // �������
    DWC_MATCH_CANCEL_STATE_EXEC,      // �L�����Z���������s���i�L�����Z�����j
    DWC_MATCH_CANCEL_STATE_NUM
};

// �}�b�`���C�N�N���[�Y��ԗ񋓎q
enum {
    DWC_MATCH_CLOSE_STATE_INIT = 0,          // �������
    DWC_MATCH_CLOSE_STATE_CHANGE_TO_CLIENT,  // �N���C�A���g�ւ̓]���̂��߃N���[�Y
    DWC_MATCH_CLOSE_STATE_TIMEOUT,           // ���������z�X�g�����m�������߃N���[�Y
    DWC_MATCH_CLOSE_STATE_FORCED,            // ���z�X�g����N���[�Y���ꂽ���ߎc��z�X�g���N���[�Y
    DWC_MATCH_CLOSE_STATE_NUM
};

// DWCi_PostProcessConnection()�ɓn�������̎��
typedef enum {
    DWC_PP_CONNECTION_SV_CONNECT = 0,     // �T�[�o���g��gt2Connect�����A�܂��̓N���C�A���g���m�̃R�l�N�V��������
    DWC_PP_CONNECTION_CL_GT2_CONNECT,     // �ڑ��ς݃N���C�A���g��gt2Connect����
    DWC_PP_CONNECTION_CL_GT2_ACCEPT,      // �V�K�ڑ��N���C�A���g��gt2Accept����
    DWC_PP_CONNECTION_CL_FINISH_CONNECT,  // �V�K�ڑ��N���C�A���g�̐ڑ��ς݃N���C�A���g�ւ̐ڑ��S�Ċ���
    DWC_PP_CONNECTION_SYN_FINISH,         // �}�b�`���C�N�������������I��
    DWC_PP_CONNECTION_NUM
} DWCMatchPpConnectionType;

// �}�b�`���C�N����p�����[�^�̃��Z�b�g���x���BDWCi_ResetMatchParam()�ɓn��
typedef enum {
    DWC_MATCH_RESET_ALL = 0,   // �}�b�`���C�N������������
    DWC_MATCH_RESET_RESTART,   // �ŏ�����}�b�`���C�N����蒼��
    DWC_MATCH_RESET_CONTINUE,  // �����������������Ń}�b�`���C�N�p��
    DWC_MATCH_RESET_NUM
} DWCMatchResetLevel;


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
// �}�b�`���C�N����I�u�W�F�N�g�ւ̃|�C���^
// �\�[�X�̉��� static DWCMatchControl* stpMatchCnt = NULL;

// SB�T�[�o�A�b�v�f�[�g�p�t�B���^�ւ̃|�C���^
static char* stpAddFilter = NULL;

// �w��l���ȉ��ł��}�b�`���C�N����������I�v�V�����̐���\���̂ւ̃|�C���^
static DWCMatchOptMinCompleteIn* stpOptMinComp = NULL;

// �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�P��}�b�`���C�N������ɑ��̐ڑ���
// �󂯕t���Ȃ�����I�v�V�����̐���\����
static DWCMatchOptSCBlock stOptSCBlock;

// �Q�[����`��QR2�p�L�[�f�[�^�z��
// [todo]
// �Q�[���ŕK�v�ȕ��������������m�ۂ��Ă��炤�悤�ɕύX����
static DWCGameMatchKeyData stGameMatchKeys[DWC_QR2_GAME_RESERVED_KEYS];

// �T�[�o�]�����s�Ȃ��ꍇ�̊e�T�[�o�̑I�΂�₷���̊���
static const stEvalRate[DWC_SB_UPDATE_MAX_SERVERS] = { 3, 3, 2, 2, 1, 1, };
// �T�[�o�[�u���E�U�[�I�u�W�F�N�g����x���̂��߂̑Ώ�
static int s_sbCallbackLevel = 0;
static BOOL s_needSbFree = FALSE;

// NN�I�u�W�F�N�g����x���̂��߂̑Ώ�
static BOOL s_bInNNThink = FALSE;
static BOOL s_bNNFreeNegotiateList = FALSE;

void DWCi_NNFreeNegotiateList()
{
    if(s_bInNNThink == FALSE)
    {
        NNFreeNegotiateList();
    }
    else
    {
        s_bNNFreeNegotiateList = TRUE;
    }
}

#if 0
// for test
int DWCi_connect_errorcount = 0;
#endif

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void DWCi_ResetMatchParam(DWCMatchResetLevel level);
static void DWCi_SetMatchCommonParam(u8  matchType,
                                     u8  numEntry,
                                     DWCMatchedSCCallback callback,
                                     void* param);

static void DWCi_CloseMatching(void);

static SBError DWCi_SBUpdateAsync(int profileID);
static int DWCi_GetDefaultMatchFilter(char* filter, int profileID, u8 numEntry, u8 matchType);

static NegotiateError DWCi_NNStartupAsync(int isQR2, int cookie, SBServer server);
static NegotiateError DWCi_DoNatNegotiationAsync(DWCNNInfo* nnInfo);

static int  DWCi_SendMatchCommand(u8 command, int profileID, u32 ip, u16 port, const u32 data[], int len);
static SBError DWCi_SendSBMsgCommand(u8 command, u32 ip, u16 port, const u32 data[], int len);
static GPResult DWCi_SendGPBuddyMsgCommand(GPConnection* connection, u8 command, int profileID, const char* message);
static int  DWCi_GetGPBuddyAdditionalMsg(char* dstMsg, const char* srcMsg, int index);
static void DWCi_StopResendingMatchCommand(void);

static BOOL DWCi_ProcessRecvMatchCommand(u8  command,
                                         int srcPid,
                                         u32 srcIP,
                                         u16 srcPort,
                                         const u32 data[],
                                         int len);
static u8   DWCi_CheckResvCommand(int profileID, u32 qr2IP, u16 qr2Port, u32 matchType, BOOL priorFlag);
static int  DWCi_ProcessResvOK(int profileID, u32 ip, u16 port);
static void DWCi_MakeBackupServerData(int profileID, const u32 data[]);

static int  DWCi_HandleMatchCommandError(int error);
static int  DWCi_SendResvCommand(int profileID, BOOL delay);
static int  DWCi_SendResvCommandToFriend(BOOL delay, BOOL init, int resendPid);
static BOOL DWCi_RetryReserving(int resendPid);

static int  DWCi_CancelReservation(int profileID);
static BOOL DWCi_CancelPreConnectedServerProcess(int clientPid);
static BOOL DWCi_CancelPreConnectedClientProcess(int serverPid);

static int  DWCi_ChangeToClient(void);

static void DWCi_PostProcessConnection(DWCMatchPpConnectionType type);

static BOOL DWCi_AreAllBuddies(const u32 pidList[], u32 pidListLen);

static void DWCi_DoCancelMatching(void);
static void DWCi_FinishCancelMatching(void);
static int  DWCi_InvalidateReservation(void);
static void DWCi_RestartFromCancel(DWCMatchResetLevel level);
static void DWCi_RestartFromTimeout(void);
static int  DWCi_ResumeMatching(void);
static BOOL DWCi_CloseCancelHostAsync(int profileID);
static void DWCi_CloseAllConnectionsByTimeout(void);
static BOOL DWCi_CloseShutdownClientSC(u32 bitmap);

static void DWCi_SendMatchSynPacket(u8 aid, u16 type);
static BOOL DWCi_ProcessMatchSynTimeout(void);

static BOOL DWCi_SendCancelMatchSynCommand(int profileID, u8 command);
static BOOL DWCi_ProcessCancelMatchSynCommand(int profileID, u8 command, u32 data);
static BOOL DWCi_ProcessCancelMatchSynTimeout(void);

static u8   DWCi_GetNewMatchKey(void);
static void DWCi_ClearGameMatchKeys(void);

static u8   DWCi_GetAIDFromList(void);
static BOOL DWCi_IsFriendByIdxList(int profileID);
static int  DWCi_CheckDWCServer(SBServer server);

static int  DWCi_GetPidListIndex(int profileID, BOOL idx0);
static u8   DWCi_GetAIDFromProfileID(int profileID, BOOL idx0);
static u32  DWCi_GetAIDBitmask(BOOL valid);

static void DWCi_InitClWaitTimeout(void);

static void DWCi_InitOptMinCompParam(BOOL reset);
static void DWCi_ProcessOptMinComp(void);

static GPResult DWCi_HandleGPError(GPResult result);
static SBError DWCi_HandleSBError(SBError error);
static qr2_error_t DWCi_HandleQR2Error(qr2_error_t error);
static NegotiateError DWCi_HandleNNError(NegotiateError error);
static NegotiateResult DWCi_HandleNNResult(NegotiateResult result);
static GT2Result DWCi_HandleGT2Error(GT2Result result);

static void DWCi_SBCallback(ServerBrowser sb, SBCallbackReason reason, SBServer server, void *instance);
static void DWCi_SBPrintServerData(SBServer server);
static BOOL DWCi_EvaluateServers(BOOL sort);
static void DWCi_RandomizeServers(void);

static void DWCi_QR2ServerKeyCallback(int keyid, qr2_buffer_t outbuf, void *userdata);
static void DWCi_QR2PlayerKeyCallback(int keyid, int index, qr2_buffer_t outbuf, void *userdata);
static void DWCi_QR2TeamKeyCallback(int keyid, int index, qr2_buffer_t outbuf, void *userdata);
static void DWCi_QR2KeyListCallback(qr2_key_type keytype, qr2_keybuffer_t keybuffer, void *userdata);
static int DWCi_QR2CountCallback(qr2_key_type keytype, void *userdata);
static void DWCi_QR2AddErrorCallback(qr2_error_t error, gsi_char *errmsg, void *userdata);
static void DWCi_QR2PublicAddrCallback(unsigned int ip, unsigned short port, void* userdata);
static void DWCi_QR2NatnegCallback(int cookie, void *userdata);
static void DWCi_QR2ClientMsgCallback(gsi_char* data, int len, void* userdata);

static void DWCi_NNProgressCallback(NegotiateState state, void* userdata);
static void DWCi_NNCompletedCallback(NegotiateResult result, SOCKET gamesocket, struct sockaddr_in* remoteaddr, void* userdata);
static BOOL DWCi_ProcessNNFailure(BOOL ignoreError);

static void DWCi_SetMatchCnt(DWCMatchControl *pCnt);
static DWCMatchControl *DWCi_GetMatchCnt(void);
static void DWCi_SetMatchStatus(DWCMatchState state);

//#define NITRODWC_DEBUG

#ifdef NITRODWC_DEBUG
static void disp_time(void);
static void disp_match_state(DWCMatchState old, DWCMatchState now);
static const char *get_dwccommandstr(u8 command);
#endif
//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
/*---------------------------------------------------------------------------*
  �}�b�`���C�N�X�e�[�^�X�̃}�X�^�[�T�[�o�o�^�֐��i�F�B�w��A���w��s�A�}�b�`���C�N�p�j
  �����@�F�Ȃ�
  �߂�l�FTRUE :�}�X�^�[�T�[�o�Ƀ}�b�`���C�N�X�e�[�^�X���o�^���ꂽ�A
          FALSE:���O�C���O�A�������̓G���[�̂��ߓo�^����Ȃ�����
  �p�r�@�F���z�X�g�̃}�b�`���C�N�X�e�[�^�X���}�X�^�[�T�[�o�ɓo�^����B
          ���̊֐����Ă΂�Ă���ŒZ15�b���ŁA���z�X�g���玩�z�X�g�̏�Ԃ�
          ������悤�ɂȂ�A�}�b�`���C�N��i�߂���悤�ɂȂ�B
          �T�[�o�N���C�A���g�}�b�`���C�N�ȊO�ł́A�}�b�`���C�N���I��������_��
          �}�X�^�[�T�[�o���炱�̃}�b�`���C�N�X�e�[�^�X�����������B
          �}�b�`���C�N�J�n�֐����Ăׂ΁A���C�u�������ł��̊֐����Ă΂�邪�A
          �}�b�`���C�N�̏I���𑁂߂�ׂɁA�\�߃}�b�`���C�N�X�e�[�^�X��
          �}�X�^�[�T�[�o�ɓo�^���Ă��������ꍇ�́A���̊֐����Ăяo���΂悢�B
          �܂��A�}�b�`���C�N�X�e�[�^�X��DWC_ShutdownFriendsMatch()���Ă񂾎��ɂ�
          ���������B
 *---------------------------------------------------------------------------*/
BOOL DWC_RegisterMatchingStatus(void)
{

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_RegisterMatchingStatus() was called!!\n");

    if ((DWCi_GetMatchCnt() == NULL) || (DWCi_GetMatchCnt()->profileID == 0) || DWCi_IsError()){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

    if (DWCi_QR2Startup(DWCi_GetMatchCnt()->profileID)) return FALSE;
    return TRUE;
}
#endif


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����L�����Z���֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�L�����Z�����������s�����AFALSE:�}�b�`���C�N���łȂ�
  �p�r�@�F�i�s���̃}�b�`���C�N�������L�����Z������B
          �L�����Z�������͖{�֐����Ŋ������A�}�b�`���C�N�����R�[���o�b�N��
          �Ăяo�����B
 *---------------------------------------------------------------------------*/
BOOL DWC_CancelMatching(void)
{

    if (DWC_IsValidCancelMatching()){
        // �L�����Z���������s��
        DWCi_DoCancelMatching();
        return TRUE;
    }
    else {
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Now unable to cancel.\n");
        return FALSE;
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����L�����Z���֐� �񓯊��� (obsolete function)
  �����@�F�Ȃ�
  �߂�l�FTRUE:�L�����Z�������̎��s���J�n�����AFALSE:�}�b�`���C�N���łȂ�
  �p�r�@�F�i�s���̃}�b�`���C�N�������L�����Z������B
          �K�v�ȃL�����Z���������s������A�}�b�`���C�N�����R�[���o�b�N��
          �Ăяo�����B
          ��DWC_CancelMatching()���g���ĉ�����
 *---------------------------------------------------------------------------*/
BOOL DWC_CancelMatchingAsync(void)
{

    if (DWC_IsValidCancelMatching()){
        // �L�����Z�������̗\���Ԃɐi��
        DWCi_DoCancelMatching();
        return TRUE;
    }
    else {
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Now unable to cancel.\n");
        return FALSE;
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�L�����Z���L���m�F�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�L�����Z���\�AFALSE:�}�b�`���C�N���łȂ�������A���f�ł��Ȃ�
          �󋵂ŃL�����Z���s�\�B
  �p�r�@�F�i�s���̃}�b�`���C�N�������L�����Z���ł��邩�ǂ����𒲂ׂ�
 *---------------------------------------------------------------------------*/
BOOL DWC_IsValidCancelMatching(void)
{

    if (DWCi_IsError()) return FALSE;

    if (!DWCi_GetMatchCnt() ||
        (DWC_GetState() != DWC_STATE_MATCHING) ||
        (DWCi_GetMatchCnt()->cancelState != DWC_MATCH_CANCEL_STATE_INIT)){
        return FALSE;
    }
    else {
        return TRUE;
    }
}


#ifdef DWC_STOP_SC_SERVER
/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N �T�[�o���ߐ؂�֐�
  �����@�Fcallback ���ߐ؂芮���R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�}�b�`���C�N�̒��ߐ؂�J�n�B����������R�[���o�b�N���Ԃ��Ă���B
          FALSE:�G���[�����AFriendsMatch���C�u�����񓮍쒆�Ȃǂ̗��R�Œ��ߐ؂莸�s�B
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�̃}�b�`���C�N����ߐ؂�A�Ȍ�V�K��
          �ڑ����󂯕t���Ȃ��悤�ɂ���B
 *---------------------------------------------------------------------------*/
BOOL DWC_StopSCMatchingAsync(DWCStopSCCallback callback, void* param)
{

    if (DWCi_IsError() || !DWCi_GetMatchCnt() ||
        (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_SV) ||
        ((DWC_GetState() != DWC_STATE_MATCHING) && (DWC_GetState() != DWC_STATE_CONNECTED)))
        return FALSE;

    if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_WAITING){
        // �}�b�`���C�N���łȂ���΍ő�ڑ��l�������ݐڑ��l���ɍ��킹�邾����
        // �Ȍ�̐ڑ������ۂł���
        DWCi_GetMatchCnt()->qr2NumEntry = DWCi_GetMatchCnt()->gt2NumConnection;
        (void)DWCi_GPSetServerStatus();

        if (callback) callback(param);  // �R�[���o�b�N�Ăяo��
    }
    else {
        DWCi_GetMatchCnt()->stopSCCallback = callback;
        DWCi_GetMatchCnt()->stopSCParam    = param;
        DWCi_GetMatchCnt()->stopSCFlag     = 1;
    }

    return TRUE;
}
#endif


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�ǉ��֐� int��
  �����@�FkeyID        �L�[ID���w�肵�ăZ�b�g�������ꍇ�͂����ɃL�[ID��n���B
                       �V�K��keyID���擾��������� DWC_QR2_GAME_KEY_START
                       �ȉ��̒l�i0�Ȃǁj���Z�b�g����Ηǂ��B
                       ���Ɏg�p���̒ǉ��L�[ID���w�肵���ꍇ�́A�g�p���̂��̂�
                       ����keyString��n����valueSrc���Z�b�g���������A
                       �ႤkeyString��n�����ꍇ�͉������Ȃ��B
          keyString    �L�[���ʂ̂��߂̕�����ւ̃|�C���^�B
                       ������̓R�s�[���ă��C�u�������ŕێ����܂��B
          valueSrc     �L�[�ɑΉ�����l�ւ̃|�C���^�ŁA�}�X�^�[�T�[�o�����
                       �|�[�����O������ƁA���̃|�C���^����l���Q�Ƃ���
                       �}�X�^�[�T�[�o�ɒl���A�b�v����B
  �߂�l�F�ǉ������L�[�̃L�[ID�B�p�����[�^�G���[�A�������̓L�[�̐ݒ�����
          �z�������ߒǉ��ł��Ȃ������ꍇ��0��Ԃ��B
          DWC_InitFriendsMatch()���Ă΂����O�ɌĂяo���Ă�0��Ԃ��B
  �p�r�@�F�}�b�`���C�N�̎w�W�ƂȂ�L�[�iint�^�j��ǉ�����B
 *---------------------------------------------------------------------------*/
u8  DWC_AddMatchKeyInt(u8 keyID,
                       const char* keyString,
                       const int* valueSrc)
{
    int index;

    if (!keyString || !valueSrc) return 0;

    if ((keyID >= DWC_QR2_GAME_KEY_START) &&
        stGameMatchKeys[keyID-DWC_QR2_GAME_KEY_START].keyID){
        // ���ɒ�`����Ă���Q�[����`�L�[ID���w�肳�ꂽ�ꍇ
        if ((stGameMatchKeys[keyID-DWC_QR2_GAME_KEY_START].keyStr != NULL) &&
            (strcmp(stGameMatchKeys[keyID-DWC_QR2_GAME_KEY_START].keyStr, keyString) != 0)){
            // ���ɒ�`����Ă�����̂ƃL�[�����Ⴄ�ꍇ�͉������Ȃ�
            return 0;
        }
        // ���ɒ�`����Ă�����̂ƃL�[���������ꍇ�́A������i�߂�
        // �|�C���^�̍����ւ����s�Ȃ�
    }
    else {
        keyID = DWCi_GetNewMatchKey();  // �󂫃L�[ID���擾
        if (!keyID) return 0;
    }

    // �V�K�L�[�f�[�^��o�^����
    index = keyID-DWC_QR2_GAME_KEY_START;
    stGameMatchKeys[index].keyID  = keyID;
    stGameMatchKeys[index].isStr  = 0;
    stGameMatchKeys[index].pad    = 0;

    // ������O�̃L�[�����c���Ă�����������
    if (stGameMatchKeys[index].keyStr)
        DWC_Free(DWC_ALLOCTYPE_BASE, stGameMatchKeys[index].keyStr, 0);

    // �L�[���̂��߂̗̈��ALLOC���ĕ�������Z�b�g����
    stGameMatchKeys[index].keyStr = DWC_Alloc(DWC_ALLOCTYPE_BASE, strlen(keyString)+1);
    if (!stGameMatchKeys[index].keyStr){
        DWCi_StopMatching(DWC_ERROR_FATAL, DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_ALLOC);
        return 0;
    }

    strcpy(stGameMatchKeys[index].keyStr, keyString);

    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "DWC_AddMatchKeyInt: key='%s', value=%d\n",
               keyString, *valueSrc);
    
    stGameMatchKeys[index].value  = valueSrc;

    // �J�X�^���L�[��ݒ肷��
    qr2_register_key(keyID, stGameMatchKeys[index].keyStr);

    return keyID;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�ǉ��֐� string��
  �����@�FkeyID        �L�[ID���w�肵�ăZ�b�g�������ꍇ�͂����ɃL�[ID��n���B
                       �V�K��keyID���擾��������� DWC_QR2_GAME_KEY_START
                       �ȉ��̒l�i0�Ȃǁj���Z�b�g����Ηǂ��B
                       ���Ɏg�p���̒ǉ��L�[ID���w�肵���ꍇ�́A�g�p���̂��̂�
                       ����keyString��n����valueSrc���Z�b�g���������A
                       �ႤkeyString��n�����ꍇ�͉������Ȃ��B
          keyString    �L�[���ʂ̂��߂̕�����ւ̃|�C���^�B
                       ������̓R�s�[���ă��C�u�������ŕێ����܂��B
          valueSrc     �L�[�ɑΉ����镶����ւ̃|�C���^�ŁA�}�X�^�[�T�[�o�����
                       �|�[�����O������ƁA���̃|�C���^���當������Q�Ƃ���
                       �}�X�^�[�T�[�o�ɕ�������A�b�v����B
  �߂�l�F�ǉ������L�[�̃L�[ID�B�p�����[�^�G���[�A�������̓L�[�̐ݒ�����
          �z�������ߒǉ��ł��Ȃ������ꍇ��0��Ԃ��B
          DWC_InitFriendsMatch()���Ă΂����O�ɌĂяo���Ă�0��Ԃ��B
  �p�r�@�F�}�b�`���C�N�̎w�W�ƂȂ�L�[�ichar*�^�j��ǉ�����B
 *---------------------------------------------------------------------------*/
u8  DWC_AddMatchKeyString(u8 keyID,
                          const char* keyString,
                          const char* valueSrc)
{
    int index;

    if (!keyString || !valueSrc) return 0;

    if ((keyID >= DWC_QR2_GAME_KEY_START) &&
        stGameMatchKeys[keyID-DWC_QR2_GAME_KEY_START].keyID){
        // ���ɒ�`����Ă���Q�[����`�L�[ID���w�肳�ꂽ�ꍇ
        if ((stGameMatchKeys[keyID-DWC_QR2_GAME_KEY_START].keyStr != NULL) &&
            (strcmp(stGameMatchKeys[keyID-DWC_QR2_GAME_KEY_START].keyStr, keyString) != 0)){
            // ���ɒ�`����Ă�����̂ƃL�[�����Ⴄ�ꍇ�͉������Ȃ�
            return 0;
        }
        // ���ɒ�`����Ă�����̂ƃL�[���������ꍇ�́A������i�߂�
        // �|�C���^�̍����ւ����s�Ȃ�
    }
    else {
        keyID = DWCi_GetNewMatchKey();  // �󂫃L�[ID���擾
        if (!keyID) return 0;
    }

    // �V�K�L�[�f�[�^��o�^����
    index = keyID-DWC_QR2_GAME_KEY_START;
    stGameMatchKeys[index].keyID      = keyID;
    stGameMatchKeys[index].isStr      = 1;
    stGameMatchKeys[index].pad        = 0;

    // ������O�̃L�[�����c���Ă�����������
    if (stGameMatchKeys[index].keyStr)
        DWC_Free(DWC_ALLOCTYPE_BASE, stGameMatchKeys[index].keyStr, 0);

    // �L�[���̂��߂̗̈��ALLOC���ĕ�������Z�b�g����
    stGameMatchKeys[index].keyStr = DWC_Alloc(DWC_ALLOCTYPE_BASE, strlen(keyString)+1);
    if (!stGameMatchKeys[index].keyStr){
        DWCi_StopMatching(DWC_ERROR_FATAL, DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_ALLOC);
        return 0;
    }

    strcpy(stGameMatchKeys[index].keyStr, keyString);

    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "DWC_AddMatchKeyString: key='%s' value='%s'\n",

               keyString, valueSrc);
    
    stGameMatchKeys[index].value      = valueSrc;

    // �J�X�^���L�[��ݒ肷��
    qr2_register_key(keyID, stGameMatchKeys[index].keyStr);

    return keyID;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�擾�֐� int��
  �����@�Findex     �}�b�`���C�N���v���C���[�̃C���f�b�N�X�B�]���R�[���o�b�N��
                    ���� index �����̂܂ܓn���B
          keyString �L�[���ʂ̂��߂̕�����ւ̃|�C���^
          idefault  �w�肵���L�[�������Ă��Ȃ��v���C���[�������ꍇ��
                    �f�t�H���g�l
  �߂�l�F�}�b�`���C�N���v���C���[�����A�w�肵���L�[�ɑΉ�����l�B
          �L�[���Ȃ��ꍇ�͈��� idefault �ɓn�����l���Ԃ��Ă���B
  �p�r�@�F�}�b�`���C�N�w�W�L�[�ɑΉ�����l���擾����B
          �v���C���[�]���R�[���o�b�N���ł̎g�p�̂ݕۏ؂��Ă���A
          ����ȊO�̏ꏊ�ŌĂяo�����ꍇ�̖߂�l�͕s��B
 *---------------------------------------------------------------------------*/
int DWC_GetMatchIntValue(int index, const char* keyString, int idefault)
{
    SBServer server;

    if (!DWCi_GetMatchCnt() || DWCi_IsError()) return idefault;

    server = ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, index);

    if (!server) return idefault;
    
    return SBServerGetIntValue(server, keyString, idefault);
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�w�W�L�[�擾�֐� string��
  �����@�Findex     �}�b�`���C�N���v���C���[�̃C���f�b�N�X�B�]���R�[���o�b�N��
                    ���� index �����̂܂ܓn���B
          keyString �L�[���ʂ̂��߂̕�����ւ̃|�C���^
          sdefault  �w�肵���L�[�������Ă��Ȃ��v���C���[�������ꍇ��
                    �f�t�H���g������
  �߂�l�F�}�b�`���C�N���v���C���[�����A�w�肵���L�[�ɑΉ����镶����B
          �L�[���Ȃ��ꍇ�͈��� sdefault �ɓn���������񂪕Ԃ��Ă���B
  �p�r�@�F�}�b�`���C�N�w�W�L�[�ɑΉ����镶������擾����B
          �v���C���[�]���R�[���o�b�N���ł̎g�p�̂ݕۏ؂��Ă���A
          ����ȊO�̏ꏊ�ŌĂяo�����ꍇ�̖߂�l�͕s��B
 *---------------------------------------------------------------------------*/
const char* DWC_GetMatchStringValue(int index, const char* keyString, const char* sdefault)
{
    SBServer server;

    if (!DWCi_GetMatchCnt() || DWCi_IsError()) return sdefault;

    server = ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, index);

    if (!server) return sdefault;
    
    return SBServerGetStringValue(server, keyString, sdefault);
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�^�C�v�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ō�ɍs�����}�b�`���C�N��ʁBDWC_MATCH_TYPE_*�Œ�`�����B
          �I�t���C����ԂȂ�-1��Ԃ��B
  �p�r�@�F�Ō�ɍs�����}�b�`���C�N��ʂ��擾����B
 *---------------------------------------------------------------------------*/
int DWC_GetLastMatchingType(void)
{

    if (DWCi_GetMatchCnt()) return DWCi_GetMatchCnt()->qr2MatchType;
    else return -1;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I�v�V�����Z�b�g�֐�
  �����@�Fopttype �}�b�`���C�N�I�v�V�����̌^
          optval  �}�b�`���C�N�I�v�V�����̒l�ւ̃|�C���^�B
                  opttype�ɑΉ������^�̃|�C���^��n���B
          optlen  �^���z��̏ꍇ�̂݁A���̔z��̃T�C�Y��n���K�v������B
  �߂�l�FDWC_SET_MATCH_OPT_RESULT_*�^�̗񋓎q
  �p�r�@�F�}�b�`���C�N�ɂ��Ă̗l�X�ȃI�v�V������ݒ肷��B
 *---------------------------------------------------------------------------*/
int DWC_SetMatchingOption(DWCMatchOptType opttype, const void* optval, int optlen)
{
#pragma unused(optlen)
    DWCMatchOptMinComplete* pMinComp;

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "!!DWC_SetMatchingOption() was called!! type %d\n",
               opttype);

    if (!DWCi_GetMatchCnt()) return DWC_SET_MATCH_OPT_RESULT_E_BAD_STATE;
    if (!optval) return DWC_SET_MATCH_OPT_RESULT_E_PARAM;
    
    switch (opttype){
    case DWC_MATCH_OPTION_MIN_COMPLETE:
        // �w��l�����W�܂�Ȃ��Ă���莞�ԂŃ}�b�`���C�N�����ɂ���I�v�V�����B
        // optval�� DWCMatchOptMinComplete�\���̂Ŏ󂯎��B
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_POLL_TIMEOUT){
            // ���x�^�C���A�E�g�|�[�����O�̏������ɌĂ�ł��܂����ꍇ
            return DWC_SET_MATCH_OPT_RESULT_E_BAD_STATE;
        }
        
        pMinComp = (DWCMatchOptMinComplete *)optval;
        // �Œ�l��1�ȉ��A�^�C���A�E�g0�̓p�����[�^�G���[
        if (pMinComp->valid && (pMinComp->minEntry <= 1))
            return DWC_SET_MATCH_OPT_RESULT_E_PARAM;

        if (!stpOptMinComp){
            // �\���̂̎��̂��Ȃ����Alloc
            stpOptMinComp =
                DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCMatchOptMinCompleteIn));
            if (!stpOptMinComp) return DWC_SET_MATCH_OPT_RESULT_E_ALLOC;
        }

        stpOptMinComp->valid        = pMinComp->valid;
        stpOptMinComp->minEntry     = pMinComp->minEntry;
        stpOptMinComp->retry        = 0;
        stpOptMinComp->pad          = 0;
        stpOptMinComp->timeout      = pMinComp->timeout;
        stpOptMinComp->recvBit      = 0;
        stpOptMinComp->timeoutBit   = 0;
        stpOptMinComp->startTime    = OS_GetTick();
        stpOptMinComp->lastPollTime = OS_GetTick();
        return DWC_SET_MATCH_OPT_RESULT_SUCCESS;

    case DWC_MATCH_OPTION_SC_CONNECT_BLOCK:
        // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�P��}�b�`���C�N�������
        // ���̐ڑ����󂯕t���Ȃ�����I�v�V�����B
        // optval��BOOL�Ŏ󂯎��B
        // �}�b�`���C�N�J�n�O�ɐݒ肷��K�v������̂ŁA�T�[�o�N���C�A���g
        // �}�b�`���C�N�łȂ��Ɛݒ�ł��Ȃ��A�Ƃ�������͍s��Ȃ��B
        if (*(BOOL *)optval) stOptSCBlock.valid = 1;
        else                 stOptSCBlock.valid = 0;
        stOptSCBlock.lock  = 0;
        return DWC_SET_MATCH_OPT_RESULT_SUCCESS;

    default:
        return DWC_SET_MATCH_OPT_RESULT_E_INVALID;
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I�v�V�����擾�֐�
  �����@�Fopttype �}�b�`���C�N�I�v�V�����̌^
          optval  �}�b�`���C�N�I�v�V�����̒l�i�[��|�C���^�B
                  opttype�ɑΉ������^�̃|�C���^��n���B
          optlen  �擾�����I�v�V�����̒l�̃T�C�Y�i�[��
  �߂�l�FDWC_SET_MATCH_OPT_RESULT_*�^�̗񋓎q
  �p�r�@�F�}�b�`���C�N�ɂ��Ă̐ݒ蒆�̃I�v�V�������擾����B
          �w�肵���I�v�V�������ݒ肳��Ă��Ȃ��ꍇ�́A���������ɁA
          ����optlen��0���i�[���ĕԂ��B
 *---------------------------------------------------------------------------*/
int DWC_GetMatchingOption(DWCMatchOptType opttype, void* optval, int* optlen)
{

    if (!DWCi_GetMatchCnt()) return DWC_SET_MATCH_OPT_RESULT_E_BAD_STATE;
    if (!optval) return DWC_SET_MATCH_OPT_RESULT_E_PARAM;
    
    switch (opttype){
    case DWC_MATCH_OPTION_MIN_COMPLETE:
        // �w��l�����W�܂�Ȃ��Ă���莞�ԂŃ}�b�`���C�N�����ɂ���I�v�V����
        if (!stpOptMinComp){
            if (optlen) *optlen = 0;
        }
        else {
            DWCMatchOptMinComplete* pVal = optval;
            pVal->valid    = stpOptMinComp->valid;
            pVal->minEntry = stpOptMinComp->minEntry;
            pVal->timeout  = stpOptMinComp->timeout;
            pVal->pad[0] = pVal->pad[1] = 0;
            if (optlen) *optlen = sizeof(DWCMatchOptMinComplete);
        }
        return DWC_SET_MATCH_OPT_RESULT_SUCCESS;

    case DWC_MATCH_OPTION_SC_CONNECT_BLOCK:
        // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�P��}�b�`���C�N�������
        // ���̐ڑ����󂯕t���Ȃ�����I�v�V�����B
        // �I�v�V�������L�����ǂ����݂̂�Ԃ��B
        if (stOptSCBlock.valid == 1) *(BOOL *)optval = TRUE;
        else                         *(BOOL *)optval = FALSE;
        if (optlen) *optlen = sizeof(BOOL);
        return DWC_SET_MATCH_OPT_RESULT_SUCCESS;

    default:
        return DWC_SET_MATCH_OPT_RESULT_E_INVALID;
    }
}


/*---------------------------------------------------------------------------*
  �w��l���ȉ��}�b�`���C�N�����I�v�V������Ԏ擾�֐�
  �����@�Ftime �}�b�`���C�N�J�n����̌o�ߎ��ԁi�P�ʁFmsec�j�i�[��|�C���^�B
               ���Ԃ��K�v�Ȃ����NULL��n���Ηǂ��B
  �߂�l�F0 :�^�C���A�E�g���ԓ�
          1 :�^�C���A�E�g���Ԃ��z���Ă���A�w��l���ȉ��Ń}�b�`���C�N��������\������
          -1:���̃}�b�`���C�N�I�v�V�������ݒ肳��Ă��Ȃ��A��������OFF�ɂȂ��Ă���
  �p�r�@�F�w��l���ȉ��}�b�`���C�N�����I�v�V�����̎��Ԍo�ߏ󋵂��擾����
 *---------------------------------------------------------------------------*/
int DWC_GetMOMinCompState(u64* time)
{
    u64 passTime;

    if (!stpOptMinComp || !stpOptMinComp->valid) return -1;

    passTime = OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->startTime);
    if (time) *time = passTime;  // �o�ߎ��Ԃ��Z�b�g

    if (passTime >= stpOptMinComp->timeout) return 1;
    else return 0;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�u���b�N�I�v�V������Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE :�V�K�ڑ��N���C�A���g���u���b�N���A
          FALSE:�u���b�N���łȂ����A�I�v�V�������ݒ肳��Ă��Ȃ�
  �p�r�@�F�}�b�`���C�N�u���b�N�I�v�V�����̃u���b�N��Ԃ��擾����
 *---------------------------------------------------------------------------*/
BOOL DWC_GetMOSCConnectBlockState(void)
{

    if ((stOptSCBlock.valid == 0) || (stOptSCBlock.lock == 0)){
        // �I�v�V�������ݒ肳��Ă��Ȃ����A�u���b�N���łȂ����FALSE��Ԃ�
        return FALSE;
    }
    else {
        return TRUE;
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�u���b�N�����֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�u���b�N�I�v�V�����̃u���b�N��Ԃ���������
 *---------------------------------------------------------------------------*/
void DWC_ClearMOSCConnectBlock(void)
{

    // �V�K�ڑ��̃u���b�N����������
    stOptSCBlock.lock = 0;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "[OPT_SC_BLOCK] ClearMOSCConnectBlock\n");
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N������Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�F���݂̃}�b�`���C�N�������
  �p�r�@�F�}�b�`���C�N���̓�����Ԃ��擾����B�f�o�b�O���Ƀ}�b�`���C�N�̓�����Ԃ�
          �m�肽���ꍇ�ɂ̂ݎg�p����B
 *---------------------------------------------------------------------------*/
DWCMatchState DWC_GetMatchingState(void)
{

    if (DWCi_GetMatchCnt()){
        return DWCi_GetMatchCnt()->state;
    }
    else {
        return DWC_MATCH_STATE_INIT;
    }
}


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �}�b�`���C�N����\���̏������֐�
  �����@�Fmatchcnt      �}�b�`���C�N����I�u�W�F�N�g�ւ̃|�C���^
          pGpObj        GP�R�l�N�V�����\���̂ւ̃|�C���^
          pGt2Socket    gt2�\�P�b�g�\���̂ւ̃|�C���^
          gt2Callbacks  GT2�R�[���o�b�N�W�ւ̃|�C���^
          gameName      GameSpy����^������Q�[����
          secretKey     GameSpy����^������V�[�N���b�g�L�[
          friendList    �F�B���X�g�i�Q�[���Ŏg�p���Ȃ��ꍇ��NULL�ł��ǂ��j
          friendListLen �F�B���X�g��
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N����\���̂�����������
 *---------------------------------------------------------------------------*/
void DWCi_MatchInit(DWCMatchControl* matchcnt,
                    GPConnection pGpObj,
                    GT2Socket* pGt2Socket,
                    GT2ConnectionCallbacks* gt2Callbacks,
                    const char* gameName,
                    const char* secretKey,
                    const DWCFriendData friendList[],
                    int  friendListLen)
{

    SDK_ASSERT(matchcnt);

	DWCi_SetMatchCnt(matchcnt); //stpMatchCnt = matchcnt;

    DWCi_GetMatchCnt()->pGpObj            = pGpObj;
    DWCi_GetMatchCnt()->pGt2Socket        = pGt2Socket;
    DWCi_GetMatchCnt()->gt2Callbacks      = gt2Callbacks;
    DWCi_GetMatchCnt()->qr2Obj            = NULL;
    DWCi_GetMatchCnt()->qr2IP             = 0;
    DWCi_GetMatchCnt()->qr2Port           = 0;
    DWCi_GetMatchCnt()->sbObj             = NULL;
    
    DWCi_SetMatchStatus(DWC_MATCH_STATE_INIT);
    DWCi_GetMatchCnt()->pad1              = 0;
    DWCi_GetMatchCnt()->pad2              = 0;
#ifdef DWC_STOP_SC_SERVER
    DWCi_GetMatchCnt()->pad3              = 0;
    DWCi_GetMatchCnt()->pad4              = 0;
#endif
#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
    DWCi_GetMatchCnt()->friendAcceptBit   = 0;
#endif
    DWCi_GetMatchCnt()->profileID         = 0;
    DWCi_GetMatchCnt()->gameName          = gameName;
    DWCi_GetMatchCnt()->secretKey         = secretKey;

    DWCi_GetMatchCnt()->friendList        = friendList;
    DWCi_GetMatchCnt()->friendListLen     = friendListLen;
    MI_CpuClear8(DWCi_GetMatchCnt()->friendIdxList, DWC_MAX_MATCH_IDX_LIST);
    DWCi_GetMatchCnt()->friendIdxListLen  = 0;

    DWCi_GetMatchCnt()->matchedCallback   = NULL;
    DWCi_GetMatchCnt()->matchedParam      = NULL;
    DWCi_GetMatchCnt()->evalCallback      = NULL;
    DWCi_GetMatchCnt()->evalParam         = NULL;
#ifdef DWC_STOP_SC_SERVER
    DWCi_GetMatchCnt()->stopSCCallback    = NULL;
    DWCi_GetMatchCnt()->stopSCParam       = NULL;
#endif

    // [nakata] ServerBrowserLimitUpdate�Ăяo��Tick��������
    DWCi_GetMatchCnt()->sbUpdateRequestTick = 0;
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout reset.(%s)\n", __FUNCTION__);

    // �Q�[����`��QR2�p�L�[�f�[�^�z���������
    DWCi_ClearGameMatchKeys();

    stOptSCBlock.valid = 0;
    stOptSCBlock.lock  = 0;
    stOptSCBlock.pad   = 0;

    // �}�b�`���C�N���Ƃɏ��������K�v�ȃp�����[�^�͂�����ł܂Ƃ߂ď�����
    DWCi_ResetMatchParam(DWC_MATCH_RESET_ALL);
}


/*---------------------------------------------------------------------------*
  QR2�N���֐�
  �����@�FprofileID �����̃v���t�@�C��ID
  �߂�l�FQR2�G���[���
  �p�r�@�FQR2���N������B��x���s���Ă����g���C����B
 *---------------------------------------------------------------------------*/
qr2_error_t DWCi_QR2Startup(int profileID)
{
    int i;
    qr2_error_t qr2Error = e_qrnoerror;

    if (DWCi_GetMatchCnt()->qr2Obj){
        // QR2�����ɋN������Ă���̂ɂ����ɗ����ꍇ�͉������Ȃ�
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "QR2 is already set up.\n");
        return e_qrnoerror;
    }

    DWCi_GetMatchCnt()->profileID = profileID;

    for (i = 0; i < DWC_DNS_ERROR_RETRY_MAX; i++){
        // QR2������
        qr2Error = qr2_init_socket(&DWCi_GetMatchCnt()->qr2Obj,
                                   gt2GetSocketSOCKET(*DWCi_GetMatchCnt()->pGt2Socket),
                                   gt2GetLocalPort(*DWCi_GetMatchCnt()->pGt2Socket),
                                   DWCi_GetMatchCnt()->gameName, DWCi_GetMatchCnt()->secretKey, 1, 1,
                                   DWCi_QR2ServerKeyCallback,
                                   DWCi_QR2PlayerKeyCallback,
                                   DWCi_QR2TeamKeyCallback,
                                   DWCi_QR2KeyListCallback,
                                   DWCi_QR2CountCallback,
                                   DWCi_QR2AddErrorCallback,
                                   NULL);
        if (!qr2Error) break;
        else {
            qr2_shutdown(DWCi_GetMatchCnt()->qr2Obj);
            DWCi_GetMatchCnt()->qr2Obj = NULL;
        }

        // DNS�G���[�Ȃ���񐔂̓��g���C����
        if ((qr2Error != e_qrdnserror) || (i == DWC_DNS_ERROR_RETRY_MAX-1)){
            DWCi_HandleQR2Error(qr2Error);
            return qr2Error;
        }
    }

#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    // ������QR2�p�p�u���b�NIP�ƃ|�[�g�ԍ���������
    DWCi_GetMatchCnt()->qr2IP   = 0;
    DWCi_GetMatchCnt()->qr2Port = 0;
#endif

    // �p�u���b�N�A�h���X�ʒm�R�[���o�b�N�֐��o�^
    qr2_register_publicaddress_callback(DWCi_GetMatchCnt()->qr2Obj, DWCi_QR2PublicAddrCallback);
        
    // NAT�l�S�V�G�[�V�����R�[���o�b�N�֐��o�^
    qr2_register_natneg_callback(DWCi_GetMatchCnt()->qr2Obj, DWCi_QR2NatnegCallback);

    // �N���C�A���g���b�Z�[�W��M�R�[���o�b�N�֐��o�^
    qr2_register_clientmessage_callback(DWCi_GetMatchCnt()->qr2Obj, DWCi_QR2ClientMsgCallback);

    // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
    qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);

    return qr2Error;
}


/*---------------------------------------------------------------------------*
  �F�B���w��s�A�}�b�`���C�N�J�n�֐�
  �����@�FnumEntry        �ڑ��v���l���i�����͊܂܂Ȃ��j
          addFilter       �Q�[���Œǉ�������SB�T�[�o�A�b�v�f�[�g�p�t�B���^
          matchedCallback �}�b�`���C�N�����R�[���o�b�N
          matehedParam    ��L�R�[���o�b�N�p�p�����[�^
          evalCallback    �v���C���[�]���R�[���o�b�N
          evalParam       ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�w��s�A�}�b�`���C�N���J�n����
 *---------------------------------------------------------------------------*/
void DWCi_ConnectToAnybodyAsync(u8  numEntry,
                                const char* addFilter,
                                DWCMatchedSCCallback matchedCallback,
                                void* matchedParam,
                                DWCEvalPlayerCallback evalCallback,
                                void* evalParam)
{
    char filter[MAX_FILTER_LEN];
    u32  addFilterLenMax;
    SBError sbError;

    // ������O�̃t�B���^���c���Ă�����������
    if (stpAddFilter){
        DWC_Free(DWC_ALLOCTYPE_BASE, stpAddFilter, 0);
        stpAddFilter = NULL;
    }

    // �ǉ��t�B���^�[�����������������z���Ă��Ȃ����𒲂ׂ�
    if (addFilter){
        // �ǉ��t�B���^�̍ő勖�e�������iNULL�I�[�܂ށj���Z�o
        addFilterLenMax = MAX_FILTER_LEN-DWCi_GetDefaultMatchFilter(filter, 0xffffffff, DWC_MAX_CONNECTIONS, DWC_MATCH_TYPE_SC_CL)-strlen(" and ()");

        SDK_ASSERT(strlen(addFilter) < addFilterLenMax);

        stpAddFilter = DWC_Alloc(DWC_ALLOCTYPE_BASE, addFilterLenMax);
        if (!stpAddFilter){
            DWCi_StopMatching(DWC_ERROR_FATAL, DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_ALLOC);
            return;
        }

        MI_CpuCopy8(addFilter, stpAddFilter, addFilterLenMax);
        stpAddFilter[addFilterLenMax-1] = '\0';  // NULL�I�[��ۏ�
    }

    // �}�b�`���C�N�p���ʃp�����[�^�Z�b�g
    DWCi_SetMatchCommonParam(DWC_MATCH_TYPE_ANYBODY,
                             numEntry, matchedCallback, matchedParam);

    DWCi_GetMatchCnt()->evalCallback = evalCallback;
    DWCi_GetMatchCnt()->evalParam    = evalParam;

    DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_OWN);

    // SB������
    if ( !DWCi_GetMatchCnt()->sbObj )
            DWCi_GetMatchCnt()->sbObj = ServerBrowserNew(DWCi_GetMatchCnt()->gameName,
                                                  DWCi_GetMatchCnt()->gameName,
                                                  DWCi_GetMatchCnt()->secretKey, 0, 20,
                                                  QVERSION_QR2, SBFalse,
                                                  DWCi_SBCallback, NULL);
    if (!DWCi_GetMatchCnt()->sbObj){
        // �������m�ۃG���[�Ǝv����
        if (DWCi_HandleSBError(sbe_allocerror)) return;
    }

    // GP�X�e�[�^�X��F�B���w��s�A�}�b�`���C�N���ɃZ�b�g����i�G���[�͖�������j
    (void)DWCi_SetGPStatus(DWC_STATUS_MATCH_ANYBODY, "", NULL);

    // SB�T�[�o�A�b�v�f�[�g���s���A�����̃f�[�^�̔��f���m�F����
    sbError = DWCi_SBUpdateAsync(DWCi_GetMatchCnt()->profileID);
    if (DWCi_HandleSBError(sbError)) return;

#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    if (DWCi_GetMatchCnt()->qr2Obj == NULL){
        // QR2���N������Ă��Ȃ���΂����ŋN������
        DWCi_QR2Startup(DWCi_GetMatchCnt()->profileID);
    }

#else
    // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
    qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
#endif

    // �w��l���ȉ��ł̃}�b�`���C�N�����I�v�V�����p������
    DWCi_InitOptMinCompParam(FALSE);
}


/*---------------------------------------------------------------------------*
  �F�B�w��s�A�}�b�`���C�N�J�n�֐�
  �����@�FfriendIdxList    �ڑ��v���F�B�C���f�b�N�X���X�g�ւ̃|�C���^
          friendIdxListLen �ڑ��v���F�B�C���f�b�N�X���X�g��
          numEntry         �ڑ��v���l���i�����͊܂܂Ȃ��j
          distantFriend    TRUE:�F�B�̗F�B�Ƃ̐ڑ��������AFALSE:�����Ȃ�
          matchedCallback  �}�b�`���C�N�����R�[���o�b�N
          matchedParam     ��L�R�[���o�b�N�p�p�����[�^
          evalCallback     �v���C���[�]���R�[���o�b�N
          evalParam        ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�w��s�A�}�b�`���C�N���J�n����
 *---------------------------------------------------------------------------*/
void DWCi_ConnectToFriendsAsync(const u8 friendIdxList[],
                                int friendIdxListLen,
                                u8  numEntry,
                                BOOL distantFriend,
                                DWCMatchedSCCallback matchedCallback,
                                void* matchedParam,
                                DWCEvalPlayerCallback evalCallback,
                                void* evalParam)
{
    char valueStr[12], keyValueStr[32];
#ifndef DWC_MATCH_ACCEPT_NO_FRIEND
    int  checkpid;
    int  friendCount;
    int  i;
#endif
    int  result;
    GPResult gpResult;

    // �}�b�`���C�N�p���ʃp�����[�^�Z�b�g
    DWCi_SetMatchCommonParam(DWC_MATCH_TYPE_FRIEND,
                             numEntry, matchedCallback, matchedParam);

    DWCi_GetMatchCnt()->distantFriend = (u8)(distantFriend ? 1 : 0);
    DWCi_GetMatchCnt()->evalCallback  = evalCallback;
    DWCi_GetMatchCnt()->evalParam     = evalParam;

    // �F�B�C���f�b�N�X���X�g���R�s�[
#ifdef DWC_MATCH_ACCEPT_NO_FRIEND
    MI_CpuCopy8(friendIdxList, DWCi_GetMatchCnt()->friendIdxList, (u32)friendIdxListLen);
    DWCi_GetMatchCnt()->friendIdxListLen = friendIdxListLen;

    if (!friendIdxListLen ||
        ((friendIdxListLen < numEntry) &&
         !distantFriend &&
         (!stpOptMinComp || !stpOptMinComp->valid || (friendIdxListLen < stpOptMinComp->minEntry-1)))){
        // �v������ڑ��l�����F�B�̐l�������Ȃ��ꍇ�A
        // �F�B�̗F�B�������Ȃ��ݒ�ŁA���w��l���ȉ��}�b�`���C�N
        // �����I�v�V�����̍Œ�l�������F�B�̐l�������Ȃ����
        // �F�B�l���s���G���[�B�L���F�B�l��0�ł��G���[�B
        DWCi_StopMatching(DWC_ERROR_FRIENDS_SHORTAGE, 0);
        return;
    }

#else
    for (i = 0, friendCount = 0; i < friendIdxListLen; i++){
        if (!(checkpid = DWCi_GetProfileIDFromList(friendIdxList[i])) ||
            !gpIsBuddy(DWCi_GetMatchCnt()->pGpObj, checkpid)){
            // �w�肳�ꂽ�C���f�b�N�X���v���t�@�C��ID�������Ă��Ȃ��A
            // �������͑��肪�o�f�B�łȂ������ꍇ�̓��X�g����O��
            continue;
        }

        DWCi_GetMatchCnt()->friendIdxList[friendCount] = friendIdxList[i];
        friendCount++;
    }

    if (!friendCount ||
        ((friendCount < numEntry) &&
         !distantFriend &&
         (!stpOptMinComp || !stpOptMinComp->valid || (friendCount < stpOptMinComp->minEntry-1)))){
        // �v������ڑ��l�����F�B�̐l�������Ȃ��ꍇ�A
        // �F�B�̗F�B�������Ȃ��ݒ�ŁA���w��l���ȉ��}�b�`���C�N
        // �����I�v�V�����̍Œ�l�������F�B�̐l�������Ȃ����
        // �F�B�l���s���G���[�B�L���F�B�l��0�ł��G���[�B
        // [todo]
        // �}�b�`���C�N���ɐ��������o�f�B���C���f�b�N�X���X�g�ɉ�����悤��
        // ������A�����̃G���[�͂�߂�
        // ����߂�
        DWCi_StopMatching(DWC_ERROR_FRIENDS_SHORTAGE, 0);
        return;
    }

    DWCi_GetMatchCnt()->friendIdxListLen = friendCount;
#endif
    
    DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);

    // SB������
    if ( !DWCi_GetMatchCnt()->sbObj )
            DWCi_GetMatchCnt()->sbObj = ServerBrowserNew(DWCi_GetMatchCnt()->gameName,
                                                  DWCi_GetMatchCnt()->gameName,
                                                  DWCi_GetMatchCnt()->secretKey, 0, 20,
                                                  QVERSION_QR2, SBFalse,
                                                  DWCi_SBCallback, NULL);
    if (!DWCi_GetMatchCnt()->sbObj){
        // �������m�ۃG���[�Ǝv����
        if (DWCi_HandleSBError(sbe_allocerror)) return;
    }

    // ��]�ڑ��l���ƗF�B�̗F�B����������GP�X�e�[�^�X������Ƃ��č쐬����
    (void)OS_SNPrintf(valueStr, sizeof(valueStr), "%u", numEntry);
    (void)DWC_SetCommonKeyValueString(DWC_GP_SSTR_KEY_MATCH_FRIEND_NUM,
                                      valueStr, keyValueStr, '/');
    (void)DWC_AddCommonKeyValueString(DWC_GP_SSTR_KEY_DISTANT_FRIEND,
                                      distantFriend ? "Y" : "N",
                                      keyValueStr, '/');

    // �}�b�`���C�N�o�[�W�������Z�b�g����
    (void)OS_SNPrintf(valueStr, sizeof(valueStr), "%u", DWC_MATCHING_VERSION);
    (void)DWC_AddCommonKeyValueString(DWC_GP_SSTR_KEY_MATCH_VERSION,
                                      valueStr, keyValueStr, '/');

    // GP�X�e�[�^�X��F�B�w��s�A�}�b�`���C�N���ɂ��A�쐬����GP�X�e�[�^�X�������
    // �Z�b�g����
    gpResult = DWCi_SetGPStatus(DWC_STATUS_MATCH_FRIEND, keyValueStr, NULL);
    if (DWCi_HandleGPError(gpResult)) return;

#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    if (DWCi_GetMatchCnt()->qr2Obj == NULL){
        // QR2���N������Ă��Ȃ���΂����ŋN������B
        // �\��R�}���h�𑗂�O��qr2IP��qr2Port������������K�v������B
        if (DWCi_QR2Startup(DWCi_GetMatchCnt()->profileID)) return;
    }

#else
    // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
    qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
#endif

    // GP���b�Z�[�W�ōŏ��̃T�[�o��NAT�l�S�V�G�[�V�����̗\����s���B
    // �v���t�@�C��ID���K���o�f�B�ł��邱�Ƃ͂����ł͕ۏ؂���Ă���B
    result = DWCi_SendResvCommandToFriend(FALSE, TRUE, 0);
    if (DWCi_HandleMatchCommandError(result)) return;

    // �w��l���ȉ��ł̃}�b�`���C�N�����I�v�V�����p������
    DWCi_InitOptMinCompParam(FALSE);
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�E�T�[�o�J�n�֐�
  �����@�FmaxEntry          �ő�ڑ��l���i�����͊܂܂Ȃ��j
          matchedCallback   �}�b�`���C�N�����R�[���o�b�N
          matchedParam      ��L�R�[���o�b�N�p�p�����[�^
          newClientCallback �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
          newClientParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���Ƃ��ă}�b�`���C�N���J�n����
 *---------------------------------------------------------------------------*/
// [arakit] main 051027
void DWCi_SetupGameServer(u8   maxEntry,
                          DWCMatchedSCCallback matchedCallback,
                          void* matchedParam,
                          DWCNewClientCallback newClientCallback,
                          void* newClientParam)
{
    GPResult gpResult;

    // �}�b�`���C�N�p���ʃp�����[�^�Z�b�g
    DWCi_SetMatchCommonParam(DWC_MATCH_TYPE_SC_SV,
                             maxEntry, matchedCallback, matchedParam);

    // �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N���Z�b�g
    DWCi_GetMatchCnt()->newClientCallback = newClientCallback;
    DWCi_GetMatchCnt()->newClientParam    = newClientParam;

    // �v���t�@�C��ID���X�g�̍ŏ��ɂ͎����̃v���t�@�C��ID������
    DWCi_GetMatchCnt()->sbPidList[0] = DWCi_GetMatchCnt()->profileID;

    // ������AID�͕K���ŏ�����L��
    DWCi_GetMatchCnt()->validAidBitmap  = 1;
    DWCi_GetMatchCnt()->gt2NumValidConn = 0;

    // �}�b�`���C�N�J�n���ɂ͕K���}�b�`���C�N�I�v�V�����̐V�K�ڑ��u���b�N�͉�������
    stOptSCBlock.lock  = 0;

    DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_WAITING);

    // GP�X�e�[�^�X���Z�b�g����
    gpResult = DWCi_GPSetServerStatus();
    if (DWCi_HandleGPError(gpResult)) return;

    // [arakit] main 051008
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    if (DWCi_GetMatchCnt()->qr2Obj == NULL){
        // QR2���N������Ă��Ȃ���΂����ŋN������
        DWCi_QR2Startup(DWCi_GetMatchCnt()->profileID);
    }

#else
    // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
    qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
#endif
    // [arakit] main 051008
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�E�T�[�o�ڑ��J�n�֐�
  �����@�FserverPid         �T�[�o�̃v���t�@�C��ID
          matchedCallback   �}�b�`���C�N�����R�[���o�b�N
          matchedParam      ��L�R�[���o�b�N�p�p�����[�^
          newClientCallback �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
          newClientParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g���Ƃ���
          �}�b�`���C�N���J�n����
 *---------------------------------------------------------------------------*/
void DWCi_ConnectToGameServerAsync(int serverPid,
                                   DWCMatchedSCCallback matchedCallback,
                                   void* matchedParam,
                                   DWCNewClientCallback newClientCallback,
                                   void* newClientParam)
{
    int result;
    GPResult gpResult;

    // �}�b�`���C�N�p���ʃp�����[�^�Z�b�g
    DWCi_SetMatchCommonParam(DWC_MATCH_TYPE_SC_CL,
                             0, matchedCallback, matchedParam);

    // �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N���Z�b�g
    DWCi_GetMatchCnt()->newClientCallback = newClientCallback;
    DWCi_GetMatchCnt()->newClientParam    = newClientParam;

    // �N���C�A���g�ł��邱�Ƃ��L�^����
    DWCi_GetMatchCnt()->qr2IsReserved  = 1;
    DWCi_GetMatchCnt()->qr2Reservation = DWCi_GetMatchCnt()->profileID;

    // �T�[�o�̃v���t�@�C��ID�����X�g�ɃZ�b�g����
    DWCi_GetMatchCnt()->sbPidList[0] = serverPid;

    DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);

    // SB������
    if ( !DWCi_GetMatchCnt()->sbObj )
            DWCi_GetMatchCnt()->sbObj = ServerBrowserNew(DWCi_GetMatchCnt()->gameName,
                                                  DWCi_GetMatchCnt()->gameName,
                                                  DWCi_GetMatchCnt()->secretKey,
                                                  0, 20,
                                                  QVERSION_QR2, SBFalse,
                                                  DWCi_SBCallback, NULL);
    if (!DWCi_GetMatchCnt()->sbObj){
        // �������m�ۃG���[�Ǝv����
        if (DWCi_HandleSBError(sbe_allocerror)) return;
    }

    // GP�X�e�[�^�X���T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�ɃZ�b�g����
    gpResult = DWCi_SetGPStatus(DWC_STATUS_MATCH_SC_CL, "", NULL);
    if (DWCi_HandleGPError(gpResult)) return;

    // [arakit] main 051008
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    if (DWCi_GetMatchCnt()->qr2Obj == NULL){
        // QR2���N������Ă��Ȃ���΂����ŋN������B
        // �\��R�}���h�𑗂�O��qr2IP��qr2Port������������K�v������B
        if (DWCi_QR2Startup(DWCi_GetMatchCnt()->profileID)) return;
    }

#else
    // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
    qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
#endif
    // [arakit] main 051008

    // GP���b�Z�[�W�ŖڕW�̃T�[�o��NAT�l�S�V�G�[�V�����̗\����s���B
    // �v���t�@�C��ID���K���o�f�B�ł��邱�Ƃ͂����ł͕ۏ؂���Ă���B
    // �܂����肪�K���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���N�����Ă��邱�Ƃ�
    // �ۏ؂���Ă���B
    result = DWCi_SendResvCommand(DWCi_GetMatchCnt()->sbPidList[0], FALSE);
    if (DWCi_HandleMatchCommandError(result)) return;
}

/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����X�V�֐��̉������֐��B�K�v�ɉ�����qr2_send_statechanged ���Ă�
  �����@�Fqr2obj 
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�f�[�^���f�̂��߂ɏ�Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
 *---------------------------------------------------------------------------*/
static void DWCi_SendStateChanged(qr2_t qr2Obj)
{
  if(qr2Obj == NULL){ return; }

  qr2_think(qr2Obj);
  
  if(qr2Obj->userstatechangerequested){ return; }
  
  // �N���C�A���g���������s���^�C�~���O��QR2�f�[�^���f��
  // �������邽�߂ɏ�Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
  // �i���̃^�C�~���O���Ǝ��ۂɂ̓t���O���Z�b�g����邾���j
  switch(DWCi_GetMatchCnt()->qr2MatchType){
  case DWC_MATCH_TYPE_ANYBODY:
  case DWC_MATCH_TYPE_FRIEND:
	switch(DWCi_GetMatchCnt()->state){
	case DWC_MATCH_STATE_CL_WAITING:
	case DWC_MATCH_STATE_CL_SEARCH_OWN:
	case DWC_MATCH_STATE_CL_SEARCH_HOST:
	case DWC_MATCH_STATE_CL_WAIT_RESV:
	case DWC_MATCH_STATE_CL_NN:
	case DWC_MATCH_STATE_SV_OWN_NN:
	  qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
	  break;
	}
	break;
  case DWC_MATCH_TYPE_SC_SV:
	if(DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_NN){
	  qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
	}
	break;
  }
}

/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����X�V�֐�
  �����@�FfullSpec TRUE :�}�b�`���C�N�ׂ̈̏�����S�čs���A
                   FALSE:GT2��QR2�̍X�V�̂ݍs��
  �߂�l�F�Ȃ�
  �p�r�@�F���Q�[���t���[���Ăяo���A�}�b�`���C�N�ʐM�������X�V����
 *---------------------------------------------------------------------------*/
void DWCi_MatchProcess(BOOL fullSpec)
{
  u32  resendInterval;
  u32  sbInterval;
  int  result;
  SBError sbError;
    
  if(!DWCi_GetMatchCnt() || DWCi_IsError()){ return; }
  
  if(!fullSpec){ // GT2��QR2�̍X�V�̂ݍs��
	if(DWCi_GetMatchCnt()->qr2Obj){ qr2_think(DWCi_GetMatchCnt()->qr2Obj); }
	if (DWCi_GetMatchCnt()->pGt2Socket){ gt2Think(*DWCi_GetMatchCnt()->pGt2Socket); }
	return;
  }

  if(DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_INIT){	return; } // ������Ԃŗ��Ă��������Ȃ�

    /*** ���^�C���A�E�g���� ***/
    switch (DWCi_GetMatchCnt()->state){
    case DWC_MATCH_STATE_CL_WAIT_RESV:  // NN�\���ԓ��҂����
	  //        if (DWCi_GetMatchCnt()->cmdTimeoutCount){
	  if (DWCi_GetMatchCnt()->cmdTimeoutTime > 0 ){
		// NN�\��R�}���h�̉����҂�
		//            DWCi_GetMatchCnt()->cmdTimeoutCount++;
		//            if (DWCi_GetMatchCnt()->cmdTimeoutCount >= DWC_MATCH_CMD_RESV_TIMEOUT){
		if (OS_TicksToMilliSeconds( OS_GetTick() - DWCi_GetMatchCnt()->cmdTimeoutStartTick ) > DWCi_GetMatchCnt()->cmdTimeoutTime){
		  // �T�[�o�����ؒf���ĉ������Ȃ��Ȃ����ƍl������
		  //                DWCi_GetMatchCnt()->cmdTimeoutCount = 0;
		  DWCi_GetMatchCnt()->cmdTimeoutTime = 0;
		  
		  if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
			// �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ
			DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
					   "Timeout: wait server response %d/%d.\n",
					   DWCi_GetMatchCnt()->scResvRetryCount,
					   DWC_MATCH_CMD_RETRY_MAX );
			
			DWCi_GetMatchCnt()->scResvRetryCount++;
			if (DWCi_GetMatchCnt()->scResvRetryCount > DWC_MATCH_CMD_RETRY_MAX){
			  // ���񐔎��s������}�b�`���C�N���G���[�I������
			  DWCi_StopMatching(DWC_ERROR_NETWORK,
								DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_SC_CL_FAIL);
			  return;
			}
			else {
			  // �\��R�}���h���đ�����
			  result = DWCi_SendResvCommand(DWCi_GetMatchCnt()->sbPidList[0], FALSE);
			  if (DWCi_HandleMatchCommandError(result)) return;
			}
		  }
		  else {
			// �F�B�w��E���w��s�A�}�b�`���C�N�̏ꍇ�͎��̃T�[�o��T��
			// �G���[�����͊֐����ōs���Ă���
			DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN resv(with %u) timed out. Try next server.\n", DWCi_GetMatchCnt()->sbPidList[0]);
			if (!DWCi_RetryReserving(0)) return;
		  }
		}
	  }
	  
	  //        if (DWCi_GetMatchCnt()->cmdResendCount){
	  if (DWCi_GetMatchCnt()->cmdResendFlag){
		// NN�\��R�}���h�̍đ��҂�
		// ���ɃN���C�A���g���q�����Ă���ꍇ�́A�Č����J�n��x�点�A
		// ���̃z�X�g�̗\��𒷂߂ɑ҂�
		resendInterval =
		  (u32)DWC_MATCH_CMD_RESEND_INTERVAL_MSEC+DWC_MATCH_CMD_RESEND_INTERVAL_ADD_MSEC*DWCi_GetMatchCnt()->gt2NumConnection;
		
		//            DWCi_GetMatchCnt()->cmdResendCount++;
		//            if (DWCi_GetMatchCnt()->cmdResendCount >= resendInterval){
		if (OS_TicksToMilliSeconds( OS_GetTick() - DWCi_GetMatchCnt()->cmdResendTick ) >= resendInterval){
		  // �ĂуT�[�o��NN�\��R�}���h�𑗂�
		  result = DWCi_SendResvCommand(DWCi_GetMatchCnt()->sbPidList[0], FALSE);
		  if (DWCi_HandleMatchCommandError(result)) return;
		}
	  }
	  break;

    case DWC_MATCH_STATE_CL_SEARCH_OWN:      // ���z�X�g��񌟍���
    case DWC_MATCH_STATE_CL_SEARCH_HOST:     // �󂫃z�X�g�������i�F�B���w��s�A�}�b�`���C�N���j
    case DWC_MATCH_STATE_CL_SEARCH_NN_HOST:  // NN�\�񊮗��z�X�g������
	  if(DWCi_GetMatchCnt()->sbUpdateFlag <= 0){ break; }

	  if(DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_SEARCH_HOST){
		// �F�B���w��s�A�}�b�`���C�N�̃T�[�o�ŁA���ɃN���C�A���g��
		// �q�����Ă���ꍇ�́A�Č����J�n��x�点�A���̃z�X�g�̗\���
		// ���߂ɑ҂�
		sbInterval = (u32)DWC_SB_UPDATE_INTERVAL_MSEC+DWC_SB_UPDATE_INTERVAL_ADD_MSEC*DWCi_GetMatchCnt()->gt2NumConnection;
	  }else{
		if (DWCi_GetMatchCnt()->sbUpdateFlag == DWC_SB_UPDATE_INTERVAL_SHORT){
		  sbInterval = DWC_SB_UPDATE_INTERVAL_SHORT_MSEC;
		}else{
		  sbInterval = DWC_SB_UPDATE_INTERVAL_MSEC;
		}
	  }
	  // DWCi_GetMatchCnt()->sbUpdateCount++;
	  if (OS_TicksToMilliSeconds( OS_GetTick() - DWCi_GetMatchCnt()->sbUpdateTick ) > sbInterval){
		// SB�T�[�o�A�b�v�f�[�g���s��
		sbError = DWCi_SBUpdateAsync(DWCi_GetMatchCnt()->reqProfileID);
		if(DWCi_HandleSBError(sbError)){ return; }
		DWCi_GetMatchCnt()->sbUpdateFlag = 0;
	  }
	  break;
    case DWC_MATCH_STATE_CL_GT2:  // GT2�R�l�N�V�����m����
        if (DWCi_GetMatchCnt()->nnFinishTime){
            // NN�J�n���ł���(nnFinishTime > 0)�ɂȂ�Ȃ�
            if (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->nnFinishTime) > DWC_WAIT_GT2_CONNECT_TIMEOUT){
                // NAT�O�̑����NAT�l�S�V�G�[�V����������������A
                // ���肩��gt2Connect���Ȃ��Ȃ��Ԃ��Ă��Ȃ��ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout: wait gt2Connect().\n");

                DWCi_GetMatchCnt()->nnFinishTime = 0;
                // �S�ẴN���C�A���g�������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�̓G���[�I������B
                if (!DWCi_CancelPreConnectedClientProcess(DWCi_GetMatchCnt()->sbPidList[0]))
                    return;
            }
        }
        else if ((DWCi_GetMatchCnt()->cmdCnt.command == DWC_MATCH_COMMAND_TELL_ADDR) &&
                 (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->cmdCnt.sendTime) > DWC_MATCH_CMD_RTT_TIMEOUT)){
                DWC_Printf(DWC_REPORTFLAG_DEBUG,
                           "RTT Timeout with DWC_MATCH_STATE_CL_GT2.\n" );
            // ����NAT���̑��肩��gt2Connect���Ȃ��Ȃ��Ԃ��Ă��Ȃ��ꍇ
            DWCi_GetMatchCnt()->cmdCnt.count++;
            if (DWCi_GetMatchCnt()->cmdCnt.count > DWC_MATCH_CMD_RETRY_MAX){
                // �đ��񐔃I�[�o�[�̏ꍇ
                DWCi_StopResendingMatchCommand();  // �R�}���h�đ���~

                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Stop resending command %d.\n",
                           DWC_MATCH_COMMAND_TELL_ADDR);
                    
                // �S�ẴN���C�A���g�������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�̓G���[�I������B
                if (!DWCi_CancelPreConnectedClientProcess(DWCi_GetMatchCnt()->sbPidList[0]))
                    return;
                break;
            }

            // �A�h���X�ʒm�R�}���h�̍đ����s��
            result =
                DWCi_SendMatchCommand(DWC_MATCH_COMMAND_TELL_ADDR,
                                      DWCi_GetMatchCnt()->cmdCnt.profileID,
                                      DWCi_GetMatchCnt()->cmdCnt.ip,
                                      DWCi_GetMatchCnt()->cmdCnt.port,
                                      DWCi_GetMatchCnt()->cmdCnt.data,
                                      DWCi_GetMatchCnt()->cmdCnt.len);
            if (DWCi_HandleMatchCommandError(result)) return;
        }
        break;

    case DWC_MATCH_STATE_SV_OWN_NN:  // NAT�l�S�V�G�[�V������
        if ((DWCi_GetMatchCnt()->cmdCnt.command == DWC_MATCH_COMMAND_RESV_OK) &&
            (((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) &&
              (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->cmdCnt.sendTime) > DWC_MATCH_RESV_KEEP_TIME_ANYBODY)) ||
             ((DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_ANYBODY) &&
              (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->cmdCnt.sendTime) > DWC_MATCH_RESV_KEEP_TIME_FRIEND)))){
                // �\������F�����N���C�A���g���A���܂Ōo���Ă�NN�ڑ���
                // ���Ȃ��ꍇ�A�\����N���A���ď�����Ԃɖ߂�B
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Reservation timeout. Cancel reservation.\n");

                DWCi_StopResendingMatchCommand();  // �R�}���h�đ���~
                
                // �V�K�ڑ��̎󂯕t���������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�́A�V�K�ڑ��N���C�A���g��
                // �ڑ��L�����Z���Ƃ��ď�������B
                if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection+1]))
                    return;
        }
        break;

    case DWC_MATCH_STATE_SV_WAIT_CL_LINK:  // �N���C�A���g���m�̐ڑ������҂�
        if ((DWCi_GetMatchCnt()->cmdCnt.command == DWC_MATCH_COMMAND_LINK_CLS_REQ) &&
            (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->cmdCnt.sendTime) > DWC_MATCH_LINK_CLS_TIMEOUT)){
            // �N���C�A���g���m�̐ڑ����Ȃ��Ȃ��I���Ȃ���
            DWCi_GetMatchCnt()->cmdCnt.count++;
            if (DWCi_GetMatchCnt()->cmdCnt.count > 0){ // [sasakit] retry�̈Ӗ������܂�Ȃ��̂ŁA��蒼���Ȃ��B
                // �đ��񐔃I�[�o�[�̏ꍇ�B
                DWCi_StopResendingMatchCommand();  // �R�}���h�đ���~

                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Wait clients connecting timeout.\n");

                if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
                    // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�́A
                    // �ڑ���Ԃ��p������K�v������̂ŁA�V�K�ڑ��N���C�A���g
                    // �݂̂��N���[�Y���đ�����
                    if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection]))
                        return;
                }
                else {
                    DWCi_RestartFromTimeout();  // �}�b�`���C�N���ŏ������蒼��
                }
                break;
            }

            // �N���C�A���g���m�̐ڑ��v���R�}���h���������ׂ�������Ȃ��̂ŁA
            // �R�}���h�̍đ����s��
            result =
                DWCi_SendMatchCommand(DWC_MATCH_COMMAND_LINK_CLS_REQ,
                                      DWCi_GetMatchCnt()->cmdCnt.profileID,
                                      DWCi_GetMatchCnt()->cmdCnt.ip,
                                      DWCi_GetMatchCnt()->cmdCnt.port,
                                      DWCi_GetMatchCnt()->cmdCnt.data,
                                      DWCi_GetMatchCnt()->cmdCnt.len);
            if (DWCi_HandleMatchCommandError(result)) return;
        }
        break;

    case DWC_MATCH_STATE_CL_WAITING:
        if ((DWC_GetState() == DWC_STATE_MATCHING) &&
            (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->clWaitTime) > DWC_MATCH_CL_WAIT_TIMEOUT)){
            // �N���C�A���g���҂���Ԃ̎��Ɉ�莞�Ԃ��o�߂����ꍇ�A
            // �T�[�o�Ƀ|�[�����O�p�P�b�g�𑗂�
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "No data from server %d/%d.\n",
                       DWCi_GetMatchCnt()->clWaitTimeoutCount, DWC_MATCH_CMD_RETRY_MAX);
                
            if (DWCi_GetMatchCnt()->clWaitTimeoutCount >= DWC_MATCH_CMD_RETRY_MAX){
                // 5�񑗂��Ă��T�[�o����ԓ����Ȃ������ꍇ�̓}�b�`���C�N����蒼���B
                // �S�ẴN���C�A���g�������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�̓G���[�I������B
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Timeout: Connection to server was shut down.\n");
                    
                if (!DWCi_CancelPreConnectedClientProcess(DWCi_GetMatchCnt()->sbPidList[0]))
                    return;
            }
            else {
                result =
                    DWCi_SendMatchCommand(DWC_MATCH_COMMAND_CL_WAIT_POLL,
                                          DWCi_GetMatchCnt()->sbPidList[0],
                                          DWCi_GetMatchCnt()->qr2IPList[0],
                                          DWCi_GetMatchCnt()->qr2PortList[0],
                                          NULL, 0);
                if (DWCi_HandleMatchCommandError(result)) return;

                DWCi_GetMatchCnt()->clWaitTimeoutCount++;
                // �R�}���h�̉����^�C���A�E�g���Ԍo�ߌ�Ɏ��̃^�C���A�E�g��
                // ��������悤�A�^�C���A�E�g�v���J�n���Ԃ𒲐�����
                DWCi_GetMatchCnt()->clWaitTime =
                    OS_GetTick()-OS_MilliSecondsToTicks(DWC_MATCH_CL_WAIT_TIMEOUT-DWC_MATCH_CMD_RTT_TIMEOUT);
            }
        }
        break;

    default:
        break;
    }

    if (((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_NN) ||
         (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_NN)) &&
        DWCi_GetMatchCnt()->nnFailedTime &&
        (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->nnFailedTime) > DWC_WAIT_NN_RETRY_TIMEOUT)){
        // NN���s��A���܂ł����g���C�������A�^�C���A�E�g�ɂȂ����ꍇ
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout : wait NN retry.\n");

        // NAT�l�S�V�G�[�V���������R�[���o�b�N���A���肩��̉��������G���[��
        // �Ăяo��
        DWCi_NNCompletedCallback(nr_deadbeatpartner, NULL, NULL, &DWCi_GetMatchCnt()->nnInfo);
    }
    /*** ���^�C���A�E�g���� ***/


    /*** ��GameSpy �eSDK�X�V���� ***/
    if (DWCi_GetMatchCnt()->sbObj){
#if 1
        // �����ԃ}�X�^�[�T�[�o�ƒʐM���Ȃ���sbe_connecterror�̃G���[��
        // �Ԃ����A�����ł͖�������B
        // �}�X�^�[�T�[�o�Ƃ̒ʐM���s�����ɐؒf����Ă����玩���I�ɍĐڑ������B
        s_sbCallbackLevel = 0;
        s_needSbFree = FALSE;
        
        (void)ServerBrowserThink(DWCi_GetMatchCnt()->sbObj);
       
        if(s_needSbFree)
        {
            ServerBrowserFree(DWCi_GetMatchCnt()->sbObj);
            DWCi_GetMatchCnt()->sbObj = NULL;
        }

#else
        sbError = ServerBrowserThink(DWCi_GetMatchCnt()->sbObj);
        if (DWCi_HandleSBError(sbError)) return;
#endif
		// SB���ڑ���Ԃł���LimitUpdate���ł��������ԉ������Ȃ��ꍇ�ɂ̓}�b�`���O���G���[�I��������
        if(DWCi_GetMatchCnt()->sbObj) { 
            if(ServerBrowserState(DWCi_GetMatchCnt()->sbObj) != sb_disconnected &&
               DWCi_GetMatchCnt()->sbUpdateRequestTick != 0 &&
               OS_GetTick() > DWCi_GetMatchCnt()->sbUpdateRequestTick) {
                DWCi_StopMatching(DWC_ERROR_NETWORK, DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_SB+DWC_ECODE_TYPE_NETWORK);
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "ServerBrowserLimitUpdate timeout.\n" );
            }
        }
    }

	DWCi_SendStateChanged(DWCi_GetMatchCnt()->qr2Obj); // �X�e�[�g�`�F���W����莞�Ԃ��Ƃɑ���

    // NNCancel��A�L�����Z�����ꂽcookie�ɑΉ�����NATNegotiator��
    // NNThink()����Remove�����̂ŁA����܂ł�gt2Think()����
    // UnrecognizedCallback����NNProcessData()���Ăяo�����͍̂D�܂����Ȃ��B
    // ���̂��߁Agt2Think()����NNThink()�̕����ɌĂяo���B
    s_bNNFreeNegotiateList = FALSE;
    s_bInNNThink = TRUE;
    NNThink();
    s_bInNNThink = FALSE;

    if(s_bNNFreeNegotiateList == TRUE)
    {
        s_bNNFreeNegotiateList = FALSE;
        NNFreeNegotiateList();
    }

    if (DWCi_GetMatchCnt()->pGt2Socket){
        gt2Think(*DWCi_GetMatchCnt()->pGt2Socket);
    }
    /*** ��GameSpy �eSDK�X�V���� ***/


    /*** ���̑����Q�[���t���[�������i�eSDK�X�V��ɌĂяo���K�v��������́j ***/
    if ((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_WAIT_CLOSE) &&
        (OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->closedTime) > (DWC_MATCH_CMD_RTT_TIMEOUT >> 1))){
        // ���z�X�g����N���[�Y���ꂽ�ꍇ�̗D��\���T�[�o�ʒm�҂���
        // �^�C���A�E�g�����ꍇ�A�ŏ�����}�b�`���C�N����蒼��
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "RTT Timeout with DWCi_MatchProcess.\n" );
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout : Wait prior profileID.\n");
        
        if (DWCi_ResumeMatching()) return;
    }

    // �}�b�`���C�N����������������
    if (!DWCi_ProcessMatchSynTimeout()) return;

    // �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z��������������
    if (!DWCi_ProcessCancelMatchSynTimeout()) return;

    // �w��l���ȉ��ł̃}�b�`���C�N�����I�v�V��������
    DWCi_ProcessOptMinComp();

#ifdef DWC_STOP_SC_SERVER
    // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���ŁA�}�b�`���C�N���ߐ؂�t���O��
    // �����Ă�����A�}�b�`���C�N����������A�ő�ڑ��l�������ݐڑ��l����
    // ���킹�ĈȌ�̐ڑ������ۂł���
    if (DWCi_GetMatchCnt()->stopSCFlag && (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_WAITING)){
        DWCi_GetMatchCnt()->qr2NumEntry = DWCi_GetMatchCnt()->gt2NumConnection;
        (void)DWCi_GPSetServerStatus();
        
        DWCi_GetMatchCnt()->stopSCFlag = 0;

        // �R�[���o�b�N�Ăяo��
        if (DWCi_GetMatchCnt()->stopSCCallback)
            DWCi_GetMatchCnt()->stopSCCallback(DWCi_GetMatchCnt()->stopSCParam);
    }
#endif
}


/*---------------------------------------------------------------------------*
  GT2�F���s�\���b�Z�[�W��M�R�[���o�b�N�֐�
  �����@�Fsocket  GT2�\�P�b�g�ւ̃|�C���^
          ip      ����z�X�g��IP
          port    ����z�X�g�̃|�[�g�ԍ�
          message ��M�f�[�^
          len     ��M�f�[�^�T�C�Y
  �߂�l�FGT2�̃��b�Z�[�W�łȂ����GT2False��Ԃ�
  �p�r�@�FGT2�F���s�\���b�Z�[�W��M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
GT2Bool DWCi_GT2UnrecognizedMessageCallback(GT2Socket socket,
                                            unsigned int ip,
                                            unsigned short port,
                                            GT2Byte* message,
                                            int len)
{
#pragma unused(socket)
	struct sockaddr_in saddr;

	if (!len || !message) return GT2False;

    //(void)memset(&saddr, 0, sizeof(saddr));
    MI_CpuClear32(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
   	saddr.sin_addr.s_addr = ip;
    saddr.sin_port = htons(port);

    if (((message[0] == QR_MAGIC_1) && (message[1] == QR_MAGIC_2))
        || (message[0] == '\\')){
        // QR2���b�Z�[�W��M�̏ꍇ
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "GT2 Unrecognized : Received QR2 data.\n");
        
        // �K��NULL�I�[�ɂ�����
        // [note]
        // �萔GSI_UNICODE����`����Ă��Ȃ����NULL�I�[�m�F�͕K�v�Ȃ����A
        // ��`���ꂽ�ꍇ�́Aqr2_parse_query()�̐�ŏI�[��NULL������T��
        // ����������̂ŁA�o�O�̌����ƂȂ�
        if (DWCi_GetMatchCnt()->qr2Obj){
         	qr2_parse_query(DWCi_GetMatchCnt()->qr2Obj, (gsi_char *)message, len, (struct sockaddr *)&saddr);
        }
        else {
            DWC_Printf( DWC_REPORTFLAG_MATCH_NN, "  ignore qr2 message.\n" );
        }
    }
    else if (!memcmp(message, NNMagicData, NATNEG_MAGIC_LEN)){
        // NN���b�Z�[�W��M�̏ꍇ
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "GT2 Unrecognized : Received NN data.\n");
        NNProcessData((char *)message, len, &saddr);
    }
    else if ((message[0] == 0xfe) && (message[1] == 0xfe))
    {
        // GT2Connection�̂Ȃ�GT2Packet
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "GT2 Unrecognized : Not Connected gt2 data.\n");

        // ������GT2False��Ԃ��Ȃ��ƁAConnectAttemptCallback�̌Ăяo����
        // �i��ł���Ȃ��̂Œ��ӁI�I
        return GT2False;
    }
    else {
        // �F���s�\���b�Z�[�W����M�����ꍇ
        // �܂��R�l�N�V�����̂Ȃ����肩��f�[�^����M�����ꍇ�́A
        // GT2�p�P�b�g�p�̃}�W�b�N�i���o�[���擪�ɂ����Ă������ɗ���
        DWC_Printf(DWC_REPORTFLAG_WARNING, "GT2 Unrecognized : Received unrecognized data.\n");

        return GT2False;
    }

    return GT2True;
}


/*---------------------------------------------------------------------------*
  GT2�ڑ��v����M�R�[���o�b�N�֐�
  �����@�Fsocket     GT2�\�P�b�g�ւ̃|�C���^
          connection �󂯕t�����R�l�N�V�����̍\���̃|�C���^
          ip         ����z�X�g��IP
          port       ����z�X�g�̃|�[�g�ԍ�
          latency    ����z�X�g�Ƃ̃��C�e���V�i�������ԁj
          message    ��M�f�[�^
          len        ��M�f�[�^�T�C�Y
  �߂�l�F�Ȃ�
  �p�r�@�FGT2�ڑ��v����M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
void DWCi_GT2ConnectAttemptCallback(GT2Socket socket,
                                    GT2Connection connection,
                                    unsigned int ip,
                                    unsigned short port,
                                    int latency,
                                    GT2Byte* message,
                                    int len)
{
#pragma unused(socket)
#pragma unused(message)
#pragma unused(len)
    int index;
    GT2Bool result;
    GT2Connection* pGt2Con;
    DWCConnectionInfo* pConInfo;

    if (!DWCi_GetMatchCnt() || (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_GT2) ||
        (DWCi_GetMatchCnt()->cancelState != DWC_MATCH_CANCEL_STATE_INIT)){
        // �L�����Z������Ȃǂ�GT2Connect���ꂽ�ꍇ��Reject����
        gt2Reject(connection, (const GT2Byte *)"Init state", -1);
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Reject was called : Init state\n");
        return;
    }

    // �R�l�N�V�������X�g�̋󂫂�����
    index = DWCi_GT2GetConnectionListIdx();
	if (index == -1){
        // ���ɃR�l�N�V�����������ς��̏ꍇ��Reject����
        gt2Reject(connection, (const GT2Byte *)"Server full", -1);
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Reject was called : Server full\n");

        // �}�b�`���C�N���G���[�I������
        DWCi_StopMatching(DWC_ERROR_NETWORK,
                          DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_GT2+DWC_ECODE_TYPE_CONN_OVER);
    }
    else {
        if ((ip != DWCi_GetMatchCnt()->ipList[DWCi_GetMatchCnt()->gt2NumConnection]) ||
            (port != DWCi_GetMatchCnt()->portList[DWCi_GetMatchCnt()->gt2NumConnection])){
            if (message[0] &&
                (strtoul((const char *)message, NULL, 10) == DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection])){
                // �܂�NAT�l�S�V�G�[�V�������������Ă��Ȃ��̂ɁA
                // ���gt2Connect()����M�����ꍇ
                DWC_Printf(DWC_REPORTFLAG_WARNING,
                           "gt2Connect() came before NN complete.\n");

                // IP�ƃ|�[�g�͂����Ń��X�g�ɐݒ肷��
                DWCi_GetMatchCnt()->ipList[DWCi_GetMatchCnt()->gt2NumConnection]   = ip;
                DWCi_GetMatchCnt()->portList[DWCi_GetMatchCnt()->gt2NumConnection] = port;
            }
            else {
                // �S�R�֌W�Ȃ��z�X�g�����gt2Connect()�������ꍇ�͉������Ȃ�
                gt2Reject(connection, (const GT2Byte *)"Unknown connect attempt", -1);
                DWC_Printf(DWC_REPORTFLAG_WARNING,
                           "gt2Reject was called : Unknown connect attempt from %s\n",
                           gt2AddressToString(ip, port, NULL));
                return;
            }
        }

        
        // NN�I�����̎������N���A����
        DWCi_GetMatchCnt()->nnFinishTime = 0;

        // �R�l�N�V�����󂯓���
	    result = gt2Accept(connection, DWCi_GetMatchCnt()->gt2Callbacks);
        if (result == GT2False){
            // [test]
            // ���̃��C�u�����ɂ����ẮAConnect�󂯕t����A��Accept����̂�
            // result��GT2False�ɂȂ邱�Ƃ́A�X���b�h�Ɋ��荞�݂ł�����Ȃ�
            // ����͂��肦�Ȃ�
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Unexpected failure to gt2Accept.\n");
                
            // ���ɐؒf����Ă����ꍇ�A�G���[�Ń}�b�`���C�N�������~�߂�
            // [todo]
            // �T�[�o�N���C�A���g�}�b�`���C�N�ȊO�̏ꍇ�́A
            // �}�b�`���C�N����蒼���悤�ɕύX����K�v������
            DWCi_StopMatching(DWC_ERROR_NETWORK,
                              DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_OTHER);
            return;
        }
        
	    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                   "Accepted connection from %s (latency %d)\n",
                   gt2AddressToString(ip, port, NULL), latency);

        // TELL_ADDR�R�}���h������ȏ�đ����Ȃ��悤�ɍđ��R�}���h���N���A����
        DWCi_StopResendingMatchCommand();

        if (DWCi_GetMatchCnt()->gt2NumConnection == 0){
            // ��ŃT�[�o�ɓ`����ׂɁA�T�[�o�Ƃ̊Ԃ̕Г����C�e���V���L�^����
            DWCi_GetMatchCnt()->baseLatency =
                (u16)(((latency >> 1) < 0xffff) ? (latency >> 1) : 0xffff);
        }

        // GT2�R�l�N�V�������X�g�ƃR�l�N�V������񃊃X�g�̗v�f�ւ̃|�C���^�擾
        pGt2Con  = DWCi_GetGT2ConnectionByIdx(index);
        pConInfo = DWCi_GetConnectionInfoByIdx(index);

        // �󂯕t�����R�l�N�V���������X�g�ɒǉ�
        *pGt2Con = connection;
        DWCi_GetMatchCnt()->gt2NumConnection++;

        // �R�l�N�V�����f�[�^���Z�b�g����
        pConInfo->index   = (u8)index;
        pConInfo->aid     = DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->gt2NumConnection-1];
        pConInfo->reserve = 0;
        pConInfo->param   = NULL;

        // GT2Connection�\���̂̃����odata�i���[�U�f�[�^�o�^�p�j�ɁA
        // ���̃R�l�N�V�����̏����Z�b�g���Ă���
    	gt2SetConnectionData(connection, pConInfo);

        // Connect����鑤�͏�ɐV�K�ڑ��N���C�A���g
        DWCi_PostProcessConnection(DWC_PP_CONNECTION_CL_GT2_ACCEPT);
    }
}


/*---------------------------------------------------------------------------*
  GT2�ڑ������R�[���o�b�N�֐�
  �����@�Fconnection ���������R�l�N�V�����̍\���̃|�C���^
          result     GT2���ʌ^
          message    ��M�f�[�^
          len        ��M�f�[�^�T�C�Y
  �߂�l�F�Ȃ�
  �p�r�@�FGT2�ڑ������R�[���o�b�N
 *---------------------------------------------------------------------------*/
void DWCi_GT2ConnectedCallback(GT2Connection connection,
                               GT2Result result,
                               GT2Byte* message,
                               int len)
{
#pragma unused(len)
    char pidStr[12];
    int  index;
    GT2Connection* pGt2Con;
    GT2Result gt2Result;
    DWCConnectionInfo* pConInfo;

    if (!DWCi_GetMatchCnt() ||
        ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_GT2) &&
         (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_SV_OWN_GT2))){
        // �L�����Z������Ȃǂɗ��Ă���������
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                   "gt2ConnectedCallback: Already cancel\n");
        return;
    }

	if (result != GT2Success){
		if (!message) message = (GT2Byte *)"";
		DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "GT2 connect failed %d: %s\n", result, message);

        if (result == GT2DuplicateAddress){
            // result = GT2DuplicateAddress�̎��́Aconnect�����̒ʒm���͂��O��
            // �^�C���A�E�g��connect���g���C���Ă��܂����ꍇ�ƍl������̂�
            // �������Ȃ�
            return;
        }
        else if (result == GT2TimedOut){
            // �^�C���A�E�g�̏ꍇ�̓��g���C����
            DWCi_GetMatchCnt()->gt2ConnectCount++;
            if (DWCi_GetMatchCnt()->gt2ConnectCount > DWC_MATCH_CMD_RETRY_MAX){
                // �đ��񐔃I�[�o�[�̏ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Connect() retry over.\n");
                DWCi_GetMatchCnt()->gt2ConnectCount = 0;

                // �V�K�ڑ��̎󂯕t���������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�́A�V�K�ڑ��N���C�A���g��
                // �ڑ��L�����Z���Ƃ��ď�������B
                DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->qr2NNFinishCount]);
                return;
            }
                
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Retry to gt2Connect.\n");

            (void)OS_SNPrintf(pidStr, sizeof(pidStr), "%u", DWCi_GetMatchCnt()->profileID);
            gt2Result =
                gt2Connect(*DWCi_GetMatchCnt()->pGt2Socket,
                           NULL,
                           gt2AddressToString(
                               DWCi_GetMatchCnt()->ipList[DWCi_GetMatchCnt()->qr2NNFinishCount],
                               DWCi_GetMatchCnt()->portList[DWCi_GetMatchCnt()->qr2NNFinishCount],
                               NULL),
                           (GT2Byte *)pidStr,
                           -1,
                           DWC_GT2_CONNECT_TIMEOUT,
                           DWCi_GetMatchCnt()->gt2Callbacks,
                           GT2False);
            if ( gt2Result == GT2OutOfMemory )
            {
                DWCi_HandleGT2Error(gt2Result);
                return;
            }
            else if ( gt2Result == GT2Success )
            {
                // �����̂Ƃ��́A�R�[���o�b�N��҂B
            }
            else if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->qr2NNFinishCount]))
            {
                // gt2Connect�ɂ����Ȃ莸�s�����Ƃ��́A���Z�b�g���Ď��̐ڑ���҂B
                return;
            }
        }
        else {
            // Reject���ꂽ�ꍇ�́A�V�K�ڑ��̎󂯕t���������I������
            // �}�b�`���C�N���ĊJ����B����ȊO�̃G���[�̂Ƃ������l�B
            // �i�����Ŗ������Ă��A����ȊO��Socket�ŃG���[��
            // �o��͂��Ȃ̂ŁA�����Ɋ��҂���j
            // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�́A�V�K�ڑ��N���C�A���g��
            // �ڑ��L�����Z���Ƃ��ď�������B
            if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection+1]))
                return;
        }
	}
    else {
    	DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "GT2 connected.\n");

        // �R�l�N�V�������X�g�̋󂫂�����
        index = DWCi_GT2GetConnectionListIdx();

        if (index == -1){
            // �Q�[���ŃR�l�N�V�������N���[�Y�����Ƀ}�b�`���C�N���J��Ԃ����ꍇ
            // �R�l�N�V�������X�g�������ς��ɂȂ邱�Ƃ͂��蓾��B
            DWC_Printf(DWC_REPORTFLAG_WARNING,
                       "Don't continue matching without closing connections!!\n");

            // �}�b�`���C�N���G���[�I������
            DWCi_StopMatching(DWC_ERROR_NETWORK,
                              DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_GT2+DWC_ECODE_TYPE_CONN_OVER);
            return;
        }

        // GT2�R�l�N�V�������X�g�ƃR�l�N�V������񃊃X�g�̗v�f�ւ̃|�C���^�擾
        pGt2Con  = DWCi_GetGT2ConnectionByIdx(index);
        pConInfo = DWCi_GetConnectionInfoByIdx(index);

        // �󂯕t����ꂽ�R�l�N�V���������X�g�ɒǉ�
        *pGt2Con = connection;
        DWCi_GetMatchCnt()->gt2NumConnection++;

        // �R�l�N�V�����f�[�^���Z�b�g����
        pConInfo->index   = (u8)index;
        pConInfo->reserve = 0;
        pConInfo->param   = NULL;
        pConInfo->aid     = DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->gt2NumConnection];

        // GT2Connection�\���̂̃����odata�i���[�U�f�[�^�o�^�p�j�ɁA
        // ���̃R�l�N�V�����̏����Z�b�g���Ă���
	    gt2SetConnectionData(connection, pConInfo);

        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_GT2){
            // �T�[�o���̏ꍇ
            DWCi_PostProcessConnection(DWC_PP_CONNECTION_SV_CONNECT);
        }
        else {
            // �ڑ��ς݃N���C�A���g�̏ꍇ
            DWCi_PostProcessConnection(DWC_PP_CONNECTION_CL_GT2_CONNECT);
        }
    }
}


/*---------------------------------------------------------------------------*
  GP�o�f�B���b�Z�[�W��M�R�[���o�b�N�֐�
  �����@�Fpconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          profileID   ���b�Z�[�W���M���̃v���t�@�C��ID
          message     ��M�������b�Z�[�W�{��
  �߂�l�F�Ȃ�
  �p�r�@�FGP�o�f�B���b�Z�[�W����M�����炻�̃R�[���o�b�N���ŌĂяo����A
          �}�b�`���C�N�������ɌŗL�̏������s��
 *---------------------------------------------------------------------------*/
void DWCi_MatchGPRecvBuddyMsgCallback(GPConnection* pconnection,
                                      int profileID,
                                      char* message)
{
#pragma unused(pconnection)
    char tmpStr[16];
    // [arakit] main 051011
    u32 data[DWC_MATCH_COMMAND_ADD_MESSAGE_NUM];
    int i;

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "<GP> RECV-0x%02x <- [--------:-----] [pid=%u]\n",
               *message, profileID);

    // ������Ŋi�[����Ă���ǉ����b�Z�[�W��u32�ɕϊ����Ĕz��Ɋi�[����
    for (i = 0; i < DWC_MATCH_COMMAND_ADD_MESSAGE_NUM; i++){ 
        if (DWCi_GetGPBuddyAdditionalMsg(tmpStr, message+1, i) == -1) break;
        data[i] = strtoul(tmpStr, NULL, 10);
    }

    // ��M�����R�}���h����������B
    // �G���[�����͊֐����ōs���Ă���
    (void)DWCi_ProcessRecvMatchCommand((u8)*message, profileID, 0, 0, data, i);
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�R�}���h�đ���~�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�R�}���h�̑��M����\���̂̈ꕔ�����o�����������A
          �R�}���h�̍đ����~�߂�
 *---------------------------------------------------------------------------*/
static void DWCi_StopResendingMatchCommand(void)
{

    DWCi_GetMatchCnt()->cmdCnt.command = DWC_MATCH_COMMAND_DUMMY;
    DWCi_GetMatchCnt()->cmdCnt.count   = 0;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�G���[�I���֐�
  �����@�Ferror     DWC�G���[���
          errorCode �G���[�R�[�h
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�������ɃG���[�����������ꍇ�ɁA�����𒆒f����
 *---------------------------------------------------------------------------*/
void DWCi_StopMatching(DWCError error, int errorCode)
{

    if (!DWCi_GetMatchCnt() || (error == DWC_ERROR_NONE)) return;

    // �R�l�N�V�����͑S�ăN���[�Y����
    DWCi_CloseAllConnectionsByTimeout();

    DWCi_SetError(error, errorCode);

    // GP�X�e�[�^�X���I�����C����ԂɃZ�b�g����
    // [arakit] main 051007
    (void)DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);

    // �}�b�`���C�N�����R�[���o�b�N�֐��Ăяo��
    DWCi_GetMatchCnt()->matchedCallback(error,
                                 FALSE,
                                 DWCi_GetMatchCnt()->cbEventPid ? FALSE : TRUE,
                                 (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) ? TRUE : FALSE, 
                                 DWCi_GetFriendListIndex(DWCi_GetMatchCnt()->cbEventPid),
                                 DWCi_GetMatchCnt()->matchedParam);

    DWCi_CloseMatching();
}


/*---------------------------------------------------------------------------*
  QR2�p�L�[�������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N������A�ʏ�̃I�����C����Ԃɖ߂��Ă���SB�T�[�o�A�b�v�f�[�g��
          ��錟���Ɉ����|����Ȃ��悤�ɂ��邽�߁AQR2�L�[������������
 *---------------------------------------------------------------------------*/
void DWCi_ClearQR2Key(void)
{

    // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o���A�܂��}�b�`���C�N�������̂�
    // �����ɗ���悤�ɍ���Ă͂����Ȃ��I�I

    if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_SV){
        DWCi_GetMatchCnt()->qr2NNFinishCount = 0;  // �ʏ�͂����ɗ��鎞�ɂ�0�ɂȂ��Ă���
        DWCi_GetMatchCnt()->qr2NumEntry      = 0;
        //DWCi_GetMatchCnt()->qr2Reservation   = 0;

        // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
        qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�������������p�P�b�g��M�����֐�
  �����@�Faid  ���M���z�X�g��AID
          type DWC Transport�p�P�b�g�^�C�v�BDWC_SEND_TYPE_*�Œ�`�����
          data �w�b�_�ɕt�������f�[�^�̐擪�ւ̃|�C���^�B
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�������������̃p�P�b�g��M���̏������s��
 *---------------------------------------------------------------------------*/
void DWCi_ProcessMatchSynPacket(u8 aid, u16 type, u8* data)
{
    u8  aidIdx;
    u32 bitmask;
    int i;

    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Received SYN %d packet from aid %d.\n",
               type-DWC_SEND_TYPE_MATCH_SYN, aid);

    switch (type){
    case DWC_SEND_TYPE_MATCH_SYN:
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAITING){
            // SYN�p�P�b�g���n�߂Ď�M������
            if (data[0] == 1){
                // �������V�K�ڑ��N���C�A���g�̏ꍇ�B
                // �S�ẴN���C�A���g�Ƃ̃R�l�N�V�������m�����ꂽ�B
                // �R�[���o�b�N�p�̃v���t�@�C��ID��0�B
                DWCi_GetMatchCnt()->cbEventPid = 0;
            }

            aidIdx = data[1];
            // ������AID��o�^����
            DWCi_GetMatchCnt()->aidList[aidIdx] = data[2];
            // �����̃v���t�@�C��ID���o�^���Ă���
            DWCi_GetMatchCnt()->sbPidList[aidIdx] = DWCi_GetMatchCnt()->profileID;

            // �w��l���ȉ��ł̃}�b�`���C�N�����I�v�V�����̂��߂ɁA
            // �ڑ���]�l�������ۂ̐ڑ����ɍ��킹��
            if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
                (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
                DWCi_GetMatchCnt()->qr2NumEntry = (u8)DWCi_GetMatchCnt()->gt2NumConnection;
            }

            // �N���C�A���g�̃^�C���A�E�g�����񐔃J�E���^������������
            DWCi_GetMatchCnt()->clWaitTimeoutCount = 0;

            // �}�b�`���C�N����������Ԃɐi��
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SYN);
        }

        // �T�[�o��SYN-ACK�p�P�b�g�𑗐M����
        DWCi_SendMatchSynPacket(aid, DWC_SEND_TYPE_MATCH_SYN_ACK);
        break;

    case DWC_SEND_TYPE_MATCH_SYN_ACK:
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_SYN){
            // SYN-ACK�҂���Ԃ̏ꍇ
            // SYN-ACK��MAID�r�b�g�}�b�v�Z�b�g
            DWCi_GetMatchCnt()->synAckBit |= 1 << aid;

            // �N���C�A���g�Ƃ̃��C�e���V���ő�Ȃ�X�V
            if ((data[0] | (data[1] << 8)) > DWCi_GetMatchCnt()->baseLatency){
                DWCi_GetMatchCnt()->baseLatency = (u16)(data[0] | (data[1] << 8));
            }

            bitmask = DWCi_GetAIDBitmask(FALSE);  // ����pAID�r�b�g�}�X�N���擾
            if (DWCi_GetMatchCnt()->synAckBit == bitmask){
                // �S�N���C�A���g����SYN-ACK���Ԃ��Ă�����A�S�N���C�A���g��
                // ACK�𑗐M����
                for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                    DWCi_SendMatchSynPacket(DWCi_GetMatchCnt()->aidList[i],
                                            DWC_SEND_TYPE_MATCH_ACK);
                }

                // �}�b�`���C�N���������҂���Ԃɐi��
                DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_SYN_WAIT);

                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                           "Wait max latency %d msec.\n",
                           DWCi_GetMatchCnt()->baseLatency);
            }
        }
        else {
            // ACK�𑗐M������A�ʂɒx��Ă���SYN-ACK�ɑΉ�����ꍇ
            // ������ACK��ԐM����
            DWCi_SendMatchSynPacket(aid, DWC_SEND_TYPE_MATCH_ACK);
        }
        break;

    case DWC_SEND_TYPE_MATCH_ACK:
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_SYN){
            // �}�b�`���C�N�����������Ăяo��
            DWCi_PostProcessConnection(DWC_PP_CONNECTION_SYN_FINISH);
        }
        break;
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����p�R�l�N�V�����N���[�Y�����֐�
  �����@�Ferror     �N���[�Y�����̃G���[��DWCError�^�ɕϊ������l
          errorCode �G���[�R�[�h
          profileID �N���[�Y�z�X�g�̃v���t�@�C��ID
  �߂�l�FTRUE :�}�b�`���C�N���̃L�����Z����^�C���A�E�g������̃N���[�Y�������A
          FALSE:�܂��ʂ̃N���[�Y������
  �p�r�@�F�}�b�`���C�N���ɃR�l�N�V�����N���[�Y�����������ꍇ��
          gt2ConnectionClose�R�[���o�b�N������Ăяo���A�N���C�A���g�Ƃ��Ă�
          �}�b�`���C�N�������ĊJ����B
          �܂��́A�}�b�`���C�N���L�����Z�����ꂽ�ꍇ�ɂ��N���[�Y���s����B
 *---------------------------------------------------------------------------*/
BOOL DWCi_ProcessMatchClosing(DWCError error, int errorCode, int profileID)
{
#pragma unused(profileID)

    if (DWC_GetState() != DWC_STATE_MATCHING) return FALSE;

    // [test]
    // gt2Connection���m������Ă���connected�R�[���o�b�N��
    // �Ăяo�����܂ł̋͂��Ȏ��Ԃɖ����ꂱ���ɗ����Ȃ�
    // �������Ȃ��A�Ƃ����΍􂪕K�v
    if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_ProcessMatchClosing: SC_SV.\n");
        return TRUE;
    }

    if (error){
        // �}�b�`���C�N�I�������Ăяo��
        DWCi_StopMatching(error, errorCode+DWC_ECODE_SEQ_MATCH);
        return TRUE;
    }

    // dwc_main.c��DWCi_GT2ClosedCallback()���ŌĂяo�����
    // DWCi_DeleteAID()�̓�����AaidList������Ďc���Ă���̂ŁA
    // �����ŃT�[�o��aid������������
    DWCi_GetMatchCnt()->aidList[0] = 0;

    if ((DWCi_GetMatchCnt()->cancelState == DWC_MATCH_CANCEL_STATE_EXEC) ||
        (DWCi_GetMatchCnt()->closeState == DWC_MATCH_CLOSE_STATE_CHANGE_TO_CLIENT) ||
        (DWCi_GetMatchCnt()->closeState == DWC_MATCH_CLOSE_STATE_TIMEOUT))
        // �����ŃL�����Z�������ꍇ�A�������̓N���[�Y�����ꍇ�͉������Ȃ�
        return TRUE;

    
    if (DWCi_GetMatchCnt()->nnInfo.cookie){
        // NAT�l�S�V�G�[�V�������Ȃ�L�����Z��
        NNCancel(DWCi_GetMatchCnt()->nnInfo.cookie);
        DWCi_GetMatchCnt()->nnInfo.cookie = 0;
    }

    if (DWCi_GetMatchCnt()->gt2NumConnection){
        // �܂��R�l�N�V�������c���Ă���ꍇ
        if (DWCi_GetMatchCnt()->closeState == DWC_MATCH_CLOSE_STATE_INIT){
            // ���z�X�g���珉�߂ăN���[�Y���ꂽ�ꍇ�A���z�X�g�����
            // �N���[�Y�ɂ��A�c��z�X�g�̃N���[�Y���ł��邱�Ƃ��L�^���A
            // �S�ẴR�l�N�V�������N���[�Y����
            DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_FORCED;
            gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);
        }
    }
    else {
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
            // �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g��
            // �N���[�Y���ꂽ��}�b�`���C�N���G���[�ŏI������
            // [todo]
            // �V�K�ڑ��N���C�A���g���T�[�o�ȊO�Ɛڑ����ɁA�T�[�o�ȊO��
            // �N���C�A���g�Ɛؒf�A�������͔������Ȃ��Ȃ��Ă��A
            // �}�b�`���C�N���ŏ������蒼���悤�ɂ��������ǂ�
            DWCi_StopMatching(DWC_ERROR_NETWORK,
                              DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_SC_CL_FAIL);
        }
        else if (DWCi_GetMatchCnt()->priorProfileID){
            // �D��\���T�[�o�̃v���t�@�C��ID���Z�b�g����Ă���ꍇ
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Reserve NN to %u.\n",
                       DWCi_GetMatchCnt()->priorProfileID);
            
            (void)DWCi_ResumeMatching();
        }
        else if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAITING){
            // ���ɃN���C�A���g�Ƃ��Đڑ����A�҂���Ԃɂ������z�X�g�̏ꍇ�A
            // �T�[�o����D��\��悪�����ė��邩������Ȃ��̂ŁA
            // ���΂炭�҂B
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Wait prior profileID.\n");

            // state��ύX���Ȃ���NN�v���Ȃǂ��󂯂Ă��܂��̂Ő�p��
            // state�Ɉڍs����
            DWCi_SetMatchStatus(DWC_MATCH_STATE_WAIT_CLOSE);
            DWCi_GetMatchCnt()->closedTime = OS_GetTick();
        }
        else {
            // �^�C���A�E�g�����m�������z�X�g����̃N���[�Y���A
            // �}�b�`���C�N���L�����Z�����ꂽ�z�X�g�̏ꍇ�͂����ɗ���̂ŁA
            // �}�b�`���C�N���ŏ������蒼��
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Restart matching immediately.\n");
            
            DWCi_RestartFromCancel(DWC_MATCH_RESET_RESTART);
        }
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�p�V�K�ڑ��N���C�A���g�N���[�Y�����֐�
  �����@�FclientPid �N���[�Y�����N���C�A���g�̃v���t�@�C��ID
  �߂�l�F�Ȃ�
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�̐V�K�ڑ��N���C�A���g�ڑ����ɁA
          �V�K�ڑ��N���C�A���g���N���[�Y�����ꍇ�ɁAgt2ConnectionClose
          �R�[���o�b�N������Ăяo���A�ڑ��ς݃N���C�A���g�ɃL�����Z����ʒm����B
          ���̊֐����Ăяo���͕̂K���T�[�o�B
 *---------------------------------------------------------------------------*/
void DWCi_ProcessMatchSCClosing(int clientPid)
{

    if (DWCi_GetMatchCnt()->closeState != DWC_MATCH_CLOSE_STATE_TIMEOUT){
        // �����ŃN���[�Y�����̂łȂ���ΐڑ��ς݃N���C�A���g��
        // �L�����Z���R�}���h�𑗐M���A�T�[�o�������L�����Z������
        DWCi_CancelPreConnectedServerProcess(clientPid);
    }
}


/*---------------------------------------------------------------------------*
  �z�X�g�f�[�^���X�g�폜�֐��i�v���t�@�C��ID�w��Łj
  �����@�FprofileID �폜�������z�X�g�̃v���t�@�C��ID
          numHost   �ڑ��z�X�g��
  �߂�l�FTRUE:�폜���s�����AFALSE:�w��̃v���t�@�C��ID�����X�g�ɂȂ�����
  �p�r�@�F�e��z�X�g�f�[�^���X�g����A�v���t�@�C��ID�Ŏw�肳�ꂽ�z�X�g��
          �f�[�^���폜����
 *---------------------------------------------------------------------------*/
BOOL DWCi_DeleteHostByProfileID(int profileID, int numHost)
{
    int i;

    if (!DWCi_GetMatchCnt()) return FALSE;

    for (i = 0; i < numHost; i++){
        if (DWCi_GetMatchCnt()->sbPidList[i] == profileID){
            DWCi_DeleteHostByIndex(i, numHost);
            return TRUE;
        }
    }

    return FALSE;
}


/*---------------------------------------------------------------------------*
  �z�X�g�f�[�^���X�g�폜�֐��i�C���f�b�N�X�w��Łj
  �����@�Findex   �폜�������z�X�g�̃C���f�b�N�X
          numHost �ڑ��z�X�g��
  �߂�l�F�폜�����z�X�g�̃v���t�@�C��ID�B�V���b�g�_�E����ɌĂ΂ꂽ��0��Ԃ��B
  �p�r�@�F�e��z�X�g�f�[�^���X�g����A�C���f�b�N�X�Ŏw�肳�ꂽ�z�X�g��
          �f�[�^���폜����
 *---------------------------------------------------------------------------*/
int DWCi_DeleteHostByIndex(int index, int numHost)
{
    int profileID;

    if (!DWCi_GetMatchCnt()) return 0;

    profileID = DWCi_GetMatchCnt()->sbPidList[index];

    // �L����AID�r�b�g�}�b�v�̃r�b�g�����낷
    DWCi_GetMatchCnt()->validAidBitmap &= ~(1 << DWCi_GetMatchCnt()->aidList[index]);
    // �L���R�l�N�V���������Z�b�g�ōX�V
    DWCi_SetNumValidConnection();

    if (index < numHost-1){
        // 1�O�ɂ��炷
        int i;
        for ( i = 0 ; i < numHost - index - 1 ; ++i )
        {
            DWCi_GetMatchCnt()->qr2IPList  [index+i] = DWCi_GetMatchCnt()->qr2IPList  [index+i+1];
            DWCi_GetMatchCnt()->qr2PortList[index+i] = DWCi_GetMatchCnt()->qr2PortList[index+i+1];
            DWCi_GetMatchCnt()->sbPidList  [index+i] = DWCi_GetMatchCnt()->sbPidList  [index+i+1];
            DWCi_GetMatchCnt()->ipList     [index+i] = DWCi_GetMatchCnt()->ipList     [index+i+1];
            DWCi_GetMatchCnt()->portList   [index+i] = DWCi_GetMatchCnt()->portList   [index+i+1];
            DWCi_GetMatchCnt()->aidList    [index+i] = DWCi_GetMatchCnt()->aidList    [index+i+1];
        }
    }

    if ( numHost > 0 )
    {
        DWCi_GetMatchCnt()->qr2IPList[numHost-1]   = 0;
        DWCi_GetMatchCnt()->qr2PortList[numHost-1] = 0;
        DWCi_GetMatchCnt()->sbPidList[numHost-1]   = 0;
        DWCi_GetMatchCnt()->ipList[numHost-1]      = 0;
        DWCi_GetMatchCnt()->portList[numHost-1]    = 0;
        DWCi_GetMatchCnt()->aidList[numHost-1]     = 0;
    }

    return profileID;
}


/*---------------------------------------------------------------------------*
  �S�ẴR�l�N�V�������擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�����ɐڑ����Ă���z�X�g���i�������܂܂Ȃ��j�B
          �I�t���C������0��Ԃ��B
  �p�r�@�F�����Ɛڑ����Ă���A�z�X�g�̐����擾����B
          �}�b�`���C�N�i�s�������݂̐ڑ��������̂܂ܕԂ��B
 *---------------------------------------------------------------------------*/
int DWCi_GetNumAllConnection(void)
{

    if (!DWCi_GetMatchCnt()) return 0;
    return DWCi_GetMatchCnt()->gt2NumConnection;
}


/*---------------------------------------------------------------------------*
  �L���R�l�N�V�������擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�����ɐڑ����Ă���z�X�g���i�������܂܂Ȃ��j�B
          �I�t���C������0��Ԃ��B
  �p�r�@�F�����Ɛڑ����Ă���A�L���ȃz�X�g�i�S�z�X�g�Ƒ��݂Ɍq�����Ă���z�X�g�j
          �̐����擾����B
          ��ɁA�Q�[���̒ʐM���s���Ȃ���A�����Ƀ}�b�`���C�N���i�s����ꍇ�ɁA
          �}�b�`���C�N�����O�ɕ����I�ɐڑ�����Ă���z�X�g������������
          �擾�������ꍇ�Ɏg���B
 *---------------------------------------------------------------------------*/
int DWCi_GetNumValidConnection(void)
{
    
    if (!DWCi_GetMatchCnt()) return 0;
    return DWCi_GetMatchCnt()->gt2NumValidConn;
}


/*---------------------------------------------------------------------------*
  �L���R�l�N�V�������Z�b�g�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�����Ɛڑ����Ă���A�L���ȃz�X�g�i�S�z�X�g�Ƒ��݂Ɍq�����Ă���z�X�g�j
          �̐��𐧌�\���̂̃����o�ɋL�^����B
          validAidBitmap���ύX���ꂽ���͕K���Z�b�g����K�v������B
 *---------------------------------------------------------------------------*/
void DWCi_SetNumValidConnection(void)
{
    int count = -1;
    int i;

    for (i = 0; i < DWC_MAX_CONNECTIONS; i++){
        if (DWCi_GetMatchCnt()->validAidBitmap & (1 << i))
            count++;
    }

    if (count == -1) DWCi_GetMatchCnt()->gt2NumValidConn = 0;
    else DWCi_GetMatchCnt()->gt2NumValidConn = (u8)count;
}


/*---------------------------------------------------------------------------*
  �S�Ă̐ڑ����z�X�g��AID���X�g�擾�֐�
  �����@�FaidList AID���X�g�ւ̃|�C���^
  �߂�l�FAID���X�g���i�������܂ށj
  �p�r�@�FAID���X�g�ւ̃|�C���^���擾����i�R�s�[�͂��Ȃ��j�B
          AID���X�g�́A���ݐڑ�����AID���O���珇�ɋl�߂�ꂽ���X�g�ŁA
          ���̗v�f���͎������܂߂��ڑ����z�X�g���ɂȂ�B
          �}�b�`���C�N�i�s�������݂̐ڑ�AID�Ɛڑ��������̂܂ܕԂ��B
 *---------------------------------------------------------------------------*/
int DWCi_GetAllAIDList(u8** aidList)
{

    if (!DWCi_GetMatchCnt()) return 0;

    *aidList = DWCi_GetMatchCnt()->aidList;
    return DWCi_GetMatchCnt()->gt2NumConnection+1;
}


/*---------------------------------------------------------------------------*
  �L���Ȑڑ����z�X�g��AID���X�g�擾�֐�
  �����@�FaidList AID���X�g�ւ̃|�C���^
  �߂�l�FAID���X�g���i�������܂ށj
  �p�r�@�FAID���X�g�ւ̃|�C���^���擾����i�R�s�[�͂��Ȃ��j�B
          AID���X�g�́A���ݗL����AID���O���珇�ɋl�߂�ꂽ���X�g�ŁA
          ���̗v�f���͎������܂߂��ڑ����z�X�g���ɂȂ�B
          ��ɁA�Q�[���̒ʐM���s���Ȃ���A�����Ƀ}�b�`���C�N���i�s����ꍇ�ɁA
          �}�b�`���C�N�����O�ɕ����I�ɐڑ�����Ă���z�X�g��������AID�̃��X�g��
          �擾�������ꍇ�Ɏg���B
 *---------------------------------------------------------------------------*/
int DWCi_GetValidAIDList(u8** aidList)
{
    static u8 validAidList[DWC_MAX_CONNECTIONS];
    int i;

    if (!DWCi_GetMatchCnt()) return 0;

    // �Q�Ɨp�L��AID���X�g�N���A
    MI_CpuClear8(validAidList, sizeof(validAidList));
    
    for (i = 0; i <= DWCi_GetMatchCnt()->gt2NumValidConn; i++){
        if (DWCi_GetMatchCnt()->validAidBitmap & (1 << DWCi_GetMatchCnt()->aidList[i])){
            // �L����AID�Ȃ烊�X�g�ɃZ�b�g
            validAidList[i] = DWCi_GetMatchCnt()->aidList[i];
        }
        else break;
        // �O�l�߂Ȃ̂ŁA������AID��1����΂���ȍ~�ɗL����AID�͂Ȃ�
    }

    *aidList = validAidList;

    return DWCi_GetMatchCnt()->gt2NumValidConn+1;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�E�T�[�oGPstatus�Z�b�g�֐�
  �����@�F�Ȃ�
  �߂�l�FGP�������ʌ^
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N���̃T�[�o�pgp Status���Z�b�g����
 *---------------------------------------------------------------------------*/
GPResult DWCi_GPSetServerStatus(void)
{
    char valueStr[12], keyValueStr[32];

    if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_SV) return GP_NO_ERROR;

    // ��]�ڑ��l���ƌ��݂̐ڑ��l����GP�X�e�[�^�X������Ƃ��č쐬����
    (void)OS_SNPrintf(valueStr, sizeof(valueStr), "%u", DWCi_GetMatchCnt()->qr2NumEntry+1);
    (void)DWC_SetCommonKeyValueString(DWC_GP_SSTR_KEY_MATCH_SC_MAX,
                                      valueStr, keyValueStr, '/');
    (void)OS_SNPrintf(valueStr, sizeof(valueStr), "%u", DWCi_GetMatchCnt()->gt2NumConnection+1);
    (void)DWC_AddCommonKeyValueString(DWC_GP_SSTR_KEY_MATCH_SC_NUM,
                                      valueStr, keyValueStr, '/');

    // �}�b�`���C�N�o�[�W�������Z�b�g����B
    // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�͖����ʂɃ}�b�`���C�N���Ȃ��̂ŁA
    // ���̃X�e�[�^�X�͎Q�Ƃ��Ȃ����A�m�F�p�ɃZ�b�g�����͂��Ă����B
    (void)OS_SNPrintf(valueStr, sizeof(valueStr), "%u", DWC_MATCHING_VERSION);
    (void)DWC_AddCommonKeyValueString(DWC_GP_SSTR_KEY_MATCH_VERSION,
                                      valueStr, keyValueStr, '/');

    // GP�X�e�[�^�X���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o��Ԃɂ��A
    // �쐬����GP�X�e�[�^�X��������Z�b�g����
    return DWCi_SetGPStatus(DWC_STATUS_MATCH_SC_SV, keyValueStr, NULL);
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N����I�u�W�F�N�g����֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N����I�u�W�F�N�g�ւ̃|�C���^��NULL�N���A����
 *---------------------------------------------------------------------------*/
void DWCi_ShutdownMatch(void)
{

  DWCi_SetMatchCnt(NULL); //stpMatchCnt = NULL;

    // �t�B���^����������
    if (stpAddFilter){
        DWC_Free(DWC_ALLOCTYPE_BASE, stpAddFilter, 0);
        stpAddFilter = NULL;
    }

    // �Q�[����`��QR2�p�L�[�f�[�^�z���������
    DWCi_ClearGameMatchKeys();

    // �}�b�`���C�N�I�v�V��������\���̂����
    if (stpOptMinComp != NULL){
        DWC_Free(DWC_ALLOCTYPE_BASE, stpOptMinComp, 0);
        stpOptMinComp = NULL;
    }

    stOptSCBlock.valid = 0;
    stOptSCBlock.lock  = 0;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�V���b�g�_�E���`�F�b�N�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�}�b�`���C�N���V���b�g�_�E������Ă���AFALSE:����
  �p�r�@�FDWCi_ShutdownMatch()���Ă΂�āA�}�b�`���C�N���V���b�g�_�E�����ꂽ
          ��Ԃ��ǂ����𒲂ׂ�
 *---------------------------------------------------------------------------*/
BOOL DWCi_IsShutdownMatch(void)
{

    if (!DWCi_GetMatchCnt()) return TRUE;
    else return FALSE;
}


//----------------------------------------------------------------------------
// function - static
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �}�b�`���C�N���Ɛ���\���̏������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N���Ƃɏ��������K�v�Ȑ���\���̃����o������������
 *---------------------------------------------------------------------------*/
static void DWCi_ResetMatchParam(DWCMatchResetLevel level)
{

    DWCi_GetMatchCnt()->gt2ConnectCount      = 0;
    DWCi_GetMatchCnt()->nnRecvCount          = 0;
    DWCi_GetMatchCnt()->nnCookieRand         = (u16)DWCi_GetMathRand32(0x10000);
    DWCi_GetMatchCnt()->nnLastCookie         = 0;
    DWCi_GetMatchCnt()->nnFailedTime         = 0;
    DWCi_GetMatchCnt()->nnFinishTime         = 0;
    DWCi_GetMatchCnt()->clLinkProgress       = 0;
    DWCi_GetMatchCnt()->cancelState          = DWC_MATCH_CANCEL_STATE_INIT;
    DWCi_GetMatchCnt()->scResvRetryCount     = 0;
    DWCi_GetMatchCnt()->synResendCount       = 0;
    DWCi_GetMatchCnt()->cancelSynResendCount = 0;
    DWCi_GetMatchCnt()->resvWaitCount        = 0;
    DWCi_GetMatchCnt()->closeState           = DWC_MATCH_CLOSE_STATE_INIT;
    DWCi_GetMatchCnt()->cancelBaseLatency    = 0;
    DWCi_GetMatchCnt()->searchPort           = 0;
    DWCi_GetMatchCnt()->searchIP             = 0;
    DWCi_GetMatchCnt()->lastSynSent          = 0;
    DWCi_GetMatchCnt()->closedTime           = 0;
    MI_CpuClear32(&DWCi_GetMatchCnt()->cmdCnt, sizeof(DWCi_GetMatchCnt()->cmdCnt));

    if (level == DWC_MATCH_RESET_CONTINUE){
        // ���������}�b�`���C�N���s���ꍇ��NN�����I�������z�X�g�̃J�E���g��
        // �N���A����
        DWCi_GetMatchCnt()->qr2NNFinishCount = (u8)DWCi_GetMatchCnt()->gt2NumConnection;

        // �T�[�o�N���C�A���g�}�b�`���C�N�̎���state���Z�b�g����
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAITING);
        }
        else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
            DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_WAITING);
        }
    }
    else {
        DWCi_GetMatchCnt()->gt2NumConnection = 0;
        DWCi_GetMatchCnt()->gt2NumValidConn  = 0;
        
        DWCi_GetMatchCnt()->qr2NNFinishCount = 0;
        DWCi_GetMatchCnt()->qr2IsReserved    = 0;
        DWCi_GetMatchCnt()->qr2Reservation   = 0;
        
//        DWCi_GetMatchCnt()->sbUpdateCount    = 0;
        DWCi_GetMatchCnt()->sbUpdateFlag     = FALSE;

        DWCi_GetMatchCnt()->friendCount      = 0;
        DWCi_GetMatchCnt()->baseLatency      = 0;
//        DWCi_GetMatchCnt()->cmdResendCount   = 0;
        DWCi_GetMatchCnt()->cmdResendFlag    = FALSE;
        DWCi_GetMatchCnt()->cmdResendTick    = 0;
//        DWCi_GetMatchCnt()->cmdTimeoutCount  = 0;
        DWCi_GetMatchCnt()->cmdTimeoutTime = 0;
        DWCi_GetMatchCnt()->cmdTimeoutStartTick = 0;
        DWCi_GetMatchCnt()->synAckBit        = 0;
        DWCi_GetMatchCnt()->reqProfileID     = 0;
        DWCi_GetMatchCnt()->priorProfileID   = 0;
        DWCi_GetMatchCnt()->validAidBitmap   = 0;

        MI_CpuClear32(DWCi_GetMatchCnt()->qr2IPList, sizeof(DWCi_GetMatchCnt()->qr2IPList));
        MI_CpuClear16(DWCi_GetMatchCnt()->qr2PortList, sizeof(DWCi_GetMatchCnt()->qr2PortList));
        MI_CpuClear32(DWCi_GetMatchCnt()->sbPidList, sizeof(DWCi_GetMatchCnt()->sbPidList));
        MI_CpuClear32(&DWCi_GetMatchCnt()->nnInfo, sizeof(DWCNNInfo));
        MI_CpuClear32(DWCi_GetMatchCnt()->ipList, sizeof(DWCi_GetMatchCnt()->ipList));
        MI_CpuClear16(DWCi_GetMatchCnt()->portList, sizeof(DWCi_GetMatchCnt()->portList));
        MI_CpuClear8(DWCi_GetMatchCnt()->aidList, sizeof(DWCi_GetMatchCnt()->aidList));
        MI_CpuClear32(DWCi_GetMatchCnt()->svDataBak, sizeof(DWCi_GetMatchCnt()->svDataBak));

        if (level == DWC_MATCH_RESET_RESTART){
            // �ŏ�����}�b�`���C�N����蒼���ꍇ��state��������Ԃɖ߂�
            if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);
            }
            else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);
            }
            // �T�[�o�N���C�A���g�}�b�`���C�N�͂���level�ɂ͂Ȃ�Ȃ�
        }
        else {
            // �ȉ��̃p�����[�^�� DWC_MATCH_RESET_ALL �̎������N���A���Ȃ�
            DWCi_GetMatchCnt()->qr2MatchType       = 0;
            DWCi_GetMatchCnt()->qr2NumEntry        = 0;
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
            DWCi_GetMatchCnt()->qr2ShutdownFlag    = 0;
#endif
            DWCi_GetMatchCnt()->cbEventPid         = 0;
            DWCi_GetMatchCnt()->distantFriend      = 0;
#ifdef DWC_STOP_SC_SERVER
            DWCi_GetMatchCnt()->stopSCFlag         = 0;
#endif
            DWCi_GetMatchCnt()->clWaitTimeoutCount = 0;
            DWCi_GetMatchCnt()->clWaitTime         = 0;
            DWCi_GetMatchCnt()->newClientCallback  = NULL;
            DWCi_GetMatchCnt()->newClientParam     = NULL;
        }
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N���ʃp�����[�^�������֐�
  �����@�FmatchType �}�b�`���C�N�^�C�v�BDWC_MATCH_TYPE_*�񋓎q
          numEntry  �ڑ��v���l���i�����͊܂܂Ȃ��j
          callback  �}�b�`���C�N�����R�[���o�b�N
          param     �}�b�`���C�N�����R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�J�n���̋��ʃp�����[�^�ݒ���s��
 *---------------------------------------------------------------------------*/
static void DWCi_SetMatchCommonParam(u8  matchType,
                                     u8  numEntry,
                                     DWCMatchedSCCallback callback,
                                     void* param)
{

    // �}�b�`���C�N���Ƃɏ��������K�v�ȃp�����[�^��������
    DWCi_ResetMatchParam(DWC_MATCH_RESET_ALL);

    DWCi_GetMatchCnt()->qr2MatchType    = matchType;
    DWCi_GetMatchCnt()->qr2NumEntry     = numEntry;
    DWCi_GetMatchCnt()->matchedCallback = callback;
    DWCi_GetMatchCnt()->matchedParam    = param;
    DWCi_GetMatchCnt()->nnFailureCount  = 0;

    // �T�[�o��AID�͕K��0
    DWCi_GetMatchCnt()->aidList[0] = 0;

    // �J�X�^���L�[��ݒ肷��
    qr2_register_key(DWC_QR2_PID_KEY, DWC_QR2_PID_KEY_STR);
    qr2_register_key(DWC_QR2_MATCH_TYPE_KEY, DWC_QR2_MATCH_TYPE_KEY_STR);
    qr2_register_key(DWC_QR2_MATCH_RESV_KEY, DWC_QR2_MATCH_RESV_KEY_STR);
    qr2_register_key(DWC_QR2_MATCH_VER_KEY, DWC_QR2_MATCH_VER_KEY_STR);
    qr2_register_key(DWC_QR2_MATCH_EVAL_KEY, DWC_QR2_MATCH_EVAL_KEY_STR);
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N����I�u�W�F�N�g�����������AGameSpy SDK���m�ۂ��Ă���
          �I�u�W�F�N�g���J������
 *---------------------------------------------------------------------------*/
static void DWCi_CloseMatching(void)
{
    
    DWC_Printf( DWC_REPORTFLAG_MATCH_NN, " Close Matching....\n" );

    if (!DWCi_GetMatchCnt()) return;

#ifndef DWC_BUG_WIFI_DELAY_SEND
    // SB�I�u�W�F�N�g�J��
    if (DWCi_GetMatchCnt()->sbObj)
    {
        if(s_sbCallbackLevel == 0) 
        {
            ServerBrowserFree(DWCi_GetMatchCnt()->sbObj);
            DWCi_GetMatchCnt()->sbObj = NULL;
        }
        else
        {
            s_needSbFree = TRUE;
        }
    }
#endif
    // NN�g�p�̈���J������
    DWCi_NNFreeNegotiateList();

    // ��Ԃ�����������
    DWCi_SetMatchStatus(DWC_MATCH_STATE_INIT);

    // SB�T�[�o�A�b�v�f�[�g�p�t�B���^���Z�b�g���Ă�����������
    if (stpAddFilter){
        DWC_Free(DWC_ALLOCTYPE_BASE, stpAddFilter, 0);
        stpAddFilter = NULL;
    }

    // �Q�[����`��QR2�p�L�[�f�[�^�z���������
    DWCi_ClearGameMatchKeys();

#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    // QR2���V���b�g�_�E�����邽�߂̃t���O�𗧂Ă�B
    // QR2�R�[���o�b�N�𔲂��Ă��܂��܂ł̓V���b�g�_�E�����Ȃ��B
    // �T�[�o�N���C�A���g�}�b�`���C�N�ł͂����ɗ��Ȃ��̂ŃV���b�g�_�E������Ȃ�
    DWCi_GetMatchCnt()->qr2ShutdownFlag = 1;
#endif
}


#define DWC_NUM_QR2_MATCH_KEY 7  // ���C�u�������g�p����QR2�L�[�̐�
/*---------------------------------------------------------------------------*
  SB�T�[�o�A�b�v�f�[�g�֐�
  �����@�FprofileID �v���t�@�C��ID���w�肵�Č�������ꍇ�̃v���t�@�C��ID�B
                    �w�肵�Ȃ��Ȃ�0��n���B
  �߂�l�FSB�G���[���
  �p�r�@�FServerBrowserLimitUpdate�֐����Astate�ɉ����������ŌĂяo��
 *---------------------------------------------------------------------------*/
static SBError DWCi_SBUpdateAsync(int profileID)
{
    char filter[MAX_FILTER_LEN];
    char* pFilter = filter;
    u8 basicFields[DWC_NUM_QR2_MATCH_KEY+DWC_QR2_GAME_RESERVED_KEYS];
    int len;
    int numBasicFields = DWC_NUM_QR2_MATCH_KEY;
    int i;
    SBError sbError = sbe_noerror;

    // ���C�u�������g�p����QR2�L�[���Z�b�g
    basicFields[0] = NUMPLAYERS_KEY;
    basicFields[1] = MAXPLAYERS_KEY;
    basicFields[2] = DWC_QR2_PID_KEY;
    basicFields[3] = DWC_QR2_MATCH_TYPE_KEY;
    basicFields[4] = DWC_QR2_MATCH_RESV_KEY;
    basicFields[5] = DWC_QR2_MATCH_VER_KEY;
    basicFields[6] = DWC_QR2_MATCH_EVAL_KEY;

    // �Q�[����`�̃L�[��ǉ�
    if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
        (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
        for (i = 0; i < DWC_QR2_GAME_RESERVED_KEYS; i++){
            if (stGameMatchKeys[i].keyID){
                basicFields[numBasicFields] = stGameMatchKeys[i].keyID;
                numBasicFields++;
            }
        }
    }

    switch (DWCi_GetMatchCnt()->state){
    case DWC_MATCH_STATE_CL_SEARCH_HOST:
        if (!DWCi_GetMatchCnt()->priorProfileID){
            len = DWCi_GetDefaultMatchFilter(filter, DWCi_GetMatchCnt()->profileID,
                                             DWCi_GetMatchCnt()->qr2NumEntry,
                                             DWCi_GetMatchCnt()->qr2MatchType);

            // �Q�[���Œǉ��������t�B���^���ݒ肳��Ă���Βǉ�����
            if (stpAddFilter){
                (void)OS_SNPrintf(&filter[len], sizeof(filter) - len,
                                  " and (%s)",
                                  stpAddFilter);
            }
            break;
        }
        else {
            // �D��\��v���t�@�C��ID���ݒ肳��Ă���ꍇ�́A
            // ���̃v���t�@�C��ID�Ō������s��
            profileID = DWCi_GetMatchCnt()->priorProfileID;
        }

    case DWC_MATCH_STATE_CL_SEARCH_OWN:
    case DWC_MATCH_STATE_CL_WAIT_RESV:
    case DWC_MATCH_STATE_CL_SEARCH_NN_HOST:
        // �v���t�@�C��ID�݂̂Ō�������
        (void)OS_SNPrintf(filter, sizeof(filter),
                          "%s = %u",
                          DWC_QR2_PID_KEY_STR, profileID);

        // �F�B���w��s�A�}�b�`���C�N�A�T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ��
        // reqProfileID ���g��Ȃ��̂ŁA�đ��p�ɂ����ŃZ�b�g����K�v������
        DWCi_GetMatchCnt()->reqProfileID = profileID;
        break;

    default:
        // [test]
        // �L�蓾�Ȃ�
        DWC_Printf(DWC_REPORTFLAG_ERROR,
                   "---DWCi_SBUpdateAsync() illegal state %d.\n",
                   DWCi_GetMatchCnt()->state);
    }

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "ServerBrowserFilter : %s\n", filter);

#if 1
    // ��x�}�X�^�[�T�[�o��disconnect���Ȃ���SBUpdate�ł��Ȃ��̂�
    // �T�[�o���X�g�N���A
    ServerBrowserClear(DWCi_GetMatchCnt()->sbObj);
#else
    if (ServerBrowserState(DWCi_GetMatchCnt()->sbObj) != sb_disconnected){
        // ��x�}�X�^�[�T�[�o��disconnect���Ȃ���SBUpdate�ł��Ȃ��̂�
        // �T�[�o���X�g�N���A
        ServerBrowserClear(DWCi_GetMatchCnt()->sbObj);
    }
#endif

    // �ő�ڑ���]�z�X�g���܂ł̃T�[�o���X�g�A�b�v�f�[�g���s���B
    for (i = 0; i < DWC_DNS_ERROR_RETRY_MAX; i++){
        sbError = ServerBrowserLimitUpdate(DWCi_GetMatchCnt()->sbObj, SBTrue, SBFalse,
                                           basicFields, numBasicFields,
                                           filter, DWC_SB_UPDATE_MAX_SERVERS);
        if (!sbError) break;
        else if (sbError != sbe_dnserror) break;
        // DNS�G���[�Ȃ���񐔂̓��g���C����
    }

    // ServerBrowserLimitUpdate�Ăяo�����Ԃ��L�^����
    if (!sbError){
        DWCi_GetMatchCnt()->sbUpdateRequestTick = OS_GetTick()+OS_MilliSecondsToTicks(DWC_SB_UPDATE_TIMEOUT);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout set.(%s)\n", __FUNCTION__);
    }

    return sbError;
}


/*---------------------------------------------------------------------------*
  �F�B���w��s�A�}�b�`���C�N��SB�T�[�o�A�b�v�f�[�g�p�f�t�H���g�t�B���^������擾�֐�
  �����@�Ffilter    ������Z�b�g��|�C���^
          profileID �����̃v���t�@�C��ID
          numEntry  �ڑ��v���l���i�����͊܂܂Ȃ��j
          matchType �}�b�`���C�N�^�C�v
  �߂�l�F������
  �p�r�@�F�F�B���w��s�A�}�b�`���C�N��SB�T�[�o�A�b�v�f�[�g�p�f�t�H���g�t�B���^
          ��������쐬�E�擾����
 *---------------------------------------------------------------------------*/
static int DWCi_GetDefaultMatchFilter(char* filter, int profileID, u8 numEntry, u8 matchType)
{

    // �}�b�`���C�N�����������Œ�������QR2�z�X�g����������B
    // �Â������̃z�X�g�f�[�^�ɔ������Ȃ��悤�� pid ���m�F����B
    return OS_SNPrintf(filter, MAX_FILTER_LEN,
                       "%s = %d and %s != %u and maxplayers = %d and numplayers < %d and %s = %d and %s != %s",
                       DWC_QR2_MATCH_VER_KEY_STR,
                       DWC_MATCHING_VERSION,
                       DWC_QR2_PID_KEY_STR,
                       profileID,
                       numEntry,
                       numEntry,
                       DWC_QR2_MATCH_TYPE_KEY_STR,
                       matchType,
                       DWC_QR2_MATCH_RESV_KEY_STR,
                       DWC_QR2_PID_KEY_STR);
}


/*---------------------------------------------------------------------------*
  NAT�l�S�V�G�[�V�����J�n�֐�
  �����@�FisQR2    1:QR2���A0�FSB��
          cookie   QR2���̏ꍇ�ɕK�v�ȃN�b�L�[�l�BSB���͊֐����Ő�������
          server   SBServer�I�u�W�F�N�g�B�X�^�[�^NAT�l�S�V�G�[�V��������SB���ŕK�v
  �߂�l�FNN�G���[���
  �p�r�@�FNAT�l�S�V�G�[�V�������Astate�ɉ����������ŊJ�n����
 *---------------------------------------------------------------------------*/
static NegotiateError DWCi_NNStartupAsync(int isQR2, int cookie, SBServer server)
{
    u32 ip;
    u32 senddata[2];
    int index = DWCi_GetMatchCnt()->qr2NNFinishCount;
    int result;
    BOOL nnValid;
    NegotiateError nnError = ne_noerror;

    if (!isQR2){
        // NN�J�n���̏ꍇ
        // �N�b�L�[�l����
        cookie = (DWCi_GetMatchCnt()->profileID & 0x0ffff) | (DWCi_GetMatchCnt()->nnCookieRand << 16);

        if (SBServerHasPrivateAddress(server)){
            // �T�[�o��NAT���ɂ���ꍇ
            if (SBServerGetPublicInetAddress(server) ==
                ServerBrowserGetMyPublicIPAddr(DWCi_GetMatchCnt()->sbObj)){
                // �T�[�o������NAT���ɂ���ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Server[%d] is behind same NAT as me.\n", index);

                // ���ۂɎg�p����IP�A�|�[�g�̓v���C�x�[�g�l�ɂȂ�
                DWCi_GetMatchCnt()->ipList[index]   = SBServerGetPrivateInetAddress(server);
                DWCi_GetMatchCnt()->portList[index] = SBServerGetPrivateQueryPort(server);

                nnValid = FALSE;  // NAT�l�S�V�G�[�V�����͕K�v�Ȃ�
            }
            else {
                // �T�[�o���ʂ�NAT���ɂ���ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Server[%d] is behind NAT.\n", index);
                
                // ���ۂɎg�p����IP�A�|�[�g��NAT�l�S�V�G�[�V�����Ō��܂�
                nnValid = TRUE;  // NAT�l�S�V�G�[�V�������K�v
            }
        }
        else {
            // �T�[�o��NAT�O�ɂ���ꍇ
            ip = (u16)SO_GetHostID();
            if (((ip & 0xffff) == 0xa8c0) ||
                (((ip & 0xff) == 0xac) && ((ip & 0xff00) >= 0x1000) && ((ip & 0xff00) <= 0x1f00)) ||
                ((ip & 0xff) == 0x0a)){
                // ���肪NAT�O�ł�������NAT���Ȃ�NAT�l�S�V�G�[�V�������K�v
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Server[%d] is not behind NAT. But I'm behind NAT.\n",
                           index);

                // ���ۂɎg�p����IP�A�|�[�g��NAT�l�S�V�G�[�V�����Ō��܂�
                nnValid = TRUE;  // NAT�l�S�V�G�[�V�������K�v
            }
            else {
                // ���݂�NAT�O�̏ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Both I and Server[%d] are not behind NAT.\n",
                           index);

                // ���ۂɎg�p����IP�A�|�[�g�̓p�u���b�N�l�ɂȂ�

                DWCi_GetMatchCnt()->ipList[index]   = SBServerGetPublicInetAddress(server);
                DWCi_GetMatchCnt()->portList[index] = SBServerGetPublicQueryPort(server);
                    
                nnValid = FALSE;  // NAT�l�S�V�G�[�V�����͕K�v�Ȃ�
            }
        }

        if (nnValid){
            // ���ۂ�NAT�l�S�V�G�[�V�����v�����s�����̂ݐ���
            DWCi_GetMatchCnt()->nnCookieRand  = (u16)DWCi_GetMathRand32(0x10000);

            // �N�b�L�[�͎��ۂ�NAT�l�S�V�G�[�V�������s���ꍇ�̂݃Z�b�g
            DWCi_GetMatchCnt()->nnInfo.cookie = cookie;
        }
        else {
            // NAT�l�S�V�G�[�V�����s�v�̏ꍇ�́A�����IP�ƃ|�[�g�ԍ���������
            // NAT�l�S�V�G�[�V����������Ԃɂ��Ă��炤
            senddata[0] = (u32)SO_GetHostID();
            senddata[1] = gt2GetLocalPort(*DWCi_GetMatchCnt()->pGt2Socket);

            // �R�}���h���M
            result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_TELL_ADDR,
                                           DWCi_GetMatchCnt()->sbPidList[index],
                                           SBServerGetPublicInetAddress(server),
                                           SBServerGetPublicQueryPort(server),
                                           senddata, 2);

            DWCi_GetMatchCnt()->cmdCnt.count = 0;  // �R�}���h�đ��J�E���g�J�n
                                               
            // [todo]
            // �����̃G���[�͂ǂ���������H
            if (result) return ne_socketerror;

            // ���ۂ�NAT�l�S�V�G�[�V�������s��Ȃ��ꍇ�̓N�b�L�[���Z�b�g���Ȃ�
            DWCi_GetMatchCnt()->nnInfo.cookie = 0;
        }

        // NN�p�����[�^�͕K���ݒ肷��BNAT�l�S�V�G�[�V�����s�v�̏ꍇ�ł�
        // NN�����R�[���o�b�N���Ń����oisQR2�̂ݎQ�Ƃ���B
        DWCi_GetMatchCnt()->nnInfo.isQR2      = 0;
        DWCi_GetMatchCnt()->nnInfo.retryCount = 0;
        DWCi_GetMatchCnt()->nnInfo.port       = SBServerGetPublicQueryPort(server);
        DWCi_GetMatchCnt()->nnInfo.ip         = SBServerGetPublicInetAddress(server);
    }
    else {
        // QR2���͂�����NN�p�����[�^�ݒ�
        DWCi_GetMatchCnt()->nnInfo.isQR2      = 1;
        DWCi_GetMatchCnt()->nnInfo.retryCount = 0;
        DWCi_GetMatchCnt()->nnInfo.port       = 0;
        DWCi_GetMatchCnt()->nnInfo.ip         = 0;
        DWCi_GetMatchCnt()->nnInfo.cookie     = cookie;

        nnValid = TRUE;  // NAT�l�S�V�G�[�V�������K�v
    }

    
    if (nnValid){
        // NAT�l�S�V�G�[�V�����������s
        // �G���[�����͒��ōs���Ă���
        nnError = DWCi_DoNatNegotiationAsync(&DWCi_GetMatchCnt()->nnInfo);
    }
    else {
        // NAT�l�S�V�G�[�V�����s�v�̏ꍇ�́ANAT�l�S�V�G�[�V��������
        // �R�[���o�b�N���Ăяo���B
        // remoteaddr = userdata = NULL �Ƃ��Ėڈ�ɂ���B
        DWCi_NNCompletedCallback(nr_success,
                                 gt2GetSocketSOCKET(*DWCi_GetMatchCnt()->pGt2Socket),
                                 NULL, &DWCi_GetMatchCnt()->nnInfo);

        // ���ۂ�NAT�l�S�V�G�[�V�������s��Ȃ������ꍇ�́ANN�I�����̎�����
        // �N���A����
        DWCi_GetMatchCnt()->nnFinishTime = 0;
    }

    return nnError;
}


/*---------------------------------------------------------------------------*
  NAT�l�S�V�G�[�V�����J�n�֐�
  �����@�FnnInfo NAT�l�S�V�G�[�V�����p�����[�^�\���̂ւ̃|�C���^
  �߂�l�FNN�G���[���
  �p�r�@�FNAT�l�S�V�G�[�V�����p�����[�^�Ɋ�Â��āA���ۂɃl�S�V�G�[�V�������J�n����
 *---------------------------------------------------------------------------*/
static NegotiateError DWCi_DoNatNegotiationAsync(DWCNNInfo* nnInfo)
{
    SBError sbError;
    NegotiateError nnError;
    int i;

    if (!nnInfo->isQR2){
        // QR2���łȂ���΃N�b�L�[���T�[�o�ɑ��M
        sbError =
            ServerBrowserSendNatNegotiateCookieToServer(
                DWCi_GetMatchCnt()->sbObj,
                gt2AddressToString(nnInfo->ip, 0, NULL),
                nnInfo->port,
                nnInfo->cookie);
        // [todo]
        // �����̃G���[�͂ǂ���������H
        if (DWCi_HandleSBError(sbError)) return ne_socketerror;

        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Send NN cookie = %x.\n",
                   nnInfo->cookie);
    }

    // NAT�l�S�V�G�[�V�����J�n
    for ( i = 0 ; i < DWC_DNS_ERROR_RETRY_MAX ; ++i )
    {
        nnError =
                NNBeginNegotiationWithSocket(gt2GetSocketSOCKET(*DWCi_GetMatchCnt()->pGt2Socket),
                                             nnInfo->cookie, nnInfo->isQR2,
                                             DWCi_NNProgressCallback,
                                             DWCi_NNCompletedCallback,
                                             nnInfo);
        if ( nnError == ne_noerror || nnError != ne_dnserror) break;

        DWC_Printf(DWC_REPORTFLAG_DEBUG, " dns error occurs when NatNegotiation begin... retry\n");
    }

    return nnError;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�R�}���h���M�֐�
  �����@�Fcommand   ���M�������R�}���h�萔
          profileID ���M��z�X�g�̃v���t�@�C��ID
          ip        �F�B���w��s�A�}�b�`���C�N�̏ꍇ�̑��M��z�X�g��QR2�pIP
          port      �F�B���w��s�A�}�b�`���C�N�̏ꍇ�̑��M��z�X�g��QR2�p�|�[�g�ԍ�
          data      �R�}���h�ɒǉ����đ��肽���f�[�^�iu32�^�z��ւ̃|�C���^�j�B
                    �Ȃ����NULL��n��
          len       �z��data�̗v�f���Bdata=NULL�Ȃ牽��n���Ă����������B
  �߂�l�FSBError�^��GPResult�^��int�^�ɕϊ������l
  �p�r�@�F�}�b�`���C�N�R�}���h���ASB���b�Z�[�W��GP�o�f�B���b�Z�[�W����
          �I�����đ��M����
 *---------------------------------------------------------------------------*/
static int  DWCi_SendMatchCommand(u8 command, int profileID, u32 ip, u16 port, const u32 data[], int len)
{
    char message[DWC_MATCH_COMMAND_ADD_MESSAGE_MAX];
    char tmpStr[16];
    int msgLen = 0, tmpLen;
    int result;
    int i;

    if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
        (((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL) ||
          DWCi_GetMatchCnt()->distantFriend) &&
         (command == DWC_MATCH_COMMAND_TELL_ADDR))){
        // SB���b�Z�[�W�R�}���h���M
        // �T�[�o�N���C�A���g�}�b�`���C�N�ł��A�N���C�A���g���m��
        // �o�f�B�łȂ��\��������̂�SB���b�Z�[�W���g���B
        // ���l�ɁA�F�B�̗F�B���i�F�B�w��s�A�}�b�`���C�N�̏ꍇ�̂݁j��
        // �ꍇ����L�R�}���h�Ɍ���SB���b�Z�[�W���g���B
        result = (int)DWCi_SendSBMsgCommand(command, ip, port, data, len);
    }
    else {
        if (data && len){
            // u32�̔z��f�[�^�𕶎���ɕϊ�����
            msgLen = OS_SNPrintf(message, sizeof(message), "%u", data[0]);

            for (i = 1; i < len; i++){
                tmpLen = OS_SNPrintf(tmpStr, sizeof(tmpStr), "/%u", data[i]);

                // ���b�Z�[�W������ǉ�(�o�b�t�@���ɒB������ǉ����~�߂�)
                if ( msgLen + tmpLen + 1 > sizeof(message) )
                {
                    SDK_ASSERTMSG( 0, "Match command is too long." );
                    break;
                }
                MI_CpuCopy8(tmpStr, &message[msgLen], (u32)tmpLen);
                msgLen += tmpLen;
            }
        }
        message[msgLen] = '\0';

        // GP�o�f�B���b�Z�[�W�R�}���h���M
        result =
            (int)DWCi_SendGPBuddyMsgCommand(DWCi_GetMatchCnt()->pGpObj,
                                            command,
                                            profileID,
                                            message);
    }

    // �đ��Ɋւ�鑗�M�R�}���h�̃f�[�^�̂݃X�g�b�N���Ă���
    if ((command == DWC_MATCH_COMMAND_RESV_OK)      ||
        (command == DWC_MATCH_COMMAND_TELL_ADDR)    ||
        (command == DWC_MATCH_COMMAND_LINK_CLS_REQ) ||
        (command == DWC_MATCH_COMMAND_LINK_CLS_SUC)){
        DWCi_GetMatchCnt()->cmdCnt.command   = command;
        DWCi_GetMatchCnt()->cmdCnt.port      = port;
        DWCi_GetMatchCnt()->cmdCnt.ip        = ip;
        DWCi_GetMatchCnt()->cmdCnt.profileID = profileID;
        DWCi_GetMatchCnt()->cmdCnt.len       = len;
        DWCi_GetMatchCnt()->cmdCnt.sendTime  = OS_GetTick();
        if (data && len){
            MI_CpuCopy32(data, DWCi_GetMatchCnt()->cmdCnt.data, (u32)len*4);
        }
    }

    return result;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�NSB���b�Z�[�W�R�}���h���M�֐�
  �����@�Fcommand ���M�������R�}���h�萔
          ip      ���M��z�X�g��QR2�pIP
          port    ���M��z�X�g��QR2�p�|�[�g�ԍ�
          data    �R�}���h�ɒǉ����đ��肽���f�[�^�iu32�^�z��ւ̃|�C���^�j�B
                  �Ȃ����NULL��n��
          len     �z��data�̗v�f���Bdata=NULL�Ȃ牽��n���Ă����������B
  �߂�l�FSBError�^
  �p�r�@�FSB���b�Z�[�W�Ń}�b�`���C�N�̂��߂̐���R�}���h�𑗐M����B
 *---------------------------------------------------------------------------*/
static SBError DWCi_SendSBMsgCommand(u8 command, u32 ip, u16 port, const u32 data[], int len)
{
    int i;
    DWCSBMessage senddata;
    SBError sbError;

    // �������f�[�^����肷�����ꍇ��ASSERT
    if ( len > sizeof(senddata.data) / sizeof(u32) )
    {
        SDK_ASSERTMSG( 0, "Match command is too long." );
        len = sizeof(senddata.data) / sizeof(u32);    // �o�b�t�@�ɓ��钷���Ɋۂ߂�
    }

    // ���M�f�[�^�쐬
    if (data && len){
        MI_CpuCopy32(data, senddata.data, sizeof(u32)*len);
    }
    else {
        // data = NULL �̎���len�ɗ^����ꂽ�l�𖳎�����
        len = 0;
    }

    strcpy(senddata.header.identifier, DWC_SB_COMMAND_STRING);
    senddata.header.version   = DWC_MATCHING_VERSION;
    senddata.header.command   = command;
    senddata.header.size      = (u8)(sizeof(u32)*len);
    senddata.header.qr2Port   = DWCi_GetMatchCnt()->qr2Port;
    senddata.header.qr2IP     = DWCi_GetMatchCnt()->qr2IP;
    senddata.header.profileID = DWCi_GetMatchCnt()->profileID;

#ifdef NITRODWC_DEBUG
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "<SB> SEND-%s -> [%08x:%d] [pid=--------]\n",
               get_dwccommandstr(command), ip, port);
#else
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "<SB> SEND-0x%02x -> [%08x:%d] [pid=--------]\n",
               command, ip, port);
#endif
    // SB���b�Z�[�W���M
    for (i = 0; i < DWC_DNS_ERROR_RETRY_MAX; i++){
        sbError =
            ServerBrowserSendMessageToServer(DWCi_GetMatchCnt()->sbObj,
                                             gt2AddressToString(ip, 0, NULL),
                                             port,
                                             (gsi_char *)&senddata,
                                             (int)(sizeof(DWCSBMessageHeader)+senddata.header.size));

        if (!sbError) break;
        else if (sbError != sbe_dnserror) break;
        // DNS�G���[�Ȃ���񐔂̓��g���C����
    }

    return sbError;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�NGP�o�f�B���b�Z�[�W�R�}���h���M�֐�
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          profileID  ���M��o�f�B�̃v���t�@�C��ID
          command    ���M�������R�}���h�萔
          message    �R�}���h�ɒǉ����đ��肽�����b�Z�[�W�B�K�v�Ȃ����NULL��n��
                     ���l���K��������Ƃ��ēn�����Ɓi0���I�[�Ƃ݂Ȃ����̂Łj
  �߂�l�FGP���ʌ^
  �p�r�@�FGP�o�f�B���b�Z�[�W�Ń}�b�`���C�N�̂��߂̐���R�}���h�𑗐M����B
 *---------------------------------------------------------------------------*/
static GPResult DWCi_SendGPBuddyMsgCommand(GPConnection* connection, u8 command, int profileID, const char* message)
{
    char tmpStr[DWC_MATCH_COMMAND_ADD_MESSAGE_MAX];
    u32  len, len2;
    GPResult result;

    len = (u32)OS_SNPrintf(tmpStr, sizeof(tmpStr),
                           "%s%dv%s",
                           DWC_GP_COMMAND_STRING,
                           DWC_MATCHING_VERSION,
                           DWC_GP_COMMAND_MATCH_STRING);
    tmpStr[len]   = (char)command;
    tmpStr[len+1] = '\0';

    if (message){
        len2 = strlen(message);

        // [test]
        // ��������ǉ��R�}���h���b�Z�[�W������Ă����玩���ւ̌x��
        if ( (len + 1) + len2 + 1 > sizeof(tmpStr) )
        {
            SDK_ASSERTMSG( 0, "Match command is too long." );
            len2 = sizeof(tmpStr) - (len + 1) - 1;    // �o�b�t�@�ɓ��钷���Ɋۂ߂�
        }

        // �ǉ��R�}���h���b�Z�[�W�𑗐M�R�}���h�ɒǉ�
        MI_CpuCopy8(message, &tmpStr[len+1], len2);
        tmpStr[len+1+len2] = '\0';
    }

    // �o�f�B���b�Z�[�W���M
    result = gpSendBuddyMessage(connection, profileID, tmpStr);

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "<GP> SEND-0x%02x -> [--------:-----] [pid=%u]\n",
               command, profileID);

    return result;
}


/*---------------------------------------------------------------------------*
  GP�o�f�B���b�Z�[�W�R�}���h�ǉ����b�Z�[�W�擾�֐�
  �����@�FdstMsg �����o������������R�s�[����z��ւ̃|�C���^
          srcMsg GP�o�f�B���b�Z�[�W�R�}���h�ǉ����b�Z�[�W
          index  '/'�ŋ�؂�ꂽ������̉��ڂ��擾��������
  �߂�l�F�����񒷁B���݂��Ȃ��C���f�b�N�X���w�肵���ꍇ��-1��Ԃ�
  �p�r�@�FGP�o�f�B���b�Z�[�W�R�}���h�̒ǉ����b�Z�[�W����A'/'�ŋ�؂�ꂽ
          �R�}���h�̂����̂����ڂ����w�肵�ĕ�������擾����
 *---------------------------------------------------------------------------*/
static int  DWCi_GetGPBuddyAdditionalMsg(char* dstMsg, const char* srcMsg, int index)
{
    const char* pSrcBegin = srcMsg;
    char* pSrcNext = NULL;
    char* pSrcEnd;
    int len;
    int i;

    pSrcEnd = strchr(pSrcBegin, '\0');  // �I�[�̃|�C���^���擾

    // pSrcBegin�Ɏw��̃C���f�b�N�X�̕�����̐擪�ւ̃|�C���^���Z�b�g����
    for (i = 0; i < index; i++){
        pSrcNext = strchr(pSrcBegin, '/');
        if (!pSrcNext){
            // �s���ȃC���f�b�N�X�̎w��
            return -1;
        }
        pSrcBegin = pSrcNext+1;
    }

    // �Ō�̃C���f�b�N�X�̏ꍇ��������̏I�[�ւ̃|�C���^���Z�b�g����
    //if (!pSrcNext) pSrcNext = pSrcEnd;
    pSrcNext = strchr(pSrcBegin, '/');
    if (!pSrcNext){
        // '/'��������Ȃ������ꍇ
        pSrcNext = pSrcEnd;
    }

    // �ǉ����b�Z�[�W���Ȃ��̂ɌĂяo���ꂽ�ꍇ�͕s���ȃC���f�b�N�X�w��
    if (pSrcBegin == pSrcNext) return -1;

    // �w��̕�������R�s�[
    len = pSrcNext-pSrcBegin;
    MI_CpuCopy8(pSrcBegin, dstMsg, (u32)len);
    dstMsg[len] = '\0';

    return len;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�R�}���h�����֐�
  �����@�Fcommand ��M�����}�b�`���C�N�R�}���h
          srcPid  �R�}���h���M���̃v���t�@�C��ID
          srcIP   SB���b�Z�[�W�Ŏ�M�����ꍇ�̃R�}���h���M��QR2�p�p�u���b�NIP
          srcPort SB���b�Z�[�W�Ŏ�M�����ꍇ�̃R�}���h���M��QR2�p�p�u���b�N�|�[�g�ԍ�
          data    ��M�f�[�^�z��ւ̃|�C���^
          len     �z��data�̗v�f��
  �߂�l�FTRUE:����I���AFALSE:�G���[����
  �p�r�@�F��M�����R�}���h�ƃf�[�^���Q�Ƃ��ď������s��
 *---------------------------------------------------------------------------*/
static BOOL DWCi_ProcessRecvMatchCommand(u8  command,
                                         int srcPid,
                                         u32 srcIP,
                                         u16 srcPort,
                                         const u32 data[],
                                         int len)
{
#pragma unused(len)
    u8  sendCommand;
    u8  aid;
    u16 port = 0;
    u32 ip = 0;
    u32 aidIdx;
    u32 senddata[DWC_MAX_MATCH_IDX_LIST+1];
    int profileID;
    int sendlen = 0;
    int result;
    int i;
    SBError sbError;
    NegotiateError nnError;
    struct sockaddr_in remoteaddr;

    if (!DWCi_GetMatchCnt() || (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_INIT)){
        // �}�b�`���C�N�L�����Z������ȂǂɎ�M�����ꍇ�͉����Ԃ��Ȃ�
        // [todo]
        // ���̏����������ƁA�L�����Z���シ���ɕʂ̃}�b�`���C�N������
        // ��蒼�������Ɏ�M�R�}���h���������Ă��܂��B
        // �����Ȃ��Ă��\�����Ȃ��悤�ɐ݌v���Ȃ��Ƃ����Ȃ��B
        return TRUE;
    }

    if ((DWC_GetState() == DWC_STATE_MATCHING) &&
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAITING) &&
        (srcPid == DWCi_GetMatchCnt()->sbPidList[0])){
        // �ڑ��ς݃N���C�A���g�̏ꍇ�́A�T�[�o����R�}���h����M������
        // �^�C���A�E�g�J�E���^������������
        DWCi_InitClWaitTimeout();
    }

    switch (command){
    case DWC_MATCH_COMMAND_RESERVATION:  // NN�\��v��
    case DWC_MATCH_COMMAND_RESV_PRIOR:   // NN�D��\��v��
        if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_ANYBODY){
            // �F�B�w��A�T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ��
            // �ǉ���M�f�[�^���ɃN���C�A���g��QR2�p�p�u���b�NIP�A�|�[�g�ԍ���
            // �����Ă���
            srcIP   = data[1];
            srcPort = (u16)data[2];
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "friend IP:%x, port:%d\n",
                       srcIP, srcPort);
        }
        
        // �\��R�}���h�̑��M�ҏ������ɁA����Ԃ��R�}���h���擾����
        sendCommand =
            DWCi_CheckResvCommand(srcPid, srcIP, srcPort, data[0],
                                  command == DWC_MATCH_COMMAND_RESV_PRIOR ? TRUE : FALSE);

        if (sendCommand == DWC_MATCH_COMMAND_RESV_OK){
            // �\�񏳔F�̏ꍇ
#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
            // �V�K�ڑ��F�B���R�}���h��M�r�b�g�}�b�v���N���A����
            DWCi_GetMatchCnt()->friendAcceptBit = 0;
#endif
                
            // �\�񏳔F�̍ۂ̋��ʐݒ�ƁA�ڑ��ς݂̃N���C�A���g�ւ�
            // �V�K�ڑ��z�X�g�ʒm�������s��
            result = DWCi_ProcessResvOK(srcPid, srcIP, srcPort);
            if (DWCi_HandleMatchCommandError(result)) return FALSE;

            if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
                // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ��
                // �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N���Ăяo��
                if (DWCi_GetMatchCnt()->newClientCallback){
                    DWCi_GetMatchCnt()->newClientCallback(DWCi_GetFriendListIndex(srcPid),
                                                   DWCi_GetMatchCnt()->newClientParam);
                }
            }

            senddata[0] = DWCi_GetMatchCnt()->qr2NNFinishCount;

            // �ڑ��ς݃N���C�A���g���Ɗe�N���C�A���g�̃v���t�@�C��ID�𑗐M����B
            // �i���݂́A��M���ł͗F�B�w��E���w��s�A�}�b�`���C�N�̏ꍇ����
            // ���̃f�[�^���Q�Ƃ��Ă��Ȃ��j
            for (sendlen = 1; sendlen <= DWCi_GetMatchCnt()->qr2NNFinishCount; sendlen++){
                senddata[sendlen] = (u32)DWCi_GetMatchCnt()->sbPidList[sendlen];
            }

            // �����̍ŐV��QR2�p�p�u���b�NIP�ƃ|�[�g�ԍ����N���C�A���g�ɋ�����
            senddata[sendlen++] = DWCi_GetMatchCnt()->qr2IP;
            senddata[sendlen++] = DWCi_GetMatchCnt()->qr2Port;

            // ��Ԃ�NAT�l�S�V�G�[�V�������ɐi�߂�
            DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_OWN_NN);
        }
        // [arakit] main 051024
        else if (sendCommand == DWC_MATCH_COMMAND_RESV_DENY){
            // RESV_DENY�̏ꍇ�͋��ۗ��R�𑗐M����
            // [note]
            // qr2NumEntry > 0 ���K�v�ł��闝�R�́H
            // ���ꂪ����ƁA�N�Ƃ��ڑ����Ă��Ȃ����A�\���M���O��
            // DWC_StopSCMatching()���Ăяo����qr2NumEntry = 0 �ɂȂ�̂ŁA
            // �N���C�A���g��80410�̃G���[�ɂȂ�
            if ((DWCi_GetMatchCnt()->qr2NumEntry > 0) &&
                (DWCi_GetMatchCnt()->qr2NNFinishCount == DWCi_GetMatchCnt()->qr2NumEntry)){
                // �T�[�o������I�[�o�[�̏ꍇ
                if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
                    // ���̂Ƃ���T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ����
                    // ���ۗ��R�͕K�v�Ȃ�
                    senddata[0] = DWC_MATCH_RESV_DENY_REASON_SV_FULL;
                    sendlen     = 1;
                }
            }
        }
        // [arakit] main 051024

        if (sendCommand != DWC_MATCH_COMMAND_DUMMY){
            // �\��R�}���h���M���z�X�g�ɃR�}���h�𑗐M����
            result =
                DWCi_SendMatchCommand(sendCommand, srcPid, srcIP, srcPort,
                                      senddata, sendlen);
            if (DWCi_HandleMatchCommandError(result)) return FALSE;
        }
        break;

    case DWC_MATCH_COMMAND_RESV_OK:  // NN�\�񊮗�
        // ���ɗ\������F�����̂ɒx��Ă��̃R�}���h�����������ꍇ�͖�������
        if (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAIT_RESV) break;
        
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Succeeded NN reservation.\n");

        if (srcPid != DWCi_GetMatchCnt()->reqProfileID){
            // �\��v���𑗂��Ă��Ȃ��T�[�o����\�񊮗��ʒm���Ԃ��ė��Ă���������
            break;
        }

        DWCi_GetMatchCnt()->priorProfileID = 0;  // �D��\��N���A

        DWCi_GetMatchCnt()->resvWaitCount  = 0;  // WAIT��M�ɂ��đ��J�E���^�N���A

//        DWCi_GetMatchCnt()->cmdTimeoutCount = 0;
        DWCi_GetMatchCnt()->cmdTimeoutTime = 0;

//        DWCi_GetMatchCnt()->cmdResendCount  = 0;
        DWCi_GetMatchCnt()->cmdResendFlag  = FALSE;

        // �T�[�o�̍ŐV��QR2�p�p�u���b�NIP�ƃ|�[�g�ԍ����L�^����
        DWCi_GetMatchCnt()->qr2IPList[0]   = data[data[0]+1];
        DWCi_GetMatchCnt()->qr2PortList[0] = (u16)data[data[0]+2];
        DWCi_GetMatchCnt()->searchIP       = data[data[0]+1];
        DWCi_GetMatchCnt()->searchPort     = (u16)data[data[0]+2];
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Server IP:%x, port:%d\n",
                   DWCi_GetMatchCnt()->searchIP, DWCi_GetMatchCnt()->searchPort);

        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
            // �F�B�w��s�A�}�b�`���C�N�̏ꍇ
            if (DWCi_AreAllBuddies(&data[1], data[0])){
                // �T�[�o���̐ڑ��ς݃N���C�A���g���S�Ď����̗F�B�������ꍇ
                if (DWCi_GetMatchCnt()->gt2NumConnection){
                    // �����ɐڑ��ς݃N���C�A���g����������A�����ɂ�
                    // data��]������ׂɃo�b�N�A�b�v������Ă���
                    DWCi_MakeBackupServerData(srcPid, data);
                }
                // ���Ƃ͂܂�����z�X�g�̏���T��
            }
            else {
                // �T�[�o���ɗF�B�łȂ��N���C�A���g���ڑ�����Ă����ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "But some clients are not friends.\n");
                
                // �\��L�����Z�����s��
                result = DWCi_CancelReservation(srcPid);
                if (DWCi_HandleMatchCommandError(result)) return FALSE;

                // ���̗F�B��NN�\��v���𑗐M����
                result = DWCi_SendResvCommandToFriend(FALSE, FALSE, srcPid);
                if (DWCi_HandleMatchCommandError(result)) return FALSE;
                break;  // �ȉ��̏����͔�΂�
            }
        }

        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
            // �F�B���w��s�A�}�b�`���C�N�̏ꍇ
            if (DWCi_GetMatchCnt()->gt2NumConnection){
                // �����ɐڑ��ς݂̃N���C�A���g����������A�S�ẴR�l�N�V������
                // �N���[�Y���Ċ��S�ȂP�N���C�A���g�ɖ߂�
                DWCi_MakeBackupServerData(srcPid, data);

                result = DWCi_ChangeToClient();
                if (DWCi_HandleMatchCommandError(result)) return FALSE;
            }

            // NAT�l�S�V�G�[�V������Ԃ��Z�b�g
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_NN);
               
            // �T�[�o�ɑ΂���NAT�l�S�V�G�[�V�������J�n����
            nnError = DWCi_NNStartupAsync(0, 0, ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, 0));
            if (DWCi_HandleNNError(nnError)) return FALSE;
        }
        else {
            // �\��z�X�g�̌�����Ԃɐi��
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_NN_HOST);

            // SB�T�[�o�A�b�v�f�[�g�ő���z�X�g�̏���T��
            sbError = DWCi_SBUpdateAsync(srcPid);
            if (DWCi_HandleSBError(sbError)) return FALSE;
        }
        break;
            
    case DWC_MATCH_COMMAND_RESV_DENY:  // NN�\�񋑔�
        // �T�[�o����̉����Ȃ��Ń^�C���A�E�g�ɂȂ�������
        // srcIP, srcPort�̒l��0�ɂȂ�
        // �����̎d�l�͂Ȃ��Ȃ���
            
        // ���ɑ��̃N���C�A���g����̗\������F�����̂ɁA
        // �x��Ă��̃R�}���h�����������ꍇ�͖�������
        if ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAIT_RESV) ||
            (srcPid != DWCi_GetMatchCnt()->reqProfileID))
            break;

        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Reservation was denied by %u.\n", srcPid);

        // [arakit] main 051024
        if (len > 0){
            // ���ۗ��R������ꍇ
            if (data[0] == DWC_MATCH_RESV_DENY_REASON_SV_FULL){
                // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o������I�[�o�[
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Game server is fully occupied.\n");

                DWCi_StopMatching(DWC_ERROR_SERVER_FULL, 0);
                return FALSE;
            }
        }
        // [arakit] main 051024

        // ���̗\����T��
        return DWCi_RetryReserving(DWCi_GetMatchCnt()->reqProfileID);
        break;

    case DWC_MATCH_COMMAND_RESV_WAIT:  // NN�\��҂�
        // ���ɗ\������F�����̂ɒx��Ă��̃R�}���h�����������ꍇ�͖�������
        if (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAIT_RESV) break;
        
        // �T�[�o������Ɍq����N���C�A���g���m��
        // �R�l�N�V�����m�����Ȃ̂ŁA���ꂪ�I���̂�҂�
        if (srcPid == DWCi_GetMatchCnt()->reqProfileID){
            // �^�C���A�E�g�v���J�n���ԍăZ�b�g
            DWCi_GetMatchCnt()->cmdTimeoutStartTick = OS_GetTick();
            
            if ((DWCi_GetMatchCnt()->priorProfileID &&
                 (DWCi_GetMatchCnt()->resvWaitCount < DWC_RESV_COMMAND_RETRY_MAX)) ||
                (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
                // �D��\��悪���܂��Ă���ꍇ�͈�莞�Ԍ��
                // �\��R�}���h���đ�����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�ł��A���̃T�[�o�Ɍq���ɍs��
                // �K�v���Ȃ��̂ōđ�����B
//                DWCi_GetMatchCnt()->cmdResendCount = 1;
                DWCi_GetMatchCnt()->cmdResendFlag = TRUE;
                DWCi_GetMatchCnt()->cmdResendTick = OS_GetTick();
                
                if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_CL){
                    DWCi_GetMatchCnt()->resvWaitCount++;
                }
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�A�w��̃T�[�o�ȊO�ɂ�
                // �\��R�}���h�𑗂�Ȃ��̂ŁAWAIT���Ԃ��Ă���Ԃ�
                // �đ��𑱂��č\��Ȃ�
                break;  // �Ȍ�̏����͔�΂�
            }

            DWCi_GetMatchCnt()->priorProfileID = 0;  // �D��\��̓N���A����
            DWCi_GetMatchCnt()->resvWaitCount  = 0;  // WAIT��M�\�񑗐M�J�E���^�N���A

            // �D��\��悪���܂��Ă���ꍇ�łȂ���΁A�����ɒ��߂�
            // ���̗\����T���@
            if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
                // �󂫃z�X�g�̌�����Ԃɖ߂�
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);
//                DWCi_GetMatchCnt()->sbUpdateCount = 1;  // ��莞�Ԍ�Ɍ���
                DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_SHORT;  // ��莞�Ԍ�Ɍ���
                DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
            }
            else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
                // ���̗F�B��NN�\��v���𑗐M����
                (void)DWCi_SendResvCommandToFriend(TRUE, FALSE, 0);
            }
            // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�͂����܂ŗ��Ȃ�
        }
        // �\�񑗐M��������ȊO����̃R�}���h�A�������͎��������ɗ\�񏳔F�A
        // �\��L�����Z�������ꍇ�̗\��҂��R�}���h�͖�������
        break;

    case DWC_MATCH_COMMAND_RESV_CANCEL:  // NN�\������v��
        if (DWCi_GetMatchCnt()->qr2IsReserved &&
            (srcPid == DWCi_GetMatchCnt()->qr2Reservation)){ 
            // �\�񒆂̑��肩��̗\������v���Ȃ牞����
            if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) &&
                (DWCi_GetMatchCnt()->gt2NumConnection == 1) &&
                (DWCi_GetMatchCnt()->sbPidList[1] == srcPid)){
                // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA
                // �V�K�ڑ��N���C�A���g�����ڃL�����Z�����Ă����ꍇ�ŁA
                // gt2Connect��������������ł���΁A�R�l�N�V������
                // �N���[�Y����
                gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);
            }
                
            // �V�K�ڑ��̎󂯕t���������I�����ă}�b�`���C�N���ĊJ����
            if (!DWCi_CancelPreConnectedServerProcess(srcPid)) return FALSE;
        }
        break;

    case DWC_MATCH_COMMAND_TELL_ADDR:  // NN�J�n���A�h���X�ʒm
        // ���z�X�g��NN�J�n���Ɠ����NAT���ɂ���ANAT�l�S�V�G�[�V������
        // �K�v�Ȃ��ꍇ�ɁANAT�l�S�V�G�[�V�����I����Ԃɂ��邽�߂̃R�}���h
        ip   = data[0];
        port = (u16)data[1];

        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "NN parent is behind same NAT as me. Received IP %x & port %d\n",
                   ip, port);

        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAITING){
            // �ڑ��ς݃N���C�A���g�̏ꍇ�͕֋X��A��Ԃ�ύX����
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_NN);
        }
        else if (((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_NN) &&
                  (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_SV_OWN_NN)) ||
                 (srcPid != DWCi_GetMatchCnt()->qr2Reservation)){
            // �Ȃ��Ȃ��N���C�A���g���\��ɗ��Ȃ��̂ŗ\����L�����Z���������
            // NN�v���������ꍇ�͖�������
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "But already canceled reservation.\n");
            break;
        }

        // �\��ێ��^�C���A�E�g�v���I��
        DWCi_GetMatchCnt()->cmdCnt.command = DWC_MATCH_COMMAND_DUMMY;

        // �����V�K�ڑ��N���C�A���g�ʒm�R�}���h����M�ł��Ȃ��āA
        // �܂�����̃v���t�@�C��ID��m��Ȃ������ꍇ�́A�����ŃZ�b�g����B
        // NAT�l�S�V�G�[�V���������ۂɍs�����ꍇ�́A���̂悤�ȃv���t�@�C��ID��
        // ���J�o�[���s���@��S���Ȃ��B
        if (DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection+1] != srcPid){
            DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection+1] = srcPid;
        }

        // NAT�l�S�V�G�[�V���������R�[���o�b�N�ɓn���A�h���X�f�[�^���쐬
        remoteaddr.sin_addr.s_addr = ip;
        // �|�[�g�ԍ���NN�R�[���o�b�N�֐����ŕϊ������̂ŁA
        // �����ł͋t�ϊ����Ă���
        remoteaddr.sin_port = SO_HtoNs(port);

        // NAT�l�S�V�G�[�V���������R�[���o�b�N���Ăяo��
        DWCi_GetMatchCnt()->nnInfo.isQR2 = 1;
        DWCi_NNCompletedCallback(nr_success,
                                 gt2GetSocketSOCKET(*DWCi_GetMatchCnt()->pGt2Socket),
                                 &remoteaddr, &DWCi_GetMatchCnt()->nnInfo);

        // ���ۂ�NAT�l�S�V�G�[�V�������s��Ȃ������ꍇ�́ANN�I�����̎�����
        // �N���A����
        DWCi_GetMatchCnt()->nnFinishTime = 0;
        break;

    case DWC_MATCH_COMMAND_NEW_PID_AID:  // �V�K�ڑ��N���C�A���g���
        // [arakit] main 051010
        // �K���ȏ�ԂŐ������T�[�o�����M�����ꍇ�ȊO�͉������Ȃ�
        if ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAITING) ||
            (srcPid != DWCi_GetMatchCnt()->sbPidList[0])){
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "Ignore delayed NEW_PID_AID command.\n");
            break;
        }
        // [arakit] main 051010
            
        profileID = (int)data[0];
        aid       = (u8)data[1];

#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND) &&
            !DWCi_GetMatchCnt()->distantFriend){
            // �F�B�w��s�A�}�b�`���C�N�ŗF�B�̗F�B�Ƃ̐ڑ��������Ȃ��ꍇ�́A
            // �V�K�ڑ��N���C�A���g�������̗F�B�C���f�b�N�X���X�g�Ŏw�肳�ꂽ
            // �F�B���ǂ������ׂāA���ʂ��T�[�o�ɑ��M����
            senddata[0] = (u32)DWCi_IsFriendByIdxList(profileID);

            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "profileID %d is acceptable? - %d.\n",
                       profileID, senddata[0]);
            
            result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_FRIEND_ACCEPT,
                                           srcPid, srcIP, srcPort,
                                           senddata, 1);
            if (DWCi_HandleMatchCommandError(result)) return FALSE;
        }
#endif

        // �v���t�@�C��ID��AID�����X�g�ɃZ�b�g����
        DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->qr2NNFinishCount+1] = profileID;
        DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->qr2NNFinishCount+1]   = aid;

        // �߂������ɐV�K�ڑ��N���C�A���g��������QR2�f�[�^����������̂ŁA
        // �A�b�v�f�[�g���s��
        qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);

        if (DWCi_GetMatchCnt()->newClientCallback){
            // �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N���Ăяo��
            DWCi_GetMatchCnt()->newClientCallback(DWCi_GetFriendListIndex(profileID),
                                           DWCi_GetMatchCnt()->newClientParam);
        }

        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "Received new client's profileID = %u & aid = %d.\n",
                   profileID, aid);
        break;

    case DWC_MATCH_COMMAND_LINK_CLS_REQ:  // �N���C�A���g���m�̐ڑ��v��
        // [arakit] main 051010
        // �K���ȏ�ԂŐ������T�[�o�����M�����ꍇ�ȊO�͉������Ȃ�
        if ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAITING) ||
            (srcPid != DWCi_GetMatchCnt()->sbPidList[0])){
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "Ignore delayed LINK_CLS_REQ command.\n");
            break;
        }
        // [arakit] main 051010
        
        profileID = (int)data[0];
        if (!profileID){
            // �S�ẴN���C�A���g�Ƃ̃R�l�N�V�������m�����ꂽ
            aidIdx = data[1];
            aid    = (u8)data[2];

            // ������AID��o�^����
            DWCi_GetMatchCnt()->aidList[aidIdx] = aid;
            // �����̃v���t�@�C��ID���o�^���Ă���
            DWCi_GetMatchCnt()->sbPidList[aidIdx] = DWCi_GetMatchCnt()->profileID;
            
            DWCi_PostProcessConnection(DWC_PP_CONNECTION_CL_FINISH_CONNECT);
        }
        else {
            // �w��̃v���t�@�C��ID�ɑ΂���NAT�l�S�V�G�[�V������v������B
            // ���̏ꍇ�͗\��Ȃ��ŗǂ��B
            // ���ɐڑ����s���N���C�A���g�̃v���t�@�C��ID��AID���i�[����
            aidIdx = data[1];
            aid    = (u8)data[2];

            if ((DWCi_GetMatchCnt()->sbPidList[aidIdx] == profileID) &&
                (aidIdx == DWCi_GetMatchCnt()->gt2NumConnection-1)){
                // ���ɐڑ��������Đڑ������̃R�}���h�𑗐M�����̂ɁA
                // ����Ⴂ�ł܂��ڑ��v���R�}���h����M�����ꍇ�A
                // �ڑ������R�}���h���đ�����
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Resend command %d for delayed command %d.\n",
                           DWC_MATCH_COMMAND_LINK_CLS_SUC,
                           DWC_MATCH_COMMAND_LINK_CLS_REQ);
                
                result =
                    DWCi_SendMatchCommand(DWC_MATCH_COMMAND_LINK_CLS_SUC,
                                          srcPid,
                                          DWCi_GetMatchCnt()->qr2IPList[0],
                                          DWCi_GetMatchCnt()->qr2PortList[0],
                                          (u32 *)&profileID, 1);
                if (DWCi_HandleMatchCommandError(result)) return FALSE;
                break;
            }
            
            DWCi_GetMatchCnt()->sbPidList[aidIdx] = profileID;
            DWCi_GetMatchCnt()->aidList[aidIdx]   = aid;

            // QR2�p�p�u���b�NIP�ƃ|�[�g�ԍ�����M���Ă���̂ŁA
            // ���ɐڑ�����N���C�A���g��QR2�f�[�^�𐳊m�Ɍ������邽�ߋL�^
            DWCi_GetMatchCnt()->qr2IPList[aidIdx]   = data[3];
            DWCi_GetMatchCnt()->qr2PortList[aidIdx] = (u16)data[4];
            DWCi_GetMatchCnt()->searchIP            = data[3];
            DWCi_GetMatchCnt()->searchPort          = (u16)data[4];
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Client IP:%x, port:%d\n",
                       DWCi_GetMatchCnt()->searchIP, DWCi_GetMatchCnt()->searchPort);
                    
            // �T�[�o���擾�̂��߂̏�ԂɈڍs
            // [note]
            // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�A�v���C�x�[�g�A�h���X�ƃ|�[�g��
            // ���X�g�����悤�ɂ���΁A������SBUpdate�ŃT�[�o���������Ȃ��Ă�
            // �����Ȃ�NAT�l�S�V�G�[�V�������n�߂���̂ŁA�ꌏ�ɂ�
            // 0.5�b���x�͒Z�k�ł���]�n���c���Ă���Ǝv����
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_NN_HOST);

            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "Next, try to connect to %u.\n", profileID);

            // NAT�l�S�V�G�[�V�������鑊���SB�T�[�o�A�b�v�f�[�g�Ō�����
            sbError = DWCi_SBUpdateAsync(profileID);
            if (DWCi_HandleSBError(sbError)) return FALSE;

//            DWCi_GetMatchCnt()->cmdTimeoutCount = 0;
            DWCi_GetMatchCnt()->cmdTimeoutTime = 0;
//            DWCi_GetMatchCnt()->cmdResendCount  = 0;
            DWCi_GetMatchCnt()->cmdResendFlag = FALSE;
        }
        break;

    case DWC_MATCH_COMMAND_LINK_CLS_SUC:  // �N���C�A���g���m�̐ڑ�����
        if ((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_WAIT_CL_LINK) &&
            (DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->clLinkProgress+1] == data[0])){
            // data[0]�̓N���C�A���g���ڑ�������������̃v���t�@�C��ID�B
            // ���ɐڑ��v�����o���Ă�������Ƃ̐ڑ������������ꍇ�̂�
            // ���̐ڑ��v���𑗐M����B
            DWCi_GetMatchCnt()->clLinkProgress++;

            // ���̃R�l�N�V�����m�����J�n����
            DWCi_PostProcessConnection(DWC_PP_CONNECTION_SV_CONNECT);
        }
        else {
            // ���Ɋm�������R�l�N�V�����̐����R�}���h������Ⴂ�œ͂����ꍇ��
            // �������Ȃ�
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Ignore delayed command %d.\n",
                       DWC_MATCH_COMMAND_LINK_CLS_SUC);
        }
        break;

    case DWC_MATCH_COMMAND_CLOSE_LINK:  // �R�l�N�V�����N���[�Y����
        // [arakit] main 051010
        // �K���ȏ�ԂŎ�M�����ꍇ�ȊO�͉������Ȃ�
        if ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAITING) &&
            (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_WAIT_CLOSE)){
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "Ignore delayed CLOSE_LINK command.\n");
            break;
        }
        // [arakit] main 051010
            
        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
            DWCi_AreAllBuddies(&data[1], data[0])){
            // �N���[�Y���ߑ��M���̐V�K�ڑ���T�[�o�̐ڑ��ς݃N���C�A���g���A
            // �S�Ď����̗F�B�������ꍇ�A���������ɂ����ɐڑ������݂�B
            // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�͗F�B���ǂ����͊֌W�Ȃ��B
            DWCi_GetMatchCnt()->priorProfileID = (int)data[1];  // ����ڑ���L�^

            // WAIT��M�ɂ��đ��ׂ̈̏���
            DWCi_GetMatchCnt()->resvWaitCount = 0;

            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "Received close command. Next try to %u.\n",
                       data[1]);
        }
        else {
            // �N���[�Y���ߑ��M���̐V�K�ڑ���T�[�o�̐ڑ��ς݃N���C�A���g�ɁA
            // �����̗F�B�łȂ��N���C�A���g�������ꍇ�͎���͂܂��ʂ̐ڑ����T��
            DWCi_GetMatchCnt()->priorProfileID = 0;

            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "Received close command. Server %u or its clients are not friends.\n",
                       data[1]);
        }
        
        if (DWCi_GetMatchCnt()->gt2NumConnection){
            // �܂��R�l�N�V�����̃N���[�Y���������Ă��Ȃ��Ȃ玩����
            // �N���[�Y����B
            // �����̏����̓R�[���o�b�N���ōs����B
            gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);
        }
        else {
            // ���ɃR�l�N�V�����̃N���[�Y���I����Ă����ꍇ�́A
            // �����ɗD��\���z�X�g�ւ̐ڑ������݂�
            if (DWCi_ResumeMatching()) return FALSE;
        }
        break;

    case DWC_MATCH_COMMAND_CANCEL:  // �}�b�`���C�N�L�����Z��
        // ���̃R�}���h����M����̂́A�T�[�o�ւ̗\��������Ƃ��납��
        // gt2Connection�m�����O�܂ł̃N���C�A���g�̂�

        // [arakit] main 051010
        // �K���ȏ�ԂŐ������T�[�o�����M�����ꍇ�ȊO�͉������Ȃ�
        if (srcPid != DWCi_GetMatchCnt()->sbPidList[0]){
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "Ignore delayed CANCEL command.\n");
            break;
        }
        // [arakit] main 051010
            
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "Received cancel command from %u data[0] = %d.\n",
                   srcPid, data[0]);
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "numHost nn=%d gt2=%d, state %d\n",
                   DWCi_GetMatchCnt()->qr2NNFinishCount,
                   DWCi_GetMatchCnt()->gt2NumConnection, DWCi_GetMatchCnt()->state);

        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
            (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
            // �S�ẴN���C�A���g�������I�����ă}�b�`���C�N���ĊJ����B
            if (!DWCi_CancelPreConnectedClientProcess(srcPid)) return FALSE;
        }
        else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
            // �V�K�ڑ��N���C�A���g�ŁA�T�[�o���L�����Z�������ꍇ�A
            // �}�b�`���C�N���I������
            // �L�����Z���z�X�g�̃v���t�@�C��ID���L�^
            DWCi_GetMatchCnt()->cbEventPid = srcPid;

            // �R�l�N�V�������c���Ă�����S�ăN���[�Y����
            DWCi_CloseAllConnectionsByTimeout();
            DWCi_RestartFromCancel(DWC_MATCH_RESET_ALL);
        }
        break;

    case DWC_MATCH_COMMAND_CANCEL_SYN:
    case DWC_MATCH_COMMAND_CANCEL_SYN_ACK:
    case DWC_MATCH_COMMAND_CANCEL_ACK:
        // �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������R�}���h����������
        if (!DWCi_ProcessCancelMatchSynCommand(srcPid, command, data[0]))
            return FALSE;
        break;

    case DWC_MATCH_COMMAND_SC_CLOSE_CL:   // �ڑ��ς݃N���C�A���g�̐ؒf�ʒm
        // ������T�[�o�ȊO���炱�̃R�}���h����M���Ă��������Ȃ�
        if (srcPid != DWCi_GetMatchCnt()->sbPidList[0]) return TRUE;

        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Close shutdown client.\n");

        // �ؒf�Ɣ��肳�ꂽ�N���C�A���g���N���[�Y����
        for (i = 0; i < len; i++){
            aid = DWCi_GetAIDFromProfileID((int)data[i], FALSE);
            if (aid != 0xff){
                DWC_CloseConnectionHard(aid);
            }
        }
        break;

    case DWC_MATCH_COMMAND_POLL_TIMEOUT:  // OPTION_MIN_COMPLETE�̃^�C���A�E�g�|�[�����O
        if (stpOptMinComp && stpOptMinComp->valid &&
            (OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->startTime) >= stpOptMinComp->timeout)){
            // ������OPTION_MIN_COMPLETE�̃I�v�V������ݒ肵�Ă��āA
            // �^�C���A�E�g���Ԃ��z���Ă�����1��Ԃ�
            senddata[0] = 1;

            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                       "[OPT_MIN_COMP] time is %lu.\n",
                       OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->startTime));
        }
        else {
            senddata[0] = 0;
        }

        result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_POLL_TO_ACK,
                                       srcPid, srcIP, srcPort,
                                       senddata, 1);
        if (DWCi_HandleMatchCommandError(result)) return FALSE;
        break;

    case DWC_MATCH_COMMAND_POLL_TO_ACK:  // OPTION_MIN_COMPLETE�̃^�C���A�E�g�|�[�����OACK
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_POLL_TIMEOUT){
            // ���M���N���C�A���g����̎�M�r�b�g�𗧂Ă�iAID�r�b�g�}�b�v�j
            aid = DWCi_GetAIDFromProfileID(srcPid, FALSE);
            if (aid != 0xff){
                stpOptMinComp->recvBit |= 1 << aid;
            
                if (data[0]){
                    // �N���C�A���g���^�C���A�E�g�Ȃ�r�b�g�𗧂Ă�
                    stpOptMinComp->timeoutBit |= 1 << aid;
                }
            }
        }
        // �Ⴄ��Ԃ̎��ɒx��Ă���ACK����M���Ă��������Ȃ�
        break;
 
    case DWC_MATCH_COMMAND_SC_CONN_BLOCK:  // OPTION_SC_CONNECT_BLOCK�̃u���b�N�ɂ��\�񋑔�
        // �}�b�`���C�N���G���[�I������
        // [arakit] main 051011
        DWCi_StopMatching(DWC_ERROR_MO_SC_CONNECT_BLOCK, 0);
        return FALSE;

#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
    case DWC_MATCH_COMMAND_FRIEND_ACCEPT:  // �N���C�A���g����̐V�K�ڑ��F�B����
        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND) &&
            !DWCi_GetMatchCnt()->distantFriend){
            // �F�B�w��s�A�}�b�`���C�N�ŗF�B�̗F�B�Ƃ̐ڑ��s���̏ꍇ�̂�
            // �ȉ��̏����ɐi��
            // [todo]
            // �v���t�@�C��ID���玩���̐ڑ��ς݃N���C�A���g���ǂ������ׂ�֐������
            for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                if (DWCi_GetMatchCnt()->sbPidList[i] == srcPid){
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                               "New client was accepted? - %d.\n", data[0]);
                    if (!data[0]){
                        // �ڑ��ς݃N���C�A���g�ɂƂ��āA�V�K�ڑ��N���C�A���g��
                        // �F�B�C���f�b�N�X���X�g�L�ڂ̗F�B�łȂ������ꍇ��
                        // �}�b�`���C�N����蒼��
                        if ((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_NN) ||
                            (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_GT2)){

                            // �V�K�ڑ��N���C�A���g�Ƃ̐ڑ����Ȃ�A
                            // �}�b�`���C�N�L�����Z���R�}���h�𑗐M
                            result = DWCi_InvalidateReservation();
                            if (DWCi_HandleMatchCommandError(result)) return FALSE;
                        }

                        if (DWCi_GetMatchCnt()->nnInfo.cookie){
                            // NAT�l�S�V�G�[�V�������Ȃ�L�����Z��
                            NNCancel(DWCi_GetMatchCnt()->nnInfo.cookie);
                            DWCi_GetMatchCnt()->nnInfo.cookie = 0;
                        }

                        // �}�b�`���C�N��蒼��
                        DWCi_RestartFromTimeout();
                    }
                    else {
                        // �V�K�ڑ��F�B������M�������Ƃ��L�^����
                        DWCi_GetMatchCnt()->friendAcceptBit |=
                            1 << DWCi_GetAIDFromProfileID(srcPid, FALSE);
                    }
                    break;
                }
            }
        }
        break;
#endif

    case DWC_MATCH_COMMAND_CL_WAIT_POLL:  // �N���C�A���g����̃L�[�v�A���C�u�v��
        for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
            // �����ɐڑ����Ă���N���C�A���g�����M�����̂ł���΁A
            // �L�[�v�A���C�u�̃R�}���h��ԐM����
            if (DWCi_GetMatchCnt()->sbPidList[i] == srcPid){
                result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_SV_KA_TO_CL,
                                               srcPid, srcIP, srcPort,
                                               NULL, 0);
                if (DWCi_HandleMatchCommandError(result)) return FALSE;
                else break;
            }
        }
        break;

    case DWC_MATCH_COMMAND_SV_KA_TO_CL:  // �T�[�o����̃L�[�v�A���C�u
        // �{�֐��̍ŏ��Ń^�C���A�E�g�J�E���^�̏������͍s���Ă���̂ŁA
        // �����ł͓��ɉ�������K�v�͂Ȃ�
        break;

    default:
        // [test]
        // �J�����̂ݕ\��
        DWC_Printf(DWC_REPORTFLAG_ERROR,
                   "Received unexpected matching command 0x%02x.\n",
                   command);
        break;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�\��R�}���h�`�F�b�N�֐�
  �����@�FprofileID �R�}���h���M���z�X�g�̃v���t�@�C��ID
          qr2IP     �R�}���h���M���z�X�g��QR2�p�p�u���b�NIP
          qr2Port   �R�}���h���M���z�X�g��QR2�p�p�u���b�N�|�[�g�ԍ�
          matchType �R�}���h���M���z�X�g�̃}�b�`���C�N�^
          priorFlag �D��\��t���O
  �߂�l�F���ɑ���Ԃ��ׂ��R�}���h�BDWC_MATCH_COMMAND_RESV_* �񋓎q�B
  �p�r�@�FNAT�l�S�V�G�[�V�����\��R�}���h����M�������ɁA���ɑ���Ԃ��ׂ�
          �R�}���h�𔻒肷��
 *---------------------------------------------------------------------------*/
static u8  DWCi_CheckResvCommand(int profileID, u32 qr2IP, u16 qr2Port, u32 matchType, BOOL priorFlag)
{
    u8  sendCommand;
    int result;

    switch (DWCi_GetMatchCnt()->qr2MatchType){
    case DWC_MATCH_TYPE_FRIEND:   // �F�B�w��s�A�}�b�`���C�N
        if (!gpIsBuddy(DWCi_GetMatchCnt()->pGpObj, profileID)){
            // �Е����݂̗̂F�B����̗\��͖�������B
            // �����QR2���オ���Ă��邩�ǂ�����������Ȃ��̂ŁA
            // SB���b�Z�[�W�ŕԂ����Ƃ����Ȃ��B
            sendCommand = DWC_MATCH_COMMAND_DUMMY;  // �_�~�[�R�}���h
            break;
        }
#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
        else if (!DWCi_IsFriendByIdxList(profileID)){
            // �F�B�C���f�b�N�X���X�g�ɂȂ��F�B����̗\��͋��ۂ���
            sendCommand = DWC_MATCH_COMMAND_RESV_DENY;  // �\�񋑔ۃR�}���h
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "This friend doesn't exist in friendIdxList.\n");
            break;
        }
#endif
        // ���������F�B���w��s�A�}�b�`���C�N�Ƌ��ʂ̏����ɐi��
        
    case DWC_MATCH_TYPE_ANYBODY:  // �F�B���w��s�A�}�b�`���C�N
        if ((matchType != DWCi_GetMatchCnt()->qr2MatchType)                    ||
            (DWCi_GetMatchCnt()->cancelState != DWC_MATCH_CANCEL_STATE_INIT)   ||
            (DWCi_GetMatchCnt()->qr2NNFinishCount == DWCi_GetMatchCnt()->qr2NumEntry) ||
            (DWCi_GetMatchCnt()->qr2IsReserved &&
             (DWCi_GetMatchCnt()->qr2Reservation == DWCi_GetMatchCnt()->profileID))){
            // ���������ɐڑ�����Ă���N���C�A���g�̏ꍇ��A
            // ���ɐڑ����������ς��̏ꍇ�͗\������ۂ���B
            sendCommand = DWC_MATCH_COMMAND_RESV_DENY;  // �\�񋑔ۃR�}���h

            if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) &&
                !DWCi_GetMatchCnt()->qr2Obj->userstatechangerequested &&
                DWCi_GetMatchCnt()->qr2IsReserved &&
                (DWCi_GetMatchCnt()->qr2Reservation == DWCi_GetMatchCnt()->profileID)){
                // �F�B���w��s�A�}�b�`���C�N�Ŋ��ɐڑ��ς݃N���C�A���g�Ȃ̂�
                // �\��R�}���h����M�����ꍇ�́A�����T�[�o�ɐڑ��ςݏ��
                // ���f�����悤�ɂ���
                qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
            }
        }
        else if (((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_SEARCH_HOST) &&
                  (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_WAIT_RESV))     ||
                 ((DWCi_GetMatchCnt()->qr2IP == 0) && (DWCi_GetMatchCnt()->qr2Port == 0)) ||
                 ((qr2IP == 0) && (qr2Port == 0))){
            // �����̃}�b�`���C�N�������i�s���Ȃ炻�ꂪ�I���܂ő҂��Ă��炤�B
            // �܂������������QR2�p�p�u���b�NIP�ƃ|�[�g�����܂��Ă��Ȃ��ꍇ�����l
            sendCommand = DWC_MATCH_COMMAND_RESV_WAIT;  // �\��ҋ@�R�}���h
        }
        else {
            if (DWCi_GetMatchCnt()->reqProfileID){
                // �N����NN�v���𑗂��đ҂��Ă�����
                if (DWCi_GetMatchCnt()->reqProfileID == profileID){
                    // ������NN�v���𑗂������肩��NN�v���������ꍇ
                    if (priorFlag ||
                        ((DWCi_GetMatchCnt()->profileID < profileID) &&
                        (profileID != DWCi_GetMatchCnt()->priorProfileID))){
                        // �����̃v���t�@�C��ID�������菬�����A��������
                        // �D��\��t���O��ON�̏ꍇ�͗\�񏳔F�B
                        // �������A��L�𖞂����Ă��Ă��A�������D��\��v����
                        // ���������肩��̗v���͏��F���Ȃ�
                        sendCommand = DWC_MATCH_COMMAND_RESV_OK;  // �\�񏳔F�R�}���h
                    }
                    else {
                        // ���肩��NN�v���ւ̕ԓ�������͂��Ȃ̂�
                        // �R�}���h�𑗐M���Ȃ��ׂɃ_�~�[�R�}���h��ݒ肷��
                        sendCommand = DWC_MATCH_COMMAND_DUMMY;  // �_�~�[�R�}���h
                    }
                }
                else {
                    // ������NN�v���𑗂��Ă��Ȃ����肩��NN�v���������ꍇ
                    if (priorFlag ||
                        ((DWCi_GetMatchCnt()->profileID < profileID) &&
                        !DWCi_GetMatchCnt()->priorProfileID)){
                        // �����̃v���t�@�C��ID�������菬�����A�D��\��v����
                        // ���M�ς݂łȂ����A�������͗D��\��t���O��ON��
                        // �ꍇ�́A������NN�v���͒��ߎ�M����NN�v�������F����B
                        // �\��L�����Z�����s���B
                        result = DWCi_CancelReservation(DWCi_GetMatchCnt()->reqProfileID);
                        if (DWCi_HandleMatchCommandError(result)){
                            return DWC_MATCH_COMMAND_DUMMY;  // �_�~�[�R�}���h
                        }
                        else {
                            sendCommand = DWC_MATCH_COMMAND_RESV_OK;  // �\�񏳔F�R�}���h
                        }
                    }
                    else {
                        sendCommand = DWC_MATCH_COMMAND_RESV_DENY;  // �\�񋑔ۃR�}���h
                    }
                }
            }
            else {
                sendCommand = DWC_MATCH_COMMAND_RESV_OK;    // �\�񏳔F�R�}���h
            }
        }
        break;

    case DWC_MATCH_TYPE_SC_SV:  // �T�[�o�N���C�A���g�}�b�`���C�N�T�[�o��
        if (!gpIsBuddy(DWCi_GetMatchCnt()->pGpObj, profileID)){
            // �Е����݂̗̂F�B����̗\��͖�������B
            // �����QR2���オ���Ă��邩�ǂ�����������Ȃ��̂ŁA
            // SB���b�Z�[�W�ŕԂ����Ƃ����Ȃ��B
            sendCommand = DWC_MATCH_COMMAND_DUMMY;  // �_�~�[�R�}���h
        }
        else if ((matchType != DWC_MATCH_TYPE_SC_CL) ||
                 (DWCi_GetMatchCnt()->qr2NNFinishCount == DWCi_GetMatchCnt()->qr2NumEntry)){
            // ���ɂ����ς��̏ꍇ�͗\������ۂ���B
            // �}�b�`���C�N���L�����Z�����������NN�v������M���Ă��܂���
            // �ꍇ�����ۂ���B
            sendCommand = DWC_MATCH_COMMAND_RESV_DENY;  // �\�񋑔ۃR�}���h
        }
        else if ((stOptSCBlock.valid == 1) && (stOptSCBlock.lock == 1)){
            // OPTION_SC_CONNECT_BLOCK�̃I�v�V�������ݒ肳��Ă��āA
            // �V�K�ڑ����u���b�N����Ă���ꍇ�́A���ʂȋ��ۃR�}���h�𑗂�
            sendCommand = DWC_MATCH_COMMAND_SC_CONN_BLOCK;
        }
        else if ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_SV_WAITING)         ||
                 ((DWCi_GetMatchCnt()->qr2IP == 0) && (DWCi_GetMatchCnt()->qr2Port == 0)) ||
                 ((qr2IP == 0) && (qr2Port == 0))){
            // ���̐l�Ƃ̃R�l�N�V�����m���������i�s���Ȃ�A
            // ���ꂪ�I���܂ő҂��Ă��炤�B
            // �܂������������QR2�p�p�u���b�NIP�ƃ|�[�g�����܂��Ă��Ȃ��ꍇ�����l�B
            sendCommand = DWC_MATCH_COMMAND_RESV_WAIT;  // �\��ҋ@�R�}���h
        }
        else {
            sendCommand = DWC_MATCH_COMMAND_RESV_OK;    // �\�񏳔F�R�}���h
        }
        break;
    }

    return sendCommand;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�\��OK�ԐM�O�����֐�
  �����@�FprofileID �\��R�}���h���M���z�X�g�̃v���t�@�C��ID
          ip        �F�B���w��s�A�}�b�`���C�N�̏ꍇ�̑����QR2�p�p�u���b�NIP
          port      �F�B���w��s�A�}�b�`���C�N�̏ꍇ�̑����QR2�p�p�u���b�N�|�[�g�ԍ�
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l
  �p�r�@�FNAT�l�S�V�G�[�V�����\���OK��Ԃ��O�ɁA�������̐ݒ��
          �ڑ��ς݂̃N���C�A���g�ւ̐V�K�ڑ��N���C�A���g�̒ʒm���s��
 *---------------------------------------------------------------------------*/
static int DWCi_ProcessResvOK(int profileID, u32 ip, u16 port)
{
    u32  senddata[2];
    int  result;
    int  i;

    if (DWCi_GetMatchCnt()->qr2IsReserved && (DWCi_GetMatchCnt()->qr2Reservation == profileID)){

        // ���ɗ\��ł��Ă���̂ɂ����ɗ����ꍇ�͉������Ȃ�
        return 0;
    }

    // �N���C�A���g�ɂ��\��𐬗�������
    DWCi_GetMatchCnt()->qr2IsReserved   = 1;
    DWCi_GetMatchCnt()->qr2Reservation  = profileID;
//    DWCi_GetMatchCnt()->cmdResendCount  = 0;  // �R�}���h�đ��J�E���^�N���A
    DWCi_GetMatchCnt()->cmdResendFlag   = FALSE;  // �R�}���h�đ��J�E���^�N���A
//    DWCi_GetMatchCnt()->cmdTimeoutCount = 0;  // �R�}���h���X�|���X�^�C���A�E�g�J�E���g�N���A
    DWCi_GetMatchCnt()->cmdTimeoutTime = 0;  // �R�}���h���X�|���X�^�C���A�E�g�J�E���g�N���A

    // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
    qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);

    // �F�B�w��s�A�}�b�`���C�N�̏ꍇ�̂��߂ɁANN�v�����M��v���t�@�C��ID���N���A
    DWCi_GetMatchCnt()->reqProfileID = 0;

    // ����̃v���t�@�C��ID�����X�g�Ɋi�[����
    DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->qr2NNFinishCount+1] = profileID;

    // �����QR2�p�p�u���b�NIP�ƃ|�[�g�ԍ����Z�b�g
    DWCi_GetMatchCnt()->qr2IPList[DWCi_GetMatchCnt()->qr2NNFinishCount+1]    = ip;
    DWCi_GetMatchCnt()->qr2PortList[DWCi_GetMatchCnt()->qr2NNFinishCount+1] = port;
    DWCi_GetMatchCnt()->searchIP   = ip;
    DWCi_GetMatchCnt()->searchPort = port;

    // AID���X�g��AID��o�^
    DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->qr2NNFinishCount+1] = DWCi_GetAIDFromList();

    // �ڑ��ς݂̃N���C�A���g�ɐV�K�ڑ��N���C�A���g�̃v���t�@�C��ID��
    // AID�𑗐M����
    senddata[0] = (u32)profileID;
    senddata[1] = DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->qr2NNFinishCount+1];

    for (i = 1; i <= DWCi_GetMatchCnt()->qr2NNFinishCount; i++){
        result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_NEW_PID_AID,
                                       DWCi_GetMatchCnt()->sbPidList[i],
                                       DWCi_GetMatchCnt()->qr2IPList[i],
                                       DWCi_GetMatchCnt()->qr2PortList[i],
                                       senddata, 2);
        if (result) return result;
    }

    // �T�[�o�ɂȂ邱�Ƃ����肵���ꍇ�A�w��l���ȉ��}�b�`���C�N�����I�v�V������
    // �ݒ肳��Ă���΁A���̃p�����[�^������������
    DWCi_InitOptMinCompParam(TRUE);

    return 0;
}


/*---------------------------------------------------------------------------*
  �T�[�o�̐ڑ��ς݃z�X�g���X�g�ޔ��֐�
  �����@�FprofileID �T�[�o�̃v���t�@�C��ID
          data      �T�[�o�����M�����ڑ��ς݃z�X�g���X�g
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�w��s�A�}�b�`���C�N�ŁA�T�[�o����\��OK�̃R�}���h����M�������ɁA
          �T�[�o�̐ڑ��ς݃z�X�g�f�[�^�̃o�b�N�A�b�v������Ă���
 *---------------------------------------------------------------------------*/
static void DWCi_MakeBackupServerData(int profileID, const u32 data[])
{
    u32 len;

    // svDataBak[1]�̓T�[�o�̃v���t�@�C��ID�Ƃ��A���̌���pidList��
    // �R�s�[����BsvDataBak[0]�̓v���t�@�C��ID���X�g��
    len = data[0]+2;
    if (len > 2){
        MI_CpuCopy32(&data[1], &DWCi_GetMatchCnt()->svDataBak[2], sizeof(u32)*(len-2));
    }

    DWCi_GetMatchCnt()->svDataBak[0] = len-1;
    DWCi_GetMatchCnt()->svDataBak[1] = (u32)profileID;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�R�}���h�������G���[�����֐�
  �����@�Fint SBError�^��������GPResult�^
  �߂�l�F�����Ɏ󂯎�������̂�int�^�ɂ��ĕԂ�
  �p�r�@�F�}�b�`���C�N�R�}���h��GP�o�f�B���b�Z�[�W��SB���b�Z�[�W�̂ǂ��炩��
          �g���̂ŁA�����̃G���[�^���n���h�����O���ĂP�̃G���[�Ƃ��ĕԂ�
 *---------------------------------------------------------------------------*/
static int DWCi_HandleMatchCommandError(int error)
{

    // [todo]
    // �F�B���w��s�A�}�b�`���C�N�łȂ��Ă�SB���b�Z�[�W�ŃR�}���h�𑗂邱�Ƃ�
    // ����̂ł��̏ꍇ�������Ɛ��m�ł͂Ȃ�
    if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
        return (int)DWCi_HandleSBError((SBError)error);
    }
    else {
        return (int)DWCi_HandleGPError((GPResult)error);
    }
}


/*---------------------------------------------------------------------------*
  NN�\��R�}���h���M�֐�
  �����@�FprofileID �\�񂵂����T�[�o�̃v���t�@�C��ID�B
                    �F�B�w��A�T�[�o�N���C�A���g�}�b�`���C�N�Ŏg�p�B
          delay     TRUE :���t���[����ɃR�}���h���M����A
                    FALSE:���R�}���h���M����
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l
  �p�r�@�F�w��̃T�[�o�̏����Z�b�g���ANN�\��R�}���h�𑗐M����
 *---------------------------------------------------------------------------*/
static int DWCi_SendResvCommand(int profileID, BOOL delay)
{
    u8  command;
    u32 senddata[3];
    int sendlen;
    int result;
    SBServer server;

    if (delay || ((DWCi_GetMatchCnt()->qr2IP == 0) && (DWCi_GetMatchCnt()->qr2Port == 0))){
        // ���ۂ̑��M��x�点�����ꍇ�͍đ��J�E���^���Z�b�g���A
        // �v���t�@�C��ID���X�g[0]�Ɏ��̑��M��v���t�@�C��ID���Z�b�g���Ă�����
        // DWCi_MatchProcess()���ōđ��Ƃ����`��NN�\��v�������M�����B
        // QR2�p�p�u���b�NIP�A�|�[�g�ԍ����m�肵�Ă��Ȃ��ꍇ�́A
        // �T�[�o�ɒʒm�ł��Ȃ��̂ŁA���M��x�点��B
//        DWCi_GetMatchCnt()->cmdResendCount = 1;
        DWCi_GetMatchCnt()->cmdResendFlag = TRUE;
        DWCi_GetMatchCnt()->cmdResendTick = OS_GetTick();
        DWCi_GetMatchCnt()->sbPidList[0] = profileID;

        if (!delay){
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "Delay ResvCommand - qr2IP & qr2Port = 0.\n");
        }
        return 0;
    }

    if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
        // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�́A�T�[�o���̃v���t�@�C��ID�A
        // QR2�p�p�u���b�NIP�A�|�[�g�ԍ������X�g�ɃZ�b�g
        server = ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, 0); 
        DWCi_GetMatchCnt()->sbPidList[0]   =
            SBServerGetIntValue(server, DWC_QR2_PID_KEY_STR, 0);
        DWCi_GetMatchCnt()->qr2IPList[0]   = SBServerGetPublicInetAddress(server);
        DWCi_GetMatchCnt()->qr2PortList[0] = SBServerGetPublicQueryPort(server);

        // NN�v�����M��v���t�@�C��ID���L�^
        DWCi_GetMatchCnt()->reqProfileID = DWCi_GetMatchCnt()->sbPidList[0];
        sendlen = 1;
    }
    else {
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
            // �v���t�@�C��ID���X�g�̍ŏ��ɃT�[�o���̃v���t�@�C��ID���Z�b�g
            DWCi_GetMatchCnt()->sbPidList[0] = profileID;
        }

        // NN�v�����M��v���t�@�C��ID���L�^
        DWCi_GetMatchCnt()->reqProfileID = profileID;

        // �F�B�w��A�T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ��
        // ������QR2�p�p�u���b�NIP�A�|�[�g�ԍ��𑗐M����B
        // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�́ASB���b�Z�[�W�w�b�_�Ɋi�[����Ă���B
        senddata[1] = DWCi_GetMatchCnt()->qr2IP;
        senddata[2] = DWCi_GetMatchCnt()->qr2Port;
        sendlen = 3;
    }
    
//    DWCi_GetMatchCnt()->cmdTimeoutCount = 1;  // �^�C���A�E�g�J�E���g�J�n
    DWCi_GetMatchCnt()->cmdTimeoutTime = DWC_MATCH_CMD_RESV_TIMEOUT_MSEC;
    DWCi_GetMatchCnt()->cmdTimeoutStartTick = OS_GetTick();
//    DWCi_GetMatchCnt()->cmdResendCount  = 0;
    DWCi_GetMatchCnt()->cmdResendFlag  = FALSE;
        
    if (DWCi_GetMatchCnt()->priorProfileID){
        // �T�[�o����ؒf����A�ؒf���T�[�o���玟�̐ڑ���T�[�o��
        // �������ꂽ�ꍇ�́A�D��\��R�}���h�𑗐M����
        command = DWC_MATCH_COMMAND_RESV_PRIOR;
    }
    else {
        // �ʏ�̗\��R�}���h
        command = DWC_MATCH_COMMAND_RESERVATION;
    }

    // ���M�f�[�^�Ɏ����̃}�b�`���C�N�^�C�v���Z�b�g����
    senddata[0] = DWCi_GetMatchCnt()->qr2MatchType;

    // NN�\��R�}���h���M
    // �\��R�}���h�̑��M�͕K���T�[�o�ɑ΂��čs���̂ŃC���f�b�N�X��[0]
    result = DWCi_SendMatchCommand(command,
                                   profileID,
                                   DWCi_GetMatchCnt()->qr2IPList[0],
                                   DWCi_GetMatchCnt()->qr2PortList[0],
                                   senddata, sendlen);

    return result;
}


/*---------------------------------------------------------------------------*
  �F�B�w��s�A�}�b�`���C�N�pNN�\��R�}���h���M�֐�
  �����@�Fdelay     TRUE:���t���[����ɃR�}���h���M����A
                    FALSE:���R�}���h���M����
          init      TRUE:����Ăяo���BfriendCount��+1�������Ȃ��ꍇ�A
                    FALSE:����ȊO
          resendPid �F�B�w��s�A�}�b�`���C�N�ŁAdelay = FALSE �ł����Ă��A
                    �O��Ɠ����v���t�@�C��ID�ɑ΂��ĘA�����ė\��R�}���h��
                    ���M����ꍇ�ɊԊu���󂯂����ꍇ�́A�O�񑗐M���
                    �v���t�@�C��ID���w�肷��B���̕K�v���Ȃ����0���w�肷��B
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l�B
          ���� delay = TRUE �̎��͕K��0
  �p�r�@�F�F�B�C���f�b�N�X���X�g���玟�ɐڑ������݂�z�X�g��I�����A
          ���̃z�X�g�̏����Z�b�g���āANN�\��R�}���h�𑗐M����B
          ���� delay = TRUE �̎��́A��莞�Ԍ��NN�\��R�}���h�𑗐M����B
 *---------------------------------------------------------------------------*/
static int DWCi_SendResvCommandToFriend(BOOL delay, BOOL init, int resendPid)
{
    char version[12];
    char numEntry[4];
    char distantFriend[2];
    int  profileID;
    int  startIdx = init ? DWCi_GetMatchCnt()->friendCount :
                           (DWCi_GetMatchCnt()->friendCount < DWCi_GetMatchCnt()->friendIdxListLen-1) ? DWCi_GetMatchCnt()->friendCount+1 : 0;
    int  buddyIdx;
    int  initFinished = 0;
    int  versionLen, numEntryLen, distantFriendLen;
    int  i;
    GPBuddyStatus status;
    GPResult gpResult;

    while (1){
        if (!init || initFinished){
            DWCi_GetMatchCnt()->friendCount++;
            if (DWCi_GetMatchCnt()->friendCount >= DWCi_GetMatchCnt()->friendIdxListLen){
                // �Ō�܂Œ��ׂ���ŏ��ɖ߂�
                DWCi_GetMatchCnt()->friendCount = 0;
            }
        }

        if (initFinished){
            if (DWCi_GetMatchCnt()->friendCount == startIdx){
                // 1���[�v���Ă��N�����Ȃ���΃^�C���A�E�g�ł̍đ���҂�
                // �����A�Q�l�������Ƀ}�b�`���C�N���J�n�����ꍇ�́A
                // �����҂Ă΂�����GPStatus���X�V����A���m�ł���̂ŁA
                // �^�C���A�E�g�܂ł̎��Ԃ�Z�����Ă���
//                DWCi_GetMatchCnt()->cmdTimeoutCount =
//                    DWC_MATCH_CMD_RESV_TIMEOUT-DWC_MATCH_CMD_RESEND_INTERVAL;
                DWCi_GetMatchCnt()->cmdTimeoutTime = DWC_MATCH_CMD_RESEND_INTERVAL_MSEC;
                DWCi_GetMatchCnt()->cmdTimeoutStartTick = OS_GetTick();

//                DWCi_GetMatchCnt()->cmdResendCount  = 0;
                DWCi_GetMatchCnt()->cmdResendFlag  = FALSE;
                return 0;
            }
        }
        initFinished = 1;

        profileID = DWC_GetGsProfileId(DWCi_GetUserData(), &DWCi_GetMatchCnt()->friendList[DWCi_GetMatchCnt()->friendIdxList[DWCi_GetMatchCnt()->friendCount]]);

#ifdef DWC_MATCH_ACCEPT_NO_FRIEND
        // �܂��v���t�@�C��ID���ł��Ă��Ȃ���Ή������Ȃ�
        if ((profileID == 0) || (profileID == -1)) continue;

        // �����ς݃o�f�B�łȂ��ꍇ���������Ȃ�
        if (!DWCi_Acc_IsValidFriendData(&DWCi_GetMatchCnt()->friendList[DWCi_GetMatchCnt()->friendIdxList[DWCi_GetMatchCnt()->friendCount]])) continue;
#endif

        // ���̃T�[�o��₪���ɐڑ��ς݂łȂ������ׂ�
        for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
            if (DWCi_GetMatchCnt()->sbPidList[i] == profileID) break;
        }

        if (i > DWCi_GetMatchCnt()->gt2NumConnection){
            // ���肪�F�B�w��s�A�}�b�`���C�N���ł��邩�ǂ������`�F�b�N����
            gpResult  = gpGetBuddyIndex(DWCi_GetMatchCnt()->pGpObj, profileID, &buddyIdx);
            gpResult |= gpGetBuddyStatus(DWCi_GetMatchCnt()->pGpObj, buddyIdx, &status);

            if (!gpResult && (status.status == DWC_STATUS_MATCH_FRIEND)){
                // ������F�B�w��s�A�}�b�`���C�N���Ȃ̂ŁA�o�[�W������������
                // �����Ă��邩�A�ڑ��v���l���������ƍ����Ă��邩�A
                // �F�B�̗F�B�������������Ɠ��������ׂ�
                versionLen =
                    DWC_GetCommonValueString(DWC_GP_SSTR_KEY_MATCH_VERSION,
                                             version, 
                                             status.statusString,
                                             '/');
                numEntryLen = 
                    DWC_GetCommonValueString(DWC_GP_SSTR_KEY_MATCH_FRIEND_NUM,
                                             numEntry, 
                                             status.statusString,
                                             '/');
                distantFriendLen = 
                    DWC_GetCommonValueString(DWC_GP_SSTR_KEY_DISTANT_FRIEND,
                                             distantFriend,
                                             status.statusString,
                                             '/');

                if ((versionLen > 0) && (numEntryLen > 0) && (distantFriendLen > 0) &&
                    (strtoul(version, NULL, 10) == DWC_MATCHING_VERSION) &&
                    (strtoul(numEntry, NULL, 10) == DWCi_GetMatchCnt()->qr2NumEntry)){
                    // �\�񑗐M��T�[�o�Ƃ��Č���
                    break;
                }
            }
            // gpResult�̃G���[�͖���
        }
    }

    // �F�B����l�������Ȃ��ꍇ�ȂǂɁA�O��Ɠ����v���t�@�C��ID�ɊԊu���󂯂���
    // �A�����M���Ă��܂����Ƃ�h�����߂ɁA�O�񑗐M��v���t�@�C��ID resendPid��
    // �ݒ肳��Ă���ꍇ�́A��������M�悪�����Ȃ���t���[����ɑ��M����
    if (profileID == resendPid) delay = TRUE;

    // ���̗F�B��NN�\��v���𑗐M����
    return DWCi_SendResvCommand(profileID, delay);
}


/*---------------------------------------------------------------------------*
  �\��R�}���h�đ����g���C�֐�
  �����@�FresendPid �F�B�w��s�A�}�b�`���C�N�ŁAdelay = FALSE �ł����Ă��A
                    �O��Ɠ����v���t�@�C��ID�ɑ΂��ĘA�����ė\��R�}���h��
                    ���M����ꍇ�ɊԊu���󂯂����ꍇ�́A�O�񑗐M���
                    �v���t�@�C��ID���w�肷��B���̕K�v���Ȃ����0���w�肷��B
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l
  �p�r�@�FRESV_DENY�R�}���h��M����A�R�}���h�����҂��^�C���A�E�g��ȂǂɁA
          �\��R�}���h���M�������s���B
          �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�̏ꍇ�̓��g���C�̕K�v��
          �Ȃ��B�G���[�����͊֐����ōs���Ă���B
 *---------------------------------------------------------------------------*/
static BOOL DWCi_RetryReserving(int resendPid)
{
    SBError sbError;
    int result;

    DWCi_GetMatchCnt()->priorProfileID = 0;  // �D��\��N���A
    DWCi_GetMatchCnt()->reqProfileID   = 0;  // �\��N���A

    DWCi_GetMatchCnt()->resvWaitCount  = 0;  // WAIT��M�ɂ��đ��J�E���^�N���A
    // �^�C���A�E�g�v���J�n���ԍăZ�b�g
    DWCi_GetMatchCnt()->cmdTimeoutStartTick = OS_GetTick();

    if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
        // �󂫃z�X�g�̌�����Ԃɖ߂�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);

        // SB�T�[�o�A�b�v�f�[�g���s��
        // [todo]
        // ���ꂾ�Ɗ��ɃN���C�A���g���q�����Ă���T�[�o�̏ꍇ���A�Ԃ�u������
        // SB�A�b�v�f�[�g���s���Ă��܂��̂ŗǂ��Ȃ�
        sbError = DWCi_SBUpdateAsync(0);
        if (DWCi_HandleSBError(sbError)) return FALSE;
    }
    else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
        // ���̗F�B��NN�\��v���𑗐M����
        result = DWCi_SendResvCommandToFriend(FALSE, FALSE, resendPid);
        if (DWCi_HandleMatchCommandError(result)) return FALSE;
    }
    else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
        // �T�[�o�������ȃ^�C�~���O�Œ���I�[�o�[�ɂȂ������A
        // �}�b�`���C�N�L�����Z�������̂Ń}�b�`���C�N�𒆒f����
        DWCi_StopMatching(DWC_ERROR_NETWORK,
                          DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_OTHER);
        return FALSE;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  NN�\��L�����Z���֐�
  �����@�FprofileID �\����L�����Z���������T�[�o�̃v���t�@�C��ID�B
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l
  �p�r�@�FNN�\��L�����Z���R�}���h�𑗐M����B
 *---------------------------------------------------------------------------*/
static int  DWCi_CancelReservation(int profileID)
{
    int result;

    result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_RESV_CANCEL,
                                   profileID,
                                   DWCi_GetMatchCnt()->qr2IPList[0],
                                   DWCi_GetMatchCnt()->qr2PortList[0],
                                   NULL, 0);

    // NN�v�����M��v���t�@�C��ID���N���A
    DWCi_GetMatchCnt()->reqProfileID = 0;

    return result;
}


/*---------------------------------------------------------------------------*
  �ڑ��m���O�T�[�o�̏����L�����Z���֐�
  �����@�FclientPid �\��ς݃N���C�A���g�̃v���t�@�C��ID
  �߂�l�FTRUE:�����AFALSE:���s
  �p�r�@�F�ڑ��m���O�̂���蒆�̃T�[�o�ɂ����āA�S�Ă̏�����

          �L�����Z�����A������R�l�N�V�����͂��̂܂܂Ń}�b�`���C�N���ĊJ����B
          �V�K�ڑ��N���C�A���g�Ƃ�NN���s�AGT2�R�l�N�g���s�Ƃ������ꍇ�́A
          �ڑ��ς݃N���C�A���g�ł����̊֐��ɗ��邱�Ƃ�����B
          ���̊֐��́A�V�K�ڑ��N���C�A���g�Ƃ�gt2Connect���������Ă��Ȃ�
          ��ԂŌĂ΂�邱�Ƃ�O��Ƃ��Ă���B
 *---------------------------------------------------------------------------*/
static BOOL DWCi_CancelPreConnectedServerProcess(int clientPid)
{
    int  i;
    BOOL isServer;

    if (DWCi_GetMatchCnt()->qr2IsReserved && (DWCi_GetMatchCnt()->qr2Reservation == DWCi_GetMatchCnt()->profileID)){
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "DWCi_CancelPreConnectedServerProcess : client\n");
        isServer = FALSE;
    }
    else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "DWCi_CancelPreConnectedServerProcess : server\n");
        isServer = TRUE;
    }

    if (isServer){
        // �T�[�o�Ȃ�\����N���A�B
        // �N���C�A���g�̏ꍇ�A�ʏ�͂��̐��DWCi_RestartFromTimeout()����
        // �\����N���A���邪�A�T�[�o�N���C�A���g�}�b�`���C�N�̐ڑ��ς�
        // �N���C�A���g�̓}�b�`���C�N���p������̂ŁA�����̃v���t�@�C��ID�ł�
        // �\����N���A���Ȃ��B
        DWCi_GetMatchCnt()->qr2IsReserved   = 0;
        DWCi_GetMatchCnt()->qr2Reservation  = 0;
    
        // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
        qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);
    }

    // �\��N���C�A���g�̃v���t�@�C��ID�����X�g��������B
    // gt2NumConnection�ɐV�K�ڑ��N���C�A���g�̕��������Ă��Ȃ����Ƃ��O��B
    if (DWCi_GetMatchCnt()->gt2NumConnection < DWC_MAX_CONNECTIONS-1){
        DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection+1] = 0;
    }

    // �\��ێ��^�C���A�E�g�v���I��
    DWCi_GetMatchCnt()->cmdCnt.command = DWC_MATCH_COMMAND_DUMMY;

    if (DWCi_GetMatchCnt()->nnInfo.cookie){
        // NAT�l�S�V�G�[�V�������Ȃ�L�����Z��
        NNCancel(DWCi_GetMatchCnt()->nnInfo.cookie);
        DWCi_GetMatchCnt()->nnInfo.cookie = 0;
    }

    // �T�[�o�����ĊJ�̂��߂Ƀp�����[�^�������O��Ԃɖ߂�
    DWCi_GetMatchCnt()->qr2NNFinishCount = (u8)DWCi_GetMatchCnt()->gt2NumConnection;
    DWCi_GetMatchCnt()->reqProfileID     = 0;

    if (!isServer){
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
            // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�ڑ��ς݃N���C�A���g��
            // �V�K�ڑ��N���C�A���g�̉������������o�����ꍇ�́A
            // �T�[�o���N���C�A���g���m�̐ڑ��^�C���A�E�g�����o���āA
            // �L�����Z���R�}���h�𑗂��Ă����̂�҂B
            // [todo]
            // �L�����Z���R�}���h��M�܂ł̃^�C���A�E�g���Ԃ�ݒ肵�āA
            // �I�[�o�[������T�[�o���V���b�g�_�E�������Ɣ��f����K�v������
        }
        else {
            // ���̑��̃}�b�`���C�N�^�C�v�ł́A�R�l�N�V������S�ăN���[�Y����
            // �}�b�`���C�N����蒼��
            DWCi_RestartFromTimeout();
        }
    }
    else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
        // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�A�󂫃z�X�g������Ԃɖ߂�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);
        
        // ���t���[����ɋ󂫃z�X�g�̌������s��
//        DWCi_GetMatchCnt()->sbUpdateCount = 1;
        DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_LONG;
        DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
    }
    else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
        // �F�B�w��s�A�}�b�`���C�N�̏ꍇ�A�ŏ��̏�Ԃɖ߂�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);

        // ���t���[�����NN�\��v�����M���s��
        (void)DWCi_SendResvCommandToFriend(TRUE, FALSE, 0);
    }
    else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�́A
        // �ڑ��ς݃N���C�A���g�S���ɃL�����Z����`����B
        // �L�����Z��SYN-ACK��M�҂���Ԃɐi��
        DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_CANCEL_SYN);

        DWCi_GetMatchCnt()->cancelSynAckBit   = 0;  // �L�����Z��SYN-ACK��MBit�N���A
        DWCi_GetMatchCnt()->cancelBaseLatency = 0;  // �L�����Z�������p���C�e���V�N���A

        // �V�K�ڑ��N���C�A���g���N���[�Y����
        DWCi_CloseCancelHostAsync(clientPid);

        // �ڑ��ς݃N���C�A���g��SYN�p�P�b�g�𑗐M����B
        for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
            if (!DWCi_SendCancelMatchSynCommand(DWCi_GetMatchCnt()->sbPidList[i],
                                                DWC_MATCH_COMMAND_CANCEL_SYN))
                return FALSE;
        }

        if (DWCi_GetMatchCnt()->gt2NumConnection == 0){
            // �ڑ��z�X�g���Ȃ���Ώ��SYN�p�P�b�g���N���[�Y���������Ȃ��̂�
            // ���������}�b�`���C�N��Ԃɂ���
            DWCi_RestartFromCancel(DWC_MATCH_RESET_CONTINUE);
        }
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �ڑ��m���O�N���C�A���g�̏����L�����Z���֐�
  �����@�FserverPid �\���T�[�o�̃v���t�@�C��ID
  �߂�l�FTRUE:�����AFALSE:���s
  �p�r�@�F�ڑ��m���O�̂���蒆�̃N���C�A���g�ɂ����āA�S�Ă̏�����
          �L�����Z������
 *---------------------------------------------------------------------------*/
static BOOL DWCi_CancelPreConnectedClientProcess(int serverPid)
{
#pragma unused(serverPid)
    BOOL result = TRUE;

    if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̐V�K�ڑ��N���C�A���g�Ȃ�
        // �ڑ��������G���[�I������
        if (DWCi_GetMatchCnt()->gt2NumConnection){
            // �S�ẴR�l�N�V�������N���[�Y����
            DWCi_CloseAllConnectionsByTimeout();
        }

        // �}�b�`���C�N�G���[�I��
        DWCi_StopMatching(DWC_ERROR_NETWORK,
                          DWC_ECODE_SEQ_MATCH+DWC_ECODE_TYPE_SC_CL_FAIL);
        return FALSE;
    }

    // �T�[�o�����ĊJ�̂��߂Ƀp�����[�^�������O��Ԃɖ߂�
    DWCi_GetMatchCnt()->qr2NNFinishCount = (u8)DWCi_GetMatchCnt()->gt2NumConnection;
    DWCi_GetMatchCnt()->priorProfileID   = 0;

    if (DWCi_GetMatchCnt()->nnInfo.cookie){
        // NAT�l�S�V�G�[�V�������Ȃ�L�����Z��
        NNCancel(DWCi_GetMatchCnt()->nnInfo.cookie);
        DWCi_GetMatchCnt()->nnInfo.cookie = 0;
    }

    if (DWCi_GetMatchCnt()->gt2NumConnection){
        // �T�[�o�Ɍq�����Ă���N���C�A���g�Ɛڑ����̏ꍇ�A
        // �������̓T�[�o�Ɛڑ����̏ꍇ�ł��A����gt2Connect()��Accept����
        // ���܂��Ă�����A�R�l�N�V������S�ăN���[�Y���ă}�b�`���C�N����蒼��
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "Close all connection and restart matching.\n");
            
        DWCi_RestartFromTimeout();
    }
    else {
        // �܂��R�l�N�V�������Ȃ���ԂȂ�A��Ԃ����ɖ߂���
        // �N���C�A���g�̃}�b�`���C�N�������ĊJ����
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "Cancel and restart client process.\n");
        
        // ��Ԃ�߂�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);

        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Cancel and retry to reserve.\n");

        // RESV_DENY�R�}���h����M�������Ɠ��������ŁA���̃T�[�o��T��
        result = DWCi_RetryReserving(0);
    }

    return result;
}


/*---------------------------------------------------------------------------*
  �N���C�A���g�]���֐�
  �����@�F�Ȃ�
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l
  �p�r�@�F���S�ɃN���C�A���g�ɖ߂��āA�V�K�T�[�o�֐ڑ����ɍs��
 *---------------------------------------------------------------------------*/
static int  DWCi_ChangeToClient(void)
{
    int result;
    int i;

    // �ڑ��ς݂̃N���C�A���g�ɃN���[�Y���߂𑗐M����
    for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
        result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_CLOSE_LINK,
                                       DWCi_GetMatchCnt()->sbPidList[i],
                                       DWCi_GetMatchCnt()->qr2IPList[i],
                                       DWCi_GetMatchCnt()->qr2PortList[i],
                                       DWCi_GetMatchCnt()->svDataBak,
                                       (int)(DWCi_GetMatchCnt()->svDataBak[0]+1));
        if (result) return result;
    }

    DWCi_GetMatchCnt()->qr2IsReserved  = 0;
    DWCi_GetMatchCnt()->qr2Reservation = 0;

    // �N���[�Y��Ԃ��N���C�A���g�ւ̓]���������ɂ���
    DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_CHANGE_TO_CLIENT;
    
    // �S�ẴR�l�N�V������ؒf����
    gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);

    // �N���[�Y��ԏ�����
    DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_INIT;

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "Closed all connections. Begin NN to %u\n",
               DWCi_GetMatchCnt()->reqProfileID);

    return 0;
}


/*---------------------------------------------------------------------------*
  �R�l�N�V�����m���㏈���֐�
  �����@�Ftype ������ʁBDWCMatchPpConnectionType�^�B
  �߂�l�F�Ȃ�
  �p�r�@�F�P�̃R�l�N�V�����m�����I�������Ɏ��̃R�l�N�V�����m���J�n�A
          �������͐ڑ������������s���B
 *---------------------------------------------------------------------------*/
static void DWCi_PostProcessConnection(DWCMatchPpConnectionType type)
{
    u32 senddata[5];
    int sendlen = 3;
    int result;
    int i;
    BOOL sbClear = FALSE;
    GPResult gpResult;

    switch (type){
    case DWC_PP_CONNECTION_SV_CONNECT:
        // �T�[�o�ňꌏ�ڑ�����
        if (DWCi_GetMatchCnt()->clLinkProgress < DWCi_GetMatchCnt()->gt2NumConnection-1){
            // �V�K�ڑ��N���C�A���g�́A�ڑ��ς݃N���C�A���g�ւ̐ڑ���
            // �S�ďI����Ă��Ȃ��ꍇ
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "Send client-client link request.\n");

            // �N���C�A���g���m�̃R�l�N�V�����m���҂���ԂɈڍs����
            DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_WAIT_CL_LINK);

            // �N���C�A���g�ɐڑ����ɍs���Ăق������N���C�A���g��
            // �v���t�@�C��ID��AID�C���f�b�N�X�AAID��ʒm����
            senddata[0] = (u32)DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->clLinkProgress+1];
            senddata[1] = (u32)(DWCi_GetMatchCnt()->clLinkProgress+1);
            senddata[2] = DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->clLinkProgress+1];

            // qr2�p�p�u���b�NIP�ƃ|�[�g�ԍ������M����
            senddata[3] = DWCi_GetMatchCnt()->qr2IPList[DWCi_GetMatchCnt()->clLinkProgress+1];
            senddata[4] = DWCi_GetMatchCnt()->qr2PortList[DWCi_GetMatchCnt()->clLinkProgress+1];
            sendlen     = 5;
        }
        else {
            // ���߂ẴN���C�A���g�Ƃ̎�����GT2�ڑ������������ꍇ���A
            // �S�Ă̐ڑ��N���C�A���g���m�̐ڑ����I������ꍇ
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "Tell new client completion of matching.\n");
            // �\�������
            DWCi_GetMatchCnt()->qr2IsReserved  = 0;
            DWCi_GetMatchCnt()->qr2Reservation = 0;

            // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
            qr2_send_statechanged(DWCi_GetMatchCnt()->qr2Obj);

            if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
                // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�́A�󂫃z�X�g�̌�����Ԃɖ߂�
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);
            }
            else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
                // �F�B�w��s�A�}�b�`���C�N�̏ꍇ�̓N���C�A���g������Ԃɖ߂�
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);
            }
            else {
                // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�͑҂���Ԃɖ߂�
                DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_WAITING);
            }
            
            DWCi_GetMatchCnt()->clLinkProgress = 0;  // �N���C�A���g�Ԑڑ����N���A

            if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
                (DWCi_GetMatchCnt()->gt2NumConnection == DWCi_GetMatchCnt()->qr2NumEntry)){
                // �K��l���̃l�b�g���[�N�����i�T�[�o�N���C�A���g�}�b�`���C�N��
                // �ꍇ�͂P��ڑ����Ɓj�B
                // �R�[���o�b�N�ɓn���v���t�@�C��ID��ޔ�
                if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
                    DWCi_GetMatchCnt()->cbEventPid =
                        DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection];
                }
                else {
                    DWCi_GetMatchCnt()->cbEventPid = 0;

                    // �T�[�o����N���C�A���g�ɓ]�������z�X�g�̂��߂ɁA
                    // �����ŉ��߂Ď����̃v���t�@�C��ID�����X�g�ɃZ�b�g����
                    // �i����ڑ����̃}�b�`���C�N���������Ă����ꍇ�ɕK�v�j
                    DWCi_GetMatchCnt()->sbPidList[0] = DWCi_GetMatchCnt()->profileID;
                }

                // SYN-ACK��M�҂���Ԃɐi��
                DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_SYN);

                // SYN-ACK��MBit�N���A
                DWCi_GetMatchCnt()->synAckBit = 0;

                // �ڑ��ς݃N���C�A���g��SYN�p�P�b�g�𑗐M����B
                for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                    DWCi_SendMatchSynPacket(DWCi_GetMatchCnt()->aidList[i],
                                            DWC_SEND_TYPE_MATCH_SYN);
                }
            }
            else {
                // �F�B�w��E���w��s�A�}�b�`���C�N�Őڑ��z�X�g���K�萔�ɖ����Ȃ��ꍇ
                // �Ԃ�u���ĐV���Ȑڑ����T��
                senddata[0] = 0;  // �v���t�@�C��ID=0�ŏI����ʒm
                // �V�K�ڑ��N���C�A���g��AID�C���f�b�N�X��AID��������
                senddata[1] = (u8)DWCi_GetMatchCnt()->gt2NumConnection;
                senddata[2] = DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->gt2NumConnection];

                if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
                    // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�́A�󂫃z�X�g�̌�����Ԃɖ߂�
//                    DWCi_GetMatchCnt()->sbUpdateCount = 1;
                    DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_LONG;
                    DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
                }
                else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
                    // �F�B�w��s�A�}�b�`���C�N�̏ꍇ�́A���t���[�����
                    // NN�\��v���𑗐M����
                    (void)DWCi_SendResvCommandToFriend(TRUE, FALSE, 0);

#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
                    if (!DWCi_GetMatchCnt()->distantFriend &&
                        (DWCi_GetMatchCnt()->gt2NumConnection >= 2)){
                        // �F�B�̗F�B�Ƃ̐ڑ����������A�Q�l�ڈȍ~��
                        // �N���C�A���g�Ƃ̐ڑ����I�����ꍇ�́A
                        // �ڑ��ς݃N���C�A���g����̐V�K�ڑ��F�B���R�}���h��
                        // �S�Ď�M�ł��Ă��邩���ׂ�
                        if (DWCi_GetMatchCnt()->friendAcceptBit !=
                            (DWCi_GetAIDBitmask(FALSE) & ~(1 << DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->gt2NumConnection]))){
                            // ��M�ł��Ă��Ȃ���΃}�b�`���C�N����蒼��
                            // [todo]
                            // �x�������������ꍇ�̂��Ƃ��l����ƁA��������
                            // �҂��������ǂ�
                            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                                       "FRIEND_ACCEPT command droped.\n");
                            DWCi_RestartFromTimeout();
                            break;
                        }
                    }
#endif
                }
            }

            if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_SV){
                // �T�[�o�N���C�A���g�}�b�`���C�N�łȂ����
                // �T�[�o�u���E�W���O���N���A
                sbClear = TRUE;
            }
        }

        if (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_SV_SYN){
            // �N���C�A���g���m�̐ڑ��v���ʒm���s���i�S�ďI���Ȃ�I���ʒm�ɂȂ�j
            result =
                DWCi_SendMatchCommand(DWC_MATCH_COMMAND_LINK_CLS_REQ,
                                      DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection],
                                      DWCi_GetMatchCnt()->qr2IPList[DWCi_GetMatchCnt()->gt2NumConnection],
                                      DWCi_GetMatchCnt()->qr2PortList[DWCi_GetMatchCnt()->gt2NumConnection],
                                      senddata, sendlen);
            if (DWCi_HandleMatchCommandError(result)) return;

            DWCi_GetMatchCnt()->cmdCnt.count = 0;  // �R�}���h�đ��J�E���g�J�n
        }
        break;
    
    case DWC_PP_CONNECTION_CL_GT2_CONNECT:
        // �҂���Ԃɖ߂�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAITING);

        // �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�̏ꍇ��
        // �����ŃR�[���o�b�N�ɓn���v���t�@�C��ID��ޔ�
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
            DWCi_GetMatchCnt()->cbEventPid = DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection];
        }

        // �T�[�o�u���E�W���O���N���A
        sbClear = TRUE;
        break;

    case DWC_PP_CONNECTION_CL_GT2_ACCEPT:
        // �҂���ԂɂȂ�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAITING);

        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
            (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
            // �F�B�w��E���w��s�A�}�b�`���C�N�̏ꍇ�́A�����ŏ��߂Ď�����
            // �v���t�@�C��ID�ŗ\���Ԃɂ��āA���̃z�X�g���ڑ����Ă���̂�h��
            DWCi_GetMatchCnt()->qr2IsReserved  = 1;
            DWCi_GetMatchCnt()->qr2Reservation = DWCi_GetMatchCnt()->profileID;
        }

        // ���ɃT�[�o����DWC_MATCH_COMMAND_LINK_CLS_REQ�R�}���h����M����܂ł�
        // �҂���ԂȂ̂ŁA�^�C���A�E�g�J�E���^������������
        DWCi_InitClWaitTimeout();
        
        if (DWCi_GetMatchCnt()->gt2NumConnection > 1){
            // �V�K�ڑ��N���C�A���g���A�ڑ��ς݂̃N���C�A���g�Ƃ�
            // �R�l�N�V�������m�������ꍇ�́A�T�[�o�Ɋ�����`����B
            // ���R�l�N�V�������m�������N���C�A���g�̃v���t�@�C��ID���`����B
            result =
                DWCi_SendMatchCommand(DWC_MATCH_COMMAND_LINK_CLS_SUC,
                                      DWCi_GetMatchCnt()->sbPidList[0],
                                      DWCi_GetMatchCnt()->qr2IPList[0],
                                      DWCi_GetMatchCnt()->qr2PortList[0],
                                      (u32 *)&DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection-1],
                                      1);
            if (DWCi_HandleMatchCommandError(result)) return;
        }
        // �T�[�o����̐ڑ��������ڑ��ς݂̃N���C�A���g�ւ̐ڑ����̎w����҂�
        break;
        
    case DWC_PP_CONNECTION_CL_FINISH_CONNECT:
        // CLS_REQ�R�}���h�������Ă����v�Ȃ悤�ɁASYN�p�P�b�g�ŕK��
        // aid�𑗐M����悤�ɂ��Ă���̂Ń��J�o�[�ł��邪�A
        // �T�[�o�u���E�W���O�̃N���A���ł��Ȃ��̂ŁA�Ȃ��Ȃ��}�b�`���C�N��
        // �I�����Ȃ����SB�^�C���A�E�g�G���[�ɂȂ��Ă��܂�
            
        // �҂���ԂɂȂ�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAITING);

        // �}�b�`���C�N����������܂ł͑҂���ԂɂȂ�̂ŁA
        // �^�C���A�E�g�J�E���^������������
        DWCi_InitClWaitTimeout();

        // �R�[���o�b�N�ɓn���v���t�@�C��ID�͕K�v�Ȃ�
        DWCi_GetMatchCnt()->cbEventPid = 0;

        // �T�[�o�u���E�W���O���N���A
        sbClear = TRUE;
        break;

    case DWC_PP_CONNECTION_SYN_FINISH:
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Completed matching!\n");

        if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_SV){
            // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�łȂ����
            // GP�X�e�[�^�X���Q�[���v���C��ԂɃZ�b�g����
            (void)DWCi_SetGPStatus(DWC_STATUS_PLAYING, "", NULL);
        }

        // dwc_main.c�̃R�[���o�b�N���Ăяo��
        DWCi_GetMatchCnt()->matchedCallback(DWC_ERROR_NONE,
                                     FALSE,
                                     DWCi_GetMatchCnt()->cbEventPid ? FALSE : TRUE,
                                     FALSE,
                                     DWCi_GetFriendListIndex(DWCi_GetMatchCnt()->cbEventPid),
                                     DWCi_GetMatchCnt()->matchedParam);

        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
            (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
            // �F�B�w��E���w��s�A�}�b�`���C�N�͓r���Q���Ȃ��Ȃ̂�
            // ������GameSpy�̐���\���̂��J������
            DWCi_CloseMatching();
        }
        else {
#ifndef DWC_BUG_WIFI_DELAY_SEND
            // �T�[�o�N���C�A���g�}�b�`���C�N�ł�SB�I�u�W�F�N�g�̂݊J������
            if (DWCi_GetMatchCnt()->sbObj){
                ServerBrowserFree(DWCi_GetMatchCnt()->sbObj);
                DWCi_GetMatchCnt()->sbObj = NULL;  // ������NULL�N���A���Ȃ���΂Ȃ�Ȃ��I
            }
#endif

            // NN�g�p�̈���J������
            DWCi_NNFreeNegotiateList();

            if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV){
                // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�́A
                // �ڑ��l���̑�����GP�X�e�[�^�X�ɃZ�b�g����
                gpResult = DWCi_GPSetServerStatus();
                if (DWCi_HandleGPError(gpResult)) return;

                // �}�b�`���C�N���P������������V�K�ڑ����u���b�N����I�v�V������
                // �L���Ȃ�A�����Ńu���b�N����
                if (stOptSCBlock.valid == 1){
                    stOptSCBlock.lock = 1;
                    DWC_Printf(DWC_REPORTFLAG_DEBUG,
                               "[OPT_SC_BLOCK] Connect block start!\n");
                }

                // ��Ԃ��T�[�o�҂���Ԃɖ߂�
                DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_WAITING);
            }
            else {
                // ��Ԃ��N���C�A���g�҂���Ԃɖ߂�
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAITING);
            }

            // �R�[���o�b�N�ɓn���ׂ̃v���t�@�C��ID������������
            DWCi_GetMatchCnt()->cbEventPid = 0;
        }

        // �L�����Z�����������ꂽ�ꍇ�ׂ̈ɃL�����Z����Ԃ�����������
        DWCi_GetMatchCnt()->cancelState = DWC_MATCH_CANCEL_STATE_INIT;
        break;
    }


#ifndef DWC_BUG_WIFI_DELAY_SEND
#if 1
    if (sbClear &&
        (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_CL)){
        // �F�B�w��A���w��s�A�}�b�`���C�N�̏ꍇ�A���΂炭�}�X�^�[�T�[�o��
        // �A�N�Z�X���Ȃ��ꍇ�́Adisconnect���Ȃ��ƒʐM�G���[�ɂȂ�̂�
        // �T�[�o���X�g�N���A
        ServerBrowserClear(DWCi_GetMatchCnt()->sbObj);
	}
#else
    if (sbClear &&
        (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_CL) &&
        (ServerBrowserState(DWCi_GetMatchCnt()->sbObj) != sb_disconnected)){
        // �F�B�w��A���w��s�}�b�`���C�N�̏ꍇ�A���΂炭�}�X�^�[�T�[�o��
        // �A�N�Z�X���Ȃ��ꍇ�́Adisconnect���Ȃ��ƒʐM�G���[�ɂȂ�̂�
        // �T�[�o���X�g�N���A
        ServerBrowserClear(DWCi_GetMatchCnt()->sbObj);
    }
#endif
#endif
}


/*---------------------------------------------------------------------------*
  �����o�f�B�m�F�֐�
  �����@�FpidList    �o�f�B���ǂ����m�F�������v���t�@�C��ID�̔z��ւ̃|�C���^
          pidListLen ��L�z��̗v�f��
  �߂�l�FTRUE:�S�Ẵv���t�@�C��ID���o�f�B�AFALSE:�o�f�B�łȂ��v���t�@�C��ID������
  �p�r�@�F�v���t�@�C��ID�̃��X�g�𒲂ׂāA�S�ăo�f�B���ǂ�����Ԃ�
 *---------------------------------------------------------------------------*/
static BOOL DWCi_AreAllBuddies(const u32 pidList[], u32 pidListLen)
{
    int i;

    // �F�B�̗F�B�������ꍇ�A�V�K�T�[�o���������N���C�A���g�͕K��
    // ���̃T�[�o�ւ̐ڑ���������
    if (DWCi_GetMatchCnt()->distantFriend &&
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAIT_RESV))
        return TRUE;

    for (i = 0; i < pidListLen; i++){
#ifdef DWC_LIMIT_FRIENDS_MATCH_VALID
        if (!DWCi_IsFriendByIdxList((int)pidList[i])) return FALSE;
#else
        if (!gpIsBuddy(DWCi_GetMatchCnt()->pGpObj, (int)pidList[i])) return FALSE;
#endif

        if (DWCi_GetMatchCnt()->distantFriend &&
            (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAITING)){
            // �F�B�̗F�B�������ꍇ�A���݂̃T�[�o����N���[�Y���ꂽ
            // �N���C�A���g�́A�V�K�T�[�o���ipidList[0]�j�̂�
            // �����̗F�B�Ȃ�A���̃T�[�o�ւ̐ڑ���������
            return TRUE;
        }
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�L�����Z�����s�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�̃L�����Z�����������s����
 *---------------------------------------------------------------------------*/
static void DWCi_DoCancelMatching(void)
{
    u32 senddata;
    int result;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "CANCEL! state %d, numHost nn=%d gt2=%d.\n",
               DWCi_GetMatchCnt()->state, DWCi_GetMatchCnt()->qr2NNFinishCount,
               DWCi_GetMatchCnt()->gt2NumConnection);

    // �����ŃL�����Z�������ꍇ�̓L�����Z���z�X�g�̃v���t�@�C��ID��0
    DWCi_GetMatchCnt()->cbEventPid = 0;
    
    if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_SEARCH_OWN){
        // ���ڑ��Ȃ̂ő��L�����Z�����������s�ł���
        DWCi_FinishCancelMatching();
        return;
    }

    // �L�����Z�����s��Ԃ��Z�b�g����
    DWCi_GetMatchCnt()->cancelState = DWC_MATCH_CANCEL_STATE_EXEC;

    if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̐V�K�ڑ��N���C�A���g���L�����Z��
        senddata = 0;

        // �L�����Z�����s��Ԃ��Z�b�g
        DWCi_GetMatchCnt()->cancelState = DWC_MATCH_CANCEL_STATE_EXEC;

        if (DWCi_GetMatchCnt()->gt2NumConnection){
            // �S�ẴR�l�N�V������ؒf����
            gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);
        }
        else {
            // �R�l�N�V�������܂��Ȃ��ꍇ�͗\��̃L�����Z���̂ݍs���B
            // ���̃R�}���h�������Ă��A�T�[�o���Ń^�C���A�E�g�ŋC�t���̂�
            // �đ��̕K�v�͂Ȃ��B
            result = DWCi_CancelReservation(DWCi_GetMatchCnt()->sbPidList[0]);
            if (DWCi_HandleMatchCommandError(result)) return;
        }

        // �L�����Z�������I��
        DWCi_FinishCancelMatching();
        return;
    }

    // gt2�R�l�N�V�������܂��ł��Ă��Ȃ����A�ڑ��������̃z�X�g�ɑ΂��鏈��
    if ((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAIT_RESV) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_SEARCH_NN_HOST) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_NN) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_GT2) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_NN) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_GT2)){
        if (DWCi_GetMatchCnt()->reqProfileID){
            // �N���ɗ\��R�}���h�𑗂�����̏�Ԃ̏ꍇ
            // �\��R�}���h�𑗂�������ɑ΂��ė\����L�����Z������
            result = DWCi_CancelReservation(DWCi_GetMatchCnt()->sbPidList[0]);
            if (DWCi_HandleMatchCommandError(result)) return;
        }

        // �������N���ɗ\�񂳂�Ă���΁A�}�b�`���C�N�L�����Z���R�}���h��
        // �N���C�A���g�ɑ��M
        result = DWCi_InvalidateReservation();
        if (DWCi_HandleMatchCommandError(result)) return;
    }

    if (DWCi_GetMatchCnt()->nnInfo.cookie){
        // NAT�l�S�V�G�[�V�������Ȃ�L�����Z��
        NNCancel(DWCi_GetMatchCnt()->nnInfo.cookie);
        DWCi_GetMatchCnt()->nnInfo.cookie = 0;
    }

    if (DWCi_GetMatchCnt()->gt2NumConnection){
        // �L�����Z�����s��ԂɈڍs����
        DWCi_GetMatchCnt()->cancelState = DWC_MATCH_CANCEL_STATE_EXEC;

        // �S�ẴR�l�N�V������ؒf����
        gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);
    }

    // �L�����Z�������I��
    DWCi_FinishCancelMatching();
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�L�����Z���I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�L�����Z�����������A�L�����Z����������������
          �I���������s��
 *---------------------------------------------------------------------------*/
static void DWCi_FinishCancelMatching(void)
{
    GPResult gpResult;

    // GP�X�e�[�^�X���I�����C���ɂ���
    gpResult = DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);
    if (DWCi_HandleGPError(gpResult)) return;

    // �}�b�`���C�N�������N���[�Y����
    DWCi_CloseMatching();

    // �}�b�`���C�N�����R�[���o�b�N�Ăяo��
    DWCi_GetMatchCnt()->matchedCallback(DWC_ERROR_NONE,
                                 TRUE,
                                 DWCi_GetMatchCnt()->cbEventPid ? FALSE : TRUE,
                                 DWCi_GetMatchCnt()->cbEventPid ? TRUE : ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) ? TRUE : FALSE),
                                 DWCi_GetFriendListIndex(DWCi_GetMatchCnt()->cbEventPid),
                                 DWCi_GetMatchCnt()->matchedParam);

    // �L�����Z����ԏ�����
    DWCi_GetMatchCnt()->cancelState = DWC_MATCH_CANCEL_STATE_INIT;
}


/*---------------------------------------------------------------------------*
  �N���C�A���g�\�񖳌����֐�
  �����@�F�Ȃ�
  �߂�l�FSBError�^��������GPResult�^��int�^�ɕϊ������l
  �p�r�@�F�������N���C�A���g�ɗ\�񂳂�Ă���ꍇ�A���̗\����������A
          �\�񌳃N���C�A���g�ɂ��̎|��ʒm����
 *---------------------------------------------------------------------------*/
static int  DWCi_InvalidateReservation(void)
{
    u32 senddata = 0;
    int result   = 0;

    if (DWCi_GetMatchCnt()->qr2IsReserved && DWCi_GetMatchCnt()->qr2Reservation &&
        (DWCi_GetMatchCnt()->qr2Reservation != DWCi_GetMatchCnt()->profileID)){
        result =
            DWCi_SendMatchCommand(DWC_MATCH_COMMAND_CANCEL,
                                  DWCi_GetMatchCnt()->qr2Reservation,
                                  DWCi_GetMatchCnt()->qr2IPList[DWCi_GetMatchCnt()->gt2NumConnection+1],
                                  DWCi_GetMatchCnt()->qr2PortList[DWCi_GetMatchCnt()->gt2NumConnection+1],
                                  &senddata, 1);
        
        DWCi_GetMatchCnt()->qr2IsReserved  = 0;
        DWCi_GetMatchCnt()->qr2Reservation = 0;

        // send_statechanged�K�v�Ȃ��H
    }

    return result;
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�L�����Z����ăX�^�[�g�֐�
  �����@�Flevel �}�b�`���C�N�ĊJ���x���BDWCMatchResetLevel�񋓎q�Œ�`�����B
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N���L�����Z�����ꂽ�����A�L�����Z���������I��������
          �}�b�`���C�N�̍ĊJ�i�ꍇ�ɂ���Ă͏I���j�������s��
 *---------------------------------------------------------------------------*/
static void DWCi_RestartFromCancel(DWCMatchResetLevel level)
{
    SBError sbError;

    if (level == DWC_MATCH_RESET_ALL){        
        // �}�b�`���C�N���I������
        DWCi_FinishCancelMatching();
    }
    else {
        // �}�b�`���C�N����p�����[�^������
        DWCi_ResetMatchParam(level);

        if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
            (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
            // �L�����Z���Ń}�b�`���C�N�R�[���o�b�N���Ăяo���B
            // level���ǂ���̒l�ł��R�[���o�b�N�̓Z�b�g�ŁA
            // ���̌�̓��Z�b�g���ꂽ�p�����[�^�ő҂���ԂɂȂ�B
            DWCi_GetMatchCnt()->matchedCallback(DWC_ERROR_NONE,
                                         TRUE,
                                         DWCi_GetMatchCnt()->cbEventPid ? FALSE : TRUE,
                                         FALSE,
                                         DWCi_GetFriendListIndex(DWCi_GetMatchCnt()->cbEventPid),
                                         DWCi_GetMatchCnt()->matchedParam);
        }
        else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
            if (level == DWC_MATCH_RESET_RESTART){
                // SB�T�[�o�A�b�v�f�[�g���s��
                sbError = DWCi_SBUpdateAsync(0);
                if (DWCi_HandleSBError(sbError)) return;
            }
        }
        else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
            if (level == DWC_MATCH_RESET_RESTART){
                // GP���b�Z�[�W�ōŏ��̃T�[�o��NAT�l�S�V�G�[�V�����̗\����s��
                // ���� init �� TRUE �ɂ���Ɩ������[�v�Ɋׂ�\��������
                // [todo]
                // �Ȃ��������[�v�Ȃ̂����ׂāA�ł��邱�ƂȂ�init=TRUE��
                (void)DWCi_SendResvCommandToFriend(FALSE/*TRUE*/, FALSE, 0);
            }
        }
        else {
            // [test]
            // ���邱�Ƃ͂Ȃ��͂�
            DWC_Printf(DWC_REPORTFLAG_ERROR,
                       "ERROR - DWCi_RestartFromCancel : matchType %d, level %d\n",
                       DWCi_GetMatchCnt()->qr2MatchType, level);
        }
    }
}


/*---------------------------------------------------------------------------*
  �^�C���A�E�g���o��ăX�^�[�g�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�^�C���A�E�g�z�X�g���������z�X�g���A�S�R�l�N�V�������N���[�Y���A
          �ŏ�����}�b�`���C�N����蒼���B
          �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�͂��̊֐����Ă�ł͂����Ȃ��I
 *---------------------------------------------------------------------------*/
static void DWCi_RestartFromTimeout(void)
{

    // [test]
    // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�͂��̊֐����Ă�ł͂����Ȃ��I
    if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
        (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
        DWC_Printf(DWC_REPORTFLAG_WARNING,
                   "DWCi_RestartFromTimeout() shouldn't be called.\n");
        return;
    }

    // �S�ẴR�l�N�V�������N���[�Y����
    DWCi_CloseAllConnectionsByTimeout();

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "Closed all connections and restart matching.\n");

    // �ŏ�����}�b�`���C�N����蒼��
    DWCi_RestartFromCancel(DWC_MATCH_RESET_RESTART);
}


/*---------------------------------------------------------------------------*
  �R�l�N�V�����N���[�Y��}�b�`���C�N�ĊJ�֐�
  �����@�F�Ȃ�
  �߂�l�FSBError�^��GPResult�^��int�^�ɕϊ������l
  �p�r�@�F��U�T�[�o�ɐڑ����ҋ@���ƂȂ��Ă����N���C�A���g�ɂ����āA
          �}�b�`���C�N�L�����Z���ȂǂŁA�R�l�N�V���������z�X�g����N���[�Y
          ���ꂽ��A�}�b�`���C�N���ĊJ����B
          �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�͂��̊֐����Ă�ł͂����Ȃ��I
 *---------------------------------------------------------------------------*/
static int DWCi_ResumeMatching(void)
{
    SBError sbError;
    int result;

    // �\������� 
    DWCi_GetMatchCnt()->qr2IsReserved  = 0;
    DWCi_GetMatchCnt()->qr2Reservation = 0;

    // �N���[�Y��Ԃ�����������
    DWCi_GetMatchCnt()->closeState     = DWC_MATCH_CLOSE_STATE_INIT;

    if (DWCi_GetMatchCnt()->priorProfileID){
        // �N���[�Y���ߑ��M���T�[�o���爴�����ꂽ���̐ڑ��T�[�o���L���ȏꍇ�B
        // �T�[�o�N���C�A���g�}�b�`���C�N�ł�priorProfileID�̓Z�b�g����Ȃ�
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
            // �F�B���w��̏ꍇ�́A���T�[�o����Љ�ꂽ
            // �V�T�[�o��T����Ԃɖ߂�
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);

            sbError = DWCi_SBUpdateAsync(0);
            if (DWCi_HandleSBError(sbError)) return sbError;
        }
        else if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND){
            // �F�B�w��̏ꍇ�A������NN�\����s��
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);
            
            result = DWCi_SendResvCommand(DWCi_GetMatchCnt()->priorProfileID, FALSE);
            if (DWCi_HandleMatchCommandError(result)) return result;
        }
    }
    else {
        // �N���[�Y���ߑ��M���T�[�o���爴�����ꂽ���̐ڑ��T�[�o�A
        // �������͂����̐ڑ��z�X�g�������̗F�B�Ŗ��������ꍇ�B
        DWCi_RestartFromCancel(DWC_MATCH_RESET_RESTART);
    }

    return 0;
}


/*---------------------------------------------------------------------------*
  �L�����Z���z�X�g�N���[�Y�֐�
  �����@�FprofileID �N���[�Y�������z�X�g�̃v���t�@�C��ID
  �߂�l�FTRUE:�N���[�Y�����������AFALSE:�w��z�X�g�Ƃ͌��X�R�l�N�V�������Ȃ�
          �N���[�Y�����͍s���Ă��Ȃ�
  �p�r�@�F�}�b�`���C�N�L�����Z�����ɁA�L�����Z�����z�X�g�Ƃ̃R�l�N�V������
          �N���[�Y����B
 *---------------------------------------------------------------------------*/
static BOOL DWCi_CloseCancelHostAsync(int profileID)
{
    BOOL ret;
    GT2Connection* connection;

    // �L�����Z���N���C�A���g�̃v���t�@�C��ID���L�^
    DWCi_GetMatchCnt()->cbEventPid = profileID;

    // �w��̃v���t�@�C��ID�̃z�X�g�Ƃ�gt2�R�l�N�V�������擾
    connection =
        DWCi_GetGT2ConnectionByProfileID(profileID, DWCi_GetMatchCnt()->gt2NumConnection+1);

    if (connection){
        // �N���[�Y��Ԃ��^�C���A�E�g�ɂ��N���[�Y�ɂ���
        DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_TIMEOUT;
        
        // �R�l�N�V����������ꍇ�̓R�l�N�V�����̃N���[�Y���s���B
        gt2CloseConnectionHard(*connection);

        // �N���[�Y��Ԃ����ɖ߂�
        DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_INIT;
        ret = TRUE;
    }
    else {
        // �R�l�N�V�������Ȃ��ꍇ�̓��X�g���������������
        (void)DWCi_DeleteHostByProfileID(profileID, DWCi_GetMatchCnt()->gt2NumConnection+1);
        
        ret = FALSE;
    }

    return ret;
}


/*---------------------------------------------------------------------------*
  �^�C���A�E�g�z�X�g�������̃R�l�N�V�����N���[�Y�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�^�C���A�E�g�z�X�g�𔭌��������ɁA�S�Ẵz�X�g�Ƃ̃R�l�N�V������
          �N���[�Y����B
          �R�l�N�V�����N���[�Y�R�[���o�b�N���ŉ��������ɃR�l�N�V�������N���[�Y
          �������ꍇ���A���̃N���[�Y�֐����g���B
 *---------------------------------------------------------------------------*/
static void DWCi_CloseAllConnectionsByTimeout(void) 
{

    // �N���[�Y��Ԃ��^�C���A�E�g���o�ɂ��N���[�Y�ɂ���
    DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_TIMEOUT;
    
    // �S�ẴR�l�N�V������ؒf����
    gt2CloseAllConnectionsHard(*DWCi_GetMatchCnt()->pGt2Socket);

    // �N���[�Y��ԏ�����
    DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_INIT;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N���^�C���A�E�g�N���C�A���g�N���[�Y���ʒm�֐�
  �����@�Fbitmap �����̂���z�X�g��AID�r�b�g�}�b�v�i�T�[�o�͊܂܂Ȃ��j
  �߂�l�FTRUE:�����AFALSE:�G���[����
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ɂ����āA
          �^�C���A�E�g�N���C�A���g�𔭌��������ɁA�����̃N���C�A���g��
          �N���[�Y���A�ڑ��ς݃N���C�A���g�ɂ��ʒm����
 *---------------------------------------------------------------------------*/
static BOOL DWCi_CloseShutdownClientSC(u32 bitmap)
{
    u8  aid;
    int senddata[DWC_MAX_CONNECTIONS];
    int senddataLen = 0;
    int validPid[DWC_MAX_CONNECTIONS];
    int validPidLen = 0;
    int result;
    int i;

    // [note]
    // ���̂������ƁA�T�[�o���̓N���[�Y�R�[���o�b�N���Ă΂�A���̌�
    // �����������������Ă��̃R�[���o�b�N���Ԃ��Ă��邪�A�N���C�A���g����
    // �}�b�`���C�N�������������̏ꍇ�́AGT2�̃p�P�b�g�̕��������͂��̂ŁA
    // �������������̃R�[���o�b�N���Ԃ��Ă��Ă���A�N���[�Y�R�[���o�b�N��
    // �Ă΂�Ă��܂�
    
    // �܂��͖������z�X�g�̃v���t�@�C��ID���X�g���쐬����
    for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
        if (bitmap & (1 << DWCi_GetMatchCnt()->aidList[i])){
            validPid[validPidLen] = DWCi_GetMatchCnt()->sbPidList[i];
            validPidLen++;
        }
        else {
            senddata[senddataLen] = DWCi_GetMatchCnt()->sbPidList[i];
            senddataLen++;
        }
    }

    // �����̂������N���C�A���g�ɂ͖������N���C�A���g�N���[�Y�̃R�}���h�𑗐M����
    for (i = 0; i < validPidLen; i++){
        result = DWCi_SendMatchCommand(DWC_MATCH_COMMAND_SC_CLOSE_CL,
                                       validPid[i], 0, 0,
                                       (u32 *)senddata, senddataLen);
        if (DWCi_HandleMatchCommandError(result)) return FALSE;
    }

    // �}�b�`���C�N���̃N���C�A���g���N���[�Y���Ă��������Ȃ��悤��
    // �N���[�Y��Ԃ��Z�b�g����
    DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_TIMEOUT;

    // �������z�X�g���N���[�Y����
    for (i = 0; i < senddataLen; i++){
        aid = DWCi_GetAIDFromProfileID(senddata[i], FALSE);
        if (aid != 0xff){
            // �R�l�N�V�������N���[�Y����
            DWC_CloseConnectionHard(aid);
        }
    }

    // �N���[�Y��Ԃ��N���A����
    DWCi_GetMatchCnt()->closeState = DWC_MATCH_CLOSE_STATE_INIT;

    return TRUE;
}

/*---------------------------------------------------------------------------*
  �}�b�`���C�N�������������p�P�b�g���M�֐�
  �����@�Faid  ���M��z�X�g��AID
          type DWC Transport�p�P�b�g�^�C�v�BDWC_SEND_TYPE_*�Œ�`�����
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�������������̃p�P�b�g�𑗐M����
 *---------------------------------------------------------------------------*/
static void DWCi_SendMatchSynPacket(u8 aid, u16 type)
{
    u8  i;
    u8  senddata[4];

    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Sent SYN %d packet to aid %d.\n",
               type-DWC_SEND_TYPE_MATCH_SYN, aid);

    switch (type){
    case DWC_SEND_TYPE_MATCH_SYN:
        if (aid == DWCi_GetMatchCnt()->aidList[DWCi_GetMatchCnt()->gt2NumConnection]){
            // �V�K�ڑ��N���C�A���g�ɑ��鎞�̓t���O�𗧂Ă�
            senddata[0] = 1;
        }
        else {
            senddata[0] = 0;
        }

        for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
            if (aid == DWCi_GetMatchCnt()->aidList[i]){
                // �R�}���hCLS_REQ�����������ׂ̈ɁA�K��AID�C���f�b�N�X��
                // AID���`����B
                // ���݂̎d�l�ł͕K��AID�C���f�b�N�X��AID�B
                senddata[1] = i;
                senddata[2] = aid;
                break;
            }
        }
        break;

    case DWC_SEND_TYPE_MATCH_SYN_ACK:
        // �T�[�o�Ƃ̃��C�e���V��t�����đ��M����
        senddata[0] = (u8)(DWCi_GetMatchCnt()->baseLatency & 0xff);
        senddata[1] = (u8)((DWCi_GetMatchCnt()->baseLatency >> 8) & 0xff);
        break;
    }
    // DWC_SEND_TYPE_MATCH_ACK�̎��͕t���f�[�^�͂Ȃ�

    DWCi_SendReliable(type, aid, senddata, DWC_MATCH_SYN_DATA_BODY_SIZE);

    // ���M���Ԃ��L�^
    DWCi_GetMatchCnt()->lastSynSent = OS_GetTick();
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�������������^�C���A�E�g�����֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:���퓮��AFALSE:�G���[����
  �p�r�@�F�}�b�`���C�N�������������̃^�C���A�E�g���`�F�b�N���A
          �^�C���A�E�g�ɒB���Ă�����đ��������s��
 *---------------------------------------------------------------------------*/
static BOOL DWCi_ProcessMatchSynTimeout(void)
{
    u64 passTime;
    int i;

    if ((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_SYN) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_SYN) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_SYN_WAIT))
        passTime = OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->lastSynSent);
    else return TRUE;

    switch (DWCi_GetMatchCnt()->state){
    case DWC_MATCH_STATE_CL_SYN:
        if (passTime > DWC_MATCH_SYN_ACK_WAIT_TIME){
            // �N���C�A���g��ACK�҂���Ԃ̎��Ɉ�莞�Ԃ��o�߂����ꍇ�A
            // �T�[�o��SYN-ACK���đ�����
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "[SYN] No ACK from server %d/%d.\n",
                       DWCi_GetMatchCnt()->clWaitTimeoutCount, DWC_MATCH_CMD_RETRY_MAX);
            if ((DWC_GetState() == DWC_STATE_MATCHING) &&
                (DWCi_GetMatchCnt()->clWaitTimeoutCount >= DWC_MATCH_CMD_RETRY_MAX)){
                // 5�񑗂��Ă��T�[�o����ԓ����Ȃ������ꍇ�̓}�b�`���C�N����蒼���B
                // �S�ẴN���C�A���g�������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̐V�K�ڑ��N���C�A���g�̏ꍇ��
                // �G���[�I������B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̐ڑ��ς݃N���C�A���g�̏ꍇ��
                // �ڑ�������̃^�C���A�E�g�����̓Q�[�����Ɉς˂���B
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                           "Timeout: [SYN] Connection to server was shut down.\n");
                    
                if (!DWCi_CancelPreConnectedClientProcess(DWCi_GetMatchCnt()->sbPidList[0]))
                    return FALSE;
            }
            else {
                DWCi_GetMatchCnt()->clWaitTimeoutCount++;

                // �T�[�o��SYN-ACK���đ�����
                DWCi_SendMatchSynPacket(DWCi_GetMatchCnt()->aidList[0],
                                        DWC_SEND_TYPE_MATCH_SYN_ACK);

            }
        }
        break;

    case DWC_MATCH_STATE_SV_SYN:
        if (passTime > DWC_MATCH_SYN_ACK_WAIT_TIME){
            // SYN-ACK��M�҂��^�C���A�E�g���Ԍo��
            DWCi_GetMatchCnt()->synResendCount++;
            if (DWCi_GetMatchCnt()->synResendCount > DWC_MATCH_CMD_RETRY_MAX){
                // ���񐔍đ����Ă��Ԏ����Ȃ������ꍇ�́A���̃N���C�A���g��
                // �ʐM��ؒf�����Ɣ��f����
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Timeout: wait SYN-ACK (aidbitmap 0x%x). Restart matching.\n",
                           DWCi_GetMatchCnt()->synAckBit);
                    
                if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
                    (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
                    // �F�B�w��A���w��s�A�}�b�`���C�N�ł́A�S�ẴR�l�N�V������
                    // �ؒf���A�}�b�`���C�N���ŏ������蒼��
                    DWCi_CloseAllConnectionsByTimeout();
                    DWCi_RestartFromCancel(DWC_MATCH_RESET_RESTART);
                }
                else {
                    // �T�[�o�N���C�A���g�}�b�`���C�N�ł́A����������
                    // �N���C�A���g�݂̂�ؒf���A���̃N���C�A���g�ɂ������
                    // �`���Ă��̃N���C�A���g��ؒf������B
                    if (!DWCi_CloseShutdownClientSC(DWCi_GetMatchCnt()->synAckBit))
                        return FALSE;

                    if (DWCi_GetMatchCnt()->gt2NumConnection != 0){
                        // �܂��ڑ����Ă���N���C�A���g���c���Ă���ꍇ��
                        // ���g���C�J�E���g��SYN�p�P�b�g���M���Ԃ����������A
                        // �Ăѓ������������𑱂���
                        DWCi_GetMatchCnt()->synResendCount = 0;
                        DWCi_GetMatchCnt()->lastSynSent    = OS_GetTick();
                    }
                    else {
                        // �ڑ��ς݃N���C�A���g�S�������Ȃ��Ȃ����ꍇ��
                        // �S�ẴT�[�o�������I�����ă}�b�`���C�N�J�n��Ԃɖ߂�
                        if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->cbEventPid))
                            return FALSE;
                    }
                }
            }
            else {
                // SYN-ACK��Ԃ��Ă��Ȃ��N���C�A���g�ɂ̂�SYN�p�P�b�g���đ�����
                for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                    if (!(DWCi_GetMatchCnt()->synAckBit & (1 << DWCi_GetMatchCnt()->aidList[i]))){
                        DWCi_SendMatchSynPacket(DWCi_GetMatchCnt()->aidList[i],
                                                DWC_SEND_TYPE_MATCH_SYN);
                    }
                }
            }
        }
        break;

    case DWC_MATCH_STATE_SV_SYN_WAIT:
        if (passTime > DWCi_GetMatchCnt()->baseLatency){
            // ACK���M����ő僌�C�e���V���Ԍo�߂�����}�b�`���C�N���������Ăяo��
            DWCi_PostProcessConnection(DWC_PP_CONNECTION_SYN_FINISH);
        }
        break;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������R�}���h���M�֐�
  �����@�FprofileID ���M��z�X�g�̃v���t�@�C��ID
          command   ���M���铯�������R�}���h
  �߂�l�FTRUE:���M�����AFALSE:�G���[����
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������R�}���h�𑗐M����B
          �G���[�����͓����ōs���Ă���B
 *---------------------------------------------------------------------------*/
static BOOL DWCi_SendCancelMatchSynCommand(int profileID, u8 command)
{
    u32 senddata;
    int len;
    int result;

    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Sent CANCEL SYN %d command to %u.\n",
               command-DWC_MATCH_COMMAND_CANCEL_SYN, profileID);

    if (command == DWC_MATCH_COMMAND_CANCEL_SYN){
        // �L�����Z��SYN���M���ɃL�����Z���N���C�A���g�̃v���t�@�C��ID�𑗂�
        senddata = (u32)DWCi_GetMatchCnt()->cbEventPid;
        len      = 1;
    }
    else {
        len      = 0;
    }

    result = DWCi_SendMatchCommand(command, profileID, 0, 0, &senddata, len);
    if (DWCi_HandleMatchCommandError(result)) return FALSE;

    // ���M���Ԃ��L�^
    DWCi_GetMatchCnt()->lastCancelSynSent = OS_GetTick();

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������R�}���h��M�����֐�
  �����@�FprofileID ���M���z�X�g�̃v���t�@�C��ID
          command   ��M�������������R�}���h
          data      �R�}���h�ɕt������u32�f�[�^
  �߂�l�FTRUE:����I���AFALSE:�G���[����
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������R�}���h��M����
          �������s��
 *---------------------------------------------------------------------------*/
static BOOL DWCi_ProcessCancelMatchSynCommand(int profileID, u8 command, u32 data)
{
    u8  aid;
    u32 bitmask;
    u64 baseTime, latency;
    int i;

    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Received CANCEL SYN %d command from %u.\n",
               command-DWC_MATCH_COMMAND_CANCEL_SYN, profileID);

    if (DWC_GetState() != DWC_STATE_CONNECTED){
        // ���ɃN���[�Y������Ɏ�M�����ꍇ�͖�������
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Ignore delayed CANCEL SYN.\n");
        return TRUE;
    }

    switch (command){
    case DWC_MATCH_COMMAND_CANCEL_SYN:
        if (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_CANCEL_SYN){
            // �ŏ��̎�M���Ȃ�}�b�`���C�N�L�����Z��������Ԃɐi��
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_CANCEL_SYN);

            // �V�K�ڑ��N���C�A���g���N���[�Y����B
            // data�ɐV�K�ڑ��N���C�A���g�̃v���t�@�C��ID�������Ă���B
            DWCi_CloseCancelHostAsync((int)data);
        }

        // �T�[�o��SYN-ACK�p�P�b�g�𑗐M����
        if (!DWCi_SendCancelMatchSynCommand(profileID, DWC_MATCH_COMMAND_CANCEL_SYN_ACK))
            return FALSE;
        break;

    case DWC_MATCH_COMMAND_CANCEL_SYN_ACK:
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_CANCEL_SYN){
            // SYN-ACK�҂���Ԃ̏ꍇ�B
            // SYN�𑗂��Ă���Ԃ��Ă���܂ł̎��Ԃ��炨���悻�̕Г���
            // ���C�e���V�����߁A�ő�ł���΋L�^����B
            // �đ�����ɑO��SYN-ACK���Ԃ��Ă����ꍇ�͒Z�߂ɂȂ邪�A
            // �Z���Ȃ镪�ɂ͍\��Ȃ��B
            baseTime = OS_GetTick();
            if ((OS_TicksToMilliSeconds(baseTime-DWCi_GetMatchCnt()->lastCancelSynSent) >> 1) > DWC_GP_PROCESS_INTERVAL){
                // �N���C�A���g���ł̃R�}���h��M����R�}���h���M�܂ł�
                // �ő�z�莞�Ԃ������Ă���
                latency = (OS_TicksToMilliSeconds(baseTime-DWCi_GetMatchCnt()->lastCancelSynSent) >> 1)-DWC_GP_PROCESS_INTERVAL;
                if (latency > DWCi_GetMatchCnt()->cancelBaseLatency){
                    DWCi_GetMatchCnt()->cancelBaseLatency = (u16)latency;
                }
            }
                
            // SYN-ACK��MBit�Z�b�g
            // AID�r�b�g�}�b�v�ŋL�^����
            aid = DWCi_GetAIDFromProfileID(profileID, FALSE);
            if (aid != 0xff){
                DWCi_GetMatchCnt()->cancelSynAckBit |= 1 << aid;
            }

            bitmask = DWCi_GetAIDBitmask(TRUE);  // ����pAID�r�b�g�}�X�N���擾
            if (DWCi_GetMatchCnt()->cancelSynAckBit == bitmask){
                // �S�N���C�A���g����SYN-ACK���Ԃ��Ă�����A�S�N���C�A���g��
                // ACK�𑗐M����
                for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                    if (!DWCi_SendCancelMatchSynCommand(DWCi_GetMatchCnt()->sbPidList[i],
                                                        DWC_MATCH_COMMAND_CANCEL_ACK))
                        return FALSE;
                }

                // �}�b�`���C�N�L�����Z�����������҂���Ԃɐi��
                DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT);

                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                           "Wait max latency %d msec.\n",
                           DWCi_GetMatchCnt()->cancelBaseLatency);
            }
        }
        else {
            // ACK�𑗐M������A�ʂɒx��Ă���SYN-ACK�ɑΉ�����ꍇ
            // ������ACK��ԐM����
            if (!DWCi_SendCancelMatchSynCommand(profileID, DWC_MATCH_COMMAND_CANCEL_ACK))
                return FALSE;
        }
        break;

    case DWC_MATCH_COMMAND_CANCEL_ACK:
        if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_CANCEL_SYN){            
            // �N���C�A���g�}�b�`���C�N�ĊJ
            DWCi_RestartFromCancel(DWC_MATCH_RESET_CONTINUE);
        }
        break;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������^�C���A�E�g�����֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:����I���AFALSE:�G���[����
  �p�r�@�F�T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�����������̃^�C���A�E�g��
          �`�F�b�N���A�^�C���A�E�g�ɒB���Ă�����đ��������s��
 *---------------------------------------------------------------------------*/
static BOOL DWCi_ProcessCancelMatchSynTimeout(void)
{
    u64 passTime;
    int i;

    if ((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_CANCEL_SYN) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_CANCEL_SYN) ||
        (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT))
        passTime = OS_TicksToMilliSeconds(OS_GetTick()-DWCi_GetMatchCnt()->lastCancelSynSent);
    else return TRUE;

    switch (DWCi_GetMatchCnt()->state){
    case DWC_MATCH_STATE_CL_CANCEL_SYN:
        if (passTime > DWC_MATCH_CANCEL_SYN_ACK_WAIT_TIME){
            // �T�[�o�ɃL�����Z��SYN-ACK���đ�����
            if (!DWCi_SendCancelMatchSynCommand(DWCi_GetMatchCnt()->sbPidList[0],
                                                DWC_MATCH_COMMAND_CANCEL_SYN_ACK))
                return FALSE;
        }
        break;

    case DWC_MATCH_STATE_SV_CANCEL_SYN:
        if (passTime > DWC_MATCH_CANCEL_SYN_ACK_WAIT_TIME){
            // SYN-ACK��M�҂��^�C���A�E�g���Ԍo��
            DWCi_GetMatchCnt()->cancelSynResendCount++;
            if (DWCi_GetMatchCnt()->cancelSynResendCount > DWC_MATCH_CMD_RETRY_MAX){
                // ���񐔍đ����Ă��Ԏ����Ȃ������ꍇ�́A���̃N���C�A���g��
                // �ʐM��ؒf�����Ɣ��f����
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Timeout: wait cancel SYN-ACK (aidbitmap 0x%x).\n",
                           DWCi_GetMatchCnt()->cancelSynAckBit);

                // �T�[�o�N���C�A���g�}�b�`���C�N�ł́A����������
                // �N���C�A���g�݂̂�ؒf���A���̃N���C�A���g�ɂ������
                // �`���Ă��̃N���C�A���g��ؒf������B
                if (!DWCi_CloseShutdownClientSC(DWCi_GetMatchCnt()->cancelSynAckBit))
                    return FALSE;

                if (DWCi_GetMatchCnt()->gt2NumConnection != 0){
                    // �܂��ڑ����Ă���N���C�A���g���c���Ă���ꍇ��
                    // ���g���C�J�E���g��SYN�p�P�b�g���M���Ԃ����������A
                    // �Ăѓ������������𑱂���
                    DWCi_GetMatchCnt()->cancelSynResendCount = 0;
                    DWCi_GetMatchCnt()->lastCancelSynSent    = OS_GetTick();
                }
                else {
                    // �ڑ��ς݃N���C�A���g�S�������Ȃ��Ȃ����ꍇ��
                    // ���������}�b�`���C�N��Ԃɂ���
                    DWCi_RestartFromCancel(DWC_MATCH_RESET_CONTINUE);
                }
            }
            else {
                // SYN-ACK��Ԃ��Ă��Ȃ��N���C�A���g�ɂ̂�SYN�p�P�b�g���đ�����
                for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                    if (!(DWCi_GetMatchCnt()->cancelSynAckBit & (1 << DWCi_GetMatchCnt()->aidList[i]))){
                        if (!DWCi_SendCancelMatchSynCommand(DWCi_GetMatchCnt()->sbPidList[i],
                                                            DWC_MATCH_COMMAND_CANCEL_SYN))
                            return FALSE;
                    }
                }
            }
        }
        break;

    case DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT:
        if (passTime > DWCi_GetMatchCnt()->cancelBaseLatency){
            // �L�����Z��ACK���M����ő僌�C�e���V���Ԍo�߂�����}�b�`���C�N�ĊJ
            DWCi_RestartFromCancel(DWC_MATCH_RESET_CONTINUE);
        }
        break;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  ��QR2�L�[ID�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F���p�\�ȃQ�[����`QR2�L�[ID�iDWC_QR2_GAME_KEY_START�ȍ~�̒l�j
  �p�r�@�FQR2�p�L�[�f�[�^�z����������A���p�\��QR2�L�[ID���擾����
 *---------------------------------------------------------------------------*/
static u8  DWCi_GetNewMatchKey(void)
{
    u8 i;

    // �󂢂Ă���L�[�ԍ���T��
    for (i = 0; i < DWC_QR2_GAME_RESERVED_KEYS; i++){
        if (!stGameMatchKeys[i].keyID) return (u8)(DWC_QR2_GAME_KEY_START+i);
    }

    // ���ɓo�^�L�[���������ς�
    return 0;
}


/*---------------------------------------------------------------------------*
  �Q�[���pQR2�L�[�N���A�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�p�L�[�f�[�^�z����N���A����
 *---------------------------------------------------------------------------*/
static void DWCi_ClearGameMatchKeys(void)
{
    int i;

    // �܂��S�ẴL�[�𒲂ׂ�ALLOC�ςݏ�Ԃ̂��̂��������
    for (i = 0; i < DWC_QR2_GAME_RESERVED_KEYS; i++){
        if (stGameMatchKeys[i].keyStr)
            DWC_Free(DWC_ALLOCTYPE_BASE, stGameMatchKeys[i].keyStr, 0);
    }

    // �S�ă[���N���A����
    MI_CpuClear32(stGameMatchKeys, sizeof(DWCGameMatchKeyData)*DWC_QR2_GAME_RESERVED_KEYS);
}

/*---------------------------------------------------------------------------*
  ��AID�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F���p�\�ȍŏ����l��AID
  �p�r�@�FAID���X�g���������A���p�\��AID�̍ŏ��l���擾����
 *---------------------------------------------------------------------------*/
static u8   DWCi_GetAIDFromList(void)
{
    u8  i;
    int j;

    for (i = 0; i < DWC_MAX_CONNECTIONS; i++){
        for (j = 0; j <= DWCi_GetMatchCnt()->qr2NNFinishCount; j++){
            if (DWCi_GetMatchCnt()->aidList[j] == i) break;
        }
        if (j > DWCi_GetMatchCnt()->qr2NNFinishCount) break;
    }

    return i;
}


/*---------------------------------------------------------------------------*
  �F�B����֐��i�F�B�C���f�b�N�X���X�g�g�p�Łj
  �����@�FprofileID �v���t�@�C��ID
  �߂�l�FTRUE:�C���f�b�N�X���X�g�ɂ���F�B�ł���AFALSE:�F�B�ł͂Ȃ�
  �p�r�@�F�Q�[������o�^���ꂽ�F�B�C���f�b�N�X���X�g�ƗF�B���X�g���Q�Ƃ��A
          �w��̃v���t�@�C��ID���F�B�C���f�b�N�X���X�g�ɓo�^���ꂽ
          �v���t�@�C��ID���𒲂ׂ�
 *---------------------------------------------------------------------------*/
static BOOL DWCi_IsFriendByIdxList(int profileID)
{
    int listProfileID;
    int i;

    if (DWCi_GetMatchCnt()->friendList == NULL) return FALSE;

    for (i = 0; i < DWCi_GetMatchCnt()->friendIdxListLen; i++){
        if (((listProfileID = DWCi_GetProfileIDFromList(DWCi_GetMatchCnt()->friendIdxList[i])) > 0) &&
            (listProfileID == profileID)){
            // �F�B���X�g�Ƀv���t�@�C��ID�����݂��Ă���ΗF�B�Ɣ��肷��B
            // ���̊֐��̎g�������炵�āAgpIsBuddy()�Œ��ׂ�K�v�͂Ȃ��B
            return TRUE;
        }
    }

    return FALSE;
}


/*---------------------------------------------------------------------------*
  DWC���C�u�����g�p�T�[�o�`�F�b�N�֐�
  �����@�Fserver SBServer�^
  �߂�l�Fserver������ dwc_pid �L�[�̒l�i�v���t�@�C��ID�j��Ԃ��B
          DWC���C�u�����g�p�T�[�o�łȂ����0��Ԃ�
  �p�r�@�FSBServer�I�u�W�F�N�g�𒲂ׁADWC���C�u�������ݒ肷��L�[�����邩
          �ǂ������肵�A����� dwc_pid �L�[�̒l�i�v���t�@�C��ID�j��Ԃ��A
          �������0��Ԃ��B
 *---------------------------------------------------------------------------*/
static int  DWCi_CheckDWCServer(SBServer server)
{

    if (SBServerGetIntValue(server, "numplayers", -1) == -1) return 0;
    if (SBServerGetIntValue(server, "maxplayers", -1) == -1) return 0;
    if (SBServerGetIntValue(server, DWC_QR2_MATCH_TYPE_KEY_STR, -1) == -1) return 0;
    if (SBServerGetIntValue(server, DWC_QR2_MATCH_RESV_KEY_STR, -1) == -1){
        if (SBServerGetIntValue(server, DWC_QR2_MATCH_RESV_KEY_STR, 0) == 0) return 0;
    }
    if (SBServerGetIntValue(server, DWC_QR2_MATCH_VER_KEY_STR, -1) == -1) return 0;
    return SBServerGetIntValue(server, DWC_QR2_PID_KEY_STR, 0);
}


/*---------------------------------------------------------------------------*
  sbPidList�C���f�b�N�X�擾�֐�
  �����@�FprofileID �C���f�b�N�X��m�肽���z�X�g�̃v���t�@�C��ID
          idx0      TRUE:��������C���f�b�N�X��0���܂߂�AFALSE:0�͕s��Ȃ̂Ŋ܂߂Ȃ�
  �߂�l�FsbPidList�����X�g�̃C���f�b�N�X�B���X�g�ɂȂ��v���t�@�C��ID�ł����
          -1��Ԃ�
  �p�r�@�F�w��v���t�@�C��ID�̃z�X�g�́AsbPidList�C���f�b�N�X���擾����
 *---------------------------------------------------------------------------*/
static int  DWCi_GetPidListIndex(int profileID, BOOL idx0)
{
    int i;

    for (i = (idx0 ? 0 : 1); i <= DWCi_GetMatchCnt()->qr2NNFinishCount; i++){
        if (DWCi_GetMatchCnt()->sbPidList[i] == profileID) return i;
    }

    return -1;
}


/*---------------------------------------------------------------------------*
  �v���t�@�C��ID����AID�擾�֐�
  �����@�FprofileID AID��m�肽���z�X�g�̃v���t�@�C��ID
          idx0      TRUE:��������C���f�b�N�X��0���܂߂�AFALSE:0�͕s��Ȃ̂Ŋ܂߂Ȃ�
  �߂�l�FAID�B�Ή�����AID���Ȃ��ꍇ��0xff��Ԃ��B
  �p�r�@�F�w��v���t�@�C��ID�̃z�X�g�́AAID���擾����
 *---------------------------------------------------------------------------*/
static u8   DWCi_GetAIDFromProfileID(int profileID, BOOL idx0)
{
    int i;

    for (i = (idx0 ? 0 : 1); i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
        if (DWCi_GetMatchCnt()->sbPidList[i] == profileID)
            return DWCi_GetMatchCnt()->aidList[i];
    }

    return 0xff;
}


/*---------------------------------------------------------------------------*
  ��������������pAID�r�b�g�}�b�v�擾�֐�
  �����@�Fvalid TRUE: �ڑ������ς݂̃z�X�g�݂̂̃r�b�g�}�b�v
                     �i�T�[�o�N���C�A���g�}�b�`���C�N�L�����Z�������Ŏg�p�j
                FALSE:�}�b�`���C�N�����܂߂��ڑ��z�X�g�̃r�b�g�}�b�v
                      �i�}�b�`���C�N���������Ɏg�p�j
  �߂�l�FAID�r�b�g�}�b�v
  �p�r�@�F�e�퓯���������ɁASYN-ACK��M�z�X�g�𒲂ׂ�ׂ̃r�b�g�}�b�v���擾����
 *---------------------------------------------------------------------------*/
static u32  DWCi_GetAIDBitmask(BOOL valid)
{
    u32 bitmask = 0;
    int i;

    if (valid){
        // ���ɐڑ������ς݂�AID����A�T�[�o��aid�i=0�j���������r�b�g�}�X�N
        return (DWCi_GetMatchCnt()->validAidBitmap & ~1);
    }
    else {
        // �}�b�`���C�N���̃z�X�g���܂߂��ڑ��ς݂�AID����A�T�[�o��aid�i=0�j��
        // �������r�b�g�}�X�N
        for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
            bitmask |= 1 << DWCi_GetMatchCnt()->aidList[i];
        }
        return bitmask;
    }
}


/*---------------------------------------------------------------------------*
  �ڑ��ς݃N���C�A���g�^�C���A�E�g�J�E���^�������֐�
  �����@�Ftimeout �^�C���A�E�g�Ɣ��肷��T�[�o�Ƃ̖��ʐM����
  �߂�l�F�Ȃ�
  �p�r�@�F�ڑ��ς݃N���C�A���g���A�T�[�o����̃f�[�^��M���Ȃ����Ԃ��v������
          ���߂̍ŏI�f�[�^��M���ԂƁA�^�C���A�E�g�����񐔃J�E���^������������
 *---------------------------------------------------------------------------*/
static void DWCi_InitClWaitTimeout(void)
{

    DWCi_GetMatchCnt()->clWaitTimeoutCount = 0;
    DWCi_GetMatchCnt()->clWaitTime         = OS_GetTick();
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I�v�V�����@�w��l���ȉ��}�b�`���C�N�����J�E���^�������֐�
  �����@�Freset TRUE:�T�[�o�ɂȂ������̃��Z�b�g�AFALSE:���S������
  �߂�l�F�Ȃ�
  �p�r�@�F�w��l���ȉ��}�b�`���C�N�����̃}�b�`���C�N�I�v�V�����́A
          �^�C���A�E�g�v���p�J�E���^�������֐�
 *---------------------------------------------------------------------------*/
static void DWCi_InitOptMinCompParam(BOOL reset)
{

    if (stpOptMinComp && stpOptMinComp->valid){
        stpOptMinComp->recvBit      = 0;
        stpOptMinComp->timeoutBit   = 0;
        stpOptMinComp->retry        = 0;
        stpOptMinComp->lastPollTime = OS_GetTick();
        if (!reset) stpOptMinComp->startTime = OS_GetTick();
    }
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�I�v�V�����@�w��l���ȉ��}�b�`���C�N�������t���[�������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�w��l���ȉ��}�b�`���C�N�����̃}�b�`���C�N�I�v�V�����́A�^�C���A�E�g
          �`�F�b�N�A�y�уN���C�A���g�ւ̃|�[�����O�������s��
 *---------------------------------------------------------------------------*/
static void DWCi_ProcessOptMinComp(void)
{
    u32 bitmask;
    int result;
    int i;

    if (!stpOptMinComp || !stpOptMinComp->valid ||
        (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
        (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL))
        return;

    if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_POLL_TIMEOUT){
        bitmask = DWCi_GetAIDBitmask(FALSE);  // ����pAID�r�b�g�}�X�N���擾
        if (stpOptMinComp->recvBit == bitmask){
            // �S������ԐM���������ꍇ
            if (stpOptMinComp->timeoutBit == bitmask){
                // �N���C�A���g���S���^�C���A�E�g���Ԃ��z���đ҂��Ă����ꍇ
                // �}�b�`���C�N�����������s���B
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                           "[OPT_MIN_COMP] Timeout occured in all hosts.\n");
                    
                // ���X�̐ڑ���]�l�����A���݂̐ڑ��l���ɍ��킹�ĕύX����B
                DWCi_GetMatchCnt()->qr2NumEntry = (u8)DWCi_GetMatchCnt()->gt2NumConnection;
                // �I�������ɐi�ވׂ̃p�����[�^����
                DWCi_GetMatchCnt()->clLinkProgress = (u8)(DWCi_GetMatchCnt()->gt2NumConnection-1);

                // �}�b�`���C�N�I�������Ăяo��
                DWCi_PostProcessConnection(DWC_PP_CONNECTION_SV_CONNECT);
            }
            else {
                // �܂��^�C���A�E�g�ɒB���Ă��Ȃ��N���C�A���g�������ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                           "[OPT_MIN_COMP] Some clients is in time.\n");

                // �đ��Ԋu�̌v�����J�n����
                stpOptMinComp->lastPollTime = OS_GetTick();
                stpOptMinComp->recvBit      = 0;  // ��M�r�b�g�N���A

                if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
                    // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�́A�󂫃z�X�g�̌�����Ԃɖ߂�
                    DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);
//                    DWCi_GetMatchCnt()->sbUpdateCount = 1;
                    DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_LONG;
                    DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
                }
                else {
                    // �F�B�w��s�A�}�b�`���C�N�̏ꍇ�́A���t���[�����
                    // NN�\��v�����M���s��
                    DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);
                    (void)DWCi_SendResvCommandToFriend(TRUE, FALSE, 0);
                }
            }
        }
        else if (OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->lastPollTime) >= DWC_MATCH_CMD_RTT_TIMEOUT*stpOptMinComp->retry){
            // �^�C���A�E�g���Ԃ��߂��Ă��N���C�A���g����̕ԐM������Ȃ��ꍇ
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "[OPT_MIN_COMP] Timeout: wait poll-ACK %d/%d.\n",
                       stpOptMinComp->retry-1, DWC_MATCH_OPT_MIN_COMP_POLL_RETRY_MAX);
            
            if (stpOptMinComp->retry > DWC_MATCH_OPT_MIN_COMP_POLL_RETRY_MAX){
                // �ő僊�g���C�񐔃|�[�����O���Ă��N���C�A���g����̔������Ȃ�
                // �ꍇ�́A���̃N���C�A���g�͐ؒf���ꂽ���̂Ɣ��f���A
                // �}�b�`���C�N����蒼��
                DWC_Printf(DWC_REPORTFLAG_DEBUG, 
                           "[OPT_MIN_COMP] Timeout: aidbitmap 0x%x. Restart matching.\n",
                           stpOptMinComp->recvBit);
                
                // ����p�����[�^������
                DWCi_InitOptMinCompParam(TRUE);

                // �S�ẴR�l�N�V������ؒf���A�}�b�`���C�N���ŏ������蒼��
                DWCi_CloseAllConnectionsByTimeout();
                DWCi_RestartFromCancel(DWC_MATCH_RESET_RESTART);
            }
            else {
                // �܂��ԐM���Ă��Ȃ��N���C�A���g�ɃR�}���h���đ�����
                for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
                    if (!(stpOptMinComp->recvBit & (1 << DWCi_GetMatchCnt()->aidList[i]))){
                        result =
                            DWCi_SendMatchCommand(DWC_MATCH_COMMAND_POLL_TIMEOUT,
                                                  DWCi_GetMatchCnt()->sbPidList[i],
                                                  DWCi_GetMatchCnt()->qr2IPList[i],
                                                  DWCi_GetMatchCnt()->qr2PortList[i],
                                                  NULL, 0);
                        if (DWCi_HandleMatchCommandError(result)) return;
                    }
                }
                stpOptMinComp->retry++;
            }
        }
    }
    else if (((DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAIT_RESV) ||
              (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_SEARCH_HOST)) &&
             (DWCi_GetMatchCnt()->gt2NumConnection >= stpOptMinComp->minEntry-1) &&
             ((!stpOptMinComp->retry &&
               (OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->startTime) >= stpOptMinComp->timeout)) ||
              (stpOptMinComp->retry &&
               (OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->lastPollTime) >= stpOptMinComp->timeout >> 2)))){
        // �T�[�o���^�C���A�E�g���Ԃ��z���Ă���A�Œ�}�b�`���C�N
        // �����l�����ڑ�����Ă�����A�N���C�A���g���^�C���A�E�g���Ԃ�
        // �z���Ă��Ȃ������|�[�����O����B
        // �Q�x�ڈȍ~�̃|�[�����O�́A�^�C���A�E�g���Ԃ�1/4�̎��ԊԊu��
        // ����I�ɍs���B

        // �܂��͗\�񑗐M�ς݂Ȃ�\����L�����Z������
        if (DWCi_GetMatchCnt()->reqProfileID){
            result = DWCi_CancelReservation(DWCi_GetMatchCnt()->reqProfileID);
            if (DWCi_HandleMatchCommandError(result)) return;
        }

        // �|�[�����O��Ԃɐi��
        DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_POLL_TIMEOUT);

        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                   "[OPT_MIN_COMP] Poll timeout (my time is %lu).\n",
                   OS_TicksToMilliSeconds(OS_GetTick()-stpOptMinComp->startTime));

        // �|�[�����O�p�P�b�g�𑗐M����
        for (i = 1; i <= DWCi_GetMatchCnt()->gt2NumConnection; i++){
            result =
                DWCi_SendMatchCommand(DWC_MATCH_COMMAND_POLL_TIMEOUT,
                                      DWCi_GetMatchCnt()->sbPidList[i],
                                      DWCi_GetMatchCnt()->qr2IPList[i],
                                      DWCi_GetMatchCnt()->qr2PortList[i],
                                      NULL, 0);
            if (DWCi_HandleMatchCommandError(result)) return;
        }

        // �S�ẴN���C�A���g����̃f�[�^���A���Ă���܂ł̎��Ԃ�
        // �J�E���g���邽�߂�lastPollTime�����ݎ��Ԃŏ㏑������
        stpOptMinComp->lastPollTime = OS_GetTick();
        stpOptMinComp->retry        = 1;  // �P�񑗐M�������Ƃ��L�^
    }
}


/*---------------------------------------------------------------------------*
  GPResult�Ή��G���[�����֐�
  �����@�Fresult GP�̏������ʌ^
  �߂�l�FGP�̏������ʌ^�i���������̂܂ܕԂ��j
  �p�r�@�FGPResult�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���āA�Ή�����R�[���o�b�N���Ăяo��
 *---------------------------------------------------------------------------*/
static GPResult DWCi_HandleGPError(GPResult result)
{
    int errorCode;
    DWCError dwcError;

    if (result == GP_NO_ERROR) return GP_NO_ERROR;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, GP error %d\n", result);

    switch (result){
    case GP_MEMORY_ERROR:
        dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_ALLOC;
        break;
    case GP_PARAMETER_ERROR:
        dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_PARAM;
        break;
    case GP_NETWORK_ERROR:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_NETWORK;
        break;
    case GP_SERVER_ERROR:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SERVER;
        break;
    }

    // �x�[�X�G���[�R�[�h��ǉ�
    errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_GP;

    // �}�b�`���C�N���f�֐����Ăяo��
    DWCi_StopMatching(dwcError, errorCode);

    return result;
}


/*---------------------------------------------------------------------------*
  SBError�Ή��G���[�����֐�
  �����@�Ferror SB�̃G���[�^
  �߂�l�FSB�̃G���[�^�i���������̂܂ܕԂ��j
  �p�r�@�FSBError�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���ă}�b�`���C�N�葱�����I������
 *---------------------------------------------------------------------------*/
static SBError DWCi_HandleSBError(SBError error)
{
    int errorCode;
    DWCError dwcError;

    if (error == sbe_noerror) return sbe_noerror;

#if 0
    // [sasakit] ignore connecterror
    if (error == sbe_connecterror)
    {
        DWCi_connect_errorcount++;
        DWC_Printf(DWC_REPORTFLAG_ERROR, "********************************%d\n", DWCi_connect_errorcount);
        return sbe_noerror;
    }
#endif

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, SB error %d\n", error);

    switch (error){
    case sbe_socketerror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SOCKET;
        break;
    case sbe_dnserror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DNS;
        break;
    case sbe_connecterror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SERVER;
        break;
    case sbe_dataerror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DATA;
        break;
    case sbe_allocerror:
        dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_ALLOC;
        break;
    case sbe_paramerror:
        dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_PARAM;
        break;
    }

    // �x�[�X�G���[�R�[�h��ǉ�
    errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_SB;

    DWCi_StopMatching(dwcError, errorCode);
    
    return error;
}


/*---------------------------------------------------------------------------*
  qr2_error_t�Ή��G���[�����֐�
  �����@�Ferror QR2�̃G���[�^
  �߂�l�FQR2�̃G���[�^�i���������̂܂ܕԂ��j
  �p�r�@�Fqr2_error_t�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���ă}�b�`���C�N�葱�����I������
 *---------------------------------------------------------------------------*/
static qr2_error_t DWCi_HandleQR2Error(qr2_error_t error)
{
    int errorCode;
    DWCError dwcError;

    if (error == e_qrnoerror) return e_qrnoerror;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, QR2 error %d\n", error);

    switch (error){
    case e_qrwsockerror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SOCKET;
        break;
    case e_qrbinderror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_BIND;
        break;
    case e_qrdnserror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DNS;
        break;
    case e_qrconnerror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_PEER;
        break;
    case e_qrnochallengeerror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SERVER;
        break;
    }

    // �x�[�X�G���[�R�[�h��ǉ����Ċe�폈�����̌ŗL�G���[�������Ăяo��
    switch (DWC_GetState()){
    case DWC_STATE_LOGIN:
        errorCode += DWC_ECODE_SEQ_LOGIN+DWC_ECODE_GS_QR2;
        DWCi_StopLogin(dwcError, errorCode);     // ���O�C�������I��
        break;
    case DWC_STATE_UPDATE_SERVERS:
        errorCode += DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_QR2;
        DWCi_StopFriendProcess(dwcError, errorCode);  // �F�B�Ǘ������I��
        break;
    case DWC_STATE_MATCHING:
        // �}�b�`���C�N���ɃG���[�ɂȂ����ꍇ
        // �T�[�o�N���C�A���g�}�b�`���C�N�Ŋ���STATE_CONNECTED�ɂȂ��Ă���
        // ��Ԃł̃G���[���́A�ʂɃ}�b�`���C�N�R�[���o�b�N��Ԃ��Ȃ��Ă��ǂ�
        errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_QR2;
        DWCi_StopMatching(dwcError, errorCode);  // �}�b�`���C�N�����I��
        break;
    default:
        errorCode += DWC_ECODE_SEQ_ETC+DWC_ECODE_GS_QR2;
        DWCi_SetError(dwcError, errorCode);
        break;
    }
    
    return error;
}


/*---------------------------------------------------------------------------*
  NegotiateError�Ή��G���[�����֐�
  �����@�Ferror NN�̃G���[�^
  �߂�l�FNN�̃G���[�^�i���������̂܂ܕԂ��j
  �p�r�@�FNegotiateError�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���ă}�b�`���C�N�葱�����I������
 *---------------------------------------------------------------------------*/
static NegotiateError DWCi_HandleNNError(NegotiateError error)
{
    int errorCode;
    DWCError dwcError;

    if (error == ne_noerror) return ne_noerror;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, NN error %d\n", error);

    switch (error){
    case ne_allocerror:
        dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_ALLOC;
        break;
    case ne_socketerror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SOCKET;
        break;
    case ne_dnserror:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DNS;
        break;
    }

    // �x�[�X�G���[�R�[�h��ǉ�
    errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_NN;

    DWCi_StopMatching(dwcError, errorCode);
    
    return error;
}


/*---------------------------------------------------------------------------*
  NegotiateResult�Ή��G���[�����֐�
  �����@�Fresult NN�̌��ʌ^
  �߂�l�FNN�̌��ʌ^�i���������̂܂ܕԂ��j
  �p�r�@�FNegotiateResult�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���ă}�b�`���C�N�葱�����I������
 *---------------------------------------------------------------------------*/
static NegotiateResult DWCi_HandleNNResult(NegotiateResult result)
{
    int errorCode;
    DWCError dwcError;

    if (result == nr_success) return nr_success;

    DWC_Printf(DWC_REPORTFLAG_WARNING, "Match, NN result %d\n", result);

    switch (result){
    case nr_deadbeatpartner:
        return nr_deadbeatpartner;
        break;
    case nr_inittimeout:
        return nr_inittimeout;
        break;
    default:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_UNEXPECTED;
        break;
    }

    if (dwcError){
        // �x�[�X�G���[�R�[�h��ǉ�
        errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_NN;
        DWCi_StopMatching(dwcError, errorCode);
    }
    
    return result;
}


/*---------------------------------------------------------------------------*
  GT2Result�Ή��G���[�����֐�
  �����@�Fresult GT2�̏������ʌ^
  �߂�l�FGT2�̏������ʌ^�i���������̂܂ܕԂ��j
  �p�r�@�FGT2Result�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���āA�Ή�����R�[���o�b�N���Ăяo��
 *---------------------------------------------------------------------------*/
// [note]
// gt2Connect�ł����g���Ă��Ȃ�
static GT2Result DWCi_HandleGT2Error(GT2Result result)
{
    int errorCode;
    DWCError dwcError;

    if (result == GT2Success) return GT2Success;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, GT2 error %d\n", result);

    switch (result){
    case GT2OutOfMemory:
        dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_ALLOC;
        break;
    case GT2Rejected:
    case GT2DuplicateAddress:
        dwcError  = DWC_ERROR_NONE;
        errorCode = 0;
        result    = GT2Success;
        break;
    case GT2NetworkError:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_NETWORK;
        break;
    case GT2AddressError:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DNS;
        break;
    case GT2TimedOut:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_TIMEOUT;
        break;
    case GT2NegotiationError:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_PEER;
        break;
    }

    if (dwcError){
        // �x�[�X�G���[�R�[�h��ǉ�
        errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_GT2;
        DWCi_StopMatching(dwcError, errorCode);
    }

    return result;
}


/*---------------------------------------------------------------------------*
  SB�R�[���o�b�N�֐�
  �����@�Fsb       SB�I�u�W�F�N�g
          reason   �R�[���o�b�N�Ăяo�����R
          server   �X�V���ꂽ�T�[�o�\����
          instance �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FSB�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_SBCallback(ServerBrowser sb, SBCallbackReason reason, SBServer server, void *instance)
{
#pragma unused(instance)
    int profileID;
    int result;
    int i;
    NegotiateError nnError;

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "SBCallback : reason %d (state = %d)\n",
               reason, DWCi_GetMatchCnt()->state);

    // �R�[���o�b�N�̌Ăяo�����x�����J�E���g
    s_sbCallbackLevel++;

    // ServerBrowserLimitUpdate�̃^�C���A�E�g�̍X�V
    if(DWCi_GetMatchCnt()->sbUpdateRequestTick != 0)
    {
        if(reason == sbc_updatecomplete || reason == sbc_queryerror)
        {
            // �I��
            DWCi_GetMatchCnt()->sbUpdateRequestTick = 0;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout reset.(%s)\n", __FUNCTION__);
        }
        else
        {
            // �p���̂��߂Ƀ^�C���A�E�g������
            DWCi_GetMatchCnt()->sbUpdateRequestTick = OS_GetTick() + OS_MilliSecondsToTicks(DWC_SB_UPDATE_TIMEOUT);
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout update.(%s)\n", __FUNCTION__);
        }
    }

    switch (reason){
    case sbc_serveradded:     // �T�[�o���ǉ�
        DWCi_SBPrintServerData(server);  // �T�[�o�f�[�^�e�X�g�\��
        break;
        
    case sbc_updatecomplete:  // �T�[�o���S�ăA�b�v�f�[�g����
        // �T�[�o���X�g�𒲂ׂĐ��肷�܂��T�[�o�����X�g��������Ă����B
        // ����ȍ~�A������DWC�T�[�o�̃f�[�^���������݂�����̂Ƃ���
        // �������s���B���S�Ȑ��肷�܂��T�[�o���������ꍇ�͑��̃G���[��
        // �����|���邱�Ƃ����҂���B
        for (i = 0; i < ServerBrowserCount(sb); i++){
            server = ServerBrowserGetServer(sb, i);

            // server��DWC���C�u�������g�p���Ă��邩���`�F�b�N����
            // �v���t�@�C��ID���擾
            if (!DWCi_CheckDWCServer(server)){
                // �L���ȃv���t�@�C��ID�������Ȃ��T�[�o�̓��X�g����폜����
                ServerBrowserRemoveServer(sb, server);

                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
                           "Deleted server [%d].\n", i);
                i--;
            }
        }

        switch (DWCi_GetMatchCnt()->state){
        case DWC_MATCH_STATE_CL_SEARCH_OWN:
            // �F�B���w��s�A�}�b�`���C�N�̎��z�X�g������
            for (i = 0; i < ServerBrowserCount(sb); i++){
                server = ServerBrowserGetServer(sb, i);

                if (DWCi_GetMatchCnt()->qr2IP &&
                    (DWCi_GetMatchCnt()->qr2IP == SBServerGetPublicInetAddress(server)) &&
                    DWCi_GetMatchCnt()->qr2Port &&
                    (DWCi_GetMatchCnt()->qr2Port == SBServerGetPublicQueryPort(server)))
                    break;
            }

            if (i < ServerBrowserCount(sb)){
                SBError sbError;
                // ����QR2�A�h���X�R�[���o�b�N�Ŏ����̌��݂�IP�A�|�[�g�ԍ���
                // �������Ă���A�����ꂪ�}�X�^�[�T�[�o�ɃA�b�v����Ă���ꍇ
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_SEARCH_HOST);
                DWCi_GetMatchCnt()->reqProfileID = 0;  // �\�񑗐M��N���A

                // ���̌������J�n�B
                sbError = DWCi_SBUpdateAsync(DWCi_GetMatchCnt()->reqProfileID);
                if (DWCi_HandleSBError(sbError)) goto on_exit;
            }
            else
            {
                // �܂������̏�񂪃}�X�^�[�T�[�o�ɃA�b�v����Ă��Ȃ��ꍇ��
                // ���������ōĂ�SB�T�[�o�A�b�v�f�[�g��\�񂵂Ă���

//            DWCi_GetMatchCnt()->sbUpdateCount = 1;  // SB�T�[�o�A�b�v�f�[�g�\��
                DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_LONG;
                DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
            }
            break;
            
        case DWC_MATCH_STATE_CL_SEARCH_HOST:
            // �F�B���w��s�A�}�b�`���C�N�̋󂫃z�X�g�������B
            // �T�[�o����ؒf����A�V�����T�[�o���Љ��ăv���t�@�C��ID��
            // �����������������ɗ���B
            (void)DWCi_EvaluateServers(TRUE);  // �e�T�[�o��]������
            DWCi_RandomizeServers();           // �X�Ƀ����_���v�f��������

            if (ServerBrowserCount(sb)){
                // �������z�X�g������������A�\��v���R�}���h���M
                // �w��̃T�[�o�̏����Z�b�g���A�����ɗ\��R�}���h�𑗐M����
                result = DWCi_SendResvCommand(0, FALSE); 
                if (DWCi_HandleMatchCommandError(result)) goto on_exit; 

                // �\��m�F��Ԃֈڍs
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);

//                DWCi_GetMatchCnt()->sbUpdateCount = 0;
                DWCi_GetMatchCnt()->sbUpdateFlag = 0;
            }
            else {
                // 1���T�[�o��������Ȃ���΍Ă�SB�T�[�o�A�b�v�f�[�g��\�񂷂�
//                DWCi_GetMatchCnt()->sbUpdateCount = 1;
                DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_LONG;
                DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
            }
            break;
            
        case DWC_MATCH_STATE_CL_SEARCH_NN_HOST:
            // NAT�l�S�V�G�[�V�����̑���z�X�g������
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "searchIP: %x, searchPort: %d\n",
                       DWCi_GetMatchCnt()->searchIP, DWCi_GetMatchCnt()->searchPort);

            // �ŐV�̏��łȂ���΃T�[�o���X�g�������
            while (ServerBrowserCount(sb)){
                server = ServerBrowserGetServer(sb, 0);
                if ((SBServerGetPublicInetAddress(server) == DWCi_GetMatchCnt()->searchIP) &&
                    (SBServerGetPublicQueryPort(server) == DWCi_GetMatchCnt()->searchPort)){
                    break;
                }
                else {
                    ServerBrowserRemoveServer(sb, server);
                }
            }
                
            if (ServerBrowserCount(sb)){
                // �T�[�o�����������ꍇ
                profileID =
                    SBServerGetIntValue(ServerBrowserGetServer(sb, 0),
                                        DWC_QR2_PID_KEY_STR, 0);
                if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_FRIEND) &&
                    (profileID == DWCi_GetMatchCnt()->sbPidList[0])){
                    // �F�B�w��s�A�}�b�`���C�N�ł́A�T�[�o����������
                    // ���̂݁A���̃T�[�o�̕]�����s��
                    if (DWCi_EvaluateServers(FALSE)){
                        if (DWCi_GetMatchCnt()->gt2NumConnection){
                            // �����ɐڑ��ς݃z�X�g����������S�ăR�l�N�V����
                            // �N���[�Y���Ċ��S�ȂP�N���C�A���g�ɖ߂�
                            result = DWCi_ChangeToClient();
                            if (DWCi_HandleMatchCommandError(result)) goto on_exit;

                            // �R�l�N�V�����̃N���[�Y��҂��߁A�ȉ��̏����͔�΂�
                        }
                    }
                    else {
                        // �]���̌��ʃT�[�o���폜���ꂽ��\����L�����Z������
                        result = DWCi_CancelReservation(DWCi_GetMatchCnt()->sbPidList[0]);
                        if (DWCi_HandleMatchCommandError(result)) goto on_exit;

                        // ��Ԃ�\��҂��ɖ߂�
                        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_WAIT_RESV);

                        // ���̗F�B��NN�\��v���𑗐M����
                        result = DWCi_SendResvCommandToFriend(FALSE, FALSE, DWCi_GetMatchCnt()->sbPidList[0]);
                        if (DWCi_HandleMatchCommandError(result)) goto on_exit;
                        break;  // �ȉ��̏����͔�΂�
                    }
                }
                
                // NAT�l�S�V�G�[�V������Ԃ��Z�b�g
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_NN);
                
                // �T�[�o�ɑ΂���NAT�l�S�V�G�[�V�������J�n����
                server = ServerBrowserGetServer(sb, 0);
                nnError = DWCi_NNStartupAsync(0, 0, server);
                if (DWCi_HandleNNError(nnError)) goto on_exit;
            }
            else {
                // �T�[�o��������Ȃ������ꍇ�A�}�X�^�[�T�[�o��
                // �f�[�^���A�b�v�����܂ł̑҂����Ԃ��Ǝv����̂�
                // �Ă�SB�T�[�o�A�b�v�f�[�g���s��
//                DWCi_GetMatchCnt()->sbUpdateCount = 1;  // SB�T�[�o�A�b�v�f�[�g�J�E���g�J�n
                DWCi_GetMatchCnt()->sbUpdateFlag = DWC_SB_UPDATE_INTERVAL_LONG;
                DWCi_GetMatchCnt()->sbUpdateTick = OS_GetTick();
            }
            break;

        default:
            // �F�B���w��s�A�}�b�`���C�N���ɁA�󂫃z�X�g�������ɑ��z�X�g�����
            // �\����󂯕t�����ꍇ�́Astate = DWC_MATCH_STATE_SV_OWN_NN��
            // ��Ԃł����ɗ��邱�Ƃ����邪�A���ɉ������Ȃ��B
            break;
        }
        break;
        
    case sbc_queryerror:      // �N�G��������G���[
        break;
        
    default:
        // �A�b�v�f�[�g�P�񂲂ƂɃT�[�o���X�g���N���A����̂ŁA
        // reason �� sbc_serverupdated, sbc_serverupdatefailed�ɂȂ邱�Ƃ�
        // �Ȃ��͂��Bsbc_serverdeleted �͂���
        break;
    }

on_exit:

    // �R�[���o�b�N�̌Ăяo�����x�����J�E���g
    s_sbCallbackLevel--;

    return;

}


/*---------------------------------------------------------------------------*
  SB�T�[�o�f�[�^�\���e�X�g�֐�
  �����@�Fserver �X�V���ꂽ�T�[�o�\����
  �߂�l�F�Ȃ�
  �p�r�@�FSB�T�[�o�f�[�^���R���\�[���ɕ\������
 *---------------------------------------------------------------------------*/
static void DWCi_SBPrintServerData(SBServer server)
{
#if defined(SDK_FINALROM)
#pragma unused(server)
#endif

    int i;

    // �T�[�o�̏���\������
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerGetPrivateAddress     = %s\n",
               SBServerGetPrivateAddress(server));
  	DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerGetPrivateInetAddress = %x\n",
               SBServerGetPrivateInetAddress(server));
  	DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerGetPrivateQueryPort   = %d\n",
               SBServerGetPrivateQueryPort(server));
  	DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerGetPublicAddress      = %s\n",
               SBServerGetPublicAddress(server));
   	DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerGetPublicInetAddres   = %x\n",
               SBServerGetPublicInetAddress(server));
  	DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerGetPublicQueryPort    = %d\n",
               SBServerGetPublicQueryPort(server));
   	DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
               "SBServerHasPrivateAddress     = %d\n",
               SBServerHasPrivateAddress(server));

    // DWC�Ŏg�p���Ă���L�[�f�[�^��\������
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "numplayers  = %d\n",
               SBServerGetIntValue(server, "numplayers", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "maxplayers  = %d\n",
               SBServerGetIntValue(server, "maxplayers", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s     = %u\n",
               DWC_QR2_PID_KEY_STR,
               SBServerGetIntValue(server, DWC_QR2_PID_KEY_STR, 0));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %u\n",
               DWC_QR2_MATCH_RESV_KEY_STR,
               SBServerGetIntValue(server, DWC_QR2_MATCH_RESV_KEY_STR, -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n",
               DWC_QR2_MATCH_TYPE_KEY_STR,
               SBServerGetIntValue(server, DWC_QR2_MATCH_TYPE_KEY_STR, -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n",
               DWC_QR2_MATCH_VER_KEY_STR,
               SBServerGetIntValue(server, DWC_QR2_MATCH_VER_KEY_STR, -1));

    // �Q�[����`�̃L�[�f�[�^��\������
    for (i = 0; i < DWC_QR2_GAME_RESERVED_KEYS; i++){
        if (stGameMatchKeys[i].keyID){
            if (stGameMatchKeys[i].isStr){
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
                           "%s  = %s\n",
                           stGameMatchKeys[i].keyStr,
                           SBServerGetStringValue(server,
                                                  stGameMatchKeys[i].keyStr,
                                                  "NONE"));
            }
            else {
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
                           "%s  = %d\n",
                           stGameMatchKeys[i].keyStr,
                           SBServerGetIntValue(server,
                                               stGameMatchKeys[i].keyStr,
                                               -1));
            }
        }
    }
}


/*---------------------------------------------------------------------------*
  �T�[�o�]���֐�
  �����@�Fsort TRUE:�T�[�o���X�g��]���l���Ƀ\�[�g����AFALSE:�\�[�g���Ȃ�
  �߂�l�FTRUE:�ʏ�AFALSE:�T�[�o�f�[�^���폜���ăT�[�o���X�g����ɂȂ���
  �p�r�@�F�T�[�o�]���֐�
 *---------------------------------------------------------------------------*/
static BOOL DWCi_EvaluateServers(BOOL sort)
{
    int eval;
    int deleteFlag = 0;
    int localDelete;
    int profileID;
    int i, j;
    SBServer server;

    // �e�T�[�o�̕]�����s��
    for (i = 0; i < ServerBrowserCount(DWCi_GetMatchCnt()->sbObj); i++){
        server = ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, i);

        // �F�B���w��s�A�}�b�`���C�N�̏ꍇ�́A���Ɏ����Ɍq�������N���C�A���g��
        // �Â��T�[�o�f�[�^���Q�Ƃ��Ȃ��悤�ɂ���
        if (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_ANYBODY){
            profileID   = SBServerGetIntValue(server, DWC_QR2_PID_KEY_STR, 0);
            localDelete = 0;
            for (j = 1; j <= DWCi_GetMatchCnt()->gt2NumConnection; j++){
                if (profileID == DWCi_GetMatchCnt()->sbPidList[j]){
                    // ���ɐڑ��ς݂̃T�[�o�Ȃ����
                    ServerBrowserRemoveServer(DWCi_GetMatchCnt()->sbObj, server);

                    i--;
                    localDelete = 1;
                    break;
                }
            }

            if (localDelete) continue;
        }

        if (DWCi_GetMatchCnt()->evalCallback){
            eval = DWCi_GetMatchCnt()->evalCallback(i, DWCi_GetMatchCnt()->evalParam);
            
            if (eval > 0){
                // �]���l��0���傫����Ε]���l���T�[�o���X�g�ɏ������ށB
                // �����l�ł��K������t���邽�߂ɃV�t�g���ĉ��ʂɃ����_���l��
                // �Z�b�g����B
                if ( eval > 0x007fffff ) eval = 0x007fffff;
                SBServerAddIntKeyValue(server,
                                       DWC_QR2_MATCH_EVAL_KEY_STR,
                                       (int)((eval << 8) | DWCi_GetMathRand32(256)));
            }
            else {
                // �]���l��0�ȉ��Ȃ�}�b�`���C�N�̑Ώۂ���O��
                ServerBrowserRemoveServer(DWCi_GetMatchCnt()->sbObj, server);

                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
                           "Deleted server [%d] (eval point is %d).\n",
                           i, eval);

                i--;
                deleteFlag = 1;
            }
        }
        else {
            // �T�[�o�]�����s���Ȃ��ꍇ�������̓x�ɃT�[�o�̕��т�
            // �ς������̂ŁA�����_���ȕ]���l���Z�b�g����
            // �i���̎��͕]���͂��Ă��Ȃ��̂ŁA�]���l��0�ł��ǂ��j
            SBServerAddIntKeyValue(server, DWC_QR2_MATCH_EVAL_KEY_STR,
                                   (int)DWCi_GetMathRand32(DWC_MAX_CONNECTIONS << 2));
        }
    }

    //if (!DWCi_GetMatchCnt()->evalCallback) return TRUE;

    if (sort && ServerBrowserCount(DWCi_GetMatchCnt()->sbObj)){
        // �T�[�o���X�g�̃\�[�g���K�v�Ȃ�\�[�g����B
        // �]���R�[���o�b�N���ݒ肳��Ă��Ȃ��Ă��\���������_���ɂ������̂�
        // �\�[�g�͕K�v�B
        ServerBrowserSort(DWCi_GetMatchCnt()->sbObj, SBFalse, DWC_QR2_MATCH_EVAL_KEY_STR, sbcm_int);
    }

    if (deleteFlag && !ServerBrowserCount(DWCi_GetMatchCnt()->sbObj)) return FALSE;
    else return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o���X�g���������_�����֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�]���ς݃T�[�o���X�g���A�]���l�̑傫�����l���������_���v�f��
          �����čă\�[�g����B
          DWCi_EvaluateServers()�̌Ăяo������ɌĂяo���ׂ����̊֐��B
          [note]
          �I�񂾃T�[�o�ȊO�̃f�[�^���K�v�Ȃ��΃\�[�g�͂��Ȃ��ėǂ�
 *---------------------------------------------------------------------------*/
static void DWCi_RandomizeServers(void)
{
    u32 rand;
    int tmpEval, maxEval = 0;
    int sum = 0;
    int i;
    int eval[DWC_SB_UPDATE_MAX_SERVERS];
    SBServer server;

    if (ServerBrowserCount(DWCi_GetMatchCnt()->sbObj) <= 1) return;

    // �S�T�[�o�𒲂ׂĕ]���l�̍ő�l�ƁA�]�����[�g�̍��v�����߂�
    for (i = 0; i < ServerBrowserCount(DWCi_GetMatchCnt()->sbObj); i++){
        server  = ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, i);
        tmpEval = SBServerGetIntValue(server, DWC_QR2_MATCH_EVAL_KEY_STR, -1);
        if (tmpEval > maxEval) maxEval = tmpEval;
        
        sum += stEvalRate[i];
    }

    // 0�`99�̗����l�����߂�
    rand = DWCi_GetMathRand32(100);

    // �e�T�[�o�̕]�����[�g�l�̊����i���j�����߁A�����l����T�[�o��I��
    for (i = 0; i < ServerBrowserCount(DWCi_GetMatchCnt()->sbObj); i++){
        if (i == ServerBrowserCount(DWCi_GetMatchCnt()->sbObj)-1){
            // ���Z�덷�̂��ߍ��v�K��100���ɂ͂Ȃ�Ȃ��̂ŁA�Ō�܂ł�������
            // �K�����̃T�[�o��I��
            eval[i] = 100;
            break;
        }
        else {
            eval[i] = (100*stEvalRate[i])/sum+(i > 0 ? eval[i-1] : 0);
            if (rand < eval[i]) break;
        }
    }

    // �����ŕ\������C���f�b�N�X[]�̒l�́A�]���l�̑傫�����ɃT�[�o���X�g��
    // �\�[�g������ł̃C���f�b�N�X�l�ł��邱�Ƃɒ��ӂ���
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "Server[%d] is selected (%d/100: rand %d)\n",
               i, eval[i], rand);

    // �I�΂ꂽ�T�[�o�ɍő�̕]���l��^���Ă���A�\�[�g������
    if (maxEval < 0x7fffffff) maxEval++;
    SBServerAddIntKeyValue(ServerBrowserGetServer(DWCi_GetMatchCnt()->sbObj, i),
                           DWC_QR2_MATCH_EVAL_KEY_STR,
                           maxEval);

    ServerBrowserSort(DWCi_GetMatchCnt()->sbObj, SBFalse, DWC_QR2_MATCH_EVAL_KEY_STR, sbcm_int);
}


/*---------------------------------------------------------------------------*
  QR2�T�[�o�L�[�v����M�R�[���o�b�N�֐�
  �����@�Fkeyid    �L�[ID
          outbuf   ���M�p�o�b�t�@
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�T�[�o�L�[�v����M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2ServerKeyCallback(int keyid, qr2_buffer_t outbuf, void *userdata)
{
#pragma unused(userdata)
    int index;

    switch (keyid){
    case NUMPLAYERS_KEY:
        qr2_buffer_add_int(outbuf, DWCi_GetMatchCnt()->qr2NNFinishCount);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, DWCi_GetMatchCnt()->qr2NNFinishCount);
        break;
    case MAXPLAYERS_KEY:
        qr2_buffer_add_int(outbuf, DWCi_GetMatchCnt()->qr2NumEntry);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, DWCi_GetMatchCnt()->qr2NumEntry );
        break;
    case DWC_QR2_PID_KEY:
        qr2_buffer_add_int(outbuf, DWCi_GetMatchCnt()->profileID);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, DWCi_GetMatchCnt()->profileID );
        break;
    case DWC_QR2_MATCH_TYPE_KEY:
        qr2_buffer_add_int(outbuf, DWCi_GetMatchCnt()->qr2MatchType);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, DWCi_GetMatchCnt()->qr2MatchType );
        break;
    case DWC_QR2_MATCH_RESV_KEY:
        qr2_buffer_add_int(outbuf, DWCi_GetMatchCnt()->qr2Reservation);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, DWCi_GetMatchCnt()->qr2Reservation );
        break;
    case DWC_QR2_MATCH_VER_KEY:
        qr2_buffer_add_int(outbuf, DWC_MATCHING_VERSION);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, DWC_MATCHING_VERSION);
        break;
    case DWC_QR2_MATCH_EVAL_KEY:
        qr2_buffer_add_int(outbuf, 1);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d - %d\n", keyid, 1 );
        break;
    default:
        // �Q�[����`�̃L�[�f�[�^�����݂���Ȃ�A�b�v����
        index = keyid-DWC_QR2_GAME_KEY_START;
        if ( ( index >= 0 && index < DWC_QR2_GAME_RESERVED_KEYS )
             && stGameMatchKeys[index].keyID)
        {
            if (stGameMatchKeys[index].isStr){
                qr2_buffer_add(outbuf, (char *)stGameMatchKeys[index].value);
            }
            else {
                qr2_buffer_add_int(outbuf, *(int *)stGameMatchKeys[index].value);
            }
        }
        break;
    }
}


/*---------------------------------------------------------------------------*
  QR2�v���C���[�L�[�v����M�R�[���o�b�N�֐�
  �����@�Fkeyid    �L�[ID
          index    �v���C���[�C���f�b�N�X
          outbuf   ���M�p�o�b�t�@
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�v���C���[�L�[�v����M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2PlayerKeyCallback(int keyid, int index, qr2_buffer_t outbuf, void *userdata)
{
#pragma unused(keyid)
#pragma unused(index)
#pragma unused(outbuf)
#pragma unused(userdata)
}


/*---------------------------------------------------------------------------*
  QR2�`�[���L�[�v����M�R�[���o�b�N�֐�
  �����@�Fkeyid    �L�[ID
          index    �`�[���C���f�b�N�X
          outbuf   ���M�p�o�b�t�@
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�`�[���L�[�v����M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2TeamKeyCallback(int keyid, int index, qr2_buffer_t outbuf, void *userdata)
{
#pragma unused(keyid)
#pragma unused(index)
#pragma unused(outbuf)
#pragma unused(userdata)
}


/*---------------------------------------------------------------------------*
  QR2�L�[���X�g�v����M�R�[���o�b�N�֐�
  �����@�Fkeytype   �L�[���
          keybuffer �L�[���M�p�o�b�t�@
          userdata  �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�L�[���X�g�v����M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2KeyListCallback(qr2_key_type keytype, qr2_keybuffer_t keybuffer, void *userdata)
{
#pragma unused(userdata)
    int i;

    // �K�v�ȃL�[�̃��X�g�𑗐M���邽�߂Ƀo�b�t�@�ɓo�^����
    switch (keytype){
	case key_server:
        qr2_keybuffer_add(keybuffer, NUMPLAYERS_KEY);
        qr2_keybuffer_add(keybuffer, MAXPLAYERS_KEY);
        qr2_keybuffer_add(keybuffer, DWC_QR2_PID_KEY);
        qr2_keybuffer_add(keybuffer, DWC_QR2_MATCH_TYPE_KEY);
        qr2_keybuffer_add(keybuffer, DWC_QR2_MATCH_RESV_KEY);
        qr2_keybuffer_add(keybuffer, DWC_QR2_MATCH_VER_KEY);
        qr2_keybuffer_add(keybuffer, DWC_QR2_MATCH_EVAL_KEY);

        // �Q�[����`�̃L�[������Ȃ炻������M����
        for (i = 0; i < DWC_QR2_GAME_RESERVED_KEYS; i++){
            if (stGameMatchKeys[i].keyID){
                qr2_keybuffer_add(keybuffer, stGameMatchKeys[i].keyID);
            }
        }
        break;
    case key_player:
        break;
    case key_team:
        break;
    }

    DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received KeyListReq : keytype %d\n", keytype);
}


/*---------------------------------------------------------------------------*
  QR2�o�^�v���C���[���������̓`�[�����擾�R�[���o�b�N�֐�
  �����@�Fkeytype  �L�[���
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�v���C���[���A�������̓`�[����
  �p�r�@�FQR2�o�^�v���C���[���������̓`�[�����擾�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static int DWCi_QR2CountCallback(qr2_key_type keytype, void *userdata)
{
#pragma unused(keytype)
#pragma unused(userdata)

    return 0;
}


/*---------------------------------------------------------------------------*
  QR2�}�X�^�[�T�[�o�ւ̓o�^���s�R�[���o�b�N�֐�
  �����@�Ferror    QR2�G���[���
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�}�X�^�[�T�[�o�ւ̓o�^���s�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2AddErrorCallback(qr2_error_t error, gsi_char *errmsg, void *userdata)
{
#pragma unused(userdata)
#if defined(SDK_FINALROM)
#pragma unused(errmsg)
#endif

    DWC_Printf(DWC_REPORTFLAG_ERROR, "QR2 Failed query addition to master server %d\n", error);
    DWC_Printf(DWC_REPORTFLAG_ERROR, "%s\n", errmsg);

    (void)DWCi_HandleQR2Error(error);
}


/*---------------------------------------------------------------------------*
  QR2�p�u���b�N�A�h���X�ʒm�R�[���o�b�N�֐�
  �����@�Fip       QR2���g�p����p�u���b�NIP
          port     QR2���g�p����p�u���b�N�|�[�g�ԍ�
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�p�u���b�N�A�h���X�ʒm�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2PublicAddrCallback(unsigned int ip, unsigned short port, void* userdata)
{
#pragma unused(userdata)

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
               "Got my query IP %08x & port %d.\n", ip, port);

    DWCi_GetMatchCnt()->qr2IP   = ip;
    DWCi_GetMatchCnt()->qr2Port = port;
}


/*---------------------------------------------------------------------------*
  QR2 NAT�l�S�V�G�[�V�����v����M�R�[���o�b�N�֐�
  �����@�Fcookie   ��M�����N�b�L�[�l
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2 NAT�l�S�V�G�[�V�����v����M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2NatnegCallback(int cookie, void *userdata)
{
#pragma unused(userdata)
    NegotiateError nnError;

    // NAT�l�S�V�G�[�V�����v���͕K���󂯓����
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Got NN request, cookie = %x.\n", cookie);

    if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_CL_WAITING){
        // �V�K�ڑ��N���C�A���g����̗v�����󂯕t�����ꍇ�͏�Ԃ�i�߂�
        DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_NN);
    }
    else if ((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_NN) &&
             (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_SV_OWN_NN)){
        // �Ȃ��Ȃ��N���C�A���g���\��ɗ��Ȃ��̂ŗ\����L�����Z���������
        // NN�v���������ꍇ�͖�������
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "But already canceled reservation.\n");
        return;
    }

    if (DWCi_GetMatchCnt()->nnLastCookie == cookie){
        // �O��Ɠ����N�b�L�[����M������NN���g���C�Ɣ��f���ăJ�E���g�A�b�v
        DWCi_GetMatchCnt()->nnRecvCount++;
    }
    else {
        // �O��ƈႤ�N�b�L�[����M������V����NN�Ȃ̂ŃJ�E���^�N���A
        DWCi_GetMatchCnt()->nnRecvCount  = 0;
        DWCi_GetMatchCnt()->nnLastCookie = cookie;
    }

    // NAT�l�S�V�G�[�V�����̃N�b�L�[����M�����玸�s�������N���A
    DWCi_GetMatchCnt()->nnFailedTime = 0;

    // NAT�l�S�V�G�[�V�����J�n
    nnError = DWCi_NNStartupAsync(1, cookie, NULL);
    if (DWCi_HandleNNError(nnError)) return;

    // �\��ێ��^�C���A�E�g�v���I��
    DWCi_GetMatchCnt()->cmdCnt.command = DWC_MATCH_COMMAND_DUMMY;
}


/*---------------------------------------------------------------------------*
  QR2�N���C�A���g���b�Z�[�W��M�R�[���o�b�N�֐�
  �����@�Fdata     ��M�����f�[�^
          len      ��M�����f�[�^��
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FQR2�N���C�A���g���b�Z�[�W��M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_QR2ClientMsgCallback(gsi_char* data, int len, void* userdata)
{
#pragma unused(userdata)
    int offset = 0;
    BOOL boolResult;
    DWCSBMessage sbMsg;

    // [arakit] main 051010
    // �}�b�`���C�N���łȂ���΃R�}���h���󂯕t���Ȃ�
    if ((DWC_GetState() != DWC_STATE_MATCHING) &&
        ((DWC_GetState() != DWC_STATE_CONNECTED) ||
         ((DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_SV) &&
          (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_CL)))){
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed SB matching command.\n");
        return;
    }
    // [arakit] main 051010

    //if (len < sizeof(DWCSBMessageHeader)){
        // ��`����Ă��Ȃ��R�}���h�̃��b�Z�[�W����M�����ꍇ
    //    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "QR2, Got undefined SBcommand\n");
    //    return;
    //}

    // �����R�}���h���A�������p�P�b�g�ɂ��Ή��ł���悤�ɂ��Ă��邪�A
    // ���̂悤�ȃp�P�b�g�𑗐M���邱�Ƃ͂Ȃ�
    while (offset+sizeof(DWCSBMessageHeader) <= len){
        // SB���b�Z�[�W���R�s�[����
        MI_CpuCopy8(data, &sbMsg, sizeof(DWCSBMessageHeader));

        if (strncmp(sbMsg.header.identifier, DWC_SB_COMMAND_STRING, 4)){
            // ���ʎq�̈Ⴄ�R�}���h����M����
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Got undefined SBcommand.\n");
            return;
        }
        if (sbMsg.header.version != DWC_MATCHING_VERSION){
            // �}�b�`���C�N�v���g�R���̃o�[�W�����Ⴂ�R�}���h����M����
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Got different version SBcommand.\n");
            return;
        }
        
        MI_CpuCopy8(data+sizeof(DWCSBMessageHeader), sbMsg.data, sbMsg.header.size);

#ifdef NITRODWC_DEBUG
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "<SB> RECV-%s <- [%08x:%d] [pid=%u]\n",
                   get_dwccommandstr(sbMsg.header.command),
                   sbMsg.header.qr2IP, sbMsg.header.qr2Port,
                   sbMsg.header.profileID);
#else
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                   "<SB> RECV-0x%02x <- [%08x:%d] [pid=%u]\n",
                   sbMsg.header.command,
                   sbMsg.header.qr2IP, sbMsg.header.qr2Port,
                   sbMsg.header.profileID);
#endif		
        // ��M�����R�}���h����������
        // �G���[�����͊֐����ōs���Ă���
        boolResult = DWCi_ProcessRecvMatchCommand(sbMsg.header.command,
                                                  sbMsg.header.profileID,
                                                  sbMsg.header.qr2IP,
                                                  sbMsg.header.qr2Port,
                                                  sbMsg.data,
                                                  sbMsg.header.size >> 2);
        if (!boolResult) return;  // �G���[�̏ꍇ�͑��I��

        offset += sizeof(DWCSBMessageHeader)+sbMsg.header.size;
    }
}


/*---------------------------------------------------------------------------*
  NN�l�S�V�G�[�V���������R�[���o�b�N�֐�
  �����@�Fstate    NAT�l�S�V�G�[�V�����������
          userdata �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FNAT�l�S�V�G�[�V���������R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_NNProgressCallback(NegotiateState state, void* userdata)
{
#pragma unused(userdata)
#if defined(SDK_FINALROM)
#pragma unused(state)
#endif

	DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN, Got state update: %d\n", state);
}


/*---------------------------------------------------------------------------*
  NN�l�S�V�G�[�V���������R�[���o�b�N�֐�
  �����@�Fresult     NAT�l�S�V�G�[�V�������ʌ^
          gamesocket �ʐM�\�P�b�g�L�q�q
          remoteaddr �A�h���X�\���̂ւ̃|�C���^
          userdata   �R�[���o�b�N�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FNAT�l�S�V�G�[�V���������R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_NNCompletedCallback(NegotiateResult result, SOCKET gamesocket, struct sockaddr_in* remoteaddr, void* userdata)
{
#pragma unused(gamesocket)
    char pidStr[12];
    int  index;
    NegotiateError nnError;
    NegotiateResult nnResult;
    GT2Result gt2Result;
    DWCNNInfo* nnInfo = (DWCNNInfo *)userdata;

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN, Complete NAT Negotiation. result : %d\n", result);
    if (nnInfo)
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN cookie = %x.\n", nnInfo->cookie);

    if (((DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_CL_NN) &&
         (DWCi_GetMatchCnt()->state != DWC_MATCH_STATE_SV_OWN_NN)) ||
        !nnInfo){
        // �}�b�`���C�N�L�����Z�������A�L�����Z��������ɗ��Ă���������B
        // NN�N�b�L�[����M�ł����ANAT�l�S�V�G�[�V�������s�R�[���o�b�N����
        // ��M�����ꍇ�i!nnInfo�̏ꍇ�j����������B
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed NN after cancel.\n");
        return;
    }

    if (result == nr_success){
        if (remoteaddr){
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN, remote address : %s\n",
                       gt2AddressToString(remoteaddr->sin_addr.s_addr, SO_NtoHs(remoteaddr->sin_port), NULL));
        }

        nnInfo->cookie = 0;  // NN�I�����L�^

        DWCi_GetMatchCnt()->qr2NNFinishCount++;        // NN�����J�E���g�A�b�v
        index = DWCi_GetMatchCnt()->qr2NNFinishCount;  // �C���f�b�N�X�ޔ�

        if (nnInfo->isQR2){
            // NN��M���̏ꍇ
            // IP�A�|�[�g���X�g���X�V
            DWCi_GetMatchCnt()->ipList[index]   = remoteaddr->sin_addr.s_addr;
            DWCi_GetMatchCnt()->portList[index] = SO_NtoHs(remoteaddr->sin_port);

            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "NN child finished Nat Negotiation.\n");

            // NN��M���N���A
            DWCi_GetMatchCnt()->nnRecvCount  = 0;
            DWCi_GetMatchCnt()->nnLastCookie = 0;
            DWCi_GetMatchCnt()->nnFailedTime = 0;

            // GT2�ڑ���Ԃɐi��
            if (DWCi_GetMatchCnt()->state == DWC_MATCH_STATE_SV_OWN_NN){
                DWCi_SetMatchStatus(DWC_MATCH_STATE_SV_OWN_GT2);
            }
            else {
                DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_GT2);
            }

            // GT2Connect���g���C�J�E���g������
            DWCi_GetMatchCnt()->gt2ConnectCount = 0;

            // �K�������炩��gt2Connect()���s��
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2,
                       "gt2Connect() to pidList[%d] (%s)\n",
                       index,
                       gt2AddressToString(DWCi_GetMatchCnt()->ipList[index], DWCi_GetMatchCnt()->portList[index], NULL));

            // �R�l�N�V�����\���̂�ConnectedCallback���Ŏ擾�������̂�
            // �����ł�NULL��n���Ă����B
            // message�Ƃ��Ď����̃v���t�@�C��ID�𑗐M����B
            (void)OS_SNPrintf(pidStr, sizeof(pidStr), "%u", DWCi_GetMatchCnt()->profileID);
            gt2Result =
                gt2Connect(*DWCi_GetMatchCnt()->pGt2Socket,
                           NULL,
                           gt2AddressToString(DWCi_GetMatchCnt()->ipList[index], DWCi_GetMatchCnt()->portList[index], NULL),
                           (GT2Byte *)pidStr,
                           -1,
                           DWC_GT2_CONNECT_TIMEOUT,
                           DWCi_GetMatchCnt()->gt2Callbacks,
                           GT2False);
            if ( gt2Result == GT2OutOfMemory )
            {
                DWCi_HandleGT2Error(gt2Result);
                return;
            }
            else if ( gt2Result == GT2Success )
            {
                // �����̂Ƃ��́A�R�[���o�b�N��҂B
            }
            else if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[index]))
            {
                // gt2Connect�ɂ����Ȃ莸�s�����Ƃ��́A���Z�b�g���Ď��̐ڑ���҂B
                return;
            }
        }
        else {
            // NN�J�n���̏ꍇ
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                       "NN parent finished Nat Negotiation.\n");

            if (remoteaddr){
                // ���ۂ�NAT�l�S�V�G�[�V�������s�����ꍇ��
                // IP�A�|�[�g���X�g���X�V
                DWCi_GetMatchCnt()->ipList[index-1]   = remoteaddr->sin_addr.s_addr;
                DWCi_GetMatchCnt()->portList[index-1] = SO_NtoHs(remoteaddr->sin_port);
            }

            // gt2Connect()����M����܂ł̎��Ԃ��v�����邽�߁A
            // ���ݎ������L�^����
            DWCi_GetMatchCnt()->nnFinishTime = OS_GetTick();

            // ������GT2�R�l�N�V�������m������igt2Connect�����̂�҂j
            DWCi_SetMatchStatus(DWC_MATCH_STATE_CL_GT2);
        }
    }
    else {
        // NAT�l�S�V�G�[�V�������s�̏ꍇ
        if (!nnInfo->cookie){
            // �}�b�`���C�N�L�����Z��������ɗ����ꍇ�͖�������
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed NN error after cancel.\n");
            return;
        }
            
        // nr_inittimeout, nr_deadbeatpartner�ł̓G���[�����͂��Ȃ��悤�ɂ��Ă���
        nnResult = DWCi_HandleNNResult(result);

        if ((nnResult != nr_inittimeout) && (nnResult != nr_deadbeatpartner)) return;

        if (!nnInfo->isQR2){
            // SB���iNAT�l�S�V�G�[�V�����J�n���j
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Failed %d/%d NN send.\n",
                       nnInfo->retryCount, DWC_MAX_MATCH_NN_RETRY);
                
            if ((nnResult == nr_deadbeatpartner) ||
                ((nnResult == nr_inittimeout) &&
                 (nnInfo->retryCount >= DWC_MAX_MATCH_NN_RETRY))){
                // ���肩��̉������Ȃ��Ȃ����A�������̓��g���C�񐔃I�[�o�̏ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Abort NN.\n");

                nnInfo->cookie = 0;  // NN�I�����L�^

                // NN���s�񐔂��J�E���g���ď�������
                if (!DWCi_ProcessNNFailure(FALSE)) return;

                // �S�ẴN���C�A���g�������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�̓G���[�I������B
                if (!DWCi_CancelPreConnectedClientProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection]))
                    return;
            }
            else {
                nnInfo->retryCount++;  // �^�C���A�E�g�񐔋L�^
                
                // NAT�l�S�V�G�[�V�����J�n
                nnError = DWCi_DoNatNegotiationAsync(nnInfo);
                if (DWCi_HandleNNError(nnError)) return;
            }
        }
        else {
            // QR2���iNAT�l�S�V�G�[�V������M���j
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Failed %d/%d NN recv.\n",
                       DWCi_GetMatchCnt()->nnRecvCount, DWC_MAX_MATCH_NN_RETRY);

            // NAT�l�S�V�G�[�V�����Ɏ��s�������Ԃ��L�^
            DWCi_GetMatchCnt()->nnFailedTime = OS_GetTick();
            
            if ((nnResult == nr_deadbeatpartner) ||
                ((nnResult == nr_inittimeout) &&
                 (DWCi_GetMatchCnt()->nnRecvCount >= DWC_MAX_MATCH_NN_RETRY))){
                // ���肩��̉������Ȃ��Ȃ����A�������̓��g���C�񐔃I�[�o�̏ꍇ
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Abort NN.\n");

                nnInfo->cookie = 0;  // NN�I�����L�^

                // NN���s�񐔂��J�E���g���ď�������
                if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL) ||
                    (DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_SV)){
                    if (!DWCi_ProcessNNFailure(TRUE)) return;
                }
                else {
                    if (!DWCi_ProcessNNFailure(FALSE)) return;
                }

                // NN��M���N���A
                DWCi_GetMatchCnt()->nnRecvCount  = 0;
                DWCi_GetMatchCnt()->nnLastCookie = 0;
                DWCi_GetMatchCnt()->nnFailedTime = 0;
                    
                // �V�K�ڑ��̎󂯕t���������I�����ă}�b�`���C�N���ĊJ����B
                // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�́A�V�K�ڑ��N���C�A���g��
                // �ڑ��L�����Z���Ƃ��ď�������B
                if (!DWCi_CancelPreConnectedServerProcess(DWCi_GetMatchCnt()->sbPidList[DWCi_GetMatchCnt()->gt2NumConnection+1]))
                    return;
            }
        }
    }
}


/*---------------------------------------------------------------------------*
  NN�l�S�V�G�[�V�������s�񐔊֘A�����֐�
  �����@�FignoreError TRUE:���s���J�E���g���Ȃ��AFALSE:���s���J�E���g����
  �߂�l�FTRUE:�܂����s�ł���AFALSE:�K��񐔎��s�Ń}�b�`���C�N�G���[�I��
  �p�r�@�FNAT�l�S�V�G�[�V���������R�[���o�b�N
 *---------------------------------------------------------------------------*/
static BOOL DWCi_ProcessNNFailure(BOOL ignoreError)
{

    // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�Ɛڑ��ς݃N���C�A���g�́A
    // NN���L�����Z�����ăv���C�𑱍s����ׁANN���s�񐔂̃J�E���g�͍s��Ȃ�
    if (ignoreError) return TRUE;
    
    if (DWCi_GetMatchCnt()->qr2MatchType != DWC_MATCH_TYPE_SC_CL){
        // NN���s�񐔂��J�E���g
        DWCi_GetMatchCnt()->nnFailureCount++;

        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN failure %d/%d.\n",
                   DWCi_GetMatchCnt()->nnFailureCount, DWC_MATCH_NN_FAILURE_MAX);
    }

    if ((DWCi_GetMatchCnt()->qr2MatchType == DWC_MATCH_TYPE_SC_CL) ||
        (DWCi_GetMatchCnt()->nnFailureCount >= DWC_MATCH_NN_FAILURE_MAX)){
        // �K��񐔈ȏ㎸�s������}�b�`���C�N���G���[�I������B
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�ł͕K���G���[�I������
        DWCi_StopMatching(DWC_ERROR_NETWORK,
                          DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_NN+DWC_ECODE_TYPE_MUCH_FAILURE);
        return FALSE;
    }
    else {
        return TRUE;
    }
}
//----------------------------------------------------------------------------
// �J�v�Z�������̏��������邽�߂ɉ��ɂ����Ă��Ă���ϐ�
//----------------------------------------------------------------------------
// �}�b�`���C�N����I�u�W�F�N�g�ւ̃|�C���^
static DWCMatchControl* stpMatchCnt = NULL;
#ifdef NITRODWC_DEBUG
static OSTick sMatchStateTick;
#endif
/*---------------------------------------------------------------------------*
  �A�N�Z�X�֐�
 *---------------------------------------------------------------------------*/
static void DWCi_SetMatchCnt(DWCMatchControl *pCnt){ 
  stpMatchCnt = pCnt; 
#ifdef NITRODWC_DEBUG
  sMatchStateTick = OS_GetTick();
#endif
}
static DWCMatchControl *DWCi_GetMatchCnt(void){ return (stpMatchCnt); }
static void DWCi_SetMatchStatus(DWCMatchState state)
{
  SDK_ASSERT(stpMatchCnt);
#ifdef NITRODWC_DEBUG
  if(stpMatchCnt->state != state){
	disp_match_state(stpMatchCnt->state, state);
  }
#endif
  stpMatchCnt->state = state;
}

#ifdef NITRODWC_DEBUG
static char *match_st_string[DWC_MATCH_STATE_NUM] = 
{
  "DWC_MATCH_STATE_INIT",           // �������
  // �N���C�A���g���̏��
  "DWC_MATCH_STATE_CL_WAITING",         // �҂����
  "DWC_MATCH_STATE_CL_SEARCH_OWN",      // ���z�X�g��񌟍���
  "DWC_MATCH_STATE_CL_SEARCH_HOST",     // �󂫃z�X�g������ԁi�F�B���w��̎��̂݁j
  "DWC_MATCH_STATE_CL_WAIT_RESV",       // �\��ɑ΂���T�[�o����̕ԓ��҂�
  "DWC_MATCH_STATE_CL_SEARCH_NN_HOST",  // �\�񂪊�����������z�X�g��������
  "DWC_MATCH_STATE_CL_NN",              // NAT�l�S�V�G�[�V������
  "DWC_MATCH_STATE_CL_GT2",             // GT2�R�l�N�V�����m����
  "DWC_MATCH_STATE_CL_CANCEL_SYN",      // �T�[�o�N���C�A���g�Ń}�b�`���C�N�L�����Z������������
  "DWC_MATCH_STATE_CL_SYN",             // �}�b�`���C�N��������������
        
  // �T�[�o���̏��
  "DWC_MATCH_STATE_SV_WAITING",         // �҂����
  "DWC_MATCH_STATE_SV_OWN_NN",          // �N���C�A���g�Ƃ�NAT�l�S�V�G�[�V������
  "DWC_MATCH_STATE_SV_OWN_GT2",         // �N���C�A���g�Ƃ�GT2�R�l�N�V�����m����
  "DWC_MATCH_STATE_SV_WAIT_CL_LINK",    // �N���C�A���g���m�̐ڑ������҂�
  "DWC_MATCH_STATE_SV_CANCEL_SYN",      // �T�[�o�N���C�A���g�Ń}�b�`���C�N�L�����Z����������SYN-ACK�҂�
  "DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT", // �T�[�o�N���C�A���g�Ń}�b�`���C�N�L�����Z�����������I���҂�
  "DWC_MATCH_STATE_SV_SYN",             // �}�b�`���C�N�I����������SYN-ACK�҂�
  "DWC_MATCH_STATE_SV_SYN_WAIT",        // �}�b�`���C�N�I�����������I���҂�
  
  // ���ʏ��
  "DWC_MATCH_STATE_WAIT_CLOSE",         // �R�l�N�V�����N���[�Y�����҂�
  
  // �}�b�`���C�N�I�v�V�����g�p���݂̂̏��
  "DWC_MATCH_STATE_SV_POLL_TIMEOUT",    // �T�[�o��OPTION_MIN_COMPLETE�̃^�C���A�E�g���|�[�����O��
};

static void disp_time(void)
{
  int m_time, sec;

  m_time = (int)OS_TicksToMilliSeconds(OS_GetTick() - sMatchStateTick);
  sec = m_time/1000;
  m_time = (m_time-sec+50)/100;
  OS_TPrintf("�o�ߎ��� %d.%d �b\n", sec, m_time);
}
static void disp_match_state(DWCMatchState old, DWCMatchState now)
{
  int m_time, sec;
  OSTick tick;
  tick = sMatchStateTick; sMatchStateTick = OS_GetTick(); tick = sMatchStateTick - tick;
  m_time = (int)OS_TicksToMilliSeconds(tick);
  sec = m_time/1000;
  m_time = (m_time-sec+50)/100;
  OS_TPrintf("�X�e�[�^�X�ύX %s(%d.%d �b) -> %s\n", match_st_string[old], sec, m_time, match_st_string[now]);
}
static const char *get_dwccommandstr(u8 command)
{
	switch(command) {
	case DWC_MATCH_COMMAND_RESERVATION:
		return "DWC_MATCH_COMMAND_RESERVATION";
	case DWC_MATCH_COMMAND_RESV_OK:
		return "DWC_MATCH_COMMAND_RESV_OK";
	case DWC_MATCH_COMMAND_RESV_DENY:
		return "DWC_MATCH_COMMAND_RESV_DENY";
	case DWC_MATCH_COMMAND_RESV_WAIT:
		return "DWC_MATCH_COMMAND_RESV_WAIT";
	case DWC_MATCH_COMMAND_RESV_CANCEL:
		return "DWC_MATCH_COMMAND_RESV_CANCEL";
	case DWC_MATCH_COMMAND_TELL_ADDR:
		return "DWC_MATCH_COMMAND_TELL_ADDR";
	case DWC_MATCH_COMMAND_NEW_PID_AID:
		return "DWC_MATCH_COMMAND_NEW_PID_AID";
	case DWC_MATCH_COMMAND_LINK_CLS_REQ:
		return "DWC_MATCH_COMMAND_LINK_CLS_REQ";
	case DWC_MATCH_COMMAND_LINK_CLS_SUC:
		return "DWC_MATCH_COMMAND_LINK_CLS_SUC";
	case DWC_MATCH_COMMAND_CLOSE_LINK:
		return "DWC_MATCH_COMMAND_CLOSE_LINK";
	case DWC_MATCH_COMMAND_RESV_PRIOR:
		return "DWC_MATCH_COMMAND_RESV_PRIOR";
	case DWC_MATCH_COMMAND_CANCEL:
		return "DWC_MATCH_COMMAND_CANCEL";
	case DWC_MATCH_COMMAND_CANCEL_SYN:
		return "DWC_MATCH_COMMAND_CANCEL_SYN";
	case DWC_MATCH_COMMAND_CANCEL_SYN_ACK:
		return "DWC_MATCH_COMMAND_CANCEL_SYN_ACK";
	case DWC_MATCH_COMMAND_CANCEL_ACK:
		return "DWC_MATCH_COMMAND_CANCEL_ACK";
	case DWC_MATCH_COMMAND_SC_CLOSE_CL:
		return "DWC_MATCH_COMMAND_SC_CLOSE_CL";
	case DWC_MATCH_COMMAND_POLL_TIMEOUT:
		return "DWC_MATCH_COMMAND_POLL_TIMEOUT";
	case DWC_MATCH_COMMAND_POLL_TO_ACK:
		return "DWC_MATCH_COMMAND_POLL_TO_ACK";
	case DWC_MATCH_COMMAND_SC_CONN_BLOCK:
		return "DWC_MATCH_COMMAND_SC_CONN_BLOCK";
	case DWC_MATCH_COMMAND_FRIEND_ACCEPT:
		return "DWC_MATCH_COMMAND_FRIEND_ACCEPT";
	case DWC_MATCH_COMMAND_CL_WAIT_POLL:
		return "DWC_MATCH_COMMAND_CL_WAIT_POLL";
	case DWC_MATCH_COMMAND_SV_KA_TO_CL:
		return "DWC_MATCH_COMMAND_SV_KA_TO_CL";
	case DWC_MATCH_COMMAND_DUMMY:
		return "DWC_MATCH_COMMAND_DUMMY";
	default:
		return "DWC_MATCH_COMMAND_UNDEFINED";
	}
}
#endif
