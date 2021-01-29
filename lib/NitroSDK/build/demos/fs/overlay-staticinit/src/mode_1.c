/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     mode_1.c

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mode_1.c,v $
  Revision 1.2  2005/02/28 05:26:03  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:45  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include	"mode.h"
#include	"draw.h"

/*
 * NitroStaticInit() を static initializer に指定するには
 * このヘッダをインクルードします.
 */
#include <nitro/sinit.h>


/*****************************************************************************/
/* constant */


/*****************************************************************************/
/* variable */


/*****************************************************************************/
/* function */

/*---------------------------------------------------------------------------*
  Name:         MyUpdateFrame

  Description:  現在のモードで 1 フレーム分だけ内部状態を更新します.

  Arguments:    frame_count      現在の動作フレームカウント.
                input            入力情報の配列.
                player_count     現在のプレーヤー総数. (input の有効要素数)
                own_player_id    自身のプレーヤー番号.

  Returns:      現在のモードが今回のフレームで終了するならば FALSE を,
                そうでなければ TRUE を返します.
 *---------------------------------------------------------------------------*/
static BOOL MyUpdateFrame(int frame_count,
                          const InputData * input, int player_count, int own_player_id)
{
    (void)frame_count;
    (void)player_count;

    /* 何かを押したら終了 */
    return !IS_INPUT_(input[own_player_id], ~0, push);
}

/*---------------------------------------------------------------------------*
  Name:         MyDrawFrame

  Description:  現在のモードで内部状態をもとに描画更新します.

  Arguments:    frame_count      現在の動作フレームカウント.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MyDrawFrame(int frame_count)
{
    /* ファイル名と簡易操作説明の表示 */
    SetTextColor(GX_RGB(31, 31, 31));
    DrawText(0, 10, "%s", __FILE__);
    DrawText(30, 40, "press any key to return");
    /* 単にフレームカウントの表示 */
    SetTextColor(GX_RGB(31, 31, 16));
    DrawText(40, 80, "frame count = %d", frame_count);
}

/*---------------------------------------------------------------------------*
  Name:         MyEndFrame

  Description:  現在のモードを終了します.

  Arguments:    p_next_mode      次のモードを明示的に指定する場合は.
                                 このポインタが指す先にIDを上書きします.
                                 特に指定しない場合, 現在のモードを呼び出した
                                 モードが選択されます.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MyEndFrame(FSOverlayID *p_next_mode)
{
    /* 何もせずトップメニューに返る */
    (void)p_next_mode;
}

/*---------------------------------------------------------------------------*
  Name:         NitroStaticInit

  Description:  static initializer としての自動初期化関数.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void NitroStaticInit(void)
{
    UpdateFrame = MyUpdateFrame;
    DrawFrame = MyDrawFrame;
    EndFrame = MyEndFrame;

    /* モードごとに必要な初期化処理をここで行います. */

    SetClearColor(GX_RGB(0, 0, 0));
}
