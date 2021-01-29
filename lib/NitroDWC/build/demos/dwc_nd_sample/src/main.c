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
#define GAME_FRAME				1		// 想定するゲームフレーム（1/60を1とする）
#define NETCONFIG_USE_HEAP		1
//#define USE_AUTHSERVER_RELEASE			// 本番サーバへ接続

// -- ND に関連した定数 --
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
static BOOL           printOverride;				// デバッグ出力のオーバーライド
static u8             dwcWork[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );
static char           printBuffer[ 256 ];			// thread unsafe, but less use of stack
static KeyInfo        keyData;							// キー入力

static u32 loopcnt = 0;
static char loopstr[] = "/|\\-";

// -- ND に関連した変数 --
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

// -- ND に関連したプロトタイプ --
static int  NdMain           ( void );
static void WaitNdCallback   ( void );
static void NdSetAttribute   ( void );
static int  SelectFile       ( int num );
static void NdCallback       ( DWCNdCallbackReason reason, DWCNdError error, int servererror );
static void NdCleanupCallback( void );

/* ========================================================================= *
 * 関連する関数群
 * ========================================================================= */
/*-------------------------------------------------------------------------*
 * Name        : NitroMain
 * Description : メインルーチン
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

    // スタック溢れチェック初期設定
    OS_SetThreadStackWarningOffset( OS_GetCurrentThread(), 0x100 );
    Heap_Init();

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
    printOverride = TRUE; // OS_TPrintf()の出力をconsoleにつなぐ.

    // 表示開始
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // デバッグ用にコンソールの情報を削除する。
    //DWC_Debug_DWCInitError( dwcWork, DWC_INIT_RESULT_DESTROY_OTHER_SETTING );

    // デバッグ表示レベル指定
    DWC_SetReportLevel((unsigned long)(DWC_REPORTFLAG_ALL & ~DWC_REPORTFLAG_QR2_REQ));

    // DWCライブラリ初期化
    ret = DWC_Init( dwcWork );
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
    
	// インターネット接続ループ
	while( 1 )
	{
		if( StartIPMain() == TRUE ) break;
		WaitVBlankIntr();
    }
	
	// 実際のプログラムでは、ダウンロードライブラリを使用する前に必ず認証サーバでの認証が成功していることを確認してください
	while( 1 )
	{
		//OS_DumpHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE);
		
		result = NdMain();
		
		// 実際のプログラムでは、エラーが発生した場合にはDWC_GetLastErrorEx関数で取得する対処法に従ってください
		
		// 解放処理
		gdlcallback2 = FALSE;
		if(DWC_NdCleanupAsync( NdCleanupCallback ))
		{
			while( gdlcallback2 == FALSE )
				WaitVBlankIntr();
		}
		
		//OS_DumpHeap(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE);
		
		// エラーが発生していた場合は処理を止める
		if(result == ND_RESULT_ERROR)
			OS_Panic("Nd library returned an error.\n");

		WaitVBlankIntr();
	}
	OS_Terminate();
}

/*-------------------------------------------------------------------------*
 * Name        : NdMain
 * Description : NDのメイン処理
 * Arguments   : None.
 * Returns     : int - 終了結果
 *-------------------------------------------------------------------------*/
static int NdMain( void )
{
	int no,entrynum;

	// ND Start
	OS_TPrintf("--------------------------------\n");
	OS_TPrintf("DWC_ND: start - download -\n");
	OS_TPrintf("--------------------------------\n");
	WaitVBlankIntr();

	// -- Ndライブラリ初期化 --------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdInitAsync( NdCallback, "NTRJ", "1234123412341234" ) == FALSE )
		return ND_RESULT_ERROR;

	WaitNdCallback();
	
	if(nderror == TRUE)
		return ND_RESULT_ERROR;

	// -- アトリビュート設定（指定無し、全てのファイルを取得する）-------------
	NdSetAttribute();

	// -- ファイル数取得 ------------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdGetFileListNumAsync( &entrynum ) == FALSE )
		return ND_RESULT_ERROR;
	
	WaitNdCallback();
	
	if(nderror == TRUE)
		return ND_RESULT_ERROR;
	OS_TPrintf( "Get FileNum = %d\n", entrynum );

	// -- ファイルリスト取得 --------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdGetFileListAsync( fileInfo, 0, FILE_NUM ) == FALSE )
		return ND_RESULT_ERROR;

	WaitNdCallback();
	
	if(nderror == TRUE)
		return ND_RESULT_ERROR;

	// 取得したファイル数が０の場合、終了する
	if( entrynum == 0 )
	{
		// ダウンロードできるファイルがなかった
		return ND_RESULT_NOT_FOUND_FILES;
	}

	// -- 取得するファイルを選択 ----------------------------------------------
	no = SelectFile( entrynum );

	// -- ファイル取得 --------------------------------------------------------
	gdlcallback = nderror = FALSE;
	if( DWC_NdGetFileAsync( &fileInfo[ no ], buffer, sizeof( buffer )) == FALSE )
		return ND_RESULT_ERROR;

	// ファイル取得終了待ち
	while( gdlcallback == FALSE )
	{
		u32 recived,contentlen,percent;
		// キャンセル処理
		KeyRead();
		if( KEY_TRIGGER( PAD_BUTTON_B ))
		{
			if( DWC_NdCancelAsync() == FALSE )
			{
				OS_Panic( "DWC_NdCancelAsync: Failed.\n" );
			}
			// ダウンロードキャンセル
			WaitNdCallback();
			return 	ND_RESULT_DOWNLOAD_CANCEL;
		}
		// 進行度チェック
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

	// ダウンロード終了
	return 	ND_RESULT_COMPLETE;
}

/*-------------------------------------------------------------------------*
 * Name        : WaitNdCallback
 * Description : ND用コールバック待ち処理
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
 * Description : ND用　ダウンロードするファイル属性を設定する
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
		// キー入力処理
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
		// 表示の更新
		if( KEY_TRIGGER( PAD_KEY_UP|PAD_KEY_DOWN ))
		{
			NdSetAttributeInfo( attr1, attr2, attr3 );
		}
        // Vブランク待ち処理
        WaitVBlankIntr();
	}

	// アトリビュートのセット
	if( DWC_NdSetAttr( attr1, attr2, attr3 ) == FALSE )
	{
		OS_Panic( "DWC_NdSetAttr: Failed\n." );
	}
}

/*-------------------------------------------------------------------------*
 * Name        : SelectFile
 * Description : ND用　ダウンロードするファイルを決める
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
		// キー入力処理
		KeyRead();
		if( KEY_TRIGGER( PAD_BUTTON_A )) break;
		if( KEY_TRIGGER( PAD_KEY_UP ))        no++;
		else if( KEY_TRIGGER( PAD_KEY_DOWN )) no--;
		if( no < 0 )         no = num - 1;
		else if( no >= num ) no = 0;
		// 表示の更新
		if( KEY_TRIGGER( PAD_KEY_UP|PAD_KEY_DOWN ))
		{
			SelectFileInfo( no, num );
		}
        // Vブランク待ち処理
        WaitVBlankIntr();
	}
	return no;
}

/*-------------------------------------------------------------------------*
 * Name        : NdCallback
 * Description : ND用コールバック
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
	
	// 実際のプログラムでは適切なエラー処理を行ってください
	// また、その際にはコールバック内からDWC_NdCleanupAsync()を呼び出せないことに注意してください
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
 * Description : DWC_NdCleanupAsync用コールバック
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
 * 非関連関数群
 * ========================================================================= */
/*-------------------------------------------------------------------------*
 * Name        : NetConfigMain
 * Description : 設定GUI
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
FS_EXTERN_OVERLAY(main_overlay_1);

static void NetConfigMain( void )
{
  //GameMode returnSeq = GAME_MODE_MAIN;

    (void)FS_LoadOverlay( MI_PROCESSOR_ARM9, FS_OVERLAY_ID(main_overlay_1) ) ;

    printOverride = FALSE; // OS_TPrintf()の出力を一時的に元に戻す。
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
    printOverride = TRUE;			// OS_TPrintf()の出力をconsoleにつなぐ.
    OS_WaitVBlankIntr();			// Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    //return returnSeq;
}

/*-------------------------------------------------------------------------*
 * Name        : StartIPMain
 * Description : インターネットへの接続処理
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

	// 接続開始
	DWC_ConnectInetAsync();

	// 安定なステートまで待つ。
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        // Vブランク待ち処理
        WaitVBlankIntr();
    }

	// 接続処理完了、結果を確認
    switch ( DWC_GetInetStatus() )
    {
	// 接続失敗
    case DWC_CONNECTINET_STATE_ERROR:
        {
            // エラー表示
            DWCError err;
            int  errcode;
            err = DWC_GetLastError( &errcode );
            OS_TPrintf( "   Error occurred %d %d.\n", err, errcode );
        }
        DWC_ClearError();
        DWC_CleanupInet();
        break;
	// 接続失敗
    case DWC_CONNECTINET_STATE_FATAL_ERROR:
        {
            // エラー表示
            DWCError err;
            int  errcode;
            err = DWC_GetLastError( &errcode );
            OS_Panic( "   Error occurred %d %d.\n", err, errcode );
        }
        break;
	// 接続成功
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
 * Description : Vブランク待ち関数
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void WaitVBlankIntr(void)
{
    int i;
    
    dbs_Print( 30, 0, "%c",   loopstr[((loopcnt)>>2)&3] );
    loopcnt++;

    dbs_DemoUpdate();

    // 想定するゲームフレームに合わせてVブランク待ちの回数を変える
    for ( i = 0; i < GAME_FRAME; i++ )
	{
        OS_WaitIrq( TRUE, OS_IE_V_BLANK );
    }

    Heap_Debug();
    dbs_DemoLoad();

    // スタック溢れチェック
    OS_CheckStack( OS_GetCurrentThread() );
}

/*-------------------------------------------------------------------------*
 * Name        : VBlankIntr
 * Description : Vブランク割り込み関数
 * Arguments   : None.
 * Returns     : None.
 *-------------------------------------------------------------------------*/
static void VBlankIntr( void )
{

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*-------------------------------------------------------------------------*
 * Name        : AllocFunc
 * Description : メモリ確保関数
 * Arguments   : name  - 確保する名前
 *             : size  - 確保するサイズ
 *             : align - 確保するアライメント
 * Returns     : *void - 確保したポインタ
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
 * Description : メモリ開放関数
 * Arguments   : name - 開放するメモリ名
 *             : *ptr - 解放するメモリのポインタ
 *             : size - 解放するサイズ
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
 * Description : キー入力情報を編集する。
 * Arguments   : None.
 * Returns     : None.
 *---------------------------------------------------------------------------*/
static void KeyRead( void )
{
	int			i;
	u16			r;

	// パッド情報を取得
	r = PAD_Read();

	// メモリをクリア
	keyData.trg = 0;

	// 全キーをチェック
	for( i = 0 ; i < 12 ; i ++ )
	{
		if( r & ( 0x0001 << i ) )
		{
			// 押しトリガの取得
			if( !( keyData.cnt & ( 0x0001 << i ) ) )
			{
				keyData.trg |= ( 0x0001 << i );
			}
		}
	}
	// 未加工キー入力を代入
	keyData.cnt = r;
}

/*-------------------------------------------------------------------------*
 * Name        : OS_TPrintf
 * Description : OS_TPrintf()のコンソール出力対応
 * Arguments   : *fmt - 表示文字列
 *             : ...  - フォーマット
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


