/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - demos
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.7  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.6  2005/11/21 10:47:04  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.5  2005/04/19 07:14:37  seiki_masashi
  小さなテーブルを使った CRC-32 の参考実装を追加

  Revision 1.4  2005/04/19 06:50:34  seiki_masashi
  比較のため、小さなテーブルを使った CRC-16 の参考実装を追加

  Revision 1.3  2005/04/13 04:54:54  seiki_masashi
  Checksum16 がバイト境界に関係なく使用できるようになったことへの対応

  Revision 1.2  2005/04/12 11:15:02  seiki_masashi
  Checksum 値計算関数のテストを追加

  Revision 1.1  2005/04/11 10:33:36  seiki_masashi
  dgt-2 デモの追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  MATH ライブラリ Digest 関数群の動作確認デモ
  速度比較を行う
 *---------------------------------------------------------------------------*/

#include    <nitro.h>



static void VBlankIntr(void);
static void DisplayInit(void);
static void CompareDigestSpeed(void);
static u16 CalcCRC16(void *datap, u32 size);
static u32 CalcCRC32(void *datap, u32 size);

/*---------------------------------------------------------------------------*
    変数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    関数定義
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  初期化及びメインループ。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    // 各種初期化
    OS_Init();
    OS_InitTick();

    DisplayInit();

    CompareDigestSpeed();

    // メインループ
    while (TRUE)
    {
        // Ｖブランク待ち
        OS_WaitVBlankIntr();
    }
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  Ｖブランク割込みベクトル。

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // IRQ チェックフラグをセット
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         DisplayInit

  Description:  Graphics Initialization

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void DisplayInit(void)
{

    GX_Init();
    FX_Init();

    GX_DispOff();
    GXS_DispOff();

    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);

    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);         // to generate VBlank interrupt request
    (void)OS_EnableIrq();


    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);

    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   // clear OAM
    MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     // clear the standard palette

    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
    MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
    MI_DmaFill32(3, (void *)HW_LCDC_VRAM_C, 0x7FFF7FFF, 256 * 192 * sizeof(u16));


    GX_SetBankForOBJ(GX_VRAM_OBJ_256_AB);       // Set VRAM-A,B for OBJ

    GX_SetGraphicsMode(GX_DISPMODE_VRAM_C,      // VRAM mode.
                       (GXBGMode)0,    // dummy
                       (GXBG0As)0);    // dummy

    GX_SetVisiblePlane(GX_PLANEMASK_OBJ);       // make OBJ visible
    GX_SetOBJVRamModeBmp(GX_OBJVRAMMODE_BMP_1D_128K);   // 2D mapping OBJ

    OS_WaitVBlankIntr();               // Waiting the end of VBlank interrupt
    GX_DispOn();

}


/*---------------------------------------------------------------------------*
  Name:         CompareDigestSpeed

  Description:  digest 関数のテストルーチン

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
#define DATA_SIZE (1024*1024)
static void CompareDigestSpeed(void)
{
    static u8 data[DATA_SIZE] ATTRIBUTE_ALIGN(32);
    static MATHCRC8Table table_crc8;
    static MATHCRC16Table table_crc16, table_crc16_ccitt;
    static MATHCRC32Table table_crc32, table_crc32_posix;

    int     i;
    MATHRandContext32 rand;
    OSTick  start, end;
    u8      digest[20];

    OS_TPrintf("Initialize Array...\n");
    // data の初期化
    MATH_InitRand32(&rand, 0);
    for (i = 0; i < DATA_SIZE; i++)
    {
        data[i] = (u8)MATH_Rand32(&rand, 0x100);
    }

    OS_TPrintf("Initialize CRC Tables...\n");
    // table の初期化
    MATH_CRC8InitTable(&table_crc8);
    MATH_CRC16InitTable(&table_crc16);
    MATH_CRC16CCITTInitTable(&table_crc16_ccitt);
    MATH_CRC32InitTable(&table_crc32);
    MATH_CRC32POSIXInitTable(&table_crc32_posix);

    OS_TPrintf("Start Comparing...\n");

    OS_TPrintf("Checksum8:\t");
    start = OS_GetTick();
    (void)MATH_CalcChecksum8(data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("Checksum16:\t");
    start = OS_GetTick();
    (void)MATH_CalcChecksum16(data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-8:\t\t");
    start = OS_GetTick();
    (void)MATH_CalcCRC8(&table_crc8, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-16:\t\t");
    start = OS_GetTick();
    (void)MATH_CalcCRC16(&table_crc16, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-16/CCITT:\t");
    start = OS_GetTick();
    (void)MATH_CalcCRC16CCITT(&table_crc16_ccitt, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-32:\t\t");
    start = OS_GetTick();
    (void)MATH_CalcCRC32(&table_crc32, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-32/POSIX:\t");
    start = OS_GetTick();
    (void)MATH_CalcCRC32POSIX(&table_crc32_posix, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("MD5:\t\t");
    start = OS_GetTick();
    MATH_CalcMD5(digest, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("SHA-1:\t\t");
    start = OS_GetTick();
    MATH_CalcSHA1(digest, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("HMAC-MD5:\t");
    start = OS_GetTick();
    MATH_CalcHMACMD5(digest, data, sizeof(data), data, 32);
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("HMAC-SHA-1:\t");
    start = OS_GetTick();
    MATH_CalcHMACSHA1(digest, data, sizeof(data), data, 32);
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-16(SVC):\t");
    start = OS_GetTick();
    (void)SVC_GetCRC16(0, data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    // 小さいテーブルを使った参考実装
    OS_TPrintf("CRC-16(4bit):\t");
    start = OS_GetTick();
    (void)CalcCRC16(data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("CRC-32(4bit):\t");
    start = OS_GetTick();
    (void)CalcCRC32(data, sizeof(data));
    end = OS_GetTick();
    OS_Printf("%lld us/MB\n", OS_TicksToMicroSeconds(end - start));

    OS_TPrintf("Done.\n");

    return;
}

// 参考実装
/*---------------------------------------------------------------------------*
  Name:         CalcCRC16

  Description:  4bit 単位で CRC-16 を計算する。
                テーブルサイズが 32 バイトと小さい代わりに、
                MATH_CalcCRC16 関数よりも 1.5 倍程度遅い。

  Arguments:    datap - データへのポインタ。
                size  - 計算するデータのサイズ。

  Returns:      u16   - CRC計算結果。
 *---------------------------------------------------------------------------*/
static u16 CalcCRC16(void *datap, u32 size)
{
    // CRC-16 算出用テーブル (4bit 単位での計算用)
    // MATH_CRC16InitTable 関数で計算したテーブルの
    // 0, 0x10, 0x20, ..., 0xf0 番目の数値を抜き出したものと等しい。
    // crc16_table を u32 の配列として定義すると、さらに速度が向上する。
    static const u16 crc16_table[16] = {
        0x0000, 0xCC01, 0xD801, 0x1400,
        0xF001, 0x3C00, 0x2800, 0xE401,
        0xA001, 0x6C00, 0x7800, 0xB401,
        0x5000, 0x9C01, 0x8801, 0x4400,
    };

    u32     r = 0;                     // CRC の初期値
    u8     *p = (u8 *)datap;

    while (size--)
    {
        u32     data = *p;
        p++;

        // 4bit 単位での CRC 計算
        r = (r >> 4) ^ crc16_table[(r ^ data) & 0xf];
        data >>= 4;

        r = (r >> 4) ^ crc16_table[(r ^ data) & 0xf];
    }

    return (u16)r;
}

/*---------------------------------------------------------------------------*
  Name:         CalcCRC32

  Description:  4bit 単位で CRC-32 を計算する。
                テーブルサイズが 64 バイトと小さい代わりに、
                MATH_CalcCRC32 関数よりも 1.5 倍程度遅い。

  Arguments:    datap - データへのポインタ。
                size  - 計算するデータのサイズ。

  Returns:      u32   - CRC計算結果。
 *---------------------------------------------------------------------------*/
static u32 CalcCRC32(void *datap, u32 size)
{
    // CRC-32 算出用テーブル (4bit 単位での計算用)
    // MATH_CRC32InitTable 関数で計算したテーブルの
    // 0, 0x10, 0x20, ..., 0xf0 番目の数値を抜き出したものと等しい。
    static const u32 crc32_table[16] = {
        0x00000000U, 0x1DB71064U, 0x3B6E20C8U, 0x26D930ACU,
        0x76DC4190U, 0x6B6B51F4U, 0x4DB26158U, 0x5005713CU,
        0xEDB88320U, 0xF00F9344U, 0xD6D6A3E8U, 0xCB61B38CU,
        0x9B64C2B0U, 0x86D3D2D4U, 0xA00AE278U, 0xBDBDF21CU,
    };

    u32     r = 0xffffffffU;           // CRC の初期値
    u8     *p = (u8 *)datap;

    while (size--)
    {
        u32     data = *p;
        p++;

        // 4bit 単位での CRC 計算
        r = (r >> 4) ^ crc32_table[(r ^ data) & 0xf];
        data >>= 4;

        r = (r >> 4) ^ crc32_table[(r ^ data) & 0xf];
    }

    return ~r;                         // 出力は not する
}

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
