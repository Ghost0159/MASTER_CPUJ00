#include <nitro.h>
#include <dwc.h>

#include "main.h"
#include "init.h"

#include "dbs.h"

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_FRAME       1         // �z�肷��Q�[���t���[���i1/60��1�Ƃ���j
#define NETCONFIG_USE_HEAP 1

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static DWCInetControl stConnCtrl;

static BOOL sPrintOverride; // �f�o�b�O�o�͂̃I�[�o�[���C�h

//----------------------------------------------------------------------------
// prototype
//----------------------------------------------------------------------------
static void NetConfigMain(void);
static void StartIPMain(void);
static void GameWaitVBlankIntr(void);
static void VBlankIntr(void);

static void NasLoginMain(void);
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

    // �q�[�v�g�p�ʕ\��ON
    //Heap_SetDebug(TRUE);

    // �������m�ۊ֐��ݒ�
    DWC_SetMemFunc( AllocFunc, FreeFunc );
        
	NetConfigMain(); // �܂�WiFi�R�l�N�V�����ݒ�GUI���N������
    // ���C�����[�v
    while (1){
	  StartIPMain();
    }

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

    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_TEST);

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
static void StartIPMain(void)
{
    DWC_InitInet( &stConnCtrl );

#if defined( USE_AUTHSERVER_RELEASE )
    DWC_SetAuthServer(DWC_CONNECTINET_AUTH_RELEASE);
#endif

    DWC_ConnectInetAsync();
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
		NasLoginMain();
		break;
    }

    //return GAME_MODE_MAIN;
}

/*---------------------------------------------------------------------------*
  NASLogin�擾���C�����[�`��
 *---------------------------------------------------------------------------*/
static void NasLoginMain(void)
{
	DWCNasLoginState		nasloginstate;
	DWCError				dwcerror;
	int						errorcode;
	DWCErrorType			dwcerrortype;
	
	// �J�X�^��NAS���g�p
	DWC_SetAuthServer(DWC_CONNECTINET_AUTH_TEST);
	
	Heap_Print();
	
	// A�{�^����NASLogin�J�n
	OS_TPrintf("**********************************************************\n");
	OS_TPrintf("Press A to check NASLogin function.\n");
	OS_TPrintf("While processing, you can abort NASLogin by pressing A.\n");
	OS_TPrintf("**********************************************************\n");

	while(1) {
		// V�u�����N�҂�����
		GameWaitVBlankIntr();
		
		if(PAD_Read() & PAD_BUTTON_A)
			break;
	}
	while(1) {
		// V�u�����N�҂�����
		GameWaitVBlankIntr();
		
		if(!(PAD_Read() & PAD_BUTTON_A))
			break;
	}

	// NASLogin�J�n
	if(DWC_NASLoginAsync() == FALSE)
		OS_TPanic("DWC_NASLoginAsync failed");
	
	while(1) {
		GameWaitVBlankIntr();
		DWC_UpdateConnection();
		
		nasloginstate = DWC_NASLoginProcess();
		if(nasloginstate == DWC_NASLOGIN_STATE_SUCCESS) {
			OS_TPrintf("NAS Login Successed\n");
			break;
		}
		else if(nasloginstate == DWC_NASLOGIN_STATE_ERROR) {
			OS_TPrintf("NAS Login Failed\n");
			dwcerror = DWC_GetLastErrorEx(&errorcode, &dwcerrortype);
			OS_TPrintf("dwcerror = %d, errorcode = %d, dwcerrortype = %d\n", dwcerror, errorcode, dwcerrortype);
			break;
		}
		else if(nasloginstate == DWC_NASLOGIN_STATE_CANCELED) {
			OS_TPrintf("NAS Login canceled\n");
			break;
		}
		// A�{�^���ŃA�{�[�g
		if(PAD_Read() & PAD_BUTTON_A)
			DWC_NASLoginAbort();
	}
	
	OS_TPrintf("\n");
	OS_TPrintf("*************************\n");
	OS_TPrintf("Please turn off NDS power\n");
	OS_TPrintf("*************************\n");
	
	while(1)
		GameWaitVBlankIntr();
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
