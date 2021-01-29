/*---------------------------------------------------------------------------*
  Project:  NitroSDK - SPI - demos
  File:     monkey.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: monkey.h,v $
  Revision 1.4  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.3  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.2  2004/06/10 09:37:46  terui
  メッセージTYPEを追加。サンプリング周期を調整。

  Revision 1.1  2004/06/02 09:53:00  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	MONKEY_H_
#define	MONKEY_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include	<nitro/os.h>
#include	<nitro/spi.h>

/*---------------------------------------------------------------------------*
	定数定義
 *---------------------------------------------------------------------------*/
#define		MONKEY_MESSAGE_ARRAY_MAX	8
#define		MONKEY_STACK_SIZE			1024
#define		MONKEY_THREAD_PRIORITY		15
#define		MONKEY_MIC_SPAN_TICK		( HW_SYSTEM_CLOCK / 64 / 60 / 263 )     //約 1 ライン
#define		MONKEY_MIC_ARRAY_MAX		256
#define		MONKEY_TP_ARRAY_MAX			8
#define		MONKEY_SAMPLING_SPAN_LINE	4       // Periodicタイマーのライン周期

#define		MONKEY_MESSAGE_TYPE_MIC		1
#define		MONKEY_MESSAGE_TYPE_TP		2


/*---------------------------------------------------------------------------*
	構造体定義
 *---------------------------------------------------------------------------*/
typedef struct MonkeyWork
{
    OSMessageQueue msg_q;
    OSMessage msg_buf[MONKEY_MESSAGE_ARRAY_MAX];
    OSThread thread;
    u32     stack[MONKEY_STACK_SIZE / sizeof(u32)];
    OSAlarm alarm;
    u16     micBuf[MONKEY_MIC_ARRAY_MAX];
    u16     micIndex;
    TPData  tpBuf[MONKEY_TP_ARRAY_MAX];
    u16     tpIndex;
    u32     timerCount;

}
MonkeyWork;


/*---------------------------------------------------------------------------*
	関数定義
 *---------------------------------------------------------------------------*/

// 初期化、スレッド起動
void    MonkeyInit(void);

// タッチパネル入力値を配列に取得
void    MonkeyGetNewTpData(s32 num, TPData *array);

// マイク入力値を配列に取得
void    MonkeyGetNewMicData(s32 num, u16 *array);


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif // MONKEY_H_

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
