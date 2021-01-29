/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - include
  File:     prc/algo_light.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: algo_light.h,v $
  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/03/04 12:45:07  seiki_masashi
  PRC_InitPrototypeDB �y�� PRC_InitInputPattern ���쐬�̐����E���s��Ԃ��悤�ɕύX

  Revision 1.4  2005/02/28 05:26:01  yosizaki
  do-indent.

  Revision 1.3  2004/11/04 07:59:53  seiki_masashi
  regularize �� normalize �ɏC��

  Revision 1.2  2004/07/19 06:52:06  seiki_masashi
  ���ʎq���̐���

  Revision 1.1  2004/06/30 08:49:45  seiki_masashi
  algo_simple -> algo_light
  algo_standard -> algo_fine
  algo_accurate -> algo_superfine
  �V���� algo_standard �̒ǉ�

  Revision 1.3  2004/06/28 13:18:22  seiki_masashi
  PRC_GetInputPatternStrokes �̒ǉ�

  Revision 1.2  2004/06/25 03:03:39  seiki_masashi
  �e�֐����� bufferSize �̎w����폜
  GetRecognitionBufferSize* �̒ǉ�
  ���̊֐��Ăяo���������Ă��Ȃ��֐��� inline ��

  Revision 1.1  2004/06/24 07:03:10  seiki_masashi
  �w�b�_�t�@�C���𕪊��E�������܂���

  Revision 1.1  2004/06/23 12:00:29  seiki_masashi
  add 'PRC*' to SDK

  Revision 1.3  2004/06/22 11:19:00  seiki_masashi
  ����\�o�[�W����(light�̂�)

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
#ifndef NITRO_PRC_ALGO_LIGHT_H_
#define NITRO_PRC_ALGO_LIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/prc/types.h>
#include <nitro/prc/common.h>
#include <nitro/prc/resample.h>
#include <nitro/prc/algo_common.h>

/*===========================================================================*
  Constant Definitions
 *===========================================================================*/

/*===========================================================================*
  Type Definitions
 *===========================================================================*/

/*---------------------------------------------------------------------------*
  ���{DB�̓W�J�Ɋւ���p�����[�^
    normalizeSize       ���K���T�C�Y.
    resampleThreshold   ���K����ɍs���郊�T���v����臒l.
 *---------------------------------------------------------------------------*/
typedef PRCPrototypeDBParam_Common PRCPrototypeDBParam_Light;

/*---------------------------------------------------------------------------*
  ���̓p�^�[���̉��߂Ɋւ���p�����[�^
    normalizeSize       ���K���T�C�Y.
    resampleThreshold   ���K����ɍs���郊�T���v����臒l.
 *---------------------------------------------------------------------------*/
typedef PRCInputPatternParam_Common PRCInputPatternParam_Light;

/*---------------------------------------------------------------------------*
  ���͗R���̔F���p�^�[��
 *---------------------------------------------------------------------------*/
typedef PRCInputPattern_Common PRCInputPattern_Light;

/*---------------------------------------------------------------------------*
  �������ɓW�J���ꂽ���{DB
 *---------------------------------------------------------------------------*/
typedef PRCPrototypeDB_Common PRCPrototypeDB_Light;

/*---------------------------------------------------------------------------*
  �F�������Ɋւ���p�����[�^
 *---------------------------------------------------------------------------*/
typedef struct PRCRecognizeParam_Light
{
    int     dummy;
}
PRCRecognizeParam_Light;



/*---------------------------------------------------------------------------*
  Name:         PRC_Init_Light

  Description:  �}�`�F�� API �����������܂�.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void PRC_Init_Light(void)
{
    PRC_Init_Common();
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetPrototypeDBBufferSize_Light

  Description:  ���{DB��W�J����̂ɕK�v�ȃ��[�N�̈�̑傫�����v�Z���܂�.

  Arguments:    prototypeList           ���{�p�^�[���̈ꗗ

  Returns:      ���{DB��W�J����̂ɕK�v�ȃ�������.
 *---------------------------------------------------------------------------*/
static inline u32 PRC_GetPrototypeDBBufferSize_Light(const PRCPrototypeList *prototypeList)
{
    return PRC_GetPrototypeDBBufferSize_Common(prototypeList);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetPrototypeDBBufferSizeEx_Light

  Description:  ���{DB��W�J����̂ɕK�v�ȃ��[�N�̈�̑傫�����v�Z���܂�.

  Arguments:    prototypeList           ���{�p�^�[���̈ꗗ
                kindMask                ��ގw��p�}�X�N
                ignoreDisabledEntries   enabled �t���O�� FALSE �̌��{DB�G���g����
                                        ���������W�J���Ȃ����ǂ���
                param                   ���{DB�̓W�J�Ɋւ���p�����[�^

  Returns:      ���{DB��W�J����̂ɕK�v�ȃ�������.
 *---------------------------------------------------------------------------*/
static inline u32
PRC_GetPrototypeDBBufferSizeEx_Light(const PRCPrototypeList *prototypeList,
                                     u32 kindMask,
                                     BOOL ignoreDisabledEntries,
                                     const PRCPrototypeDBParam_Light *param)
{
    return PRC_GetPrototypeDBBufferSizeEx_Common(prototypeList, kindMask, ignoreDisabledEntries,
                                                 param);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_InitPrototypeDB_Light

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
PRC_InitPrototypeDB_Light(PRCPrototypeDB_Light *protoDB,
                          void *buffer, const PRCPrototypeList *prototypeList)
{
    return PRC_InitPrototypeDB_Common(protoDB, buffer, prototypeList);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_InitPrototypeDBEx_Light

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
static inline BOOL
PRC_InitPrototypeDBEx_Light(PRCPrototypeDB_Light *protoDB,
                            void *buffer,
                            const PRCPrototypeList *prototypeList,
                            u32 kindMask,
                            BOOL ignoreDisabledEntries, const PRCPrototypeDBParam_Light *param)
{
    return PRC_InitPrototypeDBEx_Common(protoDB, buffer, prototypeList, kindMask,
                                        ignoreDisabledEntries, param);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetInputPatternBufferSize_Light

  Description:  �p�^�[���f�[�^���r�p�ɓW�J����̂ɕK�v�ȃ��[�N�̈�̑傫����
                �v�Z���܂�.

  Arguments:    maxPointCount   ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount  �搔�̏��

  Returns:      �p�^�[����W�J����̂ɕK�v�ȃ�������.
 *---------------------------------------------------------------------------*/
static inline u32 PRC_GetInputPatternBufferSize_Light(int maxPointCount, int maxStrokeCount)
{
    return PRC_GetInputPatternBufferSize_Common(maxPointCount, maxStrokeCount);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_InitInputPattern_Light

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
PRC_InitInputPattern_Light(PRCInputPattern_Light *pattern,
                           void *buffer,
                           const PRCStrokes *strokes, int maxPointCount, int maxStrokeCount)
{
    return PRC_InitInputPattern_Common(pattern, buffer, strokes, maxPointCount, maxStrokeCount);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_InitInputPatternEx_Light

  Description:  PRCInputPattern �\���̂��쐬���܂�.
                param �ɂ���āA���̓p�^�[���̉��߂Ɋւ���p�����[�^���w��ł��܂�.

  Arguments:    pattern             ����������p�^�[���\����.
                buffer              �p�^�[���̓W�J�Ɏg�p���郁�����̈�ւ̃|�C���^.
                                    (�̈�T�C�Y>=PRC_GetInputPatternBufferSize �̕Ԃ�l)
                strokes             ���`�O�̐����͍��W�l.
                param               ���̓p�^�[���̉��߂Ɋւ���p�����[�^

  Returns:      �쐬�ɐ���������^.
 *---------------------------------------------------------------------------*/
static inline BOOL
PRC_InitInputPatternEx_Light(PRCInputPattern_Light *pattern,
                             void *buffer,
                             const PRCStrokes *strokes,
                             int maxPointCount,
                             int maxStrokeCount, const PRCInputPatternParam_Light *param)
{
    return PRC_InitInputPatternEx_Common(pattern, buffer, strokes, maxPointCount, maxStrokeCount,
                                         param);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetRecognitionBufferSizeEx_Light

  Description:  �F���A���S���Y�����K�v�Ƃ��郏�[�N�̈�̑傫����
                �v�Z���܂�.

  Arguments:    maxPointCount   ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount  �搔�̏��
                protoDB         ���{DB.
                param           �F�������Ɋւ���p�����[�^.

  Returns:      �F���A���S���Y�����K�v�Ƃ��郁������.
 *---------------------------------------------------------------------------*/
u32
 
 
PRC_GetRecognitionBufferSizeEx_Light(int maxPointCount,
                                     int maxStrokeCount,
                                     const PRCPrototypeDB_Light *protoDB,
                                     const PRCRecognizeParam_Light *param);

/*---------------------------------------------------------------------------*
  Name:         PRC_GetRecognitionBufferSize_Light

  Description:  �F���A���S���Y�����K�v�Ƃ��郏�[�N�̈�̑傫����
                �v�Z���܂�.

  Arguments:    maxPointCount   ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount  �搔�̏��
                protoDB         ���{DB.

  Returns:      �F���A���S���Y�����K�v�Ƃ��郁������.
 *---------------------------------------------------------------------------*/
static inline u32
PRC_GetRecognitionBufferSize_Light(int maxPointCount,
                                   int maxStrokeCount, const PRCPrototypeDB_Light *protoDB)
{
    return PRC_GetRecognitionBufferSizeEx_Light(maxPointCount, maxStrokeCount, protoDB, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetRecognizedEntriesEx_Light

  Description:  ���{DB�̓���� kind �̃G���g���Ɠ��̓p�^�[�����r���ĔF����,
                ���ʂ̏�� numRanking �ʂ�Ԃ��܂�.

  Arguments:    resultEntries   �F�����ʂւ̃|�C���^������z��ւ̃|�C���^.
                                ���߂���ȉ������F���ł��Ȃ������ꍇ�A
                                �c��� NULL �Ŗ��߂��܂�.
                resultScores    �F�����ʂ̃X�R�A�̔z��ւ̃|�C���^.
                numRanking      result* �ɕԂ���
                buffer          �F���A���S���Y�����g�p���郁�����̈�ւ̃|�C���^.
                                (�̈�T�C�Y>=PRC_GetRecognitionBufferSize �̕Ԃ�l)
                input           ���̓p�^�[��.
                protoDB         ���{DB.
                kindMask        �e���{DB�G���g���� kind �l�Ƃ̘_���ς��Ƃ�,
                                ��[���ł���ΗL���Ƃ݂Ȃ��܂�.
                param           �F�������Ɋւ���p�����[�^.

  Returns:      ��r���ꂽ���{DB��̃p�^�[����.
 *---------------------------------------------------------------------------*/
int
 
 
PRC_GetRecognizedEntriesEx_Light(PRCPrototypeEntry **resultEntries,
                                 fx32 *resultScores,
                                 int numRanking,
                                 void *buffer,
                                 const PRCInputPattern_Light *input,
                                 const PRCPrototypeDB_Light *protoDB,
                                 u32 kindMask, const PRCRecognizeParam_Light *param);

/*---------------------------------------------------------------------------*
  Name:         PRC_GetRecognizedEntries_Light

  Description:  ���{DB�Ɠ��̓p�^�[�����r���ĔF����,
                ���ʂ̏�� numRanking �ʂ�Ԃ��܂�.

  Arguments:    resultEntries   �F�����ʂւ̃|�C���^������z��ւ̃|�C���^.
                                ���߂���ȉ������F���ł��Ȃ������ꍇ�A
                                �c��� NULL �Ŗ��߂��܂�.
                resultScores    �F�����ʂ̃X�R�A�̔z��ւ̃|�C���^.
                numRanking      result* �ɕԂ��Ăق�����
                buffer          �F���A���S���Y�����g�p���郁�����̈�ւ̃|�C���^.
                                (�̈�T�C�Y>=PRC_GetRecognitionBufferSize �̕Ԃ�l)
                input           ���̓p�^�[��.
                protoDB         ���{DB.

  Returns:      ��r���ꂽ���{DB��̃p�^�[����.
 *---------------------------------------------------------------------------*/
static inline int
PRC_GetRecognizedEntries_Light(PRCPrototypeEntry **resultEntries,
                               fx32 *resultScores,
                               int numRanking,
                               void *buffer,
                               const PRCInputPattern_Light *input,
                               const PRCPrototypeDB_Light *protoDB)
{
    return PRC_GetRecognizedEntriesEx_Light(resultEntries, resultScores, numRanking, buffer, input,
                                            protoDB, PRC_KIND_ALL, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetRecognizedEntryEx_Light

  Description:  ���{DB�̓���� kind �̃G���g���Ɠ��̓p�^�[�����r���ĔF�����܂�.

  Arguments:    resultEntry �F�����ꂽ���{DB�G���g���ւ̃|�C���^���󂯎��|�C���^.
                            �F���ł��Ȃ������ꍇ�ANULL ������܂�.
                buffer      �F���A���S���Y�����g�p���郁�����̈�ւ̃|�C���^.
                            (�̈�T�C�Y>=PRC_GetRecognitionBufferSize �̕Ԃ�l)
                input       ���̓p�^�[��.
                protoDB     ���{DB.
                kindMask    �e���{DB�G���g���� kind �l�Ƃ̘_���ς��Ƃ�,
                            ��[���ł���ΗL���Ƃ݂Ȃ��܂�.
                param       �F�������Ɋւ���p�����[�^.

  Returns:      �X�R�A�l.
 *---------------------------------------------------------------------------*/
static inline fx32
PRC_GetRecognizedEntryEx_Light(PRCPrototypeEntry **resultEntry,
                               void *buffer,
                               const PRCInputPattern_Light *input,
                               const PRCPrototypeDB_Light *protoDB,
                               u32 kindMask, const PRCRecognizeParam_Light *param)
{
    fx32    score;
    (void)PRC_GetRecognizedEntriesEx_Light(resultEntry, &score, 1, buffer, input, protoDB, kindMask,
                                           param);
    return score;
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetRecognizedEntry_Light

  Description:  ���{DB�Ɠ��̓p�^�[�����r���ĔF�����܂�.

  Arguments:    resultEntry �F�����ꂽ���{DB�G���g���ւ̃|�C���^���󂯎��|�C���^.
                            �F���ł��Ȃ������ꍇ�ANULL ������܂�.
                buffer      �F���A���S���Y�����g�p���郁�����̈�ւ̃|�C���^.
                            (�̈�T�C�Y>=PRC_GetRecognitionBufferSize �̕Ԃ�l)
                input       ���̓p�^�[��.
                protoDB     ���{DB.

  Returns:      �X�R�A�l.
 *---------------------------------------------------------------------------*/
static inline fx32
PRC_GetRecognizedEntry_Light(PRCPrototypeEntry **resultEntry,
                             void *buffer,
                             const PRCInputPattern_Light *input,
                             const PRCPrototypeDB_Light *protoDB)
{
    return PRC_GetRecognizedEntryEx_Light(resultEntry, buffer, input, protoDB, PRC_KIND_ALL, NULL);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetInputPatternStrokes_Light

  Description:  PRCInputPattern �\���̂���_��f�[�^�𓾂܂�.

  Arguments:    strokes         ����ꂽ�_��f�[�^.
                                ���������Ă͂����܂���.
                input           ���̓p�^�[��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void
PRC_GetInputPatternStrokes_Light(PRCStrokes *strokes, const PRCInputPattern_Light *input)
{
    PRC_GetInputPatternStrokes_Common(strokes, input);
}


/*===========================================================================*
  Inline Functions
 *===========================================================================*/




/*===========================================================================*
  �}�`�F�� API �̕W���֐��Ƃ��Đݒ�
 *===========================================================================*/
#ifndef PRC_DEFAULT_ALGORITHM
#define PRC_DEFAULT_ALGORITHM "Light"

/*---------------------------------------------------------------------------*
  ���{DB�̓W�J�Ɋւ���p�����[�^
 *---------------------------------------------------------------------------*/
typedef PRCPrototypeDBParam_Light PRCPrototypeDBParam;

/*---------------------------------------------------------------------------*
  ���̓p�^�[���̉��߂Ɋւ���p�����[�^
 *---------------------------------------------------------------------------*/
typedef PRCInputPatternParam_Light PRCInputPatternParam;

/*---------------------------------------------------------------------------*
  ���͗R���̔F���p�^�[��
 *---------------------------------------------------------------------------*/
typedef PRCInputPattern_Light PRCInputPattern;

/*---------------------------------------------------------------------------*
  �������ɓW�J���ꂽ���{DB
 *---------------------------------------------------------------------------*/
typedef PRCPrototypeDB_Light PRCPrototypeDB;

/*---------------------------------------------------------------------------*
  �F�������Ɋւ���p�����[�^
 *---------------------------------------------------------------------------*/
typedef PRCRecognizeParam_Light PRCRecognizeParam;


/*---------------------------------------------------------------------------*
  �֐�
 *---------------------------------------------------------------------------*/
static inline void PRC_Init(void)
{
    PRC_Init_Light();
}

static inline u32 PRC_GetPrototypeDBBufferSize(const PRCPrototypeList *prototypeList)
{
    return PRC_GetPrototypeDBBufferSize_Light(prototypeList);
}

static inline u32
PRC_GetPrototypeDBBufferSizeEx(const PRCPrototypeList *prototypeList,
                               u32 kindMask,
                               BOOL ignoreDisabledEntries, const PRCPrototypeDBParam *param)
{
    return PRC_GetPrototypeDBBufferSizeEx_Light(prototypeList, kindMask, ignoreDisabledEntries,
                                                param);
}

static inline BOOL
PRC_InitPrototypeDB(PRCPrototypeDB *protoDB, void *buffer, const PRCPrototypeList *prototypeList)
{
    return PRC_InitPrototypeDB_Light(protoDB, buffer, prototypeList);
}

static inline BOOL
PRC_InitPrototypeDBEx(PRCPrototypeDB *protoDB,
                      void *buffer,
                      const PRCPrototypeList *prototypeList,
                      u32 kindMask, BOOL ignoreDisabledEntries, const PRCPrototypeDBParam *param)
{
    return PRC_InitPrototypeDBEx_Light(protoDB, buffer, prototypeList, kindMask,
                                       ignoreDisabledEntries, param);
}

static inline u32 PRC_GetInputPatternBufferSize(int maxPointCount, int maxStrokeCount)
{
    return PRC_GetInputPatternBufferSize_Light(maxPointCount, maxStrokeCount);
}

static inline BOOL
PRC_InitInputPattern(PRCInputPattern *pattern,
                     void *buffer, const PRCStrokes *strokes, int maxPointCount, int maxStrokeCount)
{
    return PRC_InitInputPattern_Light(pattern, buffer, strokes, maxPointCount, maxStrokeCount);
}

static inline BOOL
PRC_InitInputPatternEx(PRCInputPattern *pattern,
                       void *buffer,
                       const PRCStrokes *strokes,
                       int maxPointCount, int maxStrokeCount, const PRCInputPatternParam *param)
{
    return PRC_InitInputPatternEx_Light(pattern, buffer, strokes, maxPointCount, maxStrokeCount,
                                        param);
}

static inline u32
PRC_GetRecognitionBufferSize(int maxPointCount,
                             int maxStrokeCount, const PRCPrototypeDB_Light *protoDB)
{
    return PRC_GetRecognitionBufferSize_Light(maxPointCount, maxStrokeCount, protoDB);
}

static inline u32
PRC_GetRecognitionBufferSizeEx(int maxPointCount,
                               int maxStrokeCount,
                               const PRCPrototypeDB_Light *protoDB,
                               const PRCRecognizeParam_Light *param)
{
    return PRC_GetRecognitionBufferSizeEx_Light(maxPointCount, maxStrokeCount, protoDB, param);
}

static inline fx32
PRC_GetRecognizedEntry(PRCPrototypeEntry **resultEntry,
                       void *buffer, const PRCInputPattern *input, const PRCPrototypeDB *protoDB)
{
    return PRC_GetRecognizedEntry_Light(resultEntry, buffer, input, protoDB);
}

static inline fx32
PRC_GetRecognizedEntryEx(PRCPrototypeEntry **resultEntry,
                         void *buffer,
                         const PRCInputPattern *input,
                         const PRCPrototypeDB *protoDB,
                         u32 kindMask, const PRCRecognizeParam *param)
{
    return PRC_GetRecognizedEntryEx_Light(resultEntry, buffer, input, protoDB, kindMask, param);
}

static inline int
PRC_GetRecognizedEntries(PRCPrototypeEntry **resultEntries,
                         fx32 *resultScores,
                         int numRanking,
                         void *buffer, const PRCInputPattern *input, const PRCPrototypeDB *protoDB)
{
    return PRC_GetRecognizedEntries_Light(resultEntries, resultScores, numRanking, buffer, input,
                                          protoDB);
}

static inline int
PRC_GetRecognizedEntriesEx(PRCPrototypeEntry **resultEntries,
                           fx32 *resultScores,
                           int numRanking,
                           void *buffer,
                           const PRCInputPattern *input,
                           const PRCPrototypeDB *protoDB,
                           u32 kindMask, const PRCRecognizeParam *param)
{
    return PRC_GetRecognizedEntriesEx_Light(resultEntries, resultScores, numRanking, buffer, input,
                                            protoDB, kindMask, param);
}

static inline void PRC_GetInputPatternStrokes(PRCStrokes *strokes, const PRCInputPattern *input)
{
    PRC_GetInputPatternStrokes_Light(strokes, input);
}

#endif // PRC_DEFAULT_ALGORITHM




#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_PRC_ALGO_LIGHT_H_ */
#endif
