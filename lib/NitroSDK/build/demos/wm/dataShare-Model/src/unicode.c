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
  ���쌠�\�L�C��

  
  Revision 1.1  2005/11/21 00:41:10  adachi_hiroaki
  �e�@�I����ʂɎQ���l���ƃj�b�N�l�[����\������悤�ɂ���

  $NoKeywords$
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "unicode.h"

// define data------------------------------------------
#define TBL8140_ELEM_NUM				0xbd    // 8140tbl�̗v�f��
#define TBL849f_ELEM_NUM				0x20    // 849ftbl�̗v�f��

        // SJIS�R�[�h����p�̒l
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
// SJIS-BE����UTF16-LE�ւ̕ϊ�
//======================================================
void ExSJIS_BEtoUTF16_LE(u8 *sjisp, u16 *unip, u16 length)
{
    u16     code;

    while ((*sjisp) && (length-- > 0))
    {
        if (((*sjisp >= SJIS_HIGHER_CODE1_MIN) && (*sjisp <= SJIS_HIGHER_CODE1_MAX))
            || ((*sjisp >= SJIS_HIGHER_CODE2_MIN) && (*sjisp <= SJIS_HIGHER_CODE2_MAX)))
        {                              // SJIS���H
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
            *unip = code;              // ���s�R�[�h

        }
        else if ((code >= 0x0020) && (code < 0x007e))   // ' ' �` }
        {
            *unip = code;

        }
        else if ((code >= 0x00a1) && (code <= 0x00df))  // � �` �
        {
            *unip = (u16)((code - 0x00a1) + 0xff61);

        }
        else if ((code >= 0x8140) && (code <= 0x81fc))  // '�@' �` ��
        {
            // �o���o���Ȃ̂ŁA�e�[�u������
            *unip = tblSJIS_UTF16_8140[code - 0x8140].unicode;

        }
        else if ((code >= 0x824f) && (code <= 0x8258))  // �O �` �X
        {
            *unip = (u16)((code - 0x824f) + 0xff10);

        }
        else if ((code >= 0x8260) && (code <= 0x8279))  // �` �` �y
        {
            *unip = (u16)((code - 0x8260) + 0xff21);

        }
        else if ((code >= 0x8281) && (code <= 0x829a))  // �� �` ��
        {
            *unip = (u16)((code - 0x8281) + 0xff41);

        }
        else if ((code >= 0x829f) && (code <= 0x82f1))  // �� �` ��
        {
            *unip = (u16)((code - 0x829f) + 0x3041);

        }
        else if ((code >= 0x8340) && (code <= 0x8396))  // �@ �` ��      ��0x837f�͔���
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
        else if ((code >= 0x839f) && (code <= 0x83b6))  // �� �` ��
        {
            *unip = (u16)((code - 0x839f) + 0x0391);
            if (code >= 0x83b0)
                (*unip)++;

        }
        else if ((code >= 0x83bf) && (code <= 0x83d6))  // �� �` ��
        {
            *unip = (u16)((code - 0x83bf) + 0x03b1);
            if (code >= 0x83d0)
                (*unip)++;

        }
        else if ((code >= 0x8440) && (code <= 0x8460))  // �@ �` �`
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
        else if ((code >= 0x8470) && (code <= 0x8491))  // �p �` ��     ��0x847f�͔���
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
        else if ((code >= 0x849f) && (code <= 0x84be))  // �� �` ��
        {
            // �o���o���Ȃ̂ŁA�e�[�u������
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
// UTF16-LE����SJIS-BE�ւ̕ϊ�
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
            sjis_le = 0x815f;          // �_

        }
        else if (code == 0x203e)
        {
            sjis_le = 0x007e;          // ~

        }
        else if ((code == 0x000d) || (code == 0x000a))
        {
            sjis_le = code;            // ���s�R�[�h

        }
        else if ((code >= 0x0020) && (code < 0x007e))   // ' ' �` }
        {
            sjis_le = code;

        }
        else if ((code >= 0x00a2) && (code <= 0x00f7))
        {                              // Unicode = 0x00a2 - 0x00f7 �́A SJIS = 0x814c - 0x81f7�ɔz�u
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0xc],
                                   0x81f7 - 0x814c);

        }
        else if ((code >= 0xff61) && (code <= 0xff9f))  // � �` �
        {
            sjis_le = (u16)((code - 0xff61) + 0x00a1);

        }
        else if (code == 0x4edd)
        {
            sjis_le = 0x8157;

        }
        else if ((code >= 0xff01) && (code <= 0xffe5))
        {
            if ((code >= 0xff10) && (code <= 0xff19))   // �O �` �X
            {
                sjis_le = (u16)((code - 0xff10) + 0x824f);

            }
            else if ((code >= 0xff21) && (code <= 0xff3a))      // �` �` �y
            {
                sjis_le = (u16)((code - 0xff21) + 0x8260);

            }
            else if ((code >= 0xff41) && (code <= 0xff5a))      // �� �` ��
            {
                sjis_le = (u16)((code - 0xff41) + 0x8281);
            }
            else
            {                          // Unicode = 0xff01 - 0xffe5 �́A SJIS = 0x8143 - 0x8197�ɔz�u
                sjis_le =
                    SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[3],
                                       0x8197 - 0x8143);
            }

        }
        else if ((code >= 0x3000) && (code <= 0x30fe))
        {
            if ((code >= 0x3041) && (code <= 0x3093))   // �� �` ��
            {
                sjis_le = (u16)((code - 0x3041) + 0x829f);

            }
            else if ((code >= 0x30a1) && (code <= 0x30f6))      // �@ �` ��      ��0x837f�͔���
            {
                sjis_le = (u16)((code - 0x30a1) + 0x8340);
                if (code >= 0x30e0)
                {
                    (sjis_le)++;
                }
            }
            else
            {                          // Unicode = 0x3000 - 0x30fe �́A SJIS = 0x8140 - 0x81ac�ɔz�u
                sjis_le =
                    SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0],
                                       0x81ac - 0x8140);
            }

        }
        else if ((code >= 0x0391) && (code <= 0x03a9))  // �� �` ��
        {
            sjis_le = (u16)((code - 0x0391) + 0x839f);
            if (code >= 0x03a3)
                (sjis_le)--;

        }
        else if ((code >= 0x03b1) && (code <= 0x03c9))  // �� �` ��
        {
            sjis_le = (u16)((code - 0x03b1) + 0x83bf);
            if (code >= 0x03c3)
                (sjis_le)--;

        }
        else if (code == 0x0401)
        {
            sjis_le = 0x8446;

        }
        else if ((code >= 0x0410) && (code <= 0x042f))  // �@ �` �`
        {
            sjis_le = (u16)((code - 0x0410) + 0x8440);
            if (code >= 0x0416)
            {
                (sjis_le)++;
            }

        }
        else if ((code >= 0x0430) && (code <= 0x044f))  // �p �` ��     ��0x847f�͔���
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
        else if ((code >= 0x2500) && (code <= 0x254b))  // �� �` ��
        {
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_849f, TBL849f_ELEM_NUM);

        }
        else if (((code >= 0x2010) && (code <= 0x2312)) || ((code >= 0x25a0) && (code <= 0x266f)))
        {                              // ��L�R�[�h�́A SJIS = 0x815c - 0x81fc�ɔz�u
            sjis_le =
                SearchUnicodeTable(code, (SjisUtf16Pare *) & tblSJIS_UTF16_8140[0x815c - 0x8140],
                                   0x81fc - 0x815c);

        }
        else
        {
            sjis_le = 0x8140;
        }

        // �ϊ�����SJIS�R�[�h���o�b�t�@�Ɋi�[
        if (sjis_le & 0xff00)
        {                              // ASCII�R�[�h�łȂ���΁A�r�b�O�G���f�B�A���`���Ŋi�[�B
            *sjisp++ = (u8)(sjis_le >> 8);
        }
        *sjisp++ = (u8)(sjis_le);
    }
}


// Unicode -> SJIS�ւ̃e�[�u������
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
// SJIS-BE <-> UTF16-LE�ϊ��̃`�F�b�N
//======================================================
void CheckSJIS_BEtoUTF16_LE(void)
{
    u16     sjis, sjis_be, rev_sjis, sjis_le;
    u16     unicode;

    // ASCII�R�[�h�̃`�F�b�N
    for (sjis = 0; sjis < 0x00ff; sjis++)
    {
        unicode = 0;
        rev_sjis = 0;
        ExSJIS_BEtoUTF16_LE((u8 *)&sjis, &unicode, 1);
        ExUTF16_LEtoSJIS_BE((u8 *)&rev_sjis, &unicode, 1);
        sjis_le = (u16)((rev_sjis >> 8) | (rev_sjis << 8));
        OS_Printf("0x%x\t-> 0x%x\t-> 0x%x\n", sjis, unicode, sjis_le);
    }

    // SJIS�R�[�h�̃`�F�b�N
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
// SJIS�R�[�h->Unicode�e�[�u��
//======================================================

// 0x8140�`
static const SjisUtf16Pare tblSJIS_UTF16_8140[TBL8140_ELEM_NUM] = {
    {0x8140, 0x3000},                  // �@ 
    {0x8141, 0x3001},                  // �A 
    {0x8142, 0x3002},                  // �B 
    {0x8143, 0xFF0C},                  // �C 
    {0x8144, 0xFF0E},                  // �D 
    {0x8145, 0x30FB},                  // �E 
    {0x8146, 0xFF1A},                  // �F 
    {0x8147, 0xFF1B},                  // �G 
    {0x8148, 0xFF1F},                  // �H 
    {0x8149, 0xFF01},                  // �I 
    {0x814A, 0x309B},                  // �J 
    {0x814B, 0x309C},                  // �K 
    {0x814C, 0x00B4},                  // �L 
    {0x814D, 0xFF40},                  // �M 
    {0x814E, 0x00A8},                  // �N 
    {0x814F, 0xFF3E},                  // �O 
    {0x8150, 0xFFE3},                  // �P 
    {0x8151, 0xFF3F},                  // �Q 
    {0x8152, 0x30FD},                  // �R 
    {0x8153, 0x30FE},                  // �S 
    {0x8154, 0x309D},                  // �T 
    {0x8155, 0x309E},                  // �U 
    {0x8156, 0x3003},                  // �V 
    {0x8157, 0x4EDD},                  // �W 
    {0x8158, 0x3005},                  // �X 
    {0x8159, 0x3006},                  // �Y 
    {0x815A, 0x3007},                  // �Z 
    {0x815B, 0x30FC},                  // �[ 
    {0x815C, 0x2015},                  // �\ 
    {0x815D, 0x2010},                  // �] 
    {0x815E, 0xFF0F},                  // �^ 
    {0x815F, 0x005C},                  // �_ 
    {0x8160, 0x301C},                  // �` 
    {0x8161, 0x2016},                  // �a 
    {0x8162, 0xFF5C},                  // �b 
    {0x8163, 0x2026},                  // �c 
    {0x8164, 0x2025},                  // �d 
    {0x8165, 0x2018},                  // �e 
    {0x8166, 0x2019},                  // �f 
    {0x8167, 0x201C},                  // �g 
    {0x8168, 0x201D},                  // �h 
    {0x8169, 0xFF08},                  // �i 
    {0x816A, 0xFF09},                  // �j 
    {0x816B, 0x3014},                  // �k 
    {0x816C, 0x3015},                  // �l 
    {0x816D, 0xFF3B},                  // �m 
    {0x816E, 0xFF3D},                  // �n 
    {0x816F, 0xFF5B},                  // �o 
    {0x8170, 0xFF5D},                  // �p 
    {0x8171, 0x3008},                  // �q 
    {0x8172, 0x3009},                  // �r 
    {0x8173, 0x300A},                  // �s 
    {0x8174, 0x300B},                  // �t 
    {0x8175, 0x300C},                  // �u 
    {0x8176, 0x300D},                  // �v 
    {0x8177, 0x300E},                  // �w 
    {0x8178, 0x300F},                  // �x 
    {0x8179, 0x3010},                  // �y 
    {0x817A, 0x3011},                  // �z 
    {0x817B, 0xFF0B},                  // �{ 
    {0x817C, 0x2212},                  // �| 
    {0x817D, 0x00B1},                  // �} 
    {0x817E, 0x00D7},                  // �~ 
    {0x817F, 0x3000},                  //  
    {0x8180, 0x00F7},                  // �� 
    {0x8181, 0xFF1D},                  // �� 
    {0x8182, 0x2260},                  // �� 
    {0x8183, 0xFF1C},                  // �� 
    {0x8184, 0xFF1E},                  // �� 
    {0x8185, 0x2266},                  // �� 
    {0x8186, 0x2267},                  // �� 
    {0x8187, 0x221E},                  // �� 
    {0x8188, 0x2234},                  // �� 
    {0x8189, 0x2642},                  // �� 
    {0x818A, 0x2640},                  // �� 
    {0x818B, 0x00B0},                  // �� 
    {0x818C, 0x2032},                  // �� 
    {0x818D, 0x2033},                  // �� 
    {0x818E, 0x2103},                  // �� 
    {0x818F, 0xFFE5},                  // �� 
    {0x8190, 0xFF04},                  // �� 
    {0x8191, 0x00A2},                  // �� 
    {0x8192, 0x00A3},                  // �� 
    {0x8193, 0xFF05},                  // �� 
    {0x8194, 0xFF03},                  // �� 
    {0x8195, 0xFF06},                  // �� 
    {0x8196, 0xFF0A},                  // �� 
    {0x8197, 0xFF20},                  // �� 
    {0x8198, 0x00A7},                  // �� 
    {0x8199, 0x2606},                  // �� 
    {0x819A, 0x2605},                  // �� 
    {0x819B, 0x25CB},                  // �� 
    {0x819C, 0x25CF},                  // �� 
    {0x819D, 0x25CE},                  // �� 
    {0x819E, 0x25C7},                  // �� 
    {0x819F, 0x25C6},                  // �� 
    {0x81A0, 0x25A1},                  // �� 
    {0x81A1, 0x25A0},                  // �� 
    {0x81A2, 0x25B3},                  // �� 
    {0x81A3, 0x25B2},                  // �� 
    {0x81A4, 0x25BD},                  // �� 
    {0x81A5, 0x25BC},                  // �� 
    {0x81A6, 0x203B},                  // �� 
    {0x81A7, 0x3012},                  // �� 
    {0x81A8, 0x2192},                  // �� 
    {0x81A9, 0x2190},                  // �� 
    {0x81AA, 0x2191},                  // �� 
    {0x81AB, 0x2193},                  // �� 
    {0x81AC, 0x3013},                  // �� 
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
    {0x81B8, 0x2208},                  // �� 
    {0x81B9, 0x220B},                  // �� 
    {0x81BA, 0x2286},                  // �� 
    {0x81BB, 0x2287},                  // �� 
    {0x81BC, 0x2282},                  // �� 
    {0x81BD, 0x2283},                  // �� 
    {0x81BE, 0x222A},                  // �� 
    {0x81BF, 0x2229},                  // �� 
    {0x81C0, 0x3000},                  //  
    {0x81C1, 0x3000},                  //  
    {0x81C2, 0x3000},                  //  
    {0x81C3, 0x3000},                  //  
    {0x81C4, 0x3000},                  //  
    {0x81C5, 0x3000},                  //  
    {0x81C6, 0x3000},                  //  
    {0x81C7, 0x3000},                  //  
    {0x81C8, 0x2227},                  // �� 
    {0x81C9, 0x2228},                  // �� 
    {0x81CA, 0x00AC},                  // �� 
    {0x81CB, 0x21D2},                  // �� 
    {0x81CC, 0x21D4},                  // �� 
    {0x81CD, 0x2200},                  // �� 
    {0x81CE, 0x2203},                  // �� 
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
    {0x81DA, 0x2220},                  // �� 
    {0x81DB, 0x22A5},                  // �� 
    {0x81DC, 0x2312},                  // �� 
    {0x81DD, 0x2202},                  // �� 
    {0x81DE, 0x2207},                  // �� 
    {0x81DF, 0x2261},                  // �� 
    {0x81E0, 0x2252},                  // �� 
    {0x81E1, 0x226A},                  // �� 
    {0x81E2, 0x226B},                  // �� 
    {0x81E3, 0x221A},                  // �� 
    {0x81E4, 0x223D},                  // �� 
    {0x81E5, 0x221D},                  // �� 
    {0x81E6, 0x2235},                  // �� 
    {0x81E7, 0x222B},                  // �� 
    {0x81E8, 0x222C},                  // �� 
    {0x81E9, 0x3000},                  //  
    {0x81EA, 0x3000},                  //  
    {0x81EB, 0x3000},                  //  
    {0x81EC, 0x3000},                  //  
    {0x81EE, 0x3000},                  //  
    {0x81EE, 0x3000},                  //  
    {0x81EF, 0x3000},                  //  
    {0x81F0, 0x212B},                  // �� 
    {0x81F1, 0x2030},                  // �� 
    {0x81F2, 0x266F},                  // �� 
    {0x81F3, 0x266D},                  // �� 
    {0x81F4, 0x266A},                  // �� 
    {0x81F5, 0x2020},                  // �� 
    {0x81F6, 0x2021},                  // �� 
    {0x81F7, 0x00B6},                  // �� 
    {0x81F8, 0x3000},                  //  
    {0x81F9, 0x3000},                  //  
    {0x81FA, 0x3000},                  //  
    {0x81FB, 0x3000},                  //  
    {0x81FC, 0x25EF},                  // �� 
};


// 0x849f�`
// Unicode�ł́A0x2500-0x254b�̊�
static const SjisUtf16Pare tblSJIS_UTF16_849f[TBL849f_ELEM_NUM] = {
    {0x849F, 0x2500},                  // �� 
    {0x84A0, 0x2502},                  // �� 
    {0x84A1, 0x250C},                  // �� 
    {0x84A2, 0x2510},                  // �� 
    {0x84A3, 0x2518},                  // �� 
    {0x84A4, 0x2514},                  // �� 
    {0x84A5, 0x251C},                  // �� 
    {0x84A6, 0x252C},                  // �� 
    {0x84A7, 0x2524},                  // �� 
    {0x84A8, 0x2534},                  // �� 
    {0x84A9, 0x253C},                  // �� 
    {0x84AA, 0x2501},                  // �� 
    {0x84AB, 0x2503},                  // �� 
    {0x84AC, 0x250F},                  // �� 
    {0x84AD, 0x2513},                  // �� 
    {0x84AE, 0x251B},                  // �� 
    {0x84AF, 0x2517},                  // �� 
    {0x84B0, 0x2523},                  // �� 
    {0x84B1, 0x2533},                  // �� 
    {0x84B2, 0x252B},                  // �� 
    {0x84B3, 0x253B},                  // �� 
    {0x84B4, 0x254B},                  // �� 
    {0x84B5, 0x2520},                  // �� 
    {0x84B6, 0x252F},                  // �� 
    {0x84B7, 0x2528},                  // �� 
    {0x84B8, 0x2537},                  // �� 
    {0x84B9, 0x253F},                  // �� 
    {0x84BA, 0x251D},                  // �� 
    {0x84BB, 0x2530},                  // �� 
    {0x84BC, 0x2525},                  // �� 
    {0x84BD, 0x2538},                  // �� 
    {0x84BE, 0x2542},                  // �� 
};
