/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - 
  File:     prc_algo_common.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: prc_resample.c,v $
  Revision 1.12  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.11  2005/03/04 14:07:53  seiki_masashi
  PRC_ResampleStrokes_* �𐬌��E���s��Ԃ��悤�ɕύX

  Revision 1.10  2005/02/28 05:26:29  yosizaki
  do-indent.

  Revision 1.9  2004/08/31 00:47:47  seiki_masashi
  fix a comment.

  Revision 1.8  2004/07/19 06:52:06  seiki_masashi
  ���ʎq���̐���

  Revision 1.7  2004/07/09 00:12:59  seiki_masashi
  PRCi_Resample* -> PRC_Resample*

  Revision 1.6  2004/06/29 07:23:30  seiki_masashi
  PRC_RESAMPLE_METHOD_ANGLE �̏C��

  Revision 1.5  2004/06/28 11:11:36  seiki_masashi
  ���{�p�^�[���̈ꗗ�� regularizeSize ��ǉ�
  �e��p�����[�^�ނ𐮗�

  Revision 1.4  2004/06/25 13:32:11  seiki_masashi
  �F���A���S���Y�� 'Standard' �̒ǉ�

  Revision 1.3  2004/06/24 08:08:33  seiki_masashi
  fix typo

  Revision 1.2  2004/06/24 07:03:39  seiki_masashi
  �w�b�_�t�@�C���𕪊��E�������܂���

  Revision 1.1  2004/06/23 11:59:30  seiki_masashi
  add 'PRC*' to SDK

  Revision 1.1  2004/06/22 11:19:00  seiki_masashi
  ����\�o�[�W����(simple�̂�)


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/prc/resample.h>

/*===========================================================================*
  Prototype Declarations
 *===========================================================================*/


/*===========================================================================*
  Variable Definitions
 *===========================================================================*/

/*===========================================================================*
  Functions
 *===========================================================================*/

static BOOL
PRCi_TerminateStrokes(u16 *selectedPoints,
                      int *pSelectedPointNum, int maxPointCount, const PRCStrokes *strokes)
{
    int     selectedPointNum = *pSelectedPointNum;
    const PRCPoint *inputPoints;

    inputPoints = strokes->points;
    if (selectedPointNum < 2)
    {
        // ������ 0 �� 1 �̓_��͖���
        *pSelectedPointNum = 0;
        return FALSE;
    }
    if (!PRC_IsPenUpMarker(&inputPoints[selectedPoints[selectedPointNum - 1]]))
    {
        // Pen Up Marker �ŏI����Ă��Ȃ�
        if (!PRC_IsPenUpMarker(&inputPoints[selectedPoints[selectedPointNum - 2]]))
        {
            // 2�O�� Pen Up Marker �ł͂Ȃ�
            if (selectedPointNum < maxPointCount)
            {
                // ���ɗ]�T������� Pen Up Marker ��t����
                selectedPoints[selectedPointNum] = (u16)-1;
                selectedPointNum++;
            }
            else
            {
                // ���ɋ󂫂��Ȃ��ꍇ
                if (selectedPointNum >= 3
                    && !PRC_IsPenUpMarker(&inputPoints[selectedPoints[selectedPointNum - 3]]))
                {
                    // 3�O�� Pen Up Marker �łȂ��Ȃ�΁A�Ō�� Pen Up �ŏ㏑��
                    selectedPoints[selectedPointNum - 1] = (u16)-1;
                }
                else
                {
                    // �����Ȃ��΁A2���
                    selectedPointNum -= 2;
                }
            }
        }
        else
        {
            // 2�O�� Pen  Up Marker �Ȃ牽���l����1�؂�l�߂�
            selectedPointNum--;
        }
    }

    *pSelectedPointNum = selectedPointNum;
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         PRC_ResampleStrokes_None

  Description:  ���T���v�����O���܂���.

  Arguments:    selectedPoints, pSelectedPointNum ���ʂ��Ԃ�|�C���^
                maxPointCount       ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount      �搔�̏��
                strokes             ���`�O�̐����͍��W�l.
                threshold           ���T���v����臒l
                buffer              ��Ɨ̈�(sizeof(int)*maxPointCount �K�v)

  Returns:      ���T���v�����O�ɐ���������^.
 *---------------------------------------------------------------------------*/
BOOL
PRC_ResampleStrokes_None(u16 *selectedPoints,
                         int *pSelectedPointNum,
                         int maxPointCount,
                         int maxStrokeCount, const PRCStrokes *strokes, int threshold, void *buffer)
{
    // ���T���v�������ɐ��̓_����g��
    u16     iPoint;
    int     size = strokes->size;

    (void)maxStrokeCount;
    (void)threshold;
    (void)buffer;

    if (size > maxPointCount)
    {
        size = maxPointCount;
    }
    if (size < 2)
    {
        // ������ 0 �� 1 �̓_��͖���
        *pSelectedPointNum = 0;
    }
    else
    {
        // �����l�����ɓ_��I��
        for (iPoint = 0; iPoint < size; iPoint++)
        {
            selectedPoints[iPoint] = iPoint;
        }
        *pSelectedPointNum = iPoint;

        if (!PRC_IsPenUpMarker(&strokes->points[size - 1]))
        {
            // Pen Up Marker �ŏI�[����Ă��Ȃ�
            (void)PRCi_TerminateStrokes(selectedPoints, pSelectedPointNum, maxPointCount, strokes);
        }
    }

    return (*pSelectedPointNum > 0);
}

#define PRCi_ABS(x) (((x)>=0)?(x):-(x))

/*---------------------------------------------------------------------------*
  Name:         PRC_ResampleStrokes_Distance

  Description:  �V�e�B�u���b�N��������Ƀ��T���v�����O���܂�.

  Arguments:    selectedPoints, pSelectedPointNum ���ʂ��Ԃ�|�C���^
                maxPointCount       ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount      �搔�̏��
                strokes             ���`�O�̐����͍��W�l.
                threshold           ���T���v����臒l
                buffer              ��Ɨ̈�(sizeof(int)*maxPointCount �K�v)

  Returns:      ���T���v�����O�ɐ���������^.
 *---------------------------------------------------------------------------*/
BOOL
PRC_ResampleStrokes_Distance(u16 *selectedPoints,
                             int *pSelectedPointNum,
                             int maxPointCount,
                             int maxStrokeCount,
                             const PRCStrokes *strokes, int threshold, void *buffer)
{
    int     selectedPointNum;
    int     strokeCount;
    int     iPoint;
    int     size;
    PRCPoint prevPoint;
    PRCPoint *point;
    BOOL    newFlag;
    int     length;

    SDK_ASSERT(maxPointCount > 0);
    SDK_ASSERT(maxStrokeCount > 0);

    (void)buffer;

    selectedPointNum = 0;
    strokeCount = 0;

    size = strokes->size;
    point = strokes->points;

    newFlag = TRUE;
    for (iPoint = 0; iPoint < size && selectedPointNum < maxPointCount; iPoint++, point++)
    {
        if (!PRC_IsPenUpMarker(point))
        {
            if (newFlag)
            {
                // �n�_�͕K���I��
                selectedPoints[selectedPointNum] = (u16)iPoint;
                selectedPointNum++;
                length = 0;
                newFlag = FALSE;
            }
            else
            {
                length += PRCi_ABS(point->x - prevPoint.x) + PRCi_ABS(point->y - prevPoint.y);
                if (length >= threshold)
                {
                    selectedPoints[selectedPointNum] = (u16)iPoint;
                    selectedPointNum++;
                    length = 0;
                }
            }
            prevPoint = *point;
        }
        else
        {
            if (newFlag)
            {
                // �A������ Pen Up Marker �͖���
                continue;
            }
            else
            {
                if (selectedPoints[selectedPointNum - 1] != iPoint - 1) // �����ɂ���Ƃ��͏�� selectedPointNum>0
                {
                    // �I�_���K���I��
                    selectedPoints[selectedPointNum] = (u16)(iPoint - 1);
                    selectedPointNum++;
                    if (selectedPointNum >= maxPointCount)
                    {
                        break;
                    }
                }
                selectedPoints[selectedPointNum] = (u16)iPoint;
                selectedPointNum++;
                newFlag = TRUE;

                strokeCount++;
                if (strokeCount >= maxStrokeCount)
                {
                    // �搔��������
                    iPoint++;          // ���[�v��̏����̂��� // �����_�ł� newFlag == TRUE �Ȃ̂ňӖ��Ȃ�
                    break;
                }
            }
        }
    }

    *pSelectedPointNum = selectedPointNum;

    if (!newFlag)
    {
        // Pen Up Marker �ŏI�[����Ă��Ȃ�

        // �܂��A�I�_���I������Ă��邩���m�F
        if (selectedPointNum > 0 && selectedPoints[selectedPointNum - 1] != iPoint - 1
            && selectedPointNum < maxPointCount)
        {
            // �I�_���K���I��
            selectedPoints[*pSelectedPointNum] = (u16)(iPoint - 1);
            (*pSelectedPointNum)++;
        }

        (void)PRCi_TerminateStrokes(selectedPoints, pSelectedPointNum, maxPointCount, strokes);
    }

    return (*pSelectedPointNum > 0);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_ResampleStrokes_Angle

  Description:  �p�x�̂������Ƀ��T���v�����O���܂�.

  Arguments:    selectedPoints, pSelectedPointNum ���ʂ��Ԃ�|�C���^
                maxPointCount       ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount      �搔�̏��
                strokes             ���`�O�̐����͍��W�l.
                threshold           ���T���v����臒l
                buffer              ��Ɨ̈�(sizeof(int)*maxPointCount �K�v)

  Returns:      ���T���v�����O�ɐ���������^.
 *---------------------------------------------------------------------------*/
BOOL
PRC_ResampleStrokes_Angle(u16 *selectedPoints,
                          int *pSelectedPointNum,
                          int maxPointCount,
                          int maxStrokeCount,
                          const PRCStrokes *strokes, int threshold, void *buffer)
{
#define PRC_RESAMPLE_ANGLE_LENGTH_THRESHOLD 6   // 6 city-block length ���炢����Ȃ��ƗL���Ȋp�x�����Ȃ�
    int     selectedPointNum;
    int     strokeCount;
    int     iPoint;
    int     size;
    PRCPoint *point;
    BOOL    newFlag;
    u16     prevAngle;
    PRCPoint prevPoint;
    BOOL    firstFlag;

    SDK_ASSERT(maxPointCount > 0);
    SDK_ASSERT(maxStrokeCount > 0);

    (void)buffer;

    selectedPointNum = 0;
    strokeCount = 0;

    size = strokes->size;
    point = strokes->points;

    newFlag = TRUE;
    for (iPoint = 0; iPoint < size && selectedPointNum < maxPointCount; iPoint++, point++)
    {
        if (!PRC_IsPenUpMarker(point))
        {
            if (newFlag)
            {
                // �n�_�͕K���I��
                selectedPoints[selectedPointNum] = (u16)iPoint;
                selectedPointNum++;
                prevPoint = *point;
                newFlag = FALSE;
                firstFlag = TRUE;
                if (iPoint + 1 < size)
                {
                    prevAngle =
                        FX_Atan2Idx(((point + 1)->y - point->y) << FX32_SHIFT,
                                    ((point + 1)->x - point->x) << FX32_SHIFT);
                }
            }
            else
            {
                int     length;
                length = PRCi_ABS(point->x - prevPoint.x) + PRCi_ABS(point->y - prevPoint.y);
                if (length >= PRC_RESAMPLE_ANGLE_LENGTH_THRESHOLD)
                {
                    if (firstFlag)
                    {
                        // 2�_�ڂ�I�ԂƂ���܂ł́A���̓_�ւ̊p�x������
                        // �������Ȃ��ƁA�ŏ��̏����o���̕�����������
                        if (iPoint + 1 < size && !PRC_IsPenUpMarker(point + 1))
                            // point+1 �� Pen Up Marker �̂Ƃ��͂ǂ���ɂ���I�_�Ƃ��đI�������̂�
                            // ���̏����͂Ȃ��Ă������̂ł����c�c
                        {
                            u16     currAngle, nextAngle;
                            nextAngle =
                                FX_Atan2Idx(((point + 1)->y - point->y) << FX32_SHIFT,
                                            ((point + 1)->x - point->x) << FX32_SHIFT);
                            if (PRCi_ABS((s16)(prevAngle - nextAngle)) >= threshold)
                            {
                                currAngle =
                                    FX_Atan2Idx((point->y - prevPoint.y) << FX32_SHIFT,
                                                (point->x - prevPoint.x) << FX32_SHIFT);
                                selectedPoints[selectedPointNum] = (u16)iPoint;
                                selectedPointNum++;
                                prevAngle = currAngle;
                            }
                        }
                        firstFlag = FALSE;
                    }
                    else
                    {
                        u16     currAngle;
                        currAngle =
                            FX_Atan2Idx((point->y - prevPoint.y) << FX32_SHIFT,
                                        (point->x - prevPoint.x) << FX32_SHIFT);
                        if (PRCi_ABS((s16)(prevAngle - currAngle)) >= threshold)
                        {
                            selectedPoints[selectedPointNum] = (u16)iPoint;
                            selectedPointNum++;
                            prevAngle = currAngle;
                        }
                    }
                    prevPoint = *point;
                }
            }
        }
        else
        {
            if (newFlag)
            {
                // �A������ Pen Up Marker �͖���
                continue;
            }
            else
            {
                if (selectedPoints[selectedPointNum - 1] != iPoint - 1) // �����ɂ���Ƃ��͏�� selectedPointNum>0
                {
                    // �I�_���K���I��
                    selectedPoints[selectedPointNum] = (u16)(iPoint - 1);
                    selectedPointNum++;
                    if (selectedPointNum >= maxPointCount)
                    {
                        break;
                    }
                }
                selectedPoints[selectedPointNum] = (u16)iPoint;
                selectedPointNum++;
                newFlag = TRUE;

                strokeCount++;
                if (strokeCount >= maxStrokeCount)
                {
                    // �搔��������
                    iPoint++;          // ���[�v��̏����̂��� // �����_�ł� newFlag == TRUE �Ȃ̂ňӖ��Ȃ�
                    break;
                }
            }
        }
    }

    *pSelectedPointNum = selectedPointNum;

    if (!newFlag)
    {
        // Pen Up Marker �ŏI�[����Ă��Ȃ�

        // �܂��A�I�_���I������Ă��邩���m�F
        if (selectedPointNum > 0 && selectedPoints[selectedPointNum - 1] != iPoint - 1
            && selectedPointNum < maxPointCount)
        {
            // �I�_���K���I��
            selectedPoints[*pSelectedPointNum] = (u16)(iPoint - 1);
            (*pSelectedPointNum)++;
        }

        (void)PRCi_TerminateStrokes(selectedPoints, pSelectedPointNum, maxPointCount, strokes);
    }

    return (*pSelectedPointNum > 0);
}

/*---------------------------------------------------------------------------*
  Name:         PRC_ResampleStrokes_Recursive

  Description:  �ċA�I��@�Ń��T���v�����O���܂�.

  Arguments:    selectedPoints, pSelectedPointNum ���ʂ��Ԃ�|�C���^
                maxPointCount       ���͓_���̏��(pen up marker ���܂�)
                maxStrokeCount      �搔�̏��
                strokes             ���`�O�̐����͍��W�l.
                threshold           ���T���v����臒l
                buffer              ��Ɨ̈�(sizeof(int)*maxPointCount �K�v)

  Returns:      ���T���v�����O�ɐ���������^.
 *---------------------------------------------------------------------------*/
BOOL
PRC_ResampleStrokes_Recursive(u16 *selectedPoints,
                              int *pSelectedPointNum,
                              int maxPointCount,
                              int maxStrokeCount,
                              const PRCStrokes *strokes, int threshold, void *buffer)
{
    u16     beginIndex, endIndex;
    int     stackSize;
    int     stackTop, stackTail;
    int     strokeCount;
    int     selectedPointNum;
    int     size;
    PRCPoint *inputPoints;
    u16    *stackP1;
    u16    *stackP2;
    int     squaredThreshold;

    stackP1 = (u16 *)buffer;
    stackP2 = (u16 *)buffer + maxPointCount;

    squaredThreshold = threshold * threshold;

    beginIndex = 0;
    endIndex = 0;
    strokeCount = 0;
    selectedPointNum = 0;

    inputPoints = strokes->points;
    size = strokes->size;

    while (1)
    {
        if (selectedPointNum + 3 > maxPointCount || strokeCount > maxStrokeCount)
        {
            // ���� stroke ���i�[����X�y�[�X���Ȃ�
            // stroke ��1�i�[����ɂ́A�n�_�E�I�_�EPenUpMarker ��
            // 3�̋󂫂��Œ�ł��K�v
            break;
        }

        // PenUpMarker ��ǂݔ�΂�
        while (endIndex < size && PRC_IsPenUpMarker(&inputPoints[endIndex]))
        {
            endIndex++;
        }

        beginIndex = endIndex;
        if (beginIndex >= size)
        {
            // �I��
            break;
        }

        // ���� PenUpMarker ��T��
        while (endIndex < size && !PRC_IsPenUpMarker(&inputPoints[endIndex]))
        {
            endIndex++;
        }
        if (endIndex < size)
        {
            selectedPoints[selectedPointNum] = endIndex;
            selectedPointNum++;        // �K�v�� PenUpMarker �͑I��
        }
        else
        {
            selectedPoints[selectedPointNum] = (u16)-1;
            selectedPointNum++;        // -1 �͓��ʂɏI�[�� PenUpMarker ������
        }

        SDK_ASSERT(endIndex > 0);
        selectedPoints[selectedPointNum] = beginIndex;
        selectedPointNum++;
        selectedPoints[selectedPointNum] = (u16)(endIndex - 1);
        selectedPointNum++;

        strokeCount++;                 // strokeCount �� maxStrokeCount �ł̐����̂��߂����ɐ����Ă���

        if (selectedPointNum >= maxPointCount)
        {
            // ���_�������E
            break;
        }

        if (endIndex - beginIndex <= 2)
            continue;

        // stack ��p���čċA�I�ɓ����_�𒊏o
        stackP1[0] = beginIndex;
        stackP2[0] = (u16)(endIndex - 1);
        stackSize = 1;
        stackTop = 0;
        stackTail = 1;
        while (stackSize > 0)
        {
            u16     p1, p2;
            int     x1, y1, x2, y2, xDir, yDir, offs;
            int     lastX, lastY;
            int     i;
            int     maxDist;
            u16     maxP;

            p1 = stackP1[stackTop];
            p2 = stackP2[stackTop];
            stackTop++;
            if (stackTop >= maxPointCount)
            {
                stackTop = 0;
            }
            stackSize--;               // pop

            if (p2 - p1 <= 1)
                continue;

            x1 = inputPoints[p1].x;    // �n�_
            y1 = inputPoints[p1].y;
            x2 = inputPoints[p2].x;    // �I�_
            y2 = inputPoints[p2].y;
            xDir = x2 - x1;            // �����x�N�g��
            yDir = y2 - y1;
            offs = -(x1 * y2 - x2 * y1);        // �v�Z�ʍ팸�̂��߂ɂ����肾������

            maxDist = -1;
            maxP = (u16)-1;
            lastX = -1;
            lastY = -1;
            for (i = p1 + 1; i < p2; i++)
            {
                int     dist;
                int     x, y;
                x = inputPoints[i].x;
                y = inputPoints[i].y;

                if (lastX == x && lastY == y)
                    continue;
                lastX = x;
                lastY = y;

                // �����Ɠ_�̋����̌v�Z
                // ���ۂ͖{���̋����Ɏn�_�ƏI�_�̋������|����ꂽ�l�ł���
                dist = x * yDir - y * xDir + offs;
                if (dist < 0)
                {
                    dist = -dist;
                }

                if (maxDist < dist)
                {
                    maxP = (u16)i;
                    maxDist = dist;
                }
            }

            // �n�_���W�ƏI�_���W�����S�Ɉ�v����P�[�X�ł͕K�� maxDist == 0 �ł���̂�
            // dist �̌v�Z�� (x, y) �Ǝn�_���W�̃��[�N���b�h������ʂɂƂ邱�Ƃ��]�܂�����
            // ���A�P�[�X�p�̏�������ꂽ���Ȃ����߁AmaxDist==0, maxP==p1+1 �� p1+1 ��
            // ��ɍ̗p����邱�Ƃɂ��Ă݂�(���̍ۂ� xDir*xDir+yDir*yDir == 0 �ł��邱�Ƃɒ���)
            if (maxDist * maxDist >= (xDir * xDir + yDir * yDir) * squaredThreshold)
            {
                // 臒l�ȏ㗣�ꂽ�_�͓����_�Ƃ��č̗p
                // maxDist �͖{���̋����Ɏn�_�ƏI�_�̋������|����ꂽ�l�ł��������Ƃɒ���
                selectedPoints[selectedPointNum] = maxP;
                selectedPointNum++;
                stackP1[stackTail] = maxP;
                stackP2[stackTail] = p2;
                stackTail++;
                if (stackTail >= maxPointCount)
                {
                    stackTail = 0;
                }
                stackSize++;           // push
                stackP1[stackTail] = p1;
                stackP2[stackTail] = maxP;
                stackTail++;
                if (stackTail >= maxPointCount)
                {
                    stackTail = 0;
                }
                stackSize++;           // push
                SDK_ASSERT(stackSize <= maxPointCount);
                if (selectedPointNum >= maxPointCount)
                {
                    // ���_�������E
                    break;
                }
            }
        }
    }

    *pSelectedPointNum = selectedPointNum;

//{OSTick start, end; start = OS_GetTick();
    // �Ԃ��O�ɏ����Ƀ\�[�g����
    // ToDo: quick sort �łǂ̒��x����������邩��������
    {
        int     i, j;
        for (i = 0; i < selectedPointNum - 1; i++)
        {
            for (j = i + 1; j < selectedPointNum; j++)
            {
                if (selectedPoints[i] > selectedPoints[j])
                {
                    u16     tmp;
                    tmp = selectedPoints[i];
                    selectedPoints[i] = selectedPoints[j];
                    selectedPoints[j] = tmp;
                }
            }
        }
    }
//end = OS_GetTick(); OS_Printf("// sort in resample: %lld��s selectedPointNum=%d\n", OS_TicksToMicroSeconds(end-start), selectedPointNum); }
    return (*pSelectedPointNum > 0);
}























/*===========================================================================*
  Static Functions
 *===========================================================================*/


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
