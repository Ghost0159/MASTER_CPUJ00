/*---------------------------------------------------------------------------*
  Project:  NitroSDK - include
  File:     qsort.h

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: qsort.h,v $
  Revision 1.6  2007/02/20 00:28:10  kitase_hirotake
  indent source

  Revision 1.5  2006/07/18 07:59:38  okubata_ryoma
  ��sort�̃f���Ń\�[�g�������0�ɂ����Ƃ���ASSERT�Ŏ~�܂�����C��

  Revision 1.4  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.3  2005/04/13 00:16:03  takano_makoto
  �����̌^��int����u32�ɕύX

  Revision 1.2  2005/04/12 23:55:34  takano_makoto
  fix for warning message.

  Revision 1.1  2005/04/12 10:16:14  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef NITRO_MATH_QSORT_H_
#define NITRO_MATH_QSORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/math/math.h>

/* �l��r�p�֐� */
typedef s32 (*MATHCompareFunc) (void *elem1, void *elem2);

/* �ő�K�v�X�^�b�N�T�C�Y */
/*---------------------------------------------------------------------------*
  Name:         MATH_QSortStackSize
  
  Description:  MATH_QSort���s�Ȃ��ۂɕK�v�ȍ�ƃo�b�t�@�T�C�Y���v�Z���܂��B
  
  Arguments:    num      �\�[�g����f�[�^�̌�
                
  Returns:      �K�v�o�b�t�@�T�C�Y.
 *---------------------------------------------------------------------------*/
static inline u32 MATH_QSortStackSize(u32 num)
{
    int     tmp = MATH_ILog2(num);

    if (tmp <= 0)
    {
        return sizeof(u32);
    }
    else
    {
        return (u32)((MATH_ILog2(num) + 1) * sizeof(u32) * 2);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MATH_QSort
  
  Description:  �ċA���g�p�����ɃN�C�b�N�\�[�g���s�Ȃ��܂��B
                �\�[�g�p�̃o�b�t�@�̈��n���K�v������A
                �K�v�ȃo�b�t�@�T�C�Y��MATH_QSORT_STACK_SIZE( num )�Ŏ擾�ł��܂��B
  
  Arguments:    head     �\�[�g����f�[�^�ւ̃|�C���^
                num      �\�[�g����f�[�^�̌�
                width    �\�[�g����f�[�^�P���̃f�[�^�T�C�Y
                comp     ��r�֐��|�C���^
                stackBuf �\�[�g�p�o�b�t�@
                
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    MATH_QSort(void *head, u32 num, u32 width, MATHCompareFunc comp, void *stackBuf);


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NITRO_MATH_QSORT_H_ */
#endif
