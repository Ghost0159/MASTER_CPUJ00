/*---------------------------------------------------------------------------*
  Project:  NitroSDK - libraries - OS
  File:     os_irqHandler.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: os_irqHandler.c,v $
  Revision 1.54  2007/01/22 00:04:44  okubata_ryoma
  Copyright fix

  Revision 1.53  2007/01/19 08:15:25  yada
  modified that the register R1 was changed illegally in leaving from
  the irq handler.

  Revision 1.52  2006/08/09 06:53:13  takano_makoto
  delete unused macro SDK_CP_NO_SAFE
  change CP_RestoreContext to CPi_RestoreContext

  Revision 1.51  2006/04/10 06:04:01  yosizaki
  rollback to 1.49

  Revision 1.50  2006/04/04 08:36:03  yosizaki
  �R�[���X�^�b�N��8�o�C�g�����Ɋւ���C��.

  Revision 1.49  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.48  2005/06/28 09:49:19  yada
  applied for SDK_THREAD_INFINITY

  Revision 1.47  2005/06/28 04:47:24  yosizaki
  correct the code committed on Revision 1.46.

  Revision 1.46  2005/06/28 02:57:28  yosizaki
  avoid cpu bug. (add writeback-wait)

  Revision 1.45  2005/02/28 05:26:28  yosizaki
  do-indent.

  Revision 1.44  2004/11/02 10:25:09  yada
  just fix comment

  Revision 1.43  2004/07/10 06:11:24  yasu
  Support to select a section 'ITCM/DTCM/WRAM'

  Revision 1.42  2004/07/02 08:46:00  yada
  section name Dtcm->DTCM, Itcm->ITCM

  Revision 1.41  2004/07/01 12:57:47  yada
  send IrqHandler() into .itcm and .dtcm section
  arrange function description

  Revision 1.40  2004/06/15 12:35:05  yada
  consider for saving/loading CPContext in switching thread in IrqHandler

  Revision 1.39  2004/05/25 10:44:58  yada
  when IE&IF==0 in irqHandler, leave without IF=0

  Revision 1.38  2004/05/24 04:48:29  yada
  OS_WaitIntr() -> OS_WaitInterrupt()

  Revision 1.37  2004/05/20 04:29:56  yada
  consider for SDK_NO_THREAD

  Revision 1.36  2004/05/18 12:16:09  yasu
  re-support OSi_IrqThreadQueue in irqHandler

  Revision 1.35  2004/05/18 11:59:44  yasu
  add IRQ level counter 'irqDepth' in OSThreadInfo

  Revision 1.34  2004/05/17 06:47:35  yada
  in OS_IrqHandler(), if IME=0 then return.

  Revision 1.33  2004/05/10 12:37:12  yada
  forbid interrupt while clearing checkflag

  Revision 1.32  2004/05/10 11:24:52  yada
  SVC_WaitIntr() -> OS_WaitIntr()

  Revision 1.31  2004/05/06 11:14:48  takano_makoto
  fix a bug when SDK_NO_THREAD defined.

  Revision 1.30  2004/05/01 05:47:31  yada
  OS_WaitInterrupt->OS_WaitIrq, OS_WaitAnyInterrupt->OS_WaitAnyIrq

  Revision 1.29  2004/04/30 08:11:06  yada
  add OS_WaitAnyInterrupt

  Revision 1.28  2004/04/30 02:19:17  yada
  add OS_WaitInterrupt().
  change irqHandler to see irqThreadQueue

  Revision 1.27  2004/04/15 12:50:23  yada
  divide OS_IrqHandler to 2 function

  Revision 1.26  2004/04/14 04:13:31  yasu
  Fix irqHandler when IE & IF == 0 && NO_THREAD

  Revision 1.25  2004/04/08 02:36:40  yada
  �����݃n���h�����ŁAIE&IF==0�̂Ƃ���IF=0�Ƃ��Ĕ�����悤�ɂ����B

  Revision 1.24  2004/03/31 09:25:14  yada
  add thread switch callback function

  Revision 1.23  2004/03/30 06:09:29  yada
  ldconst �� lda �� ldr = ���g�p����L�q�ɂ����B

  Revision 1.22  2004/02/26 09:44:12  yada
  �X���b�h���A�D��x���X�g�Ǘ�����悤�ɕύX�B
  ����ɔ����C��

  Revision 1.21  2004/02/16 00:17:55  yada
  only add comment

  Revision 1.20  2004/02/13 01:15:32  yada
  SPOS->OS

  Revision 1.19  2004/02/12 13:38:42  yasu
  change directory of ioreg_OS.h

  Revision 1.18  2004/02/12 11:08:30  yada
  ARM9/ARM7 �U�蕪�����

  Revision 1.17  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.16  2004/01/18 02:25:37  yada
  �C���f���g���̐��`

  Revision 1.15  2003/12/25 07:31:26  yada
  �^���[������ɂ��ύX

  Revision 1.14  2003/12/22 13:38:56  yasu
  ARCH5 �R�[�h������

  Revision 1.13  2003/12/17 08:20:20  yasu
  ARM7 �Ƃ̋��p�R�[�h��

  Revision 1.12  2003/12/17 06:27:44  yasu
  ARCH4 �p�R�[�h�̒ǉ�

  Revision 1.11  2003/12/16 06:42:30  yasu
  ISDebugger �T�|�[�g�̂��߁A�X���b�h�؂�ւ����� FIQ ���֎~����

  Revision 1.10  2003/12/16 06:01:03  yasu
  IRQ �x�N�^�v�Z�����ύX

  Revision 1.9  2003/12/16 00:17:22  yada
  thread�����܂Ƃ߂����Ƃɂ��ύX

  Revision 1.8  2003/12/12 04:40:19  yada
  �v���Z�b�T���[�h�̒�`�ύX�ɔ����C��

  Revision 1.7  2003/12/08 12:16:45  yada
  �C���N���[�h����t�@�C����MemoryMap.h���� mmap_global.h�ɕύX

  Revision 1.6  2003/12/08 10:11:00  yasu
  �s�v�� define �̍폜

  Revision 1.5  2003/12/08 01:25:05  yasu
  context �� SP_svc ��ǉ�����

  Revision 1.4  2003/12/01 09:51:04  yasu
  context �ɃZ�[�u����l�� PC->PC+4 ��

  Revision 1.3  2003/11/30 05:10:42  yasu
  thread �֌W�̊֐������荞�݃T�[�r�X����Ăяo���\�ɂ����D

  Revision 1.2  2003/11/29 04:36:30  yasu
  ���̏C��

  Revision 1.1  2003/11/29 03:58:56  yasu
  �t�@�C�����̕ύX osInterruptBase.c -> os_irqhandler.c

  Revision 1.7  2003/11/28 12:01:57  yasu
  OSi_IRQHandler_Begin_Label/End_Label ��ǉ�

  Revision 1.6  2003/11/27 13:39:14  yasu
  �W�����v�e�[�u���̃o�O�̏C��

  Revision 1.5  2003/11/27 10:14:18  yasu
  ICE �֌W�̑΍�� SDK_BB -> SDK_BB_ICE �ɕύX

  Revision 1.4  2003/11/27 07:34:55  yasu
  ���荞�ݏI����� Context Switch �ɑΉ�
  BreadBoad �ł� VBlank �� 30Hz ���ɑΉ�

  Revision 1.2  2003/11/10 02:33:19  yasu
  interrupt table �̒ǉ�

  Revision 1.1  2003/11/06 13:39:01  yasu
  osInterruptBase.s ���C�����C���A�Z���u���� .c �ɕύX
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include        <nitro/code32.h>
#include        <nitro/types.h>
#include        <nitro/os/common/interrupt.h>
#include        <nitro/os/common/thread.h>
#include        <nitro/os/common/systemCall.h>

#ifdef SDK_ARM9
#include        <nitro/hw/ARM9/mmap_global.h>
#include        <nitro/hw/ARM9/ioreg_OS.h>
#else  // SDK_ARM9
#include        <nitro/hw/ARM7/mmap_global.h>
#include        <nitro/hw/ARM7/ioreg_OS.h>
#endif // SDK_ARM9


#ifdef      SDK_ARM9
#include    <nitro/dtcm_begin.h>
#endif

//---- thread queue for interrupt
#ifndef SDK_THREAD_INFINITY
OSThreadQueue OSi_IrqThreadQueue = 0;
#else
OSThreadQueue OSi_IrqThreadQueue = { NULL, NULL };
#endif

#ifdef      SDK_ARM9
#include    <nitro/dtcm_end.h>
#include    <nitro/itcm_begin.h>
#endif

/*---------------------------------------------------------------------------*
  Name:         OS_IrqHandler

  Description:  IRQ handler. call handler according to OS_InterruptTable

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
asm void OS_IrqHandler( void )
{
#ifdef  SDK_NO_THREAD
#else
        stmfd   sp!, { lr }                     // save LR
#endif
        // get IE address
        mov     r12,      #HW_REG_BASE
        add     r12, r12, #REG_IE_OFFSET        // r12: REG_IE address

        // get IME
        ldr     r1, [ r12, #REG_IME_ADDR - REG_IE_ADDR ]  // r1: IME

        // if IME==0 then return (without changing IF)
        cmp     r1, #0
#ifdef  SDK_NO_THREAD
        bxeq    lr
#else
        ldmeqfd sp!, { pc }
#endif

        // get IE&IF
        ldmia   r12, { r1-r2 }                  // r1: IE, r2: IF
        ands    r1, r1, r2                      // r1: IE & IF

        // if IE&IF==0 then return (without changing IF)
#ifdef  SDK_NO_THREAD
        bxeq    lr
#else
        ldmeqfd sp!, { pc }
#endif


#if     defined(SDK_ARM9) && !defined(SDK_CWBUG_PROC_OPT)
        //--------------------------------------------------
        // IRQ HANDLING CODE for ARCHITECTURE VERSION 5
        //--------------------------------------------------
        
        // get lowest 1 bit
        mov     r3, #1<<31
@1:     clz     r0, r1                  // count zero of high bit
        bics    r1, r1, r3, LSR r0
        bne     @1

        // clear IF
        mov     r1, r3, LSR r0
        str     r1, [ r12, #REG_IF_ADDR - REG_IE_ADDR ]

        rsbs    r0, r0, #31

#else //defined(SDK_ARM9) && !defined(SDK_CWBUG_PROC_OPT)
        //--------------------------------------------------
        // IRQ HANDLING CODE for ARCHITECTURE VERSION 4
        //--------------------------------------------------
        mov     r3, #1
        mov     r0, #0
@1:     ands    r2, r1, r3, LSL r0              // count zero of high bit
        addeq   r0, r0, #1
        beq     @1

        // clear IF
        str     r2, [ r12, #REG_IF_ADDR - REG_IE_ADDR ]
#endif //defined(SDK_ARM9) && !defined(SDK_CWBUG_PROC_OPT)

        // get jump vector
#ifdef  SDK_DEBUG
        cmp     r0, #OS_IRQ_TABLE_MAX
@2:     bge     @2                              // Error Trap
#endif//SDK_DEBUG
        ldr     r1, =OS_IRQTable
        ldr     r0, [ r1, r0, LSL #2 ]
        
#ifdef  SDK_NO_THREAD
        bx      r0
#else //SDK_NO_THREAD
        ldr     lr, =OS_IrqHandler_ThreadSwitch
        bx      r0      // set return address for thread rescheduling
#endif//SDK_NO_THREAD
}



/*---------------------------------------------------------------------------*
  Name:         OS_IRQHandler_ThreadSwitch

  Description:  ���荞�ݕ��򏈗��i�e�[�u�� OS_InterruptTable �����j

  Arguments:    �Ȃ�

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
asm void OS_IrqHandler_ThreadSwitch(void)
{
#ifdef  SDK_NO_THREAD
#else
#ifndef SDK_THREAD_INFINITY //------------
        //--------------------------------------------------
        // wakeup threads in OSi_IrqThreadQueue
        //--------------------------------------------------
        ldr     r12, =OSi_IrqThreadQueue
#if     ( OS_THREAD_MAX_NUM <= 16 )
        ldrh    r3, [r12]                       // r3 = OSi_IrqThreadQueue
        mov     r0, #0
        cmp     r3, #0
        beq     @thread_switch                  // if r3 == 0 exit
        strh    r0, [r12]                       // OSi_IrqThreadQueue = 0
#else   //OS_THREAD_MAX_NUM
        ldr     r3, [r12]
        mov     r0, #0
        cmp     r3, #0
        beq     @thread_switch
        str     r0, [r12]
#endif  //OS_THREAD_MAX_NUM

        ldr     r12, =OSi_ThreadInfo    // isNeedRescheduling=OS_THREADINFO_RESCHEDULING_DISABLE_LATER
        mov     r1,  #1
        strh    r1,  [ r12, #OS_THREADINFO_OFFSET_ISNEEDRESCHEDULING ]
        ldr     r12, [ r12, #OS_THREADINFO_OFFSET_LIST ]    // r12 = OSi_ThreadInfo.list
        mov     r2,  #OS_THREAD_STATE_READY
@1:
        cmp     r12, #0
        beq     @thread_switch
        ldr     r0,  [r12, #OS_THREAD_OFFSET_ID]
        tst     r3,  r1, LSL r0                      // OSi_IrqThreadQueue & (1<<thread->id)
        strne   r2,  [r12, #OS_THREAD_OFFSET_STATE]
        ldr     r12, [r12, #OS_THREAD_OFFSET_NEXT]
        b       @1

#else //ifndef SDK_THREAD_INFINITY ------------
        //--------------------------------------------------
        // wakeup threads in OSi_IrqThreadQueue
        //--------------------------------------------------
           ldr             r12, =OSi_IrqThreadQueue
           mov             r3,  #0                      // avoid stall
           ldr             r12, [r12, #OSThreadQueue.head] // r12 = OSi_IrqThreadQueue.head
           mov             r2,  #OS_THREAD_STATE_READY  // avoid stall
           cmp             r12, #0

           beq             @thread_switch                                  // if r12 == 0 exit

@1:        str             r2,  [r12, #OSThread.state]
           str             r3,  [r12, #OSThread.queue]
           str             r3,  [r12, #OSThread.link.prev]
           ldr             r0,  [r12, #OSThread.link.next]
           str             r3,  [r12, #OSThread.link.next]
           mov             r12,  r0

           cmp             r12, #0
           bne             @1

           ldr             r12, =OSi_IrqThreadQueue
           str             r3, [r12, #OSThreadQueue.head]  // clear OSi_IrqThreadQueue.head
           str             r3, [r12, #OSThreadQueue.tail]  // clear OSi_IrqThreadQueue.tail

           ldr             r12, =OSi_ThreadInfo                    // need to do scheduling
           mov             r1, #1
           strh            r1, [ r12, #OS_THREADINFO_OFFSET_ISNEEDRESCHEDULING ]

#endif //ifndef SDK_THREAD_INFINITY ------------

@thread_switch:
        //--------------------------------------------------
        // THREAD SWITCH
        //--------------------------------------------------
        // pseudo code
        //
        // if ( isNeedRescheduling == FALSE ) return;
        // isNeedRescheduling = FALSE;
        //
        // // OS_SelectThread
        // OSThread* t = OSi_ThreadInfo.list;
        // while( t && ! OS_IsThreadRunnable( t ) ){ t = t->next; }
        // return t;
        //
        // select:
        // current = CurrentThread;
        // if ( next == current ) return;
        // CurrentThread = next;
        // OS_SaveContext( current );
        // OS_LoadContext( next );
        // 
        
        // [[[ new OS_SelectThread ]]]
        ldr     r12, =OSi_ThreadInfo
        ldrh    r1, [ r12, #OS_THREADINFO_OFFSET_ISNEEDRESCHEDULING ]
        cmp     r1, #0
        ldreq   pc, [ sp ], #4          // return if OSi_IsNeedResceduling == 0
        
        mov     r1, #0
        strh    r1, [ r12, #OS_THREADINFO_OFFSET_ISNEEDRESCHEDULING ]

        // ---- OS_SelectThread (disable FIQ to support IS-Debugger snooping thread information)
        mov     r3, #HW_PSR_IRQ_MODE|HW_PSR_FIQ_DISABLE|HW_PSR_IRQ_DISABLE|HW_PSR_ARM_STATE
        msr     cpsr_c, r3

        add     r2, r12, #OS_THREADINFO_OFFSET_LIST // r2 = &OSi_ThreadInfo.list
        ldr     r1, [r2]                            // r1 = *r2 = TopOfList
@11:
        cmp     r1, #0
        ldrneh  r0, [ r1, #OS_THREAD_OFFSET_STATE ] // r0 = t->state
        cmpne   r0, #OS_THREAD_STATE_READY          
        ldrne   r1, [ r1, #OS_THREAD_OFFSET_NEXT ]
        bne     @11

        cmp     r1, #0
        bne     @12

_dont_switched_:
        mov     r3, #HW_PSR_IRQ_MODE|HW_PSR_IRQ_DISABLE|HW_PSR_ARM_STATE
        msr     cpsr_c, r3

        ldr     pc, [ sp ], #4          // return to irq master handler
        // not reach here


@12:
        // ---- OS_GetCurrentThread
        ldr     r0, [ r12, #OS_THREADINFO_OFFSET_CURRENT ]
        cmp     r1, r0
        beq     _dont_switched_         // return if no thread switching

        // call thread switch callback (need to save register r0, r1, r12)
        ldr     r3, [ r12, #OS_THREADINFO_OFFSET_SWITCHCALLBACK ]
        cmp     r3, #0
        beq     @13                     // skip calling callback when callback == 0
        stmfd   sp!, { r0, r1, r12 }
        mov     lr, pc
        bx      r3
        ldmfd   sp!, { r0, r1, r12 }

@13:
        // ---- OS_SetCurrentThread
        str     r1, [ r12, #OS_THREADINFO_OFFSET_CURRENT ]
        
        // ---- OS_SaveContext 
        // r0=currentThread  r1=nextThread
        // stack=Lo[LR,R0,R1,R2,R3,R12,LR]Hi
        mrs     r2, SPSR
        str     r2, [ r0, #OS_THREAD_OFFSET_CONTEXT ]!  // *r0=context:CPSR
        
#if defined(SDK_ARM9)
        // first, save CP context
        stmfd   sp!, { r0, r1 }
        add     r0, r0, #OS_THREAD_OFFSET_CONTEXT
        add     r0, r0, #OS_CONTEXT_CP_CONTEXT
        ldr     r1, =CP_SaveContext
        blx     r1
        ldmfd   sp!, { r0, r1 }
#endif

        ldmib   sp!, { r2,r3 }          // Get R0,R1    // *sp=stack:R1
        stmib   r0!, { r2,r3 }          // Put R0,R1    // *r0=context:R1
        
        ldmib   sp!, { r2,r3,r12,r14 }  // Get R2,R3,R12,LR / *sp=stack:LR
        stmib   r0!, { r2-r14        }^ // Put R2-R14^  // *r0=context:R14
        stmib   r0!, { r14           }  // Put R14_irq  // *r0=context:R15+4
#ifdef  SDK_CONTEXT_HAS_SP_SVC
        mov     r3, #HW_PSR_SVC_MODE|HW_PSR_FIQ_DISABLE|HW_PSR_IRQ_DISABLE|HW_PSR_ARM_STATE
        msr     cpsr_c, r3
        stmib   r0!, { sp }
#endif

        // ---- OS_LoadContext
#if defined(SDK_ARM9)
        // first, load CP context
        stmfd   sp!, { r1 }
        add     r0, r1, #OS_THREAD_OFFSET_CONTEXT
        add     r0, r0, #OS_CONTEXT_CP_CONTEXT
        ldr     r1, =CPi_RestoreContext
        blx     r1
        
        ldmfd   sp!, { r1 }

#endif // if defined(SDK_ARM9)


#ifdef  SDK_CONTEXT_HAS_SP_SVC
        ldr     sp, [ r1, #OS_THREAD_OFFSET_CONTEXT+OS_CONTEXT_SP_SVC ]
        mov     r3, #HW_PSR_IRQ_MODE|HW_PSR_FIQ_DISABLE|HW_PSR_IRQ_DISABLE|HW_PSR_ARM_STATE
        msr     cpsr_c, r3
#endif

#if defined(SDK_ARM9)
#if 0 // don't need, because it spent more than 68 cycle for divider already.
    //---- CP_WaitDiv
        ldr     r2, =REG_DIVCNT_ADDR
@_cp_wait_in_02:
        ldr     r3, [ r2 ]
        and     r3, r3, #REG_CP_DIVCNT_BUSY_MASK
        bne     @_cp_wait_in_02
#endif
#endif
        ldr     r2, [ r1, #OS_THREAD_OFFSET_CONTEXT ]!  // *r1=context:CPSR
        msr     SPSR, r2                                // Put SPSR
        
        ldr     r14, [ r1, #OS_CONTEXT_PC_PLUS4 - OS_CONTEXT_CPSR ]   // Get R15
        ldmib   r1, { r0-r14 }^         // Get R0-R14^  // *r1=over written
        nop
        stmda   sp!, { r0-r3,r12,r14 }  // Put R0-R3,R12,LR / *sp=stack:LR
        
        ldmfd   sp!, { pc }             // return to irq master handler
#endif
}

#ifdef      SDK_ARM9
#include    <nitro/itcm_end.h>
#endif

/*---------------------------------------------------------------------------*
  Name:         OS_WaitIrq

  Description:  wait specified interrupt
                the difference between OS_WaitIrq and OS_WaitInterrupt,
                in waiting interrupt
                OS_WaitIrq does switch thread,
                OS_WaitInterrupt doesn't switch thread.
                OS_WaitIrq wait by using OS_SleepThread() with threadQueue,
                OS_WaitInterrupt wait by using OS_Halt().
                if SDK_NO_THREAD defined, 2 functions become same.
  
  Arguments:    clear       TRUE if want to clear interrupt flag before wait
                            FALSE if not
                irqFlags    bit of interrupts to wait for

  Returns:      None
 *---------------------------------------------------------------------------*/
void OS_WaitIrq(BOOL clear, OSIrqMask irqFlags)
{
#ifdef SDK_NO_THREAD
    OS_WaitInterrupt(clear, irqFlags);

#else
    OSIntrMode enabled = OS_DisableInterrupts();

    //---- clear interrupt check flags (if needed)
    if (clear)
    {
        (void)OS_ClearIrqCheckFlag(irqFlags);
    }

    (void)OS_RestoreInterrupts(enabled);

    //---- sleep till requied interrupts
    while (!(OS_GetIrqCheckFlag() & irqFlags))
    {
        OS_SleepThread(&OSi_IrqThreadQueue);
    }
#endif // ifdef SDK_NO_THREAD
}

/*---------------------------------------------------------------------------*
  Name:         OS_WaitAnyIrq

  Description:  wait any interrupt
  
  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void OS_WaitAnyIrq(void)
{
#ifdef SDK_NO_THREAD
    OS_Halt();
#else
    OS_SleepThread(&OSi_IrqThreadQueue);
#endif
}
