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

/** --------------------------------------------------------------------
  globals
  ----------------------------------------------------------------------*/
KeyControl		g_KeyCtrl;

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
static DWCFriendsMatchControl s_FMCtrl;
static DWCInetControl		  s_ConnCtrl;
static char stFriendKey[sizeof(DWCFriendData)]; // �F�B�o�^��
static int  s_state  = state_init;
static BOOL s_logined;
static BOOL s_canceled;

static int  s_profileID = 0;
static u64  s_friendkey = 0;

/** --------------------------------------------------------------------
  static functions
  ----------------------------------------------------------------------*/
static int update_friend( void );
static int update_init( void );
static int update_connect( void );
static int update_login( void );
static int update_online( void );
static int update_initFriendKey( void );
static int update_changeFriendKey( void );
static int update_initFriendData( void );
static int update_inputFriendIndex( void );
static int update_deleteFriendData( void );
static int update_disconnect( void );
static int update_save( void );
static int update_error( void );

static void cb_login( DWCError error, int profileID, void* param );
static void cb_updateServers(DWCError error, BOOL isChanged, void* param);
static void cb_close( DWCError error, BOOL isLocal, BOOL isServer, u8  aid, int index, void* param );
static int  InputNumber( u8 maxFigure );
static void DispMenuMsg( const u8 figure, const u8 maxFigure );
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
    case state_init:			 s_state = update_init();			 break;
    case state_connect:			 s_state = update_connect();		 break;
    case state_login:			 s_state = update_login();			 break;
    case state_online:			 s_state = update_online();			 break;
    case state_initFriendKey:    s_state = update_initFriendKey();   break;
    case state_changeFriendKey:  s_state = update_changeFriendKey(); break;
    case state_initFriendData:   s_state = update_initFriendData();  break;
    case state_inputFriendIndex: s_state = update_inputFriendIndex();break;
    case state_deleteFriendData: s_state = update_deleteFriendData();break;
    case state_disconnect:		 s_state = update_disconnect();		 break;
    case state_save:             s_state = update_save();            break;
    case state_error:			 s_state = update_error();			 break;
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
  �������
  ----------------------------------------------------------------------*/
static int
update_init( void )
{
    // DS�{�̂�DS�J�[�h�̃��[�U�f�[�^����v���邩
    if ( DWC_CheckValidConsole( DTUD_GetUserData() ) )
    {
        DispMessage( "STATE : CONNECTING", NULL, NULL, NULL, NULL, NULL );

        DWC_InitInet( &s_ConnCtrl );
        
        DWC_SetAuthServer( DWC_CONNECTINET_AUTH_RELEASE );
        
        DWC_ConnectInetAsync();

        // �F�B�o�^�����擾
        s_friendkey = DWC_CreateFriendKey( DTUD_GetUserData() );
        
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
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );

        // ingamesn�`�F�b�N�̌��ʂ��擾����
	    if( DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID )
        {
            // �s�K�؂Ȗ��O�Ɣ��f���ꂽ�ꍇ�͓��ʂȏ������K�v
		    OS_TPrintf("BAD ingamesn is detected by NAS.\n");
        }

        // �F�B�o�^�����擾
        s_friendkey = DWC_CreateFriendKey( DTUD_GetUserData() );

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
        DispMessage( "SAVE DATA?", "save", "cancel", NULL, NULL, NULL );
        return state_save;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DWC_ShutdownFriendsMatch();
        return state_disconnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_X )
    {
        DispMessage( "CHANGE FRIEND KEY?", "yes", "no", NULL, NULL, NULL );
        return state_initFriendKey;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_Y )
    { 
        DispMessage( "DELETE FRIEND DATA?", "yes", "no", NULL, NULL, NULL );
        return state_initFriendData;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_L )
    {
        DTUD_DispFriendList();
        return state_online;
    }

    return state_online;
}

/*---------------------------------------------------------------------------*
  Name:         update_initFriendKey

  Description:  �F�B�o�^���̏��������s���B

  Arguments:    NONE.

  Returns:      �F�B���X�g�������ς��œo�^�s�\���A�L�����Z�������ꍇ state_online ��Ԃ��B
                ��o�J�n�̏ꍇ state_changeFriendKey ��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_initFriendKey( void )
{
    int  friendIdx = -1;

    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // �F�B�o�^���̕������'0'�ɃN���A����
        MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));

        //�F�B���X�g�̋󂫂��`�F�b�N
        friendIdx = DTUD_GetAvailableFriendListIndex();
        if ( friendIdx == -1 )
        {
            OS_TPrintf( "Failed to register new friend. Friend list is full.\n" );
            DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
            return state_online;
        }
        else
        {
            return state_changeFriendKey;
        }
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // �L�����Z�����ꂽ��I�����C����Ԃɖ߂�
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    return state_initFriendKey;
}

/*---------------------------------------------------------------------------*
  Name:         update_changeFriendKey

  Description:  �F�B�o�^���̓��͂ƁA�F�B�o�^���s���B

  Arguments:    NONE.

  Returns:      �o�^���I�������ꍇ�� state_init ��Ԃ��B
                �I�����Ă��Ȃ��ꍇ�� state_changeFriendKey ��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_changeFriendKey( void )
{
    DWCUserData*   s_UserData;
    DWCFriendData* s_FriendList;
    int  friendIdx = -1;
    u8   maxFigure = 12;// �F�B�o�^���͍ō��P�Q��

    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���������Ɠ��̓L�����Z��
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;        
    }
    // �F�B�o�^�����l����
    else if( InputNumber( maxFigure ) )
    {
        // �F�B�o�^���̐������`�F�b�N
        s_UserData = DTUD_GetUserData();
        if( DWC_CheckFriendKey( s_UserData, DWC_StringToFriendKey( stFriendKey )))
        {
            // �F�B�o�^���ɂ��F�B�o�^
            friendIdx = DTUD_GetAvailableFriendListIndex();
            s_FriendList = DTUD_GetFriendList();
            DWC_CreateFriendKeyToken( &s_FriendList[friendIdx], DWC_StringToFriendKey( stFriendKey ) );
        }
        else
        {
            OS_TPrintf( "Invalid Friend Code!\n" );
        }
        DTUD_DispFriendList();
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    
    return state_changeFriendKey;
}

/*---------------------------------------------------------------------------*
  Name:         update_initFriendData

  Description:  �F�B�폜�̂��߂̏��������s���B

  Arguments:    NONE.

  Returns:      �폜�I�����A�L�����Z�������ꍇ state_online ��Ԃ��B
                �܂���o�^���̏ꍇ state_deleteFriendData ��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_initFriendData( void )
{
    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        DTUD_DispFriendList();
        
        // �F�B�o�^���̕������'0'�ɃN���A����
        MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));

        return state_inputFriendIndex;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }

    return state_initFriendData;
    
}

/*---------------------------------------------------------------------------*
  Name:         update_inputFriendIndex

  Description:  �F�B���X�g�C���f�b�N�X�̐��l���͂��s���B

  Arguments:    NONE.

  Returns:      BOOL - �폜���I���A�܂��̓L�����Z�������ꍇTRUE
                       �I�����Ă��Ȃ��ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_inputFriendIndex( void )
{
    u8 maxFigure = 2;
    u8 i;

    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V

    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���������Ɠ��̓L�����Z��
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;        
    }
    else if( InputNumber( maxFigure ) )
    {
        // �F�B���X�g���ҏW�\���m�F
        if( DWC_CanChangeFriendList() )
        {
            DispMessage( "Is Data deleted, Really?", "yes", "no", NULL, NULL, NULL );
            PrintString( 1, 15, 0xf, "Set friend index:");
            for(i = 0; i < maxFigure; i++){
                PrintString( (s16)(18+i), 15, 0xf, "%c", stFriendKey[i] );
            }
            return state_deleteFriendData;
        }
        else
        {
            OS_TPrintf( "Invalid Friend Index!\n" );
            DTUD_DispFriendList();
            DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
            return state_online;
        }
    }
    return state_inputFriendIndex;
}

/*---------------------------------------------------------------------------*
  Name:         update_deleteFriendData

  Description:  �F�B���X�g�C���f�b�N�X�ɂ��F�B�폜���s���B

  Arguments:    NONE.

  Returns:      �폜���I���A�܂��̓L�����Z�������ꍇ��state_online��Ԃ��B
                �I�����Ă��Ȃ��ꍇ��state_deleteFriendData��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_deleteFriendData( void )
{
    DWCFriendData* s_FriendList;
    int friendIdx;
    u8 maxFigure = 2;

    DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V
        
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // ���̓f�[�^��int�^�ɕϊ�
        friendIdx = (int)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));

        // �F�B���X�g����F�B���폜
        s_FriendList = DTUD_GetFriendList();
        DWC_DeleteBuddyFriendData(&s_FriendList[friendIdx]);

        DTUD_DispFriendList();
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    return state_deleteFriendData;
}

/** --------------------------------------------------------------------
  �ڑ��ؒf
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

        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_online;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
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

/*---------------------------------------------------------------------------*
  Name:         InputNumber

  Description:  �F�B�o�^���̐��l���͂��s���B

  Arguments:    maxFigure - ���͂���ő�̌���

  Returns:      ���͏I���Ȃ�TRUE
                ���͒��Ȃ�FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL InputNumber( u8 maxFigure )
{
    static u8 figure = 0;

    if(figure > maxFigure){
        figure = 0;
    }
    
    DispMenuMsg(figure, maxFigure);

    if (g_KeyCtrl.trg & PAD_BUTTON_A)
    {
        figure = 0;
        return TRUE;
    }
    else if (g_KeyCtrl.trg & PAD_KEY_UP)
    {
        // �\���L�[�㉺�őI�𒆂̐��l�̑���
        stFriendKey[figure]++;
        if(stFriendKey[figure] > '9')
        {
            stFriendKey[figure] = '0';
        }
        // ���͒����l�\��
        DispMenuMsg(figure ,maxFigure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_DOWN)
    {
        // �\���L�[�㉺�őI�𒆂̐��l�̑���
        stFriendKey[figure]--;
        if(stFriendKey[figure] < '0')
        {
            stFriendKey[figure] = '9';
        }
        // ���͒����l�\��
        DispMenuMsg(figure, maxFigure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_LEFT)
    {
        // �\���L�[���E�őI�������̈ړ�
        if (figure > 0 && figure <= maxFigure-1 ) figure--;
        // ���͒����l�\��
        DispMenuMsg(figure, maxFigure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_RIGHT)
    {
        // �\���L�[���E�őI�������̈ړ�
        if (figure >= 0 && figure < maxFigure-1 ) figure++;
        // ���͒����l�\��
        DispMenuMsg(figure, maxFigure);
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         DispMenuMsg

  Description:  �F�B�o�^���̐��l��\������B

  Arguments:    figure    - ���͒��̌���
                maxFigure - �ő包��

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
static void DispMenuMsg( const u8 figure, const u8 maxFigure )
{
    int i;

    int x = 16;

    DispMessage( "NUMBER INPUT", "set data", "cancel", NULL, NULL, NULL );
    
    // ���l���͒�
    // �F�B�o�^�B�F�B�o�^�����͒�
    if( maxFigure == 12 )
    {
        PrintString( 1, 15, 0xf, "Set friend key:");
        for(i = 0; i < maxFigure; i++){
            PrintString( (s16)(x+i), 15, 0xf, "%c", stFriendKey[i] );
            if(i%4 == 3 && i != maxFigure - 1)
            {
                x++;
                PrintString( (s16)(x+i), 15, 0xf, "-" );
            }
        }
        // �I�𒆂̐����������ׂ̉�����\������
        PrintString( (s16)(16+figure+figure/4), 16, 0xf, "-" );
    }
    else if( maxFigure == 2 )
    {
        x += 2;
        PrintString( 1, 15, 0xf, "Set friend index:");
        for(i = 0; i < maxFigure; i++){
            PrintString( (s16)(x+i), 15, 0xf, "%c", stFriendKey[i] );
        }
        // �I�𒆂̐����������ׂ̉�����\������
        PrintString( (s16)(18+figure), 16, 0xf, "-" );
    }
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
    PrintString( 2, 1, 0xf, "*** FRIEND DATA SAMPLE ***" );
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
DrawConsole( void )
{
    static int i = 0;
    char loop[] = "/|\\-";

    dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
    dbs_Print( 13, 0, "n:%d", DWC_GetNumConnectionHost() );
    dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
    dbs_Print( 13, 1, "p:%d", s_profileID );
    dbs_Print( 30, 0, "%c", loop[((i)>>2)&3] );
    dbs_Print( 0, 2, "friendkey:%012lld", s_friendkey );

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
        DrawConsole();

        ReadKeyData();

        update();

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
        dbs_DemoLoad();
    }

    OS_Terminate();
}
