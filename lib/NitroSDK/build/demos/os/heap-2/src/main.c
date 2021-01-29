/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - heap-2
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.10  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.9  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.8  2004/09/28 12:22:10  yada
  not call OS_InitArena() because OS_Init() called

  Revision 1.7  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.6  2004/04/14 00:13:30  yada
  �f�o�O�r���h�Œ�~���鎞�̂��߂̃��b�Z�[�W��ǉ�

  Revision 1.5  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.4  2004/02/12 11:41:07  yada
  ARM9/ARM7 �U�蕪�����

  Revision 1.3  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/09 09:18:55  yada
  �R�����g���C�������̂�

  Revision 1.1  2004/01/07 07:34:19  yada
  ����

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

//----------------------------------------------------------------
#define   MAIN_HEAP_SIZE    0x2000

//================================================================================
//        �q�[�v�̃T���v��
//        ���C���������̃A���[�i�ɑ΂� �q�[�v���쐬
//        �����̃q�[�v���쐬���A�Е���j������B
//        �j�������q�[�v����alloc���悤�Ƃ��Ă݂�B
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    void   *heapStart[2];
    void   *nstart;
    OSHeapHandle handle[2];
    void   *area[10];

    OS_Init();
    //OS_InitArena(); /*called in OS_Init()*/
    OS_Printf("arena lo = %x\n", OS_GetMainArenaLo());
    OS_Printf("arena hi = %x\n", OS_GetMainArenaHi());


    //---- MainRAM �A���[�i�ɑ΂��� ���������蓖�ăV�X�e��������
    nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 2);
    OS_SetMainArenaLo(nstart);

    //---- �A���[�i����q�[�v�p�̈�m��
    heapStart[0] = OS_AllocFromMainArenaLo(MAIN_HEAP_SIZE, 32);
    OS_Printf("heapStart (0) %x\n", heapStart[0]);

    //---- �q�[�v�O�쐬
    handle[0] =
        OS_CreateHeap(OS_ARENA_MAIN, heapStart[0], (void *)((u32)heapStart[0] + MAIN_HEAP_SIZE));
    OS_Printf("heap handle (0) %d\n", handle[0]);

    //---- �A���[�i����q�[�v�p�̈�m��
    heapStart[1] = OS_AllocFromMainArenaLo(MAIN_HEAP_SIZE, 32);
    OS_Printf("heapStart (1) %x\n", heapStart[1]);

    //---- �q�[�v�P�쐬
    handle[1] =
        OS_CreateHeap(OS_ARENA_MAIN, heapStart[1], (void *)((u32)heapStart[1] + MAIN_HEAP_SIZE));
    OS_Printf("heap handle (1) %d\n", handle[1]);

    //---- �J�����g�q�[�v�ݒ�i�q�[�v�O��)
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle[0]);

    //---- �̈���m�ۂ��Ă݂�
    area[0] = OS_Alloc(32);
    area[1] = OS_Alloc(32);
    area[2] = OS_Alloc(32);
    area[3] = OS_Alloc(64);

    OS_Printf("area[0] %x\n", area[0]);
    OS_Printf("area[1] %x\n", area[1]);
    OS_Printf("area[2] %x\n", area[2]);
    OS_Printf("area[3] %x\n", area[3]);

    //---- �q�[�v���\��
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);


    //---- �̈��������Ă݂�
    OS_Free(area[1]);
    OS_Free(area[3]);

    //---- �q�[�v���\��
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);

    //---- �q�[�v�O��j������
    OS_DestroyHeap(OS_ARENA_MAIN, handle[0]);
    OS_Printf("---------------- heap 0 destroyed\n");

    //---- �q�[�v���\��
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);

    //---- �j�������q�[�v�O����̈���m�ۂ��Ă݂�(���s����͂�)
#ifdef SDK_DEBUG
    OS_Printf
        ("**** Next 'OS_AllocFromHeap' will be stopped by assertion in case of DEBUG-build. ***\n");
#endif
    area[4] = OS_AllocFromHeap(OS_ARENA_MAIN, 0, 64);
    OS_Printf("\narea[4] %x\n", area[4]);

    //---- ������x�q�[�v�쐬(�q�[�v�O)
    handle[0] =
        OS_CreateHeap(OS_ARENA_MAIN, heapStart[0], (void *)((u32)heapStart[0] + MAIN_HEAP_SIZE));
    OS_Printf("---------------- heap 0 re-created\n");
    OS_Printf("heap handle (0) %d\n", handle[0]);

    //---- �̈�m��
    area[5] = OS_AllocFromHeap(OS_ARENA_MAIN, 0, 64);
    OS_Printf("\narea[5] %x\n", area[5]);

    //---- �q�[�v���\��
    OS_DumpHeap(OS_ARENA_MAIN, handle[0]);
    OS_DumpHeap(OS_ARENA_MAIN, handle[1]);

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/*====== End of main.c ======*/
