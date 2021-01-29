/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - dataShare-Model
  File:     unicode.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: unicode.c,v $
  Revision 1.3  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.2  2005/11/25 02:12:41  adachi_hiroaki
  íòçÏå†ï\ãLèCê≥

  
  Revision 1.1  2005/11/21 00:41:10  adachi_hiroaki
  êeã@ëIëâÊñ Ç…éQâ¡êlêîÇ∆ÉjÉbÉNÉlÅ[ÉÄÇï\é¶Ç∑ÇÈÇÊÇ§Ç…ÇµÇΩ

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "unicode.h"

// define data------------------------------------------
#define TBL8140_ELEM_NUM				0xbd    // 8140tblÇÃóvëfêî
#define TBL849f_ELEM_NUM				0x20    // 849ftblÇÃóvëfêî

        // SJISÉRÅ[ÉhîªíËópÇÃíl
#define SJIS_HIGHER_CODE1_MIN			0x81
#define SJIS_HIGHER_CODE1_MAX			0x9f
#define SJIS_HIGHER_CODE2_MIN			0xe0
#define SJIS_HIGHER_CODE2_MAX			0xea

typedef struct SjisUtf16Pare
{
    u16     sjis;
    u16     unicode;
}
SjisUtf16Pare;

// extern data------------------------------------------

// function's prototype declaration---------------------
static u16 SearchUnicodeTable(u16 unicode, SjisUtf16Pare * tblp, int elem_num);

// global variable -------------------------------------

// static variable -------------------------------------

// const data  -----------------------------------------
static const SjisUtf16Pare tblSJIS_UTF16_8140[TBL8140_ELEM_NUM];
static const SjisUtf16Pare tblSJIS_UTF16_849f[TBL849f_ELEM_NUM];


//======================================================
// SJIS-BEÇ©ÇÁUTF16-LEÇ÷ÇÃïœä∑
//======================================================
void ExSJIS_BEtoUTF16_LE(u8 *sjisp, u16 *unip, u16 length)
{
    u16     code;

    while ((*sjisp) && (length-- > 0))
    {
        if (((*sjisp >= SJIS_HIGHER_CODE1_MIN) && (*sjisp <= SJIS_HIGHER_CODE1_MAX))
            || ((*sjisp >= SJIS_HIGHER_CODE2_MIN) && (*sjisp <= SJIS_HIGHER_CODE2_MAX)))
        {                              // SJISÇ©ÅH
            code = (u16)((u16)*sjisp++ << 8);
            code |= (u16)*sjisp++;
        }
        else
        {                              // ASCII
            code = (u16)*sjisp++;
        }

        if (code == 0x005c)
        {
            *unip = 0x00a5;            // \

        }
        else if (code == 0x007e)
        {
            *unip = 0x203e;            // ~

        }
        else if ((code == 0x000d) || (code == 0x000a))
        {
            *unip = code;              // â¸çsÉRÅ[Éh

        }
        else if ((code >= 0x0020) && (code < 0x007e))   // ' ' Å` }
        {
            *unip = code;

        }
        else if ((code >= 0x00a1) && (code <= 0x00df))  // ° Å` ﬂ
        {
            *unip = (u16)((code - 0x00a1) + 0xff61);

        }
        else if ((code >= 0x8140) && (code <= 0x81fc))  // 'Å@' Å` Åõ
        {
            // ÉoÉâÉoÉâÇ»ÇÃÇ≈ÅAÉeÅ[ÉuÉãà¯Ç´
            *unip = tblSJIS_UTF16_8140[code - 0x8140].unicode;

        }
        else if ((code >= 0x824f) && (code <= 0x8258))  // ÇO Å` ÇX
        {
            *unip = (u16)((code - 0x824f) + 0xff10);

        }
        else if ((code >= 0x8260) && (code <= 0x8279))  // Ç` Å` Çy
        {
            *unip = (u16)((code - 0x8260) + 0xff21);

        }
        else if ((code >= 0x8281) && (code <= 0x829a))  // ÇÅ Å` Çö
        {
            *unip = (u16)((code - 0x8281) + 0xff41);

        }
        else if ((code >= 0x829f) && (code <= 0x82f1))  // Çü Å` ÇÒ
        {
            *unip = (u16)((code - 0x829f) + 0x3041);

        }
        else if ((code >= 0x8340) && (code <= 0x8396))  // É@ Å` Éñ      Å¶0x837fÇÕî≤ÇØ
        {
            *unip = (u16)((code - 0x8340) + 0x30a1);
            if (code == 0x837f)
            {
                *unip = 0x3000;
            }
            else if (code > 0x837f)
            {
                (*unip)--;
            }

        }
        else if ((code >= 0x839f) && (code <= 0x83b6))  // Éü Å` É∂
        {
            *unip = (u16)((code - 0x839f) + 0x0391);
            if (code >= 0x83b0)
                (*unip)++;

        }
        else if ((code >= 0x83bf) && (code <= 0x83d6))  // Éø Å` É÷
        {
            *unip = (u16)((code - 0x83bf) + 0x03b1);
            if (code >= 0x83d0)
                (*unip)++;

        }
        else if ((code >= 0x8440) && (code <= 0x8460))  // Ñ@ Å` Ñ`
        {
            *unip = (u16)((code - 0x8440) + 0x0410);
            if (code == 0x8446)
            {
                *unip = 0x0401;
            }
            else if (code > 0x8446)
            {
                (*unip)--;
            }

        }
        else if ((code >= 0x8470) && (code <= 0x8491))  // Ñp Å` Ñë     Å¶0x847fÇÕî≤ÇØ
        {
            *unip = (u16)((code - 0x8470) + 0x0430);
            if (code == 0x8476)
            {
                *unip = 0x0451;
            }
            else if (code == 0x847f)
            {
                *unip = 0x3000;
            }
            else if (code > 0x8476)
            {
                (*unip)--;
                if (code > 0x847f)
                {
                    (*unip)--;
                }
            }

        }
        else if ((code >= 0x849f) && (code <= 0x84be))  // Ñü Å` Ñæ
        {
            // ÉoÉâÉoÉâÇ»ÇÃÇ≈ÅAÉeÅ[ÉuÉãà¯Ç´
            *unip = tblSJIS_UTF16_849f[code - 0x849f].unicode;
        }
        else
        {
            *unip = 0x3000;
        }

        unip++;
    }
}


//======================================================
// UTF16-LEÇ©ÇÁSJIS-BEÇ÷ÇÃïœä∑
//======================================================
void ExUTF16_LEtoSJIS_BE(u8 *sjisp, u16 *unip, u16 length)
{
    u16     code, sjis_le;

    while ((*unip) && (length-- > 0))
    {
        code = *unip++;
        sjis_le = 0;

        if (code == 0x00a5)
        {
            sjis_le = 0x005c;          // \

        }
        else if (code == 0x005c)
        {
            sjis_le = 0x815f;          // Å_

        }
        else if (code == 0x203e)
        {
            sjis_le = 0x007e;          // ~

        }
        else if ((code == 0x000d) || (code == 0x000a))
        {
            sjis_le = code;            // â¸çsÉRÅ[Éh

        }
        else if ((code >= 0x0020) && (code < 0x007e))   // ' ' Å` }
        {
            sjis_le = code;

        }
        else if ((code >= 0x00a2) && (code <= 0x00f7))
        {                              // Unicode = 0x00a2 - 0x00f7 ÇÕÅA SJIS = 0x814c - 0x81f7Ç…îzíu
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0xc],
                                   0x81f7 - 0x814c);

        }
        else if ((code >= 0xff61) && (code <= 0xff9f))  // ° Å` ﬂ
        {
            sjis_le = (u16)((code - 0xff61) + 0x00a1);

        }
        else if (code == 0x4edd)
        {
            sjis_le = 0x8157;

        }
        else if ((code >= 0xff01) && (code <= 0xffe5))
        {
            if ((code >= 0xff10) && (code <= 0xff19))   // ÇO Å` ÇX
            {
                sjis_le = (u16)((code - 0xff10) + 0x824f);

            }
            else if ((code >= 0xff21) && (code <= 0xff3a))      // Ç` Å` Çy
            {
                sjis_le = (u16)((code - 0xff21) + 0x8260);

            }
            else if ((code >= 0xff41) && (code <= 0xff5a))      // ÇÅ Å` Çö
            {
                sjis_le = (u16)((code - 0xff41) + 0x8281);
            }
            else
            {                          // Unicode = 0xff01 - 0xffe5 ÇÕÅA SJIS = 0x8143 - 0x8197Ç…îzíu
                sjis_le =
                    SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[3],
                                       0x8197 - 0x8143);
            }

        }
        else if ((code >= 0x3000) && (code <= 0x30fe))
        {
            if ((code >= 0x3041) && (code <= 0x3093))   // Çü Å` ÇÒ
            {
                sjis_le = (u16)((code - 0x3041) + 0x829f);

            }
            else if ((code >= 0x30a1) && (code <= 0x30f6))      // É@ Å` Éñ      Å¶0x837fÇÕî≤ÇØ
            {
                sjis_le = (u16)((code - 0x30a1) + 0x8340);
                if (code >= 0x30e0)
                {
                    (sjis_le)++;
                }
            }
            else
            {                          // Unicode = 0x3000 - 0x30fe ÇÕÅA SJIS = 0x8140 - 0x81acÇ…îzíu
                sjis_le =
                    SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0],
                                       0x81ac - 0x8140);
            }

        }
        else if ((code >= 0x0391) && (code <= 0x03a9))  // Éü Å` É∂
        {
            sjis_le = (u16)((code - 0x0391) + 0x839f);
            if (code >= 0x03a3)
                (sjis_le)--;

        }
        else if ((code >= 0x03b1) && (code <= 0x03c9))  // Éø Å` É÷
        {
            sjis_le = (u16)((code - 0x03b1) + 0x83bf);
            if (code >= 0x03c3)
                (sjis_le)--;

        }
        else if (code == 0x0401)
        {
            sjis_le = 0x8446;

        }
        else if ((code >= 0x0410) && (code <= 0x042f))  // Ñ@ Å` Ñ`
        {
            sjis_le = (u16)((code - 0x0410) + 0x8440);
            if (code >= 0x0416)
            {
                (sjis_le)++;
            }

        }
        else if ((code >= 0x0430) && (code <= 0x044f))  // Ñp Å` Ñë     Å¶0x847fÇÕî≤ÇØ
        {
            sjis_le = (u16)((code - 0x0430) + 0x8470);
            if (code >= 0x0436)
            {
                (sjis_le)++;
                if (code >= 0x043e)
                {
                    (sjis_le)++;
                }
            }

        }
        else if (code == 0x0451)
        {
            sjis_le = 0x8476;

        }
        else if ((code >= 0x2500) && (code <= 0x254b))  // Ñü Å` Ñæ
        {
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_849f, TBL849f_ELEM_NUM);

        }
        else if (((code >= 0x2010) && (code <= 0x2312)) || ((code >= 0x25a0) && (code <= 0x266f)))
        {                              // è„ãLÉRÅ[ÉhÇÕÅA SJIS = 0x815c - 0x81fcÇ…îzíu
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0x815c - 0x8140],
                                   0x81fc - 0x815c);

        }
        else
        {
            sjis_le = 0x8140;
        }

        // ïœä∑ÇµÇΩSJISÉRÅ[ÉhÇÉoÉbÉtÉ@Ç…äiî[
        if (sjis_le & 0xff00)
        {                              // ASCIIÉRÅ[ÉhÇ≈Ç»ÇØÇÍÇŒÅAÉrÉbÉOÉGÉìÉfÉBÉAÉìå`éÆÇ≈äiî[ÅB
            *sjisp++ = (u8)(sjis_le >> 8);
        }
        *sjisp++ = (u8)(sjis_le);
    }
}


// Unicode -> SJISÇ÷ÇÃÉeÅ[ÉuÉãà¯Ç´
static u16 SearchUnicodeTable(u16 unicode, SjisUtf16Pare * tblp, int elem_num)
{
    elem_num++;
    while (elem_num--)
    {
        if (tblp->unicode == unicode)
        {
            return tblp->sjis;
        }
        tblp++;
    }
    return 0x8140;
}


//======================================================
// SJIS-BE <-> UTF16-LEïœä∑ÇÃÉ`ÉFÉbÉN
//======================================================
void CheckSJIS_BEtoUTF16_LE(void)
{
    u16     sjis, sjis_be, rev_sjis, sjis_le;
    u16     unicode;

    // ASCIIÉRÅ[ÉhÇÃÉ`ÉFÉbÉN
    for (sjis = 0; sjis < 0x00ff; sjis++)
    {
        unicode = 0;
        rev_sjis = 0;
        ExSJIS_BEtoUTF16_LE((u8 *)&sjis, &unicode, 1);
        ExUTF16_LEtoSJIS_BE((u8 *)&rev_sjis, &unicode, 1);
        sjis_le = (u16)((rev_sjis >> 8) | (rev_sjis << 8));
        OS_Printf("0x%x\t-> 0x%x\t-> 0x%x\n", sjis, unicode, sjis_le);
    }

    // SJISÉRÅ[ÉhÇÃÉ`ÉFÉbÉN
    for (sjis = 0x8140; sjis < 0x84ff; sjis++)
    {
        unicode = 0;
        rev_sjis = 0;
        sjis_be = (u16)((sjis >> 8) | (sjis << 8));
        ExSJIS_BEtoUTF16_LE((u8 *)&sjis_be, &unicode, 1);
        ExUTF16_LEtoSJIS_BE((u8 *)&rev_sjis, &unicode, 1);
        sjis_le = (u16)((rev_sjis >> 8) | (rev_sjis << 8));
        OS_Printf("0x%x\t-> 0x%x\t-> 0x%x\n", sjis, unicode, sjis_le);
    }
}


//======================================================
// SJISÉRÅ[Éh->UnicodeÉeÅ[ÉuÉã
//======================================================

// 0x8140Å`
static const SjisUtf16Pare tblSJIS_UTF16_8140[TBL8140_ELEM_NUM] = {
    {0x8140, 0x3000},                  // Å@ 
    {0x8141, 0x3001},                  // ÅA 
    {0x8142, 0x3002},                  // ÅB 
    {0x8143, 0xFF0C},                  // ÅC 
    {0x8144, 0xFF0E},                  // ÅD 
    {0x8145, 0x30FB},                  // ÅE 
    {0x8146, 0xFF1A},                  // ÅF 
    {0x8147, 0xFF1B},                  // ÅG 
    {0x8148, 0xFF1F},                  // ÅH 
    {0x8149, 0xFF01},                  // ÅI 
    {0x814A, 0x309B},                  // ÅJ 
    {0x814B, 0x309C},                  // ÅK 
    {0x814C, 0x00B4},                  // ÅL 
    {0x814D, 0xFF40},                  // ÅM 
    {0x814E, 0x00A8},                  // ÅN 
    {0x814F, 0xFF3E},                  // ÅO 
    {0x8150, 0xFFE3},                  // ÅP 
    {0x8151, 0xFF3F},                  // ÅQ 
    {0x8152, 0x30FD},                  // ÅR 
    {0x8153, 0x30FE},                  // ÅS 
    {0x8154, 0x309D},                  // ÅT 
    {0x8155, 0x309E},                  // ÅU 
    {0x8156, 0x3003},                  // ÅV 
    {0x8157, 0x4EDD},                  // ÅW 
    {0x8158, 0x3005},                  // ÅX 
    {0x8159, 0x3006},                  // ÅY 
    {0x815A, 0x3007},                  // ÅZ 
    {0x815B, 0x30FC},                  // Å[ 
    {0x815C, 0x2015},                  // Å\ 
    {0x815D, 0x2010},                  // Å] 
    {0x815E, 0xFF0F},                  // Å^ 
    {0x815F, 0x005C},                  // Å_ 
    {0x8160, 0x301C},                  // Å` 
    {0x8161, 0x2016},                  // Åa 
    {0x8162, 0xFF5C},                  // Åb 
    {0x8163, 0x2026},                  // Åc 
    {0x8164, 0x2025},                  // Åd 
    {0x8165, 0x2018},                  // Åe 
    {0x8166, 0x2019},                  // Åf 
    {0x8167, 0x201C},                  // Åg 
    {0x8168, 0x201D},                  // Åh 
    {0x8169, 0xFF08},                  // Åi 
    {0x816A, 0xFF09},                  // Åj 
    {0x816B, 0x3014},                  // Åk 
    {0x816C, 0x3015},                  // Ål 
    {0x816D, 0xFF3B},                  // Åm 
    {0x816E, 0xFF3D},                  // Ån 
    {0x816F, 0xFF5B},                  // Åo 
    {0x8170, 0xFF5D},                  // Åp 
    {0x8171, 0x3008},                  // Åq 
    {0x8172, 0x3009},                  // År 
    {0x8173, 0x300A},                  // Ås 
    {0x8174, 0x300B},                  // Åt 
    {0x8175, 0x300C},                  // Åu 
    {0x8176, 0x300D},                  // Åv 
    {0x8177, 0x300E},                  // Åw 
    {0x8178, 0x300F},                  // Åx 
    {0x8179, 0x3010},                  // Åy 
    {0x817A, 0x3011},                  // Åz 
    {0x817B, 0xFF0B},                  // Å{ 
    {0x817C, 0x2212},                  // Å| 
    {0x817D, 0x00B1},                  // Å} 
    {0x817E, 0x00D7},                  // Å~ 
    {0x817F, 0x3000},                  //  
    {0x8180, 0x00F7},                  // ÅÄ 
    {0x8181, 0xFF1D},                  // ÅÅ 
    {0x8182, 0x2260},                  // ÅÇ 
    {0x8183, 0xFF1C},                  // ÅÉ 
    {0x8184, 0xFF1E},                  // ÅÑ 
    {0x8185, 0x2266},                  // ÅÖ 
    {0x8186, 0x2267},                  // ÅÜ 
    {0x8187, 0x221E},                  // Åá 
    {0x8188, 0x2234},                  // Åà 
    {0x8189, 0x2642},                  // Åâ 
    {0x818A, 0x2640},                  // Åä 
    {0x818B, 0x00B0},                  // Åã 
    {0x818C, 0x2032},                  // Åå 
    {0x818D, 0x2033},                  // Åç 
    {0x818E, 0x2103},                  // Åé 
    {0x818F, 0xFFE5},                  // Åè 
    {0x8190, 0xFF04},                  // Åê 
    {0x8191, 0x00A2},                  // Åë 
    {0x8192, 0x00A3},                  // Åí 
    {0x8193, 0xFF05},                  // Åì 
    {0x8194, 0xFF03},                  // Åî 
    {0x8195, 0xFF06},                  // Åï 
    {0x8196, 0xFF0A},                  // Åñ 
    {0x8197, 0xFF20},                  // Åó 
    {0x8198, 0x00A7},                  // Åò 
    {0x8199, 0x2606},                  // Åô 
    {0x819A, 0x2605},                  // Åö 
    {0x819B, 0x25CB},                  // Åõ 
    {0x819C, 0x25CF},                  // Åú 
    {0x819D, 0x25CE},                  // Åù 
    {0x819E, 0x25C7},                  // Åû 
    {0x819F, 0x25C6},                  // Åü 
    {0x81A0, 0x25A1},                  // Å† 
    {0x81A1, 0x25A0},                  // Å° 
    {0x81A2, 0x25B3},                  // Å¢ 
    {0x81A3, 0x25B2},                  // Å£ 
    {0x81A4, 0x25BD},                  // Å§ 
    {0x81A5, 0x25BC},                  // Å• 
    {0x81A6, 0x203B},                  // Å¶ 
    {0x81A7, 0x3012},                  // Åß 
    {0x81A8, 0x2192},                  // Å® 
    {0x81A9, 0x2190},                  // Å© 
    {0x81AA, 0x2191},                  // Å™ 
    {0x81AB, 0x2193},                  // Å´ 
    {0x81AC, 0x3013},                  // Å¨ 
    {0x81AD, 0x3000},                  //  
    {0x81AE, 0x3000},                  //  
    {0x81AF, 0x3000},                  //  
    {0x81B0, 0x3000},                  //  
    {0x81B1, 0x3000},                  //  
    {0x81B2, 0x3000},                  //  
    {0x81B3, 0x3000},                  //  
    {0x81B4, 0x3000},                  //  
    {0x81B5, 0x3000},                  //  
    {0x81B6, 0x3000},                  //  
    {0x81B7, 0x3000},                  //  
    {0x81B8, 0x2208},                  // Å∏ 
    {0x81B9, 0x220B},                  // Åπ 
    {0x81BA, 0x2286},                  // Å∫ 
    {0x81BB, 0x2287},                  // Åª 
    {0x81BC, 0x2282},                  // Åº 
    {0x81BD, 0x2283},                  // ÅΩ 
    {0x81BE, 0x222A},                  // Åæ 
    {0x81BF, 0x2229},                  // Åø 
    {0x81C0, 0x3000},                  //  
    {0x81C1, 0x3000},                  //  
    {0x81C2, 0x3000},                  //  
    {0x81C3, 0x3000},                  //  
    {0x81C4, 0x3000},                  //  
    {0x81C5, 0x3000},                  //  
    {0x81C6, 0x3000},                  //  
    {0x81C7, 0x3000},                  //  
    {0x81C8, 0x2227},                  // Å» 
    {0x81C9, 0x2228},                  // Å… 
    {0x81CA, 0x00AC},                  // Å  
    {0x81CB, 0x21D2},                  // ÅÀ 
    {0x81CC, 0x21D4},                  // ÅÃ 
    {0x81CD, 0x2200},                  // ÅÕ 
    {0x81CE, 0x2203},                  // ÅŒ 
    {0x81CF, 0x3000},                  //  
    {0x81D0, 0x3000},                  //  
    {0x81D1, 0x3000},                  //  
    {0x81D2, 0x3000},                  //  
    {0x81D3, 0x3000},                  //  
    {0x81D4, 0x3000},                  //  
    {0x81D5, 0x3000},                  //  
    {0x81D6, 0x3000},                  //  
    {0x81D7, 0x3000},                  //  
    {0x81D8, 0x3000},                  //  
    {0x81D9, 0x3000},                  //  
    {0x81DA, 0x2220},                  // Å⁄ 
    {0x81DB, 0x22A5},                  // Å€ 
    {0x81DC, 0x2312},                  // Å‹ 
    {0x81DD, 0x2202},                  // Å› 
    {0x81DE, 0x2207},                  // Åﬁ 
    {0x81DF, 0x2261},                  // Åﬂ 
    {0x81E0, 0x2252},                  // Å‡ 
    {0x81E1, 0x226A},                  // Å· 
    {0x81E2, 0x226B},                  // Å‚ 
    {0x81E3, 0x221A},                  // Å„ 
    {0x81E4, 0x223D},                  // Å‰ 
    {0x81E5, 0x221D},                  // ÅÂ 
    {0x81E6, 0x2235},                  // ÅÊ 
    {0x81E7, 0x222B},                  // ÅÁ 
    {0x81E8, 0x222C},                  // ÅË 
    {0x81E9, 0x3000},                  //  
    {0x81EA, 0x3000},                  //  
    {0x81EB, 0x3000},                  //  
    {0x81EC, 0x3000},                  //  
    {0x81EE, 0x3000},                  //  
    {0x81EE, 0x3000},                  //  
    {0x81EF, 0x3000},                  //  
    {0x81F0, 0x212B},                  // Å 
    {0x81F1, 0x2030},                  // ÅÒ 
    {0x81F2, 0x266F},                  // ÅÚ 
    {0x81F3, 0x266D},                  // ÅÛ 
    {0x81F4, 0x266A},                  // ÅÙ 
    {0x81F5, 0x2020},                  // Åı 
    {0x81F6, 0x2021},                  // Åˆ 
    {0x81F7, 0x00B6},                  // Å˜ 
    {0x81F8, 0x3000},                  //  
    {0x81F9, 0x3000},                  //  
    {0x81FA, 0x3000},                  //  
    {0x81FB, 0x3000},                  //  
    {0x81FC, 0x25EF},                  // Å¸ 
};


// 0x849fÅ`
// UnicodeÇ≈ÇÕÅA0x2500-0x254bÇÃä‘
static const SjisUtf16Pare tblSJIS_UTF16_849f[TBL849f_ELEM_NUM] = {
    {0x849F, 0x2500},                  // Ñü 
    {0x84A0, 0x2502},                  // Ñ† 
    {0x84A1, 0x250C},                  // Ñ° 
    {0x84A2, 0x2510},                  // Ñ¢ 
    {0x84A3, 0x2518},                  // Ñ£ 
    {0x84A4, 0x2514},                  // Ñ§ 
    {0x84A5, 0x251C},                  // Ñ• 
    {0x84A6, 0x252C},                  // Ñ¶ 
    {0x84A7, 0x2524},                  // Ñß 
    {0x84A8, 0x2534},                  // Ñ® 
    {0x84A9, 0x253C},                  // Ñ© 
    {0x84AA, 0x2501},                  // Ñ™ 
    {0x84AB, 0x2503},                  // Ñ´ 
    {0x84AC, 0x250F},                  // Ñ¨ 
    {0x84AD, 0x2513},                  // Ñ≠ 
    {0x84AE, 0x251B},                  // ÑÆ 
    {0x84AF, 0x2517},                  // ÑØ 
    {0x84B0, 0x2523},                  // Ñ∞ 
    {0x84B1, 0x2533},                  // Ñ± 
    {0x84B2, 0x252B},                  // Ñ≤ 
    {0x84B3, 0x253B},                  // Ñ≥ 
    {0x84B4, 0x254B},                  // Ñ¥ 
    {0x84B5, 0x2520},                  // Ñµ 
    {0x84B6, 0x252F},                  // Ñ∂ 
    {0x84B7, 0x2528},                  // Ñ∑ 
    {0x84B8, 0x2537},                  // Ñ∏ 
    {0x84B9, 0x253F},                  // Ñπ 
    {0x84BA, 0x251D},                  // Ñ∫ 
    {0x84BB, 0x2530},                  // Ñª 
    {0x84BC, 0x2525},                  // Ñº 
    {0x84BD, 0x2538},                  // ÑΩ 
    {0x84BE, 0x2542},                  // Ñæ 
};
