/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - include
  File:     prc/algo_common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: algo_common.h,v $
  Revision 1.12  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.11  2005/03/04 12:45:07  seiki_masashi
  PRC_InitPrototypeDB �y�� PRC_InitInputPattern ���쐬�̐����E���s��Ԃ��悤�ɕύX

  Revision 1.10  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.9  2004/11/10 01:00:25  seiki_masashi
  normalizeSize ���@regularizeSize �ł��A�N�Z�X�ł���悤 union ��ǉ�

  Revision 1.8  2004/11/04 07:59:53  seiki_masashi
  regularize �� normalize �ɏC��

  Revision 1.7  2004/07/19 06:52:06  seiki_masashi
  ���ʎq���̐���

  Revision 1.6  2004/07/01 09:05:31  seiki_masashi
  ���ׂȏC��

  Revision 1.5  2004/06/29 02:13:28  seiki_masashi
  remove lineSegmentRatioToWholeArray

  Revision 1.4  2004/06/28 13:18:22  seiki_masashi
  PRC_GetInputPatternStrokes �̒ǉ�

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

  Revision 1.3  2004/06/22 11:19:00  seiki_masashi
  ����\�o�[�W����(simple�̂�)

  Revision 1.2  2004/06/17 08:14:52  seiki_masashi
  PRC_GetPrototypeDBBufferSizeEx

  Revision 1.1  2004/06/17 02:30:29  seiki_masashi
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

#ifndef NITRO_PRC_ALGO_COMMON_H_
#define NITRO_PRC_ALGO_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/prc/types.h>
#include <nitro/prc/common.h>


/*===========================================================================*
  Macros
 *===========================================================================*/

//--- Get*BufferSize �p
#define PRCi_ARRAY_SIZE(type, size, dummy) \
    sizeof(type) * (size)

//--- Set*BufferInfo �p
#define PRCi_ALLOC_ARRAY(dst, type, size, base, offset) \
    (dst) = (type*)((u8*)(base) + (offset)); \
    (offset) += sizeof(type) * (size)


/*===========================================================================*
  Constant Definitions
 *===========================================================================*/

#define PRC_RESAMPLE_DEFAULT_METHOD_COMMON PRC_RESAMPLE_METHOD_RECURSIVE
#define PRC_RESAMPLE_DEFAULT_THRESHOLD_COMMON 5

/*===========================================================================*
  Type Definitions
 *===========================================================================*/

/*---------------------------------------------------------------------------*
  ���{DB�̓W�J�Ɋւ���p�����[�^
 *---------------------------------------------------------------------------*/
typedef struct PRCPrototypeDBParam_Common
{
    int     dummy;
}
PRCPrototypeDBParam_Common;

/*---------------------------------------------------------------------------*
  ���̓p�^�[���̉��߂Ɋւ���p�����[�^
    normalizeSize       ���K���T�C�Y.
    resampleMethod      ���K���ɗp������A���S���Y���̎��.
    resampleThreshold   ���K����ɍs���郊�T���v����臒l.
 *---------------------------------------------------------------------------*/
typedef struct PRCInputPatternParam_Common
{
    union
    {
        int     normalizeSize;
        int     regularizeSize;        // ����
    };
    PRCResampleMethod resampleMethod;
    int     resampleThreshold;
}
PRCInputPatternParam_Common;

/*---------------------------------------------------------------------------*
  ���{DB�R������ѓ��͗R���̔F���p�^�[���̋��ʕ�
 *---------------------------------------------------------------------------*/
typedef struct PRCiPatternData_Common
{
    u16     strokeCount;               // entry �Əd�����
    u16     pointCount;                // entry �Əd�����
    const PRCPoint *pointArray;
    int    *strokes;
    int    *strokeSizes;
    fx32   *lineSegmentLengthArray;
    fx16   *lineSegmentRatioToStrokeArray;
//    fx16            *lineSegmentRatioToWholeArray;
    u16    *lineSegmentAngleArray;
    fx32   *strokeLengths;
    fx16   *strokeRatios;
    fx32    wholeLength;
    PRCBoundingBox *strokeBoundingBoxes;
    PRCBoundingBox wholeBoundingBox;
}
PRCiPatternData_Common;

/*---------------------------------------------------------------------------*
  ���͎��ɓ��I�Ƀp�^�[�����\�����邽�߂� Buffer �̏��
  �e�F���A���S���Y���͂�������Ɋg�����Ă���\��������̂ŁA
  �ύX�̍ۂɂ͊e�A���S���Y���� �`BufferInfo_* ��v�m�F
 *---------------------------------------------------------------------------*/
typedef struct PRCiPatternBufferInfo_Common
{
    PRCPoint *pointArray;
    int    *strokes;
    int    *strokeSizes;
    fx32   *lineSegmentLengthArray;
    fx16   *lineSegmentRatioToStrokeArray;
//    fx16            *lineSegmentRatioToWholeArray;
    u16    *lineSegmentAngleArray;
    fx32   *strokeLengths;
    fx16   *strokeRatios;
    PRCBoundingBox *strokeBoundingBoxes;
}
PRCiPatternBufferInfo_Common;

/*---------------------------------------------------------------------------*
  ���{DB�R���̔F���p�^�[��
 *---------------------------------------------------------------------------*/
typedef struct PRCiPrototypeEntry_Common
{
    PRCiPatternData_Common data;
    const PRCPrototypeEntry *entry;
}
PRCiPrototypeEntry_Common;

/*---------------------------------------------------------------------------*
  ���͗R���̔F���p�^�[��
 *---------------------------------------------------------------------------*/
typedef struct PRCInputPattern_Common
{
    PRCiPatternData_Common data;
    union
    {
        int     normalizeSize;
        int     regularizeSize;        // ����
    };

    void   *buffer;
    u32     bufferSize;
}
PRCInputPattern_Common;

/*---------------------------------------------------------------------------*
  ���͎��ɓ��I�Ɍ��{DB���\�����邽�߂� Buffer �̏��
  �e�F���A���S���Y���͂�������Ɋg�����Ă���\��������̂ŁA
  �ύX�̍ۂɂ͊e�A���S���Y���� �`BufferInfo_* ��v�m�F
 *---------------------------------------------------------------------------*/
typedef struct PRCiPrototypeListBufferInfo_Common
{
    PRCiPrototypeEntry_Common *patterns;
    fx32   *lineSegmentLengthArray;
    fx16   *lineSegmentRatioToStrokeArray;
//    fx16            *lineSegmentRatioToWholeArray;
    u16    *lineSegmentAngleArray;
    int    *strokeArray;
    int    *strokeSizeArray;
    fx32   *strokeLengthArray;
    fx16   *strokeRatioArray;
    PRCBoundingBox *strokeBoundingBoxArray;
}
PRCiPrototypeListBufferInfo_Common;

/*---------------------------------------------------------------------------*
  �������ɓW�J���ꂽ���{DB
 *---------------------------------------------------------------------------*/
typedef struct PRCPrototypeDB_Common
{
    PRCiPrototypeEntry_Common *patterns;
    int     patternCount;

    fx32   *lineSegmentLengthArray;
    fx16   *lineSegmentRatioToStrokeArray;
//    fx16                        *lineSegmentRatioToWholeArray;
    u16    *lineSegmentAngleArray;
    int     wholePointCount;

    int    *strokeArray;
    int    *strokeSizeArray;
    fx32   *strokeLengthArray;
    fx16   *strokeRatioArray;
    PRCBoundingBox *strokeBoundingBoxArray;
    int     wholeStrokeCount;

    int     maxPointCount;
    int     maxStrokeCount;

    union
    {
        int     normalizeSize;
        int     regularizeSize;        // ����
    };

    const PRCPrototypeList *prototypeList;
    void   *buffer;
    u32     bufferSize;
}
PRCPrototypeDB_Common;


/*---------------------------------------------------------------------------*
  Name:         PRC_Init_Common

  Description:  �}�`�F�� API �����������܂�.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void PRC_Init_Common(void)
{
    PRCi_Init();
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetPrototypeDBBufferSizeEx_Common

  Description:  ���{DB��W�J����̂ɕK�v�ȃ��[�N�̈�̑傫�����v�Z���܂�.

  Arguments:    prototypeList           ���{�p�^�[���̈ꗗ
                kindMask                ��ގw��p�}�X�N
                ignoreDisabledEntries   enabled �t���O�� FALSE �̌��{DB�G���g����
                                        ���������W�J���Ȃ����ǂ���
                param                   ���{DB�̓W�J�Ɋւ���p�����[�^

  Returns:      ���{DB��W�J����̂ɕK�v�ȃ�������.
 *---------------------------------------------------------------------------*/
u32
 
 
PRC_GetPrototypeDBBufferSizeEx_Common(const PRCPrototypeList *prototypeList,
                                      u32 kindMask,
                                      BOOL ignoreDisabledEntries,
                                      const PRCPrototypeDBParam_Common *param);

/*---------------------------------------------------------------------------*
  Name:         PRC_GetPrototypeDBBufferSize_Common

  Description:  ���{DB��W�J����̂ɕK�v�ȃ��[�N�̈�̑傫�����v�Z���܂�.

  Arguments:    prototypeList           ���{�p�^�[���̈ꗗ

  Returns:      ���{DB��W�J����̂ɕK�v�ȃ�������.
 *---------------------------------------------------------------------------*/
u32 static inline PRC_GetPrototypeDBBufferSize_Common(const PRCPrototypeList *prototypeList)
{
    return PRC_GetPrototypeDBBufferSizeEx_Common(prototypeList, PRC_KIND_ALL, FALSE, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_InitPrototypeDBEx_Common

  Description:  PRCPrototypeDB �\���̂��쐬���܂�.
                buffer �ɂ� PRC_GetPrototypeDBBufferSize ���Ԃ��T�C�Y�ȏ��
                �o�b�t�@�̈悪�ݒ肳��Ă���K�v������܂�.
                param �ɂ���āA�W�J���̃p�����[�^���w��ł��܂�.

  Arguments:    protoDB         ���������錩�{DB�\����.
                buffer          ���{DB�̓W�J�Ɏg�p���郁�����̈�ւ̃|�C���^.
                                (�������̈�T�C�Y>=PRC_GetPrototypeDBBufferSize �̕Ԃ�l)
                prototypeList   ���{�p�^�[���̈ꗗ
                kindMask        ��ގw��p�}�X�N
                ignoreDisabledEntries   enabled �t���O�� FALSE �̌��{DB�G���g����
                                        ���������W�J���Ȃ����ǂ���
                param           ���{DB�̓W�J�Ɋւ���p�����[�^

  Returns:      �쐬�ɐ���������^.
 *---------------------------------------------------------------------------*/
BOOL
 
 PRC_InitPrototypeDBEx_Common(PRCPrototypeDB_Common *protoDB,
                              void *buffer,
                              const PRCPrototypeList *prototypeList,
                              u32 kindMask,
                              BOOL ignoreDisabledEntries, const PRCPrototypeDBParam_Common *param);

/*---------------------------------------------------------------------------*
  Name:         PRC_InitPrototypeDB_Common

  Description:  PRCPrototypeDB �\���̂��쐬���܂�.
                buffer �ɂ� PRC_GetPrototypeDBBufferSize ���Ԃ��T�C�Y�ȏ��
                �o�b�t�@�̈悪�ݒ肳��Ă���K�v������܂�.

  Arguments:    protoDB         ���������錩�{DB�\����.
                buffer          ���{DB�̓W�J�Ɏg�p���郁�����̈�ւ̃|�C���^.
                                (�������̈�T�C�Y>=PRC_GetPrototypeDBBufferSize �̕Ԃ�l)
                prototypeList   ���{�p�^�[���̈ꗗ

  Returns:      �쐬�ɐ���������^.
 *---------------------------------------------------------------------------*/
static inline BOOL
PRC_InitPrototypeDB_Common(PRCPrototypeDB_Common *protoDB,
                           void *buffer, const PRCPrototypeList *prototypeList)
{
    return PRC_InitPrototypeDBEx_Common(protoDB, buffer, prototypeList, PRC_KIND_ALL, FALSE, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetInputPatternBufferSize_Common

  Description:  �p�^�[���f�[�^���r�p�ɓW�J����̂ɕK�v�ȃ��[�N�̈�̑傫����
                �v�Z���܂�.

  Arguments:    maxPointCount   ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount  �搔�̏��

  Returns:      �p�^�[����W�J����̂ɕK�v�ȃ�������.
 *---------------------------------------------------------------------------*/
u32     PRC_GetInputPatternBufferSize_Common(int maxPointCount, int maxStrokeCount);

/*---------------------------------------------------------------------------*
  Name:         PRC_InitInputPatternEx_Common

  Description:  PRCInputPattern �\���̂��쐬���܂�.
                param �ɂ���āA���̓p�^�[���̉��߂Ɋւ���p�����[�^���w��ł��܂�.

  Arguments:    pattern             ����������p�^�[���\����.
                buffer              �p�^�[���̓W�J�Ɏg�p���郁�����̈�ւ̃|�C���^.
                                    (�̈�T�C�Y>=PRC_GetInputPatternBufferSize �̕Ԃ�l)
                strokes             ���`�O�̐����͍��W�l.
                maxPointCount       ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount      �搔�̏��
                param               ���̓p�^�[���̉��߂Ɋւ���p�����[�^

  Returns:      �쐬�ɐ���������^.
 *---------------------------------------------------------------------------*/
BOOL
 
 PRC_InitInputPatternEx_Common(PRCInputPattern_Common *pattern,
                               void *buffer,
                               const PRCStrokes *strokes,
                               int maxPointCount,
                               int maxStrokeCount, const PRCInputPatternParam_Common *param);

/*---------------------------------------------------------------------------*
  Name:         PRC_InitInputPattern_Common

  Description:  PRCInputPattern �\���̂��쐬���܂�.

  Arguments:    pattern             ����������p�^�[���\����.
                buffer              �p�^�[���̓W�J�Ɏg�p���郁�����̈�ւ̃|�C���^.
                                    (�̈�T�C�Y>=PRC_GetInputPatternBufferSize �̕Ԃ�l)
                strokes             ���`�O�̐����͍��W�l.
                maxPointCount       ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount      �搔�̏��

  Returns:      �쐬�ɐ���������^.
 *---------------------------------------------------------------------------*/
static inline BOOL
PRC_InitInputPattern_Common(PRCInputPattern_Common *pattern,
                            void *buffer,
                            const PRCStrokes *strokes, int maxPointCount, int maxStrokeCount)
{
    return PRC_InitInputPatternEx_Common(pattern, buffer, strokes, maxPointCount, maxStrokeCount,
                                         NULL);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetInputPatternStrokes_Common

  Description:  PRCInputPattern �\���̂���_��f�[�^�𓾂܂�.

  Arguments:    strokes         ����ꂽ�_��f�[�^.
                                ���������Ă͂����܂���.
                input           ���̓p�^�[��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    PRC_GetInputPatternStrokes_Common(PRCStrokes *strokes, const PRCInputPattern_Common *input);


// for debug
void    PRCi_PrintPatternData_Common(PRCiPatternData_Common *data);

/*===========================================================================*
  Inline Functions
 *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_PRC_ALGO_COMMON_H_ */
#endif
