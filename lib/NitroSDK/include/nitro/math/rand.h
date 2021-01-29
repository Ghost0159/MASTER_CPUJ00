/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - include
  File:     rand.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: rand.h,v $
  Revision 1.4  2006/01/18 02:12:28  kitase_hirotake
  do-indent

  Revision 1.3  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.2  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.1  2004/12/14 13:11:59  takano_makoto
  FX����MATH�ֈړ�

  Revision 1.1  2004/12/13 07:44:20  takano_makoto
  RND����FX�ֈړ�

  Revision 1.1  2004/12/09 12:35:22  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/types.h>

#ifndef NITRO_MATH_RAND_H_
#define NITRO_MATH_RAND_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    u64     x;                         // �����l
    u64     mul;                       // �搔
    u64     add;                       // ���Z���鐔
}
MATHRandContext32;


typedef struct
{
    u32     x;                         // �����l
    u32     mul;                       // �搔
    u32     add;                       // ���Z���鐔
}
MATHRandContext16;


/*---------------------------------------------------------------------------*
  Name:         MATH_InitRand32

  Description:  ���`�����@�ɂ��32bit�����R���e�L�X�g�����������܂��B

  Arguments:    context �����R���e�L�X�g�ւ̃|�C���^
  
                seed    �����l�Ƃ��Đݒ肷�闐���̎�
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_InitRand32(MATHRandContext32 *context, u64 seed)
{
    context->x = seed;
    context->mul = (1566083941LL << 32) + 1812433253LL;
    context->add = 2531011;
}


/*---------------------------------------------------------------------------*
  Name:         MATH_Rand32

  Description:  ���`�����@�ɂ��32bit�����擾�֐�

  Arguments:    context �����R���e�L�X�g�ւ̃|�C���^
  
                max     �擾���l�͈̔͂��w�� 0 �` max-1 �͈̔͂̒l���擾�ł��܂��B
                        0���w�肵���ꍇ�ɂ͂��ׂĂ͈̔͂�32bit�l�ƂȂ�܂��B
  
  Returns:      32bit�̃����_���l
 *---------------------------------------------------------------------------*/
static inline u32 MATH_Rand32(MATHRandContext32 *context, u32 max)
{
    context->x = context->mul * context->x + context->add;

    // ����max���萔�Ȃ�΃R���p�C���ɂ��œK�������B
    if (max == 0)
    {
        return (u32)(context->x >> 32);
    }
    else
    {
        return (u32)(((context->x >> 32) * max) >> 32);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MATH_InitRand16

  Description:  ���`�����@�ɂ��16bit�����R���e�L�X�g�����������܂��B

  Arguments:    context �����R���e�L�X�g�ւ̃|�C���^
  
                seed    �����l�Ƃ��Đݒ肷�闐���̎�
  
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MATH_InitRand16(MATHRandContext16 *context, u32 seed)
{
    context->x = seed;
    context->mul = 1566083941LL;
    context->add = 2531011;
}


/*---------------------------------------------------------------------------*
  Name:         MATH_Rand16

  Description:  ���`�����@�ɂ��16bit�����擾�֐�

  Arguments:    context �����R���e�L�X�g�ւ̃|�C���^
  
                max     �擾���l�͈̔͂��w�� 0 �` max-1 �͈̔͂̒l���擾�ł��܂��B
                        0���w�肵���ꍇ�ɂ͂��ׂĂ͈̔͂�16bit�l�ƂȂ�܂��B
  
  Returns:      16bit�̃����_���l
 *---------------------------------------------------------------------------*/
static inline u16 MATH_Rand16(MATHRandContext16 *context, u16 max)
{
    context->x = context->mul * context->x + context->add;

    // ����max���萔�Ȃ�΃R���p�C���ɂ��œK�������B
    if (max == 0)
    {
        return (u16)(context->x >> 16);
    }
    else
    {
        return (u16)(((context->x >> 16) * max) >> 16);
    }
}


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif // NITRO_MATH_RAND_H_
