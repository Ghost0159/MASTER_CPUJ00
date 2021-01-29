/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-4
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.13  2005/11/21 10:56:02  kitase_hirotake
  SVC_WaitVBlankIntr �� OS_WaitVBlankIntr �ɕύX

  Revision 1.12  2005/03/03 13:00:06  takano_makoto
  exclude any code which is available in specific compiler.

  Revision 1.11  2005/02/28 05:26:20  yosizaki
  do-indent.

  Revision 1.10  2004/04/15 06:51:12  yada
  only arrange comment

  Revision 1.9  2004/03/08 08:31:58  yada
  fix top comment

  Revision 1.8  2004/02/26 05:51:46  yada
  OS_CreateThread() �ɋN���֐��̈������ǉ�����A
  ����ɔ����C��

  Revision 1.7  2004/02/24 00:05:20  yada
  (none)

  Revision 1.6  2004/02/05 07:09:03  yasu
  change SDK prefix iris -> nitro

  Revision 1.5  2004/02/02 05:34:12  yada
  OS_SetIrqMask()��OS_EnableIrqMask() ��

  Revision 1.4  2004/01/18 06:49:20  yada
  (none)

  Revision 1.3  2004/01/09 10:53:49  yada
  �����݃`�F�b�N�t���O�̃Z�b�g�֐���������

  Revision 1.2  2003/12/25 07:32:21  yada
  �^���[������ɂ��ύX

  Revision 1.1  2003/12/16 06:30:36  yasu
  thread-4 ���荞�݂ɂ�� thread �؂�ւ������̃T���v��

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

void    VBlankIntr(void);

#define    STACK_SIZE     1024
#define    THREAD1_PRIO   1
#define    THREAD2_PRIO   2

void    proc1(void *p1);
void    proc2(void *p1);

OSThread thread1;
OSThread thread2;

u32     stack1[STACK_SIZE / sizeof(u32)];
u32     stack2[STACK_SIZE / sizeof(u32)];

//================================================================================
//      ���荞�݂𗘗p�����X���b�h�̐؂�ւ��̃T���v��
//      IRQ �n���h�����ŃX���b�h��Q��������N��������o����A���������ۂ�
//      �X���b�h�̐؂�ւ����N����̂� IRQ ���[�`���I���܂ŉ��������D
/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  main

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    s32     n;

    OS_Init();

    //================ �X���b�h�N��
    OS_InitThread();
    OS_CreateThread(&thread1, proc1, NULL, stack1 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD1_PRIO);
    OS_CreateThread(&thread2, proc2, NULL, stack2 + STACK_SIZE / sizeof(u32), STACK_SIZE,
                    THREAD2_PRIO);
    OS_WakeupThreadDirect(&thread1);
    OS_WakeupThreadDirect(&thread2);

    //================ �ݒ�
#ifdef SDK_ARM9
    //---- �O���t�B�N�X������
    GX_Init();
#endif // SDK_ARM9

    //----  V�u�����N��������
    (void)OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    //---- V�u�����N�����ݒ�
    (void)GX_VBlankIntr(TRUE);

    //================ ���C�����[�v
    for (n = 0; n < 5; n++)
    {
        //---- V�u�����N�����I���҂�
        OS_WaitVBlankIntr();

        OS_Printf("Idle\n");
    }

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

//--------------------------------------------------------------------------------
//    �u�u�����N���荞�ݏ��� (IRQ ������)
//
void VBlankIntr(void)
{
    //---- ���荞�݃`�F�b�N�t���O
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    //---- �X���b�h�P�N��
    OS_WakeupThreadDirect(&thread1);
}

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        OS_Printf("------- Thread1 sleep\n");
        OS_SleepThread(NULL);
        OS_Printf("------- Thread1 wakeup\n");

        //---- �X���b�h�Q�N��
        OS_WakeupThreadDirect(&thread2);
    }
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *p1)
{
#pragma unused(p1)
    while (1)
    {
        OS_Printf("--------------- Thread2 sleep\n");
        OS_SleepThread(NULL);
        OS_Printf("--------------- Thread2 wakeup\n");
    }
}

/*====== End of main.c ======*/
