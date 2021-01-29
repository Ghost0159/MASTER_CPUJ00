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
  Revision 1.10  2006/01/18 02:11:28  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/21 10:46:54  kitase_hirotake
  SVC_WaitVBlankIntr を OS_WaitVBlankIntr に変更

  Revision 1.8  2005/07/08 12:51:00  seiki_masashi
  MD5 のテストベクターを増強

  Revision 1.7  2005/04/13 11:56:15  seiki_masashi
  GetDigest -> GetHash

  Revision 1.6  2005/04/13 04:54:31  seiki_masashi
  Checksum16 がバイト境界に関係なく使用できるようになったことへの対応
  128k 境界を越えた場合の Checksum の動作テストを追加

  Revision 1.5  2005/04/12 11:14:55  seiki_masashi
  Checksum 値計算関数のテストを追加

  Revision 1.4  2005/04/11 08:25:07  seiki_masashi
  CRC の動作確認を追加

  Revision 1.3  2005/04/01 07:44:53  seiki_masashi
  HMAC 関連関数の変更に対応

  Revision 1.2  2005/03/29 09:30:05  seiki_masashi
  HMAC のテストを追加

  Revision 1.1  2005/03/28 04:25:02  seiki_masashi
  math/dgt-1 デモを追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  MATH ライブラリ Digest 関数群の動作確認デモ
 *---------------------------------------------------------------------------*/

#include    <nitro.h>



static void VBlankIntr(void);
static void DisplayInit(void);
static void FillScreen(u16 col);
static BOOL DigestTest(void);
static BOOL CompareDigests(u8 *ptr1, u8 *ptr2, int n);
static u32 GetStringLength(char *str);

/*---------------------------------------------------------------------------*
    変数定義
 *---------------------------------------------------------------------------*/

static u32 sBigArray[(1024 * 1024 / sizeof(u32))];

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

    DisplayInit();

    if (DigestTest())
    {
        // 成功
        OS_TPrintf("------ Test Succeeded ------\n");
        FillScreen(GX_RGB(0, 31, 0));
    }
    else
    {
        // 失敗
        OS_TPrintf("****** Test Failed ******\n");
        FillScreen(GX_RGB(31, 0, 0));
    }

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
  Name:         FillScreen

  Description:  画面を塗りつぶす

  Arguments:    col - FillColor.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void FillScreen(u16 col)
{
    MI_CpuFill16((void *)HW_LCDC_VRAM_C, col, 256 * 192 * 2);
}

/*---------------------------------------------------------------------------*
  Name:         DigestTest

  Description:  digest 関数のテストルーチン

  Arguments:    None.

  Returns:      テストが成功したら true.
 *---------------------------------------------------------------------------*/
#define PrintResultEq( a, b, f ) \
    { OS_TPrintf( ((a) == (b)) ? "[--OK--] " : "[**NG**] " ); (f) = (f) && ((a) == (b)); }
#define PrintResultDigestEq( a, b, l, f ) \
    { OS_TPrintf( (CompareDigests((a), (b), (l))) ? "[--OK--] " : "[**NG**] " ); (f) = (f) && (CompareDigests((a), (b), (l))); }

static BOOL DigestTest(void)
{
    int     i, j;
    BOOL    flag = TRUE;

    // 8bit Checksum の動作テストを行う
    {
        char   *a[] = {
            "\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00\x01",
            "\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00\x01",
            "\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00\x01",
        };
        u32     offset[] = {
            0,
            2,
            1,
        };
        u32     l[] = {
            8,
            8,
            8,
        };
        u8      result_checksum8[] = {
            0x2f,
            0x2f,
            0x2f,
        };
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHChecksum8Context context;
            u8      result;

            result = MATH_CalcChecksum8(a[i], l[i]);
            PrintResultEq(result, result_checksum8[i], flag);
            OS_TPrintf("MATH_Checksum8[%d] = %02x\n", i, result);

            // Checksum 値を結合して全体の Checksum を取ると 0x0000 になる
            MATH_Checksum8Init(&context);
            MATH_Checksum8Update(&context, &result, sizeof(result));
            MATH_Checksum8Update(&context, a[i], l[i]);
            result = MATH_Checksum8GetHash(&context);
            PrintResultEq(result, 0, flag);
            OS_TPrintf("  Check Value = %02x\n", result);
        }
    }
    {
        u8      a[] = {
            0xff,
            0xff,
            0xff,
        };
        u32     l[] = {
            sizeof(sBigArray) - 2,
            sizeof(sBigArray) - 2,
            sizeof(sBigArray) - 2,
        };
        u32     offset[] = {
            0,
            1,
            2,
        };
        u8      result_checksum8[] = {
            0x00,
            0x00,
            0x00,
        };
        for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        {
            MATHChecksum8Context context;
            u8      result;

            MI_CpuFill8(((u8 *)sBigArray) + offset[i], a[i], l[i]);
            result = MATH_CalcChecksum8(((u8 *)sBigArray) + offset[i], l[i]);
            PrintResultEq(result, result_checksum8[i], flag);
            OS_TPrintf("MATH_Checksum8(Large)[%d] = %02x\n", i, result);

            // Checksum 値を結合して全体の Checksum を取ると 0x0000 になる
            MATH_Checksum8Init(&context);
            MATH_Checksum8Update(&context, &result, sizeof(result));
            MATH_Checksum8Update(&context, ((u8 *)sBigArray) + offset[i], l[i]);
            result = MATH_Checksum8GetHash(&context);
            PrintResultEq(result, 0, flag);
            OS_TPrintf("  Check Value = %02x\n", result);
        }
    }

    // 16bit Checksum (Internet Checksum) の動作テストを行う
    {
        char   *a[] = {
            "\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00\x01",
            "\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00\x01",
            "\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00\x01",
            "\xf7\x00\x01\xf2\x03\xf4\xf5\xf6\xf7\x00",
        };
        u32     offset[] = {
            0,
            2,
            1,
            1,
        };
        u32     l[] = {
            8,
            8,
            8,
            8,
        };
        u16     result_checksum16[] = {
            0x0d22,
            0x0d22,
            0x220d,
            0x0d22,
        };
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHChecksum16Context context;
            u16     result;

            result = MATH_CalcChecksum16(a[i] + offset[i], l[i]);
            PrintResultEq(result, result_checksum16[i], flag);
            OS_TPrintf("MATH_Checksum16[%d] = %04x\n", i, result);

            // Internet Checksum は Checksum 値を結合して
            // 全体の Checksum を取ると 0x0000 になる
            MATH_Checksum16Init(&context);
            MATH_Checksum16Update(&context, &result, sizeof(result));
            MATH_Checksum16Update(&context, a[i] + offset[i], l[i]);
            result = MATH_Checksum16GetHash(&context);
            PrintResultEq(result, 0, flag);
            OS_TPrintf("  Check Value = %04x\n", result);
        }
    }
    {
        u8      a[] = {
            0xff,
            0xff,
            0xff,
        };
        u32     l[] = {
            sizeof(sBigArray) - 2,
            sizeof(sBigArray) - 2,
            sizeof(sBigArray) - 2,
        };
        u32     offset[] = {
            0,
            1,
            2,
        };
        u16     result_checksum16[] = {
            0x0000,
            0x0000,
            0x0000,
        };
        for (i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        {
            MATHChecksum16Context context;
            u16     result;

            MI_CpuFill8(((u8 *)sBigArray) + offset[i], a[i], l[i]);
            result = MATH_CalcChecksum16(((u8 *)sBigArray) + offset[i], l[i]);
            PrintResultEq(result, result_checksum16[i], flag);
            OS_TPrintf("MATH_Checksum16(Large)[%d] = %04x\n", i, result);

            // Checksum 値を結合して全体の Checksum を取ると 0x0000 になる
            MATH_Checksum16Init(&context);
            MATH_Checksum16Update(&context, &result, sizeof(result));
            MATH_Checksum16Update(&context, ((u8 *)sBigArray) + offset[i], l[i]);
            result = MATH_Checksum16GetHash(&context);
            PrintResultEq(result, 0, flag);
            OS_TPrintf("  Check Value = %04x\n", result);
        }
    }

    // CRC-8 の動作テストを行う
    {
        static MATHCRC8Table table_crc8;
        char   *a[] = {
            "123456789",
        };
        u8      result_crc8[] = {
            0xf4,
        };
        MATH_CRC8InitTable(&table_crc8);
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHCRC8Context context;
            u8      result;

            MATH_CRC8Init(&context);
            MATH_CRC8Update(&table_crc8, &context, a[i], GetStringLength(a[i]));
            result = MATH_CRC8GetHash(&context);
            PrintResultEq(result, result_crc8[i], flag);
            OS_TPrintf("MATH_CRC8(%s) = %02x\n", a[i], result);
        }
    }

    // CRC-16 の動作テストを行う
    {
        static MATHCRC16Table table_crc16;
        char   *a[] = {
            "123456789",
        };
        u16     result_crc16[] = {
            0xbb3d,
        };
        MATH_CRC16InitTable(&table_crc16);
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHCRC16Context context;
            u16     result;

            MATH_CRC16Init(&context);
            MATH_CRC16Update(&table_crc16, &context, a[i], GetStringLength(a[i]));
            result = MATH_CRC16GetHash(&context);
            PrintResultEq(result, result_crc16[i], flag);
            OS_TPrintf("MATH_CRC16(%s) = %04x\n", a[i], result);
        }
    }

    // CRC-16/CCITT の動作テストを行う
    {
        static MATHCRC16Table table_crc16_ccitt;
        char   *a[] = {
            "123456789",
        };
        u16     result_crc16_ccitt[] = {
            0x29b1,
        };
        MATH_CRC16CCITTInitTable(&table_crc16_ccitt);
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHCRC16Context context;
            u16     result;

            MATH_CRC16CCITTInit(&context);
            MATH_CRC16CCITTUpdate(&table_crc16_ccitt, &context, a[i], GetStringLength(a[i]));
            result = MATH_CRC16CCITTGetHash(&context);
            PrintResultEq(result, result_crc16_ccitt[i], flag);
            OS_TPrintf("MATH_CRC16CCITT(%s) = %04x\n", a[i], result);
        }
    }

    // CRC-32 の動作テストを行う
    {
        static MATHCRC32Table table_crc32;
        char   *a[] = {
            "123456789",
        };
        u32     result_crc32[] = {
            0xcbf43926,
        };
        MATH_CRC32InitTable(&table_crc32);
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHCRC32Context context;
            u32     result;

            MATH_CRC32Init(&context);
            MATH_CRC32Update(&table_crc32, &context, a[i], GetStringLength(a[i]));
            result = MATH_CRC32GetHash(&context);
            PrintResultEq(result, result_crc32[i], flag);
            OS_TPrintf("MATH_CRC32(%s) = %04x\n", a[i], result);
        }
    }

    // CRC-32/POSIX の動作テストを行う
    {
        static MATHCRC32Table table_crc32_posix;
        char   *a[] = {
            "123456789",
        };
        u32     result_crc32_posix[] = {
            0x765e7680,
        };
        MATH_CRC32POSIXInitTable(&table_crc32_posix);
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHCRC32Context context;
            u32     result;

            MATH_CRC32POSIXInit(&context);
            MATH_CRC32POSIXUpdate(&table_crc32_posix, &context, a[i], GetStringLength(a[i]));
            result = MATH_CRC32POSIXGetHash(&context);
            PrintResultEq(result, result_crc32_posix[i], flag);
            OS_TPrintf("MATH_CRC32POSIX(%s) = %04x\n", a[i], result);
        }
    }

    // MD5 の動作テストを行う
    // RFC1321 の A.5 Test suite を参照
    {
        char   *a[] = {
            "",
            "a",
            "abc",
            "message digest",
            "abcdefghijklmnopqrstuvwxyz",
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
            "1234567890",
            "FHXXKCVGGOFRILWGDLFQLZBLQSNYXWUCTXCBBVHUDWXLSFNIFTBNCGOMLXPQQFFJEOBXFBZNEDWMSKPPEWXONABBPRBHVDNQXPRHQGMLUHJVUPOXYUHDEXYMZBJCFNYZSKMPNLPYADWGXEKPYPKGNHQZMPBMSCYYYUPVPQZSYPPPIGKBQZUWOUCMVLFKORUIZWVWFVBEPZBXASLDAVBEHMJGYPYBQZAKHNLOFCPQVRRGKOVVRNERIFFJZIYAECMNHTRCUWGAKRZXPORLXLRXVRVVJIRTNDDIEFNGWFXBFDPPFOMPFWRGXQUBJYTNCCXKTUGRQYYGRFMDPKTWWFREVAUNHSAWUQQJOPIGRWHJDDOVHZHHGXDKVSDYRGXGCWMKQSENCIMJQOGNGAZFTDXYEVNRDWDKCKPJPIXZKCGJPEGFIJANNPJYHIRLPUBQAJSFMYLVGCPQZRUNKRRWEBLZYYOQUHGEMKGCXPKBWRXRANLRRHLGZSMONYDRIFPHOYSVYKZJRUSOVVUCULTSBOSGKYUVYCVHMIASIBFKGPRCKLVXZAGKCQUKMIHOIQJRBCJBKOYRNACLNHPSDBKUFBXAODNVHVTVVHHZEBAVRVMKBBEOYJSABXUDBJQEFLLYVBPWMSQQVLTGCASWRSHPIDOZRPZUCHYREWFCDZJOMMUGPHPTKWEIJIIGEHNFXKLWIHPKYORNBXESRYZMUNTJDGNQFGAFFRAWCKYRBZGNBZKOWAGWXZCOOEERHWPYIVXHDSZAVYFUBXNRJCVDDUJWWMYYXMCTXANZOPVULYAGPXZUQRBOAWEJKTUKIIDZVBVQRKINSAMSYKZVCINUUWVBVVUGGXLKMVBTFEESRQTNPOEYZRZVJBUEEXIQOXEWLMUUJQUWNOQTQQNJFBDNTCXMNJMJBIOCQZUCKCZFECNAXBXHLTCVURSEBLVAUIJKVGWZETIVGHWOFGRFPLIWQAUMTZOELCXBMVLHBRFZUIEIYWWUY",
            "YEDCGPVKFGWXYSDLTQOYAFHSJYABKIMQHJGDCKUSLRDQFJDGPVWBSBQ",
            "YEDCGPVKFGWXYSDLTQOYAFHSJYABKIMQHJGDCKUSLRDQFJDGPVWBSBQZ",
            "YEDCGPVKFGWXYSDLTQOYAFHSJYABKIMQHJGDCKUSLRDQFJDGPVWBSBQZZ",
        };
        int     c[] = { 1, 1, 1, 1, 1, 1, 8, 1, 2, 2, 2 };
        char   *result_md5[] = {
            "\xD4\x1D\x8C\xD9\x8F\x00\xB2\x04\xE9\x80\x09\x98\xEC\xF8\x42\x7E",
            "\x0C\xC1\x75\xB9\xC0\xF1\xB6\xA8\x31\xC3\x99\xE2\x69\x77\x26\x61",
            "\x90\x01\x50\x98\x3C\xD2\x4F\xB0\xD6\x96\x3F\x7D\x28\xE1\x7F\x72",
            "\xF9\x6B\x69\x7D\x7C\xB7\x93\x8D\x52\x5A\x2F\x31\xAA\xF1\x61\xD0",
            "\xC3\xFC\xD3\xD7\x61\x92\xE4\x00\x7D\xFB\x49\x6C\xCA\x67\xE1\x3B",
            "\xD1\x74\xAB\x98\xD2\x77\xD9\xF5\xA5\x61\x1C\x2C\x9F\x41\x9D\x9F",
            "\x57\xED\xF4\xA2\x2B\xE3\xC9\x55\xAC\x49\xDA\x2E\x21\x07\xB6\x7A",
            "\x72\xcc\x6a\x07\x35\x47\x48\x5d\xe7\x53\xcd\xa5\x90\xcb\x90\xbd",
            "\x5c\x07\x79\xcb\x86\xb1\x89\x2b\xe5\x3b\xdc\x72\x7b\x3b\x9b\x41",
            "\xc8\x69\xd4\x26\xf3\x4f\x78\x1b\xed\xaf\xdc\x9d\x29\xaf\x75\x7f",
            "\xa3\x1b\xd7\x8a\x72\x15\xa7\xc8\x59\x41\x51\xd5\x4b\xc8\x7d\x30",
        };
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHMD5Context context;
            u8      result[MATH_MD5_DIGEST_SIZE];

            MATH_MD5Init(&context);
            for (j = 0; j < c[i]; j++)
            {
                MATH_MD5Update(&context, a[i], GetStringLength(a[i]));
            }
            MATH_MD5GetHash(&context, result);
            PrintResultDigestEq(result, (u8 *)result_md5[i], sizeof(result), flag);
            OS_TPrintf("MATH_MD5(%s) * %d\n", a[i], c[i]);
        }
    }

    // HMAC-MD5 の動作テストを行う
    // RFC2202 を参照
    {
        char   *d[] = {
            "Hi There",
            "what do ya want for nothing?",
            "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd",
            "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd",
            "Test With Truncation",    // このテストプログラムでは truncation は行いません
            "Test Using Larger Than Block-Size Key - Hash Key First",
            "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data",
        };
        u32     dl[] = { 8, 28, 50, 50, 20, 54, 73 };
        char   *k[] = {
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
            "Jefe",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
            "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19",
            "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
        };
        u32     kl[] = { 16, 4, 16, 25, 16, 80, 80 };
        char   *result_hmacmd5[] = {
            "\x92\x94\x72\x7a\x36\x38\xbb\x1c\x13\xf4\x8e\xf8\x15\x8b\xfc\x9d",
            "\x75\x0c\x78\x3e\x6a\xb0\xb5\x03\xea\xa8\x6e\x31\x0a\x5d\xb7\x38",
            "\x56\xbe\x34\x52\x1d\x14\x4c\x88\xdb\xb8\xc7\x33\xf0\xe8\xb3\xf6",
            "\x69\x7e\xaf\x0a\xca\x3a\x3a\xea\x3a\x75\x16\x47\x46\xff\xaa\x79",
            "\x56\x46\x1e\xf2\x34\x2e\xdc\x00\xf9\xba\xb9\x95\x69\x0e\xfd\x4c",
            "\x6b\x1a\xb7\xfe\x4b\xd7\xbf\x8f\x0b\x62\xe6\xce\x61\xb9\xd0\xcd",
            "\x6f\x63\x0f\xad\x67\xcd\xa0\xee\x1f\xb1\xf5\x62\xdb\x3a\xa5\x3e",
        };
        for (i = 0; i < sizeof(d) / sizeof(char *); i++)
        {
            u8      result[MATH_MD5_DIGEST_SIZE];

            MATH_CalcHMACMD5(result, d[i], dl[i], k[i], kl[i]);
            PrintResultDigestEq(result, (u8 *)result_hmacmd5[i], sizeof(result), flag);
            OS_TPrintf("MATH_MD5CalcHmac: Test Case %d\n", i + 1);
        }
    }

    // SHA-1 の動作テストを行う
    // RFC3174 の 7.3 Test Driver を参照
    {
        char   *a[] = {
            "abc",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "a",
            "0123456701234567012345670123456701234567012345670123456701234567",
        };
        int     c[] = { 1, 1, 1000000, 10 };
        char   *result_sha1[] = {
            "\xA9\x99\x3E\x36\x47\x06\x81\x6A\xBA\x3E\x25\x71\x78\x50\xC2\x6C\x9C\xD0\xD8\x9D",
            "\x84\x98\x3E\x44\x1C\x3B\xD2\x6E\xBA\xAE\x4A\xA1\xF9\x51\x29\xE5\xE5\x46\x70\xF1",
            "\x34\xAA\x97\x3C\xD4\xC4\xDA\xA4\xF6\x1E\xEB\x2B\xDB\xAD\x27\x31\x65\x34\x01\x6F",
            "\xDE\xA3\x56\xA2\xCD\xDD\x90\xC7\xA7\xEC\xED\xC5\xEB\xB5\x63\x93\x4F\x46\x04\x52",
        };
        for (i = 0; i < sizeof(a) / sizeof(char *); i++)
        {
            MATHSHA1Context context;
            u8      result[MATH_SHA1_DIGEST_SIZE];

            MATH_SHA1Init(&context);
            for (j = 0; j < c[i]; j++)
            {
                MATH_SHA1Update(&context, a[i], GetStringLength(a[i]));
            }
            MATH_SHA1GetHash(&context, result);
            PrintResultDigestEq(result, (u8 *)result_sha1[i], sizeof(result), flag);
            OS_TPrintf("MATH_SHA1(%s) * %d\n", a[i], c[i]);
        }
    }

    // HMAC-SHA-1 の動作テストを行う
    // RFC2202 を参照
    {
        char   *d[] = {
            "Hi There",
            "what do ya want for nothing?",
            "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
                "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd",
            "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
                "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd",
            "Test With Truncation",    // このテストプログラムでは truncation は行いません
            "Test Using Larger Than Block-Size Key - Hash Key First",
            "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data",
        };
        u32     dl[] = { 8, 28, 50, 50, 20, 54, 73 };
        char   *k[] = {
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
            "Jefe",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
            "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19",
            "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
            "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
                "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
        };
        u32     kl[] = { 20, 4, 20, 25, 20, 80, 80 };
        char   *result_hmacsha1[] = {
            "\xb6\x17\x31\x86\x55\x05\x72\x64\xe2\x8b\xc0\xb6\xfb\x37\x8c\x8e\xf1\x46\xbe\x00",
            "\xef\xfc\xdf\x6a\xe5\xeb\x2f\xa2\xd2\x74\x16\xd5\xf1\x84\xdf\x9c\x25\x9a\x7c\x79",
            "\x12\x5d\x73\x42\xb9\xac\x11\xcd\x91\xa3\x9a\xf4\x8a\xa1\x7b\x4f\x63\xf1\x75\xd3",
            "\x4c\x90\x07\xf4\x02\x62\x50\xc6\xbc\x84\x14\xf9\xbf\x50\xc8\x6c\x2d\x72\x35\xda",
            "\x4c\x1a\x03\x42\x4b\x55\xe0\x7f\xe7\xf2\x7b\xe1\xd5\x8b\xb9\x32\x4a\x9a\x5a\x04",
            "\xaa\x4a\xe5\xe1\x52\x72\xd0\x0e\x95\x70\x56\x37\xce\x8a\x3b\x55\xed\x40\x21\x12",
            "\xe8\xe9\x9d\x0f\x45\x23\x7d\x78\x6d\x6b\xba\xa7\x96\x5c\x78\x08\xbb\xff\x1a\x91",
        };
        for (i = 0; i < sizeof(d) / sizeof(char *); i++)
        {
            u8      result[MATH_SHA1_DIGEST_SIZE];

            MATH_CalcHMACSHA1(result, d[i], dl[i], k[i], kl[i]);
            PrintResultDigestEq(result, (u8 *)result_hmacsha1[i], sizeof(result), flag);
            OS_TPrintf("MATH_SHA1CalcHmac: Test Case %d\n", i + 1);
        }
    }




    return flag;
}


/*---------------------------------------------------------------------------*
  Name:         CompareDigests

  Description:  メモリ内容比較

  Arguments:    ptr1, ptr2 - 比較するメモリ位置.
                n - 比較する長さ

  Returns:      一致したら TRUE.
 *---------------------------------------------------------------------------*/
static BOOL CompareDigests(u8 *ptr1, u8 *ptr2, int n)
{
    int     i;
    for (i = 0; i < n; i++)
    {
        if (*(ptr1++) != *(ptr2++))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         GetStringLength

  Description:  文字列長を得る

  Arguments:    str - 文字列へのポインタ.

  Returns:      文字列長.
 *---------------------------------------------------------------------------*/
static u32 GetStringLength(char *str)
{
    u32     i;
    for (i = 0;; i++)
    {
        if (*(str++) == '\0')
        {
            return i;
        }
    }
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
