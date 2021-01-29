/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - basic - nitro
  File:     nns_util.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

#include "nns_util.h"
#include <nnsys/misc.h>

GamePad          gGamePad;      // ゲームパッド
NNSFndHeapHandle gAppHeap;      // アプリケーションヒープ（拡張ヒープを使用）


/*---------------------------------------------------------------------------*
  Name:         InitMemory

  Description:  アプリケーションヒープを作成します。このヒープは
                NITRO-Systemの拡張ヒープを使用しています。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
InitMemory(void)
{
    u32   arenaLow      = ROUND_UP  (OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = ROUND_DOWN(OS_GetMainArenaHi(), 16);
    u32   appHeapSize   = arenaHigh - arenaLow;
    void* appHeapMemory = OS_AllocFromMainArenaLo(appHeapSize, 16);

    gAppHeap = NNS_FndCreateExpHeap(appHeapMemory, appHeapSize     );
}


/*---------------------------------------------------------------------------*
  Name:         LoadFile

  Description:  バッファを確保してファイルを読み込みます。

  Arguments:    filename:  読み込むファイル名

  Returns:      読み込みに成功するとファイルの内容を格納しているバッファへの
                ポインタを返します。このバッファは不必要になったときに
                NNS_FndFreeToExpHeap で開放しなければなりません。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
static void*
LoadFile(const char* filename)
{
    FSFile  file;
    void*   dataBuf = NULL;
    BOOL    bSuccess;

    SDK_NULL_ASSERT(filename);

    FS_InitFile(&file);
    bSuccess = FS_OpenFile(&file, filename);

    if(bSuccess)
    {
        const u32 fileSize = FS_GetLength(&file);

        dataBuf = NNS_FndAllocFromExpHeapEx(gAppHeap, fileSize, 16);
        NNS_NULL_ASSERT(dataBuf);

        if(fileSize != FS_ReadFile(&file, dataBuf, (s32)fileSize))
        {
            NNS_FndFreeToExpHeap(gAppHeap, dataBuf);
            dataBuf = NULL;
        }
        else
        {
            DC_FlushRange(dataBuf, fileSize);
        }

        bSuccess = FS_CloseFile(&file);
        NNS_ASSERT(bSuccess);
    }else{
        OS_Warning("Can't find the file : %s ", filename);
    }

    return dataBuf;
}

/*---------------------------------------------------------------------------*
  Name:         LoadPicture

  Description:  ファイルを読み込みキャラクタ、カラーパレット、スクリーンの
                各データをVRAMに転送します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
LoadPicture(void)
{
    void* chrData = LoadFile("/data/dog256.chr");
    void* scnData = LoadFile("/data/dog256.scn");
    void* palData = LoadFile("/data/dog256.pal");

    // データをVRAMのロード。
    GX_LoadBG0Char(chrData, 0, 0x10000);
    GX_LoadBG0Scr (scnData, 0, 0x00800);
    GX_LoadBGPltt (palData, 0, 0x00200);
}


/*---------------------------------------------------------------------------*
  Name:         ReadGamePad

  Description:  キーを読み込み、トリガとリリーストリガを求めます。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void ReadGamePad(void)
{
    u16 status = PAD_Read();

    gGamePad.trigger = (u16)(status          & (status ^ gGamePad.button));
    gGamePad.release = (u16)(gGamePad.button & (status ^ gGamePad.button));
    gGamePad.button  = status;
}
