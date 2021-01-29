/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     soc_cleanup.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_cleanup.c,v $
  Revision 1.6  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.5  2005/10/12 01:45:29  yasu
  DCF �R�[���o�b�N�̃L�����Z��

  Revision 1.4  2005/09/28 11:44:37  yasu
  �R�[�h�𐮗�
  SOCL_CalmDown() ��ǉ�
  CPS ���C�u�����̐����Ή����܂��Ȃ̂� SOC ���ŏ������J�o�[���Ă���

  Revision 1.3  2005/09/27 14:18:09  yasu
  SOC_Close �̔񓯊�����T�|�[�g

  Revision 1.2  2005/09/15 12:51:01  yasu
  DHCP Requested IP �T�|�[�g

  Revision 1.1  2005/08/18 13:17:59  yasu
  cleanup API ��ʃt�@�C���ֈړ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>
#include "../wcm/include/wcm_cpsif.h"

/*---------------------------------------------------------------------------*
  Name:         SOCL_Clearup

  Description:  SOC/CPS �\�P�b�g���C�u�������V���b�g�_�E�����܂��B

  Arguments:    �Ȃ�

  Returns:       0 ����I��
                -1 �S�Ẵ\�P�b�g�T�[�r�X����~���Ă��܂���
 *---------------------------------------------------------------------------*/
int SOCL_Cleanup(void)
{
    int result;

    SDK_ASSERT(SOCLiConfigPtr);

    // IP �A�h���X���ۑ�����Ă��Ȃ��Ȃ�ۑ����Ă���
    if (SOCLiRequestedIP == 0)
    {
        SOCLiRequestedIP = CPSMyIp;
    }

    // �S�Ẵ\�P�b�g���N���[�Y���Ă����D
    // SOCLiUDPSendSocket �����̏����ŃN���[�Y�����
    while (SOCL_EINPROGRESS == SOCL_CalmDown())
    {
        OS_Sleep(100);
    }

    result = SOCLi_CleanupCommandPacketQueue();

    if (result >= 0)
    {
#ifdef SDK_MY_DEBUG
        OS_TPrintf("CPS_Cleanup\n");
#endif

        // DHCP Release ���s�Ȃ����߂Ƀu���b�N����
        // �������������ꍇ�� SOCL_CalmDown() �ŏ�����i�߂Ă�������
        CPS_Cleanup();

        // �񓯊� Cleanup �΍�
        CPS_SetScavengerCallback(NULL);

        if (!SOCLiConfigPtr->lan_buffer)    // ���͊m�ۂ����̈�̊J��
        {
            SOCLi_Free(SOCLiCPSConfig.lan_buf);
        }

        SOCLiConfigPtr = NULL;
    }

    return result;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_CloseAll

  Description:  ���[�U�\�P�b�g��S�Ĕ񓯊��ŃN���[�Y����
                (����\�P�b�g�͎c��)
  
  Arguments:    �Ȃ�
  
  Returns:      SOCL_EINPROGRESS=-26 �Ȃ�N���[�Y������
                SOCL_ESUCCESS   = 0  �Ȃ犮��
 *---------------------------------------------------------------------------*/
int SOCL_CloseAll(void)
{
    SOCLSocket*     socket;
    OSIntrMode      enable;

    for (;;)
    {
        // �N���[�Y�Ăяo������ SocketList �̏�Ԃ��ω����邩������Ȃ��̂�
        // ����擪���猟������
        enable = OS_DisableInterrupts();
        for (socket = SOCLiSocketList; socket; socket = socket->next)
        {
            // �܂��N���[�Y�������n�܂��Ă��Ȃ���ʂ̃\�P�b�g�ł��邩�H
            if ((int)socket != SOCLiUDPSendSocket && !SOCL_SocketIsWaitingClose(socket))
            {
                break;
            }
        }
        (void)OS_RestoreInterrupts(enable);

        if (!socket)
        {
            break;
        }

        (void)SOCL_Close((int)socket);
    }

    // �\�P�b�g���X�g����A���邢�� UDPSend �\�P�b�g�݂̂ł���p�����X�g����Ȃ�I��
    if (SOCLiSocketList == NULL || ((int)SOCLiSocketList == SOCLiUDPSendSocket && SOCLiSocketList->next == NULL))
    {
        if (SOCLiSocketListTrash == NULL)
        {
            return SOCL_ESUCCESS;
        }
    }

    return SOCL_EINPROGRESS;
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_CalmDown

  Description:  �S�Ẵ\�P�b�g����ѕ⏕�\�P�b�g��񓯊��ŃN���[�Y���Acleanup ��
                �������s�Ȃ��DCPS ���C�u�����̒�~�������J�n����D
  
  Arguments:    �Ȃ�
  
  Returns:      SOCL_EINPROGRESS=-26 �Ȃ�N���[�Y������
                SOCL_ESUCCESS   = 0  �Ȃ犮��
 *---------------------------------------------------------------------------*/
int SOCL_CalmDown(void)
{
    int result;

    if (SOCLiUDPSendSocket)
    {
        result = SOCL_CloseAll();

        if (result == SOCL_ESUCCESS)
        {
            (void)SOCL_Close(SOCLiUDPSendSocket);

            if (SOCL_IsClosed(SOCLiUDPSendSocket))
            {
                SOCLiUDPSendSocket = NULL;
            }

            result = SOCL_EINPROGRESS;
        }

        SOCLi_TrashSocket();
    }
    else
    {   // �񓯊� Cleanup �΍�
        if (CPS_CalmDown())
        {
            WCM_SetRecvDCFCallback(NULL);
            result = SOCL_ESUCCESS;
        }
        else
        {
            result = SOCL_EINPROGRESS;
        }
    }

    return result;
}
