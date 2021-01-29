/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - thread-8
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.4  2006/01/18 02:11:29  kitase_hirotake
  do-indent

  Revision 1.3  2005/07/22 01:12:06  adachi_hiroaki
  �p�b�h������ʃX���b�h�ɕ�����

  Revision 1.2  2005/07/12 06:42:05  adachi_hiroaki
  �f�X�g���N�^�Ή��ASDK_THREAD_INFINITY���Ȃ��ꍇ��������Ɠ��삷��悤��

  Revision 1.1  2005/07/12 01:57:17  adachi_hiroaki
  initial release


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#define STACK_SIZE 1536
#define MAIN_HEAP_SIZE 0x100000
#define MESSAGE_QUEUE_SIZE 64

#ifndef SDK_THREAD_INFINITY
const int g_nThreadMax = OS_THREAD_MAX_NUM;
#else
const int g_nThreadMax = 1024;
#endif

OSThreadQueue g_threadQueue;

OSMessage g_msgBuffer[MESSAGE_QUEUE_SIZE];
OSMessageQueue g_msgQueue;

BOOL    g_bExit = FALSE;
int     g_nTotalThreadCount = 0;

void    proc1(void *);
void    proc2(void *);
void    proc3(void *);


void    AllocThreadStorage(OSThread **ppThread, void **ppStackTop);
void    FreeThreadStorage(OSThread *pThread);
void    ManageThreadStorage(void *);
void    ReadPad(void *);

void    RequestFree(void *);

OSThread *SpawnThread(void (*func) (void *), u32 prio);
void    PrepareAlloc();

void NitroMain(void)
{

    OSThread *pThreadManage = NULL;
    int     nThreadStart;

    /* ������ */
    OS_Init();
    OS_InitThread();
    OS_InitThreadQueue(&g_threadQueue);
    OS_InitMessageQueue(&g_msgQueue, &g_msgBuffer[0], MESSAGE_QUEUE_SIZE);

    /* OS_Sleep()�̂��߂̏����� */
    OS_InitTick();
    OS_InitAlarm();
    (void)OS_EnableIrq();

    PrepareAlloc();

    OS_Printf("==== sample start\n");
    OS_Printf("infinity thread support: ");
#ifdef SDK_THREAD_INFINITY
    OS_Printf("yes\n");
#else
    OS_Printf("no\n");
#endif

    (void)OS_SetThreadPriority(OS_GetCurrentThread(), 31);

    /* �Ǘ��p�X���b�h�̍쐬 */
    pThreadManage = SpawnThread(ManageThreadStorage, 12);
    OS_Printf("ManageThreadStorage id=%d\n", pThreadManage->id);

    /* �����X���b�h�̐����o���Ă���(�Ǘ��X���b�h��) */
    nThreadStart = OS_GetNumberOfThread();

    /*  �p�b�h���ǂݎ��X���b�h�̍쐬 */
    (void)SpawnThread(ReadPad, 8);

    /* ���􂷂�X���b�h�̍ŏ����쐬 */
    (void)SpawnThread(proc1, 16);

    /* ���݂̃X���b�h���Ə����X���b�h�̐�����v����ΏI�� */
    while (nThreadStart < OS_GetNumberOfThread())
    {
        (void)OS_DisableScheduler();
        OS_WakeupThread(&g_threadQueue);
        (void)OS_EnableScheduler();
        OS_RescheduleThread();
    }
    /* �Ǘ��p�X���b�h���I�������� */

#ifdef SDK_THREAD_INFINITY
    OS_KillThread(pThreadManage, NULL);
    OS_JoinThread(pThreadManage);
#else
    OS_DestroyThread(pThreadManage);
    OS_JoinThread(pThreadManage);
    FreeThreadStorage(pThreadManage);
#endif

    OS_Printf("==== Finish sample.\n");
    OS_Terminate();
}

/* �X���b�h�ɕK�v�ȃ��������m�ۂ��� */
void AllocThreadStorage(OSThread **ppThread, void **ppStackTop)
{
    /* Warning���o�Ȃ��悤�ɋ󂫃��������`�F�b�N */
    if (OS_GetMaxFreeSize(OS_ARENA_MAIN, OS_CURRENT_HEAP_HANDLE) < STACK_SIZE
        || OS_GetTotalFreeSize(OS_ARENA_MAIN,
                               OS_CURRENT_HEAP_HANDLE) < STACK_SIZE + sizeof(OSThread))
    {
        *ppThread = NULL;
        *ppStackTop = NULL;
        return;
    }
    *ppThread = OS_Alloc(sizeof(OSThread));
    *ppStackTop = OS_Alloc(STACK_SIZE);
}

/* �X���b�h���g�p���Ă������������J������ */
void FreeThreadStorage(OSThread *pThread)
{
    SDK_NULL_ASSERT(pThread);
    OS_Free((void *)pThread->stackTop);
    OS_Free(pThread);
}

/* �X���b�h�ɕK�v�ȃ��������m�ۂ��ăX���b�h���쐬���� */
OSThread *SpawnThread(void (*func) (void *), u32 prio)
{
    OSThread *pThread = NULL;
    void   *pStackTop = NULL;
    AllocThreadStorage(&pThread, &pStackTop);
    if (!pThread || !pStackTop || OS_GetNumberOfThread() >= g_nThreadMax)
    {
        return NULL;
    }
    OS_CreateThread(pThread, func, NULL, (void *)((u8 *)pStackTop + STACK_SIZE), STACK_SIZE, prio);

#ifdef SDK_THREAD_INFINITY
    OS_SetThreadDestructor(pThread, RequestFree);
#endif

    /* ���s�҂���Ԃɂ͂��邪�A�R���e�L�X�g�X�C�b�`�͂����Ȃ� */
    (void)OS_DisableScheduler();
    OS_WakeupThreadDirect(pThread);
    (void)OS_EnableScheduler();

    ++g_nTotalThreadCount;
    OS_Printf("spawn     : id=%08x, number=%d(total %d)\n", pThread->id, OS_GetNumberOfThread(),
              g_nTotalThreadCount);
    return pThread;
}

/* �q�[�v�̏��� */
void PrepareAlloc()
{
    void   *heapStart = NULL;
    void   *nstart = NULL;
    OSHeapHandle handle;
    //---- MainRAM �A���[�i�ɑ΂��� ���������蓖�ăV�X�e��������
    nstart = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 2);
    OS_SetMainArenaLo(nstart);

    //---- �A���[�i����q�[�v�p�̈�m��
    heapStart = OS_AllocFromMainArenaLo(MAIN_HEAP_SIZE, 32);
    OS_Printf("heapStart %x\n", heapStart);

    //---- �q�[�v�쐬
    handle = OS_CreateHeap(OS_ARENA_MAIN, heapStart, (void *)((u32)heapStart + MAIN_HEAP_SIZE));
    OS_Printf("heap handle %d\n", handle);

    //---- �J�����g�q�[�v�ݒ�
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, handle);
}

/* �Ǘ��p�X���b�h�F�����Ă���OSThread*�̃X���b�h���I��������Ɏg�p���Ă������������J������ */
void ManageThreadStorage(void *)
{
    OSMessage message;
    OSThread *pThread = NULL;
    while (TRUE)
    {
        (void)OS_ReceiveMessage(&g_msgQueue, &message, OS_MESSAGE_BLOCK);
        pThread = (OSThread *)message;
        OS_JoinThread(pThread);
        OS_Printf("terminate : id=%08x, number=%d\n", pThread->id, OS_GetNumberOfThread());
        FreeThreadStorage(pThread);
    }
}

/*  */
void ReadPad(void *)
{
    u32     nPadResult = 0, nPadResultPrev = 0;
    while (!g_bExit)
    {
        nPadResultPrev = nPadResult;
        nPadResult = PAD_Read();
        if ((nPadResult ^ nPadResultPrev) & nPadResult & PAD_BUTTON_A)
        {
            /* A�{�^���ŃX���b�h�_���v��\�� */
            OS_DumpThreadList();
        }
        if (nPadResult & PAD_BUTTON_A)
        {
            /* A�{�^���������Ă���Ԃ͈ꎞ��~ */
            continue;
        }
        else if (nPadResult & PAD_BUTTON_B)
        {
            /* �X���b�h�̍쐬����߂� */
            g_bExit = TRUE;
        }
        OS_Sleep(100);
    }
#ifndef SDK_THREAD_INFINITY
    RequestFree(NULL);
#endif
}

/* �Ǘ��p�X���b�h�Ɏ��g�̃X���b�h���g�p���Ă��郁�������J�����Ă��炤 */
void RequestFree(void *)
{
    OSMessage message = (OSMessage)OS_GetCurrentThread();
    (void)OS_SendMessage(&g_msgQueue, message, OS_MESSAGE_NOBLOCK);
}

//--------------------------------------------------------------------------------
//    proc1
//
void proc1(void *)
{
    OSMessage message = (OSMessage)OS_GetCurrentThread();
    int     i;
    for (i = 0; i < 32; ++i)
    {
        OS_SleepThread(&g_threadQueue);
    }
    if (!g_bExit)
    {
        (void)SpawnThread(proc2, 16);
        (void)SpawnThread(proc3, 16);
    }
#ifndef SDK_THREAD_INFINITY
    RequestFree(NULL);
#endif
}

//--------------------------------------------------------------------------------
//    proc2
//
void proc2(void *)
{
    OSMessage message = (OSMessage)OS_GetCurrentThread();
    int     i;
    for (i = 0; i < 4; ++i)
    {
        OS_SleepThread(&g_threadQueue);
    }
    if (!g_bExit)
    {
        (void)SpawnThread(proc1, 16);
        (void)SpawnThread(proc3, 16);
    }
#ifndef SDK_THREAD_INFINITY
    RequestFree(NULL);
#endif
}

//--------------------------------------------------------------------------------
//    proc3
//
void proc3(void *)
{
    OSMessage message = (OSMessage)OS_GetCurrentThread();
    int     i;
    for (i = 0; i < 64; ++i)
    {
        OS_SleepThread(&g_threadQueue);
    }
    if (!g_bExit)
    {
        (void)SpawnThread(proc1, 16);
        (void)SpawnThread(proc2, 16);
    }
#ifndef SDK_THREAD_INFINITY
    RequestFree(NULL);
#endif
}

/*====== End of main.c ======*/
