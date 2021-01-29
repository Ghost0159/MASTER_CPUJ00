#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "userdata.h"
#include "screen.h"

#include "dbs.h"

/** --------------------------------------------------------------------
  defines
  ----------------------------------------------------------------------*/
#define GAME_NAME        "dwctest" // ���̃T���v�����g�p����Q�[����
#define GAME_SECRET_KEY  "d4q9GZ"  // ���̃T���v�����g�p����V�[�N���b�g�L�[
#define GAME_PRODUCTID   10824      // ���̃T���v�����g�p����v���_�N�gID

#define CB_CONNECT_SERVER 0        // �T�[�o�N���C�A���g�}�b�`���C�N�̃T�[�oDS
#define CB_CONNECT_CLIENT 1        // �T�[�o�N���C�A���g�}�b�`���C�N�̃N���C�A���gDS

/** --------------------------------------------------------------------
  globals
  ----------------------------------------------------------------------*/
KeyControl		g_KeyCtrl;

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
static DWCFriendsMatchControl	s_FMCtrl;
static DWCInetControl			s_ConnCtrl;
static int  s_state;
static BOOL s_logined;
static BOOL s_keepmatch;
static BOOL s_matched;
static BOOL s_canceld;
static BOOL s_closed;
static int  s_matched_frame;
static int  s_profileID = 0;
static u8   s_entry = 0;

const int	SIZE_SEND_BUFFER = 512;
const int	SIZE_RECV_BUFFER = 4 * 1024;

static u8	s_send_buf[ SIZE_SEND_BUFFER ];
static u8	s_recv_buf[ DWC_MAX_CONNECTIONS ][ SIZE_RECV_BUFFER ];

/** --------------------------------------------------------------------
  static functions
  ----------------------------------------------------------------------*/
static int update_init( void );
static int update_connect( void );
static int update_login( void );
static int update_online( void );
static int update_server( void );
static int update_client( void );
static int update_matching( void );
static int update_matched( void );
static int update_close( void );
static int update_disconnect( void );
static int update_save( void );
static int update_error( void );

static BOOL InputNumber( u8* number );
static void DispMenuMsg( const u8 figure, const u8 number );

static void cb_login(DWCError error, int profileID, void* param);
static void cb_updateServers(DWCError error, BOOL isChanged, void* param);
static void cb_connect(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void* param);
static void cb_close(DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param);

static void DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l );

/*---------------------------------------------------------------------------*
  V�u�����N���荞�݊֐�
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    //---- upload pseudo screen to VRAM
    DC_FlushRange(gScreen, sizeof(gScreen));
    GX_LoadBG0Scr(gScreen, 0, sizeof(gScreen));

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/** --------------------------------------------------------------------
  ����������
  ----------------------------------------------------------------------*/
static void
initFunctions( void )
{
	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    CARD_Init();
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    PXI_Init();
    GX_Init();
    FS_Init( MI_DMA_MAX_NUM ); 

    GX_DispOff();
    GXS_DispOff();

    // V�u�����N���荞�݋���
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);
	GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    Heap_Init();
    Heap_SetDebug( TRUE );

    // �o�͏�����
    InitScreen();
    dbs_DemoInit();
    DTDB_SetPrintOverride( TRUE );

    // �L�[���͏�����
    InitKeyData( &g_KeyCtrl );

    // �\���J�n
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

}

/** --------------------------------------------------------------------
  1�t���[��1��Ă΂��֐�
  ----------------------------------------------------------------------*/
static void
update( void )
{
    int errcode;
    DWCError err;
    DWCErrorType errtype;

    switch ( s_state )
    {
    case state_init:			s_state = update_init();			break;
    case state_connect:			s_state = update_connect();			break;
    case state_login:			s_state = update_login();			break;
    case state_online:			s_state = update_online();			break;
    case state_server:          s_state = update_server();          break;
    case state_client:          s_state = update_client();          break;
    case state_matching:		s_state = update_matching();		break;
    case state_matched:			s_state = update_matched();			break;
    case state_close:			s_state = update_close();			break;
    case state_disconnect:		s_state = update_disconnect();		break;
    case state_save:            s_state = update_save();            break;
    case state_error:			s_state = update_error();			break;
    }

#if 1
    err = DWC_GetLastErrorEx(&errcode, &errtype);

    if ( err != DWC_ERROR_NONE && s_state != state_error )
    {
        // �G���[�����������ꍇ�A�����ł͏�Ԃ��G���[�����҂��ɂ��邾��
        OS_TPrintf( "\n*******************************\n" );
        OS_TPrintf( "ERROR : %d : %d\n", err, errcode );
        OS_TPrintf( "Push START BUTTON to continue\n" );
        OS_TPrintf( "*******************************\n\n" );
        s_state = state_error;
    }

#else
    err = DWC_GetLastErrorEx(&errcode, &errtype);
    if ( err != DWC_ERROR_NONE )
    {
        OS_TPrintf("ERROR: %d:%d\n", err, errcode);
        DWC_ClearError();
        
        if ( errtype == DWC_ETYPE_DISCONNECT )
        {
            // �ʐM�ؒf����FriendsMatch�����̏I���ƃC���^�[�l�b�g�ڑ���
            // Cleanup���s�Ȃ�
            DWC_ShutdownFriendsMatch();
            s_state = state_disconnect;
        }
        else if ( errtype == DWC_ETYPE_FATAL )
        {
            // Fatal Error�Ȃ̂ŏI��
            OS_Panic("++FATAL ERROR\n");
        }
        else if ( errtype == DWC_ETYPE_LIGHT )
        {
            // �y���G���[�͉��������ɃI�����C����Ԃɖ߂�
            DTUD_DispFriendList();
            s_state = state_online;
        }
        else {
            // FriendsMatch�����̂ݏI��������A�Ă�FrientsMatch���������s�Ȃ�
            pushResult( err );
            DWC_ShutdownFriendsMatch();
            s_state = state_connect;
        }
    }
#endif
}

/** --------------------------------------------------------------------
  �C���^�[�l�b�g�ɐڑ�
  ----------------------------------------------------------------------*/
static int
update_init( void )
{
    // DS�{�̂�DS�J�[�h�̃��[�U�f�[�^����v���邩
    if ( DWC_CheckValidConsole( DTUD_GetUserData() ) )
    {
        DispMessage( "STATE : CONNECTING", NULL, NULL, NULL, NULL, NULL);
        
        DWC_InitInet( &s_ConnCtrl );
        
        DWC_SetAuthServer( DWC_CONNECTINET_AUTH_RELEASE );
        
        DWC_ConnectInetAsync();
        
        return state_connect;
    }
    else
    {
        // ��v���Ă��Ȃ��ꍇ�̓C���^�[�l�b�g�ɐڑ��ł��Ȃ�
        OS_TPrintf( "UserData is not Valid Console.\n" );
        return state_init;
    }

    return state_init;
}

/** --------------------------------------------------------------------
  �ڑ���
  ----------------------------------------------------------------------*/
static int
update_connect( void )
{
    DWC_ProcessInet();

    if ( DWC_CheckInet() )
    {
        int status;

        status = DWC_GetLastError(NULL);

        if ( status == DWC_ERROR_NONE )
        {

            OS_TPrintf("Connected!..start FriendsMatch.\n");
            
            s_logined = FALSE;
            
            DWC_InitFriendsMatch(&s_FMCtrl, DTUD_GetUserData(),
                                 GAME_PRODUCTID, GAME_NAME, GAME_SECRET_KEY,
                                 0, 0,
                                 DTUD_GetFriendList(), FRIEND_LIST_LEN );
            
            // �F�ؗp�֐����g���ă��O�C��
            DWC_LoginAsync(L"�Ȃ܂�", NULL, cb_login, NULL);
            
            return state_login;
            
        }
        else
        {
            OS_TPrintf("Error\n");
            return state_disconnect;
        }
    }

    return state_connect;
}

/** --------------------------------------------------------------------
  ���O�C����
  ----------------------------------------------------------------------*/
int
update_login( void )
{
    u8 i;
    
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( s_logined )
    {
        // ingamesn�`�F�b�N�̌��ʂ��擾����
	    if( DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID )
        {
            // �s�K�؂Ȗ��O�Ɣ��f���ꂽ�ꍇ�͓��ʂȏ������K�v
		    OS_TPrintf("BAD ingamesn is detected by NAS.\n");
        }
        
        OS_TPrintf("================================\n");

        s_keepmatch = FALSE;
        s_matched   = FALSE;
        s_canceld   = FALSE;
        s_closed    = FALSE;

        DTUD_DispFriendList();

        // ���̒i�K�ŃR�l�N�V�����N���[�Y�R�[���o�b�N��ݒ�
        DWC_SetConnectionClosedCallback(cb_close, NULL);

        // ��M�o�b�t�@���ݒ�
        for ( i = 0; i < DWC_MAX_CONNECTIONS; ++i )
        {
            DWC_SetRecvBuffer( i, &s_recv_buf[i], SIZE_RECV_BUFFER );
        }

        // ���O�C������
        DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data");

        return state_online;
    }

    return state_login;
}

/** --------------------------------------------------------------------
  �I�����C����
  ----------------------------------------------------------------------*/
int
update_online( void )
{
    BOOL changed = FALSE;
    
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŒʐM��ؒf���A������Ԃɖ߂�
        DWC_ShutdownFriendsMatch();
        return state_disconnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���ŃN���C�A���g���[�h��
        s_matched = FALSE;
        DTUD_DispFriendList();
        return state_client;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_X )
    {
        // X�{�^���ŃT�[�o�[���[�h��
        s_matched = FALSE;
        DTUD_DispFriendList();
        return state_server;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_Y )
    {
        // Y�{�^���ŗF�B�̏���\������
        DTUD_DispFriendList();
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_START )
    {
        // �X�^�[�g�{�^���ŒʐM��ؒf���A������Ԃɖ߂�B
        DWC_ShutdownFriendsMatch();
        return state_disconnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_L )
    {
        DispMessage( "SAVE DATA?", "save", "cancel", NULL, NULL, NULL );
        return state_save;
    }

    return state_online;
}

/** --------------------------------------------------------------------
  �T�[�oDS�̗����グ
  ----------------------------------------------------------------------*/
static int
update_server( void )
{
    static u8 s_entry;
    
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    DispMessage( "SETUP GAME SERVER", "set", "cancel", NULL, NULL, NULL );
    PrintString( 1, 14, 0xf, "MAX MATCHING NUMBER:");
    PrintString( 1, 15, 0xf, "Please input 2 or more.");
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        // B�{�^���ŒʐM���L�����Z������
        return state_online;
    }
    else if( InputNumber( &s_entry ) )
    {
        // �ő�ڑ��l���͕K��2�l�ȏ�łȂ���΂Ȃ�Ȃ�
        if( s_entry > 1 ){
            DispMessage( "STATE : MATCHING", NULL, "cancel", NULL, NULL, NULL );
            
            DWC_SetupGameServer( s_entry,
                                 cb_connect, (void *)CB_CONNECT_SERVER,
                                 NULL, NULL );
            
            s_entry = 0;
            
            return state_matching;
        }
    }
    return state_server;
}

/** --------------------------------------------------------------------
  �N���C�A���gDS�̗����グ
  ----------------------------------------------------------------------*/
static int
update_client( void )
{
    static u8 s_entry;
    u8   maxEntry, numEntry;
    DWCFriendData* s_FriendList;

    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    DispMessage( "CONNECT TO SERVER", "set", "cancel", NULL, NULL, NULL );
    PrintString( 1, 14, 0xf, "CONNECT FRIEND INDEX:");

    // �����}�b�`���C�N�ĊJ�҂��̎���B�{�^���ŉ�������
    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŒʐM���L�����Z������
        DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        return state_online;
    }
    else if( InputNumber( &s_entry ) )
    {
        s_FriendList = DTUD_GetFriendList();

        DWC_GetFriendStatusSC( &s_FriendList[s_entry], &maxEntry, &numEntry, NULL );
        if ( numEntry >= maxEntry )
        {
            // ���ɃT�[�oDS������I�[�o�[
            OS_TPrintf( "GameServer is full!\n" );
            OS_TPrintf( "Do over again.\n" );
        }
        else
        {
            DispMessage( "STATE : MATCHING", NULL, "cancel", NULL, NULL, NULL );
        
            DWC_ConnectToGameServerAsync( s_entry,
                                          cb_connect, (void *)CB_CONNECT_CLIENT,
                                          NULL, NULL );
            s_entry = 0;
            
            return state_matching;
        }
    }
    return state_client;
}
        
/** --------------------------------------------------------------------
  �}�b�`���C�N��
  ----------------------------------------------------------------------*/
static int
update_matching( void )
{
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���Ń}�b�`���C�N���L�����Z������            
        s_canceld = TRUE;
    }

    if ( s_matched )
    {
        DispMessage( "STATE : MATCHED", "send all", "shutdown", "my state", "friend state", NULL );

        s_matched_frame = 0;

        return state_matched;
    }
    else if ( s_canceld )
    {
        // �L�����Z�����ꂽ��I�����C����Ԃɖ߂�
            DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        OS_TPrintf("Canceled connection ... retry\n");

        DWC_CancelMatching();

        s_canceld = FALSE;

        DTUD_DispFriendList();

        return state_online;
    }

    return state_matching;
}

/** --------------------------------------------------------------------
  �}�b�`���C�N����
  ----------------------------------------------------------------------*/
static int
update_matched( void )
{
    s_matched_frame++;

    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    // �T�[�oDS���N���[�Y����state���ς�����ꍇ�͂����ɔ�����
    if ( s_state != state_matched )
    {
        DTUD_DispFriendList();
        
        return s_state;
    }
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���őS�ڑ��z�X�g��Reliable���M���s�Ȃ�
        DWC_SendReliableBitmap( DWC_GetAIDBitmap(), s_send_buf, SIZE_SEND_BUFFER );
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŃR�l�N�V�������N���[�Y����
        int result = DWC_CloseAllConnectionsHard();
        
        OS_TPrintf( "CLOSE! result %d.\n", result );
        
        DTUD_DispFriendList();
        DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        return state_online;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_X )
    {
        // X�{�^���Ŏ����̏���\������
        u8* pAidList;
        int num;
        int i;

        num = DWC_GetAIDList(&pAidList);
        for ( i = 0; i < num; i++ )
        {
            if ( pAidList[i] == DWC_GetMyAID() ) break;
        }
        OS_TPrintf( "[%d] aid:%d, num:%d\n", i, DWC_GetMyAID(), num );
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_Y )
    {
        // Y�{�^���ŗF�B�̏���\������
        DTUD_DispFriendList();
    }

    return state_matched;
}

/** --------------------------------------------------------------------
  �N���[�Y��
  ----------------------------------------------------------------------*/
static int
update_close( void )
{
    s_logined = FALSE;

    return state_login;
}

/** --------------------------------------------------------------------
  �ؒf���ꂽ�B
  ----------------------------------------------------------------------*/
static int
update_disconnect( void )
{
    if ( DWC_CleanupInetAsync() )
    {
        return state_init;
    }

    return state_disconnect;
}

/** --------------------------------------------------------------------
  �f�[�^���Z�[�u����B
  ----------------------------------------------------------------------*/
static int
update_save( void )
{
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        if( DWC_CheckDirtyFlag( DTUD_GetUserData() ) ){
            DTUD_SaveUserData();
            OS_TPrintf( "Saved User Data.\n" );
        }
        
        DTUD_SaveFriendList();
        OS_TPrintf( "Saved Friend Data.\n" );

        DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        return state_online;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        return state_online;
    }
    return state_save;
}

/** --------------------------------------------------------------------
  �G���[�����҂�
  ----------------------------------------------------------------------*/
static int update_error( void )
{
    int errcode;
    DWCError err;
    DWCErrorType errtype;
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_START )
    {
        // �X�^�[�g�{�^���ŃG���[����
        err = DWC_GetLastErrorEx(&errcode, &errtype);

        DWC_ClearError();
        
        if ( errtype == DWC_ETYPE_DISCONNECT )
        {
            // �ʐM�ؒf����FriendsMatch�����̏I���ƃC���^�[�l�b�g�ڑ���
            // Clearup���s�Ȃ�
            DWC_ShutdownFriendsMatch();
            return state_disconnect;
        }
        else if ( errtype == DWC_ETYPE_FATAL )
        {
            // Fatal Error�Ȃ̂ŏI��
            OS_Panic("++FATAL ERROR\n");
            return state_init;  // not reached;
        }
        else if ( errtype == DWC_ETYPE_LIGHT )
        {
            // �y���G���[�͉��������ɃI�����C����Ԃɖ߂�
            DTUD_DispFriendList();
            return state_online;
        }
        else {
            // FriendsMatch�����̂ݏI��������A�Ă�FrientsMatch���������s�Ȃ�
            DWC_ShutdownFriendsMatch();
            return state_connect;
        }
    }

    return state_error;
}

/** --------------------------------------------------------------------
  ----------------------------------------------------------------------*/
/** --------------------------------------------------------------------
  �R�[���o�b�N�֐�
  ----------------------------------------------------------------------*/
static void
cb_login(DWCError error, int profileID, void* param)
{
    (void)param;
    if (error == DWC_ERROR_NONE)
    {
        // �F�ؐ����A�v���t�@�C��ID�擾
        OS_TPrintf("Login succeeded. profileID = %u\n\n", profileID);

        s_profileID = profileID;  // �v���t�@�C��ID���L�^

        // stUserData���X�V����Ă��邩�ǂ������m�F�B
        if ( DTUD_CheckUpdateUserData() )
        {
            DTUD_SaveUserData();
        }

        // �F�B���X�g��GameSpy�T�[�oDS��̃o�f�B���X�g�̓�������
        DWC_UpdateServersAsync(NULL,
                               cb_updateServers, NULL,
                               NULL, NULL,
                               NULL, NULL);
    }
    else
    {
        OS_TPrintf("Error\n");
    }
}

static void
cb_updateServers(DWCError error, BOOL isChanged, void* param)
{
    (void)param;
    (void)isChanged;

    if ( error == DWC_ERROR_NONE )
    {
        // �F�B���X�g������������
        OS_TPrintf( "Update Servers succeeded.\n" );
        
        s_logined = TRUE;
    }
    else
    {
        OS_TPrintf("Error\n");
    }
}

static void
cb_connect(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index, void* param)
{

    if ( (int)param == CB_CONNECT_SERVER )
    {
        // �T�[�oDS
        OS_TPrintf( "connected callback : SERVER\n" );
    }
    else if ( (int)param == CB_CONNECT_CLIENT )
    {
        // �N���C�A���gDS
        OS_TPrintf( "connected callback : CLIENT\n" );
    }
    OS_TPrintf( "err:%d, cancel:%d, self:%d, srv:%d, idx:%d\n",
                error, cancel, self, isServer, index );

    if ( error == DWC_ERROR_NONE )
    {
        if ( !cancel )
        {
            // �ڑ�����
            s_matched = TRUE;
        }
        else if ( self || isServer )
        {
            // �������}�b�`���C�N���L�����Z�������A�������͎������N���C�A���g��
            // �T�[�oDS���}�b�`���C�N���L�����Z������
            s_canceld = TRUE;
        }
        // �V�K�ڑ��N���C�A���g���}�b�`���C�N���L�����Z�����Ă��������Ȃ�
    }
    else
    {
        OS_TPrintf("Error\n");
    }

    s_keepmatch = FALSE;
}

static void
cb_close(DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param)
{
    (void)param;

    OS_TPrintf( "Closed Callback err:%d, local:%d, srv:%d, aid:%d, idx:%d\n",
               error, isLocal, isServer, aid, index );

    if ( error == DWC_ERROR_NONE )
    {
        if ( !isLocal && isServer )
        {
            // �������N���C�A���g�̎��ɁA�T�[�oDS���R�l�N�V�������N���[�Y�����B
            // �T�[�oDS���N���[�Y�����̂Ŏ������N���[�Y����B
            s_closed = TRUE;
            DWC_CloseAllConnectionsHard();
        }
        
        if ( s_closed && DWC_GetNumConnectionHost() == 1 ){
            // ��̃N���[�Y�����őS�ẴR�l�N�V�������N���[�Y���ꂽ�ꍇ
            s_closed = FALSE;
            s_state  = state_online;
            DispMessage( "STATE : ONLINE", "connect to server", "shutdown", "setup game server", "friend state", "save firned data" );
        }
    }
    else
    {
        OS_TPrintf("Error\n");
    }
}

/** -------------------------------------------------------------------------
  ---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  Name:         InputNumber

  Description:  �F�B�o�^���̐��l���͂��s���B

  Arguments:    maxFigure - ���͂���ő�̌���

  Returns:      ���͏I���Ȃ�TRUE
                ���͒��Ȃ�FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL InputNumber( u8* number )
{
    static u8 figure = 0;
    u8 maxFigure = 2;
    u8 num;

    num = *number;

    DispMenuMsg( figure, num );
    
    if (g_KeyCtrl.trg & PAD_BUTTON_A)
    {
        figure = 0;
        return TRUE;
    }
    else if ( g_KeyCtrl.trg & PAD_KEY_UP )
    {
        // �\���L�[�㉺�őI�𒆂̐��l�̑���
        if ( (num/UtilPow(10, figure) )%10 == 9)
        {
            num -= UtilPow(10, figure)*9;
        }
        else
        {
            num += UtilPow(10, figure);
        }
        // ���͒����l�\��
        DispMenuMsg( figure, num );
    }
    else if ( g_KeyCtrl.trg & PAD_KEY_DOWN )
    {
        // �\���L�[�㉺�őI�𒆂̐��l�̑���
        if ( (num/UtilPow(10, figure) )%10 == 0)
        {
            num += UtilPow(10, figure)*9;
        }
        else
        {
            num -= UtilPow(10, figure);
        }
        // ���͒����l�\��
        DispMenuMsg( figure, num );
    }
    else if ( g_KeyCtrl.trg & PAD_KEY_LEFT )
    {
        // �\���L�[���E�őI�������̈ړ�
        if (figure < maxFigure-1) figure++;
        // ���͒����l�\��
        DispMenuMsg( figure, num );
    }
    else if ( g_KeyCtrl.trg & PAD_KEY_RIGHT )
    {
        // �\���L�[���E�őI�������̈ړ�
        if (figure > 0) figure--;
        // ���͒����l�\��
        DispMenuMsg( figure, num );
    }

    *number = num;
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DispMenuMsg

  Description:  �F�B�o�^���̐��l��\������B

  Arguments:    figure    - ���͒��̌���
                maxFigure - �ő包��

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
static void DispMenuMsg( const u8 figure, const u8 number )
{
    PrintString( 1, 17, 0xf, "Input Number:" );
    PrintString( 15, 17, 0xf, "%2.2u", number );

// �I�𒆂̐����������ׂ̉�����\������
    PrintString( (s16)(16-figure), 18, 0xf, "-" );
}

/*---------------------------------------------------------------------------*
  Name:         DispMessage

  Description:  �ėp�̃��b�Z�[�W��\������B

  Arguments:    main_msg - ���݂̃��[�h
                msg_a    - A�{�^���������ꂽ���̏���
                msg_b    - B�{�^���������ꂽ���̏���
                msg_x    - X�{�^���������ꂽ���̏���
                msg_y    - Y�{�^���������ꂽ���̏���
                msg_l    - L�{�^���������ꂽ���̏���

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
static void
DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l )
{
    s16 x = 1;
    s16 y = 5;

    ClearScreen();
    
    PrintString( 0, 0, 0xf, "================================" );
    PrintString( 2, 1, 0xf, "*** SC MATCHING SAMPLE ***" );
    PrintString( 0, 2, 0xf, "================================" );
    
    if ( main_msg == NULL ) return;

    PrintString( 0, y, 0xf, "--------------------------------" );
    y++;
    // ���C�����b�Z�[�W�\��
    while ( *main_msg != '\0' )
    {
        if ( *main_msg == '\n' )
        {
            x = 1;  y++;
        }
        else
        {
            PrintString( x, y, 0xf, "%c", *main_msg );
            x++;
        }
        main_msg++;
    }
    y++;
    PrintString( 0, y, 0xf, "--------------------------------" );

    y += 2;

    // A�{�^�������AB�{�^���AX�{�^���AY�{�^�������\��
    if ( msg_a != NULL )
    {
        PrintString( 1, y, 0xf, "A BUTTON:%s", msg_a );
    }
    y++;
    if ( msg_b != NULL )
    {
        PrintString( 1, y, 0xf, "B BUTTON:%s", msg_b );
    }
    y += 2;
    if ( msg_x != NULL )
    {
        PrintString( 1, y, 0xf, "X BUTTON:%s", msg_x );
    }
    y++;
    if ( msg_y != NULL )
    {
        PrintString( 1, y, 0xf, "Y BUTTON:%s", msg_y );
    }
    y += 2;
    if ( msg_l != NULL )
    {
        PrintString( 1, y, 0xf, "L BUTTON:%s", msg_l );
    }
}

/** --------------------------------------------------------------------
  ���t���[���Ăяo���A��ԕ\���֐�
  ----------------------------------------------------------------------*/
void
drawConsole( void )
{
    static int i = 0;
    char loop[] = "/|\\-";
    char state[16];
    int  matchType = DWC_GetLastMatchingType();

    if ( ( DWC_GetState() != DWC_STATE_CONNECTED ) &&
         ( DWC_GetState() != DWC_STATE_MATCHING ) )
    {
        strcpy( state, "**" );
    }
    else if ( matchType == DWC_MATCH_TYPE_SC_SV )
    {
        strcpy( state, "SV" );
    }
    else if ( matchType == DWC_MATCH_TYPE_SC_CL )
    {
        strcpy( state, "CL" );
        if ( DWC_GetState() == DWC_STATE_CONNECTED )
        {
            strcat( state, "-CONN" );
        }
        else if ( DWC_GetState() == DWC_STATE_MATCHING )
        {
            strcat( state, "-MATCH" );
        }
    }
    else
    {
        strcpy( state, "**" );
    }
    
    dbs_Print( 0, 0, "s:%s", state );
    dbs_Print( 13, 0, "n:%d", DWC_GetNumConnectionHost() );
    dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
    dbs_Print( 4, 1, "ss:%d", s_state );
    dbs_Print( 13, 1, "p:%d", s_profileID );

    dbs_Print( 30,0, "%c", loop[((i)>>2)&3] );
    i++;
}

/** --------------------------------------------------------------------
  �G���g���|�C���g
  ----------------------------------------------------------------------*/
void NitroMain ()
{
    initFunctions();

    // �f�o�b�O�\�����x���w��
    DWC_SetReportLevel((u32)( DWC_REPORTFLAG_ALL &
                              ~DWC_REPORTFLAG_QR2_REQ ));
    // DWC���C�u����������
    DWC_Init( s_Work );

    // ���[�U�f�[�^�ǂݍ���
    DTUD_Init();
    // �������m�ۊ֐��ݒ�
    DWC_SetMemFunc( DTDB_GetAllocFunc(), DTDB_GetFreeFunc() );

    while (1)
    {
        drawConsole();

        ReadKeyData();

        update();

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
        dbs_DemoLoad();
    }

    OS_Terminate();
}
