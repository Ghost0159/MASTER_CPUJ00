/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - libraries
  File:     socl_list.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: socl_list.c,v $
  Revision 1.5  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.4  2005/09/28 11:44:37  yasu
  �R�[�h�𐮗�
  SOCL_CalmDown() ��ǉ�
  CPS ���C�u�����̐����Ή����܂��Ȃ̂� SOC ���ŏ������J�o�[���Ă���

  Revision 1.3  2005/09/27 14:18:09  yasu
  SOC_Close �̔񓯊�����T�|�[�g

  Revision 1.2  2005/09/01 06:42:55  yasu
  �\�P�b�g�������Ă��邩�ǂ����̔�����ꌳ��

  Revision 1.1  2005/08/18 13:18:49  yasu
  �\�P�b�g�����N���X�g�ɂ�� cleanup �����̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitroWiFi/socl.h>

SOCLSocket*         SOCLiSocketList = NULL;
SOCLSocket*         SOCLiSocketListTrash = NULL;

static SOCLSocket **     SOCLi_SocketGetNextPtr(SOCLSocket ** start, SOCLSocket* socket);
static void SOCLi_SocketRegisterList(SOCLSocket ** next, SOCLSocket* socket);
static void SOCLi_SocketUnregisterList(SOCLSocket ** next, SOCLSocket* socket);

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SocketRegister

  Description:  �\�P�b�g�����X�g�֓o�^����

  Arguments:    socket  �\�P�b�g

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_SocketRegister(SOCLSocket* socket)
{
    SOCLi_SocketRegisterList(&SOCLiSocketList, socket);
}

static void SOCLi_SocketRegisterList(SOCLSocket ** next, SOCLSocket* socket)
{
    socket->next = *next;
    *next = socket;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SocketRegisterTrash

  Description:  �\�P�b�g��p�����X�g�֓o�^����

  Arguments:    socket  �\�P�b�g

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_SocketRegisterTrash(SOCLSocket* socket)
{
    SOCLi_SocketRegisterList(&SOCLiSocketListTrash, socket);
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SocketUnregister

  Description:  �\�P�b�g�����X�g����폜����

  Arguments:    socket  �\�P�b�g

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_SocketUnregister(SOCLSocket* socket)
{
    SOCLi_SocketUnregisterList(&SOCLiSocketList, socket);
}

static void SOCLi_SocketUnregisterList(SOCLSocket ** next, SOCLSocket* socket)
{
    next = SOCLi_SocketGetNextPtr(next, socket);

    if (next)
    {
        *next = socket->next;
    }
}

static SOCLSocket ** SOCLi_SocketGetNextPtr(SOCLSocket ** next, SOCLSocket* socket)
{
    SOCLSocket*     t;

    for (t = *next; t; t = t->next)
    {
        if (t == socket)
        {
            return next;
        }

        next = &t->next;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         SOCLi_SocketUnregisterTrash

  Description:  �\�P�b�g��p�����X�g����폜����

  Arguments:    socket  �\�P�b�g

  Returns:      �Ȃ�
 *---------------------------------------------------------------------------*/
void SOCLi_SocketUnregisterTrash(SOCLSocket* socket)
{
    SOCLi_SocketUnregisterList(&SOCLiSocketListTrash, socket);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_SocketIsInvalid

  Description:  �\�P�b�g���\�P�b�g���X�g�ɓo�^����Ă��鐳���Ȃ��̂��̊m�F

  Arguments:    socket  �\�P�b�g

  Returns:      0 �ȊO �o�^����Ă���  0 �o�^����Ă��Ȃ�
 *---------------------------------------------------------------------------*/
int SOCL_SocketIsInvalid(SOCLSocket* socket)
{
    return((int)socket <= 0) || !SOCLi_SocketGetNextPtr(&SOCLiSocketList, socket);
}

/*---------------------------------------------------------------------------*
  Name:         SOCL_SocketIsInTrash

  Description:  �\�P�b�g���p���\�P�b�g���X�g�ɓo�^����Ă��邩�̊m�F

  Arguments:    socket  �\�P�b�g

  Returns:      0 �ȊO �o�^����Ă���  0 �o�^����Ă��Ȃ�
 *---------------------------------------------------------------------------*/
int SOCL_SocketIsInTrash(SOCLSocket* socket)
{
    return SOCLi_SocketGetNextPtr(&SOCLiSocketListTrash, socket) != NULL;
}
