/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless-all
  File:     common.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: common.h,v $
  Revision 1.2  2006/04/10 13:37:12  yosizaki
  beautify sourcecode indent.

  Revision 1.1  2006/04/10 13:07:32  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef WM_DEMO_WIRELESS_ALL_COMMON_H_
#define WM_DEMO_WIRELESS_ALL_COMMON_H_


#include <nitro.h>

#include "mbp.h"
#include "wh.h"
#include "wfs.h"


/*****************************************************************************/
/* declaration */

/* データシェアリングに使用するキー入力構造体 */
typedef struct
{
    u32     count;                     /* フレーム情報 */
    u16     key;                       /* キー情報 */
    u16     padding;
}
GShareData;

SDK_COMPILER_ASSERT(sizeof(GShareData) <= WH_DS_DATA_SIZE);


/*****************************************************************************/
/* constant */

/* テキスト用パレットカラー */
enum
{
    PLTT_BLACK = 0,
    PLTT_BLUE = 1,
    PLTT_RED = 2,
    PLTT_PURPLE = 3,
    PLTT_GREEN = 4,
    PLTT_CYAN = 5,
    PLTT_YELLOW = 6,
    PLTT_WHITE = 7,

    PLTT_LINK_ICON                     /* ワイヤレスリンク強度表示アイコン用パレット */
};


/*****************************************************************************/
/* function */

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
  Name:         InitCommon

  Description:  共通初期化関数

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    InitCommon(void);

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_PRESS

  Description:  自身のキー判定.

  Arguments:    判定するキーフラグ.

  Returns:      指定したキーが押下されている場合は TRUE,
                されていない場合は FALSE.
 *---------------------------------------------------------------------------*/
BOOL    IS_PAD_PRESS(u16 flag);

/*---------------------------------------------------------------------------*
  Name:         IS_PAD_TRIGGER

  Description:  自身のキートリガ判定.

  Arguments:    判定するキーフラグ.

  Returns:      指定したキーのトリガが立っている場合は TRUE,
                立っていない場合は FALSE.
 *---------------------------------------------------------------------------*/
BOOL    IS_PAD_TRIGGER(u16 flag);

/*---------------------------------------------------------------------------*
  Name:         GetCurrentInput

  Description:  指定プレイヤーの現在のキー入力を取得.

  Arguments:    aid               キー入力を取得するプレイヤー.

  Returns:      最新のデータシェアリングに成功していれば有効なアドレス,
                そうでなければ NULL.
 *---------------------------------------------------------------------------*/
GShareData *GetCurrentInput(int aid);

/*---------------------------------------------------------------------------*
  Name:         GetCurrentChannel

  Description:  現在選択しているワイヤレスチャンネルを取得.

  Arguments:    None

  Returns:      現在選択しているワイヤレスチャンネル.
 *---------------------------------------------------------------------------*/
u16     GetCurrentChannel(void);

/*---------------------------------------------------------------------------*
  Name:         SetCurrentChannel

  Description:  現在のワイヤレスチャンネルを設定.

  Arguments:    channel           設定するチャンネル.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    SetCurrentChannel(u16 channel);

/*---------------------------------------------------------------------------*
  Name:         LoadLinkIcon

  Description:  リンクアイコンをVRAMにロード.

  Arguments:    id                ロード先のキャラクタID.
                palette           ロード先のパレット.
                level             リンク強度.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    LoadLinkIcon(int id, int palette, int level);

/*---------------------------------------------------------------------------*
  Name:         PrintString

  Description:  BGテキストを描画.

  Arguments:    x                 描画 x グリッド位置.
                y                 描画 x グリッド位置.
                palette           パレットインデックス.
                format            書式文字列.

  Returns:      None
 *---------------------------------------------------------------------------*/
void    PrintString(int x, int y, int palette, const char *format, ...);

/*---------------------------------------------------------------------------*
  Name:         WaitNextFrame

  Description:  次のピクチャーフレームを待ち描画を初期化する.

  Arguments:    None.

  Returns:      キー入力が更新されたら TRUE.
 *---------------------------------------------------------------------------*/
BOOL    WaitNextFrame(void);

/*---------------------------------------------------------------------------*
  Name:         WaitWHState

  Description:  WH の指定のステートになるまで待機.

  Arguments:    state             遷移を待つステート.

  Returns:      None
 *---------------------------------------------------------------------------*/
void    WaitWHState(int state);

/*---------------------------------------------------------------------------*
  Name:         MeasureChannel

  Description:  使用可能な各チャンネルのワイヤレス使用率を計測.
                完了するまで内部で表示を行いブロッキングする.

  Arguments:    None.

  Returns:      もっともワイヤレス使用率の小さいチャンネル.
 *---------------------------------------------------------------------------*/
u16     MeasureChannel(void);

/*---------------------------------------------------------------------------*
  Name:         ExecuteDownloadServer

  Description:  DSダウンロードプレイ親機処理を実行.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ExecuteDownloadServer(void);

/*---------------------------------------------------------------------------*
  Name:         StartWirelessParent

  Description:  データシェアリング親機モードを起動.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    StartWirelessParent(void);

/*---------------------------------------------------------------------------*
  Name:         StartWirelessChild

  Description:  データシェアリング子機モードを起動.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    StartWirelessChild(void);

/*---------------------------------------------------------------------------*
  Name:         ExecuteDataSharing

  Description:  データシェアリングのメイン処理.
                バックグラウンドで WFS 処理.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    ExecuteDataSharing(void);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // WM_DEMO_WIRELESS_ALL_COMMON_H_
