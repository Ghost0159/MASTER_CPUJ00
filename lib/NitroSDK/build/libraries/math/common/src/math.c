/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MATH - 
  File:     math.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: math.c,v $
  Revision 1.8  2007/10/04 11:36:15  okubata_ryoma
  Copyright fix

  Revision 1.7  2007/10/03 02:34:33  seiki_masashi
  ARM9 ��ł� thumb ���C�u�����ł���� CLZ ���߂��Ăяo���悤�ɕύX

  Revision 1.6  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.5  2005/11/24 03:09:27  seiki_masashi
  MATH_CountPopulation �� ARM �A�Z���u���ɂ��Q�l���� (writtten by terui) ���R�����g�Ƃ��Ēǉ�

  Revision 1.4  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.3  2005/02/18 07:12:45  seiki_masashi
  warning �΍�

  Revision 1.2  2005/01/11 07:40:17  takano_makoto
  fix copyright header.

  Revision 1.1  2005/01/06 06:25:50  seiki_masashi
  ARM7�p���C�u��������������悤�ɕύX

  Revision 1.2  2004/12/15 09:17:38  seiki_masashi
  MATH_CountPopulation �̒ǉ�

  Revision 1.1  2004/12/14 10:51:26  seiki_masashi
  MATH �W��������ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro/math.h>

/*---------------------------------------------------------------------------*
  Name:         MATH_CountLeadingZerosFunc

  Description:  2�i��32�r�b�g�\���ŏ�ʉ��r�b�g��0�������߂�
                (CLZ ���߂��Ȃ� ARM7 or ARM9 Thumb �p)

  Arguments:    x

  Returns:      ��ʂ���A������0�̃r�b�g��
 *---------------------------------------------------------------------------*/
#if defined(SDK_ARM9) && (defined(SDK_CW) || defined(__MWERKS__))

#pragma thumb off
u32 MATH_CountLeadingZerosFunc(u32 x)
{
    // ARM9 ��ł́A�Ăяo������ Thumb �������Ƃ��Ă�
    // ARM ���[�h�ɐ؂�ւ��� CLZ ���߂��Ăяo��������
    // ���x���T�C�Y���L��
    asm
    {
    clz     x, x}
    return  x;
}
#pragma thumb reset

#else // !ARM9 || !(CW || __MWERKS__)

u32 MATH_CountLeadingZerosFunc(u32 x)
{
    u32     y;
    u32     n = 32;

    // �񕪒T���� 0 ���I���ꏊ��T���B
    y = x >> 16;
    if (y != 0)
    {
        n -= 16;
        x = y;
    }
    y = x >> 8;
    if (y != 0)
    {
        n -= 8;
        x = y;
    }
    y = x >> 4;
    if (y != 0)
    {
        n -= 4;
        x = y;
    }
    y = x >> 2;
    if (y != 0)
    {
        n -= 2;
        x = y;
    }
    y = x >> 1;
    if (y != 0)
    {
        n -= 2;
    }                                  // x == 0b10 or 0b11 -> n -= 2
    else
    {
        n -= x;
    }                                  // x == 0b00 or 0b01 -> n -= x

    return n;
}

#endif // ARM9 && (CW || __MWERKS__)

/*---------------------------------------------------------------------------*
  Name:         MATH_CountPopulation

  Description:  2�i��32�r�b�g�\����1�̃r�b�g�������߂�

  Arguments:    x

  Returns:      2�i�\����1�ƂȂ�r�b�g��
 *---------------------------------------------------------------------------*/
u8 MATH_CountPopulation(u32 x)
{
    // �ȉ��AARM �R�[�h�ɂ����Ă̓V�t�g�ƎZ�p���Z�͓����ɍs���邱�Ƃɒ��ӁB
    // Release Build �ȏ�ł̓X�g�[�������� 13 ���� + bx lr �ƂȂ�B

    // 32bit �𒼐ڐ������ɁA�ŏ��͊e 2bit ���� 1 �̐���
    // �����ʒu�� 2bit �Ɋi�[����B
    // ���Ȃ킿�A�e 2bit �� 00 -> 00, 01 -> 01, 10 -> 01, 11 -> 10 �̕ϊ��B
    // x -> x' �Ƃ���ƁA2bit �l�ł� x' = x - (x >> 1)
    x -= ((x >> 1) & 0x55555555);

    // 4bit �P�ʂōl���A��� 2bit �� 2bit �Ɋi�[����Ă��� 1 �̐��𑫂���
    // �ŏ��̂��̈ʒu�� 4bit ���ɂ����� 1 �̐��Ƃ��Ċi�[����B
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);

    // 8bit �P�ʂœ��l�ɁB
    // �������A�e���̌��ʂ͍ő�� 8 �ł��邱�Ƃ��� 4bit �Ŏ��܂�̂�
    // ���O�Ƀ}�X�N����K�v�͂Ȃ��B
    x += (x >> 4);

    // ���̉��Z�ɔ����A�s�v�ȕ������}�X�N����B
    x &= 0x0f0f0f0f;

    // 16bit �P�ʂŏ�� 8bit �Ɖ��� 8bit �̘a�����B
    x += (x >> 8);

    // 32bit �P�ʂœ��l�ɁB
    x += (x >> 16);

    // ���� 8bit �̒l�����ʂł���B
    return (u8)x;
}

#if 0
// �A�Z���u���ɂ��Q�l����
// �r�b�g�������Ă��Ȃ����Ƃ̂ق��������ꍇ�͂�����̂ق�������
/*---------------------------------------------------------------------------*
  Name:         MATH_CountPopulation_Asm

  Description:  �l��32�r�b�g�ŕ\�����ꍇ�� 1 �ɂȂ��Ă���r�b�g�̐��𒲂ׂ�B

  Arguments:    value   -   ��������l�B

  Returns:      u32     -   �r�b�g�̐���Ԃ��B0 �` 32 �̐��l�B
 *---------------------------------------------------------------------------*/
#include <nitro/code32.h>
asm u32 MATH_CountPopulation_Asm(u32 value)
{
    mov     r1 ,    #0
    mov     r2 ,    #1

@loop:
    clz     r3 ,    r0
    rsbs    r3 ,    r3 ,    #31
    bcc     @end
    add     r1 ,    r1 ,    #1
    mvn     r3 ,    r2 ,    LSL r3
    and     r0 ,    r0 ,    r3
    b       @loop

@end:
    mov     r0 ,    r1
    bx      lr
}
#include <nitro/codereset.h>
#endif
