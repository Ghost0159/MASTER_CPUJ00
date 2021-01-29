/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WBT - libraries
  File:     wbt_api.c

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/

#include	<nitro.h>
#include	<nitro/wbt.h>
#include	<nitro/wbt/context.h>

/*---------------------------------------------------------------------------*
	�ϐ���`
 *---------------------------------------------------------------------------*/

static BOOL wbt_initialize_flag = FALSE;

static WBTContext wbti_command_work[1];

/* 2 �i�̃R�}���h�L���[ */
static WBTCommandList cmd_q[2];


/*---------------------------------------------------------------------------*
	�֐���`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         WBT_PrintBTList

  Description:  �u���b�N��񃊃X�g��OS_Printf�ŕ\������

  Arguments:    none

  Returns:      none
 *---------------------------------------------------------------------------*/
void WBT_PrintBTList(void)
{
    WBTBlockInfoList *list = wbti_command_work->list;
    for (; list != NULL; list = list->next)
    {
        OS_Printf("BTList id = %d\n", list->data_info.id);
        OS_Printf("  data size %d\n", list->data_info.block_size);
        OS_Printf("  uid %s\n", list->data_info.user_id);
        OS_Printf("  info ptr = %p\n", &(list->data_info));
    }
}

/*---------------------------------------------------------------------------*
  Name:         WBT_AidbitmapToAid

  Description:  AID�r�b�g�}�b�v��AID�ɕϊ�����i�ŉ��ʃr�b�g�̂݌���j

  Arguments:    abmp - AID�r�b�g�}�b�v

  Returns:      int  - AID
 *---------------------------------------------------------------------------*/
int WBT_AidbitmapToAid(WBTAidBitmap abmp)
{
    return abmp ? (int)MATH_CTZ(abmp) : -1;
}

/*---------------------------------------------------------------------------*
  Name:         WBT_InitParent

  Description:  �e�@�p�u���b�N�]���������֐�

  Arguments:    send_packet_size - ���M�p�P�b�g�̃T�C�Y
                recv_packet_size - ��M�p�P�b�g�̃T�C�Y
                callback  - ����ǂ���̃��N�G�X�g�������ɂ�����R�[���o�b�N�֐�

  Returns:      none.
 *---------------------------------------------------------------------------*/
void WBT_InitParent(int send_packet_size, int recv_packet_size, WBTCallback callback)
{
    PLATFORM_ENTER_CRITICALSECTION();
    if (!wbt_initialize_flag)
    {
        wbt_initialize_flag = TRUE;
        WBT_InitContext(wbti_command_work, NULL, NULL);
        wbti_command_work->system_cmd.callback = callback;
        /* �R�}���h�v�[���������� */
        MI_CpuFill8(cmd_q, 0x00, sizeof(cmd_q));
        WBT_AddCommandPool(wbti_command_work, cmd_q, sizeof(cmd_q) / sizeof(*cmd_q));
        WBT_StartParent(wbti_command_work, send_packet_size, recv_packet_size);
    }
    PLATFORM_LEAVE_CRITICALSECTION();
}

/*---------------------------------------------------------------------------*
  Name:         WBT_InitChild

  Description:  �q�@�p�u���b�N�]���������֐�

  Arguments:    callback - ����ǂ���̃��N�G�X�g�������ɂ�����R�[���o�b�N�֐�

  Returns:      none.
 *---------------------------------------------------------------------------*/
void WBT_InitChild(WBTCallback callback)
{
    PLATFORM_ENTER_CRITICALSECTION();
    if (!wbt_initialize_flag)
    {
        wbt_initialize_flag = TRUE;
        WBT_InitContext(wbti_command_work, NULL, NULL);
        wbti_command_work->system_cmd.callback = callback;
        /* �R�}���h�v�[���������� */
        MI_CpuFill8(cmd_q, 0x00, sizeof(cmd_q));
        WBT_AddCommandPool(wbti_command_work, cmd_q, sizeof(cmd_q) / sizeof(*cmd_q));
    }
    PLATFORM_LEAVE_CRITICALSECTION();
}

/*---------------------------------------------------------------------------*
  Name:         WBT_End

  Description:  �e�@�q�@���p�u���b�N�]���I���֐�

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WBT_End(void)
{
    PLATFORM_ENTER_CRITICALSECTION();
    if (wbt_initialize_flag)
    {
        wbt_initialize_flag = FALSE;
        wbti_command_work->system_cmd.callback = NULL;
        WBT_ResetContext(wbti_command_work, NULL);
    }
    PLATFORM_LEAVE_CRITICALSECTION();
}

/*---------------------------------------------------------------------------*
  Name:         WBT_SetOwnAid

  Description:  ���ǂ�AID��ݒ肷��

  Arguments:    aid - ���ǂ�AID

  Returns:      none.
 *---------------------------------------------------------------------------*/
void WBT_SetOwnAid(int aid)
{
    WBTContext *const work = wbti_command_work;
    if (WBT_GetAid(work) == -1)
    {
        WBT_StartChild(work, aid);
    }
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetOwnAid

  Description:  ���ǂ�AID�𓾂�

  Arguments:    none.

  Returns:      int - ���ǂ�AID
 *---------------------------------------------------------------------------*/
int WBT_GetOwnAid(void)
{
    const WBTContext *const work = wbti_command_work;
    return WBT_GetAid(work);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_CalcPacketbitmapSize

  Description:  �p�P�b�g��M�ԍ��L�^�p�o�b�t�@�̃T�C�Y���v�Z����
                �i�q�@�̏ꍇ�͐e�@�Ɠ������������ɃR�[�����Ȃ���΂Ȃ�Ȃ��j

  Arguments:    block_size - ��M����u���b�N�̃T�C�Y

  Returns:      int - �p�P�b�g��M�ԍ��L�^�p�o�b�t�@�T�C�Y�i�o�C�g�j
 *---------------------------------------------------------------------------*/
int WBT_CalcPacketbitmapSize(int block_size)
{
    return WBT_GetBitmapLength(wbti_command_work, block_size);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetCurrentDownloadProgress

  Description:  ���݃u���b�N��M�̐i�s�x����������

  Arguments:    block_id       - ��M���̃u���b�NID
                aid            - ���M�����AID
                *current_count - ��M���������p�P�b�g�J�E���g
                *total_count   - �S�p�P�b�g�J�E���g

  Returns:      BOOL - �����^���s
 *---------------------------------------------------------------------------*/
BOOL WBT_GetCurrentDownloadProgress(u32 block_id, int aid, int *current_count, int *total_count)
{
    WBT_GetDownloadProgress(wbti_command_work, block_id, aid, current_count, total_count);
    return (*total_count != 0);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_SetPacketSize

  Description:  �e�@�p����M�p�P�b�g�T�C�Y�ύX�֐��iWBT_InitParent��ɕύX����ꍇ�Ɏg�p�j

  Arguments:    send_packet_size - ���M�p�P�b�g�T�C�Y
                recv_packet_size - ��M�p�P�b�g�T�C�Y

  Returns:      BOOL   - FALSE �T�C�Y�ύX�Ɏ��s
                         TRUE  �T�C�Y�ύX�ɐ���
 *---------------------------------------------------------------------------*/
BOOL WBT_SetPacketSize(int send_packet_size, int recv_packet_size)
{
    return WBT_SetPacketLength(wbti_command_work, send_packet_size, recv_packet_size);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_NumOfRegisteredBlock

  Description:  �o�^�ς݂̃u���b�N����Ԃ�

  Arguments:    none.

  Returns:      WBTBlockNumEntry - �u���b�N��
 *---------------------------------------------------------------------------*/
int WBT_NumOfRegisteredBlock(void)
{
    return WBT_GetRegisteredCount(wbti_command_work);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_MpParentSendHook

  Description:  �e�@�p�̑��M�f�[�^�����֐�

  Arguments:    sendbuf   - ���M�o�b�t�@
                send_size - ���M�o�b�t�@�T�C�Y

  Returns:      int - �u���b�N�]�����M�T�C�Y
 *---------------------------------------------------------------------------*/
int WBT_MpParentSendHook(void *sendbuf, int send_size)
{
    SDK_ASSERT(wbt_initialize_flag);
    return WBT_CallPacketSendHook(wbti_command_work, sendbuf, send_size, TRUE);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_MpChildSendHook

  Description:  �q�@�̑��M�f�[�^�����֐�

  Arguments:    sendbuf   - ���M�o�b�t�@
                send_size - ���M�o�b�t�@�T�C�Y

  Returns:      int - �u���b�N�]�����M�T�C�Y
 *---------------------------------------------------------------------------*/
int WBT_MpChildSendHook(void *sendbuf, int send_size)
{
    SDK_ASSERT(wbt_initialize_flag);
    return WBT_CallPacketSendHook(wbti_command_work, sendbuf, send_size, FALSE);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_MpParentRecvHook

  Description:  �e�@�p�̎�M�f�[�^����͂���֐�

  Arguments:    recv_buf  - ��M�o�b�t�@
                recv_size - ��M�o�b�t�@�T�C�Y
                aid       - ��M����ǂ�AID

  Returns:      none.
 *---------------------------------------------------------------------------*/
void WBT_MpParentRecvHook(const void *buf, int size, int aid)
{
    SDK_ASSERT(wbt_initialize_flag);
    WBT_CallPacketRecvHook(wbti_command_work, aid, buf, size);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_MpChildRecvHook

  Description:  �q�@�p�̎�M�f�[�^����͂���֐�

  Arguments:    recv_buf  - ��M�o�b�t�@
                recv_size - ��M�o�b�t�@�T�C�Y

  Returns:      none.
 *---------------------------------------------------------------------------*/
void WBT_MpChildRecvHook(const void *buf, int size)
{
    SDK_ASSERT(wbt_initialize_flag);
    WBT_CallPacketRecvHook(wbti_command_work, 0, buf, size);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_RegisterBlock

  Description:  ���M�i�z�M�j�\�ȃu���b�N��o�^����

  Arguments:    block_info_list - �u���b�N���o�^�p�\����
                block_id        - �u���b�NID
                user_id         - ���[�U�[��`���
                data_ptr        - �f�[�^�ւ̃|�C���^�i�m�t�k�k�̏ꍇ�A����ǂ���v�������邽�т�
                                  ���[�U�[�R�[���o�b�N��������A���[�U�[�͂��̃R�[���o�b�N�֐�����
                                  �f�[�^�|�C���^��ݒ�ł���j
                data_size       - �f�[�^�T�C�Y
                permission_bmp  - �z�M���r�b�g�}�b�v�i�O�ŋ��u�\��v�j

  Returns:      BOOL   - TRUE  �o�^����
                         FALSE block_id�͊��ɓo�^�ς�
 *---------------------------------------------------------------------------*/
BOOL
WBT_RegisterBlock(WBTBlockInfoList *block_info_list, WBTBlockId block_id, const void *user_id,
                  const void *data_ptr, int data_size, WBTAidBitmap permission_bmp)
{
    (void)permission_bmp;
    SDK_ASSERT(wbt_initialize_flag);
    WBT_RegisterBlockInfo(wbti_command_work, block_info_list, block_id, user_id,
                          data_ptr, data_size);
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         WBT_UnregisterBlock

  Description:  �u���b�N�̔z�M�o�^���͂���

  Arguments:    block_id - �z�M��~����u���b�NID

  Returns:      WBTBlockInfoList - �u���b�N���o�^�p�\����
 *---------------------------------------------------------------------------*/
WBTBlockInfoList *WBT_UnregisterBlock(WBTBlockId block_id)
{
    SDK_ASSERT(wbt_initialize_flag);
    return WBT_UnregisterBlockInfo(wbti_command_work, block_id);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_RequstSync

  Description:  ����ǂƓ��������i�u���b�N�]���̊J�n���ɕK���Ă΂Ȃ���΂Ȃ�Ȃ��j

  Arguments:    target   - ��������鑊��ǁiAID�r�b�g�}�b�v�Ŏw�肷��j
                callback - �������������ɂ�����R�[���o�b�N�֐�

  Returns:      BOOL   - FALSE �ȑO�̃R�}���h�����s��
                         TRUE  �R�}���h���s����
 *---------------------------------------------------------------------------*/
BOOL WBT_RequestSync(WBTAidBitmap target, WBTCallback callback)
{
    WBTCommandList *list = WBT_AllocCommandList(wbti_command_work);
    if (list)
    {
        list->command.callback = callback;
        WBT_CreateCommandSYNC(wbti_command_work, list);
        WBT_PostCommand(wbti_command_work, list, target, NULL);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetBlockInfo

  Description:  �u���b�N�����擾����

  Arguments:    target           - �u���b�N����v�����鑊��ǁiAID�r�b�g�}�b�v�Ŏw�肷��j
                block_info_no    - �u���b�N���ԍ�
                block_info_table - �擾�����u���b�N�����i�[����o�b�t�@�ւ̃|�C���^�e�[�u��
                callback         - �u���b�N���擾��ɂ�����R�[���o�b�N�֐�

  Returns:      BOOL   - FALSE �ȑO�̃R�}���h�����s��
                         TRUE  �R�}���h���s����
 *---------------------------------------------------------------------------*/
BOOL
WBT_GetBlockInfo(WBTAidBitmap target, int block_info_no, WBTBlockInfoTable *block_info_table,
                 WBTCallback callback)
{
    WBTCommandList *list = WBT_AllocCommandList(wbti_command_work);
    if (list)
    {
        list->command.callback = callback;
        WBT_CreateCommandINFO(wbti_command_work, list, block_info_no, block_info_table);
        WBT_PostCommand(wbti_command_work, list, target, NULL);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_GetBlock

  Description:  �u���b�N���擾����
                
  Arguments:    target         - �u���b�N��v�����鑊��ǁiAID�r�b�g�}�b�v�Ŏw�肷��j
                block_id       - �u���b�NID
                recv_buf_table - ��M�����u���b�N���i�[����o�b�t�@�ւ̃|�C���^�e�[�u��
                recv_size      - ��M�u���b�N�T�C�Y
                p_bmp_table    - �p�P�b�g��M�ԍ��L�^�p�o�b�t�@�ւ̃|�C���^�e�[�u��
                callback       - �u���b�N�擾��ɂ�����R�[���o�b�N�֐�

  Returns:      BOOL   - FALSE �ȑO�̃R�}���h�����s��
                         TRUE  �R�}���h���s����
 *---------------------------------------------------------------------------*/
BOOL
WBT_GetBlock(WBTAidBitmap target, WBTBlockId block_id, WBTRecvBufTable *recv_buf_table,
             u32 recv_size, WBTPacketBitmapTable *p_bmp_table, WBTCallback callback)
{
    WBTCommandList *list = WBT_AllocCommandList(wbti_command_work);
    if (list)
    {
        list->command.callback = callback;
        WBT_CreateCommandGET(wbti_command_work, list, block_id, recv_size, recv_buf_table, p_bmp_table);
        WBT_PostCommand(wbti_command_work, list, target, NULL);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_PutUserData

  Description:  ����ǂɂX�o�C�g�ȉ��̃f�[�^�𑗂�
                
  Arguments:    target    - ����ǁiAID�r�b�g�}�b�v�Ŏw�肷��j
                user_data - ���肽���f�[�^�̃|�C���^
                size      - �f�[�^�T�C�Y
                callback  - �R�[���o�b�N�֐�

  Returns:      BOOL   - FALSE �ȑO�̃R�}���h�����s��
                         TRUE  �R�}���h���s����
 *---------------------------------------------------------------------------*/
BOOL WBT_PutUserData(WBTAidBitmap target, const void *user_data, int size, WBTCallback callback)
{
    WBTCommandList *list = WBT_AllocCommandList(wbti_command_work);
    if (list)
    {
        list->command.callback = callback;
        WBT_CreateCommandMSG(wbti_command_work, list, user_data, (u32)size);
        WBT_PostCommand(wbti_command_work, list, target, NULL);
    }
    return (list != NULL);
}

/*---------------------------------------------------------------------------*
  Name:         WBT_CancelCurrentCommand

  Description:  ���ݔ��s���̂v�a�s�R�}���h�𒆒f����
                
  Arguments:    target    - ����ǁiAID�r�b�g�}�b�v�Ŏw�肷��j

  Returns:      BOOL   - FALSE �L�����Z������R�}���h���Ȃ�
                         TRUE  �L�����Z������
 *---------------------------------------------------------------------------*/
BOOL WBT_CancelCurrentCommand(WBTAidBitmap cancel_target)
{
    SDK_ASSERT(wbt_initialize_flag);
    return (WBT_CancelCommand(wbti_command_work, cancel_target) != 0);
}


/*---------------------------------------------------------------------------*
  $Log: wbt_api.c,v $
  Revision 1.2  2007/04/10 23:50:11  yosizaki
  enable WBT_AidbitmapToAid().

  Revision 1.1  2007/04/10 08:19:45  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
