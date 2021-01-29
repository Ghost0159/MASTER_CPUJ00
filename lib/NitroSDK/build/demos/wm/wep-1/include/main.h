/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wep-1
  File:     main.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.h,v $
  Revision 1.1  2005/03/25 07:41:37  seiki_masashi
  dataShare-Model に WEP Key の動的設定を加え、wep-1 デモとして新規登録

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#ifndef __MAIN_H__
#define __MAIN_H__

enum
{
    SYSMODE_SELECT_ROLE,               // 役割（親子のどちらとして開始するか）の選択画面
    SYSMODE_OPTION,                    // オプション画面
    SYSMODE_SELECT_PARENT,             // 親機選択画面（子機専用）
    SYSMODE_LOBBY,                     // ロビー画面（親機専用）
    SYSMODE_LOBBYWAIT,                 // ロビー待機画面（子機専用）
    SYSMODE_SELECT_CHANNEL,            // チャンネル選択画面
    SYSMODE_SCAN_PARENT,               // 親機検索画面
    SYSMODE_ERROR,                     // エラー報告画面
    SYSMODE_PARENT,                    // 親機モード画面
    SYSMODE_CHILD,                     // 子機モード画面
    SYSMODE_NUM
};

enum
{
    SHARECMD_NONE = 0,                 // 特に無し（ノーマル）
    SHARECMD_EXITLOBBY,                // ロビー画面終了の合図
    SHARECMD_NUM
};

typedef struct ShareData_
{
    unsigned int command:3;            // 指令（ゲーム状態の一斉切り換えなどに使用）
    unsigned int level:2;              // 電波受信強度
    unsigned int data:3;               // グラフ用
    unsigned int key:16;               // キーデータ
    unsigned int count:24;             // フレーム数
}
ShareData;

struct PRScreen_;

extern BOOL isDataReceived(int index);
extern ShareData *getRecvData(int index);
extern ShareData *getSendData(void);
extern void changeSysMode(int s);
extern struct PRScreen_ *getInfoScreen(void);

#endif
