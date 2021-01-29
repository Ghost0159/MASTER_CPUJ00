#include <nitro.h>
#include <base/dwc_base_gamespy.h>
#include <available.h>
#include <natneg/natneg.h>
#include <gstats/gpersist.h>

#include <base/dwc_report.h>
#include <base/dwc_account.h>
#include <base/dwc_error.h>
#include <base/dwc_connectinet.h>
#include <base/dwc_login.h>
#include <base/dwc_friend.h>
#include <base/dwc_match.h>
#include <base/dwc_transport.h>
#include <base/dwc_common.h>
#include <base/dwc_main.h>


//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
// FriendsMatch����I�u�W�F�N�g�ւ̃|�C���^
static DWCFriendsMatchControl* stpDwcCnt = NULL;

static int stLastSocketError = 0;  // �Ō�ɔ��������\�P�b�g�G���[�ԍ�

// gt2�R�l�N�V�������X�g
// [todo]
// ����InfoList�Ƌ��ɁA�Q�[������w�肳�ꂽ�T�C�Y�������X�g�����悤�ɕύX����
static GT2Connection stGt2ConnectionList[DWC_MAX_CONNECTIONS];
// gt2�R�l�N�V������񃊃X�g
static DWCConnectionInfo stConnectionInfoList[DWC_MAX_CONNECTIONS];


//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static BOOL DWCs_UpdateConnection( void );
static void DWCs_ForceShutdown( void );

static void DWCi_SetState(DWCState state);

static int  DWCi_DeleteAID(u8 aid);
static u32  DWCi_GetAIDBitmapFromList(u8* aidList, int aidListLen);

static GPResult DWCi_HandleGPError(GPResult result);
static GT2Result DWCi_HandleGT2Error(GT2Result result);

static void DWCi_LoginCallback(DWCError error, int profileID, void *param);
static void DWCi_UpdateServersCallback(DWCError error, BOOL isChanged, void* param);
static void DWCi_MatchedCallback(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int profileID, void* param);

static void DWCi_GPErrorCallback(GPConnection* pconnection, GPErrorArg* arg, void* param);
static void DWCi_GPRecvBuddyMessageCallback(GPConnection* pconnection, GPRecvBuddyMessageArg* arg, void* param);

static void DWCi_GT2ReceivedCallback(GT2Connection connection, GT2Byte* message, int len, GT2Bool reliable);
static void DWCi_GT2ClosedCallback(GT2Connection connection, GT2CloseReason reason);
static void DWCi_GT2PingCallback(GT2Connection connection, int latency);
static void DWCi_GT2SocketErrorCallback(GT2Socket socket);


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  FriendsMatch���C�u�����������֐�
  �����@�Fdwccnt        FriendsMatch����I�u�W�F�N�g�ւ̃|�C���^
          userdata      ���[�U�f�[�^�I�u�W�F�N�g�ւ̃|�C���^
          productID     GameSpy����^������v���_�N�gID
          gameName      GameSpy����^����ꂽ�Q�[�����iNULL�I�[�K�v�j
          secretKey     GameSpy����^����ꂽ�V�[�N���b�g�L�[�iNULL�I�[�K�v�j
          sendBufSize   DWC_Transport���g�����M�o�b�t�@�T�C�Y�B0�Ȃ�f�t�H���g8KByte���g�p����B
          recvBufSize   DWC_Transport���g����M�o�b�t�@�T�C�Y�B0�Ȃ�f�t�H���g8KByte���g�p����B
          friendList    �F�B���X�g�i�Q�[���Ŏg�p���Ȃ��ꍇ��NULL�ł��ǂ��j
          friendListLen �F�B���X�g�̍ő咷�i�v�f���j
  �߂�l�F�Ȃ�
  �p�r�@�FFriendsMatch���C�u����������������
 *---------------------------------------------------------------------------*/
void DWC_InitFriendsMatch(DWCFriendsMatchControl* dwccnt,
              DWCUserData* userdata,
              int productID,
              const char* gameName,
              const char* secretKey,
              int sendBufSize,
              int recvBufSize,
              DWCFriendData friendList[],
              int friendListLen)
{
    u32 cpySize;

    SDK_ASSERT(dwccnt);
    SDK_ASSERT(gameName);
    SDK_ASSERT(secretKey);
    SDK_ASSERT(friendListLen <= DWC_MAX_MATCH_IDX_LIST);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_InitFriendsMatch() was called!!\n");

    stpDwcCnt = dwccnt;  // ����\���̂ւ̃|�C���^���Z�b�g

    DWC_ClearError();  // �����܂ł̃G���[���N���A

    stpDwcCnt->gt2Socket              = NULL;
    stpDwcCnt->gt2Callbacks.connected = DWCi_GT2ConnectedCallback;  // dwc_match.c�Œ�`
    stpDwcCnt->gt2Callbacks.received  = DWCi_GT2ReceivedCallback;
    stpDwcCnt->gt2Callbacks.closed    = DWCi_GT2ClosedCallback;
    stpDwcCnt->gt2Callbacks.ping      = DWCi_GT2PingCallback;
    stpDwcCnt->gt2SendBufSize         = sendBufSize ? sendBufSize : 8*1024;
    stpDwcCnt->gt2RecvBufSize         = recvBufSize ? recvBufSize : 8*1024;

    stpDwcCnt->gpObj            = NULL;
    stpDwcCnt->userdata         = userdata;

    stpDwcCnt->state            = DWC_STATE_INIT;
    stpDwcCnt->lastState        = DWC_STATE_INIT;
    stpDwcCnt->aid              = 0;
    stpDwcCnt->ownCloseFlag     = FALSE;
    stpDwcCnt->profileID        = 0;
    stpDwcCnt->gameName         = gcd_gamename;    // stats�ϐ��ւ̃|�C���^���i�[
    stpDwcCnt->secretKey        = gcd_secret_key;  // stats�ϐ��ւ̃|�C���^���i�[

    stpDwcCnt->loginCallback         = NULL;
    stpDwcCnt->loginParam            = NULL;
    stpDwcCnt->updateServersCallback = NULL;
    stpDwcCnt->updateServersParam    = NULL;
    stpDwcCnt->matchedCallback       = NULL;
    stpDwcCnt->matchedParam          = NULL;
    stpDwcCnt->matchedSCCallback     = NULL;
    stpDwcCnt->matchedSCParam        = NULL;
    stpDwcCnt->closedCallback        = NULL;
    stpDwcCnt->closedParam           = NULL;

    // gt2�R�l�N�V�������X�g��gt2�R�l�N�V������񃊃X�g��������
    DWCi_ClearGT2ConnectionList();

    // ���O�C������\���̏�����
    DWCi_LoginInit(&stpDwcCnt->logcnt, userdata, &stpDwcCnt->gpObj, productID, userdata->gamecode, stpDwcCnt->playerName, DWCi_LoginCallback, NULL);

    // �F�B�Ǘ��\���̏�����
    DWCi_FriendInit(&stpDwcCnt->friendcnt, &stpDwcCnt->gpObj, stpDwcCnt->playerName, friendList, friendListLen);

    // �}�b�`���C�N����\���̏�����
    DWCi_MatchInit(&stpDwcCnt->matchcnt, &stpDwcCnt->gpObj,
                   &stpDwcCnt->gt2Socket, &stpDwcCnt->gt2Callbacks,
                   gcd_gamename, gcd_secret_key,
                   friendList, friendListLen);

    // �g�����X�|�[�g����\���̏�����
    DWCi_InitTransport(&stpDwcCnt->transinfo);

    // GameSpy persistent SDK �̂��߂ɃO���[�o���ϐ���ݒ�igcd_gamename, gcd_secret_key�j
    cpySize = strlen(gameName) < sizeof(gcd_gamename) ? strlen(gameName) : sizeof(gcd_gamename)-1;
    MI_CpuCopy8(gameName, gcd_gamename, cpySize);
    gcd_gamename[cpySize] = '\0';
    
    cpySize = strlen(secretKey) < sizeof(gcd_secret_key) ? strlen(secretKey) : sizeof(gcd_secret_key)-1;
    MI_CpuCopy8(secretKey, gcd_secret_key, cpySize);
    gcd_secret_key[cpySize] = '\0';
}


/*---------------------------------------------------------------------------*
  FriendsMatch���C�u�����I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FFriendsMatch���C�u�������I�����AGameSpySDK�̃q�[�v�̈���J������
 *---------------------------------------------------------------------------*/
void DWC_ShutdownFriendsMatch(void)
{

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "!!DWC_ShutdownFriendsMatch() was called!! stpDwcCnt = 0x%x\n",
               stpDwcCnt);

    if (!stpDwcCnt) return;

    // GP�X�e�[�^�X���I�t���C���ɂ���
    // ��gpDestroy()���̃N���[�Y���M�i����͑����M�����j����������
    //if (stpDwcCnt->gpObj){
    //    (void)DWCi_SetGPStatus(DWC_STATUS_OFFLINE, "", "");
    //    (void)gpProcess(&stpDwcCnt->gpObj);  // GP�T�[�o�ɃA�b�v
    //}

    // QR2�I�u�W�F�N�g�J��
    // qr2_shutdown()�͒ʐM����̂�gt2CloseSocket()���O�ɌĂ�
    if (stpDwcCnt->matchcnt.qr2Obj){
        qr2_shutdown(stpDwcCnt->matchcnt.qr2Obj);
        stpDwcCnt->matchcnt.qr2Obj = NULL;  // ������NULL�N���A���Ȃ���΂Ȃ�Ȃ��I
    }
#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    stpDwcCnt->matchcnt.qr2ShutdownFlag = 0;  // QR2�V���b�g�_�E���t���O�N���A
#endif

    // SB�I�u�W�F�N�g�J��
    if (stpDwcCnt->matchcnt.sbObj){
        ServerBrowserFree(stpDwcCnt->matchcnt.sbObj);
        stpDwcCnt->matchcnt.sbObj = NULL;  // ������NULL�N���A���Ȃ���΂Ȃ�Ȃ��I
    }

    // �l�S�V�G�[�V�������X�g���
    DWCi_NNFreeNegotiateList();

    CloseStatsConnection();  // Persistent�I�u�W�F�N�g�̊J��

    // GP�I�u�W�F�N�g�̊J��
    // [todo]
    // ������R�[���o�b�N���ŌĂ΂ꂽ�ꍇ�ɖ�肪���邪�A�J������K�v������
    if (stpDwcCnt->gpObj){
         // WCM��disconnect���Ă���Ƃ��ɁA�Ō�̑��M�G���[���o��ꍇ������A���̂Ƃ��Ƀ��������[�N����B
        (void)gpSetCallback(&stpDwcCnt->gpObj, GP_ERROR, (GPCallback)NULL, NULL);
        (void)gpSetCallback(&stpDwcCnt->gpObj, GP_RECV_BUDDY_MESSAGE, (GPCallback)NULL, NULL);
        (void)gpSetCallback(&stpDwcCnt->gpObj, GP_RECV_BUDDY_REQUEST, (GPCallback)NULL, NULL);
        (void)gpSetCallback(&stpDwcCnt->gpObj, GP_RECV_BUDDY_STATUS, (GPCallback)NULL, NULL);
        (void)gpProcess(&stpDwcCnt->gpObj); // ���܂��Ă��郁������f���o���B
        gpDestroy(&stpDwcCnt->gpObj);
        stpDwcCnt->gpObj = NULL;
    }

    // �e���W���[���̃V���b�g�_�E���i��ɐ���I�u�W�F�N�g�̃N���A�j
    DWCi_ShutdownLogin();
    DWCi_ShutdownFriend();
    DWCi_ShutdownMatch();
    DWCi_ShutdownTransport();

    // GT2�\�P�b�g�̊J��
    // [todo]
    // �R�[���o�b�N���Ŋ֐����Ă΂ꂽ�ꍇ�ɃI�u�W�F�N�g���Ȃ��Ď~�܂�
    // ���Ƃ��Ȃ����m�F
    if (stpDwcCnt->gt2Socket){
        gt2CloseSocket(stpDwcCnt->gt2Socket);
        stpDwcCnt->gt2Socket = NULL;  // ������NULL�N���A���Ȃ���΂Ȃ�Ȃ��I
    }

#ifdef GSI_MEM_MANAGED
    gsMemMgrDestroy();  // GameSpy�̃q�[�v�̈�ƃq�[�v�}�l�[�W���J��
#endif

    stpDwcCnt = NULL;  // FriendsMatch����I�u�W�F�N�g�N���A
    
    // �p�����[�^������
    //DWC_InitFriendsMatch(dwccnt, dwccnt->userdata, dwccnt->logcnt.productID, dwccnt->gameName,
    //                     dwccnt->secretKey, dwccnt->gt2SendBufSize,
    //                     dwccnt->gt2RecvBufSize, NULL, NULL);
}


/*---------------------------------------------------------------------------*
  FriendsMatch���C�u�����ʐM�����X�V�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F���Q�[���t���[���Ăяo���A FriendsMatch���C�u�����̒ʐM�������X�V����
 *---------------------------------------------------------------------------*/
void DWC_ProcessFriendsMatch(void)
{
    GSIACResult acResult;
    GPResult gpResult;

    if ( DWC_UpdateConnection() )
    {
        // DCF���ؒf���ꂽ�B
        DWCs_ForceShutdown();
    }

    if (!stpDwcCnt || (stpDwcCnt->state == DWC_STATE_INIT) || DWCi_IsError())
        return;

    switch (stpDwcCnt->state){
    case DWC_STATE_AVAILABLE_CHECK:  // �Q�[�����p�\�󋵃`�F�b�N��
        acResult = GSIAvailableCheckThink();  // AvailableCheck�X�V
        switch (acResult){
        case GSIACAvailable:    // ���p�\�B
            DWC_Printf(DWC_REPORTFLAG_ACHECK, "Confirmed the backend of GameSpy server.\n");
            // GP������
            //
            // Nintendo DS
            //
            // namespaceID - 16
            // partnerID -   11(not used for this GameSpy SDK version)
            // partnerCode - NDS(used by NAS)
            //
            gpResult = gpInitialize(&stpDwcCnt->gpObj, stpDwcCnt->logcnt.productID, 0);
            if (DWCi_HandleGPError(gpResult)) return;

#if 0       // �����؂̂��ߕۗ�
            // GP�̃L���b�V����BUDDY�݂̂Ɍ���
            gpResult = gpDisable(&stpDwcCnt->gpObj, GP_INFO_CACHING_BUDDY_ONLY); 
            if (DWCi_HandleGPError(gpResult)) return;
#endif
            
            // GP�̃R�[���o�b�N�֐���ݒ�
            gpResult = gpSetCallback(&stpDwcCnt->gpObj, GP_ERROR,
                                     (GPCallback)DWCi_GPErrorCallback,
                                     NULL);
            if (DWCi_HandleGPError(gpResult)) return;

            gpResult = gpSetCallback(&stpDwcCnt->gpObj, GP_RECV_BUDDY_MESSAGE,
                                     (GPCallback)DWCi_GPRecvBuddyMessageCallback,
                                     NULL);
            if (DWCi_HandleGPError(gpResult)) return;

            // DWCi_GPRecvBuddyRequestCallback()��dwc_friend.c�Œ�`����Ă���
            gpResult = gpSetCallback(&stpDwcCnt->gpObj, GP_RECV_BUDDY_REQUEST,
                                     (GPCallback)DWCi_GPRecvBuddyRequestCallback,
                                     NULL);
            if (DWCi_HandleGPError(gpResult)) return;

            // DWCi_GPRecvBuddyStatusCallback()��dwc_friend.c�Œ�`����Ă���
            gpResult = gpSetCallback(&stpDwcCnt->gpObj, GP_RECV_BUDDY_STATUS,
                                     (GPCallback)DWCi_GPRecvBuddyStatusCallback,
                                     NULL);
            if (DWCi_HandleGPError(gpResult)) return;

            DWCi_SetState(DWC_STATE_LOGIN);

            // ���O�C�������֐����Ăяo��
            DWCi_LoginAsync();
            break;
        case GSIACUnavailable:  // �T�[�r�X�I��
            // ���O�C�������I��
            DWCi_StopLogin(DWC_ERROR_AUTH_OUT_OF_SERVICE, -20110); // �T�[�r�X�I���̃G���[�R�[�h�F-20110
            return;
            break;
        case GSIACTemporarilyUnavailable:  // �ꎞ�I�ɗ��p�s��
            // ���O�C�������I��
            DWCi_StopLogin(DWC_ERROR_AUTH_STOP_SERVICE, -20101); // �T�[�r�X�ꎞ���p�s�̃G���[�R�[�h�F-20101
            return;
            break;
        default:
            break;
        }
        break;
        
    case DWC_STATE_LOGIN:  // ���O�C��������
        DWCi_LoginProcess();
        break;

    case DWC_STATE_UPDATE_SERVERS:  // �F�B���X�g����������
    case DWC_STATE_ONLINE:          // �I�����C����
        DWCi_FriendProcess();
        DWCi_MatchProcess(FALSE);  // ������qr2_think(), gt2Think()���Ăяo��
        break;

    case DWC_STATE_MATCHING:  // �}�b�`���C�N������
        DWCi_MatchProcess(TRUE);
        DWCi_FriendProcess();
        break;

    case DWC_STATE_CONNECTED:  // �ڑ��������
		DWCi_TransportProcess();
        DWCi_FriendProcess();

        if ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
            (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
            // �r���Q������̏ꍇ�̓}�b�`���C�N�������s��
            DWCi_MatchProcess(TRUE);
        }
        else if (stpDwcCnt->gt2Socket){
            // ��L�ȊO�̏ꍇ�͂�����qr2_think(), gt2Think()���Ăяo��
            DWCi_MatchProcess(FALSE);
        }
        break;

    default:
        break;
    }

#ifdef DWC_QR2_ALIVE_DURING_MATCHING
    if (stpDwcCnt->matchcnt.qr2ShutdownFlag == 1){
        // QR2�V���b�g�_�E���t���O�������Ă�����QR2���I������
        if (stpDwcCnt->matchcnt.qr2Obj != NULL){
            qr2_shutdown(stpDwcCnt->matchcnt.qr2Obj);
            stpDwcCnt->matchcnt.qr2Obj = NULL;  // ������NULL�N���A���Ȃ���΂Ȃ�Ȃ��I
        }
        stpDwcCnt->matchcnt.qr2ShutdownFlag = 0;  // QR2�V���b�g�_�E���t���O�N���A
    }
#endif
}


/*---------------------------------------------------------------------------*
  Wi-Fi�R�l�N�V�������O�C���֐�
  �����@�Fingamesn �Q�[�����X�N���[���l�[��
          reserved �ߋ��̎d�l�BNULL��n���B
          callback ���O�C�������ʒm�p�R�[���o�b�N�֐�
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B���������s�ŃR�[���o�b�N���Ԃ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�Q�[�������p�\�ł��邩���ׁA�����[�g�F�؁AGP�T�[�o�ւ̐ڑ����s��
 *---------------------------------------------------------------------------*/
BOOL DWC_LoginAsync(const u16*  ingamesn,
                    const char* reserved,
                    DWCLoginCallback callback,
                    void* param)
{
#pragma unused(reserved)
    u32 len;

    SDK_ASSERT(stpDwcCnt);
//    SDK_ASSERT(userID);
//    SDK_ASSERT(password);
    SDK_ASSERT(callback);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_LoginAsync() was called!!\n");

    if (ingamesn == NULL){
        // ingamesn�͕K���w�肵�Ă��炤�K�v������
        DWC_Printf(DWC_REPORTFLAG_WARNING, "ingamesn is NULL!!\n");
        return FALSE;
    }

    // [arakit] main 051025
    if (DWCi_IsError() || (stpDwcCnt->state != DWC_STATE_INIT)){
        // ���Ƀ��O�C���������O�C�����Ă���̂ɌĂ΂ꂽ�牽�����Ȃ�
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }
    // [arakit] main 051025

    stpDwcCnt->loginCallback   = callback;
    stpDwcCnt->loginParam      = param;
    //stpDwcCnt->logcnt.userID   = userID;
    //stpDwcCnt->logcnt.password = password;

    // �Q�[�����X�N���[���l�[����o�^����
    if (!ingamesn || (ingamesn[0] == '\0')){
        len = 0;
    }
    else {
        // �ő啶�����Ɏ��܂镪�����R�s�[����
        MI_CpuClear16( stpDwcCnt->playerName, DWC_MAX_PLAYER_NAME*2 );
        len = DWCi_WStrLen(ingamesn) <= DWC_MAX_PLAYER_NAME-1 ? DWCi_WStrLen(ingamesn) : DWC_MAX_PLAYER_NAME-1;
        MI_CpuCopy16(ingamesn, stpDwcCnt->playerName, len*2);
    }
    stpDwcCnt->playerName[len] = 0;

    // �܂��C���^�[�l�b�g�ɐڑ����ĂȂ��̂ɁALogin���悤�Ƃ����B
    if (DWC_GetInetStatus() != DWC_CONNECTINET_STATE_CONNECTED )
    {
        DWCi_StopLogin( DWC_ERROR_AUTH_ANY, DWC_ECODE_SEQ_LOGIN + DWC_ECODE_TYPE_NETWORK );
        // [arakit] main 051025
        return TRUE;
    }

    DWCi_SetState(DWC_STATE_AVAILABLE_CHECK);

    // GameSpy API���g�p����O�ɁAGAME_NAME�Ŏw�肵���Q�[����
    // GameSpy�T�[�o�ŏ����ł��邩�m�F����B
	// Available�`�F�b�N�J�n
	GSIStartAvailableCheck(stpDwcCnt->gameName);

    // [arakit] main 051025
    return TRUE;
}


/*---------------------------------------------------------------------------*
  �F�B���X�g���������֐�
  �����@�FplayerName     ���̃��[�U���Q�Ƃ��鎩���̃v���C���[��
                         ���ߋ��̎d�l�ł��B���݂̓Z�b�g���ꂽ�l�𖳎����Ă��܂��B
          updateCallback �F�B���X�g�������������R�[���o�b�N
          updateParam    ��L�R�[���o�b�N�p�p�����[�^
          statusCallback �F�B��ԕω��ʒm�R�[���o�b�N
          statusParam    ��L�R�[���o�b�N�p�p�����[�^
          deleteCallback �F�B���X�g�폜�R�[���o�b�N
          deleteParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B���������s�ŃR�[���o�b�N���Ԃ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F���O�C��������AGP�T�[�o��̗F�B���X�g�i�o�f�B���X�g�j��
          ���[�J���̗F�B���X�g�̓����������s��
 *---------------------------------------------------------------------------*/
BOOL DWC_UpdateServersAsync(const char* playerName,
                            DWCUpdateServersCallback updateCallback,
                            void* updateParam,
                            DWCFriendStatusCallback statusCallback,
                            void* statusParam,
                            DWCDeleteFriendListCallback deleteCallback,
                            void* deleteParam)
{
#pragma unused(playerName)
    //u32 len;

    // [arakit] main 051025
    SDK_ASSERT(stpDwcCnt);
    SDK_ASSERT(updateCallback);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_UpdateServersAsync() was called!!\n");

    // [arakit] main 051025
    if (DWCi_IsError() ||
        (stpDwcCnt->state < DWC_STATE_ONLINE) || (stpDwcCnt->state == DWC_STATE_UPDATE_SERVERS)){
        // ���O�C���R�[���o�b�N���ɌĂ΂���STATE_ONLINE
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

#if 0
    // �v���C���[�����擾���v���C���[���͂����ł̓Z�b�g���Ȃ�
    if (!playerName || (playerName[0] == '\0')){
        len = 0;
    }
    else {
        len = strlen(playerName) < DWC_MAX_PLAYER_NAME ? strlen(playerName) : DWC_MAX_PLAYER_NAME-1;
        MI_CpuCopy8(playerName, stpDwcCnt->playerName, len);
    }
    stpDwcCnt->playerName[len] = '\0';  // NULL�I�[��ۏ�
#endif

    stpDwcCnt->updateServersCallback = updateCallback;
    stpDwcCnt->updateServersParam    = updateParam;

    // �F�B���X�g����������ԂɈڍs
    DWCi_SetState(DWC_STATE_UPDATE_SERVERS);

    // �F�B���X�g���������J�n
    DWCi_UpdateServersAsync(stpDwcCnt->logcnt.authToken,
                            stpDwcCnt->logcnt.partnerChallenge,
                            DWCi_UpdateServersCallback, NULL,
                            statusCallback, statusParam,
                            deleteCallback, deleteParam);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �F�B���w��s�A�}�b�`���C�N�J�n�֐�
  �����@�FnumEntry        �v������l�b�g���[�N�\���l���i�������܂ށj
          addFilter       �Q�[���Œǉ��������}�b�`���C�N����������B
                          ������ǉ����Ȃ��ꍇ��NULL��n���B
                          �����̓X�^���_�[�h��SQL�̏����ŏ������Ƃ��ł��܂��B
                          ���̂Ƃ���ݒ�ł��镶���񒷂͍ő�127�����ŁA
                          �f�o�b�O�r���h�ł͕������I�[�o�`�F�b�N�����Ă��܂��B
                          ������̓R�s�[���ă��C�u�������ŕێ����܂��B
          matchedCallback �}�b�`���C�N�����R�[���o�b�N
          matehedParam    ��L�R�[���o�b�N�p�p�����[�^
          evalCallback    �v���C���[�]���R�[���o�b�N
          evalParam       ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�F�B���w�肹���ɁA�l���w��Ń��b�V���^�l�b�g���[�N���쐬����
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
BOOL DWC_ConnectToAnybodyAsync(u8  numEntry,
                               const char* addFilter,
                               DWCMatchedCallback matchedCallback,
                               void* matchedParam,
                               DWCEvalPlayerCallback evalCallback,
                               void* evalParam)
{

    SDK_ASSERT(stpDwcCnt);
    SDK_ASSERT((numEntry > 1) && (numEntry <= DWC_MAX_CONNECTIONS));
    SDK_ASSERT(matchedCallback);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToAnybodyAsync() was called!!\n");

    if (DWCi_IsError() || (stpDwcCnt->state != DWC_STATE_ONLINE)){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        // [arakit] main 051025
        return FALSE;
    }

    // gt2�R�l�N�V�������X�g��gt2�R�l�N�V������񃊃X�g��������
    DWCi_ClearGT2ConnectionList();

    stpDwcCnt->matchedCallback = matchedCallback;
    stpDwcCnt->matchedParam    = matchedParam;

    DWCi_SetState(DWC_STATE_MATCHING);

    // �}�b�`���C�N�֐��Ăяo��
    // numEntry�̓}�b�`���C�N�������ł͎������܂܂Ȃ��ڑ��l����\���̂�-1���ēn��
    DWCi_ConnectToAnybodyAsync((u8)(numEntry-1),
                               addFilter,
                               DWCi_MatchedCallback, NULL,
                               evalCallback, evalParam);

    // [arakit] main 051025
    return TRUE;
}


/*---------------------------------------------------------------------------*
  �F�B�w��s�A�}�b�`���C�N�J�n�֐�
  �����@�FfriendIdxList    �ڑ��v���F�B�C���f�b�N�X���X�g�B
                           NULL�Ȃ�F�B���X�g�S�Ă�ڑ��v���ΏۂƂ���B
          friendIdxListLen �ڑ��v���F�B�C���f�b�N�X���X�g��
          numEntry         �v������l�b�g���[�N�\���l���i�������܂ށj
          distantFriend    TRUE:�F�B�̗F�B�Ƃ̐ڑ��������AFALSE:�����Ȃ�
          matchedCallback  �}�b�`���C�N�����R�[���o�b�N
          matchedParam     ��L�R�[���o�b�N�p�p�����[�^
          evalCallback     �v���C���[�]���R�[���o�b�N
          evalParam        ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�F�B���w�肵�Đڑ����A���b�V���^�l�b�g���[�N���쐬����
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
BOOL DWC_ConnectToFriendsAsync(const u8 friendIdxList[],
                               int friendIdxListLen,
                               u8  numEntry,
                               BOOL distantFriend,
                               DWCMatchedCallback matchedCallback,
                               void* matchedParam,
                               DWCEvalPlayerCallback evalCallback,
                               void* evalParam)
{
    // �ꎞ�F�B���X�g�B�Q�[������F�B�ő吔�����炤�悤�ɂȂ�����T�C�Y��ύX����
    u8  tmpFriendIdxList[DWC_MAX_MATCH_IDX_LIST];
    u8  idxList[DWC_MAX_MATCH_IDX_LIST];
    u8  i;
    u32 j;

    SDK_ASSERT(stpDwcCnt);
    SDK_ASSERT(friendIdxListLen <= DWC_MAX_MATCH_IDX_LIST);
    SDK_ASSERT((numEntry > 1) && (numEntry <= DWC_MAX_CONNECTIONS));
    SDK_ASSERT(matchedCallback);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToFriendsAsync() was called!!\n");

    if (DWCi_IsError() || (stpDwcCnt->state != DWC_STATE_ONLINE)){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        // [arakit] main 051025
        return FALSE;
    }

    // gt2�R�l�N�V�������X�g��gt2�R�l�N�V������񃊃X�g��������
    DWCi_ClearGT2ConnectionList();

    stpDwcCnt->matchedCallback = matchedCallback;
    stpDwcCnt->matchedParam    = matchedParam;

    DWCi_SetState(DWC_STATE_MATCHING);

    // numEntry�̓}�b�`���C�N�������ł͎������܂܂Ȃ��ڑ��l����\���̂�-1���ēn��
    if (friendIdxList){
        // �}�b�`���C�N�֐��Ăяo��
        DWCi_ConnectToFriendsAsync(friendIdxList, friendIdxListLen,
                                   (u8)(numEntry-1),
                                   distantFriend,
                                   DWCi_MatchedCallback, NULL,
                                   evalCallback, evalParam);
    }
    else {
        // �F�B�C���f�b�N�X���X�g�̎w�肪�����ꍇ�́A�S�Ă̗F�B�̃C���f�b�N�X��
        // �ꎞ�C���f�b�N�X���X�g�ɓo�^���āA�}�b�`���C�N�֐��ɓn��
        friendIdxListLen = 0;
        
        // �C���f�b�N�X�����o�����ƂȂ�C���f�b�N�X���X�g���쐬����
        for (i = 0; i < DWCi_GetFriendListLen(); i++){
            idxList[i] = i;
        }

        // �C���f�b�N�X���X�g���烉���_���ɃC���f�b�N�X�l�𔲂��o���A
        // �F�B�C���f�b�N�X���X�g���쐬����
        for (i = 0; i < DWCi_GetFriendListLen(); i++){
            u32 randIdx = DWCi_GetMathRand32((u32)(DWCi_GetFriendListLen()-i));

#ifdef DWC_MATCH_ACCEPT_NO_FRIEND
            // �����ς݃o�f�B�łȂ��Ă��S�ăZ�b�g����
            tmpFriendIdxList[i] = idxList[randIdx];
            friendIdxListLen++;
            
#else
            // �����ȗF�B���͑I�΂�Ȃ��悤�ɂ���
            if (DWCi_Acc_IsValidFriendData(&(DWCi_GetFriendList())[idxList[randIdx]])){
                tmpFriendIdxList[i] = idxList[randIdx];
                friendIdxListLen++;
            }
#endif

            // �����o���ꂽ�C���f�b�N�X������̃C���f�b�N�X���X�g��O�ɋl�߂�
            for (j = randIdx; j < DWCi_GetFriendListLen()-i-1; j++){
                idxList[j] = idxList[j+1];
            }
        }
            
        // �}�b�`���C�N�֐��Ăяo��
        DWCi_ConnectToFriendsAsync(tmpFriendIdxList, friendIdxListLen,
                                   (u8)(numEntry-1),
                                   distantFriend,
                                   DWCi_MatchedCallback, NULL,
                                   evalCallback, evalParam);
    }

    // [arakit] main 051025
    return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�N���֐�
  �����@�FmaxEntry          �ő�ڑ��l���i�������܂ށj
          matchedCallback   �}�b�`���C�N�����R�[���o�b�N�B�P�l�ڑ����邲�ƂɌĂ΂��B
          matchedParam      ��L�R�[���o�b�N�p�p�����[�^
          newClientCallback �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
          newClientParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�T�[�o�𗧂ĂăN���C�A���g����̗v��������΁A�R�l�N�V������
          �ڑ��E�ؒf�������s���B
 *---------------------------------------------------------------------------*/
// [arakit] main 051025 051027
BOOL DWC_SetupGameServer(u8  maxEntry,
                         DWCMatchedSCCallback matchedCallback,
                         void* matchedParam,
                         DWCNewClientCallback newClientCallback,
                         void* newClientParam)
{

    SDK_ASSERT(stpDwcCnt);
    SDK_ASSERT((maxEntry > 1) && (maxEntry <= DWC_MAX_CONNECTIONS));
    SDK_ASSERT(matchedCallback);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_SetupGameServer() was called!!\n");

    if (DWCi_IsError() || (stpDwcCnt->state != DWC_STATE_ONLINE)){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        // [arakit] main 051025
        return FALSE;
    }

    // gt2�R�l�N�V�������X�g��gt2�R�l�N�V������񃊃X�g��������
    DWCi_ClearGT2ConnectionList();

    stpDwcCnt->matchedSCCallback = matchedCallback;
    stpDwcCnt->matchedSCParam    = matchedParam;

    // �T�[�o�̏ꍇ�͕K��AID = 0
    stpDwcCnt->aid = 0;

    DWCi_SetState(DWC_STATE_MATCHING);

    // �}�b�`���C�N�֐��Ăяo��
    // maxEntry�̓}�b�`���C�N�������ł͎������܂܂Ȃ��ڑ��l����\���̂�-1���ēn��
    // [arakit] main 051027
    DWCi_SetupGameServer((u8)(maxEntry-1),
                         DWCi_MatchedCallback, NULL,
                         newClientCallback, newClientParam);

    // [arakit] main 051025
    return TRUE;
}


/*---------------------------------------------------------------------------*
  �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���g�N���֐�
  �����@�FserverIndex       �ڑ���T�[�o�̗F�B���X�g�C���f�b�N�X
          matchedCallback   �}�b�`���C�N�����R�[���o�b�N�B�P�l�ڑ����邲�ƂɌĂ΂��B
          matchedParam      ��L�R�[���o�b�N�p�p�����[�^
          newClientCallback �V�K�ڑ��N���C�A���g�ʒm�R�[���o�b�N
          newClientParam    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�֐��Ăяo�������B�}�b�`���C�N�̌��ʂ��R�[���o�b�N�ŕԂ��Ă���B
          FALSE:�{�֐����Ă�ŗǂ���Ԃł͂Ȃ��B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�F�F�B���X�g�̃C���f�b�N�X�Őڑ�����T�[�o���w�肵�A�����֐ڑ�����B
          �܂��A�T�[�o�֐V���ɐڑ������N���C�A���g������΁A���̃N���C�A���g��
          �̐ڑ��������s���B
 *---------------------------------------------------------------------------*/
// [arakit] main 051025
BOOL DWC_ConnectToGameServerAsync(int serverIndex,
                                  DWCMatchedSCCallback matchedCallback,
                                  void* matchedParam,
                                  DWCNewClientCallback newClientCallback,
                                  void* newClientParam)
{
    int profileID;
    int buddyIdx = -1;
    GPBuddyStatus status;
    // [arakit] main 051025
    DWCError dwcError;

    SDK_ASSERT(stpDwcCnt);
    SDK_ASSERT(matchedCallback);

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToGameServerAsync() was called!!\n");

    if (DWCi_IsError() || (stpDwcCnt->state != DWC_STATE_ONLINE)){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        // [arakit] main 051025
        return FALSE;
    }

    // gt2�R�l�N�V�������X�g��gt2�R�l�N�V������񃊃X�g��������
    DWCi_ClearGT2ConnectionList();

    stpDwcCnt->matchedSCCallback = matchedCallback;
    stpDwcCnt->matchedSCParam    = matchedParam;

    DWCi_SetState(DWC_STATE_MATCHING);

    if (!(profileID = DWCi_GetProfileIDFromList(serverIndex)) ||
        !gpIsBuddy(&stpDwcCnt->gpObj, profileID)){
        // �w�肳�ꂽ�C���f�b�N�X���v���t�@�C��ID�������Ă��Ȃ��A��������
        // ���肪�o�f�B�łȂ������ꍇ�̓G���[�ŃR�[���o�b�N���Ăяo��
        // [arakit] main 051025
        DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is not buddy.\n", profileID);
        dwcError = DWC_ERROR_NOT_FRIEND_SERVER;
        goto error;
        // [arakit] main 051025
    }

    // �G���[�͗L�蓾�Ȃ�
    (void)gpGetBuddyIndex(&stpDwcCnt->gpObj, profileID, &buddyIdx);
    (void)gpGetBuddyStatus(&stpDwcCnt->gpObj, buddyIdx, &status);
    
    if (status.status != DWC_STATUS_MATCH_SC_SV){
        // �w�肳�ꂽ���肪�T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂��グ��
        // ���Ȃ������ꍇ�̓G���[�ŃR�[���o�b�N���Ăяo��
        // [arakit] main 051025
        DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is not game server.\n", profileID);
        dwcError = DWC_ERROR_NOT_FRIEND_SERVER;
        goto error;
        // [arakit] main 051025
    }

    // [arakit] main 051024
    {
        char valueStr[4];
        u8   maxEntry, numEntry;

        valueStr[0] = '0';  // �O�̂���0�l�ŏ�����
            
        // �T�[�o�̍ő�ڑ��l���̐ݒ��ǂݏo��
        DWC_GetCommonValueString(DWC_GP_SSTR_KEY_MATCH_SC_MAX, valueStr,
                                 status.statusString, '/');
        maxEntry = (u8)strtoul(valueStr, NULL, 10);

        // �T�[�o�̌��ݐڑ��l���̐ݒ��ǂݏo��
        DWC_GetCommonValueString(DWC_GP_SSTR_KEY_MATCH_SC_NUM, valueStr,
                                 status.statusString, '/');
        numEntry = (u8)strtoul(valueStr, NULL, 10);

        if (numEntry == maxEntry){
            // [arakit] main 051025
            DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is fully occupied.\n", profileID);
            dwcError = DWC_ERROR_SERVER_FULL;
            goto error;
            // [arakit] main 051025
        }
    }
    // [arakit] main 051024

    // �}�b�`���C�N�֐��Ăяo��
    DWCi_ConnectToGameServerAsync(profileID, DWCi_MatchedCallback, NULL,
                                  newClientCallback, newClientParam);

    // [arakit] main 051025
    return TRUE;

    // [arakit] main 051025
error:
    // �G���[����
    DWCi_SetError(dwcError, 0);
            
    stpDwcCnt->matchedSCCallback(dwcError,
                                 FALSE, TRUE, FALSE,
                                 0, stpDwcCnt->matchedSCParam);

    if ((stpDwcCnt != NULL) && (stpDwcCnt->state == DWC_STATE_MATCHING)){
        // �}�b�`���C�N��Ԃ̂܂܂Ȃ�I�����C����Ԃɖ߂�
        DWCi_SetState(DWC_STATE_ONLINE);
        // GP�X�e�[�^�X���I�����C���ɂ���B
        (void)DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);
    }

    return TRUE;
    // [arakit] main 051025
}


/*---------------------------------------------------------------------------*
  �R�l�N�V�����N���[�Y�R�[���o�b�N�ݒ�֐�
  �����@�Fcallback �R�l�N�V�����P���N���[�Y�����x�ɌĂяo�����R�[���o�b�N
          param    ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F�R�l�N�V�����N���[�Y�R�[���o�b�N��ݒ肷��
 *---------------------------------------------------------------------------*/
BOOL DWC_SetConnectionClosedCallback(DWCConnectionClosedCallback callback, void* param)
{

    if (!stpDwcCnt) return FALSE;

    stpDwcCnt->closedCallback = callback;
    stpDwcCnt->closedParam    = param;

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �S�R�l�N�V�����N���[�Y�֐� (obsolete function)
  �����@�F�Ȃ�
  �߂�l�F0�ȏ�Ȃ琬���A���̐��Ȃ玸�s�B
          0 :�N���[�Y�J�n�B�N���[�Y����������������R�[���o�b�N���Ă΂��B
          1 :�ڑ��z�X�g��0�Ȃ̂ŁA�N���[�Y�����͍s�������A�R�[���o�b�N�͌Ă΂�Ȃ��B
             �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�̂݁B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
  �p�r�@�F�ڑ����̃R�l�N�V������S�ăN���[�Y����B
          �P�̃R�l�N�V�������N���[�Y���邲�ƂɁA
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ɂ����āA���ɐڑ����̃z�X�g��
          �Ȃ��ꍇ�́A�I���������s�������ŁA�R�[���o�b�N�͌Ă΂�Ȃ��B
          ��DWC_CloseAllConnectionsHard()���g���ĉ�����
 *---------------------------------------------------------------------------*/
int  DWC_CloseConnectionsAsync(void)
{

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_CloseConnectionsAsync() was called!!\n");

    if (!stpDwcCnt || DWCi_IsError() ||
        ((stpDwcCnt->state != DWC_STATE_MATCHING) && (stpDwcCnt->state != DWC_STATE_CONNECTED))){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return -1;
    }

    if (!stpDwcCnt->matchcnt.gt2NumConnection){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�p�ɂ����ŏI���������s��
        // �i�ڑ��z�X�g��0�ŃN���[�Y���Ăׂ�̂̓T�[�o�N���C�A���g�}�b�`���C�N
        // �̃T�[�o�����j
        DWC_Printf(DWC_REPORTFLAG_TRANSPORT, "Closed 0 connection.\n");

        // GP�X�e�[�^�X���I�����C���ɂ���B
        (void)DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);

        // NN�g�p�̈���J������
        DWCi_NNFreeNegotiateList();

        DWCi_SetState(DWC_STATE_ONLINE);  // ���O�C����Ԃɖ߂�
            
        return 1;
    }

    // �ڑ����̑S�Ẵz�X�g�ɑ΂��ăR�l�N�V�������N���[�Y����
    gt2CloseAllConnections(stpDwcCnt->gt2Socket);

    return 0;
}


/*---------------------------------------------------------------------------*
  �S�R�l�N�V���������N���[�Y�֐�
  �����@�F�Ȃ�
  �߂�l�F0�ȏ�Ȃ琬���A���̐��Ȃ玸�s�B
          0 :�N���[�Y���s�B
          1 :�ڑ��z�X�g��0�Ȃ̂ŁA�N���[�Y�����͍s�������A�R�[���o�b�N�͌Ă΂�Ȃ��B
             �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�̂݁B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
  �p�r�@�F�ڑ����̃R�l�N�V������S�ăN���[�Y����B
          �N���[�Y�����͖{�֐����Ŋ������A�{�֐��𔲂���O�ɁA
          �P�̃R�l�N�V�������N���[�Y���邲�ƂɁA
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ɂ����āA���ɐڑ����̃z�X�g��
          �Ȃ��ꍇ�́A�I���������s�������ŁA�R�[���o�b�N�͌Ă΂�Ȃ��B
 *---------------------------------------------------------------------------*/
int  DWC_CloseAllConnectionsHard(void)
{

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_CloseAllConnectionsHard() was called!!\n");

    if (!stpDwcCnt || DWCi_IsError() ||
        ((stpDwcCnt->state != DWC_STATE_MATCHING) && (stpDwcCnt->state != DWC_STATE_CONNECTED))){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return -1;
    }

    if (!stpDwcCnt->matchcnt.gt2NumConnection){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�p�ɂ����ŏI���������s��
        // �i�ڑ��z�X�g��0�ŃN���[�Y���Ăׂ�̂̓T�[�o�N���C�A���g�}�b�`���C�N
        // �̃T�[�o�����j
        DWC_Printf(DWC_REPORTFLAG_TRANSPORT, "Closed 0 connection.\n");

        // GP�X�e�[�^�X���I�����C���ɂ���B
        (void)DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);

        // NN�g�p�̈���J������
        DWCi_NNFreeNegotiateList();

        DWCi_SetState(DWC_STATE_ONLINE);  // ���O�C����Ԃɖ߂�
            
        return 1;
    }

    stpDwcCnt->ownCloseFlag = TRUE;   // �Q�[��������N���[�Y���ꂽ���Ƃ��L�^

    // �ڑ����̑S�Ẵz�X�g�ɑ΂��ăR�l�N�V�������N���[�Y����
    gt2CloseAllConnectionsHard(stpDwcCnt->gt2Socket);

    stpDwcCnt->ownCloseFlag = FALSE;  // �t���O�����낷

    return 0;
}


/*---------------------------------------------------------------------------*
  �R�l�N�V���������N���[�Y�֐�
  �����@�Faid �N���[�Y�������z�X�g��AID
  �߂�l�F0 :�N���[�Y���s�B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
          -2:���ɃN���[�Y�ς݂�AID���w�肳�ꂽ�̂ŉ������Ȃ������B
  �p�r�@�F�w���AID�Ƃ̃R�l�N�V�����������N���[�Y����B
          �N���[�Y�����͖{�֐����Ŋ������A�{�֐��𔲂���O��
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �d����؂�Ȃǂ̗��R�ŒʐM�s�\�ɂȂ����z�X�g�ɑ΂��A�R�l�N�V������
          �N���[�Y����Ƃ����悤�ȁA�ُ��ԏ����̗p�r�ł݂̂��g�p�������B
 *---------------------------------------------------------------------------*/
int  DWC_CloseConnectionHard(u8 aid)
{
    GT2Connection connection;

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_CloseConnectionHard() was called!! aid = %d.\n", aid);

    if (!stpDwcCnt || DWCi_IsError() ||
        ((stpDwcCnt->state != DWC_STATE_MATCHING) && (stpDwcCnt->state != DWC_STATE_CONNECTED))){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return -1;
    }
    
    if (!(connection = DWCi_GetGT2Connection(aid))){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "No connection!\n");
        return -2;
    }
    
    gt2CloseConnectionHard(connection);

    return 0;
}


/*---------------------------------------------------------------------------*
  �r�b�g�}�b�v�w��R�l�N�V���������N���[�Y�֐�
  �����@�Fbitmap �N���[�Y�������z�X�g��AID�r�b�g�}�b�v�ւ̃|�C���^�B
                 ���ۂɃN���[�Y�ɐ�������AID�̃r�b�g�݂̂𗧂ĂĕԂ��B
                 �����̃r�b�g�͗����Ă��Ă��K�����낷�B
  �߂�l�F0 :�N���[�Y�J�n�B�N���[�Y��������������x�ɃR�[���o�b�N���Ă΂��B
          -1:�ڑ�������łȂ��A�������̓G���[�������Ȃ̂ŉ������Ȃ������B
          -2:�S�Ă�AID�����ɃN���[�Y�ς݂������̂ŉ������Ȃ������B
  �p�r�@�F�w���AID�Ƃ̃R�l�N�V�����������N���[�Y����B
          �N���[�Y�����͖{�֐����Ŋ������A�{�֐��𔲂���O��
          DWC_SetConnectionClosedCallback()�Őݒ肵���R�[���o�b�N�֐���
          �Ăяo�����BWi-Fi�R�l�N�V�����ɂ̓��O�C�������܂܂ƂȂ�B
          ���̃N���[�Y�͑���z�X�g�ɂ��ʒm�����B
          �d����؂�Ȃǂ̗��R�ŒʐM�s�\�ɂȂ����z�X�g�ɑ΂��A�R�l�N�V������
          �N���[�Y����Ƃ����悤�ȁA�ُ��ԏ����̗p�r�ł݂̂��g�p�������B
 *---------------------------------------------------------------------------*/
int  DWC_CloseConnectionHardBitmap(u32* bitmap)
{
    u8  aid;
    u32 bitmask;

    // [arakit] main 051013
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "!!DWC_CloseConnectionHardBitmap() was called!! AID bitmap = 0x%x\n",
               (bitmap == NULL) ? 0 : *bitmap);

    if (!stpDwcCnt || !bitmap || DWCi_IsError() ||
        ((stpDwcCnt->state != DWC_STATE_MATCHING) && (stpDwcCnt->state != DWC_STATE_CONNECTED))){
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return -1;
    }

    for (aid = 0; aid < DWC_MAX_CONNECTIONS; aid++){
        bitmask = 1U << aid;

        if (*bitmap & bitmask){
            if (aid == DWC_GetMyAID()){
                *bitmap &= ~bitmask;       // ������AID�̃r�b�g�͉��낷
            }
            else if (DWC_CloseConnectionHard(aid)){
                *bitmap &= ~bitmask;  // �N���[�Y���s
            }
        }
    }

    if (!*bitmap) return -2;  // �S�đ��݂��Ȃ��R�l�N�V����������
    return 0;  // �N���[�Y����
}


/*---------------------------------------------------------------------------*
  �ڑ��z�X�g���擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�l�b�g���[�N���\�����Ă���z�X�g���i�������܂ށj�B
          �I�t���C������0��Ԃ��B
  �p�r�@�F���b�V���^�l�b�g���[�N�\���z�X�g����Ԃ�
 *---------------------------------------------------------------------------*/
int DWC_GetNumConnectionHost(void)
{

    if (!stpDwcCnt) return 0;

    if ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
        (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
        // �r���Q������̏ꍇ�͗L����aid�̐��̂ݕԂ�
        return DWCi_GetNumValidConnection()+1;
    }
    else {
        // �}�b�`���C�N���ł����݂̎��ېڑ�����Ԃ�
        return DWCi_GetNumAllConnection()+1;
    }
}


/*---------------------------------------------------------------------------*
  ������AID�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F������AID
  �p�r�@�F������AID�i���b�V���^�l�b�g���[�N���ŌŗL�̃z�X�gID�j��Ԃ�
 *---------------------------------------------------------------------------*/
u8  DWC_GetMyAID(void)
{

    if (!stpDwcCnt) return 0;

    return stpDwcCnt->aid;
}


/*---------------------------------------------------------------------------*
  �ڑ����z�X�g��AID���X�g�擾�֐�
  �����@�FaidList AID���X�g�ւ̃|�C���^
  �߂�l�FAID���X�g���i�������܂ށj
  �p�r�@�FAID���X�g�ւ̃|�C���^���擾����i�R�s�[�͂��Ȃ��j�B
          AID���X�g�́A���ݗL����AID���O���珇�ɋl�߂�ꂽ���X�g�ŁA
          ���̗v�f���͎������܂߂��ڑ����z�X�g���ɂȂ�B
 *---------------------------------------------------------------------------*/
int DWC_GetAIDList(u8** aidList)
{

    if (!stpDwcCnt) return 0;

    *aidList = stpDwcCnt->matchcnt.aidList;

    if ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
        (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
        // �r���Q������̏ꍇ�͗L����aid�̂ݕԂ�
        return DWCi_GetValidAIDList(aidList);
    }
    else {
        return DWCi_GetAllAIDList(aidList);
    }
}


/*---------------------------------------------------------------------------*
  �ڑ����z�X�g��AID�r�b�g�}�b�v�擾�֐�
  �����@�F�Ȃ�
  �߂�l�FAID�r�b�g�}�b�v�B�I�t���C������0��Ԃ��B
  �p�r�@�F�ڑ����z�X�g��AID�r�b�g�}�b�v���擾����B
 *---------------------------------------------------------------------------*/
u32 DWC_GetAIDBitmap(void)
{
    u8* pAidList;
    u32 bitmap = 0;
    int numHost;

    if (!stpDwcCnt) return 0;

    numHost = DWC_GetAIDList(&pAidList);

    // AID�r�b�g�}�b�v�擾
    bitmap = DWCi_GetAIDBitmapFromList(pAidList, numHost);

    return bitmap;
}


/*---------------------------------------------------------------------------*
  AID�L������֐�
  �����@�Faid AID
  �߂�l�FTRUE:AID���L���AFALSE:AID������
  �p�r�@�F�w�肳�ꂽAID���L�����ǂ����i�����ɐڑ�����Ă��邩�ǂ����j�𒲂ׂ�B
          ������AID���w�肵���ꍇ��FALSE��Ԃ��B
          �}�b�`���C�N���̐V�K�ڑ��N���C�A���g��AID�͖����Ɣ��肷��B
 *---------------------------------------------------------------------------*/
BOOL DWC_IsValidAID(u8 aid)
{

    if (!stpDwcCnt) return FALSE;

    // �R�l�N�V�����������Ă��Ȃ��Ă��A�Q�[������̓}�b�`���C�N����������
    // �z�X�g�����L���ƌ����Ȃ��悤�ɂ���
    if (!(stpDwcCnt->matchcnt.validAidBitmap & (1 << aid))) return FALSE;

    return DWCi_IsValidAID(aid);
}


/*---------------------------------------------------------------------------*
  FriendsMatch���C�u������Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�FDWCState�^�񋓎q
  �p�r�@�FFriendsMatch���C�u�����̏�����Ԃ��擾����
 *---------------------------------------------------------------------------*/
DWCState DWC_GetState(void)
{

    if (!stpDwcCnt){
        return DWC_STATE_INIT;
    }
    else {
        return stpDwcCnt->state;
    }
}


/*---------------------------------------------------------------------------*
  �\�P�b�g�G���[�擾�֐�
  �����@�F�Ȃ�
  �߂�l�FGT2�\�P�b�g�G���[���
  �p�r�@�FGameSpy gt2 SDK �̃\�P�b�g�G���[��ʂ��擾����
 *---------------------------------------------------------------------------*/
int DWC_GetLastSocketError(void)
{

    return stLastSocketError;
}


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  GT2�������֐�
  �����@�F�Ȃ�
  �߂�l�FGT2�̏������ʌ^
  �p�r�@�FGT2�̏��������s��
 *---------------------------------------------------------------------------*/
GT2Result DWCi_GT2Startup(void)
{
    u16 baseport;
    GT2Result gt2Result;

    if (stpDwcCnt->gt2Socket){
        // ������gt2�\�P�b�g�����ɂ���̂ɂ����ɗ����ꍇ�͉������Ȃ�
        DWC_Printf(DWC_REPORTFLAG_WARNING, "gt2Socket is already made.\n");
        return GT2Success;
    }

    // GT2�\�P�b�g�Ɏg�p����|�[�g�������_���ɐ�������
    baseport = (u16)(0xc000+DWCi_GetMathRand32(0x4000));

    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "--- Private port = %d ---\n", baseport);

    // �\�P�b�g�쐬
    gt2Result = gt2CreateSocket(&stpDwcCnt->gt2Socket,
                                gt2AddressToString(0, baseport, NULL),
                                stpDwcCnt->gt2SendBufSize,
                                stpDwcCnt->gt2RecvBufSize,
                                DWCi_GT2SocketErrorCallback);
    if (DWCi_HandleGT2Error(gt2Result)) return gt2Result;

    // �ڑ��v����M�R�[���o�b�N�֐��Z�b�g
    // dwc_match.c�Œ�`
    gt2Listen(stpDwcCnt->gt2Socket, DWCi_GT2ConnectAttemptCallback);

    // GT2�F���s�\���b�Z�[�W��M�R�[���o�b�N�֐��o�^
    // dwc_match.c�Œ�`
    gt2SetUnrecognizedMessageCallback(stpDwcCnt->gt2Socket, DWCi_GT2UnrecognizedMessageCallback);

    return gt2Result;
}


/*---------------------------------------------------------------------------*
  GT2Connection�擾�֐�
  �����@�Faid �z�X�g��AID
  �߂�l�FGT2Connection�BAID�ɑΉ�����R�l�N�V�������Ȃ��ꍇ��NULL��Ԃ�
  �p�r�@�FAID�ɑΉ�����GT2Connection�l���擾����
 *---------------------------------------------------------------------------*/
GT2Connection DWCi_GetGT2Connection(u8 aid)
{
    int i;

    if (!stpDwcCnt) return NULL;

    for (i = 0; i < DWC_MAX_CONNECTIONS; i++){
        if (stGt2ConnectionList[i] &&
            (((DWCConnectionInfo *)gt2GetConnectionData(stGt2ConnectionList[i]))->aid == aid)){
            return stGt2ConnectionList[i];
        }
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  GT2Connection�����AID�擾�֐�
  �����@�Fconnection GT2Connection�^
  �߂�l�F�R�l�N�V������AID
  �p�r�@�FGT2Connection����AID���擾����
 *---------------------------------------------------------------------------*/
u8  DWCi_GetConnectionAID(GT2Connection connection)
{

    return ((DWCConnectionInfo *)gt2GetConnectionData(connection))->aid;
}


/*---------------------------------------------------------------------------*
  GT2Connection����̃C���f�b�N�X�擾�֐�
  �����@�Fconnection GT2Connection�^
  �߂�l�F�R�l�N�V�����́A�R�l�N�V�������X�g���̃C���f�b�N�X
  �p�r�@�FGT2Connection����R�l�N�V�������X�g���ł̃C���f�b�N�X���擾����
 *---------------------------------------------------------------------------*/
u8  DWCi_GetConnectionIndex(GT2Connection connection)
{

    return ((DWCConnectionInfo *)gt2GetConnectionData(connection))->index;
}


/*---------------------------------------------------------------------------*
  GT2Connection����̃��[�U�f�[�^�擾�֐�
  �����@�Fconnection GT2Connection�^
  �߂�l�F�R�l�N�V�����ŗL�̃��[�U�ݒ�f�[�^�ւ̃|�C���^
  �p�r�@�FGT2Connection����R�l�N�V�����ŗL�̃��[�U�ݒ�f�[�^�ւ̃|�C���^��
          �擾����
 *---------------------------------------------------------------------------*/
void* DWCi_GetConnectionUserData(GT2Connection connection)
{

    return ((DWCConnectionInfo *)gt2GetConnectionData(connection))->param;
}


/*---------------------------------------------------------------------------*
  GT2�R�l�N�V�������X�g�̋󂫃C���f�b�N�X�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F���X�g�̋󂫃C���f�b�N�X�B�󂫂��������-1��Ԃ��B
  �p�r�@�FGT2�R�l�N�V�������X�g�̋󂫃C���f�b�N�X���擾����B
 *---------------------------------------------------------------------------*/
int  DWCi_GT2GetConnectionListIdx(void)
{
    int i;

    for (i = 0; i < DWC_MAX_CONNECTIONS; i++){
        if (!stGt2ConnectionList[i]){
            return i;
        }
    }

    return -1;
}


/*---------------------------------------------------------------------------*
  GT2�R�l�N�V�������X�g�y��GT2�R�l�N�V������񃊃X�g�������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FGT2�R�l�N�V�������X�g�y��GT2�R�l�N�V������񃊃X�g������������
 *---------------------------------------------------------------------------*/
void DWCi_ClearGT2ConnectionList(void)
{
    
    MI_CpuClear32(stGt2ConnectionList, sizeof(GT2Connection)*DWC_MAX_CONNECTIONS);
    MI_CpuClear32(stConnectionInfoList, sizeof(DWCConnectionInfo)*DWC_MAX_CONNECTIONS);
}


/*---------------------------------------------------------------------------*
  GT2�R�l�N�V�������X�g�|�C���^�擾�֐�
  �����@�Findex GT2�R�l�N�V�������X�g�̃C���f�b�N�X
  �߂�l�FGT2�R�l�N�V�������X�g�ւ̃|�C���^
  �p�r�@�F�C���f�b�N�X����GT2�R�l�N�V�������X�g�̗v�f�ւ̃|�C���^���擾����
 *---------------------------------------------------------------------------*/
GT2Connection* DWCi_GetGT2ConnectionByIdx(int index)
{

    return &stGt2ConnectionList[index];
}


/*---------------------------------------------------------------------------*
  GT2�R�l�N�V�������X�g�|�C���^�擾�֐�
  �����@�FprofileID �z�X�g�̃v���t�@�C��ID
          numHost   ��������z�X�g��
  �߂�l�FGT2�R�l�N�V�������X�g�ւ̃|�C���^�B
          �v���t�@�C��ID�ɑΉ�����R�l�N�V�������Ȃ��ꍇ��NULL��Ԃ��B
  �p�r�@�F�v���t�@�C��ID����GT2�R�l�N�V�������X�g�̗v�f�ւ̃|�C���^���擾����
 *---------------------------------------------------------------------------*/
GT2Connection* DWCi_GetGT2ConnectionByProfileID(int profileID, int numHost)
{
    u8 i;

    for (i = 0; i < numHost; i++){
        if (stpDwcCnt->matchcnt.sbPidList[i] == profileID) break;
    }

    if (i >= numHost) return NULL;  // �w��̃z�X�g�Ȃ�

    return DWCi_GetGT2ConnectionByIdx(DWCi_GetConnectionIndex(DWCi_GetGT2Connection(stpDwcCnt->matchcnt.aidList[i])));
}


/*---------------------------------------------------------------------------*
  GT2�R�l�N�V������񃊃X�g�|�C���^�擾�֐�
  �����@�Findex GT2�R�l�N�V�������X�g�̃C���f�b�N�X
  �߂�l�FGT2�R�l�N�V������񃊃X�g�ւ̃|�C���^
  �p�r�@�FGT2�R�l�N�V�������X�g�̃C���f�b�N�X�ɑΉ�����i�����C���f�b�N�X�́j
          GT2�R�l�N�V������񃊃X�g�̗v�f�ւ̃|�C���^���擾����
 *---------------------------------------------------------------------------*/
DWCConnectionInfo* DWCi_GetConnectionInfoByIdx(int index)
{

    return &stConnectionInfoList[index];
}


/*---------------------------------------------------------------------------*
  AID�L������֐��@�����g�p��
  �����@�Faid AID
  �߂�l�FTRUE:AID���L���AFALSE:AID������
  �p�r�@�F�w�肳�ꂽAID���L�����ǂ����i�ڑ�����Ă��邩�ǂ����j�𒲂ׂ�B
          ������AID���w�肵���ꍇ��FALSE��Ԃ��B
          �O�����J�łƈقȂ�A�}�b�`���C�N���̐V�K�ڑ��N���C�A���g��AID��
          �L���Ɣ��肷��B
 *---------------------------------------------------------------------------*/
BOOL DWCi_IsValidAID(u8 aid)
{
    int i;

    for (i = 0; i < DWC_MAX_CONNECTIONS; i++){
        if (stGt2ConnectionList[i] &&
            (((DWCConnectionInfo *)gt2GetConnectionData(stGt2ConnectionList[i]))->aid == aid)){
            return TRUE;
        }
    }

    return FALSE;
}


//----------------------------------------------------------------------------
// function - static
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  DCF�ʐM�����I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FDCF���ؒf���ꂽ�Ƃ��ɌĂяo����A�K�v�ȏ���������ꍇ�ɂ����ɋL�q����B
 *---------------------------------------------------------------------------*/
static void DWCs_ForceShutdown( void )
{
    // �����I�ɌĂ�ł��܂��Ă����܂�Ȃ��֐��������ŌĂяo���B
}


/*---------------------------------------------------------------------------*
  FriendsMatch���C�u������ԃZ�b�g�֐�
  �����@�FDWCState�^�񋓎q
  �߂�l�F�Ȃ�
  �p�r�@�FFriendsMatch���C�u�����̏�����Ԃ�ݒ肷��
 *---------------------------------------------------------------------------*/
static void DWCi_SetState(DWCState state)
{

    stpDwcCnt->lastState = stpDwcCnt->state;
    stpDwcCnt->state     = state;
}


/*---------------------------------------------------------------------------*
  AID�����֐�
  �����@�Faid ���X�g�����������AID
  �߂�l�F�����ꂽAID�ɑΉ�����v���t�@�C��ID
  �p�r�@�F�w�肳�ꂽAID��AID���X�g����������đO�ɋl�߁A�����ɃC���f�b�N�X��
          �ˑ��֌W�ɂ���v���t�@�C�����X�g�����O�ɋl�߂�
 *---------------------------------------------------------------------------*/
static int DWCi_DeleteAID(u8 aid)
{
    u8* pAidList;
    int numHost;
    int i;

    numHost = DWCi_GetAllAIDList(&pAidList);

    for (i = 0; i < numHost; i++){
        if (pAidList[i] == aid) break;
    }

    if (i == numHost) return 0;  // ���݂��Ȃ�AID���w�肳�ꂽ

    return DWCi_DeleteHostByIndex(i, numHost);
}


/*---------------------------------------------------------------------------*
  AID�r�b�g�}�b�v�擾�֐�
  �����@�FaidList    AID�r�b�g�}�b�v�擾�����X�g
          aidListLen AID���X�g���i�ڑ��z�X�g���{�P�j
  �߂�l�FAID�r�b�g�}�b�v
  �p�r�@�F�w�肳�ꂽAID���X�g����AID�r�b�g�}�b�v���쐬����
 *---------------------------------------------------------------------------*/
static u32 DWCi_GetAIDBitmapFromList(u8* aidList, int aidListLen)
{
    u32 bitmap = 0;
    int i;

    for (i = 0; i < aidListLen; i++){
        bitmap |= 1 << aidList[i];
    }

    return bitmap;
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

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Main, GP error %d\n", result);

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

    // �x�[�X�G���[�R�[�h��ǉ����Ċe�폈�����̌ŗL�G���[�������Ăяo��
    switch (stpDwcCnt->state){
    case DWC_STATE_AVAILABLE_CHECK:
        errorCode += DWC_ECODE_SEQ_LOGIN+DWC_ECODE_GS_GP;
        DWCi_StopLogin(dwcError, errorCode);
        break;
    case DWC_STATE_LOGIN:
        errorCode += DWC_ECODE_SEQ_LOGIN+DWC_ECODE_GS_GP;
        // [arakit] main 051027
        if (stpDwcCnt->logcnt.state < DWC_LOGIN_STATE_REMOTE_AUTH){
            // ���O�C������gpConnect�J�n�O�Ȃ�A���O�C�����~����
            // �Q�[�����̃R�[���o�b�N���Ăяo��
            DWCi_StopLogin(dwcError, errorCode);
        }
        else {
            // ���O�C������gpConnect�J�n��Ȃ�A���O�C���̒�~�ƃQ�[������
            // �R�[���o�b�N�Ăяo���́AGPConnectCallback���ōs�Ȃ���̂�
            // �����ł͉������Ȃ��i�G���[���Z�b�g����Ə������i�܂Ȃ��̂ł�������Ȃ��j
            DWC_Printf(DWC_REPORTFLAG_ERROR, "Not handle an error here.\n");
            // [arakit] main 051027
        }
        break;
    case DWC_STATE_MATCHING:
        // �T�[�o�N���C�A���g�}�b�`���C�N�Ŋ���STATE_CONNECTED�ɂȂ��Ă���
        // ��Ԃł̃}�b�`���C�N�G���[���́A�ʂɃ}�b�`���C�N�����R�[���o�b�N��
        // �Ԃ��Ȃ��Ă��ǂ�
        errorCode += DWC_ECODE_SEQ_MATCH+DWC_ECODE_GS_GP;
        DWCi_StopMatching(dwcError, errorCode);
        break;
    case DWC_STATE_UPDATE_SERVERS:
        errorCode += DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_GP;
        break;
    default:
        errorCode += DWC_ECODE_SEQ_ETC+DWC_ECODE_GS_GP;
        break;
    }

    // �F�B�Ǘ������͕K���I��
    DWCi_StopFriendProcess(dwcError, errorCode);

    return result;
}


/*---------------------------------------------------------------------------*
  GT2Result�Ή��G���[�����֐�
  �����@�Fresult GT2�̏������ʌ^
  �߂�l�FGT2�̏������ʌ^�i���������̂܂ܕԂ��j
  �p�r�@�FGT2Result�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���āA�Ή�����R�[���o�b�N���Ăяo��
 *---------------------------------------------------------------------------*/
static GT2Result DWCi_HandleGT2Error(GT2Result result)
{
    int errorCode;
    DWCError dwcError;

    if (result == GT2Success) return GT2Success;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Main, GT2 error %d\n", result);

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

    // ���̓��O�C������gt2CreateSocket�̎������Ă΂�Ȃ�
    if (dwcError){
        // �x�[�X�G���[�R�[�h��ǉ�
        errorCode += DWC_ECODE_SEQ_LOGIN+DWC_ECODE_GS_GT2;
        DWCi_StopLogin(dwcError, errorCode);
    }

    return result;
}


/*---------------------------------------------------------------------------*
  ���O�C�������R�[���o�b�N�֐�
  �����@�Ferror     DWC�G���[���
          profileID ���O�C���̌��ʓ���ꂽ�v���t�@�C��ID
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F���O�C�������R�[���o�b�N�B
          ���O�C����������dwc_login.c���̊֐�����Ă΂��B
 *---------------------------------------------------------------------------*/
static void DWCi_LoginCallback(DWCError error, int profileID, void *param)
{
#pragma unused(param)

    if (error == DWC_ERROR_NONE){
        // ���O�C�������Ȃ��Ԃ��I�����C���ɂ���
        stpDwcCnt->profileID = profileID;
        DWCi_SetState(DWC_STATE_ONLINE);

        // gpProcess()�Ăяo���񐔃J�E���^������������
        DWCi_InitGPProcessCount();
    }
    else {
        // ���O�C�����s�̏ꍇ
        DWCi_SetState(DWC_STATE_INIT);
    }

    // �Q�[������^����ꂽ�R�[���o�b�N�֐����Ăяo��
    if ( stpDwcCnt->loginCallback != NULL )
    {
        stpDwcCnt->loginCallback(error, profileID, stpDwcCnt->loginParam);
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�������������R�[���o�b�N�֐�
  �����@�Ferror     DWC�G���[���
          isChanged TRUE:�F�B���X�g���ύX���ꂽ�AFALSE:�F�B���X�g�ɕύX�Ȃ�
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B���X�g�������������R�[���o�b�N�B
          �F�B���X�g����������������dwc_friend.c���̊֐�����Ă΂��B
 *---------------------------------------------------------------------------*/
static void DWCi_UpdateServersCallback(DWCError error, BOOL isChanged, void* param)
{
#pragma unused(param)

    // [arakit] main 051008
    if (stpDwcCnt->lastState != DWC_STATE_UPDATE_SERVERS){
        // DWC_UpdateServersAsyn()�ďo����A�ʂ�state�Ɉڂ��Ă��Ȃ���΁A
        // �Ăяo���O�̏�Ԃɖ߂�
        // [todo]
        // �Q�ȏ�state���ς���Ă�����Ή��ł��Ȃ��̂ŁA
        // UPDATE_SERVERS�̓��C����state����͊O���K�v������
        DWCi_SetState(stpDwcCnt->lastState);
    }
    // [arakit] main 051008

    // �Q�[������^����ꂽ�R�[���o�b�N���Ăяo��
    stpDwcCnt->updateServersCallback(error, isChanged, stpDwcCnt->updateServersParam);
}


/*---------------------------------------------------------------------------*
  �}�b�`���C�N�����R�[���o�b�N�֐�
  �����@�Ferror  DWC�G���[���
          cancel TRUE :�L�����Z���ɂ��}�b�`���C�N���I�������A
                 FALSE:�L�����Z���ł͂Ȃ�
          self   TRUE:�����̐ڑ����������AFALSE:���l�̐ڑ���������
          index  �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�̐ڑ��z�X�g��
                 �F�B���X�g�C���f�b�N�X
          param  �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�}�b�`���C�N�����R�[���o�b�N�B
          �}�b�`���C�N��������dwc_match.c���̊֐�����Ă΂��B
 *---------------------------------------------------------------------------*/
static void DWCi_MatchedCallback(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void* param)
{
#pragma unused(param)
    int i;

    if ((error == DWC_ERROR_NONE) && cancel){
        // �}�b�`���C�N���L�����Z�����ꂽ�ꍇ
        if (stpDwcCnt->matchcnt.state == DWC_MATCH_STATE_INIT){
            // �������L�����Z���Ń}�b�`���C�N�����������ꍇ
            // �c����QR2�L�[�f�[�^�𑼃N���C�A���g���Q�Ƃ��ė\��R�}���h��
            // �����Ă��Ȃ��悤�ɁAQR2�L�[�f�[�^���N���A����
            DWCi_ClearQR2Key();

            // �I�����C����Ԃɖ߂�
            DWCi_SetState(DWC_STATE_ONLINE);
        }
        // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�͑��z�X�g���L�����Z�����Ă�������
    }
    else if (error == DWC_ERROR_NONE){
        // ��Ԃ�ڑ���Ԃɂ���
        DWCi_SetState(DWC_STATE_CONNECTED);

        // ������AID���擾����
        // �i�T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̓}�b�`���C�N�J�n����
        // �擾���Ă��邪����������ʂ��Ă����j
        for (i = 0; i <= stpDwcCnt->matchcnt.gt2NumConnection; i++){
            if (stpDwcCnt->matchcnt.sbPidList[i] == stpDwcCnt->profileID){
                stpDwcCnt->aid = stpDwcCnt->matchcnt.aidList[i];
                break;
            }
        }
    }

#ifndef	SDK_FINALROM
    if (!error){
        int i;
        // [test]
        // �f�o�b�O�\��
        for (i = 0; i < stpDwcCnt->matchcnt.gt2NumConnection+1; i++){
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "aid[%d] = %d, pid[%d] = %u\n",
                       i, stpDwcCnt->matchcnt.aidList[i],
                       i, stpDwcCnt->matchcnt.sbPidList[i]);
        }
    }
#endif

    // �L����AID�r�b�g�}�b�v���X�V����
    stpDwcCnt->matchcnt.validAidBitmap =
        DWCi_GetAIDBitmapFromList(stpDwcCnt->matchcnt.aidList, stpDwcCnt->matchcnt.gt2NumConnection+1);
    // �L���R�l�N�V���������Z�b�g�ōX�V
    DWCi_SetNumValidConnection();

    // �Q�[������^����ꂽ�R�[���o�b�N���Ăяo��
    if ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
        (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL)){
        stpDwcCnt->matchedSCCallback(error, cancel, self, isServer, index,
                                     stpDwcCnt->matchedSCParam);
    }
    else {
        stpDwcCnt->matchedCallback(error, cancel, stpDwcCnt->matchedParam);
    }

    // [arakit] main 051007
    if ((error != DWC_ERROR_NONE) &&
        (stpDwcCnt != NULL) && (stpDwcCnt->state == DWC_STATE_MATCHING)){
        // �G���[�������Ƀ}�b�`���C�N��Ԃ̂܂܂Ȃ�I�����C����Ԃɖ߂�
        DWCi_SetState(DWC_STATE_ONLINE);
        // GP�X�e�[�^�X�͕K��DWCi_StopMatching()���ŃZ�b�g�����
    }
    // [arakit] main 051007
}


/*---------------------------------------------------------------------------*
  GP�G���[�R�[���o�b�N�֐�
  �����@�Fpconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg         GP�G���[�\���̂ւ̃|�C���^
          param       �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FGP�G���[�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GPErrorCallback(GPConnection* pconnection, GPErrorArg* arg, void* param)
{
#pragma unused(pconnection)
#pragma unused(param)
	gsi_char* errorCodeString;
	gsi_char* resultString;
    GPResult gpResult;

#ifdef DWC_IGNORE_GP_ERROR_ALREADY_BUDDY
    if (arg->errorCode == GP_ADDBUDDY_ALREADY_BUDDY ||
        arg->errorCode == GP_BM_NOT_BUDDY ||
        arg->errorCode == GP_DELBUDDY_NOT_BUDDY
	){
        // �o�^�ς݃o�f�B�ւ̃o�f�B�o�^�v���G���[�͖�������
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "Ignore GP ALLREADY_BUDDY or NOT_BUDDY %d.\n",
                   arg->errorCode);
        return;
    }
#endif

#define RESULT(x) case x: resultString = #x; break;
	switch (arg->result){
	RESULT(GP_NO_ERROR)
	RESULT(GP_MEMORY_ERROR)
	RESULT(GP_PARAMETER_ERROR)
	RESULT(GP_NETWORK_ERROR)
	RESULT(GP_SERVER_ERROR)
	default:
		resultString = "Unknown result!\n";
	}

#define ERRORCODE(x) case x: errorCodeString = #x; break;
	switch(arg->errorCode){
	ERRORCODE(GP_GENERAL)
	ERRORCODE(GP_PARSE)
	ERRORCODE(GP_NOT_LOGGED_IN)
	ERRORCODE(GP_BAD_SESSKEY)
	ERRORCODE(GP_DATABASE)
	ERRORCODE(GP_NETWORK)
	ERRORCODE(GP_FORCED_DISCONNECT)
	ERRORCODE(GP_CONNECTION_CLOSED)
	ERRORCODE(GP_LOGIN)
	ERRORCODE(GP_LOGIN_TIMEOUT)
	ERRORCODE(GP_LOGIN_BAD_NICK)
	ERRORCODE(GP_LOGIN_BAD_EMAIL)
	ERRORCODE(GP_LOGIN_BAD_PASSWORD)
	ERRORCODE(GP_LOGIN_BAD_PROFILE)
	ERRORCODE(GP_LOGIN_PROFILE_DELETED)
	ERRORCODE(GP_LOGIN_CONNECTION_FAILED)
	ERRORCODE(GP_LOGIN_SERVER_AUTH_FAILED)
	ERRORCODE(GP_NEWUSER)
	ERRORCODE(GP_NEWUSER_BAD_NICK)
	ERRORCODE(GP_NEWUSER_BAD_PASSWORD)
	ERRORCODE(GP_UPDATEUI)
	ERRORCODE(GP_UPDATEUI_BAD_EMAIL)
	ERRORCODE(GP_NEWPROFILE)
	ERRORCODE(GP_NEWPROFILE_BAD_NICK)
	ERRORCODE(GP_NEWPROFILE_BAD_OLD_NICK)
	ERRORCODE(GP_UPDATEPRO)
	ERRORCODE(GP_UPDATEPRO_BAD_NICK)
	ERRORCODE(GP_ADDBUDDY)
	ERRORCODE(GP_ADDBUDDY_BAD_FROM)
	ERRORCODE(GP_ADDBUDDY_BAD_NEW)
	ERRORCODE(GP_ADDBUDDY_ALREADY_BUDDY)
	ERRORCODE(GP_AUTHADD)
	ERRORCODE(GP_AUTHADD_BAD_FROM)
	ERRORCODE(GP_AUTHADD_BAD_SIG)
	ERRORCODE(GP_STATUS)
	ERRORCODE(GP_BM)
	ERRORCODE(GP_BM_NOT_BUDDY)
	ERRORCODE(GP_GETPROFILE)
	ERRORCODE(GP_GETPROFILE_BAD_PROFILE)
	ERRORCODE(GP_DELBUDDY)
	ERRORCODE(GP_DELBUDDY_NOT_BUDDY)
	ERRORCODE(GP_DELPROFILE)
	ERRORCODE(GP_DELPROFILE_LAST_PROFILE)
	ERRORCODE(GP_SEARCH)
	ERRORCODE(GP_SEARCH_CONNECTION_FAILED)
	default:
		errorCodeString = "Unknown error code!\n";
	}

	if (arg->fatal){
		DWC_Printf(DWC_REPORTFLAG_ERROR, "FATAL ERROR\n");
	}
	else {
		DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR\n");
	}
    
	DWC_Printf(DWC_REPORTFLAG_ERROR, "RESULT: %s (%d)\n", resultString, arg->result);
	DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR CODE: %s (0x%X)\n", errorCodeString, arg->errorCode);
	DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR STRING: %s\n", arg->errorString);

    // �S�ăl�b�g���[�N�G���[�Ƃ���
    gpResult = GP_NETWORK_ERROR;

    // GP�G���[�����֐��Ăяo��
    (void)DWCi_HandleGPError(gpResult);
}


/*---------------------------------------------------------------------------*
  GP�o�f�B���b�Z�[�W��M�R�[���o�b�N�֐�
  �����@�Fpconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg         GPRecvBuddyMessageArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param       �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FGP�o�f�B���b�Z�[�W��M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GPRecvBuddyMessageCallback(GPConnection* pconnection, GPRecvBuddyMessageArg* arg, void* param)
{
#pragma unused(param)
    char version[12] = { 0, };
    char* message = arg->message;
    u32 len;

    // dwc_friend.c�̃o�f�B�o�^���X�|���X�R�[���o�b�N�֔�ԁB
    if ( DWCi_GPRecvBuddyAuthCallback( pconnection, arg, param ) )
    {
        return;
    }

    if (memcmp(message, DWC_GP_COMMAND_STRING, strlen(DWC_GP_COMMAND_STRING))){
        // DWC��GP�R�}���h�ł͂Ȃ��o�f�B���b�Z�[�W����M���Ă��������Ȃ�
        DWC_Printf(DWC_REPORTFLAG_WARNING,
                   "Received undefined buddy message. '%s'\n",
                   message);
        return;
    }

    message += strlen(DWC_GP_COMMAND_STRING);
    len = (u32)(strchr(message, 'v')-message);
    strncpy(version, message, len);
    if ((len > 10) || (strtoul(version, NULL, 10) != DWC_MATCHING_VERSION)){
        // �o�[�W�����Ⴂ�R�}���h����M����
        DWC_Printf(DWC_REPORTFLAG_WARNING,
                   "Received different version buddy message command. '%s'\n",
                   message);
        return;
    }

    message += len+1;
    if (memcmp(message, DWC_GP_COMMAND_MATCH_STRING, strlen(DWC_GP_COMMAND_MATCH_STRING)) == 0){
        // [arakit] main 051010
        // �}�b�`���C�N�p�R�}���h�ŁA�}�b�`���C�N���Ȃ�}�b�`���C�N�p��
        // �R�[���o�b�N���Ăяo���i�T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ��
        // CONNECTED��Ԃł��Ăяo���j
        if ((stpDwcCnt->state == DWC_STATE_MATCHING) ||
            ((stpDwcCnt->state == DWC_STATE_CONNECTED) &&
             ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
              (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL)))){
            message += strlen(DWC_GP_COMMAND_MATCH_STRING);
            DWCi_MatchGPRecvBuddyMsgCallback(pconnection, arg->profile, message);
        }
        else {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed GP matching command.\n");
        }
        // [arakit] main 051010
    }
    //else {
    //    DWC_Printf(DWC_REPORTFLAG_WARNING,
    //               "Received undefined buddy message command. '%s'\n",
    //               arg->message);
    //}
}


/*---------------------------------------------------------------------------*
  GT2�f�[�^��M�R�[���o�b�N�֐�
  �����@�Fconnection ��M�����R�l�N�V�����̍\���̃|�C���^
          message    ��M�f�[�^
          len        ��M�f�[�^�T�C�Y
          reliable   �M�����̂���f�[�^���ǂ���
  �߂�l�F�Ȃ�
  �p�r�@�FGT2�f�[�^��M�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GT2ReceivedCallback(GT2Connection connection, GT2Byte* message, int len, GT2Bool reliable)
{

	// DWC Transport�̎�M����
    DWCi_RecvCallback( connection, message, len, reliable );
}


/*---------------------------------------------------------------------------*
  GT2�R�l�N�V�����ؒf�R�[���o�b�N�֐�
  �����@�Fconnection �ؒf���ꂽ�R�l�N�V�����̍\���̃|�C���^
          reason     �ؒf���R
  �߂�l�F�Ȃ�
  �p�r�@�FGT2�R�l�N�V�����ؒf�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GT2ClosedCallback(GT2Connection connection, GT2CloseReason reason)
{
    u8  aid;
    int profileID = 0;
    int errorCode;
    int i;
    BOOL isServer = FALSE;
    BOOL aidValid;
    DWCConnectionInfo* conninfo;
    DWCError dwcError;

#if 0
    // [todo]
    // GOAGetLastError()�̖߂�l�͂������ŁA�����Ƃ��Ďg���Ȃ������B
    // �\�P�b�g�G���[�̎��ɂ��̂܂ܐؒf�R�[���o�b�N�Ăяo���ɐi�ނ�
    // �Q�[���̎����ɂ���Ă͕ςȓ���ɂȂ��Ă��܂��Ǝv����B
    if (GOAGetLastError(gt2GetSocketSOCKET(stpDwcCnt->gt2Socket)) < 0){
        // �\�P�b�g�G���[�̍ۂ́Areason��LocalClose�ł����ɗ��邪�A
        // SocketErrorCallback����������̕�����ɌĂ΂��̂ŁA
        // GameSpy�̃Z�b�g�����G���[�����āA���̐�ɐi�ނ̂��~�߂�
        DWC_Printf(DWC_REPORTFLAG_ERROR,
                   "Connection was closed by socket error.\n");
        return;
    }
#endif

    if (DWCi_IsShutdownMatch()){
        // �ڑ��������A�}�b�`���C�N����DWC_ShutdownFriendsMatch()��
        // �Ă΂�Ă��܂����ꍇ�i�ʏ�Ă�ł͂����Ȃ��j�A�f�[�^�A�N�Z�X��O��
        // �Ȃ�Ȃ��悤�ɂ��邽�߁A�����ł͉������Ȃ�
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_WARNING,
                   "Called DWC_ShutdownFriendsMatch() with unexpected status.\n");
        return;
    }
        

    switch (reason){
	case GT2LocalClose:
	case GT2RemoteClose:
        dwcError = DWC_ERROR_NONE;
        break;
	case GT2CommunicationError:
	case GT2SocketError:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_GS_GT2+DWC_ECODE_TYPE_CLOSE;
        break;
	case GT2NotEnoughMemory:
		dwcError  = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_GS_GT2+DWC_ECODE_TYPE_CLOSE+DWC_ECODE_TYPE_ALLOC;
        break;
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "Connection was closed (reason %d).\n", reason);

    if (!dwcError){
        // �R�l�N�V���������擾����
	    conninfo = (DWCConnectionInfo *)gt2GetConnectionData(connection);

        if (!conninfo){
            // �����ꂱ�̃R�[���o�b�N������gt2CloseSocket()���Ă΂�A
            // �Ăт����ɗ����ꍇ�B�������Ȃ��B
            // ���C�u�������Ăяo�����Ƃ͂Ȃ����A���[�U���R�[���o�b�N����
            // DWC_ShutdownFriendsMatch()���Ăяo�����炱���ɗ���B
            return;
        }
        
        aid = conninfo->aid;  // AID��ޔ�

        // AID�����Ƀ}�b�`���C�N���������z�X�g�̂��̂ł��邩�ǂ������L�^����
        if (stpDwcCnt->matchcnt.validAidBitmap & (1 << aid))
             aidValid = TRUE;
        else aidValid = FALSE;

        // �g�����X�|�[�g�̃R�l�N�V�����\���̂��N���A����
        DWCi_ClearTransConnection(aid);

        DWC_Printf(DWC_REPORTFLAG_DEBUG, "aid = %d (validity %d).\n",
                   aid, aidValid);

        // �T�[�o�N���C�A���g�}�b�`���C�N�̏ꍇ�́A�T�[�o��
        // �N���[�Y�������Ƃ��L�^����
        // �N���C�A���g�̏ꍇ�Aaid = 0 �͕K���T�[�o�Ȃ̂ŁA���ꂾ���Ŕ���ł���
        if (((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) &&
             (reason == GT2LocalClose)) ||
            ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL) &&
             (aid == 0))){
            isServer = TRUE;
        }

        // AID���X�g����AID���������đO�ɋl�߂�B
        // �v���t�@�C��ID���X�g�������l�̏����������s���B
        profileID = DWCi_DeleteAID(aid);

        // [todo]
        // �{���͂����Ń������J��
        stGt2ConnectionList[conninfo->index] = NULL;

        if ( stpDwcCnt->matchcnt.gt2NumConnection > 0 )
        {
            stpDwcCnt->matchcnt.gt2NumConnection--;
        }
        if ( stpDwcCnt->matchcnt.qr2NNFinishCount > 0 )
        {
            stpDwcCnt->matchcnt.qr2NNFinishCount--;
        }
    }

    if (!stpDwcCnt->ownCloseFlag &&
        (stpDwcCnt->state == DWC_STATE_CONNECTED) && !aidValid){
        // �T�[�o�N���C�A���g�}�b�`���C�N�ŐV�K�ڑ��N���C�A���g��
        // �}�b�`���C�N���L�����Z�������ꍇ�B
        // �������́A�}�b�`���C�N�������������������ɐV�K�ڑ��N���C�A���g��
        // �������Ȃ��Ȃ����ꍇ
        if ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) &&
            (dwcError == DWC_ERROR_NONE)){
            // �����������̏ꍇ�ɁA�ڑ��ς݃N���C�A���g���N���[�Y��������
            // �V�K�ڑ��N���C�A���g���܂߂��l����gpStatus�ɃZ�b�g����
            // ���܂��Ă���ꍇ������̂ŁA�����Őڑ��l�������߂�
            // GP�X�e�[�^�X�ɃZ�b�g����
            (void)DWCi_GPSetServerStatus();
            
            // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�͐ڑ��ς݃N���C�A���g�ւ�
            // �L�����Z���ʒm���s��
            DWCi_ProcessMatchSCClosing(profileID);
        }

        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Closing process by matching SC.\n");
        return;
    }
    else if (DWCi_ProcessMatchClosing(dwcError, errorCode, profileID)){
        // �}�b�`���C�N���ɃR�l�N�V�����N���[�Y���ꂽ�ꍇ�́A�}�b�`���C�N�p��
        // �������s���A���̐�̏����͔�΂��B
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Closing process by matching.\n");
        return;
    }

    if (dwcError){
        // �G���[�̏ꍇ�͂����ŏ�������
        DWCi_SetError(dwcError,  errorCode+DWC_ECODE_SEQ_MATCH);
        return;
    }


    // �����ȉ��́A�F�B�w��A�F�B���w��̏ꍇ�́A�}�b�`���C�N������ɂ������Ȃ��B
    // �T�[�o�N���C�A���g�̏ꍇ�́A�������ڑ�������A�ڑ��ς݃z�X�g���i���j
    // �N���[�Y�����ꍇ�̂ݗ���B

    // [arakit] main 051010
    if ((!stpDwcCnt->ownCloseFlag) &&
        ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV) ||
         (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_CL)) &&
        (stpDwcCnt->matchcnt.sbPidList[stpDwcCnt->matchcnt.gt2NumConnection+2] != 0)){
        // �T�[�o�N���C�A���g�}�b�`���C�N�ŁA�V�K�ڑ��N���C�A���g���ڑ�����
        // ����O�ɁA�ڑ��ς݃N���C�A���g���R�l�N�V�������N���[�Y�����ꍇ�A
        // ���X�g������Ȃ��悤�ɒ������s�Ȃ��B
        // ���̊֐��ŗL����AID�r�b�g�}�b�v���ύX�����̂ŁAAID = 0 �̃r�b�g��
        // ���낵�Ă��܂�Ȃ��悤�Ɉꎞ�I��AID���Z�b�g���Ă����B
        stpDwcCnt->matchcnt.aidList[stpDwcCnt->matchcnt.gt2NumConnection+1] =
            stpDwcCnt->matchcnt.aidList[stpDwcCnt->matchcnt.gt2NumConnection+2];

        // �V�K�ڑ��N���C�A���g�̃f�[�^�����X�g��őO�ɂ��炷
        (void)DWCi_DeleteHostByIndex(stpDwcCnt->matchcnt.gt2NumConnection+1,
                                     stpDwcCnt->matchcnt.gt2NumConnection+3);
    }
    // [arakit] main 051010
        
    if (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ
        if (!stpDwcCnt->ownCloseFlag){
            // DWC_CloseAllConnectionsHard()�ŃN���[�Y�����̂łȂ��ꍇ�́A
            // �ڑ��l���̌�����GP�X�e�[�^�X�ɃZ�b�g����
            (void)DWCi_GPSetServerStatus();
        }
        else if (stpDwcCnt->matchcnt.gt2NumConnection == 0){
            // DWC_CloseAllConnectionsHard()�ŃN���[�Y�����������ꍇ��
            // GP�X�e�[�^�X���I�����C���ɂ���
            (void)DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);
        }
    }
    else if (stpDwcCnt->matchcnt.gt2NumConnection == 0){
        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�ȊO�ł́A
        // �S�ẴR�l�N�V�������N���[�Y���ꂽ��GP�X�e�[�^�X���I�����C���ɂ���
        (void)DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);
    }

    for (i = 0; i < stpDwcCnt->matchcnt.gt2NumConnection+1; i++){
         DWC_Printf(DWC_REPORTFLAG_DEBUG,
                    "aid[%d] = %d, pid[%d] = %u\n",
                    i, stpDwcCnt->matchcnt.aidList[i],
                    i, stpDwcCnt->matchcnt.sbPidList[i]);
    }

    if ((stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_ANYBODY) ||
        (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_FRIEND)){
        // �F�B�w��A���w��}�b�`���C�N�̏ꍇ�́A�ڑ���]�l�������݂̐ڑ�����
        // ���킹�Č��������A�N���C�A���g����̗\�񂪗��Ȃ��悤�ɂ���
        stpDwcCnt->matchcnt.qr2NumEntry = stpDwcCnt->matchcnt.qr2NNFinishCount;

        // ��Ԃ̕ω����}�X�^�[�T�[�o�ɒʒm����
        qr2_send_statechanged(stpDwcCnt->matchcnt.qr2Obj);
    }

    if (stpDwcCnt->closedCallback && aidValid){
        // �Q�[������^����ꂽ�R�[���o�b�N���Ăяo���B
        // �T�[�o�N���C�A���g�}�b�`���C�N�Őڑ��������Ă��Ȃ��N���C�A���g��
        // �ꍇ�̓R�[���o�b�N���Ăяo���Ȃ��B
        stpDwcCnt->closedCallback(dwcError,
                                  reason == GT2LocalClose ? TRUE : FALSE,
                                  isServer,
                                  aid,
                                  DWCi_GetFriendListIndex(profileID),
                                  stpDwcCnt->closedParam);
    }

    // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�̏ꍇ�́A�N���[�Y�����
    // �ڑ��z�X�g����0�ɂȂ��Ă��֌W�Ȃ��̂ŁA�����Ŕ�����
    if (!stpDwcCnt->ownCloseFlag &&
        (stpDwcCnt->matchcnt.qr2MatchType == DWC_MATCH_TYPE_SC_SV))
        return;

    if (!stpDwcCnt->matchcnt.gt2NumConnection){
        // �S�ẴR�l�N�V�������N���[�Y���ꂽ��
        // �T�[�o�N���C�A���g�}�b�`���C�N�p��NN�g�p�̈���J������
        DWCi_NNFreeNegotiateList();

        // �c����QR2�L�[�f�[�^�𑼃N���C�A���g���Q�Ƃ��ė\��R�}���h��
        // �����Ă��Ȃ��悤�ɁAQR2�L�[�f�[�^���N���A����
        DWCi_ClearQR2Key();

        DWCi_SetState(DWC_STATE_ONLINE);  // ���O�C����Ԃɖ߂�
    }
}


/*---------------------------------------------------------------------------*
  GT2 PING�R�[���o�b�N�֐�
  �����@�Fconnection PING�����������R�l�N�V�����̍\���̃|�C���^
          latency    ����z�X�g�ւ̃��C�e���V
  �߂�l�F�Ȃ�
  �p�r�@�FGT2 PING�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GT2PingCallback(GT2Connection connection, int latency)
{
#pragma unused(connection)
    
	DWC_Printf(DWC_REPORTFLAG_TRANSPORT, "Ping: %dms\n", latency);

	// DWC Transport�Őݒ肷��Ping�R�[���o�b�N
    DWCi_PingCallback( connection, latency );
}


/*---------------------------------------------------------------------------*
  GT2�\�P�b�g�G���[�R�[���o�b�N�֐�
  �����@�Fsocket GT2Socket�I�u�W�F�N�g
  �߂�l�F�Ȃ�
  �p�r�@�FGT2�\�P�b�g�G���[�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GT2SocketErrorCallback(GT2Socket socket)
{

    stLastSocketError = GOAGetLastError(gt2GetSocketSOCKET(socket));

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Socket fatal error! (%d)\n",
               stLastSocketError);

    // �}�b�`���C�N���ɂ��N���蓾�邪�AFATAL�G���[�Ƃ��ď�������̂�
    // �}�b�`���C�N�I�������͌Ă΂Ȃ�
    DWCi_SetError(DWC_ERROR_FATAL, DWC_ECODE_SEQ_ETC+DWC_ECODE_GS_GT2+DWC_ECODE_TYPE_SO_SOCKET);

    // �\�P�b�g�G���[�̏ꍇgti2SocketError()���Ń\�P�b�g���N���[�Y�����̂�
    // NULL�N���A���Ȃ���΂Ȃ�Ȃ��I
    stpDwcCnt->gt2Socket = NULL;
}
