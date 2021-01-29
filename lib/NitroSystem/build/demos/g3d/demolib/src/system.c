/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib
  File:     system.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/

#include "g3d_demolib/system.h"

#define DEFAULT_DMA_NUMBER      MI_DMA_MAX_NUM
#define SYSTEM_HEAP_SIZE        64*1024


G3DDemoGamePad   G3DDemo_GamePad;       // ゲームパッド
NNSFndHeapHandle G3DDemo_SysHeap;       // システムヒープ　　　　（拡張ヒープを使用）
NNSFndHeapHandle G3DDemo_AppHeap;       // アプリケーションヒープ（拡張ヒープを使用）


/*---------------------------------------------------------------------------*
  Name:         G3DDemo_VBlankIntr

  Description:  Vブランクのコールバックルルーチン。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitSystem

  Description:  NITROの初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitSystem(void)
{
    OS_Init();
    FX_Init();
    GX_SetPower(GX_POWER_ALL);
    GX_Init();
    OS_InitTick();

    GX_DispOff();
    GXS_DispOff();

    OS_SetIrqFunction(OS_IE_V_BLANK, G3DDemo_VBlankIntr);

    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrqMask(OS_IE_FIFO_RECV);
    (void)OS_EnableIrq();

    FS_Init(DEFAULT_DMA_NUMBER);

    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request

    // ゲームパッドを空読みします。
    G3DDemo_ReadGamePad();
}

#define SYSTEM_HEAP_SIZE        64*1024

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitVRAM

  Description:  ＶＲＡＭの初期化を行います。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitVRAM(void)
{
    // 全てのバンクをLCDCに割り当てる
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);

    // LCDC空間全てをクリア
    MI_CpuClearFast( (void*)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE );
    
    // LCDCに割り当てられたバンクをDisable
    (void)GX_DisableBankForLCDC();

    MI_CpuFillFast ((void*)HW_OAM    , 192, HW_OAM_SIZE   );  // OAM クリア
    MI_CpuFillFast ((void*)HW_DB_OAM , 192, HW_DB_OAM_SIZE);  // clear OAM

    MI_CpuClearFast((void*)HW_PLTT   , HW_PLTT_SIZE   );      // パレット クリア
    MI_CpuClearFast((void*)HW_DB_PLTT, HW_DB_PLTT_SIZE);      // clear the standard palette
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitMemory

  Description:  システムヒープとアプリケーションヒープを作成します。両ヒープ
                にはNITRO-Systemの拡張ヒープを使用しています。
                
                システムヒープ用のメモリとして、SYSTEM_HEAP_SIZE分をメインア
                リーナから確保し、メインアリーナの残りを全てアプリケーション
                ヒープ用のメモリに確保しています。
                
                システムヒープは、ゲームシステム等のシステムプログラムで使用
                することを想定しています。アプリケーションヒープには、ゲーム
                で使用するデータをロードする為に使用します。

  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitMemory(void)
{
    void* sysHeapMemory = OS_AllocFromMainArenaLo(SYSTEM_HEAP_SIZE, 16);

    u32   arenaLow      = G3DDEMO_ROUND_UP  (OS_GetMainArenaLo(), 16);
    u32   arenaHigh     = G3DDEMO_ROUND_DOWN(OS_GetMainArenaHi(), 16);
    u32   appHeapSize   = arenaHigh - arenaLow;
    void* appHeapMemory = OS_AllocFromMainArenaLo(appHeapSize, 16);

    G3DDemo_SysHeap = NNS_FndCreateExpHeap(sysHeapMemory, SYSTEM_HEAP_SIZE);
    G3DDemo_AppHeap = NNS_FndCreateExpHeap(appHeapMemory, appHeapSize      );
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_LoadFile

  Description:  ファイルの読み込み
  
  Arguments:    path        ファイルへのパス

  Returns:      ファイル読み込み先アドレス
 *---------------------------------------------------------------------------*/
void* G3DDemo_LoadFile(const char *path)
{
    FSFile file;
    void*  memory;

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path))
    {
        u32 fileSize = FS_GetLength(&file);

        memory = NNS_FndAllocFromExpHeapEx(G3DDemo_AppHeap, fileSize, 16);
        if (memory == NULL)
        {
            OS_Printf("no enough memory.\n");
        }
        else
        {
            if (FS_ReadFile(&file, memory, (s32)fileSize) != fileSize)   // ファイルサイズ分読み込めていない場合
            {
                NNS_FndFreeToExpHeap(G3DDemo_AppHeap, memory);
                memory = NULL;
                OS_Printf("file reading failed.\n");
            }
        }
        (void)FS_CloseFile(&file);
    }
    return memory;
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_ReadGamePad

  Description:  キーを読み込み、トリガとリリーストリガを求めます。
                
  Arguments:    なし。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void G3DDemo_ReadGamePad(void)
{
    u16 status = PAD_Read();

    G3DDemo_GamePad.trigger = (u16)(status                 & (status ^ G3DDemo_GamePad.button));
    G3DDemo_GamePad.release = (u16)(G3DDemo_GamePad.button & (status ^ G3DDemo_GamePad.button));
    G3DDemo_GamePad.button  = status;
}
