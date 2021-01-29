/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_Font.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys/g2d/g2d_Font.h>
#include <nnsys/g2d/load/g2d_NFT_load.h>
#include <nnsys/g2d/fmt/g2d_Font_data.h>
#include <nnsys/g2d/g2d_config.h>



//****************************************************************************
// Local Functions
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         GetGlyphIndex

  Description:  �����R�[�h�}�b�v�u���b�N���� c �ɑΉ�����O���t�C���f�b�N�X��
                �擾���܂��B

  Arguments:    pMap:   �t�H���g�̕����R�[�h�}�b�v�u���b�N�B
                c:      �����R�[�h�B

  Returns:      c �ɑΉ�����O���t�C���f�b�N�X�B
                ���݂��Ȃ��ꍇ�� NNS_G2D_GLYPH_INDEX_NOT_FOUND�B
 *---------------------------------------------------------------------------*/
static u16 GetGlyphIndex(const NNSG2dFontCodeMap* pMap, u16 c)
{
    u16 index = NNS_G2D_GLYPH_INDEX_NOT_FOUND;

    NNS_G2D_POINTER_ASSERT( pMap );
    NNS_G2D_ASSERT(pMap->ccodeBegin <= c && c <= pMap->ccodeEnd);

    switch( pMap->mappingMethod )
    {
    //-----------------------------------------------------------
    // �C���f�b�N�X = �����R�[�h - �I�t�Z�b�g
    case NNS_G2D_MAPMETHOD_DIRECT:
        {
            u16 offset = pMap->mapInfo[0];
            index = (u16)(c - pMap->ccodeBegin + offset);
        }
        break;

    //-----------------------------------------------------------
    // �C���f�b�N�X = table[�����R�[�h - �����R�[�h�I�t�Z�b�g]
    case NNS_G2D_MAPMETHOD_TABLE:
        {
            const int table_index = c - pMap->ccodeBegin;

            index = pMap->mapInfo[table_index];
        }
        break;

    //-----------------------------------------------------------
    // �C���f�b�N�X = �񕪒T��(�����R�[�h)
    case NNS_G2D_MAPMETHOD_SCAN:
        {
            const NNSG2dCMapInfoScan* const ws = (NNSG2dCMapInfoScan*)(pMap->mapInfo);
            const NNSG2dCMapScanEntry* st = &(ws->entries[0]);
            const NNSG2dCMapScanEntry* ed = &(ws->entries[ws->num - 1]);

            while( st <= ed )
            {
                const NNSG2dCMapScanEntry* md = st + (ed - st) / 2;

                if( md->ccode < c )
                {
                    st = md + 1;
                }
                else if( c < md->ccode )
                {
                    ed = md - 1;
                }
                else
                {
                    index = md->index;
                    break;
                }
            }
        }
        break;

    //-----------------------------------------------------------
    // unknown
    default:
        NNS_G2D_ASSERTMSG(FALSE, "unknwon MAPMETHOD");
    }

    return index;
}



/*---------------------------------------------------------------------------*
  Name:         GetCharWidthsFromIndex

  Description:  �������u���b�N����O���t�C���f�b�N�X�ɑΉ����镶��������
                �擾���܂��B

  Arguments:    pWidth: �t�H���g�̕������u���b�N�ւ̃|�C���^�B
                idx:    �O���t�C���f�b�N�X�B

  Returns:      idx �ɑΉ����镶�������B
                idx �� pWidth �Ɋ܂܂�Ȃ��C���f�b�N�X�ł���ꍇ�̓���͕s��ł��B
 *---------------------------------------------------------------------------*/
static NNS_G2D_INLINE const NNSG2dCharWidths* GetCharWidthsFromIndex( const NNSG2dFontWidth* const pWidth, int idx )
{
    NNS_G2D_POINTER_ASSERT( pWidth );
    NNS_G2D_ASSERT(pWidth->indexBegin <= idx && idx <= pWidth->indexEnd);

    return (NNSG2dCharWidths*)(pWidth->widthTable) + (idx - pWidth->indexBegin);
}





//*******************************************************************************
// Global Functions
//*******************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontInit*

  Description:  �t�H���g���\�[�X����t�H���g���\�z���܂��B

  Arguments:    pFont:      �t�H���g�ւ̃|�C���^�B
                pNftrFile:  �t�H���g���\�[�X�ւ̃|�C���^�B

  Returns:      �t�H���g�̍\�z�ɐ��������ꍇ��FALSE�ȊO��Ԃ��܂��B
 *---------------------------------------------------------------------------*/

void NNS_G2dFontInitAuto(NNSG2dFont* pFont, void* pNftrFile)
{
    const static NNSiG2dSplitCharCallback spliterTable[] =
    {
        NNSi_G2dSplitCharUTF8,
        NNSi_G2dSplitCharUTF16,
        NNSi_G2dSplitCharShiftJIS,
        NNSi_G2dSplitChar1Byte
    };
    NNSG2dFontEncoding encoding;
    BOOL result;

    NNS_G2D_POINTER_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( pNftrFile );

    result = NNSi_G2dGetUnpackedFont(pNftrFile, &(pFont->pRes));
    NNS_G2D_ASSERTMSG(result, "Faild to unpack font.");
    encoding = (NNSG2dFontEncoding)pFont->pRes->encoding;
    NNS_G2D_MINMAX_ASSERT( encoding, 0, NNS_G2D_NUM_OF_ENCODING - 1 );

    pFont->cbCharSpliter = spliterTable[encoding];
}


#define NNS_G2D_DEFINE_FONT_INIT(name, enc, spliter)                        \
    void NNS_G2dFontInit##name(NNSG2dFont* pFont, void* pNftrFile)          \
    {                                                                       \
        BOOL result;                                                        \
        NNS_G2D_POINTER_ASSERT( pFont );                                    \
        NNS_G2D_POINTER_ASSERT( pNftrFile );                                \
                                                                            \
        result = NNSi_G2dGetUnpackedFont(pNftrFile, &(pFont->pRes));        \
        NNS_G2D_ASSERTMSG(result, "Faild to unpack font.");                 \
        NNS_G2D_ASSERT(  (NNSG2dFontEncoding)pFont->pRes->encoding          \
                                    == NNS_G2D_FONT_ENCODING_##enc );       \
                                                                            \
        pFont->cbCharSpliter = NNSi_G2dSplitChar##spliter;                  \
    }

NNS_G2D_DEFINE_FONT_INIT(UTF8, UTF8, UTF8)
NNS_G2D_DEFINE_FONT_INIT(UTF16, UTF16, UTF16)
NNS_G2D_DEFINE_FONT_INIT(ShiftJIS, SJIS, ShiftJIS)
NNS_G2D_DEFINE_FONT_INIT(CP1252, CP1252, 1Byte)



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontFindGlyphIndex

  Description:  �����R�[�h����Ή�����O���t�̃C���f�b�N�X���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      �O���t�C���f�b�N�X���擾���镶���R�[�h�B

  Returns:      c �ɑΉ�����O���t�����݂���ꍇ�͂��̃C���f�b�N�X�B
                ���݂��Ȃ��ꍇ�� NNS_G2D_GLYPH_INDEX_NOT_FOUND�B
 *---------------------------------------------------------------------------*/
u16 NNS_G2dFontFindGlyphIndex( const NNSG2dFont* pFont, u16 c )
{
    const NNSG2dFontCodeMap* pMap;

    NNS_G2D_FONT_ASSERT(pFont);

    pMap = pFont->pRes->pMap;

    // CMAP�u���b�N�̃��X�g����`�T��
    while( pMap != NULL )
    {
        if( (pMap->ccodeBegin <= c) && (c <= pMap->ccodeEnd) )
        {
            return GetGlyphIndex(pMap, c);
        }

        pMap = pMap->pNext;
    }

    // ������Ȃ�����
    return NNS_G2D_GLYPH_INDEX_NOT_FOUND;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCharWidthsFromIndex

  Description:  �O���t�C���f�b�N�X���當�������𓾂܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                idx:    �O���t�C���f�b�N�X�B

  Returns:      idx �ɑΉ����镶������񂪑��݂���ꍇ��
                �t�H���g���Ɋi�[����Ă��镶�������ւ̃|�C���^�B
                ���݂��Ȃ��ꍇ�̓t�H���g�̃f�t�H���g�����ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
const NNSG2dCharWidths* NNS_G2dFontGetCharWidthsFromIndex( const NNSG2dFont* pFont, u16 idx )
{
    const NNSG2dFontWidth* pWidth;

    NNS_G2D_FONT_ASSERT(pFont);

    pWidth = pFont->pRes->pWidth;

    // �����u���b�N�̃��X�g����`�T��
    while( pWidth != NULL )
    {
        if( (pWidth->indexBegin <= idx) && (idx <= pWidth->indexEnd) )
        {
            return GetCharWidthsFromIndex(pWidth, idx);

        }

        pWidth = pWidth->pNext;
    }

    // ������Ȃ���΃f�t�H���g��Ԃ��B
    return &(pFont->pRes->defaultWidth);
}















//----------------------------------------------------------------------------
// �T�C�Y�擾
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetStringWidth

  Description:  �������1�s���`�悵���ꍇ�̕������߂܂��B
                i.e. txt ������s�����������͏I�[�����܂ł�`�悵���ꍇ��
                �����񕝂����߂܂��B

  Arguments:    pTxn:   �e�L�X�g�X�N���[���\���̂ւ̃|�C���^�B
                txt:    NUL�I�[������ւ̃|�C���^�B
                pPos:   txt �����s�������܂ޏꍇ�ɍŏ��̉��s�����̎��̕����ւ�
                        �|�C���^���i�[����o�b�t�@�ւ̃|�C���^�B
                        txt �����s�������܂܂Ȃ��ꍇ�� NULL ���i�[����܂��B
                        �s�v�ȏꍇ�� NULL ���w�肷�邱�Ƃ��ł��܂��B

  Returns:      txt �̕��B
 *---------------------------------------------------------------------------*/
int NNSi_G2dFontGetStringWidth(
    const NNSG2dFont* pFont,
    int hSpace,
    const void* str,
    const void** pPos
)
{
    int width = 0;
    const void* pos = str;
    u16 c;
    NNSiG2dSplitCharCallback getNextChar;

    NNS_G2D_FONT_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( str );

    getNextChar = NNSi_G2dFontGetSpliter(pFont);

    while( (c = getNextChar((const void**)&pos)) != 0 )
    {
        if( c == '\n' )
        {
            break;
        }

        width += NNS_G2dFontGetCharWidth(pFont, c) + hSpace;
    }

    if( pPos != NULL )
    {
        *pPos = (c == '\n') ? pos: NULL;
    }
    if( width > 0 )
    {
        width -= hSpace;
    }
    return width;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetTextHeight

  Description:  ���s���܂ޕ������`�悵���ꍇ�̍��������߂܂��B

  Arguments:    pTxn:   �e�L�X�g�X�N���[���\���̂ւ̃|�C���^�B
                txt:    NUL�I�[������ւ̃|�C���^�B

  Returns:      txt �̍����B
 *---------------------------------------------------------------------------*/
int NNSi_G2dFontGetTextHeight(
    const NNSG2dFont* pFont,
    int vSpace,
    const void* txt
)
{
    const void* pos = txt;
    int lines = 1;
    NNSG2dTextRect rect = {0, 0};
    u16 c;
    NNSiG2dSplitCharCallback getNextChar;

    NNS_G2D_FONT_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( txt );

    getNextChar = NNSi_G2dFontGetSpliter(pFont);

    while( (c = getNextChar((const void**)&pos)) != 0 )
    {
        if( c == '\n' )
        {
            lines++;
        }
    }

    return lines * (NNS_G2dFontGetLineFeed(pFont) + vSpace) - vSpace;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetTextWidth

  Description:  �������`�悵���ꍇ�̕������߂܂��B
                ����͊e�s�̕��̍ő�l�ɂȂ�܂��B

  Arguments:    pTxn:   �v�Z�̊�ƂȂ� Font �ւ̃|�C���^�B
                txt:    ������B

  Returns:      �������`�悵���ꍇ�̕��B
 *---------------------------------------------------------------------------*/
int NNSi_G2dFontGetTextWidth(
    const NNSG2dFont* pFont,
    int hSpace,
    const void* txt
)
{
    int width = 0;

    NNS_G2D_FONT_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( txt );

    while( txt != NULL )
    {
        const int line_width = NNSi_G2dFontGetStringWidth(pFont, hSpace, txt, &txt);
        if( line_width > width )
        {
            width = line_width;
        }
    }

    return width;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetTextRect

  Description:  ���s���܂ޕ������`�悵���ꍇ�̍ő啝�ƍ��������߂܂��B

  Arguments:    pTxn:   �e�L�X�g�X�N���[���\���̂ւ̃|�C���^�B
                txt:    NUL�I�[������ւ̃|�C���^�B

  Returns:      txt ������ŏ��̋�`�̕��ƍ����B
 *---------------------------------------------------------------------------*/
NNSG2dTextRect NNSi_G2dFontGetTextRect(
    const NNSG2dFont* pFont,
    int hSpace,
    int vSpace,
    const void* txt
)
{
    int lines = 1;
    NNSG2dTextRect rect = {0, 0};

    NNS_G2D_FONT_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( txt );

    while( txt != NULL )
    {
        const int width = NNSi_G2dFontGetStringWidth(pFont, hSpace, txt, &txt);
        if( width > rect.width )
        {
            rect.width = width;
        }
        lines++;
    }

    // ���� = �s�� x (�s�� + �s��) - �s��
    rect.height = ((lines - 1) * (NNS_G2dFontGetLineFeed(pFont) + vSpace) - vSpace);

    return rect;
}

