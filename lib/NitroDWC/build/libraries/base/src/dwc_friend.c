#include <nitro.h>
#include <base/dwc_base_gamespy.h>
#include <gp/gpi.h>
#include <gstats/gpersist.h>

#include <auth/util_base64.h>
#include <base/dwc_report.h>
#include <base/dwc_account.h>
#include <base/dwc_error.h>
#include <base/dwc_login.h>
#include <base/dwc_friend.h>
#include <base/dwc_match.h>
#include <base/dwc_transport.h>
#include <base/dwc_main.h>
#include <base/dwc_common.h>


//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
// GP�T�[�o�ւ̃L�[�v�A���C�u������B
//#define DWC_ENABLE_KEEPALIVE_TO_GPSERVER


//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
// �F�B�Ǘ��I�u�W�F�N�g�ւ̃|�C���^
static DWCFriendControl* stpFriendCnt = NULL;
static int stPersState = DWC_PERS_STATE_INIT;

// CloseStatsConnection()��PersistThink()����Ă΂Ȃ��悤�ɒ�������t���O
static BOOL s_bInPersistThink = FALSE;
static BOOL s_bNeedCloseStatsConnection = FALSE;

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static GPResult DWCi_GPProcess(void);
static void DWCi_CloseFriendProcess(void);
static void  DWCi_UpdateFriendReq(DWCFriendData friendList[], int friendListLen);
static void DWCi_EndUpdateServers(void);

static void DWCi_DeleteFriendFromList(DWCFriendData friendList[], int index, int otherIndex);
static BOOL DWCi_RefreshFriendListForth(DWCFriendData friendList[], int index, int profileID);
static int  DWCi_RefreshFriendListAll(DWCFriendData friendList[], int friendListLen, int profileID);

static GPResult DWCi_GPSendBuddyRequest(int profileID);

static BOOL DWCi_GetFriendBuddyStatus(const DWCFriendData* friendData, GPBuddyStatus* status);

static GPResult DWCi_HandleGPError(GPResult result);
static int DWCi_HandlePersError(int error);

static void DWCi_GPProfileSearchCallback(GPConnection* connection,
                                         GPProfileSearchResponseArg* arg,
                                         void* param);
static void DWCi_GPGetInfoCallback_RecvBuddyRequest(GPConnection* connection,
                                                    GPGetInfoResponseArg* arg,
                                                    void* param);
static void DWCi_GPGetInfoCallback_RecvAuthMessage(GPConnection* connection,
                                                   GPGetInfoResponseArg* arg,
                                                   void* param);

static void DWCi_StopPersLogin(DWCError error, int errorCode);
static void DWCi_SetPersistDataValuesAsync(int profileID,
                                           persisttype_t type,
                                           gsi_char* keyvalues,
                                           void* param);
static void DWCi_GetPersistDataValuesAsync(int profileID,
                                           persisttype_t type,
                                           gsi_char* keys,
                                           void* param);
static void DWCi_PersAuthCallback(int localid, int profileid,
                                  int authenticated, gsi_char* errmsg,
                                  void* instance);
static void DWCi_PersDataCallback(int localid, int profileid,
                                  persisttype_t type, int index, int success,
                                  time_t modified, char* data, int len,
                                  void* instance);
static void DWCi_PersDataSaveCallback(int localid, int profileid,
                                      persisttype_t type, int index, int success,
                                      time_t modified, void* instance);
static void DWCi_AddPersCallbackLevel(void);
static void DWCi_SubPersCallbackLevel(void);
static u32  DWCi_GetPersCallbackLevel(void);


//----------------------------------------------------------------------------
// function - external
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐�
  �����@�FfriendData   �F�B���ւ̃|�C���^
          statusString �Q�[���Œ�`����GameSpy status������i�[��|�C���^�B
                       NULL��n���Ε�����̃R�s�[�͍s��Ȃ��B
                       ������͍ő�255�����{NULL�I�[�B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
 *---------------------------------------------------------------------------*/
u8 DWC_GetFriendStatus(const DWCFriendData* friendData, char* statusString)
{

    return DWC_GetFriendStatusSC(friendData, NULL, NULL, statusString);
}


/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐��i�T�[�o�N���C�A���g�}�b�`���C�N�Q�[���p�j
  �����@�FfriendData   �F�B���ւ̃|�C���^
          maxEntry     �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                       ����ꍇ�̍ő�ڑ��l���B
                       �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          numEntry     �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                       ����ꍇ�̌��ݐڑ��l���B
                       �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          statusString �Q�[���Œ�`����GameSpy status������i�[��|�C���^�B
                       NULL��n���Ε�����̃R�s�[�͍s��Ȃ��B
                       ������͍ő�255�����{NULL�I�[�B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
 *---------------------------------------------------------------------------*/
u8 DWC_GetFriendStatusSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry, char* statusString)
{
    char valueStr[4];
    int  len;
    GPBuddyStatus status;

    // GPBudyStatus���擾����
    if (DWCi_GetFriendBuddyStatus(friendData, &status)){
        if (status.status == DWC_STATUS_MATCH_SC_SV){
            // �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂ĂĂ���ꍇ
            if (maxEntry){
                // �ő�ڑ��l���̐ݒ��ǂݏo��
                len = DWC_GetCommonValueString(DWC_GP_SSTR_KEY_MATCH_SC_MAX,
                                               valueStr,
                                               status.statusString, '/');
                if (len > 0) *maxEntry = (u8)strtoul(valueStr, NULL, 10);
                else *maxEntry = 0;
            }

            if (numEntry){
                // ���ݐڑ��l���̐ݒ��ǂݏo��
                len = DWC_GetCommonValueString(DWC_GP_SSTR_KEY_MATCH_SC_NUM,
                                               valueStr,
                                               status.statusString, '/');
                if (len > 0) *numEntry = (u8)strtoul(valueStr, NULL, 10);
                else *numEntry = 0;
            }
        }
        else {
            // �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�łȂ��ꍇ
            if (maxEntry) *maxEntry = 0;
            if (numEntry) *numEntry = 0;
        }

        if (statusString){
            // statusString��locationString���Z�b�g
            strcpy(statusString, status.locationString);
        }

        return status.status;
    }
    else {
        // �G���[�̏ꍇ��OFFLINE����
        if (maxEntry) *maxEntry = 0;
        if (numEntry) *numEntry = 0;
        return DWC_STATUS_OFFLINE;
    }
}


/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐��istatusData�Łj
  �����@�FfriendData �F�B���ւ̃|�C���^
          statusData �Q�[���Œ�`����GameSpy status�f�[�^�i�[��|�C���^�B
                     NULL��n���΃f�[�^�̃R�s�[�͍s��Ȃ��B
          size       �Q�[���Œ�`����GameSpy status�f�[�^���̊i�[��|�C���^�B
                     �����ȃf�[�^�̏ꍇ��-1���i�[�����B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
          �߂�l��DWC_STATUS_OFFLINE�̏ꍇ�́A*size = -1 ���Z�b�g����ȊO��
          �l���Z�b�g�����ɕԂ��܂��B
 *---------------------------------------------------------------------------*/
u8 DWC_GetFriendStatusData(const DWCFriendData* friendData, char* statusData, int* size)
{

    return DWC_GetFriendStatusDataSC(friendData, NULL, NULL, statusData, size);
}


/*---------------------------------------------------------------------------*
  �F�B�ʐM��Ԏ擾�֐��i�T�[�o�N���C�A���g�}�b�`���C�N�Q�[���pstatusData�Łj
  �����@�FfriendData �F�B���ւ̃|�C���^
          maxEntry   �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                     ����ꍇ�̍ő�ڑ��l���B
                     �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          numEntry   �F�B���T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�o�𗧂Ă�
                     ����ꍇ�̌��ݐڑ��l���B
                     �i�[��|�C���^�BNULL��n���Ύ擾���Ȃ��B
          statusData �Q�[���Œ�`����GameSpy status������i�[��|�C���^�B
                     NULL��n���Ε�����̃R�s�[�͍s��Ȃ��B
          size       �Q�[���Œ�`����GameSpy status�f�[�^���̊i�[��|�C���^�B
                     �����ȃf�[�^�̏ꍇ��-1���i�[�����B
  �߂�l�F�ʐM���enum�l�idwc_friend.h ��DWC_STATUS_*�j�B
          �������I�t���C���̎��ȂǂɌĂяo����DWC_STATUS_OFFLINE�����Ԃ��Ȃ��B
          �܂����肪�܂�GP�̃o�f�B���X�g�ɓo�^����Ă��Ȃ�����
          DWC_STATUS_OFFLINE�Ԃ��B
  �p�r�@�F���[�J���ɃL���b�V�����ꂽ�A�w��̗F�B�̒ʐM��Ԃ��擾����B
  �@�@�@�@�ʐM�͍s��Ȃ��B
          �F�B�̒ʐM��Ԃ́A�ω�������x��GP�T�[�o����ʒm�����B
          �������A�ʐM���ɓˑR�d����؂����ꍇ�Ȃǂ͐����Ԃ͑O�̏�Ԃ��c��B
          �߂�l��DWC_STATUS_OFFLINE�̏ꍇ�́A*size = -1 ���Z�b�g����ȊO��
          �l���Z�b�g�����ɕԂ��܂��B
 *---------------------------------------------------------------------------*/
u8 DWC_GetFriendStatusDataSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry, char* statusData, int* size)
{
    u8   ret;
    char statusString[DWC_GS_STATUS_STRING_LEN];

    ret = DWC_GetFriendStatusSC(friendData, maxEntry, numEntry, statusString);

    if (ret == DWC_STATUS_OFFLINE){
        *size = -1;
        return ret;
    }

    // �܂��̓o�C�i���f�[�^�̃T�C�Y�𒲂ׂ邾��
    *size = DWC_Auth_Base64Decode(statusString, strlen(statusString), NULL, 0);

    if (!statusData || (*size == -1)) return ret;

    // �e�L�X�g�f�[�^���o�C�i���f�[�^�ɕϊ����Ċi�[����
    (void)DWC_Auth_Base64Decode(statusString, strlen(statusString), statusData, (u32)*size);

    return ret;
}


/*---------------------------------------------------------------------------*
  �F�B���擾�֐�
  �����@�FfriendList    �F�B���X�g
          friendListLen �F�B���X�g�̍ő咷�i�v�f���j
  �߂�l�F�F�B���X�g�ɓo�^����Ă���F�B�̐�
  �p�r�@�F���[�J���̗F�B���X�g���Q�Ƃ��āA�F�B�o�^����Ԃ��B�ʐM�͍s��Ȃ��B
 *---------------------------------------------------------------------------*/
int  DWC_GetNumFriend(const DWCFriendData friendList[], int friendListLen)
{
    int count = 0;
    int i;

    if (!friendList) return 0;

    for (i = 0; i < friendListLen; i++){
        // [note]
        // ���̔��肾�ƁA�Q�[���R�[�h�Ⴂ�́i�ꍇ�ɂ���Ă͖����ȁj
        // �F�B�����J�E���g���Ă��܂�
        if (DWCi_Acc_IsValidFriendData(&friendList[i])) count++;
    }

    return count;
}


/*---------------------------------------------------------------------------*
  GameSpy Status������Z�b�g�֐�
  �����@�FstatusString GameSpy��Status�ɃZ�b�g���镶����B255�����{NULL�I�[�܂ŁB
  �߂�l�FTRUE :�Z�b�g����
          FALSE:���O�C���O�ȂǂŃZ�b�g���s
  �p�r�@�F������GameSpy Status�ɕ�������Z�b�g����B
          �Z�b�g���ꂽ������́A���̃z�X�g�����DWC_GetFriendStatus()��
          �擾���邱�Ƃ��ł���B
          '/'��'\\'�͎��ʕ����Ƃ��ă��C�u�������g���̂ŁA�����񒆂�
          �g�p���Ȃ��ŉ������B
          �Z�b�g���ꂽ������́ADWC_ShutdownFriendsMatch()���Ă΂���
          �N���A����܂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_SetOwnStatusString(const char* statusString)
{
    
    if ((stpFriendCnt == NULL) || !DWCi_CheckLogin()) return FALSE;
    
    if (DWCi_SetGPStatus(DWC_GP_STATUS_NO_CHANGE, NULL, statusString)) return FALSE;
    else return TRUE;
}


/*---------------------------------------------------------------------------*
  GameSpy Status������擾�֐�
  �����@�FstatusString GameSpy��Status������i�[��|�C���^�B255�����{NULL�I�[�܂ŁB
  �߂�l�FTRUE :�擾�����AFALSE:���O�C���O�ȂǂŎ擾���s
  �p�r�@�F���݂̎�����GameSpy Status��������擾����B
 *---------------------------------------------------------------------------*/
BOOL DWC_GetOwnStatusString(char* statusString)
{

    if ((stpFriendCnt == NULL) || (*stpFriendCnt->pGpObj == NULL) || !statusString) return FALSE;

    // [note]
    // gpi.h�ɋL�q����Ă���GPIConnection�\���̂��g���Ă���
    strcpy(statusString, ((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  GameSpy Status�f�[�^�Z�b�g�֐�
  �����@�FstatusData GameSpy��Status�ɃZ�b�g����o�C�i���f�[�^�ւ̃|�C���^�B
          size       �o�C�i���f�[�^�̃T�C�Y�B189Byte�܂ŁB
  �߂�l�FTRUE :�Z�b�g����
          FALSE:���O�C���O�A�e�ʃI�[�o�[�ȂǂŃZ�b�g���s
  �p�r�@�F������GameSpy Status�Ƀo�C�i���f�[�^��Base64�Ńe�L�X�g������
          �Z�b�g����B
          �Z�b�g���ꂽ�f�[�^�́A���̃z�X�g����DWC_GetFriendStatusData()��
          �擾���邱�Ƃ��ł���B
          �Z�b�g���ꂽ�f�[�^�́ADWC_ShutdownFriendsMatch()���Ă΂���
          �N���A����܂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_SetOwnStatusData(const char* statusData, u32 size)
{
    char statusString[DWC_GS_STATUS_STRING_LEN];
    int  len;
    
    if (!stpFriendCnt || !DWCi_CheckLogin()) return FALSE;

    // �e�L�X�g�ϊ����s��
    len = DWC_Auth_Base64Encode(statusData, size,
                                statusString, DWC_GS_STATUS_STRING_LEN-1);

    if (len == -1) return FALSE;  // �����������E�I�[�o�[�Ȃ玸�s

    statusString[len] = '\0';     // NULL�I�[��ۏ؂���
    
    if (DWCi_SetGPStatus(DWC_GP_STATUS_NO_CHANGE, NULL, statusString)) return FALSE;
    else return TRUE;
}


/*---------------------------------------------------------------------------*
  GameSpy Status�f�[�^�擾�֐�
  �����@�FstatusData GameSpy��Status�f�[�^�i�[��|�C���^
  �߂�l�F-1:���O�C���O�A�s���ȃf�[�^�ȂǂŎ擾���s�A
          �񕉐���:�擾�����o�C�i���f�[�^�̃T�C�Y
  �p�r�@�F���݂̎�����GameSpy Status�f�[�^���擾����B
          ����statusData��NULL��n���ƁA�o�C�i���f�[�^�̃T�C�Y�̂ݎ擾����B
 *---------------------------------------------------------------------------*/
int DWC_GetOwnStatusData(char* statusData)
{
    int size;

    if (!stpFriendCnt || !DWCi_CheckLogin()) return FALSE;

    // [note]
    // gpi.h�ɋL�q����Ă���GPIConnection�\���̂��g���Ă���

    // �܂��̓o�C�i���f�[�^�̃T�C�Y�𒲂ׂ邾��
    size = DWC_Auth_Base64Decode(((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString,
                                 strlen(((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString),
                                 NULL, 0);

    if (!statusData || (size == -1)) return size;

    // �e�L�X�g�f�[�^���o�C�i���f�[�^�ɕϊ����Ċi�[����
    return DWC_Auth_Base64Decode(((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString,
                                 strlen(((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString),
                                 statusData, (u32)size);
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�ҏW�\��Ԏ擾�֐�
  �����@�F�Ȃ�
  �߂�l�FTRUE:�F�B���X�g��ҏW���ėǂ��AFALSE:�F�B���X�g��ҏW���Ă͂����Ȃ�
  �p�r�@�F�F�B���X�g��ҏW���Ă��ǂ��^�C�~���O���ǂ����𒲂ׂ�B
          �����ł̖߂�l�́A����DWC_ProcessFriendsMatch()���Ă΂��܂ŗL���B
 *---------------------------------------------------------------------------*/
BOOL DWC_CanChangeFriendList(void)
{

    if ((stpFriendCnt != NULL) &&
        ((stpFriendCnt->buddyUpdateState == DWC_BUDDY_UPDATE_STATE_CHECK) ||
         (stpFriendCnt->buddyUpdateState == DWC_BUDDY_UPDATE_STATE_PSEARCH))){
        // DWCi_UpdateFriendReq()�̏��������S�ɏI���܂ł́A
        // �F�B���X�g�̃f�[�^�����炳�ꂽ�肷��ƃ`�F�b�N�ł��Ȃ��F�B���
        // �c���Ă��܂����肷��̂ŁA���̊Ԃ͕ҏW�֎~�Ƃ���
        return FALSE;
    }
    else {
        return TRUE;
    }
}


/*---------------------------------------------------------------------------*
  �F�B��񊮑S�����֐�
  �����@�FfriendData �F�B���ւ̃|�C���^
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�����[���N���A���AGP�T�[�o��̃o�f�B�֌W����������B
 *---------------------------------------------------------------------------*/
void DWC_DeleteBuddyFriendData(DWCFriendData* friendData)
{
    int  profileID;

    if ((stpFriendCnt != NULL)          &&
        DWCi_CheckLogin()               &&
        (DWCi_GetUserData() != NULL)    &&
        ((profileID = DWC_GetGsProfileId(DWCi_GetUserData(), friendData)) != 0) &&
        (profileID != -1) &&
        gpIsBuddy(stpFriendCnt->pGpObj, profileID)){
        // �F�B���ɗL���ȃv���t�@�C��ID�������Ă���A���̃v���t�@�C��ID��
        // GP��̃o�f�B�ł���ꍇ
        // �o�f�B������
        (void)gpDeleteBuddy(stpFriendCnt->pGpObj, profileID);

        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "DWC_DeleteBuddyFriendData : Deleted buddy.\n");
    }
    else {
        // [arakit] main 051013
        DWC_Printf(DWC_REPORTFLAG_DEBUG,
                   "DWC_DeleteBuddyFriendData : Only clear data.\n");
    }

    // �F�B�����[���N���A
    MI_CpuClear8(friendData, sizeof(DWCFriendData));
}


/*---------------------------------------------------------------------------*
  �o�f�B�����R�[���o�b�N�o�^�֐�
  �����@�Fcallback �o�f�B�����R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F���[�J���̗F�B���X�g�ɓo�^���ꂽ�F�B���ADWC_UpdateServersAsync()
          �ďo�����ȊO�̃^�C�~���O�ŁAGP�T�[�o��ł��o�f�B�Ƃ���
          �F�߂�ꂽ���ɌĂяo�����R�[���o�b�N��o�^����
 *---------------------------------------------------------------------------*/
BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void* param)
{

    if (!stpFriendCnt) return FALSE;

    stpFriendCnt->buddyCallback = callback;
    stpFriendCnt->buddyParam    = param;

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �F�B��ԕω��ʒm�R�[���o�b�N�o�^�֐�
  �����@�Fcallback �F�B��ԕω��ʒm�R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^�BNULL��n���Ή����Z�b�g���Ȃ��B
                   �R�[���o�b�N�p�p�����[�^�̃f�t�H���g�l��NULL�B
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F�F�B�̒ʐM��Ԃ��ω��������ɒʒm�����R�[���o�b�N��o�^����B
          �ʏ��DWC_UpdateServersAsync()�œo�^�ł��邪�A
          DWC_UpdateServersAsync()���g��Ȃ����F�B�̏�Ԃ͎擾�������Ƃ���
          �ꍇ�ɖ{�֐���p����B
 *---------------------------------------------------------------------------*/
BOOL DWC_SetFriendStatusCallback(DWCFriendStatusCallback callback, void* param)
{

    if (!stpFriendCnt) return FALSE;

    stpFriendCnt->statusCallback  = callback;
    if (param != NULL){
        stpFriendCnt->statusParam = param;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  Base64�G���R�[�h�֐�
  �����@�Fsrc    �ϊ����f�[�^�ւ̃|�C���^
          srclen �ϊ����f�[�^��
          dst    �ϊ���f�[�^�i�[�p�o�b�t�@�ւ̃|�C���^�B
                 NULL���w�肷��Εϊ���ɕK�v�ȍő�o�b�t�@�T�C�Y��Ԃ������B
          dstlen �ϊ���f�[�^�i�[�p�o�b�t�@�̃T�C�Y�i�����񒷁j
  �߂�l�F-1      :�ϊ����s�B�K�v�ȕϊ���o�b�t�@�T�C�Y��dstlen���z���Ă���B
          ����ȊO:�ϊ����ʂ̕�����
  �p�r�@�F�^����ꂽ�f�[�^��Base64�ŃG���R�[�h���Achar�^�̕�����Ƃ��ďo�͂���
 *---------------------------------------------------------------------------*/
int DWC_Base64Encode(const char *src, const u32 srclen, char *dst, const u32 dstlen)
{
    return DWC_Auth_Base64Encode(src, srclen, dst, dstlen);
}


/*---------------------------------------------------------------------------*
  Base64�f�R�[�h�֐�
  �����@�Fsrc    Base64�f�[�^�ւ̃|�C���^
          srclen Base64�f�[�^���i�����񒷁j
          dst    �ϊ���f�[�^�i�[�p�o�b�t�@�ւ̃|�C���^�B
                 NULL���w�肷��Εϊ���ɕK�v�ȍő�o�b�t�@�T�C�Y��Ԃ������B
          dstlen �ϊ���f�[�^�i�[�p�o�b�t�@�̃T�C�Y
  �߂�l�F-1      :�ϊ����s�B�K�v�ȕϊ���o�b�t�@�T�C�Y��dstlen���z���Ă���B
          ����ȊO:�ϊ����ʂ̃f�[�^�T�C�Y
  �p�r�@�F�^����ꂽBase64�f�[�^���f�R�[�h���Achar�^�̃f�[�^�Ƃ��ďo�͂���
 *---------------------------------------------------------------------------*/
int DWC_Base64Decode(const char *src, const u32 srclen, char *dst, const u32 dstlen)
{
    return DWC_Auth_Base64Decode(src, srclen, dst, dstlen);
}

/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o�ւ̃��O�C���֐�
  �����@�Fcallback ���O�C�������ʒm�R�[���o�b�N
          param    �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE :���O�C���J�n�B���������s�ŃR�[���o�b�N���Ԃ��Ă���B
          FALSE:DWC_LoginAsync()�����O�A���Ƀ��O�C���ς݂ȂǂŃ��O�C����
                �J�n���Ȃ������B�R�[���o�b�N�͕Ԃ��Ă��Ȃ��B
  �p�r�@�FGameSpy�f�[�^�X�g���[�W�T�[�o�ւ̃��O�C�����s�Ȃ��B
          ���O�C�����Ȃ��ƃf�[�^�X�g���[�W�T�[�o�ւ̃Z�[�u�E���[�h�͂ł��Ȃ��B
 *---------------------------------------------------------------------------*/
BOOL DWC_LoginToStorageServerAsync(DWCStorageLoginCallback callback, void* param)
{
    char response[33];
    char *authToken, *partnerChallenge;
    int  persResult;
    int  i;

    // �܂�GP�T�[�o�Ƀ��O�C�����ĂȂ����A����Persistent�T�[�o�Ƀ��O�C���ς݂Ȃ�
    // ���������ɖ߂�
    if (!DWCi_GetAuthInfo(&authToken, &partnerChallenge) || IsStatsConnected())
        return FALSE;

    stpFriendCnt->persLoginCallback = callback;
    stpFriendCnt->persLoginParam    = param;

    for (i = 0; i < DWC_DNS_ERROR_RETRY_MAX; i++){
		stPersState = DWC_PERS_STATE_LOGIN;
        persResult = InitStatsConnection(0);  // stats��persistentSDK���ʏ�����
    
        if (persResult == GE_NOERROR) break;
    
        // DNS�G���[�Ȃ���񐔂̓��g���C����
        if ((persResult != GE_NODNS) || (i == DWC_DNS_ERROR_RETRY_MAX-1)){
            DWCi_HandlePersError(persResult);
            return TRUE;  // �R�[���o�b�N�ŃG���[��Ԃ�
        }
    }

    // Persistent�T�[�o�ւ̔F�؂��s��
    (void)GenerateAuth(GetChallenge(NULL), partnerChallenge, response);
    PreAuthenticatePlayerPartner(0, authToken, response,
                                 DWCi_PersAuthCallback, NULL);

    DWCi_AddPersCallbackLevel();

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o����̃��O�A�E�g�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FGameSpy�f�[�^�X�g���[�W�T�[�o���烍�O�A�E�g���A
          ����I�u�W�F�N�g���������B
          ���̊֐����Ă΂Ȃ��Ă��ADWC_ShutdownFriendsMatch()���Ăׂ΁A
          ���O�A�E�g�������s�Ȃ���B
 *---------------------------------------------------------------------------*/
void DWC_LogoutFromStorageServer(void)
{
    if(s_bInPersistThink == TRUE)
    {
        s_bNeedCloseStatsConnection = TRUE;
    }
    else
    {
        CloseStatsConnection();  // Persistent�I�u�W�F�N�g�̊J��
    }

    stPersState = DWC_PERS_STATE_INIT;

    if (stpFriendCnt != NULL) stpFriendCnt->persCallbackLevel = 0;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o �f�[�^�Z�[�u�E���[�h�����ʒm�R�[���o�b�N�o�^�֐�
  �����@�FsaveCallback �Z�[�u�����ʒm�R�[���o�b�N
          loadCallback ���[�h�����ʒm�R�[���o�b�N
  �߂�l�FTRUE :�o�^����
          FALSE:FriendsMatch���C�u�����񓮍쒆�œo�^���s
  �p�r�@�F�f�[�^�X�g���[�W�ւ̃Z�[�u�E���[�h�������������ɌĂяo�����
          �R�[���o�b�N��o�^����
 *---------------------------------------------------------------------------*/
BOOL DWC_SetStorageServerCallback(DWCSaveToServerCallback saveCallback,
                                  DWCLoadFromServerCallback loadCallback)
{

    if (!stpFriendCnt) return FALSE;

    stpFriendCnt->saveCallback = saveCallback;
    stpFriendCnt->loadCallback = loadCallback;

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�oPublic�f�[�^�Z�[�u�֐�
  �����@�Fkeyvalues �f�[�^�x�[�X������ikey/value�l�j�ւ̃|�C���^
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:�Z�[�u�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���ŃZ�[�u�ł��Ȃ�
  �p�r�@�F�N�ł��Q�Ɖ\�ȃf�[�^���f�[�^�X�g���[�W�T�[�o�ɃZ�[�u����B
          �Z�[�u����f�[�^��NULL�I�[�̕�����ŁAkey/value�̑g�ł���K�v������B
          �i"\\name\\mario\\stage\\3"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key/value�g����x�ɃZ�[�u���邱�Ƃ��ł���j
          �f�[�^�̃Z�b�g������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_SavePublicDataAsync(char* keyvalues, void* param)
{

    if ((stPersState != DWC_PERS_STATE_CONNECTED) || DWCi_IsError() || !stpFriendCnt) return FALSE;

    DWCi_SetPersistDataValuesAsync(stpFriendCnt->profileID, pd_public_rw,
                                   keyvalues, param);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�oPrivate�f�[�^�Z�[�u�֐�
  �����@�Fkeyvalues �Z�[�u�������f�[�^�x�[�X������ikey/value�l�j�ւ̃|�C���^
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:�Z�[�u�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���ŃZ�[�u�ł��Ȃ�
  �p�r�@�F�{�l�̂ݎQ�Ɖ\�ȃf�[�^���f�[�^�X�g���[�W�T�[�o�ɃZ�[�u����B
          �Z�[�u����f�[�^��NULL�I�[�̕�����ŁAkey/value�̑g�ł���K�v������B
          �i"\\name\\mario\\stage\\3"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key/value�g����x�ɃZ�[�u���邱�Ƃ��ł���j
          �f�[�^�̃Z�b�g������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_SavePrivateDataAsync(char* keyvalues, void* param)
{

    if ((stPersState != DWC_PERS_STATE_CONNECTED) || DWCi_IsError() || !stpFriendCnt) return FALSE;

    DWCi_SetPersistDataValuesAsync(stpFriendCnt->profileID, pd_private_rw,
                                   keyvalues, param);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o ������Public�f�[�^���[�h�֐�
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����Public�f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_LoadOwnPublicDataAsync(char* keys, void* param)
{

    if ((stPersState != DWC_PERS_STATE_CONNECTED) || DWCi_IsError() || !stpFriendCnt) return FALSE;

    DWCi_GetPersistDataValuesAsync(stpFriendCnt->profileID, pd_public_rw, keys, param);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o ������Private�f�[�^���[�h�֐�
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�AFALSE�F�I�t���C����Ԃ��ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����Private�f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_LoadOwnPrivateDataAsync(char* keys, void* param)
{

    if ((stPersState != DWC_PERS_STATE_CONNECTED) || DWCi_IsError() || !stpFriendCnt) return FALSE;

    DWCi_GetPersistDataValuesAsync(stpFriendCnt->profileID, pd_private_rw, keys, param);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �f�[�^�X�g���[�W�T�[�o ���l��Public�f�[�^���[�h�֐�
  �����@�Fkeys  ���[�h�������f�[�^��key������ւ̃|�C���^
          index ���[�h���������[�U�̗F�B���X�g�C���f�b�N�X
          param �R�[���o�b�N�p�p�����[�^
  �߂�l�FTRUE:���[�h�J�n�A
          FALSE�F�������I�t���C����Ԃ����肪�F�B�łȂ��A�ʐM�G���[���Ń��[�h�ł��Ȃ�
  �p�r�@�F�f�[�^�X�g���[�W�T�[�o����Public�f�[�^�����[�h����B
          ���[�h����f�[�^��key�Ŏw�肵�ANULL�I�[�̕�����ł���K�v������B
          �i"\\name\\stage"�̂悤��'\\'�ŋ�؂��ċL�q����B
          ���̗�̂悤�ɕ�����key����x�Ƀ��[�h���邱�Ƃ��ł���j
          �f�[�^�̃��[�h������������R�[���o�b�N���Ă΂��B
 *---------------------------------------------------------------------------*/
BOOL DWC_LoadOthersDataAsync(char* keys, int index, void* param)
{

    if ((stPersState != DWC_PERS_STATE_CONNECTED) || DWCi_IsError() || !stpFriendCnt ||
        !DWCi_GetProfileIDFromList(index))
        return FALSE;

    DWCi_GetPersistDataValuesAsync(DWCi_GetProfileIDFromList(index), pd_public_rw, keys, param);

    return TRUE;
}


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  �F�B�Ǘ��\���̏������֐�
  �����@�Ffriendcnt     �F�B�Ǘ��I�u�W�F�N�g�ւ̃|�C���^
          pGpObj        gp�R�l�N�V�����\���̃|�C���^�ւ̃|�C���^
          playerName    ���̃��[�U���Q�Ƃ��鎩���̃v���C���[���ւ̃|�C���^
          friendList    �F�B���X�g�ւ̃|�C���^
          friendListLen �F�B���X�g�̍ő咷�i�v�f���j
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�Ǘ��\���̂�����������
 *---------------------------------------------------------------------------*/
void DWCi_FriendInit(DWCFriendControl* friendcnt,
                     GPConnection* pGpObj,
                     const u16* playerName,
                     DWCFriendData* friendList,
                     int friendListLen)
{

    SDK_ASSERT(friendcnt);

    stpFriendCnt = friendcnt;

    stpFriendCnt->state              = DWC_FRIEND_STATE_INIT;
    stpFriendCnt->pGpObj             = pGpObj;
    stpFriendCnt->gpProcessCount     = 0;
    stpFriendCnt->lastGpProcess      = 0;
    stpFriendCnt->friendListLen      = friendListLen;
    stpFriendCnt->friendList         = friendList;
    stpFriendCnt->buddyUpdateIdx     = 0;
    stpFriendCnt->friendListChanged  = 0;
    stpFriendCnt->buddyUpdateState   = DWC_BUDDY_UPDATE_STATE_WAIT;
    stpFriendCnt->svUpdateComplete   = 0;
    stpFriendCnt->persCallbackLevel  = 0;
    stpFriendCnt->profileID          = 0;
    stpFriendCnt->playerName         = playerName;
    stpFriendCnt->updateCallback     = NULL;
    stpFriendCnt->updateParam        = NULL;
    stpFriendCnt->statusCallback     = NULL;
    stpFriendCnt->statusParam        = NULL;
    stpFriendCnt->deleteCallback     = NULL;
    stpFriendCnt->deleteParam        = NULL;
    stpFriendCnt->buddyCallback      = NULL;
    stpFriendCnt->buddyParam         = NULL;
    stpFriendCnt->persLoginCallback  = NULL;
    stpFriendCnt->persLoginParam     = NULL;
    stpFriendCnt->saveCallback       = NULL;
    stpFriendCnt->loadCallback       = NULL;
}


/*---------------------------------------------------------------------------*
  �F�B�Ǘ��X�V�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F���Q�[���t���[���Ăяo���A�ʐM�������X�V����
 *---------------------------------------------------------------------------*/
void DWCi_FriendProcess(void)
{
    GPResult gpResult;

    if (!stpFriendCnt || DWCi_IsError())
        return;
    else if (!stpFriendCnt->friendList)
    {
        // GP �ʐM�����X�V
        if (stpFriendCnt->pGpObj && *stpFriendCnt->pGpObj)
        {
            gpResult = DWCi_GPProcess();
            // GP�G���[�̏�����gpProcess()����Ă΂��G���[�R�[���o�b�N����
            // �s����
        }
        return;
    }

    if (DWCi_GetPersCallbackLevel() || IsStatsConnected()){
        // Persistent�T�[�o�ւ̐ڑ����A��������Persistent�̃R�[���o�b�N�҂���
        // �Ȃ�Persistent �̒ʐM�����X�V

        s_bInPersistThink = TRUE; // PersistThink()�Ăяo�����ł��邱�Ƃ��L�^
        s_bNeedCloseStatsConnection = FALSE;

        if (!PersistThink()){
            s_bInPersistThink = FALSE;
            DWC_Printf(DWC_REPORTFLAG_WARNING,
                       "Connection to the stats server was lost\n");

            // [note]
            // �����̖߂�l�����́AServerBrowsing SDK �Ɠ��l�ŕK�v�Ȃ�
#if 0
            DWCi_StopFriendProcess(DWC_ERROR_NETWORK,

DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_PERS+DWC_ECODE_TYPE_NETWORK);
            return;
#endif
        }

        s_bInPersistThink = FALSE;

        if(s_bNeedCloseStatsConnection == TRUE)
        {
            s_bNeedCloseStatsConnection = FALSE;
            CloseStatsConnection();  // Persistent�I�u�W�F�N�g�̊J��
        }
    }

    // GP �ʐM�����X�V
    if (stpFriendCnt->pGpObj && *stpFriendCnt->pGpObj){
        gpResult = DWCi_GPProcess();

        // GP�G���[�̏�����gpProcess()����Ă΂��G���[�R�[���o�b�N����
        // �s���邽�߁A�����ł̓G���[�����o�����牽�������ɔ����邾���ŗǂ�
        if (gpResult != GP_NO_ERROR) return;
		
		// ������Ԃɖ߂��Ă���̂�GP�����𑱍s�����FATAL�G���[�ɂȂ邽�߉���R�[�h��ǉ�
        if(stpFriendCnt->state == DWC_FRIEND_STATE_INIT) return; 
		
        // GP�T�[�o���O�C����A��xgpProcess()���Ă΂�Ȃ��ƃ��[�J����
        // �o�f�B���X�g�͍X�V����Ȃ��̂ŁA�����ŗF�B���X�g��
        // �o�^�E�����������Ăяo���B

		// �ȉ��̃R�[�h��DWCi_UpdateServersAsync���Ă΂�A�X�e�[�g������̂��̂ɂȂ�Ȃ��ꍇ�͎��s����Ȃ�
        if (stpFriendCnt->friendList &&
            (stpFriendCnt->buddyUpdateState != DWC_BUDDY_UPDATE_STATE_COMPLETE) &&
            (stpFriendCnt->gpProcessCount > DWC_FRIEND_UPDATE_WAIT_COUNT)){
            if ((stpFriendCnt->buddyUpdateState == DWC_BUDDY_UPDATE_STATE_WAIT) ||
                (stpFriendCnt->buddyUpdateState == DWC_BUDDY_UPDATE_STATE_CHECK)){
                // �o�f�B���X�g�ƃ��[�J���F�B���X�g�̓�������
                DWCi_UpdateFriendReq(stpFriendCnt->friendList,
                                     stpFriendCnt->friendListLen);
            }

            if (stpFriendCnt->buddyUpdateIdx >= stpFriendCnt->friendListLen){
                // �A�b�v�f�[�g����
                stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_COMPLETE;
                stpFriendCnt->svUpdateComplete++;
            }
        }
    }

    if (stpFriendCnt->svUpdateComplete >= 2){
        // �F�B���X�g��������������������A�I���������Ăяo��
        stpFriendCnt->svUpdateComplete = 0;
        DWCi_EndUpdateServers();
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g���������֐�
  �����@�FauthToken          �F�؂Ɏg�����g�[�N���ւ̃|�C���^
          partnerChallenge   �F�؂Ɏg�����`�������W������ւ̃|�C���^
          updateCallback     �F�B�������������R�[���o�b�N
          updateParam        ��L�R�[���o�b�N�p�p�����[�^
          statusCallback     �F�B��ԕω��ʒm�R�[���o�b�N
          statusParam        ��L�R�[���o�b�N�p�p�����[�^
          deleteCallback     �F�B���X�g�폜�R�[���o�b�N
          deleteParam        ��L�R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F���O�C��������AGP�T�[�o��̗F�B���X�g�i�o�f�B���X�g�j��
          ���[�J���̗F�B���X�g�̓����������s��
 *---------------------------------------------------------------------------*/
void DWCi_UpdateServersAsync(const char* authToken,
                             const char* partnerChallenge,
                             DWCUpdateServersCallback updateCallback,
                             void* updateParam,
                             DWCFriendStatusCallback statusCallback,
                             void* statusParam,
                             DWCDeleteFriendListCallback deleteCallback,
                             void* deleteParam)
{
#if 0  // 051007 Persistent�T�[�o�ɂ͕K���A�N�Z�X���Ȃ��悤�ɂ��Ă���
    char response[33];
    int  persResult;
    int  i;
#else
#pragma unused(authToken)
#pragma unused(partnerChallenge)
#endif // 051007 Persistent�T�[�o�ɂ͕K���A�N�Z�X���Ȃ��悤�ɂ��Ă���

    SDK_ASSERT(updateCallback);

    stpFriendCnt->updateCallback = updateCallback;
    stpFriendCnt->updateParam    = updateParam;
    stpFriendCnt->statusCallback = statusCallback;
    stpFriendCnt->statusParam    = statusParam;
    stpFriendCnt->deleteCallback = deleteCallback;
    stpFriendCnt->deleteParam    = deleteParam;

    stpFriendCnt->friendListChanged = 0;  // �F�B���X�g�̕ύX�t���O��������
    // ����gpProcess()�ďo����ɁA�F�B���X�g�o�^�E�����v������������
    stpFriendCnt->buddyUpdateState  = DWC_BUDDY_UPDATE_STATE_WAIT;
    stpFriendCnt->svUpdateComplete  = 0;  // �F�B���X�g�������������t���O�N���A
    stpFriendCnt->buddyUpdateIdx    = 0;
    
    // Persistent�T�[�o���O�C����ԂɈڍs
    stpFriendCnt->state = DWC_FRIEND_STATE_PERS_LOGIN;

    // [arakit] main 051020
    if (stpFriendCnt->friendList == NULL){
        // �F�B���X�g���g��Ȃ��ꍇ�͗F�B���X�g�̓����������I���������Ƃɂ���
        stpFriendCnt->svUpdateComplete++; 
    }
    // [arakit] main 051020

#if 0  // 051007 Persistent�T�[�o�ɂ͕K���A�N�Z�X���Ȃ��悤�ɂ��Ă���
    if (stpFriendCnt->playerName[0] != '\0'){
        // �v���C���[�����Z�b�g����Ă�����Persistent�T�[�o�ɃZ�b�g����
        for (i = 0; i < DWC_DNS_ERROR_RETRY_MAX; i++){
            persResult = InitStatsConnection(0);  // stats��persistentSDK���ʏ�����
    
            if (!persResult) break;
    
            // DNS�G���[�Ȃ���񐔂̓��g���C����
            if ((persResult != GE_NODNS) || (i == DWC_DNS_ERROR_RETRY_MAX-1)){
                DWCi_HandlePersError(persResult);
                return;  // �R�[���o�b�N�ŃG���[��Ԃ�
            }
        }

        // Persistent�T�[�o�ւ̔F�؂��s��
        (void)GenerateAuth(GetChallenge(NULL), partnerChallenge, response);
        PreAuthenticatePlayerPartner(0, authToken, response,
                                     DWCi_PersAuthCallback, NULL);

        DWCi_AddPersCallbackLevel();
    }
    else {
#endif // 051007 Persistent�T�[�o�ɂ͕K���A�N�Z�X���Ȃ��悤�ɂ��Ă���
        // �v���C���[�����Z�b�g����Ă��Ȃ����Persistent�̏�����
        // �I���������Ƃ��L�^���Ă����ɏI���
        stpFriendCnt->svUpdateComplete++;
#if 0  // 051007 Persistent�T�[�o�ɂ͕K���A�N�Z�X���Ȃ��悤�ɂ��Ă���
    }
#endif // 051007 Persistent�T�[�o�ɂ͕K���A�N�Z�X���Ȃ��悤�ɂ��Ă���
}


/*---------------------------------------------------------------------------*
  �F�B�Ǘ������G���[�I���֐�
  �����@�Ferror     DWC�G���[���
          errorCode �G���[�R�[�h
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�Ǘ��������ɃG���[�����������ꍇ�ɁA�����𒆒f����
 *---------------------------------------------------------------------------*/
void DWCi_StopFriendProcess(DWCError error, int errorCode)
{

    if (!stpFriendCnt || (error == DWC_ERROR_NONE)) return;

    DWCi_SetError(error, errorCode);

    if ((stpFriendCnt->state != DWC_FRIEND_STATE_INIT) &&
        (stpFriendCnt->state != DWC_FRIEND_STATE_LOGON)){
        // �F�B���X�g�����������Ȃ�R�[���o�b�N�֐����G���[�ŌĂяo��
        stpFriendCnt->updateCallback(error, stpFriendCnt->friendListChanged,
                                     stpFriendCnt->updateParam);
    }

    DWCi_CloseFriendProcess();
}


/*---------------------------------------------------------------------------*
  GP�o�f�B���N�G�X�g��M�R�[���o�b�N�֐�
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg        GPRecvBuddyRequestArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FGP�o�f�B���N�G�X�g��M�R�[���o�b�N�B
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
void DWCi_GPRecvBuddyRequestCallback(GPConnection* connection,
                                     GPRecvBuddyRequestArg* arg,
                                     void* param)
{
#pragma unused(param)

    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Received buddy request from %u\n",
               arg->profile);

    if (!stpFriendCnt->friendList) return;

    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Begin to search gpInfo.\n");

    // ���Ƀv���t�@�C��ID�������Ă���F�B�ł������Ƃ��Ă��A
    // �K��gpGetInfo�Ńv���t�@�C��ID����lastname���擾����
    gpGetInfo(connection, arg->profile, GP_DONT_CHECK_CACHE,
              GP_NON_BLOCKING, (GPCallback)DWCi_GPGetInfoCallback_RecvBuddyRequest, NULL );
}

/*---------------------------------------------------------------------------*
  GP�o�f�B���X�|���X��M�R�[���o�b�N�֐�
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg        GPRecvBuddyMessageArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�FGP�o�f�B���N�G�X�g�̕ԓ��Ȃ�TRUE
  �p�r�@�FGP�o�f�B���X�|���X��M�R�[���o�b�N�BDWCi_GPRecvBuddyMessageCallback���
          �Ăяo�������B
 *---------------------------------------------------------------------------*/
BOOL DWCi_GPRecvBuddyAuthCallback( GPConnection* connection,
                                   GPRecvBuddyMessageArg* arg,
                                   void* param )
{
#pragma unused(param)
    char* message = arg->message;

    if (!strcmp(message, "I have authorized your request to add me to your list"))
    {
        // ���Ƀv���t�@�C��ID�������Ă���F�B�ł������Ƃ��Ă��A
        // �K��gpGetInfo�Ńv���t�@�C��ID����lastname���擾����
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                   "Received buddy authenticated message from %u.\n",
                   arg->profile);
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Begin to search gpInfo.\n");
            
        gpGetInfo(connection, arg->profile, GP_DONT_CHECK_CACHE,
                  GP_NON_BLOCKING, (GPCallback)DWCi_GPGetInfoCallback_RecvAuthMessage, NULL );
        return TRUE;
    }

    return FALSE;
}


/*---------------------------------------------------------------------------*
  GP�o�f�B��ԕω���M�R�[���o�b�N�֐�
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg        GPRecvBuddyStatusArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FGP�o�f�B��ԕω���M�R�[���o�b�N
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
void DWCi_GPRecvBuddyStatusCallback(GPConnection* connection,
                                    GPRecvBuddyStatusArg* arg,
                                    void* param)
{
#pragma unused(param)
    int friendIdx;
    GPBuddyStatus status;

    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "RECV update friend status. p:%d\n", arg->profile);

    if (stpFriendCnt->statusCallback){
        friendIdx = DWCi_GetFriendListIndex(arg->profile);

        if (friendIdx == -1){
            // ����������ȃ^�C�~���O�Ń��[�J���̗F�B���X�g����������Ă����ꍇ
            return;
        }

        // �G���[�͂Ȃ��͂�
        (void)gpGetBuddyStatus(connection, arg->index, &status);

        // ���[�U���o�^�����R�[���o�b�N���Ăяo��
        stpFriendCnt->statusCallback(friendIdx,
                                     (u8)status.status,
                                     status.locationString,
                                     stpFriendCnt->statusParam);
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�F�B���X�g�ւ̃|�C���^�B�F�B�Ǘ��I�u�W�F�N�g���Ȃ����NULL�B
  �p�r�@�F�F�B�Ǘ��I�u�W�F�N�g���ێ����Ă���F�B���X�g�ւ̃|�C���^���擾����
 *---------------------------------------------------------------------------*/
DWCFriendData* DWCi_GetFriendList(void)
{

    if (stpFriendCnt){
        return stpFriendCnt->friendList;
    }
    else {
        return NULL;
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g���擾�֐�
  �����@�F�Ȃ�
  �߂�l�F�F�B���X�g���B�F�B�Ǘ��I�u�W�F�N�g���Ȃ����0�B
  �p�r�@�F�F�B�Ǘ��I�u�W�F�N�g���ێ����Ă���F�B���X�g�����擾����
 *---------------------------------------------------------------------------*/
int DWCi_GetFriendListLen(void)
{

    if (stpFriendCnt){
        return stpFriendCnt->friendListLen;
    }
    else {
        return 0;
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g����v���t�@�C��ID���擾����֐�
  �����@�Findex �v���t�@�C��ID���擾�������F�B�̗F�B���X�g�C���f�b�N�X
  �߂�l�F0:���X�g�ɖ����ȃf�[�^�i�󂫊܂ށj���A���O�C��ID���������Ă��Ȃ��A
  �@�@�@�@�@�������͗F�B���X�g���Ȃ�
          ��0:�F�B�̃v���t�@�C��ID
  �p�r�@�F���[�J���̗F�B���X�g����A�w�肵���C���f�b�N�X�̗F�B����
          �v���t�@�C��ID���擾����B
          ���̊֐��̓��X�g���󂫂Ȃ̂��A�Q�[���R�[�h�Ⴂ�Ȃ̂��A���O�C��ID
          �Ȃ̂�����ʂ��Ȃ��̂ŁA�Ƃ肠�����v���t�@�C��ID������Ȃ�
          �擾�������Ƃ������Ɏg���B
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
int  DWCi_GetProfileIDFromList(int index)
{
    int profileID;

    if (stpFriendCnt->friendList == NULL) return 0;

    profileID = DWC_GetGsProfileId(DWCi_GetUserData(), &stpFriendCnt->friendList[index]);

    if (!profileID || (profileID == -1)) return 0;
    else return profileID;
}


/*---------------------------------------------------------------------------*
  �F�B���X�g���̃C���f�b�N�X�擾�֐�
  �����@�FprofileID �v���t�@�C��ID
  �߂�l�F�Y������v���t�@�C��ID�̗F�B���X�g���C���f�b�N�X�B�Ȃ��ꍇ��-1��Ԃ�
  �p�r�@�F�v���t�@�C��ID����A���ꂪ���݂���F�B���X�g���̃C���f�b�N�X��Ԃ��B
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
int DWCi_GetFriendListIndex(int profileID)
{
    int i;

    if (!stpFriendCnt || !profileID) return -1;

    for (i = 0; i < stpFriendCnt->friendListLen; i++){
        if (DWCi_GetProfileIDFromList(i) == profileID){
            return i;
        }
    }

    return -1;
}


/*---------------------------------------------------------------------------*
  gpProcess()�Ăяo���񐔃J�E���^�������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FgpProcess()�Ăяo���񐔃J�E���^������������
 *---------------------------------------------------------------------------*/
void DWCi_InitGPProcessCount(void)
{

    if (stpFriendCnt){
        stpFriendCnt->gpProcessCount = 0;
        stpFriendCnt->lastGpProcess  = OS_GetTick();
    }
}


/*---------------------------------------------------------------------------*
  gpStatus�Z�b�g�֐�
  �����@�Fstatus         gpStatus��status�ɃZ�b�g���镶����B
                         ���݂̒l��ύX�������Ȃ����DWC_GP_STATUS_NO_CHANGE��n���B
          statusString   gpStatus��statusString�ɃZ�b�g���镶����B
                         ���݂̕������ύX�������Ȃ����NULL��n���B
          locationString gpStatus��locationString�ɃZ�b�g���镶����
                         ���݂̕������ύX�������Ȃ����NULL��n���B
                         �����̓Q�[����`�̕�����Ƃ��Ďg���̂ŁA���C�u������
                         �㏑�����邱�Ƃ͒ʏ�͂Ȃ��B
  �߂�l�FGP�̏������ʌ^�B���O�C���O�ɌĂ΂ꂽ��GP_NO_ERROR��Ԃ����A
          �����͉������Ȃ��B
  �p�r�@�F������gpStatus���Z�b�g����
 *---------------------------------------------------------------------------*/
GPResult DWCi_SetGPStatus(int status, const char* statusString, const char* locationString)
{

    // statusString, locationString�́A���������������Ă��AgpSetStatus����
    // �G���[�Ȃ��ōő啶�����܂ł̂݃R�s�[�����

#if 1
    // [note]
    // �{���͂��������ׂ��Ȃ̂ɁAelse�̂悤�ɏ����Ȃ��ƁA
    // ���O�C���R�[���o�b�N����DWC_SetOwnStatusData()���Ăяo�����ꍇ��
    // ���΂炭���Ă���Ȃ���GP�G���[�ɂȂ�B�G���[�R�[�h��GP_PARSE�B
    // �ǂ���̏ꍇ�����̔���͔�����gpSetStatus()�܂Ŏ��s�����B
    // ����������
    if ((stpFriendCnt == NULL) || !DWCi_CheckLogin()) return GP_NO_ERROR;
#else
    if ((stpFriendCnt == NULL) || (stpFriendCnt->pGpObj == NULL)) return GP_NO_ERROR;
#endif

    // [note]
    // gpi.h�ɋL�q����Ă���GPIConnection�\���̂��g���Ă���
    if (status == DWC_GP_STATUS_NO_CHANGE){
        status = ((GPIConnection *)*stpFriendCnt->pGpObj)->lastStatus;
    }
    else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Change GP status->status %d\n", status);
    }

    if (!statusString){
        statusString = ((GPIConnection *)*stpFriendCnt->pGpObj)->lastStatusString;
    }
    else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Change GP status->statusString %s\n", statusString);
    }
        
    if (!locationString){
        locationString = ((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString;
    }
    else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Change GP status->locationString %s\n", locationString);
    }

    return gpSetStatus(stpFriendCnt->pGpObj, (GPEnum)status, statusString, locationString);
}


/*---------------------------------------------------------------------------*
  gpStatus�擾�֐�
  �����@�Fstatus         gpStatus��status
          statusString   gpStatus��statusString������
          locationString gpStatus��locationString������
  �߂�l�FTRUE :�擾�����AFALSE:���O�C���O�ȂǂŎ擾���s
  �p�r�@�F���݂̎�����gpStatus���擾����
 *---------------------------------------------------------------------------*/
BOOL DWCi_GetGPStatus(int* status, char* statusString, char* locationString)
{

    if ((stpFriendCnt == NULL) || (*stpFriendCnt->pGpObj == NULL)) return FALSE;

    // [note]
    // gpi.h�ɋL�q����Ă���GPIConnection�\���̂��g���Ă���
    *status = ((GPIConnection *)*stpFriendCnt->pGpObj)->lastStatus;
    strcpy(statusString, ((GPIConnection *)*stpFriendCnt->pGpObj)->lastStatusString);
    strcpy(locationString, ((GPIConnection *)*stpFriendCnt->pGpObj)->lastLocationString);

    return TRUE;
}


/*---------------------------------------------------------------------------*
  �o�f�B�����R�[���o�b�N�Ăяo���֐�
  �����@�Findex �o�f�B�֌W�����������F�B�̗F�B���X�g�C���f�b�N�X
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B���X�g�����������ȊO�Ȃ�A�o�f�B�����R�[���o�b�N�֐����Ăяo��
 *---------------------------------------------------------------------------*/
void DWCi_CallBuddyFriendCallback(int index)
{

    if (stpFriendCnt->buddyCallback &&
        (stpFriendCnt->state != DWC_FRIEND_STATE_PERS_LOGIN)){
        stpFriendCnt->buddyCallback(index, stpFriendCnt->buddyParam);
    }

    // �o�f�B�������͖����I�ɃX�e�[�^�X�X�V�R�[���o�b�N���Ăяo���B
    if (stpFriendCnt->statusCallback){
        u8 st;
        GPBuddyStatus status;

        st = DWC_GetFriendStatus( &stpFriendCnt->friendList[index], status.locationString );
        
        // ���[�U���o�^�����R�[���o�b�N���Ăяo��
        stpFriendCnt->statusCallback(index,
                                     st,
                                     status.locationString,
                                     stpFriendCnt->statusParam);
    }
}


/*---------------------------------------------------------------------------*
  �F�B�Ǘ��I�u�W�F�N�g����֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�Ǘ��I�u�W�F�N�g�ւ̃|�C���^��NULL�N���A����
 *---------------------------------------------------------------------------*/
void DWCi_ShutdownFriend(void)
{
    stpFriendCnt = NULL;
}


//----------------------------------------------------------------------------
// function - static
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  gpProcess()�Ăяo���֐�
  �����@�F�Ȃ�
  �߂�l�FGP�̏������ʌ^
  �p�r�@�FgpProcess()���Ăяo���BgpProcess�̌Ăяo���񐔂��J�E���g���Ă���B
 *---------------------------------------------------------------------------*/
static GPResult DWCi_GPProcess(void)
{
    GPResult result = GP_NO_ERROR;

    if (OS_TicksToMilliSeconds(OS_GetTick()-stpFriendCnt->lastGpProcess) >= DWC_GP_PROCESS_INTERVAL)
    {
        stpFriendCnt->gpProcessCount++; // 40�N���炢�̓��[�v���Ȃ��̂ŁA�΍�Ȃ��B

#if defined(DWC_ENABLE_KEEPALIVE_TO_GPSERVER)
        if ((stpFriendCnt->gpProcessCount % DWC_GP_KEEP_ALIVE_COUNT) == 0)
        {
            // ��莞�Ԃ��Ƃ�GP�̃Z�b�V�������؂��Ȃ��悤��
            // �L�[�v�A���C�u����̊֐����Ăяo��
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "GP:keep alive\n");
                
            (void)gpSetInfoi(stpFriendCnt->pGpObj, GP_INTERESTS1, 0);
        }
#endif
        result = gpProcess(stpFriendCnt->pGpObj);

        stpFriendCnt->lastGpProcess = OS_GetTick();
    }

    return result;
}


/*---------------------------------------------------------------------------*
  �F�B�Ǘ������I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B�Ǘ��I�u�W�F�N�g�����������āA�F�B�Ǘ��������I������
 *---------------------------------------------------------------------------*/
static void DWCi_CloseFriendProcess(void)
{

    if (!stpFriendCnt) return;

#if 0
    CloseStatsConnection();
#endif
    
    // �F�B�Ǘ��I�u�W�F�N�g������
    stpFriendCnt->state = DWC_FRIEND_STATE_INIT;

    // [arakit] main 051027
    stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_WAIT;
    stpFriendCnt->svUpdateComplete = 0;
    // [arakit] main 051027
}


/*---------------------------------------------------------------------------*
  �F�B�o�^�X�V�֐�
  �����@�FfriendList    �F�B���X�g
          friendListLen �X�V���s��friendList�̗v�f��
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B���X�g���Q�Ƃ��ABuddy�o�^�E�������s���B
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
static void DWCi_UpdateFriendReq(DWCFriendData friendList[], int friendListLen)
{
    char userName[21];
    int numBuddy;
    int profileID;
    int i, j;
    GPBuddyStatus status;
    GPResult gpResult;

    // [todo]
    // �����̃G���[�����͂ǂ�������悢��
    // �o�f�B���X�g�����邾���̊֐��̓p�����[�^�G���[�����Ȃ�

    if (stpFriendCnt->buddyUpdateState == DWC_BUDDY_UPDATE_STATE_WAIT){
        // ���GP�o�f�B���X�g�̗F�B�����������ŁA����͂P�񂾂��s��
        // �o�f�B�̐����擾
        gpResult = gpGetNumBuddies(stpFriendCnt->pGpObj, &numBuddy);
        (void)DWCi_HandleGPError(gpResult);

        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "gpGetNumBuddies -> %d\n", numBuddy);
    
        // �F�B�̏������s��
        for (i = 0; i < numBuddy; i++){
            gpResult = gpGetBuddyStatus(stpFriendCnt->pGpObj, i, &status);
            (void)DWCi_HandleGPError(gpResult);
    
            for (j = 0; j < friendListLen; j++){
                if (DWCi_GetProfileIDFromList(j) == status.profile){
                    // �����̃��X�g�ɓ����v���t�@�C��ID������Ό�����F�B
                    if (!DWC_IsBuddyFriendData(&friendList[j])){
                        // �o�f�B�����t���O�������Ă��Ȃ�������A�O��t���O��
                        // �Z�[�u�����O�ɓd����؂�ꂽ�ƍl������̂�
                        // �t���O�𗧂ĂĂ���
                        DWC_SetGsProfileId(&friendList[j], status.profile);
                        DWCi_SetBuddyFriendData(&friendList[j]);

                        stpFriendCnt->friendListChanged = 1;  // �F�B���X�g�ύX����
                    }
                    break;
                }
            }
    
            if (j == friendListLen){
                // �o�f�B���X�g�ɂ����ėF�B���X�g�ɂȂ��v���t�@�C��ID�Ƃ�
                // �o�f�B�֌W����������
                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Deleted buddy %u\n", status.profile);
                           
                gpResult = gpDeleteBuddy(stpFriendCnt->pGpObj, status.profile);
                (void)DWCi_HandleGPError(gpResult);
    
                numBuddy--;
                i--;
            }
        }

        stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_CHECK;
    }

    
    // �F�B�̓o�^���s��
    for ( ; stpFriendCnt->buddyUpdateIdx < friendListLen; stpFriendCnt->buddyUpdateIdx++){
        profileID = DWCi_GetProfileIDFromList(stpFriendCnt->buddyUpdateIdx);
        if (profileID){
            // �F�B���X�g�̃v���t�@�C��ID���L���ȏꍇ

            // ���ɓ����v���t�@�C��ID�����݂��ď������ꂽ�ꍇ�͉������Ȃ�
            if (DWCi_RefreshFriendListForth(friendList, stpFriendCnt->buddyUpdateIdx, profileID))
                continue;
            
            // ���Ƀo�f�B�����t���O�������Ă���ꍇ�́A���ۂɃo�f�B�Ȃ���Ȃ����A
            // GP�o�f�B���X�g�ɂȂ��ꍇ�̓o�f�B���X�g�̃_�E�����[�h����������
            // ���Ȃ������ƍl������̂ŉ������Ȃ�
            //if (DWC_IsBuddyFriendData(&friendList[stpFriendCnt->buddyUpdateIdx]))
            //    continue;
            
            gpResult = gpGetBuddyIndex(stpFriendCnt->pGpObj, profileID, &i);
            (void)DWCi_HandleGPError(gpResult);
        
            if (i == -1){
                // �v���t�@�C��ID���܂��o�f�B���X�g�ɓo�^����Ă��Ȃ����
                // �o�f�B�o�^�v�����s��
                (void)DWCi_GPSendBuddyRequest(profileID);

#ifndef SDK_FINALROM    
                if (DWC_IsBuddyFriendData(&friendList[stpFriendCnt->buddyUpdateIdx])){
                    DWC_Printf(DWC_REPORTFLAG_DEBUG, "Local only friend\n");
                }
#endif
            }
            // ���Ƀo�f�B���X�g���o�^����Ă��ăt�����h�R�[�h�̂܂܂̏ꍇ�́A
            // �ŏ��̗F�B�������[�v�̕��ŏ�������Ă���
        }
        else if (DWC_GetGsProfileId(DWCi_GetUserData(), &friendList[stpFriendCnt->buddyUpdateIdx]) == -1){
            // �F�B���X�g�Ƀ��O�C��ID���Z�b�g����Ă���ꍇ
            // GP�ɖ₢���킹�ăv���t�@�C��ID���擾���Ă���K�v�����邽��
            // �K�v�ȃf�[�^��param�ɓn���Ĕ񓯊��֐����Ăяo���A
            // �����̏����̓R�[���o�b�N�֐����ōs��
            // ���O�C��ID����GP�v���t�@�C���̃��X�g�l�[�����擾
            DWC_LoginIdToUserName(DWCi_GetUserData(),
                                  &friendList[stpFriendCnt->buddyUpdateIdx],
                                  userName);
            // ���X�g�l�[������F�B������
            gpProfileSearch(stpFriendCnt->pGpObj,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            userName,
                            NULL,
                            GP_NON_BLOCKING,
                            (GPCallback)DWCi_GPProfileSearchCallback,
                            (void *)stpFriendCnt->buddyUpdateIdx);

            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Called gpProfileSearch().\n");

            stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_PSEARCH;
            break;
        }
    }

    return;
}


/*---------------------------------------------------------------------------*
  �F�B���X�g���������I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F�F�B���X�g���������I�����̏������s��
 *---------------------------------------------------------------------------*/
static void DWCi_EndUpdateServers(void)
{
    
    // �R�[���o�b�N���Ăяo��
    stpFriendCnt->updateCallback(DWC_ERROR_NONE,
                                 stpFriendCnt->friendListChanged,
                                 stpFriendCnt->updateParam);

    stpFriendCnt->state = DWC_FRIEND_STATE_LOGON;
}


/*---------------------------------------------------------------------------*
  �F�B���X�g��̗F�B�������֐��i�R�[���o�b�N�Ăяo���t���j
  �����@�FfriendList  �F�B���X�g
          deleteIndex �����������F�B���̃C���f�b�N�X
          otherIndex  ���ɂ��R�[���o�b�N�ɓn�������F�B���X�g�C���f�b�N�X
  �߂�l�F�Ȃ�
  �p�r�@�F���[�J���̗F�B���X�g����F�B�����������A�Q�[������o�^���ꂽ
          �폜�R�[���o�b�N�֐����Ăяo���B
          �Q�[�����ňӐ}���Ȃ��폜�ɂ��Ă͕K�����̊֐����Ăяo���B
 *---------------------------------------------------------------------------*/
static void DWCi_DeleteFriendFromList(DWCFriendData friendData[], int deleteIndex, int otherIndex)
{

    if (stpFriendCnt){
        // �F�B���N���A
        MI_CpuClear8(&friendData[deleteIndex], sizeof(DWCFriendData));
        
        if (stpFriendCnt->deleteCallback){
            // �Q�[�����œo�^���ꂽ�폜�R�[���o�b�N���Ăяo��
            stpFriendCnt->deleteCallback(deleteIndex, otherIndex,
                                         stpFriendCnt->deleteParam);
        }
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�����֐�
  �����@�FfriendList �F�B���X�g�ւ̃|�C���^
          index      �d�����m�F�������F�B�̗F�B���X�g�C���f�b�N�X
          profileID  �d�����m�F�������F�B�̃v���t�@�C��ID
  �߂�l�FTRUE:�F�B���X�g�̓��e��ύX�����AFALSE:�F�B���X�g�̓��e��ύX���Ȃ�����
  �p�r�@�F�w�肵���F�B���X�g�C���f�b�N�X�ƃv���t�@�C��ID�̗F�B�ɂ��āA
          �w��̃C���f�b�N�X���O�̃C���f�b�N�X�ɁA�d�����Ă���
          �v���t�@�C��ID���Ȃ����𒲂ׁA�������玩��̗F�B������������B
          �������A�w��̗F�B��񂪃o�f�B�����ς݂ŁA�O�̃C���f�b�N�X��
          �F�B��񂪃o�f�B�����ς݂łȂ��ꍇ�̂݁A�O�̃C���f�b�N�X��
          �F�B���̕�����������B
 *---------------------------------------------------------------------------*/
static BOOL DWCi_RefreshFriendListForth(DWCFriendData friendList[], int index, int profileID)
{
    int pid_i;
    int i;

    for (i = 0; i < index; i++){
        pid_i = DWCi_GetProfileIDFromList(i);
        if (pid_i && (pid_i == profileID)){
            // �d������������폜����
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                       "Found same friend in the list [%d] & [%d], %d\n",
                       i, index, DWC_GetFriendDataType(&friendList[i]));

            if (DWC_IsBuddyFriendData(&friendList[index]) &&
                !DWC_IsBuddyFriendData(&friendList[i])){
                // �������Ǝv���Ă����F�B���Ńo�f�B�����ς݃t���O�������Ă���A
                // ���f�[�^�Ǝv���Ă����F�B���Ńo�f�B�����ς݃t���O��
                // �����Ă��Ȃ��ꍇ�̂݁A���f�[�^�̕�����������
                DWCi_DeleteFriendFromList(friendList, i, index);
            }
            else {
                // �ʏ�͌��f�[�^���c���A�w��̃C���f�b�N�X�̃f�[�^����������
                DWCi_DeleteFriendFromList(friendList, index, i);
            }

            stpFriendCnt->friendListChanged = 1;  // �F�B���X�g�ύX����
            return TRUE;
        }
    }

    return FALSE;
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�����֐��i�S�̒����Łj
  �����@�FfriendList    �F�B���X�g�ւ̃|�C���^
          friendListLen �F�B���X�g�̍ő咷�i�v�f���j
          profileID     �C���f�b�N�X�𒲂ׂ����v���t�@�C��ID
  �߂�l�F����profileID�̃C���f�b�N�X�B�F�B���X�g�ɂȂ��ꍇ��-1��Ԃ��B
  �p�r�@�F�F�B���X�g��S�Ē��ׁA�d�����Ă���v���t�@�C��ID�����������������B
          �ŏI�I�Ɉ���profileID�����Ԃ̃C���f�b�N�X�Ƃ��Ďc��������Ԃ��B
 *---------------------------------------------------------------------------*/
static int DWCi_RefreshFriendListAll(DWCFriendData friendList[], int friendListLen, int profileID)
{
    int  index = -1;
    int  pid_i;
    int  i, j;

    for (i = 0; i < friendListLen; i++){
        pid_i = DWCi_GetProfileIDFromList(i);
        if (pid_i == 0) continue;

        // �Ō�܂Ŏc��profileID�̃C���f�b�N�X��ޔ�
        if (pid_i == profileID) index = i;

        for (j = i+1; j < friendListLen; j++){
            if (pid_i == DWCi_GetProfileIDFromList(j)){
                // �v���t�@�C��ID�̏d�����������ꍇ
                if ((DWC_GetFriendDataType(&friendList[i]) == DWC_FRIENDDATA_FRIEND_KEY) &&
                    (DWC_GetFriendDataType(&friendList[j]) == DWC_FRIENDDATA_GS_PROFILE_ID)){
                    // i���F�B�o�^����j���v���t�@�C��ID�̏ꍇ�A
                    // �܂���i�̗F�B�����v���t�@�C��ID�̏�Ԃɂ���
                    DWC_SetGsProfileId(&friendList[i], pid_i);
                }

                if (DWC_IsBuddyFriendData(&friendList[j])){
                    // j���o�f�B�����ς݂̏ꍇ�Ai�̗F�B�����o�f�B�����ς݂ɂ���
                    DWCi_SetBuddyFriendData(&friendList[i]);
                }

                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                           "Found same friend in the list [%d] & [%d], %d\n",
                           i, j, DWC_GetFriendDataType(&friendList[i]));

                // j�̗F�B�����폜����
                DWCi_DeleteFriendFromList(friendList, j, i);

                stpFriendCnt->friendListChanged = 1;  // �F�B���X�g�ύX����
            }
        }
    }

    return index;
}


/*---------------------------------------------------------------------------*
  �o�f�B���N�G�X�g���M�֐�
  �����@�FprofileID �o�f�B���N�G�X�g���M��v���t�@�C��ID
  �߂�l�FGP�̏������ʌ^
  �p�r�@�F�o�f�B���N�G�X�g�𑗐M����B�G���[�����͒��ōs���B
 *---------------------------------------------------------------------------*/
static GPResult DWCi_GPSendBuddyRequest(int profileID)
{
    GPResult gpResult;
    
    gpResult = gpSendBuddyRequest(stpFriendCnt->pGpObj,
                                  profileID,
                                  ""/*(gsi_char *)stpFriendCnt->playerName*/);
    (void)DWCi_HandleGPError(gpResult);

    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
               "Send buddy request to %u\n",
               profileID);

    return gpResult;
}


/*---------------------------------------------------------------------------*
  �F�BBuddyStatus�擾�֐�
  �����@�FfriendData �F�B���ւ̃|�C���^
          status     BuddyStatus�i�[��|�C���^
  �߂�l�FTRUE:�o�f�B�X�e�[�^�X�擾�����AFALSE:�o�f�B�X�e�[�^�X�擾���s
  �p�r�@�F�F�B���X�g�̃C���f�b�N�X���w�肵�āA���̗F�B��BuddyStatus���擾����
 *---------------------------------------------------------------------------*/
static BOOL DWCi_GetFriendBuddyStatus(const DWCFriendData* friendData, GPBuddyStatus* status)
{
    u8  ret;
    int buddyIdx = 0;
    int profileid;

    SDK_ASSERT(friendData);
    SDK_ASSERT(status);

    if (!stpFriendCnt || !DWCi_CheckLogin()) return FALSE;

    profileid = DWC_GetGsProfileId(DWCi_GetUserData(), friendData);
    if (profileid > 0 && gpGetBuddyIndex(stpFriendCnt->pGpObj,
                                         profileid,
                                         &buddyIdx)){
        // �G���[�̓p�����[�^�G���[�����Ȃ��̂ŃI�t���C�������ŏ[��
        ret = FALSE;
    }
    else if (profileid <= 0 || buddyIdx == -1){
        // �܂��o�f�B���o�f�B���X�g�ɓo�^����Ă��Ȃ��ꍇ���I�t���C�������B
        // ���O�C���R�[���o�b�N���ŌĂ΂ꂽ�ꍇ�ȂǁA�܂��o�f�B���X�g��
        // �S�Ď擾�ł��Ă��Ȃ��ꍇ�������ɗ���B
        ret = FALSE;
    }
    else {
        // �o�f�B�����擾
        if (gpGetBuddyStatus(stpFriendCnt->pGpObj, buddyIdx, status)){
            // �G���[�̓p�����[�^�G���[�����Ȃ��̂ŃI�t���C�������ŏ[��
            ret = FALSE;
        }
        else {
            ret = TRUE;
        }
    }

    return ret;
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

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Friend, GP error %d\n", result);

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
    errorCode += DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_GP;

    // �F�B�Ǘ��������f�֐����Ăяo��
    DWCi_StopFriendProcess(dwcError, errorCode);

    return result;
}


/*---------------------------------------------------------------------------*
  Persistent�G���[�����֐�
  �����@�Ferror Persistent�̃G���[�^
  �߂�l�FPersistent�̃G���[�^�i���������̂܂ܕԂ��j
  �p�r�@�FPersisten�G���[�l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���āA�Ή�����R�[���o�b�N���Ăяo��
 *---------------------------------------------------------------------------*/
static int DWCi_HandlePersError(int error)
{
    int errorCode;
    DWCError dwcError;

    if (error == GE_NOERROR) return GE_NOERROR;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Friend, persistent error %d\n", error);

    switch (error){
    case GE_NOSOCKET:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SOCKET;
        break;
    case GE_NODNS:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DNS;
        break;
    case GE_NOCONNECT:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SERVER;
        break;
    case GE_DATAERROR:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_DATA;
        break;
    case GE_TIMEDOUT:
        dwcError  = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_TIMEOUT;
        break;
    }
    // InitStatsAsync()���g��Ȃ��̂�GE_CONNECTING�͗��Ȃ��͂�

#if 0
    // �x�[�X�G���[�R�[�h��ǉ�
    errorCode += DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_PERS;

    // �F�B�Ǘ��������f�֐����Ăяo��
    DWCi_StopFriendProcess(dwcError, errorCode);

#else
    // �x�[�X�G���[�R�[�h��ǉ�
    errorCode += DWC_ECODE_SEQ_ETC+DWC_ECODE_GS_PERS;

    if (stPersState == DWC_PERS_STATE_LOGIN){
        DWCi_StopPersLogin(dwcError, errorCode);
    }
#endif
        
    return error;
}


/*---------------------------------------------------------------------------*
  GP�v���t�@�C���T�[�`�����R�[���o�b�N�֐�
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg        GPProfileSearchResponseArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FGP�v���t�@�C���T�[�`�����R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GPProfileSearchCallback(GPConnection* connection,
                                         GPProfileSearchResponseArg* arg,
                                         void* param)
{
    int idx;
    GPResult gpResult;
    int friendIdx = (int)param;

    DWC_Printf( DWC_REPORTFLAG_DEBUG, "ProfileSearch: num:%d m:%x\n", arg->numMatches, arg->more );

    if ((arg->result == GP_NO_ERROR) && arg->numMatches &&
        (DWC_GetFriendDataType(&stpFriendCnt->friendList[friendIdx]) != DWC_FRIENDDATA_NODATA)){
        int match_index;
        // �v���t�@�C������������
        if (arg->numMatches > 1){
            // ������Q�ȏ�}�b�`����v���t�@�C�����������Ă��܂����ꍇ
            // ���ɋH�ȃP�[�X�Ȃ̂ŁA���ɑ΍�͂��Ȃ�
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Happned to find %d profiles.\n",
                       arg->numMatches);
        }

        if (stpFriendCnt->state == DWC_FRIEND_STATE_PERS_LOGIN){
            // ���ׂĂ̌��������v���t�@�C���ɂ��āA�`�F�b�N����B
            for ( match_index = 0 ; match_index < arg->numMatches ; ++match_index )
            {
                // ���ɂ���v���t�@�C��ID�����������ꍇ�́A�����߂�B
                // �v���t�@�C��ID�̏d�����`�F�b�N����
                if (DWCi_RefreshFriendListForth(stpFriendCnt->friendList,
                                                friendIdx,
                                                arg->matches[match_index].profile)){
                    // �V���Ɏ擾�����v���t�@�C��ID�͊��ɓo�^�ς݂������̂�
                    // ���������ꍇ�B
                    // �������̗F�B���X�g�C���f�b�N�X��i�߁A���������𑱂���
                    stpFriendCnt->buddyUpdateIdx++;
                    stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_CHECK;
                    arg->more = GP_DONE; // �I���B
                    return;
                }
            }

            // ���ׂĂ̌��������v���t�@�C���ɂ��āA�`�F�b�N����B
            for ( match_index = 0 ; match_index < arg->numMatches ; ++match_index )
            {
                gpResult = gpGetBuddyIndex(connection, arg->matches[match_index].profile, &idx);
                (void)DWCi_HandleGPError(gpResult);

                if (idx == -1){
                    // �v���t�@�C��ID���܂��o�f�B���X�g�ɓo�^�����
                    // ���Ȃ���΃o�f�B���N�G�X�g���s��
                    (void)DWCi_GPSendBuddyRequest(arg->matches[match_index].profile);
                }
                else {
                    // �v���t�@�C��ID�����Ƀo�f�B���X�g�ɓo�^����Ă����ꍇ��
                    // �O�񃍃O�C��ID���o�^���ꂽ��A�o�f�B�֌W�����������̂ɁA
                    // �Z�[�u�����ɓd����؂�ꂽ�ꍇ�ƍl������̂ŁA
                    // �v���t�@�C��ID��o�^���A�o�f�B�����t���O�����ĂĂ���
                    DWC_SetGsProfileId(&stpFriendCnt->friendList[friendIdx],
                                       arg->matches->profile);
                    DWCi_SetBuddyFriendData(&stpFriendCnt->friendList[friendIdx]);
                    // �o�f�B�����R�[���o�b�N���Ăяo��
                    DWCi_CallBuddyFriendCallback(friendIdx);

                    stpFriendCnt->buddyUpdateIdx++;
                    stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_CHECK;
                    arg->more = GP_DONE; // �I���B
		            stpFriendCnt->friendListChanged = 1;  // �F�B���X�g�ύX����
                    return;
                }
            }

            if ( arg->more != GP_MORE )
            {
                // �F�B���X�g����M�I�������̂ŁA��������M���邽�߁A��x�߂�B
                // �������̗F�B���X�g�C���f�b�N�X��i�߁A���������𑱂���
                stpFriendCnt->buddyUpdateIdx++;
                stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_CHECK;
            }
            else
            {
                DWC_Printf(DWC_REPORTFLAG_DEBUG, "more profiles will come...\n");
            }
        }
    }
    else if (arg->result){
        // �G���[����
        if (DWCi_HandleGPError(arg->result)) return;
    }
    else if ((stpFriendCnt->state == DWC_FRIEND_STATE_PERS_LOGIN) ||
             (DWC_GetFriendDataType(&stpFriendCnt->friendList[friendIdx]) == DWC_FRIENDDATA_NODATA)){
        // �Y���v���t�@�C����������Ȃ������ꍇ��
        // �������̗F�B���X�g�C���f�b�N�X��i�߁A���������𑱂���B
        // ProfileSearchCallback������O�ɁAGetInfo�R�[���o�b�N����
        // �F�B��񂪍폜����Ă����ꍇ�������ɗ���
        stpFriendCnt->buddyUpdateIdx++;
        stpFriendCnt->buddyUpdateState = DWC_BUDDY_UPDATE_STATE_CHECK;
    }
}


/*---------------------------------------------------------------------------*
  GP GPInfo��M�R�[���o�b�N�֐��i�o�f�B���N�G�X�g��M���p�j
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg        GPGetInfoResponseArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�o�f�B���N�G�X�g��M���p�AGPInfo��M�R�[���o�b�N
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
static void DWCi_GPGetInfoCallback_RecvBuddyRequest(GPConnection* connection,
                                                    GPGetInfoResponseArg* arg,
                                                    void* param)
{
#pragma unused(param)
    char userName[21];
	char gamecode_str[5];
	unsigned int gamecode;
    GPResult gpResult;
    int i;
    BOOL confirm = FALSE;

    // [arakit] main 051013
    if (arg->result != GP_NO_ERROR)
    {
        DWC_Printf( DWC_REPORTFLAG_LOGIN, " ERROR: GetInfo Req. why??? : %d\n", arg->result );
        return;
    }
    // [arakit] main 051013

    // [arakit] main 051012
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "GetInfo Req: profileID %u, lastname '%s'.\n",
               arg->profile, arg->lastname);

    for (i = 0; i < stpFriendCnt->friendListLen; i++)
    {
        if ( DWC_GetFriendDataType( &stpFriendCnt->friendList[i] ) == DWC_FRIENDDATA_LOGIN_ID )
        {
            DWC_LoginIdToUserName(DWCi_GetUserData(),
                                  &stpFriendCnt->friendList[i],
                                  userName);

            if (strcmp(userName, arg->lastname) == 0)
            {
                // �o�f�B���N�G�X�g�����F����
                gpResult = gpAuthBuddyRequest(connection, arg->profile);
                // �F�B�������O�C��ID����v���t�@�C��ID�ɒu��������
                DWC_SetGsProfileId(&stpFriendCnt->friendList[i], arg->profile);

                confirm = TRUE;

                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                           "Auth buddy request from %u, friend[%d]mp.\n",
                           arg->profile, i);
            }
        }
        else if ( DWC_GetFriendDataType( &stpFriendCnt->friendList[i] ) == DWC_FRIENDDATA_GS_PROFILE_ID ||
                  DWC_GetFriendDataType( &stpFriendCnt->friendList[i] ) == DWC_FRIENDDATA_FRIEND_KEY )
        {
			// �Q�[���R�[�h�̊m�F
			gamecode = DWCi_GetUserData()->gamecode;
			OS_SNPrintf( gamecode_str, 5, "%c%c%c%c", (u8)(gamecode >> 24), (u8)(gamecode >> 16), (u8)(gamecode >> 8), (u8)gamecode);
            // �v���t�@�C��ID�x�[�X�̂Ƃ�
            if ( DWC_GetGsProfileId( DWCi_GetUserData(), &stpFriendCnt->friendList[i] ) == arg->profile &&
			     memcmp(gamecode_str, arg->lastname+DWC_ACC_USERNAME_GSBRCD_OFFSET, 4) == 0)
            {
                // �o�f�B���N�G�X�g�����F����
                gpResult = gpAuthBuddyRequest(connection, arg->profile);

                confirm = TRUE;

                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                           "Auth buddy request from %u, friend[%d]gs.\n",
                           arg->profile, i);
            }
        }
    }

    if ( confirm )
    {
        // �����炩����o�f�B���N�G�X�g�𑗐M����B
        // �I�����C�����ɑ���Ƀo�f�B���N�G�X�g���͂��Ȃ��ƍ���̂�
        // �ߏ�ɂȂ��Ă��ǂ�����K�����M����B
        (void)DWCi_GPSendBuddyRequest(arg->profile);
    }
    else {
        // �F�B���X�g�ɂȂ��F�B����̃o�f�B���N�G�X�g�������ꍇ�͗v�������ۂ���
        gpDenyBuddyRequest(connection, arg->profile);

        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Denied buddy request from %u.\n",
                   arg->profile);
    }
}


/*---------------------------------------------------------------------------*
  GP GPInfo��M�R�[���o�b�N�֐��i�o�f�B���F�R�[���o�b�N��M���p�j
  �����@�Fconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg        GPGetInfoResponseArg�^�I�u�W�F�N�g�ւ̃|�C���^
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�o�f�B���F�R�[���o�b�N��M���p�AGPInfo��M�R�[���o�b�N
          DWC_GetUserData()���g���̂ŁADWC_InitFriendsMatch()���O�ɌĂԂ悤��
          �ύX���Ă͂Ȃ�Ȃ��B
 *---------------------------------------------------------------------------*/
static void DWCi_GPGetInfoCallback_RecvAuthMessage(GPConnection* connection,
                                                   GPGetInfoResponseArg* arg,
                                                   void* param)
{
#pragma unused(connection)
#pragma unused(param)
    char userName[21];
    int index;
    int i;
    BOOL listChanged  = FALSE;
    BOOL alreadyBuddy = FALSE;

    // [arakit] main 051013
    if (arg->result != GP_NO_ERROR)
    {
        DWC_Printf( DWC_REPORTFLAG_LOGIN, " ERROR: GetInfo Auth. why??? : %d\n", arg->result );
        return;
    }
    // [arakit] main 051013

    // [arakit] main 051012
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "GetInfo Auth: profileID %u, lastname '%s'.\n",
               arg->profile, arg->lastname);

    // �F�B���X�g���������ĉ����O�C��ID�Ȃ��r����
    for (i = 0; i < stpFriendCnt->friendListLen; i++)
    {
        if ( DWC_GetFriendDataType( &stpFriendCnt->friendList[i] ) == DWC_FRIENDDATA_LOGIN_ID )
        {
            // ��ID�̂Ƃ�
            DWC_LoginIdToUserName(DWCi_GetUserData(),
                                  &stpFriendCnt->friendList[i],
                                  userName);

            if (strcmp(userName, arg->lastname) == 0)
            {
                // �F�B���X�g�����O�C��ID����v���t�@�C��ID�ɒu�������ėF�B�֌W������
                DWC_SetGsProfileId(&stpFriendCnt->friendList[i], arg->profile);
                DWCi_SetBuddyFriendData(&stpFriendCnt->friendList[i]);
                listChanged = TRUE;

                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                           "Established buddy with %u, friend[%d]mp.\n",
                           arg->profile, i);

            }
        }
        else if ( DWC_GetFriendDataType( &stpFriendCnt->friendList[i] ) == DWC_FRIENDDATA_GS_PROFILE_ID ||
                  DWC_GetFriendDataType( &stpFriendCnt->friendList[i] ) == DWC_FRIENDDATA_FRIEND_KEY )
        {
            // �v���t�@�C��ID���F�B�o�^���̂Ƃ�
            if ( DWC_GetGsProfileId( DWCi_GetUserData(), &stpFriendCnt->friendList[i] ) == arg->profile )
            {
                if( DWC_IsBuddyFriendData( &stpFriendCnt->friendList[i] ) == TRUE )
                {
                    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, 
                               "This profile is already my friend[%d].\n", i);
                    alreadyBuddy = TRUE;
                }
                else
                {
                    // �F�B���X�g��F�B�o�^������v���t�@�C��ID�ɒu�������ėF�B�֌W������
                    DWC_SetGsProfileId(&stpFriendCnt->friendList[i], arg->profile);
                    DWCi_SetBuddyFriendData(&stpFriendCnt->friendList[i]);
                    listChanged = TRUE;

                    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                               "Established buddy with %u, friend[%d]gs.\n",
                               arg->profile, i);
                }
            }
        }
    }

    // �F�B���X�g�̕ύX���������ꍇ�̓��X�g�̐����ƁA�K�v�ɉ����ăo�f�B�����R�[���o�b�N���Ăяo��
    if ( listChanged )
    {
        // �v���t�@�C��ID�̏d���������ď�������
        index = DWCi_RefreshFriendListAll(stpFriendCnt->friendList,
                                          stpFriendCnt->friendListLen,
                                          arg->profile);

        // �o�f�B�����R�[���o�b�N���Ăяo��
        if ( !alreadyBuddy )
        {
            DWCi_CallBuddyFriendCallback(index);
        }

        stpFriendCnt->friendListChanged = 1;  // �F�B���X�g�ύX����
    }
    else
    {
        if ( !alreadyBuddy )
        {
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Not Established buddy with %u.\n",
                       arg->profile);
        }
    }
}


/*---------------------------------------------------------------------------*
  Persistent�T�[�o���O�C���I���֐�
  �����@�Ferror     DWC�G���[���
          errorCode �G���[�R�[�h
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�T�[�o���O�C���������ɃG���[�����������ꍇ�ɁA�����𒆒f����
 *---------------------------------------------------------------------------*/
static void DWCi_StopPersLogin(DWCError error, int errorCode)
{

    if (!stpFriendCnt || (error == DWC_ERROR_NONE)) return;

    DWCi_SetError(error, errorCode);

    // Persistent�T�[�o���烍�O�A�E�g����
    if(IsStatsConnected())
		DWC_LogoutFromStorageServer();

    if (stpFriendCnt->persLoginCallback != NULL){
        stpFriendCnt->persLoginCallback(error, stpFriendCnt->persLoginParam);
    }
}


/*---------------------------------------------------------------------------*
  Persistent�T�[�o�Z�[�u�֐�
  �����@�FprofileID �Z�[�u��̃v���t�@�C��ID
          type      �f�[�^�A�N�Z�X�^�C�v
          keyvalues �f�[�^�x�[�X������ikey/value�l�j�ւ̃|�C���^
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�T�[�o�Ƀf�[�^���Z�[�u����B
 *---------------------------------------------------------------------------*/
static void DWCi_SetPersistDataValuesAsync(int profileID,
                                           persisttype_t type,
                                           gsi_char* keyvalues,
                                           void* param)
{

    DWCi_AddPersCallbackLevel();

    SetPersistDataValues(0, profileID, type, 0, keyvalues,
                         DWCi_PersDataSaveCallback, param);
}


/*---------------------------------------------------------------------------*
  Persistent�T�[�o���[�h�֐�
  �����@�FprofileID ���[�h��̃v���t�@�C��ID
          type      �f�[�^�A�N�Z�X�^�C�v
          keys      �f�[�^�x�[�X������ikey�l�j�ւ̃|�C���^
          param     �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�T�[�o����f�[�^�����[�h����B
 *---------------------------------------------------------------------------*/
static void DWCi_GetPersistDataValuesAsync(int profileID,
                                           persisttype_t type,
                                           gsi_char* keys,
                                           void* param)
{

    DWCi_AddPersCallbackLevel();

    GetPersistDataValues(0, profileID, type, 0, keys,
                         DWCi_PersDataCallback, param);
}


/*---------------------------------------------------------------------------*
  Persistent�T�[�o�F�؃R�[���o�b�N�֐�
  �����@�Flocalid       �F�؂��s�������[�U�̃��[�J��ID
          profileid     �F�؂��s�������[�U�̃v���t�@�C��ID
          authenticated TRUE:�F�ؐ����AFALSE:�F�؎��s
          errmsg        �F�؎��s�̏ꍇ�̃G���[���b�Z�[�W
          instance      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�T�[�o�F�؃R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_PersAuthCallback(int localid, int profileid,
                                  int authenticated, gsi_char* errmsg,
                                  void* instance)
{
#ifdef SDK_FINALROM
#pragma unused(errmsg)
#endif
#pragma unused(instance)
#if 0
    char tmpStr[DWC_MAX_PLAYER_NAME*2];
#endif

    DWCi_SubPersCallbackLevel();
    
    if (!authenticated || (localid != 0)){
        // �F�؎��s
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Stats server authentication failed.\n");
        DWC_Printf(DWC_REPORTFLAG_ERROR, "%s\n", errmsg);

#if 0
        // �F�B�Ǘ��������f�֐����Ăяo��
        DWCi_StopFriendProcess(DWC_ERROR_NETWORK,
                               DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_PERS+DWC_ECODE_TYPE_STATS_AUTH);
#else
        DWCi_StopPersLogin(DWC_ERROR_NETWORK,
                           DWC_ECODE_SEQ_ETC+DWC_ECODE_GS_PERS+DWC_ECODE_TYPE_STATS_AUTH);
#endif
        return;
    }
    else {
        // �F�ؐ���
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Stats server authentication succeeded.\n");

        stpFriendCnt->profileID = profileid;

#if 0
        // �v���C���[���̃L�[���쐬
        (void)OS_SNPrintf(tmpStr, sizeof(tmpStr),
                          "\\dwc_name\\%s",
                          stpFriendCnt->playerName);
        // �v���C���[����Persistent�T�[�o�ɃZ�[�u����
        DWCi_SetPersistDataValuesAsync(stpFriendCnt->profileID, pd_public_rw,
                                       tmpStr,
                                       (void *)DWC_PERS_KEY_ID_PLAYER_NAME);

        // �v���C���[���̃L�[�o�^���I����܂ł�Persistent�T�[�o
        // ���O�C����Ԃ̂܂܂ɂ��Ă���

#else
        stPersState = DWC_PERS_STATE_CONNECTED;

        // �R�[���o�b�N���Ăяo��
        if (stpFriendCnt->persLoginCallback != NULL){
            stpFriendCnt->persLoginCallback(DWC_ERROR_NONE, stpFriendCnt->persLoginParam);
        }
#endif
    }
}


/*---------------------------------------------------------------------------*
  Persistent�f�[�^�擾�R�[���o�b�N�֐�
  �����@�Flocalid   �f�[�^�������[�U�̃��[�J��ID
          profileid �f�[�^�������[�U�̃v���t�@�C��ID
          type      �A�N�Z�X�^�C�v
          index     �L�[���C���f�b�N�X
          success   TRUE:�擾�����AFALSE:�擾���s
          modified  �f�[�^���C�����ꂽ����
          data      �擾�����f�[�^
          len       �擾�f�[�^��
          instance  �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�f�[�^�擾�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_PersDataCallback(int localid, int profileid,
                                  persisttype_t type, int index, int success,
                                  time_t modified, char* data, int len,
                                  void* instance)
{
#pragma unused(localid)
#pragma unused(type)
#pragma unused(index)
#pragma unused(modified)
    BOOL result;

    DWCi_SubPersCallbackLevel();

    if (success){
        if (len == 0){
            // �L�[�����݂��Ȃ������ꍇ�B
            // �������͑��l��Private�L�[�����[�h���悤�Ƃ����ꍇ�B
            DWC_Printf(DWC_REPORTFLAG_WARNING,
                       "Persitent, Specified key is not exist.\n");
            // ���s�̃R�[���o�b�N��Ԃ�
            result = FALSE;
        }
        else {
            // �L�[�����݂����ꍇ
            DWC_Printf(DWC_REPORTFLAG_DEBUG,
                       "Loaded data from persistent server. -> %s\n", data);
            result = TRUE;
        }

        if (stpFriendCnt->loadCallback){
            // �Q�[������o�^���ꂽ�R�[���o�b�N���Ăяo��
            stpFriendCnt->loadCallback(result, DWCi_GetFriendListIndex(profileid), data, len, instance);
        }
    }
    else {
        // �G���[����
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Failed to load persistent data.\n");

#if 0
        // �F�B�Ǘ��������f�֐����Ăяo��
        DWCi_StopFriendProcess(DWC_ERROR_NETWORK,
                               DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_PERS+DWC_ECODE_TYPE_STATS_LOAD);
        return;
#endif

        // �Q�[������o�^���ꂽ�R�[���o�b�N���Ăяo��
        if (stpFriendCnt->loadCallback){
            stpFriendCnt->loadCallback(FALSE, DWCi_GetFriendListIndex(profileid), data, len, instance);
        }
    }
}


/*---------------------------------------------------------------------------*
  Persistent�f�[�^�Z�[�u�R�[���o�b�N�֐�
  �����@�Flocalid   �f�[�^���Z�[�u�������[�U�̃��[�J��ID
          profileid �f�[�^���Z�[�u�������[�U�̃v���t�@�C��ID
          type      �A�N�Z�X�^�C�v
          index     �L�[���C���f�b�N�X
          success   TRUE:�Z�[�u�����AFALSE:�Z�[�u���s
          modified  �f�[�^���Z�[�u���ꂽ����
          instance  �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�f�[�^�Z�[�u�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_PersDataSaveCallback(int localid, int profileid,
                                      persisttype_t type, int index, int success,
                                      time_t modified, void* instance)
{
#pragma unused(localid)
#pragma unused(profileid)
#pragma unused(index)
#pragma unused(modified)
    u32  flag = (u32)instance;
    BOOL isPublic;

    DWCi_SubPersCallbackLevel();

#if 0
    if (flag == DWC_PERS_KEY_ID_PLAYER_NAME){
        if (success){
            // �{����dwc_name�̃Z�[�u�R�[���o�b�N���m�F����
            if (stpFriendCnt->state == DWC_FRIEND_STATE_PERS_LOGIN){
                // �v���C���[�����Z�[�u����
                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                           "Saved player name to persistent server.\n");

                // Persistent�̏������I���������Ƃ��L�^����
                stpFriendCnt->svUpdateComplete++;
            }
            else {
                // ���܂��܃p�����[�^���ꏏ�ɂȂ�������������
                flag = 0;
            }
        }
        else {
            // �G���[����
            DWC_Printf(DWC_REPORTFLAG_ERROR, "Failed to save persistent data.\n");

            // �F�B�Ǘ��������f�֐����Ăяo��
            DWCi_StopFriendProcess(DWC_ERROR_NETWORK,
                                   DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_PERS+DWC_ECODE_TYPE_STATS_SAVE);
        }
    }

    // DWC���䉺�̃f�[�^�Z�[�u���͂����܂łŏI��
    if (flag & DWC_PERS_KEY_ID_MASK) return;
#endif

    // �ȉ��Q�[���ł̃f�[�^�Z�[�u�̏ꍇ
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "Saved data to persistent server.\n");

    if ((type == pd_public_ro) || (type == pd_public_rw)){
        isPublic = TRUE;
    }
    else {
        isPublic = FALSE;
    }
        
    if (success){
        if (stpFriendCnt->saveCallback){
            // �Q�[������o�^���ꂽ�R�[���o�b�N���Ăяo��
            stpFriendCnt->saveCallback(TRUE, isPublic, instance);
        }
    }
    else {
        // �G���[����
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Failed to save persistent data.\n");

#if 0
        // �F�B�Ǘ��������f�֐����Ăяo��
        DWCi_StopFriendProcess(DWC_ERROR_NETWORK,
                               DWC_ECODE_SEQ_FRIEND+DWC_ECODE_GS_PERS+DWC_ECODE_TYPE_STATS_SAVE);
#endif

        // �Q�[������o�^���ꂽ�R�[���o�b�N���Ăяo��
        if (stpFriendCnt->saveCallback){
            stpFriendCnt->saveCallback(FALSE, isPublic, instance);
        }
    }
}


/*---------------------------------------------------------------------------*
  Persistent�R�[���o�b�N���x�������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�̔񓯊��֐��Ăяo�����ɃR�[���o�b�N���x����1���₷
 *---------------------------------------------------------------------------*/
static void DWCi_AddPersCallbackLevel(void)
{

    if (stpFriendCnt->persCallbackLevel == 0xffffffff){
        DWCi_StopFriendProcess(DWC_ERROR_FATAL,
                               DWC_ECODE_SEQ_FRIEND+DWC_ECODE_TYPE_UNEXPECTED);
        return;
    }
    stpFriendCnt->persCallbackLevel++;
}


/*---------------------------------------------------------------------------*
  Persistent�R�[���o�b�N���x�������֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FPersistent�̔񓯊��֐������R�[���o�b�N�Ăяo������
          �R�[���o�b�N���x����1���炷
 *---------------------------------------------------------------------------*/
static void DWCi_SubPersCallbackLevel(void)
{

    if (stpFriendCnt->persCallbackLevel == 0){
        DWCi_StopFriendProcess(DWC_ERROR_FATAL,
                               DWC_ECODE_SEQ_FRIEND+DWC_ECODE_TYPE_UNEXPECTED);
        return;
    }
    stpFriendCnt->persCallbackLevel--;
}


/*---------------------------------------------------------------------------*
  Persistent�R�[���o�b�N���x���擾�֐�
  �����@�F�Ȃ�
  �߂�l�FPersistent�R�[���o�b�N���x��
  �p�r�@�FPersistent�̃R�[���o�b�N�Ăяo���҂��������擾����
 *---------------------------------------------------------------------------*/
static u32 DWCi_GetPersCallbackLevel(void)
{

    return stpFriendCnt->persCallbackLevel;
}
