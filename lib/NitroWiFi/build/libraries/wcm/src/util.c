/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - libraries
  File:     util.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util.c,v $
  Revision 1.3  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/10/05 08:51:45  terui
  �d�g���x������o�b�t�@�ɒ~�ς���֐��̒ǉ��B
  WCM_GetLinkLevel�֐��ɂ��擾���郊���N���x��������o�b�t�@�̃f�[�^�����Ɍv�Z����悤�ɉ����B

  Revision 1.1  2005/09/10 03:08:43  terui
  Initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include "wcm_private.h"

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define WCM_NETWORK_CAPABILITY_KIND_MASK        0x0003
#define WCM_NETWORK_CAPABILITY_KIND_ESS         0x0001
#define WCM_NETWORK_CAPABILITY_KIND_IBSS        0x0002
#define WCM_NETWORK_CAPABILITY_PRIVACY          0x0010
#define WCM_NETWORK_CAPABILITY_PREAMBLE_MASK    0x0020
#define WCM_NETWORK_CAPABILITY_PREAMBLE_LONG    0x0000
#define WCM_NETWORK_CAPABILITY_PREAMBLE_SHORT   0x0020

#define WCM_RSSI_COUNT_MAX                      16
#define WCM_RSSI_BORDER_HIGH                    28
#define WCM_RSSI_BORDER_MIDDLE                  22
#define WCM_RSSI_BORDER_LOW                     16

/*---------------------------------------------------------------------------*
    ���[�J���֐��v���g�^�C�v
 *---------------------------------------------------------------------------*/
static WMLinkLevel  WcmGetLinkLevel(void);

/*---------------------------------------------------------------------------*
    �ϐ���`
 *---------------------------------------------------------------------------*/
static u8       wcmRssi[WCM_RSSI_COUNT_MAX];
static u8       wcmRssiIndex = 0;

/*---------------------------------------------------------------------------*
  Name:         WCM_CompareBssID

  Description:  �Q�� BSSID ���r����B�����֐��B

  Arguments:    a       -   ��r�Ώۂ� BSSID �ւ̃|�C���^���w�肷��B
                b       -   ��r�Ώۂ� BSSID �ւ̃|�C���^���w�肷��B

  Returns:      BOOL    -   �Q�� BSSID �������Ȃ�� TRUE ��Ԃ��B
                            �قȂ� BSSID �ł���ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_CompareBssID(u8* a, u8* b)
{
    s32 i;

    for (i = 0; i < WCM_BSSID_SIZE; i++)
    {
        if (a[i] != b[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_GetLinkLevel

  Description:  DCF �ʐM���̃����N���x���擾����B�����֐��B

  Arguments:    None.

  Returns:      WMLinkLevel -   4�i�K�ɕ]�����������N���x��Ԃ��B
 *---------------------------------------------------------------------------*/
WMLinkLevel WCM_GetLinkLevel(void)
{
    OSIntrMode  e = OS_DisableInterrupts();
    WCMWork*    w = WCMi_GetSystemWork();
    WMLinkLevel ret = WM_LINK_LEVEL_0;

    // �������m�F
    if (w != NULL)
    {
        if (w->phase == WCM_PHASE_DCF)
        {
            ret = WcmGetLinkLevel();
        }
    }

    (void)OS_RestoreInterrupts(e);
    return ret;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_IsInfrastructureNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃C���t���X�g���N�`���l�b�g���[�N
                �̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �C���t���X�g���N�`���l�b�g���[�N�ł���ꍇ�� TRUE ���A
                            �����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_IsInfrastructureNetwork(WMBssDesc* bssDesc)
{
    if (bssDesc != NULL)
    {
        if ((bssDesc->capaInfo & WCM_NETWORK_CAPABILITY_KIND_MASK) == WCM_NETWORK_CAPABILITY_KIND_ESS)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_IsAdhocNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃A�h�z�b�N�l�b�g���[�N�̂���
                �ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �A�h�z�b�N�l�b�g���[�N�ł���ꍇ�� TRUE ���A
                            �����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_IsAdhocNetwork(WMBssDesc* bssDesc)
{
    if (bssDesc != NULL)
    {
        if ((bssDesc->capaInfo & WCM_NETWORK_CAPABILITY_KIND_MASK) == WCM_NETWORK_CAPABILITY_KIND_IBSS)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_IsPrivacyNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��� WEP �Z�L�����e�B�̓K�p���ꂽ
                �l�b�g���[�N�̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   WEP �Z�L�����e�B�̓K�p���ꂽ�l�b�g���[�N�ł���ꍇ��
                            TRUE ���A�����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_IsPrivacyNetwork(WMBssDesc* bssDesc)
{
    if (bssDesc != NULL)
    {
        if (bssDesc->capaInfo & WCM_NETWORK_CAPABILITY_PRIVACY)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_IsShortPreambleNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃V���[�g�v���A���u�����g�p����
                �l�b�g���[�N�̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �V���[�g�v���A���u�����g�p�����l�b�g���[�N�ł���ꍇ��
                            TRUE ���A�����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_IsShortPreambleNetwork(WMBssDesc* bssDesc)
{
    if (bssDesc != NULL)
    {
        if ((bssDesc->capaInfo & WCM_NETWORK_CAPABILITY_PREAMBLE_MASK) == WCM_NETWORK_CAPABILITY_PREAMBLE_SHORT)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_IsLongPreambleNetwork

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��񂪃����O�v���A���u�����g�p����
                �l�b�g���[�N�̂��̂ł��邩�ǂ����𒲂ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   �����O�v���A���u�����g�p�����l�b�g���[�N�ł���ꍇ��
                            TRUE ���A�����łȂ��ꍇ�� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_IsLongPreambleNetwork(WMBssDesc* bssDesc)
{
    if (bssDesc != NULL)
    {
        if ((bssDesc->capaInfo & WCM_NETWORK_CAPABILITY_PREAMBLE_MASK) == WCM_NETWORK_CAPABILITY_PREAMBLE_LONG)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WCM_IsEssidHidden

  Description:  AP �T�[�`���ʂƂ��ē���ꂽ��� ESS-ID ���B�����Ă��邩�ǂ���
                ���ׂ�B

  Arguments:    bssDesc -   ���ׂ� AP ��� ( �l�b�g���[�N��� )�B

  Returns:      BOOL    -   ESS-ID ���B������Ă���ꍇ�� TRUE ��Ԃ��B
                            �L���� ESS-ID �����J����Ă���ꍇ�ɂ� FALSE ��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL WCM_IsEssidHidden(WMBssDesc* bssDesc)
{
    if (bssDesc != NULL)
    {
        s32 i;

        if (bssDesc->ssidLength == 0)
        {
            return TRUE;    // ���� 0 �̏ꍇ�X�e���X
        }

        for (i = 0; (i < bssDesc->ssidLength) && (i < WM_SIZE_SSID); i++)
        {
            if (bssDesc->ssid[i] != 0x00)
            {
                return FALSE;
            }
        }

        return TRUE;        // �����Ԃ�S�� 0x00 �̏ꍇ�X�e���X
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WCMi_GetRssiAverage

  Description:  �����o�b�t�@�ɒ~�ς���Ă����M�d�g���x�̗�������A���ς�
                RSSI �l���擾����B

  Arguments:    None.

  Returns:      u8      -   ���� RSSI �l��Ԃ��B�����̕��ςȂ̂ŁA�ڑ������
                            ���Ȃ��ꍇ�Ȃǂł��ߋ��̗��������ɗL���Ȓl��Ԃ�
                            ���Ƃɒ��ӁB
 *---------------------------------------------------------------------------*/
u8 WCMi_GetRssiAverage(void)
{
    s32 sum = 0;
    s32 i;

    if (wcmRssiIndex > WCM_RSSI_COUNT_MAX)
    {
        for (i = 0; i < WCM_RSSI_COUNT_MAX; i++)
        {
            sum += wcmRssi[i];
        }

        sum /= WCM_RSSI_COUNT_MAX;
    }
    else if (wcmRssiIndex > 0)
    {
        for (i = 0; i < wcmRssiIndex; i++)
        {
            sum += wcmRssi[i];
        }

        sum /= wcmRssiIndex;
    }

    return(u8) sum;
}

/*---------------------------------------------------------------------------*
  Name:         WcmGetLinkLevel

  Description:  �����o�b�t�@�ɒ~�ς���Ă����M�d�g���x�̗��������ɂ��������N
                ���x�����擾����B

  Arguments:    None.

  Returns:      WMLinkLevel -   �����N���x����Ԃ��B
 *---------------------------------------------------------------------------*/
static WMLinkLevel WcmGetLinkLevel(void)
{
    u8          ave = WCMi_GetRssiAverage();
    WMLinkLevel level = WM_LINK_LEVEL_0;

    // sum ���� level ���v�Z
    if (ave >= WCM_RSSI_BORDER_HIGH)
    {
        level = WM_LINK_LEVEL_3;
    }
    else if (ave >= WCM_RSSI_BORDER_MIDDLE)
    {
        level = WM_LINK_LEVEL_2;
    }
    else if (ave >= WCM_RSSI_BORDER_LOW)
    {
        level = WM_LINK_LEVEL_1;
    }

    return level;
}

/*---------------------------------------------------------------------------*
  Name:         WCMi_ShelterRssi

  Description:  DCF �f�[�^����M�����ۂɁA��M�d�g���x�� RSSI �l������o�b�t�@
                �ɒ~�ς���B

  Arguments:    rssi    -   ��M�d�g���x�� RSSI �l���w�肷��B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WCMi_ShelterRssi(u8 rssi)
{
    u8  raw;

    if (rssi & 0x02)
    {
        raw = (u8) (rssi >> 2);
    }
    else
    {
        raw = (u8) (25 + (rssi >> 2));
    }

    wcmRssi[wcmRssiIndex % WCM_RSSI_COUNT_MAX] = raw;
    if (wcmRssiIndex >= WCM_RSSI_COUNT_MAX)
    {
        wcmRssiIndex = (u8) (((1 + wcmRssiIndex) % WCM_RSSI_COUNT_MAX) + WCM_RSSI_COUNT_MAX);
    }
    else
    {
        wcmRssiIndex++;
    }
}

/*---------------------------------------------------------------------------*
    End of file
 *---------------------------------------------------------------------------*/
