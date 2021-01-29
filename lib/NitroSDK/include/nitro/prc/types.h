/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - include
  File:     prc/types.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: types.h,v $
  Revision 1.10  2007/08/24 04:35:04  okubata_ryoma
  fix about headerfiles.

  Revision 1.9  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.8  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.7  2004/11/10 01:00:25  seiki_masashi
  normalizeSize ���@regularizeSize �ł��A�N�Z�X�ł���悤 union ��ǉ�

  Revision 1.6  2004/11/04 07:59:53  seiki_masashi
  regularize �� normalize �ɏC��

  Revision 1.5  2004/07/19 06:52:06  seiki_masashi
  ���ʎq���̐���

  Revision 1.4  2004/07/01 09:05:31  seiki_masashi
  ���ׂȏC��

  Revision 1.3  2004/06/28 11:10:41  seiki_masashi
  ���{�p�^�[���̈ꗗ�� regularizeSize ��ǉ�
  �e��p�����[�^�ނ𐮗�

  Revision 1.2  2004/06/25 03:03:39  seiki_masashi
  �e�֐����� bufferSize �̎w����폜
  GetRecognitionBufferSize* �̒ǉ�
  ���̊֐��Ăяo���������Ă��Ȃ��֐��� inline ��

  Revision 1.1  2004/06/24 07:03:10  seiki_masashi
  �w�b�_�t�@�C���𕪊��E�������܂���

  Revision 1.1  2004/06/23 12:00:29  seiki_masashi
  add 'PRC*' to SDK

  Revision 1.5  2004/06/22 11:19:00  seiki_masashi
  ����\�o�[�W����(simple�̂�)

  Revision 1.4  2004/06/17 08:14:52  seiki_masashi
  PRC_GetPrototypeDBBufferSizeEx

  Revision 1.3  2004/06/17 02:30:29  seiki_masashi
  �F���A���S���Y���̍����ւ���e�Ղɂ��邽�߂� prc_algo_* �ɕ������܂���

  Revision 1.2  2004/06/17 00:52:36  seiki_masashi
  PRC_InitPrototypeDBEX, PRC_InitInputPatternEX

  Revision 1.1  2004/06/16 23:56:09  seiki_masashi
  rename ptrcg to prc.

  Revision 1.3  2004/06/15 08:10:55  seiki_masashi
  �_�~�[�̃��C�u�����ŃT���v���v���O����������\��

  Revision 1.2  2004/06/15 05:55:09  seiki_masashi
  �C���^�[�t�F�C�X�̉��P

  Revision 1.1  2004/06/15 00:54:42  seiki_masashi
  �C���^�[�t�F�C�X�̐݌v


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_PRC_TYPES_H_
#define NITRO_PRC_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/fx/fx.h>

/*===========================================================================*
  Constant Definitions
 *===========================================================================*/

//--- �y�����オ�������Ƃ�������̒l
#define PRC_PEN_UP_MARKER_X (-1)
#define PRC_PEN_UP_MARKER_Y (-1)

//--- ���F���E�F���s�\��\���R�[�h
#define PRC_CODE_UNKNOWN (-1)

//--- ���ׂĂ̎�ނ�����킷 kindMask �l
#define PRC_KIND_ALL      (0xffffffffU)

//--- �Ԍ��p�̏\���ɑ傫��/���������W�l
#define PRC_LARGE_ENOUGH_X (512)
#define PRC_SMALL_ENOUGH_X (-512)
#define PRC_LARGE_ENOUGH_Y (512)
#define PRC_SMALL_ENOUGH_Y (-512)

//--- fx32 �l�ŏ����Ȓl(0���Z����)
#define PRC_TINY_LENGTH (1)

/*===========================================================================*
  Type Definitions
 *===========================================================================*/

/*---------------------------------------------------------------------------*
  2�������W
 *---------------------------------------------------------------------------*/
typedef struct PRCPoint
{
    s16     x;
    s16     y;
}
PRCPoint;

/*---------------------------------------------------------------------------*
  bounding box
 *---------------------------------------------------------------------------*/
typedef struct PRCBoundingBox
{
    s16     x1, y1;                    // �o�E���f�B���O�{�b�N�X�̍���̍��W
    s16     x2, y2;                    // �o�E���f�B���O�{�b�N�X�̉E���̍��W
}
PRCBoundingBox;

/*---------------------------------------------------------------------------*
  ��̏W��
  �K���APenUpMarker �� terminate ����Ă��Ȃ��Ƃ����Ȃ�
 *---------------------------------------------------------------------------*/
typedef struct PRCStrokes
{
    PRCPoint *points;
    int     size;
    u32     capacity;
}
PRCStrokes;

/*---------------------------------------------------------------------------*
  �F���p�^�[��
 *---------------------------------------------------------------------------*/
typedef struct PRCPrototypeEntry
{
    BOOL    enabled;
    u32     kind;
    u16     code;
    fx16    correction;
    void   *data;
    int     pointIndex;
    u16     pointCount;
    u16     strokeCount;
}
PRCPrototypeEntry;

/*---------------------------------------------------------------------------*
  ���{DB�̌��f�[�^
 *---------------------------------------------------------------------------*/
typedef struct PRCPrototypeList
{
    const PRCPrototypeEntry *entries;
    int     entrySize;
    const PRCPoint *pointArray;
    int     pointArraySize;

    union
    {
        int     normalizeSize;         // ���{DB���̍��W�l�̐��K���T�C�Y
        int     regularizeSize;        // ����
    };
}
PRCPrototypeList;

/*---------------------------------------------------------------------------*
  ���T���v�����O��@
 *---------------------------------------------------------------------------*/
typedef enum PRCResampleMethod
{
    PRC_RESAMPLE_METHOD_NONE = 0,
    PRC_RESAMPLE_METHOD_DISTANCE,
    PRC_RESAMPLE_METHOD_ANGLE,
    PRC_RESAMPLE_METHOD_RECURSIVE,
    PRC_RESAMPLE_METHOD_USER = 256
}
PRCResampleMethod;


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_PRC_TYPES_H_ */
#endif
