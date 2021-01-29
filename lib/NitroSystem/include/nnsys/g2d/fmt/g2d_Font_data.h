/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d - fmt
  File:     g2d_Font_data.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_G2D_FONT_DATA_H_
#define NNS_G2D_FONT_DATA_H_

//#include <nitro/types.h>
#include <nnsys/g2d/fmt/g2d_Common_data.h>

#ifdef _MSC_VER // for VC
    #pragma warning( disable: 4200 )
	#pragma warning( disable: 4201 )
#endif  //_MSC_VER

#ifndef SDK_ARM9
    #include <nitro_win32.h>
#endif //!SDK_ARM9

#ifdef __cplusplus
extern "C" {
#endif



//------------------------------------------------------------------------------
// �V�O�l�`����`
//------------------------------------------------------------------------------

// Binary file signature
#define NNS_G2D_BINFILE_SIG_FONTDATA          (u32)'NFTR'

// Binary block signature
#define NNS_G2D_BINBLK_SIG_FINFDATA           (u32)'FINF'
#define NNS_G2D_BINBLK_SIG_CGLPDATA           (u32)'CGLP'
#define NNS_G2D_BINBLK_SIG_TGLPDATA           (u32)'TGLP'
#define NNS_G2D_BINBLK_SIG_TCLPDATA           (u32)'TCLP'
#define NNS_G2D_BINBLK_SIG_CWDHDATA           (u32)'CWDH'
#define NNS_G2D_BINBLK_SIG_CMAPDATA           (u32)'CMAP'


#define NNS_G2D_BINFILE_EXT_FONTDATA          "NFTR"





//------------------------------------------------------------------------------
// ���\�[�X�o�[�W������`
//------------------------------------------------------------------------------

//
// �o�[�W�������
// Ver         �ύX�_
// -------------------------------------
// 1.0         ����
// 1.1         ��]/�c�����̏���ǉ�
//
#define NNS_G2D_NFTR_MAJOR_VER              (u8)1
#define NNS_G2D_NFTR_MINOR_VER              (u8)1

#define NNS_G2D_NFTR_VER        (u16)((NNS_G2D_NFTR_MAJOR_VER << 8) | NNS_G2D_NFTR_MINOR_VER)




//------------------------------------------------------------------------------
// enum ��`
//------------------------------------------------------------------------------

// �t�H���g�^�C�v
typedef enum NNSG2dFontType
{
    NNS_G2D_FONTTYPE_GLYPH,     // BMP
    NNS_G2D_NUM_OF_FONTTYPE
}
NNSG2dFontType;

// ������G���R�[�f�B���O�^�C�v
typedef enum NNSG2dFontEncoding
{
    NNS_G2D_FONT_ENCODING_UTF8,			// UTF-8
    NNS_G2D_FONT_ENCODING_UTF16,		// UTF-16
    NNS_G2D_FONT_ENCODING_SJIS,			// ShiftJIS
	NNS_G2D_FONT_ENCODING_CP1252,	    // CP1252
    NNS_G2D_NUM_OF_ENCODING
}
NNSG2dFontEncoding;

// �����R�[�h�}�b�v�^�C�v
typedef enum NNSG2dFontMappingMethod
{
    NNS_G2D_MAPMETHOD_DIRECT,       // �C���f�b�N�X = �����R�[�h - �I�t�Z�b�g
    NNS_G2D_MAPMETHOD_TABLE,        // �C���f�b�N�X = mapInfo[�����R�[�h - �I�t�Z�b�g]
    NNS_G2D_MAPMETHOD_SCAN,         // �C���f�b�N�X = search(mapInfo, �����R�[�h)
    NNS_G2D_NUM_OF_MAPMETHOD
}
NNSG2dFontMappingMethod;

// �c�����Ɖ�]�Ɋւ���t���O
typedef enum NNSG2dFontGlyphFlag
{
    NNS_G2D_FONT_FLAG_TBRL      = (1 << 0),     // �c�����p�̃t�H���g
    NNS_G2D_FONT_FLAG_ROT_0     = (0 << 1),     // ���v���   0 �x��]�̃t�H���g
    NNS_G2D_FONT_FLAG_ROT_90    = (1 << 1),     // ���v���  90 �x��]�̃t�H���g
    NNS_G2D_FONT_FLAG_ROT_180   = (2 << 1),     // ���v��� 180 �x��]�̃t�H���g
    NNS_G2D_FONT_FLAG_ROT_270   = (3 << 1),     // ���v��� 270 �x��]�̃t�H���g
    NNS_G2D_FONT_FLAG_ROT_MASK  = (3 << 1)      // ��]�}�X�N
}
NNSG2dFontGlyphFlag;


//------------------------------------------------------------------------------
// �\���̒�`
//------------------------------------------------------------------------------
// ���������
typedef struct NNSG2dCharWidths
{
    s8 left;                            // �����̍��X�y�[�X�̕�
    u8 glyphWidth;                      // �����̃O���t��
    s8 charWidth;                       // �����̕� = ���X�y�[�X�� + �O���t�� + �E�X�y�[�X��
}
NNSG2dCharWidths;

// �����R�[�h�ƃO���t�C���f�b�N�X�y�A
typedef struct NNSG2dCMapScanEntry
{
    u16 ccode;                          // �����R�[�h
    u16 index;                          // �O���t�C���f�b�N�X
}
NNSG2dCMapScanEntry;

// NNS_G2D_MAPMETHOD_SCAN �̏ꍇ�� NNSG2dFontCodeMap.mapInfo
typedef struct NNSG2dCMapInfoScan
{
    u16 num;                            // entries �̗v�f��
    NNSG2dCMapScanEntry entries[];      // �����R�[�h����O���t�C���f�b�N�X�ւ̃}�b�s���O���X�g
}
NNSG2dCMapInfoScan;





//------------------------------------------------------------------------------
// �t�H���g���\�[�X
//------------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNSG2dFontGlyph

  Description:  �t�H���g�̃O���tBMP���i�[���܂��B
                �S�ẴO���tBMP�͕��ƍ������������A�䂦�Ƀf�[�^�T�C�Y��
                ���������ߔz��Ƃ��ăA�N�Z�X�ł��܂��B
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dFontGlyph
{
    u8  cellWidth;          // �O���t�C���[�W1���C���̃s�N�Z����
    u8  cellHeight;         // �O���t�C���[�W�̃��C����
    u16 cellSize;           // 1�O���t������̃O���t�C���[�W�f�[�^�T�C�Y
    s8  baselinePos;        // (*)�O���t�C���[�W��[����x�[�X���C���܂ł̋���
    u8  maxCharWidth;       // (*)�ő啶����
    u8  bpp;                // �O���t�C���[�W1�s�N�Z���������bit��
    u8  flags;              // NNSG2dFontGlyphFlag

    u8  glyphTable[];       // �O���t�C���[�W�̔z��
}
NNSG2dFontGlyph;



/*---------------------------------------------------------------------------*
  Name:         NNSG2dFontWidth

  Description:  �e�����̕����������i�[���܂��B
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dFontWidth
{
    u16                     indexBegin;     // widthTable �̍ŏ��̃G���g�����Ή�����O���t�C���f�b�N�X
    u16                     indexEnd;       // widthTable �̍Ō�̃G���g�����Ή�����O���t�C���f�b�N�X
    struct NNSG2dFontWidth* pNext;          // ���� NNSG2dFontWidth �ւ̃|�C���^
    NNSG2dCharWidths        widthTable[];   // �����̔z��
}
NNSG2dFontWidth;



/*---------------------------------------------------------------------------*
  Name:         NNSG2dFontCodeMap

  Description:  �����R�[�h����A�����R�[�h�ɑΉ�����O���t��
                �O���t�C���[�W�z�񒆂̃C���f�b�N�X�ւ̃}�b�s���O���K�肵�܂��B
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dFontCodeMap
{
    u16                         ccodeBegin;     // ���̃u���b�N���S�����镶���R�[�h�͈͂̍ŏ��̕����R�[�h
    u16                         ccodeEnd;       // ���̃u���b�N���S�����镶���R�[�h�͈͂̍Ō�̕����R�[�h
    u16                         mappingMethod;  // �}�b�s���O���@ (NNSG2dFontMappingMethod�^)
    u16                         reserved;       // �\��
    struct NNSG2dFontCodeMap*   pNext;          // ���� NNSG2dFontCodeMap �ւ̃|�C���^
    u16                         mapInfo[];      // �����R�[�h�}�b�s���O��� ��̓I�ȓ��e�� mappingMethod �Ɉ˂�
}
NNSG2dFontCodeMap;



/*---------------------------------------------------------------------------*
  Name:         NNSG2dFontInformation

  Description:  �t�H���g�S�̂ɓn������i�[���܂��B
 *---------------------------------------------------------------------------*/
typedef struct NNSG2dFontInformation
{
    u8                  fontType;       // �O���t�f�[�^�^�C�v (NNSG2dFontType�^)
    s8                  linefeed;       // (*)== leading
    u16                 alterCharIndex; // (*)�t�H���g�Ɋ܂܂�Ȃ������p�̃O���t�̃O���t�C���f�b�N�X
    NNSG2dCharWidths    defaultWidth;   // (*)���������������Ȃ��O���t�p�̕��������
    u8                  encoding;       // �Ή����镶����G���R�[�f�B���O (NNSG2dFontEncoding�^)
    NNSG2dFontGlyph*    pGlyph;         // �B��� NNSG2dFontGlyph �ւ̃|�C���^
    NNSG2dFontWidth*    pWidth;         // �ŏ��� NNSG2dFontWidth �ւ̃|�C���^
    NNSG2dFontCodeMap*  pMap;           // �ŏ��� NNSG2dFontCodeMap �ւ̃|�C���^
}
NNSG2dFontInformation;




//------------------------------------------------------------------------------
// NITRO-System �o�C�i���t�@�C�� �u���b�N
//------------------------------------------------------------------------------

typedef struct NNSG2dFontInformationBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dFontInformation       blockBody;
}
NNSG2dFontInformationBlock;

typedef struct NNSG2dFontGlyphBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dFontGlyph             blockBody;
}
NNSG2dFontGlyphBlock;

typedef struct NNSG2dFontWidthBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dFontWidth             blockBody;
}
NNSG2dFontWidthBlock;

typedef struct NNSG2dFontCodeMapBlock
{
    NNSG2dBinaryBlockHeader     blockHeader;
    NNSG2dFontCodeMap           blockBody;
}
NNSG2dFontCodeMapBlock;

//------------------------------------------------------------------------------


#ifdef NNS_G2D_FONT_USE_OLD_RESOURCE

#define NNS_G2D_NFTR_PREV_MAJOR_VER         (u8)1
#define NNS_G2D_NFTR_PREV_MINOR_VER         (u8)0

#define NNS_G2D_NFTR_PREV_VER   (u16)((NNS_G2D_NFTR_PREV_MAJOR_VER << 8) | NNS_G2D_NFTR_PREV_MINOR_VER)

#endif  // NNS_G2D_FONT_USE_OLD_RESOURCE




#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef _MSC_VER
    #pragma warning( default: 4200 )
    #pragma warning( default: 4201 )
#endif  //_MSC_VER

#endif // NNS_G2D_FONT_DATA_H_
