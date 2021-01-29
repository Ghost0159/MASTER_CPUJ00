/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm_dcf.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_dcf.c,v $
  Revision 1.7  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.6  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.5  2004/12/03 06:35:20  ooe
  �ő呗�M�T�C�Y�̃`�F�b�N���A1500��WM_DCF_MAX_SIZE�ɏC���B

  Revision 1.4  2004/10/22 04:37:28  terui
  WMErrCode�Ƃ���int�^��Ԃ��Ă����֐��ɂ��āAWMErrCode�񋓌^��Ԃ��悤�ɕύX�B

  Revision 1.3  2004/10/08 08:37:37  ooe
  small fix

  Revision 1.2  2004/10/05 09:45:28  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.1  2004/09/10 11:03:30  terui
  wm.c�̕����ɔ����A�V�Kupload�B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/wm.h>
#include    "wm_arm9_private.h"


/*---------------------------------------------------------------------------*
  Name:         WM_StartDCF

  Description:  �C���t���X�g���N�`�����[�h�ł̒ʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                recvBuf     -   �f�[�^��M�o�b�t�@�ւ̃|�C���^�B
                                ARM7�����ڃf�[�^�������o���̂ŁA�L���b�V���ɒ��ӁB
                recvBufSize -   �f�[�^��M�o�b�t�@�̃T�C�Y�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartDCF(WMCallbackFunc callback, WMDcfRecvBuf *recvBuf, u16 recvBufSize)
{
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();

    // �X�e�[�g�`�F�b�N
    result = WMi_CheckState(WM_STATE_CHILD);
    WM_CHECK_RESULT(result);

    // DCF��Ԋm�F
    DC_InvalidateRange(&(p->status->dcf_flag), 4);
    if (p->status->dcf_flag == TRUE)
    {
        WM_WARNING("Already DCF mode. So can't start DCF again.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    // �p�����[�^�`�F�b�N
    if (recvBufSize < 16)
    {
        WM_WARNING("Parameter \"recvBufSize\" must not be less than %d.\n", 16);
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (recvBuf == NULL)
    {
        WM_WARNING("Parameter \"recvBuf\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)recvBuf & 0x01f)
    {
        // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
        WM_WARNING("Parameter \"recvBuf\" is not 32-byte aligned.\n");
    }

    // �w��o�b�t�@�̃L���b�V���������o��
    DC_StoreRange(recvBuf, recvBufSize);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_DCF, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_START_DCF, 2, (u32)recvBuf, (u32)recvBufSize);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetDCFData

  Description:  �C���t���X�g���N�`�����[�h�ʐM�ɂđ��M����f�[�^��\�񂷂�

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                destAdr     -   �ʐM�����MAC�A�h���X�������o�b�t�@�ւ̃|�C���^�B
                sendData    -   ���M�\�񂷂�f�[�^�ւ̃|�C���^�B
                                ���M�\�񂷂�f�[�^�̎��̂͋����I�ɃL���b�V���X�g�A
                                �����_�ɒ��ӁB
                sendDataSize -  ���M�\�񂷂�f�[�^�̃T�C�Y�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode
WM_SetDCFData(WMCallbackFunc callback, const u8 *destAdr, const u16 *sendData, u16 sendDataSize)
{
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();
    u32     wMac[2];

    // �X�e�[�g�`�F�b�N
    result = WMi_CheckState(WM_STATE_DCF_CHILD);
    WM_CHECK_RESULT(result);

    // DCF��Ԋm�F
    DC_InvalidateRange(&(p->status->dcf_flag), 4);
    if (p->status->dcf_flag == FALSE)
    {
        WM_WARNING("It is not DCF mode now.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    // �p�����[�^�`�F�b�N
    if (sendDataSize > WM_DCF_MAX_SIZE)
    {
        WM_WARNING("Parameter \"sendDataSize\" is over %d.\n", WM_DCF_MAX_SIZE);
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)sendData & 0x01f)
    {
        // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
        WM_WARNING("Parameter \"sendData\" is not 32-byte aligned.\n");
    }

    // �w��o�b�t�@�̃L���b�V���������o��
    DC_StoreRange((void *)sendData, sendDataSize);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_DCF_DATA, callback);

    // MAC�A�h���X���R�s�[
    MI_CpuCopy8(destAdr, wMac, 6);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_DCF_DATA, 4,
                             (u32)wMac[0], (u32)wMac[1], (u32)sendData, (u32)sendDataSize);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_EndDCF

  Description:  �C���t���X�g���N�`�����[�h�ł̒ʐM���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndDCF(WMCallbackFunc callback)
{
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();

    // �X�e�[�g�`�F�b�N
    result = WMi_CheckState(WM_STATE_DCF_CHILD);
    WM_CHECK_RESULT(result);

    // DCF��Ԋm�F
    DC_InvalidateRange(&(p->status->dcf_flag), 4);
    if (p->status->dcf_flag == FALSE)
    {
        WM_WARNING("It is not DCF mode now.\n");
        return WM_ERRCODE_ILLEGAL_STATE;
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_END_DCF, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_END_DCF, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
