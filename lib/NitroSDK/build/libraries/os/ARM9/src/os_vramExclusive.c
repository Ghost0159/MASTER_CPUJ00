/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - libraries
  File:     os_vramExclusive.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: os_vramExclusive.c,v $
  Revision 1.4  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.3  2005/02/28 05:26:29  yosizaki
  do-indent.

  Revision 1.2  2005/02/18 06:18:49  yasu
  Signed/Unsigned �ϊ��x���}��

  Revision 1.1  2005/02/08 08:38:20  terui
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include        <nitro/os/ARM9/vramExclusive.h>
#include        <nitro/os/common/interrupt.h>


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u32 OSi_vramExclusive;
static u16 OSi_vramLockId[OS_VRAM_BANK_KINDS];


/*===========================================================================*/

/*---------------------------------------------------------------------------*
  Name:         OsCountZeroBits

  Description:  32�r�b�g�̒l�̓��A��ʂ��琔����0������ł��鐔��Ԃ��B

  Arguments:    bitmap  -       ��������l�B

  Returns:      u32             -       0�r�b�g�̐��B
                                                        0x80000000�̏ꍇ 0 �A0x00000000�̏ꍇ 32�ƂȂ�B
 *---------------------------------------------------------------------------*/
#include        <nitro/code32.h>
static asm u32
OsCountZeroBits( u32 bitmap )
{
    clz         r0,             r0
        bx              lr
        }
#include        <nitro/codereset.h>


/*---------------------------------------------------------------------------*
  Name:         OSi_InitVramExclusive

  Description:  VRAM �r������������������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void
OSi_InitVramExclusive( void )
{
    s32         i;

    OSi_vramExclusive = 0x0000;
    for( i = 0 ; i < OS_VRAM_BANK_KINDS ; i ++ )
    {
        OSi_vramLockId[ i ] = 0;
    }
}

/*---------------------------------------------------------------------------*
  Name:         OSi_TryLockVram

  Description:  VRAM �r�����b�N�����݂�B

  Arguments:    bank    -       �r�����b�N�����݂� VRAM �� ID �r�b�g�}�b�v�B
                                lockId  -       ���b�N����ۂ̌��ƂȂ�C�ӂ� ID�B

  Returns:      BOOL    -       ���b�N�ɐ��������ꍇ�� TRUE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL OSi_TryLockVram(u16 bank, u16 lockId)
{
    u32     workMap;
    s32     zeroBits;
    OSIntrMode enabled = OS_DisableInterrupts();

    // ���ɕ� ID �ɂ���Ĕr�����b�N����Ă��Ȃ����m�F
    workMap = (u32)(bank & OSi_vramExclusive);
    while (TRUE)
    {
        zeroBits = (s32)(31 - OsCountZeroBits(workMap));
        if (zeroBits < 0)
        {
            break;
        }
        workMap &= ~(0x00000001 << zeroBits);
        if (OSi_vramLockId[zeroBits] != lockId)
        {
            (void)OS_RestoreInterrupts(enabled);
            return FALSE;
        }
    }

    // �w�肳�ꂽ ID �� VRAM ��S�ă��b�N
    workMap = (u32)(bank & OS_VRAM_BANK_ID_ALL);
    while (TRUE)
    {
        zeroBits = (s32)(31 - OsCountZeroBits(workMap));
        if (zeroBits < 0)
        {
            break;
        }
        workMap &= ~(0x00000001 << zeroBits);
        OSi_vramLockId[zeroBits] = lockId;
        OSi_vramExclusive |= (0x00000001 << zeroBits);
    }

    (void)OS_RestoreInterrupts(enabled);
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         OSi_UnlockVram

  Description:  VRAM �r�����b�N����������B

  Arguments:    bank    -       �r�����b�N���������� VRAM �� ID �r�b�g�}�b�v�B
                                lockId  -       ���b�N�����ۂɎw�肵���C�ӂ� ID�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void OSi_UnlockVram(u16 bank, u16 lockId)
{
    u32     workMap;
    s32     zeroBits;
    OSIntrMode enabled = OS_DisableInterrupts();

    workMap = (u32)(bank & OSi_vramExclusive & OS_VRAM_BANK_ID_ALL);
    while (TRUE)
    {
        zeroBits = (s32)(31 - OsCountZeroBits((u32)workMap));
        if (zeroBits < 0)
        {
            break;
        }
        workMap &= ~(0x00000001 << zeroBits);
        if (OSi_vramLockId[zeroBits] == lockId)
        {
            OSi_vramLockId[zeroBits] = 0;
            OSi_vramExclusive &= ~(0x00000001 << zeroBits);
        }
    }

    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
