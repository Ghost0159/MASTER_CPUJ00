/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CHT - demos - wmscan
  File:     chtmin.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wmscan.c,v $
  Revision 1.4  2007/10/29 00:29:31  seiki_masashi
  Copyright date

  Revision 1.3  2007/10/22 11:12:36  seiki_masashi
  �s�v�ȏ����̏���

  Revision 1.2  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.1  2005/07/27 07:43:35  seiki_masashi
  �V�K�ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include    <nitro.h>
#include    <nitro/cht.h>
#include    "wmscan.h"


static int sSysState = WS_SYSSTATE_STOP;        // wmscan�������
static WSStartScanCallbackFunc sScanCallback = NULL;    // �e�@�������̃R�[���o�b�N�|�C���^
static WMBssDesc sBssDesc ATTRIBUTE_ALIGN(32);  // �e�@�f�[�^�擾�p�o�b�t�@
static WMScanParam sScanParam ATTRIBUTE_ALIGN(32);      // �X�L�����p�����[�^�\����
static BOOL sContinuousScanFlag;       // �A���X�L�����t���O
static BOOL sPictoCatchFlag = FALSE;   // �s�N�g�L���b�`�t���O
static u32 sGgid = 0;                  // GGID

#define WS_ASSERT(exp) \
    (void) ((exp) || (OSi_Panic(__FILE__, __LINE__, "Failed assertion " #exp), 0))

static void WS_ChangeSysState(int state);
static void WS_StateOutInitialize(void *arg);
static BOOL WS_StateInStartScan(void);
static void WS_StateOutStartScan(void *arg);
static BOOL WS_StateInEndScan(void);
static void WS_StateOutEndScan(void *arg);
static void WS_StateOutEnd(void *arg);


/*---------------------------------------------------------------------------*
  Name:         WS_ChangeSysState
  Description:  WS��Ԃ̕ύX
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void WS_ChangeSysState(int state)
{
    sSysState = state;
}

/*---------------------------------------------------------------------------*
  Name:         WS_GetSystemState

  Description:  WS��Ԃ��擾���܂��B

  Arguments:    None.

  Returns:      WS���.
 *---------------------------------------------------------------------------*/
int WS_GetSystemState(void)
{
    return sSysState;
}

/*---------------------------------------------------------------------------*
  Name:         WS_Initialize

  Description:  ���������������܂��B

  Arguments:    buf   WM�ɓn���o�b�t�@�T�C�Y�ł��BWM_SYSTEM_BUF_SIZE�̗̈悪�K�v�ł��B
                dmaNo �����Ŏg�p�����DMA�ԍ����w�肵�܂��B

  Returns:      ����������ɊJ�n���ꂽ�ꍇ�ɂ� TRUE
                �J�n����Ȃ������ꍇ�ɂ͏ꍇ�ɂ� FALSE
 *---------------------------------------------------------------------------*/
BOOL WS_Initialize(void *buf, u16 dmaNo)
{
    WMErrCode result;

    SDK_NULL_ASSERT(buf);

    WS_ChangeSysState(WS_SYSSTATE_BUSY);
    result = WM_Initialize(buf, WS_StateOutInitialize, dmaNo);
    if (result != WM_ERRCODE_OPERATING)
    {
        WS_ChangeSysState(WS_SYSSTATE_FATAL);
        return FALSE;
    }
    sScanParam.channel = 0;
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         WS_StateOutInitialize
  Description:  �����̏����������s���܂��B
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
static void WS_StateOutInitialize(void *arg)
{
    // �d���������Ԃł��B
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WS_ChangeSysState(WS_SYSSTATE_FATAL);
        return;
    }

    // �V�X�e����Ԃ��A�C�h�����O�i�ҋ@���j�ɕύX�B
    WS_ChangeSysState(WS_SYSSTATE_IDLE);

    // ���̏�Ԃ��Z�b�g���Ȃ��̂ŁA�����ŃV�[�P���X�͂�������I���ł��B
}

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOnPictoCatch

  Description:  �s�N�g�L���b�`�@�\��L���ɂ���B
                WS_StartScan�ɂăX�L�������ɁA�s�N�g�`���b�g�𔭌������ꍇ�ɂ�
                �R�[���o�b�N�֐����Ă΂��悤�ɂȂ�B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WS_TurnOnPictoCatch(void)
{
    sPictoCatchFlag = TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOffPictoCatch

  Description:  �s�N�g�L���b�`�@�\�𖳌��ɂ���B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WS_TurnOffPictoCatch(void)
{
    sPictoCatchFlag = FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WS_SetGgid

  Description:  �Q�[���O���[�vID��ݒ肵�܂��B
                �e�@�̐ڑ��O�ɌĂяo���܂��B

  Arguments:    ggid    �ݒ肷��Q�[���O���[�vID.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WS_SetGgid(u32 ggid)
{
    sGgid = ggid;
}


/*---------------------------------------------------------------------------*
  Name:         WS_StartScan

  Description:  �e�@�̃X�L�������J�n���܂��B

  Arguments:    callback   �e�@�������̃R�[���o�b�N
                macAddr    �e�@��MAC�A�h���X���w�肵�܂��B
                           �S�e�@����������ꍇ�� FF:FF:FF:FF:FF:FF���w�肵�܂�
                continuous ���̃t���O��TRUE�̏ꍇ�́AWS_EndScan���R�[�������܂ł̊�
                           �L���ȑS�`�����l�����X�L�����������܂��B
                           FALSE�̏ꍇ�́A�L���ȃ`�����l�������[�e�[�V�������Ȃ���A
                           �P�X�L�������ɃX�L�������~���AWS_SYSSTATE_SCANIDLE��ԂɑJ�ڂ��܂��B

  Returns:      ����������ɊJ�n���ꂽ�ꍇ�ɂ� TRUE
                �J�n����Ȃ������ꍇ�ɂ͏ꍇ�ɂ� FALSE
 *---------------------------------------------------------------------------*/
BOOL WS_StartScan(WSStartScanCallbackFunc callback, const u8 *macAddr, BOOL continuous)
{
    OSIntrMode enabled;

    enabled = OS_DisableInterrupts();

    sScanCallback = callback;
    sContinuousScanFlag = continuous;

    // ��������MAC�A�h���X�̏�����ݒ�
    *(u16 *)(&sScanParam.bssid[4]) = *(u16 *)(macAddr + 4);
    *(u16 *)(&sScanParam.bssid[2]) = *(u16 *)(macAddr + 2);
    *(u16 *)(&sScanParam.bssid[0]) = *(u16 *)(macAddr);

    (void)OS_RestoreInterrupts(enabled);

    if (sSysState == WS_SYSSTATE_SCANNING)
    {
        return TRUE;
    }

    WS_ChangeSysState(WS_SYSSTATE_SCANNING);

    if (!WS_StateInStartScan())
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);
        return FALSE;
    }

    return TRUE;
}

/* ----------------------------------------------------------------------
  state : StartScan
  ---------------------------------------------------------------------- */
static BOOL WS_StateInStartScan(void)
{
    // ���̏�Ԃ̎��A�e�@��T�����܂��B
    WMErrCode result;
    u16     chanpat;

    WS_ASSERT(sSysState == WS_SYSSTATE_SCANNING);

    chanpat = WM_GetAllowedChannel();

    // �������g�p�\���ǂ������`�F�b�N����
    if (chanpat == 0x8000)
    {
        // 0x8000 ���Ԃ��Ă����ꍇ�́A����������������Ă��Ȃ��Ȃ�
        // �������C�u�����̏�Ԉُ��\���Ă���̂ŃG���[�ɂ��܂��B
        return FALSE;
    }
    if (chanpat == 0)
    {
        // �������g���Ȃ���ԁB
        return FALSE;
    }

    /* ���݂̎w�肩�珸���ɁA�\�ȃ`�����l�����������܂� */
    while (TRUE)
    {
        sScanParam.channel++;
        if (sScanParam.channel > 16)
        {
            sScanParam.channel = 1;
        }

        if (chanpat & (0x0001 << (sScanParam.channel - 1)))
        {
            break;
        }
    }

    sScanParam.maxChannelTime = WM_GetDispersionScanPeriod();
    sScanParam.scanBuf = &sBssDesc;
    result = WM_StartScan(WS_StateOutStartScan, &sScanParam);

    if (result != WM_ERRCODE_OPERATING)
    {
        return FALSE;
    }
    return TRUE;
}

static void WS_StateOutStartScan(void *arg)
{
    WMstartScanCallback *cb = (WMstartScanCallback *)arg;

    // �X�L�����R�}���h�Ɏ��s�����ꍇ
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);
        return;
    }

    if (sSysState != WS_SYSSTATE_SCANNING)
    {
        // ��Ԃ��ύX����Ă���΃X�L�����I��
        if (!WS_StateInEndScan())
        {
            WS_ChangeSysState(WS_SYSSTATE_ERROR);
        }
        return;
    }

    switch (cb->state)
    {
    case WM_STATECODE_PARENT_NOT_FOUND:
        break;

    case WM_STATECODE_PARENT_FOUND:
        // �e�@�����������ꍇ
        // ggid ���r���A����Ă����玸�s�Ƃ��܂��B
        // �܂��AWMBssDesc.gameInfoLength ���m�F���A
        // ggid �ɗL���Ȓl�������Ă��邱�Ƃ��璲�ׂ�K�v������܂��B

        // BssDesc�̏��ARM7�����珑�����܂�Ă��邽��
        // �o�b�t�@�ɐݒ肳�ꂽBssDesc�̃L���b�V����j��
        DC_InvalidateRange(&sBssDesc, sizeof(WMbssDesc));

        // �e�@���s�N�g�`���b�g���ǂ�������
        if (sPictoCatchFlag)
        {
            if (CHT_IsPictochatParent(&sBssDesc))
            {
                // �e�@���s�N�g�`���b�g�ł���ꍇ
                if (sScanCallback != NULL)
                {
                    sScanCallback(&sBssDesc);
                }
                break;
            }
        }

        if (cb->gameInfoLength < 8 || cb->gameInfo.ggid != sGgid)
        {
            // GGID������Ă���Ζ�������
            break;
        }

        // �G���g���[�t���O�������Ă��Ȃ���Ύq�@����t���łȂ��̂Ŗ�������
        if (!(cb->gameInfo.gameNameCount_attribute & WM_ATTR_FLAG_ENTRY))
        {
            break;
        }

        // �R�[���o�b�N���K�v�Ȃ�ΌĂяo��
        if (sScanCallback != NULL)
        {
            sScanCallback(&sBssDesc);
        }

        break;
    }

    if (!sContinuousScanFlag)
    {
        WS_ChangeSysState(WS_SYSSTATE_SCANIDLE);
        return;
    }

    // �`�����l����ύX���čăX�L�������J�n���܂��B
    if (!WS_StateInStartScan())
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);
    }
}


/*---------------------------------------------------------------------------*
  Name:         WS_EndScan

  Description:  �X�L�������I������֐�

  Arguments:    None.

  Returns:      ����������ɊJ�n���ꂽ�ꍇ�ɂ� TRUE
                �J�n����Ȃ������ꍇ�ɂ͏ꍇ�ɂ� FALSE
 *---------------------------------------------------------------------------*/
BOOL WS_EndScan(void)
{
    if (sSysState == WS_SYSSTATE_SCANIDLE)
    {
        return WS_StateInEndScan();
    }

    if (sSysState != WS_SYSSTATE_SCANNING)
    {
        return FALSE;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();
        sScanCallback = NULL;
        (void)OS_RestoreInterrupts(enabled);
    }

    WS_ChangeSysState(WS_SYSSTATE_BUSY);
    return TRUE;
}


static BOOL WS_StateInEndScan(void)
{
    WMErrCode result;

    // ���̏�Ԃł́A�X�L�����̏I���������s���܂��B
    result = WM_EndScan(WS_StateOutEndScan);
    if (result != WM_ERRCODE_OPERATING)
    {
        return FALSE;
    }

    return TRUE;
}

static void WS_StateOutEndScan(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;

    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        return;
    }

    WS_ChangeSysState(WS_SYSSTATE_IDLE);
}


/*---------------------------------------------------------------------------*
  Name:         WS_End

  Description:  �����ʐM���I������B

  Arguments:    None.

  Returns:      ��������ΐ^�B
 *---------------------------------------------------------------------------*/
BOOL WS_End(void)
{
    WS_ASSERT(sSysState == WS_SYSSTATE_IDLE);

    WS_ChangeSysState(WS_SYSSTATE_BUSY);
    if (WM_End(WS_StateOutEnd) != WM_ERRCODE_OPERATING)
    {
        WS_ChangeSysState(WS_SYSSTATE_ERROR);

        return FALSE;
    }
    return TRUE;
}

/* ----------------------------------------------------------------------
  state : WS_StateOutEnd
  ---------------------------------------------------------------------- */
static void WS_StateOutEnd(void *arg)
{
    WMCallback *cb = (WMCallback *)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS)
    {
        WS_ChangeSysState(WS_SYSSTATE_FATAL);
        return;
    }
    WS_ChangeSysState(WS_SYSSTATE_STOP);
}
