#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "userdata.h"
#include "screen.h"
#include "mp.h"

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
static DWCFriendsMatchControl	s_FMCtrl;
static DWCInetControl			s_ConnCtrl;
static DWCFriendData stSampleFriendData ATTRIBUTE_ALIGN( 32 );// �T���v��MP�����p�F�B���
static u16* s_SendBuf ATTRIBUTE_ALIGN(32);                  // �f�[�^���M�p�o�b�t�@
static char stFriendKey[sizeof(DWCFriendData)];             // �F�B�o�^��
static int  s_state  = state_init;
static int  s_msgSeq;

static const char* s_messages[msg_seq_last][6] =
{
    {
        NULL, NULL, NULL, NULL, NULL, NULL
    },
    {
        "The Wi-Fi ID may have been\nerased.\nPlease check your access-point\nsettings with Nintendo WFC\nsetup.",
        "continue",
        NULL, NULL, NULL, NULL
    },
    {
        "Connect to Nintendo WFC?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "Nintendo WFC treats your\nGame Card and Nintendo DS as\na set.\nSave this Nintendo DS system's\nNintendo WFC configuration to\nthis Game Card and connect to\nNintendo WFC?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "The Wi-Fi ID from this \nNintendo DS has been saved to \nthe Game Card.\nFrom now on, please use this\nNintendo DS when attempting to\nconnect to Nintendo WFC.",
        "continue",
        NULL, NULL, NULL, NULL
    },
    {
        "Unable to connect to Nintendo \nWFC because the Wi-Fi ID from \nthe Nintendo DS and the Game \nCard do not match.",
        "continue",
        NULL, NULL, NULL, NULL
    },
    {
        "When you update your Wi-Fi ID,\nyou will lose any data used \nto connect to Nintendo WFC up \nto this point.\nUpdate the Wi-Fi ID saved to \nthis Game Card anyway?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "Do you still want to update?",
        "yes",
        "no", NULL, NULL, NULL
    },
    {
        "Connecting to Nintendo WFC...\nPlease wait a moment...",
        "success",
        "error", NULL, NULL, NULL
    },
};

/** --------------------------------------------------------------------
  static functions
  ----------------------------------------------------------------------*/
static int  update_init( void );
static int  update_initMPData( void );
static int  update_changeMPData( void );
static int  update_initFriendKey( void );
static int  update_changeFriendKey( void );
static int  update_readyToConnect( void );
static BOOL InputNumber( void );
static void DispMenuMsg( const u8 figure );
static int  do_msgSeqInit( void );
static int  do_msgSeq0003( void );
static int  do_msgSeq0004( void );
static int  do_msgSeq0005( void );
static int  do_msgSeq0006( void );
static int  do_msgSeq0007( void );
static int  do_msgSeq0008( void );
static int  do_msgSeq0009( void );

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
    switch ( s_state )
    {
    case state_init:			s_state = update_init();			  break;
    case state_initMPData:      s_state = update_initMPData();        break;
    case state_changeMPData:    s_state = update_changeMPData();      break;
    case state_initFriendKey:   s_state = update_initFriendKey();     break;
    case state_changeFriendKey: s_state = update_changeFriendKey();   break;
    case state_readyToConnect:	s_state = update_readyToConnect();	  break;
    }
}

/** --------------------------------------------------------------------
  �������
  ----------------------------------------------------------------------*/
static int
update_init( void )
{
    DispMessage( "INIT MODE", "save", "create userdata", "change MPdata", "change friendkey", "friend state" );

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^����Wi-Fi�R�l�N�V�����ڑ������ɐi��  
        s_msgSeq = msg_seq_init;
        return state_readyToConnect;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���Ńo�b�N�A�b�v�f�[�^����������
        DTUD_DeleteUserData();
        DTUD_DeleteFriendList();
        DTUD_Init();
        
        OS_TPrintf( "Delete and Create UserData.\n" );
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_X )
    {
        // X�{�^����MP�ŗF�B���o�^���[�h
        DispMessage( "CHANGE MP FRIENDDATA?", "yes", "no", NULL, NULL, NULL );
        return state_initMPData;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_Y )
    {
        // Y�{�^���ŗF�B�o�^���ŗF�B���o�^���[�h
        DispMessage( "CHANGE FRIEND KEY?", "yes", "no", NULL, NULL, NULL );
        return state_initFriendKey;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_L )
    {
        DTUD_DispFriendList();
    }

    return state_init;
}

/*---------------------------------------------------------------------------*
  Name:         update_initMPData

  Description:  MP�ʐM�p�̃f�[�^�̍쐬�ƃf�[�^�̃Z�b�g���s���B

  Arguments:    NONE.

  Returns:      �������s���ꍇ state_changeMPData ��Ԃ��B
                �����L�����Z���̏ꍇ state_init ��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_initMPData( void )
{
    DWCUserData*   s_UserData;
    DWCFriendData* s_FriendList;
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // MP�ʐM�p�f�[�^�쐬
        s_UserData = DTUD_GetUserData();
        DWC_CreateExchangeToken(s_UserData, &stSampleFriendData);
        s_SendBuf = (u16*)(&stSampleFriendData);
        
        // MP�f�[�^�Z�b�g
        s_FriendList = DTUD_GetFriendList();
        MP_SetData(s_SendBuf, s_FriendList);
        return state_changeMPData;
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        return state_init;
    }
    
    return state_initMPData;
}

/*---------------------------------------------------------------------------*
  Name:         update_changeMPData

  Description:  MP�ŗF�B�����������A������������o�^����B

  Arguments:    NONE.

  Returns:      �������I���A�܂��̓L�����Z�����ꂽ�ꍇ�� state_init ��Ԃ��B
                �I�����Ă��Ȃ��ꍇ�� state_changeMPData ��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_changeMPData( void )
{
    DWCFriendData* s_FriendList;
    int  friendIdx;
    int  i,j;

    // MP�ʐM�X�^�[�g
    if( MP_Match() )
    {
        // MP�ʐM����
        s_FriendList = DTUD_GetFriendList();
        friendIdx = DTUD_GetAvailableFriendListIndex();// ����̃f�[�^���Ȃ�
        for(i = 0; i < friendIdx; i++){
            for(j = i+1; j < friendIdx; j++){
                if(DWC_IsEqualFriendData( (const DWCFriendData*)&s_FriendList[i], (const DWCFriendData*)&s_FriendList[j])){
                    MI_CpuClear8(&s_FriendList[j], sizeof(DWCFriendData));
                }
            }
        }
        MP_Finish();
        DTUD_DispFriendList();
        return state_init;
    }
    return state_changeMPData;
}

/*---------------------------------------------------------------------------*
  Name:         update_initFriendKey

  Description:  �F�B�o�^���̏��������s���B

  Arguments:    NONE.

  Returns:      �F�B���X�g�������ς��œo�^�s�\���A�L�����Z�������ꍇ state_init ��Ԃ��B
                ��o�J�n�̏ꍇ state_changeFriendKey ��Ԃ��B
 *---------------------------------------------------------------------------*/
static int
update_initFriendKey( void )
{
    int  friendIdx = -1;
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // �F�B�o�^���̕������'0'�ɃN���A����
        MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));
        
        // �F�B���X�g�̋󂫂��`�F�b�N
        friendIdx = DTUD_GetAvailableFriendListIndex();
        if ( friendIdx == -1 )
        {
            OS_TPrintf( "Failed to register new friend. Friend list is full.\n" );
            return state_init;
        }
        else
        {
            return state_changeFriendKey;
        }
    }
    else if( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        return state_init;        
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

    if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�������Ɠ��̓L�����Z��
        DispMessage( "STATE : ONLINE", "save data", "shutdown", "change friend key", "delete friend data", "friend state" );
        return state_init;
    }
    // �F�B�o�^�����l����
    else if( InputNumber() )
    {
        // �F�B�o�^���̐������`�F�b�N
        s_UserData = DTUD_GetUserData();
        if( DWC_CheckFriendKey( s_UserData, DWC_StringToFriendKey( stFriendKey )))
        {
            // �F�B�o�^���ɂ��F�B�o�^
            s_FriendList = DTUD_GetFriendList();
            friendIdx = DTUD_GetAvailableFriendListIndex();
            DWC_CreateFriendKeyToken( &s_FriendList[friendIdx], DWC_StringToFriendKey( stFriendKey ) );
        }
        else
        {
            OS_TPrintf( "Invalid Friend Code!\n" );
        }
        DTUD_DispFriendList();
        return state_init;
    }
    
    return state_changeFriendKey;
}

/** --------------------------------------------------------------------
  Wi-Fi�R�l�N�V�����ڑ�����
  ----------------------------------------------------------------------*/
static int
update_readyToConnect( void )
{
    // �e�V�[�P���X�p�̃��b�Z�[�W��\������
    DispMessage( s_messages[s_msgSeq][0], s_messages[s_msgSeq][1], s_messages[s_msgSeq][2], s_messages[s_msgSeq][3], s_messages[s_msgSeq][4], s_messages[s_msgSeq][5]);

    switch ( s_msgSeq )
    {
    case msg_seq_init:			s_msgSeq = do_msgSeqInit();			break;
    case msg_seq_0003:			s_msgSeq = do_msgSeq0003();			break;
    case msg_seq_0004:			s_msgSeq = do_msgSeq0004();			break;
    case msg_seq_0005:			s_msgSeq = do_msgSeq0005();			break;
    case msg_seq_0006:			s_msgSeq = do_msgSeq0006();			break;
    case msg_seq_0007:			s_msgSeq = do_msgSeq0007();			break;
    case msg_seq_0008:			s_msgSeq = do_msgSeq0008();			break;
    case msg_seq_0009:			s_msgSeq = do_msgSeq0009();			break;
    }

    if ( s_msgSeq == msg_seq_exit )
    {
        return state_init;
    }
    else
    {
        return state_readyToConnect;
    }
}

/** --------------------------------------------------------------------
  �ڑ����t���[�ɏ]�����V�[�P���X����
  ----------------------------------------------------------------------*/
static int
do_msgSeqInit( void )
{
    if ( DWC_CheckHasProfile( DTUD_GetUserData() ) )
    {
        // ����GameSpy�̃v���t�@�C���������Ă���ꍇ
        if ( DWC_CheckValidConsole( DTUD_GetUserData() ) )
        {
            // DS�{�̂�DS�J�[�h�̃��[�U�f�[�^����v�����ꍇ
            return msg_seq_0003;
        }
        else
        {
            // DS�{�̂�DS�J�[�h�̃��[�U�f�[�^����v���Ȃ������ꍇ
            OS_TPrintf( "UserData is not Valid Console.\n" );

            return msg_seq_0006;
        }
    }
    else
    {
        // �܂���x�����O�C�����Ă��Ȃ��i�������̓��O�C����ɃZ�[�u
        // �ł��Ȃ������j�ꍇ�́A���̂܂ܐڑ��V�[�P���X�ɐi�ށB
        // ���̃T���v���ł́AWi-Fi�R�l�N�V�����ւ̐ڑ��͎��ۂɂ͍s�Ȃ�Ȃ��̂ŁA
        // �F�؍ς݃��O�C��ID������ł����A���񂱂��ɗ��邱�ƂɂȂ�B
        return msg_seq_0004;
    }

    return msg_seq_init;
}

static int
do_msgSeq0003( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^����Wi-Fi�R�l�N�V�����ւ̐ڑ����J�n����
        return msg_seq_0009;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŏ����𒆒f���čŏ��ɖ߂�
        return msg_seq_exit;
    }

    return msg_seq_0003;
}

static int
do_msgSeq0004( void )
{
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^����Wi-Fi�R�l�N�V�����ւ̐ڑ����J�n����
        return msg_seq_0009;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŏ����𒆒f���čŏ��ɖ߂�
        return msg_seq_exit;
    }

    return msg_seq_0004;
}

static int
do_msgSeq0005( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���Őڑ������������Ƃɂ��āA�܂��ŏ��ɖ߂�
        OS_TPrintf( "Connected!\n" );
        return msg_seq_exit;
    }

    return msg_seq_0005;
}

static int
do_msgSeq0006( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���Ŏ��ɐi��
        return msg_seq_0007;
    }

    return msg_seq_0006;
}

static int
do_msgSeq0007( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���Ŏ��ɐi��
        return msg_seq_0008;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŏ����𒆒f���čŏ��ɖ߂�
        return msg_seq_exit;
    }

    return msg_seq_0007;
}

static int
do_msgSeq0008( void )
{

    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^����Wi-Fi�R�l�N�V�����ւ̐ڑ����J�n����
        return msg_seq_0009;
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŏ����𒆒f���čŏ��̎���ɖ߂�
        return msg_seq_0006;
    }

    return msg_seq_0008;
}

static int
do_msgSeq0009( void )
{
    
    if ( g_KeyCtrl.trg & PAD_BUTTON_A )
    {
        // A�{�^���ŃG���[�Ȃ������������Ƃɂ���
        DTUD_SaveFriendList();
        OS_TPrintf( "Saved FriendData.\n");
        if ( DTUD_CheckUpdateUserData() )
        {
            // ���[�U�f�[�^���X�V����Ă���ꍇ�̓Z�[�u����
            DTUD_SaveUserData();

            OS_TPrintf( "Saved UserData.\n");
            return msg_seq_0005;
        }
        else
        {
            // �ڑ������������Ƃɂ��āA�܂��ŏ��ɖ߂�
            OS_TPrintf( "Connected!\n" );
            return msg_seq_exit;
        }
    }
    else if ( g_KeyCtrl.trg & PAD_BUTTON_B )
    {
        // B�{�^���ŃG���[�������������Ƃɂ��čŏ��ɖ߂�
        OS_TPrintf( "Error occured!\n" );
        return msg_seq_exit;
    }

    return msg_seq_0009;
}

/*---------------------------------------------------------------------------*
  Name:         InputNumber

  Description:  �F�B�o�^���̐��l���͂��s���B

  Arguments:    NONE.

  Returns:      �F�B�o�^�����͂̏�Ԃ�Ԃ��B
                key_change:���͏I��
                key_set   :���͒�
 *---------------------------------------------------------------------------*/
static BOOL InputNumber( void )
{
    static u8  figure = 0;
    u8 maxFigure = 12;

    DispMenuMsg(figure);
    
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
        DispMenuMsg(figure);
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
        DispMenuMsg(figure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_LEFT)
    {
        // �\���L�[���E�őI�������̈ړ�
        if (figure > 0 && figure <= maxFigure-1 ) figure--;
        // ���͒����l�\��
        DispMenuMsg(figure);
    }
    else if (g_KeyCtrl.trg & PAD_KEY_RIGHT)
    {
        // �\���L�[���E�őI�������̈ړ�
        if (figure >= 0 && figure < maxFigure-1 ) figure++;
        // ���͒����l�\��
        DispMenuMsg(figure);
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
static void DispMenuMsg( const u8 figure )
{
    int i;
    u8 maxFigure = 12;

    int x = 16;

    DispMessage( "NUMBER INPUT", "set data", "cancel", NULL, NULL, NULL );
    
    // ���l���͒�
    // �F�B�o�^�B�F�B�o�^�����͒�
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
void
DispMessage( const char* main_msg, const char* msg_a, const char* msg_b, const char* msg_x, const char* msg_y,  const char* msg_l )
{
    s16 x = 1;
    s16 y = 7;

    ClearScreen();
    
    PrintString( 0, 0, 0xf, "================================" );
    PrintString( 2, 1, 0xf, "*** USER DATA SAMPLE ***" );
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
  ----------------------------------------------------------------------*/
/** --------------------------------------------------------------------
  ���t���[���Ăяo���A��ԕ\���֐�
  ----------------------------------------------------------------------*/
void
DrawConsole( void )
{
    static int i = 0;
    char loop[] = "/|\\-";

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
    if ( DWC_Init( s_Work ) == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        // DS�{�̂�NVRAM���j�󂳂�Ă���
        DispMessage( s_messages[s_msgSeq][0], s_messages[s_msgSeq][1], s_messages[s_msgSeq][2], s_messages[s_msgSeq][3], s_messages[s_msgSeq][4], s_messages[s_msgSeq][5]);
        
        while (1)
        {
            DrawConsole();
            ReadKeyData();

            // A�{�^���Ő�ɐi��
            if ( g_KeyCtrl.trg & PAD_BUTTON_A ) break;

            dbs_DemoUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();
        }
    }

    // ���[�U�f�[�^�ǂݍ���
    DTUD_Init();
    // �����܂ŋN�����̏���


    // ��������Q�[������Wi-Fi�֘A����
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
