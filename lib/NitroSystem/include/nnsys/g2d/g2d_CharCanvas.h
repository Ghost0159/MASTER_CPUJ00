/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_CharCanvas.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.8 $
 *---------------------------------------------------------------------------*/
#ifndef G2D_CHARCANVAS_H_
#define G2D_CHARCANVAS_H_

#include <nnsys/g2d/g2d_Font.h>
#include <nnsys/g2d/g2di_AssertUtil.h>
#include <nnsys/g2d/fmt/g2d_Cell_data.h>

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------

//---------------------------------------------------------------------
// CharCanvas �}�N��
//---------------------------------------------------------------------

#define NNS_G2D_CHARCANVAS_ASSERT( pCC )                                \
    NNS_G2D_ASSERTMSG(                                                  \
        NNS_G2D_IS_VALID_POINTER(pCC)                                   \
        && (0 < (pCC)->areaWidth)                                       \
        && (0 < (pCC)->areaHeight)                                      \
        && ( ((pCC)->dstBpp == 4) || ((pCC)->dstBpp == 8) )             \
        && NNS_G2D_IS_VALID_POINTER( (pCC)->charBase )                  \
        && NNS_G2D_IS_ALIGNED((pCC)->charBase, 4)                       \
        && NNS_G2D_IS_VALID_POINTER( (pCC)->vtable )                    \
        && NNS_G2D_IS_VALID_POINTER( (pCC)->vtable->pDrawGlyph )        \
        && NNS_G2D_IS_VALID_POINTER( (pCC)->vtable->pClear )            \
        && NNS_G2D_IS_VALID_POINTER( (pCC)->vtable->pClearArea )        \
        , "Illegal NNSG2dCharCanvas.")

#define NNS_G2D_COLORMODE_ASSERT( cmode )                               \
    NNS_G2D_ASSERTMSG(                                                  \
        (cmode) == NNS_G2D_CHARA_COLORMODE_16                           \
        || (cmode) == NNS_G2D_CHARA_COLORMODE_256                       \
        , "Illegal Color Mode(=%d)", (cmode) )

#define NNS_G2D_OBJVRAMMODE_ASSERT( vmode )                             \
    NNS_G2D_ASSERTMSG(                                                  \
        (vmode) == NNS_G2D_OBJVRAMMODE_32K                              \
        || (vmode) == NNS_G2D_OBJVRAMMODE_64K                           \
        || (vmode) == NNS_G2D_OBJVRAMMODE_128K                          \
        || (vmode) == NNS_G2D_OBJVRAMMODE_256K                          \
        , "Illegal VRAM Mode(=%d)", (vmode) )

#define NNS_G2D_TEXT_BG_WIDTH_ASSERT( width )                           \
    NNS_G2D_ASSERTMSG(                                                  \
        (width) == NNS_G2D_TEXT_BG_WIDTH_256                            \
        || (width) == NNS_G2D_TEXT_BG_WIDTH_512                         \
        , "Illegal Text BG Width(=%d).", (width) )

#define NNS_G2D_AFFINE_BG_WIDTH_ASSERT( width )                         \
    NNS_G2D_ASSERTMSG(                                                  \
        (width) == NNS_G2D_AFFINE_BG_WIDTH_128                          \
        || (width) == NNS_G2D_AFFINE_BG_WIDTH_256                       \
        || (width) == NNS_G2D_AFFINE_BG_WIDTH_512                       \
        || (width) == NNS_G2D_AFFINE_BG_WIDTH_1024                      \
        , "Illegal Affine BG Width(=%d).", (width) )

#define NNS_G2D_256x16PLTT_BG_WIDTH_ASSERT( width )                     \
    NNS_G2D_ASSERTMSG(                                                  \
        (width) == NNS_G2D_256x16PLTT_BG_WIDTH_128                      \
        || (width) == NNS_G2D_256x16PLTT_BG_WIDTH_256                   \
        || (width) == NNS_G2D_256x16PLTT_BG_WIDTH_512                   \
        || (width) == NNS_G2D_256x16PLTT_BG_WIDTH_1024                  \
        , "Illegal 256x16Pltt BG Width(=%d).", (width) )




// TEXT BG �̕�
typedef enum NNSG2dTextBGWidth
{
    NNS_G2D_TEXT_BG_WIDTH_256   = 32,
    NNS_G2D_TEXT_BG_WIDTH_512   = 64
}
NNSG2dTextBGWidth;

// �A�t�B��BG �̕�
typedef enum NNSG2dAffineBGWidth
{
    NNS_G2D_AFFINE_BG_WIDTH_128     = 16,
    NNS_G2D_AFFINE_BG_WIDTH_256     = 32,
    NNS_G2D_AFFINE_BG_WIDTH_512     = 64,
    NNS_G2D_AFFINE_BG_WIDTH_1024    = 128
}
NNSG2dAffineBGWidth;

// �A�t�B���g�� 256x16�p���b�g�^�C�v BG �̕�
typedef enum NNSG2d256x16PlttBGWidth
{
    NNS_G2D_256x16PLTT_BG_WIDTH_128     = 16,
    NNS_G2D_256x16PLTT_BG_WIDTH_256     = 32,
    NNS_G2D_256x16PLTT_BG_WIDTH_512     = 64,
    NNS_G2D_256x16PLTT_BG_WIDTH_1024    = 128
}
NNSG2d256x16PlttBGWidth;

// �L�����N�^�̃J���[���[�h
typedef enum NNSG2dCharaColorMode
{
    NNS_G2D_CHARA_COLORMODE_16 = 4,
    NNS_G2D_CHARA_COLORMODE_256 = 8
}
NNSG2dCharaColorMode;

// OBJ �L�����N�^�̈�Q�Ɖ\�T�C�Y
typedef enum NNSG2dOBJVramMode
{
    NNS_G2D_OBJVRAMMODE_32K     = 0,
    NNS_G2D_OBJVRAMMODE_64K     = 1,
    NNS_G2D_OBJVRAMMODE_128K    = 2,
    NNS_G2D_OBJVRAMMODE_256K    = 3
}
NNSG2dOBJVramMode;

//---------------------------------------------------------------------
// CharCanvas ��`
//---------------------------------------------------------------------


struct NNSG2dCharCanvas;

// CharCanvas �O���t�`��֐���`
typedef void (*NNSiG2dDrawGlyphFunc)(
    const struct NNSG2dCharCanvas* pCC,
    const NNSG2dFont* pFont,
    int x,
    int y,
    int cl,
    const NNSG2dGlyph* pGlyph
);

// CharCanvas �N���A�֐���`
typedef void (*NNSiG2dClearFunc)(
    const struct NNSG2dCharCanvas* pCC,
    int cl
);

// CharCanvas �����N���A�֐���`
typedef void (*NNSiG2dClearAreaFunc)(
    const struct NNSG2dCharCanvas* pCC,
    int cl,
    int x,
    int y,
    int w,
    int h
);

typedef struct NNSiG2dCharCanvasVTable
{
    NNSiG2dDrawGlyphFunc    pDrawGlyph;
    NNSiG2dClearFunc        pClear;
    NNSiG2dClearAreaFunc    pClearArea;
}
NNSiG2dCharCanvasVTable;

// CharCanvas�\����
typedef struct NNSG2dCharCanvas
{
    u8*     charBase;
    int     areaWidth;
    int     areaHeight;
    u8      dstBpp;
    u8      reserved[3];
    u32     param;
    const NNSiG2dCharCanvasVTable* vtable;
}
NNSG2dCharCanvas;





//****************************************************************************
// BG �X�N���[���\���֐�
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dMapScrToCharText

  Description:  CharCanvas �p�ɃX�N���[������L�����N�^��1��1�ɑΉ������܂��B
                �e�L�X�gBG�p�ł��B

  Arguments:    scnBase:    CharCanvas �����p���Ă��� BG�ʂ�
                            �X�N���[���x�[�X�ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕�
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍���
                areaLeft:   BG�ʂ̍����(0,0)�Ƃ������� CharCanvas �����
                            x ���W�B
                areaTop:    BG�ʂ̍����(0,0)�Ƃ������� CharCanvas �����
                            y ���W�B
                scnWidth:   BG�ʂ̕�
                charNo:     CharCanvas �Ɋ��蓖�Ă��Ă���L�����N�^���
                            �擪�̃L�����N�^�ւ̃|�C���^�B
                cplt:       �X�N���[���Ɏw�肷��J���[�p���b�g�ԍ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dMapScrToCharText(
    void* scnBase,
    int areaWidth,
    int areaHeight,
    int areaLeft,
    int areaTop,
    NNSG2dTextBGWidth scnWidth,
    int charNo,
    int cplt
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dMapScrToCharAffine

  Description:  CharCanvas �p�ɃX�N���[������L�����N�^��1��1�ɑΉ������܂��B
                �A�t�B��BG�p�ł��B

  Arguments:    areaBase:   CharCanvas ����ɂ�����X�N���[���ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕�
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍���
                scnWidth:   BG�ʂ̕�
                charNo:     CharCanvas �Ɋ��蓖�Ă��Ă���L�����N�^���
                            �擪�̃L�����N�^�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dMapScrToCharAffine(
    void* areaBase,
    int areaWidth,
    int areaHeight,
    NNSG2dAffineBGWidth scnWidth,
    int charNo
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dMapScrToChar256x16Pltt

  Description:  CharCanvas �p�ɃX�N���[������L�����N�^��1��1�ɑΉ������܂��B
                �A�t�B���g��BG��256x16�p���b�g�^�C�v�p�ł��B

  Arguments:    areaBase:   CharCanvas ����ɂ�����X�N���[���ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕�
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍���
                scnWidth:   BG�ʂ̕�
                charNo:     CharCanvas �Ɋ��蓖�Ă��Ă���L�����N�^���
                            �擪�̃L�����N�^�ւ̃|�C���^�B
                cplt:       �X�N���[���Ɏw�肷��J���[�p���b�g�ԍ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dMapScrToChar256x16Pltt(
    void* areaBase,
    int areaWidth,
    int areaHeight,
    NNSG2d256x16PlttBGWidth scnWidth,
    int charNo,
    int cplt
);





//****************************************************************************
// OBJ �z��֐�
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dCalcRequiredOBJ
                NNS_G2dCalcRequiredOBJ1D
                NNS_G2dCalcRequiredOBJ2DRect

  Description:  NNS_G2dArrangeOBJ* �ŕK�v��OBJ�����v�Z���܂��B
                CharCanvas �̑傫���������ł���� NNS_G2dArrangeOBJ1D ��
                NNS_G2dArrangeOBJ2DRect �͓����悤��OBJ����ׂ܂��B
                ���̂��� NNS_G2dCalcRequireOBJ1D ��
                NNS_G2dCalcRequireOBJ2DRect �������I�ɂ͓������̂ł��B

  Arguments:    areaWidth:  OBJ�����v�Z���� CharCanvas �̃L�����N�^�P�ʂł̕��B
                areaHeight: OBJ�����v�Z���� CharCanvas �̃L�����N�^�P�ʂł̍����B

  Returns:      �K�v��OBJ���B
 *---------------------------------------------------------------------------*/
int NNSi_G2dCalcRequiredOBJ(
    int areaWidth,
    int areaHeight
);

NNS_G2D_INLINE int NNS_G2dCalcRequiredOBJ1D(
    int areaWidth,
    int areaHeight
)
{
    return NNSi_G2dCalcRequiredOBJ(areaWidth, areaHeight);
}

NNS_G2D_INLINE int NNS_G2dCalcRequiredOBJ2DRect(
    int areaWidth,
    int areaHeight
)
{
    return NNSi_G2dCalcRequiredOBJ(areaWidth, areaHeight);
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dArrangeOBJ1D

  Description:  NNS_G2dCharCanvasInitForOBJ1D �ŕ`�悵��OBJ��K�؂�
                �\���ł���悤��OBJ�𐮗񂳂��܂��B
                oam ���n�_�Ƃ��� NNS_G2dCalcRequireOBJ1D(areaWidth, areaHeight)
                �� OBJ ���g�p���܂��B

  Arguments:    oam:        �g�p����OAM��̎n�_�ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕��B
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍����B
                x:          CharCanvas ������̕\���ʒu�B
                y:          CharCanvas ������̕\���ʒu�B
                color:      CharCanvas �̃J���[���[�h�B
                charName:   OBJ�񂪕\���ɗp����L�����N�^��擪�̃L�����N�^�l�[���B
                vramMode:   OBJ VRAM�e��

  Returns:
 *---------------------------------------------------------------------------*/
int NNS_G2dArrangeOBJ1D(
    GXOamAttr * oam,
    int areaWidth,
    int areaHeight,
    int x,
    int y,
    GXOamColorMode color,
    int charName,
    NNSG2dOBJVramMode vramMode
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dArrangeOBJ2DRect

  Description:  NNS_G2dCharCanvasInitForOBJ2DRect �ŕ`�悵��OBJ��K�؂�
                �\���ł���悤��OBJ�𐮗񂳂��܂��B
                oam ���n�_�Ƃ��� NNS_G2dCalcRequireOBJ2DRect(areaWidth, areaHeight)
                �� OBJ ���g�p���܂��B

  Arguments:    oam:        �g�p����OAM��̎n�_�ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕��B
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍����B
                x:          CharCanvas ������̕\���ʒu�B
                y:          CharCanvas ������̕\���ʒu�B
                color:      CharCanvas �̃J���[���[�h�B
                charName:   OBJ�񂪕\���ɗp����L�����N�^��擪�̃L�����N�^�l�[���B

  Returns:
 *---------------------------------------------------------------------------*/
int NNS_G2dArrangeOBJ2DRect(
    GXOamAttr * oam,
    int areaWidth,
    int areaHeight,
    int x,
    int y,
    GXOamColorMode color,
    int charName
);





//****************************************************************************
// CharCanvas ����
//****************************************************************************

// �`��

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasDrawChar

  Description:  CharCanvas �ɕ����R�[�h�w���1�����`�悵�܂��B

  Arguments:    pCC:    CharCanvas �ւ̃|�C���^�B
                pFont:  �`��ɗp����t�H���g�ւ̃|�C���^�B
                x:      �����̍���̍��W
                y:      �����̍���̍��W
                cl:     �����F�̐F�ԍ��B
                pGlyph: �`�悷�镶���̕����R�[�h�B

  Returns:      �����̕`�敝�B
 *---------------------------------------------------------------------------*/
int NNS_G2dCharCanvasDrawChar(
    const NNSG2dCharCanvas* pCC,
    const NNSG2dFont* pFont,
    int x,
    int y,
    int cl,
    u16 ccode
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasDrawGlyph

  Description:  CharCanvas �Ɏw�肵���O���t��`�悵�܂��B

  Arguments:    pCC:    CharCanvas �ւ̃|�C���^�B
                pFont:  �`��ɗp����t�H���g�ւ̃|�C���^�B
                x:      �����̍���̍��W�B
                y:      �����̍���̍��W�B
                cl:     �����F�̐F�ԍ��B
                pGlyph: �`�悷��O���t�ւ̃|�C���^�B�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dCharCanvasDrawGlyph(
    const NNSG2dCharCanvas* pCC,
    const NNSG2dFont* pFont,
    int x,
    int y,
    int cl,
    const NNSG2dGlyph* pGlyph
)
{
    NNS_G2D_CHARCANVAS_ASSERT( pCC );
    pCC->vtable->pDrawGlyph(pCC, pFont, x, y, cl, pGlyph);
}


// ����

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasClear

  Description:  CharCanvas �ɑ����Ă���L�����N�^�S�̂�
                �w�肳�ꂽ�F�œh��Ԃ��܂��B

  Arguments:    pCC:    CharCanvas �ւ̃|�C���^�B
                cl:     �h��Ԃ��F�̐F�ԍ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dCharCanvasClear(
    const NNSG2dCharCanvas* pCC,
    int cl
)
{
    NNS_G2D_CHARCANVAS_ASSERT( pCC );
    pCC->vtable->pClear(pCC, cl);
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasClearArea

  Description:  CharCanvas �ɑ����Ă���L�����N�^�̎w�肳�ꂽ�̈��
                �w�肳�ꂽ�F�œh��Ԃ��܂��B

  Arguments:    pCC:    CharCanvas �ւ̃|�C���^�B
                cl:     �h��Ԃ��F�̐F�ԍ��B
                x:      �h��Ԃ��̈�̍���� x ���W�B
                y:      �h��Ԃ��̈�̍���� y ���W�B
                w:      �h��Ԃ��̈�̕��B
                h:      �h��Ԃ��̈�̍����B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNS_G2dCharCanvasClearArea(
    const NNSG2dCharCanvas* pCC,
    int cl,
    int x,
    int y,
    int w,
    int h
)
{
    NNS_G2D_CHARCANVAS_ASSERT( pCC );
    pCC->vtable->pClearArea(pCC, cl, x, y, w, h);
}



//----------------------------------------------------------------------------
// �\�z
//----------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasInitForBG

  Description:  CharCanvas �����������܂��B
                BG�p�Ƀp�����[�^�𒼐ڎw�肵�ď��������܂��B

  Arguments:    pCC:        CharCanvas �ւ̃|�C���^�B
                charBase:   CharCanvas �Ɋ��蓖�Ă�L�����N�^���
                            �擪�̃L�����N�^�ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕��B
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍����B
                colorMode:  �`��ΏۃL�����N�^�̃J���[���[�h�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dCharCanvasInitForBG(
    NNSG2dCharCanvas* pCC,
    void* charBase,
    int areaWidth,
    int areaHeight,
    NNSG2dCharaColorMode colorMode
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasInitForOBJ1D

  Description:  CharCanvas ��1D�}�b�s���OOBJ�Ŏg�p���邽�߂ɏ��������܂��B

  Arguments:    pCC:        ���������� CharCanvas �ւ̃|�C���^�B
                charBase:   CharCanvas�Ɋ��蓖�Ă�L�����N�^�̎n�_�ւ̃|�C���^�B
                areaWidth:  CharCanvas�̃L�����N�^�P�ʂł̕��B
                areaHeight: CharCanvas�̃L�����N�^�P�ʂł̍����B
                colorMode:  �`��ΏۃL�����N�^�̃J���[���[�h�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dCharCanvasInitForOBJ1D(
    NNSG2dCharCanvas* pCC,
    void* charBase,
    int areaWidth,
    int areaHeight,
    NNSG2dCharaColorMode colorMode
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasInitForOBJ2DRect

  Description:  CharCanvas �����������܂��B
                2�����}�b�s���O��OBJ�ŃL�����N�^�[���������̋�`��
                CharCanvas �Ƃ��鎞�ɗp���܂��B

  Arguments:    pCC:        CharCanvas �ւ̃|�C���^�B
                charBase:   CharCanvas �̍���̃L�����N�^�ւ̃|�C���^�B
                areaWidth:  CharCanvas �̃L�����N�^�P�ʂł̕��B
                areaHeight: CharCanvas �̃L�����N�^�P�ʂł̍����B
                colorMode:  �`��ΏۃL�����N�^�̃J���[���[�h�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dCharCanvasInitForOBJ2DRect(
    NNSG2dCharCanvas* pCC,
    void* charBase,
    int areaWidth,
    int areaHeight,
    NNSG2dCharaColorMode colorMode
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasMakeCell1D

  Description:  NNS_G2dCharCanvasInitForOBJ1D() �ŏ��������� CharCanvas ��
                �\�����邽�߂̃Z�����쐬���܂��B

  Arguments:    pCell:      ���������Z���f�[�^���i�[����o�b�t�@
                pCC:        CharCanvas �ւ̃|�C���^
                x:          �Z���̒��S���W(CharCanvas���W�n)
                y:          �Z���̒��S���W(CharCanvas���W�n)
                priority:   �Z���̃v���C�I���e�B
                mode:       �Z���̃��[�h
                mosaic:     �Z���̃��U�C�N
                effect:     �Z���̃G�t�F�N�g
                color:      CharCanvas �̃J���[���[�h
                charName:   �擪�L�����N�^�l�[��
                cParam:     �J���[�p���b�g�ԍ�
                vramMode:   OBJ VRAM�e�ʂ̐ݒ���w�肵�܂��B
                makeBR:     ���E��`����t�����邩�ǂ������w�肵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dCharCanvasMakeCell1D(
    NNSG2dCellData* pCell,
    const NNSG2dCharCanvas* pCC,
    int x,
    int y,
    int priority,
    GXOamMode mode,
    BOOL mosaic,
    GXOamEffect effect,
    GXOamColorMode color,
    int charName,
    int cParam,
    NNSG2dOBJVramMode vramMode,
    BOOL makeBR
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasMakeCell2DRect

  Description:  NNS_G2dCharCanvasInitForOBJ2DRect() �ŏ��������� CharCanvas ��
                �\�����邽�߂̃Z�����쐬���܂��B

  Arguments:    pCell:      ���������Z���f�[�^���i�[����o�b�t�@
                pCC:        CharCanvas �ւ̃|�C���^
                x:          �Z���̒��S���W(CharCanvas���W�n)
                y:          �Z���̒��S���W(CharCanvas���W�n)
                priority:   �Z���̃v���C�I���e�B
                mode:       �Z���̃��[�h
                mosaic:     �Z���̃��U�C�N
                effect:     �Z���̃G�t�F�N�g
                color:      CharCanvas �̃J���[���[�h
                charName:   �擪�L�����N�^�l�[��
                cParam:     �J���[�p���b�g�ԍ�
                makeBR:     ���E��`����t�����邩�ǂ������w�肵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NNS_G2dCharCanvasMakeCell2DRect(
    NNSG2dCellData* pCell,
    const NNSG2dCharCanvas* pCC,
    int x,
    int y,
    int priority,
    GXOamMode mode,
    BOOL mosaic,
    GXOamEffect effect,
    GXOamColorMode color,
    int charName,
    int cParam,
    BOOL makeBR
);



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dCharCanvasCalcCellDataSize*

  Description:  NNS_G2dCharCanvasMakeCell*() �̑������ɓn���ׂ�
                �o�b�t�@�̃T�C�Y���v�Z���܂��B

  Arguments:    pCC:        CharCanvas �ւ̃|�C���^
                makeBR:     ���E��`����t�����邩�ǂ������w�肵�܂��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE size_t NNSi_G2dCharCanvasCalcCellDataSize(
    const NNSG2dCharCanvas* pCC,
    BOOL makeBR
)
{
    const int numObj        = NNSi_G2dCalcRequiredOBJ(pCC->areaWidth, pCC->areaHeight);
    const size_t oamSize    = sizeof(NNSG2dCellOAMAttrData) * numObj;
    const size_t brSize     = makeBR ? sizeof(NNSG2dCellBoundingRectS16): 0;

    return sizeof(NNSG2dCellData) + brSize + oamSize;
}

NNS_G2D_INLINE size_t NNS_G2dCharCanvasCalcCellDataSize1D(
    const NNSG2dCharCanvas* pCC,
    BOOL makeBR
)
{
    return NNSi_G2dCharCanvasCalcCellDataSize(pCC, makeBR);
}

NNS_G2D_INLINE size_t NNS_G2dCharCanvasCalcCellDataSize2DRect(
    const NNSG2dCharCanvas* pCC,
    BOOL makeBR
)
{
    return NNSi_G2dCharCanvasCalcCellDataSize(pCC, makeBR);
}






//---------------------------------------------------------------------

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // G2D_CHARCANVAS_H_

