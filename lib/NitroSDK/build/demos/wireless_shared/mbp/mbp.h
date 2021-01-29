/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - mbp
  File:     mbp.h


  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mbp.h,v $
  Revision 1.5  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.4  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.3  2004/12/03 08:13:32  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX に修正、 MBP_USING_PREVIOUS_DOWNLOADスイッチを追加

  Revision 1.2  2004/11/24 06:15:12  yosizaki
  rename MB_StartParentEx/MB_EndEx

  Revision 1.1  2004/11/12 09:02:39  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITROSDK_DEMO_WIRELESSSHARED_MBP_H_
#define NITROSDK_DEMO_WIRELESSSHARED_MBP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/mb.h>


/******************************************************************************/
/* constant */

/*
 * ゲームグループ ID.
 *
 * MBGameRegistry 構造体において個々のファイルへ
 * GGID を設定できるようになったため,
 * MB_Init() での指定は将来的に意味を成さなくなるかもしれません.
 */

/*
 * 親機が配信するチャンネル.
 *
 * マルチブート子機は全ての可能なチャンネルを巡回しますので
 * WM ライブラリが許す値(現状 3, 8, 13)のどれを採用してもかまいません.
 *
 * ユーザアプリケーションにおいて通信の混雑を避ける場合は可変にします.
 * チャンネル変更のタイミングについてはアプリケーションの裁量になりますが,
 * 例えば, レスポンスの悪さに対しユーザが「再スタート」を押すなどの
 * 契機を設けるといった方法が考えられます.
 */

/* 接続可能な子機最大数 */
#define MBP_CHILD_MAX       (15)

/* MB ライブラリに割り当てる DMA 番号 */
#define MBP_DMA_NO          (2)

/*
 * MB ライブラリで MB_StartParentFromIdle() / MB_EndToIdle() を使用するスイッチ.
 * マルチブート処理を IDLE 状態で経由することができます.
 */
#define MBP_USING_MB_EX

/*
 * エントリーして来た子機に対して、あらかじめデータ送信を開始しておくスイッチ.
 * エントリーすると即座にデータ送信を開始するため、子機がMAX人数になる前開始する場合には
 * ダウンロードの為の待ち時間を短縮することができます。
 */
#define MBP_USING_PREVIOUS_DOWNLOAD


/* MB親機状態 */
typedef struct
{
    u16     state;                     // 親機状態
    u16     connectChildBmp;           // 接続中の全子機フラグ
    u16     requestChildBmp;           // エントリーリクエスト中の子機フラグ
    u16     entryChildBmp;             // データ待ち中の子機フラグ
    u16     downloadChildBmp;          // データダウンロード中の子機フラグ
    u16     bootableChildBmp;          // ダウンロード完了した子機フラグ
    u16     rebootChildBmp;            // ブートを送信した子機フラグ
}
MBPState;


/* 接続子機状態 */
typedef struct
{
    MBUserInfo user;
    u8      macAddress[6];
    u16     playerNo;
}
MBPChildInfo;


/* このデモのゲームシーケンス状態値 */
enum
{
    MBP_STATE_STOP,                    // 停止状態
    MBP_STATE_IDLE,                    // アイドル状態(Init済)
    MBP_STATE_ENTRY,                   // 子機エントリー受付中
    MBP_STATE_DATASENDING,             // MBデータ送信中
    MBP_STATE_REBOOTING,               // 子機リブート中
    MBP_STATE_COMPLETE,                // 子機リブート完了
    MBP_STATE_CANCEL,                  // マルチブートのキャンセル処理中
    MBP_STATE_ERROR,                   // エラー発生
    MBP_STATE_NUM
};

/* 接続状態ビットマップのタイプ */
typedef enum
{
    MBP_BMPTYPE_CONNECT,               // 接続子機情報
    MBP_BMPTYPE_REQUEST,               // 接続要求中の子機情報
    MBP_BMPTYPE_ENTRY,                 // エントリーしてダウンロード待ちの子機情報
    MBP_BMPTYPE_DOWNLOADING,           // ダウンロード中の子機情報
    MBP_BMPTYPE_BOOTABLE,              // ブート可能子機
    MBP_BMPTYPE_REBOOT,                // リブート完了子機
    MBP_BMPTYPE_NUM
}
MBPBmpType;

/* マルチブート子機状態 */
typedef enum
{
    MBP_CHILDSTATE_NONE,               // 接続なし
    MBP_CHILDSTATE_CONNECTING,         // 接続中
    MBP_CHILDSTATE_REQUEST,            // 接続要求中
    MBP_CHILDSTATE_ENTRY,              // エントリー中
    MBP_CHILDSTATE_DOWNLOADING,        // ダウンロード中
    MBP_CHILDSTATE_BOOTABLE,           // ブート待機中
    MBP_CHILDSTATE_REBOOT,             // リブート実行
    MBP_CHILDSTATE_NUM
}
MBPChildState;


/******************************************************************************/
/* variable */

/* 親機初期化 */
void    MBP_Init(u32 ggid, u16 tgid);
void    MBP_Start(const MBGameRegistry *gameInfo, u16 channel);

/* 1 フレームごとの親機メイン処理 */
const MBPState *MBP_Main(void);

void    MBP_KickChild(u16 child_aid);
void    MBP_AcceptChild(u16 child_aid);
void    MBP_StartRebootAll(void);
void    MBP_StartDownload(u16 child_aid);
void    MBP_StartDownloadAll(void);
BOOL    MBP_IsBootableAll(void);

void    MBP_Cancel(void);

u16     MBP_GetState(void);
u16     MBP_GetChildBmp(MBPBmpType bmpType);
void    MBP_GetConnectState(MBPState * state);
const u8 *MBP_GetChildMacAddress(u16 aid);
MBPChildState MBP_GetChildState(u16 aid);
u16     MBP_GetPlayerNo(const u8 *macAddress);
const MBPChildInfo *MBP_GetChildInfo(u16 child_aid);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // NITROSDK_DEMO_WIRELESSSHARED_MBP_H_
