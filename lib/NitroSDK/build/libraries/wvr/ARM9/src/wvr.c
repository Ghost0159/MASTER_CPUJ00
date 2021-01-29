/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WVR - libraries
  File:     wvr.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wvr.c,v $
  Revision 1.9  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.8  2005/05/09 08:30:20  terui
  Fix comment.
  �����I����������PXI�R�[���o�b�N���NULL�N���A����悤�C��

  Revision 1.7  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.6  2005/02/17 00:01:51  terui
  PXI��M�R�[���o�b�N�̐ݒ�͏���݂̂Ɍ��肷��悤�C���B

  Revision 1.5  2005/02/16 10:36:18  terui
  �C���f���g�C���̂݁B

  Revision 1.4  2005/02/09 09:21:47  terui
  VRAM�o���N�̔r���@�\��ǉ��B

  Revision 1.3  2005/01/11 07:46:42  takano_makoto
  fix copyright header.

  Revision 1.2  2005/01/05 07:30:37  terui
  wvr���C�u������include�p�X�𖾋L����悤�C���B

  Revision 1.1  2004/12/20 00:52:43  terui
  �b��łł͂��邪�A����͂����ԂŐV�K�o�^�B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/wvr/common/wvr_common.h>
#include    <nitro/wvr/ARM9/wvr.h>
#include    <nitro/pxi.h>
#include    <nitro/os/ARM9/vramExclusive.h>
#include    <nitro/gx/gx_vramcnt.h>
#include    <nitro/wm.h>


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void WvrReceiveCallback(PXIFifoTag tag, u32 data, BOOL err);
static void WvrDummyAsyncCallback(void *arg, WVRResult result);


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static WVRCallbackFunc wvrCallback = NULL;
static void *wvrArg = NULL;
static vu16 wvrVRam = 0;
static vu16 wvrLockId = 0;


/*---------------------------------------------------------------------------*
  Name:         WVR_StartUpAsync

  Description:  �������C�u�������쓮�J�n����B
                ������~����܂ŁA�w�肵��VRAM( C or D )�ւ̃A�N�Z�X�͋֎~�ɂȂ�B

  Arguments:    vram        -   ARM7�Ɋ��蓖�Ă�VRAM�o���N���w��B
                callback    -   �����������̃R�[���o�b�N�֐����w��B
                arg         -   �R�[���o�b�N�֐��ɓn���������w��B

  Returns:      �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WVRResult WVR_StartUpAsync(GXVRamARM7 vram, WVRCallbackFunc callback, void *arg)
{
    OSIntrMode e;

    // �R���|�[�l���g��WVR�ɑΉ����Ă��邩�m�F
    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM7))
    {
        return WVR_RESULT_DISABLE;
    }

    // �r���pID���擾���Ă��Ȃ���Ύ擾
    while (wvrLockId == 0)
    {
        s32     result = OS_GetLockID();

        if (result == OS_LOCK_ID_ERROR)
        {
            return WVR_RESULT_FATAL_ERROR;
        }
        wvrLockId = (u16)result;
    }

    // �����݋֎~
    e = OS_DisableInterrupts();

    // WVR�񓯊��֐������쒆�łȂ����m�F
    if (wvrCallback != NULL)
    {
        (void)OS_RestoreInterrupts(e);
        return WVR_RESULT_ILLEGAL_STATUS;
    }

    // VRAM�r����Ԃ��`�F�b�N
    if (wvrVRam)
    {
        // ���łɔr���ς�
        (void)OS_RestoreInterrupts(e);
        return WVR_RESULT_ILLEGAL_STATUS;
    }
    switch (vram)
    {
    case GX_VRAM_ARM7_128_C:
        // VRAM-C ��r�����b�N
        if (!OSi_TryLockVram(OS_VRAM_BANK_ID_C, wvrLockId))
        {
            // VRAM-C �̃��b�N�Ɏ��s
            (void)OS_RestoreInterrupts(e);
            return WVR_RESULT_VRAM_LOCKED;
        }
        wvrVRam = OS_VRAM_BANK_ID_C;
        // GX_SetBankForARM7( GX_VRAM_ARM7_128_C ) �ɑ������郌�W�X�^�ݒ菈��
        reg_GX_VRAMCNT_C = ((2 << REG_GX_VRAMCNT_C_MST_SHIFT) |
                            (0 << REG_GX_VRAMCNT_C_OFS_SHIFT) | (1 << REG_GX_VRAMCNT_C_E_SHIFT));
        break;
    case GX_VRAM_ARM7_128_D:
        // VRAM-D ��r�����b�N
        if (!OSi_TryLockVram(OS_VRAM_BANK_ID_D, wvrLockId))
        {
            // VRAM-D �̃��b�N�Ɏ��s
            (void)OS_RestoreInterrupts(e);
            return WVR_RESULT_VRAM_LOCKED;
        }
        wvrVRam = OS_VRAM_BANK_ID_D;
        // GX_SetBankForARM7( GX_VRAM_ARM7_128_D ) �ɑ������郌�W�X�^�ݒ菈��
        reg_GX_VRAMCNT_D = ((2 << REG_GX_VRAMCNT_D_MST_SHIFT) |
                            (0 << REG_GX_VRAMCNT_D_OFS_SHIFT) | (1 << REG_GX_VRAMCNT_D_E_SHIFT));
        break;
    case GX_VRAM_ARM7_256_CD:
        // VRAM-C/D ��r�����b�N
        if (!OSi_TryLockVram((OS_VRAM_BANK_ID_C | OS_VRAM_BANK_ID_D), wvrLockId))
        {
            // VRAM-C/D �̃��b�N�Ɏ��s
            (void)OS_RestoreInterrupts(e);
            return WVR_RESULT_VRAM_LOCKED;
        }
        wvrVRam = OS_VRAM_BANK_ID_C | OS_VRAM_BANK_ID_D;
        // GX_SetBankForARM7( GX_VRAM_ARM7_256_CD ) �ɑ������郌�W�X�^�ݒ菈��
        reg_GX_VRAMCNT_C = ((2 << REG_GX_VRAMCNT_C_MST_SHIFT) |
                            (0 << REG_GX_VRAMCNT_C_OFS_SHIFT) | (1 << REG_GX_VRAMCNT_C_E_SHIFT));
        reg_GX_VRAMCNT_D = ((2 << REG_GX_VRAMCNT_D_MST_SHIFT) |
                            (1 << REG_GX_VRAMCNT_D_OFS_SHIFT) | (1 << REG_GX_VRAMCNT_D_E_SHIFT));
        break;
    default:
        (void)OS_RestoreInterrupts(e);
        return WVR_RESULT_INVALID_PARAM;
    }

    // PXI�R�[���o�b�N�֐���ݒ�
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM9))
    {
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, WvrReceiveCallback);
    }

    // �R�[���o�b�N�֐���ޔ�
    if (callback == NULL)
    {
        wvrCallback = WvrDummyAsyncCallback;
    }
    else
    {
        wvrCallback = callback;
    }
    wvrArg = arg;

    // PXI�o�R��StartUp��ʒm
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, WVR_PXI_COMMAND_STARTUP, FALSE))
    {
        // ��U�r�����b�N���� VRAM ��r������
        OSi_UnlockVram(wvrVRam, wvrLockId);
        wvrVRam = 0;
        wvrCallback = NULL;
        (void)OS_RestoreInterrupts(e);
        return WVR_RESULT_FIFO_ERROR;
    }

    // �񓯊������𐳏�ɊJ�n����
    (void)OS_RestoreInterrupts(e);
    return WVR_RESULT_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WVR_TerminateAsync

  Description:  �������C�u������������~����B
                �񓯊�����������AVRAM( C or D )�ւ̃A�N�Z�X�͋������B

  Arguments:    callback    -   �����������̃R�[���o�b�N�֐����w��B
                arg         -   �R�[���o�b�N�֐��ɓn���������w��B

  Returns:      �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WVRResult WVR_TerminateAsync(WVRCallbackFunc callback, void *arg)
{
    OSIntrMode e;

    // �R���|�[�l���g�̑Ή����m�F
    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM7))
    {
        return WVR_RESULT_DISABLE;
    }

    // �����݋֎~
    e = OS_DisableInterrupts();

    // WVR�񓯊��֐������쒆�łȂ����m�F
    if (wvrCallback != NULL)
    {
        (void)OS_RestoreInterrupts(e);
        return WVR_RESULT_ILLEGAL_STATUS;
    }

    // PXI�R�[���o�b�N�֐���ݒ�
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM9))
    {
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, WvrReceiveCallback);
    }

    // �R�[���o�b�N�֐���ޔ�
    if (callback == NULL)
    {
        wvrCallback = WvrDummyAsyncCallback;
    }
    else
    {
        wvrCallback = callback;
    }
    wvrArg = arg;

    // PXI�o�R��Terminate��ʒm
    if (0 > PXI_SendWordByFifo(PXI_FIFO_TAG_WVR, WVR_PXI_COMMAND_TERMINATE, FALSE))
    {
        wvrCallback = NULL;
        (void)OS_RestoreInterrupts(e);
        return WVR_RESULT_FIFO_ERROR;
    }

    // �񓯊������𐳏�ɊJ�n����
    (void)OS_RestoreInterrupts(e);
    return WVR_RESULT_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WvrReceiveCallback

  Description:  FIFO�����ARM7����̃R�[���o�b�N����M����B

  Arguments:    tag          - ���g�p�B
                data         - ARM7�����M�����f�[�^�B
                err          - ���g�p�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WvrReceiveCallback(PXIFifoTag tag, u32 data, BOOL err)
{
#pragma unused( tag , err )

    WVRCallbackFunc cb = wvrCallback;
    void   *cbArg = wvrArg;
    WVRResult result = (WVRResult)(data & 0x000000ff);

    // �R�}���h��ʂɂ�菈����U�蕪��
    switch (data & 0xffff0000)
    {
    case WVR_PXI_COMMAND_STARTUP:
        switch (result)
        {
        case WVR_RESULT_FATAL_ERROR:
            // VRAM ��r������
            if ((wvrVRam != 0) && (wvrLockId != 0))
            {
                OSi_UnlockVram(wvrVRam, wvrLockId);
                wvrVRam = 0;
            }
            break;
        }
        break;
    case WVR_PXI_COMMAND_TERMINATE:
        switch (result)
        {
        case WVR_RESULT_SUCCESS:
            // VRAM ��r������
            if ((wvrVRam != 0) && (wvrLockId != 0))
            {
                OSi_UnlockVram(wvrVRam, wvrLockId);
                wvrVRam = 0;
            }
            break;
        }
        // PXI �R�[���o�b�N��ݒ�� NULL �N���A
        PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WVR, NULL);
        break;
    }

    // ���[�U�[�R�[���o�b�N�Ăяo��
    if (cb != NULL)
    {
        wvrCallback = NULL;
        wvrArg = NULL;
        cb(cbArg, result);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WvrDummyAsyncCallback

  Description:  �񓯊��֐��ɐݒ肷��_�~�[�̃R�[���o�b�N�֐��B

  Arguments:    arg     -   ���g�p�B
                result  -   �񓯊������̏������ʁB

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WvrDummyAsyncCallback(void *arg, WVRResult result)
{
#pragma unused( arg , result )
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
