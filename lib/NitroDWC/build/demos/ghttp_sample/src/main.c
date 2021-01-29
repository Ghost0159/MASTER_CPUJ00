/*===========================================================================*
 *                                                                           *
 *  ghttpライブラリ　サンプルプログラム                                      *
 *                                                                           *
 *                                           Copyright 2005 Nintendo Co.,Ltd *
 *===========================================================================*/
#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

// ヘッダファイルインクルード
//-------------------------------------
#include "main.h"
#include "init.h"
#include "utility.h"

// 定数宣言
//---------------------------------------------------------
#define GAMENAME "dwctest"		//! ゲーム名
#define USERID "12345"   		//! ユーザーID
#define RECV_SIZE 256           //! 受信データサイズ

// グローバル変数宣言
//---------------------------------------------------------
DWCInetControl stConnCtrl;//! 
int		g_pending;		  //! 通信完了待ちか否か
char*   buffer = NULL;           //! 

// 送受信データ形式
//---------------------------------------------------------
typedef struct _tagGHTTPDATA{
	u8	tex[128];	//! テキスト
	u8	title[10];	//! タイトル
}GHTTPDATA;


// 関数プロトタイプ
//---------------------------------------------------------
static void POSTCallback(const char*, int, DWCGHTTPResult result, void*);
static void GETCallback(const char* buf, int buflen, DWCGHTTPResult result, void* param);
static void GETCallbackEx(const char* buf, int buflen, DWCGHTTPResult result, void* param);
static void GETProgressCallbackEx(DWCGHTTPState state, const char* buf, int len, int bytesReceived, int totalSize, void* param);
static BOOL StartIPMain(void);
static void VBlankIntr(void);

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

//=============================================================================
/*!
 *	@brief	データ送信完了時のコールバック
 *
 *	@param	buf		[in] データバッファ
 *	@param	buflen	[in] bufのサイズ
 *
 *	@retval	TRUE	buf開放
 *	@retval	FALSE   buf開放せず
*/
//=============================================================================
static void POSTCallback(const char* buf, int buflen, DWCGHTTPResult result, void* param){
#ifdef SDK_FINALROM
#pragma unused(result)
#endif
#pragma unused(buf)
#pragma unused(buflen)
#pragma unused(param)

	g_pending = 0;

    OS_Printf("Post Result : %d\n", result);

}

//=============================================================================
/*!
 *	@brief	データ受信完了時のコールバック
 *
 *	@param	buf		[in] データバッファ
 *	@param	buflen	[in] bufのサイズ
 *  @param  param   [in] コールバック用パラメータ
 *
 *	@retval	TRUE	buf開放
 *	@retval	FALSE   buf開放せず
 */
//=============================================================================
static void GETCallback(const char* buf, int buflen, DWCGHTTPResult result, void* param){
#ifdef SDK_FINALROM
#pragma unused(result)
#endif
    char*	g_data;	//! 受信データのバッファ

    g_data = param;
    
	if(buf){
        MI_CpuCopy8(buf, g_data, (u32)buflen);
	}

	g_pending = 0;

    OS_Printf("Recv Result : %d\n", result);

}

//=============================================================================
/*!
 *	@brief	データ受信完了時のコールバック(ユーザ受信バッファ指定)
 *
 *	@param	buf		[in] データバッファ
 *	@param	buflen	[in] bufのサイズ
 *  @param  param   [in] コールバック用パラメータ
 *
 *	@retval	TRUE	buffer開放
 *	@retval	FALSE   buffer開放せず
 */
//=============================================================================
static void GETCallbackEx(const char* buf, int buflen, DWCGHTTPResult result, void* param){
#ifdef SDK_FINALROM
#pragma unused(result)
#pragma unused(buflen)
#endif
#pragma unused(param)

    if(buf){
        buffer = (char*)buf;
    }
    
    OS_Printf("Recv Result : %d\n", result);
    OS_Printf("buflen : %d\n", buflen);
    
	g_pending = 0;

}

//=============================================================================
/*!
 *	@brief	データ受信進行時のコールバック
 *
 *	@param	buf		[in] データバッファ
 *	@param	buflen	[in] bufのサイズ
 *  @param  param   [in] コールバック用パラメータ
 *
 *	@retval	TRUE	buffer開放
 *	@retval	FALSE   buffer開放せず
 */
//=============================================================================
static void GETProgressCallbackEx(DWCGHTTPState state, const char* buf, int len, int bytesReceived, int totalSize, void* param){
#ifdef SDK_FINALROM
#pragma unused(state)
#pragma unused(len)
#pragma unused(bytesReceived)
#pragma unused(totalSize)
#endif
#pragma unused(buf)
#pragma unused(param)

    OS_Printf("state: %d\n", state);
    OS_Printf("size : %d/%d\n", len, totalSize);
    OS_Printf("totalsize : %d/%d\n", bytesReceived, totalSize);
    
}

//=============================================================================
/*!
 *	@brief	IPの取得
 *
 *	@retval	TRUE		成功
 *	@retval	FALSE   	失敗
 */
//=============================================================================
static BOOL StartIPMain(void)
{
    DWC_InitInet( &stConnCtrl );

    DWC_ConnectInetAsync();
    // 安定なステートまで待つ。
    while ( !DWC_CheckInet() )
    {
        DWC_ProcessInet();

        OS_WaitVBlankIntr();

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
        return TRUE;
    }
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Vブランク割り込み関数
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

//=============================================================================
/*!
 *	@brief	メイン
 */
//=============================================================================
static u8 s_Work[ DWC_INIT_WORK_SIZE ] ATTRIBUTE_ALIGN( 32 );

void NitroMain()
{
	// データ用
	GHTTPDATA data;
	int    len;
    char   g_data[1024];	//! 受信データ用バッファ
    
    int req;//リクエスト識別子
    
    DWCGHTTPPost post;

	// initialize
	//-------------------------------------------
	OS_Printf("initialize\n");

	OS_Init();
    OS_InitTick();
    OS_InitAlarm();
    RTC_Init();

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

    // 表示開始
    OS_WaitVBlankIntr();              // Waiting the end of VBlank interrupt
    GX_DispOn();
    GXS_DispOn();

    // DWCライブラリ初期化
    DWC_Init( s_Work );
    
    // デバッグ表示レベル指定
    DWC_SetReportLevel(DWC_REPORTFLAG_ALL);
    
    DWC_SetMemFunc( AllocFunc, FreeFunc );

	//IPの取得
	while(!StartIPMain())
	{
		OS_Printf("error at StartIPMain()\n");	
	}
    
	// ghttpの初期化
	if(DWC_InitGHTTP(GAMENAME) != TRUE){
		OS_Printf("error at SCInitialize()\n");
	}

	// send data
	//-------------------------------------------
	memset(&data, 0, sizeof(data));
	len = sizeof(data);
    
	// *debug code* put some string
	strcpy((char*)data.tex, "this is a test message");
	strcpy((char*)data.title, "test");
    OS_Printf("[sent] %s\n", (char*)data.tex);
	OS_Printf("[sent] %s\n", (char*)data.title);

	// post data
	//-------------------------------------------
    DWC_GHTTPNewPost(&post);
    DWC_GHTTPPostAddString(&post, "data", (const char*)&data);
                           
	g_pending = 1;
	req = DWC_PostGHTTPData("URL", &post, POSTCallback, NULL);// postするためのURLの指定が必要
	do{
		DWC_ProcessGHTTP();
//	    OS_Printf("get request state: %d\n", DWC_GetGHTTPState(req));// 通信状況のチェック
	}while(g_pending);

	// receive data
	//-------------------------------------------
	g_pending = 1;
	req = DWC_GetGHTTPData("URL", GETCallback, (void*)g_data);// getするためのURLの指定が必要
	do{
		DWC_ProcessGHTTP();
//	    OS_Printf("get request state: %d\n", DWC_GetGHTTPState(req));// 通信状況のチェック
	}while(g_pending);

	// *debug code* put some string
	OS_Printf("[recv] %s\n", g_data);
	
	// receive data
	//-------------------------------------------
	g_pending = 1;
	req = DWC_GetGHTTPDataEx("URL", RECV_SIZE, FALSE, GETProgressCallbackEx, GETCallbackEx, NULL);// getするためのURLの指定が必要
	do{
		DWC_ProcessGHTTP();
//	    OS_Printf("get request state: %d\n", DWC_GetGHTTPState(req));// 通信状況のチェック
	}while(g_pending);

    if(buffer){
        // *debug code* put some string
        OS_Printf("[recv] %s\n", buffer);
        DWC_Free((DWCAllocType)0, (void*)buffer, 0);
    }
    // shutdown
	//-------------------------------------------
	OS_TPrintf("-- done --\n");

	DWC_ShutdownGHTTP();

	OS_Terminate();
}
