/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_child.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_measure_channel.c,v $
  Revision 1.10  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.8  2005/02/18 11:08:01  yosizaki
  fix around hidden warnings.

  Revision 1.7  2004/12/09 08:15:19  takano_makoto
  乱数の種の取得方法をOS_GetVBlankCount関数を使用するように修正

  Revision 1.6  2004/12/08 14:27:30  takano_makoto
  同じ電波使用率のチャンネルがあった場合には乱数で決定

  Revision 1.5  2004/10/22 04:10:30  takano_makoto
  MBM_MeasureChannelExをMBM_MeasureChannelInIdleに改名

  Revision 1.4  2004/10/22 02:54:54  takano_makoto
  ユーザコールバック前に状態をクリアするように修正

  Revision 1.3  2004/10/22 02:39:57  takano_makoto
  MBM_MeasureChannelExを追加

  Revision 1.2  2004/09/28 09:52:44  takano_makoto
  デバッグプリントの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/wm.h>

#include "mb_measure_channel.h"

#define MBM_DEBUG

#ifdef MBM_DEBUG
#define PRINTF OS_TPrintf
#else
#define PRINTF(...) ((void)0)
#endif

#define MBM_MEASURE_DMA_NO                 2

// 乱数マクロ
#define RAND()  ( sRand = sRand + 69069UL + 12345 )
#define RAND_INIT(x) ( sRand = (u32)(x) )

enum
{
    MBM_STATE_INIT,                    // 初期状態
    MBM_STATE_MEASURING,               // 開始状態
    MBM_STATE_END_MEASURE,             // 検査終了
    MBM_STATE_END,                     // 無線OFF状態
    MBM_STATE_ERR                      // エラー状態
};


enum
{
    ERRCODE_SUCCESS = 0,               // 成功
    ERRCODE_NOMORE_CHANNEL,            // これ以上のチャンネルが見つからない
    ERRCODE_API_ERR                    // WMのAPI実行エラー
};

//===========================================================================
// 変数宣言
//===========================================================================

static u32 sRand;

static MBMCallbackFunc sUserCallbackFunc = NULL;
static int mbm_measure_state = MBM_STATE_END;
// 通信チャンネル格納用
static s16 sChannel;
static u16 sChannelBitmap;
static u16 sChannelBusyRatio;

static BOOL sUseInIdle = FALSE;


//===========================================================================
// 関数プロトタイプ宣言
//===========================================================================

static int wmi_measure_channel(WMCallbackFunc callback, u16 channel);
static void wm_callback(void *arg);
static void start_measure_channel(void);
static u16 state_in_measure_channel(u16 channel);
static void state_out_measure_channel(void *arg);
static void state_in_wm_end(void);
static void user_callback(s16 num);
static void change_mbm_state(u16 state);
static void callback_ready(s16 result);
static s16 select_channel(u16 bitmap);

//===========================================================================
// 関数定義
//===========================================================================

/* ----------------------------------------------------------------------
  電波使用率のチェック
  ---------------------------------------------------------------------- */
static inline int wmi_measure_channel(WMCallbackFunc callback, u16 channel)
{
#define MBM_MEASURE_TIME         30    // 1フレームに一回通信している電波を拾えるだけの間隔(ms)
#define MBM_MEASURE_CS_OR_ED     3     // キャリアセンスとED値の論理和
#define MBM_MEASURE_ED_THRESHOLD 17    // 実験データによる経験的に有効と思われるお勧めED閾値

    /*
     * 電波使用率取得パラメータとして、
     * 実験による経験的に有効と思われる値を入れています。
     */
    return WM_MeasureChannel(callback, // コールバック設定
                             MBM_MEASURE_CS_OR_ED,      // CS or ED
                             MBM_MEASURE_ED_THRESHOLD,  // キャリアセンスのみの場合は無効
                             channel,  // WM_GetAllowedChannelで取得したチャンネルのどれか
                             MBM_MEASURE_TIME); //１チャンネルあたりの調査時間(ms)
}



/*---------------------------------------------------------------------------*
  Name:         MBM_MeasureChannel

  Description:  最も使用率の低いチャンネルを検索します。
                無線off状態でコールしてください。
                内部で電波を測定し無線off状態にしてからcallbackを返します。

  Arguments:    wm_buffer   WMワークメモリ
                callback    調査完了時に呼び出すユーザコールバックを指定

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBM_MeasureChannel(u8 *wm_buffer, MBMCallbackFunc callback)
{
    sUseInIdle = FALSE;
    sUserCallbackFunc = callback;

    // 初期化シーケンス開始。
    if (mbm_measure_state != MBM_STATE_END)
    {
        user_callback(MBM_MEASURE_ERROR_ILLEGAL_STATE);
        return;
    }

    if (WM_Initialize(wm_buffer, wm_callback, MBM_MEASURE_DMA_NO) != WM_ERRCODE_OPERATING)
    {
        user_callback(MBM_MEASURE_ERROR_INIT_API);
        return;
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBM_MeasureChannelInIdle

  Description:  最も使用率の低いチャンネルを検索します。
                IDLEステートでコールしてください。
                内部で電波を測定しIDLEステートにしてからcallbackを返します。

  Arguments:    callback    調査完了時に呼び出すユーザコールバックを指定

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBM_MeasureChannelInIdle(MBMCallbackFunc callback)
{
    sUseInIdle = TRUE;
    sUserCallbackFunc = callback;

    // 初期化シーケンス開始。
    if (mbm_measure_state != MBM_STATE_END)
    {
        user_callback(MBM_MEASURE_ERROR_ILLEGAL_STATE);
        return;
    }

    start_measure_channel();
}

/* ----------------------------------------------------------------------
  WMコールバック
  ---------------------------------------------------------------------- */
static void wm_callback(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    switch (cb->apiid)
    {
    case WM_APIID_INITIALIZE:
        /* WM_Initializeのコールバック */
        {
            WMCallback *cb = (WMCallback *)arg;
            if (cb->errcode != WM_ERRCODE_SUCCESS)
            {
                user_callback(MBM_MEASURE_ERROR_INIT_CALLBACK);
                return;
            }
        }
        change_mbm_state(MBM_STATE_INIT);
        start_measure_channel();
        break;

    case WM_APIID_MEASURE_CHANNEL:
        /* WM_MeasureChannelのコールバック */
        state_out_measure_channel(arg);
        break;

    case WM_APIID_END:
        change_mbm_state(MBM_STATE_END);
        user_callback(sChannel);
        break;

    default:
        OS_TPanic("Get illegal callback");

        break;
    }
}

/* ----------------------------------------------------------------------
  電波使用率検索開始
  ---------------------------------------------------------------------- */
static void start_measure_channel(void)
{
#define MAX_RATIO 100                  // チャンネル使用率は0～100の範囲
    u16     result;
    u8      macAddr[6];

    OS_GetMacAddress(macAddr);
    RAND_INIT(OS_GetVBlankCount() + *(u16 *)&macAddr[0] + *(u16 *)&macAddr[2] + *(u16 *)&macAddr[4]);   // 乱数初期化
    RAND();

    sChannel = 0;
    sChannelBitmap = 0;
    sChannelBusyRatio = MAX_RATIO + 1;

    result = state_in_measure_channel(1);       // チャンネル１から順にチェック

    if (result == ERRCODE_NOMORE_CHANNEL)
    {
        // 使用できるチャンネルが1つもない
        callback_ready(MBM_MEASURE_ERROR_NO_ALLOWED_CHANNEL);
        return;
    }

    if (result == ERRCODE_API_ERR)
    {
        // エラー終了
        callback_ready(MBM_MEASURE_ERROR_MEASURECHANNEL_API);
        return;
    }

    // 電波使用率調査開始
    change_mbm_state(MBM_STATE_MEASURING);
}


/*---------------------------------------------------------------------------*
  Name:         state_in_measure_channel

  Arguments:    channel     検索を開始するチャンネル番号

  Returns:      ERRCODE_SUCCESS        - 検索中
                ERRCODE_NOMORE_CHANNEL - もう検索するチャンネルがない
                ERRCODE_API_ERR        - WM_MeasureChannelのAPI呼び出し失敗
 *---------------------------------------------------------------------------*/
static u16 state_in_measure_channel(u16 channel)
{
    u16     allowedCannel;

    allowedCannel = WM_GetAllowedChannel();

    while (((1 << (channel - 1)) & allowedCannel) == 0)
    {
        channel++;
        if (channel > 16)
        {
            /* 許可されたチャンネルをすべて調べ終わった場合 */
            return ERRCODE_NOMORE_CHANNEL;
        }
    }

    if (wmi_measure_channel(wm_callback, channel) != WM_ERRCODE_OPERATING)
    {
        return ERRCODE_API_ERR;
    }
    return ERRCODE_SUCCESS;
}


static void state_out_measure_channel(void *arg)
{
    u16     result;
    u16     channel;
    WMMeasureChannelCallback *cb = (WMMeasureChannelCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        callback_ready(MBM_MEASURE_ERROR_MEASURECHANNEL_CALLBACK);
        return;
    }

    channel = cb->channel;
    PRINTF("CH%d = %d\n", cb->channel, cb->ccaBusyRatio);

    if (cb->ccaBusyRatio < sChannelBusyRatio)
    {
        sChannelBusyRatio = cb->ccaBusyRatio;
        sChannelBitmap = (u16)(1 << (channel - 1));
    }
    else if (cb->ccaBusyRatio == sChannelBusyRatio)
    {
        sChannelBitmap |= (u16)(1 << (channel - 1));
    }

    result = state_in_measure_channel(++channel);

    if (result == ERRCODE_NOMORE_CHANNEL)
    {
        // チャンネル検索終了
        callback_ready(select_channel(sChannelBitmap));
        return;
    }

    if (result == ERRCODE_API_ERR)
    {
        // エラー終了
        callback_ready(MBM_MEASURE_ERROR_MEASURECHANNEL_API);
        return;
    }

    // ERRCODE_SUCCESSの場合は何もしない

}


/* ----------------------------------------------------------------------
  コールバック前に無線をOFFにする。
  ---------------------------------------------------------------------- */
static void callback_ready(s16 result)
{
    sChannel = result;
    if (sUseInIdle)
    {
        change_mbm_state(MBM_STATE_END);
        user_callback(result);
    }
    else
    {
        state_in_wm_end();
        change_mbm_state(MBM_STATE_END_MEASURE);
    }
}

/* ----------------------------------------------------------------------
  WM終了
  ---------------------------------------------------------------------- */
static void state_in_wm_end(void)
{
    if (WM_End(wm_callback) != WM_ERRCODE_OPERATING)
    {
        OS_TPanic("fail WM_End");
    }
}


/* ----------------------------------------------------------------------
  MBM内部状態変更
  ---------------------------------------------------------------------- */
static void change_mbm_state(u16 state)
{
#ifdef MBM_DEBUG
    static const char *STATE_STR[] = {
        "INIT",
        "MEASURING",                   // 開始状態
        "END_MEASURE",                 // 検査終了
        "END",                         // 無線OFF状態
        "ERR"                          // エラー状態
    };
#endif

    PRINTF("%s -> ", STATE_STR[mbm_measure_state]);
    mbm_measure_state = state;
    PRINTF("%s\n", STATE_STR[mbm_measure_state]);
}

/* ----------------------------------------------------------------------
  チャンネルを決定する
  ---------------------------------------------------------------------- */
static s16 select_channel(u16 bitmap)
{
    s16     i;
    s16     channel = 0;
    u16     num = 0;
    u16     select;

    for (i = 0; i < 16; i++)
    {
        if (bitmap & (1 << i))
        {
            channel = (s16)(i + 1);
            num++;
        }
    }

    if (num <= 1)
    {
        return channel;
    }

    // 同じ電波使用率のチャンネルが複数存在した場合
    select = (u16)(((RAND() & 0xFF) * num) / 0x100);

    channel = 1;

    for (i = 0; i < 16; i++)
    {
        if (bitmap & 1)
        {
            if (select == 0)
            {
                return (s16)(i + 1);
            }
            select--;
        }
        bitmap >>= 1;
    }

    return 0;
}

/* ----------------------------------------------------------------------
  ユーザーコールバックを呼び出す
  ---------------------------------------------------------------------- */
static void user_callback(s16 type)
{
    MBMCallback arg;

    if (!sUserCallbackFunc)
    {
        return;
    }

    if (type > 0)
    {
        arg.errcode = MBM_MEASURE_SUCCESS;
        arg.channel = (u16)type;
    }
    else
    {
        arg.errcode = type;
        arg.channel = 0;
    }
    sUserCallbackFunc(&arg);
}
