/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FS - demos - overlay-staticinit
  File:     mode.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mode.h,v $
  Revision 1.3  2006/01/18 02:11:20  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:06  yosizaki
  do-indent.

  Revision 1.1  2005/01/19 07:44:53  yosizaki
  initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#if	!defined(NITRO_DEMOS_FS_OVERLAY_STATICINIT_MODE_H_)
#define NITRO_DEMOS_FS_OVERLAY_STATICINIT_MODE_H_


#include <nitro.h>


/*
 * 各モード用オーバーレイのインタフェースを定義します.
 * オーバーレイの static initializer でこれらを自己設定します.
 */


#if	defined(__cplusplus)
extern  "C"
{
#endif


/********************************************************************/
/* structure */

/* 入力情報構造体 */
    typedef struct
    {
        TPData  tp;
        u16     push_bits;             /* 押された瞬間のキービット */
        u16     hold_bits;             /* 押されている状態のキービット */
        u16     release_bits;          /* 離された瞬間のキービット */
        u16     reserved;
    }
    InputData;


/********************************************************************/
/* constant */

/* タッチペン接触状態を示す独自拡張ビット */
#define PAD_BUTTON_TP   0x4000


/********************************************************************/
/* function */

/* 指定のキーに対する状態判定マクロ */
#define	IS_INPUT_(input, key, action)	\
	(((input).action ## _bits & (key)) != 0)


/*---------------------------------------------------------------------------*
  Name:         UpdateFrame

  Description:  現在のモードで 1 フレーム分だけ内部状態を更新します.

  Arguments:    frame_count      現在の動作フレームカウント.
                input            入力情報の配列.
                player_count     現在のプレーヤー総数. (input の有効要素数)
                own_player_id    自身のプレーヤー番号.

  Returns:      現在のモードが今回のフレームで終了するならば FALSE を,
                そうでなければ TRUE を返します.
 *---------------------------------------------------------------------------*/
    extern BOOL (*UpdateFrame) (int frame_count,
                                const InputData * input, int player_count, int own_player_id);

/*---------------------------------------------------------------------------*
  Name:         DrawFrame

  Description:  現在のモードで内部状態をもとに描画更新します.

  Arguments:    frame_count      現在の動作フレームカウント.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    extern void (*DrawFrame) (int frame_count);

/*---------------------------------------------------------------------------*
  Name:         EndFrame

  Description:  現在のモードを終了します.

  Arguments:    p_next_mode      次のモードを明示的に指定する場合は.
                                 このポインタが指す先にIDを上書きします.
                                 特に指定しない場合, 現在のモードを呼び出した
                                 モードが選択されます.

  Returns:      None.
 *---------------------------------------------------------------------------*/
    extern void (*EndFrame) (FSOverlayID *p_next_mode);


#if	defined(__cplusplus)
}                                      /* extern "C" */
#endif


#endif                                 /* NITRO_DEMOS_FS_OVERLAY_STATICINIT_MODE_H_ */
