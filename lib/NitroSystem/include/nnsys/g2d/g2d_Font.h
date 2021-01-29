/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_Font.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/
#ifndef G2D_FONT_H_
#define G2D_FONT_H_

#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/g2di_AssertUtil.h>
#include <nnsys/g2d/fmt/g2d_Font_data.h>
#include <nnsys/g2d/g2di_SplitChar.h>
#include <nnsys/g2d/g2di_Char.h>

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------

//*****************************************************************************
// Font �}�N��
//*****************************************************************************

#define NNS_G2D_FONT_ASSERT( pFont )                                            \
    NNS_G2D_ASSERTMSG(                                                          \
        ((pFont) != NULL)                                                       \
            && (*(((u32*)((pFont)->pRes)) - 2) == NNS_G2D_BINBLK_SIG_FINFDATA)  \
            && ((pFont)->pRes->pGlyph != NULL)                                  \
            && NNS_G2D_IS_VALID_POINTER((pFont)->cbCharSpliter)                 \
        , "invalid NNSG2dFont data" )

#define NNS_G2D_GLYPH_ASSERT( pGlyph )                                          \
    NNS_G2D_ASSERTMSG(                                                          \
        ((pGlyph) != NULL)                                                      \
            && ((pGlyph)->image != NULL)                                        \
        , "invalid NNSG2dGlyph data")                                           \

#define NNS_G2D_FONT_MAX_GLYPH_INDEX( pFont )                                   \
    ((*((u32*)(pFont)->pRes->pGlyph - 1) - sizeof(*((pFont)->pRes->pGlyph)))    \
        / (pFont)->pRes->pGlyph->cellSize)                                      \


#define NNS_G2D_GLYPH_INDEX_NOT_FOUND   0xFFFF





//*****************************************************************************
// Font ��`
//*****************************************************************************

// �t�H���g
typedef struct NNSG2dFont
{
    NNSG2dFontInformation*   pRes;          // �W�J�ς݃t�H���g���\�[�X�ւ̃|�C���^
    NNSiG2dSplitCharCallback cbCharSpliter; // ������G���R�[�f�B���O�����R�[���o�b�N�ւ̃|�C���^
}
NNSG2dFont;



// �O���t
typedef struct NNSG2dGlyph
{
    const NNSG2dCharWidths* pWidths;    // �����ւ̃|�C���^
    const u8* image;                    // �O���t�C���[�W�ւ̃|�C���^
}
NNSG2dGlyph;



// �������`
typedef struct NNSG2dTextRect
{
    int width;      // ��`��
    int height;     // ��`����
}
NNSG2dTextRect;





//*****************************************************************************
// Font �\�z
//*****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontInit*

  Description:  �t�H���g���\�[�X����t�H���g���\�z���܂��B

  Arguments:    pFont:      �t�H���g�ւ̃|�C���^�B
                pNftrFile:  �t�H���g���\�[�X�ւ̃|�C���^�B

  Returns:      �t�H���g�̍\�z�ɐ��������ꍇ��FALSE�ȊO��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
void NNS_G2dFontInitAuto(NNSG2dFont* pFont, void* pNftrFile);
void NNS_G2dFontInitUTF8(NNSG2dFont* pFont, void* pNftrFile);
void NNS_G2dFontInitUTF16(NNSG2dFont* pFont, void* pNftrFile);
void NNS_G2dFontInitShiftJIS(NNSG2dFont* pFont, void* pNftrFile);
void NNS_G2dFontInitCP1252(NNSG2dFont* pFont, void* pNftrFile);





//*****************************************************************************
// Font ����
//*****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontFindGlyphIndex

  Description:  �����R�[�h����Ή�����O���t�̃C���f�b�N�X���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      �O���t�C���f�b�N�X���擾���镶���R�[�h�B

  Returns:      c �ɑΉ�����O���t�����݂���ꍇ�͂��̃C���f�b�N�X�B
                ���݂��Ȃ��ꍇ�� NNS_G2D_GLYPH_INDEX_NOT_FOUND�B
 *---------------------------------------------------------------------------*/
u16 NNS_G2dFontFindGlyphIndex( const NNSG2dFont* pFont, u16 c );



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCharWidthsFromIndex

  Description:  �O���t�C���f�b�N�X���當���������擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                idx:    �O���t�C���f�b�N�X�B

  Returns:      ���������ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
const NNSG2dCharWidths* NNS_G2dFontGetCharWidthsFromIndex(
                                        const NNSG2dFont* pFont, u16 idx );





//*****************************************************************************
// Font �A�N�Z�T
//*****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetType

  Description:  �t�H���g�̃^�C�v���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �t�H���g�̃^�C�v�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSG2dFontType NNS_G2dFontGetType( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return (NNSG2dFontType)pFont->pRes->fontType;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetLineFeed

  Description:  �t�H���g�̉��s�����擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �t�H���g�̉��s���B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE s8 NNS_G2dFontGetLineFeed( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->linefeed;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetAlternateGlyphIndex

  Description:  �t�H���g�̑�֕����O���t�C���f�b�N�X���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �t�H���g�̑�֕����O���t�C���f�b�N�X�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u16 NNS_G2dFontGetAlternateGlyphIndex( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->alterCharIndex;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetDefaultCharWidths

  Description:  �f�t�H���g�̕����������擾���܂��B
                �f�t�H���g�̕��������͕������ŗL�̕��������������Ȃ��ꍇ��
                �g�p����܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �f�t�H���g�̕��������ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSG2dCharWidths* NNS_G2dFontGetDefaultCharWidths( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return &pFont->pRes->defaultWidth;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetHeight

  Description:  �t�H���g�̍������擾���܂��B
                �ʏ�A�O���t�C���[�W�̍������t�H���g�̍����ɂȂ�܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �t�H���g�̍����B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dFontGetHeight( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->cellHeight;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCellHeight

  Description:  �O���t�C���[�W�̍������擾���܂��B
                �O���t�C���[�W�̑傫���̓t�H���g���̑S�Ă̕����ŋ��ʂł��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�C���[�W�̍����B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dFontGetCellHeight( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->cellHeight;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCellWidth

  Description:  �O���t�C���[�W�̕����擾���܂��B
                �O���t�C���[�W�̑傫���̓t�H���g���̑S�Ă̕����ŋ��ʂł��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�C���[�W�̕��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dFontGetCellWidth( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->cellWidth;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetBaselinePos

  Description:  �O���t�C���[�W�̏�[����݂��x�[�X���C���̈ʒu���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�C���[�W�̏�[�� 0 �Ƃ����x�[�X���C���̈ʒu
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE int NNS_G2dFontGetBaselinePos( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT( pFont );
    return pFont->pRes->pGlyph->baselinePos;
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetGlyphDataSize

  Description:  �O���t�C���[�W��1����������̃o�C�g�T�C�Y���擾���܂��B
                �O���t�C���[�W�̑傫���̓t�H���g���̑S�Ă̕����ŋ��ʂł��B

                (CellWidth * CellHeight * bpp + 7) / 8 �Ɠ������Ȃ�܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�C���[�W��1����������̃o�C�g�T�C�Y�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE int NNSi_G2dFontGetGlyphDataSize( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->cellSize;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetMaxCharWidth

  Description:  �t�H���g���̍ő啶�������擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �t�H���g���̍ő啶�����B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dFontGetMaxCharWidth( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->maxCharWidth;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetBpp

  Description:  �O���t�C���[�W��1�h�b�g������̃r�b�g�����擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�C���[�W��1�h�b�g������̃r�b�g���B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dFontGetBpp( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->bpp;
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetEncoding

  Description:  �t�H���g���Ή����镶����G���R�[�f�B���O���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �t�H���g���Ή����镶����G���R�[�f�B���O�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSG2dFontEncoding NNSi_G2dFontGetEncoding( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return (NNSG2dFontEncoding)pFont->pRes->encoding;
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetSpliter

  Description:  ������G���R�[�f�B���O�����R�[���o�b�N�֐��ւ̃|�C���^��
                �擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      ������G���R�[�f�B���O�����R�[���o�b�N�֐��ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE NNSiG2dSplitCharCallback NNSi_G2dFontGetSpliter( const NNSG2dFont* pFont )
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->cbCharSpliter;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetGlyphImageFromIndex

  Description:  �O���t�C���f�b�N�X����O���t�C���[�W���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                idx:    �O���t�C���f�b�N�X�B

  Returns:      �O���t�C���[�W�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const u8* NNS_G2dFontGetGlyphImageFromIndex(
                                        const NNSG2dFont* pFont, u16 idx )
{
    NNS_G2D_FONT_ASSERT(pFont);
    NNS_G2D_ASSERT( idx < NNS_G2D_FONT_MAX_GLYPH_INDEX(pFont) );
    return pFont->pRes->pGlyph->glyphTable + idx * NNSi_G2dFontGetGlyphDataSize(pFont);
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetFlags

  Description:  �O���t�̓����t���O���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�̓����t���O��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u8 NNS_G2dFontGetFlags(const NNSG2dFont* pFont)
{
    NNS_G2D_FONT_ASSERT(pFont);
    return pFont->pRes->pGlyph->flags;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontSetLineFeed

  Description:  �t�H���g�̉��s����ݒ肵�܂��B

  Arguments:    pFont:      �t�H���g�ւ̃|�C���^�B
                linefeed:   �V�������s��

  Returns:      �t�H���g�̉��s���B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dFontSetLineFeed( NNSG2dFont* pFont, s8 linefeed )
{
    NNS_G2D_FONT_ASSERT(pFont);
    pFont->pRes->linefeed = linefeed;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontSetDefaultCharWidths

  Description:  �f�t�H���g�̕���������ݒ肵�܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                gw:     �V�����f�t�H���g���������B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dFontSetDefaultCharWidths(
                                NNSG2dFont* pFont, NNSG2dCharWidths cw)
{
    NNS_G2D_FONT_ASSERT(pFont);
    pFont->pRes->defaultWidth = cw;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontSetAlternateGlyphIndex

  Description:  �t�H���g�̑�֕����O���t�C���f�b�N�X��ݒ肵�܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                idx:    �V������֕����O���t�C���f�b�N�X�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dFontSetAlternateGlyphIndex( NNSG2dFont* pFont, u16 idx )
{
    NNS_G2D_FONT_ASSERT(pFont);
    NNS_G2D_ASSERT( idx < NNS_G2D_FONT_MAX_GLYPH_INDEX(pFont) );
    pFont->pRes->alterCharIndex = idx;
}





//*****************************************************************************
// inline function
//*****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetGlyphIndex

  Description:  �����R�[�h����O���t�C���f�b�N�X���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      �����R�[�h�B

  Returns:      �O���t�C���f�b�N�X�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE u16 NNS_G2dFontGetGlyphIndex( const NNSG2dFont* pFont, u16 c )
{
    const u16 idx = NNS_G2dFontFindGlyphIndex(pFont, c);

    return (idx != NNS_G2D_GLYPH_INDEX_NOT_FOUND) ?
                idx :
                pFont->pRes->alterCharIndex ;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetGlyphFromIndex

  Description:  �O���t�C���f�b�N�X����O���t���擾���܂��B

  Arguments:    pGlyph: �O���t���i�[����o�b�t�@�ւ̃|�C���^�B
                pFont:  �t�H���g�ւ̃|�C���^�B
                idx:    �擾����O���t�̃O���t�C���f�b�N�X�B

  Returns:      �O���t�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dFontGetGlyphFromIndex(
                        NNSG2dGlyph* pGlyph, const NNSG2dFont* pFont, u16 idx )
{
    NNS_G2D_FONT_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( pGlyph );
    pGlyph->pWidths = NNS_G2dFontGetCharWidthsFromIndex(pFont, idx);
    pGlyph->image = NNS_G2dFontGetGlyphImageFromIndex(pFont, idx);
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCharWidths

  Description:  �����R�[�h���當���������擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      �����������擾���镶���̕����R�[�h

  Returns:      ���������B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const NNSG2dCharWidths* NNS_G2dFontGetCharWidths(
                                            const NNSG2dFont* pFont, u16 c )
{
    u16 iGlyph;

    NNS_G2D_FONT_ASSERT( pFont );

    iGlyph = NNS_G2dFontGetGlyphIndex( pFont, c );
    return NNS_G2dFontGetCharWidthsFromIndex( pFont, iGlyph );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCharWidth

  Description:  �����R�[�h���當�������擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      ���������擾���镶���̕����R�[�h

  Returns:      �s�N�Z���P�ʂ̕������B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE int NNS_G2dFontGetCharWidth( const NNSG2dFont* pFont, u16 c )
{
    const NNSG2dCharWidths* pWidths;

    NNS_G2D_FONT_ASSERT( pFont );

    pWidths = NNS_G2dFontGetCharWidths(pFont, c);
    return pWidths->charWidth;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCharWidthFromIndex

  Description:  �O���t�C���f�b�N�X���當�������擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                idx:    ���������擾���镶���̃O���t�C���f�b�N�X

  Returns:      �s�N�Z���P�ʂ̕������B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE int NNS_G2dFontGetCharWidthFromIndex( const NNSG2dFont* pFont, u16 idx )
{
    const NNSG2dCharWidths* pWidths;

    NNS_G2D_FONT_ASSERT( pFont );

    pWidths = NNS_G2dFontGetCharWidthsFromIndex(pFont, idx);
    return pWidths->charWidth;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetGlyphImage

  Description:  �����R�[�h����O���t�C���[�W���擾���܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B

  Returns:      �O���t�C���[�W�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE const u8* NNS_G2dFontGetGlyphImage( const NNSG2dFont* pFont, u16 c )
{
    u16 iGlyph;

    NNS_G2D_FONT_ASSERT( pFont );

    iGlyph = NNS_G2dFontGetGlyphIndex( pFont, c );
    return NNS_G2dFontGetGlyphImageFromIndex( pFont, iGlyph );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetGlyph

  Description:  �����R�[�h����O���t���擾���܂��B

  Arguments:    pGlyph: �O���t���i�[����o�b�t�@�ւ̃|�C���^�B
                pFont:  �t�H���g�ւ̃|�C���^�B
                ccode:  �O���t���擾���镶���̕����R�[�h

  Returns:      �O���t�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dFontGetGlyph(
                    NNSG2dGlyph* pGlyph, const NNSG2dFont* pFont, u16 ccode )
{
    u16 iGlyph;

    NNS_G2D_FONT_ASSERT( pFont );
    NNS_G2D_POINTER_ASSERT( pGlyph );

    iGlyph = NNS_G2dFontGetGlyphIndex(pFont, ccode);
    NNS_G2dFontGetGlyphFromIndex(pGlyph, pFont, iGlyph);
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontSetAlternateChar

  Description:  ��֕������w�肵�������R�[�h�̕����ɒu�������܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      �V������֕����̕����R�[�h�B

  Returns:      ��֕����̒u�������ɐ�������� TRUE�B
                �t�H���g���� c �ɑΉ����镶�������݂��Ȃ��ꍇ�Ɏ��s���܂��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE BOOL NNS_G2dFontSetAlternateChar( NNSG2dFont* pFont, u16 c )
{
    u16 iGlyph;

    NNS_G2D_FONT_ASSERT(pFont);

    iGlyph = NNS_G2dFontFindGlyphIndex(pFont, c);

    if( iGlyph == NNS_G2D_GLYPH_INDEX_NOT_FOUND )
    {
        return FALSE;
    }

    pFont->pRes->alterCharIndex = iGlyph;
    return TRUE;
}






/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFontGetCharWidthFromIndex

  Description:  ��֕������w�肵�������R�[�h�̕����ɒu�������܂��B

  Arguments:    pFont:  �t�H���g�ւ̃|�C���^�B
                c:      �V������֕����̕����R�[�h�B

  Returns:      ��֕����̒u�������ɐ�������� TRUE�B
                �t�H���g���� c �ɑΉ����镶�������݂��Ȃ��ꍇ�Ɏ��s���܂��B
 *---------------------------------------------------------------------------*/





//*****************************************************************************
// inline function
//*****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetStringWidth

  Description:  �w�肳�ꂽ�����ŕ������1�s�`�悵���ꍇ�̕������߂܂��B
                �����ɍs�̏I�[�ʒu�̃|�C���^���擾���܂��B

  Arguments:    pFont:      �����񕝂̌v�Z�ɗp����t�H���g�ւ̃|�C���^�B
                hSpace:     �s�N�Z���P�ʂ̕����ƕ����̊Ԋu�B
                str:        �����񕝂����߂镶����ւ̃|�C���^�B
                pPos:       �s�I�[�ʒu�ւ̃|�C���^���󂯎��o�b�t�@�ւ̃|�C���^�B
                            NULL�w��B

  Returns:      str ����1�s���̕������`�悵���ꍇ�̃s�N�Z���P�ʂł̕��B
 *---------------------------------------------------------------------------*/
int NNSi_G2dFontGetStringWidth(
    const NNSG2dFont* pFont,
    int hSpace,
    const void* str,
    const void** pPos
);



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetTextHeight

  Description:  �w�肳�ꂽ�����ŕ������`�悵���ꍇ�̍��������߂܂��B

  Arguments:    pFont:  �����񕝂̌v�Z�ɗp����t�H���g�ւ̃|�C���^�B
                vSpace: �s�N�Z���P�ʂ̍s�ƍs�̊Ԋu�B
                txt:    �����񕝂����߂镶����ւ̃|�C���^�B

  Returns:      �������`�悵���ꍇ�̃s�N�Z���P�ʂł̍����B
 *---------------------------------------------------------------------------*/
int NNSi_G2dFontGetTextHeight(
    const NNSG2dFont* pFont,
    int vSpace,
    const void* txt
);



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetTextWidth

  Description:  �w�肳�ꂽ�����ŕ������`�悵���ꍇ�̕������߂܂��B

  Arguments:    pFont:  �����񕝂̌v�Z�ɗp����t�H���g�ւ̃|�C���^�B
                hSpace: �s�N�Z���P�ʂ̕����ƕ����̊Ԋu�B
                txt:    �����񕝂����߂镶����ւ̃|�C���^�B

  Returns:      �������`�悵���ꍇ�̃s�N�Z���P�ʂł̕��B
                ����͊e�s�̕��̂����ő�̂��̂��Ӗ����܂��B
 *---------------------------------------------------------------------------*/
int NNSi_G2dFontGetTextWidth(
    const NNSG2dFont* pFont,
    int hSpace,
    const void* txt
);



/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dFontGetTextRect

  Description:  �w�肳�ꂽ�����ŕ������`�悵���ꍇ�̕��ƍ��������߂܂��B

  Arguments:    pFont:  �����񕝂̌v�Z�ɗp����t�H���g�ւ̃|�C���^�B
                hSpace: �s�N�Z���P�ʂ̕����ƕ����̊Ԋu�B
                vSpace: �s�N�Z���P�ʂ̍s�ƍs�̊Ԋu�B
                txt:    �����񕝂����߂镶����ւ̃|�C���^�B

  Returns:      �������`�悵���ꍇ�̃s�N�Z���P�ʂł̕��ƍ������i�[����
                �\���́B
 *---------------------------------------------------------------------------*/
NNSG2dTextRect NNSi_G2dFontGetTextRect(
    const NNSG2dFont* pFont,
    int hSpace,
    int vSpace,
    const void* txt
);



//---------------------------------------------------------------------

NNS_G2D_INLINE int NNS_G2dFontGetStringWidth( const NNSG2dFont* pFont, int hSpace, const NNSG2dChar* str, const NNSG2dChar** pPos )
    { return NNSi_G2dFontGetStringWidth(pFont, hSpace, str, (const void**)pPos); }
NNS_G2D_INLINE int NNS_G2dFontGetTextHeight( const NNSG2dFont* pFont, int vSpace, const NNSG2dChar* txt )
    { return NNSi_G2dFontGetTextHeight(pFont, vSpace, txt); }
NNS_G2D_INLINE int NNS_G2dFontGetTextWidth( const NNSG2dFont* pFont, int hSpace, const NNSG2dChar* txt )
    { return NNSi_G2dFontGetTextWidth(pFont, hSpace, txt); }
NNS_G2D_INLINE NNSG2dTextRect NNS_G2dFontGetTextRect( const NNSG2dFont* pFont, int hSpace, int vSpace, const NNSG2dChar* txt )
    { return NNSi_G2dFontGetTextRect(pFont, hSpace, vSpace, txt); }

//---------------------------------------------------------------------




//---------------------------------------------------------------------

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // G2D_FONT_H_

