/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib - include - g2d_textdemolib
  File:     txt.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/
#ifndef TEXT_H_
#define TEXT_H_

#include <nnsys/g2d/g2d_Font.h>

#ifdef __cplusplus
extern "C" {
#endif

// DrawText �ł̍����
#define TXT_DRAWTEXT_FLAG_DEFAULT   (NNS_G2D_VERTICALORIGIN_TOP | NNS_G2D_HORIZONTALORIGIN_LEFT | NNS_G2D_HORIZONTALALIGN_LEFT)

// �f���p�t�H���g���\�[�X��
#define TXT_FONTRESOURCE_NAME               "/data/fonts.NFTR"
#define TXT_SJIS_FONTRESOURCE_NAME          "/data/fonts.NFTR"
#define TXT_UTF8_FONTRESOURCE_NAME          "/data/fontu8.NFTR"
#define TXT_UTF16_FONTRESOURCE_NAME         "/data/fontu16.NFTR"
#define TXT_CP1252_FONTRESOURCE_NAME        "/data/font1252.NFTR"



// TXTColorPalette �̐F�� 16�F�p���b�g�ւ̃��[�h��z��
enum
{
    // �p���b�g0 TXT_CPALETTE_MAIN
    TXT_COLOR_NULL=0,
    TXT_COLOR_WHITE,
    TXT_COLOR_BLACK,
    TXT_COLOR_RED,
    TXT_COLOR_GREEN,
    TXT_COLOR_BLUE,
    TXT_COLOR_CYAN,
    TXT_COLOR_MAGENTA,
    TXT_COLOR_YELLOW,

    // �p���b�g1 TXT_CPALETTE_USERCOLOR
    TXT_UCOLOR_NULL=0,
    TXT_UCOLOR_GRAY,
    TXT_UCOLOR_BROWN,
    TXT_UCOLOR_RED,
    TXT_UCOLOR_PINK,
    TXT_UCOLOR_ORANGE,
    TXT_UCOLOR_YELLOW,
    TXT_UCOLOR_LIMEGREEN,
    TXT_UCOLOR_DARKGREEN,
    TXT_UCOLOR_SEAGREEN,
    TXT_UCOLOR_TURQUOISE,
    TXT_UCOLOR_BLUE,
    TXT_UCOLOR_DARKBLUE,
    TXT_UCOLOR_PURPLE,
    TXT_UCOLOR_VIOLET,
    TXT_UCOLOR_MAGENTA,

    // �p���b�g TXT_CPALETTE_4BPP
    TXT_COLOR_4BPP_NULL=0,
    TXT_COLOR_4BPP_BG=1,
    TXT_COLOR_4BPP_TEXT=1
};

// TXTColorPalette �̃p���b�g�� 16�F�p���b�g�ւ̃��[�h��z��
enum
{
    TXT_CPALETTE_MAIN,
    TXT_CPALETTE_USERCOLOR,
    TXT_CPALETTE_4BPP,
    TXT_NUM_CPALEETE
};

// ���ʃJ���[�p���b�g�f�[�^
extern GXRgb TXTColorPalette[TXT_NUM_CPALEETE * 16];



//****************************************************************************
//
//****************************************************************************

/*---------------------------------------------------------------------------*
  Name:         TXT_Init

  Description:  �T���v�����ʂ̏������B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_Init(void);



/*---------------------------------------------------------------------------*
  Name:         TXT_SetupBackground

  Description:  ���C�����BG0�ɔw�i�摜�����[�h���\�����܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_SetupBackground( void );



/*---------------------------------------------------------------------------*
  Name:         TXT_Alloc

  Description:  ���������m�ۂ��܂��B

  Arguments:    size:   �m�ۂ��郁�����̃T�C�Y

  Returns:      �m�ۂ����������̈�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
void* TXT_Alloc(u32 size);



/*---------------------------------------------------------------------------*
  Name:         TXT_Free

  Description:  TXT_Alloc() �Ŋm�ۂ�����������������܂��B

  Arguments:    ptr:    �J�����郁�����̈�ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_Free(void* ptr);



/*---------------------------------------------------------------------------*
  Name:         TXT_SetCharCanvasOBJAttrs

  Description:  �A������OAM��NNS_G2dArrangeOBJ* �Őݒ肳��Ȃ��p�����[�^��
                �܂Ƃ߂Đݒ肵�܂��B

  Arguments:    oam:        �Ώۂ� OAM ��̐擪
                num:        �Ώۂ� OAM �̌�
                priority:   �\���D�揇��(0�`3)
                mode:       OBJ���[�h
                mosaic:     ���U�C�N�̗L��
                effect:     �G�t�F�N�g�̎��
                cParam:     �J���[�p�����[�^
                rsParam:    �A�t�B���ϊ��p�����[�^�C���f�b�N�X

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void TXT_SetCharCanvasOBJAttrs(
    GXOamAttr * oam,
    int         num,
    int         priority,
    GXOamMode   mode,
    BOOL        mosaic,
    GXOamEffect effect,
    int         cParam,
    int         rsParam
);



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFont

  Description:  �t�H���g���t�@�C�����烁�����Ƀ��[�h���܂��B

  Arguments:    pFname: �t�H���g���\�[�X�̃p�X�B

  Returns:      ���[�h�����t�H���g�ւ̃|�C���^�B
 *---------------------------------------------------------------------------*/
void TXT_LoadFont( NNSG2dFont* pFont, const char* pFname );



/*---------------------------------------------------------------------------*
  Name:         TXT_LoadFile

  Description:  �t�@�C�����������Ƀ��[�h���܂��B

  Arguments:    ppFile: �t�@�C�������[�h�����������A�h���X���󂯎��
                        �o�b�t�@�ւ̃|�C���^�B
                fpath:  ���[�h����t�@�C���̃p�X

  Returns:      ���[�h�����t�@�C���̃t�@�C���T�C�Y��Ԃ��܂��B
                0 �̏ꍇ�̓t�@�C�����[�h�Ɏ��s��������\���܂��B
                ���̏ꍇ *ppFile �̒l�͖����ł��B
 *---------------------------------------------------------------------------*/
u32 TXT_LoadFile(void** ppFile, const char* fpath);





#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // TEXT_H_

