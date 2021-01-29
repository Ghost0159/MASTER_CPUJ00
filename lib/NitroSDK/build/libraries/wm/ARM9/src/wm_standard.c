/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - libraries
  File:     wm_standard.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wm_standard.c,v $
  Revision 1.28.2.1  2008/01/18 00:58:32  seiki_masashi
  HEAD �ƃ}�[�W

  Revision 1.29  2008/01/17 08:32:09  seiki_masashi
  WM_EnableForListening �֐��� blink �����̉��߂��C��

  Revision 1.28  2007/11/12 09:49:23  seiki_masashi
  fix copyright header.

  Revision 1.27  2007/11/09 14:05:46  seiki_masashi
  �s��C��

  Revision 1.26  2007/11/09 13:08:43  seiki_masashi
  �d�g�̎�M�̂݉\�ȏ�Ԃɖ���������������֐���ǉ�

  Revision 1.25  2006/07/06 11:20:52  okubata_ryoma
  undo

  Revision 1.23  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.22  2005/11/22 09:00:52  seiki_masashi
  WM_SendCommandDirect �֐����g�p����悤�ɕύX

  Revision 1.21  2005/11/01 11:48:42  seiki_masashi
  �f�o�b�O�R�[�h�̒ǉ�

  Revision 1.20  2005/09/08 00:49:06  seiki_masashi
  WM_StartScanEx �֐��� param �� ssidMatchLength �w��ɑΉ�

  Revision 1.19  2005/04/11 01:15:20  seiki_masashi
  WMi_StartParentEx ��ǉ�

  Revision 1.18  2005/03/16 06:18:16  terui
  WM_StartScanEx�̃R�[���o�b�N�֐��o�^�e�[�u��index���C���B

  Revision 1.17  2005/03/06 23:59:31  terui
  WM_StartConnectEx�֐��ɂăL���b�V���X�g�A����T�C�Y��K���l�ɏC���B

  Revision 1.16  2005/02/28 05:26:35  yosizaki
  do-indent.

  Revision 1.15  2005/02/14 11:43:06  terui
  WM_StartScan[Ex]�ɂāAparam�p�����[�^�̃L���b�V���X�g�A�͕s�K�v�Ȃ̂ō폜�B

  Revision 1.14  2005/02/14 06:38:45  terui
  �f�[�^�]�����v���Ԃ��`�F�b�N����@�\��MP�J�n���Ɉړ��B

  Revision 1.13  2005/02/08 06:50:15  ooe
  WM_StartScanEx���d�l�ύX�B

  Revision 1.12  2005/01/25 01:57:25  yasu
  Copyright �\�L�̏C��

  Revision 1.11  2005/01/17 00:59:36  ooe
  WM_StartConnect���AWM_StartConnectEx�ɍ����ւ��B

  Revision 1.10  2004/10/29 02:53:25  terui
  WM_StartScanEx���ꎞ�I�ɔ���J�֐��ɕύX�B

  Revision 1.9  2004/10/27 04:23:57  ooe
  �s�v��OS_Printf���폜�B

  Revision 1.8  2004/10/26 08:21:44  ooe
  WM_StartScanEx��ǉ�

  Revision 1.7  2004/10/22 04:37:28  terui
  WMErrCode�Ƃ���int�^��Ԃ��Ă����֐��ɂ��āAWMErrCode�񋓌^��Ԃ��悤�ɕύX�B

  Revision 1.6  2004/10/05 09:45:28  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.5  2004/10/01 11:05:22  terui
  �x�����b�Z�[�W������ɏC���B

  Revision 1.4  2004/09/15 08:31:43  terui
  Debug�r���h���̃A���C���`�F�b�N�̔�����@���Ԉ���Ă����_���C���B

  Revision 1.3  2004/09/15 02:19:48  terui
  MP����M�T�C�Y�̌x����MP�p�P�b�g����M�ɗv���鎞�Ԃ̌x���ɍ��ւ��B

  Revision 1.2  2004/09/10 12:05:40  seiki_masashi
  �L�[�V�F�A�����O�̑��M�o�C�g�������C�u�������ŉB������悤�ɕύX

  Revision 1.1  2004/09/10 11:03:30  terui
  wm.c�̕����ɔ����A�V�Kupload�B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include    <nitro/wm.h>
#include    "wm_arm9_private.h"


/*---------------------------------------------------------------------------*
    �����֐���`
 *---------------------------------------------------------------------------*/
static BOOL WmCheckParentParameter(const WMParentParam *param);


/*---------------------------------------------------------------------------*
  Name:         WM_Enable

  Description:  �����n�[�h�E�F�A���g�p�\�ȏ�Ԃɂ���B
                ������Ԃ�READY��Ԃ���STOP��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Enable(WMCallbackFunc callback)
{
    return WMi_EnableEx(callback, 0);
}

/*---------------------------------------------------------------------------*
  Name:         WM_EnableForListening

  Description:  �����n�[�h�E�F�A���g�p�\�ȏ�Ԃɂ���B
                ������Ԃ�READY��Ԃ���STOP��ԂɑJ�ڂ����B
                �d�g�𑗏o����I�y���[�V�����͂ł��Ȃ��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                blink       -   LED ��_�ł����邩

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EnableForListening(WMCallbackFunc callback, BOOL blink)
{
    u32 miscFlags = WM_MISC_FLAG_LISTEN_ONLY;

    if (!blink)
    {
        miscFlags |= WM_MISC_FLAG_NO_BLINK;
    }

    return WMi_EnableEx(callback, miscFlags);
}

/*---------------------------------------------------------------------------*
  Name:         WMi_EnableEx

  Description:  �����n�[�h�E�F�A���g�p�\�ȏ�Ԃɂ���B
                ������Ԃ�READY��Ԃ���STOP��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                miscFlags   -   ���������̃t���O

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_EnableEx(WMCallbackFunc callback, u32 miscFlags)
{
    WMErrCode result;

    // READY�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_READY);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_ENABLE, callback);

    // ARM7��FIFO�Œʒm
    {
        WMArm9Buf *p = WMi_GetSystemWork();

        result = WMi_SendCommand(WM_APIID_ENABLE, 4,
                                 (u32)(p->WM7), (u32)(p->status), (u32)(p->fifo7to9), miscFlags);
        WM_CHECK_RESULT(result);
    }

    // ����I��
    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_Disable

  Description:  �����n�[�h�E�F�A���g�p�֎~��Ԃɂ���B
                ������Ԃ�STOP��Ԃ���READY��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Disable(WMCallbackFunc callback)
{
    WMErrCode result;

    // STOP�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_STOP);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_DISABLE, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_DISABLE, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_PowerOn

  Description:  �����n�[�h�E�F�A���N������B
                ������Ԃ�STOP��Ԃ���IDLE��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_PowerOn(WMCallbackFunc callback)
{
    WMErrCode result;

    // STOP�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_STOP);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_POWER_ON, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_POWER_ON, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_PowerOff

  Description:  �����n�[�h�E�F�A���V���b�g�_�E������B
                ������Ԃ�IDLE��Ԃ���STOP��ԂɑJ�ڂ����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_PowerOff(WMCallbackFunc callback)
{
    WMErrCode result;

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_POWER_OFF, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_POWER_OFF, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_Initialize

  Description:  WM�������������s���B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                dmaNo       -   WM���g�p����DMA�ԍ��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Initialize(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo)
{
    return WMi_InitializeEx(wmSysBuf, callback, dmaNo, 0);
}

/*---------------------------------------------------------------------------*
  Name:         WM_InitializeForListening

  Description:  WM�������������s���B
                �d�g�𑗏o����I�y���[�V�����͂ł��Ȃ��B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                dmaNo       -   WM���g�p����DMA�ԍ��B
                blink       -   LED ��_�ł����邩�ǂ���

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_InitializeForListening(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo, BOOL blink)
{
    u32 miscFlags = WM_MISC_FLAG_LISTEN_ONLY;

    if (!blink)
    {
        miscFlags |= WM_MISC_FLAG_NO_BLINK;
    }

    return WMi_InitializeEx(wmSysBuf, callback, dmaNo, miscFlags);
}

/*---------------------------------------------------------------------------*
  Name:         WMi_InitializeEx

  Description:  WM�������������s���B

  Arguments:    wmSysBuf    -   �Ăяo�����ɂ���Ċm�ۂ��ꂽ�o�b�t�@�ւ̃|�C���^�B
                                �o�b�t�@�̃T�C�Y��WM_SYSTEM_BUF_SIZE�����K�v�B
                callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                dmaNo       -   WM���g�p����DMA�ԍ��B
                miscFlags   -   ���������̃t���O

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_InitializeEx(void *wmSysBuf, WMCallbackFunc callback, u16 dmaNo, u32 miscFlags)
{
    WMErrCode result;

    // �����������s
    result = WM_Init(wmSysBuf, dmaNo);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_INITIALIZE, callback);

    // ARM7��FIFO�Œʒm
    {
        WMArm9Buf *p = WMi_GetSystemWork();

        result = WMi_SendCommand(WM_APIID_INITIALIZE, 4,
                                 (u32)(p->WM7), (u32)(p->status), (u32)(p->fifo7to9), miscFlags);
        WM_CHECK_RESULT(result);
    }

    // ����I��
    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_Reset

  Description:  �������C�u���������Z�b�g���A����������̏�Ԃɖ߂��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Reset(WMCallbackFunc callback)
{
    WMErrCode result;

    // �����n�[�h�N���ς݂��m�F
    result = WMi_CheckIdle();
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_RESET, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_RESET, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_End

  Description:  �������C�u�������I������B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_End(WMCallbackFunc callback)
{
    WMErrCode result;

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_END, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_END, 0);
    WM_CHECK_RESULT(result);

    // ARM9��WM���C�u�����̏I�������̓R�[���o�b�N���ōs��

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_SetParentParameter

  Description:  �e�@����ݒ肷��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                pparaBuf    -   �e�@���������\���̂ւ̃|�C���^�B
                                pparaBuf�y��pparaBuf->userGameInfo�̎��̂͋����I��
                                �L���b�V���X�g�A�����_�ɒ��ӁB

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_SetParentParameter(WMCallbackFunc callback, const WMParentParam *pparaBuf)
{
    WMErrCode result;

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (pparaBuf == NULL)
    {
        WM_WARNING("Parameter \"pparaBuf\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)pparaBuf & 0x01f)
    {
        // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
        WM_WARNING("Parameter \"pparaBuf\" is not 32-byte aligned.\n");
    }
    if (pparaBuf->userGameInfoLength > 0)
    {
        if (pparaBuf->userGameInfo == NULL)
        {
            WM_WARNING("Parameter \"pparaBuf->userGameInfo\" must not be NULL.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }
        if ((u32)(pparaBuf->userGameInfo) & 0x01f)
        {
            // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
            WM_WARNING("Parameter \"pparaBuf->userGameInfo\" is not 32-byte aligned.\n");
        }
    }

    // �]���f�[�^�ő咷���`�F�b�N
    if ((pparaBuf->parentMaxSize +
         (pparaBuf->KS_Flag ? WM_SIZE_KS_PARENT_DATA + WM_SIZE_MP_PARENT_PADDING : 0) >
         WM_SIZE_MP_DATA_MAX)
        || (pparaBuf->childMaxSize +
            (pparaBuf->KS_Flag ? WM_SIZE_KS_CHILD_DATA + WM_SIZE_MP_CHILD_PADDING : 0) >
            WM_SIZE_MP_DATA_MAX))
    {
        WM_WARNING("Transfer data size is over %d byte.\n", WM_SIZE_MP_DATA_MAX);
        return WM_ERRCODE_INVALID_PARAM;
    }
    (void)WmCheckParentParameter(pparaBuf);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_SET_P_PARAM, callback);

    // �w��o�b�t�@�̃L���b�V���������o��
    DC_StoreRange((void *)pparaBuf, WM_PARENT_PARAM_SIZE);
    if (pparaBuf->userGameInfoLength > 0)
    {
        DC_StoreRange(pparaBuf->userGameInfo, pparaBuf->userGameInfoLength);
    }

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_SET_P_PARAM, 1, (u32)pparaBuf);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WmCheckParentParameter

  Description:  �e�@�ݒ�p�����[�^���K�C�h���C���Ɏ�����鋖�e�͈͓����ǂ���
                ���`�F�b�N����f�o�b�O�p�֐��B

  Arguments:    param   -   �`�F�b�N����e�@�ݒ�p�����[�^�ւ̃|�C���^�B

  Returns:      BOOL    -   ���Ȃ��ꍇ��TRUE���A���e�ł��Ȃ��ݒ�l�̏ꍇ��
                            FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL WmCheckParentParameter(const WMParentParam *param)
{
    // ���[�U�[�Q�[������ 112 �o�C�g�܂�
    if (param->userGameInfoLength > WM_SIZE_USER_GAMEINFO)
    {
        OS_TWarning("User gameInfo length must be less than %d .\n", WM_SIZE_USER_GAMEINFO);
        return FALSE;
    }
    // �r�[�R�����M�Ԋu�� 10 �` 1000
    if ((param->beaconPeriod < 10) || (param->beaconPeriod > 1000))
    {
        OS_TWarning("Beacon send period must be between 10 and 1000 .\n");
        return FALSE;
    }
    // �ڑ��`�����l���� 1 �` 14
    if ((param->channel < 1) || (param->channel > 14))
    {
        OS_TWarning("Channel must be between 1 and 14 .\n");
        return FALSE;
    }
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WMi_StartParentEx

  Description:  �e�@�Ƃ��ĒʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                powerSave   -   �ȓd�̓��[�h���g�p����ꍇ��TRUE�A���Ȃ��ꍇ��FALSE�B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WMi_StartParentEx(WMCallbackFunc callback, BOOL powerSave)
{
    WMErrCode result;

    // IDLE�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    {
        WMArm9Buf *w9b = WMi_GetSystemWork();
#ifdef WM_DEBUG
        if (w9b->connectedAidBitmap != 0)
        {
            WM_DPRINTF("Warning: connectedAidBitmap should be 0, but %04x",
                       w9b->connectedAidBitmap);
        }
#endif
        w9b->myAid = 0;
        w9b->connectedAidBitmap = 0;
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_PARENT, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_START_PARENT, 1, (u32)powerSave);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StartParent

  Description:  �e�@�Ƃ��ĒʐM���J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartParent(WMCallbackFunc callback)
{
    return WMi_StartParentEx(callback, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         WM_EndParent

  Description:  �e�@�Ƃ��Ă̒ʐM���~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndParent(WMCallbackFunc callback)
{
    WMErrCode result;

    // PARENT�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_PARENT);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_END_PARENT, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_END_PARENT, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StartScan

  Description:  �q�@�Ƃ��Đe�@�̃X�L�������J�n����B
                ��x�̌Ăяo���ň��̐e�@�����擾����B
                WM_EndScan���Ăяo�����ɘA�����ČĂяo���Ă��ǂ��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                param       -   �X�L�������������\���̂ւ̃|�C���^�B
                                �X�L�������ʂ̏���param->scanBuf��ARM7�����ڏ����o��
                                �̂ŁA�L���b�V�����C���ɍ��킹�Ă����K�v������B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartScan(WMCallbackFunc callback, const WMScanParam *param)
{
    WMErrCode result;

    // IDLE CALSS1 SCAN �X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckStateEx(3, WM_STATE_IDLE, WM_STATE_CLASS1, WM_STATE_SCAN);
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (param == NULL)
    {
        WM_WARNING("Parameter \"param\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanBuf == NULL)
    {
        WM_WARNING("Parameter \"param->scanBuf\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((param->channel < 1) || (param->channel > 14))
    {
        WM_WARNING("Parameter \"param->channel\" must be between 1 and 14.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)(param->scanBuf) & 0x01f)
    {
        // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
        WM_WARNING("Parameter \"param->scanBuf\" is not 32-byte aligned.\n");
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_SCAN, callback);

    // ARM7��FIFO�Œʒm
    {
        WMStartScanReq Req;

        Req.apiid = WM_APIID_START_SCAN;
        Req.channel = param->channel;
        Req.scanBuf = param->scanBuf;
        Req.maxChannelTime = param->maxChannelTime;
        Req.bssid[0] = param->bssid[0];
        Req.bssid[1] = param->bssid[1];
        Req.bssid[2] = param->bssid[2];
        Req.bssid[3] = param->bssid[3];
        Req.bssid[4] = param->bssid[4];
        Req.bssid[5] = param->bssid[5];
        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        WM_CHECK_RESULT(result);
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StartScanEx

  Description:  �q�@�Ƃ��Đe�@�̃X�L�������J�n����B
                ��x�̌Ăяo���ŕ�����̐e�@�����擾����B
                WM_EndScan���Ăяo�����ɘA�����ČĂяo���Ă��ǂ��B

  Arguments:    callback - �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                param    - �X�L�������������\���̂ւ̃|�C���^�B
                           �X�L�������ʂ̏���param->scanBuf��ARM7�����ڏ����o��
                           �̂ŁA�L���b�V�����C���ɍ��킹�Ă����K�v������B

  Returns:      int      - WM_ERRCODE_*�^�̏������ʂ�Ԃ��B
 *---------------------------------------------------------------------------*/
WMErrCode WM_StartScanEx(WMCallbackFunc callback, const WMScanExParam *param)
{
    WMErrCode result;

    // IDLE CALSS1 SCAN �X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckStateEx(3, WM_STATE_IDLE, WM_STATE_CLASS1, WM_STATE_SCAN);
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (param == NULL)
    {
        WM_WARNING("Parameter \"param\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanBuf == NULL)
    {
        WM_WARNING("Parameter \"param->scanBuf\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanBufSize > WM_SIZE_SCAN_EX_BUF)
    {
        WM_WARNING
            ("Parameter \"param->scanBufSize\" must be less than or equal to WM_SIZE_SCAN_EX_BUF.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)(param->scanBuf) & 0x01f)
    {
        // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
        WM_WARNING("Parameter \"param->scanBuf\" is not 32-byte aligned.\n");
    }
    if (param->ssidLength > WM_SIZE_SSID)
    {
        WM_WARNING("Parameter \"param->ssidLength\" must be less than or equal to WM_SIZE_SSID.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanType != WM_SCANTYPE_ACTIVE && param->scanType != WM_SCANTYPE_PASSIVE
        && param->scanType != WM_SCANTYPE_ACTIVE_CUSTOM
        && param->scanType != WM_SCANTYPE_PASSIVE_CUSTOM)
    {
        WM_WARNING
            ("Parameter \"param->scanType\" must be WM_SCANTYPE_PASSIVE or WM_SCANTYPE_ACTIVE.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((param->scanType == WM_SCANTYPE_ACTIVE_CUSTOM
         || param->scanType == WM_SCANTYPE_PASSIVE_CUSTOM) && param->ssidMatchLength > WM_SIZE_SSID)
    {
        WM_WARNING
            ("Parameter \"param->ssidMatchLength\" must be less than or equal to WM_SIZE_SSID.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_SCAN_EX, callback);

    // ARM7��FIFO�Œʒm
    {
        WMStartScanExReq Req;

        Req.apiid = WM_APIID_START_SCAN_EX;
        Req.channelList = param->channelList;
        Req.scanBuf = param->scanBuf;
        Req.scanBufSize = param->scanBufSize;
        Req.maxChannelTime = param->maxChannelTime;
        MI_CpuCopy8(param->bssid, Req.bssid, WM_SIZE_MACADDR);
        Req.scanType = param->scanType;
        Req.ssidMatchLength = param->ssidMatchLength;
        Req.ssidLength = param->ssidLength;
        MI_CpuCopy8(param->ssid, Req.ssid, WM_SIZE_SSID);

        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        WM_CHECK_RESULT(result);
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_EndScan

  Description:  �q�@�Ƃ��ẴX�L�����������~����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_EndScan(WMCallbackFunc callback)
{
    WMErrCode result;

    // SCAN�X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_SCAN);
    WM_CHECK_RESULT(result);

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_END_SCAN, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_END_SCAN, 0);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_StartConnectEx

  Description:  �q�@�Ƃ��Đe�@�ւ̐ڑ����J�n����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                pInfo       -   �ڑ�����e�@�̏��B
                                WM_StartScan�ɂĎ擾�����\���̂��w�肷��B
                                ���̍\���̂̎��̂͋����I�ɃL���b�V���X�g�A
                                �����_�ɒ��ӁB
                ssid        -   �e�@�ɒʒm����q�@���(24Byte(WM_SIZE_CHILD_SSID)�Œ�T�C�Y)
                powerSave   -   �ȓd�̓��[�h���g�p����ꍇ��TRUE�A���Ȃ��ꍇ��FALSE�B
                authMode    -   Authentication�̃��[�h�I���B
                                  WM_AUTHMODE_OPEN_SYSTEM : OPEN SYSTEM���[�h
                                  WM_AUTHMODE_SHARED_KEY  : SHARED KEY ���[�h

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode
WM_StartConnectEx(WMCallbackFunc callback, const WMBssDesc *pInfo, const u8 *ssid,
                  BOOL powerSave, const u16 authMode)
{
    WMErrCode result;

    // IDLE �X�e�[�g�ȊO�ł͎��s�s��
    result = WMi_CheckState(WM_STATE_IDLE);
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    if (pInfo == NULL)
    {
        WM_WARNING("Parameter \"pInfo\" must not be NULL.\n");
        return WM_ERRCODE_INVALID_PARAM;
    }
    if ((u32)pInfo & 0x01f)
    {
        // �A���C���`�F�b�N�͌x���݂̂ŃG���[�ɂ͂��Ȃ�
        WM_WARNING("Parameter \"pInfo\" is not 32-byte aligned.\n");
    }
    if ((authMode != WM_AUTHMODE_OPEN_SYSTEM) && (authMode != WM_AUTHMODE_SHARED_KEY))
    {
        WM_WARNING
            ("Parameter \"authMode\" must be WM_AUTHMODE_OPEN_SYSTEM or WM_AUTHMODE_SHARED_KEY.\n");
    }

    // �w��o�b�t�@�̃L���b�V���������o��
    DC_StoreRange((void *)pInfo, (u32)(pInfo->length * 2));

    {
        WMArm9Buf *w9b = WMi_GetSystemWork();
#ifdef WM_DEBUG
        if (w9b->connectedAidBitmap != 0)
        {
            WM_DPRINTF("Warning: connectedAidBitmap should be 0, but %04x",
                       w9b->connectedAidBitmap);
        }
#endif
        w9b->myAid = 0;
        w9b->connectedAidBitmap = 0;
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_START_CONNECT, callback);

    // ARM7��FIFO�Œʒm
    {
        WMStartConnectReq Req;

        Req.apiid = WM_APIID_START_CONNECT;
        Req.pInfo = (WMBssDesc *)pInfo;
        if (ssid != NULL)
        {
            MI_CpuCopy8(ssid, Req.ssid, WM_SIZE_CHILD_SSID);
        }
        else
        {
            MI_CpuClear8(Req.ssid, WM_SIZE_CHILD_SSID);
        }
        Req.powerSave = powerSave;
        Req.authMode = authMode;

        result = WMi_SendCommandDirect(&Req, sizeof(Req));
        WM_CHECK_RESULT(result);
    }

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_Disconnect

  Description:  �m������Ă���ڑ���ؒf����B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                aid         -   �ؒf����ʐM�����AID�B
                                �e�@�̏ꍇ�AID��1�`15�̎q�@���ʂɐؒf�B
                                �q�@�̏ꍇ�AID��0�̐e�@�Ƃ̒ʐM���I���B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_Disconnect(WMCallbackFunc callback, u16 aid)
{
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();

    // �X�e�[�g�`�F�b�N
    result = WMi_CheckStateEx(5,
                              WM_STATE_PARENT, WM_STATE_MP_PARENT,
                              WM_STATE_CHILD, WM_STATE_MP_CHILD, WM_STATE_DCF_CHILD);
    WM_CHECK_RESULT(result);

    if (                               // p->status->state�̃L���b�V���͔j���ς�
           (p->status->state == WM_STATE_PARENT) || (p->status->state == WM_STATE_MP_PARENT))
    {
        // �e�@�̏ꍇ
        if ((aid < 1) || (aid > WM_NUM_MAX_CHILD))
        {
            WM_WARNING("Parameter \"aid\" must be between 1 and %d.\n", WM_NUM_MAX_CHILD);
            return WM_ERRCODE_INVALID_PARAM;
        }
        DC_InvalidateRange(&(p->status->child_bitmap), 2);
        if (!(p->status->child_bitmap & (0x0001 << aid)))
        {
            WM_WARNING("There is no child that have aid %d.\n", aid);
            return WM_ERRCODE_NO_CHILD;
        }
    }
    else
    {
        // �q�@�̏ꍇ
        if (aid != 0)
        {
            WM_WARNING("Now child mode , so aid must be 0.\n");
            return WM_ERRCODE_INVALID_PARAM;
        }
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_DISCONNECT, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_DISCONNECT, 1, (u32)(0x0001 << aid));
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
  Name:         WM_DisconnectChildren

  Description:  �ڑ��m������Ă���q�@�����ꂼ��ؒf����B�e�@��p�֐��B

  Arguments:    callback    -   �񓯊������������������ɌĂяo�����R�[���o�b�N�֐��B
                aidBitmap   -   �ؒf����q�@��AID�r�b�g�t�B�[���h�B
                                �ŉ��ʃr�b�g�͖�������Abit 1�`15 ��AID 1�`15 �̎q�@
                                �����ꂼ�ꎦ���B
                                �ڑ�����Ă��Ȃ��q�@�������r�b�g�͖��������̂ŁA
                                �ڑ��󋵂Ɋւ�炸�S�q�@��ؒf����ꍇ��0xFFFF���w��B

  Returns:      WMErrCode   -   �������ʂ�Ԃ��B�񓯊�����������ɊJ�n���ꂽ�ꍇ��
                                WM_ERRCODE_OPERATING���Ԃ���A���̌�R�[���o�b�N��
                                �񓯊������̌��ʂ����߂ēn�����B
 *---------------------------------------------------------------------------*/
WMErrCode WM_DisconnectChildren(WMCallbackFunc callback, u16 aidBitmap)
{
    WMErrCode result;
    WMArm9Buf *p = WMi_GetSystemWork();

    // �X�e�[�g�`�F�b�N
    result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);
    WM_CHECK_RESULT(result);

    // �p�����[�^�`�F�b�N
    DC_InvalidateRange(&(p->status->child_bitmap), 2);
    if (!(p->status->child_bitmap & aidBitmap & 0xfffe))
    {
        WM_WARNING("There is no child that is included in \"aidBitmap\" %04x_.\n", aidBitmap);
        return WM_ERRCODE_NO_CHILD;
    }

    // �R�[���o�b�N�֐���o�^
    WMi_SetCallbackTable(WM_APIID_DISCONNECT, callback);

    // ARM7��FIFO�Œʒm
    result = WMi_SendCommand(WM_APIID_DISCONNECT, 1, (u32)aidBitmap);
    WM_CHECK_RESULT(result);

    return WM_ERRCODE_OPERATING;
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
