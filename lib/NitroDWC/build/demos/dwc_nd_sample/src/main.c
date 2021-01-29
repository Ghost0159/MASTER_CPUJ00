/*---------------------------------------------------------------------------*
 * Project : NitroDWC - dwc_nd_sample
 * File    : main.c
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * include
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <dwc.h>

#include "main.h"
#include "init.h"

#include "dbs.h"

/*---------------------------------------------------------------------------*
 * define
 *---------------------------------------------------------------------------*/
#define GAME_FRAME				1		// �z�肷��Q�[���t���[���i1/60��1�Ƃ���j
#define NETCONFIG_USE_HEAP		1
//#define USE_AUTHSERVER_RELEASE			// �{�ԃT�[�o�֐ڑ�

// -- ND �Ɋ֘A�����萔 --
#define FILE_NUM				10
enum{
	ND_RESULT_COMPLETE,
	ND_RESULT_NOT_FOUND_FILES,
	ND_RESULT_DOWNLOAD_CANCEL,
	ND_RESULT_ERROR
};

/*---------------------------------------------------------------------------*
 * typedef
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * variable
 *---------------------------------------------------------------------------*/
static DWCInetControl stConnCtrl;
static BOOL           printOverride;				// �f�o�b�O�o�͂̃I�[�o�[���C�h
static u8             dwcWork[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
static char           printBuffer[ 256 ];			// thread unsafe, but less use of stack
static KeyInfo        keyData;							// �L�[����

static u32 loopcnt = 0;
static char loopstr[] = "/|\\-";

// -- ND �Ɋ֘A�����ϐ� --
static BOOL           gdlcallback  = FALSE;
static BOOL           gdlcallback2 = FALSE;
static BOOL           nderror      = FALSE;
static char           buffer[ 1024*1024*1 ];
static DWCNdFileInfo  fileInfo[ FILE_NUM ];
static int            result;

/*---------------------------------------------------------------------------*
 * prototype
 *---------------------------------------------------------------------------*/
static void  NetConfigMain ( void );
static BOOL  StartIPMain   ( void );
static void  WaitVBlankIntr( void );
static void  VBlankIntr    ( void );
static void* AllocFunc     ( DWCAllocType name, u32 size, int align );
static void  FreeFunc      ( DWCAllocType name, void* ptr, u32 size );

// -- ND �Ɋ֘A�����v���g�^�C�v --
static int  NdMain           ( void );
static void WaitNdCallback   ( void );
static void NdSetAttribute   ( void );
static int  SelectFile       ( int num );
static void NdCallback       ( DWCNdCallbackReason reason, DWCNdError error, int servererror );
static void NdCleanupCallback( void );

/* ========================================================================= *
 * �֘A����֐��Q
 * ========================================================================= */
/*-------------------------------------------------------------------------*
 * Name        : NitroMain
 * Description : ���C�����[�`��
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
void NitroMain( void )
{
    int ret;

	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();

    // �X�^�b�N���`�F�b�N�����ݒ�
    OS_SetThreadStackWarningOffset( OS_GetCurrentThread(), 0x100 );
    Heap_Init();

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

    // �o�͏�����
    dbs_DemoInit();
    printOverride = TRUE; // OS_TPrintf()�̏o�͂�console�ɂȂ�.

    // �\���J�n
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // �f�o�b�O�p�ɃR���\�[���̏����폜����B
    //DWC_Debug_DWCInitError( dwcWork, DWC_INIT_RESULT_DESTROY_OTHER_SETTING );

    // �f�o�b�O�\�����x���w��
    DWC_SetReportLevel((unsigned long)(DWC_REPORTFLAG_ALL & ~DWC_REPORTFLAG_QR2_REQ));

    // DWC���C�u����������
    ret = DWC_Init( dwcWork );
    OS_TPrintf( "DWC_Init() result = %d\n", ret );

    if ( ret == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        OS_TPrintf( "Wi-Fi setting might be broken.\n" );
    }

    // �q�[�v�g�p�ʕ\��ON
    //Heap_SetDebug(TRUE);

    // �������m�ۊ֐��ݒ�
    DWC_SetMemFunc( AllocFunc, FreeFunc );
        
	NetConfigMain(); // �܂�WiFi�R�l�N�V�����ݒ�GUI���N������
    
	// �C���^�[�l�b�g�ڑ����[�v
	while( 1 )
	{
		if( StartIPMain() == TRUE ) break;
		WaitVBlankIntr();
    }
	
	// ���ۂ̃v���O�����ł́A�_�E�����[�h���C�u�������g�p����O�ɕK���F�؃T�[�o�ł̔F�؂��������Ă��邱�Ƃ��m�F���Ă�������
	while( 1 )
	{
		//OS_DumpHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE);
		
		result = NdMain();
		
		// ���ۂ̃v���O�����ł́A�G���[�����������ꍇ�ɂ�DWC_GetLastErrorEx�֐��Ŏ擾����Ώ��@�ɏ]���Ă�������
		
		// �������
		gdlcallback2 = FALSE;
		if(DWC_NdCleanupAsync( NdCleanupCallback ))
		{
			while( gdlcallback2 == FALSE )
				WaitVBlankIntr();
		}
		
		//OS_DumpHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE);
		
		// �G���[���������Ă����ꍇ�͏������~�߂�
		if(result == ND_RESULT_ERROR)
			OS_Panic("Nd library returned an error.\n");

		WaitVBlankIntr();
	}
	OS_Terminate();
}

/*-------------------------------------------------------------------------*
 * Name        : NdMain
 * Description : ND�̃��C������
 * Arguments   : None.
 * Returns     : int - �I������
 *-------------------------------------------------------------------------*/
static int NdMain( void )
{
	int no,entrynum;

	// ND Start
	OS_TPrintf("--------------------------------\n");
	OS_TPrintf("DWC_ND: start - download -\n");
	OS_TPrintf("--------------------------------\n");
	WaitVBlankIntr();

	// -- Nd���C�u���������� --------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdInitAsync( NdCallback, "NTRJ", "1234123412341234" ) == FALSE )
		return ND_RESULT_ERROR;

	WaitNdCallback();
	
	if(nderror == TRUE)
		return ND_RESULT_ERROR;

	// -- �A�g���r���[�g�ݒ�i�w�薳���A�S�Ẵt�@�C�����擾����j-------------
	NdSetAttribute();

	// -- �t�@�C�����擾 ------------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdGetFileListNumAsync( &entrynum ) == FALSE )
		return ND_RESULT_ERROR;
	
	WaitNdCallback();
	
	if(nderror == TRUE)
		return ND_RESULT_ERROR;
	OS_TPrintf( "Get FileNum = %d\n", entrynum );

	// -- �t�@�C�����X�g�擾 --------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdGetFileListAsync( fileInfo, 0, FILE_NUM ) == FALSE )
		return ND_RESULT_ERROR;

	WaitNdCallback();
	
	if(nderror == TRUE)
		return ND_RESULT_ERROR;

	// �擾�����t�@�C�������O�̏ꍇ�A�I������
	if( entrynum == 0 )
	{
		// �_�E�����[�h�ł���t�@�C�����Ȃ�����
		return ND_RESULT_NOT_FOUND_FILES;
	}

	// -- �擾����t�@�C����I�� ----------------------------------------------
	no = SelectFile( entrynum );

	// -- �t�@�C���擾 --------------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdGetFileAsync( &fileInfo[ no ], buffer, sizeof( buffer )) == FALSE )
		return ND_RESULT_ERROR;

	// �t�@�C���擾�I���҂�
	while( gdlcallback == FALSE )
	{
		u32 recived,contentlen,percent;
		// �L�����Z������
		KeyRead();
		if( KEY_TRIGGER( PAD_BUTTON_B ))
		{
			if( DWC_NdCancelAsync() == FALSE )
			{
				OS_Panic( "DWC_NdCancelAsync: Failed.\n" );
			}
			// �_�E�����[�h�L�����Z��
			WaitNdCallback();
			return 	ND_RESULT_DOWNLOAD_CANCEL;
		}
		// �i�s�x�`�F�b�N
		if( DWC_NdGetProgress( &recived, &contentlen ) == TRUE )
		{
			if( percent != (recived*100)/contentlen )
			{
				percent = (recived*100)/contentlen;
				OS_TPrintf( "Select File No[%d] %s\n", no, fileInfo[ no ].name );
				OS_TPrintf( "Download %d/100\n", percent );
			}
		}
        WaitVBlankIntr();
	}
	if(nderror == TRUE)
		return ND_RESULT_ERROR;

	// �_�E�����[�h�I��
	return 	ND_RESULT_COMPLETE;
}

/*-------------------------------------------------------------------------*
 * Name        : WaitNdCallback
 * Description : ND�p�R�[���o�b�N�҂�����
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void WaitNdCallback( void )
{
	while( gdlcallback == FALSE )
	{
		WaitVBlankIntr();
	}
}

/*-------------------------------------------------------------------------*
 * Name        : NdSetAttribute
 * Description : ND�p�@�_�E�����[�h����t�@�C��������ݒ肷��
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void NdSetAttributeInfo( char *attr1, char *attr2, char *attr3 )
{
#ifdef SDK_FINALROM
#pragma unused(attr1)
#pragma unused(attr2)
#pragma unused(attr3)
#endif
	OS_TPrintf( "\nSelect Attribute \n" );
	OS_TPrintf( " Up/Down    - change attr \n" );
	OS_TPrintf( " Left/Right - select attr no\n" );
	OS_TPrintf( " A          - done\n" );
	OS_TPrintf( " Attribute [%s] [%s] [%s]\n", attr1, attr2, attr3 );
}

static void NdSetAttribute( void )
{
	static char attr_string[ 4 ][ 2 ] = {{ 0 }, {'a'}, {'b'}, {'c'}};
	char attr1[ 2 ] = { 0 };
	char attr2[ 2 ] = { 0 };
	char attr3[ 2 ] = { 0 };
	int  no[ 3 ], pos = 0;
	no[ 0 ] = 0;
	no[ 1 ] = 0;
	no[ 2 ] = 0;

	NdSetAttributeInfo( attr1, attr2, attr3 );

	while( 1 )
	{
		// �L�[���͏���
		KeyRead();
		if( KEY_TRIGGER( PAD_BUTTON_A )) break;
		if( KEY_TRIGGER( PAD_KEY_UP ))         no[ pos ]++;
		else if( KEY_TRIGGER( PAD_KEY_DOWN ))  no[ pos ]--;
		if( KEY_TRIGGER( PAD_KEY_LEFT ))       pos--;
		else if( KEY_TRIGGER( PAD_KEY_RIGHT )) pos++;
		if( no[ pos ] < 0 )      no[ pos ] = 3;
		else if( no[ pos ] > 3 ) no[ pos ] = 0;
		if( pos < 0 )            pos = 3;
		else if( pos > 3 )       pos = 0;
		if( pos == 0 )      attr1[ 0 ] = attr_string[ no[ pos ] ][ 0 ];
		else if( pos == 1 ) attr2[ 0 ] = attr_string[ no[ pos ] ][ 0 ];
		else                attr3[ 0 ] = attr_string[ no[ pos ] ][ 0 ];
		// �\���̍X�V
		if( KEY_TRIGGER( PAD_KEY_UP|PAD_KEY_DOWN ))
		{
			NdSetAttributeInfo( attr1, attr2, attr3 );
		}
        // V�u�����N�҂�����
        WaitVBlankIntr();
	}

	// �A�g���r���[�g�̃Z�b�g
	if( DWC_NdSetAttr( attr1, attr2, attr3 ) == FALSE )
	{
		OS_Panic( "DWC_NdSetAttr: Failed\n." );
	}
}

/*-------------------------------------------------------------------------*
 * Name        : SelectFile
 * Description : ND�p�@�_�E�����[�h����t�@�C�������߂�
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void SelectFileInfo( int no, int num )
{
#ifdef SDK_FINALROM
#pragma unused(no)
#endif
	int i;

	for( i=0; i<num; i++ )
	{
		OS_TPrintf( "[%d] name = %s\n", i, fileInfo[i].name );
		OS_TPrintf( "[%d] size = %d\n", i, fileInfo[i].size );
	}
	OS_TPrintf( "\nSelect Download file\n" );
	OS_TPrintf( " Up/Down - change file\n" );
	OS_TPrintf( " A       - done\n\n" );
	OS_TPrintf( "Select File No[%d] %s\n", no, fileInfo[ no ].name );
}

static int SelectFile( int num )
{
	int no = 0;

	SelectFileInfo( no, num );
	while( 1 )
	{
		// �L�[���͏���
		KeyRead();
		if( KEY_TRIGGER( PAD_BUTTON_A )) break;
		if( KEY_TRIGGER( PAD_KEY_UP ))        no++;
		else if( KEY_TRIGGER( PAD_KEY_DOWN )) no--;
		if( no < 0 )         no = num - 1;
		else if( no >= num ) no = 0;
		// �\���̍X�V
		if( KEY_TRIGGER( PAD_KEY_UP|PAD_KEY_DOWN ))
		{
			SelectFileInfo( no, num );
		}
        // V�u�����N�҂�����
        WaitVBlankIntr();
	}
	return no;
}

/*-------------------------------------------------------------------------*
 * Name        : NdCallback
 * Description : ND�p�R�[���o�b�N
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void NdCallback(DWCNdCallbackReason reason, DWCNdError error, int servererror)
{
#pragma unused(reason)
#pragma unused(error)
#pragma unused(servererror)
	
	OS_TPrintf("NdCallback: Called\n");
	switch(reason) {
	case DWC_ND_CBREASON_GETFILELISTNUM:
		OS_TPrintf("DWC_ND_CBREASON_GETFILELISTNUM\n");
		break;
	case DWC_ND_CBREASON_GETFILELIST:
		OS_TPrintf("DWC_ND_CBREASON_GETFILELIST\n");
		break;
	case DWC_ND_CBREASON_GETFILE:
		OS_TPrintf("DWC_ND_CBREASON_GETFILE\n");
		break;
	case DWC_ND_CBREASON_INITIALIZE:
		OS_TPrintf("DWC_ND_CBREASON_INITIALIZE\n");
		break;
	}
	
	// ���ۂ̃v���O�����ł͓K�؂ȃG���[�������s���Ă�������
	// �܂��A���̍ۂɂ̓R�[���o�b�N������DWC_NdCleanupAsync()���Ăяo���Ȃ����Ƃɒ��ӂ��Ă�������
	switch(error) {
	case DWC_ND_ERROR_NONE:
		OS_TPrintf("DWC_ND_NOERR\n");
		break;
	case DWC_ND_ERROR_ALLOC:
		OS_TPrintf("DWC_ND_MEMERR\n");
		break;
	case DWC_ND_ERROR_BUSY:
		OS_TPrintf("DWC_ND_BUSYERR\n");
		break;
	case DWC_ND_ERROR_HTTP:
		OS_TPrintf("DWC_ND_HTTPERR\n");
		break;
	case DWC_ND_ERROR_BUFFULL:
		OS_TPrintf("DWC_ND_BUFFULLERR\n");
		break;
	case DWC_ND_ERROR_DLSERVER:
		OS_TPrintf("DWC_ND_SERVERERR\n");
		break;
	case DWC_ND_ERROR_CANCELED:
		OS_TPrintf("DWC_ND_CANCELERR\n");
		break;
	}
	OS_TPrintf("errorcode = %d\n", servererror);
	gdlcallback = TRUE;
	if(error != DWC_ND_ERROR_NONE)
		nderror = TRUE;
}

/*-------------------------------------------------------------------------*
 * Name        : NdCleanupCallback
 * Description : DWC_NdCleanupAsync�p�R�[���o�b�N
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void NdCleanupCallback( void )
{

	OS_TPrintf("--------------------------------\n");
	switch( result )
	{
	case ND_RESULT_COMPLETE:
		OS_TPrintf("DWC_ND: finished - complete -\n");
		break;
	case ND_RESULT_NOT_FOUND_FILES:
		OS_TPrintf("DWC_ND: finished - no files -\n");
		break;
	case ND_RESULT_DOWNLOAD_CANCEL:
		OS_TPrintf("DWC_ND: finished - cancel -\n");
		break;
	case ND_RESULT_ERROR:
		OS_TPrintf("DWC_ND: finished - error -\n");
		break;
	}
	OS_TPrintf("--------------------------------\n");

	gdlcallback2 = TRUE;
}

/* ========================================================================= *
 * ��֘A�֐��Q
 * ========================================================================= */
/*-------------------------------------------------------------------------*
 * Name        : NetConfigMain
 * Description : �ݒ�GUI
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static void NetConfigMain( void )
{
  //GameMode returnSeq = GAME_MODE_MAIN;

    (void)FS_LoadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    printOverride = FALSE; // OS_TPrintf()�̏o�͂��ꎞ�I�Ɍ��ɖ߂��B
    dbs_DemoFinalize();

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer( DWC_CONNECTINET_AUTH_RELEASE );
#endif

#if defined( NETCONFIG_USE_HEAP )
    {
        void* work = OS_Alloc( DWC_UTILITY_WORK_SIZE );

        (void)DWC_StartUtility( work, DWC_LANGUAGE_JAPANESE, DWC_UTILITY_TOP_MENU_FOR_JPN );

        OS_Free( work );
    }
#else
    (void)DWC_StartUtilityEx( DWC_LANGUAGE_JAPANESE, 0 );
#endif

    (void)FS_UnloadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID( main_overlay_1 )) ;

    OS_SetIrqFunction( OS_IE_V_BLANK, VBlankIntr );
    (void)OS_EnableIrqMask( OS_IE_V_BLANK );
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr( TRUE );

    dbs_DemoInit();
    printOverride = TRUE;			// OS_TPrintf()�̏o�͂�console�ɂȂ�.
    OS_WaitVBlankIntr();			// Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    //return returnSeq;
}

/*-------------------------------------------------------------------------*
 * Name        : StartIPMain
 * Description : �C���^�[�l�b�g�ւ̐ڑ�����
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static BOOL StartIPMain( void )
{
	DWCApInfo apinfo;

    DWC_InitInet( &stConnCtrl );

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer( DWC_CONNECTINET_AUTH_RELEASE );
#endif

	// �ڑ��J�n
	DWC_ConnectInetAsync();

	// ����ȃX�e�[�g�܂ő҂B
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        // V�u�����N�҂�����
        WaitVBlankIntr();
    }

	// �ڑ����������A���ʂ��m�F
    switch ( DWC_GetInetStatus() )
    {
	// �ڑ����s
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int  errcode;
            err = DWC_GetLastError( &errcode );
            OS_TPrintf( "   Error occurred %d %d.\n", err, errcode );
        }
        DWC_ClearError();
        DWC_CleanupInet();
        break;
	// �ڑ����s
    case DWC_CONNECTINET_STATE_FATAL_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int  errcode;
            err = DWC_GetLastError( &errcode );
            OS_Panic( "   Error occurred %d %d.\n", err, errcode );
        }
        break;
	// �ڑ�����
    case DWC_CONNECTINET_STATE_CONNECTED:
		OS_TPrintf( "   CONNECTED!!!.\n" );
		if( DWC_GetApInfo(&apinfo) == TRUE ) {
			OS_TPrintf( "   DWCApInfo.aptype = %d\n", apinfo.aptype );
			OS_TPrintf( "   DWCApInfo.area   = %d\n", apinfo.area );
			OS_TPrintf( "   DWCApInfo.spotinfo = %s\n", apinfo.spotinfo );
			OS_TPrintf( "   DWCApInfo.essid = %s\n", apinfo.essid );
			OS_TPrintf( "   DWCApInfo.bssid = %02x:%02x:%02x:%02x:%02x:%02x\n", apinfo.bssid[0], apinfo.bssid[1], apinfo.bssid[2], apinfo.bssid[3], apinfo.bssid[4], apinfo.bssid[5] );
		}
		else {
			OS_TPrintf( "   Failed to get DWCApInfo\n" );
		}
		return TRUE;
    }
	return FALSE;
}

/*-------------------------------------------------------------------------*
 * Name        : WaitVBlankIntr
 * Description : V�u�����N�҂��֐�
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void WaitVBlankIntr(void)
{
    int i;
    
    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
    loopcnt++;

    dbs_DemoUpdate();

    // �z�肷��Q�[���t���[���ɍ��킹��V�u�����N�҂��̉񐔂�ς���
    for ( i = 0; i < GAME_FRAME; i++ )
	{
        OS_WaitIrq( TRUE, OS_IE_V_BLANK );
    }

    Heap_Debug();
    dbs_DemoLoad();

    // �X�^�b�N���`�F�b�N
    OS_CheckStack( OS_GetCurrentThread() );
}

/*-------------------------------------------------------------------------*
 * Name        : VBlankIntr
 * Description : V�u�����N���荞�݊֐�
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void VBlankIntr( void )
{

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*-------------------------------------------------------------------------*
 * Name        : AllocFunc
 * Description : �������m�ۊ֐�
 * Arguments   : name  - �m�ۂ��閼�O
 *             : size  - �m�ۂ���T�C�Y
 *             : align - �m�ۂ���A���C�����g
 * Returns     : *void - �m�ۂ����|�C���^
 *-------------------------------------------------------------------------*/
static void* AllocFunc( DWCAllocType name, u32 size, int align )
{
	#pragma unused( name, align )

    OSIntrMode  old;
	void       *ptr;

    old = OS_DisableInterrupts();

    ptr = OS_AllocFromMain( size );
    OS_RestoreInterrupts( old );
	
    return ptr;
}

/*-------------------------------------------------------------------------*
 * Name        : FreeFunc
 * Description : �������J���֐�
 * Arguments   : name - �J�����郁������
 *             : *ptr - ������郁�����̃|�C���^
 *             : size - �������T�C�Y
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void FreeFunc( DWCAllocType name, void* ptr, u32 size  )
{
	#pragma unused( name, size )
    OSIntrMode old;

    if ( !ptr ) return;

    old = OS_DisableInterrupts();

	OS_FreeToMain( ptr );
    OS_RestoreInterrupts( old );
}

/*---------------------------------------------------------------------------*
 * Name        : KeyRead
 * Description : �L�[���͏���ҏW����B
 * Arguments   : None.
 * Returns     : None.
 *---------------------------------------------------------------------------*/
static void KeyRead( void )
{
	int			i;
	u16			r;

	// �p�b�h�����擾
	r = PAD_Read();

	// ���������N���A
	keyData.trg = 0;

	// �S�L�[���`�F�b�N
	for( i = 0 ; i < 12 ; i ++ )
	{
		if( r & ( 0x0001 << i ) )
		{
			// �����g���K�̎擾
			if( !( keyData.cnt & ( 0x0001 << i ) ) )
			{
				keyData.trg |= ( 0x0001 << i );
			}
		}
	}
	// �����H�L�[���͂���
	keyData.cnt = r;
}

/*-------------------------------------------------------------------------*
 * Name        : OS_TPrintf
 * Description : OS_TPrintf()�̃R���\�[���o�͑Ή�
 * Arguments   : *fmt - �\��������
 *             : ...  - �t�H�[�}�b�g
 * Returns     : None.
 *-------------------------------------------------------------------------*/
#ifdef SDK_FINALROM
#undef OS_TVPrintf
#undef OS_TPrintf
#endif
extern void OS_TVPrintf( const char *fmt, va_list vlist );

void OS_TVPrintf( const char *fmt, va_list vlist )
{
    (void)OS_VSNPrintf( printBuffer, sizeof(printBuffer), fmt, vlist );
#ifndef SDK_FINALROM
    OS_PutString( printBuffer );
#endif

    if ( printOverride )
    {
        dbs_CVPrintf( NULL, fmt, vlist );
    }
}
#ifdef SDK_FINALROM
extern void OS_TPrintf( const char *fmt, ... );
void OS_TPrintf( const char *fmt, ... )
{
    va_list vlist;

    va_start   ( vlist, fmt );
    OS_TVPrintf( fmt, vlist );
    va_end     ( vlist );
}
#endif


