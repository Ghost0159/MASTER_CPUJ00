/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - bg - BgEx
  File:     loader.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/
#ifndef LOAD_H_
#define LOAD_H_

#include <nnsys/fnd.h>
#include <nnsys/g2d.h>

#ifdef __cplusplus
extern "C" {
#endif



/*---------------------------------------------------------------------------*
  Name:         LoadN**R

  Description:  G2Dバイナリファイルをメモリにロードし、
                ランタイム構造体を構築します。

  Arguments:    ppCharData: キャラクタデータへのポインタを格納するバッファへの
                            ポインタ。
                ppPltData:  パレットデータへのポインタを格納するバッファへの
                            ポインタ。
                ppScrData:  スクリーンデータへのポインタを格納するバッファへの
                            ポインタ。
                pFname:     G2D バイナリファイルのファイル名。
                pAllocator: メモリアロケータへのポインタ。

  Returns:      関数が成功した場合は pAllocator で確保したメモリ領域先頭への
                ポインタを返します。
                関数が失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* LoadNCGR(
    NNSG2dCharacterData** ppCharData,
    const char* pFname,
    NNSFndAllocator* pAllocator
);
void* LoadNCLR(
    NNSG2dPaletteData** ppPltData,
    const char* pFname,
    NNSFndAllocator* pAllocator
);
void* LoadNSCR(
    NNSG2dScreenData** ppScrData,
    const char* pFname,
    NNSFndAllocator* pAllocator
);


/*---------------------------------------------------------------------------*
  Name:         LoadNCGREx

  Description:  部分キャラクタのG2Dバイナリファイルをメモリにロードし、
                ランタイム構造体を構築します。

  Arguments:    ppCharData: キャラクタデータへのポインタを格納するバッファへの
                            ポインタ。
                ppPosInfo:  部分キャラクタ位置情報データへのポインタを格納する
                            バッファへのポインタ。
                pFname:     部分キャラクタの G2D バイナリファイルのファイル名。
                pAllocator: メモリアロケータへのポインタ。

  Returns:      関数が成功した場合は pAllocator で確保したメモリ領域先頭への
                ポインタを返します。
                関数が失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* LoadNCGREx(
    NNSG2dCharacterData** ppCharData,
    NNSG2dCharacterPosInfo** ppPosInfo,
    const char* pFname,
    NNSFndAllocator* pAllocator
);

/*---------------------------------------------------------------------------*
  Name:         LoadNCLREx

  Description:  部分キャラクタのG2Dバイナリファイルをメモリにロードし、
                ランタイム構造体を構築します。

  Arguments:    ppCharData: パレットデータへのポインタを格納するバッファへの
                            ポインタ。
                ppPosInfo:  パレット圧縮データへのポインタを格納する
                            バッファへのポインタ。
                pFname:     圧縮パレットの G2D バイナリファイルのファイル名。
                pAllocator: メモリアロケータへのポインタ。

  Returns:      関数が成功した場合は pAllocator で確保したメモリ領域先頭への
                ポインタを返します。
                関数が失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* LoadNCLREx(
    NNSG2dPaletteData** ppPltData,
    NNSG2dPaletteCompressInfo** ppCmpInfo,
    const char* pFname,
    NNSFndAllocator* pAllocator
);


#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // LOAD_H_

