/*---------------------------------------------------------------------------*
  Project:  NitroDWC - ranking sample program - 
  File:     main.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.17  2007/04/18 06:34:56  hayashi
  768 -> 764

  Revision 1.16  2007/02/14 11:46:33  takayama
  DWCAccUserData   -> DWCUserData
  DWCAccFriendData -> DWCFriendData

  Revision 1.15  2006/12/15 09:06:19  hayashi
  crc -> checksum
  bug fixed: size of sending user defined data

  Revision 1.14  2006/06/16 04:53:39  hayashi
  added crc feature to the user defined data

  Revision 1.13  2006/06/01 13:25:25  hayashi
  added sample to cancel

  Revision 1.12  2006/05/24 09:17:02  hayashi
  changed to descending order at getting order part

  Revision 1.11  2006/05/24 06:20:40  hayashi
  change the initialize data for Ranking which is adapted to "dwctest"

  Revision 1.10  2006/05/22 07:29:57  hayashi
  changed to be set the server by DWC_SetAuthServer()

  Revision 1.9  2006/05/17 02:47:09  hayashi
  add "last update" output

  Revision 1.8  2006/05/12 08:28:54  hayashi
  NOTINITIALIZED -> NOTREADY
  RIVAL -> NEAR
  DWCAccUserData -> DWCUserData
  BOOL	DWC_RnkShutdown() -> DWCRnkError

  Revision 1.7  2006/04/26 09:25:40  hayashi
  specification is changed related to PID handling

  Revision 1.6  2006/04/17 06:06:38  takayama
  SVC_WaitVBlankIntr() -> OS_WaitVBlankIntr()

  Revision 1.5  2006/04/13 11:35:19  hayashi
  Change prefix Ranking->Rnk

  Revision 1.4  2006/04/13 09:01:37  hayashi
  character code converting

  Revision 1.3  2006/04/11 09:01:33  hayashi
  DWC_RnkInitializeで、接続先のサーバーを選択できるようにした

  Revision 1.2  2006/04/11 07:04:19  hayashi
  base64のエンコードサイズの計算が間違っていたので修正

  Revision 1.1  2006/04/10 10:16:58  hayashi
  added the ranking module


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

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

#define RANKING_INITDATA "hxBfMOkOvlOHpUGfQlOb0001ed350000685d0040000037012e6bdwctest"
#define CATEGORY		10

// 構造体型宣言
//---------------------------------------------------------

// ユーザー定義データ
typedef struct _tagMYUSERDEFINEDDATA{
	u32		checksum;		// checksum
	u32		size;		// サイズ
	u8		data[32];	// データ(32bytes)
}MYUSERDEFINEDDATA;


// グローバル変数宣言
//---------------------------------------------------------
DWCInetControl stConnCtrl;


// 関数プロトタイプ
//---------------------------------------------------------
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

	DWCUserData	    userdata;	// DWCのユーザーデータ
	DWCRnkError		res;		// ランキングライブラリのエラー取得用
	DWCRnkGetParam	param;		// ランキング取得時のパラメータ

	MYUSERDEFINEDDATA	userdefineddata;	// ユーザー定義データ

	u32 order;
	u32 count;
	s32 timeout;
	int i;

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
	OS_WaitVBlankIntr();			  // Waiting the end of VBlank interrupt
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

	//---------------------------------
	// ログイン処理
	//---------------------------------
	//
	// ここでユーザーデータの取得処理を行ってください。ランキングライブラリは
	// GameSpyに依存しているため、少なくとも一度はログインをした有効な
	// GameSpyProfileIDを取得しているユーザーデータでなければ初期化に失敗しま
	// す。仮IDは無効です。
	//
	// 既に過去にログインして取得したユーザーデータがバックアップに保存さ
	// れている場合はそれを使用することができます。ただし、ガイドラインに定め
	// られているとおり、インターネットに接続する際にはその都度必ずログインす
	// る必要があることに注意してください
	//
	// 外部認証サーバーなどを使用している場合は有効なGameSpyProfileIDを取得す
	// ることはできませんので、ランキングライブラリは使用できません。そもそも
	// その場合はGameSpyのサーバー資源を使用できません。
	//
	// <注意>
	//
	// このサンプルでは簡単のためにユーザーデータを無理やり生成していますが、
	// 実際にはログインして得られた正式なユーザーデータを使用するようにしてく
	// ださい。それ以外のデータを渡した場合はランキングモジュールの初期化に
	// 成功する可能性はありますが、動作は未定義です。
	//
	{
		// ユーザーデータの設定
		//
		//
		u32 dummyuserdata[DWC_ACC_USERDATA_BUFSIZE] = {
								0x00000040,0xa1b2c3d4,0x5e6f7a8b,0xc9d0e1f2,
								0x00000996,0x3a4b5c6d,0xe7f8a9b0,0x0001e240,
								0x1c2d3e4f,0xa5b6c7d8,0x9e0f1a2b,0xc3d4e5f6,
								0x7a8b9c0d,0xe1f2a3b4,0x5c6d7e8f,0xcfa4db0e };

		memcpy( &userdata, dummyuserdata, DWC_ACC_USERDATA_BUFSIZE );

		// 接続先サーバーの設定
		//
		// DWC_CONNECTINET_AUTH_TEST	開発向けサーバー
		// DWC_CONNECTINET_AUTH_RELEASE	製品向けサーバー
		//
		// ランキングライブラリでもサーバーが二つ用意されており、どちらを使う
		// かはどちらの認証サーバーを使うかで決定されます。ここでは便宜上
		// 明示的に呼び出していますが、正式なログインのステップを実装してくだ
		// さい。
		//
		DWC_SetAuthServer( DWC_CONNECTINET_AUTH_TEST );
	}

	//---------------------------------
	// ランキングモジュールの処理
	//---------------------------------

	// ランキングモジュールの初期化
	res = DWC_RnkInitialize(	RANKING_INITDATA,
								&userdata );

	switch( res ){
	case DWC_RNK_SUCCESS:
		OS_Printf("DWC_RnkInitialize success\n");
		break;
	case DWC_RNK_ERROR_INIT_ALREADYINITIALIZED:
		OS_Printf("[err]DWC_RnkInitialize already initialized\n");
		goto exit;
	case DWC_RNK_ERROR_INIT_INVALID_INITDATASIZE:
		OS_Printf("[err]DWC_RnkInitialize invalid init data size\n");
		goto exit;
	case DWC_RNK_ERROR_INIT_INVALID_INITDATA:
		OS_Printf("[err]DWC_RnkInitialize invalid init data\n");
		goto exit;
	case DWC_RNK_ERROR_INIT_INVALID_USERDATA:
		OS_Printf("[err]DWC_RnkInitialize invaild user data\n");
		goto exit;
	}

	//
	// アップロード
	//

	// ユーザー定義データの生成
	//
	// ユーザー定義データには任意のバイナリデータを764バイト指定することができ
	// ます。このデータはランキングリストの取得時に一緒に取得することができま
	// す。
	//
	// このデータは、悪意のあるユーザーによる偽装サーバーなどにより、不正に
	// 書き換えられる可能性があります。不正な書き換えが行われたデータにより、
	// プログラムが暴走したり、悪意のあるコードが実行されることのないように、
	// クライアントでは入力されるデータは信頼されるものではないという前提の上に
	// コーディングを行ってください。
	//
	// 本サンプルでは、ユーザー定義データにサイズとChecksumを記録し、データの
    // 正当性をチェックしています。実際には、NULL終端であると仮定したデータの
    // サーチによるオーバーフロー等にも留意し、少なくともユーザー定義データの
    // サイズ以上のメモリ領域にアクセスしないようにコーディングしてください。
	//
	userdefineddata.size = 32;
	strcpy((char *)userdefineddata.data, "test data");
	userdefineddata.checksum = (u32)MATH_CalcChecksum8(
										(const void*)userdefineddata.data,
										userdefineddata.size );

	// スコアアップロードリクエスト開始
	res = DWC_RnkPutScoreAsync(	CATEGORY,				// category
								DWC_RNK_REGION_JP,		// region
								1234,					// score
								(void*)&userdefineddata,// user defined data
								sizeof( userdefineddata ) );

	switch( res ){
	case DWC_RNK_SUCCESS:
		OS_Printf("DWC_RnkPutScoreAsync success\n");
		break;
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkPutScoreAsync in error\n");
		goto exit;
	case DWC_RNK_ERROR_PUT_NOTREADY:
		OS_Printf("[err]DWC_RnkPutScoreAsync not ready\n");
		goto exit;
	case DWC_RNK_ERROR_INVALID_PARAMETER:
		OS_Printf("[err]DWC_RnkPutScoreAsync invalid parameter\n");
		goto exit;
	case DWC_RNK_ERROR_PUT_INVALID_KEY:
		OS_Printf("[err]DWC_RnkPutScoreAsync invalid key\n");
		goto exit;
	case DWC_RNK_ERROR_PUT_NOMEMORY:
		OS_Printf("[err]DWC_RnkPutScoreAsync nomemory\n");
		goto exit;
	}

	// 非同期処理

	timeout = 5000;

	while( (res = DWC_RnkProcess()) == DWC_RNK_SUCCESS ){
	
		OS_Sleep(10);

		// 5秒経過していたらタイムアウトする
		//  -> キャンセルして終了
		if( (timeout -= 10) < 0 ){

			res = DWC_RnkCancelProcess();

			switch(res){
			case DWC_RNK_SUCCESS:
				OS_Printf("[timeout] DWC_RnkCancelProcess\n");
				goto exit;
			case DWC_RNK_IN_ERROR:
				break;
			case DWC_RNK_ERROR_CANCEL_NOTASK:
				break;
			}

		}
	
	}

	switch( res ){
	case DWC_RNK_PROCESS_NOTASK:
		OS_Printf("DWC_RnkProcess done\n");
		break;
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkProcess in error [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;
	}

	// 通信結果取得
	switch( DWC_RnkGetState() ){
	
	case DWC_RNK_STATE_COMPLETED:
		OS_Printf("DWC_RnkProcess Succeeded \n");
		break;

	case DWC_RNK_STATE_ERROR:
		OS_Printf("[err]DWC_RnkGetState [code:%d]\n", DWC_RnkGetState() );
		goto exit;
	
	}



	//
	// 自分の順位を取得
	//

	// 順位取得リクエスト開始

	param.size = sizeof( param.order );
	param.order.since = 0;
	param.order.sort = DWC_RNK_ORDER_DES;

	res = DWC_RnkGetScoreAsync(	DWC_RNK_GET_MODE_ORDER,		// mode
								CATEGORY,					// category
								DWC_RNK_REGION_JP,			// region
								&param );					// parameter

	switch( res ){
	case DWC_RNK_SUCCESS:
		OS_Printf("DWC_RnkGetScoreAsync success\n");
		break;
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkGetScoreAsync in error\n");
		goto exit;
	case DWC_RNK_ERROR_GET_NOTREADY:
		OS_Printf("[err]DWC_RnkGetScoreAsync not ready\n");
		goto exit;
	case DWC_RNK_ERROR_INVALID_PARAMETER:
		OS_Printf("[err]DWC_RnkGetScoreAsync invalid parameter\n");
		goto exit;
	case DWC_RNK_ERROR_GET_INVALID_KEY:
		OS_Printf("[err]DWC_RnkGetScoreAsync invalid key\n");
		goto exit;
	case DWC_RNK_ERROR_GET_NOMEMORY:
		OS_Printf("[err]DWC_RnkGetScoreAsync nomemory\n");
		goto exit;
	}

	// 非同期処理

	timeout = 5000;

	while( (res = DWC_RnkProcess()) == DWC_RNK_SUCCESS ){
	
		OS_Sleep(10);

		// 5秒経過していたらタイムアウトする
		//  -> キャンセルして終了
		if( (timeout -= 10) < 0 ){

			res = DWC_RnkCancelProcess();

			switch(res){
			case DWC_RNK_SUCCESS:
				OS_Printf("[timeout] DWC_RnkCancelProcess\n");
				goto exit;
			case DWC_RNK_IN_ERROR:
				break;
			case DWC_RNK_ERROR_CANCEL_NOTASK:
				break;
			}

		}
	
	}

	switch( res ){
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkProcess in error [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;
	case DWC_RNK_PROCESS_NOTASK:
		OS_Printf("DWC_RnkProcess done\n");
		break;
	}

	// 通信結果取得
	switch( DWC_RnkGetState() ){
	
	case DWC_RNK_STATE_COMPLETED:
		OS_Printf("DWC_RnkProcess success \n");
		break;

	case DWC_RNK_STATE_ERROR:
		OS_Printf("[err]DWC_RnkGetState [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;
	
	}

	// 表示
	res = DWC_RnkResGetOrder( &order );
	switch( res ){
	case DWC_RNK_SUCCESS:
		break;
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkResGetOrder in error [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;
	case DWC_RNK_ERROR_INVALID_MODE:
		OS_Printf("[err]DWC_RnkResGetOrder invalid mode\n");
		goto exit;
	case DWC_RNK_ERROR_INVALID_PARAMETER:
		OS_Printf("[err]DWC_RnkResGetOrder invalid parameter\n");
		goto exit;
	case DWC_RNK_ERROR_NOTCOMPLETED:
		OS_Printf("[err]DWC_RnkResGetOrder not completed\n");
		goto exit;
	case DWC_RNK_ERROR_EMPTY_RESPONSE:
		OS_Printf("[err]DWC_RnkResGetOrder empty response\n");
		goto exit;
	}

	OS_Printf("order=%d\n", order);


	//
	// ランキングを取得
	//

	// トップランキング取得リクエスト開始
	param.size = sizeof( param.toplist );
	param.toplist.sort = DWC_RNK_ORDER_DES;
	param.toplist.limit = 10;
	param.toplist.since = 0;

	res = DWC_RnkGetScoreAsync(	DWC_RNK_GET_MODE_TOPLIST,	// mode
									CATEGORY,				// category
									DWC_RNK_REGION_JP,		// region
									&param );				// parameter

	switch( res ){
	case DWC_RNK_SUCCESS:
		OS_Printf("DWC_RnkGetScoreAsync success\n");
		break;
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkGetScoreAsync in error\n");
		goto exit;
	case DWC_RNK_ERROR_GET_NOTREADY:
		OS_Printf("[err]DWC_RnkGetScoreAsync not ready\n");
		goto exit;
	case DWC_RNK_ERROR_INVALID_PARAMETER:
		OS_Printf("[err]DWC_RnkGetScoreAsync invalid parameter\n");
		goto exit;
	case DWC_RNK_ERROR_GET_INVALID_KEY:
		OS_Printf("[err]DWC_RnkGetScoreAsync invalid key\n");
		goto exit;
	case DWC_RNK_ERROR_GET_NOMEMORY:
		OS_Printf("[err]DWC_RnkGetScoreAsync nomemory\n");
		goto exit;
	}

	// 非同期処理

	timeout = 5000;

	while( (res = DWC_RnkProcess()) == DWC_RNK_SUCCESS ){
	
		OS_Sleep(10);

		// 5秒経過していたらタイムアウトする
		//  -> キャンセルして終了
		if( (timeout -= 10) < 0 ){

			res = DWC_RnkCancelProcess();

			switch(res){
			case DWC_RNK_SUCCESS:
				OS_Printf("[timeout] DWC_RnkCancelProcess\n");
				goto exit;
			case DWC_RNK_IN_ERROR:
				break;
			case DWC_RNK_ERROR_CANCEL_NOTASK:
				break;
			}

		}
	
	}

	switch( res ){
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkProcess in error [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;
	case DWC_RNK_PROCESS_NOTASK:
		OS_Printf("DWC_RnkProcess done\n");
		break;
	}

	// 通信結果取得
	switch( DWC_RnkGetState() ){

	case DWC_RNK_STATE_COMPLETED:
		OS_Printf("DWC_RnkProcess success \n");
		break;

	case DWC_RNK_STATE_ERROR:
		OS_Printf("[err]DWC_RnkGetState [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;

	}


	// 一覧を取得する

	res = DWC_RnkResGetRowCount( &count );

	switch( res ){
	case DWC_RNK_SUCCESS:
		break;
	case DWC_RNK_IN_ERROR:
		OS_Printf("[err]DWC_RnkResGetRowCount in error [code:%d]\n", DWC_GetLastError( NULL ) );
		goto exit;
	case DWC_RNK_ERROR_INVALID_MODE:
		OS_Printf("[err]DWC_RnkResGetRowCount invalid mode\n");
		goto exit;
	case DWC_RNK_ERROR_INVALID_PARAMETER:
		OS_Printf("[err]DWC_RnkResGetRowCount invalid parameter\n");
		goto exit;
	case DWC_RNK_ERROR_NOTCOMPLETED:
		OS_Printf("[err]DWC_RnkResGetRowCount not completed\n");
		goto exit;
	case DWC_RNK_ERROR_EMPTY_RESPONSE:
		OS_Printf("[err]DWC_RnkResGetRowCount empty response\n");
		goto exit;
	}
	
	for( i=0; i<count; i++ ){

		DWCRnkData data;

		if( DWC_RnkResGetRow( &data, (u32)i ) != DWC_RNK_SUCCESS ){
			OS_Printf("[err]DWC_RnkResGetRow()\n");
			break;
		}

		// 出力
		OS_TPrintf("%dth score=%d pid=%d region=%d update=%dmin before ",
						data.order,
						data.score,
						data.pid,
						data.region,
						data.lastupdate );

		// ユーザー定義データの正当性チェック
		{
			MYUSERDEFINEDDATA*	mydata;
			
			mydata = (MYUSERDEFINEDDATA*)data.userdata;

			if(mydata->size != 32){

				// データサイズが不正
				OS_TPrintf("userdata=[invalid data size]\n");

			}else if(mydata->checksum != (u32)MATH_CalcChecksum8(
										(const void*)mydata->data,
										mydata->size )){

				// Checksumが不正
				OS_TPrintf("userdata=[invalid checksum]\n");
			
			}else{

				// 正しいデータ
				OS_TPrintf("userdata=%s\n", mydata->data);

			}

		}

	}

exit:
	// ランキングモジュールの終了
	DWC_RnkShutdown();
	
	// 終了
	OS_Printf("Terminated.\n");	

	OS_Terminate();

}
