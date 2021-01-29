/*---------------------------------------------------------------------------*
  Project:  NitroSDK - PRC - 
  File:     prc.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: prc.c,v $
  Revision 1.7  2006/01/18 02:12:27  kitase_hirotake
  do-indent

  Revision 1.6  2005/02/28 05:26:29  yosizaki
  do-indent.

  Revision 1.5  2004/07/19 06:52:06  seiki_masashi
  ���ʎq���̐���

  Revision 1.4  2004/06/25 13:32:11  seiki_masashi
  �F���A���S���Y�� 'Standard' �̒ǉ�

  Revision 1.3  2004/06/24 08:07:34  seiki_masashi
  PRCi_Init ��2�d�Ăяo���ɑΉ�

  Revision 1.2  2004/06/24 07:03:39  seiki_masashi
  �w�b�_�t�@�C���𕪊��E�������܂���

  Revision 1.1  2004/06/23 11:59:30  seiki_masashi
  add 'PRC*' to SDK

  Revision 1.3  2004/06/17 02:30:29  seiki_masashi
  �F���A���S���Y���̍����ւ���e�Ղɂ��邽�߂� prc_algo_* �ɕ������܂���

  Revision 1.2  2004/06/17 00:52:36  seiki_masashi
  PRC_InitPrototypeDBEX, PRC_InitInputPatternEX

  Revision 1.1  2004/06/16 23:56:09  seiki_masashi
  rename ptrcg to prc.


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nitro/prc/common.h>

/*===========================================================================*
  Prototype Declarations
 *===========================================================================*/

/*===========================================================================*
  Variable Definitions
 *===========================================================================*/
static BOOL PRCi_Initialized = FALSE;

/*===========================================================================*
  Functions
 *===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         PRCi_Init

  Description:  �}�`�F�� API �����������܂�.
                �e�F���A���S���Y���̏��������[�`������ŏ��ɌĂяo����܂�.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PRCi_Init(void)
{
    if (PRCi_Initialized == TRUE)
        return;

    // Initialize Something...

    PRCi_Initialized = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         PRC_GetEntryStrokes

  Description:  ���{DB�ƌ��{DB�G���g������A�_��f�[�^�𓾂܂�.

  Arguments:    strokes         ����ꂽ�_��f�[�^.
                                ���������Ă͂����܂���.
                prototypeList   ���{�p�^�[���̈ꗗ.
                entry           ���{DB�G���g��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void
PRC_GetEntryStrokes(PRCStrokes *strokes,
                    const PRCPrototypeList *prototypeList, const PRCPrototypeEntry *entry)
{
    if (entry != NULL)
    {
        strokes->points = (PRCPoint *)&prototypeList->pointArray[entry->pointIndex];
        strokes->size = entry->pointCount;
        strokes->capacity = entry->pointCount;
    }
    else
    {
        strokes->points = NULL;
        strokes->size = 0;
        strokes->capacity = 0;
    }
}

/*---------------------------------------------------------------------------*
  Name:         PRC_CopyStrokes

  Description:  �_��f�[�^���f�B�[�v�R�s�[���܂�.

  Arguments:    srcstrokes         �R�s�[���� PRC_Strokes �\����
                dststrokes         �R�s�[��� PRC_Strokes �\����.

  Returns:      �R�s�[�ł����� TRUE.
 *---------------------------------------------------------------------------*/
BOOL PRC_CopyStrokes(const PRCStrokes *srcStrokes, PRCStrokes *dstStrokes)
{
    int     iPoint, size;
    PRCPoint *dstPoint;
    const PRCPoint *srcPoint;

    SDK_ASSERT(dstStrokes);
    SDK_ASSERT(srcStrokes);

    if (srcStrokes->size > dstStrokes->capacity)
    {
        return FALSE;
    }

    size = dstStrokes->size = srcStrokes->size;
    srcPoint = srcStrokes->points;
    dstPoint = dstStrokes->points;
    for (iPoint = 0; iPoint < size; iPoint++)
    {
        *dstPoint = *srcPoint;
        dstPoint++;
        srcPoint++;
    }

    return TRUE;
}


/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
