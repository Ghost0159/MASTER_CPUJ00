#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "screen.h"
#include "font.h"
#include "dbs.h"
#include "wc.h"

#include "mp.h"

#define     PICTURE_FRAME_PER_GAME_FRAME    1

#define     DEFAULT_GGID            0x003fff11
#define     DEFAULT_CHAN            1
#define     NUM_MAX_CHILD           15

extern KeyControl stKeyCnt;    // キー入力制御構造体

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

//データ受信用フラグ
static BOOL gRecvFlag[NUM_MAX_CHILD];

void mp_match(u16* gSendBuf, DWCFriendData* friendlist){
    u8 child_flag = 0, mp_flag = 0;//通信制御用フラグ

    MI_CpuClear8( gRecvFlag, sizeof(gRecvFlag) );

    // WM初期化
    WcInit(&defaultParameter, TRUE, FALSE, PICTURE_FRAME_PER_GAME_FRAME);
    // 通信状態により処理を振り分け
    while(!mp_flag){
        switch (WcGetStatus())
        {
        case WC_STATUS_READY:// 親機、子機の選択
            if(child_flag){
                mp_flag = 1;
                break;
            }
            mp_flag = ModeSelect();
            break;
        case WC_STATUS_ERROR:
            ModeError();
             break;
        case WC_STATUS_BUSY:// 実行中
            mp_flag = ModeWorking();
            break;
        case WC_STATUS_PARENT:// 親機の場合
            ModeParent(gSendBuf, friendlist);
            break;
        case WC_STATUS_CHILD:// 子機の場合
            ModeChild(gSendBuf, friendlist);
            child_flag = 1;//親から切断されるとReady状態に戻ってしまうための対処
            break;
        }
        ReadKeyData();  // キーデータ取得
        if (stKeyCnt.trg & PAD_BUTTON_B){//通信終了
        	WcEnd();
            mp_flag = 1;
        }

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
        // スタック溢れチェック
        OS_CheckStack(OS_GetCurrentThread());
    }
    // WMの終了
    WcFinish();
    ClearScreen();
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
    // スクリーンクリア
    ClearScreen();
    // 文字表示
    PrintString(3, 8, 0xf,  "Push L to connect as PARENT");
    PrintString(3, 10, 0xf, "Push R to connect as CHILD");
    PrintString(3, 16, 0xf, "Push B to STOP");

    dbs_DemoUpdate();
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
    dbs_DemoLoad();
    // スタック溢れチェック
    OS_CheckStack(OS_GetCurrentThread());
    
    while(1){
        ReadKeyData();  // キーデータ取得
        if (stKeyCnt.trg & PAD_BUTTON_L)
        {
            // 親機通信開始
            WcStartParent();
            return 0;
        }
        else if (stKeyCnt.trg & PAD_BUTTON_R)
        {
            // 子機接続開始
            WcStartChild();
            return 0;
        }
        else if (stKeyCnt.trg & PAD_BUTTON_B)
        {
            return 1;
        }
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
    // スクリーンクリア
    ClearScreen();
    // 文字表示
    PrintString(5, 10, 0x1, "======= ERROR! =======");
    PrintString(5, 13, 0xf, " Fatal error occured.");
    PrintString(5, 14, 0xf, "Please reboot program.");
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
    // スクリーンクリア
    ClearScreen();
    // 文字表示
    PrintString(9, 9, 0xf, "Now working...");
    PrintString(3, 16, 0xf, "Push B to STOP");

    ReadKeyData();  // キーデータ取得
    if (stKeyCnt.trg & PAD_BUTTON_B)
    {
        // 通信終了
        WcEnd();
        return 1;
    }
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

    // スクリーンクリア
    ClearScreen();
    // 文字表示
    PrintString(8, 1, 0x2, "Parent mode");
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

    // スクリーンクリア
    ClearScreen();
    // 文字表示
    PrintString(8, 1, 0x2, "Child mode");
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


    // 親機 + 子機最大数分をループ
    for (i = 0; i < (1 + WM_NUM_MAX_CHILD); i++)
    {
        if (bitmap & (0x0001 << i))
        {
            // IDが"i"の端末からのデータ取得
            p = WcGetSharedDataAddress(i);

            if (p != NULL)
            {
                if(!gRecvFlag[i] && WcGetAid() != i){// 自分を除くフレンドデータを取得
                    friendIdx = GetAvailableFriendListIndex();
                    gRecvFlag[i] = 1;
                    MI_CpuCopy8(p, &friendlist[friendIdx], sizeof(DWCFriendData));
                }
                PrintString(2, (s16)(3 + i), 0x4, "Number%d Receive Success!!", i);
            }
            else
            {
                PrintString(2, (s16)(3 + i), 0x1, "xxxxxxxxxxxxxxxx");
            }
        }
        else{
            gRecvFlag[i] = 0;
        }
    }
    PrintString(3, 20, 0xf, "Push B to STOP");
    // 自機のデータだけ色を変更
    ColorString(2, (s16)(3 + WcGetAid()), 25, 0x1);
}
