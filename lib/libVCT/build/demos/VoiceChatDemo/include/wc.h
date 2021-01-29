#ifndef __WC_H_
#define __WC_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include    <nitro/types.h>
#include    <nitro/wm.h>


/*---------------------------------------------------------------------------*
     定数定義 
 *---------------------------------------------------------------------------*/
#define     WC_DMA_NO                   2       // GX等のDMA番号と重ならないよう注意 
#define     WC_DEFAULT_PORT             4       // ポート番号( 4～7 ) 
#define     WC_DEFAULT_PORT_PRIO        2       // ポートの優先度( 0～3 ) 
#define     WC_DS_PORT_NUMBER           12      // データシェア用ポート番号( 12～15 ) 


/*---------------------------------------------------------------------------*
     マクロ定義 
 *---------------------------------------------------------------------------*/
//  32バイトアライン用 
//  For 32-byte alignment 
#define     WC_ROUNDUP_32B( x ) ( ( x + 31 ) & 0xffffffe0 )


/*---------------------------------------------------------------------------*
     構造体定義 
 *---------------------------------------------------------------------------*/
typedef enum WCStatus
{
    WC_STATUS_READY = 0,               //  初期化状態  
    WC_STATUS_ERROR,                   //  どうにもならないエラー状態  
    WC_STATUS_BUSY,                    //  状態遷移中につきビジー状態  
    WC_STATUS_PARENT,                  //  親機として接続済み状態  
    WC_STATUS_CHILD,                   //  子機として接続済み状態  
    WC_STATUS_MAX
}
WCStatus;

//  単体MPモード用のコールバック関数型定義 
typedef void (*WCCallbackFunc) (u16 aid, u16 *data, u16 length);

// ブロック転送モード用のコールバック関数型定義
// 現在は、以下の時点で WMCallback ポインタが渡される
// ・WM_StartMP 完了時
// ・WcSetParentData,WcSetChildData 完了時
// ・WM_Reset 完了時
typedef void (*WCBlockTransferCallbackFunc) (WMCallback *arg);

//  ピクトチャットサーチ用のコールバック関数型定義 
typedef void (*WCPictoCatchCallbackFunc) (WMBssDesc *pBssDesc);


/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/
void WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod);        // 各種初期化。主にメモリ確保 
void WcFinish(void);                   // 初期化前の状態に戻す。主にメモリ解放 
void WcStartParent(void);              // 親機としての接続開始 
void WcStartChild(void);               // 子機としての接続開始 
void WcEnd(void);                      // 初期化状態への移行開始 
WCStatus WcGetStatus(void);            // WCStatus型の内部状態取得 
u16 WcGetAid(void);                    // AID取得 

// MP通信のデータ受信時に呼び出すコールバック設定
void WcSetMpReceiveCallback(WCCallbackFunc cb);
// ブロック転送モード設定時に呼び出すコールバック設定
void WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb);
// 親機としての送信データセット
void WcSetParentData(const void *buf, u16 size);
// 子機としての送信データセット
void WcSetChildData(const void *buf, u16 size);

// 接続完了後のデータシェア通信を進める。1ゲームフレームに一回コール
BOOL WcStepDataSharing(const void *send, u16 *bitmap);
// 受信データから各端末のデータへのポインタを取得する。
u8 *WcGetSharedDataAddress(u16 aid);

// ピクトチャットサーチ機能を有効にする
void WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func);
// ピクトチャットサーチ機能を無効にする
void WcTurnOffPictoCatch(void);


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* DEMOS_WIRELESS_SHARED_WC_H_ */

