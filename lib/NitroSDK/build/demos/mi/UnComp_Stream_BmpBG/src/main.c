/*---------------------------------------------------------------------------*
  Project:  NitroSDK - demos - MI - UnComp_Stream_BmpBG
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.16  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.15  2005/11/21 10:53:27  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.14  2005/06/27 11:10:12  yosizaki
  add comment about DC_WaitWriteBufferEmpty().

  Revision 1.13  2005/02/28 05:26:19  yosizaki
  do-indent.

  Revision 1.12  2005/02/18 13:47:49  seiki_masashi
  warning 対策

  Revision 1.11  2005/01/18 00:20:07  takano_makoto
  fix Copyright header.

  Revision 1.10  2005/01/14 06:03:23  takano_makoto
  無駄なキャッシュクリアを削除

  Revision 1.9  2004/12/08 05:28:01  takano_makoto
  展開用バッファを32バイトアラインに揃える。

  Revision 1.8  2004/12/08 04:29:00  takano_makoto
  ファイルの非同期リードのために読み込み位置を512バイトアラインに揃える

  Revision 1.7  2004/12/07 12:29:31  takano_makoto
  キャッシュをフラッシュするように修正

  Revision 1.6  2004/12/01 08:11:12  takano_makoto
  CARDを非同期リードするサンプルを追加

  Revision 1.5  2004/12/01 00:02:27  takano_makoto
  関数名を MI_UncompReadXX -> MI_ReadUncompXXへ変更

  Revision 1.4  2004/11/30 14:07:39  takano_makoto
  直接VRAMへ展開するサンプルを追加。

  Revision 1.3  2004/11/30 08:36:04  takano_makoto
  MI_InitUncompContextHuff -> MI_InitUncompContextHuffmanに名称変更

  Revision 1.2  2004/11/30 08:01:37  takano_makoto
  small fix in comment.

  Revision 1.1  2004/11/30 07:59:58  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// A sample that displays image uncompressed by streaming:
//
// If display mode is VRAM display mode, a bitmap image on VRAM is displayed.
// This sample loads different four images compressed different format to VRAM A-D,
// and displays them by turns.
//
//---------------------------------------------------------------------------

#include <nitro.h>
#include <nitro/mi/uncomp_stream.h>
#include "DEMO.h"

#define TEMP_BUF_SIZE   512

static u8 *data_uncomp_buf;
static u8 doubleBuf[2][TEMP_BUF_SIZE] ATTRIBUTE_ALIGN(32);

static void InitAlloc(void)
{
    void   *tempLo;
    OSHeapHandle hh;

    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    hh = OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}


void NitroMain(void)
{
    int     vram_slot = 0, count = 0;
    u8     *tmpBuf = doubleBuf[0];

    //---------------------------------------------------------------------------
    // Initialize:
    // They enable IRQ interrupts, and initialize VRAM.
    //---------------------------------------------------------------------------
    DEMOInitCommon();
    DEMOInitVRAM();
    InitAlloc();
    FS_Init(3);
    OS_InitTick();

    //---------------------------------------------------------------------------
    // Map VRAM bank A-D onto LCDC.
    //---------------------------------------------------------------------------
    GX_SetBankForLCDC(GX_VRAM_LCDC_A | GX_VRAM_LCDC_B | GX_VRAM_LCDC_C | GX_VRAM_LCDC_D);

    //---------------------------------------------------------------------------
    // Download images
    //---------------------------------------------------------------------------

    //===========================
    // 無圧縮データ
    //===========================
    {
        FSFile  file;
        u32     read_len;

        // ファイルのオープン
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image1.bin"))
        {
            OS_TPrintf(" Open File /data/image1.bin fail\n");
            return;
        }
        // データ読み込み領域の確保
        read_len = FS_GetLength(&file);
        data_uncomp_buf = (u8 *)OS_Alloc(read_len);

        // ファイル転送
        (void)FS_ReadFile(&file, data_uncomp_buf, 256 * 192 * 2);

        // VRAMに転送
        MI_DmaCopy32(3, data_uncomp_buf, (void *)HW_LCDC_VRAM_A,
                     256 * 192 * sizeof(unsigned short));

        OS_Free(data_uncomp_buf);
        (void)FS_CloseFile(&file);

    }

    //===========================
    // ランレングス圧縮データ
    //===========================
    {
        FSFile  file;
        MIUncompContextRL context;
        s32     read_len;
        u32     data_size;

        // ファイルのオープン
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image2_RL.bin"))
        {
            OS_TPrintf(" Open File /data/image2_RL.bin fail\n");
            return;
        }

        // 圧縮データのヘッダ部分を取得
        (void)FS_ReadFile(&file, tmpBuf, sizeof(MICompressionHeader));

        // データ展開領域の確保
        data_size = MI_GetUncompressedSize(tmpBuf);
        data_uncomp_buf = (u8 *)OS_Alloc(data_size);

        // コンテキストの初期化
        MI_InitUncompContextRL(&context, data_uncomp_buf, (MICompressionHeader *)tmpBuf);

        while (1)
        {
            // 任意のサイズを読み込みストリーミング展開を行ないます。
            read_len = FS_ReadFile(&file, tmpBuf, TEMP_BUF_SIZE);
            if (read_len <= 0)
            {
                OS_TPanic("can't read file\n");
            }
            // メモリ上に展開する場合は高速な8bitアクセス関数を使用します。
            if (MI_ReadUncompRL8(&context, tmpBuf, (u32)read_len) == 0)
            {
                break;
            }
        }

        // VRAMに転送
        DC_FlushRange(data_uncomp_buf, data_size);      // キャッシュをフラッシュ
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        MI_DmaCopy32(3, data_uncomp_buf, (void *)HW_LCDC_VRAM_B,
                     256 * 192 * sizeof(unsigned short));

        OS_Free(data_uncomp_buf);
        (void)FS_CloseFile(&file);

    }

    //===========================
    // LZ77圧縮データ - VRAMへ直接展開するサンプル
    //===========================
    {
        FSFile  file;
        MIUncompContextLZ context;
        s32     read_len;

        // ファイルのオープン
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image3_LZ.bin"))
        {
            OS_TPrintf(" Open File /data/image3_LZ.bin fail\n");
            return;
        }

        // 圧縮データのヘッダ部分を取得
        (void)FS_ReadFile(&file, tmpBuf, sizeof(MICompressionHeader));

        // VRAMへ直接展開する場合のコンテキストの初期化、destアドレスにVRAMを指定
        MI_InitUncompContextLZ(&context, (void *)HW_LCDC_VRAM_C, (MICompressionHeader *)tmpBuf);

        while (1)
        {
            // 任意のサイズを読み込みストリーミング展開を行なう
            read_len = FS_ReadFile(&file, tmpBuf, TEMP_BUF_SIZE);
            if (read_len <= 0)
            {
                OS_TPanic("can't read file\n");
            }

            // VRAMへ直接展開する場合は16bitアクセス関数を使用する。
            if (MI_ReadUncompLZ16(&context, tmpBuf, (u32)read_len) == 0)
            {
                break;
            }
        }

        (void)FS_CloseFile(&file);
    }

    //===========================
    // ハフマン圧縮データ - カードからデータを非同期リードしながら並行して展開を行なうサンプル
    //===========================
    {
        FSFile  file;
        MIUncompContextHuffman context;
        u32     data_size;
        s32     read_len[2];
        u8      read_select;
        u8      uncomp_select;

        // ファイルのオープン
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, "/data/image4_HUFF.bin"))
        {
            OS_TPrintf(" Open File /data/image4_HUFF.bin fail\n");
            return;
        }

        // 圧縮データのヘッダ部分を取得
        (void)FS_ReadFile(&file, tmpBuf, sizeof(MICompressionHeader));

        // データ展開領域の確保
        data_size = MI_GetUncompressedSize(tmpBuf);
        data_uncomp_buf = (u8 *)OS_Alloc(data_size);

        // コンテキストの初期化
        MI_InitUncompContextHuffman(&context, data_uncomp_buf, (MICompressionHeader *)tmpBuf);

        read_select = 0;
        uncomp_select = 1;
        read_len[0] = 0;
        read_len[1] = 0;

        // ファイルの非同期リードを行なうために、読み込み位置を512Byteアラインに合わせる必要があります。
        // ヘッダ部分の4Byteを先行して読み込んでいるので、残り 512 - 4Byte読み込んでおきます。
        read_len[read_select] =
            FS_ReadFileAsync(&file, doubleBuf[read_select], 512 - sizeof(MICompressionHeader));

        /* カードを非同期にリードしながら並列して展開を行ないます */
        while (1)
        {
            // ダブルバッファの切り替え
            read_select ^= 0x1;
            uncomp_select ^= 0x1;

            // カードの読み込み完了を待ちます。
            (void)FS_WaitAsync(&file);

            // 任意のサイズを読み込みストリーミング展開を行ないます。
            read_len[read_select] = FS_ReadFileAsync(&file, doubleBuf[read_select], TEMP_BUF_SIZE);

            if (read_len[uncomp_select] == 0)
            {
                continue;
            }
            if (read_len[uncomp_select] == -1)
            {
                OS_TPanic("can't read file\n");
            }

            if (MI_ReadUncompHuffman
                (&context, doubleBuf[uncomp_select], (u32)read_len[uncomp_select]) == 0)
            {
                // データ展開完了
                break;
            }
        }

        // VRAMに転送
        DC_FlushRange(data_uncomp_buf, data_size);      // キャッシュをフラッシュ
        /* DMA操作でIOレジスタへアクセスするのでキャッシュの Wait は不要 */
        // DC_WaitWriteBufferEmpty();
        MI_DmaCopy32(3, data_uncomp_buf, (void *)HW_LCDC_VRAM_D,
                     256 * 192 * sizeof(unsigned short));

        OS_Free(data_uncomp_buf);
        (void)FS_CloseFile(&file);
    }

    //---------------------------------------------------------------------------
    // Set graphics mode VRAM display mode
    //---------------------------------------------------------------------------
    GX_SetGraphicsMode(GX_DISPMODE_VRAM_A,      // display VRAM-A
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    DEMOStartDisplay();
    while (1)
    {

#ifdef SDK_AUTOTEST
        EXT_TestScreenShot(100, 0xEFA8B182);
        EXT_TestTickCounter();
#endif //SDK_AUTOTEST

        OS_WaitVBlankIntr();           // Waiting the end of VBlank interrupt

        //---------------------------------------------------------------------------
        // Change the VRAM slot displayed every 90 frames
        //---------------------------------------------------------------------------
        if (count++ > 90)
        {
            vram_slot++;
            vram_slot &= 0x03;
            switch (vram_slot)
            {
            case 0:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_A,  // display VRAM-A
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 1:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_B,  // display VRAM-B
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 2:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,  // display VRAM-C
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            case 3:
                GX_SetGraphicsMode(GX_DISPMODE_VRAM_D,  // display VRAM-D
                                   (GXBGMode)0, // dummy
                                   (GXBG0As)0); // dummy
                break;
            }
            // reset a counter
            count = 0;
        }
    }
}

//---------------------------------------------------------------------------
// VBlank interrupt function:
//
// Interrupt handlers are registered on the interrupt table by OS_SetIRQFunction.
// OS_EnableIrqMask selects IRQ interrupts to enable, and
// OS_EnableIrq enables IRQ interrupts.
// Notice that you have to call 'OS_SetIrqCheckFlag' to check a VBlank interrupt.
//---------------------------------------------------------------------------
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK); // checking VBlank interrupt
}
