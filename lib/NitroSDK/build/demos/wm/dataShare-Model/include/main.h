/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     main.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.h,v $
  Revision 1.7  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/25 01:35:27  seiki_masashi
  構造体名を GameInfo から MyGameInfo に変更

  Revision 1.5  2005/11/21 00:41:10  adachi_hiroaki
  親機選択画面に参加人数とニックネームを表示するようにした

  Revision 1.4  2005/02/28 05:26:12  yosizaki
  do-indent.

  Revision 1.3  2004/11/05 09:16:48  sasakis
  ウィンドウの挙動を改良し、コードを整理した。

  Revision 1.2  2004/11/05 04:27:40  sasakis
  ロビー画面、親機選択画面の追加と、それに必要な改造（scan関連など）。

  Revision 1.1  2004/11/02 01:07:34  sasakis
  意味の無いコードになってしまっていた部分を戻した他、整理等。

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

typedef struct MyGameInfo_
{
    u8      nickName[10 * 2];
    u8      nickNameLength;
    u8      entryCount;
    u16     periodicalCount;
}
MyGameInfo;

struct PRScreen_;

extern BOOL isDataReceived(int index);
extern ShareData *getRecvData(int index);
extern ShareData *getSendData(void);
extern void changeSysMode(int s);
extern struct PRScreen_ *getInfoScreen(void);

#endif
