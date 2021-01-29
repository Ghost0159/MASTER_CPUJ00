#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"

#include "dbs.h"

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#undef	CONNECT_TYPE_SPECIFY_AP_TYPE	// �ڑ���^�C�v���w�肷�鎩���ڑ�
#undef	CONNECT_TYPE_SPECIFY_AP			// �ڑ����SSID���w�肷�鎩���ڑ�

#define GAME_FRAME       1         // �z�肷��Q�[���t���[���i1/60��1�Ƃ���j
#define NETCONFIG_USE_HEAP 1
//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static KeyControl stKeyCnt;    // �L�[���͐���\����

static DWCInetControl stConnCtrl;

static BOOL sPrintOverride; // �f�o�b�O�o�͂̃I�[�o�[���C�h

static u16 card_lock_id;
//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void NetConfigMain(void);
static BOOL StartIPMain(void);
static void GameWaitVBlankIntr(void);
static void VBlankIntr(void);
//----------------------------------------------------------------------------
// initialized variable
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  �������m�ۊ֐�
 *---------------------------------------------------------------------------*/
static void*
AllocFunc( DWCAllocType name, u32   size, int align )
{
    void * ptr;
    OSIntrMode old;
    (void)name;
    (void)align;

    old = OS_DisableInterrupts();

    ptr = OS_AllocFromMain( size );

    OS_RestoreInterrupts( old );

#if 0
    OS_TPrintf("##ALLOC:%d:%p\n",size, ptr);

    if ( (u32) ptr == 0x0210f160 )
    {
        OS_TPrintf("#########################leak!\n");
    }
#endif

    return ptr;
}

/*---------------------------------------------------------------------------*
  �������J���֐�
 *---------------------------------------------------------------------------*/
static void
FreeFunc( DWCAllocType name, void* ptr,  u32 size  )
{
    OSIntrMode old;
    (void)name;
    (void)size;

    if ( !ptr ) return;

    old = OS_DisableInterrupts();

    OS_FreeToMain( ptr );

    OS_RestoreInterrupts( old );
}

/** --------------------------------------------------------------------
  OS_TPrintf()�̃R���\�[���o�͑Ή�
  ----------------------------------------------------------------------*/
#ifdef SDK_FINALROM
#undef OS_TVPrintf
#undef OS_TPrintf
#endif
extern void OS_TVPrintf(const char *fmt, va_list vlist );
static char common_buffer[256];        // thread unsafe, but less use of stack

void OS_TVPrintf(const char *fmt, va_list vlist )
{
    (void)OS_VSNPrintf(common_buffer, sizeof(common_buffer), fmt, vlist);
#ifndef SDK_FINALROM
    OS_PutString(common_buffer);
#endif

    if ( sPrintOverride )
    {
        dbs_CVPrintf( NULL, fmt, vlist );
    }
}
#ifdef SDK_FINALROM
extern void OS_TPrintf(const char *fmt, ...);
void OS_TPrintf(const char *fmt, ...)
{
    va_list vlist;

    va_start(vlist, fmt);
    OS_TVPrintf(fmt, vlist);
    va_end(vlist);
}
#endif

/*---------------------------------------------------------------------------*
  ���[�UID�\��
 *---------------------------------------------------------------------------*/
static void PrintAuthenticatedUserId( void )
{
    u64 authenticatedUserId;
    
    authenticatedUserId = DWC_GetAuthenticatedUserId();
    OS_TPrintf( "Authenticated User ID : ");
    if( authenticatedUserId ){
        OS_TPrintf( "%013llu\n", authenticatedUserId );
    }else{
        OS_TPrintf( "-------------\n" );
    }
}

/*---------------------------------------------------------------------------*
  ���C�����[�`��
 *---------------------------------------------------------------------------*/
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
void NitroMain ()
{
    int ret;

	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();
    CARD_Init();
    ret = OS_GetLockID();
    if (ret == OS_LOCK_ID_ERROR)
    {
        OS_Panic("demo fatal error! OS_GetLockID() failed");
    }
    card_lock_id = (u16)ret;

    // �X�^�b�N���`�F�b�N�����ݒ�
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    
    PXI_Init();
    
    Heap_Init();
	Alarm_Init();

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
    sPrintOverride = TRUE; // OS_TPrintf()�̏o�͂�console�ɂȂ�.

    // �\���J�n
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // �L�[���͏�����
    InitKeyData(&stKeyCnt);

    //InitGameData();

    // �f�o�b�O�p�ɃR���\�[���̏����폜����B
    //DWC_Debug_DWCInitError( s_Work, DWC_INIT_RESULT_DESTROY_OTHER_SETTING );

    // �f�o�b�O�\�����x���w��
    DWC_SetReportLevel((unsigned long)(DWC_REPORTFLAG_ALL & ~DWC_REPORTFLAG_QR2_REQ));

    // DWC���C�u����������
    ret = DWC_Init( s_Work );
    OS_TPrintf( "DWC_Init() result = %d\n", ret );

    if ( ret == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        OS_TPrintf( "Wi-Fi setting might be broken.\n" );
    }
    
    // �F�؍ς݃��[�UID��\��
    // �Y���̖{�̂̍ŏ���DWC_Init()�̌Ăяo���ŉ��̃��[�UID���쐬�����
    PrintAuthenticatedUserId();

    // �q�[�v�g�p�ʕ\��ON
    //Heap_SetDebug(TRUE);

    // �������m�ۊ֐��ݒ�
    DWC_SetMemFunc( AllocFunc, FreeFunc );
	
	// �܂�WiFi�R�l�N�V�����ݒ�GUI���N������(�ݒ��ʂ𔲂���܂Ő���͕Ԃ���Ȃ�)
	NetConfigMain();
    
    // �F�؍ς݃��[�UID��\��
    // �Y���̖{�̂̍ŏ��̃C���^�[�l�b�g�ڑ����ڑ��e�X�g�ł����[�UID�͔F�؂����
    PrintAuthenticatedUserId(); // 

	// �C���^�[�l�b�g�ڑ��������[�v�̗�(��������)
    while (1){
	  if(StartIPMain() == TRUE)
			break;
    }
	OS_TPrintf("An internet connection is established.\n\n");
	// �C���^�[�l�b�g�ڑ��������[�v�̗�(�����܂�)
	
	// �C���^�[�l�b�g�ڑ��m�F�������[�v�̗�(��������)
	OS_TPrintf("An internet connection check by communicating HTTP server.\n");
	
	// �C���^�[�l�b�g�ڑ��m�F�����J�n
	if(DWC_TestInetAsync(0) == FALSE)
		OS_Panic("DWC_TestInetAsync() failed.\n\n");

	OS_TPrintf("DWC_TestInetAsync() successed.\n");
    
    // �F�؍ς݃��[�UID��\��
    // �Y���̖{�̂̍ŏ��̃C���^�[�l�b�g�ڑ��������Ƀ��[�UID�͔F�؂����
    PrintAuthenticatedUserId();

	OS_TPrintf("Processing by calling DWC_TestInetProcess().\n");
	while(1) {
		DWCTestInetResult result;
		OS_Sleep(100);
		DWC_UpdateConnection();
		result = DWC_TestInetProcess();
		
		// ���������G���[
		if(result == DWC_TESTINET_NOT_INITIALIZED)
			OS_Panic("DWC_GetInetStatusByHttpAsync() is not called.\n");
			
		if(result == DWC_TESTINET_OPERATING)
			continue;
		else if(result == DWC_TESTINET_CONNECTED) {
			// �C���^�[�l�b�g�ڑ�����Ă��邱�Ƃ��m�F���ꂽ
			OS_TPrintf("HTTP server returned a responce. An internet connection is available.\n\n");
			break;
		}
		else if(result == DWC_TESTINET_DISCONNECTED) {
			// �C���^�[�l�b�g�ڑ�����Ă��Ȃ����Ƃ��m�F���ꂽ
			OS_TPrintf("HTTP server didn't return a responce. An internet connection is unavailable.\n\n");
			break;
		}
	}
	// �C���^�[�l�b�g�ڑ��m�F�������[�v�̗�(�����܂�)
	
	// DWC_UpdateConnection�֐����g�p����ꍇ�̗�(��������)
	OS_TPrintf("Turn off the Wi-Fi AP power and DWC library will detect the DCF disconnection.\n");
	while (1){
		if(DWC_UpdateConnection()) {
			DWCError dwcerror;
			int errorcode;
			DWCErrorType errortype;
		
			dwcerror = DWC_GetLastErrorEx(&errorcode, &errortype);
			if(dwcerror != DWC_ERROR_NONE) {
				OS_TPrintf("A DCF disconnection is detected by DWC_UpdateConnection()\n");
				OS_TPrintf("DWCError = %d, errorcode = %d, DWCErrorType = %d\n", dwcerror, errorcode, errortype);
				break;
			}
		}
	}
	// DWC_UpdateConnection�֐����g�p����ꍇ�̗�(�����܂�)

	DWC_CleanupInet();

	OS_TPrintf("Turn off the NDS power.\n");
	OS_Terminate();
}

/*---------------------------------------------------------------------------*
  �ݒ�GUI
 *---------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static void NetConfigMain(void)
{
  //GameMode returnSeq = GAME_MODE_MAIN;

    (void)FS_LoadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    sPrintOverride = FALSE; // OS_TPrintf()�̏o�͂��ꎞ�I�Ɍ��ɖ߂��B
    dbs_DemoFinalize();

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

#if defined( NETCONFIG_USE_HEAP )
    {
        void* work = OS_Alloc(DWC_UTILITY_WORK_SIZE);

        (void)DWC_StartUtility(work, DWC_LANGUAGE_JAPANESE, DWC_UTILITY_TOP_MENU_FOR_JPN);

        OS_Free(work);
    }
#else
    (void)DWC_StartUtilityEx(DWC_LANGUAGE_JAPANESE, 0);
#endif

    (void)FS_UnloadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)GX_VBlankIntr( TRUE );

    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()�̏o�͂�console�ɂȂ�.
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    //return returnSeq;
}
/*---------------------------------------------------------------------------*

 *---------------------------------------------------------------------------*/
static BOOL StartIPMain(void)
{
	DWCApInfo apinfo;
	
    DWC_InitInet( &stConnCtrl );

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

	/*-----------------------------------------------------------------------*
	 * ���ʏ�̎����ڑ��̏ꍇ��
	 * �@DWC_ConnectInetAsync�݂̂��Ăяo���܂��B
	 * �@�{�T���v���̏ꍇ�ACONNECT_TYPE_SPECIFY_AP��CONNECT_TYPE_SPECIFY_AP_TYPE��undef���Ă�������
	 * ���ڑ���AP�̃^�C�v���w�肵�Đڑ��������ꍇ��
	 * �@DWC_SetConnectApType�֐ڑ���AP�̃^�C�v�������Ƃ��ČĂяo������
	 * �@DWC_ConnectInetAsync���Ăяo���܂��B
	 * �@��������FINALROM�ł͍폜����܂��B
	 * �@�{�T���v���̏ꍇ�ACONNECT_TYPE_SPECIFY_AP_TYPE��define���Ă�������
	 * ���ڑ����AP���w�肵�Đڑ��������ꍇ��
	 * �@DWC_DebugConnectInetAsync�֐ڑ���AP��SSID��WEP�L�[�������Ƃ���
	 * �@�Ăяo���܂��B
	 * �@�{�T���v���̏ꍇ�ACONNECT_TYPE_SPECIFY_AP��define���Ă�������
	 *-----------------------------------------------------------------------*/
#ifndef	CONNECT_TYPE_SPECIFY_AP
#ifdef	CONNECT_TYPE_SPECIFY_AP_TYPE
	DWC_SetConnectApType( DWC_APINFO_TYPE_USER0 );	// ���[�U�[�ݒ�P���w��
#endif 	// CONNECT_TYPE_SPECIFY_AP_TYPE
	DWC_ConnectInetAsync();
#else
	DWC_DebugConnectInetAsync( "XXXXX", NULL, WCM_WEPMODE_NONE );	// ������ύX���Ďg�p���Ă�������
#endif 	// CONNECT_TYPE_SPECIFY_AP

	// ����ȃX�e�[�g�܂ő҂B
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        // V�u�����N�҂�����
        GameWaitVBlankIntr();
    }

    switch ( DWC_GetInetStatus() )
    {
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_TPrintf("   Error occurred %d %d.\n", err, errcode );
        }
        DWC_ClearError();

        DWC_CleanupInet();
        break;
    case DWC_CONNECTINET_STATE_FATAL_ERROR:
        {
            // �G���[�\��
            DWCError err;
            int errcode;
            err = DWC_GetLastError( &errcode );

            OS_Panic("   Error occurred %d %d.\n", err, errcode );
        }
        break;

    case DWC_CONNECTINET_STATE_CONNECTED:
		OS_TPrintf("   CONNECTED!!!.\n");
		if(DWC_GetApInfo(&apinfo) == TRUE) {
			OS_TPrintf("   DWCApInfo.aptype = %d\n", apinfo.aptype);
			OS_TPrintf("   DWCApInfo.area   = %d\n", apinfo.area);
			OS_TPrintf("   DWCApInfo.spotinfo = %s\n", apinfo.spotinfo);
			OS_TPrintf("   DWCApInfo.essid = %s\n", apinfo.essid);
			OS_TPrintf("   DWCApInfo.bssid = %02x:%02x:%02x:%02x:%02x:%02x\n", apinfo.bssid[0], apinfo.bssid[1], apinfo.bssid[2], apinfo.bssid[3], apinfo.bssid[4], apinfo.bssid[5]);
		}
		else {
			OS_TPrintf("   Failed to get DWCApInfo\n");
		}
		return TRUE;
    }
	return FALSE;
}


/*---------------------------------------------------------------------------*
  V�u�����N�҂��֐�
 *---------------------------------------------------------------------------*/
static void GameWaitVBlankIntr(void)
{
    int i;

    dbs_DemoUpdate();

    // �z�肷��Q�[���t���[���ɍ��킹��V�u�����N�҂��̉񐔂�ς���
    for (i = 0; i < GAME_FRAME; i++){
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    }

    Heap_Debug();
    dbs_DemoLoad();

    // �X�^�b�N���`�F�b�N
    OS_CheckStack(OS_GetCurrentThread());
}


/*---------------------------------------------------------------------------*
  V�u�����N���荞�݊֐�
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
