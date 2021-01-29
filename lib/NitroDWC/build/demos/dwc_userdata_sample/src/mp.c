#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "userdata.h"
#include "utility.h"
#include "screen.h"
#include "font.h"
#include "dbs.h"
#include "wc.h"

#include "mp.h"

/** --------------------------------------------------------------------
  define
  ----------------------------------------------------------------------*/
#define     PICTURE_FRAME_PER_GAME_FRAME    1

#define     DEFAULT_GGID            0x003fff11
#define     DEFAULT_CHAN            1
#define     NUM_MAX_CHILD           15

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static u16* s_SendBuf;
static DWCFriendData* s_FriendList;
//データ受信用フラグ
static BOOL s_RecvFlag[NUM_MAX_CHILD];

/** --------------------------------------------------------------------
  static function
  ----------------------------------------------------------------------*/
//MP通信用モード
static u8 ModeSelect(void);             // 親機/子機 選択
static void ModeError(void);            // エラー表示
static u8 ModeWorking(void);            // ビジー
static void ModeParent(u16* gSendBuf, DWCFriendData* friendlist);  // 親機 通信
static void ModeChild(u16* gSendBuf, DWCFriendData* friendlist);   // 子機 通信

static void PrintSharedData(u16 bitmap, DWCFriendData* friendlist);// シェアされたデータの表示

//MP通信用親機の初期パラメータ
static WMParentParam defaultParameter ATTRIBUTE_ALIGN(32) = {
    NULL, 0, 0,
    DEFAULT_GGID,                             // ゲームグループID
    0x0000,                                   // temporary ID (毎回+1される)
    1,                                        // エントリー許可フラグ
    NUM_MAX_CHILD,                            // 最大接続子機数
    0,                                        // マルチブートフラグ
    0,                                        // キーシェアリング
    0,                                        // 連続送信フラグ
    100,                                      // beacon 間隔
    {0, 0, 0, 0},                             // ユーザー名
    {0, 0, 0, 0, 0, 0, 0, 0},                 // ゲーム名
    DEFAULT_CHAN,                             // 接続チャンネル
    sizeof(DWCFriendData)*(1+NUM_MAX_CHILD)+4,// 親機送信データサイズ
    sizeof(DWCFriendData)                     // 子機送信データサイズ
};

/*---------------------------------------------------------------------------*
  Name:         MP_SetData

  Description:  送信用データの初期化を行う。

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void MP_SetData(u16* sendbuf, DWCFriendData* friendlist)
{
    s_SendBuf = sendbuf;
    s_FriendList = friendlist;

    // 受信フラグの初期化
    MI_CpuClear8( s_RecvFlag, sizeof(s_RecvFlag) );

    // WM初期化
    WcInit(&defaultParameter, TRUE, FALSE, PICTURE_FRAME_PER_GAME_FRAME);

}

/*---------------------------------------------------------------------------*
  Name:         MP_Match

  Description:  MPでデータの送受信を行う。

  Arguments:    NONE.

  Returns:      通信が終了、またはキャンセルされた場合TRUE
                通信途中の場合FALSEを返す。
 *---------------------------------------------------------------------------*/
BOOL MP_Match()
{
    static u8 child_flag = 0;//通信制御用フラグ

    // 通信状態により処理を振り分け
    switch (WcGetStatus())
    {
    case WC_STATUS_READY:// 親機、子機の選択
        if(child_flag){// 親から切断された子機の通信終了
            child_flag = 0;
            WcEnd();
            return TRUE;
            break;
        }
        ModeSelect();
        break;
    case WC_STATUS_ERROR:
        ModeError();
        break;
    case WC_STATUS_BUSY:// 実行中
        ModeWorking();
        break;
    case WC_STATUS_PARENT:// 親機の場合
        ModeParent(s_SendBuf, s_FriendList);
        break;
    case WC_STATUS_CHILD:// 子機の場合
        ModeChild(s_SendBuf, s_FriendList);
        child_flag = 1;// 親から切断されるとReady状態に戻ってしまうための対処
        break;
    }
    if (g_KeyCtrl.trg & PAD_BUTTON_B){//通信終了
        child_flag = 0;
        WcEnd();
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         MP_Finish

  Description:  MP通信を終了させる。

  Arguments:    NONE.

  Returns:      NONE.
 *---------------------------------------------------------------------------*/
void MP_Finish()
{
    // WMの終了
    WcFinish();
}

/*---------------------------------------------------------------------------*
  Name:         ModeSelect

  Description:  親機/子機 選択画面での処理。

  Arguments:    None.

  Returns:      0: モード決定
                1: キャンセル
 *---------------------------------------------------------------------------*/
static u8 ModeSelect(void)
{
    // 文字表示
    DispMessage( "MP MODE SELECT", NULL, "MP Cancel", "Parent Mode", "Child Mode", NULL);

    if (g_KeyCtrl.trg & PAD_BUTTON_X)
    {
        // 親機通信開始
        WcStartParent();
        return 0;
    }
    else if (g_KeyCtrl.trg & PAD_BUTTON_Y)
    {
        // 子機接続開始
        WcStartChild();
        return 0;
    }

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         ModeError

  Description:  エラー表示画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeError(void)
{
    // 文字表示
    DispMessage( "ERROR!\n Fatal error occured.\n Please reboot program.", NULL, NULL, NULL, NULL, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         ModeWorking

  Description:  ビジー画面での処理。

  Arguments:    None.

  Returns:      0: 実行中
                1: 通信終了
 *---------------------------------------------------------------------------*/
static u8 ModeWorking(void)
{
    // 文字表示
    DispMessage( "NOW WORKING...", NULL, "MP Cancel", NULL, NULL, NULL);

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         ModeParent

  Description:  親機 通信画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeParent(u16* gSendBuf, DWCFriendData* friendlist)
{
    BOOL result;
    u16 bitmap;

    // データシェア同期
    result = WcStepDataSharing((void *)gSendBuf, &bitmap);

    // 文字表示
    DispMessage( "PARENT MODE", NULL, "MP Cancel", NULL, NULL, NULL);
    // シェアされたデータを表示
    if (result)
    {
        PrintSharedData(bitmap, friendlist);
    }
    else
    {
        // 次のフレームも StepDataSharing にトライ
    }
}

/*---------------------------------------------------------------------------*
  Name:         ModeChild

  Description:  子機 通信画面での処理。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ModeChild(u16* gSendBuf, DWCFriendData* friendlist)
{
    BOOL result;
    u16 bitmap;

    // データシェア同期
    result = WcStepDataSharing((void *)gSendBuf, &bitmap);

    // 文字表示
    DispMessage( "CHILD MODE", NULL, "MP Cancel", NULL, NULL, NULL);
    // シェアされたデータを表示
    if (result)
    {
        PrintSharedData(bitmap, friendlist);
    }
    else
    {
        // 次のフレームも StepDataSharing にトライ
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrintSharedData

  Description:  共有された各端末からのデータを表示する。

  Arguments:    bitmap - データを受信した端末を示すIDビットマップを指定。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void PrintSharedData(u16 bitmap, DWCFriendData* friendlist)
{
    u16 i;
    u8 *p;
    int friendIdx;
    u8 friend_number = 0;


    // 親機 + 子機最大数分をループ
    for (i = 0; i < (1 + WM_NUM_MAX_CHILD); i++)
    {
        if (bitmap & (0x0001 << i))
        {
            // IDが"i"の端末からのデータ取得
            p = WcGetSharedDataAddress(i);

            if (p != NULL)
            {
                friend_number++;
                if(!s_RecvFlag[i] && WcGetAid() != i){// 自分を除くフレンドデータを取得
                    friendIdx = DTUD_GetAvailableFriendListIndex();
                    s_RecvFlag[i] = 1;
                    MI_CpuCopy8(p, &friendlist[friendIdx], sizeof(DWCFriendData));
                }
//                PrintString(2, (s16)(15 + i), 0x4, "Friend %d MP Data Received!!", i);
            }
        }
        else{
            s_RecvFlag[i] = 0;
        }
    }
    PrintString(2, 15, 0x4, "Changed Number of Friend %d", friend_number-1);
    // 自機のデータだけ色を変更
//    ColorString(2, (s16)(15 + WcGetAid()), 25, 0x1);
}
