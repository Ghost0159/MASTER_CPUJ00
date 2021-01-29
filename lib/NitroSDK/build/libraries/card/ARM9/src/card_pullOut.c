/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CARD - libraries
  File:     card_pullOut.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: card_pullOut.c,v $
  Revision 1.16  2007/02/20 00:28:12  kitase_hirotake
  indent source

  Revision 1.15  2006/09/22 02:29:04  okubata_ryoma
  �J�[�h������������DMA�]�����~�����鏈����ǉ�

  Revision 1.14  2006/08/10 06:04:23  okubata_ryoma
  fix CARD_TerminateForPulledOut.

  Revision 1.13  2006/07/24 06:42:06  okubata_ryoma
  small fix

  Revision 1.12  2006/07/21 12:48:02  okubata_ryoma
  �J�[�h�������ɓd����OFF�ɂ���Ȃ��s��C���̂��߂̕ύX

  Revision 1.11  2006/02/07 08:19:36  kitase_hirotake
  CARDi_PulledOutCallback ���� CARD_UserCallback �Ăяo�������ύX

  Revision 1.10  2006/01/18 02:11:19  kitase_hirotake
  do-indent

  Revision 1.9  2005/10/24 10:49:34  yosizaki
  add CARDi_CheckPulledOutCore().

  Revision 1.8  2005/06/01 07:09:40  yosizaki
  add CARD_CheckPulledOut().

  Revision 1.7  2005/02/28 05:26:02  yosizaki
  do-indent.

  Revision 1.6  2005/02/18 11:20:17  yosizaki
  fix around hidden warnings.

  Revision 1.5  2004/11/10 06:39:20  yada
  When card pulled out, check to fold cover, and if true then power off.

  Revision 1.4  2004/09/28 05:06:54  yada
  only fix comment

  Revision 1.3  2004/09/27 13:42:46  yada
  change polling for card remove

  Revision 1.2  2004/09/16 04:05:45  yada
  fix a little

  Revision 1.1  2004/09/15 05:48:49  yada
  first release

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#include <card_rom.h>

//---- user callback for card pulled out
static CARDPulledOutCallback CARD_UserCallback;

//---- flag to be pulled out
static BOOL CARDi_IsPulledOutFlag = FALSE;

static void CARDi_PulledOutCallback(PXIFifoTag tag, u32 data, BOOL err);
static void CARDi_SendtoPxi(u32 data, u32 wait);

/*---------------------------------------------------------------------------*
  Name:         CARD_InitPulledOutCallback

  Description:  initialize callback setting

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void CARD_InitPulledOutCallback(void)
{
    PXI_Init();

    //---- set PXI callback
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CARD, CARDi_PulledOutCallback);

    //---- init user callback
    CARD_UserCallback = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_PulledOutCallback

  Description:  callback to receive data from PXI

  Arguments:    tag  : tag from PXI (unused)
                data : data from PXI
                err  : error bit (unused)

  Returns:      None
 *---------------------------------------------------------------------------*/
static void CARDi_PulledOutCallback(PXIFifoTag tag, u32 data, BOOL err)
{
#pragma unused( tag, err )

    u32     command = data & CARD_PXI_COMMAND_MASK;

    //---- receive message 'pulled out'
    if (command == CARD_PXI_COMMAND_PULLED_OUT)
    {
        if (CARDi_IsPulledOutFlag == FALSE)
        {
            BOOL    isTerminateImm = TRUE;

            CARDi_IsPulledOutFlag = TRUE;

            //---- call user callback
            if (CARD_UserCallback)
            {
                isTerminateImm = CARD_UserCallback();
            }

            //---- terminate
            if (isTerminateImm)
            {
                CARD_TerminateForPulledOut();
            }
        }
    }
    else
    {
#ifndef SDK_FINALROM
        OS_Panic("illegal card pxi command.");
#else
        OS_Panic("");
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_SetPulledOutCallback

  Description:  set user callback for being pulled out card

  Arguments:    callback : callback

  Returns:      None
 *---------------------------------------------------------------------------*/
void CARD_SetPulledOutCallback(CARDPulledOutCallback callback)
{
    CARD_UserCallback = callback;
}

/*---------------------------------------------------------------------------*
  Name:         CARD_IsPulledOut

  Description:  return if have detected card pulled out

  Arguments:    None

  Returns:      TRUE if detected
 *---------------------------------------------------------------------------*/
BOOL CARD_IsPulledOut(void)
{
    return CARDi_IsPulledOutFlag;
}

//================================================================================
//         TERMINATION
//================================================================================
/*---------------------------------------------------------------------------*
  Name:         CARD_TerminateForPulledOut

  Description:  terminate for pulling out card
                send message to do termination to ARM7

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void CARD_TerminateForPulledOut(void)
{
#ifndef SDK_TEG
    BOOL    should_be_halt = TRUE;

    //---- stop all dma
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);

    //---- if folding, power off
    if (PAD_DetectFold())
    {
        /* ���C�����X�ɂ��SPI�r���Ŏ��s�������Ď��s���� */
        u32     res;
        while ((res = PM_ForceToPowerOff()) == SPI_PXI_RESULT_EXCLUSIVE)
        {
            /* 10ms�҂��čĎ��s */
            OS_SpinWait(HW_CPU_CLOCK_ARM9 / 100);
        }
        if (res == PM_RESULT_SUCCESS)
        {
            // ARM7���Z�����ꍇ�A�d��OFF���Terminate�������ɍs���Ă��܂��d�����؂�Ȃ��\��������̂�
            // PM_ForceToPowerOff�֐������������ꍇ��ARM7����Terminate�R�}���h�͑���Ȃ��B
            should_be_halt = FALSE;
        }
    }

    // �W�������Ă��Ȃ��Ƃ��A�܂���PM_ForceToPowerOff�֐������s�����ꍇ��ARM7���~�߂Ȃ��Ƃ����Ȃ��̂�Terminate�R�}���h�𑗂�B
    if (should_be_halt)
    {
        //---- send 'TERMINATE' command to ARM7, and terminate itself immediately
        CARDi_SendtoPxi(CARD_PXI_COMMAND_TERMINATE, 1);
    }
#endif

    OS_Terminate();
}

/*---------------------------------------------------------------------------*
  Name:         CARDi_CheckPulledOutCore

  Description:  �J�[�h�������o�֐��̃��C������.
                �J�[�h�o�X�̓��b�N����Ă���K�v������.

  Arguments:    id            �J�[�h����ǂݏo���ꂽ ROM-ID

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CARDi_CheckPulledOutCore(u32 id)
{
    //---- card ID IPL had read
    vu32    iplCardID = *(vu32 *)((*(u16 *)HW_CHECK_DEBUGGER_SW ==
                                   0) ? HW_RED_RESERVED : HW_BOOT_CHECK_INFO_BUF);
    //---- if card removal has been detected, we simulate PXI-notification from ARM7
    if (id != (u32)iplCardID)
    {
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        CARDi_PulledOutCallback(PXI_FIFO_TAG_CARD, CARD_PXI_COMMAND_PULLED_OUT, FALSE);
        (void)OS_RestoreInterrupts(bak_cpsr);
    }
}

/*---------------------------------------------------------------------------*
  Name:         CARD_CheckPulledOut

  Description:  get whether system has detected pulled out card
                by comparing IPL cardID with current cardID
                (notice that once card pulled out, IDs are different absolutely)

  Arguments:    None

  Returns:      TRUE if current cardID is equal to IPL cardID
 *---------------------------------------------------------------------------*/
void CARD_CheckPulledOut(void)
{
    CARDi_CheckPulledOutCore(CARDi_ReadRomID());
}

//================================================================================
//         SEND PXI COMMAND
//================================================================================
/*---------------------------------------------------------------------------*
  Name:         CARDi_SendtoPxi

  Description:  send data via PXI

  Arguments:    data : data to send

  Returns:      None
 *---------------------------------------------------------------------------*/
static void CARDi_SendtoPxi(u32 data, u32 wait)
{
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_CARD, data, FALSE) != PXI_FIFO_SUCCESS)
    {
        SVC_WaitByLoop((s32)wait);
    }
}
