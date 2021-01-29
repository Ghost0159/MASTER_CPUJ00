#include <nitro.h>
#include <base/dwc_base_gamespy.h>

#include <auth/dwc_auth.h>
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


//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------
typedef void (*DWCRemoteAuthCallback)(const char* authToken, const char* partnerChallenge, void* param);

//----------------------------------------------------------------------------
// static variable
//----------------------------------------------------------------------------
// ���O�C������I�u�W�F�N�g�ւ̃|�C���^
static DWCLoginControl* stpLoginCnt = NULL;

static DWCRemoteAuthCallback stRemoteAuthCallback;
static void* pStRemoteAuthParam;

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void DWCi_CloseLogin(void);

static GPResult DWCi_HandleGPError     (GPResult result);
static void     DWCi_GPConnectCallback (GPConnection* pconnection, GPConnectResponseArg* arg, void* param);
static void     DWCi_RemoteAuthCallback(const char* authToken, const char* partnerChallenge, void* param);
static void     DWCi_GPConnect( const char* authToken,  const char* partnerChallenge, GPCallback callback, DWCLoginState nextstate );

static void DWCi_GPGetInfo( void );
static void DWCi_GPGetInfoCallback( GPConnection* pconnection, GPGetInfoResponseArg* arg, void* param );

static void DWCi_RemoteLogin       ( DWCRemoteAuthCallback callback, void* param );
static void DWCi_RemoteLoginProcess( void );


//----------------------------------------------------------------------------
// function - internal
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  ���O�C������\���̏������֐�
  �����@�Flogcnt     ���O�C������I�u�W�F�N�g�ւ̃|�C���^
          userdata   ���[�U�f�[�^�I�u�W�F�N�g�ւ̃|�C���^
          pGpObj     gp�R�l�N�V�����\���̃|�C���^�ւ̃|�C���^
          productID  GameSpy����^������v���_�N�gID
          gamecode   �Q�[������p�̃R�[�h�i�ʏ�C�j�V�����R�[�h�j
          playerName �Q�[�����X�N���[���l�[��
          callback   ���O�C�������ʒm�p�R�[���o�b�N�֐�
          param      �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F���O�C������\���̂�����������
 *---------------------------------------------------------------------------*/
void DWCi_LoginInit(DWCLoginControl* logcnt, DWCUserData* userdata, GPConnection* pGpObj, int productID, u32 gamecode, const u16* playerName, DWCLoginCallback callback, void* param)
{
    SDK_ASSERT(logcnt);

    DWC_Printf( DWC_REPORTFLAG_LOGIN, "Login Init\n" );

    stpLoginCnt = logcnt;

    MI_CpuClear8( stpLoginCnt, sizeof( DWCLoginControl ) );

    // ���O�C������\���̏�����
    stpLoginCnt->pGpObj     = pGpObj;
    stpLoginCnt->state      = DWC_LOGIN_STATE_INIT;
    stpLoginCnt->productID  = productID;
    stpLoginCnt->gamecode   = gamecode;
    stpLoginCnt->playerName = playerName;
    stpLoginCnt->callback   = callback;
    stpLoginCnt->param      = param;

    stpLoginCnt->userdata   = userdata;

    DWC_Printf( DWC_REPORTFLAG_LOGIN, "******************************************\n" );
    DWC_Printf( DWC_REPORTFLAG_LOGIN, "  pseudo    UserID   : %016llx\n", DWCi_Acc_GetUserId( &userdata->pseudo ) );
    DWC_Printf( DWC_REPORTFLAG_LOGIN, "  pseudo    PlayerID : %08x\n"   , DWCi_Acc_GetPlayerId( &userdata->pseudo ) );
    DWC_Printf( DWC_REPORTFLAG_LOGIN, "  authentic UserID   : %016llx\n", DWCi_Acc_GetUserId( &userdata->authentic ) );
    DWC_Printf( DWC_REPORTFLAG_LOGIN, "  authentic PlayerID : %08x\n"   , DWCi_Acc_GetPlayerId( &userdata->authentic ) );
    DWC_Printf( DWC_REPORTFLAG_LOGIN, "******************************************\n" );

}


/*---------------------------------------------------------------------------*
  Wi-Fi�R�l�N�V�������O�C���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�FWi-Fi�R�l�N�V�����ւ̃��O�C�����J�n����
 *---------------------------------------------------------------------------*/
//void DWCi_LoginAsync(DWCLoginCallback callback, void* param)
void DWCi_LoginAsync( void )
{

    DWC_Printf( DWC_REPORTFLAG_LOGIN, "Login Async\n" );

    // �{���̔F�؃T�[�o�Ăяo���B
    DWCi_RemoteLogin( DWCi_RemoteAuthCallback, NULL );

    stpLoginCnt->state = DWC_LOGIN_STATE_REMOTE_AUTH;
    stpLoginCnt->connectFlag = FALSE;
}


/*---------------------------------------------------------------------------*
  ���O�C�������X�V�֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F���Q�[���t���[���Ăяo���A���O�C���ʐM�������X�V����
 *---------------------------------------------------------------------------*/
void DWCi_LoginProcess(void)
{

    if (!stpLoginCnt || DWCi_IsError()) return;

    switch (stpLoginCnt->state){
    case DWC_LOGIN_STATE_REMOTE_AUTH:  // �����[�g�F�ؒ�
        DWCi_RemoteLoginProcess();
        break;

    case DWC_LOGIN_STATE_CONNECTING:  // GP�T�[�o�ڑ���
    case DWC_LOGIN_STATE_GPGETINFO:
    case DWC_LOGIN_STATE_GPSETINFO:
        if (stpLoginCnt->pGpObj && *stpLoginCnt->pGpObj)
        {
            gpProcess(stpLoginCnt->pGpObj);
        }
        if ( stpLoginCnt->connectFlag &&
             ( OS_TicksToMilliSeconds( OS_GetTick() - stpLoginCnt->connectTick ) > DWC_LOGIN_GPCONNECT_TIMEOUT ) )
        {
            // �^�C���A�E�g�����B
            DWCi_StopLogin( DWC_ERROR_NETWORK, DWC_ECODE_SEQ_LOGIN+DWC_ECODE_GS_GP+DWC_ECODE_TYPE_TIMEOUT );
            stpLoginCnt->connectFlag = FALSE;
        }
        break;

    case DWC_LOGIN_STATE_INIT:
    case DWC_LOGIN_STATE_CONNECTED:
    default:
        // ��L�ȊO�̏ꍇ�͉������Ȃ��Ŕ�����
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore invalid login state.\n");
        break;
    }
}


/*---------------------------------------------------------------------------*
  ���[�U�f�[�^�擾�֐�
  �����@�F�Ȃ�
  �߂�l�F���[�U�f�[�^�ւ̃|�C���^
  �p�r�@�F���[�U�f�[�^�ւ̃|�C���^���擾����
 *---------------------------------------------------------------------------*/
DWCAccUserData* DWCi_GetUserData(void)
{

    if (stpLoginCnt){
        return stpLoginCnt->userdata;
    }
    else {
        return NULL;
    }
}


/*---------------------------------------------------------------------------*
  ���O�C���G���[�I���֐�
  �����@�Ferror     DWC�G���[���
          errorCode �G���[�R�[�h
  �߂�l�F�Ȃ�
  �p�r�@�F���O�C���������ɃG���[�����������ꍇ�ɁA�����𒆒f����
 *---------------------------------------------------------------------------*/
void DWCi_StopLogin(DWCError error, int errorCode)
{

    if (!stpLoginCnt || (error == DWC_ERROR_NONE)) return;

    DWCi_SetError(error, errorCode);
    
    // ���O�C�������R�[���o�b�N�֐��Ăяo��
    if ( stpLoginCnt->callback != NULL )
    {
        stpLoginCnt->callback(error, 0, stpLoginCnt->param);
    }

    DWCi_CloseLogin();

    // ���̌�ADWC_ShutdownFriendsMatch()��GP�I�u�W�F�N�g���������K�v������
}


/*---------------------------------------------------------------------------*
  ���O�C������I�u�W�F�N�g����֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F���O�C������I�u�W�F�N�g�ւ̃|�C���^��NULL�N���A����
 *---------------------------------------------------------------------------*/
void DWCi_ShutdownLogin(void)
{
    // �F�ؒ����ǂ������m�F�B
    if ( stpLoginCnt->http != NULL )
    {
        DWC_Auth_Abort();
        DWC_Auth_Destroy();

        DWC_Free( DWC_ALLOCTYPE_AUTH, stpLoginCnt->http, 0 );
        stpLoginCnt->http = NULL;
    }

    stpLoginCnt = NULL;
}


//----------------------------------------------------------------------------
// function - static
//----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  ���O�C���I���֐�
  �����@�F�Ȃ�
  �߂�l�F�Ȃ�
  �p�r�@�F���O�C������I�u�W�F�N�g�����������ă��O�C���������I������
 *---------------------------------------------------------------------------*/
static void DWCi_CloseLogin(void)
{

    if (!stpLoginCnt) return;
    
    // ���O�C������I�u�W�F�N�g������
    stpLoginCnt->state = DWC_LOGIN_STATE_INIT;
    stpLoginCnt->connectFlag = FALSE;

    // password��authToken��partnerChallenge���APersistent Storage�ւ�
    // ���O�C���Ɏg��
}


/*---------------------------------------------------------------------------*
  GPResult�Ή��G���[�����֐�
  �����@�Fresult GP�̏������ʌ^
  �߂�l�FGP�̏������ʌ^�i���������̂܂ܕԂ��j
  �p�r�@�FGPResult�̒l�ɑΉ�����G���[�\���i�������͒�~�j���s���A
          �Ή�����DWC�G���[��n���ă��O�C���葱�����I������
 *---------------------------------------------------------------------------*/
static GPResult DWCi_HandleGPError(GPResult result)
{
    int errorCode;
    DWCError dwcError;

    if (result == GP_NO_ERROR) return GP_NO_ERROR;

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Login, GP error %d\n", result);

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
    errorCode += DWC_ECODE_SEQ_LOGIN+DWC_ECODE_GS_GP;

    DWCi_StopLogin(dwcError, errorCode);  // ���O�C���葱�����G���[�I��

    return result;
}


/*---------------------------------------------------------------------------*
  GP�R�l�N�g�R�[���o�b�N�֐�
  �����@�Fpconnection GP�R�l�N�V�����I�u�W�F�N�g�ւ̃|�C���^
          arg         GP�R�l�N�V���������\���̂ւ̃|�C���^
          param       �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�FGP�R�l�N�g�R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_GPConnectCallback(GPConnection* pconnection, GPConnectResponseArg* arg, void* param)
{
#pragma unused( param )
    GPResult gpResult;

    DWC_Printf(DWC_REPORTFLAG_LOGIN, "Finished connecting to GP server, result = %d\n", arg->result);
    // �R�[���o�b�N���Ăяo���ꂽ��A�I���B
    stpLoginCnt->connectFlag = FALSE;

    if (arg->result == GP_NO_ERROR)
    {
        // [nakata] GPConnectResponseArg��ޔ�(gpGetInfo��Ɏg�p����)
        stpLoginCnt->gpconnectresponsearg = *arg;
        if ( stpLoginCnt->state == DWC_LOGIN_STATE_CONNECTING )
        {
            // GP�X�e�[�^�X���I�����C����ԂɃZ�b�g����B
            //gpResult = DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);

            //if (DWCi_HandleGPError(gpResult)) return;

            if ( stpLoginCnt->userdata->gs_profile_id == arg->profile )
            {
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "  gs profile id is valid.\n" );

                stpLoginCnt->state = DWC_LOGIN_STATE_CONNECTED;

                // GP�X�e�[�^�X���I�����C����ԂɃZ�b�g����B
                gpResult = DWCi_SetGPStatus(DWC_STATUS_ONLINE, "", NULL);
                if (DWCi_HandleGPError(gpResult)) return;

                // �ڑ������B���O�C�������R�[���o�b�N�֐��Ăяo��
                stpLoginCnt->callback(DWC_ERROR_NONE, arg->profile, stpLoginCnt->param);

                // gt2�\�P�b�g�쐬
                if (!DWCi_GT2Startup()){
                    // ����������QR2�N��
                    if (DWCi_QR2Startup(arg->profile)) return;
                }
                else return;
                // �e�֐����ŃG���[�����������ꍇ�́A�����Ń��O�C���G���[�I��
                // �������s���Ă���
            }
            else
            {
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "  gs profile id is invalid.\n" );

                DWCi_StopLogin( DWC_ERROR_NETWORK, DWC_ECODE_SEQ_LOGIN );
            }
        }
        else if ( stpLoginCnt->state == DWC_LOGIN_STATE_GPGETINFO )
        {
            // GPGetInfo��lastname�`�F�b�N�����邽�߂̃V�[�P���X
            gpResult = gpGetInfo( pconnection, arg->profile, GP_DONT_CHECK_CACHE, GP_NON_BLOCKING, (GPCallback)DWCi_GPGetInfoCallback, NULL );
            if (DWCi_HandleGPError(gpResult)) return;
        }
    }
    else
    {
        // �ڑ��Ɏ��s�����ꍇ
        (void)DWCi_HandleGPError(arg->result);
    }
}

/*---------------------------------------------------------------------------*
  �����[�g�F�؊����R�[���o�b�N�֐�
  �����@�FauthToken        �擾�����F�ؗp�g�[�N���ւ̃|�C���^
          partnerchallenge �擾�����`�������W�ւ̃|�C���^
          param            �R�[���o�b�N�p�p�����[�^
  �߂�l�F�Ȃ�
  �p�r�@�F�����[�g�F�؊����R�[���o�b�N
 *---------------------------------------------------------------------------*/
static void DWCi_RemoteAuthCallback(const char* authToken, const char* partnerChallenge, void* param)
{
#pragma unused( param )

    DWCi_GPConnect( authToken, partnerChallenge, (GPCallback)DWCi_GPConnectCallback, DWC_LOGIN_STATE_CONNECTING );
}

static void DWCi_GPConnect( const char* authToken,  const char* partnerChallenge,
                            GPCallback callback, DWCLoginState next_state )
{
    GPResult gpResult;

    DWC_Printf(DWC_REPORTFLAG_LOGIN, "Succeeded to remote authentication.\n");

    strcpy( stpLoginCnt->authToken, authToken );
    strcpy( stpLoginCnt->partnerChallenge, partnerChallenge );

    stpLoginCnt->connectTick = OS_GetTick();
    stpLoginCnt->connectFlag = TRUE;

    gpResult = gpConnectPreAuthenticated(stpLoginCnt->pGpObj, stpLoginCnt->authToken,
                                         stpLoginCnt->partnerChallenge, GP_FIREWALL,
                                         GP_NON_BLOCKING,
                                         callback,
                                         NULL);
    if (DWCi_HandleGPError(gpResult))
    {
        return;
    }

    stpLoginCnt->state = next_state;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static void DWCi_RemoteLogin( DWCRemoteAuthCallback callback,
                              void*                 param )
{
    DWCAuthParam auth;
    void* http;

    DWC_Printf( DWC_REPORTFLAG_LOGIN, "Start Remote Auth\n" );

    MI_CpuClear8( &auth, sizeof(auth) );
    
    stRemoteAuthCallback = callback;
    pStRemoteAuthParam   = param;

    // �F�ؗp�̃f�[�^�쐬
    if ( DWCi_Acc_IsAuthentic( stpLoginCnt->userdata ) )
    {
        DWC_Printf( DWC_REPORTFLAG_LOGIN, "  Hmm.. you already have authentic login id.\n" );

        // �F�؍ς݂Ȃ̂ŁA�F�؍ς݂�userid���g�p����B
        DWCi_Acc_LoginIdToUserName( &stpLoginCnt->userdata->authentic, stpLoginCnt->userdata->gamecode, stpLoginCnt->username );
    }
    else
    {
        DWC_Printf( DWC_REPORTFLAG_LOGIN, "  Hmm.. you need to create authentic login id.\n" );

        // ���x�ڂ̎��s�����m�F����B
        if ( !DWCi_Acc_IsValidLoginId( &stpLoginCnt->tempLoginId ) )
        {
            DWC_Printf( DWC_REPORTFLAG_LOGIN, "    Hmm.. you are the first time to get authentic login id." );
            // �ŏ��̎��s�̏ꍇ�́A��ID���R�s�[���Ďg���B
            if ( DWCi_Acc_CheckConsoleUserId( &stpLoginCnt->userdata->pseudo ) )
            {
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "- copy temp loginid from pseudo login id\n" );
                stpLoginCnt->tempLoginId = stpLoginCnt->userdata->pseudo;
            }
            else
            {
                // ��ID��������̂ƈႤ�z�X�g�Ȃ̂ŁB
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "- create temp loginid from console user id\n" );
                DWCi_Acc_CreateTempLoginId( &stpLoginCnt->tempLoginId );
            }
        }
        else
        {
            MATHRandContext32   randcontext;

            DWC_Printf( DWC_REPORTFLAG_LOGIN, "    Hmm.. you are NOT the first times to get authentic login id.\n" );
            // ��x�ڈȍ~�́A�v���C���[ID�݂̂�u��������B
            // [todo] ������. -> ok
            //DWCi_Acc_SetPlayerId( &stpLoginCnt->tempLoginId, DWCi_Acc_GetPlayerId( &stpLoginCnt->tempLoginId ) + 1 );

            MATH_InitRand32( &randcontext, OS_GetTick() );
            DWCi_Acc_SetPlayerId( &stpLoginCnt->tempLoginId, MATH_Rand32( &randcontext, 0 ) );
        }

        DWCi_Acc_LoginIdToUserName( &stpLoginCnt->tempLoginId, stpLoginCnt->gamecode, stpLoginCnt->username );
    }

    // �Q�[�����X�N���[���l�[����F�ؗp�f�[�^�ɃR�s�[����
	MI_CpuCopy8(stpLoginCnt->playerName, auth.ingamesn, DWCi_WStrLen(stpLoginCnt->playerName)*2+2);

    DWC_Printf( DWC_REPORTFLAG_LOGIN, "  ingamesn : %s\n", auth.ingamesn );

    strcpy( auth.gsbrcd, &(stpLoginCnt->username[DWC_ACC_USERNAME_GSBRCD_OFFSET]) );
    DWC_Printf( DWC_REPORTFLAG_LOGIN, "  gsbrcd   : %s.\n", auth.gsbrcd );

    auth.alloc = (DWCAuthAlloc)DWC_Alloc;
    auth.free  = (DWCAuthFree)DWC_Free;

    http = DWC_AllocEx( DWC_ALLOCTYPE_AUTH, sizeof( DWCHttp ), 4 );
    SDK_ASSERTMSG( http, "DWC_LOGIN: can't alloc memory %s :%d\n", __FILE__, __LINE__ );

    // �J���p�Ɋo���Ă���
    stpLoginCnt->http   = http;
    stpLoginCnt->startTick = OS_GetTick();

    // �F�؏����̊J�n�B
    DWC_Auth_Create( &auth, http );
}

static void DWCi_RemoteLoginProcess( void )
{
    if ( DWC_Auth_GetError() == DWCAUTH_E_FINISH )
    {
        DWCAuthResult result;
        DWC_Printf(DWC_REPORTFLAG_LOGIN, " *** Auth Done\n");
        DWC_Auth_GetResult( &result );

        strcpy( stpLoginCnt->authToken , result.token );
        strcpy( stpLoginCnt->partnerChallenge , result.challenge );

        DWC_Auth_Destroy();

        DWC_Free( DWC_ALLOCTYPE_AUTH, stpLoginCnt->http, 0 );
        stpLoginCnt->http = NULL;

        // �F�؃p�P�b�g�擾�i�R�[���o�b�N���Ăяo���j
        // �{���Ȃ�A�v���C���[ID�Ȃǂ̏������K�v�B
        if ( DWCi_Acc_IsAuthentic( stpLoginCnt->userdata ) )
        {
            stRemoteAuthCallback( stpLoginCnt->authToken, stpLoginCnt->partnerChallenge, pStRemoteAuthParam );
        }
        else
        {
            // ���߂Ă̐ڑ��Ȃ̂ŁALoginID���擾����B
            DWCi_GPConnect( stpLoginCnt->authToken, stpLoginCnt->partnerChallenge, (GPCallback)DWCi_GPConnectCallback, DWC_LOGIN_STATE_GPGETINFO );
        }
    }
    else if ( DWC_Auth_GetError() != DWCAUTH_E_NOERR )
    {
        OSTick nowTick;

        DWC_Printf(DWC_REPORTFLAG_LOGIN, " *** Auth Error\n");
        nowTick = OS_GetTick();
        if ( OS_TicksToMilliSeconds( nowTick - stpLoginCnt->startTick ) > DWC_LOGIN_AUTH_TIMEOUT )
        {
            DWCAuthResult result;
            DWC_Auth_GetResult( &result );

            // �G���[�����B
            DWC_Auth_Destroy();
            DWC_Free( DWC_ALLOCTYPE_AUTH, stpLoginCnt->http, 0 );
            stpLoginCnt->http = NULL;

            DWCi_StopLogin( DWC_ERROR_AUTH_ANY, result.returncode );

            return;
            // NOT REACH HERE
        }
        else
        {
            // �^�C���A�E�g�ȉ��Ȃ̂ŁA�����p�����[�^�Ń��g���C�B
            DWCAuthParam auth;

            DWC_Printf( DWC_REPORTFLAG_LOGIN, "  **** AUTH FAILED ... retry AUTH_ERROR:%d.\n", DWC_Auth_GetError() );

            // �F�؏����̂�蒼���B
            DWC_Auth_Destroy();
            
            MI_CpuClear8( &auth, sizeof(auth) );

            // �Q�[�����X�N���[���l�[����F�ؗp�f�[�^�ɃR�s�[����
            MI_CpuCopy8(stpLoginCnt->playerName, auth.ingamesn, DWCi_WStrLen(stpLoginCnt->playerName)*2+2);

            DWC_Printf( DWC_REPORTFLAG_LOGIN, "  ingamesn : %s\n", auth.ingamesn );
            
            strcpy( auth.gsbrcd, &(stpLoginCnt->username[DWC_ACC_USERNAME_GSBRCD_OFFSET]) );
            auth.alloc = (DWCAuthAlloc)DWC_Alloc;
            auth.free  = (DWCAuthFree)DWC_Free;

            DWC_Printf( DWC_REPORTFLAG_LOGIN, "  gsbrcd   : %s.\n", auth.gsbrcd );

            DWC_Auth_Create( &auth, stpLoginCnt->http );
        }
    }
}

static void DWCi_GPGetInfoCallback( GPConnection* pconnection, GPGetInfoResponseArg* arg, void* param )
{
#pragma unused( param )

    if ( arg->result == GP_NO_ERROR )
    {
        if ( stpLoginCnt->state == DWC_LOGIN_STATE_GPGETINFO )
        {
            if ( arg->lastname[0] == 0 )
            {
                GPResult gpResult;
                char pseudo_name[DWC_ACC_USERNAME_STRING_BUFSIZE];
                // profileID���͂��߂Ďg����̂ŁAlastname���Z�b�g����B
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "    login id is authenticated. set lastname field.\n" );
                DWCi_Acc_LoginIdToUserName( &stpLoginCnt->userdata->pseudo, stpLoginCnt->gamecode, pseudo_name );
                gpResult = gpSetInfos( pconnection, GP_LASTNAME, pseudo_name );
                if (DWCi_HandleGPError(gpResult)) return;

                stpLoginCnt->state = DWC_LOGIN_STATE_GPSETINFO;

                // �{���ɃZ�b�g����Ă��邩�ǂ������擾���Ċm�F����B
                gpResult = gpGetInfo( pconnection, arg->profile, GP_DONT_CHECK_CACHE, GP_NON_BLOCKING, (GPCallback)DWCi_GPGetInfoCallback, NULL );
                if (DWCi_HandleGPError(gpResult)) return;

                DWC_Printf( DWC_REPORTFLAG_LOGIN, "    call gpSetInfos\n" );
            }
            else
            {
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "    this login id is used by anybody.... retry.\n" );

                // ��U�ؒf����B
                gpDisconnect( pconnection );

                // PlayerID��ς��Ẵ��g���C�B
                DWCi_RemoteLogin( DWCi_RemoteAuthCallback, NULL );
                stpLoginCnt->state = DWC_LOGIN_STATE_REMOTE_AUTH;
            }
        }
        else if ( stpLoginCnt->state == DWC_LOGIN_STATE_GPSETINFO )
        {
            // �Z�b�g������̃x���t�@�C
            char pseudo_name[DWC_ACC_USERNAME_STRING_BUFSIZE];
            DWCi_Acc_LoginIdToUserName( &stpLoginCnt->userdata->pseudo, stpLoginCnt->gamecode, pseudo_name );

            if ( strcmp( arg->lastname, pseudo_name ) == 0 )
            {
                char username[DWC_ACC_USERNAME_STRING_BUFSIZE];
                DWCi_Acc_LoginIdToUserName( &stpLoginCnt->tempLoginId, stpLoginCnt->gamecode, username );
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "    Account is created : %s(%s) - %d.\n", username, pseudo_name, arg->profile );

                DWCi_Acc_SetLoginIdToUserData( stpLoginCnt->userdata, &stpLoginCnt->tempLoginId, arg->profile );

                // [nakata] �ؒf/�Đڑ��͖��ʂȂ̂ł�߂Ă݂�
                //gpDisconnect( pconnection ); // �ڑ���Ԃ���������B

                // �{�Ԃ̐ڑ��B
                //stRemoteAuthCallback( stpLoginCnt->authToken, stpLoginCnt->partnerChallenge, pStRemoteAuthParam );
                stpLoginCnt->state = DWC_LOGIN_STATE_CONNECTING;
                DWCi_GPConnectCallback(pconnection, &stpLoginCnt->gpconnectresponsearg, NULL);
            }
            else
            {
                // [todo] retry
                GPResult gpResult;
                DWC_Printf( DWC_REPORTFLAG_LOGIN, "    Login but gpSetInfo failed... %s : %d retry gpGetInfo.\n", arg->lastname, arg->profile );

                gpResult = gpGetInfo( pconnection, arg->profile, GP_DONT_CHECK_CACHE, GP_NON_BLOCKING, (GPCallback)DWCi_GPGetInfoCallback, NULL );
                if (DWCi_HandleGPError(gpResult)) return;
            }
        }
    }
    else
    {
        DWC_Printf( DWC_REPORTFLAG_LOGIN, " ERROR: gpGetInfo. why??? : %d\n", arg->result );
    }
}

BOOL
DWCi_CheckLogin( void )
{
    if ( stpLoginCnt != NULL && stpLoginCnt->state == DWC_LOGIN_STATE_CONNECTED )
    {
        return TRUE;
    }

    return FALSE;
}

BOOL
DWCi_GetAuthInfo( char** authToken, char** partnerChallenge )
{
    if ( !DWCi_CheckLogin() ) return FALSE;
    
    *authToken        = stpLoginCnt->authToken;
    *partnerChallenge = stpLoginCnt->partnerChallenge;

    return TRUE;
}
