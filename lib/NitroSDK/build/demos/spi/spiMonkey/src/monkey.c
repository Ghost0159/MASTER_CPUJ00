/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos
  File:     monkey.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: monkey.c,v $
  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:23  yosizaki
  do-indent.

  Revision 1.8  2004/08/20 02:44:31  terui
  TS_VERSION=0時にもPM_SetAmpするよう修正。

  Revision 1.7  2004/08/04 23:44:28  terui
  LCDバックライト操作関数の変更

  Revision 1.6  2004/08/02 01:07:04  yada
  PM_GAINAMP_80 -> PM_AMPGAIN_80

  Revision 1.5  2004/07/31 12:46:19  terui
  PMICをマイク用に初期化するコードを追加

  Revision 1.4  2004/06/10 09:41:25  terui
  サンプリングタイミングの判断をスレッド内からタイマーハンドラ内に移動。
  タイマーの開始をスレッド内に移動。
  スレッドはメッセージTYPEによりサンプリングデバイスを判断する仕組みに変更。

  Revision 1.3  2004/06/10 05:23:46  yasu
  Change TP_WaitCalibratedResult -> TP_GetCalibratedResult

  Revision 1.2  2004/06/03 11:17:29  terui
  サンプリングレートを変更。

  Revision 1.1  2004/06/02 09:53:25  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    "monkey.h"
#include    <nitro/spi.h>

#ifdef  SDK_TS
#if     ( SDK_TS_VERSION >= 100 )
#include    <nitro/spi/common/pm_common.h>
#include    <nitro/spi/ARM9/pm.h>
#endif
#endif


/*---------------------------------------------------------------------------*
    内部関数定義
 *---------------------------------------------------------------------------*/
static void MonkeyThread(void *arg);
static void MonkeySamplingAlarm(void *arg);
static void MonkeyMicCallback(MICResult result, void *arg);
static void MonkeyTpCallback(TPRequestCommand command, TPRequestResult result, u16 index);


/*---------------------------------------------------------------------------*
    内部変数定義
 *---------------------------------------------------------------------------*/
static MonkeyWork monkey;


/*---------------------------------------------------------------------------*
  Name:         MonkeyInit

  Description:  SPIデバイスサンプリング用スレッドを起動する。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MonkeyInit(void)
{
    // サンプリングのみを行うスレッドを作成
    OS_InitMessageQueue(&(monkey.msg_q), monkey.msg_buf, MONKEY_MESSAGE_ARRAY_MAX);
    OS_CreateThread(&(monkey.thread),
                    MonkeyThread,
                    0,
                    (void *)(monkey.stack + (MONKEY_STACK_SIZE / sizeof(u32))),
                    MONKEY_STACK_SIZE, MONKEY_THREAD_PRIORITY);
    OS_WakeupThreadDirect(&(monkey.thread));
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyGetNewTpData

  Description:  最新のタッチパネルサンプリングデータを取得する。
                すでに一度読み出されているデータかどうかは考慮していない。

  Arguments:    num   - 取得するデータ数を指定する。
                array - データを取得する配列を指定する。
                        配列の先頭から新しい順にデータが格納される。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MonkeyGetNewTpData(s32 num, TPData *array)
{
    s32     i;
    s32     index;

    index = (s32)(monkey.tpIndex);
    for (i = 0; i < num; i++)
    {
        index = (index + (MONKEY_TP_ARRAY_MAX - 1)) % MONKEY_TP_ARRAY_MAX;
        array[i] = monkey.tpBuf[index];
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyGetNewMicData

  Description:  最新のマイクサンプリングデータを取得する。
                すでに一度読み出されているデータかどうかは考慮していない。

  Arguments:    num   - 取得するデータ数を指定する。
                array - データを取得する配列を指定する。
                        配列の先頭から新しい順にデータが格納される。

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MonkeyGetNewMicData(s32 num, u16 *array)
{
    s32     i;
    s32     index;

    index = (s32)(monkey.micIndex);
    for (i = 0; i < num; i++)
    {
        index = (index + (MONKEY_MIC_ARRAY_MAX - 1)) % MONKEY_MIC_ARRAY_MAX;
        array[i] = monkey.micBuf[index];
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyThread

  Description:  サンプリングを行うスレッド。

  Arguments:    arg - 未使用。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MonkeyThread(void *arg)
{
#pragma unused( arg )

    OSMessage msg;

    // マイクAPI初期化
    {
        MIC_Init();
        monkey.micIndex = 0;

#ifdef  SDK_TS
        // PMIC初期化
        PM_Init();
        // AMPオン
        (void)PM_SetAmp(PM_AMP_ON);
#if ( SDK_TS_VERSION >= 100 )
        // AMPゲイン調整
        (void)PM_SetAmpGain(PM_AMPGAIN_80);
#endif
#if ( SDK_TS_VERSION == 100 )
        // ノイズ対策の為、LCDバックライトオフ
        (void)PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);
#endif
#endif

    }
    // タッチパネルAPI初期化
    {
        TPCalibrateParam calibrate;

        TP_Init();
        if (TP_GetUserInfo(&calibrate))
        {
            TP_SetCalibrateParam(&calibrate);
        }
        else
        {
            OS_Panic("Can't find TP calibration data.");
        }
        TP_SetCallback(MonkeyTpCallback);
        monkey.tpIndex = 0;
    }

    // サンプリングタイミング用タイマーを開始
    OS_CreateAlarm(&(monkey.alarm));
    monkey.timerCount = 0;
    OS_SetPeriodicAlarm(&(monkey.alarm),
                        OS_GetTick(),
                        (MONKEY_MIC_SPAN_TICK * MONKEY_SAMPLING_SPAN_LINE),
                        MonkeySamplingAlarm, NULL);

    while (TRUE)
    {
        // メッセージを受け取るまでスレッド休止
        (void)OS_ReceiveMessage(&(monkey.msg_q), &msg, OS_MESSAGE_BLOCK);

        // タッチパネルのサンプリング
        if ((u32)msg == MONKEY_MESSAGE_TYPE_TP)
        {
            TP_RequestSamplingAsync();
        }
        // マイクのサンプリング
        else if ((u32)msg == MONKEY_MESSAGE_TYPE_MIC)
        {
            if (MIC_RESULT_SUCCESS != MIC_DoSamplingAsync(MIC_SAMPLING_TYPE_12BIT,
                                                          &(monkey.micBuf[monkey.micIndex]),
                                                          MonkeyMicCallback, NULL))
            {
                OS_Printf("Monkey: MIC request failure.\n");
            }
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeySamplingAlarm

  Description:  スレッドにメッセージを送るタイミングを制御するアラームハンドラ。

  Arguments:    arg - 未使用。

  Returns:      None.
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    Periodicタイマーとサンプリングのタイミング図

    -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
MIC   >----           >----           >----           >----           >----
TP          >--------                       >--------                       >--

 *---------------------------------------------------------------------------*/
static void MonkeySamplingAlarm(void *arg)
{
#pragma unused( arg )

    if (0 == (monkey.timerCount % 8))
    {
        (void)OS_SendMessage(&(monkey.msg_q), (void *)MONKEY_MESSAGE_TYPE_MIC, OS_MESSAGE_BLOCK);
    }
    else if (3 == (monkey.timerCount % 16))
    {
        (void)OS_SendMessage(&(monkey.msg_q), (void *)MONKEY_MESSAGE_TYPE_TP, OS_MESSAGE_BLOCK);
    }

    monkey.timerCount++;
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyMicCallback

  Description:  マイクのサンプリング要求に対する応答コールバック関数。
                正常にサンプリングできた場合はデータ格納先バッファ位置を
                次に進める。

  Arguments:    result - マイク操作要求に対する処理結果。
                arg    - 未使用。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MonkeyMicCallback(MICResult result, void *arg)
{
#pragma unused( arg )

    if (result == MIC_RESULT_SUCCESS)
    {
        // データ格納バッファ位置を次へ進める
        monkey.micIndex = (u16)((monkey.micIndex + 1) % MONKEY_MIC_ARRAY_MAX);
    }
    else
    {
        // サウンドの影響で直前のサンプリングが完了していないため、失敗。
        OS_Printf("Monkey: MIC request failure.\n");
    }
}

/*---------------------------------------------------------------------------*
  Name:         MonkeyTpCallback

  Description:  タッチパネルのサンプリング要求に対する応答コールバック関数。
                正常にサンプリングできた場合はデータ格納先バッファ位置を
                次に進める。

  Arguments:    command - 応答が対応する操作要求コマンドを示す。未使用。
                result  - タッチパネル操作要求に対する処理結果。
                index   - 自動サンプリング時のインデックス。未使用。

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MonkeyTpCallback(TPRequestCommand command, TPRequestResult result, u16 index)
{
#pragma unused( command , index )

    if (result == TP_RESULT_SUCCESS)
    {
        // サンプリング直値をスクリーン座標値に変換
        (void)TP_GetCalibratedResult(&(monkey.tpBuf[monkey.tpIndex]));
        // データ格納バッファ位置を次へ進める
        monkey.tpIndex = (u16)((monkey.tpIndex + 1) % MONKEY_TP_ARRAY_MAX);
    }
    else
    {
        // サウンドの影響で直前のサンプリングが完了していないため、失敗。
        OS_Printf("Monkey: TP request failure.\n");
    }
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
