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
#undef	CONNECT_TYPE_SPECIFY_AP_TYPE	// 接続先タイプを指定する自動接続
#undef	CONNECT_TYPE_SPECIFY_AP			// 接続先のSSIDを指定する自動接続

#define GAME_FRAME       1         // 想定するゲームフレーム（1/60を1とする）
#define NETCONFIG_USE_HEAP 1
//----------------------------------------------------------------------------
// typedef
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// variable
//----------------------------------------------------------------------------
static KeyControl stKeyCnt;    // キー入力制御構造体

static DWCInetControl stConnCtrl;

static BOOL sPrintOverride; // デバッグ出力のオーバーライド

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
  ユーザID表示
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
    ret = OS_GetLockID();
    if (ret == OS_LOCK_ID_ERROR)
    {
        OS_Panic("demo fatal error! OS_GetLockID() failed");
    }
    card_lock_id = (u16)ret;

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

    // キー入力初期化
    InitKeyData(&stKeyCnt);

    //InitGameData();

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
    
    // 認証済みユーザIDを表示
    // 該当の本体の最初のDWC_Init()の呼び出しで仮のユーザIDが作成される
    PrintAuthenticatedUserId();

    // ヒープ使用量表示ON
    //Heap_SetDebug(TRUE);

    // メモリ確保関数設定
    DWC_SetMemFunc( AllocFunc, FreeFunc );
	
	// まずWiFiコネクション設定GUIを起動する(設定画面を抜けるまで制御は返されない)
	NetConfigMain();
    
    // 認証済みユーザIDを表示
    // 該当の本体の最初のインターネット接続が接続テストでもユーザIDは認証される
    PrintAuthenticatedUserId(); // 

	// インターネット接続処理ループの例(ここから)
    while (1){
	  if(StartIPMain() == TRUE)
			break;
    }
	OS_TPrintf("An internet connection is established.\n\n");
	// インターネット接続処理ループの例(ここまで)
	
	// インターネット接続確認処理ループの例(ここから)
	OS_TPrintf("An internet connection check by communicating HTTP server.\n");
	
	// インターネット接続確認処理開始
	if(DWC_TestInetAsync(0) == FALSE)
		OS_Panic("DWC_TestInetAsync() failed.\n\n");

	OS_TPrintf("DWC_TestInetAsync() successed.\n");
    
    // 認証済みユーザIDを表示
    // 該当の本体の最初のインターネット接続処理中にユーザIDは認証される
    PrintAuthenticatedUserId();

	OS_TPrintf("Processing by calling DWC_TestInetProcess().\n");
	while(1) {
		DWCTestInetResult result;
		OS_Sleep(100);
		DWC_UpdateConnection();
		result = DWC_TestInetProcess();
		
		// 未初期化エラー
		if(result == DWC_TESTINET_NOT_INITIALIZED)
			OS_Panic("DWC_GetInetStatusByHttpAsync() is not called.\n");
			
		if(result == DWC_TESTINET_OPERATING)
			continue;
		else if(result == DWC_TESTINET_CONNECTED) {
			// インターネット接続されていることが確認された
			OS_TPrintf("HTTP server returned a responce. An internet connection is available.\n\n");
			break;
		}
		else if(result == DWC_TESTINET_DISCONNECTED) {
			// インターネット接続されていないことが確認された
			OS_TPrintf("HTTP server didn't return a responce. An internet connection is unavailable.\n\n");
			break;
		}
	}
	// インターネット接続確認処理ループの例(ここまで)
	
	// DWC_UpdateConnection関数を使用する場合の例(ここから)
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
	// DWC_UpdateConnection関数を使用する場合の例(ここまで)

	DWC_CleanupInet();

	OS_TPrintf("Turn off the NDS power.\n");
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
    sPrintOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.
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
	 * ■通常の自動接続の場合は
	 * 　DWC_ConnectInetAsyncのみを呼び出します。
	 * 　本サンプルの場合、CONNECT_TYPE_SPECIFY_APとCONNECT_TYPE_SPECIFY_AP_TYPEをundefしてください
	 * ■接続先APのタイプを指定して接続したい場合は
	 * 　DWC_SetConnectApTypeへ接続先APのタイプを引数として呼び出した後
	 * 　DWC_ConnectInetAsyncを呼び出します。
	 * 　※ただしFINALROMでは削除されます。
	 * 　本サンプルの場合、CONNECT_TYPE_SPECIFY_AP_TYPEをdefineしてください
	 * ■接続先のAPを指定して接続したい場合は
	 * 　DWC_DebugConnectInetAsyncへ接続先APのSSIDとWEPキーを引数として
	 * 　呼び出します。
	 * 　本サンプルの場合、CONNECT_TYPE_SPECIFY_APをdefineしてください
	 *-----------------------------------------------------------------------*/
#ifndef	CONNECT_TYPE_SPECIFY_AP
#ifdef	CONNECT_TYPE_SPECIFY_AP_TYPE
	DWC_SetConnectApType( DWC_APINFO_TYPE_USER0 );	// ユーザー設定１を指定
#endif 	// CONNECT_TYPE_SPECIFY_AP_TYPE
	DWC_ConnectInetAsync();
#else
	DWC_DebugConnectInetAsync( "XXXXX", NULL, WCM_WEPMODE_NONE );	// 引数を変更して使用してください
#endif 	// CONNECT_TYPE_SPECIFY_AP

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
