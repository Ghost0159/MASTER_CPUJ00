/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - 
  File:     prc_algo_light.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: prc_algo_light.c,v $
  Revision 1.6  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.5  2005/02/28 05:26:29  yosizaki
  do-indent.

  Revision 1.4  2004/07/19 06:52:06  seiki_masashi
  ���ʎq���̐���

  Revision 1.3  2004/07/05 02:37:54  seiki_masashi
  prevRatio �̏���

  Revision 1.2  2004/06/30 12:53:44  seiki_masashi
  �L�Q�ɂȂ肤�� ASSERT �̍폜

  Revision 1.1  2004/06/30 08:49:24  seiki_masashi
  algo_simple -> algo_light
  algo_standard -> algo_fine
  algo_accurate -> algo_superfine
  �V���� algo_standard �̒ǉ�

  Revision 1.5  2004/06/29 07:25:02  seiki_masashi
  �o�O�C��
  �}�؂�̎���

  Revision 1.4  2004/06/25 03:02:55  seiki_masashi
  �e�֐����� bufferSize �̎w����폜
  GetRecognitionBufferSize* �̒ǉ�

  Revision 1.3  2004/06/24 08:08:33  seiki_masashi
  fix typo

  Revision 1.2  2004/06/24 07:03:39  seiki_masashi
  �w�b�_�t�@�C���𕪊��E�������܂���

  Revision 1.1  2004/06/23 11:59:30  seiki_masashi
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


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/prc/algo_light.h>

/*===========================================================================*
  Prototype Declarations
 *===========================================================================*/

/*===========================================================================*
  Variable Definitions
 *===========================================================================*/

/*===========================================================================*
  Functions
 *===========================================================================*/

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
                                     const PRCRecognizeParam_Light *param)
{
    (void)maxPointCount;
    (void)maxStrokeCount;
    (void)protoDB;
    (void)param;

    return 1;                          // OS_Alloc(0) �̓G���[�ƂȂ邽��
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
                                 u32 kindMask, const PRCRecognizeParam_Light *param)
{
    int     i;
    const PRCiPatternData_Common *inputData;
    int     numCompared;

    (void)buffer;
    (void)param;

    SDK_ASSERT(resultEntries);
    SDK_ASSERT(resultScores);
    SDK_ASSERT(input);
    SDK_ASSERT(protoDB);
    SDK_ASSERT(numRanking > 0);

    for (i = 0; i < numRanking; i++)
    {
        resultEntries[i] = NULL;
        resultScores[i] = FX32_ONE * 32768;
    }

    inputData = &input->data;
    numCompared = 0;

    {
        const PRCiPrototypeEntry_Common *proto;
        int     iPattern;
        fx32    border;

        border = FX32_ONE;             // ���؂胉�C�� ���̒l�� FX32_ONE/2 �Ȃǂ���ƍŏ����� 0.5 �ȉ��̃X�R�A�����肳��Ȃ�
        border *= 32768;               // ���� score �̌W��

        proto = protoDB->patterns;

        for (iPattern = 0; iPattern < protoDB->patternCount; iPattern++, proto++)
        {
            const PRCiPatternData_Common *protoData;
            int     iStroke;
            fx32    wholeScore;
            fx32    patternBorder;

            if (!proto->entry->enabled || !(proto->entry->kind & kindMask))
                continue;

            protoData = &proto->data;

            if (inputData->strokeCount != protoData->strokeCount)
                continue;

            wholeScore = 0;

            // �␳�̕���߂����l�𑫐؂�ݒ�
            patternBorder = border;
            if (proto->entry->correction != 0)
            {
                // patternBorder *= 1 / (FX32_ONE - correction) ��{���͂��Ȃ��Ƃ����Ȃ��Ƃ����
                // �K���ɒ����ŋߎ����Ă��傫���l�ł͂����
                if (proto->entry->correction < -FX32_ONE)
                {
                    patternBorder >>= 1;        // 1/2 �ȉ��ŋߎ�
                }
                else if (proto->entry->correction < 0)
                {
                    // -1 < correction < 0 �ł� correction/2+1 �ŋߎ�
                    patternBorder = FX_Mul(patternBorder, FX32_ONE + proto->entry->correction / 2);
                }
                else if (proto->entry->correction < FX32_ONE / 2)
                {
                    // 0 < correction < 1/2 �ł� correction*2+1 �ŋߎ�
                    patternBorder = FX_Mul(patternBorder, FX32_ONE + proto->entry->correction * 2);
                }
                else
                {
                    patternBorder = FX32_ONE * 32768;
                }                      // ���؂��������߂�
            }

            for (iStroke = 0; iStroke < inputData->strokeCount; iStroke++)
            {
                int     iProto, iInput;
                int     protoStrokeIndex, inputStrokeIndex;
                int     protoSize, inputSize;
                const u16 *protoAngle;
                const u16 *inputAngle;
                const fx16 *protoRatio;
                const fx16 *inputRatio;
                fx16    protoNextRatio, inputNextRatio;
                fx32    score;
                fx32    localBorder;
                fx16    strokeRatio;
                int     loopEnd;

                score = 0;

                protoStrokeIndex = protoData->strokes[iStroke];
                inputStrokeIndex = inputData->strokes[iStroke];
                protoSize = protoData->strokeSizes[iStroke];
                inputSize = inputData->strokeSizes[iStroke];
                protoAngle = &protoData->lineSegmentAngleArray[protoStrokeIndex];
                inputAngle = &inputData->lineSegmentAngleArray[inputStrokeIndex];
                protoRatio = &protoData->lineSegmentRatioToStrokeArray[protoStrokeIndex];
                inputRatio = &inputData->lineSegmentRatioToStrokeArray[inputStrokeIndex];

                strokeRatio = protoData->strokeRatios[iStroke]; // ������ inputData-> �ɂ���ƁA���͂̉�̔�d���d�������d�ݕt���ɂȂ�

                // ���؂背�x���̐ݒ�
                if (strokeRatio == FX32_ONE)
                {
                    localBorder = patternBorder;
                }
                else if (strokeRatio >= FX32_ONE / 2)
                {
                    localBorder = patternBorder * 2;    // ���߂ɂƂ镪�ɂ͖��Ȃ�
                }
                else if (strokeRatio >= FX32_ONE / 3)
                {
                    localBorder = patternBorder * 3;    // ���߂ɂƂ镪�ɂ͖��Ȃ�
                }
                else
                {
                    localBorder = FX32_ONE * 32768;     // ���؂���l���Ȃ�
                }

                SDK_ASSERT(protoSize >= 2);
                SDK_ASSERT(inputSize >= 2);

                iProto = iInput = 1;
                protoNextRatio = protoRatio[iProto];
                inputNextRatio = inputRatio[iInput];
                loopEnd = protoSize + inputSize - 3;
                for (i = 0; i < loopEnd; i++)
                {
                    int     diff;
                    SDK_ASSERT(iProto < protoSize);
                    SDK_ASSERT(iInput < inputSize);
                    diff = (s16)(protoAngle[iProto] - inputAngle[iInput]);
                    if (diff < 0)
                    {
                        diff = -diff;
                    }
                    if (protoNextRatio <= inputNextRatio)
                    {
                        score += protoNextRatio * diff;
                        iProto++;
                        inputNextRatio -= protoNextRatio;
                        protoNextRatio = protoRatio[iProto];
                    }
                    else
                    {
                        score += inputNextRatio * diff;
                        iInput++;
                        protoNextRatio -= inputNextRatio;
                        inputNextRatio = inputRatio[iInput];
                    }
                    // ���؂�`�F�b�N
                    if (score > localBorder)
                    {
                        // �Œ�ʌ��ɂ����������錩���݂��Ȃ��Ȃ���
                        wholeScore = FX32_ONE * 32768;
                        goto quit_compare;
                    }
                }

                wholeScore += FX_Mul(score, strokeRatio);
            }

            if (proto->entry->correction != 0)
            {
                wholeScore = FX_Mul(wholeScore, FX32_ONE - proto->entry->correction);
            }

//                wholeScore = FX_Mul(wholeScore, FX32_ONE - proto->entry->correction)
//                            + proto->entry->correction;

            // ���̒i�K�ł͏������X�R�A�̂ق����悢���Ƃɒ���
          quit_compare:
            numCompared++;
            if (resultScores[numRanking - 1] > wholeScore)
            {
                resultScores[numRanking - 1] = wholeScore;
                resultEntries[numRanking - 1] = (PRCPrototypeEntry *)proto->entry;
                for (i = numRanking - 2; i >= 0; i--)
                {
                    if (resultScores[i] > resultScores[i + 1])
                    {
                        fx32    tmpScore;
                        PRCPrototypeEntry *tmpEntry;
                        tmpScore = resultScores[i];
                        resultScores[i] = resultScores[i + 1];
                        resultScores[i + 1] = tmpScore;
                        tmpEntry = resultEntries[i];
                        resultEntries[i] = resultEntries[i + 1];
                        resultEntries[i + 1] = tmpEntry;
                    }
                }
                // ���؂�X�R�A�̐ݒ�
                border = resultScores[numRanking - 1];
            }
        }
    }
    // �X�R�A�̐��K��
    {
        int     iRank;
        fx32    score;
        for (iRank = 0; iRank < numRanking; iRank++)
        {
            if (resultEntries[iRank] == NULL)
                break;

            score = resultScores[iRank];
            score = FX32_ONE - (score / 32768);
            if (score < 0)
                score = 0;
            if (score >= FX32_ONE)
                score = FX32_ONE;

            resultScores[iRank] = score;
        }
    }

    return numCompared;
}

/*===========================================================================*
  Static Functions
 *===========================================================================*/


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
