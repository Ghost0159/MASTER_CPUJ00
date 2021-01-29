/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm_system.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_system.c,v $
  Revision 1.41  2007/10/16 02:00:38  okubata_ryoma
  �s�v�� warning ���폜

  Revision 1.40  2006/06/28 23:57:15  seiki_masashi
  PortCallback �̏������������C��

  Revision 1.39  2006/05/25 00:31:08  terui
  WmInitCore �֐������荞�݋֎~���čs���悤�ɏC��
  WM_Finish �֐������荞�݋֎~���čs���悤�ɏC��

  Revision 1.38  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.37  2005/12/01 15:05:15  seiki_masashi
  WMPortSend/RecvCallback �\���̂� maxSend/RecvDataSize �t�B�[���h��ǉ�

  Revision 1.36  2005/11/24 03:05:33  seiki_masashi
  �x���̏����̂��߁A�֐��錾��ǉ�

  Revision 1.35  2005/11/22 08:58:10  seiki_masashi
  WM_SendCommandDirect �֐��̒ǉ�

  Revision 1.34  2005/11/07 01:10:33  okubata_ryoma
  SDK_ASSERT_ON_COMPILE����SDK_COMPILER_ASSERT�ɕύX

  Revision 1.33  2005/11/01 11:47:21  seiki_masashi
  SDK_ASSERT_ON_COMPILE �̒�`���w�b�_�t�@�C���Ɉړ�
  �f�o�b�O�R�[�h�̒ǉ�
  WM_STATECODE_PORT_RECV �ł� connectedAidBitmap ��ʒm����悤�ɕύX

  Revision 1.32  2005/11/01 08:54:28  seiki_masashi
  WmReceiveFifo �̍�����
  wm9buf->myAid, connectedAidBitmap �̒ǉ�
  WM_INDBUF_SIZE �̍폜

  Revision 1.31  2005/10/28 11:22:41  seiki_masashi
  port �̐ؒf�ʒm�� reason ��ʒm����悤�ɕύX

  Revision 1.30  2005/10/28 11:13:54  seiki_masashi
  WM_STATECODE_DISCONNECTED_FROM_MYSELF �̎���
  WMStatus �\���̂ւ̃A�N�Z�T�̎���
  �q�@���� port �ڑ��E�ؒf�ʒm�̕s��̏C��

  Revision 1.29  2005/09/16 10:14:21  terui
  WMi_GetSystemWork�֐��ł�NULL�A�T�[�g���폜

  Revision 1.28  2005/06/07 05:45:26  seiki_masashi
  Key Sharing �Ɋւ�����ʏ�����ጸ���邽�߂̕ύX

  Revision 1.27  2005/03/03 14:45:27  seiki_masashi
  WM_DISABLE_KEYSHARING �X�C�b�`�̒ǉ�

  Revision 1.26  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.25  2005/02/23 12:48:14  seiki_masashi
  �f�o�b�O�\���̐���

  Revision 1.24  2005/02/18 12:27:45  seiki_masashi
  �f�o�b�O�p�X�C�b�`�̔��蕶�� #if ���� #ifdef �ɕύX

  Revision 1.23  2005/02/14 06:37:47  terui
  WMi_CheckMpPacketTimeRequired�֐���ǉ��B

  Revision 1.22  2005/01/27 02:23:08  takano_makoto
  fix comments.

  Revision 1.21  2005/01/26 05:36:52  takano_makoto
  WMi_InitForScan��ǉ�

  Revision 1.19  2005/01/26 01:35:28  seiki_masashi
  PXI ���荞�݃n���h������ ARM9Buf->scanOnlyFlag ������
  WMStatus �� cache invalidate ������悤�ɕύX

  Revision 1.18  2005/01/26 01:12:11  seiki_masashi
  PXI ���荞�݃n���h������ WMStatus �� cache invalidate �����Ȃ��悤�ɕύX

  Revision 1.17  2005/01/25 01:57:25  yasu
  Copyright �\�L�̏C��

  Revision 1.16  2005/01/17 02:34:08  terui
  WmReceiveFifo�֐����ŁA���[�U�[�R�[���o�b�N�Ăяo�����WM_Finish���Ăяo���ꂽ���ǂ����m�F����悤�ɏC���B

  Revision 1.15  2004/12/27 05:31:55  terui
  ���N�G�X�g�`�B�p�����O�o�b�t�@�̔r��������ǉ��B

  Revision 1.14  2004/10/22 08:52:52  terui
  �Ԉ���č폜�����A�����N�G�X�g���L���[���Ă����d�l�𕜋��B

  Revision 1.13  2004/10/22 04:37:28  terui
  WMErrCode�Ƃ���int�^��Ԃ��Ă����֐��ɂ��āAWMErrCode�񋓌^��Ԃ��悤�ɕύX�B

  Revision 1.12  2004/10/21 12:08:25  terui
  PXI�R�[���o�b�N����WM_End�ɑ΂���㏈�����s���ʒu�����[�U�[�R�[���o�b�N�Ăяo�����O�ɕύX�B

  Revision 1.11  2004/10/05 09:45:28  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.10  2004/10/05 01:04:17  terui
  �R���p�C�����̍\���̃T�C�Y�`�F�b�N��ǉ��B

  Revision 1.9  2004/10/03 07:47:13  ooe
  ipl_branch�ƃ}�[�W�B Flash�G���[���̃t���[�Y������ǉ��B

  Revision 1.8  2004/09/29 09:48:25  terui
  WMi_SendCommand�������ݓ�����Ăяo����邱�Ƃ�z�肵���C���B

  Revision 1.7  2004/09/24 04:02:39  terui
  WM_Init���̃G���[�`�F�b�N������ύX�B

  Revision 1.6  2004/09/23 09:24:17  seiki_masashi
  WMi_SendCommand �̔r��������C��

  Revision 1.5  2004/09/17 06:36:08  seiki_masashi
  WMPortRecvCallback �� ssid ����� myAid �t�B�[���h��ǉ�

  Revision 1.4  2004/09/14 02:47:58  terui
  ���������ɗ^����ꂽ�o�b�t�@�̃L���b�V����j������@�\��ǉ��B

  Revision 1.3  2004/09/11 06:09:12  yasu
  Use OS_TPrintf instead of WMi_Printf

  Revision 1.2  2004/09/11 04:13:00  terui
  �x���\�����s���v�����g�����܂�X�^�b�N������Ȃ����̂ɕύX�B

  Revision 1.1  2004/09/10 11:03:30  terui
  wm.c�̕����ɔ����A�V�Kupload�B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/wm.h>
#include    "wm_arm9_private.h"


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     WM_BUF_MSG_NUM                  10


/*---------------------------------------------------------------------------*
    �����ϐ���`
 *---------------------------------------------------------------------------*/
static u16 wmInitialized = 0;          // WM�������t���O
static WMArm9Buf *wm9buf;
static u32 fifoBuf[WM_BUF_MSG_NUM][WM_FIFO_BUF_SIZE / sizeof(u32)] ATTRIBUTE_ALIGN(32);
static OSMessageQueue bufMsgQ;         // WM7�ւ̃��N�G�X�g�L���[
static OSMessage bufMsg[WM_BUF_MSG_NUM];        // WM7�ւ̃��N�G�X�g�L���[�p�o�b�t�@


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static void WmReceiveFifo(PXIFifoTag tag, u32 fifo_buf_adr, BOOL err);
static void WmClearFifoRecvFlag(void);
static WMErrCode WmInitCore(void *wmSysBuf, u16 dmaNo, u32 bufSize);
static u32 *WmGetCommandBuffer4Arm7(void);


/*---------------------------------------------------------------------------*
  Name:         WM_Init

  Description:  WM���C�u�����̏������������s���B
                ARM9���̏������݂̂��s�������֐��B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                dmaNo       -   WM���g�p����DMA�ԍ��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Init(void *wmSysBuf, u16 dmaNo)
{
    WMErrCode result;
    result = WmInitCore(wmSysBuf, dmaNo, WM_SYSTEM_BUF_SIZE);
    if (result != WM_ERRCODE_SUCCESS)
    {
        return result;
    }
    wm9buf->scanOnlyFlag = FALSE;
    return result;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_InitForScan

  Description:  �X�L�����݂̂��g�p����ꍇ��WM���C�u�����̏������������s���B
                ARM9���̏������݂̂��s�������֐��B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE_FOR_SCAN�����K�v�B
                dmaNo       -   WM���g�p����DMA�ԍ��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_InitForScan(void *wmSysBuf, u16 dmaNo);
WMErrCode WMi_InitForScan(void *wmSysBuf, u16 dmaNo)
{
#define WM_STATUS_BUF_SIZE_FOR_SCAN      768
#define WM_ARM9WM_BUF_SIZE_FOR_SCAN      320
#define WM_SYSTEM_BUF_SIZE_FOR_SCAN      (WM_ARM9WM_BUF_SIZE_FOR_SCAN + WM_ARM7WM_BUF_SIZE + WM_STATUS_BUF_SIZE_FOR_SCAN + WM_FIFO_BUF_SIZE + WM_FIFO_BUF_SIZE)

    WMErrCode result;
    result = WmInitCore(wmSysBuf, dmaNo, WM_SYSTEM_BUF_SIZE_FOR_SCAN);
    if (result != WM_ERRCODE_SUCCESS)
    {
        return result;
    }
    wm9buf = (WMArm9Buf *)wmSysBuf;
    wm9buf->WM7 = (WMArm7Buf *)((u32)wm9buf + WM_ARM9WM_BUF_SIZE_FOR_SCAN);
    wm9buf->status = (WMStatus *)((u32)(wm9buf->WM7) + WM_ARM7WM_BUF_SIZE);
    wm9buf->fifo9to7 = (u32 *)((u32)(wm9buf->status) + WM_STATUS_BUF_SIZE_FOR_SCAN);
    wm9buf->fifo7to9 = (u32 *)((u32)(wm9buf->fifo9to7) + WM_FIFO_BUF_SIZE);
    wm9buf->dmaNo = dmaNo;
    wm9buf->scanOnlyFlag = TRUE;

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         WmInitCore

  Description:  WM���C�u�����̏������������s���A���C�u���������g�p�֐��B
                ARM9���̏������݂̂��s�������֐��B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                dmaNo       -   WM���g�p����DMA�ԍ��B
                bufSize     -   WM���C�u�����Ɋ��蓖�Ă���o�b�t�@�T�C�Y

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
static WMErrCode WmInitCore(void *wmSysBuf, u16 dmaNo, u32 bufSize)
{
    OSIntrMode e;

    // �\���̂̃T�C�Y���z��l���I�[�o�[���Ă��Ȃ����`�F�b�N
    SDK_COMPILER_ASSERT(sizeof(WMArm9Buf) <= WM_ARM9WM_BUF_SIZE);
    SDK_COMPILER_ASSERT(sizeof(WMArm7Buf) <= WM_ARM7WM_BUF_SIZE);
    SDK_COMPILER_ASSERT(sizeof(WMStatus) <= WM_STATUS_BUF_SIZE);

    e = OS_DisableInterrupts();
    // WM���C�u�����̏������ς݂��m�F
    if (wmInitialized)
    {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;        // �������ς�
    }

    // �p�����[�^�`�F�b�N
    if (wmSysBuf == NULL)
    {
        WM_WARNING("Parameter \"wmSysBuf\" must not be NULL.\n");
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (dmaNo > MI_DMA_MAX_NUM)
    {
        WM_WARNING("Parameter \"dmaNo\" is over %d.\n", MI_DMA_MAX_NUM);
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)wmSysBuf & 0x01f)
    {
        WM_WARNING("Parameter \"wmSysBuf\" must be 32-byte aligned.\n");
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_INVALID_PARAM;
    }

    // ARM7����WM���C�u�������J�n����Ă��邩�m�F
    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_WM, PXI_PROC_ARM7))
    {
        WM_WARNING("WM library on ARM7 side is not ready yet.\n");
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_WM_DISABLE;
    }

    /*  �L���b�V���ɗ]�v�ȃf�[�^���c���Ă���Ƒz��O�̃^�C�~���O��
       ���C�g�o�b�N���������AARM7���̏�ԊǗ��Ɏx��𗈂����\��������B
       ���̑΍�Ƃ��Ă����őS���[�N�o�b�t�@�̃L���b�V����j�����Ă����B */
    DC_InvalidateRange(wmSysBuf, bufSize);
    // �e��o�b�t�@������
    MI_DmaClear32(dmaNo, wmSysBuf, bufSize);
    wm9buf = (WMArm9Buf *)wmSysBuf;
    wm9buf->WM7 = (WMArm7Buf *)((u32)wm9buf + WM_ARM9WM_BUF_SIZE);
    wm9buf->status = (WMStatus *)((u32)(wm9buf->WM7) + WM_ARM7WM_BUF_SIZE);
    wm9buf->fifo9to7 = (u32 *)((u32)(wm9buf->status) + WM_STATUS_BUF_SIZE);
    wm9buf->fifo7to9 = (u32 *)((u32)(wm9buf->fifo9to7) + WM_FIFO_BUF_SIZE);

    // FIFO�o�b�t�@�������݋��t���O���N���A
    WmClearFifoRecvFlag();

    // �e��ϐ���������
    wm9buf->dmaNo = dmaNo;
    wm9buf->connectedAidBitmap = 0x0000;
    wm9buf->myAid = 0;

    // �e�|�[�g�p�R�[���o�b�N�e�[�u����������
    {
        s32     i;

        for (i = 0; i < WM_NUM_OF_PORT; i++)
        {
            wm9buf->portCallbackTable[i] = NULL;
            wm9buf->portCallbackArgument[i] = NULL;
        }
    }

    // �G���g���[�o�^�p�L���[��������
    {
        s32     i;

        OS_InitMessageQueue(&bufMsgQ, bufMsg, WM_BUF_MSG_NUM);
        for (i = 0; i < WM_BUF_MSG_NUM; i++)
        {
            // �����O�o�b�t�@���g�p�\�ȏ�ԂɃN���A
            *((u16 *)(fifoBuf[i])) = WM_API_REQUEST_ACCEPTED;
            DC_StoreRange(fifoBuf[i], 2);
            (void)OS_SendMessage(&bufMsgQ, fifoBuf[i], OS_MESSAGE_BLOCK);
        }
    }

    // FIFO��M�֐���ݒ�
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, WmReceiveFifo);

    wmInitialized = 1;
    (void)OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         WM_Finish

  Description:  WM���C�u�����̏I���������s���B�����֐��B
                WM_Init�֐����ĂԑO�̏�Ԃɖ߂�B

  Arguments:    None.

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Finish(void)
{
    OSIntrMode e;
    WMErrCode result;

    e = OS_DisableInterrupts();
    // �������ς݂łȂ��ꍇ�͎��s�s��
    result = WMi_CheckInitialized();
    if (result != WM_ERRCODE_SUCCESS)
    {
        (void)OS_RestoreInterrupts(e);
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    // �X�e�[�g��READY�łȂ��ꍇ�͎��s�s��
    result = WMi_CheckState(WM_STATE_READY);
    WM_CHECK_RESULT(result);

    // WM���C�u�������I��
    WmClearFifoRecvFlag();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_WM, NULL);
    wm9buf = 0;

    wmInitialized = 0;
    (void)OS_RestoreInterrupts(e);
    return WM_ERRCODE_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_SetCallbackTable

  Description:  �e�񓯊��֐��ɑΉ������R�[���o�b�N�֐���o�^����B

  Arguments:    id          -   �񓯊��֐���API ID�B
                callback    -   �o�^����R�[���o�b�N�֐��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WMi_SetCallbackTable(WMApiid id, WMCallbackFunc callback)
{
    SDK_NULL_ASSERT(wm9buf);

    wm9buf->CallbackTable[id] = callback;
}

/*---------------------------------------------------------------------------*
  Name:         WmGetCommandBuffer4Arm7

  Description:  ARM7 �����̃R�}���h�p�o�b�t�@���v�[������m�ۂ���

  Arguments:    None.

  Returns:      �m�ۂł���΂��̒l, �m�ۂł��Ȃ���� NULL
 *---------------------------------------------------------------------------*/
u32    *WmGetCommandBuffer4Arm7(void)
{
    u32    *tmpAddr;

    if (FALSE == OS_ReceiveMessage(&bufMsgQ, (OSMessage *)&tmpAddr, OS_MESSAGE_NOBLOCK))
    {
        return NULL;
    }

    // �����O�o�b�t�@���g�p�\�ȏ��(�L���[����t�łȂ�)�����m�F
    DC_InvalidateRange(tmpAddr, 2);
    if ((*((u16 *)tmpAddr) & WM_API_REQUEST_ACCEPTED) == 0)
    {
        // �L���[�̐擪�ɍēx�\��( ��ԏ��߂Ɏg�p�\��ԂɂȂ�͂��Ȃ̂� )
        (void)OS_JamMessage(&bufMsgQ, tmpAddr, OS_MESSAGE_BLOCK);
        // �G���[�Ƃ��ďI��
        return NULL;
    }

    return tmpAddr;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_SendCommand

  Description:  FIFO�o�R�Ń��N�G�X�g��ARM7�ɑ��M����B
                u32�^�̃p�����[�^�������������R�}���h�̏ꍇ�́A
                �p�����[�^��񋓂��Ďw�肷��B

  Arguments:    id          -   ���N�G�X�g�ɑΉ�����API ID�B
                paramNum    -   ���z�����̐��B
                ...         -   ���z�����B

  Returns:      int -   WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_SendCommand(WMApiid id, u16 paramNum, ...)
{
    va_list vlist;
    s32     i;
    int     result;
    u32    *tmpAddr;

    SDK_NULL_ASSERT(wm9buf);

    // �R�}���h���M�p�̃o�b�t�@���m��
    tmpAddr = WmGetCommandBuffer4Arm7();
    if (tmpAddr == NULL)
    {
        return WM_ERRCODE_FIFO_ERROR;
    }

    // API ID
    *(u16 *)tmpAddr = (u16)id;

    // �w�萔�̈�����ǉ�
    va_start(vlist, paramNum);
    for (i = 0; i < paramNum; i++)
    {
        tmpAddr[i + 1] = va_arg(vlist, u32);
    }
    va_end(vlist);

    DC_StoreRange(tmpAddr, WM_FIFO_BUF_SIZE);

    // FIFO�Œʒm
    result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)tmpAddr, FALSE);

    (void)OS_SendMessage(&bufMsgQ, tmpAddr, OS_MESSAGE_BLOCK);

    if (result < 0)
    {
        WM_WARNING("Failed to send command through FIFO.\n");
        return WM_ERRCODE_FIFO_ERROR;
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_SendCommandDirect

  Description:  FIFO�o�R�Ń��N�G�X�g��ARM7�ɑ��M����B
                ARM7 �ɑ���R�}���h�𒼐ڎw�肷��B

  Arguments:    data        -   ARM7 �ɑ���R�}���h�B
                length      -   ARM7 �ɑ���R�}���h�̃T�C�Y�B

  Returns:      int -   WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_SendCommandDirect(void *data, u32 length)
{
    int     result;
    u32    *tmpAddr;

    SDK_NULL_ASSERT(wm9buf);
    SDK_ASSERT(length <= WM_FIFO_BUF_SIZE);

    // �R�}���h���M�p�̃o�b�t�@���m��
    tmpAddr = WmGetCommandBuffer4Arm7();
    if (tmpAddr == NULL)
    {
        return WM_ERRCODE_FIFO_ERROR;
    }

    // ARM7 �ɑ���R�}���h���p�o�b�t�@�ɃR�s�[����
    MI_CpuCopy8(data, tmpAddr, length);

    DC_StoreRange(tmpAddr, length);

    // FIFO�Œʒm
    result = PXI_SendWordByFifo(PXI_FIFO_TAG_WM, (u32)tmpAddr, FALSE);

    (void)OS_SendMessage(&bufMsgQ, tmpAddr, OS_MESSAGE_BLOCK);

    if (result < 0)
    {
        WM_WARNING("Failed to send command through FIFO.\n");
        return WM_ERRCODE_FIFO_ERROR;
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_GetSystemWork

  Description:  WM���C�u���������Ŏg�p����o�b�t�@�̐擪�ւ̃|�C���^���擾����B

  Arguments:    None.

  Returns:      WMArm9Buf*  -   �������[�N�o�b�t�@�ւ̃|�C���^��Ԃ��B
 *---------------------------------------------------------------------------*/
WMArm9Buf *WMi_GetSystemWork(void)
{
//    SDK_NULL_ASSERT(wm9buf);
    return wm9buf;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_CheckInitialized

  Description:  WM���C�u�����̏������ς݂��m�F����B

  Arguments:    None.

  Returns:      int -   WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_CheckInitialized(void)
{
    // �������ς݂��m�F
    if (!wmInitialized)
    {
        WM_WARNING("WM library is not initialized yet.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    return WM_ERRCODE_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_CheckIdle

  Description:  WM���C�u�����̓�����Ԃ��m�F���A�����n�[�h�E�F�A���N���ς݂�
                ���邱�Ƃ��m�F����B

  Arguments:    None.

  Returns:      int -   WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_CheckIdle(void)
{
    WMErrCode result;

    // �������ς݂��m�F
    result = WMi_CheckInitialized();
    WM_CHECK_RESULT(result);

    // ���X�e�[�g�m�F
    DC_InvalidateRange(&(wm9buf->status->state), 2);
    if ((wm9buf->status->state == WM_STATE_READY) || (wm9buf->status->state == WM_STATE_STOP))
    {
        WM_WARNING("WM state is \"%d\" now. So can't execute request.\n", wm9buf->status->state);
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    return WM_ERRCODE_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_CheckStateEx

  Description:  WM���C�u�����̓�����Ԃ��m�F����B
                ��������Ԃ�����WMState�^�̃p�����[�^��񋓂��Ďw�肷��B

  Arguments:    paramNum    -   ���z�����̐��B
                ...         -   ���z�����B

  Returns:      int -   WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_CheckStateEx(s32 paramNum, ...)
{
    WMErrCode result;
    u16     now;
    u32     temp;
    va_list vlist;

    // �������ς݂��m�F
    result = WMi_CheckInitialized();
    WM_CHECK_RESULT(result);

    // ���X�e�[�g�擾
    DC_InvalidateRange(&(wm9buf->status->state), 2);
    now = wm9buf->status->state;

    // ��v�m�F
    result = WM_ERRCODE_ILLEGAL_STATE;
    va_start(vlist, paramNum);
    for (; paramNum; paramNum--)
    {
        temp = va_arg(vlist, u32);
        if (temp == now)
        {
            result = WM_ERRCODE_SUCCESS;
        }
    }
    va_end(vlist);

    if (result == WM_ERRCODE_ILLEGAL_STATE)
    {
        WM_WARNING("WM state is \"%d\" now. So can't execute request.\n", now);
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         WmReceiveFifo

  Description:  FIFO�����WM7����̃R�[���o�b�N����M����B

  Arguments:    tag          - ���g�p
                fifo_buf_adr - �R�[���o�b�N�̃p�����[�^�Q�ւ̃|�C���^
                err          - ���g�p

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WmReceiveFifo(PXIFifoTag tag, u32 fifo_buf_adr, BOOL err)
{
#pragma unused( tag )

    WMCallback *callback_p = (WMCallback *)fifo_buf_adr;
    WMArm9Buf *w9b = wm9buf;           // volatile �ł͂Ȃ����Ƃ��R���p�C���ɓ`����
#ifdef WM_DEBUG_CALLBACK
    int     beginVcount = GX_GetVCount();
#endif

    if (err)
        return;                        // WM_sp_init �� PXI �n���h���̏����������Ă��Ȃ�

    // FIFO�R�}���h�o�b�t�@(9��7)�̃L���b�V���𖳌���
    DC_InvalidateRange(w9b->fifo7to9, WM_FIFO_BUF_SIZE);
    if (!w9b->scanOnlyFlag)
    {
        DC_InvalidateRange(w9b->status, WM_STATUS_BUF_SIZE);
    }

    // �����o�b�t�@�� fifo7to9 �Ƃ͕ʃo�b�t�@�łȂ����m�F
    if ((u32)callback_p != (u32)(w9b->fifo7to9))
    {
        DC_InvalidateRange(callback_p, WM_FIFO_BUF_SIZE);
    }


    if (callback_p->apiid >= WM_NUM_OF_CALLBACK)
    {
        if (callback_p->apiid == WM_APIID_INDICATION)
        {
#if SDK_VERSION_WL >= 21000
            if (callback_p->errcode == WM_ERRCODE_FLASH_ERROR)
            {
                // ARM9�t���[�Y
                OS_Terminate();        // �Ԃ��Ă��Ȃ��B
            }
#endif
            if (w9b->indCallback != NULL)
            {
                w9b->indCallback((void *)callback_p);
            }
        }
        else if (callback_p->apiid == WM_APIID_PORT_RECV)
        {
            // port �ԍ��ɉ������R�[���o�b�N�������s��
            WMPortRecvCallback *cb_Port = (WMPortRecvCallback *)callback_p;
            SDK_ASSERT(cb_Port->port < 16);
            // callback ���o�^����Ă�����ʒm����
            if (w9b->portCallbackTable[cb_Port->port] != NULL)
            {
                cb_Port->arg = w9b->portCallbackArgument[cb_Port->port];
                cb_Port->connectedAidBitmap = (u16)w9b->connectedAidBitmap;
                DC_InvalidateRange(cb_Port->recvBuf, w9b->status->mp_recvBufSize);      // ToDo: Invalidate �������Ă���
                (w9b->portCallbackTable[cb_Port->port]) ((void *)cb_Port);
            }
#ifdef WM_DEBUG
            else
            {
                WM_DPRINTF("Warning: no callback function on port %d { %04x %04x ... }\n",
                           cb_Port->port, cb_Port->data[0], cb_Port->data[1]);
            }
#endif
        }
        else if (callback_p->apiid == WM_APIID_PORT_SEND)
        {
            // �f�[�^���Z�b�g�����ۂɎw�肵���R�[���o�b�N�������s��
            WMPortSendCallback *cb_Port = (WMPortSendCallback *)callback_p;
            callback_p->apiid = WM_APIID_SET_MP_DATA;   // �U������
            if (cb_Port->callback != NULL)
            {
                (cb_Port->callback) ((void *)cb_Port);
            }
        }
        else
        {
            OS_Printf("ARM9: no callback function\n");

        }
    }
    else
    {
        // ��M�o�b�t�@�̃L���b�V���𖳌������Ă���
        // MPEND_IND, MP_IND �̌�ɑ����APORT_RECV �̌Ăяo���̍ۂ́A�����ł��ł�
        // InvalidateRange ����Ă��邽�߁A���߂ăL���b�V�����C�ɂ���K�v�͂Ȃ�
        if (callback_p->apiid == WM_APIID_START_MP)
        {
            WMStartMPCallback *cb_StartMP = (WMStartMPCallback *)callback_p;
            if (cb_StartMP->state == WM_STATECODE_MPEND_IND
                || cb_StartMP->state == WM_STATECODE_MP_IND)
            {
                if (cb_StartMP->errcode == WM_ERRCODE_SUCCESS)
                {
                    DC_InvalidateRange(cb_StartMP->recvBuf, w9b->status->mp_recvBufSize);
                }
            }
        }

        // WM_End�̌㏈��
        if (callback_p->apiid == WM_APIID_END)
        {
            if (callback_p->errcode == WM_ERRCODE_SUCCESS)
            {
                WMCallbackFunc cb = w9b->CallbackTable[callback_p->apiid];
                // WM���C�u�������I��
                (void)WM_Finish();
                if (cb != NULL)
                {
                    cb((void *)callback_p);
                }

                WM_DLOGF_CALLBACK("Cb[%x]", callback_p->apiid);

                return;
            }
        }

#if 0
        // WM_Reset, WM_Disconnect, WM_EndParent �̌㏈��
        if (callback_p->apiid == WM_APIID_RESET
            || callback_p->apiid == WM_APIID_DISCONNECT || callback_p->apiid == WM_APIID_END_PARENT)
        {
            if (w9b->connectedAidBitmap != 0)
            {
                WM_WARNING("connectedAidBitmap should be 0, but %04x", w9b->connectedAidBitmap);
            }
            w9b->myAid = 0;
            w9b->connectedAidBitmap = 0;
        }
#endif

        // apiid�ɉ������R�[���o�b�N���� (�R�[���o�b�N�����ݒ�(NULL)�Ȃ牽�����Ȃ�)
        if (NULL != w9b->CallbackTable[callback_p->apiid])
        {
            (w9b->CallbackTable[callback_p->apiid]) ((void *)callback_p);
            /* ���[�U�[�R�[���o�b�N����WM_Finish���Ăяo����A���[�N�p���������N���A���ꂽ�ꍇ�͂����ŏI�� */
            if (!wmInitialized)
            {
                return;
            }
        }
        // �㏈��
        // �ڑ��E�ؒf�̒ʒm�� port �� callback �ɂ��`���邽�߂ɁA���b�Z�[�W�����H����
        if (callback_p->apiid == WM_APIID_START_PARENT
            || callback_p->apiid == WM_APIID_START_CONNECT)
        {
            u16     state, aid, myAid, reason;
            u8     *macAddress;
            u8     *ssid;
            u16     parentSize, childSize;

            if (callback_p->apiid == WM_APIID_START_PARENT)
            {
                WMStartParentCallback *cb = (WMStartParentCallback *)callback_p;
                state = cb->state;
                aid = cb->aid;
                myAid = 0;
                macAddress = cb->macAddress;
                ssid = cb->ssid;
                reason = cb->reason;
                parentSize = cb->parentSize;
                childSize = cb->childSize;
            }
            else if (callback_p->apiid == WM_APIID_START_CONNECT)
            {
                WMStartConnectCallback *cb = (WMStartConnectCallback *)callback_p;
                state = cb->state;
                aid = 0;
                myAid = cb->aid;
                macAddress = cb->macAddress;
                ssid = NULL;
                reason = cb->reason;
                parentSize = cb->parentSize;
                childSize = cb->childSize;
            }
            if (state == WM_STATECODE_CONNECTED ||
                state == WM_STATECODE_DISCONNECTED
                || state == WM_STATECODE_DISCONNECTED_FROM_MYSELF)
            {
                // ���荞�݃n���h�����̂��߁Astatic �ł����͂Ȃ�
                static WMPortRecvCallback cb_Port;
                u16     iPort;

                // WM9 ���ŊǗ����Ă���ڑ���Ԃ�ύX
                if (state == WM_STATECODE_CONNECTED)
                {
#ifdef WM_DEBUG
                    if (w9b->connectedAidBitmap & (1 << aid))
                    {
                        WM_DPRINTF("Warning: someone is connecting to connected aid: %d (%04x)",
                                   aid, w9b->connectedAidBitmap);
                    }
#endif
                    WM_DLOGF_AIDBITMAP("aid(%d) connected: %04x -> %04x", aid,
                                       w9b->connectedAidBitmap,
                                       w9b->connectedAidBitmap | (1 << aid));
                    w9b->connectedAidBitmap |= (1 << aid);
                }
                else                   // WM_STATECODE_DISCONNECTED || WM_STATECODE_DISCONNECTED_FROM_MYSELF
                {
#ifdef WM_DEBUG
                    if (!(w9b->connectedAidBitmap & (1 << aid)))
                    {
                        WM_DPRINTF
                            ("Warning: someone is disconnecting to disconnected aid: %d (%04x)",
                             aid, w9b->connectedAidBitmap);
                    }
#endif
                    WM_DLOGF_AIDBITMAP("aid(%d) disconnected: %04x -> %04x", aid,
                                       w9b->connectedAidBitmap,
                                       w9b->connectedAidBitmap & ~(1 << aid));
                    w9b->connectedAidBitmap &= ~(1 << aid);
                }
                w9b->myAid = myAid;

                MI_CpuClear8(&cb_Port, sizeof(WMPortRecvCallback));
                cb_Port.apiid = WM_APIID_PORT_RECV;
                cb_Port.errcode = WM_ERRCODE_SUCCESS;
                cb_Port.state = state;
                cb_Port.recvBuf = NULL;
                cb_Port.data = NULL;
                cb_Port.length = 0;
                cb_Port.aid = aid;
                cb_Port.myAid = myAid;
                cb_Port.connectedAidBitmap = (u16)w9b->connectedAidBitmap;
                cb_Port.seqNo = 0xffff;
                cb_Port.reason = reason;
                MI_CpuCopy8(macAddress, cb_Port.macAddress, WM_SIZE_MACADDR);
                if (ssid != NULL)
                {
                    MI_CpuCopy16(ssid, cb_Port.ssid, WM_SIZE_CHILD_SSID);
                }
                else
                {
                    MI_CpuClear16(cb_Port.ssid, WM_SIZE_CHILD_SSID);
                }
                cb_Port.maxSendDataSize = (myAid == 0) ? parentSize : childSize;
                cb_Port.maxRecvDataSize = (myAid == 0) ? childSize : parentSize;

                // �S�Ă� port �ɑ΂��āA�ڑ��E�ؒf�̒ʒm���s��
                for (iPort = 0; iPort < WM_NUM_OF_PORT; iPort++)
                {
                    cb_Port.port = iPort;
                    if (w9b->portCallbackTable[iPort] != NULL)
                    {
                        cb_Port.arg = w9b->portCallbackArgument[iPort];
                        (w9b->portCallbackTable[iPort]) ((void *)&cb_Port);
                    }
                }
            }
        }
    }
    // PORT_SEND, PORT_RECV �� fifo7to9 �̈�ɏ������݂��s���Ă��܂��Ă��邽��
    // ���C�g�o�b�N����Ȃ��悤�L���b�V���𖳌��ɂ���
    DC_InvalidateRange(w9b->fifo7to9, WM_FIFO_BUF_SIZE);
    WmClearFifoRecvFlag();

    // �����o�b�t�@�� fifo7to9 �Ƃ͕ʃo�b�t�@�̏ꍇ�A���N�G�X�g��t�����t���O���U��
    if ((u32)callback_p != (u32)(w9b->fifo7to9))
    {
        callback_p->apiid |= WM_API_REQUEST_ACCEPTED;
        DC_StoreRange(callback_p, WM_FIFO_BUF_SIZE);
    }

    WM_DLOGF2_CALLBACK(beginVcount, "[CB](%x)", callback_p->apiid);

    return;
}

/*---------------------------------------------------------------------------*
  Name:         WmClearFifoRecvFlag

  Description:  WM7����̃R�[���o�b�N�ɗp����ꂽFIFO�f�[�^���Q�Ƃ��I�������Ƃ�
                WM7�ɒʒm����B
                WM7�ɂ����ăR�[���o�b�N��FIFO��p����ہA���̃t���O���A�����b�N
                �����̂�҂��Ă��玟�̃R�[���o�b�N��ҏW����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WmClearFifoRecvFlag(void)
{
    if (OS_GetSystemWork()->wm_callback_control & WM_EXCEPTION_CB_MASK)
    {
        // CB�r���t���O��������
        OS_GetSystemWork()->wm_callback_control &= ~WM_EXCEPTION_CB_MASK;
    }
}

/*---------------------------------------------------------------------------*
  Name:         WMi_DebugPrintSendQueue

  Description:  �|�[�g���M�L���[�̓��e���v�����g�o�͂���B

  Arguments:    queue - �|�[�g���M�L���[�ւ̃|�C���^���w��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WMi_DebugPrintSendQueue(WMPortSendQueue *queue)
{
    WMstatus *status = wm9buf->status;
    WMPortSendQueueData *queueData;
    u16     index;

    DC_InvalidateRange(wm9buf->status, WM_STATUS_BUF_SIZE);     // ARM7�X�e�[�^�X�̈�̃L���b�V���𖳌���
    queueData = status->sendQueueData;

    OS_Printf("head = %d, tail = %d, ", queue->head, queue->tail);
    if (queue->tail != WM_SEND_QUEUE_END)
    {
        OS_Printf("%s", (queueData[queue->tail].next == WM_SEND_QUEUE_END) ? "valid" : "invalid");
    }
    OS_Printf("\n");
    for (index = queue->head; index != WM_SEND_QUEUE_END; index = queueData[index].next)
    {
        WMPortSendQueueData *data = &(queueData[index]);

        OS_Printf("queueData[%d] -> %d { port=%d, destBitmap=%x, size=%d } \n", index, data->next,
                  data->port, data->destBitmap, data->size);
    }

}

/*---------------------------------------------------------------------------*
  Name:         WMi_DebugPrintAllSendQueue

  Description:  �S�Ẵ|�[�g���M�L���[�̓��e���v�����g�o�͂���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WMi_DebugPrintAllSendQueue(void)
{
    WMstatus *status = wm9buf->status;
#if 0
    int     iPrio;

    DC_InvalidateRange(wm9buf->status, WM_STATUS_BUF_SIZE);     // ARM7�X�e�[�^�X�̈�̃L���b�V���𖳌���
    for (iPrio = 0; iPrio < WM_PRIORITY_LEVEL; iPrio++)
    {
        OS_Printf("== send queue [%d]\n", iPrio);
        WMi_DebugPrintSendQueue(&status->sendQueue[iPrio]);
    }
    for (iPrio = 0; iPrio < WM_PRIORITY_LEVEL; iPrio++)
    {
        OS_Printf("== ready queue [%d]\n", iPrio);
        WMi_DebugPrintSendQueue(&status->readyQueue[iPrio]);
    }
    OS_Printf("== free queue\n");
    OS_Printf(" head: %d, tail: %d\n", status->sendQueueFreeList.head,
              status->sendQueueFreeList.tail);
//    WMi_DebugPrintSendQueue( &status->sendQueueFreeList );
#else
    DC_InvalidateRange(wm9buf->status, WM_STATUS_BUF_SIZE);     // ARM7�X�e�[�^�X�̈�̃L���b�V���𖳌���
    OS_Printf("== ready queue [2]\n");
    OS_Printf(" head: %d, tail: %d\n", status->readyQueue[2].head, status->readyQueue[2].tail);
#endif

}

/*---------------------------------------------------------------------------*
  Name:         WMi_GetStatusAddress

  Description:  WM���ŊǗ�����Ă���X�e�[�^�X�\���̂ւ̃|�C���^���擾����B
                ���̍\���̂�ARM7�����ڑ��삷��̂ŁAARM9����̏������݂͋֎~�B
                �܂��A���e�Q�Ƃ̑O�ɎQ�Ƃ��镔�����܂ރL���b�V�����C����j��
                ����K�v������_�ɒ��ӁB

  Arguments:    None.

  Returns:      const WMStatus* - �X�e�[�^�X�\���̂ւ̃|�C���^��Ԃ��B
 *---------------------------------------------------------------------------*/
const WMStatus *WMi_GetStatusAddress(void)
{
    // �������`�F�b�N
    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS)
    {
        return NULL;
    }

    return wm9buf->status;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_CheckMpPacketTimeRequired

  Description:  MP�ʐM�̂P�p�P�b�g�̓]�����v���Ԃ� 5600��s �ȉ��ł��邱�Ƃ�
                �m�F����B

  Arguments:    parentSize  -   �e�@�]���f�[�^�T�C�Y�B
                childSize   -   �q�@�]���f�[�^�T�C�Y�B
                childs      -   �ʐM����q�@�̑䐔�B

  Returns:      BOOL        -   ���e�͈͂ł���ꍇ��TRUE��Ԃ��B
                                5600��s ���z����ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WMi_CheckMpPacketTimeRequired(u16 parentSize, u16 childSize, u8 childs)
{
    s32     mp_time;

    // ����MP�ʐM�ɂ����鎞�Ԃ�us�P�ʂŌv�Z
    mp_time = ((                       // --- �e�@���M���� ---
                   96                  // Preamble
                   + (24               // 802.11 Header
                      + 4              // TXOP + PollBitmap
                      + 2              // wmHeader
                      + parentSize + 4 // wmFooter( parent )
                      + 4              // FCS
                   ) * 4               // 4us per 1 byte
               ) + (                   // --- �q�@���M���� ---
                       (10             // SIFS
                        + 96           // Preamble
                        + (24          // 802.11 Header
                           + 2         // wmHeader
                           + childSize + 2      // wmFooter( child )
                           + 4         // FCS
                        ) * 4          // 4us per 1 byte
                        + 6            // time to spare
                       ) * childs) + ( // --- MP ACK ���M���� ---
                                         10     // SIFS
                                         + 96   // Preamble
                                         + (24  // 802.11 Header
                                            + 4 // ACK
                                            + 4 // FCS
                                         ) * 4  // 4us per 1 byte
               ));

    if (mp_time > WM_MAX_MP_PACKET_TIME)
    {
        OS_TWarning
            ("It is required %dus to transfer each MP packet.\nThat should not exceed %dus.\n",
             mp_time, WM_MAX_MP_PACKET_TIME);
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_IsMP

  Description:  ���݂� MP �ʐM��Ԃ��擾����B

  Arguments:    None.

  Returns:      MP �ʐM��Ԃł���� TRUE
 *---------------------------------------------------------------------------*/
BOOL WMi_IsMP(void)
{
    WMStatus *status;
    BOOL    isMP;
    OSIntrMode e;

#ifdef SDK_DEBUG
    // �������`�F�b�N
    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS)
    {
        return FALSE;
    }
#endif

    // �����݋֎~
    e = OS_DisableInterrupts();

    if (wm9buf != NULL)
    {
        status = wm9buf->status;
        DC_InvalidateRange(&(status->mp_flag), 4);
        isMP = status->mp_flag;
    }
    else
    {
        isMP = FALSE;
    }

    // �����݋֎~�߂�
    (void)OS_RestoreInterrupts(e);

    return isMP;
}

/*---------------------------------------------------------------------------*
  Name:         WM_GetAID

  Description:  ���݂� AID ���擾����B
                ��Ԃ� PARENT, MP_PARENT, CHILD, MP_CHILD �̂����ꂩ�̎��̂�
                �L���Ȓl��Ԃ��B

  Arguments:    None.

  Returns:      AID
 *---------------------------------------------------------------------------*/
u16 WM_GetAID(void)
{
    u16     myAid;
    OSIntrMode e;

#ifdef SDK_DEBUG
    // �������`�F�b�N
    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS)
    {
        return 0;
    }
#endif

    // �����݋֎~
    e = OS_DisableInterrupts();

    if (wm9buf != NULL)
    {
        myAid = wm9buf->myAid;
    }
    else
    {
        myAid = 0;
    }

    // �����݋֎~�߂�
    (void)OS_RestoreInterrupts(e);

    return myAid;
}

/*---------------------------------------------------------------------------*
  Name:         WM_GetConnectedAIDs

  Description:  ���݂̐ڑ�������r�b�g�}�b�v�̌`�Ŏ擾����B
                ��Ԃ� PARENT, MP_PARENT, CHILD, MP_CHILD �̂����ꂩ�̎��̂�
                �L���Ȓl��Ԃ��B
                �q�@�̏ꍇ�͐e�@�Ɛڑ����� 0x0001 ��Ԃ��B

  Arguments:    None.

  Returns:      �ڑ����Ă��鑊��� AID �̃r�b�g�}�b�v
 *---------------------------------------------------------------------------*/
u16 WM_GetConnectedAIDs(void)
{
    u32     connectedAidBitmap;
    OSIntrMode e;

#ifdef SDK_DEBUG
    // �������`�F�b�N
    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS)
    {
        return 0;
    }
#endif

    // �����݋֎~
    e = OS_DisableInterrupts();

    if (wm9buf != NULL)
    {
        connectedAidBitmap = wm9buf->connectedAidBitmap;
    }
    else
    {
        connectedAidBitmap = 0;
    }

    // �����݋֎~�߂�
    (void)OS_RestoreInterrupts(e);

#ifdef WM_DEBUG
/*
    if (WMi_CheckStateEx(4, WM_STATE_PARENT, WM_STATE_CHILD, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD)
        != WM_ERRCODE_SUCCESS && connectedAidBitmap != 0)
    {
        WM_WARNING("connectedAidBitmap should be 0, but %04x", connectedAidBitmap);
    }
*/
#endif

    return (u16)connectedAidBitmap;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_GetMPReadyAIDs

  Description:  ���݂̐ڑ�����̂����AMP ����M�ł��鑊��� AID �̈ꗗ��
                �r�b�g�}�b�v�̌`�Ŏ擾����B
                ��Ԃ� PARENT, MP_PARENT, CHILD, MP_CHILD �̂����ꂩ�̎��̂�
                �L���Ȓl��Ԃ��B
                �q�@�̏ꍇ�͐e�@�Ɛڑ����� 0x0001 ��Ԃ��B

  Arguments:    None.

  Returns:      MP ���J�n���Ă��鑊��� AID �̃r�b�g�}�b�v
 *---------------------------------------------------------------------------*/
u16 WMi_GetMPReadyAIDs(void)
{
    WMStatus *status;
    u16     mpReadyAidBitmap;
    OSIntrMode e;

#ifdef SDK_DEBUG
    // �������`�F�b�N
    if (WMi_CheckInitialized() != WM_ERRCODE_SUCCESS)
    {
        return FALSE;
    }
#endif

    // �����݋֎~
    e = OS_DisableInterrupts();

    if (wm9buf != NULL)
    {
        status = wm9buf->status;
        DC_InvalidateRange(&(status->mp_readyBitmap), 2);
        mpReadyAidBitmap = status->mp_readyBitmap;
    }
    else
    {
        mpReadyAidBitmap = FALSE;
    }

    // �����݋֎~�߂�
    (void)OS_RestoreInterrupts(e);

    return mpReadyAidBitmap;
}


/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
