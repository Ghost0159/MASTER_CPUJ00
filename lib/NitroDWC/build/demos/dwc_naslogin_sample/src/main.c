#include <nitro.h>
#include <dwc.h>

#include "main.h"
#include "init.h"

#include "dbs.h"

//----------------------------------------------------------------------------
// define
//----------------------------------------------------------------------------
#define GAME_FRAME       1         // 想定するゲームフレーム（1/60を1とする）
#define NETCONFIG_USE_HEAP 1

//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static DWCInetControl stConnCtrl;

static BOOL sPrintOverride; // デバッグ出力のオーバーライド

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
  メモリ確保関数
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
  メモリ開放関数
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
  OS_TPrintf()のコンソール出力対応
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
  メインルーチン
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

    // スタック溢れチェック初期設定
    OS_SetThreadStackWarningOffset(OS_GetCurrentThread(), 0x100);
    
    PXI_Init();
    
    Heap_Init();
	Alarm_Init();

    GX_Init();
    FS_Init( MI_DMA_MAX_NUM ); 

    GX_DispOff();
    GXS_DispOff();

    // Vブランク割り込み許可
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
    (void)GX_VBlankIntr(TRUE);
	GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    // 出力初期化
    dbs_DemoInit();
    sPrintOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.

    // 表示開始
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // デバッグ用にコンソールの情報を削除する。
    //DWC_Debug_DWCInitError( s_Work, DWC_INIT_RESULT_DESTROY_OTHER_SETTING );

    // デバッグ表示レベル指定
    DWC_SetReportLevel((unsigned long)(DWC_REPORTFLAG_ALL & ~DWC_REPORTFLAG_QR2_REQ));

    // DWCライブラリ初期化
    ret = DWC_Init( s_Work );
    OS_TPrintf( "DWC_Init() result = %d\n", ret );

    if ( ret == DWC_INIT_RESULT_DESTROY_OTHER_SETTING )
    {
        OS_TPrintf( "Wi-Fi setting might be broken.\n" );
    }

    // ヒープ使用量表示ON
    //Heap_SetDebug(TRUE);

    // メモリ確保関数設定
    DWC_SetMemFunc( AllocFunc, FreeFunc );
        
	NetConfigMain(); // まずWiFiコネクション設定GUIを起動する
    // メインループ
    while (1){
	  StartIPMain();
    }

	OS_Terminate();
}

/*---------------------------------------------------------------------------*
  設定GUI
 *---------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static void NetConfigMain(void)
{
  //GameMode returnSeq = GAME_MODE_MAIN;

    (void)FS_LoadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    sPrintOverride = FALSE; // OS_TPrintf()の出力を一時的に元に戻す。
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
    sPrintOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.
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
    // 安定なステートまで待つ。
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        // Vブランク待ち処理
        GameWaitVBlankIntr();
    }

    switch ( DWC_GetInetStatus() )
    {
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // エラー表示
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
            // エラー表示
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
  NASLogin取得メインルーチン
 *---------------------------------------------------------------------------*/
static void NasLoginMain(void)
{
	DWCNasLoginState		nasloginstate;
	DWCError				dwcerror;
	int						errorcode;
	DWCErrorType			dwcerrortype;
	
	// カスタムNASを使用
	DWC_SetAuthServer(DWC_CONNECTINET_AUTH_TEST);
	
	Heap_Print();
	
	// AボタンでNASLogin開始
	OS_TPrintf("**********************************************************\n");
	OS_TPrintf("Press A to check NASLogin function.\n");
	OS_TPrintf("While processing, you can abort NASLogin by pressing A.\n");
	OS_TPrintf("**********************************************************\n");

	while(1) {
		// Vブランク待ち処理
		GameWaitVBlankIntr();
		
		if(PAD_Read() & PAD_BUTTON_A)
			break;
	}
	while(1) {
		// Vブランク待ち処理
		GameWaitVBlankIntr();
		
		if(!(PAD_Read() & PAD_BUTTON_A))
			break;
	}

	// NASLogin開始
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
		// Aボタンでアボート
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
  Vブランク待ち関数
 *---------------------------------------------------------------------------*/
static void GameWaitVBlankIntr(void)
{
    int i;

    dbs_DemoUpdate();

    // 想定するゲームフレームに合わせてVブランク待ちの回数を変える
    for (i = 0; i < GAME_FRAME; i++){
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
    }

    Heap_Debug();
    dbs_DemoLoad();

    // スタック溢れチェック
    OS_CheckStack(OS_GetCurrentThread());
}


/*---------------------------------------------------------------------------*
  Vブランク割り込み関数
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
