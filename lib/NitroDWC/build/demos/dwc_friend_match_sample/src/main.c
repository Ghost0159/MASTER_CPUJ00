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
#define	MAX_PLAYERS		 4	       // �������܂߂��ڑ��l��

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
static int  s_state  = state_init;
static BOOL s_logined;
static BOOL s_matched;
static BOOL s_canceled;

static int  s_profileID = 0;

const int	SIZE_SEND_BUFFER = 512;
const int	SIZE_RECV_BUFFER = 4 * 1024;

static u8	s_send_buf[ SIZE_SEND_BUFFER ];
static u8	s_recv_buf[ MAX_PLAYERS-1 ][ SIZE_RECV_BUFFER ];

/** --------------------------------------------------------------------
  static functions
  ----------------------------------------------------------------------*/
static int update_init( void );
static int update_connect( void );
static int update_login( void );
static int update_online( void );
static int update_matching( void );
static int update_matched( void );
static int update_disconnect( void );
static int update_save( void );
static int update_error( void );

static void cb_login( DWCError error, int profileID, void* param );
static void cb_updateServers(DWCError error, BOOL isChanged, void* param);
static void cb_connect( DWCError error, BOOL cancel, void* param );
static void cb_send( int size, u8 aid );
static void cb_recv( u8 aid, u8* buffer, int size );
static void cb_close( DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param );

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
  
  ----------------------------------------------------------------------*/

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
    case state_matching:		s_state = update_matching();		break;
    case state_matched:			s_state = update_matched();			break;
    case state_disconnect:		s_state = update_disconnect();		break;
    case state_save:            s_state = update_save();            break;
    case state_error:			s_state = update_error();			break;
    }

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
                                 DTUD_GetFriendList(), FRIEND_LIST_LEN);

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
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( s_logined )
    {
        // ���O�C������
        DispMessage( "STATE : ONLINE", "friend matching", "shutdown", NULL, "friend state", "save firned data");

        // ingamesn�`�F�b�N�̌��ʂ��擾����
	    if( DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID )
        {
            // �s�K�؂Ȗ��O�Ɣ��f���ꂽ�ꍇ�͓��ʂȏ������K�v
		    OS_TPrintf("BAD ingamesn is detected by NAS.\n");
        }

        s_matched  = FALSE;
        s_canceled = FALSE;

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
    
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���ŗF�B�w��s�A�}�b�`���C�N�J�n
        DWC_ConnectToFriendsAsync(NULL,
                                  0,
                                  MAX_PLAYERS,
                                  TRUE,//FALSE,
                                  cb_connect, NULL, NULL, NULL );

        DispMessage( "STATE : MATCHING", NULL, "cancel", NULL, NULL, NULL );

        return state_matching;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŒʐM��ؒf���A������Ԃɖ߂�B
        DWC_ShutdownFriendsMatch();
        return state_disconnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_Y )
    {
        DTUD_DispFriendList();
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_L )
    {
        DispMessage( "SAVE DATA?", "save", "cancel", NULL, NULL, NULL );
        return state_save;
    }
    return state_online;
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
        DWC_CancelMatching();
    }

    if ( s_matched )
    {
        DispMessage( "STATE : MATCHED", "send all", "shutdown", NULL, NULL, NULL );
        
        return state_matched;
    }
    else if ( s_canceled )
    {
        // �L�����Z�����ꂽ��I�����C����Ԃɖ߂�
        DispMessage( "STATE : ONLINE", "friend matching", "shutdown", NULL, "friend state", "save firned data");
        OS_TPrintf("Canceled connection.\n");

        s_canceled = FALSE;

        return state_online;
    }

    return state_matching;
}

/** --------------------------------------------------------------------
  �}�b�`���C�N�I��
  ----------------------------------------------------------------------*/
static int
update_matched( void )
{
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���őS�ڑ��z�X�g��Reliable���M���s�Ȃ�
        DWC_SendReliableBitmap( DWC_GetAIDBitmap(), s_send_buf, SIZE_SEND_BUFFER );
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŒʐM��ؒf���A������Ԃɖ߂�B
        DWC_ShutdownFriendsMatch();
        return state_disconnect;
    }

    return state_matched;
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

        DispMessage( "STATE : ONLINE", "friend matching", "shutdown", NULL, "friend state", "save firned data");
        return state_online;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "friend matching", "shutdown", NULL, "friend state", "save firned data");
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

        // �F�B���X�g��GameSpy�T�[�o��̃o�f�B���X�g�̓�������
        DWC_UpdateServersAsync(NULL,
                               cb_updateServers, NULL,
                               NULL, NULL,
                               NULL, NULL);
        
        // �R�l�N�V�����N���[�Y�R�[���o�b�N��ݒ�
        DWC_SetConnectionClosedCallback(cb_close, NULL);

        // ���M�R�[���o�b�N��ݒ�
        DWC_SetUserSendCallback( cb_send );

        // ��M�R�[���o�b�N��ݒ�
        DWC_SetUserRecvCallback( cb_recv );
    }
    else
    {
        OS_TPrintf("Login Error\n");
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
cb_connect(DWCError error, BOOL cancel, void* param)
{
    (void)param;
    
    OS_TPrintf( "Matching err:%d, cancel:%d\n", error, cancel );
    
    if ( error == DWC_ERROR_NONE )
    {
        // ��M�o�b�t�@��ݒ�
        if ( !cancel )
        {
            int i , j;

            OS_TPrintf("Connected to friend!\n");
            i = 0;
            for ( j = 0 ; j < DWC_GetNumConnectionHost() ; ++j )
            {
                if ( j != DWC_GetMyAID() )
                {
                    DWC_SetRecvBuffer( (u8)j, &s_recv_buf[i], SIZE_RECV_BUFFER );
                    i++;
                }
            }

            s_matched  = TRUE;
        }
        else
        {
            s_canceled = TRUE;
        }
    }
    else
    {
        OS_TPrintf("Matching Error\n");
    }
}

static void
cb_send( int size, u8 aid )
{
    OS_TPrintf( "Send Callback aid:%d, size:%d\n",
                aid, size );
}

static void
cb_recv( u8 aid, u8* buffer, int size )
{
    OS_TPrintf( "Recv Callback aid:%d, size:%d, buffer[0]:0x02%x\n",
                aid, size, buffer[0] );
}

static void
cb_close(DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param)
{
    (void)isLocal;
    (void)isServer;
    (void)param;

    OS_TPrintf( "Closed Callback err:%d, aid:%d, idx:%d\n",
               error, aid, index );
}


/** -------------------------------------------------------------------------
  ---------------------------------------------------------------------------*/
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
    PrintString( 1, 1, 0xf, "*** FRIEND MATCHING SAMPLE ***" );
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

    dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
    dbs_Print( 13, 0, "n:%d", DWC_GetNumConnectionHost() );
    dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
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
